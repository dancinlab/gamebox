---
schema: airgenome-gamebox/docs/d4_retired_intentional_adoption_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker
  roadmap_files_modified:
    - .roadmap.diablo4
  docs_created:
    - docs/D4_MANJEOM_REVIEW.md
  predecessor_handoffs:
    - docs/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
    - docs/airgenome_gamebox_rank_a_3_domain_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_per_game_roadmaps_landed_2026_05_03.ai.md
status: LANDED_D4_RETIRED_INTENTIONAL_ADOPTION
related_raws:
  - raw 9    # hexa-only orchestration (audit-only, no impl emitted)
  - raw 10   # honest C3 caveats inline
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 15   # env() lazy + <user> placeholder
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes in this doc)
preserved_unchanged:
  - 10 of 11 .roadmap.<game> files (only .roadmap.diablo4 modified, additive field-level edit)
  - 3 existing .roadmap.<domain> files (loader_pe, loader_win32, loader_dx)
  - all hexa modules under lib/{loader,perf} (loader 414 + perf 241 = 655 .hexa, includes 27 c_d4_* perf untouched)
  - all native sources, tests, tool/, plugin.json, entry, README.md, .gitignore
  - 3 predecessor markers
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 1   # .roadmap.diablo4 field-level append + status_enum_adopted add (additive at field-level)
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
own:
  own1: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 — hexa-only, no DRM bypass, no always-online evasion
  own2: log/error/emit/headless honest documentation
---

# airgenome-gamebox D4 retired_intentional 정식 채택 land - mk1 narrative spec freeze (Track A2)

## TL;DR

predecessor 11 .roadmap.<game> per-game fan-out (2026-05-03) 위 D4 (Diablo 4) `retired_intentional` status enum
정식 채택 + mk1 narrative spec freeze 측 land. always-online DRM 정의상 own1 (Wine 0) 측 정합 차단 = honest
documentation = PASS-equivalent posture.

- `.roadmap.diablo4` blk.1 측 `status_enum_adopted: retired_intentional` 신규 add + `retired_intentional_posture:
  block_acknowledged_pass_equivalent` 신규 add (additive at field-level).
- 3 cond.N evidence array 측 `block_acknowledged_pass_equivalent_per_own1_wine_0_no_drm_evasion` row append (3 cond
  모두).
- 신규 `docs/D4_MANJEOM_REVIEW.md` mk1 narrative spec freeze land — sibling docs (CS2/DF/ER) style/length 정합.
- 본 cycle 은 Track A2 (D4 단독), Track B (lineage_classic_r28 retired_intentional 채택) 와 병렬 동시 land.
- DRM bypass 코드 0건, always-online 진입 시도 0건, 27 c_d4_* perf module 측 self_test() 추가 0건 (별도 future cycle).
- 새 doc 1 + 새 marker 1 + 1 .roadmap field-level edit = **2 file 추가 + 1 file additive edit**, 기존 file 0건
  destructive 수정.

## §1 user_directive_paraphrase

사용자 directive (paraphrase only, no verbatim) — predecessor 11 .roadmap.<game> per-game fan-out 측
`retired_intentional_enum_application_5_games` array 측 5 게임 중 D4 (Diablo 4) 측 first-mover 정식 채택 작업.
D4 측 always-online DRM 정의상 own1 (Wine 0) 측 정합 차단 = honest documentation = PASS-equivalent posture.
DRM bypass 0건, always-online 진입 시도 0건, 27 c_d4_* perf module 측 self_test 추가 0건 (별도 future cycle scope).
Track B (lineage_classic_r28 retired_intentional) 측 병렬 동시 land. Track A (battlenet + d2r) 측 별도 cycle (offline
shim 가능 측 retired_intentional 미적용).

## §2 retired_intentional adoption table (before / after)

### §2.1 `.roadmap.diablo4` blk.1 — adoption table

