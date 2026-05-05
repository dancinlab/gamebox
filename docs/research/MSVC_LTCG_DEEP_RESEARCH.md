# MSVC LTCG Static CRT Inline Disassembly — Deep Web Research

**작성**: 2026-05-02 · airgenome-gamebox wave 15+ 준비용 외부 research 결과
**범위**: MSVC `/MT /GL /LTCG` 빌드 (VS2017 14.16, x86 32-bit) 의 inline 화된 CRT
boilerplate 정확 disassembly pattern.
**own1 enforcement**: 본 문서는 분석/citation only. 어떤 reference repo 의 코드도
lift 하지 않으며, airgenome-gamebox 내부 구현은 별도 trace 기반으로 작성한다.
**cross-ref**: [`docs/MSVC_ENCODE_POINTER_RESEARCH.md`](../MSVC_ENCODE_POINTER_RESEARCH.md),
[`docs/MSVC_CRT_INIT_BYPASS.md`](../MSVC_CRT_INIT_BYPASS.md)

---

## 0. 연구 배경

wave 11–14 까지 airgenome-gamebox 의 PE loader 는 Battle.net Setup.exe (VS2017
LTCG, /MT) 의 entry point 에서 `__scrt_common_main_seh` thunk 와 그 inline 화된
sub-routine (`__security_init_cookie`, `_initterm`, `EncodePointer/DecodePointer`
caller, vtable bootstrap) 을 만나 halt 했다. wave 15+ 의 fix path 결정을 위해
**inline 후의 정확한 disassembly pattern** 을 외부 research 로 재확인한다.

deep research 는 10 + 1 query, 각 query 마다 핵심 발견과 의사코드, 그리고
airgenome-gamebox 내부 hook 적용 방향을 정리한다.

---

## Q1. `__scrt_common_main_seh` inline disassembly

