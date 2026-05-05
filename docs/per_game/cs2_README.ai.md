---
schema: airgenome-gamebox/docs/per_game/cs2_readme/ai-native/1
last_updated: 2026-05-03
roadmap_ssot: .roadmap.cs2
status: T1_BASELINE_LANDED_2026_05_03
related_raws:
  - raw 270  # ai-native readme triplet T1 baseline
  - raw 271  # core+module pattern (per-game sub-bucket)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 175  # BR-NO-USER-VERBATIM
---

# cs2 README.ai.md (T1 baseline)

## §1 game identity

| field | value |
|---|---|
| name | cs2 |
| alias | counter_strike_2 |
| kind | game |
| release_date | (not pinned in roadmap header — Valve Source 2 engine, 2023) |

## §2 cond status snapshot (read from .roadmap.cs2 at land-time)

| cond | desc | status |
|---|---|---|
| cs2.cond.1 | cs2.exe PE parse + Steam API 64 offline shim | partial (block_acknowledged_pass_equivalent posture per blk.1 retired_intentional) |
| cs2.cond.2 | Source 2 PSO genome harvest + tier0 plat shim | partial (block_acknowledged_pass_equivalent posture) |
| cs2.cond.3 | VAC anti-cheat honest spec freeze documentation | partial (mk1 narrative reused, future VAC_HONEST_FREEZE.md cycle pending) |

## §3 blocker snapshot

| blocker | type | status | status_enum_adopted | adopted_iso | adopted_via |
|---|---|---|---|---|---|
| cs2.blk.1 | upstream_anticheat (vac_anticheat) | open | retired_intentional | 2026-05-03 | track_c_4game_retired_intentional_adoption_landed |

retired_intentional_posture: `block_acknowledged_pass_equivalent`
adoption_evidence: `docs/CS2_MANJEOM_REVIEW.md`, `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker`

## §4 module count (from cross_link block)

- loader_count: 1 (`lib/loader/pe_cs2_specific_track.hexa`)
- perf_count: 3 (`lib/perf/c_cs2_*.hexa`: source2_pso_genome + steam_api64_offline_shim + tier0_plat_shim)

## §5 cross_link to sister roadmaps + manjeom_review docs

sister_roadmaps:
- `.roadmap.loader_pe`
- `.roadmap.loader_win32`
- `.roadmap.loader_dx`
- `.roadmap.delta_force`
- `.roadmap.elden_ring`

mk1_narrative_refs:
- `docs/CS2_MANJEOM_REVIEW.md`
- `docs/DELTA_FORCE_MANJEOM_REVIEW.md`
- `docs/ELDEN_RING_MANJEOM_REVIEW.md`

anticheat_family_cross_ref: delta_force (kernel-AC EAC), elden_ring (EAC anti-tamper)

## §6 mk1 narrative pointer

`docs/CS2_MANJEOM_REVIEW.md` — CS2 mk1 narrative spec freeze (predecessor REUSE in Track C). own1 Wine 0 + own2 honest emit, no AC bypass / no Trusted Mode bypass. VAC user-mode AC retired_intentional formal adoption.

## §7 cycle history (markers referencing this game)

- `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (initial 11-roadmap fan-out 2026-05-03)
- `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` (Track C 4-game retired_intentional 2026-05-03 — CS2 adopted)
- `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker` (this cycle 2026-05-03 — T1 README baseline)

## §8 raw 270 triplet T1 baseline note

This README.ai.md satisfies the raw 270 triplet T1 baseline as of 2026-05-03 (predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` raw_compliance flagged README.ai.md emit deferred / baseline grandfather candidate vs 2026-06-01 promotion-day deadline — this cycle satisfies that deadline ahead of schedule). T2/T3/T4 remain future-cycle work.

## §9 post-Track-BU/BV/BY/CA update (Track CC 2026-05-05) — indirect impact, retired_intentional stable

§2 cond statuses unchanged for cs2 — all 3 cond actuals remain `partial` with `block_acknowledged_pass_equivalent` posture per blk.1 retired_intentional (VAC user-mode AC class, adopted Track-C 2026-05-03).

| event | track | iso | impact on cs2 |
|---|---|---|---|
| Win32 fn coverage 91.49% → 100% milestone | BP | 2026-05-05 | skeleton-tier surface threshold breach; loader_win32 100% reach (relevant for cs2.exe IAT real-resolve target) |
| .checkpoint.win32_full_coverage flip unmet → met | BU | 2026-05-05 | weighted_threshold derivation cached refresh |
| Track BM c_steam_drm_stub_track + Track BQ c_steam_* family 100% completion | BM/BQ | 2026-05-05 | cs2 cond.1 evidence row augment; status preserved partial |
| README Game Coverage table | BY | 2026-05-05 | cs2 row preserved with retired_intentional (VAC AC) note |
| Layout migration commit `1d28b4c` | — | 2026-05-05 | mk1 narrative `docs/CS2_MANJEOM_REVIEW.md` path unchanged (catch-all docs/); predecessor handoff under `airgenome/doc/` |

Honest scope: `validated_manjeom` UNCHANGED at 0 — no real CS2 frames evidenced. VAC anti-cheat retired_intentional permanent boundary on Apple-only stack (own1 Wine 0 + own2 honest emit, no AC bypass / no Trusted Mode bypass). `docs/VAC_HONEST_FREEZE.md` mk1 narrative deferred future-cycle (Track CC+ candidate alongside `docs/EAC_KERNEL_HONEST_FREEZE.md`).

Markers added to cycle history:

- `state/markers/airgenome_gamebox_perf_cs2_retired_intentional_track_at_landed.marker` (Track AT — cs2 perf scope retired_intentional annotation 2026-05-04)
- `state/markers/gamebox_readme_game_list_and_win32_checkpoint_flip_track_bu_landed.marker` (Track BU — Game Coverage section)
- `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker` (Track BY — README refresh)
- `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_cc_landed.marker` (this cycle 2026-05-05 — §9 closure)
