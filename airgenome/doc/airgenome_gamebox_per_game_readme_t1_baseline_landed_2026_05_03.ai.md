---
schema: airgenome-gamebox/airgenome/doc/airgenome_gamebox_per_game_readme_t1_baseline_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker
  readme_files_new:
    - docs/per_game/diablo2_resurrected_README.ai.md
    - docs/per_game/diablo4_README.ai.md
    - docs/per_game/wow_README.ai.md
    - docs/per_game/cs2_README.ai.md
    - docs/per_game/delta_force_README.ai.md
    - docs/per_game/elden_ring_README.ai.md
    - docs/per_game/lineage_m_README.ai.md
    - docs/per_game/lineage_w_README.ai.md
    - docs/per_game/lineage_classic_r28_README.ai.md
    - docs/per_game/battlenet_README.ai.md
    - docs/per_game/purple_launcher_README.ai.md
  predecessor_handoffs:
    - airgenome/doc/airgenome_gamebox_per_game_roadmaps_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_d4_retired_intentional_adoption_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_4game_retired_intentional_adoption_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_purple_lineage_offline_shim_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_gameguard_honest_freeze_landed_2026_05_03.ai.md
status: LANDED_PER_GAME_README_T1_BASELINE_11
related_raws:
  - raw 9    # hexa-only orchestration (audit-only, no impl)
  - raw 10   # honest C3 caveats inline
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 15   # env() lazy + repo-relative
  - raw 175  # BR-NO-USER-VERBATIM
  - raw 270  # ai-native readme triplet T1 baseline (THIS CYCLE — ahead of 2026-06-01 promotion-day deadline)
  - raw 271  # core+module pattern (per-game sub-bucket)
  - raw 272  # lint extension (T2 future cycle)
  - raw 273  # hierarchy connection direction (T3/T4 future cycles)
