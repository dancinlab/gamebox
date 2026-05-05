# Win32 → POSIX/Apple symbol mapping (2026-05-01)

> **status**: airgenome-gamebox 의 phase 10 IAT routing 참조 table.
> own1 정합: Apple SDK (libSystem, Foundation, AppKit, CoreGraphics, Metal, Security, CoreFoundation, IOKit) + Microsoft MSDN spec only.
> Wine / CrossOver / GPTk / Whisky 의 symbol mapping 인용 0줄.
> 본 문서는 IAT real resolve (cycle 80~88) 시 lookup 되는 mapping table 의 spec 화 — 실제 code 는 own2 stage1/stage2 에서 함수 단위 별 구현.

---

## 0. 사용 규약

- 모든 Win32 함수명은 MSDN documented signature 그대로 표기 (W/A suffix 명시).
- POSIX/Apple side 는 `<header>` reference 동봉 — Apple Developer Documentation 기준.
- "helper IPC kind=N" — own2 inline 으로 처리 불가하고 native helper 위임 필요한 영역 (mmap/mprotect/MTLDevice 등 sandbox/entitlement 요구).
- "own2 cycle N <name>" — airgenome stage1/stage2 의 자체 구현 ID 참조.
- "no-op" — sandbox 정책상 의미 없음, success 반환만.
- "fake handle" — 실제 OS object 없이 sentinel HANDLE 반환 (검증은 게임별 사용 패턴 기준).

---

## 1. KERNEL32 → libSystem (~100 함수, 시간/프로세스/시스템 정보)

| Win32 | POSIX/Apple | header | 비고 |
|---|---|---|---|
| GetCurrentThreadId | pthread_mach_thread_np(pthread_self()) | `<pthread.h>` | uint32 cast |
| GetCurrentProcessId | getpid() | `<unistd.h>` | direct |
| GetCurrentProcess | sentinel (HANDLE)-1 | — | pseudo handle |
| GetCurrentThread | sentinel (HANDLE)-2 | — | pseudo handle |
| GetTickCount | mach_absolute_time → mach_timebase_info → ms (uint32) | `<mach/mach_time.h>` | 32bit wrap @ ~49.7d |
| GetTickCount64 | mach_absolute_time → ms (uint64) | `<mach/mach_time.h>` | direct |
| GetSystemTime | clock_gettime(CLOCK_REALTIME) → SYSTEMTIME | `<time.h>` | gmtime_r 변환 |
| GetSystemTimeAsFileTime | clock_gettime(CLOCK_REALTIME) → FILETIME | `<time.h>` | epoch delta 11644473600s |
| GetLocalTime | clock_gettime + localtime_r | `<time.h>` | timezone |
| SystemTimeToFileTime | timegm + epoch delta | `<time.h>` | 100ns ticks |
| FileTimeToSystemTime | epoch delta + gmtime_r | `<time.h>` | 100ns ticks |
| QueryPerformanceCounter | mach_absolute_time | `<mach/mach_time.h>` | direct |
| QueryPerformanceFrequency | mach_timebase_info denom/numer | `<mach/mach_time.h>` | ns→tick |
| Sleep | usleep(ms*1000) | `<unistd.h>` | ms → us |
| SleepEx | nanosleep + alertable=false | `<time.h>` | alert no-op |
| GetSystemDirectoryW | "C:\\Windows\\System32" sentinel string | — | Win 호환 string |
| GetWindowsDirectoryW | "C:\\Windows" sentinel string | — | Win 호환 string |
| GetCurrentDirectoryW | getcwd + path translate | `<unistd.h>` | / → \\ |
| SetCurrentDirectoryW | chdir + path translate | `<unistd.h>` | \\ → / |
| GetEnvironmentVariableW | getenv + UTF-16 변환 | `<stdlib.h>` | UTF-8 → UTF-16 |
| SetEnvironmentVariableW | setenv | `<stdlib.h>` | UTF-16 → UTF-8 |
| GetEnvironmentStringsW | environ + UTF-16 block | `<unistd.h>` | double-NUL term |
| FreeEnvironmentStringsW | free | `<stdlib.h>` | direct |
| ExpandEnvironmentStringsW | own2 cycle 79 expand_env_real | — | %VAR% parse |
| GetCommandLineW | own2 saved at startup | — | UTF-16 cached |
| GetModuleHandleW | own2 PE loader module table | own2 cycle 78 | NULL → main |
| GetModuleHandleExW | own2 PE loader + refcount | own2 cycle 78 | flags 처리 |
| GetModuleFileNameW | own2 PE loader → mapped path | own2 cycle 78 | path translate |
| GetProcAddress | own2 export table walk | own2 cycle 80 | ordinal/name |
| LoadLibraryW | own2 PE loader recurse | own2 cycle 79 | api-set 처리 |
| LoadLibraryExW | own2 PE loader + flags | own2 cycle 79 | LOAD_AS_DATAFILE |
| FreeLibrary | own2 PE loader unmap + refcount | own2 cycle 79 | no-op until 0 |
| DisableThreadLibraryCalls | own2 PE loader DllMain skip flag | own2 cycle 79 | per-module |
| GetVersion | 0x0A00 (Win10) packed | — | DEPRECATED MSDN |
| GetVersionExW | OSVERSIONINFOEXW Win10 | — | major=10, minor=0 |
| IsProcessorFeaturePresent | sysctlbyname("hw.optional.*") | `<sys/sysctl.h>` | x86 → arm64 매핑 X |
| GetSystemInfo | sysctlbyname + page size | `<sys/sysctl.h>` | hw.ncpu, hw.pagesize |
| GetNativeSystemInfo | 동일 | `<sys/sysctl.h>` | direct |
| GetSystemMetrics | NSScreen + AppKit | (USER32 카테고리) | — |
| GetComputerNameW | gethostname + UTF-16 | `<unistd.h>` | first dot 잘림 |
| GetUserNameW | (ADVAPI32 카테고리 참조) | — | — |
| GetLastError / SetLastError | per-thread TLS slot | own2 cycle 82 tls | pthread_key_create |
| FormatMessageW | own2 cycle 79 win32_error_str | — | static table |
| OutputDebugStringW | os_log_with_type(OS_LOG_TYPE_DEBUG) | `<os/log.h>` | UTF-16 → UTF-8 |
| IsDebuggerPresent | sysctl P_TRACED check | `<sys/sysctl.h>` | direct |
| CheckRemoteDebuggerPresent | 동일 | `<sys/sysctl.h>` | own2 cycle 80 anti-debug |
| DebugBreak | __builtin_trap | compiler intrinsic | direct |
| RaiseException | own2 cycle 80 seh_raise | own2 cycle 80 | SEH chain |
| UnhandledExceptionFilter | own2 cycle 80 seh_unhandled | own2 cycle 80 | terminate |
| SetUnhandledExceptionFilter | own2 cycle 80 TLS | own2 cycle 80 | per-thread |
| GetStartupInfoW | own2 saved at startup | — | STARTUPINFOW |
| GetStdHandle | sentinel + fd 0/1/2 | `<unistd.h>` | STDIN/OUT/ERR |
| SetStdHandle | own2 stdio remap | — | dup2 |
| WriteConsoleW | write(1, UTF-8) | `<unistd.h>` | UTF-16 → UTF-8 |
| ReadConsoleW | read(0) + UTF-8 → UTF-16 | `<unistd.h>` | line-mode |
| GetConsoleMode / SetConsoleMode | termios tcgetattr/tcsetattr | `<termios.h>` | flag 매핑 |
| AllocConsole / FreeConsole | no-op | — | already attached |
| ExitProcess | exit(N) | `<stdlib.h>` | atexit 호출 |
| TerminateProcess | _exit(N) or kill | `<unistd.h>` | atexit skip |
| GetExitCodeProcess | own2 process table | — | wait4 결과 |
| CreateProcessW | posix_spawn + path translate | `<spawn.h>` | helper IPC kind=5 |
| OpenProcess | own2 process table fake handle | — | sandbox |
| WaitForSingleObject (process) | waitpid + dispatch_semaphore | `<sys/wait.h>` | (sync 카테고리 참조) |
| GetProcessTimes | proc_pidinfo | `<libproc.h>` | rusage |
| GetProcessAffinityMask | sysctl hw.ncpu mask | `<sys/sysctl.h>` | full-mask |
| SetProcessAffinityMask | thread_policy_set THREAD_AFFINITY_POLICY | `<mach/thread_policy.h>` | hint only on Apple |
| GetThreadTimes | thread_info THREAD_BASIC_INFO | `<mach/thread_act.h>` | user/sys time |
| GetThreadContext / SetThreadContext | thread_get_state / thread_set_state | `<mach/thread_act.h>` | x86 → arm64 매핑 X (own2 cycle 87 i386 only) |
| SwitchToThread | sched_yield | `<sched.h>` | direct |
| YieldProcessor | __builtin_ia32_pause / __asm__("yield") | compiler intrinsic | arch dependent |
| MultiByteToWideChar | own2 cycle 79 mbtowc + iconv | `<iconv.h>` | CP_UTF8/CP_ACP |
| WideCharToMultiByte | own2 cycle 79 wctomb + iconv | `<iconv.h>` | CP_UTF8/CP_ACP |
| lstrlenW / lstrlenA | wcslen / strlen | `<string.h>` | direct |
| lstrcpyW / lstrcatW | wcscpy / wcscat | `<wchar.h>` | direct |
| lstrcmpW / lstrcmpiW | wcscmp / wcscasecmp | `<wchar.h>` | locale-independent |
| GetACP / GetOEMCP | 65001 (UTF-8) | — | sentinel |
| IsValidCodePage | true (자체 iconv) | — | always true |
| GetCPInfo | UTF-8 CPINFO sentinel | — | leadbyte=0 |
| GetUserDefaultLCID | LOCALE_USER_DEFAULT 0x0400 | — | sentinel |
| GetSystemDefaultLCID | LOCALE_SYSTEM_DEFAULT 0x0800 | — | sentinel |
| GetLocaleInfoW | own2 cycle 82 locale_table | — | NSLocale 참조 |
| CompareStringW | wcscmp + LCMAP_*  | `<wchar.h>` | NSString compare |
| LCMapStringW | own2 cycle 82 lcmap | — | upper/lower |

