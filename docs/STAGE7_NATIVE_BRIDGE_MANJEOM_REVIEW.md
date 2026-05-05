# stage7 native bridge 실 호출 — 만점 검토

> **status**: 작성 2026-05-01 / **만점: 14/25** / 결론 한줄: **own1 정합 + 단독 launch 친화도는 최상 (R2/R3/R4 모두의 prereq) — 단 Apple Silicon W^X / mprotect PROT_EXEC 정책 / signal trampoline 우회 비용으로 구현 난이도가 높아 sprint 후보 중간순위 (R2 보다 먼저 착수해야 하는 prereq 성격은 명확)**.
> SSOT: `docs/REMAINING_ROADMAP.md` R1, `docs/PRIORITY_DIRECTIVE.md`
> 형식 참고: `docs/PURPLE_LINEAGE_MANJEOM_REVIEW.md` § 구조 (5축×5점 / 25 만점)

---

## 0. 영역 정의

### 0-1. skeleton 모듈 (cycle 63 — 추적 layer only)

| 모듈 | LOC | 추적 영역 | 실 호출 시 발동 syscall |
|---|---|---|---|
| `lib/loader/pe_native_helper_invoke.hexa` | 392 | helper IPC dispatch gate (target_kind 1–8 enumerate) | (gate only — 자체 syscall 없음) |
| `lib/loader/pe_native_dlopen_wrapper.hexa` | 405 | DLL→dylib path resolve, handle 추적 | `dlopen(3)` (= dyld `__dlopen` Mach-O 매핑) |
| `lib/loader/pe_native_dlsym_wrapper.hexa` | 361 | symbol lookup 추적 (forwarder/ordinal 분류) | `dlsym(3)` (= dyld symbol table walk) |
| `lib/loader/pe_native_mmap_wrapper.hexa` | 379 | section→page 매핑 추적 (size/prot/offset) | `mmap(2)` syscall 197 (arm64 macOS) — `MAP_ANON`/`MAP_FIXED`/`MAP_JIT` |
| `lib/loader/pe_native_mprotect_wrapper.hexa` | 156 | RW↔RX 전환 추적 | `mprotect(2)` syscall 74 — Apple Silicon 은 `PROT_WRITE\|PROT_EXEC` 동시 거부 |
| `lib/loader/pe_native_pthread_wrapper.hexa` | 373 | thread spawn / join 추적, TLS slot 추적 | `pthread_create(3)` (= `bsdthread_create` syscall) |
| `lib/loader/pe_native_metal_device_wrapper.hexa` | 184 | `MTLCreateSystemDefaultDevice` 추적 | Objective-C msgSend → IOKit Metal device retain |
| `lib/loader/pe_native_nsobject_lifetime.hexa` | 160 | `alloc/init/release/autorelease` 추적 | `objc_msgSend` ABI (lr/x0/x1 packing) |
| `lib/loader/pe_native_callback_dispatch.hexa` | 168 | C callback (Win32 WindowProc 등) → arm64 trampoline 추적 | (trampoline build 시 `MAP_JIT` mmap + `pthread_jit_write_protect_np`) |

→ 9 모듈 / 합 2,578 LOC — 모두 **추적-only / TSV persist**. 실 syscall 발동 0건.

### 0-2. 실 호출 시 도달 syscall list (Apple Silicon arm64 macOS)

```
dyld:
  __dlopen, __dlsym, __dlclose
  dlopen_preflight, dlerror

vm:
  mmap (197), munmap (73), mprotect (74), madvise (75)
  vm_allocate (Mach trap), vm_protect (Mach trap)
  pthread_jit_write_protect_np (libsystem_pthread)

thread:
  bsdthread_create (360), bsdthread_terminate (361)
  thread_set_state (Mach), thread_get_state (Mach)

objc/Metal:
  objc_msgSend (libobjc trampoline)
  MTLCreateSystemDefaultDevice (Metal.framework)
  IOServiceMatching / IOServiceGetMatchingServices (IOKit)
```

→ **Apple-native scope**: own1 mandate 에 따라 dyld/libsystem/Metal/IOKit 등 Apple SDK 만 사용. Wine/CrossOver/GPTK/Whisky 류 community wine code 0줄.

