# World of Warcraft (WoW) — gamebox 만점 기준 적용 가능성 검토 + retired_intentional 정식 채택 mk1 narrative

> **status**: 검토일 **2026-05-03** / 작성: Phase 0 검토 (mk1 narrative spec freeze, 모듈 구현 X)
> **만점 가능성 결론**: **불가능 (online MMO subscription + always-online 정의상 차단)** — closure path B 호환성은 D2R 수준 ~80% (DX11 default + DX12 opt-in 활용 가능), 단 **WoW subscription + always-online MMO 가 정의상 own1 + offline 정의 위반 회피 불가**.
> **status enum**: `retired_intentional` **정식 채택** (block-acknowledged honest documentation = PASS-equivalent posture, own1 Wine 0 + own2 honest emit, no subscription circumvention).
> SSOT 보조: `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` 6 phase 모델 재사용 / `docs/D2R_D4_COMPARISON.md` 비교 축 / `docs/BATTLENET_REAL_LAUNCH_PLAN.md` 상위 launcher / `docs/D4_MANJEOM_REVIEW.md` peer always-online 가족 first-mover

---

## 0. 게임 메타데이터

| 항목 | 값 | 근거 |
|---|---|---|
| 정식 명칭 | World of Warcraft (WoW) — retail (현 expansion) + WoW Classic | Blizzard Entertainment 공식 |
| 개발사 / 퍼블리셔 | Blizzard Entertainment / Activision Blizzard | 공식 |
| 출시 (retail) | 2004-11-23 (지속 expansion 2026 까지 누적) | 공식 |
| 출시 (Classic) | 2019-08-26 (vanilla rerelease) | 공식 |
| 엔진 | Blizzard 자체 엔진 (WoW 시리즈 계보, retail vs classic 분리) | Blizzard tech blog 추정 |
| Windows binary | **PE32+ x86_64** (`Wow.exe` retail / `WowClassic.exe` classic) | 설치 경로 기준 |
| 그래픽스 API | **DirectX 11 (default)** + **DirectX 12 (opt-in)** — Vulkan 부재, Metal native 부재 | Blizzard 공식 system requirements |
| 시스템 사양 (권장) | i7-9700 / Ryzen 7 2700+, 16 GB RAM, GTX 1080 / RX 5700 XT+ | Blizzard 공식 |
| **distribution** | **Battle.net 만 (PC 메인)** — Steam 부재, MS Store 부재 | Blizzard 공식 |
| **Battle.net 의존** | 필수 — WoW launch 는 Battle.net Agent → game install path → online realm select 통과 후 진입 | Blizzard 공식, 본 repo `.roadmap.battlenet` |
| **subscription** | **필수 (월 단위 결제)** — 30-day free trial 후 정기 결제 또는 game time token, sub 미결제 시 character login 거부 | Blizzard 공식 |
| **always-online** | **필수 (MMO 정의상)** — 모든 zone / instance / dungeon / raid 측 server 의존, single-player offline mode 정식 부재 | 공식 (MMO 정의) |
| anti-cheat | (kernel-level driver 미사용 — Battle.net OAuth + server-side validation + Warden client-side check) | 추정, Warden 부분 공개 |
| DRM 우회 정식 path | **부재** — D2R `start_in_offline_mode.exe` 같은 정식 toggle 없음, Elden Ring `start_game_in_offline_mode.exe` 같은 정식 toggle 없음 | 공식 / WebSearch 종합 |
| macOS 공식 지원 | **있음 (retail + classic 모두)** — Apple Silicon native (Universal 2 binary, AVX-2 emulation 측 Rosetta 부분) | Blizzard 공식 |
| macOS 비공식 우회 | **불필요** — 공식 native macOS client 존재, gamebox 의 own1 PE loader path 와는 별개 | applegamingwiki 기록 |

### 해석 (Apple Silicon native + own1 관점)

- WoW 은 **online MMO subscription 의존도가 본질적 게임 디자인 요소** — D4 always-online 보다 더 강한 종속 (subscription 측 결제 gate 추가).
- 결과: WoW closure path B 의 **cond.1 (Wow.exe + WowClassic.exe PE+IAT+CRT) / cond.2 (D3D11+D3D12 first-frame) / cond.3 (Battle.net 통과 + subscription gate)** 3 cond 모두 always-online + subscription 통과 transitive block.
- D2R (peer game in `airgenome-gamebox`) 는 **offline mode handshake 가능** (Battle.net launcher 통과 후 single-player 측 offline 가능) — WoW 와의 구조적 차이 (single-player mode 자체 부재).
- D4 (peer always-online family) 는 single-player campaign 측 always-online 의존, WoW 은 MMO 정의상 모든 mode 측 always-online + subscription 의존 — **WoW = D4 always-online family 측 더 강한 종속 변종**.
- WoW 측 macOS 공식 native client 존재 (Apple Silicon Universal 2) — 그러나 본 repo gamebox 는 **Windows PE binary 측 own1 hexa loader 경로** 에 집중하므로, macOS native 측 별도 path (Apple-native 측 own1 의미 X) 는 본 doc scope 외.
- own1 (Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0) 정의상, WoW 측 subscription + always-online 측 honest documentation = `retired_intentional` 정식 채택이 정합.

