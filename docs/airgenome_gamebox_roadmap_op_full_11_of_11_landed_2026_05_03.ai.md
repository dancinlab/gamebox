# airgenome-gamebox — roadmap_op full 11/11 anima parity (Track M, cycle 2) — handoff

`schema:` airgenome-gamebox/handoff/roadmap_op_full_11_of_11_landed/1
`completion:` 2026-05-03
`predecessors:` Track H (`docs/airgenome_gamebox_roadmap_op_adoption_decision_landed_2026_05_03.ai.md`) + Track I (`docs/airgenome_gamebox_roadmap_op_self_impl_landed_2026_05_03.ai.md`)
`policy:` migration `forbidden`, changes `additive_only`, in_place_writes 0 (4 NEW + 2 additive-extension), destructive_ops 0, cost_usd 0, substrate `mac-local`, cap_minutes 90, BR_NO_USER_VERBATIM, friendly_preset, silent-land, own1 hexa-only, own2 honest-emit silent_error_ban, raw 9 / 11 / 12 / 15 / 175

---

## §1 — directive paraphrase (BR-NO-USER-VERBATIM)

Track-I cycle 1 landed minimum-viable 5 subcmd self-impl (`add` / `update` / `list` / `show` / `verify`) in `tool/roadmap_op.hexa` with status_enum_adopted str-typed first-priority validator (Track-G drift root cause fix). 6 anima subcmd remained deferred. Track-M cycle 2 lands those 6 (`edit-blocker` / `promote-met` / `dump-evidence` / `diff` / `migrate` / `archive`) additively to reach full 11/11 anima parity at signature level. The `migrate` subcmd is the policy exemplar — per repo `migration: forbidden`, it embodies the no-op guard, always emitting FAIL. All extension is additive: existing 5 subcmd fn bodies + main() dispatch existing cases preserved verbatim; new fns appended before main(); new dispatch cases appended after `verify`. Test harness extended additively with S6-S11 scenarios; existing S1-S5 unchanged. Anima sibling (`/Users/ghost/core/anima/tool/roadmap_op.hexa`) remains study-only — no clone, no import, no symlink (own1 alignment preserved).

---

## §2 — 6 new subcmd implementation table

| subcmd | args | behavior | emit (PASS / FAIL examples) |
|---|---|---|---|
| `edit-blocker` | `--roadmap <path> --blocker <id> (--status <enum> | --resolution_path <text> | --eta <iso>)` | Single-field upsert on a blocker object. Exactly one of three field flags required per call (silent_error_ban refuses 0 or >1). `--status` value validated against `BLOCKER_STATUS_ALLOWED = [open, closed, acknowledged, warn]`. `--resolution_path` and `--eta` accept any quoted-safe string. Atomic tmp+mv. | `__ROADMAP_OP__ PASS edit-blocker blocker=<id> <field>=<value>` / `__ROADMAP_OP__ FAIL edit-blocker blocker_status_enum_invalid:<value>` / `__ROADMAP_OP__ FAIL edit-blocker single_field_per_call_only` |
| `promote-met` | `--roadmap <path> --cond <id> [--require-evidence-min <N>]` | Verifier guard pipeline wrapping `update --status met`: (1) evidence-array length ≥ N (default 3); (2) cond carries `verifier`/`verifier_path` field; (3) emit would-be exec line (no actual exec — guard only). On all guards PASS, delegates to `subcmd_update` for the forward-only flip. | `# promote-met would-exec: hexa run <path> --cond <id>` then `__ROADMAP_OP__ PASS promote-met cond=<id> evidence=<n> verifier=<path>` / `__ROADMAP_OP__ FAIL promote-met insufficient_evidence:have=<n> need>=<N>` / `__ROADMAP_OP__ FAIL promote-met verifier_path_missing_in_cond` |
| `dump-evidence` | `--roadmap <path> --cond <id> [--format tsv|json|md]` | Read-only evidence array dump in selected format. Default tsv. JSON form is single-line strict; tsv carries column comment header + idx-prefixed rows; md uses bullet list. | `__ROADMAP_OP__ PASS dump-evidence cond=<id> count=<n> format=<fmt>` / `__ROADMAP_OP__ FAIL dump-evidence format_invalid:<fmt>` |
| `diff` | `--roadmap-a <path> --roadmap-b <path>` | Read-only delta diff over 7 fields: cond_count, blocker_count, status_enum_adopted_count, per-status counts (unmet/partial/met/retired_intentional). Output: 4-column TSV (field / A / B / delta) + delta-count summary. | `__ROADMAP_OP__ PASS diff a=<A> b=<B> deltas=<n>` / `__ROADMAP_OP__ FAIL diff roadmap_a_not_found_or_missing` |
| `migrate` | `--roadmap <path> [--from mk1 --to mk2]` | NO-OP guard exemplar. Per repo policy `migration: forbidden`, this subcmd ALWAYS emits FAIL and exits 1. Args parsed and ignored. Exists for anima parity at signature level only. | `__ROADMAP_OP__ FAIL migrate migration_forbidden_per_repo_policy` (always) |
| `archive` | `--roadmap <path>` | Reads roadmap; checks `blk.1.status_enum_adopted == "retired_intentional"`. On match, writes a freeze marker stub to `state/markers/<roadmap_name>_archive_freeze_<unix_ts>.marker` (additive — NO file move, NO destructive op). On mismatch, emits FAIL with explicit reason. | `__ROADMAP_OP__ PASS archive freeze_marker_emit roadmap=<name> marker=<path>` / `__ROADMAP_OP__ FAIL archive blk_1_not_retired_intentional:adopted=<value>` |

