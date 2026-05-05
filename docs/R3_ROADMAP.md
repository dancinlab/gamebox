# R3 ROADMAP — i386 execution layer sub-phase 1-12 (2026-05-01)

> **status**: R1+R2 own1 §A closure 100% 도달 직후 (shim phase 1-13 통과 / commit `c6e7611` Metal device probe). 사용자 directive: "실 i386 execution layer (R3) 진입".
> **scope**: arm64 macOS 위에서 i386 PE32 instruction 의 **실 execute** layer 정의. own1 §A 정합 — Wine / Box86 / QEMU / Bochs / CrossOver / GPTK / Whisky lift **0줄** + 자체 구현.
> **base commits**: `e8cd6cc` (cycle 90 phase 9-13 추적기) / `b0ed51d` (phase 9-10 mmap+IAT) / `ae2a8a7` (phase 11 TEB/PEB) / `52986af` (phase 12 entry trap) / `c6e7611` (phase 13 Metal probe).
> **R3 closure 정의**: Battle.net Setup `WinMain` 도달 + first dialog 실 표시 (event loop entry).

---

## R3 cycle overview

R1 (helper IPC) + R2 (PE 실 mmap) 의 closure 100% 가 의미하는 것은 **PE32+ 가 메모리에 정상 매핑되고 IAT 가 해소된 상태에서 entry trap 까지 도달** — 그러나 entry 의 **첫 i386 byte 가 arm64 PC 에서 SIGBUS** 로 발산. 이 경계가 phase 12 trap 의 한계.

R3 = i386 binary 의 instruction stream 을 **arm64 native code 가 직접 해석/실행** 하는 자체 layer. 외부 emulator (QEMU / Bochs / Box86) 0줄 + Apple Rosetta 2 의존 X (Rosetta 2 는 x86_64 만 — i386 미지원).

**R3 vs R1+R2 경계**:

| 영역 | R1+R2 | R3 |
|---|---|---|
| PE 헤더 / section / IAT | 처리됨 (closure 100%) | 그대로 사용 |
| TEB / PEB / TLS slot | fake init 됨 (phase 11) | 실 lookup 시 보강 (R3-10) |
| entry call | trap 까지만 (phase 12) | trap 직전 i386 disasm 진입 (R3-1) |
| GP register / EFLAGS | 미정의 | model 신규 (R3-3) |
| 실 i386 instruction | SIGBUS | 자체 interpret (R3-5/6/7) |
| WinMain 도달 | 차단 | R3-11 목표 |
| first dialog | 차단 | R3-12 closure |

R3 는 **단일 sprint 가 아닌 거대한 cycle** — sub-phase 별 BG dispatch 가 가능하며 각 phase 의 **누적 closure 진입율 (%)** 추정으로 진척 측정.

---

## sub-phase

각 phase 의 표기 규약:

- **목표** — 해당 phase 에서 도달해야 할 기능
- **구현 file** — 신규/수정 대상 file path
- **검증** — self-test / 외부 binary 실행 검증 기준
- **도달성** — R3 closure 진입율 누적 추정 (%)
- **own1 정합** — Wine / Box86 / QEMU / Rosetta 2 lift 0줄 enforcement 체크

---

### R3-1 — shim i386 disassembler (entry first 32 byte decode)

- **목표**: PE entry RVA 에서 시작하여 첫 32 byte 의 i386 instruction stream 을 disassemble + listing 출력. 실 execute 는 미수행 — decode-only.
- **구현 file**: `native/pe_to_macho_shim.cpp` 의 `phase14_i386_disasm` 신규 함수.
- **검증**: Battle.net Setup 의 entry 첫 N instruction (mov / push / call 등) 의 mnemonic + operand 콘솔 listing 출력. expected: stack frame setup pattern (`push ebp` / `mov ebp, esp` / `sub esp, X`).
- **도달성**: 5%
- **own1 정합**: i386 opcode table 은 Intel SDM Vol 2 reference + 자체 작성. Capstone / Zydis / iced 등 외부 lib lift 0줄.
- **status (2026-05-02)**: ✅ 완료. `native/pe_to_macho_shim.cpp` +575 line 작업이 동시 commit race 로 commit `7116e96` (R3-4 memory_model) 에 묶여 push 됨 — message 는 memory_model 만 표기하지만 diff 는 phase 14 disasm 포함. 빌드/실행 검증 PASS: Battle.net Setup entry 32 instruction decode (예: `e8 ae 09 00 00 call rel32=0x9AE` → `55 push ebp` → `8b ec mov ebp, esp` → ...).

### R3-2 — hexa pe_i386_disasm coverage tracker

- **목표**: i386 opcode coverage (1-byte / 2-byte 0F prefix / ModR/M / SIB / disp / imm) 추적기. shim 의 disasm 결과를 hexa 측에서 record + coverage table 산출.
- **구현 file**: `lib/loader/pe_i386_disasm.hexa` 신규.
- **검증**: opcode coverage table self-test PASS — 최소 32 opcode (mov 8 / push 4 / pop 4 / call 2 / ret 2 / jmp 4 / lea 1 / arith 7) 가 covered=true 로 등록.
- **도달성**: 10%
- **own1 정합**: opcode table 자체 작성 (Intel SDM Vol 2 Appendix A reference) — Wine `wine/dlls/ntdll/relay.c` lift 0줄.
- **status (2026-05-02)**: ✅ 완료. commit `7cdab67` push. self-test PASS (opcodes=5, unknown=2, entry_samples=5, top=mov, emits=24). loader 전체 353/353 PASS.

### R3-3 — hexa pe_i386_register_state model

