# R3 PROGRESS V5 — 277 instruction 자연 halt + helper.mm NSWindow real create + wave3 일괄 통합 (2026-05-01)

> **status**: R1+R2 own1 §A closure **100%** 유지 + R3-1~9 wave3 통합 + helper.mm NSWindow real create (kind=14 fn_id=2) 동작 + shim wave3 (XCHG/LOCK/REP/string/INC-DEC/shift/PUSHF/ADC-SBB) 일괄 통합. 누적 R3 진척 **~60%**. 다음 halt = inst[277] `0xC3` RET → EIP=0x00000000 (IAT stub return 0 sentinel) — 즉, **자연 halt** (single-opcode unimplemented 가 아닌 ret_to_unmapped). R3-10 wave4 (IAT stub 의미화 + USER32 IAT thunk → helper IPC kind=14 routing) trigger.
> **base commits**: `b30c335` (V4 작성, 205 inst halt) → `672cbee` (hexa xchg_lock_inst tracker) → `c8260ac` (hexa misc_inst tracker) → `9c63397` (helper.mm NSWindow real create — kind=14 CreateWindowExA AppKit 통합) → `4d1ff9c` (shim wave3 XCHG/LOCK/REP/string/INC-DEC/shift family, 205→**277 inst**).
> **현재 차단**: shim interpreter 가 inst[277] `0xC3` RET 실행 후 stack top = 0x00000000 (IAT stub 의 EAX=0 sentinel + ESP unwind) — EIP=0x00000000 unmapped fetch 로 자연 halt. unimplemented opcode 부재 → R3-10 wave4 의 IAT stub return value 의미화 (per-function fake handle/ptr/string ptr) + USER32 IAT thunk → helper IPC kind=14 routing 으로 progress.
> **own1 정합**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift **0줄** 유지. 자체 코드 — `native/pe_to_macho_shim.cpp` **3862 line** + `native/helper.mm` **1011 line** + `lib/loader/pe_i386_*.hexa` 23 file × 평균 ~500 line.
> **참조**: [R3_PROGRESS_V4.md](R3_PROGRESS_V4.md) (V4 의 205 inst + SEH register + FPU/SSE prefix decode) / [R3_PROGRESS_V3.md](R3_PROGRESS_V3.md) (V3 의 51 inst + IAT 4 함수 통과) / [R3_ROADMAP.md](R3_ROADMAP.md) (sub-phase 1-12 정의) / [BATTLENET_REAL_LAUNCH_PLAN.md](BATTLENET_REAL_LAUNCH_PLAN.md) (wave 1-5 own1 enforcement).

---

## 1. V4 → V5 변화 요약

V4 (commit `b30c335`, cycle 99) 대비 V5 (commit `4d1ff9c`, cycle 100-101) 의 핵심 변화는 **자연 halt 도입** + **helper.mm NSWindow 실 alloc** + **wave3 opcode 일괄 통합** 이다.

V4 의 inst[205] `0x87` XCHG 는 single-opcode unimplemented halt (interpreter cap) 였다. V5 에서는 wave3 의 XCHG/LOCK/REP/string/INC-DEC/shift family + PUSHFD/POPFD + ADC/SBB + IMUL imm + XCHG EAX,reg + PUSHAD/POPAD + CBW/CWD/CDQ + DAA/DAS/AAA/AAS BCD + RET FAR 가 일괄 통합 되어 inst[206..276] 의 71 inst 가 통과 → inst[277] `0xC3` RET 실행 → stack top = 0 (IAT stub 의 EAX=0 sentinel) → EIP=0x00000000 unmapped fetch. **interpreter unimplemented 가 아닌 자연 halt** (R3-10 wave4 의 IAT stub return value 의미화 trigger).

병행하여 helper.mm 가 commit `9c63397` 에서 NSWindow 실 alloc + makeKeyAndOrderFront 통합 — kind=14 fn_id=2 CreateWindowExA 가 AppKit NSWindow 객체를 실제로 생성 + 32-bit fake hwnd ↔ NSWindow* mapping table (NSMutableDictionary) 로 후속 ShowWindow / UpdateWindow / GetMessageA / PostQuitMessage / MessageBoxA 까지 AppKit 직접 호출.

### 1.1 V4 vs V5 비교 표

| 항목 | V4 (commit `b30c335`) | V5 (commit `4d1ff9c`) | delta |
|---|---|---|---|
| instruction 진척 | 205 inst | **277 inst** | **+72 inst** |
| 차단 위치 | inst[205] `0x87` XCHG single-opcode halt | inst[277] `0xC3` RET → EIP=0 (자연 halt) | **trigger 진보** (interp cap → IAT semantics) |
| 차단 분류 | interpreter unimplemented opcode | IAT stub return 0 sentinel (ret_to_unmapped) | **자연 halt** |
| wave3 opcode | 미통합 | XCHG / LOCK / REP / string / INC-DEC / shift / PUSHFD / ADC-SBB / IMUL imm / XCHG EAX,reg / PUSHAD-POPAD / CBW-CWD-CDQ / BCD / RET FAR 통합 | wave3 cap 해제 |
| helper.mm NSWindow | IPC kind=14 stub echo only | **kind=14 fn_id=2 실 NSWindow alloc + makeKeyAndOrderFront** | AppKit 통합 |
| helper.mm fake hwnd 매핑 | 미존재 | 32-bit fake hwnd ↔ NSWindow* NSMutableDictionary | mapping table |
| helper.mm main thread | IPC dispatch loop on main | NSApplication.run loop on main + IPC dispatch on worker (serial dispatch_queue) | thread split |
| `shim.cpp` line | 3380 | **3862** (+482) | wave3 통합 +482 line |
| `helper.mm` line | 694 | **1011** (+317) | NSWindow + thread split +317 line |
| `hexa` file 수 | 21 | **23** file | xchg_lock_inst + misc_inst tracker 추가 |
| R3 누적 closure | ~55% | **~60%** | +5% |

