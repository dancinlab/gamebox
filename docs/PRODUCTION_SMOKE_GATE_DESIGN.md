# Production smoke gate design — real binary execution path 3-way decision matrix (Track AU, 2026-05-04)

> **status**: design-only doc / 작성 **2026-05-04** / 본 cycle 은 **구현 X**, 11-criteria 3-way 매트릭스 + 권고 + 사용자 lock-in 대기
> **scope**: 본 repo (airgenome-gamebox) 의 production smoke gate (real game binary 측 actual execution path) 측 도달 측 path 측 본질적 결정 측 design freeze. 현재 closure_pct 29.55%, validated_manjeom 0, Win32 syscall 측 cycle 43 시점 50.11% (218/435 fn skeleton-tier). real binary execution path 측 영구 deferred Phase 3+ caveat trail 측 누적 측 design-level 측 3-way option 측 explicit 정리 단계.
> **결론 (요약, confidence MEDIUM)**: **Option A (own1 maintained, status quo, multi-year horizon 명시 ack)** 권고 — own1 invariant (Wine 0 / 외부 게임 runtime 0) 측 본 repo 측 정의 측 design intent 측 정합 측 단일 path. **Option B (Wine 정책 재정의)** 측 own1 break + Warden L2 ban risk + ToS 정합 risk 측 disqualifying. **Option C (외부 Windows VM 측 별도 환경)** 측 ban risk 측 0 단 본 repo 측 design intent ("Apple-only stack") 측 미충족 측 fallback 후보.
> **lock-in pending**: A/B/C 측 1 개 선택 + (A 시) horizon acknowledge / (B 시) own1 redefinition spec / (C 시) gamebox-purpose-alignment ack + ban risk + ToS 정합 + raw 168 minimum-viable ack
> **predecessor 패턴**: Track H (`docs/ROADMAP_OP_ADOPTION_DECISION.md`, 11-criteria 3-way matrix) + Track Y (`docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md`, 3-way runtime decision matrix) + Track Z (`docs/CROSS_TOOL_INTEGRATION_DESIGN.md`, design-only cycle posture) + Track AA (`docs/MK2_CHECKPOINT_SCHEMA_DESIGN.md`, design-only with lock-in checklist) + Track AN (`docs/SESSION_2026_05_03_TO_05_04_AGGREGATE.md`, current state aggregate reference) — 모두 design-only pending lock-in pattern siblings, 본 doc 측 직접 mirror.

---

## §0 목적 / 한계 (purpose / scope freeze, design-only)

### §0.1 목적 (in-scope)

본 doc 의 목적은 다음 5 항목으로 한정된다:

1. 현재 Phase 3 production smoke gate 측 정의 + 현재 측정 상태 (closure_pct 29.55, validated_manjeom 0, Win32 50.11%) 측 narrative SSOT freeze
2. real binary 측 actual execution path 측 도달 측 3-way option (A own1-maintained / B Wine 정책 재정의 / C 외부 VM) 측 매트릭스 + 권고 + confidence + tradeoff explicit
3. 11-criteria scoring matrix 측 own1 / own2 정합 lens 측 measurement (Track H/Y precedent mirror)
4. 사용자 lock-in checklist 측 freeze (option choice + horizon ack + ban risk ack + ToS 정합 + raw 168 ack 등 8-12 항목)
5. own1 (Wine 0 / 외부 게임 runtime 0) + own2 (design honest, swallowed tradeoff 0) 정합 reflect — Option B 측 own1 break 측 honest disclosure (advocate X, 분석)

### §0.2 한계 (out-of-scope, 영구)

본 cycle 측 다음 항목은 **영구 out-of-scope** (구현 cycle 측 별도 land OR 영구 미land):

- Wine / CrossOver / Whisky / GPTK 측 actual download / install / config (Option B 측 advocate X, 분석 only)
- 외부 Windows VM 측 setup / install / config (Option C 측 narrative only, actual setup X)
- real D2R binary 측 actual execution / smoke test (Phase 3+ 측 영구 deferred, cycle posture inherit)
- 본 repo `lib/` / `tool/` / `native/` / `tests/` 측 modification (read-only reference)
- 기존 `.roadmap.*` family (14 file) 측 modification (read-only reference)
- 기존 `docs/*.md` 측 modification (read-only reference, Track H/Y/Z/AA/AN 측 cross-link 측 pointer-only)
- 기존 `state/markers/*.marker` 측 modification (read-only reference)

본 doc 은 design-only spec freeze — implementation 측 사용자 lock-in 후 별도 cycle 측 결정 (or 영구 미land, Option C 측 fallback case).

---

## §1 Phase 3 production smoke gate 정의 + 현재 상태

### §1.1 production smoke gate 측 정의

본 repo (airgenome-gamebox) 측 plugin 측 production-grade release 측 다음 3 layer 측 evidence 측 누적 후 도달:

1. **schema-validated emit** (Phase 1, 현재 활성 — closure_pct 29.55%, validated_manjeom 0): hexa file 측 schema validation (python3 json.loads + sha256) + skeleton-tier function 측 source presence + cond/blk evidence row 측 cross-cite. **현재 cycle posture**.
2. **runtime smoke (synthetic round-trip)** (Phase 2, partial — Track Y decision-only pending, hexa CLI runtime build path lock-in 의존): hexa file 측 actual self_test() invocation + tool/roadmap_op.hexa 측 actual subcmd dispatch + tests/test_*.hexa 측 actual round-trip.
3. **production smoke (real binary execution path)** (Phase 3+, 영구 deferred, **본 doc 측 design 측 대상**): real game binary (D2R.exe / Lineage W / etc) 측 actual launch + first frame emit + L2 Warden / GameGuard / etc 측 production environment 측 sustain.

### §1.2 현재 측정 상태 (Track AN aggregate reflect)

