# WINE_USER32_FULL_AUDIT — Wine user32 + gdi32 + winex11.drv 전수조사

**작성일**: 2026-05-02
**대상**: `/tmp/wine-research/wine/dlls/{user32,gdi32,winex11.drv,win32u}/`
**own1 enforcement**: Wine 코드 lift **0줄**. 함수 path / 호출 chain / 데이터 흐름의 *위치 정보*만 기재. 본 project (`airgenome-gamebox/native/helper.mm`) 와의 비교는 본 코드 인용으로만 수행.
**목적**: 본 helper.mm 가 IPC kind=14/15 로 노출하는 USER32/GDI32 dispatch 의 *정확성* 검증을 위한 reference map 확보.

---

## 0. 전체 architecture 요약

Wine 8.0+ 부터 USER32 는 thin import shim 으로 축소되었고, *실 구현은 `dlls/win32u/` (kernel-mode user driver)* 으로 이전됨. 본 audit 의 핵심 발견:

| layer | role | 본 helper.mm 대응 |
|-------|------|------------------|
| `dlls/user32/` | A/W variant 분기 + thunk → `NtUser*` syscall (win32u) | 본 helper.mm 는 user32 자체 X — shim 측 IAT 가 직접 helper IPC 호출 |
| `dlls/win32u/` | 실 핵심 구현 (window/class/message/painting) — wineserver 와 통신 | helper.mm 는 win32u 의 *부분 동작* (window create / message loop) 을 AppKit 으로 자체 재구현 |
| `dlls/winex11.drv/` | X11 backend driver — `user_driver_funcs` table 등록, `XEvent` → `INPUT` 변환 | macOS 의 `winemac.drv` 와 *동일 패턴* — 본 helper.mm 는 NSEvent → Win32 MSG 자체 작성 |
| `dlls/gdi32/` | A/W → `NtGdi*` syscall thin shim | helper.mm IPC kind=15 는 **fake handle stub only** (실 GDI 호출 X) |

**결정적 차이**: Wine 은 `wineserver` 라는 별도 process 가 *모든* window state / message queue / atom table 을 hold. 본 project 는 wineserver 가 없고, helper subprocess 자체가 partial state owner. (R3-12 phase, kind=14 fn_id 1..23 stub).

---

## 1. dlls/user32/ — thin A/W shim 영역

### 1.1 RegisterClassEx*
- **A variant**: `class.c:282` `RegisterClassA` → 동일 file `:298` 에서 WNDCLASSEXA 채워서 `RegisterClassExA` 위임.
- **A → core**: `class.c:330` `RegisterClassExA` 에서 `:342` 한 줄로 `NtUserRegisterClassExWOW(...)` syscall — wow_atom_flags=1 로 ANSI 표시.
- **W variant**: `class.c:351` `RegisterClassExW` → `:363` 동일 syscall, wow_flag=0.
- **bridge**: `class.c:307` `RegisterClassW` 가 WNDCLASSW → WNDCLASSEXW 채워 `RegisterClassExW` 호출.

### 1.2 CreateWindowEx*
- **internal**: `win.c:282` `WIN_CreateWindowEx(CREATESTRUCTW*, className, module, unicode_flag)` — `:412` 에서 `NtUserCreateWindowEx` 호출. unicode 분기는 wow_handlers 통한 callback.
- **A variant**: `win.c:424` `CreateWindowExA` (DECLSPEC_HOTPATCH 표시) → `:450/454` 에서 `wow_handlers.create_window(cs, classNameW, instance, FALSE)` — 즉 `WIN_CreateWindowEx`.
- **W variant**: `win.c:461` `CreateWindowExW` → `:482` 동일하게 wow_handlers 경유.
- **note**: ANSI → Unicode 변환은 user32 측에서 수행, 그 후 wow_handlers vtable indirection (Wine WoW64 split 대응).

### 1.3 ShowWindow
- user32 측 다섯 호출처 (`win.c:368, 371, 492, 503, 662, 846`) 모두 *직접* `NtUserShowWindow(hwnd, cmd)` 호출 — A/W 분기 없음 (ShowWindow 은 ASCII/Unicode parameter 가 없음).

### 1.4 GetMessage / PeekMessage / TranslateMessage / DispatchMessage
- **PeekMessageW**: `message.c:710` → `:712` `NtUserPeekMessage` 한 줄.
- **PeekMessageA**: `message.c:719` → `:722` PeekMessageW 호출 후 ANSI 변환 (`map_wparam_char_WtoA` 등은 winproc.c 참조).
- **GetMessageW**: `message.c:731` → `:733` `NtUserGetMessage`.
- **GetMessageA**: `message.c:740` → `:743` GetMessageW 호출 후 char map.
- **TranslateMessage**: `message.c:789` → `:791` `NtUserTranslateMessage(msg, 0)`.
- **DispatchMessageA**: `message.c:815` → `:835` 주석 "whenever possible avoid NtUserDispatchMessage to make the call unwindable" 후 `:839` 본 syscall.
- **DispatchMessageW**: `message.c:865` → 동일 pattern, `:892` syscall.

### 1.5 winproc.c — WINPROC_wrapper asm bridge
- **i386 asm thunk**: `winproc.c:43..80` `__ASM_GLOBAL_FUNC( WINPROC_wrapper, ... )` — pushes ebp/edi/esi/ebx, zeros edi (TreePad X Enterprise 호환 hack — WM_TIMER 시 edi < 0x80000000 가정), 4 arg push 후 indirect call. CFI directives 로 unwinding 정보 보존.
- **non-i386 fallback**: `winproc.c:82..86` static inline — proc(hwnd, msg, wp, lp) 직접.
- **call_window_proc**: `winproc.c:103..115` — TRACE_(relay) 양쪽으로 wrap, `WINPROC_wrapper` 를 호출하여 *result에 저장.
- **call_dialog_proc**: `winproc.c:118..136` — DPI awareness context 보존 (SetThreadDpiAwarenessContext), DWLP_MSGRESULT 에서 결과 fetch.
- **WINPROC_AllocProc**: `winproc.c:148..151` — `NtUserCallTwoParam((UINT_PTR)func, !unicode, NtUserAllocWinProc)` — wndproc 는 한 번 alloc 후 free X (주석 144..147).
- **A↔W 변환**: `WINPROC_CallProcAtoW` (`:175`), `WINPROC_CallProcWtoA` (`message.c:706, 712, 721` 에서 호출) — character message (WM_CHAR 등) 시 codepage 변환.

### 1.6 controls.h — 내장 클래스 등록
- `controls.h` 157 line — built-in class 6종 (button/combo/edit/listbox/scroll/static) 의 `WNDPROC` 표 + `WS_EX_*` style flags 정의. 본 helper.mm 와 무관.

