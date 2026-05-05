---
schema: airgenome-gamebox/docs/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed.marker
  roadmap_files_modified:
    - .roadmap.lineage_classic_r28
    - .roadmap.diablo2_resurrected
  modules_modified:
    - lib/loader/pe_d2r_d3d11_renderer_init.hexa
    - lib/perf/c_d2r_first_frame_trace.hexa
    - lib/perf/c_d2r_dx11_pipeline_warm.hexa
  tests_created:
    - tests/test_d2r_d3d11_synthetic_scaffold.hexa
  predecessor_handoffs:
    - docs/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_purple_lineage_offline_shim_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_d4_retired_intentional_adoption_landed_2026_05_03.ai.md
status: LANDED_TRACK_G_FIELD_NORM_AND_D2R_COND_3_EVIDENCE_AUGMENT
related_raws:
  - raw 9    # hexa-only (verifier emit native via println __D2R_RENDERER__ / __D2R_TRACE__ / __D2R_WARM__ + r0_emit)
  - raw 10   # honest C3 caveats inline (>=8 numbered C1..Cn — including cond.3 met 영구 불가 explicit)
  - raw 11   # snake_case fields throughout JSONL + JSON marker
  - raw 12   # silent-error ban — every PARTIAL emit explicit (not swallowed as PASS)
  - raw 15   # env_lazy — repo-relative paths only, no /Users/<user>/ leaks
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes anywhere — paraphrase only)
preserved_unchanged:
  - .roadmap.diablo2_resurrected cond.1 (partial, Track D territory — 14 evidence rows untouched)
  - .roadmap.diablo2_resurrected cond.2 (met, Track A territory — 9 evidence rows untouched)
  - .roadmap.lineage_classic_r28 blk.1 fields other than status_enum_adopted value type (adopted_iso, adopted_via, adoption_evidence, status_enum_candidate, desc, status, eta, resolution_path, type, id all untouched)
  - .roadmap.lineage_classic_r28 cond.1/cond.2/cond.3 (untouched)
  - all 12 other .roadmap.<game/loader> files (battlenet, diablo4, wow, cs2, delta_force, elden_ring, lineage_m, lineage_w, purple_launcher, loader_pe, loader_win32, loader_dx)
  - all native sources, plugin.json, entry, README.md, .gitignore
  - all predecessor markers + predecessor handoff docs (read-only reference)
  - all c_* perf modules NOT touched by Track G
  - all loader modules NOT touched by Track G
  - all 6 existing test harness files (test_apple_only/test_closure/test_own2/test/test_d2r_archive_round_trip/test_d2r_pe_synthetic_round_trip/test_purple_lineage_offline_shim)
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 5
  in_place_writes_note: 3 .hexa modules self_test() append (no row removal, only append before final cleanup gate) + 1 .roadmap JSONL header surgical edit on .roadmap.diablo2_resurrected (cond.3 evidence array extension 4 -> 13 + blocker_reason refresh + ai_native_handoff_cond_3_synthetic_d3d11_scaffold new field) + 1 .roadmap JSONL header surgical edit on .roadmap.lineage_classic_r28 (single field type change blk.1.status_enum_adopted bool true -> string "retired_intentional", all other fields preserved verbatim) — 0 cond.N row deleted, append/extend only; cond.1 (partial) + cond.2 (met) untouched; additive_only spirit preserved (Track D predecessor in_place_writes=3 동급 posture, 본 cycle 측 G1 + G2 combined small-scope)
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# airgenome-gamebox Track G — field normalization (lineage_classic_r28 status_enum_adopted bool->string) + D2R cond.3 evidence augment landed (BG-AGB follow-up)

## §1. user_directive_paraphrase

BG subagent landing — Track G combined small-scope land for airgenome-gamebox repo. Two sub-missions:

- **G1**: Normalize `.roadmap.lineage_classic_r28` blk.1 `status_enum_adopted` field type from bool (Track B legacy format) to string ("retired_intentional"), matching the d4/cs2/df/er/wow string-format precedent established by Track A2 + Track C. Preserve all other blk.1 fields verbatim (adopted_iso, adopted_via, adoption_evidence, status_enum_candidate, desc, status, eta, resolution_path, type, id all untouched).
- **G2**: Mirror Track D's cond.1 evidence-augment patterning, but applied to `.roadmap.diablo2_resurrected` cond.3 ("D3D11 renderer init + first-frame draw + dx11 pipeline warm"). cond.3 stays `partial` permanently (NOT met promotion) because real first-frame GPU submit is gated on loader_dx blk.1 D3DMetal binary licence — a permanent transitive blocker, no DRM/licence circumvention attempted.

Three D2R modules extended with synthetic D3D11 scaffold rows in their self_test() functions:
- `lib/loader/pe_d2r_d3d11_renderer_init.hexa` — synthetic device_init + swapchain_create + first_frame_submit_simulated 3 stage scaffold + `__D2R_RENDERER__ PARTIAL <stage>` emits.
- `lib/perf/c_d2r_first_frame_trace.hexa` — synthetic frame_0 instrumentation (frame_id 0 + render_command_count + GPU submit ts synthesis) + `__D2R_TRACE__ PARTIAL frame_0_synthetic` emit.
- `lib/perf/c_d2r_dx11_pipeline_warm.hexa` — synthetic pso_priority + srv_binding 2 stage scaffold + `__D2R_WARM__ PARTIAL <stage>` emits.

