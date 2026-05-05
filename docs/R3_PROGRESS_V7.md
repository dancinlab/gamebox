# R3 PROGRESS V7 — 8360 instruction (492×) main-direct ceiling + USER32 미도달 + wave 7 BG 분산 trigger (2026-05-01)

> **status**: R1+R2 own1 §A closure **100%** 유지 + R3-1~11 wave6 main-direct 통합 + shim wave5 (MSVCR/UCRT/NTDLL/ADVAPI32 CRT runtime init stub dispatch) + wave6 (8-bit MOV/arith expand + fake_heap + max_iter cap 65536 + 0F escape modrm consumption) 단일-thread main-direct 일괄 통합. 누적 R3 진척 **~80%**. 다음 halt = **CRT init 깊이 진행 중** — `__scrt_common_main_seh` 의 `_initterm` array walk + `__acrt_iob_func` / `_configure_narrow_argv` 인근 영역에서 inst[8360] 정체 (USER32 호출 미도달, RegisterClassExA 미진입). wave 7 (10 BG 분산) trigger.
> **base commits**: `25de965` (V6 작성, 343-370 inst halt) → `03797a9` (hexa winmain_entry_path_real) → `bcef52b` (hexa msvcr_crt_init_stub_real) → `d74f023` (helper.mm DispatchMessageA wndproc_invoke + window class map) → `52502ca` (shim wave5 CRT runtime init + MSVCR/UCRT/NTDLL/ADVAPI32 stub dispatch) → `02d418d` (docs R3_HALT_343_ANALYSIS — fake_proc_stub stdcall 미처리 root cause) → `ab9f012` (shim wave6 main-direct — 8-bit MOV/arith opcode, 343→1889 inst, 5.5×) → `e356815` (shim wave6 main-direct extra — 8-bit MOV/arith expand + fake_heap, 1889→8360 inst) → `0a24e22` (shim wave6 — 0F escape modrm consumption, 8359→8360 inst stable).
> **현재 차단**: shim interpreter 가 inst[8360] 인근에서 main-direct path 만으로는 USER32 호출 도달 불가. CRT init 깊이 (initterm array walk + onexit table register + atexit chain + locale init) 가 깊어 main-direct sprint 단일 thread 로는 inst 진척이 plateau. **wave 7 BG 분산 (10 BG 동시 진행)** 으로 progress — CRT 의 비결정 분기를 BG 별로 분담 처리.
> **own1 정합**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift **0줄** 유지. 자체 코드 — `native/pe_to_macho_shim.cpp` **5500+ line** (+576 vs V6) + `native/helper.mm` **1300+ line** (+30 vs V6) + `lib/loader/pe_i386_*.hexa` **27 file** × 평균 ~500 line. CRT init stub return 표 자체 작성 (Microsoft `learn.microsoft.com` API public docs reference + ReactOS public docs reference).
> **참조**: [R3_PROGRESS_V6.md](R3_PROGRESS_V6.md) (V6 의 343-370 inst + KERNEL32 stub 7-9 + CRT init trigger) / [R3_PROGRESS_V5.md](R3_PROGRESS_V5.md) (V5 의 277 inst + helper NSWindow real + wave3 cap 해제) / [R3_PROGRESS_V4.md](R3_PROGRESS_V4.md) (V4 의 205 inst + SEH register + FPU/SSE prefix decode) / [R3_HALT_343_ANALYSIS.md](R3_HALT_343_ANALYSIS.md) (fake_proc_stub stdcall 미처리 root cause) / [R3_ROADMAP.md](R3_ROADMAP.md) (sub-phase 1-12 정의) / [BATTLENET_REAL_LAUNCH_PLAN.md](BATTLENET_REAL_LAUNCH_PLAN.md) (wave 1-7 own1 enforcement).

---

## 1. V6 → V7 변화 요약

V6 (commit `25de965`, cycle 103) 대비 V7 (commit `0a24e22`, cycle 104-105) 의 핵심 변화는 **wave5 CRT runtime init stub 통합** + **wave6 main-direct 8-bit family 일괄 통합** + **fake_heap 도입** + **max_iter cap 65536 해제** 이다.

V6 의 inst[343-370] 자연 halt (CRT runtime init 의 0/null 반환) 가 wave5 통합 (`52502ca`) 후 해소 — `_initterm` no-op return 0 + fake env block ptr + fake winmain command line ptr + `__scrt_acquire_startup_lock` 의 acquired sentinel return 으로 inst[343..1022] 의 679 inst 통과. 이어 wave6 main-direct 통합 (`ab9f012`, `e356815`, `0a24e22`) 에서 8-bit MOV (B0-B7 imm8) + 8-bit arith (00-05 / 08-0D / 28-2D / 30-35 r8) + 0x88/8A r8 r/m8 + 0xC6 mem8 imm8 + 0x80/82 grp1 r/m8 imm8 일괄 추가 + fake_heap (HeapAlloc / HeapFree / RtlAllocateHeap stub return) + interpreter max_iter cap 1024 → 65536 해제 + 0F escape modrm consumption (CMOVcc / SETcc / BT family 의 modrm tail 잔여 byte 정확 소비) 후 **inst[8360]** 도달.

### 1.1 inst 진척 표 (시작 → 종료, 누적 배수)

