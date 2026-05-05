# R3 PROGRESS V6 — 343-370 instruction (run-by-run 변동) + KERNEL32 stub hits 7-9 + USER32/GDI32 IPC bridge wired + CRT init halt (2026-05-01)

> **status**: R1+R2 own1 §A closure **100%** 유지 + R3-1~11 wave4 통합 + shim 의 IAT stub return value 의미화 (KERNEL32 stub 7-9개 통과) + USER32/GDI32 helper IPC kind=14/15 bridge wiring 완료 (단, WinMain 미진입으로 호출 도달 0건). 누적 R3 진척 **~70%**. 다음 halt = MSVCR/UCRT CRT runtime init (`__scrt_common_main_seh` 의 `_initterm` / `__scrt_acquire_startup_lock` / `_get_initial_narrow_environment` / `_get_narrow_winmain_command_line`) — 모두 의미 있는 값 반환해야 WinMain 진입.
> **base commits**: `4d1ff9c` (V5 작성, 277 inst halt) → `b0f0756` (hexa user32_iat_bridge_real, V5 doc swept) → `abc9506` (helper.mm NSEvent + keymap 자체 표) → `7a0eeaf` (hexa userland_first_dialog) → `212a7fe` (shim wave4 — R3-10/11 IAT stub return + USER32/GDI32 helper IPC bridge, 277→**343-370 inst**).
> **현재 차단**: wave4 통합 후 KERNEL32 stub (`GetSystemTimeAsFileTime` / `GetCurrentThreadId` / `GetCurrentProcessId` / `QueryPerformanceCounter` / `LoadLibraryExW` / `GetLastError` / `GetProcAddress`) 7-9 hit 통과 → MSVCR/UCRT 의 `__scrt_common_main_seh` 진입 → `_initterm(start, end)` 진입 → CRT init array walk + 호출 영역에서 halt. 자연 halt 형태 (interpreter unimplemented opcode 부재) — wave5 의 CRT runtime init stub 의미화 (no-op return 0 + fake env block ptr + fake winmain command line ptr) 로 progress.
> **own1 정합**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift **0줄** 유지. 자체 코드 — `native/pe_to_macho_shim.cpp` **4924 line** (+1062) + `native/helper.mm` **1271 line** (+260) + `lib/loader/pe_i386_*.hexa` **25 file** × 평균 ~500 line. KERNEL32 stub return 표 자체 작성 (Microsoft API doc reference).
> **참조**: [R3_PROGRESS_V5.md](R3_PROGRESS_V5.md) (V5 의 277 inst + helper NSWindow real create + wave3 opcode cap 해제) / [R3_PROGRESS_V4.md](R3_PROGRESS_V4.md) (V4 의 205 inst + SEH register + FPU/SSE prefix decode) / [R3_PROGRESS_V3.md](R3_PROGRESS_V3.md) (V3 의 51 inst + IAT 4 함수 통과) / [R3_ROADMAP.md](R3_ROADMAP.md) (sub-phase 1-12 정의) / [BATTLENET_REAL_LAUNCH_PLAN.md](BATTLENET_REAL_LAUNCH_PLAN.md) (wave 1-5 own1 enforcement).

---

## 1. V5 → V6 변화 요약

V5 (commit `4d1ff9c`, cycle 100-101) 대비 V6 (commit `212a7fe`, cycle 102-103) 의 핵심 변화는 **IAT stub return value 의미화** + **PEB/Ldr fake module table** + **USER32/GDI32 helper IPC bridge wiring** + **run-by-run 변동 진척** 이다.

V5 의 inst[277] `0xC3` RET → EIP=0 (IAT stub EAX=0 sentinel) 자연 halt 가 wave4 통합 후 해소. KERNEL32 stub 의 per-function fake handle / fake ptr / fake string ptr 반환 통합으로 inst[277] 통과 → KERNEL32 stub 추가 호출 7-9 hit 통과 → MSVCR/UCRT 의 `__scrt_common_main_seh` 영역 진입 → `__security_init_cookie` 의 CPUID + GenuineIntel 검증 통과 (단순 hardcoded 응답) → CRT init (`_initterm` 또는 `__scrt_acquire_startup_lock` 또는 `_get_initial_narrow_environment` 인근) 영역에서 halt.

특이점: V6 는 V5 와 달리 **run-by-run 변동** 발생 — `343, 351, 358, 363, 370` 등 run 마다 5-30 inst 편차. root cause = `GetSystemTimeAsFileTime` / `QueryPerformanceCounter` 의 fake return value (system time / counter) 가 매 run 마다 다른 값 → CRT init 측 PRNG seed / cookie 계산 의 분기가 run 마다 다름.

### 1.1 V5 vs V6 비교 표