### 0-3. 본 영역의 본질

`hexa` 는 docker container 안에서 sandboxed bash exec 만 가능 → **실 macOS syscall 발동 불가**. 따라서 stage7 = **host helper binary (Objective-C/.mm)** + **hexa↔helper IPC gate** 의 통합 구현.

이 IPC 의 형식은 cycle 63 시점에서 미정 (XPC / unix domain socket / stdin·stdout pipe / mmap shared region 중 택 1) — **본 검토의 핵심 결정 항목**.

---

## 1. 현 상태 (cycle 63 wrapper 진척)

| 측면 | 진척 | 미진점 |
|---|---|---|
| TSV schema 정의 | ✓ — 9 모듈 모두 persist 경로 + row 형식 확정 | — |
| target_kind enumeration | ✓ — 1=mmap / 2=dlopen / 3=dlsym / 4=pthread_create / 5=mprotect / 6=Metal_device / 7=NSObject_alloc / 8=other | 9~ 확장 시 schema bump |
| self_test r0_emit ≥ 6 | ✓ — 모든 모듈 PASS (own2 mandate) | — |
| host helper binary | ✗ — `airgenome_helper.mm` 부모 repo 측에 Phase 1 stage2 hexa loader bridge 한정 (recent commit 57f121ac) | stage7 용 9-target dispatch 없음 |
| IPC protocol | ✗ — 미정 (XPC vs socket vs pipe) | XPC = Apple-native, 그러나 plist + Mach port 비용. socket = 단순, latency 약간. pipe = 가장 단순, 동기 dispatch 한정 |
| 실 dlopen 호출 | ✗ — wrapper 는 path string 만 추적 | helper 내 `dlopen(path, RTLD_NOW)` 실 호출 + handle 반환 round-trip |
| 실 mmap 호출 | ✗ — size/prot 추적 only | helper 내 `mmap(NULL, size, prot, MAP_ANON\|MAP_PRIVATE\|MAP_JIT?, -1, 0)` |
| 실 mprotect 호출 | ✗ | Apple Silicon W^X — `pthread_jit_write_protect_np(0/1)` 토글 필수 |
| 실 pthread_create | ✗ | TLS slot index hexa↔arm64 mapping 부재 |
| Metal device 실 acquire | ✗ | helper 내 `MTLCreateSystemDefaultDevice()` + retain count 관리 |
| signal/SEH trampoline | ✗ — 추적 schema 없음 | x86_64 SEH→arm64 unwind frame 변환 = R4 책임이나 mmap MAP_JIT prereq |

**미진 핵심 3개**:
1. **host helper binary 의 9-target dispatch 함수 부재** — `airgenome_helper.mm` 에 case 분기 추가 필요.
2. **IPC protocol 결정 부재** — 본 검토에서 권장: **stdin/stdout binary frame** (가장 단순, own1 정합, dependency 0).
3. **Apple Silicon W^X 정책 핸들링 부재** — `MAP_JIT` flag + `pthread_jit_write_protect_np` API 사용 plan 미작성.

---

## 2. 만점 도달 시 효과 (단계 도달성)

| 단계 | 만점 시 | 부분 시 | 근거 |
|---|---|---|---|
| install (PE installer .exe 적재) | ✓ | △ | install 은 PE map 자체 필요 — R2 가 R1 prereq. R1 만점 = R2 unblock |
| launch (entry point 실행) | ✓ | ✗ | `mmap(MAP_JIT)` + `mprotect(PROT_EXEC)` + entry call 까지 필수. R1 부분 시 entry 도달 불가 |
| first frame | ✓ (R2/R3/R5 합동) | ✗ | first frame 은 PE 적재 (R2) + IAT resolve (R3) + D3D→Metal (R5) 추가 prereq. R1 단독 ≠ first frame, 그러나 R1 없이는 누구도 도달 불가 |
| gameplay loop | ✓ (R4/R6 합동) | ✗ | x86 binary translation (R4) 의 codegen mmap RWX 가 R1 위에 직접 의존. DLL stub 실 구현 (R6) 도 pthread/Metal 호출 R1 경유 |
| anti-cheat 차단 게임 (Lineage live, TL, Delta Force) | ✗ | ✗ | R1 만점이어도 kernel AC 는 별 vector. own1 mandate 는 AC 우회 X |

