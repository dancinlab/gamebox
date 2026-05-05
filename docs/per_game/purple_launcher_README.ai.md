---
schema: airgenome-gamebox/docs/per_game/purple_launcher_readme/ai-native/1
last_updated: 2026-05-03
roadmap_ssot: .roadmap.purple_launcher
status: T1_BASELINE_LANDED_2026_05_03
related_raws:
  - raw 270  # ai-native readme triplet T1 baseline
  - raw 271  # core+module pattern (per-launcher sub-bucket)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 175  # BR-NO-USER-VERBATIM
---

# purple_launcher README.ai.md (T1 baseline)

## §1 launcher identity

| field | value |
|---|---|
| name | purple_launcher |
| alias | ncsoft_purple, plaync_purple |
| kind | launcher |
| release_date | (not pinned in roadmap header — NCSOFT PC launcher shell, ongoing) |

## §2 cond status snapshot (read from .roadmap.purple_launcher at land-time)

| cond | desc | status |
|---|---|---|
| purple_launcher.cond.1 | .NET CLR probe + WebView2 (Edge Chromium) warm | met (2026-05-03, met_via=offline_shim_track_b — 2 runtime PASS) |
| purple_launcher.cond.2 | Android emulation layer track (gles_to_metal passthrough, multi-stage) | met (2026-05-03, met_via=offline_shim_track_b — 4 stage PASS) |
| purple_launcher.cond.3 | plaync OAuth 2-leg + 2FA TOTP proxy live state machine | partial (network/user-secret dependency) |

## §3 blocker snapshot

| blocker | type | status | status_enum_candidate | status_enum_adopted |
|---|---|---|---|---|
| purple_launcher.blk.1 | upstream_network (plaync global endpoint network dependency) | open | none | (not adopted — offline-mode cond.1/cond.2 path met, online cond.3 user lock-in path) |

## §4 module count (from cross_link block)

- loader_count: 0 (no separate PE binary — plaync distribution direct download/install path)
- perf_count: 5 (`lib/perf/c_purple_*.hexa`: 2fa_totp_proxy + dotnet_clr_probe + plaync_oauth_proxy + webview2_warm + android_emul_layer_track)

## §5 cross_link to sister roadmaps + manjeom_review docs

sister_roadmaps:
- `.roadmap.loader_pe`
- `.roadmap.loader_win32`
- `.roadmap.loader_dx`
- `.roadmap.lineage_m`
- `.roadmap.lineage_w`
- `.roadmap.battlenet`

downstream_games: lineage_m (Purple required), lineage_w (Purple required), lineage_2m (future), aion (future), blade_and_soul (future), throne_and_liberty (future)

battlenet_pattern_cross_ref: analog launcher class (OAuth 2-leg + agent shell pattern shared)

mk1_narrative_refs: (none in roadmap header)

## §6 mk1 narrative pointer

`docs/PURPLE_LINEAGE_MANJEOM_REVIEW.md` (Track-B sibling family) + Track-B handoff `docs/airgenome_gamebox_purple_lineage_offline_shim_landed_2026_05_03.ai.md`. own1 Wine 0 + own2 honest emit, no plaync OAuth circumvention. cond.1+cond.2 offline-shim PASS posture, cond.3 awaits user lock-in.

## §7 cycle history (markers referencing this launcher)

- `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (initial 11-roadmap fan-out 2026-05-03)
- `state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker` (Track-B 2026-05-03 — cond.1+cond.2 met via offline shim, downstream lineage_m/lineage_w cascading)
- `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker` (this cycle 2026-05-03 — T1 README baseline)

## §8 raw 270 triplet T1 baseline note

This README.ai.md satisfies the raw 270 triplet T1 baseline as of 2026-05-03 (predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` raw_compliance flagged README.ai.md emit deferred / baseline grandfather candidate vs 2026-06-01 promotion-day deadline — this cycle satisfies that deadline ahead of schedule). T2/T3/T4 remain future-cycle work.