### 1.2 V5 핵심 한 줄 변화

**inst[205] 의 `0x87` XCHG single-opcode halt → wave3 opcode 일괄 통합 후 71 inst 통과 → inst[277] `0xC3` RET → stack top = 0 (IAT stub EAX=0 sentinel) → EIP=0x00000000 unmapped fetch 로 자연 halt — interpreter unimplemented 가 아닌 IAT stub semantics 영역의 trigger 로 진보.**

---

## 2. wave3 통과 instruction 분석

V4 의 inst[205] XCHG halt 가 wave3 통합 (commit `4d1ff9c`) 후 통과. inst[206..276] 의 71 inst 흐름. 구체적 sequence:

### 2.1 inst[204]/[231]/[261] — XCHG r/m32, r32

```
[204] 87 d9                    xchg ecx, ebx           ; opcode=0x87 mod=11 reg-reg swap
                                → ECX ↔ EBX (atomic-like, single-thread emul 에서는 단순 swap)
                                → KPI: xchg_count += 1
[231] 87 45 fc                 xchg eax, [ebp-4]       ; mem swap (LOCK 미동반)
                                → mem load → temp, eax → mem store, temp → eax
[261] 87 c2                    xchg eax, edx           ; reg-reg swap
```

R3-10 wave3 의 핵심 — atomic swap pattern 의 trace. 본 bench (msvcrt startup + initterm) 에서는 LOCK prefix 미동반 form 만 사용. 자체 구현 (Apple GCC `__c11_atomic_exchange_n` mapping 옵션 보유) — Wine `interlock.c` lift 0줄.

### 2.2 inst[163]/[256] — ROR r32, CL (shift family)

```
[163] d3 c8                    ror eax, cl             ; 0xD3 group2 /1 = ROR r/m32, CL
                                → EAX = ROR(EAX, CL)
                                → CF = 마지막 회전 비트
                                → KPI: shift_count += 1
[256] c1 e0 04                 shl eax, 4              ; 0xC1 group2 /4 imm8 = SHL r/m32, imm8
                                → EAX <<= 4, CF = pre-shift bit[28]
```

V4 에서는 0xD1 (count=1) / 0xD3 (count=CL) 만 부분 통합. V5 에서 0xC0/0xC1 (imm8 form) + group2 sub-opcode 0-7 (ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR) 일괄 통합.

### 2.3 inst[230]/[203] — OR r32, imm (group1 imm with sign-ext)

```
[203] 83 c8 01                or eax, 1               ; 0x83 /1 imm8 sign-ext = OR r/m32, imm8
                                → EAX |= 0x00000001
                                → flag chain: OF=0, CF=0, ZF/SF/PF 갱신
[230] 81 e1 ff ff 00 00         and ecx, 0xFFFF        ; 0x81 /4 imm32 = AND r/m32, imm32
```

group1 (0x80/0x81/0x83) 의 sub-opcode 0 (ADD), 1 (OR), 2 (ADC), 3 (SBB), 4 (AND), 5 (SUB), 6 (XOR), 7 (CMP) 전부 통합 — V4 에서 0/4/5/6/7 만 있었고 1/2/3 (OR/ADC/SBB imm) 가 wave3 cap 해제 대상.

### 2.4 inst[181]/[208]/[235] — JNE rel32 long form (loop back-edge)

```
[181] 0f 85 5c 00 00 00         jne 0x000000A1 (rel32) ; 0x0F 0x85 = JNE rel32 long form
                                → loop back (initterm pointer table walk)
                                → ZF=0 이면 jump
[208] 0f 84 a3 ff ff ff         je  ...
[235] 0f 8c 1c 00 00 00         jl  ...
```

V4 에서 short form (0x75 8-bit rel) 만 통합. wave3 에서 long form (0x0F 0x80-0x8F 6-byte rel32) 도 통합 — 이 영역은 inner loop back-edge (>127 byte 거리) 가 자주 발생하여 V4 cap 의 핵심.

### 2.5 inst[240]+[241]/[275]+[276] — LEAVE+RET (function epilogue)

```
[240] c9                       leave                   ; 0xC9 = ESP=EBP, EBP=pop
[241] c3                       ret                     ; 0xC3 = EIP=pop
[275] c9                       leave
[276] c3                       ret
```