---

## 2. KERNEL32 file I/O → POSIX libc (~30 함수)

| Win32 | POSIX/Apple | header | 비고 |
|---|---|---|---|
| CreateFileW | open(O_RDWR|O_CREAT, mode) + path translate | `<fcntl.h>` | DesiredAccess 매핑 |
| CreateFileMappingW | shm_open + ftruncate | `<sys/mman.h>` | 명명 mapping |
| MapViewOfFile | mmap | `<sys/mman.h>` | helper IPC kind=2 |
| UnmapViewOfFile | munmap | `<sys/mman.h>` | helper IPC kind=2 |
| FlushViewOfFile | msync | `<sys/mman.h>` | direct |
| ReadFile | read(fd, buf, n) | `<unistd.h>` | OVERLAPPED → aio_read |
| ReadFileEx | aio_read + completion routine | `<aio.h>` | callback |
| WriteFile | write(fd, buf, n) | `<unistd.h>` | OVERLAPPED → aio_write |
| WriteFileEx | aio_write | `<aio.h>` | callback |
| CloseHandle | close(fd) or own2 handle table | `<unistd.h>` | type-dispatch |
| SetFilePointer | lseek(int32 → int64 truncate) | `<unistd.h>` | direct |
| SetFilePointerEx | lseek (int64) | `<unistd.h>` | direct |
| GetFileSize | fstat → st_size lo32 | `<sys/stat.h>` | hi32 별도 |
| GetFileSizeEx | fstat → st_size | `<sys/stat.h>` | direct |
| GetFileType | fstat S_IFMT → FILE_TYPE_DISK/PIPE/CHAR | `<sys/stat.h>` | switch |
| GetFileAttributesW | stat + S_IFDIR / chflags UF_HIDDEN | `<sys/stat.h>` | mapping |
| GetFileAttributesExW | stat → WIN32_FILE_ATTRIBUTE_DATA | `<sys/stat.h>` | direct |
| SetFileAttributesW | chmod + chflags | `<sys/stat.h>` | best-effort |
| GetFileInformationByHandle | fstat → BY_HANDLE_FILE_INFORMATION | `<sys/stat.h>` | volume serial fake |
| FlushFileBuffers | fsync | `<unistd.h>` | direct |
| DeleteFileW | unlink + path translate | `<unistd.h>` | direct |
| CopyFileW | copyfile(COPYFILE_ALL) | `<copyfile.h>` | Apple BSD |
| CopyFileExW | copyfile + progress callback | `<copyfile.h>` | callback wrap |
| MoveFileW | rename | `<stdio.h>` | direct |
| MoveFileExW | rename + MOVEFILE_REPLACE_EXISTING | `<stdio.h>` | unlink-first |
| CreateDirectoryW | mkdir(path, 0755) | `<sys/stat.h>` | direct |
| RemoveDirectoryW | rmdir | `<unistd.h>` | direct |
| FindFirstFileW | opendir + readdir + fnmatch | `<dirent.h>` | wildcard |
| FindNextFileW | readdir + fnmatch | `<dirent.h>` | direct |
| FindClose | closedir | `<dirent.h>` | direct |
| GetFullPathNameW | realpath + path translate | `<stdlib.h>` | / → \\ |
| GetTempPathW | NSTemporaryDirectory | `<Foundation/NSPathUtilities.h>` | UTF-16 |
| GetTempFileNameW | mkstemp | `<stdlib.h>` | + close |
| LockFile / LockFileEx | fcntl(F_SETLK) | `<fcntl.h>` | advisory |
| UnlockFile / UnlockFileEx | fcntl(F_SETLK F_UNLCK) | `<fcntl.h>` | advisory |
| GetDriveTypeW | always DRIVE_FIXED | — | sentinel |
| GetLogicalDrives / GetLogicalDriveStringsW | "C:\\\0" sentinel | — | one-drive |
| GetVolumeInformationW | statvfs + UTF-16 label | `<sys/statvfs.h>` | NTFS sentinel |
| GetDiskFreeSpaceExW | statvfs f_bavail/f_blocks | `<sys/statvfs.h>` | direct |

