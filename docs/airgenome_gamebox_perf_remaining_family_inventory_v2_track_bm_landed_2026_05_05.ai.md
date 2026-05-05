# airgenome-gamebox — Track BM perf remaining-family inventory v2 cycle landing (2026-05-05)

## §1. Mission paraphrase

Track BM cycle: extend 7 perf modules across 7 distinct previously-untouched
families (c_sote_*, c_steam_*, c_wot_*, c_wows_*, c_iat_*, c_dll_*) — multi-family
breadth, NOT family-completion. Each module gains +35-37 LOC of synthetic
state machine round-trip rows in `self_test()`, mirroring the Track BF/BJ
predecessor extension pattern (4-phase pipeline + 16-slot matrix + 2 PARTIAL
emit stages per module). The result: 7 representative modules from 6
remaining untouched families now carry `__<DOMAIN>__ PARTIAL <stage>` emit
markers spanning 14 stages (7 modules × 2 stages each), surfacing a v2
inventory snapshot beyond Track T's ~93 unidentified "others" residual.

`.roadmap.elden_ring` cond.2 + cond.3 evidence arrays gained +3 rows
(Track BM c_sote_realm_of_shadow_zone_load cond.2 augment + c_sote_dlc_content_probe
cond.3 augment + cross-link inventory cycle row). `.roadmap.cs2` cond.1
evidence array gained +2 rows (Track BM c_steam_drm_stub_track cond.1
augment + cross-link inventory cycle row). All cond statuses unchanged
(elden_ring cond.1 partial / cond.2 partial / cond.3 partial; cs2 cond.1
partial / cond.2 partial / cond.3 partial). Posture =
`evidence_augment_partial_to_partial`, NOT met promotion.

Cumulative Track-BM synthetic stage emit count: 14 PARTIAL stage emits
(7 modules × 2 stages each).

## §2. Cycle scope (additive_only)

| Concern               | Value                                             |
|-----------------------|---------------------------------------------------|
| modules extended      | 7 (1 per family × 6 untouched families, c_sote × 2) |
| families touched      | 6 (c_sote_/c_steam_/c_wot_/c_wows_/c_iat_/c_dll_) |
| LOC delta             | +35-37 each, total +248 LOC                       |
| roadmap edits         | 2 (.roadmap.elden_ring + .roadmap.cs2)            |
| handoff               | 1 NEW (this doc)                                  |
| marker                | 1 NEW                                             |
| cond status changes   | 0 (all preserved partial)                         |
| destructive_ops       | 0                                                 |
| in_place_writes       | 9 (7 modules + 2 roadmap)                         |
| migration             | forbidden (additive_only)                         |
| cost_usd              | 0                                                 |
| substrate             | mac-local                                         |
| cap_minutes           | 90                                                |

## §3. Modules extended (7 total)

| # | path | family | before LOC | after LOC | delta | emit prefix | stages |
|---|------|--------|-----------|-----------|-------|-------------|--------|
| 1 | lib/perf/c_sote_dlc_content_probe.hexa | c_sote_* | 194 | 230 | +36 | `__SOTE_DLC__` | dlc_probe_phase / probe_matrix |
| 2 | lib/perf/c_sote_realm_of_shadow_zone_load.hexa | c_sote_* | 186 | 221 | +35 | `__SOTE_ZONE__` | zone_load_phase / streaming_matrix |
| 3 | lib/perf/c_steam_drm_stub_track.hexa | c_steam_* | 148 | 184 | +36 | `__STEAM_DRM__` | drm_stub_phase / dispatch_matrix |
| 4 | lib/perf/c_wot_garage_warmup.hexa | c_wot_* | 186 | 221 | +35 | `__WOT_GARAGE__` | garage_warmup_phase / warmup_matrix |
| 5 | lib/perf/c_wows_port_warmup.hexa | c_wows_* | 187 | 223 | +36 | `__WOWS_PORT__` | port_warmup_phase / warmup_matrix |
| 6 | lib/perf/c_iat_resolve_cache.hexa | c_iat_* | 298 | 333 | +35 | `__IAT_RESOLVE__` | iat_resolve_phase / resolve_matrix |
| 7 | lib/perf/c_dll_load_predict.hexa | c_dll_* | 505 | 540 | +35 | `__DLL_PREDICT__` | dll_predict_phase / freq_matrix |

