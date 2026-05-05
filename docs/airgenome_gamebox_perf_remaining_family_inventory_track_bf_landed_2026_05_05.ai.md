# airgenome-gamebox — Track BF perf remaining-family inventory cycle landing (2026-05-05)

## §1. Mission paraphrase

Track BF cycle: extend 7 untouched perf modules across 7 distinct families
(c_macos_/c_fps_/c_dxr_/c_dxil_/c_display_/c_wg_/c_bigworld_) — multi-family
breadth, NOT family-completion. Each module gains +33-35 LOC of synthetic
state machine round-trip rows in `self_test()`, mirroring the Track AP/AQ/AR/BB
predecessor extension pattern. The result: 7 representative modules from
remaining untouched families now carry `__<DOMAIN>__ PARTIAL <stage>` emit
markers spanning 14 stages (7 modules × 2 stages each), preparing future
family-completion cycles (BG+) with a cross-family scaffold baseline.

`.roadmap.diablo2_resurrected` cond.3 evidence array gained +2 rows (Track
BF c_fps_/c_dxil_/c_display_ aggregate detail row + cross-family inventory
cycle row). `.roadmap.battlenet` cond.3 evidence array gained +2 rows
(Track BF c_macos_/c_wg_/c_bigworld_ aggregate detail row + cross-launcher
inventory cycle row). All cond statuses unchanged (d2r cond.1 partial /
cond.2 met / cond.3 partial; battlenet cond.1 met / cond.2 met / cond.3
partial). Posture = `evidence_augment_partial_to_partial`, NOT met
promotion.

Cumulative Track-BF synthetic stage emit count: 14 PARTIAL stage emits
(7 modules × 2 stages each).

## §2. Cycle scope (additive_only)

| Concern               | Value                                             |
|-----------------------|---------------------------------------------------|
| modules extended      | 7 (1 per family × 7 untouched families)           |
| families touched      | 7 (c_macos_/c_fps_/c_dxr_/c_dxil_/c_display_/c_wg_/c_bigworld_) |
| LOC delta             | +33-35 each, total +240 LOC                       |
| roadmap edits         | 2 (.roadmap.diablo2_resurrected + .roadmap.battlenet) |
| handoff               | 1 NEW (this doc)                                  |
| marker                | 1 NEW                                             |
| cond status changes   | 0 (all preserved)                                 |
| destructive_ops       | 0                                                 |
| in_place_writes       | 9 (7 modules + 2 roadmap)                         |
| migration             | forbidden (additive_only)                         |
| cost_usd              | 0                                                 |
| substrate             | mac-local                                         |
| cap_minutes           | 90                                                |

## §3. Modules extended (7 total)

| # | path | family | before LOC | after LOC | delta | emit prefix | stages |
|---|------|--------|-----------|-----------|-------|-------------|--------|
| 1 | lib/perf/c_macos_menubar_status_item.hexa | c_macos_* | 170 | 205 | +35 | `__MACOS_STATUS__` | register_status_phase / length_visibility_matrix |
| 2 | lib/perf/c_fps_history_graph_render.hexa | c_fps_* | 169 | 202 | +33 | `__FPS_GRAPH__` | graph_render_phase / bucket_quantize_matrix |
| 3 | lib/perf/c_dxr_inline_rt_shader.hexa | c_dxr_* | 127 | 161 | +34 | `__DXR_INLINE__` | rayquery_phase / raytrace_matrix |
| 4 | lib/perf/c_dxil_to_air_fusion.hexa | c_dxil_* | 156 | 191 | +35 | `__DXIL_FUSION__` | fusion_lower_phase / lowering_matrix |
| 5 | lib/perf/c_display_mode_dpi_scale_adapt.hexa | c_display_* | 170 | 204 | +34 | `__DISPLAY_DPI__` | dpi_scale_phase / scale_matrix |
| 6 | lib/perf/c_wg_battle_entry_block.hexa | c_wg_* | 191 | 225 | +34 | `__WG_ENTRY__` | entry_block_phase / decision_matrix |
| 7 | lib/perf/c_bigworld_core_dx11_first_frame.hexa | c_bigworld_* | 185 | 220 | +35 | `__BIGWORLD_DX11__` | first_frame_phase / first_frame_matrix |

7 modules × 2 stages = 14 PARTIAL emit stage 누적.

## §4. Roadmap evidence augments

### .roadmap.diablo2_resurrected cond.3 (Apple Metal layer)
- **before**: 18 evidence rows (post Track BB).
- **after**: 20 evidence rows (Track BF +2 additive: c_fps_/c_dxil_/c_display_
  3-module aggregate detail row + cross-family inventory cycle row).
- **status**: partial -> partial (NOT met promotion, evidence_augment_partial_to_partial
  posture).