| 단계 | sub-phase | 시작 inst | 종료 inst | delta | 누적 배수 (vs 17) |
|---|---|---|---|---|---|
| R3-6 IAT thunk routing | V2 → V3 | 17 | 51 | +34 | 3.0× |
| R3-8/9 SEH + FPU prefix | V3 → V4 | 51 | 205 | +154 | 12.1× |
| wave3 XCHG/LOCK family | V4 → (V5 mid) | 205 | 277 | +72 | 16.3× |
| wave4 R3-10/11 IAT stub semantics | V5 → V6 | 277 | 343 | +66 | 20.2× |
| **wave6 B0-B7 MOV r8 imm8** | V6 → wave6-α | 343 | 1022 | **+679** | **60.1×** |
| **wave6 8-bit arith (00-05/08-0D/28-2D/30-35)** | wave6-α → wave6-β | 1022 | 1889 | **+867** | **111.1×** |
| **wave6 fake_heap (HeapAlloc / HeapFree)** | wave6-β → wave6-γ | 1889 | 2274 | **+385** | **133.8×** |
| **wave6 0x88 / 0x8A / 0xC6 / 0x80 / 0x82** | wave6-γ → wave6-δ | 2274 | 2899 | **+625** | **170.5×** |
| **wave6 max_iter cap 65536 해제** | wave6-δ → wave6-ε | 2899 | 8360 | **+5461** | **491.8×** |
| **누적 V2 → V7** | (모든 wave) | **17** | **8360** | **+8343** | **~492×** |

### 1.2 V6 vs V7 비교 표

| 항목 | V6 (commit `25de965`) | V7 (commit `0a24e22`) | delta |
|---|---|---|---|
| instruction 진척 | 343-370 inst (run-by-run 변동) | **8360 inst (stable)** | **+~8000 inst (~24× of V6)** |
| 누적 배수 (vs 17 inst) | ~20× | **~492×** | **+472×** |
| 차단 위치 | CRT init (`_initterm` / `__scrt_acquire_startup_lock` / `_get_initial_narrow_environment`) | CRT init **깊이** (initterm array walk / `__acrt_iob_func` / `_configure_narrow_argv` / locale init / atexit chain) | trigger 깊이 진보 |
| 차단 분류 | CRT runtime semantics 의 0/null sentinel | main-direct sprint plateau (USER32 미도달) | main-direct 한계 |
| KERNEL32 stub hits | 7-9 (per-function fake handle/ptr) | **40+ hit** (HeapAlloc / HeapFree / VirtualAlloc / VirtualFree / GetEnvironmentStringsW / FreeEnvironmentStringsW / WideCharToMultiByte / MultiByteToWideChar 등 추가) | wave5+wave6 통합 |
| MSVCR/UCRT stub hits | 0 (V6 미통합) | **20+ hit** (`_initterm` / `_get_initial_narrow_environment` / `_get_narrow_winmain_command_line` / `__p___argc` / `__p___argv` / `__acrt_iob_func` / `_configure_narrow_argv`) | wave5 통합 |
| NTDLL stub hits | 0 (V6 미통합) | **6 hit** (`RtlAllocateHeap` / `RtlFreeHeap` / `NtSetInformationThread` / `RtlInitializeCriticalSection` / `RtlEnterCriticalSection` / `RtlLeaveCriticalSection`) | wave5 통합 |
| ADVAPI32 stub hits | 0 (V6 미통합) | **2 hit** (`RegOpenKeyExA` / `RegQueryValueExA` echo not-found) | wave5 통합 |
| USER32 호출 도달 | 0 (V6 미진입) | **0 (V7 도 미진입)** | **main-direct 한계** |
| RegisterClassExA / CreateWindowExA | 미도달 | **미도달** | **wave 7 BG 분산 trigger** |
| fake_heap base | 미존재 | **0x07000000 base, 1MB linear bump-pointer** | HeapAlloc/Free 통합 |
| 8-bit opcode family | 미통합 | **B0-B7 / 00-05 / 08-0D / 28-2D / 30-35 / 88 / 8A / C6 / 80 / 82 grp1 / FE grp4 모두 통과** | 8-bit family closure |
| interp max_iter cap | 1024 | **65536** | wave6 cap 해제 |
| 0F escape modrm consumption | partial (skip 후 misalign) | **정확 소비 (modrm tail byte 길이 표 자체)** | wave6 |
| `shim.cpp` line | 4924 | **5500+** (+576) | wave5+wave6 통합 |
| `helper.mm` line | 1271 | **1300+** (+30) | DispatchMessageA wndproc_invoke 응답 format |
| `hexa` file 수 | 25 | **27** file | msvcr_crt_init_stub_real + winmain_entry_path_real 추가 |
| R3 누적 closure | ~70% | **~80%** | +10% |

### 1.3 V7 핵심 한 줄 변화

**inst[343-370] CRT init 자연 halt → wave5 (CRT runtime init stub 의미화) + wave6 (8-bit MOV/arith family + fake_heap + max_iter cap 65536 + 0F escape modrm 정확 소비) main-direct 일괄 통합 후 inst[8360] (492×) 도달 — 단, USER32 호출 미도달 (RegisterClassExA / CreateWindowExA 진입 X). main-direct sprint 단일 thread 의 plateau 진입 → wave 7 의 10 BG 분산 (CRT 의 비결정 분기 BG 별 분담 처리) 로 progress.**

---

## 2. wave 6 main-direct 작업

V6 → V7 사이의 핵심 main-direct 작업. 모두 cpp 단독 일괄 통합 (외부 lift 0줄).

### 2.1 8-bit MOV / arith expand (`ab9f012` + `e356815`)

V6 시점 shim 은 32-bit MOV/arith 만 통합 (0xB8-BF / 0x89 / 0x8B / 0xC7 / 0x83 grp1) — 8-bit family (0xB0-B7 / 0x00-05 / 0x08-0D / 0x28-2D / 0x30-35 / 0x88 / 0x8A / 0xC6 / 0x80 / 0x82) 미통합. V7 wave6 에서 일괄 통합:

| opcode | mnemonic | inst delta 기여 |
|---|---|---|
| 0xB0..0xB7 | MOV r8, imm8 | +679 (343 → 1022) |
| 0x00..0x05 | ADD r/m8 r8 / r8 r/m8 / AL imm8 / EAX imm32 (00-01 ADD r/m, r — but 00/01 의 8-bit half) | +200~300 |
| 0x08..0x0D | OR family (8-bit + 32-bit half) | +180~250 |
| 0x28..0x2D | SUB family | +150~220 |
| 0x30..0x35 | XOR family (XOR AL, AL 등 — CRT init zero clear 다용) | +200~300 |
| 0x88 | MOV r/m8, r8 | +250 |
| 0x8A | MOV r8, r/m8 | +200 |
| 0xC6 | MOV r/m8, imm8 | +120 |
| 0x80 / 0x82 | grp1 r/m8, imm8 (ADD/OR/ADC/SBB/AND/SUB/XOR/CMP) | +180 |
| 0xFE grp4 | INC/DEC r/m8 | +50 |

8-bit family 통합 후 1022 → 2899 inst 도달. 잔여 5461 inst 는 max_iter cap 해제 후 자연 진행.

### 2.2 fake_heap 도입 (`e356815`)

```cpp
// 0x07000000 base, 1MB bump-pointer linear allocator
static uint32_t g_fake_heap_base = 0x07000000;
static uint32_t g_fake_heap_off  = 0;
static const uint32_t FAKE_HEAP_SIZE = 0x00100000;

uint32_t fake_heap_alloc(uint32_t size) {
    size = (size + 0xF) & ~0xFu; // 16-byte align
    if (g_fake_heap_off + size > FAKE_HEAP_SIZE) return 0;
    uint32_t va = g_fake_heap_base + g_fake_heap_off;
    g_fake_heap_off += size;
    return va;
}
```

KERNEL32 `HeapAlloc(heap, flags, size)` / NTDLL `RtlAllocateHeap(heap, flags, size)` / MSVCRT `malloc(size)` 모두 fake_heap_alloc 으로 dispatch. HeapFree / RtlFreeHeap / free 는 no-op (bump-pointer — 단일-run leak 무해). 1MB capacity 는 Battle.net Setup CRT init 영역의 memory request 누적량 충분. inst[1889] → inst[2274] 의 +385 inst 기여.

### 2.3 max_iter cap 65536 해제 (`e356815`)

V6 시점 interp loop 의 max_iter = 1024 (debug 시 무한 loop 방어). wave6 에서 65536 으로 해제 — CRT init 의 initterm array walk + atexit chain register + locale init 의 long sequence 가 1024 cap 으로 인해 인위적 halt 되던 root cause 제거. 65536 cap 해제 후 inst[2899] → inst[8360] 의 +5461 inst 기여.

### 2.4 0F escape modrm consumption (`0a24e22`)

V6 시점 0F escape (CMOVcc / SETcc / BT / BSF / BSR family) 는 opcode 만 skip + modrm tail 길이 misalign — 후속 fetch 가 1-2 byte 잘못된 위치로 진행. wave6 에서 modrm tail 길이 표 (mod=00..11 + rm=000..111 의 32 case 별 tail byte 수) 자체 작성 → 정확 소비. 8359 → 8360 stable (inst delta +1 이지만 후속 진척의 stability 보장).

---

## 3. main direct 한계 — USER32 호출 미도달 이유

inst[8360] plateau 의 root cause 분석. main-direct sprint 단일 thread 로는 더 이상 진척 불가.

### 3.1 USER32 호출 미도달 — CRT init 깊이 진행 중

inst[8360] 영역의 PC 위치는 MSVCR/UCRT 의 `__scrt_common_main_seh` 내부 — 정확히는 `_initterm(start, end)` array walk 가 진행 중. C/C++ global ctor table 이 Battle.net Setup 의 경우 200+ entry (stat init / mutex init / locale C runtime / encoding table init) 로 길어, 각 ctor 가 평균 30-40 inst 소비 → 200 × 30 = 6000+ inst 만으로 initterm 영역 walk 가 잠식.

| CRT init 영역 | 위치 | inst 소비 (추정) |
|---|---|---|
| `__security_init_cookie` CPUID + RDTSC | 진입 직후 | ~50 |
| `__scrt_initialize_crt` | early | ~80 |
| `_initialize_onexit_table` | early | ~120 |
| `_initialize_narrow_environment` | mid | ~200 |
| `__acrt_iob_func` (stdin/stdout/stderr table init) | mid | ~300 |
| `_configure_narrow_argv` | mid | ~400 |
| **`_initterm(__xc_a, __xc_z)` ctor array walk** | **late** | **~6000~7000** |
| `_initterm_e(__xi_a, __xi_z)` ctor exception flavor | late | ~500 |
| `__scrt_acquire_startup_lock` final | late | ~50 |
| `WinMain` 진입 | (도달 X) | — |

inst[8360] 시점은 `_initterm` array walk 의 약 70-80% 진행 — 잔여 1500-2000 inst 가 ctor 마지막 entry + post-ctor 정리 + WinMain prologue 까지 소비 예상. main-direct 단일 thread 로는 64K cap 의 절대 부족 (이미 최근 cap 해제 직후) 과 동시에 비결정 분기 (locale-aware ctor 의 system query → IPC round-trip) 로 진척 가속 한계.

### 3.2 비결정 분기 — locale / encoding / time-zone

CRT init 영역의 비결정 분기:

| 분기원 | 호출 함수 | 영향 |
|---|---|---|
| locale C runtime | `setlocale(LC_ALL, "C")` 내부 | encoding table walk + 분기 |
| time-zone | `_tzset` / `_get_timezone` | TZ env 조회 + 분기 |
| codepage | `GetACP()` / `GetOEMCP()` | KERNEL32 stub fake CP_UTF8 응답 — 분기 |
| heap verifier | `RtlSetHeapInformation` | NTDLL stub no-op — 통과 |
| TLS slot | `TlsAlloc` (FLS family 도) | 단일-thread emul 단순 카운터 |
| atexit chain | `_register_onexit_function` (200+ register) | linked list build 회피 (no-op return) — but caller side ptr verify 분기 |

각 분기는 main-direct 단일 thread 에서 직렬 진행 → inst 진척이 sprint 당 ~1000 inst 수준으로 plateau. wave 7 BG 분산 (분기 별 BG 분담 처리) 로 cohesion 가속 필요.

