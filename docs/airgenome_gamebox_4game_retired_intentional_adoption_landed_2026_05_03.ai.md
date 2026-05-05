---
schema: airgenome-gamebox/docs/4game_retired_intentional_adoption_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker
  roadmap_files_modified:
    - .roadmap.cs2
    - .roadmap.delta_force
    - .roadmap.elden_ring
    - .roadmap.wow
  docs_created:
    - docs/WOW_MANJEOM_REVIEW.md
  docs_reused:
    - docs/CS2_MANJEOM_REVIEW.md
    - docs/DELTA_FORCE_MANJEOM_REVIEW.md
    - docs/ELDEN_RING_MANJEOM_REVIEW.md
  predecessor_handoffs:
    - docs/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
    - docs/airgenome_gamebox_rank_a_3_domain_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_per_game_roadmaps_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_d4_retired_intentional_adoption_landed_2026_05_03.ai.md
status: LANDED_4GAME_RETIRED_INTENTIONAL_ADOPTION
related_raws:
  - raw 9    # hexa-only orchestration (audit-only, no impl emitted)
  - raw 10   # honest C3 caveats inline
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 15   # env() lazy + <user> placeholder
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes in this doc)
preserved_unchanged:
  - 7 of 11 .roadmap.<game> files (only cs2 / delta_force / elden_ring / wow modified, additive field-level edit)
  - 3 existing .roadmap.<domain> files (loader_pe, loader_win32, loader_dx)
  - all hexa modules under lib/{loader,perf} (loader 414 + perf 241 = 655 .hexa)
  - all native sources, tests, tool/, plugin.json, entry, README.md, .gitignore
  - 5 predecessor markers (self_mk2_tuning + rank_a_3_domain + per_game_roadmaps + d4_retired_intentional + purple_lineage_offline_shim + battlenet_d2r_offline_shim)
  - 3 reused MANJEOM_REVIEW.md docs (CS2 / DELTA_FORCE / ELDEN_RING — untouched)
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 4   # 4 .roadmap files field-level append + status_enum_adopted add (additive at field-level)
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
own:
  own1: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 — hexa-only, no AC bypass, no DRM bypass, no subscription circumvention
  own2: log/error/emit/headless honest documentation
---

# airgenome-gamebox Track C 4-game retired_intentional 정식 채택 land - mk1 narrative reuse + WoW NEW (2026-05-03)

## TL;DR

predecessor 11 .roadmap.<game> per-game fan-out (2026-05-03) + D4 retired_intentional first-mover (2026-05-03 Track A2)
+ lineage_classic_r28 Track B (2026-05-03) 후속 측 4 게임 (cs2 / delta_force / elden_ring / wow) `retired_intentional`
status enum 정식 채택 + WoW mk1 narrative spec freeze land (Track C 본 cycle).

- 4 `.roadmap.<game>` 측 blk.1 `status_enum_adopted: retired_intentional` 신규 add + `retired_intentional_posture:
  block_acknowledged_pass_equivalent` 신규 add + `adopted_iso: 2026-05-03` + `adopted_via:
  track_c_4game_retired_intentional_adoption_landed` + `blocker_class: <type>` (additive at field-level).
- 12 cond evidence row append (3 cond × 4 게임) — `block_acknowledged_pass_equivalent_per_own1_wine_0_no_<bypass_kind>`.
- 신규 `docs/WOW_MANJEOM_REVIEW.md` mk1 narrative spec freeze land — sibling docs (CS2 / DF / ER / D4) style/length 정합 (380 LOC).
- 기존 3 MANJEOM_REVIEW (CS2 / DELTA_FORCE / ELDEN_RING) REUSE — 본 cycle 측 untouched.
- 본 cycle 후 retired_intentional 정식 채택 = **6 게임** (D4 + lineage_classic_r28 + cs2 + delta_force + elden_ring + wow).
  predecessor 5-candidate set (D4 / cs2 / delta_force / elden_ring / lineage_classic_r28) **5/5 정식 채택 완료** + WoW
  6th lock-in.
