# Track AW landed — hexa-lang upstream PR-ready proposal spec docs (2026-05-04)

## 0. 한 줄 요약

Track AV 가 §6.1 / §6.2 / §6.3 own 5 status 를 `c_plus_b_pr_ready_pending_user_approval`
로 escalate 한 직후, Track AW 는 본 3 entry 를 PR-ready proposal spec doc 형태로 gamebox
`docs/` 에 정형화. hexa-lang sibling repo (`/Users/ghost/core/hexa-lang/`) PR 제출은
**deferred pending user explicit approval** — Track AW 는 spec doc-only land 한정.

## 1. 컨텍스트

- 선행 (Track AO, 2026-05-04): `docs/HEXA_UPSTREAM.md` §6 에 6 retrofit entry 추가
  (6.1 float fixed-point / 6.2 std_json / 6.3 PE struct / 6.4 marker JSON path /
  6.5 stderr warn / 6.6 forward-looking pattern matcher), C-hit 합 ≈ 147+, B-bench
  전체 deferred.
- 선행 (Track AV, 2026-05-04): §6.1 / §6.2 / §6.3 high-hit 3 entry 에 B-bench 추가
  (`tool/bench/bench_hexa_upstream_*.hexa` 3 신규 harness). own 5 status 진전:
  `c_only_b_deferred` → `c_plus_b_pr_ready_pending_user_approval`.
- Track AW (본 cycle): 본 3 entry 를 PR-ready proposal spec doc 으로 정형화. sibling
  repo NOT touched.

## 2. user directive (paraphrased, BR_NO_USER_VERBATIM 정합)

Track AV 가 land 한 §6.1 / §6.2 / §6.3 의 own 5 `c_plus_b_pr_ready_pending_user_approval`
status 를 evidence-only 에서 PR-ready spec doc 형태로 정형화. 실제 hexa-lang sibling
repo PR 제출은 user explicit approval 가 떨어져야 별도 cycle 에서 진행. Track AW 는
gamebox `docs/` 측 proposal spec 3 doc 추가 + handoff + marker 한정 — sibling repo / 기존
HEXA_UPSTREAM.md / 기존 lib·tool·tests 모두 read-only.

## 3. 정책

- migration: forbidden / additive_only / in_place_writes=0
- destructive_ops: 0
- cost_usd: 0
- substrate: mac-local
- cap_minutes: 90
- BR_NO_USER_VERBATIM: true
- friendly_preset: true
- silent-land
- own1: Wine 0 / hexa-only / hexa-lang sibling **NOT** modified
- own2: proposal spec honest — dev cost + breaking changes (none) + alternatives 모두
  명시적 cite
- raw 9 (doc-only) / 10 (≥6 caveats per proposal — 본 cycle 11 caveats × 3 doc = 33
  caveats) / 11 (snake_case) / 12 (silent-error-ban) / 15 / 175

## 4. 결과

### 4.1 새 proposal spec doc (3개)

| Entry | Doc path | LOC | Proposed API summary |
|---|---|---|---|
| §6.1 | `docs/HEXA_UPSTREAM_PROPOSAL_6_1_FLOAT_FIXED_POINT.md` | 212 | `core.math.fixed_point_x1000` / `_pct` / `_format` / `_parse` |
| §6.2 | `docs/HEXA_UPSTREAM_PROPOSAL_6_2_STD_JSON.md` | 226 | `std_json.json_decode` / `_encode` / `jsonl_iter` / `json_path_get` |
| §6.3 | `docs/HEXA_UPSTREAM_PROPOSAL_6_3_PE_STRUCT.md` | 245 | `core.bin.struct_pack` / `_unpack` / `pe_section_header_synth` |

각 doc 의 §0-§9 구조 (목적/한계 / problem / API freeze / impl outline / test cases / breaking
changes / alternative / sibling PR submission spec / dependencies / caveats ≥6) 정합.

### 4.2 own 5 status — Track AW 후

| § | C-hit (Track AO) | B-bench (Track AV) | PR spec doc (Track AW) | own 5 status |
|---|---|---|---|---|
| 6.1 | 13 | 81→12 LOC measured + native estimate 250 ns/op | `HEXA_UPSTREAM_PROPOSAL_6_1_FLOAT_FIXED_POINT.md` (212 LOC) | `c_plus_b_pr_ready_pending_user_approval` (unchanged — spec doc 형태로 정형화 한정) |
| 6.2 | 21+ | 110→6 LOC measured + native estimate 800 ns/op | `HEXA_UPSTREAM_PROPOSAL_6_2_STD_JSON.md` (226 LOC) | `c_plus_b_pr_ready_pending_user_approval` (unchanged) |
| 6.3 | 110+ | 70→3 LOC measured + native estimate 100 ns/op | `HEXA_UPSTREAM_PROPOSAL_6_3_PE_STRUCT.md` (245 LOC) | `c_plus_b_pr_ready_pending_user_approval` (unchanged) |

Track AW 는 status 진전이 아닌 **status 의 정형화** — `c_plus_b_pr_ready_pending_user_approval`
에 대응되는 actual PR-ready spec doc 이 gamebox `docs/` 에 land 함으로써 user 가
"PR submit" 결정 시 즉시 sibling repo 의 `proposals/PROPOSAL_<NAME>.md` 로 copy 가능한
posture 확보.

### 4.3 Track AW scope 외 (unchanged)