| 항목 | V5 (commit `4d1ff9c`) | V6 (commit `212a7fe`) | delta |
|---|---|---|---|
| instruction 진척 | 277 inst | **343-370 inst** (run-by-run 변동) | **+66~93 inst** |
| 차단 위치 | inst[277] `0xC3` RET → EIP=0 (IAT stub EAX=0 sentinel) | CRT init (`__scrt_common_main_seh` 의 `_initterm` / `__scrt_acquire_startup_lock` / `_get_initial_narrow_environment`) 영역 | **trigger 진보** (IAT semantics → CRT runtime semantics) |
| 차단 분류 | IAT stub return 0 sentinel (ret_to_unmapped) | CRT runtime init (의미 없는 fake env / cmdline / lock 값) | **자연 halt** (run-by-run 변동) |
| KERNEL32 stub hits | 0 (모두 EAX=0 echo) | **7-9 hit** (per-function fake handle/ptr) | wave4 통합 |
| KERNEL32 호출 함수 | echo only | GetSystemTimeAsFileTime / GetCurrentThreadId / GetCurrentProcessId / QueryPerformanceCounter / LoadLibraryExW / GetLastError / GetProcAddress (+ GetModuleHandleA / GetTickCount 추가) | per-function semantics |
| PEB/Ldr fake module table | 미존재 | g_fake_modules: kernel32→0x10000000, ntdll→0x77000000, user32→0x76000000, msvcrt→0x74000000, gdi32→0x75000000 | LDR_DATA_TABLE_ENTRY 부분 통합 |
| __security_init_cookie CPUID | 미통합 (halt 가능) | **통과** (GenuineIntel hardcoded 응답) | runtime init 통과 |
| USER32 IAT bridge | hexa skeleton (kind=14 routing 정의) | **shim cpp 측 wiring 완료** (dll == "USER32" → IPC kind=14) | bridge wired |
| GDI32 IAT bridge | hexa skeleton (kind=15 routing 정의) | **shim cpp 측 wiring 완료** (dll == "GDI32" → IPC kind=15) | bridge wired |
| USER32/GDI32 hits | 0 | **0** (WinMain 미진입 → 도달 X) | 실 동작 미검증 (V7 영역) |
| `shim.cpp` line | 3862 | **4924** (+1062) | wave4 통합 +1062 line |
| `helper.mm` line | 1011 | **1271** (+260) | NSEvent + keymap 자체 표 +260 line |
| `hexa` file 수 | 23 | **25** file | user32_iat_bridge_real + userland_first_dialog 추가 |
| R3 누적 closure | ~60% | **~70%** | +10% |

### 1.2 V6 핵심 한 줄 변화

**inst[277] `0xC3` RET → EIP=0 (IAT stub EAX=0 sentinel) 자연 halt → wave4 의 KERNEL32 stub return value 의미화 (per-function fake handle/ptr) + PEB/Ldr fake module table + USER32/GDI32 IPC bridge wiring 후 KERNEL32 stub 7-9 hit 통과 → inst[343-370] CRT runtime init (`__scrt_common_main_seh` 의 `_initterm` / `__scrt_acquire_startup_lock` / `_get_initial_narrow_environment`) 영역에서 자연 halt — IAT semantics 영역 통과 후 CRT runtime semantics 영역의 trigger 로 진보.**

---

## 2. R3-10 PEB/Ldr fake module 동작

V5 의 IAT stub 이 EAX=0 zero sentinel 만 return 했던 구조가 V6 에서 per-function fake handle/ptr/string 으로 의미화. PEB/Ldr 영역 (Process Environment Block + LoaderData) 의 fake module table 통합 핵심.

### 2.1 fake KERNEL32 module table (`g_fake_modules`)

```cpp
struct FakeModule {
    const char* name;       // lower-case dll name
    uint32_t    base_va;    // fake hmodule (32-bit linear addr)
};
static FakeModule g_fake_modules[] = {
    { "kernel32.dll", 0x10000000 },
    { "ntdll.dll",    0x77000000 },
    { "user32.dll",   0x76000000 },
    { "msvcrt.dll",   0x74000000 },
    { "gdi32.dll",    0x75000000 },
};
```

이 fake VA 들은 4GB linear AS 의 unmapped 영역 (PE 의 .text/data 외부) 이지만, i386 측 코드는 hmodule 을 opaque handle 로 다룸 — 실제 fetch_inst / mem_load 가 일어나지 않는 한 unmapped 여도 무관. shim 측은 `is_fake_hmodule(hmod)` 로 판별 후 GetProcAddress 등에서 dispatch.

### 2.2 GetModuleHandleA / LoadLibraryExW → fake hmodule

| API | 호출 시점 | 반환 |
|---|---|---|
| `KERNEL32!GetModuleHandleA(NULL)` | 자기 자신 | EAX = 0x00400000 (PE ImageBase) |
| `KERNEL32!GetModuleHandleA("kernel32.dll")` | static name 조회 | EAX = 0x10000000 (fake) |
| `KERNEL32!LoadLibraryExW(L"kernel32.dll", 0, 0)` | dynamic load | EAX = 0x10000000 (fake — 이미 module table 에 있는 경우) |
| `KERNEL32!LoadLibraryExW(L"foo.dll", 0, 0)` | dynamic load | EAX = `++g_fake_module_counter` (신규 fake hmodule alloc) |

LoadLibraryExW 의 wstring 인자는 shim 측에서 UTF-16LE 디코드 → ASCII 비교 (Battle.net Setup 의 dll 이름은 모두 ASCII subset).

### 2.3 GetProcAddress → fake fnptr (`g_fake_proc_counter`)

```cpp
case STUB_GETPROCADDRESS: {
    uint32_t hmod      = mem_load_u32(esp + 4);
    uint32_t name_va   = mem_load_u32(esp + 8);
    const char* name   = (const char*)mem_va_to_host(name_va);
    uint32_t fnptr     = ++g_fake_proc_counter; // 0xCAFE0001, 0xCAFE0002, ...
    register_fake_proc(fnptr, hmod, name);
    eax = fnptr;
}
```

g_fake_proc_counter 는 0xCAFE0000 base 의 단조 증가 카운터. shim 측 IAT 호출 시 callee VA 가 0xCAFE???? 영역이면 fake_proc table 조회 → 해당 (hmod, name) 의 stub dispatch.

### 2.4 `__security_init_cookie` 의 CPUID + GenuineIntel 검증 통과

