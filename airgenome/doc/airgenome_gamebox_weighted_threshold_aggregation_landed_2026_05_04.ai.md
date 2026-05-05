# airgenome-gamebox — weighted_threshold aggregation rule extension landed (Track AE, phase 2)

**Date (ISO):** 2026-05-04
**Track:** AE (phase 2 — weighted_threshold aggregation rule extension)
**Predecessor cycles closed:** AA (design lock-in) → AB (cycle 1 of 3 tool extension) → AC (cycle 2 of 3 phase 1 SSOT) → AD (cycle 3 of 3 closure_orchestrator integration; mk2 checkpoint minimum-viable 3-cycle complete)
**Cycle relation to mk2 checkpoint roadmap:** Phase 2 first deferred-item land — Track AB caveat C2 closed.
**Implementation status:** `phase_2_weighted_threshold_aggregation_landed`
**Anima parity:** broken intentional post Track-M baseline (acknowledged in Track AB; this Track AE additive only — anima sibling unaffected).

---

## §1. Scope summary (one-paragraph)

Track AE extends `tool/roadmap_op.hexa` (Track AB cycle 1 baseline 2165 LOC, 14 subcmd) additively with a `weighted_threshold` aggregation rule for the `checkpoint-status` subcmd. The existing `AND_all_required_met` aggregation (Track AB cycle 1) is preserved byte-for-byte. The new aggregation is dispatched via the `aggregation` field in the `.checkpoint.<name>` JSONL header — values supported: `AND_all_required_met` (default), `weighted_threshold` (new). Unknown values fall back to AND with an explicit stderr warning emit (own2 honest fallback). The schema additionally accepts OPTIONAL header fields `threshold_kind` / `threshold_value` / `progress_metric` / `progress_source`, and OPTIONAL per-cond_ref `weight` (default 1). Test harness `tests/test_roadmap_op_self_impl.hexa` extended additively with S15-S17 scenarios. NO `.checkpoint.*` SSOT file is created or modified by this Track AE; the 4 phase 2 candidate `.checkpoint.<name>` files (`d2r_battlenet_online_play` / `lineage_m_purple_play` / `win32_50pct_coverage` / `win32_full_coverage`) are landed by Track AF in parallel.

## §2. Aggregation kind dispatch table

| Aggregation kind value | Track | Behavior |
| --- | --- | --- |
| (absent) | AB cycle 1 | Default to `AND_all_required_met` (backward-compatible) |
| `AND_all_required_met` | AB cycle 1 | All cond_refs must satisfy required → met; any required="met" hard-fail with actual="unmet" → unmet; otherwise → partial |
| `weighted_threshold` | **AE phase 2 (this cycle)** | sum(weight(ref)) for refs where actual ∈ {met, retired_intentional}; sum ≥ threshold_value → met; sum > 0 → partial; sum == 0 → unmet |
| any other (e.g. `frobnicate`) | AE phase 2 | AND fallback + explicit stderr WARN line `__ROADMAP_OP__ WARN checkpoint-status unknown_aggregation_kind:<v> fallback=AND_all_required_met` |

## §3. Schema extension (header-level, all OPTIONAL with backward-compatible defaults)

```jsonc
{
  "type": "header",
  "kind": "checkpoint",
  "name": "<name>",
  "mk": 2,
  "status": "<unmet|partial|met|retired_intentional>",
  "aggregation": "<AND_all_required_met|weighted_threshold>",  // OPTIONAL, default AND_all_required_met
  "threshold_kind": "<percentage|fn_count|weighted_sum>",       // OPTIONAL, default percentage
  "threshold_value": <int>,                                      // OPTIONAL, default 0
  "progress_metric": "<metric_name>",                            // OPTIONAL informational
  "progress_source": "<source_path/field>",                      // OPTIONAL informational
  "cond_refs": [
    {
      "roadmap": ".roadmap.<X>",
      "cond": "<X>.cond.<N>",
      "required": "<met|partial>",
      "weight": <int>                                            // OPTIONAL, default 1
    },
    ...
  ]
}
```

For `win32_50pct_coverage`: `threshold_value=50`, `threshold_kind=percentage`, `progress_metric=win32_fn_coverage_count`, `progress_source` references a marker progression field (Track AF lands the actual checkpoint file with these exact values).

For `win32_full_coverage`: `threshold_value=100` (100% coverage threshold).

## §4. Files

### Created (2)

| Path | Bytes | sha256 | Notes |
| --- | --- | --- | --- |
| `airgenome/doc/airgenome_gamebox_weighted_threshold_aggregation_landed_2026_05_04.ai.md` | (this file) | (sha256 pinned in marker post-write) | 7 sections, ≥10 caveats inline §6 |
| `state/markers/airgenome_gamebox_weighted_threshold_aggregation_landed.marker` | (marker) | self-referential — sha256 not pinned in own body | Track-AE marker schema mirror Track AB / AC / AD pattern |

