# R6 — 220+ DLL stub 함수 실 구현 만점 검토

> **status**: 작성 **2026-05-01** (cycle 79 직후 R-series 잔여 로드맵 entry)
> **scope**: `lib/loader/win32_*_stage*.hexa` (cycle 58–61 stage 시리즈) 전체 stub 함수의 *실 동작화* — 표 기록·추적기에서 실제 macOS syscall + Apple BSD 매핑까지.
> **SSOT**: [REMAINING_ROADMAP.md](REMAINING_ROADMAP.md) §R6, [SCORING.md](SCORING.md) (5축 채점), [PURPLE_LINEAGE_MANJEOM_REVIEW.md](PURPLE_LINEAGE_MANJEOM_REVIEW.md) (형식 모형)
> **own 1 강제**: Wine / Reactos / CrossOver / Whisky / GPTK 코드 **0줄**. 각 함수의 의미는 **MSDN (`learn.microsoft.com/en-us/windows/win32/api/...`) 만 참조** — Wine `dlls/kernel32` source tree, Reactos `reactos/dll/win32` source tree 둘 다 import / copy / port 금지.
> **결론 한줄**: **/25 만점은 도달 불가 (16/25 = "후순위" tier)** — Tier 1~2 (~120 함수) 는 1 cycle 당 ~30 함수 페이스로 **4 cycle 내 만점 가능**, 그러나 Tier 4~5 (handle table / SEH / TLS / Critical Section / COM) 는 R1~R3 prereq 동시 필요 + Apple BSD ↔ Win32 NT semantics 비대칭으로 own1 정합 X 항목 존재 → **함수별 split scoring 필수**.

---

## 0. 컨텍스트

### 0-1. R6 의 정확한 정의

cycle 58–61 batch 에서 작성된 `win32_*_stage{1..5}.hexa` 들은 다음 5단계 layered stub:

| stage | 의미 | 현 상태 |
|---|---|---|
| stage1 | mapping table 기록 (TSV) | ✓ 완료 |
| stage2 | call profile + arg shape 추적 | ✓ 완료 |
| stage3 | hot path forecast (Holt) | ✓ 완료 (일부 DLL) |
| stage4 | result_cache (return value memo) | ✓ 완료 (일부) |
| stage5 | first-call vs re-call 구분 + r0_emit | ✓ 완료 (일부) |
| **R6** | **실 동작 — 진짜 BSD/POSIX/Mach syscall 호출** | **본 검토 대상 — 미진** |

→ 즉 `GetSystemTimeAsFileTime` 호출 시 **현재**: TSV 한 줄 기록 + synthetic 0 반환. **R6 만점 후**: `gettimeofday(2)` → Win32 epoch (1601-01-01 UTC) 보정 → `FILETIME` 64bit struct 기입.

### 0-2. DLL 별 함수 분량 (실측 — Battle.net `Battle.net Setup.exe` 기준)

| DLL | 함수 수 | 비고 |
|---|---|---|
| KERNEL32 | **224** | 가장 큼. file IO / memory / sync / process 전반 |
| ADVAPI32 | 53 | registry / service / SAM / cryptoapi |
| USER32 | 50 | window / message / input |
| WS2_32 | 31 | 대부분 ordinal (1–115) — Berkeley socket 매핑 |
| GDI32 | 18 | DC / brush / font / bitmap |
| CRYPT32 | 18 | CMS / cert / cryptmsg |
| WININET | 11 | WinINet (legacy IE network) |
| SHELL32 | 8 | shell folder / command line |
| UIAutomationCore | 5 | accessibility |
| OLEAUT32 | 4 (ordinal) | VARIANT / SafeArray |
| WINHTTP | 4 | WinHTTP (modern) |
| RPCRT4 | 3 | RPC client stub |
| VERSION | 3 | file version info |
| ole32 | 2 | COM core (CoInitialize / CoCreateInstance) |
| WINTRUST | 1 | WinVerifyTrust |
| MSIMG32 | 1 | TransparentBlt / AlphaBlend |
| **합계** | **~436** | 실측 (PURPLE / D2R / D4 시 추가 +60 예상 — DXGI / D3D11 / XAudio2 등) |

→ R6 의 실 모수는 **220+** (제목) 보다 **~436** (Battle.net 단일 .exe 기준). PURPLE / D4 / TL 은 +60 ~ +120 추가 가능.

