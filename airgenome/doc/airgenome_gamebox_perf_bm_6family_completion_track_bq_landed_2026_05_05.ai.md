# airgenome-gamebox · Perf BM 6-Family Completion · Track BQ landed (2026-05-05)

## Mission

Track BQ closes the six perf families that Track BM seeded (one module each across
`c_sote_/c_steam_/c_wot_/c_wows_/c_iat_/c_dll_`) to **100% cumulative completion**.
Track BM extended 7 modules across 6 previously-untouched families for breadth, NOT
family-completion. Track BQ extends every remaining untouched module in those 6
families with the same synthetic 4-phase pipeline + 16-slot matrix round-trip pattern,
emitting two PARTIAL stages per module. Status preservation = partial across the
roadmap touch points; posture = `evidence_augment_partial_to_partial`.

| metric | value |
| --- | --- |
| track | BQ |
| iso landed | 2026-05-05 |
| modules extended this cycle | 5 |
| families touched | 6 (c_sote_/c_steam_/c_wot_/c_wows_/c_iat_/c_dll_ — 5 modules; c_sote_+c_steam_ already 100% post-BM, no module re-touch) |
| LOC delta total | +173 |
| PARTIAL emit stage count | 10 |
| .roadmap rows added | 4 |
| destructive ops | 0 |
| cost (USD) | 0 |
| substrate | mac-local |
| cap minutes | 90 |

## Scope held

| boundary | held |
| --- | --- |
| Track BM 7 modules read-only | yes (no re-touch on c_sote_dlc_content_probe / c_sote_realm_of_shadow_zone_load / c_steam_drm_stub_track / c_wot_garage_warmup / c_wows_port_warmup / c_iat_resolve_cache / c_dll_load_predict) |
| 18 other already-100% families untouched | yes |
| `lib/loader/*` untouched (Track BP parallel) | yes |
| `tool/`, `native/`, `tests/` untouched | yes |
| existing `docs/*.md` untouched | yes (new handoff only) |
| `docs/HEXA_UPSTREAM.md` untouched | yes |
| `blk.1` fields untouched | yes |

## Modules extended (5)

| # | path | family | before | after | delta | emit prefix | stages |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | lib/perf/c_wot_training_room_profile.hexa | c_wot_* | 197 | 232 | +35 | `__WOT_TRAINING__` | training_profile_phase / training_matrix |
| 2 | lib/perf/c_wows_replay_playback.hexa | c_wows_* | 188 | 223 | +35 | `__WOWS_REPLAY__` | replay_playback_phase / replay_matrix |
| 3 | lib/perf/c_iat_runtime_bind_track.hexa | c_iat_* | 132 | 167 | +35 | `__IAT_RUNTIME__` | iat_bind_phase / bind_matrix |
| 4 | lib/perf/c_dll_adapter_coverage_track.hexa | c_dll_* | 143 | 177 | +34 | `__DLL_ADAPTER__` | adapter_coverage_phase / coverage_matrix |
| 5 | lib/perf/c_dll_coverage_growth_track.hexa | c_dll_* | 219 | 253 | +34 | `__DLL_GROWTH__` | coverage_growth_phase / growth_matrix |

Each append block follows the BM template: comment header documenting the synthetic
4-phase pipeline + 16-slot matrix, then two `r0_emit` calls each followed by a
`println("__<DOMAIN>__ PARTIAL <stage>")` PARTIAL evidence emit. Insertion is in
`self_test()` immediately before the trailing `cleanup_*()` call so the existing
assertion ladder remains the source of truth and the new emits are pure additive
PARTIAL evidence (status not promoted to PASS, no swallow).

## Roadmap edits (4 rows)