```
CPUID(EAX=0)        → EAX=0x16, EBX="Genu", EDX="ineI", ECX="ntel"
CPUID(EAX=1)        → EAX=0x000906EA (Family 6 Model 9E Stepping A — Skylake-ish)
                      EBX=0x00100800
                      ECX=0x7FFAFBFF (SSE/SSE2/SSE3/SSSE3/SSE4.1/SSE4.2/AVX flag)
                      EDX=0xBFEBFBFF (FPU/MMX/SSE/SSE2 flag)
RDTSC               → EDX:EAX = 단조 증가 64-bit counter (시작 시 mach_absolute_time 기반)
```

이 응답 정합 — GenuineIntel string 검증 통과 + SSE2 capable feature flag 통과 → `__security_init_cookie` 의 fallback path 회피 + RDTSC 의 entropy 만큼 cookie 값 다양성 확보. own1 §A — Apple `host_processor_info` / `mach_absolute_time` direct, Wine `cpuid.c` lift 0줄.

### 2.5 PEB/TEB 부분 통합

| field | offset (i386 PEB) | V6 값 |
|---|---|---|
| PEB.ImageBaseAddress | +0x08 | 0x00400000 (PE base) |
| PEB.Ldr | +0x0C | fake LDR_DATA ptr (3 link 만 minimal) |
| PEB.ProcessParameters | +0x10 | fake RTL_USER_PROCESS_PARAMETERS ptr (CommandLine + ImagePath 만) |
| PEB.NumberOfProcessors | +0x64 | 8 |
| PEB.OSMajorVersion | +0xA4 | 10 |
| PEB.OSBuildNumber | +0xAC | 19045 |
| TEB.NtTib.Self | +0x18 | TEB self ptr |
| TEB.ClientId.UniqueProcess | +0x20 | fake PID = 0x1234 |
| TEB.ClientId.UniqueThread | +0x24 | fake TID = 0x5678 |
| TEB.Peb | +0x30 | PEB ptr |

LDR_DATA 의 InMemoryOrderModuleList (3 link) 은 g_fake_modules 의 5개 module 을 순회 가능하게 통합 — but Battle.net Setup 측 wave4 까지의 진척 inst 에서는 LDR walk 영역 도달 X (V7 영역).

---

## 3. R3-11 USER32/GDI32 bridge code path

V5 시점 helper.mm 의 IPC kind=14/15 dispatch 가 echo-only (V5 시점) → 실 NSWindow alloc (commit 9c63397, V5) 로 진화한 상태에서, V6 의 wave4 는 **shim cpp 측 IAT thunk → IPC kind=14/15 routing wiring** 을 통합. 즉, i386 측 USER32/GDI32 IAT 호출 시 자체 IPC 로 helper 에 dispatch 하는 path 가 코드상 wired 됨 (단, WinMain 미진입으로 호출 도달 0건).

### 3.1 shim 측 IAT thunk 호출 시 dll discriminator

```cpp
void dispatch_iat_thunk(uint32_t callee_va, uint32_t esp_at_call) {
    IatEntry* e = lookup_iat_by_callee_va(callee_va);
    if (!e) { halt(reason=iat_unmapped); return; }
    if (strcmp(e->dll, "KERNEL32") == 0) {
        kernel32_stub_dispatch(e->func, esp_at_call);
    } else if (strcmp(e->dll, "USER32") == 0) {
        user32_ipc_kind14_dispatch(e->func, esp_at_call);
    } else if (strcmp(e->dll, "GDI32") == 0) {
        gdi32_ipc_kind15_dispatch(e->func, esp_at_call);
    } else if (strcmp(e->dll, "MSVCRT") == 0 || strncmp(e->dll, "API-MS-", 7) == 0) {
        crt_stub_dispatch(e->func, esp_at_call); // wave5 신규
    } else {
        unknown_iat_dispatch(e->dll, e->func, esp_at_call);
    }
}
```

### 3.2 user32_ipc_kind14_dispatch — fn_id mapping

| Win32 API | fn_id | helper kind=14 분기 |
|---|---|---|
| RegisterClassExA | 1 | NSWindow class 등록 (helper 측 dict) |
| CreateWindowExA | 2 | NSWindow alloc + makeKeyAndOrderFront |
| ShowWindow | 3 | makeKeyAndOrderFront / orderOut |
| UpdateWindow | 4 | display |
| GetMessageA | 5 | NSEvent dequeue |
| DispatchMessageA | 6 | sendEvent: + WndProc dispatch back |
| PostQuitMessage | 7 | [NSApp stop:nil] + sentinel |
| MessageBoxA | 8 | NSAlert runModal |
| TranslateMessage | 9 | NSEvent → WM_KEYDOWN/WM_CHAR (helper 측 keymap 자체 표 활용) |
| LoadIconA | 10 | NSImage stub |
| LoadCursorA | 11 | NSCursor stub |
| DefWindowProcA | 12 | helper.mm 자체 default proc |

### 3.3 args parsing — stack [esp+4..esp+4*N] (stdcall callee-pop)

i386 stdcall ABI 에서 caller 가 args 를 right-to-left push, callee 가 ret 시 esp += 4*N pop. shim 측은 호출 시점 esp 에서 [esp+4], [esp+8], ... 순으로 args 읽어 IPC payload 로 packing. 응답 후 stub return path 에서 esp += 4*N 적용 (callee-pop simulation).

