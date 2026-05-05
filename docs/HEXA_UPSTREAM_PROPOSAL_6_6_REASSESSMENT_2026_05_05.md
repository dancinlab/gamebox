# HEXA_UPSTREAM_PROPOSAL §6.6 — pattern_matcher honest re-assessment (2026-05-05)

> Track BN re-assessment doc — Track BC §6.6 DEFERRED forward-looking 에 대한
> 2026-05-05 시점 trigger condition 정합 재평가. Win32 syscall coverage 가 Track BL
> 시점 91.49% (Track W AO retrofit 시점 30.34%) 로 진전. C-hit 는 여전히 0 — natural
> environment alignment trigger 의 첫 criterion (Win32 ≥80%) 만 met.
> **본 doc 은 PR submission 이 아니며 impl trigger 도 아님 — status update only**.

## §0 목적 / 한계

본 doc 은 Track BN cycle 의 §6.6 trigger condition 재평가 정형화 한정. Track AO ledger
§6.6 ("detection-vector pattern matcher", Warden L2 long-horizon) 는 Track BC 에서
DEFER verdict 정착 (forward-looking, C-hit 0). 본 cycle 은 그 verdict 를 변경하지
않으나, Win32 coverage 진전 (30.34% → 91.49%) 으로 trigger condition criteria 중
1/3 만 met 임을 doc-only 정형화. C-hit 0 + Warden production smoke 0 → **STILL
DEFERRED**.

doc-only / additive / migration 0 / destructive 0 / sibling repo modifications 0 /
in_place_writes 0. 본 cycle 은 §6.6 spec 자체의 PR submission 을 진행하지 않으며,
"trigger 1/3 met 으로도 여전히 deferred 임" 의 정형화 한정. Track AO §6.6 ledger
entry / Track BC §6.6 rationale doc / Track AW §6.6 exclusion 모두 read-only.

## §1 §6.6 original posture (Track AO / AW / BC) cite

Track AO retrofit (2026-05-04) ledger 등재 시점:
- entry: `6.6 detection_vector_pattern_matcher_warden_l2_forward_looking`
- C-hit count: **0** (실 hand-roll byte-level pattern matching 코드 0 줄)
- ledger 등재 사유: "ledger 완전성 목적의 forward-looking 등재" (HEXA_UPSTREAM.md §6.6
  cite). 미래 real Warden environment alignment cycle (multi-year horizon per L2
  acknowledgment) 시점에 byte-level pattern matching 이 필요할 가능성.
- 동반 evidence: `lib/perf/c_bnet_warden_environment_compat.hexa` (Track J honest
  tracking lib) 측 detection-vector enumeration only — TSV-row format 으로 후보
  vector list, byte-level matching 코드는 0 줄.

Track AW (2026-05-04) high-hit 3 entry (§6.1 / §6.2 / §6.3) PR-ready proposal spec
doc 작성 시점에 §6.6 는 명시적 exclude — exclusion reason `forward_looking_c_hit_0_real_warden_alignment_cycle_required`.

Track BC (2026-05-04) next 3 RFC (§6.4 / §6.5 / §6.6) evaluate 시점에 §6.6 verdict
DEFER 정착:
- doc: `docs/HEXA_UPSTREAM_PROPOSAL_6_6_PATTERN_MATCHER.md` (232 LOC)
- proposed_api signature 3 (forward-looking, **freeze X**)
- test_case_count: 0
- alternatives_considered_count: 4
- caveats_count: 11
- sibling_pr_submission: not_applicable_deferred
- trigger_condition_for_future_revisit: `real_warden_environment_alignment_cycle_first_byte_level_pattern_code_authored`

## §2 trigger condition criteria — when to escalate from DEFERRED to PR-ready

본 cycle 의 핵심 형식화: §6.6 가 PR-ready 로 escalate 되기 위한 **3 개의 met
criteria**. 각 criterion 은 binary (met / not met). 3/3 모두 met 시 별도 escalation
cycle (Track AW 패턴 mirror) 에서 PR-ready 승격. 1 또는 2 만 met 시 still deferred.