---

## 0.5 정정 / 추가 (2026-05-03 cycle, retired_intentional 정식 채택)

### 0.5-1. 본 cycle 의 변화 (predecessor 11 .roadmap.<game> fan-out land + D4 retired_intentional first-mover 후속)

| 항목 | predecessor (2026-05-03 fan-out) | 본 cycle (Track C 4-game retired_intentional 채택) |
|---|---|---|
| `.roadmap.wow` blk.1 `status_enum_candidate` | `retired_intentional` (후보) | (유지) |
| `.roadmap.wow` blk.1 `status_enum_adopted` | (없음) | **`retired_intentional` 신규 add** |
| `.roadmap.wow` blk.1 `retired_intentional_posture` | (없음) | **`block_acknowledged_pass_equivalent` 신규 add** |
| 3 cond evidence rows (block-acknowledged 명시) | 부재 | **append 1 row each (`block_acknowledged_pass_equivalent_per_own1_wine_0_no_subscription_bypass`)** |
| docs mk1 narrative spec freeze | 부재 (가칭만) | **본 doc land** |
| `blocker_class` blk.1 측 명시 | (header 측 `blocker_class: online_mmo_subscription` 만) | **`online_mmo_subscription` (blk.1 측 add)** |

### 0.5-2. retired_intentional posture 정의 (own1 정합)

D4 first-mover 정의 (`docs/D4_MANJEOM_REVIEW.md` §0.5-2) 와 동일 정의 적용:

- **Block-acknowledged**: 차단 사유 명시 + 차단 honest documentation
- **PASS-equivalent**: own1 + own2 정합 측 cond.N 측정 의도 자체가 retired (정의상 PASS X but honest 측면 PASS-equivalent)
- **No subscription circumvention**: 결제 우회 코드 emit 0건, character data 위조 X, server-side subscription validation bypass X
- **No always-online evasion**: realm server emulator 자체 구현 시도 X, Warden client-side check 우회 X, login token forge X

→ `retired_intentional` 의 의미: "owned game 측 정합 측정 X (subscription + always-online 영구 경계), 그러나 honest documentation 측 honest = PASS-equivalent posture".

### 0.5-3. cross-link to peer Track C 3 anti-cheat games (CS2 / DF / ER) + D4 first-mover

본 cycle Track C (4-game retired_intentional) 측 4 게임 동시 land:

- **CS2** (VAC user-mode AC) — `docs/CS2_MANJEOM_REVIEW.md` REUSE
- **Delta Force** (EAC kernel-mode AC) — `docs/DELTA_FORCE_MANJEOM_REVIEW.md` REUSE
- **Elden Ring** (EAC anti-tamper) — `docs/ELDEN_RING_MANJEOM_REVIEW.md` REUSE
- **WoW** (online MMO subscription + always-online) — **본 doc NEW**

→ Track C 4 게임 측 WoW 만 신규 mk1 narrative doc 작성 (sibling 3 게임 측 predecessor 측 기존 land doc REUSE). D4 first-mover (`docs/D4_MANJEOM_REVIEW.md`, 2026-05-03 Track A2) 측 always-online family precedent 정합.

### 0.5-4. retired_intentional adoption 5+1 status (predecessor `.roadmap.<game>` 5-game candidate set + WoW lock-in)

| 게임 | blocker | type | 정식 채택 시점 | mk1 narrative doc |
|---|---|---|---|---|
| diablo4 (D4) | diablo4.blk.1 | always_online_drm | **2026-05-03** (Track A2 first-mover) | `docs/D4_MANJEOM_REVIEW.md` (NEW 2026-05-03) |
| lineage_classic_r28 | lineage_classic_r28.blk.1 | gameguard_kernel_ac | **2026-05-03** (Track B 병렬) | `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` (NEW 2026-05-03) |
| cs2 | cs2.blk.1 | vac_anticheat | **2026-05-03** (Track C 본 cycle) | `docs/CS2_MANJEOM_REVIEW.md` (REUSE) |
| delta_force | delta_force.blk.1 | eac_kernel_anticheat | **2026-05-03** (Track C 본 cycle) | `docs/DELTA_FORCE_MANJEOM_REVIEW.md` (REUSE) |
| elden_ring | elden_ring.blk.1 | eac_anti_tamper | **2026-05-03** (Track C 본 cycle) | `docs/ELDEN_RING_MANJEOM_REVIEW.md` (REUSE) |
| **wow** (6th) | wow.blk.1 | online_mmo_subscription | **2026-05-03** (Track C 본 cycle, 6th lock-in) | **`docs/WOW_MANJEOM_REVIEW.md` (NEW 2026-05-03)** |

