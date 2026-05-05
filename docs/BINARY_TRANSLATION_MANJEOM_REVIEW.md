# R4 — x86_64 → arm64 Binary Translation 만점 검토

> **status**: 검토일 **2026-05-01** / 잔여 로드맵 R4 (REMAINING_ROADMAP.md 항목 4)
> 작성: 2026-05-01
> 채점 rubric: REMAINING_ROADMAP.md § "만점 검토 채점 축" — 5축 × 5점 / **/25**
> own 1 정의: SCORING.md §A — `hexa PE loader + macOS syscalls + Apple D3DMetal binary + Rosetta 2 + DIY Win32 shim. Wine/CrossOver/community 0 줄`
> 결론 한줄: **시나리오 B (PE → Mach-O thin shim → Rosetta 2 위임) 가 own1 정합 + 실 동작 도달 양면에서 최고 18/25. 시나리오 A (DIY x86 decoder + arm64 emit) 는 own1 5/5 만점이지만 SIMD/SMC/JIT entitlement 통합 시 16+ cycle ETA 로 D 축 1/5 → 12/25. 시나리오 C (interpret-first + B-fallback hot loop) 가 first-frame 도달까지의 점진 path 로 17/25**.

---

## ⚠️ 0.5 own1 정의 정합성 — Rosetta 2 활용 가능성 (CRITICAL 명확화)

### 0.5-1. Rosetta 2 = own1 허용 영역 (재확인)

SCORING.md §A 의 정확한 own1 정의:

> "Apple-only stack: hexa PE loader + macOS syscalls + Apple D3DMetal binary + **Rosetta 2** + DIY Win32 shim. Wine/CrossOver/community 0 줄."

→ **Rosetta 2 는 Apple 의 자체 binary translation 으로 own1 허용**.
→ 금지 대상은 **community Wine/CrossOver/Whisky/GPTK 코드** 이지 Apple binary translator 자체가 아님.
→ user MEMORY.md feedback `own1_no_third_party_wine.md` 도 Rosetta 2 를 trigger 로 삼지 않음 (third-party Wine 만 enforcement 대상).

### 0.5-2. 그러나 Rosetta 2 직접 활용 불가능 (PE format mismatch)

| 측면 | Rosetta 2 (Apple 공식) | gamebox 요구 |
|---|---|---|
| 입력 binary 형식 | **Mach-O x86_64** (또는 dyld_shared_cache) | **PE32+ x86_64 (PE/COFF)** |
| 호출 진입점 | `LC_MAIN` / `LC_UNIXTHREAD` | PE `AddressOfEntryPoint` (RVA 기반) |
| import resolve | dyld + `LC_LOAD_DYLIB` | IAT (Import Address Table, INT/IAT 페어) |
| TLS | dyld TLV (`__thread_vars`/`__thread_data`) | PE `IMAGE_TLS_DIRECTORY` (callback array) |
| relocation | Mach-O LC_DYSYMTAB (S_NON_LAZY_SYMBOL_POINTERS 등) | PE `.reloc` (IMAGE_REL_BASED_DIR64 등) |
| exception | DWARF/compact unwind | PE/SEH (`.pdata`/`.xdata`, `RUNTIME_FUNCTION`) |
| signal | BSD signal/Mach exception | Windows VEH/VCH/SEH chain |

→ **PE 를 Rosetta 2 입력으로 직접 던질 수 없음** — Rosetta 2 는 PE 헤더를 인식 안함.
→ 따라서 가능 시나리오 3 가지 (A / B / C) 분기.

### 0.5-3. 시나리오 분기

| 시나리오 | 설명 | own1 정합 | Rosetta 2 활용 |
|---|---|---|---|
| **A** | **DIY x86 decoder + arm64 emit** — PE 매핑 후 자체 JIT 으로 모든 x86_64 명령 변환 | 5/5 (100% own) | ✗ (Rosetta 2 우회) |
| **B** | **PE → Mach-O thin shim** — PE 의 .text/.data 를 Mach-O LC_SEGMENT 로 wrap, IAT → __got 로 patch, 그 후 Rosetta 2 가 처리 | 4/5 (Rosetta 2 위임 — own1 허용 영역 내) | ✓ (위임) |
| **C** | **Hybrid** — interpret 우선 (느림, 정확), hot loop 만 시나리오 A 또는 B 로 JIT | 5/5 또는 4/5 | 부분 |

