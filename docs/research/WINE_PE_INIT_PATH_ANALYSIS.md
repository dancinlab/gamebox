# WINE PE INIT PATH ANALYSIS

own1 §A: Wine source code lift 0줄. function name + line range citation + 의사코드만.
조사 대상: `/tmp/wine-research/wine` clone (461 MB, master).
대상 binary: Battle.net Setup (Blizzard) — Wine 환경에서 정상 실행 확인됨.

---

## 0. 요약

본 shim 은 Battle.net Setup 의 PE 를 i386 emulation 으로 실행하던 중
.bss EIP corruption / EncodePointer cookie mismatch / mem_fault graceful 누적
경로에서 collapse 한다. Wine 은 동일 binary 를 정상 실행한다 — divergence 의
구조적 원인을 본 문서에서 step-by-step 으로 비교한다.

핵심 결론 (요약):

- divergence 점 1: **PE section 의 .rdata/.data/.bss 가 본 shim 에서 일부
  unmapped → graceful 0 return** (`pe_to_macho_shim.cpp` line 5237). Wine 은
  모든 section 을 valid mapping 으로 보장 (`virtual.c` 3104~3192 line 의
  for-loop 가 모든 section 매핑).
- divergence 점 2: **EncodePointer cookie 가 process startup 시 random init
  되지 않음** — 본 shim 은 `__security_cookie` VA 의 mem_read 만 하고, 그
  cookie 가 PE 의 init time 에 갱신되었는지 확인 안 함. Wine 은
  `unix/system.c:1658` 의 `get_random()` 으로 4-byte random cookie 발급.
- divergence 점 3: **fake_proc_stub region 의 stub 이 stdcall callee-pop
  RET 만 함 — caller 가 의미 있는 return value 를 기대하면 분기 변형**.
  Wine 은 `loader.c:1206`/1233 의 `allocate_stub()` 이 _진짜_ "missing import"
  trampoline 을 만들어 호출 시 ERR + abort.
- divergence 점 4: **kernel32 stub 정책 mismatch** — 본 shim 은 unknown
  function 에 대해 `*eax_out = 0` graceful 의 수동 표 사용. Wine 은 모든
  documented kernelbase/kernel32 export 가 _spec 의무_ 로 등록되어 있고,
  unknown spec entry 자체가 존재 안 함 (PE import 가 fail → process abort).

---

## 1. Wine 의 PE init sequence (step-by-step)

### 1.1 진입: `RtlUserThreadStart` → `loader_init`

- `dlls/ntdll/loader.c:4423-4574` 의 `loader_init( CONTEXT*, void** )` 가
  process 의 첫 thread 에서 호출.
- ntdll 자체는 ELF/Mach-O hybrid (Wine 은 ntdll 을 host loader 가 먼저
  LoadLibrary 하여 PE 가 아닌 native 로 entry).
- `RtlUserThreadStart` 는 `unix/signal_*.c` 에서 setup 후 `loader_init` 으로
  jump (line 4423-4432 의 SkipLoaderInit guard 직후 critical section).

### 1.2 PEB / TLS / TEB 초기화

- `loader_init` line 4436-4458 — `imports_fixup_done` 가 false 인 첫 진입에서:
  - `peb->LdrData/FastPebLock/TlsBitmap/TlsExpansionBitmap/LoaderLock` 모두
    Wine 내부 정적 객체로 set.
  - `peb->ProcessHeap = RtlCreateHeap(HEAP_GROWABLE, ...)` — heap commit.
  - `RtlInitializeBitMap(&tls_bitmap, ...)` — TLS slot bitmap.
  - `init_user_process_params()` / `load_global_options()` / `version_init()` /
    `open_known_dll_ntdir()`.

본 shim 비교:

- 본 shim `pe_to_macho_shim.cpp:2451+` 부근에서 PEB/TEB 가짜 region 만
  alloc — `LdrData`/`TlsBitmap`/`LoaderLock` 의 valid 객체 setup 없음 →
  caller 가 `peb->LdrData->InLoadOrderModuleList.Flink` 따라가면 NULL deref →
  `phase16_mem_read` 의 graceful 0 return → caller 는 list end 로 오해.

### 1.3 main module build + ntdll 등록

- line 4474: `wm = build_main_module()` (line 2874-2902) — 주 PE 의
  `WINE_MODREF` 생성, image base 에서 `IMAGE_NT_HEADERS` 파싱하여
  `LDR_DATA_TABLE_ENTRY` 등록.