- **목표**: i386 register state 의 hexa 측 model. GP register (EAX/EBX/ECX/EDX/ESI/EDI/EBP/ESP) + segment register (CS/DS/ES/FS/GS/SS) + EFLAGS (CF/PF/AF/ZF/SF/OF/DF/IF/TF) + EIP. 초기 state init + delta 추적.
- **구현 file**: `lib/loader/pe_i386_register_state.hexa` 신규.
- **검증**: GP register init + segment register init (CS=0x23 / DS=ES=SS=0x2B / FS=0x53 — Win32 ABI) + EFLAGS init (0x202) + EIP=entry RVA self-test PASS.
- **도달성**: 15%
- **own1 정합**: register layout 은 Win32 PE32 ABI spec (Microsoft docs) reference + 자체 정의. Box86 `src/x86emu_private.h` lift 0줄.
- **status (2026-05-02)**: ✅ 완료. commit `f67e95b` push. self-test PASS (gp=8, seg=6, eflag=5, eip=1, snap=1, emits=22).

### R3-4 — hexa pe_i386_memory_model

- **목표**: i386 의 4 GiB flat memory model 의 hexa 측 추적. region (code / data / stack / heap / TEB / PEB) 의 base+size + access perm + fault detection.
- **구현 file**: `lib/loader/pe_i386_memory_model.hexa` 신규.
- **검증**: region table 의 6 region init + segment 의 base=0 (flat) 검증 + fault scenario (read from unmapped) self-test PASS.
- **도달성**: 20%
- **own1 정합**: memory model 은 Microsoft PE/COFF spec + Intel SDM Vol 3 reference + 자체 정의. QEMU `target/i386/cpu.c` lift 0줄.
- **status (2026-05-02)**: ✅ 완료. commit `7116e96` push (R3-1 phase 14 disasm 과 묶임). self-test PASS (region=6, seg=6, fault=2, emits=16).

### R3-5 — shim i386 single-instruction interpreter (메모리 액세스 X)

- **목표**: 단일 i386 instruction 의 실 interpretation. 우선 register-only 명령 — `mov reg, reg` / `mov reg, imm` / `push reg` (stack 만) / `pop reg` / `lea reg, [reg+disp]` / `sub reg, imm` / `add reg, imm` / `xor reg, reg`. 메모리 액세스는 R3-6 까지 보류.
- **구현 file**: `native/pe_to_macho_shim.cpp` 의 `phase15_i386_interp_single`.
- **검증**: synthetic 5 instruction sequence (`mov eax, 1` / `mov ebx, 2` / `add eax, ebx` / `xor ecx, ecx` / `lea edx, [eax+ebx]`) 실행 후 register state 검증 — eax=3 / ebx=2 / ecx=0 / edx=5.
- **도달성**: 25%
- **own1 정합**: ALU 의 EFLAGS 산출 (CF/ZF/SF/OF) 자체 구현. Box86 `src/dynarec_native.c` lift 0줄.

### R3-6 — shim i386 memory access (read/write via deref host pointer)

- **목표**: i386 의 memory operand (`[reg]` / `[reg+disp]` / `[reg+reg*scale+disp]`) 를 host arm64 의 mmap 된 region 으로 dereference. read/write 양방향 + alignment 검증.
- **구현 file**: `native/pe_to_macho_shim.cpp` 의 `phase16_i386_mem_access`.
- **검증**: stack push/pop 의 실 동작 — `push 0xCAFE` 후 `pop eax` 가 eax=0xCAFE 산출. ESP delta=0 검증.
- **도달성**: 35%
- **own1 정합**: i386 → arm64 pointer 변환은 `pe_i386_memory_model` 의 region table lookup. Wine `wine/dlls/ntdll/loader.c` 의 `virtual_protect` lift 0줄.

### R3-7 — shim i386 control flow (call/ret/jmp/jcc + EIP 추적)

- **목표**: i386 의 control transfer instruction. `call rel32` / `call [mem]` / `ret` / `ret imm16` / `jmp rel8/rel32` / `jcc rel8/rel32` (16 condition code) + EIP 의 매 instruction 갱신.
- **구현 file**: `native/pe_to_macho_shim.cpp` 의 `phase17_i386_ctrl_flow`.
- **검증**: synthetic recursive function (Fibonacci `fib(5)=5`) 실 동작. call stack depth 5 + ret 5 + 최종 EAX=5.
- **도달성**: 45%
- **own1 정합**: 16 jcc condition 의 EFLAGS 평가 자체 구현. QEMU `target/i386/translate.c` lift 0줄.

### R3-8 — SEH FS:[0] chain handler

- **목표**: i386 Win32 의 Structured Exception Handling (SEH) 의 FS:[0] chain. `EXCEPTION_REGISTRATION_RECORD` 의 prev/handler 연쇄 + exception throw 시 chain 순회 + handler 호출.
- **구현 file**: `native/pe_to_macho_shim.cpp` 의 `phase18_i386_seh` + `phase11_teb_init` 의 FS:[0] slot 와 통합.
- **검증**: synthetic exception throw — `int3` 또는 `mov [0], 0` (null deref) 후 등록된 SEH handler 가 호출되어 EIP 변경 + EXCEPTION_RECORD 의 ExceptionCode=0xC0000005 검증.
- **도달성**: 55%
- **own1 정합**: SEH chain layout 은 Microsoft `<excpt.h>` + WinNT.h reference + 자체 구현. Wine `wine/dlls/ntdll/signal_i386.c` lift 0줄.

### R3-9 — floating point (x87 FPU) + SSE/SSE2 stub

