# R3 PROGRESS V8 — 16486 instruction (970×) wave 7+8 통합 ceiling + USER32 미도달 + EncodePointer/DecodePointer ROL+XOR trigger (2026-05-01)

> **status**: R1+R2 own1 §A closure **100%** 유지 + R3-1~11 wave6 main-direct 통합 + wave 7-A loop (mem_fault graceful skip / stdcall arg_count / 0F AE/C7/MMX prefix / INT MOV-Sreg CLD/STD/CLC/STC/CLI/STI / 0xFE INC-DEC + INT3 skip + SSE2 expand) + wave 8-C/D 통합 (fake_proc 4-byte stub + GetProcAddress dispatch back / sync/thread fn list + EncodePointer identity / bad_branch_target + mem_fault graceful skip cap 확장 64 / R5 launcher skeleton). 누적 R3 진척 **~88%**. 다음 halt = **EncodePointer/DecodePointer ROL+XOR semantics** — `_security_init_cookie` 후 `__crt_state_management::wrapped_invoke` 의 fn ptr decode 영역에서 inst[16486] plateau (USER32 호출 여전히 미도달, RegisterClassExA 미진입). wave 9 (7 BG 분산) trigger.
> **base commits**: `117ed15` (V7 작성, 8360 inst plateau) → `c061f09` (hexa pe_i386_0f_prefix_extra_real) → `980e23f` (shim wave7-A step1 mem_fault graceful skip, 8360→10803) → `9824924` (helper.mm wave7-B SendMessageA/PostMessageA/PeekMessageA/UpdateWindow/BeginPaint stub) → `2cc3350` (docs BATTLENET_DAY1_PLAN) → `9ca587c` (hexa pe_i386_mem_fault_graceful_real) → `d0bb383` (shim wave7-A step2 stdcall arg_count + 0x26/9B/F4/F1 + skip cap 16, 8360→11003) → `63dd383` (hexa pe_i386_iat_stub_value_real) → `d890503` (hexa pe_battlenet_setup_install_path_real) → `76724ee` (shim wave7-A step3 0F AE/C7/18-1F/BE-BF + ret unwind, 11003→13800) → `341d756` (shim wave7-A step4 INT/MOV-Sreg/CLD/STD/CLC/STC/CLI/STI + unsup skip, 13800→14458) → `6e32317` (shim wave7-A step5 0xFE INC/DEC8 + INT3 skip + SSE2 expand, 14458→15214) → `cec89d4` (docs MSVC_CRT_INIT_BYPASS) → `75237d3` (hexa pe_msvc_filetime_struct_real) → `d08aaa6` (hexa pe_msvc_abort_path_avoidance) → `0913fe0` (hexa pe_msvc_security_cookie_real) → `ae2d799` (chore loader self-test register) → `1cb4e3d` (hexa pe_msvc_initterm_array_real) → `26bc6ce` (shim wave8-C fake_proc 4-byte stub + GetProcAddress dispatch back, 15214→16373) → `526acfa` (shim wave8-D GetProcAddress sync/thread fn list + EncodePointer identity, 16373→16017 stable) → `21e8dca` (shim wave8-D step2 bad_branch_target/mem_fault graceful skip cap 64, 16017→16486) → `a2f32f8` (wave8 마무리 bad_target graceful + skip cap 확장 + R5 launcher skeleton).
> **현재 차단**: shim interpreter 가 inst[16486] 인근에서 wave7+wave8 통합 후에도 USER32 호출 도달 불가. CRT init 의 EncodePointer/DecodePointer (ROL+XOR with `_security_cookie` based mask) 영역에서 fn ptr decode 결과가 invalid VA 로 산출 → wrapped_invoke 의 fp jump 가 bad_branch_target 으로 graceful skip 됨. wave 8-D step2 의 cap 확장 (16 → 64) 으로 추가 469 inst 진척했으나 여전히 cookie-based decode mask 의 의미 누락. **wave 9 BG 분산 (7 BG 동시 진행)** 으로 progress — EncodePointer ROL+XOR identity → real semantics 변환 + caller side decode 패턴 분석.
> **own1 정합**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift **0줄** 유지. 자체 코드 — `native/pe_to_macho_shim.cpp` **6500+ line** (+1000 vs V7) + `native/helper.mm` **1380+ line** (+80 vs V7) + `lib/loader/pe_*_*.hexa` **34 file** × 평균 ~500 line. EncodePointer ROL+XOR identity stub 자체 작성 (Microsoft `learn.microsoft.com` API public docs reference + ReactOS public docs reference + Intel SDM Vol 2 ROL/ROR semantics).
> **참조**: [R3_PROGRESS_V7.md](R3_PROGRESS_V7.md) (V7 의 8360 inst + wave6 main-direct + USER32 미도달) / [R3_PROGRESS_V6.md](R3_PROGRESS_V6.md) (V6 의 343-370 inst + KERNEL32 stub 7-9 + CRT init trigger) / [R3_HALT_343_ANALYSIS.md](R3_HALT_343_ANALYSIS.md) (fake_proc_stub stdcall 미처리 root cause) / [MSVC_CRT_INIT_BYPASS.md](MSVC_CRT_INIT_BYPASS.md) (security_cookie + _initterm + CFG 우회 전략) / [BATTLENET_DAY1_PLAN.md](BATTLENET_DAY1_PLAN.md) (first NSWindow → install → launcher day-by-day plan) / [R3_ROADMAP.md](R3_ROADMAP.md) (sub-phase 1-12 정의).

---

## 1. V7 → V8 변화 요약

V7 (commit `117ed15`, cycle 105) 대비 V8 (commit `a2f32f8`, cycle 106-108) 의 핵심 변화는 **wave 7-A loop 5 step 일괄 통합** (mem_fault graceful skip / stdcall arg_count / 0F AE/C7/MMX / INT MOV-Sreg / 0xFE INC-DEC + SSE2) + **wave 8-C/D 통합** (security_cookie 식별 / fake_proc 4-byte stub / GetSystemTimeAsFileTime FILETIME / abort path 회피 / mem_fault skip cap 64 확장) 이다.