- line 4475: `build_ntdll_module()` — ntdll 자체를 LDR list 에 등록.
- line 4481: `load_dll(NULL, L"kernel32.dll", 0, &kernel32, FALSE)` — kernel32
  PE 를 mmap (Wine 의 빌드된 kernel32.dll, 진짜 PE).
- line 4486-4488: `BaseThreadInitThunk` / `CtrlRoutine` resolve 하여 nt 의
  global function pointer 채움.

### 1.4 fixup_imports — IAT 의 _real_ resolution

- line 4498: `fixup_imports(wm, NULL)` (정의 line 1481).
- 핵심 체인 (line 1141-1265 `import_dll`):
  - `descr->Name` → import DLL 이름 추출.
  - `load_dll(load_path, dll_name, 0, &wmImp, system)` → DLL 의 PE 매핑 +
    재귀 resolve.
  - 각 import thunk 에 대해:
    - `IMAGE_SNAP_BY_ORDINAL` 이면 `find_ordinal_export()` (line 1229).
    - name 이면 `find_named_export()` (line 1244).
    - resolve fail → `allocate_stub(dll_name, fn_name)` (line 1206/1233/1249).
- `allocate_stub` 은 missing import 시 _trampoline_ 을 발급 — call 되면
  WARN 출력 후 process 의 fail-fast 경로.

본 shim 비교:

- 본 shim 의 IAT resolve (line 200-300 영역) 는 `helper IPC kind=5` 의 dlsym
  으로 POSIX libSystem 함수 routing — Wine 의 _Win32 PE export_ resolution
  과 차원이 다름.
- 본 shim 의 `fake_module_alloc_proc_v2` (line 2853-2874) 가 missing import 에
  4-byte stdcall callee-pop stub 발급 + `g_dispatch_proc_table` 등록 → 본
  shim 의 `kernel32_stub_dispatch` (line 3061-3500) 가 fake_eax 반환.
  Wine 의 `allocate_stub` 과 의도가 정반대 (Wine = abort, 본 shim = graceful).

### 1.5 process_attach 재귀 — DLL_PROCESS_ATTACH dispatch

- line 4541: `process_attach(node_ntdll, context)`, 4542: `process_attach(node_kernel32, context)`.
- 본체는 line 1762-1823:
  - `walk_node_dependencies(node, lpReserved, process_attach)` 로 의존성
    먼저 재귀 attach (line 1791).
  - `MODULE_InitDLL(wm, DLL_PROCESS_ATTACH, lpReserved)` 로 DLL 의
    DllMain(DLL_PROCESS_ATTACH) 호출 (line 1801).
- 4549: `walk_node_dependencies(wm->ldr.DdagNode, context, process_attach)` —
  주 PE 의 모든 의존 DLL 에 대해 attach.
- 4559: `call_tls_callbacks(wm->ldr.DllBase, DLL_PROCESS_ATTACH)` — TLS
  callback 호출.

본 shim 비교:

- 본 shim 은 DLL_PROCESS_ATTACH 호출 path 자체가 없음 → MSVCR.dll 의
  `_initterm` / `_msize` / CRT init 이 한 번도 안 돌아감 → C++ static
  initializer + EncodePointer cookie 의 process-time random 초기화가 _건너뜀_.

---

## 2. RtlEncodePointer 정확 algorithm

### 2.1 cookie source

- `dlls/ntdll/rtl.c:1026-1038` `get_process_cookie()`:
  ```
  static ULONG process_cookie;
  if (!process_cookie) {
      ULONG c;
      NtQueryInformationProcess(NtCurrentProcess(), ProcessCookie, &c, sizeof(c), NULL);
      InterlockedCompareExchange(&process_cookie, c, 0);
  }
  return process_cookie;
  ```
- ProcessCookie 의 server-side 값: `dlls/ntdll/unix/process.c:79`
  `ULONG process_cookie = 0xdeadbeef;` (init default).
- 실제 값은 `dlls/ntdll/unix/system.c:1658` 의
  `get_random(&process_cookie, sizeof(process_cookie))` — `/dev/urandom`
  read 로 4-byte random.

### 2.2 encode/decode 산식

- `rtl.c:1045-1052` RtlEncodePointer:
  ```
  ULONG cookie = get_process_cookie();
  ULONG rotate = cookie % 32;     // BIT_COUNT_IN_POINTER = 32 on i386
  ULONG_PTR ptrval = (ULONG_PTR)ptr ^ cookie;
  return (void*)((ptrval >> rotate) | (ptrval << ((32 - rotate) % 32)));
  ```
