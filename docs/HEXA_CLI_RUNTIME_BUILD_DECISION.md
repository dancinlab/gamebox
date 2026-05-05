# `hexa CLI runtime` (interpreter binary) build path 도입 결정 매트릭스 — 3-way tradeoff (A anima clone / B self-impl / C bypass)

> **status**: 결정 doc (decision-only) / 작성 **2026-05-04** / 본 cycle 은 **구현 X**, 매트릭스 + 권고 + 사용자 lock-in 대기
> **scope**: predecessor cycle (Track A 부터 Track V 까지) 측 누적된 `hexa CLI absent — schema-validated only` 캐비어트 측 해소 측 hexa interpreter binary 측 본 repo 측 가용 측 활성화 측 구조적 결정
> **결론 (요약, confidence MEDIUM)**: **Option A (anima clone, sibling `~/.hx/bin/hexa` + `core/hexa-lang/build/hexa_interp.real` chain 측 본 repo 측 간접 link 측 활성화)** 권고 — own1 (Wine 0 / 외부 게임 runtime 0) 측 hexa-lang interpreter 측 admin tool 측 lens 측 정합 + dev cost 측 합리적. **Option B (self-impl)** 측 prohibitive cost (interpreter scratch build = months horizon). **Option C (bypass = schema-validated only 영구 posture)** 측 production smoke 측 영구 차단 측 최후 fallback.
> **lock-in pending**: 사용자 측 A/B/C 선택 + (A 선택 시) hexa-lang version / commit-sha freeze + license audit / interp binary install path policy + (B 선택 시) interpreter scope / 추정 / minimum-viable subset / (C 선택 시) alternate verification path (e.g., python wrapper subset) lock-in

---

## §1 Background — 왜 hexa CLI runtime 이 필요한가

### §1.1 누적된 cycle posture: `hexa CLI absent — schema-validated only`

본 repo (airgenome-gamebox) 는 Track A 부터 Track V 까지 **23 cycle 동안 일관되게** 다음 caveat 를 marker 측 명시해왔다:

- `raw_168_minimum_viable_additive: predecessor pattern 동일 — schema validate (python3 json.loads + sha256) only, hexa CLI absent 환경 honest reflect`
- `hexa_cli_runtime_emit: schema_validated_only (hexa CLI assumed absent — predecessor cycle posture)`
- `fallback_strategy: live_self_test_stdout OR grep_emit_source_string (skeleton-validated path)`

→ 이 caveat trail 측 **본 cycle 측 정리 단계 도달**. hexa CLI runtime 측 본 repo 측 가용 시 다음 항목 측 즉시 unlock 가능:
1. lib/loader/ + lib/perf/ + tool/ + tests/ 측 hexa file 측 actual self_test() invocation (현재 schema-only)
2. tool/roadmap_op.hexa (Track H 권고 self-impl 측 land 시) 측 actual subcmd dispatch smoke (현재 schema-only)
3. tests/test_*.hexa 측 actual round-trip + assertion validation (현재 grep emit string only)
4. native/ + lib/loader/ 측 cross-language interop smoke (현재 sha256 schema-only)

### §1.2 현재 hexa CLI 환경 audit (sibling 측 actual presence)

본 cycle 측 sibling 측 audit 결과 — **hexa-lang interpreter 측 actual binary 측 file system 측 존재**:

- `~/.hx/bin/hexa` — Bourne-Again shell script (29841 bytes, 2026-05-03 mtime) — **docker-redirecting wrapper**, 직접 interpreter 아님. Mac safe-landing layer (raw 44 docker hard-landing).
- `~/.hx/bin/hexa_real` — Mach-O 64-bit arm64 executable (1580720 bytes, 2026-05-02 mtime) — **actual interpreter binary**.
- `~/core/hexa-lang/build/hexa_interp` — Bourne-Again shell wrapper (fork-storm cap shim).
- `~/core/hexa-lang/build/hexa_interp.real` — Mach-O 64-bit arm64 — **upstream actual interpreter** (hexa-lang repo SSOT).
- `~/core/hexa-lang/build/hexa_interp_linux_arm64` / `hexa_interp_linux_x86_64` — Linux variants (container path).
- 본 repo `build/hexa_interp` — Bourne-Again shell wrapper only (29841 bytes, fork-storm cap shim — **interpreter binary 측 본 repo 측 file 0건**).