---

## 1. 5축 × 5점 채점 — 시나리오 별

### 1-1. 시나리오 A — DIY x86 decoder + arm64 emit

| 축 | 점 | 근거 |
|---|---|---|
| **A. own1 정합** | **5/5** | 100% DIY hexa. Rosetta 2 도 우회 → own1 자체 만점. Wine/community 0 줄 자명. |
| **B. 실 동작 도달** | **3/5** | 만점 도달 시 launch 가능. 그러나 SIMD coverage 미흡 시 first frame 직전 illegal instruction. 실 도달까지 SSE2/SSE3/SSSE3/SSE4.1/SSE4.2 + AVX/AVX2 + 일부 BMI 까지 필수 (D2R/D4 minimum). AVX-512 / AES-NI / SHA / VAES 는 후속. 단계적 도달 가능. |
| **C. 자원 영향** | **3/5** | 코드 캐시 RWX 영역 큼 (PE .text 의 1.2~2.0× 출력 — pe_jit_codegen_track expansion_ratio_pct). per-thread translation pool, signal handler 추가 stack. 메모리 +200~400MB (D2R 급), perf overhead 평균 10-30% (Rosetta 2 의 native ahead-of-time 보다 느림). |
| **D. 구현 난이도 (역)** | **1/5** | 8+ cycle 명시 (REMAINING_ROADMAP). 실 SIMD 매핑 + signal handler + SMC + W^X 통합 시 **16+ cycle** 예상. 하단 § 2 ISA 분량 참조. |
| **E. 의존성 (역)** | **3/5** | R1 (mmap RWX) 의존. R2/R3 와 약결합 (entry call 후에야 의미). M1 (i386 ABI FAIL) 우선 수정 권장. 의존 ≤2 영역 → 3점. |

**합계: 5+3+3+1+3 = 15/25**

→ own1 만점이지만 D 축 (난이도) 가 발목 — sprint 후보 cutoff 15 와 정확히 동률.

---

### 1-2. 시나리오 B — PE → Mach-O thin shim → Rosetta 2 위임

| 축 | 점 | 근거 |
|---|---|---|
| **A. own1 정합** | **4/5** | Rosetta 2 는 own1 허용 영역 (SCORING.md §A 명시). 그러나 5/5 는 **DIY hexa 100%** 정의 — Rosetta 2 위임은 4/5. shim 자체 (PE → Mach-O 변환기) 는 100% DIY. |
| **B. 실 동작 도달** | **5/5** | Rosetta 2 가 SSE/AVX/AVX2/AVX-512 + BMI + AES-NI + SHA 등 ISA 전체 커버 (Apple 검증 코드 — 2020 이후 8000+ 게임에서 검증). PE → Mach-O 변환만 만점 시 즉시 launch. |
| **C. 자원 영향** | **4/5** | Rosetta 2 자체의 cache 는 OS 가 관리 (`/var/db/oah/`). 추가 overhead = shim 변환 (one-time, install 단계로 amortize). 단 RIP-relative 보존을 위해 `__text` 재배치 시 PE relocation 재생성 필요 → install per-PE +수백ms~수초. |
| **D. 구현 난이도 (역)** | **4/5** | 1-2 cycle 예상. shim 구성: (1) PE 헤더 → Mach-O 헤더 emit, (2) `.text/.rdata/.data` → `__TEXT/__RODATA/__DATA` LC_SEGMENT_64 매핑, (3) IAT 항목 → `__DATA,__got` indirect symbol patch, (4) entry → `LC_MAIN` / `LC_UNIXTHREAD`. 대부분 헤더 manipulation. 어려운 부분: PE/SEH `.pdata` → DWARF/compact unwind (선택적, 없으면 unwind 미동작). |
| **E. 의존성 (역)** | **4/5** | R2 (PE 실 mmap) 의존. R3 (IAT 실 resolve) 와 강결합 (IAT → __got patch). R1/R5/R6 무관. 의존 2 영역 → 4점. |