| field | before (predecessor 2026-05-03) | after (본 cycle) |
|---|---|---|
| `id` | `diablo4.blk.1` | (유지) |
| `desc` | "...status `retired_intentional` 정식 채택 권고..." | "...status `retired_intentional` 정식 채택 (block-acknowledged honest documentation = PASS-equivalent posture, own1 Wine 0 + own2 honest emit, no DRM evasion). docs/D4_MANJEOM_REVIEW.md mk1 narrative spec freeze land 시점 정식 adopted." |
| `type` | `upstream_drm` | (유지) |
| `blocker_class` | (없음 — header 측 `blocker_class: always_online_drm` 만) | **`always_online_drm` (blk.1 측 add)** |
| `status` | `open` | (유지) |
| `eta` | `""` | (유지) |
| `resolution_path` | "(a) D4 DRM honest spec freeze documentation land + status enum `retired_intentional` 사용자 lock-in / (b) D4 always-online 영구 boundary 정합 (own1 Wine 0 + own2 honest emit)" | "(a) docs/D4_MANJEOM_REVIEW.md mk1 narrative spec freeze land 정식 adopted (현 cycle 완료) / (b) D4 always-online 영구 boundary 정합 (own1 Wine 0 + own2 honest emit, no DRM bypass code)" |
| `status_enum_candidate` | `retired_intentional` | (유지) |
| `status_enum_adopted` | (없음) | **`retired_intentional` (신규 add)** |
| `retired_intentional_posture` | (없음) | **`block_acknowledged_pass_equivalent` (신규 add)** |
| `adoption_evidence` | (없음) | **["docs/D4_MANJEOM_REVIEW.md", "state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker"] (신규 add)** |
| `adopted_iso` | (없음) | **`2026-05-03` (신규 add)** |

### §2.2 `.roadmap.diablo4` 3 cond.N — evidence array append summary

| cond | before evidence count | after evidence count | append rows |
|---|---:|---:|---|
| `diablo4.cond.1` | 4 | **5** | `block_acknowledged_pass_equivalent_per_own1_wine_0_no_drm_evasion` |
| `diablo4.cond.2` | 4 | **5** | `block_acknowledged_pass_equivalent_per_own1_wine_0_no_drm_evasion` |
| `diablo4.cond.3` | 2 | **4** | `block_acknowledged_pass_equivalent_per_own1_wine_0_no_drm_evasion` + `docs/D4_MANJEOM_REVIEW.md mk1 narrative spec freeze land` |

3 cond.N status field 자체는 `partial` 유지 (변경 X) — block-acknowledged posture 측 evidence row append only.
blocker_reason 측 retired_intentional 정식 채택 명시 reflect.

### §2.3 file size delta

| metric | before | after | delta |
|---|---:|---:|---:|
| `.roadmap.diablo4` bytes | 5498 | 6354 | +856 |
| `.roadmap.diablo4` LOC | 5 | 5 | 0 (JSONL header line 1 + comment lines 4 유지) |
| `.roadmap.diablo4` sha256 | `556ec0d600e22ffc0a77c5b0b76fd46718e3c06f08b72ff22d75a468638c73bc` | `df21dbb286e9bec05a29ec964aff2455026d70bc119856550d4abede98a82efa` | (변경) |

## §3 cond evidence annotations summary

3 cond.N 측 block-acknowledged 명시 패턴:

| cond | block 사유 (transitive) | annotation 추가 |
|---|---|---|
| `cond.1` (D4.exe PE+IAT+CRT smoke) | `loader_pe blk.1 D4 always-online class transitive` (Battle.net launcher 통과 필요) | `block_acknowledged_pass_equivalent_per_own1_wine_0_no_drm_evasion` |
| `cond.2` (D3D12+DXR PSO+ray-trace) | `loader_dx cond.2 partial transitive` (D3DMetal binary licence + D4 always-online cross-blocked) | `block_acknowledged_pass_equivalent_per_own1_wine_0_no_drm_evasion` |
| `cond.3` (27 c_d4_* perf cluster spec freeze) | live game state 측 always-online 의존 | `block_acknowledged_pass_equivalent_per_own1_wine_0_no_drm_evasion` + `docs/D4_MANJEOM_REVIEW.md mk1 narrative spec freeze land` |

→ 3 cond 모두 `partial` status 유지 (block-acknowledged honest documentation 측 evidence row append, status field 측 변경 X).

## §4 mk1 narrative summary (`docs/D4_MANJEOM_REVIEW.md`)

신규 `docs/D4_MANJEOM_REVIEW.md` mk1 narrative spec freeze 측 land. sibling docs (CS2 / DF / ER) 와 동일 style/length:

| 비교 축 | CS2 | DF | ER | **D4 (본 cycle)** |
|---|---:|---:|---:|---:|
| LOC | 314 | 366 | 403 | **337** |
| sections (top-level) | 5 + 0.5 정정 | 6 + 0.5 정정 | 6 + 0.5 정정 | **9 + 0.5 정정** |
| anti-cheat / DRM type | VAC user-mode | EAC kernel-mode | EAC anti-tamper kernel | **always-online DRM** |
| retired_intentional 정식 채택 | (후보 명시만) | (후보 명시만) | (후보 명시만) | **정식 채택 (본 cycle first-mover)** |

D4_MANJEOM_REVIEW.md sections (9 + 0.5):
- §0 게임 메타데이터 (alias D4, Battle.net, DX12+DXR, always-online)
- §0.5 정정 / 추가 (본 cycle retired_intentional 채택 정정)
- §1 6 phase 만점 가능성 매트릭스 (D2R/D4 모델 재사용)
- §2 always-online DRM 차단 위험 평가 (own1 정합 측면)
- §3 closure path B 호환성 (block 가정 시 95%, 정식 채택 시 0)
- §4 27 c_d4_* perf module cluster (이미 land, spec only)
- §5 retired_intentional 정식 채택 rationale
- §6 downstream Battle.net launcher relationship (cross-link)
- §7 cross-link to peer D2R offline mode (peer game)
- §8 결론 — 우선순위
- §9 출처 (WebSearch + 본 repo 자료 종합)

만점 axis A/B/C 측 reflect:
- A (Apple-only stack): 5 — Apple-only hexa PE loader, Wine 0, D3DMetal stage2 활용
- B (Runtime — gameplay 중 효과): 5 / 0 (block 가정 시 5, retired_intentional 정식 채택 시 measurable 0)
- C (데이터재해석 — raw → cache/filter/forecast): 5 (27 c_d4_* perf cluster 측 spec only)

## §5 cross-link to Track A battlenet/d2r cycle (concurrent landing 측 caveat)

본 cycle (Track A2 D4 단독 retired_intentional 정식 채택) 과 병렬 (concurrent) 진행:

| Track | scope | 본 cycle 측 acknowledge | 본 cycle 측 file 수정 |
|---|---|---|---|
| **Track A** | `.roadmap.battlenet` + `.roadmap.diablo2_resurrected` (offline shim path 측 cond.N 측정 진행) | concurrent landing 존재 | **0 (file scope 분리 — 본 cycle 측 untouched)** |
| **Track A2 (본 cycle)** | `.roadmap.diablo4` retired_intentional 정식 채택 + mk1 narrative spec freeze | (본 cycle) | `.roadmap.diablo4` + `docs/D4_MANJEOM_REVIEW.md` |
| **Track B** | `.roadmap.lineage_classic_r28` retired_intentional 정식 채택 + GameGuard mk1 narrative | concurrent landing 존재 | **0 (file scope 분리 — 본 cycle 측 untouched)** |

**concurrent landing 측 caveat**:
- Track A 측 `state/markers/airgenome_gamebox_battlenet_d2r_*.marker` (가칭 — 실 hash 미 unknown, Track A 완료 후 확정)
- Track B 측 `state/markers/airgenome_gamebox_lineage_classic_r28_retired_intentional_adoption_landed.marker` (가칭 — 실 hash 미 unknown, Track B 완료 후 확정)
- 본 cycle 측 marker 측 Track A / Track B 측 marker 측 cross-reference placeholder 측 명시 (실 hash 측 future cycle 측 reconciliation)

cross-link from `docs/D4_MANJEOM_REVIEW.md` §6/§7 측 Track A/B 측 reference:
- §6 (downstream Battle.net launcher relationship): D4 측 retired_intentional 정식 채택 = Battle.net launcher 측 retired_intentional 채택 여부와 **독립적** (Track A scope 별도 결정)
- §7 (peer D2R offline mode): D2R 측 offline mode 정식 path = D4 측 always-online retired_intentional posture 와 **상호 보완 (peer relationship)** — 본 repo design decision

## §6 raw#10 honest C3 (≥6 caveat)

C1 — 본 cycle 은 D4 단독 retired_intentional 정식 채택 + mk1 narrative spec freeze 만. 27 c_d4_* perf module 측
self_test() validation 측 추가 0건 (별도 future cycle scope, retired_intentional posture 측 synthetic-data only).
live game state 측 측정 0건 (always-online 진입 X).

