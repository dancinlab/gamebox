# Closure Orchestrator Design (mk1 narrative — Track S, 2026-05-03)

> **scope**: airgenome-gamebox `tool/closure_orchestrator.hexa` (Track S — closure_orchestrator rank B+C cycle) — closure progression tracker mk1 narrative spec. SSOT of orchestrator surface, current state snapshot, CM-20→CM-30 progression intent, cross-link.
> **own1**: Wine 0 / hexa-only / NO closure-advancement work (this is the TRACKER, not the closure work).
> **own2**: PASS/FAIL emit honest / silent_swallow 0건.
> **policy**: additive_only / migration forbidden / destructive 0 / cost 0 / mac-local / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land.
> **predecessor**: `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` (Track P — 6 BG-AGB rank B+C candidates UNBLOCKED gate).

---

## §0 목적 (purpose)

본 doc 의 목적은 다음 4 가지로 한정한다 — 그 외는 (예) closure 본체 advancement work / docs/CLOSURE_ROADMAP.md SSOT modification / lib/loader 신규 모듈 land — 영구 out-of-scope.

1. `tool/closure_orchestrator.hexa` (Track S, 신규 hexa-native CLI tool) 측 surface (status / emit / list 3 subcmd) 측 narrative SSOT 명시.
2. 현재 closure 진행 snapshot — closure_pct = **25%** (skeleton 6/6 × 25 score), CM-20/30 = 67% milestone 단위 (per docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md §0a + docs/CLOSURE_ROADMAP.md §1) — pin.
3. CM-20 → CM-30 progression path — game-install-dependent 영역 (CM-21~CM-30) 의 의존성 + 본 tracker 측 monitoring scope 명시.
4. own1 / own2 정합 + cross-link — Track P (battlenet_bypass own1 disambiguation) 측 6 BG-AGB rank B+C unblock gate 통과 후 본 cycle 측 closure_orchestrator candidate land confirm.

본 doc 은 mk1 narrative — code body 측 source-of-truth 는 `tool/closure_orchestrator.hexa` 모듈 자체 (526 LOC). 본 doc 은 그 narrative 부속 (반복 X, 인용 정합).

---

## §1 closure_pct 25% current state

### §1.1 dashboard snapshot (cycle 37)

| 영역 | 값 | 출처 |
|---|---|---|
| **closure_pct** | **25%** (skeleton 6/6 × 25 score) | docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md §0a |
| **cm_ratio** | **20/30** (CM-0~CM-20 done = 21/31 = 67% milestone) | docs/CLOSURE_ROADMAP.md §1 |
| **skeleton_manjeom** | **100** (cycle 37 milestone — Phase 1~6 모두 커버) | docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md §0a |
| **validated_manjeom** | **0** (실 게임 1프레임도 안 돔) | docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md §0a |
| **baseline_cycle** | **37** | docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md status line |

### §1.2 closure_pct 25% 의 의미

closure_pct 는 docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md 의 phase dashboard score (25 / 100). skeleton 6/6 phase coverage 를 25 점으로 환산 — gameplay validation 0 / install path 0 / launch path 0 단계 더 합쳐 100 점 만점 (closure full).

milestone 단위 cm_ratio (20/30 = 67%) 와 phase_dashboard 단위 closure_pct (25%) 는 서로 다른 axis 의 measure — 둘 다 동시 보고 (orchestrator status subcmd 측 모두 emit).

### §1.3 cycle 37 milestone landmark

cycle 37 = skeleton manjeom 100 첫 도달 (Phase 1~6 모두 module skeleton + self_test PASS). 이 시점부터 다음 차원 (stage1 → validated → done) 측 진행이 game-install-dependent 영역 으로 진입.

---

## §2 CM-20/30 → CM-30/30 progression path

### §2.1 done (CM-0..CM-20) — milestone-level skeleton 단계

CLOSURE_ROADMAP.md §5 측 21 stages 모두 milestone-level skeleton ✓ 완료. 본 tracker 측 list subcmd 측 ✅ marker 측 검출 (count_substr "✅ CM-").

### §2.2 planned (CM-21..CM-30) — game-install-dependent 영역