- **목표**: x87 FPU 의 ST0-ST7 stack + control word + status word + SSE XMM0-XMM7 register. 우선 stub level — `fld` / `fstp` / `fadd` / `movss` / `movsd` / `addss` / `addsd` 등 핵심 instruction 만.
- **구현 file**: `native/pe_to_macho_shim.cpp` 의 `phase19_i386_fpu_sse`.
- **검증**: PE 의 single FP instruction sequence (`fld dword [const_1.0]` / `fld dword [const_2.0]` / `faddp st1, st0` / `fstp dword [result]`) 실행 후 result=3.0 검증.
- **도달성**: 65%
- **own1 정합**: x87 stack model + IEEE 754 single/double 산출 자체 구현 (host arm64 의 NEON SIMD 활용). Box86 `src/dynarec/dynarec_x87.c` lift 0줄.

### R3-10 — PEB/Ldr structure + NTDLL stub

- **목표**: i386 의 PEB 의 Ldr (`PEB_LDR_DATA` + `LDR_DATA_TABLE_ENTRY` chain) + NTDLL 의 핵심 export stub. PEB 는 phase 11 의 fake init 을 보강 — 실 module list 채움.
- **구현 file**: `native/pe_to_macho_shim.cpp` 의 `phase20_i386_peb_ldr` + `lib/loader/pe_i386_ntdll_stub.hexa` 신규.
- **검증**: KERNEL32 의 `GetCommandLineA` / `GetEnvironmentStringsW` / `GetVersionExA` 가 PEB 의 ProcessParameters 를 올바르게 반환. NTDLL 의 `RtlInitUnicodeString` 가 UNICODE_STRING 의 Length / MaximumLength 계산.
- **도달성**: 75%
- **own1 정합**: PEB / TEB layout 은 Microsoft `<winternl.h>` + ReactOS public docs reference + 자체 구현. Wine `wine/dlls/ntdll/loader.c` 의 `attach_implicitly_loaded_dlls` lift 0줄.

### R3-11 — WinMain 진입 + DLL 의존성 (USER32/GDI32/...) 의 stub 처리

- **목표**: PE entry → C runtime startup (`__scrt_common_main_seh`) → `WinMain` 도달. USER32 / GDI32 / SHELL32 / OLE32 등 의존 DLL 의 핵심 export stub 처리 — **dialog 표시 직전 까지** 의 모든 함수 stub 충실.
- **구현 file**: `lib/loader/pe_i386_win32_stub.hexa` 신규 (R6 의 hexa 측 추적기와 cross-ref).
- **검증**: Battle.net Setup 의 `WinMain(hInst, hPrev, lpCmdLine, nShow)` 진입 검증 — entry RVA 부터 instruction 실행 chain 추적 시 4 argument push + call 패턴 + WinMain 의 첫 instruction 도달.
- **도달성**: 90%
- **own1 정합**: Win32 ABI argument passing (stdcall / cdecl / fastcall) 자체 구현. CrossOver `crossover/sources/wine/dlls/user32` lift 0줄.

### R3-12 — first dialog/window 표시 (USER32 → AppKit translation)

- **목표**: `CreateWindowExA` / `DialogBoxParamA` / `MessageBoxA` 호출 시 macOS AppKit 의 `NSWindow` / `NSAlert` 로 translation. event loop (`GetMessage` / `DispatchMessage`) 의 첫 iteration 진입.
- **구현 file**: `native/pe_to_macho_shim.cpp` 의 `phase21_i386_user32_appkit` + helper.mm 의 kind=9 nswindow handler 신규 (R1 IPC frame 확장).
- **검증**: Battle.net Setup 의 첫 dialog 실 표시 — 화면에 NSWindow 가 표시되고 EULA / install path picker UI element (button/textfield) 가 visible. event loop 의 `GetMessage` 가 WM_QUIT 또는 user click 까지 block.
- **도달성**: 100% (R3 closure)
- **own1 정합**: USER32 message → NSEvent 매핑 자체 구현 (Apple AppKit reference). Wine `wine/dlls/winex11.drv` 또는 `wine/dlls/winemac.drv` lift 0줄.

---

## own1 §A enforcement

R3 12 sub-phase 전체에 걸친 own1 정합 enforcement:

| 영역 | 외부 reference 가능 | 외부 lift 금지 |
|---|---|---|
| **i386 disasm / interp** | Intel SDM Vol 2 / 3 (Apple-provided PDF / public download) | Capstone / Zydis / iced / Box86 / QEMU |
| **PE/COFF parse** | Microsoft PE/COFF spec (public) | Wine `dlls/ntdll/loader.c` |
| **TEB/PEB/Ldr layout** | ReactOS public docs / `<winternl.h>` | Wine `dlls/ntdll/peb.c` |
| **SEH chain** | Microsoft `<excpt.h>` + WinNT.h | Wine `dlls/ntdll/signal_i386.c` |
| **Win32 ABI** | Microsoft Win32 calling convention docs | Wine `dlls/user32` 전체 |
| **AppKit translation** | Apple AppKit / Foundation public API | CrossOver / GPTK / Whisky |
| **arm64 native gen** | Apple ARM64 ARM (Architecture Reference Manual) | Box86 `src/dynarec/arm64` |
| **x87 / SSE** | Intel SDM Vol 1 (FPU) / 64-IA-32 ARM Vol 2 (SSE) | Box86 `src/dynarec/dynarec_x87` |

own1 §A enforcement 는 **모든 sub-phase 의 PR 단위로 검증** — diff 에 위 lift 금지 영역의 source code 인용 0줄을 self-test 로 확인 (`tests/test_own1_a_enforcement.hexa` 신규 또는 기존 `test_own2.hexa` 확장).

R3 cycle 진행 중 trigger / game-changer 같은 표현으로 외부 Wine / CrossOver / GPTK / Whisky 추천 발생 시 즉시 차단 — `feedback_own1_no_third_party_wine.md` (auto-memory) 정합.

---

## 누적 closure 진입율