### Modified (2 — additive only, AND aggregation byte-for-byte preserved)

| Path | Before LOC | After LOC | Δ LOC | Bytes (after) | sha256 (after) |
| --- | --- | --- | --- | --- | --- |
| `tool/roadmap_op.hexa` | 2165 | 2346 | +181 | 93370 | `55a97f6c0b1f999363a3874939272ecebbc9de4e8b6f5a2ac00f7a44846f784d` |
| `tests/test_roadmap_op_self_impl.hexa` | 817 | 1009 | +192 | 52963 | `4da9d265de9e2b684a00c944f8bac7e5bd617b4bcc33fae2b70bb80ddd7515d1` |

### Functions added in `tool/roadmap_op.hexa` (Track AE, 7 new — all additive)

1. `parse_uint_safe(s: str) -> int` — non-negative integer parser (no `parse_int` builtin in hexa runtime; returns -1 on parse failure).
2. `extract_int_field(obj: str, key: str, default_val: int) -> int` — reads a non-negative integer JSON field with default fallback.
3. `checkpoint_extract_aggregation_kind(header: str) -> str` — returns the aggregation field value (default `AND_all_required_met`).
4. `checkpoint_extract_threshold_kind(header: str) -> str` — returns threshold_kind (default `percentage`).
5. `checkpoint_extract_threshold_value(header: str) -> int` — returns threshold_value (default 0).
6. `checkpoint_ref_weight(ref_obj: str) -> int` — returns per-cond_ref weight (default 1 — backward-compat).
7. `stderr_warn(msg: str) -> void` — explicit stderr emit helper (uses `printf '%s\n' ... >&2` via `exec`).
8. `checkpoint_aggregate_weighted_threshold(header: str, refs: array, actuals: array) -> str` — returns `met` / `partial` / `unmet`.

The `subcmd_checkpoint_status` body was extended: it now captures the per-ref resolved `actual` status into a parallel `actuals` array, then dispatches on `effective_kind` (`AND_all_required_met` / `weighted_threshold`). The Track-AB AND derivation is preserved byte-for-byte inside the `else` branch. The `__ROADMAP_OP__ <PASS|FAIL> checkpoint-status ...` emit now appends `aggregation=<effective_kind>` for observability.

### Test scenarios added in `tests/test_roadmap_op_self_impl.hexa` (Track AE, 3 new)

| ID | Scenario intent | Assertions |
| --- | --- | --- |
| S15 | `weighted_threshold` met — 2 refs each weight=30, threshold=50, both met → sum=60 ≥ 50 → met | emit `__ROADMAP_OP__ PASS checkpoint-status` + `status=met` + `aggregation=weighted_threshold` |
| S16 | `weighted_threshold` partial — refs (met w=20, unmet w=20), threshold=50 → sum=20 ∈ (0,50) → partial | emit `__ROADMAP_OP__ FAIL checkpoint-status` + `status=partial` + `aggregation=weighted_threshold` |
| S17 | unknown aggregation `frobnicate` → AND fallback + stderr warning | stderr WARN `unknown_aggregation_kind:frobnicate fallback=AND_all_required_met` + emit `aggregation=AND_all_required_met` + AND derivation reaches `met` (both refs met) → PASS |

## §5. Predecessor chain

| Track | Cycle | Marker | Relation |
| --- | --- | --- | --- |
| AA | design (cycle 0) | `state/markers/airgenome_gamebox_mk2_checkpoint_schema_design_landed.marker` | 4 D-options lock-in; phase 2 candidates count 4; weighted_threshold flagged as phase 2 deferred per caveat C9 |
| AB | cycle 1 of 3 | `state/markers/airgenome_gamebox_roadmap_op_checkpoint_subcmd_extension_landed.marker` | Tool extension (3 NEW checkpoint subcmd; AND aggregation hardcoded). **Caveat C2 explicitly deferred weighted_threshold to phase 2 — this Track AE closes that caveat.** |
| AC | cycle 2 of 3 | `state/markers/airgenome_gamebox_phase_1_3_checkpoints_landed.marker` | First 3 phase-1 .checkpoint SSOT files |
| AD | cycle 3 of 3 | `state/markers/airgenome_gamebox_closure_orchestrator_checkpoint_integration_landed.marker` | Closure orchestrator integration; mk2 checkpoint minimum-viable 3-cycle complete |
| **AE** | **phase 2 first item** | **(this marker)** | **weighted_threshold aggregation rule extension — Track AB caveat C2 closed** |

## §6. Caveats (≥10 numbered honest, raw 10 honest_c3 floor)

**C1.** Phase 2 SSOT (the 4 `.checkpoint.<name>` files using `weighted_threshold`) is landed by Track AF in parallel — this Track AE is tool/test/docs surface only. Until Track AF lands, no production checkpoint exercises the weighted aggregation path; coverage validation is via S15-S17 against synthetic fixtures.

