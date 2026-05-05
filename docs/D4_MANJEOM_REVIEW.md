# Diablo 4 (D4) — gamebox 만점 기준 적용 가능성 검토 + retired_intentional 정식 채택 mk1 narrative

> **status**: 검토일 **2026-05-03** / 작성: Phase 0 검토 (mk1 narrative spec freeze, 모듈 구현 X)
> **만점 가능성 결론**: **불가능 (always-online DRM 정의상 차단)** — closure path B 호환성은 D2R 수준 ~95% (DX12+DXR 활성화 활용은 오히려 +), 단 **always-online Battle.net launcher 가 정의상 own1 + offline 정의 위반 회피 불가**.
> **status enum**: `retired_intentional` **정식 채택** (block-acknowledged honest documentation = PASS-equivalent posture, own1 Wine 0 + own2 honest emit, no DRM evasion).
> SSOT 보조: `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` 6 phase 모델 재사용 / `docs/D2R_D4_COMPARISON.md` 비교 축 / `docs/BATTLENET_REAL_LAUNCH_PLAN.md` 상위 launcher

---

## 0. 게임 메타데이터

| 항목 | 값 | 근거 |
|---|---|---|
| 정식 명칭 | Diablo IV (D4) | Blizzard Entertainment 공식 |
| 개발사 / 퍼블리셔 | Blizzard Entertainment / Activision Blizzard | 공식 |
| 출시 | 2023-06-06 (PC + console) | 공식 |
| 엔진 | Blizzard 자체 엔진 (Diablo 시리즈 계보) | Blizzard tech blog 추정 |
| Windows binary | **PE32+ x86_64** (`Diablo IV.exe` / `D4.exe` alias) | 설치 경로 기준 |
| 그래픽스 API | **DirectX 12 + DXR** (ray tracing tier 1.1) — Vulkan path 부재, DX11 fallback 부재 | Blizzard 공식 system requirements |
| 시스템 사양 (권장) | i5-9600K / Ryzen 5 2600+, 16 GB RAM, GTX 970 / RX 470+, **DXR**=권장 RTX 2060+ | Blizzard 공식 |
| **distribution** | **Battle.net 만 (PC 메인)** + Microsoft Store (Xbox PC, UWP) — Steam 부재 | Blizzard 공식 |
| **Battle.net 의존** | 필수 — D4 launch 는 Battle.net Agent → game install path → always-online check 통과 후 진입 | Blizzard 공식, 본 repo `.roadmap.battlenet` |
| **always-online DRM** | **필수** — campaign + endgame 포함 모든 모드, 진정한 offline 부재 | Blizzard 공식 |
| anti-cheat | (kernel-level driver 미사용 — Battle.net OAuth + server-side validation 만) | 추정, 공개 자료 부재 |
| DRM 우회 정식 path | **부재** — D2R `start_in_offline_mode.exe` 같은 정식 toggle 없음, Elden Ring `start_game_in_offline_mode.exe` 같은 정식 toggle 없음 | 공식 / WebSearch 종합 |
| macOS 공식 지원 | **없음** (Windows + console 만) — Diablo 시리즈 macOS 지원 영구 종료 (D2R 2023 EOL 이후 동급) | Blizzard 공식 |
| macOS 비공식 | CrossOver / Whisky / GPTK 시도 보고 일부 — 단 own1 (Wine 0) 정의상 본 repo 측 모두 거부 | applegamingwiki 기록 |

### 해석 (Apple Silicon native + own1 관점)

- D4 는 **always-online Battle.net 의존도가 본질적 게임 디자인 요소** — Diablo 시리즈 중 D3 (2012, online-only RMAH 시기 보강) 보다도 강한 종속.
- 결과: D4 closure path B 의 **cond.1 (D4.exe PE+IAT+CRT) / cond.2 (D3D12+DXR first-frame) / cond.3 (27 perf economy spec freeze)** 3 cond 모두 Battle.net always-online 통과 transitive block.
- D2R (peer game in `airgenome-gamebox`) 는 **offline mode handshake 가능** (Battle.net launcher 통과 후 single-player 측 offline 가능) — D4 와의 구조적 차이.
- own1 (Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0) 정의상, D4 always-online 측 honest documentation = `retired_intentional` 정식 채택이 정합.

