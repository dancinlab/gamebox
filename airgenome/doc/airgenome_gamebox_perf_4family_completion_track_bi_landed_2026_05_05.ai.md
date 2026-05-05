# airgenome-gamebox — Track BI perf 4-family completion cycle landing (2026-05-05)

## §1. Mission paraphrase

Track BI cycle: close 4 perf module families to 100% (c_macos_/c_fps_/
c_dxr_+c_rt_/c_dxil_) by extending the 12 untouched modules across these
4 families (Track BF previously landed 1 representative per family — 4/16
total, BI lands the remaining 12, achieving 16/16 = 100% across all 4
families). Each module gains +34-36 LOC of synthetic state machine
round-trip rows in `self_test()`, mirroring the Track AP/AQ/AR/BB/BF
predecessor extension pattern. The result: 12 additional modules from
these 4 families now carry `__<DOMAIN>__ PARTIAL <stage>` emit markers
spanning 24 stages (12 modules × 2 stages each).

`.roadmap.diablo2_resurrected` cond.3 evidence array gained +4 rows
(Track BI c_macos_/c_fps_/c_dxil_ aggregate detail rows + cumulative
4-family completion row). `.roadmap.diablo4` cond.2 evidence array
gained +3 rows (Track BI c_dxr_/c_rt_ aggregate detail row + cumulative
completion row + Track BF deferred-annotation close-out row). All cond
statuses unchanged (d2r cond.1 partial / cond.2 met / cond.3 partial;
d4 cond.1 partial / cond.2 partial / cond.3 partial). Posture =
`evidence_augment_partial_to_partial`, NOT met promotion.

Cumulative Track-BI synthetic stage emit count: 24 PARTIAL stage emits
(12 modules × 2 stages each).

## §2. Cycle scope (additive_only)

| Concern               | Value                                             |
|-----------------------|---------------------------------------------------|
| modules extended      | 12 (3 per family × 4 families)                    |
| families touched      | 4 (c_macos_/c_fps_/c_dxr_+c_rt_/c_dxil_)          |
| LOC delta             | +34-36 each, total +421 LOC                       |
| roadmap edits         | 2 (.roadmap.diablo2_resurrected + .roadmap.diablo4) |
| handoff               | 1 NEW (this doc)                                  |
| marker                | 1 NEW                                             |
| cond status changes   | 0 (all preserved)                                 |
| destructive_ops       | 0                                                 |
| in_place_writes       | 14 (12 modules + 2 roadmap)                       |
| migration             | forbidden (additive_only)                         |
| cost_usd              | 0                                                 |
| substrate             | mac-local                                         |
| cap_minutes           | 90                                                |

## §3. Modules extended (12 total)

### c_macos_* family (3 — completes 4/4 with Track BF 1)

| # | path | before LOC | after LOC | delta | emit prefix | stages |
|---|------|-----------|-----------|-------|-------------|--------|
| 1 | lib/perf/c_macos_menubar_display_mode_submenu.hexa | 170 | 204 | +34 | `__MACOS_SUBMENU__` | submenu_open_phase / mode_switch_matrix |
| 2 | lib/perf/c_macos_menubar_fps_toggle_item.hexa | 173 | 207 | +34 | `__MACOS_FPS_TOGGLE__` | fps_toggle_phase / toggle_state_matrix |
| 3 | lib/perf/c_macos_menubar_pip_focus_item.hexa | 171 | 207 | +36 | `__MACOS_PIP__` | pip_focus_phase / focus_swap_matrix |

### c_fps_* family (3 — completes 4/4 with Track BF 1)

| # | path | before LOC | after LOC | delta | emit prefix | stages |
|---|------|-----------|-----------|-------|-------------|--------|
| 4 | lib/perf/c_fps_hotkey_handler.hexa | 182 | 217 | +35 | `__FPS_HOTKEY__` | hotkey_dispatch_phase / source_action_matrix |
| 5 | lib/perf/c_fps_metric_aggregator.hexa | 189 | 225 | +36 | `__FPS_METRIC__` | metric_aggregation_phase / percentile_matrix |
| 6 | lib/perf/c_fps_overlay_metal_compose.hexa | 164 | 200 | +36 | `__FPS_OVERLAY__` | overlay_compose_phase / resolution_format_matrix |