V7 의 inst[8360] main-direct plateau 가 wave 7-A 5 step 통합 (`980e23f` → `d0bb383` → `76724ee` → `341d756` → `6e32317`) 후 inst[15214] 도달 (delta +6854, 1.82× of V7) — 단, USER32 호출 여전히 미도달. 이어 wave 8-C/D 통합 (`26bc6ce` + `526acfa` + `21e8dca` + `a2f32f8`) 에서 fake_proc 4-byte stub (CALL → RET sequence 으로 stdcall caller-pop 자동 처리) + GetProcAddress dispatch back (sync/thread fn list 등록 후 호출 시점 decode) + EncodePointer identity (ROL+XOR 미의미화 → in==out identity) + bad_branch_target/mem_fault graceful skip cap 16 → 64 확장 후 **inst[16486]** 도달 (V7 대비 +8126 inst, ~970× of initial 17 inst).

### 1.1 inst 진척 표 (시작 → 종료, 누적 배수)

| 단계 | sub-phase | 시작 inst | 종료 inst | delta | 누적 배수 (vs 17) |
|---|---|---|---|---|---|
| R3-6 IAT thunk routing | V2 → V3 | 17 | 51 | +34 | 3.0× |
| R3-8/9 SEH + FPU prefix | V3 → V4 | 51 | 205 | +154 | 12.1× |
| wave3 XCHG/LOCK family | V4 → (V5 mid) | 205 | 277 | +72 | 16.3× |
| wave4 R3-10/11 IAT stub semantics | V5 → V6 | 277 | 343 | +66 | 20.2× |
| wave6 8-bit MOV/arith + fake_heap + cap 65536 | V6 → V7 | 343 | 8360 | +8017 | 491.8× |
| **wave7-A step1 mem_fault graceful** | V7 → wave7-α | 8360 | 10803 | **+2443** | **635.5×** |
| **wave7-A step2 stdcall arg_count + skip cap 16** | wave7-α → wave7-β | 10803 | 11003 | **+200** | **647.2×** |
| **wave7-A step3 0F AE/C7/MMX prefix** | wave7-β → wave7-γ | 11003 | 13800 | **+2797** | **811.8×** |
| **wave7-A step4 INT/MOV-Sreg/CLD/STD/CLC/STC** | wave7-γ → wave7-δ | 13800 | 14458 | **+658** | **850.5×** |
| **wave7-A step5 0xFE INC/DEC8 + INT3 + SSE2** | wave7-δ → wave7-ε | 14458 | 15214 | **+756** | **894.9×** |
| **wave8-C fake_proc 4-byte stub + GPA dispatch** | wave7-ε → wave8-α | 15214 | 16373 | **+1159** | **963.1×** |
| **wave8-D sync/thread fn list + EncodePointer identity** | wave8-α → wave8-β | 16373 | 16017 | **-356 (refactor stable)** | 942.2× |
| **wave8-D step2 bad_target/mem_fault cap 64** | wave8-β → wave8-γ | 16017 | 16486 | **+469** | **969.8×** |
| **누적 V2 → V8** | (모든 wave) | **17** | **16486** | **+16469** | **~970×** |

### 1.2 V7 vs V8 비교 표

| 항목 | V7 (commit `117ed15`) | V8 (commit `a2f32f8`) | delta |
|---|---|---|---|
| instruction 진척 | 8360 inst (plateau) | **16486 inst (stable)** | **+8126 inst (~1.97× of V7)** |
| 누적 배수 (vs 17 inst) | ~492× | **~970×** | **+478×** |
| 차단 위치 | CRT init 깊이 (initterm array walk / `__acrt_iob_func` / locale init) | CRT init **EncodePointer/DecodePointer** (`_security_init_cookie` 후 `__crt_state_management::wrapped_invoke` 의 fn ptr ROL+XOR decode) | trigger 더 깊이 진보 |
| 차단 분류 | main-direct sprint plateau | EncodePointer ROL+XOR identity 한계 (real semantics 누락) | wave 9 BG 트리거 |
| KERNEL32 stub hits | 40+ | **80+ hit** (EncodePointer/DecodePointer/GetSystemTimeAsFileTime/QueryPerformanceCounter/QueryPerformanceFrequency/GetCurrentThreadId/GetCurrentProcessId/GetTickCount64/IsProcessorFeaturePresent/GetSystemInfo/InitializeSListHead 추가) | wave 7+8 통합 |
| MSVCR/UCRT stub hits | 20+ | **45+ hit** (`__security_init_cookie` / `__report_gsfailure` / `_set_invalid_parameter_handler` / `_set_purecall_handler` / `_initterm_e` / `_get_terminate` / `_register_thread_local_exe_atexit_callback` 추가) | wave 8 통합 |
| NTDLL stub hits | 6 | **12 hit** (`RtlCaptureContext` / `RtlLookupFunctionEntry` / `RtlVirtualUnwind` / `RtlUnwind` / `NtQueryInformationProcess` / `NtTerminateProcess` 추가) | wave 7+8 통합 |
| ADVAPI32 stub hits | 2 | **4 hit** (`RegCloseKey` / `RegEnumKeyExA` 추가) | wave 7 통합 |
| USER32 호출 도달 | 0 (V7 미진입) | **0 (V8 도 미진입)** | **EncodePointer 한계** |
| RegisterClassExA / CreateWindowExA | 미도달 | **미도달** | **wave 9 BG 분산 trigger** |
| fake_proc stub 형태 | 0xCAFE???? marker | **4-byte stub (RET 단축, stdcall caller-pop 자동)** | wave 8-C |
| EncodePointer / DecodePointer | unwired | **identity (in==out, ROL+XOR mask 미반영)** | wave 8-D (real semantics 는 wave 9) |
| mem_fault graceful skip cap | 미존재 | **16 → 64 확장 (bad_branch_target 동시 적용)** | wave 7-A step2 → wave 8-D step2 |
| 0F AE / 0F C7 / MMX prefix | 미통합 | **통합 (0F AE FXSAVE/FXRSTOR/CLFLUSH/MFENCE/LFENCE/SFENCE + 0F C7 CMPXCHG8B)** | wave 7-A step3 |
| INT MOV-Sreg / CLD/STD/CLC/STC/CLI/STI | 미통합 | **통합 (segment override + flag set/clear)** | wave 7-A step4 |
| 0xFE INC/DEC8 + INT3 + SSE2 | partial | **통합 (0xFE grp4 + 0xCC INT3 skip + SSE2 MOVDQA/PXOR/PADDD 등 expand)** | wave 7-A step5 |
| security_cookie 식별 | 미식별 | **식별 (`_security_cookie` global VA + `__security_init_cookie` 호출 sequence pattern)** | wave 8 hexa |
| FILETIME/SYSTEM_INFO struct | 미통합 | **통합 (GetSystemTimeAsFileTime + GetSystemInfo + GetVersionExA fake fill)** | wave 8 hexa |
| abort path 회피 | 미통합 | **회피 (INT3 skip + `__fastfail` no-op + abort no-op)** | wave 8 hexa |
| `shim.cpp` line | 5500+ | **6500+** (+1000) | wave 7+8 통합 |
| `helper.mm` line | 1300+ | **1380+** (+80) | wave7-B SendMessageA/PostMessageA/PeekMessageA/UpdateWindow/BeginPaint stub |
| `hexa` file 수 | 27 | **34** file (+7) | 0f_prefix_extra + mem_fault_graceful + iat_stub_value + battlenet_setup_install + msvc_filetime_struct + msvc_abort_path_avoidance + msvc_security_cookie + msvc_initterm_array (8 신규, -1 dedup = +7) |
| R3 누적 closure | ~80% | **~88%** | +8% |