→ 본 cycle 후: 5/5 original candidate + WoW = **6 게임 정식 채택 완료**.

---

## 1. 6 phase 별 만점 가능성 매트릭스

D2R/D4 로드맵의 6 phase 모델 재사용 (Phase 1=launcher install, Phase 2=launcher runtime, Phase 3=game install, Phase 4=first frame, Phase 5=gameplay, Phase 6=live tune).

| Phase | WoW 적용 | 만점 가능성 | 핵심 제약 |
|---|---|---|---|
| **P1** Battle.net Setup install | spec only — `.roadmap.battlenet` cond.2 transitive | A=5 / B=4 / C=5 → 14 | Agent SD install path, signature check |
| **P2** Battle.net runtime (Agent SD + REST) | spec only — `.roadmap.battlenet` cond.1 transitive | A=5 / B=5 / C=5 → **15** | Agent SD process forging, OAuth 2-leg, login state |
| **P3** WoW download + install (retail + classic 2 binary) | depot CDN — D2R/D4 와 chunk 인프라 공유 | A=5 / B=4 / C=5 → 14 | Battle.net Agent depot 프로토콜 (retail vs classic 분기) |
| **P4** first frame (D3D11 default / D3D12 opt-in) | **Apple D3DMetal stage1 (D3D11) + stage2 (D3D12) 활용 가능 영역** | A=5 / B=5 / C=5 → **15** (만점 후보 2) | always-online + subscription 통과 후 측정 가능 (block 측 측정 X = `retired_intentional`) |
| **P5** gameplay (MMO live world) | open world / dungeon / raid — live server state 의존 | A=5 / B=5 / C=5 → **15** (만점 후보 ≥5) | live game state 측정 = always-online + subscription 의존 |
| **P6** live tune | 30+ 시간 측정 = subscription 활성 시기 한정 | A=5 / B=5 / C=5 → **15** | 측정 cycle 측 subscription 결제 cycle 정합 |

### 누적 만점 도달 추정

- **always-online + subscription OK 가정 (정의상 own1 위반)**: 만점 후보 **약 5-8 모듈** (현 land 2 c_wows_* perf module + spec freeze 후 추가 가능)
- **실 closure path B (own1 + offline 정의)**: **0** — always-online + subscription 측 cond.1/2/3 모두 transitive block, `retired_intentional` 정식 채택

---

## 2. online MMO subscription + always-online 차단 위험 평가 (own1 정합 측면)

### 2.1 Battle.net + WoW realm 통과 흐름 — gamebox PE loader 와의 직접 충돌

WoW launch 흐름 (공식 + WebSearch 추정):

| step | 동작 | own1 closure path B 영향 |
|---|---|---|
| 1. Battle.net Setup install | `.roadmap.battlenet` cond.2 (Agent SD install) | offline shim 가능 측 가능 (Track A scope) |
| 2. Battle.net Agent SD + REST | `.roadmap.battlenet` cond.1 (Agent SD forging) | offline shim 가능 측 가능 (Track A scope) |
| 3. OAuth 2-leg login | `.roadmap.battlenet` cond.3 (login + chat) | offline shim 측 token forge 측 own1 위반 → 거부 |
| 4. WoW game catalog 조회 | Agent SD REST | offline shim 가능 측 가능 |
| 5. WoW download depot (retail vs classic) | Battle.net Agent CDN | depot prefetch 측 가능 (retail vs classic 분기) |
| 6. Wow.exe / WowClassic.exe launch | Agent → Wow*.exe child process | spec only — own1 측 PE loader 측 가능 |
| 7. **realm server connect (always-online + subscription check)** | server side validation (subscription valid?) | **closure path B 차단 — own1 측 우회 X** |
| 8. character login + zone load | live realm state | always-online + subscription 의존 — offline 측 측정 X |
| 9. open world / dungeon / raid play | live MMO server state | always-online 의존 — offline 측 측정 X |

**결론**: step 7 이 own1 closure path B = **즉시 retired_intentional candidate**. **만점 정의의 "Wine 0줄 + DIY hexa PE loader" 는 always-online + subscription WoW 와 정의상 양립 불가**.

### 2.2 회피 가능성 — gamebox 정책상 **거부**

