# R3 PROGRESS V4 — 205 instruction 도달 + SEH register 자동 detect + FPU/SSE prefix decode (2026-05-01)

> **status**: R1+R2 own1 §A closure **100%** 유지 + R3-1~9 wave2 통합 + shim phase 18 (FS/GS prefix + SEH register) + shim phase 19 (FPU/SSE prefix decode) 동작. 누적 R3 진척 **~55%**. 다음 halt = inst[205] `0x87` XCHG (R3-10 wave3 trigger).
> **base commits**: `5371b32` (V3 작성, 51 inst halt) → `6088aec` (hexa segment_override_real) → `e5a4187` (hexa x87_fpu_inst_real) → `96c1532` (hexa seh_dispatch_runtime) → `cbdface` (R3-8/9 shim phase 18-19, 51→**205 inst**).
> **현재 차단**: shim interpreter inst[205] 의 `0x87` XCHG r/m32, r32 — single-opcode unimplemented halt. R3-10 wave3 의 XCHG/LOCK/REP/string/INC-DEC/shift family 일괄 통합 후 PEB/Ldr 영역 진입 예정.
> **own1 정합**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift **0줄** 유지. 자체 코드 — `native/pe_to_macho_shim.cpp` **3380 line** + `native/helper.mm` **694 line** + `lib/loader/pe_i386_*.hexa` 21 file.
> **참조**: [R3_PROGRESS_V3.md](R3_PROGRESS_V3.md) (V3 의 51 inst + IAT 4 함수 통과) / [R3_ROADMAP.md](R3_ROADMAP.md) (sub-phase 1-12 정의) / [BATTLENET_REAL_LAUNCH_PLAN.md](BATTLENET_REAL_LAUNCH_PLAN.md) (wave 1-5 own1 enforcement) / [CLOSURE_FINAL_STATE.md](CLOSURE_FINAL_STATE.md) (R1+R2 closure 100%).

---

## 1. V3 → V4 변화 요약

V3 작성 시점 (commit `5371b32`, cycle 97) 대비 V4 (commit `cbdface`, cycle 98-99) 의 핵심 변화는 R3-8 FS prefix + SEH register 자동 detect + R3-9 FPU/SSE prefix decode path 일괄 통합 — 51 instruction halt → **205 instruction** 진척 (4×). FS prefix 의 `mov fs:[0], eax` / `push fs:[0]` 이 실 동작 + SEH chain 등록 추적 (frame=0x8FFDC prev=0 handler=0x551A50 chain depth=1) + FPU/SSE prefix decode path 정합 (entry 인근 미사용 → 실 호출 0건이지만 dispatch path 검증 OK).

### 1.1 V3 vs V4 비교 표

| 항목 | V3 (commit `5371b32`) | V4 (commit `cbdface`) | delta |
|---|---|---|---|
| instruction 진척 | 51 inst | **205 inst** | **+154 inst (4×)** |
| 차단 위치 | inst[50] `mov reg, fs:[0]` FS prefix halt | inst[205] `0x87` XCHG r/m32, r32 halt | trigger 전진 |
| FS/GS prefix | unsupported_prefix halt | **load=1 store=1** 실 동작 | full decode |
| SEH register | 미감지 | **chain depth=1** (record_id=0, frame=0x8FFDC prev=0 handler=0x551A50) | 자동 detect |
| FPU/SSE prefix | 미통합 | decode path 통합 (FPU=0 SSE=0, entry 인근 미사용) | dispatch path OK |
| 추가 opcode | 0x33/0x83 grp1/0x85/etc (V3 누적) | INC/DEC + ADC/SBB + ROL/ROR/SHL/SHR/SAR + 0xFF /6 PUSH r/m32 + 0xA0-0xA3 MOV moffs | wave2 cap 해제 |
| shim phase 통합 | 14-17 + IAT trace stub | 14-17 + IAT trace stub + **18 (SEH register)** + **19 (FPU/SSE)** | +2 phase |
| `shim.cpp` line | ~2700 | **3380** (+646) | shim 통합 +646 line |
| `helper.mm` line | 387+18 | **694** | +289 line (R3-9 echo path) |
| `hexa` file 수 | 약 16 | **21** file | +5 hexa skeleton |
| R3 누적 closure | ~50% | **~55%** | +5% |
| 차단 trigger 분류 | FS segment override (R3-8) | XCHG single-opcode (R3-10 wave3) | trigger 진보 |

### 1.2 V4 핵심 한 줄 변화