### 1.7 user_main.c — DLL 진입
- `user_main.c` 352 line — DllMain (DLL_PROCESS_ATTACH 시 wow_handlers 설정), wow_handlers vtable. wow_handlers 는 user32 ↔ user32(WoW64) 양쪽 binding 위해 분리됨.
- 본 project 와의 관련: 본 helper.mm 는 user32 자체를 load 하지 않음 (own1 § A — Wine code 0줄). shim 측 IAT 가 RegisterClassExA / CreateWindowExA / GetMessageA 등을 helper IPC 로 직접 redirect.

### 1.8 input.c — TranslateAccelerator / SendInput
- `input.c` 871 line — SendInput, GetKeyState, GetAsyncKeyState, TranslateAccelerator 등.
- `SendInput` → `NtUserSendInput` (win32u/input.c) — n×INPUT 구조를 wineserver hardware queue 에 batch.
- 본 project: SendInput 자체 IPC 미정의 — game 이 SendInput 으로 자체 mouse/key 합성 (자동화 매크로) 시 미지원.

### 1.9 message.c 의 send/post 계열 (USER32 측)
- SendMessageA/W, PostMessageA/W, PostThreadMessageA/W, SendMessageTimeoutA/W, SendMessageCallbackA/W, SendNotifyMessageA/W — 전부 `NtUserMessageCall(hwnd, msg, wp, lp, info, NtUserSend*Message, ansi)` 단일 entry 로 수렴 (win32u/message.c).
- BroadcastSystemMessageA/W: HWND_BROADCAST 처리 — `broadcast_message` (win32u/message.c:4060..) 가 list_window_children(0) 으로 top-level 순회.

---

## 2. dlls/win32u/ — 실 구현 영역 (Wine 8.0+ kernel-mode split)

### 2.1 Class atom table — class.c
- **integer atom 처리**: `class.c:282..345` `get_int_atom_value`, `is_integral_atom` — "#1234" 형 string 또는 LOWORD(p) < MAXINTATOM(0xC000) 검사. 본 project 의 helper.mm 는 integer atom 자체 발급 (sequence id, `:1286` case 1).
- **NtUserRegisterClassExWOW**: `class.c:510..605`
  - `:566` `wine_server_add_atom(req, name)` — wineserver 에 atom 추가.
  - `:570` `atom = reply->atom` — server 권위 atom.
  - `:592` TRACE 에 `name=%s atom=%04x wndproc=%p hinst=%p bg=%p style=%08x clsExt=%d winExt=%d class=%p` — 9개 field 가 server 측 class object 의 핵심.
- **find_class**: `class.c:460` — module + name → CLASS* lookup.
- **shared class**: `class.c:406..` `get_shared_window_class` — `obj_locator` 통해 wineserver shared memory mapping 으로 read-only access.

### 2.2 Window handle / CreateWindowEx — window.c
- **NtUserCreateWindowEx**: `window.c:5716..` 핵심 entry.
  - `:5795` `create_window_handle(parent, owner, class_name, class_instance, ...)` — wineserver 에 win 추가.
  - `:5966` `user_driver->pCreateWindow(hwnd)` — driver 측 (winex11.drv 면 X11 window 생성) 호출. 실패 시 goto failed.
  - 마지막 `:6015` `NtUserShowWindow(hwnd, sw)` — startup_show_window 적용.
  - 그 후 `:6332` peek-and-discard (WM_MOUSEFIRST..WM_MOUSELAST) — start 시 stale mouse event drain.
- **show_window** (static): `window.c:4815..4915`
  - `:4836` cmd == SW_SHOWDEFAULT 시 startup_show_window (process startup info) 적용.
  - `:4911` `user_driver->pShowWindow(hwnd, cmd, &newPos, swp)` — driver hook. 반환값이 ~0 이 아니면 swp flag 변경.
- **NtUserShowWindow**: `window.c:5028..5040` — full handle 보장 후 `show_window`.
- **NtUserShowWindowAsync**: `window.c:5005..5021` — 동일 path, async post.
- **set_window_pos**: `window.c:3941..` — SERVER_START_REQ(set_window_pos) 로 server state 동기화.

### 2.3 Message queue — message.c
- **NtUserPeekMessage**: `message.c:3540..3584`
  - `user_check_not_lock()`, `check_for_driver_events()` — peek 전 driver pump.
  - `peek_message(&msg, &filter)` 가 0 반환 (no msg): `flush_window_surfaces(TRUE)` + `KeUserDispatchCallback(thunk_lock)` (sleep waiting), `NtYieldExecution`.
  - 성공 시 `*msg_out = msg` — 주석 (`:3573..3576`): "msg_out 은 program 의 변수, send_message 가 program 에 다시 들어가서 corrupt 할 수 있으므로 internal copy".
- **NtUserGetMessage**: `message.c:3589..3621`
  - mask = QS_POSTMESSAGE|QS_SENDMESSAGE 기본, first/last 에 따라 QS_KEY/QS_MOUSE/QS_TIMER/QS_PAINT 추가 (`:3601..3606`).
  - `peek_message` 가 0 반환 시 `wait_objects(server_queue, INFINITE, ...)` block.
  - return `msg->message != WM_QUIT`.
- **NtUserDispatchMessage**: `message.c:4002..4050`
  - WM_TIMER + lParam: `init_win_proc_params(...)` + `dispatch_win_proc_params(&params, ...)`.
  - WM_SYSTIMER: SYSTEM_TIMER_CARET → toggle_caret, SYSTEM_TIMER_TRACK_MOUSE → update_mouse_tracking_info.
  - 일반 msg: `init_window_call_params` 후 `dispatch_win_proc_params` — KeUserModeCallback 통해 user-mode WndProc 호출.
  - WM_PAINT 후처리 (`:4042..4047`): `NtGdiCreateRectRgn` + `NtUserGetUpdateRgn(hwnd, hrgn, TRUE)` — non-client area 가 invalid 면 WM_NCPAINT/WM_ERASEBKGND 보냄.
- **call_window_proc** (internal, static): `message.c:2329..2390`
  - msg & 0x80000000 → `handle_internal_message` (Wine internal extension, 본 project 와 무관).
  - WH_CALLWNDPROC hook → `dispatch_win_proc_params` → WH_CALLWNDPROCRET hook.
  - `init_window_call_params`: hwnd → wndproc + ANSI/Unicode flag + DPI context 묶음.
  - `:2348` `user_message_size(hwnd, msg, wparam, lparam, type==MSG_OTHER_PROCESS, ansi, &reply_size)` — packed_size 계산. cross-process 면 lParam 가 ptr 일 때 message-specific size table 참조 (e.g. WM_SETTEXT 의 string length).
  - `:2358..2362` packed_size 가 있으면 sizeof(*params)+packed_size 크기로 malloc 후 16-byte align.
  - `:2371..2372` `pack_user_message(...)` — extra_buffer 에 packed lParam 직렬화 (heap allocation, NtFreeVirtualMemory 로 release `:2377`).
  - `:2379` `copy_user_result(ret_ptr, ...)` — wndproc 반환값에서 lParam (e.g. WM_GETTEXT 의 buffer) 를 caller 측 메모리에 다시 복사.