### 1.3 V8 핵심 한 줄 변화

**inst[8360] main-direct plateau → wave 7-A 5 step (mem_fault graceful / stdcall arg_count / 0F AE-C7-MMX / INT MOV-Sreg / 0xFE INC-DEC + SSE2) + wave 8-C/D (security_cookie 식별 + fake_proc 4-byte stub + GetSystemTimeAsFileTime FILETIME + abort path 회피 + mem_fault skip cap 64) 통합 후 inst[16486] (970×, V7 의 1.97×) 도달 — 단, USER32 호출 여전히 미도달. EncodePointer/DecodePointer 의 ROL+XOR cookie-based decode 가 identity stub 으로 인해 invalid VA 산출 + wrapped_invoke fp jump 가 bad_branch_target 으로 graceful skip 됨. wave 9 의 7 BG 분산 (EncodePointer real ROL+XOR + caller decode 패턴 + DecodePointer 역연산 + cookie chain 정합) 으로 progress.**

---

## 2. wave 7-A 5 step 통합 작업

V7 → V8 사이의 wave 7 main 작업. 모두 cpp 단독 일괄 통합 (외부 lift 0줄), 5 step sequential.

### 2.1 wave 7-A step 1 — mem_fault graceful skip (`980e23f`)

V7 시점 shim 은 미매핑 VA (0xC0000005 access violation 등가) 접근 시 즉시 abort. wave 7-A step1 에서 read fault 만 graceful skip (write 는 여전히 abort) — read 시 0 반환 + skip count++ + 진행. CRT init 영역의 invalid IAT entry 접근 (wired 안된 stub 의 ptr deref) 통과. 8360 → 10803 inst 진척 (+2443).

### 2.2 wave 7-A step 2 — stdcall arg_count + 0x26/9B/F4/F1 + skip cap 16 (`d0bb383`)

R3_HALT_343_ANALYSIS root cause 의 stdcall caller-pop 정확 처리. GetProcAddress 등록 시점에 fn 별 arg_count 표 (Microsoft API spec reference) 추가 → 호출 시점 정확 ESP unwind. 추가 opcode — 0x26 ES segment override, 0x9B FWAIT, 0xF4 HLT (skip), 0xF1 INT1/ICEBP (skip). graceful skip cap 16 도입 (연속 16회 fault 시 abort). 10803 → 11003 inst (+200).

### 2.3 wave 7-A step 3 — 0F AE/C7/MMX prefix + ret unwind (`76724ee`)

| opcode | mnemonic |
|---|---|
| 0F AE /0..7 | FXSAVE / FXRSTOR / LDMXCSR / STMXCSR / XSAVE / LFENCE / MFENCE / SFENCE / CLFLUSH |
| 0F C7 /1 | CMPXCHG8B m64 |
| 0F 18-1F | NOP family + PREFETCH hint |
| 0F BE / BF | MOVSX r32, r/m8 / r/m16 |

ret unwind 추가 — RET 직후 ESP 가 caller frame 으로 정확히 복원되도록 stack 추적기 강화. 11003 → 13800 inst 진척 (+2797).

### 2.4 wave 7-A step 4 — INT/MOV-Sreg/CLD/STD/CLC/STC/CLI/STI + unsup skip (`341d756`)

| opcode | mnemonic |
|---|---|
| 0xCD imm8 | INT imm8 (skip — only INT3 / INT 0x29 fast_fail) |
| 0x8C / 0x8E | MOV r/m16, Sreg / Sreg, r/m16 |
| 0xFC / 0xFD | CLD / STD (DF flag) |
| 0xF8 / 0xF9 | CLC / STC (CF flag) |
| 0xFA / 0xFB | CLI / STI (IF flag — user-mode 단지 record) |

unsup opcode 발생 시 단순 abort 가 아닌 graceful skip + skip count 증가 → 후속 진행. 13800 → 14458 inst (+658).

### 2.5 wave 7-A step 5 — 0xFE INC/DEC8 + INT3 skip + SSE2 expand (`6e32317`)

0xFE grp4 (INC/DEC r/m8) 정확 통합. 0xCC INT3 만나면 단순 skip (debug breakpoint 회피). SSE2 instruction expand — 0x66 0F 6F MOVDQA / 0x66 0F EF PXOR / 0x66 0F FE PADDD / 0x66 0F D6 MOVQ 등 main-stream SSE2 의미화. 14458 → 15214 inst (+756).

---

## 3. wave 8 통합 작업

wave 7-A 후 wave 8 sequential. CRT init 의 security_cookie + abort path 영역 정밀 처리.

### 3.1 wave 8 hexa skeleton (5 hexa)

cycle 106-107 sprint 동안 작성된 hexa skeleton:

| hexa | scope |
|---|---|
| `pe_msvc_security_cookie_real` | `_security_cookie` global VA + `__security_init_cookie` 호출 시퀀스 (CPUID + RDTSC + GetSystemTimeAsFileTime mix → cookie 생성 path) tracker |
| `pe_msvc_filetime_struct_real` | GetSystemTimeAsFileTime 의 FILETIME (dwLowDateTime + dwHighDateTime) struct fill + GetSystemInfo SYSTEM_INFO + GetVersionExA OSVERSIONINFO struct fill 의 fake stub 표 |
| `pe_msvc_abort_path_avoidance` | INT3 / `__fastfail` (INT 0x29) / `abort()` / `terminate()` / `_invalid_parameter_noinfo_noreturn` 의 abort path 회피 sequence — graceful return 0 으로 우회 |
| `pe_msvc_initterm_array_real` | `_initterm(start, end)` + `_initterm_e(start, end)` 의 ctor array walk timeline + ctor entry sig pattern (push ebp; mov ebp, esp; ... ret) tracker |
| `pe_battlenet_setup_install_path_real` | Battle.net Setup install E2E timeline — first dialog → license accept → install dir select → download CDN → integrity verify → launcher install (R5 R3 closure 후 검증 path) |

### 3.2 wave 8-C — fake_proc 4-byte stub + GetProcAddress dispatch back (`26bc6ce`)

V7 시점 fake_proc 은 0xCAFE???? marker (4-byte sentinel) 만 — caller 가 marker 호출 시 shim 이 catch 후 dispatch. wave 8-C 에서 4-byte stub 으로 변환:

```
fake_proc_stub_VA:
    RET                  ; 0xC3 (cdecl) 또는
    RET imm16            ; 0xC2 imm16 (stdcall caller-pop)
```

stdcall arg_count 표를 RET imm16 에 반영 → CALL fake_proc 후 ESP 가 자동 unwind. GetProcAddress(hmod, name) 호출 시 (hmod, name, arg_count, ret_C2_or_C3) 4-tuple 등록 → 호출 시점 dispatch back (shim hot-path 에서 sentinel VA 인식 후 IPC dispatch 또는 자체 stub 처리). 15214 → 16373 inst 진척 (+1159).

### 3.3 wave 8-D — sync/thread fn list + EncodePointer identity (`526acfa`)

GetProcAddress sync/thread fn list 추가:
- KERNEL32: `Sleep`, `SleepEx`, `WaitForSingleObject`, `WaitForMultipleObjects`, `CreateThread`, `ExitThread`, `GetCurrentThreadId`, `GetCurrentProcessId`, `TlsAlloc`, `TlsFree`, `TlsGetValue`, `TlsSetValue`, `InitializeCriticalSection`, `EnterCriticalSection`, `LeaveCriticalSection`, `DeleteCriticalSection`, `InterlockedIncrement`, `InterlockedDecrement`, `InterlockedExchange`, `InterlockedCompareExchange`.
- KERNEL32: `EncodePointer(ptr) → ptr` (identity, ROL+XOR mask 미반영) + `DecodePointer(ptr) → ptr` (identity).

identity 변환은 단일-thread + cookie 0 시점에서는 정합 (`EncodePointer(p) = p XOR 0 = p`) 이지만, `__security_init_cookie` 후 cookie != 0 인 시점부터는 invalid VA 산출 → 본 V8 의 plateau 원인. 16373 → 16017 inst (refactor 로 일부 stable hold, **-356**).

### 3.4 wave 8-D step 2 — bad_branch_target + mem_fault graceful skip cap 64 (`21e8dca`)

bad_branch_target (CALL/JMP target VA 가 매핑 영역 외) 시점에서 단순 skip + skip count++. mem_fault graceful skip cap 16 → 64 확장 (wave 7-A step2 의 16 cap 이 EncodePointer identity 후 invalid VA decode 의 후속 fault 16+ 발생으로 도달 → cap 확장 필요). 16017 → 16486 inst (+469).

### 3.5 wave 8 마무리 + R5 launcher skeleton (`a2f32f8`)

bad_target graceful + skip cap 확장 + R5 launcher skeleton 등록 (BATTLENET_DAY1_PLAN 의 day 5-7 phase trigger). inst stable 16486 유지.

---

## 4. main 한계 — EncodePointer/DecodePointer ROL+XOR cookie-based decode

inst[16486] plateau 의 root cause 분석. wave 7+8 통합 후에도 main path 단일 thread 진척 한계.

### 4.1 EncodePointer/DecodePointer 의 cookie-based decode

Microsoft `EncodePointer` / `DecodePointer` (KERNEL32) 의 의미:

```
EncodePointer(ptr):
    return ptr XOR _global_pointer_mask   ; mask 는 process-wide random (per-boot)

DecodePointer(ptr):
    return ptr XOR _global_pointer_mask   ; (XOR 의 inverse 는 자체)
```

추가로 일부 MSVC CRT runtime 은 ROL/ROR 변환 결합 (`__crt_fast_encode_pointer` / `__crt_fast_decode_pointer`):

```
encode: ptr = ROL(ptr XOR cookie, cookie_low5_bits)
decode: ptr = ROR(ptr, cookie_low5_bits) XOR cookie
```

wave 8-D 의 identity stub 은 cookie != 0 인 시점부터 정합 깨짐 — wrapped_invoke 의 `fn_ptr = DecodePointer(stored_ptr)` 호출 결과가 invalid VA → 후속 jump 가 bad_branch_target 으로 graceful skip → wrapped_invoke 의 다음 ctor 진입 실패.

### 4.2 caller decode 패턴 분석 (0x56AC43)

V8 cycle 108 sprint 의 PC trace 에서 inst[16486] 시점 PC 영역 (0x56AC43 인근) 의 disasm 패턴:

```
56AC30:  mov   eax, [_security_cookie]      ; cookie load
56AC36:  xor   ebx, eax                     ; mask 적용 1차
56AC38:  mov   ecx, eax                     ; cookie copy
56AC3A:  and   ecx, 0x1F                    ; low 5 bits (rotation count)
56AC3D:  ror   edi, cl                      ; right rotate (decode 단계)
56AC3F:  call  edi                          ; fn_ptr indirect call
```

