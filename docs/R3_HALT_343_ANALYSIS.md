# R3 wave 5 halt 분석 — inst[343] ret_to_unmapped

**작성**: 2026-05-02 cycle 102+ · base commit `52502ca`

---

## 1. 현상

```
inst[330] 0x00558A1F: 74 15              je rel8=0x15  (fall-through)
inst[331] 0x00558A21: ff 75 10           push [ebp+0x10]
inst[332] 0x00558A24: 8b ce              mov ecx, esi
inst[333] 0x00558A26: ff 75 0c           push [ebp+0xC]
inst[334] 0x00558A29: ff 75 08           push [ebp+0x8]
inst[335] 0x00558A2C: ff 15 0c a7 6c 00  call [0x6CA70C]  ← IAT thunk (KERNEL32 unknown fn)
inst[336] 0x00406DE0: c3                 ret              ← thunk inner ret
inst[337] 0x00558A32: ff d6              call esi         ← esi = 0x40000000 (fake_proc_stub)
inst[338] 0x40000000: c3                 ret              ← fake_proc_stub immediate return
inst[339] 0x00558A34: eb 0c              jmp rel8=0xC
inst[340] 0x00558A42: 5e                 pop esi
inst[341] 0x00558A43: 5d                 pop ebp
inst[342] 0x00558A44: c3                 ret              ← stack [esp] = 0x00000000
[halt] inst[343] reason=ret_to_unmapped
```

KERNEL32 stub 6/6 통과:
- GetSystemTimeAsFileTime / GetCurrentThreadId / GetCurrentProcessId / QueryPerformanceCounter / LoadLibraryExW (→ 0x10000000 fake hmodule) / GetProcAddress (→ **0x40000000 fake_proc_stub**)

USER32/GDI32 IPC bridge 호출 **0건** (CRT init 미진입 → WinMain 미도달 → RegisterClassExA 미호출).

---

## 2. 본질 원인

### 2.1 fake_proc_stub 의 calling convention 문제

GetProcAddress(hk32, "FunctionName") → shim 이 fake addr `0x40000000` 반환.
caller 가 stdcall 함수 호출 시: `push args; call esi` (esi=0x40000000) 후 callee 가 args pop
하고 ret 해야 함. 그러나 0x40000000 위치는 단순 `c3 ret` 1-byte:
- caller stack 에 args 는 아직 남아 있음 (push 4번 = 16 byte)
- ret 은 그 자리의 첫 4-byte 를 EIP 로 해석 (= push 한 첫 arg [ebp+0x8] 값)
- 후속 inst 가 stack 의 args 를 pop esi/ebp 로 처리 → ESI/EBP 값이 misaligned

inst[342] 의 `ret` 가 stack [esp]=0 을 RA 로 읽음 — 깊은 stack frame 의 outer 함수 가
caller 인데 그 RA 가 0 으로 corruption.

### 2.2 진짜 fix path — GetProcAddress dispatch back

shim 이 GetProcAddress(hk32, fn_name) 받으면:
1. fn_name 으로 KERNEL32/MSVCR/USER32/GDI32 dispatch table 검색
2. unique fake addr 발급 (0x40001000 + counter)
3. g_dispatch_addr_table 에 (addr, dll, fn_name) 등록
4. phase 17 의 `call esi` indirect call 시:
   - EIP 가 dispatch_addr_table 안에 있으면 → 해당 dll/fn 의 stub_dispatch 호출
   - 그 stub 이 args 받고 fake EAX 채우고 stdcall arg_count_pop 만큼 ESP 정리 후 EIP=ret_addr

이는 wave 4 의 IAT thunk routing 과 같은 패턴 — IAT entry 가 아닌 GetProcAddress
결과 addr 도 동일 dispatch path.

### 2.3 차선 fix — fake_proc_stub array

각 함수별 4-byte stub: `83 c4 NN c3` = `add esp, NN; ret`
- NN = arg_count × 4 (stdcall callee-pop simulation)
- 메모리에 미리 N × 4-byte stub array mmap
- GetProcAddress 가 함수별 unique offset 반환

---

## 3. wave 6 작업 정의

목표: GetProcAddress 결과 호출 후 정상 stack 정리 → CRT init 통과 → WinMain 진입 →
USER32 RegisterClassExA / CreateWindowExA → helper IPC kind=14 → **첫 NSWindow 실
표시**.

### 3.1 shim 측 (BG 1)

`native/pe_to_macho_shim.cpp`:
1. 신규 struct `DispatchAddrEntry { uint32_t addr; const char* dll; const char* fn; uint8_t arg_count; }`
2. 신규 글로벌 `g_dispatch_addr_table[256]` + counter
3. `GetProcAddress` stub 강화: fn_name lookup → 신규 entry 등록 → unique fake addr return
4. phase 17 의 `0xFF /2 CALL r/m32` mod==11 (call reg) 처리에서:
   - target addr 이 g_dispatch_addr_table 에 있으면 → dispatch (KERNEL32/MSVCR/USER32/GDI32 stub_dispatch 호출)
   - args 는 esp_at_args 에서 read
   - fake EAX set + ESP += arg_count × 4 (stdcall) + EIP = ret_addr

### 3.2 hexa 측 (BG 2)

`lib/loader/pe_i386_get_proc_address_dispatch_real.hexa`:
- record_dispatch_entry / record_indirect_call / record_arg_unwind / record_routed_to_kind
- self-test ≥ 12 record + 14 emit

### 3.3 docs (BG 3)

`docs/R3_PROGRESS_V7.md`:
- inst 343 → 600+ (예상) 진척
- GetProcAddress dispatch back 통합 후 USER32 IPC bridge 첫 호출 도달 여부

---

## 4. 통합 후 expected inst sequence

CRT init (`__scrt_common_main_seh`) 통과:
- `_initterm_e` / `_initterm` 의 init array walk skip
- `__scrt_acquire_startup_lock` 통과
- `_get_initial_narrow_environment` → fake env block ptr
- `_get_narrow_winmain_command_line` → fake cmdline
- WinMain(hInst=0x10000000, NULL, lpCmdLine, nShowCmd=10) **진입**

WinMain body:
- WNDCLASSEX struct fill
- RegisterClassExA → IAT thunk → helper IPC kind=14 fn_id=1 → atom return
- CreateWindowExA → IAT thunk → helper IPC kind=14 fn_id=2 → **실 NSWindow alloc** ← 마일스톤
- ShowWindow → fn_id=3 → makeKeyAndOrderFront
- UpdateWindow → fn_id=4 → displayIfNeeded
- message loop GetMessageA → fn_id=5

**예상 도달**: inst 1000-2000+ + 첫 NSWindow 실 표시.

---

## 5. own1 정합 enforcement

- 모든 dispatch table 자체 작성 (Microsoft API doc reference 만)
- Wine implementation 0줄
- Apple SDK only (AppKit / Foundation) for helper-side window creation
- KERNEL32/MSVCR/USER32/GDI32 stub 모두 자체 작성 fake return value

---

## 6. 자료

- base commit: `52502ca` (shim wave5 — CRT runtime init dispatch wired)
- verbose trace log: `./entry launch tool/bench/Battle.net-Setup.exe --verbose`
- shim cpp: `native/pe_to_macho_shim.cpp` (현 4900+ line)
- helper.mm: `native/helper.mm` (현 1300+ line, NSWindow real create + WndProc invoke OK)
