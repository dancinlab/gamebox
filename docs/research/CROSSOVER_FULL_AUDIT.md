# CROSSOVER_FULL_AUDIT — CrossOver source + wine-staging 전수조사

**범위**: CrossOver Mac 25.0.0 (Wine 10.0 base) public source tarball — `crossover-sources-25.0.0.tar.gz` (170 MB, sha256 `b0f3c1263bb1d7bfb8afa63493550be832ca55cd5f3d0bd2c9077991638d4e44`) + Wine Staging 11.8 (upstream commit `71e7a2e81eec`).

**원칙**: own 1 (DIY hexa PE 로더 + Win32 shim, Wine/CrossOver/community 코드 0줄). 본 문서는 *analysis only* — code lift 0 줄. 알고리즘 방향성만 reference.

**분석 위치**: `/tmp/crossover-source/sources/wine/` (CrossOver 25 patched Wine), `/tmp/crossover-research/wine-staging/patches/` (vanilla Staging).

---

## 0. Source Tree 개요

CrossOver 25 tarball top-level: `wine`, `dxvk`, `moltenvk`, `vkd3d`, `freetype`, `gnutls`, `gstreamer`, `glib`, `cabextract`, `samba3`, `flatpak`, `android`, `busybox`, `ghostscript`, `htmltextview`, `makedep`, `po4a`, `pyxdg` (총 19 subproject). 핵심은 `wine` (Wine 10.0 + CodeWeavers patches) 와 graphics translation layer 3개 (`dxvk`, `vkd3d`, `moltenvk`).

CrossOver-specific 마커 grep 결과 (`grep -rl "CrossOver Hack" sources/wine/`): **27 파일**에 `CrossOver Hack` / `CW HACK` / `CW Hack` 주석. 마커 문법은 두 종류:
- `CrossOver Hack <bug-id>` — 옛 형태 (10000~19999 range, 2010~2018 era)
- `CW HACK <bug-id>` 또는 `CW Hack <bug-id>` — 신 형태 (20000+ range, 2020 이후)

각 marker 는 CodeWeavers 사내 bugzilla ticket 번호. 예: 10523, 13438, 16371, 17634, 18576, 18775, 18896, 18947, 19776, 20512, 22011, 22144, 22310, 22434, 22795, 22939, 22996, 23015, 23655, 23881, 24067, 24141, 24199, 24560, 24717.

---

## 1. msync — Mach Semaphore 동기화 (CrossOver 의 "esync 대체")

### 1.1 배경

vanilla Wine 의 NT 동기화 객체 (Event / Mutex / Semaphore) 는 wineserver 와의 SCM_RIGHTS unix socket round-trip 으로 대기. esync (eventfd) / fsync (futex) 는 Linux 전용. macOS 에는 둘 다 없으므로 CrossOver 는 **msync** (Mach semaphore + ulock) 를 자체 도입.

### 1.2 파일 surface (CrossOver 25)

| 파일 | LOC | 역할 |
|---|---|---|
| `dlls/ntdll/unix/msync.c` | 1778 | client-side: handle 캐시 + wait/signal + Mach msg pump |
| `dlls/ntdll/unix/msync.h` | 50 | type / enum 정의 |
| `server/msync.c` | 1025 | wineserver-side: registered waiter table |
| `server/msync.h` | – | server protocol struct |

비교 대상: `dlls/ntdll/unix/esync.c` 1477 line, `server/esync.c` (Linux eventfd path). msync 는 esync 보다 server-side 가 더 큼 — Mach port 등록/해제 path 추가.

### 1.3 알고리즘 (own 1 reference 가능 direction)

1. **Semaphore Pool** — `static struct semaphore_memory_pool *pool;` 로 Mach `semaphore_create()` 결과를 1024개까지 free-list 에 캐시. `semaphore_pool_alloc/free` 로 재활용. 임계 영역은 `os_unfair_lock` 보호.
2. **Single-object wait** — `__ulock_wait2(UL_COMPARE_AND_WAIT_SHARED, addr, value, timeout, 0)` 로 user-space 비교 후 kernel block. fsync futex 와 동등한 fast path.
3. **Multi-object wait** — pool 에서 semaphore 1개 빌려서 wineserver 에 Mach message 로 등록, server 가 임의 객체가 signaled 되면 `semaphore_signal()`, client 는 `semaphore_timedwait()` 으로 깨어남.
4. **Shared memory layout** — `/tmp/wine-msync-XXXX` shm 파일 page-on-demand `mmap`. 객체당 16 byte (primary value, secondary, destroyed flag, waiter count). semaphore = (count, max), event = (signaled, –), mutex = (tid, recursion).
5. **Server protocol** — Mach port (`server_port`) 로 `server_register_wait` / `server_remove_wait` message. message ID 는 unique (현재 wait 호출 추적용).
6. **Init order** — `dlls/ntdll/unix/sync.c:320` 에서 모든 sync entry-point (`NtCreateSemaphore` 등) 가 `if (do_msync()) ... else if (do_esync()) ...` dispatch. msync 가 우선.

### 1.4 wine-staging 11.8 비교

