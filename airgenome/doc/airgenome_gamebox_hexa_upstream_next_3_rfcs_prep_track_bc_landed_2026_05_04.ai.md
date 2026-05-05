# Track BC landed — hexa-lang upstream next 3 RFCs prep (§6.4 / §6.5 / §6.6) (2026-05-04)

## 0. 한 줄 요약

PR #47 (rfc_012 / rfc_013 / rfc_014) merged 2026-05-04T11:48:58Z 직후, Track BC 는
Track AO §6.4-§6.6 의 next 3 RFC 후보를 evaluate. 결과: §6.4 RETIRE (rfc_013 의
`json_path_get` 가 subsume) / §6.5 PR-ready (pending user explicit approval) /
§6.6 DEFER (forward-looking, C-hit 0). 본 cycle land = gamebox docs/ 측 3 rationale
doc + handoff + marker 한정 — sibling repo NOT touched.

## 1. 컨텍스트

- 선행 (Track AO retrofit, 2026-05-04): `docs/HEXA_UPSTREAM.md` §6 6 retrofit entry
  (6.1 ~ 6.6) ledger 등재. C-hit 합 ≈ 147+, B-bench 전체 deferred.
- 선행 (Track AV B-bench, 2026-05-04): §6.1 / §6.2 / §6.3 high-hit 3 entry 에 B-bench
  harness 추가. own 5 status `c_only_b_deferred` → `c_plus_b_pr_ready_pending_user_approval`.
- 선행 (Track AW PR-ready spec, 2026-05-04): §6.1 / §6.2 / §6.3 의 PR-ready proposal
  spec doc 3 가 gamebox `docs/` 에 land.
- 선행 (Track AY sibling local commit, 2026-05-04): rfc_012 / rfc_013 / rfc_014 가
  sibling repo `/tmp/hexa-lang-track-ay-worktree` branch `proposal/upstream-track-ao-aw-2026-05-04`
  의 local commit `14218c0b` 로 land. push / PR open 은 deferred.