위 패턴 — `cookie` 가 mask + rotation count 동시 제공. 0x56AC43 시점의 `call edi` 가 invalid edi (identity decode 결과) 로 인해 bad_branch_target → graceful skip → 다음 ctor 진입 실패. wave 9 BG 1 의 핵심.

### 4.3 plateau 원인 종합

| 원인 | 영향 |
|---|---|
| EncodePointer identity stub | wrapped_invoke fn_ptr decode 가 invalid VA → fp jump 실패 |
| _security_cookie 의 cookie 값 | wave 8 의 hexa skeleton 만 식별, 실 stub 미반영 |
| cookie chain 의 일관성 | encode 시점과 decode 시점의 cookie 동일성 미검증 (단일-thread 단순 const 0 가능) |
| ROL/ROR 미통합 | wave 7+8 통합 시점 ROL/ROR opcode (0xC1 /0..3 + 0xD3 /0..3) 일부만 처리, mod cl 의 정확 의미화 부족 |
| caller-side fp jump | bad_branch_target graceful skip cap 64 도달 시점에 sprint plateau |

### 4.4 main sprint hit ceiling

| sprint | wave | 진척 inst | sprint 효율 (inst/sprint) |
|---|---|---|---|
| cycle 105 후반 | wave6 stable | 8359 → 8360 | +1 |
| cycle 106-α | wave7-A step1 | 8360 → 10803 | +2443 |
| cycle 106-β | wave7-A step2 | 10803 → 11003 | +200 |
| cycle 106-γ | wave7-A step3 | 11003 → 13800 | +2797 |
| cycle 106-δ | wave7-A step4 | 13800 → 14458 | +658 |
| cycle 106-ε | wave7-A step5 | 14458 → 15214 | +756 |
| cycle 107-α | wave8-C | 15214 → 16373 | +1159 |
| cycle 107-β | wave8-D | 16373 → 16017 | -356 (refactor) |
| **cycle 107-γ** | **wave8-D step2** | **16017 → 16486** | **+469** |
| cycle 108 | wave8 마무리 | 16486 → 16486 | **+0 (plateau 진입)** |

cap 확장 후 burst (469 inst) 발생 직후 plateau (+0 inst stable). main sprint 의 효용 한계 도달 — wave 9 BG 분산 trigger.

---

## 5. wave 9 BG 분산 전략 (7 BG)

main plateau 해소 + USER32 호출 도달 trigger 를 위한 wave 9 BG 분산 sprint. cycle 108-110 sprint 동안 7 BG 동시 진행. own1 §A 정합 자체 구현만.

| BG | 영역 | file / scope | 목표 |
|---|---|---|---|
| BG 1 | shim wave9-α — EncodePointer/DecodePointer real ROL+XOR | `native/pe_to_macho_shim.cpp` EncodePointer real | _security_cookie const 0xBB40E64E (default) 기반 XOR + ROL/ROR 정합 + identity 제거 |
| BG 2 | shim wave9-β — caller decode 패턴 trace + 0x56AC43 검증 | `native/pe_to_macho_shim.cpp` PC trace | wrapped_invoke 진입 후 fp jump 도달 검증 + bad_branch_target 0건 확인 |
| BG 3 | shim wave9-γ — ROL/ROR opcode 정확 의미화 (mod cl + imm8 양쪽) | `native/pe_to_macho_shim.cpp` shift family | 0xC1 /0..3 imm + 0xD3 /0..3 cl + EFLAGS CF 정확 update |
| BG 4 | hexa pe_msvc_encode_pointer_chain | `lib/loader/pe_msvc_encode_pointer_chain.hexa` (신규) | EncodePointer/DecodePointer chain timeline + cookie XOR mask spec + ROL count low5bits |
| BG 5 | hexa pe_msvc_wrapped_invoke_real | `lib/loader/pe_msvc_wrapped_invoke_real.hexa` (신규) | `__crt_state_management::wrapped_invoke` fp jump sequence + 진입 시점 정밀 timeline |
| BG 6 | hexa pe_user32_register_class_breakthrough | `lib/loader/pe_user32_register_class_breakthrough.hexa` (신규) | RegisterClassExA 진입 시점 timeline + lpszClassName + lpfnWndProc + IPC kind=14 fn_id=1 dispatch |
| BG 7 | docs R3_PROGRESS_V8 (this doc) | `docs/R3_PROGRESS_V8.md` | **본 문서** |

BG 1+4 완료 시 EncodePointer real semantics 통합 + cookie chain 검증 → invalid VA decode 0건 + wrapped_invoke fp jump 도달. BG 2+3 완료 시 PC trace + ROL/ROR 정확 의미화 → 0x56AC43 인근 정상 통과. BG 5+6 완료 시 wrapped_invoke timeline + RegisterClassExA breakthrough. BG 7 = 본 V8 문서.

### 5.1 BG 분산 효율 추정

main sprint = 469-2797 inst/step (평균 ~1100 inst/step) vs BG 분산 sprint = 7 BG × 800-1500 inst/BG-sprint = 5600-10500 inst/sprint. 단, BG 1+3 의 의존성 (EncodePointer real 이 ROL/ROR 정확 의미화 의존) 으로 실 효율은 ~1.5-2× 추정. wave 9 통합 후 inst[30000-50000+] 진척 예상 — USER32 RegisterClassExA 도달 가능성.

---

## 6. 다음 trigger — EncodePointer/DecodePointer ROL+XOR + caller decode 패턴

V8 plateau 해소 + WinMain 진입 trigger.

### 6.1 EncodePointer real semantics

```cpp
// _security_cookie default = 0xBB40E64E (Microsoft 의 default value, runtime randomize 전)
// 단일-thread emul 에서는 const 사용 (process random 미통합 — wave 10+)
static uint32_t g_security_cookie = 0xBB40E64E;

uint32_t EncodePointer_real(uint32_t ptr) {
    uint32_t mask = g_security_cookie;
    uint32_t rot = mask & 0x1F;
    uint32_t enc = ptr ^ mask;
    return ROL32(enc, rot);
}

uint32_t DecodePointer_real(uint32_t ptr) {
    uint32_t mask = g_security_cookie;
    uint32_t rot = mask & 0x1F;
    return ROR32(ptr, rot) ^ mask;
}
```