### 0-3. 현재 stub 의 한계 (cycle 79 시점)

```
GetSystemTimeAsFileTime():
    [stub] write_call_log("GetSystemTimeAsFileTime", "")
    return 0   // synthetic
```

**R6 만점 후**:
```
GetSystemTimeAsFileTime() -> i64:
    let unix_us = gettimeofday()        // BSD syscall
    let win_epoch_us = unix_us + 11644473600000000  // 1601→1970 보정
    return win_epoch_us * 10            // FILETIME = 100ns ticks
```

→ 단순 1-line 함수 (Tier 1) 는 즉시 가능. Tier 4~5 는 별도 sub-system 필요.

---

## 1. 5축×5점 채점 (/25 만점)

채점 기준: [REMAINING_ROADMAP.md §"만점 검토 채점 축"](REMAINING_ROADMAP.md).

| 축 | 정의 | R6 점수 | 근거 |
|---|---|---|---|
| **A. own1 정합** | Wine 0줄 유지 가능? | **3 / 5** | Tier 1~3 은 MSDN 만으로 작성 가능. **그러나** SEH / TLS / Critical Section / Heap manager 4영역은 Wine / Reactos source 미참조 시 1년+ 소요 → 강제 own1 → **B / D 가 큰 폭 깎임**. own 1 자체는 유지 가능하나 비용 대칭 손해 |
| **B. 실 동작 도달** | 만점 시 launch 가능? | **4 / 5** | R3 (IAT 실 resolve) 가 prereq. R3 만 끝나면 R6 진척률 = launch 가능률 1:1 매칭. Tier 1~3 만 (60% 함수) 만점화 시 D2R private offline launch 가능률 ~70% 추정. Tier 4~5 미완 시 multi-thread 게임 (D4 / PURPLE / Lineage) launch 차단 |
| **C. 자원 영향** | perf / 메모리 cost | **4 / 5** | 함수 호출당 BSD syscall 1~2회 → perf 영향 무시 수준. 단 handle table 자체가 hash map (현재 hexa stage1 은 TSV — O(N) lookup) → hot loop 에서 ~100us/call 측정시 Holt forecast hot path 우선 hash 재구현 필수 |
| **D. 구현 난이도 (역)** | 낮을수록 점수 높음 | **2 / 5** | 12+ cycle 추정 (Tier 별 split: T1=2cycle, T2=2cycle, T3=2cycle, T4=4cycle, T5=4cycle+). Tier 5 SEH 는 단독으로 Wine 의 1/3 분량 → own1 강제 시 16+ cycle 가능 |
| **E. 의존성 (역)** | 적을수록 점수 높음 | **3 / 5** | R3 (IAT resolve) **필수 prereq** — IAT 가 실 함수 주소를 patch 해야 R6 함수가 실제 호출됨. R2 (PE mmap) 도 prereq. R1 (native bridge) 도 syscall 수단. 즉 R6 = **3 영역 prereq** → 5점 (독립) 이 아닌 3점 |

### 합계

| 축 | A | B | C | D | E | **/25** |
|---|---|---|---|---|---|---|
| 점수 | 3 | 4 | 4 | 2 | 3 | **16 / 25** |

→ **tier**: 15 이상 = sprint 후보 → **R6 sprint 후보 진입 (가까스로)**.
→ **그러나** 본 검토는 단일 R6 score 가 *오해 유발* 함을 명시 — Tier 1~3 만 떼어 sprint 화 시 **22/25 가능** (아래 §3 세부).

---

## 2. 함수 난이도 Tier 분석

### Tier 1 — trivial 1-line (~80 함수)

| 예시 | 매핑 | 비고 |
|---|---|---|
| `GetCurrentThreadId()` | `pthread_mach_thread_np(pthread_self())` | ID 반환만 |
| `GetCurrentProcessId()` | `getpid()` | 동상 |
| `GetTickCount()` | `mach_absolute_time()` × timebase / 1e6 | 32bit ms |
| `GetLastError()` / `SetLastError()` | per-thread `__thread DWORD` | TLS 작은 단위 |
| `lstrlenW()` / `lstrlenA()` | `wcslen` / `strlen` (BSD) | UTF-16 ↔ wchar_t (4byte on macOS!) 변환 주의 |
| `GetCommandLineW()` | `_NSGetArgv()` 후 UTF-16 변환 | 1회 cache |
| `IsDebuggerPresent()` | `sysctl KERN_PROC_PID + P_TRACED` | trivial |
| `Sleep(ms)` | `usleep(ms*1000)` | trivial (그러나 alertable=FALSE 만, alertable Sleep 은 Tier 5) |
| `OutputDebugStringA/W` | `fprintf(stderr, ...)` | trivial |
| `ExitProcess(code)` | `exit(code)` | trivial (단 atexit / DLL detach 순서 미준수) |