- **send_hardware_message**: `message.c:3887..3906` — `SERVER_START_REQ(send_hardware_message)` 로 wineserver 에 INPUT 전달. winex11.drv → NtUserSendHardwareInput → win32u/input.c → 본 함수 → wineserver 가 적절한 thread queue 에 hardware msg post.
- **process_hardware_message**: `message.c:2854..2887`
  - `:2862..2863` thread_info->msg_source.{deviceType, originId} = msg_data->source.{device, origin} — INPUT_MESSAGE_SOURCE 채움 (GetCurrentInputMessageSource 결과).
  - `:2866..2869` raw 좌표 → virtual screen 변환 (`map_rect_raw_to_virt`), DPI context = NTUSER_DPI_PER_MONITOR_AWARE 강제.
  - `:2871..2884` 분기:
    - WM_INPUT/WM_INPUT_DEVICE_CHANGE → `process_rawinput_message`.
    - is_keyboard_message → `process_keyboard_message`.
    - is_mouse_message → `process_mouse_message` (hwnd hit-test + capture + class cursor + WM_NCLBUTTON* 합성).
    - WM_POINTER* → `process_pointer_message` (touch).
    - WM_WINE_CLIPCURSOR / WM_WINE_SETCURSOR → 내부 cursor 동기화.
- **peek_message** (static): `message.c:2931..3227`
  - `:2956..2967` flags / mask → wineserver request 의 signal_bits / clear_bits 변환. QS_RAWINPUT 시 QS_KEY|QS_MOUSEMOVE|QS_MOUSEBUTTON 도 함께 fetch.
  - `:2972..3001` `SERVER_START_REQ(get_message)` — wineserver 에서 다음 msg 한 개 fetch. reply 가 STATUS_BUFFER_OVERFLOW 면 buffer realloc 후 retry.
  - `:3029, 3032, 3044, 3081, 3121` info.flags = ISMEX_SEND/ISMEX_NOTIFY/ISMEX_CALLBACK 등 — 다양한 inter-thread send 모드.
  - `:3140` 위 process_hardware_message 호출 site — hw_id 가 nonzero 면 hardware 분기.
  - `:3171..3180` 재귀 peek (filter.internal=TRUE) — 내부 system message 가 끼어들 때.
  - `:3227` `thread_info->client_info.receive_flags = info.flags` — wndproc 진입 직전 GetMessageExtraInfo 등 호환.
- **dispatch_win_proc_params**: 위 init_window_call_params 와 한 쌍. 본 project 와 가장 강하게 대조됨 — Wine 은 *user-mode callback* (KeUserModeCallback) 으로 ntoskrnl-style transition 후 wndproc 호출 (callback id = NtUserCallWinProc). 본 project 는 IPC 응답으로 wndproc invoke 요청 → shim phase 17 interp 가 i386 ABI 로 wndproc 직접 call.

### 2.4 Painting — painting.c, dce.c
- **NtUserBeginPaint**: `dce.c:1704..1729`
  - `NtUserHideCaret(hwnd)` first.
  - `send_ncpaint(hwnd, NULL, &flags)` — UPDATE_NONCLIENT|UPDATE_ERASE|UPDATE_PAINT|UPDATE_INTERNALPAINT|UPDATE_NOCHILDREN (`:1710`).
  - `send_erase(hwnd, flags, hrgn, &rect, &hdc)` — WM_ERASEBKGND 보내고 hdc 획득.
  - PAINTSTRUCT 채워서 (fErase / rcPaint / hdc) 반환.
- **NtUserEndPaint**: `dce.c:1734..1741`
  - `NtUserShowCaret(hwnd)`.
  - `flush_window_surfaces(FALSE)` — 모든 dirty surface 를 driver 측 commit.
  - `release_dc(hwnd, ps->hdc, TRUE)`.
- **erase_now**: `dce.c:1748..` RDW_ERASENOW 처리.

### 2.5 driver vtable — driver.c
- `driver.c:39` `static const struct user_driver_funcs lazy_load_driver` — 첫 호출 시 driver dll load.
- `driver.c:1023` `load_driver(void)` — registry/env 에서 driver 이름 (winex11/winemac/winewayland) 조회 후 dlopen + `__wine_set_user_driver` callback.
- `driver.c:1242` `lazy_load_driver` 의 모든 entry 는 *실제 driver load 후 vtable swap → 자기 자신 재호출* 패턴.
- `driver.c:1320` `const struct user_driver_funcs *user_driver = &lazy_load_driver` — 글로벌 pointer.
- `driver.c:1325` `__wine_set_user_driver` — 지정 driver 가 lazy 면 null_user_driver, 아니면 본 vtable 로 swap.

### 2.6 spec / syscall table — win32u.spec, win32syscalls.h
- `win32u.spec:809` `NtUserBeginPaint(long ptr)` syscall ordinal.
- `win32u.spec:958` `NtUserEndPaint(long ptr)`.
- `win32syscalls.h:811` `SYSCALL_ENTRY(0x1327, NtUserBeginPaint, 8)` — 8 byte parameter total (HWND + PAINTSTRUCT*).
- `win32syscalls.h:960` `0x13bc, NtUserEndPaint, 8`.

---

## 3. dlls/winex11.drv/ — X11 driver 영역 (winemac.drv 와 동일 patron)

### 3.1 driver 등록 — init.c
- `init.c:625..718` `static const struct user_driver_funcs x11drv_funcs = { ... }` — 약 90개 hook 점.
  - `:688` `pCreateWindow = X11DRV_CreateWindow`
  - `:690` `pDestroyWindow = X11DRV_DestroyWindow`
  - `:693` `pProcessEvents = X11DRV_ProcessEvents`
  - `:703` `pSetWindowStyle = X11DRV_SetWindowStyle`
  - `:705` `pShowWindow = X11DRV_ShowWindow`
  - `:714` `pCreateWindowSurface = X11DRV_CreateWindowSurface`
- `init.c:725` `init_user_driver(void)` → `:727` `__wine_set_user_driver(&x11drv_funcs, WINE_GDI_DRIVER_VERSION)`.
- `x11drv_main.c:679` `init_user_driver()` 가 dll DLL_PROCESS_ATTACH 시 호출.

### 3.2 X11DRV_CreateWindow / Show / SetStyle — window.c
- **X11DRV_CreateWindow**: `window.c:2761` `(HWND hwnd)` — driver entry.
  - `:2771` clip_window: `XCreateWindow(display, root_window, 0,0,1,1, 0, 0, ...)` — input clipping helper (cursor clip).
  - `:2554, 2852, 3159` 세 호출 site 에서 `create_whole_window(data)` (`:2400..2554`) 호출 — `XCreateWindow` 로 frame window 생성.
  - `:2378` `data->client_window = XCreateWindow(...)` — client area window (sub-window inside whole).
  - `:2436` `data->whole_window = XCreateWindow(data->display, root_window, pos.x, pos.y, ...)` — whole frame (window manager 가 보는 외곽).