| CM | 단계 | dependency |
|---|---|---|
| CM-21 | M1 D2R baseline + 만점 18 real validation | game install + 1 frame draw |
| CM-22 | M8 Sprint 1 측정 + PSB-1 brainstorm + 채택 구현 | M1 baseline 후 |
| CM-23 | M11 Sprint 2 측정 + PSB-2 | Sprint 1 후 |
| CM-24 | M15 Sprint 3 측정 + PSB-3 | Sprint 2 후 |
| CM-25 | Phase 10 D4 첫 frame (D3D12 + DXR) | D4 install + Phase 5b stage2+ |
| CM-26 | M1 D4 baseline + 만점 18 D4 검증 | CM-25 후 |
| CM-27 | PSB-4 (D4-input) | CM-26 후 |
| CM-28 | PSB-5+ 반복 (새 만점 0 × 3 sprint) | CM-27 후 |
| CM-29 | EXHAUSTION 인정 | docs/EXHAUSTION_<date>.md |
| CM-30 | closure — final commit + push | github.com/dancinlife/airgenome-gamebox |

### §2.3 본 cycle 측 closure 본체 advancement 0건

본 cycle (Track S) 은 **TRACKER** 만 land — closure_pct 측 advance 0건 / CM-21+ 측 work 0건. 그 의도는 다음 cycle 측 closure 본체 work 측 progress emit hook 정합 + monitoring scope mk1 narrative pin.

---

## §3 orchestrator tool surface

### §3.1 path + 모듈 identity

- path: `tool/closure_orchestrator.hexa`
- LOC: 526 (350-600 range 정합)
- import: `use "../lib/perf/r0_common"` (own2 r0_emit + r0_session_set 정합)
- emit prefix: `__CLOSURE__ <PASS|FAIL> <subcmd> <reason>` (mirror Track M roadmap_op `__ROADMAP_OP__` style)
- exit codes: 0 (PASS) / 1 (FAIL) — silent_error_ban (raw 12)

### §3.2 subcommand surface (3 — 모두 read-only/additive)

| subcmd | I/O kind | description |
|---|---|---|
| `status` | read-only print | closure_pct + cm_ratio + skeleton_manjeom + validated_manjeom + snapshot_source emit. SSOT scrape (CLOSURE_ROADMAP.md + D2R_D4_INSTALL_LAUNCH_ROADMAP.md) — fallback baseline cycle 37 if SSOT unreadable. |
| `list` | read-only print | CM-0..CM-30 milestone ladder (31 stages) 측 status (done / planned / done(baseline)) + label tabular emit. |
| `emit` | additive write | `state/markers/closure_progress_<unix_ts>.marker` 신규 파일 1개 land — JSON body schema `airgenome-gamebox/markers/closure_progress/1`. additive_only — 기존 marker overwrite 0건 (same-second collision guard PRESENT). |

### §3.3 fallback baseline (cycle 37 hardcoded)

SSOT scrape 실패 시 (CLOSURE_ROADMAP.md / D2R_D4_INSTALL_LAUNCH_ROADMAP.md unreadable) hardcoded baseline 사용:

- `BASELINE_CLOSURE_PCT = 25`
- `BASELINE_CM_DONE = 20`
- `BASELINE_CM_TOTAL = 30`
- `BASELINE_SKELETON_MANJEOM = 100`
- `BASELINE_VALIDATED_MANJEOM = 0`
- `BASELINE_CYCLE = 37`

본 baseline 은 **cycle 37 시점 pin** — 다음 cycle 측 SSOT update 시 본 baseline 도 별도 cycle 측 in-place edit 권고 (본 cycle 측 본 baseline 측 land 시점 SSOT 와 동기). source_kind label 측 `baseline_cycle37_pinned` / `ssot_partial_baseline_filled` / `ssot_full` 3-tier emit — silent_swallow 0건.

### §3.4 schema — closure_progress marker JSON body

```json
{
  "schema": "airgenome-gamebox/markers/closure_progress/1",
  "completion_iso": "2026-05-03T00:00:00Z",
  "emit_unix_ts": <int>,
  "emitter": "tool/closure_orchestrator.hexa subcmd_emit",
  "emitter_version": "0.1.0-track-s-rankbc",
  "closure_pct": <int 0..100>,
  "cm_done_index": <int 0..30>,
  "cm_total": 30,
  "cm_ratio": "<int>/<int>",
  "skeleton_manjeom": <int>,
  "validated_manjeom": <int>,
  "baseline_cycle": <int>,
  "snapshot_source": "<baseline_cycle37_pinned|ssot_partial_baseline_filled|ssot_full>",
  "ssot_closure_roadmap": "docs/CLOSURE_ROADMAP.md",
  "ssot_d2r_d4_roadmap": "docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md",
  "policy": { "migration": "forbidden", "changes": "additive_only", "destructive_ops": 0, "cost_usd": 0, "substrate": "mac-local", "closure_advancement": false },
  "own1": "Wine 0 / hexa-only / NO closure-advancement (tracker only)",
  "own2": "PASS/FAIL emit honest / silent_swallow 0",
  "raw_compliance": [9, 11, 12, 15, 175]
}
```