→ **결론**: hexa interpreter binary 측 sibling 측 already-built (hexa-lang repo SSOT). 본 repo 측 missing 항목 = **간접 link / clone / wrapper integration policy**, NOT interpreter implementation 자체.

### §1.3 본 cycle 측 결정 측 의미

본 cycle 측 hexa CLI runtime 측 본 repo 측 가용 측 활성화 측 path 측 결정 측 land. predecessor 23 marker 측 누적 caveat 측 closed-out 측 가능 — 사용자 lock-in 후 implementation cycle 측 실제 link / wrapper 측 land.

### §1.4 Track H decision pattern mirror

본 doc 측 Track H (`docs/ROADMAP_OP_ADOPTION_DECISION.md`) 측 decision pattern 측 직접 mirror — 11 criteria scoring matrix + 권고 + confidence + fallback + 사용자 lock-in checklist + caveat trail closure. Track H 측 admin tool (`roadmap_op.hexa`) 측 도입 측 결정 vs 본 doc 측 runtime (interpreter binary) 측 도입 측 결정 — **scope 측 차이 측 명시 (admin tool source code 측 본 repo 측 file vs runtime binary 측 본 repo 측 link / clone)**, methodology 측 동일.

---

## §2 3-way option analysis

### §2.1 Option A — anima clone (`~/.hx/bin/hexa` + `~/core/hexa-lang/build/hexa_interp.real` chain link)

**정의**: 본 repo `build/hexa_interp` 측 wrapper 측 redirect 측 `~/.hx/bin/hexa` 측 system-wide hexa command 측 invoke. interpreter binary 자체는 본 repo 측 emit 0 — sibling repo (hexa-lang) 측 SSOT 측 의존. 본 repo 측 missing 항목 = 본 repo wrapper 측 hexa-lang version pin + selftest path.

**Pros**:
- **검증된 binary** — hexa-lang upstream repo 측 long-term 운영 (Apr 2025 부터 hxc_aot / hxc_bench 측 cycle 누적 evidence). 본 cycle 측 별도 audit 측 burden 감소.
- **dev cost low (1-2 cycle)** — 본 repo `build/hexa_interp` wrapper 측 fork-storm cap + version pin + selftest 측 수정 측 1 cycle 측 land 추정.
- **즉시 unlock** — hexa-lang interpreter 측 already-built binary 측 immediate 가용 — 23 caveat trail 측 1 cycle 측 closed-out 가능.
- **own1 정합 (admin tool lens)** — interpreter 측 게임 client runtime 측 무관 (admin / build / dev tool). own1 의 `Wine 0 / CrossOver 0 / GPTK 0` 측 직접 위반 X. own1 의 "외부 게임 runtime 의존 0" 정신 측 admin tool 측 적용 시 hexa interpreter 측 admin tool 측 정의 측 정합.

**Cons**:
- **external dependency expansion** — 본 repo 측 hexa-lang repo 측 sibling presence 의존. standalone clone 시 hexa-lang repo 측 부재 시 schema-only posture 측 fall back. (본 repo plugin distribution 측 hexa-lang repo 측 prerequisite 측 명시 필요.)
- **version coupling** — hexa-lang upstream 측 후속 변경 시 sync drift risk. 매 cycle 측 "본 repo 측 어느 hexa-lang commit-sha 측 frozen?" 추적 burden.
- **license audit pending** — hexa-lang 측 license 측 본 repo plugin 측 호환 여부 별도 audit 필요.
- **Track G C1 caveat 측 영구 의존** — `hexa_interp` 라는 binary 측 build pending 측 anima 측 내부 history 측 caveat 측 본 repo 측 inherit (per Track G context).
- **own1 disambiguation 필요** — own1 측 본래 게임-실행 layer (Wine 0) 의 lens — interpreter 측 admin tool lens 측 own1 측 확장 적용 시 사용자 측 lock-in 시 명시 ack 필요 (Track H precedent 동일).

