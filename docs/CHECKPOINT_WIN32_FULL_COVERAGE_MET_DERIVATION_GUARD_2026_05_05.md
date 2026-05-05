# CHECKPOINT_WIN32_FULL_COVERAGE_MET_DERIVATION_GUARD_2026_05_05

Pre-emptive derivation guard documenting the transition criteria for the
`.checkpoint.win32_full_coverage` mk2 SSOT to flip from `partial` (current
post Track BO snapshot, 91.49 / 100) to `met` (full 100% Win32 fn coverage
milestone) once the concurrently landing Track BP cycle 60-62 bundled
increment crosses the 100% threshold (435/435 fn skeleton-tier minimum).

Track BR (BG subagent) cycle, doc-only — NO `.checkpoint.win32_full_coverage`
state mutation, NO `.roadmap.loader_win32` cond status mutation, NO tool
invocation, NO marker mutation outside the 3 NEW files for this track.

## §0 목적 / 한계 (Purpose / Boundaries)

- guard_kind: pre-emptive derivation transition documentation. NOT an
  actual checkpoint state write-back. The cached `status` field on line
  8 of `.checkpoint.win32_full_coverage` remains `unmet` until a
  separate later track explicitly invokes `tool/roadmap_op.hexa
  checkpoint-status --update --checkpoint .checkpoint.win32_full_coverage`
  (drift fix cycle, deferred).
- audience: future Track BS+ closure cycles, future
  `tool/roadmap_op.hexa checkpoint-status` invocations, future snapshot
  v3+ producers (Track BO mirror), human reviewers reasoning about the
  Win32 full-coverage milestone after Track BP lands.
- relation_to_BO: Track BO (CHECKPOINT_STATUS_SNAPSHOT_2026_05_05.md)
  recorded `win32_full_coverage` derived = `partial` 91.49 / 100 with
  drift = YES (cached unmet → derived partial). This Track BR guard
  doc documents the next transition: derived partial → derived met,
  conditioned on Track BP completion.
- non-goals: this doc does NOT trigger the transition; does NOT predict
  the exact cycle 60-62 fn count breakdown (Track BP authoritative);
  does NOT modify any cached header status; does NOT modify any
  `.roadmap.loader_win32` cond status; does NOT promote skeleton-tier
  fn to real-tier (separate production-smoke matrix work, multi-cycle
  long-horizon).
- limit: single-point-in-time guard dated 2026-05-05. If Track BP
  lands a different cycle range (e.g., 60-63 instead of 60-62) or a
  different module mix (e.g., winhttp + msvcp without winsta), the
  §2 trajectory section is informational only — the §3 derivation
  logic and §4 transition criteria remain authoritative.

## §1 current win32_full_coverage state (post Track BL / Track BO snapshot)

Per `.checkpoint.win32_full_coverage` line 8 JSONL header (cached, not
yet refreshed):

| field | value | source |
|---|---|---|
| `name` | `win32_full_coverage` | header.name |
| `aggregation` | `weighted_threshold` | header.aggregation |
| `threshold_kind` | `percentage` | header.threshold_kind |
| `threshold_value` | `100` | header.threshold_value |
| `progress_metric` | `win32_fn_coverage_percent` | header.progress_metric |
| `progress_source` | `state/markers/airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker` | header.progress_source (stale per BO §6) |
| cached `status` | `unmet` | header.status (stale, never refreshed) |
| cached `progress` | `30.34/100` | header.progress (stale) |
| BO derived `status` | `partial` | snapshot 2026-05-05 |
| BO derived `progress` | `91.49/100` | snapshot 2026-05-05, cycle_56_59 marker |
| BO drift detected | YES (unmet → partial) | snapshot drift_count=3 |
| current latest progress marker | `state/markers/airgenome_gamebox_win32_cycle56_59_bundled_increment_landed.marker` | cycle_59 = 398/435 = 91.49% |
| gap_pp | 8.51 | 100 − 91.49 |
| gap_fn | 37 | 435 − 398 |

cond_refs[] (secondary accounting under weighted_threshold percentage
mode per `.checkpoint.win32_full_coverage` line 7 caveat):