**C2.** `parse_uint_safe` accepts only non-negative integers (digits 0-9). Negative weights and decimal weights are rejected (return -1 → default fallback). This is a deliberate scope-limit per phase 2 minimum-viable; if a future cycle requires fractional weights, extend the parser.

**C3.** `extract_int_field` parses up to first `,` / `}` / `]` / whitespace boundary. JSON-encoded numbers wrapped in strings (`"weight":"30"` instead of `"weight":30`) are NOT supported — they would fail the digit check and fall back to default. The Track AF `.checkpoint` files MUST emit `weight` and `threshold_value` as raw integers to match this parser.

**C4.** Stderr warning emit uses `exec("printf '%s\\n' ... >&2")` rather than a hexa-native `eprintln` (no such builtin). Tests capture stderr via `2>&1` redirect, which mixes stderr+stdout into the same stream — S17 verifies the WARN line presence in this combined stream rather than verifying stderr-vs-stdout discrimination. A future cycle may add a hexa-native stderr helper to `lib/perf/r0_common`.

**C5.** Aggregation dispatch is at the `subcmd_checkpoint_status` level only; `checkpoint-list` cached-status display and `checkpoint-verify` schema validator continue to read the `aggregation` field for surface display only (existing behavior). The verify subcmd does NOT validate that `weighted_threshold` checkpoints have `threshold_value > 0` or that all `cond_refs` carry `weight` — these are tolerant defaults (threshold 0 / weight 1). A future cycle may add stricter weighted_threshold-specific schema validation.

**C6.** The error-count branch (any cond_ref resolution error → `derived = "unmet"`) preempts both AND and weighted_threshold aggregation paths. This is intentional: a parsing error MUST NOT silently degrade a weighted sum into a misleading "partial" verdict. Verified explicitly in the dispatch block.

**C7.** `retired_intentional` cond status contributes its weight to the weighted sum (sideways-PASS per Track-AB precedent). This is consistent with the AND aggregation treating `retired_intentional` as satisfying any required level.

**C8.** Threshold 0 fixture behavior: if `threshold_value == 0` and `sum > 0` → met; if both 0 → unmet. This defensive default means a checkpoint with `aggregation: weighted_threshold` but missing `threshold_value` will resolve as `unmet` whenever no refs are met (and `met` if any ref is). Track AF should always set `threshold_value` explicitly.

**C9.** The `aggregation=` suffix appended to the `__ROADMAP_OP__ <PASS|FAIL> checkpoint-status ...` emit line is a NEW field. Any downstream parser (currently none) that reads this line as a fixed-width tuple should be reviewed; the change is forward-compatible (suffix append).

**C10.** `closure_orchestrator.hexa` (Track AD scope) does NOT need weighted_threshold awareness in this cycle — it enumerates checkpoint files and reads cached status only. A future cycle integrating weighted progression into closure_pct calculation would extend Track AD.

**C11.** Anima parity: anima sibling `/Users/ghost/core/anima/tool/roadmap_op.hexa` does NOT have any of the checkpoint subcmd or weighted_threshold logic. Parity break is intentional (Track AB documented; Track AE extends the gap further). The 8 new fns landed in Track AE would migrate cleanly if D4=b (separate `tool/checkpoint_op.hexa`) is selected in a future cycle.

**C12.** No production `.checkpoint.<name>` or `.roadmap.<X>` file is touched by this Track AE. The 4 phase 2 candidates are Track AF parallel territory. This cycle is tool/test/docs additive only — destructive_ops 0, in-place writes 2 (extensions, no overwrites of existing fn bodies).

## §7. Next-cycle actions (post-Track-AE)

- Track AF (parallel to this Track AE): land 4 phase 2 `.checkpoint.<name>` SSOT files at repo root (`d2r_battlenet_online_play`, `lineage_m_purple_play`, `win32_50pct_coverage`, `win32_full_coverage`). The latter two MUST use `aggregation: weighted_threshold` with explicit `threshold_value` and per-ref `weight`.
- Future cycle: stricter weighted_threshold schema validation in `checkpoint-verify` (threshold_value > 0 guard, weight presence guard).
- Future cycle: extend `closure_orchestrator.hexa` `closure_pct` formula to surface weighted progression in addition to discrete reached-count.
- Future cycle: hexa-native stderr emit helper in `lib/perf/r0_common` (replaces the `exec >&2` shellout pattern).

---

*BR-NO-USER-VERBATIM enforce — no verbatim Korean user quotes inline. Friendly preset / silent-land / own1 (Wine 0 / hexa-only / anima clone 0) / own2 (PASS/FAIL emit honest / silent_error_ban / aggregation fallback emit explicit warning) / raw 9 / 11 / 12 / 15 / 175 enforce. mk2 schema migration forbidden. Additive only. Destructive 0. Cost USD 0. Substrate mac-local. Cap 90min.*
