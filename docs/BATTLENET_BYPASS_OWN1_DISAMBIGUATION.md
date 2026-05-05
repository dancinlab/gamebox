# `lib/loader/battlenet_bypass.hexa` own1 Disambiguation — Track P naming clarification

> **status**: 작성일 **2026-05-03** / **scope: NAMING DISAMBIGUATION ONLY** — 본 문서는 `lib/loader/battlenet_bypass.hexa` (8 battlenet loader cluster member) 의 모듈명 측 "bypass" 단어가 own1 (Wine 0 / NO DRM evasion / NO account auth bypass / NO Warden circumvention) 정합 측 misinterpretation 측 risk 가지는 점을 정직하게 disambiguate.
> 작성: 2026-05-03 (Track-P own1 disambiguation cycle, sister Track-E GameGuard kernel-mode + Track-J Warden user-mode + Track-L D2R realm protocol freeze cluster)
> SSOT: `lib/loader/battlenet_bypass.hexa` 본체 + 그 모듈 측 prepend annotation block (additive only, content rewrite X)
> 결론 한줄: **`battlenet_bypass.hexa` = launcher-absence standalone enumeration + feasibility matrix honest emit (category a) — NOT DRM evasion / NOT account auth bypass / NOT Warden circumvention. own1 정합 verdict = PASS. 본 doc 는 그 verdict 의 mk1 narrative reference**.

---

## 0. 본 doc 의 목적과 한계 (가장 먼저 명시)

### 0.1 목적 (in-scope)

- `lib/loader/battlenet_bypass.hexa` 모듈명 측 "bypass" 단어 측 own1 misinterpretation risk 정직하게 disambiguate
- 모듈 actual scope 측 4 카테고리 (a/b/c/d) 분석 + verdict 명시 (PASS / FLAGGED)
- 모듈 측 prepend annotation block (additive only) 측 mk1 narrative reference
- 6 BG-AGB rank B+C candidates (loader_native_helper / perf_manjeom / perf_game_d2r / perf_game_d4 / closure_orchestrator / battlenet_bypass) 측 own1 review gate 통과 확인

### 0.2 한계 (out-of-scope, 영구)

본 doc 측 다음 영역은 **영구 out-of-scope** 이며, 어떤 형태의 implementation 도 airgenome-gamebox 측 import 되지 않음:

- DRM evasion / license circumvention
- Account authentication bypass
- Warden user-mode anti-cheat bypass / hook chain shim / signature spoof / NOP-patch / memory-scan dodge
- GameGuard kernel-mode anti-cheat bypass (non-Blizzard family이지만 동일 분류)
- Battle.net 계정 측 탈취 / fraud / ticket validation 우회
- Wine / CrossOver / Whisky / GPTK staging patch import / 인용
- Online ToS violation tooling / cheat / botting / automation signature pattern

본 doc 는 위 카테고리 중 어느 것도 implement 하지 않음. 모듈명의 "bypass" 단어가 위 영역과 의미 혼동될 위험 (own1 misinterpretation) 을 정직하게 disambiguate 만 수행.

### 0.3 본 cycle 의 changes 측 minimal additive 정합

- `lib/loader/battlenet_bypass.hexa` 본체 fn / self_test / main / TSV schema / r0_emit content 측 modification 0건
- 모듈 측 PREPEND annotation block (Track-P 명시) 만 추가
- `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` (본 doc) NEW
- handoff doc + marker NEW
- destructive_ops 0 / migration X / additive_only

---

## 1. Module identity

### 1.1 path + cluster membership

| 항목 | 값 |
|---|---|
| path | `lib/loader/battlenet_bypass.hexa` |
| cluster | 8 battlenet loader cluster (per `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` cross_link) |
| pattern | `lib/loader/(battlenet_bypass + pe_battle_net_* 4 + pe_battlenet_* 4)` |
| sister modules (8 cluster, untouched in 본 cycle) | `pe_battle_net_agent_ipc_track.hexa` / `pe_battle_net_download_track.hexa` / `pe_battle_net_oauth_token.hexa` / `pe_battle_net_setup_install_phase.hexa` / `pe_battlenet_agent_http_rest.hexa` / `pe_battlenet_agent_sd_forging.hexa` / `pe_battlenet_setup_install_path_real.hexa` / `pe_battlenet_setup_static_crt_path.hexa` |
| LOC (Track-P 측 annotation prepend 후) | 379 (annotation block prepend +31) |
| LOC (Track-P 측 annotation prepend 전) | 348 |
| Phase | 8 (Battle.net 우회 skeleton — naming legacy) |

