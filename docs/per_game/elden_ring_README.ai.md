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

## §9 post-Track-BU/BV/BY/CA update (Track CC 2026-05-05) — DLC scope clarified + nightreign sibling spinoff

§2 cond statuses unchanged for elden_ring — all 3 cond actuals remain `partial` with `block_acknowledged_pass_equivalent` posture per blk.1 retired_intentional (EAC anti-tamper class, adopted Track-C 2026-05-03).

**Scope clarification (Track BY 2026-05-05)**: `.roadmap.elden_ring` scope = **base game (2022-02-25) + Shadow of the Erdtree DLC (2024-06-21)**. DLC inherits same EAC anti-tamper class as base; no separate cond entries needed for DLC. Sibling spinoff Elden Ring Nightreign (2025-05-30 standalone multiplayer co-op) tracked separately as `.roadmap.elden_ring_nightreign` per Track BY documentation-only fan-out (lib module fan-out deferred to Track CA+ candidate).

| event | track | iso | impact on elden_ring |
|---|---|---|---|
| Win32 fn coverage 91.49% → 100% milestone | BP | 2026-05-05 | skeleton-tier surface threshold breach; loader_win32 100% reach (relevant via eldenring.exe IAT real-resolve target) |
| .checkpoint.win32_full_coverage flip unmet → met | BU | 2026-05-05 | weighted_threshold derivation cached refresh |
| Track BE c_elden_* family 100% completion | BE | 2026-05-05 | 10/10 c_elden_* perf modules cumulative annotation (Track AS 6 + Track BE 4); status preserved partial (retired_intentional posture) |
| Track BM c_sote_realm_of_shadow_zone_load + c_sote_dlc_content_probe | BM | 2026-05-05 | Shadow of the Erdtree DLC zone load + DLC content probe scaffold; cond.2 + cond.3 evidence augment |
| Track BQ c_sote_* family 100% completion reaffirm | BQ | 2026-05-05 | 2/2 c_sote_* modules already extended in Track BM; no re-touch in BQ |
| **Track BY DLC scope clarification + nightreign fan-out** | **BY** | **2026-05-05** | **`.roadmap.elden_ring` goal field updated to mention base+DLC scope; new `dlc_scope` field added (Shadow of the Erdtree 2024-06-21); new `spinoff_cross_link` field references `.roadmap.elden_ring_nightreign`** |
| Layout migration commit `1d28b4c` | — | 2026-05-05 | mk1 narrative `docs/ELDEN_RING_MANJEOM_REVIEW.md` path unchanged (catch-all docs/); predecessor handoff under `airgenome/doc/` |

Honest scope: `validated_manjeom` UNCHANGED at 0 — no real Elden Ring frames evidenced (base or DLC). EAC anti-tamper retired_intentional permanent boundary on Apple-only stack (own1 Wine 0 + own2 honest emit, no AC bypass / no anti-tamper bypass / kernel driver avoid track honest). Sibling Elden Ring Nightreign inherits same class via Track BY fan-out.

Markers added to cycle history:

- `state/markers/airgenome_gamebox_perf_elden_retired_intentional_track_as_landed.marker` (Track AS — elden_ring perf scope retired_intentional annotation 2026-05-04)
- `state/markers/airgenome_gamebox_perf_d4_elden_completion_track_be_landed.marker` (Track BE — d4 + elden completion 2026-05-05)
- `state/markers/airgenome_gamebox_perf_remaining_family_inventory_v2_track_bm_landed.marker` (Track BM — c_sote_* DLC scaffold)
- `state/markers/airgenome_gamebox_perf_bm_6family_completion_track_bq_landed.marker` (Track BQ — 6-family completion incl. c_sote_* reaffirm)
- `state/markers/gamebox_readme_game_list_and_win32_checkpoint_flip_track_bu_landed.marker` (Track BU — Game Coverage section)
- `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker` (Track BY — DLC scope + nightreign fan-out)
- `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_cc_landed.marker` (this cycle 2026-05-05 — §9 closure)
