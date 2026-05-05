# Cross-tool integration design (mk1 narrative — Track Z, 2026-05-04)

> **scope**: airgenome-gamebox 측 3 independent tool/perf module (`tool/roadmap_op.hexa` Track I+M / `tool/closure_orchestrator.hexa` Track S / `lib/perf/c_bnet_warden_environment_compat.hexa` Track J) cross-tool integration design — aggregate report, cross-reference surface, unified emit envelope mk1 narrative spec freeze.
> **own1**: Wine 0 / hexa-only / external orchestrator (make / just / bash) 0 — integration 측 hexa-native only.
> **own2**: design honest — 현재 emit format 측 working surface 와 extension 가능 surface 명시 (silent_swallow 0).
> **policy**: additive_only / migration forbidden / destructive 0 / cost 0 / mac-local / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land / **design-only — implementation 0건**.
> **predecessor**: `state/markers/airgenome_gamebox_perf_manjeom_aggregation_review_landed.marker` (Track T — perf_manjeom aggregation review, cleanest cycle posture mirror Track I/L/T/Y).

---

## §0 목적 / 한계 (purpose / scope freeze)

### §0.1 목적 (in-scope)

본 doc 의 목적은 다음 5 항목으로 한정된다:

1. 3 independent tool/perf module 측 cross-cutting integration **surface 측 narrative SSOT 측 freeze** — 어떤 cross-reference / aggregate / unified emit pattern 이 land 가능한가.
2. integration 측 3 cross-cutting cross-reference (A — roadmap_op verify + closure_orchestrator status aggregate / B — roadmap_op show .roadmap.battlenet + warden self_test cross-cite / C — closure_orchestrator emit + Track A/G/N PARTIAL emit aggregation) 측 surface enumerate.
3. emit format unification proposal — common envelope `__<TOOL>__ <verdict> <subcmd> <reason> [<cross_ref>]` (backward-compatible, cross_ref 측 OPTIONAL extension).
4. proposed `tool/integration_report.hexa` (NOT impl in this cycle) 측 surface 측 spec — 3 tool 측 driver + unified TSV / markdown aggregate emit.
5. own1 / own2 alignment — integration 측 hexa-native only / 외부 orchestrator (make / just / bash) 측 0줄 / additive at protocol level (existing emit format extended, NEVER replaced).

### §0.2 한계 (out-of-scope, 영구)

본 cycle 측 다음 항목은 **영구 out-of-scope**:

- `tool/integration_report.hexa` 측 implementation (design-only — 별도 cycle 측 land)
- `tool/roadmap_op.hexa` / `tool/closure_orchestrator.hexa` / `lib/perf/c_bnet_warden_environment_compat.hexa` 측 modification (read-only reference for emit format only)
- 기존 `__ROADMAP_OP__` / `__CLOSURE__` / `__BNET_WARDEN__` emit format 측 in-place modification — 본 design 은 **OPTIONAL extension field** 만 제안 (backward-compatible).
- `.roadmap.*` family file 측 modification
- 기존 `docs/*.md` 측 modification (read-only reference)
- 외부 orchestrator (make / just / bash / shell pipeline) 측 도입 — own1 정합 hexa-native only.

본 doc 은 mk1 narrative spec freeze — 후속 implementation cycle 측 본 doc 측 spec 측 정합 land.

---

## §1 current state — 3 independent tools / perf modules

### §1.1 inventory

| # | path | track | LOC | emit prefix | exit codes | scope |
|---|---|---|---|---|---|---|
| 1 | `tool/roadmap_op.hexa` | I + M | 1626 | `__ROADMAP_OP__` | 0/1 | 14 mk2 `.roadmap.<name>` JSONL header file CLI dispatcher (11/11 anima parity — add / update / remove / list / show / link / domains / verify / render / selftest / track-new) |
| 2 | `tool/closure_orchestrator.hexa` | S | 526 | `__CLOSURE__` | 0/1 | closure_pct + CM-N/30 progression tracker (3 subcmd — status / list / emit) |
| 3 | `lib/perf/c_bnet_warden_environment_compat.hexa` | J | 195 | `__BNET_WARDEN__` | n/a (perf module) | Warden user-mode AC honest detection-vector tracker (7 vectors, NOT bypass) |