### 1.2 module emit pattern (Track-P 측 annotation block 추가 전 baseline)

- `r0_emit ≥ 1` (실제 ≥ 2: `bypass_check` + `bypass_stats`)
- `fn self_test()` + `fn main()` dispatch
- headless (stdin/stdout/file 만)
- `panic("battlenet_bypass FAIL [<sub>] expected=X got=Y")` 형식

### 1.3 own1 enforcement 측 module-internal 명시

모듈 본체 line 3-5 (Track-P 측 annotation block 위 baseline) 측 already 명시:

```
own 1 enforcement: Battle.net launcher / Blizzard SDK / community bypass 코드 0 줄.
  참조 only — 공개 PE / Wine prefix layout / OAuth bearer 일반 spec.
  Wine / Lutris / Heroic / community bypass 단 1 줄도 import / copy / port 금지.
```

즉 module 본체 측 own1 정합 명시는 이미 baseline 측 존재. 본 Track-P cycle 은 모듈명 측 "bypass" 단어가 misinterpretation risk 가지는 점을 더 명시적으로 disambiguate (annotation block prepend).

---

## 2. Scope finding — 4 category 분석

### 2.1 4 카테고리 정의

본 doc 측 module 측 actual scope 분류 위해 4 카테고리 분석:

| 카테고리 | 정의 | own1 정합 |
|---|---|---|
| (a) Battle.net launcher-absence standalone path | launcher 부재 시 단독 실행 path enumeration + feasibility matrix | ✓ own1 정합 |
| (b) Battle.net DRM evasion attempt | DRM ticket 위조 / license bypass / always-online check 회피 | ✗ own1 위반 |
| (c) Battle.net session/agent shim | session 측 hook chain / agent 측 in-process shim / IAT 위조 | ✗ own1 위반 |
| (d) something else | 위 3 카테고리 외 | 별도 evaluate |

### 2.2 Module actual scope finding (line-by-line audit)

본 doc 측 module 본체 (348 baseline LOC) 측 line-by-line audit 결과:

| 모듈 측 fn / 영역 | 카테고리 | 분석 |
|---|---|---|
| `fn battlenet_launcher_path(prefix)` | (a) | Wine prefix 측 표준 launcher path 추정 — string concatenation only, 실 launcher 측 hook X |
| `fn battlenet_deps()` | (a) | launcher 측 dependency enumeration (`auth_token` / `version_check` / `region` / `drm_ticket` / `session_id` / `product_code`) — 명칭 reference only, dep 측 위조 / spoof 0줄 |
| `fn auth_token_cache_path()` | (a) | cache file path spec only (`/tmp/airgenome/battlenet/auth_token.cache`) — 실 token 측 사용자 manual 추출 영역 (모듈 측 token forge X) |
| `fn direct_launch_spec(game, prefix)` | (a) | JSON-like spec emit (game / prefix / exe / args / env / token_cache) — 실 spawn X, spec emit only. `BNET_BYPASS=1` env name 은 launcher-absence flag (DRM bypass 의미 X) |
| `fn bypass_feasibility(game)` | (a) | feasibility matrix — D2R feasible (offline mode 존재) / D4 NOT feasible (online-only / server-side auth / always-online DRM blocker 정직 보고). 즉 D4 측 회피 시도 X — 정직하게 BLOCKED 보고 |
| `fn bypass_stats()` | (a) | stats — `[d2r=feasible, d4=blocked]` honest report |
| `fn self_test()` | (a) | self-test — bypass_feasibility / bypass_stats / direct_launch_spec / battlenet_deps 측 invariant verification |
| `fn main()` dispatch | (a) | CLI dispatch — self-test / launcher-path / deps / token-cache / spec / feasibility / stats |

**결과**: 100% 카테고리 (a) — launcher-absence standalone enumeration + feasibility matrix honest emit. 카테고리 (b) (c) (d) 측 0줄.

### 2.3 결정적 own1 정합 evidence 3 항목

본 doc 측 module 측 own1 정합 evidence 3 항목:

1. **D4 측 NOT feasible 정직 보고**: `bypass_feasibility("d4")` 측 `[false, "D4 is online-only — server-side auth blocks any launcher bypass", ["server_side_auth", "always_online_drm", "ProductDB version handshake"]]` — 즉 module 은 D4 측 online-only DRM 측 회피 시도 X, **honest BLOCKED 보고**. Track-J Warden honest freeze 측 retired_intentional posture 와 정합.
2. **community bypass code 0줄 명시**: module header (line 3-5) 측 "Wine / Lutris / Heroic / community bypass 단 1 줄도 import / copy / port 금지" 명시.
3. **token forge X**: `auth_token_cache_path()` 측 path spec only — 실 token 측 사용자 manual 추출 영역 (모듈 측 forge X). HWID / machine_id / ticket forge 0줄.

### 2.4 module 측 "bypass" 단어 측 occurrence audit

모듈 (Track-P 측 annotation 추가 후 379 LOC, 추가 전 348 LOC baseline) 측 `bypass` 단어 occurrence:

| 위치 (annotation block 제외) | context | own1 정합 |
|---|---|---|
| 모듈명 자체 (`battlenet_bypass.hexa`) | naming legacy (Phase 8 시점, 본 cycle 측 rename X — additive_only) | naming risk → 본 disambiguation cycle 측 명시 |
| 모듈 header `Battle.net 우회 skeleton` | naming context | 본 disambiguation 측 launcher-absence 영역 |
| `community bypass 코드 0 줄` | negation (own1 enforcement) | ✓ own1 정합 |
| `community bypass 단 1 줄도 import / copy / port 금지` | negation | ✓ own1 정합 |
| `bypass_check` r0_emit name | event name (feasibility check) | functional name, DRM bypass X |
| `bypass_stats` r0_emit name | event name (stats summary) | functional name, DRM bypass X |
| `bypass_feasibility(game)` fn name | feasibility matrix | functional name, honest emit |
| `우회 가능성 검증` comment | feasibility check | functional context |
| `우회 전략` comment | launcher-absence strategy | category (a) context |
| `우회 가능 / 불가능` comment | D2R feasible / D4 not feasible | honest emit |
| `BNET_BYPASS=1` env name | launcher-absence flag (env hint to game.exe) | functional flag, DRM bypass 의미 X |

모든 "bypass" / "우회" 측 occurrence 는 (1) negation context (own1 enforcement), (2) functional name (feasibility matrix), (3) launcher-absence context — 즉 **DRM bypass / account auth bypass / Warden bypass / hook chain shim / signature spoof / NOP-patch / memory scan dodge 의미 측 occurrence 0건**. 명명 자체 측 risk 만 disambiguation 필요.

### 2.5 cross-reference — sister 8-cluster member 비교

8 battlenet cluster 중 다른 7 module 측 "bypass" 단어 측 occurrence 측 비교:

| 모듈 | "bypass" 측 occurrence | 비고 |
|---|---|---|
| `battlenet_bypass.hexa` (본 module) | 多 (모듈명 + functional name + negation) | 본 disambiguation 측 target |
| `pe_battle_net_agent_ipc_track.hexa` | 무관 | sister, untouched |
| `pe_battle_net_download_track.hexa` | 무관 | sister, untouched |
| `pe_battle_net_oauth_token.hexa` | 무관 | sister, untouched |
| `pe_battle_net_setup_install_phase.hexa` | 무관 | sister, untouched |
| `pe_battlenet_agent_http_rest.hexa` | 무관 | sister, untouched |
| `pe_battlenet_agent_sd_forging.hexa` | 무관 | sister (CrossOver Hack 23881 algorithm direction reference, but cite domain forbidden — own1 정합) |
| `pe_battlenet_setup_install_path_real.hexa` | 무관 | sister, untouched |
| `pe_battlenet_setup_static_crt_path.hexa` | 무관 | sister, untouched |

즉 8 cluster 중 본 module 만이 "bypass" 명명 측 disambiguation 필요. 다른 7 module 측 own1 위반 risk 측 absent (별도 cycle 측 audit 권고).

---

## 3. own1 정합 verdict

### 3.1 verdict — PASS

**본 cycle 측 verdict = PASS**.

근거:
- §2.2 측 line-by-line audit 결과 100% 카테고리 (a) — launcher-absence standalone enumeration + feasibility matrix honest emit
- §2.3 측 결정적 own1 정합 evidence 3 항목 (D4 NOT feasible 정직 보고 + community bypass 0줄 명시 + token forge X)
- §2.4 측 "bypass" 단어 occurrence audit — 100% negation / functional name / launcher-absence context, DRM bypass 의미 0건
- 모듈 본체 line 3-5 측 own1 enforcement 명시 (`community bypass 코드 0 줄`)

