# R3 PROGRESS V2 — 17 instruction 도달 + opcode coverage 표 + 잔여 trigger (2026-05-01)

> **status**: R1+R2 own1 §A closure **100%** 유지 + R3-1~7 완료 + R3-8~12 hexa skeleton 완료 + shim 통합 phase 14-17 동작. 누적 R3 진척 **~45%**.
> **base commits**: `c6e7611` (R1+R2 closure 100%) → `84c31ec` (R3 ROADMAP) → `7116e96` / `f67e95b` / `7cdab67` / `7dc4b20` / `9126c84` / `2b90d2c` (R3-1~7 + R3-8~12 skeleton + shim phase 14-17).
> **현재 차단**: shim interpreter inst[16] 의 `call [0x6CA3B0]` IAT indirect 호출에서 `bad_branch_target` halt — phase 15_setup_regions 의 PE image mmap 이 phase 9 helper-side mmap 과 별도라서 phase 10 의 dlsym patch 가 unresolved. R3-6 IAT thunk routing 이 다음 trigger.
> **own1 정합**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift **0줄** 유지. 자체 코드 — `native/pe_to_macho_shim.cpp` 2700+ line + `native/helper.mm` 240+ line + `lib/loader/pe_i386_*.hexa` 6 모듈.
> **참조**: [R3_ROADMAP.md](R3_ROADMAP.md) (sub-phase 1-12 정의) / [CLOSURE_FINAL_STATE.md](CLOSURE_FINAL_STATE.md) (R1+R2 84% → 100% timeline) / [feedback_own1_no_third_party_wine.md](../../.claude-claude3/projects/-Users-ghost-core-airgenome/memory/feedback_own1_no_third_party_wine.md).

---

## 1. 현재 상태 요약

R3 cycle 진입 후 cycle 91-94 의 4 sprint 동안 sub-phase R3-1 ~ R3-7 완료 + R3-8 ~ R3-12 의 hexa-side skeleton 완료. shim 측 통합 phase 14-17 이 동작하여 Battle.net Setup entry RVA 부터 실 i386 instruction stream interpret 진척이 **17 instruction** 도달. R3 ROADMAP §누적 closure 진입율 표에서 R3-7 = 45% 시점에 일치.

### 1.1 closure 100% (R1+R2 own1 §A)

R1 (helper.mm IPC) + R2 (PE 실 mmap + IAT + TEB/PEB + entry trap + Metal device probe) 의 own1 §A 정합 closure 는 commit `c6e7611` 시점 도달 — shim phase 1-13 통과. 본 V2 작성 시점에도 그대로 유지 (R3 작업이 phase 14+ 신규 추가, 기존 phase 1-13 회귀 0).

### 1.2 R3-1~7 완료 (shim + hexa interp layer)

| sub-phase | 영역 | commit | self-test |
|---|---|---|---|
| R3-1 | shim entry 32 byte disasm (`phase14_i386_disasm`) | `7116e96` (race 묶임) | Battle.net Setup entry 32 inst decode PASS |
| R3-2 | hexa pe_i386_disasm coverage tracker | `7cdab67` | opcodes=5, unknown=2, entry_samples=5, top=mov, emits=24 |
| R3-3 | hexa pe_i386_register_state model | `f67e95b` | gp=8, seg=6, eflag=5, eip=1, snap=1, emits=22 |
| R3-4 | hexa pe_i386_memory_model | `7116e96` | region=6, seg=6, fault=2, emits=16 |
| R3-5 | shim single-instruction interp (`phase15_*`) | `9126c84` | register-only 8 opcode 통과 |
| R3-6 | shim memory access (`phase16_*`) | `9126c84` | stack push/pop 양방향 PASS |
| R3-7 | shim control flow (`phase17_*`) | `9126c84` | Fibonacci(5)=5 recursion PASS |

### 1.3 R3-8~12 hexa skeleton 완료

shim 측 phase 18-21 은 미통합이지만 hexa 측 추적기/스켈레톤 모듈은 commit `9126c84` 에 6 phase 일괄 push:

- `pe_i386_seh_chain.hexa` — FS:[0] EXCEPTION_REGISTRATION_RECORD chain skeleton
- `pe_i386_fpu_sse_stub.hexa` — x87 ST0-ST7 + XMM0-XMM7 layout skeleton
- `pe_i386_peb_ldr.hexa` — PEB_LDR_DATA + LDR_DATA_TABLE_ENTRY chain skeleton
- `pe_i386_user32_stub.hexa` — USER32 핵심 export stub schema
- `pe_i386_gdi32_stub.hexa` — GDI32 GDI handle table schema
- `pe_i386_dialog_appkit.hexa` — DialogBoxParamA → NSWindow translation schema

### 1.4 shim 통합 phase 14-17 동작 — 17 instruction 진척

cycle 95 sprint commit `2b90d2c` 에서 shim 측 phase 14-15 opcode coverage 확장 — entry RVA 부터 7 → **17 instruction** 누적 진척 (Battle.net Setup `Battle.net Setup.exe`).

instruction sequence:

```
[0]  e8 ae 09 00 00       call rel32=+0x9AE          (entry → C runtime preamble)
[1]  c7 05 ?? ?? 00 00 00 mov [mem32], imm32         (cookie init)
[2]  68 ?? ?? ?? ??        push imm32                 (security cookie seed)
[3]  b8 ?? ?? ?? ??        mov eax, imm32             (encoder constant)
[4]  3b c8                cmp ecx, eax               (cookie compare)
[5]  74 ??                 je rel8                     (skip rand cookie)
[6]  e8 ?? ?? ?? ??        call rel32                  (RtlGetRandom-equivalent)
[7]  55                   push ebp                    (frame setup)
[8]  8b ec                mov ebp, esp                (frame ptr)
[9]  83 ec ??              sub esp, imm8              (local space)
[10] 83 e0 ??              and eax, imm8              (low-bit clear)
[11] 8d 4d ??              lea ecx, [ebp+disp8]        (locals addr)
[12] 83 e1 ??              and ecx, imm8               (align mask)
[13] 50                   push eax                    (param 1)
[14] 51                   push ecx                    (param 2)
[15] 6a ??                 push imm8                   (param 3 = nShow-equiv)
[16] ff 15 b0 a3 6c 00     call dword ptr [0x6CA3B0]   ← IAT indirect
                                                        bad_branch_target HALT
```

phase 17 control flow 는 `call rel32` / `ret` / `jmp rel*` / `jcc rel*` 까지 정상 동작 — IAT indirect (`call [mem]` ModR/M=disp32 form) 만 미해결.

---

## 2. opcode coverage 표

cycle 95 시점 shim phase 14 (decode) + phase 15 (interp) 의 opcode coverage. 빈도 추정은 Battle.net Setup entry 부터 17 instruction sample + Microsoft C runtime startup typical 패턴 추정.