### §1.2 emit format current state (per-tool)

각 tool / module 측 honest emit baseline:

- **roadmap_op**: `__ROADMAP_OP__ <PASS|FAIL> <subcmd> <reason>` — e.g. `__ROADMAP_OP__ PASS verify .roadmap.battlenet 0 clean` / `__ROADMAP_OP__ FAIL update cond=cond.3 status_downgrade_refused:met->partial`
- **closure_orchestrator**: `__CLOSURE__ <PASS|FAIL> <subcmd> <reason>` — e.g. `__CLOSURE__ PASS status pct=25 cm=20/30 src=baseline_cycle37_pinned` / `__CLOSURE__ FAIL emit marker_already_exists:state/markers/closure_progress_<unix_ts>.marker`
- **warden**: `__BNET_WARDEN__ <signal_kind> <vector_id>` — e.g. `__BNET_WARDEN__ DETECTION_TRACKED memory_scan_periodicity` (signal_kind ∈ {DETECTION_TRACKED / OBSERVED_INERT / BLOCK_ACKNOWLEDGED}, NOT verdict — vector 별 honest tracking posture)

### §1.3 independent operation — 현재 구조

세 tool / module 은 모두 **standalone hexa CLI** — 각자 `r0_common` lib 측 `r0_emit` + `r0_session_set` baseline 정합 + `exec("date +%s")` + `write_file` atomic write pattern. cross-tool 측 invoke / aggregate / cross-cite 측 0건 — **manual orchestration only** (사용자 측 각 tool 측 별도 호출 후 emit 측 별도 read).

---

## §2 integration surfaces — 3 cross-cutting cross-reference

본 §2 측 3 cross-cutting integration surface 측 enumerate. 각 surface 측 (a) 두 tool / module 측 어떤 cross-reference 가 의미 있는지 / (b) 현재 manual 측 어떤 burden 이 누적되는지 / (c) integration land 후 어떤 unified surface 로 reduce 되는지 명시.

### §2.1 cross-ref A — `roadmap_op verify` + `closure_orchestrator status` aggregate

**현재 manual burden**:
- `roadmap_op verify --roadmap .roadmap.<name>` 측 14 file 측 schema clean 검증 — 각 .roadmap.* file 측 별도 verify 호출
- `closure_orchestrator status` 측 closure_pct + cm_ratio + skeleton_manjeom + validated_manjeom snapshot 별도 호출
- 두 tool 측 emit 측 사용자 측 manual pair-up — `.roadmap.<name>` 측 cond.<n> partial / met progression 과 closure_pct 측 advance 측 cross-link manual

**cross-reference rationale**:
- 두 tool 모두 `.roadmap.*` family + closure SSOT (`docs/CLOSURE_ROADMAP.md` + `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md`) 측 touch
- `.roadmap.diablo2_resurrected` cond.1 (offline shim) met → closure_pct 측 skeleton manjeom +Δ 측 정합 추적 필요
- 14 .roadmap.* 측 verify clean 결과 + closure_pct snapshot 측 single unified report 가 의미 있음

**integration land 후 unified surface (proposed `tool/integration_report.hexa status_aggregate`)**:
```
__INTEGRATION_REPORT__ PASS status_aggregate
  roadmap_op_verify: 14/14 clean
  closure_orchestrator_status: pct=25 cm=20/30 src=baseline_cycle37_pinned
  cross_ref: closure_pct_skeleton_to_roadmap_cond_met_alignment
```

### §2.2 cross-ref B — `roadmap_op show .roadmap.battlenet` + `c_bnet_warden_environment_compat self_test` cross-cite