- 선행 (PR #47 merged, 2026-05-04T11:48:58Z): rfc_012 / rfc_013 / rfc_014 가 hexa-lang
  upstream main 합류 (cite: Track BC mission directive 의 user-provided context).
- Track BC (본 cycle): next 3 RFC 후보 (§6.4 / §6.5 / §6.6) evaluate + status 정착
  doc 3 land. sibling repo NOT touched / no PR submission.

## 2. user directive (paraphrased, BR_NO_USER_VERBATIM 정합)

PR #47 merged 직후 Track AO §6.4 / §6.5 / §6.6 의 next 3 RFC 후보를 evaluate.
각 entry 에 대해 PR-ready 여부 / RETIRE / DEFER status 를 own 2 honest 평가로
결정. 결과 status 의 정형화 doc 3 + handoff + marker 5 NEW 한정 — sibling repo
PR submission 본 cycle 진행 X. 기존 docs / lib / tool / tests 모두 read-only.

## 3. 정책

- migration: forbidden / additive_only / in_place_writes=0
- destructive_ops: 0
- cost_usd: 0
- substrate: mac-local
- cap_minutes: 90
- BR_NO_USER_VERBATIM: true
- friendly_preset: true
- silent-land
- own1: Wine 0 / hexa-only / hexa-lang sibling **NOT** modified / NO PR submission
  this cycle
- own2: status honest per RFC (§6.4 retired / §6.5 pr_ready / §6.6 deferred) explicit
- raw 9 (doc-only) / 10 (≥6 caveats per doc — 본 cycle 11 caveats × 3 doc = 33
  caveats) / 11 (snake_case) / 12 (silent-error-ban) / 15 / 175

## 4. 결과

### 4.1 3 RFC 후보 evaluate 결과

| Entry | Verdict | Doc path | LOC | sha256 (head 16) |
|---|---|---|---|---|
| §6.4 marker JSON path read | RETIRE (subsumed by rfc_013) | `docs/HEXA_UPSTREAM_PROPOSAL_6_4_MARKER_JSON_PATH.md` | 227 | 40400cca751bbff2 |
| §6.5 stderr eprintln | PR-ready (pending user approval) | `docs/HEXA_UPSTREAM_PROPOSAL_6_5_STDERR_EPRINTLN.md` | 231 | ca3c179c0abaec37 |
| §6.6 pattern matcher | DEFER (forward-looking, C-hit 0) | `docs/HEXA_UPSTREAM_PROPOSAL_6_6_PATTERN_MATCHER.md` | 232 | 581b770c42920e0c |

각 doc 의 §0-§9 (+ §10 optional) 구조 정합. 각 doc caveats 11 — 33 caveats total.

### 4.2 own 5 status 정착 (Track BC 후)

| § | C-hit | B-bench | RFC verdict (Track BC) | own 5 status |
|---|---|---|---|---|
| 6.4 | 1 (Track AG) | N/A (RETIRE) | RETIRED — subsumed by rfc_013 `json_path_get` | `retired_subsumed_by_rfc_013_marker` |
| 6.5 | 2 (Track AE+AB) | deferred (correctness) | PR-ready spec land — pending user approval | `pr_ready_pending_user_approval` |
| 6.6 | 0 (forward-looking) | N/A (no impl) | DEFERRED — no current evidence | `deferred_no_current_evidence_forward_looking_only` |

`pr_submission_ready_count = 1` (only §6.5).

### 4.3 PR submission status — DEFERRED for §6.5

§6.5 가 PR-ready spec 형태로 정형화 됐지만 sibling repo 측 actual RFC submission
은 **deferred pending user explicit approval** — Track AY 의 staged approval 은
PR #47 (rfc_012 / rfc_013 / rfc_014) 의 local commit / push / PR open 까지로 한정
(`approval_excluded_scope: ["git_push", "gh_pr_create"]` cite Track AY marker).
§6.5 의 sibling repo land 는 별도 user 동의 필요.

`pr_submission_status: "deferred_pending_user_explicit_approval_for_section_6_5"`
(marker pinned).

deferred 이유 (own2 honest):
- own1 wine 0 / hexa-lang sibling NOT modified 정책 정합 — 본 cycle 은 doc-only.
- Track AY 의 staged approval 이 §6.1 / §6.2 / §6.3 (rfc_012 / _013 / _014) 한정.
  §6.5 (rfc_015 가설 후보) 는 본 staged approval 외 — 별도 명시적 동의 필요.

§6.5 의 user explicit approval 가 land 시 future cycle 의 작업 (본 cycle 미진행):
1. sibling repo `/Users/ghost/core/hexa-lang/proposals/rfc_015_core_io_eprintln.md`
   생성 — 본 §6.5 doc 의 rfc_NNN_short_name.md format 사본.
2. reference impl skeleton (§3 outline) 의 actual hexa code 작성 in `src/core/io.hexa`
   (기존 모듈 추가).
3. test case (§4) 의 actual test file 작성 in `tests/core/test_io_eprintln.hexa`.
4. PR open + 리뷰.

본 4 단계 모두 user explicit greenlight 후 별도 cycle.

### 4.4 Track BC scope 외 (unchanged)

- §6.1 / §6.2 / §6.3 — Track AY 가 rfc_012 / rfc_013 / rfc_014 로 land 완료. PR #47
  merged. Track BC scope 외.
- HEXA_UPSTREAM.md ledger row 의 §6.4 / §6.5 / §6.6 표현은 read-only — Track BC marker
  + 본 doc 가 status 정착의 단일 source of truth. ledger row inline edit 는 future
  track 의 별도 cycle.

## 5. 파일 목록 (5 NEW)

1. `docs/HEXA_UPSTREAM_PROPOSAL_6_4_MARKER_JSON_PATH.md` (227 LOC, NEW)
2. `docs/HEXA_UPSTREAM_PROPOSAL_6_5_STDERR_EPRINTLN.md` (231 LOC, NEW)
3. `docs/HEXA_UPSTREAM_PROPOSAL_6_6_PATTERN_MATCHER.md` (232 LOC, NEW)
4. `airgenome/doc/airgenome_gamebox_hexa_upstream_next_3_rfcs_prep_track_bc_landed_2026_05_04.ai.md` (본 handoff, NEW)
5. `state/markers/airgenome_gamebox_hexa_upstream_next_3_rfcs_prep_track_bc_landed.marker` (NEW)

수정: 0. 삭제: 0. in_place_writes: 0.

## 6. 검증

- 3 proposal doc 모두 200-300 LOC 범위 ✓ (227 / 231 / 232)
- 각 doc §0-§9 (+ §10 optional) section 정합 ✓
- 각 doc caveats ≥6 ✓ (실제 11 each → 33 total)
- API signature freeze §2:
  - §6.4: NONE (RETIRE — rfc_013 측 위임) ✓
  - §6.5: 2 fn (`eprintln` / `stderr_write`) ✓
  - §6.6: 가설 3 fn (forward-looking, freeze X) ✓
- alternative section §6 ✓ (각 doc 3-4 option × 명시적 verdict)
- sibling PR submission spec §7:
  - §6.4: 해당 없음 (RETIRE) ✓
  - §6.5: 디렉터리 layout / commit msg / 리뷰 checklist ✓
  - §6.6: 해당 없음 (DEFER) ✓
- marker `pr_47_status: "merged_2026_05_04T11_48_58Z"` ✓
- marker `next_3_rfcs_status` 3-entry 정착 ✓
- marker `pr_submission_ready_count: 1` ✓
- marker sha256 pinned ✓
- in_place_writes: 0 ✓ (모든 5 파일 NEW)
- sibling repo `/Users/ghost/core/hexa-lang/` 0 modification ✓
- 기존 `docs/HEXA_UPSTREAM.md` 0 modification ✓ (Track AO/AV territory read-only)
- 기존 `state/markers/*.marker` 0 modification ✓
- airgenome sibling project (`~/core/airgenome/`) 0 modification ✓ (Track AX territory)

## 7. 주의 / caveats (Track BC 자체)

A1. 본 cycle 은 doc-only — sibling repo / lib / tool / tests / 기존 docs 모두 read-only.
A2. 본 cycle PR submission 자체 진행 X — §6.5 가 PR-ready 이지만 sibling repo land 는
    별도 user explicit approval pending. marker `pr_submission_status` 가
    `deferred_pending_user_explicit_approval_for_section_6_5` 명시.
A3. PR #47 merged 2026-05-04T11:48:58Z cite 는 Track BC mission directive 의
    user-provided context — 본 cycle 은 sibling repo `git log` verification 진행 X
    (own 1 hexa-lang sibling NOT modified 정합으로 read-only verification 도 본 cycle
    scope 외).
A4. 3 RFC 후보 verdict (RETIRE / PR-ready / DEFER) 는 own 2 honest 정합 — C-hit
    count + actual evidence 기준의 evaluate 결과. future cycle 에서 evidence 변경
    (예: §6.6 의 actual hit-site 발생) 시 verdict revisit.
A5. §6.4 RETIRE 결정 의 reviewer-facing risk — rfc_013 의 `json_path_get` 가 actual
    stage1 land 시 syntax / semantics 미세 차이 가능. PR review 시점에 spec 변경
    시 §6.4 RETIRE rationale 도 revisit 필요.
A6. §6.5 의 native syscall write(2) dispatch 가정은 hexa stage1 의 internal infrastructure
    의존 — sibling repo PR review 시점에 actual implementation path (native vs FFI vs
    shell escape default) 가 spec 의 fork cost claim 과 정합 검증 필요.
A7. §6.6 DEFER 결정 의 trigger condition — "real Warden alignment cycle 의 첫
    byte-level pattern code 작성 시점" — 은 multi-year horizon. Track BC 는 multi-year
    horizon t=0 시점의 ledger 정형화 한정.
A8. parallel BG Track AZ / BA / BB 와 file scope 분리 — Track BC 는 gamebox `docs/`
    + 본 cycle marker 한정. 5 NEW total / 0 modifications.
A9. raw 10 ≥6 caveats per doc — 각 doc 11 caveats (3 doc × 11 = 33 total) 로 여유.
    handoff 자체도 ≥8 (실제 12) 로 여유.
A10. spec doc LOC (227 / 231 / 232) 은 §0-§9 + §10 optional 구조 — 실제 sibling repo
     land 시 ±30% 변동 가능 (rfc_NNN_short_name.md 측 format 차이). own 2 honest
     정합으로 본 LOC 는 estimate 명시.
A11. `pr_submission_ready_count = 1` 은 본 cycle 시점의 status — future cycle 에서
     §6.6 의 evidence 발생 시 `2` 로 진전, §6.5 의 user approval 후 sibling repo
     land 시 `0` 로 collapse (post-submit 분류).
A12. 본 cycle 은 cleanest cycle — `in_place_writes: 0` / 5 NEW total / 0 modifications
     / 0 deletions. Track AO ~ Track AY series 중 본 cycle 이 가장 minimal disk
     footprint.

---

*Track BC next 3 RFCs prep doc-only landed 2026-05-04. doc-only / additive /
migration 0 / destructive 0 / sibling repo modifications 0 / in_place_writes 0 /
pr_submission_ready_count 1 (only §6.5).*
