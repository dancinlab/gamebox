# HEXA_UPSTREAM_PROPOSAL §6.4 — marker JSON path read (RETIRED, subsumed by rfc_013)

> Track BC consolidation rationale doc — §6.4 (Track AO ledger entry "marker JSON
> path read") 가 PR #47 merged 2026-05-04T11:48:58Z 의 `rfc_013_std_json_decode_path.md`
> 에 포함된 `json_path_get` API 로 fully covered. 본 문서는 §6.4 를 별도 RFC 로
> 추진하지 않고 RETIRED status 로 ledger 정리 한다는 결정의 근거 정형화.
> hexa-lang sibling repo NOT touched — gamebox docs/ 측 consolidation rationale doc
> 한정.

## §0 목적 / 한계

본 문서는 Track BC cycle 의 §6.4 → RETIRE 결정 근거 정형화. Track AO ledger §6.4
("marker JSON path read") 는 본래 §6.2 std_json 의 sub-entry 로 등재되어 있었고
(HEXA_UPSTREAM.md §7 합계 표 cite "deferred per 6.2"), Track AY 가 sibling repo
local commit 로 land 한 PR #47 (rfc_012 / rfc_013 / rfc_014) 의 rfc_013 가 정확히
`json_path_get(value, dotted_path)` API 를 포함. 본 cycle (Track BC) 는 §6.4 를
독립 RFC 로 submit 하지 않고 retired status 로 정리.

doc-only / additive / migration 0 / destructive 0 / sibling repo modifications 0 /
in_place_writes 0. 본 cycle 은 §6.4 spec 자체의 PR submission 을 진행하지 않으며,
"submit 하지 않는다는 결정" 의 정형화 한정.

## §1 problem statement — 왜 §6.4 가 별도 RFC 가 아닌가

Track AG (`tool/closure_orchestrator.hexa` line 593 의 `read_progress_field_from_marker`)
가 cycle marker 의 nested field (예: `win32_coverage_progression.percent`) 를
line-grep + manual substring 으로 hand-roll. 본 패턴은:

- input: marker file path + dotted field path (예: `"win32_coverage_progression.percent"`).
- 현재: `read_progress_field_from_marker(marker_path, field_path)` 가 line-grep + bracket
  walker 로 nested field 추출.
- C-hit count: 1 (closure_orchestrator 단일 도구의 단일 fn).

본 패턴은 정확히 rfc_013 의 `json_path_get` 으로 collapse 가능:

```hexa
// 현재 (Track AG hand-roll)
let pct = read_progress_field_from_marker(marker_path, "win32_coverage_progression.percent")

// rfc_013 land 후
let v = json_decode(read_file(marker_path))
let pct = json_path_get(v, "win32_coverage_progression.percent")
```

§6.4 의 모든 use-case 는 §6.2 의 `json_path_get` + `json_decode` 의 2-fn composition
으로 covered. §6.4 가 추가하는 unique API 가 0.

## §2 proposed API — 없음 (intentional empty)

§6.4 는 Track BC 결정으로 **별도 신규 API 제안 없음**. 모든 use-case 가
rfc_013 의 기존 API 로 cover:

```hexa
// rfc_013 (PR #47 merged 2026-05-04T11:48:58Z) 측 API
fn json_decode(s: str) -> any
fn json_path_get(value: any, dotted_path: str) -> any
```

§6.4 가 별도 신규 API 제안 시 후보였던 가설 (rejected):
- `marker_field_get(marker_path: str, field_path: str) -> str` — file IO + json decode
  + path get 의 wrapper. rejected: 3-step composition 을 1-fn 으로 wrap 하는 sugar 일
  뿐 — own 1 hexa-only stdlib 의 minimal API surface 정합 X. caller 측 명시적
  composition 이 더 명확.

## §3 reference impl outline — 없음

§6.4 가 신규 impl 제안 X. 기존 closure_orchestrator 측 `read_progress_field_from_marker`
는 rfc_013 land 후 다음 cycle 에서 migrate 가능 (migration 0 정책 정합으로
**deprecate 강제 X** — 기존 hand-roll 호환 layer 유지, 신규 코드만 권장).

migration 후보 hexa pseudo (informational, 본 cycle land 외):

```hexa
// 미래 cycle: Track AG 의 closure_orchestrator migrate
// 현재
fn read_progress_field_from_marker_legacy(marker_path: str, field_path: str) -> str {
    // line-grep + manual substring (~40 LOC)
}
// 신규 (rfc_013 의존)
fn read_progress_field_from_marker_v2(marker_path: str, field_path: str) -> str {
    let s = read_file(marker_path)
    let v = json_decode(s)
    let result = json_path_get(v, field_path)
    return any_to_str(result)
}
```

본 migrate 는 Track BC scope 외 — Track BC 는 RETIRE 결정 정형화 한정.

## §4 test cases — 없음 (rfc_013 측에 위임)

§6.4 의 모든 test scenario 는 rfc_013 의 `json_path_get` test case (sibling repo
local commit `14218c0b`, gamebox source `HEXA_UPSTREAM_PROPOSAL_6_2_STD_JSON.md`
의 §4 12 cases) 가 cover. §6.4 는 별도 test 추가 X.

rfc_013 의 §4 cases 중 §6.4 use-case 를 직접 cover 하는 항목:
- case 7: `json_path_get(decoded, "key1.key2") == nested_value` — basic dotted path
- case 8: `json_path_get(decoded, "blockers.0.status") == specific_array_index` — array
  index access
- case 9: `json_path_get(decoded, "missing.path") == null` — missing field 처리
- case 10: malformed marker (JSON decode 실패) → `json_decode` 가 null + warn → caller
  의 null check.

§6.4 use-case 는 위 4 case 의 합성으로 fully covered.

## §5 breaking changes — 없음

§6.4 는 본 cycle 에서 신규 API 제안 X. closure_orchestrator 의 기존 hand-roll
fn 은 rfc_013 land 후에도 호환 유지 (migration 0 정책 정합).

미래 cycle 의 closure_orchestrator migrate 는 caller-side 변경 한정 — hexa-lang
core / stdlib 측 breaking change 0.

## §6 alternative considered — RETIRE 외 옵션

**Option A** (REJECTED — 본 cycle 진행 X): §6.4 를 별도 RFC 로 submit, 신규
`marker_field_get(marker_path, field_path)` 1-fn API 추가.
**Verdict**: rejected. rfc_013 의 `json_decode` + `json_path_get` 2-fn 으로 fully
covered. 신규 1-fn 은 sugar layer 일 뿐 — own 1 minimal stdlib API surface 정합 X.

**Option B** (REJECTED): §6.4 를 §6.2 의 sub-entry 로 explicit re-list (HEXA_UPSTREAM.md
§7 의 "deferred per 6.2" cite 를 "subsumed by rfc_013" 로 재기술).
**Verdict**: 의미 있는 변경 X — 이미 §7 의 "deferred per 6.2" 가 implicit 으로
subsumption 을 표현. HEXA_UPSTREAM.md 가 read-only 인 본 cycle scope 정합으로
inline 수정 X.

**Option C** (ADOPTED): §6.4 RETIRE — Track BC consolidation rationale doc
(본 문서) 으로 RETIRE 결정 정형화. ledger 측 next-cycle 의 status field 가
`retired_subsumed_by_rfc_013_marker` 로 정착.
**Verdict**: 본 cycle adopted.

## §7 sibling PR submission spec — 해당 없음

§6.4 는 별도 PR submission 진행 X. Track BC 는 §6.4 를 RETIRE 로 정리 한정 —
sibling repo `/Users/ghost/core/hexa-lang/proposals/` 측 추가 RFC 파일 land 0.

future cycle 의 closure_orchestrator migrate (informational):
- §6.4 와 직접 관련 X — closure_orchestrator 의 `read_progress_field_from_marker`
  는 caller-side 코드 — gamebox `tool/` 측 변경. 본 cycle scope 외 (read-only).

## §8 dependencies

§6.4 는 신규 의존성 0. 모든 의존이 rfc_013 (PR #47 merged 2026-05-04T11:48:58Z)
의 기존 API:
- `json_decode(s: str) -> any`
- `json_path_get(value: any, dotted_path: str) -> any`

closure_orchestrator 측 migrate 는 rfc_013 의 actual hexa stage1 land (`std_json`
모듈의 사실 import 가능 시점) 후로 deferred — 현 시점 (PR merged but 아직 stage1
runtime 미합류 가능성) 의 migrate 는 premature.

## §9 caveats (≥6, 11 listed)

C1. 본 문서는 §6.4 RETIRE 결정 의 정형화 — 신규 API / impl / test / PR 모두 없음.
    Track BC scope: RETIRE rationale doc 한정.
C2. PR #47 (rfc_012 / rfc_013 / rfc_014) merged 2026-05-04T11:48:58Z cite 는
    Track BC mission directive 의 user-provided context — 본 cycle 은 sibling
    repo `git log` verification 진행 X (own 1 hexa-lang sibling NOT modified
    정합으로 read-only verification 도 본 cycle scope 외).
C3. closure_orchestrator 측 hand-roll `read_progress_field_from_marker` 의 migrate
    는 본 cycle scope 외 — 미래 cycle 에서 actual stage1 `std_json` import 가능 시점에
    별도 진행. 현 시점 migrate 는 premature.
C4. HEXA_UPSTREAM.md §7 합계 표의 §6.4 row ("deferred per 6.2") 는 read-only —
    본 cycle 은 inline 수정 X. ledger 측 RETIRE status 정착은 본 doc + 본 cycle
    marker 가 단일 source of truth.
C5. own 2 honest: §6.4 가 별도 unique value 0 임을 explicit 명시 — RETIRE 가
    "기능 부재" 아니고 "rfc_013 가 fully cover 했음을 인정" 의 의미. low-hit 1 (Track
    AG single fn) 와 무관하게, hit count 가 더 많아도 RETIRE 결정 동일 (rfc_013
    composition 으로 cover 가능 여부가 기준).
C6. Track AO ledger §6.4 는 Track BC marker 측 `next_3_rfcs_status` field 의
    `6_4: "retired_subsumed_by_rfc_013_marker"` 로 status 정착. future track 가
    §6.4 를 다시 살릴 사유 (예: rfc_013 미land / `json_path_get` 의 unforeseen
    한계) 가 발생 시 Track BC RETIRE 결정 revisit.
C7. rfc_013 의 `json_path_get` 가 actual stage1 land 시 syntax / semantics 미세 차이
    가능 — sibling repo PR review 시점에 spec 변경 시 §6.4 RETIRE rationale 도
    revisit 필요.
C8. closure_orchestrator 의 marker 는 strict single-line JSON 가정 (mk2 convention)
    — multi-line JSON 마커가 future cycle 에 등장 시 `json_decode` 측 streaming
    variant (rfc_013 §0 caveat C8 cite "future extension") 가 필요할 수 있음.
C9. own 5 status: §6.4 = `retired_subsumed_by_rfc_013_marker` (Track BC 정착).
    C-hit 1 / B-bench N/A (RETIRE entry 는 bench 측정 무의미) / PR-ready N/A.
C10. raw 10 caveats ≥6 — 본 doc 11 caveats 로 여유.
C11. Track BC 는 doc-only — `lib/` / `tool/` / `tests/` / `state/markers/` (본 cycle
     marker 외) / sibling repo 모두 read-only / modifications 0.

## §10 ledger consolidation rationale 상세

본 §10 은 §0-§9 의 보충 — Track AO ledger §6.4 row 의 evolution timeline 정형화.

**timeline**:
- 2026-05-04 (Track AO retrofit cycle): §6.4 ledger 등재. C-hit 1 (closure_orchestrator
  `read_progress_field_from_marker`). 등재 시점 평가: §6.2 std_json sub-entry 로
  통합 진행 권고 — `deferred per 6.2`.
- 2026-05-04 (Track AV B-bench cycle): §6.4 는 high-hit 3 entry (§6.1 / §6.2 / §6.3)
  대상이 아님 — bench harness 추가 X. status `c_only_b_deferred` 유지.
- 2026-05-04 (Track AW PR-ready spec cycle): §6.4 는 PR-ready 3 entry 대상이 아님 —
  Track AW scope 외 (HEXA_UPSTREAM.md §7 합계 표 cite "deferred per 6.2").
- 2026-05-04 (Track AY sibling local commit cycle): rfc_012 / rfc_013 / rfc_014 가
  sibling repo 측 worktree 에 land. rfc_013 의 `json_path_get` API 가 §6.4 use-case
  를 fully cover.
- 2026-05-04 (PR #47 merged 11:48:58Z): rfc_012 / rfc_013 / rfc_014 가 hexa-lang
  upstream main 에 합류. §6.4 의 ledger status 가 implicit 으로 "subsumed" 로 진입.
- 2026-05-04 (Track BC, 본 cycle): §6.4 RETIRE 결정 정형화 — 본 doc + 본 cycle
  marker 가 status 정착의 단일 source of truth.

**ledger row 의 future post-Track-BC 표현** (informational, HEXA_UPSTREAM.md
read-only 정책으로 본 cycle inline 변경 X):
- `| 6.4 | marker JSON path read | Track AG | 1 | retired (subsumed by rfc_013) |`
- 위 row 표현은 future cycle 에서 HEXA_UPSTREAM.md edit 가 허용되는 track 가
  진행 시 적용 가능. Track BC 는 marker 측 status field 만 land.

**RETIRE 후 ledger 측 entry 보존 이유**:
- §6.4 entry 자체를 ledger 에서 제거 X — own 2 honest 정합으로 "RETIRE 됐다는
  사실" 도 ledger 의 일부. future track 가 §6.4 를 다시 살릴 사유 (예: rfc_013
  의 unforeseen 한계) 발생 시 entry 가 ledger 에 보존되어 있으면 revisit 가능.
- entry 제거 (RETIRE → 삭제) 는 own 2 honest 의 audit trail 손실 — 보존이 정합.

**Track BC 의 §6.4 RETIRE 결정 의 reviewer-facing 메시지**:
- "§6.4 RETIRE 는 기능 부재 의 수용 X — rfc_013 가 fully cover 했음을 인정"
- "future revisit 가능 — ledger entry 보존 + status field 만 정착"
- "actual closure_orchestrator migrate 는 별도 cycle (rfc_013 stage1 land 시점 후)"

---

*Track BC §6.4 RETIRE consolidation rationale doc, written 2026-05-04. doc-only /
additive / migration 0 / destructive 0 / sibling repo modifications 0 /
in_place_writes 0.*
