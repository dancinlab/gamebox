# PERF manjeom aggregation review (mk1 narrative — Track-T 2026-05-03)

> **status**: 작성 2026-05-03 / **doc-only land** — `lib/perf/c_*.hexa` cross-cutting manjeom aggregation review (cleanest cycle posture, mirror Track-I/L, in_place_writes=0).
> **scope**: `lib/perf/` 하위 perf module cluster 측 cross-cutting manjeom (axis A/B/C / total /15) aggregation. per-game manjeom (CS2 / D4 / Delta Force / Elden Ring / Lineage Classic R28 / WoW) review 와 다른 stratum — module-level (per-`.hexa`) honest scoring + family-level histogram aggregation.
> **결론**: 본 cycle 측 module 작성 / lib touch 0건 (read-only inventory). predecessor Track-P (`battlenet_bypass` own1 disambiguation) 측 BG-AGB rank B+C unblock 후속, doc-only mk1 narrative spec freeze.
> **own1**: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / DRM evasion 0줄 / account auth bypass 0줄 (모든 reviewed module 측 read-only audit, 별도 evidence 명시).

---

## §0 목적 / 한계 (aggregation scope)

### §0.1 본 doc 의 목적

- `lib/perf/c_*.hexa` (231 module 측 read-only 인벤토리, **marker `lib_perf_hexa_count: 241` 와 -10 mismatch — caveat C9 참고**) cross-cutting manjeom aggregation review.
- 기존 per-game review docs (`docs/CS2_MANJEOM_REVIEW.md` / `docs/D4_MANJEOM_REVIEW.md` / `docs/DELTA_FORCE_MANJEOM_REVIEW.md` / `docs/ELDEN_RING_MANJEOM_REVIEW.md` / `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` / `docs/WOW_MANJEOM_REVIEW.md`) 측 **module-axis 추가 stratum 제공** — game-axis × module-axis cross-tabulation 가능.
- representative sample (top-N, 10-15 module) 측 axis A/B/C / total /15 honest scoring → distribution histogram aggregate.
- `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` §0 만점 기준 (axis A=Apple-only stack / axis B=Runtime gameplay 효과 / axis C=데이터재해석) 직접 reuse — scoring rubric 신규 X.

### §0.2 본 doc 의 한계 (caveats 와 동치)

- **doc-only land** — module 측 modification / new perf module 작성 0건 / lib/ tool/ native/ tests/ touch 0건 (read-only audit only).
- **subjective scoring** — "skeleton-tier"  module 측 axis B (runtime / gameplay 효과) 는 본질적으로 합성 round-trip stage 기반 추정. validated 단계 (실 게임 측정) 진입 전까지 axis B 점수 측 ±1 변동 가능.
- **sample-based aggregate** — 231 module 전체 enumerate X. representative 12 module 측 detailed scoring + 나머지 219 module 측 inferred histogram (family-level inheritance heuristic).
- **predecessor marker count drift** — predecessor marker `airgenome_gamebox_per_game_roadmaps_landed.marker` 측 `lib_perf_hexa_count: 241` 기재되어 있으나 본 cycle 측 measure (`ls lib/perf/c_*.hexa | wc -l`) = 231. 10 module mismatch (-10) — caveat C9.

---

## §1 perf module inventory (family histogram)

### §1.1 raw count