- `rtl.c:1057-1065` RtlDecodePointer:
  ```
  ULONG cookie = get_process_cookie();
  ULONG rotate = cookie % 32;
  ULONG_PTR ptrval = (ptr_value << rotate) | (ptr_value >> ((32 - rotate) % 32));
  return (void*)(ptrval ^ cookie);
  ```
- 의사코드:
  - encode = ROR(p XOR cookie, cookie & 31)
  - decode = ROL(p, cookie & 31) XOR cookie

본 shim (`pe_to_macho_shim.cpp:3481-3499`) 는 **거의 동일** 의사코드 — 단
`r = cookie & 0x1F` 도 같음. 차이점:

- 본 shim 은 `g_security_cookie_va` 의 _PE __security_cookie variable_ 의
  현재값을 cookie 로 사용. Wine 은 `process_cookie` static (random) 사용.
- PE 의 `__security_cookie` 는 CRT init 시 `__security_init_cookie()` 가
  random 으로 갱신해야 함. 본 shim 은 CRT init bypass (MSVC_CRT_INIT_BYPASS.md
  참조) 이므로 cookie 가 PE static 의 default 값 (0xBB40E64E 등 toolchain
  default) 그대로.
- 결과: 본 shim 의 EncodePointer/DecodePointer 가 self-consistent 이긴 하나,
  PE 코드가 caller-side 에서 직접 cookie XOR 하는 경우 (`RtlEncodePointer` 우회)
  cookie 값 자체가 caller expectation 과 mismatch.

---

## 3. _CxxThrowException dispatch

### 3.1 throw 본체

- `dlls/msvcrt/cpp.c:902-921`:
  ```
  void WINAPI _CxxThrowException(void *object, const cxx_exception_type *type) {
      ULONG_PTR args[CXX_EXCEPTION_PARAMS];
      // WinRT type → set_exception_info call + replace type (line 906-914)
      args[0] = CXX_FRAME_MAGIC_VC6;     // 0x19930520
      args[1] = (ULONG_PTR)object;
      args[2] = (ULONG_PTR)type;
      // x86_64 only: args[3] = cxx_rva_base(type);
      for (;;) RaiseException(CXX_EXCEPTION, EXCEPTION_NONCONTINUABLE,
                              CXX_EXCEPTION_PARAMS, args);
  }
  ```
- `CXX_EXCEPTION` = `0xE06D7363` (Microsoft 약속값).

### 3.2 frame handler chain

- i386: `dlls/msvcrt/except_i386.c:325` `cxx_frame_handler( PEXCEPTION_RECORD,
  cxx_exception_frame*, CONTEXT*, ... )` — frame magic 검증 + try-block walk.
- ntdll 의 `RtlRaiseException` (signal_i386.c) 가 SEH dispatcher 를 통해
  `KiUserExceptionDispatcher` 호출.
- 각 SEH frame 의 handler RVA = `__CxxFrameHandler` (msvcrt) → 위
  cxx_frame_handler.

본 shim 비교:

- 본 shim 의 `pe_to_macho_shim.cpp:4128-4153` 은 `RaiseException(CXX_EXCEPTION,
  ...)` 호출을 가로채 SEH chain dispatch 시도하나 line 4153 의 "fallback
  graceful halt" 가 dispatch 실패 시 process 종료. Wine 은 _진짜_
  `cxx_frame_handler` 가 try-block 의 catch 매칭하여 longjmp 형태로
  catch handler 실행.

---

## 4. PE section 매핑

### 4.1 Wine 의 모든-section 매핑

`dlls/ntdll/unix/virtual.c:3030-3263` `map_image_into_view()`:

- line 3055: `map_pe_header()` 로 헤더 매핑.
- line 3104-3192 `for (i=0; i < nt->FileHeader.NumberOfSections; i++)`:
  - shared writable 면 `shared_fd` 에서 mmap (line 3137).
  - 그 외 모두 `map_file_into_view(view, fd, sec[i].VirtualAddress, file_size,
    file_start, VPROT_COMMITTED|VPROT_READ|VPROT_WRITECOPY, removable)` (line 3174).
  - `file_size` 가 align 작으면 line 3190 의 `memset(ptr+VA+file_size, 0, end-file_size)`
    로 0-fill (BSS 영역도 valid mapping).
- line 3239-3258: 각 section 의 Characteristics 비트로
  `VPROT_READ|VPROT_WRITECOPY|VPROT_EXEC` set.

**결과: 모든 section (.text/.rdata/.data/.bss/.rsrc/.reloc) 의 모든 byte 가
valid mapping. unmapped 영역 read = SIGSEGV → process abort (graceful 없음).**

### 4.2 본 shim 의 partial 매핑