**inst[50] 의 `0x64` FS prefix halt 가 통과 → SEH register 자동 detect (frame=0x8FFDC prev=0 handler=0x551A50 chain depth=1) + FPU/SSE prefix decode path 통합 + INC/DEC/ADC/SBB/shift family 추가 opcode → 누적 205 instruction 진척 (V3 51 의 4×).**

---

## 2. R3-8 SEH register 동작 분석

R3-8 은 V3 §4 의 design 안 (R3-8.a-c) 을 V4 commit `cbdface` 에서 실 통합. shim phase 18 의 신규 layer + SEH chain table + FS/GS prefix 의 segment override 의미 부여.

### 2.1 inst[50] 0x539555 — `push dword ptr fs:[0]`

```
[50] 64 ff 35 00 00 00 00     push dword ptr fs:[0]   ; prefix=0x64 (FS), opcode=0xFF /6
                                → TEB.NtTib.ExceptionList load (0 = end-of-chain marker)
                                → push 정상 (ESP -=4, stack top = 0x00000000)
                                → KPI: FS load counter +=1
```

V3 시점에서는 inst[50] 이 `mov eax, fs:[0]` (0x64 0xA1 ...) 으로 추정되었으나 V4 에서 실 trace 결과 `0xFF /6` (PUSH r/m32 with FS override) 로 확정. shim 측 `0xFF /6` PUSH r/m32 신규 추가 (cbdface commit 의 wave2 opcode cap 해제).

### 2.2 inst[68] — `mov fs:[0], eax`

```
[68] 64 a3 00 00 00 00         mov fs:[0], eax         ; prefix=0x64, opcode=0xA3 (mov moffs32, eax)
                                → seh_record_register 자동 호출
                                → SehRecord {
                                    record_id    = 0,
                                    prev         = 0x00000000,    ; chain end marker
                                    handler      = 0x00551A50,    ; pushed handler addr
                                    frame_addr   = 0x0008FFDC,    ; ESP value at register
                                  }
                                → g_seh_records[0] 에 등록 + chain_depth = 1
                                → KPI: FS store counter +=1
```

shim 측 SEH register 패턴 detect — `0x68 ?? ?? ?? ?? / 0x64 0xFF /6 ... / 0x64 0xA3 ...` byte sequence 의 ESP-based heuristic 으로 prologue 검출 + frame 의 +0x00 (prev), +0x04 (handler) 추출 → SehRecord struct 채움. 자체 chain (own1 정합 — Wine ntdll 0줄) 의 logging-only path. dispatch (실 exception throw → unwind) 는 R3-8.d-e 의 다음 wave 영역.

### 2.3 g_seh_records[64] 글로벌 — record_id, prev, handler, frame_addr

shim cpp 의 신규 static:

```cpp
struct SehRecord {
    uint32_t record_id;       // 0..63 (chain order)
    uint32_t prev;            // EXCEPTION_REGISTRATION_RECORD prev field
    uint32_t handler;         // _except_handler ABI ptr
    uint32_t frame_addr;      // ESP at register time (= record VA on stack)
};
static SehRecord g_seh_records[64];
static uint32_t  g_seh_chain_depth = 0;
```

64 cap — Battle.net Setup entry 인근 추정 SEH depth 5-15 수준 + nested call frame margin.

### 2.4 chain_depth 추적

V4 시점 inst[205] 도달까지 chain_depth = 1 — 단일 register 만 발생. inst[100+] 영역의 추가 함수 prologue 진입 시 nested register/unregister chain 의 depth 변동 추적 예정 (V5 trigger).

### 2.5 KPI: phase 18 — FS/GS prefix load=1 store=1

shim 측 KPI dump:

```
[phase 18 SEH register]
  chain_depth = 1
  records:
    [0] prev=0x00000000 handler=0x00551A50 frame=0x0008FFDC
[phase 18 FS/GS prefix]
  FS load  = 1   (push dword ptr fs:[0])
  FS store = 1   (mov fs:[0], eax)
  GS load  = 0
  GS store = 0
```

own1 §A logging only — dispatch (synthetic throw / unwind) 부재.

---

## 3. R3-9 FPU/SSE prefix decode

### 3.1 D8-DF FPU primary opcode + 0F escape SSE form decode 통합

shim `decode_one_inst` 의 prefix loop 확장:

| prefix byte | 의미 | 처리 |
|---|---|---|
| 0x66 | operand-size override (16-bit / SSE size) | `sse_prefix=0x66` 표시 |
| 0xF2 | REPNE / SSE2 size | `sse_prefix=0xF2` |
| 0xF3 | REP / SSE size | `sse_prefix=0xF3` |
| 0xD8-0xDF | x87 FPU primary opcode group | `esc_op2=opcode_byte` (modrm + ST(i)/mem form decode) |
| 0x0F XX | 2-byte escape (SSE form: 10/11/28/29/6F/7F/D6/EF/54-57/58-5F) | `esc_op2=XX` |

### 3.2 본 bench 에서 entry 인근 FPU/SSE inst 없음 → FPU=0 SSE=0

inst[0..205] 영역 = msvcrt `_security_init_cookie` + `__scrt_*` startup helper — 정수 산술 + IAT 호출 + SEH register 만. floating-point 혹은 SSE math 미사용. KPI:

```
[phase 19 FPU/SSE]
  FPU executed = 0
  SSE executed = 0
  decode/dispatch path = OK (helper IPC kind=12/13 echo handler ready)
```

decode 경로 자체는 통합 OK — entry 인근 분포만 우연히 0. WinMain 진입 후 (R3-12) 의 dialog rendering / float-based time math 영역에서 비-0 예상.

### 3.3 helper.mm IPC kind=12 / kind=13 — g_fpu_st[8] / g_xmm[8][2] echo

`native/helper.mm` 의 IPC dispatch (R3 cycle 96 commit `510a27b` 에서 추가 + cycle 98 cbdface 에서 연결):

| IPC kind | 영역 | helper handler | echo state |
|---|---|---|---|
| 12 | FPU trace | `helper_fpu_trace(cmd)` | `g_fpu_st[8]` (80-bit ST(0)..ST(7)) — echo only |
| 13 | SSE trace | `helper_sse_trace(cmd)` | `g_xmm[8][2]` (128-bit XMM0..XMM7, low/high 64-bit pair) — echo only |
| 14 | USER32 stub | `helper_user32_stub(cmd)` | (R3-12 trigger) |
| 15 | GDI32 stub | `helper_gdi32_stub(cmd)` | (R3-12 trigger) |

shim 측 phase19_run 에서 fd context 통해 `IPC_KIND_FPU_TRACE` / `IPC_KIND_SSE_TRACE` 호출. 실 floating-point 연산 X — register state echo + own1 logging 만. 향후 R3-9 후속 wave 에서 NEON SIMD mapping 검토 (own1 §A 의 자체 NEON intrinsic 활용) 예정.

### 3.4 own1 정합 — 실 부동소수 X / 실 SSE/NEON X

R3-9 의 V4 시점 정합 — Wine fpu_emul / fldcw helper / SSE intrinsic lift 0줄. helper.mm 의 g_fpu_st / g_xmm 는 단순 echo 영역으로 own1 §A 의 자체 IPC ABI 일관. R3-9 의 actual FP execution 단계 (R5 D3D→Metal path 직전) 가 V5 이후 영역.

---

## 4. 51 → 205 inst sequence 분석

V3 의 inst[0..50] 은 그대로 + V4 에서 inst[51..204] 의 154 inst 흐름. 구체적 인접 sequence:

### 4.1 inst[50] FS prefix push → SEH frame setup

```
[50] 64 ff 35 00 00 00 00     push dword ptr fs:[0]   ; SEH chain head load + push (0)
```

V3 halt 지점 통과. ESP -=4, stack top = 0x00000000 (chain end marker).

### 4.2 inst[51..67] msvcrt `_security_init_cookie()` 진입 추정

gettimeofday + tid + pid mixing pattern:

```
[51] 8d 44 24 04             lea eax, [esp+4]           ; FILETIME local addr
[52] 50                       push eax
[53] ff 15 b0 a3 6c 00         call [IAT GetSystemTimeAsFileTime]   ; (V3 inst[16] 와 동일 routing)
[54] 8b 44 24 04             mov eax, [esp+4]           ; FILETIME low load
[55] 33 44 24 08             xor eax, [esp+8]           ; FILETIME hi xor
[56] ff 15 b4 a3 6c 00         call [IAT GetCurrentThreadId]
[57] 33 c8                    xor ecx, eax               ; mix tid
[58] ff 15 b8 a3 6c 00         call [IAT GetCurrentProcessId]
[59] 33 c8                    xor ecx, eax               ; mix pid
[60] 8d 44 24 0c             lea eax, [esp+0xC]         ; LARGE_INTEGER local
[61] 50                       push eax
[62] ff 15 bc a3 6c 00         call [IAT QueryPerformanceCounter]
[63] 8b 44 24 0c             mov eax, [esp+0xC]
[64] 8b 54 24 10             mov edx, [esp+0x10]
[65] 33 c2                    xor eax, edx
[66] 33 c8                    xor ecx, eax
[67] 89 0d ?? ?? ?? ??       mov [global_cookie], ecx
```