- **closure_pct**: 29.55% (Track AG weighted-aware aggregation, 2026-05-04 시점 — `docs/SESSION_2026_05_03_TO_05_04_AGGREGATE.md` § 3.2 직접 reference)
- **validated_manjeom**: 0 (production smoke 측 측정 미수행 — Phase 3+ deferred 측 직접 결과)
- **Win32 syscall coverage**: cycle 43 시점 218/435 (~50.11%) skeleton-tier (Track AM bundled 측 50% threshold breach milestone) — production smoke 측 actual binary execution 측 ~80%+ Win32 fn coverage 측 minimum-viable 추정 (real D2R binary 측 syscall surface 측 measurement 미수행, ~80% 측 conservative 추정)
- **D4 / lineage_classic_r28 / cs2 / delta_force / elden_ring**: 영구 retired_intentional (Battle.net AC L2 / GameGuard kernel-mode / Steam 의존 / VAC / EAC / kernel-AC) — 본 repo production smoke 측 영구 차단
- **Lineage M Android emul layer**: permanent boundary — Apple-native stack 측 Android runtime emulation 측 본 repo design intent 측 미정합

### §1.3 horizon estimate (per-game)

- **D2R offline single-player**: months-1yr+ horizon — Win32 syscall coverage 50.11% → ~80%+ 측 cumulative skeleton + real binary 측 cross-validation cycle 측 multi-cycle 측 누적
- **D2R online (realm)**: 1-2yr+ horizon — Warden L2 측 natural environment alignment 측 long-horizon (offline path 측 dependent + Warden surface 측 separate horizon)
- **gamebox closure 측 multi-year horizon**: closure_pct 29.55% → 80%+ 측 production smoke threshold 측 multi-year (Track AG weighted-aware progression 측 cycle-by-cycle ~1-2pct 측 horizon)
- **D4 / lineage_classic_r28 / cs2 / delta_force / elden_ring**: 영구 retired_intentional (horizon X)
- **Lineage M / Lineage W**: Android emul layer / UE4 dual renderer 측 horizon ≥ 2yr+

### §1.4 본 cycle 측 결정 측 의미

본 cycle 측 production smoke gate 측 도달 path 측 design-level 측 3-way option 측 explicit 정리. 사용자 측 A / B / C 측 명시적 lock-in 후 implementation cycle 측 결정 (or Option C 측 영구 fallback case 측 implementation 측 X).

---

## §2 Option A — own1 maintained (status quo, multi-year horizon)

### §2.1 정의

본 repo cycle posture 측 그대로 유지 — own1 invariant (Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / 외부 게임 runtime 0) 측 strict 정합. real binary 측 actual execution path 측 다음 측 cumulative skeleton 측 누적 측 도달:

- D2R offline: Win32 syscall ~80%+ + Battle.net launcher offline shim + MPQ archive + d2s round-trip 측 multi-cycle 측 누적 → real binary 측 cross-validation cycle 측 land
- D2R online: offline path 측 dependent + Warden L2 측 natural environment alignment cycle 측 별도 long-horizon
- 다른 게임: retired_intentional 측 영구 (D4/L Classic R28/CS2/Delta Force/Elden Ring) OR Android/UE4 측 long-horizon (Lineage M/W)

### §2.2 Pros

- **own1 invariant 측 strict 정합** — 본 repo design intent 측 정의 측 가장 정합 (Wine 0 / 외부 game runtime 0 측 Apple-native stack 측 plugin scope intact)
- **plugin scope intact** — 본 repo plugin distribution 측 외부 dependency 측 추가 0 (사용자 측 plugin install 시 추가 prerequisite X)
- **ToS 정합** — 외부 wrapper 측 도입 X 측 BattleNet ToS / Warden ToS / 측 직접 정합 (CrossOver detection ban risk X)
- **closure_pct progression 측 자연스러운 trail 측 정합** — Track AG weighted-aware aggregation 측 closure_pct 29.55% → 80%+ 측 cycle-by-cycle 측 measurable progression
- **fallback 측 well-defined** — Option C 측 영구 fallback 측 명시 (closure_pct 측 영구 80% 미만 측 case 측 사용자 측 외부 VM 측 별도 환경 측 production smoke 측 redirect 가능)

### §2.3 Cons

- **multi-year horizon (months-1yr+ for D2R offline, 1-2yr+ for D2R online, multi-year for closure)** — 사용자 측 short-term real binary execution 측 expectation 측 mismatch
- **Win32 syscall coverage gap** — 50.11% (cycle 43) → ~80%+ 측 ~30pct (130 fn) 측 cumulative skeleton 측 multi-cycle horizon
- **D4 / Lineage Classic R28 / cs2 / delta_force / elden_ring 측 영구 retired_intentional** — multi-game coverage 측 본질적 제약 (Battle.net D4 AC / GameGuard kernel / VAC / EAC / kernel-AC)
- **Lineage M Android emul layer permanent boundary** — Apple-native stack 측 Android runtime emulation 측 design intent 측 미정합 측 Lineage M 측 영구 미land
- **runtime_smoke_capability 측 horizon-deferred** — Track Y decision-only pending lock-in 측 hexa CLI runtime path 측 별도 lock-in 측 dependent (본 doc 측 production smoke 측 Track Y runtime smoke 측 dependent 측 chained horizon)

### §2.4 own1 / own2 verdict

- **own1**: PASS (strict 정합)
- **own2**: PASS (honest horizon disclosure — multi-year horizon 측 사용자 lock-in 시 명시 ack 의무)

---

## §3 Option B — Wine 정책 재정의 (own1 break, advocate X — honest 분석)

### §3.1 정의

본 repo own1 invariant 측 재정의 — Wine 0 → Wine N (or CrossOver / Whisky / GPTK 측 도입). real game binary 측 actual execution 측 즉시 가능 측 path. 단 own1 break + ToS / ban risk 측 본질적 trade-off.