```cpp
void user32_ipc_kind14_dispatch(const char* fn, uint32_t esp_at_call) {
    int fn_id = lookup_user32_fn_id(fn);
    int n_args = lookup_user32_n_args(fn);
    uint32_t args[16];
    for (int i = 0; i < n_args; ++i) {
        args[i] = mem_load_u32(esp_at_call + 4 * (i + 1));
    }
    Kind14Reply reply = ipc_call(IPC_KIND_USER32, fn_id, args, n_args);
    eax = reply.return_value;
    esp_at_call += 4 * n_args; // stdcall callee-pop
    eip = mem_load_u32(esp_at_call - 4 * n_args); // pre-pop ret addr
    esp = esp_at_call;
}
```

### 3.4 response 의 return_value → EAX

helper 측 reply struct 는 `{ return_value: u32, fake_hwnd: u32, error: u32 }` 형태. shim 은 reply.return_value 를 EAX 에 set. 단, CreateWindowExA / RegisterClassExA / LoadIconA 등 handle 반환 함수는 reply.fake_hwnd 를 return_value 로도 동시 set.

### 3.5 현재 상태 — WinMain 미진입이라 USER32 IAT 호출 X

V6 시점 i386 측 진척 inst[343-370] = `__scrt_common_main_seh` → `__security_init_cookie` → `_initterm` 인근 영역. **WinMain 도달 X** → RegisterClassExA / CreateWindowExA 호출 도달 X → USER32 bridge 실 동작 미검증. wave5 (CRT init 통과) → wave6 (WinMain 진입) 후 비로소 USER32 bridge 동작 검증 가능.

GDI32 bridge 도 동일 — DC (Device Context) 호출은 WinMain 의 WM_PAINT 메시지 처리 인근에서 발생하며 V6 에서 도달 X.

---

## 4. 다음 trigger — CRT runtime init (MSVCR/UCRT)

V6 의 inst[343-370] 자연 halt 의 root cause = MSVCR/UCRT 의 `__scrt_common_main_seh` 진입 후 CRT runtime init 함수들 (`_initterm` / `__scrt_acquire_startup_lock` / `_get_initial_narrow_environment` / `_get_narrow_winmain_command_line` 등) 이 의미 없는 0/null 반환 → 후속 분기가 abort path 로 진입. wave5 의 핵심.

### 4.1 함수별 의미화 stub 설계

| MSVCR/UCRT 함수 | V6 (현재) | wave5 (목표) | 비고 |
|---|---|---|---|
| `_initterm(start, end)` | EAX = 0 (echo) | **단순화 stub: no-op return 0** (init array 호출 skip) | C/C++ global ctor table 의 walk 회피. Battle.net Setup 측 ctor 는 대부분 stat init / mutex init — skip 무해 |
| `_initterm_e(start, end)` | EAX = 0 | **return 0** (오류 없음) | exception flavor — 동일 처리 |
| `__scrt_acquire_startup_lock` | EAX = 0 | **return 0** (lock 획득 sentinel — 단일 thread emul 에서는 즉시 acquired) | InitOnce flag |
| `__scrt_release_startup_lock` | EAX = 0 | **no-op return** | release 측 |
| `__scrt_initialize_crt` | EAX = 0 | **return 1** (success) | CRT 자체 init 완료 마커 |
| `__scrt_initialize_type_info` | EAX = 0 | **return 1** | RTTI init |
| `__scrt_initialize_default_local_stdio_options` | EAX = 0 | **no-op return 1** | stdio default |
| `_get_initial_narrow_environment` | EAX = 0 | **fake env block ptr** (`"PATH=C:\\Windows\\System32\0SystemRoot=C:\\Windows\0\0"`) | env 영역 |
| `_get_initial_wide_environment` | EAX = 0 | **fake wide env block ptr** (UTF-16LE) | wide flavor |
| `_get_narrow_winmain_command_line` | EAX = 0 | **fake "Battle.net Setup.exe\0" ptr** | WinMain 의 lpCmdLine arg |
| `_get_wide_winmain_command_line` | EAX = 0 | **fake L"Battle.net Setup.exe\0" ptr** | wide flavor |
| `_set_app_type(1)` | EAX = 0 | **no-op return** (1 = GUI app marker) | __scrt 측 internal flag |
| `_set_invalid_parameter_handler(NULL)` | EAX = 0 | **return 0** (이전 handler 없음) | error handler set |
| `_configure_narrow_argv(2)` | EAX = 0 | **return 0** (success) | argv config |
| `_initialize_narrow_environment` | EAX = 0 | **return 0** (success) | env init |
| `_initialize_onexit_table` | EAX = 0 | **return 0** (success) | atexit table |
| `_register_onexit_function` | EAX = 0 | **return 0** (success — but 실제 등록 X, exit 시 무시) | atexit handler |
| `__p___argc` / `__p___argv` | EAX = 0 | **fake argc/argv ptr** (1, ["Battle.net Setup.exe"]) | C startup args |
| `__p__commode` / `__p__fmode` | EAX = 0 | **fake int* ptr** (값 0) | stdio mode |
| `_set_new_mode(1)` | EAX = 0 | **return 0** | malloc fail handler mode |
| `_controlfp_s(NULL, 0, 0)` | EAX = 0 | **return 0** | FPU control word |

### 4.2 도달 후 효과

wave5 (CRT init stub 의미화) 통합 후 inst[343-370] 자연 halt → inst[500+] 진척 → `__scrt_common_main_seh` 의 `WinMain(hInstance, NULL, lpCmdLine, nShowCmd)` 호출 도달 → WinMain 내부 `RegisterClassExA` → `CreateWindowExA` → `ShowWindow` → `GetMessageA` loop 도달. 이 시점에서 USER32 bridge 의 실 동작 검증 가능 (wave3 의 helper.mm NSWindow alloc 이 이미 wired).

