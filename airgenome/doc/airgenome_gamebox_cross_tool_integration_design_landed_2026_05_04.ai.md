# airgenome-gamebox — Track Z cross-tool integration design landed (2026-05-04)

> **TL;DR**: 3 independent tool/perf module (`tool/roadmap_op.hexa` Track I+M 1626 LOC / `tool/closure_orchestrator.hexa` Track S 526 LOC / `lib/perf/c_bnet_warden_environment_compat.hexa` Track J 195 LOC) cross-tool integration design mk1 narrative spec freeze. design-only land (cleanest cycle posture, mirror Track I/L/T/Y, in_place_writes=0). 3 cross-cutting cross-reference surface enumerate (A — roadmap_op verify + closure_orchestrator status aggregate / B — roadmap_op show .roadmap.battlenet + warden self_test cross-cite / C — closure_orchestrator emit + Track A/G/N PARTIAL emit aggregation). emit format unification proposal — common envelope `__<TOOL>__ <verdict> <subcmd> <reason> [<cross_ref>]` (backward-compatible, cross_ref OPTIONAL extension). proposed `tool/integration_report.hexa` (NOT impl in this cycle, design only). own1 (Wine 0 / hexa-native integration / 외부 orchestrator 0) + own2 (design honest / signal_kind↔verdict partial fit explicit / silent_swallow 0). additive_only / migration forbidden / in_place_writes=0 / destructive_ops=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker.

## §0 baseline