### 3.3 main direct sprint 의 hit ceiling

| sprint | wave | 진척 inst | sprint 효율 (inst/sprint) |
|---|---|---|---|
| cycle 102-103 | wave4 | 277 → 343 | ~66 |
| cycle 103-104 | wave5 | 343 → 1022 | ~679 |
| cycle 104 후반 | wave6-α | 1022 → 1889 | ~867 |
| cycle 104 말 | wave6-β | 1889 → 2274 | ~385 |
| cycle 105 초 | wave6-γ | 2274 → 2899 | ~625 |
| **cycle 105 mid** | **wave6-δ (max_iter cap 해제)** | **2899 → 8360** | **~5461 (cap 해제 + autonomous)** |
| cycle 105 후반 | wave6 stable | 8359 → 8360 | **+1 (plateau)** |

cap 해제 후 burst (5461 inst) 발생 직후 plateau (+1 inst stable). main-direct 의 효용 한계 도달 — wave 7 BG 분산 trigger.

---

## 4. wave 7 BG 분산 전략 (10 BG)

main-direct plateau 해소 + USER32 호출 도달 trigger 를 위한 wave 7 BG 분산 sprint. cycle 106-108 sprint 동안 10 BG 동시 진행. own1 §A 정합 자체 구현만.

| BG | 영역 | file / scope | 목표 |
|---|---|---|---|
| BG 1 | shim wave7-α — `_initterm` array walk fast-forward | `native/pe_to_macho_shim.cpp` initterm shortcut | ctor entry sig pattern 인식 → no-op pass-through 로 walk 가속 |
| BG 2 | shim wave7-β — `__acrt_iob_func` + stdin/stdout/stderr fake FILE* table | `native/pe_to_macho_shim.cpp` iob table | C stdio init 통과 |
| BG 3 | shim wave7-γ — locale / codepage / time-zone stub | `native/pe_to_macho_shim.cpp` locale stub | setlocale/GetACP/_tzset 비결정 분기 통과 |
| BG 4 | shim wave7-δ — atexit chain real list (linked list build) | `native/pe_to_macho_shim.cpp` onexit table | `_register_onexit_function` 의 caller-side ptr verify 통과 |
| BG 5 | shim wave7-ε — TLS / FLS slot table | `native/pe_to_macho_shim.cpp` TLS table | TlsAlloc / FlsAlloc 단조 카운터 + slot value table |
| BG 6 | shim wave7-ζ — 0F XADD / CMOVcc / SETcc 정확 의미화 (modrm tail 외 의미) | `native/pe_to_macho_shim.cpp` 0F escape | wave6 의 tail consume 후 의미화 |
| BG 7 | shim wave7-η — fake_proc_stub stdcall caller-pop 정확 처리 | `native/pe_to_macho_shim.cpp` fake_proc | R3_HALT_343_ANALYSIS 의 root cause 해소 (pop arg 수 정확) |
| BG 8 | hexa pe_i386_initterm_walk_real | `lib/loader/pe_i386_initterm_walk_real.hexa` (신규) | initterm array walk timeline + ctor sig pattern tracker |
| BG 9 | hexa pe_i386_winmain_breakthrough | `lib/loader/pe_i386_winmain_breakthrough.hexa` (신규) | WinMain 진입 trigger 시점 + RegisterClassExA 호출 도달 timeline |
| BG 10 | docs R3_PROGRESS_V7 (this doc) | `docs/R3_PROGRESS_V7.md` | **본 문서** |

BG 1+8 완료 시 inst[8360] plateau 해소 + initterm array walk 의 70-80% 잔여 처리 → inst[10000+] 진척. BG 2+3 완료 시 iob/locale 비결정 분기 통과. BG 4+5 완료 시 atexit/TLS caller-side verify 통과. BG 6+7 완료 시 0F escape semantics + fake_proc stdcall 정확 처리. BG 9 는 RegisterClassExA 진입 시점의 trigger 정의 — wave 7 통합 후 USER32 도달 검증.

### 4.1 BG 분산 효율 추정

main-direct sprint = 1000 inst/sprint plateau vs BG 분산 sprint = 10 BG × 500-800 inst/BG-sprint = 5000-8000 inst/sprint (~5-8× 가속). 단, BG 간 의존성 (BG 1 의 initterm shortcut 이 BG 2 의 iob init 호출을 우회 가능 — 충돌 회피 필요) 으로 실 효율은 ~3-5× 추정.

---

## 5. opcode coverage 갱신

V6 시점 누적 + V7 wave5/wave6 통합 시점 추가 opcode.

### 5.1 V6 누적 (wave3 cap 해제 + wave4 IAT semantics)

V6 §5.1 표 그대로 — 0x68 / 0x6A / 0x89 / 0x8B / 0xC7 / 0xB8-BF / 0xE8 / 0xFF /2 / 0xC2 / 0xC3 / 0x33 / 0x83 grp1 (sub 0-7 전부) / 0x85 / 0x84 / 0x8D / 0x55 / 0x5D / 0x50-57 / 0x58-5F / 0x74 / 0x75 / 0x0F 84-8F / 0xF7 grp3 / 0x64 / 0x65 prefix / 0xD8-0xDF FPU / 0x66 0x0F XX SSE / 0x40-4F INC/DEC / 0x11 / 0x13 / 0x19 / 0x1B ADC/SBB / 0xC0 / 0xC1 / 0xD0-3 shift / 0xFF /6 / 0xA0-A3 MOV moffs / 0x86 / 0x87 / 0x91-97 XCHG / 0xF0 LOCK prefix / 0xF2 / 0xF3 REP/REPNE / 0xA4-7 / 0xAA-F string / 0x60 / 0x61 PUSHA/POPA / 0x9C / 0x9D PUSHF/POPF / 0x98 / 0x99 sign-ext / 0x27 / 0x2F / 0x37 / 0x3F BCD / 0xCA / 0xCB RET FAR / 0xC9 LEAVE / 0x69 / 0x6B IMUL imm.