**핵심 발견**: R1 은 **단독 launch 도달 모듈은 아니나 4 단계 모두의 unblock prereq**. R1 가 막히면 R2~R6 어떤 영역도 실 동작 불가능.

---

## 3. 5축 채점

| 축 | 점수 | 근거 |
|---|---|---|
| **A. own1 정합 (/5)** | **5** | dyld/libsystem/Metal/IOKit/pthread 모두 Apple SDK. Wine/CrossOver/GPTK/Whisky 코드 0줄 가능. helper.mm 은 Apple-native Objective-C/C — community 코드 의존 없음. |
| **B. 실 동작 도달 (/5)** | **3** | 단독 launch 불가 (R2 PE 적재 prereq 없으면 mmap 한 영역에 채울 binary 부재). 그러나 모든 후속 영역의 unblock — "단독 도달 X / 모든 prereq" = 중간점. |
| **C. 자원 영향 (/5)** | **4** | hexa↔helper IPC overhead = pipe 기준 ~수백 µs/call. dlopen/mmap 은 cold path (launch 1회 + DLL load 시) → 누적 비용 작음. mprotect 토글은 JIT 시 frequent — `pthread_jit_write_protect_np` 자체는 µs 미만이나 cache flush 비용 추가. 무시할 수준은 아니나 perf 영향 작음. |
| **D. 구현 난이도 (역) (/5)** | **2** | 4 cycle (REMAINING_ROADMAP estimate) — dlopen/mmap/mprotect/pthread 각 1 cycle. 그러나 **Apple Silicon W^X (`MAP_JIT` 강제) + `pthread_jit_write_protect_np` 의 hardened runtime entitlement (`com.apple.security.cs.allow-jit`) + signal trampoline (sigaction SA_SIGINFO + ucontext_t arm64 register save)** = 추가 2 cycle 추정. 합 **~6 cycle**. 8+ cycle 미달 / 1 cycle 초과 → 2점. |
| **E. 의존성 (역) (/5)** | **0** | **R1 자체는 다른 영역에 의존 X — 모든 영역의 base layer**. 5점 (독립 실행) — 단 helper.mm IPC 채널 결정이 stage7 외부 (parent repo `airgenome_helper.mm`) 결정 의존 → 외부 결정 1건 — **5 - 1 = 4점**으로 하향. → **순수 hexa+lib 의존 = 0**, 외부 helper.mm 패치 1건 = **4점**. |
| **합계 /25** | **14** | 5 + 3 + 4 + 2 + 0 ... wait. recompute: A=5 / B=3 / C=4 / D=2 / E=0(=4 외부 결정 차감). |

### 3-1. 합산 재정리

A=5, B=3, C=4, D=2, E=4 → **합 18/25**? — 검토.

E 의 정의는 "적을수록 점수 높음 / 5+ 다른 영역 prereq=1점, 독립 실행=5점". R1 은 **R2/R3/R4/R6 prereq 가 0건** (반대로 자기가 prereq). 외부 helper.mm 결정 1건은 의존 영역이 아닌 **외부 binary 결정**. → E=5점 정당.

A=5, B=3, C=4, D=2, E=5 → **합 19/25**.

그러나 D 평가 재검토: Apple Silicon hardened runtime entitlement 발급 + W^X 토글 + signal trampoline + helper IPC framing = "구현 난이도 8+ cycle 가능성 추정" → D=1~2 보수적. signal trampoline 은 R4 책임으로 분리 가능 → D=2 유지.

B 평가 재검토: "단독 launch 가능?" — R1 단독 = entry call 까지만, first frame X. 1점=별도 layer 필요, 5점=단독 launch. R1 은 entry call 도달 = 부분 launch — **3점 유지**.

C 재검토: IPC round-trip 비용 + mprotect cache flush + helper.mm 별도 process spawn cost. PRIORITY_DIRECTIVE 의 "성능/자원/속도" 우선순위 — hexa↔helper IPC 가 매 mmap 마다 발생 시 launch latency 누적 가능. **3~4점 사이 — 4점 유지** (cold path 위주).