New artifacts:
- `tests/test_d2r_d3d11_synthetic_scaffold.hexa` — Track G test harness (mirror of Track D `test_d2r_pe_synthetic_round_trip.hexa` style, drives the 3 modules' new self_test rows, r0_emit ≥ 4).
- This handoff doc.
- `state/markers/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed.marker` — sha256-pinned marker with predecessors array.

Policy enforced: additive_only / migration forbidden / destructive 0 / cost $0 / mac-local / cap 90min / BR-NO-USER-VERBATIM / friendly_preset / silent_land / own1 (Wine 0, hexa-only, no DRM bypass, no D3DMetal binary linkage attempt — synthetic byte arrays inline only) / own2 (log/error/emit/headless honest pattern, PARTIAL emits explicit, NO swallow as PASS) / raw 9 / 11 / 12 / 15 / 175 enforce.

## §2. G1 field normalization table (before/after type)

| Aspect | Before (Track B legacy) | After (Track G normalization) | Reference (Track A2 + Track C exemplar) |
|---|---|---|---|
| File | `.roadmap.lineage_classic_r28` | same | `.roadmap.diablo4`, `.roadmap.cs2`, `.roadmap.delta_force`, `.roadmap.elden_ring`, `.roadmap.wow` |
| Path within JSONL | `blockers[0].status_enum_adopted` | same | same path in d4/cs2/df/er/wow |
| Value type | `bool` (Python `True`) | `str` | `str` |
| Value | `true` | `"retired_intentional"` | `"retired_intentional"` |
| `status_enum_candidate` (sibling field) | `"retired_intentional"` (str, untouched) | `"retired_intentional"` (str, untouched) | `"retired_intentional"` |
| `adopted_iso` (sibling field) | `"2026-05-03"` (str, untouched) | `"2026-05-03"` (str, untouched) | `"2026-05-03"` |
| `adopted_via` (sibling field) | `"track_b_purple_lineage_offline_shim_landed"` (str, untouched) | `"track_b_purple_lineage_offline_shim_landed"` (str, untouched) | varies per game |
| `adoption_evidence` (sibling field) | 3 rows (untouched) | 3 rows (untouched) | varies per game |

Verification (Python json):
```
python3 -c "import json; h=[json.loads(l) for l in open('.roadmap.lineage_classic_r28') if l.strip() and not l.startswith('#')][0]; print(type(h['blockers'][0]['status_enum_adopted']).__name__)"
# -> str
```

## §3. G2 evidence augment table (cond.3 evidence count before/after)

| Aspect | Before | After | Delta |
|---|---|---|---|
| `.roadmap.diablo2_resurrected` cond.3 status | `partial` | `partial` | unchanged (intentional — met 승격 영구 X, loader_dx blk.1 D3DMetal binary licence transitive) |
| cond.3 evidence row count | 4 | 13 | +9 |
| cond.3 blocker_reason | "first-frame gpu submit 실측 미수행 ..." | "synthetic D3D11 scaffold landed (...) — real first-frame GPU submit gated on loader_dx blk.1 D3DMetal binary licence ..." | refreshed (acknowledges synthetic scaffold landed + real GPU gap permanent) |
| cond.3 `ai_native_handoff_cond_3_synthetic_d3d11_scaffold` (new field) | absent | `"docs/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed_2026_05_03.ai.md §3"` | added |
| cond.1 status / evidence count | `partial` / 14 | `partial` / 14 | unchanged (Track D territory) |
| cond.2 status / evidence count | `met` / 9 | `met` / 9 | unchanged (Track A territory) |
| Module emit families added | 0 | 6 (`__D2R_RENDERER__` x3 + `__D2R_TRACE__` x1 + `__D2R_WARM__` x2) | +6 |

## §4. Module diff summary (LOC delta per file)

| File | Before LOC | After LOC | Delta LOC | Section appended |
|---|---:|---:|---:|---|
| `lib/loader/pe_d2r_d3d11_renderer_init.hexa` | 442 | 493 | +51 | synthetic_d3d11_renderer_scaffold (12-A device_init / 12-B swapchain_create / 12-C first_frame_submit_simulated / 12-D summary) |
| `lib/perf/c_d2r_first_frame_trace.hexa` | 150 | 172 | +22 | synthetic_first_frame_instrumentation (frame_0_synthetic single emit) |
| `lib/perf/c_d2r_dx11_pipeline_warm.hexa` | 128 | 162 | +34 | synthetic_pipeline_warm (pso_priority + srv_binding 2 emit) |
| `tests/test_d2r_d3d11_synthetic_scaffold.hexa` | 0 (new) | 224 | +224 | full harness (check_renderer_scaffold + check_first_frame_trace + check_pipeline_warm + emit_count_audit + chain_done) |
| `.roadmap.diablo2_resurrected` | 5 (single JSONL header line) | 5 | 0 | header line bytes 8227 -> 10802 (+2575 bytes inline cond.3 evidence + blocker_reason + new field) |
| `.roadmap.lineage_classic_r28` | 5 | 5 | 0 | header line bytes 6248 -> 6265 (+17 bytes — single field bool->string normalization) |
| `docs/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed_2026_05_03.ai.md` | 0 (new) | this file | +this file | full handoff doc (§1-§7) |
| `state/markers/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed.marker` | 0 (new) | marker file | +marker | sha256-pinned marker with predecessors array |

Total LOC growth in 3 modified `.hexa` modules: +107 LOC.

## §5. Synthetic vs real GPU gap (loader_dx blk.1 transitive caveat)

**The synthetic D3D11 scaffold is NOT a substitute for real GPU first-frame submit.** Track G's evidence augment lands inline byte-array stubs that simulate the shape of D3D11 init / DXGI swapchain / first-frame command submit / PSO compile cache priority / SRV descriptor binding — but no real GPU driver call is made. Specifically:

| Layer | Synthetic (Track G) | Real (would-be, gated on loader_dx blk.1) |
|---|---|---|
| ID3D11Device CreateDevice | Inline byte stub (adapter_count 1, feature_level 0xB000 marker) | D3DMetal binary translates D3D11 -> Metal MTLDevice |
| IDXGISwapChain1 Create | Inline byte stub (buf_count 2, format 0x0000001C, 1280x720) | DXGI swapchain backed by CAMetalLayer via D3DMetal |
| ID3D11DeviceContext::ExecuteCommandList (first frame) | Inline byte stub (cmd_list_count 1, gpu_submit_ts_ns 0 placeholder) | Real GPU submit via Metal command queue, real ts |
| PSO compile cache | Inline byte stub (pso_compile_cache_off 0x140074000, no real compile) | D3DCompile -> DXBC -> D3DMetal -> Metal pipeline state |
| SRV descriptor binding | Inline byte stub (srv_descriptor_count 12 marker) | PSSetShaderResources -> Metal argument buffer binding |
| GPU submit timestamp | 0 (synthetic) | Real CAMetalLayer present timestamp (nanoseconds since epoch) |

The permanent transitive blocker is **loader_dx blk.1 D3DMetal binary licence** — Apple/community D3DMetal binary cannot be linked or shimmed into this codebase under own1 boundary (no DRM bypass, no licence circumvention, no GPU driver shim attempt). Therefore cond.3 status `partial` is locked permanently; only evidence rows accumulate via synthetic scaffold landings (this cycle) or future cycles (e.g., real first-frame Present round-trip when D3DMetal licence resolved upstream).

This is identical posture to Track D cond.1 (real D2R.exe binary smoke gated on Battle.net launcher install) — both are honest acknowledgments of permanent external dependencies, not fake `met` promotions.

## §6. Caveats (>=8 honest items)

- **C1**: D2R cond.3 status is permanently `partial` for the lifetime of the loader_dx blk.1 D3DMetal binary licence transitive blocker — `met` promotion is intentionally never attempted in this cycle and will not be attempted in any future synthetic-only cycle. Promotion path (if any) requires real GPU submit timestamp via real D3DMetal binary, gated on upstream Apple/community licence resolution outside this codebase's own1 boundary.
- **C2**: All synthetic byte arrays in the 3 modified modules are inline JSON-string emits referencing hex offsets/values. No actual D3D11 / DXGI / Metal binary is parsed, linked, or invoked. CrossOver / Whisky / GPTK / community wrapper / D3DMetal binary linkage = 0 lines (own1 strict).
- **C3**: cond.3 evidence row count (4 -> 13, +9) is an *augment* not a *promotion*. Track A cond.2 met promotion (synthetic + opensource MPQ-listfile-min byte-equal) is a distinct pattern — that path is open to cond.2 because synthetic round-trip is byte-equivalent to opensource sample. cond.3 has no opensource D3D11-ish first-frame sample to byte-compare against, so synthetic is *evidence-only*, never promotion-eligible.
- **C4**: G1 field normalization (`status_enum_adopted` bool -> string) is a single-field type change at JSONL header level. All other blk.1 fields (adopted_iso, adopted_via, adoption_evidence, status_enum_candidate, desc, status, eta, resolution_path, type, id) preserved verbatim per scope guard. cond.1/cond.2/cond.3 of lineage_classic_r28 untouched.
- **C5**: Track B (predecessor) chose bool format because `status_enum_adopted` was modeled as a "boolean adoption flag" sibling to `status_enum_candidate` string. Track A2 + Track C standardized on string format (value duplicates `status_enum_candidate`), making downstream consumers able to read `status_enum_adopted` as the SSOT enum directly. Track G aligns lineage_classic_r28 with the latter convention. No data is lost — bool true with candidate "retired_intentional" maps unambiguously to string "retired_intentional".
- **C6**: silent_error_ban (raw 12) preserved — every new self_test PARTIAL emit (`__D2R_RENDERER__` x3, `__D2R_TRACE__` x1, `__D2R_WARM__` x2 = 6 total) is explicit PARTIAL, never swallowed as PASS. Module self_test PASS gates remain at their original assertion thresholds (renderer step_count=8, first_frame milestones=8, dx11_warm shaders=10) — Track G evidence appends emit AFTER those assertions but BEFORE cleanup, so a regression in synthetic emits surfaces immediately at next self_test run via grep_emit() in the new test harness.
- **C7**: Track G test harness (`tests/test_d2r_d3d11_synthetic_scaffold.hexa`, 224 LOC) mirrors Track D predecessor (`tests/test_d2r_pe_synthetic_round_trip.hexa`, 197 LOC) one-to-one in structure: r0_emit dispatch via session_set, child hexa spawn + grep_emit fallback, panic-on-fail with explicit expected/got, main() dispatch on `self-test` / `status` subargs. r0_emit ≥ 4 enforced (track_g_check_renderer_scaffold + track_g_check_first_frame_trace + track_g_check_pipeline_warm + track_g_emit_count_audit + track_g_chain_done = 5).
- **C8**: in_place_writes = 5 (3 .hexa append + 2 .roadmap JSONL surgical edits) is up from Track D in_place_writes = 3, but additive_only spirit preserved — 0 row/field deletion, only append/extend on .hexa modules and field-level edit (1 type change + 1 evidence array extend + 1 blocker_reason refresh + 1 new ai_native_handoff field) on roadmaps. Both roadmap edits stay strictly within scope guard (lineage_classic_r28: blk.1.status_enum_adopted only; diablo2_resurrected: cond.3 only, cond.1/cond.2 untouched).
- **C9**: BR-NO-USER-VERBATIM (raw 175) — handoff §1 paraphrases the user directive only. No verbatim Korean user quotes appear in any of the 5 created/modified files (handoff doc + marker + 3 modules + test harness + 2 roadmaps). Marker `user_directive_paraphrase` field is paraphrase only.
- **C10**: own2 honest emit posture — module self_test `println(" ✅ ...PASS ...")` lines preserved verbatim (Track G evidence appends do NOT remove or modify any existing emit / println / panic). Existing self_test assertions (step_count, total_resource_count, etc.) execute first; new synthetic emits land at end before cleanup gate. This means a Track G regression cannot mask a pre-existing module fault (own2-compliant ordering).
- **C11**: synthetic byte offsets (0x140070000 / 0x140072000 / 0x140074000 / 0x140076000) are illustrative scaffolding RVA — they extend the Track D synthetic RVA layout (0x140050000 bnet_marker / 0x14005A000 .pdata / 0x14006C000 .rsrc) into the D3D11 renderer surface (0x14007XXXX cluster). No real D2R.exe RVA is observed, parsed, or patched.
- **C12**: marker schema mirrors Track D predecessor schema verbatim — `airgenome-gamebox/markers/track_g_field_norm_d2r_cond3_landed/1`. predecessors array references Track D + Track B + per-game-roadmaps + d4 retired_intentional adoption (the four immediate predecessors that establish (a) cond.3 evidence augment patterning, (b) lineage_classic_r28 bool-format origin, (c) per-game roadmap fan-out, (d) string-format exemplar for status_enum_adopted).

## §7. next-cycle hooks

- diablo2_resurrected.cond.3 real first-frame GPU submit (loader_dx blk.1 D3DMetal binary licence resolve transitive — no synthetic-to-real shortcut available).
- diablo2_resurrected.cond.3 real PSO compile via D3DCompile -> DXBC -> Metal pipeline state (gated on D3DMetal licence).
- diablo2_resurrected.cond.3 real SRV descriptor heap warm via PSSetShaderResources -> Metal argument buffer (gated on D3DMetal licence).
- Track-H candidate — extend cond.3 synthetic scaffold to pe_d2r_specific_track style D3D11 surface tracker (e.g., synthetic vendor query + adapter description + IDXGIFactory enumeration), partial -> partial++ patterning continues.
- Track-I candidate — apply Track G G1 field normalization sweep to any other roadmap that may still hold `status_enum_adopted: bool` legacy format (audit script: `for f in .roadmap.*; do python3 -c "import json,sys; ..."; done`). Currently lineage_classic_r28 was the last known offender; sweep validates none remain.
- Track-J candidate — diablo2_resurrected per-game README.ai.md emit (T1-large bucket promotion-day candidate 2026-06-01).
- own2 enforcement entry-too-large + headless first_dialog 매칭 측 2 fail resolve cycle (pre-existing inherited from predecessor cycles, 본 cycle 0 신규 fail).
- Track-K candidate — cross-game cond evidence audit script (compare cond.3 evidence row count across d2r / d4 / cs2 / etc to surface coverage gaps).