C2 — `status_enum_adopted: retired_intentional` field 측 first-mover 정식 도입 (predecessor `status_enum_candidate`
측 adopted 분리). 향후 5 게임 (D4 / CS2 / DF / ER / LC R28) 별 정식 채택 cycle 측 동일 schema 측 사용 권고.
mk2 schema spec freeze 측 정식 채택은 별도 cycle 결정 (rank A 3 도메인 land 시 동일 caveat 누적).

C3 — `retired_intentional_posture: block_acknowledged_pass_equivalent` 측 first-mover 정식 도입. 정의 = "차단
사유 명시 + own1 (Wine 0) + own2 (honest emit) + no DRM evasion". 향후 4 게임 (CS2 VAC / DF EAC kernel /
ER EAC anti-tamper / LC R28 GameGuard) 별 동일 정의 vs 다른 정의 (예: `block_acknowledged_pass_equivalent_per_anti_cheat`)
측 분기 가능성 caveat.

C4 — concurrent Track A / Track B 측 marker hash 측 본 cycle 측 placeholder 만 (실 hash 측 Track A / Track B
완료 후 reconciliation 필요). Track A 측 file scope 분리 (`.roadmap.battlenet`, `.roadmap.diablo2_resurrected`,
`lib/loader/pe_battlenet_*`, `lib/loader/pe_d2r_*`) + Track B 측 file scope 분리 (`.roadmap.purple_launcher`,
`.roadmap.lineage_*`, `lib/perf/c_purple_*`, `lib/loader/pe_lineage_*`) — 본 cycle 측 untouched.

C5 — `.roadmap.diablo4` 측 in-place edit (3 cond evidence array append + blocker field add) = 정의상 `additive
at field-level` 이지만, file-level 측 sha256 변경 (5498 → 6354 bytes, sha256 556ec0... → df21db...). predecessor
`state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` 측 명시한 D4 sha256 (556ec0d600e22ffc0a77c5b0b76fd46718e3c06f08b72ff22d75a468638c73bc)
는 본 cycle 측 변경 reflect — 본 cycle marker 측 새 sha256 (df21dbb286e9bec05a29ec964aff2455026d70bc119856550d4abede98a82efa)
pin.

C6 — `docs/D4_MANJEOM_REVIEW.md` mk1 narrative 측 출처 (WebSearch + 본 repo 자료 종합) 일부 추정 (D4
always-online 측 server-side validation bypass 가능성 community 보고 등). 본 repo 측 우회 코드 emit 0,
실 D4.exe 분해 X — skeleton-stage spec only, validated 0.

C7 — 27 c_d4_* perf module 측 만점 (15) 후보 spec only 평가는 본 doc §4 측 추정. live game state 측정 X 측
**validated 만점 0** (always-online retired_intentional posture 측 정합). future cycle 측 self_test() validation
시 synthetic-data only (live game state 측 측정 X 측 정합).

C8 — D4 측 retired_intentional 정식 채택은 D2R (peer game) 측 offline mode 정식 path 와 **명확 분리** (본 doc
§7 측 peer relationship). 향후 D2R 측 cond.N 측정 진행 (Track A scope) 측 D4 측 retired_intentional posture 와
독립적 진행 — 본 repo design decision (Blizzard D4 always-online 정식 정책 + D2R offline mode 정식 정책).

## §7 next-cycle hooks

1. **27 c_d4_* perf module 측 self_test() synthetic-data validation cycle** (별도 — retired_intentional posture
   측 정합, live game state 측정 X) — 27 module × self_test() 추가 = skeleton-stage validated 27/27 PASS 목표.
2. **`.roadmap.battlenet` retired_intentional 채택 여부 결정 cycle** (Track A 후속) — D4 측 retired_intentional
   정식 채택 (본 cycle) 와 독립적, Battle.net 측 offline shim 가능 측 cond.1/cond.2 PASS 가능 → 미적용 권고.
3. **`.roadmap.wow` retired_intentional 채택 여부 결정 cycle** (D4 always-online family precedent 적용 — predecessor
   marker 측 후보 명시).
4. **`CS2 / DELTA_FORCE / ELDEN_RING` 3 anti-cheat 측 retired_intentional 정식 채택 여부 결정 cycle** (현재는
   후보 명시 만, 본 cycle 측 D4 측 first-mover 정식 채택 후 sibling 4 게임 측 동일 schema 적용).
5. **`LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` mk1 narrative spec freeze land** (Track B 병렬 — 본 cycle 측 동시
   landing 존재, 실 hash reconciliation 후속).