- AC bypass 코드 0건, DRM bypass 0건, subscription circumvention 0건, always-online 진입 시도 0건.
- 새 doc 2 + 새 marker 1 + 4 .roadmap field-level edit = **3 file 추가 + 4 file additive edit**, 기존 file 0건
  destructive 수정.

## §1 user_directive_paraphrase

사용자 directive (paraphrase only, no verbatim) — predecessor `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker`
측 `retired_intentional_enum_application_5_games` array (D4 / cs2 / delta_force / elden_ring / lineage_classic_r28)
중 D4 (Track A2) + lineage_classic_r28 (Track B) 측 first-mover 정식 채택 후 남은 4 게임 (cs2 / delta_force /
elden_ring + WoW 6th lock-in 추가) 측 `retired_intentional` enum 정식 채택 + WoW NEW mk1 narrative spec freeze 작업.
Track C scope = 4 게임 동시 land. 4 게임 중 3 게임 (cs2 / delta_force / elden_ring) 측 기존 MANJEOM_REVIEW 측
REUSE (predecessor 측 land 완료, 본 cycle 측 untouched), WoW 측 NEW mk1 narrative spec freeze 신규 land. AC bypass
0건, DRM bypass 0건, subscription circumvention 0건, always-online 진입 0건. 본 cycle 측 file scope = 4 .roadmap
+ docs/WOW_MANJEOM_REVIEW.md (신규) + 본 doc + marker 만 (다른 .roadmap / lib / tests / native / tool 측 미터치).

## §2 4-game retired_intentional adoption table (before / after)

### §2.1 `.roadmap.cs2` blk.1 — adoption table

| field | before (predecessor 2026-05-03) | after (본 cycle) |
|---|---|---|
| `id` | `cs2.blk.1` | (유지) |
| `type` | `upstream_anticheat` | (유지) |
| `blocker_class` | (없음 — header 측 `vac_anticheat` 만) | **`vac_anticheat` (blk.1 측 add)** |
| `status` | `open` | (유지) |
| `status_enum_candidate` | `retired_intentional` | (유지) |
| `status_enum_adopted` | (없음) | **`retired_intentional` (신규 add)** |
| `retired_intentional_posture` | (없음) | **`block_acknowledged_pass_equivalent` (신규 add)** |
| `adoption_evidence` | (없음) | **["docs/CS2_MANJEOM_REVIEW.md", "state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker"]** |
| `adopted_iso` | (없음) | **`2026-05-03` (신규 add)** |
| `adopted_via` | (없음) | **`track_c_4game_retired_intentional_adoption_landed` (신규 add)** |

### §2.2 `.roadmap.delta_force` blk.1 — adoption table

| field | before | after |
|---|---|---|
| `id` | `delta_force.blk.1` | (유지) |
| `type` | `upstream_anticheat_kernel` | (유지) |
| `blocker_class` | (없음) | **`eac_kernel_anticheat` (blk.1 측 add)** |
| `status_enum_adopted` | (없음) | **`retired_intentional` (신규 add)** |
| `retired_intentional_posture` | (없음) | **`block_acknowledged_pass_equivalent` (신규 add)** |
| `adoption_evidence` | (없음) | **["docs/DELTA_FORCE_MANJEOM_REVIEW.md", "state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker"]** |
| `adopted_iso` | (없음) | **`2026-05-03` (신규 add)** |
| `adopted_via` | (없음) | **`track_c_4game_retired_intentional_adoption_landed` (신규 add)** |

### §2.3 `.roadmap.elden_ring` blk.1 — adoption table

| field | before | after |
|---|---|---|
| `id` | `elden_ring.blk.1` | (유지) |
| `type` | `upstream_anticheat_anti_tamper` | (유지) |
| `blocker_class` | (없음) | **`eac_anti_tamper` (blk.1 측 add)** |
| `status_enum_adopted` | (없음) | **`retired_intentional` (신규 add)** |
| `retired_intentional_posture` | (없음) | **`block_acknowledged_pass_equivalent` (신규 add)** |
| `adoption_evidence` | (없음) | **["docs/ELDEN_RING_MANJEOM_REVIEW.md", "state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker"]** |
| `adopted_iso` | (없음) | **`2026-05-03` (신규 add)** |
| `adopted_via` | (없음) | **`track_c_4game_retired_intentional_adoption_landed` (신규 add)** |

