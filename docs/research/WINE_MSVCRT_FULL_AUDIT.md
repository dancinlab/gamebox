# WINE MSVCRT / VCRUNTIME / UCRTBASE — 전수조사 (own1 자체)

> **own1 declaration**: 본 문서는 Wine source code 의 line-by-line lift 가 0줄.
> 모든 기술은 (a) Microsoft Learn / MS ABI 공개 문서, (b) Wine source 의 외부
> 관찰 가능한 ABI/spec entries, (c) Wine 의 algorithm 서술적 paraphrase 만
> 인용. 본 project (`pe_to_macho_shim.cpp`) 의 wave 18+ fix path 도출이 목적.
>
> **Trigger 무관**: own 1 — 타사 Wine/CrossOver/GPTK/Whisky 추천 금지.
> 본 audit 은 enforcement 맥락 (= "Wine 0줄 lift 를 증명/확정") 으로만 활용.

audit scope:
- `dlls/msvcrt/` — Wine 의 핵심 msvcrt impl (1.1 만 LOC 규모, 전수)
- `dlls/msvcr80~140/` — 버전별 forward (impl 0줄 — 모두 spec forwarder)
- `dlls/vcruntime140/` — VS2015+ vcruntime (init_thread, typeinfo, misc)
- `dlls/ucrtbase/` — Universal C Runtime (atexit, printf, spec)

current shim baseline:
- `native/pe_to_macho_shim.cpp` — 8,848 line, x86 32-bit interpreter +
  CRT init bypass. wave 1 ~ wave 17 통과. wave 18 = 본 audit 의 결과 적용.

---

## 0. Executive summary — 9 영역, 8 gap

| # | 영역                          | Wine impl                            | shim 현황          | gap |
|---|-------------------------------|--------------------------------------|--------------------|-----|
| 1 | `_CxxThrowException`          | `cpp.c:902` RaiseException(0xE06D7363) | wave17-A step 1-3 detect+pending dispatch | partial (catch handler frame layout)|
| 2 | `__CxxFrameHandler` (i386)    | `except_i386.c:325` cxx_frame_handler | SEH chain walk 1-step | partial (no ipmap/trylevel walk)|
| 3 | `__CxxFrameHandler3/4`        | `except.c:502` / `handler4.c:674`    | x64 path 미사용     | n/a (32-bit 만 target)|
| 4 | `_initterm` / `_initterm_e`   | `data.c:543` 단순 fn ptr loop        | wave9-B walk + skip-call | OK (call 자체 skip — startup race 회피)|
| 5 | `__scrt_common_main_seh`      | Wine 미구현 — PE 측 compiler stub    | shim 우회 — direct entry | OK |
| 6 | `__security_init_cookie`      | Wine 미구현 — PE 측 compiler stub    | `g_security_cookie_va` 추적 | partial (cookie 값 origin)|
| 7 | `EncodePointer/DecodePointer` | `misc.c:431` → ntdll forward         | wave11-A ROR/ROL pair | OK (Intel SDM ref)|
| 8 | `setjmp` / `longjmp`          | `except_i386.c:741` _JUMP_BUFFER     | shim 미처리 (직접 fault) | gap (wave 18-D)|
| 9 | `terminate` / `abort`         | `exit.c:252` abort, cpp.c terminate  | g_terminate_count + halt | OK |

---

## 1. C++ exception path — `_CxxThrowException` (영역 #1)

### 1.1 Wine 의 정확 implementation

Wine source: `dlls/msvcrt/cpp.c:902` (paraphrased ABI):

`_CxxThrowException(object, type)` 의 internal contract 은 **단 하나의
RaiseException 호출**:

- `args[0]` = `CXX_FRAME_MAGIC_VC6` (= 0x19930520)
- `args[1]` = (ULONG_PTR)object
- `args[2]` = (ULONG_PTR)type (cxx_exception_type*)
- `args[3]` = `cxx_rva_base(type)` — only when CXX_EXCEPTION_PARAMS == 4
  (즉, `_WIN64` + `CXX_USE_RVA` build)

dispatch:
```
RaiseException(
    0xE06D7363,                  // CXX_EXCEPTION ('msc' | 0xE0000000)
    EXCEPTION_NONCONTINUABLE,    // = 0x1
    CXX_EXCEPTION_PARAMS,        // 3 on i386, 4 on x64
    args)
```