candidate wrapper:
- **Wine** (open-source, free) — Linux primary, macOS port (community)
- **CrossOver** (CodeWeavers commercial) — D2R 측 historical support, Battle.net launcher 측 historical compat
- **Whisky** (open-source, modern macOS GPTK frontend) — Apple Game Porting Toolkit 측 frontend
- **Apple GPTK** (Game Porting Toolkit, Apple official, dev-only license) — D3D12 → Metal translation, x86_64 → arm64 emulation

### §3.2 Pros

- **즉시 real binary 측 execution 가능** — Wine / CrossOver 측 D2R 측 historical support evidence 측 누적 (years horizon)
- **multi-game coverage 측 즉시 expansion** — D2R + (D4 측 Battle.net launcher 측 가능 측 case) + 다른 Win32 game 측 즉시 launch 가능 (단 AC / Warden 측 별도 risk)
- **runtime_smoke_capability 측 즉시 unlock** — Win32 syscall coverage gap 측 본 repo skeleton-tier 측 의존 X (Wine 측 own implementation 측 의존)
- **horizon 측 dramatic 단축** — multi-year → days-weeks (CrossOver / Whisky 측 install + config 측 하루 cycle)

### §3.3 Cons

- **own1 invariant 측 break** — 본 repo 측 정의 측 가장 critical violation. plugin scope 측 본질적 redefine 의미 (Wine 0 → Wine N).
- **plugin scope 측 redefinition 의무** — 본 repo plugin distribution 측 prerequisite 측 Wine / CrossOver 측 install 측 명시 의무 (사용자 plugin install 시 추가 step + license issue 측 CrossOver 측 commercial)
- **ToS / ban risk** — Warden 측 CrossOver detection history evidence 측 누적 (community report). Battle.net ToS 측 third-party launcher 측 prohibition 측 grey area. ban risk 측 사용자 측 자산 (D2R license) 측 직접 risk
- **Apple GPTK 측 dev-only license** — production-grade plugin distribution 측 license incompatibility (Apple GPTK 측 EULA 측 production redistribution 측 prohibit)
- **Warden L2 detection 측 historical evidence** — CrossOver detection 측 community report 측 누적 (Reddit / forum / Battle.net ToS report) — production smoke 측 sustain 측 본질적 risk
- **drift risk** — Wine / CrossOver / Whisky / GPTK 측 upstream version 측 cycle-by-cycle 측 sync drift + version pin burden

### §3.4 own1 / own2 verdict

- **own1**: FAIL (strict reading 측 violation 측 explicit). own1 redefinition 측 사용자 측 명시 lock-in + 본 repo 측 own1 spec 측 update 측 의무 (caveat C5).
- **own2**: PASS conditional (honest disclosure 측 own1 break 측 explicit + ban risk + ToS / EULA risk 측 explicit). 단 own1 = FAIL 측 chained → 본 doc 측 권고 X.

### §3.5 Honest disclosure (advocate X)

본 § 측 Option B 측 honest 분석 — advocate X. own1 break 측 critical risk + ban risk + ToS / EULA / license incompatibility 측 cumulative. 사용자 측 명시적 own1 redefinition + ban risk acknowledge 측 lock-in 시 redirect 가능 단 본 doc 측 권고 X.

---

## §4 Option C — 외부 Windows VM / 별도 환경 (gamebox-purpose-misalignment)

### §4.1 정의

Mac 측 Windows VM (Parallels Desktop / VMware Fusion / UTM / VirtualBox 등) 측 별도 환경 측 real game binary 측 actual execution. 본 repo (airgenome-gamebox) 측 plugin 측 무관 — 사용자 측 별도 Windows host 측 game binary 측 production smoke 측 redirect.

candidate VM:
- **Parallels Desktop** (commercial, native arm64 on Apple silicon, Windows 11 ARM)
- **VMware Fusion** (commercial → free for personal, native arm64)
- **UTM** (open-source, QEMU frontend, Windows 11 ARM 측 가능)
- **VirtualBox** (open-source, x86_64 only — Apple silicon 측 미정합)

### §4.2 Pros

- **own1 정책 유지** — gamebox 측 외부 도구 측 game binary 측 production smoke 측 redirect 측 본 repo 측 own1 (Wine 0 / 외부 game runtime 0) 측 strict 정합 (gamebox 측 plugin 측 외부 wrapper 측 미도입)
- **ban risk 0** — Windows VM 측 native Windows environment 측 Battle.net / Warden / GameGuard 측 정상 detection 측 ban risk 측 wrapper 측 의존 측 없음 (단 GeForce Now / Stadia / Cloud gaming 측 별도 ToS issue 측 본 § 측 무관)
- **ToS 정합** — native Windows host 측 game binary 측 정상 launch 측 ToS 측 직접 정합
- **즉시 production smoke 측 capability** — Windows VM 측 setup 후 real D2R binary 측 즉시 launch + first frame + 실제 play 측 가능
- **multi-game coverage 측 expansion** — D2R + D4 + cs2 + delta_force + elden_ring + Lineage 측 모두 native Windows host 측 즉시 launch 가능 (단 retired_intentional posture 측 본 repo 측 정의 측 별도)
- **simple + low-risk** — 외부 VM 측 native Windows 측 most well-tested path (years horizon evidence 누적)

### §4.3 Cons