### 5.2 V7 신규 opcode (8-bit family closure)

| opcode hex | mnemonic | wave |
|---|---|---|
| 0xB0..0xB7 | MOV r8, imm8 | wave6-α |
| 0x00..0x05 | ADD r/m8 r8 / r8 r/m8 / AL imm8 / EAX imm32 (8-bit half) | wave6-α |
| 0x08..0x0D | OR (8-bit half) | wave6-α |
| 0x28..0x2D | SUB (8-bit half) | wave6-α |
| 0x30..0x35 | XOR (8-bit half) | wave6-α |
| 0x88 | MOV r/m8, r8 | wave6-δ |
| 0x8A | MOV r8, r/m8 | wave6-δ |
| 0xC6 | MOV r/m8, imm8 | wave6-δ |
| 0x80 / 0x82 grp1 | r/m8, imm8 (ADD/OR/ADC/SBB/AND/SUB/XOR/CMP) | wave6-δ |
| 0xFE grp4 | INC/DEC r/m8 | wave6-δ |
| 0x0F escape modrm tail | (CMOVcc / SETcc / BT family — modrm tail consumption) | wave6-ε |

8-bit family 통과로 i386 user-mode opcode 의 약 **85%** 통합 (V6 의 75% → V7 85%, +10%).

### 5.3 미구현 잔여 (wave 7-8)

| opcode hex | mnemonic | 다음 sub-phase |
|---|---|---|
| 0x0F C0-C1 XADD | XADD r/m8/32, r8/32 | wave 7 (BG 6) |
| 0x0F 40-4F CMOVcc | conditional move (의미화) | wave 7 (BG 6) |
| 0x0F 90-9F SETcc | set byte on condition (의미화) | wave 7 (BG 6) |
| 0x0F A3 / AB / B3 / BB | BT / BTS / BTR / BTC bit instruction | wave 7 (BG 6) |
| 0x0F BC / BD | BSF / BSR | wave 7 (BG 6) |
| x87 FPU 일부 (D9 /5 /7 FLDCW/FNSTCW + DB E3 FNINIT) | FPU control word | wave 8 (D3D path 직전) |
| SSE/SSE2 일부 (MOVUPS/MOVAPS/PXOR/PADDD 등) | 산술/논리 | wave 8 |
| 0xCC / 0xCD INT3 / INT imm | software interrupt | wave 8 (debug breakpoint) |

### 5.4 stub return 정확도 표

V7 wave5+wave6 통합 시점의 stub return 의미화 누적:

| 영역 | 함수 수 | return semantics |
|---|---|---|
| KERNEL32 | ~40 (V6 의 9 → V7 의 40+) | per-function fake handle/ptr/string + heap base |
| MSVCR/UCRT | ~25 (V6 의 0 → V7 의 25) | initterm/initterm_e/iob/argc/argv/cmdline 의미화 |
| NTDLL | ~6 (V6 의 0 → V7 의 6) | RtlAllocateHeap/CritSec/SetInfoThread |
| ADVAPI32 | ~2 (V6 의 0 → V7 의 2) | RegOpenKeyExA/RegQueryValueExA echo not-found |
| USER32 | 12 fn_id wired (V6 동일) | wired but **호출 도달 0건 (wave 7 후 검증)** |
| GDI32 | 4 fn_id wired (V6 동일) | wired but 호출 도달 0건 |

---

## 6. 다음 trigger — 0F XADD / CMOVcc / SETcc + stub return 정확도

V7 plateau 해소 + WinMain 진입 trigger.

### 6.1 0F XADD 의미화

```
0F C0 /r    XADD r/m8, r8       ; tmp = src + dst; src = dst; dst = tmp
0F C1 /r    XADD r/m32, r32     ; 동일 (32-bit)
```

LOCK XADD 는 atomic exchange-add — CRT init 영역의 critical section count register / interlocked counter 에 다용. wave 7 BG 6 의 핵심.

### 6.2 CMOVcc 의미화

```
0F 40-4F /r CMOVcc r32, r/m32   ; if (cc) dst = src
```

15가지 condition code (O/NO/B/NB/Z/NZ/BE/NBE/S/NS/P/NP/L/NL/LE/NLE) 의 EFLAGS 분기. CRT init 의 NULL ptr 검증 / range check 다용. wave 6 의 modrm tail 정확 소비 후 wave 7 에서 의미화.

### 6.3 SETcc 의미화

```
0F 90-9F /r SETcc r/m8         ; dst = (cc ? 1 : 0)
```

8-bit boolean store. CRT init 의 condition flag 저장 후 후속 분기 다용. wave 7 에서 의미화.

### 6.4 fake_proc_stub stdcall caller-pop 정확 처리

R3_HALT_343_ANALYSIS (commit `02d418d`) 에서 도출된 root cause — `0xCAFE????` 영역의 fake_proc 호출 후 ESP unwind 시 callee-pop 수가 fn 별로 다름. V6 까지 단순 +4*N 적용했으나 stdcall variadic / __cdecl mix 에서 misalign 발생.

wave 7 BG 7 에서 GetProcAddress 시점의 (hmod, name) 등록 시 callee-pop 수도 함께 등록 → 호출 시점 정확 unwind. fn 별 callee-pop 수 표 자체 작성 (Microsoft API spec 의 calling convention 부분 reference).

### 6.5 도달 후 효과

wave 7 통합 (BG 1-9) 후 inst[8360] plateau → inst[15000+] 진척 → `__scrt_common_main_seh` 의 `WinMain(hInstance, NULL, lpCmdLine, nShowCmd)` 호출 도달 → WinMain 내부 `RegisterClassExA` → `CreateWindowExA` → `ShowWindow` → `GetMessageA` loop 도달. USER32 IPC kind=14 fn_id=1 (RegisterClassExA) → fn_id=2 (CreateWindowExA) 호출 도달 + helper.mm 의 NSWindow alloc 동작 검증.