### 3.2 verdict 의 의미

PASS verdict = 본 module 측 다음 사항:
- own1 (Wine 0 / NO DRM evasion / NO account auth bypass / NO Warden circumvention) 정합 lock-in
- module rename 측 본 cycle 측 X (additive_only — annotation block prepend only)
- 6 BG-AGB rank B+C candidates own1 review gate 통과 (다음 cycle work 측 unblock)

### 3.3 verdict 가 FLAGGED 였다면 (가정)

만약 본 audit 측 module 측 카테고리 (b) DRM evasion / (c) hook chain shim 측 line 발견했다면 — verdict = `flagged_for_separate_cycle` + 본 doc 측 그 line 측 별도 cycle 측 rewrite / removal 권고 명시 + 본 cycle 측 content modification X (해당 별도 cycle 측 land 권고). 본 cycle 측 그 시나리오 측 X — verdict = PASS.

---

## 4. Future-cycle non-goals (영구 out-of-scope)

본 doc 의 verdict (PASS) 가 다음 영역 측 imply X — 다음 영역 측 영구 out-of-scope:

### 4.1 DRM evasion (영구 X)

- DRM ticket 위조 / spoof / replay
- license activation 측 우회 (fraud / counterfeit)
- always-online DRM 측 server response 위조 (D4 측 NOT feasible 정직 보고 영구 lock-in)
- ProductDB version handshake 측 spoof
- Battle.net region routing 측 위조

### 4.2 Account authentication bypass (영구 X)

- OAuth bearer token 측 forge / spoof / pre-image attack
- Battle.net 계정 측 탈취 / fraud
- credential stuffing / brute force / dictionary attack
- Battle.net 계정 측 위협 행위 (사용자 보호 영역)

### 4.3 Warden anti-cheat bypass (영구 X)

- Warden user-mode hook chain bypass / shim
- IAT / EAT / inline user hook NOP-patch
- memory scan dodge / signature spoof / pre-image attack
- ticket validation 측 spoof / 우회
- HWID / machine_id 위조 / spoof
- module enumeration 측 spoof (PEB Ldr loaded module list 위조)
- process tree fingerprint / syscall sequence pattern / file integrity hash 측 spoof
- anti-debug 측 회피 (RDTSC timing spoof / NtQueryInformationProcess hook)
- Warden detection circumvention 의 어떤 형태도

### 4.4 GameGuard kernel-mode AC bypass (non-Blizzard family이지만 동일 분류, 영구 X)

- GameGuard kernel driver 측 SSDT / IRP / inline kernel hook bypass
- Lineage Classic / Aion / B&S / L2 측 GameGuard family 측 회피
- (별도 — `docs/GAMEGUARD_HONEST_FREEZE.md` 측 retired_intentional posture lock-in)

### 4.5 Wine / CrossOver / Whisky / GPTK staging patch import (영구 X — own1 정합)

- Wine staging branch 측 patch import / cite
- CrossOver bottle 측 module fetch / port
- Whisky wrapper 측 import
- Apple GPTK metal translation layer 측 import / shim

---

## 5. Cross-link

본 doc 측 reference cross-link (각 path repo-relative):

### 5.1 sister freeze docs cluster (3+1)

| path | role | track |
|---|---|---|
| `docs/GAMEGUARD_HONEST_FREEZE.md` | sister freeze doc (kernel-mode AC) | Track-E |
| `docs/WARDEN_HONEST_FREEZE.md` | sister freeze doc (user-mode AC) | Track-J |
| `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` | sister freeze doc (network protocol) | Track-L |
| `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` (본 doc) | own1 disambiguation (loader naming) | **Track-P** |

4 doc cluster — 각 doc 은 다른 surface (kernel AC / user AC / network protocol / loader naming) 측 honest documentation, 모두 own1 (Wine 0) + own2 (honest emit) 정합.

### 5.2 module + annotation block

| path | role |
|---|---|
| `lib/loader/battlenet_bypass.hexa` | 본 disambiguation 측 target module (8 battlenet cluster member, Phase 8) |
| 모듈 측 prepend annotation block (Track-P 명시) | 본 doc 측 cross-reference (additive only, content rewrite X) |

### 5.3 battlenet family