- **gamebox 측 design intent 측 미충족** — 본 repo (airgenome-gamebox) 측 정의 측 "Apple-native stack 측 plugin" — Windows VM 측 redirect 측 본 repo 측 의의 측 본질적 mismatch
- **gamebox-purpose-alignment 측 본질적 break** — Mac 측 Apple silicon 측 native run 측 design goal 측 Windows VM 측 redirect 측 정의 미충족
- **plugin scope 측 무관** — 외부 VM 측 본 repo plugin 측 외부 — production smoke gate 측 본 repo plugin 측 evidence 측 0건 (validated_manjeom 측 본 repo plugin 측 무관 측 외부 VM 측 land)
- **사용자 측 추가 비용** — Parallels / VMware 측 commercial license + Windows 11 ARM license + VM 측 disk space (50-100GB+) + RAM (8GB+ allocate)
- **arm64 emulation overhead** — Windows 11 ARM 측 x86_64 game binary 측 native arm64 emulation 측 performance overhead (D2R 측 native arm64 binary 측 absent — Windows ARM 측 x86_64 측 emulate)
- **gamebox plugin 측 production smoke 측 영구 미land** — Option C 측 fallback case 측 본 repo plugin 측 production-grade release 측 영구 not 가능 (plugin 측 의의 측 redirect)

### §4.4 own1 / own2 verdict

- **own1**: PASS (gamebox 측 외부 도구 측 redirect 측 본 repo 측 own1 strict 정합)
- **own2**: PASS conditional (honest disclosure 측 gamebox 측 design intent 측 미충족 측 explicit ack 측 사용자 lock-in 의무)

### §4.5 Fallback 측 의의

Option C 측 본 doc 측 권고 X 단 **fallback 측 well-defined** — Option A 측 multi-year horizon 측 사용자 측 short-term real binary 측 expectation 측 mismatch 시 외부 VM 측 redirect 측 본 repo 측 own1 정합 + ban risk 0 + 즉시 capability 측 cumulative trade-off 측 Option B 측 own1 break 측 vs 측 명확 우위.

---

## §5 결정 매트릭스 (low=1 / med=3 / high=5; 점수 낮을수록 정합, 11 criteria mirror Track H/Y)

> **scoring direction**: 본 매트릭스 측 모든 criteria 측 **낮을수록 better**. own1_alignment = 1 (full PASS) / 3 (partial / disambiguation 필요) / 5 (FAIL / break). horizon_to_first_frame = 1 (즉시) / 3 (medium) / 5 (multi-year). 명료성 측 매 row 측 "good direction" column 명시.

| criterion | A: own1-maintained | B: Wine 재정의 | C: 외부 VM | good direction | 비고 |
|---|---|---|---|---|---|
| **own1_alignment** (Wine 0 / 외부 game runtime 0) | 1 (low risk = full PASS) | 5 (high risk = FAIL break) | 1 (low risk = gamebox plugin 측 외부 redirect 측 strict) | 낮을수록 better | A 측 strict; B 측 own1 invariant 측 explicit break; C 측 gamebox 측 외부 도구 측 redirect 측 strict |
| **own2_alignment** (honest emit + verify) | 1 (low) | 3 (med — own1 break disclosure 의무 + ban risk + ToS) | 3 (med — gamebox-purpose-alignment 측 미충족 disclosure 의무) | 낮을수록 better | A 측 honest horizon disclosure; B 측 own1 break + ban risk + ToS explicit 의무; C 측 design intent 미충족 explicit 의무 |
| **horizon_to_first_frame** (real D2R binary first frame 측 도달 horizon) | 5 (high — months-1yr+ for D2R offline, multi-year for closure) | 1 (low — days-weeks, CrossOver install + config) | 1 (low — VM setup 1 cycle, immediate) | 낮을수록 better | A 측 multi-year; B 측 즉시; C 측 즉시 |
| **ban_risk** (BattleNet / Warden / etc 측 ban evidence) | 1 (low — own1 strict 정합 측 wrapper 측 detection X) | 5 (high — Warden CrossOver detection community evidence) | 1 (low — native Windows VM 측 ban risk X) | 낮을수록 better | B 측 single critical risk; A/C 측 risk 0 |
| **plugin_scope_intact** (본 repo plugin 측 외부 dependency 측 추가 0) | 1 (low — plugin scope 100% intact) | 5 (high — Wine prerequisite 추가 + license issue) | 1 (low — gamebox plugin 측 무관 측 외부 VM 측 redirect) | 낮을수록 better | A 측 plugin scope intact; B 측 plugin 측 prerequisite redefine; C 측 plugin scope 측 무관 (production smoke gate 측 외부 redirect) |
| **gamebox_purpose_alignment** (Apple-only stack 측 design intent) | 1 (low — design intent 정합) | 3 (med — Wine wrapper 측 Apple-native 측 partial alignment / partial break) | 5 (high — Mac 측 Windows VM 측 design intent 측 본질적 미충족) | 낮을수록 better | A 측 정합; B 측 partial; C 측 본질적 break |
| **ToS_alignment** (BattleNet ToS / Warden ToS / EULA / license) | 1 (low — own1 strict 측 ToS 측 직접 정합) | 5 (high — third-party launcher prohibition + GPTK dev-only license + CrossOver commercial) | 1 (low — native Windows VM 측 ToS 측 직접 정합) | 낮을수록 better | B 측 third-party launcher prohibition + GPTK production redistribution prohibit + CrossOver commercial license |
| **user_lock_in_required** (사용자 측 추가 ack / 추가 비용 / 추가 setup) | 3 (med — multi-year horizon ack 의무) | 5 (high — own1 redefinition + ban risk ack + license + install + config) | 5 (high — gamebox-purpose-alignment ack + VM commercial license + Windows 11 ARM license + VM setup) | 낮을수록 better | A 측 horizon ack 만; B 측 own1 redefinition + ban risk + license cumulative; C 측 design intent ack + VM commercial cost |
| **multi_game_coverage** (real binary 측 launch 가능 게임 수) | 5 (high — D2R offline only, D2R online long-horizon, D4/etc retired_intentional) | 1 (low — D2R + D4 + cs2 + Lineage 측 즉시) | 1 (low — D2R + D4 + cs2 + delta_force + elden_ring + Lineage 측 native Windows 측 모두 launch 가능) | 낮을수록 better | A 측 D2R offline only; B 측 multi-game; C 측 multi-game |
| **runtime_smoke_capability** (production smoke 측 actual execution path 측 unlock) | 5 (high risk = horizon-deferred multi-year) | 1 (low risk = immediate via Wine) | 1 (low risk = immediate via VM) | 낮을수록 better | A 측 multi-year deferred; B 측 즉시 via wrapper; C 측 즉시 via VM |
| **effort_dev_cost** (본 repo 측 implementation cost) | 3 (med — multi-cycle skeleton-tier 누적, but design 측 본 cycle 측 0) | 5 (high — own1 spec redefinition + plugin distribution prerequisite update + selftest extension + ban risk monitoring cycle 측 cumulative) | 1 (low — 본 repo 측 implementation 측 0건, 외부 VM 측 redirect 측 사용자 측 setup 만) | 낮을수록 better | A 측 multi-cycle skeleton-tier; B 측 own1 spec rewrite + selftest 측 cumulative; C 측 본 repo 측 0건 |
| **합계 (낮을수록 better, 11 criteria)** | **27** | **39** | **21** | — | C 측 최저 점수 단 gamebox_purpose_alignment = 5 (max) 측 disqualifying for "본 repo 측 의의" |

