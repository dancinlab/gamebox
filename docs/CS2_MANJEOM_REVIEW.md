# Counter-Strike 2 (CS2) — gamebox 만점 기준 적용 가능성 검토

> **status**: 검토일 2026-05-01 / 갱신 **2026-05-01-rev2** (사용자 directive + WebSearch 후속)
> **만점 가능성 결론**: **부분가능 (offline path 만)** — closure path B 호환성은 **D2R 수준 ~70%**, 단 VAC 활성 multiplayer 는 **불가능 (own1 + 만점 정의 위반 회피 불가)**.
> 작성: Phase 0 검토 (모듈 구현 X)
> SSOT 보조: `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` 6 phase 모델 재사용

---

## ⚠️ 0.5 정정 / 추가 (rev2 — 2026-05-01 사용자 directive 후속 research)

### 0.5-1. 2026 VAC 강화 사항 (web search update)

기존 §2-1 의 VAC 검출 벡터 분석 후 발생한 update — 모두 **차단 강화 방향**, 결론 변경 X (오히려 강화):

| 사건 | 시점 | 변화 |
|---|---|---|
| **VAC Live 대규모 update** | 2025-09 | 역대 최대 crackdown — DMA card 식 외부 H/W cheat 까지 차단. cheat ecosystem 광범위 비활성 |
| **VAC 3.0 testing** | 2026-01 | VAC 신버전 wingman 모드 우선 deploy |
| **Wingman rage-hack 즉시 cancel** | 2026-01~ | 2v2 모드에서 rage-hack 감지 시 match 즉시 종료 + 자동 BAN |

→ **online VAC = 0%** 결론 강화. multiplayer 진입 위험 = 더 큼.

### 0.5-2. Steam client 없이 install + 게임 실행 가능 여부 (사용자 질문)

**기술 가능성**: **부분 가능 (offline path 만)** — gamebox closure path B 적용 시.

**Install (Steam client 미실행)**:
- Steam content depot 다이렉트 다운로드 (DepotDownloader / SteamCMD-like protocol, reverse-engineered) 가능
- cs2.exe + assets (~30 GB VPK pak) 다운로드는 Steam 계정 라이선스 token 만 있으면 가능
- 단 Steam 라이선스 우회는 TOS 회색 → 사용자 자발적 opt-in 필요

**Launch (Steam client 미실행)**:

| 의존성 | 우회 path | 가능? |
|---|---|---|
| `steam_api64.dll` | Goldberg-style stub (`SteamAPI_Init` no-op + `steam_appid.txt=730`) | ○ |
| `steamclient64.dll` IPC | tier0.dll Plat_* 의 일부 ownership 검증 → 추가 stub 필요 | △ reverse 비용 |
| steamstub DRM | CS2 미적용 (Valve 자체 게임 정책) | ○ 우호적 |
| ownership token | offline.txt 또는 cached token | ○ offline-only |

**Mode 별 실행 가능성**:

| Mode | Steam client 미실행 시 | 비고 |
|---|---|---|
| OFFLINE PRACTICE (`bot_add` + `sv_lan 1`) | **○ 가능** | gamebox 메인 use case |
| LAN bot match | ○ | Winsock loopback + Goldberg stub |
| Workshop maps offline | ○ | local pak 만 사용 |
| Online matchmaking | ✗ **불가능** | VAC + Trusted Mode + Steam 계정 binding 필요 |
| Community servers | ✗ | Steam server browser API 의존 |

**결론**: Steam client 0%, **offline 가능 ~80%** — Goldberg-style steam_api64 shim + tier0.dll Plat_* 추가 stub 만 있으면 cs2.exe 자체는 macOS 위 closure path B 환경에서 실행 가능 (offline only).

### 0.5-3. Multiplayer **보류** 확정 (사용자 directive 2026-05-01)

> 사용자 directive: "multiplayer 는 정의상 거부 때문에 보류"

본 검토 §2-1/2-2 의 결론 (own 1 + cheat-bypass 거부 정책상 multiplayer out-of-scope) 을 사용자가 명시 재확인 → **multiplayer 영역 모듈 작성 / 진입 시도 모두 영구 보류**.