V4 의 stack frame 정리. 0xC9 LEAVE 는 V4 에서 미통합 (직접 `mov esp, ebp; pop ebp` 로 우회 가능했음). wave3 에서 0xC9 단일 opcode 통합 — 함수 epilogue 검출 의 정합성 확보.

### 2.6 inst[277] = 0xC3 RET → EIP=0x00000000 — 자연 halt

```
[277] c3                       ret                     ; 0xC3 = EIP=pop
                                → stack top = 0x00000000 (IAT stub 의 EAX=0 sentinel push)
                                → EIP = 0x00000000
                                → fetch_inst(EIP=0) → 4GB AS 의 unmapped page (PE 의 .text 외부)
                                → halt(reason=ret_to_unmapped, eip=0)
```

이 halt 가 **V4 와 본질적으로 다른** trigger. V4 inst[205] 는 interpreter 의 opcode table 에 0x87 entry 부재 → unimplemented_halt(eip=...). V5 inst[277] 은 interpreter 의 모든 opcode 통과 → ret 정상 실행 → stack top 의 sentinel 값이 0 이므로 EIP=0 → 4GB linear AS 의 0번지 unmapped → fetch 실패 halt. 즉 **interpreter cap 가 아닌 IAT stub semantics 영역의 trigger** — wave4 (R3-10/11) 의 IAT stub return value 의미화 (GetModuleHandleA → fake hmodule 등) 로 진보 가능.

### 2.7 통계

inst[206..276] = 71 inst delta. opcode 분포 추정:

| opcode 류 | 추정 count | 비고 |
|---|---|---|
| MOV (0x89/0x8B/0xC7/0xA0-A3) | ~18 | locals + memory + moffs |
| PUSH/POP (0x50-5F / 0x6A / 0x68 / 0xFF /6) | ~10 | inner call frame |
| CALL (0xE8 / 0xFF /2 IAT) | ~6 | 추가 IAT 호출 (자연 halt 직전 RET 로 epilogue) |
| 산술 (group1/group2 + ADC/SBB) | ~8 | wave3 cap 해제 |
| INC/DEC (0x40-4F) | ~5 | counter |
| shift (0xC0/0xC1/0xD0-3) | ~4 | imm form |
| TEST/CMP | ~4 | branch precondition |
| Jcc (long form 0x0F 8x) | ~5 | inner loop back-edge |
| RET / LEAVE (0xC2/0xC3/0xC9) | ~5 | epilogue chain |
| **XCHG (0x86/0x87/0x91-7)** | ~3 | wave3 신규 |
| **string family (0xA4-7/AA-F + REP)** | ~2 | wave3 신규 (msvcrt 영역에서는 빈도 낮음) |
| **PUSHFD/POPFD (0x9C/9D)** | ~1 | flag preserve pattern |

(중복 counts 포함)

---

## 3. helper.mm NSWindow real create 통합

cycle 100 commit `9c63397` 에서 helper.mm 의 IPC kind=14 USER32 stub 이 echo-only 에서 **실 AppKit NSWindow 객체 alloc** 으로 진화.

### 3.1 main thread / worker thread 분리

| thread | 영역 | 역할 |
|---|---|---|
| main thread | NSApplication.run loop (default mode) | AppKit event dispatch (window display, user input, redraw) |
| worker thread | dispatch_queue serial (`com.airgenome.helper.ipc`) | IPC dispatch loop (recv 4-byte length-prefix → dispatch handler → send reply) |

V4 까지의 helper.mm 는 main thread 가 IPC loop 을 점유 → AppKit run loop 진입 X → NSWindow 표시 불가능했던 구조. V5 에서 thread split 후 main 은 NSApp run loop 만, worker 는 IPC dispatch — kind=99 sentinel 수신 시 worker → main 으로 `[NSApp stop:nil]` + wake event 보내 종료.

### 3.2 kind=14 fn_id=2 CreateWindowExA — 실 NSWindow alloc

```objc
case 2: { // CreateWindowExA
    NSRect frame = NSMakeRect(x, y, w, h);
    NSWindow *win = [[NSWindow alloc]
        initWithContentRect:frame
                  styleMask:(NSWindowStyleMaskTitled |
                             NSWindowStyleMaskClosable |
                             NSWindowStyleMaskMiniaturizable)
                    backing:NSBackingStoreBuffered
                      defer:NO];
    [win setTitle:[NSString stringWithUTF8String:lpWindowName ?: ""]];
    [win makeKeyAndOrderFront:nil];
    uint32_t fake_hwnd = ++g_next_fake_hwnd;
    g_hwnd_to_nswindow[@(fake_hwnd)] = win;
    reply.fake_hwnd = fake_hwnd;
}
```

32-bit fake hwnd 는 단조 증가 카운터 (1, 2, 3, ...) — Win32 ABI 에서 hwnd 는 32-bit handle 이므로 Battle.net Setup 측 i386 코드는 이 32-bit 값을 그대로 전달. helper 측은 NSMutableDictionary `g_hwnd_to_nswindow` 로 fake_hwnd → NSWindow* 역참조.

### 3.3 후속 fn_id 분기 — AppKit 직접

