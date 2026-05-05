# airgenome-gamebox — phase 1 first 3 .checkpoint.<name> SSOT landed (Track AC, cycle 2 of 3) — handoff

`schema:` airgenome-gamebox/handoff/phase_1_3_checkpoints_landed/1
`completion:` 2026-05-04
`predecessors:` Track AB (`docs/airgenome_gamebox_roadmap_op_checkpoint_subcmd_extension_landed_2026_05_04.ai.md`) tool extension cycle 1 + Track AA (`docs/airgenome_gamebox_mk2_checkpoint_schema_design_landed_2026_05_04.ai.md`) design cycle 0
`policy:` migration `forbidden`, changes `additive_only` (5 NEW files; 0 modification), in_place_writes 0, destructive_ops 0, cost_usd 0, substrate `mac-local`, cap_minutes 90, BR_NO_USER_VERBATIM, friendly_preset, silent-land, own1 hexa-only Wine 0, own2 honest-emit silent_error_ban, raw 9 / 11 / 12 / 15 / 175

---

## §1 — directive paraphrase (BR-NO-USER-VERBATIM)

Track-AC is cycle 2 of 3 in the mk2 checkpoint mechanism implementation chain. Track-AA (cycle 0, design lock-in) and Track-AB (cycle 1, tool extension) landed previously. This Track-AC cycle lands the first 3 phase-1 `.checkpoint.<name>` SSOT files at repo root, exercising the Track-AB schema validator (`tool/roadmap_op.hexa checkpoint-verify`) and the AND_all_required_met aggregation rule (`tool/roadmap_op.hexa checkpoint-status`). The 3 phase-1 candidates are: `d2r_battlenet_offline_play` (own1 정합 most viable, D2R offline single-player + Battle.net agent stack), `lineage_w_purple_play` (UE4 + Purple Launcher cascading), and `lineage_classic_r28_play` (retired_intentional cascading first concrete reference). Status derivation per checkpoint reflects the current state of referenced cond statuses honestly — none of the 3 derive `met` (per own2 honest disclosure: status was NOT inflated). Phase 2 candidates (`d2r_battlenet_online_play`, `lineage_m_purple_play`, `win32_50pct_coverage`, `win32_full_coverage`) remain deferred to Track AE+. No `.roadmap.<X>` file is modified (read-only reference for cond_refs[] field validation only). Track AD cycle 3 (closure_orchestrator integration) remains pending.

---

## §2 — 3-checkpoint table (name / cond_refs count / derived status / blockers)

| name | cond_refs count | derived status (Track AB checkpoint-status) | blockers count | aggregation |
|---|---|---|---|---|
| `d2r_battlenet_offline_play` | 7 | `partial` (3/7 satisfied) | 3 | AND_all_required_met |
| `lineage_w_purple_play` | 5 | `partial` (3/5 satisfied) | 3 | AND_all_required_met |
| `lineage_classic_r28_play` | 3 | `partial` (0/3 satisfied) | 2 | AND_all_required_met |

All 3 derived statuses match the `status` field encoded in each checkpoint header (own2 honest emit). All 3 PASS `checkpoint-verify` schema validation (issue_count=0). `checkpoint-list` count=3.

---

## §3 — status derivation per checkpoint (each cond_ref actual vs required)

### §3.1 — `.checkpoint.d2r_battlenet_offline_play` (7 cond_refs, derived partial)

| idx | roadmap | cond | required | actual | satisfied |
|---|---|---|---|---|---|
| 0 | `.roadmap.diablo2_resurrected` | `diablo2_resurrected.cond.1` | met | partial | no |
| 1 | `.roadmap.diablo2_resurrected` | `diablo2_resurrected.cond.2` | met | met | yes |
| 2 | `.roadmap.diablo2_resurrected` | `diablo2_resurrected.cond.3` | met | partial | no |
| 3 | `.roadmap.battlenet` | `battlenet.cond.1` | met | met | yes |
| 4 | `.roadmap.battlenet` | `battlenet.cond.2` | met | met | yes |
| 5 | `.roadmap.loader_pe` | `loader_pe.cond.1` | met | partial | no |
| 6 | `.roadmap.loader_dx` | `loader_dx.cond.1` | met | partial | no |

Aggregate: `satisfied=3/7 errors=0 hard_unmet=false → derived=partial` (own2 honest, NOT inflated to met).

### §3.2 — `.checkpoint.lineage_w_purple_play` (5 cond_refs, derived partial)

| idx | roadmap | cond | required | actual | satisfied |
|---|---|---|---|---|---|
| 0 | `.roadmap.lineage_w` | `lineage_w.cond.1` | met | met | yes |
| 1 | `.roadmap.lineage_w` | `lineage_w.cond.2` | met | partial | no |
| 2 | `.roadmap.lineage_w` | `lineage_w.cond.3` | met | partial | no |
| 3 | `.roadmap.purple_launcher` | `purple_launcher.cond.1` | met | met | yes |
| 4 | `.roadmap.purple_launcher` | `purple_launcher.cond.2` | met | met | yes |