- **X11DRV_ShowWindow**: `window.c:3348..3393`
  - `:3354` style = `NtUserGetWindowLongW(hwnd, GWL_STYLE)`.
  - `:3358..3367` 데이터 X 또는 minimize 상태면 좌표 보정만 (-32000, -32000 hide-icon trick) 후 return.
  - `:3368` managed=FALSE 또는 wm_state != NormalState 면 (즉 unmapped 인 상태) skip — XShowWindow 자체는 win32u/window.c 의 generic show_window 가 ConfigureWindow 로 처리.
  - `:3372..3377` *current_event 가 ConfigureNotify/PropertyNotify 가 아니면* skip (재진입 방지).
  - `:3382..3388` `XGetGeometry` + `XTranslateCoordinates` + `root_to_virtual_screen` 로 *현재 actual position* 가져와서 `*rect` 에 set, `swp &= ~SWP_NOMOVE|...` (즉 "이미 옮겨졌다" 라고 win32u 측에 알림).
  - return swp — win32u show_window 는 이 swp 를 set_window_pos 에 전달.
- **X11DRV_SetWindowStyle**: `window.c:2581..` (offset, STYLESTRUCT*) — GWL_STYLE/GWL_EXSTYLE 변경 시 XReconfigureWMWindow / XSetWMNormalHints 갱신.
- **X11DRV_DestroyWindow**: `window.c:2618..` — XDestroyWindow + win_data free + XDeleteContext (winContext map 에서 제거).
- **XMapWindow / XUnmapWindow** 호출처: `:1632, 1637, 2269, 2385` — managed/unmanaged 분기.

### 3.3 Event loop — event.c
- **handler table**: `event.c:96..134` `static x11drv_event_handler handlers[MAX_EVENT_HANDLERS] = { ... }` — XEvent type code (KeyPress=2 .. GenericEvent=35) 별 handler pointer.
- **register**: `event.c:269` `X11DRV_register_event_handler(int type, x11drv_event_handler handler, const char *name)` — 동적 추가 (XInput2 등).
- **call_event_handler** (static inline): `event.c:436..464`
  - `:443..447` handler[event->type] == NULL → silently ignore.
  - `:452` `XFindContext(display, event->xany.window, winContext, &hwnd)` — Xlib window → HWND lookup.
  - `:454` 못 찾으면 `NtUserGetDesktopWindow()`.
  - `:458..462` thread_data->current_event swap 후 handler 호출 (recursion 방지).
- **X11DRV_ProcessEvents**: `event.c:476..` driver hook entry.
  - `:484` 이미 current_event 있으면 mask=0 (nested 진입 안 함).
  - `:489` `XCheckIfEvent(data->display, &event, filter_event, mask)` — non-blocking dequeue + filter.
  - `:504` `XFilterEvent(&event, None)` — XIM (input method) 가 먹으면 skip.
  - `:507..` `merge_events(&prev_event, &event)` — MotionNotify / RawMotion 인접 동일 window 묶기 (`:397..427`).

### 3.4 Keyboard event → Win32 INPUT — keyboard.c
- **X11DRV_KeyEvent**: `keyboard.c:1357..1447`
  - `:1383` KeyPress + XIC 있으면 `XmbLookupString` (XIM composing).
  - `:1397` 그 외엔 `XLookupString`.
  - `:1427` `vkey = EVENT_event_to_vkey(xic, event)` — Wine 자체 keycode 표.
  - `:1430` `bScan = keyc2scan[event->keycode] & 0xFF` — scan code.
  - `:1439..1441` flags = (release ? KEYEVENTF_KEYUP : 0) | (vkey & 0x100 ? KEYEVENTF_EXTENDEDKEY : 0).
  - `:1443` `update_lock_state(hwnd, vkey, event->state, event_time)` — CapsLock/NumLock 동기화.
  - `:1445` `X11DRV_send_keyboard_input(hwnd, vkey & 0xff, bScan, dwFlags, event_time)`.
- **X11DRV_send_keyboard_input** (static): `keyboard.c:1149..1164`
  - `:1164` `NtUserSendHardwareInput(hwnd, 0, &input, 0)` — win32u/input.c → win32u/message.c send_hardware_message → wineserver.

### 3.5 Mouse event → Win32 INPUT — mouse.c
- **X11DRV_ButtonPress**: `mouse.c:1533..1560` — INPUT.mi.{dx,dy,mouseData,dwFlags,time,dwExtraInfo} 채움. `MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE` 동시 set (Windows 는 button + move 을 하나의 event 로 흔히 처리). `map_event_coords` 후 `send_mouse_input`.
- **X11DRV_ButtonRelease**: `mouse.c:1566..1586` 대칭.
- **X11DRV_MotionNotify**: `mouse.c:1592..1615` — `MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE`. `is_old_motion_event(serial)` 체크 — warp 후 stale event drop.
- **X11DRV_EnterNotify**: `mouse.c:1621..` — *fake* motion event 합성 (Win32 에는 EnterNotify 없음).
- **send_mouse_input** (static): `mouse.c:536..570`
  - `:546, 570` `NtUserSendHardwareInput(hwnd, 0, input, 0)` — 두 코드 path (capture / non-capture).

### 3.6 BitBlt / window surface — bitblt.c
- **x11drv_window_surface**: `bitblt.c:1585..1597` — `struct window_surface header` 가 base, get_x11_surface 캐스트.
- **x11drv_surface_funcs**: `bitblt.c:1873` — set_clip(`:1772`), flush(`:1791`), destroy(`:1864`).
- **x11drv_surface_flush**: `bitblt.c:1791..` — XImage to XPutImage / XCopyArea, alpha_mask 처리.
- **create_surface** (static): `bitblt.c:1883..1950` — `window_surface_create(sizeof(*surface), &x11drv_surface_funcs, hwnd, rect, info, bitmap)` (Wine 공통 base) 호출.
- **X11DRV_CreateWindowSurface**: `bitblt.c:1967..` driver hook — layered window 분기, previous surface release.
- **gdi32 flow**: gdi32/dc.c:1942 `BitBlt(hdc_dst, ...)` → `:1954` `NtGdiBitBlt(...)` syscall → win32u/bitblt.c — DC->driver hook (e.g. dibdrv 또는 x11drv). 본 helper.mm 측 GDI dispatch 는 *실 호출 X*.

---

## 4. dlls/gdi32/ — thin shim (실 구현은 win32u)

### 4.1 BitBlt — dc.c
- `dc.c:1942..1954` `BitBlt(HDC dst, x, y, w, h, HDC src, sx, sy, rop)` →
  - `:1947` `is_meta_dc(dst)` 면 `METADC_BitBlt` (metafile recording).
  - `:1951` `dc_attr->emf` 면 `EMFDC_BitBlt` (enhanced metafile, recording 후 fall-through).
  - `:1954` `NtGdiBitBlt(...)` syscall.