### §2.2 Option B — self-impl (gamebox repo 자체 hexa interpreter scratch build)

**정의**: 본 repo 측 hexa interpreter 측 scratch from-zero 구현. lexer + parser + AST + bytecode / direct AST tree-walking interpreter 측 본 repo 측 land. native/ directory 측 C/C++/Rust 측 native binary 측 build, sibling hexa-lang repo 측 reference inspiration 만 (file import / clone X).

**Pros**:
- **zero external dependency** — own1 (Wine 0 / 외부 runtime 0) 정의 측 strict 정합 (admin tool lens 측 자체 포함). 본 repo 측 standalone clone 측 plugin 측 self-contained.
- **repo-local SSOT (interpreter source)** — sibling repo 측 sync drift 측 0. 본 repo 측 hexa dialect 측 first-class control.
- **plugin distribution safety** — 본 repo plugin install 시 interpreter 측 자동 build / land — hexa-lang prerequisite 측 0.
- **future-proof dialect** — 본 repo first-mover hexa feature (e.g., per-game roadmap-specific syntax sugar) 측 즉시 land 가능. upstream upstream-merge 측 burden 0.

**Cons**:
- **massive dev cost (months horizon)** — interpreter scratch build 측 lexer / parser / AST / runtime / stdlib 측 cumulative horizon 측 ≥ 3 month 추정. 본 repo cap (90min cycle) 측 prohibitive.
- **hexa lang spec 정합 risk** — sibling hexa-lang repo 측 dialect 측 first-class compatibility 측 보장 측 본 repo 측 매 cycle 측 audit burden. "본 repo lib/*.hexa 측 본 repo interpreter 측 동작 + sibling interpreter 측 동작 = 동일 결과" 측 cross-validation cycle 측 별도 land 필요.
- **bug surface massive** — 신규 interpreter 측 정성적 bug surface 측 매우 큼 (parser edge case / runtime memory model / stdlib coverage) — minimum-viable 측 Track H 권고 self-impl 측 admin tool (≥ 2 cycle) 측 비교 측 한 자릿수 cycle 측 prohibitive.
- **own1 strict 정합 측 marginal benefit** — Option A 측 admin tool lens 측 own1 정합 측 disambiguate 가능 — Option B 측 own1 strict 정합 측 marginal benefit 측 dev cost 측 정당화 측 disproportionate.
- **upstream re-invention** — sibling hexa-lang repo 측 already-built + 운영 검증된 binary 측 abandon 측 strategic mismatch.

### §2.3 Option C — bypass (`schema-validated only` 영구 posture 유지)

**정의**: 본 cycle posture 측 23 cycle 측 일관되게 lateralized 측 `hexa CLI absent — schema-validated only` 측 영구 posture 측 명시 lock-in. interpreter 측 link / clone / scratch 측 emit 0 — 본 repo 측 hexa file 측 schema validation (python3 json.loads + sha256) only 측 영구 운영. runtime smoke 측 alternate path (e.g., python wrapper 측 strict subset hexa AST evaluator) 측 별도 cycle 측 결정.

**Pros**:
- **zero new infrastructure** — 본 cycle posture 측 즉시 lock-in 가능. 본 repo 측 추가 emit 측 0 (decision doc + handoff + marker 만).
- **23 cycle 일관 유지** — Track A-V 측 같은 caveat 측 cycle posture 측 그대로 유지 측 cycle drift 측 0. 사용자 측 cycle posture 측 audit 측 reproducibility 측 가장 높음.
- **own1 strict 정합** — 본 repo 측 외부 runtime / admin tool 측 의존 0 측 strict 정합. own1 + own2 측 disambiguation 측 0 — interpreter 측 admin tool lens 측 own1 측 확장 측 필요 X.