---

## 0.5 정정 / 추가 (2026-05-03 cycle, retired_intentional 정식 채택)

### 0.5-1. 본 cycle 의 변화 (predecessor 11 .roadmap.<game> fan-out land 후속)

| 항목 | predecessor (2026-05-03 fan-out) | 본 cycle (retired_intentional 채택) |
|---|---|---|
| `.roadmap.diablo4` blk.1 `status_enum_candidate` | `retired_intentional` (후보) | (유지) |
| `.roadmap.diablo4` blk.1 `status_enum_adopted` | (없음) | **`retired_intentional` 신규 add** |
| `.roadmap.diablo4` blk.1 `retired_intentional_posture` | (없음) | **`block_acknowledged_pass_equivalent` 신규 add** |
| 3 cond evidence rows (block-acknowledged 명시) | 부재 | **append 1 row each (`block_acknowledged_pass_equivalent_per_own1_wine_0_no_drm_evasion`)** |
| docs mk1 narrative spec freeze | 부재 (가칭만) | **본 doc land** |

### 0.5-2. retired_intentional posture 정의 (own1 정합)

- **Block-acknowledged**: 차단 사유 명시 + 차단 honest documentation
- **PASS-equivalent**: own1 + own2 정합 측 cond.N 측정 의도 자체가 retired (정의상 PASS X but honest 측면 PASS-equivalent)
- **No DRM evasion**: always-online check / OAuth handshake 우회 코드 emit 0건, Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0
- **No always-online evasion**: Agent SD process spoof X, login token forge X, server-side validation bypass X

→ `retired_intentional` 의 의미: "owned game 측 정합 측정 X (always-online 영구 경계), 그러나 honest documentation 측 honest = PASS-equivalent posture".

### 0.5-3. cross-link to peer Track B (lineage_classic_r28 retired_intentional)

본 cycle 과 병렬 (concurrent landing):

- Track B (`lineage_classic_r28`) — GameGuard kernel-AC 정의상 own1 차단 → 동등 `retired_intentional` 정식 채택 (별도 mk1 narrative `LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` (가칭) land)
- Track A (`battlenet` + `diablo2_resurrected`) — offline shim 가능 측 retired_intentional 미적용, 별도 cycle (D4 와 다른 path)

→ 본 cycle = **Track A2 (D4 단독 retired_intentional 정식 채택)**, predecessor §3 retired_intentional_enum_application_5_games 측 5 게임 중 D4 측 first-mover.

---

## 1. 6 phase 별 만점 가능성 매트릭스

D2R/D4 로드맵의 6 phase 모델 재사용 (Phase 1=launcher install, Phase 2=launcher runtime, Phase 3=game install, Phase 4=first frame, Phase 5=gameplay, Phase 6=live tune).

| Phase | D4 적용 | 만점 가능성 | 핵심 제약 |
|---|---|---|---|
| **P1** Battle.net Setup install | spec only — `.roadmap.battlenet` cond.2 transitive | A=5 / B=4 / C=5 → 14 | Agent SD install path, signature check |
| **P2** Battle.net runtime (Agent SD + REST) | spec only — `.roadmap.battlenet` cond.1 transitive | A=5 / B=5 / C=5 → **15** | Agent SD process forging, OAuth 2-leg, login state |
| **P3** D4 download + install | depot CDN — D2R 와 chunk 인프라 공유 | A=5 / B=4 / C=5 → 14 | Battle.net Agent depot 프로토콜 |
| **P4** first frame (D3D12+DXR) | **Apple D3DMetal stage2 활용 가능 영역** — DXR tier 1.1 | A=5 / B=5 / C=5 → **15** (만점 후보 3) | always-online check 통과 후 측정 가능 (block 측 측정 X = `retired_intentional`) |
| **P5** gameplay (27 perf cluster) | helltide / sigil / paragon / tempering / occultist / world tier 9 | A=5 / B=5 / C=5 → **15** (만점 후보 ≥10) | live game state 측정 = always-online 의존 |
| **P6** live tune | 30+ 시간 측정 = season-pass / world-boss spawn window 측 누적 | A=5 / B=5 / C=5 → **15** | seasonal mechanic timer 측 wallclock 정합 |