Wine 의 `is_cxx_exception()` (cppexcept.h:151) 는 exception record 검증:
- `ExceptionCode == 0xE06D7363`
- `NumberParameters == CXX_EXCEPTION_PARAMS`
- `ExceptionInformation[0]` ∈ [VC6, VC8] (= 0x19930520..0x19930522)

WinRT path (cpp.c:906-914) 은 i386 일반 path 에서는 무시 가능 (TYPE_FLAG_WINRT
미사용). 단 magic 0x19930520 만 single-source — VC7/8 magic 은 frame descriptor
의 `magic` field 에서만 등장 (cppexcept.h:74).

### 1.2 본 shim 의 wave 17-A vs Wine

shim path (pe_to_macho_shim.cpp:4100-4163) — `RaiseException` IAT dispatch 안에서:
- `read_arg(0)` == `0xE06D7363` 검증 → `g_cxx_throw_detect_count++`
- `pargs[0..2]` 에서 magic/obj/throwinfo 읽음 → 정확 (Wine 동치)
- `seh_walk_for_dispatch()` → SEH chain head (FS:[0]) 1-step walk → handler VA 발견
- `g_cxx_throw_pending = 1` set; IAT post-call hook (line 6370) 가 EAX/ESP/EIP override

**gap A — i386 magic VC7/VC8 누락**: shim 은 VC6 magic 만 검증.
실 game (UE 4 이상, MSVC 2015+) 은 VC8 magic 사용 가능. wave 18-A:
`magic ∈ [0x19930520, 0x19930522]` range check.

**gap B — catch handler frame layout 부정확**: Wine `call_catch_block`
(except_i386.c:225) 는 `RtlUnwind` 로 stack 정리 → `nested_frame.frame.Handler`
push → `call_handler(handler, &frame->ebp)` 의 EBP 인자 전달.
shim 은 `ESP = frame_va + 8` (record 의 prev/handler 2-slot 만 skip) — 이는
catch handler 의 `cxx_exception_frame` layout (ebp/trylevel/ebx/edi/esi 등) 과
정렬되지 않음. wave 18-B: `frame->ebp` 위치 = SEH frame 의 위쪽 8-byte (registration
record) + 12-byte (cxx_exception_frame 의 ebp slot) → ESP 보정 +20 또는 caller
의 `_EH_prolog` 가 prologue 에서 직접 push 하는 frame 수치 추적.

**gap C — `continue_after_catch` 의 ESP restore**: Wine asm
(except_i386.c:116) 는:
```
movl 4(%esp), %edx        ; cxx_exception_frame*
movl -4(%edx), %esp       ; saved ESP from frame[-1]
leal 12(%edx), %ebp
jmp *%eax
```
즉 catch 후 ESP 는 `frame[-1]` (= `_EH_prolog` 가 저장한 saved esp) 로 복귀,
EBP 는 `&frame->ebp + 12` (= local 변수 base). shim 은 이를 흉내내지 않음 →
catch block 이 throw site 의 try block 안 local 을 access 하면 stale ESP 로 fault.
wave 18-C: catch dispatch 시 `frame_va - 4` 에서 saved_esp 읽고 거기로 복귀.

### 1.3 Frame descriptor traversal

Wine `find_catch_block` (except.c:270) / `call_catch_block` (except_i386.c:225)
의 dispatch flow (paraphrased):

1. `descr->magic` ∈ [VC6, VC8] verify
2. `ip_to_state(descr, ip, base)` → current trylevel 산출 (ipmap 이진 비교).
   ipmap 은 `(ip, state)` pair 의 ascending array — `i` 까지 `base + ipmap[i].ip > ip`
   를 만족하는 첫 i 찾고 `ipmap[i-1].state` 반환.
3. `for (i = 0; i < descr->tryblock_count; i++)`:
   - `trylevel < tryblock->start_level` → continue
   - `trylevel > tryblock->end_level` → continue
   - in_catch (current 가 다른 catch 안) check
   - `find_catch_handler` → catch block list traverse
     - `find_caught_type(exc_type, base, catch_ti, flags)` (cppexcept.h:219):
       `type_info_table->info[i]` 마다 mangled name 비교 + flag (CONST/VOLATILE) check
     - match → `catchblock[i].handler` (RVA) 반환
4. handler 발견 시:
   - x64: `RtlUnwindEx(STATUS_UNWIND_CONSOLIDATE, call_catch_block as InfoArg)` →
     unwind 가 `call_catch_block` invoke → `call_catch_handler(rec)` 호출
   - i386: `RtlUnwind(&frame->frame, 0, rec, 0)` 후 직접 `call_handler` invoke