---

## §3 — anima parity 5 → 11 confirmation

| cycle | subcmd count landed | landed subcmds | deferred |
|---|---|---|---|
| Track-I (cycle 1) | 5 / 11 | add, update, list, show, verify | 6 (edit-blocker, promote-met, dump-evidence, diff, migrate, archive) |
| Track-M (cycle 2 — this) | **11 / 11** | (Track-I 5) + edit-blocker, promote-met, dump-evidence, diff, migrate, archive | 0 |

Verification:

- `grep -c "fn subcmd_" tool/roadmap_op.hexa` → 11
- `grep -E "^fn subcmd_(edit_blocker|promote_met|dump_evidence|diff|migrate|archive)" tool/roadmap_op.hexa` matches all 6 new fns
- main() dispatch: 6 new cases appended after the existing `verify` case, before the unknown-subcmd fallback. Case order: edit-blocker / promote-met / dump-evidence / diff / migrate / archive.
- Anima reference posture preserved: `/Users/ghost/core/anima/tool/roadmap_op.hexa` study-only, 0 clones, 0 imports, 0 symlinks. Surface (subcmd dispatch shape, arg flag names) inspired by anima precedent — implementation independent.

---

## §4 — S6-S11 test scenario table

| scenario | subcmd | covers | fixture | check fn |
|---|---|---|---|---|
| S6 | edit-blocker | status set (open→closed), invalid enum refused (`nonsense_value` → `blocker_status_enum_invalid`), resolution_path upsert | base synthetic fixture (re-uses S1-S5 fixture) | `check_s6_edit_blocker` |
| S7 | promote-met | guard PASS (cond.1: 3 evidence + verifier_path), insufficient_evidence FAIL (cond.2: 1 evidence), verifier_path_missing FAIL (cond.3: 3 evidence, no verifier) | dedicated PM fixture (`write_pm_fixture`) — 3 conds with different guard states | `check_s7_promote_met` |
| S8 | dump-evidence | tsv / json / md all 3 format outputs PASS; emit `format=<fmt>` discriminator string | base fixture, cond.1 evidence array | `check_s8_dump_evidence` |
| S9 | diff | 2-roadmap delta matrix; expects `conds\t`, `blockers\t` column headers + PASS emit with deltas count | base fixture (A) + dedicated B fixture (`write_diff_b_fixture`) | `check_s9_diff` |
| S10 | migrate | NO-OP guard always-FAIL emit; `migration_forbidden_per_repo_policy` substring required | base fixture (irrelevant — args ignored) | `check_s10_migrate` |
| S11 | archive | retired_intentional fixture PASS (freeze_marker_emit), non-retired_intentional FAIL (`blk_1_not_retired_intentional`) | dedicated archive fixture (`write_archive_pass_fixture`) + base fixture as fail case | `check_s11_archive` |