**합계: 4+5+4+4+4 = 21/25**

→ **5축 모두 4 이상** — 본 검토에서 가장 균형 + 만점 + sprint 즉시 진입 가능.
→ 그러나 caveat 존재 (§ 4 하단 PE/SEH + RIP-relative + IAT 동작 보존).

---

### 1-3. 시나리오 C — Hybrid (interpret-first + hot loop JIT)

| 축 | 점 | 근거 |
|---|---|---|
| **A. own1 정합** | **5/5** (A path) 또는 **4/5** (B path). 평균 **4.5 → 5/5 round** | interpret 자체는 100% DIY. hot loop 만 시나리오 A 또는 B 사용 — A 채택 시 5/5, B 채택 시 4/5. 본 검토는 A path 를 hot loop 에 채택한다고 가정 → 5/5. |
| **B. 실 동작 도달** | **3/5** | interpret 만으로 first frame 도달 가능 (느리지만 기능적). gameplay 60fps 만점은 hot loop JIT coverage 비율에 비례. 50% coverage → 5fps, 90% → 30fps 추정. |
| **C. 자원 영향** | **2/5** | interpret + JIT cache 동시 운영 → 메모리 2 layer. interpret 의 fetch-decode-dispatch overhead 는 native 의 30-50× — gameplay impractical. CPU 사용률 매우 높음 (Apple Silicon thermal throttle 위험). |
| **D. 구현 난이도 (역)** | **2/5** | interpret loop 자체 3-4 cycle (single-threaded fetch-decode-dispatch + state machine). + hot loop JIT (시나리오 A 의 축소판) 4-6 cycle = **7-10 cycle 합산**. |
| **E. 의존성 (역)** | **3/5** | R1 (mmap, JIT 영역만) 의존, R2/R3 와 결합. C-WIN32-CALL-PROFILE / pe_jit_codegen_track / hot loop detector (별도 skeleton 필요) 와 결합. |

**합계: 5+3+2+2+3 = 15/25**

→ A 와 동률 15/25. interpret-first 의 점진 도달은 demo/POC 가치 있으나 sprint cutoff 동률 → 우선순위 후순.

---

### 1-4. 종합 비교표

| 시나리오 | A own1 | B 실동작 | C 자원 | D 난이도역 | E 의존역 | **합계** | rank |
|---|---|---|---|---|---|---|---|
| **A** DIY x86 decoder | 5 | 3 | 3 | 1 | 3 | **15/25** | 3위 (동률) |
| **B** Mach-O shim → Rosetta 2 | 4 | **5** | 4 | **4** | **4** | **21/25** ⭐ | **1위** |
| **C** Hybrid interpret + JIT | 5 | 3 | 2 | 2 | 3 | **15/25** | 3위 (동률) |

→ **시나리오 B 권장** — 5축 모두 ≥4, 합계 21 (sprint cutoff 15 대비 +6).

---

## 2. ISA 분량 평가 (시나리오 A 만 해당)

> 시나리오 B 는 Rosetta 2 가 모두 처리 → 본 절 무관. 시나리오 A/C 의 D 축 (난이도) 근거.

### 2-1. 필수 ISA 커버 (D2R/D4/Lineage 2 first frame 기준)