preserved_unchanged:
  - 11 .roadmap.<game>/<launcher> (read-only, 0 in-place writes)
  - 3 .roadmap.<domain> (loader_pe, loader_win32, loader_dx)
  - all existing docs/*.md (mk1 narrative + 7 prior handoffs read-only)
  - all hexa modules under lib/{loader,perf}
  - all native sources, tests, tool/bench, plugin.json, entry, README.md, .gitignore
  - 7 predecessor markers
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 0
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# airgenome-gamebox per-game README.ai.md T1 baseline landed (Track F — raw 270 triplet T1)

## §1 user_directive_paraphrase

BG subagent Track F — land 11 per-game/launcher `README.ai.md` sub-bucket files under new `docs/per_game/` directory satisfying raw 270 triplet T1 baseline. Predecessor `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` raw_compliance.raw_270_271_272_273_triplet field flagged README.ai.md emit deferred / baseline grandfather candidate vs 2026-06-01 promotion-day deadline. This cycle satisfies that deadline ahead of schedule. 11 files = 1 file per existing .roadmap (10 game + 1 purple_launcher + 1 battlenet → 9 game + 2 launcher = 11). Each README ~50-150 LOC, 8 sections (identity / cond status / blocker / module count / cross_link / mk1 narrative / cycle history / T1 baseline note). All file contents derived from .roadmap.* JSONL header read at land-time (authoritative status, no guessing). Additive only, migration forbidden, $0 cost, destructive 0, cap 90min, BR-NO-USER-VERBATIM, friendly preset, silent land. own1 Wine 0 + own2 honest emit posture preserved (no AC/DRM bypass discussion in any README).

## §2 11-file land table

| path | LOC | sha256 |
|---|---:|---|
| docs/per_game/diablo2_resurrected_README.ai.md | 69 | `0dca305981db1703677266debbe019811c3925d4dc79ed06d1e32b2605f57a49` |
| docs/per_game/diablo4_README.ai.md | 73 | `e70f73f6c3de9188c709d744fc19e7550bb1b0c6044a14d653d837e372e057df` |
| docs/per_game/wow_README.ai.md | 71 | `2f0ad127770cfea97b9db3ef2a665978055e53890ad1b0ddf03b36e71a2253f6` |
| docs/per_game/cs2_README.ai.md | 75 | `af010341ae3082b35a133af79cc2993d8109550a24a88c3cd602b27e9c357196` |
| docs/per_game/delta_force_README.ai.md | 75 | `d5da1436f817b72c87308d36a29aad4bf8c4dd0e6b5cd47d9f347192f06261cc` |
| docs/per_game/elden_ring_README.ai.md | 75 | `a0f4c043919360010c564f66623d5b015d8e5a908142a8814e2fe549da5102da` |
| docs/per_game/lineage_m_README.ai.md | 68 | `0014854fb7d4dd485fd14ad20f59194699a460745d407f94609063377e4e850b` |
| docs/per_game/lineage_w_README.ai.md | 68 | `1e337d4add45420c19ab6eb64371fdcbd5637079451f7bfae4ac597ca29be90b` |
| docs/per_game/lineage_classic_r28_README.ai.md | 72 | `7e749e9d6ee8268d8d0edae7e3e41dae83ab98f77b384e42cc7949aebca091ff` |
| docs/per_game/battlenet_README.ai.md | 74 | `4cfc0431e4da0c2f7c746e62b6186ac507fa7b6caa427a98756d625ab5c1677f` |
| docs/per_game/purple_launcher_README.ai.md | 72 | `1b7188667ecd6683d1a2263b0229ae5378b0c5a966c9d52de87cc0b08321f7e2` |
| airgenome/doc/airgenome_gamebox_per_game_readme_t1_baseline_landed_2026_05_03.ai.md | (this doc) | (set after write) |

LOC range = 68..75 (all within 50-150 target).

## §3 cross-cycle reference matrix

| game/launcher | per_game_roadmaps | battlenet_d2r_shim | d2r_cond1_synthetic | d4_retired_intentional | 4game_retired_intentional | purple_lineage_shim | gameguard_honest_freeze |
|---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| diablo2_resurrected | yes | yes (cond.2 met) | yes (cond.1 evidence) | (peer ref) | — | — | — |
| diablo4 | yes | (peer ref) | — | yes (Track A2 first-mover) | (precedent ref) | — | — |
| wow | yes | (peer ref) | — | (peer precedent) | yes (6th lock-in) | — | — |
| cs2 | yes | — | — | (peer precedent) | yes (Track C adopted) | — | — |
| delta_force | yes | — | — | (peer precedent) | yes (Track C adopted) | — | — |
| elden_ring | yes | — | — | (peer precedent) | yes (Track C adopted) | — | — |
| lineage_m | yes | — | — | — | — | yes (cond.1 cascading met) | — |
| lineage_w | yes | — | — | — | — | yes (cond.1 cascading met + new loader) | — |
| lineage_classic_r28 | yes | — | — | — | — | yes (Track-B retired_intentional) | yes (Track-E cond.3 evidence) |
| battlenet | yes (+self_mk2 + rank_a_3) | yes (cond.1+cond.2 met) | — | — | — | — | — |
| purple_launcher | yes | — | — | — | — | yes (cond.1+cond.2 met) | — |

## §4 raw 270 triplet T1 baseline confirmation

deadline: 2026-06-01 (promotion-day)
landed: 2026-05-03 (29 days ahead of schedule)

raw 270 triplet tier mapping:
- T1 (this cycle) — ai-native README.ai.md baseline per game/launcher (50-150 LOC, 8 standard sections)
- T2 (future) — lint extension (raw 272) — automated section/LOC/cross-link checks
- T3 (future) — hierarchy connection direction (raw 273) — game→loader→native dependency edges formalized
- T4 (future) — full module-by-module spec freeze (per-loader / per-perf module README.ai.md inside lib/{loader,perf}/)

Sub-bucket structure: `docs/per_game/<name>_README.ai.md` (NEW directory, 11 files). Each file derives §2-§4 content from authoritative `.roadmap.<name>` JSONL header read at land-time (no guessing, honest reflect of status partial / met / retired_intentional posture).

## §5 honest C3 caveats (8 inline)

C1 — README.ai.md content extracted from 11 .roadmap.* files at 2026-05-03 read-time. .roadmap.* may be subsequently mutated (verifier execution / status partial → met transition). Future T2 lint cycle should add freshness check (last_updated vs roadmap mtime).

C2 — `release_date` field present only for `lineage_classic_r28` (2026-02-07), `lineage_m` (2017), `lineage_w` (2021) per roadmap header. Other 8 games/launchers `release_date` honest stated as "(not pinned in roadmap header)". No external lookup performed (raw 15 env_lazy compliance).

C3 — §6 mk1 narrative pointer: 6 games have landed manjeom/honest freeze docs (`docs/{D4,CS2,DELTA_FORCE,ELDEN_RING,WOW,LINEAGE_CLASSIC_R28}_MANJEOM_REVIEW.md` + `docs/GAMEGUARD_HONEST_FREEZE.md`). 5 games/launchers (`diablo2_resurrected`, `lineage_m`, `lineage_w`, `battlenet`, `purple_launcher`) honest stated as "manjeom review pending separate cycle" — no fabricated reference.

C4 — §7 cycle history extracted via marker grep at 2026-05-03 land-time. Markers without semantic relevance (mere mention in another marker's body) filtered to direct cycle-relevant references only. `state/markers/test_own2_*.marker` + `pe_d2r_mpq_archive_load_*.marker` excluded as ephemeral test/log markers.

C5 — own1 Wine 0 + own2 honest emit posture preserved across all 11 README files. No AC/DRM/anti-tamper bypass discussion. retired_intentional posture for 6 games (diablo4, wow, cs2, delta_force, elden_ring, lineage_classic_r28) reflected as `block_acknowledged_pass_equivalent` only — honest documentation, no evasion.

C6 — File scope strictly additive: 11 NEW README + 1 NEW handoff + 1 NEW marker = 13 NEW files, 0 in-place writes, 0 destructive ops. `.roadmap.*` files read-only, `lib/`, `tests/`, `native/`, `tool/` untouched. Predecessor docs/*.md read-only reference only.

C7 — BR-NO-USER-VERBATIM (raw 175) compliance: 0 verbatim Korean user prompt quotes in any of 13 created files. §1 user_directive_paraphrase paraphrased to English structural summary.

C8 — Marker JSON field naming: snake_case throughout per raw 11. `files_created` array mirrors predecessor `state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker` schema (path / type / loc / bytes / sha256).

## §6 next-cycle hooks (T2/T3/T4 of raw 270 triplet)

T2 — lint extension cycle (raw 272):
- automated check: each `docs/per_game/<name>_README.ai.md` 50..150 LOC range
- automated check: §2 cond status matches `.roadmap.<name>` cond status (parse JSONL header, diff)
- automated check: §3 blocker.status_enum_adopted matches roadmap blocker field
- automated check: §5 sister_roadmaps array element-equal to roadmap cross_link.sister_roadmaps
- automated check: §6 mk1 narrative path exists when not "pending separate cycle"

T3 — hierarchy connection direction cycle (raw 273):
- emit `docs/per_game/_HIERARCHY.ai.md` summary diagram (game → loader_pe → loader_win32 → loader_dx → native)
- formalize downstream/upstream dependency edges per game

T4 — full module-by-module spec freeze cycle:
- per-loader README.ai.md inside `lib/loader/<module>_README.ai.md` (414 modules)
- per-perf README.ai.md inside `lib/perf/<module>_README.ai.md` (241 modules)
- aggregate index doc `docs/per_module/_INDEX.ai.md`

future per-game cycle (orthogonal to T1-T4):
- 5 remaining mk1 narrative manjeom land (diablo2_resurrected, lineage_m, lineage_w, battlenet, purple_launcher)
- 3 anti-cheat honest freeze docs (VAC_HONEST_FREEZE.md / EAC_KERNEL_HONEST_FREEZE.md / EAC_ANTITAMPER_HONEST_FREEZE.md)
- verifier execution + cond.N status partial → met transitions per per-game blockers

## §7 file index (sha-pin at land time)

| path | type | bytes | LOC | sha256_hex |
|---|---|---:|---:|---|
| docs/per_game/diablo2_resurrected_README.ai.md | per_game_readme_t1 | 3284 | 69 | `0dca305981db1703677266debbe019811c3925d4dc79ed06d1e32b2605f57a49` |
| docs/per_game/diablo4_README.ai.md | per_game_readme_t1 | 3412 | 73 | `e70f73f6c3de9188c709d744fc19e7550bb1b0c6044a14d653d837e372e057df` |
| docs/per_game/wow_README.ai.md | per_game_readme_t1 | 3369 | 71 | `2f0ad127770cfea97b9db3ef2a665978055e53890ad1b0ddf03b36e71a2253f6` |
| docs/per_game/cs2_README.ai.md | per_game_readme_t1 | 3305 | 75 | `af010341ae3082b35a133af79cc2993d8109550a24a88c3cd602b27e9c357196` |
| docs/per_game/delta_force_README.ai.md | per_game_readme_t1 | 3451 | 75 | `d5da1436f817b72c87308d36a29aad4bf8c4dd0e6b5cd47d9f347192f06261cc` |
| docs/per_game/elden_ring_README.ai.md | per_game_readme_t1 | 3631 | 75 | `a0f4c043919360010c564f66623d5b015d8e5a908142a8814e2fe549da5102da` |
| docs/per_game/lineage_m_README.ai.md | per_game_readme_t1 | 3075 | 68 | `0014854fb7d4dd485fd14ad20f59194699a460745d407f94609063377e4e850b` |
| docs/per_game/lineage_w_README.ai.md | per_game_readme_t1 | 3268 | 68 | `1e337d4add45420c19ab6eb64371fdcbd5637079451f7bfae4ac597ca29be90b` |
| docs/per_game/lineage_classic_r28_README.ai.md | per_game_readme_t1 | 3823 | 72 | `7e749e9d6ee8268d8d0edae7e3e41dae83ab98f77b384e42cc7949aebca091ff` |
| docs/per_game/battlenet_README.ai.md | per_launcher_readme_t1 | 3615 | 74 | `4cfc0431e4da0c2f7c746e62b6186ac507fa7b6caa427a98756d625ab5c1677f` |
| docs/per_game/purple_launcher_README.ai.md | per_launcher_readme_t1 | 3557 | 72 | `1b7188667ecd6683d1a2263b0229ae5378b0c5a966c9d52de87cc0b08321f7e2` |
| airgenome/doc/airgenome_gamebox_per_game_readme_t1_baseline_landed_2026_05_03.ai.md | handoff_doc | TBD | TBD | (set after write) |
| state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker | marker | TBD | TBD | (self-referential) |

## §8 policy summary

- migration: forbidden — 0 emit
- additive only — 13 NEW files (11 README + 1 handoff + 1 marker), 0 existing files modified
- destructive ops — 0
- in-place writes — 0
- substrate — mac-local
- cost — $0
- cap — 90min (실제 소요 ≪ cap)
- raw 9 hexa-only orchestration — doc-only land (raw 168 minimum-viable exempt)
- raw 10 honest C3 — 8 caveats inline §5
- raw 11 snake_case — all marker JSON fields snake_case
- raw 12 silent-error ban — every blocker state explicit (status / status_enum_candidate / status_enum_adopted)
- raw 15 env_lazy — repo-relative paths only, no `<user>` absolute path inserts
- raw 175 BR-NO-USER-VERBATIM — 0 verbatim Korean user prompt quotes in any of 13 created files
- raw 270 triplet T1 — landed (this cycle, 2026-05-03) ahead of 2026-06-01 deadline
- friendly preset — handoff doc only
- silent land — marker emit, no user response