---

## 3. KERNEL32 memory / heap → libc malloc + mach VM (~15 함수)

| Win32 | POSIX/Apple | header | 비고 |
|---|---|---|---|
| HeapCreate | own2 user-mode heap manager | own2 cycle 82 win32_kernel32_tier4_heap | per-heap arena |
| HeapDestroy | own2 arena free | own2 cycle 82 | block free 후 |
| HeapAlloc | malloc / posix_memalign | `<malloc/malloc.h>` | HEAP_ZERO_MEMORY → calloc |
| HeapFree | free | `<stdlib.h>` | direct |
| HeapReAlloc | realloc | `<stdlib.h>` | move flag 처리 |
| HeapSize | malloc_size | `<malloc/malloc.h>` | Apple ext |
| HeapValidate | malloc_zone_check | `<malloc/malloc.h>` | best-effort |
| HeapCompact | malloc_zone_pressure_relief | `<malloc/malloc.h>` | hint |
| HeapWalk | malloc_zone_log + iter | `<malloc/malloc.h>` | best-effort |
| GetProcessHeap | sentinel HANDLE 1 | — | global heap |
| GetProcessHeaps | own2 heap table iter | own2 cycle 82 | array fill |
| VirtualAlloc | mmap(MAP_ANON|MAP_PRIVATE) | `<sys/mman.h>` | helper IPC kind=2 |
| VirtualAllocEx | mach_vm_allocate (other proc) | `<mach/mach_vm.h>` | sandbox 제한 |
| VirtualFree | munmap + MEM_RELEASE / mmap MADV_FREE | `<sys/mman.h>` | helper IPC kind=2 |
| VirtualProtect | mprotect | `<sys/mman.h>` | helper IPC kind=3 (W^X) |
| VirtualQuery | mach_vm_region MEMORY_OBJECT_INFO | `<mach/mach_vm.h>` | MBI 변환 |
| VirtualLock / VirtualUnlock | mlock / munlock | `<sys/mman.h>` | direct |
| GlobalAlloc / GlobalFree | malloc / free | `<stdlib.h>` | GMEM_MOVEABLE 무시 |
| GlobalLock / GlobalUnlock | identity (return ptr) | — | no-op |
| LocalAlloc / LocalFree | malloc / free | `<stdlib.h>` | LMEM_MOVEABLE 무시 |
| GlobalMemoryStatusEx | host_statistics64 + sysctl | `<mach/mach_host.h>` | hw.memsize |

---

## 4. KERNEL32 sync → pthread + dispatch (~25 함수)

| Win32 | POSIX/Apple | header | 비고 |
|---|---|---|---|
| InitializeCriticalSection | pthread_mutex_init(PTHREAD_MUTEX_RECURSIVE) | `<pthread.h>` | recursive |
| InitializeCriticalSectionAndSpinCount | + own2 spin count slot | own2 cycle 82 | spin-then-block |
| InitializeCriticalSectionEx | + flags | `<pthread.h>` | NO_DEBUG_INFO 무시 |
| EnterCriticalSection | pthread_mutex_lock | `<pthread.h>` | direct |
| TryEnterCriticalSection | pthread_mutex_trylock | `<pthread.h>` | direct |
| LeaveCriticalSection | pthread_mutex_unlock | `<pthread.h>` | direct |
| DeleteCriticalSection | pthread_mutex_destroy | `<pthread.h>` | direct |
| CreateEventW | pthread_cond_init + manual/auto reset state | `<pthread.h>` | own2 cycle 82 |
| SetEvent | pthread_cond_signal / _broadcast | `<pthread.h>` | manual=broadcast |
| ResetEvent | own2 state slot clear | own2 cycle 82 | manual only |
| CreateMutexW | pthread_mutex_init | `<pthread.h>` | named → table |
| OpenMutexW | own2 named-object table lookup | own2 cycle 82 | named |
| ReleaseMutex | pthread_mutex_unlock | `<pthread.h>` | direct |
| CreateSemaphoreW | dispatch_semaphore_create | `<dispatch/dispatch.h>` | initial count |
| ReleaseSemaphore | dispatch_semaphore_signal × N | `<dispatch/dispatch.h>` | loop |
| WaitForSingleObject | dispatch_semaphore_wait or pthread_cond_timedwait | `<dispatch/dispatch.h>` | type-dispatch |
| WaitForSingleObjectEx | + alertable APC queue | own2 cycle 82 | alert false |
| WaitForMultipleObjects | own2 cycle 82 wait_multi | — | OR/AND |
| WaitForMultipleObjectsEx | + alertable | own2 cycle 82 | — |
| SignalObjectAndWait | atomic signal + wait | own2 cycle 82 | sequence |
| InitializeSRWLock | pthread_rwlock_init | `<pthread.h>` | direct |
| AcquireSRWLockExclusive / Shared | pthread_rwlock_wrlock / rdlock | `<pthread.h>` | direct |
| ReleaseSRWLockExclusive / Shared | pthread_rwlock_unlock | `<pthread.h>` | direct |
| InitializeConditionVariable | pthread_cond_init | `<pthread.h>` | direct |
| SleepConditionVariableSRW | pthread_cond_timedwait | `<pthread.h>` | direct |
| WakeConditionVariable / WakeAllConditionVariable | pthread_cond_signal / _broadcast | `<pthread.h>` | direct |
| InitOnceExecuteOnce | pthread_once | `<pthread.h>` | direct |
| InterlockedIncrement / Decrement / Exchange / CompareExchange | __atomic_* / OSAtomic* | `<stdatomic.h>` | C11 atomics |
| MemoryBarrier | __sync_synchronize | compiler intrinsic | direct |
| Sleep | usleep | `<unistd.h>` | (중복, 시간 카테고리 참조) |