### 4.2 TextOut*, ExtTextOut* — text.c
- `text.c:1059..1063` `TextOutA(hdc, x, y, str, count)` → `ExtTextOutA(hdc, x, y, 0, NULL, str, count, NULL)`.
- `text.c:1067..1071` `TextOutW` → `ExtTextOutW`.
- `text.c:1000..1056` `ExtTextOutA` — string 이 RTL 형이면 (`:1014` flags & ETO_GLYPH_INDEX 등) 직접 wide 처리, 아니면 `MultiByteToWideChar` + dx 배열 변환 후 `ExtTextOutW`.
- `text.c:954..993` `ExtTextOutW` → `:993` `NtGdiExtTextOutW(hdc, x, y, flags, rect, str, count, dx, 0)`.
- `text.c:1076, 1087` PolyTextOut* — N개 POLYTEXT 구조체를 ExtTextOut 으로 펼침.

### 4.3 user32 측 BeginPaint/EndPaint 사용 (built-in controls)
- button.c:223/230, combo.c:773/815, edit.c:3721/3791, listbox.c:3089/3091, static.c:364/375 — 모두 `NtUserBeginPaint` / `NtUserEndPaint` 패턴. wParam 이 HDC 전달이면 그것 사용, 아니면 BeginPaint/EndPaint 페어. PAINTSTRUCT 변수 이름은 일관 `ps`.

### 4.4 GDI32 의 metafile / EMF 분기 (BitBlt 외 공통 패턴)
- `dc.c:1942..1954` BitBlt 의 두 단계 분기:
  1. `is_meta_dc(hdc_dst)` true → `METADC_BitBlt` — Win16 호환 metafile recording.
  2. `dc_attr->emf` truthy → `EMFDC_BitBlt` — Win32 enhanced metafile recording (record 하고 fall-through 하여 실제 그리기도 수행).
  3. fall-through → `NtGdiBitBlt`.
- 동일 패턴이 GDI32 의 거의 모든 그리기 함수 (LineTo, Rectangle, Ellipse, PolyBezier, FillRect 등) 에 일관 적용.
- 본 project: metafile 계열은 미지원 (game 거의 사용 X).

### 4.5 Text rendering 의 ANSI → Unicode 경로
- `text.c:1000..1056` ExtTextOutA 의 ANSI 변환:
  - `:1014` 직접 wide path 가능 한 경우 (flags & ETO_GLYPH_INDEX) 즉시 ExtTextOutW.
  - 그 외엔 dx[] 배열 변환 필요 — ANSI 문자 1개가 multi-byte 일 수 있고 dx[] 는 *원본 문자 단위* 이므로 wide character 단위로 재계산.
  - `:1051` MultiByteToWideChar(CP_ACP, ...) + dxW[] 재계산 후 ExtTextOutW.
- 본 project gdi32_call IPC 는 fn_id 1..8 stub only (helper.mm:1411..). 실 text rendering 시 CoreGraphics CGContextDrawLineOfText 또는 CTLine 으로 redirect 필요.

### 4.6 NtGdi syscall 진입 — gdi32.spec
- `gdi32.spec` 에서 `BitBlt` → `NtGdiBitBlt`, `ExtTextOutW` → `NtGdiExtTextOutW` 등 직접 forward (user32.spec 의 BeginPaint→NtUserBeginPaint 와 동일 패턴).
- win32u 측에서는 `NtGdiBitBlt` (win32u/bitblt.c) → DC->driver hook (DC_FUNC) 호출 — DIB 면 dibdrv (win32u/dibdrv/), X11 면 X11DRV BitBlt.

---

## 5. 본 project (helper.mm) ↔ Wine 비교

### 5.1 NSEvent → Win32 MSG mapping vs Wine X11 → INPUT
- **본 project**: `helper.mm:793..809` `nsevent_to_win32_msg(NSEventType)` — 11 case (LeftMouseDown..ApplicationDefined) → WM_* 직접 변환. NSEvent 자체가 button + 좌표 묶음이므로 Wine 의 `XButtonEvent` + `MOUSEEVENTF_ABSOLUTE|MOVE` 합성 (mouse.c:1547) 에 해당하는 *INPUT 단계 생략*. helper 가 직접 MSG 합성.
- **Wine path**: XButtonEvent → `send_mouse_input` → `NtUserSendHardwareInput` → win32u/message.c `send_hardware_message` → wineserver hardware queue → peek 시 `process_hardware_message` 가 hwnd hit-test + WM_* 합성. 즉 *event time 과 dispatch time 의 분리* 가 본질.
- **본 project 의 단순화**: NSEvent → MSG 합성을 즉시 (GetMessageA 차단 해제 시점) 수행. process_hardware_message 의 *cursor capture / class-cursor / WM_NCHITTEST* 처리는 없음. Pure top-level window → client area 에서만 동작.
- **keyboard 표**: `helper.mm:710..776` `g_keymap[256]` — Apple HIToolbox virtual keycode (kVK_*) → VK_* 직접 표. Wine `keyc2scan` (winex11.drv/keyboard.c) 와 *완전 다른 source* (X11 keysym 기반 vs HIToolbox kVK_*). 자체 작성 명시 (`:704..707` 주석).

### 5.2 USER32 IPC kind=14 의 정확성
- **fn_id=1 RegisterClassExA** (`helper.mm:1286..1291`): 응답 = sequence id (fake atom). 실 atom table X. Wine 은 wineserver 가 atom 발급 (class.c:566).
  - **gap**: integer atom (`#1234`) 동등성 (helper.mm 의 fake atom 이 우연히 string "#NNNN" 변환 시 isIntegralAtom 으로 reverse 가능한지 미보장). 실 game 이 GetClassInfoEx 로 lookup 하면 fail.
- **fn_id=2 CreateWindowExA** (`helper.mm:567..648`): 실 NSWindow alloc + makeKeyAndOrderFront, 32-bit fake hwnd return. Win32 spec 준수: WM_NCCREATE / WM_CREATE 를 `pending_msg_push` 로 inject (helper.mm:648 직후, 주석 `:574`).
  - **gap**: WM_GETMINMAXINFO / WM_NCCALCSIZE / WM_PARENTNOTIFY 등 미합성. Wine win32u/window.c:5716..6015 는 약 30개 message 합성. 본 project 는 NCCREATE/CREATE 만.
- **fn_id=3 ShowWindow** (`helper.mm:661..696`): NSWindow makeKeyAndOrderFront / orderOut. Wine show_window (`window.c:4815..4915`) 는 SW_* 11 case + visible/invisible state 변환 + WM_SHOWWINDOW 발송. helper 측은 visible toggle 만.
- **fn_id=5 GetMessageA** (`helper.mm:698..1015 일부`): NSApp.nextEventMatchingMask:untilDate:distantFuture (block). 본질적으로 win32u NtUserGetMessage 의 `wait_objects(server_queue, INFINITE, ...)` 와 동등 — 단 wait 객체가 wineserver port 가 아닌 NSEvent queue.
  - **pending_msg FIFO**: helper.mm:818..846. NSWindow notification observer (`AHWindowNotifyObserver`, `:850..`) 가 didBecomeKey/willClose/didResize 등 → WM_SETFOCUS/WM_KILLFOCUS/WM_CLOSE/WM_SIZE/WM_MOVE 합성 push. Wine winex11.drv 의 X11DRV_FocusIn/FocusOut/ConfigureNotify (event.c:83..91) 와 동등 의도.