### §5.1 합계 해석 (단순 합계 측 misleading — disqualifying criterion 측 명시)

- **C 외부 VM = 21 (formal lowest)** 단 **gamebox_purpose_alignment = 5 (max)** = 본 repo (airgenome-gamebox) 측 design intent ("Apple-native stack 측 plugin") 측 본질적 미충족 측 disqualifying. 본 repo plugin 측 production-grade release 측 외부 VM 측 redirect 측 본 repo 측 의의 측 redirect 의미.
- **A own1-maintained = 27 (medium)** 단 **own1 + own2 + ban_risk + plugin_scope + gamebox_purpose + ToS + effort 측 모두 1-3** = 본 repo 측 design intent + own1 invariant 측 strict 정합. multi-year horizon 측 사용자 측 명시 lock-in ack 시 정합 회복 가능 (caveat C1).
- **B Wine 재정의 = 39 (highest formal)** = own1 break + ban risk + ToS + plugin scope + license + drift 측 cumulative. own1 strict reading 측 violation + Warden CrossOver detection community evidence + ToS prohibition 측 disqualifying.

→ 매트릭스 측 **단순 합계 측 misleading** — disqualifying criterion (C 측 gamebox_purpose_alignment = 5 / B 측 own1_alignment = 5 + ban_risk = 5) 측 weight 측 사용자 측 own1-strict-vs-pragmatic + design-intent-strict-vs-flexible stance 측 결정. 본 doc 측 **사용자 측 own1 strict 정합 + 본 repo design intent strict 정합 + multi-year horizon ack 측 lock-in 가정 측 권고 = A**.

### §5.2 매트릭스 측 비대칭 측 honest disclosure (own2 정합)

- **A own1-maintained 측 single weakness**: horizon_to_first_frame 5 + runtime_smoke_capability 5 + multi_game_coverage 5 — 3 criteria 측 max value. 합계 27 측 medium 단 본 repo design intent 측 alignment 측 strict 정합 측 사용자 lock-in 시 multi-year horizon ack 측 trade-off 정합 회복.
- **B Wine 재정의 측 multi-criteria critical risk**: own1_alignment 5 + ban_risk 5 + ToS_alignment 5 + plugin_scope_intact 5 + user_lock_in_required 5 + effort_dev_cost 5 — 6 criteria 측 max value. own1 break + ban risk + ToS + plugin scope + license + dev cost 측 cumulative disqualifying.
- **C 외부 VM 측 single weakness**: gamebox_purpose_alignment 5 + user_lock_in_required 5 — 2 criteria 측 max value. 합계 21 측 best (formal) 단 본 repo plugin 측 의의 측 외부 redirect 측 본질적 redefine.

---

## §6 권고 — Option A (own1 maintained, status quo, multi-year horizon ack), confidence MEDIUM

### §6.1 권고 측 근거

1. **own1 invariant 측 strict 정합** — 본 repo 측 design intent 측 정의 측 가장 정합 측 단일 path. own1 (Wine 0 / 외부 game runtime 0) 측 strict reading 측 본 repo plugin scope 측 정의 측 지속.
2. **ban risk 0 + ToS 측 직접 정합** — Option B 측 Warden CrossOver detection 측 community evidence + Battle.net ToS 측 third-party launcher prohibition 측 risk 측 회피. 사용자 자산 (game license) 측 보호.
3. **plugin scope 측 본 repo design intent 정합** — 본 repo plugin distribution 측 외부 prerequisite (Wine / CrossOver / Whisky / GPTK) 측 추가 X — 사용자 plugin install 측 simple.
4. **closure_pct progression 측 well-defined trail** — Track AG weighted-aware aggregation 측 closure_pct 29.55% → 80%+ 측 cycle-by-cycle 측 measurable progression. validated_manjeom 0 → 1+ 측 multi-cycle horizon 측 honest acknowledge.
5. **fallback 측 Option C 측 well-defined** — Option A 측 multi-year horizon 측 사용자 측 short-term expectation 측 mismatch 시 Option C (외부 VM) 측 redirect 측 본 repo own1 정합 + ban risk 0 측 cumulative.

### §6.2 confidence level — MEDIUM (not MEDIUM-HIGH or HIGH)

- **MEDIUM (not HIGH)** 이유:
  - **multi-year horizon 측 사용자 측 expectation 측 mismatch risk** — 사용자 측 short-term real binary execution 측 expectation 측 mismatch 측 lock-in 시 명시 ack 측 의무. ack 측 부재 시 사용자 측 frustration risk.
  - **Win32 syscall coverage gap (50.11% → ~80%+) 측 estimate 측 underdetermined** — ~80%+ 측 production smoke 측 minimum-viable 측 추정 단 actual D2R binary 측 syscall surface 측 measurement 미수행. real coverage threshold 측 사용자 측 명시 lock-in 측 의무.
  - **Track Y runtime smoke 측 chained dependent** — production smoke (Phase 3+) 측 runtime smoke (Phase 2, Track Y decision-only pending lock-in) 측 chained — Track Y 측 사용자 측 lock-in 측 본 doc 측 dependent (caveat C2).
  - **D4 / Lineage Classic R28 / cs2 / delta_force / elden_ring 측 영구 retired_intentional + Lineage M Android emul boundary** — multi-game coverage 측 제약 측 사용자 측 명시 ack 의무.