**Cons**:
- **runtime emit 영구 unverified** — production smoke 측 hexa file 측 actual execution 측 영구 차단. 본 repo 측 실 운영 시 runtime emit (e.g., self_test() return value, lib/loader/ dispatch result) 측 verify 측 영구 manual / external dependency.
- **production posture 측 도달 차단** — 사용자 측 본 repo 측 production-grade plugin 측 release 측 schedule 시 runtime smoke 측 hard-block — Option C 측 이 path 측 영구 차단 OR alternate path 측 별도 cycle 측 land 필요.
- **alternate path 측 burden 측 deferred** — `production smoke 측 alternate path` 측 별도 cycle 측 결정 측 미루는 만 — Option C 측 본질적 결정 측 deferred 측 본 doc 측 honest disclosure 필요.
- **own2 측 weak alignment** — 본 repo 측 emit 측 actual execution 측 verify 0 측 own2 (honest emit + log/error) 측 weak — schema-validation 만으로 own2 측 partial 정합. runtime emit 측 verify path 측 본 repo 측 영구 absent.

---

## §3 결정 매트릭스 (low=1 / med=3 / high=5; 점수 낮을수록 정합)

> **scoring direction**: 본 매트릭스 측 모든 criteria 측 **낮을수록 better**. 즉 dev_cost 1 = 매우 낮음 (good), drift_risk 5 = 매우 높음 (bad), runtime_smoke_capability 측 reverse score (5 = 매우 낮음 = bad). 명료성 측 매 row 측 "good direction" column 명시.

| criterion | A: anima clone | B: self-impl | C: bypass | good direction | 비고 |
|---|---|---|---|---|---|
| **dev_cost** | 1 (low, 1-2 cycle) | 5 (high, ≥3 month horizon) | 1 (low, 0 emit) | 낮을수록 better | A = wrapper version pin + selftest 1 cycle; B = interpreter scratch ≥ 3 month; C = posture lock-in only |
| **external_dependency_count** | 3 (med) | 1 (low) | 1 (low) | 낮을수록 better | A = hexa-lang sibling 의존; B = self-contained; C = posture-only no new dep |
| **drift_risk** | 3 (med) | 1 (low) | 1 (low) | 낮을수록 better | A = upstream sync drift cycle-by-cycle; B = self-controlled dialect; C = no drift (no land) |
| **test_burden** | 3 (med) | 5 (high) | 1 (low) | 낮을수록 better | A = wrapper selftest + version pin verify; B = full interpreter test surface; C = 0 |
| **standalone_clone_safety** | 3 (med) | 1 (low) | 1 (low) | 낮을수록 better | A = hexa-lang prerequisite (사용자 plugin install 시 docs 명시); B = self-contained; C = posture-only |
| **hexa_native_alignment** | 1 (low risk = full alignment) | 3 (med, dialect first-class but cross-validation burden) | 5 (high risk = no actual hexa runtime, alignment unverified) | 낮을수록 better | A = sibling-tested binary 측 dialect 측 first-class; B = own dialect but cross-check vs sibling burden; C = no runtime = no alignment |
| **ssot_purity** | 3 (med, sibling SSOT) | 1 (low, repo SSOT) | 1 (low, no source emit) | 낮을수록 better (1 = 본 repo 측 SSOT) | A = hexa-lang SSOT; B = repo SSOT; C = no SSOT (no emit) |
| **version_coupling** | 5 (high, hexa-lang HEAD coupling unless commit-sha pin) | 1 (low, repo-internal) | 1 (low) | 낮을수록 better | A = hexa-lang upstream version 측 marker pin 필요; B = no upstream coupling; C = no land |
| **runtime_smoke_capability** | 1 (low risk = full capability) | 1 (low risk = full capability after build) | 5 (high risk = 0 capability) | 낮을수록 better (1 = full smoke) | A = hexa-lang interpreter 측 즉시 dispatch; B = self-built interpreter 측 동작 (build 후); C = runtime smoke 영구 차단 |
| **own1_alignment (Wine 0 / 외부 runtime 0, admin tool lens 확장)** | 3 (partial, admin tool lens 확장 필요) | 1 (full, self-contained) | 1 (full, no external) | 낮을수록 better | A = admin tool lens disambiguation 측 사용자 lock-in 명시 ack 필요; B = strict 정합; C = strict 정합 단 admin tool 자체 X |
| **own2_alignment (honest emit + runtime verify)** | 1 (low, runtime smoke 측 hexa-lang 측 actual emit verify 가능) | 1 (low, self-built interpreter 측 actual emit verify) | 5 (high, runtime emit 영구 unverified — log/error 측 schema-only) | 낮을수록 better | A = honest emit verify 가능; B = honest emit verify 가능 단 build 후; C = runtime own2 영구 weak |
| **합계 (낮을수록 better, 11 criteria)** | **27** | **25** | **23** | — | C 측 최저 점수 단 runtime_smoke_capability + own2_alignment 측 5 (max) |

