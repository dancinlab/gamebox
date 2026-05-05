# airgenome-gamebox — perf 3-family completion (Track BJ) — 2026-05-05 landed

## Mission

Track BF landed 1 module per family for c_display_*, c_wg_*/c_wgc_*, c_bigworld_*. Track BJ
completes the remaining untouched modules in each family — 3-family 100% completion (cumulative).

- c_display_*: Track BF 1/8 (c_display_mode_dpi_scale_adapt) → Track BJ +7/8 → cumulative 8/8 100%.
- c_wg_*/c_wgc_* (combined): Track BF 1/6 (c_wg_battle_entry_block) → Track BJ +5/6 → cumulative 6/6 100%.
- c_bigworld_*: Track BF 1/3 (c_bigworld_core_dx11_first_frame) → Track BJ +2/3 → cumulative 3/3 100%.

Each module: append +25-50 LOC synthetic round-trip block (4-phase pipeline + 16-slot matrix),
emit `__<DOMAIN>__ PARTIAL <state>`, posture = evidence_augment_partial_to_partial, met 승격 X.

## Track BJ — 14 module landings

### c_display_* (7 untouched, +35 LOC each)

| module | emit prefix | phase RVA | states |
|---|---|---|---|
| c_display_mode_fullscreen_exclusive_metal.hexa | __DISPLAY_FS__ | 0x180050000 | fullscreen_phase, swap_matrix |
| c_display_mode_input_focus_routing.hexa | __DISPLAY_FOCUS__ | 0x180052000 | focus_routing_phase, routing_matrix |
| c_display_mode_pip_apple_avfoundation.hexa | __DISPLAY_PIP_AV__ | 0x180054000 | pip_av_phase, corner_matrix |
| c_display_mode_pip_focus_ctrl_g.hexa | __DISPLAY_CTRL_G__ | 0x180056000 | focus_toggle_phase, direction_matrix |
| c_display_mode_pip_render_concurrent.hexa | __DISPLAY_PIP_RENDER__ | 0x180058000 | concurrent_phase, overhead_matrix |
| c_display_mode_transition_perf.hexa | __DISPLAY_TRANSITION__ | 0x18005A000 | transition_phase, mode_matrix |
| c_display_mode_window_state.hexa | __DISPLAY_WINDOW__ | 0x18005C000 | window_state_phase, state_matrix |

cond cross-link = diablo2_resurrected.cond.3 (D3DMetal binary licence transitive — loader_dx blk.1).

### c_wg_*/c_wgc_* (5 untouched, +35 LOC each)

| module | emit prefix | phase RVA | states |
|---|---|---|---|
| c_wg_cdn_chunk_parallel.hexa | __WG_CDN__ | 0x140148000 | cdn_chunk_phase, worker_matrix |
| c_wg_delta_update_proto.hexa | __WG_DELTA__ | 0x14014A000 | delta_update_phase, ratio_matrix |
| c_wg_oauth_proxy.hexa | __WG_OAUTH__ | 0x14014C000 | oauth_proxy_phase, grant_matrix |
| c_wgc_chromium_multi_proc_track.hexa | __WGC_IPC__ | 0x14014E000 | chromium_ipc_phase, proc_matrix |
| c_wgc_pe64_loader_probe.hexa | __WGC_PE64__ | 0x140150000 | pe64_probe_phase, section_matrix |

cond cross-link = battlenet.cond.3 (production smoke gated on user network smoke + 자연 환경 long-horizon).

### c_bigworld_* (2 untouched, +35 LOC each)

| module | emit prefix | phase RVA | states |
|---|---|---|---|
| c_bigworld_core_shader_persist.hexa | __BIGWORLD_SHADER__ | 0x140152000 | shader_persist_phase, cache_matrix |
| c_bigworld_python_script_cache.hexa | __BIGWORLD_PY__ | 0x140154000 | py_cache_phase, hit_matrix |

cond cross-link = battlenet.cond.3 (BigWorld engine binary licence transitive — long-horizon).

## .roadmap edits (+5 evidence rows)

- `.roadmap.diablo2_resurrected` cond.3 evidence: 24→27 (+3 Track BJ rows) — c_display 7-module
  detail + c_bigworld DX11 cross-link Metal layer evidence + 3-family cumulative 100% summary.
- `.roadmap.battlenet` cond.3 evidence: 20→22 (+2 Track BJ rows) — c_wg/c_wgc 5-module detail +
  c_bigworld engine cross-link battlenet network stack.

Both cond.3 status remain `partial` (evidence_augment_partial_to_partial; met 승격 X — D3DMetal
binary licence + BigWorld engine binary licence + battlenet network production smoke transitive
long-horizon gates).

## Cumulative 3-family completion

- c_display_*: 8/8 100% (Track BF 1 + Track BJ 7).
- c_wg_*/c_wgc_*: 6/6 100% (Track BF 1 + Track BJ 5).
- c_bigworld_*: 3/3 100% (Track BF 1 + Track BJ 2).
- Total Track BF + BJ across 3 families = 17 modules (3 + 14).

## Policy

- migration: forbidden / additive_only / destructive 0 / cost 0 / mac-local / cap 90min — held.
- BR_NO_USER_VERBATIM / friendly_preset / silent-land — held.
- own1: Wine 0 / hexa-only / synthetic only — held (no real CAMetalLayer/AVFoundation/Mojo/
  D3DCompile/PyMarshal calls; all synthetic_inline).
- own2: PARTIAL emit explicit — held (28 emits across 14 modules: 14 phase + 14 matrix).
- raw 9/11/12/15/175 — applied.

## Verification

1. 14 modules show LOC growth (~35 LOC each, total 3341→3832 = +491 LOC in target families).
2. Both roadmap JSONL parse OK (Python json.loads roundtrip).
3. Marker JSON parses with 3 cumulative 100% fields.
4. sha256 pinned in marker.

Forbidden scope held: Track BF 3 modules read-only (no edits to c_display_mode_dpi_scale_adapt,
c_wg_battle_entry_block, c_bigworld_core_dx11_first_frame). No tool/native/tests/lib loader
edits. No other family edits. No existing docs/*.md edits. Only cond.3 of d2r/battlenet
augmented (cond.1/cond.2 untouched, blk.1 untouched).