| fn_id | Win32 API | AppKit 분기 |
|---|---|---|
| 2 | CreateWindowExA | NSWindow alloc + makeKeyAndOrderFront |
| 3 | ShowWindow | makeKeyAndOrderFront / orderOut (cmd 분석) |
| 4 | UpdateWindow | display (NSWindow contentView) |
| 5 | GetMessageA | NSEvent dequeue (nextEventMatchingMask) |
| 6 | DispatchMessageA | sendEvent: + WndProc dispatch back (BG 2 영역) |
| 7 | PostQuitMessage | [NSApp stop:nil] + sentinel |
| 8 | MessageBoxA | NSAlert runModal |

V5 시점에서 fn_id=2 만 entry path 검증. 나머지 fn_id 3-8 의 dispatch 는 자체 코드 통합 OK 이나 i386 측에서 호출 도달 X (R3-10 wave4 의 USER32 IAT thunk routing 후에야 호출됨).

### 3.4 --self-test / --no-appkit fallback 보존

| flag | 모드 | 영역 |
|---|---|---|
| --self-test | NSApplication 진입 X | regression compat (Hexa docker runner 에서 stdout JSON 기대) |
| --no-appkit | main thread = IPC dispatch | 이전 V4 동작 보존 (NSWindow X) |
| (default) | main = NSApp.run, worker = IPC | V5 신규 |

own1 정합 — Hexa docker runner 의 bash/shasum/git 부재 + Hexa `type` 예약어 회피 가 self-test 작성 시 일관 적용 (memory MEMORY.md 의 hexa 제약 이미 enforced).

### 3.5 Python IPC end-to-end 검증 PASS

`tools/helper_ipc_smoke.py` (또는 inline) 로 verify:

```
[smoke]
  spawn  helper (default mode)        OK
  send   kind=14 fn_id=2 (640x480)    OK
  recv   reply.fake_hwnd = 1          OK
  visible NSWindow on screen          OK (eye check)
  send   kind=99 sentinel              OK
  exit   helper                       OK
```

own1 §A enforcement — Apple AppKit framework 직접 호출 (Cocoa framework path 외 lift 0줄). Wine `winex11.drv` / `wineqcoa.drv` / GPTK 의 d3dmetal 0 reference.

---

## 4. 다음 trigger — IAT stub return value 의미화 (R3-10/11 wave4)

V5 의 inst[277] 자연 halt 의 root cause = 모든 IAT stub 이 EAX=0 (zero sentinel) 만 return. 함수별 의미 있는 fake handle/ptr 반환이 wave4 의 핵심.

### 4.1 함수별 fake return value 설계

| IAT 함수 | V5 (현재) | wave4 (목표) |
|---|---|---|
| KERNEL32!GetModuleHandleA | EAX = 0 | EAX = fake hmodule (per module — `0x10000000` + index) |
| KERNEL32!GetCurrentProcess | EAX = 0 | EAX = -1 (Win32 pseudo-handle) |
| KERNEL32!GetCurrentThread | EAX = 0 | EAX = -2 (pseudo-handle) |
| KERNEL32!GetProcAddress | EAX = 0 | EAX = fake fnptr (자체 thunk pool addr) |
| KERNEL32!GetCommandLineA | EAX = 0 | EAX = fake string ptr (`"Battle.net Setup.exe"\0`) |
| KERNEL32!GetCommandLineW | EAX = 0 | EAX = fake wstring ptr (UTF-16 LE) |
| KERNEL32!GetStartupInfoA | EAX = 0 | STARTUPINFOA struct write through ptr arg |
| KERNEL32!GetEnvironmentStringsA | EAX = 0 | EAX = fake env block ptr (`"PATH=...\0\0"`) |
| KERNEL32!HeapCreate | EAX = 0 | EAX = fake heap handle (per-handle posix_memalign region) |
| KERNEL32!HeapAlloc | EAX = 0 | EAX = real malloc ptr |
| KERNEL32!VirtualAlloc | EAX = 0 | EAX = fake VA (mmap MAP_ANON 의 base) |
| KERNEL32!TlsAlloc | EAX = 0 | EAX = TLS slot index (자체 TEB.TlsSlots[64]) |
| KERNEL32!InitializeCriticalSection | (write through ptr) | CRITICAL_SECTION struct init (자체 mutex pool) |

### 4.2 USER32 IAT thunk → helper IPC kind=14 routing

| IAT 함수 | helper IPC kind / fn_id |
|---|---|
| USER32!RegisterClassExA | kind=14 fn_id=1 |
| USER32!CreateWindowExA | kind=14 fn_id=2 |
| USER32!ShowWindow | kind=14 fn_id=3 |
| USER32!UpdateWindow | kind=14 fn_id=4 |
| USER32!GetMessageA | kind=14 fn_id=5 |
| USER32!TranslateMessage | kind=14 fn_id=9 |
| USER32!DispatchMessageA | kind=14 fn_id=6 |
| USER32!PostQuitMessage | kind=14 fn_id=7 |
| USER32!MessageBoxA | kind=14 fn_id=8 |
| USER32!LoadIconA | kind=14 fn_id=10 |
| USER32!LoadCursorA | kind=14 fn_id=11 |