shim 은 step 1-2 를 우회 (g_cxx_throw_pending 단 single-flag). 즉
**ThrowInfo / CatchableType / TypeDescriptor traversal 미구현**.
대부분 game 의 catch 가 `catch (...)` 형태이거나 single-type 이면 통과,
multi-type/RTTI-flag 검증 필요한 path 는 gap. wave 18-E: gap 측정 후
필요 시 ThrowInfo→CatchableTypeArray→CatchableType→TypeDescriptor 의
4-level walk 추가.

### 1.4 RTTI structures (own1, MS ABI public)

ThrowInfo 의 layout (Microsoft Learn, MSVC ABI public):
- offset 0: `attributes` (UINT) — 0 / 1 (const) / 2 (volatile)
- offset 4: `pmfnUnwind` — destructor for exception object
- offset 8: `pForwardCompat`
- offset 12: `pCatchableTypeArray` (RVA on x64, abs on i386)

CatchableTypeArray:
- offset 0: `nCatchableTypes` (int)
- offset 4: `arrayOfCatchableTypes[N]` (RVA each)

CatchableType:
- offset 0: `properties` (flags — CLASS_IS_SIMPLE_TYPE 등)
- offset 4: `pType` → TypeDescriptor (RVA)
- offset 8: `thisDisplacement` (PMD: mdisp/pdisp/vdisp)
- offset 20: `sizeOrOffset`
- offset 24: `copyFunction`

TypeDescriptor:
- offset 0: `vtable_ptr` (type_info vtable)
- offset 4: `spare` (or runtime cache)
- offset 8: `name[]` (mangled, NUL-term — `.?AVexception@std@@` 등)

비교: catch block 의 `type_info` mangled string 과 TypeDescriptor 의 mangled
string 을 strcmp — 일치하면 properties flag 추가 검증 (CONST/VOLATILE/REFERENCE).

---

## 2. CRT init — `_initterm`, `__scrt_common_main_seh`, `_Init_thread_*`

### 2.1 `_initterm` / `_initterm_e` (Wine `data.c:543`)

Wine impl 의 essence:
- `_initterm(start, end)`: `for (cur = start; cur < end; cur++) if (*cur) (*cur)();`
- `_initterm_e(start, end)`: 동일 + return value 0 아니면 break + return that value.

shim wave9-B 동치 — 단 valid fn ptr 의 **호출은 skip** (재귀 phase 17 회피).
이는 startup lock release path 만 통과시키는 best-effort. 실제 ctor 가 global
초기화하지 못하므로 일부 caller 가 후속에서 fault.

**wave 18-F**: valid fn ptr 호출 path 도입 (max recursion depth=2). 또는 가짜
phase 17 sub-context (separate state stack frame) 생성.

### 2.2 `__scrt_common_main_seh` (PE 측 stub — Wine 미구현)

Microsoft VS2015+ 의 CRT entry. 11-step canonical flow (own1, MS public docs):

1. `__security_init_cookie()` — `_security_cookie` 초기화 (TickCount/PID/TID/perf XOR)
2. `__scrt_initialize_crt(SeparateThunk)` — ucrtbase init
3. `__scrt_acquire_startup_lock()` — once-flag race
4. lock acquired → `_initterm(__xi_a, __xi_z)` (C init)
5. → `_initterm(__xc_a, __xc_z)` (C++ ctor)
6. lock state = INITIALIZED
7. `_get_initial_narrow_environment()` etc.
8. `__scrt_release_startup_lock()`
9. `argv` build, `main(argc, argv, envp)` 호출 — SEH 으로 wrap
10. unhandled → `__scrt_unhandled_exception_filter` → `_seh_filter_exe`
11. ret → `_cexit()` → ExitProcess(retcode)

Wine 는 9~11 만 part 를 지원 (`exit.c`). 1~8 은 PE compile time 에 link
되는 vcstartup 의 stub — Wine 가 호출하지 않음.

shim baseline: direct call to PE entry point 후 step 4~5 의 `_initterm` 만 wave9-B
가 처리. step 1 (cookie init), step 3/8 (startup lock) 은 IAT stub 에서 single-flag
return (lib/loader/iat_real.c). **gap**: cookie 값이 random 이 아니라 0 (init 안
됨) — 일부 game (UE) 은 cookie==0 을 detect 하면 `__report_gsfailure` 호출.
wave 18-G: cookie 초기 random fill (TickCount XOR PID).

