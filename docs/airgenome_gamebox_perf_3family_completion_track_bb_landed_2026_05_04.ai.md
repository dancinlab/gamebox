# airgenome-gamebox — Track BB perf 3-family 100% completion landing (2026-05-04)

## §1. Mission paraphrase

Track BB cycle: complete 3 perf families (c_metal_* / c_pe_* / c_install_*) to
100% cumulative coverage by extending the remaining untouched modules (5 +
4 + 2 = 11 modules). Track AP had extended 6/11 c_metal_*, Track AQ had
extended 6/10 c_pe_*, Track AR had extended 4/6 c_install_*. Track BB
extends the residual untouched modules — 5 c_metal_* (private_heap_prewarm,
profiler_feedback, res_opt_sampler, shared_event_sched, simdgroup_reduce) +
4 c_pe_* (imports_growth_chart, install_coverage, parse_cache,
reloc_optimize) + 2 c_install_* (disk_space_check, progress_eta) — each
gaining +25-50 LOC of synthetic state machine round-trip rows in
`self_test()`, mirroring the Track AP/AQ/AR predecessor extension pattern.
The result: cumulative_c_metal_extended 11/11 (100%) +
cumulative_c_pe_extended 10/10 (100%) +
cumulative_c_install_extended 6/6 (100%). Three families closed.

`.roadmap.diablo2_resurrected` cond.1 evidence array gained +3 rows (Track
BB 4 c_pe_* aggregate + cumulative 10/10 row). cond.3 evidence array
gained +2 rows (Track BB 5 c_metal_* aggregate + cumulative 11/11 row).
`.roadmap.battlenet` cond.3 evidence array gained +2 rows (Track BB 2
c_install_* aggregate + cumulative 6/6 row). All cond statuses unchanged
(d2r cond.1 partial / cond.2 met / cond.3 partial; battlenet cond.1 met /
cond.2 met / cond.3 partial). Posture =
`evidence_augment_partial_to_partial`, NOT met promotion.

Cumulative Track-BB synthetic stage emit count: 22 PARTIAL stage emits
(11 modules × 2 stages each).

## §2. Cycle scope (additive_only)

| Concern               | Value                                             |
|-----------------------|---------------------------------------------------|
| modules extended      | 11 (5 c_metal_* + 4 c_pe_* + 2 c_install_*)       |
| LOC delta             | each +32-38 LOC, total +~390 LOC                  |
| roadmap edits         | 2 (.roadmap.diablo2_resurrected + .roadmap.battlenet) |
| handoff               | 1 NEW (this doc)                                  |
| marker                | 1 NEW                                             |
| cond status changes   | 0 (all preserved)                                 |
| destructive_ops       | 0                                                 |
| in_place_writes       | 13 (11 modules + 2 roadmap)                       |
| migration             | forbidden (additive_only)                         |
| cost_usd              | 0                                                 |
| substrate             | mac-local                                         |
| cap_minutes           | 90                                                |

## §3. Modules extended (11 total)

### c_metal_* — Track BB closes 5/11 untouched → cumulative 11/11 (100%)

| # | path | before LOC | after LOC | delta | emit prefix | stages |
|---|------|-----------|-----------|-------|-------------|--------|
| 1 | lib/perf/c_metal_private_heap_prewarm.hexa | 153 | 186 | +33 | `__METAL_HEAP__` | prewarm_phase / residency_class_matrix |
| 2 | lib/perf/c_metal_profiler_feedback.hexa | 187 | 220 | +33 | `__METAL_PROFILER__` | feedback_phase / shader_precision_matrix |
| 3 | lib/perf/c_metal_res_opt_sampler.hexa | 189 | 222 | +33 | `__METAL_SAMPLER__` | dedup_phase / canonical_matrix |
| 4 | lib/perf/c_metal_shared_event_sched.hexa | 141 | 175 | +34 | `__METAL_EVENT__` | event_phase / dependency_matrix |
| 5 | lib/perf/c_metal_simdgroup_reduce.hexa | 158 | 192 | +34 | `__METAL_SIMD__` | reduce_phase / eligibility_matrix |

### c_pe_* — Track BB closes 4/10 untouched → cumulative 10/10 (100%)

| # | path | before LOC | after LOC | delta | emit prefix | stages |
|---|------|-----------|-----------|-------|-------------|--------|
| 6 | lib/perf/c_pe_imports_growth_chart.hexa | 199 | 235 | +36 | `__PE_GROWTH__` | growth_phase / pe_dll_growth_matrix |
| 7 | lib/perf/c_pe_install_coverage.hexa | 266 | 303 | +37 | `__PE_COVERAGE__` | coverage_phase / coverage_class_matrix |
| 8 | lib/perf/c_pe_parse_cache.hexa | 309 | 348 | +39 | `__PE_CACHE__` | cache_phase / columnar_lookup_matrix |
| 9 | lib/perf/c_pe_reloc_optimize.hexa | 239 | 277 | +38 | `__PE_RELOC_OPT__` | optimize_phase / strategy_matrix |