| opcode hex | mnemonic | phase14 decode | phase15 interp | 빈도 추정 |
|---|---|---|---|---|
| 0x01 / 0x03 | ADD r/m, r / ADD r, r/m | ✓ | ✓ | 매우 빈번 |
| 0x09 / 0x0B | OR r/m, r / OR r, r/m | ✓ | ✓ | 매우 빈번 |
| 0x11 / 0x13 | ADC r/m, r / ADC r, r/m | ✓ | ✓ | 가끔 |
| 0x19 / 0x1B | SBB r/m, r / SBB r, r/m | ✓ | ✓ | 가끔 |
| 0x21 / 0x23 | AND r/m, r / AND r, r/m | ✓ | ✓ | 매우 빈번 |
| 0x29 / 0x2B | SUB r/m, r / SUB r, r/m | ✓ | ✓ | 매우 빈번 |
| 0x31 / 0x33 | XOR r/m, r / XOR r, r/m | ✓ | ✓ | 매우 빈번 |
| 0x39 / 0x3B | CMP r/m, r / CMP r, r/m | ✓ | ✓ | 매우 빈번 |
| 0x50-0x57 | PUSH r32 | ✓ | ✓ | 매우 빈번 |
| 0x58-0x5F | POP r32 | ✓ | ✓ | 매우 빈번 |
| 0x68 | PUSH imm32 | ✓ | ✓ | 빈번 |
| 0x6A | PUSH imm8 | ✓ | ✓ | 빈번 |
| 0x70-0x7F | Jcc rel8 (16 condition) | ✓ (74/75/7C-7F) | ✓ all 16 | 빈번 |
| 0x81 | grp1 r/m32, imm32 | ✓ | ✓ | 빈번 |
| 0x83 | grp1 r/m32, imm8 (sign-ext) | ✓ | ✓ | 매우 빈번 |
| 0x84 / 0x85 | TEST r/m, r | ✓ | ✓ | 빈번 |
| 0x89 | MOV r/m32, r32 | ✓ | ✓ | 매우 빈번 |
| 0x8B | MOV r32, r/m32 | ✓ | ✓ | 매우 빈번 |
| 0x8D | LEA r32, m | ✓ | ✓ | 빈번 |
| 0x90 | NOP | ✓ | ✓ | 가끔 |
| 0xA8 / 0xA9 | TEST AL/EAX, imm | ✓ | ✓ | 빈번 |
| 0xB8-0xBF | MOV r32, imm32 | ✓ | ✓ | 매우 빈번 |
| 0xC2 / 0xC3 | RET imm16 / RET | ✓ | ✓ | 매우 빈번 |
| 0xC7 | MOV r/m32, imm32 | ✓ | ✓ | 빈번 |
| 0xC9 | LEAVE | ✓ | ✓ | 가끔 |
| 0xCC | INT3 | ✓ | ✓ | 드물 |
| 0xE8 | CALL rel32 | ✓ | ✓ | 매우 빈번 |
| 0xE9 / 0xEB | JMP rel32 / JMP rel8 | ✓ | ✓ | 빈번 |
| 0xF6 / 0xF7 | grp3 (TEST/NOT/NEG/MUL/IMUL/DIV/IDIV) | ✓ | ✓ | 가끔 |
| 0xFF | grp4 (INC/DEC/CALL r/m / JMP r/m / PUSH r/m) | ✓ | ✓ | 빈번 |
| 0x0F 80-8F | Jcc rel32 (16 condition) | ✓ | ✓ | 빈번 |
| 0x0F AF | IMUL r32, r/m32 | ✓ | ✓ | 가끔 |
| 0x0F B6 / 0x0F B7 | MOVZX r32, r/m8 / r/m16 | ✓ | ✓ | 빈번 |
| 0x0F BE / 0x0F BF | MOVSX r32, r/m8 / r/m16 | ✓ | ✓ | 빈번 |

### 2.1 미구현 opcode — 향후 trigger

| opcode hex | mnemonic | trigger sub-phase |
|---|---|---|
| 0xF2 / 0xF3 | REPNE / REP prefix (string ops) | R3-7 보강 또는 R3-9 |
| 0x66 | operand-size prefix (16-bit) | R3-9 |
| 0x67 | address-size prefix (16-bit) | R3-9 |
| 0xD8-0xDF | x87 FPU instruction (FLD/FADD/FSTP/...) | R3-9 |
| 0x0F 10-17 | SSE MOVUPS / MOVHPS 등 | R3-9 |
| 0x0F 28-2F | SSE MOVAPS / SSE math | R3-9 |
| 0x0F 38 / 3A | SSE3+ / SSSE3 / SSE4.x extended | R3-9 (선택적) |
| 0x0F 40-4F | CMOVcc r32, r/m32 (16 condition) | R3-7 보강 |
| 0x0F 90-9F | SETcc r/m8 (16 condition) | R3-7 보강 |
| 0x0F A3 / AB / B3 / BB | BT / BTS / BTR / BTC | R3-9 보강 |
| 0x0F C8-CF | BSWAP r32 | R3-9 보강 |
| 0xF0 | LOCK prefix (atomic) | R3-8 (SEH 와 함께) |
| 0x64 / 0x65 | FS: / GS: segment override prefix | R3-8 (SEH FS:[0] 진입) |

REP prefix 미구현이 가장 큰 잠재 trigger — `memcpy` / `memset` C runtime 의 `rep movsd` / `rep stosd` 가 미해결 시 startup 의 cookie / TLS init 진행 차단 가능.