| ISA | 명령 수 (대략) | gamebox 등장 빈도 | A path cycle 비용 |
|---|---|---|---|
| **x86_64 base (general purpose)** | ~700 | 거의 모든 PE | 2 cycle (ALU/branch/load-store/REX prefix) |
| **x87 FPU (legacy float)** | ~80 | 일부 game (legacy DDraw 시대) | 1 cycle (대부분 무시 가능 — modern build 는 SSE) |
| **SSE / SSE2** | ~140 | **필수** (CRT memcpy/strcmp 의 SIMD 분기 포함) | 2 cycle (NEON 매핑 거의 1:1) |
| **SSE3 / SSSE3** | ~25 | 자주 (UE2.5/UE4 vector math) | 0.5 cycle (NEON 매핑) |
| **SSE4.1 / SSE4.2** | ~45 | 자주 (CRC32 등) | 1 cycle (PMULLD/CRC32 매핑 미묘) |
| **AVX (256bit)** | ~150 | **D4 필수** (HLSL → DXIL → AVX intrinsics) | 2 cycle (NEON 128bit 2× emit 또는 SVE 매핑) |
| **AVX2** | ~130 | D4 자주 | 2 cycle |
| **BMI / BMI2** | ~15 | 자주 (bit scan, pdep/pext) | 0.5 cycle |
| **AES-NI** | ~10 | DRM/anti-cheat 자주 | 0.5 cycle (Apple AES coproc 매핑) |
| **SHA-NI** | ~7 | 일부 anti-cheat | 0.5 cycle (Apple SHA coproc 매핑) |
| **AVX-512** | ~250 | 거의 무 (소비자 게임 미사용) | **선택** — 4 cycle (구현 시) |
| **VAES / GFNI / VPCLMULQDQ** | ~40 | 거의 무 | **선택** — 1 cycle |

**A path 합계 (필수만)**: 2+1+2+0.5+1+2+2+0.5+0.5+0.5 = **12 cycle 의 ISA decoding/emit 작업**.
**전체 (선택 포함)**: +5 cycle = **17 cycle**.

### 2-2. arm64 NEON / SVE 매핑 가능성

| x86 SIMD | arm64 매핑 | 어려움 |
|---|---|---|
| SSE/SSE2 128bit | NEON 128bit (Q register) | **쉬움** — 대부분 1:1, mask/predicate 만 주의 |
| AVX 256bit | NEON 128bit × 2 emit | **중간** — register pressure (NEON 32×128bit 만, AVX 16×256bit 와 다름) |
| AVX2 256bit gather | NEON 등가 무 → loop 분해 | **어려움** — sequential gather emit, perf 손실 |
| AVX-512 512bit | **SVE2** (Apple Silicon 미지원, M1~M4 NEON only) → 4× emit | **매우 어려움** — Apple 은 SVE 미탑재 (2026-05 기준) |
| BMI pdep/pext | arm64 등가 무 → bit-by-bit emit | **어려움** — 5-15× 느림 |
| AES-NI AESENC | arm64 AESE+AESMC pair | 쉬움 (1:1 매핑 가능) |
| SHA-NI SHA256RNDS2 | arm64 SHA256H/SHA256H2 | 쉬움 |

→ **Apple Silicon 의 SVE 부재** (2026-05 기준 M1/M2/M3/M4 모두 NEON only) 가 AVX-512 path 의 결정적 장벽 — 시나리오 A 의 D 축 1/5 근거.

### 2-3. Self-Modifying Code (SMC) 처리

| 출처 | 동작 | A path 처리 |
|---|---|---|
| 일반 PE | 거의 없음 (modern compiler 비생성) | 무관 |
| **anti-cheat (GameGuard, EAC, BattlEye)** | 자주 — runtime 에 자기 코드 mutate (검증 회피) | **A path 의 통증 지점**. Rosetta 2 는 SMC detect → invalidate → re-translate 자동 처리. A path 는 별도 invalidation 메커니즘 필요 (mprotect-based write-watch + cache flush). |
| JIT 게임 (Lua/V8/Mono) | runtime 에 신규 코드 emit | **A path 추가 비용** — JIT-on-JIT (게임 JIT 가 emit 한 native 를 다시 a path JIT 가 변환) |

→ anti-cheat 우회는 본 검토 범위 외 (DELTA_FORCE/PURPLE_LINEAGE 만점 검토 docs 가 정의한 영구 차단 영역).
→ Lua/Mono JIT-on-JIT 는 시나리오 B 가 자동 처리 (Rosetta 2 의 강점).

### 2-4. W^X 위반 회피 — Apple Silicon 제약

```
mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE | MAP_JIT, -1, 0)
```