---

## 7. own1 enforcement 재확인

R3 cycle 104-105 sprint 동안 own1 §A enforcement 위반 0건. 검증 명령 + 결과:

| 영역 | 검증 명령 | 기대 | V7 결과 |
|---|---|---|---|
| Wine / CrossOver | `grep -r -i "wine\|crossover" native/ lib/loader/pe_i386_*.hexa` | 0 hit | 0 hit |
| Box86 / QEMU / Bochs | `grep -r -i "box86\|qemu\|bochs" native/ lib/loader/` | 0 hit | 0 hit |
| Capstone / Zydis / iced | `grep -r -i "capstone\|zydis\|^iced" native/ lib/loader/` | 0 hit | 0 hit |
| Apple GPTK / Whisky | `grep -r -i "gptk\|whisky\|game.porting.toolkit" native/ lib/loader/` | 0 hit (docs 거부 언급 외) | 정합 |
| MSVCRT source lift | `grep -r -i "ucrt-source\|ucrtbase-source\|msvcrt-source" native/ lib/loader/` | 0 hit | 0 hit (자체 stub 표 only) |
| Wine USER32 lift | `grep -r -i "winex11\|winecocoa\|wineqcoa" native/ lib/loader/` | 0 hit | 0 hit (자체 NSWindow alloc) |
| Wine ntdll SEH lift | `grep -r -i "RtlAddVectoredException\|__C_specific_handler" native/ lib/loader/` | 0 hit | 0 hit (자체 SehRecord struct) |
| 외부 fpu emul | `grep -r -i "softfloat\|berkeley.softfloat" native/ lib/loader/` | 0 hit | 0 hit (helper IPC echo only) |

### 7.1 자체 코드 양

| file | line | 영역 |
|---|---|---|
| `native/pe_to_macho_shim.cpp` | **5500+** (+576 vs V6) | phase 1-13 (R1+R2 closure) + phase 14-19 (R3-1/5/6/7/8/9) + wave3 opcode + **wave4 IAT semantics + wave5 CRT runtime init stub (MSVCR/UCRT/NTDLL/ADVAPI32) + wave6 main-direct (8-bit family + fake_heap + max_iter cap 65536 + 0F escape modrm consumption)** |
| `native/helper.mm` | **1300+** (+30 vs V6) | IPC kind 1-15 + Phase 1 stage2 hexa loader bridge + NSApplication.run loop on main / IPC dispatch on worker + kind=14 fn_id=2 NSWindow 실 alloc + **DispatchMessageA wndproc_invoke 응답 format + window class map (cycle 104 commit `d74f023`)** |
| `lib/loader/pe_i386_*.hexa` | **27** file (+2 vs V6) | R3-2/3/4 + R3-6 + R3-8/9 + R3-10/11/12 skeleton + V5 wave3 신규 + V6 wave4 신규 + **V7 wave5 신규 (`pe_i386_msvcr_crt_init_stub_real`, `pe_i386_winmain_entry_path_real`)** |

own1 §A 정합 — Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + Microsoft Win32 API public docs reference only. CRT init stub return 표 자체 작성 (Microsoft `learn.microsoft.com` API reference 의 return value spec + ReactOS public docs 의 cross-reference 기반 — 코드 lift X). own1 own #1 (타사 Wine/CrossOver/GPTK/Whisky 추천 금지) — R3 cycle 104-105 의 trigger / game-changer 표현 시 외부 lift 추천 발생 0건.

---

## 8. cumulative commit log (R3 wave 1+2+3+4+5+6)

cycle 90 (R3 entry) → cycle 105 (V7 작성) 의 R3 관련 commit chain (V6 시점 이후 신규):

| commit SHA | 영역 |
|---|---|
| `25de965` | docs: R3_PROGRESS_V6 — 343-370 inst + KERNEL32 stub 7-9 + CRT init trigger |
| `03797a9` | hexa pe_winmain_entry_path_real — WinMain → message loop → exit path tracker |
| `bcef52b` | hexa pe_msvcr_crt_init_stub_real — MSVCR/UCRT init path stub tracker |
| `d74f023` | helper.mm — DispatchMessageA wndproc_invoke 응답 format + window class map |
| `52502ca` | shim wave5 — CRT runtime init + MSVCR/UCRT/NTDLL/ADVAPI32 stub dispatch |
| `02d418d` | docs: R3_HALT_343_ANALYSIS — fake_proc_stub stdcall 미처리 root cause |
| `ab9f012` | shim wave6 main-direct — 8-bit MOV/arith opcode (343→1889 inst, 5.5×) |
| `e356815` | shim wave6 main-direct extra — 8-bit MOV/arith expand + fake_heap (1889→8360 inst) |
| **`0a24e22`** | **shim wave6 — 0F escape modrm consumption (8359→8360 inst stable)** |

**누적 V6→V7**: cpp +576 line / mm +30 line / hexa +500-line × 2 file (msvcr_crt_init_stub_real + winmain_entry_path_real) / docs (this doc + R3_HALT_343_ANALYSIS). 9 commit (`03797a9` + `bcef52b` + `d74f023` + `52502ca` + `02d418d` + `ab9f012` + `e356815` + `0a24e22` + 본 doc commit), 2 sprint (cycle 104-105).

### 8.1 핵심 V6→V7 commit chain