**Tier 1 own1 정합**: **5/5** — MSDN spec 만으로 1줄 매핑.
**Tier 1 ETA**: **2 cycle** (~40 함수/cycle 페이스).

### Tier 2 — POSIX 매핑 (~60 함수)

| 예시 | 매핑 | 비고 |
|---|---|---|
| `CreateFileW(name, access, share, sa, disp, attr, tmpl)` | `open(path_a, flags, mode)` | UTF-16→UTF-8 + access flag 매핑 (GENERIC_READ→O_RDONLY 등) |
| `ReadFile(h, buf, n, &got, ovl)` | `read(fd, buf, n)` | overlap=NULL 만, async (OVERLAPPED) 는 Tier 4 |
| `WriteFile(...)` | `write(fd, buf, n)` | 동상 |
| `CloseHandle(h)` | handle table lookup → `close(fd)` | handle manager 필수 (Tier 4 의존) |
| `SetFilePointer(...)` / `SetFilePointerEx` | `lseek(fd, off, whence)` | LARGE_INTEGER split |
| `GetFileSize(h, &hi)` | `fstat(fd, &st)` → `st.st_size` | trivial |
| `DeleteFileW(name)` | `unlink(path_a)` | UTF-16 변환 |
| `CreateDirectoryW` / `RemoveDirectoryW` | `mkdir` / `rmdir` | mode 0755 |
| `FindFirstFileW` / `FindNextFileW` / `FindClose` | `opendir` / `readdir` / `closedir` | WIN32_FIND_DATA struct 채우기 + glob pattern (* / ?) — 자체 구현 |
| `MoveFileW` | `rename(2)` | trivial |
| `GetFileAttributesW` | `stat(2)` → mode 변환 | hidden/system flag 는 macOS 미지원 → 0 반환 |
| `WaitForSingleObject(h, ms)` | (event) `pthread_cond_timedwait` / (proc) `waitpid(WNOHANG)` 폴 | Tier 5 (Critical Section / Event) 와 인접 |

**Tier 2 own1 정합**: **5/5** — POSIX spec 직매핑.
**Tier 2 ETA**: **2 cycle** (~30 함수/cycle).
**Caveat**: handle table = Tier 4 prereq → 실제로는 Tier 4 와 동시 진행.

### Tier 3 — struct marshal (~30 함수)

| 예시 | 매핑 | 비고 |
|---|---|---|
| `GetSystemInfo(LPSYSTEM_INFO)` | `sysctl HW_NCPU + HW_PAGESIZE + HW_MACHINE` | dwOemId / dwPageSize / lpMin/MaxApplicationAddress / dwNumberOfProcessors / dwProcessorType / dwActiveProcessorMask / wProcessorLevel / wProcessorRevision 모두 채움. Apple Silicon → PROCESSOR_ARCHITECTURE_ARM64 매핑 (단 PE 측은 x86_64 emul 모드 — Rosetta 흔적과 정합 필요) |
| `GetSystemTime(LPSYSTEMTIME)` | `gettimeofday` + `gmtime_r` | wYear/wMonth/wDayOfWeek/wDay/wHour/wMinute/wSecond/wMilliseconds 채움 |
| `GetSystemTimeAsFileTime(LPFILETIME)` | `gettimeofday` + 1601 epoch 보정 + ×10 | (위 §0-3 예시) |
| `GetVersionExW(LPOSVERSIONINFOEXW)` | hardcode Win10 19041 (build) — Apple BSD 미반영. `dwMajorVersion=10 / dwMinorVersion=0 / dwBuildNumber=19041` 가 오늘날 안전 fallback (Win11 22000 도 가능) | own1 — Wine 의 win10 정책 참조 X, Microsoft public Win10 SDK doc 만 |
| `GetEnvironmentVariableW` / `SetEnvironmentVariableW` | `getenv` / `setenv` | UTF-16 변환 |
| `GetComputerNameW` | `gethostname(2)` | UTF-16 |
| `GetUserNameW` | `getlogin_r(3)` | 동상 |
| `GetLocalTime` / `SystemTimeToFileTime` / `FileTimeToSystemTime` | 위 패밀리 보정 산수 | trivial 산수 |
| `GlobalMemoryStatusEx` | `sysctl HW_MEMSIZE + vm_statistics64` | ullTotalPhys / ullAvailPhys / ullTotalVirtual 등 |