| 요구 | 설명 |
|---|---|
| `MAP_JIT` flag | mmap 시 RWX 영역 생성 허용 (Apple Silicon hardware-enforced W^X 우회) |
| `com.apple.security.cs.allow-jit` entitlement | 코드사인된 binary 만 MAP_JIT 사용 가능. gamebox dispatcher 에 entitlement 필수 |
| `pthread_jit_write_protect_np(false)` | 작성 직전 호출 → write 가능 모드 |
| `pthread_jit_write_protect_np(true)` | 작성 후 즉시 호출 → execute 가능 모드 |
| `sys_icache_invalidate(...)` | cache coherency — write 후 execute 전 필수 |
| **per-thread!** | `pthread_jit_write_protect_np` 는 per-thread 상태 — 모든 codegen thread 가 따로 toggle |

→ 시나리오 A: 본 메커니즘 직접 구현 (R1 stage7 native bridge 가 prerequisite).
→ 시나리오 B: Rosetta 2 가 처리 (Apple 자체 entitlement 사용).
→ 시나리오 C: hot loop JIT path 만 본 메커니즘 필요.

→ **R1 만점 검토 (STAGE7_NATIVE_BRIDGE) cross-link** — pthread_jit_write_protect_np 와 sys_icache_invalidate 가 R4 의 prerequisite.

### 2-5. Signal Handler 통합

x86_64 PE 의 SEH (Structured Exception Handling) → macOS BSD signal / Mach exception 변환 필수:

| Windows | macOS | 변환 cost |
|---|---|---|
| `EXCEPTION_ACCESS_VIOLATION` | `SIGSEGV` (BSD) / `EXC_BAD_ACCESS` (Mach) | 중 — context (CONTEXT struct) 변환 |
| `EXCEPTION_INT_DIVIDE_BY_ZERO` | `SIGFPE` / `EXC_ARITHMETIC` | 쉬움 |
| `EXCEPTION_BREAKPOINT` (int3) | `SIGTRAP` / `EXC_BREAKPOINT` | 쉬움 |
| VEH (Vectored Exception Handler) | (Windows 고유) | **A path 직접 구현** — handler chain 재현 |
| `RtlAddFunctionTable` (.pdata 동적) | (X) | **A path 자체 unwind** |

→ A path: 신규 4 cycle 추가 (signal/Mach exception bridge + CONTEXT 변환 + VEH chain).
→ B path: Rosetta 2 의 signal bridge 활용 가능 (Apple 이 PE/SEH 모름 → DIY shim 단계에서 unwind metadata 합성).

---

## 3. cycle 68 calling convention adapter 와의 관계

cycle 68 skeleton (`c_abi_calling_convention_track.hexa` + 관련):
- `stdcall` / `cdecl` / `fastcall` / `vectorcall` / `x64_ms` / `to_arm64` / `thiscall` / `va_args`

본 R4 의 binary translation 과 결합 지점:

| convention | x86_64 ABI | arm64 ABI | binary translation 의 처리 |
|---|---|---|---|
| `cdecl` (i386) | stack-based, caller cleanup | x0..x7 + stack | adapter trampoline 자동 emit |
| `stdcall` (i386) | stack-based, callee cleanup | 동일 + cleanup adjustment | 동일 |
| `fastcall` (i386) | ECX, EDX + stack | x0, x1 + stack | 동일 |
| `vectorcall` (i386/x64) | XMM0-5 + GPR | NEON V0-V7 | SIMD register 매핑 |
| `x64_ms` (Windows x64) | RCX, RDX, R8, R9 + stack (32B home) | x0..x7 + stack | **R4 의 핵심** — 모든 PE x86_64 함수 |
| `thiscall` (C++) | ECX = `this` (MSVC) | x0 = `this` | C++ vtable 호출 path |
| `va_args` | RAX = vector count | NEON 별도 va_list spill | varargs 변환 (CRT printf 등) |

→ cycle 68 adapter 가 R4 의 **함수 진입/퇴출 frame** 을 제공.
→ R4 본문은 함수 **내부 명령** 변환. 두 layer 가 결합 시 entry → body → exit 완전 path.

→ 시나리오 B 도 adapter 필요 (PE IAT → Mach-O __got 으로 patch 시 host shim 함수 진입에서 ABI 변환).
→ 시나리오 A 는 본문 + adapter 모두 직접 emit.

---

## 4. 시나리오 B 의 caveat (PE → Mach-O thin shim)