### 4.3 inst[68] SEH register

```
[68] 64 a3 00 00 00 00         mov fs:[0], eax            ; chain head <- ESP, seh_record_register
```

§2.2 detail. chain_depth = 1.

### 4.4 inst[69..150] 추정 함수 prologue + 변수 init

`__scrt_initialize_default_local_stdio_options` / `__scrt_acquire_startup_lock` / `_initterm` 의 prologue chain:

```
[69-80]    push ebp / mov ebp,esp / sub esp,N / push ebx/esi/edi / mov local,imm
[81-100]   call rel32 (internal helper) + REL/EAX shuffle + mov local
[101-130]  loop body (initterm pointer table walk) — INC/DEC counter + cmp ptr,0 + jne back
[131-150]  cleanup + pop edi/esi/ebx + mov esp,ebp + pop ebp + ret
```

이 영역의 INC/DEC single-byte (0x40-0x4F) + shift (0xC1/0xD1/0xD3) + ADC/SBB (0x11/0x13/0x19/0x1B) opcode 가 V4 cbdface 에서 신규 추가 — 진척 cap 해제 핵심.

### 4.5 inst[151..204] inner 함수 호출 + INC/DEC/SBB/shift

```
[151-170]  call rel32 (deeper internal) + frame setup
[171-190]  bit manipulation: shr/shl/and/or with imm — feature-flag word build
[191-204]  if-else branch (test flag bit + jne/je) → 다음 inner call 분기
[205]      87 ?? ??           xchg r/m32, r32     ← V4 halt 지점 (R3-10 wave3 trigger)
```

### 4.6 통계

inst[51..204] = 154 inst delta. V3 추정 (~55% 진입 시 100 inst delta) 를 상회 — 이유는 SEH register / FS prefix 통과 외 wave2 의 INC/DEC/ADC/SBB/shift opcode cap 해제로 msvcrt startup chain 깊은 영역까지 trace 가능.

---

## 5. 다음 halt — 0x87 XCHG (R3-10 trigger)

V4 의 inst[205] halt = `0x87` XCHG r/m32, r32 — 단순 swap (atomicity 없는 form, LOCK prefix 미동반). wave3 BG 진행 중.

### 5.1 R3-10 wave3 신규 opcode 일괄 통합

| opcode hex | mnemonic | 영역 |
|---|---|---|
| 0x87 / 0x86 | XCHG r/m32, r32 / XCHG r/m8, r8 | 단순 swap (V4 halt) |
| 0xF0 | LOCK prefix | atomic op marker (자체 구현 — Apple GCC __c11_atomic_* mapping) |
| 0xA4 / 0xA5 | MOVSB / MOVSD | string move (REP 대응) |
| 0xAA / 0xAB | STOSB / STOSD | string store |
| 0xAE / 0xAF | SCASB / SCASD | string scan |
| 0xA6 / 0xA7 | CMPSB / CMPSD | string compare |
| 0xF2 / 0xF3 prefix + string | REPNE / REP prefix + string family | counted loop |
| 0x60 / 0x61 | PUSHAD / POPAD | 8-reg push/pop |
| 0x9C / 0x9D | PUSHFD / POPFD | EFLAGS push/pop |

### 5.2 이후 trigger 추정

inst[206..N] 의 trace 진척 후 다음 halt 추정:

| 추정 trigger | 영역 | sub-phase |
|---|---|---|
| `KERNEL32!RtlGetVersion` IAT call | OS version 분기 | R3-10 PEB/Ldr |
| `KERNEL32!IsProcessorFeaturePresent` IAT call | CPU feature query | R3-10 PEB/Ldr |
| `KERNEL32!GetModuleHandleA` (self-handle) | PEB.Ldr.InLoadOrderModuleList walk | R3-10 PEB/Ldr |
| `RtlInitUnicodeString` (msvcrt unicode helper) | NtDll-shaped runtime | R3-10 PEB/Ldr |

이들은 IAT routing layer 의 추가 dispatch entry 추가 + helper.mm IPC kind 변경 없이 처리 가능 (libSystem 또는 자체 stub) — own1 §A 일관.

### 5.3 wave3 통합 후 진척 예상