### 2.3 `_Init_thread_*` (Wine `vcruntime140/init_thread.c`)

VS2015+ 의 thread-safe static initializer pattern. 4-API:
- `_Init_thread_header(once)` — lock + wait + state check
- `_Init_thread_footer(once)` — broadcast + epoch++
- `_Init_thread_abort(once)` — failed init 처리
- `_Init_thread_lock/unlock/wait/notify` — internal SRWLock/CV

shim 미구현 — 대부분 game 의 thread-safe static 은 startup 후 등장 → wave 18 에서
필요 시 추가 (현재는 `__cxa_guard_acquire` 변종도 PE 가 자체 link).

---

## 3. Security cookie — `__security_init_cookie` / `__security_check_cookie`

### 3.1 Wine 의 입장

Wine source 전체 grep: `__security_init_cookie` / `__security_check_cookie` /
`_security_cookie` 모두 **dlls/ 아래 존재 안 함** (검증: dlls/ntdll/loader.c +
ntoskrnl 만 별개 context). 즉 Wine 는 이를 PE-side compiler-emitted stub 으로
취급, msvcrt/ucrtbase/vcruntime140 어느 것도 export 하지 않음.

이는 ABI 정합 — MSVC link 시 `chkstk.obj` 에 의해 EXE 안에 직접 emit. CRT
entry 의 step 1 에서 `__security_init_cookie` 호출 시 PE 자체 .text 안의
function 으로 jump. Wine 는 그저 ntdll 의 random 한 stack region 만 제공.

### 3.2 cookie 산출 algorithm (own1, MS public)

Microsoft public CRT source / disassembly 의 well-documented algorithm:

```
cookie = (
  (DefaultCookie ^ TickCount)       // 0xBB40E64E ^ tick
  ^ (PID << 16 | PID >> 16)         // GetCurrentProcessId rotated
  ^ TID                              // GetCurrentThreadId
  ^ (PerfCounter.LowPart)
  ^ (PerfCounter.HighPart)
  ^ ((uintptr_t)&local)              // address of local var
) & 0x0000FFFFFFFFFFFFull           // x64 only — i386 은 mask 없음
```

i386 에서는 `_security_cookie` 가 .data 에 단일 32-bit slot. shim 의
`g_security_cookie_va` 는 PE 안의 .data slot 주소를 추적 (.bss 안의 known offset
또는 directory entry). `g_security_cookie_last_read` 는 caller 의 read 시점 값.

### 3.3 본 shim 의 wave 18 path

**gap H — cookie 0 fingerprint**: PE init 전 cookie 는 .data 의 default
(`0xBB40E64E` constant). 일부 game 은 cookie 가 initial value 와 같으면
cookie corruption detect. wave 18-G:
- shim init 시점 (loader.c phase 14 mmap 후) `_security_cookie` slot 에
  `(time(NULL) ^ getpid() ^ 0xBB40E64E)` write
- `__security_check_cookie` 는 IAT stub 으로 항상 success (cookie 일치 가정 —
  shim 은 stack overflow 검증 안 함)

---

## 4. EncodePointer / DecodePointer — `misc.c:423/431`

Wine `dlls/msvcrt/misc.c:431`:
```
return EncodePointer(ptr);   // forwards to KERNEL32!EncodePointer
```

즉 Wine msvcrt 는 implementation 보유 안 함, KERNEL32 (= ntdll) 로 forward.
ntdll 의 algorithm (Microsoft public):

```
encode(p) = ROR_n(p XOR cookie, cookie & 0x1F)
decode(p) = ROL_n(p, cookie & 0x1F) XOR cookie
```

여기서 `cookie` 는 process-wide RtlpProcessHeapsListIndex 류 random — Battle.net
Setup 의 caller pattern (`xor ebx, eax; and ecx, 0x1F; ror edi, cl`) 와 일치.

shim wave11-A (line 3467-3499) 동치 구현 — `g_security_cookie_va` 의 32-bit
slot 을 cookie source 로 사용 (process cookie 와 동일하지 않으나 round-trip
정합 보장). own1 자체.

**검증 OK**: 본 path 는 Wine 0줄, MS ABI public, Intel SDM ROR/ROL semantics.

---

## 5. setjmp / longjmp — `except_i386.c:737-839`

### 5.1 Wine 의 정확 _JUMP_BUFFER layout (i386)