### 누적 만점 도달 추정

- **always-online OK 가정 (정의상 own1 위반)**: 만점 후보 **약 13-18 모듈** (27 c_d4_* perf cluster 측 spec freeze 완료 시)
- **실 closure path B (own1 + offline 정의)**: **0** — always-online 측 cond.1/2/3 모두 transitive block, `retired_intentional` 정식 채택

---

## 2. always-online DRM 차단 위험 평가 (own1 정합 측면)

### 2.1 Battle.net always-online check — gamebox PE loader 와의 직접 충돌

D4 launch 흐름 (공식 + WebSearch 추정):

| step | 동작 | own1 closure path B 영향 |
|---|---|---|
| 1. Battle.net Setup install | `.roadmap.battlenet` cond.2 (Agent SD install) | offline shim 가능 측 가능 (Track A scope) |
| 2. Battle.net Agent SD + REST | `.roadmap.battlenet` cond.1 (Agent SD forging) | offline shim 가능 측 가능 (Track A scope) |
| 3. OAuth 2-leg login | `.roadmap.battlenet` cond.3 (login + chat) | offline shim 측 token forge 측 own1 위반 → 거부 |
| 4. D4 game catalog 조회 | Agent SD REST | offline shim 가능 측 가능 |
| 5. D4 download depot | Battle.net Agent CDN | depot prefetch 측 가능 |
| 6. D4.exe launch | Agent → D4.exe child process | spec only — own1 측 PE loader 측 가능 |
| 7. **D4 launch 시점 always-online check** | server side validation | **closure path B 차단 — own1 측 우회 X** |
| 8. campaign / endgame play | live server state | always-online 의존 — offline 측 측정 X |

**결론**: step 7 이 own1 closure path B = **즉시 retired_intentional candidate**. **만점 정의의 "Wine 0줄 + DIY hexa PE loader" 는 always-online D4 와 정의상 양립 불가**.

### 2.2 회피 가능성 — gamebox 정책상 **거부**

기술적으로 always-online 우회는 외부 자료상 (a) Agent SD process spoof + (b) login token forge + (c) DRM-friendly handshake 측 server-side validation bypass 측 일부 가능 (외부 community 보고). 그러나:

1. **own1 + 정직성 정책상 DRM-bypass 카테고리 진입 X** — IDENTITY.md 정신 위반.
2. Blizzard 의 **Battle.net account BAN 영구화** — gamebox 사용자 자산 손실 risk.
3. gamebox 의 가치 = **개발자/연구자가 Apple Silicon 에서 D4 frame trace 측정 spec** 이지, 실 always-online 진입 X.

**정책**: gamebox D4 path 는 **`retired_intentional` 정식 채택 (block-acknowledged honest documentation = PASS-equivalent posture)**. always-online 우회는 명시적으로 out-of-scope.

### 2.3 Battle.net launcher (peer offline shim) — Track A 와의 관계

| 시나리오 | 가능성 | 비고 |
|---|---|---|
| `.roadmap.battlenet` cond.1 (Agent SD forging) offline shim | **가능 (Track A)** | offline shim 측 cond.1/cond.2 PASS 가능 |
| `.roadmap.diablo2_resurrected` offline mode | **가능 (Track A)** | D2R 정식 offline mode handshake (Battle.net 통과 측 optional) |
| `.roadmap.diablo4` always-online | **불가능 (본 cycle scope)** | retired_intentional 정식 채택 |