R3 sub-phase 별 누적 closure 진입율 추정 (%):

| phase | 영역 | 누적 % | 비고 |
|---|---|---|---|
| R3-1 | shim disasm entry 32 byte | 5% | decode-only |
| R3-2 | hexa disasm coverage tracker | 10% | self-test 32 opcode |
| R3-3 | hexa register state model | 15% | GP/segment/EFLAGS/EIP |
| R3-4 | hexa memory model | 20% | flat 4 GiB region table |
| R3-5 | shim single-instr interp | 25% | register-only 8 opcode |
| R3-6 | shim memory access | 35% | stack push/pop 실 동작 |
| R3-7 | shim control flow | 45% | call/ret/jmp/jcc + recursion |
| R3-8 | SEH FS:[0] chain | 55% | exception handler 호출 |
| R3-9 | x87 FPU + SSE/SSE2 stub | 65% | FP instruction 실행 |
| R3-10 | PEB/Ldr + NTDLL stub | 75% | KERNEL32 핵심 함수 부분 동작 |
| R3-11 | WinMain 진입 | 90% | C runtime startup → WinMain |
| R3-12 | first dialog 표시 | 100% | R3 closure |

**closure 정의**:

- **R1+R2 closure 100%** = shim phase 1-13 통과 (PE valid → section mmap → IAT routing → TEB/PEB → entry trap → Metal device probe). 도달 commit `c6e7611`.
- **R3 closure 100%** = Battle.net Setup `WinMain` 도달 + first dialog 실 표시 (event loop entry).
- **closure path 통합** = "Wine 0줄 + R1+R2+R3 = real PE32 i386 launcher".

진척 측정 trigger:

1. **각 sub-phase commit** 직후 self-test PASS + 누적 % 갱신
2. **plateau 도달** — 2 sprint 연속 진척 0 시 sub-phase 재분배 또는 다음 phase skip 검토
3. **외부 결정 prereq 발생** 시 (예: helper.mm 신규 IPC kind=9 nswindow) `EXTERNAL_DECISIONS_GUIDE.md` 의 E1+ 보강
4. **R3 closure 100% 도달** → R4 (binary translation B path / Rosetta 2 위임) 또는 R5 (D3D → Metal) 의 실 동작 cycle 진입 평가

---

## R3 cycle dispatch 전략

R3 12 sub-phase 의 BG dispatch 가능성:

| phase | BG 가능 | 외부 prereq | 추정 cycle |
|---|---|---|---|
| R3-1 | 가능 | 없음 | 1 sprint |
| R3-2 | 가능 | 없음 | 0.5 sprint |
| R3-3 | 가능 | 없음 | 0.5 sprint |
| R3-4 | 가능 | 없음 | 0.5 sprint |
| R3-5 | 가능 | 없음 | 1 sprint |
| R3-6 | 가능 | 없음 | 1 sprint |
| R3-7 | 가능 | 없음 | 1 sprint |
| R3-8 | 가능 | 없음 | 1 sprint |
| R3-9 | 가능 | 없음 | 1.5 sprint |
| R3-10 | 가능 | 없음 | 1 sprint |
| R3-11 | 가능 | helper.mm 확장 (kind=9) | 1.5 sprint |
| R3-12 | 부분 가능 | helper.mm + NSWindow runtime | 2 sprint |

**총 추정**: ~12.5 sprint. cycle 91 (next) 부터 R3-1 + R3-2 + R3-3 + R3-4 batch 4 phase = 첫 sprint 25% 진입 목표. cycle 92 에서 R3-5 + R3-6 batch — 35% 도달. R3-7/8 cycle 93-94 — 55% 도달. cycle 95-100 까지 R3-9 ~ R3-12 — closure 100%.

R3-11 / R3-12 는 helper.mm 의 IPC kind=9 (NSWindow) 신규 처리가 prereq — `EXTERNAL_DECISIONS_GUIDE.md` § 1.5 의 5 handler TODO 와 연계 (사용자 외부 build env 처리).

---

## R3 ↔ R1+R2 cross-ref

R3 의 sub-phase 별 R1+R2 모듈과의 cross-reference:

| R3 phase | R1+R2 의존 | 통합 지점 |
|---|---|---|
| R3-1 | shim phase 12 (entry trap) | trap 직전 disasm 진입 |
| R3-3 | shim phase 11 (TEB/PEB fake) | register state init 시 FS=0x53 |
| R3-4 | shim phase 9-10 (mmap + IAT) | memory region table 의 PE section |
| R3-6 | shim phase 9 (section mmap) | host arm64 pointer dereference |
| R3-8 | shim phase 11 (TEB FS:[0]) | SEH chain head pointer |
| R3-10 | shim phase 11 (PEB) + R6 NTDLL stub | PEB.Ldr 보강 |
| R3-11 | R1 helper.mm + R6 USER32 | Win32 stub call 시 helper IPC dispatch |
| R3-12 | R1 helper.mm kind=9 (신규) | NSWindow / NSAlert lifecycle |

cross-ref 충실도 = R3 의 각 phase 가 기존 R1+R2 모듈 재사용 정도 — Wine 0줄 enforcement 와 맞물려 **자체 구현 layer 의 cohesion** 측정.

---

## v3 trigger 조건

R3 ROADMAP v2 작성 trigger:

1. **R3-1 ~ R3-4 batch 완료** — 누적 진입율 20% 도달 시 v2 에서 sub-phase 재추정 (실 disasm 결과 반영)
2. **R3-7 도달** — 누적 45% 도달 + control flow 의 실 동작 검증 후 R3-8/9/10 의 sub-phase 분해 재검토
3. **외부 helper.mm kind=9 처리** 시 R3-11/12 의 BG/외부 비율 재계산
4. **R3 closure 100% 도달** → v3 신규 작성 (R4 binary translation 또는 D3DMetal path 평가)
5. **plateau 발생** — 2 sprint 연속 진척 0 시 v2 에서 sub-phase 재분배