- **fn_id=6 TranslateMessage** (`helper.mm:1314 ~ case 6`): 항상 1 반환 (AppKit auto translate). Wine `NtUserTranslateMessage` 는 keysym → WM_CHAR/WM_DEADCHAR 합성 후 queue 에 post — 본 project 는 NSEvent characters 자체 사용 가정 (R3-12 stub).
- **fn_id=7 DispatchMessageA** (`helper.mm:1017..1351 일부`): 확장 응답 format — head + WndProcInvoke block. 즉 helper 가 wndproc 을 *호출하지 않고*, shim 측 phase 17 interp 영역에 invoke request 보냄.
  - **이는 Wine 과 본질적으로 다름**: Wine NtUserDispatchMessage (message.c:4002..4050) 는 `dispatch_win_proc_params` 를 통해 KeUserModeCallback 으로 user-mode WndProc 직접 호출. 본 project 는 i386 stub 측이 wndproc 을 hold 하므로 IPC 로 다시 shim 에 위임.
  - **WndProc dispatch back to shim path**: helper.mm:483..540 `g_window_class_map` (atom → wndproc_va), `g_hwnd_wndproc_map` (hwnd → wndproc_va) 로 lookup → DispatchMessageA 응답에 wndproc_va 첨부 → shim 측이 phase 17 interp 진입 후 i386 ABI 로 wndproc 호출.
- **fn_id=14 SendMessageA** (`helper.mm:1352..`): DispatchMessageA 와 동일 확장 응답. Wine 은 `NtUserMessageCall` 에서 inter-thread 면 wineserver 통한 sync send, 동일 thread 면 직접 call_window_proc — 본 project 는 동일 process 내 sync invoke 만 가정.
- **fn_id=15 PostMessageA** (`helper.mm:1396 근방`): pending_msg FIFO push. 다음 GetMessageA 가 dequeue. Wine 은 wineserver thread queue 에 post → per-thread queue 분리.
  - **gap**: 본 project 는 thread queue 1개. cross-thread PostMessage → 다른 thread peek 의 동작 미보장.

### 5.3 GDI32 IPC kind=15
- helper.mm:1410 `gdi32_stub_lookup(fn_id, arg_count, args)` — fn_id 1..8 → fake handle (sequence id 또는 0xCAFE 류) 반환.
- own1 § A 명시 (helper.mm:46..47): "kind 14/15 는 trace stub only — 실 USER32/GDI32 함수 호출 X. AppKit GUI translation 은 R3-12 별도 phase 에서 진행."
- 실 GDI32 path 는 gdi32/dc.c:1954 BitBlt → NtGdiBitBlt → win32u/bitblt.c → DC->funcs (dibdrv: software DIB, 또는 x11drv_funcs.dc_funcs: X server). 본 project 가 Metal/CoreGraphics surface 로 대체하려면 win32u/dibdrv (CPU rasterizer) 동등물 + helper 측 surface flush 가 필요. 현재 미구현.

---

## 5.4 message queue 구조 비교 — wineserver vs helper subprocess

| 측면 | Wine | 본 project |
|------|------|-----------|
| state owner | wineserver process (별도 daemon) | helper subprocess (game process 와 fork-exec 관계) |
| per-thread queue | thread_id 기반 multi queue | 단일 queue (g_pending_msgs) |
| inter-thread send | wineserver port 통한 ack/wait | 미지원 (cross-thread call 시 sync invoke 만) |
| atom table | wineserver 권위 (atom_t = uint32) | helper 측 sequence id (NSMutableDictionary) |
| hwnd | wineserver issued (32-bit handle) | helper 측 32-bit fake (uint32, NSWindow* mapping) |
| hardware msg path | XEvent → INPUT → server.send_hardware_message → server.queue → process_hardware_message → MSG | NSEvent → 즉시 nsevent_to_win32_msg → MSG (process 단계 생략) |
| WM_PAINT 합성 | wineserver 가 update region 추적, peek_message 시 합성 | 미구현 (helper 측 WM_PAINT 합성 X) |
| sync send (SendMessage) | 같은 thread 면 직접 call_window_proc, 다른 thread 면 server.send_message + queue | helper 가 IPC 응답에 wndproc invoke 첨부 → shim 측 동기 호출 |
| broadcast (HWND_BROADCAST) | server-side window enumeration + per-window send | 미지원 |

### Wine 의 KeUserModeCallback 트랜지션

Wine 8.0+ 의 win32u 는 *kernel-mode-style* dll 로 모델링됨. wndproc 호출은 다음 단계:

1. win32u/message.c `dispatch_win_proc_params` 호출.
2. `KeUserModeCallback(NtUserCallWinProc, &params, ...)` — ntdll 의 KiUserCallbackDispatcher 동등물로 user-mode 진입.
3. ntdll/Wow64 통해 user32 측 `User32CallWindowProc` callback 진입 → winproc.c 의 call_window_proc → WINPROC_wrapper 호출 → 실 game wndproc.

본 project 는 *helper 가 ntdll/win32u 의 책임 일부만 가짐*. wndproc 자체는 i386 game shim 이 hold 하므로:
1. helper IPC 응답으로 wndproc_va + 4 args (hwnd/msg/wp/lp) 전달.
2. shim phase 17 interp 가 i386 ABI 로 wndproc 호출 (push 4 args, CALL, EAX=return).
3. shim 이 다시 IPC 로 SendMessageReply (또는 단순 GetMessageA 재호출) 보냄.

이는 *Wine 의 KeUserModeCallback path 우회* — interp 영역에서 Wine 의 winproc_wrapper asm 동등물이 필요. 본 project 는 i386 stub 측에서 cdecl/stdcall 차이 + edi=0 hack (Wine winproc.c:62) 동등물 검토 필요.

---

## 6. R3-12 phase 후속 closure 권고

### 6.1 RegisterClassEx atom 정확성 — 중간 우선
- 현재 sequence id fake atom. shim 측 GetClassInfoExA → helper IPC 추가 fn_id (e.g., fn_id=23) 로 atom → CLASS dump 가능하게 해야 cross-process compat 확보.

### 6.2 WM_NCCALCSIZE / WM_NCHITTEST 합성 — 높은 우선
- 본 project CreateWindowExA 는 NCCREATE/CREATE 만 push. 게임 측 *non-client area 처리* 의존하면 fail. helper.mm:567..648 사이에 추가 push 필요.

### 6.3 process_hardware_message 동등물 — 높은 우선
- Wine win32u/message.c:2854 가 hwnd hit-test + cursor 변환 + WM_NCMOUSE* 합성. helper.mm 의 mouse event 는 즉시 client area 가정.
- 권고: GetMessageA 차단 해제 직후, NSEvent.locationInWindow 좌표 → window content area 검사 → bar 영역이면 WM_NCLBUTTONDOWN, 아니면 WM_LBUTTONDOWN 분기.