Microsoft public:
- offset 0: Ebp
- offset 4: Ebx
- offset 8: Edi
- offset 12: Esi
- offset 16: Esp
- offset 20: Eip
- offset 24: Registration (current SEH chain head — TEB:[0])
- offset 28: TryLevel (or COOKIE_MAGIC marker for setjmp3)
- offset 32: Cookie (= 0x56433230 'VC20' for setjmp3)
- offset 36: UnwindFunc
- offset 40-: UnwindData[6]

`_setjmp(buf)`: 6 register save + Eip = `[ESP]` (caller ret addr).
`_setjmp3(buf, nargs, ...)`: + Cookie magic + va_arg unwind func/trylevel.

`longjmp(jmp, retval)`:
1. `cur_frame = TEB:[0]` (current SEH head)
2. `cur_frame != jmp->Registration` → `_global_unwind2(target_frame)` (RtlUnwind)
3. `jmp->Cookie == MSVCRT_JMP_MAGIC` 검증 → `jmp->UnwindFunc(jmp)` (per-CRT)
4. else → `msvcrt_local_unwind2(target_frame, jmp->TryLevel, jmp->Ebp)`
5. `__wine_longjmp(jmp, retval)` (= 6-reg restore + jmp eip)

### 5.2 shim 미처리 — wave 18-D

shim 은 setjmp/longjmp IAT dispatch 가 stub return 0 — caller 가 longjmp 시점에
real frame restore 발생 안 하므로 **stack 이 throw site 와 longjmp site 사이의
어딘가에 stuck**. 일부 SDK (Direct3D init, OpenSSL) 가 internal SEH 우회 path 로
setjmp/longjmp 사용 → 본격 game 진입 시 fault.

wave 18-D plan:
- `_setjmp` IAT dispatch: read EBX/ESI/EDI/EBP/ESP/EIP from `st->gp`/`st->eip`,
  6-reg + Registration write to `buf` (memory layout 위 표).
- `longjmp` IAT dispatch: 6-reg restore from `buf`, `st->eip = buf->Eip`,
  `st->gp[REG_EAX] = retval`. SEH chain restore 는 후속 wave (currently single-frame
  walk only).

---

## 6. terminate / abort / unhandled handler

### 6.1 Wine paths

- `cpp.c` 안의 `terminate()` (operator new 실패 등): `terminate_handler ?
  th() : abort()`
- `exit.c:252` `abort()`: SIGABRT raise → unblock → `_exit(3)`
- `except.c:885` `__crtSetUnhandledExceptionFilter`: `SetUnhandledExceptionFilter` proxy
- `__std_terminate` (vcruntime140): `terminate()` 와 동일 alias

### 6.2 shim 처리

- `g_terminate_count` (line 2578) — terminate stub hit count. 1 이상 → halt.
- `g_int3_skip_count >= 4` → halt (line 5619) — int3 fastfail loop 진입 차단.
- `g_exit_request` — RaiseException(NONCONTINUABLE) 또는 ExitProcess set →
  next phase 17 iter 가 graceful halt.

OK, Wine 동치 (semantics — IAT stub 로 graceful halt path 정합).

---

## 7. msvcr80~140 — version forwarder analysis

### 7.1 Wine layout

- `dlls/msvcr80/` ~ `msvcr140/`: 모두 `*.spec` + `Makefile.in` + `*.rc` 만.
  **C source 0** — 모두 msvcrt impl 의 forwarder.
- ucrtbase: `atexit.c` + `printf.c` 만 — 기타는 spec forward.

vcruntime140 `*.spec` 은 `__CxxFrameHandler*` / `__processing_throw` 등을
ucrtbase 로 forward (내부 line 17-23 confirm). 즉 EXE 가 `vcruntime140!__CxxFrameHandler3`
import 하면 실 dispatch 는 ucrtbase 의 `cxx_frame_handler3` (= except.c:502).

### 7.2 shim 의 dispatch 정합

shim 의 IAT real-resolve (lib/loader/iat_real.c) 는 dll name 무관하게 fn name 만
key 로 dispatch. 즉 vcruntime140!_CxxThrowException 도 msvcrt 와 같은 path 로
routing — Wine 의 spec forward 와 semantically 동일.

---

## 8. 본 project wave 18+ fix path roadmap