wine-staging 11.8 patch 디렉토리 (총 119 patchset) 에는 **msync 없음**. esync 는 mainline 통합되어 wine-staging 에서도 별도 patchset 미존재. msync 는 *CrossOver 단독 + GitHub `marzent/wine-msync` upstream*. CrossOver 25 의 msync 코드는 `Copyright (C) 2018 Zebediah Figura` (esync 원저자) + `Copyright (C) 2023 Marc-Aurel Zent` (msync 저자) 헤더 — `marzent/wine-msync` upstream 을 그대로 통합.

### 1.5 msync 데이터 layout 상세

`dlls/ntdll/unix/msync.c:578-597` 에 명시된 *각 객체 8 byte* — 단 `get_shm()` 의 `(idx * 16)` 으로 보아 entry stride 는 16 byte (8 byte payload + 8 byte padding/destroyed). `MSYNC_LIST_BLOCK_SIZE = 65536/sizeof(struct msync) = ~5460`, `MSYNC_LIST_ENTRIES = 256` → 최대 ~1.4M handle 까지 O(1) lookup. handle 의 high bit 둘은 `((handle>>2)-1)` 로 변환되는 표준 NT handle convention.

Mach message 구조 (line 291-308):
```
mach_register_message_t = {
  mach_msg_header_t header;
  mach_msg_body_t body;
  mach_msg_port_descriptor_t descriptor;  // semaphore_t port
  unsigned int shm_idx[MAXIMUM_WAIT_OBJECTS + 1];  // up to 64+1 indices
}
```

→ 한 wait 호출 당 메시지 1개. server 가 해당 indices 를 watch list 에 추가, 임의 index 가 signal 되면 `semaphore_signal(port)`. unregister 도 동일 포맷 (descriptor 없음).

### 1.6 own 1 정합 reference

본 프로젝트의 동기화는 `pthread_mutex_t` / `dispatch_semaphore_t` / Apple `os_unfair_lock` 만 사용. msync 의 *알고리즘 direction* (semaphore pool reuse + ulock fast path + Mach msg multi-wait + 16-byte fixed-stride shm) 만 docs/research 에 기록하고, 구현은 hexa 로 zero-copy 작성. shm path naming (`/tmp/wine-msync-XXXX`) 도 own naming 으로 (`/tmp/airgenome-sync-<pid>`).

---

## 2. winemac.drv — Mac driver 강화

### 2.1 wine-staging 의 winemac patch

wine-staging 11.8 의 `winemac.drv` 관련 patch 는 **단 1개**: `winemac.drv-no-flicker-patch/0001-winemac.drv-No-Flicker-patch.patch` (Ken Thomases, CodeWeavers 직원). 효과: registry `ForceOpenGLBackingStore` 추가, true 면 `kCGLPFABackingStore` attribute 강제 — fullscreen flicker 방지 (`Fixes: [34166]`).

### 2.2 CrossOver 25 의 winemac.drv

`sources/wine/dlls/winemac.drv/` 의 `.c`+`.m` 합산 **27 035 LOC**. 주요 추가/변경 (vanilla Wine 대비):

| 영역 | CrossOver 추가 | Marker |
|---|---|---|
| **Mac Edit menu** (Cut/Copy/Paste 시스템 키 매핑) | `cocoa_app.m` / `cocoa_window.m` / `event.c` / `keyboard.c` 전반 | Hack 10912 |
| **App Nap / Activation 차단** (`enable_app_nap`) | `cocoa_app.m` `beginActivityWithOptions:` 강제 leak | Hack 14364 |
| **External quit request** (Steam/Rockstar/Epic launcher 종료 시 child 동시 종료) | `postExternalQuitRequest` whitelist | Hack 22310 / 23655 / 24199 |
| **Dock icon black-list** (GOG Galaxy Notifications) | `cocoa_app.m:325` 에서 `setActivationPolicy` skip | Hack 24141 |
| **App icon URL fallback** (passed-in URL 의 icon resource 추출) | `cocoa_app.m:2924` | Hack 13440 |
| **Display mode safe-flag bypass** on Apple Silicon | `display.c:39, 132` `is_apple_silicon()` true 면 `kDisplayModeSafeFlag` check 생략 | Hack 18576 |
| **Skyrim SE Launcher** 특수 처리 | `display.c:61` `is_skyrim_se_launcher()` | Hack 20512 |
| **Window focus 보존** (focus 이벤트 discard 안함) | `cocoa_window.m:2133, 3001` | Hack 18896 |
| **D3DMetal hooks** (별도 § 5) | `dlls/winemac.drv/d3dmetal.c` 404 LOC | – |

### 2.3 알고리즘 direction (own 1 reference)

본 프로젝트의 macOS window backend 는 hexa 가 직접 AppKit FFI 로 만든다. 위 hack 들은 *어떤 macOS 행동이 Win32 app 호환성을 깨는가* 의 *evidence sheet* 로만 reference:

- AppNap → 게임 idle 시 throttling. `NSProcessInfo beginActivityWithOptions:` 로 disable.
- DisplayMode safe-flag → Apple Silicon 에서 일부 mode 가 unsafe-marked 됨에도 정상 동작. validation skip 필요.
- Steam-style launcher → child 가 IPC 로 quit 신호 받아야 정상 종료. `applicationShouldTerminate:` reply 를 IPC bridge 로 forward.
- BackingStore (`ForceOpenGLBackingStore`) → fullscreen 일부 frame missing 방지.

---

## 3. 32-bit on 64-bit (wine32on64 / wow64on32)

### 3.1 wine32on64 의 정체

CodeWeavers 가 macOS 10.15+ (32-bit binary 지원 OS-level 폐기) 에 대응하기 위해 만든 hack. 32-bit Windows binary 를 *64-bit macOS process 안의 32-bit code segment* 에서 실행. 핵심 trick: macOS Catalina+ 가 64-bit process 안에서도 i386 code segment 를 매핑/실행 가능 (단, 32-bit dyld / dylib 는 부재). custom Clang fork 로 32-bit code 를 64-bit Mach-O 안에 묶는다.

### 3.2 CrossOver 25 source 확인

CrossOver 25 의 wine 은 **upstream wow64 path 기반** — `dlls/wow64`, `dlls/wow64cpu`, `dlls/wow64win` 폴더 그대로. 별도 `wine32on64` 디렉토리는 *tarball 에 없음* (Wine 10.0 의 ELF→PE 전환 + new-WoW64 (PE-only 32-bit on 64-bit ntdll) 가 완성되어 CrossOver 가 "wine32on64 hack 졸업" — 2024~2025 year 의 changelog 에서 공식화).

흔적은 다음에 남음:
- `dlls/ntdll/unix/process.c:838` `if (!is_win64 ^ !is_child_64bit) loader = get_alternate_loader( &wineloader );` — child architecture 가 다르면 alternate loader (e.g. `wineloader64`) 호출. CodeWeavers 의 22144 hack 으로 loader name 을 dynamically rename (Dock title 용).
- `dlls/ntdll/unix/loader.c:570, 718` (Hack 22144) — preloader 의 `argv[0]` 을 exe basename 으로 rewrite, `wineloader` symlink 를 *exe-name* 으로 만들어 exec — Activity Monitor / Dock 에서 process name 이 "explorer.exe" 로 보이게 함.

### 3.3 wine-staging 비교

wine-staging 11.8 patches: 32-on-64 patchset 없음. CrossOver 만의 영역.

### 3.4 본 프로젝트 입장 (own 1)

own 1 (e) 항목에 의해 wine32on64 / GPTK / Whisky 사용 금지. 본 프로젝트의 32-bit support direction:

- D2R/D4 는 둘 다 **x86_64 PE** — 32-bit PE 지원 outright 불필요 (Phase 0~10 범위 내).
- 만약 Phase 11+ 에서 i386 PE 가 필요해지면 → Rosetta 2 (Apple) 가 i386 user-mode binary 를 직접 실행하지 않으므로, *hexa-native i386 interpreter* 또는 box86 link (own 1 위반) 중 택1. 현재 plan 에는 없음.

---

## 4. D3D / DXVK / MoltenVK / vkd3d / D3DMetal

### 4.1 CrossOver 25 의 graphics stack

| 백엔드 | 위치 | 출처 |
|---|---|---|
| **wined3d** | `wine/dlls/wined3d/` | Wine 10.0 mainline + CW Hack 18775 (FBO check skip) |
| **DXVK** | `sources/dxvk/` | doitsujin/dxvk fork |
| **vkd3d** | `sources/vkd3d/` | Wine vkd3d fork |
| **MoltenVK** | `sources/moltenvk/` | KhronosGroup/MoltenVK (2015–2024 © Brenwill) |
| **D3DMetal** | `dlls/winemac.drv/d3dmetal.c` (404 LOC) | Apple Game Porting Toolkit (closed-source `.dylib`) + thin Wine glue |
| **DXMT** | (CrossOver 25 추가, source 미포함; 3Shain/dxmt) | community |

### 4.2 D3DMetal 통합 메커니즘

`dlls/winemac.drv/d3dmetal.c` 가 **유일한 glue layer**. 구조:

```c
DECLSPEC_EXPORT struct macdrv_functions_t macdrv_functions = {
    &my_macdrv_init_display_devices,
    &my_get_win_data,
    &my_release_win_data,
    &my_macdrv_get_cocoa_window,
    &my_macdrv_create_metal_device,
    &my_macdrv_release_metal_device,
    &my_macdrv_view_create_metal_view,
    &my_macdrv_view_get_metal_layer,
    &my_macdrv_view_release_metal_view,
    &my_OnMainThread,
    &my_RegQueryValueExA, &my_RegSetValueExA, &my_RegOpenKeyExA, &my_RegCreateKeyExA, &my_RegCloseKey,
    &my_EnumDisplayMonitors, &my_GetMonitorInfoA, &my_AdjustWindowRectEx,
    &my_GetWindowLongPtrW, &my_GetWindowRect, &my_MoveWindow, &my_SetWindowPos,
    &my_GetSystemMetrics, &my_SetWindowLongPtrW,
};
```