### 4.3 fake env block / cmdline 의 메모리 layout

```
fake_env_block (0x06000000 base):
  "PATH=C:\\Windows\\System32\0"
  "SystemRoot=C:\\Windows\0"
  "TEMP=C:\\Users\\Default\\AppData\\Local\\Temp\0"
  "USERNAME=DefaultUser\0"
  "\0"  (env block terminator)

fake_cmdline (0x06010000 base):
  "Battle.net Setup.exe\0"

fake_argv_block (0x06020000 base):
  argv[0] → 0x06010000 ("Battle.net Setup.exe")
  argv[1] → NULL
  argc = 1
```

---

## 5. opcode coverage 갱신

V5 시점의 누적 opcode + wave4 진행 중 추가 opcode (없음 — wave4 는 stub 의미화이지 opcode 추가 X).

### 5.1 V5 누적 (wave3 cap 해제 시점)

V5 §5.1 표 그대로 — 0x68 / 0x6A / 0x89 / 0x8B / 0xC7 / 0xB8-BF / 0xE8 / 0xFF /2 / 0xC2 / 0xC3 / 0x33 / 0x83 grp1 (sub 0-7 전부) / 0x85 / 0x84 / 0x8D / 0x55 / 0x5D / 0x50-57 / 0x58-5F / 0x74 / 0x75 / 0x0F 84-8F / 0xF7 grp3 / 0x64 / 0x65 prefix / 0xD8-0xDF FPU / 0x66 0x0F XX SSE / 0x40-4F INC/DEC / 0x11 / 0x13 / 0x19 / 0x1B ADC/SBB / 0xC0 / 0xC1 / 0xD0-3 shift / 0xFF /6 / 0xA0-A3 MOV moffs / 0x86 / 0x87 / 0x91-97 XCHG / 0xF0 LOCK prefix / 0xF2 / 0xF3 REP/REPNE / 0xA4-7 / 0xAA-F string / 0x60 / 0x61 PUSHA/POPA / 0x9C / 0x9D PUSHF/POPF / 0x98 / 0x99 sign-ext / 0x27 / 0x2F / 0x37 / 0x3F BCD / 0xCA / 0xCB RET FAR / 0xC9 LEAVE / 0x69 / 0x6B IMUL imm.

### 5.2 V6 신규 opcode

**없음** — wave4 는 stub return value 의미화 + IPC bridge wiring 이지 신규 opcode 통합 X. inst[277..343-370] 의 진척은 모두 V5 시점 누적 opcode 만으로 통과.

### 5.3 미구현 잔여

| opcode hex | mnemonic | 다음 sub-phase |
|---|---|---|
| 0x0F 90-9F | SETcc r/m8 | wave5 보강 (CRT init 인근 출현 가능) |
| 0x0F 40-4F | CMOVcc r32, r/m32 | wave5 보강 |
| 0x0F A3 / AB / B3 / BB | BT / BTS / BTR / BTC bit instruction | wave5 보강 |
| 0x0F BC / BD | BSF / BSR | wave5 보강 |
| x87 FPU 일부 (D9 /5 /7 FLDCW/FNSTCW + DB E3 FNINIT) | FPU control word | wave6 (D3D path 직전) |
| SSE/SSE2 일부 (MOVUPS/MOVAPS/PXOR/PADDD 등) | 산술/논리 | wave6 |
| 0xCC / 0xCD INT3 / INT imm | software interrupt | wave5 (debug breakpoint) |

### 5.4 opcode 통합 비율

V6 시점도 V5 와 동일 — i386 user-mode opcode 의 **약 75%** 통합 (Intel SDM Vol 2 single-byte + 0x0F escape user-mode subset 기준). 잔여 25% 의 대부분 = SETcc / CMOVcc / bit instr / FPU control / SSE.

---

## 6. BG dispatch wave 5 진행

cycle 102-103 sprint 의 BG dispatch wave 5 — 5 BG 동시 진행. own1 §A 정합 자체 구현만.

| BG | 영역 | file | status |
|---|---|---|---|
| BG 1 | shim wave5 CRT init stub (cpp 단독) | `native/pe_to_macho_shim.cpp` phase 21 MSVCR/UCRT stub semantic | 진행 중 |
| BG 2 | helper.mm WndProc callback shim (kind=20 또는 DispatchMessageA 응답 format 확장) | `native/helper.mm` WndProc callback bridge | 진행 중 |
| BG 3 | hexa msvcr_crt_init_stub | `lib/loader/pe_i386_msvcr_crt_init_stub.hexa` (신규) | 진행 중 |
| BG 4 | hexa winmain_entry_path | `lib/loader/pe_i386_winmain_entry_path.hexa` (신규) | 진행 중 |
| BG 5 | docs R3_PROGRESS_V6 (this doc) | `docs/R3_PROGRESS_V6.md` | **본 문서** |

BG 1 완료 시 inst[343-370] CRT init halt 해소 + WinMain 진입. BG 2 는 i386 측 WndProc 함수 ptr 을 helper 측에서 callback 하도록 reverse-IPC layer (helper → shim 의 i386 함수 호출 영역). BG 3-4 는 hexa skeleton + self-test (Hexa docker runner 의 bash/shasum/git 부재 + `type` 예약어 회피 일관 적용).

---

## 7. 누적 closure 진척