### c_dxr_* + c_rt_* family (3 — completes 4/4 with Track BF 1 c_dxr_inline_rt_shader)

| # | path | before LOC | after LOC | delta | emit prefix | stages |
|---|------|-----------|-----------|-------|-------------|--------|
| 7 | lib/perf/c_dxr_blas_refit_track.hexa | 136 | 171 | +35 | `__DXR_REFIT__` | blas_refit_phase / primitive_decision_matrix |
| 8 | lib/perf/c_dxr_ray_coherence.hexa | 176 | 211 | +35 | `__DXR_COHERENCE__` | ray_coherence_phase / dispatch_decision_matrix |
| 9 | lib/perf/c_rt_as_compaction.hexa | 151 | 186 | +35 | `__RT_COMPACT__` | as_compaction_phase / ratio_kind_matrix |

### c_dxil_* family (3 — completes 4/4 with Track BF 1)

| # | path | before LOC | after LOC | delta | emit prefix | stages |
|---|------|-----------|-----------|-------|-------------|--------|
| 10 | lib/perf/c_dxil_delta.hexa | 188 | 223 | +35 | `__DXIL_DELTA__` | dxil_delta_phase / version_delta_matrix |
| 11 | lib/perf/c_dxil_persist.hexa | 210 | 245 | +35 | `__DXIL_PERSIST__` | dxil_persist_phase / gptk_cache_matrix |
| 12 | lib/perf/c_dxil_vector_scalarize.hexa | 186 | 221 | +35 | `__DXIL_SCALARIZE__` | dxil_scalarize_phase / vec_width_matrix |

12 modules × 2 stages = 24 PARTIAL emit stage 누적.

## §4. Roadmap evidence augments

### .roadmap.diablo2_resurrected cond.3 (Apple Metal layer)

- **before**: 20 evidence rows (post Track BF).
- **after**: 24 evidence rows (Track BI +4 additive: c_macos_ 3-module
  detail row + c_fps_ 3-module detail row + c_dxil_ 3-module detail row +
  cumulative 3-family completion row).
- **status**: partial -> partial (NOT met promotion,
  evidence_augment_partial_to_partial posture).
- **blocker_reason refresh**: appends Track BI 4-family completion clause
  acknowledging 9 of 12 cross-linked d2r cond.3 Apple Metal layer scaffolding
  (c_macos_ 3 + c_fps_ 3 + c_dxil_ 3) × 2 stage = 18 PARTIAL emit stage
  cumulative cond.3 stage 측 42+ scaffold; D3DMetal binary licence
  transitive (loader_dx blk.1) gate remains permanent.
- **handoff field added**: `ai_native_handoff_cond_3_perf_4family_completion_track_bi`.

### .roadmap.diablo4 cond.2 (DXR raytracing first-frame)

- **before**: 5 evidence rows (pre-Track-BI baseline).
- **after**: 8 evidence rows (Track BI +3 additive: c_dxr_/c_rt_ 3-module
  detail row + cumulative completion row + Track BF deferred-annotation
  close-out row).
- **status**: partial -> partial (NOT met promotion).
- **blocker_reason refresh**: appends Track BI c_dxr_/c_rt_ 4/4 100%
  completion clause acknowledging 3 modules × 2 stage = 6 PARTIAL emit
  stage 추가 + Track BF c_dxr_inline_rt_shader carried 2 stage = 8 PARTIAL
  emit stage cumulative cond.2 scaffold; D4 always-online (blk.1) +
  D3DMetal binary licence (loader_dx blk.1 transitive) cross-blocked
  permanent gate.
- **handoff field added**: `ai_native_handoff_cond_2_perf_4family_completion_track_bi`.

## §5. Cross-link decision

Module-to-roadmap cross-link based on surface contact:

- **d2r cond.3 (Apple Metal layer)**: 9 modules cross-link —
  - c_macos_* (3): NSStatusBar / NSMenu / AVPictureInPictureController
    (Apple AppKit / AVFoundation surface).
  - c_fps_* (3): NSEvent globalMonitor / mach_absolute_time / CAMetalLayer
    drawable (Apple Metal compose pipeline + clock + event).
  - c_dxil_* (3): DXIL→AIR translator + GPTK persist + LLVM scalarize
    (D3DMetal predecessor surface).
