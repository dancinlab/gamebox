# airgenome-gamebox — closure_orchestrator checkpoint awareness integration landed (Track AD, cycle 3 of 3) — handoff

`schema:` airgenome-gamebox/handoff/closure_orchestrator_checkpoint_integration_landed/1
`completion:` 2026-05-04
`predecessors:` Track AC (`airgenome/doc/airgenome_gamebox_phase_1_3_checkpoints_landed_2026_05_04.ai.md`) phase 1 checkpoints land + Track AB (`airgenome/doc/airgenome_gamebox_roadmap_op_checkpoint_subcmd_extension_landed_2026_05_04.ai.md`) tool extension + Track AA (`airgenome/doc/airgenome_gamebox_mk2_checkpoint_schema_design_landed_2026_05_04.ai.md`) design lock-in + Track S original (`airgenome/doc/airgenome_gamebox_closure_orchestrator_landed_2026_05_03.ai.md`) closure_orchestrator genesis
`policy:` migration `forbidden`, changes `additive_only` (1 EXTEND + 2 NEW; 0 file deletion / no body removal), in_place_writes 1 (tool/closure_orchestrator.hexa extended additively from 526 LOC → 875 LOC), destructive_ops 0, cost_usd 0, substrate `mac-local`, cap_minutes 90, BR_NO_USER_VERBATIM, friendly_preset, silent-land, own1 hexa-only Wine 0, own2 honest-emit silent_error_ban, raw 9 / 11 / 12 / 15 / 175

---

## §1 — directive paraphrase (BR-NO-USER-VERBATIM)

Track-AD is cycle 3 of 3 in the mk2 checkpoint mechanism implementation chain. Track-AA (cycle 0, design lock-in across 4 D options), Track-AB (cycle 1, `tool/roadmap_op.hexa` extended 11 → 14 subcmd), and Track-AC (cycle 2, 3 phase-1 `.checkpoint.<name>` SSOT files landed at repo root) closed the predecessor cycles. This Track-AD cycle integrates checkpoint awareness into `tool/closure_orchestrator.hexa` (Track S, originally 526 LOC), extending its 3 existing subcmd (status / list / emit) and self_test additively. The closure_pct calculation is extended with a checkpoint-progress contribution formula, but per own2 honest disclosure, the current effective closure_pct stays at 25% (no checkpoints derive `met` today; all 3 phase-1 checkpoints carry `status: "partial"`). The tool reads `.checkpoint.<name>` SSOT files directly via own helpers; it does NOT exec `tool/roadmap_op.hexa` checkpoint subcmd from inside closure_orchestrator (design-level integration, not runtime delegation). This Track-AD cycle closes the third milestone of the 3-cycle minimum-viable plan; the mk2 checkpoint minimum-viable surface is now complete (`mk2_checkpoint_minimum_viable_complete: true`).

---

## §2 — 3 subcmd extension table (status / list / emit)

| subcmd | extension shape | new fields / output | LOC delta | preserved verbatim |
|---|---|---|---|---|
| `status` | append checkpoint awareness fields after existing 9 emit lines + before existing `__CLOSURE__ PASS status` line | `checkpoint_count: <N>`; `checkpoint_reached: <M>/<N>` (or `0/0` when N==0); `checkpoint_pct: <M*100/N>%` (or `N/A` when N==0); `closure_pct_breakdown: { base, checkpoint_contribution, total }`; `checkpoint_schema_version: airgenome-gamebox/checkpoint/mk2/header/1`; new emit line `__CLOSURE__ PASS status closure_pct=<X>% cm_ratio=<R>/30 checkpoint_reached=<M>/<N>` (precedes the existing emit line) | +12 lines | existing 9 println lines + existing `__CLOSURE__ PASS status pct=...` emit line are byte-for-byte preserved |
| `list` | append `--- CHECKPOINTS ---` section after existing CM-0..CM-30 enumeration, with one tab-separated line per `.checkpoint.<name>` file: `CHECKPOINT\t<name>\t<status>\trefs=<N>` | new emit line `__CLOSURE__ PASS list shown=<combined_total> done=<combined_done> cm_done=<cm_done_cnt> checkpoint_reached=<M>/<N>` (precedes existing emit line) | +25 lines | existing CM-0..CM-30 while-loop emit + existing `__CLOSURE__ PASS list shown=...` line byte-for-byte preserved |
| `emit` | extend `build_progress_marker` JSON body with new fields: `checkpoint_count`, `checkpoint_reached`, `checkpoint_schema_version`, `closure_pct_calculation` (formula), `closure_pct_breakdown`, `checkpoint_snapshot[]` (one entry per `.checkpoint.<name>`: `{name, status, cond_refs_count}`), `cross_ref_envelope: "track_z_compliant_optional_additive"`. Subcmd emit body adds new emit line `__CLOSURE__ PASS emit marker=<path> closure_pct=<X> cm_ratio=<R>/<T> checkpoint_reached=<M>/<N>` (precedes existing emit line) | +20 lines (marker body) + 8 lines (subcmd body) | existing 22 marker JSON fields preserved verbatim; existing `__CLOSURE__ PASS emit marker=... pct=...` emit line preserved; same-second collision guard preserved |