해당 영역에서 보류된 항목:
- VAC bypass / Trusted Mode bypass / matchmaking 진입 시도
- Steam account 위협 행위
- VAC Live / VAC 3.0 우회 시뮬레이션 — 단순 profile 추적 (skeleton-only) 도 실 multiplayer 진입 의도 없으므로 후순위
- §4 candidate 10 `c_cs2_vac_avoid_guard` 는 **사용자 자산 보호 목적** (VAC secure server connect 자동 거부) 이므로 보류 X — 정책 가드 카테고리

### 0.5-4. 본 검토에서 진행 가능 영역 (보류 외)

§4 candidate list 에서 multiplayer 와 분리 가능한 offline-only / 보호 모듈만 진행:

| candidate | 영역 | 진행? |
|---|---|---|
| `c_cs2_steam_api64_offline_shim` (§4-6) | offline shim | ○ — offline launch prereq |
| `c_cs2_tier0_plat_shim` (§4-5) | Source 2 진입 prereq | ○ |
| `c_cs2_source2_pso_genome` (§4-1) | Phase 4 first frame | ○ — offline path |
| `c_cs2_vpk_asset_prefetch` (§4-7) | Phase 5 asset | ○ — offline path |
| `c_cs2_offline_bot_profile_cache` (§4-4) | Phase 5 bot match | ○ — offline 한정 |
| `c_cs2_dx11_vulkan_ab_fork` (§4-3) | Phase 5 path 비교 | ○ — offline 한정 |
| `c_cs2_shader_cache_warm` (§4-8) | Phase 4-5 cache | ○ — offline 한정 |
| `c_cs2_netstack_lan_loopback` (§4-9) | Phase 5 LAN bot | ○ — `sv_lan 1` 한정 |
| `c_cs2_vac_avoid_guard` (§4-10) | Phase 5 보호 | ○ — 사용자 자산 보호 |
| `c_cs2_subtick_input_latency` (§4-2) | Phase 5 input | △ — sub-tick 은 본질이 server 의존, offline 측정은 의미 제한 |

→ 9~10 candidates 진행 가능, 이전 §5-3 우선순위 그대로 유지.

### 0.5-5. cycle 50+ 후속 batch 후보 (사용자 보류 directive 반영)

multiplayer 영역 모듈 모두 제외, offline 만 진행 시:

- **cycle 51 후반 (5 모듈 — CS2 prereq + 보호)**: c_cs2_tier0_plat_shim, c_cs2_steam_api64_offline_shim, c_cs2_vac_avoid_guard, c_cs2_source2_pso_genome, c_cs2_vpk_asset_prefetch
- **cycle 52 (5 모듈 — CS2 offline path)**: c_cs2_dx11_vulkan_ab_fork, c_cs2_offline_bot_profile_cache, c_cs2_shader_cache_warm, c_cs2_netstack_lan_loopback, (TBD c_cs2_subtick_input_latency 또는 신규)

multiplayer 차단/추적 신규 candidates (web search 신규 발견 — 보류 directive 영향) :
- ~~`c_cs2_vac_live_2025_evolution_track`~~ — 보류 (multiplayer 영역)
- ~~`c_cs2_dma_card_block_simulate`~~ — 보류 (multiplayer 영역)
- ~~`c_cs2_wingman_ragehack_cancel_track`~~ — 보류 (multiplayer 영역)

### 0.5-6. 출처 추가 (rev2 web search)

- VAC Live 2025-09 update (Strafe.com): https://www.strafe.com/news/read/vac-live-update-devastates-cs2-cheating-community/
- VAC 3.0 testing 2026-01 (ggscore): https://ggscore.com/en/csgo/news/74878
- VAC Live + Wingman rage-hack 자동 cancel (csmarketcap, blix.gg): https://csmarketcap.com/blog/esports/new-vac-30-anti-cheat-will-change-the-cs2-scene
- CS2 macOS 미지원 확인 (MacGamingDB 2026): https://macgamingdb.app/games/730
- Goldberg Steam Emulator 패턴 (github inflation/goldberg_emulator): https://github.com/inflation/goldberg_emulator

---

---

## 0. 게임 메타데이터

