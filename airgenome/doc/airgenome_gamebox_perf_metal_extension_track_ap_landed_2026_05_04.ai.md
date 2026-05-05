# airgenome-gamebox — Track AP perf_metal extension landing (2026-05-04)

## §1. Mission paraphrase
Track AP cycle: Apple Metal API surface (lib/perf/c_metal_*.hexa, family 11) extension — pick 6 untouched modules (none had been extended in any prior cycle), append synthetic state machine round-trip rows mirroring Track G/U/AI predecessor pattern (`__METAL_<DOMAIN>__ PARTIAL <stage>` emit prefix style). 6 modules × 2 stage = 12 PARTIAL emit stage 추가. .roadmap.diablo2_resurrected cond.3 evidence array augment (13 -> 16, additive +3 rows referencing Track AP Apple-native scaffold). cond.3 status `partial` 영구 유지 — D3DMetal binary licence transitive blocker (loader_dx blk.1) remains permanent gate, met 승격 X. cond.1 (Track AI) + cond.2 (met, Track A) untouched.

## §2. Modules extended (6/11)
| # | path | before LOC | after LOC | delta | emit prefix | stages |
|---|------|-----------|-----------|-------|-------------|--------|
| 1 | lib/perf/c_metal_capture_auto.hexa       | 201 | 233 | +32 | `__METAL_CAPTURE__`    | capture_phase / trigger_scope_matrix |
| 2 | lib/perf/c_metal_fence_track.hexa        | 195 | 227 | +32 | `__METAL_FENCE__`      | fence_phase / queue_value_matrix |
| 3 | lib/perf/c_metal_pipeline_archive.hexa   | 182 | 214 | +32 | `__METAL_ARCHIVE__`    | archive_phase / archive_pso_matrix |
| 4 | lib/perf/c_metal_queue_priority.hexa     | 160 | 192 | +32 | `__METAL_QUEUE__`      | priority_phase / frametime_hostility_matrix |
| 5 | lib/perf/c_metal_residency_coalesce.hexa | 155 | 187 | +32 | `__METAL_RESIDENCY__`  | residency_phase / frame_kind_matrix |
| 6 | lib/perf/c_metal_visibility_result.hexa  | 185 | 217 | +32 | `__METAL_VISIBILITY__` | visibility_phase / query_pass_matrix |

Untouched 5/11 (next-cycle candidates): c_metal_private_heap_prewarm / c_metal_profiler_feedback / c_metal_res_opt_sampler / c_metal_shared_event_sched / c_metal_simdgroup_reduce.

## §3. cond.3 evidence augment (Apple-native scaffold)
- **before**: 13 evidence rows (Track G renderer scaffold 6 stage + ancestor 7).
- **after**: 16 evidence rows (Track AP +3 additive: capture_auto detail row + 5-module aggregate row + cumulative 6/11 marker row).
- **status**: partial -> partial (NOT met promotion, evidence_augment_partial_to_partial posture).
- **blocker_reason refresh**: acknowledges Track AP 6/11 c_metal_* modules + 12 PARTIAL emit stage 추가 (cumulative 16 cond.3 stage 누적). D3DMetal binary licence transitive (loader_dx blk.1) remains permanent gate.
- **handoff field added**: `ai_native_handoff_cond_3_perf_metal_extension_track_ap`.
- **untouched**: cond.1 evidence_count=32 (Track AI/AK territory), cond.2 evidence_count=9 (met, Track A territory).

## §4. Cross-link to D2R/D4 D3DMetal renderer path
c_metal_* modules are Apple-native counterparts to the D3DMetal binary licence transitive blocker (loader_dx blk.1) for D2R cond.3 / D4 cond.2 (DXR). Track AP adds skeleton-tier evidence to this layer, contributing to the long-horizon path toward eventual real D3DMetal layer integration. Each module covers a distinct Metal API surface:
- MTLCaptureManager (anomaly trigger frame capture)
- MTLFence (cross-queue signal/wait sync)
- MTLBinaryArchive (PSO compile cache cold-launch hitch elimination)
- MTLCommandQueue priority (frametime + hostility decision)
- MTLHeap residency coalesce (delta accumulate + match pair)
- MTLVisibilityResultBuffer (occlusion query GPU readback)