**현재 manual burden**:
- `.roadmap.battlenet` cond.3 evidence chain 측 Warden detection-vector 명시 → 현재 hand-edit 측 evidence row append (`docs/WARDEN_HONEST_FREEZE.md` cross-link 측 manual reference)
- `c_bnet_warden_environment_compat self_test` 측 7 detection-vector emit (`__BNET_WARDEN__ DETECTION_TRACKED <vector>`) — 별도 자체 honest tracking posture, .roadmap.battlenet cond.3 evidence chain 과 cross-cite 측 manual

**cross-reference rationale**:
- `.roadmap.battlenet` cond.3 = "online expansion lock-in posture documented (`docs/WARDEN_HONEST_FREEZE.md` evidence)" — Warden detection-vector 7 항목 측 cond.3 evidence row 측 자연스러운 source
- 현재 evidence row append 측 hand-edit + Warden self_test emit 측 별도 read → manual error-prone path
- integration 측 `roadmap_op show --roadmap .roadmap.battlenet` 호출 시 c_bnet_warden_environment_compat 측 detection-vector enumerate 결과 측 cross-cite emit 가능

**integration land 후 unified surface (proposed)**:
```
__ROADMAP_OP__ PASS show roadmap=.roadmap.battlenet name=battlenet  ← 기존 emit (변경 X)
__ROADMAP_OP__ INFO show cross_ref=c_bnet_warden_environment_compat vectors=7 cond=cond.3  ← OPTIONAL extension
```
또는:
```
__INTEGRATION_REPORT__ PASS warden_evidence_cross_cite cond=battlenet.cond.3 vectors=7 source=lib/perf/c_bnet_warden_environment_compat.hexa
```

### §2.3 cross-ref C — `closure_orchestrator emit` + Track A/G/N PARTIAL emit aggregation

**현재 manual burden**:
- Track A (anticheat detection) / G (D2R cond.3 field norm) / N (network stack 3-loader) 등 측 각 cycle 측 PARTIAL emit (예: `r0_emit("d2r_cond3_partial", ...)` / `r0_emit("network_stack_partial", ...)`) 측 누적
- closure_pct 측 progression (skeleton manjeom +Δ) 측 위 PARTIAL emit count growth 와 직접 연결 — 그러나 closure_orchestrator emit 측 PARTIAL count 측 자동 reflect 없음
- skeleton-tier emit count growth 측 cycle-by-cycle manual tabulate

**cross-reference rationale**:
- closure_pct = skeleton 6/6 × 25 score (cycle 37) — 다음 phase 측 stage1 → validated → done 측 advance 시 PARTIAL emit count 측 직접 reflect 필요
- Track A/G/N + 다른 BG-AGB cycle 측 cumulative PARTIAL emit 측 closure_pct 측 advance 의 numerical evidence — 현재 closure_orchestrator emit 측 cm_ratio + skeleton_manjeom 측 hardcoded baseline 만 reflect

**integration land 후 unified surface (proposed)**:
```
__CLOSURE__ PASS emit marker=state/markers/closure_progress_<unix_ts>.marker pct=25 cm=20/30  ← 기존 emit (변경 X)
__CLOSURE__ INFO emit partial_emit_aggregate_count=<N> source_tracks=[A, G, N, ...]  ← OPTIONAL extension
```
또는 `tool/integration_report.hexa partial_emit_aggregate` subcmd 측 land 후 dedicated emit.

---

## §3 emit format unification proposal

### §3.1 common envelope

본 design 은 다음 common envelope 측 propose:

```
__<TOOL>__ <verdict> <subcmd> <reason> [<cross_ref>]
```