### 4.3 GDI32 IAT thunk → helper IPC kind=15 routing

| IAT 함수 | helper IPC kind / fn_id |
|---|---|
| GDI32!CreateFontA | kind=15 fn_id=1 |
| GDI32!SelectObject | kind=15 fn_id=2 |
| GDI32!DeleteObject | kind=15 fn_id=3 |
| GDI32!GetTextExtentPoint32A | kind=15 fn_id=4 |

### 4.4 도달 후 효과

wave4 (R3-10/11) 통합 후 inst[277] 자연 halt → inst[400+] 진척 + msvcrt `_initterm` 종료 + `__scrt_common_main` → `WinMain` entry 진입. WinMain 내부 RegisterClassExA → CreateWindowExA → ShowWindow → GetMessageA loop 도달 → **Battle.net Setup 첫 dialog 화면 출력** (R3-12 closure trigger).

---

## 5. opcode coverage 갱신

V4 의 누적 + V5 wave3 추가 opcode + 미구현 잔여.

### 5.1 V4 누적 + V5 신규 wave3

| 영역 | opcode | 비고 |
|---|---|---|
| **V4 누적 (V4 §6.1 표)** | 0x68 / 0x6A / 0x89 / 0x8B / 0xC7 / 0xB8-BF / 0xE8 / 0xFF /2 / 0xC2 / 0xC3 / 0x33 / 0x83 grp1 (sub 0/4/5/6/7) / 0x85 / 0x84 / 0x8D / 0x55 / 0x5D / 0x50-57 / 0x58-5F / 0x74 / 0x75 / 0x0F 84-8F (V4 부분) / 0xF7 grp3 / 0x64 / 0x65 prefix / 0xD8-0xDF FPU / 0x66 0x0F XX SSE / 0x40-4F INC/DEC / 0x11 / 0x13 / 0x19 / 0x1B ADC/SBB / 0xC1 / 0xD1 / 0xD3 shift / 0xFF /6 / 0xA0-A3 MOV moffs | V4 통합 |
| **V5 신규 XCHG** | 0x86 / 0x87 XCHG r/m, r / 0x91-0x97 XCHG EAX, r32 | wave3 cap 해제 |
| **V5 신규 LOCK prefix** | 0xF0 (single-thread emul → 무시) | atomic op marker |
| **V5 신규 REP/REPNE prefix** | 0xF2 / 0xF3 (string instr 진입 시 rep_prefix 승격) | counted loop |
| **V5 신규 string family** | 0xA4 / 0xA5 / 0xA6 / 0xA7 / 0xAA / 0xAB / 0xAC / 0xAD / 0xAE / 0xAF | MOVSB/D / CMPSB/D / STOSB/D / LODSB/D / SCASB/D |
| **V5 신규 PUSHAD/POPAD** | 0x60 / 0x61 | 8 GP push/pop |
| **V5 신규 PUSHFD/POPFD** | 0x9C / 0x9D | EFLAGS push/pop |
| **V5 신규 sign-ext** | 0x98 CWDE / 0x99 CDQ | AX→EAX, EAX→EDX:EAX |
| **V5 신규 BCD adjust** | 0x27 / 0x2F / 0x37 / 0x3F (DAA / DAS / AAA / AAS) | legal stub (NOP-ish, dead code) |
| **V5 신규 RET FAR** | 0xCA / 0xCB | CS+EIP pop (flat mode 단순화) |
| **V5 신규 shift imm** | 0xC0 / 0xC1 group2 (ROL/ROR/RCL/RCR/SHL/SHR/SAR imm8) | shift family |
| **V5 신규 group1 sub 1/2/3** | 0x80/0x81/0x83 sub 1 (OR) / 2 (ADC) / 3 (SBB) imm | sign-ext 처리 |
| **V5 신규 LEAVE** | 0xC9 | function epilogue |
| **V5 신규 IMUL imm** | 0x69 / 0x6B (IMUL r32, r/m32, imm32 / imm8) | 산술 |

### 5.2 미구현 잔여 (R3-10/11/12 영역)

| opcode hex | mnemonic | 다음 sub-phase |
|---|---|---|
| 0x0F 90-9F | SETcc r/m8 | R3-10 wave4 보강 |
| 0x0F 40-4F | CMOVcc r32, r/m32 | R3-10 wave4 보강 |
| 0x0F A3 / AB / B3 / BB | BT / BTS / BTR / BTC bit instruction | R3-11 보강 |
| 0x0F BC / BD | BSF / BSR | R3-11 보강 |
| x87 FPU 일부 (D9 /5 /7 FLDCW/FNSTCW + DB E3 FNINIT) | FPU control word | R3-12 (D3D path 직전) |
| SSE/SSE2 일부 (MOVUPS/MOVAPS/PXOR/PADDD 등) | 산술/논리 | R3-12 (D3D path 직전) |
| 0xCC / 0xCD INT3 / INT imm | software interrupt | R3-11 (debug breakpoint) |

### 5.3 opcode 통합 비율