**Tier 3 own1 정합**: **5/5** — sysctl + struct 채우기, Wine 코드 미참조.
**Tier 3 ETA**: **2 cycle** (~15 함수/cycle, struct 매핑 정확도 검증 필요).

### Tier 4 — heap / handle / async (~30 함수)

| 예시 | 매핑 | 비고 |
|---|---|---|
| `HeapCreate` / `HeapDestroy` / `HeapAlloc` / `HeapFree` / `HeapReAlloc` / `HeapSize` | **별도 heap manager 필요** — `malloc` 직매핑은 정확하지 않음 (Win32 HEAP_ZERO_MEMORY / HEAP_GENERATE_EXCEPTIONS / serialize 옵션). own1 path 는 자체 freelist + arena 작성 (jemalloc/dlmalloc 카피 금지) | ~6 cycle 단독 |
| `VirtualAlloc(addr, size, type, protect)` | `mmap(addr, size, prot, MAP_ANON|MAP_PRIVATE, -1, 0)` + `mprotect` (MEM_COMMIT 분리) | MEM_RESERVE / MEM_COMMIT 2-stage 모사 자체 책임 |
| `VirtualFree` / `VirtualProtect` / `VirtualQuery` | `munmap` / `mprotect` / `mach_vm_region` | `MEMORY_BASIC_INFORMATION` struct 채우기 (Tier 3 도 동시 필요) |
| `CreateFileMapping` / `MapViewOfFile` | `shm_open` + `ftruncate` + `mmap` | named (kernel32 namespace) ↔ posix shm 충돌 — 자체 namespace prefix 필요 |
| `CreateMutex` / `CreateEvent` / `CreateSemaphore` | `pthread_mutex_t` / `pthread_cond_t` + waitable handle | handle 가 thread 간 공유 → handle table mandatory |
| `OVERLAPPED I/O` (`ReadFileEx` 등) | kqueue (`EVFILT_READ`) 기반 자체 IOCP-like 시스템 | own1 path 의 가장 어려운 항목 중 하나, ~6 cycle |
| handle table 자체 | 자체 user-mode handle manager — `kr_handle_table.hexa` 별도 모듈 | hash map 기반 (h32 key, fd/pthread_obj/etc value) |

**Tier 4 own1 정합**: **3/5** — heap manager / handle table 모두 자체 구현 가능, 그러나 OVERLAPPED kqueue 매핑 정밀도가 Wine 의 IOCP 코드 양과 1:1.
**Tier 4 ETA**: **4 cycle** (`heap_arena` + `handle_table` + `vmem_mapper` + `overlapped_kqueue` 4 모듈).

### Tier 5 — SEH / TLS / Critical Section / COM (~20 함수 + 시스템 효과)

| 예시 | 매핑 | 비고 |
|---|---|---|
| `RtlUnwind` / `__try` / `__except` (SEH) | macOS는 SEH 자체 부재. arm64 EH는 Itanium-style table-driven (DWARF / unwind_info). x86_64 PE 의 `.pdata` / `.xdata` 를 parse → arm64 trampoline 으로 redirect. **자체 unwinder 필수** (LLVM `libunwind` import 도 own1 위반 가능 — 단순 referencing 만 허용) | ~6 cycle 단독, 가장 어려움 |
| `TlsAlloc` / `TlsGetValue` / `TlsSetValue` / `TlsFree` | per-thread `fs[0]` (PE 가 fs:[0x18] 로 TEB 접근) — Apple Silicon 은 fs 없음, `pthread_key_create` 매핑 가능. 그러나 PE 내 inlined `mov rax, gs:[N]` 는 patch 필요 (R4 binary translation 영역 침범) | ~3 cycle |
| `InitializeCriticalSection` / `EnterCriticalSection` / `LeaveCriticalSection` / `DeleteCriticalSection` | `pthread_mutex_t` (PTHREAD_MUTEX_RECURSIVE) | 단순 매핑이지만 LOCK_COUNT semantics 정확도 (Win32 = recursive lock) |
| `WaitForMultipleObjects` / `MsgWaitForMultipleObjects` | kqueue 기반 multi-wait — IOCP 와 인접. `WaitForSingleObject` 는 Tier 4 |
| `CoInitialize` / `CoCreateInstance` / COM IUnknown::QueryInterface | **COM 자체** — class registry (HKCR\\CLSID) + apartment threading. 자체 in-process COM minirt 필요. ole32 / oleaut32 함수 4 개 + COM proxy 자체 |