---

## 5. KERNEL32 thread → pthread (~10 함수)

| Win32 | POSIX/Apple | header | 비고 |
|---|---|---|---|
| CreateThread | pthread_create | `<pthread.h>` | helper IPC kind=4 (entitlement) |
| _beginthreadex (CRT) | pthread_create + CRT init | `<pthread.h>` | CRT TLS |
| ExitThread | pthread_exit | `<pthread.h>` | direct |
| TerminateThread | pthread_cancel | `<pthread.h>` | unsafe but spec |
| GetCurrentThread | pseudo handle (HANDLE)-2 | — | self |
| GetCurrentThreadId | pthread_mach_thread_np(pthread_self()) | `<pthread.h>` | uint32 |
| ResumeThread | thread_resume | `<mach/thread_act.h>` | suspend count |
| SuspendThread | thread_suspend | `<mach/thread_act.h>` | suspend count |
| SetThreadPriority | pthread_setschedparam | `<pthread.h>` | THREAD_PRIORITY_* 매핑 |
| GetThreadPriority | pthread_getschedparam | `<pthread.h>` | inverse |
| SetThreadDescription | pthread_setname_np | `<pthread.h>` | UTF-16 → UTF-8 |
| GetThreadDescription | pthread_getname_np | `<pthread.h>` | UTF-8 → UTF-16 |
| QueueUserAPC | own2 cycle 82 apc_queue | own2 cycle 82 | per-thread |
| TlsAlloc | pthread_key_create | `<pthread.h>` | own2 cycle 82 pe_tls_alloc_real |
| TlsGetValue | pthread_getspecific | `<pthread.h>` | direct |
| TlsSetValue | pthread_setspecific | `<pthread.h>` | direct |
| TlsFree | pthread_key_delete | `<pthread.h>` | direct |
| FlsAlloc / FlsGetValue / FlsSetValue / FlsFree | 동일 + dtor | `<pthread.h>` | fiber→thread |

---

## 6. USER32 → AppKit / CoreGraphics (~50 함수, window/event/input)

| Win32 | AppKit / CG | header | 비고 |
|---|---|---|---|
| GetSystemMetrics(SM_CXSCREEN/CYSCREEN) | NSScreen.mainScreen.frame.size | `<AppKit/NSScreen.h>` | backing scale 별도 |
| GetSystemMetrics(SM_CXFULLSCREEN) | NSScreen visibleFrame | `<AppKit/NSScreen.h>` | menu bar 제외 |
| GetSystemMetrics(SM_CMOUSEBUTTONS) | sentinel 5 | — | NSEvent 없음 |
| GetCursorPos | CGEventGetLocation(NULL event) or NSEvent.mouseLocation | `<CoreGraphics/CGEvent.h>` | flip Y |
| SetCursorPos | CGWarpMouseCursorPosition | `<CoreGraphics/CGRemoteOperation.h>` | requires entitlement |
| GetForegroundWindow | NSApp.keyWindow → own2 HWND map | `<AppKit/NSApplication.h>` | window table |
| GetActiveWindow | NSApp.mainWindow → HWND | `<AppKit/NSApplication.h>` | direct |
| SetForegroundWindow | NSWindow makeKeyAndOrderFront | `<AppKit/NSWindow.h>` | activation |
| GetFocus / SetFocus | NSWindow firstResponder | `<AppKit/NSWindow.h>` | makeFirstResponder |
| FindWindowW | own2 window table title scan | own2 cycle 83 | linear |
| GetWindowRect / GetClientRect | NSWindow.frame / contentRectForFrameRect | `<AppKit/NSWindow.h>` | flip Y |
| ScreenToClient / ClientToScreen | NSWindow convertRectFromScreen | `<AppKit/NSWindow.h>` | flip Y |
| MoveWindow / SetWindowPos | NSWindow setFrame:display:animate: | `<AppKit/NSWindow.h>` | direct |
| ShowWindow | NSWindow orderFront / orderOut / miniaturize | `<AppKit/NSWindow.h>` | SW_* 매핑 |
| UpdateWindow | NSView setNeedsDisplay + display | `<AppKit/NSView.h>` | sync |
| InvalidateRect | NSView setNeedsDisplayInRect | `<AppKit/NSView.h>` | direct |
| ValidateRect | own2 dirty-rect mask clear | own2 cycle 83 | hint |
| BeginPaint / EndPaint | NSView lockFocus / unlockFocus | `<AppKit/NSView.h>` | drawRect |
| GetDC / ReleaseDC | own2 CG bitmap context per-window | own2 cycle 83 | (GDI 카테고리 참조) |
| CreateWindowExW | NSWindow alloc + style mask | `<AppKit/NSWindow.h>` | WS_* → NSWindowStyleMask |
| DestroyWindow | NSWindow close + dealloc | `<AppKit/NSWindow.h>` | own2 table 제거 |
| RegisterClassExW | own2 wndclass table | own2 cycle 83 | wndproc 등록 |
| UnregisterClassW | own2 wndclass table 제거 | own2 cycle 83 | direct |
| GetMessage / PeekMessage | NSApplication nextEventMatchingMask:untilDate: | `<AppKit/NSApplication.h>` | event loop |
| TranslateMessage | NSEvent → IME (interpretKeyEvents) | `<AppKit/NSEvent.h>` | dead key |
| DispatchMessage | NSWindow sendEvent → own2 wndproc | `<AppKit/NSWindow.h>` | dispatch |
| PostMessageW / SendMessageW | own2 message queue | own2 cycle 83 | thread-safe |
| PostQuitMessage | NSApp stop | `<AppKit/NSApplication.h>` | direct |
| MessageBoxW | NSAlert runModal | `<AppKit/NSAlert.h>` | MB_* → 버튼 매핑 |
| LoadCursorW | NSCursor system cursor | `<AppKit/NSCursor.h>` | IDC_ARROW 등 |
| SetCursor | NSCursor.set | `<AppKit/NSCursor.h>` | direct |
| ShowCursor | CGDisplayShowCursor / HideCursor | `<CoreGraphics/CGRemoteOperation.h>` | refcount |
| LoadIconW | NSImage imageNamed | `<AppKit/NSImage.h>` | resource lookup |
| LoadBitmapW | NSBitmapImageRep | `<AppKit/NSBitmapImageRep.h>` | resource lookup |
| GetKeyState | NSEvent modifierFlags + own2 KB state | `<AppKit/NSEvent.h>` | bit-field |
| GetAsyncKeyState | own2 cycle 83 keyboard hook state | own2 cycle 83 | hook chain |
| GetKeyboardState | own2 256-byte state | own2 cycle 83 | snapshot |
| MapVirtualKeyW | own2 VK ↔ NSEvent keyCode table | own2 cycle 83 | static map |
| ToUnicode | UCKeyTranslate | `<Carbon/HIToolbox/UchrInputData.h>` | dead key |
| RegisterHotKey | NSEvent addGlobalMonitorForEventsMatchingMask | `<AppKit/NSEvent.h>` | accessibility |
| SetTimer / KillTimer | dispatch_source_timer + own2 table | `<dispatch/dispatch.h>` | per-window |
| GetDoubleClickTime | NSEvent doubleClickInterval × 1000 | `<AppKit/NSEvent.h>` | ms |
| ClipCursor | CGAssociateMouseAndMouseCursorPosition + rect | `<CoreGraphics/CGRemoteOperation.h>` | best-effort |
| ChangeDisplaySettingsExW | CGDisplaySetDisplayMode | `<CoreGraphics/CGDirectDisplay.h>` | mode lookup |
| EnumDisplaySettingsW | CGDisplayCopyAllDisplayModes | `<CoreGraphics/CGDirectDisplay.h>` | iter |
| MonitorFromWindow | NSWindow.screen → CGDirectDisplayID | `<AppKit/NSScreen.h>` | NSScreenNumber |
| GetMonitorInfoW | CGDisplayBounds + CGDisplayModeGetWidth/Height | `<CoreGraphics/CGDirectDisplay.h>` | direct |
| GetClipboardData / SetClipboardData | NSPasteboard general | `<AppKit/NSPasteboard.h>` | UTF-16 형변환 |
| OpenClipboard / CloseClipboard / EmptyClipboard | NSPasteboard clearContents | `<AppKit/NSPasteboard.h>` | no-op open/close |