- `<TOOL>` ∈ {`ROADMAP_OP`, `CLOSURE`, `BNET_WARDEN`, `INTEGRATION_REPORT`} — 본 cycle 측 4 prefix.
- `<verdict>` ∈ {`PASS`, `FAIL`, `INFO`} — 기존 `PASS` / `FAIL` 측 정합 + `INFO` 측 cross-ref / aggregate context emit (verdict semantic 측 PASS/FAIL 측 결정 영향 X).
- `<subcmd>` — tool 측 subcommand (e.g. `verify` / `status` / `emit` / `self-test` / `status_aggregate` / `partial_emit_aggregate` / `warden_evidence_cross_cite`).
- `<reason>` — 기존 emit 측 reason / detail 측 그대로.
- `[<cross_ref>]` — **OPTIONAL extension field** (backward-compatible, 기존 emit consumer 측 ignore 가능).

### §3.2 backward compatibility 정합

- 기존 emit 3 prefix (`__ROADMAP_OP__` / `__CLOSURE__` / `__BNET_WARDEN__`) 측 verdict + subcmd + reason 측 이미 본 envelope 측 정합 — extension 측 cross_ref OPTIONAL field 측 추가만.
- 기존 emit 측 in-place modification 0건 — 본 cycle 측 design 측 **additive at protocol level**.
- `__BNET_WARDEN__` 측 verdict 측 위치 측 `<signal_kind>` (DETECTION_TRACKED 등) — 본 envelope 측 verdict ∈ {PASS/FAIL/INFO} 측 strict view 측 mismatch. 본 cycle 측 honest disclosure (caveat C2) — Warden emit 측 envelope 측 partial fit only, integration_report.hexa 측 driver 측 signal_kind 측 verdict 측 mapping (DETECTION_TRACKED → INFO + cross_ref=signal_kind).

### §3.3 OPTIONAL cross_ref field 측 형식 examples

- `cond=battlenet.cond.3` — `.roadmap.<name>` cond reference
- `closure_pct=25` — closure snapshot reference
- `vectors=7` — detection-vector count reference
- `source_tracks=[A,G,N]` — PARTIAL emit aggregate source tracks
- `module=c_bnet_warden_environment_compat` — module cross-cite

본 cross_ref field 측 free-form key=value (no strict schema, additive expansion 정합) — 후속 cycle 측 strict schema land 측 별도 design.

---

## §4 aggregate report flow — proposed `tool/integration_report.hexa`

### §4.1 path + identity (NOT impl in this cycle)

- proposed path: `tool/integration_report.hexa`
- proposed LOC: 250-400 (estimate, mirror closure_orchestrator 526 LOC scale 측 축소 — driver only, scrape body 측 r0_common reuse)
- proposed import: `use "../lib/perf/r0_common"`
- proposed emit prefix: `__INTEGRATION_REPORT__ <verdict> <subcmd> <reason> [<cross_ref>]`
- proposed exit codes: 0 (PASS) / 1 (FAIL)

### §4.2 proposed subcommand surface (3 — 모두 read-only/additive)

| subcmd | I/O kind | description |
|---|---|---|
| `status_aggregate` | read-only print | `roadmap_op verify` (14 .roadmap.*) + `closure_orchestrator status` 측 driver — unified TSV + markdown summary emit. cross-ref A surface. |
| `warden_evidence_cross_cite` | read-only print | `c_bnet_warden_environment_compat vectors` + `roadmap_op show --roadmap .roadmap.battlenet` 측 driver — Warden 7 detection-vector 측 cond.3 evidence cross-cite emit. cross-ref B surface. |
| `partial_emit_aggregate` | read-only print | session-level r0_emit log scrape — Track A/G/N 등 측 PARTIAL emit count tabulate + closure_pct progression cross-cite. cross-ref C surface. |

### §4.3 proposed unified TSV / markdown schema

**TSV (machine-readable)**:
```
section	tool	subcmd	verdict	reason	cross_ref
status_aggregate	roadmap_op	verify	PASS	14_files_clean	—
status_aggregate	closure_orchestrator	status	PASS	pct=25_cm=20/30	src=baseline_cycle37_pinned
warden_evidence_cross_cite	c_bnet_warden_environment_compat	vectors	INFO	7_distinct_vectors	cond=battlenet.cond.3
partial_emit_aggregate	closure_orchestrator	emit	INFO	partial_count=<N>	source_tracks=[A,G,N]
```