| path | role |
|---|---|
| `.roadmap.battlenet` | game roadmap (mk2 SSOT, 본 cycle 측 untouched — Track-P 측 .roadmap touch X) |
| `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` | per-game 11 fan-out (battlenet 진입점 + 본 module 측 cluster member 명시 source) |
| `state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker` | Track-A (offline shim cond.1/cond.2 met) |
| `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker` | Track-J (Warden honest freeze) |
| `state/markers/airgenome_gamebox_d2r_realm_protocol_honest_freeze_landed.marker` | Track-L (D2R realm protocol honest freeze) |
| `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` | **본 cycle marker (Track-P)** |
| `airgenome/doc/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed_2026_05_03.ai.md` | **본 cycle handoff (Track-P)** |

### 5.4 retired_intentional / online expansion lock-in / honest disambiguation 3 posture

| posture | 적용 영역 | 본 cycle 의 정합 |
|---|---|---|
| retired_intentional (영구 X) | lineage_classic_r28 / d4 / cs2 / delta_force / elden_ring | 영향 X (D4 측 NOT feasible 정직 보고 측 본 module 측 정합) |
| online expansion lock-in (a+ path) | diablo2_resurrected online (Track-J) | 영향 X (Track-J 측 별도 SSOT) |
| **honest disambiguation (naming clarification)** | **본 cycle Track-P** | **본 doc 측 1차 사례** |

Track-P = 첫 honest naming disambiguation cycle. 다른 module 측 동일 disambiguation 권고는 별도 cycle 측 evaluate (e.g. 다른 module 측 "bypass" / "shim" / "hack" 단어 occurrence audit).

---

## 6. 6 BG-AGB rank B+C unblock confirmation

본 cycle 측 disambiguation land 시점에서 다음 6 BG-AGB rank B+C candidates own1 review gate 통과 확인:

| candidate | rank | own1 review gate status (본 cycle land 후) |
|---|---|---|
| `loader_native_helper` | B | UNBLOCKED — own1 review gate 통과 (battlenet_bypass disambiguation land 측 전제 조건 충족) |
| `perf_manjeom` | B | UNBLOCKED |
| `perf_game_d2r` | B | UNBLOCKED |
| `perf_game_d4` | C | UNBLOCKED (D4 retired_intentional posture 영구 lock-in 정합 — 본 disambiguation 측 module 측 D4 NOT feasible 정직 보고 정합) |
| `closure_orchestrator` | C | UNBLOCKED |
| `battlenet_bypass` | (본 module) | UNBLOCKED — 본 cycle 측 자체 verdict PASS |

**unblock 의 의미**:
- 다음 cycle work (위 6 candidates) 측 own1 misinterpretation risk 측 disambiguation precedent 정합
- 본 doc 측 verdict structure (PASS / FLAGGED 4 카테고리 분석) 측 반복 적용 가능
- module rename 측 X — naming legacy 유지 (additive_only)
- 본 disambiguation cycle 측 file scope (1 prepend + 3 NEW) 측 disjoint 정합 (다른 6 candidates 측 untouched)

**unblock 의 NOT 의미**:
- 6 candidates 측 자동 own1 정합 X — 각 candidate 측 별도 audit + 별도 cycle land
- 6 candidates 측 본 cycle 측 implementation X
- 6 candidates 측 별도 marker / handoff land 시점 측 own1 verdict 별도 명시 권고

---

## 7. raw#10 honest C3 caveats (≥ 6 items, naming disambiguation specific)

C1 — **본 doc 측 verdict = PASS 의 정확한 의미**: PASS = §2 의 line-by-line audit 결과 module 본체 측 카테고리 (a) launcher-absence standalone enumeration + feasibility matrix honest emit only. 카테고리 (b) (c) 측 0줄. 본 verdict 는 module 본체 (348 baseline LOC) 측 정합이며, 별도 cycle 측 module modification 시 verdict 재confirm 권고.

C2 — **module rename 측 영구 X (additive_only)**: 본 cycle 측 module rename (e.g. `battlenet_bypass.hexa` → `battlenet_launcher_absence.hexa`) 측 X. additive_only / destructive_ops 0 spirit 정합. naming legacy 유지하면서 prepend annotation block + 본 disambiguation doc 측 disambiguation reference. 다음 cycle 측 rename 권고도 X — 모듈명 측 git history / cross-reference / sister 8-cluster member naming convention 측 disruption risk.

C3 — **모듈 본체 측 fn / self_test / main / TSV schema / r0_emit content modification 0건**: 본 cycle 측 module 측 annotation block PREPEND only — 기존 line 측 deletion / modification 0건. `git diff lib/loader/battlenet_bypass.hexa` 측 pure insertion (no `-` line) 정합 verification.