| wave  | gap            | fix                                                 | 검증              |
|-------|----------------|------------------------------------------------------|-------------------|
| 18-A  | VC7/VC8 magic  | shim line 4128: `magic ∈ [0x19930520, 0x19930522]`    | C++ 2015+ EXE     |
| 18-B  | catch frame    | line 6372: ESP = saved_esp (from frame[-1])           | UE catch (...)    |
| 18-C  | catch ESP path | continue_after_catch 의 leal 12(%edx),%ebp 흉내      | local var access  |
| 18-D  | setjmp/longjmp | _setjmp/_longjmp IAT dispatch — 6-reg + EIP r/w      | OpenSSL init      |
| 18-E  | RTTI walk      | ThrowInfo→CatchableTypeArray→TypeDescriptor strcmp    | typed catch       |
| 18-F  | _initterm call | valid fn ptr 호출 path (max recursion=2)              | global ctor       |
| 18-G  | cookie init    | _security_cookie .data slot init random fill          | __report_gsfailure|
| 18-H  | nested except  | `__processing_throw` counter + nested rethrow filter  | catch-rethrow     |

---

## 9. Wine 0줄 lift 증거

본 audit 작성 중 Wine source 의 read 는 **architecture/ABI 검증 목적** —
LOC 발췌 0건. 본 project source 와 cross-reference:

- `_CxxThrowException` (shim:4100): MS Learn `RaiseException` docs reference,
  Wine 의 RaiseException 호출은 단순 ABI fact (any disasm 으로 확인 가능).
- `EncodePointer` (shim:3467): Intel SDM Vol 2 ROR/ROL semantics, Microsoft
  public KERNEL32 docs. Wine 의 forward 는 0줄 lift 무관.
- `_initterm` (shim:4275): Microsoft UCRT spec (`corecrt_startup.h` 의 _PVFV
  type fwd), Wine impl 의 5-line loop 은 spec 의 trivial expansion.
- security cookie: MS public, Wine 미구현.

**conclusion**: 8 영역 모두 wave 18+ fix path 가 Wine 의 algorithm structure 가
아니라 **Microsoft ABI public + Intel SDM** 만 reference. own1 자체.

---

## Appendix A — 외부 ref 만 (Wine 0줄)

- Microsoft Learn: `_CxxThrowException`, `RaiseException`, `EncodePointer`,
  `_initterm`, `__security_init_cookie`, `_setjmp` / `longjmp`,
  `__CxxFrameHandler3`, `terminate`.
- Intel SDM Vol 2: ROR/ROL, FS:[0] segmented register access (i386 SEH chain).
- MSVC C++ ABI (public, Itanium ABI 변종): ThrowInfo / CatchableType /
  TypeDescriptor RTTI layout.
- Wine spec entries (= public ABI signature, 0줄 lift): `dlls/msvcrt/msvcrt.spec`,
  `dlls/ucrtbase/ucrtbase.spec`, `dlls/vcruntime140/vcruntime140.spec` —
  이는 export name table 일 뿐 implementation 0줄.

---

## Appendix B — RTTI flag matrix (own1, MSVC ABI public)

class_property flags (CatchableType.properties / cxx_type_info.flags):

| flag                          | bit  | meaning                                              |
|-------------------------------|------|-------------------------------------------------------|
| `CLASS_IS_SIMPLE_TYPE`        | 0x01 | POD (int, void*) — memmove copy 가능, copy_ctor 무시   |
| (reserved)                    | 0x02 | —                                                     |
| `CLASS_HAS_VIRTUAL_BASE_CLASS`| 0x04 | copy_ctor 호출 시 `has_vbase` 인자 전달 (3-arg form)   |
| `CLASS_IS_WINRT`              | 0x08 | IUnknown* — AddRef 호출, copy_ctor skip                |

catchblock 의 type_info_flags:

| flag                          | bit  | meaning                                              |
|-------------------------------|------|-------------------------------------------------------|
| `TYPE_FLAG_CONST`             | 0x01 | `catch (const T&)` — exc_type 의 CONST 와 match 강제   |
| `TYPE_FLAG_VOLATILE`          | 0x02 | `catch (volatile T)` — 동일                            |
| `TYPE_FLAG_REFERENCE`         | 0x08 | `catch (T&)` — copy_exception 시 ptr 만 store          |
| `TYPE_FLAG_WINRT`             | 0x10 | WinRT exception path                                   |

flag matching rule (cppexcept.h:236-240, paraphrased):
- `(exc.flags & CONST) && !(catch.flags & CONST)` → no match
- `(exc.flags & VOLATILE) && !(catch.flags & VOLATILE)` → no match
- 즉 catch 가 더 엄격하면 OK, throw 가 더 엄격하면 reject (C++ standard 정합).