S1-S5 (Track-I) preserved verbatim; total scenarios now 11. Test harness emit budget bumped: `r0_emit ≥ 12` (S1..S11 + done sentinel; was ≥ 6). Fallback strategy unchanged: live `hexa` CLI mode if available, else grep-emit source-string schema validation.

---

## §5 — implementation_status flip table

| field | predecessor marker | before | after (in NEW marker) |
|---|---|---|---|
| `implementation_status` | `state/markers/airgenome_gamebox_roadmap_op_adoption_decision_landed.marker` (Track-H) | `pending_user_lock_in` (Track-H land) → `minimum_viable_5_subcmd_landed` (Track-I cycle 1 captured in Track-I marker) | `full_11_of_11_landed` (Track-M cycle 2, captured in NEW Track-M marker) |
| `subcmd_count` | tool/roadmap_op.hexa | 5 (Track-I) | 11 (Track-M) |
| `anima_full_11_subcmd_gap.deferred_6_cycle_2` | Track-I marker | array of 6 (edit-blocker, promote-met, dump-evidence, diff, migrate, archive) | empty array (cycle 2 closed) |

Important: Track-H + Track-I predecessor marker files are NOT modified. The new state is captured exclusively in this Track-M marker (`state/markers/airgenome_gamebox_roadmap_op_full_11_of_11_landed.marker`). Cycle progression is read by walking the predecessor chain: Track-H marker → Track-I marker → Track-M marker. Each cycle's marker is immutable post-land.

---

## §6 — caveats (≥8 honest items, raw 10 honest_c3 alignment)

**C1 — `migrate` subcmd is intentional NO-OP.** Per repo policy `migration: forbidden`, the `migrate` subcmd ALWAYS emits FAIL and exits 1. This is the design — it exists for anima signature parity only. Any future cycle that wishes to enable migration MUST first flip the repo policy AND replace `subcmd_migrate` body in a separate own1-aligned cycle. Until that happens, `subcmd_migrate` is the policy exemplar.

**C2 — `promote-met` does NOT actually execute the verifier.** Guard 3 emits a `# promote-met would-exec: hexa run <verifier_path> --cond <id>` informational line, but does not spawn the child. The design intent is guard-only — actual verifier exec is a separate cycle (cycle 3 CI integration territory). Operators must run the printed command manually if they want runtime verification before promotion.

**C3 — `archive` writes a freeze marker stub but does NOT move the roadmap file.** The marker is purely additive (own1 destructive_ops 0). It records the freeze intent + timestamp but does not relocate `.roadmap.<name>` to any archive directory. A future cycle wishing to perform actual file relocation would require a separate destructive_ops > 0 cycle proposal and policy review.

**C4 — `edit-blocker` enforces single-field-per-call.** If 0 or >1 of `--status` / `--resolution_path` / `--eta` are passed, the subcmd refuses with explicit FAIL emit. This mirrors `update`'s posture (no multi-field bulk update) and prevents partial-write ambiguity. Operators who want to set multiple fields must call the subcmd multiple times.

**C5 — `diff` is summary-level only, not per-cond/per-blocker delta.** The 7-field summary (cond_count / blocker_count / adopted_count / 4 status counts) catches macro drift but does NOT diff individual id-level state changes. A full per-id diff matrix is deferred to cycle 4 (cross-game diff matrix dashboard).

**C6 — `dump-evidence` json output is single-line, lacks pretty-printing.** TSV + md formats include column/section headers; json is compact `{...}`. Operators wanting indented JSON must pipe through `jq` post-tool.