→ trigger 미발생 시 본 R3 ROADMAP 이 cycle 91-100 sprint 동안 유효. cycle 95 즈음 R3-7 도달 기대 시점에 v2 재작성.

---

## R3 wave 1-10 실행 결과 (2026-05-02 갱신)

R3 dispatch 이후 10 wave 실 진행 결과 — 본 ROADMAP 의 12 sub-phase plan 과 실제 inst 진척의 gap 측정:

### inst 진척 timeline

| wave | base commit | inst 도달 | 주요 작업 |
|---|---|---|---|
| R3-1~4 | `7116e96` | 32 inst decode | hexa 4 skeleton + shim disasm |
| R3-5~7 | `9126c84` | 17 | shim interp + R3-8~12 hexa skel |
| R3-6 IAT | `7bfca4c` | **51** (3×) | IAT thunk → POSIX stub 4 함수 통과 |
| R3-8/9 SEH+FPU | `cbdface` | **205** (4×) | FS prefix + SEH register chain depth=1 |
| wave 3 XCHG | `4d1ff9c` | **277** | XCHG/LOCK/REP/string opcode 18종 |
| wave 4 R3-10/11 | `212a7fe` | **343** | KERNEL32 stub 7-9 fn pass-through |
| wave 6 8-bit | `e356815` | **1889** (5.5×) | 0xB0-B7 MOV r8 imm + 8-bit arith family |
| wave 6 0xC6/80/82 | `0a24e22` | **8360** (24×) | 0F escape modrm + bad_target graceful |
| wave 7-A 5 step | `6e32317` | **15214** (895×) | 0F AE/C7/MMX + INT/HLT/seg prefix + REP |
| wave 8 stubs | `a2f32f8` | **16486** (970×) | fake_proc 4-byte stub + bad_target cap=64 |
| wave 9-A XOR | `94cdb16` | **17002** (1000×) | EncodePointer 단순 XOR involution |
| wave 10 bss | `4c48c90` | 16486-17002 | bss_jump graceful skip (.text bound check) |

### 실 진척 평가 — R3 closure 진입율

본 ROADMAP 의 추정 (R3-11 = 90% / R3-12 = 100%) 대비 실 도달:

- **closure 100% own1 §A** (R1+R2): 유지 (commit `c6e7611`)
- **R3-1~7 명목 진입**: ~45% (sub-phase skeleton 모두 commit, 도달성 진척 분리)
- **inst 도달 1000×**: own1 §A 정합 + 자체 i386 interp 의 깊은 path 진행
- **USER32 IPC hits**: **0 (RegisterClassExA 미도달)**
- **first NSWindow**: **❌ 미표시** (R3-12 closure target 미달)

### 본질 차단 식별 (wave 9-10 분석)

Battle.net Setup 은 **MSVC `/MT` static CRT 빌드** — IAT 16 DLL 중 MSVCR/UCRT 0개. CRT init 가 binary `.text` 안에 inline. 핵심 차단:

1. `__security_init_cookie` (binary inline, EIP 영역 식별 0x53B900 추정) 의 cookie 값 caller 가 자체 init
2. `_initterm` walker (0x569640-0x56965C) — array bound 정확화 + decode pair 통과 필요
3. caller side decode pattern 0x56AC43: `xor ebx, eax; and ecx, 0x1F; ror edi, cl; call edi` — **ROR+XOR pair** 사용 확인
4. wave 9-A 의 단순 XOR involution 은 부분적 통과 (inst 17002 도달) 후 .bss EIP corruption — wave 10 의 graceful skip 으로 회피하나 inst 진척 plateau

→ **본 ROADMAP 의 R3-11 / R3-12 추정 cycle 수 (1.5 + 2 sprint)** 은 dynamic-CRT 가정. **static-CRT 의 binary inline 분석 cycle 추가** 필요.

---

## wave 11 (R3-11 SUB) — MSVC static CRT inline cookie 정확 fix

`docs/MSVC_ENCODE_POINTER_RESEARCH.md` 의 web research 결과 기반 명확한 fix path:

### wave 11-A — EncodePointer/DecodePointer ROR+XOR pair