`build_checkpoint_snapshot_json` is a NEW helper (19 LOC) emitting the `checkpoint_snapshot[]` array body. It is invoked only inside `build_progress_marker` and is not on any pre-existing call path.

---

## §3 — closure_pct calculation extension formula

### §3.1 — formula

```
closure_pct = min(CHECKPOINT_PCT_MAX, CHECKPOINT_BASE_PCT + checkpoint_reached * CHECKPOINT_WEIGHT)
```

Constants pinned in the source file (top-level, with explicit comments):

- `CHECKPOINT_BASE_PCT = 25` (pinned to `BASELINE_CLOSURE_PCT`, cycle 37 skeleton 6/6 × 25 score). Reviewable post phase 2.
- `CHECKPOINT_WEIGHT = 5` (per-checkpoint contribution). Phase 2 (Track AE+) introduces `weighted_threshold` for `win32_*pct` candidates; this constant should be reviewed at that point.
- `CHECKPOINT_PCT_MAX = 100` (closure_pct hard cap; never inflate beyond 100%).

### §3.2 — current state honest disclosure (own2 mandate)

| candidate | header `status` | derived (Track AB AND_all_required_met) | reached? |
|---|---|---|---|
| `d2r_battlenet_offline_play` | `partial` (cached) | `partial` (3/7 satisfied) | no |
| `lineage_w_purple_play` | `partial` (cached) | `partial` (3/5 satisfied) | no |
| `lineage_classic_r28_play` | `partial` (cached) | `partial` (0/3 satisfied) | no |

`checkpoint_reached_current = 0`, `checkpoint_total_current = 3`.

`closure_pct = min(100, 25 + 0 × 5) = 25` (no change from cycle 37 baseline). This is honest per own2: the calculation is EXTENDED to support future progression, but the current value is NOT inflated.

### §3.3 — Track AD scope vs. Track AB derivation

`count_checkpoint_reached` evaluates each entry's CACHED header `status` field (== "met"). The full AND_all_required_met derivation across `.roadmap.<X>` cond_refs is Track AB territory (`tool/roadmap_op.hexa checkpoint-status`); Track AD intentionally does NOT exec the roadmap_op subcmd from inside closure_orchestrator. The result: derived-met (which would require live cond status reads across every referenced `.roadmap.<X>`) is fronted by the cached header field. Today the cached value matches the Track AB derived value (all 3 partial); future drift is honest caveat C5.

---

## §4 — self_test S6-S8 additions (additive — S1-S5 preserved verbatim)

| step | intent | hard-FAIL conditions | informational |
|---|---|---|---|
| **S6** | `enumerate_checkpoint_files()` count sanity | `len < 0` (defensive) | production count = 3 (Track AC phase 1 land); count == 0 also PASS (pre-Track-AC posture) |
| **S7** | `parse_checkpoint_header(first_path)` | `name == ""`, `cond_refs_count <= 0`, `status == ""` (when count > 0) | skipped if count == 0 (`r0_emit` `S7_parse_skipped` instead of FAIL) |
| **S8** | `count_checkpoint_reached` + `compute_closure_pct` range sanity | `reached < 0`, `reached > total`, `compute_closure_pct` out of `[0, CHECKPOINT_PCT_MAX]` | precise `reached` value informational (today 0; future cycles flip as conds converge) |

Final emit lines (additive both):

```
__CLOSURE__ PASS self-test all_8_steps
__CLOSURE__ PASS self-test all_5_steps
```

