---
schema: airgenome-gamebox/docs/hexa_cli_runtime_build_decision_landed/ai-native/1
last_updated: 2026-05-04
ssot:
  marker: state/markers/airgenome_gamebox_hexa_cli_runtime_build_decision_landed.marker
  decision_doc:
    - docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md
  predecessor_handoffs:
    - docs/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
    - docs/airgenome_gamebox_roadmap_op_adoption_decision_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_roadmap_op_self_impl_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_roadmap_op_full_11_of_11_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed_2026_05_03.ai.md
status: LANDED_HEXA_CLI_RUNTIME_BUILD_DECISION_DOC (implementation_status: pending_user_lock_in)
related_raws:
  - raw 9    # hexa-only orchestration (audit-only, no impl emitted — decision doc only)
  - raw 10   # honest C3 caveats inline (≥ 6 items)
  - raw 11   # snake_case
  - raw 12   # silent-error ban (every tradeoff explicit, no swallowed scoring)
  - raw 15   # env() lazy + repo-relative paths (no /Users/<user>/... absolute in created files; only audit data refs sibling absolute paths)
  - raw 168  # minimum-viable doc-only land (raw 9 audit-only exempt)
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes in this doc)
preserved_unchanged:
  - all 14 .roadmap.<domain> files (11 per-game + 3 loader-domain — read-only reference)
  - all docs/*.md (existing — read-only style reference, no modify)
  - all state/markers/*.marker (existing — read-only cross-link reference)
  - all hexa modules under lib/, native/, tool/, tests/
  - plugin.json, entry, README.md, .gitignore
  - 23 predecessor markers (Track A-V trail citing hexa CLI absent caveat)
  - build/hexa_interp wrapper (untouched — A option implementation cycle land)
  - native/ directory (5 files — untouched)
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
  own1: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 — admin tool (interpreter binary 측 admin / build / dev tool lens) 도입 결정 측 own1 의 "외부 게임 runtime 의존 0" 정신 측 lens 측 매트릭스 reflect; admin tool 측 own1 측 strict 게임-실행 layer 측 분리 적용 측 사용자 lock-in 시 명시 ack 정합 회복
  own2: log/error/emit/headless honest documentation — 매트릭스 점수 explicit, 권고 측 confidence level 명시, 사용자 lock-in 대기 명시, swallowed tradeoff 0; Option C 측 own2 weak alignment 측 honest disclosure (runtime emit 측 schema-only 영구 — log/error verify 측 영구 absent)
---

# airgenome-gamebox `hexa CLI runtime` build path 도입 결정 매트릭스 land (Track Y, decision-only)

## TL;DR

predecessor 23 markers (Track A 부터 Track V 까지) 측 누적 caveat (`hexa CLI absent — schema-validated only` posture trail) 측 **decision matrix 형식 측 정리 land**. 본 cycle 측 **구현 0**, 결정 doc + 권고 + 사용자 lock-in checklist 만 land.

- 신규 `docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` (239 LOC) — 7 section, 11 criteria scoring matrix, 권고 + confidence level + fallback scenario + lock-in checklist + cross-link to 23 predecessor markers + 11 caveats.
- 권고: **Option A (anima clone)**, confidence MEDIUM, 매트릭스 합계 27 (formal highest 단 critical criteria runtime_smoke_capability + own2_alignment 측 1 정합).
- C bypass = 23 (formal lowest) 단 runtime_smoke_capability 5 + own2_alignment 5 = production smoke 영구 차단 측 disqualifying.
- B self-impl = 25 단 dev_cost 5 + test_burden 5 = ≥ 3 month horizon 측 본 repo cap (90min) 측 prohibitive.
- fallback: A 측 license incompatible 시 C bypass + alternate path / A 측 cross-OS portability fail 시 macOS only path.
- lock-in pending: 사용자 측 A/B/C 선택 + (A 선택 시) hexa-lang commit-sha freeze + license audit + interp install path policy + own1 admin tool lens disambiguation explicit ack + cross-OS scope.
- **구현 0** — `tool/hexa_cli_runtime/*` 또는 `build/hexa_interp` 측 본 cycle 측 file emit X. 후속 implementation cycle 측 사용자 lock-in 후 별도 cycle 측 진행.
- 새 doc 1 + 새 handoff doc 1 + 새 marker 1 = **3 file 추가**, 기존 file 0건 modify, destructive 0.

## §1 user_directive_paraphrase

사용자 directive (paraphrase only, no verbatim) — predecessor Track A-V 23 markers 측 누적 caveat (`hexa CLI absent — schema-validated only` posture trail) 측 정리 작업. 본 cycle 측 hexa interpreter 측 본 repo 측 가용 측 활성화 측 path 측 **결정 X — 매트릭스 분석 + 권고 + 사용자 lock-in 대기 만**. 3-way option (A: anima clone via `~/.hx/bin/hexa` + `~/core/hexa-lang/build/hexa_interp.real` chain / B: self-impl interpreter scratch build / C: bypass = schema-only posture 영구 lock-in) 측 11 criteria scoring + 권고 confidence MEDIUM + fallback + 사용자 lock-in checklist. own1 (Wine 0 / 외부 게임 runtime 0 정신 측 admin tool 측 분리 적용) + own2 (honest emit, 매 tradeoff explicit) 정합. Track H pattern (`docs/ROADMAP_OP_ADOPTION_DECISION.md`) 측 직접 mirror — admin tool source code (Track H) vs runtime binary (Track Y) 측 scope 측 차이 측 명시. 후속 implementation cycle 측 사용자 측 A/B/C 선택 후 별도 cycle 측 land.

## §2 decision doc summary

### §2.1 sections (`docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md`)

| § | title | LOC 추정 |
|---|---|---|
| §1 | Background — 왜 hexa CLI runtime 이 필요한가 (캐비어트 trail 정리 + sibling audit) | ~40 |
| §2 | 3-way option analysis (A anima clone / B self-impl / C bypass) | ~55 |
| §3 | 결정 매트릭스 (11 criteria scoring + 단순 합계 측 misleading 정리) | ~30 |
| §4 | 권고 — Option A anima clone, confidence MEDIUM | ~30 |
| §5 | 사용자 lock-in 체크리스트 (≥ 13 항목) | ~20 |
| §6 | cross-link to 23 predecessor markers | ~30 |
| §7 | caveats ≥ 11 honest items | ~35 |
| **합계** | — | **239** |

### §2.2 매트릭스 11 criteria

`docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` § 3 측 measurement criteria (각 row 측 low=1 / med=3 / high=5; 낮을수록 better):

1. dev_cost
2. external_dependency_count
3. drift_risk
4. test_burden
5. standalone_clone_safety
6. hexa_native_alignment
7. ssot_purity
8. version_coupling
9. runtime_smoke_capability
10. own1_alignment (Wine 0 / 외부 runtime 0 정신 측 admin tool lens)
11. own2_alignment (honest emit + runtime verify capability)

### §2.3 점수 합계 (낮을수록 better, 단 단순 합계 측 misleading)

- **A (anima clone)**: **27** (formal highest 단 dev_cost 1 + runtime_smoke_capability 1 + own2_alignment 1 + hexa_native_alignment 1 = production posture 즉시 unlock 가능)
- **B (self-impl)**: **25** (formal middle 단 dev_cost 5 + test_burden 5 = ≥ 3 month horizon prohibitive)
- **C (bypass)**: **23** (formal lowest 단 runtime_smoke_capability 5 + own2_alignment 5 = production smoke 영구 차단 disqualifying)

→ 단순 합계 측 misleading. disqualifying criterion (runtime_smoke_capability + own2_alignment 측 5 = production posture 영구 차단) 측 weight 측 사용자 측 own1 strict-vs-pragmatic stance 측 결정.

## §3 recommended option + rationale

### §3.1 권고: Option A (anima clone), confidence MEDIUM

근거 (decision doc § 4.1):

1. **production posture 즉시 unlock** — 23 cycle 누적 caveat 측 1-2 cycle 측 closed-out 가능. runtime emit 측 schema-only → actual execution 측 transition.
2. **dev cost 합리적** — wrapper version pin + selftest path 측 1-2 cycle. Option B (≥ 3 month) 측 비교 측 100x 효율.
3. **sibling already-stable** — hexa-lang interpreter 측 long-term 운영 검증 (Apr 2025 부터 hxc_aot/hxc_bench cycle 누적 evidence + 본 repo Track A-V indirect dependency stability).
4. **own1 admin tool lens disambiguation 가능** — Track H + Track 11 `battlenet_bypass_own1_disambiguation_landed.marker` precedent 측 동일 — admin tool 측 own1 측 분리 적용 측 사용자 lock-in 시 명시 ack 정합.

### §3.2 confidence MEDIUM (not MEDIUM-HIGH or HIGH) 이유

- **own1 admin tool lens 확장 측 사용자 lock-in 측 explicit ack 의존** — strict reading 측 사용자 측 admin tool 측 분리 적용 측 명시 ack 측 부재 시 own1 violation risk.
- **hexa-lang version pin 측 marker frozen pin 필요** — Track H A 옵션 동일 caveat — version pin 측 reproducibility 측 사용자 책임.
- **license audit pending** — hexa-lang license 측 본 repo plugin 측 호환 여부 측 사용자 audit 결과 lock-in 의존.
- **standalone clone safety caveat** — 본 repo plugin install 시 hexa-lang sibling prerequisite 측 명시 필요. cross-OS scope 측 사용자 lock-in 의존.

### §3.3 fallback 시나리오

- **A 측 hexa-lang license incompatible 시** → **Option C bypass** 영구 fallback. alternate verification path (e.g., python3 측 strict hexa AST subset evaluator) 측 별도 cycle 측 land 측 사용자 sequencing lock-in.
- **A 측 cross-OS portability fail 시** → **Option C bypass** cross-OS partial fall back. macOS A path sustain.
- **B self-impl** 측 ≥ 3 month horizon 측 strategic 정당화 X — 사용자 측 명시적 lock-in 시 별도 prolonged cycle (6+ cycle) 측 land 가능 단 본 doc 권고 X.

## §4 사용자 lock-in 체크리스트 (pending)

> **상태**: ⏳ awaiting user lock-in

`docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` § 5 측 정합:

- [ ] **option choice**: A (anima clone) / B (self-impl) / C (bypass) 측 1 개 선택
- [ ] **(if A)**: hexa-lang 측 commit-sha frozen reference + license audit 결과 + interp binary install path policy + own1 admin tool lens 확장 explicit ack + cross-OS portability scope
- [ ] **(if B)**: implementation cycle 추정 + minimum-viable scope (interpreter for `self_test()` + `main()` dispatch only) + hexa lang spec dialect alignment + native binary build chain (Rust/Go/C++/self-host)
- [ ] **(if C)**: alternate verification path (python3 hexa AST subset evaluator vs system Python wrapper vs 영구 schema-only) + production posture 영구 차단 explicit ack
- [ ] **raw 168 minimum-viable acknowledge**: 본 cycle 측 decision doc only — `hexa CLI runtime` 측 본 cycle 측 emit 0건 ack
- [ ] **schema migrate 의무 X 명시 ack**: 본 repo `migration: forbidden` 정합 — interp wrapper 측 in-place modify 0 vs additive only

## §5 caveats (≥ 6 honest items)

- **C1 (matrix scoring 주관성)**: § 3 11 criteria 점수 측 own1 / own2 lens 측 주관 반영 — 다른 weight 적용 시 winner 다를 수 있음. 단순 합계 측 misleading (C 23 best 단 disqualifying criteria 5 = production 영구 차단).
- **C2 (hexa-lang version commit-sha frozen pin 부재)**: 본 cycle 측 sibling `~/core/hexa-lang/build/hexa_interp.real` presence 확인 (Mach-O arm64, 1580720 bytes, 2026-05-02 mtime) — commit-sha pin 0. A option land 시 사용자 측 commit-sha 명시 marker frozen pin 필요.
- **C3 (B self-impl ≥ 3 month horizon underdetermined)**: § 2.2 추정 측 본 repo hexa lang spec coverage actual measurement X 추정. minimum-viable subset (interpreter for self_test()+main() dispatch only) cycle estimate 측 사용자 conservative/aggressive 판단 lock-in. native build chain 측 별도 lock-in 항목.
- **C4 (cross-OS portability macOS-arm64 가정)**: sibling audit 측 macOS arm64 확인 — Linux variants 측 docker container path 의존, Windows 측 binary 부재 (raw 15 portability 위반 가능). plugin distribution cross-OS scope 사용자 lock-in 필요.
- **C5 (own1 admin tool lens 확장 측 사용자 lock-in 의존)**: own1 측 strict 게임-실행 layer reading vs admin/build/dev tool lens 측 분리 적용 측 사용자 명시 ack 측 정합 회복. Track H + Track 11 `battlenet_bypass_own1_disambiguation_landed.marker` precedent 동일 disambiguation pattern.
- **C6 (alternate verification path Option C deferred decision)**: Option C 측 영구 schema-only posture 측 production runtime smoke alternate path (python3 hexa AST subset evaluator) 측 별도 cycle 결정 측 deferred. 본 cycle 측 honest disclosure — Option C 측 본질적 결정 deferred 측 사용자 sequencing lock-in 필요.
- **C7 (own2 weak alignment for Option C)**: Option C 측 runtime emit 측 schema-only — log/error/runtime self_test actual execution verify 0 측 own2 weak alignment. (대조 측 Option A + B own2 strict 정합 가능.)
- **C8 (test 측 본 cycle emit X)**: decision doc only — interpreter selftest / wrapper round-trip test / sibling binary smoke test 0건. raw 9 (audit-only no impl emit) + raw 168 (minimum-viable exempt) 정합 — implementation cycle 측 selftest 필수 land.
- **C9 (Track G `hexa_interp` build pending caveat C1 inherit risk for Option A)**: Track G `track_g_field_norm_d2r_cond3_landed.marker` caveat C1 측 anima sibling internal history inherit — A clone path 측 land 시 anima sibling caveat 측 본 repo cross-repo audit burden 측 별도 marker 명시 필요.
- **C10 (rate-limit recovery disk write priority)**: 본 cycle 측 BG subagent rate-limit recovery 시점 측 disk write priority — report deferred 가능. doc + handoff + marker disk land 측 cycle success criteria. (Track H precedent 동일.)
- **C11 (Track H scope 측 차이 측 명시)**: 본 doc 측 Track H decision pattern direct mirror — Track H scope = admin tool source code (`tool/roadmap_op.hexa`) 측 본 repo file vs Track Y scope = runtime binary (interpreter) 측 본 repo link/clone. Track H 권고 = B self-impl confidence MEDIUM-HIGH (admin tool source 측 본 repo SSOT 측 합리적) vs Track Y 권고 = A anima clone confidence MEDIUM (runtime binary 측 sibling SSOT 측 합리적, scratch build prohibitive). 두 권고 측 different rationale 측 own2 정합 (각 scope 측 honest measurement).

## §6 next-cycle hooks (post-decision implementation cycle estimate)

본 cycle 후속 — 사용자 lock-in 후 implementation cycle 측 다음 sequencing 권고:

### §6.1 if user picks **A (anima clone)** — 권고 path

- **cycle 1 (wrapper version pin + selftest path)**: 본 repo `build/hexa_interp` wrapper 측 hexa-lang commit-sha pin + `~/.hx/bin/hexa` 측 invoke path 측 selftest harness 측 land. 추정 1 cycle.
- **cycle 2 (sibling smoke + 23 marker caveat closure)**: lib/loader/ + lib/perf/ + tests/ 측 sample hexa file 측 actual self_test() invocation smoke + 23 marker caveat trail 측 closed marker 측 land. 추정 1 cycle.
- **cycle 3 (cross-OS scope expansion if locked)**: Linux container path 측 hexa_interp_linux_arm64 측 wrapper integration + Windows N/A acknowledge marker. 추정 1 cycle.
- **cycle 4 (license audit closure if pending)**: hexa-lang license 측 본 repo plugin 측 compatibility audit 결과 측 marker 측 land. 추정 0.5 cycle.

### §6.2 if user picks **B (self-impl)** — fallback path (≥ 3 month horizon, 비권고)

- **cycle 1-N (lexer + parser + AST)**: 본 repo native/ Rust/Go/C++ 측 lexer + parser + AST 구현. minimum-viable subset (self_test()+main() dispatch). 추정 ≥ 4 cycle.
- **cycle N+1 (runtime + stdlib coverage)**: tree-walking interpreter + stdlib (json / file io / sha256). 추정 ≥ 6 cycle.
- **cycle N+M (cross-validation vs sibling)**: 본 repo interpreter 측 동작 vs sibling hexa-lang interpreter 측 동작 측 same-result audit. 추정 ≥ 2 cycle.
- → 합계 ≥ 12 cycle (3 month horizon at 4 cycle/week pace, 본 repo cap 90min/cycle 제약).

### §6.3 if user picks **C (bypass)** — 비권고 path (영구 schema-only)

- **cycle 1 (alternate verification path 결정)**: python3 측 strict hexa AST subset evaluator OR system Python wrapper vs 영구 schema-only 측 결정 doc + marker. 추정 1 cycle.
- **cycle 2 (alternate path land if locked)**: python3 evaluator 측 minimum-viable subset 측 land + tests/ 측 alternate verification harness 측 land. 추정 ≥ 2 cycle.
- → 후속 cycle 측 production posture 측 영구 차단 측 사용자 측 cycle-by-cycle 책임 + plugin distribution 측 runtime emit 영구 unverified 측 명시 acknowledge marker 측 누적.

### §6.4 23 marker caveat trail closure 측 cross-link

- 본 cycle 측 decision doc + handoff + marker 측 land 측 23 marker 측 누적 caveat 측 **decision matrix 측 explicit 정리 단계 도달**.
- 후속 implementation cycle (사용자 lock-in 후) 측 marker 측 본 cycle marker 측 cross-link 측 trail 측 chained.
- 23 marker 측 actual closed 측 사용자 측 A/B/C lock-in + implementation cycle 측 land 후 별도 closure marker 측 land.

---

(handoff doc end. § 1-6, decision doc summary + 권고 confidence MEDIUM + lock-in checklist + 11 caveats + next-cycle 4-path estimate. Track H pattern direct mirror — admin tool decision (Track H) + runtime binary decision (Track Y) 측 paired closure.)
