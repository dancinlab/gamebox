# PURPLE 런처 + Lineage 시리즈 만점 검토

> **status**: 검토일 **2026-05-01** / 갱신 **2026-05-01-rev2** (사용자 후속 directive + WebSearch 정정) / **만점 가능성: 부분가능 (PURPLE shell + Lineage 2 만 install~login~CDN), gameplay 만점 불가능 (kernel anti-cheat 차단)**
> 작성: 2026-05-01
> SSOT: `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` (단계 형식 참조), `docs/DELTA_FORCE_MANJEOM_REVIEW.md` (kernel anti-cheat 차단 본보기)
> 결론 한줄: **PURPLE 자체는 .NET/WinForms shell 로 closure path B 호환 가능 (Battle.net 보다 가벼움), 그러나 Lineage 시리즈 전체가 nProtect GameGuard / VIOLET / XIGNCODE3 / EAC 중 하나 이상을 kernel mode 로 강제 → first frame 이후 만점 불가. 가장 현실적 후보는 Lineage 2 (DX9, 단 GameGuard 우회 X 정책) 와 Lineage 2M (UE4 mobile-cross, kernel AC 모듈 분리 가능성 추정)**.

---

## ⚠️ 0.5 정정 (rev2 — 2026-05-01 사용자 directive 후속 research)

원본 §0~5 의 일부 결론이 **outdated** — WebSearch 재확인 결과 다음 정정 필요:

### 0.5-1. 리니지 클래식 = 신작 r28 (2026-02-07 출시), Lineage 1 (1998) 아님

| 항목 | 원본 (틀림) | 정정 |
|---|---|---|
| 출시 | "1998" | **2026-02-07** (KR/대만, NCSOFT 신작) |
| 빌드 | "i386 PE + DDraw" | **x86_64 PE32+ + OpenGL 4.3+** (Win10/11 64bit only) |
| 시스템 사양 | "DX5/6 시대 매우 단순" | Intel 4세대(Haswell)+ / Ryzen 1000+ / GTX 1050 Ti+ / **16GB RAM 권장 / SSD 40GB** |
| 라이선스 | "1998 free download" | **월정액 29,700원** (사전 무료 ~2026-02-11, 이후 유료) |
| 보안 | "원판 무 anti-cheat → 후기 GameGuard" | **GameGuard + 필수 기기 등록 (HWID 기반)** — 출시 시점부터 활성 |
| closure path B 적용 | "i386 PE 매우 단순, private 만 가능" | **x86_64 native + OpenGL → closure path B 직접 적용 가능** (live KR/대만 서버는 GameGuard 차단) |

→ **본 검토에서 가장 closure path B 친화적 후보** (D2R/D4 와 동급 PE32+ + OpenGL stack 으로 Apple Metal 변환 가능).
→ private 서버 ecosystem (l2j-classic 등) 은 r28 기반 build 에 대해 아직 미형성 — **live 서버 한정 검토** (GameGuard 차단 수용 시 skeleton-only).

### 0.5-2. 리니지 M PC version 존재 — "PC 클라이언트 부재" 정정

| 항목 | 원본 (틀림) | 정정 |
|---|---|---|
| platform | "Android / iOS (mobile only). PC 클라이언트 별도 발표 없음" | **Android / iOS / Windows PC (PURPLE 통한 정식 출시)** |
| 다운로드 | "PURPLE 으로 launch 대상 아님" | `LineageM_PURPLE_Installer.exe` → PURPLE STORE 통한 PC 정식 설치 (`purple.plaync.com/game/lmt`) |
| 가능성 | "**불가능** — PC 클라이언트 부재" | **가능 (PURPLE 2-layer)** — 단 closure path B 적용 layer 가 다름 (§0.5-3 참조) |

### 0.5-3. PURPLE 의 본질 = NCSOFT in-house Android emulator (정정)

원본 §0-1 은 PURPLE 을 ".NET / WinForms shell" 로 분류했으나 정확한 기술 본질은:

> "Purple works similar to an Android emulator, but simply just plays games from NCsoft while having various community features. Rather than a third-party solution like BlueStacks, Purple is being built in-house." — MMOs.com / Massively OP / MMO Culture 2019

**closure path B 적용 layer 재정립**:

| Layer | 기술 | closure path B 직접 적용? |
|---|---|---|
| Layer 0 — PURPLE.exe 자체 | Windows PE32+ x86_64 + .NET CLR + WPF + WebView2 (lounge UI) + **자체 Android emul core (in-house)** | **○ 가능** — PE loader 로 PURPLE.exe 매핑 |
| Layer 1 — PURPLE STORE 게임 (mobile-first) | Android APK / Dalvik VM bytecode | △ Android emul core 까지 통과해야 game logic 도달 (2-layer) |
| Layer 1' — PURPLE STORE 게임 (PC native) | x86_64 native PE (예: L Classic r28) | **○ 가능** — PURPLE 우회하고 직접 PE 매핑 (단 PURPLE 인증 dependency 별개) |

→ **L M / L W / L 2M = mobile-first** → Layer 1 = APK + Dalvik VM (2-layer, prerequisite 큼).
→ **L Classic (r28) = PC native** → Layer 1' = direct PE (closure path B 직접 적용, 가장 단순).

### 0.5-4. rev2 우선순위 재정렬 (사용자 directive)

사용자 directive ("리니지 클래식 / 리니지 M / 리니지 W 진행") 반영:

| 순위 | 게임 | path | ETA |
|---|---|---|---|
| **1** | **리니지 클래식 r28 (live)** | closure path B 직접 (x86_64 + OpenGL 4.3 + GameGuard skeleton-only) | 1 cycle (skeleton) |
| **2** | **리니지 W (PURPLE 2-layer)** | PURPLE shell prerequisite 5 모듈 → UE4 cross-platform skeleton | 2 cycle (PURPLE shell + LW skeleton) |
| **3** | **리니지 M (PURPLE 2-layer)** | PURPLE shell 공유 + Android emul layer track | 1 cycle (LW 공유 후) |