| roadmap | cond | required | actual_2026_05_05 |
|---|---|---|---|
| `.roadmap.loader_win32` | `loader_win32.cond.1` | met | partial |
| `.roadmap.loader_win32` | `loader_win32.cond.2` | met | partial |
| `.roadmap.loader_win32` | `loader_win32.cond.3` | met | partial |

Under pre-Track-AE fallback (Track AB caveat C2), aggregation falls
back to `AND_all_required_met` against cond_refs[] → all three are
partial → fallback derivation = `unmet`. Track AE/AG documented
weighted_threshold semantics (used as primary in this guard) → derived
= `partial 91.49 / 100`.

## §2 Track BP target trajectory (cycle 60-62, informational)

Track BP is concurrently landing (per user directive paraphrase in
this Track BR cycle). Target trajectory:

| cycle | bundled module candidate | est. NEW fn | running cumulative (fn) | running cumulative (pct) |
|---|---|---|---|---|
| 59 (Track BL4 baseline) | pe_d2d1_core.hexa (already landed) | — | 398 | 91.49% |
| 60 (Track BP est.) | winhttp module | ~12-15 | ~410-413 | ~94.25-94.94% |
| 61 (Track BP est.) | msvcp/runtime module | ~10-13 | ~420-426 | ~96.55-97.93% |
| 62 (Track BP est.) | winsta module + closure | ~9-15 | 435/435 | 100.00% |

Notes:
- exact module mix + per-cycle fn count is Track BP authoritative; the
  estimate above is informational and may shift by ±1-2 cycle if
  closure tail (e.g., remainder fn promotions) push to cycle 63.
- Track BL4 (cycle 59) baseline 398/435 = 91.49% is fixed (cycle_56_59
  marker on disk).
- skeleton-tier fn 100% reach is the threshold semantic; real-tier
  (production-smoke promotion per fn matrix) is separate multi-cycle
  long-horizon work and does NOT block the `met` derivation.

## §3 derivation logic (per Track AE / AG weighted_threshold semantics)

The `.checkpoint.win32_full_coverage` header declares
`aggregation = weighted_threshold` with the following primary derivation
inputs (per `.checkpoint.win32_full_coverage` line 7 explicit semantics
and Track AG closure_orchestrator weighted_threshold awareness handoff):

```
threshold_kind   : percentage
threshold_value  : 100
progress_metric  : win32_fn_coverage_percent
progress_source  : state/markers/airgenome_gamebox_win32_cycle60_62_bundled_increment_landed.marker
                   (path AFTER Track BP lands; current header pins cycle_32_35 path,
                    Track BP / a separate drift-fix cycle additively updates this field)
progress_field   : win32_coverage_progression.percent (cycle_62 entry)
```

derivation rule:

```
if progress_pct >= threshold_value:    # 100
    derived_status = met               # full coverage milestone
elif progress_pct > 0:
    derived_status = partial            # current state under BO snapshot
else:
    derived_status = unmet
```

cond_refs[] under weighted_threshold percentage mode are SECONDARY
accounting only — they identify which 3 `.roadmap.loader_win32` conds
contribute to the fn coverage progression (cond.1 kernel32 / cond.2
user32+gdi32 / cond.3 ntdll). cond statuses being `partial` does NOT
block `met` derivation under primary semantics, because the
threshold is checked against the external progress field, NOT against
a sum of cond weights.

Under fallback (pre-Track-AE: AND_all_required_met against cond_refs[]):
all three conds remain `partial` post Track BP land (skeleton-tier fn
coverage milestone does NOT auto-promote cond status to `met` — cond
promotion requires production-smoke matrix completion per cond
blocker_reason, multi-cycle long-horizon). Therefore under fallback,
even after Track BP cycle 62 reaches 100%, `derived_status = unmet`.
Primary (Track AE/AG) semantics are required for the `met` derivation.

## §4 transition criteria checklist (when Track BP completes)

For `win32_full_coverage` to flip from BO-snapshot derived `partial` to
post-BP derived `met`, ALL of the following must be true:

- [ ] **Win32 fn coverage ≥ 100% (435/435)** — current latest Track BP
  marker `win32_coverage_progression.percent` for the highest cycle
  entry MUST be ≥ 100.00. Critical / required.
- [ ] **Track BP marker `threshold_100pct_breached: true` field present**
  — explicit milestone flag in
  `state/markers/airgenome_gamebox_win32_cycle60_62_bundled_increment_landed.marker`
  (or successor path). Required for unambiguous derivation; mirrors
  Track BL4 90% threshold breach pattern.
- [ ] **Track BP marker `win32_full_coverage_milestone: true` field
  present** — secondary checkpoint linkage flag pointing back to
  `.checkpoint.win32_full_coverage`. Required for downstream
  `tool/roadmap_op.hexa checkpoint-status` linkage chain.
- [ ] **Tool `tool/roadmap_op.hexa checkpoint-status --checkpoint
  .checkpoint.win32_full_coverage` invocation emits derived `met`** —
  with current Track BP marker as `--progress-source` (CLI flag) or
  via header-pinned `progress_source` field after additive update in
  drift-fix cycle. Triggers cached header `status` field refresh
  separately.
- [ ] **(optional but recommended) cached header status field
  refresh** to `met` via `--update` flag or additive in-place edit
  in a separate drift-fix cycle (NOT this Track BR cycle, NOT
  Track BP cycle — drift-fix is its own cycle for transparency).

Any one of the first four MISSING → derivation remains `partial` → guard
NOT yet satisfied → cached header refresh deferred.

## §5 next-cycle hook

Post Track BP land, the recommended sequence (NOT executed in this
Track BR cycle, NOT mandatory in any single subsequent cycle — recorded
here for handoff continuity):

1. Track BP completes cycle 60-62 bundled increment land — emits
   `state/markers/airgenome_gamebox_win32_cycle60_62_bundled_increment_landed.marker`
   with `threshold_100pct_breached: true` + `win32_full_coverage_milestone:
   true` + `win32_coverage_progression.cycle_62.percent: 100.00`.
2. Snapshot v3 cycle (Track BO mirror, separate later track) — produces
   `docs/CHECKPOINT_STATUS_SNAPSHOT_2026_05_06.md` (or later date)
   recomputing all 7 checkpoints; expected `derived_status_distribution`
   shifts from `{met: 1, partial: 5, unmet: 1}` to
   `{met: 2, partial: 4, unmet: 1}` (or similar), with
   `win32_full_coverage` derived = `met`.
3. Optional drift-fix cycle (separate later track) — additively updates
   `.checkpoint.win32_full_coverage` cached `status` field from `unmet`
   to `met` and additively updates `progress_source` field from
   cycle_32_35 marker path to cycle_60_62 marker path. Edit kind:
   surgical line 8 JSONL header field replace; spirit of additive_only
   preserved (no field deleted, no other field broken).
4. Optional `.checkpoint.win32_50pct_coverage` cached refresh in same
   drift-fix cycle (already derived `met` per Track BO snapshot, cached
   `unmet` is also stale — consolidated drift fix).

This Track BR guard doc EXPECTS sequence above to occur but does NOT
trigger any step; Track BR is documentation only.

## §6 cross-link

- Track AE — `state/markers/airgenome_gamebox_closure_orchestrator_weighted_threshold_awareness_landed.marker`
  (closure_orchestrator weighted_threshold awareness predecessor;
  documented derivation semantics this guard relies on)
- Track AG — `airgenome/doc/airgenome_gamebox_closure_orchestrator_checkpoint_integration_landed_2026_05_04.ai.md`
  (closure_orchestrator + checkpoint integration handoff predecessor)
- Track AF — `state/markers/airgenome_gamebox_phase_2_4_checkpoints_landed.marker`
  (phase 2 SSOT predecessor; landed `.checkpoint.win32_full_coverage`)
- Track BO — `docs/CHECKPOINT_STATUS_SNAPSHOT_2026_05_05.md` +
  `state/markers/airgenome_gamebox_checkpoint_status_snapshot_v2_track_bo_landed.marker`
  (snapshot v2 immediate predecessor recording derived `partial 91.49 /
  100` for `win32_full_coverage`; this Track BR guard documents the
  transition from that partial state to met)