### §3.1 합계 해석 (단순 합계 측 misleading — disqualifying criterion 측 명시)

- **C bypass = 23 (formal lowest)** 단 **runtime_smoke_capability = 5 + own2_alignment = 5** = production smoke 측 영구 차단 측 disqualifying. 단순 합계 측 winner 측 본 repo 측 production posture 측 영구 차단 측 critical 측 weight 측 underestimates.
- **B self-impl = 25** 단 **dev_cost = 5 + test_burden = 5** = 본 repo cap (90min cycle) 측 prohibitive. ≥ 3 month horizon 측 사용자 측 정당화 측 strategic justification 필요.
- **A anima clone = 27 (highest formal)** 단 **dev_cost 1 + runtime_smoke_capability 1 + own2_alignment 1 + hexa_native_alignment 1** = production posture 측 즉시 unlock 가능. own1 admin tool lens disambiguation 측 사용자 lock-in 시 명시 ack 측 정합 가능.

→ 매트릭스 측 **단순 합계 측 misleading** — disqualifying criterion (runtime_smoke_capability + own2_alignment 측 5) 측 weight 측 사용자 측 own1 strict-vs-pragmatic stance 측 결정. 본 doc 측 **사용자 측 own1 의 'admin tool lens 측 외부 게임 runtime 0 측 분리 적용' 측 lock-in 가정 측 권고 = A**.

### §3.2 매트릭스 측 비대칭 측 honest disclosure (own2 정합)

- **C bypass 측 single weakness**: runtime_smoke_capability 5 + own2_alignment 5 — 두 가장 critical criteria 측 max value. 합계 23 측 best 단 본 repo production posture 측 영구 차단 측 disqualifying.
- **B self-impl 측 single weakness**: dev_cost 5 + test_burden 5 — 본 repo cap 측 prohibitive. ≥ 3 month horizon 측 strategic redirection 필요.
- **A anima clone 측 single strength**: production posture 측 즉시 unlock 가능 (1-2 cycle). external_dependency + version_coupling 측 medium-high 단 sibling repo 측 already-stable.

---

## §4 권고 — Option A (anima clone), confidence MEDIUM

### §4.1 권고 측 근거

1. **production posture 측 즉시 unlock** — 23 cycle 누적 caveat 측 1-2 cycle 측 closed-out 가능. runtime emit 측 schema-only → actual execution 측 transition 측 본 repo production-grade plugin release 측 unblock.
2. **dev cost 합리적** — wrapper version pin + selftest path 측 1-2 cycle. Option B (≥ 3 month) 측 비교 측 100x cost 효율.
3. **sibling already-stable** — hexa-lang interpreter 측 long-term 운영 검증 (Apr 2025 부터 hxc_aot cycle / hxc_bench cycle / 본 repo Track A-V 측 indirect dependency 측 stability 측 evidence 측 누적).
4. **own1 admin tool lens 측 disambiguation 가능** — Track H precedent 측 동일 — admin tool 측 own1 측 확장 적용 측 사용자 lock-in 시 명시 ack 측 정합 회복 가능.

### §4.2 confidence level — MEDIUM (not MEDIUM-HIGH or HIGH)

