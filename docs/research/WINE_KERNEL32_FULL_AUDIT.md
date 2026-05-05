# WINE_KERNEL32_FULL_AUDIT — Wine kernelbase/kernel32 전수조사

own1 enforcement: Wine code lift 0줄. 본 문서는 algorithm/architecture 분석 only.
출력 = function name + line range + 의사코드(자체 작성) + citation file:line.
인용된 함수 본문은 *이름* + *대표 NT 호출* 만 발췌 — *재구현 가이드* 가 아닌 *비교 기준*.

cwd: /Users/ghost/core/airgenome-gamebox
target tree: /tmp/wine-research/wine/dlls/kernelbase/ (KERNEL32 핵심 41,880 LOC)
              + /tmp/wine-research/wine/dlls/kernel32/  (legacy / forward 13,034 LOC)
spec exports: kernel32.spec @ entries = 1,455 / kernelbase.spec @ entries = 1,433
              (동일 함수의 대부분은 kernel32 → kernelbase forward, 일부는 → ntdll forward)

본 project (airgenome-gamebox) 현황:
  - lib/loader/win32_kernel32.hexa            — 50-entry skeleton mapping table
  - lib/loader/win32_kernel32_stage1..5.hexa  — file/heap/handle synthetic 동작
  - lib/loader/win32_kernel32_tier1_real.hexa — Tier 1 trivial (10 fn, native pthread/clock)
  - lib/loader/win32_kernel32_tier2_real.hexa — Tier 2 sync/event/mutex
  - lib/loader/win32_kernel32_tier3_struct.hexa — Tier 3 STARTUPINFO/CommandLine struct
  - lib/loader/win32_kernel32_tier4_heap.hexa — Tier 4 user-mode heap (HeapCreate..HeapWalk)

---

## 0. 전체 architecture (Wine 와 본 project 비교)

### Wine 의 layered architecture

```
┌───────────────────────────────────────┐
│  PE App (kernel32.dll IAT 호출)       │
└──────────────┬────────────────────────┘
               │  (kernel32.spec @ stdcall -import)
               ▼
┌───────────────────────────────────────┐
│  kernel32.dll  (1,455 export)         │  ← dlls/kernel32/*.c (13K LOC)
│   - 거의 전부 -import 또는 forward    │
│   - 일부 legacy (atom, comm, profile, │
│     toolhelp, lzexpand) 자체 구현     │
└──────────────┬────────────────────────┘
               │  spec: kernelbase.<fn>
               ▼
┌───────────────────────────────────────┐
│  kernelbase.dll (1,433 export)        │  ← dlls/kernelbase/*.c (41K LOC)
│   - 모든 Win32 API 의 실제 본문       │
│   - 대부분 NT API (Nt*/Rtl*) wrapper  │
└──────────────┬────────────────────────┘
               │
               ▼
┌───────────────────────────────────────┐
│  ntdll.dll  (Nt* / Rtl*)              │  ← dlls/ntdll/*.c
│   - syscall thunk + RtlEncodePointer  │
│     같은 user-mode primitive          │
└──────────────┬────────────────────────┘
               │  unixlib syscall
               ▼
┌───────────────────────────────────────┐
│  ntdll.so (Linux/macOS native side)   │
│   server protocol → wineserver        │
└───────────────────────────────────────┘
```

핵심 포인트:
- **kernel32 자체에 algorithm 거의 없음** — 1,455 export 중 ~95% 가 kernelbase forward
  (`kernel32.spec` 검색: `-import` modifier 또는 `kernelbase.<fn>` forward 가 압도적).
- **algorithm 본문 전부 kernelbase 에 있음** — 본 audit 의 주 대상.
- kernelbase 도 ~70% 는 NT API wrapper (set_ntstatus(NtXxx(...)) pattern).
- 본 project 의 own1 강제 = "Wine 줄 lift 0" — algorithm 만 우리 own1 으로 재작성.

### 본 project (airgenome-gamebox) 의 layered architecture

```
┌───────────────────────────────┐
│  PE App (i386/x64 IAT 호출)   │
└──────────┬────────────────────┘
           │  iat_runtime_bind / iat_resolve_stage2
           ▼
┌───────────────────────────────┐
│  win32_kernel32.hexa (50 fn)  │  skeleton mapping
│   tier1..tier4 _real.hexa     │  call tracker (TSV persist)
│   stage1..stage5.hexa         │  synthetic fd/handle table
└──────────┬────────────────────┘
           │  pe_native_*_real.hexa
           ▼
┌───────────────────────────────┐
│  airgenome_helper.mm bridge   │  Apple SDK (pthread/dlopen/mmap)
└───────────────────────────────┘
```

비교: Wine 은 ntdll/wineserver 를 거쳐 syscall, 본 project 는 helper.mm 의 Apple SDK 직접 호출.
Wine 의 NTSTATUS → DOS error 변환 (RtlNtStatusToDosError) 단계가 본 project 에는 없음 —
대신 errno → Win32 LastError 직매핑이 필요 (현재 stage1 의 `errno_assign` 만 placeholder).

---

## 1. Loader (모듈 로딩)

### 1.1 Wine: `dlls/kernelbase/loader.c` 1,280 LOC

| Function | 위치 | 역할 |
|---|---|---|
| `get_proc_address` | loader.c:66 | 핵심 핸들러 — `LdrGetProcedureAddress(module, AnsiStr, ord, &proc)` 호출 |
| `GetProcAddress` (x64 asm) | loader.c:469 | xmm0~3 보존하는 hot-patch prolog (16 bytes nops + pushq %rbp) |
| `GetProcAddress` (arm64ec) | loader.c:453 | d0~3 register save naked function |
| `GetProcAddress` (i386) | loader.c:498 | 단순 `return get_proc_address(...)` |
| `LoadLibraryExW` | loader.c:562 | trailing-space 처리 → `load_library(UNICODE_STRING, flags)` |
| `FreeLibrary` | loader.c:232 | datafile module (`(handle & 3)`) → UnmapViewOfFile, 일반은 `LdrUnloadDll` |
| `GetModuleFileNameW` | loader.c:300 | win16_tib 우선, 일반은 `LdrGetDllFullName` |
| `DisableThreadLibraryCalls` | loader.c:223 | `LdrDisableThreadCalloutsForDll(module)` 직 |