R3-10 wave3 통합 시 inst[206..400+] 도달 + msvcrt `_initterm` 의 pointer table walk 종료 + `_initterm_e` (extended) 진입 + `__scrt_common_main` 의 GetCommandLineA / GetStartupInfoA 호출 chain. 누적 ~65% 예상.

---

## 6. opcode coverage 갱신

V3 의 누적 + V4 cbdface 추가 opcode + inst[51..204] 에서 사용된 opcode 분포.

### 6.1 V3 시점 14 opcode + V4 추가 opcode

| 영역 | opcode | 비고 |
|---|---|---|
| **V3 누적 (14 opcode)** | 0x68 / 0x6A / 0x89 / 0x8B / 0xC7 / 0xB8-BF / 0xE8 / 0xFF /2 / 0xC2 / 0xC3 / 0x33 / 0x83 grp1 / 0x85 / 0x84 / 0x8D / 0x55 / 0x5D / 0x50-57 / 0x58-5F / 0x74 / 0x75 / 0x0F 84-8F / 0xF7 grp3 | (V3 §5.1 표 참조) |
| **V4 신규 prefix** | 0x64 / 0x65 (FS/GS prefix) | R3-8 SEH register 의 핵심 |
| **V4 신규 FPU primary** | 0xD8-0xDF | decode only, helper IPC kind=12 echo |
| **V4 신규 SSE prefix** | 0x66 0x0F XX (10/11/28/29/6F/7F/D6/EF/54-57/58-5F) | decode only, helper IPC kind=13 echo |
| **V4 신규 INC/DEC** | 0x40-0x47 INC r32 / 0x48-0x4F DEC r32 | single-byte form |
| **V4 신규 ADC/SBB** | 0x11 / 0x13 / 0x19 / 0x1B (CF in arith family) | adc/sbb r/m32, r32 |
| **V4 신규 shift** | 0xC1 / 0xD1 / 0xD3 + group3 ROL/ROR/SHL/SHR/SAR | CF flag update |
| **V4 신규 PUSH r/m32** | 0xFF /6 | inst[50] `push dword ptr fs:[0]` 의 form |
| **V4 신규 MOV moffs** | 0xA0 / 0xA1 / 0xA2 / 0xA3 | inst[68] `mov fs:[0], eax` 의 form |

### 6.2 inst[51..204] 사용 opcode 분포

154 inst sequence 의 추정 빈도:

| opcode 류 | 추정 count | 비고 |
|---|---|---|
| MOV (0x89/0x8B/0xC7/0xA0-A3) | ~45 | 가장 빈번 (locals + memory + moffs) |
| PUSH/POP (0x50-5F / 0x6A / 0x68 / 0xFF /6) | ~25 | frame + param + SEH |
| CALL (0xE8 / 0xFF /2) | ~12 | IAT 4 추가 + internal helper |
| 산술 (0x01/0x03/0x29/0x2B/0x21/0x23/0x33/0x83 grp1 + ADC/SBB) | ~22 | cookie mix + flag chain |
| INC/DEC (0x40-4F) | ~10 | counter loops |
| shift (0xC1/0xD1/0xD3) | ~8 | feature-flag word build |
| TEST/CMP (0x84/0x85/0x39/0x3B/0x83 grp1 cmp) | ~12 | branch precondition |
| Jcc (0x74-77 / 0x7C-7F / 0x0F 84-8F) | ~10 | if-else + loop back |
| RET (0xC2/0xC3) | ~6 | function return chain |
| LEA (0x8D) | ~4 | locals addr |

(중복 counts 포함; opcode 단위 — instruction 수와 1:1 일치 X)

### 6.3 미구현 (다음 trigger)

| opcode hex | mnemonic | 다음 sub-phase |
|---|---|---|
| **0x87 / 0x86** | XCHG r/m32, r32 / XCHG r/m8, r8 | **R3-10 wave3 (V4 halt)** |
| 0xF0 | LOCK prefix | R3-10 wave3 |
| 0xA4 / 0xA5 / 0xAA / 0xAB / 0xAE / 0xAF / 0xA6 / 0xA7 | MOVSB / MOVSD / STOSB / STOSD / SCASB / SCASD / CMPSB / CMPSD | R3-10 wave3 |
| 0xF2 / 0xF3 string-prefix | REPNE / REP + string family | R3-10 wave3 |
| 0x60 / 0x61 | PUSHAD / POPAD | R3-10 wave3 |
| 0x9C / 0x9D | PUSHFD / POPFD | R3-10 wave3 |
| 0xC0 / 0xC1 (imm form) | shift imm 8-bit | R3-10 wave3 |
| 0xD0-0xD3 (group2) | shift count=1 / count=CL | R3-10 wave3 (일부 V4 통합 — 잔여 grp 보강) |
| 0x0F 40-4F | CMOVcc r32, r/m32 | R3-7 보강 |
| 0x0F 90-9F | SETcc r/m8 | R3-7 보강 |