→ Apple GPTK 의 `libd3dshared.dylib` 가 이 vtable (192 byte, `C_ASSERT` 검증) 을 dlsym/Mach-O symbol 로 lookup. registry 와 window/display API 도 이 table 로 우회 (DLL→DYLIB 호환 layer).

`d3dmetal_macdrv_win_data` struct 는 `C_ASSERT(sizeof == 120)` — Apple 측 closed-source 와의 ABI 약속. 필드가 `color_key` / `surface` 등 더 이상 안 쓰는 칸 까지 보존되는 이유는 *Apple `.dylib` 가 그 offset 을 hard-code* 하기 때문 (주석에 "color_key is no longer present in macdrv_win_data. Assume D3DMetal doesn't use it.").

### 4.3 wined3d 의 macOS-specific patch

`wined3d/utils.c:2534` (Hack 18775) — Big Sur Apple Silicon 에서 `glCheckFramebufferStatus(GL_TEXTURE_CUBE_MAP, GL_RGB_422_APPLE)` 가 Metal exception 발생 → Wine 측에서 `GL_FRAMEBUFFER_UNSUPPORTED` 를 manually return. Monterey 이후 fixed.

### 4.4 DXVK / MoltenVK 측 patch

`sources/dxvk/` 는 doitsujin/dxvk 의 standard fork — CrossOver 가 추가한 macOS 전용 patch 는 git history 에 들어있고 source layout 은 공개판과 동일. `sources/moltenvk/` 는 vanilla MoltenVK (Brenwill 2015–2024).

특수 환경변수 — `dlls/ntdll/unix/process.c:914` (Hack 24560): Path of Exile 2 Steam 빌드 실행 시 `MVK_CONFIG_ADVERTISE_EXTENSIONS=0` 강제. Vulkan extension 광고를 끔으로써 PoE2 launcher 의 detection 을 우회.

### 4.5 own 1 reference direction

본 프로젝트의 graphics: hexa → Apple Metal API 직접 binding (`lib/loader/dx_d3dmetal.hexa` Phase 5). DXVK/MoltenVK/vkd3d/wined3d 는 own 1 (e) 항목에 의해 사용 금지. **DirectX 11/12 → Metal 직접 매핑** 만이 own 정합 path. CrossOver 의 d3dmetal.c vtable 패턴은 *어떤 surface API call 이 hot path* 인지의 evidence sheet 로 reference.

---

## 5. Battle.net / Blizzard / Launcher 특수 처리

CrossOver 25 가 가장 "ad-hoc 한" 영역. own 1 의 동기 그 자체.

### 5.1 ddraw — Battle.net System Checker

`dlls/ddraw/main.c:78, 491` — `D3DDeviceEnumeration` 에서 Battle.net System Checker 가 *NULL device + GUID-based device 둘 다* 받기를 기대. 그래서 Wine 측이 양쪽 다 enumerate. vanilla Wine 도 비슷하지만 CrossOver 가 주석에 "System Checker expects" 라고 명시.

### 5.2 advapi32 — Battle.net Agent ACL hack (Hack 23881)

`dlls/advapi32/security.c:2725-2946`. 시나리오: `Agent.exe` (Battle.net launcher 의 update agent) 가 `GetNamedSecurityInfoW` 로 자기 ProgramData 디렉토리 권한을 조회 후 *expected SID/ACL* 와 비교, 불일치 시 update 거부.

알고리즘 (CrossOver 가 Wine 측에서 forge):
1. `is_battle_net_agent()` — `GetModuleFileNameW(NULL)` basename 이 `Agent.exe` 면 true.
2. `GetNamedSecurityInfoW(SE_FILE_OBJECT)` 호출 시, path 가 다음 substring 을 포함하면:
   - `/ProgramData/Battle.net/Agent`
   - `\\ProgramData\\Battle.net_components`
   - `/Program Files (x86)/Battle.net`
3. `hack_battle_net_sd()` 가 SD 를 강제 교체:
   - DACL 3 entry: `S-1-5-32-544` (BUILTIN\Administrators), `S-1-5-32-545` (Users), `S-1-5-4` (INTERACTIVE)
   - 권한 mask: `0x1F01FF` (FILE_ALL_ACCESS)
   - inheritance: `OBJECT_INHERIT_ACE | CONTAINER_INHERIT_ACE`
   - owner: `S-1-5-32-544`
   - `MakeAbsoluteSD` → `SetSecurityDescriptorDacl/Owner` → `MakeSelfRelativeSD` round-trip 으로 self-relative SD 재생성

→ Battle.net Agent 는 자기가 Windows Admin 환경에 설치되어 있다고 *믿게* 됨.

### 5.3 본 프로젝트의 Phase 8 (Battle.net 우회) direction

이 hack 의 *알고리즘 방향* 은 own 1 정합 reference 가능:

- Phase 8 에서 hexa shim `advapi32.GetNamedSecurityInfoW` 가 path-pattern 매칭 후 *forged SD* 반환. SD 구조는 Microsoft 공식 docs (`SECURITY_DESCRIPTOR`, `MakeAbsoluteSD`, etc.) 만 reference.
- Battle.net 이 Agent.exe / Bootstrapper.exe / BlizzardError.exe 어떤 path 로 SD 검증하는지는 본 프로젝트 자체 reverse engineering 으로 확인 (CrossOver 코드 재사용 X).