기술적으로 WoW always-online + subscription 우회는 외부 자료상 (a) private server emulator (TrinityCore / AzerothCore / CMaNGOS 등 community) + (b) login token forge + (c) Warden client-side check 우회 측 일부 가능 (외부 community 보고). 그러나:

1. **own1 + 정직성 정책상 subscription bypass / private server emulator 카테고리 진입 X** — IDENTITY.md 정신 위반.
2. Blizzard 의 **Battle.net account BAN 영구화** — gamebox 사용자 자산 손실 risk + WoW character data 손실 risk.
3. gamebox 의 가치 = **개발자/연구자가 Apple Silicon 에서 WoW frame trace 측정 spec** 이지, 실 always-online MMO 진입 X.
4. private server emulator 측 본 repo 측 emit 0건 — server 자체 구현 시도 X.

**정책**: gamebox WoW path 는 **`retired_intentional` 정식 채택 (block-acknowledged honest documentation = PASS-equivalent posture)**. subscription circumvention / private server emulator / always-online 우회는 명시적으로 out-of-scope.

### 2.3 Battle.net launcher (peer offline shim) — Track A 와의 관계

| 시나리오 | 가능성 | 비고 |
|---|---|---|
| `.roadmap.battlenet` cond.1 (Agent SD forging) offline shim | **가능 (Track A)** | offline shim 측 cond.1/cond.2 PASS 가능 |
| `.roadmap.diablo2_resurrected` offline mode | **가능 (Track A)** | D2R 정식 offline mode handshake (Battle.net 통과 측 optional) |
| `.roadmap.diablo4` always-online | **불가능 (Track A2 — D4 retired_intentional 정식 채택)** | always-online DRM (single-player campaign 포함) |
| `.roadmap.wow` always-online + subscription | **불가능 (본 cycle scope, 6th lock-in)** | always-online MMO + subscription gate (D4 보다 더 강한 종속) |

**결론**: WoW = D4 always-online family 측 가장 강한 종속 변종 (subscription 추가). D2R 와의 차이 = **D2R 측 offline mode 정식 path 존재 / WoW 측 always-online MMO 정의상 정식 toggle 부재 + subscription 결제 gate 추가**.

### 2.4 own1 정합 100% 정합 audit

- **Wine 0**: 본 cycle 측 Wine / Proton / CrossOver / Whisky / GPTK 의존 0건
- **subscription circumvention 0**: subscription token forge / character data 위조 / server-side validation bypass 측 emit 0건
- **always-online evasion 0**: realm server emulator 자체 구현 X, Warden client-side check 우회 X, login token forge X
- **Wow.exe / WowClassic.exe direct launch 시도 0**: Battle.net Agent 통과 X 측 Wow*.exe 단독 launch 측 own1 측 시도 X (always-online + subscription check 측 fail 측 측정 의미 0)
- **2 c_wows_* perf module spec only**: skeleton-stage 만, live game state 측 측정 0건 (alias overlap caveat — wows = World of Warships 측 perf module name 측 본 repo design decision 측 honest reflect)

→ ✓ **own1 + 만점 정의 정합 100%**.

---

## 3. closure path B 호환성

D2R/D4 closure path B (own1 + DIY PE loader + Apple D3DMetal) 와의 모듈 재사용도:

| closure path B 컴포넌트 | WoW 호환 (block 가정 시) | 비고 |
|---|---|---|
| C-PE-MAP-x86_64 (PE32+ map) | **100%** | D4 와 같은 PE32+ x86_64 (retail + classic 동일 ISA) |
| C-IAT-RESOLVE-RUNTIME | **100%** | DLL set 일부 다름 (WoW 측 자체 audio + network lib 추가) |
| C-WIN32-CALL-TRAMPOLINE (x86_64 SysV ↔ Win64 ABI) | **100%** | D2R 와 동일 |
| DLL coverage 15/16 (kernel32, user32, advapi32, ws2_32 등) | **약 90%** | WoW 측 자체 ws2_32 사용 + Warden client-side check |
| dx_d3d11_stage1 (D3DMetal binding) | **100%** (WoW default DX11 path) | WoW default 가 DX11 — D2R 와 동일 path |
| dx_d3d12_stage2 (PSO + DXR) | **70%** (WoW DX12 opt-in path, DXR 미사용) | WoW DX12 opt-in 측 PSO 활용 가능, DXR 부재 |
| Vulkan path | **불필요** | WoW 측 Vulkan 부재 |
| Battle.net launcher OAuth/Catalog | **0% (정의상 거부)** | always-online + subscription check 측 own1 측 우회 X |
| Battle.net depot CDN | **70% 재사용** (depot 프로토콜 spec only) | D2R/D4 와 chunk 인프라 공유, 단 download 측 always-online check 측 우회 X |
| WoW realm server protocol | **0% (정의상 거부)** | private server emulator 시도 X |