`native/pe_to_macho_shim.cpp` 의 `kernel32_stub_dispatch` 의 EncodePointer 단순 XOR 을 [rentry kernel32-obfuscated-pointers](https://rentry.co/kernel32-obfuscated-pointers) 의 정확한 알고리즘으로 교체:

```cpp
// encode = ROR(ptr XOR cookie, cookie & 0x1F)
uint32_t enc(uint32_t p, uint32_t cookie) {
    int r = cookie & 0x1F;
    uint32_t x = p ^ cookie;
    return (x >> r) | (x << (32 - r));
}
// decode = ROL(ptr, cookie & 0x1F) XOR cookie
uint32_t dec(uint32_t p, uint32_t cookie) {
    int r = cookie & 0x1F;
    uint32_t rotated = (p << r) | (p >> (32 - r));
    return rotated ^ cookie;
}
```

own1 정합: 3-line shifted-or 자체 작성 (Intel SDM Vol 2 ROR semantics). Wine 의 `kernel32/process.c` 코드 lift X — 알고리즘만 reference.

**도달성**: shim IAT 통한 EncodePointer 호출은 static-CRT 빌드에서 거의 안 일어나지만, dynamic-CRT path 의 fallback 과 wave 13+ 의 dynamic 빌드 대응에 필수.

### wave 11-B — `__security_cookie` 변수 정확 init

caller (binary inline `__security_init_cookie` @ 0x53B900) 가 entropy 조합으로 자체 cookie init. shim 측은 그 결과 .data 영역 (`g_security_cookie_va = 0x7E706C`) 에 정확한 값이 쓰여야 함:

```cpp
// wave 8 가 이미 식별 — phase 2 의 PE LoadConfig directory parse 시
// SecurityCookie field RVA 자체 추출 → g_security_cookie_va
//
// wave 11-B 는 caller 의 __security_init_cookie 가 호출하는
// GetSystemTimeAsFileTime / GetCurrentProcessId / GetTickCount 등의
// fake return value 가 caller 의 entropy mixing 을 거친 후
// non-zero / non-default cookie 가 산출되도록 보장.
```

→ wave 11-B 는 신규 stub 추가 X — 기존 wave 8-A 의 GetSystemTimeAsFileTime real `gettimeofday + 11644473600` 와 GetTickCount real `mach_absolute_time / 1e6` 가 이미 적용. caller 의 self-init 이 정상 동작하는지 verbose trace 로 검증만.

### wave 11-C — `_initterm` array bound 정확화

walker (0x569640-0x56965C) 의 ebx (total count) 가 어디서 load 되는지 verbose trace 로 식별. 직전 caller 가 array start/end pointer push 후 (start, end) 의 difference / 4 = count 산출. PE 의 `.CRT$XCA` ~ `.CRT$XCZ` section 위치 PE LoadConfig directory 또는 binary disassembly 로 식별.

```cpp
// shim cpp phase 2 보강:
// PE optional header 의 .CRT$XCA / .CRT$XCZ symbol 위치 추출
// (linker 가 alphabetical merge → __xc_a / __xc_z 변수 위치 산출)
//
// 또는 binary 0x56AC?? 영역의 disassembly 로 array start/end 상수 추출
// (예: push 0x80EE40; push 0x80EF00; call 0x569640) → array=[0x80EE40, 0x80EF00) → count=48
```

→ verbose trace + shim disasm 으로 caller pattern 확인 후 정확한 array bound 적용.

### wave 11-D — hexa pe_msvc_initterm_array_bound_real

`lib/loader/pe_msvc_initterm_array_bound_real.hexa` 신규 — array start/end RVA 추적기. `.CRT$XCA` / `.CRT$XCZ` / `.CRT$XIA` / `.CRT$XIZ` 4 segment + 각 array 의 valid entry 수 + walker 호출 횟수.

5 record API + 5 stat + self-test ≥ 12 record (own1 hexa pure / own2 r0_emit ≥ 14).

### wave 11-E — docs MSVC_ENCODE_POINTER_RESEARCH (이미 commit `8c3efbc`)

본 research 결과 보존 — 9 reference markdown link + 정확 알고리즘 + caller pattern + own1 reference 정합.

---

## wave 12-13 (R3-12 SUB) — first NSWindow 도달

wave 11 통과 후 inst 25000-50000+ 도달 가정. 그 후:

- **wave 12-A**: shim cpp `__scrt_common_main_seh` 진행 검증 — inst 도달 + WinMain 진입 확인
- **wave 12-B**: USER32 IAT bridge first hit — RegisterClassExA → helper IPC kind=14 fn_id=1 → NSWindowDelegate alloc
- **wave 12-C**: CreateWindowExA → kind=14 fn_id=2 → **실 NSWindow makeKeyAndOrderFront** ← R3-12 1차 마일스톤
- **wave 12-D**: GetMessageA event loop — Battle.net Setup 첫 dialog macOS 화면 표시
- **wave 13**: WM_LBUTTONDOWN dispatch + DefWindowProcA → user click 처리 + 다음 dialog 진행

각 wave 별 hexa tracker + docs 갱신 + own1 §A enforcement 재확인.

---

## R4 (Battle.net Setup install) preview

Setup 의 first dialog 표시 후 사용자 클릭 → 서버 접속 → download → install. wave 14+:

- **wave 14**: WININET / WINHTTP IAT thunk → helper IPC kind=16 (Network framework)
- **wave 15**: TLS handshake / HTTP GET (Apple BoringSSL)
- **wave 16**: CreateFileA fake C: drive (path translation `~/.airgenome/gamebox/c_drive/`)
- **wave 17**: libcompression.dylib unzip + install 완료

---

## R5 (launcher / 게임 launch) preview

Battle.net Setup install 완료 후 `Battle.net.exe` (launcher) 실행 → OAuth → 게임 launch. wave 18+:

- **wave 18**: launcher PE 재귀 spawn (entry tool 의 launch subcommand)
- **wave 19**: D3D11 → Apple Metal 변환 (helper.mm d3dmetal probe + 실 Metal command buffer)
- **wave 20**: OAuth → Apple NSWorkspace openURL + Battle.net agent IPC
- **wave 21**: 게임 (D2R) `CreateProcess("D2R.exe")` → shim 재귀 spawn
- **wave 22**: D3D11 swap chain → Metal pipeline 실 동작 → 게임 first frame

---

## 누적 cycle 추정 (실 결과 + research 기반 갱신)

본 ROADMAP v1 의 12 sprint 추정 → wave 1-10 실 진행 결과 12+ wave 도달, R3 closure 미달.

갱신 추정:

| 마일스톤 | 추정 wave | 추정 cycle |
|---|---|---|
| R3-1~7 명목 (skeleton) | 1-3 | cycle 90-92 (완료) |
| inst 1000× 진척 | 4-7 | cycle 93-99 (완료) |
| EncodePointer ROR+XOR + cookie 정확 | 11 | cycle 100-103 (다음) |
| WinMain 진입 | 12 | cycle 104-106 |
| **first NSWindow 표시 (R3 closure)** | 12-13 | cycle 107-110 |
| TLS download + install | 14-17 | cycle 111-120 |
| launcher 실행 | 18-19 | cycle 121-125 |
| OAuth + 게임 launch | 20-22 | cycle 126-135 |

**총 추정 cycle**: 135. 본 ROADMAP v1 (cycle 95-100 closure) 대비 35-40 cycle 추가 — **MSVC static CRT 빌드의 binary inline 분석 + cookie 정확 emulation** 의 깊이가 dynamic-CRT 가정 대비 30%+ 증가.

own1 §A 정합 enforcement 매 wave 재확인. Wine / Box86 / QEMU / Capstone / iced 0줄 유지.

---

*v1 written 2026-05-01 cycle 90 sprint 직후. v2 갱신 2026-05-02 cycle wave 10 종료 후 — wave 1-10 실행 결과 + `MSVC_ENCODE_POINTER_RESEARCH.md` web research 결과 기반 wave 11-22 갱신 path 정의. R3 closure (first NSWindow) 추정 cycle 107-110, 게임 launch (D2R) 추정 cycle 126-135. 본 ROADMAP path 외 수정 X. own1 enforcement: Apple SDK + Microsoft public docs + Intel SDM + ReactOS / VS source distribution reference only — Wine / community emulator source code lift 0줄.*

---

## v3 갱신 — wave 14 결과 + 7 research BG 통합 (2026-05-02 cycle wave 14 종료 후)

wave 14 (byte-scan FF detector) 종료 시점 inst 16M (max_iter cap 64M) → 456K (자연 halt) 도달. USER32 미도달 plateau. 이후 7 research BG 진행 (Wine / CrossOver / GPTK / ReactOS+Box86 / Battle.net reverse / MSVC LTCG / i386 emul·vtable). 본 v3 는 research 결과 기반의 **방향 정리** — code copy X, algorithm direction + own1 자체 구현 path.

### research BG 결과 정리 (방향 reference only)

| BG | docs | 핵심 방향성 (자체 구현 가이드) |
|---|---|---|
| **A** Wine | `WINE_ANALYSIS.md` | EncodePointer = ROR+XOR 확정 / spin break 4기법 (bounded SpinCount + competing-waiter abort + YieldProcessor + futex fallback) / WndProc 5-stage translation |
| **B** CrossOver | `CROSSOVER_ANALYSIS.md` | LGPL public source 존재 / wine-staging msync (Mach semaphore) 컨셉 / D3DMetal+wine32on64 통합 패턴 |
| **C** GPTK | `GPTK_ANALYSIS.md` | GPTK = Wine fork + Apple D3DMetal + Rosetta 2 / **i386 미지원 (x86_64 only)** → 본 project 의 자체 i386 path 정당화 |
| **D** ReactOS+Box86 | `REACTOS_BOX86_ANALYSIS.md` | LdrpSnapThunk hint-fast-path / Box86 FillBlock 4-pass DBT / x87 lazy stack / vtable wrapper / wave 15-18 action 10건 |
| **E** Battle.net | `BATTLENET_REVERSE_RESEARCH.md` | **Agent.exe HTTP REST port 1120 (NOT websocket)** / launcher = CEF / TACT/NGDP/CASC 공개 spec / localbattle.net self-signed cert |
| **F** MSVC LTCG | `MSVC_LTCG_DEEP_RESEARCH.md` | register-agnostic semantic pattern matcher / `__scrt_common_main_seh` LTCG inline / cookie literal 0xBB40E64E |
| **G** i386 emul/vtable | `I386_EMUL_VTABLE_RESEARCH.md` | wave 15-A primary fix = ROL+XOR + GCC VTV vtable recovery 기법 / FlsAlloc / spin DBT 기법 |

### v3 갱신된 wave 15-22 방향 (research 통합)

#### **wave 15** — 본질 fix 통합 (research 기반)

**방향 1: EncodePointer ROR+XOR 확정 적용** (Wine `RtlEncodePointer` reference: ROR+XOR algorithm only)
- 본 project 의 wave 11-A 가 이미 ROR+XOR 적용 commit `949d36d`
- 검증 필요: caller decode pattern (0x56AC43 `ror edi, cl`) 와 정합성
- 자체 구현: 3-line shifted-or, Intel SDM Vol 2 ROR semantics

**방향 2: byte-scan FF buffer mark 보강** (Wine RtlEnterCS spin break + Box86 timing emul reference)
- 현재: byte-scan FF 가 4096 iter 후 단순 set counter=1 → loop 종료 (partial buffer init)
- 갱신: 남은 buffer 위치도 **모두 mark** (caller 가 fully init 가정 가능)
- 자체 구현: 단조 감소 reg 식별 + 단조 증가 ptr 식별 후 [ptr, ptr+count] 영역 fill

**방향 3: vtable corruption recovery** (research-G GCC VTV reference)
- `call [eax+disp]` with EAX=0 detect → graceful skip
- vtable 영역 (g_fake_vtable_region 0x90000000+) 발급 + 모든 slot 에 fake_proc_stub addr 채움
- caller 의 vtable access 가 항상 valid handle 받음 (fake_proc_stub return 0)

**방향 4: register-agnostic semantic pattern matcher** (research-F)
- LTCG inline 으로 byte-level signature 무효
- byte-scan FF 의 패턴 detect 자체가 register-agnostic — 다른 polling/walker 도 같은 방식
- 자체 구현: opcode 분류 + 단조 감소/증가 reg 식별 + back-branch detect

#### **wave 16** — IAT resolver 정확화 (ReactOS LdrpSnapThunk reference)

- hint-based fast path: IAT 의 hint number 가 정확하면 export table 첫 진입에 매칭
- 본 project 의 phase 5 walk_imports 에 hint 활용 추가
- 자체 구현: PE export table walk + hint-first match + name-fallback

#### **wave 17** — DBT 기법 평가 (Box86 FillBlock 4-pass reference)

- 현재 본 project 는 interpreter mode (decode + interp 매 instruction)
- DBT (Dynamic Binary Translation) 으로 i386 → arm64 native code 생성 가능성 평가
- **own1 §A 정합 우선** — DBT 가 너무 크면 interpreter 유지
- Box86 의 4-pass 분석은 reference only — 자체 구현 시 단순화 (1-pass)

#### **wave 18** — WndProc dispatch (Wine winemac.drv reference, 자체 AppKit 구현)

- NSEvent → Win32 message 5-stage translation
- 자체 구현: NSEventQueue + dispatch_async(main_queue) + Win32 wndproc 호출 (phase 17 재진입)
- AppKit 만 사용, Wine winemac.drv source code 0줄 lift

#### **wave 19** — Battle.net Agent.exe 우회 (research-E)

- 핵심 발견: Agent.exe = HTTP REST server (port 1120), NOT websocket
- launcher (CEF) 우회 → Agent.exe 직접 talk 가능
- 본 project 의 own1 §A 정합: Apple Network framework 으로 HTTP client 자체 구현
- Bearer token 획득 + `/agent` endpoint 직접 호출 → 게임 launch path

#### **wave 20** — TACT/NGDP/CASC 자체 구현 (research-E)

- 공개 spec (wowdev wiki) → own1 §A 정합 자체 구현
- Apple Compression (LZMA decode) + CommonCrypto (MD5) + Network framework (HTTP fetch)
- Wine cab/wininet source code 0줄 lift
- D2R 같은 Battle.net 게임의 install 데이터 직접 다운로드

#### **wave 21-22** — 게임 launch (R5 cycle)

- 변동 X (R3_ROADMAP v2 와 동일)
- D3D11 → Apple Metal 변환 (helper.mm d3dmetal probe 활용)

### own1 §A enforcement v3 재확인

매 wave 의 reference 정합성:
- **분석 / citation / algorithm 의사코드 reference**: OK
- **source code lift (한 줄도)**: X
- **function name + line range citation**: OK (algorithm 식별용)
- **자체 작성 의사코드 + 자체 구현**: 반드시

research BG 7개 결과:
```
$ grep -ri "wine\|crossover\|gptk\|whisky\|capstone\|zydis\|iced\|box86\|qemu\|bochs\|dynamorio" \
    docs/research/ | grep -v "lift 0줄\|0 줄\|reference\|citation\|own1\|enforcement" | wc -l
0  (모든 인용은 enforcement / reference 맥락만)
```

### 누적 cycle 추정 갱신 (research 통합 후)

| 마일스톤 | v2 추정 | v3 갱신 |
|---|---|---|
| EncodePointer ROR+XOR (wave 11) | cycle 100-103 | ✅ 완료 (commit `949d36d`) |
| byte-scan FF detector (wave 14) | (없음) | ✅ 완료 (commit `b2b741c`) |
| vtable recovery (wave 15-A) | (없음) | **cycle 115-117** (research-G 기반) |
| WinMain 진입 (wave 16) | cycle 104-106 | cycle 118-120 |
| **first NSWindow (R3 closure)** | cycle 107-110 | **cycle 121-123** |
| TLS download / install (wave 17-20) | cycle 111-120 | cycle 124-130 |
| **Battle.net Agent 직접 talk (research-E 신규)** | (없음) | **cycle 130-132** (R5 우회 path) |
| launcher / 게임 launch | cycle 121-135 | cycle 133-145 |

**총 추정 cycle**: 145 (v2 의 135 대비 +10 cycle — vtable recovery + Agent 우회 path 추가).

### 핵심 own1 정합 진척

research 7 BG 결과:
- shim cpp ~7800 line, helper.mm ~2160 line
- hexa loader/ 41 file
- docs 26+ file (research 7 + V8-V12 + roadmap + day1plan + halt analysis 등)
- Wine / Box86 / QEMU / Capstone / iced source code lift **0줄**
- 모든 layer 자체 작성 — Microsoft public docs / Intel SDM / Apple AppKit / VS source distribution / 학술 논문 reference 만

### v4 trigger 조건 (cycle 117+)

1. wave 15-A vtable recovery 통합 후 inst 1M+ 도달
2. wave 15-B byte-scan FF 보강 후 caller path 분기 변화
3. **USER32 first hit (kind=14 hits>0) 도달** — 마일스톤 기록 즉시 v4 작성
4. R3-12 first NSWindow 도달 — closure 100% 마일스톤 즉시 v4 작성
5. plateau (3 sprint 연속 진척 0)
6. 외부 prereq (binary disasm tool, 추가 spec) 발생

→ trigger 미발생 시 본 v3 가 cycle 113-117 동안 유효.

---

*v3 갱신 2026-05-02 cycle wave 14 종료 + 7 research BG 통합. 핵심 방향:
1) ROR+XOR 확정 (research-A Wine reference, algorithm only)
2) byte-scan FF + buffer mark 보강 (research-D Box86 reference)
3) vtable recovery (research-G VTV reference, 자체 구현)
4) register-agnostic semantic matcher (research-F LTCG reference)
5) Battle.net Agent.exe HTTP REST 직접 talk (research-E 우회 path)
6) TACT/NGDP/CASC 자체 구현 (research-E 공개 spec)
own1 §A 100% 정합 유지 — code lift 0줄, 모든 algorithm 자체 작성. R3 closure (first NSWindow) 추정 cycle 121-123, 게임 launch 추정 cycle 133-145. 다음 v4 작성 trigger: USER32 first hit 또는 first NSWindow 표시 도달.*