→ 원본 §5-2 "Lineage 2 private 1순위" 는 사용자 directive 로 **re-prioritized**. L 2 private 은 L Classic / M / W 진행 후 검토.

### 0.5-5. 신규 candidate 추가 (rev2 후속 작업)

원본 §4 candidate list 에 추가 (cycle 49 batch 후보):

| # | candidate | phase | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|---|---|
| 16 | `c_lineage_classic_r28_native_first_frame` | 4 | 5 | 5 | 5 | **15** | x86_64 + OpenGL 4.3 native, GameGuard live 차단 — skeleton-only validated 가능 |
| 17 | `c_purple_android_emul_layer_track` | 0-2 | 5 | 4 | 5 | 14 | PURPLE 의 Android emul core 식별 — L M / L W / L 2M 의 prerequisite (2-layer 정량화) |
| 18 | `c_lineage_m_purple_apk_emul_first_frame` | 4 | 5 | 5 | 5 | **15** | PURPLE → Android APK 2-layer skeleton (Layer 1) |

→ 기존 candidates 7 (L1 DDraw skeleton) / 9 (L2M UE4) / 10 (LW UE4) 와 함께 **총 9 candidates 가 cycle 49+ 진입 후보**.

---

---

## 0. 게임 메타데이터 (조사 결과)

### 0-1. PURPLE 런처