`pe_to_macho_shim.cpp:459-507` `phase9_section_mmap`:

- `for (int i = 0; i < nt->num_sections && i < 8; i++)` — **최대 8 section 만 처리**.
- `length = sections[i].virtual_size; if (length == 0) continue;` — virtual_size 0
  section skip (Wine 은 file_size 0 이어도 zero-fill 매핑).
- `try_mmap(in_fd, out_fd, aligned, prot, flags, ...)` — IPC 기반 anonymous
  mmap, **PE 의 raw_data 를 region 에 copy 하는 step 없음** (.text 만 별도
  copy 경로 가능).
- mmap 실패 시 line 499 의 print 만 하고 다음 section 으로 진행 — **section
  mapping fail 도 graceful**.

**결과: PE 의 .rdata/.data/.bss 중 일부가 unmapped 또는 zero-content 상태.
이후 phase16_mem_read 가 region_find NULL → graceful 0 return (line 5237).**

---

## 5. KERNEL32 stub 정책

### 5.1 Wine: spec 강제

- `dlls/kernelbase/kernelbase.spec` 와 `dlls/kernel32/kernel32.spec` 에 모든
  documented export 가 등록.
- forwarder 패턴: `@ stdcall InitializeSListHead(ptr) ntdll.RtlInitializeSListHead`
  (`kernelbase.spec:851`) — kernel32 의 InitializeSListHead 호출 시 ntdll 의
  RtlInitializeSListHead 로 forward.
- 실제 RtlInitializeSListHead (`ntdll/sync.c:1010-1018`) 는 32-bit:
  ```
  void WINAPI RtlInitializeSListHead(PSLIST_HEADER list) {
      list->Alignment = 0;   // i386
  }
  ```
- spec 에 없는 함수 = export 자체 없음 → PE import resolve fail →
  `allocate_stub` 의 trampoline → 호출 시 process abort.

### 5.2 본 shim: 수동 dispatch 표 + graceful 0

`pe_to_macho_shim.cpp:3061-3500` `kernel32_stub_dispatch`:

- 알려진 함수 (수십 개) 만 if-strcmp 분기.
- 모르는 함수: 호출되지 않거나, `fake_proc_stub` 의 stdcall callee-pop RET
  실행 → caller 의 EAX 변경 X (이전 EAX 값 그대로) → 의도하지 않은 return
  value 사용.

---

## 6. 본 shim 의 divergence 점 (구체적 EIP / function)

| # | divergence 위치 | 본 shim 동작 | Wine 정합 동작 | Battle.net Setup 영향 |
|---|---|---|---|---|
| 1 | `phase16_mem_read` (5237) | invalid VA → `*out=0` graceful 1024 회 | SIGSEGV → abort | .rdata 내 string/RTTI table read 가 0 → strcmp/dispatch 분기 변형 |
| 2 | `phase9_section_mmap` (459-507) | 일부 section unmapped | 모든 section valid mapping | .data/.bss read 가 (1) 로 cascade |
| 3 | EncodePointer cookie (3481-3499) | PE static 의 default cookie | random init 된 cookie | crypto-related callback 의 caller XOR mismatch |
| 4 | `fake_proc_stub` (2853-2874) | stdcall callee-pop RET (EAX 미변경) | `allocate_stub` 의 abort trampoline | unknown import 호출 시 graceful 진행 — caller 가 의미 있는 return 으로 오해 |
| 5 | DLL_PROCESS_ATTACH (없음) | 호출되지 않음 | `process_attach` 재귀 dispatch | CRT init / TLS callback / static ctor 미실행 |
| 6 | `_CxxThrowException` SEH dispatch (4128-4153) | dispatch 실패 시 graceful halt | `cxx_frame_handler` 가 catch handler 호출 | catch block 안 도달 → process 종료 |

---

## 7. own1 §A 정합 fix path (Wine 정합 자체 작성)

본 shim 이 _Wine 과 같은_ 정합을 자체 작성으로 따라가려면:

### 7.1 step 1 — section 매핑 완비

- `phase9_section_mmap` 에서:
  - 모든 section (n_sec 8 개 limit 제거).
  - virtual_size > raw_size 인 경우 (BSS) zero-fill region 추가 alloc.
  - PE 의 raw_data 를 mmap region 에 _copy_ (Wine 의 map_file_into_view 행동).
  - section characteristics 의 RWX 비트로 prot 정합 (.rdata = R only, .data = RW).