| path | cond_id | rows before | rows after | status before | status after | posture |
| --- | --- | --- | --- | --- | --- | --- |
| .roadmap.elden_ring | elden_ring.cond.3 | 11 | 12 | partial | partial | evidence_augment_partial_to_partial |
| .roadmap.cs2 | cs2.cond.1 | 7 | 8 | partial | partial | evidence_augment_partial_to_partial |
| .roadmap.battlenet | battlenet.cond.3 | 22 | 23 | partial | partial | evidence_augment_partial_to_partial |
| .roadmap.diablo2_resurrected | diablo2_resurrected.cond.3 | 27 | 28 | partial | partial | evidence_augment_partial_to_partial |

`elden_ring.cond.3` and `cs2.cond.1` rows reaffirm the c_sote_*/c_steam_* families
that already hit 100% in Track BM (no module re-touch this cycle — narrative
cross-link only). `battlenet.cond.3` and `diablo2_resurrected.cond.3` rows record
the actual c_wot_*/c_wows_* and c_iat_*/c_dll_* family closures landed this cycle.

Handoff field added to each touched cond:
`ai_native_handoff_cond_<n>_perf_bm_6family_completion_track_bq` =
`airgenome/doc/airgenome_gamebox_perf_bm_6family_completion_track_bq_landed_2026_05_05.ai.md`.

## Cumulative family completion (post-Track-BQ)

| family | extended in BM | extended in BQ | family total | cumulative pct |
| --- | --- | --- | --- | --- |
| c_sote_* | 2 | 0 (already 100%) | 2 | 100% |
| c_steam_* | 1 | 0 (already 100%) | 1 | 100% |
| c_wot_* | 1 | 1 (training_room_profile) | 2 | 100% |
| c_wows_* | 1 | 1 (replay_playback) | 2 | 100% |
| c_iat_* | 1 | 1 (runtime_bind_track) | 2 | 100% |
| c_dll_* | 1 | 2 (adapter_coverage_track + coverage_growth_track) | 3 | 100% |

All 6 families that Track BM opened are now closed at 100% cumulative completion.

## Policy

| clause | value |
| --- | --- |
| migration | forbidden |
| additive only | true |
| destructive ops | 0 |
| cost (USD) | 0 |
| substrate | mac-local |
| cap (min) | 90 |
| BR_NO_USER_VERBATIM | true |
| friendly preset | true |
| silent land | true |
| own1 wine count | 0 |
| own1 hexa-only | true |
| own1 synthetic only | true |
| own2 PARTIAL emit explicit | 10 |
| own2 status inflated | false |
| own2 pass swallow | false |
| raw clauses | 9 / 11 / 12 / 15 / 175 |

## Verification

| check | result |
| --- | --- |
| all 5 untouched modules show LOC growth | yes (+34..+35 each, total +173) |
| all roadmap files parse as JSONL | yes |
| marker JSON parses with cumulative completion fields | yes |
| sha256 pinned in marker | yes |
| no Track BM 7 modules re-touched | yes |
| no other-family / loader / tool / native / tests touched | yes |
| no existing docs/*.md modified | yes |

## Predecessors

- airgenome/doc/airgenome_gamebox_perf_remaining_family_inventory_v2_track_bm_landed_2026_05_05.ai.md (BM — 6 families opened)
- airgenome/doc/airgenome_gamebox_perf_3family_completion_track_bj_landed_2026_05_05.ai.md
- airgenome/doc/airgenome_gamebox_perf_4family_completion_track_bi_landed_2026_05_05.ai.md
- airgenome/doc/airgenome_gamebox_perf_remaining_family_inventory_track_bf_landed_2026_05_05.ai.md

## Next-cycle actions

- Track BR+ — close any other previously-touched-but-not-100% families to cumulative 100%.
- Track BS+ — augment `.roadmap.loader_pe` directly for c_iat_*/c_dll_*/c_wot_*/c_wows_* PE-loader transitive evidence rows (instead of going through battlenet/d2r cross-link).
- Long-horizon — Wargaming production smoke gate (BigWorld engine binary licence) and Phase 1 PE x64 loader closure remain blocked by external dependencies.