- **blocker_reason refresh**: acknowledges Track BF 3 Apple-Metal-layer-touching
  modules (c_fps_history_graph_render + c_dxil_to_air_fusion + c_display_mode_dpi_scale_adapt)
  × 2 stage = 6 PARTIAL emit stage 추가, cumulative cond.3 stage 측 24+ scaffold;
  D3DMetal binary licence transitive (loader_dx blk.1) gate remains permanent.
- **handoff field added**: `ai_native_handoff_cond_3_perf_remaining_family_inventory_track_bf`.

### .roadmap.battlenet cond.3 (OAuth + install + launcher cross-link)
- **before**: 18 evidence rows (post Track BB).
- **after**: 20 evidence rows (Track BF +2 additive: c_macos_/c_wg_/c_bigworld_
  3-module aggregate detail row + cross-launcher inventory cycle row).
- **status**: partial -> partial (NOT met promotion).
- **blocker_reason refresh**: acknowledges Track BF 3 launcher/engine-touching
  modules (c_macos_menubar_status_item + c_wg_battle_entry_block + c_bigworld_core_dx11_first_frame)
  × 2 stage = 6 PARTIAL emit stage 추가, cumulative cond.3 stage 측 41+ scaffold.
- **handoff field added**: `ai_native_handoff_cond_3_perf_remaining_family_inventory_track_bf`.

## §5. Cross-link decision

Multi-family inventory was cross-linked to two roadmaps based on module surface:
- **d2r cond.3 (Apple Metal layer)**: c_fps_history_graph_render (Metal compose
  pipeline) + c_dxil_to_air_fusion (DXIL→AIR translator) + c_display_mode_dpi_scale_adapt
  (NSScreen backingScale / CAMetalLayer drawableSize) — all touch Apple Metal
  surface or its DXIL→AIR translator predecessor.
- **battlenet cond.3 (launcher + network)**: c_macos_menubar_status_item
  (NSStatusBar gamebox launcher menu surface) + c_wg_battle_entry_block
  (Wargaming launcher gateway, network OAuth-style) + c_bigworld_core_dx11_first_frame
  (BigWorld engine first-frame, launcher-loaded engine).
- **c_dxr_inline_rt_shader** (D4-specific DXR surface): no roadmap evidence
  augment landed this cycle — Track BF chose d2r/battlenet roadmaps for the
  augment scope (mission permits 1-2 roadmaps). diablo4 cond.2 (DXR
  raytracing) is acknowledged within the module's own internal annotation
  (`cond:diablo4.cond.2`) but no `.roadmap.diablo4` JSONL surgical edit
  occurred this cycle. Future Track BG+ may cross-link the DXR module to
  diablo4 cond.2 if the c_dxr_/c_rt_ family extends to completion.

## §6. Policy compliance

- `migration` forbidden / `additive_only` / `destructive_ops` 0 /
  `cost_usd` 0 / `substrate` mac-local / `cap_minutes` 90.
- `BR_NO_USER_VERBATIM`: 0 verbatim user quotes in any artifact.
- own1: Wine 0 / hexa-only / Apple SDK direct binding via hexa-native FFI
  scaffold OK (Apple platform components for c_macos_/c_fps_/c_display_
  surface). NO NSStatusBar binary linkage attempt / NO CAMetalLayer drawable
  call / NO real RayQuery.Proceed / NO real DXIL→AIR translator / NO real
  Wargaming network I/O / NO real BigWorld engine bootstrap / NO real
  D3D11CreateDevice. Synthetic byte-array offsets only (0x180040000 /
  0x180042000 / 0x140130000 / 0x140134000 / 0x180048000 / 0x140140000 /
  0x140144000) — fictitious scaffold pointers, NOT real RVAs.
- own2: 14 PARTIAL emit explicit (7 modules × 2 stage = 14 println markers
  `__<DOMAIN>__ PARTIAL <stage>` + 14 r0_emit round-trip events with
  verdict PARTIAL + posture evidence_augment_partial_to_partial). PASS
  swallow forbidden (cond statuses partial 영구 유지 across the 2 cond.*
  augmented).

## §7. Caveats (≥6)

- **C1**: cond.3 met 승격 영구 X — D3DMetal binary licence transitive
  (loader_dx blk.1) is the permanent external gate for d2r cond.3 met;
  user network smoke + 자연 환경 정합 long-horizon work is the permanent
  gate for battlenet cond.3 met. Track BF extension is evidence_augment
  only, NOT met promotion path.