### §2.4 `.roadmap.wow` blk.1 — adoption table (6th lock-in)

| field | before | after |
|---|---|---|
| `id` | `wow.blk.1` | (유지) |
| `type` | `upstream_subscription` | **`online_mmo_subscription` (정합 update — D4 always-online family 측 정합 reflect)** |
| `blocker_class` | (없음) | **`online_mmo_subscription` (blk.1 측 add)** |
| `status_enum_adopted` | (없음) | **`retired_intentional` (신규 add)** |
| `retired_intentional_posture` | (없음) | **`block_acknowledged_pass_equivalent` (신규 add)** |
| `adoption_evidence` | (없음) | **["docs/WOW_MANJEOM_REVIEW.md", "state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker"]** |
| `adopted_iso` | (없음) | **`2026-05-03` (신규 add)** |
| `adopted_via` | (없음) | **`track_c_4game_retired_intentional_adoption_landed` (신규 add)** |

### §2.5 file size delta (4 modified roadmaps)

| file | bytes_before | bytes_after | delta_bytes | sha256_before | sha256_after |
|---|---:|---:|---:|---|---|
| `.roadmap.cs2` | 4701 | 5783 | +1082 | `63dd4ff32f707389b6ac4330db691bfb696e37fa3c815ea632025c71051ac603` | `69598f9a2cd500f311dc94be9e2b3a352d53e4e6317050b2633d8418a39c681e` |
| `.roadmap.delta_force` | 4978 | 6140 | +1162 | `edc840490a37a0ca392edbc94323e4dea8d0156b713ff01e346e1f26e0c662c1` | `61a482b62cdfe7686b0810390c3262e04714c0f727449f2a5eb0a258506785f8` |
| `.roadmap.elden_ring` | 5490 | 6615 | +1125 | `d1aa8c1d2e3fb33de7b21ac008e1af05e18afaee1ee3c93c6dd008d023e323b7` | `049e09948a51ad5ef2450c8f80141aaa9e4d48ad9d0ebfd68838025121be4e0a` |
| `.roadmap.wow` | 4735 | 6475 | +1740 | `5f3d05e2d28f585091ef436c3e68fb8e85214cecdeed081dab0884e7704a2e48` | `57ba0bd76d8d64ee445d84b9f3fe76f54d9661397a8442fe97acf421d15a4181` |

## §3 manjeom_review cross-ref table (3 reused + 1 new)

| game | doc | status (본 cycle) | author/source | LOC | bytes |
|---|---|---|---|---:|---:|
| cs2 | `docs/CS2_MANJEOM_REVIEW.md` | **REUSE (untouched)** | predecessor 2026-05-01-rev2 | 314 | (predecessor land) |
| delta_force | `docs/DELTA_FORCE_MANJEOM_REVIEW.md` | **REUSE (untouched)** | predecessor | ~366 | (predecessor land) |
| elden_ring | `docs/ELDEN_RING_MANJEOM_REVIEW.md` | **REUSE (untouched)** | predecessor | ~403 | (predecessor land) |
| **wow** | **`docs/WOW_MANJEOM_REVIEW.md`** | **NEW (본 cycle)** | 본 cycle | **380** | **32453** |

WOW_MANJEOM_REVIEW.md sections (mirrored CS2 review structure):
- §0 게임 메타데이터 (alias retail + classic, Battle.net, DX11 default + DX12 opt-in, always-online + subscription)
- §0.5 정정 / 추가 (본 cycle Track C 4-game retired_intentional 채택 정정)
- §1 6 phase 만점 가능성 매트릭스 (D2R/D4 모델 재사용)
- §2 online MMO subscription + always-online 차단 위험 평가 (own1 정합 측면)
- §3 closure path B 호환성 (block 가정 시 80%, retired_intentional 채택 시 0)
- §4 2 c_wows_* perf module + alias overlap caveat (이미 land, spec only) + 향후 candidate
- §5 retired_intentional 정식 채택 rationale (subscription user-controlled cost vs always-online upstream Battle.net dep)
- §6 downstream Battle.net launcher relationship + cross-link to peer D4 (always-online family)
- §7 결론 — 우선순위
- §8 출처 (WebSearch + 본 repo 자료 종합)