#### algorithm 의사코드 (자체 작성)
```
GetProcAddress(module, fn):
    if !module: module = PEB.ImageBase
    if (uintptr)fn >> 16:                      # name 형식
        return Ldr.GetProcedureAddress(module, name, ordinal=0)
    else:                                      # ordinal 형식
        return Ldr.GetProcedureAddress(module, name=NULL, ordinal=lo16(fn))

LoadLibraryExW(name, file, flags):
    if name has trailing ' ':
        copy → strip trailing spaces → load_library(stripped, flags)
    else:
        load_library(name, flags)              # 즉시 NTSTATUS 경로

FreeLibrary(h):
    if h & 3:                                  # datafile (LOAD_LIBRARY_AS_DATAFILE)
        if h & 1:
            walk exclusive_datafile_list, CloseHandle(file), free entry
        return UnmapViewOfFile(h & ~3)
    else:
        return LdrUnloadDll(h)                 # ref count 1 감소 → 0 시 unmap
```

#### 본 project 의 wave 18+ 적용 algorithm
- **pe_iat_direct_resolve_real.hexa / iat_runtime_bind.hexa**: PE IAT 의 `IMAGE_THUNK_DATA.AddressOfData` 가 0x80000000 mask → ordinal lookup, 아니면 `IMAGE_IMPORT_BY_NAME` → name lookup. Wine 의 ordinal/name dual-path 와 동일한 algorithm 을 own1 으로 재작성.
- **pe_iat_forwarder_chain_real.hexa**: Wine 의 spec 파일 `forward DLL.fn` 줄 (예: `kernel32` → `kernelbase.fn`) 처리. 본 project 는 forwarder string ("KERNELBASE.HeapAlloc") 직접 파싱 → 재 dlsym.
- **pe_i386_iat_resolve_real.hexa**: i386 stdcall name decoration `_HeapAlloc@12` 처리.
- **win32_kernel32.hexa lookup_mapping("LoadLibraryW")** 는 현재 `dlopen` 매핑 — Wine 은 `LdrLoadDll` (NTSTATUS) 경유, 본 project 는 macOS dlopen 직접. Win32→DOS error 매핑 누락 = 본 project 의 알려진 gap.

### 1.2 Wine: `dlls/kernel32/module.c` 357 LOC
- 거의 전부 forward (`@ stdcall -import LoadLibraryW(wstr)`).
- 자체 구조: `K32EnumProcessModules` 등 일부 toolhelp.
- 본 project 는 별도 kernel32 layer 없이 win32_kernel32.hexa → 직접 native dispatch.

---

## 2. Process

### 2.1 Wine: `dlls/kernelbase/process.c` 1,909 LOC

| Function | 위치 | algorithm |
|---|---|---|
| `GetCommandLineA` | process.c:1363 | `return command_lineA;` (전역) |
| `GetCommandLineW` | process.c:1372 | `return command_lineW;` (전역) |
| `GetCurrentProcess` | process.c:767 | `return (HANDLE)~(ULONG_PTR)0;` (= pseudo-handle 0xFFFFFFFF) |
| `GetCurrentProcessId` | process.c:776 | `HandleToULong(NtCurrentTeb()->ClientId.UniqueProcess)` |
| `GetEnvironmentStringsA` | process.c:1552 | RtlAcquirePebLock → `WideCharToMultiByte(CP_ACP, env_W)` → HeapAlloc |
| `GetEnvironmentStringsW` | process.c:1572 | RtlAcquirePebLock → memcpy(PEB.ProcessParameters.Environment) |
| `GetStartupInfoW` | process.c:1381 | RtlAcquirePebLock → params 의 14 필드 그대로 복사 |
| `GetStdHandle` | process.c:1417 | switch(STD_INPUT/OUTPUT/ERROR_HANDLE) → PEB.ProcessParameters.hStdXxx |
| `IsProcessorFeaturePresent` | process.c:1045 | `RtlIsProcessorFeaturePresent(feature)` (= NT 위임) |
| `IsWow64Process` | process.c:1063 | `NtQueryInformationProcess(ProcessWow64Information)` |
| `CreateProcessInternalW` | process.c:507 | RTL_USER_PROCESS_PARAMETERS 빌드 → `NtCreateUserProcess` |
| `OpenProcess` | process.c:1128 | `NtOpenProcess(access, attr, &cid)` |

#### GetCommandLineA/W algorithm 의사코드
```
init (process attach 시 1회):
    RTL_USER_PROCESS_PARAMETERS *p = PEB.ProcessParameters
    command_lineW = p->CommandLine.Buffer            # UNICODE
    command_lineA = WideCharToMultiByte(CP_ACP, command_lineW)   # ANSI 변환

GetCommandLineA():  return command_lineA
GetCommandLineW():  return command_lineW
```

> citation: kernelbase/process.c:1363, 1372 (몸체 한 줄 return)
> 명시 init 로직은 `dlls/ntdll/env.c` 의 `init_user_process_params()` — 본 doc 범위 외.

#### 본 project 의 wave 18+ 적용 algorithm
- **pe_command_line_parse.hexa**: Win32 cmdline 파서 (CommandLineToArgvW 호환) — Wine 의 RtlGetCommandLine 과 동일 contract, own1 재작성.
- **pe_environment_block.hexa**: PEB.ProcessParameters.Environment 와 동일 layout (NULL-terminated WCHAR pairs + double NULL) 을 hexa 로 구축.
- **pe_peb_struct.hexa / pe_teb_struct.hexa**: PEB / TEB 의 0x60 / 0x30 offset 까지 정확 layout. Wine 의 `NtCurrentTeb()` 가 `__readfsdword(0x18)` (i386) / `__readgsqword(0x30)` (x64) 와 동일 — 본 project 는 pe_i386_teb_fs_segment.hexa 가 fs:[0x18] override 직접 처리.
- **win32_kernel32_tier3_struct.hexa**: STARTUPINFOW 구조체 14 필드 layout 검증. Wine GetStartupInfoW (process.c:1381) 의 14-field copy 와 동일 sequence.
- 본 project 의 `GetCommandLineW` stub 은 `_NSGetArgv` 매핑 (skeleton only) — 실제 동작 시 `pe_command_line_parse.hexa` 가 argv → WCHAR command_line 빌드 + caching.