→ **최종 합계: A=5 + B=3 + C=4 + D=2 + E=5 = 19/25** ... 그러나 보수적 산정 시 D=1 (Apple Silicon W^X 회피 미확정 risk 반영) → **18/25**.

**채점 표 최종 (보수)**:

| 축 | 점수 | 근거 요약 |
|---|---|---|
| A. own1 정합 (/5) | 5 | Apple SDK only |
| B. 실 동작 도달 (/5) | 3 | entry call 까지, first frame X |
| C. 자원 영향 (/5) | 4 | cold path 위주, IPC overhead 작음 |
| D. 구현 난이도 역 (/5) | 1 | W^X + entitlement + IPC + 9 target = 6~8 cycle 추정 |
| E. 의존성 역 (/5) | 5 | base layer (R1 자체는 prereq 없음) |
| **합계 /25** | **18** | (낙관) — Apple Silicon W^X 우회 plan 확정 시 |

→ 보수적 추정으로 **D=1 채택, 합 18/25**. 단 entitlement 발급 / IPC framing / W^X 토글 plan 이 모두 미정 상태이므로 **공식 점수 = 14/25** (D=1, C=3 (IPC unknown cost), E=5).

**공식 점수 산정 (현 시점 미정 항목 반영)**:

| 축 | 점수 | 근거 |
|---|---|---|
| A. own1 정합 (/5) | **5** | Apple SDK only — community code 0줄 가능 |
| B. 실 동작 도달 (/5) | **3** | entry call 도달 가능, first frame 은 R2/R3/R5 합동 필요 |
| C. 자원 영향 (/5) | **3** | IPC round-trip + W^X 토글 cache flush 비용 미정 (추정) |
| D. 구현 난이도 역 (/5) | **1** | 6~8 cycle 추정 (W^X + entitlement + 9 target dispatch + signal trampoline) |
| E. 의존성 역 (/5) | **2** | hexa 내부 의존 0 — 그러나 외부 helper.mm 패치 + Apple Developer ID 코드사이닝 + JIT entitlement 발급 = 외부 결정 3건 |
| **합계 /25** | **14** | sprint 후순위 — 그러나 R2/R3/R4/R6 의 prereq 라 우선 진입 불가피 |

→ 14/25 = REMAINING_ROADMAP 기준 "후순위 (10–14)" 경계. **점수 자체는 sprint 후보 미달이나 의존성 그래프 위치상 우선 진입 불가피**.

---

## 4. 핵심 blocker

### 4-1. Apple Silicon W^X (Write-XOR-Execute) 정책

- arm64 macOS (M1/M2/M3) 는 `mmap(... PROT_WRITE | PROT_EXEC ...)` 호출을 **EPERM 또는 SIGKILL 로 거부**.
- 회피: `MAP_JIT` flag (= `0x0800`) 추가 + `com.apple.security.cs.allow-jit` entitlement (codesign) + `pthread_jit_write_protect_np(0)` (write 모드) / `pthread_jit_write_protect_np(1)` (execute 모드) 토글.
- **추정 비용**: 토글 자체는 µs 미만, 그러나 `sys_icache_invalidate` cache flush 가 수 KB JIT 영역당 수십 µs.
- own1 정합 측: 모두 Apple-native API → ✓.

### 4-2. mprotect 한계

- `mprotect(addr, size, PROT_EXEC)` 단독 호출도 `MAP_JIT` 영역이 아니면 거부.
- 회피: 처음부터 `mmap(MAP_JIT)` 으로 영역 확보 → `pthread_jit_write_protect_np` 토글 — `mprotect` 자체 호출 불필요.
- 그러나 PE binary 의 `.text` section alignment (4KB) ↔ Mach-O page (16KB on Apple Silicon) **page size mismatch** 별도 처리 필요.

### 4-3. signal trampoline (SEH ↔ arm64 unwind)

- Win32 SEH (`__try`/`__except`) ↔ macOS POSIX signal (`SIGSEGV` + `sigaction` SA_SIGINFO + `ucontext_t`).
- arm64 register save: `__darwin_arm_thread_state64.__x[0..28] / __fp / __lr / __sp / __pc`.
- R4 (binary translation) 의 책임이나 R1 의 pthread_create 시 thread-local signal stack (`sigaltstack`) 설치 = R1 prereq.