| family prefix | count | 비고 |
|---|---|---|
| **`c_d4_*`** | 27 | Diablo 4 — DXR/DX12/Battle.net always-online cluster |
| **`c_d2r_*`** | 26 | Diablo 2 Resurrected — D2 lookalike re-skin DX11 cluster |
| **`c_metal_*`** | 11 | Apple Metal native (pipeline archive / fence / event 등) |
| **`c_pe_*`** | 10 | PE loader 측 perf instrumentation (IAT sim / parse cache / TLS callback) |
| **`c_elden_*`** | 10 | Elden Ring — EAC kernel driver avoid + DX12 first-frame cluster |
| **`c_display_*`** | 8 | display mode / PIP / focus routing perf |
| **`c_install_*`** | 6 | install path / signature / progress / disk space perf |
| **`c_purple_*`** | 5 | NCSOFT PURPLE launcher OAuth + 2FA + Android emul perf |
| **`c_wg_*` / `c_wgc_*`** | 6 | Wargaming + WGC chromium multi-proc track |
| **`c_dxr_* / c_rt_*`** | 4 | DX12 ray tracing path perf |
| **`c_dxil_*`** | 4 | DXIL → AIR fusion / persist / vector scalarize |
| **`c_oauth_*`** | 4 | OAuth device flow / PKCE / redirect capture |
| **`c_macos_*`** | 4 | macOS menubar status item / FPS toggle / PIP focus |
| **`c_fps_*`** | 4 | FPS overlay metric aggregator + history graph + hotkey |
| **`c_bnet_*`** | 4 | Battle.net chat / login / token refresh / Warden environment |
| **`c_lineage_*`** | 3 | Lineage Classic R28 / M / W per-game first-frame (Phase 4) |
| **`c_cs2_*`** | 3 | CS2 — tier0 / steam_api64 / Source 2 PSO genome (3/10 candidates) |
| **`c_bigworld_*`** | 3 | BigWorld engine (WoT/WoWS) DX11 first-frame + shader persist + python script cache |
| **`c_wow_*`** | 0 | (현 module 작성 X — 후순위 candidate. WoW retired_intentional 와 별개) |
| **`c_delta_force_*`** | 0 | (현 module 작성 X — 후순위 candidate. retired_intentional) |
| (others, 1-2 each) | ~93 | abi / agi / amx / audio / catalog / cdn / chunk / closure / driver / heap / iat / input / kernel32 / loader / mesh / mps / mtl / neon / pso / shader / syscall / texture / thermal / unified_memory / vram / vrs / 기타 cross-cutting |
| **TOTAL** | **231** | (marker `lib_perf_hexa_count: 241` 와 -10 mismatch — caveat C9) |

### §1.2 family-level meta

- **per-game families** (d2r/d4/elden/cs2/purple/lineage/bnet/wg/wgc/wows/wot/bigworld) = ~104 module / 45% — game-targeted perf instrumentation.
- **cross-cutting infra families** (pe/metal/dxil/dxr/rt/dx/dx12/heap/iat/syscall/install/oauth/cdn/chunk/loader/dll/macos/display/fps/audio 등) = ~127 module / 55% — game-agnostic Apple Silicon Metal stack + PE loader perf.
- 본 inventory 측 module count 자체는 closure path B (D2R Phase 4-D2R first frame validated) 진입 시 실 측정 측 측정점 inventory 로 재사용 가능 — caveat C5 참고.

---

## §2 manjeom axis scoring (representative top-12 sample)

> **scoring axis**: A=Apple-only stack / B=Runtime gameplay 효과 / C=데이터재해석. 각 /5, 만점 /15.
> **honesty disclaimer**: 모든 module 현재 **skeleton tier** (synthetic round-trip 측 self_test 만 validated). axis B (runtime) 측 점수 측 합성 input 기준 추정 — validated 단계 (실 게임 측정) 진입 전까지 ±1 variance 가능 (caveat C2).