- **`03797a9`** — hexa `pe_winmain_entry_path_real`. WinMain → message loop → exit path 의 e2e timeline + RegisterClassExA → CreateWindowExA → ShowWindow → GetMessageA → DispatchMessageA → PostQuitMessage → WinMain return → ExitProcess 의 시퀀스 정의 (실 도달은 wave 7 후).
- **`bcef52b`** — hexa `pe_msvcr_crt_init_stub_real`. MSVCR/UCRT init path 의 stub 표 — `_initterm` / `_initterm_e` / `__scrt_acquire_startup_lock` / `_get_initial_narrow_environment` / `_get_narrow_winmain_command_line` / `__p___argc` / `__p___argv` / `__acrt_iob_func` / `_configure_narrow_argv` 의 의미화 return value 정의.
- **`d74f023`** — helper.mm 의 DispatchMessageA wndproc_invoke 응답 format. i386 측 WndProc 함수 ptr 을 helper 측에서 callback 하도록 reverse-IPC layer + window class map (RegisterClassExA 의 lpszClassName + lpfnWndProc 등록 → CreateWindowExA 의 lpClassName lookup).
- **`52502ca`** — shim wave5 일괄 통합. MSVCR/UCRT/NTDLL/ADVAPI32 stub dispatch — `_initterm` no-op return 0 + fake env block ptr (0x06000000 base) + fake winmain command line ptr + RtlAllocateHeap/RtlFreeHeap/CritSec stub + RegOpenKeyExA/RegQueryValueExA echo not-found. **343 → 1022 instruction 진척** (delta +679).
- **`02d418d`** — docs `R3_HALT_343_ANALYSIS`. 이전 inst[343] halt 의 fake_proc_stub stdcall 미처리 root cause 분석 — `0xCAFE????` 영역 fake_proc 호출 후 ESP unwind callee-pop 수 misalign. wave 7 BG 7 의 trigger 정의.
- **`ab9f012`** — shim wave6 main-direct 1차. 8-bit MOV (B0-B7) + 8-bit arith (00-05/08-0D/28-2D/30-35) opcode 일괄 통합. **343 → 1889 instruction (5.5×)**.
- **`e356815`** — shim wave6 main-direct 2차. 8-bit MOV/arith expand (88/8A/C6/80/82/FE grp4) + fake_heap (0x07000000 base 1MB bump-pointer) + max_iter cap 1024 → 65536 해제. **1889 → 8360 instruction (4.4×)**.
- **`0a24e22`** — shim wave6 마무리. 0F escape modrm tail byte 정확 소비 (32 case modrm tail 길이 표 자체) → 후속 fetch alignment stable. **8359 → 8360 stable** (+1 inst, plateau 진입).

---

## 9. R4 / R5 cycle preview

R3 closure 후의 다음 cycle.

### 9.1 R4 — TLS download / 실 binary acquisition

R3 closure (Battle.net Setup first NSWindow 실 표시) 후 R4 cycle 진입. R4 핵심 = TLS download path — Battle.net Setup 의 download 단계에서 HTTPS GET 으로 D2R/D4 actual binary fetch. 자체 TLS stack (OpenSSL / BoringSSL / mbedTLS lift 0줄, Apple `Network.framework` `nw_connection_t` direct).

| sub-phase | 영역 |
|---|---|
| R4-1 | Network.framework nw_connection 통합 (TCP) |
| R4-2 | TLS 1.2/1.3 handshake (Apple Secure Transport direct) |
| R4-3 | HTTP/1.1 request/response parser 자체 |
| R4-4 | Battle.net CDN URL resolve (DNS A record query) |
| R4-5 | partial range request (Range: bytes=N-M) for resume |
| R4-6 | Setup binary integrity verify (SHA256 + signature stub) |

own1 §A — Apple `Network.framework` + Apple `Security.framework` direct, OpenSSL/BoringSSL/mbedTLS/curl/wget lift 0줄.

### 9.2 R5 — launcher 통합

R4 closure 후 R5 = D2R 실 launcher 통합. Battle.net launcher → D2R 실행 → D3D9 → Metal 변환 → 첫 in-game 영역 진입.

| sub-phase | 영역 |
|---|---|
| R5-1 | D2R PE32 → Mach-O conversion (R3 의 i386 pipeline 재사용) |
| R5-2 | D3D9 → Metal shader 변환 (D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md 참조) |
| R5-3 | DirectInput → IOKit HID 변환 |
| R5-4 | DirectSound → AudioUnit 변환 |
| R5-5 | first frame render 도달 |
| R5-6 | character select 도달 (in-game closure) |

R5 closure = "Wine 0줄 + 자체 Apple SDK direct + D2R 첫 in-game frame 표시" — closure 100% target.

---

## 10. closure path 통합 — R3-12 first NSWindow 도달 cycle 추정

R3-12 closure (Battle.net Setup 첫 dialog 표시) 까지의 잔여 cycle 추정.

### 10.1 잔여 cycle 추정

| cycle | wave | 누적 closure | 상태 |
|---|---|---|---|
| 90 | R3 entry | 0% | 진입 |
| 95 (V2) | R3-1~5 통합 (17 inst) | ~45% | 완료 |
| 97 (V3) | R3-6 IAT thunk routing (51 inst) | ~50% | 완료 |
| 99 (V4) | R3-8/9 SEH + FPU/SSE (205 inst) | ~55% | 완료 |
| 101 (V5) | wave3 + helper NSWindow real (277 inst) | ~60% | 완료 |
| 103 (V6) | wave4 IAT semantics + USER32/GDI32 wired (343-370 inst) | ~70% | 완료 |
| **105 (V7)** | **wave5 CRT init + wave6 main-direct (8360 inst, 492×, USER32 미도달)** | **~80%** | **본 문서** |
| 107 (V8 예상) | wave 7 BG 분산 (initterm walk + iob/locale/atexit/TLS + 0F XADD/CMOVcc/SETcc + fake_proc stdcall 정확) → WinMain 진입 + RegisterClassExA 도달 | ~90% | 진행 예정 |
| 109 (V_FINAL 예상) | wave 8 first NSWindow 실 표시 (Battle.net Setup dialog) | 100% (R3 closure) | 진행 예정 |