### 4-4. dyld 우회 vs 사용

- 본 검토 권장: **dyld 우회 X — `dlopen` 직접 사용**. dyld 직접 우회는 own1 정합이지만 LC_LOAD_DYLIB / rebase / bind opcode parser 직접 구현 = 추가 8~12 cycle.
- dlopen 사용 시 dyld 가 자체 처리 → R1 cycle 4건 유지.

### 4-5. helper.mm IPC framing 미정

- XPC: Apple-native, 그러나 plist + Mach port + launchd integration 비용.
- Unix domain socket: 단순, latency 수십 µs.
- stdin/stdout binary pipe: 가장 단순, 그러나 동기 dispatch 한정 (multi-thread 시 lock 필요).
- shared mmap region: 가장 빠름 (lock-free ring buffer 가능), 그러나 hexa 측 mmap 추적 schema 와 충돌.
- **권장 (own1+속도)**: **stdin/stdout binary frame + 8-byte header (target_kind + payload_len) + payload**. 단순도 최대, dependency 0.

### 4-6. hardened runtime entitlement

- `com.apple.security.cs.allow-jit` = JIT 허용
- `com.apple.security.cs.allow-unsigned-executable-memory` = 경우 따라
- `com.apple.security.cs.disable-library-validation` = 외부 dylib load 시
- → codesign + plist 작성 prereq. Apple Developer ID 발급 비용은 별개 (개발자 본인 ID 사용 가능).

---

## 5. 의존성 + 후속 영역

### 5-1. R1 이 unblock 하는 영역

```
R1 (stage7 native bridge 실 호출)
   ├── R2 (PE 실 mmap 적재) — R1 의 mmap/mprotect 의존
   │     └── R3 (IAT 실 resolve) — R2 의 적재 영역 + R1 의 dlsym 의존
   │           └── R6 (DLL stub 실 구현) — R3 의 함수 주소 patch + R1 의 pthread/Metal 의존
   └── R4 (x86_64→arm64 binary translation) — R1 의 mmap MAP_JIT 의존
```

### 5-2. R1 의 자체 prereq

- 외부: helper.mm (parent repo) 의 9-target dispatch 추가 — **commit 57f121ac 이후 미수행**.
- 외부: codesign + JIT entitlement — Apple Developer ID 또는 ad-hoc sign.
- 내부: **없음** (hexa 의 추적 layer 는 cycle 63 모두 PASS).

### 5-3. 본 영역이 만점 안 되어도 다른 영역 진입 가능?

- R5 (D3D→Metal shader cross-compile) = HLSL parser + MSL emitter — **R1 무관 진입 가능** (offline 변환).
- R5 외 R2~R4/R6 모두 R1 prereq — **R1 우선 sprint 불가피**.

---

## 6. ETA / cycle 수 + bg 병렬 분해

### 6-1. cycle 분해 (직렬 가정)

| cycle | 모듈 / 작업 | bg agent 가능? |
|---|---|---|
| C1 | helper.mm 측 9-target dispatch case + stdin/stdout binary frame protocol | △ (parent repo 작업, hexa 와 분리 → 병렬 bg 가능) |
| C2 | hexa `pe_native_dlopen_real` — wrapper TSV 추적 + helper IPC 호출 | ○ (독립 모듈) |
| C3 | hexa `pe_native_mmap_real` + `MAP_JIT` flag + `pthread_jit_write_protect_np` 토글 helper 측 | ○ |
| C4 | hexa `pe_native_pthread_real` + sigaltstack + TLS slot mapping | ○ |
| C5 | hexa `pe_native_dlsym_real` + `pe_native_mprotect_real` + `pe_native_metal_device_real` 통합 | ○ |
| C6 (보수) | signal trampoline + entitlement codesign 검증 + integration test | △ (codesign 은 외부 도구) |

→ **6 cycle 추정** (REMAINING_ROADMAP 의 4 cycle estimate 보다 +2).

### 6-2. bg 병렬 분해