**markdown (human-readable)**:
```markdown
# integration report — <unix_ts>

## §1 status_aggregate
- roadmap_op verify: 14/14 clean
- closure_orchestrator status: pct=25, cm=20/30, src=baseline_cycle37_pinned

## §2 warden_evidence_cross_cite
- c_bnet_warden_environment_compat: 7 distinct vectors → battlenet.cond.3 evidence chain

## §3 partial_emit_aggregate
- partial emit count: <N>, source tracks: [A, G, N, ...]
- closure_pct progression: <skeleton_manjeom advance Δ>
```

### §4.4 self_test outline (proposed, NOT impl)

- T1: status_aggregate dispatch 측 PASS — roadmap_op + closure_orchestrator 측 driver 호출 후 unified TSV emit
- T2: warden_evidence_cross_cite dispatch 측 PASS — c_bnet_warden_environment_compat 측 vectors() enumerate 호출 후 cond.3 evidence cross-cite emit
- T3: partial_emit_aggregate dispatch 측 PASS — synthetic partial emit count tabulate
- T4: TSV schema shape (6-column) 정합 검증
- T5: markdown 측 3 section coverage 검증

---

## §5 own1 / own2 alignment

### §5.1 own1 (Wine 0 / hexa-native integration)

- proposed `tool/integration_report.hexa` 측 import 측 `r0_common` 만 (Wine / CrossOver / Whisky / GPTK / community helper 0줄)
- 외부 orchestrator (make / just / bash / shell pipeline) 측 도입 0줄 — 본 design 측 hexa-native only.
- 3 driven tool / module 측 invoke 측 hexa CLI 측 자체 dispatch (e.g. `exec("hexa run tool/closure_orchestrator.hexa status")` 측 hexa runtime 측 가용 시) — runtime 측 hexa CLI 가용성 측 caveat C5 참고.
- own1 정합 verdict (design): **PASS**

### §5.2 own2 (design honest / silent_swallow 0)

- 본 doc 측 emit format 측 working surface 와 extension 가능 surface 측 명시 — `__BNET_WARDEN__` 측 verdict 측 partial fit (signal_kind ≠ verdict) 측 honest disclosure (§3.2 + caveat C2)
- 모든 cross-ref surface 측 manual burden + integration land 후 reduce 측 명시 — silent_swallow 0
- proposed emit envelope 측 OPTIONAL extension 측 backward-compatible 측 명시 — 기존 emit consumer 측 ignore 가능
- own2 정합 verdict (design): **PASS**

### §5.3 additive at protocol level lock-in

- 기존 emit 3 prefix 측 in-place modification 0건 — common envelope 측 OPTIONAL cross_ref field 측 extension 만.
- 기존 .roadmap.* / docs / lib / tool / native / tests 측 modification 0건 (read-only reference for emit format only).
- 후속 implementation cycle 측 `tool/integration_report.hexa` 측 NEW file 1 개 land — 기존 3 tool / module 측 untouched.

---

## §6 cross-link to predecessor markers (Track A-V trail)

본 issue 측 누적 trail 측 다음 markers / docs 측 명시 (timeline 순):

