# mk2 checkpoint schema design — cross-roadmap intermediate goal expression layer (Track AA, 2026-05-04)

> **status**: design-only doc / 작성 **2026-05-04** / 본 cycle 은 **구현 X**, 4 D 결정 매트릭스 + 권고 + 7 candidate inventory + 사용자 lock-in 대기
> **scope**: 현재 mk2 schema (`.roadmap.<name>` JSONL header with `required_conditions[]` + `blockers[]`) 측 cross-roadmap intermediate goal 표현 layer 부재 → checkpoint mechanism 추가 design freeze
> **결론 (요약, confidence MEDIUM)**: D1 = **(b) cross-roadmap (cond_refs[])** / D2 = **(b) NEW `.checkpoint.<name>` separate SSOT** / D3 = **(a) cond enum reuse (`unmet | partial | met | retired_intentional`)** / D4 = **(a) `tool/roadmap_op.hexa` 2-3 subcmd 추가 (`checkpoint-status` / `checkpoint-list` / `checkpoint-verify`)**. 모두 confidence MEDIUM (own1 정합 + own2 honest matrix).
> **lock-in pending**: 4 D 선택 + 7 candidate checkpoint inclusion/exclusion + raw 168 minimum-viable impl cycle estimate ack
> **predecessor 패턴**: Track H (`docs/ROADMAP_OP_ADOPTION_DECISION.md`, 11-criteria 3-way matrix) + Track Y (`docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md`, 3-way runtime decision) + Track Z (`docs/CROSS_TOOL_INTEGRATION_DESIGN.md`, design-only cycle posture) — 모두 design-only pending lock-in pattern siblings, 본 doc 측 직접 mirror.

---

## §0 목적 / 한계 (purpose / scope freeze)

### §0.1 목적 (in-scope)

본 doc 의 목적은 다음 5 항목으로 한정된다:

1. 현재 mk2 schema 측 cross-roadmap intermediate goal 표현 layer 부재 (gap analysis) 측 narrative SSOT freeze
2. 4 D decision (D1 scope / D2 storage / D3 status enum / D4 tool extension) 측 매트릭스 + 권고 + confidence + tradeoff explicit
3. proposed `.checkpoint.<name>` separate SSOT 측 schema example 측 narrative freeze
4. 7 candidate checkpoint 측 inventory + 각 cond_refs[] 측 sketch (NOT actual `.checkpoint.*` file 생성)
5. own1 (Wine 0 / hexa-native / 외부 의존 0) + own2 (design honest, swallowed tradeoff 0) 정합 reflect

### §0.2 한계 (out-of-scope, 영구)

본 cycle 측 다음 항목은 **영구 out-of-scope** (구현 cycle 측 별도 land):