**Tier 5 own1 정합**: **2/5** — SEH unwinder + COM mini-runtime 둘 다 own1 path 에 결정타. SEH 는 `libunwind` reference 를 허용하느냐가 own1 enforcement 기준 변경.
**Tier 5 ETA**: **4+ cycle** (SEH 단독 6, TLS 3, Critical Section 1, COM 4 → 전체 14 cycle 가능 — own1 강제 시 더 큼).

---

## 3. 함수별 split scoring (R6 의 실 가치)

R6 단일 점수 (16/25) 는 오해 유발 — Tier 별 split:

| Tier | 함수 수 | A | B | C | D | E | /25 | sprint? |
|---|---|---|---|---|---|---|---|---|
| **Tier 1** (trivial 1-line) | ~80 | 5 | 4 | 5 | 5 | 3 | **22** | ⭐ **즉시** |
| **Tier 2** (POSIX 매핑) | ~60 | 5 | 4 | 5 | 4 | 3 | **21** | ⭐ **Tier 4 handle 후** |
| **Tier 3** (struct marshal) | ~30 | 5 | 4 | 5 | 4 | 3 | **21** | ⭐ Tier 1 직후 |
| **Tier 4** (heap/handle/async) | ~30 | 4 | 5 | 4 | 2 | 2 | **17** | sprint 후보 |
| **Tier 5** (SEH/TLS/CS/COM) | ~20 | 2 | 5 | 3 | 1 | 2 | **13** | **후순위** (만점 미달) |
| **합계 weighted avg** | ~220 | 3 | 4 | 4 | 2 | 3 | **16** | (위 §1과 일치) |

→ **R6 의 실 sprint 가치**: Tier 1+3 (~110 함수, **22+21=43/50**) 만 떼어 cycle 80~83 sprint 가능.
→ Tier 5 (13/25) 는 폐기 후보 — **own1 강제 시 Lineage 2 / D2R / D4 PE 의 SEH-heavy 영역 차단** 감수 (대안: Rosetta 2 internal SEH 동작 활용 — 그러나 own1 의 "Apple-only" 정합성 검증 필요).

---

## 4. 핵심 blocker 후보

### 4-1. Apple BSD 비대칭 함수

| Win32 | Apple BSD 비대칭 지점 | 우회 |
|---|---|---|
| `GetTickCount64()` | Win32 = boot 후 64bit ms. Apple = `mach_absolute_time` 은 boot 후이지만 timebase 변환 필요 + 32bit unsigned overflow 49.7일에서 reset (GetTickCount32 와 동일 hazard). **64bit 는 mach 가 long long → 정확** | mach_continuous_time (sleep 포함) 사용 |
| `QueryPerformanceCounter` / `QueryPerformanceFrequency` | Win32 = HPET / TSC. Apple = `mach_absolute_time` + `mach_timebase_info` | trivial |
| `GetProcessTimes` (CreationTime, ExitTime, KernelTime, UserTime) | macOS = `getrusage` + `proc_pidinfo` | UserTime / KernelTime 매핑 단순. CreationTime 은 `proc_pidinfo PROC_PIDTASKINFO` 의 `pti_total_user/system` 와 분리 |
| `GetSystemTimePreciseAsFileTime` | Win10+ 1us 정확도. Apple `gettimeofday` 도 1us → trivial | trivial |
| Per-thread CPU usage (`GetThreadTimes`) | Apple = `thread_info(THREAD_BASIC_INFO)` | 매핑 가능, struct 채우기 |

→ blocker 라기보다 **표면적 비대칭** — 모두 우회 가능.

### 4-2. Registry (HKEY_*)