7 modules × 2 stages = 14 PARTIAL emit stage 누적.

## §4. Roadmap evidence augments

### .roadmap.elden_ring cond.2 (D3D12 + PSO warm + FromSoft HKS) and cond.3 (MSIX + license + offline save)
- **cond.2 before**: 8 evidence rows (post Track AS).
- **cond.2 after**: 9 evidence rows (Track BM +1: c_sote_realm_of_shadow_zone_load row).
- **cond.3 before**: 9 evidence rows (post Track BE).
- **cond.3 after**: 11 evidence rows (Track BM +2: c_sote_dlc_content_probe row + cross-link inventory cycle row).
- **status**: partial -> partial (NOT met promotion, evidence_augment_partial_to_partial posture).
- **blocker_reason refresh**: acknowledges Track BM c_sote_* 2 modules × 2 stage
  = 4 PARTIAL emit stage 추가, cumulative cond.2/cond.3 stage 측 evidence_augment 누적;
  EAC anti-tamper blk.1 retired_intentional 영구 lock + FromSoft Dantelion engine
  binary licence transitive permanent gate.
- **handoff fields added**: `ai_native_handoff_cond_2_perf_remaining_family_inventory_v2_track_bm`,
  `ai_native_handoff_cond_3_perf_remaining_family_inventory_v2_track_bm`.

### .roadmap.cs2 cond.1 (cs2.exe PE parse + Steam API 64 offline shim)
- **before**: 5 evidence rows (post Track AT).
- **after**: 7 evidence rows (Track BM +2: c_steam_drm_stub_track row + cross-link inventory cycle row).
- **status**: partial -> partial (NOT met promotion).
- **blocker_reason refresh**: acknowledges Track BM c_steam_drm_stub_track 1 module
  × 2 stage = 2 PARTIAL emit stage 추가, VAC anti-cheat blk.1 retired_intentional
  영구 lock, no AC bypass / no anti-tamper evade.
- **handoff field added**: `ai_native_handoff_cond_1_perf_remaining_family_inventory_v2_track_bm`.

## §5. Cross-link decision

Multi-family inventory v2 was cross-linked to two roadmaps based on module surface:
- **elden_ring cond.2 (D3D12 + PSO + HKS)**: c_sote_realm_of_shadow_zone_load
  (Realm of Shadow zone streaming hits PSO warm + asset stream paths) — direct
  Dantelion engine surface.
- **elden_ring cond.3 (MSIX + license + offline save)**: c_sote_dlc_content_probe
  (SotE DLC content discovery — distribution probe surface).
- **cs2 cond.1 (cs2.exe + Steam API)**: c_steam_drm_stub_track (Steam DRM stub
  dispatch sibling to c_cs2_steam_api64_offline_shim).
- **c_wot_garage_warmup + c_wows_port_warmup**: BigWorld engine modules; internal
  annotation references `battlenet.cond.3` (Wargaming network long-horizon transitive
  via launcher-class cross-ref). No `.roadmap.battlenet` JSONL surgical edit this
  cycle (mission scope permits 1-2 roadmaps; chose elden_ring + cs2 for the
  augment scope). battlenet cond.3 blocker_reason already encompasses Wargaming
  cross-launcher long-horizon gate per Track BJ/BF predecessor.
- **c_iat_resolve_cache + c_dll_load_predict**: PE loader x64 path modules;
  internal annotation references `diablo2_resurrected.cond.3` (loader_pe blk.1
  transitive via PE x64 thunk binding / LoadLibrary chain long-horizon). No
  `.roadmap.diablo2_resurrected` JSONL edit this cycle (already received Track
  BF/BI/BJ augments — over-augmented risk avoided). Future Track BN+ may
  cross-link c_iat_/c_dll_ to .roadmap.loader_pe directly.