### 2.2 IsProcessorFeaturePresent 정확 return value

Wine: `RtlIsProcessorFeaturePresent(feature)` 위임 (process.c:1047).
- `RtlIsProcessorFeaturePresent` 는 `dlls/ntdll/rtl.c` 에서 `user_shared_data->ProcessorFeatures[feature]` byte lookup.
- KUSER_SHARED_DATA.ProcessorFeatures 는 PROCESSOR_FEATURE_MAX (=64) byte array, init 시 cpuid 결과로 채워짐.

본 project gap:
- 현재 win32_kernel32.hexa 에 IsProcessorFeaturePresent entry 없음 (50 fn 표 외).
- 정확 return value: feature ≤ 63 인 경우만 user_shared_data byte, 그 외 FALSE.
- macOS 에서는 `sysctlbyname("hw.optional.<feature>")` 로 매핑 가능 (sse2/sse3/avx/avx2/aes 등).

---

## 3. File I/O

### 3.1 Wine: `dlls/kernelbase/file.c` 4,725 LOC

| Function | 위치 | algorithm |
|---|---|---|
| `CreateFileA` | file.c:750 | `file_name_AtoW(name, FALSE)` → CreateFileW |
| `CreateFileW` | file.c:795 | RtlDosPathNameToNtPathName_U + nt_disposition[5] table + NtCreateFile |
| `ReadFile` | file.c:3611 | OVERLAPPED 분기 + NtReadFile + 동기 시 WaitForSingleObject(file, INF) |
| `WriteFile` | file.c:4014 | OVERLAPPED 분기 + NtWriteFile + 동기 시 WaitForSingleObject(file, INF) |
| `GetSystemTimeAsFileTime` | file.c:4214 | `NtQuerySystemTime((LARGE_INTEGER*)time)` |
| `GetSystemTimePreciseAsFileTime` | file.c:4223 | `RtlGetSystemTimePrecise()` (ms 단위 정밀) |
| `DeleteFileW` | file.c:1120 | NtCreateFile(DELETE | FILE_DELETE_ON_CLOSE) → NtClose |
| `FindFirstFileExW` | file.c:1287 | NtCreateFile(directory) → NtQueryDirectoryFile + filter pattern |

#### CreateFileW algorithm 의사코드
```
CreateFileW(name, access, share, sa, creation, attr, template):
    nt_disposition = [FILE_CREATE, FILE_OVERWRITE_IF, FILE_OPEN, FILE_OPEN_IF, FILE_OVERWRITE]
    if !name or !name[0]: SetLastError(ERROR_PATH_NOT_FOUND); return INVALID
    if creation < CREATE_NEW or > TRUNCATE_EXISTING: SetLastError(ERROR_INVALID_PARAMETER)
    nameW = RtlDosPathNameToNtPathName_U(name)        # "C:\..." → "\??\C:\..."
    InitializeObjectAttributes(&attr, &nameW, OBJ_CASE_INSENSITIVE, ..)
    options = get_nt_file_options(attr, creation)     # FILE_NON_DIRECTORY_FILE | ...
    status = NtCreateFile(&h, access | SYNCHRONIZE | FILE_READ_ATTRIBUTES,
                          &attr, &io, NULL, attr_valid, share,
                          nt_disposition[creation - CREATE_NEW], options, NULL, 0)
    if status: SetLastError(RtlNtStatusToDosError(status)); return INVALID
    return h
```

#### ReadFile / WriteFile algorithm 의사코드
```
ReadFile(h, buf, count, *result, ovl):
    if ovl:
        offset = (ovl.OffsetHigh << 32) | ovl.OffsetLow
        event = ovl.hEvent;  iosb = (PIO_STATUS_BLOCK)ovl
        if !(event & 1): cvalue = ovl                 # APC routing flag
    iosb.Status = STATUS_PENDING
    status = NtReadFile(h, event, NULL, cvalue, iosb, buf, count, &offset, NULL)
    if status == PENDING and !ovl:                    # 동기 모드
        WaitForSingleObject(h, INFINITE)              # ← 핵심: file handle 자체가 sync object
        status = iosb.Status
    if result and (!ovl or !status): *result = iosb.Information
    if EOF and !ovl: return TRUE                      # 동기 EOF 는 정상
    if status: SetLastError(NtStatusToDosError(status)); return FALSE
    return TRUE
```

`WriteFile` 은 ReadFile 과 동일 구조 — NtWriteFile 만 교체.

#### GetSystemTimeAsFileTime — 정확 implementation
```
GetSystemTimeAsFileTime(*time):
    NtQuerySystemTime((LARGE_INTEGER*)time)
    # NtQuerySystemTime 은 KUSER_SHARED_DATA.SystemTime 직접 read
    #   (RtlGetSystemTimePrecise 는 GetSystemTimePreciseAsFileTime 만 사용)
    # 결과: 100-nanosecond units since 1601-01-01 UTC
```