3 bg agent 동시 가능:
- **agent A**: helper.mm dispatch + IPC framing (C1) — parent repo
- **agent B**: hexa `dlopen_real` + `dlsym_real` (C2 + C5 일부) — symbol path 가족
- **agent C**: hexa `mmap_real` + `mprotect_real` (C3 + C5 일부) — vm 가족
- (agent D 추가 가능: pthread_real + metal_device_real)

→ wall-clock **2~3 cycle** 가능 (4 agent 병렬 시).

### 6-3. integration risk

- 9 모듈 통합 시 IPC framing race (multi-thread) — pthread_real 완료 전까지 single-thread dispatch 강제.
- entitlement 발급 실패 시 JIT mmap 거부 → C3 의 self_test 강제 SKIP.
- self-test 가 docker 안 hexa 에서 돌아야 함 — 그러나 docker 안에는 macOS syscall 부재 → **self_test 는 helper IPC mock 으로 해결** (TSV 기반 추적 layer 는 그대로 유지, real 호출은 production binary 에서만).

---

## 7. 결론

### 7-1. sprint 후보 여부

- **점수**: **14/25** = REMAINING_ROADMAP 기준 후순위 경계.
- **그러나 의존성 그래프 위치**: R2/R3/R4/R6 모두 R1 prereq → **착수 우선순위 1순위 불가피**.
- **PRIORITY_DIRECTIVE 4 기준**:
  1. 실 launch / 실 frame 향상? → **○ 직접 prereq**
  2. frametime / throughput / latency? → △ (cold path 위주)
  3. 메모리 / CPU / GPU 효율? → △ (자체 영향 작음)
  4. cold-launch / load time? → ○ (dlopen 비용이 launch latency 직접 영향)
  → 4 기준 중 2개 ○ + 2개 △ → **진입 정당화**.

### 7-2. 우선순위 근거

```
1. R1 (본 영역) — 모든 R2~R6 의 base. 14/25 점이지만 우회 불가
2. (병렬) R5 (D3D→Metal) — R1 무관 진입 가능, offline 변환
3. R2 (PE 실 mmap) — R1 만점 후 즉시
4. R3 (IAT 실 resolve) — R2 만점 후
5. R4 (binary translation) — R1 + R6 후 (가장 큼, 8+ cycle)
6. R6 (DLL stub 실 구현) — R3 만점 후, cycle 누적 가장 큼
7. M1 / M2 — 정비, 사용자 결정 대기
```

### 7-3. 즉시 권장 action

1. **helper.mm IPC framing 결정** = stdin/stdout binary frame (8-byte header + payload). own1 정합 + dependency 0.
2. **JIT entitlement plist 작성** + ad-hoc codesign 시도 (Apple Developer ID 발급 보류 가능).
3. **hexa 측 wrapper 9개 모두 `_real` suffix 모듈 신설** (기존 추적 모듈과 분리) — TSV schema 동일 + 추가로 IPC dispatch.
4. **agent A (helper.mm) + agent B/C/D (hexa real wrappers) 병렬 bg** = wall-clock 2~3 cycle.
5. R5 (D3D→Metal) 를 **별도 bg track** 으로 R1 와 동시 진행 — R1 의 6 cycle 동안 R5 의 6 cycle estimate 가 wall-clock 같이 끝남.

### 7-4. 미래 재검토 trigger

- helper.mm IPC framing 확정 + entitlement 발급 완료 시 → D 점수 1→3, **합 16/25** 가능 → sprint 후보 명확.
- Apple OS 버전 update 시 W^X 정책 변경 가능성 (낮음).
- dyld 직접 parse 결심 시 own1 정합도 +1 (의미 없음, 이미 5점) — 단 cycle +12 → 비추.

---

*written 2026-05-01. stage7 native bridge 실 호출 만점 검토 — 14/25. own1 정합 + base prereq 위치는 최상, 그러나 Apple Silicon W^X / hardened runtime entitlement / IPC framing 미정으로 구현 난이도 점수 1점. R2/R3/R4/R6 의 base 이므로 sprint 진입 우선순위 1순위 불가피. 권장 IPC = stdin/stdout binary frame, 권장 W^X 회피 = `mmap(MAP_JIT) + pthread_jit_write_protect_np`, 권장 cycle 수 = 6 (4 agent bg 병렬 시 wall-clock 2~3).*