| track | marker / doc | relation |
|---|---|---|
| A | `state/markers/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed.marker` | PARTIAL emit source — closure_pct progression numerical evidence (cross-ref C) |
| E | `docs/GAMEGUARD_HONEST_FREEZE.md` | sister honest freeze (NCSOFT GameGuard kernel-mode counterpart) — Warden envelope partial fit precedent |
| G | `state/markers/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed.marker` | PARTIAL emit source — D2R cond.3 field norm (cross-ref C) |
| H | `docs/ROADMAP_OP_ADOPTION_DECISION.md` | predecessor decision doc — roadmap_op self-impl Option B 권고, 본 cycle 측 design doc 측 style mirror |
| I | `state/markers/airgenome_gamebox_roadmap_op_self_impl_landed.marker` | tool/roadmap_op.hexa initial land (Option B self-impl) |
| J | `docs/WARDEN_HONEST_FREEZE.md` + `lib/perf/c_bnet_warden_environment_compat.hexa` | Warden honest freeze sister doc — cross-ref B 측 source |
| L | `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` | sister doc — cleanest cycle posture mirror precedent |
| M | `state/markers/airgenome_gamebox_roadmap_op_full_11_of_11_landed.marker` | tool/roadmap_op.hexa 11/11 anima parity (1626 LOC) |
| N | `state/markers/airgenome_gamebox_online_network_stack_3_loader_landed.marker` | PARTIAL emit source — network stack 3-loader (cross-ref C) |
| P | `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` | 6 BG-AGB rank B+C unblock gate origin (closure_orchestrator 측 unblock 측 origin) |
| S | `docs/CLOSURE_ORCHESTRATOR_DESIGN.md` + `tool/closure_orchestrator.hexa` | closure_orchestrator design + impl (526 LOC, cross-ref A + C 측 source) |
| T | `docs/PERF_MANJEOM_AGGREGATION_REVIEW.md` | cleanest cycle posture mirror Track I/L/T/Y precedent |
| Y | (sibling cleanest cycle) | 본 design doc 측 cleanest cycle posture 직접 sibling |

→ 본 doc 측 land 측 위 trail 측 cross-tool integration 측 design surface 측 explicit narrative spec freeze. 후속 implementation cycle 측 별도 marker 측 land.

---

## §7 implementation cycle estimate (N cycles)

본 §7 측 design-only — 후속 implementation cycle 측 estimate 만 명시. 본 cycle 측 implementation 0건.

### §7.1 minimum-viable subset (1 cycle)

- `tool/integration_report.hexa` skeleton + `status_aggregate` subcmd 1 개 land — cross-ref A surface 만.
- 250-300 LOC 추정 — closure_orchestrator 526 LOC 측 절반 scale (driver only, scrape body 측 r0_common + 기존 tool 측 reuse).
- self_test 측 T1 + T4 (TSV schema shape) 만 land.

### §7.2 full impl (≥ 3 cycle)

- cycle 1: `tool/integration_report.hexa` skeleton + `status_aggregate` (cross-ref A) — minimum-viable.
- cycle 2: `warden_evidence_cross_cite` (cross-ref B) + Warden envelope partial fit 측 honest mapping (signal_kind → INFO + cross_ref).
- cycle 3: `partial_emit_aggregate` (cross-ref C) + session-level r0_emit log scrape + closure_pct progression cross-cite.

### §7.3 per-tool cross_ref field extension (≥ 1 cycle, optional)

- `tool/roadmap_op.hexa` 측 OPTIONAL cross_ref field 측 add (e.g. `__ROADMAP_OP__ PASS show roadmap=.roadmap.battlenet name=battlenet cross_ref=c_bnet_warden_environment_compat`)
- `tool/closure_orchestrator.hexa` 측 OPTIONAL cross_ref field 측 add
- `lib/perf/c_bnet_warden_environment_compat.hexa` 측 OPTIONAL cross_ref field 측 add
- 본 cycle 측 결정 사항 X — 사용자 lock-in 후 별도 cycle.

### §7.4 aggregation TSV schema land (≥ 1 cycle)

- TSV 6-column schema (section / tool / subcmd / verdict / reason / cross_ref) 측 strict freeze + writer + reader test land.
- markdown 측 3 section coverage 측 strict freeze + writer test land.
- 본 cycle 측 narrative-level schema 측 freeze, 후속 cycle 측 actual writer / reader land.

### §7.5 합 estimate

