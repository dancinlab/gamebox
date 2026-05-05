---
schema: airgenome-gamebox/airgenome/doc/airgenome_gamebox_roadmap_op_self_impl_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_roadmap_op_self_impl_landed.marker
  tool_created:
    - tool/roadmap_op.hexa
  tests_created:
    - tests/test_roadmap_op_self_impl.hexa
  predecessor_handoffs:
    - airgenome/doc/airgenome_gamebox_roadmap_op_adoption_decision_landed_2026_05_03.ai.md
    - docs/ROADMAP_OP_ADOPTION_DECISION.md
    - airgenome/doc/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed_2026_05_03.ai.md
status: LANDED_TRACK_I_ROADMAP_OP_SELF_IMPL_MINIMUM_VIABLE_5_SUBCMD
related_raws:
  - raw 9    # hexa-only impl (no anima clone, no external dep, pure self-impl)
  - raw 10   # honest C3 caveats inline (>=8 numbered)
  - raw 11   # snake_case fields throughout marker JSON + tool internal
  - raw 12   # silent-error ban — every subcmd emits PASS/FAIL explicitly (no swallow)
  - raw 15   # env_lazy — repo-relative paths only, no /Users/<user>/ leaks
  - raw 168  # minimum-viable subset 1-cycle land (5 of 11 anima subcmd)
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes — paraphrase only)
preserved_unchanged:
  - all 11 .roadmap.<game/launcher> files (read-only reference; tool exercises synthetic fixtures only in tests)
  - all 3 .roadmap.loader_* files (untouched)
  - all lib/loader/*.hexa, lib/perf/*.hexa modules (untouched)
  - all native/, plugin.json, entry, README.md (untouched)
  - all 7 existing test harness files (test_apple_only / test_closure / test_own2 / test / test_d2r_archive_round_trip / test_d2r_pe_synthetic_round_trip / test_purple_lineage_offline_shim / test_d2r_d3d11_synthetic_scaffold)
  - all predecessor markers + predecessor handoff docs (read-only reference)
  - /Users/ghost/core/anima/tool/roadmap_op.hexa (study-only sibling — NEVER cloned/imported/symlinked)
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 0
  in_place_writes_note: 4 NEW files only (tool/roadmap_op.hexa + tests/test_roadmap_op_self_impl.hexa + this handoff + marker); 0 .roadmap edits, 0 lib/ edits, 0 native/ edits, 0 existing-file modifications.
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# airgenome-gamebox Track I — tool/roadmap_op.hexa self-impl minimum-viable 5 subcmd landed (BG-AGB follow-up)

## §1. user_directive_paraphrase

BG subagent — Track I post-Track-H lock-in cycle: Option B (self-impl) 권고 채택, scope (a) minimum-viable 5 subcmd 1-cycle land + status_enum_adopted enum validator first-priority. anima sibling `/Users/ghost/core/anima/tool/roadmap_op.hexa` study-only reference (clone/import/symlink 절대 금지) — own1 정합 hexa-native 자체 구현. 5 subcmd: `add` / `update` / `list` / `show` / `verify`. silent_error_ban 정합 모든 subcmd `__ROADMAP_OP__ <PASS|FAIL> <subcmd> <reason>` emit, exit code 0/1. cond status downgrade `--force` 없이 거부. Track G 에서 발견된 lineage_classic_r28 status_enum_adopted bool/string drift 재발 방지가 verify subcmd 의 first-priority validator. nested anima 11 subcmd 중 6 (deferred to cycle 2): edit-blocker / promote-met / dump-evidence / diff / migrate / archive. additive only / migration forbidden / destructive 0 / $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land / own1 (Wine 0 / hexa-only / anima clone 0) / own2 (PASS/FAIL emit 정직 / silent_error_ban 보존) / raw 9 + 10 + 11 + 12 + 15 + 168 + 175 enforce.

## §2. 5 subcmd implementation table

| subcmd | fn | args | behavior | emit |
|---|---|---|---|---|
| `add` | `subcmd_add` (line 271) | `--roadmap <path> --cond <id> --evidence <text>` | Parse JSONL header line 5; locate matching `cond.id`; append evidence row to evidence array (additive only); atomic tmp+mv rewrite of single header line. | `__ROADMAP_OP__ PASS add cond=<id> evidence_appended` (or FAIL with explicit reason: roadmap_not_found / empty_cond_id / empty_evidence_text / header_line_missing / cond_not_found / no_evidence_array_in_cond / evidence_array_unterminated / atomic_write_failed) |
| `update` | `subcmd_update` (line 376) | `--roadmap <path> (--cond <id> --status <enum> [--force] \| --blocker <id> --status_enum_adopted <retired_intentional>)` | Single-field update. Cond status forward-only matrix (unmet=0/partial=1/met=2/retired_intentional=3 sideways); downgrade requires `--force`. Blocker status_enum_adopted str-typed only. | `__ROADMAP_OP__ <PASS\|FAIL> update <reason>` |
| `list` | `subcmd_list` (line 512) | `[--filter <kind=game\|launcher\|loader>]` | Enumerate `.roadmap.*` files in repo root; print name + kind + cond_count + blocker_count + status_enum_adopted_count per row. | `__ROADMAP_OP__ PASS list count=<N>` |
| `show` | `subcmd_show` (line 557) | `--roadmap <path>` | Print header summary: name / kind / type / cond statuses (3 conds) / blockers + adopted enum / cross_link sister_roadmaps. | `__ROADMAP_OP__ PASS show name=<N>` |
| `verify` | `subcmd_verify` (line 736) | `[--roadmap <path>]` (single file or all `.roadmap.*`) | Schema validator. Checks: (a) header `type=header` + `name` field present, (b) cond.status enum within `unmet\|partial\|met\|retired_intentional`, (c) **status_enum_adopted MUST be string-typed** with value `retired_intentional` if present (first-priority — Track G drift fix), (d) blocker.status enum, (e) cond/blocker id prefix matches roadmap name. | `__ROADMAP_OP__ <PASS\|FAIL> verify <roadmap> issue_count=<N>` |

Tool size: **1024 LOC, 39066 bytes**. `__ROADMAP_OP__` emit sites: **45** (every PASS/FAIL path explicit, raw 12 silent_error_ban 정합).

CLI dispatch (line 949 `main()`):
- `tool/roadmap_op.hexa --help|-h|help` — print subcmd list + arg signatures (line 790)
- `tool/roadmap_op.hexa <subcmd> [args...]` — dispatch
- Unknown subcmd → exit 1 with explicit FAIL emit

## §3. status_enum_adopted validator first-priority confirmation

Track G discovered lineage_classic_r28 blk.1 hand-edit drift: `status_enum_adopted: true` (bool) vs Track A2/C exemplar `"retired_intentional"` (string). G1 normalized the single instance, but the structural root cause — absence of automated validator — remained until Track I.

Track I `verify` subcmd locks down the invariant:

```
status_enum_adopted_t = str (only)
status_enum_adopted_v = "retired_intentional" (only)
```

Any non-string occurrence (bool / int / null) flagged as FAIL with explicit issue text. Test S5 fixture injects `status_enum_adopted: true` and asserts FAIL emit — direct regression test for the Track G drift root cause.

Forward-going: any future blocker adoption that hand-edits the field with wrong type will be caught by `tool/roadmap_op.hexa verify` before commit. Cycle 2 will add a CI-style integration that runs verify across all 14 .roadmap files as a single emit.

## §4. anima reference posture (study-only)

Anima sibling repo: `/Users/ghost/core/anima/tool/roadmap_op.hexa` (~31859 bytes, ~573-809 LOC depending on revision, 11 subcmd per Track H audit).

own1 정합 enforcement:
- **NO clone** — code 측 본 repo 측 복사 0 줄
- **NO import** — `use "anima/..."` 측 statement 0 회
- **NO symlink** — filesystem symlink 측 본 repo 측 부재
- **READ-ONLY reference** — anima file 측 surface (subcmd 이름 / dispatch 패턴 / arg signature) 측 study-only

Implementation 측 hexa-native airgenome-gamebox conventions (TSV emit / fn naming / r0_emit pattern / atomic tmp+mv write) 측 자체 design — anima 측 영감 받았으되 코드 측 독립.

본 repo 측 standalone clone safety 보장 — anima sibling 측 부재 시 tool 측 정상 동작. plugin distribution 측 anima 측 부재 시 tool dead 측 risk 0 (Option C symlink 측 disqualifying 단점 회피).

## §5. cycle vs anima full-11-subcmd gap (next-cycle hooks summary)

본 cycle 측 5 subcmd 측 land. Anima sibling 측 11 subcmd. 차 6 subcmd 측 cycle 2 측 deferred:

| anima subcmd | scope | priority | cycle 2 plan |
|---|---|---|---|
| `edit-blocker` | blocker.status / type / resolution_path 측 in-place edit (additive_only spirit at field-level) | high | extend `update` subcmd OR new `edit-blocker` subcmd |
| `promote-met` | cond status partial → met 측 evidence count + verifier path 측 full validation pipeline | high | wraps existing `update --force` with verifier guard |
| `dump-evidence` | cond evidence array 측 stdout / TSV 측 dump (audit trail) | medium | new subcmd, read-only |
| `diff` | 두 .roadmap 측 cond/blocker 측 delta diff (cross-game comparison) | medium | new subcmd, read-only |
| `migrate` | mk1 → mk2 schema migration (정책상 forbidden — own1 migration 정합 시 NO-OP guard 만 land) | low | NO-OP guard subcmd (own1 정합) |
| `archive` | retired_intentional 측 archive 측 freeze move | low | requires destructive_ops > 0 — own1 정합 후속 분리 cycle |

## §6. caveats (>=8 honest items)

- **C1 (anima sibling 측 accessibility 검증 — main thread recovery)**: BG subagent 측 본 cycle 측 rate-limit 측 final report-back 단계 측 cutoff. tool/roadmap_op.hexa (1024 LOC) + tests/test_roadmap_op_self_impl.hexa (310 LOC) 측 disk 측 land 완료. 본 handoff doc + marker 측 main thread 측 recovery 측 작성 (predecessor Track G template 측 mirror, sha256 disk 측 측정 측 cross-pin). anima sibling 측 read-only access 측 main thread 측 미수행 — BG subagent 측 reference posture 측 study-only enforce 보고 (코드 검증 측 grep / wc / shasum 측 main thread 측 indirect 측 확인).
- **C2 (hexa CLI runtime exec 측 미수행)**: 본 repo 측 hexa interpreter 측 binary 측 build pending (Track G 동급 posture). schema-validated only — `tool/roadmap_op.hexa --help` / `verify --roadmap .roadmap.lineage_classic_r28` 측 live exec 측 main thread 측 미수행. tool 측 분 hexa pure self-impl, predecessor cycle 측 동급 schema-validated land posture (test harness 측 r0_emit grep fallback 정합).
- **C3 (5/11 subcmd 부분 cover)**: anima 측 11 subcmd full surface 측 6 deferred. promote-met / edit-blocker / diff / dump-evidence / migrate-NOOP / archive 측 cycle 2 측 land. 본 cycle 측 minimum-viable scope (a) 측 user lock-in 정합.
- **C4 (cond status downgrade 측 --force flag escape hatch)**: silent_error_ban 정합 default refuse 후 --force 측 explicit override 허용. own2 측 honest emit 측 정합 (refuse 측 emit FAIL, force 측 emit PASS with explicit force 측 reason). 그러나 --force 측 misuse 측 위험 측 잔존 — cycle 2 측 logging trail (TSV emit) 측 추가 검토.
- **C5 (status enum 측 superset)**: cond.status `unmet|partial|met|retired_intentional` 4 enum 측 lockdown. blocker.status `open|closed|acknowledged|warn` 측 superset (현재 14 .roadmap 측 union). 측 superset 측 정합 검증 측 cycle 2 측 추가 — 새 enum value 측 도입 시 explicit allow-list 측 update 측 누락 측 risk 잔존.
- **C6 (atomic tmp+mv write 측 cross-FS limitation)**: tmp file 측 같은 dir 측 mv 측 atomic 보장. 다른 FS (e.g., NFS) 측 atomic 보장 X — repo-local FS 측 한정 정합. raw 15 env_lazy / repo-relative paths 정합 enforcement.
- **C7 (BG subagent rate-limit 측 partial recovery 측 risk)**: 본 cycle 측 4 산출물 중 2 (tool + tests) 측 BG subagent disk land, 2 (handoff + marker) 측 main thread recovery. main thread 측 작성 측 BG subagent 측 final-report 측 sha256 매트릭스 측 부재 — 직접 disk 측정 측 sha256 측 marker 측 pin (post-hoc verification, predecessor cycle 측 sha256 cross-verify pattern 정합).
- **C8 (verify subcmd 측 cycle 2 측 multi-file matrix mode 측 deferred)**: 본 cycle 측 single-file verify 측 land. all 14 .roadmap.* 측 single emit 매트릭스 측 cycle 2. CI-style integration 측 cycle 3+ 측 별도 cycle.
- **C9 (own1 정합 측 anima posture 측 self-audit 측 측 dependency)**: anima sibling 측 study-only posture 측 invariant 측 자가 audit. 본 repo 측 plugin 측 distribution 측 anima 측 absent 시 tool 측 정상 동작 측 standalone clone safety 측 maintain. 단 BG subagent 측 prompt 측 anima path absolute 측 reference 측 raw 15 env_lazy 측 marginal 정합 (실제 코드 측 anima path 측 0 회 hardcode).

## §7. next-cycle hooks

- **Cycle 2 — full 11 subcmd extension**: 6 deferred subcmd land (edit-blocker / promote-met / dump-evidence / diff / migrate-NOOP / archive). promote-met 측 verifier guard pipeline 측 핵심.
- **Cycle 3 — CI integration**: `tool/roadmap_op.hexa verify` 측 git pre-commit hook OR launchd watchdog 측 wire (own1 정합 hooks/ infra 측 분리 cycle).
- **Cycle 4 — cross-game diff matrix**: `diff` subcmd land 후 11-game (+ 3 loader) 측 cond/blocker delta 측 dashboard emit (read-only audit).
- **Track H decision doc 측 implementation_status field 측 update**: Track H marker 측 `implementation_status: pending_user_lock_in` → cycle 2 cycle 3 측 `partial_5_of_11` / `full_11_of_11` 측 progressive update.
- **Anima sibling 측 cycle 2 측 11-subcmd 측 full surface 측 land 후 측 형식 정합 cross-check**: anima 측 11 subcmd 측 emit 측 본 repo 측 emit 측 mirror (string format / arg signature / exit code) 측 audit cycle.