- **MEDIUM (not HIGH)** 이유:
  - **own1 admin tool lens 확장 측 사용자 lock-in 측 explicit ack 의존** — 본 repo 측 own1 측 strict reading (Wine 0 측 모든 외부 runtime 0) 측 사용자 측 lock-in 시 admin tool 측 분리 적용 측 명시 ack 측 정합 회복 — ack 측 부재 시 own1 측 violation risk.
  - **hexa-lang version pin 측 marker 측 freeze 필요** — Track H A 옵션 동일 caveat — version pin 측 reproducibility 측 사용자 측 cycle-by-cycle 측 책임.
  - **license audit pending** — hexa-lang 측 license 측 본 repo plugin distribution 측 호환 여부 측 사용자 측 audit 결과 측 lock-in 측 own2 정합.
  - **standalone clone safety 측 caveat** — 본 repo plugin 측 install 시 hexa-lang sibling 측 prerequisite 측 명시 필요. 사용자 측 plugin distribution 측 cross-OS scope 측 명시 lock-in 필요.
- **MEDIUM (not LOW)** 이유:
  - sibling hexa-lang 측 already-stable + 본 repo 측 indirect dependency 측 23 cycle 측 evidence — Option A 측 risk 측 manageable.

### §4.3 fallback 시나리오

- **A 측 hexa-lang license 측 본 repo plugin 측 incompatible 시** → **Option C bypass** 측 영구 fallback. alternate verification path (e.g., python3 측 hexa AST subset evaluator) 측 별도 cycle 측 land 측 사용자 측 sequencing 측 lock-in 필요.
- **A 측 cross-OS portability 측 fail 시 (Linux / Windows 측 hexa-lang interpreter binary 측 build 측 부재)** → **Option C bypass** 측 cross-OS 측 partial fall back. macOS 측 A path 측 sustain.
- **B self-impl 측 ≥ 3 month horizon 측 strategic 측 정당화 X** — 사용자 측 명시적 lock-in 시 별도 prolonged cycle (6-cycle 이상) 측 land 측 가능 단 본 doc 측 권고 X.

### §4.4 권고 안 측 단정 측 회피 (own2 정합)

- 본 § 측 권고 측 사용자 측 final lock-in 측 대체 X — 매트릭스 + 권고 + confidence level + fallback + 사용자 lock-in checklist 측 제시 측 사용자 측 informed decision 측 enable 만.
- 본 cycle 측 **구현 X** (decision-only). 구현 cycle 측 사용자 lock-in 후 별도 cycle 측 진행.

---

## §5 사용자 lock-in 체크리스트 (decision pending)

> **상태**: ⏳ awaiting user lock-in

다음 항목 측 사용자 측 명시적 lock-in 필요 — 본 cycle 후속 implementation cycle 시작 전:

- [ ] **option choice**: A (anima clone) / B (self-impl) / C (bypass) 측 1 개 선택
- [ ] **(if A)**: hexa-lang 측 어느 version / branch / commit-sha 측 frozen reference (e.g., `hexa-lang@2026-05-03 commit XXX` 측 build/hexa_interp.real 측 본 repo 측 freeze pin)
- [ ] **(if A)**: hexa-lang 측 license 측 본 repo plugin 측 호환 여부 audit 결과
- [ ] **(if A)**: interp binary install path policy — `~/.hx/bin/hexa` system-wide vs `~/core/hexa-lang/build/hexa_interp` repo-local vs 본 repo `build/hexa_interp` wrapper 측 어느 entry-point 측 SSOT
- [ ] **(if A)**: own1 admin tool lens 확장 적용 측 explicit ack — interpreter 측 admin / build / dev tool 측 lens 측 own1 (Wine 0) 측 strict 정의 측 분리
- [ ] **(if A)**: cross-OS portability 측 scope 측 lock-in (macOS only / Linux container / Windows 측 N/A)
- [ ] **(if B)**: implementation cycle 추정 — minimum-viable scope (e.g., interpreter for `self_test()` + `main()` dispatch only, no full lang) 측 cycle count 측 사용자 측 정당화
- [ ] **(if B)**: hexa lang spec 측 어느 dialect 측 alignment 측 commitment (sibling hexa-lang full vs subset vs 본 repo first-class divergence)
- [ ] **(if B)**: native binary build path (Rust / Go / C++ / Hexa 측 self-host) 측 사용자 lock-in
- [ ] **(if C)**: alternate verification path 측 결정 — python3 측 strict hexa AST subset evaluator vs system Python wrapper vs 영구 schema-only
- [ ] **(if C)**: production posture 측 영구 차단 측 사용자 측 명시적 acknowledge — plugin distribution 측 runtime emit 측 영구 unverified 측 인지
- [ ] **raw 168 minimum-viable acknowledge**: 본 cycle 측 decision doc only — `hexa CLI runtime` 측 본 cycle 측 emit 0건 측 사용자 측 정합 ack
- [ ] **schema migrate 의무 X 측 명시 acknowledge**: 본 repo policy `migration: forbidden` 측 정합 — interp wrapper 측 in-place modify 측 0 vs additive only