| 항목 | 값 | 근거 |
|---|---|---|
| 출시 | 2023-09-27 | Wikipedia / Steam |
| 엔진 | **Source 2** | Valve Developer Wiki, Wikipedia |
| Windows binary | **PE32+ x86_64** (`game/bin/win64/cs2.exe`) | Steam 설치 경로, 32-bit 미지원 명시 |
| Linux binary | ELF x86_64 (native) | Wikipedia |
| **macOS native** | **없음 (영구 단종)** — 2024-01-01 부로 CS:GO 레거시 macOS 지원 종료 | MacRumors / Insider Gaming / Mac Gamer HQ |
| 그래픽스 API | **DX11 (default on Windows) + Vulkan (`-vulkan` opt-in)** — Source 2 는 Shader Model 5.0+ | Source 2 Wiki, CS2 Steam page, escorenews 비교 |
| CPU 요구 | SSE 4.2 필수 (32-bit 시스템 거부) | Source 2 Wiki |
| 핵심 모듈 | `cs2.exe` → `engine2.dll` + `client.dll` + `tier0.dll` (foundational) + `Plat_RegisterModule` 동적 등록 | a2x/cs2-dumper, deepwiki, danielkrupinski/cs2-anticheat |
| Steam DRM | `steam_api64.dll` 의존 + `SteamAPI_Init()` + steam_appid (Goldberg 우회는 multiplayer 한정 BAN risk) | github goldberg_emulator |
| **Anti-cheat** | **VAC + VAC Live + Trusted Mode** — 커널급 검사, kernel-level memory scan, signature DB, IAT-hook 검출, `NtQuerySystemInformation` 시스템 이미지 검증 | danielkrupinski/VAC, r0da blog, cs2.eu |
| Trusted Mode | 미인증 third-party DLL injection → "Insecure" 모드 강제 → secure server 접속 차단 | Steam Support FAQ |
| Offline 가능 여부 | **가능** — `PLAY > PRACTICE > OFFLINE PRACTICE` + 콘솔 `bot_add` + `sv_lan 1`. 단 LAN connect 측 일부 client-side abort 보고됨 | profilerr.net, blog.cs2.ad, Steam community thread |

### 해석 (Apple Silicon native 관점)

- CS:GO 는 macOS 64-bit 빌드(`bin/osx64/`) 가 존재했고, **Rosetta 2 로 Apple Silicon 동작 확인**. 그러나 CS2 는 Vulkan 종속도 + macOS 사용자 < 1% → **Valve 정책상 macOS 지원 영구 종료**.
- 결과: **CS2 는 macOS 정식 path 0** — gamebox 가 **Windows PE32+ x86_64** 를 직접 로드하는 own1 path 만 유효.
- D2R/D4 와 달리, **CS2 는 Battle.net 같은 launcher 가 없고 직접 Steam client 가 cs2.exe 를 spawn**. Steam-less launch 는 `steam_api64.dll` shim 필요.

---

## 1. 6 phase 별 만점 가능성 매트릭스

D2R/D4 로드맵의 6 phase 모델 재사용 (Phase 1=launcher install, Phase 2=launcher runtime, Phase 3=game install, Phase 4=first frame, Phase 5=gameplay, Phase 6=live tune).

| Phase | CS2 적용 | 만점 가능성 | 핵심 제약 |
|---|---|---|---|
| **P1** Steam client install | 스킵 가능 (사용자 기설치 가정) 또는 별 path | A=5 / B=4 / C=5 → 14 (보조) | Steam client 자체는 Battle.net 보다 단순 |
| **P2** Steam runtime | OAuth + 라이브러리 카탈로그 — Phase 2 D2R/D4 모듈과 **70% 재사용** | A=5 / B=5 / C=5 → **15** (3-4 후보) | Steam Web API 우회 — `steam_api64` shim |
| **P3** CS2 download | depot CDN 병렬 — D2R/D4 Phase 3 모듈 직접 재사용 | A=5 / B=4-5 / C=5 → 14-15 (2 후보) | Steam content depot 프로토콜 reverse |
| **P4-CS2** first frame | **Source 2 entry → DX11 path** (Vulkan opt-in 가능) — D2R 의 DX11 path 와 직접 호환 | A=5 / B=5 / C=5 → **15** (3 후보) | Source 2 PSO 패턴이 Diablo와 다름 — 신규 genome 매핑 필요 |
| **P5** gameplay | **두 갈래** — (a) **offline bot match** (만점 path 가능) / (b) **online matchmaking** (불가능, VAC) | (a) A=5 / B=5 / C=5 → **15** (5-7 후보) / (b) **0** | sub-tick 서버 아키텍처는 offline 에서 의미 없음 |
| **P6** live tune | offline 30+ 시간 측정 가능 (bot match dm_dust2 loop) | A=5 / B=5 / C=5 → **15** (1-2 후보) | data 패턴 D2R/D4 와 다름 — neon-sse, AMX 활용 angle 별도 |

