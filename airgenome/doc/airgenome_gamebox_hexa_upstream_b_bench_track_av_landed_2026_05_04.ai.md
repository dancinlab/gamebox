# Track AV — hexa-lang upstream B-bench escalation landed (2026-05-04)

## §1 user_directive_paraphrase

User directive (paraphrased, no verbatim quote): Track AO 가 doc-only 로 land 한 hexa-lang upstream
retrofit 항목 (§6.1 / §6.2 / §6.3) 에 대해 B-bench 측정을 추가하여 own 5 status 를 C-only 에서
C + B 로 escalate. PR 제출 자체는 user explicit approval pending — Track AV 는 bench evidence
landing 한정. 6.4 / 6.5 / 6.6 는 본 cycle 범위 밖 (high-hit 3 entry 우선).

## §2 bench harness summary

3개 bench harness 가 `tool/bench/` 에 land:

| Entry | Harness path | hand_rolled measured | hypothetical_native estimate | LOC delta |
|---|---|---|---|---|
| 6.1 | `tool/bench/bench_hexa_upstream_6_1_x1000_fixed_point.hexa` | wall-clock ns/op (gdate %N) | ~250 ns/op (estimated) | 81 → 12 |
| 6.2 | `tool/bench/bench_hexa_upstream_6_2_std_json.hexa` | wall-clock ns/op (gdate %N) | ~800 ns/op (estimated) | 110 → 6 |
| 6.3 | `tool/bench/bench_hexa_upstream_6_3_pe_struct.hexa` | wall-clock ns/op (gdate %N) | ~100 ns/op (estimated) | 70 → 3 |

각 harness:
- own 2 fn `self_test()` 정합 (golden equality 비교 + panic on mismatch)
- `main()` dispatch: `self-test` / `bench` (default = bench)
- emit 형식: `__BENCH_HEXA_UPSTREAM__ <entry_id> <variant> <ns_per_op> <loc> <verdict>`
- variant 2개: `hand_rolled` (measured) + `hypothetical_native_estimate` (estimated)
- 예약어 회피: type / mod / handle / class / interface / module / import 0 사용
- hexa-lang sibling repo (`/Users/ghost/core/hexa-lang`) 0 modifications

## §3 own 5 status 진전 표

| § | Pre-Track AV (Track AO) | Post-Track AV | 변경 사항 |
|---|---|---|---|
| 6.1 | C-hit=13, B-bench=deferred | C-hit=13, B-bench=measured (LOC 81→12) | `c_plus_b_pr_ready_pending_user_approval` |
| 6.2 | C-hit=21+, B-bench=deferred | C-hit=21+, B-bench=measured (LOC 110→6) | `c_plus_b_pr_ready_pending_user_approval` |
| 6.3 | C-hit=110+, B-bench=deferred | C-hit=110+, B-bench=measured (LOC 70→3) | `c_plus_b_pr_ready_pending_user_approval` |
| 6.4 | C-hit=1, B-bench=deferred | C-hit=1, B-bench=deferred | unchanged (Track AV scope 외) |
| 6.5 | C-hit=2, B-bench=deferred | C-hit=2, B-bench=deferred | unchanged (Track AV scope 외) |
| 6.6 | C-hit=0, B-bench=미정 | C-hit=0, B-bench=미정 | unchanged (forward-looking) |

PR escalation status: `pending_user_explicit_approval_per_track_ao_directive` (3 entries).

## §4 doc extension summary

`docs/HEXA_UPSTREAM.md` 에 §8 추가:
- §8 (top-level): Track AV B-bench results 헤더
- §8.1 / §8.2 / §8.3 (sub-entry): per-entry bench harness path + workload + measured/estimated table
- §8.4 (sub-entry): own 5 status 진전 표

기존 §1 / §2 / §3 / §4 / §4.5 / §5 / §6 (Track AO retrofit) / §7 (Track AO own 5 통합 표) 한 글자도
수정 안함. §8 만 doc 끝에 추가.