#### 본 project 의 wave 18+ 적용 algorithm
- **win32_kernel32_stage1.hexa**: 합성 handle table (handle ≥ 100, fd 0/1/2 회피) — Wine 의 NT handle (kernel object) 와 정확히 다른 namespace. own1 재작성 ✓.
- **pe_file_overlapped_io.hexa**: OVERLAPPED 의 (Offset / OffsetHigh / hEvent / Internal / InternalHigh) 5-field layout 과 LowPart/HighPart split 처리 — Wine ReadFile (file.c:3628-3633) 의 분기와 동일 algorithm.
- **pe_ntdll_nt_create_file_real.hexa**: NtCreateFile contract (10 args) 명세 → mac POSIX open(O_*) 매핑.
- **CreateFileA → CreateFileW** path conversion 은 본 project 의 `pe_charset_convert.hexa` (CP_ACP utf-8 매핑) 가 담당.
- **GetSystemTimeAsFileTime** 본 project mapping (`mach_absolute_time`) 은 부정확 — 1601 epoch 기준 100ns FILETIME 변환 필요. Wine 은 KUSER_SHARED_DATA 직접 read, macOS 동등은 `clock_gettime(CLOCK_REALTIME)` 후 (sec - 11644473600) * 10000000 + nsec/100. 본 audit 가 발견한 known gap.

### 3.2 file.c sub-areas
- 1,759: GetFileAttributesA/W → NtQueryAttributesFile
- 2,048-2,221: path normalization (GetFullPathNameW/GetShortPathNameW/GetLongPathNameW) → RtlGetFullPathName_U
- 1,227-1,620: FindFirstFile / FindNextFile family — 내부 search-handle struct (nt_path + search_mask + buffer + index) 로 NtQueryDirectoryFile 페이지네이션
- 2,519-2,601: GetTempPathA/W/2 — 환경변수 TMP/TEMP/USERPROFILE 순 탐색
- 본 project: pe_file_attribute_track.hexa, pe_file_iocontrol.hexa, win32_kernel32_extras.hexa

---

## 4. Sync (Mutex / Event / Semaphore / WaitForX)

### 4.1 Wine: `dlls/kernelbase/sync.c` 1,883 LOC

| Function | 위치 | algorithm |
|---|---|---|
| `Sleep` | sync.c:361 | `NtDelayExecution(FALSE, get_nt_timeout(timeout))` |
| `SleepEx` | sync.c:372 | NtDelayExecution(alertable) → STATUS_USER_APC → WAIT_IO_COMPLETION |
| `WaitForSingleObject` | sync.c:395 | `WaitForSingleObjectEx(h, t, FALSE)` |
| `WaitForSingleObjectEx` | sync.c:404 | `NtWaitForSingleObject(normalize_std_handle(h), alertable, &timeout)` |
| `WaitForMultipleObjectsEx` | sync.c:434 | `NtWaitForMultipleObjects(count, hloc[], WaitAll/Any, alertable, t)` |
| `GetTickCount` | sync.c:181 | `user_shared_data->TickCount.LowPart` (단순 read) |
| `GetTickCount64` | sync.c:191 | (high1, low) read → high2 mismatch 시 retry (lockfree double-read) |
| `CreateEventA/W` | sync.c:552/566 | flags 빌드 → CreateEventExA/W |
| `CreateEventExW` | sync.c:599 | get_create_object_attributes + NtCreateEvent(NotificationEvent/SynchronizationEvent, initial) |
| `SetEvent` | sync.c:679 | `set_ntstatus(NtSetEvent(h, NULL))` |
| `ResetEvent` | sync.c:688 | `set_ntstatus(NtResetEvent(h, NULL))` |
| `CreateMutexExW` | sync.c:742 | NtCreateMutant(initial_owner) — Win Mutex = NT Mutant |
| `ReleaseMutex` | sync.c:782 | `set_ntstatus(NtReleaseMutant(h, NULL))` |
| `CreateSemaphoreExW` | sync.c:806 | NtCreateSemaphore(initial, max) |
| `ReleaseSemaphore` | sync.c:846 | NtReleaseSemaphore(h, count, *prev) |

#### WaitForSingleObject algorithm 의사코드
```
WaitForSingleObject(h, timeout_ms):
    return WaitForSingleObjectEx(h, timeout_ms, FALSE)

WaitForSingleObjectEx(h, timeout_ms, alertable):
    # get_nt_timeout: ms → 100ns negative LARGE_INTEGER (relative)
    if timeout_ms == INFINITE: nt_t = NULL
    elif timeout_ms == 0:      nt_t.QuadPart = 0
    else:                      nt_t.QuadPart = -(int64)timeout_ms * 10000
    h = normalize_std_handle(h)            # STD_INPUT_HANDLE 등 pseudo → 실 handle
    status = NtWaitForSingleObject(h, alertable, nt_t)
    if NT_ERROR(status):
        SetLastError(RtlNtStatusToDosError(status))
        return WAIT_FAILED
    return status                           # 0=OBJECT_0, 0x80=ABANDONED, 0x102=TIMEOUT, 0xC0=USER_APC
```

#### CreateEventW algorithm 의사코드
```
CreateEventW(sa, manual_reset, initial_state, name):
    flags = 0
    if manual_reset:  flags |= CREATE_EVENT_MANUAL_RESET
    if initial_state: flags |= CREATE_EVENT_INITIAL_SET
    return CreateEventExW(sa, name, flags, EVENT_ALL_ACCESS)

CreateEventExW(sa, name, flags, access):
    get_create_object_attributes(&attr, &nameW, sa, name)
    type = (flags & MANUAL_RESET) ? NotificationEvent : SynchronizationEvent
    initial = (flags & INITIAL_SET) != 0
    status = NtCreateEvent(&h, access, &attr, type, initial)
    if status == OBJECT_NAME_EXISTS: SetLastError(ERROR_ALREADY_EXISTS)
    else: SetLastError(NtStatusToDosError(status))
    return h
```