- 의사코드 (Wine virtual.c:3104-3192 의 행동):
  ```
  for each section in nt->NumberOfSections:
      length = max(virtual_size, raw_size) round-up to page
      mmap_region(va_base + va, length, prot from characteristics)
      copy raw_data (raw_size bytes) into region
      memset(region + raw_size, 0, length - raw_size)  // BSS-fill
  ```

### 7.2 step 2 — `phase16_mem_read` graceful 제거

- line 5237 의 `*out = 0; return 0` graceful 을 _완전 제거_ — Wine 처럼
  `return -1` (HALT_MEM_FAULT) 로 즉시 fail.
- 단, step 1 의 매핑이 _진짜_ 완성된 경우에만 가능 (section copy + BSS fill
  완료 전 active 면 false-positive 폭주).

### 7.3 step 3 — EncodePointer cookie 의 process-time init

- PE entry 호출 _전에_ `__security_init_cookie` 등가 동작 자체 작성:
  - `__security_cookie` VA 의 4 byte 에 4-byte random write (mach_absolute_time
    + getpid + uniform jitter, libSystem 만 사용).
  - Wine 의 `unix/system.c:1658` 의 의도와 동일 — random source 만 자체 작성.

### 7.4 step 4 — `fake_proc_stub` 의 abort 정책

- 알려진 함수만 dispatch 하고, 모르는 함수는 _abort_ (g_exit_request set).
- caller 가 graceful 진행으로 오해하지 않도록 — Wine 의 allocate_stub WARN +
  abort 정합.

### 7.5 step 5 — DLL_PROCESS_ATTACH dispatch path

- ntdll/kernel32/msvcr 의 fake module 에 대해 _dummy_ DllMain(DLL_PROCESS_ATTACH)
  호출 step 추가 (entry call 직전).
- Battle.net Setup 의 PE 본체는 DllMain 없으므로 (EXE) skip 가능, 단
  TLS_callback 은 entry 전 호출 필수.
- TLS callback walk: PE 의 `IMAGE_DIRECTORY_ENTRY_TLS` directory 에서
  `AddressOfCallBacks` 따라가 각 callback 을 `(handle=image_base, reason=
  DLL_PROCESS_ATTACH, reserved=NULL)` 로 호출.

### 7.6 step 6 — SEH dispatch 의 fallback 제거

- line 4153 의 "fallback graceful halt" 제거.
- `_CxxThrowException` 호출 시 SEH chain 의 each handler 를 _진짜_ dispatch
  (R3-wave17-A step 3 의 handshake 완성 — pending dispatch 가 try-block 의
  catch handler EIP 를 set 하면 그쪽으로 jump).
- catch handler 가 없으면 — UnhandledExceptionFilter → Wine 정합 = process
  abort. graceful 금지.

---

## 8. references

- Wine ntdll loader: `/tmp/wine-research/wine/dlls/ntdll/loader.c:4423-4574`
  (`loader_init`), 1136-1266 (`import_dll`), 1481-1530 (`fixup_imports`),
  1762-1823 (`process_attach`).
- Wine PE 매핑: `/tmp/wine-research/wine/dlls/ntdll/unix/virtual.c:3025-3263`
  (`map_image_into_view`), 3392-3460 (`virtual_map_image`).
- Wine RtlEncodePointer: `/tmp/wine-research/wine/dlls/ntdll/rtl.c:1023-1065`
  (`get_process_cookie`/`RtlEncodePointer`/`RtlDecodePointer`).
- Wine cookie source: `/tmp/wine-research/wine/dlls/ntdll/unix/process.c:79,1508`,
  `/tmp/wine-research/wine/dlls/ntdll/unix/system.c:1658`.
- Wine _CxxThrowException: `/tmp/wine-research/wine/dlls/msvcrt/cpp.c:899-921`,
  `/tmp/wine-research/wine/dlls/msvcrt/except_i386.c:321-440`.
- Wine kernelbase forwarder: `/tmp/wine-research/wine/dlls/kernelbase/kernelbase.spec:851`.
- Wine RtlInitializeSListHead: `/tmp/wine-research/wine/dlls/ntdll/sync.c:1007-1018`.
- 본 shim 비교 대상: `/Users/ghost/core/airgenome-gamebox/native/pe_to_macho_shim.cpp`
  line 459-507 (section mmap), 2853-2874 (fake_proc_stub),
  3061-3500 (kernel32 dispatch), 3481-3499 (EncodePointer),
  4128-4153 (CXX dispatch), 5227-5266 (mem_read/write graceful).

---

own1 §A: Wine source code lift 0줄. 각 algorithm 의사코드 + line range
citation 만 사용. 본 shim 의 step-by-step fix path 는 자체 작성 (Wine 의
행동과 정합 시도).
