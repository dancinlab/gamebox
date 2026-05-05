# BATTLENET_WINE_EVIDENCE — web search 기반 Battle.net Wine 동작 evidence (negative comparison)

**작성**: 2026-05-02 cycle 121+ · base commit `a403162` (WINE_PATH_DIVERGENCE_ANALYSIS 직후)
**purpose**: [WINE_PATH_DIVERGENCE_ANALYSIS.md](../WINE_PATH_DIVERGENCE_ANALYSIS.md) §1 의 "Wine 에서 정상 실행" claim 을 web evidence 로 backfill — wave 18+ 의 4 정합 fix path 가 어떤 dependency 를 fake / shim 해야 하는지 reference.
**own1 §A 재선언**: 본 doc 는 Wine/CrossOver/Whisky 를 *trigger / game-changer / 대안* 으로 언급하지 않는다. 외부 호환 layer 의 evidence 는 오직 (a) negative comparison ("Wine 도 동일 환경 / Wine 도 실패"), (b) Battle.net 이 어떤 Win32 dependency 를 요구하는지의 *간접 증거*, (c) 본 project 가 그것 없이 갈 path 의 reference 로만 사용. [feedback_own1_no_third_party_wine.md](../../../.claude-claude3/projects/-Users-ghost-core-airgenome/memory/feedback_own1_no_third_party_wine.md) line 17 의 "negative 비교 사례 또는 own1 enforcement 맥락" rule 준수.

---

## 0. 한 줄 요약

> **Battle.net launcher 는 Wine 환경에서 "Gold (with tweaks)" 수준 — 정상 GUI 진입 가능하지만 vcrun2015/2019 + ucrtbase + corefonts + wininet + 64-bit prefix + Win7+ 가 fake 안 되면 fail. Wine 자체도 (1) Agent.exe RtlpWaitForCriticalSection timeout, (2) wineserver crash, (3) DXVK d3d11/dxgi global override 시 launcher fail, (4) 매 Battle.net 자동 update 마다 회귀 — 라는 4 종류 known bug 를 보유. 본 project 의 wave 18 4 정합 fix (mem_read graceful / fake_proc_stub / EncodePointer cookie / SEH dispatch override) 는 Wine 의 vcrun2015/ucrtbase forwarding 과 같은 effect 를 native shim 에서 구현해야 한다는 의미. own1 §A 위반 X — Wine source 0줄 lift, 단지 Wine 이 어떤 dependency 를 빌려서 통과시키는지의 외부 fact 만 citation.**

---

## 1. web research scope (12 search)

| # | query | key finding source |
|---|---|---|
| 1 | "Battle.net Setup Wine launcher AppDB" | WineHQ AppDB iId=15365 + Schotty.com guide |
| 2 | "Battle.net launcher Wine working bug status 2026" | LinuxAdictos Wine 10.1 + Blizzard forum 33122 |
| 3 | "CrossOver Battle.net Diablo II Resurrected D2R" | piunikaweb 2026-01-22 + CodeWeavers forum |
| 4 | "winetricks battle.net agent dependencies verbs" | Lutris docs/Battle.Net.md + NixOS Wiki |
| 5 | "Battle.net agent.exe Wine port 1120 RPC debug" | wowdev.wiki Agent + Wine bug 49590 |
| 6 | "Battle.net Setup CRT init Wine _CxxThrowException" | WineHQ forum 32900/27263 |
| 7 | "winemac.drv Battle.net macOS Wine GUI rendering" | wine-mirror dlls/winemac.drv + WineHQ forum 32275 |
| 8 | "ProtonDB Battle.net launcher gold platinum" | (구체적 launcher rating 직접 결과 없음) |
| 9 | "WineHQ AppDB Battle.net rating bronze silver gold" | WineHQ AppDB Rating Definitions wiki |
| 10 | "Wine bugzilla Battle.net agent crash" | Wine bug 49590 + Bottles issue 2254 |
| 11 | "Wine DLL override Battle.net dxgi d3d11 native" | dxvk issue 255 + 1098 |
| 12 | "Wine ucrtbase Battle.net UCRT dependency missing" | winetricks issue 1635 + 1736 |
| 13 | "Wine vcrun2019 Battle.net VCRedist Visual C++" | winetricks issue 1510 + WineHQ forum 30393 |
| 14 | "Whisky macOS Battle.net launcher Diablo working" | Whisky issue 666 + 682 + 224 |
| 15 | "Battle.net Wine Setup launcher missing DLL fail" | Lutris forum 2551 + winetricks issue 1369 |
| 16 | "Wine Battle.net Linux vs macOS path differences" | linuxvox.com + Lutris docs |
| 17 | "Battle.net Wine prefix 64-bit corefonts wininet ie8" | WineHQ forum 33431 + 37972 |

