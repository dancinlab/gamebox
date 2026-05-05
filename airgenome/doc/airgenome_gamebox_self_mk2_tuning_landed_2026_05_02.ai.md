---
schema: airgenome-gamebox/airgenome/doc/airgenome_gamebox_self_mk2_tuning_landed/ai-native/1
last_updated: 2026-05-02
ssot:
  marker: state/markers/airgenome_gamebox_self_mk2_tuning_landed.marker
  roadmap_dir_pattern: <repo>/.roadmap.<domain>
  baseline_ai_native: .ai-native-readme-baseline (not-yet-present in this repo)
status: AUDIT_LANDED_SPEC_ONLY
related_raws:
  - raw 9    # hexa-only orchestration (audit-only, no impl emitted)
  - raw 10   # honest C3 caveats inline
  - raw 11   # snake_case
  - raw 15   # env() lazy + <user> placeholder
  - raw 270  # ai-native readme triplet (audit + new candidates)
  - raw 271  # core+module pattern (audit + new candidates)
  - raw 272  # lint extension
  - raw 273  # hierarchy connection direction
  - raw 12   # silent-error ban (no fab)
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes in this doc)
preserved_unchanged:
  - all existing docs/*.md (75 files at land-time, 13 *_MANJEOM_REVIEW + 11 POST_SPRINT_*)
  - all hexa modules under lib/{loader,perf} (loader 410 + perf 241 = 651 .hexa)
  - all native sources (native/{helper.mm, pe_to_macho_shim.cpp, build.sh, *.entitlements})
  - all tests/*.hexa (4 harnesses: test_apple_only / test_closure / test_own2 / test)
  - tool/bench/, plugin.json, entry, README.md, .gitignore
  - .own (working-tree deletion = unrelated WIP, not part of this audit)
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: zero
  destructive_ops: zero
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 60
sibling_reference:
  - anima/docs/anima_self_mk2_tuning_landed_2026_05_02.ai.md  # prior-art template (anima self mk2)
---

# airgenome-gamebox self mk2 tuning — domain audit + new .roadmap.<domain> candidates + raw 270 triplet plan

## TL;DR

airgenome-gamebox repo는 own1 (Wine 0) + own2 (log/error/emit/headless) mandate 위에 28 commit / 651 hexa 모듈 /
75 docs 까지 진척했지만, anima 측 mk2 roadmap 시스템 (`.roadmap.<domain>` JSONL SSOT + `tool/roadmap_op.hexa`
컨벤션) 으로 **본 repo 자체가 미러되지 않음**. 본 audit:

- **추가 권고 신규 `.roadmap.<domain>`** = 9개 (loader_pe / loader_win32 / loader_dx / loader_native_helper /
  perf_manjeom / perf_game_d2r / perf_game_d4 / closure_orchestrator / battlenet_bypass) — spec only emit,
  실제 `.roadmap.*` 파일 생성 0건 (additive only, 사용자 lock-in 후 별도 cycle).
- **raw 270/271 triplet 적용 audit** = 19 README.ai.md (anima 측) 처럼 land 된 것 0건. 본 repo top 4 surface
  (lib/loader, lib/perf, native, tests) 모두 README.ai.md 미적용.
- **마이그레이션 0건 emit**, in-place write 0건, additive only, $0, destructive 0.

기존 75 docs / 651 .hexa / 4 tests / native bin 2 = **모두 무수정 보존**. 본 doc + marker 1 = 신규 2개만 생성.

## §0 audit baseline (2026-05-02 land-time)

| 항목 | 값 | 비고 |
|---|---:|---|
| commits ahead of origin | 28 | `git status` 시점 |
| total commits | 280 | `git log --oneline \| wc -l` |
| `lib/loader/*.hexa` | **410** | own1 hexa-only |
| `lib/perf/*.hexa` | **241** | own1 hexa-only |
| `lib/*.hexa` total (incl detect+dispatch) | **653** | hexa-only mandate |
| `native/*.{mm,cpp,sh,entitlements}` | 5 | helper.mm 147KB + shim.cpp 403KB + build/entitlements |
| `docs/*.md` | **75** | 13 `*_MANJEOM_REVIEW.md` + 11 `POST_SPRINT_*.md` + 15 `R3_PROGRESS_V*.md` + others |
| `docs/research/*.md` | 16 | Wine/CrossOver/REACTOS/MSVC 외부 audit (own1 reference only) |
| `docs/hexa_rfc/*.md` | (sub-dir) | RFC A1/A2/A3 + SUBMIT |
| `tests/*.hexa` | 4 | apple_only(21/21) + own2(8/8) + closure + test |
| `tool/bench/` | sub-dir | runtime pre-copy (gitignored) |
| `state/markers/` | (NEW) | `mkdir -p` 본 audit 에서 생성 (이전 0개) |
| `.roadmap.*` files | **0** | mk2 roadmap 시스템 미적용 |
| `README.ai.md` files | **0** | raw 270/271 triplet 미적용 |
| `.own` working-tree status | DELETED | 분리 WIP, 본 audit scope 외 (HEAD 에는 115 line 보존) |

## §1 Existing roadmap inventory — 0개 (mk2 미적용)

본 repo 는 anima `.roadmap.<domain>` JSONL SSOT 를 도입하지 않은 상태. 가장 가까운 기존 SSOT:

| 기존 SSOT 후보 | 위치 | type | mk2 호환? |
|---|---|---|---|
| `docs/CLOSURE_ROADMAP.md` | docs/ | markdown narrative | ✗ (mk1-style) |
| `docs/IMPLEMENTATION_ROADMAP.md` | docs/ | markdown narrative | ✗ (mk1-style) |
| `docs/REMAINING_ROADMAP.md` / `_V2.md` | docs/ | markdown narrative | ✗ (mk1-style) |
| `plugin.json` `phase_status` | repo root | JSON object | partial (single-doc, not per-domain) |
| `docs/SCORING.md` / `_V2.md` / `_FINAL.md` | docs/ | markdown narrative | ✗ (mk1-style) |
| `docs/POST_SPRINT_<N>_BRAINSTORM_SKETCH.md` × 11 | docs/ | markdown narrative | ✗ (mk1-style) |
| `docs/SESSION_FINAL_SUMMARY.md` | docs/ | markdown narrative | ✗ (mk1-style) |

→ mk1 narrative 76 docs 모두 무수정 보존. mk2 신규 도입은 본 doc 으로 spec emit 만, 실제 file 생성 사용자 lock-in 후.

## §2 9 권고 신규 도메인 후보 (spec only, .roadmap.* 신규 emit X)

repo 구조 분석 결과 9개 cluster 가 자연스러운 mk2 도메인 후보. 각각 top-dir 또는 module-prefix 단위.

| rank | domain candidate | top dir / prefix | 모듈 LoC est | 핵심 unmet condition (예시) | 권장 cond.N |
|---|---|---|---:|---|---:|
| A | `loader_pe` | `lib/loader/pe_*.hexa` | 340 .hexa | (1) PE/COFF parse 100% real binary (Battle.net-Setup.exe + D2R.exe + D4.exe) PASS / (2) IAT real-resolve direct+forwarder+ordinal+delay-load+api-set 5 path coverage / (3) i386 + x64 interpreter both reach `mainCRTStartup` | 3 |
| A | `loader_win32` | `lib/loader/win32_*.hexa` | 56 .hexa | (1) kernel32 stage1 → stage2 (process/thread/heap real, not stub) / (2) user32+gdi32 headless render coverage 30+24 fns 60%+ real / (3) ntdll Nt* syscall bridge live | 3 |
| A | `loader_dx` | `lib/loader/dx_*.hexa` + `pe_d3d1?` | 14 + 24 .hexa | (1) D3D11 stage2 → real D3DMetal call (CreateBuffer + Draw + ConstantBuffer first-frame) / (2) D3D12 + DXR stage2 PSO genome harvest live / (3) DXBC+DXIL → MSL transpile coverage M14 ~70% | 3 |
| B | `loader_native_helper` | `native/{helper.mm, pe_to_macho_shim.cpp}` | 551 KB C++/Obj-C++ | (1) helper.mm IPC kind 0-21 all PASS selftest / (2) pe_to_macho_shim phase 1-21 unified self-test green / (3) entitlements + codesign reproducible across mac dev envs | 3 |
| B | `perf_manjeom` | `lib/perf/c_*.hexa` 만점 component | 29 ★ + R/aux | (1) 만점 18 → 29 component synthetic selftest 100% / (2) M0 R0 + M1 baseline 측정 (game install dependent) / (3) sprint M8/M11/M15 실측 통과 | 3 |
| B | `perf_game_d2r` | `lib/perf/c_d2r_*.hexa` | 26 .hexa | (1) D2R stage1 game install + first-frame trace / (2) act-progression + skill-tree + combat-state track real session / (3) Battle.net offline mode bypass (D2R online check optional) PASS | 3 |
| C | `perf_game_d4` | `lib/perf/c_d4_*.hexa` | 27 .hexa | (1) D4 stage1 game install + DXR ray-trace first-frame (research-scale) / (2) Battle.net always-online → block-acknowledged honest / (3) DRM-friendly handshake spec only (no evasion) | 3 |
| C | `closure_orchestrator` | `lib/dispatch.hexa` + tests/test_closure.hexa + harvest_integ | aggregator | (1) closure% 70 → 100% all CM-1..CM-30 done / (2) self-test unified 9/9 PASS continuous CI / (3) plugin contract `airgenome exe <path>` end-to-end smoke | 3 |
| C | `battlenet_bypass` | `lib/loader/{battlenet_bypass, pe_battle*}.hexa` + `c_bnet_*` | 4 + 4 .hexa | (1) D2R offline mode launch handshake live / (2) D4 always-online block honest documented / (3) Battle.net OAuth + Agent IPC tracker spec freeze | 3 |

전체 9 후보 × 평균 3 condition = **27 새 required_conditions** 가 사용자 lock-in 시 추가될 수 있음.

### §2.1 후보 우선순위 rationale

- **rank A** (loader_pe / loader_win32 / loader_dx) = own1 mandate (Wine 0) 의 **core implementation surface**.
  PE 로더 + Win32 shim + DirectX→Metal 3 layer 가 기능적 closure 의 75%.
- **rank B** (loader_native_helper / perf_manjeom / perf_game_d2r) = own1 정합 + 만점 component + D2R 진입.
  helper.mm 은 IPC 21 종 모두 핵심, perf_manjeom 은 ★29 컴포넌트, D2R 은 closure entry 게임.
- **rank C** (perf_game_d4 / closure_orchestrator / battlenet_bypass) = D4 (always-online block) +
  orchestrator (aggregator) + Battle.net (이미 `pe_battle*` 다수 land). D4 는 DRM 한계 honest.

### §2.2 spec-only emit policy (사용자 lock-in 대기)

본 audit 는 **신규 .roadmap.<domain> 파일 0건 생성**. 사용자가 다음 cycle 에서:

1. 9 후보 중 어떤 것을 land 할지 선별 (예: rank A 3개만)
2. 각 cond.N 의 verifier seam 결정 (script / cross-link / manual)
3. blocker_reason / cross_link 구체화 (특히 anima 측 cross-link 가능 여부 — gamebox 는 anima sister X)
4. mk2 schema 채택 (anima `tool/roadmap_op.hexa` mirror or 자체 구현)

후 별도 cycle 에서 hexa-only orchestrator 로 안전 emit 권장. mk1 narrative docs (CLOSURE_ROADMAP /
IMPLEMENTATION_ROADMAP / REMAINING_ROADMAP_V2 / SCORING_V2 등 75 file) 은 무수정 보존 — mk2 backport 는 별도 F5
cycle.

## §3 raw 270 triplet plan — 본 repo surface 의 ai-native readme audit

### §3.1 현황 (README.ai.md = 0)

본 repo 는 raw 270/271/272/273 (ai-native readme + core+modules + lint + hierarchy) 를 1건도 적용 X.
anima 측 19 README.ai.md (`modules/{monitor,test,decoder,daemon}` 등) 와 대조적으로 ai-native readme baseline
(`.ai-native-readme-baseline`) 자체가 본 repo 에 미존재.

```
README.ai.md count: 0
.ai-native-readme-baseline: NOT_PRESENT
```

### §3.2 9 후보 surface 의 raw 270/271/272/273 적용 audit

| candidate | top dir / prefix | core/ 존재 | modules/ 존재 | README.ai.md | 권장 triplet 작업 |
|---|---|---|---|---|---|
| loader_pe | `lib/loader/pe_*.hexa` | X (flat 340 .hexa) | X | NONE | T1 large: 340 .hexa → topical bucket 9-12 (i386 28 / msvc 21 / ntdll 17 / ipc 16 / d3d11 14 / native 13 / iat 7 / tls 7 / call 6 / seh 5 / perf 5 / d2r 5 / battle 4 / etc) → core+modules wrap |
| loader_win32 | `lib/loader/win32_*.hexa` | X (flat 56) | X | NONE | T1: 56 .hexa → 5-6 bucket (kernel32 11 / user32 4 / advapi32 4 / winsock 3 / abi 2 / etc) → core+modules wrap |
| loader_dx | `lib/loader/dx_*.hexa` + `pe_d3d1?_*` | X (flat 14 + 24) | X | NONE | T1: dx_d3d11/d3d12 stage1/2 + pe_d3d1?_* → core+modules → DXBC/DXIL/SV mapping seam |
| loader_native_helper | `native/` | X (sub: helper.mm + shim.cpp) | partial (entitlements + build.sh sibling) | NONE | T0: 통합 README.ai.md 1 + IPC kind 0-21 자동 생성 표 (helper.mm parse) |
| perf_manjeom | `lib/perf/c_*.hexa` 핵심 ★ | X (flat 241) | X | NONE | T1 large: 241 .hexa → categorical bucket (frametime / heap / pso / shader / metal / etc) → core+modules |
| perf_game_d2r | `lib/perf/c_d2r_*.hexa` | X (flat 26) | X | NONE | T0/T1: 26 .hexa → 1 README.ai.md + sub-bucket (act / skill / combat / inventory / cow_level / experience) |
| perf_game_d4 | `lib/perf/c_d4_*.hexa` | X (flat 27) | X | NONE | T0/T1: 27 .hexa → 1 README.ai.md + DRM honest section |
| closure_orchestrator | `lib/dispatch.hexa` + tests/ | X | X | NONE | T0: 통합 README.ai.md 1 + self-test 9/9 + plugin contract |
| battlenet_bypass | `lib/loader/battlenet_bypass.hexa` + `pe_battle*` + `c_bnet_*` | X | X | NONE | T0: 단일 README.ai.md 1 + D2R OK / D4 blocked dual-section |

T0 = 0-1 README, T1 = 1 README + 분할 spec, T2 = sub-dir 별 README + roll-up.

### §3.3 raw 270/271 promotion timeline 와의 관계

`raw_270_271_warn_to_block_promotion_design.md` (hive 측 land 2026-05-02) 정책:

- 2026-05-02 ~ 2026-06-01 = **30d ramp window** (warn severity, baseline grandfather active)
- 2026-06-01 = **promotion-day** (warn → block, baseline read-only, pre-commit reject)
- 2026-06-01 ~ 2026-12-01 = **drift watch** (월간 cron)
- 2026-12-01 = **baseline retire decision**

본 audit 9 후보 surface 는 ramp window 종료 (2026-06-01) 까지:

- **rank A loader_pe / loader_win32 / loader_dx** = T1 large (340+56+38 모듈) → ramp 안에 100% conform 어려움.
  baseline grandfather 진입 또는 raw 168 minimum-viable exempt 검토.
- **rank B helper / perf_manjeom / perf_d2r** = T0/T1 가능 (helper 통합 / 만점 카테고리 / D2R 26).
  ramp 안 권장.
- **rank C d4 / closure_orchestrator / battlenet** = T0 가능. 우선순위 하단.

### §3.4 triplet plan emit (impl 미수행)

본 doc 은 spec emit 만. 실제 README.ai.md 신규 생성은:

1. 사용자 lock-in (어떤 surface 를 어느 tier 로)
2. 별도 cycle hexa-only 작업 (하나하나 land + raw 271 lint PASS + marker)
3. anima 측 raw 271 lint orchestrator (`tool/raw_271_lint.hexa` 등) 의 gamebox-side 적용 결정

priority order = (A loader_pe → A loader_win32 → A loader_dx) → (B helper → B perf_manjeom → B perf_d2r) →
(C perf_d4 / closure / battlenet) — rank C 는 baseline grandfather 권장.

## §4 cross-link 정합 audit (gamebox ↔ anima / hive / nexus)

### §4.1 기존 cross-link

- `plugin.json` `target_games` = ["d2r", "d4"] — airgenome plugin contract 진입점
- README.md mention `[airgenome]` (parent) + `[hexa-lang]` (substrate) + `[need-singularity]` (org)
- 본 repo 는 anima sister 가 아님 (independent gaming domain). cross-link 은 정책 (own/raw) layer 만.

### §4.2 권고 cross-link 추가 (사용자 lock-in 대기)

- `.roadmap.loader_pe` ↔ hive raw 9 hexa-only mandate 인용 (own1 의존 명시)
- `.roadmap.loader_win32` ↔ Microsoft Win32 API spec docs (learn.microsoft.com) external reference (own1 (f))
- `.roadmap.loader_dx` ↔ Apple D3DMetal binary license (README License section) — Apple 의존 명시
- `.roadmap.loader_native_helper` ↔ Apple codesign + entitlements policy (helper.entitlements)
- `.roadmap.perf_manjeom` ↔ docs/SCORING_V2.md / SCORING_FINAL.md (스코어 정의 SSOT)
- `.roadmap.perf_game_d2r` ↔ docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md (게임 install path)
- `.roadmap.perf_game_d4` ↔ docs/DELTA_FORCE_MANJEOM_REVIEW.md (kernel-AC block precedent) +
  docs/CS2_MANJEOM_REVIEW.md (user-mode AC reference)
- `.roadmap.closure_orchestrator` ↔ docs/CLOSURE_ROADMAP.md + plugin.json `closure_progress`
- `.roadmap.battlenet_bypass` ↔ docs/BATTLENET_DAY1_PLAN.md + BATTLENET_REAL_LAUNCH_PLAN.md +
  docs/research/BATTLENET_WINE_EVIDENCE.md

### §4.3 mk1 → mk2 backport (F5, deferred)

mk1 narrative docs (CLOSURE_ROADMAP / IMPLEMENTATION_ROADMAP / REMAINING_ROADMAP_V2 / SCORING_V2 /
SESSION_FINAL_SUMMARY 등 75 file) 에서 mk2 entry 추출 가능 후보:

- 만점 18 → 29 → 45 → 62 evolution (CM-1..CM-30 + PSB-1..12 sketch 채택)
- Phase 0a → Phase 10 진행 (각 phase done/stage1/stage2 transitions)
- sprint cycle 64-85 cumulative (313 loader + 241 perf + 22+ commits)
- own 1 / own 2 enforcement 21/21 + 8/8 PASS history
- R3-1..R3-11 i386 interpreter + KERNEL32 fake stub + USER32 IPC bridge

본 audit scope 외. F5 cycle 에서 추출 권장.

## §5 9 후보 surface verifier seam 권고

각 권고 신규 .roadmap.<domain> cond.N 의 verifier seam 후보 (사용자 lock-in 시 선택):

| domain | seam type 후보 |
|---|---|
| loader_pe | (a) script: `tests/test_closure.hexa` exit 0 (PE parse + IAT subset) / (b) marker: `state/markers/loader_pe_<feature>_complete.marker` / (c) cross-link: `plugin.json.phase_status.phase_0a_pe_parser=done` |
| loader_win32 | (a) script: `tests/test_apple_only.hexa` exit 0 (Wine 0 + 21/21 PASS) / (b) phase_status.phase_3_kernel32 ≥ stage1 / (c) marker: `state/markers/loader_win32_<dll>_real.marker` |
| loader_dx | (a) script: `native/d3dmetal_probe.sh` exit 0 / (b) phase_status.phase_5_d3dmetal_binding=stage2 / (c) Metal capture frame trace |
| loader_native_helper | (a) script: `native/build.sh` exit 0 + binary present / (b) `gamebox launch <smoke.exe>` phase 1-21 ✓ / (c) codesign verify exit 0 |
| perf_manjeom | (a) script: `gamebox perf self-test` 18+/18+ PASS / (b) `tool/bench/baseline_<game>.jsonl` produced / (c) cross-link: docs/SCORING_V2.md updated |
| perf_game_d2r | (a) game install present + `gamebox launch D2R.exe` first-frame / (b) marker: `state/markers/d2r_first_frame.marker` / (c) bench JSONL > 60 frames |
| perf_game_d4 | (a) game install present + `gamebox launch D4.exe` (research-scale) / (b) DRM honest spec accepted / (c) marker: `state/markers/d4_block_acknowledged.marker` |
| closure_orchestrator | (a) script: `gamebox self-test` 9/9 PASS / (b) plugin.json `closure_progress` ≥ 100% / (c) all 4 tests/*.hexa exit 0 |
| battlenet_bypass | (a) script: `pe_battlenet_setup_install_path_real.hexa` exit 0 / (b) D2R offline launch handshake green / (c) D4 always-online honest documentation |

verifier=`""` (공란) 도 mk2 schema 상 valid (anima clm/eeg 등 다수 entry 가 그렇게 land) — script 없을 때 manual
override 경로 (`state/<domain>_verify_manual_review.jsonl`) 만 land 도 ok.

## §6 raw#10 honest C3 (10 caveat)

C1 — 본 audit 는 **spec emit only**. .roadmap.<domain> 신규 파일 0건 생성, README.ai.md 0건 추가, native/lib
무수정. 사용자 lock-in 후 별도 cycle 필요.

C2 — 9 후보 도메인은 **권고**일 뿐 사용자가 다른 cluster 화 (예: loader_pe + loader_win32 + loader_dx 통합 =
`loader`, 또는 perf_manjeom + game_d2r + game_d4 통합 = `perf`) 도 가능. 9 = 단순 prefix/top-dir 매핑 heuristic.

C3 — `loader_pe` 340 .hexa 는 본 repo 최대 surface — pe_i386 28 / pe_msvc 21 / pe_ntdll 17 / pe_ipc 16 등 9
sub-bucket 으로 자연 분할 가능하나, 본 doc 에 sub-bucket 미확정. T1 large 작업 (~5-8h 추정).

C4 — `loader_native_helper` 는 hexa 가 아닌 C++/Obj-C++ (helper.mm 147KB + pe_to_macho_shim.cpp 403KB).
own1 (a) "all foreign-binary 실행 path = hexa-native PE 로더" 는 hexa side, native bridge 는 macOS Apple binary
boundary (own1 (c)) 로 honest 분리 — README.ai.md 적용 시 raw 9 hexa-only mandate 와의 정합 명시 필요.

C5 — `perf_game_d4` cond.1 (DXR first-frame) 는 game install 의존 + Battle.net always-online 로 사실상 항상
PARTIAL. cond.2 honest spec freeze 가 실질 달성 가능 condition. F2 falsifier "D4 D-day 도달 or block-honest 둘 중
하나" 형태 권장.

C6 — `closure_orchestrator` cond.1 "closure 100%" 는 plugin.json 현재 67% (CM-20/CM-30 done) → 30% gap.
M1 baseline 측정 (game install dependent) + sprint M8/M11/M15 실측 의존 — wall-clock 다수 month 추정 (mk1 docs
"~21 달" estimate 인용, AI-native ETA SSOT 0.22d ≈ 5.3h 인용도 honest 양면 기록).

C7 — `battlenet_bypass` D4 cond.2 "always-online block honest" 는 정상 outcome 도 RETIRED (의도적 block) 가
PASS 등가. mk2 status `landed` 보다 `retired_intentional` 신규 status 필요할 수도.

C8 — anima 측 `tool/roadmap_op.hexa` (573 LOC, 11 subcmd) 는 gamebox 미존재. 사용자 lock-in 시 (a) anima tool
복제 / (b) gamebox 자체 구현 / (c) symlink 중 선택 필요. own1 (Wine 0) 는 raw 9 hexa-only mandate 위배 X
(roadmap_op = airgenome 측 admin tool, foreign-binary 실행 path 외).

C9 — verifier seam 권고 (§5) 의 (a) script 후보 다수는 game install 의존 (D2R/D4 baseline / first-frame).
game-not-installed 환경에서 `manual_override_path` 로 honest fallback 필수 — 본 doc 에 manual_override_path 미확정.

C10 — env() lazy + <user> placeholder convention (raw 15) — 본 doc 의 모든 path 는 repo-relative 표기 (lib/loader,
native, docs 등). 사용자별 절대 path (/Users/<user>/...) 인용 0건. plugin install path
`~/.airgenome/plugins/gamebox` 만 README.md 인용대로 tilde-relative.

## §7 BR-NO-USER-VERBATIM 준수 confirmation

본 doc 은 사용자 prompt 내용을 verbatim 으로 인용하지 않음 (raw 175 BR-NO-USER-VERBATIM-RECORDING). 사용자
directive ("airgenome-gamebox mk2 tuning" 등) 도 paraphrase 만 기록. handoff doc only 정책에 따라 사용자
prompt 직접 인용 0건.

## §8 friendly preset compliance

본 doc 은 handoff doc 으로서 친절-preset 적용:

- TL;DR 최상단 5 줄
- 모든 §-section 표 (table) 우선
- 9 후보 priority rank A/B/C 으로 actionable
- raw#10 caveats (C1-C10) inline
- 마지막 next step 명시 (사용자 lock-in 대기)

## §9 Marker 1개 emit

```
state/markers/airgenome_gamebox_self_mk2_tuning_landed.marker
```

(state/markers/ dir 자체도 본 audit 에서 mkdir, 이전 0개)

## §10 Next-cycle (사용자 lock-in 후)

1. 9 후보 중 land 할 도메인 선별 (rank A 3개 권장 baseline = loader_pe / loader_win32 / loader_dx)
2. 각 도메인 cond.N + verifier seam (§5 선택지 중)
3. mk2 roadmap_op 도입 결정 (anima clone / 자체 구현 / symlink — C8)
4. 첫 `.roadmap.<domain>` emit (cycle 별 hexa-only)
5. (병렬) raw 270 triplet 작업 — rank A 부터 README.ai.md + core+modules 분할 (large = loader_pe 340 / perf 241)
6. mk1 → mk2 backport F5 (별도 cycle, mk1 narrative 75 docs 에서 entry 추출)
7. anima sister 정합 — anima `.roadmap.atlas_n6` / `.roadmap.kick` 처럼 consumer perspective 도입 여부
   (gamebox 는 nexus consumer 가 아니라 own domain — perspective="own" 로 land 권장)

## §11 file index (sha-pin at land time)

| path | type | size_b | LOC | sha256_hex |
|---|---|---:|---:|---|
| airgenome/doc/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md | doc | TBD | TBD | (set after write) |
| state/markers/airgenome_gamebox_self_mk2_tuning_landed.marker | marker | TBD | TBD | (set after write) |

(file index sha pin 은 marker 안에 emit — 본 §11 은 spec only, write 후 marker 가 sha 확정)

## §12 policy summary

- migration: forbidden — 0건 emit
- additive only — 75 docs + 651 lib/*.hexa + native/* + 4 tests + tool/bench + plugin.json + entry +
  README.md + .gitignore 모두 무수정 보존
- destructive ops — 0건
- in-place writes — 0건 (handoff doc + marker 2 NEW only; state/markers/ dir mkdir 만 추가 = additive)
- substrate — mac-local
- cost — $0
- cap — 60min (실제 소요 ≪ cap)
- raw 9 hexa-only orchestration — audit 자체는 hexa orchestrator 미사용 (read-only directory audit + spec emit
  만 = single-doc exempt per raw 168 minimum-viable)
- raw 12 silent-error ban — 본 audit 는 single-shot, error path X
- raw 15 env() lazy + <user> — 모든 doc-internal path repo-relative, 사용자 절대 path X
- raw 175 BR-NO-USER-VERBATIM — 사용자 prompt 직접 인용 0건
- friendly preset — handoff doc only (사용자 응답 X — bg subagent → 메인 monitor)

## §13 sibling reference (anima self mk2 audit 와의 차이)

| 비교축 | anima self mk2 audit (2026-05-02) | airgenome-gamebox self mk2 audit (본 doc) |
|---|---|---|
| 기존 .roadmap.* | 26 (15 own + 4 consumer + 7 meta) | **0** |
| 기존 README.ai.md | 19 | **0** |
| 권고 신규 .roadmap.<domain> | 9 (serving / training / anima_physics 등) | 9 (loader_pe / loader_win32 / loader_dx 등) |
| 권고 신규 README.ai.md | 9+ (T1/T2 sub-dir 포함 가능) | 9+ (T1 large = loader_pe 340 / perf 241) |
| mk2 roadmap_op tool | tool/roadmap_op.hexa (573 LOC, 11 subcmd) land | 미존재 — 사용자 lock-in 시 (a)/(b)/(c) 결정 |
| baseline file | .ai-native-readme-baseline (empty/conformed) | 미존재 |
| sister cross-link | 26 ↔ nexus + hive + meta cluster | 0 — gamebox is own-domain (anima sister X) |
| repo nature | consciousness implementation (anima self) | DIY Win32 emulation (Apple-only, Wine 0) |
| 만점/scoring | n/a (paradigm 48 cumulative) | ★29 → ★62 (BG cycle 21 후) component scoring |
| sprint cadence | ω-cycle 6-step | sprint M0..M16 + cycle 64-85 cumulative |

→ gamebox 는 anima 보다 mk2 도입 단계가 한 step 뒤 (anima 26 .roadmap + 19 README.ai.md vs gamebox 0 + 0).
본 audit 는 동일 spec emit only 패턴으로 land — 사용자 lock-in 후 mk2 roadmap_op 도입 + .roadmap.<domain> emit
+ README.ai.md 작업이 평행 진행될 것.