**citation**:
- [Microsoft Learn — CRT Initialization](https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization?view=msvc-170)
- [copyprogramming — Understanding `__scrt_common_main_seh`](https://copyprogramming.com/howto/what-where-is-scrt-common-main-seh)
- [ojdkbuild VS2017 14.16 `exe_common.inl` mirror](https://github.com/ojdkbuild/tools_toolchain_vs2017bt_1416/blob/master/VC/Tools/MSVC/14.16.27023/crt/src/vcruntime/exe_common.inl) — analysis only, NO lift
- [Patreon — Why PE Entry Point ≠ main](https://www.patreon.com/posts/why-is-pe-entry-61343353)

**핵심 발견**:
- PE EntryPoint → `__security_init_cookie` (cookie init) → fall-through (또는 jmp)
  to `__scrt_common_main_seh` → SEH frame 등록 → static init → `main`.
- LTCG 는 thunk 자체를 entry 로 inline 하는 경향이 강함. `__scrt_common_main_seh`
  의 prologue `mov eax, fs:[0]; push eax; ...` 가 entry point 의 첫 부분에서
  바로 시작되는 케이스 존재.
- VS2017 14.16 `exe_common.inl` 의 `__scrt_common_main_seh` 본체 흐름:
  1. `__scrt_initialize_crt(__scrt_module_type::exe)`
  2. `__scrt_acquire_startup_lock()`
  3. `_initterm(__xi_a, __xi_z)` — C initializers
  4. `_initterm(__xc_a, __xc_z)` — C++ ctors
  5. `__scrt_initialize_default_local_stdio_options()`
  6. `argc/argv` setup → `_initialize_narrow_environment()` 등
  7. `mainCRTStartup` → user `main`.

**의사코드 (entry inline 후)**:
```
entry:
  call  __security_init_cookie
  jmp   __scrt_common_main_seh

__scrt_common_main_seh:           ; LTCG inline 시 entry 안으로 fold
  ; SEH frame
  mov   eax, fs:[0]
  push  ebp
  push  HANDLER_ADDR              ; __scrt_common_main_seh 자체가 handler
  push  eax
  mov   fs:[0], esp

  ; CRT init phase 1
  call  __scrt_initialize_crt
  call  __scrt_acquire_startup_lock

  ; static init
  push  __xi_z
  push  __xi_a
  call  _initterm_e               ; C init (returns int → check)
  push  __xc_z
  push  __xc_a
  call  _initterm                 ; C++ ctors

  ; user main
  call  main
  ...
  call  __scrt_release_startup_lock
  call  exit
```

**airgenome-gamebox 적용**:
- entry 진입 시 `__security_init_cookie` 의 prologue (`mov eax, ds:_security_cookie;
  test eax, eax`) signature 매칭 → fast-path stub 으로 redirect.
- `__scrt_common_main_seh` 의 SEH push pattern (`push HANDLER_ADDR; push eax;
  mov fs:[0], esp`) 을 trace decoder 가 인식하면 **PE loader 가 SEH frame
  alloc 후 user main 으로 jump** 하는 것이 안전.

---

## Q2. `__security_init_cookie` 정확 disassembly pattern

**citation**:
- [Microsoft Learn — `__security_init_cookie`](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/security-init-cookie?view=msvc-170)
- [Microsoft Learn — VC++ Stack-based Buffer Protection](https://learn.microsoft.com/en-us/archive/msdn-magazine/2017/december/c-visual-c-support-for-stack-based-buffer-protection)
- [kallanreed — Disabling Stack Cookie in VS2013](https://kallanreed.com/2015/02/14/disabling-the-stack-cookie-generation-in-visual-studio-2013/)
- [DaintyJet/VChat_Security_Cookies (analysis only)](https://github.com/DaintyJet/VChat_Security_Cookies)
- [Wikipedia — Buffer overflow protection](https://en.wikipedia.org/wiki/Buffer_overflow_protection)

**핵심 발견**:
- `__security_cookie` 는 `.data` 의 single 4-byte (x86) global. cookie 는
  `GetSystemTimeAsFileTime` / `GetCurrentProcessId` / `GetCurrentThreadId` /
  `QueryPerformanceCounter` / `GetTickCount` 등을 XOR fold 후 `__security_cookie`
  와 `__security_cookie_complement` 에 저장.
- prologue (caller 함수): `mov eax, ds:[__security_cookie]; xor eax, ebp; mov
  [ebp - N], eax` — frame canary.
- epilogue: `mov ecx, [ebp - N]; xor ecx, ebp; call __security_check_cookie`.
- `__security_check_cookie`: `cmp ecx, ds:[__security_cookie]; jne fail; ret`.

**의사코드**:
```
__security_init_cookie:
  mov   eax, ds:[__security_cookie]
  cmp   eax, DEFAULT_SECURITY_COOKIE   ; 0xBB40E64E (x86)
  jne   already_init
  ; collect entropy
  call  GetSystemTimeAsFileTime
  ; XOR fold tick / pid / tid / qpc
  ; ensure cookie != default, top byte != 0 (guard against null deref tricks)
  mov   ds:[__security_cookie], eax
  not   eax
  mov   ds:[__security_cookie_complement], eax
  ret
```

caller prologue/epilogue:
```
prologue:
  mov   eax, [__security_cookie]
  xor   eax, ebp
  mov   [ebp - 4], eax           ; frame canary slot

epilogue:
  mov   ecx, [ebp - 4]
  xor   ecx, ebp
  call  __security_check_cookie  ; tail
```

**airgenome-gamebox 적용**:
- LTCG inline 시 `__security_init_cookie` body 가 entry 함수 내부에 주입됨.
  trace decoder 가 `cmp eax, 0xBB40E64E` literal 로 init 진입을 detect → cookie
  를 deterministic 한 값 (예: 0x12345678) 으로 한 번에 채우고 init body skip.
- `__security_check_cookie` 는 reachable 하면 `ret` 만 하는 stub 으로 patch
  (frame mismatch crash 회피).

---

## Q3. `_initterm` / `_PVFV` array walk 의 inline disassembly

**citation**:
- [Microsoft Learn — `_initterm, _initterm_e`](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/initterm-initterm-e?view=msvc-170)
- [OSDev — Visual C++ Runtime](https://wiki.osdev.org/Visual_C++_Runtime)
- [libctiny `initterm.cc` (analysis only)](https://github.com/leepa/libctiny/blob/master/initterm.cc)

**핵심 발견**:
- `_PVFV = void (__cdecl*)(void)`; `_PIFV = int (__cdecl*)(void)`.
- `_initterm(first, last)` 은 `[first, last)` 구간을 walk, NULL skip 후 해당
  function pointer 를 호출. `_initterm_e` 는 return int 가 0 이 아니면 즉시
  abort.
- LTCG 에서 `_initterm` body 가 `__scrt_common_main_seh` 안으로 inline 되어
  `cmp/jb/call` 의 tight loop 으로 보임.

**의사코드 (inline 후 typical x86)**:
```
  mov   esi, OFFSET __xc_a
  mov   edi, OFFSET __xc_z
.loop:
  cmp   esi, edi
  jae   .done
  mov   eax, [esi]
  test  eax, eax
  je    .skip
  call  eax                    ; ctor
.skip:
  add   esi, 4
  jmp   .loop
.done:
```

**airgenome-gamebox 적용**:
- trace decoder 가 `cmp esi, edi; jae .done; mov eax, [esi]; test eax, eax; ...
  call eax; add esi, 4` pattern 을 단일 macro-op 로 인식 → ctor list 를 한 번에
  walk 하거나, 위험하면 전체 loop 을 single `nop`-jump 로 short-circuit.
- ctor function pointer 를 dereferencing 하기 전 PE section bounds (`.text`)
  안에 있는지 확인 — bss/heap 으로 cross 할 경우 즉시 skip.

---

## Q4. `EncodePointer` / `DecodePointer` ROL+XOR disassembly

**citation**:
- [Microsoft Learn — EncodePointer (kernel32)](https://learn.microsoft.com/en-us/previous-versions/bb432254(v=vs.85))
- [Microsoft Learn — DecodePointer](https://learn.microsoft.com/en-us/previous-versions/bb432242(v=vs.85))
- [rentry — kernel32 obfuscated pointers](https://rentry.co/kernel32-obfuscated-pointers)
- [colinfinck — Modern VS meets Ancient Windows](https://colinfinck.de/posts/modern-visual-studio-meets-ancient-windows/)

**핵심 발견**:
- 진짜 `EncodePointer` 는 `kernelbase.dll`/`ntdll.dll` 내부에서 process-wide
  cookie (`SharedUserData->Cookie` 또는 PEB 내 cookie) 와 XOR + ROR 로 변환.
- VS2017 LTCG 의 caller 는 함수 포인터 vtable 류를 `EncodePointer` 로 한 번
  encode 해 두고, dispatch 때 `DecodePointer` 로 복원 — 직접 호출 직전.
- caller pattern (Battle.net Setup `0x56AC43`): `mov eax, [_security_cookie];
  xor ebx, eax; and ecx, 0x1F; ror edi, cl; call edi`.

**의사코드** (이미 Q4 는 `MSVC_ENCODE_POINTER_RESEARCH.md` 에서 1차 정리됨,
여기서는 LTCG inline 측면 보강):
```
encode(p):
  c  = process_cookie
  return ROR(p XOR c, c & 0x1F)

decode(p):
  c  = process_cookie
  return ROL(p, c & 0x1F) XOR c
```

LTCG inline 후 caller (DecodePointer):
```
  mov   eax, ds:[encoded_fp]
  mov   ecx, ds:[__security_cookie] ; 또는 process cookie 별도
  mov   edx, ecx
  and   ecx, 0x1F
  rol   eax, cl
  xor   eax, edx
  call  eax
```

**airgenome-gamebox 적용**:
- decoder hook 은 wave 9-A 의 단순 XOR involution 가 아니라 **ROL+XOR** 로
  업그레이드. `__security_cookie` 와 `process_cookie` 를 모두 deterministic 값
  으로 통일해 encode/decode 가 idempotent 하게 만들 것.
- 또는 decode caller pattern 자체 (`rol eax, cl; xor eax, edx; call eax`) 를
  trace decoder 가 detect → 미리 plain pointer 로 patch.

---

## Q5. MSVC LTCG `/GL /LTCG /MT` 의 inline 양상

**citation**:
- [Microsoft Learn — /LTCG](https://learn.microsoft.com/en-us/cpp/build/reference/ltcg-link-time-code-generation?view=msvc-170)
- [Microsoft Learn — /GL Whole Program Optimization](https://learn.microsoft.com/en-us/cpp/build/reference/gl-whole-program-optimization?view=msvc-170)
- [MSDN Magazine 2002 — Under the Hood: LTCG](https://learn.microsoft.com/en-us/archive/msdn-magazine/2002/may/under-the-hood-link-time-code-generation)
- [cpp-docs LTCG (GitHub)](https://github.com/MicrosoftDocs/cpp-docs/blob/main/docs/build/reference/ltcg-link-time-code-generation.md)

**핵심 발견**:
- `/GL` 은 obj 를 IL (intermediate language) 로 emit. `/LTCG` 는 link time 에
  whole-program optimize → cross-TU inlining, dead code elim, tail-call,
  vtable devirtualization 까지.
- `/MT` (static CRT) 는 `libcmt.lib` 등 static lib 내부 함수도 LTCG 대상으로
  포함 → `_initterm`, `_amsg_exit`, `__crtGetEnvironmentStringsA` 같은 boilerplate
  도 main 함수 안으로 inline 됨.
- 결과: PE 의 `.text` 안에서 named CRT symbol 이 사라지고, function boundary
  도 사실상 흐려짐. byte-level signature 매칭이 어려워짐 → **semantic pattern
  매칭 (instruction sequence)** 만이 신뢰 가능.

**의사코드 (signature → semantic)**:
```
detect_initterm(insn_window):
  if window contains:
    "mov reg1, OFFSET .CRT$X*A"  AND
    "mov reg2, OFFSET .CRT$X*Z"  AND
    a tight loop with "test eax,eax; je; call eax; add reg1, 4"
  then mark window as inlined _initterm
```

**airgenome-gamebox 적용**:
- byte-by-byte signature DB 폐기. wave 15 부터는 windowed semantic pattern
  matcher 로 전환 — 각 CRT routine 마다 minimal "fingerprint instruction set".
- LTCG 는 register allocation 을 매번 다르게 하므로 **register-agnostic
  pattern** 필수 (regex over instruction mnemonic + operand kind, not literal
  reg).

---

## Q6. C++ vtable layout (x86 32-bit, VS2017)

**citation**:
- [ALSchwalm — Reversing C++ Virtual Functions Pt2](https://alschwalm.com/blog/static/2017/01/24/reversing-c-virtual-functions-part-2-2/)
- [Dennis Babkin — Reverse Engineering Virtual Functions VC++](https://dennisbabkin.com/blog/?t=reverse-engineer-virtual-functions-vs-cpp-compiler-vtable-purecall-cfg)
- [Wikipedia — Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)
- [Random ASCII — VC++ 2013 Class Layout Change](https://randomascii.wordpress.com/2013/12/01/vc-2013-class-layout-change-and-wasted-space/)
- [Old New Thing — Layout of COM Object](https://devblogs.microsoft.com/oldnewthing/20040205-00/?p=40733)

**핵심 발견**:
- 32-bit MSVC 에서 object 의 `+0` offset 은 vfptr (vftable pointer). vtable
  자체는 `.rdata` 상의 4-byte function pointer 배열 (declaration order).
- typical virtual call: `mov edx, [ecx]; call dword ptr [edx + 4*N]`. `ecx` 는
  `__thiscall` this pointer.
- Multiple inheritance / virtual base 시 thunk 가 추가됨 (`adjustor thunk`),
  하지만 single-inheritance 만 쓰는 일반 GUI installer 는 단순 형태.

**의사코드**:
```
vcall(this_ptr, slot):
  vtable = *(uint32_t*)this_ptr
  fn     = *(uint32_t*)(vtable + 4*slot)
  fn(this_ptr, ...)              ; __thiscall: ecx = this
```

asm pattern:
```
  mov   ecx, this_obj            ; load this
  mov   edx, [ecx]               ; vftable
  call  dword ptr [edx + 0x10]   ; slot 4
```

**airgenome-gamebox 적용**:
- PE loader 가 vtable 을 dereferencing 하는 첫 시점에 `[edx + N]` 의 destination
  이 PE `.text` 범위 내인지 확인. 아니면 (예: encoded pointer remnant) → halt
  대신 stub 으로 redirect.
- `__purecall` slot 매칭 시 즉시 `ret` (purecall halt 회피).

---

## Q7. `.CRT$XCA` / `.CRT$XCZ` 등 section pointer 정렬

**citation**:
- [Microsoft Learn — CRT Initialization](https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization?view=msvc-170)
- [Luke's Blog — Initialization order in VS](https://www.luke1410.de/blog/2018/09/controlling-the-instantiation-order-of-global-and-static-objects-in-visual-studio/)
- [zeux — Death by static initialization](https://zeux.io/2010/10/10/death-by-static-initialization/)
- [Inferara — Understanding C Runtime](https://inferara.com/blog/c-runtime/)
- [OSDev — Visual C++ Runtime](https://wiki.osdev.org/Visual_C++_Runtime)

**핵심 발견**:
- CRT section group:
  - `.CRT$XIA` … `.CRT$XIZ` — C initializers (`_initterm_e`)
  - `.CRT$XCA` … `.CRT$XCZ` — C++ ctors (`_initterm`)
  - `.CRT$XPA` … `.CRT$XPZ` — pre-terminators
  - `.CRT$XTA` … `.CRT$XTZ` — terminators (`atexit` 같은 것)
  - `.CRT$XLA` … `.CRT$XLZ` — TLS callbacks
- linker 가 `$` 뒤 string 으로 alphabetical merge. `XCU` 는 user ctor pool.
  `XCA` / `XCZ` 는 sentinel — `__xc_a`, `__xc_z` symbol 이 여기 위치.
- LTCG 는 이 section 을 그대로 둠 (linker 가 별도 단계에서 처리), 하지만
  `_initterm` 호출 자체는 inline 화.

**의사코드**:
```
.CRT layout (memory order):
  __xi_a  | XIA
  ...     | XI* user
  __xi_z  | XIZ
  __xc_a  | XCA
  ...     | XCU user ctors
  __xc_z  | XCZ
  __xl_a  | XLA  (TLS callback start)
  ...
  __xl_z  | XLZ
  ...
```

**airgenome-gamebox 적용**:
- PE loader 가 `.CRT` section 을 인식하면 **boundary symbol 을 직접 매핑** 해
  `_initterm` inline 매칭 시 already-known range 로 인식. 트레이스 시 noise 감소.
- TLS callback (`XLA`–`XLZ`) 도 같은 방식 — Q9 참고.

---

## Q8. Windows i386 PE loader call sequence

**citation**:
- [wine `dlls/ntdll/loader.c` (analysis only)](https://github.com/wine-mirror/wine/blob/master/dlls/ntdll/loader.c)
- [NtDoc — LdrLoadDll](https://ntdoc.m417z.com/ldrloaddll)
- [airesoft — DLL Demystifying Loader Lapses](http://blog.airesoft.co.uk/2011/05/dll-demystifying-loader-lapses/)
- [Microsoft Learn — NT DLL Loader DllMain deadlocks](https://learn.microsoft.com/en-us/archive/blogs/mgrier/the-nt-dll-loader-dll-callouts-dllmain-dll_process_attach-deadlocks)

**핵심 발견**:
- PE 실행 흐름 (kernel side):
  `NtCreateUserProcess` → kernel maps image → `LdrInitializeThunk` (ntdll user
  side init) → import resolution → TLS callbacks (`PROCESS_ATTACH`) → DLL
  `DllMain`s → `EntryPoint`.
- DLL 동적 로드: `LoadLibraryW` → `LoadLibraryExW` (kernelbase) → `LdrLoadDll`
  (ntdll) → `LdrpLoadDll` → `LdrpFindOrMapDll` → `NtMapViewOfSection`.
- loader lock 은 `LdrpLoaderLock` (critical section) 로 직렬화.

**의사코드**:
```
LdrInitializeThunk(ctx):
  for dll in StaticImports:
    LdrpLoadDll(dll)
    LdrpInitializeProcess_TlsCallback(dll, PROCESS_ATTACH)
  run TLS callbacks (.CRT$XL* via PE TLS directory)
  jump to AddressOfEntryPoint   ; → __security_init_cookie / __scrt_*
```

**airgenome-gamebox 적용**:
- PE loader simulator (gamebox 의 user-space 구현) 가 kernel side mapping
  단계에서 import 만 채우고, TLS callback / DllMain 은 stub 으로 우회 가능
  하면 우회 (Setup.exe 에 hostile TLS callback 이 없는지 trace).
- airgenome 의 wine 기반 enforcement 에서는 `LdrLoadDll` 흐름 그대로 사용 — own1.

---

## Q9. PE TLS callback (IMAGE_TLS_CALLBACK) 실행 순서

**citation**:
- [Unprotect — TLS Callback](https://unprotect.it/technique/tls-callback/)
- [lallous — C/C++ TLS callbacks in VS](http://lallouslab.net/2017/05/30/using-cc-tls-callbacks-in-visual-studio-with-your-32-or-64bits-programs/)
- [Ring Zero Labs — Analyzing TLS Callbacks](https://www.ringzerolabs.com/2019/08/analyzing-tls-callbacks.html)
- [Infosec Inst — Debugging TLS callbacks](https://www.infosecinstitute.com/resources/reverse-engineering/debugging-tls-callbacks/)

**핵심 발견**:
- TLS callback 은 EntryPoint 호출 **전** 에 loader 가 호출. signature:
  `VOID NTAPI Cb(PVOID handle, DWORD reason, PVOID reserved)`.
- VS CRT 는 `tlssup.c` 의 `__dyn_tls_init_callback` 등을 `.CRT$XLB` 에 등록.
- linker 가 `XLA`–`XLZ` 알파벳 정렬 → `__xl_a` 와 `__xl_z` 사이 callback 들이
  순차 실행 (`reason=PROCESS_ATTACH` 시).

**의사코드**:
```
loader:
  if PE.OptionalHeader.DataDirectory[TLS].Size != 0:
    tls_dir = PE + DataDirectory[TLS].VA
    cb_array = *tls_dir.AddressOfCallBacks
    for cb in cb_array (until NULL):
      cb(image_base, DLL_PROCESS_ATTACH, NULL)
  jmp PE.EntryPoint
```

**airgenome-gamebox 적용**:
- PE loader simulator 가 TLS callback array 를 walk. callback 본체는 보통
  `__dyn_tls_init` (TLS 영역 zero-init) 이라 trace 가 안전. 다만 anti-debug
  TLS callback (debugger detect) 은 stub 화 필요.
- callback 실행 후 cookie / TLS slot 0 이 채워졌는지 확인 — `__scrt_*` 가
  의존.

---

## Q10. `RtlRaiseException` dispatch flow

**citation**:
- [Microsoft Learn — RtlRaiseException](https://learn.microsoft.com/en-us/windows/win32/api/rtlsupportapi/nf-rtlsupportapi-rtlraiseexception)
- [saza.RE — Exception Hijacking](https://saza.re/posts/exception_hijack/)
- [billdemirkapi — Abusing Exceptions for Code Execution Pt2](https://billdemirkapi.me/abusing-exceptions-for-code-execution-part-2/)
- [ReactOS dispatch.c](https://doxygen.reactos.org/d2/d15/dll_2ntdll_2dispatch_2dispatch_8c.html)

**핵심 발견**:
- `RtlRaiseException(ER)` → kernel transition → `KiUserExceptionDispatcher`
  (user side) → `RtlDispatchException` → walk SEH frames (FS:[0] chain on x86)
  → matched handler 실행.
- handler 가 `EXCEPTION_CONTINUE_SEARCH` 면 다음 frame, `EXCEPTION_EXECUTE_HANDLER`
  면 unwind 후 except block 진입.
- `RtlVirtualUnwind` (x64 위주) 또는 x86 의 `RtlUnwind` 가 stack 정리.

**의사코드**:
```
RtlDispatchException(ER, ctx):
  frame = fs:[0]                   ; head of EXCEPTION_REGISTRATION_RECORD
  while frame != 0xFFFFFFFF:
    disposition = frame.handler(ER, frame, ctx, dispatcher_ctx)
    if disposition == EXCEPTION_EXECUTE_HANDLER:
      RtlUnwind(frame, ...)
      jmp frame.except_block
    frame = frame.Next
  return STATUS_NONCONTINUABLE_EXCEPTION
```

**airgenome-gamebox 적용**:
- entry 단계에서 SEH frame 체인을 사전에 install (handler = our stub).
- 만약 trace 중 `RtlRaiseException` 류가 emit 되면, gamebox 는 dispatcher 를
  intercept 해 own SEH walker 로 처리 — kernel 까지 안 가게 함.

---

## Q11. MSVC `__try / __except` x86 SEH chain (FS:[0])

**citation**:
- [Wikipedia — Microsoft-specific exception handling](https://en.wikipedia.org/wiki/Microsoft-specific_exception_handling_mechanisms)
- [The Old New Thing / cbrumme — The Exception Model](https://learn.microsoft.com/en-us/archive/blogs/cbrumme/the-exception-model)
- [limbioliong — Understanding Windows SEH Pt2](https://limbioliong.wordpress.com/2022/01/18/understanding-windows-structured-exception-handling-part-2-digging-deeper/)
- [accu.org — MSVC and Win32 SEH](https://accu.org/journals/overload/12/63/orr_245/)

**핵심 발견**:
- 각 `__try` 는 `EXCEPTION_REGISTRATION_RECORD { Next; Handler; ScopeTable;
  TryLevel }` 을 stack 에 push 하고 `fs:[0]` 을 갱신.
- `Handler` 는 보통 `__except_handler3` (x86) 또는 `__C_specific_handler` (x64).
- ScopeTable 은 `__try` block 의 enclosing 정보 + filter / handler RVA 배열.

**의사코드**:
```
__try prologue (x86):
  push  OFFSET ScopeTable
  push  OFFSET __except_handler3
  push  fs:[0]
  mov   fs:[0], esp
  ; try body...

__try epilogue:
  mov   eax, [esp]                ; original next
  mov   fs:[0], eax
  add   esp, 12
```

**airgenome-gamebox 적용**:
- trace decoder 가 `push OFFSET ScopeTable; push OFFSET handler; push fs:[0];
  mov fs:[0], esp` 5-instruction sequence 를 macro-detect. 해당 frame 을 own
  walker 의 stack 에 등록.
- `__except_handler3` 호출 시 ScopeTable 의 filter 실행 → 1 (EXECUTE_HANDLER)
  반환 case 만 우리 unwind 로 분기.

---

## 종합 — wave 15+ 적용 algorithm

### A. Semantic Pattern Database (LTCG-aware)

byte-level signature 폐기. 대신 다음 7 가지 **register-agnostic semantic
fingerprint** 를 trace decoder 에 등록:

1. `__security_init_cookie` — `cmp <reg>, 0xBB40E64E` immediate literal.
2. `_initterm` body — XCA/XCZ offset load + tight loop with `call <reg>`.
3. `EncodePointer/DecodePointer` caller — `and <reg>, 0x1F; ror <reg>, cl;
   xor <reg>, <reg>; call <reg>`.
4. virtual call — `mov <reg2>, [<reg1>]; call dword ptr [<reg2> + N*4]`.
5. SEH push — `push OFFSET .text; push OFFSET handler; push fs:[0]; mov
   fs:[0], esp` 4-instruction window.
6. stack canary prologue — `mov <reg>, ds:[__security_cookie]; xor <reg>, ebp;
   mov [ebp - N], <reg>`.
7. TLS callback walk — `.CRT$XL*` array iterator.

### B. Hook 우선순위

| 우선 | hook | 이유 |
|---|---|---|
| 1 | `__security_init_cookie` literal | 모든 caller 에 영향 |
| 2 | EncodePointer/DecodePointer ROL+XOR caller | wave 11 의 halt root cause |
| 3 | `__purecall` stub (`ret`) | vtable mismatch crash 회피 |
| 4 | `__except_handler3` filter table | crash → recover 경로 |
| 5 | `_initterm` inline loop short-circuit | ctor 불필요 시 |
| 6 | TLS callback skip (안전 callback 만) | anti-debug 회피 |

### C. cookie 통일 전략

- `__security_cookie` = 0x12345678 (deterministic).
- `__security_cookie_complement` = ~0x12345678.
- process pointer cookie (EncodePointer 용) = 0x12345678 동일.
- 결과: encode/decode/check 가 deterministic 하게 round-trip.

### D. cross-ref 와 변경점

- [`MSVC_ENCODE_POINTER_RESEARCH.md`](../MSVC_ENCODE_POINTER_RESEARCH.md) §1–2
  의 ROL+XOR 알고리즘은 그대로 유효. 단, **caller 가 LTCG 에 의해 inline 되는
  경우의 register-agnostic pattern** 은 본 문서 Q4 / Q5 / 종합 A 가 보강한다.
- [`MSVC_CRT_INIT_BYPASS.md`](../MSVC_CRT_INIT_BYPASS.md) 의 bypass 전략은
  본 문서 Q1 / Q2 / Q3 / Q7 / Q11 의 7-window pattern 으로 강화. 특히 byte
  signature 매칭이 LTCG 에서 작동 안 하던 원인 (Q5) 이 규명됨.

---

## own1 enforcement 재확인

본 research 의 모든 reference (wine, ojdkbuild, libctiny, ReactOS, kallanreed,
DaintyJet 등) 는 **분석/citation only**. 어떤 코드도 airgenome-gamebox 에 lift
하지 않으며, 위 의사코드는 모두 (a) Microsoft Learn 의 공개 documentation 또는
(b) 본 문서 작성자의 trace 관찰에서 도출된 abstraction 만 담는다.

타사 Wine/CrossOver/GPTK/Whisky 추천은 own1 violation 이므로 본 문서에서도
trigger / game-changer 로 등장하지 않으며, 오직 enforcement 측 reference
(airgenome 부모 repo 의 wine 통합 맥락) 로만 간접 인용한다.

---

## 다음 단계 (wave 15 entry)

1. semantic pattern matcher 모듈 (`tool/scan/ltcg_patterns.py` 신규) 작성 —
   Q1–Q11 의 7 fingerprint 를 register-agnostic 으로 encode.
2. `tests/ltcg/` 아래 hand-crafted IL fixtures 로 self-test (hexa runner
   constraints 준수: bash/shasum/git 부재 + Hexa `type` 예약어 회피).
3. Battle.net Setup.exe 의 entry 첫 0x200 byte 에 7 fingerprint 적용 → 매칭
   리포트.
4. cookie 통일 transform 적용 후 wave 11 halt point 재현 시도.