6. **mk2 status enum spec freeze 측 `retired_intentional` 정식 schema 등록 cycle** (rank A 3 도메인 land 시
   동일 caveat 누적 해소) — `tool/roadmap_op.hexa` 측 enum validator 추가 결정.
7. **본 cycle 측 marker 측 Track A / Track B 측 marker hash placeholder reconciliation cycle** (Track A / B 완료
   후) — 본 cycle marker 측 cross_reference field 측 update.

## §8 file index (sha-pin at land time)

| path | type | size_b | LOC | sha256_hex |
|---|---|---:|---:|---|
| .roadmap.diablo4 (modified) | roadmap_jsonl | 6354 | 5 | `df21dbb286e9bec05a29ec964aff2455026d70bc119856550d4abede98a82efa` |
| docs/D4_MANJEOM_REVIEW.md (created) | mk1_narrative_doc | 25606 | 337 | `732eadf4f7bc9b0a583f213fe947e18d30a0979e4500e4c5b6a3fef5037b50c8` |
| docs/airgenome_gamebox_d4_retired_intentional_adoption_landed_2026_05_03.ai.md (created — 본 doc) | handoff_doc | TBD | TBD | (set after write) |
| state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker (created) | marker | TBD | TBD | (self-referential — sha256 not pinned in own body) |

(`.roadmap.diablo4` 측 predecessor sha256: `556ec0d600e22ffc0a77c5b0b76fd46718e3c06f08b72ff22d75a468638c73bc`)

## §9 policy summary

- migration: forbidden — 0건 emit
- additive only — `.roadmap.diablo4` 측 in-place edit 은 field-level additive (3 cond evidence row append + blocker
  field add), file 자체 destructive overwrite X. docs/D4_MANJEOM_REVIEW.md + 본 doc + marker = 3 NEW only.
- destructive ops — 0건
- in-place writes — 1 (`.roadmap.diablo4`, additive at field-level)
- substrate — mac-local
- cost — $0
- cap — 90min (실제 소요 ≪ cap)
- raw 9 hexa-only orchestration — 본 audit 자체는 hexa orchestrator 미사용 (single-doc + 1 JSONL field-level edit
  + 1 marker emit, predecessor 와 동일 raw 168 minimum-viable exempt)
- raw 12 silent-error ban — `.roadmap.diablo4` blk.1 측 status `open` 유지 + status_enum_adopted 측 명시,
  3 cond.N 측 status `partial` 유지 + block-acknowledged evidence row 명시 honest
- raw 15 env() lazy + <user> — 모든 path repo-relative, 사용자 절대 path X
- raw 175 BR-NO-USER-VERBATIM — 사용자 prompt 직접 인용 0건 (paraphrase only)
- friendly preset — handoff doc only (사용자 응답 X — BG subagent → 메인 monitor)
- silent-land — marker 측 silent_land 명시
- own1 — Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 — hexa-only, no DRM bypass, no always-online evasion
- own2 — log/error/emit/headless honest documentation (`docs/D4_MANJEOM_REVIEW.md` 측 retired_intentional posture
  명시 + `.roadmap.diablo4` blk.1 측 status_enum_adopted + retired_intentional_posture 명시)

## §10 sibling reference (predecessor 비교)

| 비교축 | predecessor mk2 (2026-05-02) | rank A 3 (2026-05-03) | per-game 11 (2026-05-03 fan-out) | **D4 retired_intentional (본 cycle 2026-05-03)** |
|---|---|---|---|---|
| `.roadmap.*` files added | 0 (spec only) | 3 | 11 | **0 (1 file in-place additive edit)** |
| handoff doc | 1 | 1 | 1 | **1** |
| marker | 1 | 1 | 1 | **1** |
| docs/*.md added | 0 | 0 | 0 | **1 (mk1 narrative)** |
| status enum proposal/adoption | (none) | `retired_intentional` 후보 명시 | `retired_intentional` 5 게임 후보 (D4/CS2/DF/ER/LC R28) | **`retired_intentional` D4 first-mover 정식 채택** |
| triplet_audit_tier | (n/a) | T1_large + T1 + T1 | (per-game tier 미명시) | (per-game tier 미명시) |
| cost / cap | $0 / 60min | $0 / 60min | $0 / 90min | **$0 / 90min** |
| concurrent tracks | (none) | (none) | (none) | **Track A (battlenet+d2r) + Track B (lineage_classic_r28) 측 file scope 분리** |