- **MEDIUM (not LOW)** 이유:
  - own1 invariant + design intent + ban risk + ToS 측 cumulative 측 strict 정합 — Option A 측 risk 측 manageable 측 horizon-only.

### §6.3 fallback 시나리오

- **A 측 multi-year horizon 측 사용자 측 expectation 측 mismatch 시** → **Option C (외부 VM)** 측 redirect. 본 repo plugin 측 의의 측 별도 redefine + 외부 VM 측 production smoke 측 land. 본 repo 측 plugin 측 schema-validated only posture 측 영구 (Phase 1 only).
- **A 측 D2R offline 측 1yr+ horizon 측 사용자 측 abandon 시** → **Option C** 측 redirect (동일).
- **B 측 own1 redefinition 측 사용자 측 명시 lock-in 시** — 본 doc 측 권고 X — 별도 design cycle 측 own1 spec rewrite + plugin distribution prerequisite update + ban risk monitoring cycle plan + ToS / EULA / license audit + selftest extension 측 cumulative cycle plan 측 land 측 prerequisite. 본 doc 측 advocate X.

### §6.4 권고 측 단정 측 회피 (own2 정합)

- 본 § 측 권고 측 사용자 측 final lock-in 측 대체 X — 매트릭스 + 권고 + confidence level + fallback + 사용자 lock-in checklist 측 제시 측 사용자 측 informed decision 측 enable 만.
- 본 cycle 측 **구현 X** (decision-only). real binary 측 actual execution 측 영구 deferred 측 honest disclosure.

---

## §7 사용자 lock-in 체크리스트 (decision pending, 11 items)

> **상태**: ⏳ awaiting user lock-in

다음 항목 측 사용자 측 명시적 lock-in 필요 — 본 cycle 후속 implementation cycle 시작 전 (or Option C fallback case 측 implementation 측 X 측 영구 cycle posture 측 lock-in):

- [ ] **option choice**: A (own1 maintained) / B (Wine 정책 재정의) / C (외부 VM) 측 1 개 선택 (default recommend: **A**)
- [ ] **(if A)**: multi-year horizon explicit acknowledge — D2R offline months-1yr+ / D2R online 1-2yr+ / closure 측 multi-year horizon 측 사용자 측 expectation 측 정합 ack
- [ ] **(if A)**: Win32 syscall coverage threshold lock-in — ~80%+ minimum-viable 측 estimate vs actual D2R binary syscall surface measurement 측 후속 cycle 측 결정 deferred ack
- [ ] **(if A)**: Track Y runtime smoke chained dependency ack — Phase 3+ production smoke 측 Phase 2 runtime smoke (Track Y decision-only pending) 측 chained dependent ack
- [ ] **(if A)**: retired_intentional + Lineage M Android emul boundary multi-game coverage 제약 ack — D4 / Lineage Classic R28 / cs2 / delta_force / elden_ring 측 영구 retired_intentional + Lineage M Android emul layer permanent boundary 측 production smoke 측 영구 차단 ack
- [ ] **(if B)**: own1 redefinition spec — Wine 0 → Wine N (or CrossOver / Whisky / GPTK 측 N) 측 명시 spec + 본 repo own1 spec doc 측 update path + plugin distribution prerequisite redefine
- [ ] **(if B)**: ban risk explicit acknowledge — Warden CrossOver detection community evidence + Battle.net ToS third-party launcher prohibition + 사용자 자산 (game license) 측 ban 시 직접 risk ack
- [ ] **(if B)**: ToS / EULA / license audit — Apple GPTK dev-only license + CrossOver commercial license + Wine GPL + Whisky open-source 측 본 repo plugin 측 redistribution 측 호환 audit 결과
- [ ] **(if C)**: gamebox-purpose-alignment ack — 본 repo 측 design intent ("Apple-native stack 측 plugin") 측 외부 VM 측 redirect 측 본질적 미충족 ack + plugin 측 production smoke 측 영구 redirect ack
- [ ] **(if C)**: VM commercial license + Windows 11 ARM license + VM setup 측 사용자 측 비용 ack
- [ ] **(any)**: ban risk acknowledge (any option) — Option A 측 ban risk 0 / Option B 측 high / Option C 측 ban risk 0 단 native Windows 측 ToS 측 직접 정합 ack
- [ ] **(any)**: ToS implications (any option) — 본 repo plugin 측 distribution 측 ToS / EULA / license 측 직접 정합 측 사용자 책임 ack
- [ ] **raw 168 minimum-viable acknowledge**: 본 cycle 측 design doc only — production smoke gate 측 implementation 측 0건 측 사용자 측 정합 ack
- [ ] **schema migrate 의무 X 측 명시 acknowledge**: 본 repo policy `migration: forbidden` 측 정합 — 본 doc 측 in-place modify 측 0 측 정합 ack

---

## §8 cross-link to predecessors (Track H/Y/Z/AA decision pattern + Track AN aggregate)

### §8.1 design-only pending lock-in pattern siblings (Track H/Y/Z/AA)