본 시나리오의 5/5 등급은 **3 가지 동작 보존** 전제:

### 4-1. RIP-relative addressing 보존

x86_64 PE 의 `lea rax, [rip + 0x12345]` → Mach-O 로 wrap 시 `.text` 시작 주소가 다르면 RIP 가 이동 → relocation 재계산 필수.

**해결**: PE relocation table (`IMAGE_REL_BASED_DIR64`) 을 Mach-O `__TEXT` section symbol 로 변환 + `dyld` 의 fix-up 메커니즘 활용.
**cost**: shim 변환 시 +50~200ms per PE (one-time).

### 4-2. IAT (Import Address Table) 동작 보존

PE 의 IAT 는 `mov rax, qword ptr [iat_entry]; call rax` 패턴.
Mach-O 의 indirect call 은 `__got` indirect symbol stub.

**해결**: IAT 항목 → Mach-O `__DATA,__got` indirect symbol pointer 로 변환. 호출 site (`call qword ptr [iat]`) 은 RIP-relative → 변환 후 그대로 작동.
**의존**: R3 (IAT 실 resolve) 만점 필수.

### 4-3. PE/SEH (Structured Exception Handling) 보존 또는 폐기

PE `.pdata` (RUNTIME_FUNCTION) → DWARF/compact unwind 변환은 **매우 어려움** (Windows 의 `UNWIND_INFO` 와 DWARF 의 mental model 다름).

**해결 옵션**:
- (가) 폐기 — SEH 미사용 game 한정 (D2R, Lineage 1 가능). 그러나 D4/UE4/UE5 는 SEH 의존 → unwind 안되면 crash.
- (나) DIY unwind synthesizer — `.pdata` 의 epilog/prolog code 분석 → DWARF FDE/CIE emit. 복잡 +2-4 cycle.
- (다) Rosetta 2 의 signal handler 가 raw signal 만 처리, gamebox 의 DIY VEH chain 위에 SEH 재현. 중간 cost.

→ B path 의 **5/5 (B 축)** 등급은 (다) 채택 + game-by-game SEH coverage 가정.

---

## 5. 결론 + 우선순위

### 5-1. 만점 종합

| 시나리오 | 합계 | own1 | 실동작 | 자원 | 난이도역 | 의존역 | sprint? |
|---|---|---|---|---|---|---|---|
| **A** DIY decoder | 15/25 | 5 | 3 | 3 | 1 | 3 | cutoff 동률 |
| **B** Mach-O shim → R2 | **21/25** ⭐ | 4 | 5 | 4 | 4 | 4 | **즉시** |
| **C** Hybrid | 15/25 | 5 | 3 | 2 | 2 | 3 | cutoff 동률 |

### 5-2. 권장 path

**1순위: 시나리오 B (PE → Mach-O thin shim → Rosetta 2 위임) — 21/25**

- 이유:
  - 5축 모두 4 이상 (균형)
  - own1 정합 4/5 — Rosetta 2 위임은 SCORING.md §A 명시 허용 영역
  - 실 동작 도달 5/5 — Rosetta 2 의 검증된 ISA coverage (8000+ 게임)
  - 난이도 4/5 — 1-2 cycle (대부분 헤더 manipulation)
  - 의존 R2/R3 만 (R1 무관 — Rosetta 2 가 jit 영역 관리)
- 단점:
  - PE/SEH 변환 (§ 4-3) — game-by-game caveat
  - one-time install 비용 +50~200ms per PE
  - own1 정합 5/5 미달 (Rosetta 2 dependency)

**2순위 (POC/skeleton 가치): 시나리오 C — interpret-first**

- 이유: B path 가 동작 안할 때 fallback (예: SEH-heavy game). interpret 자체는 5fps 라도 first frame 검증 가능
- 단점: gameplay impractical, 자원 2/5

**3순위 (장기 own1 100% 만점 목표): 시나리오 A — DIY decoder**

- 이유: own1 5/5 만점 달성 시 Rosetta 2 dependency 제거. anti-cheat-friendly (Rosetta 2 detection 회피 가능)
- 단점: 16+ cycle ETA, AVX-512 path 거의 불가능 (Apple SVE 부재)
- 진입 시점: 시나리오 B 로 D2R/D4 launch validated 후 — own1 100% 의 long-term aspirational sprint