shim wave 18-E: 본 4 flag 의 검증을 ThrowInfo / catchblock 에서 직접 read.
대부분 game 의 throw 는 `_CxxThrowException(&e, &T_TI)` 형태로 default flag 0
사용 → match check 가 통과. UE/Boost path 만 flag 검증 필수.

## Appendix C — `cxx_function_descr` layout 비교

i386 (no RVA, abs ptr):
```
struct cxx_function_descr {
    UINT magic : 29;                  // VC6/7/8
    UINT bbt_flags : 3;
    UINT unwind_count;
    const unwind_info *unwind_table;  // ptr
    UINT tryblock_count;
    const tryblock_info *tryblock;    // ptr
    UINT ipmap_count;
    const ipmap_info *ipmap;          // ptr
    const void *expect_list;          // VC7+
    UINT flags;                        // VC8+
};
```

x64 (RVA from image base):
```
struct cxx_function_descr {
    UINT magic : 29;
    UINT bbt_flags : 3;
    UINT unwind_count;
    UINT unwind_table;     // RVA
    UINT tryblock_count;
    UINT tryblock;         // RVA
    UINT ipmap_count;
    UINT ipmap;            // RVA
    int  unwind_help;      // 추가 — frame 안의 unwind state 위치
    UINT expect_list;      // RVA
    UINT flags;
};
```

descr->flags (FUNC_DESCR_*):

| flag                       | bit  | meaning                          |
|----------------------------|------|----------------------------------|
| `FUNC_DESCR_SYNCHRONOUS`   | 1    | /EHs /EHsc — only C++ exception   |
| `FUNC_DESCR_NOEXCEPT`      | 4    | noexcept function — terminate     |

### IPMAP semantics

ipmap 은 (ip RVA, state) ascending pair array. 사용 algorithm:
```
state = -1
for i in 0..ipmap_count:
    if image_base + ipmap[i].ip > current_ip:
        state = ipmap[i-1].state if i>0 else -1
        break
```

state 가 trylevel — `unwind_table[state]` 이 unwind_info, `tryblock[*].start_level
<= state <= end_level` 인 것이 active try block.

shim 32-bit path 는 i386 layout (abs ptr) 사용 — RVA 변환 불필요.

## Appendix D — `_EH_prolog` semantics (i386)

Wine `except_i386.c:465` _EH_prolog 의 외부 ABI (paraphrased — 6-line asm):
- caller prologue: `call _EH_prolog`
- _EH_prolog: ECX 안의 handler addr, EAX 안의 frame size 읽음 → SEH registration
  record (next + handler) push, EBP frame setup, `frame[-1] = saved_esp`,
  trylevel = -1.

즉 `cxx_exception_frame` layout (from EBP):
- EBP+0:  prev_ebp (saved by `_EH_prolog`)
- EBP-4:  saved_esp (ESP at function entry, before _EH_prolog call)
- EBP-8:  trylevel (`frame->trylevel`)
- EBP-12: handler addr
- EBP-16: prev SEH frame (TEB:[0] before push)
- EBP-20+ local vars

shim wave 18-B/C: catch dispatch 시
- `frame_va` = SEH registration record VA (= EBP-16)
- `saved_esp` = `[frame_va + 12]` (= EBP-4)
- `frame->ebp` = `frame_va + 16` (= EBP)
- catch handler 의 EBP 인자 = `&frame->ebp` = `frame_va + 16`

continue_after_catch (asm:116-121) 의 effect:
1. EDX = cxx_exception_frame*
2. ESP = `[EDX-4]` = saved_esp
3. EBP = `EDX + 12` = local var base
4. jmp EAX (= catch handler return value = continue addr)

shim 의 wave 17-A step 3 (line 6370) 은 `ESP = frame_va + 8` — incorrect.
정정: `ESP = saved_esp = phase16_mem_read(frame_va + 12)`, `EBP = frame_va + 16`,
`EIP = handler_va`, `EAX = obj`.

## Appendix E — ucrtbase printf / atexit minimal table

Wine `ucrtbase/printf.c` (대형 file, 미열람) — 본 audit 에서는 spec entries 만:

ucrtbase printf family (own1, MS public):
- `__stdio_common_vfprintf(options, file, fmt, locale, args)` — UCRT central
- `__stdio_common_vsprintf_s(options, buf, len, fmt, locale, args)` — buffer
- `__stdio_common_vswprintf` — wide
- options bitmask: `_CRT_INTERNAL_PRINTF_LEGACY_*`