C4 — **6 BG-AGB rank B+C candidates 측 own1 정합 별도 audit 권고**: §6 측 unblock confirmation 은 own1 review gate 통과 의미만 — 각 candidate 측 actual own1 정합 review 별도 cycle. 본 doc 측 6 candidates 측 implementation / modification 영역 X.

C5 — **`BNET_BYPASS=1` env name 측 launcher-absence flag 정합**: §2.4 측 audit 결과 `BNET_BYPASS=1` env 측 launcher-absence flag (env hint to game.exe — "launcher 부재 시 단독 실행" 의미) — DRM bypass / account auth bypass / Warden bypass 의미 0건. 그러나 future cycle 측 env name 측 disambiguation rename (e.g. `BNET_LAUNCHER_ABSENT=1`) 측 evaluate 가능 — 본 cycle 측 X (additive_only).

C6 — **own1 정합 evidence 3 항목 측 영구 lock**: §2.3 측 (1) D4 NOT feasible 정직 보고 + (2) community bypass 0줄 명시 + (3) token forge X — 3 항목 측 module 측 own1 정합 lock-in 영구. 별도 cycle 측 module modification 시 위 3 항목 preservation 권고.

C7 — **8 battlenet cluster sister 7 module 측 own1 audit 별도 cycle 권고**: §2.5 측 비교 결과 7 sister module 측 "bypass" 명명 측 occurrence 0건 — 그러나 own1 정합 측 별도 audit 권고 (e.g. `pe_battlenet_agent_sd_forging.hexa` 측 CrossOver Hack 23881 algorithm direction reference 측 cite domain forbidden 정합 verify). 본 cycle 측 7 sister module 측 untouched 정합 lock-in.

C8 — **Track-P 측 첫 honest naming disambiguation cycle**: 본 cycle = airgenome-gamebox 측 첫 honest naming disambiguation. 다른 module 측 동일 disambiguation 권고 (e.g. 다른 module 측 "bypass" / "shim" / "hack" 단어 occurrence audit) 는 별도 cycle 측 evaluate. 본 cycle 측 그 영역 측 X.

C9 — **본 doc 측 path / cross-link 측 repo-relative 정합 (raw 15 env_lazy)**: 본 doc 측 모든 path 는 repo-relative — `lib/...`, `docs/...`, `.roadmap.<game>`, `state/markers/...`. 사용자별 absolute path 0건, env() 측 lazy resolve 정합.

C10 — **본 cycle 의 silent-land + raw 168 minimum-viable additive**: 본 cycle 측 hexa CLI runtime emit 미verify (predecessor cycle posture 동일), schema-validated only. sha256 pin + python3 json.tool parse + wc -l + grep audit 측 verification.

---

## 8. 출처 (외부 공개 자료, 2026-05-03)

본 doc 측 cite 는 module 본체 + sister freeze docs + per-game roadmaps marker 한정. 우회 / community bypass 자료 cite 영구 X.

- airgenome-gamebox `lib/loader/battlenet_bypass.hexa` (본 disambiguation 측 target module, Track-P 측 annotation prepend 후 379 LOC)
- airgenome-gamebox `docs/GAMEGUARD_HONEST_FREEZE.md` (sister cycle, Track-E kernel-mode counterpart)
- airgenome-gamebox `docs/WARDEN_HONEST_FREEZE.md` (sister cycle, Track-J user-mode counterpart)
- airgenome-gamebox `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` (sister cycle, Track-L network protocol counterpart)
- airgenome-gamebox `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (8 battlenet cluster source)
- airgenome-gamebox `state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker` (Track-A offline shim cond.1/cond.2 met)
- airgenome-gamebox `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker` (Track-J)
- airgenome-gamebox `state/markers/airgenome_gamebox_d2r_realm_protocol_honest_freeze_landed.marker` (Track-L)

---

*written 2026-05-03 — Track-P own1 disambiguation cycle. naming clarification land 완료 (no DRM evasion / no account auth bypass / no Warden circumvention / no hook-chain shim / no NOP-patch / no memory-scan dodge / no signature spoof / no module rename / no content modification). `lib/loader/battlenet_bypass.hexa` 측 verdict = PASS — module 본체 측 카테고리 (a) launcher-absence standalone enumeration + feasibility matrix honest emit only. airgenome-gamebox 측 own1 (Wine 0) + own2 (honest emit) 영구 정합 reflect. 6 BG-AGB rank B+C candidates own1 review gate 통과 확인.*