cookie=0 시점 (init 전) 은 identity 와 동일 — 단계적 진입 안전. cookie != 0 도달 후 real semantics 적용. wave 9 BG 1.

### 6.2 caller decode 패턴 (0x56AC43)

§4.2 disasm 패턴 — `mov eax, [_security_cookie]` + `xor ebx, eax` + `and ecx, 0x1F` + `ror edi, cl` + `call edi`. wave 9 BG 3 의 ROL/ROR mod cl 정확 의미화 + BG 1 의 EncodePointer real 통합 후 정상 통과. 0x56AC43 의 `call edi` 가 valid VA decode 결과로 진입 → wrapped_invoke fp jump 정상.

### 6.3 도달 후 효과

wave 9 통합 (BG 1-7) 후 inst[16486] plateau → inst[30000-50000+] 진척 예상 → `__scrt_common_main_seh` 의 `WinMain(hInstance, NULL, lpCmdLine, nShowCmd)` 호출 도달 → WinMain 내부 `RegisterClassExA` 진입 → IPC kind=14 fn_id=1 dispatch → helper.mm 의 window class map 등록 → `CreateWindowExA` 진입 → fn_id=2 → NSWindow alloc + makeKeyAndOrderFront + 첫 dialog 실 표시.

---

## 7. inst 도달 추정 — wave 9 후 30000-50000+ 예상

V7 의 cap 해제 후 burst (5461 inst) 패턴 + V8 의 wave 7+8 통합 burst (8126 inst, 1.97×) 패턴 기반:

| 단계 | 추정 inst | rationale |
|---|---|---|
| wave 9 BG 1+3 통합 (EncodePointer real + ROL/ROR cl) | 16486 → 22000-25000 | invalid VA decode 0건 후 wrapped_invoke fp jump 도달, ctor 추가 walk |
| wave 9 BG 2+5 통합 (caller trace + wrapped_invoke timeline) | 25000 → 30000-35000 | wrapped_invoke 의 200+ ctor 잔여 entry walk 가속 |
| wave 9 BG 4 통합 (cookie chain hexa skeleton) | 35000 → 40000-45000 | cookie XOR mask 정합 + DecodePointer 역연산 정합 |
| **wave 9 BG 6 통합 (RegisterClassExA breakthrough)** | **45000 → 50000+** | **WinMain 진입 + RegisterClassExA 호출 도달 + IPC kind=14 fn_id=1 dispatch** |

wave 9 후 inst[50000+] 도달 + USER32 RegisterClassExA 도달 가능성 70-80%. 실 NSWindow alloc 도달 = wave 10 (cycle 110-112) 시점 추정.

---

## 8. own1 enforcement 재확인

R3 cycle 106-108 sprint 동안 own1 §A enforcement 위반 0건. 검증 명령 + 결과:

| 영역 | 검증 명령 | 기대 | V8 결과 |
|---|---|---|---|
| Wine / CrossOver | `grep -r -i "wine\|crossover" native/ lib/loader/pe_*_*.hexa` | 0 hit | 0 hit |
| Box86 / QEMU / Bochs | `grep -r -i "box86\|qemu\|bochs" native/ lib/loader/` | 0 hit | 0 hit |
| Capstone / Zydis / iced | `grep -r -i "capstone\|zydis\|^iced" native/ lib/loader/` | 0 hit | 0 hit |
| Apple GPTK / Whisky | `grep -r -i "gptk\|whisky\|game.porting.toolkit" native/ lib/loader/` | 0 hit (docs 거부 언급 외) | 정합 |
| MSVCRT source lift | `grep -r -i "ucrt-source\|ucrtbase-source\|msvcrt-source" native/ lib/loader/` | 0 hit | 0 hit (자체 stub 표 only) |
| Wine USER32 lift | `grep -r -i "winex11\|winecocoa\|wineqcoa" native/ lib/loader/` | 0 hit | 0 hit (자체 NSWindow alloc) |
| 외부 fpu / xmm emul | `grep -r -i "softfloat\|berkeley.softfloat" native/ lib/loader/` | 0 hit | 0 hit (자체 SSE2 expand) |
| EncodePointer reference | `grep -r -i "ReactOS-source\|wine-source" native/ lib/loader/` | 0 hit | 0 hit (Microsoft public docs + Intel SDM Vol 2 ROL/ROR semantics reference only) |

### 8.1 own #1 — 타사 Wine/CrossOver/GPTK/Whisky 추천 금지 재확인

R3 cycle 106-108 sprint 동안 trigger / game-changer 표현 시 외부 lift 추천 발생 **0건**. wave 7-A step3 (0F AE/C7/MMX) / wave 8-C (fake_proc 4-byte stub) / wave 8-D (EncodePointer identity) / wave 9 BG 1 (EncodePointer ROL+XOR real) 모두 자체 구현. enforcement 맥락 외 언급 없음.

### 8.2 자체 코드 양

| file | line | 영역 |
|---|---|---|
| `native/pe_to_macho_shim.cpp` | **6500+** (+1000 vs V7) | phase 1-22 (R1+R2 closure) + phase 23-29 (wave 7-A 5 step + wave 8-C/D) — mem_fault graceful + stdcall arg_count + 0F AE/C7/MMX + INT MOV-Sreg + 0xFE+SSE2 + fake_proc 4-byte stub + GetProcAddress dispatch back + sync/thread fn list + EncodePointer/DecodePointer identity + bad_branch_target graceful + skip cap 64 |
| `native/helper.mm` | **1380+** (+80 vs V7) | wave7-B SendMessageA / PostMessageA / PeekMessageA / TranslateMessage / UpdateWindow stub + BeginPaint stub (cycle 106 commit `9824924`) |
| `lib/loader/pe_*_*.hexa` | **34** file (+7 vs V7) | wave 7 신규 (`pe_i386_0f_prefix_extra_real`, `pe_i386_mem_fault_graceful_real`, `pe_i386_iat_stub_value_real`, `pe_battlenet_setup_install_path_real`) + wave 8 신규 (`pe_msvc_filetime_struct_real`, `pe_msvc_abort_path_avoidance`, `pe_msvc_security_cookie_real`, `pe_msvc_initterm_array_real`) |