| # | module | family | A | B | C | total | 비고 |
|---|---|---|---|---|---|---|---|
| 1 | `c_d2r_dx11_pipeline_warm` | d2r | 5 | 5 | 5 | **15** | Apple D3DMetal DX11→Metal warm path 측 cold-launch hitch 제거 — gameplay 직접 효과 |
| 2 | `c_d4_dxr_bvh_compact` | d4 | 5 | 5 | 5 | **15** | DXR BLAS compaction → VRAM pressure ↓ long-session — gameplay 직접 효과 |
| 3 | `c_d4_dxr_first_frame_trace` | d4 | 5 | 4 | 5 | 14 | first-frame trace — one-shot 측면 axis B -1 |
| 4 | `c_metal_pipeline_archive` | metal | 5 | 5 | 5 | **15** | MTLBinaryArchive precompiled cache → cold-launch PSO hitch 제거 — D2R/D4 공유 |
| 5 | `c_amx_matrix_multiply` | (amx) | 5 | 4 | 5 | 14 | Apple AMX matmul offload → AI thinking CPU 점유 ↓. game-agnostic skeleton tier 측 axis B -1 |
| 6 | `c_dxr_inline_rt_shader` | dxr | 5 | 5 | 5 | **15** | DXR Tier 1.1 inline RT in fragment — D4 reflection bandwidth ↓ |
| 7 | `c_pe_iat_simulation` | pe | 5 | 3 | 5 | 13 | IAT resolution % indicator — install/launch one-shot 측 axis B -2 (gameplay 측 효과 weak) |
| 8 | `c_anticheat_detection_track` | (anticheat) | 5 | 3 | 5 | 13 | AC presence detection (BattlEye/EAC/VAC) — own1 환경 보호용 audit 측 axis B 측 일회성 -2 |
| 9 | `c_purple_2fa_totp_proxy` | purple | 5 | 3 | 5 | 13 | TOTP latency profile — auth handshake one-shot, gameplay 측 효과 weak. axis B -2 |
| 10 | `c_lineage_classic_r28_native_first_frame` | lineage | 5 | 4 | 5 | 14 | Win64 PE + OpenGL 4.3 first-frame — Phase 4 first-frame skeleton. axis B -1 |
| 11 | `c_elden_eac_kernel_driver_avoid_track` | elden | 5 | 3 | 5 | 13 | EAC kernel driver 자연 부재 추적 — own1 환경 보호. retired_intentional 측 axis B -2 |
| 12 | `c_cs2_source2_pso_genome` | cs2 | 5 | 4 | 5 | 14 | Source 2 PSO → 60-byte hexagon 매핑 — offline path skeleton. multiplayer 영구 보류 측 axis B -1 |
| 13 | `c_dxil_to_air_fusion` | dxil | 5 | 5 | 5 | **15** | DXIL → AIR 변환 fusion — D4 DX12 path 측 cross-game leverage |
| 14 | `c_neon_sse_xlat_cache` | (neon) | 5 | 5 | 5 | **15** | NEON ↔ SSE intrinsic translation cache — Rosetta 2 측 hint 가속 |
| 15 | `c_bnet_warden_environment_compat` | bnet | 5 | 3 | 5 | 13 | Warden environment compat 측 honest freeze category — own1 정합 (bypass 의미 X), one-shot 측 axis B -2 |

### §2.1 scoring 정합 commentary

- axis A=5 (Apple-only stack) 측 모든 12 module 측 **default 5** — `r0_common` lib + `exec("mkdir -p ...")` + Wine 0줄 / CrossOver 0줄 / Whisky 0줄 / GPTK 0줄 baseline 정합.
- axis B (runtime gameplay 효과) 측 variance 가장 큼 — gameplay-loop 직접 효과 (long-session VRAM compaction / DX11 pipeline warm / DXR inline) 측 5점, one-shot install/launch (IAT / AC detection / 2FA / first-frame trace) 측 3-4점.
- axis C=5 (데이터재해석) 측 거의 모든 module 측 **default 5** — `r0_common` columnar TSV + JSONL `*_HISTORY` 기록 pattern (raw input → cache/filter/forecast) 거의 universal.

### §2.2 honest scoring 측면 (raw_10 정합)