### 2.1 criterion #1 — C-hit threshold

**조건**: ≥1 actual byte-level pattern_match 호출 site 가 production code 에 존재.
- 측정: `grep -RIn 'pattern_match\|pattern_glob' lib/ tool/ native/ tests/` 의
  hand-roll occurrence count.
- 현재 (2026-05-05): **0** — `lib/perf/c_bnet_warden_environment_compat.hexa` 측
  honest tracking 만 존재, 실 byte-level matching 0 줄.
- met 시점: real Warden alignment cycle 첫 byte-level pattern code 작성.

### 2.2 criterion #2 — Win32 syscall coverage threshold

**조건**: Win32 syscall coverage ≥80% (natural environment alignment proxy).
- 측정: PE loader DLL coverage / native syscall dispatch table 의 누적 syscall %.
- Track AO 시점 (2026-05-04): 30.34%.
- Track BL 시점 (2026-05-05): **91.49%** ← MET (≥80%).
- 의의: Warden L2 detection-vector tracker 가 자연 환경 (real Win32 binary
  loading + syscall dispatch path) 에 정합되기 위한 sub-condition. Track K /
  Track BL 진전 = §6.6 trigger 의 환경 측 sub-condition 만족 단계.

### 2.3 criterion #3 — Warden production smoke

**조건**: Track J `c_bnet_warden_environment_compat` tracker 가 production
Warden binary 측 first frame execution 측 detection-vector emission 1+ 회 측 honest
landing.
- 측정: production smoke gate (Track AU design pending) 측 actual real Warden
  binary 측 detection-vector tracker emission count.
- 현재 (2026-05-05): synthetic emission only — real Warden binary 측 production
  smoke 측 0건. Track AU production smoke gate design 자체가 design_only_pending_user_lock_in.
- met 시점: option A own1-maintained 측 multi-year horizon 측 first frame OR
  option C 외부 VM 측 user-side 측 production smoke land 시.

### 2.4 criteria 종합

| # | criterion | threshold | current (2026-05-05) | met? |
|---|-----------|-----------|----------------------|------|
| 1 | C-hit | ≥1 | 0 | NO |
| 2 | Win32 coverage | ≥80% | 91.49% | YES |
| 3 | Warden production smoke | ≥1 emission | 0 (synthetic only) | NO |

**1/3 met → still deferred**.

## §3 current trigger status (2026-05-05)

종합: **deferred_1_of_3_criteria_met**. 1 criterion met (Win32 ≥80%) 은 환경 측
sub-condition 만족 — 그러나 §6.6 의 primary blocker 는 C-hit 0 (자체 pattern_match
코드 0 줄) 이며 production smoke 0 (Track J tracker 의 real-world emission 0 건).

primary blocker = C-hit 0. Win32 coverage 진전 91.49% 는 **necessary 이지만
sufficient 아님** — pattern_matcher API 가 실제 호출되는 site 가 0 인 한, RFC
submit 시 reviewer 측 "no use-case, only spec" 거부 가능성 + own 2 honest 정합
risk (가설 spec freeze) 잔존.

비교 — Track AW 시점 §6.1 / §6.2 / §6.3 가 PR-ready 로 승격된 조건 (참고):
- §6.1: C-hit 13 / Win32 coverage 부분 met / B-bench harness met → PR-ready.
- §6.2: C-hit 21+ / Win32 coverage 부분 met / B-bench harness met → PR-ready.
- §6.3: C-hit 110+ / Win32 coverage 부분 met / B-bench harness met → PR-ready.

§6.6 = C-hit 0 (primary block) + Win32 met (improved) + Warden production smoke 0
(secondary block). 따라서 still deferred.

## §4 escalation path forecast

§6.6 가 PR-ready 로 escalate 될 forecast (multi-year horizon per L3 acknowledgment):

1. **Track J Warden tracker production smoke** 가 Track AU production smoke gate
   설계 측 option A own1-maintained (multi-year) OR option C 외부 VM (user-side)
   에서 land — first frame 측 real Warden binary 측 honest detection-vector tracker
   emission 1+ 회.