- predecessor handoff: `airgenome/doc/airgenome_gamebox_perf_manjeom_aggregation_review_landed_2026_05_03.ai.md` (Track T — cleanest cycle posture mirror precedent)
- predecessor marker: `state/markers/airgenome_gamebox_perf_manjeom_aggregation_review_landed.marker`
- predecessor design doc style mirror: `docs/CLOSURE_ORCHESTRATOR_DESIGN.md` (Track S, 221 LOC) + `docs/ROADMAP_OP_ADOPTION_DECISION.md` (Track H, 214 LOC) + `docs/WARDEN_HONEST_FREEZE.md` (Track J, 390 LOC) + `docs/PERF_MANJEOM_AGGREGATION_REVIEW.md` (Track T, 301 LOC)
- 본 cycle 측 lib/ touch 0건 / tool/ touch 0건 / native/ touch 0건 / tests/ touch 0건 / `.roadmap.*` touch 0건 / 기존 docs/*.md touch 0건 (read-only reference for emit format only)

## §1 user_directive_paraphrase

BG subagent — Track Z cross-tool integration design land for airgenome-gamebox: 3 independent tool/perf module (tool/roadmap_op.hexa Track I+M 1626 LOC 11/11 anima parity / tool/closure_orchestrator.hexa Track S 526 LOC 3 subcmd / lib/perf/c_bnet_warden_environment_compat.hexa Track J 195 LOC 7 detection-vector) 측 wire together design — aggregate report / cross-reference / unified emit dashboard mk1 narrative spec freeze. design-only cycle (cleanest cycle posture mirror Track I/L/T/Y, in_place_writes=0). 3 cross-cutting cross-reference surface (A roadmap_op verify + closure_orchestrator status / B roadmap_op show .roadmap.battlenet + warden self_test / C closure_orchestrator emit + Track A/G/N PARTIAL emit) + emit format unification proposal (common envelope, OPTIONAL cross_ref extension, backward-compatible) + proposed tool/integration_report.hexa (NOT impl in this cycle, design only) + own1/own2 alignment + cross-link to Track A-V trail + implementation cycle estimate + ≥8 honest caveats. 1 NEW design doc (docs/CROSS_TOOL_INTEGRATION_DESIGN.md, 250-400 LOC) + 1 NEW handoff + 1 NEW marker. policy: additive_only / migration forbidden / destructive_ops=0 / in_place_writes=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker. allowed scope: 3 NEW files only; 그외 모두 forbidden (read-only reference only).

## §2 design summary

### §2.1 3 independent tool/perf module current state

| # | path | track | LOC | emit prefix | scope |
|---|---|---|---|---|---|
| 1 | `tool/roadmap_op.hexa` | I + M | 1626 | `__ROADMAP_OP__ <PASS\|FAIL> <subcmd> <reason>` | 14 mk2 .roadmap.<name> JSONL CLI dispatcher (11/11 anima parity) |
| 2 | `tool/closure_orchestrator.hexa` | S | 526 | `__CLOSURE__ <PASS\|FAIL> <subcmd> <reason>` | closure_pct + CM-N/30 progression tracker (3 subcmd: status / list / emit) |
| 3 | `lib/perf/c_bnet_warden_environment_compat.hexa` | J | 195 | `__BNET_WARDEN__ <signal_kind> <vector_id>` | Warden honest detection-vector tracker (7 vectors, NOT bypass) |

### §2.2 3 cross-cutting integration surface

**Cross-ref A** — `roadmap_op verify` + `closure_orchestrator status` aggregate emit
- 두 tool 모두 `.roadmap.*` family + closure SSOT 측 touch
- 14 .roadmap.* verify clean + closure_pct snapshot 측 single unified report
- proposed unified surface: `__INTEGRATION_REPORT__ PASS status_aggregate roadmap_op_verify=14/14_clean closure_orchestrator_status=pct=25_cm=20/30 cross_ref=closure_pct_skeleton_to_roadmap_cond_met_alignment`

**Cross-ref B** — `roadmap_op show --roadmap .roadmap.battlenet` + `c_bnet_warden_environment_compat self_test` cross-cite
- `.roadmap.battlenet` cond.3 evidence chain 측 Warden 7 detection-vector 측 자연스러운 source
- 현재 hand-edit + manual reference path → integration 측 자동 cross-cite emit
- proposed unified surface: `__INTEGRATION_REPORT__ PASS warden_evidence_cross_cite cond=battlenet.cond.3 vectors=7 source=lib/perf/c_bnet_warden_environment_compat.hexa`

**Cross-ref C** — `closure_orchestrator emit` + Track A/G/N PARTIAL emit aggregation
- skeleton-tier emit count growth 측 closure_pct progression numerical evidence
- 현재 cycle-by-cycle manual tabulate path → integration 측 session r0_emit log scrape + closure_pct cross-cite
- proposed unified surface: `__INTEGRATION_REPORT__ INFO partial_emit_aggregate partial_count=<N> source_tracks=[A,G,N,...] closure_pct=<int>`

### §2.3 emit format unification proposal

common envelope: `__<TOOL>__ <verdict> <subcmd> <reason> [<cross_ref>]`

- `<TOOL>` ∈ {ROADMAP_OP, CLOSURE, BNET_WARDEN, INTEGRATION_REPORT}
- `<verdict>` ∈ {PASS, FAIL, INFO} — 기존 PASS/FAIL 정합 + INFO 측 cross-ref / aggregate context
- `<subcmd>` — tool subcommand
- `<reason>` — 기존 emit reason / detail
- `[<cross_ref>]` — **OPTIONAL extension field** (backward-compatible, 기존 consumer 측 ignore 가능)

**partial fit 측 honest disclosure**: `__BNET_WARDEN__` emit 측 verdict 측 위치 측 signal_kind (DETECTION_TRACKED 등) — strict envelope view 측 mismatch. integration_report.hexa 측 driver 측 signal_kind → INFO + cross_ref=signal_kind=DETECTION_TRACKED mapping (caveat C2).

### §2.4 proposed `tool/integration_report.hexa` (NOT impl in this cycle)

- proposed path: `tool/integration_report.hexa`
- proposed LOC: 250-400 (mirror closure_orchestrator scale 측 축소, driver only)
- proposed import: `use "../lib/perf/r0_common"`
- proposed emit prefix: `__INTEGRATION_REPORT__`
- proposed exit codes: 0 (PASS) / 1 (FAIL)
- proposed 3 subcmd: status_aggregate / warden_evidence_cross_cite / partial_emit_aggregate (모두 read-only/additive)
- proposed unified TSV (6-column: section / tool / subcmd / verdict / reason / cross_ref) + markdown 3-section (status_aggregate / warden_evidence_cross_cite / partial_emit_aggregate)

## §3 implementation cycle estimate

| scope | cycle 추정 | description |
|---|---|---|
| minimum-viable subset | 1 cycle | tool/integration_report.hexa skeleton + status_aggregate (cross-ref A) only |
| full impl | ≥ 3 cycle | cycle 1: status_aggregate / cycle 2: warden_evidence_cross_cite + Warden envelope mapping / cycle 3: partial_emit_aggregate + r0_emit log scrape |
| per-tool cross_ref field extension | ≥ 1 cycle | 3 tool/module 측 OPTIONAL cross_ref field add (사용자 lock-in 후) |
| aggregation TSV schema strict freeze | ≥ 1 cycle | TSV 6-column schema + writer/reader test land |
| **합 estimate (full)** | **≥ 5 cycle** | minimum-viable 측 1 cycle / full + extension + schema 측 5 cycle / ±2 cycle variance |

사용자 측 어느 scope 측 land 시작 측 lock-in 필요 (caveat C7).

## §4 own1 / own2 alignment

### §4.1 own1 (Wine 0 / hexa-native integration / 외부 orchestrator 0)

- proposed `tool/integration_report.hexa` 측 import 측 `r0_common` 만 (Wine / CrossOver / Whisky / GPTK / community helper 0줄)
- 외부 orchestrator (make / just / bash / shell pipeline) 측 도입 0줄 — design 측 hexa-native only 영구 lock-in (caveat C11)
- 3 driven tool/module 측 invoke 측 hexa CLI 측 자체 dispatch (runtime 가용 시) / runtime 미가용 시 manual integration fallback (caveat C5)
- own1 verdict (design 자체): **PASS**

### §4.2 own2 (design honest / signal_kind↔verdict partial fit explicit / silent_swallow 0)

- emit format working surface + extension surface 명시 — `__BNET_WARDEN__` 측 verdict 측 signal_kind 측 partial fit 측 honest disclosure (caveat C2)
- cross-ref surface 측 manual burden + integration land 후 reduce 측 명시
- proposed envelope OPTIONAL extension 측 backward-compatible 측 명시 (기존 consumer ignore 가능)
- silent_swallow 0건 / 12 honest C3 caveats inline
- own2 verdict (design 자체): **PASS**

### §4.3 additive at protocol level lock-in

- 기존 emit 3 prefix (`__ROADMAP_OP__` / `__CLOSURE__` / `__BNET_WARDEN__`) 측 in-place modification 0건 — common envelope 측 OPTIONAL cross_ref extension 만
- 기존 .roadmap.* / docs / lib / tool / native / tests 측 modification 0건 (read-only reference for emit format only)
- 후속 implementation cycle 측 NEW file (`tool/integration_report.hexa`) 1개 land — 기존 3 tool/module untouched

## §5 raw#10 honest C3 caveats summary (12 항목)

본 cycle 측 design doc §8 측 inline 12 honest C3 caveats:

- **C1** — design-only land (impl 0건)
- **C2** — `__BNET_WARDEN__` envelope partial fit (signal_kind ≠ verdict, mapping 측 honest disclosure)
- **C3** — backward-compatibility 측 protocol-level only (strict parser 측 별도 design)
- **C4** — proposed subcmd surface 측 user lock-in 필요
- **C5** — hexa CLI runtime emit 측 schema_validated_only (cycle 37 baseline 측 hexa CLI 가용 가정 X)
- **C6** — own1 verdict 본 cycle 자체 (narrative only, 실 코드 0줄)
- **C7** — implementation cycle estimate 측 underdetermined (1 cycle minimum-viable / 5 cycle full / ±2 variance)
- **C8** — cross-ref A 측 closure_pct ↔ .roadmap.<name> cond.<n> alignment 측 narrative only (실 numeric mapping 측 후속 cycle)
- **C9** — cross_ref field strict schema 측 부재 (free-form key=value, strict schema 측 별도 design)
- **C10** — partial_emit_aggregate 측 r0_emit log scrape 측 ephemeral session 의존 (cross-cycle persistence 측 별도 design)
- **C11** — external orchestrator (make/just/bash) 측 영구 X 측 lock-in (own1 정합)
- **C12** — Track Z 측 cleanest cycle posture mirror Track I/L/T/Y (in_place_writes=0 / 3 NEW file)

## §6 cross-link to Track A-V trail (predecessor markers)

| track | marker / doc | relation |
|---|---|---|
| A | `state/markers/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed.marker` | PARTIAL emit source — closure_pct progression numerical evidence (cross-ref C) |
| E | `docs/GAMEGUARD_HONEST_FREEZE.md` | sister honest freeze (envelope partial fit precedent) |
| G | `state/markers/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed.marker` | PARTIAL emit source — D2R cond.3 field norm (cross-ref C) |
| H | `docs/ROADMAP_OP_ADOPTION_DECISION.md` | predecessor decision doc — design doc style mirror |
| I | `state/markers/airgenome_gamebox_roadmap_op_self_impl_landed.marker` | tool/roadmap_op.hexa initial land (Option B self-impl) |
| J | `docs/WARDEN_HONEST_FREEZE.md` + `lib/perf/c_bnet_warden_environment_compat.hexa` | Warden honest freeze sister doc + cross-ref B 측 source |
| L | `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` | sister doc — cleanest cycle posture mirror precedent |
| M | `state/markers/airgenome_gamebox_roadmap_op_full_11_of_11_landed.marker` | tool/roadmap_op.hexa 11/11 anima parity |
| N | `state/markers/airgenome_gamebox_online_network_stack_3_loader_landed.marker` | PARTIAL emit source (cross-ref C) |
| P | `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` | 6 BG-AGB rank B+C unblock gate origin |
| S | `docs/CLOSURE_ORCHESTRATOR_DESIGN.md` + `tool/closure_orchestrator.hexa` | closure_orchestrator design + impl |
| T | `docs/PERF_MANJEOM_AGGREGATION_REVIEW.md` | cleanest cycle posture mirror precedent |
| Y | (sibling cleanest cycle) | 본 design doc 측 cleanest cycle posture 직접 sibling |
| Z | (본 cycle) | cross-tool integration design (mk1 narrative spec freeze, design-only) |

## §7 file index

- `docs/CROSS_TOOL_INTEGRATION_DESIGN.md` — mk1 narrative design doc (NEW, 350 LOC, 9 sections + 12 caveats)
- `airgenome/doc/airgenome_gamebox_cross_tool_integration_design_landed_2026_05_04.ai.md` — handoff (NEW, this file)
- `state/markers/airgenome_gamebox_cross_tool_integration_design_landed.marker` — marker (NEW)

## §8 policy summary

- migration: forbidden
- changes: additive_only
- in_place_writes: 0 (cleanest cycle posture mirror Track I/L/T/Y)
- destructive_ops: 0
- cost_usd: 0
- substrate: mac-local
- cap_minutes: 90
- br_no_user_verbatim: true
- friendly_preset: true
- silent-land marker: enforced
- own1: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / 외부 orchestrator (make/just/bash) 0 / hexa-native integration only
- own2: design honest / signal_kind↔verdict partial fit explicit (caveat C2) / 12 honest C3 caveats inline / silent_swallow 0
- design_only: implementation 0건 (후속 cycle 측 tool/integration_report.hexa land + per-tool cross_ref field extension + aggregation TSV schema 측 별도 cycle)

## §9 cross-link

- predecessor handoff: `airgenome/doc/airgenome_gamebox_perf_manjeom_aggregation_review_landed_2026_05_03.ai.md`
- predecessor marker: `state/markers/airgenome_gamebox_perf_manjeom_aggregation_review_landed.marker`
- design doc (this cycle land): `docs/CROSS_TOOL_INTEGRATION_DESIGN.md`
- 3 driven tool/module (read-only reference for emit format only):
  - `tool/roadmap_op.hexa` (1626 LOC, `__ROADMAP_OP__` emit prefix)
  - `tool/closure_orchestrator.hexa` (526 LOC, `__CLOSURE__` emit prefix)
  - `lib/perf/c_bnet_warden_environment_compat.hexa` (195 LOC, `__BNET_WARDEN__` emit prefix)
- design doc style precedents:
  - `docs/CLOSURE_ORCHESTRATOR_DESIGN.md` (Track S, mk1 narrative mirror)
  - `docs/ROADMAP_OP_ADOPTION_DECISION.md` (Track H, decision doc style)
  - `docs/WARDEN_HONEST_FREEZE.md` (Track J, honest spec freeze sister)
  - `docs/PERF_MANJEOM_AGGREGATION_REVIEW.md` (Track T, cross-cutting review style)

---

*written 2026-05-04 — Track Z cross-tool integration design handoff. design-only land, additive_only / migration forbidden / in_place_writes=0 / destructive_ops=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker. 3 cross-cutting cross-reference surface enumerate (A roadmap_op+closure / B roadmap_op+warden / C closure+partial_emit). emit format common envelope (OPTIONAL cross_ref extension, backward-compatible). proposed tool/integration_report.hexa (NOT impl in this cycle). 12 honest C3 caveats inline. own1 hexa-native integration only / own2 design honest signal_kind↔verdict partial fit explicit. implementation 0건 — 후속 cycle 측 별도 land.*