| track | doc | marker | relation |
|---|---|---|---|
| H | `docs/ROADMAP_OP_ADOPTION_DECISION.md` | `state/markers/airgenome_gamebox_roadmap_op_adoption_decision_landed.marker` | predecessor decision doc — 11 criteria 3-way matrix 직접 mirror, design-only lock-in pattern |
| Y | `docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` | `state/markers/airgenome_gamebox_hexa_cli_runtime_build_decision_landed.marker` | predecessor decision doc — recent 3-way runtime decision matrix sibling, Phase 2 runtime smoke 측 본 doc Phase 3+ chained dependent (caveat C2) |
| Z | `docs/CROSS_TOOL_INTEGRATION_DESIGN.md` | `state/markers/airgenome_gamebox_cross_tool_integration_design_landed.marker` | predecessor design-only cycle posture mirror — in_place_writes=0 cleanest cycle |
| AA | `docs/MK2_CHECKPOINT_SCHEMA_DESIGN.md` | `state/markers/airgenome_gamebox_mk2_checkpoint_schema_design_landed.marker` | predecessor design-only with lock-in checklist (11 items) — 본 doc 측 lock-in checklist 측 직접 mirror |
| AN | `docs/SESSION_2026_05_03_TO_05_04_AGGREGATE.md` | (aggregate doc, 본 cycle Track AN 측 marker 측 별도 separate) | current state reference — closure_pct 29.55%, validated_manjeom 0, Win32 50.11% 측 본 doc § 1.2 측 직접 reference |

### §8.2 cleanest cycle posture (in_place_writes=0) mirror Track I/L/T/Y/Z/AA/AN

본 cycle 측 in_place_writes=0 cleanest cycle posture 측 Track I/L/T/Y/Z/AA/AN 측 직접 mirror — 3 NEW file (design doc + handoff + marker) 만 추가, 기존 file 측 modification 0건.

### §8.3 production smoke gate caveat trail predecessors

본 doc 측 production smoke gate 측 영구 deferred Phase 3+ caveat 측 다음 markers 측 누적:

- `state/markers/airgenome_gamebox_d2r_realm_protocol_honest_freeze_landed.marker` — D2R realm protocol 측 production smoke 측 Warden L2 측 long-horizon 측 honest freeze
- `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker` — Warden L2 측 natural environment alignment posture
- `state/markers/airgenome_gamebox_gameguard_honest_freeze_landed.marker` — GameGuard kernel-mode 측 retired_intentional cascading
- `state/markers/airgenome_gamebox_perf_manjeom_aggregation_review_landed.marker` — manjeom aggregation 측 validated_manjeom 0 측 honest reflect
- `state/markers/airgenome_gamebox_closure_orchestrator_weighted_threshold_awareness_landed.marker` — closure_pct 25 → 29.55% Track AG weighted-aware aggregation
- `state/markers/airgenome_gamebox_win32_cycle40_43_bundled_increment_landed.marker` — Win32 50.11% threshold breach milestone (Track AM bundled)

→ 본 doc 측 land 측 위 markers 측 production smoke gate 측 영구 deferred caveat 측 **decision matrix 측 explicit 정리 단계 도달** (구현 X, 사용자 lock-in 대기).

---

## §9 honest scope acknowledgment (gamebox 측 design intent vs reality)

### §9.1 gamebox 측 design intent (정의)

본 repo (airgenome-gamebox) 측 정의 측 다음 4 항목 측 cumulative:

1. **Apple-native stack 측 plugin** — Mac (Apple silicon arm64) 측 native execution 측 game compatibility plugin
2. **own1 invariant** — Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / 외부 game runtime 0 — Apple-native primitives 만
3. **own2 invariant** — honest emit + log/error verify + silent_swallow 0
4. **closure_pct progression** — Track AG weighted-aware aggregation 측 cycle-by-cycle measurable progression toward 80%+ production threshold

### §9.2 reality (현재 측정)

- **closure_pct 29.55%** — 80%+ 측 ~50pct gap 측 multi-year horizon
- **validated_manjeom 0** — production smoke 측 측정 미수행
- **Win32 50.11%** — ~80%+ 측 ~30pct gap 측 multi-cycle horizon
- **D4 / Lineage Classic R28 / cs2 / delta_force / elden_ring 측 영구 retired_intentional** — multi-game coverage 측 본질적 제약
- **Lineage M Android emul layer permanent boundary** — Apple-native stack 측 Android runtime emulation 측 design intent 측 미정합

### §9.3 honest acknowledgment

- **gamebox 측 design intent + reality 측 gap** — Apple-native stack 측 strict 정합 측 multi-year horizon + multi-game 측 제약 측 cumulative
- **사용자 측 expectation 측 mismatch risk** — 사용자 측 short-term real binary execution 측 expectation 측 mismatch 측 본 doc 측 honest disclosure 의무
- **Option C 측 fallback 측 well-defined** — design intent 측 strict 정합 측 multi-year horizon 측 mismatch 시 외부 VM 측 redirect 측 honest path
- **Option B 측 own1 break 측 design intent 측 본질적 redefine** — own1 redefinition 측 본 repo 측 정의 측 본질적 update 측 사용자 측 명시 lock-in 측 의무 — 본 doc 측 advocate X (분석 only)

---

## §10 caveats (≥10 honest items)

**C1 — multi-year horizon 측 사용자 측 expectation mismatch risk (Option A)**: § 1.3 + § 6.2 — D2R offline months-1yr+ / D2R online 1-2yr+ / closure 측 multi-year horizon 측 사용자 측 short-term real binary execution 측 expectation 측 mismatch 측 lock-in 시 명시 ack 의무. ack 측 부재 시 사용자 측 frustration + 후속 cycle 측 redirect risk.

**C2 — Track Y runtime smoke chained dependent (Phase 2 ↔ Phase 3+)**: § 1.1 + § 6.2 — production smoke (Phase 3+) 측 runtime smoke (Phase 2, `docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` Track Y decision-only pending lock-in) 측 chained — Track Y 측 사용자 측 lock-in 측 본 doc 측 dependent. Track Y 측 lock-in 부재 시 본 doc Option A 측 implementation 측 시작 X.