- minimum-viable subset 측 land 만: **1 cycle**
- full impl + per-tool cross_ref field extension + TSV schema strict land: **≥ 5 cycle** (3 + 1 + 1)
- 사용자 측 어느 scope 측 land 시작 측 lock-in 필요 (caveat C7 참고)

---

## §8 caveats (raw 10 honest C3 — 12 항목)

**C1 — design-only land**: 본 cycle 측 `tool/integration_report.hexa` impl 0건 / 기존 3 tool / module 측 modification 0건 / `.roadmap.*` modification 0건 / 기존 docs/*.md modification 0건. 본 doc 측 narrative spec freeze, 실 코드 변경 X.

**C2 — `__BNET_WARDEN__` envelope partial fit**: §3.1 common envelope 측 verdict ∈ {PASS/FAIL/INFO} 측 strict view 측 Warden emit 측 signal_kind (DETECTION_TRACKED / OBSERVED_INERT / BLOCK_ACKNOWLEDGED) 측 mismatch. integration_report.hexa 측 driver 측 signal_kind 측 verdict 측 mapping (DETECTION_TRACKED → INFO + cross_ref=signal_kind=DETECTION_TRACKED) — 본 mapping 측 honest disclosure (signal_kind 측 verdict 의미 X, posture annotation only).

**C3 — backward-compatibility 측 protocol-level only**: §3.2 측 backward-compatible 측 emit format level — emit consumer (사용자 측 manual read / grep / log scrape) 측 OPTIONAL cross_ref field 측 ignore 가능 측 정합. 단 strict parser 측 land 시 cross_ref field 측 strict schema 측 별도 design (caveat C9 참고).

**C4 — proposed subcmd surface 측 user lock-in**: §4.2 측 3 proposed subcmd (status_aggregate / warden_evidence_cross_cite / partial_emit_aggregate) 측 본 cycle 측 freeze X — 사용자 측 implementation cycle 측 final subcmd surface 측 lock-in 필요 (예: 3 subcmd 측 1 unified subcmd 측 merge 가능성).

**C5 — hexa CLI runtime emit 측 schema_validated_only**: 본 cycle 측 hexa CLI runtime 측 본 doc 측 spec 측 실행 검증 0건 (predecessor cycle posture — cycle 37 baseline 측 hexa CLI 가용 가정 X). schema-level + LOC count + sha256 pin 까지만 본 cycle 측 verify. integration_report.hexa 측 후속 implementation cycle 측 hexa CLI runtime 측 self_test 측 land 시 검증.

**C6 — own1 verdict 본 cycle 자체**: 본 design doc 자체 측 own1 audit (Wine 0 / 외부 orchestrator 0 / hexa-native only) verdict = **PASS** (본 doc 측 narrative only, 실 코드 0줄). 후속 implementation cycle 측 module 별도 audit 권고.

**C7 — implementation cycle estimate 측 underdetermined**: §7 측 estimate 측 hexa stdlib actual coverage + 사용자 측 cycle budget 측 측정 X 측 추정. minimum-viable 1 cycle 측 conservative — 5 cycle 측 full impl 측 ±2 cycle variance 가능. 사용자 lock-in 시 명시.

**C8 — cross-ref A 측 closure_pct ↔ .roadmap.<name> cond.<n> alignment 측 narrative only**: §2.1 측 .roadmap.diablo2_resurrected cond.1 met → closure_pct skeleton manjeom +Δ 정합 추적 측 narrative-level alignment 측 명시. 실 numeric mapping (어느 cond.<n> met 측 어느 Δ closure_pct) 측 본 cycle 측 freeze X — 후속 cycle 측 numerical mapping 측 별도 design (예: cond.met counter × 25 / total_cond_count 측 weighted score).

**C9 — cross_ref field strict schema 측 부재**: §3.3 측 cross_ref field 측 free-form key=value 측 명시 — strict schema 측 본 cycle 측 freeze X. 후속 cycle 측 cross_ref field 측 strict schema (예: `cross_ref:cond=<id>;closure_pct=<int>;vectors=<int>` 측 semicolon-delimited key=value) 측 별도 design 권고.

**C10 — partial_emit_aggregate 측 r0_emit log scrape 측 ephemeral session 의존**: §2.3 + §4.2 측 partial_emit_aggregate 측 session-level r0_emit log scrape 측 의존 — `r0_session_set` 측 session-id 측 ephemeral. cycle-spanning aggregate 측 별도 persistence 측 land 필요 (예: state/markers/partial_emit_history.jsonl 측 append-only) — 후속 cycle 측 별도 design.

**C11 — external orchestrator (make/just/bash) 측 영구 X 측 lock-in**: own1 정합 — 본 design 측 hexa-native only. 사용자 측 후속 cycle 측 외부 orchestrator 측 도입 권고 시 — own1 위반 + raw 12 (silent_error_ban) 위반 + raw 175 (BR-NO-USER-VERBATIM) 위반 측 risk. 본 cycle 측 lock-in: integration 측 hexa-native only 영구 (caveat C5 측 hexa CLI runtime 가용성 측 별개 영역 — runtime 가용 시 hexa-native dispatch 정합 / runtime 미가용 시 manual integration 측 fallback only).

**C12 — Track Z 측 cleanest cycle posture mirror Track I/L/T/Y**: 본 cycle 측 in_place_writes=0 / destructive_ops=0 / 3 NEW file (design doc + handoff + marker) 측 land — Track I (roadmap_op self_impl) + Track L (D2R realm protocol honest freeze) + Track T (perf_manjeom aggregation review) + Track Y (sibling) 측 cleanest cycle posture 측 mirror. design-only, implementation 측 별도 cycle.

---

## §9 출처 (sources)

| 출처 | 영역 | 경로 |
|---|---|---|
| `tool/roadmap_op.hexa` | Track I+M tool — emit format SSOT (`__ROADMAP_OP__`) + 11/11 anima parity surface | 1626 LOC |
| `tool/closure_orchestrator.hexa` | Track S tool — emit format SSOT (`__CLOSURE__`) + 3 subcmd (status/list/emit) | 526 LOC |
| `lib/perf/c_bnet_warden_environment_compat.hexa` | Track J perf module — emit format SSOT (`__BNET_WARDEN__`) + 7 detection-vector | 195 LOC |
| `docs/CLOSURE_ORCHESTRATOR_DESIGN.md` | Track S design doc — 본 doc 측 style mirror | 221 LOC |
| `docs/ROADMAP_OP_ADOPTION_DECISION.md` | Track H decision doc — design doc style precedent | 214 LOC |
| `docs/WARDEN_HONEST_FREEZE.md` | Track J honest spec freeze — sister doc precedent | 390 LOC |
| `docs/PERF_MANJEOM_AGGREGATION_REVIEW.md` | Track T cross-cutting review — cleanest cycle posture mirror | 301 LOC |
| `state/markers/airgenome_gamebox_perf_manjeom_aggregation_review_landed.marker` | predecessor cycle marker (cleanest cycle posture mirror Track I/L) | — |
| `state/markers/airgenome_gamebox_closure_orchestrator_landed.marker` | predecessor cycle marker (Track S, closure_orchestrator land) | — |
| `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker` | predecessor cycle marker (Track J, Warden honest freeze land) | — |

---

*written 2026-05-04 — Track Z cross-tool integration design mk1 narrative spec freeze.*
*own1 Wine 0 / hexa-native integration only / 외부 orchestrator 0 — design-only.*
*own2 design honest / signal_kind ↔ verdict partial fit explicit / silent_swallow 0건.*
*additive at protocol level (existing emit format extended, NEVER replaced) / migration forbidden / destructive 0 / cost 0 / mac-local / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker enforced.*
*implementation 0건 — 후속 cycle 측 `tool/integration_report.hexa` land + per-tool cross_ref field extension + aggregation TSV schema 측 별도 cycle.*