- 실제 `.checkpoint.*` SSOT file 생성 (7 candidate 측 inventory만, 실 file emit 0건)
- `tool/roadmap_op.hexa` (or 다른 tool) 측 subcommand 추가 modification (D4 권고 = roadmap_op extension, 단 본 cycle 측 source 측 touch 0건)
- `.roadmap.*` family file 측 modification (read-only reference)
- 기존 docs/*.md 측 modification (read-only reference)
- mk2 schema 측 in-place migration (additive layer only — checkpoint 은 cond/blk 와 coexist)
- closure_orchestrator (Track S) 측 modification (D4 option c 측 alt branch 측 narrative 만)

본 doc 은 design-only spec freeze — 후속 implementation cycle 측 본 doc 측 spec 측 정합 land.

---

## §1 current mk2 schema gap analysis

### §1.1 현재 mk2 schema 측 expression surface

본 repo `.roadmap.*` family (14 file: per-game 11 + loader-domain 3) 측 mk2 schema 측 다음 3 layer:

1. **header** — `kind` / `name` / `goal` / `perspective` / `mk` 등 metadata
2. **`required_conditions[]`** — cond.<n> array, status ∈ {unmet | partial | met | retired_intentional}, evidence[] / blocker_reason / verifier 측 single-roadmap-internal goal
3. **`blockers[]`** — blk.<n> array, type / status / status_enum_candidate 측 single-roadmap-internal blocker

### §1.2 cross-roadmap intermediate goal 측 표현 부재

현재 schema 측 다음 형태 측 intermediate goal 측 **자연스러운 단일 expression layer 가 없다**:

- "D2R offline single-player play 가능" — `.roadmap.diablo2_resurrected` cond.1 + cond.2 met + `.roadmap.battlenet` cond.1 + cond.2 met 측 4 cond AND aggregation
- "Lineage W Purple play 가능" — `.roadmap.lineage_w` cond.1 + cond.2 + cond.3 met + `.roadmap.purple_launcher` cond.1 + cond.2 met 측 5 cond AND aggregation
- "Win32 50% coverage milestone" — `.roadmap.loader_win32` cond.1 + cond.2 + cond.3 측 cumulative coverage % 측 milestone (cycle 35 30.34% → 50%)

이 형태 측 cross-roadmap aggregation 측 현재 cycle 측 manual hand-tracking 측 누적:
- handoff doc (`docs/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md` etc) 측 prose narrative 측 cross-cite
- closure_orchestrator (Track S) 측 closure_pct + cm_ratio 측 single global aggregate (vs per-checkpoint specific)
- `.roadmap.<game>` cond.<n> 측 evidence row 측 sister cond cross-cite (e.g., `.roadmap.diablo2_resurrected` cond.1 evidence 측 ".roadmap.battlenet cond.1 met cross-link" 측 hand-edit append)

→ **gap**: cross-roadmap "intermediate goal == subset of cond_refs[] all met" 측 first-class object 측 부재.

### §1.3 본 cycle 측 결정 측 의미

본 cycle 측 checkpoint mechanism 측 design freeze. 4 D 결정 측 사용자 lock-in 후 implementation cycle 측 (a) tool subcmd 추가 + (b) 첫 3 .checkpoint.* SSOT land + (c) closure_orchestrator integration 측 sequencing.

### §1.4 design-only pending lock-in pattern 측 sibling

본 doc 측 Track H / Track Y / Track Z 측 design-only pending lock-in pattern 측 직접 sibling — 11 criteria scoring (or D1/D4 측 multi-option matrix) + 권고 + confidence + fallback + 사용자 lock-in checklist + caveat trail closure pattern 동일.

---

## §2 D1 — checkpoint scope decision matrix

### §2.1 option 측 정의

- **(a) single-roadmap internal**: checkpoint 측 own roadmap 측 cond_ids[] subset aggregation 만. e.g., `.roadmap.diablo2_resurrected` 내부 cond.1 + cond.2 + cond.3 측 own3 aggregate (cross-roadmap X).
- **(b) cross-roadmap (RECOMMENDED)**: checkpoint 측 multiple roadmaps 측 cond_refs[] array (`<roadmap_name>.cond.<n>` qualified id) 측 reference. cross-roadmap intermediate goal expression first-class.

### §2.2 매트릭스 (low=1 / med=3 / high=5; 점수 낮을수록 정합)

| criterion | (a) single-roadmap | (b) cross-roadmap | good direction | 비고 |
|---|---|---|---|---|
| **cross_roadmap_natural** | 5 (high mismatch) | 1 (low mismatch) | 낮을수록 better | (a) D2R-offline = roadmap.battlenet+roadmap.d2r 측 표현 X |
| **single_roadmap_simplicity** | 1 (low) | 3 (med) | 낮을수록 better | (a) own roadmap 내부 reference 측 simple; (b) 측 cross-link 검증 필요 |
| **verify_complexity** | 1 (low) | 3 (med) | 낮을수록 better | (a) own header parse 만; (b) N roadmap parse + cond_id 정합 |
| **expressiveness** | 5 (low) | 1 (high) | 낮을수록 better | (a) D2R+battlenet 같은 합성 goal 측 표현 X; (b) 측 first-class |
| **closure_orchestrator_alignment** | 5 (low) | 1 (high) | 낮을수록 better | closure_pct 측 cross-roadmap aggregate alignment, (b) 정합 |
| **mk2_layer_orthogonality** | 1 (low risk = orthogonal) | 1 (low risk) | 낮을수록 better | 둘 다 cond/blk 와 orthogonal additive layer 측 정합 |
| **own1_alignment (hexa-only / 외부 의존 0)** | 1 (low) | 1 (low) | 낮을수록 better | 둘 다 hexa-native 표현 가능 |
| **own2_alignment (honest aggregate verify)** | 3 (med) | 1 (low) | 낮을수록 better | (a) 측 expressiveness 부족 측 own2 honest gap; (b) 측 verify explicit 가능 |
| **합계 (8 criteria)** | **22** | **12** | — | (b) cross-roadmap = 12 (best) |

### §2.3 권고 — (b) cross-roadmap, confidence MEDIUM

- 매트릭스 합계 22 vs 12 — (b) 측 명확 우위.
- 사용자 directive 측 "D2R + Battle.net 작동" 같은 cross-roadmap intermediate goal 측 first-class expression 측 본 design 측 핵심 motivation — (a) 측 본질적 mismatch.
- confidence MEDIUM (NOT HIGH) 이유: (b) 측 verify_complexity + cross_roadmap referential integrity 측 구현 cycle 측 actual implementation 측 detail 측 결정 측 deferred (cond_refs[] 측 missing roadmap reference 측 verify-time 측 FAIL emit policy 측 lock-in 필요 — caveat C5).

---

## §3 D2 — storage location decision matrix

### §3.1 option 측 정의

- **(a) header internal `checkpoints[]` field**: 각 `.roadmap.<name>` header 측 `checkpoints[]` field 측 추가. checkpoint 측 own roadmap header 측 inline 보관.
- **(b) NEW `.checkpoint.<name>` separate SSOT (RECOMMENDED)**: 별도 file (e.g., `.checkpoint.d2r_battlenet_offline_play`) 측 JSONL header — `.roadmap.<name>` family 와 sibling SSOT.
- **(c) single top-level `.roadmap.checkpoints` aggregate file**: 단일 file 측 모든 checkpoint 측 array 측 보관.

### §3.2 매트릭스

| criterion | (a) header internal | (b) separate SSOT | (c) aggregate file | good direction | 비고 |
|---|---|---|---|---|---|
| **separation_of_concerns** | 5 (low) | 1 (high) | 3 (med) | 낮을수록 better | (a) cond/blk 와 layer 혼재; (b) checkpoint 측 own SSOT; (c) 단일 file 측 grow 시 burden |
| **cross_roadmap_natural** | 5 (low — own roadmap 소속이라 cross-roadmap mismatch) | 1 (high) | 3 (med) | 낮을수록 better | (a) cross-roadmap intermediate goal 측 어느 .roadmap 측 보관 측 결정 burden |
| **add_remove_cycle_friction** | 3 (med) | 1 (low) | 3 (med) | 낮을수록 better | (a) own roadmap header 수정 측 in-place; (b) 측 separate file add/delete; (c) 단일 file in-place |
| **discoverability** | 3 (med) | 1 (high — `ls .checkpoint.*`) | 1 (high) | 낮을수록 better | 비슷한 수준 |
| **mk2_schema_disruption** | 5 (high — header field 추가) | 1 (low — additive new file kind) | 3 (med — new aggregate file) | 낮을수록 better | (a) 측 본 repo policy `migration: forbidden` 측 header 측 in-place modification 측 risk |
| **roadmap_op_tool_extension_alignment** | 3 (med — header parse 측 새 field handle) | 1 (low — new file kind, new subcmd 측 자연) | 3 (med — single aggregate file 측 special-case) | 낮을수록 better | tool extension 측 D4 (a) roadmap_op subcmd 추가 측 (b) 측 정합 |
| **scaling_to_N_checkpoints** | 5 (low — header bloat) | 1 (low — file per checkpoint) | 5 (low — single file grow) | 낮을수록 better | 7 candidate → 추후 ≥ 10 cycle 시 scaling pattern |
| **own1_alignment** | 1 (low) | 1 (low) | 1 (low) | 낮을수록 better | 모두 hexa-native, 외부 의존 0 |
| **own2_alignment (honest separation)** | 3 (med) | 1 (low) | 3 (med) | 낮을수록 better | (b) 측 separation honest 가장 명료 |
| **합계 (9 criteria)** | **33** | **9** | **25** | — | (b) separate SSOT = 9 (best) |

### §3.3 권고 — (b) separate SSOT, confidence MEDIUM-HIGH

- 매트릭스 합계 33 vs 9 vs 25 — (b) 측 압도적 우위.
- `migration: forbidden` policy 측 정합 — (a) 측 header in-place modification 측 risk 측 본 repo policy mismatch.
- discoverability + scaling 측 (b) separate SSOT 측 자연스러운 pattern (per-checkpoint file = `ls .checkpoint.*` 측 즉시 enumerate).
- confidence MEDIUM-HIGH (NOT HIGH) 이유: (b) 측 file 측 수 측 grow 시 (≥ 20 checkpoint 가정) directory pollution 측 minor concern (caveat C7).

---

## §4 D3 — status enum decision

### §4.1 option 측 정의

- **(a) reuse cond enum (RECOMMENDED)**: `unmet | partial | met | retired_intentional` — `.roadmap.*` cond status enum 측 직접 reuse.
- **(b) new enum**: `pending | partial | reached` — checkpoint-specific 3-state 측 별도 enum.

### §4.2 권고 — (a) reuse cond enum, confidence HIGH (rationale-only, matrix 생략)

- **raw 11 alignment**: snake_case + reuse 측 정합. 본 repo cond 측 status_enum_adopted (Track G adoption) 측 정합 reuse — checkpoint 측 동일 enum 측 consistent.
- **roadmap_op verify reuse**: (D4 권고 = roadmap_op extension 시) cond status verify logic 측 그대로 checkpoint status verify 측 reuse — 별도 logic 0건.
- **retired_intentional 측 cross-roadmap 정합**: e.g., `.checkpoint.lineage_classic_r28_play` 측 status 측 `retired_intentional` (GameGuard kernel-AC 영구 차단) 측 자연스러움 — `.roadmap.lineage_classic_r28` cond.<n> 측 retired_intentional cascading 측 정합.
- **partial 측 자연 적용**: 일부 cond_refs[] 만 met / 나머지 unmet 시 checkpoint status = partial 측 자연 — (b) 측 새 enum 측 partial 측 동일 의미 측 기존 enum 측 reuse 측 더 정합.
- (b) 측 단점: 새 enum 측 cross-tool consistency 측 cycle-by-cycle 측 burden + raw 11 reuse mandate 측 mismatch + retired_intentional 측 표현 X.

confidence HIGH (matrix 생략, rationale 측 충분 — single rational answer reached without tradeoff weighting).

---

## §5 D4 — tool extension decision matrix

### §5.1 option 측 정의

- **(a) `tool/roadmap_op.hexa` 2-3 subcmd 추가 (RECOMMENDED)**: 현재 11/11 anima parity → 13-14/?? extension. 새 subcmd: `checkpoint-status` / `checkpoint-list` / `checkpoint-verify` (cross-validate cond_refs[] 측 referential integrity).
- **(b) NEW `tool/checkpoint_op.hexa` separate tool**: roadmap_op pattern 측 mirror 측 별도 tool.
- **(c) extend `tool/closure_orchestrator.hexa` (Track S)**: closure_orchestrator 측 checkpoint progression tracking 측 추가.

### §5.2 매트릭스

| criterion | (a) roadmap_op extend | (b) NEW checkpoint_op | (c) closure_orchestrator extend | good direction | 비고 |
|---|---|---|---|---|---|
| **dev_cost** | 1 (low — 2-3 subcmd 추가) | 5 (high — 별도 tool skeleton + dispatcher + selftest) | 3 (med — closure 측 N subcmd 추가, 단 closure scope 와 mismatch) | 낮을수록 better | (a) 측 11 subcmd dispatcher 측 extension 측 minimum-viable 1 cycle |
| **scope_alignment (`.roadmap.*` family + `.checkpoint.*` 측 sister kind)** | 1 (high) | 3 (med) | 5 (low — closure 측 closure_pct + cm_ratio scope 와 mismatch) | 낮을수록 better | checkpoint 측 .roadmap.* 측 sister kind = roadmap_op 측 자연 |
| **subcmd_count_growth** | 1 (low — 11 → 13-14) | 3 (med — new tool 0 → 3) | 3 (med — closure 3 → 6) | 낮을수록 better | (a) 측 single tool 측 14 subcmd 측 manageable |
| **anima_parity_break_risk** | 5 (high — 11/11 anima parity 측 break) | 1 (low — separate tool) | 1 (low — closure 측 anima 측 sibling X) | 낮을수록 better | anima parity 측 single concern — caveat C8 |
| **test_burden** | 3 (med — 2-3 추가 selftest) | 5 (high — 별도 selftest scaffold) | 3 (med — closure selftest 확장) | 낮을수록 better | |
| **standalone_tool_simplicity** | 3 (med — single tool grow) | 1 (low — focused 별도 tool) | 5 (low — closure scope drift) | 낮을수록 better | (b) 측 single-purpose tool 측 KISS principle |
| **closure_orchestrator_natural_alignment** | 3 (med — closure 측 indirect cross-cite) | 3 (med — closure 측 indirect cross-cite) | 1 (high — direct integration) | 낮을수록 better | (c) 측 closure_pct ↔ checkpoint progression direct mapping 가능 |
| **own1_alignment (hexa-only)** | 1 (low) | 1 (low) | 1 (low) | 낮을수록 better | 모두 hexa-native |
| **own2_alignment (honest extension scope)** | 1 (low — additive subcmd) | 3 (med — separate tool 측 cycle-by-cycle drift) | 5 (low — closure scope drift 측 own2 weak) | 낮을수록 better | (c) 측 closure scope drift 측 own2 weak |
| **합계 (9 criteria)** | **19** | **25** | **27** | — | (a) roadmap_op extend = 19 (best) |

### §5.3 권고 — (a) roadmap_op extend, confidence MEDIUM

- 매트릭스 합계 19 vs 25 vs 27 — (a) 측 명확 우위.
- scope_alignment + dev_cost + own2 측 (a) 측 가장 정합.
- confidence MEDIUM (NOT HIGH) 이유: anima_parity_break_risk = 5 (high) — 본 repo `tool/roadmap_op.hexa` 측 11/11 anima parity 측 본 cycle 측 13-14/?? 측 break (caveat C8). 사용자 측 lock-in 시 명시 ack 측 정합 회복 가능 — anima sibling 측 동일 extension 측 별도 cycle 측 sync 측 long-horizon (or anima 측 본 repo 측 fork-and-extend posture 측 명시 lock-in).
- fallback: anima parity 측 strict preserve 측 lock-in 시 → (b) NEW `tool/checkpoint_op.hexa` separate tool 측 redirect.

---

## §6 proposed schema example — `.checkpoint.d2r_battlenet_offline_play`

### §6.1 file path + identity

- proposed path: `.checkpoint.d2r_battlenet_offline_play`
- proposed JSONL pattern: comment lines + single header object (mirror `.roadmap.*` mk2 pattern)
- proposed `kind`: `checkpoint`
- proposed name pattern: snake_case (raw 11), descriptive (e.g., `d2r_battlenet_offline_play`)

### §6.2 proposed header schema (narrative example, NOT actual file emit in this cycle)

```jsonl
# .checkpoint.d2r_battlenet_offline_play mk1 — cross-roadmap intermediate goal SSOT (peer perspective, additive layer over .roadmap.* cond/blk).
# Header on next line; entries follow.
# Status enum candidates: unmet | partial | met | retired_intentional (mk2 cond enum reuse, D3 권고 a)
{"type":"header","kind":"checkpoint","name":"d2r_battlenet_offline_play","mk":1,"perspective":"peer","goal":"D2R offline single-player play 가능 — Battle.net launcher offline shim 측 D2R.exe launch + MPQ archive parse + offline savegame d2s round-trip 측 cumulative AND aggregation","cond_refs":["diablo2_resurrected.cond.1","diablo2_resurrected.cond.2","battlenet.cond.1","battlenet.cond.2"],"aggregation":"all_must_be_met","status":"partial","progress":{"met_count":3,"partial_count":1,"unmet_count":0,"retired_count":0,"total":4},"evidence":["docs/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md cross-link","docs/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed_2026_05_03.ai.md cross-link"],"blockers":[{"id":"d2r_battlenet_offline_play.blk.1","desc":"D2R cond.1 측 partial 영구 (production binary smoke gated on Battle.net launcher install 측 permanent)","type":"cascading_partial","status":"open","resolution_path":"production smoke gated on user binary install"}],"status":"active","since":"2026-05-04"}
```

### §6.3 핵심 field 측 narrative 측 의미

- `cond_refs[]`: cross-roadmap qualified cond ID 측 array (`<roadmap_name>.cond.<n>` 측 sub-format).
- `aggregation`: checkpoint 측 met 측 결정 rule. `all_must_be_met` (default, AND) or `any_must_be_met` (OR, 후속 lock-in 측 결정).
- `progress`: derived field (cond_refs[] 측 status counts) — verify-time 측 actual cond status 측 cross-fetch 측 fill (구현 cycle 측 결정 측 detail).
- `status`: cond enum reuse (D3 권고 a) — partial / met / unmet / retired_intentional.
- `blockers[]`: checkpoint-level blocker (.roadmap.<name>.blk.<n> 측 cascading 측 reference 측 narrative).

---

## §7 7 candidate checkpoints inventory (cond_refs[] sketches)

### §7.1 inventory table

| # | checkpoint name | cond_refs[] sketch | aggregation | expected status | rationale |
|---|---|---|---|---|---|
| 1 | `.checkpoint.d2r_battlenet_offline_play` | `[diablo2_resurrected.cond.1, diablo2_resurrected.cond.2, battlenet.cond.1, battlenet.cond.2]` | all_must_be_met | partial (3 met + 1 partial) | offline single-player path — 가장 viable, own1 정합 |
| 2 | `.checkpoint.d2r_battlenet_online_play` | `[diablo2_resurrected.cond.1, diablo2_resurrected.cond.2, diablo2_resurrected.cond.3, battlenet.cond.1, battlenet.cond.2, battlenet.cond.3]` | all_must_be_met | partial (4 met + 2 partial 영구) | online realm — Warden L2 long-horizon dependent, partial 영구 |
| 3 | `.checkpoint.lineage_m_purple_play` | `[lineage_m.cond.1, lineage_m.cond.2, lineage_m.cond.3, purple_launcher.cond.1, purple_launcher.cond.2]` | all_must_be_met | partial (Android emul layer 측 permanent boundary) | Android emul layer 측 long-horizon |
| 4 | `.checkpoint.lineage_w_purple_play` | `[lineage_w.cond.1, lineage_w.cond.2, lineage_w.cond.3, purple_launcher.cond.1, purple_launcher.cond.2]` | all_must_be_met | partial (UE4 + Purple cascading) | UE4 D3D11/D3D12 dual renderer + Purple cascading |
| 5 | `.checkpoint.lineage_classic_r28_play` | `[lineage_classic_r28.cond.1, lineage_classic_r28.cond.2, lineage_classic_r28.cond.3]` | all_must_be_met | retired_intentional | GameGuard kernel-mode 영구 차단 (retired_intentional cascading) |
| 6 | `.checkpoint.win32_50pct_coverage` | `[loader_win32.cond.1, loader_win32.cond.2, loader_win32.cond.3]` weighted by coverage % | weighted_threshold (≥ 50%) | partial (cycle 35 30.34%) | cycle 35 → 50% milestone, weighted aggregation 측 별도 lock-in |
| 7 | `.checkpoint.win32_full_coverage` | `[loader_win32.cond.1, loader_win32.cond.2, loader_win32.cond.3]` 측 435/435 fn | weighted_threshold (= 100%) | unmet (long-horizon) | 435/435 long-horizon target |

### §7.2 candidate 측 inclusion/exclusion lock-in 필요 (사용자 §9 checklist)

7 candidate 측 모두 inclusion 측 default — 사용자 측 (a) 모두 inclusion / (b) 일부 exclusion (e.g., #6/#7 weighted aggregation 측 mk1 design 측 too complex 측 phase 2 deferred) / (c) 추가 candidate (e.g., `.checkpoint.diablo4_online_play`) 측 lock-in.

### §7.3 weighted aggregation rule 측 deferred (caveat C9)

#6/#7 측 `weighted_threshold` aggregation 측 mk1 design 측 narrative-level mention 만 — 실 weight 계산 (cond.<n> 측 fn coverage % 측 어떻게 cross-cite) 측 후속 cycle 측 별도 design (caveat C9).

---

## §8 implementation cycle plan estimate (minimum-viable)

### §8.1 minimum-viable 3-cycle plan

| cycle | scope | deliverable | duration |
|---|---|---|---|
| 1 | tool extension (D4 a) | `tool/roadmap_op.hexa` 측 2-3 subcmd 추가 (`checkpoint-status` / `checkpoint-list` / `checkpoint-verify`) + selftest extension (S12+) | 1 cycle (≤ 90min cap) |
| 2 | first 3 .checkpoint.* SSOT land | `.checkpoint.d2r_battlenet_offline_play` + `.checkpoint.d2r_battlenet_online_play` + `.checkpoint.lineage_classic_r28_play` (3 representative coverage) | 1 cycle |
| 3 | closure_orchestrator integration | closure_orchestrator (Track S) 측 checkpoint count + status snapshot cross-cite (additive cross_ref field, Track Z envelope 측 정합) | 1 cycle |
| **total** | | minimum-viable 3 cycle | ≥ 3 cycle |

### §8.2 full impl horizon (≥ 5-6 cycle)

- cycle 4: 나머지 4 checkpoint land (#3 lineage_m / #4 lineage_w / #6 win32_50pct / #7 win32_full)
- cycle 5: weighted_threshold aggregation rule design + impl (caveat C9 measure)
- cycle 6: schema validator (cond_refs[] referential integrity + status_enum coherence) cycle

### §8.3 raw 168 minimum-viable acknowledge

본 cycle 측 design-only (3 file 추가 only) — implementation 측 0건. raw 168 minimum-viable exempt 측 정합. 사용자 lock-in 시 implementation cycle scope 측 명시 (3 cycle minimum-viable / 6 cycle full).

---

## §9 user lock-in checklist (decision pending)

> **상태**: ⏳ awaiting user lock-in

다음 항목 측 사용자 측 명시적 lock-in 필요 — 본 cycle 후속 implementation cycle 시작 전:

- [ ] **D1 scope choice**: (a) single-roadmap / (b) cross-roadmap (default recommend: b)
- [ ] **D2 storage choice**: (a) header internal / (b) separate SSOT / (c) aggregate file (default recommend: b)
- [ ] **D3 status enum choice**: (a) reuse cond enum / (b) new enum (default recommend: a)
- [ ] **D4 tool extension choice**: (a) roadmap_op extend / (b) NEW checkpoint_op / (c) closure_orchestrator extend (default recommend: a)
- [ ] **(if D4 a)**: anima parity break 측 explicit ack — 본 repo `tool/roadmap_op.hexa` 측 11/11 → 13-14/?? 측 break 측 본 repo first-mover lock-in
- [ ] **7 candidate inclusion/exclusion**: 7 checkpoint 측 (a) 모두 inclusion / (b) 일부 exclusion (#6/#7 weighted 측 phase 2 deferred candidate) / (c) 추가 candidate
- [ ] **aggregation rule 측 default**: `all_must_be_met` (AND) — alternative `any_must_be_met` (OR) 측 mk1 default 측 lock-in
- [ ] **weighted_threshold aggregation 측 phase 2 deferred ack**: #6/#7 측 mk1 design 측 narrative mention 만, 실 design 측 후속 cycle (caveat C9)
- [ ] **raw 168 minimum-viable ack**: 본 cycle 측 design-only, implementation 측 후속 cycle (3 cycle minimum / 6 cycle full)
- [ ] **schema migrate forbidden ack**: 본 repo policy `migration: forbidden` 정합 — checkpoint 측 additive layer only, mk2 schema 측 in-place modification 0
- [ ] **closure_orchestrator integration sequencing**: cycle 3 측 closure_orchestrator extension 측 본 design 측 dependent — 사용자 측 sequencing lock-in (cycle 3 deferred OR phase 2)

---

## §10 own1 / own2 alignment

### §10.1 own1 (Wine 0 / hexa-native / 외부 의존 0)

- proposed `.checkpoint.<name>` SSOT 측 JSONL header pattern 측 hexa-native — 외부 format / runtime / orchestrator 의존 0
- proposed roadmap_op extension (D4 a) 측 `r0_common` import 만 (Wine / CrossOver / Whisky / GPTK / community helper 0줄)
- 외부 게임 runtime (Wine) 측 도입 0 — checkpoint 측 admin tool layer (cond aggregation), 게임-실행 layer 측 무관
- own1 verdict (design 자체): **PASS**

### §10.2 own2 (design honest / silent_swallow 0)

- 4 D 결정 측 매트릭스 + 권고 + confidence + tradeoff explicit 측 own2 정합
- swallowed tradeoff 0 — 매 D 측 single recommendation 단 confidence MEDIUM (D3 만 HIGH) 측 honest
- D4 (a) 측 anima_parity_break_risk = 5 (high) 측 single weakness 측 explicit disclosure (caveat C8)
- 7 candidate 측 expected status (partial 영구 / retired_intentional cascading 등) 측 honest reflect
- own2 verdict (design 자체): **PASS**

### §10.3 additive at schema level lock-in

- 기존 `.roadmap.*` schema (cond / blk) 측 in-place modification 0건
- 기존 docs / lib / tool / native / tests 측 modification 0건 (read-only reference for schema reuse only)
- 후속 implementation cycle 측 NEW kind (`.checkpoint.<name>`) 측 land — 기존 14 .roadmap.* 측 untouched

---

## §11 cross-link to predecessors

### §11.1 design-only pending lock-in pattern siblings (Track H/Y/Z)

| track | doc | marker | relation |
|---|---|---|---|
| H | `docs/ROADMAP_OP_ADOPTION_DECISION.md` | `state/markers/airgenome_gamebox_roadmap_op_adoption_decision_landed.marker` | predecessor decision doc — 11 criteria 3-way matrix 직접 mirror, design-only lock-in pattern |
| Y | `docs/HEXA_CLI_RUNTIME_BUILD_DECISION.md` | `state/markers/airgenome_gamebox_hexa_cli_runtime_build_decision_landed.marker` | predecessor decision doc — recent 3-way runtime decision matrix sibling |
| Z | `docs/CROSS_TOOL_INTEGRATION_DESIGN.md` | `state/markers/airgenome_gamebox_cross_tool_integration_design_landed.marker` | predecessor design-only cycle posture mirror — in_place_writes=0 cleanest cycle |

### §11.2 tool pattern predecessors (Track I/M roadmap_op)

| track | doc/file | relation |
|---|---|---|
| I | `state/markers/airgenome_gamebox_roadmap_op_self_impl_landed.marker` | tool/roadmap_op.hexa minimum-viable 5 subcmd land — D4 (a) extension target |
| M | `state/markers/airgenome_gamebox_roadmap_op_full_11_of_11_landed.marker` | tool/roadmap_op.hexa 11/11 anima parity (1626 LOC) — D4 (a) 측 anima parity break risk source |
| S | `tool/closure_orchestrator.hexa` + `docs/CLOSURE_ORCHESTRATOR_DESIGN.md` | D4 (c) alt branch — closure_orchestrator integration target (cycle 3) |

### §11.3 14 .roadmap.* family (cond_refs[] sketch source)

7 candidate checkpoint 측 cond_refs[] sketch 측 14 .roadmap.* family file 측 cond.1/cond.2/cond.3 측 reference. read-only reference, modification 0건. (sources: `.roadmap.diablo2_resurrected` / `.roadmap.battlenet` / `.roadmap.lineage_m` / `.roadmap.lineage_w` / `.roadmap.lineage_classic_r28` / `.roadmap.purple_launcher` / `.roadmap.loader_win32`.)

---

## §12 caveats (≥10 honest items)

**C1 — design-only land**: 본 cycle 측 `.checkpoint.*` SSOT file 0건 / `tool/roadmap_op.hexa` (or 다른 tool) 측 modification 0건 / `.roadmap.*` modification 0건 / 기존 docs/*.md modification 0건. 본 doc 측 design spec freeze, 실 코드 변경 X. 후속 implementation cycle 측 사용자 lock-in 후 land.

**C2 — D1 cross-roadmap referential integrity 측 verify-time policy 측 deferred**: §2 측 (b) cross-roadmap 측 cond_refs[] 측 missing roadmap reference (e.g., `.checkpoint.X` 측 `nonexistent_roadmap.cond.5` 측 reference) 측 verify-time 측 FAIL emit 측 strict policy 측 본 cycle 측 freeze X — 후속 implementation cycle 측 schema validator (referential integrity check) 측 별도 design + impl. 사용자 lock-in 시 명시.

**C3 — D2 separate SSOT 측 file naming 측 collision risk**: §3 측 (b) 측 `.checkpoint.<name>` pattern 측 `.roadmap.<name>` pattern 과 file naming 측 collision X 단 `<name>` namespace 측 cycle-by-cycle 측 conflict avoidance 측 사용자 측 reproducibility 측 검증 필요 (e.g., `.roadmap.battlenet` + `.checkpoint.battlenet_X` 측 prefix-matching tool 측 robust handling).

**C4 — D3 cond enum reuse 측 retired_intentional cascading semantics 측 deferred**: §4 측 (a) reuse 측 retired_intentional 측 checkpoint 측 cascading rule (e.g., `.roadmap.lineage_classic_r28` cond.<n> 측 retired_intentional → `.checkpoint.lineage_classic_r28_play` 측 status 측 자동 retired_intentional?) 측 본 cycle 측 narrative-level only — 실 logic (auto-cascade vs explicit set) 측 후속 implementation cycle 측 결정.

**C5 — D4 anima parity break risk (Track M 11/11 → 13-14/??)**: §5 측 (a) roadmap_op extend 측 본 repo `tool/roadmap_op.hexa` 측 11/11 anima parity 측 break 측 single critical risk. 사용자 측 lock-in 시 explicit ack 필요 — anima sibling 측 동일 extension 측 별도 cycle 측 sync (long-horizon) OR 본 repo first-mover fork-and-extend posture 측 lock-in. (alternative: D4 (b) NEW checkpoint_op separate tool 측 anima parity preserve — fallback 권고.)

**C6 — aggregation rule 측 mk1 default `all_must_be_met` 측 lock-in pending**: §6 + §7 측 default `all_must_be_met` (AND) — alternative `any_must_be_met` (OR) 측 mk1 design 측 freeze X. e.g., `.checkpoint.lineage_m_purple_play` 측 Purple launcher 측 cond.1 (Webview2) OR cond.2 (CLR probe) 측 either-met 측 충분 측 case 측 가능성 — 사용자 측 mk1 default 측 명시 lock-in 필요.

**C7 — D2 separate SSOT 측 directory pollution risk (long-horizon)**: §3 측 (b) 측 file 측 수 측 grow 시 (≥ 20 checkpoint 가정, 후속 cycle) repo root directory 측 14 .roadmap.* + 20+ .checkpoint.* 측 pollution 측 minor concern. mitigation: (i) `.checkpoint/` subdirectory 측 land (file system layout 측 정합), (ii) checkpoint 측 single aggregate `.checkpoint.checkpoints` 측 후속 phase 2 hybrid posture. mk1 측 root-level `.checkpoint.<name>` 측 lock-in.

**C8 — anima parity break 측 marker 측 explicit pin 의무**: D4 (a) 측 land 시 implementation cycle marker 측 `anima_parity_status: broken_at_13_or_14_subcmd_count` field 측 explicit pin 측 own2 정합 회복. 본 caveat 측 implementation cycle 측 enforce.

**C9 — weighted_threshold aggregation 측 phase 2 deferred**: §7 #6/#7 측 `weighted_threshold` aggregation 측 mk1 design 측 narrative mention 만 — 실 weight 계산 (e.g., `loader_win32.cond.<n>` 측 fn coverage % 측 어떻게 cross-cite + 50% threshold 측 어떻게 verify) 측 후속 cycle 측 별도 design. mk1 측 `all_must_be_met` (AND) + `any_must_be_met` (OR) 2 rule 측 first-class, weighted_threshold 측 phase 2 lock-in.

**C10 — closure_orchestrator integration 측 cycle 3 측 dependency**: §8 cycle 3 측 closure_orchestrator (Track S) 측 checkpoint progression cross-cite 측 본 design 측 dependent. closure_orchestrator 측 modification 측 본 cycle 측 0건 — 후속 implementation cycle 측 `additive_only` posture 측 정합 (cross_ref OPTIONAL field extension, Track Z envelope precedent). 사용자 측 cycle 3 deferred (phase 2) lock-in 시 minimum-viable 측 cycle 1 + cycle 2 만 lock-in 가능.

**C11 — verify_complexity 측 cross-roadmap parse cost (long-horizon)**: §2 + §5 측 cross-roadmap verify (cond_refs[] 측 N roadmap 측 parse + cond_id status 측 fetch) 측 N=14 .roadmap.* + N_checkpoints≥20 측 long-horizon 측 verify-time 측 O(N×M) cost. mitigation: caching (verify run 측 cond status snapshot 측 ephemeral cache) — implementation cycle 측 결정 deferred.

**C12 — own1 strict reading vs admin tool lens**: checkpoint 측 admin tool layer (cond aggregation) — 게임-실행 layer 측 무관. own1 (Wine 0 / 외부 게임 runtime 0) 측 strict reading 측 violation X. Track H + Track Y + Track 11 (battlenet_bypass_own1_disambiguation) precedent 측 정합 — admin tool 측 own1 측 분리 적용 측 본 design 측 동일 lens.

**C13 — raw compliance 매 cycle 명시 (raw 9/10/11/12/15/168/175)**: 본 cycle 측 design doc + handoff + marker = 3 file 추가 — `.checkpoint.*` 측 emit 0건 / tool modification 0건. raw 9 (audit-only no impl emit) + raw 10 (≥10 caveats) + raw 11 (snake_case) + raw 12 (silent_error_ban — 매 D 결정 explicit) + raw 15 (env_lazy — repo-relative paths) + raw 168 (minimum-viable exempt for design cycle) + raw 175 (BR-NO-USER-VERBATIM, paraphrase only) 정합.

**C14 — rate-limit recovery 측 disk write priority**: 본 cycle 측 BG subagent 측 rate-limit recovery 시점 측 disk write 측 priority — report 측 deferred 가능. 본 doc + 후속 handoff doc + marker 측 disk land 측 cycle 측 success criteria. (Track H + Track Y + Track Z precedent 동일.)

---

(design doc end. § 0-12, 4 D decision matrix (D1 8-criteria + D2 9-criteria + D3 rationale-only + D4 9-criteria), 7 candidate checkpoint inventory, 14 caveats, recommendation D1=b / D2=b / D3=a / D4=a all confidence MEDIUM (D3 HIGH), lock-in 측 사용자 대기. predecessor Track H/Y/Z design-only pending lock-in pattern 측 closure 측 본 doc 측 land 측 단계 도달.)