### §3.5 self_test (5 steps)

S1 status PASS / S2 list PASS / S3 resolve_snapshot range check (pct ∈ [0,100], cm_done ∈ [0,30]) / S4 build_progress_marker shape (schema/closure_pct/cm_ratio fields present) / S5 cm_label coverage (31 labels CM-0..CM-30 모두 non-empty + non-"(unknown..."). all 5 steps PASS 시 honest emit.

---

## §4 own1 / own2 alignment

### §4.1 own1 (Wine 0 / hexa-only / closure-advancement X)

- 본 tool 은 hexa-native (`use "../lib/perf/r0_common"` 만 import — Wine / CrossOver / Whisky / GPTK / community helper 0줄)
- 본 cycle 측 closure 본체 advancement 0건 — TRACKER 만 land
- own1 정합 verdict (본 cycle 자체 module): **PASS**

### §4.2 own2 (honest emit / silent_swallow 0)

- 모든 subcmd 측 `__CLOSURE__ <PASS|FAIL> <subcmd> <reason>` emit (mirror roadmap_op)
- exit code 0/1 (raw 12 silent_error_ban)
- self_test() 5 step 측 명시적 FAIL emit + early return
- r0_session_set + r0_emit (r0_common 측 own2 baseline) 측 정합
- own2 정합 verdict: **PASS**

### §4.3 Track P 측 6 BG-AGB rank B+C unblock 통과 confirm

Track P (battlenet_bypass own1 disambiguation, 2026-05-03) 측 6 BG-AGB rank B+C candidates 측 unblock confirmation:

- loader_native_helper (rank B) — unblocked
- perf_manjeom (rank B) — unblocked
- perf_game_d2r (rank B) — unblocked
- perf_game_d4 (rank C) — unblocked
- **closure_orchestrator (rank C) — unblocked** ← 본 cycle 측 land target
- battlenet_bypass (self) — unblocked

본 cycle = closure_orchestrator candidate 측 own1 review gate 통과 후 first land — 다른 5 candidate 는 별도 cycle.

---

## §5 cross-link

### §5.1 SSOT 의존 docs (read-only reference)

- `docs/CLOSURE_ROADMAP.md` — 508 LOC, §5 CM-0..CM-30 milestone table SSOT
- `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` — closure_pct dashboard reference (§0a, status line cycle 37)
- `docs/CLOSURE_FINAL_STATE.md` — closure final state 정의
- `docs/CLOSURE_100_VERIFICATION_PLAN.md` — closure 100% verification plan
- `docs/CLOSURE_PATH_B_VERIFICATION.md` — closure path B verification

### §5.2 sister tool (style mirror)

- `tool/roadmap_op.hexa` — Track M, 1626 LOC (full 11/11 anima parity), `.roadmap.<name>` mk2 CLI dispatcher. emit prefix `__ROADMAP_OP__`, exit code 0/1, atomic_write tmp+mv pattern, `print_help()` + `self_test()` 측 surface 측 본 cycle 측 mirror.

### §5.3 predecessor cycle markers (다른 BG track read-only)

- `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` — Track P (6 BG-AGB rank B+C unblock gate)
- `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` — 11 .roadmap.<game> fan-out (next_cycle_actions 측 closure_orchestrator 명시)
- `state/markers/airgenome_gamebox_roadmap_op_full_11_of_11_landed.marker` — Track M tool style origin

---

## §6 caveats (raw 10 honest C3 — 8 items minimum)

