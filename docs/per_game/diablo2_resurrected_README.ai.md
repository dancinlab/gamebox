---
schema: airgenome-gamebox/docs/per_game/diablo2_resurrected_readme/ai-native/1
last_updated: 2026-05-03
roadmap_ssot: .roadmap.diablo2_resurrected
status: T1_BASELINE_LANDED_2026_05_03
related_raws:
  - raw 270  # ai-native readme triplet T1 baseline
  - raw 271  # core+module pattern (per-game sub-bucket)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 175  # BR-NO-USER-VERBATIM
---

# diablo2_resurrected README.ai.md (T1 baseline)

## §1 game identity

| field | value |
|---|---|
| name | diablo2_resurrected |
| alias | d2r |
| kind | game |
| release_date | (not pinned in roadmap header) |

## §2 cond status snapshot (read from .roadmap.diablo2_resurrected at land-time)

| cond | desc | status |
|---|---|---|
| diablo2_resurrected.cond.1 | D2R.exe PE parse + IAT real-resolve + i386/x64 mainCRTStartup reach | partial |
| diablo2_resurrected.cond.2 | MPQ archive parse + savegame d2s format read/write | met |
| diablo2_resurrected.cond.3 | D3D11 renderer init + first-frame draw + dx11 pipeline warm | partial |

## §3 blocker snapshot

| blocker | type | status | status_enum_candidate | status_enum_adopted |
|---|---|---|---|---|
| diablo2_resurrected.blk.1 | upstream_launcher (Battle.net launcher dependency, offline mode possible) | open | none | (not adopted — offline mode handshake possible) |

## §4 module count (from cross_link block)

- loader_count: 5 (`lib/loader/pe_d2r_*.hexa`: mpq + d3d11 + real-launch + savegame + specific-track)
- perf_count: 26 (`lib/perf/c_d2r_*.hexa`)

## §5 cross_link to sister roadmaps + manjeom_review docs

sister_roadmaps:
- `.roadmap.loader_pe`
- `.roadmap.loader_win32`
- `.roadmap.loader_dx`
- `.roadmap.battlenet`

mk1_narrative_refs:
- `docs/D2R_D4_COMPARISON.md`
- `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md`

## §6 mk1 narrative pointer

manjeom review pending separate cycle — D2R-specific manjeom doc not yet landed; cross-references via `docs/D2R_D4_COMPARISON.md` + `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` (peer D4 family). cond.2 met posture + cond.1/cond.3 partial reflected via Track-A Battle.net offline-shim cycle + Track-D D2R cond.1 synthetic round-trip cycle.

## §7 cycle history (markers referencing this game)

- `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (initial 11-roadmap fan-out land 2026-05-03)
- `state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker` (Track-A Battle.net+D2R offline shim 2026-05-03 — cond.2 met)
- `state/markers/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed.marker` (Track-D synthetic D2R.exe PE round-trip 2026-05-03 — cond.1 evidence augment)
- `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker` (this cycle 2026-05-03 — T1 README baseline)

## §8 raw 270 triplet T1 baseline note

This README.ai.md satisfies the raw 270 triplet T1 baseline as of 2026-05-03 (predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` raw_compliance.raw_270_271_272_273_triplet field flagged README.ai.md emit deferred / baseline grandfather candidate vs 2026-06-01 promotion-day deadline — this cycle satisfies that deadline ahead of schedule). T2 (lint extension), T3 (hierarchy connection direction), T4 (full module-by-module spec freeze) remain future-cycle work.