---

## 3. 막힌 지점 분석 — IAT indirect call

### 3.1 증상

shim phase 17 의 inst[16] 위치에서 `call dword ptr [0x6CA3B0]` 실행 시 phase 17_ctrl_flow 의 `bad_branch_target` halt 발생 — disp32 = 0x6CA3B0 은 PE image base 의 IAT 영역 (`IMAGE_DIRECTORY_ENTRY_IAT` RVA 내 entry) 이지만 phase 15_setup_regions 의 region table lookup 에서 **resolved arm64 host pointer** 가 0 (unset).

### 3.2 원인 — 이중 mmap 분기

R1+R2 closure 100% 시점의 mmap 흐름:

1. **phase 9** (helper-side) — `pe_to_macho_shim.cpp` 의 helper IPC frame kind=4 (PE section mmap 요청) → `helper.mm` 이 `mmap(MAP_PRIVATE | MAP_ANON)` + section copy. 결과 host arm64 base pointer 는 helper-side process 메모리.
2. **phase 10** (helper-side) — IAT KERNEL32 routing. helper.mm 측에서 dlsym(RTLD_DEFAULT, "GetCommandLineA") 등 → IAT entry 위치에 64-bit host pointer write. 단, 32-bit IAT slot 이라 lower 32 bit 만 의미 있음 — phase 17 의 indirect call 시 **trampoline trace stub** 로 routing 해야 함.

R3 작업 phase 15 의 `setup_regions` 은 shim 측 main process 에서 **PE image 를 다시 mmap** — phase 9 helper-side mmap 과 별도. 따라서:

- shim-side mmap 의 IAT 영역은 phase 10 의 dlsym patch 가 적용 안 됨 (모두 0 또는 PE 원본 ImportLookupTable 의 hint/name RVA 그대로).
- phase 17 의 `call [0x6CA3B0]` 은 0 (unresolved) 또는 hint/name 영역 의 byte sequence 를 entry RVA 로 오인 → `bad_branch_target` halt.

### 3.3 해결안 — R3-6 IAT thunk routing

shim 측에서 PE Import Directory 의 IAT entry 정보를 자체 추적기에 저장 + phase 17 의 indirect call/jmp 시 IAT 영역 검사 + POSIX dlsym 결과 → trace stub 호출 패턴:

1. **phase 14_5_iat_index** 신규 (shim 측) — PE Import Directory walk + 각 IAT entry 의 (RVA, dll, hint, name) 을 `g_iat_entries[]` static 배열에 등록. 이미 phase 10 의 helper-side 가 동일 walk 를 수행 중 — shim 측은 shim mmap 기준의 RVA 만 재계산.

2. **phase 17 의 `call [mem]` / `jmp [mem]`** — ModR/M=disp32 form (mod=00, r/m=101) 실행 직전:
   - mem addr 이 `[iat_base, iat_base + iat_size)` 영역인지 검사
   - 해당 IAT entry 의 (dll, name) lookup → POSIX `dlsym(RTLD_DEFAULT, name)` 결과 (host arm64 ptr) 획득
   - shim 측 trace stub (`shim_trace_iat_call`) 호출 → 함수 ID 로 dispatch (KERNEL32!GetCommandLineA / USER32!RegisterClassExA / ...).
   - stub 가 EAX=stub return value (typically 0 또는 hardcoded fake handle) 설정 + ESP 의 Win32 stdcall arg cleanup (callee-pop) + EIP=ret_addr (push 된 return address pop) 진행.

3. **trace stub 의 stdcall arg count 추정** — 현재 hexa `pe_i386_user32_stub.hexa` 의 schema 에 함수별 (name, arg_count, ret_type) 기록. shim 측은 동일 table 의 mirror (`shim_iat_function_table[]`) 로 lookup.

### 3.4 예상 도달

해결안 적용 시 inst[16] → inst[25+] 도달 예상. 이유:

- `call [0x6CA3B0]` (KERNEL32!GetSystemTimeAsFileTime 추정) 통과
- 직후 typical pattern: `ret value 를 stack 에 push → xor 로 cookie shuffle → mov [security_cookie], result` (5-6 instruction)
- 이어서 다음 IAT call (`call [0x6CA3B4]` 등) — 또 한 번 IAT routing 필요
- 누적 25-30 instruction 예상 + 다음 차단점은 SEH FS:[0] 진입 (R3-8) 또는 PEB.Ldr lookup (R3-10).

R3 누적 진척 추정: 45% → **50%** 도달 (R3-6 의 ROADMAP 표 기준).

---

## 4. 잔여 trigger

R3 closure 100% 까지의 sub-phase 별 잔여 trigger.

### 4.1 R3-6 IAT thunk routing — 17→25+ inst, 누적 ~50%

**목표**: §3.3 해결안 적용. shim 측 IAT trace stub layer 신규.
**구현 file**: `native/pe_to_macho_shim.cpp` 의 `phase14_5_iat_index` + `phase17_ctrl_flow` 의 indirect call 분기 + `shim_iat_function_table` static 배열.
**검증**: Battle.net Setup entry 부터 inst[25] 도달 + IAT trace 로그 (`[IAT] call KERNEL32!GetSystemTimeAsFileTime → host=0x?` 등).
**도달성 누적**: ~50%.
**own1 §A**: dlsym + 자체 dispatch table — Wine `dlls/kernel32` 또는 Box86 `src/wrapped/wrappedkernel32.c` lift 0줄.

### 4.2 R3-8 SEH FS:[0] runtime — try/catch 진입 시 ~55%

**목표**: shim 측 phase 18 신규 — TEB FS:[0] 의 EXCEPTION_REGISTRATION_RECORD chain 의 실 동작. exception throw 시 chain 순회 + handler 호출.
**전제**: hexa skeleton 완료 (`pe_i386_seh_chain.hexa`, commit `9126c84`).
**검증**: synthetic `int3` 또는 `mov [0], 0` (null deref) 후 등록된 SEH handler 호출 + EXCEPTION_RECORD.ExceptionCode=0xC0000005 검증.
**도달성 누적**: ~55%.
**own1 §A**: `<excpt.h>` + WinNT.h reference. Wine `dlls/ntdll/signal_i386.c` lift 0줄.

### 4.3 R3-9 FPU/SSE — float 사용 시 ~60%

**목표**: shim 측 phase 19 신규. x87 ST0-ST7 stack + XMM0-XMM7 + 핵심 instruction (fld/fstp/fadd/movss/movsd/addss/addsd).
**전제**: hexa skeleton 완료 (`pe_i386_fpu_sse_stub.hexa`).
**검증**: PE 의 single FP instruction sequence (fld const1.0 / fld const2.0 / faddp / fstp result) 실행 후 result=3.0 검증. host arm64 NEON SIMD 활용.
**도달성 누적**: ~60%.
**own1 §A**: Box86 `src/dynarec/dynarec_x87.c` lift 0줄. IEEE 754 + Intel SDM Vol 1 reference.

### 4.4 R3-10 PEB/Ldr — KERNEL32!GetModuleHandleA 호출 시 ~70%

**목표**: shim 측 phase 20 + hexa `pe_i386_ntdll_stub.hexa` 신규. PEB.Ldr 의 InMemoryOrderModuleList 채움 + NTDLL 핵심 export stub.
**전제**: hexa skeleton 완료 (`pe_i386_peb_ldr.hexa`).
**검증**: GetModuleHandleA("KERNEL32") → PEB.Ldr lookup → fake LDR_DATA_TABLE_ENTRY DllBase 반환. RtlInitUnicodeString 의 Length / MaximumLength 정합.
**도달성 누적**: ~70%.
**own1 §A**: ReactOS public docs + `<winternl.h>` reference. Wine `dlls/ntdll/loader.c` 의 attach_implicitly_loaded_dlls lift 0줄.

### 4.5 R3-11 USER32/GDI32 stub — RegisterClassExA / CreateWindowExA ~85%