### 누적 만점 도달 추정

- **Offline-only path** (bot + LAN): 만점 후보 **약 11-15 모듈** (D2R/D4 와 중복 6-8 개 제외 시 **신규 5-7**)
- **Online (VAC) path**: **0** — own1 의 PE loader 가 IAT-hook 검출 회피 불가 (VAC 가 명시적으로 IAT integrity 검증)

---

## 2. VAC + Steam DRM 차단 위험 평가

### 2.1 VAC 검출 벡터 — gamebox PE loader 와의 직접 충돌

VAC 는 다음을 스캔 (출처: danielkrupinski/VAC 분해 + r0da blog):

| 검출 벡터 | gamebox own1 PE loader 영향 | 위험도 |
|---|---|---|
| Memory signature scan (cheat hash DB) | gamebox 가 cheat 패턴은 안 만들지만, **DIY PE map 의 비표준 layout 자체가 anomaly signature 후보** | 중 (false positive 가능) |
| IAT integrity (`SaveSystemDllHandles` + WINAPI ptr) | **own1 IAT 는 100% reroute** — VAC 가 시스템 DLL 핸들과 함수 ptr 비교 시 즉시 mismatch | **치명** |
| Process / module enumeration (`NtQuerySystemInformation`) | gamebox 는 `cs2.exe` 를 host process 안에서 map 하므로, kernel 의 module list 에 **정상 LoadLibrary 등록 X** | **치명** |
| DNS cache scan + 최근 touch 파일 hash | gamebox 가 cheat 와 무관해도 비표준 file IO trace 가능 | 저-중 |
| GetVersion / 시스템 버전 | macOS 환경 → Windows API spoof 결과가 비-Windows kernel 시그니처 노출 위험 | 고 |
| Trusted Mode | 미서명 third-party 코드 injection → "Insecure" 강제 → matchmaking 차단 | 정의상 100% |

**결론**: VAC secure server 에 connect 하는 순간 own1 PE loader = **즉시 BAN 후보**. **만점 정의의 "Wine 0줄 + DIY hexa PE loader" 는 multiplayer 와 정의상 양립 불가**.

### 2.2 회피 가능성 — gamebox 정책상 **거부**

기술적으로 VAC bypass 는 (외부 자료상) thread hijacking + manual map injection + APC injection 으로 일부 우회 가능 (qlmshop, undercodetesting 2025 보고). 그러나:

1. **own1 + 정직성 정책상 cheat-bypass 카테고리 진입 X** — IDENTITY.md 정신 위반.
2. Valve 의 **Steam account BAN 영구화** — gamebox 사용자 자산 손실 risk.
3. gamebox 의 가치 = **개발자/연구자가 Apple Silicon 에서 Source 2 frame trace 측정** 이지, 경쟁전 진입 X.

**정책**: gamebox CS2 path 는 **offline practice + LAN bot match 한정**. multiplayer 는 명시적으로 out-of-scope.

### 2.3 Steam DRM (steam_api64.dll) — 우회 path

| 시나리오 | 가능성 | 비고 |
|---|---|---|
| Goldberg-style `steam_api64` shim (`SteamAPI_Init` stub + `steam_appid.txt`) | **가능** (offline-only) — github inflation/goldberg_emulator 패턴 적용 | **추정**: cs2.exe 의 ownership 검증 일부는 Steam client process IPC 의존 — 추가 stub 필요 |
| steamstub DRM 자체 | CS2 는 **steamstub 미적용** (대부분 Valve 자체 게임은 steamstub 없음 — vogons.org 보고) | **추정** 우호적 |
| Steam client process IPC | `tier0.dll Plat_*` 가 일부 ownership 검증 호출 가능 | reverse 필요, 미확인 |