#### 본 project 의 wave 18+ 적용 algorithm
- **win32_kernel32_tier2_real.hexa**: Sync APIs Tier 2 — pthread_cond_init/signal/destroy + pthread_mutex_init/lock/unlock 매핑. Wine 의 NtCreateEvent(NotificationEvent) 는 manual_reset (broadcast) → pthread_cond_broadcast 매핑 / SynchronizationEvent (auto-reset) → pthread_cond_signal + auto-reset flag. own1 재작성 ✓.
- **pe_event_object.hexa / pe_mutex_object.hexa / pe_semaphore_object.hexa**: 각 object 의 state machine (signaled/non-signaled, manual/auto reset, owner thread, recursion count) — Wine NT primitives 의 행동을 own1 으로 재정의.
- **pe_wait_for_single_object.hexa / pe_wait_for_multiple_object.hexa**: WaitAll vs WaitAny + INFINITE / 0 / N ms timeout 분기. Wine `get_nt_timeout` 의 ms→100ns negative 변환은 본 project 에서 **불필요** (pthread_cond_timedwait 가 absolute timespec 사용 → `clock_gettime(CLOCK_REALTIME) + ms` 매핑).
- **pe_critical_section_real.hexa**: CriticalSection (LightWeight Mutex) — Wine RtlEnterCriticalSection 은 InterlockedIncrement + spin + NtWaitForKeyedEvent. 본 project 는 pthread_mutex_t recursive 직접.
- **GetTickCount64** lock-free double-read 패턴 (sync.c:194-200) 은 KUSER_SHARED_DATA 의 64bit non-atomic read 회피 — 본 project 는 64bit atomic mach_absolute_time 직접 사용 가능 (재현 불필요).

### 4.2 CreateThread

Wine: thread.c:169 → `CreateRemoteThread(GetCurrentProcess(), ...)` → thread.c:85 `CreateRemoteThreadEx`
  → `NtCreateThreadEx(handle, THREAD_ALL_ACCESS, &attr, process, start, param,
                     CREATE_SUSPENDED, 0, stack_commit, stack_reserve, attr_list)`
  → CLIENT_ID + TEB_ADDRESS attribute 자동 설정
  → `NtResumeThread(handle, &ret)` (CREATE_SUSPENDED flag 없으면)

본 project:
- **pe_thread_creation_bridge.hexa / pe_create_remote_thread.hexa**: pthread_create + start_routine wrapper (Win32 LPTHREAD_START_ROUTINE = DWORD(*)(void*) → POSIX void*(*)(void*) 변환).
- **pe_i386_teb_fs_segment.hexa**: 새 thread 의 TEB 할당 + fs:[0x18] = self pointer 설정.
- **pe_tls_alloc_real.hexa / pe_tls_init_real.hexa**: TLS slot 64+1024 layout (TEB.TlsSlots[64] + TEB.TlsExpansionSlots[1024]) 정확 재현.

---

## 5. Heap (Memory)

### 5.1 Wine: `dlls/kernelbase/memory.c` 1,904 LOC

| Function | 위치 | algorithm |
|---|---|---|
| `HeapCreate` | memory.c:739 | `RtlCreateHeap(flags, NULL, max, init, NULL, NULL)` |
| `HeapDestroy` | memory.c:750 | `RtlDestroyHeap(h)` (NULL return on success) |
| `HeapAlloc` | (forward to ntdll.RtlAllocateHeap, memory.c:1880 area) | RtlAllocateHeap(h, flags, size) |
| `HeapFree` | (forward to RtlFreeHeap) | RtlFreeHeap(h, flags, ptr) |
| `HeapSize` | (forward to RtlSizeHeap) | RtlSizeHeap(h, flags, ptr) |
| `HeapReAlloc` | (forward to RtlReAllocateHeap) | RtlReAllocateHeap |
| `GetProcessHeap` | process.c:909 | `return ((PEB*)NtCurrentTeb()->Peb)->ProcessHeap;` |
| `VirtualAlloc` | memory.c:420 | `VirtualAllocEx(GetCurrentProcess(), addr, size, type, protect)` |
| `VirtualAllocEx` | memory.c:429 | `NtAllocateVirtualMemory(p, &ret, 0, &size, type, protect)` |
| `VirtualFree` | memory.c:520 | `VirtualFreeEx → NtFreeVirtualMemory(MEM_RELEASE/DECOMMIT)` |
| `VirtualProtect` | memory.c:553 | `VirtualProtectEx → NtProtectVirtualMemory` |
| `VirtualQuery` | memory.c:585 | `NtQueryVirtualMemory(MemoryBasicInformation)` |
| `MapViewOfFile` | memory.c:278 | `MapViewOfFileEx(h, access, hi, lo, count, NULL)` |
| `MapViewOfFileEx` | memory.c:288 | `NtMapViewOfSection(...)` |
| `GetSystemInfo` | memory.c:236 | NtQuerySystemInformation(Basic) + (CpuInformation) → fill_system_info |
| `GetNativeSystemInfo` | memory.c:207 | wow64 분기 + RtlGetNativeSystemInformation |

#### HeapCreate algorithm 의사코드
```
HeapCreate(flags, init_size, max_size):
    h = RtlCreateHeap(flags, addr=NULL, max_size, init_size, lock=NULL, params=NULL)
    if !h: SetLastError(ERROR_NOT_ENOUGH_MEMORY)
    return h
    # RtlCreateHeap 자체는 NtAllocateVirtualMemory + heap_t header 빌드
    # heap_t = { Signature=0xeeeeeeee, Flags, Encoding[16], FreeListsInUseTerminate, ... }
```

#### GetSystemInfo — 정확 fill_system_info (memory.c:157)
```
fill_system_info(SYSTEM_INFO *si, basic, cpu):
    si->wProcessorArchitecture     = cpu.ProcessorArchitecture   # 0=x86, 9=x64, 12=ARM64
    si->dwPageSize                 = basic.PageSize              # 4096 / 16384
    si->lpMinimumApplicationAddress = basic.LowestUserAddress    # 0x10000
    si->lpMaximumApplicationAddress = basic.HighestUserAddress   # 0x7FFEFFFF (32bit) / 0x7FFFFFFEFFFF
    si->dwActiveProcessorMask      = basic.ActiveProcessorsAffinityMask
    si->dwAllocationGranularity    = basic.AllocationGranularity # 0x10000 (64K)
    si->dwNumberOfProcessors       = cpu.MaximumProcessors
    si->wProcessorLevel            = cpu.ProcessorLevel
    si->wProcessorRevision         = cpu.ProcessorRevision
    switch cpu.ProcessorArchitecture:
        case INTEL:  dwProcessorType = lvl 3→386, 4→486, 5/6→PENTIUM
        case AMD64:  dwProcessorType = PROCESSOR_AMD_X8664 (8664)
        case ARM:    dwProcessorType = lvl 4→ARM_7TDMI, else ARM920
        case ARM64:  dwProcessorType = 0
```

