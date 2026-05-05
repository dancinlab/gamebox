# airgenome-gamebox — roadmap_op checkpoint subcmd extension (Track AB, cycle 1 of 3) — handoff

`schema:` airgenome-gamebox/handoff/roadmap_op_checkpoint_subcmd_extension_landed/1
`completion:` 2026-05-04
`predecessors:` Track AA (`airgenome/doc/airgenome_gamebox_mk2_checkpoint_schema_design_landed_2026_05_04.ai.md`) + Track M (`airgenome/doc/airgenome_gamebox_roadmap_op_full_11_of_11_landed_2026_05_03.ai.md`) + Track I (`airgenome/doc/airgenome_gamebox_roadmap_op_self_impl_landed_2026_05_03.ai.md`)
`policy:` migration `forbidden`, changes `additive_only`, in_place_writes 0 (2 NEW + 2 additive-extension), destructive_ops 0, cost_usd 0, substrate `mac-local`, cap_minutes 90, BR_NO_USER_VERBATIM, friendly_preset, silent-land, own1 hexa-only, own2 honest-emit silent_error_ban, raw 9 / 11 / 12 / 15 / 175

---

## §1 — directive paraphrase (BR-NO-USER-VERBATIM)

Track-AA cycle (design-only, design_only_pending_user_lock_in) landed 4 D decision matrix recommendations for the mk2 checkpoint mechanism (D1=b cross-roadmap, D2=b separate SSOT, D3=a reuse cond enum, D4=a roadmap_op extend) along with 7 candidate checkpoints inventory. All four D options were accepted at user lock-in. Track-AB cycle 1 of 3 closes the implementation_status progression on the tool layer only — extends `tool/roadmap_op.hexa` additively with 3 NEW checkpoint subcmd (`checkpoint-status` / `checkpoint-list` / `checkpoint-verify`) over the `.checkpoint.<name>` SSOT format. NO `.checkpoint.<name>` file is emitted in this cycle (Track-AC cycle 2 territory — first 3 phase-1 candidates: d2r_battlenet_offline_play / d2r_battlenet_online_play / lineage_classic_r28_play). NO closure_orchestrator integration (Track-AD cycle 3 territory). All extension is byte-for-byte additive: existing 11 subcmd fn bodies + main() dispatch existing 11 cases preserved verbatim. Test harness `tests/test_roadmap_op_self_impl.hexa` extended additively with S12-S14 covering the 3 new subcmd over synthetic temp `.roadmap.<X>` + `.checkpoint.<name>` fixtures (NEVER any production file). Aggregation rule: AND (all cond_refs[] required satisfied for checkpoint to be `met`). weighted_threshold (win32_*pct candidates) deferred to phase 2. Anima sibling at `/Users/ghost/core/anima/tool/roadmap_op.hexa` remains study-only — 0 clones / 0 imports / 0 symlinks. Anima parity 11/11 → 14/?? — the parity break is INTENTIONAL per Track-AA D4 matrix C5/C8 caveat lock-in (gamebox forward evolution path, confidence MEDIUM).

---

## §2 — 3 new subcmd implementation table

