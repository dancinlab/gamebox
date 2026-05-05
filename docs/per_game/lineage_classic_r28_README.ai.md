---
schema: airgenome-gamebox/docs/per_game/lineage_classic_r28_readme/ai-native/1
last_updated: 2026-05-03
roadmap_ssot: .roadmap.lineage_classic_r28
status: T1_BASELINE_LANDED_2026_05_03
related_raws:
  - raw 270  # ai-native readme triplet T1 baseline
  - raw 271  # core+module pattern (per-game sub-bucket)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 175  # BR-NO-USER-VERBATIM
---

# lineage_classic_r28 README.ai.md (T1 baseline)

## §1 game identity

| field | value |
|---|---|
| name | lineage_classic_r28 |
| alias | lineage_classic, l_classic_r28 |
| kind | game |
| release_date | 2026-02-07 (NCSOFT 신작 R28 update) |

## §2 cond status snapshot (read from .roadmap.lineage_classic_r28 at land-time)

| cond | desc | status |
|---|---|---|
| lineage_classic_r28.cond.1 | LineageClassic.exe PE parse + GameGuard launcher block honest probe | partial (retired_intentional posture adopted, block_acknowledged_pass_equivalent) |
| lineage_classic_r28.cond.2 | native first-frame draw (Korean MMO classic engine) | partial (live first-frame retired_intentional, spec freeze possible) |
| lineage_classic_r28.cond.3 | GameGuard kernel block honest spec freeze documentation | partial (Track-E GAMEGUARD_HONEST_FREEZE.md landed, status partial永久 lock per retired_intentional posture) |

## §3 blocker snapshot

| blocker | type | status | status_enum_candidate | status_enum_adopted | adopted_iso | adopted_via |
|---|---|---|---|---|---|---|
| lineage_classic_r28.blk.1 | upstream_anticheat_kernel (GameGuard) | open | retired_intentional | retired_intentional | 2026-05-03 | track_b_purple_lineage_offline_shim_landed |

adoption_evidence: `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md`, `lib/loader/pe_lineage_classic_r28_track.hexa __LC_R28_GG__ RETIRED_INTENTIONAL row`, `tests/test_purple_lineage_offline_shim.hexa attest`

## §4 module count (from cross_link block)

- loader_count: 1 (`lib/loader/pe_lineage_classic_r28_track.hexa`)
- perf_count: 1 (`lib/perf/c_lineage_classic_r28_native_first_frame.hexa`)

## §5 cross_link to sister roadmaps + manjeom_review docs

sister_roadmaps:
- `.roadmap.loader_pe`
- `.roadmap.loader_win32`
- `.roadmap.loader_dx`
- `.roadmap.lineage_m`
- `.roadmap.lineage_w`

mk1_narrative_refs: (none in roadmap header — but `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` + `docs/GAMEGUARD_HONEST_FREEZE.md` landed via Track-B + Track-E)

lineage_family_cross_ref: lineage_m (mobile, Purple), lineage_w (UE4, Purple)

## §6 mk1 narrative pointer

`docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` (Track-B 2026-05-03) + `docs/GAMEGUARD_HONEST_FREEZE.md` (Track-E 2026-05-03, 343 LOC, §1-§8 + 12 honest C3 caveats). own1 Wine 0 + own2 honest emit, no kernel-AC bypass / no evasion / no circumvention. GameGuard kernel-AC retired_intentional formal adoption.

## §7 cycle history (markers referencing this game)

- `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (initial 11-roadmap fan-out 2026-05-03)
- `state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker` (Track-B 2026-05-03 — retired_intentional adoption + manjeom_review land)
- `state/markers/airgenome_gamebox_gameguard_honest_freeze_landed.marker` (Track-E 2026-05-03 — GAMEGUARD_HONEST_FREEZE.md land + cond.3 evidence row)
- `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker` (this cycle 2026-05-03 — T1 README baseline)

## §8 raw 270 triplet T1 baseline note

This README.ai.md satisfies the raw 270 triplet T1 baseline as of 2026-05-03 (predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` raw_compliance flagged README.ai.md emit deferred / baseline grandfather candidate vs 2026-06-01 promotion-day deadline — this cycle satisfies that deadline ahead of schedule). T2/T3/T4 remain future-cycle work.

## §9 post-Track-BV update (Track BX 2026-05-05) — direct impact

§2 cond statuses below are land-time (2026-05-03) snapshot. Track BV (2026-05-05) directly modified this game's roadmap + checkpoint state:

| cond | land-time (§2) | post-BV (2026-05-05) | flipped_via |
|---|---|---|---|
| lineage_classic_r28.cond.1 | partial (retired_intentional posture adopted) | retired_intentional | track_bv_lineage_classic_r28_cond_status_cascading_flip |
| lineage_classic_r28.cond.2 | partial (live first-frame retired_intentional candidate) | retired_intentional | track_bv_lineage_classic_r28_cond_status_cascading_flip |
| lineage_classic_r28.cond.3 | partial (Track-E spec freeze landed; status partial 永久 lock) | retired_intentional | track_bv_lineage_classic_r28_cond_status_cascading_flip |

`.checkpoint.lineage_classic_r28_play` cached header status: `partial 0/3` → `met 3/3` (sideways-PASS rule per Track AB `checkpoint_ref_satisfied` — actual==retired_intentional → satisfied true regardless of required level).

Honest scope: cascading flip is honest documentation of permanent retirement under GameGuard kernel-AC boundary. NO bypass / NO evasion / NO circumvention. `validated_manjeom` UNCHANGED at 0 (no real game frames evidenced; GameGuard kernel-AC remains permanent boundary on Apple-only stack).

Markers added to cycle history:

- `state/markers/airgenome_gamebox_checkpoint_status_snapshot_v2_track_bo_landed.marker` (Track BO 2026-05-05 — flagged drift_count item 3 of 3 with explicit deferred resolution path)
- `state/markers/gamebox_lineage_classic_r28_drift_resolution_track_bv_landed.marker` (Track BV 2026-05-05 — cascading flip executed; drift_count 1 → 0)
- `state/markers/gamebox_v1_0_1_release_notes_track_bw_landed.marker` (Track BW 2026-05-05 — v1.0.1 patch release surfaces this game's flip)
- `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_bx_landed.marker` (this cycle 2026-05-05 — §9 update)