---

## 7. ADVAPI32 → security / Foundation (~20 함수)

| Win32 | POSIX/Apple | header | 비고 |
|---|---|---|---|
| GetUserNameW | getpwuid(getuid())->pw_name + UTF-16 | `<pwd.h>` | direct |
| LookupAccountSidW | own2 SID table fake | own2 cycle 82 | sentinel |
| RegOpenKeyExW | own2 file-based registry hive | own2 cycle 82 win32_advapi32_tier2_real | path map |
| RegCreateKeyExW | own2 hive create | own2 cycle 82 | mkdir-like |
| RegCloseKey | own2 hive close | own2 cycle 82 | direct |
| RegQueryValueExW | own2 hive lookup | own2 cycle 82 | type-dispatch |
| RegSetValueExW | own2 hive write | own2 cycle 82 | atomic temp+rename |
| RegDeleteKeyW / RegDeleteValueW | own2 hive remove | own2 cycle 82 | direct |
| RegEnumKeyExW / RegEnumValueW | own2 hive iter | own2 cycle 82 | index-based |
| RegFlushKey | own2 hive fsync | own2 cycle 82 | direct |
| RegNotifyChangeKeyValue | own2 dispatch_source_vnode | `<dispatch/dispatch.h>` | best-effort |
| OpenProcessToken | sentinel fake handle | — | sandbox |
| GetTokenInformation | TokenUser fake getuid | — | sandbox |
| AdjustTokenPrivileges | no-op success | — | sandbox |
| ImpersonateLoggedOnUser / RevertToSelf | no-op success | — | sandbox |
| AllocateAndInitializeSid / FreeSid | malloc / free | `<stdlib.h>` | opaque blob |
| ConvertSidToStringSidW | "S-1-5-21-..." sentinel | — | static |
| CryptGenRandom | SecRandomCopyBytes | `<Security/SecRandom.h>` | direct |
| OpenSCManager / OpenServiceW | no-op fake handle | — | service none |
| StartServiceW / ControlService | no-op false | — | service none |
| EventRegister / EventWrite (ETW) | os_log + os_signpost | `<os/log.h>` | best-effort |
| ReportEventW | os_log_with_type | `<os/log.h>` | level 매핑 |

---

## 8. GDI32 → CoreGraphics + CoreText (~18 함수)

| Win32 | CG / CT | header | 비고 |
|---|---|---|---|
| CreateCompatibleDC | own2 DC table + CGBitmapContext | own2 cycle 83 | per-DC state |
| CreateCompatibleBitmap | CGBitmapContextCreate(NULL, w, h, 8, w*4, RGBA) | `<CoreGraphics/CGBitmapContext.h>` | RGBA8 |
| CreateDIBSection | mmap + CGBitmapContextCreate(buf, ...) | `<CoreGraphics/CGBitmapContext.h>` | shared mem |
| SelectObject | own2 DC state slot 교체 | own2 cycle 83 | bitmap/font/pen |
| DeleteObject | own2 handle table free + CGRelease | own2 cycle 83 | type-dispatch |
| DeleteDC | own2 DC table free + CGContextRelease | `<CoreGraphics/CGContext.h>` | direct |
| BitBlt | CGContextDrawImage + ROP | `<CoreGraphics/CGContext.h>` | SRCCOPY only common |
| StretchBlt | CGContextDrawImage scaled | `<CoreGraphics/CGContext.h>` | direct |
| AlphaBlend | CGContextSetAlpha + DrawImage | `<CoreGraphics/CGContext.h>` | per-pixel |
| TextOutW | CTLineDraw | `<CoreText/CTLine.h>` | UTF-16 → CFString |
| ExtTextOutW | CTLineDraw + clip | `<CoreText/CTLine.h>` | options |
| GetTextExtentPoint32W | CTLineGetTypographicBounds | `<CoreText/CTLine.h>` | width/height |
| SetTextColor | own2 DC state slot (CGContextSetRGBFillColor at draw) | `<CoreGraphics/CGContext.h>` | deferred |
| SetBkColor / SetBkMode | own2 DC state slot | own2 cycle 83 | deferred |
| CreateFontW / CreateFontIndirectW | CTFontCreateWithName | `<CoreText/CTFont.h>` | LOGFONT 매핑 |
| CreatePen / CreateSolidBrush | own2 GDI obj table (CG state) | own2 cycle 83 | per-DC apply |
| Rectangle / Ellipse / LineTo | CGContextStrokeRect / StrokeEllipse / AddLineToPoint | `<CoreGraphics/CGContext.h>` | path build |
| MoveToEx | CGContextMoveToPoint | `<CoreGraphics/CGContext.h>` | direct |
| GetDeviceCaps | NSScreen + CG resolution constants | `<AppKit/NSScreen.h>` | LOGPIXELSX=72 등 |
| GdiFlush | CGContextFlush | `<CoreGraphics/CGContext.h>` | direct |