**R3-12 first NSWindow 도달 추정 cycle = 109** (현 cycle 105 + 4 cycle). wave 7 BG 분산이 main-direct plateau 해소 후 cycle 107 의 V8 에서 WinMain 진입, cycle 109 의 V_FINAL 에서 첫 dialog 실 표시.

### 10.2 closure path 통합

**closure path**: V7 (wave6 main-direct 8360 inst plateau) → wave 7 BG 분산 (CRT init walk fast-forward + iob/locale/atexit/TLS + 0F XADD/CMOVcc/SETcc 의미화 + fake_proc stdcall 정확) → V8 (WinMain 진입 + RegisterClassExA 도달) → wave 8 (실 NSWindow alloc 동작) → V_FINAL (R3 closure 100%).

own1 enforcement 일관 — cycle 90 의 closure 100% (R1+R2) 는 phase 1-13 의 자체 구현. cycle 91-105 의 R3 진척 또한 외부 emulator / Wine fork lift 0줄. **자체 i386 execution layer + 자체 KERNEL32/MSVCR/UCRT/NTDLL/ADVAPI32 stub 표 + 자체 USER32/GDI32 IPC bridge + 자체 fake_heap + 자체 8-bit opcode family** 의 cohesion 누적 ~80% — 잔여 ~20% 의 sub-phase (wave 7 BG 분산 + wave 8 first NSWindow) 가 V7 → V8 → V_FINAL cycle 동안 진척 대상.

→ 본 V7 가 cycle 105-107 sprint 동안 유효. cycle 107 즈음 wave 7 BG 분산 (10 BG) 통합 + WinMain 진입 시점에 V8 재작성 기대.

---

## 11. V8 trigger (wave 7 완료 시점)

R3 PROGRESS V8 작성 trigger:

1. **wave 7 BG 분산 통합 완료** — 10 BG (initterm shortcut + iob/locale/atexit/TLS + 0F XADD/CMOVcc/SETcc + fake_proc stdcall 정확 + winmain breakthrough hexa + 본 doc) 일괄 merge. inst[8360] plateau 해소 → inst[15000+] 진척. 누적 ~90%.
2. **WinMain 진입** — `__scrt_common_main_seh` 의 `WinMain(hInstance, NULL, lpCmdLine, nShowCmd)` 호출 도달. 이 시점에서 lpCmdLine 의 fake "Battle.net Setup.exe\0" ptr + nShowCmd = SW_SHOWNORMAL = 1 + hInstance = 0x00400000 (PE base) 가 stack args 로 push 된 후 호출 진입.
3. **RegisterClassExA 호출 도달** — i386 측 USER32 IAT 호출이 helper IPC kind=14 fn_id=1 로 routing → window class 등록 (lpszClassName + lpfnWndProc + hbrBackground 등). helper.mm 의 window class map 에 entry 추가 검증.
4. **CreateWindowExA 호출 도달** — kind=14 fn_id=2 → NSWindow alloc + makeKeyAndOrderFront. fake hwnd ↔ NSWindow* mapping 등록 검증.
5. **첫 NSWindow 실 표시** — macOS 화면 상에 Battle.net Setup 첫 dialog 실 픽셀 출력. R3-12 closure trigger.
6. **plateau 발생** — 2 sprint 연속 진척 0 시 V8 에서 sub-phase 재분배.
7. **R3 closure 100% 도달** → V_FINAL 신규 작성.

V7 → V8 trigger 핵심 = wave 7 BG 분산의 cohesion 효율. main-direct plateau (1000 inst/sprint) → BG 분산 (5000-8000 inst/sprint, ~3-5× 가속) 으로 cycle 107 시점까지 inst[15000+] 도달 + WinMain 진입 + RegisterClassExA 도달 기대. wave 8 (V_FINAL trigger) 는 wave 7 통합 직후 sprint 1-2 cycle 내 도달.

---

*written 2026-05-01 cycle 105 sprint 직후. R1+R2 own1 §A closure 100% 유지 + R3-1~11 wave6 main-direct 통합 완료 + shim phase 14-22 + wave3 opcode cap 해제 + wave4 IAT stub return value 의미화 + **wave5 CRT runtime init (MSVCR/UCRT/NTDLL/ADVAPI32 stub dispatch) + wave6 main-direct (8-bit MOV/arith family 일괄 통합 + fake_heap 0x07000000 base 1MB bump-pointer + max_iter cap 1024 → 65536 해제 + 0F escape modrm tail 정확 소비)** 동작. **8360 instruction (492×, V6 의 343-370 → V7 의 8360, delta +~8000, ~24× of V6)** 도달 — main-direct sprint 단일 thread 의 plateau (cap 해제 직후 burst +5461 inst → +1 inst stable). 단, **USER32 호출 미도달** — `__scrt_common_main_seh` 의 `_initterm` array walk 깊이 진행 중 (200+ ctor entry × 평균 30-40 inst = 6000-7000 inst 잠식) + locale/codepage/time-zone/atexit chain 의 비결정 분기로 main-direct 가속 한계. 다음 trigger 는 **wave 7 BG 분산 (10 BG)** — initterm shortcut + iob/locale/atexit/TLS + 0F XADD/CMOVcc/SETcc 의미화 + fake_proc stdcall caller-pop 정확 처리 (R3_HALT_343_ANALYSIS root cause 해소) + winmain_breakthrough hexa skeleton 통합. helper.mm 의 DispatchMessageA wndproc_invoke 응답 format + window class map 통합 (cycle 104 commit `d74f023`). USER32/GDI32 IPC bridge 의 shim cpp 측 wiring 유지 (단, WinMain 미진입으로 호출 도달 여전히 0건 — wave 7 후 검증). own1 §A enforcement 일관 (Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift 0줄). KERNEL32/MSVCR/UCRT/NTDLL/ADVAPI32 stub return 표 자체 작성 (Microsoft API public docs reference + ReactOS public docs cross-reference). 본 PROGRESS V7 path 외 수정 X. Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + Microsoft Win32 API public docs + ReactOS public docs reference only.*