## §6. Policy compliance

- `migration` forbidden / `additive_only` / `destructive_ops` 0 /
  `cost_usd` 0 / `substrate` mac-local / `cap_minutes` 90.
- `BR_NO_USER_VERBATIM`: 0 verbatim user quotes in any artifact.
- own1: Wine 0 / hexa-only / synthetic only. NO real SteamAPI_Init binding /
  NO real DCX archive parse / NO real BigWorld pkg disk read / NO real
  GetProcAddress / NO real LoadLibraryW / NO real FromSoft DLC manifest read /
  NO real production VAC ticket interaction. Synthetic byte-array offsets only
  (0x140160000 / 0x140162000 / 0x140164000 / 0x140166000 / 0x140168000 /
  0x14016A000 / 0x14016C000) — fictitious scaffold pointers, NOT real RVAs.
- own2: 14 PARTIAL emit explicit (7 modules × 2 stage = 14 println markers
  `__<DOMAIN>__ PARTIAL <stage>` + 14 r0_emit round-trip events with
  verdict PARTIAL + posture evidence_augment_partial_to_partial). PASS
  swallow forbidden (cond statuses partial 영구 유지 across all 3 augmented
  cond.* fields).
- raw 9/11/12/15/175 — applied.

## §7. Caveats (≥6)

- **C1**: cond.1/cond.2/cond.3 met 승격 영구 X — EAC anti-tamper blk.1
  retired_intentional permanent for elden_ring; VAC anti-cheat blk.1
  retired_intentional permanent for cs2; loader_pe blk.1 + Wargaming network
  long-horizon for c_iat_/c_dll_/c_wot_/c_wows_ transitives. Track BM
  extension is evidence_augment only, NOT met promotion path.
- **C2**: Track BM picks 1 module per family across 6 untouched families
  (c_sote_* gets 2 modules — completes 2/2 in family). cumulative_c_sote_extended
  = 2/2 (100% — c_sote_* family closes), cumulative_c_steam_extended = 1/1
  (100% — only 1 module exists), cumulative_c_wot_extended = 1/2,
  cumulative_c_wows_extended = 1/2, cumulative_c_iat_extended = 1/2,
  cumulative_c_dll_extended = 1/3. Future Track BN+ may close any of
  these to 100%.
- **C3**: c_wot_/c_wows_/c_iat_/c_dll_ internal annotations reference
  battlenet.cond.3 / diablo2_resurrected.cond.3 but those .roadmap JSONLs
  were NOT edited this cycle (mission scope permits 1-2 roadmaps; chose
  elden_ring + cs2). Modules functional and emit-tagged; future cycle may
  augment .roadmap.battlenet or .roadmap.loader_pe if BigWorld cross-game or
  PE loader family completion lands.
- **C4**: synthetic byte-array offsets (0x140160000 / 0x140162000 /
  0x140164000 / 0x140166000 / 0x140168000 / 0x14016A000 / 0x14016C000)
  are fictitious scaffold pointers. Distinct from Track BJ c_display_/c_wg_/
  c_bigworld_ (0x180050000..0x14014C000 / 0x140152000..0x140154000),
  Track BB c_metal_* (0x18001C000..0x180024000), Track BB c_install_*
  (0x180030000..0x180032000), Track AQ/BB c_pe_* (0x140110000..0x140124000),
  Track BF (0x180040000..0x140144000) — collision check verified manually,
  RVA range 0x140160000-0x14016D000 is uniquely Track BM territory.
- **C5**: append goes between emit-count assertion and `cleanup_*()` gate
  (or between assertion and `let _idx_rm = exec("rm -f...` for c_iat_/c_dll_)
  — additive_only spirit preserved (no row removal, no self_test body
  modification, no main() change, no fn signature change). Existing
  emit-count assertion thresholds unchanged in each module — append occurs
  after assertion, so extra 2 emits per module are bonus (actual counts
  will exceed thresholds).