**목표**: shim 측 phase 21 + hexa `pe_i386_user32_stub.hexa` / `pe_i386_gdi32_stub.hexa` 의 실 동작.
**전제**: helper.mm IPC kind=9 (NSWindow lifecycle) prereq — `EXTERNAL_DECISIONS_GUIDE.md` § 1.5 의 5 handler TODO.
**검증**: WinMain entry → RegisterClassExA → CreateWindowExA → ShowWindow chain 의 instruction 실행 + helper IPC 로 NSWindow 생성 요청.
**도달성 누적**: ~85%.
**own1 §A**: AppKit translation 자체 구현. Wine `dlls/winex11.drv` / `winemac.drv` lift 0줄.

### 4.6 R3-12 first dialog — NSEvent → Win32 message ~100%

**목표**: USER32 message → NSEvent translation + GetMessage / DispatchMessage 의 첫 iteration 진입. Battle.net Setup 의 EULA / install path picker UI element visible.
**전제**: helper.mm + NSWindow runtime + R3-11 통과.
**검증**: 화면에 NSWindow 표시 + EULA / install path picker UI element (button/textfield) visible. event loop 의 GetMessage 가 WM_QUIT 또는 user click 까지 block.
**도달성 누적**: 100% (R3 closure).
**own1 §A**: USER32 message → NSEvent 자체 매핑. CrossOver / GPTK / Whisky lift 0줄.

---

## 5. own1 enforcement 재확인

R3 cycle 91-95 sprint 동안 own1 §A enforcement 위반 0건. 검증 방법:

| 영역 | 검증 명령 | 기대 결과 | 현재 |
|---|---|---|---|
| Wine 의존 | `grep -r -i "wine\|crossover" native/ lib/loader/pe_i386_*.hexa` | 0 hit | ✓ 0 hit |
| Capstone / Zydis / iced | `grep -r -i "capstone\|zydis\|^iced" native/` | 0 hit | ✓ 0 hit |
| Box86 / QEMU / Bochs | `grep -r -i "box86\|qemu\|bochs" native/ lib/loader/` | 0 hit | ✓ 0 hit |
| Apple GPTK / Whisky | `grep -r -i "gptk\|whisky\|game.porting.toolkit" .` | docs 의 거부 언급 외 0 hit | ✓ 정합 |

**자체 코드 양**:

- `native/pe_to_macho_shim.cpp` — 2700+ line (cycle 95 commit `2b90d2c` 시점). phase 1-13 (R1+R2 closure) + phase 14-17 (R3-1/5/6/7 통합).
- `native/helper.mm` — 240+ line. IPC kind 1-8 (subprocess spawn / dlopen / dlsym / mmap / TEB init / Metal device probe / ...). R3-11/12 의 kind=9 (NSWindow) 는 향후 추가.
- `lib/loader/pe_i386_*.hexa` — R3-2/3/4 + R3-8/9/10/11/12 skeleton 의 6+ 모듈.

own1 §A 정합 — Apple SDK + Microsoft PE/COFF spec + Intel SDM + ReactOS public docs reference only. Apple Rosetta 2 는 i386 미지원 (x86_64 만) — DIY 자체 구현 외 path 부재.

own1 own #1 (타사 Wine/CrossOver/GPTK/Whisky 추천 금지) — R3 cycle 의 trigger / game-changer 표현 시 외부 lift 추천 발생 0건. enforcement 맥락에서만 언급.

---

## 6. cumulative commit log (R3 관련)

cycle 90 (R3 entry) → cycle 95 (현재) 의 R3 관련 commit chain:

| commit SHA | 영역 | line delta |
|---|---|---|
| `84c31ec` | docs: R3 ROADMAP — sub-phase 1-12 정의 | docs +259 line |
| `7116e96` | R3-1 (shim phase 14 disasm cpp +575) + R3-4 hexa pe_i386_memory_model (commit message race) | cpp +575, hexa +320 |
| `f67e95b` | R3-3 hexa pe_i386_register_state | hexa +280 |
| `7cdab67` | R3-2 hexa pe_i386_disasm coverage tracker | hexa +245 |
| `7dc4b20` | docs: R3-1~4 status note (4 phase 완료 표기 + commit race 기록) | docs +95 |
| `9126c84` | R3-5/6/7 shim phase 15-17 interpreter + R3-8~12 hexa skeleton (6 phase 일괄) | cpp +820, hexa +1180 |
| `2b90d2c` | shim phase 14-15 opcode 확장 — 7→17 instruction 진척 | cpp +180 |