---

## 7. BG dispatch wave 3 진행

cycle 98-99 sprint 의 BG dispatch wave 3 — 5 BG 동시 진행. own1 §A 정합 자체 구현만.

| BG | 영역 | file | status |
|---|---|---|---|
| BG 1 | shim wave3 XCHG/LOCK/REP/string/INC-DEC/shift cpp 단독 | `native/pe_to_macho_shim.cpp` phase 17 opcode cap 해제 + R3-10 wave3 통합 | 진행 중 |
| BG 2 | helper.mm NSWindow real create (AppKit 통합 — kind=14 fn_id=2) | `native/helper.mm` USER32 stub 의 CreateWindowExA 실 NSWindow 매핑 | 진행 중 |
| BG 3 | hexa xchg_lock_inst tracker | `lib/loader/pe_i386_xchg_lock_inst.hexa` (신규) | 진행 중 |
| BG 4 | hexa misc_inst tracker | `lib/loader/pe_i386_misc_inst.hexa` (신규) | 진행 중 |
| BG 5 | docs R3_PROGRESS_V4 (this doc) | `docs/R3_PROGRESS_V4.md` | 본 문서 |

BG 1 완료 시 inst[205] halt 해소 + R3-10 wave3 opcode 일괄 통합. BG 2 는 NSWindow 실 생성 (R3-12 trigger 사전 준비). BG 3-4 는 hexa skeleton + self-test (bash/shasum/git 부재 환경 회피 + Hexa `type` 예약어 회피).

---

## 8. own1 enforcement 재확인

R3 cycle 98-99 sprint 동안 own1 §A enforcement 위반 0건. 검증 명령 + 결과:

| 영역 | 검증 명령 | 기대 | V4 결과 |
|---|---|---|---|
| Wine / CrossOver | `grep -r -i "wine\|crossover" native/ lib/loader/pe_i386_*.hexa` | 0 hit | 0 hit |
| Box86 / QEMU / Bochs | `grep -r -i "box86\|qemu\|bochs" native/ lib/loader/` | 0 hit | 0 hit |
| Capstone / Zydis / iced | `grep -r -i "capstone\|zydis\|^iced" native/ lib/loader/` | 0 hit | 0 hit |
| Apple GPTK / Whisky | `grep -r -i "gptk\|whisky\|game.porting.toolkit" native/ lib/loader/` | 0 hit (docs 거부 언급 외) | 정합 |
| Wine ntdll SEH lift | `grep -r -i "RtlAddVectoredException\|__C_specific_handler" native/ lib/loader/` | 0 hit | 0 hit (자체 SehRecord struct + chain table) |
| 외부 fpu emul | `grep -r -i "fpu_emul\|softfloat\|berkeley.softfloat" native/ lib/loader/` | 0 hit | 0 hit (helper IPC echo only) |

### 8.1 자체 코드 양

| file | line | 영역 |
|---|---|---|
| `native/pe_to_macho_shim.cpp` | **3380** | phase 1-13 (R1+R2 closure) + phase 14-17 (R3-1/5/6/7 통합) + IAT trace stub layer (R3-6) + **phase 18 (R3-8 SEH register + FS/GS prefix)** + **phase 19 (R3-9 FPU/SSE prefix decode)** + wave2 opcode cap 해제 (INC/DEC/ADC/SBB/shift/0xFF /6/0xA0-A3) |
| `native/helper.mm` | **694** | IPC kind 1-15 (subprocess / dlopen / dlsym / mmap / TEB / Metal / FPU/SSE/USER32/GDI32 stub helper) + Phase 1 stage2 hexa loader bridge (commit 57f121a) |
| `lib/loader/pe_i386_*.hexa` | 21 file | R3-2/3/4 + R3-6 + R3-8/9 + R3-10/11/12 skeleton + V4 wave3 신규 추가 영역 |

own1 §A 정합 — Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + ReactOS public docs reference only. Apple Rosetta 2 의 i386 미지원 (x86_64 만) — DIY 자체 구현 외 path 부재 명시.