**C3 — Win32 syscall coverage threshold (~80%+) 측 estimate underdetermined**: § 1.2 + § 2.5 — ~80%+ 측 production smoke 측 minimum-viable 측 conservative estimate. actual D2R binary 측 syscall surface 측 measurement 미수행. real coverage threshold 측 후속 cycle 측 별도 design (e.g., D2R binary 측 IAT scan + Win32 syscall enumerate cycle) 측 결정 deferred. 사용자 측 명시 lock-in 측 의무.

**C4 — closure_pct progression 측 cycle-by-cycle ~1-2pct estimate 측 underdetermined**: § 1.3 — closure_pct 29.55% → 80%+ 측 multi-year horizon 측 cycle-by-cycle ~1-2pct progression 측 Track AG 측 historical evidence 측 추정. 후속 cycle 측 progression rate 측 사용자 측 expectation 측 명시 lock-in 측 의무.

**C5 — Option B own1 redefinition 측 사용자 lock-in 측 explicit 의무**: § 3 + § 6.3 — Option B 측 own1 spec rewrite + plugin distribution prerequisite update + ban risk monitoring cycle plan + ToS / EULA / license audit + selftest extension 측 cumulative cycle plan 측 land 측 prerequisite. 사용자 측 명시 lock-in 부재 시 own1 invariant 측 silent break risk. 본 doc 측 advocate X (분석 only).

**C6 — Warden L2 detection 측 CrossOver community evidence 측 grey area**: § 3.3 — Warden 측 CrossOver detection 측 community report (Reddit / forum / Battle.net ToS report) 측 누적 단 official Battle.net 측 detection policy doc 측 absent. ban risk 측 grey area 측 사용자 자산 (game license) 측 직접 risk + 본 repo 측 lawful disclosure 측 의무.

**C7 — Apple GPTK dev-only license 측 production redistribution prohibit**: § 3.3 — Apple Game Porting Toolkit 측 EULA 측 dev-only license 측 production-grade plugin distribution 측 prohibit. Option B 측 GPTK 측 wrapper 측 도입 시 본 repo plugin 측 redistribution 측 license incompatibility 측 production-grade release 측 영구 차단.

**C8 — Option C VM 측 arm64 emulation overhead**: § 4.3 — Windows 11 ARM 측 x86_64 game binary 측 native arm64 emulation 측 performance overhead. D2R 측 native arm64 binary 측 absent — Windows ARM 측 x86_64 측 emulate. real binary 측 actual play 측 frame rate / latency 측 native Windows host 측 비교 측 degradation evidence 측 community report 측 누적 단 measurement 미수행.

**C9 — gamebox-purpose-alignment 측 본질적 redefine (Option C)**: § 4.3 + § 9.3 — Option C 측 본 repo 측 plugin 측 production smoke gate 측 외부 redirect — 본 repo plugin 측 의의 측 본질적 redefine. plugin 측 production-grade release 측 영구 미land 측 사용자 측 명시 ack 의무.

**C10 — D4 / Lineage Classic R28 / cs2 / delta_force / elden_ring 영구 retired_intentional + Lineage M Android emul boundary 측 multi-game coverage 측 제약**: § 1.2 + § 2.3 — Battle.net D4 AC / GameGuard kernel-mode / Steam (cs2) / EAC (delta_force) / kernel-AC (elden_ring) 측 cumulative 측 본 repo production smoke gate 측 영구 차단. Lineage M Android emul layer 측 Apple-native stack 측 design intent 측 미정합 측 영구 boundary. multi-game coverage 측 본질적 제약 측 사용자 lock-in 시 명시 ack 의무.

**C11 — own1 strict reading vs admin tool lens disambiguation**: § 2.4 + § 4.4 — own1 (Wine 0 / 외부 game runtime 0) 측 strict reading 측 본 doc Option A 측 정합. Option C 측 외부 VM 측 본 repo plugin 측 외부 — own1 측 plugin lens 측 strict 정합 (gamebox plugin 측 wrapper 측 미도입). Track H + Track Y + Track AA precedent 측 own1 disambiguation pattern 측 정합.

**C12 — design-only land**: 본 cycle 측 production smoke 측 implementation 0건 / `lib/` / `tool/` / `native/` / `tests/` 측 modification 0건 / `.roadmap.*` 측 modification 0건 / 기존 docs/*.md 측 modification 0건. 본 doc 측 design spec freeze, 실 코드 변경 X. 후속 implementation cycle 측 사용자 lock-in 후 land (or Option C fallback case 측 implementation X 측 영구 cycle posture 측 lock-in).

**C13 — raw compliance 매 cycle 명시 (raw 9/10/11/12/15/168/175)**: 본 cycle 측 design doc + handoff + marker = 3 file 추가 — production smoke implementation 측 emit 0건. raw 9 (audit-only no impl emit) + raw 10 (≥10 caveats) + raw 11 (snake_case) + raw 12 (silent_error_ban — 매 option explicit + 매 criteria explicit) + raw 15 (env_lazy — repo-relative paths) + raw 168 (decision-only minimum-viable exempt) + raw 175 (BR-NO-USER-VERBATIM, paraphrase only) 정합.

**C14 — rate-limit recovery 측 disk write priority**: 본 cycle 측 BG subagent 측 rate-limit recovery 시점 측 disk write 측 priority — report 측 deferred 가능. 본 doc + 후속 handoff doc + marker 측 disk land 측 cycle 측 success criteria. (Track H + Track Y + Track Z + Track AA precedent 동일.)

---

(decision doc end. § 0-10, 11 criteria scoring 3-way matrix, recommendation A confidence MEDIUM, lock-in 측 사용자 대기. predecessor Track H/Y/Z/AA design-only pending lock-in pattern + Track AN current state aggregate 측 closure 측 본 doc 측 land 측 단계 도달. real binary execution path 측 영구 deferred Phase 3+ caveat trail 측 explicit decision matrix 정리.)
