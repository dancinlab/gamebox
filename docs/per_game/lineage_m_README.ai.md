---
schema: airgenome-gamebox/docs/per_game/lineage_m_readme/ai-native/1
last_updated: 2026-05-03
roadmap_ssot: .roadmap.lineage_m
status: T1_BASELINE_LANDED_2026_05_03
related_raws:
  - raw 270  # ai-native readme triplet T1 baseline
  - raw 271  # core+module pattern (per-game sub-bucket)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 175  # BR-NO-USER-VERBATIM
---

# lineage_m README.ai.md (T1 baseline)

## §1 game identity

| field | value |
|---|---|
| name | lineage_m |
| alias | lineage_mobile, l_m_purple |
| kind | game |
| release_date | 2017 (KR mobile MMORPG, NCSOFT) |

## §2 cond status snapshot (read from .roadmap.lineage_m at land-time)

| cond | desc | status |
|---|---|---|
| lineage_m.cond.1 | Purple launcher Android emul layer APK launch | met (2026-05-03, met_via=purple_cascading_offline_shim) |
| lineage_m.cond.2 | APK emul first-frame draw + asset cache | partial (Android emul layer + plaync OAuth gate transitive) |
| lineage_m.cond.3 | plaync OAuth + 2FA TOTP gate | partial (network/user-secret dependency) |

## §3 blocker snapshot

| blocker | type | status | status_enum_candidate | status_enum_adopted |
|---|---|---|---|---|
| lineage_m.blk.1 | upstream_launcher (purple_launcher transitive) | open | none | (not adopted — purple_launcher cond met cascading PASS path available) |

## §4 module count (from cross_link block)

- loader_count: 1 (`lib/loader/pe_lineage_m_purple_track.hexa`)
- perf_count: 1 (`lib/perf/c_lineage_m_purple_apk_emul_first_frame.hexa`)

## §5 cross_link to sister roadmaps + manjeom_review docs

sister_roadmaps:
- `.roadmap.loader_pe`
- `.roadmap.loader_win32`
- `.roadmap.loader_dx`
- `.roadmap.purple_launcher`
- `.roadmap.lineage_classic_r28`
- `.roadmap.lineage_w`

mk1_narrative_refs: (none in roadmap header — Lineage M-specific manjeom doc not yet landed)

## §6 mk1 narrative pointer

manjeom review pending separate cycle — Lineage M-specific manjeom doc not yet landed; cross-references via Purple launcher chain (`.roadmap.purple_launcher` cond.1+cond.2 met via offline_shim_track_b) + lineage family sister roadmaps (lineage_classic_r28 / lineage_w). cond.1 cascaded met posture from purple_launcher offline shim landing.

## §7 cycle history (markers referencing this game)

- `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (initial 11-roadmap fan-out 2026-05-03)
- `state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker` (Track-B Purple→Lineage offline-shim 2026-05-03 — cond.1 cascaded met)
- `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker` (this cycle 2026-05-03 — T1 README baseline)

## §8 raw 270 triplet T1 baseline note

This README.ai.md satisfies the raw 270 triplet T1 baseline as of 2026-05-03 (predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` raw_compliance flagged README.ai.md emit deferred / baseline grandfather candidate vs 2026-06-01 promotion-day deadline — this cycle satisfies that deadline ahead of schedule). T2/T3/T4 remain future-cycle work.

## §9 post-Track-BU/BV update (Track BX 2026-05-05) — indirect impact via cross-cutting loader

§2 cond statuses unchanged for lineage_m at this cycle (cond.1 met via Track-B Purple cascading; cond.2 partial — Android emul layer + plaync OAuth gate transitive; cond.3 partial — plaync OAuth network/user secret).

| event | track | iso | impact on lineage_m |
|---|---|---|---|
| Win32 fn coverage 91.49% → 100% milestone | BP | 2026-05-05 | skeleton-tier surface threshold breach; relevant via loader_win32 cross-cutting (Purple shell uses Win32 surface) |
| .checkpoint.win32_full_coverage flip unmet → met | BU | 2026-05-05 | weighted_threshold derivation cached refresh |
| .checkpoint.lineage_m_purple_play | (no flip) | 2026-05-05 | derived partial 3/5 unchanged (lineage_m cond.2/cond.3 still partial; purple_launcher cond.1+cond.2 met) |

Honest scope: `validated_manjeom` UNCHANGED at 0 — no real Lineage M APK emul first-frame evidenced. Android emul layer permanent boundary on Apple-only stack remains the structural gate for cond.2; plaync OAuth user lock-in remains the cond.3 gate.

Markers added to cycle history:

- `state/markers/airgenome_gamebox_win32_cycle60_62_bundled_increment_landed.marker` (Track BP — Win32 100% milestone)
- `state/markers/gamebox_readme_game_list_and_win32_checkpoint_flip_track_bu_landed.marker` (Track BU)
- `state/markers/gamebox_v1_0_1_release_notes_track_bw_landed.marker` (Track BW — v1.0.1 release)
- `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_bx_landed.marker` (this cycle 2026-05-05 — §9 update)