## §5. Policy compliance
- `migration` forbidden / `additive_only` / `destructive_ops` 0 / `cost_usd` 0 / `substrate` mac-local / `cap_minutes` 90.
- `BR_NO_USER_VERBATIM`: 0 verbatim user quotes in any artifact.
- own1: Wine 0 / hexa-only / Apple SDK direct binding via hexa-native FFI scaffold OK / NO Metal binary linkage attempt (synthetic byte-array offsets 0x180010000..0x18001A000 are inline literals, no real Metal RVA correspondence) / synthetic round-trip ONLY (no real MTLCaptureManager.startCapture / MTLFence.signal / MTLBinaryArchive.serializeToURL / MTLCommandQueue.priority setter / MTLHeap.makeAliasable / setVisibilityResultMode invocation).
- own2: 12 PARTIAL emit explicit (6 modules × 2 stage = 12 println markers + 12 r0_emit round-trip events with verdict PARTIAL + posture evidence_augment_partial_to_partial). PASS swallow forbidden (cond.3 status partial 영구 유지).

## §6. Caveats (≥6)
- **C1**: cond.3 met 승격 영구 X — D3DMetal binary licence transitive (loader_dx blk.1) is the permanent external gate. Track AP extension is evidence_augment only, NOT met promotion path.
- **C2**: c_metal_* synthetic byte-array offsets (0x180010000..0x18001A000) are fictitious scaffold pointers in 0x18001x range, NOT real Metal framework RVAs. Distinct from c_d2r_* synthetic offsets (0x14008x..0x1400Ax range, fictitious D2R.exe RVAs).
- **C3**: 5/11 c_metal_* modules untouched (private_heap_prewarm / profiler_feedback / res_opt_sampler / shared_event_sched / simdgroup_reduce) — deferred to follow-up Track for next-cycle extension to reach 11/11 cumulative.
- **C4**: append goes between emit-count assertion and `cleanup_*()` gate — additive_only spirit preserved (no row removal, no self_test body modification, no main() change). Each module gains exactly +32 LOC.
- **C5**: hexa-lang upstream candidates: 0 hits discovered in this cycle. Each module already uses hexa-native primitives (r0_emit, exec, write_file, awk, sort -u). No Metal API hand-binding gap surfaced (synthetic emit only). docs/hexa_upstream_track_ap_*.md NOT created (per user 2026-05-04 directive — no entry if no hits).
- **C6**: in_place_writes=7 (6 c_metal_*.hexa + 1 .roadmap.diablo2_resurrected JSONL header surgical edit) — Track AI predecessor (in_place_writes=7) 동급 posture. additive_only spirit preserved, 0 row removal across all artifacts.
- **C7**: Track AP territory ban: c_d2r_* / c_d4_* / c_purple_* / c_bnet_* / c_lineage_* / loader / tool / native / tests / .roadmap (other than diablo2_resurrected cond.3) untouched. Parallel BG (AK/AL/AM/AN/AO/AQ) territory respected.
- **C8**: docs/HEXA_UPSTREAM.md NOT modified (Track AO concurrent territory — race condition risk per user 2026-05-04 directive).
- **C9**: cumulative_c_metal_extended_after_track_ap = 6/11. Earlier all 11 were skeleton-tier (none extended in any prior cycle per Track T inventory).
- **C10**: emit_count assertion threshold unchanged in each module (4, 6, 4, 3, 3, 4 respectively) — append occurs after assertion, so extra 2 emits per module are bonus (actual counts will exceed thresholds). Existing self_test body verification 0 line modify.

## §7. Predecessors
- `lib/perf/c_d2r_first_frame_trace.hexa` (Track G) — append pattern reference.
- `lib/perf/c_d2r_act_progression.hexa` (Track AI) — append pattern reference.
- `state/markers/airgenome_gamebox_perf_game_d2r_extension_track_ai_landed.marker` (Track AI) — schema parent.
- `state/markers/airgenome_gamebox_perf_game_d2r_integration_landed.marker` (Track U) — pattern reference.
- `state/markers/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed.marker` (Track G) — D3DMetal blocker context.

## §8. Next-cycle actions
- Track AQ — extend remaining 5 untouched c_metal_* modules (private_heap_prewarm / profiler_feedback / res_opt_sampler / shared_event_sched / simdgroup_reduce) to reach 11/11 cumulative.
- Track AR — D4 cond.2 (DXR) cross-game roadmap evidence augment cross-link to Track AP Apple-native scaffold.
- production smoke remains gated on D3DMetal binary licence (permanent external dependency; not Track-AZ-resolvable from within own1).