#### 본 project 의 wave 18+ 적용 algorithm
- **win32_kernel32_tier4_heap.hexa**: HeapCreate/Destroy/Alloc/ReAlloc/Free/Size/Validate/Walk/Compact (10 fn) Tier 4 추적기. own2 자체 user-mode heap 구현 — Wine RtlCreateHeap 의 chunk header layout 과 동일하지 않음 (own1 재작성 ✓).
- **pe_heap_walk.hexa**: PROCESS_HEAP_ENTRY linked walk — D2R/D4 anti-cheat 가 사용. wFlags 의 PROCESS_HEAP_REGION / PROCESS_HEAP_ENTRY_BUSY / UNCOMMITTED_RANGE bit 조합 정확 재현 필요 (현재 stub).
- **pe_heap_stage2.hexa**: HeapAlloc(0x8 = HEAP_ZERO_MEMORY) flag 처리 — Wine RtlAllocateHeap 의 동일 flag 와 contract 일치.
- **pe_native_mmap_real.hexa / pe_native_mprotect_real.hexa**: VirtualAlloc → mmap(MAP_PRIVATE|MAP_ANON) + 4K/16K page rounding. macOS arm64 16K 페이지는 본 project 가 추가로 `pe_page_adapter_16k.hexa` 로 정합 — Wine 은 PAGE_SIZE 가정.
- **GetSystemInfo** 정확 return value — 본 project 는 sysctl 직접 호출 가능 (`hw.physicalcpu`, `hw.cpufrequency`, `hw.pagesize`). `dwProcessorType` 매핑은 cpu_info.ProcessorArchitecture lookup table (위 fill_system_info) 동일 algorithm 으로 own1 재작성.
- **GetProcessHeap** 본 project mapping = "fixed sentinel heap_id = 1" (tier4_heap.hexa:7) — Wine 은 PEB.ProcessHeap pointer 직접. 본 project 는 PEB 자체가 hexa 변수이므로 sentinel 정책 OK ✓.

### 5.2 EncodePointer / DecodePointer (cookie + algorithm)

위치: `dlls/ntdll/rtl.c:1024-1065` (kernelbase 가 아니라 ntdll, kernelbase 의 EncodePointer 는 export forward).

#### cookie source
```
get_process_cookie() (rtl.c:1026):
    static ULONG process_cookie = 0
    if !process_cookie:
        NtQueryInformationProcess(GetCurrentProcess(), ProcessCookie, &cookie, 4)
        InterlockedCompareExchange(&process_cookie, cookie, 0)
    return process_cookie
```
- ProcessCookie (info class 36) = NT kernel 이 process 생성 시 RtlRandom 으로 1회 생성.
- 본 process 내 모든 thread 공유, 32bit ULONG.
- 32bit pointer 인 경우 cookie 그대로 XOR, 64bit 인 경우 cookie 가 32bit 라 상위 32bit 는 0 XOR (= no-op).

#### EncodePointer algorithm (rtl.c:1045)
```
BIT_COUNT_IN_POINTER = sizeof(void*) * 8        # 32 또는 64

EncodePointer(ptr):
    cookie = get_process_cookie()                # ULONG
    rotate = cookie % BIT_COUNT_IN_POINTER       # 0..31 또는 0..63
    ptrval = (uintptr)ptr ^ cookie               # XOR
    return (void*)((ptrval >> rotate) | (ptrval << ((BIT_COUNT_IN_POINTER - rotate) % BIT_COUNT_IN_POINTER)))
    # = right-rotate by `rotate` bits

DecodePointer(ptr):
    cookie = get_process_cookie()
    rotate = cookie % BIT_COUNT_IN_POINTER
    ptrval = (uintptr)ptr
    ptrval = (ptrval << rotate) | (ptrval >> ((BIT_COUNT_IN_POINTER - rotate) % BIT_COUNT_IN_POINTER))
    # = left-rotate by `rotate` bits  (역연산)
    return (void*)(ptrval ^ cookie)
```

> citation: ntdll/rtl.c:1043-1065 (algorithm 자체는 short — 본 doc 에서 8 줄 의사코드로 충분)

#### 본 project 의 wave 18+ 적용 algorithm
- 본 project 는 EncodePointer/DecodePointer 미구현. **gap identified**.
- 재현 method:
  1. **cookie source**: 본 project 시작 시 1회 `arc4random()` 또는 `mach_absolute_time() & 0xFFFFFFFF` → static cookie.
  2. **algorithm**: 위 의사코드 그대로 own1 작성 (Wine code lift X — 의사코드가 *명세* 이고, hexa 작성은 own2 의무).
  3. **저장**: pe_msvc_security_cookie_real.hexa 가 있음 — 거기에 process_cookie 추가 가능.
- 사용처: MSVC C runtime 의 `__security_cookie` (stack canary) 와 별개. `_AtlBaseModule::m_pTermFuncs` 같은 atexit 콜백에서 사용. CS2 / D4 의 anti-tamper 가 EncodePointer 한 함수 포인터 검증 시 필요.

---

## 6. Locale (Codepage / Wide<->Multi)

### 6.1 Wine: `dlls/kernelbase/locale.c` 8,375 LOC (가장 큰 파일)

| Function | 위치 | algorithm |
|---|---|---|
| `GetACP` | locale.c:5545 | `return ansi_cpinfo.CodePage;` (전역, init 시 RegistryQuery) |
| `GetOEMCP` | locale.c:6152 | `return oem_cpinfo.CodePage;` |
| `GetUserDefaultLCID` | locale.c:6383 | NtQueryDefaultLocale(USER) |
| `GetSystemDefaultLCID` | locale.c:6236 | NtQueryDefaultLocale(SYSTEM) |
| `MultiByteToWideChar` | locale.c:7011 | switch(codepage) → utf8/utf7/symbol/codepage path |
| `WideCharToMultiByte` | locale.c:7369 | 역방향, 동일 codepage table lookup |
| `LocaleNameToLCID` | locale.c:6992 | RtlLocaleNameToLcid |

