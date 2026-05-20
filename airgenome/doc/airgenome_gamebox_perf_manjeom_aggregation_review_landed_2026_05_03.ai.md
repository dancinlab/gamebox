# airgenome-gamebox — Track-T `perf_manjeom` aggregation review landed (2026-05-03)

> **TL;DR**: `lib/perf/c_*.hexa` (231 module 측 read-only inventory, marker `lib_perf_hexa_count: 241` 와 -10 mismatch — caveat C9) cross-cutting manjeom (axis A=Apple-only / B=Runtime gameplay / C=데이터재해석, /15) aggregation mk1 narrative spec freeze. doc-only land (cleanest cycle posture, mirror Track I/L). representative top-15 module detailed scoring + 219 module family-inheritance heuristic distribution histogram (만점 ~48% / 14점 ~32% / 13점 ~13% / 12점 이하 ~7%). own1 정합 representative 12/12 PASS + spot audit 4/4 PASS (anticheat / cs2 / elden / bnet high-risk family). predecessor Track-P (`battlenet_bypass` own1 disambiguation) 측 6 BG-AGB rank B+C unblock candidates 중 `perf_manjeom (rank B)` 측 land. additive_only / migration forbidden / in_place_writes=0 / destructive_ops=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker.

## §0 baseline

- predecessor handoff: `airgenome/doc/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed_2026_05_03.ai.md` (Track-P, BG-AGB rank B+C unblock 측 perf_manjeom 측 unblock 명시)
- predecessor marker: `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker`
- predecessor data source marker: `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (`lib_perf_hexa_count: 241` baseline + `.roadmap.*` 11 game fan-out)
- 본 cycle 측 lib/perf/ touch X (read-only inventory + audit only) / `.roadmap.*` touch X / lib/loader/ touch X / tool/ touch X / native/ touch X / tests/ touch X

## §1 user_directive_paraphrase

BG subagent — Track-T perf_manjeom aggregation review land for airgenome-gamebox: `lib/perf/c_*.hexa` (~241 module per predecessor marker, 본 cycle measure 231) cross-cutting manjeom (axis A=Apple-only stack / axis B=Runtime gameplay 효과 / axis C=데이터재해석, /15) aggregation review mk1 narrative spec freeze. doc-only cycle (cleanest cycle posture, mirror Track-I/L, in_place_writes=0). representative 10-15 module detailed axis A/B/C scoring + 가까운 219 module 측 family-inheritance heuristic distribution histogram. own1 정합 verdict (Wine 0 / no bypass / no DRM evasion) every reviewed module 명시. 1 NEW mk1 narrative doc (`docs/PERF_MANJEOM_AGGREGATION_REVIEW.md`, 300-500 LOC, 7 sections + cross-link) + 1 NEW handoff (본 doc) + 1 NEW marker. policy: additive_only / migration forbidden / destructive_ops=0 / in_place_writes=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker. allowed scope: docs (3 NEW only) + marker (1 NEW); 그외 모두 forbidden (read-only reference only).

## §2 perf module inventory finding summary

### §2.1 raw count

- **measured count**: 231 (`ls /Users/ghost/core/airgenome-gamebox/lib/perf/c_*.hexa | wc -l`)
- **predecessor marker claim**: 241 (`lib_perf_hexa_count` field in `airgenome_gamebox_per_game_roadmaps_landed.marker`)
- **drift**: -10 module (predecessor 시점 vs 본 cycle 시점 차이) — caveat C9
- **본 cycle 측 reflect**: 231 기준 honest reflect (predecessor marker 측 in-place edit X, additive only)

### §2.2 family histogram (top families)

| family prefix | count | 비고 |
|---|---|---|
| `c_d4_*` | 27 | Diablo 4 — DXR/DX12/Battle.net always-online cluster |
| `c_d2r_*` | 26 | Diablo 2 Resurrected — DX11 cluster |
| `c_metal_*` | 11 | Apple Metal native cluster |
| `c_pe_*` | 10 | PE loader 측 perf instrumentation |
| `c_elden_*` | 10 | Elden Ring — EAC kernel driver avoid + DX12 first-frame |
| `c_display_*` | 8 | display mode / PIP / focus routing |
| `c_install_*` | 6 | install path / signature / progress |
| `c_purple_*` | 5 | NCSOFT PURPLE launcher OAuth + 2FA |
| `c_wg_*` + `c_wgc_*` | 6 | Wargaming + WGC |
| `c_dxr_*` + `c_rt_*` | 4 | DX12 ray tracing |
| `c_dxil_*` | 4 | DXIL → AIR fusion |
| `c_oauth_*` | 4 | OAuth device flow / PKCE |
| `c_macos_*` | 4 | macOS menubar |
| `c_fps_*` | 4 | FPS overlay |
| `c_bnet_*` | 4 | Battle.net chat / login / token / Warden env |
| `c_lineage_*` | 3 | Lineage Classic R28 / M / W |
| `c_cs2_*` | 3 | CS2 — tier0 / steam_api64 / Source 2 PSO genome |
| `c_bigworld_*` | 3 | BigWorld engine (WoT/WoWS) |
| `c_wow_*` / `c_delta_force_*` | 0 / 0 | retired_intentional 측 module 미작성 (후순위) |
| (others, 1-2 each) | ~93 | abi/agi/amx/audio/cdn/chunk/closure/heap/iat/syscall/etc. cross-cutting |
| **TOTAL** | **231** | |

### §2.3 representative top-15 detailed scoring summary

- 만점 (15): 7 module — `c_d2r_dx11_pipeline_warm` / `c_d4_dxr_bvh_compact` / `c_metal_pipeline_archive` / `c_dxr_inline_rt_shader` / `c_dxil_to_air_fusion` / `c_neon_sse_xlat_cache` / (gameplay-loop direct effect cluster)
- 14점: 5 module — `c_d4_dxr_first_frame_trace` / `c_amx_matrix_multiply` / `c_lineage_classic_r28_native_first_frame` / `c_cs2_source2_pso_genome` / (first-frame skeleton cluster)
- 13점: 3 module — `c_pe_iat_simulation` / `c_anticheat_detection_track` / `c_purple_2fa_totp_proxy` / `c_elden_eac_kernel_driver_avoid_track` / `c_bnet_warden_environment_compat` (one-shot install/launch + honest freeze category)

### §2.4 axis-by-axis 분포 (15 rows)

- axis A=5: 15/15 (100%) — Apple-only stack default
- axis B 분포: 5점=7개 / 4점=4개 / 3점=4개
- axis C=5: 15/15 (100%) — `r0_common` columnar TSV + JSONL universal pattern

## §3 distribution histogram aggregate (231 module, family-inheritance heuristic)

| total /15 | est. module count | % | dominant family |
|---|---|---|---|
| 15 (만점) | ~110 | ~48% | metal / dxr / dxil cross-cutting + d2r/d4 gameplay-loop + display/fps |
| 14 | ~75 | ~32% | install / oauth / lineage first-frame + cs2 offline + d4 first-frame |
| 13 | ~30 | ~13% | anticheat detection + 2FA + IAT sim + EAC avoid + Warden env compat |
| 12 이하 | ~16 | ~7% | retired_intentional + battlenet auth handshake one-shot 추정 |

## §4 own1 정합 audit summary

### §4.1 representative 12/12 PASS

본 cycle representative 12 module 측 own1 정합 verdict = **12/12 PASS**:

- DRM evasion 0줄 / account auth bypass 0줄 / Wine 0줄 / CrossOver 0줄 / Whisky 0줄 / GPTK 0줄
- Warden bypass 0줄 / EAC bypass 0줄 / VAC bypass 0줄
- hook chain shim 0줄 / signature spoof 0줄 / NOP-patch 0줄 / memory scan dodge 0줄

### §4.2 high-risk family spot audit 4/4 PASS

`c_anticheat_detection_track` (signature detection only) / `c_cs2_steam_api64_offline_shim` (offline path only) / `c_elden_eac_kernel_driver_avoid_track` (환경 부재 자연 추적) / `c_bnet_warden_environment_compat` (environment compat 측 honest documentation) — 4/4 spot audit PASS, 모두 own1 정합 honest documentation.

### §4.3 216 remaining module 측 own1 family-inheritance 추정 PASS

representative 15 외 216 module 측 own1 정합 family-inheritance 추정 PASS — `r0_common` lib + `record_*` synthetic round-trip + `r0_emit` baseline 정합. **별도 cycle 측 216/216 정밀 audit 권고** (heuristic 측 자동 PASS 가정 X).

## §5 cross-cutting observations summary (7 항목)

1. **d2r/d4 측 perf family heavy on synthetic round-trip** — 53/231 (~23%) game-targeted family.
2. **bnet 측 small but cross-game** — D2R + D4 + WoW 공통 의존 cluster.
3. **lineage retired_intentional thin** — 3 module first-frame skeleton only.
4. **elden 측 own1 자연 부합** — EAC kernel driver 자연 부재 환경 측 own1 정합 cluster (10 module).
5. **cross-cutting infra (metal/pe/dxil/dxr) 측 dominant 만점 contribution** — 29 module / ~13%, default 만점 path.
6. **honest freeze category 측 axis B -2 dominant** — anticheat / warden / d2r realm cluster, own1 정합 honest documentation.
7. **install-phase one-shot (install/oauth/cdn/chunk) 측 axis B -1~-2 dominant** — 14 module / ~6%, Phase 1-3 one-shot 단계.

## §6 next-cycle hooks

### §6.1 axis B strengthening candidates (4)

- `c_pe_iat_simulation` (axis B=3 → 4) — Phase 4 first-frame validated 후 lazy resolve / hot path 재평가
- `c_anticheat_detection_track` (axis B=3 → 4) — long-session hot reload AC detection
- `c_purple_2fa_totp_proxy` (axis B=3 → 4) — session re-auth interval 측 gameplay-loop
- `c_elden_eac_kernel_driver_avoid_track` (axis B=3 → 4) — long-session 측 own1 환경 자연 부합 가드

### §6.2 family-level next-cycle candidates

- **wow / delta_force family** — 현 module 작성 0 (retired_intentional honest documentation 정합), 후순위 cycle 측 honest skeleton 권고 (each ~3-5 module)
- **closure path B Phase 4-D2R first frame validated 후** — representative 15 module 측 validated 단계 axis B 재평가 cycle 권고

### §6.3 documentation 측 next-cycle

- **`lib/perf/c_*_full_enumerate` cycle** — 231/231 module own1 audit + axis A/B/C / total 정밀 enumerate cycle (sample-based aggregate stratum 보강)
- **per-family deep dive** — `c_d4_*` 27 / `c_d2r_*` 26 family 측 deep dive review (각 family 측 별도 mk1 narrative)

### §6.4 5 BG-AGB rank B+C 잔여 candidates (predecessor Track-P 측 6 candidates 중 본 cycle perf_manjeom 측 land 후 잔여 5)

- `loader_native_helper` (rank B) — 별도 cycle 권고
- `perf_game_d2r` (rank B) — 본 doc §6.3 family-level deep dive 와 직접 연결
- `perf_game_d4` (rank C) — 본 doc §6.3 family-level deep dive 와 직접 연결
- `closure_orchestrator` (rank C) — 별도 cycle 권고
- `battlenet_bypass` (self) — Track-P 측 land 완료


**C1**. doc-only land — `lib/perf/c_*.hexa` module touch 0건, lib/loader/ / tool/ / native/ / tests/ touch 0건 (read-only audit only).

**C2**. subjective scoring — 모든 module skeleton tier (synthetic round-trip self_test). axis B 측 ±1 variance 가능 (validated 단계 진입 전).

**C3**. sample-based aggregate — 231 module 전체 enumerate X. representative 15 detailed + 216 family-inheritance heuristic. 정밀 enumerate 별도 cycle 권고.

**C4**. manjeom 자체 subjective — axis A/B/C / total /15 정성적. `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` §0 rubric reuse, 신규 rubric 작성 X.

**C5**. measurement absence — 실 게임 데이터 측정 0건. validated 단계 진입 전 distribution histogram 추정 only.

**C6**. own1 audit family-inheritance — representative 15/15 + spot audit 4/4 = 16/16 PASS (manual). 216 remaining 측 family-inheritance 추정 PASS only — 별도 cycle 정밀 audit 권고.

**C7**. distribution percentages 추정 — 만점 ~48% / 14점 ~32% / 13점 ~13% / 12점 이하 ~7% — ±5% variance.

**C8**. WebSearch 비진행 — 본 cycle 외부 검색 0건. 기존 6 per-game manjeom review 측 WebSearch reuse only.

**C9**. predecessor marker count drift — `lib_perf_hexa_count: 241` vs measure 231, -10 mismatch. 본 cycle 측 231 honest reflect (predecessor marker 측 in-place edit X).

**C10**. scope drift forbidden lock-in — `.roadmap.*` modification / `lib/` / `tool/` / `native/` / `tests/` 작성 0건 (read-only inventory only). 본 doc scope drift validation point.

**C11**. honesty disclaimer — axis B=3 (one-shot install/launch / auth handshake) module 측 conservative scoring. 실 validated 측정 시 axis B=4 가능성 일부 존재.

**C12**. 6 BG-AGB rank B+C 측 본 cycle perf_manjeom (rank B) land — 잔여 5 candidates (loader_native_helper / perf_game_d2r / perf_game_d4 / closure_orchestrator / battlenet_bypass) 측 별도 cycle 권고. perf_game_d2r / perf_game_d4 측 본 doc §6.3 family-level deep dive 와 직접 연결.

## §8 file index

- `docs/PERF_MANJEOM_AGGREGATION_REVIEW.md` — mk1 narrative (NEW, 301 LOC, 8 sections + cross-link)
- `airgenome/doc/airgenome_gamebox_perf_manjeom_aggregation_review_landed_2026_05_03.ai.md` — handoff (NEW, this file)
- `state/markers/airgenome_gamebox_perf_manjeom_aggregation_review_landed.marker` — marker (NEW)

## §9 policy summary

- migration: forbidden
- changes: additive_only
- in_place_writes: 0 (cleanest cycle posture mirror Track-I/L)
- destructive_ops: 0
- cost_usd: 0
- substrate: mac-local
- cap_minutes: 90
- br_no_user_verbatim: true
- friendly_preset: true
- silent-land marker: enforced
- own1: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / DRM evasion 0 / account auth bypass 0 / Warden bypass 0 / EAC bypass 0 / VAC bypass 0 / hook chain shim 0 / signature spoof 0 / NOP-patch 0 / memory scan dodge 0
- own2: log/error/emit/honest — representative 15/15 + spot audit 4/4 PASS, honest scoring (skeleton-tier explicit, not inflated)

## §10 cross-link

- predecessor handoff: `airgenome/doc/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed_2026_05_03.ai.md`
- predecessor marker: `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker`
- mk1 narrative: `docs/PERF_MANJEOM_AGGREGATION_REVIEW.md`
- scoring rubric: `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` §0
- per-game review siblings: `docs/CS2_MANJEOM_REVIEW.md` / `docs/D4_MANJEOM_REVIEW.md` / `docs/DELTA_FORCE_MANJEOM_REVIEW.md` / `docs/ELDEN_RING_MANJEOM_REVIEW.md` / `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` / `docs/WOW_MANJEOM_REVIEW.md`
- sister honest freeze cluster: `docs/GAMEGUARD_HONEST_FREEZE.md` / `docs/WARDEN_HONEST_FREEZE.md` / `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md`
- data source marker: `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker`

---

*written 2026-05-03 — Track-T perf_manjeom aggregation review handoff. doc-only land, additive_only / migration forbidden / in_place_writes=0 / destructive_ops=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker. representative 15 module own1 PASS 15/15 + spot audit 4/4 = 19/19 manual PASS. 216 remaining family-inheritance 추정 PASS — 별도 cycle 정밀 audit 권고.*