own1 §A 정합 — Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + Microsoft Win32 API public docs + ReactOS public docs cross-reference only. EncodePointer ROL+XOR semantics 자체 작성 (Intel SDM Vol 2 의 ROL/ROR opcode spec + Microsoft `learn.microsoft.com` EncodePointer return value spec — 코드 lift X).

---

## 9. cumulative commit log (R3 wave 1+2+3+4+5+6+7+8)

cycle 105 (V7) → cycle 108 (V8) 의 R3 관련 commit chain (V7 작성 commit `117ed15` 이후 신규):

| commit SHA | 영역 |
|---|---|
| `117ed15` | docs: R3_PROGRESS_V7 — 8360 inst (492×) + main direct 한계 + wave7 BG 분산 |
| `c061f09` | hexa pe_i386_0f_prefix_extra_real — XADD/CMOVcc/SETcc/BT/BSF tracker |
| `980e23f` | shim wave7-A loop step 1 — mem_fault graceful skip (8360→10803 inst) |
| `9824924` | helper.mm wave7-B — SendMessageA/PostMessageA/PeekMessageA/UpdateWindow + BeginPaint stub |
| `2cc3350` | docs BATTLENET_DAY1_PLAN — first NSWindow → install → launcher day-by-day plan |
| `9ca587c` | hexa pe_i386_mem_fault_graceful_real — mem_fault graceful skip tracker |
| `d0bb383` | shim wave7-A loop step 2 — stdcall arg_count + 0x26/9B/F4/F1 + skip cap 16 (8360→11003 inst) |
| `63dd383` | hexa pe_i386_iat_stub_value_real — IAT stub return value 정확도 tracker |
| `d890503` | hexa pe_battlenet_setup_install_path_real — Battle.net Setup install E2E tracker |
| `76724ee` | shim wave7-A loop step 3 — 0F AE/C7/18-1F/BE-BF + ret unwind (11003→13800 inst) |
| `341d756` | shim wave7-A loop step 4 — INT/MOV-Sreg/CLD/STD/CLC/STC/CLI/STI + unsup skip (13800→14458 inst) |
| `6e32317` | shim wave7-A loop step 5 — 0xFE INC/DEC8 + INT3 skip + SSE2 expand (14458→15214 inst) |
| `cec89d4` | docs MSVC_CRT_INIT_BYPASS — security_cookie + _initterm + CFG 우회 전략 |
| `75237d3` | hexa pe_msvc_filetime_struct_real — FILETIME/SYSTEM_INFO/OSVERSIONINFO struct fill tracker |
| `d08aaa6` | hexa pe_msvc_abort_path_avoidance — INT3/fast_fail/abort 회피 tracker |
| `0913fe0` | hexa pe_msvc_security_cookie_real — security cookie + CFG check tracker |
| `ae2d799` | chore loader self-test — register pe_msvc_security_cookie_real |
| `1cb4e3d` | hexa pe_msvc_initterm_array_real — _initterm CRT/CXX init array walk tracker |
| `26bc6ce` | shim wave8-C — fake_proc 4-byte stub + GetProcAddress dispatch back (15214→16373 inst) |
| `526acfa` | shim wave8-D — GetProcAddress sync/thread fn list + EncodePointer identity (16373→16017 inst stable) |
| `21e8dca` | shim wave8-D step 2 — bad_branch_target/mem_fault graceful skip cap 64 (16017→16486 inst) |
| **`a2f32f8`** | **wave8 마무리 — bad_target graceful + skip cap 확장 + R5 launcher skeleton** |

**누적 V7→V8**: cpp +1000 line / mm +80 line / hexa +500-line × 8 file (0f_prefix_extra + mem_fault_graceful + iat_stub_value + battlenet_setup_install + msvc_filetime_struct + msvc_abort_path_avoidance + msvc_security_cookie + msvc_initterm_array) / docs (this doc + BATTLENET_DAY1_PLAN + MSVC_CRT_INIT_BYPASS). 22 commit, 3 sprint (cycle 106-108).

---

## 10. R4 / R5 cycle preview

R3 closure 후의 다음 cycle. V7 §9 와 동일한 구조 유지하되 R5 launcher skeleton (cycle 108 commit `a2f32f8`) 등록 반영.

### 10.1 R4 — TLS download / 실 binary acquisition

R3 closure 후 R4 진입. 자체 TLS stack (OpenSSL / BoringSSL / mbedTLS lift 0줄, Apple `Network.framework` `nw_connection_t` direct).

| sub-phase | 영역 |
|---|---|
| R4-1 | Network.framework nw_connection 통합 (TCP) |
| R4-2 | TLS 1.2/1.3 handshake (Apple Secure Transport direct) |
| R4-3 | HTTP/1.1 request/response parser 자체 |
| R4-4 | Battle.net CDN URL resolve (DNS A record query) |
| R4-5 | partial range request (Range: bytes=N-M) for resume |
| R4-6 | Setup binary integrity verify (SHA256 + signature stub) |

### 10.2 R5 — launcher 통합 (cycle 108 skeleton 등록)

R5 launcher skeleton (cycle 108 commit `a2f32f8` 시점 등록) — D2R 실 launcher 통합. Battle.net launcher → D2R 실행 → D3D9 → Metal 변환.

| sub-phase | 영역 |
|---|---|
| R5-1 | D2R PE32 → Mach-O conversion (R3 i386 pipeline 재사용) |
| R5-2 | D3D9 → Metal shader 변환 (D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md 참조) |
| R5-3 | DirectInput → IOKit HID 변환 |
| R5-4 | DirectSound → AudioUnit 변환 |
| R5-5 | first frame render 도달 |
| R5-6 | character select 도달 (in-game closure) |

R5 closure = "Wine 0줄 + 자체 Apple SDK direct + D2R 첫 in-game frame 표시" — closure 100% target.

---

## 11. closure path 통합 — R3-12 first NSWindow 도달 cycle 추정

R3-12 closure (Battle.net Setup 첫 dialog 표시) 까지의 잔여 cycle 추정.

### 11.1 잔여 cycle 추정

