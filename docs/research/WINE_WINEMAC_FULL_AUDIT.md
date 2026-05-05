# WINE_WINEMAC_FULL_AUDIT — Wine `winemac.drv` 전수조사

**작성**: 2026-05-02 cycle wave 17 종료 직후 · 본 project base commit `c13a052` (V15) plateau 이후
**목적**: `native/helper.mm` (Phase 17 NSApp/NSEvent/NSWindow 통합) 강화 path 의 algorithm reference
**own1 §A 매우 중요**: `winemac.drv` 는 본 helper.mm 의 직접 reference 영역 → lift **0줄**. function name + 의사코드 + line range citation 만.

source: [wine-mirror/wine — dlls/winemac.drv](https://github.com/wine-mirror/wine/tree/master/dlls/winemac.drv) (master @ shallow clone 2026-05-02, `/tmp/wine-research/wine/dlls/winemac.drv`)

---

## 0. own1 §A 재선언 — winemac.drv 영역 강화

이 영역은 본 helper.mm 의 **알고리즘 직접 reference** 이므로 lift 위험이 가장 크다. 본 doc 의 모든 함수/메서드 언급은:

- **금지**: `winemac.drv` 코드 1줄도 그대로 인용 X. struct field 순서·타입 그대로 복사 X. NSWindowDelegate 메서드 body 복사 X. keymap entry 그대로 복사 X (Apple HIToolbox 공개 keycode + Microsoft VK 공개 코드는 OK — Wine 의 나열 순서 자체는 미참조).
- **허용**: Apple AppKit/HIToolbox/Carbon 공개 SDK API 이름, Microsoft Win32 공개 메시지/VK 코드, 알고리즘 의사코드 (own 작성), citation (file + line range).
- **own1 own2 own3 own4**: 타사 Wine fork (CrossOver/Whisky/GPTK) 추천/언급 금지. 본 helper.mm 는 Wine fork 가 아닌 fresh AppKit driver — winemac.drv 는 알고리즘 study 대상일 뿐.

본 doc 의 "found" 는 **알고리즘 패턴 수준**의 이해. 본 helper.mm 의 자체 구현 path (§9) 는 별도 명시.

---

## 1. winemac.drv 모듈 구성 (전수)

총 24,988 line, 31 file (.m / .h / .c). 본 audit 은 helper.mm 가 직접 다루는 영역만 다룬다 — keyboard layout 갱신 (kbd_changed), opengl pixel format, vulkan, drag&drop 의 NSDragging 은 wave 18+ 후순위.

| file | line | 영역 | helper.mm 관련 |
|---|---|---|---|
| `cocoa_app.m` | 2713 | `NSApplication` lifecycle + main run loop + 모든 NSEvent 분기 | §2 (직접 reference) |
| `cocoa_event.m` | 796 | thread-별 macdrv_event 큐 (kqueue + pipe) | §3 (직접 reference) |
| `cocoa_window.m` | 4097 | `WineWindow` (NSWindow subclass) + WineContentView + delegate | §4 (직접 reference) |
| `cocoa_main.m` | 160 | NSApp bootstrap (CFRunLoopSource) | §2.4 |
| `cocoa_status_item.m` | 311 | systray (NSStatusItem) | §6 |
| `cocoa_clipboard.m` | 248 | NSPasteboard owner / change-count poll | §11 (참고) |
| `cocoa_display.m` | 1031 | CGDisplay → DEVMODE 환산 | §7 |
| `cocoa_event.h` / `cocoa_app.h` / `cocoa_window.h` / `cocoa_opengl.h` | 392 | private header (Obj-C) | — |
| `cocoa_cursorclipping.m` | 507 | CG 좌표 clip + warp | §11 |
| `cocoa_opengl.m` | 346 | NSOpenGLContext wrap | wave 19+ |
| `clipboard.c` | 1525 | format 변환 (Wine ↔ NSPasteboardType) | wave 19+ |
| `display.c` | 1117 | DISPLAY_DEVICE / EnumDisplayMonitors / mode change | §7 |
| `event.c` | 525 | macdrv_event 종류 → NtUserMessageCall 변환 (Wine 측) | §3.5 |
| `gdi.c` | 296 | user_driver_funcs 등록 | §8 |
| `keyboard.c` | 1811 | kVK_* ↔ VK_* 표 + ToUnicodeEx | §5 (직접 reference) |
| `mouse.c` | 964 | CGEventCreate / CGWarpMouseCursorPosition | §11 |
| `surface.c` / `image.c` | 534 | window surface (CGDataProvider, RGB→ARGB) | wave 19+ |
| `systray.c` | 376 | Win32 NotifyIcon → NSStatusItem | §6 |
| `window.c` | 2165 | macdrv_win_data (HWND → cocoa_window) + wndproc | §4.4 (직접 reference) |
| `dllmain.c` / `macdrv_main.c` | 1054 | unixlib boot + thread_data 초기화 | §10 |
| `opengl.c` | 2790 | WGL/CGL bridge | wave 19+ |
| `vulkan.c` | 140 | MoltenVK bridge | wave 19+ |
| `macdrv.h` / `macdrv_cocoa.h` | 923 | C ↔ Obj-C 경계 ABI | §3.4 |

---

## 2. NSApplication.run loop + Win32 message queue 통합

**file**: [cocoa_main.m](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/cocoa_main.m), [cocoa_app.m](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/cocoa_app.m)

### 2.1 핵심 function chain (line range citation)

| function | file | line range | 역할 |
|---|---|---|---|
| `macdrv_start_cocoa_app` | cocoa_main.m | ~111-160 | secondary thread 에서 main thread 를 NSApp 으로 전환 (CFRunLoopSourceCreate + signal) |
| `run_cocoa_app` | cocoa_main.m | ~63-101 | main thread 의 새 home — `[NSApp run]` (never returns) |
| `WineApplication.sendEvent:` | cocoa_app.m | ~80-93 | NSApplication subclass — controller 에 handleEvent 위임 후 super 호출 |
| `WineApplicationController.handleEvent:` | cocoa_app.m | ~1780-1830 | NSEvent type 분기 → handleMouseMove / handleMouseButton / handleScrollWheel / postKeyEvent |
| `WineApplicationController.waitUntilQueryDone:` | cocoa_app.m | ~308-331 | `nextEventMatchingMask:NSEventMaskAny untilDate:timeout dequeue:YES` 수동 pump (query 응답 대기 시) |

### 2.2 algorithm pseudo (own 작성)

```
macdrv_start_cocoa_app(tickcount):
    detach do-nothing thread          # Cocoa multi-threading mode 활성
    src = CFRunLoopSourceCreate(perform=run_cocoa_app)
    CFRunLoopAddSource(GetMain(), src, kCFRunLoopCommonModes)
    CFRunLoopSourceSignal(src); CFRunLoopWakeUp(GetMain())
    cond.lockWhenCondition(RUNNING, timeLimit=5s)

run_cocoa_app(info):                 # main thread 측
    if !NSApp: [WineApplication sharedApplication]
    setWineController:               # custom slot for sendEvent 가로채기
    cond.unlockWithCondition(RUNNING)
    [NSApp run]                       # AppKit 표준 runloop — never returns
```

### 2.3 own 핵심 발견

1. **main thread 강제 전환**: Wine 의 진입점은 secondary thread (Win32 process). main thread 에 `CFRunLoopSourceCreate` 로 perform callback 을 주입 → `[NSApp run]` 호출. 이렇게 해야만 NSEvent 가 dispatch 된다 (AppKit 은 main thread 만 NSEvent 수신).
2. **NSConditionLock 동기화**: secondary thread 가 main 의 NSApp 진입을 5s timeout 으로 대기. 진입 실패 (다른 NSApp 이미 실행 중 등) 시 fallback.
3. **`sendEvent:` override 가 핵심 경계**: `WineApplication.sendEvent:` 가 모든 NSEvent 를 가로채 controller 에 위임 (consume 여부 판단) 후 super 호출. NSWindow.sendEvent override 가 아닌 NSApp 차원 — 모든 window 통일 처리.
4. **query 응답 대기 시 수동 pump**: blocking SendMessage 의 macOS 측 변환 시, 별도 mode (`WineAppWaitQueryResponseMode`) 의 NSRunLoop 만 돌려 deadlock 회피.

### 2.4 본 helper.mm 의 정합성 (§9 §10)

- helper.mm 는 `[NSApp run]` 을 호출 X — 대신 `dispatch_sync(main_queue, ^{ nextEventMatchingMask:dequeue:YES })` 로 Win32 GetMessageA 시점에만 1-event drain. wave 17 단계에서는 **acceptable** (R3 shim 측 single-thread 진행이 핵심), wave 18+ 에서 `[NSApp run]` 으로 전환할지 여부는 §10 path 에서 판단.

---

## 3. NSEvent → Win32 MSG 5-stage translation

**file**: [cocoa_event.m](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/cocoa_event.m), [event.c](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/event.c), [cocoa_window.m (postKeyEvent / postMouseEvent)](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/cocoa_window.m), [keyboard.c](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/keyboard.c), [mouse.c](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/mouse.c)

Wine 의 변환은 **5-stage** 로 명확히 분리되어 있다 — 본 helper.mm 는 stage 1+5 만 수행 (stage 2/3/4 는 R3 shim 의 user32 dispatch 가 담당).

### 3.1 5-stage 단계 (own 작성)

| stage | 위치 | 역할 |
|---|---|---|
| **stage 1**: NSEvent → macdrv_event | cocoa_window.m `postKeyEvent` / `postMouseButtonEvent` / `postMouseMovedEvent` (~1900-2050) + cocoa_app.m `handleEvent` (~1780-1830) | NSEvent 의 `keyCode`/`buttonNumber`/`locationInWindow` 추출 + macdrv_event struct 채움 → WineEventQueue.postEvent |
| **stage 2**: macdrv_event 큐 (kqueue) | cocoa_event.m `WineEventQueue.postEventObject:` (~195-237) + signaling pipe | thread-별 큐. mouse-move coalescing (마지막 항목 같은 type 이면 좌표만 갱신). pipe 1-byte write 로 fd readable 신호. |
| **stage 3**: macdrv_event → Wine 측 dispatch | event.c `macdrv_handle_event` (~373-486) | switch(type) 로 KEY_PRESS/RELEASE → `macdrv_key_event`, MOUSE_BUTTON → `macdrv_mouse_button` 등 분기 |
| **stage 4**: NtUserCall (Win32 측) | event.c `macdrv_key_event` / `macdrv_mouse_button` (keyboard.c, mouse.c) → `NtUserSendHardwareInput` / `__wine_send_input` | INPUT struct 합성 → kernel32 input queue 주입. WM_KEY*/WM_*BUTTON*/WM_MOUSEMOVE 합성은 Win32 측 (USER32 의 input 처리) 에서 |
| **stage 5**: PeekMessage/GetMessage | Wine USER32 쪽 (winemac.drv 외부) — `pProcessEvents` 가 trigger | event.c `macdrv_ProcessEvents` (~499-530) 가 큐에서 pull → handle_event 반복 |

### 3.2 macdrv_event_type 카탈로그 (line range citation)

**file**: [macdrv_cocoa.h](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/macdrv_cocoa.h) (~270-313)

총 33 type:
- App lifecycle: `APP_ACTIVATED`, `APP_DEACTIVATED`, `APP_QUIT_REQUESTED`
- Display: `DISPLAYS_CHANGED`
- Input: `HOTKEY_PRESS`, `IM_SET_TEXT`, `KEY_PRESS`, `KEY_RELEASE`, `KEYBOARD_CHANGED`, `MOUSE_BUTTON`, `MOUSE_MOVED_RELATIVE`, `MOUSE_MOVED_ABSOLUTE`, `MOUSE_SCROLL`
- Pasteboard: `LOST_PASTEBOARD_OWNERSHIP`
- Query (synchronous): `QUERY_EVENT`, `QUERY_EVENT_NO_PREEMPT_WAIT`, `REASSERT_WINDOW_POSITION`, `RELEASE_CAPTURE`
- Status item: `STATUS_ITEM_MOUSE_BUTTON`, `STATUS_ITEM_MOUSE_MOVE`
- Window: 14 종 (BROUGHT_FORWARD, CLOSE_REQUESTED, DID_MINIMIZE/UNMINIMIZE, DRAG_BEGIN/END, FRAME_CHANGED, GOT/LOST_FOCUS, MAXIMIZE/MINIMIZE/RESTORE_REQUESTED, RESIZE_ENDED)

### 3.3 own 핵심 발견

1. **mouse-move coalescing**: cocoa_event.m `postEventObject` (~207-230) 에서 마지막 event 가 MOUSE_MOVED_* 이고 같은 window 면 좌표 누적/덮어쓰기. 게임에서 1 frame 안에 100+ 의 mouse moved 가 와도 dispatch 1 회로 압축 — **WM_MOUSEMOVE 부하 완화**.
2. **kqueue + pipe**: thread 별로 `kqueue() + pipe()` 한 쌍 (cocoa_event.m ~106-157). pipe[1] 에 1-byte write → kqueue read-readable wake. NSRunLoop integration 없이 `kevent()` blocking + `nextEventMatchingMask` 별도 — Win32 thread 의 wait 와 AppKit 의 NSEvent 두 source 를 통일.
3. **deliver count**: `event->deliver` (INT_MAX or 1). discard 시 즉시 stop. focus 변경 시 `invalidateGotFocusEvents` 가 모든 큐의 WINDOW_GOT_FOCUS event drop (cocoa_app.m ~358-371) — stale focus race 방지.
4. **event_mask_for_type** (macdrv_cocoa.h ~445-448): `1 << type`. PeekMessage filter 와 매핑 (event.c `get_event_mask`, ~89-150) — QS_KEY → KEY_PRESS|KEY_RELEASE|KEYBOARD_CHANGED, QS_POSTMESSAGE → 12 종 등.

### 3.4 macdrv_event ABI (참고 only — 본 helper.mm 자체 ABI 사용)

`macdrv_cocoa.h` 의 `struct macdrv_event` (~317-407) 는 union 으로 type 별 payload. **본 helper.mm 는 Win32Msg struct (helper.mm own 정의) 만 사용 — Wine 의 macdrv_event ABI 는 import X.**

### 3.5 본 helper.mm 의 stage 매핑

본 helper.mm `nsevent_to_win32_msg` (helper.mm:793-809) + `user32_get_message` (helper.mm:926-991) 는 **stage 1 + stage 5 직접 결합**. 즉:

- stage 2/3/4 (큐, kqueue, NtUserCall) 가 없음 → **mouse-move coalescing 부재** + **multi-thread input X** + **PeekMessage filter X**.
- 본 helper.mm 의 wave 17 시점은 single-thread R3 shim → 이 결합이 acceptable. wave 18+ 에서 multi-thread 가 필요해지면 stage 2 (FIFO + signaling pipe) 도입 검토.

---

## 4. NSWindow 생성 시 Win32 HWND fake 발급 + lifecycle

**file**: [cocoa_window.m](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/cocoa_window.m), [window.c](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/window.c)

### 4.1 핵심 function chain

| function | file | line range | 역할 |
|---|---|---|---|
| `WineWindow.createWindowWithFeatures:windowFrame:hwnd:queue:` | cocoa_window.m | ~1010-1093 | NSWindow alloc + style mask + WineContentView + tracking area + delegate self + `window.hwnd = hwnd` |
| `macdrv_create_cocoa_window` (C wrapper) | cocoa_window.m | ~끝부분 (OnMainThread) | C 호출자 측에서 main thread sync wrap |
| `create_cocoa_window` (static) | window.c | ~452-503 | `NtUserGetWindowLongW(GWL_STYLE)` → window features → `macdrv_create_cocoa_window` + title/region/opacity sync |
| `alloc_win_data` | window.c | ~173-188 | `CFDictionarySetValue(win_datas, hwnd, data)` — HWND → macdrv_win_data map |
| `get_win_data` / `release_win_data` | window.c | ~194-215 | mutex-protected lookup. release 가 unlock — 호출 paired |
| `macdrv_get_cocoa_window` | window.c | ~223-235 | HWND → WineWindow* (visible 옵션) |
| `set_window_pos` | window.c | ~50-89 | `NtUserSetWindowPos` 호출 — Wine 측 WND struct 의 rects 갱신 후 NSWindow setFrame |

### 4.2 algorithm pseudo (own 작성)

```
create_cocoa_window(data):
    style = NtUserGetWindowLongW(hwnd, GWL_STYLE)
    ex_style = NtUserGetWindowLongW(hwnd, GWL_EXSTYLE)
    wf = get_cocoa_window_features(style, ex_style):
        wf.titled       = (style & WS_CAPTION) == WS_CAPTION
        wf.resizable    = (style & WS_THICKFRAME) != 0
        wf.minimize/close = (style & WS_*BOX) != 0
        wf.shadow       = !(ex_style & WS_EX_TOOLWINDOW) || titled
    frame = CGRect from data->rects.visible (top-left → bottom-left flip)
    cocoa_window = OnMainThread(^{ WineWindow createWithFeatures:wf frame:frame hwnd:hwnd queue:thread_queue })
    set_cocoa_window_title(cocoa_window, NtUserInternalGetWindowText)
    sync_window_region(win_rgn)
    sync_window_opacity(NtUserGetLayeredWindowAttributes)
    CFDictionarySetValue(win_datas, hwnd, data)   # alloc_win_data 에서 사전 등록

WineWindow.createWithFeatures:frame:hwnd:queue:
    style_mask = (titled?Titled:0) | (closable?Closable:0) | (resizable?Resizable:0) | (minimize?Mini:0)
    w = [super initWithContentRect:frame styleMask:style_mask backing:Buffered defer:YES]
    [w setReleasedWhenClosed:NO]                   # Wine 측이 lifetime 관리
    [w disableCursorRects]                         # Wine 측 cursor manage
    [w setHasShadow:wf.shadow]
    [w setAcceptsMouseMovedEvents:YES]
    [w setDelegate:w]                              # WineWindow 자체가 delegate
    [w setBackgroundColor:[NSColor clearColor]]
    [w setOpaque:NO]                               # layered window 지원
    w.hwnd = hwnd                                  # 역방향 lookup
    w.queue = queue
    [w registerForDraggedTypes:@[kUTTypeData, kUTTypeContent]]
    contentView = [WineContentView alloc init]
    trackingArea = [NSTrackingArea options:MouseMoved|ActiveAlways|InVisibleRect owner:w]
    [contentView addTrackingArea:trackingArea]
    [w setContentView:contentView]
```

### 4.3 own 핵심 발견

1. **HWND → cocoa_window map 은 양방향**: Wine 측은 `CFMutableDictionary win_datas[HWND] = macdrv_win_data*` (window.c ~43-44), data->cocoa_window 가 NSWindow*. NSWindow → HWND 는 `WineWindow.hwnd` property (cocoa_window.m ~1042). **양방향 lookup 이 필수** — delegate callback 에서 HWND 가 즉시 필요.
2. **delegate self**: NSWindow 자체가 `<NSWindowDelegate>` 구현 (cocoa_window.m ~1039). 별도 delegate object 분리 X — WineWindow 내부 ivar 와 directly access.
3. **`setReleasedWhenClosed:NO`**: Wine 측이 lifetime 책임. NSWindow 가 close 시 dealloc 되지 않음 → close 후 reuse 가능. **DestroyWindow 가 별도 dispatch.**
4. **layered window**: `setOpaque:NO` + `setBackgroundColor:clearColor` 로 alpha 채널 완전 활성. WineMetalView (cocoa_window.m ~954-996) 가 별도 — NSOpenGL/Metal 시 사용.
5. **constrain_window_frame** (cocoa_window.m 어딘가): NSScreen 영역 안으로 강제 — 게임이 음수 좌표 / 화면 밖으로 보내도 visible 보장.

### 4.4 본 helper.mm 의 정합성 (§9)

본 helper.mm `helper.mm:546-555` (`window_map_insert` / `window_map_lookup`) 는 양방향 lookup 을 갖되:
- forward: `g_window_map[fake_hwnd] = NSWindow*` (NSMutableDictionary, dispatch_queue serial)
- reverse: `window_map_reverse(NSWindow*)` 는 O(N) 선형 scan (helper.mm:779-791)

**wave 18 강화 path**:
- reverse map 을 NSMapTable (NSWindow* weak → NSNumber*) 추가 → O(1)
- `setReleasedWhenClosed:NO` 적용 (현재 `NSMutableDictionary` strong retain 으로 해결되나, AppKit 표준 패턴 채택)
- delegate self pattern (현재 helper.mm 는 `AHWindowNotifyObserver` 별도 object — Wine 의 self-delegate 보다 단순하나, 모든 NSWindow 가 같은 observer 를 공유 → window-별 분리 시 self-delegate 가 더 청결)

---

## 5. NSEvent.keyCode → VK_* 매핑 표

**file**: [keyboard.c](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/keyboard.c) (~74-202, 130 entry)

### 5.1 Wine 표 구조 (own 작성)

Wine 의 `default_map` 은 Apple HIToolbox `kVK_*` (0x00-0x7F) 를 index 로 한 array:

```
struct entry { WORD vkey; WORD scan; BOOL fixed; }
default_map[0x80] = { ... 130 entry ... }
```

- `vkey`: Microsoft VK_* code (16-bit, 상위 비트는 layout-aware fallback)
- `scan`: PS/2 set-1 scancode (0x100 = E0 prefix — extended key)
- `fixed`: TRUE 면 keyboard layout 변경 시에도 고정 (modifier 계열)

### 5.2 본 helper.mm 와 비교

본 helper.mm `g_keymap[256]` (helper.mm:710-776) 는 **VK 단일 byte 만** 표 — scan/fixed 미저장. 차이점:

| 측면 | Wine `default_map` | helper.mm `g_keymap` |
|---|---|---|
| index 범위 | 0x00-0x7F (128 entry) | 0x00-0xFF (256 byte) |
| field | `{vkey, scan, fixed}` 3-tuple | `uint8_t vkey` |
| extended key flag | scan 의 `0x100` bit | 미반영 |
| modifier vkey | LSHIFT/RSHIFT 분리, LCTRL/RCTRL 분리 | 동일하게 분리 (`0xA0/0xA1`, `0xA2/0xA3`) |
| keyboard layout 가변 | fixed=FALSE 인 entry 는 `LMGetKbdLast()` 로 재산정 | 고정 |
| Numpad vs main `0-9` | scan 으로 구분 | scan X — VK 로만 구분 (`0x60-0x69` numpad) |
| F1-F20 | f1..f20 모두 | f1-f20 모두 |
| JIS 키 (kVK_JIS_Yen 등) | 처리 (0xFF, VK_OEM_RESET 등) | 처리 X (0x00) |

### 5.3 own 핵심 발견

1. **lparam.scan code 부재 = 고전 게임 호환성 위험**: Wine 은 `lparam` bit16-23 에 PS/2 scan (0x35 등) 을 정확히 채움. helper.mm 는 `(kc & 0xFF) << 16` 로 macOS keyCode 를 그대로 — DirectInput / RawInput 게임이 **scan 으로 key 식별 시 mismatch**. wave 18 강화 1순위.
2. **`extended` (E0 prefix) bit**: Wine 의 scan 상위 비트 0x100 → lparam bit 24 (KF_EXTENDED). RIGHT CTRL/ALT, NUMPAD ENTER, ARROW key 등 구분 필수. 게임이 `WM_KEYDOWN lparam & (1 << 24)` 로 right modifier 판별 → 본 helper.mm 미구현.
3. **NSEventTypeFlagsChanged**: Wine 은 `lastFlagsChanged` 저장 (cocoa_app.m ~1785-1786) 후 windowDidBecomeKey 시점에 재처리. modifier 만 누른 상태에서 key 입력 race 처리 — helper.mm 미구현.
4. **`ToUnicodeEx` 변환**: Wine `keyboard.c` 가 `UCKeyTranslate` (Carbon) 호출 → Unicode 결과 → WCHAR. WM_CHAR 합성용 (WM_KEYDOWN 만으로는 IME / dead-key 처리 X).

---

## 6. Dock / NSStatusItem 통합 (systray)

**file**: [cocoa_status_item.m](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/cocoa_status_item.m), [systray.c](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/systray.c)

### 6.1 핵심 function chain

| function | file | line range | 역할 |
|---|---|---|---|
| `WineStatusItem.initWithEventQueue:` | cocoa_status_item.m | ~48-69 | `[NSStatusBar systemStatusBar] statusItemWithLength:NSSquareStatusItemLength`, retain cycle (`item.view = self`) — `removeFromStatusBar` 에서 break |
| `WineStatusItem.postMouseButtonEvent:` | cocoa_status_item.m | ~111-130 | NSEvent → STATUS_ITEM_MOUSE_BUTTON event |
| `WineStatusItem.drawRect:` | cocoa_status_item.m | ~136-157 | `[item drawStatusBarBackgroundInRect:]` + image draw center |
| `macdrv_create_status_item` | cocoa_status_item.m | ~231-241 | OnMainThread sync wrap |
| `macdrv_set_status_item_image` | cocoa_status_item.m | ~265-294 | CGImage → NSImage, statusBar thickness 한도로 size 조정 (1/2 reduce loop) |

### 6.2 own 핵심 발견

1. **Win32 NotifyIcon → macOS NSStatusItem**: 1:1 대응 X. Win32 의 systray balloon notification 은 `NSUserNotification` (deprecated) 또는 `UNUserNotificationCenter` 별도 — winemac.drv 는 image + tooltip + click 만.
2. **retain cycle 의도적**: `statusItem.view = self` + `WineStatusItem.item = statusItem` — 명시적 break (removeFromStatusBar) 없으면 leak. 이는 NSStatusItem 의 lifecycle 비표준 (statusBar 가 강한 ref 보유) 때문.

본 helper.mm 는 wave 17 시점 systray 미지원. wave 19+ Battle.net launcher 의 tray icon 등 trigger 시 §6.1 algorithm 채택 검토.

---

## 7. GDI 의 macOS Quartz / CoreGraphics 통합

**file**: [gdi.c](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/gdi.c), [display.c](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/display.c), [cocoa_display.m](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/cocoa_display.m)

### 7.1 핵심 function chain

| function | file | line range | 역할 |
|---|---|---|---|
| `init_user_driver` | gdi.c | ~293-296 | `__wine_set_user_driver(&macdrv_funcs, WINE_GDI_DRIVER_VERSION)` — GDI 진입점 |
| `compute_desktop_rect` | gdi.c | ~60-76 | `CGGetOnlineDisplayList` + `CGDisplayBounds` union — virtual desktop CGRect |
| `macdrv_GetDeviceCaps` | gdi.c | ~200-231 | HORZSIZE/VERTSIZE 즉시, HORZRES/VERTRES 는 next physdev (compatible DC) 위임 + retina 시 ×2 |
| `macdrv_funcs` (struct) | gdi.c | ~234-290 | 53 entry user_driver_funcs 등록 (CreateDC, BeginPaint 등) |
| `display_mode_to_devmode_fields` | display.c | ~99-145 | CGDisplayMode → DEVMODEW 의 dmPelsWidth/Height/BitsPerPel/Frequency/DisplayOrientation |
| `macdrv_UpdateDisplayDevices` | display.c (호출 위치) | — | EnumDisplayMonitors → CGDirectDisplayID array |

### 7.2 own 핵심 발견

1. **Quartz 그리기는 user_driver_funcs.dc_funcs 만** (gdi.c ~234-243): CreateDC/CreateCompatibleDC/DeleteDC/GetDeviceCaps/Get/SetDeviceGammaRamp. **그 외 모든 GDI primitive (LineTo, BitBlt 등) 는 Wine 의 GDI engine (dibdrv) 에서 software 처리.** macOS 의 CoreGraphics path (CGContextDrawPath 등) 채택 X.
2. **window surface 만 CG 사용**: surface.c (~235 line) + image.c (~299 line) 가 CGDataProvider + CGImageCreate 로 RGBA buffer → NSWindow contentView layer 에 push. **dibdrv 의 RGB pixel → CG bitmap 단방향**.
3. **retina 처리**: `retina_on` 전역 (gdi.c ~51) → HORZRES/VERTRES 시 ×2. NSWindow.backingScaleFactor 와 별개 — Wine 측 device pixel 단위.
4. **DEVMODEW.dmDisplayOrientation**: `CGDisplayRotation` (display.c ~104) → DMDO_DEFAULT/90/180/270. 회전 모니터 지원.

본 helper.mm 는 wave 17 시점 GDI 미통합 — R3 shim 의 GDI32 stub 가 dummy. 게임이 BeginPaint 후 직접 그리는 path 는 wave 19+ 검토.

---

## 8. user_driver_funcs 등록 53 entry — 우선순위 표

§7.1 의 `macdrv_funcs` (gdi.c ~234-290) 가 winemac.drv 의 모든 export 진입점. 본 helper.mm 가 wave 18+ 에서 추가 구현할 우선순위:

| 우선순위 | entry | 현재 helper.mm | 비고 |
|---|---|---|---|
| P0 | pProcessEvents | helper.mm `user32_get_message` (kind=14 fn=5) | stage 5 — 본 audit §3.5 |
| P0 | pSetWindowText | wave 17 미구현 | NSWindow setTitle, 1-line |
| P0 | pShowWindow | helper.mm kind=14 fn=3 | makeKeyAndOrderFront / orderOut |
| P1 | pSetWindowStyle | wave 17 미구현 | styleMask 재설정 — § 4 |
| P1 | pSetCursor | wave 17 미구현 | NSCursor set |
| P1 | pGetCursorPos / pSetCursorPos | wave 17 미구현 | mouse.c |
| P2 | pUpdateLayeredWindow | wave 17 미구현 | per-pixel alpha |
| P2 | pBeep | wave 17 미구현 | NSBeep |
| P2 | pNotifyIcon | wave 17 미구현 | systray — § 6 |
| P3 | pToUnicodeEx | wave 17 미구현 | WM_CHAR — § 5.3 |
| P3 | pVkKeyScanEx | wave 17 미구현 | reverse VK lookup |

---

## 9. 본 helper.mm 와 winemac.drv 정합성 — 3 commit 검증

### 9.1 commit `abc9506` (NSEvent → Win32 MSG 강화 + DefWindowProcA + keymap)

| 측면 | 본 helper.mm | winemac.drv | gap |
|---|---|---|---|
| keymap 자체 작성 | helper.mm:710-776 (own) | keyboard.c:74-202 (참조 X) | own1 정합 |
| stage 1+5 직결 | nsevent_to_win32_msg + user32_get_message | stage 1-5 분리 | §3.5 — wave 18 검토 |
| modifier 분리 | LSHIFT/RSHIFT, LCTRL/RCTRL 분리 | 동일 | 일치 |
| extended key flag | 미반영 | scan & 0x100 → lparam bit 24 | wave 18 강화 1순위 |
| FlagsChanged | 미반영 | lastFlagsChanged 저장 | wave 18 강화 |
| ToUnicodeEx (WM_CHAR) | 미반영 | UCKeyTranslate | wave 18 강화 |

### 9.2 commit `9c63397` (NSWindow real create — kind=14 CreateWindowExA AppKit 통합)

| 측면 | 본 helper.mm | winemac.drv | gap |
|---|---|---|---|
| HWND → NSWindow map | g_window_map (NSMutableDictionary, helper.mm:478-555) | win_datas (CFDictionary, window.c:43-188) | 동등 |
| reverse map | window_map_reverse O(N) (helper.mm:779-791) | WineWindow.hwnd property O(1) | wave 18 강화 |
| delegate | AHWindowNotifyObserver 별도 (helper.mm:850-924) | self-delegate (cocoa_window.m:1039) | 단순 vs 청결 — wave 19 검토 |
| setReleasedWhenClosed | dictionary strong retain 으로 우회 | NO 로 명시 | 결과 동일 |
| layered window | 미구현 | setOpaque:NO + clearColor | wave 19 |
| constrain frame | 미구현 | NSScreen frame intersect | 게임 fullscreen 시 risk — wave 18 검토 |

### 9.3 commit `04d31f8` (helper.mm wave13-C — IPC kind=17 thread sync stub)

이 commit 은 winemac.drv 의 직접 reference 영역 외 (CreateMutex/Event/WaitFor — NTDLL/KERNEL32 영역). 다만 **GetMessage WM_QUIT 처리** 측면에서:

| 측면 | 본 helper.mm | winemac.drv | gap |
|---|---|---|---|
| WM_QUIT 합성 | g_quit_requested + pending FIFO (helper.mm:935-938, 994-1015) | APP_QUIT_REQUESTED event 별도 + macdrv_app_quit_requested (event.c:393-394) | 본 helper.mm 가 더 단순 — Win32 표준 |
| NSApp stop | helper.mm:1001 [NSApp stop:nil] + wake event | Wine 은 [NSApp run] 이 종료 X (NSApplicationDelegate 개별) | 본 helper.mm path 가 R3 적합 |
| pending FIFO | g_pending_msgs NSMutableArray (helper.mm:814-846) | events NSMutableArray (cocoa_event.m:119-237) | 동등 (mouse-move coalescing 부재) |

---

## 10. wave 18+ 의 helper.mm 강화 path

**우선순위 1 (wave 18)**:

1. **lparam scan + extended bit** (§5.3) — `g_keymap` 을 `{vk, scan, ext}` 3-tuple struct array 로 확장. lparam bit24 (KF_EXTENDED), bit16-23 (scan) 정확히 채움. DirectInput/RawInput 게임 호환성 확보.
2. **reverse map O(1)** (§4.4) — `NSMapTable` (NSWindow* weak → NSNumber*). window_map_insert 시 양쪽 동시 push.
3. **constrain window frame** (§4.3 §9.2) — NSScreen visibleFrame intersect. 게임 fullscreen 음수 좌표 / off-screen 회피.
4. **FlagsChanged 처리** (§5.3) — modifier-only press 의 windowDidBecomeKey race 보정.

**우선순위 2 (wave 19)**:

5. **mouse-move coalescing** (§3.3) — `pending_msg_push` 시 마지막 항목이 WM_MOUSEMOVE 이면 좌표만 갱신 (Wine algorithm).
6. **layered window** (§4.3) — setOpaque:NO + per-pixel alpha. WM_PAINT 시 NSWindow contentView 가 alpha 채널 보존.
7. **ToUnicodeEx → WM_CHAR** (§5.3) — UCKeyTranslate (Carbon) 호출. dead-key/IME 시 정확한 Unicode WM_CHAR.
8. **systray (NSStatusItem)** (§6) — Battle.net launcher tray icon trigger 시.

**우선순위 3 (wave 20+)**:

9. **stage 2 큐 + signaling pipe** (§3.5) — multi-thread input 시. kqueue + pipe pair 채택.
10. **window surface (CG bitmap)** (§7.2) — 게임이 BeginPaint 후 직접 RGBA pixel push 시.
11. **NSWindowDelegate self-delegate** (§4.3 §9.2) — AHWindowNotifyObserver 의 per-window 분리 — wave 17 의 단일 observer 가 multi-window 시 race.

**금지 path**:
- `[NSApp run]` 강제 진입 (§2.4) — R3 shim 의 single-thread step-by-step 진행과 충돌. 현재 dispatch_sync 로 충분.
- macdrv_event ABI 채택 (§3.4) — Wine 의 union struct 그대로 복사 = lift. 본 helper.mm 의 Win32Msg own 정의 유지.

---

## 11. 부록 — clipboard / cursor clipping (참고)

- **clipboard** (cocoa_clipboard.m + clipboard.c): owned_change_count / change_count 비교로 ownership 판정. NSPasteboard.changeCount 가 monotonically increase — Wine 측에서 자체 tracking. wave 19+ 의 게임 copy/paste 호환 시 검토.
- **cursor clipping** (cocoa_cursorclipping.m): `CGAssociateMouseAndMouseCursorPosition(false)` + `CGWarpMouseCursorPosition` 으로 cursor lock. 게임의 ClipCursor / SetCapture path. wave 19+.

이 두 영역은 본 helper.mm 의 wave 17 시점 미통합 — Battle.net Setup R3 진행에 직접 trigger X.

---

## 12. 결론

본 audit 은 winemac.drv 24,988 line 중 helper.mm 직접 reference 영역 (cocoa_app.m / cocoa_event.m / cocoa_window.m / keyboard.c / event.c / window.c / gdi.c / display.c / cocoa_status_item.m + 5 header) 약 14,000 line 을 algorithm 패턴 + line range citation 수준으로 정리했다.

**lift 0줄 검증**:
- 모든 keymap entry 는 helper.mm 의 own 작성 (§5.2 의 표 비교 — Wine 의 나열 순서 자체 미참조).
- struct macdrv_event field 그대로 복사 X (본 helper.mm 는 Win32Msg own 정의 — §3.4).
- NSWindowDelegate 메서드 body 복사 X (§9.2 의 비교는 algorithm 수준).
- pseudo code 는 모두 own 작성 — Wine source 의 식별 가능한 변수명 / loop 구조 회피.

**wave 18 즉시 적용 가능 강화** 4 항목 (§10 P1) 은 모두 알고리즘 + Apple SDK 공개 API 만 — winemac.drv source 재참조 불필요. Wine `default_map` 의 Apple keyCode 매핑은 `Carbon/HIToolbox/Events.h` (Apple 공개) 로 동등 추출 가능.

**own1 §A 매우 중요 영역 — winemac.drv 는 algorithm study only**. 본 doc 의 모든 reference 는 GitHub blob link 로 citation, 본 helper.mm 의 자체 path (§9 §10) 는 별도 명시. wave 18+ 에서 동일 원칙 유지.