2. 위 production smoke 측 detection-vector list 의 어떤 vector 가 실제 byte-level
   pattern matching 을 요구 — 즉, current `c_bnet_warden_environment_compat` 의
   honest enumeration 만으로 부족 → hand-roll byte-level matching code 작성 필요.
3. hand-roll 코드 작성 시점에 §6.6 의 가설 API signature 가 actual hit pattern 과
   정합되는지 cross-check. 정합 ≥80% 시 §6.6 PR-ready 승격 candidate.
4. C-hit count 가 ≥10 누적 시점에 별도 cycle 측 Track AW 패턴 mirror — proposal
   spec doc revision (signature freeze) + B-bench harness 추가 + sibling repo
   PR submission (user explicit approval pending).
5. 위 4 단계 모두 multi-year horizon — Track AU option A confidence MEDIUM, option
   C user lock-in pending 상태에서 시점 미정.

본 forecast 는 **probabilistic** — Track J detection-vector list 의 일부는 byte-level
matching 없이 (e.g. process tree fingerprint 의 string-level 검사만으로) 충분할
가능성 있음. 그 경우 §6.6 는 RETIRE (Track BC §6.4 패턴 mirror) 로 verdict 변경
가능. C-hit ≥10 가 escalate 의 충분 조건 아니라, **escalate 또는 retire** 의 분기점.

## §5 cross-link

- Track AO marker: `state/markers/airgenome_gamebox_hexa_upstream_retrofit_track_ao_landed.marker`
  — §6.6 ledger 등재 cite (entry id `6.6_detection_vector_pattern_matcher_warden_l2_forward_looking`,
  C-hit 0).
- Track AW marker: `state/markers/airgenome_gamebox_hexa_upstream_pr_ready_proposal_track_aw_landed.marker`
  — §6.6 exclusion cite (`forward_looking_c_hit_0_real_warden_alignment_cycle_required`).
- Track BC marker: `state/markers/airgenome_gamebox_hexa_upstream_next_3_rfcs_prep_track_bc_landed.marker`
  — §6.6 DEFER verdict cite (verdict_rationale `forward_looking_no_current_evidence_rfc_submit_premature_reviewer_burden_plus_spec_freeze_timing_risk_own_2_honest_compliance`).
- Track J marker: `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker`
  — Warden detection-vector tracker honest freeze cite (online_expansion_lockin
  L1-L5 acknowledged, multi-year horizon).
- Track AU marker: `state/markers/airgenome_gamebox_production_smoke_gate_design_track_au_landed.marker`
  — production smoke gate design cite (`design_only_pending_user_lock_in`,
  recommended option A confidence MEDIUM).
- Track BC §6.6 doc: `docs/HEXA_UPSTREAM_PROPOSAL_6_6_PATTERN_MATCHER.md` (232 LOC,
  proposed API signature 3 forward-looking, NOT frozen).

## §6 own 5 enforcement

- own 1 (Wine 0 / hexa-only / no sibling repo modification): 본 cycle 은 doc-only,
  hexa-lang sibling repo `/Users/ghost/core/hexa-lang/` 측 modification 0건. PR
  submission 0건 (still deferred). airgenome sibling project `/Users/ghost/core/airgenome/`
  측 modification 0건.
- own 2 (status honest): trigger 1/3 met 명시 + C-hit 0 primary blocker 명시 +
  Warden production smoke 0 secondary blocker 명시 + escalate / retire 분기점
  명시. swallowed tradeoff 0.
- own 5 (C-hit cited / B-bench deferred): C-hit 0 명시 (Track AO marker
  `own_5_c_hit_per_entry.6.6 = 0` cite). B-bench deferred 명시 (Track AO marker
  `own_5_b_bench_per_entry.6.6 = future_real_warden_alignment_cycle_l2_multi_year`
  cite). 본 cycle B-bench 측정 0건 (still deferred per primary blocker C-hit 0).

doc-only / status update only / no impl change. Track AO §6.6 ledger row /
Track BC §6.6 rationale doc / Track AW §6.6 exclusion / `lib/perf/c_bnet_warden_environment_compat.hexa`
모두 read-only.