1. **C1 — closure_pct 25% 측 measure axis**: closure_pct (25%) 와 cm_ratio (20/30=67%) 는 서로 다른 axis. closure_pct 는 phase dashboard score, cm_ratio 는 milestone count. 둘 다 honest emit — 사용자 측 두 measure 를 단일 score 로 평균화 하지 말 것.
2. **C2 — baseline cycle 37 pin staleness risk**: hardcoded baseline (closure_pct=25, cm_done=20) 은 cycle 37 시점 pin. 다음 cycle 측 SSOT update 시 본 baseline 도 별도 cycle 측 sync 필요 — 본 tool 자체 측 자동 update 없음 (additive_only / migration forbidden 정합).
3. **C3 — SSOT scrape 측 정확도 한계**: ssot_count_cm_done 측 단순 substring count ("✅ CM-") — 다른 doc 측 동일 patterns 우연 발생 시 over-count 가능. CLOSURE_ROADMAP.md 측 단일 occurrence 영역 (§5 table) 측 한정 — 본 doc 측 다른 영역 측 ✅ CM- prefix 우연 발생 시 silent over-count risk (mitigation: 다음 cycle 측 더 엄격한 line-anchored regex 측 transition 권고).
4. **C4 — game-install dependency**: CM-21~CM-30 모두 game-install-dependent (D2R + D4 실 설치 + 실 frame draw). 본 tool 은 game state 측 probe 0건 — closure 본체 advancement 측 본 tool 측 측 contribute 0건.
5. **C5 — `emit` subcmd same-second collision guard**: state/markers/closure_progress_<stamp>.marker 측 stamp = unix sec. 같은 초 측 두 번 emit 시 두 번째 호출 측 FAIL emit (`marker_already_exists`) — additive_only 정합 (overwrite X).
6. **C6 — closure-advancement 0건 lock-in**: 본 cycle 측 마커 schema (`airgenome-gamebox/markers/closure_progress/1`) `policy.closure_advancement: false` 명시 — 본 tool 측 emit 측 어떤 marker 도 closure_pct advance 0건. 사용자 측 closure 본체 work 시 별도 cycle 측 별도 marker (closure 본체 advance 마커) 측 분리 권고.
7. **C7 — hexa CLI runtime emit 측 schema_validated_only**: 본 cycle 측 hexa CLI runtime 측 본 tool 측 실행 검증 0건 (predecessor cycle posture — cycle 37 baseline 측 hexa CLI 가용 가정 X). schema-level + LOC count + sha256 pin 까지만 본 cycle 측 verify.
8. **C8 — own1 verdict 본 cycle 자체**: 본 cycle 측 module 자체 측 own1 audit (Wine 0 / community helper 0 / hexa-only) verdict = **PASS** (line-by-line 검토 — `use "../lib/perf/r0_common"` 만 import, 다른 외부 의존 0건). 다음 cycle 측 module 별도 audit 권고 (Track P 측 sister cluster precedent 정합).
9. **C9 — `r0_common` import 측 transitive dep**: `use "../lib/perf/r0_common"` 측 r0_common.hexa 측 자체 own2 baseline (HOME / GAMEBOX_HOME / R0_HOME 측 env path resolve) — 본 tool 측 직접 의존 영역 X. r0_emit 측 emit-only 영역 사용 (silent_swallow 0).
10. **C10 — BR-NO-USER-VERBATIM lock-in**: 본 doc 측 raw 175 정합 — 사용자 verbatim quote 0건. 모든 narrative 는 paraphrase only.

---

## §7 출처 (sources)

| 출처 | 영역 | 경로 |
|---|---|---|
| docs/CLOSURE_ROADMAP.md §5 | CM-0..CM-30 milestone table SSOT | `docs/CLOSURE_ROADMAP.md` |
| docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md §0a | closure_pct dashboard + skeleton manjeom 100 (cycle 37) | `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` |
| state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker | Track P — 6 BG-AGB rank B+C unblock gate (closure_orchestrator candidate UNBLOCKED) | `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` |
| state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker §next_cycle_actions | 6 BG-AGB candidates 측 closure_orchestrator mention | `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` |
| tool/roadmap_op.hexa | Track M sister tool (style mirror — emit prefix / atomic_write / self_test) | `tool/roadmap_op.hexa` |

---

*written 2026-05-03 (Track S — closure_orchestrator rank B+C cycle, mk1 narrative).*
*own1 Wine 0 / hexa-only / closure-advancement X — TRACKER only.*
*own2 PASS/FAIL emit honest / silent_swallow 0건.*
*additive_only / migration forbidden / destructive 0 / cost 0 / mac-local / cap 90min.*
