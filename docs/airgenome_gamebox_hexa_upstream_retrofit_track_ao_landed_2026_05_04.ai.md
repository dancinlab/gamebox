# Track AO — hexa-lang upstream ledger retrofit landed (2026-05-04)

## §1 user_directive_paraphrase

User directive (paraphrased, no verbatim quote): BG cycle 에서 hexa-lang upstream 개선 후보가 발견되면
같은 cycle 에 함께 ledger 갱신을 진행한다. Track AO 는 본 directive 가 정해지기 이전 cycle 들 (Track
AG / AB / AE / J / K / O / Q / W / AH) 에서 발견되었으나 ledger 가 갱신되지 않았던 항목을 RETROFIT
형식으로 doc-only 추가하는 cycle 이다. own 5 enforcement 정합: evidence = C (code site hit) + B
(bench measurement) 중 Track AO 는 C 만 cite, B 는 entry 별 future cycle 로 deferred.

## §2 retrofit summary

- 추가된 entry: 6
- 추가 위치: `docs/HEXA_UPSTREAM.md` §6.1 ~ §6.6 (기존 §1-§5 M0 R0 entry verbatim 유지)
- 인용된 cycle: Track AG, Track AB, Track AE, Track K, Track O, Track Q, Track W, Track AH, Track J (총 9)

| § | Entry | Discovery cycle | Proposed API |
|---|---|---|---|
| 6.1 | float runtime / x1000 fixed-point | AG | `core.math.fixed_point_x1000` / `int_pct_format` / `int_pct_parse` |
| 6.2 | std_json / JSONL parser | AB + AE | `json_decode` / `json_encode` / `jsonl_iter` / `json_path_get` |
| 6.3 | PE struct serializer | K + O + Q + W + AH | `core.bin.struct_pack` / `struct_unpack` / `pe_section_header_synth` |
| 6.4 | marker JSON path read | AG | (6.2 sub-entry — `json_path_get`) |
| 6.5 | stderr explicit warn | AE + AB | `core.io.eprintln` / `core.io.stderr_write` |
| 6.6 | pattern matcher (forward) | J | `std_pattern.pattern_match_byte_seq` / `pattern_glob` |

기존 §1-§5 (M0 R0 entry 1.1 ~ 1.10, 4.5.1 ~ 4.5.3, 5.docker) 는 한 글자도 수정하지 않음. 추가만.

## §3 own 5 enforcement table

| § | C-hit count | C-evidence file | B-bench status | Escalation policy |
|---|---|---|---|---|
| 6.1 | 13 | `tool/closure_orchestrator.hexa` line 453-510, 517-540, 593-620, 753-790 | deferred | future cycle 에서 perf-critical 측정 시 escalate |
| 6.2 | 21+ | `tool/roadmap_op.hexa` line 131, 1605, 1739, 1765-1788 | future | parser land 시 ~200 LOC delta 측정 후 escalate |
| 6.3 | 110+ | `lib/loader/pe_*.hexa` × 11 file | future | byte-array layout mismatch 발생률 cycle 에서 측정 |
| 6.4 | 1 | `tool/closure_orchestrator.hexa` line 593 | deferred | 6.2 std_json sub-entry — 6.2 escalate 와 함께 |
| 6.5 | 2 | `tool/roadmap_op.hexa` line 1788 + `tool/roadmap_op.hexa` 1936 (stderr_warn 호출 site) | future | correctness (warn / err 채널 분리) 측면 측정 |
| 6.6 | 0 | `lib/perf/c_bnet_warden_environment_compat.hexa` (forward-looking) | 미정 | real Warden alignment cycle 시작 시 escalate (L2 multi-year horizon) |
| **합계** | **147+** | — | 전체 deferred | per-entry future cycle 별 escalate |

**Track AO own 5 status**: C-hit cited = 100%; B-bench cited = 0% (전체 deferred per directive
"Track AO 는 C-hit only retrofit, B-bench 는 entry 별 future cycle"). 위반 0.

## §4 forward-looking entries

§6.6 (Warden detection-vector pattern matcher) 는 현 시점 wraround 0 (Track J 가 honest tracking only,
실제 pattern matching code 0 줄) 이지만 ledger 완전성 + multi-year L2 horizon 명시 목적으로 등재.
실제 PR 작업 / B-bench 측정은 real Warden environment alignment cycle 시작 시점 (multi-year horizon)
에 escalate.

기타 entry (6.1~6.5) 는 모두 현재 wraround 가 active 한 코드 base 에 존재 — Track AO 는 등재만.

## §5 future-cycle hooks (per-entry escalation 시점)

| § | Escalation trigger | Target action |
|---|---|---|
| 6.1 | closure_orchestrator 가 perf-critical loop 에 진입할 때 (현재 doc-only) | bench 측정 + hexa-lang `core.math.fixed_point_x1000` PR 후보화 |
| 6.2 | JSONL parser bypass 가 다른 tool 로 확산되어 200+ LOC 도달 또는 escape 버그 발생 시 | `std_json` PR 후보화 (decode + path-get 포함) |
| 6.3 | Win32 loader 신규 추가 cycle 에서 layout-mismatch incident 발생 또는 loader 갯수 ≥ 15 도달 시 | `core.bin.struct_pack` PR 후보화 |
| 6.4 | 6.2 와 동기화 — 6.2 escalate 시 sub-entry 로 같이 진행 | 6.2 와 함께 |
| 6.5 | warn / err 채널 분리 incident (예: stderr 가 stdout 와 mix 되어 parser 가 깨지는 상황) 발생 시 | `core.io.eprintln` PR 후보화 |
| 6.6 | real Warden alignment cycle 시작 (multi-year L2 horizon — user explicit greenlight 필요) | `std_pattern` 설계 RFC 작성 |