- Track BP — concurrently landing (cycle 60-62 bundled increment).
  Marker path:
  `state/markers/airgenome_gamebox_win32_cycle60_62_bundled_increment_landed.marker`
  (expected). Authoritative for actual fn count + module mix + cycle
  range.
- Track BL — `state/markers/airgenome_gamebox_win32_cycle56_59_bundled_increment_landed.marker`
  (cycle_59 = 398/435 = 91.49% baseline; current latest before
  Track BP)

## §7 caveats

- C1 — guard_doc_only: this Track BR cycle does NOT mutate
  `.checkpoint.win32_full_coverage` cached `status` field or
  `progress_source` field. Drift remains until a separate drift-fix
  cycle.
- C2 — pre_AE_fallback_aggregation: under fallback aggregation
  (`AND_all_required_met` per Track AB caveat C2), `win32_full_coverage`
  derives `unmet` even after Track BP 100% reach because all three
  cond_refs[] conds (`loader_win32.cond.1` / `.cond.2` / `.cond.3`)
  remain `partial` (skeleton-tier reach does NOT auto-promote cond
  status to `met`; production-smoke matrix is multi-cycle long-horizon
  work). Primary (Track AE/AG) semantics are REQUIRED for the `met`
  derivation under this guard.
- C3 — skeleton_tier_not_real_tier: 100% fn coverage at Track BP cycle
  62 is skeleton-tier (signature + minimal stub) NOT real-tier
  (production-smoke + Wine equivalence + macOS bridge). Real-tier
  promotion is a separate multi-cycle long-horizon track per the fn
  matrix; the `.checkpoint.win32_full_coverage` `met` derivation under
  this guard captures the SKELETON-TIER milestone only. Real-tier
  100% would need a separate `.checkpoint.win32_full_real_tier_coverage`
  (does not currently exist).
- C4 — track_bp_authoritative: §2 trajectory module mix + per-cycle fn
  count is informational estimate only. Track BP marker is
  authoritative for the actual cycle 60-62 (or 60-63 if closure tail)
  contents. If Track BP lands a different module / fn count, only the
  trajectory section is informational; §3 derivation logic + §4
  transition criteria remain authoritative.
- C5 — cached_progress_source_drift: `.checkpoint.win32_full_coverage`
  header line 8 still points `progress_source` at the cycle_32_35
  marker path (original land time). Closure_orchestrator must follow
  the successor chain (Track BO snapshot §0 + caveat C5 acknowledged)
  OR each new Win32 cycle should additively update the field. This
  Track BR guard does NOT update the field — drift fix is a separate
  cycle.
- C6 — single_point_in_time: this guard is dated 2026-05-05.
  Subsequent reality (Track BP timing slip, threshold semantic
  refinement, additional cond_ref additions) may invalidate parts.
  Future Tracks should treat this as informational predecessor and
  re-document if needed.
- C7 — silent_land + BR_NO_USER_VERBATIM: the user_directive_paraphrase
  in the marker omits exact user phrasing per BR_NO_USER_VERBATIM
  policy. Friendly preset + silent-land active.
- C8 — own1_apple_only: doc-only cycle, 0 Wine invocation, 0 DRM
  bypass, 0 Warden bypass, 0 Android emul bypass, 0 plaync bypass,
  0 execution surface change. Win32 syscall perfection rosetta toward
  Warden detection compat is long-horizon natural environment
  alignment posture (not a bypass), unchanged by this guard.
- C9 — predecessor_chain_read_only: all referenced predecessor markers
  (Track AE / AG / AF / BO / BP / BL / phase_1 / phase_2 /
  closure_orchestrator_weighted_threshold_awareness /
  roadmap_op_checkpoint_subcmd_extension) are READ-ONLY in this Track
  BR cycle. 0 predecessor marker mutation. 0 existing `.checkpoint`
  mutation. 0 existing `.roadmap` mutation. 0 lib/tool/native/tests
  mutation. 0 existing docs/*.md mutation.