만점 axis A/B/C 측 reflect:
- A (Apple-only stack): 5 — Apple-only hexa PE loader, Wine 0, D3DMetal stage1 (DX11) + stage2 (DX12) 양 path 활용
- B (Runtime — gameplay 중 효과): 5 / 0 (block 가정 시 5, retired_intentional 정식 채택 시 measurable 0)
- C (데이터재해석 — raw → cache/filter/forecast): 5 (2 c_wows_* perf module 측 spec only — alias overlap honest reflect)

## §4 retired_intentional set status

### §4.1 5-candidate original set (predecessor `retired_intentional_enum_application_5_games`)

| 게임 | blocker | type | 정식 채택 cycle | 정식 채택 doc |
|---|---|---|---|---|
| diablo4 (D4) | diablo4.blk.1 | always_online_drm | **Track A2 (2026-05-03)** | docs/D4_MANJEOM_REVIEW.md (NEW) |
| cs2 | cs2.blk.1 | vac_anticheat | **Track C (본 cycle 2026-05-03)** | docs/CS2_MANJEOM_REVIEW.md (REUSE) |
| delta_force | delta_force.blk.1 | eac_kernel_anticheat | **Track C (본 cycle 2026-05-03)** | docs/DELTA_FORCE_MANJEOM_REVIEW.md (REUSE) |
| elden_ring | elden_ring.blk.1 | eac_anti_tamper | **Track C (본 cycle 2026-05-03)** | docs/ELDEN_RING_MANJEOM_REVIEW.md (REUSE) |
| lineage_classic_r28 | lineage_classic_r28.blk.1 | gameguard_kernel_ac | **Track B (2026-05-03)** | docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md (NEW) |

→ **5/5 original candidate 정식 채택 완료**.

### §4.2 6th lock-in (WoW)

| 게임 | blocker | type | 정식 채택 cycle | 정식 채택 doc |
|---|---|---|---|---|
| **wow** | wow.blk.1 | online_mmo_subscription | **Track C (본 cycle 2026-05-03)** | **docs/WOW_MANJEOM_REVIEW.md (NEW)** |

→ predecessor `retired_intentional_enum_not_applied_6` array 측 wow 측 "candidate (lock-in pending)" rationale → 본 cycle 측
**6th lock-in 정식 채택 완료** (D4 always-online family peer adoption).

### §4.3 not-applied 5 (predecessor `retired_intentional_enum_not_applied_6` minus wow)

| 게임 | rationale (predecessor) | 본 cycle 후 status |
|---|---|---|
| diablo2_resurrected | offline mode handshake 가능 | (Track A scope, 본 cycle 측 untouched) |
| battlenet | offline shim 측 cond.1/cond.2 PASS 가능 | (Track A scope, 본 cycle 측 untouched) |
| lineage_m | purple_launcher cond met 시 cascading PASS | (Track B scope, 본 cycle 측 untouched) |
| lineage_w | loader sub-module land + purple_launcher cond met 시 cascading PASS | (Track B scope, 본 cycle 측 untouched) |
| purple_launcher | offline shim 측 cond.1/cond.2 PASS 가능 | (Track B scope, 본 cycle 측 untouched) |

→ 본 cycle 측 5 게임 측 file scope 분리 (untouched).

## §5 honest C3 (≥6 caveat)

C1 — 본 cycle 은 4-game retired_intentional 정식 채택 + WoW NEW mk1 narrative spec freeze 만. 본 cycle 측 perf module
self_test() validation 측 추가 0건 (별도 future cycle scope, retired_intentional posture 측 synthetic-data only).
live game state 측 측정 0건 (always-online + subscription 진입 X, AC bypass X).