### 5.4 다른 Launcher 전용 hack

| 앱 | Hack ID | 설명 |
|---|---|---|
| Steam (`Valve.Steam.Client`) | 22310 | external quit request (NSWorkspace logout 시 IPC 로 child 종료) |
| Rockstar Social Club | 23655 | 같은 quit-request whitelist |
| Epic Games Launcher | 24199 | EpicWebHelper child 동시 종료 |
| GOG Galaxy Notifications | 24141 | dock icon transformation 차단 |
| MS VC++ Redistributable 2015/2017 | 19776 (msi/files.c) | 설치 path / file overwrite hack |
| Skyrim SE Launcher | 20512 | display mode 검증 skip |
| Path of Exile 2 Steam | 24560 | `MVK_CONFIG_ADVERTISE_EXTENSIONS=0` 강제 |
| Apple Silicon Rosetta cross-process write | 18947 | `mach_vm_write` 후 page exec bit toggle 로 Rosetta cache invalidate |

→ CrossOver 의 본질이 "**Wine 위에 게임/launcher 별 N개 hack 누적**" 임이 통계로 확인됨 (27 marker 파일, 200+ ticket 추정).

---

## 6. 기타 핵심 macOS-specific hack (정리)

### 6.1 Loader / Process

- **Hack 10523** — `dlls/ntdll/unix/process.c`: `send_to_cx_loader()`. CreateProcess 시 unix socket 으로 Mac app code (`WineLoader.m`) 에 process 생성 위임. magic constant `REQUEST_LOAD_WINE = 0x52c17355`. macOS launcher 가 새 prefix bottle / log 등을 통합 관리하기 위함.
- **Hack 22144** — preloader 가 없을 때 `wineloader` 이름을 exe basename 으로 rewrite 후 exec (Dock title 표시용). symlink 생성 로직.
- **Hack 24717** — `__CX_UNIX_<NAME>=value` PE 환경변수가 child unix process 환경으로 promote (mac-side 만).
- **Hack 13438** — `loader/main.c:apple_override_bundle_name`: Mach-O `__TEXT,__info_plist` section 의 `CFBundleName` 을 process 시작 시 padding 영역 활용해 in-place rewrite. AppKit framework 가 menu title 을 bundle name 에서만 가져오기 때문.
- **Hack 22939** — ASLR (`IMAGE_FLAGS_ImageDynamicallyRelocated`) 처리를 Vista+ OS 에서만 수행.
- **Hack 22996** — `simulate_writecopy` 글로벌. Windows COW page semantic 흉내.

### 6.2 Memory

- **Hack 16371** — `virtual_release_address_space()`: macOS 에서 32-bit OpenGL 리소스를 위해 `0x40001000~0x7F000000` 를 free (vanilla 는 `0x20000000~0x7F000000`). preloader 부재 시 적용.
- **Hack 17634** — `force_laa()`: registry `HKCU\Software\Wine\AppDefaults\<exe>\LargeAddressAware=1` 또는 `WINE_LARGE_ADDRESS_AWARE` env var 면 32-bit app 에 LAA 강제.
- **Hack 22011** — `<sys/utsname.h>` 포함, macOS kernel version 분기.
- **Hack 18947** — Rosetta 2 가 cross-process write 를 못 봐서, `mach_vm_write` 직후 *target process 에서* 해당 page 의 exec bit 를 일시 OFF/ON toggle → Rosetta 가 retranslate 강제. `is_apple_silicon()` (`sysctl.proc_translated`) 에서만 발동.

### 6.3 ntdll/sync 자체 hack

- **Hack 23015** — `dlls/ntdll/unix/sync.c` 에 7회 marker. msync/esync dispatch 위에 추가 layer (정확한 bug 내용은 주석 부족). server.c:1941 에도 동일 marker.

### 6.4 etc.

- **Hack 22795** — `kernelbase/debug.c`: SetUnhandledExceptionFilter 관련.
- **Hack 22434** — `loader.c:1176` — TBD.
- **Hack 24067** — `loader.c` — `simulate_writecopy` symbol visibility default 보존.
- **Hack 12735, 12791** — `shell32/shellpath.c`: known folder path resolution.
- **Hack 10879** — `win32u/defwnd.c`: window message dispatch.
- **Hack 15207** — `ntdll/unix/file.c:1347`: `~$` 시작 파일 hide (Office lock file).

### 6.5 Hack ticket 통합 index