- **C6**: hexa-lang upstream candidates: 0 hits discovered in this cycle.
  Each module already uses hexa-native primitives (r0_emit / println /
  exec for awk/grep). No new gap surfaced. `docs/HEXA_UPSTREAM.md` NOT
  modified (Track BG parallel territory — race condition risk per user
  directive).
- **C7**: in_place_writes=9 (7 c_*.hexa + 2 .roadmap.* JSONL header
  surgical edits). additive_only spirit preserved across all artifacts.
  Both roadmaps: cond fields untouched verbatim except 1-2 evidence array
  +rows + 1-2 handoff field add + blocker_reason refresh additive.
  blk.1 fields untouched verbatim (forbidden scope held).
- **C8**: Track BM territory ban respected: NO Track G/U/AI/AK/BA c_d2r_*
  (26 — 100%) / NO Track V/AJ/AL/BA/BE c_d4_* (27 — 100%) / NO Track B
  c_purple_* / NO Track J/N c_bnet_* / NO Track B/V c_lineage_* / NO Track
  AS/BE c_elden_* / NO Track AT c_cs2_* / NO Track AR c_oauth_* / NO Track
  AR/BB c_install_* / NO Track AP/BB c_metal_* / NO Track AQ/BB c_pe_* /
  NO Track BF/BI c_macos_/c_fps_/c_dxr_/c_rt_/c_dxil_ / NO Track BF/BJ
  c_display_/c_wg_/c_wgc_/c_bigworld_ / NO lib/loader (Track BL parallel) /
  NO tool / NO native / NO tests / NO existing docs. blk.1 fields untouched.
- **C9**: docs/HEXA_UPSTREAM.md NOT modified (parallel BG/BC track territory —
  race condition risk per user directive). docs/airgenome_gamebox_perf_remaining_family_inventory_track_bf_landed_2026_05_05.ai.md
  read-only ref, NOT modified.

## §8. Predecessors

- `lib/perf/c_d2r_first_frame_trace.hexa` (Track G) — append pattern reference.
- `lib/perf/c_metal_capture_auto.hexa` (Track AP) — Apple-native synthetic round-trip pattern.
- `lib/perf/c_metal_simdgroup_reduce.hexa` (Track BB) — phase + matrix two-stage pattern.
- `lib/perf/c_pe_iat_simulation.hexa` (Track AQ) — PE loader synthetic round-trip pattern.
- `lib/perf/c_install_disk_space_check.hexa` (Track BB) — install path synthetic round-trip pattern.
- `lib/perf/c_macos_menubar_status_item.hexa` (Track BF) — multi-family inventory representative pattern.
- `lib/perf/c_bigworld_python_script_cache.hexa` (Track BJ) — direct precedent v1 cycle.
- `state/markers/airgenome_gamebox_perf_remaining_family_inventory_track_bf_landed.marker` (Track BF) — schema parent.
- `docs/airgenome_gamebox_perf_remaining_family_inventory_track_bf_landed_2026_05_05.ai.md` (Track BF) — direct predecessor for v2 cycle.

## §9. Next-cycle actions

- Track BL (this BM subagent's parallel) — lib/loader territory.
- Track BN+ — close c_wot_/c_wows_/c_iat_/c_dll_ families to 100% cumulative
  completion (each remaining 1-2 modules per family). Pattern reference: this
  Track BM representative module per family + Track BJ cumulative completion playbook.
- Track BO+ — augment .roadmap.battlenet (Wargaming cross-launcher) or
  .roadmap.loader_pe (PE x64 loader family) directly for c_iat_/c_dll_/c_wot_/c_wows_
  evidence rows.
- Production smoke remains gated on:
  - Real eldenring.exe binary + EAC ticket (permanent retired_intentional).
  - Real cs2.exe binary + VAC ticket (permanent retired_intentional).
  - BigWorld engine binary licence (permanent transitive — Wargaming long-horizon).
  - User network endpoint smoke (battlenet/Wargaming cond.3, multi-cycle long-horizon).