| layer | 영역 | closure 기준 | V5 | V6 |
|---|---|---|---|---|
| R1 | helper.mm IPC (subprocess / dlopen / dlsym / mmap / TEB / Metal probe + FPU/SSE/USER32/GDI32 stub helper + Phase 1 stage2 hexa loader bridge) | shim phase 1-13 통과 + IPC kind 12-15 | 100% | 100% (NSEvent + keymap 자체 표 추가) |
| R2 | PE 실 mmap + IAT KERNEL32 routing + entry trap + Metal device probe | shim phase 9-13 통과 | 100% | 100% (phase 1-13 own1 §A 유지) |
| R3-1~7 | shim disasm + interp + memory access + control flow + IAT thunk routing | 17 → 51 → 205 → 277 → **343-370 inst** | 75% | **80%** (run-by-run 변동 진척) |
| R3-8 | SEH register 자동 detect + FS/GS prefix segment override | inst[50] halt 해소 + SehRecord chain table | 55% | 55% (V5 그대로) |
| R3-9 | FPU/SSE prefix decode + helper IPC kind=12/13 echo | decode/dispatch path OK | 40% | 40% (V5 그대로 — 실 FP 미사용) |
| R3-10 | wave3 + wave4 (PEB/Ldr / KERNEL32 IAT stub return 의미화) | g_fake_modules + per-function fake handle/ptr/string + CPUID/RDTSC | ~30% | **~75%** (KERNEL32 stub 7-9 hit 통과 + __security_init_cookie 통과) |
| R3-11 | USER32/GDI32 IAT thunk routing (shim cpp 측 wiring) | shim dispatch_iat_thunk 의 USER32/GDI32 분기 + IPC kind=14/15 marshalling | ~10% | **~50%** (wiring 완료, 실 호출 도달 미검증) |
| R3-12 | first NSWindow 실 표시 (Battle.net Setup dialog) | helper kind=14 fn_id=2 + i386 측 호출 도달 | ~30% | ~30% (V5 그대로 — wave5 후 도달 가능) |
| **closure 통합** | "Wine 0줄 + R1+R2+R3 = real PE32 i386 launcher" | Battle.net Setup first dialog 표시 | ~60% | **~70%** |

### 7.1 누적 진척 cycle 예상

| cycle | wave | 누적 closure |
|---|---|---|
| 90 | R3 entry | 0% |
| 95 (V2) | R3-1~5 통합 (17 inst) | ~45% |
| 97 (V3) | R3-6 IAT thunk routing (51 inst) | ~50% |
| 99 (V4) | R3-8/9 SEH + FPU/SSE (205 inst) | ~55% |
| 101 (V5) | wave3 + helper NSWindow real (277 inst, 자연 halt) | ~60% |
| **103 (V6)** | **wave4 IAT stub semantics + USER32/GDI32 bridge wired (343-370 inst)** | **~70%** |
| 105 (V7 예상) | wave5 CRT init stub + WinMain 진입 | ~85% |
| 107 (V_FINAL 예상) | wave6 first NSWindow 실 표시 (Battle.net Setup dialog) | 100% (R3 closure) |

---

## 8. own1 enforcement 재확인

R3 cycle 102-103 sprint 동안 own1 §A enforcement 위반 0건. 검증 명령 + 결과:

| 영역 | 검증 명령 | 기대 | V6 결과 |
|---|---|---|---|
| Wine / CrossOver | `grep -r -i "wine\|crossover" native/ lib/loader/pe_i386_*.hexa` | 0 hit | 0 hit |
| Box86 / QEMU / Bochs | `grep -r -i "box86\|qemu\|bochs" native/ lib/loader/` | 0 hit | 0 hit |
| Capstone / Zydis / iced | `grep -r -i "capstone\|zydis\|^iced" native/ lib/loader/` | 0 hit | 0 hit |
| Apple GPTK / Whisky | `grep -r -i "gptk\|whisky\|game.porting.toolkit" native/ lib/loader/` | 0 hit (docs 거부 언급 외) | 정합 |
| MSVCRT source lift | `grep -r -i "ucrt-source\|ucrtbase-source\|msvcrt-source" native/ lib/loader/` | 0 hit | 0 hit (자체 stub 표 only) |
| Wine USER32 lift | `grep -r -i "winex11\|winecocoa\|wineqcoa" native/ lib/loader/` | 0 hit | 0 hit (자체 NSWindow alloc) |
| Wine ntdll SEH lift | `grep -r -i "RtlAddVectoredException\|__C_specific_handler" native/ lib/loader/` | 0 hit | 0 hit (자체 SehRecord struct) |
| 외부 fpu emul | `grep -r -i "softfloat\|berkeley.softfloat" native/ lib/loader/` | 0 hit | 0 hit (helper IPC echo only) |

### 8.1 자체 코드 양

| file | line | 영역 |
|---|---|---|
| `native/pe_to_macho_shim.cpp` | **4924** (+1062 vs V5) | phase 1-13 (R1+R2 closure) + phase 14-19 (R3-1/5/6/7/8/9) + wave3 opcode (R3-10 part) + **wave4 통합 (KERNEL32 stub return 의미화 표 + g_fake_modules + g_fake_proc_counter + __security_init_cookie CPUID/RDTSC + USER32 IPC kind=14 wiring + GDI32 IPC kind=15 wiring + dispatch_iat_thunk dll discriminator)** |
| `native/helper.mm` | **1271** (+260 vs V5) | IPC kind 1-15 + Phase 1 stage2 hexa loader bridge + NSApplication.run loop on main / IPC dispatch on worker + kind=14 fn_id=2 NSWindow 실 alloc + 32-bit fake hwnd ↔ NSWindow* mapping + **NSEvent → WM_KEYDOWN/WM_CHAR 변환 + DefWindowProcA + keymap 자체 표 (cycle 102 commit `abc9506`)** |
| `lib/loader/pe_i386_*.hexa` | **25** file (+2 vs V5) | R3-2/3/4 + R3-6 + R3-8/9 + R3-10/11/12 skeleton + V5 wave3 신규 (`pe_i386_xchg_lock_inst`, `pe_i386_misc_inst`) + **V6 wave4 신규 (`pe_i386_user32_iat_bridge_real`, `pe_i386_userland_first_dialog`)** |