**호환성 점수**: **(block 가정 시) 80%** — WoW 측 DX11 default + DX12 opt-in 양 path 활용 가능 (DXR 부재 측면 D4 보다 약). **(retired_intentional 채택 시) 정의상 measurable 0**, honest documentation = PASS-equivalent posture.

---

## 4. 추천 skeleton 모듈 cluster (predecessor 측 land 2 개 + alias caveat)

WoW 측 land 된 perf module (predecessor `.roadmap.wow` cross_link.perf_pattern) — alias overlap honest reflect:

| # | 모듈 | A | B | C | 만점 | phase | 비고 |
|---|---|---|---|---|---|---|---|
| 1 | `c_wows_port_warmup` | 5 | 5 | 5 | **15** | P4 | port warmup spec (alias = World of Warships 측 perf module 측 본 repo design decision) |
| 2 | `c_wows_replay_playback` | 5 | 5 | 5 | **15** | P5 | replay playback spec (alias overlap caveat 동일) |

**alias overlap honest reflect**: `c_wows_*` perf module 측 "wows" prefix 는 **World of Warships sibling** 의 약자 — 본 repo airgenome-gamebox 측 module naming 측 honest reflect (`.roadmap.wow` cross_link.perf_pattern 측 명시). WoW (World of Warcraft) 측 별도 module prefix 추가 없이 alias share 측 design decision (별도 future cycle 측 `c_wow_*` rename 가능성 caveat).

→ **총 2 모듈, 모두 만점 (15) 후보 spec only (skeleton 단계)**. live game state 측 validated 측 always-online + subscription 의존 = `retired_intentional` posture 측 정합.

### 4.1 향후 추가 가능 candidate (skeleton 측 별도 future cycle)

WoW 측 추가 가능 perf module candidate (skeleton 측 별도 future cycle):

| # | 모듈 (가칭) | 영역 | retired_intentional 정합 |
|---|---|---|---|
| 1 | `c_wow_dx11_stage1_first_frame` | P4 first-frame DX11 | spec only, live X |
| 2 | `c_wow_dx12_pso_warm` | P4 first-frame DX12 opt-in | spec only, live X |
| 3 | `c_wow_classic_legacy_renderer` | classic 측 legacy renderer track | spec only, live X |
| 4 | `c_wow_realm_handshake_offline_shim` | offline shim spec only | (subscription circumvention 거부, spec only honest) |
| 5 | `c_wow_warden_check_avoid_track` | Warden client-side check avoid (정책 가드) | (보호 카테고리, 사용자 자산 보호) |

→ skeleton 단계 candidate, validated 0 — retired_intentional posture 측 정합 (live X, synthetic-data only).

---

## 5. retired_intentional 정식 채택 rationale

### 5.1 결정 evidence

| evidence | 출처 |
|---|---|
| online MMO subscription + always-online 정의상 own1 차단 | Blizzard 공식 (WoW launch 흐름) |
| `start_in_offline_mode.exe` 같은 정식 toggle 부재 (single-player mode 자체 부재) | 공식 / WebSearch 종합 |
| Wow.exe / WowClassic.exe 단독 launch 시 always-online + subscription check fail | 추정 (공식 자료 부재, WebSearch community 보고) |
| `.roadmap.wow` blk.1 `status_enum_candidate: retired_intentional` 명시 (predecessor 2026-05-03) | `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` §`retired_intentional_enum_not_applied_6` 측 lock-in pending 명시 |
| WoW 2 c_wows_* perf cluster spec only land 완료 (skeleton-stage) | predecessor `.roadmap.wow` cross_link.perf_pattern |
| D4 always-online retired_intentional 정식 채택 (peer family precedent, 2026-05-03 Track A2) | `docs/D4_MANJEOM_REVIEW.md` + `state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker` |
| D2R offline mode 정식 (peer game) | 본 repo `.roadmap.diablo2_resurrected` cond.3 |

→ **retired_intentional 정식 채택 = honest documentation 정의 측 정합, D4 always-online family precedent 정합, D2R offline mode 정식 path 와 명확 분리**.

### 5.2 정식 채택 후 의미

- `.roadmap.wow` blk.1 status: `open` (유지) + `status_enum_candidate: retired_intentional` (유지) + **`status_enum_adopted: retired_intentional` (신규 add)** + **`retired_intentional_posture: block_acknowledged_pass_equivalent` (신규 add)** + **`adopted_iso: 2026-05-03` (신규 add)** + **`adopted_via: track_c_4game_retired_intentional_adoption_landed` (신규 add)** + **`blocker_class: online_mmo_subscription` (blk.1 측 add)**
- 3 cond.N status: `partial` (유지) + 3 cond evidence array 측 **`block_acknowledged_pass_equivalent_per_own1_wine_0_no_subscription_bypass` row append**
- docs/WOW_MANJEOM_REVIEW.md (본 doc) — mk1 narrative spec freeze land
- 향후 cycle 측 2 c_wows_* perf module 측 self_test() validation 진행 가능 (synthetic-data 측 — live game state 측 측정 X)