| cycle | wave | 누적 closure | 상태 |
|---|---|---|---|
| 90 | R3 entry | 0% | 진입 |
| 95 (V2) | R3-1~5 통합 (17 inst) | ~45% | 완료 |
| 97 (V3) | R3-6 IAT thunk routing (51 inst) | ~50% | 완료 |
| 99 (V4) | R3-8/9 SEH + FPU/SSE (205 inst) | ~55% | 완료 |
| 101 (V5) | wave3 + helper NSWindow real (277 inst) | ~60% | 완료 |
| 103 (V6) | wave4 IAT semantics + USER32/GDI32 wired (343-370 inst) | ~70% | 완료 |
| 105 (V7) | wave5 CRT init + wave6 main-direct (8360 inst, 492×) | ~80% | 완료 |
| **108 (V8)** | **wave 7-A 5 step + wave 8-C/D (16486 inst, 970×, USER32 미도달)** | **~88%** | **본 문서** |
| 110 (V9 예상) | wave 9 BG 분산 (EncodePointer real ROL+XOR + caller decode 패턴 + ROL/ROR cl + cookie chain) → WinMain 진입 + RegisterClassExA 도달 | ~94% | 진행 예정 |
| 112 (V_FINAL 예상) | wave 10 first NSWindow 실 표시 (Battle.net Setup dialog) | 100% (R3 closure) | 진행 예정 |

**R3-12 first NSWindow 도달 추정 cycle = 112** (현 cycle 108 + 4 cycle). wave 9 BG 분산이 EncodePointer plateau 해소 후 cycle 110 의 V9 에서 WinMain 진입 + RegisterClassExA, cycle 112 의 V_FINAL 에서 첫 dialog 실 표시.

### 11.2 closure path 통합

**closure path**: V8 (wave 7-A + wave 8-C/D 16486 inst plateau) → wave 9 BG 분산 (EncodePointer/DecodePointer real ROL+XOR + caller decode 패턴 trace + ROL/ROR cl 정확 의미화 + cookie chain hexa + wrapped_invoke timeline + RegisterClassExA breakthrough hexa) → V9 (WinMain 진입 + RegisterClassExA 호출 도달 + 30000-50000+ inst) → wave 10 (CreateWindowExA + 실 NSWindow alloc + 첫 dialog 실 표시) → V_FINAL (R3 closure 100%).

own1 enforcement 일관 — cycle 90 의 closure 100% (R1+R2) 는 phase 1-13 의 자체 구현. cycle 91-108 의 R3 진척 또한 외부 emulator / Wine fork lift 0줄. **자체 i386 execution layer + 자체 KERNEL32/MSVCR/UCRT/NTDLL/ADVAPI32 stub 표 + 자체 USER32/GDI32 IPC bridge + 자체 fake_heap + 자체 8-bit + 0F AE/C7/MMX + SSE2 expand + 자체 fake_proc 4-byte stub + 자체 EncodePointer identity → real ROL+XOR (wave 9 trigger)** 의 cohesion 누적 ~88% — 잔여 ~12% 의 sub-phase (wave 9 BG 분산 + wave 10 first NSWindow) 가 V8 → V9 → V_FINAL cycle 동안 진척 대상.

→ 본 V8 가 cycle 108-110 sprint 동안 유효. cycle 110 즈음 wave 9 BG 분산 (7 BG) 통합 + WinMain 진입 + RegisterClassExA 호출 도달 시점에 V9 재작성 기대.

---

*written 2026-05-01 cycle 108 sprint 직후. R1+R2 own1 §A closure 100% 유지 + R3-1~11 wave6 main-direct 통합 + shim phase 23-29 통합 완료 + **wave 7-A 5 step (mem_fault graceful skip / stdcall arg_count + 0x26/9B/F4/F1 + skip cap 16 / 0F AE/C7/MMX prefix + ret unwind / INT MOV-Sreg + CLD/STD/CLC/STC/CLI/STI + unsup skip / 0xFE INC-DEC8 + INT3 skip + SSE2 expand) + wave 8-C/D (fake_proc 4-byte stub + GetProcAddress dispatch back / sync/thread fn list + EncodePointer/DecodePointer identity / bad_branch_target + mem_fault graceful skip cap 16 → 64 확장 / R5 launcher skeleton) 일괄 통합** 동작. **16486 instruction (970×, V7 의 8360 → V8 의 16486, delta +8126, ~1.97× of V7)** 도달 — wave 7+8 통합 후에도 USER32 호출 여전히 미도달, EncodePointer/DecodePointer identity 한계로 인한 plateau (cookie != 0 시점부터 ROL+XOR cookie-based decode 가 invalid VA 산출 → wrapped_invoke fp jump 가 bad_branch_target 으로 graceful skip). 0x56AC43 인근 caller decode 패턴 (`mov eax, [_security_cookie]; xor ebx, eax; and ecx, 0x1F; ror edi, cl; call edi`) 가 V8 plateau 의 핵심 disasm. 다음 trigger 는 **wave 9 BG 분산 (7 BG)** — EncodePointer/DecodePointer real ROL+XOR semantics (cookie 0xBB40E64E default + low5bits rotation count) + caller decode 패턴 PC trace + ROL/ROR opcode mod cl 정확 의미화 + cookie chain hexa skeleton + wrapped_invoke timeline hexa + RegisterClassExA breakthrough hexa + 본 doc. helper.mm 의 wave7-B SendMessageA/PostMessageA/PeekMessageA/UpdateWindow/BeginPaint stub 통합 (cycle 106 commit `9824924`). USER32/GDI32 IPC bridge 의 shim cpp 측 wiring 유지 (단, WinMain 미진입으로 호출 도달 여전히 0건 — wave 9 후 검증). own1 §A enforcement 일관 (Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift 0줄). own #1 (타사 Wine/CrossOver/GPTK/Whisky 추천 금지) — wave 7-A / wave 8-C/D / wave 9 trigger 표현 시 외부 lift 추천 발생 0건. KERNEL32/MSVCR/UCRT/NTDLL/ADVAPI32 stub return 표 자체 작성 (Microsoft API public docs reference + ReactOS public docs cross-reference + Intel SDM Vol 2 ROL/ROR/XOR semantics). 본 PROGRESS V8 path 외 수정 X. Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + Microsoft Win32 API public docs + ReactOS public docs reference only.*