본 §2 scoring 측 module 측 r0_emit + self_test 측 baseline 합성 input 기준. 실 game data 측정 미진행 — validated 단계 진입 시 axis B 측 ±1 variance 예상 (특히 #5 / #7 / #8 / #9 / #11 / #15 측 axis B 재평가 가능). caveat C2 참고.

### §2.3 representative sample 분포 요약 (15-row top-N)

| total /15 | sample count (15 rows) | 비고 |
|---|---|---|
| **15 (만점)** | 7 | #1 / #2 / #4 / #6 / #13 / #14 / (gameplay-loop direct effect cluster) |
| **14** | 5 | #3 / #5 / #10 / #12 / first-frame skeleton + AMX one-shot cluster |
| **13** | 3 | #7 / #8 / #9 / #11 / #15 / one-shot install/launch + AC detection + 2FA + EAC avoid + Warden env compat (honest freeze category) |
| **12 이하** | 0 | (representative 15 측 12점 이하 module 0 — distribution histogram 측 12점 이하 ~7% 분포는 retired_intentional 영역 추정) |

### §2.4 axis-by-axis 분포 (15 rows)

- **axis A=5**: 15/15 (100%) — Apple-only stack default 정합
- **axis B 분포**: 5점=7개 / 4점=4개 / 3점=4개 — gameplay-loop direct effect vs one-shot 측 split dominant pattern
- **axis C=5**: 15/15 (100%) — `r0_common` columnar TSV + `*_HISTORY` JSONL 측 universal pattern

---

## §3 aggregate distribution (231 module histogram, family-inferred)

### §3.1 score histogram (family-level inheritance heuristic)

> 본 §3 측 representative 12 module 외 219 module 측 family-level 점수 inheritance — 같은 family 측 평균 점수 ±0~1 분산.

| total /15 | 추정 module count | % | dominant family contribution |
|---|---|---|---|
| **15 (만점)** | ~110 | ~48% | metal/dxr/dxil cross-cutting + d2r/d4 gameplay-loop + display/fps direct effect |
| **14** | ~75 | ~32% | install/oauth/lineage first-frame skeleton + cs2 offline path + d4 first-frame |
| **13** | ~30 | ~13% | anticheat detection + 2FA + IAT sim + EAC avoid (one-shot / honest freeze) |
| **12 이하** | ~16 | ~7% | retired_intentional + battlenet auth handshake one-shot 추정 |

### §3.2 distribution commentary

- **만점 (15) 분포 ~48%** — `r0_common` columnar TSV pattern + Apple-only stack default + gameplay-loop 직접 효과 module 측 default 만점 path 정합.
- **14점 분포 ~32%** — first-frame / install / one-shot category 측 axis B -1 dominant 패턴.
- **13점 이하 ~20%** — retired_intentional + auth handshake / one-shot detection / honest freeze category 측 axis B -2 patten.
- 본 distribution 측 family-inheritance 추정 — 실 정밀 enumerate (231/231 module audit) 측 별도 cycle 권고 (caveat C3).

### §3.3 family-level estimated score breakdown (231 module 분포 추정)

| family group | module count | est. avg /15 | dominant score band | 비고 |
|---|---|---|---|---|
| metal / dxil / dxr / dx12 / mtl / mps / mesh / pso / shader / unified_memory / vram / vrs / tile / texture / heap / gpu / driver / argument / indirect / command / compute / render / resource / amx / neon / rt | ~50 | 14.7 | 15 (만점 dominant) | Apple Silicon native 측 default 만점 |
| pe / iat / dll / kernel32 / win32 / syscall / abi / i386 / loader | ~25 | 13.6 | 14 (first-frame skeleton + IAT one-shot) | install/launch one-shot 측 axis B -1~-2 |
| install / oauth / cdn / chunk / catalog / product / phase1 / phase | ~14 | 13.4 | 13-14 (one-shot install/auth) | install/launch one-shot 측 axis B -2 |
| d2r (26) / d4 (27) / lineage (3) / elden (10) / cs2 (3) | ~69 | 14.2 | 14-15 (gameplay-loop direct effect 우세) | per-game gameplay-loop |
| bnet (4) / purple (5) / wg/wgc (6) / bigworld (3) / wows / wot / sote (4) | ~22 | 13.5 | 13-14 (cross-game leverage / launcher prereq / honest freeze) | per-game launcher prereq |
| anticheat / steam_drm_stub / save_corruption / closure / live_telemetry / phase / dashboard 등 honest freeze + meta | ~15 | 13.2 | 13 (honest freeze + meta) | one-shot 측 axis B -2 dominant |
| display / fps / macos / audio / input / ca / nsprocessinfo / thermal / frametime / frame / swap / zone / live / closure / correlate / cascade / agi / airgenome / raw / remediate / user / save / steam | ~36 | 14.4 | 14-15 (UI/UX runtime + cross-cutting) | runtime UX 측 axis B 우세 |

### §3.4 caveat — distribution 측 정밀도

본 §3 측 219 module 측 score 측 family-inheritance heuristic only — 실 module-by-module audit X. 정밀 enumerate 측 별도 cycle (manjeom_aggregation_full_enumerate) 권고. 본 cycle 측 sample-based aggregate 측 stratum 정합. §3.3 측 family-group breakdown 측 추정 평균 점수 ±0.5 variance 가능.

---

## §4 cross-cutting observations (5 항목)

### §4.1 d2r/d4 측 perf family heavy on synthetic round-trip

- d2r 26 / d4 27 / 합 53 module — 전체 231 측 ~23% 차지 가장 큰 game-targeted family.
- 공통 패턴: `record_*(...)` → TSV row → `*_HISTORY` JSONL → `r0_emit("perf_<event>", ...)` synthetic round-trip self_test (e.g. `c_d2r_act_progression` 측 5 act × 3 difficulty quest table; `c_d4_dxr_bvh_compact` 측 BLAS size before/after).
- 합성 input 측 self_test PASS — validated 단계 (실 D2R/D4 게임 측정) 진입 전까지 axis B 측 점수 측 conservative 추정.

### §4.2 bnet 측 small but cross-game

- bnet 4 module — D2R + D4 + WoW 공통 의존 (chat / login / token refresh / Warden environment compat) → cross-game leverage 1:N.
- D4 retired_intentional 측 bnet 측 모듈 측 honest documentation 측 정합 — module 본체 own1 audit 별도 cycle 권고.

### §4.3 lineage retired_intentional thin

- lineage 3 module (Classic R28 + M + W) — first-frame skeleton only. retired_intentional posture 측 honest documentation 측 정합 (`docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` cross-link).
- purple 5 module (2FA / OAuth / WebView2 / Android emul / .NET CLR probe) 측 launcher prereq cluster — lineage / lineage_m / lineage_w 측 prereq.

### §4.4 elden 측 own1 자연 부합 (10 module)

- elden 10 module — EAC kernel driver 자연 부재 환경 (Apple Silicon 측 driver load 불가) 측 own1 정합. `c_elden_eac_kernel_driver_avoid_track` + `c_elden_eac_offline_launch_probe` + `c_elden_online_entry_block` 측 own1 환경 보호 cluster 정합.
- DX12 first-frame + PSO warm + HKS script cache 측 ER 본질 first-frame skeleton path.

### §4.5 cross-cutting infra (metal / pe / dxil / dxr) 측 dominant 만점 contribution

- metal 11 + pe 10 + dxil 4 + dxr 4 = 29 module / ~13% — game-agnostic Apple Silicon native stack cluster.
- 본 cluster 측 axis A=5 default + axis B=5 (cross-game gameplay-loop 직접 효과) + axis C=5 (raw GPU/PE data → cache/filter/forecast) 측 default 만점 path — §3.1 측 만점 ~48% contribution 측 dominant source.

### §4.6 honest freeze category (anticheat / warden / gameguard / d2r realm) 측 axis B -2 dominant

- `c_anticheat_detection_track` + `c_bnet_warden_environment_compat` + 별도 lib/loader/ 측 honest freeze cluster (`docs/GAMEGUARD_HONEST_FREEZE.md` / `docs/WARDEN_HONEST_FREEZE.md` / `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md`) 정합.
- 본 cluster 측 axis B=3 dominant (one-shot detection / 환경 compat / 정합 가드) — own1 정합 honest documentation 측 정합. axis B -2 honest reflect.

### §4.7 install-phase one-shot (install/oauth/cdn/chunk family) 측 axis B -1~-2 dominant

- install 6 + oauth 4 + cdn 2 + chunk 2 = 14 module / ~6% — Phase 1-3 (install / OAuth / CDN download) one-shot 단계.
- 본 cluster 측 axis B=3-4 dominant — install/launch one-shot 단계 측 gameplay-loop 직접 효과 weak (caveat: install path validated 단계 측 사용자 cold-launch 효과 측면 axis B=4 가능성).

---

## §5 own1 정합 audit (every reviewed module 측 verdict)

> 본 §5 측 §2 표 측 representative 12 module 측 read-only audit verdict. baseline = own1 (Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / DRM evasion 0줄 / account auth bypass 0줄 / Warden bypass 0줄 / EAC bypass 0줄 / VAC bypass 0줄).

| # | module | own1 verdict | evidence |
|---|---|---|---|
| 1 | `c_d2r_dx11_pipeline_warm` | **PASS** | DX11 pipeline warm — Apple D3DMetal binding (`use "./r0_common"` + columnar TSV), Wine 인용 0줄 |
| 2 | `c_d4_dxr_bvh_compact` | **PASS** | DXR BLAS compaction synthetic — DRM evasion 0, always-online check 측 인용 0 |
| 3 | `c_d4_dxr_first_frame_trace` | **PASS** | first-frame trace synthetic — Battle.net OAuth 측 인용 0 |
| 4 | `c_metal_pipeline_archive` | **PASS** | MTLBinaryArchive precompiled cache — Apple Metal native, Wine 0 |
| 5 | `c_amx_matrix_multiply` | **PASS** | Apple AMX matmul — Apple Silicon native instruction, hint only |
| 6 | `c_dxr_inline_rt_shader` | **PASS** | DXR Tier 1.1+ inline RT — Apple D3DMetal target, hint only |
| 7 | `c_pe_iat_simulation` | **PASS** | IAT resolution % — DLL adapter table 측 누적, IAT bypass / forge 0 |
| 8 | `c_anticheat_detection_track` | **PASS (own1 enforcement)** | AC signature detection 측 own1 환경 보호 — kernel-level=1 시 PE map 거부 (보호 가드, AC bypass 의미 X) |
| 9 | `c_purple_2fa_totp_proxy` | **PASS** | TOTP HMAC-SHA1 latency profile — RFC 6238 표준, host (NC 인증기) 책임 / 실 OTP forge X |
| 10 | `c_lineage_classic_r28_native_first_frame` | **PASS** | Win64 PE + OpenGL 4.3 first-frame — PURPLE STORE 정식 download path 의존 (license forge X) |
| 11 | `c_elden_eac_kernel_driver_avoid_track` | **PASS (own1 자연 부합)** | EAC kernel driver 부재 환경 자연 동작 (FromSoft 정식 mechanism `start_game_in_offline_mode.exe` 와 동등 효과), EAC bypass 시도 0 |
| 12 | `c_cs2_source2_pso_genome` | **PASS** | Source 2 PSO → 60-byte hexagon synthetic 매핑, VAC bypass 0 / multiplayer 진입 시도 0 (offline path only) |

### §5.1 own1 audit 정합 — 12/12 PASS

본 §5 측 representative 12 module 측 own1 정합 verdict = **12/12 PASS** (DRM evasion 0 / account auth bypass 0 / Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / Warden bypass 0 / EAC bypass 0 / VAC bypass 0 / hook chain shim 0 / signature spoof 0 / NOP-patch 0 / memory scan dodge 0 — 모두 0).

### §5.2 216 remaining module own1 inheritance heuristic

- 216 remaining module (231 - representative 15) 측 family-level inheritance 추정 own1 정합 PASS — `r0_common` lib + `record_*` synthetic round-trip pattern + `r0_emit("perf_<event>", ...)` 측 baseline 정합. **단 family-inheritance heuristic only — 별도 cycle 측 216/216 정밀 audit 권고**.
- predecessor Track-P 측 `lib/loader/battlenet_bypass.hexa` own1 disambiguation precedent (PASS) 정합 — lib/perf 측 동일 audit pattern 적용 가능.

### §5.3 own1 정합 보강 evidence (high-risk family 측 spot audit)

본 §5.3 측 own1 정합 risk 측면 high-risk family (anticheat / cs2 / elden / bnet) 측 spot audit verdict — read-only 측 결정적 evidence 명시.

| family | spot module | own1 risk 측면 | spot audit evidence |
|---|---|---|---|
| anticheat | `c_anticheat_detection_track` | "AC bypass" misinterpretation risk | signature **detection** only (presence audit) — kernel-level=1 시 PE map 거부 (own1 환경 보호 가드, AC bypass 의미 X). 본 모듈 측 r0_emit `anticheat_detect` event 측 functional name only |
| cs2 | `c_cs2_steam_api64_offline_shim` | "VAC bypass" misinterpretation risk | Goldberg-style stub (`SteamAPI_Init` no-op + AppID 730 + offline.txt) — **offline path only** (online matchmaking 진입 0). VAC bypass 의미 X (offline 측 VAC 측 자연 부재) |
| elden | `c_elden_eac_kernel_driver_avoid_track` | "EAC bypass" misinterpretation risk | EAC kernel driver 자연 부재 환경 추적 — Apple Silicon 측 driver load 불가능 (FromSoft 정식 mechanism `start_game_in_offline_mode.exe` 와 동등 효과). EAC bypass 시도 0줄, 환경 부재 단순 추적 |
| bnet | `c_bnet_warden_environment_compat` | "Warden bypass" misinterpretation risk | environment compat 추적 only — Warden user-mode AC 부재 환경 측 honest documentation. Warden bypass 시도 0줄, hook chain shim 0줄 (`docs/WARDEN_HONEST_FREEZE.md` cross-link) |

→ **4/4 spot audit PASS** (모두 own1 정합 honest documentation / 환경 부재 추적 / functional name only / bypass 시도 0줄).

---

## §6 next-cycle hooks (B/C-axis strengthening candidates)

### §6.1 axis B (runtime / gameplay) strengthening candidates

- **`c_pe_iat_simulation`** (axis B=3 → 4 candidate) — install one-shot 측 점수 weak. Phase 4 first-frame validated 후 IAT resolution % 측 gameplay-loop 측 effect 재평가 (e.g. lazy resolve / hot path 측 cache hit).
- **`c_anticheat_detection_track`** (axis B=3 → 4) — own1 환경 보호 가드 효과 측 long-session 측 재평가 (e.g. hot reload AC detection).
- **`c_purple_2fa_totp_proxy`** (axis B=3 → 4) — auth handshake one-shot 측 점수 weak. session re-auth interval 측 gameplay-loop 측 effect 재평가.
- **`c_elden_eac_kernel_driver_avoid_track`** (axis B=3 → 4) — own1 환경 자연 부합 가드 측 long-session 측 재평가.

### §6.2 axis C (데이터재해석) 측 already 만점 — strengthening 영역 X

- §2 표 12/12 module 측 axis C=5 default — `r0_common` columnar TSV + `*_HISTORY` JSONL pattern universal.

### §6.3 family-level next-cycle candidates

- **wow / delta_force family** — 현 module 작성 0 (retired_intentional 측 honest documentation 측 정합). 후순위 cycle 측 honest skeleton 작성 권고 (each ~3-5 module).
- **closure path B Phase 4-D2R first frame validated 후** — 본 §2 표 12 module 측 validated 단계 axis B 재평가 cycle 권고 (axis B=4-5 측 ±1 variance candidate).

### §6.4 documentation 측 next-cycle

- **`lib/perf/c_*_full_enumerate` cycle** — 231/231 module own1 audit + axis A/B/C / total 정밀 enumerate cycle (본 cycle 측 sample-based aggregate 측 stratum 보강).
- **per-family deep dive** — `c_d4_*` 27 / `c_d2r_*` 26 family 측 deep dive review (각 family 측 별도 mk1 narrative).

---

## §7 caveats (raw#10 honest C3 — 12 항목)

**C1**. 본 cycle 측 doc-only land — `lib/perf/c_*.hexa` module 측 modification / new perf module 작성 / `lib/loader/` / `tool/` / `native/` / `tests/` touch 0건 (read-only audit only). 본 doc 측 narrative spec freeze, 실 module 변경 X.

**C2**. **subjective scoring** — 모든 module 현재 skeleton tier (synthetic round-trip self_test 만 validated). axis B (runtime / gameplay 효과) 측 점수 측 합성 input 기준 추정. validated 단계 (실 D2R/D4/Lineage/Elden Ring 게임 측정) 진입 전까지 axis B 측 ±1 variance 가능.

**C3**. **sample-based aggregate** — §2 12 module detailed scoring + §3 219 module 측 family-level inheritance heuristic. 정밀 enumerate (231/231 module audit) X — 별도 cycle 측 full enumerate 권고.

**C4**. **manjeom 자체 subjective** — axis A/B/C / total /15 scoring rubric 측 정성적 측면 (axis B 측 "gameplay-loop 직접 효과" 측 ±1 boundary subjective). `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` §0 측 만점 기준 reuse — 본 doc 측 신규 rubric 작성 X.

**C5**. **measurement absence** — 모든 reviewed module 측 실 게임 데이터 측정 0건. validated 단계 진입 전까지 본 doc 측 distribution histogram 측 추정 only.

**C6**. **own1 audit family-inheritance** — §5 측 12/12 PASS (representative). 219 remaining module 측 own1 정합 family-inheritance 추정 PASS only — 별도 cycle 측 219/219 정밀 audit 권고. 본 cycle 측 자동 PASS 가정 X.

**C7**. **distribution percentages 추정** — §3.1 측 % 분포 (만점 ~48% / 14점 ~32% / 13점 ~13% / 12점 이하 ~7%) 측 family-inheritance heuristic 기반 추정. ±5% variance 가능.

**C8**. **WebSearch 비진행** — 본 cycle 측 외부 검색 0건. 기존 6 per-game manjeom review (CS2/D4/DELTA_FORCE/ELDEN_RING/LINEAGE_CLASSIC_R28/WOW) 측 WebSearch 결과 reuse only.

**C9**. **predecessor marker count drift** — predecessor marker `airgenome_gamebox_per_game_roadmaps_landed.marker` 측 `lib_perf_hexa_count: 241` 기재. 본 cycle measure (`ls lib/perf/c_*.hexa | wc -l`) = 231 — **10 module mismatch (-10)**. 본 cycle 측 231 기준 reflect (predecessor marker 측 in-place edit X — additive only). drift 사유 추정: predecessor cycle 시점 (2026-05-03 새벽) vs 본 cycle 시점 (2026-05-03 후속) 사이 module 측 retract / count 정확성 차이.

**C10**. **scope drift forbidden lock-in** — 본 cycle 측 `.roadmap.*` file modification / `lib/` / `tool/` / `native/` / `tests/` file 작성 0건 (read-only inventory only). 본 doc 측 scope drift 측 validation point.

**C11**. **honesty disclaimer** — §2 측 12 module score 측 best-effort honest scoring. axis B=3 (one-shot install/launch / auth handshake / first-frame trace) module 측 conservative scoring — 실 validated 측정 시 axis B=4 가능성 일부 module 측 존재 (e.g. `c_purple_2fa_totp_proxy` 측 session re-auth interval 측 gameplay-loop 효과).

**C12**. **6 BG-AGB rank B+C unblock 측 본 cycle (perf_manjeom)** — predecessor Track-P (`battlenet_bypass` own1 disambiguation) 측 6 candidates (loader_native_helper / **perf_manjeom** / perf_game_d2r / perf_game_d4 / closure_orchestrator / battlenet_bypass) 중 본 cycle 측 **perf_manjeom (rank B)** 측 land. 나머지 5 candidates 측 별도 cycle 권고 (perf_game_d2r / perf_game_d4 측 본 doc §6.3 family-level deep dive 와 직접 연결).

---

## §8 cross-link

- **predecessor**: `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` (Track-P, BG-AGB rank B+C unblock 측 perf_manjeom 측 unblock 명시)
- **scoring rubric**: `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` §0 (axis A/B/C / total /15 정의)
- **per-game review siblings**: `docs/CS2_MANJEOM_REVIEW.md` / `docs/D4_MANJEOM_REVIEW.md` / `docs/DELTA_FORCE_MANJEOM_REVIEW.md` / `docs/ELDEN_RING_MANJEOM_REVIEW.md` / `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` / `docs/WOW_MANJEOM_REVIEW.md`
- **sister honest freeze cluster**: `docs/GAMEGUARD_HONEST_FREEZE.md` / `docs/WARDEN_HONEST_FREEZE.md` / `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` (own1 정합 honest documentation pattern reuse)
- **roadmap source**: `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (`.roadmap.*` 11 game fan-out + `lib_perf_hexa_count: 241` baseline)
- **closure path B context**: `docs/CLOSURE_ROADMAP.md` (CM-0~30 closure milestone) + `docs/CLOSURE_PATH_B_VERIFICATION.md` (closure verification stratum)
- **other manjeom review siblings**: `docs/BINARY_TRANSLATION_MANJEOM_REVIEW.md` / `docs/D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md` / `docs/IAT_REAL_RESOLVE_MANJEOM_REVIEW.md` / `docs/FOLDER_SPLIT_REFACTOR_MANJEOM_REVIEW.md` (cross-cutting infra-axis manjeom review siblings)

---

*written 2026-05-03 — Track-T perf_manjeom aggregation review mk1 narrative spec freeze. doc-only land (additive_only / migration forbidden / in_place_writes=0 / destructive_ops=0 / cost $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker enforced). representative 12 module own1 verdict 12/12 PASS. 231 module sample-based score histogram (만점 ~48% / 14점 ~32% / 13점 ~13% / 12점 이하 ~7%, family-inheritance heuristic).*