위 trigger 는 모두 future cycle 에서 user explicit approval 후 진행. Track AO 자체는 hook 등재만.

## §6 caveats (≥10 honest items)

C1. Track AO 는 doc-only retrofit cycle. tool / lib / native / tests / .roadmap / .checkpoint 어느
    file 도 수정 안함. 기존 §1-§5 entry 도 한 글자 수정 안함.

C2. C-hit count 는 grep / line-number 기반 counting. 동일 file 내 같은 fn 이 여러 cycle 에서 수정
    되었어도 1 hit 으로 count (line site 기준, 변경 횟수 무관). 따라서 "147+" 은 lower bound.

C3. B-bench status 는 전체 deferred. 측정 cycle 미정. own 5 의 B 는 본 retrofit cycle 에서 cite
    하지 않으며, 본 결정 자체가 own 5 의 explicit deferral 정책에 따른 것 (C 만으로는 PR-ready 가
    아니나, retrofit ledger 등재는 가능).

C4. §6.3 의 "11 loader × 평균 10-15 site / loader = 110-165" 는 평균 산출. 각 loader 의 정확한
    site 수는 별도 cycle 에서 enumeration 필요. 본 retrofit 은 loader 갯수 + 평균 site 만 cite.

C5. §6.6 (Warden pattern matcher) 는 현재 hit 0 — forward-looking entry. own 5 의 C-hit 정책상
    "0 hit" entry 는 ledger 진입 자격이 borderline 이지만, multi-year L2 horizon + ledger 완전성
    목적으로 예외 등재. 실제 PR 작업은 user explicit approval 시점 까지 deferred.

C6. hexa-lang sibling repo (`/Users/ghost/core/hexa-lang`) 는 본 cycle 에서 read-only reference 로
    만 사용. clone / symlink / 수정 0. PR 작업은 user explicit approval 후 별도 cycle.

C7. sibling project ledger (`~/core/airgenome/docs/hexa_lang_upstream_candidates.md`) 도 수정 안함.
    cross-link cite 는 가능하나 본 cycle 에서는 cite 0 (gamebox 측 ledger 자체 완성도가 우선).

C8. Track AO 가 cite 한 9 cycle (AG / AB / AE / J / K / O / Q / W / AH) 의 marker / handoff doc 은
    본 cycle 에서 read-only reference. 수정 0.

C9. 직접 escalate (PR 화) 는 0. own 5 + user directive 모두 "Track AO 는 ledger 등재 한정" 으로 명시
    — PR 화는 entry 별 future cycle + user explicit approval.

C10. parallel BG 영역 (Track AK / AL / AM / AN) file 은 수정 0. file scope 위반 0.

C11. 기존 HEXA_UPSTREAM.md 의 §1.10 (float arithmetic stage1 broken) 과 본 cycle §6.1 (x1000
    fixed-point retrofit) 은 동일 root cause (float runtime 부재) 의 두 측면 — §1.10 은 M0 시점
    discovery, §6.1 은 closure_orchestrator (Track AG) 시점 discovery. 중복 cite 가 아니라 cycle
    별 hit site 누적 목적으로 별도 §.

C12. §6.2 가 §1.2 (encode-only 제안) 를 decode + path-get 으로 확장하는 형태 — 기존 §1.2 entry 는
    그대로 보존되며, §6.2 에서 "encode 만 제안되어 있던 1.2 를 확장" 임을 명시.

C13. cycle_cited count = 9 이지만 Track O / Q / W / AH 는 §6.3 (PE struct serializer) 단일 entry
    에서 합쳐 cite 됨. entry-level cycle reference 와 cycle-level discovery 갯수는 다름.

C14. 본 retrofit 은 git commit 안함. silent-land. user 가 별도 cycle 에서 commit 결정.

## §7 hexa-lang sibling repo PR posture

- 위치: `/Users/ghost/core/hexa-lang` (sibling repo, separate git tree)
- 현 cycle posture: **study-only reference** — read 가능하나 modify / clone / symlink / fork 모두 0.
- PR 작업: **deferred — user explicit approval pending**.
- 본 retrofit ledger 의 §6.1 ~ §6.6 entry 는 "PR 후보 등재" 까지만; 실제 RFC / PR 화는
  - per-entry escalation trigger 발동 (§5)
  - user explicit greenlight
  - 측정 + bench (own 5 의 B-evidence) 충족
  의 3 조건이 모두 갖춰진 future cycle 에서만.
- Track AO 는 본 sibling repo 를 1 byte 도 수정하지 않았으며, 차후 작업의 ground rule 이
  "study-only → PR 은 user 명시적 greenlight 후" 임을 ledger 에 pin.

---

*written 2026-05-04. Track AO RETROFIT cycle. doc-only / additive / migration 0 / destructive 0 /
silent-land / own1 (Wine 0, hexa-only, no anima clone, no hexa-lang sibling PR) / own2 (own 5
enforcement explicit, C-hit cited, B-bench deferred per-entry).*