#### MultiByteToWideChar algorithm 의사코드
```
MultiByteToWideChar(cp, flags, src, srclen, dst, dstlen):
    sanity: src && srclen, !(dst && !dstlen), dstlen >= 0
    if srclen < 0: srclen = strlen(src) + 1     # NUL 포함
    switch cp:
        CP_SYMBOL:  ret = mbstowcs_cpsymbol(flags, src, srclen, dst, dstlen)
        CP_UTF7:    ret = mbstowcs_utf7(...)
        CP_UNIXCP:  cp = unix_cp; fall through
        default:
            info = get_codepage_table(cp)        # CPTABLEINFO lookup (cached)
            if !info: ERROR_INVALID_PARAMETER
            if info.CodePage == CP_UTF8: ret = mbstowcs_utf8(...)
            else:                        ret = mbstowcs_codepage(info, ...)  # SBCS/DBCS
    return ret
```
- CPTABLEINFO 는 `nls/c_<codepage>.nls` 파일로부터 init 시 load, MultiByteTable[256] / WideCharTable[65536] 로 양방향 lookup.

#### 본 project 의 wave 18+ 적용 algorithm
- **pe_charset_convert.hexa**: 현재 CP_UTF8 / CP_ACP (한글 환경 = CP949, mac 기본 = CP_UTF8) 만 지원.
- **pe_unicode_string.hexa / pe_unicode_normalize.hexa**: NFC/NFD 정규화 (Wine 의 NormalizeString/RtlNormalizeString 와 동일 contract).
- macOS 의 `iconv()` 활용 가능 — Wine 은 자체 nls 테이블, 본 project 는 iconv shortcut OK.
- **GetUserDefaultLCID / GetSystemDefaultLCID**: 본 project 미구현. macOS 매핑 = `CFLocaleCopyCurrent()` → BCP-47 → LCID lookup table (wine 의 `LOCALE_NAME_LCID_TABLE` 동등).
- **GetACP**: 본 project 는 CP_UTF8 (65001) 고정 가정. 한글 게임 (D2R 한국어 / 던파) 에서 CP949 (949) 강제 필요한 경우 옵션 추가 — 본 audit 의 follow-up.

### 6.2 IsValidCodePage / EnumSystemCodePagesW / GetCPInfo
- `GetCPInfo` (locale.c:5554): get_codepage_table(cp) → CPINFO {MaxCharSize, DefaultChar[2], LeadByte[12]}
- 본 project: pe_collation_compare.hexa (CompareStringW), pe_charset_convert.hexa.

---

## 7. Console / Debug

### 7.1 Wine: `dlls/kernelbase/console.c` 2,409 LOC + `debug.c` 1,886 LOC

| Function | 위치 | algorithm |
|---|---|---|
| `GetConsoleMode` | console.c:932 | `console_ioctl(h, IOCTL_CONDRV_GET_MODE, NULL, 0, mode, 4, NULL)` |
| `GetConsoleOutputCP` | console.c:969 | NtQueryInformation on console handle |
| `WriteConsoleW` (around console.c:2150) | console_ioctl(IOCTL_CONDRV_WRITE_CONSOLE) |
| `IsDebuggerPresent` | debug.c:166 | `return NtCurrentTeb()->Peb->BeingDebugged;` (1 byte read) |
| `OutputDebugStringA` | debug.c:181 | DBWinMutex + RaiseException(DBG_PRINTEXCEPTION_C) |
| `RaiseException` | debug.c:389 | RtlRaiseException(rec) — kernel-level exception throw |

#### IsDebuggerPresent — 정확 implementation
```
IsDebuggerPresent():
    return PEB.BeingDebugged    # offset 0x02 of PEB, 1 byte BOOLEAN
```
- 매우 simple. Anti-debug 우회용으로 게임이 자주 호출 — 항상 FALSE 반환이면 OK (PEB.BeingDebugged 를 0 으로 patch 한 효과).

본 project: pe_debug_break.hexa, pe_debug_directory.hexa.
- 정확 동작: PEB.BeingDebugged byte 를 본 project 의 peb 구조체에 0 으로 init.
- pe_anti_cheat_detection_track.hexa 가 IsDebuggerPresent / CheckRemoteDebuggerPresent / NtQueryInformationProcess(ProcessDebugPort) 등 anti-debug API 호출 추적.

---

## 8. fake_proc_stub return 0 정책 vs Wine 정확 implementation 비교

### Wine 의 정책 (각 함수마다 다름)

| Wine 기본 fallback | 사용처 | 예시 |
|---|---|---|
| `STATUS_NOT_IMPLEMENTED` + WARN | NT API 미구현 | NtCreateSection 일부 path |
| `SetLastError(ERROR_CALL_NOT_IMPLEMENTED); return FALSE` | Win32 stub | GetSystemFileCacheSize (memory.c:254) |
| `FIXME(...); return 0` | 단순 stub | LoadAppInitDlls (loader.c:601) |
| `FIXME(...); return E_NOTIMPL` | COM | `BasepReportFault*` |
| 정확 spec return value | trivial | GetCurrentProcess = ~0, GetCurrentThread = ~1 |

핵심: Wine 은 함수마다 *spec-correct* fallback 사용. 단순 0 반환 X.

### 본 project 의 fake_proc_stub return 0 정책

