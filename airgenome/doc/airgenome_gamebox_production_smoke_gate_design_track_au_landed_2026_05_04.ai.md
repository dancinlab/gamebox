# airgenome-gamebox — Track AU production smoke gate design landed (2026-05-04)

> **TL;DR**: Phase 3+ production smoke gate (real binary execution path) 측 도달 측 path 측 design-level 측 3-way option (A own1-maintained / B Wine 정책 재정의 / C 외부 VM) 측 11-criteria 매트릭스 + 권고 (A own1 maintained, multi-year horizon ack) + confidence MEDIUM + 사용자 lock-in 대기. design-only land (cleanest cycle posture mirror Track I/L/T/Y/Z/AA/AN, in_place_writes=0). own1 (Wine 0 / 외부 game runtime 0 / hexa-native) + own2 (3 option 매 매트릭스 explicit / Option B own1 break advocate X honest 분석 / 14 honest C3 caveats inline). additive_only / migration forbidden / in_place_writes=0 / destructive_ops=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker.

## §0 baseline

- predecessor handoff (가장 직접): `airgenome/doc/airgenome_gamebox_hexa_upstream_retrofit_track_ao_landed_2026_05_04.ai.md` (Track AO 가장 최근 land), `airgenome/doc/airgenome_gamebox_mk2_checkpoint_schema_design_landed_2026_05_04.ai.md` (Track AA, design-only with lock-in checklist 직접 mirror)
- predecessor decision doc style mirror: `docs/ROADMAP_OP_ADOPTION_DECISION.md` (Track H, 11-criteria 3-way matrix) + `docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` (Track Y, 3-way runtime decision matrix) + `docs/CROSS_TOOL_INTEGRATION_DESIGN.md` (Track Z, design-only cycle posture) + `docs/MK2_CHECKPOINT_SCHEMA_DESIGN.md` (Track AA, design-only lock-in checklist) + `docs/SESSION_2026_05_03_TO_05_04_AGGREGATE.md` (Track AN, current state aggregate reference)
- 본 cycle 측 lib/ touch 0건 / tool/ touch 0건 / native/ touch 0건 / tests/ touch 0건 / `.roadmap.*` touch 0건 / `.checkpoint.*` 측 file 0건 / 기존 docs/*.md touch 0건 (read-only reference) / 기존 state/markers/*.marker touch 0건 (read-only reference)

## §1 user_directive_paraphrase

BG subagent — Track AU production smoke gate design land for airgenome-gamebox: Phase 3+ deferred 측 multiple cycle markers 측 누적 측 design-level 측 3-way decision matrix (real binary 측 actual execution path) 측 doc-only 정리. 3 option — A own1 maintained (status quo, multi-year horizon, gamebox closure ~80%+ multi-year, D2R offline months-1yr+, D2R online 1-2yr+, D4/L Classic R28/cs2/delta_force/elden_ring 영구 retired_intentional, Lineage M Android emul permanent boundary), B Wine 정책 재정의 (Wine/CrossOver/Whisky/GPTK 도입 측 own1 break Wine 0 → Wine N, 즉시 D2R/D4/Lineage 실행, ban risk Warden CrossOver detection community evidence, ToS 정합 grey area), C 외부 Windows VM (Mac 측 Windows VM Parallels/VMware/UTM 측 별도 환경, gamebox plugin 측 무관, ban risk 0, own1 정책 유지 단 gamebox-purpose-alignment 미충족 Apple-only stack 미충족). 11-criteria scoring (own1_alignment / own2_alignment / horizon_to_first_frame / ban_risk / plugin_scope_intact / gamebox_purpose_alignment / ToS_alignment / user_lock_in_required / multi_game_coverage / runtime_smoke_capability / effort_dev_cost) low=1/med=3/high=5 lower=better. 권고 = A own1-maintained confidence MEDIUM, fallback C 외부 VM, B advocate X honest 분석. 14 honest caveats inline + 12 사용자 lock-in checklist items (option choice + horizon ack + own1 redefinition + ban risk + ToS + raw 168 minimum-viable + schema migrate forbidden ack 등). 1 NEW design doc (`docs/PRODUCTION_SMOKE_GATE_DESIGN.md`, 366 LOC, 11 sections + 14 caveats) + 1 NEW handoff (this file) + 1 NEW marker (`state/markers/airgenome_gamebox_production_smoke_gate_design_track_au_landed.marker`). policy: additive_only / migration forbidden / destructive_ops=0 / in_place_writes=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land. allowed scope: 3 NEW files only; 그외 forbidden (read-only reference). predecessor pattern: Track H/Y/Z/AA/AN design-only pending lock-in pattern siblings, 직접 mirror.

## §2 design summary

### §2.1 production smoke gate 정의 + 현재 상태 (Track AN reflect)

- **Phase 1 schema-validated emit** (현재 활성): closure_pct 29.55%, validated_manjeom 0
- **Phase 2 runtime smoke (synthetic round-trip)**: Track Y decision-only pending — 본 doc Phase 3+ 측 chained dependent
- **Phase 3+ production smoke (real binary execution path)**: 영구 deferred — **본 doc 측 design 측 대상**
- Win32 syscall coverage cycle 43 시점 218/435 ~50.11% skeleton-tier (Track AM bundled, 50% threshold breach milestone)

### §2.2 3-way option 매트릭스 summary (11 criteria)

| criterion | A: own1-maintained | B: Wine 재정의 | C: 외부 VM | good direction |
|---|---|---|---|---|
| own1_alignment | 1 | 5 | 1 | low |
| own2_alignment | 1 | 3 | 3 | low |
| horizon_to_first_frame | 5 | 1 | 1 | low |
| ban_risk | 1 | 5 | 1 | low |
| plugin_scope_intact | 1 | 5 | 1 | low |
| gamebox_purpose_alignment | 1 | 3 | 5 | low |
| ToS_alignment | 1 | 5 | 1 | low |
| user_lock_in_required | 3 | 5 | 5 | low |
| multi_game_coverage | 5 | 1 | 1 | low |
| runtime_smoke_capability | 5 | 1 | 1 | low |
| effort_dev_cost | 3 | 5 | 1 | low |
| **합계 (lower=better)** | **27** | **39** | **21** | — |

### §2.3 권고 = Option A own1-maintained, confidence MEDIUM

- **C = 21 (formal lowest)** 단 gamebox_purpose_alignment = 5 (max) = disqualifying for 본 repo design intent ("Apple-native stack plugin")
- **A = 27 (medium)** 단 own1 / own2 / ban_risk / plugin_scope / gamebox_purpose / ToS / effort 측 모두 1-3 = strict 정합 + multi-year horizon ack 측 trade-off 정합 회복
- **B = 39 (highest)** = own1 break + ban risk + ToS + plugin scope + license + drift cumulative disqualifying
- → 권고 **A**, confidence **MEDIUM** (multi-year horizon expectation mismatch risk + Track Y chained dependent + Win32 coverage threshold underdetermined + multi-game coverage 제약)
- fallback: **C 외부 VM** (design intent 측 mismatch 시 redirect, own1 + ban risk 정합)
- B advocate **X** (own1 break + ban risk + ToS / EULA / license 측 cumulative disqualifying, honest 분석 only)

## §3 own1 / own2 alignment

### §3.1 own1 (Wine 0 / 외부 game runtime 0 / hexa-native)

- 본 doc 측 design 자체 측 own1 strict 정합 (3 NEW file 측 모두 hexa-native + 외부 의존 0)
- Option A 측 권고 = own1 strict 정합 (Wine 0 / 외부 game runtime 0 invariant 유지)
- Option B 측 own1 break 측 explicit disclosure (advocate X, 분석 only — caveat C5)
- Option C 측 gamebox plugin 측 외부 — own1 plugin lens 측 strict 정합 (Track H/Y/AA precedent 정합)
- own1 verdict (design 자체): **PASS**

### §3.2 own2 (design honest / silent_swallow 0)

- 3 option 매 매트릭스 + 권고 + confidence + tradeoff explicit 측 own2 정합
- swallowed tradeoff 0 — 매 option 측 verdict (PASS / FAIL conditional / PASS conditional) 측 명시 (NOT silent_swallow)
- Option B own1 break + ban risk + ToS / EULA / license 측 cumulative disqualifying 측 explicit disclosure
- Option C gamebox-purpose-alignment 측 본질적 redefine 측 explicit disclosure
- 14 honest C3 caveats inline (≥10 raw 10 정합 요구 +4 추가)
- own2 verdict (design 자체): **PASS** (explicit verdict NOT silent_swallow)

### §3.3 additive at design level lock-in

- 기존 `.roadmap.*` schema (cond / blk) 측 in-place modification 0건
- 기존 `lib/` / `tool/` / `native/` / `tests/` 측 modification 0건 (read-only reference)
- 기존 `docs/*.md` 측 modification 0건 (read-only reference)
- 기존 `state/markers/*.marker` 측 modification 0건 (read-only reference)
- 본 cycle 측 NEW file 3건만 (design doc + handoff + marker)


본 cycle 측 design doc §10 측 inline 14 honest C3 caveats:

- **C1** — multi-year horizon 측 사용자 expectation mismatch risk (Option A)
- **C2** — Track Y runtime smoke chained dependent (Phase 2 ↔ Phase 3+)
- **C3** — Win32 syscall coverage threshold (~80%+) 측 estimate underdetermined
- **C4** — closure_pct progression 측 cycle-by-cycle ~1-2pct estimate underdetermined
- **C5** — Option B own1 redefinition 측 사용자 lock-in 측 explicit 의무 (advocate X)
- **C6** — Warden L2 CrossOver community evidence 측 grey area
- **C7** — Apple GPTK dev-only license 측 production redistribution prohibit
- **C8** — Option C VM 측 arm64 emulation overhead
- **C9** — gamebox-purpose-alignment 측 본질적 redefine (Option C)
- **C10** — multi-game coverage 측 영구 retired_intentional + Lineage M Android emul boundary 제약
- **C11** — own1 strict reading vs admin tool / plugin lens disambiguation (Track H/Y/AA precedent)
- **C12** — design-only land (impl 0건)
- **C13** — raw compliance 매 cycle 명시 (raw 9/10/11/12/15/168/175)
- **C14** — rate-limit recovery disk write priority

## §5 사용자 lock-in checklist (12 items, decision pending)

- (1) **option choice**: A / B / C 1 개 (default recommend: **A**)
- (2) **(if A)** multi-year horizon explicit acknowledge
- (3) **(if A)** Win32 syscall coverage threshold lock-in
- (4) **(if A)** Track Y runtime smoke chained dependency ack
- (5) **(if A)** retired_intentional + Lineage M boundary 제약 ack
- (6) **(if B)** own1 redefinition spec + plugin distribution prerequisite redefine
- (7) **(if B)** ban risk explicit acknowledge
- (8) **(if B)** ToS / EULA / license audit
- (9) **(if C)** gamebox-purpose-alignment ack
- (10) **(if C)** VM commercial license + Windows 11 ARM license + setup 비용 ack
- (11) **(any)** ban risk acknowledge + ToS implications
- (12) **raw 168 minimum-viable + schema migrate forbidden ack**

## §6 cross-link to design-only pending lock-in pattern siblings (Track H/Y/Z/AA + Track AN aggregate)

| track | doc | marker | relation |
|---|---|---|---|
| H | `docs/ROADMAP_OP_ADOPTION_DECISION.md` | `state/markers/airgenome_gamebox_roadmap_op_adoption_decision_landed.marker` | predecessor decision doc — 11 criteria 3-way matrix 직접 mirror |
| Y | `docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` | `state/markers/airgenome_gamebox_hexa_cli_runtime_build_decision_landed.marker` | predecessor decision doc — 3-way runtime decision matrix sibling, Phase 2 ↔ Phase 3+ chained dependent (caveat C2) |
| Z | `docs/CROSS_TOOL_INTEGRATION_DESIGN.md` | `state/markers/airgenome_gamebox_cross_tool_integration_design_landed.marker` | predecessor design-only cycle posture mirror — in_place_writes=0 cleanest cycle |
| AA | `docs/MK2_CHECKPOINT_SCHEMA_DESIGN.md` | `state/markers/airgenome_gamebox_mk2_checkpoint_schema_design_landed.marker` | predecessor design-only with lock-in checklist (11 items) — 본 doc lock-in checklist 직접 mirror |
| AN | `docs/SESSION_2026_05_03_TO_05_04_AGGREGATE.md` | (aggregate doc, separate marker) | current state reference — closure_pct 29.55%, validated_manjeom 0, Win32 50.11% 직접 reference |
| AU | (본 cycle) | (본 marker) | production smoke gate 3-way decision matrix (Phase 3+ deferred design freeze) |

## §7 file index

- `docs/PRODUCTION_SMOKE_GATE_DESIGN.md` — design doc (NEW, 366 LOC, 11 sections + 14 caveats, 41164 bytes)
- `airgenome/doc/airgenome_gamebox_production_smoke_gate_design_track_au_landed_2026_05_04.ai.md` — handoff (NEW, this file)
- `state/markers/airgenome_gamebox_production_smoke_gate_design_track_au_landed.marker` — marker (NEW)

## §8 policy summary

- migration: forbidden
- changes: additive_only
- in_place_writes: 0 (cleanest cycle posture mirror Track I/L/T/Y/Z/AA/AN)
- destructive_ops: 0
- cost_usd: 0
- substrate: mac-local
- cap_minutes: 90
- br_no_user_verbatim: true
- friendly_preset: true
- silent-land marker: enforced
- own1: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / hexa-native / 외부 의존 0 — design 자체 strict 정합 (3 NEW file 측 모두 hexa-native + 외부 의존 0)
- own2: design honest / 3 option 매 매트릭스 explicit + verdict explicit (PASS / FAIL conditional / PASS conditional) / 14 honest C3 caveats inline / silent_swallow 0
- design_only: implementation 0건 — production smoke gate 측 영구 deferred Phase 3+ caveat trail 측 explicit decision matrix 정리. 후속 implementation cycle 측 사용자 lock-in 후 결정 (or Option C fallback case 측 implementation X 측 영구 cycle posture 측 lock-in)

## §9 next cycle paths (사용자 lock-in 후 implementation cycle or 영구 deferred lock-in)

### §9.1 if Option A (own1 maintained, default recommended)

- 본 cycle 측 implementation 측 즉시 land 0건 — Track Y runtime smoke chained dependent (Phase 2) 측 사용자 측 lock-in + implementation 측 prerequisite
- Track Y land 후 cycle: closure_pct progression cycle (Track AG weighted-aware 측 cycle-by-cycle ~1-2pct) + Win32 syscall coverage 측 50.11% → ~80%+ multi-cycle skeleton-tier 누적
- D2R offline first frame: months-1yr+ horizon 측 cumulative skeleton + real binary 측 cross-validation cycle 측 land
- D2R online: offline path dependent + Warden L2 natural environment alignment cycle 측 별도 long-horizon (1-2yr+)

### §9.2 if Option B (Wine 재정의, 본 doc advocate X — 사용자 명시 lock-in 측 의무)

- own1 spec rewrite cycle (본 repo own1 invariant doc update + 새 own1 spec land)
- plugin distribution prerequisite redefine cycle (사용자 plugin install step + license issue)
- ban risk monitoring cycle (Warden CrossOver detection community evidence 측 cycle-by-cycle 추적)
- ToS / EULA / license audit cycle (Apple GPTK dev-only / CrossOver commercial / Wine GPL / Whisky open-source)
- selftest extension cycle (Wine wrapper 측 selftest + version pin)
- → cumulative ≥ 5 cycle 측 prerequisite (본 doc 측 advocate X)

### §9.3 if Option C (외부 VM, fallback recommended)

- 본 repo 측 implementation 측 0건 — 영구 cycle posture 측 lock-in
- 사용자 측 외부 Windows VM (Parallels / VMware / UTM) 측 setup 측 별도
- 본 repo plugin 측 의의 측 schema-validated only posture 측 영구 lock-in (Phase 1 only)
- 본 repo plugin 측 production-grade release 측 영구 미land

## §10 cross-link

- predecessor handoff (가장 직접): `airgenome/doc/airgenome_gamebox_hexa_upstream_retrofit_track_ao_landed_2026_05_04.ai.md` (Track AO, 가장 최근 land), `airgenome/doc/airgenome_gamebox_mk2_checkpoint_schema_design_landed_2026_05_04.ai.md` (Track AA, design-only lock-in checklist 직접 mirror)
- predecessor decision doc style mirror: `docs/ROADMAP_OP_ADOPTION_DECISION.md` (Track H), `docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` (Track Y), `docs/CROSS_TOOL_INTEGRATION_DESIGN.md` (Track Z), `docs/MK2_CHECKPOINT_SCHEMA_DESIGN.md` (Track AA)
- current state aggregate reference: `docs/SESSION_2026_05_03_TO_05_04_AGGREGATE.md` (Track AN — closure_pct 29.55%, validated_manjeom 0, Win32 50.11%)
- existing closure / production reference (read-only): `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` + `docs/CLOSURE_ROADMAP.md`
- production smoke gate caveat trail predecessors (read-only):
  - `state/markers/airgenome_gamebox_d2r_realm_protocol_honest_freeze_landed.marker` (D2R realm protocol Warden L2 long-horizon honest freeze)
  - `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker` (Warden L2 natural environment alignment posture)
  - `state/markers/airgenome_gamebox_gameguard_honest_freeze_landed.marker` (GameGuard kernel-mode retired_intentional cascading)
  - `state/markers/airgenome_gamebox_perf_manjeom_aggregation_review_landed.marker` (manjeom aggregation validated_manjeom 0)
  - `state/markers/airgenome_gamebox_closure_orchestrator_weighted_threshold_awareness_landed.marker` (closure_pct 25 → 29.55% Track AG)
  - `state/markers/airgenome_gamebox_win32_cycle40_43_bundled_increment_landed.marker` (Win32 50.11% threshold breach Track AM)

---

*written 2026-05-04 — Track AU production smoke gate design handoff. design-only land, additive_only / migration forbidden / in_place_writes=0 / destructive_ops=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker. 3-way option matrix 11 criteria scoring (A own1-maintained 27 / B Wine 재정의 39 / C 외부 VM 21 단순 합계 misleading — disqualifying criterion 측 명시). 권고 A own1-maintained confidence MEDIUM, fallback C 외부 VM, B advocate X honest 분석 only. 14 honest C3 caveats inline. 12-item user lock-in checklist. own1 hexa-native / 외부 의존 0 / own2 design honest 3 option verdict explicit (PASS / FAIL conditional / PASS conditional) NOT silent_swallow. implementation 0건 — production smoke gate 측 영구 deferred Phase 3+ caveat trail 측 explicit decision matrix 정리.*