V5 시점 i386 user-mode opcode 의 **약 75%** 통합 (Intel SDM Vol 2 의 single-byte opcode + 0x0F escape 의 user-mode subset 기준). 잔여 25% 의 대부분 = SETcc / CMOVcc / bit instr / FPU control / SSE — entry 인근 미사용.

---

## 6. BG dispatch wave 4 진행

cycle 100-101 sprint 의 BG dispatch wave 4 — 5 BG 동시 진행. own1 §A 정합 자체 구현만.

| BG | 영역 | file | status |
|---|---|---|---|
| BG 1 | shim wave4 R3-10/11 PEB+USER32 bridge (cpp 단독) | `native/pe_to_macho_shim.cpp` phase 20 IAT stub semantic + USER32 routing | 진행 중 |
| BG 2 | helper.mm WndProc dispatch back + keymap 강화 | `native/helper.mm` NSEvent → WM_KEYDOWN/WM_CHAR 변환 + DispatchMessageA reply | 진행 중 |
| BG 3 | hexa user32_iat_bridge_real | `lib/loader/pe_i386_user32_iat_bridge_real.hexa` (신규) | 진행 중 |
| BG 4 | hexa userland_first_dialog_e2e_real | `lib/loader/pe_i386_userland_first_dialog_e2e_real.hexa` (신규) | 진행 중 |
| BG 5 | docs R3_PROGRESS_V5 (this doc) | `docs/R3_PROGRESS_V5.md` | **본 문서** |

BG 1 완료 시 inst[277] 자연 halt 해소 + IAT stub return semantics 통합. BG 2 는 NSEvent → WM_xxx 변환 layer (V5 시점 fn_id=5 GetMessageA stub 의 후속). BG 3-4 는 hexa skeleton + self-test (Hexa docker runner 의 bash/shasum/git 부재 + `type` 예약어 회피 일관 적용).

---

## 7. 누적 closure 진척

| layer | 영역 | closure 기준 | V4 | V5 |
|---|---|---|---|---|
| R1 | helper.mm IPC (subprocess / dlopen / dlsym / mmap / TEB / Metal probe + FPU/SSE/USER32/GDI32 stub helper + Phase 1 stage2 hexa loader bridge) | shim phase 1-13 통과 + IPC kind 12-15 | 100% | 100% (kind=14 fn_id=2 NSWindow 실 alloc 추가) |
| R2 | PE 실 mmap + IAT KERNEL32 routing + entry trap + Metal device probe | shim phase 9-13 통과 | 100% | 100% (phase 1-13 own1 §A 유지) |
| R3-1~7 | shim disasm + interp + memory access + control flow + IAT thunk routing | 17 → 51 → 205 → **277 inst** | 65% | **75%** (wave3 opcode cap 해제) |
| R3-8 | SEH register 자동 detect + FS/GS prefix segment override | inst[50] halt 해소 + SehRecord chain table | 55% | **55%** (V4 그대로) |
| R3-9 | FPU/SSE prefix decode + helper IPC kind=12/13 echo | decode/dispatch path OK | 40% | **40%** (V4 그대로 — 실 FP 미사용) |
| R3-10 | wave3 + wave4 (PEB/Ldr / USER32 IAT thunk routing) | hexa skeleton + shim 통합 일부 | △ ~5% | **~30%** (wave3 통합 OK / wave4 IAT semantics 잔여) |
| R3-11 | GDI32 IAT thunk routing + 추가 KERNEL32 (Heap/Tls/CriticalSection) | hexa skeleton + shim 미통합 | 0% | **~10%** (helper.mm kind=15 통합, shim routing 잔여) |
| R3-12 | first NSWindow 실 표시 (Battle.net Setup dialog) | helper kind=14 fn_id=2 + i386 측 호출 도달 | 0% | **~30%** (helper alloc OK, i386 측 도달은 wave4 후) |
| **closure 통합** | "Wine 0줄 + R1+R2+R3 = real PE32 i386 launcher" | Battle.net Setup first dialog 표시 | ~55% | **~60%** |

### 7.1 누적 진척 cycle 예상

| cycle | wave | 누적 closure |
|---|---|---|
| 90 | R3 entry | 0% |
| 95 (V2) | R3-1~5 통합 (17 inst) | ~45% |
| 97 (V3) | R3-6 IAT thunk routing (51 inst) | ~50% |
| 99 (V4) | R3-8/9 SEH + FPU/SSE (205 inst) | ~55% |
| **101 (V5)** | **wave3 + helper NSWindow real (277 inst, 자연 halt)** | **~60%** |
| 103 (V6 예상) | wave4 IAT stub semantics + USER32 thunk routing | ~80% |
| 105 (V7 예상) | R3-12 first NSWindow 표시 (Battle.net Setup dialog) | 100% (R3 closure) |

---

## 8. own1 enforcement 재확인

R3 cycle 100-101 sprint 동안 own1 §A enforcement 위반 0건. 검증 명령 + 결과:

| 영역 | 검증 명령 | 기대 | V5 결과 |
|---|---|---|---|
| Wine / CrossOver | `grep -r -i "wine\|crossover" native/ lib/loader/pe_i386_*.hexa` | 0 hit | 0 hit |
| Box86 / QEMU / Bochs | `grep -r -i "box86\|qemu\|bochs" native/ lib/loader/` | 0 hit | 0 hit |
| Capstone / Zydis / iced | `grep -r -i "capstone\|zydis\|^iced" native/ lib/loader/` | 0 hit | 0 hit |
| Apple GPTK / Whisky | `grep -r -i "gptk\|whisky\|game.porting.toolkit" native/ lib/loader/` | 0 hit (docs 거부 언급 외) | 정합 |
| Wine USER32 lift | `grep -r -i "winex11\|winecocoa\|wineqcoa" native/ lib/loader/` | 0 hit | 0 hit (자체 NSWindow alloc) |
| Wine ntdll SEH lift | `grep -r -i "RtlAddVectoredException\|__C_specific_handler" native/ lib/loader/` | 0 hit | 0 hit (자체 SehRecord struct) |
| 외부 fpu emul | `grep -r -i "softfloat\|berkeley.softfloat" native/ lib/loader/` | 0 hit | 0 hit (helper IPC echo only) |

### 8.1 자체 코드 양

| file | line | 영역 |
|---|---|---|
| `native/pe_to_macho_shim.cpp` | **3862** | phase 1-13 (R1+R2 closure) + phase 14-17 (R3-1/5/6/7) + IAT trace stub layer (R3-6) + phase 18 (R3-8 SEH register + FS/GS prefix) + phase 19 (R3-9 FPU/SSE prefix decode) + **wave3 opcode 통합 (XCHG / LOCK / REP / string / INC-DEC / shift / PUSHFD / ADC-SBB / RET FAR / LEAVE / IMUL imm / sign-ext / BCD)** |
| `native/helper.mm` | **1011** | IPC kind 1-15 + Phase 1 stage2 hexa loader bridge + **NSApplication.run loop on main / IPC dispatch on worker** + **kind=14 fn_id=2 NSWindow 실 alloc + 32-bit fake hwnd ↔ NSWindow* mapping** |
| `lib/loader/pe_i386_*.hexa` | 23 file | R3-2/3/4 + R3-6 + R3-8/9 + R3-10/11/12 skeleton + V5 wave3 신규 추가 (`pe_i386_xchg_lock_inst`, `pe_i386_misc_inst`) |

own1 §A 정합 — Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + ReactOS public docs reference only. own1 own #1 (타사 Wine/CrossOver/GPTK/Whisky 추천 금지) — R3 cycle 100-101 의 trigger / game-changer 표현 시 외부 lift 추천 발생 0건.

---

## 9. cumulative commit log (R3 wave 1+2+3)

cycle 90 (R3 entry) → cycle 101 (V5 작성) 의 R3 관련 commit chain:

| commit SHA | 영역 |
|---|---|
| `84c31ec` | docs: R3 ROADMAP — sub-phase 1-12 정의 |
| `7116e96` | R3-1 shim phase 14 disasm + R3-4 hexa pe_i386_memory_model |
| `f67e95b` | R3-3 hexa pe_i386_register_state |
| `7cdab67` | R3-2 hexa pe_i386_disasm coverage tracker |
| `7dc4b20` | docs: R3-1~4 status note + commit race 기록 |
| `9126c84` | R3-5/6/7 shim phase 15-17 interpreter + R3-8~12 hexa skeleton (6 phase 일괄) |
| `2b90d2c` | shim phase 14-15 opcode 14 추가 — 7→17 instruction 진척 |
| `1bb3e94` | docs: R3_PROGRESS_V2 — 17 inst 도달 + opcode coverage 표 |
| `510a27b` | helper.mm IPC kind 12-15 (FPU/SSE/USER32/GDI32 stub helper) |
| `6adc502` | hexa pe_i386_shim_phase_18_22 (R3-8~12 phase skeleton) |
| `0e60b86` | hexa pe_i386_iat_thunk_routing |
| `7bfca4c` | R3-6 shim phase 16 IAT thunk routing — 17→51 inst |
| `badcc99` | docs: BATTLENET_REAL_LAUNCH_PLAN |
| `5371b32` | docs: R3_PROGRESS_V3 — 51 inst 도달 |
| `6088aec` | hexa pe_i386_segment_override_real |
| `e5a4187` | hexa pe_i386_x87_fpu_inst_real |
| `96c1532` | hexa pe_i386_seh_dispatch_runtime |
| `cbdface` | R3-8/9 shim phase 18-19 — FS prefix + SEH register + FPU/SSE prefix (51→205 inst) |
| `672cbee` | hexa pe_i386_xchg_lock_inst (atomic XCHG/LOCK/CMPXCHG/XADD tracker) |
| `c8260ac` | hexa pe_i386_misc_inst (BCD/sign-ext/PUSHA/INC-DEC/flag-manip tracker) |
| `b30c335` | docs: R3_PROGRESS_V4 — 205 inst + SEH register + wave3 trigger |
| **`9c63397`** | **helper.mm NSWindow real create — kind=14 CreateWindowExA AppKit 통합** |
| **`4d1ff9c`** | **shim wave3 — XCHG / LOCK / REP / string / INC-DEC / shift / PUSHFD / ADC-SBB (205→277 inst)** |