- **d4 cond.2 (DXR raytracing)**: 3 modules cross-link —
  - c_dxr_/c_rt_* (3): D3D12 RT BLAS refit + ray dispatch + AS
    compaction (D4-specific DXR raytracing surface).

Track BF c_dxr_inline_rt_shader (deferred internal annotation only —
no .roadmap.diablo4 surgical edit at BF time) is now formally close-out
referenced in d4 cond.2 evidence Track BI augment row 3, completing the
deferred annotation→explicit edit posture transition.

## §6. Policy compliance

- `migration` forbidden / `additive_only` / `destructive_ops` 0 /
  `cost_usd` 0 / `substrate` mac-local / `cap_minutes` 90.
- `BR_NO_USER_VERBATIM`: 0 verbatim user quotes in any artifact.
- own1: Wine 0 / hexa-only / Apple SDK direct binding via hexa-native FFI
  scaffold OK posture (Apple platform components for c_macos_/c_fps_/
  c_dxil_ DXIL→AIR predecessor). NO real NSStatusBarItem / NO real
  NSMenuDelegate / NO real AVPictureInPictureController / NO real NSEvent
  globalMonitor / NO real mach_absolute_time / NO real CAMetalLayer
  drawable / NO real D3D12 RT command list / NO real BuildRaytracingAS
  / NO real DispatchRays / NO real DXIL bytecode parse / NO real GPTK
  persist / NO real LLVM IR scalarize. Synthetic byte-array offsets only
  (0x180050000 / 0x180052000 / 0x180054000 / 0x180056000 / 0x180058000 /
  0x18005A000 / 0x140150000 / 0x140152000 / 0x140154000 / 0x140156000 /
  0x140158000 / 0x14015A000) — fictitious scaffold pointers, NOT real
  RVAs.
- own2: 24 PARTIAL emit explicit (12 modules × 2 stage = 24 println
  markers `__<DOMAIN>__ PARTIAL <stage>` + 24 r0_emit round-trip events
  with verdict PARTIAL + posture evidence_augment_partial_to_partial).
  PASS swallow forbidden (cond statuses partial 영구 유지 across both
  augmented cond.* / status met preserved on d2r cond.2).

## §7. Caveats (≥6)

- **C1**: cond.3 (d2r) + cond.2 (d4) met 승격 영구 X — D3DMetal binary
  licence transitive (loader_dx blk.1) is the permanent external gate
  for d2r cond.3 met; D4 always-online (blk.1) + D3DMetal binary licence
  cross-blocked are the permanent gates for d4 cond.2 met (retired_intentional
  posture per d4 cond.3 precedent NOT applicable here — d4 cond.2
  partial 정식 유지). Track BI extension is evidence_augment only, NOT
  met promotion path.
- **C2**: Track BI closes 4 families to 4/4 100% completion (BREADTH +
  DEPTH). cumulative_c_macos_extended_after_track_bi = 4/4 (Track BF 1 +
  Track BI 3), cumulative_c_fps_extended_after_track_bi = 4/4 (Track BF
  1 + Track BI 3), cumulative_c_dxr_rt_extended_after_track_bi = 4/4
  (Track BF 1 + Track BI 3 — c_dxr_*+c_rt_* combined family),
  cumulative_c_dxil_extended_after_track_bi = 4/4 (Track BF 1 + Track BI
  3). Cumulative cross-track tally: c_d2r_ 26/26 + c_d4_ 27/27 +
  c_purple_ 100% + c_bnet_ 100% + c_lineage_ 100% + c_elden_ 100% +
  c_cs2_ 100% + c_oauth_ 100% + c_install_ 100% + c_metal_ 11/11 +
  c_pe_ 10/10 (all from prior tracks — read-only at Track BI) + Track
  BI 4/4/4/4 (this cycle). Track BJ parallel = c_display_/c_wg_+c_wgc_/
  c_bigworld_ remaining family completion (BJ subagent territory).
- **C3**: synthetic byte-array offsets (0x180050000 / 0x180052000 /
  0x180054000 / 0x180056000 / 0x180058000 / 0x18005A000 / 0x140150000 /
  0x140152000 / 0x140154000 / 0x140156000 / 0x140158000 / 0x14015A000)
  are fictitious scaffold pointers. Distinct from Track BF (0x180040000 /
  0x180042000 / 0x140130000 / 0x140134000 / 0x180048000 / 0x140140000 /
  0x140144000), Track BB c_metal_* (0x18001C000..0x180024000), Track BB
  c_install_* (0x180030000..0x180032000), Track AQ/BB c_pe_*
  (0x140110000..0x140124000) — collision check verified manually.