C2 — 4 게임 측 `status_enum_adopted: retired_intentional` field 측 D4 first-mover (Track A2 2026-05-03) +
lineage_classic_r28 (Track B 2026-05-03) precedent 정합 적용. 본 cycle 측 새 schema field 도입 X — 단순 D4 first-mover
schema 적용 (status_enum_adopted + retired_intentional_posture + adoption_evidence + adopted_iso + adopted_via +
blocker_class). `adopted_via: track_c_4game_retired_intentional_adoption_landed` 측 본 cycle 측 first-introduction
(D4 측 `adopted_via` field 부재 — D4 marker 측 implicit 측 Track A2 명시), 향후 cycle 측 동일 pattern 적용 권고.

C3 — `retired_intentional_posture: block_acknowledged_pass_equivalent` 측 D4 first-mover 정의 (`docs/D4_MANJEOM_REVIEW.md`
§0.5-2) 와 동일 정의 적용 (4 게임 + WoW 측 모두 동일 정의 정합). 그러나 4 게임 측 차단 유형 다름 (CS2 = VAC user-mode AC,
DF = EAC kernel-mode AC, ER = EAC anti-tamper, WoW = online MMO subscription + always-online). evidence row 측
bypass kind 측 분기 reflect (`no_ac_bypass` for CS2/DF/ER, `no_subscription_bypass` for WoW). future cycle 측 4 차단
유형 별 spec freeze docs (VAC / EAC kernel / EAC anti-tamper / WoW realm + subscription) 별도 land 권고.

C4 — 4 .roadmap 측 in-place edit (3 cond evidence array append + blocker field add) = 정의상 `additive at field-level`
이지만, file-level 측 sha256 변경 (각 ~4700-5500 → 5800-6600 bytes). predecessor `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker`
측 명시한 4 게임 sha256 (63dd4ff... / edc8404... / d1aa8c1... / 5f3d05e...) 는 본 cycle 측 변경 reflect — 본 cycle marker
측 새 sha256 (69598f9... / 61a482b... / 049e099... / 57ba0bd...) pin.

C5 — `docs/WOW_MANJEOM_REVIEW.md` mk1 narrative 측 출처 (WebSearch + 본 repo 자료 종합) 일부 추정 (WoW realm server
emulator 가능성 community 보고 등 — TrinityCore / AzerothCore). 본 repo 측 우회 코드 emit 0, 실 Wow.exe / WowClassic.exe
분해 X, server emulator 자체 구현 X — skeleton-stage spec only, validated 0.

C6 — 2 c_wows_* perf module 측 alias overlap caveat (World of Warcraft vs World of Warships) — 본 repo airgenome-gamebox
design decision 측 honest reflect (`.roadmap.wow` cross_link.perf_pattern 측 명시 + `docs/WOW_MANJEOM_REVIEW.md` §4 측
명시). 별도 future cycle 측 `c_wows_*` → `c_wow_*` rename 가능성 caveat — 본 cycle 측 alias overlap 측 변경 X (file
scope 분리).

C7 — WoW 측 macOS 공식 native client (Apple Silicon Universal 2) 존재 측 본 cycle scope 외 (gamebox 는 Windows PE binary
측 own1 hexa loader 경로 집중 — Apple-native 측 own1 의미 X). future cycle 측 Apple-native macOS WoW path 측 별도
roadmap candidate 가능성 caveat (현재 .roadmap.wow 측 Windows PE binary path 측 집중 reflect).

C8 — 본 cycle 측 marker 측 Track A (battlenet + d2r) 측 marker (`state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker`
predecessor 존재 확인 — 별도 cycle 완료 land 추정) cross-reference 측 reconciliation cycle 측 별도 결정. 본 cycle
측 file scope 분리 (Track A scope 측 .roadmap.battlenet + .roadmap.diablo2_resurrected 측 untouched).

C9 — `.roadmap.wow` blk.1 type 측 `upstream_subscription` → `online_mmo_subscription` update — predecessor type 측
정합 reflect 측 (header 측 `blocker_class: online_mmo_subscription` 측 정합). 정의상 단순 rename (additive at field-level
+ value semantic 정합) — destructive 의미 X.

C10 — 본 cycle 측 file scope 측 lib/ tests/ native/ tool/ 측 untouched 100% (forbidden file scope 정합). 4 .roadmap
+ 1 NEW doc + 1 NEW handoff + 1 NEW marker = 7 files total (3 NEW + 4 additive edit).