Aggregate: `satisfied=3/5 errors=0 hard_unmet=false → derived=partial` (own2 honest, NOT inflated to met).

### §3.3 — `.checkpoint.lineage_classic_r28_play` (3 cond_refs, derived partial)

| idx | roadmap | cond | required | actual | satisfied |
|---|---|---|---|---|---|
| 0 | `.roadmap.lineage_classic_r28` | `lineage_classic_r28.cond.1` | retired_intentional | partial | no |
| 1 | `.roadmap.lineage_classic_r28` | `lineage_classic_r28.cond.2` | retired_intentional | partial | no |
| 2 | `.roadmap.lineage_classic_r28` | `lineage_classic_r28.cond.3` | retired_intentional | partial | no |

Aggregate: `satisfied=0/3 errors=0 hard_unmet=false → derived=partial` (own2 honest disclosure of retired_intentional cascading semantics; see §4).

---

## §4 — retired_intentional cascading semantics handling (AC3-specific)

### §4.1 — the choice matrix

For `.checkpoint.lineage_classic_r28_play`, two design choices were available:

- **Option A** — set `cond_refs[].required` based on actual current cond status (`partial` → required `partial`). Each ref would PASS sideways. Checkpoint would derive `met`.
- **Option B** — set `cond_refs[].required` to `retired_intentional` and rely on Track-AB `checkpoint_ref_satisfied` sideways-PASS rule (`actual==retired_intentional → return true` regardless of required rank).

Track-AC cycle adopted **Option B** per the task spec recommendation, because it expresses the cascading intent honestly: this checkpoint is fundamentally about retired_intentional acknowledgment of GameGuard kernel-mode AC permanent boundary, not about partial/met progress.

### §4.2 — current state honest disclosure

Today, the cond actual statuses are `partial` (not yet flipped to `retired_intentional` at the cond.<id>.status level). The `.roadmap.lineage_classic_r28` blocker has `status_enum_adopted=retired_intentional` adopted at `blk.1`, but the individual cond statuses themselves remain `partial` (the adoption did not cascade to cond statuses automatically — Track-I `update --blocker` flow does not auto-flip cond statuses). Therefore, the Track-AB `checkpoint_ref_satisfied(actual=partial, required=retired_intentional)` evaluates as: `partial != retired_intentional` (skip sideways-PASS) AND `rank(partial)=1 < rank(retired_intentional)=3` → `false`. All 3 cond_refs return unsatisfied; checkpoint derives `partial` (NOT `met`, NOT `retired_intentional`). This is honest per own2.

### §4.3 — flip pathway documented (NOT executed this cycle)

If a future cycle wishes for this checkpoint to derive `met` via sideways-PASS, the flip pathway is: `tool/roadmap_op.hexa update --roadmap .roadmap.lineage_classic_r28 --cond lineage_classic_r28.cond.<N> --status retired_intentional --force` (per Track-I / Track-M update subcmd semantics). After all 3 cond statuses flipped, Track-AB `checkpoint_ref_satisfied(actual=retired_intentional, _) → true` for each, satisfied=3/3, derived=met. This Track-AC cycle does NOT execute this flip — `.roadmap.<X>` files are read-only reference per task scope. The flip is a separate Track decision (likely Track AE+ or a dedicated retired_intentional cascading cycle) requiring user lock-in on whether cond statuses themselves should reflect blocker adoption.

### §4.4 — DRM / GameGuard explicit non-discussion

NO DRM bypass / NO Warden circumvention / NO GameGuard kernel driver injection / NO anti-cheat circumvention is discussed, designed, or implied at any layer of this cycle. The `retired_intentional` adoption is HONEST DOCUMENTATION of Apple-only stack permanent boundary (kernel-mode driver injection is fundamentally outside the Apple-only own1 envelope — Wine 0 strict reading).

---

## §5 — implementation_status flip table

| track | cycle | implementation_status | landed in this cycle? |
|---|---|---|---|
| Track AA | cycle 0 | `design_only_pending_user_lock_in` | no (predecessor) |
| Track AB | cycle 1 of 3 | `cycle_1_of_3_tool_extension_landed` | no (predecessor) |
| **Track AC** | **cycle 2 of 3** | **`cycle_2_of_3_phase_1_checkpoints_landed`** | **YES (this cycle)** |
| Track AD (pending) | cycle 3 of 3 | `cycle_3_of_3_closure_orchestrator_integration_pending` | no |
| Track AE+ (pending) | phase 2+ | `phase_2_weighted_threshold_and_remaining_4_candidates_pending` | no |

The implementation_status field flip from `cycle_1_of_3_tool_extension_landed` (Track AB marker) to `cycle_2_of_3_phase_1_checkpoints_landed` (this Track AC marker) closes the second milestone of the 3-cycle minimum-viable plan locked in at Track AA.

---

## §6 — caveats (honest C3 inline; ≥10 items)