---

## 9. WS2_32 → BSD socket (~31 함수, 일부 ordinal-only)

| Win32 ordinal | BSD socket | header | 비고 |
|---|---|---|---|
| socket (#23) | socket | `<sys/socket.h>` | AF_/SOCK_ 매핑 |
| connect (#4) | connect | `<sys/socket.h>` | direct |
| accept (#1) | accept | `<sys/socket.h>` | direct |
| bind (#2) | bind | `<sys/socket.h>` | direct |
| listen (#13) | listen | `<sys/socket.h>` | direct |
| recv (#16) | recv | `<sys/socket.h>` | flags 매핑 |
| recvfrom (#17) | recvfrom | `<sys/socket.h>` | direct |
| send (#19) | send | `<sys/socket.h>` | direct |
| sendto (#20) | sendto | `<sys/socket.h>` | direct |
| select (#18) | select | `<sys/select.h>` | timeval |
| WSAPoll | poll | `<poll.h>` | events 매핑 |
| WSAStartup | no-op success | — | version 무시 |
| WSACleanup | no-op success | — | direct |
| WSAGetLastError | own2 TLS errno → WSA code 매핑 | own2 cycle 82 | EAGAIN→WSAEWOULDBLOCK |
| WSASetLastError | own2 TLS errno set | own2 cycle 82 | inverse |
| ioctlsocket(FIONBIO) | fcntl(F_SETFL, O_NONBLOCK) | `<fcntl.h>` | flag |
| ioctlsocket(FIONREAD) | ioctl(FIONREAD) | `<sys/ioctl.h>` | direct |
| closesocket (#3) | close | `<unistd.h>` | direct |
| shutdown (#22) | shutdown | `<sys/socket.h>` | how 매핑 |
| getsockname (#6) / getpeername (#5) | 동일 | `<sys/socket.h>` | direct |
| getsockopt (#7) / setsockopt (#21) | 동일 | `<sys/socket.h>` | level 매핑 |
| inet_addr / inet_ntoa | inet_aton / inet_ntoa | `<arpa/inet.h>` | direct |
| inet_pton / inet_ntop | 동일 | `<arpa/inet.h>` | direct |
| htons / htonl / ntohs / ntohl | 동일 | `<arpa/inet.h>` | direct |
| gethostbyname / gethostbyaddr | DEPRECATED — getaddrinfo wrap | `<netdb.h>` | wrap |
| getaddrinfo / freeaddrinfo / getnameinfo | 동일 | `<netdb.h>` | direct |
| WSAAsyncSelect | own2 dispatch_source_read/write | `<dispatch/dispatch.h>` | message post |
| WSAEventSelect / WSAEnumNetworkEvents | own2 cycle 82 wsa_event | own2 cycle 82 | event mask |
| WSARecv / WSASend | recvmsg / sendmsg + own2 OVERLAPPED | `<sys/socket.h>` | aio_read/write 옵션 |
| WSAIoctl(SIO_GET_EXTENSION_FUNCTION_POINTER) | own2 ext fn table | own2 cycle 82 | AcceptEx etc |

---

## 10. CRYPT32 / BCRYPT → Security framework + CommonCrypto (~18 함수)

| Win32 | Apple | header | 비고 |
|---|---|---|---|
| CryptAcquireContextW | no-op fake handle | — | provider 무시 |
| CryptReleaseContext | no-op | — | direct |
| CryptCreateHash | own2 hash ctx (CC_SHA1/256/512_CTX) | `<CommonCrypto/CommonDigest.h>` | algid 매핑 |
| CryptHashData | CC_SHA*_Update | `<CommonCrypto/CommonDigest.h>` | direct |
| CryptGetHashParam(HP_HASHVAL) | CC_SHA*_Final | `<CommonCrypto/CommonDigest.h>` | finalize |
| CryptDestroyHash | free ctx | — | direct |
| CryptGenRandom | SecRandomCopyBytes(kSecRandomDefault) | `<Security/SecRandom.h>` | direct |
| CryptEncrypt / CryptDecrypt | CCCrypt(AES/3DES) | `<CommonCrypto/CommonCryptor.h>` | direct |
| CryptImportKey / CryptExportKey | own2 key blob ↔ CCKey | own2 cycle 82 | format 변환 |
| CryptSignHashW | SecKeyCreateSignature | `<Security/SecKey.h>` | RSA/ECDSA |
| CryptVerifySignatureW | SecKeyVerifySignature | `<Security/SecKey.h>` | direct |
| CertOpenSystemStoreW | SecTrustCopyAnchorCertificates | `<Security/SecTrust.h>` | "ROOT" |
| CertFindCertificateInStore | SecItemCopyMatching | `<Security/SecItem.h>` | query dict |
| CertFreeCertificateContext | CFRelease | `<CoreFoundation/CFBase.h>` | direct |
| CertVerifyCertificateChainPolicy | SecTrustEvaluateWithError | `<Security/SecTrust.h>` | result 매핑 |
| BCryptOpenAlgorithmProvider | own2 algorithm table | own2 cycle 82 | algid lookup |
| BCryptGenRandom | SecRandomCopyBytes | `<Security/SecRandom.h>` | direct |
| BCryptHashData / BCryptFinishHash | CC_SHA*_Update / _Final | `<CommonCrypto/CommonDigest.h>` | direct |

---

## 11. ole32 / OLEAUT32 → Foundation + own2 COM emul (~6+ 함수)

| Win32 | Apple / own2 | header | 비고 |
|---|---|---|---|
| CoInitializeEx | own2 cycle 77 pe_com_initialize | own2 cycle 77 | apartment |
| CoInitialize | 동일 (APARTMENTTHREADED) | own2 cycle 77 | wrap |
| CoUninitialize | own2 cycle 77 com_uninit | own2 cycle 77 | refcount |
| CoCreateInstance | own2 CLSID lookup table | own2 cycle 77 | 게임별 known CLSID |
| CoCreateInstanceEx | + multi-IID | own2 cycle 77 | array fill |
| CoTaskMemAlloc / CoTaskMemFree | malloc / free | `<stdlib.h>` | direct |
| CoTaskMemRealloc | realloc | `<stdlib.h>` | direct |
| CoCreateGuid | uuid_generate | `<uuid/uuid.h>` | Apple |
| StringFromGUID2 | uuid_unparse + UTF-16 | `<uuid/uuid.h>` | direct |
| CLSIDFromString | uuid_parse | `<uuid/uuid.h>` | direct |
| SysAllocString | malloc(len*2 + 4 + 2) + 4-byte len prefix | `<stdlib.h>` | BSTR layout |
| SysAllocStringLen | 동일 + 길이 지정 | `<stdlib.h>` | direct |
| SysFreeString | free(ptr - 4) | `<stdlib.h>` | prefix offset |
| SysStringLen | *(uint32*)(ptr - 4) / 2 | — | inline |
| SysStringByteLen | *(uint32*)(ptr - 4) | — | inline |
| VariantInit | memset 0 + VT_EMPTY | `<string.h>` | direct |
| VariantClear | type-dispatch release (BSTR/IUnknown) | own2 cycle 77 | switch |
| VariantCopy | + BSTR dup / AddRef | own2 cycle 77 | deep |
| VariantChangeType | own2 cycle 77 variant_coerce | own2 cycle 77 | int↔str |
| SafeArrayCreate / SafeArrayDestroy | own2 SAFEARRAY emul | own2 cycle 77 | malloc + descriptor |

---

## 12. DXGI / D3D11 / D3D12 → Metal (~30 함수, 일부 D3DMetal 위임)

| Win32 | Metal | header | 비고 |
|---|---|---|---|
| D3D11CreateDevice | MTLCreateSystemDefaultDevice | `<Metal/MTLDevice.h>` | helper IPC kind=7 |
| D3D11CreateDeviceAndSwapChain | + CAMetalLayer | `<QuartzCore/CAMetalLayer.h>` | NSWindow contentView |
| ID3D11Device::CreateBuffer | MTLDevice newBufferWithLength:options: | `<Metal/MTLBuffer.h>` | usage 매핑 |
| ID3D11Device::CreateTexture2D | newTextureWithDescriptor | `<Metal/MTLTexture.h>` | format 매핑 |
| ID3D11Device::CreateTexture3D | 동일 | `<Metal/MTLTexture.h>` | type=3D |
| ID3D11Device::CreateShaderResourceView | MTLTexture newTextureViewWithPixelFormat | `<Metal/MTLTexture.h>` | view |
| ID3D11Device::CreateRenderTargetView | MTLRenderPassColorAttachmentDescriptor | `<Metal/MTLRenderPass.h>` | per-pass |
| ID3D11Device::CreateDepthStencilView | MTLRenderPassDepthAttachmentDescriptor | `<Metal/MTLRenderPass.h>` | per-pass |
| ID3D11Device::CreateVertexShader / PixelShader | MTLLibrary newFunctionWithName | `<Metal/MTLLibrary.h>` | DXIL→AIR (D3DMetal) |
| ID3D11Device::CreateInputLayout | MTLVertexDescriptor | `<Metal/MTLVertexDescriptor.h>` | semantic 매핑 |
| ID3D11Device::CreateBlendState | MTLRenderPipelineColorAttachmentDescriptor | `<Metal/MTLRenderPipeline.h>` | direct |
| ID3D11Device::CreateRasterizerState | MTLRenderCommandEncoder setCullMode etc | `<Metal/MTLRenderCommandEncoder.h>` | per-encoder |
| ID3D11Device::CreateDepthStencilState | MTLDepthStencilState | `<Metal/MTLDepthStencil.h>` | direct |
| ID3D11Device::CreateSamplerState | MTLSamplerState | `<Metal/MTLSampler.h>` | direct |
| ID3D11DeviceContext::Draw | MTLRenderCommandEncoder drawPrimitives | `<Metal/MTLRenderCommandEncoder.h>` | topology 매핑 |
| ID3D11DeviceContext::DrawIndexed | drawIndexedPrimitives | `<Metal/MTLRenderCommandEncoder.h>` | direct |
| ID3D11DeviceContext::DrawInstanced | drawPrimitives instanceCount | `<Metal/MTLRenderCommandEncoder.h>` | direct |
| ID3D11DeviceContext::Map / Unmap | MTLBuffer contents + didModifyRange | `<Metal/MTLBuffer.h>` | usage 의존 |
| ID3D11DeviceContext::UpdateSubresource | MTLBuffer didModifyRange / MTLTexture replaceRegion | `<Metal/MTLBuffer.h>` | direct |
| ID3D11DeviceContext::CopyResource / CopySubresourceRegion | MTLBlitCommandEncoder copyFromBuffer/Texture | `<Metal/MTLBlitCommandEncoder.h>` | direct |
| ID3D11DeviceContext::ClearRenderTargetView / DepthStencilView | MTLRenderPass loadAction=clear | `<Metal/MTLRenderPass.h>` | per-pass |
| ID3D11DeviceContext::OMSetRenderTargets | MTLRenderPassDescriptor 설정 | `<Metal/MTLRenderPass.h>` | per-pass |
| ID3D11DeviceContext::IASetVertexBuffers | MTLRenderCommandEncoder setVertexBuffer:offset:atIndex: | `<Metal/MTLRenderCommandEncoder.h>` | direct |
| ID3D11DeviceContext::IASetIndexBuffer | drawIndexedPrimitives 인자로 전달 | `<Metal/MTLRenderCommandEncoder.h>` | per-draw |
| ID3D11DeviceContext::PSSetShaderResources | setFragmentTexture:atIndex: | `<Metal/MTLRenderCommandEncoder.h>` | direct |
| ID3D11DeviceContext::PSSetSamplers | setFragmentSamplerState:atIndex: | `<Metal/MTLRenderCommandEncoder.h>` | direct |
| IDXGIFactory::CreateSwapChain / CreateSwapChainForHwnd | CAMetalLayer attach | `<QuartzCore/CAMetalLayer.h>` | NSWindow contentView |
| IDXGISwapChain::Present | CAMetalLayer nextDrawable + presentDrawable + commit | `<QuartzCore/CAMetalDrawable.h>` | vsync |
| IDXGISwapChain::ResizeBuffers | CAMetalLayer drawableSize 갱신 | `<QuartzCore/CAMetalLayer.h>` | direct |
| IDXGIAdapter::GetDesc | MTLDevice name/recommendedMaxWorkingSetSize | `<Metal/MTLDevice.h>` | 매핑 |
| IDXGIOutput::GetDisplayModeList | CGDisplayCopyAllDisplayModes | `<CoreGraphics/CGDirectDisplay.h>` | iter |

> D3DMetal binary 위임 가능 영역 ~85% (DXIL→AIR translation, command list replay), DIY fallback ~15% (state object, swap chain, factory).

---

## 13. DLL forwarder / api-set (~20 케이스)

| api-set / forwarder | 실제 destination | 비고 |
|---|---|---|
| api-ms-win-core-string-l1-1-0.dll | kernelbase.dll | string fn |
| api-ms-win-core-file-l1-1-0.dll | kernelbase.dll | file I/O |
| api-ms-win-core-heap-l1-1-0.dll | kernelbase.dll | HeapAlloc 등 |
| api-ms-win-core-memory-l1-1-0.dll | kernelbase.dll | VirtualAlloc 등 |
| api-ms-win-core-synch-l1-1-0.dll | kernelbase.dll | sync primitives |
| api-ms-win-core-processthreads-l1-1-0.dll | kernelbase.dll | thread/process |
| api-ms-win-core-libraryloader-l1-1-0.dll | kernelbase.dll | LoadLibrary |
| api-ms-win-core-handle-l1-1-0.dll | kernelbase.dll | CloseHandle |
| api-ms-win-core-errorhandling-l1-1-0.dll | kernelbase.dll | GetLastError |
| api-ms-win-core-localization-l1-2-0.dll | kernelbase.dll | locale |
| api-ms-win-crt-runtime-l1-1-0.dll | ucrtbase.dll | CRT |
| api-ms-win-crt-string-l1-1-0.dll | ucrtbase.dll | strlen 등 |
| api-ms-win-crt-stdio-l1-1-0.dll | ucrtbase.dll | printf 등 |
| api-ms-win-crt-math-l1-1-0.dll | ucrtbase.dll | sin 등 |
| KERNEL32!HeapAlloc | NTDLL!RtlAllocateHeap | forwarder string |
| KERNEL32!HeapFree | NTDLL!RtlFreeHeap | forwarder string |
| KERNEL32!RaiseException | NTDLL!RtlRaiseException | forwarder string |
| KERNEL32!GetCurrentTeb | NTDLL!NtCurrentTeb (i386 fs:[0x18]) | own2 cycle 87 |
| msvcr140.dll | ucrtbase.dll | redirect |
| vcruntime140.dll | partial → ucrtbase + own2 cycle 79 vcrt | helper EH |

> own2 cycle 79 forwarder 처리 — `IMAGE_EXPORT_DIRECTORY` AddressOfFunctions 가 RVA 가 export section 안에 있으면 forwarder string ("DLL.fn") 으로 해석.

---

## 14. cycle 87 + 88 i386 specific 추가 매핑

| 영역 | 매핑 | own2 cycle |
|---|---|---|
| IMAGE_THUNK_DATA32 (i386) high bit 0x80000000 | ordinal lookup (lo16 mask) | cycle 87 pe_i386_iat |
| FS:[0] EXCEPTION_REGISTRATION_RECORD chain | per-thread mmap + segment register emul | cycle 87 pe_i386_seh_fs0_chain |
| FS:[0x18] / FS:[0x30] TEB layout | own2 fake TEB struct + ldt_entry | cycle 87 pe_i386_teb_fs_segment |
| IMAGE_REL_BASED_HIGHLOW (type 3) | base relocation: *(uint32*)addr += delta | cycle 87 pe_i386_relocation_highlow |
| IMAGE_REL_BASED_HIGH / LOW (type 1/2) | half-only: *(uint16*) | cycle 87 pe_i386_relocation_halves |
| stdcall name decoration `_fn@N` | own2 export lookup strip prefix/suffix | cycle 88 pe_i386_export_decorate |
| fastcall `@fn@N` | 동일 + first 2 args ECX/EDX 호출규약 | cycle 88 pe_i386_fastcall_thunk |
| __ImageBase / __DllMainCRTStartup | i386 CRT entry — call DllMain DLL_PROCESS_ATTACH | cycle 88 pe_i386_crt_entry |
| __security_cookie / __security_check_cookie | per-DLL random + check | cycle 88 pe_i386_security_cookie |
| _CxxThrowException / __CxxFrameHandler3 | i386 SEH-based C++ EH | cycle 88 pe_i386_cxx_eh |

---

## 15. 매핑되지 않은 / 부분 매핑 함수 영역

- **Win32 SEH** (`RtlUnwind`, `__C_specific_handler`, `_except_handler4`) — own2 cycle 80 `pe_seh_unwinder_real` 자체 구현. x86_64 .pdata/.xdata 만 cover, i386 fs:[0] 은 cycle 87.
- **Win32 anti-debug** (`NtQueryInformationProcess(ProcessDebugPort/ProcessDebugFlags)`) — own2 cycle 80 fake null/zero 반환.
- **Win32 IME** (Korean/Japanese specific `ImmGetContext`, `ImmSetCompositionStringW`) — phase 후순위, 현재 NSTextInputContext 위임 stub 만.
- **DirectInput 8** (`IDirectInput8::CreateDevice`, `IDirectInputDevice8::GetDeviceState`) — phase 후순위, NSEvent + IOHIDManager 매핑 예정.
- **XInput 1.4** (`XInputGetState`, `XInputSetState`) — IOKit IOHIDManager + GameController.framework 매핑.
- **MediaFoundation / DirectShow** — 비매핑, AVFoundation 으로 전환.
- **WMI** (`IWbemServices`) — sysctl/IOKit dispatch table 위임 phase 후순위.
- **Windows Sockets RIO** (`RIOReceive`, `RIOSend`) — kqueue 매핑 phase 후순위.

---

## 16. own1 정합 검증

- 모든 매핑 = Apple SDK (libSystem, Foundation, AppKit, CoreGraphics, CoreText, Metal, QuartzCore, Security, CommonCrypto, CoreFoundation, IOKit, dispatch, mach VM) + Microsoft MSDN spec.
- Wine `wine/dlls/kernel32/*` / Reactos `dll/win32/*` / community symbol map (winapi-tracker, winapi_research, ApiMonitor) 인용 0줄.
- 본 표의 모든 Apple side 함수는 Apple Developer Documentation 기준으로 검증 가능.
- 본 표의 모든 Win32 side 함수는 MSDN documented signature 그대로.
- own2 cycle N 자체 구현 영역은 Wine port 가 아닌 spec-from-MSDN 재구현임을 cycle 별 review 에서 검증.
- 향후 phase 10 IAT routing 코드 작성 시 본 표만 reference, Wine source 금지.