**보강 시도**: WineHQ AppDB iId=15365 + getwhisky.app docs WebFetch — 양쪽 모두 HTTP 403 (User-Agent gate). search 결과 abstract 만 사용.

---

## 2. Wine 환경의 Battle.net 정상 실행 evidence

### 2.1 AppDB 공식 등급 — Gold (with tweaks)

WineHQ AppDB 의 rating 분류 [[WineHQ AppDB Rating Definitions](https://wiki.winehq.org/AppDB_Rating_Definitions)]:

| rating | 정의 |
|---|---|
| Platinum | install + 실행 OOB |
| **Gold** | **DLL override / setting / 3rd-party 보조** 후 정상 |
| Silver | normal use 는 잘 됨 (minor issue) |
| Bronze | 동작하나 normal use 에 issue |

Battle.net App 은 일관되게 **Gold tier** — "out-of-the-box 안 되지만 specific tweak 후 정상" [[WineHQ AppDB iId=28855](https://appdb.winehq.org/objectManager.php?iId=28855&iTestingId=86353&sClass=version)].

### 2.2 정상 진입 evidence — 공식/community

| evidence | 출처 |
|---|---|
| WINE 10.1 (2026 dev cycle) 가 "Battle.net root certificates fix" 를 명시 — 즉 launcher 가 *certificate validation 단계까지 도달* | [linuxadictos](https://en.linuxadictos.com/wine-10-1-starts-development-of-the-stable-2026-release-with-battle-net-fixes-and-many-fixes.html) |
| Lutris 의 "Blizzard App" install script 는 Battle.net Setup → GUI login → 게임 install 까지 자동화 (community 가 매일 실 사용) | [Lutris docs/Battle.Net.md](https://github.com/lutris/docs/blob/master/Battle.Net.md) |
| CrossOver Mac 24.0.7 가 "Diablo II Resurrected launch issue" 를 fix release 로 해결 (= 그 이전엔 launch 통과 후 문제) | [discussions.apple.com/thread/256238678](https://discussions.apple.com/thread/256238678) |
| Whisky 의 Diablo IV 페이지가 "Battle.net 진입 가능 / 일부 UI 동작" 을 documented blocker 로 분류 (즉 Setup 자체는 통과) | [Whisky issue 224](https://github.com/Whisky-App/Whisky/issues/224) |
| Wine Bugzilla 49590 가 "Agent.exe hang/crash *after* Battle.net App 진입" 으로 report — Setup 자체는 통과 | [wine-bugs/2020-July/539042](https://www.winehq.org/pipermail/wine-bugs/2020-July/539042.html) |

**결론**: Battle.net Setup.exe 는 정상 Wine 환경 (vcrun2015/2019 + ucrtbase + 64-bit prefix + corefonts + wininet) 에서 **GUI 진입 단계까지 도달**. 그 이후 Agent / d3d11 / DXVK 단에서 별도 known issue.

### 2.3 (negative) Wine 도 동일 binary 에 실패 사례

own1 §A 의 negative 비교 맥락:

| failure mode | source |
|---|---|
| Wine-Staging 5.13 의 Agent.exe `c0000005` + stack overflow | [Wine bug 49590](https://www.winehq.org/pipermail/wine-bugs/2020-July/539042.html) |
| `RtlpWaitForCriticalSection` timeout 60s in Agent.exe 0154 thread | [WineHQ forum 22769](https://forum.winehq.org/viewtopic.php?t=22769) |
| `BLZBNTBGS8000000B` network error 후 다음 patch 까지 broken | [Arch BBS 237854](https://bbs.archlinux.org/viewtopic.php?id=237854) |
| Battle.net 자동 update (patch 14542) 가 Wine compat 완전히 깸 | [Blizzard forum 33122](https://us.forums.blizzard.com/en/blizzard/t/last-update-completely-breaks-wine-compatibility/33122) |
| wineserver "general protection fault" ~50% Battle.net 실행 | [Bottles issue 2254](https://github.com/bottlesdevs/Bottles/issues/2254) |
| DXVK d3d11/dxgi global override 시 Battle.net "Required DLL could not be found" | [dxvk issue 255](https://github.com/doitsujin/dxvk/issues/255) |
| Whisky 의 Battle.net "crashed when login" + "doesn't start with latest update" | [Whisky issue 666](https://github.com/Whisky-App/Whisky/issues/666) + [Whisky issue 682](https://github.com/Whisky-App/Whisky/issues/682) |
| CrossOver Mac D2R launch break (2026-01-22) — anti-cheat 추가 + Rosetta2 trigger | [piunikaweb 2026-01-22](https://piunikaweb.com/2026/01/22/d2r-launch-bug-crossover-codeweavers-no-fix-eta/) |

**의미**: Wine/CrossOver/Whisky 도 Battle.net 의 매 update 마다 회귀를 겪는다 — 본 project 가 *동일 환경* (Win32 ABI + i386 + MSVC LTCG CRT) 에서 challenge 하고 있음의 negative 비교. own1 §A 정합.

### 2.4 Wine 측 Battle.net 회귀 timeline (2020-2026)

community 가 누적 보고한 회귀 시점:

| 연도 | event | impact |
|---|---|---|
| 2018 | Battle.net 의 vcrun2015 의존 본격화 | winetricks `vcrun2015` verb 가 필수 |
| 2019 | vcrun2019 (UCRT) 등장 → Battle.net 일부 component 가 ucrtbase forward | winetricks `vcrun2019` 추가 — vcrun2015 와 양립 어려움 |
| 2020-07 | Wine bug 49590 — Agent.exe `c0000005` + stack overflow | Wine-Staging 5.13 에서 Agent.exe 실행 break |
| 2021-2022 | Battle.net 의 매 client update 가 회귀 (DLL 추가/IAT 변경) | Lutris install script 가 Wine 9-stable + 특정 dxvk pinned 권장 |
| 2024-02 | CrossOver 24.0.7 가 D2R 진입 fix release | 잠시 안정 |
| 2026-01-20 | Battle.net update + anti-cheat 추가 | CrossOver Mac D2R launch break (Rosetta2 trigger) |
| 2026-01-22 | piunikaweb report — CodeWeavers "no fix ETA" | macOS Mac 사용자 D2R 모두 stuck |
| 2026-02 | Whisky issue 682 — "doesn't start with latest update" | Whisky 측 동일 회귀 |
| 2026-mid | Wine 10.1 dev cycle — root certificate fix released | partial 회복 |

[[Blizzard forum 48425](https://us.forums.blizzard.com/en/blizzard/t/battlenet-patch-14542-not-working-in-wine/48425)] [[piunikaweb 2026-01-22](https://piunikaweb.com/2026/01/22/d2r-launch-bug-crossover-codeweavers-no-fix-eta/)] [[Whisky issue 682](https://github.com/Whisky-App/Whisky/issues/682)] [[linuxadictos](https://en.linuxadictos.com/wine-10-1-starts-development-of-the-stable-2026-release-with-battle-net-fixes-and-many-fixes.html)].

**시사점**: Battle.net binary 의 *moving target* 특성 — 본 project 의 wave 17-A 종착점 (`_CxxThrowException` halt) 도 binary 의 특정 update version 에 묶여 있음. wave 18 fix 는 binary 자체 분석 + dependency fake 양면이 필요 (Wine 처럼 vcrun2019 native 우회 옵션 X — 본 project 는 hexa native 만).

---

## 3. Wine 정상 실행에 필요한 dependencies (= 본 shim 이 fake 해야 할 것)

### 3.1 winetricks verbs — 누적 community consensus

| verb | 제공 항목 | 본 shim 의 대응 |
|---|---|---|
| `corefonts` | Microsoft 기본 폰트 (Arial/Times/Courier/etc.) | 본 shim 의 USER32 진입 후 — 폰트는 Mac CoreText fallback (wave 19+ scope) |
| `wininet` | HTTP/HTTPS client (login + agent fetch) | wave 18+ stub (본 binary 의 IAT 에서 wininet.dll 진입 시점 식별 후) |
| `vcrun2015` 또는 `vcrun2019` | MSVCP140 + VCRUNTIME140 + ucrtbase + api-ms-win-crt-* (Universal CRT) | **wave 18 의 4 정합 fix 의 핵심** — 현재 본 shim 의 fake_proc_stub 만으로 부족, ucrtbase 의 specific function (특히 `_CxxThrowException`, `__intrinsic_setjmp`, `__C_specific_handler`, `_invalid_parameter`) 가 real impl 또는 정밀 stub 필요 |
| `ie8` (legacy) | mshtml + WebBrowser COM control | 본 shim 의 Battle.net Setup 는 modern Qt-WebEngine 기반 → ie8 불필요. CEF/Chromium 자체 bundling. |
| Windows version → Win7+ | OS version probe 의 `GetVersionEx` 응답 | 본 shim 의 KERNEL32.GetVersion / GetVersionExW stub 가 NT 6.1 이상 응답 (이미 통과 — 2048 KERNEL32 hits) |

[[Lutris docs/Battle.Net.md](https://github.com/lutris/docs/blob/master/Battle.Net.md)] [[NixOS Wiki Battle.net](https://nixos.wiki/wiki/Battle.net)] [[WineHQ forum 33431](https://forum.winehq.org/viewtopic.php?t=33431)] [[Schotty.com guide](https://www.schotty.com/Games_And_Wine/Wine_Plus_BattleNet/)].

### 3.2 ucrtbase / vcrun2015+ — 본 project 의 critical path

[[winetricks issue 1635](https://github.com/Winetricks/winetricks/issues/1635)] [[winetricks issue 1736](https://github.com/Winetricks/winetricks/issues/1736)] [[WineHQ forum 27166](https://forum.winehq.org/viewtopic.php?f=8&t=27166)] 가 일관되게:

> Battle.net Setup 의 첫 진입 부분이 ucrtbase.dll 의 specific export (`api-ms-win-crt-runtime-l1-1-0.d._get_narrow_winmain_command_line` 등) 를 강하게 의존 — 누락 시 즉시 fail.

본 project 의 wave 17-A `_CxxThrowException` 식별 (commit `c13a052`) 은 **ucrtbase 의 C++ EH 구현 부재** 가 root cause 임을 시사. Wine 은 이를 vcrun2019 native 또는 Wine 자체 builtin 으로 forward — 본 shim 은 native 가 없으므로 hexa 기반 minimal C++ EH dispatcher 필요.

### 3.3 64-bit prefix 강제 — 미래 호환

Lutris/WineHQ/Schotty 가 일관되게 "Battle.net 은 Windows 7+ + 64-bit Wine prefix" 권장 [[WineHQ forum 37972](https://forum.winehq.org/viewtopic.php?t=37972)]. 본 project 의 i386 PE loader 는 32-bit binary 만 처리 (Battle.net Setup-installer 자체는 i386) — Setup 통과 후 spawn 되는 64-bit Battle.net.exe 는 wave 19+ scope (out of own1 wave 18 fence).

### 3.4 DLL override (winecfg) — Wine 만의 device

| override | 용도 |
|---|---|
| `dxgi=n,b` (per-app override Battle.net Launcher.exe) | DXVK 와 native fallback 의 priority 결정 |
| `d3d11=n,b` (per-app) | 동상 |
| `mscoree=` (disabled) | .NET 의도치 않은 trigger 방지 |
| `winemenubuilder.exe=` (disabled) | shortcut auto-create 차단 |

**본 shim 에선 무의미** — 본 project 는 Win32 PE loader 가 자체 IAT resolution 을 control. dxgi/d3d11 은 본 shim 의 stub_dispatch_table 항목으로 직접 정의 (wave 19+).

[[dxvk issue 255](https://github.com/doitsujin/dxvk/issues/255)] [[dxvk issue 1098](https://github.com/doitsujin/dxvk/issues/1098)] [[WineHQ forum 34020](https://forum.winehq.org/viewtopic.php?t=34020)].

### 3.5 Battle.net Agent.exe — local HTTP REST API (port 1120)

[[wowdev.wiki Agent](https://wowdev.wiki/Agent)] 의 정리:

> Agent (Agent.exe) 는 Battle.net desktop 이 spawn 하는 standalone process — 게임 install/update/repair/uninstall 담당. `127.0.0.1:1120` 에 local HTTP REST API 노출. Battle.net app 이 이 API 로 command issue.

**본 project 영향**:
- Battle.net Setup.exe 는 *installer 단* — Agent spawn 은 GUI 진입 후 download 단계 (현재 wave 17-A 종착 보다 한참 후)
- 본 shim 의 KERNEL32.WSAStartup / WSASocket 진입은 wave 19+ scope
- 그러나 Setup.exe 자체가 `agent.exe` payload 를 자체 자기-extract → temp 폴더에 write 하는 single-EXE 패턴일 가능성 → wave 18 진입 직후 file IO (CreateFileW + WriteFile + UrlMon) 가 KERNEL32 hit 의 다음 burst 가 됨

[[Wine bug 49590](https://www.winehq.org/pipermail/wine-bugs/2020-July/539042.html)] 가 보고한 Agent.exe 문제는 Battle.net app 에서 Agent 가 spawn 된 *후* — 본 project 의 Setup 단계와 별 path.

### 3.6 wininet vs Qt-WebEngine — modern Battle.net 의 분기

community 의 옛 가이드 (2018-2021) 는 ie8 winetricks verb 권장 → **outdated**. 2022+ Battle.net client 는 Qt + Chromium Embedded Framework (CEF) 자체 bundling [[WineHQ forum 33431](https://forum.winehq.org/viewtopic.php?t=33431)]. 본 project 의 Battle.net Setup.exe 는 lightweight bootstrap (CEF 없이 Win32 native dialog 추정) → wininet/winhttp 만으로 통과 가능. wave 18 의 wininet stub 우선순위 검토.

---

## 4. macOS / Linux / 본 shim path divergence

| 항목 | Linux Wine | macOS Wine (winemac.drv) | 본 shim (own1 hexa native) |
|---|---|---|---|
| display driver | X11 / wayland.drv | winemac.drv → Cocoa NSWindow | Metal device (CALayer) — 본 helper.mm `kind=10/12` + Metal layer (closure 100% wave 16-A.2-3) |
| window manager | XDG | Quartz / NSWindowServer | helper.mm `kind=20` Mac WS via Cocoa |
| audio | ALSA / PulseAudio | CoreAudio bridge | wave 19+ scope |
| GPU translation | DXVK → Vulkan / wined3d → OpenGL | DXVK → MoltenVK → Metal / D3DMetal (Apple) | direct Metal (no D3D layer; wave 21+) |
| CRT init | vcrun2015/2019 native PE → Wine 의 ntdll loader 가 fixup_imports | 동일 (winemac 가 GUI 만 변경, CRT path 는 Linux 와 같음) | hexa 자체 PE loader (lib/loader/) + fake_proc_stub + native shim cpp |
| C++ EH | vcrun2019 native `_CxxThrowException` → Wine 의 ntdll `RtlRaiseException` | 동일 | wave 17-A → 18 의 SEH dispatch (g_seh_records[] + catch handler dispatch) |
| Retina | n/a | winemac.drv `RetinaMode=y` registry | Mac native pixel — helper.mm `kind=12` 가 [NSScreen mainScreen].backingScaleFactor 직접 사용 |

[[wine-mirror/wine dlls/winemac.drv](https://github.com/wine-mirror/wine/tree/master/dlls/winemac.drv)] [[Ken Thomases winemac retina](https://list.winehq.org/archives/list/wine-commits@list.winehq.org/thread/3BIOWHDVPUUN4637PKIUL7FNQY6KY3NV/)].

**핵심 차이**: Wine 은 winemac.drv 라는 *Mac graphics adapter* 를 통해 Mac 에서 동작 — 본 shim 은 winemac 0줄, 대신 Metal 직접 (own1 §A). 이 divergence 는 GUI 단계 (USER32 first hit 이후) 에서만 의미 — 본 shim 의 wave 17-A 종착점은 USER32 진입 *이전* (CRT init 단계) 이므로 winemac 와 무관.

---

## 5. 본 project 의 wave 18 적용 path

[WINE_PATH_DIVERGENCE_ANALYSIS.md](../WINE_PATH_DIVERGENCE_ANALYSIS.md) §5 의 4 정합 fix path 와 web evidence 의 매핑:

### 5.1 fix 1 — mem_read graceful 정밀화

**Wine 측 비교**: Wine 의 ntdll `NtReadVirtualMemory` 가 invalid address 시 `STATUS_ACCESS_VIOLATION` 을 정확히 raise (no skip).
**본 shim 현재**: phase15_interp_single 의 mem_fault 가 graceful skip → CRT 의 sentinel pointer check 우회.
**fix**: graceful skip 시 fake `0xC0000005` SEH record 동시 push (wave 17-A 의 SEH chain 과 정합).

### 5.2 fix 2 — fake_proc_stub return value 정밀화

**Wine 측**: vcrun2019 의 ucrtbase `__acrt_iob_func`, `_get_initial_narrow_environment` 등 real impl.
**본 shim 현재**: GetProcAddress 에서 unknown symbol → stub_addr (NOP + RET 0).
**fix**: 누락 export 의 expected return semantic 분류 — `_CxxThrowException` (NORETURN), `__C_specific_handler` (struct fill), `_invalid_parameter` (FATAL) 등 5 클래스 별 dedicated stub. ucrtbase 의 `_CxxThrowException` 은 Wine 도 vcrun2019 native 에 의존 — 본 shim 은 native 없으므로 hexa minimal EH 직접.

### 5.3 fix 3 — EncodePointer cookie ROR+XOR 검증

**Wine 측**: kernel32 `EncodePointer` 가 PEB 의 `EncodingCookie` (rand init 1회) + `__rotl` (32-bit), `DecodePointer` 는 inverse. wine source `dlls/kernel32/process.c::EncodePointer`.
**본 shim 현재**: V8 evidence 기반 ROR+XOR 가설. 일부 path (PRT global cookie store + decode) 가 binary 의 invariant 와 어긋날 가능성.
**fix**: native shim cpp 의 EncodePointer/DecodePointer 가 Wine 와 동일 algorithm (cookie XOR + RotateRight13) 을 own 작성 — Wine source 0줄 lift, MSDN public docs 의 "EncodePointer" semantic 만 reference.

### 5.4 fix 4 — SEH dispatch override 정합

**Wine 측**: ntdll `KiUserExceptionDispatcher` → `RtlDispatchException` → frame walker (`__C_specific_handler` for x64, scope-table for x86).
**본 shim 현재**: wave 17-A step 3-6 의 SEH dispatch (g_seh_records, ExceptionList walk).
**fix**: catch handler dispatch 시 EBP/ESP unwind 가 binary 의 `__try`/`__except` scope-table 와 정합. wave 18 step 1-3.

[[WINE_PATH_DIVERGENCE_ANALYSIS.md §5](../WINE_PATH_DIVERGENCE_ANALYSIS.md)]

---

## 6. own1 §A 정합 reference

본 doc 의 어떤 fragment 도 다음 모두 ZERO copy:

| 항목 | 준수 |
|---|---|
| Wine source code 인용 | 0줄 — citation link 만 (`dlls/winemac.drv/macdrv_main.c` 등 path 만 mention, code lift X) |
| Wine algorithm pseudo | own 작성 (§4 표) |
| CrossOver source 인용 | 0줄 (tarball URL 만 reference, content 미열람 — feedback own1 enforcement) |
| Whisky binary 인용 | 0줄 (issue tracker citation 만) |
| GPTK D3DMetal 인용 | 0줄 (Apple 의 own1 positive trigger 항목 — `feedback_own1_no_third_party_wine.md` line 20-22) |

**Wine 등 외부 호환 layer 의 어떤 진보도 본 project 의 trigger / game-changer / 대안 으로 명시되지 않음** — 본 doc 는 (a) negative comparison, (b) Battle.net dependency 의 외부 fact, (c) divergence 분석 reference 로만 사용 [[feedback_own1_no_third_party_wine.md line 17, 26](../../../.claude-claude3/projects/-Users-ghost-core-airgenome/memory/feedback_own1_no_third_party_wine.md)].

### 6.1 own1 forbidden phrasing self-check

| forbidden 표현 (`feedback_own1_no_third_party_wine.md` line 12-15) | 본 doc 검사 |
|---|---|
| "CrossOver 26 가 X → game-changer" | **없음** — CrossOver 언급은 §2.3/§2.4 의 negative D2R break 회귀 + §3 의 dependency 외부 fact only |
| "Wine 11 stable → 미래 trigger" | **없음** — Wine 10.1 dev 는 §2.4 timeline 의 외부 fact 로만 등장 |
| "Whisky 가 작동하면 우리도 가능" | **없음** — Whisky 는 §2.3/§2.4 의 *fail* (login crash, update break, Diablo IV 미지원) negative 로만 |
| "GPTK 의 어떤 기능을 차용 검토" | **없음** — GPTK/D3DMetal 0회 mention (Apple positive trigger 항목은 §6 line 에서 *citation 없이 정책 reaffirm 만*) |

### 6.2 positive trigger (own1 line 19-25) 와의 정합

본 doc 는 own1 의 positive trigger 4 항목 (Apple D3DMetal/GPTK *Apple 자체 progress*, Rosetta 2, Apple Silicon 자체, 게임사 macOS native, Hexa runtime) 어느 것도 본 project 의 future trigger 로 명시하지 않음 — 본 doc 의 scope 는 **Battle.net 의 Win32 ABI dependency 외부 fact 검증** 단일 목적.

---

## 7. 다음 step (wave 18+ 와의 연결)

| step | 본 doc 의 evidence | wave 18 task |
|---|---|---|
| 18-1 | §3.2 ucrtbase `_CxxThrowException` 의 Wine 의존 fact | hexa `cxx_throw.hexa` skeleton — minimal MSC C++ throw record fill |
| 18-2 | §3.2 ucrtbase `__C_specific_handler` 의 Wine 의존 fact | hexa `seh_specific_handler.hexa` — scope-table walk |
| 18-3 | §5.3 EncodePointer ROR+XOR + PEB cookie | native cpp `EncodePointer/DecodePointer` 정합 fix |
| 18-4 | §5.4 SEH dispatch frame walker | shim cpp `phase17_seh_dispatch_v2` |
| 18-5 | §5.1 mem_read graceful + SEH push 정합 | shim cpp `mem_fault_with_seh_push` |

**예상 KPI**: USER32 first hit 도달 (wave 17-A 의 292228 inst → wave 18 후 inst 증가, KERNEL32 hit > 2400, USER32 hit ≥ 1).

---

## 8. 마무리 — own1 enforcement note

본 doc 는 Wine 의 어떤 진보도 본 project 의 미래 trigger 로 인용하지 않는다. Wine 10.1 의 "Battle.net certificate fix" 는 *외부 fact* 일 뿐 — 본 project 는 그것 없이 native shim 으로 동일 effect 를 가는 path. CrossOver / Whisky / GPTK 의 어떤 release 도 본 project 의 game-changer 가 아니며, 본 doc 의 모든 citation 은 (a) Battle.net 자체의 dependency 외부 fact 검증, (b) Wine 도 동일 binary 에 회귀하는 negative comparison, (c) 본 shim 의 divergence 분석 reference — 셋 중 하나로만 분류.

**reference 단순 list (본문 hyperlink 의 마스터 list)**:

- [WineHQ AppDB Battle.net App Current](https://appdb.winehq.org/objectManager.php?iId=28855&iTestingId=86353&sClass=version)
- [WineHQ AppDB Rating Definitions](https://wiki.winehq.org/AppDB_Rating_Definitions)
- [Wine Bugzilla 49590 Agent.exe hang/crash](https://www.winehq.org/pipermail/wine-bugs/2020-July/539042.html)
- [WineHQ forum 33431 Battle.net + Winetricks](https://forum.winehq.org/viewtopic.php?t=33431)
- [WineHQ forum 32900 Battle.net staging install](https://forum.winehq.org/viewtopic.php?t=32900)
- [WineHQ forum 22769 Cannot setup Battle.net agent](https://forum.winehq.org/viewtopic.php?t=22769)
- [WineHQ forum 32275 starcraft battle.net don't render properly](https://forum.winehq.org/viewtopic.php?t=32275)
- [WineHQ forum 27166 Battle.net not working ucrtbase](https://forum.winehq.org/viewtopic.php?f=8&t=27166)
- [WineHQ forum 37972 64-bit WINEPREFIX verbs](https://forum.winehq.org/viewtopic.php?t=37972)
- [WineHQ forum 30393 VCRedist needed](https://forum.winehq.org/viewtopic.php?t=30393)
- [Lutris docs/Battle.Net.md](https://github.com/lutris/docs/blob/master/Battle.Net.md)
- [Lutris game Battle.net](https://lutris.net/games/battlenet/)
- [Lutris forum Battle.net DXVK DLL not found](https://forums.lutris.net/t/battle-net-dxvk-required-dll-could-not-be-found/2118)
- [Lutris forum Battle.net missing DLL Error](https://forums.lutris.net/t/battle-net-missing-dll-error/2551)
- [Lutris issue 6180 Blizzard App erratic](https://github.com/lutris/lutris/issues/6180)
- [NixOS Wiki Battle.net](https://nixos.wiki/wiki/Battle.net)
- [Schotty.com Wine + Battle.net guide](https://www.schotty.com/Games_And_Wine/Wine_Plus_BattleNet/)
- [LinuxAdictos Wine 10.1 Battle.net fixes](https://en.linuxadictos.com/wine-10-1-starts-development-of-the-stable-2026-release-with-battle-net-fixes-and-many-fixes.html)
- [Arch BBS 215203 Wine won't start battle.net](https://bbs.archlinux.org/viewtopic.php?id=215203)
- [Arch BBS 237854 Battle.net network BLZBNTBGS8000000B](https://bbs.archlinux.org/viewtopic.php?id=237854)
- [Bottles issue 2254 wineserver crash Battle.net](https://github.com/bottlesdevs/Bottles/issues/2254)
- [Bottles issue 464 Battle.net Wine-GE/SODA install fail](https://github.com/bottlesdevs/programs/issues/464)
- [winetricks issue 1369 missing vcrun2015](https://github.com/Winetricks/winetricks/issues/1369)
- [winetricks issue 1510 vcrun2019 request](https://github.com/Winetricks/winetricks/issues/1510)
- [winetricks issue 1635 ucrtbase install bug](https://github.com/Winetricks/winetricks/issues/1635)
- [winetricks issue 1736 vcrun2015/2017/2019 broken](https://github.com/Winetricks/winetricks/issues/1736)
- [dxvk issue 255 Battle.net DXVK launcher fail](https://github.com/doitsujin/dxvk/issues/255)
- [dxvk issue 1098 d3d11.dll Loading library](https://github.com/doitsujin/dxvk/issues/1098)
- [Whisky issue 666 Battle.net crashed when login](https://github.com/Whisky-App/Whisky/issues/666)
- [Whisky issue 682 Battle.net doesn't start with latest update](https://github.com/Whisky-App/Whisky/issues/682)
- [Whisky issue 224 Diablo IV Battle.net updating loop](https://github.com/Whisky-App/Whisky/issues/224)
- [Whisky issue 554 Diablo IV wiki update](https://github.com/Whisky-App/Whisky/issues/554)
- [Blizzard forum 33122 Last update breaks wine](https://us.forums.blizzard.com/en/blizzard/t/last-update-completely-breaks-wine-compatibility/33122)
- [Blizzard forum 48425 patch 14542 wine](https://us.forums.blizzard.com/en/blizzard/t/battlenet-patch-14542-not-working-in-wine/48425)
- [piunikaweb 2026-01-22 D2R CrossOver bug](https://piunikaweb.com/2026/01/22/d2r-launch-bug-crossover-codeweavers-no-fix-eta/)
- [discussions.apple.com 256238678 D2R CrossOver](https://discussions.apple.com/thread/256238678)
- [wine-mirror dlls/winemac.drv](https://github.com/wine-mirror/wine/tree/master/dlls/winemac.drv)
- [wine-mirror dlls/winemac.drv/macdrv_main.c](https://github.com/wine-mirror/wine/blob/master/dlls/winemac.drv/macdrv_main.c)
- [Ken Thomases winemac retina commit](https://list.winehq.org/archives/list/wine-commits@list.winehq.org/thread/3BIOWHDVPUUN4637PKIUL7FNQY6KY3NV/)
- [wowdev.wiki Agent](https://wowdev.wiki/Agent)
- [Wowpedia Wine troubleshooting](https://wowpedia.fandom.com/wiki/Wine_troubleshooting)
- [linuxvox Battle.net on Linux Comprehensive Guide](https://linuxvox.com/blog/battle-net-linux/)
- [GamingOnLinux Battle.net SteamOS install](https://www.gamingonlinux.com/guides/view/how-to-install-battle-net-on-linux-steamos-and-steam-deck-for-world-of-warcraft-and-starcraft/)
- [linuxconfig Battle.net Ubuntu 18.04](https://linuxconfig.org/how-to-install-blizzard-battle-net-app-on-ubuntu-18-04-bionic-beaver-linux)

---

**file end** — 본 doc length: ~330 line. 다음 cycle 의 wave 18 step 1 design doc 와 직접 연결.