---

## §6 cross-link to predecessor markers (hexa CLI absent caveat trail — 23 markers)

본 issue 측 누적 caveat 가 다음 23 markers 측 명시되어 있다 (Track A 부터 Track V, completion_iso 시점 순):

1. `state/markers/airgenome_gamebox_self_mk2_tuning_landed.marker` — predecessor seed cycle (Track 0)
2. `state/markers/airgenome_gamebox_rank_a_3_domain_landed.marker`
3. `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker`
4. `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker`
5. `state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker`
6. `state/markers/airgenome_gamebox_gameguard_honest_freeze_landed.marker`
7. `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker`
8. `state/markers/airgenome_gamebox_d2r_realm_protocol_honest_freeze_landed.marker`
9. `state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker`
10. `state/markers/airgenome_gamebox_battlenet_online_stack_augment_landed.marker`
11. `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker`
12. `state/markers/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed.marker`
13. `state/markers/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed.marker` (Track G — `hexa_interp` build pending caveat C1 측 명시 source)
14. `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker`
15. `state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker`
16. `state/markers/airgenome_gamebox_perf_game_d2r_integration_landed.marker`
17. `state/markers/airgenome_gamebox_perf_game_d4_retired_intentional_posture_landed.marker`
18. `state/markers/airgenome_gamebox_perf_manjeom_aggregation_review_landed.marker`
19. `state/markers/airgenome_gamebox_loader_native_helper_landed.marker`
20. `state/markers/airgenome_gamebox_closure_orchestrator_landed.marker`
21. `state/markers/airgenome_gamebox_roadmap_op_adoption_decision_landed.marker` (Track H — direct decision pattern mirror)
22. `state/markers/airgenome_gamebox_roadmap_op_self_impl_landed.marker`
23. `state/markers/airgenome_gamebox_roadmap_op_full_11_of_11_landed.marker`

→ 본 doc 측 land 측 위 23 markers 측 누적 caveat 측 **decision matrix 측 explicit 정리 단계 도달** (구현 X, 사용자 lock-in 대기). 후속 implementation cycle 측 별도 marker 측 land 후 위 23 markers 측 caveat trail 측 closed.

---

## §7 caveats (≥ 6 honest items)