Existing `all_5_steps` line preserved verbatim (mirror Track S round-trip semantic), new `all_8_steps` line precedes it.

---

## §5 — implementation_status flip table

| track | cycle | implementation_status | landed in this cycle? |
|---|---|---|---|
| Track AA | cycle 0 | `design_only_pending_user_lock_in` | no (predecessor) |
| Track AB | cycle 1 of 3 | `cycle_1_of_3_tool_extension_landed` | no (predecessor) |
| Track AC | cycle 2 of 3 | `cycle_2_of_3_phase_1_checkpoints_landed` | no (predecessor) |
| **Track AD** | **cycle 3 of 3** | **`cycle_3_of_3_closure_orchestrator_integration_landed`** | **YES (this cycle)** |
| Track AE+ (pending) | phase 2+ | `phase_2_weighted_threshold_and_remaining_4_candidates_pending` | no |

The implementation_status field flip from `cycle_2_of_3_phase_1_checkpoints_landed` (Track AC marker) to `cycle_3_of_3_closure_orchestrator_integration_landed` (this Track AD marker) closes the third and final milestone of the 3-cycle minimum-viable plan locked in at Track AA. The marker also pins `mk2_checkpoint_minimum_viable_complete: true` — the mk2 checkpoint mechanism is now end-to-end usable: SSOT files land at repo root (Track AC), the tool layer validates and derives status (Track AB), and the closure tracker surfaces aggregate progress alongside the existing CM-0..CM-30 view (Track AD this cycle).

---

## §6 — Track Z cross_ref envelope alignment confirmation

`docs/CROSS_TOOL_INTEGRATION_DESIGN.md` (Track Z mk1 narrative) proposed a common emit envelope `__<TOOL>__ <verdict> <subcmd> <reason> [<cross_ref>]` with `[<cross_ref>]` as an OPTIONAL backward-compatible extension field. Track AD complies as follows:

- The `subcmd_emit` marker JSON body adds the new field `cross_ref_envelope: "track_z_compliant_optional_additive"` declaring alignment. The EXISTING marker JSON fields (schema, completion_iso, emit_unix_ts, emitter, emitter_version, closure_pct, cm_done_index, cm_total, cm_ratio, skeleton_manjeom, validated_manjeom, baseline_cycle, snapshot_source, ssot_closure_roadmap, ssot_d2r_d4_roadmap, policy{}, own1, own2, raw_compliance) are preserved byte-for-byte. The new fields (`checkpoint_count`, `checkpoint_reached`, `checkpoint_schema_version`, `closure_pct_calculation`, `closure_pct_breakdown`, `checkpoint_snapshot[]`, `cross_ref_envelope`) are appended after the existing fields, before the closing brace.
- The new `__CLOSURE__ PASS status closure_pct=<X>% cm_ratio=<R>/30 checkpoint_reached=<M>/<N>` and `__CLOSURE__ PASS list shown=<combined> done=<combined> cm_done=<X> checkpoint_reached=<M>/<N>` and `__CLOSURE__ PASS emit marker=<P> closure_pct=<X> cm_ratio=<R>/<T> checkpoint_reached=<M>/<N>` lines are PREPENDED before the existing `__CLOSURE__ PASS <subcmd> ...` emit lines. Existing emit consumers continue to work unchanged (they will simply see two PASS lines per subcmd; the second matches the legacy format byte-for-byte).
- The `cross_ref` field convention (free-form key=value space-separated per Track Z §3.3) is honored at the `__CLOSURE__ PASS <subcmd> ... checkpoint_reached=M/N closure_pct=X` shape — `closure_pct=X` and `checkpoint_reached=M/N` ARE the cross_ref payload.

No schema strict-freeze is attempted (Track Z C9 caveat). Future strict schema for `cross_ref` field remains a separate Track Z+ design cycle.

---

## §7 — caveats (honest C3 inline; ≥10 items)