- **C1** — The Track-AB AND aggregation rule does NOT auto-cascade `retired_intentional` from blocker `status_enum_adopted` to cond `status`. AC3 derived `partial` not `met`; this is honest per own2 (NOT a bug, expected semantics). The cascading flip is a separate Track decision.
- **C2** — `.checkpoint.<name>` files are ADDITIVE SSOT — they reference but do NOT modify `.roadmap.<X>` cond statuses. No `.roadmap.*` file is modified by this cycle. Read-only reference posture preserved.
- **C3** — The Track-AB schema validator was used as ground truth: each checkpoint PASSes `checkpoint-verify` with `issue_count=0`. Schema-validated only at this Track-AC cycle (mirrors Track AA / AB design-only / schema-validate-only posture).
- **C4** — Cond `actual` values were captured at land time (2026-05-04). Future `.roadmap.<X>` cond status changes will alter `checkpoint-status` derivation (intended — checkpoint is a derived view, not a frozen snapshot). The header `status: "partial"` field is a cached reflection of current derivation, NOT a frozen contract.
- **C5** — `progress: "X/Y"` field is the satisfied-count over total cond_refs at land time. It is NOT auto-recomputed by `checkpoint-status` — that subcmd only emits the verdict. If `progress` drifts from the live derivation in a future state, `checkpoint-status` will still emit honest derivation; the cached field is informational.
- **C6** — No `.checkpoint.<name>` file emit triggers any `.roadmap.<X>` modification, lib/, native/, tool/, or tests/ modification. 5 NEW files only.
- **C7** — Anima parity 11/11 → 14/?? remains broken intentional post-Track-M baseline (Track AB acknowledgment carried). This Track AC does NOT itself touch tool/roadmap_op.hexa; the parity status is unchanged from Track AB.
- **C8** — Phase 2 deferred candidates (`d2r_battlenet_online_play`, `lineage_m_purple_play`, `win32_50pct_coverage`, `win32_full_coverage`) remain unlanded. Weighted_threshold aggregation rule (for win32_*pct candidates) is phase 2 design territory.
- **C9** — Track AD cycle 3 (closure_orchestrator integration — cross_ref OPTIONAL field summarizing checkpoint count + status snapshot) remains the next pending cycle. This Track AC does NOT touch `tool/closure_orchestrator.hexa`.
- **C10** — `predecessors[]` field in each checkpoint references existing markers (Track AA / Track AB / per-game predecessor markers). These markers are NOT modified by this cycle.
- **C11** — `since: "2026-05-04"` ISO date pinned in all 3 checkpoints (matches Track AC land iso). If checkpoint header is rewritten later, `since` should be preserved (creation date), not bumped (mirror per-game `.roadmap.<X>` since field semantics).
- **C12** — `.checkpoint.<name>` filename and header `name` field MUST match exactly per Track-AB validator check `(h)`. All 3 verified at land.
- **C13** — The `aggregation` field is hardcoded `AND_all_required_met` for phase 1 per Track AB. The Track AB validator ignores this field's value (it always uses AND aggregation in `checkpoint-status` derivation logic for the current cycle); the field is informational + future-proofing for phase 2 weighted_threshold extension.
- **C14** — `alias[]` field (e.g., `["d2r_offline","d2r_bnet_offline"]`) is informational only — Track AB tool layer does not currently resolve checkpoint references via alias. Alias is preserved for future query convenience.

---

## §7 — next-cycle hooks (cycle 3 = Track AD = closure_orchestrator integration)

- **Track AD cycle 3 of 3** — Add `cross_ref` OPTIONAL field to `tool/closure_orchestrator.hexa` summary emit per Track Z envelope. Field shape suggestion: `cross_ref.checkpoint_count: 3` + `cross_ref.checkpoint_met_count: 0` + `cross_ref.checkpoint_status_distribution: {met:0, partial:3, unmet:0, retired:0}` (computed via `tool/roadmap_op.hexa checkpoint-status` invocations). Update `closure_pct` calculation to optionally factor in checkpoint-reached count. NO `.checkpoint.*` modification, NO `tool/roadmap_op.hexa` modification.
- **Track AE+ phase 2** — Land 4 phase 2 candidates (`d2r_battlenet_online_play` / `lineage_m_purple_play` / `win32_50pct_coverage` / `win32_full_coverage`). Design weighted_threshold aggregation for win32_*pct (per Track AA caveat C9). Likely separate Track AE / AF / AG pending user lock-in.
- **Track AF (potential)** — retired_intentional cond-status cascading flip cycle: tool/roadmap_op.hexa update --blocker pathway extension to optionally cascade adopted enum down to cond.<id>.status. Required for `.checkpoint.lineage_classic_r28_play` to derive `met` via sideways-PASS. Pending separate user lock-in.
- **Marker chain progressive update** — Track H/Y/Z/AA/AB/AC marker chain implementation_status field forms a trail observable across markers (decision-only → tool-extension → phase 1 SSOT → closure integration → phase 2 weighted-threshold). Track AC marker closes the second link.