**결론**: offline path 의 Steam DRM 차단 위험은 **D2R Battle.net offline mode 와 비슷한 수준 (가능, 단 reverse 필요)**.

---

## 3. closure path B 와의 호환성

D2R/D4 closure path B (own1 + DIY PE loader + Apple D3DMetal) 와의 모듈 재사용도:

| closure path B 컴포넌트 | CS2 호환 | 비고 |
|---|---|---|
| C-PE-MAP-x86_64 (PE32+ map) | **100%** | CS2 는 D4 와 같은 PE32+ x86_64 |
| C-IAT-RESOLVE-RUNTIME | **100%** | 단 export 함수 set 일부 다름 (Source 2 추가 의존) |
| C-WIN32-CALL-TRAMPOLINE (x86_64 SysV ↔ Win64 ABI) | **100%** | D4 와 동일 |
| DLL coverage 15/16 (kernel32, user32, advapi32, ws2_32 등) | **약 80%** | Source 2 가 `tier0.dll`, `vstdlib.dll` 등 Valve 자체 lib 추가 의존 — 신규 shim 필요 |
| dx_d3d11_stage1 (D3DMetal binding) | **100%** | CS2 default 가 DX11 — D2R 와 동일 path |
| dx_d3d12_stage2 (DXR) | **불필요** | Source 2 는 DX12/DXR 미사용 (DX11 + Vulkan 만) |
| Vulkan path | **신규 필요** | MoltenVK 또는 native Metal translation — **추정** D3DMetal 는 DX 만 cover, Vulkan 별도 |
| OAuth/Catalog (Battle.net) | **20% 재사용** | Steam 은 OAuth 아닌 Steam Web API — 별도 구현, 패턴은 비슷 |
| CDN download | **70% 재사용** | Steam content depot 프로토콜 신규, 멀티-mirror/병렬 chunk infra 재사용 |

**호환성 점수**: **약 70%** (D2R 수준).
- 강점: PE/IAT/DX11 path 직접 재사용
- 약점: **MoltenVK 또는 Source 2 의 Vulkan 의존 path** + Valve `tier0.dll` shim 신규
- DX11 default 사용 시 Vulkan path 는 옵셔널 — 진입 우호적

---

## 4. 추천 skeleton 모듈 candidate (5-10 개)

D2R/D4 와 중복되지 않는 **CS2 specific 신규 skeleton** 만 (만점 ROI 순):

| # | 모듈 | A | B | C | 만점 | phase | 비고 |
|---|---|---|---|---|---|---|---|
| 1 | **C-CS2-SOURCE2-PSO-GENOME** — Source 2 PSO → 60-byte hexagon 매핑 (Diablo D2 와 다른 레이아웃) | 5 | 5 | 5 | **15** | P4 | own2 D2 pso_genome 의 Source 2 변형 |
| 2 | **C-CS2-SUBTICK-INPUT-LATENCY** — sub-tick 서버 입력 timestamp 재해석 (offline practice 측정용) | 5 | 5 | 5 | **15** | P5 | CS2 만의 architectural feature, AMX/NEON 미가능 |
| 3 | **C-CS2-DX11-VULKAN-AB-FORK** — DX11 path 와 Vulkan path 의 동일 frame 측정 비교 → 자동 선택 | 5 | 5 | 5 | **15** | P5 | Source 2 만 양 path 동시 보유 |
| 4 | **C-CS2-OFFLINE-BOT-PROFILE-CACHE** — bot AI behavior 결정성 활용한 frame replay cache | 5 | 5 | 5 | **15** | P5 | bot match 의 high-determinism 활용 |
| 5 | **C-CS2-TIER0-PLAT-SHIM** — Valve `tier0.dll` `Plat_RegisterModule` / `Plat_GetRegisteredModules` 등 DIY shim | 5 | 4 | 5 | 14 | P4 | Source 2 진입 prerequisite |
| 6 | **C-CS2-STEAM-API64-OFFLINE-SHIM** — `SteamAPI_Init` stub + AppID 730 + offline.txt path (Goldberg 패턴) | 5 | 4 | 5 | 14 | P3-P4 | offline-only |
| 7 | **C-CS2-VPK-ASSET-PREFETCH** — Source 2 VPK pak 의 map-specific asset 예측 prefetch (de_dust2 / de_mirage / de_inferno) | 5 | 5 | 5 | **15** | P5 | C 축 강점 (forecast) |
| 8 | **C-CS2-SHADER-CACHE-WARM** — Source 2 shader cache 의 macOS Metal lib 으로 미리 변환 cache | 5 | 5 | 5 | **15** | P4-P5 | Apple D3DMetal hint 활용 |
| 9 | **C-CS2-NETSTACK-LAN-LOOPBACK** — `sv_lan 1` 환경에서 Winsock → loopback 최적화 (offline LAN bot) | 5 | 5 | 5 | **15** | P5 | LAN bot 양립 |
| 10 | **C-CS2-VAC-AVOID-GUARD** — VAC 감지 시 자동 secure server connect 거부 (정책 가드, 사용자 자산 보호) | 5 | 4 | 5 | 14 | P5 | 보조 — own1 정직성 가드 |

