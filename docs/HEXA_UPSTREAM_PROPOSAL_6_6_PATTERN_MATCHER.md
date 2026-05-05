# HEXA_UPSTREAM_PROPOSAL §6.6 — detection-vector pattern matcher (DEFERRED, forward-looking)

> Track BC deferred-posture rationale doc — §6.6 (Track AO ledger entry "detection-vector
> pattern matcher", Warden L2 long-horizon) 의 RFC submission 은 forward-looking entry
> 로 **현재 PR-ready 가 아님**. C-hit count 0 (실 hand-roll 코드 0 줄) 상태에서 RFC
> 제출은 premature. 본 문서는 "왜 §6.6 가 현 시점 RFC submit 대상이 아닌가" 의 정형화
> 한정. PR #47 merged 2026-05-04T11:48:58Z 와는 별개 — sibling repo NOT touched.

## §0 목적 / 한계

본 문서는 Track BC cycle 의 §6.6 → DEFER 결정 근거 정형화. Track AO ledger §6.6
("detection-vector pattern matcher", Warden L2 long-horizon) 는 본래 forward-looking
등재 — `lib/perf/c_bnet_warden_environment_compat.hexa` 측 honest enumeration 만 존재,
실제 byte-level pattern matching 코드는 0 줄. 본 cycle 은 §6.6 를 PR-ready RFC 로
승격하지 않고, "current evidence 부재로 RFC submit 보류" 의 정형화 한정.

doc-only / additive / migration 0 / destructive 0 / sibling repo modifications 0 /
in_place_writes 0. 본 cycle 은 §6.6 spec 자체의 PR submission 을 진행하지 않으며,
"submit 하지 않는다는 결정" 의 정형화 한정 (§6.4 RETIRE 와 평행 구조 — but §6.6
는 RETIRE 가 아닌 DEFER, 미래 cycle 에 revisit 가능).

## §1 problem statement — forward-looking 의 의미

`lib/perf/c_bnet_warden_environment_compat.hexa` (Track J 측 honest tracking lib)
는 detection-vector enumeration 만 존재 — TSV-row format 으로 후보 vector 를 list,
실제 byte-level pattern matching 코드는 0 줄. C-hit count = 0.

본 entry 가 ledger 에 등재된 이유 (HEXA_UPSTREAM.md §6.6 cite "ledger 완전성 목적의
forward-looking 등재"):
- 미래 real Warden environment alignment cycle (multi-year horizon per L2 acknowledgment)
  시점에 byte-level pattern matching (process tree fingerprint / syscall sequence
  pattern / DLL loader_data_table_entry walk) 이 필요할 가능성.
- hexa stage1 의 regex 부재 (§1.3 항목) 와 별개로 byte-level matching 도 stdlib 후보.
- C-hit 가 0 이지만 "ledger 가 완전하다는 own 2 honest 정합" 측면에서 등재.

→ §6.6 를 현 시점 RFC submit 시 측정 가능한 부담:
- 가설 use-case 의 spec 만으로 API freeze 시 future actual hit-pattern 과 mismatch 위험.
- sibling repo reviewer 측 "no evidence, no use-case" 거부 가능성 — Track AY local
  commit `14218c0b` 의 rfc_012 / rfc_013 / rfc_014 는 모두 C-hit 13 / 21+ / 110+
  의 강한 evidence 동반. §6.6 는 0 hit 으로 PR review 시점에 reviewer burden 증가.
- own 2 honest 정합으로 "현재 hand-roll 0 줄, 가설 spec 만" 의 status 가 reviewer 에게
  명시적 — RFC 제출 자체가 own 2 정합 risk.

## §2 proposed API — 가설 (forward-looking, 본 cycle 제출 X)

```hexa
// std_pattern — byte-level / glob pattern matcher (가설, forward-looking)
// 본 모듈은 future Warden alignment cycle 시점에 actual hit-site 동반 시 RFC 승격.
// 본 cycle (Track BC) 은 spec 자체의 PR submission 진행 X.

fn pattern_match_byte_seq(haystack: bytes, needle: bytes) -> int
    // -1 if not found, byte offset if found.
    // 가설: KMP / Boyer-Moore 알고리즘 native 구현.
    // current C-hit: 0 (가설 use-case 만).

fn pattern_glob(haystack: str, pattern: str) -> bool
    // glob 패턴 (예: "core.*.exe") 매칭.
    // 가설: stage1 의 regex 부재 (§1.3) 와 별개의 simpler glob.
    // current C-hit: 0 (가설 use-case 만).

fn pattern_match_byte_seq_all(haystack: bytes, needle: bytes) -> array
    // -1 의 single-match 가 아닌, 전체 occurrence offset list.
    // 가설: process tree fingerprint 의 multi-occurrence 매칭 시 필요.
    // current C-hit: 0.
```

**중요**: 본 §2 의 signature 는 **freeze 가 아님** — forward-looking 가설 spec.
Track BC cycle 은 본 signature 를 sibling repo 측 RFC 로 submit 하지 않음. 미래
cycle 에서 actual hit-site (예: real Warden environment alignment 의 첫 byte-level
pattern code 작성) 가 발생 시 본 spec 을 base 로 actual API freeze 진행.

## §3 reference impl outline — 미작성 (forward-looking)

§6.6 는 forward-looking — actual reference impl skeleton 작성은 premature.
가설 algorithm 후보 (informational, 본 cycle 작성 X):
- Boyer-Moore / KMP / Rabin-Karp 중 hexa stage1 의 native syscall dispatch 호환 algorithm.
- glob → finite automaton 변환 (hexa stage1 의 regex 부재 와 별개의 simpler subset).

본 outline 의 actual hexa pseudo 작성은 future cycle (real Warden alignment cycle
시점) 의 spec 작성 단계 — Track BC 는 forward-looking 등재 한정.

## §4 test cases — 미작성 (forward-looking)

§6.6 의 actual test case 는 future cycle 의 actual hit-site 가 발생 시 작성. 가설
test scenario (informational):
- byte-level pattern: `pattern_match_byte_seq(b"abcdef", b"cd") == 2` — basic offset.
- not found: `pattern_match_byte_seq(b"abcdef", b"xy") == -1`.
- glob: `pattern_glob("core.exe", "core.*") == true`.
- multi-occurrence: `pattern_match_byte_seq_all(b"abab", b"ab") == [0, 2]`.

본 4 가설 case 는 reference 만 — Track BC cycle 의 actual test land 0.

## §5 breaking changes — 해당 없음

§6.6 는 본 cycle 에서 신규 API 제안 X (가설 spec 한정). Warden alignment 측 hand-roll
fn 는 현재 0 줄 — migration 대상 자체 없음.

미래 cycle 의 actual RFC 승격 시에도 신규 module 추가 (additive) 형태 가정 — breaking
change 0 정합 유지.

## §6 alternative considered — DEFER 외 옵션

**Option A** (REJECTED — 본 cycle 진행 X): §6.6 를 forward-looking RFC 로 submit,
가설 spec freeze + sibling repo 측 `proposals/rfc_016_std_pattern.md` land.
**Verdict**: rejected. C-hit 0 / actual use-case 0 / reviewer burden 증가. own 2
honest 정합 risk — "no evidence, hypothetical only" RFC 는 reviewer 측 거부 가능성
및 spec mismatch 위험.

**Option B** (REJECTED): §6.6 entry 를 ledger 에서 RETIRE — "ledger 완전성 목적의
forward-looking 등재" 자체를 부정.
**Verdict**: rejected. RETIRE 는 "rfc 가 cover 했음을 인정" 의 의미 — §6.6 는
어떤 rfc 도 cover X. RETIRE 부적합. DEFER 가 정확.

**Option C** (ADOPTED): §6.6 DEFER — Track BC deferred-posture rationale doc (본
문서) 으로 "current evidence 부재로 RFC submit 보류, future cycle 에 revisit" 의
정형화. ledger 측 next-cycle status field 가 `deferred_no_current_evidence_forward_looking_only`
로 정착.
**Verdict**: 본 cycle adopted.

**Option D** (forward-looking, 본 cycle 외): §6.6 escalation 의 trigger condition
명시 — "real Warden alignment cycle 시 첫 byte-level pattern code 작성 발생 시점".
**Verdict**: future cycle 에서 trigger event 발생 시 본 §6.6 doc 의 §0 / §1 update
+ actual API freeze 진행. Track BC scope 외.

## §7 sibling PR submission spec — 해당 없음

§6.6 는 본 cycle 에서 PR submission 진행 X. 가설 spec 의 sibling repo 측 RFC
file naming 후보 (informational, 본 cycle land 외):
- `proposals/rfc_NNN_std_pattern.md` — Track AY naming convention 정합 (rfc_NNN_short_name.md).
- 미래 cycle 에서 actual hit-site 동반 시 위 path 로 land 가능.

본 sibling repo land 는 Track BC scope 외 — 미래 real Warden alignment cycle 시점
에 별도 진행.

## §8 dependencies — 해당 없음 (현 시점)

§6.6 는 forward-looking — 현 시점 의존 0. 미래 cycle 의 actual API freeze 시 후보
의존:
- hexa stage1 의 native syscall dispatch (string / bytes manipulation builtin).
- rfc_013 의 `json_decode` (process tree fingerprint 가 JSON 형식 일 경우).
- §6.5 (rfc_015 가설) 의 `eprintln` (silent-error-ban 정합 path 의 stderr warn).

본 의존 list 는 가설 — actual freeze 시점에 confirm.

## §9 caveats (≥6, 11 listed)

C1. 본 문서는 §6.6 DEFER 결정 의 정형화 — actual RFC / impl / test / PR 모두 본
    cycle 진행 X. Track BC scope: deferred-posture rationale doc 한정.
C2. PR #47 (rfc_012 / rfc_013 / rfc_014) merged 2026-05-04T11:48:58Z cite 는
    Track BC mission directive 의 user-provided context — §6.6 는 PR #47 와
    독립 entry, sibling repo 측 추가 RFC 가 아닌 DEFER 결정 정형화.
C3. C-hit count = 0 의 explicit cite — own 2 honest 정합으로 §6.6 가 hand-roll
    code 0 줄 / 가설 use-case 만 의 status 가 본 doc 의 단일 source of truth.
    HEXA_UPSTREAM.md §6.6 ("ledger 완전성 목적의 forward-looking 등재") cite.
C4. trigger condition 명시 — "real Warden environment alignment cycle 의 첫
    byte-level pattern matching code 작성 시점". future cycle 에서 본 trigger
    발생 시 §6.6 escalation (DEFER → PR-ready) 진행.
C5. forward-looking 등재 자체의 own 2 honest risk — ledger 완전성 측 가치 vs reviewer
    측 "no evidence" 거부 가능성. Track BC 는 후자 risk 우회 (RFC submit 보류).
C6. 본 §2 가설 signature (`pattern_match_byte_seq` / `pattern_glob` /
    `pattern_match_byte_seq_all`) 는 freeze 가 아님 — future cycle actual API freeze
    시 변경 가능. 본 doc 의 signature 는 informational reference.
C7. hexa stage1 regex 부재 (§1.3) 와 별개 entry — regex (PCRE-class) 와 byte-level
    pattern (KMP / Boyer-Moore) + glob (simpler subset) 은 별도 module 후보. §1.3
    이 land 하더라도 §6.6 는 독립 entry 유지.
C8. multi-year L2 horizon per Warden environment alignment acknowledgment — Track J
    측 honest tracking 의 multi-year horizon 가정 정합. 본 cycle 은 multi-year horizon
    중 t=0 시점의 ledger 정형화 한정.
C9. own 5 status: §6.6 = `deferred_no_current_evidence_forward_looking_only`.
    Track BC marker `next_3_rfcs_status.6_6: "deferred_no_current_evidence_forward_looking_only"`
    정착. C-hit 0 / B-bench N/A (no impl) / PR-ready N/A (no evidence).
C10. raw 10 caveats ≥6 — 본 doc 11 caveats 로 여유.
C11. Track BC 는 doc-only — `lib/` / `tool/` / `tests/` / sibling repo / 기존 docs
     모두 read-only / modifications 0.

## §10 forward-looking entry rationale 상세

본 §10 은 §0-§9 의 보충 — Track AO ledger §6.6 의 forward-looking 등재 의 own 2
honest 정형화.

**forward-looking 등재 의 정의** (HEXA_UPSTREAM.md §6.6 cite):
- C-hit count = 0 (현 시점, hand-roll 코드 0 줄).
- ledger 완전성 목적 — "미래 발생 가능한 use-case" 의 사전 등재.
- multi-year horizon 의 L2 acknowledgment 정합.

**왜 ledger 등재 만으로 RFC 승격이 부족한가**:
1. RFC 는 sibling repo reviewer 가 평가 — "현재 hand-roll 패턴 / hit-count / actual
   use-case" 가 review 의 핵심 evidence. C-hit 0 RFC 는 reviewer 가 "speculative,
   no evidence" 로 거부 가능성 높음.
2. spec freeze 의 timing risk — actual hit-pattern 발생 전 spec freeze 시 future
   use-case 와 mismatch. signature 변경은 breaking change risk 동반.
3. own 2 honest 정합 — "evidence 있다" vs "evidence 없다, 가설 만" 의 명시적 구분이
   doc 측에서 reviewer 에게 visible 해야 함. C-hit 0 RFC 는 own 2 정합 자체에
   risk.

**Track BC 의 DEFER 결정의 reviewer-facing 메시지**:
- "§6.6 는 forward-looking — 현 시점 RFC submit X"
- "trigger condition: real Warden alignment cycle 의 첫 byte-level pattern code
  작성 시점"
- "trigger 발생 시 본 §6.6 doc 의 §0 / §1 update + actual API freeze 진행"
- "RETIRE 가 아닌 DEFER — entry 보존 + 미래 revisit 가능"

**§6.4 RETIRE 와의 평행 / 차이**:
- 평행: 둘 다 본 cycle 에서 RFC submit X. 둘 다 doc-only 정형화 한정.
- 차이: §6.4 는 RETIRE — rfc_013 가 fully cover 했음을 인정. §6.6 는 DEFER —
  actual evidence 부재로 보류, future revisit 가능.

**timeline (Track AO ~ Track BC)**:
- 2026-05-04 (Track AO retrofit): §6.6 ledger 등재. C-hit 0. 등재 평가: "ledger
  완전성 목적의 forward-looking 등재".
- 2026-05-04 (Track AV B-bench): §6.6 는 high-hit 3 entry 대상 X — bench harness
  추가 X. status `forward_looking_c_hit_zero` 유지.
- 2026-05-04 (Track AW PR-ready spec): §6.6 는 PR-ready 3 entry 대상 X — Track AW
  scope 외 (HEXA_UPSTREAM.md §7 합계 표 cite "forward-looking, real Warden 정합
  cycle 필요").
- 2026-05-04 (Track AY sibling local commit): rfc_012 / rfc_013 / rfc_014 가 sibling
  repo 측 land. §6.6 는 본 3 RFC 에 미포함 — forward-looking 유지.
- 2026-05-04 (PR #47 merged 11:48:58Z): rfc_012 / rfc_013 / rfc_014 합류. §6.6 는
  PR #47 와 독립 entry — 영향 없음.
- 2026-05-04 (Track BC, 본 cycle): §6.6 DEFER 결정 정형화 — 본 doc + 본 cycle
  marker 가 status 정착의 단일 source of truth.

**ledger row 의 post-Track-BC 표현** (informational, HEXA_UPSTREAM.md read-only):
- `| 6.6 | pattern matcher (forward) | Track J (none yet) | 0 | deferred (no current evidence) |`
- 본 표현은 future cycle 에서 HEXA_UPSTREAM.md edit 가 허용되는 track 시 적용 가능.
  Track BC 는 marker 측 status field 만 land.

---

*Track BC §6.6 DEFER deferred-posture rationale doc, written 2026-05-04. doc-only /
additive / migration 0 / destructive 0 / sibling repo modifications 0 /
in_place_writes 0.*