| 항목 | 비고 |
|---|---|
| 함수 13개 (RegOpenKeyEx / RegQueryValueEx / RegSetValueEx / RegEnumKey / RegDeleteKey 등) | ADVAPI32 의 ~25% |
| 자체 hive emulation 필요 | cycle 72 `pe_registry_hive_load` skeleton + cycle 72 `pe_registry_transactional` skeleton 존재 — 실 구현 미진 |
| HKLM (HKEY_LOCAL_MACHINE) — system-wide | 자체 backing file (`~/.airgenome/gamebox/persist/registry/HKLM.tsv`) |
| HKCU (HKEY_CURRENT_USER) — per-user | `~/.airgenome/gamebox/persist/registry/HKCU.tsv` |
| HKCR (HKEY_CLASSES_ROOT) — COM CLSID merge view | HKLM\Software\Classes ∪ HKCU\Software\Classes |
| transactional (`RegCreateKeyTransacted`) | TXF 모사 — TSV append-only journal + commit/rollback |
| ETA | 2 cycle (skeleton → 실 동작화) |

### 4-3. Security Descriptor / ACL (cycle 74 `pe_acl_descriptor` skeleton)

| 항목 | 비고 |
|---|---|
| `GetFileSecurity` / `SetFileSecurity` / `IsUserAnAdmin` / `OpenProcessToken` / `GetTokenInformation` | ADVAPI32 의 ~30% |
| ACL 자체 — Win32 SID + ACE list | macOS `acl_t` (POSIX.1e draft) 와 비대칭 — 1:1 매핑 X. 자체 SID arena + ACE list 자체 구현 |
| 단순화 옵션 | 모든 query 에 "Admin / Allow All" return — Lineage / D2R / D4 모두 client-side ACL 검증 미진행 → 단순화 가능 |
| ETA | 1 cycle (단순화), 3 cycle (정밀) |

### 4-4. Handle Table

| 항목 | 비고 |
|---|---|
| user-mode handle manager | 핵심 — Tier 4 의 대부분이 의존 |
| handle = u32 (Win32 = void*, 64bit OS 도 32bit 사용 권장 — `HandleSpace` doc) | hash map: u32 → struct {kind, fd/pthread_obj/regkey/etc} |
| kind enum | FILE / DIR / EVENT / MUTEX / SEMAPHORE / THREAD / PROCESS / TOKEN / REGKEY / FILEMAP / NAMEDPIPE / SOCKET / WND / DC / GDI |
| 동시 thread-safe | RW lock 또는 lock-free trie (own1 path = 자체 trie, 기존 `r0_trie` 모듈 활용 가능) |
| ETA | 1 cycle (단독) |

---

## 5. 의존성 / 진행 순서

```
R1 (stage7 native bridge) — dlopen/mmap/pthread 실 호출
    └── R6 prereq: BSD syscall 수단 확보

R2 (PE 실 mmap) — .text/.data 적재
    └── R6 prereq: import 함수가 patch 될 area 확보

R3 (IAT 실 resolve) — 실 함수 주소 patch
    └── R6 = 이 patch 의 *target* (R3 가 R6 함수의 주소를 IAT 에 채움)

R6-Tier1 (trivial 80 함수)        ← R3 직후 즉시 가능
R6-Tier3 (struct marshal 30)      ← Tier1 직후
R6-Tier4-handle (handle table)    ← Tier3 와 병렬
R6-Tier2 (POSIX 60)               ← handle table 후
R6-Tier4-나머지 (heap/vmem/async) ← handle table 후
R6-Tier5-Critical Section (5)     ← handle table 후
R6-Tier5-TLS                      ← R4 (binary translation) 와 약결합
R6-Tier5-SEH                      ← R4 (binary translation) prereq + 자체 unwinder
R6-Tier5-COM                      ← Registry 실 동작 prereq
```

→ R6 의 실 sprint 진입 trigger = **R3 완료**.
→ Tier1+3 만 단독 가능 (R3 후 1 cycle), Tier4-handle 까지 추가 시 Tier2 도 즉시 가능 (R3 후 2 cycle).

---

## 6. ETA / cycle 페이스

