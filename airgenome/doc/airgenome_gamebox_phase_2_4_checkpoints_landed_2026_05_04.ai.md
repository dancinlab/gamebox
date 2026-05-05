# airgenome-gamebox — phase 2 4 .checkpoint.<name> SSOT landed (Track AF, parallel BG with Track AE) — handoff

`schema:` airgenome-gamebox/handoff/phase_2_4_checkpoints_landed/1
`completion:` 2026-05-04
`predecessors:` Track AD (`airgenome/doc/airgenome_gamebox_self_mk2_tuning_landed_2026_05_03.ai.md` cycle 3 of 3 closure_orchestrator integration mk2_checkpoint_minimum_viable_complete=true) + Track AC (`airgenome/doc/airgenome_gamebox_phase_1_3_checkpoints_landed_2026_05_04.ai.md` phase 1 SSOT cycle 2 of 3) + Track AB (`airgenome/doc/airgenome_gamebox_roadmap_op_checkpoint_subcmd_extension_landed_2026_05_04.ai.md` tool extension cycle 1 of 3) + Track AA (`airgenome/doc/airgenome_gamebox_mk2_checkpoint_schema_design_landed_2026_05_04.ai.md` design cycle 0)
`parallel_bg:` Track AE (tool/roadmap_op.hexa weighted_threshold dispatch — extends checkpoint-status subcmd; not strictly ordered with Track AF; both must exist for win32_*pct candidates' weighted_threshold derivation to actually function — JSONL valid in any case)
`policy:` migration `forbidden`, changes `additive_only` (6 NEW files; 0 modification), in_place_writes 0, destructive_ops 0, cost_usd 0, substrate `mac-local`, cap_minutes 90, BR_NO_USER_VERBATIM, friendly_preset, silent-land, own1 hexa-only Wine 0, own2 honest-emit silent_error_ban, raw 9 / 10 (>=10 caveats) / 11 / 12 / 15 / 175

---

## §1 — directive paraphrase (BR-NO-USER-VERBATIM)

Track-AF is parallel BG with Track-AE in phase 2 of the mk2 checkpoint mechanism. The Track-AA design cycle 0, Track-AB tool extension cycle 1, Track-AC phase 1 SSOT cycle 2, and Track-AD closure_orchestrator integration cycle 3 (mk2_checkpoint_minimum_viable_complete=true) all landed previously. This Track-AF cycle lands the 4 phase-2 `.checkpoint.<name>` SSOT files at repo root: `d2r_battlenet_online_play` (own1 long-horizon, D2R online realm + Warden L2 + EULA/ToS L1 honest carry, AND aggregation), `lineage_m_purple_play` (Android emul layer permanent boundary on macOS, AND aggregation), `win32_50pct_coverage` (Win32 fn coverage 50% milestone, weighted_threshold aggregation), and `win32_full_coverage` (Win32 fn coverage 100% long-horizon target, weighted_threshold aggregation). Two checkpoints are AND-aggregation phase-2 long-horizon goals; two are weighted_threshold percentage-mode milestones referencing an external `progress_source` marker (Track W cycle 32-35 bundled increment marker, `win32_coverage_progression.percent` field, current 132/435 = 30.34%). Status derivation honest per own2: `d2r_battlenet_online_play` partial (3/8), `lineage_m_purple_play` partial (3/5), `win32_50pct_coverage` unmet (30.34 < 50 under percentage-mode reading, OR 0/3 under fallback AND aggregation per Track AB caveat C2 pre-Track-AE), `win32_full_coverage` unmet (30.34 < 100 OR 0/3 fallback). No `.roadmap.<X>` modification, no tool/lib/native/tests/existing-docs modification, no DRM/Warden/AC bypass, no Android emul layer bypass — read-only reference for cond_refs[] cross-validation only. Track AE parallel BG (tool dispatch for `aggregation: "weighted_threshold"`) is acknowledged but not a hard ordering dependency — JSONL files are valid and become correctly derivable once Track AE lands; until then, weighted_threshold checkpoints fall back to AND aggregation (per Track AB caveat C2).

---

## §2 — 4-checkpoint table (name / aggregation / cond_refs count / status / blockers count / sha256)

| name | aggregation | cond_refs count | status | blockers count | sha256 |
|---|---|---|---|---|---|
| `d2r_battlenet_online_play` | AND_all_required_met | 8 | `partial` (3/8 satisfied) | 5 | `7f562c4422cc589bab07055f7448a09963dac4c9885e7ca936b2d625764c4f29` |
| `lineage_m_purple_play` | AND_all_required_met | 5 | `partial` (3/5 satisfied) | 3 | `40476a1f29683d9912b4aa2b1312e58757f5221da302cbfb80548a14943ef6c1` |
| `win32_50pct_coverage` | weighted_threshold | 3 | `unmet` (30.34/50 pct) | 5 | `7a90f35db22d720dc3db5e39621a4256a4528201c093f4d720db5b3fb7381108` |
| `win32_full_coverage` | weighted_threshold | 3 | `unmet` (30.34/100 pct) | 6 | `6b43667ce261d9f466d0e51004b4b2fd1de6f16fb9451d995fa906105d2f504d` |

All 4 PASS Track-AB `checkpoint-verify` schema validation with `issue_count=0`. `checkpoint-list` count=7 (3 phase-1 from Track AC + 4 phase-2 from this Track AF). `checkpoint-status` per file:
- `d2r_battlenet_online_play` → `status=partial met=3/total=8` (FAIL exit per honest emit when derived != met, errors=0)
- `lineage_m_purple_play` → `status=partial met=3/total=5 aggregation=AND_all_required_met`
- `win32_50pct_coverage` → `status=unmet met=0/total=3 aggregation=weighted_threshold` (fallback AND derivation pre-Track-AE; correct percentage-mode derivation pending Track AE land)
- `win32_full_coverage` → `status=unmet met=0/total=3 aggregation=weighted_threshold` (fallback AND derivation pre-Track-AE)

---

## §3 — weighted_threshold semantics for AF3 + AF4 (honest acknowledgment of pct-mode progression)

### §3.1 — design intent (Track AA §7 #6/#7 + caveat C9 alignment)

The Track-AA design (`docs/MK2_CHECKPOINT_SCHEMA_DESIGN.md` §7 row #6/#7) flagged win32_*pct candidates as `weighted_threshold (≥ 50%)` and `weighted_threshold (= 100%)` as a phase-2 deferred design. The intent: Win32 fn coverage progression is fundamentally a percentage metric across a 435-fn surface, NOT a discrete `met/unmet` predicate over a small set of cond_refs. AND aggregation under-expresses the milestone semantics — a cond can be `partial` for many cycles while fn coverage progresses meaningfully (e.g., 30.34% baseline → 50% milestone → 100% target). The `weighted_threshold` aggregation kind encodes percentage-mode progression: `progress_field >= threshold_value` against an external metric source, rather than `all(cond.actual >= cond.required)`.

### §3.2 — chosen semantics for AF3 + AF4

Both AF3 (`win32_50pct_coverage`) and AF4 (`win32_full_coverage`) adopt the percentage-mode reading:

- `aggregation: "weighted_threshold"`
- `threshold_kind: "percentage"`
- `threshold_value: 50` (AF3) / `100` (AF4)
- `progress_metric: "win32_fn_coverage_percent"`
- `progress_source: "state/markers/airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker (win32_coverage_progression.percent field, cycle_35_percent=30.34)"`

Under this reading, the primary derivation is: `progress_field_at(progress_source) >= threshold_value`. With current `cycle_35_percent=30.34`:
- AF3: `30.34 >= 50` → false → `unmet`
- AF4: `30.34 >= 100` → false → `unmet`

The `cond_refs[]` field is SECONDARY ACCOUNTING — it pins which 3 `loader_win32` conds (`cond.1` KERNEL32 + `cond.2` USER32/GDI32 + `cond.3` NTDLL Nt* syscall bridge) contribute to fn coverage progression. The `weight` field (set to `1` for each) is informational under percentage mode; it would matter only if a different `threshold_kind` (e.g., `weighted_sum`) were chosen in a later phase.

### §3.3 — Track AE parallel BG dependency + fallback behavior

Track AE provides the tool dispatch that consumes `aggregation: "weighted_threshold"` — specifically extending `tool/roadmap_op.hexa checkpoint-status` to recognize the new aggregation kind, parse `threshold_kind` / `threshold_value` / `progress_metric` / `progress_source`, read the external progress field, and emit the percentage-mode derivation. Until Track AE lands the dispatch, the existing tool falls back to AND aggregation (per Track AB caveat C2 — schema validator accepts `weighted_threshold` as a valid enum value; `checkpoint-status` derivation defaults to AND-walk when the kind is unrecognized). Under fallback, AF3 + AF4 derive `unmet` as `met=0/total=3` (since all 3 `loader_win32.cond.<n>` are `partial`). This is incorrect under percentage-mode semantics — both should derive `unmet` because `30.34 < threshold` (true for both), not because `met=0/total=3`. The numeric verdict (`unmet`) happens to match by coincidence in this case, but the reasoning is wrong; future fn-coverage progression that flips a `loader_win32` cond to `met` (e.g., production-smoke promotion of cond.2 to met without reaching 50% fn coverage) would produce a wrong `met` derivation under fallback. JSONL files are valid in either case.

### §3.4 — honest acknowledgment of pct-mode quirks

- `progress` header field uses string format `"30.34/50"` and `"30.34/100"` (not `X/Y` integer ratio like AC1 `3/7`) — a deliberate signal of percentage-mode rather than count-mode.
- `progress_source` marker field is a frozen reference at land time; future cycles that update Win32 coverage will land NEW markers (e.g., cycle_36+_marker), at which point AF3 + AF4 should be additively re-pointed via a separate cycle (NOT this cycle). The current `progress_source` frozen reference is honest per own2.
- `weight` field on each cond_ref is set to `1` for parity; a later phase MAY re-weight if/when `threshold_kind: "weighted_sum"` is introduced (separate Track decision).

---

## §4 — cross-link to Track AE (parallel BG providing tool dispatch)

Track AE territory: `tool/roadmap_op.hexa` (extends `checkpoint-status` to dispatch on `aggregation` field for `weighted_threshold` semantics) + `tests/test_roadmap_op_self_impl.hexa` (extends self-test coverage).

Track AF territory (this cycle): the 4 `.checkpoint.<name>` SSOT files at repo root + handoff + marker.

Ordering: AE+AF are not strictly ordered. AE provides tool dispatch; AF provides checkpoint definitions. Both must exist for win32_*pct candidates' weighted_threshold derivation to actually function correctly. If AE has not yet landed when AF lands (this cycle's posture per task spec acknowledgment), the existing tool falls back to AND aggregation (per Track AB caveat C2) — derivation is wrong for win32_*pct, but JSONL files are valid and will work once AE lands.

This handoff DOES NOT modify any Track AE territory file (read-only reference posture preserved): `tool/roadmap_op.hexa` (85068 bytes, untouched) + `tests/test_roadmap_op_self_impl.hexa` (untouched) + any `tool/` other file (untouched).

---

## §5 — implementation_status flip table

| track | cycle | implementation_status | landed in this cycle? |
|---|---|---|---|
| Track AA | cycle 0 | `design_only_pending_user_lock_in` | no (predecessor) |
| Track AB | cycle 1 of 3 | `cycle_1_of_3_tool_extension_landed` | no (predecessor) |
| Track AC | cycle 2 of 3 | `cycle_2_of_3_phase_1_checkpoints_landed` | no (predecessor) |
| Track AD | cycle 3 of 3 | `cycle_3_of_3_closure_orchestrator_integration_landed mk2_checkpoint_minimum_viable_complete=true` | no (predecessor) |
| Track AE (parallel BG) | phase 2 tool dispatch | `phase_2_weighted_threshold_dispatch_pending_or_concurrent` | no (parallel BG, no marker dependency) |
| **Track AF** | **phase 2 SSOT** | **`phase_2_4_checkpoints_landed`** | **YES (this cycle)** |

The implementation_status field flip from Track-AD `mk2_checkpoint_minimum_viable_complete=true` to Track-AF `phase_2_4_checkpoints_landed` closes the SSOT half of the phase-2 expansion (4 NEW `.checkpoint.<name>` files). Track-AE parallel BG closes the tool-dispatch half independently. Combined, total `.checkpoint.*` count flips from 3 (post-AC) to 7 (post-AF).

---

## §6 — caveats (honest C3 inline; ≥10 items)

- **C1** — AF1 `d2r_battlenet_online_play` cond_refs[] de-duplicates against AC1 `d2r_battlenet_offline_play`: `diablo2_resurrected.cond.3` is already in AC1 (offline); AF1 adds `battlenet.cond.3` as the net-new entry plus carries the 7 AC1 entries forward. Total 8 cond_refs (NOT 9) — re-validated to avoid double-counting per task spec instruction. Predecessor checkpoint pinned: `predecessor_checkpoint: ".checkpoint.d2r_battlenet_offline_play"`.
- **C2** — AF1 `d2r_battlenet_online_play` derived `partial` 3/8 honest per own2 (NOT inflated). The 3 `met` cond_refs are: `diablo2_resurrected.cond.2` (MPQ + d2s offline shim met) + `battlenet.cond.1` (Agent SD forging offline shim met) + `battlenet.cond.2` (setup install path offline shim met). The 5 `partial` cond_refs are: `d2r.cond.1` (PE+IAT real binary), `d2r.cond.3` (D3D11 + first-frame), `battlenet.cond.3` (OAuth 2-leg + chat + login + token live), `loader_pe.cond.1` (production PE smoke), `loader_dx.cond.1` (D3D11 D3DMetal licence).
- **C3** — AF2 `lineage_m_purple_play` derived `partial` 3/5 honest per own2. The 3 `met` cond_refs: `lineage_m.cond.1` (Purple-transitive APK met per Track B 2026-05-03) + `purple_launcher.cond.1` (.NET CLR + WebView2 met) + `purple_launcher.cond.2` (Android emul layer track met). The 2 `partial`: `lineage_m.cond.2` (APK emul first-frame), `lineage_m.cond.3` (plaync OAuth + 2FA TOTP). Android emul layer permanent boundary acknowledged honestly — long-horizon multi-year target on macOS; status enum candidate may eventually flip to `retired_intentional` pending separate Track decision similar to lineage_classic_r28 GameGuard pattern, NOT this cycle.
- **C4** — AF3 `win32_50pct_coverage` and AF4 `win32_full_coverage` use `aggregation: "weighted_threshold"` with `threshold_kind: "percentage"`. The primary derivation is `progress_field >= threshold_value` against the external `progress_source` marker; `cond_refs[]` are secondary accounting. `weight: 1` on each cond_ref is informational under percentage mode. Track AE parallel BG provides the tool dispatch; until it lands, fallback aggregation = AND (per Track AB caveat C2). Numeric `unmet` verdict happens to match under both readings for AF3+AF4 today, but the reasoning differs — this is honest disclosure, NOT dismissal of the fallback issue.
- **C5** — `progress` header field uses string format `"30.34/50"` and `"30.34/100"` for AF3+AF4 (decimal/threshold, not integer ratio) — a deliberate signal of percentage-mode rather than count-mode. AC1/AC2/AC3 use integer ratios `"3/7"` `"3/5"` `"0/3"`. Both are valid under the schema's free-form `progress` string field.
- **C6** — `progress_source` field references a specific frozen marker path (`state/markers/airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker`). Future Win32 coverage cycles that extend coverage will land NEW markers; AF3+AF4's `progress_source` should be additively re-pointed in a separate cycle (NOT this cycle) — current frozen reference is honest per own2.
- **C7** — `predecessor_checkpoint` field is NEW in this cycle (added on AF1 + AF4). Track AB validator does NOT require this field; it is informational + future-proofing for cross-checkpoint dependency derivation (e.g., a hypothetical `checkpoint-derive-chain` subcmd in a later cycle could traverse `predecessor_checkpoint` references). AF1 → AC1 (offline must precede online); AF4 → AF3 (50% must precede 100%). AF2 + AF3 have no predecessor_checkpoint (AF2 because Lineage M permanent-boundary is independent of other phase-1 checkpoints; AF3 because it is the first percentage milestone).
- **C8** — NO DRM bypass / NO Warden circumvention / NO realm protocol forging / NO Android emul layer bypass / NO plaync OAuth bypass / NO 2FA TOTP secret extraction is discussed, designed, or implied at any layer of this cycle. AF1 references Warden L2 long-horizon natural environment alignment as a blocker (Win32 syscall perfection rosetta toward detection compat per Track J/K/L/O/Q/W cumulative posture, online_expansion_lockin L1+L2+L3+L4+L5 OK 2026-05-04, indirect baseline only). AF2 references Android emul layer permanent boundary on macOS as a blocker (Houdini/HAXM-equivalent absent, Wine 0 strict reading). AF3+AF4 reference Win32 coverage skeleton-tier surface (no real syscall yet — real CreateFileW/HeapAlloc/CreateProcessW/EnterCriticalSection/NtCreateFile/CoInitializeEx/VariantInit etc all 0 per Track W marker disclosure).
- **C9** — `.checkpoint.<name>` files are ADDITIVE SSOT — they reference but do NOT modify any `.roadmap.<X>` cond statuses. No `.roadmap.*` file is modified. No `tool/`, `lib/`, `native/`, `tests/` modification. Read-only reference posture preserved across all 4 created files.
- **C10** — Anima parity 11/11 → 14/?? remains broken intentional post-Track-M baseline (Track AB acknowledgment carried, Track AC + Track AD did not modify tool, Track AF does not modify tool). Track AE parallel BG MAY further extend tool/roadmap_op.hexa with weighted_threshold dispatch; Track AF posture is parity-status-unchanged from Track AD baseline.
- **C11** — `since: "2026-05-04"` ISO date pinned in all 4 checkpoints. If checkpoint header is rewritten later, `since` should be preserved (creation date), not bumped (mirror per-game `.roadmap.<X>` since field semantics + AC1/AC2/AC3 precedent).
- **C12** — `.checkpoint.<name>` filename and header `name` field match exactly per Track-AB validator check. All 4 verified at land via `checkpoint-verify issue_count=0`.
- **C13** — `aggregation` field enum extended: phase 1 = `AND_all_required_met` only; phase 2 introduces `weighted_threshold`. Track AB validator accepts both as valid enum values (per Track AB schema design pre-acknowledgment). The actual derivation logic for `weighted_threshold` is Track AE territory; until that lands, fallback = AND walk.
- **C14** — `alias[]` field preserved (e.g., `["d2r_online","d2r_bnet_online","d2r_realm"]`) — informational only; Track AB tool layer does not currently resolve checkpoint references via alias.
- **C15** — Track AE parallel BG has NO marker path dependency in this Track AF marker (per task spec instruction: cross-ref optional, no marker path dependency in case Track AE rate-limited). If/when Track AE lands, its marker can additively cite this Track AF marker as the SSOT consumer of the new dispatch.
- **C16** — checkpoint-list count=7 verified at land (3 phase-1 + 4 phase-2). All 4 NEW files PASS schema verify with issue_count=0. checkpoint-status FAIL exit codes for all 4 are the Track-AB intentional emit when derived != met (per silent_error_ban / honest emit) — NOT a schema or referential integrity error; errors=0 in every case.
- **C17** — Phase-2 candidate count is exactly 4 per Track AA inventory (matches Track AC marker `phase_2_deferred_list[]`). No additional phase-2 candidates introduced; no phase-1 candidates re-shuffled.

---

## §7 — next-cycle hooks

- **Track AE parallel BG (independent ordering)** — `tool/roadmap_op.hexa checkpoint-status` extension to dispatch on `aggregation: "weighted_threshold"`: parse `threshold_kind` / `threshold_value` / `progress_metric` / `progress_source` fields; read the external progress field (e.g., from a marker JSON `win32_coverage_progression.percent` field at the path pinned in `progress_source`); emit `progress_field >= threshold_value` derivation. Self-test coverage extension in `tests/test_roadmap_op_self_impl.hexa`. Once AE lands, AF3+AF4 derive correctly under percentage-mode reading. NO `.checkpoint.*` modification.
- **Track AG (potential)** — Win32 coverage progression cycle 36+ continued (per Track O/Q/W roadmap) toward 50% milestone. If a cycle 36 lands NEW marker (e.g., `airgenome_gamebox_win32_cycle36_..._landed.marker`), AF3+AF4's `progress_source` field MAY be additively re-pointed via a dedicated cycle (NOT auto-rebound).
- **Track AH (potential)** — `lineage_m_purple_play` Android emul layer permanent-boundary status enum flip cycle: if/when user lock-in adopts `retired_intentional` posture for Android emul layer (similar to lineage_classic_r28 GameGuard precedent), AF2 cond_refs `lineage_m.cond.2` could flip required from `met` to `retired_intentional`. NOT this cycle.
- **Track AI (potential)** — `d2r_battlenet_online_play` realm protocol freeze cross-link extension. Track L `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` already pinned as predecessor marker; a future cycle MAY land additional realm protocol synthetic state-machine evidence augment per Track J/N pattern. NOT this cycle.
- **Marker chain progressive update** — Track AA/AB/AC/AD/AE/AF marker chain implementation_status field forms a trail observable across markers (decision-only → tool-extension → phase 1 SSOT → closure integration → phase 2 weighted-threshold dispatch + phase 2 SSOT). Track AF marker closes the SSOT half of the phase-2 expansion.