### 5.3 정식 채택 X 시 vs 채택 시 비교

| 비교 축 | retired_intentional 미채택 | retired_intentional 정식 채택 (본 cycle) |
|---|---|---|
| `.roadmap.wow` blk.1 status | `open` (영구 unmet 상태) | `open` + `status_enum_adopted: retired_intentional` (block-acknowledged PASS-equivalent) |
| 3 cond.N status | `partial` (영구 transitive block) | `partial` + block-acknowledged 명시 (honest documentation) |
| own1 정합 | (애매 — block 측 status 이름 부재) | **명확 (retired_intentional posture)** |
| own2 정합 | (애매) | **명확 (honest emit, block-acknowledged)** |
| 향후 cycle 측 측정 의도 | (불명확 — always-online + subscription 측 측정 의지) | **명확 (live game state 측정 X, 2 c_wows_* perf module synthetic-data only)** |
| 사용자 친화 (handoff) | (현 status 이해 어려움) | **명확 (retired_intentional 정의 명시)** |
| D4 always-online family precedent 정합 | (미정합) | **정합 (D4 first-mover + WoW 6th lock-in peer)** |

→ retired_intentional 정식 채택 = **honest documentation 정의 측 정합 + own1/own2 정합 + 사용자 친화 + D4 family precedent 정합**.

### 5.4 subscription 측 user-controlled cost vs always-online 측 upstream Battle.net dependency

본 doc 측 핵심 honest 분리:

| 측 | 성격 | 정식 채택 rationale |
|---|---|---|
| **subscription** | user-controlled cost (사용자 자발 결제) | gamebox 측 결제 우회 X = 사용자 자발 결제 honest 가정 |
| **always-online** | upstream Battle.net dependency (사용자 통제 외) | gamebox 측 realm emulator X = upstream 영구 의존 |

→ 두 측 모두 retired_intentional 정식 채택 scope 측 정합 (subscription 우회 X + realm emulator X 측 both honest). **bypass 시도 0건** = own1 + own2 정합 100%.

---

## 6. downstream Battle.net launcher relationship + cross-link to peer D4 (always-online family)

### 6.1 `.roadmap.wow` 와 `.roadmap.battlenet` cross-link

`.roadmap.wow` 와 `.roadmap.battlenet` 의 cross-link 관계 (predecessor `.roadmap.wow` cross_link.battlenet_dependency 측 명시):

| relation | WoW | Battle.net launcher |
|---|---|---|
| upstream dependency | (WoW 측 Battle.net 의존) | (자기 자신 = upstream) |
| `.roadmap.wow` blk.1 type | `online_mmo_subscription` | (자기 자신 측 `online_endpoint_network`) |
| offline shim 가능성 | **불가능 (정의상)** | **가능 (Track A scope)** |
| status_enum_adopted | **`retired_intentional` (본 cycle)** | (Track A 측 별도 결정 — 미채택 가능성) |
| cond.1 transitive | Battle.net cond.1 (Agent SD forging) 측 transitive | (자기 자신) |
| cond.2 transitive | Battle.net cond.2 (Agent SD install) 측 transitive | (자기 자신) |
| cond.3 transitive | Battle.net cond.3 (OAuth 2-leg) + WoW subscription gate 측 transitive | (자기 자신) |

**결론**: WoW 측 retired_intentional 정식 채택은 Battle.net launcher 의 retired_intentional 채택 여부와 **독립적**. Battle.net 측 offline shim 가능 측 cond.1/cond.2 PASS 가능 (Track A scope), WoW 측 always-online + subscription check 는 별도 단계 (step 7) 측 정의상 own1 차단.

→ 두 .roadmap 측 retired_intentional 정식 채택 결정은 **분리 land** (본 cycle = WoW 단독 (+ CS2 / DF / ER 동시), Track A = Battle.net + D2R 별도).

### 6.2 cross-link to peer D4 (always-online family first-mover)

`.roadmap.diablo4` (peer always-online game in 본 repo) 와의 비교:

| 비교 축 | D4 (peer always-online) | WoW (본 cycle) |
|---|---|---|
| 출시 | 2023-06-06 | 2004-11-23 (retail 누적 expansion) + 2019-08-26 (classic) |
| 그래픽스 API | DX12 + DXR (D3DMetal stage2 활용) | DX11 default + DX12 opt-in (D3DMetal stage1 + stage2 양 path) |
| Battle.net 의존 | 필수 | 필수 |
| **subscription** | **부재 (1회 결제 buy-to-play)** | **필수 (월 단위 결제 또는 game time token)** |
| always-online | 필수 (single-player campaign 포함) | 필수 (MMO 정의상 모든 mode) |
| `start_in_offline_mode.exe` 같은 정식 toggle | **부재** | **부재 (single-player mode 자체 부재)** |
| `.roadmap.<game>` blk.1 status_enum_adopted | **`retired_intentional` (Track A2 first-mover, 2026-05-03)** | **`retired_intentional` (Track C 본 cycle 6th lock-in, 2026-05-03)** |
| cond.1 (PE+IAT+CRT) 측 own1 closure path B | **불가능 (always-online check transitive block)** | **불가능 (always-online + subscription check transitive block)** |
| cond.2 (DX renderer first-frame) | **불가능 (DX12+DXR path 측 always-online 의존)** | **불가능 (DX11+DX12 path 측 always-online + subscription 의존)** |
| cond.3 (perf cluster validated) | **불가능 (live game state 측 always-online 의존)** | **불가능 (live MMO state 측 always-online + subscription 의존)** |
| 만점 가능성 | **불가능 (always-online 정의상)** | **불가능 (always-online + subscription 정의상)** |
| 종속 강도 | always-online (1축) | always-online + subscription (**2축, D4 보다 더 강한 종속**) |

**핵심 차이**: D4 = always-online 1축 종속 (single-player campaign 포함), WoW = always-online + subscription 2축 종속 (subscription gate 추가). 두 게임 모두 retired_intentional 정식 채택, **WoW = D4 always-online family 측 더 강한 종속 변종 (peer adoption precedent 정합)**.

→ **D4 와 WoW 의 path 정합은 본 repo airgenome-gamebox 의 always-online family design decision**. WoW 측 retired_intentional 정식 채택 = D4 first-mover precedent 정합 (peer relationship).

---

## 7. 결론: 우선순위

### 7.1 만점 가능성 종합

| 항목 | 값 |
|---|---|
| **만점 가능성 (always-online + subscription OK 가정 — 정의상 own1 위반)** | (의미 X — own1 정의상 거부) |
| **만점 가능성 (own1 + retired_intentional 채택)** | **0% (정의상 measurable 0)**, honest documentation = PASS-equivalent posture |
| 도달 가능 만점 점수 | (live MMO state 측정 X) — 2 c_wows_* perf module 측 spec only (skeleton-stage) |
| validated 만점 가능성 | **0** (always-online + subscription 측 transitive block) |
| skeleton 만점 가능성 | **2** (이미 land, 본 cycle 측 self_test 추가 X — 별도 future cycle scope) |
| BAN risk | **0** (always-online + subscription 측 진입 X, retired_intentional posture 측 측정 X) |
| subscription 결제 risk | **0** (subscription circumvention X, 사용자 자발 결제 honest 가정 측 본 doc scope 외) |
| gamebox identity 와의 정합성 | **정합 100%** — Wine 0줄, subscription bypass 시도 X (retired_intentional posture), always-online 거부 directive 자연 정합 |

### 7.2 WoW retired_intentional 정식 채택 후 cycle 권고

1. **현 cycle (본 doc)**: docs/WOW_MANJEOM_REVIEW.md mk1 narrative spec freeze land + `.roadmap.wow` blk.1 status_enum_adopted 정식 add — 완료 (Track C 6th lock-in)
2. **future cycle (별도)**: 2 c_wows_* perf module 측 self_test() synthetic-data validation (live MMO state 측정 X — retired_intentional posture 측 정합)
3. **future cycle (별도)**: `.roadmap.battlenet` retired_intentional 채택 여부 결정 (Track A scope) — WoW 와 독립
4. **future cycle (별도)**: `c_wows_*` → `c_wow_*` rename 결정 cycle (alias overlap honest reflect 측 long-term resolution)
5. **future cycle (별도)**: docs/WOW_REALM_HONEST_FREEZE.md (가칭) + docs/WOW_SUBSCRIPTION_HONEST_FREEZE.md (가칭) spec freeze 신규 land — 별도 future cycle scope (본 cycle 측 mk1 narrative 측 cover)
6. **영구 미진입**: WoW realm server emulator 자체 구현 / WoW subscription bypass / Warden client-side check 우회 / Battle.net account 위협 행위 — 모두 own1 + retired_intentional posture 정의상 거부

### 7.3 cycle hooks (다음 land 권고)