### c_install_* — Track BB closes 2/6 untouched → cumulative 6/6 (100%)

| #  | path | before LOC | after LOC | delta | emit prefix | stages |
|----|------|-----------|-----------|-------|-------------|--------|
| 10 | lib/perf/c_install_disk_space_check.hexa | 139 | 178 | +39 | `__INSTALL_DISK__` | disk_check_phase / game_verdict_matrix |
| 11 | lib/perf/c_install_progress_eta.hexa | 161 | 201 | +40 | `__INSTALL_ETA__` | eta_phase / throughput_fmt_matrix |

## §4. Roadmap evidence augments

### .roadmap.diablo2_resurrected cond.1 (PE parse path)
- **before**: ~35 evidence rows (post Track AQ).
- **after**: ~38 evidence rows (Track BB +3 additive: imports_growth+install_coverage detail row + parse_cache+reloc_optimize detail row + cumulative 10/10 100% completion row).
- **status**: partial -> partial (NOT met promotion, evidence_augment_partial_to_partial posture).
- **blocker_reason refresh**: acknowledges Track BB 4 c_pe_* additional modules → cumulative_c_pe_extended_after_track_bb 10/10 100% reached, total 20 PARTIAL emit stage 누적.
- **handoff field added**: `ai_native_handoff_cond_1_perf_3family_completion_track_bb`.

### .roadmap.diablo2_resurrected cond.3 (Apple Metal layer)
- **before**: ~16 evidence rows (post Track AP).
- **after**: ~18 evidence rows (Track BB +2 additive: heap_prewarm+profiler_feedback+sampler aggregate row + shared_event+simdgroup cumulative 11/11 100% completion row).
- **status**: partial -> partial (NOT met promotion).
- **blocker_reason refresh**: acknowledges cumulative_c_metal_extended_after_track_bb 11/11 100% reached, total 22 PARTIAL emit stage 누적; D3DMetal binary licence transitive (loader_dx blk.1) gate remains permanent.
- **handoff field added**: `ai_native_handoff_cond_3_perf_3family_completion_track_bb`.

### .roadmap.battlenet cond.3 (OAuth + install path)
- **before**: ~16 evidence rows (post Track AR).
- **after**: ~18 evidence rows (Track BB +2 additive: disk_space_check+progress_eta detail row + cumulative 6/6 100% completion row).
- **status**: partial -> partial (NOT met promotion).
- **blocker_reason refresh**: acknowledges cumulative_c_install_extended_after_track_bb 6/6 100% reached.
- **handoff field added**: `ai_native_handoff_cond_3_perf_3family_completion_track_bb`.

## §5. Cross-link

c_metal_* modules (Apple-native scaffold) are counterparts to the D3DMetal
binary licence transitive blocker (loader_dx blk.1) for D2R cond.3 / D4
cond.2. c_pe_* modules cover the PE loader perf surface
(IAT/relocation/runtime layout/section/TLS/segment/imports growth/install
coverage/parse cache/reloc optimize) directly cross-linked to D2R cond.1
and D4 cond.1 PE parse paths. c_install_* modules cover the Battle.net
Setup install Phase 1 path (path detect/path resolve/progress
bar/signature verify/disk space check/progress ETA) cross-linked to
battlenet cond.3 and battlenet cond.2 setup install phase.

Track BB closes 3 perf families to 100% — this is the first cycle where
c_metal_* / c_pe_* / c_install_* simultaneously reach cumulative full
coverage on this airgenome-gamebox session.

## §6. Policy compliance

- `migration` forbidden / `additive_only` / `destructive_ops` 0 /
  `cost_usd` 0 / `substrate` mac-local / `cap_minutes` 90.
- `BR_NO_USER_VERBATIM`: 0 verbatim user quotes in any artifact.
- own1: Wine 0 / hexa-only / Apple SDK direct binding via hexa-native FFI
  scaffold OK (Apple platform component for c_metal_*) / NO Metal binary
  linkage attempt / synthetic byte-array offsets only (0x18001C000..
  0x180024000 for c_metal_*; 0x14011E000..0x140124000 for c_pe_*;
  0x180030000..0x180032000 for c_install_*) — fictitious scaffold
  pointers, NOT real RVAs. No real
  MTLHeap/MTLCounterSampleBuffer/MTLSamplerState/MTLSharedEvent/SIMD-group
  intrinsic invocation. No real PE re-parse / no real win32_*.hexa
  adapter grep / no real cache stat / no real reloc apply / no real df
  probe / no real wall clock.
- own2: 22 PARTIAL emit explicit (11 modules × 2 stage = 22 println markers
  + 22 r0_emit round-trip events with verdict PARTIAL + posture
  evidence_augment_partial_to_partial). PASS swallow forbidden (cond
  statuses partial 영구 유지 across the 3 cond.* augmented).

## §7. Caveats (≥10)