### 6.4 GDI32 surface flush — 미정 (설계 선결)
- own1 § A 가 명시한 "AppKit GUI translation 별도 phase". helper.mm:1410 gdi32_stub_lookup 은 trace only. Metal layer 기반 surface 도입 시 win32u/bitblt.c:create_surface 를 helper 측 NSWindow.contentView.layer 로 매핑하는 구조 필요.

### 6.5 winproc.c i386 asm thunk 동등물
- Wine winproc.c:43..80 의 WINPROC_wrapper (xor edi 등 hack) 는 본 project shim 측 phase 17 interp 가 *완전히 책임* 영역. helper 측 wndproc invoke request 응답에 wndproc_va + 4 args 만 포함되므로, shim interp 가 i386 ABI 정확히 구현해야 함 — Wine 의 edi=0 hack 도 옮길지 검토 필요 (TreePad 비슷한 game 미만 가능성 낮음).

---

## 6.6 추가 — peek_message 의 server-bit logic 동등물 부재

Wine win32u/message.c:2956..2967 의 `signal_bits` / `clear_bits` 변환 logic 은 wineserver 와 완벽히 동일한 mask 협약을 따름 (`/* use the same logic as in server/queue.c get_message */` 주석 :2955). 본 project 는 이 mask 협약 자체가 없음 — fn_id=5 (GetMessageA) 가 무조건 NSEvent block, fn_id=8 (PeekMessageA, helper.mm:1106 부근) 도 동일.

권고 (낮은 우선): GetMessageA 의 first/last 인자 (helper.mm 측 args[1..2]) 를 검사해서 WM_KEYFIRST/WM_MOUSEFIRST/WM_PAINT mask 처리. 단 이는 game 이 GetMessage(NULL,0,0,0) 만 쓸 경우 영향 없음 (대부분의 게임 main loop 패턴).

## 6.7 추가 — XIM / TextInput / WM_DEADCHAR

Wine winex11.drv/keyboard.c:1383 `XmbLookupString` (XIM) → `xim_set_result_string` (helper.mm 와 무관, X11 한정) 후 win32u 측에서 WM_IME_* 합성. 본 project 는 NSTextInputClient protocol (AppKit) 으로 한국어/일본어/중국어 IME 입력 처리가 필요 — 현재 helper.mm 는 NSEventTypeKeyDown 의 characters property 만 사용, dead key composition 은 OS 단계에서 이미 끝난 상태.

이는 *대부분 game 에서 직접 입력 X — chat box / login id 영역에서만 필요*. R3-12 phase 의 closure 시점에는 우선순위 낮음.

---

## 6.8 추가 — winex11.drv 의 win_data lock 패턴 동등물 부재

Wine winex11.drv 의 모든 함수가 `get_win_data(hwnd)` → 처리 → `release_win_data(data)` 패턴 (e.g. mouse.c:1551, window.c:3356, keyboard.c:1374). win_data 는 per-hwnd lock + X11 데이터 (whole_window, client_window, rects, surface) 를 묶은 구조.

본 project: `g_window_map` (NSMutableDictionary, fake_hwnd→NSWindow) + `g_window_map_q` (DISPATCH_QUEUE_SERIAL) 으로 동등 효과. 단 lock granularity 가 *전역* 이므로 다중 window 동시 update 시 contention 가능. R3-12 phase 의 multi-window 게임 (e.g. 보조 dialog) 에서 측정 후 per-hwnd lock 로 분리 검토.

## 6.9 추가 — Wine USER32/GDI32 의 buffer ownership 규약

- A/W 변환 시 Wine 은 ANSI buffer 를 stack 또는 heap (RtlAllocateHeap) 에 임시 배치 (e.g. text.c:1051 의 `p` malloc 후 종료 직전 free).
- packed message (cross-process) 는 NtAllocateVirtualMemory 로 *별도 page* 할당 — wineserver shared memory mapping.

본 project: helper IPC frame 의 payload 는 max 65536 byte (helper.mm:69 `uint8_t payload[65536]`). DispatchMessageA / SendMessageA 의 packed lParam (e.g. WM_SETTEXT 의 string) 이 64KB 초과 시 fail — 큰 게임 (textbox 입력 X) 에서는 영향 없음.

---

## 7. 결론

- **own1 enforcement OK**: Wine 코드 0줄 lift 확인. 모든 reference 는 path/line 만 기재.
- **본 helper.mm 의 USER32 dispatch 는 Wine win32u/window.c+message.c+winex11.drv/event.c 의 *축소 모델***. wineserver-equivalent state holder X (helper subprocess 자체가 partial state).
- **macOS winemac.drv 와 동일 patron**: NSEvent → Win32 MSG/INPUT 변환 위치가 `winex11.drv/{event,keyboard,mouse}.c` 와 1:1 대응 (단 본 project 는 INPUT layer 우회).
- **R3-12 closure 후속**: WM_NC* 합성 / hardware hit-test / GDI surface 가 우선. atom precision 은 cross-process 게임에서만 영향.
- **WndProc dispatch path**: helper 측이 호출 X — shim phase 17 interp 가 i386 ABI 로 직접 호출. 본질적으로 *Wine 의 inter-process callback 우회* (KeUserModeCallback → IPC 응답).

---

**참고 line 인용 규약**: 본 문서의 line 번호는 모두 `/tmp/wine-research/wine/dlls/.../*.c` 의 read-only 위치 marker. 코드 내용 transcribed X. 본 project helper.mm 의 line 번호는 `/Users/ghost/core/airgenome-gamebox/native/helper.mm` 기준.

---

## 부록 A — Wine 의 message dispatch 전체 graph (path-only)

```
[i386 game shim main loop]
   |
   v (IAT call to USER32!GetMessageA)
[helper IPC kind=14 fn_id=5]                          [Wine: user32/message.c:740 GetMessageA]
   |                                                       |
   v (NSApp.nextEventMatchingMask block)                   v
[NSEvent received]                                    [user32/message.c:743 GetMessageW]
   |                                                       |
   v (helper.mm:793 nsevent_to_win32_msg)                  v
[Win32Msg synth]                                      [win32u/message.c:3589 NtUserGetMessage]
   |                                                       |
   v (frame response)                                      v
[shim: GetMessageA returns 1, MSG filled]             [win32u/message.c:2931 peek_message]
   |                                                       |
   v (shim: TranslateMessage / DispatchMessageA)           v
[helper IPC kind=14 fn_id=7]                          [SERVER_START_REQ(get_message)]
   |                                                       |
   v (helper: hwnd → wndproc_va lookup)                    v
[response: WndProcInvoke {wndproc_va, hwnd, msg, wp, lp}]  [wineserver returns msg + hw_id]
   |                                                       |
   v (shim phase 17 interp i386 ABI)                       v
[wndproc called in i386 land]                         [if hardware: process_hardware_message]
   |                                                       |
   v (interp: EAX = return)                                v
[shim: SendMessageReply or just loop again]           [win32u/message.c:2329 call_window_proc]
                                                          |
                                                          v
                                                     [WH_CALLWNDPROC hook]
                                                          |
                                                          v
                                                     [dispatch_win_proc_params]
                                                          |
                                                          v
                                                     [KeUserModeCallback(NtUserCallWinProc)]
                                                          |
                                                          v
                                                     [user32/winproc.c call_window_proc]
                                                          |
                                                          v
                                                     [WINPROC_wrapper (i386 asm)]
                                                          |
                                                          v
                                                     [game wndproc]
                                                          |
                                                          v
                                                     [WH_CALLWNDPROCRET hook]
```

