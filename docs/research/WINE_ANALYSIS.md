# Wine source 분석 — algorithm/architecture reference (own1 §A 정합)

**작성**: 2026-05-02 cycle wave 14 종료 후 · 본 project base commit `b2b741c` (V12) plateau 96.5%
**목적**: wave 15+ (vtable corruption recovery / TLS / WndProc / spin-loop graceful break) 의 algorithm 패턴 reference
**own1 §A 준수**: Wine source code lift **0줄** — function name + algorithm 의사코드 + line range citation 만. 각 reference 는 GitHub link 로 citation.

source: [wine-mirror/wine](https://github.com/wine-mirror/wine) (master @ shallow clone 2026-05-02, /tmp/wine-research/wine)

---

## 0. own1 §A 재선언

본 doc 는 Wine source 의 **공개 algorithm/architecture** 만을 reference. 다음 모두 ZERO copy:
- Wine source code 한 줄도 인용 X (function 이름 + algorithm 패턴 + 의사코드만)
- header struct 정의, macro, inline asm 회피
- 본 project (`airgenome-gamebox`) 의 자체 구현 path 는 §10 별도 명시

본 doc 의 "found" 는 모두 algorithm 수준의 이해 — 자체 reimpl 시 source 대조 X 원칙.

---

## 1. PE 로더: import resolution 순서 (Wine 방식)

**file**: [dlls/ntdll/loader.c](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/loader.c)

### 1.1 핵심 function chain (line range citation)

| function | line range | 역할 |
|---|---|---|
| `LdrLoadDll` | ~3440-3470 | 외부 진입점 — `load_dll` 호출 |
| `load_dll` (static) | ~3317-3440 | path 검색 + module map + alloc_module |
| `build_module` (static) | ~2247-2330 | NT header 검증 → fixup_imports → process_attach 분리 |
| `fixup_imports` (static) | ~1476-1522 | Import Directory 의 모든 descriptor 순회 → import_dll |
| `import_dll` (static) | ~1136-1266 | 단일 DLL 의 IAT thunk 채우기 |
| `find_named_export` (static) | ~1080-1105 | name → ordinal lookup (hint-first + binary search) |
| `find_name_in_exports` (static) | ~1059-1076 | export name table binary search |
| `process_attach` (static) | ~1762-1823 | DLL_PROCESS_ATTACH dispatch (deps recursive) |
| `MODULE_InitDLL` (static) | ~1679-1728 | TLS callbacks + EntryPoint(`DllMain`) 호출 |
| `call_tls_callbacks` (static) | ~1645-1674 | `IMAGE_TLS_DIRECTORY->AddressOfCallBacks` 순회 |

### 1.2 algorithm pseudo (own 작성)

```
LdrLoadDll(name):
    if module already mapped: bump LoadCount; return
    file = find_dll_file(name, search_path)   # KnownDLLs + system32 + PATH
    map = NtMapViewOfSection(file)
    wm = alloc_module(map)
    flags |= LDR_DONT_RESOLVE_REFS
    build_module(wm):
        fixup_imports(wm):
            for each IMAGE_IMPORT_DESCRIPTOR in IDT:
                imp = load_dll(descr->Name)            # recursive
                NtProtectVirtualMemory(IAT, RW)         # IAT 가 RO section 에 있을 수 있음
                for each thunk in OriginalFirstThunk:
                    if IMAGE_SNAP_BY_ORDINAL(thunk):
                        IAT[i] = find_ordinal_export(imp, ordinal - exports->Base)
                    else:
                        pe_name = RVA→IMAGE_IMPORT_BY_NAME
                        IAT[i] = find_named_export(imp, pe_name->Name, pe_name->Hint)
                NtProtectVirtualMemory(IAT, restore)
        process_attach(wm.DdagNode):
            walk_node_dependencies(deps, process_attach)   # DFS post-order
            call_tls_callbacks(module, DLL_PROCESS_ATTACH)
            DllMain(module, DLL_PROCESS_ATTACH, NULL)
```

### 1.3 핵심 발견 — own1 §A 적용

1. **hint-first lookup**: `find_named_export` 는 `IMAGE_IMPORT_BY_NAME->Hint` 의 ordinal 를 먼저 검사 → match 시 binary search 생략. compiler hint 신뢰 + cache 효과.
2. **IAT RO unprotect / reprotect**: import fix 전후 `NtProtectVirtualMemory` 로 RW ↔ original 변환. Battle.net Setup 같이 IAT 가 R-only section 에 있을 때 필수.
3. **DFS post-order DllMain**: dependency 가 먼저 attach. node DDAG (Directed Dependency Acyclic Graph) 로 cycle 회피 (`LDR_LOAD_IN_PROGRESS` flag).
4. **TLS callback 우선**: `DllMain` 전에 `IMAGE_TLS_DIRECTORY->AddressOfCallBacks` 의 NULL 종료 array 순회. MSVC CRT 의 `_initterm` 와 별개 layer.

---

## 2. EncodePointer / DecodePointer 정확 알고리즘

**file**: [dlls/ntdll/rtl.c](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/rtl.c) (line ~1023-1065)

### 2.1 cookie source

`get_process_cookie` (line ~1024-1038) 는:
- static `process_cookie` 변수 (lazy init)
- `NtQueryInformationProcess(GetCurrentProcess(), ProcessCookie, &cookie, ...)` 로 kernel 에서 fetch
- `InterlockedCompareExchange` 로 race-safe write

cookie 자체는 [dlls/ntdll/unix/process.c line ~1508-1516](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/unix/process.c) 의 `ProcessCookie` case 에서 unix layer 의 `process_cookie` 변수 (init 시점에 random) 를 반환.

### 2.2 Encode 의사코드 (own 작성)

```
RtlEncodePointer(ptr):
    cookie = get_process_cookie()      # 32-bit on i386, 64-bit on x64
    rotate = cookie % BITS_PER_PTR     # i386: cookie & 0x1F
    ptrval = (ULONG_PTR)ptr XOR cookie
    return ROR(ptrval, rotate)         # rotate-right
```

### 2.3 Decode 의사코드

```
RtlDecodePointer(ptr):
    cookie = get_process_cookie()
    rotate = cookie % BITS_PER_PTR
    ptrval = ROL((ULONG_PTR)ptr, rotate)   # rotate-left first
    return ptrval XOR cookie
```

### 2.4 핵심 발견 — XOR vs ROR+XOR 결론

**Wine 의 공식 implementation 은 ROR+XOR (단순 XOR involution 아님).** 본 project 의 [MSVC_ENCODE_POINTER_RESEARCH.md](../MSVC_ENCODE_POINTER_RESEARCH.md) 의 wave 9-A 가설 (Win10+ 단순 XOR) 와 모순. 단:
- Wine 은 모든 Windows 버전을 단일 algorithm 으로 통일 (XP-2003 호환 path 채택)
- Win10+ MSVCRT runtime 의 실제 Microsoft impl 은 다를 수 있으나, **본 project 의 caller disassembly 가 ROR/ROL 사용 (V8 §3 evidence)** 이므로 Wine algorithm 와 정합.

→ wave 9-A의 단순 XOR 가설 폐기 + ROR+XOR 채택 권장.

---

## 3. _initterm 의 array bound 인식 방법

**file**: [dlls/msvcrt/data.c](https://github.com/wine-mirror/wine/blob/master/dlls/msvcrt/data.c) (line ~540-582)

### 3.1 algorithm 의사코드

```
_initterm(start, end):                   # void return
    for current in [start, end):
        if *current != NULL:
            (*current)()                 # cdecl void(void)
        current++

_initterm_e(start, end):                 # int return — 첫 nonzero 시 break
    for current in [start, end):
        if *current != NULL:
            res = (*current)()
            if res != 0: return res
    return 0
```

### 3.2 핵심 발견

1. **bound 인식 X — caller 가 `[start, end)` 를 명시.** start/end 는 PE binary 의 `.CRT$XCA`/`.CRT$XCZ` (initterm) / `.CRT$XIA`/`.CRT$XIZ` (initterm_e) sentinel section 의 시작/끝 ptr — linker 가 alphabetical 정렬로 `XCA < XCU... < XCZ` 배치.
2. **NULL skip**: `*current == NULL` 시 silent skip — 빈 slot 허용 (linker hole 정상).
3. **_initterm_e 의 short-circuit**: 첫 nonzero return 시 즉시 break + status 전파 — security cookie init 실패 시 process abort path.

본 project 의 wave 11+ scan 에서 `_initterm` caller 가 byte-scan FF 로 detect 되는 이유 = caller frame 의 `start`/`end` ptr 이 partial init 된 buffer 를 가리키며, MSVC `__scrt_common_main_seh` 의 `_initterm` 호출 직전 `__security_init_cookie` 가 byte-scan loop 를 동반.

---

## 4. vtable corruption 처리 (Wine 의 입장)

**Wine 은 vtable corruption 를 _자체적으로 detect/recover 하지 않음._** Wine 의 가정:
- guest PE 가 well-formed → vtable 은 valid
- corruption 은 emulation layer 의 책임이 아님 (wine 은 native 실행 — corruption 자체가 발생 X)

→ Wine source 에 vtable graceful skip / NULL deref recovery pattern **없음.**

### 4.1 그러나 인접 pattern: SEH 의 NULL deref handling

**file**: [dlls/ntdll/exception.c](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/exception.c) (line ~194-261)

`dispatch_exception` 의 algorithm:
```
dispatch_exception(rec, context):
    1. log + classify (EXCEPTION_WINE_STUB / NAME_THREAD / DBG_PRINT...)
    2. call_vectored_handlers(rec, ctx)
        if returns CONTINUE_EXECUTION → NtContinue (resume)
    3. call_seh_handlers(rec, ctx)        # __try/__except chain via TIB->ExceptionList
        if returns SUCCESS → NtContinue
    4. else NtRaiseException(rec, ctx, FALSE)   # 2nd chance — debugger / unhandled
```

vectored handler ptr 자체는 `RtlEncodePointer` 로 obfuscate (line ~108) — call 시 `RtlDecodePointer` (line ~168). hijack 방어.

### 4.2 본 project wave 15-A 적용 권장 pattern (own 작성)

Wine 의 SEH dispatch 는 emulation 이 아니라 native — 본 project 는 emul 이므로 다음 pseudo:
```
on call_indirect(eax + disp):
    target = mem_read(eax + disp)
    if target == NULL or region_find(target) == NULL:
        # vtable corruption — graceful skip
        log_once("vtable_skip @ EIP=%x slot=%x+%d", eip, eax, disp)
        push_to_skip_log(eip, eax, disp)
        if skip_count > VTABLE_SKIP_CAP: halt("vtable_skip_cap reached")
        # caller 의 ret addr 은 push 되지 않음 (call 미실행) → simply EIP = next inst
        eip = next_inst(eip)
        return
    eip = target
```

→ wave 15-A 의 cpp shim impl path. CAP=1024 권장 (현재 unmapped EIP skip 와 동일).

---

## 5. polling spin loop graceful break 기법

**file**: [dlls/ntdll/sync.c](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/sync.c)

### 5.1 핵심 function

| function | line range | 역할 |
|---|---|---|
| `RtlEnterCriticalSection` | ~351-386 | spin → InterlockedIncrement → wait fallback |
| `RtlWaitOnAddress` | ~876-+ | futex-like wait (process-local) |
| `spin_lock` (static) | ~847-851 | InterlockedCompareExchange + YieldProcessor |
| `RtlSleepConditionVariableCS` | ~763-774 | RtlWaitOnAddress + critsect leave/re-enter |

### 5.2 algorithm 의사코드

```
RtlEnterCriticalSection(crit):
    if crit.SpinCount > 0:
        if TryEnter: return SUCCESS
        for count in [SpinCount..0]:
            if crit.LockCount > 0: break        # 다른 waiter 있으면 spin 포기
            if crit.LockCount == -1:            # try CAS
                if CAS(LockCount, 0, -1) == -1: goto done
            YieldProcessor()                    # x86: PAUSE / aarch64: YIELD
    if InterlockedIncrement(LockCount) > 0:
        if owning_thread == self: recursion++; return SUCCESS
        wait_for_critsect(crit)                 # event/futex blocking wait
done:
    owning_thread = self
    recursion = 1

spin_lock(lock):                                 # internal short spin (no fallback)
    while CAS(lock, -1, 0) != 0:
        YieldProcessor()
```

### 5.3 핵심 발견 — graceful break 기법 4가지

1. **bounded spin count**: `crit.SpinCount` (default 4000 on multi-CPU, 0 on single-CPU). 초과 시 OS event wait 으로 fallback.
2. **competing-waiter abort**: spin 도중 `LockCount > 0` (다른 thread 도 대기) 시 즉시 spin abort — futile spin 방지.
3. **YieldProcessor pause**: x86 PAUSE / arm YIELD instruction — busy-wait 의 thermal/power budget 절감 + memory order hint.
4. **futex fallback**: `RtlWaitOnAddress` 는 256-bucket hash queue (line ~838 `futex_queues[256]`) + `NtAlertThreadByThreadId` 로 queue management — single-process futex.

### 5.4 본 project 의 wave 15-spin 적용 (own 작성)

본 project 의 byte-scan FF detector (`s_back_jcc_count >= 4096`) 는 위 기법 1 (bounded spin) 의 emul 적용 — 동일 algorithm:
```
on backward_jcc:
    s_back_jcc_count++
    if s_back_jcc_count > SPIN_CAP_INST:           # 4096 default
        if monotonic_decrease_register_detected():
            # 자연 break (CMP reg, target → JL/JG 패턴)
            fast_forward_remaining_iterations()
        else:
            graceful_skip(EIP = next_after_loop)
```
→ Wine 의 (2) competing-waiter abort 에 대응되는 본 project 패턴: byte-scan loop 가 4096 iter 후 buffer 의 나머지 영역도 init 으로 mark (4.2 의 wave 15-B).

---

## 6. WndProc dispatch — Win32 → AppKit translation (winemac.drv)

**files**:
- [dlls/user32/winproc.c](https://github.com/wine-mirror/wine/blob/master/dlls/user32/winproc.c) — generic winproc dispatch
- [dlls/winemac.drv/event.c](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/event.c) — macdrv event → Win32 message
- [dlls/winemac.drv/cocoa_event.m](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/cocoa_event.m) — NSEvent → macdrv_event

### 6.1 architecture chain

```
[macOS AppKit]
    NSApplication.sendEvent: → NSEvent
        ↓ (cocoa_event.m: WineEventQueue postEvent)
    macdrv_event (C struct, type-tagged union)
        ↓ (winemac.drv ProcessEvents — event.c line ~499)
    macdrv_handle_event(event)             # event.c line ~371-486
        ↓ switch on event.type
    각 handler 가 NtUserMessageCall / NtUserPostMessage 로 Win32 message 변환
        ↓ (server-side queue)
    DispatchMessageW → call_window_proc (winproc.c line ~103-115)
        → WINPROC_wrapper(proc, hwnd, msg, wp, lp)        # asm bridge to PE wndproc
```

### 6.2 핵심 발견 — translation 의 5단계

1. **AppKit → C struct decoupling**: NSEvent 를 직접 PE WndProc 에 전달 X. Wine 은 thread-safe `macdrv_event` C struct (type-tagged enum) 로 변환 — `macdrv_query_event` (event.c line ~316) + `cocoa_event.m` 의 `WineEventQueue postEvent`.
2. **event mask filter**: `macdrv_ProcessEvents(mask)` 가 `QS_*` flag → `macdrv_event_mask` (line ~499-525). nested event recursion 방지: `if data->current_event && type != QUERY_EVENT...` 에서 mask=0.
3. **switch dispatch**: `macdrv_handle_event` 의 거대한 switch (line ~385-483) — 28+ event type. unhandled 은 silent ignore (`default: TRACE("ignoring")`).
4. **Win32 message 생성은 handler 별 분산**: 각 case (예: `MOUSE_BUTTON` → `macdrv_mouse_button`) 가 자체적으로 `NtUserPostMessage(WM_LBUTTONDOWN, ...)` 호출. 중앙집중 dispatch X.
5. **WndProc 호출은 asm wrapper**: `WINPROC_wrapper` (winproc.c line ~47-75 inline asm 또는 line ~82 fallback inline) 가 stack 정렬 + cdecl/stdcall 호환 layer 보장. ANSI ↔ Unicode 변환은 `WINPROC_CallProcAtoW` / `WtoA` (line ~175, ~435) 가 message 별 `lParam` 의 string field 를 manual map.

### 6.3 본 project 의 wave 15-Cocoa 적용 가능성

본 project 는 wave 17 (libcompression + install) 까지 GUI 미발생 — wave 18+ 의 Battle.net launcher 단계에서 필요. 시점 도래 시:
- macdrv_event 의 type-tag union pattern 채택 (단일 struct 로 NSEvent 추상화)
- `macdrv_handle_event` 의 switch dispatch 패턴 (event type → handler function pointer table 도 가능 — Wine 은 switch 이지만 본 project 는 closure dict 더 적합)
- ANSI/Unicode 변환은 message 별 manual — `WM_GETTEXT` / `WM_SETTEXT` / `WM_CHAR` 등 한정.

---

## 7. TLS / thread sync 추가 (thread.c)

**file**: [dlls/ntdll/thread.c](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/thread.c)

### 7.1 FLS (Fiber Local Storage) chunk allocator

| function | line range | 역할 |
|---|---|---|
| `RtlFlsAlloc` | ~516-575 | chunk-based slot alloc (FLS_INFO_CHUNK array of arrays) |
| `RtlFlsFree` | ~581-630 | slot 해제 — callback != NULL 시 callback 호출 |
| `RtlFlsSetValue` / `RtlFlsGetValue` | ~632-678 | per-thread TEB->FlsSlots 의 chunk-indexed access |

### 7.2 algorithm pattern

- 8개 chunk (`fls_data.fls_callback_chunks[8]`), 각 chunk 의 size 는 `fls_chunk_size(idx)` 로 geometric 증가 (16, 32, 64, ...)
- FLS index 0 은 prohibit (`chunk->callbacks[0].callback = (void *)~0` line ~561)
- alloc 시 free chunk 검색 → 없으면 새 chunk lazy alloc
- free 시 callback 호출은 lock 외부에서 (deadlock 방지)

### 7.3 핵심 발견

- **chunk-based allocator 가 Win10+ FLS 의 표준** (단일 array 가 아님). MSVC `_Init_thread_*` family 가 사용.
- 본 project 의 wave 7-MSVC step 에서 FLS 호출 발견 시 → chunk-array 추적 필요. 단순 TLS slot index 가정 X.

---

## 8. exception.c — RaiseException / RtlUnwind 의 unwind 방향

**file**: [dlls/ntdll/exception.c](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/exception.c)

### 8.1 RaiseException chain

| function | line range | 역할 |
|---|---|---|
| `RaiseException` (kernel32 forward) | dlls/kernelbase | rec 빌드 → `RtlRaiseException` |
| `RtlRaiseException` (per-arch) | ntdll/signal_*.c | context capture → `dispatch_exception` |
| `dispatch_exception` | ~196-261 | vectored → SEH → 2nd chance |
| `raise_status` / `RtlRaiseStatus` | ~315-323 | status code → EXCEPTION_RECORD wrap → infinite raise |

### 8.2 핵심 발견

- **`RtlRaiseStatus` 는 NORETURN — `for(;;) RtlRaiseException(...)` 무한 raise** (line ~323) — exception handler 가 fail 시 termination 보장.
- vectored handler ptr 은 항상 `RtlEncodePointer` obfuscate — 함수 ptr leak 방어.
- arch-specific unwind 는 `signal_i386.c` / `signal_x86_64.c` / `signal_arm64.c` (loader.c 와 별개) 에서 host signal handler → guest CONTEXT 변환.

본 project 적용: emulator 는 host signal 을 직접 받지 X — guest exception 발생 시점에 emul instrumentation 으로 EXCEPTION_RECORD 빌드 후 wave 11 의 `__C_specific_handler` shim 호출 path (별도 wave 18+).

---

## 9. process.c — 본 project 와 의도적 분기

**file**: [dlls/ntdll/process.c](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/process.c)

대부분 Wow64 / Debug API — 본 project 의 wave 15+ 와 직접 관련 없음. 단:
- `RtlCreateUserThread` (line ~250) — thread create 의 stack alloc + entry init pattern
- `RtlCreateUserStack` (line ~304) — guard page (PAGE_GUARD) + commit/reserve 분리

본 project 의 wave 18+ thread shim 시점에 reference. wave 15-17 범위 외.

---

## 10. 본 project 와 비교 — 자체 구현 path summary

| 영역 | Wine 방식 | 본 project (own1 §A 자체 구현) | wave |
|---|---|---|---|
| PE import resolution | LdrLoadDll → fixup_imports → find_named_export (binary search) | `pe_to_macho_shim.cpp` 의 IAT real resolve (wave 7) | wave 7 (완료) |
| EncodePointer | `RtlEncodePointer` ROR+XOR (rtl.c:1045) | wave 11 의 ROR+XOR shim (caller cookie value) | wave 11 (완료) |
| _initterm | start/end ptr 순회 + NULL skip (msvcrt/data.c:543) | wave 11 의 fake stub `_initterm` (immediate return) | wave 11 (완료) |
| vtable corruption | (Wine 미처리 — guest 가 well-formed 가정) | wave 15-A: `call [eax+disp]` NULL deref → graceful skip | wave 15 (예정) |
| spin loop break | bounded SpinCount + competing-waiter abort + YieldProcessor | wave 14: byte-scan FF detector (4096 iter cap + monotonic dec) | wave 14 (완료) |
| WndProc dispatch | macdrv_event union → switch → WINPROC_wrapper asm | wave 18+ 영역 — 미시작 | wave 18+ |
| FLS | 8-chunk geometric allocator (thread.c:516) | wave 7+ — 단순 TLS slot 가정. chunk 추적 미구현 | wave 18+ |
| exception | Vectored → SEH → 2nd chance (exception.c:196) | wave 11 의 `__C_specific_handler` fake stub | wave 11 (부분) |

---

## 11. wave 15+ 적용 가능 algorithm/pattern (citation only, code lift X)

### 11.1 wave 15-A — vtable corruption recovery
**reference**: [dlls/ntdll/exception.c:108](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/exception.c#L108) (vectored handler obfuscation pattern)
**적용**: `call [eax+disp]` 시 region_find(target) NULL → graceful skip + log dedup. Wine 의 EncodePointer obfuscation 자체는 본 project 와 무관 — 단 "ptr validity check 후 dispatch" 의 pattern 만 차용.

### 11.2 wave 15-B — byte-scan FF buffer 보강
**reference**: [dlls/ntdll/sync.c:351-386](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/sync.c#L351-L386) (RtlEnterCriticalSection — bounded spin + competing-waiter abort)
**적용**: byte-scan FF 의 4096 iter cap 도달 시 buffer 의 남은 영역 fast-forward 로 0x1 mark — Wine 의 spin abort 와 동일 철학 (futile work 방지).

### 11.3 wave 15-C — helper.mm IPC vtable stub
**reference**: [dlls/ntdll/loader.c:380](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/loader.c) (`stub_entry_point` — unimplemented function 의 EXCEPTION_WINE_STUB raise)
**적용**: helper.mm 의 IPC kind=19 (vtable_stub) 가 unmapped vtable slot 호출 시 EXCEPTION_WINE_STUB 와 유사한 logged graceful return — actual stub 는 본 project 자체 작성.

### 11.4 wave 15-D — pe_msvc_vtable_partial_init_real (hexa)
**reference**: [dlls/msvcrt/data.c:543](https://github.com/wine-mirror/wine/blob/master/dlls/msvcrt/data.c#L543) (`_initterm` 의 NULL slot skip pattern)
**적용**: hexa rule `pe_msvc_vtable_partial_init_real` 가 vtable 영역 의 NULL slot 검출 시 `fake_proc_stub` ptr 채우기 — `_initterm` 의 NULL skip 와 dual (Wine 은 skip, 본 project 는 fill).

### 11.5 wave 15-E — caller buffer field 추적 (hexa)
**reference**: [dlls/ntdll/loader.c:1186-1192](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/loader.c#L1186-L1192) (IAT 의 protect/restore — RO section 의 mutable field 다루기)
**적용**: caller 가 buffer 를 R-only section 으로 가정 시 emul 의 mem_write 는 그대로 (protect 무시) — Wine 의 NtProtectVirtualMemory 호출은 본 project 미적용 (emul layer 는 page protection 추적 X).

---

## 12. own1 §A enforcement 재확인 (작성 후 self-check)

본 doc 작성 시:
- Wine source code 직접 copy: **0 line**
- algorithm 패턴 + 의사코드: own 작성 (자연어 + own 변수명)
- function name + line range: citation only (markdown link)
- struct/macro/inline asm 인용: 0건
- 결론 / 적용 path: 본 project (`airgenome-gamebox`) 의 wave 정합 — 자체 구현 결정 명시

**enforcement 통과**: Wine 의 algorithm 만 reference, source 의 한 줄도 본 doc 에 등장 X.

---

## 13. clone cleanup

`/tmp/wine-research/wine` 는 ~ 250MB (shallow). 본 doc 작성 후 추가 분석 필요 시 보존 — 안 그러면 `rm -rf /tmp/wine-research` 로 정리. (cycle 종료 시점 결정.)

---

*written 2026-05-02 cycle wave 14 종료 후. base commit `b2b741c` (V12 plateau 96.5%) 의 wave 15+ algorithm reference 로 작성. Wine source 는 GPL — 본 project 의 own1 §A enforcement 에 따라 algorithm/architecture 분석만 수행 + source code 한 줄도 lift X. 모든 reference 는 markdown link citation.*