| subcmd | args | behavior | emit (PASS / FAIL examples) |
|---|---|---|---|
| `checkpoint-status` | `--checkpoint <path>` | Parses the `.checkpoint.<name>` JSONL header (line 5 by mk2 convention), extracts `cond_refs[]`, and for each ref tuple `{roadmap, cond, required}` parses the referenced `.roadmap.<X>` file, locates `cond.<id>` within `required_conditions[]`, reads its current `status`, and compares to the `required` field. Aggregation: AND — every cond_ref must have `actual` satisfying `required` (`retired_intentional` is sideways-PASS regardless). Derives checkpoint status: all satisfied → `met`; any required="met" but actual="unmet" → `unmet`; else → `partial`. Prints per-ref summary table + aggregate counts. | `__ROADMAP_OP__ PASS checkpoint-status <name> status=met met=N/total=N` / `__ROADMAP_OP__ FAIL checkpoint-status <name> status=unmet met=K/total=N` / `__ROADMAP_OP__ FAIL checkpoint-status checkpoint_not_found_or_missing` / `__ROADMAP_OP__ FAIL checkpoint-status kind_not_checkpoint:<kind>` |
| `checkpoint-list` | `[--filter <kind>]` (future-proofing — accepted but unused this cycle) | Enumerates `.checkpoint.*` files in repo root via existing `ls -1` shell helper pattern. For each: parses header, prints `name | kind | cond_refs_count | cached_status` (the cached `status` field from the header). When zero `.checkpoint.*` files exist (the case in cycle 1 — Track-AC lands them), emits `count=0` PASS with `no checkpoints found` notice. | `__ROADMAP_OP__ PASS checkpoint-list count=<N>` / `__ROADMAP_OP__ PASS checkpoint-list count=0` (with explanatory line) |
| `checkpoint-verify` | `--checkpoint <path>` (single) OR no args (verify all) | Schema validator. Eight checks: (a) header type=`header`; (b) kind=`checkpoint`; (c) name field present + filename consistency; (d) checkpoint-level status field present + within cond enum; (e) cond_refs array present + non-empty; (f) each cond_ref has roadmap (must be `.roadmap.<X>` prefix), cond, required fields; (g) referenced .roadmap.<X> file exists + cond.<id> present in target; (h) required value within cond enum (`unmet|partial|met|retired_intentional`). All accumulated into issue_count + messages string. | `__ROADMAP_OP__ PASS checkpoint-verify <name> issue_count=0` / `__ROADMAP_OP__ FAIL checkpoint-verify <name> issue_count=<N> <reason1> <reason2> ...` / `__ROADMAP_OP__ PASS checkpoint-verify all_count=0` (no checkpoints exist) |

All 3 follow Track I+M conventions exactly: honest emit `__ROADMAP_OP__ <PASS|FAIL> <subcmd> <reason>` per silent_error_ban; exit 0 on PASS, 1 on FAIL; reuse existing helpers (`file_exists`, `read_text`, `header_line_get`, `extract_str_after`, `read_str_field`, `locate_obj_span`, `count_substr`, `enum_contains`, `status_rank`); snake_case throughout; reserved-word avoidance verified (no `type`/`mod`/`handle`/`class`/`interface`/`module`/`import` as identifiers).

New helpers added (snake_case, end of helper section, all minimal):

- `list_checkpoints_dir()` — sibling of `list_roadmaps_dir()` (cwd-scoped `ls .checkpoint.*`)
- `checkpoint_basename(path)` — sibling of `roadmap_basename(path)` (strip dir + `.checkpoint.` prefix)
- `checkpoint_count_refs(header)` — count `"roadmap":"` occurrences
- `checkpoint_extract_refs_body(header)` — depth-aware `[...]` body extractor (skips nested arrays/strings)
- `checkpoint_split_refs(body)` — depth-aware `{...}` object splitter (skips nested objects/strings)
- `checkpoint_ref_satisfied(actual, required)` — AND-aggregation predicate (actual rank ≥ required rank, retired_intentional sideways-PASS)
- `checkpoint_verify_one(path)` — per-file validator (parallels `verify_one`)

---

## §3 — anima parity 11 → 14 confirmation + own1 honest disclosure of break

| cycle | subcmd count landed | landed subcmds | anima sibling subcmd count | parity status |
|---|---|---|---|---|
| Track-I (cycle 1) | 5 / 11 | add, update, list, show, verify | 11 | minimum-viable subset of anima parity |
| Track-M (cycle 2) | 11 / 11 | (Track-I 5) + edit-blocker, promote-met, dump-evidence, diff, migrate, archive | 11 | full signature-level parity |
| Track-AB (this cycle 1 of 3) | **14** / 11 anima | (Track-M 11) + checkpoint-status, checkpoint-list, checkpoint-verify | 11 | **break_intentional_post_track_m_baseline_11** |

**own1 honest disclosure of anima parity break (Track-AA D4 matrix C5/C8 caveat lock-in, confidence MEDIUM):**

Anima sibling at `/Users/ghost/core/anima/tool/roadmap_op.hexa` has 11 subcmd, no checkpoint subcmd. Gamebox forward evolution path lands 3 checkpoint subcmd post-Track-M baseline. The break is INTENTIONAL — accepted at user lock-in per Track-AA D4 matrix:

- D4 score: a_roadmap_op_extend = 19, b_new_checkpoint_op = 25, c_closure_orchestrator_extend = 27 (lower=better)
- D4 winner: a_roadmap_op_extend (margin 6 points)
- D4 single weakness: anima_parity_break_risk = 5 (high)
- D4 fallback (if anima parity strict preserve required): b_new_checkpoint_op (separate `tool/checkpoint_op.hexa`)

User lock-in selected option (a) explicitly accepting the parity break. The anima sibling itself is not modified, not cloned, not imported, not symlinked. Standalone-clone safety preserved (the airgenome-gamebox plugin distribution does not depend on anima sibling presence at runtime). Surface inspiration (subcmd dispatch shape, arg flag conventions) carries forward; implementation independent.

Verification:

- `wc -l tool/roadmap_op.hexa` → 2165 (was 1626 in Track-M; +539 LOC additive)
- `grep -c "^fn subcmd_" tool/roadmap_op.hexa` → 14
- `grep -E "^fn subcmd_(checkpoint_status|checkpoint_list|checkpoint_verify)" tool/roadmap_op.hexa` matches all 3 new fns
- main() dispatch: 3 new cases appended after `archive`, before unknown-subcmd fallback; existing 11 cases byte-for-byte preserved.
- `--help` text: 4 new lines appended under existing 11 subcmd help block; existing help lines byte-for-byte preserved.

---

## §4 — S12-S14 test scenario table

| scenario | subcmd | covers | fixture | check fn |
|---|---|---|---|---|
| S12 | checkpoint-status | both refs met → checkpoint status=met (PASS); flip one ref to unmet → checkpoint status=unmet (FAIL); per-ref summary table emitted | `write_synth_roadmap_for_cp` x2 (cp_road_a met / cp_road_b met) + `write_clean_checkpoint_fixture` (cp_pass + cp_fail variants) | `check_s12_checkpoint_status` |
| S13 | checkpoint-list | synthetic 2 (or more) `.checkpoint.*` files in tmp dir; assert `count=N` emit (N≥2, includes cp_pass + cp_fail from S12); also covers `no checkpoints found` schema-validate path when CLI absent | re-uses S12 dir contents | `check_s13_checkpoint_list` |
| S14 | checkpoint-verify | clean fixture (re-uses S12 cp_pass) PASSes; injected drift fixture (`cond_ref.required="frobnicate"` not in cond enum) FAILs with explicit `required_enum_invalid` substring | `write_drift_checkpoint_fixture` (cp_drift) + S12 cp_pass | `check_s14_checkpoint_verify` |

S1-S11 (Track-I + Track-M) preserved verbatim; total scenarios now 14. Test harness emit budget bumped: `r0_emit ≥ 14` (S1..S14 + done sentinels; was ≥ 12). Fallback strategy unchanged: live `hexa` CLI mode if available, else grep-emit source-string schema validation. NO production `.checkpoint.*` file emitted (zero exist; Track-AC cycle 2 territory). NO production `.roadmap.*` file touched.

---

## §5 — implementation_status flip table

| field | predecessor marker | before | after (in NEW marker) |
|---|---|---|---|
| `implementation_status` | `state/markers/airgenome_gamebox_mk2_checkpoint_schema_design_landed.marker` (Track-AA) | `design_only_pending_user_lock_in` (Track-AA captured) | `cycle_1_of_3_tool_extension_landed` (Track-AB cycle 1, captured in NEW Track-AB marker) |
| `subcmd_count_after` | tool/roadmap_op.hexa | 11 (Track-M) | 14 (Track-AB +3) |
| `mk2_checkpoint_lockin` | (predecessor recommendations only — `recommended_options` field) | recommended D1=b / D2=b / D3=a / D4=a | locked-in D1=b / D2=b / D3=a / D4=a (all 4 explicitly accepted by user, captured in NEW marker block) |
| `anima_parity_status` | tool/roadmap_op.hexa | `signature_level_full_parity` (11/11) | `broken_intentional_post_track_m_baseline_11` (14 vs 11 anima) |

Important: Track-AA + Track-M + Track-I predecessor marker files are NOT modified. The new state is captured exclusively in this Track-AB marker (`state/markers/airgenome_gamebox_roadmap_op_checkpoint_subcmd_extension_landed.marker`). Cycle progression read by walking the predecessor chain: Track-I marker → Track-M marker → Track-AA marker → Track-AB marker. Each cycle's marker is immutable post-land.

