# airgenome-gamebox — Track-P `battlenet_bypass` own1 Disambiguation Landed (2026-05-03)

> **TL;DR**: `lib/loader/battlenet_bypass.hexa` (8 battlenet loader cluster member, Phase 8 baseline) 측 모듈명 측 "bypass" 단어 측 own1 (Wine 0 / NO DRM evasion / NO account auth bypass / NO Warden circumvention) misinterpretation risk 정직하게 disambiguate. 모듈 본체 fn / self_test / main / TSV schema / r0_emit content modification 0건 — annotation block PREPEND only (+31 LOC). `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` mk1 narrative NEW (338 LOC, 9 sections). own1 정합 verdict = **PASS** (module 본체 측 카테고리 (a) launcher-absence standalone enumeration + feasibility matrix honest emit only — D4 NOT feasible 정직 보고 + community bypass 0줄 명시 + token forge X 측 3 evidence 정합). 6 BG-AGB rank B+C candidates (loader_native_helper / perf_manjeom / perf_game_d2r / perf_game_d4 / closure_orchestrator / battlenet_bypass) own1 review gate 통과 확인 (UNBLOCKED). additive_only / destructive_ops 0 / migration X / cost $0 / cap 90min / silent-land marker.

## §0 baseline

- predecessor handoff: `airgenome/doc/airgenome_gamebox_warden_honest_freeze_landed_2026_05_03.ai.md` (Track-J sister cycle, user-mode AC honest freeze)
- predecessor marker: `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker`
- predecessor 8-cluster source marker: `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (loader_pattern: `lib/loader/(battlenet_bypass + pe_battle_net_* 4 + pe_battlenet_* 4)`)
- 본 cycle 측 `.roadmap.battlenet` touch X (Track-P 측 .roadmap 측 modification 0건)

## §1 user_directive_paraphrase

BG subagent — Track-P own1 disambiguation land for airgenome-gamebox: `lib/loader/battlenet_bypass.hexa` (8 battlenet loader cluster, Phase 8 baseline 348 LOC) 모듈명 측 "bypass" 단어 측 own1 (Wine 0 / NO DRM evasion / NO account auth bypass / NO Warden circumvention) misinterpretation risk 정직하게 disambiguate. 1 file PREPEND comment block (annotation only, content rewrite X) + 1 NEW mk1 narrative spec disambiguation doc (`docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md`) + 1 NEW handoff + 1 NEW marker. 6 BG-AGB rank B+C candidates own1 review gate 통과 확인. policy: additive_only / migration forbidden / destructive_ops 0 / $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker.

## §2 module finding summary

### §2.1 scope category

- **category (a) Battle.net launcher-absence standalone path** — 100% 정합
- category (b) Battle.net DRM evasion attempt — 0줄
- category (c) Battle.net session/agent shim — 0줄
- category (d) something else — 0줄

### §2.2 module LOC

- baseline (Track-P 전): 348 LOC
- after Track-P annotation prepend: 379 LOC
- delta: +31 LOC (annotation block only)

### §2.3 module fn / self_test / main 측 카테고리 분포

| fn / 영역 | 카테고리 | 분석 |
|---|---|---|
| `battlenet_launcher_path(prefix)` | (a) | Wine prefix 측 표준 launcher path 추정 — string concat only |
| `battlenet_deps()` | (a) | launcher 측 dep enumeration — 명칭 reference only, dep forge X |
| `auth_token_cache_path()` | (a) | cache path spec only — 실 token 측 사용자 manual 추출 영역, token forge X |
| `direct_launch_spec(game, prefix)` | (a) | JSON-like spec emit — 실 spawn X. `BNET_BYPASS=1` env 측 launcher-absence flag |
| `bypass_feasibility(game)` | (a) | feasibility matrix — D2R feasible / D4 NOT feasible 정직 보고 (online-only / server-side auth / always-online DRM blocker list) |
| `bypass_stats()` | (a) | stats `[d2r=feasible, d4=blocked]` honest report |
| `self_test()` | (a) | invariant verification |
| `main()` dispatch | (a) | CLI dispatch (self-test / launcher-path / deps / token-cache / spec / feasibility / stats) |

### §2.4 결정적 own1 정합 evidence 3 항목

1. D4 측 NOT feasible 정직 보고 (`bypass_feasibility("d4")` 측 `[false, "D4 is online-only — server-side auth blocks any launcher bypass", ["server_side_auth", "always_online_drm", "ProductDB version handshake"]]`)
2. community bypass 0줄 명시 (모듈 header line 3-5 "Wine / Lutris / Heroic / community bypass 단 1 줄도 import / copy / port 금지")
3. token forge X (`auth_token_cache_path()` 측 path spec only — 실 token 측 사용자 manual 추출 영역)

### §2.5 "bypass" 단어 occurrence audit 요약

- 100% negation context (own1 enforcement) / functional name (feasibility matrix) / launcher-absence context
- DRM bypass / account auth bypass / Warden bypass / hook chain shim / signature spoof / NOP-patch / memory scan dodge 의미 측 occurrence 0건
- `BNET_BYPASS=1` env 측 launcher-absence flag — DRM bypass 의미 X

## §3 annotation block diff

### §3.1 LOC delta on module

- before: 348 LOC, 12256 bytes
- after: 379 LOC, 14788 bytes
- delta: +31 LOC, +2532 bytes
- diff kind: pure insertion (no `-` line in `git diff`)

### §3.2 annotation block 위치

- 모듈 file 측 line 1 측 PREPEND
- 기존 line 1-348 측 deletion / modification 0건
- 기존 file header comment (Phase 8 — Battle.net 우회 skeleton + own 1 enforcement + own 2 mandate + 문제 정의 + 우회 전략 + feasibility 매트릭스 + 범위) 측 모두 보존

### §3.3 annotation block 측 14 sub-항목

1. `OWN1 DISAMBIGUATION — TRACK P 2026-05-03` 헤더
2. SCOPE (in-scope, own1 정합) — launcher-absence standalone enumeration + feasibility matrix
3. NOT IN SCOPE (영구 out-of-scope) — DRM evasion / account auth bypass / Warden bypass / GameGuard bypass / online ToS violation tooling / Battle.net 계정 측 위협 행위 / Wine etc.
4. VERDICT — PASS (module honestly enumerates feasibility + spec emit only)
5. REFERENCE — `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` cross-link
6. SISTER cluster — Track-J Warden + Track-E GameGuard + Track-L D2R realm protocol
7. 6 BG-AGB rank B+C unblock 명시
8. additive_only / destructive_ops 0 명시

## §4 disambiguation doc summary

### §4.1 path + LOC

- path: `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md`
- LOC: 338
- bytes: 23309
- sha256: `fdb579a1c5be96ec772da86bd3f6d4f5c0010fa1afd6565ed5b2d33caa6a07ce`

### §4.2 sections (9)

| § | title |
|---|---|
| §0 | 본 doc 의 목적과 한계 |
| §1 | Module identity (path + cluster + emit pattern + own1 enforcement 측 module-internal 명시) |
| §2 | Scope finding — 4 category 분석 (정의 + line-by-line audit + own1 정합 evidence 3 항목 + "bypass" occurrence audit + sister 8-cluster member 비교) |
| §3 | own1 정합 verdict — PASS |
| §4 | Future-cycle non-goals (영구 out-of-scope) — 5 카테고리 (DRM evasion / account auth bypass / Warden bypass / GameGuard bypass / Wine staging patch import) |
| §5 | Cross-link — sister freeze docs cluster 4 + module + battlenet family + 3 posture 비교 |
| §6 | 6 BG-AGB rank B+C unblock confirmation |
| §7 | raw#10 honest C3 caveats (10 items) |
| §8 | 출처 |

### §4.3 honest C3 caveats count

- 10 caveats inline (C1-C10) in §7
- ≥6 정합 (본 prompt 측 minimum threshold)

### §4.4 mirror style

- `docs/GAMEGUARD_HONEST_FREEZE.md` (Track-E mk1 narrative)
- `docs/WARDEN_HONEST_FREEZE.md` (Track-J mk1 narrative — 가장 가까운 sister)

## §5 own1 verdict

**verdict = PASS**.

근거:
- §2 측 line-by-line audit 결과 100% 카테고리 (a) launcher-absence standalone enumeration + feasibility matrix honest emit
- 결정적 own1 정합 evidence 3 항목 (D4 NOT feasible 정직 보고 + community bypass 0줄 명시 + token forge X)
- "bypass" 단어 occurrence audit — 100% negation / functional name / launcher-absence context
- 모듈 본체 line 3-5 측 own1 enforcement 명시

verdict 의 의미:
- own1 (Wine 0 / NO DRM evasion / NO account auth bypass / NO Warden circumvention) 정합 lock-in
- module rename 측 본 cycle 측 X (additive_only)
- 6 BG-AGB rank B+C candidates own1 review gate 통과

## §6 6 BG-AGB rank B+C unblock confirmation

| candidate | rank | own1 review gate status |
|---|---|---|
| `loader_native_helper` | B | UNBLOCKED |
| `perf_manjeom` | B | UNBLOCKED |
| `perf_game_d2r` | B | UNBLOCKED |
| `perf_game_d4` | C | UNBLOCKED (D4 retired_intentional + module 측 D4 NOT feasible 정직 보고 정합) |
| `closure_orchestrator` | C | UNBLOCKED |
| `battlenet_bypass` | (본 module) | UNBLOCKED — 본 cycle 자체 verdict PASS |

unblock 의 의미:
- 다음 cycle work 측 own1 misinterpretation risk 측 disambiguation precedent 정합
- 본 doc 측 verdict structure (PASS / FLAGGED 4 카테고리 분석) 측 반복 적용 가능
- module rename X — naming legacy 유지 (additive_only)

unblock 의 NOT 의미:
- 6 candidates 측 자동 own1 정합 X — 각 candidate 측 별도 audit 권고
- 6 candidates 측 본 cycle 측 implementation 0건
- 6 candidates 측 별도 marker / handoff land 시점 측 own1 verdict 별도 명시 권고

## §7 raw#10 honest C3 caveats (≥6 items, naming disambiguation specific)

C1 — **본 cycle = 첫 honest naming disambiguation cycle**: airgenome-gamebox 측 첫 honest naming disambiguation. 다른 module 측 동일 disambiguation 권고 (e.g. 다른 module 측 "bypass" / "shim" / "hack" 단어 occurrence audit) 는 별도 cycle 측 evaluate. 본 cycle 측 그 영역 측 X.

C2 — **module rename 측 영구 X (additive_only)**: 본 cycle 측 `battlenet_bypass.hexa` → 다른 이름 rename 측 X. naming legacy 유지하면서 prepend annotation block + 본 disambiguation doc 측 disambiguation reference. 다음 cycle 측 rename 권고도 X — 모듈명 측 git history / cross-reference / sister 8-cluster member naming convention 측 disruption risk.

C3 — **모듈 본체 측 fn / self_test / main / TSV schema / r0_emit content modification 0건 lock-in**: 본 cycle 측 module 측 annotation block PREPEND only — 기존 line 측 deletion / modification 0건. `git diff lib/loader/battlenet_bypass.hexa` 측 pure insertion (no `-` line) 정합 verification.

C4 — **6 BG-AGB rank B+C candidates 측 own1 정합 별도 audit 권고**: §6 측 unblock confirmation 은 own1 review gate 통과 의미만 — 각 candidate 측 actual own1 정합 review 별도 cycle. 본 doc 측 6 candidates 측 implementation / modification 영역 X.

C5 — **`BNET_BYPASS=1` env name 측 future cycle disambiguation rename evaluate 가능**: §3.5 측 audit 결과 `BNET_BYPASS=1` env 측 launcher-absence flag — DRM bypass 의미 X. 그러나 future cycle 측 env name 측 disambiguation rename (e.g. `BNET_LAUNCHER_ABSENT=1`) 측 evaluate 가능. 본 cycle 측 X (additive_only).

C6 — **own1 정합 evidence 3 항목 측 영구 lock**: §2.4 측 (1) D4 NOT feasible 정직 보고 + (2) community bypass 0줄 명시 + (3) token forge X — 3 항목 측 module 측 own1 정합 lock-in 영구. 별도 cycle 측 module modification 시 위 3 항목 preservation 권고.

C7 — **8 battlenet cluster sister 7 module 측 own1 audit 별도 cycle 권고**: §2.5 측 비교 결과 7 sister module 측 "bypass" 명명 측 occurrence 0건 — 그러나 own1 정합 측 별도 audit 권고 (e.g. `pe_battlenet_agent_sd_forging.hexa` 측 CrossOver Hack 23881 algorithm direction reference 측 cite domain forbidden 정합 verify). 본 cycle 측 7 sister module 측 untouched 정합 lock-in.

C8 — **annotation block 측 LOC 측 module 본체 LOC 비율**: annotation block +31 LOC / baseline 348 LOC ≈ 8.9% — substantial annotation 영역. 그러나 추가만 정합 (deletion / modification 0건). 모듈 본체 정보 measurement (TSV schema / fn signature) 영역 측 annotation block 측 영향 X.

C9 — **본 doc 측 path / cross-link 측 repo-relative 정합 (raw 15 env_lazy)**: 본 doc 측 모든 path 는 repo-relative — `lib/...`, `docs/...`, `state/markers/...`. 사용자별 absolute path 0건, env() 측 lazy resolve 정합.

C10 — **본 cycle 의 silent-land + raw 168 minimum-viable additive**: 본 cycle 측 hexa CLI runtime emit 미verify (predecessor cycle posture 동일), schema-validated only. sha256 pin + python3 json.tool parse + wc -l + grep audit 측 verification.

C11 — **`.roadmap.battlenet` cond.3 evidence row 측 추가 X**: Track-J Warden honest freeze 측 cond.3 evidence row 추가 (in-place edit) 측 cycle pattern 과 다름 — 본 Track-P 측 .roadmap 측 touch X (file scope 측 forbidden). 본 cycle 측 disambiguation 측 .roadmap 측 SSOT 측 N/A. 별도 cycle 측 .roadmap.battlenet cond.3 evidence row 측 본 doc cross-link 추가 권고 가능 (별도 cycle 측 in-place edit 권고).

C12 — **6 BG-AGB rank B+C unblock 측 자동 implementation X**: §6 측 unblock 은 review gate 통과 의미 — 6 candidates 측 본 cycle 측 implementation 0건. 다음 cycle 측 각 candidate 측 별도 audit + 별도 cycle land. 본 doc 측 6 candidates 측 implementation 영역 X.

## §8 next-cycle hooks

- **별도 cycle 측 6 BG-AGB rank B+C candidates land** — 각 candidate 측 own1 audit + 별도 cycle land (loader_native_helper / perf_manjeom / perf_game_d2r / perf_game_d4 / closure_orchestrator / battlenet_bypass cycle work)
- **별도 cycle 측 8 battlenet cluster sister 7 module own1 audit** — `pe_battlenet_agent_sd_forging.hexa` (CrossOver Hack 23881 cite domain forbidden) + 다른 6 sister module 측 own1 정합 audit
- **별도 cycle 측 다른 module 측 honest naming disambiguation** — 다른 module 측 "bypass" / "shim" / "hack" 단어 occurrence audit (예: `lib/loader/pe_*.hexa` cluster 측 "shim" 단어, `lib/perf/c_*.hexa` 측 "hack" 단어 등)
- **별도 cycle 측 `.roadmap.battlenet` cond.3 evidence row 측 본 doc cross-link 추가** — Track-J 측 Warden honest freeze cycle pattern 동일 (in-place edit, 별도 cycle 측 land 권고)
- **별도 cycle 측 `BNET_BYPASS=1` env name 측 disambiguation rename evaluate** — `BNET_LAUNCHER_ABSENT=1` 등 명시적 명명 측 transition 권고 (additive only — 별도 cycle 측 evaluate)
- **사용자 lock-in 후 production smoke** — 사용자 정식 lock-in (L1+L2 + Track-P disambiguation acknowledgment) 후 D2R offline launch 측 본 module 측 spec emit verify (별도 cycle, 사용자 자체 책임 영역)

## §9 file index

### §9.1 files_created

| path | type | LOC | bytes | sha256 |
|---|---|---|---|---|
| `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` | mk1_narrative (disambiguation spec) | 338 | 23309 | fdb579a1c5be96ec772da86bd3f6d4f5c0010fa1afd6565ed5b2d33caa6a07ce |
| `airgenome/doc/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed_2026_05_03.ai.md` | handoff_doc | (본 doc) | (self-referential) | (self-referential) |
| `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` | marker | (별도) | (별도) | (self-referential) |

### §9.2 files_modified

| path | type | LOC delta | bytes delta | sha256 (before / after) |
|---|---|---|---|---|
| `lib/loader/battlenet_bypass.hexa` | annotation_prepend (additive only, content rewrite X) | +31 (348→379) | +2532 (12256→14788) | 11bd70fd2c371cd6b6d0f3784260242bf51ebd1b81466deed2719fca3eddf750 / 92cb3e538b8fe41108a44aeac57cda1ae82b0d25c23b3e39f547a4ce8eab3a70 |

### §9.3 files_deleted / dirs_created

- files_deleted: 0
- dirs_created: 0

### §9.4 preserved_unchanged

- `.roadmap.*` 측 14 file (battlenet / diablo2_resurrected / diablo4 / lineage_classic_r28 / lineage_m / lineage_w / wow / cs2 / delta_force / elden_ring / loader_pe / loader_win32 / loader_dx / purple_launcher) untouched
- 8 battlenet cluster sister 7 module untouched
- `lib/perf/` 측 모든 module untouched
- `tests/` 측 모든 file untouched
- `native/` 측 모든 file untouched
- `tool/` 측 모든 file untouched
- 다른 `docs/*.md` 측 modification 0건 (read-only reference only)

## §10 policy summary

- migration: forbidden
- changes: additive_only (1 file PREPEND comment block + 3 NEW files; existing code/comments preserved verbatim)
- destructive_ops: 0
- cost_usd: 0
- substrate: mac-local
- cap_minutes: 90
- BR-NO-USER-VERBATIM: true
- friendly_preset: true
- silent-land: marker-only emit
- own1: Wine 0 / NO DRM evasion / NO account auth bypass / NO Warden circumvention
- own2: honest emit, explicit verdict (PASS — not silent_swallow)
- raw 9 (hexa-only) / 10 (≥6 caveats — 12 inline) / 11 (snake_case) / 12 (silent error ban) / 15 (env_lazy) / 168 (minimum-viable additive) / 175 (BR-NO-USER-VERBATIM)

---

*written 2026-05-03 — Track-P own1 disambiguation cycle handoff. naming clarification land 완료. own1 verdict = PASS. 6 BG-AGB rank B+C candidates UNBLOCKED. additive_only / destructive_ops 0 / migration X / silent-land marker.*