| ID | 영역 | 설명 |
|---|---|---|
| 10523 | ntdll/unix/process.c | Mac app 에 process 생성 위임 (unix socket, magic 0x52c17355) |
| 10879 | win32u/defwnd.c | window message dispatch tweak |
| 10912 | winemac.drv (전반) | Mac Edit menu (Cmd+C/V/X) Win32 menu mapping |
| 12735 | shell32/shellpath.c | known folder path resolution |
| 12791 | shell32/shellpath.c | known folder fallback |
| 13438 | loader/main.c | Mach-O Info.plist `CFBundleName` 동적 rewrite |
| 13440 | winemac.drv/cocoa_app.m | passed-in URL 의 icon resource fallback |
| 14364 | winemac.drv/macdrv_main.c | App Nap 차단 |
| 15207 | ntdll/unix/file.c | `~$` 시작 파일 hide (Office lock file) |
| 16371 | ntdll/unix/virtual.c | mac OpenGL 용 0x40001000~0x7F000000 free |
| 17634 | ntdll/unix/virtual.c | `force_laa()` LargeAddressAware 강제 |
| 18576 | winemac.drv/display.c | Apple Silicon `kDisplayModeSafeFlag` bypass |
| 18775 | wined3d/utils.c | Big Sur cube-map FBO check skip |
| 18896 | winemac.drv/cocoa_window.m | focus 이벤트 discard 안함 |
| 18947 | ntdll/unix/virtual.c | Rosetta 2 page exec bit toggle (write invalidation) |
| 19776 | msi/files.c | MS VC++ 2015/2017 redist 설치 hack |
| 20512 | winemac.drv/display.c | Skyrim SE Launcher 특수 처리 |
| 22011 | ntdll/unix/virtual.c | macOS kernel version 분기 (utsname) |
| 22144 | ntdll/unix/loader.c | `wineloader` 이름을 exe basename 으로 rewrite + symlink exec |
| 22310 | winemac.drv/cocoa_app.m | external quit request (Steam 등) |
| 22434 | ntdll/unix/loader.c | TBD |
| 22795 | kernelbase/debug.c | SetUnhandledExceptionFilter |
| 22939 | ntdll/unix/virtual.c | ASLR 처리 OS Vista+ 한정 |
| 22996 | ntdll/unix/loader.c | `simulate_writecopy` 글로벌 |
| 23015 | ntdll/unix/sync.c (7회), server.c | sync dispatch 추가 layer |
| 23655 | winemac.drv/cocoa_app.m | Rockstar Social Club quit-request whitelist |
| 23881 | advapi32/security.c | **Battle.net Agent ACL forging** |
| 24067 | ntdll/unix/loader.c | symbol visibility |
| 24141 | winemac.drv/cocoa_app.m | dock icon transformation 차단 (GOG Galaxy) |
| 24199 | winemac.drv/cocoa_app.m | Epic Web Helper child 동시 종료 |
| 24560 | ntdll/unix/process.c | Path of Exile 2 — `MVK_CONFIG_ADVERTISE_EXTENSIONS=0` |
| 24717 | ntdll/unix/process.c | `__CX_UNIX_<NAME>=` env var promotion |

---

## 6.6 Battle.net Phase 8 design reference

Hack 23881 의 algorithm direction 을 own 1 정합으로 풀어 쓰면:

```
phase8_battle_net.hexa (의사 design):
  fn shim_GetNamedSecurityInfoW(name, type, info, owner, group, dacl, sacl, descriptor):
    // 1. baseline call (own 자체 advapi32 shim)
    err = own_advapi.GetNamedSecurityInfoW(...)
    if err != ERROR_SUCCESS: return err

    // 2. own detection (basename + path-pattern)
    if not is_battle_net_agent_exe():  // GetModuleFileNameW basename "Agent.exe"
      return err
    if type != SE_FILE_OBJECT:
      return err
    if not (name.contains("/ProgramData/Battle.net/Agent")
            or name.contains("\\ProgramData\\Battle.net_components")
            or name.contains("/Program Files (x86)/Battle.net")):
      return err

    // 3. forge SD (Microsoft docs spec)
    forge_sd_with(
      owner = SID(S-1-5-32-544),                // Administrators
      dacl_entries = [
        ACE(S-1-5-32-544, 0x1F01FF, OBJECT_INHERIT|CONTAINER_INHERIT),
        ACE(S-1-5-32-545, 0x1F01FF, OBJECT_INHERIT|CONTAINER_INHERIT),
        ACE(S-1-5-4,      0x1F01FF, OBJECT_INHERIT|CONTAINER_INHERIT),
      ]
    )
    return ERROR_SUCCESS
```

→ CrossOver 의 `hack_battle_net_sd` 와 동등한 *효과* 를 own 자체 SECURITY_DESCRIPTOR 빌더로 달성. 코드 lift 0줄 — 모든 mask/SID 는 Microsoft `winnt.h` docs 에서 직접 인용.

reverse engineering 의무 (own 자체):
- Battle.net 의 어떤 binary 가 SD 검증? (Agent.exe, Bootstrapper.exe, BlizzardError.exe — own 측 직접 확인)
- 어떤 path 가 검증? (CrossOver 가 적은 3개 외 추가 존재 가능 — own 측 직접 확인)
- forge SD 가 부족하면 Bootstrapper 단계에서 거부될 수 있음 — own 측 retry strategy 별도.

---

## 7. wine-staging 11.8 patchset 전수 분류

총 119 patchset. CrossOver 와 무관한 항목은 macOS 프로젝트와 직접 비교 의미가 작으나, 본 프로젝트 reference 가능 영역만 태그:

| 카테고리 | 대표 patch | 본 프로젝트 reference 여부 |
|---|---|---|
| macOS 전용 | `winemac.drv-no-flicker-patch` | direction 만 reference (own 1 정합) |
| sync 가속 | (mainline esync; msync 는 별도) | algorithm direction (Mach 부분만) |
| ntdll memory | `ntdll-WRITECOPY`, `ntdll-ForceBottomUpAlloc`, `ntdll-NtQueryVirtualMemory` | spec direction (Win32 docs reference) |
| server | `server-Inherited_ACLs`, `server-Stored_ACLs`, `server-File_Permissions` | own shim 직접 작성 (spec only) |
| d3d/wined3d | `wined3d-SWVP-shaders`, `wined3d-Indexed_Vertex_Blending` | DirectX spec → Metal 직접 매핑 |
| dinput / wintab | `dinput-joy-mappings`, `wintab32-improvements` | 무관 (HID 직접) |
| 그 외 | shell32 / ole / msxml / mshtml / inseng / ddraw / etc. | 본 프로젝트 phase 0~10 범위 외 |

**Staging 의 전체 본질** — Wine mainline 이 받기엔 침습적 / 호환성 위험 patches. CrossOver 는 이 중 *macOS 영역에서 효과 있는 일부* + *자체 N개 hack* 을 합쳐 빌드.

---

## 8. 비교 표 — vanilla Wine vs Wine Staging 11.8 vs CrossOver 25 vs 본 프로젝트

| 영역 | vanilla Wine 10.0 | Wine Staging 11.8 | CrossOver Mac 25 | airgenome-gamebox |
|---|---|---|---|---|
| **License** | LGPL 2.1+ | LGPL 2.1+ | LGPL 2.1+ (Wine 부분) + closed (Mac.app) | GPL/MIT + Apple SDK only |
| **Code base** | 3M+ LoC C/PE | +119 patchsets | Wine 10.0 + 27 마커 파일 patch + GPTK link | hexa from scratch |
| **PE 로더** | Wine builtin | Wine builtin | Wine builtin + Hack 10523 (Mac app delegation) | DIY hexa (`lib/loader/pe_*.hexa`) |
| **Win32 API shim** | 600+ DLL | mainline + N | mainline + 27 hack 파일 | DIY hexa (Phase 2-4) |
| **NT 동기화** | wineserver socket | esync/fsync (Linux) | **msync** (Mach sema + ulock) | pthread/dispatch_semaphore (own) |
| **Window backend (mac)** | winemac.drv | winemac.drv + flicker patch | winemac.drv + 10+ Hack | hexa AppKit FFI (Phase 4+) |
| **D3D11/12** | wined3d (GL) / vkd3d-proton | mainline + a few patches | wined3d + DXVK + DXMT + **D3DMetal** vtable hook | hexa → Metal 직접 (Phase 5) |
| **MoltenVK 사용** | – | – | yes (DXVK→MoltenVK) | no (own 1 — Apple Metal 직접) |
| **32-bit on 64-bit mac** | wow64 (PE) | wow64 (PE) | wow64 (PE), 옛날 wine32on64 졸업 | x86_64 PE 만 (Phase 0~10), i386 미지원 |
| **App Nap 차단** | no | no | yes (Hack 14364) | own NSProcessInfo 호출 |
| **Battle.net Agent ACL forge** | no | no | **yes (Hack 23881)** | Phase 8 자체 reverse — algorithm direction reference |
| **Steam/Epic launcher quit IPC** | no | no | yes (Hack 22310/24199) | Phase 7 design TBD (own IPC) |
| **Rosetta cache invalidate** | no | no | yes (Hack 18947) | own (필요 시 hexa 직접) |
| **Dock title rewrite** | no | no | yes (Hack 13438) | own (Phase 4 AppKit FFI) |
| **own 1 정합** | ✗ (Wine source) | ✗ (Wine source) | ✗ (CrossOver = Wine fork) | **✓** (Apple-only + Microsoft docs) |

---

## 9. own 1 §A 정합 reference algorithm 목록

본 프로젝트가 *알고리즘 방향* 만 reference 가능한 항목 (code 0줄, spec/direction 만):