**누적**: cpp +1575 line (shim) / hexa +2025 line / docs +354 line. 7 commit, 4 sprint (cycle 91-94 + 95 부분).

### 6.1 commit message race 기록

`7116e96` 은 동시 작업 race 로 R3-1 (shim phase 14 disasm) + R3-4 (hexa memory_model) 의 diff 가 묶임. message 표기는 R3-4 (memory_model) 만 — diff inspect 시 R3-1 cpp +575 line 포함. `7dc4b20` docs status note 에 race 사실 명시 — 향후 cherry-pick / bisect 시 본 race 인지 필수.

---

## 7. v3 trigger 조건

R3 PROGRESS V3 작성 trigger:

1. **R3-6 IAT thunk routing 완료** — 누적 50% 도달 + inst[25+] 진척 검증 후 V3 에서 R3-7 control flow 의 보강 영역 (CMOVcc / SETcc / REP prefix) 재추정.
2. **R3-8 SEH 진입** — exception throw 검증 후 V3 에서 R3-9/10 의 sub-phase 분해 재검토.
3. **R3-10 PEB/Ldr 통과** — 누적 70% 도달 시 V3 에서 R3-11/12 의 helper.mm IPC kind=9 prereq 갱신.
4. **R3 closure 100% 도달** → V4 신규 작성 (R4 binary translation B path 또는 R5 D3D → Metal path 평가).
5. **plateau 발생** — 2 sprint 연속 진척 0 시 V3 에서 sub-phase 재분배 또는 외부 결정 prereq 추가.

→ 본 V2 가 cycle 95-100 sprint 동안 유효. cycle 97 즈음 R3-6 IAT thunk routing 도달 기대 시점에 V3 재작성.

---

## 8. closure path 통합 정리

R1 + R2 + R3 = 자체 i386 PE32 launcher (own1 §A 정합):

| layer | 영역 | closure 기준 | 현재 |
|---|---|---|---|
| R1 | helper.mm IPC (subprocess / dlopen / dlsym / mmap / TEB / Metal probe) | shim phase 1-13 통과 | ✓ 100% |
| R2 | PE 실 mmap + IAT KERNEL32 routing + entry trap + Metal device probe | shim phase 9-13 통과 | ✓ 100% |
| R3-1~7 | shim disasm + interp + memory access + control flow | 17 inst + Fibonacci recursion PASS | ✓ 45% |
| R3-8~12 | SEH / FPU/SSE / PEB-Ldr / USER32-GDI32 / first dialog | hexa skeleton + shim 미통합 | △ 0% (skeleton)|
| **closure 통합** | "Wine 0줄 + R1+R2+R3 = real PE32 i386 launcher" | Battle.net Setup first dialog 표시 | ~45% |

own1 enforcement 일관 — cycle 90 의 closure 100% (R1+R2) 는 phase 1-13 의 자체 구현. cycle 91-95 의 R3 진척 또한 외부 emulator / Wine fork lift 0줄. **자체 i386 execution layer** 의 cohesion 누적 ~45% — 잔여 ~55% 의 sub-phase 6 (R3-6/8/9/10/11/12) 가 V2 → V3 → V4 cycle 동안 진척 대상.

---

*written 2026-05-01 cycle 95 sprint 직후. R1+R2 own1 §A closure 100% 유지 + R3-1~7 완료 + R3-8~12 hexa skeleton 완료 + shim 통합 phase 14-17 동작 + 17 instruction 도달. inst[16] IAT indirect (`call [0x6CA3B0]`) 의 bad_branch_target halt 가 다음 trigger — R3-6 IAT thunk routing 적용 시 25+ instruction 도달 예상 + 누적 50% 도달. own1 §A enforcement 일관 (Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift 0줄). 본 PROGRESS V2 path 외 수정 X. Apple SDK + Microsoft PE/COFF spec + Intel SDM + ReactOS public docs reference only.*
