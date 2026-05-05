---
schema: airgenome-gamebox/docs/per_game/elden_ring_readme/ai-native/1
last_updated: 2026-05-03
roadmap_ssot: .roadmap.elden_ring
status: T1_BASELINE_LANDED_2026_05_03
related_raws:
  - raw 270  # ai-native readme triplet T1 baseline
  - raw 271  # core+module pattern (per-game sub-bucket)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 175  # BR-NO-USER-VERBATIM
---

# elden_ring README.ai.md (T1 baseline)

## §1 game identity

| field | value |
|---|---|
| name | elden_ring |
| alias | er, eldenring |
| kind | game |
| release_date | (not pinned in roadmap header — FromSoftware action RPG, 2022) |

## §2 cond status snapshot (read from .roadmap.elden_ring at land-time)

| cond | desc | status |
|---|---|---|
| elden_ring.cond.1 | eldenring.exe PE parse + EAC offline launch probe | partial (block_acknowledged_pass_equivalent posture per blk.1 retired_intentional) |
| elden_ring.cond.2 | D3D12 first-frame + PSO warm + FromSoft HKS script cache | partial (block_acknowledged_pass_equivalent posture) |
| elden_ring.cond.3 | MSIX container extract + MS Store license stub + offline save compat | partial (3 component spec, real container round-trip pending) |

## §3 blocker snapshot

| blocker | type | status | status_enum_adopted | adopted_iso | adopted_via |
|---|---|---|---|---|---|
| elden_ring.blk.1 | upstream_anticheat_anti_tamper (eac_anti_tamper) | open | retired_intentional | 2026-05-03 | track_c_4game_retired_intentional_adoption_landed |

retired_intentional_posture: `block_acknowledged_pass_equivalent`
adoption_evidence: `docs/ELDEN_RING_MANJEOM_REVIEW.md`, `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker`

## §4 module count (from cross_link block)

- loader_count: 1 (`lib/loader/pe_elden_ring_specific_track.hexa`)
- perf_count: 10 (`lib/perf/c_elden_*.hexa`: dx12_first_frame + dx12_pso_warm + eac_kernel_driver_avoid_track + eac_offline_launch_probe + fromsoft_hks_script_cache + msix_container_extract + msstore_license_stub + offline_save_compat + online_entry_block + pe64_loader_probe)

## §5 cross_link to sister roadmaps + manjeom_review docs

sister_roadmaps:
- `.roadmap.loader_pe`
- `.roadmap.loader_win32`
- `.roadmap.loader_dx`
- `.roadmap.cs2`
- `.roadmap.delta_force`

mk1_narrative_refs:
- `docs/ELDEN_RING_MANJEOM_REVIEW.md`
- `docs/DELTA_FORCE_MANJEOM_REVIEW.md`
- `docs/CS2_MANJEOM_REVIEW.md`

anticheat_family_cross_ref: cs2 (VAC user-mode AC), delta_force (EAC kernel-AC)

## §6 mk1 narrative pointer

`docs/ELDEN_RING_MANJEOM_REVIEW.md` — Elden Ring mk1 narrative spec freeze (predecessor REUSE in Track C). own1 Wine 0 + own2 honest emit, no AC bypass / no anti-tamper bypass / kernel driver avoid track honest. EAC anti-tamper retired_intentional formal adoption.

## §7 cycle history (markers referencing this game)

- `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (initial 11-roadmap fan-out 2026-05-03)
- `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` (Track C 4-game retired_intentional 2026-05-03 — Elden Ring adopted)
- `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker` (this cycle 2026-05-03 — T1 README baseline)

## §8 raw 270 triplet T1 baseline note

This README.ai.md satisfies the raw 270 triplet T1 baseline as of 2026-05-03 (predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` raw_compliance flagged README.ai.md emit deferred / baseline grandfather candidate vs 2026-06-01 promotion-day deadline — this cycle satisfies that deadline ahead of schedule). T2/T3/T4 remain future-cycle work.