## §6 next-cycle hooks

1. **2 c_wows_* perf module 측 self_test() synthetic-data validation cycle** (별도 — retired_intentional posture 측
   정합, live game state 측정 X) — `c_wows_port_warmup` + `c_wows_replay_playback` 측 self_test() 추가.
2. **3 c_cs2_* + 2 c_df_* + 10 c_elden_* perf module 측 self_test() synthetic-data validation cycle** (별도 — 본
   cycle 측 4 게임 측 retired_intentional 정식 채택 후 자연 후속).
3. **`.roadmap.battlenet` retired_intentional 채택 여부 결정 cycle** (Track A 후속) — 본 cycle 측 4 게임 + D4 + lineage_classic_r28
   측 retired_intentional 정식 채택 후 자연 후속 (Battle.net 측 offline shim 가능 측 미적용 권고).
4. **VAC honest spec freeze docs (`docs/VAC_HONEST_FREEZE.md` 가칭) + EAC kernel honest spec freeze docs
   (`docs/EAC_KERNEL_HONEST_FREEZE.md` 가칭) + EAC anti-tamper honest spec freeze docs
   (`docs/EAC_ANTITAMPER_HONEST_FREEZE.md` 가칭) + WoW realm + subscription honest spec freeze docs
   (`docs/WOW_REALM_HONEST_FREEZE.md` 가칭, `docs/WOW_SUBSCRIPTION_HONEST_FREEZE.md` 가칭) 별도 future cycle land**.
5. **`c_wows_*` → `c_wow_*` rename 결정 cycle** (alias overlap honest reflect 측 long-term resolution).
6. **mk2 status enum spec freeze 측 `retired_intentional` 정식 schema 등록 cycle** (rank A 3 도메인 land 시 동일
   caveat 누적 해소) — `tool/roadmap_op.hexa` 측 enum validator 추가 결정.
7. **본 cycle 측 marker 측 Track A (battlenet + d2r) 측 marker hash placeholder reconciliation cycle** (Track A 완료
   후) — 본 cycle marker 측 cross_reference field 측 update.
8. **WoW 측 macOS 공식 native client 측 별도 roadmap candidate 검토 cycle** (Apple Silicon Universal 2 binary 측 own1
   measurement 의미 분석 — gamebox identity 측 Apple-native 정합 가능성 결정).

## §7 file index (sha-pin at land time)

| path | type | size_b | LOC | sha256_hex |
|---|---|---:|---:|---|
| .roadmap.cs2 (modified) | roadmap_jsonl | 5783 | 5 | `69598f9a2cd500f311dc94be9e2b3a352d53e4e6317050b2633d8418a39c681e` |
| .roadmap.delta_force (modified) | roadmap_jsonl | 6140 | 5 | `61a482b62cdfe7686b0810390c3262e04714c0f727449f2a5eb0a258506785f8` |
| .roadmap.elden_ring (modified) | roadmap_jsonl | 6615 | 5 | `049e09948a51ad5ef2450c8f80141aaa9e4d48ad9d0ebfd68838025121be4e0a` |
| .roadmap.wow (modified) | roadmap_jsonl | 6475 | 5 | `57ba0bd76d8d64ee445d84b9f3fe76f54d9661397a8442fe97acf421d15a4181` |
| docs/WOW_MANJEOM_REVIEW.md (created) | mk1_narrative_doc | 32453 | 380 | `3daf5569711de9edfffd58586447aceae5f395e4f753dd3b963d913a18c8af97` |
| docs/airgenome_gamebox_4game_retired_intentional_adoption_landed_2026_05_03.ai.md (created — 본 doc) | handoff_doc | TBD | TBD | (set after write) |
| state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker (created) | marker | TBD | TBD | (self-referential — sha256 not pinned in own body) |

(`.roadmap.cs2` 측 predecessor sha256: `63dd4ff32f707389b6ac4330db691bfb696e37fa3c815ea632025c71051ac603`)
(`.roadmap.delta_force` 측 predecessor sha256: `edc840490a37a0ca392edbc94323e4dea8d0156b713ff01e346e1f26e0c662c1`)
(`.roadmap.elden_ring` 측 predecessor sha256: `d1aa8c1d2e3fb33de7b21ac008e1af05e18afaee1ee3c93c6dd008d023e323b7`)
(`.roadmap.wow` 측 predecessor sha256: `5f3d05e2d28f585091ef436c3e68fb8e85214cecdeed081dab0884e7704a2e48`)

