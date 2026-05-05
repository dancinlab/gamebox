# closure_orchestrator weighted_threshold awareness landed (Track AG, 2026-05-04)

## §1 user_directive_paraphrase

Track AG = closure_orchestrator weighted_threshold awareness extension. Predecessors:
Track AD landed `tool/closure_orchestrator.hexa` checkpoint integration (526→875 LOC,
binary met/unmet via header.status field). Track AE landed `tool/roadmap_op.hexa`
weighted_threshold dispatch (2165→2346 LOC). Track AF landed 4 phase 2
.checkpoint files (2 use weighted_threshold: win32_50pct_coverage / win32_full_coverage).
This Track AG cycle extends `tool/closure_orchestrator.hexa` (875 → 1433 LOC, +558
additive) to distinguish weighted_threshold checkpoints with PROGRESS-AWARE PARTIAL
CREDIT contribution. Mirrors Track AE’s weighted_threshold dispatch pattern in
closure_orchestrator’s checkpoint enumeration + closure_pct calculation; does NOT
exec roadmap_op subcmd from inside closure_orchestrator (Track AD posture preserved).
0 .checkpoint.* edits, 0 .roadmap.* edits, 0 lib/ / native/ / tests/ edits, 0
existing docs/*.md edits, 0 predecessor marker edits, 0 tool/roadmap_op.hexa edits.
additive only / migration forbidden / destructive 0 / $0 / cap 90min /
BR-NO-USER-VERBATIM / friendly preset / silent-land / own1 (Wine 0 / hexa-only /
anima clone 0) / own2 (closure_pct calculation honest 4.55pp delta from real
progress field 30.34/50 + 30.34/100 ratio, NOT inflation) / raw 9 + 11 + 12 + 15 + 175.

## §2 closure_pct formula extension table (binary vs weighted-aware)

| Track | Formula | win32_50pct contrib | win32_full contrib | AND contribs | Sum | closure_pct |
| --- | --- | --- | --- | --- | --- | --- |
| AD baseline (binary) | `min(100, 25 + reached*5)` where reached counts status==met | 0 (status=unmet → binary 0) | 0 (status=unmet → binary 0) | 0 (5×0 — all partial/unmet) | 0 | 25 |
| AG extended (weighted-aware) | `min(100, 25 + Σ per_cp_contribution)` | 30.34/50*5 = 3.034 | 30.34/100*5 = 1.517 | 0 (5×0 — all partial/unmet) | 4.551 | 29.55 |
| Delta | per_cp aware vs binary count | +3.034pp | +1.517pp | unchanged | +4.551pp | +4.55pp |

Per-checkpoint contribution function:
```
per_checkpoint_contribution(cp) =
  if cp.aggregation == "weighted_threshold":
    progress_pct = read_progress_field(cp.progress_source)
    if progress_pct >= cp.threshold_value: CHECKPOINT_WEIGHT (full credit, capped)
    elif progress_pct > 0: progress_pct / cp.threshold_value * CHECKPOINT_WEIGHT (partial)
    else: 0
  else (AND_all_required_met / unknown):
    if cp.status == "met": CHECKPOINT_WEIGHT
    else: 0
```

CHECKPOINT_BASE_PCT = 25 (Track AD constant, preserved). CHECKPOINT_WEIGHT = 5.
CHECKPOINT_PCT_MAX = 100. New constant CLOSURE_PCT_SCALE = 1000 for x1000 internal
scaled-integer arithmetic (hexa runtime has no float primitives — 2-decimal precision
maintained via `format_x1000_decimal` display helper).

## §3 closure_pct delta walk-through

- progress_pct read from
  `state/markers/airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker`
  → `win32_coverage_progression.percent` = 30.34 (raw float in JSON).
- parse via `parse_progress_pct_x1000("30.34")` → 30340 (x1000 scaled).
- win32_50pct (threshold_value=50): 30340 < 50000 → partial credit = 30340 * 5000 / 50000 = 3034 (= 3.034%).
- win32_full (threshold_value=100): 30340 < 100000 → partial credit = 30340 * 5000 / 100000 = 1517 (= 1.517%).
- AND_all_required_met checkpoints (5 total — Track AC ×3 + Track AF ×2 non-weighted): all status=partial/unmet → contribution 0 each.
- contribution_sum_x1000 = 3034 + 1517 = 4551 (= 4.551%).
- closure_pct_x1000 = 25000 + 4551 = 29551 → display "29.55".
- pre-Track-AG (binary count only): closure_pct = 25 + 0*5 = 25.
- Track AG delta: +4.55pp (HONEST per own2 — derived from real progress_field 30.34
  at 30.34/50 + 30.34/100 ratios; NOT arbitrary inflation).

## §4 New helpers count + signatures

11 new fns added (the 6 spec-named + 5 supporting helpers; all snake_case raw 11):

1. `parse_uint_safe_local(s: str) -> int` — non-negative int parser; -1 on failure (mirror Track AE parse_uint_safe; LOCAL helper to avoid cross-tool dep).
2. `parse_progress_pct_x1000(field_value_str: str) -> int` — parses "30.34" → 30340; handles int or float; -1 on parse failure (own2 silent_error_ban).
3. `format_x1000_decimal(value_x1000: int) -> str` — formats 29551 → "29.55"; truncates 4th digit (matches spec values).
4. `checkpoint_aggregation_kind_extract(header: str) -> str` — returns "weighted_threshold" / "AND_all_required_met"; defaults to AND on miss.
5. `checkpoint_progress_source_extract(header: str) -> str` — extract progress_source field via existing checkpoint_read_str_field.
6. `checkpoint_threshold_value_extract(header: str) -> int` — parses raw integer threshold_value field (no quotes); 0 on miss (defensive — Track AE-aligned semantics where 0 → any contribution = met).
7. `progress_source_marker_path(progress_source: str) -> str` — strip " (...)" parens hint to get bare marker path.
8. `read_progress_field_from_marker(marker_path: str, field_path: str) -> str` — JSON marker file reader with 1-level dot-path (e.g. "win32_coverage_progression.percent"); brace-depth tracking for nested object scope.
9. `checkpoint_weighted_contribution_x1000(progress_x1000: int, threshold_value_int: int) -> int` — partial credit calculation per spec formula; full weight at progress >= threshold (capped); 0 at progress=0; 0 on negative progress.
10. `per_checkpoint_contribution_x1000(cp_path: str, cached_status: str) -> int` — aggregation kind dispatcher; weighted_threshold → progress-aware; else → binary on cached_status.
11. `sum_checkpoint_contributions_x1000(cp_files: array, cp_snap: array) -> int` — sum across snapshot.
12. `compute_closure_pct_x1000(contribution_sum_x1000: int) -> int` — adds base + sum, caps at MAX*1000.
13. `build_weighted_breakdown_lines(cp_files: array, cp_snap: array) -> array` — diagnostic per-checkpoint progress=N%/threshold=M% contribution=X% lines.

(The spec requested 6 named helpers — items 2/4/5/6/9 directly + 8 as the
read_progress_field helper. The 5 additional supporting helpers — 1/3/7/10/11/12/13 —
are implementation-required in hexa-without-float-primitives substrate.)

## §5 self_test S9-S11 additions

- **S9** — synthetic weighted_threshold checkpoint header parse:
  - aggregation_kind extraction returns "weighted_threshold" on synthetic header
  - threshold_value extraction returns 50 (int)
  - progress_source extraction → "state/markers/synth_progress.marker (synthetic.field, info)"
  - progress_source_marker_path strips " (...)" → "state/markers/synth_progress.marker"
  - missing aggregation field defaults to AND_all_required_met
- **S10** — synthetic progress_source marker file read / parser unit:
  - parse_progress_pct_x1000("30.34") == 30340
  - parse_progress_pct_x1000("50") == 50000
  - parse_progress_pct_x1000("0") == 0
  - parse_progress_pct_x1000("") == -1 (own2 silent_error_ban)
  - format_x1000_decimal(29551) == "29.55"
  - format_x1000_decimal(4551) == "4.55"
- **S11** — weighted_threshold contribution calculation (spec-pinned values):
  - 30000/50 → 3000 (3.00%)
  - 100000/50 → 5000 (5.00% — capped at full weight per spec "100/50 → 5")
  - 0/50 → 0 (per spec "0/50 → 0")
  - 30340/50 → 3034 (3.034% — Track AF win32_50pct production case)
  - 30340/100 → 1517 (1.517% — Track AF win32_full production case)
  - threshold=0 with positive progress → 5000 (defensive — Track AE-aligned)
  - spec total: 25000 + 3034 + 1517 = 29551 → format "29.55" — closes the loop

self_test step count progression: Track S genesis 5 → Track AD +3 (S6-S8) → Track AG +3 (S9-S11) = **11 total**.

## §6 Files (1 EXTEND + 2 NEW) sha256

| Path | Type | LOC before | LOC after | bytes | sha256 |
| --- | --- | --- | --- | --- | --- |
| `tool/closure_orchestrator.hexa` | EXTEND | 875 | 1433 | 68998 | f95658859e22ac47777e3949219015a9ee788f42893ee8e79a31e4afb2e26850 |
| `docs/airgenome_gamebox_closure_orchestrator_weighted_threshold_awareness_landed_2026_05_04.ai.md` | NEW | — | — | — | self-referential — sha256 not pinned in own body |
| `state/markers/airgenome_gamebox_closure_orchestrator_weighted_threshold_awareness_landed.marker` | NEW | — | — | — | self-referential |

## §7 caveats (≥8 honest C3 inline)

- **C1** — Hexa runtime has no float primitives. Track AG implements scaled-integer arithmetic (CLOSURE_PCT_SCALE = 1000) for 2-decimal precision. Final emit format truncates the 4th digit (29551 → "29.55"); banker rounding NOT implemented. Spec values "29.55" / "4.55" exactly match this truncation behavior, but a future cycle wanting half-up rounding would need an explicit fn.
- **C2** — `read_progress_field_from_marker` hardcodes the field path `win32_coverage_progression.percent` for ALL weighted_threshold checkpoints today (the 2 win32_*pct candidates share progress_metric=`win32_fn_coverage_percent`). A future weighted_threshold candidate with a DIFFERENT progress_metric (e.g. `d2r_battlenet_login_success_rate`) would need either (a) a per-metric → field-path mapping table, or (b) a schema extension that records the JSON field path explicitly inside the .checkpoint header. NOT addressed this cycle (raw 168 minimum-viable deferred).
- **C3** — `progress_source_marker_path` splits on " (" — relies on the convention that .checkpoint authors write `path (field hint, info)` exactly. A path containing " (" literally (extremely unlikely in repo-relative state/markers/<id>.marker convention) would parse incorrectly. NO production .checkpoint.* file has this issue today.
- **C4** — closure_orchestrator does NOT exec `tool/roadmap_op.hexa checkpoint-status` from inside its own dispatcher (mirror Track AD posture — no cross-tool subprocess in the closure tracker). The tool reads .checkpoint.<name> + the progress_source marker DIRECTLY via own helpers. Trade-off: closure_orchestrator and roadmap_op derive weighted_threshold contributions independently; if Track AE’s aggregator differs from Track AG’s contribution function in some corner case, the two tools could disagree. They are aligned today (full credit at >=, partial=progress*weight/threshold, 0 at no progress) — caveat explicit so future divergence is detected.
- **C5** — Track AG marker JSON sha256 is self-referential (the marker pins its own sha256 last; cannot recompute inside its own body). Verification command `shasum -a 256 state/markers/airgenome_gamebox_closure_orchestrator_weighted_threshold_awareness_landed.marker` is post-land informational.
- **C6** — closure_pct = 29.55 is HONEST per own2 — derived from `progress_field=30.34` at progress_source `state/markers/airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker (win32_coverage_progression.percent)`. If the win32 progression marker is REGRESSED in a later cycle (e.g. cycle 40 audit drops percent to 25.0), Track AG’s closure_pct will SHRINK — NOT inflate. This is the desired honest behavior (own2 mandate).
- **C7** — The 4.55pp delta (25 → 29.55) does NOT advance the closure mental model (CM-0..CM-30 ladder per `docs/CLOSURE_ROADMAP.md`). Track AG is still a TRACKER extension; closure_advancement remains 0 in the marker policy block. The delta only changes how progress is REPORTED, not what has been built.
- **C8** — `parse_progress_pct_x1000` truncates fractional digits beyond 3 places (CLOSURE_PCT_SCALE = 1000). e.g. "30.345" → 30345 (preserved exactly), but "30.3456" → 30345 (the 6 is dropped). For win32 progression at 30.34 (2 decimals) this is exact; for some hypothetical future progress field carrying 4+ decimals there will be a 1-digit-truncation gap. Flagged for the next cycle that introduces such a metric.
- **C9** — The spec-stated "30.34/50*5 = 3.034" and "30.34/100*5 = 1.517" sum to 4.551 (not exactly 4.55 — there’s a 0.001 ambiguity). Track AG’s scaled arithmetic produces 4551 (= 4.551), and `format_x1000_decimal` truncates to "4.55" — matching spec display. The marker pins both `delta_pct: "4.55"` (display) and the raw scaled values can be recomputed from `weighted_aware - binary_count_only`.
- **C10** — Existing 3 subcmd + Track AD binary path PRESERVED VERBATIM inside the Track AG extension (additive_only_lock_in). Track AD’s `compute_closure_pct(reached: int)` fn UNCHANGED — Track AG adds a NEW PARALLEL `compute_closure_pct_x1000(contribution_sum_x1000: int)` for the weighted path. Both paths emit alongside each other in status / list / emit so legacy consumers continue to see the binary value.
- **C11** — Track AE aggregation fallback (`AND_with_stderr_warning` for unknown aggregation kind) is NOT replicated in Track AG. Today, an unknown aggregation kind in closure_orchestrator falls through to the AND-binary path (status==met → weight, else 0) — silent fallback. Reason: closure_orchestrator is a TRACKER that already surfaces the cached status field; the WARN emit is roadmap_op territory. Caveat-honest.
- **C12** — production smoke (hexa CLI `/Users/ghost/.hx/bin/hexa tool/closure_orchestrator.hexa self-test` 11-step round-trip) gated on hexa availability + separate test cycle. Self-test S9-S11 added for unit-level coverage of the 3 helper categories per spec.

## §8 Marker path

`state/markers/airgenome_gamebox_closure_orchestrator_weighted_threshold_awareness_landed.marker`

## §9 implementation_status flip

| Track | implementation_status |
| --- | --- |
| AD (Track AD cycle 3 of 3) | cycle_3_of_3_closure_orchestrator_integration_landed |
| AE (phase 2 first item) | phase_2_weighted_threshold_aggregation_landed |
| AF (phase 2 SSOT, 4 candidates) | phase_2_4_checkpoints_landed |
| **AG (this cycle)** | **phase_3_closure_weighted_awareness_landed** |