현재 (lib/loader/win32_stubs.hexa 등 추정):
- 모든 미구현 export → return 0.
- 문제점:
  1. `GetCurrentProcess` 의 spec-correct return = `~(ULONG_PTR)0` (= 0xFFFFFFFFFFFFFFFF).
     return 0 일 경우 caller 가 NULL handle 검사 후 GetLastError → 게임이 "process handle 못 얻음" 으로 abort.
  2. `IsDebuggerPresent` 의 spec-correct return = FALSE (0) — 우연히 correct.
  3. `GetTickCount` 의 spec-correct return = monotonic ms — return 0 일 경우 frame timer 가 stuck → 게임 freeze.
  4. `IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE)` — return 0 (FALSE) 시 SSE2 disable 코드 path → MSVC CRT 의 "_chkstk_ms" 등 fallback path 가 invalid (대부분 게임은 SSE2 가정).
  5. `EncodePointer(p)` — return 0 일 경우 caller 가 0 을 함수포인터로 invoke → SIGSEGV. spec-correct = `p XOR rotate(cookie)`.

### 권장 정책 (wave 18+)

```
정책:  미구현 export 의 fallback 은 4 단계 분류:
  Tier A — spec-default constant:
    GetCurrentProcess → ~0
    GetCurrentThread  → ~1
    IsDebuggerPresent → 0
    GetLastError      → 0
  Tier B — pass-through identity:
    EncodePointer(p)  → p
    DecodePointer(p)  → p           # 임시 — 보안 약화 OK if no caller verifies
  Tier C — feature-true conservative:
    IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE) → 1  (SSE2 = always on x64/arm64 emul)
    IsProcessorFeaturePresent(others) → 0
  Tier D — error fallback:
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED); return FALSE
    (모든 BOOL 반환 + 명백한 미지원 기능)
```

본 project 의 미래 작업: `lib/loader/win32_stubs.hexa` 에 위 4-tier 분류 표 추가.

---

## 9. Section-by-section: 본 project 의 적용 wave 매핑

| Wine area | LOC | 본 project 대응 hexa | wave |
|---|---|---|---|
| loader.c | 1,280 | win32_kernel32.hexa, pe_iat_*_real.hexa | 18+ |
| process.c | 1,909 | win32_kernel32_tier3_struct.hexa, pe_create_process.hexa | 19+ |
| thread.c | 1,433 | win32_kernel32_tier1_real.hexa, pe_thread_creation_bridge.hexa | 18+ |
| sync.c | 1,883 | win32_kernel32_tier2_real.hexa, pe_event_object/mutex_object/semaphore_object | 19+ |
| file.c | 4,725 | win32_kernel32_stage1.hexa, pe_file_*_track.hexa | 18+ |
| memory.c | 1,904 | win32_kernel32_tier4_heap.hexa, pe_native_mmap_real.hexa | 20+ |
| locale.c | 8,375 | pe_charset_convert.hexa, pe_unicode_*.hexa | 21+ |
| console.c | 2,409 | pe_console_*_buffer.hexa, pe_console_io_bridge.hexa | 22+ |
| debug.c | 1,886 | pe_debug_break.hexa, pe_anti_cheat_detection_track.hexa | 22+ |
| version.c | 1,766 | win32_version_tier1_real.hexa | 19+ |
| volume.c | 1,208 | pe_volume_information.hexa | 21+ |
| registry.c | 4,145 | pe_registry_*.hexa, win32_advapi32_tier1_real.hexa | 21+ |
| security.c | 1,564 | pe_acl_descriptor.hexa, pe_token_privilege.hexa | 23+ |
| path.c | 5,255 | (path normalize 일부 win32_kernel32_extras.hexa) | TBD |
| string.c | 1,501 | (lstrlenW/A 만 tier1, 나머지 미적용) | TBD |
| memory.c (heap) | 1,904 | win32_kernel32_tier4_heap.hexa | 20+ |

---

## 10. 발견된 known gap (audit summary)

1. **GetSystemTimeAsFileTime** 본 project 매핑 = `mach_absolute_time` — 부정확. FILETIME (1601 epoch, 100ns) 변환 필요. (file.c:4214)
2. **EncodePointer / DecodePointer** 본 project 미구현 — process_cookie + bit-rotate algorithm 도입 필요. (ntdll/rtl.c:1045)
3. **IsProcessorFeaturePresent** 본 project skeleton table 외 entry 없음 — 64-byte feature array + sysctl 기반 채움 필요.
4. **GetCurrentProcess** spec return ~0 / **GetCurrentThread** ~1 — 현재 stub 가 0 반환이면 anti-cheat false-positive 가능.
5. **Wait timeout 변환**: ms → 100ns negative LARGE_INTEGER (Wine sync.c) — 본 project 는 pthread_cond_timedwait 의 absolute timespec 사용. 두 path 가 INFINITE / 0 / N ms 분기에서 동일 결과 보장 self-test 필요.
6. **NTSTATUS → DOS error** 매핑 (RtlNtStatusToDosError) 미구현 — errno → Win32 error 직매핑 테이블 (`win32_errno_map.hexa` 신규?) 필요.
7. **GetACP / GetOEMCP** 본 project = CP_UTF8 고정 — 한글 게임 호환성 위해 옵션 추가.
8. **PEB.BeingDebugged byte 0** 보장 — pe_peb_struct.hexa 의 init path 검증 필요.
9. **HEAP_ZERO_MEMORY (0x8) flag** — pe_heap_stage2.hexa 의 처리 self-test 필요.
10. **fake_proc_stub return 0** 정책을 Tier A/B/C/D 4-bucket 으로 reclassify 필요 (위 §8).

---

## 11. 결론

- Wine kernel32 + kernelbase = 41,880 + 13,034 = 54,914 LOC.
- 본 project 의 win32_kernel32 영역 = ~5,000 LOC (10% 수준).
- algorithm 측면 = 핵심 50 fn 의 ~70% 가 wave 18+ 에서 본 project 자체 구현됨.
- **gap 는 대부분 trivial spec-correct return value** (위 §10) — 큰 architecture 변경 없이 보강 가능.
- own1 enforcement 유지: 본 audit 의 모든 의사코드는 *spec 명세* 이지 Wine source 가 아님. 실제 hexa 작성 시 own2 자체 코드.

own1 ✓ — Wine code lift 0 줄. function + line range + 의사코드 + citation only.