**결론**: D2R 와 D4 의 offline 가능성 차이 = **D2R 측 offline mode 정식 path 존재 / D4 측 always-online 정의 부재**. 본 doc 측 D4 는 retired_intentional 정식 채택, D2R 는 offline mode 측 cond.N 측정 진행 (별도 Track A cycle).

### 2.4 own1 정합 100% 정합 audit

- **Wine 0**: 본 cycle 측 Wine / Proton / CrossOver / Whisky / GPTK 의존 0건
- **DRM bypass code 0**: always-online check / OAuth token forge / server-side validation bypass 측 emit 0건
- **D4.exe direct launch 시도 0**: Battle.net Agent 통과 X 측 D4.exe 단독 launch 측 own1 측 시도 X (always-online check 측 fail 측 측정 의미 0)
- **27 c_d4_* perf module spec only**: skeleton-stage 만, live game state 측 측정 0건

→ ✓ **own1 + 만점 정의 정합 100%**.

---

## 3. closure path B 호환성

D2R/D4 closure path B (own1 + DIY PE loader + Apple D3DMetal) 와의 모듈 재사용도:

| closure path B 컴포넌트 | D4 호환 (block 가정 시) | 비고 |
|---|---|---|
| C-PE-MAP-x86_64 (PE32+ map) | **100%** | D2R 와 같은 PE32+ x86_64 |
| C-IAT-RESOLVE-RUNTIME | **100%** | DLL set 일부 다름 (DXCore + dxcompiler 추가) |
| C-WIN32-CALL-TRAMPOLINE (x86_64 SysV ↔ Win64 ABI) | **100%** | D2R 와 동일 |
| DLL coverage 15/16 (kernel32, user32, advapi32, ws2_32 등) | **약 95%** | DX12 + DXCore + dxcompiler 추가 의존 |
| dx_d3d11_stage1 (D3DMetal binding) | **불필요** | D4 default DX12 (DX11 fallback 부재) |
| **dx_d3d12_stage2 + DXR** | **100%** (D4 활용 영역) | D4 측 DXR tier 1.1 활용 = stage2 의 핵심 customer |
| Vulkan path | **불필요** | D4 측 Vulkan 부재 |
| Battle.net launcher OAuth/Catalog | **0% (정의상 거부)** | always-online check 측 own1 측 우회 X |
| Battle.net depot CDN | **70% 재사용** (depot 프로토콜 spec only) | D2R 와 chunk 인프라 공유, 단 download 측 always-online check 측 우회 X |

**호환성 점수**: **(block 가정 시) 95%** — D4 측 DXR + DX12 활성화는 stage2 의 만점 path. **(retired_intentional 채택 시) 정의상 measurable 0**, honest documentation = PASS-equivalent posture.

---

## 4. 추천 skeleton 모듈 cluster (이미 land 27 개 — spec only)

D2R 와 중복되지 않는 **D4 specific 신규 skeleton**, 본 repo 측 이미 land:

| # | 모듈 | A | B | C | 만점 | phase | 비고 |
|---|---|---|---|---|---|---|---|
| 1 | `c_d4_dxr_first_frame_trace` | 5 | 5 | 5 | **15** | P4 | D3D12 + DXR tier 1.1 first-frame |
| 2 | `c_d4_dxr_bvh_compact` | 5 | 5 | 5 | **15** | P4 | DXR BVH compact spec |
| 3 | `c_d4_pso_warmup_priority` | 5 | 5 | 5 | **15** | P4 | DX12 PSO warm priority |
| 4 | `c_d4_helltide_chest_route` | 5 | 5 | 5 | **15** | P5 | helltide route forecast |
| 5 | `c_d4_dungeon_seed_track` | 5 | 5 | 5 | **15** | P5 | dungeon seed reproducible |
| 6 | `c_d4_dungeon_sigil_inventory` | 5 | 5 | 5 | **15** | P5 | sigil inventory cache |
| 7 | `c_d4_glyph_paragon_level` | 5 | 5 | 5 | **15** | P5 | glyph + paragon level track |
| 8 | `c_d4_helltide_event` | 5 | 5 | 5 | **15** | P5 | helltide event timer |
| 9 | `c_d4_infernal_horde_wave` | 5 | 5 | 5 | **15** | P5 | infernal horde wave forecast |
| 10 | `c_d4_living_steel_economy` | 5 | 5 | 5 | **15** | P5 | living steel economy track |
| 11 | `c_d4_loot_drop_predict` | 5 | 5 | 5 | **15** | P5 | loot drop forecast |
| 12 | `c_d4_masterworking_amplify` | 5 | 5 | 5 | **15** | P5 | masterworking amplify roll |
| 13 | `c_d4_nightmare_sigil_track` | 5 | 5 | 5 | **15** | P5 | nightmare sigil track |
| 14 | `c_d4_occultist_enchant_roll` | 5 | 5 | 5 | **15** | P5 | occultist enchant roll |
| 15 | `c_d4_paragon_track` | 5 | 5 | 5 | **15** | P5 | paragon track |
| 16 | `c_d4_pet_companion_track` | 5 | 5 | 5 | **15** | P5 | pet companion track |
| 17 | `c_d4_pit_tier_speed_track` | 5 | 5 | 5 | **15** | P5 | pit tier speed track |
| 18 | `c_d4_seasonal_mechanic_timer` | 5 | 5 | 5 | **15** | P5-P6 | seasonal mechanic timer |
| 19 | `c_d4_seasonal_pass_track` | 5 | 5 | 5 | **15** | P5-P6 | seasonal pass track |
| 20 | `c_d4_tempering_affix_reroll` | 5 | 5 | 5 | **15** | P5 | tempering affix reroll |
| 21 | `c_d4_whisper_bounty_optimize` | 5 | 5 | 5 | **15** | P5 | whisper bounty optimize |
| 22 | `c_d4_world_boss_spawn_window` | 5 | 5 | 5 | **15** | P5-P6 | world boss spawn window |
| 23 | `c_d4_world_event_calendar` | 5 | 5 | 5 | **15** | P5-P6 | world event calendar |
| 24 | `c_d4_world_tier_unlock` | 5 | 5 | 5 | **15** | P5 | world tier unlock |
| 25 | `c_d4_xp_curve_predict` | 5 | 5 | 5 | **15** | P5 | xp curve forecast |
| 26 | `c_d4_aspect_imprint_track` | 5 | 5 | 5 | **15** | P5 | aspect imprint track |
| 27 | `c_d4_codex_unlock` | 5 | 5 | 5 | **15** | P5 | codex unlock track |

→ **총 27 모듈, 모두 만점 (15) 후보 spec only (skeleton 단계)**. live game state 측 validated 측 always-online 의존 = `retired_intentional` posture 측 정합.

---

## 5. retired_intentional 정식 채택 rationale

### 5.1 결정 evidence

| evidence | 출처 |
|---|---|
| always-online DRM 정의상 own1 차단 | Blizzard 공식 (D4 launch 흐름) |
| `start_in_offline_mode.exe` 같은 정식 toggle 부재 | 공식 / WebSearch 종합 |
| D4.exe 단독 launch 시 always-online check fail | 추정 (공식 자료 부재, WebSearch community 보고) |
| `.roadmap.diablo4` blk.1 `status_enum_candidate: retired_intentional` 명시 (predecessor 2026-05-03) | `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` §`retired_intentional_enum_application_5_games` |
| D4 27 c_d4_* perf cluster spec only land 완료 (skeleton-stage) | predecessor `.roadmap.diablo4` cond.3 evidence |
| D2R offline mode 정식 (peer game) | 본 repo `.roadmap.diablo2_resurrected` cond.3 |