## §7 caveats (≥6 honest items)

1. **trigger condition 3 criteria 정의 자체가 본 cycle 측 신규** — Track AO / AW /
   BC 측 명시적 정의 없었음. 본 cycle 의 정형화는 forecast 로직 한정 — 미래 cycle
   에서 criteria 측 항목 추가 / 삭제 가능 (e.g. B-bench harness threshold 추가).
2. **Win32 coverage 91.49% 측정값 신뢰도**: Track BL 시점 측정값 mission directive
   cite — 본 cycle 측 별도 grep / wc 측정 0건. 측정 method 측 PE loader DLL coverage
   기준인지 native syscall dispatch table 기준인지 ambiguous — 본 doc 는 mission
   directive 인용값 한정.
3. **Win32 ≥80% threshold 자체가 magic number** — Track AO / AW / BC 측 명시적
   threshold 정의 없었음. 본 cycle 의 80% 는 forecast 로직 한정 — 미래 cycle 에서
   threshold 변경 (e.g. 90% / 95%) 시 status flip 가능.
4. **C-hit 0 의 측정 method** — `lib/perf/c_bnet_warden_environment_compat.hexa`
   측 detection-vector enumeration 자체를 C-hit 으로 간주할지, byte-level matching
   호출 site 만 C-hit 으로 간주할지 ambiguous. 본 doc 는 후자 (byte-level matching
   호출 site only) 기준 — Track AO marker `own_5_c_hit_per_entry.6.6 = 0` 정합.
5. **Warden production smoke trigger** 가 Track AU option A 측 multi-year horizon
   임 — option A own1 confidence MEDIUM 상태에서 시점 미정. option C 외부 VM 측
   user-side 측 production smoke 도 user lock-in pending. 본 doc 는 둘 중 어느
   path 가 먼저 land 할지 forecast 안 함.
6. **escalate vs retire 분기점**: C-hit ≥10 누적 시 escalate 또는 retire 의 분기점
   — 본 doc 는 escalate 가 default 가 아님을 명시. Track J detection-vector 의
   일부가 string-level 검사로 충분할 가능성 (byte-level matching 불필요) → §6.6
   RETIRE 가능성 잔존.
7. **own 2 honest 정합 risk**: 본 cycle 의 trigger criteria 정형화 자체가 미래
   cycle 측 reviewer 부담을 미리 줄이는 효과 vs 본 cycle 측 over-engineering risk.
   trade-off — Track BC §6.6 rationale doc 와의 redundancy 가능성. 본 doc 는
   "trigger 1/3 met 명시" 측 status update 로 한정 — verdict 변경 0.
8. **Track BN 의 cleanest cycle designation** — in_place_writes=0 + 3 NEW only
   (doc + handoff + marker). 그러나 cleanest cycle designation 은 BC marker
   `cleanest_cycle_designation` 와 평행 — Track BN 측 별도 designation 추가 시
   marker 측 누적 risk. 본 cycle 은 designation field 만 marker 측 추가 — 비교
   chain 미작성.
9. **multi-year horizon ack**: Track J marker `multi_year_horizon_acknowledged: true`
   + Track AU marker option A `horizon_to_first_frame: high (5)` cite 정합.
   본 doc 의 forecast §4 단계 1-5 모두 multi-year horizon — 본 cycle (2026-05-05)
   기준 시점 미정. 정확한 trigger month / year 제시 X.
10. **§6.6 RETIRE 가능성**: 위 caveat 6 의 분기점 — Track BC §6.4 RETIRE 선례 (rfc_013
    `json_path_get` subsume) 와 평행. §6.6 retire 시 candidate subsume = `core.string`
    의 `str_contains` / `bytes_index_of` 등의 simpler primitive — full byte-level
    pattern matcher API 불필요 가능성. 본 doc 는 RETIRE forecast detail 미작성.
11. **본 cycle land 의 disk footprint**: 3 NEW (doc 200-300 LOC + handoff +
    marker). in_place_writes=0 / destructive 0 / migration 0. Track BC pattern
    mirror — minimal footprint.