**만점 (15) 후보**: 7 개 / **보조 (14)**: 3 개.
**누적 추가**: 만점 +7 (현 D2R/D4 100 base 에 추가 가능 시).

---

## 5. 결론: 우선순위

### 5.1 만점 가능성

- **Offline path (bot + LAN)**: **부분가능 ~70%** — D2R 수준의 closure path B 호환성, 신규 만점 후보 7 개
- **Online path (matchmaking)**: **불가능 0%** — VAC + Trusted Mode 와 own1 + DIY PE loader 가 정의상 양립 불가

### 5.2 D2R/D4 와의 ROI 비교

| 게임 | 만점 신규 후보 | closure path B 재사용도 | macOS native 가능성 | online 가능 여부 |
|---|---|---|---|---|
| **D2R** | ~3 (Phase 4-D2R) | 100% | 높음 (offline 가능) | 부분 (Battle.net offline mode) |
| **D4** | ~2 (Phase 4-D4) | 100% (DX12+DXR 활용 활성) | 낮음 (online-only DRM) | 필수 (offline 불가) |
| **CS2** | ~7 (Phase 4-5 신규) | 70% (Source 2 + tier0 shim 신규) | 0% (영구 단종) | 부분 (offline bot 만, online VAC BAN risk) |

→ **만점 ROI**: CS2 offline path 가 단일 게임 기준 **D2R/D4 보다 만점 후보 수치는 높음 (+7 vs +3/+2)**, 그러나:
- **prerequisite 비용 큼** (tier0/vstdlib shim + Source 2 PSO genome 신규 + steam_api64 shim)
- **online 사용자 가치 0** — 실 경쟁전 사용 불가
- **online MP 가 CS2 의 본질** — offline-only gamebox 는 fan/dev 시연 가치만

### 5.3 우선순위 권고

**T1 (즉시 진입 X — 후순위 보류)**: D2R/D4 가 closure 까지 cycle 38~ 약 7-10 개월 소모 예정. CS2 진입은 **D2R first frame validated 후** (Phase 4-D2R done) 검토.

**T2 (향후 진입 시 path)**:
1. **C-CS2-TIER0-PLAT-SHIM** + **C-CS2-STEAM-API64-OFFLINE-SHIM** — prerequisite skeleton 2 개
2. **C-CS2-SOURCE2-PSO-GENOME** + **C-CS2-VPK-ASSET-PREFETCH** — manjeom +2
3. **C-CS2-DX11-VULKAN-AB-FORK** + **C-CS2-SHADER-CACHE-WARM** — manjeom +2
4. **C-CS2-OFFLINE-BOT-PROFILE-CACHE** + **C-CS2-SUBTICK-INPUT-LATENCY** + **C-CS2-NETSTACK-LAN-LOOPBACK** — manjeom +3
5. **C-CS2-VAC-AVOID-GUARD** — 보조 (정책 가드, 사용자 보호)

**T3 (거부 영역 — 명시적 out-of-scope)**:
- VAC bypass / Trusted Mode bypass / matchmaking 진입
- Steam account 위협 행위
- 어떤 형태의 cheat 시그니처

### 5.4 정직성 메모