## §8 policy summary

- migration: forbidden — 0건 emit
- additive only — 4 .roadmap 측 in-place edit 은 field-level additive (3 cond evidence row append + blocker field
  add), file 자체 destructive overwrite X. docs/WOW_MANJEOM_REVIEW.md + 본 doc + marker = 3 NEW only.
- destructive ops — 0건
- in-place writes — 4 (`.roadmap.cs2`, `.roadmap.delta_force`, `.roadmap.elden_ring`, `.roadmap.wow`, additive at
  field-level)
- substrate — mac-local
- cost — $0
- cap — 90min (실제 소요 ≪ cap)
- raw 9 hexa-only orchestration — 본 audit 자체는 hexa orchestrator 미사용 (single-doc + 4 JSONL field-level edit + 1
  marker emit + 1 NEW MANJEOM_REVIEW doc, predecessor 와 동일 raw 168 minimum-viable exempt)
- raw 12 silent-error ban — 4 .roadmap 측 blk.1 status `open` 유지 + status_enum_adopted 측 명시, 12 cond evidence
  row 측 status `partial` 유지 + block-acknowledged evidence row 명시 honest
- raw 15 env() lazy + <user> — 모든 path repo-relative, 사용자 절대 path X
- raw 175 BR-NO-USER-VERBATIM — 사용자 prompt 직접 인용 0건 (paraphrase only)
- friendly preset — handoff doc only (사용자 응답 X — BG subagent → 메인 monitor)
- silent-land — marker 측 silent_land 명시
- own1 — Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 — hexa-only, no AC bypass, no DRM bypass, no subscription
  circumvention
- own2 — log/error/emit/headless honest documentation (4 .roadmap 측 blk.1 status_enum_adopted + retired_intentional_posture
  명시 + docs/WOW_MANJEOM_REVIEW.md retired_intentional posture 명시)

## §9 sibling reference (predecessor 비교)

| 비교축 | predecessor mk2 (2026-05-02) | rank A 3 (2026-05-03) | per-game 11 (2026-05-03 fan-out) | D4 retired_intentional (2026-05-03 Track A2) | **4-game retired_intentional (본 cycle 2026-05-03 Track C)** |
|---|---|---|---|---|---|
| `.roadmap.*` files added | 0 (spec only) | 3 | 11 | 0 (1 file in-place additive edit) | **0 (4 file in-place additive edit)** |
| handoff doc | 1 | 1 | 1 | 1 | **1** |
| marker | 1 | 1 | 1 | 1 | **1** |
| docs/*.md added | 0 | 0 | 0 | 1 (mk1 narrative D4) | **1 (mk1 narrative WoW; CS2/DF/ER reused)** |
| status enum proposal/adoption | (none) | `retired_intentional` 후보 명시 | `retired_intentional` 5 게임 후보 (D4/CS2/DF/ER/LC R28) | `retired_intentional` D4 first-mover 정식 채택 | **`retired_intentional` 4 게임 정식 채택 (cs2 / delta_force / elden_ring / wow 6th lock-in)** |
| triplet_audit_tier | (n/a) | T1_large + T1 + T1 | (per-game tier 미명시) | (per-game tier 미명시) | (per-game tier 미명시) |
| cost / cap | $0 / 60min | $0 / 60min | $0 / 90min | $0 / 90min | **$0 / 90min** |
| concurrent tracks | (none) | (none) | (none) | Track A (battlenet+d2r) + Track B (lineage_classic_r28) 측 file scope 분리 | **Track A scope 측 file 분리 (battlenet+d2r 별 cycle 완료 추정)** |
| retired_intentional 정식 채택 누적 | 0 | 0 | 0 (5 후보 명시) | 1 (D4) | **6 (D4 + LC R28 + cs2 + DF + ER + wow)** — 5/5 original + 1 lock-in |
