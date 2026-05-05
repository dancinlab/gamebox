---
schema: airgenome-gamebox/docs/per_game/wow_readme/ai-native/1
last_updated: 2026-05-03
roadmap_ssot: .roadmap.wow
status: T1_BASELINE_LANDED_2026_05_03
related_raws:
  - raw 270  # ai-native readme triplet T1 baseline
  - raw 271  # core+module pattern (per-game sub-bucket)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 175  # BR-NO-USER-VERBATIM
---

# wow README.ai.md (T1 baseline)

## §1 game identity

| field | value |
|---|---|
| name | wow |
| alias | world_of_warcraft |
| kind | game |
| release_date | (not pinned in roadmap header — Blizzard MMO retail + classic 2 client) |

## §2 cond status snapshot (read from .roadmap.wow at land-time)

| cond | desc | status |
|---|---|---|
| wow.cond.1 | Wow.exe (retail) + WowClassic.exe PE parse + IAT real-resolve + x64 mainCRTStartup reach | partial (block_acknowledged_pass_equivalent posture per blk.1 retired_intentional) |
| wow.cond.2 | D3D11/D3D12 renderer first-frame (dual renderer path) | partial (block_acknowledged_pass_equivalent posture) |
| wow.cond.3 | Battle.net launcher + online MMO subscription gate | partial (block_acknowledged_pass_equivalent posture, no subscription circumvention) |

## §3 blocker snapshot

| blocker | type | status | status_enum_adopted | adopted_iso | adopted_via |
|---|---|---|---|---|---|
| wow.blk.1 | online_mmo_subscription | open | retired_intentional | 2026-05-03 | track_c_4game_retired_intentional_adoption_landed |

retired_intentional_posture: `block_acknowledged_pass_equivalent` (D4 always-online family peer precedent)
adoption_evidence: `docs/WOW_MANJEOM_REVIEW.md`, `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker`

## §4 module count (from cross_link block)

- loader_count: 2 (`lib/loader/pe_wow_retail_track.hexa` + `lib/loader/pe_wow_classic_track.hexa`)
- perf_count: 2 (`lib/perf/c_wows_*.hexa` — alias overlap with World of Warships honest reflect)

## §5 cross_link to sister roadmaps + manjeom_review docs

sister_roadmaps:
- `.roadmap.loader_pe`
- `.roadmap.loader_win32`
- `.roadmap.loader_dx`
- `.roadmap.battlenet`

mk1_narrative_refs:
- `docs/D2R_D4_COMPARISON.md`
- `docs/BATTLENET_REAL_LAUNCH_PLAN.md`

## §6 mk1 narrative pointer

`docs/WOW_MANJEOM_REVIEW.md` — WoW mk1 narrative spec freeze. own1 Wine 0 + own2 honest emit, no subscription circumvention / no offline emulator. D4 always-online family peer precedent (D4 = retired_intentional first-mover Track A2; WoW = peer adoption Track C 6th lock-in).

## §7 cycle history (markers referencing this game)

- `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (initial 11-roadmap fan-out 2026-05-03)
- `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` (Track C 4-game + WoW 6th lock-in 2026-05-03 — peer retired_intentional adoption)
- `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker` (this cycle 2026-05-03 — T1 README baseline)

## §8 raw 270 triplet T1 baseline note

This README.ai.md satisfies the raw 270 triplet T1 baseline as of 2026-05-03 (predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` raw_compliance flagged README.ai.md emit deferred / baseline grandfather candidate vs 2026-06-01 promotion-day deadline — this cycle satisfies that deadline ahead of schedule). T2/T3/T4 remain future-cycle work.