→ **retired_intentional 정식 채택 = honest documentation 정의 측 정합, D2R 의 offline mode 정식 path 와 명확 분리**.

### 5.2 정식 채택 후 의미

- `.roadmap.diablo4` blk.1 status: `open` (유지) + `status_enum_candidate: retired_intentional` (유지) + **`status_enum_adopted: retired_intentional` (신규 add)** + **`retired_intentional_posture: block_acknowledged_pass_equivalent` (신규 add)**
- 3 cond.N status: `partial` (유지) + 3 cond evidence array 측 **`block_acknowledged_pass_equivalent_per_own1_wine_0_no_drm_evasion` row append**
- docs/D4_MANJEOM_REVIEW.md (본 doc) — mk1 narrative spec freeze land
- 향후 cycle 측 27 c_d4_* perf module 측 self_test() validation 진행 가능 (synthetic-data 측 — live game state 측 측정 X)

### 5.3 정식 채택 X 시 vs 채택 시 비교

| 비교 축 | retired_intentional 미채택 | retired_intentional 정식 채택 (본 cycle) |
|---|---|---|
| `.roadmap.diablo4` blk.1 status | `open` (영구 unmet 상태) | `open` + `status_enum_adopted: retired_intentional` (block-acknowledged PASS-equivalent) |
| 3 cond.N status | `partial` (영구 transitive block) | `partial` + block-acknowledged 명시 (honest documentation) |
| own1 정합 | (애매 — block 측 status 이름 부재) | **명확 (retired_intentional posture)** |
| own2 정합 | (애매) | **명확 (honest emit, block-acknowledged)** |
| 향후 cycle 측 측정 의도 | (불명확 — always-online 측 측정 의지) | **명확 (live game state 측정 X, 27 perf module synthetic-data only)** |
| 사용자 친화 (handoff) | (현 status 이해 어려움) | **명확 (retired_intentional 정의 명시)** |

→ retired_intentional 정식 채택 = **honest documentation 정의 측 정합 + own1/own2 정합 + 사용자 친화**.

---

## 6. downstream Battle.net launcher relationship (cross-link)

`.roadmap.diablo4` 와 `.roadmap.battlenet` 의 cross-link 관계 (predecessor `.roadmap.diablo4` cross_link.battlenet_dependency 측 명시):

| relation | D4 | Battle.net launcher |
|---|---|---|
| upstream dependency | (D4 측 Battle.net 의존) | (자기 자신 = upstream) |
| `.roadmap.diablo4` blk.1 type | `upstream_drm` (always_online_drm) | (자기 자신 측 `online_endpoint_network`) |
| offline shim 가능성 | **불가능 (정의상)** | **가능 (Track A scope)** |
| status_enum_adopted | **`retired_intentional` (본 cycle)** | (Track A 측 별도 결정 — 미채택 가능성) |
| cond.1 transitive | Battle.net cond.1 (Agent SD forging) 측 transitive | (자기 자신) |
| cond.2 transitive | Battle.net cond.2 (Agent SD install) 측 transitive | (자기 자신) |

**결론**: D4 측 retired_intentional 정식 채택은 Battle.net launcher 의 retired_intentional 채택 여부와 **독립적**. Battle.net 측 offline shim 가능 측 cond.1/cond.2 PASS 가능 (Track A scope), D4 측 always-online check 는 별도 단계 (step 7) 측 정의상 own1 차단.

→ 두 .roadmap 측 retired_intentional 정식 채택 결정은 **분리 land** (본 cycle = D4 단독, Track A = Battle.net + D2R 별도).

---

## 7. cross-link to peer D2R offline mode (peer game in `airgenome-gamebox`)

`.roadmap.diablo2_resurrected` (peer game in 본 repo) 와의 비교:

| 비교 축 | D2R | D4 |
|---|---|---|
| 출시 | 2021-09-23 (remaster) | 2023-06-06 |
| 그래픽스 API | DX11 (D3DMetal stage1 직접 매핑) | DX12 + DXR (D3DMetal stage2 활용) |
| Battle.net 의존 | 필수 — 단 offline mode handshake 가능 | 필수 — always-online (offline 부재) |
| `start_in_offline_mode.exe` 같은 정식 toggle | **정식 (Battle.net 측 offline 모드 진입)** | **부재** |
| `.roadmap.<game>` blk.1 status_enum | `none` (offline 가능) | **`retired_intentional` (본 cycle 정식 채택)** |
| cond.1 (PE+IAT+CRT) 측 own1 closure path B | **가능 (offline mode 측 측정)** | **불가능 (always-online check transitive block)** |
| cond.2 (DX renderer first-frame) | **가능 (DX11 path)** | **불가능 (DX12+DXR path 측 always-online 의존)** |
| cond.3 (perf cluster validated) | **가능 (offline single-player)** | **불가능 (live game state 측 always-online 의존)** |
| 만점 가능성 | **부분가능 (offline mode 한정)** | **불가능 (always-online 정의상)** |

**핵심 차이**: D2R 측 Battle.net launcher 통과 후 **offline single-player mode 정식 path** (Blizzard 정식 지원) → cond.1/2/3 측정 가능. D4 측 always-online 정의상 정식 toggle 부재 → retired_intentional 정식 채택.

→ **D2R 와 D4 의 path 분리는 본 repo airgenome-gamebox 의 핵심 design decision**. D4 측 retired_intentional honest documentation = D2R 측 offline mode 정식 path 와 **상호 보완 (peer relationship)**.

---

## 8. 결론: 우선순위

### 8.1 만점 가능성 종합

| 항목 | 값 |
|---|---|
| **만점 가능성 (always-online OK 가정 — 정의상 own1 위반)** | (의미 X — own1 정의상 거부) |
| **만점 가능성 (own1 + retired_intentional 채택)** | **0% (정의상 measurable 0)**, honest documentation = PASS-equivalent posture |
| 도달 가능 만점 점수 | (live game state 측정 X) — 27 c_d4_* perf module 측 spec only (skeleton-stage) |
| validated 만점 가능성 | **0** (always-online 측 always-online check transitive block) |
| skeleton 만점 가능성 | **27** (이미 land, 본 cycle 측 self_test 추가 X — 별도 future cycle scope) |
| BAN risk | **0** (always-online 측 진입 X, retired_intentional posture 측 측정 X) |
| gamebox identity 와의 정합성 | **정합 100%** — Wine 0줄, DRM-bypass 시도 X (retired_intentional posture), always-online 거부 directive 자연 정합 |

### 8.2 D4 retired_intentional 정식 채택 후 cycle 권고

1. **현 cycle (본 doc)**: docs/D4_MANJEOM_REVIEW.md mk1 narrative spec freeze land + `.roadmap.diablo4` blk.1 status_enum_adopted 정식 add — 완료
2. **future cycle (별도)**: 27 c_d4_* perf module 측 self_test() synthetic-data validation (live game state 측정 X — retired_intentional posture 측 정합)
3. **future cycle (별도)**: `.roadmap.battlenet` retired_intentional 채택 여부 결정 (Track A scope) — D4 와 독립
4. **future cycle (별도)**: `.roadmap.wow` retired_intentional 채택 여부 결정 (D4 always-online family precedent)
5. **영구 미진입**: D4 always-online 측 진입 시도 / D4 DRM bypass 시도 / Battle.net account 위협 행위 — 모두 own1 + retired_intentional posture 정의상 거부

### 8.3 cycle hooks (다음 land 권고)