---

## §6 — caveats (≥10 honest items, raw 10 honest_c3 alignment)

**C1 — No `.checkpoint.<name>` file is emitted in this cycle.** This is intentional (cycle 1 of 3 = TOOL EXTENSION ONLY). Track-AC cycle 2 lands the first 3 phase-1 candidates (d2r_battlenet_offline_play / d2r_battlenet_online_play / lineage_classic_r28_play). Until then, `checkpoint-list` always returns `count=0` against the production repo (the fall-through PASS path). Operators can verify their `.checkpoint.<name>` drafts against synthetic dirs by passing absolute paths to `checkpoint-status` / `checkpoint-verify`.

**C2 — Aggregation rule is hardcoded AND for cycle 1.** `weighted_threshold` (win32_50pct / win32_full coverage candidates) is phase 2 deferred per Track-AA caveat C9. The `aggregation` field in the checkpoint header is read for display in `checkpoint-list` cached output but is not consulted by `checkpoint-status` derivation logic — that is currently AND-only. Phase 2 will introduce a dispatch on the `aggregation` field value.

**C3 — `checkpoint_ref_satisfied` treats `retired_intentional` as sideways-PASS.** This mirrors the existing `status_rank` logic from Track-I (rank 3 sideways from any). A retired_intentional cond is a block-acknowledged PASS-equivalent regardless of the cond_ref `required` field. This is consistent with the lineage_classic_r28_play candidate's `expected_status: retired_intentional` rationale (Track-AA candidate inventory).

**C4 — `checkpoint-status` derivation has a 3-state output: met / partial / unmet.** When all refs satisfied → met (PASS). When any ref hard-fails (required="met" but actual="unmet") → unmet (FAIL). Otherwise → partial (FAIL). The PASS/FAIL emit aligns with whether checkpoint reaches `met`; FAIL exit code is 1 even for `partial` — operators wanting partial-as-PASS semantics must wrap externally.

**C5 — `checkpoint_extract_refs_body` is a depth-aware bracket walker, not a JSON parser.** Same trust assumption as the rest of the tool — the header is single-line strict JSON per mk2 convention. Adversarial inputs (e.g., square brackets inside strings) are handled via the in-string + escape flags. Production-grade JSON parsing would require a separate library (out of scope for own1 hexa-only).

**C6 — `checkpoint-verify` ref-prefix check accepts both bare `.roadmap.<X>` AND path forms.** `cond_ref.roadmap` may be a bare filename (e.g., `.roadmap.diablo2_resurrected`) or include path prefix (`/tmp/.../...roadmap.X`). The check splits on `/` and validates the trailing segment starts with `.roadmap.`. Invalid drift like `_roadmap_*` would trip prefix_violation. This is permissive on path form but strict on filename pattern.