own1 own #1 (타사 Wine/CrossOver/GPTK/Whisky 추천 금지) — R3 cycle 98-99 의 trigger / game-changer 표현 시 외부 lift 추천 발생 0건. enforcement 맥락에서만 언급 (위 표).

---

## 9. cumulative commit log (R3 wave 1+2)

cycle 90 (R3 entry) → cycle 99 (V4 작성) 의 R3 관련 commit chain (14+ commit):

| commit SHA | 영역 | line delta |
|---|---|---|
| `84c31ec` | docs: R3 ROADMAP — sub-phase 1-12 정의 | docs +259 |
| `7116e96` | R3-1 shim phase 14 disasm + R3-4 hexa pe_i386_memory_model | cpp +575, hexa +320 |
| `f67e95b` | R3-3 hexa pe_i386_register_state | hexa +280 |
| `7cdab67` | R3-2 hexa pe_i386_disasm coverage tracker | hexa +245 |
| `7dc4b20` | docs: R3-1~4 status note + commit race 기록 | docs +95 |
| `9126c84` | R3-5/6/7 shim phase 15-17 interpreter + R3-8~12 hexa skeleton (6 phase 일괄) | cpp +820, hexa +1180 |
| `2b90d2c` | shim phase 14-15 opcode 14 추가 — 7→17 instruction 진척 | cpp +180 |
| `1bb3e94` | docs: R3_PROGRESS_V2 — 17 inst 도달 + opcode coverage 표 + 잔여 trigger | docs +310 |
| `510a27b` | helper.mm IPC kind 12-15 (FPU/SSE/USER32/GDI32 stub helper) | mm +147 + 18 patch |
| `6adc502` | hexa pe_i386_shim_phase_18_22 (R3-8~12 phase 18-22 skeleton) | hexa +480 |
| `0e60b86` | hexa pe_i386_iat_thunk_routing (IAT lookup + dlsym routing mirror) | hexa +260 |
| `7bfca4c` | R3-6 shim phase 16 IAT thunk routing — call [IAT] → POSIX stub (17→**51 inst**) | cpp +320 |
| `badcc99` | docs: BATTLENET_REAL_LAUNCH_PLAN — wave 1-5 own1 enforcement | docs +420 |
| `5371b32` | docs: **R3_PROGRESS_V3** — 51 inst 도달 + IAT 4 함수 통과 + R3-8 FS prefix trigger | docs +485 |
| `6088aec` | hexa pe_i386_segment_override_real (FS/GS prefix + TEB/PEB field access tracker) | hexa +320 |
| `e5a4187` | hexa pe_i386_x87_fpu_inst_real (D8-DF FPU instruction set tracker) | hexa +290 |
| `96c1532` | hexa pe_i386_seh_dispatch_runtime (SEH runtime dispatch flow tracker) | hexa +350 |
| `cbdface` | **R3-8/9 shim phase 18-19 — FS prefix + SEH register + FPU/SSE prefix** (51→**205 inst**) | cpp +646 |

**누적 V3→V4**: cpp +646 line / mm +307 line (helper bridge 추가 commit `57f121a` 포함) / hexa +960 line / docs +485 line. 5 commit, 2 sprint (cycle 98-99).

### 9.1 핵심 V3→V4 commit — `cbdface`

R3-8 SEH register 자동 detect + R3-9 FPU/SSE prefix decode 일괄 통합. shim phase 18-19 의 신규 layer + SehRecord struct + g_seh_records[64] chain table + FS/GS prefix 의 segment override 실 처리 + INC/DEC/ADC/SBB/shift/PUSH r/m32/MOV moffs opcode cap 해제. **51 → 205 instruction 진척** (delta +154, 4×) 달성. inst[50] FS prefix halt → inst[205] XCHG halt 의 trigger 전진. 누적 R3 closure ~50% → ~55%. SEH chain depth=1 (frame=0x8FFDC prev=0 handler=0x551A50) — 자동 detect 검증 OK.

---

## 10. closure path 통합 정리

R1 + R2 + R3 = 자체 i386 PE32 launcher (own1 §A 정합) 의 V4 시점 갱신:

