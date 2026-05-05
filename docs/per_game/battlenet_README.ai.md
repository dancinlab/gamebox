---
schema: airgenome-gamebox/docs/per_game/battlenet_readme/ai-native/1
last_updated: 2026-05-03
roadmap_ssot: .roadmap.battlenet
status: T1_BASELINE_LANDED_2026_05_03
related_raws:
  - raw 270  # ai-native readme triplet T1 baseline
  - raw 271  # core+module pattern (per-launcher sub-bucket)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 175  # BR-NO-USER-VERBATIM
---

# battlenet README.ai.md (T1 baseline)

## §1 launcher identity

| field | value |
|---|---|
| name | battlenet |
| alias | battle_net, bnet |
| kind | launcher |
| release_date | (not pinned in roadmap header — Blizzard launcher shell, ongoing) |

## §2 cond status snapshot (read from .roadmap.battlenet at land-time)

| cond | desc | status |
|---|---|---|
| battlenet.cond.1 | Battle.net Agent SD forging + REST endpoint round-trip (install/launch/uninstall) | met (offline shim PASS) |
| battlenet.cond.2 | setup install phase + install path resolve real (not stub) — 3 phase resolve/stage/write_manifest | met (offline shim PASS) |
| battlenet.cond.3 | OAuth 2-leg token + chat/login state track live | partial (network-dependent, online endpoint transitive blk.1) |

## §3 blocker snapshot

| blocker | type | status | status_enum_candidate | status_enum_adopted |
|---|---|---|---|---|
| battlenet.blk.1 | upstream_network (online endpoint network dependency) | open | none | (not adopted — offline-mode shim cond.1+cond.2 path met, network condition cond.3 met transition possible) |

## §4 module count (from cross_link block)

- loader_count: 8 (`lib/loader/(battlenet_bypass + pe_battle_net_* 4 + pe_battlenet_* 4)`)
- perf_count: 3 (`lib/perf/c_bnet_*.hexa`: chat_state + login_state_track + token_refresh_track)

## §5 cross_link to sister roadmaps + manjeom_review docs

sister_roadmaps:
- `.roadmap.loader_pe`
- `.roadmap.loader_win32`
- `.roadmap.diablo2_resurrected`
- `.roadmap.diablo4`
- `.roadmap.wow`

downstream_games: diablo2_resurrected (offline mode optional), diablo4 (always-online required), wow (retail+classic)

mk1_narrative_refs:
- `docs/BATTLENET_REAL_LAUNCH_PLAN.md`
- `docs/BATTLENET_DAY1_PLAN.md`
- `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md`

## §6 mk1 narrative pointer

manjeom review pending separate cycle — Battle.net-specific manjeom doc not yet landed; cross-references via `docs/BATTLENET_REAL_LAUNCH_PLAN.md` + `docs/BATTLENET_DAY1_PLAN.md` planning docs (predecessor REUSE) + Track-A Battle.net+D2R offline-shim cycle handoff `docs/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md`.

## §7 cycle history (markers referencing this launcher)

- `state/markers/airgenome_gamebox_self_mk2_tuning_landed.marker` (predecessor mk2 tuning 2026-05-02)
- `state/markers/airgenome_gamebox_rank_a_3_domain_landed.marker` (rank A 3-domain land 2026-05-03 — Battle.net-Setup.exe cross-ref)
- `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (initial 11-roadmap fan-out 2026-05-03)
- `state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker` (Track-A 2026-05-03 — cond.1+cond.2 offline shim met)
- `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker` (this cycle 2026-05-03 — T1 README baseline)

## §8 raw 270 triplet T1 baseline note

This README.ai.md satisfies the raw 270 triplet T1 baseline as of 2026-05-03 (predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` raw_compliance flagged README.ai.md emit deferred / baseline grandfather candidate vs 2026-06-01 promotion-day deadline — this cycle satisfies that deadline ahead of schedule). T2/T3/T4 remain future-cycle work.