### 5-3. 즉시 권장 action

1. **시나리오 B skeleton candidate 작성** — `c_pe_to_macho_shim_track` (PE 헤더 → Mach-O 헤더 emit count + size delta tracking). cycle 80+ batch 후보 (만점 15/15, own1 4/5 + 정합 4 + 실동작 5 + 자원 5 + 의존 1 — D-list 추정).
2. **시나리오 B 의존 R2/R3 prerequisite 명확화** — REMAINING_ROADMAP.md 의존성 그래프에 `R4 (B path) ← R2/R3` 추가 (현재 `R4 ← R1` 만 명시).
3. **PE/SEH coverage 분석 skeleton** — D2R / D4 / Lineage 2 의 `.pdata` size 분포 측정 → § 4-3 (다) 의 추정 cost 재평가.
4. **시나리오 A 의 cycle 68 calling convention adapter 와의 통합 매트릭스 구체화** — 본 검토는 skeleton-level. 실 emit 단계에서 adapter trampoline + RIP-relative + PE/SEH unwind 의 통합 시나리오 별도 검토.
5. **Rosetta 2 entitlement / signed binary 정책 확인** — gamebox dispatcher 의 코드사인 + entitlement 분포. `com.apple.security.cs.allow-jit` 와 Rosetta 2 위임 시 entitlement 변경 사항.

### 5-4. 미래 가능성 (재검토 trigger)

다음 중 하나 발생 시 재검토:
- **Apple M5+ 가 SVE2 탑재** → AVX-512 매핑 가능 → 시나리오 A 의 D 축 +2 (3/5) → 합계 17/25 진입
- **Rosetta 2 deprecation** (Apple 공식 예고) → 시나리오 B 무력 → A 강제 채택
- **Rosetta 2 가 PE 직접 지원** (가능성 매우 낮음) → 시나리오 D 신규 (PE 직접 위임, own1 4/5 + 실동작 5 + 자원 5 + 난이도 5 + 의존 5 = 24/25 만점)
- **WSL/Win32 원격 sidecar pattern 채택** (own1 정의 변경 시) — 본 검토 무관 (own1 mandate 위반)

### 5-5. cross-link

- R1: [STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md](STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md) — pthread_jit_write_protect_np / sys_icache_invalidate / mmap MAP_JIT prereq
- R2: [PE_LOADER_REAL_MMAP_MANJEOM_REVIEW.md](PE_LOADER_REAL_MMAP_MANJEOM_REVIEW.md) — `.text/.data` 매핑 prereq (시나리오 B 의 LC_SEGMENT 변환 의존)
- R3: [IAT_REAL_RESOLVE_MANJEOM_REVIEW.md](IAT_REAL_RESOLVE_MANJEOM_REVIEW.md) — IAT → __got 매핑 prereq
- R5: [D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md](D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md) — first frame 가능 후 의미
- R6: [WIN32_DLL_REAL_IMPL_MANJEOM_REVIEW.md](WIN32_DLL_REAL_IMPL_MANJEOM_REVIEW.md) — IAT 가 가리키는 DLL 함수 실 구현
- skeleton: `lib/loader/pe_jit_codegen_track.hexa` (cycle 67) — codegen 통계 추적기 (시나리오 A path 의 input/output bytes/elapsed_us aggregation 가능)
- adapter skeleton: `lib/perf/c_abi_calling_convention_track.hexa` (cycle 68) — convention 분포 추적기

---

*written 2026-05-01. R4 binary translation 만점 검토 — 시나리오 B (PE → Mach-O shim → Rosetta 2 위임) 21/25 권장. own1 정의상 Rosetta 2 는 허용 영역 (SCORING.md §A 명시) — 단 5/5 own1 만점은 DIY 100% (시나리오 A) 만 도달 가능. 시나리오 A 의 16+ cycle ETA + Apple Silicon SVE 부재 (AVX-512 path 불가) 가 발목 → B path 가 실 launch 우선. SEH coverage 가 game-by-game caveat — 후속 별도 검토 필요.*