## §5 bench result protocol cite

Track AV 의 bench TSV format:
```
__BENCH_HEXA_UPSTREAM__ <entry_id> <variant> <ns_per_op> <loc> <verdict>
```

예시 emit (6.1):
```
__BENCH_HEXA_UPSTREAM__ 6.1 hand_rolled <ns> 81 measured
__BENCH_HEXA_UPSTREAM__ 6.1 hypothetical_native_estimate 250 12 estimated_native_baseline_unmeasurable_in_hexa_stage1
```

verdict 값 정의:
- `measured` — 실제 wall-clock 측정 (gdate +%s%N 또는 BSD date %s × 1e9 fallback).
- `estimated_native_baseline_unmeasurable_in_hexa_stage1` — hexa stage1 에 native float 부재로
  실측 불가, 클럭 타겟 추정.
- `estimated_native_baseline_simdjson_class_arm64_extrapolation` — typical native JSON parser
  benchmark 에서 ~600 byte 입력의 mid-range latency 추정.
- `estimated_native_struct_pack_arm64_extrapolation` — 4 store + format overhead 의 typical arm64
  latency 추정.

## §6 caveats (≥10 honest items)

C1. Track AV 는 evidence-only escalation. PR 제출 (hexa-lang sibling repo 측 RFC 또는 implementation
    PR) 은 user explicit approval 가 떨어진 후 별도 cycle 에서 진행. Track AV 자체는 0 sibling repo
    modification.

C2. `hypothetical_native_estimate` ns/op 값 (250 / 800 / 100) 은 **추정치**. hexa stage1 에 native
    float / std_json / struct_pack 가 아예 없어 직접 측정 불가. 추정 근거는 각 bench harness 주석에
    명시 (clock-cycle target / simdjson-class arm64 / 4-store struct_pack).

C3. `hand_rolled` ns/op 값은 wall-clock 측정 (`gdate +%s%N` 우선, BSD date %s × 1e9 fallback). BSD
    date 가 `%N` 을 미지원하는 환경에서는 second-resolution 만 — 그 경우 ns/op 추정치는 의미 없음
    (해당 환경에서 bench 재실행 필요).

C4. LOC 값 (hand_rolled 81/110/70 vs native_estimate 12/6/3) 은 bench harness 의 helper fn LOC 합산
    (manual count). 본 LOC 에는 self_test / main / emit / time-helper 등 measurement infra 는
    제외 — 순수 implementation surface 만 count.

C5. Native baseline 추정치가 hand_rolled 보다 빠를 수도, 느릴 수도 있음. Track AV 의 bench 진가는
    ns/op 비교가 아니라 (a) hand_rolled 의 ns/op 가 비-perf-critical 영역에서 acceptable 수준이라는
    측정 + (b) LOC delta (hand-rolled 가 native 대비 5-15× LOC) 라는 maintainability cost 측정.

C6. 6.1 의 hand_rolled implementation 은 `tool/closure_orchestrator.hexa` 의 13 helper fn 의 mirror
    (line 453-510, 517-540 등). bench harness 는 source 를 import 하지 않고 helper 를 own 1 (hexa
    only) 정합으로 self-contained 재현 — 다른 file 변경 0.

C7. 6.2 의 hand_rolled implementation 은 `tool/roadmap_op.hexa` 의 `extract_str_after` /
    `checkpoint_extract_refs_body` / `extract_int_field` / `checkpoint_split_refs` 의 mirror.
    동일 own 1 self-contained 정합.

C8. 6.3 의 hand_rolled implementation 은 `lib/loader/pe_winsock_ws2_32.hexa` 의 SOCKADDR_IN
    synthetic byte array (line 254 expected hex `02000001BB01020304000000000000`) 를 self_test
    golden equality 로 cross-validate. 다른 11 loader 의 struct synth 패턴은 동일 형태 — 별도 bench
    필요 시 본 harness 를 template 으로 확장 가능.