좌측 = 본 project, 우측 = Wine 표준. 핵심 차이:
- Wine 은 wineserver port 를 거쳐 hardware msg 가 thread queue 에 enqueue → peek 시 process_hardware_message → MSG 합성. 본 project 는 NSEvent 를 즉시 MSG 로 변환.
- Wine 은 WndProc 호출이 *프로세스 내부* (KeUserModeCallback 후 user-mode 진입). 본 project 는 *프로세스 내부 동일* 단 IPC reply 로 invoke 위임.

## 부록 B — winex11.drv 의 user_driver_funcs 필드 분포

`init.c:625..718` 의 x11drv_funcs 초기화 에서 채워지는 hook 약 90개 (.pCreateWindow 외 .p* 다수). 분류:

- **Window lifecycle** (~10): pCreateWindow, pDestroyWindow, pCreateDesktopWindow, pSetDesktopWindow, pShowWindow, pSetParent, pSetWindowStyle, pSetWindowPos, pSetWindowText, pSetWindowIcon.
- **Surface / paint** (~5): pCreateWindowSurface, pUpdateLayeredWindow, pSetLayeredWindowAttributes, pSetWindowRgn, pFlushWindowSurfaces.
- **Input / cursor** (~10): pSetCursor, pGetCursorPos, pSetCursorPos, pClipCursor, pVkKeyScanEx, pToUnicodeEx, pGetKeyboardLayout, pActivateKeyboardLayout, pBeep, pUpdateInputState.
- **DC functions** (dc_funcs sub-struct, ~30): pBitBlt, pStretchBlt, pAlphaBlend, pTransparentBlt, pPolygon, pPolyline, pPolyBezier, pExtTextOut, pCreateDC, pDeleteDC, pCreateCompatibleDC, pSelectBitmap, pSelectBrush, pSelectFont, pSelectPen, etc.
- **Display / monitor** (~10): pUpdateDisplayDevices, pGetCurrentDisplaySettings, pChangeDisplaySettings, pGetMonitorInfo, pEnumDisplayMonitors.
- **Clipboard** (~5): pUpdateClipboard, pGetClipboardData, pSetClipboardData, pCountClipboardFormats, pEnumClipboardFormats.
- **Misc** (~20): pNotifyIcon (systray), pSystrayDockInit/Insert/Clear/Remove, pMessageBeep, pVulkanInit, pwineGetSpec, pProcessEvents, pMsgWaitForMultipleObjectsEx, etc.

본 project 가 winemac.drv 패턴으로 *완전 mirror* 하려면 helper 측에 90개 hook 동등물 필요. 현재 IPC kind=14/15 는 핵심 ~12개 만 (RegisterClassExA, CreateWindowExA, ShowWindow, GetMessageA, PeekMessageA, TranslateMessage, DispatchMessageA, PostMessageA, SendMessageA, DefWindowProcA, DestroyWindow, UpdateWindow + GDI 관련).

R3-12 closure 후 phase R4 의 후보:
- pSetWindowPos (게임 fullscreen toggle / borderless),
- pUpdateLayeredWindow (Game launcher overlay — Battle.net etc),
- pClipCursor (FPS 게임의 cursor 가두기),
- pUpdateDisplayDevices (resolution 변경).

## 부록 C — 본 project helper.mm 의 ip kind 표 (참조 용도)

helper.mm:25..47 의 17 kind 중 USER32/GDI32 관련:
- **kind 14 (user32_call)**: fn_id 1..23, 응답 size 가 fn_id 별로 가변. 자주 쓰이는:
  - 1=RegisterClassExA, 2=CreateWindowExA, 3=ShowWindow, 5=GetMessageA, 6=TranslateMessage, 7=DispatchMessageA, 8=PeekMessageA, 14=SendMessageA, 15=PostMessageA.
- **kind 15 (gdi32_call)**: fn_id 1..8 — fake handle stub (own1 § A 명시).
- **kind 17 (thread_sync_call)**: kernel32 thread sync — WaitForSingleObject 등 — message loop 와 간접 관련 (window message 처리 thread 가 다른 sync object 에 block 되면 GetMessage 진행 X).

helper.mm:1247 `handle_user32_call` 에서 fn_id 별 dispatch. 응답 layout:
- 일반: `<ack_status:int32><return_value:uint64>` (12 byte).
- fn_id=5 (GetMessageA): `<ack:int32><ret:uint64><Win32Msg(28)>` (40 byte).
- fn_id=7 (DispatchMessageA), fn_id=14 (SendMessageA): head + WndProcInvoke block (가변).

## 부록 D — 본 audit 의 한계

- Wine 8.0 (현 release branch) 기준. Wine 9.x / 10.x 의 변경사항 반영 X — 단 USER32 thin shim 패턴은 8.0 이후 안정.
- DDE (Dynamic Data Exchange) message (WM_DDE_*) 는 Wine 자체가 별도 DLL (dlls/user32/dde_*.c) 처리. 본 project 미사용 — 게임 거의 X.
- Hook (SetWindowsHookEx) 은 위 분석에서 path 만 언급 (WH_CALLWNDPROC). Wine 의 hook chain (win32u/hook.c) 자체 audit 은 별도 필요. 본 project: 미지원 (cheating tool 영역).
- DPI awareness (NTUSER_DPI_PER_MONITOR_AWARE) 는 win32u 전반에 흩어져 있음 (set_thread_dpi_awareness_context). 본 project: macOS 의 NSScreen.backingScaleFactor 와 Win32 GetDpiForMonitor 매핑 별도 phase.

## 부록 E — 다음 audit candidate

- `dlls/d3d11/`, `dlls/dxgi/` — D3D11 device 생성 / swapchain → MTL 변환 (R3 phase).
- `dlls/dinput8/` — DirectInput8 → IOKit HID 변환.
- `dlls/winmm/`, `dlls/dsound/` — multimedia → CoreAudio.
- `dlls/wininet/`, `dlls/ws2_32/` — Winsock → BSD socket (이미 macOS 호환 layer 존재).
- `dlls/ntdll/` — process / thread / module lifecycle (본 project 의 IPC kind 1/4 와 부분 비교 됨, 별도 audit 권고).