| sub-영역 | 함수 수 | ETA cycle | bg 병렬 가능? |
|---|---|---|---|
| Tier 1 (1-line) | ~80 | 2 | ✓ 4 bg agent (DLL 별 split) |
| Tier 3 (struct marshal) | ~30 | 2 | ✓ 2 bg agent |
| handle table 자체 | (인프라) | 1 | △ 단독 |
| Tier 2 (POSIX) | ~60 | 2 | ✓ 4 bg agent |
| heap manager | (인프라) | 4 | △ 단독 (own1 freelist+arena) |
| vmem mapper (VirtualAlloc) | ~6 | 1 | △ |
| overlapped/IOCP (kqueue) | ~5 | 4 | △ |
| Tier 5 — Critical Section | ~5 | 1 | ✓ |
| Tier 5 — TLS | ~5 | 3 | △ R4 의존 |
| Tier 5 — SEH | (인프라) | 6 | ✗ 단독, 가장 어려움 |
| Tier 5 — COM mini-runtime | ~10 | 4 | △ Registry 의존 |
| **합계 (own1 강제)** | ~220 | **30 cycle** | bg 활용시 ~18 cycle 압축 가능 |

→ REMAINING_ROADMAP.md §"ETA / cycle 수 estimate" 의 R6 = **12+ cycle** estimate 는 **Tier 1~3 + handle 일부만** 반영 — 전체 own1 R6 은 **30 cycle**.

---

## 7. 추천 모듈 candidate (CN 미정 — cycle 80+ 진입 후보)

| # | candidate | tier | 함수 수 | A | B | C | D | E | /25 | 비고 |
|---|---|---|---|---|---|---|---|---|---|---|
| 1 | `c_win32_tier1_trivial_real` | 1 | ~80 | 5 | 4 | 5 | 5 | 3 | **22** ⭐ | DLL × 4 batch 분할 가능 |
| 2 | `c_win32_tier3_struct_marshal_real` | 3 | ~30 | 5 | 4 | 5 | 4 | 3 | **21** ⭐ | sysctl + struct 채우기 |
| 3 | `c_win32_handle_table` | 4-infra | (infra) | 5 | 5 | 4 | 4 | 4 | **22** ⭐ | trie 기반, own1 정합 ◎ |
| 4 | `c_win32_tier2_posix_real` | 2 | ~60 | 5 | 4 | 5 | 4 | 3 | **21** ⭐ | handle table 후 |
| 5 | `c_win32_heap_arena_own1` | 4-infra | (infra) | 4 | 5 | 4 | 2 | 3 | **18** | own1 freelist+arena |
| 6 | `c_win32_vmem_mapper` | 4 | ~6 | 5 | 5 | 5 | 4 | 3 | **22** ⭐ | mmap 직매핑 |
| 7 | `c_win32_overlapped_kqueue` | 4 | ~5 | 4 | 5 | 4 | 2 | 2 | **17** | kqueue IOCP-like |
| 8 | `c_win32_critical_section` | 5 | ~5 | 5 | 4 | 5 | 5 | 3 | **22** ⭐ | pthread recursive mutex |
| 9 | `c_win32_tls_per_thread` | 5 | ~5 | 4 | 5 | 4 | 3 | 2 | **18** | R4 약의존 |
| 10 | `c_win32_seh_unwinder_own1` | 5 | (infra) | 2 | 5 | 3 | 1 | 2 | **13** | **만점 미달** — Rosetta SEH 활용 검토 |
| 11 | `c_win32_com_mini_runtime` | 5 | ~10 | 3 | 5 | 4 | 2 | 2 | **16** | Registry 의존 |
| 12 | `c_win32_registry_hive_real` | (infra) | ~13 | 5 | 5 | 5 | 4 | 4 | **23** ⭐ | cycle 72 skeleton 활용 |
| 13 | `c_win32_acl_simplified_admin_allow` | (infra) | ~8 | 5 | 4 | 5 | 5 | 5 | **24** ⭐ | "Admin / Allow All" 단순화 |

→ **/25 만점 ≥ 20** 후보 = **8 개** (1, 2, 3, 4, 6, 8, 12, 13).
→ **만점 미달 (<15)** = 1 개 (10 SEH unwinder) — 별도 검토 후 own1 mandate vs Rosetta SEH 활용 trade-off 결정.

---

## 8. 결론

### 8-1. 종합 점수