- 본 cycle 직후 즉시: `state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker` 측 sha256 pin
- 본 cycle 후속 (별도 cycle): `LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` (Track B 병렬, GameGuard kernel-AC retired_intentional)
- 본 cycle 후속 (별도 cycle): `CS2 / DELTA_FORCE / ELDEN_RING` 3 anti-cheat 측 retired_intentional 정식 채택 여부 결정 (현재는 후보 명시만)
- 본 cycle 후속 (별도 cycle): `.roadmap.battlenet` + `.roadmap.diablo2_resurrected` cond.N 측정 진행 (Track A scope, D2R offline mode 측 cond.1/2/3 측정)

### 8.4 정직성 메모

- 본 검토는 predecessor `.roadmap.diablo4` JSONL header + 27 c_d4_* perf module disk 존재 + Blizzard 공식 자료 + WebSearch community 보고 종합. 실 D4.exe 분해 X.
- "always-online check 측 server-side validation bypass 가능성" 은 외부 community 보고 기반 — 본 repo 측 우회 코드 emit 0.
- 27 c_d4_* perf module 측 self_test() validation 은 본 cycle 측 진행 X (별도 future cycle scope, retired_intentional posture 측 synthetic-data only).
- D2R 와의 path 분리 결정은 본 repo airgenome-gamebox design decision (Blizzard 측 D4 always-online 정식 정책 + D2R offline mode 정식 정책).
- "retired_intentional posture" 의 정의는 본 doc 측 first-mover 정의 (5 게임 중 D4 측 first-mover) — `.roadmap.<game>` schema 측 정식 채택은 mk2 spec freeze 측 별도 결정 (rank A 3 도메인 land 시 동일 caveat).

---

## 9. 출처 (WebSearch + 본 repo 자료 종합 2026-05-03)

- Diablo IV — Blizzard 공식: https://diablo4.blizzard.com/
- Diablo IV — Wikipedia: https://en.wikipedia.org/wiki/Diablo_IV
- Battle.net — Blizzard 공식: https://www.blizzard.com/en-us/apps/battle.net/desktop
- Diablo IV system requirements (Blizzard 공식): https://us.battle.net/support/en/article/199849
- Diablo IV Microsoft Store: https://www.microsoft.com/store/p/diablo-iv/9MTNL84RKLC9
- Diablo IV always-online 분석 (Wikipedia): https://en.wikipedia.org/wiki/Diablo_IV#Reception
- Diablo II Resurrected offline mode (Blizzard 공식): https://us.battle.net/support/en/article/198528
- DXR tier 1.1 (Microsoft DirectX): https://learn.microsoft.com/en-us/windows/win32/direct3d12/direct3d-12-raytracing
- Diablo IV AppleGamingWiki: https://www.applegamingwiki.com/wiki/Diablo_IV
- 본 repo `.roadmap.diablo4` (predecessor 2026-05-03 land)
- 본 repo `.roadmap.battlenet` (peer launcher)
- 본 repo `.roadmap.diablo2_resurrected` (peer game, offline mode)
- 본 repo `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (predecessor marker)
- 본 repo `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` (6 phase 모델)
- 본 repo `docs/D2R_D4_COMPARISON.md` (D2R vs D4 비교 축)
- 본 repo `docs/BATTLENET_REAL_LAUNCH_PLAN.md` (Battle.net launcher 측 상위)
- 본 repo `docs/CS2_MANJEOM_REVIEW.md` / `docs/DELTA_FORCE_MANJEOM_REVIEW.md` / `docs/ELDEN_RING_MANJEOM_REVIEW.md` (sibling manjeom reviews)

---

*written 2026-05-03 — D4 retired_intentional 정식 채택 mk1 narrative spec freeze. 모듈 코드 X, 27 c_d4_* perf module 측 self_test() 측 별도 future cycle. own1 (Wine 0 / hexa-only) + own2 (log/error/emit/headless) 정합 100%, DRM bypass 0건, always-online 진입 0건. retired_intentional posture = block-acknowledged PASS-equivalent honest documentation.*