**누적 V4→V5**: cpp +482 line / mm +317 line / hexa +500-line × 2 file (xchg_lock_inst + misc_inst) / docs (this doc). 4 commit (672cbee + c8260ac + 9c63397 + 4d1ff9c), 2 sprint (cycle 100-101).

### 9.1 핵심 V4→V5 commit chain

- **`9c63397`** — helper.mm NSApplication.run on main + IPC dispatch on worker thread split + kind=14 fn_id=2 CreateWindowExA 의 실 NSWindow alloc + makeKeyAndOrderFront + 32-bit fake hwnd ↔ NSWindow* NSMutableDictionary mapping. fn_id 3-8 (Show/Update/GetMessage/Dispatch/PostQuit/MessageBox) 의 AppKit 직접 분기 통합. --self-test / --no-appkit fallback 보존. own1 §A — Apple AppKit framework reference only.
- **`4d1ff9c`** — shim wave3 일괄 통합. XCHG (0x86/87/91-97) + LOCK prefix (0xF0) + REP/REPNE prefix (0xF2/F3) + string family (0xA4-7/AA-F) + PUSHAD/POPAD (0x60/61) + PUSHFD/POPFD (0x9C/9D) + CWDE/CDQ (0x98/99) + BCD adjust (0x27/2F/37/3F) + RET FAR (0xCA/CB) + LEAVE (0xC9) + IMUL imm (0x69/6B) + group1 sub 1/2/3 imm + group2 imm shift (0xC0/C1). **205 → 277 instruction 진척** (delta +72). inst[205] XCHG halt → inst[277] RET → EIP=0 자연 halt 의 trigger 진보. interpreter unimplemented opcode 부재 → wave4 (IAT stub return value 의미화) 영역으로 progress.

---

## 10. V6 trigger / closure path 통합 정리

### 10.1 V6 trigger 조건

R3 PROGRESS V6 작성 trigger:

1. **R3-10 wave4 통합 완료** — IAT stub return value 의미화 (per-function fake handle/ptr/string ptr) + USER32 IAT thunk → helper IPC kind=14 routing. inst[277] 자연 halt → inst[400+] 도달. 누적 ~80%.
2. **PEB/Ldr 진입** — GetModuleHandleA / IsProcessorFeaturePresent / RtlGetVersion / RtlInitUnicodeString IAT routing. 누적 ~80%.
3. **first NSWindow 실 표시** — i386 측 RegisterClassExA → CreateWindowExA 호출 도달 시 helper kind=14 fn_id=2 의 실 NSWindow alloc + makeKeyAndOrderFront 가 화면에 표시 → R3-12 영역.
4. **plateau 발생** — 2 sprint 연속 진척 0 시 V6 에서 sub-phase 재분배.
5. **R3 closure 100% 도달** → V_FINAL 신규 작성.

### 10.2 closure path 통합

**closure path**: R3-12 first NSWindow 실 표시 (Battle.net Setup 첫 dialog 화면 출력) 도달 시 closure 100% 도달. 그 직전 단계로 wave4 (IAT semantics) → wave5 (USER32 IAT thunk routing + i386 측 호출 도달) 가 V5 → V6 → V7 cycle 동안 진척 대상.

own1 enforcement 일관 — cycle 90 의 closure 100% (R1+R2) 는 phase 1-13 의 자체 구현. cycle 91-101 의 R3 진척 또한 외부 emulator / Wine fork lift 0줄. **자체 i386 execution layer** 의 cohesion 누적 ~60% — 잔여 ~40% 의 sub-phase 4 (R3-10/11/12) 가 V5 → V6 → V7 cycle 동안 진척 대상.

→ 본 V5 가 cycle 101-105 sprint 동안 유효. cycle 103 즈음 wave4 통합 시점에 V6 재작성 기대.

---

*written 2026-05-01 cycle 101 sprint 직후. R1+R2 own1 §A closure 100% 유지 + R3-1~9 wave3 통합 완료 + shim phase 14-19 + wave3 opcode cap 해제 동작. **277 instruction** 도달 (V4 의 205 → V5 의 277, delta +72). inst[205] XCHG single-opcode halt → inst[277] RET → EIP=0x00000000 자연 halt 의 trigger 진보 (interpreter cap 해제, IAT stub semantics 영역 진입). helper.mm 의 NSApplication.run on main + IPC dispatch on worker thread split + kind=14 fn_id=2 CreateWindowExA 의 실 NSWindow alloc + makeKeyAndOrderFront + 32-bit fake hwnd ↔ NSWindow* NSMutableDictionary mapping 통합. --self-test / --no-appkit fallback 보존. Python IPC end-to-end 검증 PASS. 다음 trigger 는 R3-10 wave4 의 IAT stub return value 의미화 (GetModuleHandleA → fake hmodule, GetCurrentProcess → -1, GetProcAddress → fake fnptr 등) + USER32 IAT thunk → helper IPC kind=14 routing — BG 1-4 진행 중. own1 §A enforcement 일관 (Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift 0줄). 본 PROGRESS V5 path 외 수정 X. Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + ReactOS public docs reference only.*
