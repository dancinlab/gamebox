# PE binary 실 mmap 적재 — 만점 검토

> **status**: 작성 2026-05-01 / 만점: **17/25** / 결론 한줄
> **결론**: own1 정합 + 의존성 측면은 만점 가깝지만, Apple Silicon **16KB page granularity** 제약과 **W^X (RWX 동시 불가)** 정책으로 "실 launch 도달" 축이 부분점에 묶인다. R1 (stage7 native bridge) 의 `mmap_anon` / `mprotect_rx` 가 prereq — R1 진척 없이는 본 영역도 skeleton 천장.
> SSOT: `docs/REMAINING_ROADMAP.md` 5축×5점, 형식 `docs/PURPLE_LINEAGE_MANJEOM_REVIEW.md`.

---

## 0. 영역 정의

본 영역 = **R2 — PE binary 실 mmap 적재** (잔여 로드맵 #2). 다음 4 sub-step 이 "만점" 정의:

| sub | 범위 | Microsoft PE/COFF spec 참조 |
|---|---|---|
| **0-A. PE32+ COFF section header → mmap segments** | `IMAGE_SECTION_HEADER` (40 byte each) 의 `VirtualAddress / VirtualSize / SizeOfRawData / Characteristics` 를 read → host VM 에 anonymous mmap (+ raw bytes copy) | spec § "Section Table" |
| **0-B. base relocation → x64 RIP-relative 보존** | `IMAGE_DIRECTORY_ENTRY_BASERELOC` (DataDir[5]) 의 `IMAGE_BASE_RELOCATION` block walk → `IMAGE_REL_BASED_DIR64` (10) / `IMAGE_REL_BASED_HIGHLOW` (3) 적용. delta = `actual_base - OptionalHeader.ImageBase` | spec § "The .reloc Section" |
| **0-C. section permissions (W^X 적용)** | `Characteristics & IMAGE_SCN_MEM_EXECUTE/READ/WRITE` → POSIX `mprotect(PROT_READ\|PROT_WRITE\|PROT_EXEC)`. Apple Silicon 은 `MAP_JIT` + `pthread_jit_write_protect_np` 토글로 W^X 우회 | spec § "Section Flags" + Apple `mmap(2)` |
| **0-D. TLS index → fs[0]/gs[0] 매핑** | `IMAGE_TLS_DIRECTORY64` 의 `AddressOfIndex` / `StartAddressOfRawData` / `AddressOfCallBacks` → host TEB stub 의 `gs:[0x58]` (TLS array) emulate. Win64 TEB ≠ macOS pthread TLS | spec § "The .tls Section" + AMD64 ABI § "TLS" |

**비범위**:
- 실 Mach-O 코드 변환 (= R4 binary translation)
- IAT thunk 주소 patch (= R3 IAT resolve)
- DLL stub 함수 동작 (= R6 DLL real impl)

---

## 1. 현 상태 [skeleton 진척 — 9 modules, 2881 lines]

| 모듈 | LOC | 단계 | 본 영역 sub | 비고 |
|---|---|---|---|---|
| `pe_loader_stage2.hexa` | 369 | orchestrate skeleton (TSV — `orchestrate_history.tsv`) | 0-A 진입점 | section copy 추적만 — 실 mmap 미호출 |
| `pe_iat_resolve_stage2.hexa` | 260 | skeleton | (R3) | 본 영역 비대상 (cross-link only) |
| `pe_reloc_stage2.hexa` | 307 | skeleton (`reloc_log.tsv` — `pe_id/reloc_count/delta_bytes/applied`) | **0-B** | delta 계산 추적, 실 fixup write-back 미구현 (stage3) |
| `pe_tls_stage2.hexa` | 339 | skeleton (`callback_history.tsv` — reason 1~4) | **0-D 부분** | TLS callback dispatch 추적 — `AddressOfIndex` write 미구현 |
| `pe_heap_stage2.hexa` | 359 | skeleton | (boundary — process heap 초기화) | RtlCreateHeap 추적, 실 segregated free-list X |
| `pe_stack_stage2.hexa` | 319 | skeleton | (boundary — initial stack/guard page) | guard page 추적, 실 mmap X |
| `pe_seh_stage2.hexa` | 339 | skeleton | (boundary — `.pdata`/`.xdata` register) | RUNTIME_FUNCTION 추적, 실 unwind X |
| `pe_entry_stage2.hexa` | 201 | skeleton | (boundary — `AddressOfEntryPoint` 호출) | dispatch 추적, 실 jump X |
| `pe_mmap_protection.hexa` | 388 | skeleton (`pmp_history.tsv` — prot_kind 1~4) | **0-C** | prot toggle 추적, 실 mprotect 미호출 |

→ **공통**: 모든 stage2 가 TSV-only instrumentation. 실 syscall 미호출 → "PE 적재 skeleton coverage = 100%, 실 적재 = 0%".
→ **0-A (section mmap) 가 가장 미진**: `pe_loader_stage2` 가 sections 수만 count, 실 raw bytes copy 무.
→ **0-D (TLS index) 가 두번째 미진**: callback dispatch 추적은 있으나 `AddressOfIndex` 가 가리키는 DWORD 위치에 TLS slot index write 가 본 skeleton 에 부재.

---

## 2. 단계 도달성

| 단계 | 만점 시 도달 | 부분점 시 도달 |
|---|---|---|
| **0-A. section mmap** | `IMAGE_SECTION_HEADER` 전체 walk + `mmap(addr=image_base+VA, len=align_up(VS, page), PROT_NONE, MAP_ANON\|MAP_PRIVATE\|MAP_FIXED)` + raw bytes `memcpy` (최대 `min(VirtualSize, SizeOfRawData)`). bss tail 은 zero-fill. | section 수 + 누적 bytes 만 TSV record (현재) |
| **0-B. base reloc** | `IMAGE_REL_BASED_DIR64` block 별 `*(uint64*)(page+offset) += delta` write-back. ASLR 시뮬 시 delta ≠ 0 강제 검증. | delta 계산 + applied count TSV (현재) |
| **0-C. section perm** | `mmap(MAP_JIT)` 로 .text reservation → `pthread_jit_write_protect_np(false)` write phase → `mprotect(PROT_READ\|PROT_EXEC)` + `pthread_jit_write_protect_np(true)`. .data 는 `PROT_READ\|PROT_WRITE`. | prot_kind enum (1/2/3/4) TSV record (현재) |
| **0-D. TLS index** | host TEB stub `gs:[0x58]` 에 TLS array 포인터 set + `*AddressOfIndex = allocated_slot_idx` write + initial raw data 영역 `memcpy` per-thread. | callback invocation 추적만 (현재) |

→ **만점 = 4 sub 모두 실 syscall 도달**. 부분점 = TSV instrumentation only (현 상태).

---

## 3. 5축 채점 /25

| 축 | 점수 | 근거 |
|---|---|---|
| **A. own1 정합** | **5/5** | Microsoft PE/COFF spec + Apple `mmap(2)`/`mprotect(2)` man page + AMD64 ABI § TLS 만 참조. Wine `dlls/ntdll/loader.c` / `loader/pe_image.c` / Proton `vrclient_x64` 코드 0 줄. R1 의 `pe_native_mmap_wrapper` 가 자체 POSIX wrapper — 외부 community 의존 없음. |
| **B. 실 동작 도달** | **3/5** | 만점 도달 시 entry point 까지는 jump 가능 — 그러나 단독 launch 불가 (R3 IAT + R6 DLL 실 구현이 없으면 첫 import 호출에서 `0xCC` trap). 본 영역 단독으로는 "first instruction 까지 도달" 만 보장. **0-D TLS 가 Win64 TEB 의 `gs:[0]`/`gs:[0x30]`/`gs:[0x58]` 전부를 host pthread TLS 위에 emulate 해야 함 → macOS `gs` 부재 (`fs` 만 있음, 그것도 ARM64 는 `tpidr_el0`)** → arm64 native 환경에서 TEB 자체가 가짜 segment 로 emulate 필요. |
| **C. 자원 영향** | **4/5** | mmap 자체 overhead 무시 가능 (lazy faulting). 단 **Apple Silicon 16KB page** 가 Win32 4KB 와 다름 → small section (.rsrc/.tls/.pdata < 4KB) 마다 16KB 강제 점유 → fragmentation +20~30%. .data 의 4KB-align 가정 binary 는 page-overlap 시 W^X 위반 → 추가 split mmap 필요 → +1 page per 그러한 section. |
| **D. 구현 난이도 (역)** | **2/5** | REMAINING_ROADMAP estimate: **3 cycle** (section mapper / reloc applier / TLS init bg 분해). 하지만 manjeom 채점의 "1 cycle 이내 = 5점" 기준으로는 **2점** 이 정직. R1 prereq 까지 합산 시 7 cycle 누적 가능. |
| **E. 의존성 (역)** | **3/5** | R1 (stage7 native bridge — `mmap_anon` / `mprotect_rx` / `pthread_jit_write_protect`) 가 hard prereq. R3 (IAT resolve) 는 본 영역 출력에 의존 (= 후행). R4 (binary translation) 와는 약결합 — Rosetta path 사용 시 R4 우회 가능. **prereq 1, 후행 1** → 5점에서 -2 → **3점**. |

**합계**: 5 + 3 + 4 + 2 + 3 = **17/25**

→ 15 이상 → **sprint 후보**. own1 정합 만점 + 자원 영향 양호 — 구현 난이도와 실 동작 도달이 발목.

---

## 4. 핵심 blocker

### 4-1. Apple Silicon 16KB page granularity ≠ Win32 4KB

- macOS arm64 의 hardware page size = **16384 bytes** (`getpagesize()` 확정).
- Win32 PE optional header `SectionAlignment` 기본값 = **4096**, `FileAlignment` = **512**.
- 결과: `.text` (예: 0x1000–0x4FFF, 16KB) 직후 `.rdata` (0x5000–0x5FFF, 4KB) 가 같은 host 16KB page 에 packed → W^X 위반 (실행+읽기 한 page).
- **해결**: section 별 별도 16KB-align 강제 mmap (= file 의 4KB layout 무시하고 host VA 재배치). 단 RIP-relative offset 보존을 위해 reloc 추가 fixup 필요 (모든 cross-section reference) → reloc table 부재 PE binary (DLL 외 EXE 의 `IMAGE_FILE_RELOCS_STRIPPED` flag) 시 **불가능**.

### 4-2. RWX 동시 불가 (W^X policy)

- macOS Hardened Runtime + Apple Silicon = `MAP_JIT` + `pthread_jit_write_protect_np()` 토글만이 RWX 의 정당 path.
- `MAP_JIT` 는 entitlement `com.apple.security.cs.allow-jit` 필수 → airgenome-helper.app 의 codesign 에 추가 필요 (현재 cs entitlement 미확인).
- toggle cost: per-thread, ~50-200ns. .text section 의 reloc fixup 처럼 **자주 write phase 진입** 시 cumulative overhead 무시 못함.

### 4-3. .reloc 누락 binary

- 일부 게임 EXE (특히 비-ASLR 강제 binary) 가 `IMAGE_FILE_RELOCS_STRIPPED` 로 reloc table 제거 → preferred ImageBase (예: `0x140000000`) 로 mmap 강제 필요.
- macOS dyld shared cache 가 `0x140000000` 영역을 이미 점유 시 (가능성 높음 — system framework) → mmap fail.
- **해결**: `vm_allocate(VM_FLAGS_FIXED)` + `vm_remap` 으로 강제 → 그러나 Apple 정책상 dyld 영역 collision 시 abort. **차선**: emulate 위한 별도 process spawn (R1 의 native helper 활용).

### 4-4. forwarder DLL / bound import

- `IMAGE_DIRECTORY_ENTRY_IAT` 와 `IMAGE_DIRECTORY_ENTRY_IMPORT` 의 hint/name table 이 forwarder string (`"NTDLL.RtlSomething"`) 가리킬 시 R3 영역. 본 영역에서는 IAT 영역 자체를 R+W 로 mapping 해두기만 하면 됨 — boundary 명확.

### 4-5. TLS callback ABI mismatch

- Win64 ABI: `void NTAPI TlsCallback(PVOID DllHandle, DWORD Reason, PVOID Reserved)` — `rcx/rdx/r8` 인자.
- macOS arm64 ABI: `x0/x1/x2`.
- R4 (binary translation) 미진척 시 callback 직접 호출 불가 → 본 영역의 0-D 만점은 Rosetta runtime path 가정 필요.

### 4-6. own1 enforcement

- Wine 의 `dlls/ntdll/unix/loader.c` 의 `map_image_into_view()` / `relocate_image()` 알고리즘과 **유사하지만 별개로 작성**해야 함. spec only 참조 — Wine 코드 0 줄 검증 필수 (CI 의 `oss_validate.hexa` 가 path scan).

---

## 5. 의존성

### 5-1. prereq (본 영역 시작 전 필요)

| 영역 | 무엇 필요 | 현 상태 |
|---|---|---|
| **R1. stage7 native bridge** | `mmap_anon(addr, len, prot, flags)` / `mprotect_rx(addr, len)` / `memcpy_to_jit(dst, src, len)` 실 호출 wrapper | skeleton (`pe_native_mmap_wrapper.hexa`, `pe_native_mprotect_wrapper.hexa`) — 실 syscall 미호출 |
| codesign entitlement | `com.apple.security.cs.allow-jit` 추가 | 미확인 (helper.app 의 entitlements.plist 미점검) |

### 5-2. 후행 (본 영역 결과를 소비)

| 영역 | 무엇 의존 | 비고 |
|---|---|---|
| **R3. IAT 실 resolve** | 본 영역의 mapped 영역 base 주소 + IAT slot 주소 | 본 영역 0-A 결과 그대로 사용 |
| R6. DLL stub 실 구현 | R3 가 patch 한 stub 주소 | R3 통과 후 |
| R4. binary translation | RWX page 위에 ARM64 codegen — 본 영역의 W^X toggle infra 공유 | 약결합 (R4 가 자체 `MAP_JIT` 영역 별도 alloc 가능) |

### 5-3. 의존 그래프 (REMAINING_ROADMAP § "의존성 그래프" 발췌)

```
R1 (stage7 native bridge)
   └── R2 (PE 실 mmap)        ← 본 영역
         └── R3 (IAT 실 resolve)
               └── R6 (DLL stub 실 구현)
```

→ **본 영역 = critical path 의 두번째 노드**. R1 통과 없이는 시작도 불가.

---

## 6. ETA / cycle 수 + bg 분해

REMAINING_ROADMAP estimate = **3 cycle (병렬 bg)**. 본 검토 정밀화:

| bg | sub | 예상 cycle | 산출물 |
|---|---|---|---|
| **bg-A** | 0-A. section mapper 실 mmap | 1.0 | `pe_section_mapper_stage3.hexa` — `pe_loader_stage2` 가 호출, raw bytes `memcpy` + bss zero-fill, `MAP_FIXED` 충돌 시 fallback 경로 |
| **bg-B** | 0-B. reloc applier 실 fixup | 0.7 | `pe_reloc_apply_stage3.hexa` — `IMAGE_REL_BASED_DIR64`/`HIGHLOW` write-back, page-batch (16KB 단위 grouped fixup) |
| **bg-C** | 0-C. mprotect + W^X toggle | 0.6 | `pe_mmap_protection_stage3.hexa` — `MAP_JIT` reservation + `pthread_jit_write_protect_np` ledger + entitlement 검증 self-test |
| **bg-D** | 0-D. TLS index + TEB emulate | 1.2 | `pe_tls_apply_stage3.hexa` + `pe_teb_struct` 갱신 — host pthread TLS 위에 fake `gs` segment table, `AddressOfIndex` write |
| **bg-E** | self-test 통합 + golden binary fixture | 0.5 | tiny PE32+ fixture (자체 합성, "hello world" stub) → end-to-end mmap → entry jmp → ret 검증 |

→ 병렬화 시 **3 cycle 합리적** (max(bg-A,bg-D) + bg-E = 1.2 + 0.5 ≈ 1.7 wall-clock). 직렬 시 4 cycle.
→ R1 prereq 미통과 시 본 영역 ETA = **R1 ETA + 3 cycle**.

---

## 7. 결론

본 영역 R2 = **17/25 — sprint 후보 (15 이상 임계 통과)**.

- **own1 정합 5/5** + **자원 영향 4/5** 가 강점. Apple Silicon 16KB page 강제 점유 외에는 PE 적재 로직 자체의 메모리/perf 부담은 작다.
- **실 동작 도달 3/5** + **구현 난이도 2/5** 가 약점. 단독으로는 "entry instruction 1개 실행" 까지만 보장 — R3 / R6 와 함께 가야 의미 있는 launch 가 된다.
- **의존성 3/5** — R1 의 `mmap_anon` / `mprotect_rx` / `MAP_JIT` wrapper 가 prereq, 그 위에서만 본 영역 시작 가능.

**즉시 권장 action**:
1. R1 만점 검토 (`STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md`) 가 완료되어 native bridge 실 호출이 도달한 시점에 본 영역 sprint 진입.
2. helper.app 의 entitlements.plist 에 `com.apple.security.cs.allow-jit` 존재 여부 우선 점검 — 부재 시 R2 자체가 entitlement gate 에서 멈춤.
3. bg-A / bg-B / bg-C / bg-D 4-way 병렬 — bg-D (TLS + TEB emulate) 가 가장 critical path (Win64 TEB ≠ macOS pthread TLS, fake segment 합성 필요).
4. golden fixture (자체 합성 tiny PE32+) self-test 를 cycle 끝에 추가 — Wine 의 `tests/binary/` 코드 0줄 — 자체 PE writer 작성 (own1 정합 유지).
5. **own1 검증**: 본 영역 모든 hexa 파일이 `oss_validate.hexa` path scan 통과 — Wine `dlls/ntdll/unix/loader.c` 알고리즘 학습은 **spec 만 인용** 형태로 주석화 (코드 carry 금지).

---

*written 2026-05-01. 잔여 로드맵 R2 — PE binary 실 mmap 적재 만점 검토. 17/25 = sprint 후보 (단 R1 prereq 통과 후). own1 enforcement: Wine/CrossOver/community 코드 0 줄 — Microsoft PE/COFF spec + Apple mmap/mprotect man page + AMD64 ABI § TLS 만 참조.*
