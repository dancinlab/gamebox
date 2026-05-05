---
schema: airgenome-gamebox/docs/roadmap_op_adoption_decision_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_roadmap_op_adoption_decision_landed.marker
  decision_doc:
    - docs/ROADMAP_OP_ADOPTION_DECISION.md
  predecessor_handoffs:
    - airgenome/doc/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
    - airgenome/doc/airgenome_gamebox_per_game_roadmaps_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_d4_retired_intentional_adoption_landed_2026_05_03.ai.md
status: LANDED_ROADMAP_OP_ADOPTION_DECISION_DOC (implementation_status: pending_user_lock_in)
related_raws:
  - raw 9    # hexa-only orchestration (audit-only, no impl emitted — decision doc only)
  - raw 10   # honest C3 caveats inline
  - raw 11   # snake_case
  - raw 12   # silent-error ban (every tradeoff explicit, no swallowed scoring)
  - raw 15   # env() lazy + repo-relative paths (no /Users/<user>/... absolute)
  - raw 168  # minimum-viable doc-only land (raw 9 audit-only exempt)
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes in this doc)
preserved_unchanged:
  - all 14 .roadmap.<domain> files (11 per-game + 3 loader-domain — read-only reference)
  - all docs/*.md (existing — read-only style reference, no modify)
  - all state/markers/*.marker (existing — read-only cross-link reference)
  - all hexa modules under lib/, native/, tool/, tests/
  - plugin.json, entry, README.md, .gitignore
  - 3 predecessor markers (self_mk2 / per_game_roadmaps / d4_retired_intentional)
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 0
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
  implementation_status: pending_user_lock_in
own:
  own1: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 — admin tool 도입 결정 측 own1 의 "외부 repo / runtime 의존 0" 정신 측 lens 측 매트릭스 reflect
  own2: log/error/emit/headless honest documentation — 매트릭스 점수 explicit, 권고 측 confidence level 명시, 사용자 lock-in 대기 명시
---

# airgenome-gamebox `tool/roadmap_op.hexa` 도입 결정 매트릭스 land (Track H, decision-only)

## TL;DR

predecessor 6 markers 측 누적 caveat ("tool/roadmap_op.hexa adoption decision (anima clone / self-impl / symlink — accumulating caveat)") 측 **decision matrix 형식 측 정리 land**. 본 cycle 측 **구현 0**, 결정 doc + 권고 + 사용자 lock-in checklist 만 land.

- 신규 `docs/ROADMAP_OP_ADOPTION_DECISION.md` (214 LOC) — 7 section, 11 criteria scoring matrix, 권고 + confidence level + fallback scenario + lock-in checklist + cross-link to 6 predecessor markers + 9 caveats.
- 권고: **Option B (self-impl)**, confidence MEDIUM-HIGH, 매트릭스 합계 19 (best vs A=31 / C=35).
- fallback: B 측 dev cost prohibitive 시 A clone, A 측 dialect compat fail 시 B minimum-viable subset.
- lock-in pending: 사용자 측 A/B/C 선택 + 후속 항목 (anima version reference / impl cycle estimate / symlink target policy 등 9 checklist 항목).
- **구현 0** — `tool/roadmap_op.hexa` 측 본 cycle 측 emit X. 후속 implementation cycle 측 사용자 lock-in 후 별도 cycle 측 진행.
- 새 doc 1 + 새 handoff doc 1 + 새 marker 1 = **3 file 추가**, 기존 file 0건 modify, destructive 0.

## §1 user_directive_paraphrase

사용자 directive (paraphrase only, no verbatim) — predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` next_cycle_actions field 측 명시된 누적 caveat 측 정리 작업. 본 cycle 측 `tool/roadmap_op.hexa` 측 **구현 X** — 3-way option (A: anima clone / B: self-impl / C: symlink) 측 매트릭스 분석 + 권고 + 사용자 lock-in 대기 만. own1 (Wine 0 / 외부 의존 0 정신 측 admin tool 측 적용) + own2 (honest emit, 매 tradeoff explicit) 정합 측 매트릭스 점수 측 swallowed 항목 0. 후속 implementation cycle 측 사용자 측 A/B/C 선택 후 별도 cycle 측 land.

## §2 decision doc summary

### §2.1 sections (`docs/ROADMAP_OP_ADOPTION_DECISION.md`)

| § | title | LOC 추정 |
|---|---|---|
| §1 | Background — 왜 `tool/roadmap_op.hexa` 가 필요한가 | ~30 |
| §2 | 3-way option analysis (A / B / C) | ~50 |
| §3 | 결정 매트릭스 (11 criteria scoring) | ~25 |
| §4 | 권고 — Option B self-impl, confidence MEDIUM-HIGH | ~25 |
| §5 | 사용자 lock-in 체크리스트 | ~15 |
| §6 | cross-link to 6 predecessor markers | ~25 |
| §7 | caveats ≥ 9 honest items | ~30 |
| **합계** | — | **214** |

### §2.2 매트릭스 11 criteria

`docs/ROADMAP_OP_ADOPTION_DECISION.md` § 3 측 measurement criteria (각 row 측 low=1 / med=3 / high=5; 낮을수록 better):

1. dev_cost
2. external_dependency_count
3. drift_risk
4. test_burden
5. standalone_clone_safety
6. hexa_native_alignment
7. SSOT_purity
8. version_coupling
9. plugin_distribution_safety
10. own1_alignment (Wine 0 / 외부 runtime 0 정신 측 lens)
11. own2_alignment (honest emit + log/error 명시)

### §2.3 점수 합계 (낮을수록 better)

- **A (anima clone)**: **31** (avg 2.8 — drift / version_coupling / external_dependency 측 high)
- **B (self-impl)**: **19** (avg 1.7 — winner, dev_cost / test_burden 만 med-high, 그 외 모두 low)
- **C (symlink)**: **35** (avg 3.2 — standalone_clone_safety / external_dependency / SSOT_purity / version_coupling / plugin_distribution_safety 측 모두 high)

## §3 recommended option + rationale

### §3.1 권고: Option B (self-impl), confidence MEDIUM-HIGH

근거 (decision doc § 4.1):

1. **own1 정합 우선** — Wine 0 / 외부 runtime 0 정신 측 외부 repo 의존 0 측 확장 적용. external_dependency_count 1 (low) + plugin_distribution_safety 1 (low) + standalone_clone_safety 1 (low).
2. **own2 정합** — SSOT_purity 1 (best) + own2_alignment 1 (best) + hexa-native dialect 측 본 repo 측 control.
3. **future-proof schema** — 본 repo first-mover schema field (`status_enum_adopted` / `retired_intentional_posture` / `adoption_evidence` / `adopted_iso` / `blocker_class`) 측 validator 측 본 repo 측 자유롭게 추가 가능.
4. **매트릭스 합계 19 (best)** — 11 criteria 측 honest scoring 측 명료한 winner.

### §3.2 confidence MEDIUM-HIGH (not HIGH) 이유

- dev_cost 측 3 (med) — 사용자 측 implementation cycle budget 측 prohibitive 시 A clone 측 fallback 가능.
- test_burden 측 5 (high) — selftest 측 별도 cycle burden 측 사용자 lock-in 측 explicit ack 필요.
- 11 subcmd full impl 측 ≥ 2 cycle 추정 — minimum-viable 5 subcmd subset 측 1 cycle 가능.

### §3.3 fallback 시나리오

- **B 측 dev cost prohibitive 시** → **A anima clone** 측 fallback (anima 측 commit-sha 측 marker 측 frozen pin 필요).
- **A 측 anima dialect compat fail 시** → **B 측 minimum-viable subset (5 subcmd: add/update/list/show/verify)** 측 land 후 cycle-by-cycle 측 11 subcmd 측 확장.
- **C symlink 측 권고 X** — standalone_clone_safety / plugin_distribution_safety 측 high risk disqualifying. 사용자 측 명시적 lock-in 시에만 land.

## §4 사용자 lock-in 체크리스트 (pending)

> **상태**: ⏳ awaiting user lock-in

`docs/ROADMAP_OP_ADOPTION_DECISION.md` § 5 측 정합:

- [ ] **option choice**: A (anima clone) / B (self-impl) / C (symlink) 측 1 개 선택
- [ ] **(if A)**: anima 측 commit-sha frozen reference + license audit
- [ ] **(if B)**: implementation cycle scope (minimum-viable 5 subcmd 1 cycle / 11 subcmd full ≥ 2 cycle)
- [ ] **(if B)**: schema field validator 추가 우선순위
- [ ] **(if C)**: symlink target 절대/상대경로 정책 + standalone clone safety 위반 ack
- [ ] **selftest scope**: S1..Sn (subcmd 정합) land 시점
- [ ] **schema migrate 의무 X 명시 ack**: 본 repo `migration: forbidden` 정합 — tool 측 in-place modify 0 vs additive only

## §5 caveats (≥ 6 honest items)

- **C1 (matrix scoring 주관성)**: § 3 11 criteria 점수 측 own1 / own2 lens 측 주관 측 반영 — 사용자 측 다른 weight 적용 시 winner 측 다를 수 있음. 매트릭스 측 honest starting point 측 제시이지 절대 측정 X.
- **C2 (anima version frozen pin 부재)**: 본 cycle 측 `../anima/tool/roadmap_op.hexa` 측 sibling presence 확인 (809 LOC, 31859 bytes, 2026-05-02 mtime) — 단 commit-sha pin 0. A option land 시 사용자 측 commit-sha 명시 marker 측 frozen pin 필요.
- **C3 (B self-impl dev cost 추정 underdetermined)**: 11 subcmd full 측 ≥ 2 cycle / minimum-viable 5 subcmd 측 1 cycle 측 추정 — 본 repo hexa stdlib actual coverage 측정 X 측 추정. 사용자 측 conservative / aggressive 판단 lock-in 필요.
- **C4 (symlink portability macOS 가정)**: symlink 측 macOS 정상 동작 가정 — Linux 동작 가정 — Windows git symlink 측 OS-별 차이 측 raw 15 portability 위반 가능. plugin cross-OS scope 측 사용자 lock-in 필요.
- **C5 (own1 admin tool 정의 first-class 명시 X)**: tool 자체 측 게임-실행 X 측 own1 (Wine 0) 직접 위반 X — own1 의 "외부 repo / runtime 의존 0" 정신 측 B self-impl 측 정합. 본 doc 측 own1 의미 측 admin tool 측 확장 측 사용자 lock-in 시 명시 ack 필요.
- **C6 (mk2 status enum spec freeze 의존)**: tool enum validator 측 add 측 요구 (per `4game_retired_intentional_adoption_landed.marker` next_cycle_actions) — 단 enum spec freeze 측 본 repo 측 별도 cycle land 미완. implementation cycle 측 enum spec freeze 측 동시 / 선행 land 측 사용자 측 sequencing 결정 필요.
- **C7 (raw 15 env_lazy 위반 anima default)**: anima 측 `REPO_ROOT_DEFAULT = "/Users/<user>/core/anima"` hard-coded — A clone 측 land 시 본 repo path 측 patch 필요 (raw 15 정합 회복). C symlink 측 land 시 patch control X 측 raw 15 영구 위반 risk.
- **C8 (test 측 본 cycle 측 emit X)**: 본 cycle 측 decision doc only — tool selftest / round-trip test / schema validator test 0건. 본 cycle 측 raw 9 (audit-only no impl emit) + raw 168 (minimum-viable exempt) 정합 — implementation cycle 측 selftest 측 필수 land.
- **C9 (rate-limit recovery disk write priority)**: 본 cycle 측 BG subagent 측 rate-limit recovery 시점 측 disk write priority — report deferred 가능. 본 doc + handoff doc + marker 측 disk land 측 cycle success criteria.
- **C10 (no `tool/roadmap_op.hexa` emit in this cycle)**: 본 cycle scope 측 명시 — `tool/roadmap_op.hexa` 측 본 cycle 측 file 측 emit 0건. 사용자 측 본 점 측 acknowledge — tool 측 후속 implementation cycle 측 사용자 lock-in 후 land.

## §6 next-cycle hooks (post-decision implementation cycle estimate)

본 cycle 후속 — 사용자 lock-in 후 implementation cycle 측 다음 sequencing 권고:

### §6.1 if user picks **B (self-impl)** — 권고 path

- **cycle 1 (minimum-viable)**: 5 subcmd skeleton (add / update / list / show / verify) + JSONL parse / write helper + atomic write (tmp+mv per raw 15 SSOT) + selftest S1..S5. 추정 1 cycle.
- **cycle 2 (full subset)**: 6 subcmd 추가 (remove / link / domains / render / track-new / selftest extension) + S6..S11 selftest. 추정 1 cycle.
- **cycle 3 (schema validator)**: enum validator (`status_enum_adopted` / `retired_intentional_posture` / `adoption_evidence` / `adopted_iso` / `blocker_class`) + cond status state machine validator (proposed → partial → met) + cross-link validator. 추정 1 cycle.
- **cycle 4 (mk2 status enum spec freeze sync)**: per `4game_retired_intentional_adoption_landed.marker` next_cycle_actions — enum 정식 schema 등록 cycle 측 본 tool 측 validator 측 sync land. 추정 1 cycle.

### §6.2 if user picks **A (anima clone)** — fallback path

- **cycle 1 (clone + patch)**: anima 측 specific commit-sha 측 file frozen-copy + `REPO_ROOT_DEFAULT` patch (raw 15 정합) + path 재설정 + selftest S1..S11 재실행. 추정 1 cycle.
- **cycle 2 (본 repo schema field add)**: 본 repo first-mover field 측 validator 추가 (anima 측 not-yet-have 항목). 추정 1 cycle.

### §6.3 if user picks **C (symlink)** — 비권고 path

- **cycle 1 (symlink + caveat doc)**: `ln -s ../anima/tool/roadmap_op.hexa tool/roadmap_op.hexa` + standalone clone safety 위반 acknowledge marker + plugin distribution 측 caveat doc. 추정 0.5 cycle.
- 후속 cycle 측 anima 측 어느 commit 측 frozen pin 측 marker 측 reproducibility 측 사용자 측 cycle-by-cycle 책임.

### §6.4 enum validator + mk2 status enum spec freeze 측 cross-link

- predecessor `4game_retired_intentional_adoption_landed.marker` next_cycle_actions: "`tool/roadmap_op.hexa` 측 enum validator 추가 결정"
- 본 cycle 측 위 caveat 측 결정 doc 측 정리 — 후속 implementation cycle 측 enum validator 측 actual land.
- mk2 status enum spec freeze 측 별도 cycle (currently 별도 caveat) 측 본 tool 측 land 측 동시 / 선행 land 측 사용자 측 sequencing lock-in 필요.

---

(handoff doc end. § 1-6, decision doc summary + 권고 confidence MEDIUM-HIGH + lock-in checklist + 10 caveats + next-cycle 4-path estimate.)