- **C4**: append goes between emit-count assertion and `cleanup_*()`
  gate (or before final cleanup block in c_dxil_persist) — additive_only
  spirit preserved (no row removal, no self_test body modification, no
  main() change, no fn signature change). Existing emit-count assertion
  thresholds unchanged in each module — append occurs after assertion,
  so extra 2 emits per module are bonus (actual counts will exceed
  thresholds).
- **C5**: hexa-lang upstream candidates: 0 hits discovered in this cycle.
  Each module already uses hexa-native primitives (r0_emit / println /
  exec for awk/grep). No new gap surfaced. `docs/HEXA_UPSTREAM.md` NOT
  modified (parallel BG: BG track territory, race condition risk per
  user directive).
- **C6**: in_place_writes=14 (12 c_*.hexa + 2 .roadmap.* JSONL header
  surgical edits). additive_only spirit preserved across all artifacts.
  Both roadmaps: untouched cond statuses preserved verbatim, blk.1
  fields untouched verbatim. cond.3 (d2r) + cond.2 (d4) evidence array:
  only += 3-4 rows + handoff field add + blocker_reason refresh.
- **C7**: Track BI territory ban respected: NO Track G/U/AI/AK/BA c_d2r_*
  (26 — 100%) / NO Track V/AJ/AL/BA/BE c_d4_* (27 — 100%) / NO Track B
  c_purple_* / NO Track J/N c_bnet_* / NO Track B/V c_lineage_* / NO
  Track AS c_elden_* / NO Track AT c_cs2_* / NO Track AR/BB c_oauth_* /
  NO Track AR/BB c_install_* / NO Track AP/BB c_metal_* / NO Track AQ/BB
  c_pe_* / NO Track BF (4 modules read-only: c_macos_menubar_status_item
  + c_fps_history_graph_render + c_dxr_inline_rt_shader +
  c_dxil_to_air_fusion) / NO lib/loader / NO tool / NO native / NO tests
  / NO existing docs. Parallel BJ (c_display_/c_wg_/c_wgc_/c_bigworld_)
  + parallel BH (lib/loader) territory respected.
- **C8**: docs/HEXA_UPSTREAM.md NOT modified (parallel track territory —
  race condition risk per user directive).

## §8. Predecessors

- `lib/perf/c_macos_menubar_status_item.hexa` (Track BF) — c_macos_* family
  Apple-native synthetic round-trip pattern reference (read-only at BI).
- `lib/perf/c_fps_history_graph_render.hexa` (Track BF) — c_fps_* family
  Apple-native synthetic round-trip pattern reference (read-only at BI).
- `lib/perf/c_dxr_inline_rt_shader.hexa` (Track BF) — c_dxr_* family
  RayQuery synthetic round-trip pattern reference (read-only at BI).
- `lib/perf/c_dxil_to_air_fusion.hexa` (Track BF) — c_dxil_* family
  DXIL→AIR fusion synthetic round-trip pattern reference (read-only at BI).
- `lib/perf/c_metal_simdgroup_reduce.hexa` (Track BB) — phase + matrix
  two-stage pattern.
- `state/markers/airgenome_gamebox_perf_remaining_family_inventory_track_bf_landed.marker`
  (Track BF) — direct predecessor schema parent.
- `airgenome/doc/airgenome_gamebox_perf_remaining_family_inventory_track_bf_landed_2026_05_05.ai.md`
  (Track BF) — direct precedent for multi-family augmentation.

## §9. Next-cycle actions

- Track BJ — close c_display_/c_wg_+c_wgc_/c_bigworld_ remaining families
  to 100% cumulative completion (parallel BJ subagent territory).
- Track BH — lib/loader extension cycle (parallel BH subagent territory).
- Production smoke remains gated on:
  - Real D2R.exe binary (Battle.net launcher install dependent — permanent).
  - D3DMetal binary licence (permanent external dependency, not Track-BI-resolvable).
  - D4 always-online network endpoint smoke (multi-cycle long-horizon).
  - User network endpoint smoke (battlenet cond.3, multi-cycle long-horizon).
