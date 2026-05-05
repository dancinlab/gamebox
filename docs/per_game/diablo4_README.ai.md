---
schema: airgenome-gamebox/docs/per_game/diablo4_readme/ai-native/1
last_updated: 2026-05-03
roadmap_ssot: .roadmap.diablo4
status: T1_BASELINE_LANDED_2026_05_03
related_raws:
  - raw 270  # ai-native readme triplet T1 baseline
  - raw 271  # core+module pattern (per-game sub-bucket)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 175  # BR-NO-USER-VERBATIM
---

# diablo4 README.ai.md (T1 baseline)

## §1 game identity

| field | value |
|---|---|
| name | diablo4 |
| alias | d4 |
| kind | game |
| release_date | (not pinned in roadmap header — Blizzard 2023 retail, ongoing seasons) |

## §2 cond status snapshot (read from .roadmap.diablo4 at land-time)

| cond | desc | status |
|---|---|---|
| diablo4.cond.1 | D4.exe PE parse + IAT real-resolve + x64 mainCRTStartup reach | partial (block_acknowledged_pass_equivalent posture per blk.1 retired_intentional) |
| diablo4.cond.2 | D3D12 + DXR PSO genome harvest + first-frame ray-trace | partial (block_acknowledged_pass_equivalent posture) |
| diablo4.cond.3 | seasonal/world/dungeon/loot economy genome harvest spec freeze | partial (block_acknowledged_pass_equivalent posture, 27 module spec freeze cross-link) |

## §3 blocker snapshot

| blocker | type | status | status_enum_adopted | adopted_iso | adopted_via |
|---|---|---|---|---|---|
| diablo4.blk.1 | upstream_drm (always_online_drm) | open | retired_intentional | 2026-05-03 | (Track A2 D4 retired_intentional adoption — first-mover) |

retired_intentional_posture: `block_acknowledged_pass_equivalent`
adoption_evidence: `docs/D4_MANJEOM_REVIEW.md`, `state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker`

## §4 module count (from cross_link block)

- loader_count: 2 (`lib/loader/d4_launch.hexa` + `lib/loader/pe_d4_specific_track.hexa`)
- perf_count: 27 (`lib/perf/c_d4_*.hexa`)

## §5 cross_link to sister roadmaps + manjeom_review docs

sister_roadmaps:
- `.roadmap.loader_pe`
- `.roadmap.loader_win32`
- `.roadmap.loader_dx`
- `.roadmap.battlenet`

mk1_narrative_refs:
- `docs/D2R_D4_COMPARISON.md`
- `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md`
- `docs/BATTLENET_REAL_LAUNCH_PLAN.md`

## §6 mk1 narrative pointer

`docs/D4_MANJEOM_REVIEW.md` — D4 mk1 narrative spec freeze (337 LOC, 9 top-level sections, 3 manjeom axes A/B/C reflected). retired_intentional first-mover formal adoption doc. own1 Wine 0 + own2 honest emit, no DRM bypass / no always-online evasion.

## §7 cycle history (markers referencing this game)

- `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (initial 11-roadmap fan-out 2026-05-03)
- `state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker` (Track A2 retired_intentional first-mover 2026-05-03)
- `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` (Track C 4-game peer adoption — D4 precedent referenced)
- `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker` (this cycle 2026-05-03 — T1 README baseline)

## §8 raw 270 triplet T1 baseline note

This README.ai.md satisfies the raw 270 triplet T1 baseline as of 2026-05-03 (predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` raw_compliance flagged README.ai.md emit deferred / baseline grandfather candidate vs 2026-06-01 promotion-day deadline — this cycle satisfies that deadline ahead of schedule). T2/T3/T4 remain future-cycle work.

## §9 post-Track-BU/BV/BY/CA update (Track CC 2026-05-05) — indirect impact, retired_intentional stable

§2 cond statuses unchanged for diablo4 — all 3 cond actuals remain `partial` with `block_acknowledged_pass_equivalent` posture per blk.1 retired_intentional (always-online DRM class, adopted Track A2 first-mover 2026-05-03; D4 = first retired_intentional adoption precedent for the project).

| event | track | iso | impact on diablo4 |
|---|---|---|---|
| Win32 fn coverage 91.49% → 100% milestone | BP | 2026-05-05 | skeleton-tier surface threshold breach; loader_win32 100% reach (relevant via D4.exe + battlenet launcher cross-cutting) |
| .checkpoint.win32_full_coverage flip unmet → met | BU | 2026-05-05 | weighted_threshold derivation cached refresh |
| Track BE c_d4_* family 100% completion | BE | 2026-05-05 | 27/27 c_d4_* perf modules cumulative annotation; status preserved partial (retired_intentional posture) |
| Track BA c_d2r_d4_completion | BA | 2026-05-04 | d2r + d4 perf families joint completion |
| README Game Coverage table | BY | 2026-05-05 | diablo4 row preserved with retired_intentional (always-online DRM) note |
| Layout migration commit `1d28b4c` | — | 2026-05-05 | mk1 narrative `docs/D4_MANJEOM_REVIEW.md` + `docs/D2R_D4_COMPARISON.md` + `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` paths unchanged (catch-all docs/); predecessor handoff under `airgenome/doc/` |

Honest scope: `validated_manjeom` UNCHANGED at 0 — no real D4 frames evidenced. Always-online DRM permanent boundary (own1 Wine 0 + own2 honest emit, no DRM bypass / no always-online evasion). D4 is the project's first retired_intentional adoption precedent — sibling games (cs2, delta_force, elden_ring, lineage_classic_r28, wow) inherit this class via Track-C 4-game adoption pattern.

Markers added to cycle history:

- `state/markers/airgenome_gamebox_perf_d2r_d4_completion_track_ba_landed.marker` (Track BA — d2r + d4 family completion 2026-05-04)
- `state/markers/airgenome_gamebox_perf_d4_elden_completion_track_be_landed.marker` (Track BE — d4 + elden completion 2026-05-05)
- `state/markers/gamebox_readme_game_list_and_win32_checkpoint_flip_track_bu_landed.marker` (Track BU — Game Coverage section)
- `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker` (Track BY — README refresh)
- `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_cc_landed.marker` (this cycle 2026-05-05 — §9 closure)