- **C1** — `count_checkpoint_reached` evaluates the CACHED header `status` field (== "met"). The full AND_all_required_met derivation across referenced `.roadmap.<X>` cond_refs is Track AB territory and is NOT executed from inside closure_orchestrator. Today the cached value matches the Track AB derived value for all 3 phase-1 checkpoints (all partial); a future drift between cached and live-derived statuses would NOT be caught by this tool — Track AB `checkpoint-status` would need to be invoked separately. This is a deliberate scope limit for Track AD.
- **C2** — `closure_pct` stays at 25% in this cycle. The calculation is EXTENDED to support progressive progression, but with `checkpoint_reached_current=0`, `checkpoint_contribution = 0 × 5 = 0`, total = 25 + 0 = 25. This is honest per own2 — closure_pct is NOT inflated despite the checkpoint mechanism being end-to-end live.
- **C3** — `CHECKPOINT_WEIGHT = 5` is a designable constant. With 3 phase-1 checkpoints and 4 phase-2 deferred (7 candidates total), max possible contribution from candidates would be 7 × 5 = 35 → capped 25 + 35 = 60. The `CHECKPOINT_PCT_MAX = 100` cap is well clear of this, but a future weighted_threshold scheme (Track AE+) for `win32_*pct` candidates may want a higher per-checkpoint weight or a fractional formula.
- **C4** — The checkpoint-aware emit fields are appended AFTER the existing fields in the marker JSON body, before the closing brace. Existing JSON parsers (e.g., `python3 -m json.tool`) continue to parse the body successfully. Backward compatibility is protocol-level (mirror Track Z §3.2 / C3 caveat).
- **C5** — Existing `__CLOSURE__ PASS <subcmd> ...` legacy emit lines are PRESERVED byte-for-byte and emitted ALONGSIDE the new checkpoint-aware emit lines. Each subcmd now emits TWO PASS lines (legacy first kept second, new first). Strict line-counting consumers may see an unexpected duplicate; strict schema parsers should select the desired line by reason fragment (e.g., `closure_pct=` vs. `pct=`).
- **C6** — `parse_checkpoint_header` uses a tiny local string scanner rather than reusing `tool/roadmap_op.hexa` `read_str_field`. This is intentional — closure_orchestrator imports only `../lib/perf/r0_common`, NOT roadmap_op (which is itself a separate top-level tool, not an importable module). Code duplication is acknowledged; refactoring to share helpers is a separate cycle (likely Track AG+).
- **C7** — `enumerate_checkpoint_files()` invokes `ls -1 .checkpoint.* 2>/dev/null` (mirror of Track AB `list_checkpoints_dir`). When invoked from a non-repo-root cwd, the `ls` glob will return empty and the tool will report `count=0` (PASS path). This matches the closure_orchestrator existing posture (all paths repo-relative; cwd assumed to be repo root). Caveat: invoking from elsewhere yields silent zero rather than a hard FAIL.
- **C8** — `build_checkpoint_snapshot_json` emits raw string interpolation for the `name` and `status` fields. Both fields are validated upstream (Track AB `checkpoint-verify` (a) header line shape, (g) status enum, (h) name-filename match), so injection is not a runtime concern in this tool's scope. A defensive escape pass would be a future strict-mode hardening cycle.
- **C9** — `subcmd_status`, `subcmd_list`, `subcmd_emit` fn bodies are EXTENDED additively (new lines appended before the existing terminal emit + return, and within `build_progress_marker` before the closing brace). The existing lines and code paths inside each fn are byte-for-byte preserved. This satisfies the spec mandate "PRESERVE existing 3 subcmd + self_test fn bodies — extension only adds new helpers + new self_test steps + new dispatch logic for checkpoint awareness within existing subcmd output".
- **C10** — The Track AD self_test extension adds 3 new steps (S6 / S7 / S8) and prints two PASS lines on completion (`all_8_steps` new, `all_5_steps` legacy). S5 internal structure remains unchanged.
- **C11** — The marker JSON body uses 2-space indent for the existing fields and 4-space indent for the existing `policy{}` nested block. New fields follow the existing 2-space convention. No reformat of existing fields.
- **C12** — Anima parity break (Track AB acknowledgment carried) remains broken intentional — this Track AD cycle does NOT touch `tool/roadmap_op.hexa`, parity status unchanged from Track AB.
- **C13** — `CHECKPOINT_BASE_PCT` is hardcoded to `25` (matches `BASELINE_CLOSURE_PCT`). The two constants are intentionally kept separate (different review cadences): `BASELINE_CLOSURE_PCT` syncs with cycle 37 SSOT; `CHECKPOINT_BASE_PCT` is the formula base, reviewable independently when phase 2 weighted_threshold lands.
- **C14** — The closure_orchestrator `subcmd_status` does NOT recompute `closure_pct` for the existing top-line emit (`closure_pct: 25%`). The line is preserved verbatim from Track S. The new `closure_pct_breakdown` line and the new `__CLOSURE__ PASS status closure_pct=...` emit line surface the EXTENDED calculation. Today the two values are equal (25 == 25); future cycles where checkpoints reach met will see them diverge until the SSOT closure_pct dashboard is bumped manually (caveat C13 from Track S — `BASELINE_*` SSOT sync cycle pending).
- **C15** — The `checkpoint_snapshot[]` JSON array in the marker body lists `name` / `status` / `cond_refs_count` per checkpoint. This is intentionally minimal — `blockers` / `predecessors` / `aggregation` / `goal` fields are NOT replicated (they live in the `.checkpoint.<name>` SSOT files, which can be queried via Track AB `checkpoint-list`). The marker is a SNAPSHOT, not a clone.
- **C16** — `mk2_checkpoint_minimum_viable_complete: true` flag in the marker affirms end-to-end live state of the 3-cycle minimum-viable plan. It does NOT mean: phase 2 is done (it isn't), production smoke gates pass (no checkpoints reached), or the closure_pct has progressed (it stays 25%). The flag is scoped strictly to the 3-cycle minimum-viable surface specified at Track AA.

---

## §8 — next-cycle hooks

- **Track AE+ phase 2** — Land 4 phase 2 checkpoint candidates (`d2r_battlenet_online_play` / `lineage_m_purple_play` / `win32_50pct_coverage` / `win32_full_coverage`). Design `weighted_threshold` aggregation for `win32_*pct` candidates (per Track AA caveat C9). Likely separate Track AE / AF / AG pending user lock-in. The Track AD closure_orchestrator integration will surface these new checkpoints automatically (no closure_orchestrator changes required) once the `.checkpoint.<name>` files land at repo root.
- **Track AF (potential)** — `retired_intentional` cond-status cascading flip cycle: extend `tool/roadmap_op.hexa update --blocker` to optionally cascade `status_enum_adopted` down to `cond.<id>.status`. Required for `.checkpoint.lineage_classic_r28_play` to derive `met` via sideways-PASS. Pending separate user lock-in.
- **Track AG (potential)** — `BASELINE_CLOSURE_PCT` / `CHECKPOINT_BASE_PCT` SSOT sync cycle: when cycle 38+ docs/CLOSURE_ROADMAP.md / docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md update closure_pct, sync both constants in `tool/closure_orchestrator.hexa`. The two constants currently match (25 == 25); diverge if a future cycle bumps SSOT closure_pct without recomputing the formula base.
- **Track AH (potential)** — closure_orchestrator + `tool/roadmap_op.hexa` `checkpoint-status` runtime delegation: invoke `checkpoint-status` per checkpoint from inside `subcmd_status` to surface DERIVED (live) status instead of CACHED header status. Today Track AD prefers cached for performance + scope simplicity; production smoke after weighted_threshold lands may motivate a delegation cycle.
- **Production smoke gates** — None in this cycle (mirror Track AC posture). The hexa CLI runtime smoke of `tool/closure_orchestrator.hexa` self-test is gated on `/Users/ghost/.hx/bin/hexa` availability + a separate test cycle. This Track AD cycle is design-level integration + schema-validate-only.
- **Marker chain progressive update** — The Track H/Y/Z/AA/AB/AC/AD marker chain `implementation_status` field forms an observable trail: decision-only → tool-extension → phase 1 SSOT → closure integration → phase 2 weighted-threshold (pending). Track AD marker closes the third link.

---

## §9 — file index

| path | role | LOC | sha256 | delta |
|---|---|---|---|---|
| `tool/closure_orchestrator.hexa` | extended additively (Track AD cycle 3 of 3) | 875 | `713db687abe02999792b1c5b99cd806233bce86f68d2c0c3ea83f1239fa8d6c0` | +349 from Track S 526 LOC baseline |
| `airgenome/doc/airgenome_gamebox_closure_orchestrator_checkpoint_integration_landed_2026_05_04.ai.md` | NEW handoff (this file) | (self) | (self) | NEW |
| `state/markers/airgenome_gamebox_closure_orchestrator_checkpoint_integration_landed.marker` | NEW marker | (sibling) | (sibling) | NEW |

---

end of handoff.