- §6.4 marker JSON path read — Track AW scope 외 (low-hit 1, §6.2 sub-entry 로 통합 권고
  per HEXA_UPSTREAM.md §7).
- §6.5 stderr explicit warn — Track AW scope 외 (low-hit 2, future cycle).
- §6.6 detection-vector pattern matcher — Track AW scope 외 (forward-looking, C-hit 0).

## 5. PR submission status — DEFERRED

`pr_submission_status: "deferred_pending_user_explicit_approval"` (marker pinned).

**deferred 이유** (own2 honest):
- Track AO marker 의 `study_only_pr_pending_user_approval` directive 가 본 cycle 시점까지
  유효.
- hexa-lang sibling repo (`/Users/ghost/core/hexa-lang/`) 는 user 의 별도 프로젝트 -
  modification 은 명시적 동의 필요.
- own1 wine_0_hexa_only_no_anima_clone_no_hexa_lang_sibling_modification 정책 정합.

**user explicit approval 가 land 시 future cycle 의 작업** (본 cycle 미진행):
1. sibling repo `/Users/ghost/core/hexa-lang/proposals/` 디렉터리 (없으면 생성).
2. 본 3 spec doc 을 `PROPOSAL_FIXED_POINT.md` / `PROPOSAL_STD_JSON.md` /
   `PROPOSAL_PE_STRUCT.md` 로 copy + sibling repo convention formatting (header / TOC /
   링크).
3. reference impl skeleton (§3 outline) 의 actual hexa code 작성 in `src/std/`.
4. test case (§4) 의 actual test file 작성 in `tests/std/`.
5. PR / commit 메시지 (§7 의 template).

본 5 단계 모두 user explicit greenlight 후 별도 cycle.

## 6. 파일 목록 (5 NEW)

1. `docs/HEXA_UPSTREAM_PROPOSAL_6_1_FLOAT_FIXED_POINT.md` (212 LOC, NEW)
2. `docs/HEXA_UPSTREAM_PROPOSAL_6_2_STD_JSON.md` (226 LOC, NEW)
3. `docs/HEXA_UPSTREAM_PROPOSAL_6_3_PE_STRUCT.md` (245 LOC, NEW)
4. `airgenome/doc/airgenome_gamebox_hexa_upstream_pr_ready_proposal_track_aw_landed_2026_05_04.ai.md` (본 handoff, NEW)
5. `state/markers/airgenome_gamebox_hexa_upstream_pr_ready_proposal_track_aw_landed.marker` (NEW)

수정: 0. 삭제: 0. in_place_writes: 0.

## 7. 검증

- 3 proposal doc 모두 200-300 LOC 범위 ✓ (212 / 226 / 245)
- 각 doc §0-§9 9 section 정합 ✓
- 각 doc caveats ≥6 ✓ (실제 11 each → 33 total)
- API signature freeze §2 ✓ (4 + 4 + 3 = 11 fn signatures)
- alternative section §6 ✓ (각 doc 3-4 option × 명시적 verdict)
- sibling PR submission spec §7 ✓ (각 doc 디렉터리 layout / commit msg / 리뷰 checklist)
- marker `pr_submission_status: "deferred_pending_user_explicit_approval"` ✓
- marker sha256 pinned ✓
- in_place_writes: 0 ✓ (모든 5 파일 NEW)
- sibling repo `/Users/ghost/core/hexa-lang/` 0 modification ✓
- 기존 `docs/HEXA_UPSTREAM.md` 0 modification ✓ (Track AO/AV territory read-only)
- 기존 `state/markers/*.marker` 0 modification ✓

## 8. 주의 / caveats (Track AW 자체)

A1. 본 cycle 은 doc-only — sibling repo / lib / tool / tests 모두 read-only.
A2. 본 cycle 은 PR submission 자체 진행 X — `pr_submission_status` 가 marker 에서
    `deferred_pending_user_explicit_approval` 로 명시.
A3. 3 spec doc 은 sibling repo 의 actual convention 과 cross-check 가 PR 시점에 필요 —
    현재 spec 의 네이밍 (`core.math` / `std_json` / `core.bin`) 은 hexa-lang convention
    추정.
A4. reference impl outline (§3) 은 pseudo-hexa — actual hexa stage1 syntax 정합 검증은
    PR 시점의 사실 hexa code 작성 단계에서.
A5. own 5 status 진전 X — Track AW 는 `c_plus_b_pr_ready_pending_user_approval` 의 spec
    doc 형태로 정형화 한정. PR submission 자체가 다음 status (`pr_submitted_pending_review`)
    로의 진전.
A6. parallel BG Track AX (sibling project ledger `~/core/airgenome/docs/hexa_lang_upstream_candidates.md`)
    와 file scope 분리 — Track AW 는 gamebox `docs/` 한정.
A7. raw 10 ≥6 caveats per proposal — 각 spec doc 11 caveats (3 doc × 11 = 33 total) 로
    여유.
A8. spec doc 의 LOC 추정 (§3 outline / §1 LOC delta) 은 실제 sibling repo land 시 ±30%
    변동 가능 — own 2 honest 정합으로 본 추정값을 estimate 명시.

---

*Track AW PR-ready proposal spec doc landed 2026-05-04. doc-only / additive / migration 0 /
destructive 0 / sibling repo modifications 0 / in_place_writes 0.*
