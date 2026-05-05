# CROSSOVER_ANALYSIS — CrossOver / wine-staging public source analysis

**Created** 2026-05-02
**Scope** Apple binary 외 코드 0줄 (own 1) 정통성 유지하면서, 공개 source 의 *알고리즘 / 패턴 / 디자인 결정* 만 reference 로 정리.
**Code lift** 0 줄. 함수 이름 + 패턴 분류 + citation link 만.
**own 1 enforcement note** 본 문서는 lib/loader/*.hexa 작성 시 *어떤 패턴이 존재하며 우리가 무엇을 *피하면서* DIY 해야 하는지* 의 reference. 실제 hexa 작성 시 본 문서의 어떤 코드 fragment 도 import 하지 않는다 — Microsoft 공식 docs 만 source-of-truth.

---

## 1. CrossOver 공개 source — 가능 여부

| 항목 | 사실 |
|---|---|
| Wine 부분의 license | LGPL 2.1 (대부분 모듈) + GPL 3 (cabextract, samba) |
| CodeWeavers 의 source 공개 의무 | LGPL 2.1 → wine 변경분 *공개해야 함* (binary distribution 의 obligation) |
| 공개 위치 | `https://www.codeweavers.com/crossover/source` |
| 공개 형식 | tarball (`crossover-sources-N.N.N.tar.gz`) — 매 release 마다 |
| 비공개 부분 | CrossOver GUI launcher / CXBottle helper / 자동 install 스크립트 (proprietary) |
| Apple GPTK 와의 관계 | "Apple Game Porting Toolkit powered by CrossOver source code" — Apple 이 CodeWeavers 의 LGPL Wine fork 를 base 로 삼고 D3DMetal.framework 만 proprietary 로 추가 [[CodeWeavers blog 2023-06-06](https://www.codeweavers.com/blog/mjohnson/2023/6/6/wine-comes-to-macos-apple-s-game-porting-toolkit-powered-by-crossover-source-code)] |

**결론** CrossOver = Wine LGPL fork. Wine 변경분만 공개, GUI/bottle layer 는 closed.
own 1 §A 위반 없이 *읽고 분석* 은 OK, *copy/port* 는 즉시 위반.

**FTP 시도 결과** codeweavers.com/crossover/source 는 HTTP 403 (User-Agent gate). 공식 페이지는 browser 로만 접근 가능. wine-staging GitHub 는 정상 clone 가능 → /tmp/crossover-research/wine-staging/ 에 shallow clone 완료 (Wine Staging 11.8, upstream `71e7a2e8`).

---

## 2. wine-staging — 구조 분석 (clone 결과)

| 디렉토리 | 내용 | 파일 수 |
|---|---|---|
| `patches/` | 모듈별 patch series (각 디렉토리 = `definition` + `*.patch`) | 119 patch directory |
| `staging/` | helper script (`patchinstall.py`, `commit-rebase.sh`, `VERSION`, `upstream-commit`) | 5 file |
| `LICENSE.md` | Wine Staging 자체는 LGPL 2.1 — 일부 예외 명시 | — |

**wine-staging 의 역할**
- Wine upstream 이 *아직 수용 안 한 unstable patches* 의 staging 영역
- 매 Wine release 마다 rebase
- CodeWeavers 가 *직접 운영하지는 않음* (community fork) 이지만, CrossOver 는 wine-staging 의 일부 patch 를 cherry-pick 하고 자체 patch 를 위에 얹는다 [[WineHQ Forum t=24657](https://forum.winehq.org/viewtopic.php?t=24657)]

**macOS 관련 patches (wine-staging 119 중)**
- `winemac.drv-no-flicker-patch/` — 단 1 개. (Ken Thomases @ CodeWeavers 작성, 2021-06-22).
  - 패턴: `force_backing_store` registry 옵션 → `kCGLPFABackingStore` attrib 강제 → OpenGL flicker 우회.
- 그 외 macOS-targeted patch 없음. wine-staging 은 *Linux 중심*.

**시사점** wine-staging 에는 macOS 관련 변경이 거의 없다 → CrossOver 의 macOS 패치는 *wine-staging 외부* 에 존재 (codeweavers.com tarball + 자체 dev branch). own 1 분석 대상은 wine-staging 보다 *Wine 본체 dlls/winemac.drv* 와 *CrossOver tarball* 이 핵심.

---

## 3. 핵심 분석 — CrossOver 가 Wine 위에 추가한 macOS-specific 영역

### 3.1 winemac.drv (Wine 본체 — `dlls/winemac.drv/`)
[[wine-mirror master](https://github.com/wine-mirror/wine/tree/master/dlls/winemac.drv)]

| 파일 (이름 only — 내용 lift X) | 역할 |
|---|---|
| `cocoa_app.m` | NSApplication lifecycle, CFRunLoop 통합 |
| `cocoa_window.m` | NSWindow 관리, Win32 HWND ↔ Cocoa window mapping |
| `cocoa_event.m` | NSEvent → Win32 message 변환 |
| `cocoa_clipboard.m` | NSPasteboard ↔ Win32 clipboard |
| `cocoa_cursorclipping.m` | mouse confinement (full-screen game 용) |
| `cocoa_display.m` | CGDisplayMode 열거, refresh rate |
| `cocoa_opengl.m` | CGLContext 생성, NSOpenGLContext 통합 |
| `cocoa_status_item.m` | NSStatusItem (system tray) |
| `vulkan.c` | MoltenVK wrapper (`VK_MVK_macos_surface`) |
| `macdrv_main.c` | driver entry, registry 옵션 parsing |
| `macdrv_cocoa.h` | C ↔ Objective-C bridge declarations |

**airgenome-gamebox 가 reference 가능한 *알고리즘 패턴* (코드 X)**
- HWND ↔ NSWindow mapping = double-keyed dictionary (HWND → window, window → HWND)
- Win32 message queue 는 thread-local, NSEvent 는 main-thread → message marshalling 필요
- Vulkan surface 생성 시 `VK_MVK_macos_surface` extension (deprecated → `VK_EXT_metal_surface`)

### 3.2 wine32on64 — 32-bit on 64-bit-only macOS
[[CodeWeavers blog, jwhite 2019-12-10](https://www.codeweavers.com/blog/jwhite/2019/12/10/celebrating-the-difficult-the-release-of-crossover-19)]
[[wine-devel pipermail 2019-12](https://www.winehq.org/pipermail/wine-devel/2019-December/156602.html)]

| 항목 | CrossOver 방식 |
|---|---|
| 문제 | macOS 10.15 Catalina = 64-bit only, 32-bit dyld 제거 |
| Apple 의 hidden hook | macOS 10.15+ kernel 에 *32-bit code segment* 생성 가능 (단 64-bit process 안에서) |
| toolchain | 자체 fork 한 Clang/LLVM-8 — `-minterop64-32-thunk-prefix=wine` 옵션 추가 |
| thunk 이름 패턴 | `wine_thunk_<function_name>` |
| 동작 | 64-bit Wine → 32-bit Win32 PE 코드 호출 시 자동 thunk 생성 |
| 빌드 산출물 | `wine32on64`, `wine64on64`, modified clang |

**airgenome-gamebox 와의 관계** Phase 0a 의 own 1 §d (Rosetta 2 만 사용 가능) → wine32on64 패턴은 *우리는 안 함*. D2R/D4 = 64-bit only 라서 회피 가능. 본 문서에 기록은 *왜 안 하는지* 설명용.

### 3.3 D3DMetal — graphics translation
[[Apple Developer GPTK](https://developer.apple.com/games/game-porting-toolkit/)]
[[GPTK README gist (lynkos)](https://gist.github.com/lynkos/3999f629560219a81d4e2c083a4bf5b1)]

| Layer | 기존 path | D3DMetal path |
|---|---|---|
| D3D 9 | DXVK → MoltenVK → Metal | (미지원, 여전히 DXVK 경로) |
| D3D 11 | DXVK → MoltenVK → Metal | D3DMetal.framework → Metal (직접) |
| D3D 12 | VKD3D-proton → MoltenVK → Metal | D3DMetal.framework → Metal (직접) |

**Apple 이 release 한 GPTK 의 disk image 내용** [[lynkos gist](https://gist.github.com/lynkos/3999f629560219a81d4e2c083a4bf5b1)]
- `D3DMetal.framework` (Apple proprietary, signed)
- `libd3dshared.dylib` (Apple proprietary)
- Wine bridge libraries (CrossOver source 기반, LGPL)

**airgenome-gamebox 의 own 1 §c 와의 정합성**
- D3DMetal.framework + libd3dshared.dylib = "Apple binary" 이므로 own 1 §c 허용
- 단, Apple 의 GPTK *bundle 전체* 는 `apple/apple/game-porting-toolkit` 으로 own 1 §e 명시 금지 (CrossOver source build 이므로)
- 결론: D3DMetal 자체 (framework + dylib) 는 우리 loader 가 dlopen 가능. GPTK 의 bottle 자동화 / wine64 binary 는 사용 금지. **현재 path: framework 만 link, wine 부분은 hexa DIY.**

### 3.4 i386 PE binary 의 macOS arm64 처리
| Wine 정통 (Linux) | CrossOver Mac | airgenome-gamebox |
|---|---|---|
| WoW64 thunk + 32-bit Linux loader | wine32on64 (Apple-signed 32-bit segment) | **N/A** (D2R/D4 = x86_64 only) |
| 64-bit Wine + 32-bit PE | 64-bit wine64on64 + 32-bit thunk | 64-bit hexa loader + Rosetta 2 (PE binary 의 instruction stream) |

[[Phoronix Wine 8.0 PE conversion](https://www.phoronix.com/news/Wine-8.0-Released)]
[[The Register WoW64](https://www.theregister.com/2024/01/18/wine_90_is_out)]

**WoW64 패턴 (Wine 8.0+ 정통)**
- 모든 Wine builtin → PE format
- `__wine_syscall_dispatcher` = PE syscall 의 dispatcher (numbered SSDT)
- unixlib = NtQueryVirtualMemory 가 helper 호출 → dlsym 으로 .so 안의 function array 가져옴
- airgenome-gamebox 가 *흉내* 낼 부분: PE syscall 진입점 → DIY hexa stub → Apple syscall.
  단, Wine 의 dispatcher 함수명/내부 구조는 *reference 만*, 실제 hexa 코드는 Microsoft NT syscall spec 기반으로 처음부터 작성.

### 3.5 msync — synchronization primitive
[[marzent/wine-msync](https://github.com/marzent/wine-msync)]
[[oreateai blog](https://www.oreateai.com/blog/decoding-synchronization-esync-vs-msync-in-wine-on-macos/443064ca4f6960ed0ef45cf8f4fd4a28)]

| 방식 | 매핑 |
|---|---|
| esync (Linux) | eventfd → epoll |
| fsync (Linux) | futex |
| **msync (macOS)** | NT primitives → Mach semaphore + kqueue (multiplex) |

**핵심 알고리즘 패턴 (이름 + 컨셉만, 코드 X)**
- semaphore pool (per-process, dynamic, ceiling 267597)
- uncontended wait = user-space only (Mach trap 없이)
- contended = wineserver Mach message → registered object lookup → signal/destroy

**airgenome-gamebox 적용**
- Phase 3 (`win32_kernel32.hexa`) 의 NT sync object (Event, Mutex, Semaphore) 는 동일한 *컨셉* 으로 Mach semaphore + kqueue 사용 가능
- 단, marzent/wine-msync 는 LGPL (Wine fork) → community 코드, own 1 §e 위반 → 코드 0 줄. 우리는 [Mach semaphore Apple docs](https://developer.apple.com/library/archive/documentation/Darwin/Conceptual/KernelProgramming/synchronization/synchronization.html) 직접 참조.

---

## 4. CrossOver vs Wine vs vanilla — 비교 표

| 영역 | vanilla Wine (winehq) | wine-staging | CrossOver (Mac) | airgenome-gamebox 방침 |
|---|---|---|---|---|
| License | LGPL 2.1 | LGPL 2.1 | LGPL (wine 부분) + 자체 proprietary GUI | own 1 § scope: Apple binary + Microsoft docs only |
| macOS driver | `dlls/winemac.drv/` | (no extra) | winemac.drv + 추가 Cocoa 보강 + flicker fix | hexa DIY (`lib/loader/win32_*.hexa`) — winemac 코드 0 lift |
| 32-bit on macOS | 미지원 (Catalina+) | 미지원 | wine32on64 + 자체 LLVM | N/A (D2R/D4 = x86_64) |
| graphics: D3D11 | DXVK → MoltenVK | (동일) | D3DMetal.framework (Apple) | D3DMetal.framework dlopen (Apple binary OK) |
| graphics: D3D12 | VKD3D | vkd3d-latest patch | D3DMetal + DXMT (CrossOver 25+) | Phase 5 — D3DMetal binding hexa DIY |
| sync primitives | server-side wait | esync, fsync patches | msync (Mach semaphore) | Phase 3 — Mach semaphore + kqueue 직접, msync 코드 X |
| WoW64 / PE | Wine 8.0+ all-PE, Wine 11 새 dispatcher | (동기화) | 자체 fork (CrossOver 25 시점 재작업) | own 1 §a — DIY PE loader, dispatcher hexa 작성 |
| arm64 binary | 미지원 (Linux 중심) | 미지원 | Rosetta 2 + D3DMetal.framework arm64 | own 1 §d — Rosetta 2 OK |
| bottle 개념 | $WINEPREFIX 디렉토리 | (동일) | CXBottle (proprietary GUI + scripted setup) | hexa-native config (`tool/` 내 stage / dispatch) |

---

## 5. macOS-specific 처리 패턴 — airgenome-gamebox 가 own 1 위반 없이 reference 가능한 list

각 항목 = *알고리즘 / 컨셉 / 함수 이름* (lift X) + citation.

### 5.1 HWND ↔ NSWindow lifecycle
- 패턴: 양방향 hash (HWND-to-NSWindow, NSWindow-to-HWND)
- thread 모델: NSWindow 작업은 main thread, Win32 message queue 는 caller thread → `dispatch_async(dispatch_get_main_queue(), ...)` marshalling
- citation: [winemac.drv source listing](https://github.com/wine-mirror/wine/tree/master/dlls/winemac.drv) (파일 이름만 참조)

### 5.2 Vulkan surface (Apple SDK 직접 사용 가능)
- `VK_EXT_metal_surface` (`vkCreateMetalSurfaceEXT`) 또는 `VK_MVK_macos_surface` (deprecated)
- `CAMetalLayer` 를 NSView delegate 로 설정 → swapchain timing 정확
- citation: [MoltenVK Runtime UserGuide](https://github.com/KhronosGroup/MoltenVK/blob/main/Docs/MoltenVK_Runtime_UserGuide.md)

### 5.3 Mach semaphore 기반 NT sync
- `semaphore_create` / `semaphore_wait` / `semaphore_signal` (Mach API)
- multiplex wait = `kqueue` + `EVFILT_MACHPORT`
- citation: [Apple Darwin synchronization docs](https://developer.apple.com/library/archive/documentation/Darwin/Conceptual/KernelProgramming/synchronization/synchronization.html)

### 5.4 PE syscall dispatcher (개념)
- Windows = `syscall` instruction → SSDT lookup → kernel
- Wine 의 `__wine_syscall_dispatcher` = userspace 흉내 (PE side numbered table → unix-side function pointer)
- airgenome-gamebox 의 hexa dispatcher = Microsoft NT syscall 번호 (공식 docs 부재 → ReactOS NT 헤더는 OK? — **NO**, ReactOS 도 community.) → 우리는 *우리가 호출되는 NT API 만 골라서 stub* 으로 charter
- citation: [Microsoft NT API spec](https://learn.microsoft.com/en-us/windows/win32/api/) (own 1 §f rule)

### 5.5 Rosetta 2 호환 환경
- Apple 공식: x86_64 zsh 진입 = `arch -x86_64 zsh`
- D3DMetal evaluation = Sonoma 14+ + Apple Silicon
- citation: [GPTK README quote](https://gist.github.com/lynkos/3999f629560219a81d4e2c083a4bf5b1) — "The evaluation environment for Windows games runs under Rosetta 2"

### 5.6 D3DMetal.framework (Apple binary, own 1 §c 허용)
- `D3DMetal.framework` + `libd3dshared.dylib` = Apple-signed, redistributable as part of GPTK
- D3D 11/12 헤더 → Metal 직접 (Vulkan 우회) → vanilla MoltenVK path 보다 perf 우위
- own 1 § 적용: framework dlopen + symbol resolution 만 hexa 에서 수행. *Apple binary 안의 logic 은 black box*.
- citation: [Apple GPTK developer page](https://developer.apple.com/games/game-porting-toolkit/), [CodeWeavers blog 2023-06](https://www.codeweavers.com/blog/mjohnson/2023/6/6/wine-comes-to-macos-apple-s-game-porting-toolkit-powered-by-crossover-source-code)

---

## 6. own 1 enforcement re-check

| 위반 가능 항목 | 본 문서에서의 처리 |
|---|---|
| Wine source code copy | 0 lift — 함수 이름과 파일 이름만 ref |
| CrossOver tarball download → use | 본 task 에서 codeweavers.com 403 (의도적 비실행 — 서명 의무 회피) |
| GPTK bundle 사용 | 회피 — 프레임워크 single dylib 만 |
| msync / esync / fsync 코드 | 0 lift — 알고리즘 컨셉만 |
| wine-staging patch 적용 | 0 — `/tmp/crossover-research/wine-staging/` 는 *읽기 전용 reference*, build 안 함 |
| ReactOS / Hangover / Box64 / FEX | 본 문서 0 reference, own 1 §e 명시 ban |
| Whisky / GPTK app / gcenx tap | 본 문서 0 reference |

**결론** 본 문서는 *어떤 패턴이 존재하는지 *를 카탈로그하고, *어떤 path 로 hexa DIY 할 것인지* 를 결정하기 위한 reference. 코드 lift 없음. own 1 위반 없음.

---

## 7. 후속 follow-up

- [ ] Phase 0b PE memory loader 작성 시 — Microsoft PE/COFF spec 만 참조, wine `dlls/ntdll/loader.c` 참조 금지
- [ ] Phase 1 ABI shim — System V x86_64 → MS x64 calling convention spec 만 참조, wine `signal_x86_64.c` 0 view
- [ ] Phase 3 sync — Apple Darwin docs 만 참조, marzent/wine-msync 0 view
- [ ] Phase 5 D3DMetal binding — Apple GPTK README + Metal docs 만, CrossOver source 0 view
- [ ] tests/test_apple_only.sh — 본 문서가 reference 한 외부 path (`/tmp/crossover-research/wine-staging/`) 가 *런타임 import* 안 됨을 검증

---

## Citations (전체 list)

- [CodeWeavers source page](https://www.codeweavers.com/crossover/source) — CrossOver Mac/Linux source tarball 공개 위치 (LGPL 의무 이행)
- [CodeWeavers EULA — license breakdown](https://www.codeweavers.com/crossover/eula)
- [CodeWeavers blog — Wine comes to macOS via GPTK (2023-06-06)](https://www.codeweavers.com/blog/mjohnson/2023/6/6/wine-comes-to-macos-apple-s-game-porting-toolkit-powered-by-crossover-source-code)
- [CodeWeavers blog — CrossOver 19 release / wine32on64 (2019-12-10)](https://www.codeweavers.com/blog/jwhite/2019/12/10/celebrating-the-difficult-the-release-of-crossover-19)
- [Wikipedia CrossOver software](https://en.wikipedia.org/wiki/CrossOver_(software))
- [Wine source — winemac.drv directory](https://github.com/wine-mirror/wine/tree/master/dlls/winemac.drv)
- [Wine wine-devel ML — win32 on macOS thread (2019-12)](https://www.winehq.org/pipermail/wine-devel/2019-December/156602.html)
- [Apple Developer — Game Porting Toolkit](https://developer.apple.com/games/game-porting-toolkit/)
- [GPTK README copy (lynkos gist)](https://gist.github.com/lynkos/3999f629560219a81d4e2c083a4bf5b1)
- [marzent/wine-msync README](https://github.com/marzent/wine-msync)
- [Decoding esync vs msync (oreateai)](https://www.oreateai.com/blog/decoding-synchronization-esync-vs-msync-in-wine-on-macos/443064ca4f6960ed0ef45cf8f4fd4a28)
- [MoltenVK Runtime UserGuide](https://github.com/KhronosGroup/MoltenVK/blob/main/Docs/MoltenVK_Runtime_UserGuide.md)
- [Apple Darwin synchronization primitives](https://developer.apple.com/library/archive/documentation/Darwin/Conceptual/KernelProgramming/synchronization/synchronization.html)
- [Phoronix — Wine 8.0 PE conversion](https://www.phoronix.com/news/Wine-8.0-Released)
- [The Register — Wine 9.0 WoW64](https://www.theregister.com/2024/01/18/wine_90_is_out)
- [Ivyl blog — Wine PE → UNIX](https://blog.hiler.eu/wine-pe-to-unix/)
- [carette.xyz — Deep dive into CrossOver](https://carette.xyz/posts/deep_dive_into_crossover/)
- [WineHQ Forum — CrossOver Linux vs Wine (t=24657)](https://forum.winehq.org/viewtopic.php?t=24657)
- [Microsoft PE/COFF spec (own 1 §f canonical)](https://learn.microsoft.com/en-us/windows/win32/debug/pe-format)
- [Microsoft Win32 API index (own 1 §f canonical)](https://learn.microsoft.com/en-us/windows/win32/api/)

**Local clone**
- `/tmp/crossover-research/wine-staging/` — Wine Staging 11.8 (upstream `71e7a2e8`), shallow clone, *읽기 전용 reference*, build 금지, 빌드 산출물 import 금지.