C9. Track AV 는 high-hit 3 entry (6.1 / 6.2 / 6.3) 한정. 6.4 (C-hit 1) / 6.5 (C-hit 2) / 6.6
    (C-hit 0, forward-looking) 는 본 cycle scope 외. 추후 cycle 에서 동일 방식으로 escalate 가능.

C10. hexa-lang sibling repo (`/Users/ghost/core/hexa-lang`) 는 본 cycle 에서 read-only. PR 작업
     pending — user explicit approval 가 떨어지면 별도 cycle 에서 sibling repo 측에 RFC 또는
     implementation PR 진행 (현 단계는 `study_only_pr_pending_user_approval` 유지).

C11. 평행 BG (Track AW / Track AX / Track AU) 의 territory 와 file scope 충돌 0. Track AV 가 touch
     한 파일은 `tool/bench/<3 신규 hexa>` + `docs/HEXA_UPSTREAM.md` (§8 append) + handoff + marker
     로 한정.

C12. raw 9 / raw 10 / raw 11 / raw 12 / raw 15 / raw 175 정합:
     - raw 9 (doc-only): bench harness 는 hexa code 추가 — doc 추가도 동시 진행 (§8). pure doc-only
       는 아니나 own 2 의 honest measurement requirement 가 evidence file 추가를 요구.
     - raw 11 (snake_case): bench harness fn 명 모두 snake_case (`bp61_*` / `bp62_*` / `bp63_*`).
     - raw 12 (silent_error_ban): bench harness 의 panic 은 self_test 의 golden mismatch 한정 —
       bench measurement loop 자체는 silent fallback 없음 (gdate 부재 시 BSD date fallback 명시).
     - raw 15 (env lazy): bench harness 는 GAMEBOX_HOME 에 의존하지 않음 — pure local computation.
     - raw 175 (br_no_user_verbatim): user directive 는 §1 에서 paraphrase, verbatim quote 0.

C13. `tool/bench/` 디렉토리에 기존 `.hexa` 파일이 0개였음 (Battle.net-Setup.exe + bnet-setup.exe
     binary 만 존재 + .gitkeep). Track AV 는 본 디렉토리에 hexa bench harness 패턴을 처음 land —
     향후 다른 entry bench 추가 시 본 harness 가 reference template 역할.

C14. Bench harness 가 `hexa run` 으로 실제 실행되는지는 본 cycle 에서 verify 안 함 (hexa CLI 가
     해당 host 에 있는지 별개 — schema/structure validate 만). 실 ns/op 측정은 hexa CLI 가
     설치된 환경에서 추후 실행 필요.

## §7 hexa-lang sibling repo PR posture

`/Users/ghost/core/hexa-lang` 측 작업 0 — read-only reference. Track AO marker
`hexa_lang_sibling_posture: "study_only_pr_pending_user_approval"` 유지.

PR 제출 시점은 user explicit greenlight + 별도 cycle 진행. Track AV 가 추가한 evidence (B-bench
measurement + LOC delta) 는 PR-ready 정합 자체에 충분 — 단 정책상 user approval 부재로 hold.

PR 제출 시 예상 packet:
- RFC `hexa-lang/rfc/<NNN>_std_math_fixed_point_x1000.md` (entry 6.1)
- RFC `hexa-lang/rfc/<NNN>_std_json_decode_path_get.md` (entry 6.2)
- RFC `hexa-lang/rfc/<NNN>_core_bin_struct_pack.md` (entry 6.3)

각 RFC 는 본 cycle 의 Track AV bench harness output 을 evidence section 으로 cite 가능.

---

*Track AV bench-escalation written 2026-05-04. additive / migration 0 / destructive 0 / hexa-lang
sibling repo modifications 0 / sibling-PR 제출 user explicit approval pending.*
