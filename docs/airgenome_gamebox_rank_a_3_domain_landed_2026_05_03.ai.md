---
schema: airgenome-gamebox/docs/airgenome_gamebox_rank_a_3_domain_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_rank_a_3_domain_landed.marker
  roadmap_files:
    - .roadmap.loader_pe
    - .roadmap.loader_win32
    - .roadmap.loader_dx
  predecessor_handoff: docs/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
status: LANDED_RANK_A_3_DOMAIN
related_raws:
  - raw 9    # hexa-only orchestration (audit-only, no impl emitted)
  - raw 10   # honest C3 caveats inline
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 15   # env() lazy + <user> placeholder
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes in this doc)
  - raw 270  # ai-native readme triplet (T1 large/std pending separate cycle)
  - raw 271  # core+module pattern (pending)
  - raw 272  # lint extension (pending)
  - raw 273  # hierarchy connection direction
preserved_unchanged:
  - all existing docs/*.md (76 files at land-time, including predecessor mk2 handoff)
  - all hexa modules under lib/{loader,perf} (loader 414 + perf 241 = 655 .hexa)
  - all native sources (native/{helper.mm, pe_to_macho_shim.cpp, build.sh, *.entitlements})
  - all tests/*.hexa (4 harnesses: test_apple_only / test_closure / test_own2 / test)
  - tool/bench/, plugin.json, entry, README.md, .gitignore
  - predecessor marker state/markers/airgenome_gamebox_self_mk2_tuning_landed.marker
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: zero
  destructive_ops: zero
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 60
  br_no_user_verbatim: true
  friendly_preset: true
sibling_reference:
  - .roadmap.serving (anima)  # peer perspective JSONL header pattern source
  - .roadmap.atlas_n6 (anima) # consumer perspective comparison
  - .roadmap.kick (anima)     # status enum draft pattern
---

# airgenome-gamebox rank A 3 domain landed — loader_pe / loader_win32 / loader_dx (BG-AGB follow-up)

## TL;DR

BG-AGB 9 candidate spec-only audit (2026-05-02 predecessor) 의 rank A 3 후보를 실제 `.roadmap.<domain>` JSONL
header 로 land. 마이그레이션 0건, additive only, 75+1 mk1 narrative docs + 655 .hexa + native + tests 모두 무수정.

- 신규 3 .roadmap.<domain> = `.roadmap.loader_pe` (340 .hexa surface) + `.roadmap.loader_win32` (56 .hexa shim) +
  `.roadmap.loader_dx` (38 .hexa DirectX→Metal). 각 JSONL header 1줄 + comment lines 4 = 5 LOC each.
- 각 도메인 = peer perspective + 3 required_conditions (own1 21/21 PASS + own2 8/8 PASS reflect) + 1 blocker.
- **신규 status enum 후보 명시** = `retired_intentional` (block-acknowledged = PASS-equivalent). D4 always-online
  block 측 / kernel-AC class 측 / D3DMetal binary licence boundary 측 3 blocker 모두 후보 정식 채택 사용자
  lock-in 대기.
- 신규 도메인 status = `active` (3 cond.N 모두 partial, 사용자 lock-in 후 verifier 실행 cycle 대기)
- 새 doc 1 + 새 marker 1 + 새 .roadmap 3 = **5 file 추가 only**, 기존 file 0건 수정.

## §0 baseline (2026-05-03 land-time)

| 항목 | 2026-05-02 (predecessor) | 2026-05-03 (본 land) | delta |
|---|---:|---:|---:|
| `.roadmap.<domain>` files | 0 | **3** | +3 |
| handoff docs (`*_landed_2026_05_*.ai.md`) | 1 | **2** | +1 |
| `state/markers/*` | 1 | **2** | +1 |
| `docs/*.md` total | 75 | 76 | +1 (본 doc) |
| `lib/loader/*.hexa` | 410 | 414 | +4 (별도 BG-R3 cycle, 본 작업 외) |
| `lib/perf/*.hexa` | 241 | 241 | 0 (본 작업 외) |
| native/* | 5 | 5 | 0 |
| tests/*.hexa | 4 | 4 | 0 |
| `README.ai.md` files | 0 | 0 | 0 (raw 270/271 triplet 별도 cycle, baseline grandfather candidate) |

## §1 3 신규 .roadmap.<domain> 요약

### §1.1 `.roadmap.loader_pe`

| field | value |
|---|---|
| name | loader_pe |
| perspective | peer |
| top dir | `lib/loader/pe_*.hexa` (340 .hexa surface) |
| required_conditions | 3 (PE/COFF parse 100% real binary / IAT 5-path coverage / i386+x64 mainCRTStartup) |
| blocker count | 1 (`loader_pe.blk.1` D4 always-online class) |
| status | active |
| status enum candidate | `retired_intentional` (D4 측) |
| size_b | 5236 |
| sha256 | `807b65cafba7160be1de8116d5f1129b8bc8d225c7393356d1124ec53526d323` |
| triplet_audit_tier | T1_large (340 .hexa → 9-12 sub-bucket 분할 별도 cycle) |
| sister cross-link | loader_win32 / loader_dx / loader_native_helper / perf_manjeom / battlenet_bypass / closure_orchestrator |

3 cond 모두 `status: partial` (own1 21/21 + own2 8/8 PASS reflect 됐지만 production binary 측 first-frame
real-resolve coverage 수치화 미수행).

### §1.2 `.roadmap.loader_win32`

| field | value |
|---|---|
| name | loader_win32 |
| perspective | peer |
| top dir | `lib/loader/win32_*.hexa` (56 .hexa shim) |
| required_conditions | 3 (kernel32 stage1→stage2 process/thread/heap real / user32+gdi32 60% headless coverage / ntdll Nt* syscall bridge live) |
| blocker count | 1 (`loader_win32.blk.1` kernel-AC DLL inject class) |
| status | active |
| status enum candidate | `retired_intentional` (kernel-AC 측) |
| size_b | 5745 |
| sha256 | `2aab6633f926fa25002353f656f713b8d79d567cb6b1135e5066ca4aa79d06fa` |
| triplet_audit_tier | T1 (56 .hexa → 5-6 sub-bucket 분할 별도 cycle) |
| sister cross-link | loader_pe / loader_dx / loader_native_helper / perf_manjeom / battlenet_bypass / closure_orchestrator |

R3 wave18-A USER32=0 stable (recent commits) = stage1 안정화 단계 reflect, stage2 real coverage matrix SSOT 별도.

### §1.3 `.roadmap.loader_dx`

| field | value |
|---|---|
| name | loader_dx |
| perspective | peer |
| top dir | `lib/loader/dx_*.hexa` + `lib/loader/pe_d3d1?_*.hexa` (38 .hexa) |
| required_conditions | 3 (D3D11 stage2 first-frame Buffer/Draw/CB / D3D12+DXR PSO genome harvest live / DXBC+DXIL→MSL 70% coverage) |
| blocker count | 1 (`loader_dx.blk.1` D3DMetal binary licence + D4 always-online cross-blocked) |
| status | active |
| status enum candidate | `retired_intentional` (D3DMetal licence boundary 측) |
| size_b | 6260 |
| sha256 | `4836a3943d8984e397d2eb417da8536ddbb842a7ca58126d352776256dd17dff` |
| triplet_audit_tier | T1 (38 .hexa → 5-6 sub-bucket 분할 별도 cycle) |
| sister cross-link | loader_pe / loader_win32 / loader_native_helper / perf_manjeom / perf_game_d4 / battlenet_bypass / closure_orchestrator |

cond.3 (MSL transpile coverage) 는 game install 무관 측정 가능 → 우선 진척 권장 (cond.1/cond.2 game install
+ D3DMetal licence transitive block).

## §2 status enum 신규 후보 `retired_intentional` 명시

### §2.1 motivation

3 신규 .roadmap.<domain> 의 blocker 모두 동일 패턴:

- `loader_pe.blk.1` = D4 always-online (Battle.net launcher DRM, own1 (Wine 0) 정합 → 우회 X 정책)
- `loader_win32.blk.1` = kernel-AC DLL inject class (Vanguard / EAC, Delta Force / CS2 mk1 review precedent)
- `loader_dx.blk.1` = D3DMetal binary licence boundary (Apple D3DMetal binary 의존, README License 정합)

3 측 모두 **honest documentation = PASS-equivalent**. 기존 mk2 schema status enum
(`unmet | partial | met` + sister-domain 측 `landed | active | draft | blocked | open`) 만으로는 "block 을
의도적으로 honest 하게 받아들임 (block-acknowledged)" semantics 표현 미흡.

### §2.2 후보 정의

```
status: retired_intentional
semantics: block-acknowledged honest documentation = PASS-equivalent
verifier: spec freeze documentation land + cross_link to mk1 narrative review docs
not_evasion_clause: own1 (Wine 0) + own2 (honest emit) 정합 — DRM/AC bypass 시도 X
```

### §2.3 적용 범위 (사용자 lock-in 대기)

| blocker | candidate scope | mk1 narrative cross-ref |
|---|---|---|
| loader_pe.blk.1 (D4 always-online) | spec freeze docs/D4_DRM_HONEST_FREEZE.md (가칭) | docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md / docs/BATTLENET_REAL_LAUNCH_PLAN.md |
| loader_win32.blk.1 (kernel-AC) | spec freeze docs/KERNEL_AC_HONEST_FREEZE.md (가칭) | docs/DELTA_FORCE_MANJEOM_REVIEW.md / docs/CS2_MANJEOM_REVIEW.md |
| loader_dx.blk.1 (D3DMetal licence) | spec freeze docs/D3DMETAL_LICENCE_HONEST_FREEZE.md (가칭) | README.md License section / docs/D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md |

본 doc 은 **spec only** — 신규 spec freeze docs 0건 emit, 사용자 lock-in 후 별도 cycle.

### §2.4 enum 정합 방향 (anima 측 호환)

anima `.roadmap.*` 26 file (`.roadmap.kick` 등) 측 status enum 도 `retired_intentional` 누락. 사용자 lock-in 시
gamebox 측 first-mover, anima 측 backport 별도 결정 가능.

## §3 own1 + own2 reflect 정합 audit

### §3.1 own1 (Wine 0 / hexa-only) 정합

| 신규 도메인 | own1 cond 정합 | reflection |
|---|---|---|
| loader_pe | (a) hexa-native PE 로더 / (c) macOS Apple binary boundary native helper | cond.1 (PE parse) + cond.2 (IAT real-resolve) + cond.3 (CRT init) 모두 hexa-native (Wine 0) |
| loader_win32 | (a) hexa-native Win32 shim / (c) macOS posix syscall pass-through | cond.1 (kernel32 real) + cond.2 (user32+gdi32 real) + cond.3 (ntdll bridge) 모두 hexa-native |
| loader_dx | (a) hexa-native DirectX → Metal / D3DMetal binary boundary licence honest | cond.1 (D3D11 first-frame) + cond.2 (D3D12 PSO) + cond.3 (DXBC/DXIL→MSL) 모두 hexa-native, D3DMetal binary 측만 native |

3 도메인 21/21 own1 PASS reflect: tests/test_apple_only.hexa 21/21 cumulative PASS 사실을 evidence array 인용.

### §3.2 own2 (log/error/emit/headless) 정합

| 신규 도메인 | own2 cond 정합 | reflection |
|---|---|---|
| loader_pe | (a) log structured / (b) error honest / (c) emit JSON / (d) headless | cond verifier 의 status_emit (`__LOADER_PE_PARSE__` etc) = own2 (c) emit 정합 |
| loader_win32 | (b) error honest / (c) emit / (d) headless render | cond.2 (user32+gdi32 headless coverage) = own2 (d) headless mandate 정합, status_emit own2 (c) |
| loader_dx | (b) error honest / (c) emit | cond.1/2 verifier status_emit own2 (c) 정합, blocker_reason transitive 명시 own2 (b) |

3 도메인 8/8 own2 PASS reflect: tests/test_own2.hexa 8/8 cumulative PASS 사실을 evidence array 인용.

## §4 cross-link matrix

3 신규 도메인 + 6 잔존 BG-AGB 후보 (loader_native_helper / perf_manjeom / perf_game_d2r / perf_game_d4 /
closure_orchestrator / battlenet_bypass) cross-link symmetry:

| from \ to | loader_pe | loader_win32 | loader_dx | loader_native_helper | perf_manjeom | perf_game_d2r | perf_game_d4 | closure_orchestrator | battlenet_bypass |
|---|---|---|---|---|---|---|---|---|---|
| **loader_pe** | — | IAT target | pe_d3d* surface | macho bridge | — | — | — | aggregator | pe_battle* sub |
| **loader_win32** | reverse IAT | — | COM (ole32) | helper IPC | — | — | — | aggregator | — |
| **loader_dx** | pe_d3d* sub | COM prereq | — | Metal layer | — | — | DXR target | aggregator | — |

3 land 도메인 ↔ 6 spec-only 후보 간 cross-link 모두 evidence array 또는 cross_link object 안에 명시.

## §5 raw#10 honest C3 (10 caveat)

C1 — 본 land 는 **JSONL header 만**. 3 cond.N entries (`{"type":"entry",...}`) 추가는 0건. 사용자 lock-in 후
별도 cycle (verifier 실행 + status met 전환).

C2 — `.roadmap.<domain>` 신규 3 file 의 `required_conditions` 모두 `status: partial` 로 land. own1 21/21 +
own2 8/8 PASS reflect 됐지만 cond.N 별 production binary 측 first-frame coverage 수치화 미수행 = honest partial.

C3 — `retired_intentional` status enum 은 본 doc 내 **후보 명시 only**. mk2 schema 정식 채택 X, 사용자
lock-in 시 (a) gamebox 측 first-mover / (b) anima 측 backport / (c) tool/roadmap_op.hexa 측 enum validator 추가
3 결정 필요.

C4 — `.roadmap.loader_pe` cond.1 의 D4.exe parse 측 transitive block 은 `loader_pe.blk.1` 한 곳에 합쳐서
표현 (D4 always-online + DRM-friendly handshake). cond-level vs blocker-level 분리 strictness 별도.

C5 — `.roadmap.loader_win32` cond.2 (user32+gdi32 60% threshold) 의 60% 는 **mk1 narrative 측 미land
heuristic**. 정확한 threshold 는 사용자 lock-in 시 docs/SCORING_V2.md / SCORING_FINAL.md 측 cross-link 후
재정의 필요.

C6 — `.roadmap.loader_dx` cond.3 (DXBC+DXIL→MSL 70% coverage) 의 70% 도 동일 — mk1 narrative
D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md 측 M14 phase 명시 인용 only, 정확 measurement spec 미land.

C7 — 3 도메인 모두 `triplet_audit_tier` 명시 (T1_large / T1 / T1) 했지만 README.ai.md 신규 0건. raw 270/271
promotion-day 2026-06-01 까지 baseline grandfather candidate (predecessor mk2 audit §3.3 인용).

C8 — `tool/roadmap_op.hexa` (anima 측 573 LOC, 11 subcmd) 는 gamebox 미존재. 신규 3 .roadmap.<domain> 의
verifier 자동 실행 + status update 는 (a) anima tool clone / (b) gamebox 자체 구현 / (c) symlink 결정 사용자
lock-in 대기 (predecessor mk2 audit C8 인용).

C9 — 3 도메인 cond verifier 의 script path 다수 (lib/loader/iat_runtime_bind.hexa 등) 는 own1 (a) hexa-native
실제 모듈. exit_zero_means_met 는 entry-point 실행 contract 명시이지만, 모듈 자체가 standalone CLI exit-code
emit 보장 미land — `tool/bench/` runtime 측 wrapper 별도 cycle 가능.

C10 — env() lazy + <user> placeholder convention (raw 15) — 3 신규 .roadmap 의 모든 path 는 repo-relative 표기
(`lib/loader/...`, `tests/...`, `docs/...`, `state/markers/...`, `plugin.json`). 사용자별 절대 path 인용 0건.

## §6 BR-NO-USER-VERBATIM 준수 confirmation

본 doc + 3 신규 .roadmap.<domain> + 1 신규 marker 모두 사용자 prompt 내용을 verbatim 으로 인용하지 않음
(raw 175). 사용자 directive ("airgenome-gamebox rank A 3 candidate domain land" 등) 도 paraphrase 만 기록.

## §7 friendly preset compliance

본 doc 은 handoff doc 으로서 친절-preset 적용:

- TL;DR 최상단 5 줄
- 모든 §-section 표 (table) 우선
- 3 land 도메인 summary table 통일 형식
- raw#10 caveats (C1-C10) inline
- 마지막 next step 명시 (사용자 lock-in 대기)

## §8 marker emit

```
state/markers/airgenome_gamebox_rank_a_3_domain_landed.marker
```

(state/markers/ dir 은 predecessor 본 audit 에서 mkdir, 이미 존재)

## §9 next-cycle (사용자 lock-in 후)

1. 3 신규 .roadmap.<domain> 의 cond.N verifier 실행 (script invoke + status partial → met 전환)
2. `retired_intentional` status enum 정식 채택 결정 + spec freeze docs 3 land (D4 / kernel-AC / D3DMetal)
3. 잔존 6 BG-AGB 후보 (loader_native_helper / perf_manjeom / perf_game_d2r / perf_game_d4 /
   closure_orchestrator / battlenet_bypass) 별도 cycle land — rank B → rank C
4. raw 270 triplet T1 작업 — rank A 부터 README.ai.md + core+modules 분할 (large = loader_pe 340 / shim = win32 56
   / dx = 38)
5. mk2 roadmap_op tool 도입 결정 (anima clone / 자체 구현 / symlink — predecessor C8)
6. mk1 → mk2 backport F5 (별도 cycle, mk1 narrative 76 docs 에서 entry 추출)

## §10 file index (sha-pin at land time)

| path | type | size_b | LOC | sha256_hex |
|---|---|---:|---:|---|
| .roadmap.loader_pe | roadmap_jsonl | 5236 | 5 | `807b65cafba7160be1de8116d5f1129b8bc8d225c7393356d1124ec53526d323` |
| .roadmap.loader_win32 | roadmap_jsonl | 5745 | 5 | `2aab6633f926fa25002353f656f713b8d79d567cb6b1135e5066ca4aa79d06fa` |
| .roadmap.loader_dx | roadmap_jsonl | 6260 | 5 | `4836a3943d8984e397d2eb417da8536ddbb842a7ca58126d352776256dd17dff` |
| docs/airgenome_gamebox_rank_a_3_domain_landed_2026_05_03.ai.md | doc | TBD | TBD | (set after write) |
| state/markers/airgenome_gamebox_rank_a_3_domain_landed.marker | marker | TBD | TBD | (self-referential) |

## §11 policy summary

- migration: forbidden — 0건 emit
- additive only — 76 docs (75 + 본 doc) + 655 lib/*.hexa + native/* + 4 tests + tool/bench + plugin.json + entry +
  README.md + .gitignore + predecessor marker 1 모두 무수정 보존
- destructive ops — 0건
- in-place writes — 0건 (3 신규 .roadmap + 1 본 doc + 1 marker = 5 NEW only)
- substrate — mac-local
- cost — $0
- cap — 60min (실제 소요 ≪ cap)
- raw 9 hexa-only orchestration — 본 audit 자체는 hexa orchestrator 미사용 (single-doc + JSONL header emit 만 =
  predecessor 와 동일 raw 168 minimum-viable exempt)
- raw 12 silent-error ban — 3 .roadmap blocker 모두 `status: open` + `status_enum_candidate: retired_intentional`
  honest 명시
- raw 15 env() lazy + <user> — 모든 path repo-relative, 사용자 절대 path X
- raw 175 BR-NO-USER-VERBATIM — 사용자 prompt 직접 인용 0건
- friendly preset — handoff doc only (사용자 응답 X — bg subagent → 메인 monitor)

## §12 sibling reference (anima `.roadmap.*` 패턴 비교)

| 비교축 | anima `.roadmap.serving` (peer) | anima `.roadmap.atlas_n6` (consumer) | anima `.roadmap.kick` (consumer draft) | gamebox 본 land 3 도메인 (peer) |
|---|---|---|---|---|
| perspective | peer | consumer | consumer | peer |
| origin_repo | (own) | nexus | nexus | (own) |
| required_conditions count | 3 | 1 | 0 (draft) | 3 each |
| blocker count | 2 | 1 | 1 | 1 each |
| status enum used | active | active | draft | active |
| status_enum_candidate | (none) | (none) | (none) | `retired_intentional` 후보 명시 |
| triplet_audit_tier | T1 | (n/a) | (n/a) | T1_large + T1 + T1 |
| sister cross-link | voice + clm | (none) | (none) | 6-7 each |

→ gamebox peer 도메인 = anima serving 패턴 mirror, status enum 신규 후보 (`retired_intentional`) = gamebox first-
mover proposal.