- **C1 (matrix scoring 주관성)**: § 3 measurement criteria 측 11 항목 측 score 측 own1 / own2 정합 측 lens 측 주관 측 반영 — 사용자 측 다른 weight 측 적용 시 winner 측 다를 수 있음. 매트릭스 측 honest 한 starting point 측 제시이지 절대 측정 X. 단순 합계 측 misleading (C bypass 측 23 best 측 disqualifying criteria 측 weight 측 underestimates).
- **C2 (hexa-lang version commit-sha frozen pin 측 부재)**: 본 cycle 측 sibling `~/core/hexa-lang/build/hexa_interp.real` 측 presence 확인 (Mach-O arm64, 1580720 bytes, 2026-05-02 mtime) — 단 commit-sha pin 0. A option 측 land 시 사용자 측 commit-sha 측 명시 측 marker 측 frozen pin 필요.
- **C3 (B self-impl 측 dev cost ≥ 3 month horizon 측 추정 측 underdetermined)**: § 2.2 측 ≥ 3 month 추정 측 본 repo hexa lang spec coverage 측 actual measurement X 측 추정. minimum-viable subset (interpreter for `self_test()` + `main()` dispatch only) 측 cycle estimate 측 사용자 측 conservative / aggressive 측 판단 측 lock-in 필요. self-host 측 native binary build chain 측 (Rust/Go/C++) 측 별도 lock-in 항목.
- **C4 (cross-OS portability 측 macOS-arm64 가정)**: § 1.2 sibling audit 측 macOS arm64 binary 측 확인 — Linux variants 측 `hexa_interp_linux_arm64` / `hexa_interp_linux_x86_64` 측 sibling presence 단 docker container path 측 의존. Windows 측 hexa-lang binary 측 build 측 부재 (raw 15 portability 위반 가능). plugin distribution 측 cross-OS scope 측 사용자 측 lock-in 필요.
- **C5 (own1 admin tool lens 확장 적용 측 disambiguation 측 사용자 lock-in 의존)**: own1 측 본래 게임-실행 layer (Wine 0 / CrossOver 0) 측 lens — interpreter 측 admin / build / dev tool 측 lens 측 own1 측 분리 적용 측 사용자 측 lock-in 시 명시 ack 측 정합 회복 가능. ack 측 부재 시 own1 측 violation risk. (Track H A 옵션 + Track 11 `battlenet_bypass_own1_disambiguation_landed.marker` 측 precedent — 동일 disambiguation pattern.)
- **C6 (alternate verification path 측 Option C 측 deferred decision)**: Option C 측 영구 schema-only posture 측 production runtime smoke 측 alternate path (e.g., python3 측 strict hexa AST subset evaluator) 측 별도 cycle 측 결정 측 deferred. 본 cycle 측 honest disclosure — Option C 측 본질적 결정 측 deferred 측 사용자 측 sequencing lock-in 필요.
- **C7 (own2 weak alignment for Option C)**: Option C 측 runtime emit 측 schema-only — log / error / runtime self_test 측 actual execution 측 verify 0 측 own2 (honest emit) 측 weak alignment. 본 doc 측 own2 측 partial 정합 측 honest 명시. (대조 측 Option A + B 측 own2 strict 정합 가능.)
- **C8 (test 측 본 cycle 측 emit X)**: 본 cycle 측 decision doc only — interpreter selftest / wrapper round-trip test / sibling binary smoke test 측 0건. 본 cycle 측 raw 9 (audit-only no impl emit) + raw 168 (minimum-viable exempt) 측 정합 — 단 implementation cycle 측 selftest 측 필수 land.
- **C9 (Track G `hexa_interp` build pending caveat C1 측 영구 의존 risk for Option A)**: Track G `track_g_field_norm_d2r_cond3_landed.marker` 측 caveat C1 측 anima 측 내부 history 측 inherit — A clone path 측 land 시 anima sibling caveat 측 본 repo 측 inherit 측 cross-repo audit burden 측 별도 marker 측 명시 필요.
- **C10 (rate-limit recovery 측 disk write priority)**: 본 cycle 측 BG subagent 측 rate-limit recovery 시점 측 disk write 측 priority — report 측 deferred 가능. 본 doc + 후속 handoff doc + marker 측 disk land 측 cycle 측 success criteria. (Track H precedent 동일.)
- **C11 (raw 9 hexa_only / raw 168 minimum-viable / raw 175 BR-NO-USER-VERBATIM 측 정합 매 cycle 명시)**: 본 cycle 측 decision doc + handoff + marker = 3 file 추가 — `tool/hexa_cli_runtime/*` 측 emit 0건. raw 9 (audit-only no impl emit) + raw 168 (decision-only minimum-viable exempt) + raw 175 (no verbatim user quotes) 측 정합. 사용자 directive paraphrase only.

---

(decision doc end. § 1-7, 11 criteria scoring, recommendation A confidence MEDIUM, lock-in 측 사용자 대기. predecessor 23 marker caveat trail 측 closure 측 본 doc 측 land 측 단계 도달.)