| 항목 | 값 | 출처 |
|---|---|---|
| 정식 명칭 | **PURPLE** (NC America 명칭 "PURPLE Mega Launcher") | MMORPG.com, NC 공식 |
| 개발사 | NCSOFT / NC America | MMORPG.com |
| 출시 | 2019-11 (Lineage 2M 동반 KR 출시 → 이후 NA/EU/SEA 순차 확대) | MMO Culture, MMOs.com |
| 지원 게임 (현 시점) | Lineage 2M, Lineage W, Lineage 2, Aion (KR/NA), Blade & Soul (NA, 추정), Throne and Liberty 일부 region | NC America, MMORPG.com, AreWeAntiCheatYet |
| 기능 | 게임 launcher + 채팅 (Talk) + 스트리밍 (Live, On) + Store + Lounge + Multiplay (다계정) + push notification | NC America, MMORPG.com |
| Windows binary 포맷 | **PE32+ x86_64** (`.NET / WinForms` 추정 — Proton 이슈 trace 에서 .NET tag 확인) | Proton issue #7878 |
| 사용 protocol | HTTPS + **PLAYNC** account OAuth (NCWEST 계정 → PLAYNC 통합) + 자체 update protocol | NC America, login.plaync.com |
| 인증 | PLAYNC OAuth + NC 인증기 (TOTP 또는 biometric mobile push) | NC 인증기 (Google Play), saaspass.com |
| macOS 공식 지원 | **없음** (Windows + iOS + Android만; Mac 은 "in talks with Apple" 단계 — 2019 발언, 이후 미실현) | MMOs.com |
| Wine / Proton 상태 | **불안정** — login 까지는 가능, **lobby 진입 직후 crash** (Proton issue #7878 — .NET 관련, 2024-07 보고 / 후속 issue #8515 duplicate) | Proton GitHub #7878, Whisky #724 |
| Whisky (macOS Wine) | login 후 **black screen** — Whisky-App discussion #724 | Whisky-App #724 |
| 설치 protocol | `PurpleInstaller_NCS_*.exe` (any.run sandbox 분석 — `purple-ncsoft.myskcdn.net` CDN) | any.run |
| 게임 launch 메커니즘 (추정) | Steam 식 wrapper (자식 프로세스 spawn) + 일부 게임은 **VIOLET** (NCSOFT 자체 anti-cheat launcher) 가 inject — Aion KR/NA 가 그 예 | AreWeAntiCheatYet issue #1737 |

### 0-2. Lineage 시리즈 — 각 버전 메타데이터

| 게임 | 출시 | 엔진 / API | platform | 안티치트 | macOS 가능성 (closure path B 관점) | 출처 |
|---|---|---|---|---|---|---|
| **Lineage (1)** "Classic" | **1998** | 2D isometric (Diablo 2 / Ultima Online 풍) — **DirectDraw** 추정 (DX5/6 시대) | Windows PC only (KR 위주) | 원판은 무 anti-cheat → 후기 patch 에 GameGuard 추가 (KR 라이브 서버 기준) | **부분가능** — i386 PE + DirectDraw 매우 단순. 단 KR 서버 GameGuard 차단. private/legacy 서버는 가능 | Wikipedia Lineage, PCGamingWiki Lineage |
| **Lineage 2** | **2003** (KR), 2004 (NA) | **Unreal Engine 2.5** — 후기 chronicle 에서 일부 reskin, 그러나 정식 UE3 upgrade 없음. **DirectX 9.0c** 요구. UE4 / UE5 reskin 은 비공식 fan project (Lu4) 또는 2026 진행 중 NCSOFT remaster 예고 | Windows PC | **nProtect GameGuard** (KR/NA 라이브) — kernel mode. EU 는 한때 GameGuard 사용 중지, 이후 Gameforge XIGNCODE3 도입 | **부분가능** — DX9 + UE2.5 stack 은 D2R/D4 보다 가벼움. 그러나 GameGuard kernel 이 closure path B entry call 차단 → Lineage 1 과 동일하게 private/legacy 서버에서만 만점 가능 | Wikipedia Lineage II, PCGamingWiki Lineage II, NProtect GameGuard wiki |
| **Lineage M** | 2017 (KR mobile) | Unity (추정 — mobile-first) | Android / iOS (mobile only). PC 클라이언트 별도 발표 없음 | 모바일 native 안티치트 (NC 자체 추정) | **불가능** — PC 클라이언트 부재 → PURPLE 으로 launch 대상 아님. Android emulator 우회는 closure path B 와 별개 vector | (출처 부족 — 추정) |
| **Lineage W** | **2021** | **Unreal Engine 4** ("a completely new engine and technologies") | mobile (iOS/Android) + **PC via PURPLE** (cross-platform play 지원) | NC 자체 + PURPLE 통합 anti-cheat (추정 — VIOLET/nProtect 계열 가능) | **부분가능** — UE4 + DX11 stack. PURPLE 의존 → PURPLE 의 .NET shell 이 가능해야 시작 가능 | NoxPlayer Lineage W article, lineagew.plaync.com |
| **Lineage 2M** | **2019-11** (KR) | **Unreal Engine 4** (4K UHD, full 3D) — Unity 아님 (조사 시 자주 혼동되나 NC 공식 자료는 UE4 명시) | mobile + **PC via PURPLE** (cross-platform) | **PURPLE 통합** — VIOLET / nProtect 계열 (Aion KR/NA 와 유사 추정) | **부분가능** — Lineage W 와 같은 stack. UE4 PE32+ x86_64 + DX11. PURPLE shell 통과가 prerequisite | MMO Culture, BlueStacks, gildor's forum (UE4 확인) |
| **Throne and Liberty (TL)** | **2024-10** (글로벌) | **Unreal Engine 5** | PC + console (PS5/XSX) | **Easy Anti-Cheat (EAC)** — kernel mode | **불가능** — Delta Force 와 동일 등급의 kernel EAC. EAC 47 / 41 error 빈번. Steam 배포로 Battle.net/PURPLE 우회 가능하나 anti-cheat 차단 동일 | Wikipedia Throne and Liberty, PCGamingWiki THRONE and Liberty, Steam community |

### 0-3. Region 별 차이 (Lineage 2 + Aion 사례, 다른 NC 게임도 유사 추정)

| Region | 런처 | anti-cheat |
|---|---|---|
| KR | PURPLE | **nProtect GameGuard + VIOLET** (NC 자체) |
| NA | PURPLE | **nProtect GameGuard + VIOLET** |
| EU | Gameforge Client (PURPLE 아님) | **XIGNCODE3** |
| SEA | PURPLE | (KR 기준 추정) |

→ EU 만 PURPLE 미사용 → **검토 대상은 KR/NA/SEA region** 으로 한정.
→ 출처: AreWeAntiCheatYet issue #1737 (AION 항목 — AION EU 는 Gameforge + XIGNCODE3, AION KR/NA 는 PURPLE + nProtect + VIOLET).

### 0-4. 예상 DLL set (PURPLE shell — 추정)

`KERNEL32, USER32, GDI32, ADVAPI32, OLE32, OLEAUT32, SHELL32, SHLWAPI, WS2_32, WININET, WINHTTP, CRYPT32, BCRYPT, NTDLL, MSVCP140, VCRUNTIME140, UCRTBASE, **mscoree.dll, clr.dll, mscorlib (.NET CLR)**, **WPF / WinForms (PresentationCore.dll, PresentationFramework.dll, System.Windows.Forms.dll)**, DWMAPI, COMCTL32, **WebView2 (Microsoft.Web.WebView2.Core.dll — store/lounge UI 추정)**`.

→ Lineage 2 / W / 2M / TL 게임 본체 별 DLL 추가 (D3D9, D3D11, D3D12 + UE2.5/UE4/UE5 + GameGuard / VIOLET / XIGNCODE3 / EAC).

---

## 1. 6 phase 별 만점 가능성 매트릭스

### Phase 1 — PURPLE 런처 install

**시나리오**: `PurpleInstaller_NCS_*.exe` (CDN: `purple-ncsoft.myskcdn.net`) → PE32+ x86_64 installer → Windows file 시스템 + .NET runtime check → registry shim → PURPLE.exe 배치.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-PURPLE-INSTALLER-PE64-MAP | 5 | 4 | 5 | 14 | D4 PE64 인프라 ~80% 재사용 |
| C-PURPLE-DOTNET-CLR-PROBE | 5 | 4 | 5 | 14 | .NET CLR PE 식별 + WinForms / WPF entry 패턴 |
| C-PURPLE-CDN-MIRROR-RESOLVE | 5 | 4 | 5 | 14 | `purple-ncsoft.myskcdn.net` mirror 해석 (Battle.net Akamai 와 유사) |

**phase 결론**: **부분 가능** (보조 +3). install 단계는 anti-cheat 비활성.
**ETA**: 0.5 개월 (D4 PE64 + Battle.net CDN 인프라 재사용).

---

### Phase 2 — PLAYNC OAuth login + PURPLE shell 부팅

**시나리오**: `login.plaync.com` OAuth (Authorization Code + Token) → 2-step: NC 인증기 (TOTP / push) → PURPLE shell 메인 화면 (Lobby 진입 전).

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-PURPLE-PLAYNC-OAUTH-PROXY | 5 | 5 | 5 | **15** | Battle.net OAuth / Steam OAuth / Tencent ID 와 동형 — generic OAuth 모듈 흡수 후보 |
| C-PURPLE-NC-2FA-PROXY | 5 | 5 | 5 | **15** | TOTP HMAC-SHA1 (RFC 6238) — generic 2FA 모듈 |
| C-PURPLE-WINFORMS-WPF-WARM | 5 | 5 | 5 | **15** | WinForms / WPF / WebView2 PE 의 ngen/JIT warm 인지 모델 |

**phase 차단 risk (CRITICAL)**:
- Proton issue #7878 / Whisky #724: **lobby 진입 직후 crash / black screen**.
- 원인 (추정): .NET 5/6/8 LTS + WPF 의 D3D9 → DXGI 호환 layer + Win32 message pump 가 Wine/Proton 의 user32 message pump 를 정확히 따라가지 못함.
- airgenome-gamebox 는 **DIY hexa shim** 이므로 Wine 보다 더 정밀히 Win32 message pump + .NET interop 을 모사해야 함 → **D2R/D4 대비 난이도 +2 레벨 상승**.

**phase 결론**: **가능 — 단 .NET / WinForms / WPF / WebView2 closure 가 prerequisite**. login 까지는 도달 가능 (Proton 도 도달함).
**ETA**: 1-1.5 개월 (.NET CLR + WPF 모사가 신규 비용).

---

### Phase 3 — game catalog 조회 + CDN download

**시나리오**: PURPLE shell 내 game store → 사용자가 Lineage 2/W/2M/TL 선택 → CDN download (`purple-ncsoft.myskcdn.net` + game-specific mirror) → SHA verify → 디스크 install.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-PURPLE-GAME-CATALOG-PROBE | 5 | 5 | 5 | **15** | PLAYNC catalog endpoint enumerate (TLS pinning 추정) |
| C-PURPLE-CDN-PARALLEL-CHUNK | 5 | 4 | 5 | 14 | C-CDN-PARALLEL-CHUNK 재사용 (D4 와 공유) |
| C-PURPLE-MULTI-GAME-LAYOUT-PREDICT | 5 | 5 | 5 | **15** | UE2.5 (L2) / UE4 (L2M, LW) / UE5 (TL) 별 .pak / .upx layout prefetch — 다중 UE 버전 일반화 |

**phase 결론**: **가능 — Phase 2 lobby crash 만 우회되면 즉시 도달**.
**ETA**: 0.5 개월 (인프라 재사용).

---

### Phase 4 — game launch + first frame (각 Lineage 별)

#### 4-A. Lineage 1 Classic (1998)

**시나리오**: PURPLE → Lineage.exe (i386 PE) → DirectDraw + Win32 GDI + Winsock → 첫 frame.

| 모듈 후보 | A | B | C | 만점 | 차단 가능성 |
|---|---|---|---|---|---|
| C-LIN1-DDRAW-FIRST-FRAME | 5 | 5 | 5 | **15** | **부분가능** — KR 라이브 서버 GameGuard 차단. private/legacy 서버 (예: 비공식 server emulator) 에서는 가능 |
| C-LIN1-I386-PE-MAP | 5 | 4 | 5 | 14 | D2R i386 인프라 100% 재사용 |
| C-LIN1-WINSOCK-LOBBY-WARM | 5 | 4 | 5 | 14 | TCP 직결 — 매우 단순 |

**phase 결론**: **부분 가능 (KR live 서버 차단, private 서버 가능)**.
**ETA**: 1 개월 (D2R 재사용).

#### 4-B. Lineage 2 (2003, UE2.5 + DX9)

**시나리오**: PURPLE → l2.exe (UE2.5) → D3D9 → first frame → GameGuard kernel 검증.

| 모듈 후보 | A | B | C | 만점 | 차단 가능성 |
|---|---|---|---|---|---|
| C-LIN2-UE25-FIRST-FRAME | 5 | 5 | 5 | **15** | **차단됨 (KR/NA live)** — GameGuard kernel pre-attest 실패 |
| C-LIN2-DX9-PIPELINE-WARM | 5 | 5 | 5 | **15** | D2R DX11 stage1 보다 1세대 하위. MoltenVK / Apple D3DMetal (D3D9 path) 이용 가능 |
| C-LIN2-UE25-PAK-PREDICT | 5 | 5 | 5 | **15** | UE2.5 .ukx / .utx / .uax layout (.pak 와 다름) |

**phase 결론**: **차단됨 (KR/NA live), 부분가능 (private/legacy server, GameGuard 비활성 instance)**.
**ETA**: KR/NA = ∞, private = 1 개월.

#### 4-C. Lineage 2M / Lineage W (UE4 + DX11)

**시나리오**: PURPLE → game.exe (UE4 PE32+ x86_64) → D3D11 → first frame → VIOLET / nProtect 검증.

| 모듈 후보 | A | B | C | 만점 | 차단 가능성 |
|---|---|---|---|---|---|
| C-LIN2M-UE4-FIRST-FRAME | 5 | 5 | 5 | **15** | **차단됨 — VIOLET / nProtect kernel** (Aion KR/NA 와 동일 stack 추정) |
| C-LINW-UE4-FIRST-FRAME | 5 | 5 | 5 | **15** | **차단됨 — 동일** |
| C-PURPLE-UE4-DX11-WARM-SHARED | 5 | 5 | 5 | **15** | L2M / LW 공통 — generic UE4 + DX11 warm-up |

**phase 결론**: **차단됨 (KR/NA live)**. EU 는 PURPLE 미사용 + XIGNCODE3 도 kernel — 어느 region 도 closure path B 통과 불가.
**ETA**: ∞ (kernel anti-cheat).

#### 4-D. Throne and Liberty (UE5 + DX11/DX12 + EAC)

**phase 결론**: **불가능 — Delta Force 와 동일** (Easy Anti-Cheat kernel).
**ETA**: ∞.

→ 자세한 EAC 차단 분석은 `docs/DELTA_FORCE_MANJEOM_REVIEW.md` 의 ACE 분석과 동형 (driver 부재, kernel callback 검증, syscall hash 모두 동일하게 실패).

---

### Phase 5 — gameplay 만점

| 게임 | 가능성 |
|---|---|
| Lineage 1 (KR live) | 차단 (GameGuard) |
| Lineage 1 (private) | **가능** |
| Lineage 2 (KR/NA live) | 차단 (GameGuard) |
| Lineage 2 (private/legacy) | **가능** (Lineage 2 private 서버 ecosystem 존재 — l2j 등) |
| Lineage M | 불가 (PC client 없음) |
| Lineage W (live) | 차단 (PURPLE + 추정 kernel AC) |
| Lineage 2M (live) | 차단 (동일) |
| Throne and Liberty (live) | 차단 (EAC kernel) |

---

### Phase 6 — live tune

**phase 결론**: Phase 5 가능 game (Lineage 1/2 private) 만 의미 있음 — gameplay 가능 후 Apple Silicon AMX/Metal compute 로 macro/AI 가속 가능 (단, **bot 우회 시 TOS 위반 risk** — 단순 frame perf 가속만 권장).

---

### 매트릭스 요약 (Lineage 시리즈 별)

| Phase | Lineage 1 (live/private) | Lineage 2 (live/private) | Lineage 2M | Lineage W | Lineage M | Throne and Liberty |
|---|---|---|---|---|---|---|
| 1. PURPLE install | 보조 +3 | 보조 +3 | 보조 +3 | 보조 +3 | N/A | N/A (Steam) |
| 2. PLAYNC login | +3 만점 | +3 만점 | +3 만점 | +3 만점 | N/A | N/A (Steam OAuth) |
| 3. CDN download | +3 만점 | +3 만점 | +3 만점 | +3 만점 | N/A | N/A |
| 4. first frame | 차단 / **+2** | 차단 / **+3** | 차단 | 차단 | N/A | 차단 |
| 5. gameplay | 차단 / **+1** | 차단 / **+2** | 차단 | 차단 | N/A | 차단 |
| 6. live tune | 차단 / +1 | 차단 / +1 | 차단 | 차단 | N/A | 차단 |

→ **만점 도달 가능 (live)**: Phase 1-3 만 — 즉 PURPLE shell + PLAYNC OAuth + CDN download 만 (보조 +3 + 만점 +6).
→ **만점 도달 가능 (private/legacy)**: Lineage 1 + Lineage 2 가 가능. private 서버 ecosystem 의 합법성 / TOS 별도 검토 필요.
→ **만점 가능성 비율 (live 게임 한정)**: ~30% (6 phase 중 1.8 phase).
→ **만점 가능성 비율 (private 포함)**: ~55% (Lineage 1/2 private 으로 6 phase 중 3.3 phase).

---

## 2. anti-cheat 차단 위험 평가

### 2-1. 4종 anti-cheat 비교 (PURPLE / Lineage stack 에서 등장)

| AC | Vendor | Mode | airgenome 호환 | 비고 |
|---|---|---|---|---|
| **nProtect GameGuard** | INCA Internet (KR) | **kernel** + user-mode hybrid | **불가능** — Wine 정책상 "unfixable" 명시. PCGamingWiki: "kernel-level access makes it incompatible with compatibility layers such as Wine and Proton" | Lineage 1, Lineage 2 (KR/NA), AION KR/NA |
| **VIOLET (NC 자체)** | NCSOFT | (추정) **kernel** + user-mode | **불가능** — AreWeAntiCheatYet issue 에서 PURPLE + VIOLET stack 이 nProtect 와 함께 동작 보고. 단 VIOLET 단독 실태 미확인 (추정) | AION KR/NA, Lineage W/2M (추정) |
| **XIGNCODE3** | Wellbia (KR) | **kernel** | **불가능** — "third-party kernel-level anti-cheat solutions, mostly Korean-developed titles" | AION EU (Gameforge), 일부 Lineage 2 EU |
| **Easy Anti-Cheat (EAC)** | Epic | **kernel** + user-mode | **불가능** (live) — "EAC injects itself into the system kernel with a special driver". Wine/Proton 일부 user-mode variant 만 가능 | Throne and Liberty |

→ **모든 AC 가 kernel mode + Wine 정책상 unfixable 또는 user-mode 한정 우회만 가능**.

### 2-2. closure path B 와의 충돌 지점

closure path B 의 3 step (PE map / IAT bind / entry call) 중 **entry call 직후 attest** 에서 실패:

1. **GameGuard 검증**:
   - `gg.des` driver kernel signature 확인
   - `KUSER_SHARED_DATA` (0x7FFE0000) build hash
   - `PEB->Ldr` 정식 Windows DLL 검증
   - process 의 syscall stub hash
   - Windows Defender / WHQL signature 확인

2. **VIOLET 검증** (추정):
   - PURPLE 모회사 NCSOFT 자체 정책으로 hardware fingerprint 강제 (Mac UUID/serial 노출 risk)
   - PLAYNC token 과 hardware 1:1 매칭 → BAN risk

3. **XIGNCODE3 검증**:
   - `xxd-*.xem` driver 존재
   - kernel callback 등록 (`PsSetCreateProcessNotifyRoutine`)

4. **EAC 검증** (TL):
   - Delta Force ACE 와 동일 등급 (`docs/DELTA_FORCE_MANJEOM_REVIEW.md` 2-2 절 참조)

→ airgenome-gamebox (macOS native, DIY hexa loader) 환경에서 **모두 실패 보장**.

### 2-3. BAN risk

- PLAYNC 계정 영구 BAN 가능 — Lineage 시리즈 전체 (multi-game stack) 동시 BAN risk.
- HWID BAN — Mac UUID 가 NC 자체 blacklist 등록 시 향후 어떤 NC 게임도 차단.
- gamebox identity ("own 1: Wine 0줄") 와 anti-cheat 우회는 정면 모순.

### 2-4. 예외: private / legacy 서버

- **Lineage 2 private**: l2j (Java emulator) ecosystem 활성. GameGuard 미적용 instance 다수. closure path B 충분히 통과.
- **Lineage 1 private**: classic emulator project 산발적. GameGuard 미적용 가능.
- 위 두 시나리오는 **만점 검토 가치 있음**, 단 NCSOFT TOS 위반 가능 — 사용자 본인 책임.

---

## 3. closure path B 호환성 — PURPLE + 각 Lineage 별

| step | D2R | D4 | Delta Force | **PURPLE shell** | **Lineage 1** | **Lineage 2** | **Lineage 2M / W** | **TL** |
|---|---|---|---|---|---|---|---|---|
| PE map | i386 | x86_64 | x86_64 | **x86_64 + .NET CLR** | **i386 (legacy)** | x86 (legacy l2.exe — 32bit 추정) | x86_64 (UE4) | x86_64 (UE5) |
| IAT bind | DX11 | DX12+DXR | DX12+DXR+UE5+ACE | **WinForms/WPF/.NET + WebView2** | **DDraw + GDI + Winsock** | **D3D9 + UE2.5** | D3D11 + UE4 | D3D11/12 + UE5 |
| entry call | offline 가능 | online DRM | online + ACE attest | **online + PLAYNC OAuth** | online (KR/NA) / offline (private) | 동일 | online + VIOLET attest | online + EAC attest |
| anti-cheat | 없음 | 없음 (DRM) | **kernel ACE — 차단** | (없음 — PURPLE 자체) | **GameGuard — 차단 (live)** | **GameGuard — 차단 (live)** | **VIOLET/nProtect — 차단** | **EAC — 차단** |
| DLL coverage prerequisite | 일반 + DX11 | 일반 + DX12 | 일반 + DX12 + ACE | **일반 + .NET CLR + WPF + WebView2** | 일반 + DDraw | 일반 + DX9 + UE2.5 | 일반 + DX11 + UE4 | 일반 + DX12 + UE5 |
| DXR / Mesh shader | 없음 | DXR done | DXR (Lumen) | 없음 | 없음 | 없음 | 없음 (UE4) | DXR (UE5 Lumen) |
| Apple D3DMetal 매핑 | DX11 stage1 | DX12 stage2 | stage2 (그러나 차단) | 없음 (UI only) | DDraw → Metal 2D blit | **DX9 → D3DMetal — Apple D3DMetal 의 sweet spot** | DX11 → D3DMetal | DX12 → D3DMetal |

**핵심 발견**:
- PURPLE shell 은 **그래픽 부담 없음** (WinForms/WPF UI). 그러나 **.NET CLR 모사가 신규 비용** — D2R/D4/Delta Force 가 모두 native C++ 인 것과 다름.
- **Lineage 2 (DX9) 가 Apple D3DMetal 의 가장 좋은 fit** — Apple D3DMetal 은 D3D11/D3D12 path 가 우선이지만 D3D9 도 Game Porting Toolkit 에서 지원.
- **Lineage 1 (DDraw)** 는 너무 오래 → D3DMetal path 없음. 자체 2D blit (Metal MTLBlitCommandEncoder) 로 reinterpret 가능.

---

## 4. 추천 skeleton 모듈 candidate (CN 번호 미정)

### 4-1. PURPLE shell 공통 (모든 Lineage 게임 prerequisite)

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 1. **c_purple_dotnet_clr_probe** | 1-2 | 5 | 4 | 5 | 14 | .NET CLR / WinForms / WPF PE 패턴 식별 — Battle.net / 다른 .NET 런처 재사용 |
| 2. **c_purple_plaync_oauth_proxy** | 2 | 5 | 5 | 5 | **15** | OAuth generic 모듈 (Battle.net / Steam / Tencent / NCSOFT 통합) |
| 3. **c_purple_2fa_totp_proxy** | 2 | 5 | 5 | 5 | **15** | TOTP RFC 6238 — generic 2FA |
| 4. **c_purple_webview2_warm** | 2 | 5 | 5 | 5 | **15** | WebView2 (Edge Chromium) embed warm-up — Battle.net 의 CEF 와 형 비슷 |
| 5. **c_purple_cdn_myskcdn_resolve** | 1, 3 | 5 | 4 | 5 | 14 | NCSOFT CDN (`purple-ncsoft.myskcdn.net`) mirror — Akamai/CloudFront stack |
| 6. **c_purple_lobby_crash_diagnose** | 2 | 5 | 4 | 5 | 14 | Proton issue #7878 의 lobby crash 원인 분석 + 우회 — closure path B 만의 문제, generic 가치 있음 |

### 4-2. Lineage 별 (live 서버 — 차단됨, skeleton 만점 누적용)

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 7. **c_lineage1_ddraw_first_frame_skeleton** | 4 | 5 | 5 | 5 | **15** | **skeleton — KR live 차단**. private 가능. DDraw → Metal 2D blit reinterpret |
| 8. **c_lineage2_ue25_dx9_first_frame_skeleton** | 4 | 5 | 5 | 5 | **15** | **skeleton — KR/NA live 차단**. private (l2j) 가능. UE2.5 .ukx/.utx prefetch |
| 9. **c_lineage2m_ue4_dx11_first_frame_skeleton** | 4 | 5 | 5 | 5 | **15** | **skeleton — 차단**. UE4 + DX11 path (다른 UE4 게임 재사용) |
| 10. **c_lineage_w_ue4_dx11_first_frame_skeleton** | 4 | 5 | 5 | 5 | **15** | candidate 9 와 통합 가능 — `c_purple_ue4_dx11_warm_shared` |
| 11. **c_throne_liberty_ue5_first_frame_skeleton** | 4 | 5 | 5 | 5 | **15** | **skeleton — EAC 차단**. Delta Force `c_dforce_dxr_lumen_warm_skeleton` 와 100% 같은 운명 |

### 4-3. anti-cheat 보호 / 사용자 안내 (gamebox identity 보호)

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 12. **c_purple_anticheat_block_doc** | 0 | 5 | 0 | 0 | 5 | **방어 모듈** — Lineage 1/2 (KR live) / 2M / W / TL detect 시 launch 차단 + BAN 위험 고지. Delta Force `c_dforce_unsupported_block_doc` 와 동형 |
| 13. **c_purple_private_server_allow** | 0 | 5 | 3 | 5 | 13 | private 서버 (l2j 등) 식별 → kernel AC 비활성 시 path B 허용. **사용자 자발적 opt-in** + TOS 안내 필수 |

### 4-4. Apple Silicon 가속 (Phase 4 도달 시 의미 있음 — 즉 private 한정)

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 14. **c_lineage2_d3d9_to_d3dmetal_predict** | 4-5 | 5 | 5 | 5 | **15** | DX9 → Apple D3DMetal (Game Porting Toolkit) cache prediction. Lineage 2 private gameplay 가능 시 만점 candidate |
| 15. **c_lineage1_metal_2d_blit_cache** | 4-5 | 5 | 5 | 5 | **15** | DDraw → MTLBlitCommandEncoder reinterpret. AMX 활용 (2D blit pre-rotate) |

→ **추천 모듈 수: 총 15개** (실 가치 있는 것: 1-6 PURPLE shell + 12-13 보호 + 14-15 private 가속 = **10개**).

---

## 5. 결론 — 우선순위

### 5-1. 만점 가능성 종합

| 항목 | 값 |
|---|---|
| **만점 가능성 (live 게임)** | **부분가능** (PURPLE install + PLAYNC OAuth + CDN — Phase 1-3 만) |
| **만점 가능성 (private 게임)** | **가능 (조건부)** — Lineage 1 / Lineage 2 의 private 서버에서 GameGuard 비활성 instance 한정 |
| 도달 가능 만점 점수 (live) | 보조 +3 (Phase 1) + 만점 +6 (Phase 2-3) |
| 도달 가능 만점 점수 (private 합산) | 위 + 만점 +6~9 (Lineage 1/2 private gameplay) |
| validated 만점 가능성 | live = 0 (Phase 4 차단), private = 가능 |
| skeleton 만점 가능성 | +9~12 (재사용성 모듈 위주, .NET CLR + UE4 generic) |
| BAN risk | **매우 높음** — PLAYNC 계정 + HWID 통합 BAN risk |
| gamebox identity 와의 정합성 | **모호** — PURPLE shell 은 정합 (Wine 0줄 자체 가능), 그러나 anti-cheat 우회 시도는 own 1 정신과 충돌 |

### 5-2. 우선순위 (어느 Lineage 가 가장 가능성 높은지)

**1순위: Lineage 2 (private/legacy 서버)**

- **이유**:
  - DX9 + UE2.5 → Apple D3DMetal Game Porting Toolkit 의 sweet spot
  - i386/x86 32bit 또는 x86_64 (chronicle 별 다름) — D2R i386 인프라 재사용 가능
  - GameGuard 미적용 private 서버 (l2j) ecosystem 활성 → 6 phase 모두 도달 가능
  - L2 클라이언트 자체는 free download 가능 (NCSOFT 공식 + l2j 호환)
- **단점**:
  - private 서버 사용 시 NCSOFT TOS 회색 영역 — 사용자 자발적 opt-in 필수
  - 공식 KR/NA live 는 GameGuard 차단 — closure path B 본질 충돌

**2순위: Lineage 1 (private/legacy 서버)**

- **이유**: 가장 단순한 stack (DDraw + 2D + i386). 만점 phase 6 까지 가장 적은 노력으로 도달 가능
- **단점**: private 서버 ecosystem 이 Lineage 2 보다 작음, 활용가치 낮음

**3순위: PURPLE shell 자체 (게임 무관)**

- **이유**: .NET CLR + WinForms / WPF + WebView2 closure 자체가 generic 인프라 — Battle.net 외 다른 .NET 런처 재사용 가능
- **단점**: Phase 4 game launch 자체는 차단됨 → PURPLE shell 만으로 만점 6 phase 채우는 것은 불가

**4순위: Lineage 2M / Lineage W (live)**

- **이유**: UE4 + DX11 generic 가치 (다른 UE4 게임에 재사용)
- **단점**: VIOLET / nProtect kernel 차단으로 validated 0 — skeleton 만점만 가능

**최후순위: Throne and Liberty / Lineage M**

- TL: Delta Force 와 동일 운명 (EAC kernel)
- Lineage M: PC client 부재 → PURPLE 으로 launch 대상 자체가 아님

### 5-3. D2R / D4 / Delta Force 와 종합 우선순위

```
1. D2R closure 완료
2. D4 closure 완료
3. (병렬) PURPLE shell .NET CLR + PLAYNC OAuth — generic 인프라 (Battle.net OAuth 모듈 흡수)
4. Lineage 2 private (l2j) opt-in 시 — DX9 + UE2.5 Apple D3DMetal validated 만점 시도
5. Lineage 1 private — 후순위 (가치 작음)
6. Lineage 2M / W / TL — 영구 후순위 (kernel AC)
7. Delta Force — 영구 미진입
```

### 5-4. 즉시 권장 action

1. **candidate 12 (`c_purple_anticheat_block_doc`) 우선 작성** — Lineage 시리즈 + PURPLE detect 시 자동 안내 (BAN 위험 고지). gamebox 사용자 보호.
2. **candidate 1-2 (.NET CLR probe + PLAYNC OAuth)** — D2R/D4 OAuth 모듈 generic 화 시 자연 흡수 후보. **Battle.net OAuth 모듈과 통합 검토**.
3. **candidate 13 (`c_purple_private_server_allow`)** — opt-in 형태로 private server (l2j) 진입 가능성 열어둠. legal/TOS 사용자 책임 명시.
4. 본 문서를 `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` "다른 launcher" 섹션 + `docs/DELTA_FORCE_MANJEOM_REVIEW.md` "유사 차단 사례" 섹션에 cross-link.

### 5-5. 미래 가능성 (조건부)

다음 조건 하나 이상 충족 시 재검토:
- NCSOFT 가 Lineage 2 / W / 2M / TL 에 user-mode AC variant 출시 (가능성 매우 낮음)
- Apple Silicon Mac 공식 지원 발표 (Lineage 2M iOS 는 이미 가능 → Mac M-series 자연 확대 가능)
- Lineage 2 KR live 의 GameGuard 정책 변경 (가능성 낮음)
- **Lineage 2 NEXT GEN UE5 remaster** (NC 발표) — UE5 + EAC 적용 시 Delta Force 운명, UE5 + 약한 AC 적용 시 검토 가치

---

## 6. 출처 (WebSearch 결과 인용)

- PURPLE — NCSOFT holds media event to officially introduce emulator software — MMO Culture: https://mmoculture.com/2019/11/purple-ncsoft-holds-media-event-to-officially-introduce-emulator-software/
- NCSoft Reveals In-House Mobile Emulator Called Purple Ahead Of Lineage 2M Launch — MMOs.com: https://mmos.com/news/ncsoft-reveals-in-house-mobile-emulator-called-purple-ahead-of-lineage-2m-launch
- NC America Unveils Shop-Streaming-Social 'Purple' Mega Launcher App For NCSoft Games — MMORPG.com: https://www.mmorpg.com/news/nc-america-unveils-shop-streaming-social-purple-mega-launcher-app-for-ncsoft-games-2000130366
- PURPLE for Windows — Uptodown: https://purple.en.uptodown.com/windows
- PURPLE for PC Windows — APKPure: https://windows.apkpure.com/purple
- NCSOFT Purple launcher: The Lobby crashes right away after logged in on Proton — GitHub Proton issue #7878: https://github.com/ValveSoftware/Proton/issues/7878
- NCSOFT Purple launcher (duplicate) — GitHub Proton issue #8515: https://github.com/ValveSoftware/Proton/issues/8515
- NCSOFT Launcher (Purple) log in black screen — Whisky-App discussion #724: https://github.com/orgs/Whisky-App/discussions/724
- PURPLE — Lutris: https://lutris.net/games/mc-purple-ncsoft/
- PurpleInstaller_NCS — any.run sandbox: https://any.run/report/59d3e823cfb051837581084d35fe0df3aaf1a7eb64c00bcaeb57e980eee49a56/76e48dd3-97e9-4b91-842d-e53b8c8ff9c1
- LOGIN : NC (PLAYNC) — login.plaync.com: https://login.plaync.com/nclogin/signin
- NC 인증기 — Google Play: https://play.google.com/store/apps/details?id=com.ncsoft.authenticator
- NCSOFT Google Authenticator — saaspass: https://saaspass.com/totp/ncsoft-google-authenticator-two-step-verification-2-one-time-password/
- Lineage (video game) — Wikipedia: https://en.wikipedia.org/wiki/Lineage_(video_game)
- Lineage — PCGamingWiki: https://www.pcgamingwiki.com/wiki/Lineage
- Lineage Classic — NamuWiki: https://en.namu.wiki/w/%EB%A6%AC%EB%8B%88%EC%A7%80%20%ED%81%B4%EB%9E%98%EC%8B%9D
- Lineage II — Wikipedia: https://en.wikipedia.org/wiki/Lineage_II
- Lineage II — PCGamingWiki: https://www.pcgamingwiki.com/wiki/Lineage_II
- Lineage II System Requirements — Lineage II Support: https://support.lineage2.com/hc/en-us/articles/204360739-Lineage-II-System-Requirements
- Lineage 2 Remastered Being Worked on at NCSoft as an Unreal Engine 4 Upgrade — wccftech: https://wccftech.com/lineage-2-remastered-being-worked-on-at-ncsoft-as-an-unreal-engine-4-upgrade/
- Lineage 2 NEXT GEN UE5 — YouTube: https://www.youtube.com/watch?v=Wm9odFLhHc0
- Lu4: Relaunch of Lineage 2 on Unreal Engine 4 — L2ukraine: https://l2ukraine.com/en/lu4_en/
- Lineage2M — App Store: https://apps.apple.com/us/app/lineage2m/id1575427154
- Lineage 2M Purple Installer — Softonic: https://lineage-2m-purple-installer.en.softonic.com/
- [UE4] Lineage 2M (PC) — Gildor's Forums (UE4 확인): https://www.gildor.org/smf/index.php?topic=6898.0
- Lineage W PC Download — NoxPlayer: https://www.bignox.com/blog/lineage-w-pc-download-how-to-play-lineage-w-on-pc-without-purple/
- Lineage W Pre-download Schedule — plaync.com: https://lineagew.plaync.com/sg/board/notice/view?articleId=6180a9b0f484107ec63a4ac8
- Throne and Liberty — Wikipedia: https://en.wikipedia.org/wiki/Throne_and_Liberty
- THRONE and Liberty — PCGamingWiki: https://www.pcgamingwiki.com/wiki/THRONE_and_Liberty
- Throne and Liberty Easy Anti Cheat (EAC) 47 Error — Deltia's Gaming: https://deltiasgaming.com/throne-and-liberty-easy-anti-cheat-eac-47-error-fix-explored/
- nProtect GameGuard — Wikipedia: https://en.wikipedia.org/wiki/NProtect_GameGuard
- nProtect GameGuard — PCGamingWiki: https://www.pcgamingwiki.com/wiki/NProtect_GameGuard
- XIGNCODE3 — PCGamingWiki: https://www.pcgamingwiki.com/wiki/XIGNCODE3
- Anti-cheat middleware — PCGamingWiki: https://www.pcgamingwiki.com/wiki/Anti-cheat_middleware
- AION update issue (PURPLE + nProtect + VIOLET, Gameforge + XIGNCODE3) — AreWeAntiCheatYet GitHub issue #1737: https://github.com/AreWeAntiCheatYet/AreWeAntiCheatYet/issues/1737
- AION CrossOver compatibility — CodeWeavers forum: https://www.codeweavers.com/compatibility/crossover/forum/aion?msg=63674
- Game Anti-Cheat Systems with CrossOver — CodeWeavers: https://support.codeweavers.com/anti-cheat
- Every game with kernel-level anti-cheat software (2026) — levvvel: https://levvvel.com/games-with-kernel-level-anti-cheat-software/
- Are We Anti-Cheat Yet?: https://areweanticheatyet.com/
- Easy Anti-Cheat: https://www.easy.ac/

---

*written 2026-05-01. PURPLE + Lineage 시리즈 만점 검토 — PURPLE shell (.NET CLR) closure 자체는 가능, 그러나 모든 live Lineage 가 nProtect/VIOLET/XIGNCODE3/EAC 중 하나 이상으로 차단. private 서버 (Lineage 1/2 legacy) 한정 만점 가능 — TOS 사용자 책임. D2R/D4 우선, PURPLE shell + Lineage 2 private 차순위.*

*revised 2026-05-01-rev2 — §0.5 정정 추가. 사용자 directive 로 L Classic r28 (2026 신작) / L M (PURPLE PC 정식) / L W 진행 결정. PURPLE 본질 = Android emulator 본체 정정. closure path B layer 재정립 (Layer 0=PURPLE.exe, Layer 1=APK, Layer 1'=native PE). 신규 candidates 16-18 추가 (cycle 49 batch 후보).*