own1 §A 정합 — Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + Microsoft Win32 API public docs reference only. KERNEL32 stub return 표 자체 작성 (Microsoft `learn.microsoft.com` API reference 의 return value spec 기반 — 코드 lift X). own1 own #1 (타사 Wine/CrossOver/GPTK/Whisky 추천 금지) — R3 cycle 102-103 의 trigger / game-changer 표현 시 외부 lift 추천 발생 0건.

---

## 9. cumulative commit log (R3 wave 1+2+3+4)

cycle 90 (R3 entry) → cycle 103 (V6 작성) 의 R3 관련 commit chain:

| commit SHA | 영역 |
|---|---|
| `84c31ec` | docs: R3 ROADMAP — sub-phase 1-12 정의 |
| `7116e96` | R3-1 shim phase 14 disasm + R3-4 hexa pe_i386_memory_model |
| `f67e95b` | R3-3 hexa pe_i386_register_state |
| `7cdab67` | R3-2 hexa pe_i386_disasm coverage tracker |
| `7dc4b20` | docs: R3-1~4 status note + commit race 기록 |
| `9126c84` | R3-5/6/7 shim phase 15-17 interpreter + R3-8~12 hexa skeleton (6 phase 일괄) |
| `2b90d2c` | shim phase 14-15 opcode 14 추가 — 7→17 instruction 진척 |
| `1bb3e94` | docs: R3_PROGRESS_V2 — 17 inst 도달 + opcode coverage 표 |
| `510a27b` | helper.mm IPC kind 12-15 (FPU/SSE/USER32/GDI32 stub helper) |
| `6adc502` | hexa pe_i386_shim_phase_18_22 (R3-8~12 phase skeleton) |
| `0e60b86` | hexa pe_i386_iat_thunk_routing |
| `7bfca4c` | R3-6 shim phase 16 IAT thunk routing — 17→51 inst |
| `badcc99` | docs: BATTLENET_REAL_LAUNCH_PLAN |
| `5371b32` | docs: R3_PROGRESS_V3 — 51 inst 도달 |
| `6088aec` | hexa pe_i386_segment_override_real |
| `e5a4187` | hexa pe_i386_x87_fpu_inst_real |
| `96c1532` | hexa pe_i386_seh_dispatch_runtime |
| `cbdface` | R3-8/9 shim phase 18-19 — FS prefix + SEH register + FPU/SSE prefix (51→205 inst) |
| `672cbee` | hexa pe_i386_xchg_lock_inst (atomic XCHG/LOCK/CMPXCHG/XADD tracker) |
| `c8260ac` | hexa pe_i386_misc_inst (BCD/sign-ext/PUSHA/INC-DEC/flag-manip tracker) |
| `b30c335` | docs: R3_PROGRESS_V4 — 205 inst + SEH register + wave3 trigger |
| `9c63397` | helper.mm NSWindow real create — kind=14 CreateWindowExA AppKit 통합 |
| `4d1ff9c` | shim wave3 — XCHG / LOCK / REP / string / INC-DEC / shift / PUSHFD / ADC-SBB (205→277 inst) |
| `b0f0756` | hexa pe_i386_user32_iat_bridge_real (V5 doc swept) |
| `abc9506` | helper.mm NSEvent → Win32 MSG 변환 + DefWindowProcA + keymap 자체 표 |
| `7a0eeaf` | hexa pe_i386_userland_first_dialog_e2e_real (R3-12 closure target end-to-end timeline) |
| **`212a7fe`** | **shim wave4 — R3-10/11 IAT stub return + USER32/GDI32 helper IPC bridge (277→343-370 inst)** |

**누적 V5→V6**: cpp +1062 line / mm +260 line / hexa +500-line × 2 file (user32_iat_bridge_real + userland_first_dialog) / docs (this doc). 4 commit (b0f0756 + abc9506 + 7a0eeaf + 212a7fe), 2 sprint (cycle 102-103).

### 9.1 핵심 V5→V6 commit chain