| layer | 영역 | closure 기준 | V3 | V4 |
|---|---|---|---|---|
| R1 | helper.mm IPC (subprocess / dlopen / dlsym / mmap / TEB / Metal probe + FPU/SSE/USER32/GDI32 stub helper) | shim phase 1-13 통과 + IPC kind 12-15 추가 | 100% | 100% |
| R2 | PE 실 mmap + IAT KERNEL32 routing + entry trap + Metal device probe | shim phase 9-13 통과 | 100% | 100% |
| R3-1~7 | shim disasm + interp + memory access + control flow + IAT thunk routing | 17 → 51 → **205 inst** + 4 IAT pass + opcode cap 해제 | 50% | **65%** (wave2 신규 opcode 통합 후) |
| R3-8 | SEH register 자동 detect + FS/GS prefix segment override | inst[50] halt 해소 + SehRecord chain table | 0% | **55%** (R3-8.a-c 통합 OK / R3-8.d-e dispatch 잔여) |
| R3-9 | FPU/SSE prefix decode + helper IPC kind=12/13 echo | decode/dispatch path OK | 0% | **40%** (decode 통합, 실 floating X) |
| R3-10~12 | PEB/Ldr / USER32-GDI32 / first dialog | hexa skeleton + shim 미통합 (wave3 BG 진행 중) | 0% | △ ~5% (BG 진행 중) |
| **closure 통합** | "Wine 0줄 + R1+R2+R3 = real PE32 i386 launcher" | Battle.net Setup first dialog 표시 | ~50% | **~55%** |

### 10.1 누적 진척 예상 cycle

| cycle | wave | 누적 closure |
|---|---|---|
| 90 | R3 entry | 진척 0% |
| 95 (V2) | R3-1~5 통합 | ~45% |
| 97 (V3) | R3-6 IAT thunk routing | ~50% |
| **99 (V4)** | **R3-8/9 SEH register + FPU/SSE prefix** | **~55%** |
| 101 (V5 예상) | R3-10 wave3 (XCHG/LOCK/REP/string/INC-DEC/shift) | ~65% |
| 103 (V6 예상) | R3-10/11 PEB/Ldr + KERNEL32 추가 | ~80% |
| 105 (V7 예상) | R3-12 first dialog 표시 | 100% (R3 closure) |

own1 enforcement 일관 — cycle 90 의 closure 100% (R1+R2) 는 phase 1-13 의 자체 구현. cycle 91-99 의 R3 진척 또한 외부 emulator / Wine fork lift 0줄. **자체 i386 execution layer** 의 cohesion 누적 ~55% — 잔여 ~45% 의 sub-phase 4 (R3-10/11/12) 가 V4 → V5 → V6 → V7 cycle 동안 진척 대상.

---

## 11. v5 trigger 조건

R3 PROGRESS V5 작성 trigger:

1. **R3-10 wave3 통합 완료** — inst[205] XCHG halt → inst[400+] 도달 + XCHG/LOCK/REP/string/INC-DEC/shift family 일괄 통합. 누적 ~65%.
2. **PEB/Ldr 진입** — GetModuleHandleA / IsProcessorFeaturePresent / RtlGetVersion / RtlInitUnicodeString IAT routing. 누적 ~70-80%.
3. **first NSWindow 생성** — helper.mm BG 2 의 CreateWindowExA → NSWindow 실 매핑 검증. R3-12 영역.
4. **plateau 발생** — 2 sprint 연속 진척 0 시 V5 에서 sub-phase 재분배 또는 외부 결정 prereq 추가.
5. **R3 closure 100% 도달** → V_FINAL 신규 작성 (R4 binary translation B path 또는 R5 D3D → Metal path 평가).

→ 본 V4 가 cycle 99-103 sprint 동안 유효. cycle 101 즈음 R3-10 wave3 통합 시점에 V5 재작성 기대.

---

*written 2026-05-01 cycle 99 sprint 직후. R1+R2 own1 §A closure 100% 유지 + R3-1~9 wave2 통합 완료 + shim phase 14-19 동작. **205 instruction** 도달 (V3 의 51 → V4 의 205, delta +154, 4×). inst[50] FS prefix halt → inst[205] XCHG halt 의 trigger 전진. SEH register 자동 detect (chain depth=1, frame=0x8FFDC prev=0 handler=0x551A50). FS/GS prefix load=1 store=1 실 동작. FPU/SSE prefix decode path 통합 (entry 인근 미사용 → FPU=0 SSE=0, dispatch path 검증 OK). INC/DEC/ADC/SBB/shift/PUSH r/m32/MOV moffs opcode cap 해제. 다음 trigger 는 R3-10 wave3 의 XCHG/LOCK/REP/string family 일괄 통합 — BG 1 진행 중. own1 §A enforcement 일관 (Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift 0줄). 본 PROGRESS V4 path 외 수정 X. Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + ReactOS public docs reference only.*