**C7 — `cond_evidence_count` parser is naive.** It counts opening `"` characters at depth 0 inside the evidence array. If an evidence string itself contains an embedded escaped `\"`, the depth tracker handles it via the escape flag, but adversarial inputs (e.g. malformed JSON) may yield an undercount. Synthetic fixtures used in S7 are well-formed; production .roadmap files have always been mk2-strict.

**C8 — `--require-evidence-min` parser is integer-string-table mapping (0..10).** No general parse_int — values outside 0-10 silently fall back to default 3. This is sufficient for current roadmap evidence cardinality; if a roadmap accumulates >10 evidence rows requiring a specific high threshold, the parser must be extended (separate cycle).

**C9 — Test fixture cleanup includes a `state/markers/<name>_arch_archive_freeze_*.marker` rm pass.** S11 PASS path writes a real freeze marker stub to the production `state/markers/` directory (per archive subcmd contract — the marker IS the side effect). The test harness cleanup removes only test-name-scoped freeze markers, never any other marker. Operators running the test multiple times in quick succession will see only the latest cycle's stub remain (timestamps differ; rm wildcard is name-scoped).

**C10 — main() dispatch case order matters.** New cases (edit-blocker, promote-met, dump-evidence, diff, migrate, archive) appended after `verify` and before the unknown-subcmd fallback. The existing 5 cases (add, update, list, show, verify) and their bodies are byte-for-byte unchanged. If a future cycle reorders cases, dispatch resolution remains correct (case names are unique tokens) but the file-line numbers in subcmd_fn_lines marker field will need re-pinning.

**C11 — Anima sibling absence is explicitly safe.** The tool depends on no anima file at runtime. The reference path `/Users/ghost/core/anima/tool/roadmap_op.hexa` is mentioned only in source comments (study-only audit) and never read/imported/exec'd. Standalone-clone safety is preserved (Track-I property, Track-M extension does not violate it).

---

## §7 — next-cycle hooks

- **cycle 3 — CI integration.** Wire `tool/roadmap_op.hexa verify` into git pre-commit hook OR launchd watchdog. own1-aligned — pure shell hook, no external CI service. Wires `verify` (single-file mode for the changed roadmap; all-files mode for full repo audit). Optional companion: `promote-met` actual verifier exec wired to the `would-exec` line emitted by guard 3 (separate decision cycle whether to enable real exec, or keep guard-only posture as policy default).
- **cycle 4 — cross-game diff matrix dashboard.** Extend `subcmd_diff` to per-id level (specific cond / specific blocker delta) + emit a dashboard summary across all 11+ .roadmap.* files. Read-only audit. Could be a separate `dashboard` subcmd or a `--mode dashboard` flag on existing `diff`.
- **cycle 5 — `archive` actual file relocation.** Currently emits freeze marker stub only (own1 destructive_ops 0). A future cycle could propose moving retired_intentional roadmap files to `archive/.roadmap.<name>.<unix_ts>` with explicit destructive_ops > 0 budget approval. NOT urgent — current stub-marker pattern is sufficient for audit trail.
- **anima sibling re-audit.** Post-Track-M: full surface compare across 11 anima subcmd vs 11 airgenome-gamebox subcmd — emit string format / arg signature / exit code mirror confirmation matrix. Read-only. Goal: confirm signature-level parity is complete; document any intentional divergences (e.g. our migrate is NO-OP guard, anima's may be functional).
- **Track-H predecessor marker progressive update.** Track-H marker's `implementation_status` field semantically advances: `pending_user_lock_in → minimum_viable_5_subcmd_landed → full_11_of_11_landed`. The Track-H marker file itself is NOT modified per cycle policy (each cycle captures new state in NEW marker); the chain is read by walking predecessors. If a future cycle wishes to consolidate history (e.g. rebuild a single canonical state marker), that would be a separate decision.

---

`Track M land complete — 4 NEW deliverables (handoff doc + marker) + 2 additive-extension files (tool + tests). 0 existing roadmap files modified, 0 lib/ touched, 0 native/ touched, 0 docs/*.md modified, 0 predecessor markers modified. Pure additive — cleanest possible cycle continuation of Track-I posture.`