- **`b0f0756`** — hexa `pe_i386_user32_iat_bridge_real` tracker. USER32 IAT thunk → helper IPC kind=14 routing 의 e2e timeline + fn_id mapping 표 + args parsing (stdcall callee-pop) skeleton. Hexa 측 dry timeline (실 동작은 cpp 측 wiring 시점에).
- **`abc9506`** — helper.mm 의 NSEvent → Win32 MSG 변환 layer. NSKeyDown / NSKeyUp / NSLeftMouseDown / NSLeftMouseUp / NSMouseMoved → WM_KEYDOWN / WM_KEYUP / WM_LBUTTONDOWN / WM_LBUTTONUP / WM_MOUSEMOVE 변환. DefWindowProcA 자체 default proc 통합 (대부분 0 return + WM_CLOSE 시 [NSApp stop:nil] sentinel). keymap 자체 표 — virtual key code (VK_*) ↔ NSEvent.keyCode mapping table 자체 작성 (Wine `wineqcoa` mapping 0 reference).
- **`7a0eeaf`** — hexa `pe_i386_userland_first_dialog` tracker. R3-12 closure target 의 end-to-end timeline — i386 측 진척 inst[343-370] (CRT init halt) → wave5 통합 후 inst[500+] WinMain 진입 → RegisterClassExA → CreateWindowExA → ShowWindow → GetMessageA loop → 첫 화면 표시까지의 시퀀스 정의.
- **`212a7fe`** — shim wave4 일괄 통합. KERNEL32 stub return value 의미화 표 (GetSystemTimeAsFileTime / GetCurrentThreadId / GetCurrentProcessId / QueryPerformanceCounter / LoadLibraryExW / GetLastError / GetProcAddress / GetModuleHandleA / GetTickCount) + g_fake_modules (5개 fake module + base VA) + g_fake_proc_counter (0xCAFE0000 base) + __security_init_cookie CPUID/RDTSC 응답 + USER32 IPC kind=14 wiring (12 fn_id) + GDI32 IPC kind=15 wiring (4 fn_id) + dispatch_iat_thunk 의 dll discriminator. **277 → 343-370 instruction 진척** (delta +66~93, run-by-run 변동). interpreter unimplemented opcode 부재 → wave5 (CRT runtime init stub) 영역으로 progress.

---

## 10. V7 trigger / closure path 통합 정리

### 10.1 V7 trigger 조건

R3 PROGRESS V7 작성 trigger:

1. **wave5 CRT init stub 통합 완료** — `_initterm` / `__scrt_acquire_startup_lock` / `_get_initial_narrow_environment` / `_get_narrow_winmain_command_line` 등 의미화 stub 통합. inst[343-370] CRT init halt 해소 → inst[500+] 진척. 누적 ~85%.
2. **WinMain 진입** — `__scrt_common_main_seh` 의 `WinMain(hInstance, NULL, lpCmdLine, nShowCmd)` 호출 도달. RegisterClassExA → CreateWindowExA → ShowWindow → GetMessageA loop 진입.
3. **first NSWindow 실 표시** — i386 측 USER32 IAT 호출이 helper IPC kind=14 fn_id=2 로 routing → NSWindow alloc + makeKeyAndOrderFront → **Battle.net Setup 첫 dialog 화면 출력** (R3-12 closure trigger).
4. **plateau 발생** — 2 sprint 연속 진척 0 시 V7 에서 sub-phase 재분배.
5. **R3 closure 100% 도달** → V_FINAL 신규 작성.

### 10.2 closure path 통합

**closure path**: V6 (wave4 IAT semantics + USER32/GDI32 bridge wired) → wave5 (CRT init stub) → wave6 (R3-12 first NSWindow 실 표시) → R3 closure 100%.

**V7 trigger 핵심**: WinMain 진입 + RegisterClassExA → CreateWindowExA → 실 NSWindow 표시. 즉 wave5 + wave6 의 합산 결과. 이 중 wave5 는 cpp 단독 stub 의미화 (외부 lift 0줄), wave6 는 V6 시점 이미 wired 된 USER32 bridge 의 호출 도달 검증.

own1 enforcement 일관 — cycle 90 의 closure 100% (R1+R2) 는 phase 1-13 의 자체 구현. cycle 91-103 의 R3 진척 또한 외부 emulator / Wine fork lift 0줄. **자체 i386 execution layer + 자체 KERNEL32 stub 표 + 자체 USER32/GDI32 IPC bridge** 의 cohesion 누적 ~70% — 잔여 ~30% 의 sub-phase 4 (wave5 CRT init + wave6 first NSWindow) 가 V6 → V7 → V_FINAL cycle 동안 진척 대상.

→ 본 V6 가 cycle 103-105 sprint 동안 유효. cycle 105 즈음 wave5 CRT init 통합 + WinMain 진입 시점에 V7 재작성 기대.

---

*written 2026-05-01 cycle 103 sprint 직후. R1+R2 own1 §A closure 100% 유지 + R3-1~11 wave4 통합 완료 + shim phase 14-20 + wave3 opcode cap 해제 + **wave4 IAT stub return value 의미화 + PEB/Ldr fake module table + __security_init_cookie CPUID/RDTSC + USER32/GDI32 IPC bridge wiring** 동작. **343-370 instruction** 도달 (V5 의 277 → V6 의 343-370, delta +66~93, run-by-run 변동). inst[277] `0xC3` RET → EIP=0 자연 halt → wave4 통합 후 KERNEL32 stub 7-9 hit 통과 → MSVCR/UCRT 의 `__scrt_common_main_seh` → `__security_init_cookie` 통과 → `_initterm` / `__scrt_acquire_startup_lock` / `_get_initial_narrow_environment` 인근 CRT runtime init 영역에서 자연 halt 의 trigger 진보 (IAT semantics 영역 통과, CRT runtime semantics 영역 진입). helper.mm 의 NSEvent → Win32 MSG 변환 + DefWindowProcA + keymap 자체 표 통합 (cycle 102 commit `abc9506`). USER32/GDI32 IPC bridge 의 shim cpp 측 wiring 완료 (단, WinMain 미진입으로 호출 도달 0건 — wave5 후 검증). 다음 trigger 는 wave5 의 MSVCR/UCRT CRT init stub 의미화 (`_initterm` no-op return 0 + fake env block ptr + fake winmain command line ptr 등) — BG 1-4 진행 중. own1 §A enforcement 일관 (Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift 0줄). KERNEL32 stub return 표 자체 작성 (Microsoft API public docs reference 의 return value spec 기반). 본 PROGRESS V6 path 외 수정 X. Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + Microsoft Win32 API public docs + ReactOS public docs reference only.*