- **C2**: Track BF picks 1 module per family across 7 untouched families
  (multi-family BREADTH, not family-completion). cumulative_c_macos_extended
  = 1/4, cumulative_c_fps_extended = 1/4, cumulative_c_dxr_extended = 1/3
  (c_dxr_*+c_rt_* combined 1/4), cumulative_c_dxil_extended = 1/4,
  cumulative_c_display_extended = 1/8, cumulative_c_wg_extended = 1/4
  (c_wg_*+c_wgc_* combined 1/6), cumulative_c_bigworld_extended = 1/3.
  Future Track BG+ may close any of these to 100%.
- **C3**: c_dxr_inline_rt_shader internal annotation references
  `diablo4.cond.2` but `.roadmap.diablo4` JSONL was NOT edited this cycle
  (mission scope permits 1-2 roadmaps; chose d2r + battlenet). The module
  is functional and emit-tagged; future cycle may augment .roadmap.diablo4
  evidence if DXR family completion lands.
- **C4**: synthetic byte-array offsets (0x180040000 / 0x180042000 /
  0x140130000 / 0x140134000 / 0x180048000 / 0x140140000 / 0x140144000)
  are fictitious scaffold pointers. Distinct from Track BB c_metal_*
  (0x18001C000..0x180024000), Track BB c_install_* (0x180030000..0x180032000),
  Track AQ/BB c_pe_* (0x140110000..0x140124000) — collision check verified
  manually.
- **C5**: append goes between emit-count assertion and `cleanup_*()` gate
  — additive_only spirit preserved (no row removal, no self_test body
  modification, no main() change, no fn signature change). Existing
  emit-count assertion thresholds unchanged in each module — append occurs
  after assertion, so extra 2 emits per module are bonus (actual counts
  will exceed thresholds).
- **C6**: hexa-lang upstream candidates: 0 hits discovered in this cycle.
  Each module already uses hexa-native primitives (r0_emit / println /
  exec for awk/grep). No new gap surfaced. `docs/HEXA_UPSTREAM.md` NOT
  modified (parallel BG: BG track territory, race condition risk per user
  directive).
- **C7**: in_place_writes=9 (7 c_*.hexa + 2 .roadmap.* JSONL header
  surgical edits). additive_only spirit preserved across all artifacts.
  Both roadmaps: cond.1 + cond.2 fields untouched verbatim, blk.1 fields
  untouched verbatim. cond.3 evidence array: only += 2 rows + handoff
  field add + blocker_reason refresh.
- **C8**: Track BF territory ban respected: NO Track G/U/AI/AK/BA c_d2r_*
  (26 — 100%) / NO Track V/AJ/AL/BA c_d4_* (25, plus BE final 2) / NO
  Track B c_purple_* / NO Track J/N c_bnet_* / NO Track B/V c_lineage_* /
  NO Track AS c_elden_* / NO Track AT c_cs2_* / NO Track AR/BB c_oauth_*
  / NO Track AR/BB c_install_* / NO Track AP/BB c_metal_* / NO Track
  AQ/BB c_pe_* / NO lib/loader / NO tool / NO native / NO tests / NO
  existing docs. Parallel BG (BG/BD/BE) territory respected.
- **C9**: docs/HEXA_UPSTREAM.md NOT modified (parallel track territory —
  race condition risk per user directive).

## §8. Predecessors

- `lib/perf/c_d2r_first_frame_trace.hexa` (Track G) — append pattern reference.
- `lib/perf/c_metal_capture_auto.hexa` (Track AP) — Apple-native synthetic round-trip pattern.
- `lib/perf/c_metal_simdgroup_reduce.hexa` (Track BB) — phase + matrix two-stage pattern.
- `lib/perf/c_pe_iat_simulation.hexa` (Track AQ) — PE loader synthetic round-trip pattern.
- `lib/perf/c_install_disk_space_check.hexa` (Track BB) — install path synthetic round-trip pattern.
- `state/markers/airgenome_gamebox_perf_3family_completion_track_bb_landed.marker` (Track BB) — schema parent.
- `docs/airgenome_gamebox_perf_3family_completion_track_bb_landed_2026_05_04.ai.md` (Track BB) — direct precedent for multi-family augmentation.

## §9. Next-cycle actions

- Track BG (this BG subagent's parallel) — hexa-lang upstream surface
  aggregation cycle (docs/HEXA_UPSTREAM.md territory).
- Track BH+ — close c_macos_/c_fps_/c_dxr_/c_dxil_/c_display_/c_wg_/c_bigworld_
  families to 100% cumulative completion (each remaining 2-7 modules per
  family). Pattern reference: this Track BF representative module per
  family + Track BB cumulative completion playbook.
- Production smoke remains gated on:
  - Real D2R.exe binary (Battle.net launcher install dependent — permanent).
  - D3DMetal binary licence (permanent external dependency, not Track-BF-resolvable from within own1).
  - User network endpoint smoke (battlenet cond.3, multi-cycle long-horizon).