- 본 검토는 WebSearch 결과 기반 (Wikipedia / Valve Wiki / Steam community / cs2.eu / r0da blog / danielkrupinski github 등 9개 도메인). 실 cs2.exe 분해 X.
- "Source 2 의 macOS 가능성" 은 **CS:GO 가 macOS 64-bit Rosetta 2 동작 가능했다는 사실** 에서 추정 — CS2 자체는 macOS native 빌드 자체가 없으므로 own1 PE loader path 가 유일.
- 만점 후보 7 개 수치는 skeleton 단계 추정. validated 까지의 비용은 **D2R first frame 비용과 비슷 (2-3 개월)** 으로 추정.
- VAC 의 IAT 검증 메커니즘은 외부 분석 기반 — Valve 의 실 구현은 비공개. own1 PE loader 와의 실 충돌도는 **추정 "치명"**.
- "online 0%" 결론은 정의상의 결론 (own1 + cheat-bypass 거부 정책). 기술적 우회 가능성과 별개.

---

## 출처 (WebSearch 2026-05-01)

- Source 2 — https://developer.valvesoftware.com/wiki/Source_2
- Counter-Strike 2 — https://en.wikipedia.org/wiki/Counter-Strike_2
- Source 2 (Wikipedia) — https://en.wikipedia.org/wiki/Source_2
- CS2 macOS 단종 — https://www.macrumors.com/2023/10/10/valve-confirms-counter-strike-2-no-macos/
- Mac Gamer HQ CS2/macOS — https://www.macgamerhq.com/games/counter-strike-2-mac/
- Insider Gaming Mac/old-PC EOL — https://insider-gaming.com/counter-strike-2-ending-support/
- VAC Wikipedia — https://en.wikipedia.org/wiki/Valve_Anti-Cheat
- VAC reverse (danielkrupinski) — https://github.com/danielkrupinski/VAC
- VAC module loading (r0da) — https://whereisr0da.github.io/blog/posts/2021-03-10-quick-vac/
- CS2 anti-cheat extracted — https://github.com/danielkrupinski/cs2-anticheat
- Steam Trusted Mode — https://help.steampowered.com/en/faqs/view/09A0-4879-4353-EF95
- CS2 anti-cheat 설명 — https://cs2.eu/news/cs2-anti-cheat-explained-how-vac-and-trust-factor-work
- CS2 offline practice 가이드 — https://profilerr.net/how-to-play-cs2-offline-without-internet-with-friends-or-bots-full-guide/
- CS2 offline 가이드 — https://blog.cs2.ad/cs-go-offline/
- CS2 dumper (deepwiki) — https://deepwiki.com/a2x/cs2-dumper/2.1-client.dll-offsets
- CS2 dumper installation — https://deepwiki.com/a2x/cs2-dumper/1.1-installation-and-setup
- CS2 .exe 위치 — https://tradeit.gg/blog/cs2-exe-location/
- CS2 dedicated server wiki — https://developer.valvesoftware.com/wiki/Counter-Strike_2/Dedicated_Servers
- Vulkan vs DX11 비교 — https://escorenews.com/en/csgo/article/50857-dx11-and-vulkan-best-fps-comparison-for-cs2-beta-how-to-switch-from-directx-to-vulkan
- CS2 Vulkan 활성 가이드 — https://steamcommunity.com/sharedfiles/filedetails/?id=3198674462
- CS:GO Apple Silicon (DoesItARM) — https://doesitarm.com/game/csgo
- Goldberg Steam Emulator — https://github.com/inflation/goldberg_emulator
- Steam emulator README — https://github.com/su6ur6an/goldberg_emulator/blob/master/Readme_release.txt

---

*written 2026-05-01 — closure path B 의 D2R/D4 후속 검토 1 차. 모듈 코드 X, skeleton 진입은 T1 D2R first frame validated 후 재검토.*

*revised 2026-05-01-rev2 — §0.5 정정/추가. 2026 VAC 강화 (VAC Live 2025-09, VAC 3.0 2026-01, Wingman rage-hack 자동 cancel) 반영. Steam client 없이 install + game launch 가능성 분석 (offline path ~80%, Goldberg-style shim + tier0 stub 필요). 사용자 directive: multiplayer 정의상 거부 → 영구 보류 확정. cycle 50-52 batch 후보 multiplayer 영역 모듈 전부 보류.*