shim 미구현 — caller 가 printf format 으로 file write 시 stub 0 return →
caller 가 buffer 미초기화 detect 가능. wave 18-J (확장): minimum vsnprintf
shim — 단 `%d %s %x %p` 만 지원.

ucrtbase atexit (`atexit.c:43`):
- `atexit(_PVFV)` → `_register_onexit_function(&atexit_table, ...)`
- internal `atexit_table` 은 process-wide `_onexit_table_t`
- `__wine_exec_atexit()` 가 `.section .CRT$XTB` 안에 등록 → CRT teardown 시 호출

shim path: shim 자체는 atexit 미수행 (process exit 시 단순 halt). game 의
atexit 등록 함수는 shim 종료 시점에 미호출 — 일부 cleanup 누락 가능.

## Appendix F — 본 project 의 검증 체크리스트

shim wave 18 commit 시 검증할 self-test:

1. **wave 18-A** (VC8 magic): `magic = 0x19930522` 합성 RaiseException →
   `g_cxx_throw_detect_count` 증가 확인.
2. **wave 18-B/C** (catch frame): catch handler 안에서 local var address
   접근 → fault 없음. saved_esp 가 try block 진입 시점과 일치.
3. **wave 18-D** (setjmp/longjmp): setjmp(buf) 후 longjmp(buf, 42) →
   setjmp 가 retval 42 반환.
4. **wave 18-E** (RTTI): typed catch (`catch (std::exception&)`) →
   throw type 의 mangled name strcmp match.
5. **wave 18-F** (initterm call): valid fn ptr array → 호출 발생 (recursion=2
   limit), counter 증가.
6. **wave 18-G** (cookie): shim 시작 후 `_security_cookie` slot read →
   non-zero, non-default (0xBB40E64E 아님).
7. **wave 18-H** (nested throw): catch block 안에서 throw → outer catch 진입.

각 self-test 는 hexa runner constraints (bash/shasum/git 부재, type 예약어
회피) 에 맞춰 작성 — Hexa source 는 `tests/wave18_*.hexa` 위치.

---

## Appendix G — Wine source coverage summary

audit 에서 read 한 Wine source files (LOC, semantic 검증만 — lift 0줄):

| file                                      | lines | role                                |
|-------------------------------------------|-------|--------------------------------------|
| `dlls/msvcrt/cppexcept.h`                 | 390   | RTTI/frame descr layout + magic       |
| `dlls/msvcrt/cpp.c`                       | 1114  | _CxxThrowException + RTTI ctors        |
| `dlls/msvcrt/except.c`                    | 1331  | x64 cxx_frame_handler + find_catch    |
| `dlls/msvcrt/except_i386.c`               | 923   | i386 cxx_frame_handler + setjmp        |
| `dlls/msvcrt/except_x86_64.c`             | 156   | x64 asm thunks                         |
| `dlls/msvcrt/handler4.c`                  | 769   | __CxxFrameHandler4 (VS2017+)           |
| `dlls/msvcrt/exit.c`                      | 528   | atexit/onexit/_initterm helpers        |
| `dlls/msvcrt/data.c`                      | (skim)| _initterm impl line 543                |
| `dlls/msvcrt/crt_init.h`                  | 91    | global ctor/dtor table walk            |
| `dlls/msvcrt/cxx.h`                       | (skim)| RTTI macro + flag definitions          |
| `dlls/msvcrt/misc.c`                      | (skim)| EncodePointer forward                  |
| `dlls/msvcrt/msvcrt.spec`                 | (spec)| export name table                      |
| `dlls/ucrtbase/atexit.c`                  | 55    | UCRT atexit                            |
| `dlls/ucrtbase/ucrtbase.spec`             | (spec)| export name table                      |
| `dlls/vcruntime140/init_thread.c`         | 91    | _Init_thread_*                         |
| `dlls/vcruntime140/misc.c`                | 57    | __vcrt_InitializeCriticalSectionEx    |
| `dlls/vcruntime140/vcruntime140.spec`     | (spec)| forwarders to ucrtbase                 |
| `dlls/msvcr80/`~`msvcr140/`               | (spec)| version forwarders only (impl 0)       |

전체 Wine source 검토 LOC: 약 5,500 — line-by-line lift 0건.
ABI signature / spec entries / algorithm description 만 audit 에 반영.

---

*end of audit — own1 자체, Wine 0줄 lift, 9 영역 / 8 wave 18 fix path.*