1. **Mach semaphore pool reuse** (msync §1.3) — pool 1024개, free-list, `os_unfair_lock` 보호. Apple `mach/semaphore.h` 직접 호출.
2. **`__ulock_wait2` fast path** (msync §1.3) — Apple private syscall 이지만 `dlsym` 으로 lookup 가능. fallback: `pthread_cond_t`.
3. **Mach message multi-wait registration** (msync §1.3) — server 가 Mach port 받아 임의 객체 signaled 시 reply. own server 는 Phase 3+ wineserver-equivalent (자체 작성).
4. **`force_backing_store` registry trick** (winemac no-flicker) — `kCGLPFABackingStore` attribute 강제. own metal layer init 시 동등 옵션.
5. **App Nap disable** — `[[NSProcessInfo processInfo] beginActivityWithOptions:NSActivityUserInitiatedAllowingIdleSystemSleep reason:...]`. 의도적 leak.
6. **Apple Silicon proc_translated check** (`sysctl.proc_translated`) — display mode safe-flag bypass / Rosetta cache toggle 분기.
7. **Mach-O `__TEXT,__info_plist` 동적 rewrite** (Hack 13438) — `CFBundleName` 을 process 시작 시 in-place 로 padding 영역 안에서 교체. own loader Phase 0c 에서 동등 처리.
8. **`mach_vm_write` 후 page exec bit toggle** (Hack 18947) — Rosetta translation cache invalidate. own JIT/cross-write path 에 동일 직렬.
9. **Battle.net Agent SD forging direction** (Hack 23881) — `is_battle_net_agent()` (basename 매칭) → 특정 path-pattern 시 SD 교체. 본 프로젝트 Phase 8: hexa shim 으로 reverse 후 *Microsoft `SECURITY_DESCRIPTOR` docs* 만 보고 직접 forge.
10. **Path-pattern 기반 env override** (Hack 24560) — exe path substring 매칭 후 `MVK_CONFIG_ADVERTISE_EXTENSIONS=0` 등 child env 주입. 본 프로젝트 Phase 7+ 에서 *어떤 detection 우회가 필요한가* 의 evidence.
11. **External quit IPC** (Hack 22310/24199) — `applicationShouldTerminate:` 받으면 child PID 에 IPC 로 forward. own AppKit FFI + IPC bridge 에서 직접.
12. **DACL whitelist / interactive SID** — `S-1-5-4` (INTERACTIVE), `S-1-5-32-544` (Admins), `S-1-5-32-545` (Users) — Microsoft docs 의 well-known SID 표 reference 가능.

---

## 10. 본 프로젝트 (own 1) 보존 라인

1. **CrossOver source 의 lift = 0**. 본 audit 의 모든 algorithm 은 spec / direction 만. 인용된 변수/symbol 이름 (`force_backing_store`, `is_apple_silicon`, `__CX_UNIX_*`) 은 *evidence* 로 인용된 것이며 hexa 모듈에 그대로 옮기지 않음.
2. **wine-staging patchset 0 import**. Patch 의 *효과 대상 버그* (예: bug 34166 fullscreen flicker) 만 reference.
3. **Microsoft 공식 docs 만 spec 출처**: PE/COFF, Win32 API, NT object ACL, `SECURITY_DESCRIPTOR` 구조, well-known SID.
4. **Apple 공식 docs**: AppKit / Cocoa, Mach kernel API, Metal, dispatch.
5. **commit boundary**: 본 audit 는 docs/research 에만 존재. lib/ 코드 변경 0.
6. **본 프로젝트 Phase 매핑**:
   - Phase 0a (PE parser) — 무관
   - Phase 0b (PE loader) — Hack 13438 (Info.plist rewrite) reference
   - Phase 1 (x64 ABI) — 무관
   - Phase 2-4 (Win32 shim) — Hack 22310/24199 (quit IPC) reference, Hack 14364 (App Nap)
   - Phase 5 (DirectX→Metal) — d3dmetal vtable evidence + 18775 FBO pitfall
   - Phase 7 (OSS 검증) — Hack 24560 (env override patterns) reference
   - Phase 8 (Battle.net) — **Hack 23881 algorithm direction 핵심**
   - Phase 9-10 (D2R/D4) — msync algorithm direction (sync hot-path)

---

## 11. 결론

CrossOver Mac 25 = (Wine 10.0 mainline) + (msync 통합 ≈3 000 LOC) + (winemac.drv 강화 27k LOC) + (D3DMetal vtable 0.4k LOC) + (게임/launcher 별 hack 27 파일 분산). own 1 (e) 는 *이 모든 것을 사용 금지* 한다. 본 audit 의 가치는 다음:

1. **algorithm direction reference**: msync (semaphore pool + ulock + Mach msg), Mach-O Info.plist rewrite, Apple Silicon `proc_translated`, App Nap 차단, Rosetta cache invalidate — 모두 Apple 공식 API 호출. own 정합.
2. **evidence sheet**: Battle.net Agent 가 SD 검증한다는 사실 자체. Steam/Epic launcher 가 IPC 로 child quit 을 forward 받아야 한다는 사실. Path of Exile 2 가 Vulkan extension 광고를 detect 한다는 사실. — 본 프로젝트 Phase 7~8 에서 *직접 reverse* 시 시간 절약.
3. **CrossOver 가 절대 들어오지 않는 영역** = airgenome-gamebox 의 raison d'être: hexa-only + Apple-only + Microsoft-docs-only + own 자체 작성. CrossOver 의 27 file 분산 hack 패턴은 long-term 으로 maintenance 비용 증가 — 본 프로젝트는 own 2 (emit / self_test / headless) enforcement 로 처음부터 통일된 codebase 유지.

**audit 산출물**: 본 문서 1개. lib/ 변경 0. tests/ 변경 0. own 1/2 정합 ✓.

---

*audit 작성: 2026-05-02. base: CrossOver 25.0.0 (sha256 b0f3c126…), Wine Staging 11.8 (commit 71e7a2e8). own 1 (Wine 0) + own 2 (analysis only, headless OK).*
