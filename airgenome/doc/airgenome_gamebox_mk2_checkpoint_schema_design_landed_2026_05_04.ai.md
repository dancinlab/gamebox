# airgenome-gamebox — Track AA mk2 checkpoint schema design landed (2026-05-04)

> **TL;DR**: 현재 mk2 schema (`.roadmap.<name>` JSONL header with `required_conditions[]` + `blockers[]`) 측 cross-roadmap intermediate goal 표현 layer 부재 → checkpoint mechanism design freeze. 4 D decision matrix + 권고 (D1 cross-roadmap (b) / D2 separate SSOT (b) / D3 cond enum reuse (a) / D4 roadmap_op extend (a)) + 7 candidate checkpoint inventory + 사용자 lock-in 대기. design-only land (cleanest cycle posture mirror Track H/Y/Z, in_place_writes=0). own1 (Wine 0 / hexa-native / 외부 의존 0) + own2 (4 D 결정 매 매트릭스 explicit / swallowed tradeoff 0 / 14 honest C3 caveats inline). additive_only / migration forbidden / in_place_writes=0 / destructive_ops=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker.

## §0 baseline

- predecessor handoff: `airgenome/doc/airgenome_gamebox_cross_tool_integration_design_landed_2026_05_04.ai.md` (Track Z — design-only cycle posture 직접 sibling, cleanest cycle in_place_writes=0)
- predecessor marker: `state/markers/airgenome_gamebox_cross_tool_integration_design_landed.marker`
- predecessor decision doc style mirror: `docs/ROADMAP_OP_ADOPTION_DECISION.md` (Track H, 11-criteria 3-way matrix) + `docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` (Track Y, 3-way runtime decision matrix) + `docs/CROSS_TOOL_INTEGRATION_DESIGN.md` (Track Z, design-only cycle posture)
- 본 cycle 측 lib/ touch 0건 / tool/ touch 0건 / native/ touch 0건 / tests/ touch 0건 / `.roadmap.*` touch 0건 / `.checkpoint.*` 측 file 0건 (inventory only) / 기존 docs/*.md touch 0건 (read-only reference)

## §1 user_directive_paraphrase

BG subagent — Track AA mk2 checkpoint schema design land for airgenome-gamebox: 현재 mk2 schema 측 cross-roadmap intermediate goal (e.g., "D2R + Battle.net 작동", "Lineage W + Purple 작동") 측 first-class expression layer 부재 → checkpoint mechanism additive layer design freeze (NOT impl). 4 D decision matrix design — D1 scope (single-roadmap (a) vs cross-roadmap (b)) / D2 storage (header internal (a) vs NEW separate SSOT (b) vs single aggregate (c)) / D3 status enum (cond enum reuse (a) vs new enum (b)) / D4 tool extension (roadmap_op extend (a) vs NEW checkpoint_op (b) vs closure_orchestrator extend (c)) — 매 D 측 매트릭스 + 권고 + confidence + tradeoff explicit. 권고 (default): D1=b / D2=b / D3=a / D4=a 모두 confidence MEDIUM (D3 HIGH). 7 candidate checkpoint inventory — d2r_battlenet_offline_play / d2r_battlenet_online_play / lineage_m_purple_play / lineage_w_purple_play / lineage_classic_r28_play / win32_50pct_coverage / win32_full_coverage 측 cond_refs[] sketch (NOT actual file emit). implementation cycle plan estimate (3 cycle minimum / 6 cycle full) + 사용자 lock-in checklist (≥10 항목) + ≥10 honest caveats. 1 NEW design doc (`docs/MK2_CHECKPOINT_SCHEMA_DESIGN.md`, 350-500 LOC) + 1 NEW handoff (this file) + 1 NEW marker (`state/markers/airgenome_gamebox_mk2_checkpoint_schema_design_landed.marker`). policy: additive_only / migration forbidden / destructive_ops=0 / in_place_writes=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land. allowed scope: 3 NEW files only; 그외 forbidden (read-only reference). predecessor pattern: Track H/Y/Z design-only pending lock-in pattern siblings, 직접 mirror.

## §2 design summary

### §2.1 current mk2 schema gap

- `.roadmap.<name>` mk2 schema 측 3 layer (header / required_conditions[] / blockers[]) — 모두 single-roadmap-internal goal 표현
- cross-roadmap intermediate goal (e.g., D2R offline play = `.roadmap.d2r` cond.1+cond.2 + `.roadmap.battlenet` cond.1+cond.2) 측 표현 layer 0
- 현재 hand-tracking: handoff doc prose narrative + closure_orchestrator single global aggregate + cond evidence row cross-cite (manual)

### §2.2 4 D decision matrix summary

| D | question | options | recommended | confidence | matrix score |
|---|---|---|---|---|---|
| D1 | checkpoint scope | (a) single-roadmap / (b) cross-roadmap | **(b) cross-roadmap** | MEDIUM | (a) 22 vs (b) 12 (best) |
| D2 | storage location | (a) header internal / (b) separate SSOT / (c) aggregate file | **(b) separate SSOT** | MEDIUM-HIGH | (a) 33 / (b) 9 (best) / (c) 25 |
| D3 | status enum | (a) cond enum reuse / (b) new enum | **(a) reuse** | HIGH | rationale-only (raw 11 + roadmap_op verify reuse + retired_intentional cross-roadmap 정합) |
| D4 | tool extension | (a) roadmap_op extend / (b) NEW checkpoint_op / (c) closure_orchestrator extend | **(a) roadmap_op extend** | MEDIUM | (a) 19 (best) / (b) 25 / (c) 27 |

### §2.3 proposed schema example (`.checkpoint.d2r_battlenet_offline_play` narrative)

```jsonl
# .checkpoint.d2r_battlenet_offline_play mk1 — cross-roadmap intermediate goal SSOT (peer perspective, additive layer)
{"type":"header","kind":"checkpoint","name":"d2r_battlenet_offline_play","mk":1,"perspective":"peer","goal":"D2R offline single-player play","cond_refs":["diablo2_resurrected.cond.1","diablo2_resurrected.cond.2","battlenet.cond.1","battlenet.cond.2"],"aggregation":"all_must_be_met","status":"partial","progress":{"met_count":3,"partial_count":1,"unmet_count":0,"retired_count":0,"total":4},"evidence":[...],"blockers":[...],"status":"active","since":"2026-05-04"}
```

### §2.4 7 candidate checkpoint inventory

| # | checkpoint name | cond_refs[] sketch | aggregation | expected status |
|---|---|---|---|---|
| 1 | `.checkpoint.d2r_battlenet_offline_play` | `[d2r.cond.1, d2r.cond.2, battlenet.cond.1, battlenet.cond.2]` | all_must_be_met | partial (3 met + 1 partial) |
| 2 | `.checkpoint.d2r_battlenet_online_play` | `[d2r.cond.1-3, battlenet.cond.1-3]` | all_must_be_met | partial 영구 (Warden L2 long-horizon) |
| 3 | `.checkpoint.lineage_m_purple_play` | `[lineage_m.cond.1-3, purple_launcher.cond.1-2]` | all_must_be_met | partial (Android emul long-horizon) |
| 4 | `.checkpoint.lineage_w_purple_play` | `[lineage_w.cond.1-3, purple_launcher.cond.1-2]` | all_must_be_met | partial (UE4 + Purple cascading) |
| 5 | `.checkpoint.lineage_classic_r28_play` | `[lineage_classic_r28.cond.1-3]` | all_must_be_met | retired_intentional (GameGuard kernel-mode 영구 차단) |
| 6 | `.checkpoint.win32_50pct_coverage` | `[loader_win32.cond.1-3]` weighted by % | weighted_threshold (≥ 50%) | partial (cycle 35 30.34%) |
| 7 | `.checkpoint.win32_full_coverage` | `[loader_win32.cond.1-3]` 측 435/435 | weighted_threshold (= 100%) | unmet (long-horizon) |

## §3 implementation cycle plan estimate

| cycle | scope | deliverable |
|---|---|---|
| 1 | tool extension (D4 a) | `tool/roadmap_op.hexa` 측 2-3 subcmd 추가 (`checkpoint-status` / `checkpoint-list` / `checkpoint-verify`) + selftest extension (S12+) |
| 2 | first 3 .checkpoint.* SSOT land | `.checkpoint.d2r_battlenet_offline_play` + `.checkpoint.d2r_battlenet_online_play` + `.checkpoint.lineage_classic_r28_play` |
| 3 | closure_orchestrator integration | closure_orchestrator (Track S) 측 checkpoint count + status snapshot cross-cite (additive cross_ref field, Track Z envelope 정합) |
| 4-6 (full) | 나머지 4 checkpoint + weighted_threshold + schema validator | minimum-viable 3 cycle, full impl ≥ 6 cycle |

사용자 측 어느 scope 측 land 시작 측 lock-in 필요 (caveat C10).

## §4 own1 / own2 alignment

### §4.1 own1 (Wine 0 / hexa-native / 외부 의존 0)

- proposed `.checkpoint.<name>` SSOT 측 JSONL header pattern 측 hexa-native — 외부 format / runtime / orchestrator 의존 0
- proposed roadmap_op extension (D4 a) 측 `r0_common` import 만 (Wine / CrossOver / Whisky / GPTK / community helper 0줄)
- 외부 게임 runtime (Wine) 측 도입 0 — checkpoint 측 admin tool layer (cond aggregation), 게임-실행 layer 측 무관 (own1 strict reading 측 violation X, Track H + Track Y + Track 11 own1 disambiguation precedent 정합)
- own1 verdict (design 자체): **PASS**

### §4.2 own2 (design honest / silent_swallow 0)

- 4 D 결정 매 매트릭스 + 권고 + confidence + tradeoff explicit 측 own2 정합
- swallowed tradeoff 0 — 매 D 측 single recommendation 단 confidence MEDIUM (D3 만 HIGH) 측 honest
- D4 (a) 측 anima_parity_break_risk = 5 (high) 측 single weakness 측 explicit disclosure (caveat C5 + C8)
- 7 candidate 측 expected status (partial 영구 / retired_intentional cascading 등) 측 honest reflect
- 14 honest C3 caveats inline (≥10 raw 10 정합 요구 +4 추가)
- own2 verdict (design 자체): **PASS**

### §4.3 additive at schema level lock-in

- 기존 `.roadmap.*` schema (cond / blk) 측 in-place modification 0건
- 기존 docs / lib / tool / native / tests 측 modification 0건 (read-only reference for schema reuse only)
- 후속 implementation cycle 측 NEW kind (`.checkpoint.<name>`) 측 land — 기존 14 .roadmap.* 측 untouched

## §5 raw#10 honest C3 caveats summary (14 항목)

본 cycle 측 design doc §12 측 inline 14 honest C3 caveats:

- **C1** — design-only land (impl 0건, `.checkpoint.*` file 0건)
- **C2** — D1 cross-roadmap referential integrity 측 verify-time policy 측 deferred
- **C3** — D2 separate SSOT 측 file naming 측 collision risk
- **C4** — D3 cond enum reuse 측 retired_intentional cascading semantics 측 deferred
- **C5** — D4 anima parity break risk (Track M 11/11 → 13-14/??)
- **C6** — aggregation rule mk1 default `all_must_be_met` 측 lock-in pending
- **C7** — D2 separate SSOT directory pollution risk (long-horizon)
- **C8** — anima parity break 측 marker explicit pin 의무
- **C9** — weighted_threshold aggregation 측 phase 2 deferred
- **C10** — closure_orchestrator integration 측 cycle 3 측 dependency
- **C11** — verify_complexity cross-roadmap parse cost (long-horizon, O(N×M))
- **C12** — own1 strict reading vs admin tool lens (Track H/Y/11 precedent 정합)
- **C13** — raw compliance 매 cycle 명시 (raw 9/10/11/12/15/168/175)
- **C14** — rate-limit recovery disk write priority

## §6 cross-link to design-only pending lock-in pattern siblings (Track H/Y/Z)

| track | doc | marker | relation |
|---|---|---|---|
| H | `docs/ROADMAP_OP_ADOPTION_DECISION.md` | `state/markers/airgenome_gamebox_roadmap_op_adoption_decision_landed.marker` | predecessor decision doc — 11 criteria 3-way matrix 직접 mirror, design-only lock-in pattern |
| Y | `docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` | `state/markers/airgenome_gamebox_hexa_cli_runtime_build_decision_landed.marker` | predecessor decision doc — recent 3-way runtime decision matrix sibling |
| Z | `docs/CROSS_TOOL_INTEGRATION_DESIGN.md` | `state/markers/airgenome_gamebox_cross_tool_integration_design_landed.marker` | predecessor design-only cycle posture mirror — in_place_writes=0 cleanest cycle |
| AA | (본 cycle) | (본 marker) | mk2 checkpoint schema design (4 D decision matrix + 7 candidate inventory, design-only) |

## §7 file index

- `docs/MK2_CHECKPOINT_SCHEMA_DESIGN.md` — design doc (NEW, 361 LOC, 13 sections + 14 caveats)
- `airgenome/doc/airgenome_gamebox_mk2_checkpoint_schema_design_landed_2026_05_04.ai.md` — handoff (NEW, this file)
- `state/markers/airgenome_gamebox_mk2_checkpoint_schema_design_landed.marker` — marker (NEW)

## §8 policy summary

- migration: forbidden
- changes: additive_only
- in_place_writes: 0 (cleanest cycle posture mirror Track H/Y/Z)
- destructive_ops: 0
- cost_usd: 0
- substrate: mac-local
- cap_minutes: 90
- br_no_user_verbatim: true
- friendly_preset: true
- silent-land marker: enforced
- own1: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / hexa-native / 외부 의존 0
- own2: design honest / 4 D 결정 매트릭스 explicit / 14 honest C3 caveats inline / silent_swallow 0
- design_only: implementation 0건 (후속 cycle 측 tool/roadmap_op.hexa extension + .checkpoint.* SSOT land + closure_orchestrator integration 측 별도 cycle, 3 cycle minimum-viable / 6 cycle full)

## §9 next cycle paths (사용자 lock-in 후 implementation cycle)

### §9.1 if D1=b / D2=b / D3=a / D4=a (default recommended)

- cycle 1: `tool/roadmap_op.hexa` 측 `checkpoint-status` / `checkpoint-list` / `checkpoint-verify` 3 subcmd 추가 + selftest S12-S14 + anima parity break marker explicit pin
- cycle 2: 첫 3 .checkpoint.* SSOT land (`.checkpoint.d2r_battlenet_offline_play` + `.checkpoint.d2r_battlenet_online_play` + `.checkpoint.lineage_classic_r28_play`)
- cycle 3: closure_orchestrator (Track S) 측 cross_ref OPTIONAL field 추가 (Track Z envelope 정합) — checkpoint count + status snapshot cross-cite
- cycle 4-6: 나머지 4 candidate land + weighted_threshold aggregation rule design + schema validator land

### §9.2 if D4=b (NEW checkpoint_op fallback, anima parity preserve)

- cycle 1: `tool/checkpoint_op.hexa` skeleton + 3 subcmd (status/list/verify) + selftest S1-S3
- cycle 2-3: 동일 (.checkpoint.* SSOT land + closure_orchestrator integration)

### §9.3 if D1=a OR D2=a OR D2=c OR D3=b (alternative options)

- 본 cycle design 측 sub-default scope — 사용자 측 명시 rationale 측 lock-in 시 별도 implementation cycle 측 land

## §10 cross-link

- predecessor handoff: `airgenome/doc/airgenome_gamebox_cross_tool_integration_design_landed_2026_05_04.ai.md` (Track Z, design-only cycle direct sibling)
- predecessor marker: `state/markers/airgenome_gamebox_cross_tool_integration_design_landed.marker`
- design doc (this cycle land): `docs/MK2_CHECKPOINT_SCHEMA_DESIGN.md`
- 14 .roadmap.* family (read-only reference for cond_refs[] sketch source):
  - `.roadmap.diablo2_resurrected` / `.roadmap.diablo4` / `.roadmap.battlenet` / `.roadmap.wow` / `.roadmap.cs2` / `.roadmap.delta_force` / `.roadmap.elden_ring` / `.roadmap.lineage_classic_r28` / `.roadmap.lineage_m` / `.roadmap.lineage_w` / `.roadmap.purple_launcher` / `.roadmap.loader_pe` / `.roadmap.loader_win32` / `.roadmap.loader_dx`
- design doc style precedents (Track H/Y/Z mirror):
  - `docs/ROADMAP_OP_ADOPTION_DECISION.md` (Track H, 11-criteria 3-way matrix)
  - `docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` (Track Y, recent 3-way runtime decision)
  - `docs/CROSS_TOOL_INTEGRATION_DESIGN.md` (Track Z, design-only cycle posture mirror)
- D4 target tools (read-only reference for extension target):
  - `tool/roadmap_op.hexa` (1626 LOC, 11/11 anima parity — D4 (a) extension target)
  - `tool/closure_orchestrator.hexa` (526 LOC — D4 (c) alt branch)

---

*written 2026-05-04 — Track AA mk2 checkpoint schema design handoff. design-only land, additive_only / migration forbidden / in_place_writes=0 / destructive_ops=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker. 4 D decision matrix (D1 cross-roadmap (b) / D2 separate SSOT (b) / D3 cond enum reuse (a) / D4 roadmap_op extend (a)) all confidence MEDIUM (D3 HIGH). 7 candidate checkpoint inventory (cond_refs[] sketch only, NOT actual file emit). implementation cycle plan estimate (3 cycle minimum / 6 cycle full). user lock-in checklist (11 items in design doc §9). 14 honest C3 caveats inline. own1 hexa-native / 외부 의존 0 / own2 design honest swallowed_tradeoff 0. implementation 0건 — 후속 cycle 측 별도 land.*