- **C1**: cond.1+cond.3 met 승격 영구 X — D2R Battle.net launcher install
  dependency is the permanent external gate for cond.1 production smoke;
  D3DMetal binary licence transitive (loader_dx blk.1) is the permanent
  external gate for cond.3 met. Track BB extension is evidence_augment
  only, NOT met promotion path.
- **C2**: battlenet cond.3 met 승격 영구 X — production smoke gated on
  user network smoke + 자연 환경 정합 long-horizon work. Track BB
  extension is evidence_augment only.
- **C3**: c_metal_* synthetic byte-array offsets (0x18001C000..
  0x180024000) are fictitious scaffold pointers. Distinct from c_pe_*
  synthetic offsets (0x14011E000..0x140124000) and c_install_* synthetic
  offsets (0x180030000..0x180032000).
- **C4**: append goes between emit-count assertion and `cleanup_*()`
  gate (or just before final println for c_pe_parse_cache /
  c_pe_reloc_optimize where assertion ordering differs) — additive_only
  spirit preserved (no row removal, no self_test body modification, no
  main() change).
- **C5**: hexa-lang upstream candidates: 0 hits discovered in this cycle.
  Each module already uses hexa-native primitives. No new gap surfaced.
  `docs/hexa_upstream_track_bb_*.md` NOT created (per user 2026-05-04
  directive — no entry if no hits).
- **C6**: in_place_writes=13 (11 c_*.hexa + 2 .roadmap.* JSONL header
  surgical edits). additive_only spirit preserved across all artifacts.
- **C7**: Track BB territory ban respected: c_d2r_* / c_d4_* / c_purple_*
  / c_bnet_* / c_lineage_* / c_oauth_* / c_elden_* / c_cs2_* / lib/loader
  / tool / native / tests / Track AP-extended c_metal_* (6) / Track
  AQ-extended c_pe_* (6) / Track AR-extended c_install_* (4) untouched.
  Parallel BG (BC) territory respected.
- **C8**: docs/HEXA_UPSTREAM.md NOT modified (Track BC concurrent
  territory — race condition risk per user directive).
- **C9**: cumulative_c_metal_extended_after_track_bb = 11/11 (100%);
  cumulative_c_pe_extended_after_track_bb = 10/10 (100%);
  cumulative_c_install_extended_after_track_bb = 6/6 (100%). Three
  families closed simultaneously.
- **C10**: emit_count assertion thresholds unchanged in each module —
  append occurs after assertion, so extra 2 emits per module are bonus
  (actual counts will exceed thresholds). Existing self_test body
  verification 0 line modify.
- **C11**: c_pe_parse_cache append point is BEFORE final println (after
  emit-count assertion + cleanup index step) to preserve the existing
  cache-index cleanup order. Additive position differs from other
  modules but `additive_only` spirit preserved.
- **C12**: roadmap surgical edits keep cond.2 (met) untouched verbatim
  for both roadmaps. cond.1+cond.3 evidence array additive with full
  blocker_reason refresh. status fields unchanged across all 6 cond
  rows (3 in d2r + 3 in battlenet).

## §8. Predecessors

- `lib/perf/c_d2r_first_frame_trace.hexa` (Track G) — append pattern reference.
- `lib/perf/c_d2r_act_progression.hexa` (Track AI) — append pattern reference.
- `lib/perf/c_metal_capture_auto.hexa` (Track AP) — Apple-native synthetic round-trip pattern.
- `lib/perf/c_pe_iat_simulation.hexa` (Track AQ) — PE loader synthetic round-trip pattern.
- `lib/perf/c_install_path_detect.hexa` (Track AR) — install path synthetic round-trip pattern.
- `state/markers/airgenome_gamebox_perf_game_d2r_extension_track_ai_landed.marker` (Track AI) — schema parent.
- `docs/airgenome_gamebox_perf_metal_extension_track_ap_landed_2026_05_04.ai.md` (Track AP) — c_metal_* family extension precedent.
- `docs/airgenome_gamebox_perf_pe_extension_track_aq_landed_2026_05_04.ai.md` (Track AQ) — c_pe_* family extension precedent.
- `docs/airgenome_gamebox_perf_oauth_install_extension_track_ar_landed_2026_05_04.ai.md` (Track AR) — c_install_* family extension precedent.

## §9. Next-cycle actions

- Track BC — hexa-lang upstream surface aggregation cycle (parallel to BB).
- Track BD — extend remaining perf families that have not yet reached 100%
  cumulative completion (c_d4_* / c_oauth_* / c_purple_* / c_bnet_* /
  c_lineage_* / c_elden_* / c_cs2_*).
- Production smoke remains gated on:
  - Real D2R.exe binary (Battle.net launcher install dependent — permanent).
  - D3DMetal binary licence (permanent external dependency, not Track-AZ-resolvable from within own1).
  - User network endpoint smoke (battlenet cond.3, multi-cycle long-horizon).