| 항목 | 값 |
|---|---|
| **R6 단일 점수** | **16 / 25** ("후순위" 경계 — 15 이상 sprint 자격 가까스로 충족) |
| **R6 split 점수 (Tier 1~3)** | **22 / 25 + 21 / 25 + 21 / 25** (모두 sprint 후보) |
| **R6 split 점수 (Tier 4 일부 — handle/vmem/CS)** | **22 / 25 + 22 / 25 + 22 / 25** (sprint 후보) |
| **R6 split 점수 (Tier 5 SEH)** | **13 / 25** (만점 미달, 별도 검토) |
| **own1 정합** | **유지 가능 — 단 Tier 5 SEH 만 trade-off 결정 필요** |
| **실 launch 가능률** | Tier 1~3 + handle 만점화 시 D2R private (offline, single-thread heavy 영역 회피) launch ~70% — D4 / Lineage 2 / PURPLE 은 Tier 4 OVERLAPPED + Tier 5 TLS/CS 추가 필요 |
| **ETA (own1 강제 전체)** | **30 cycle** (bg 활용시 18 cycle 압축) |
| **ETA (Tier 1~3 sprint 한정)** | **6 cycle** (handle table 포함) |

### 8-2. 권장 진행 순서 (R3 완료 후)

```
1. cycle 80      handle table 인프라 (candidate 3, 22/25)
2. cycle 81      Tier 1 trivial × 4 bg parallel (candidate 1, 22/25)
3. cycle 82      Tier 3 struct marshal (candidate 2, 21/25)
4. cycle 83      Tier 2 POSIX (candidate 4, 21/25)
5. cycle 84      vmem mapper + Critical Section (candidate 6, 8 — 22/25 양쪽)
6. cycle 85-86   Registry hive 실 동작 (candidate 12, 23/25) + ACL 단순화 (candidate 13, 24/25)
7. cycle 87+     heap manager (18/25, Tier 4-infra) + overlapped/IOCP (17/25)
8. cycle 90+     TLS (18/25), COM mini-rt (16/25)
9. 별도 검토      SEH (13/25) — Rosetta 활용 결정 후 진행
```

→ **즉시 권장 action**: cycle 80 sprint = candidate 3 (handle table) → cycle 81 sprint = candidate 1 (Tier 1 trivial 80 함수) bg 병렬.
→ candidate 13 (ACL 단순화 24/25) 은 cycle 80 과 병렬 가능 (handle 의존 X).

### 8-3. own 1 mandate 재확인

본 검토 진행 동안 enforcement:

- Wine `dlls/kernel32/file.c` / `dlls/kernel32/process.c` / `dlls/ntdll/heap.c` 등 **0줄 import / copy / port 금지**.
- Reactos `reactos/dll/win32/kernel32/client/file/lfile.c` 등 **0줄 import / copy / port 금지**.
- 함수 의미 / 매개변수 형식 / 반환 의미는 **MSDN `learn.microsoft.com/en-us/windows/win32/api/...`** 만 참조.
- 단순 referencing (Wine source 가 어떻게 했는지 *읽기만*) 도 own1 path 에서는 회색 — 본 검토는 "MSDN spec 만 참조" mandate.
- Rosetta 2 internal SEH / fs[0] 지원 활용은 own1 정합 (Apple stack 자체이므로) — 단 "Apple-only" 의미는 유지됨.

---

## 9. cross-link

- [REMAINING_ROADMAP.md §R6](REMAINING_ROADMAP.md) — 본 검토의 SSOT entry
- [SCORING.md](SCORING.md) — 5축×5점 채점 정의
- [PURPLE_LINEAGE_MANJEOM_REVIEW.md](PURPLE_LINEAGE_MANJEOM_REVIEW.md) — 형식 모형 (5축 + Tier split)
- [DELTA_FORCE_MANJEOM_REVIEW.md](DELTA_FORCE_MANJEOM_REVIEW.md) — 차단 사례 (own1 + kernel AC)
- `lib/loader/win32_kernel32_stage{1..5}.hexa` — 현재 stub layered 구현
- `lib/loader/win32_user32_stage{1..2}.hexa` — USER32 stub
- `lib/loader/win32_advapi32_stage2.hexa` 외 — 나머지 DLL stub
- 향후 개별 모듈: `lib/loader/win32_kernel32_real_tier1.hexa` (가칭 — candidate 1)

---

*written 2026-05-01 cycle 79 직후. R6 = 220+ DLL stub 함수 실 구현 만점 검토. 단일 점수 16/25 (후순위 경계), 그러나 Tier split 시 8개 candidate 가 20/25 이상 sprint 후보. own1 mandate 유지 가능 — Tier 5 SEH 만 별도 trade-off. 즉시 권장: cycle 80 handle table → cycle 81+ Tier 1~3 bg 병렬 sprint.*