**C7 — `checkpoint-list --filter <kind>` is accepted but unused this cycle.** The arg is parsed (so future-proofing dispatch doesn't break) but not consulted. Track-AC or Track-AD may add filter on the `kind` field of cond_refs aggregation kinds; this cycle treats all checkpoints uniformly.

**C8 — Test harness S12 builds .roadmap fixtures via path-based references.** Synthetic `.roadmap.cp_road_a` + `.roadmap.cp_road_b` live under `dir` (tmp). Their cond_ref `roadmap` field is the absolute tmp path (e.g., `/tmp/.../.roadmap.cp_road_a`). This works because `checkpoint-status` does file_exists() on the literal field, not a repo-relative resolve. Production `.checkpoint.<name>` files (Track-AC) would use repo-relative bare names — both forms are honored.

**C9 — Anima parity break is the single explicit weakness.** Per Track-AA D4 matrix the anima_parity_break_risk score = 5 (high). User lock-in explicitly accepts this break. The anima sibling itself is not affected — gamebox is taking its own evolution path forward. Confidence MEDIUM. If a future cycle wishes to restore strict parity, the fallback path is D4=b (build separate `tool/checkpoint_op.hexa`); the 3 fns landed here would migrate cleanly.

**C10 — closure_orchestrator integration is Track-AD cycle 3 territory.** This cycle does NOT touch `tool/closure_orchestrator.hexa` (526 LOC, untouched). Track-AD will add a `cross_ref` OPTIONAL field to closure_orchestrator's emit summarizing checkpoint count + status snapshot per the Track-Z envelope. Until that lands, checkpoint state is only readable via `roadmap_op` subcmd dispatch.

**C11 — Schema validator does not enforce `aggregation` field value.** `checkpoint-verify` checks header shape (type / kind / name / status / cond_refs) and each cond_ref's roadmap+cond+required, but does NOT validate the optional `aggregation` field value (currently `AND_all_required_met` by convention). Phase 2 weighted_threshold will introduce an explicit aggregation enum + per-aggregation invariants.

**C12 — main() dispatch case order matters.** New cases (checkpoint-status / checkpoint-list / checkpoint-verify) appended after `archive` and before the unknown-subcmd fallback. The existing 11 cases (add / update / list / show / verify / edit-blocker / promote-met / dump-evidence / diff / migrate / archive) and their bodies are byte-for-byte unchanged. Case names are unique tokens — dispatch resolution is order-independent in practice.

---

## §7 — next-cycle hooks

- **cycle 2 — Track AC: first 3 `.checkpoint.*` SSOT land.** Phase-1 representative coverage from Track-AA candidate inventory: `.checkpoint.d2r_battlenet_offline_play` (expected_status=partial, cond_refs spanning .roadmap.diablo2_resurrected + .roadmap.battlenet) + `.checkpoint.d2r_battlenet_online_play` (expected_status=partial_permanent, online realm Warden L2 long-horizon dependent) + `.checkpoint.lineage_classic_r28_play` (expected_status=retired_intentional, GameGuard kernel-mode permanent block cascading). Phase 2 deferred 4 candidates: lineage_m_purple_play, lineage_w_purple_play, win32_50pct_coverage, win32_full_coverage. Track-AC will exercise `checkpoint-status` against these production .checkpoint files and pin their derived status per cycle. Phase-2 weighted_threshold landing is separate (post-AC).
- **cycle 3 — Track AD: closure_orchestrator integration.** Add `cross_ref` OPTIONAL field to `tool/closure_orchestrator.hexa` emit (per Track-Z envelope). closure_orchestrator gains a checkpoint-aware summary stage that calls `roadmap_op checkpoint-status` for each `.checkpoint.<name>` file and includes the derived status alongside roadmap-level cond/blocker counts. NO modification to Track-AB tool layer; Track-AD wires UP to the subcmd CLI surface.
- **anima sibling re-audit (post-Track-AB).** Document the 11 vs 14 break in an audit table — anima 11 anchored / gamebox 14 = 11 + 3 checkpoint extension. Confirm anima is not modified, not cloned, not imported. Goal: capture the divergence point cleanly in the audit log so future maintainers can find the rationale fast.
- **Track-AA predecessor marker progressive update.** Track-AA marker's `implementation_status` field semantically advances: `design_only_pending_user_lock_in → cycle_1_of_3_tool_extension_landed (this Track-AB) → cycle_2_of_3_first_3_checkpoint_ssot_landed (Track-AC) → cycle_3_of_3_closure_orchestrator_integrated (Track-AD)`. The Track-AA marker file itself is NOT modified per cycle policy; the chain is read by walking predecessors.
- **`checkpoint-status` per-aggregation dispatch (phase 2 weighted_threshold).** When weighted_threshold candidates (win32_50pct, win32_full) land, `checkpoint-status` dispatch must read the `aggregation` field and route to a different derivation function. Phase 2 separate cycle proposal.
- **Schema validator stricter cond_refs cardinality.** Current `checkpoint-verify` requires non-empty cond_refs but does not enforce a min count (e.g., 2 for cross-roadmap intent). Phase 2 may introduce a per-kind cond_refs cardinality floor.

---

`Track AB (cycle 1 of 3) land complete — 2 NEW deliverables (handoff doc + marker) + 2 additive-extension files (tool + tests). 0 .checkpoint.* files emitted (Track-AC cycle 2 territory), 0 .roadmap files modified, 0 lib/ touched, 0 native/ touched, 0 docs/*.md modified, 0 predecessor markers modified. Pure additive — cleanest possible cycle continuation of Track-AA design lock-in posture.`