- 본 cycle 직후 즉시: `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` 측 sha256 pin
- 본 cycle 후속 (별도 cycle): `.roadmap.battlenet` + `.roadmap.diablo2_resurrected` cond.N 측정 진행 (Track A scope, D2R offline mode 측 cond.1/2/3 측정)
- 본 cycle 후속 (별도 cycle): 본 cycle marker 측 Track A 측 marker hash placeholder reconciliation cycle (Track A 완료 후)

### 7.4 정직성 메모

- 본 검토는 predecessor `.roadmap.wow` JSONL header + 2 c_wows_* perf module disk 존재 + Blizzard 공식 자료 + WebSearch community 보고 종합. 실 Wow.exe / WowClassic.exe 분해 X.
- "always-online + subscription check 측 server-side validation bypass 가능성" 은 외부 community 보고 기반 (TrinityCore / AzerothCore 등 private server emulator) — 본 repo 측 우회 코드 emit 0, server emulator 자체 구현 시도 X.
- 2 c_wows_* perf module 측 self_test() validation 은 본 cycle 측 진행 X (별도 future cycle scope, retired_intentional posture 측 synthetic-data only).
- D4 와의 path 정합 결정은 본 repo airgenome-gamebox design decision (always-online family precedent: D4 first-mover + WoW 6th lock-in peer adoption).
- "retired_intentional posture" 의 정의는 D4 first-mover doc (`docs/D4_MANJEOM_REVIEW.md` §0.5-2) 와 동일 정의 — 본 cycle 측 WoW 측 동일 schema 측 정식 채택 (subscription 측 추가 종속 측 honest reflect 측 정합).
- `c_wows_*` alias overlap (World of Warcraft vs World of Warships) 은 본 repo design decision 측 honest reflect — 별도 future cycle 측 rename 가능성 caveat 명시.
- WoW 측 macOS native 공식 client 존재 측 본 doc scope 외 (gamebox 는 Windows PE binary 측 own1 hexa loader 경로 집중 — Apple-native 측 own1 의미 X).

---

## 8. 출처 (WebSearch + 본 repo 자료 종합 2026-05-03)

- World of Warcraft — Blizzard 공식: https://worldofwarcraft.blizzard.com/
- World of Warcraft — Wikipedia: https://en.wikipedia.org/wiki/World_of_Warcraft
- WoW Classic — Blizzard 공식: https://worldofwarcraft.blizzard.com/en-us/wowclassic
- Battle.net — Blizzard 공식: https://www.blizzard.com/en-us/apps/battle.net/desktop
- WoW system requirements (Blizzard 공식): https://us.battle.net/support/en/article/198029
- WoW subscription (Blizzard 공식): https://us.battle.net/support/en/article/9457
- WoW always-online MMO (Wikipedia): https://en.wikipedia.org/wiki/World_of_Warcraft#Gameplay
- Diablo II Resurrected offline mode (Blizzard 공식): https://us.battle.net/support/en/article/198528
- Warden anti-cheat (Wikipedia): https://en.wikipedia.org/wiki/Warden_(software)
- WoW AppleGamingWiki: https://www.applegamingwiki.com/wiki/World_of_Warcraft
- 본 repo `.roadmap.wow` (predecessor 2026-05-03 land)
- 본 repo `.roadmap.battlenet` (peer launcher)
- 본 repo `.roadmap.diablo2_resurrected` (peer game, offline mode)
- 본 repo `.roadmap.diablo4` (peer always-online family first-mover)
- 본 repo `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (predecessor marker)
- 본 repo `state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker` (D4 first-mover marker)
- 본 repo `docs/D4_MANJEOM_REVIEW.md` (peer always-online family first-mover)
- 본 repo `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` (6 phase 모델)
- 본 repo `docs/D2R_D4_COMPARISON.md` (D2R vs D4 비교 축)
- 본 repo `docs/BATTLENET_REAL_LAUNCH_PLAN.md` (Battle.net launcher 측 상위)
- 본 repo `docs/CS2_MANJEOM_REVIEW.md` / `docs/DELTA_FORCE_MANJEOM_REVIEW.md` / `docs/ELDEN_RING_MANJEOM_REVIEW.md` (sibling Track C manjeom reviews)
- 본 repo `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` (sibling Track B manjeom review)

---

*written 2026-05-03 — WoW retired_intentional 정식 채택 mk1 narrative spec freeze (Track C 4-game 6th lock-in). 모듈 코드 X, 2 c_wows_* perf module 측 self_test() 측 별도 future cycle. own1 (Wine 0 / hexa-only) + own2 (log/error/emit/headless) 정합 100%, subscription circumvention 0건, always-online 진입 0건, realm server emulator 자체 구현 0건. retired_intentional posture = block-acknowledged PASS-equivalent honest documentation. D4 always-online family precedent 정합 (peer adoption).*
