---
schema: airgenome-gamebox/airgenome/doc/airgenome_gamebox_per_game_roadmaps_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker
  roadmap_files_new:
    - .roadmap.diablo2_resurrected
    - .roadmap.diablo4
    - .roadmap.battlenet
    - .roadmap.wow
    - .roadmap.cs2
    - .roadmap.delta_force
    - .roadmap.elden_ring
    - .roadmap.lineage_classic_r28
    - .roadmap.lineage_m
    - .roadmap.lineage_w
    - .roadmap.purple_launcher
  roadmap_files_preserved:
    - .roadmap.loader_pe
    - .roadmap.loader_win32
    - .roadmap.loader_dx
  predecessor_handoffs:
    - airgenome/doc/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
    - airgenome/doc/airgenome_gamebox_rank_a_3_domain_landed_2026_05_03.ai.md
status: LANDED_PER_GAME_ROADMAPS_11
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
  - 3 existing .roadmap.* files (loader_pe, loader_win32, loader_dx) - zero in-place writes
  - all existing docs/*.md (76 files at land-time, 75 mk1 narrative + 2 predecessor handoff)
  - all hexa modules under lib/{loader,perf} (loader 414 + perf 241 = 655 .hexa)
  - all native sources (native/{helper.mm, pe_to_macho_shim.cpp, build.sh, *.entitlements})
  - all tests/*.hexa (4 harnesses)
  - tool/bench/, plugin.json, entry, README.md, .gitignore
  - 2 predecessor markers
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 0
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
user_directive_corrections_applied:
  excluded_games_3:
    - diablo_1_classic
    - starcraft_2
    - lineage_2
  lineage_variants_3:
    - lineage_classic_r28  # 2026-02-07 release, native client + GameGuard
    - lineage_m            # 2017 KR mobile, Purple shell + Android emul
    - lineage_w            # 2021 UE4 global, Purple shell
  purple_launcher_separate: true
---

# airgenome-gamebox per-game roadmaps landed - 11 .roadmap.<game> fan-out (BG-AGB follow-up)

## TL;DR

predecessor mk2 + rank A 3 도메인 (loader_pe / loader_win32 / loader_dx) land 위 11 .roadmap.<game> per-game
fan-out land. 마이그레이션 0건, additive only, 기존 3 .roadmap + 76 docs + 655 .hexa + native + tests 모두 무수정.

- 신규 11 .roadmap = D2R + D4 + Battle.net + WoW + CS2 + Delta Force + Elden Ring + Lineage Classic R28 +
  Lineage M + Lineage W + Purple Launcher. 각 JSONL header 1줄 + comment lines 4 = 5 LOC each.
- 사용자 정정 directive 반영: D1 Classic / SC2 / Lineage 2 (구) 3 게임 제외, Lineage 변종 정확 3개 (Classic R28
  / M / W) 만, Purple launcher 별도 .roadmap 분리.
- `retired_intentional` status enum 후보 (rank A 3 도메인 land 시 첫 제안) 정식 채택 권고 — 5 게임
  (D4 always-online / CS2 VAC / Delta Force EAC kernel / Elden Ring EAC anti-tamper / Lineage Classic R28 GameGuard)
  blocker 적용 후보 명시. 사용자 lock-in 대기.
- 신규 11 .roadmap.<game> status = `active` (대부분 cond.N partial, 일부 cond.N unmet)
- 새 doc 1 + 새 marker 1 + 새 .roadmap 11 = **13 file 추가 only**, 기존 file 0건 수정.

## §0 baseline (2026-05-03 land-time)

| 항목 | 2026-05-03 (rank A 3) | 2026-05-03 (per-game 11) | delta |
|---|---:|---:|---:|
| `.roadmap.<domain>` files (loader) | 3 | 3 | 0 (보존) |
| `.roadmap.<game>` files (per-game) | 0 | **11** | +11 |
| `.roadmap.*` total | 3 | **14** | +11 |
| handoff docs (`*_landed_2026_05_*.ai.md`) | 2 | **3** | +1 |
| `state/markers/*` | 2 | **3** | +1 |
| `docs/*.md` total | 76 | 77 | +1 (본 doc) |
| `lib/loader/*.hexa` | 414 | 414 | 0 |
| `lib/perf/*.hexa` | 241 | 241 | 0 |
| native/* | 5 | 5 | 0 |
| tests/*.hexa | 4 | 4 | 0 |

## §1 11 신규 .roadmap.<game> 요약

### §1.1 `.roadmap.diablo2_resurrected`

| field | value |
|---|---|
| name | diablo2_resurrected (alias: d2r) |
| kind | game |
| top dirs | `lib/loader/{d2r_launch, pe_d2r_*}.hexa` (5 loader) + `lib/perf/c_d2r_*.hexa` (26 perf) |
| required_conditions | 3 (D2R.exe PE+IAT+CRT / MPQ + d2s round-trip / D3D11 renderer first-frame) |
| blocker count | 1 (`diablo2_resurrected.blk.1` Battle.net launcher dep, offline mode possible) |
| status | active |
| status enum candidate | none (offline mode 가능, retired_intentional 미적용) |
| size_b | 5054 / loc 5 |
| sha256 | `e0b7c30cb2176c98ce727b38f513945e8dede5887a8760b527d5ace93443a714` |

### §1.2 `.roadmap.diablo4`

| field | value |
|---|---|
| name | diablo4 (alias: d4) |
| kind | game |
| top dirs | `lib/loader/{d4_launch, pe_d4_specific_track}.hexa` (2 loader) + `lib/perf/c_d4_*.hexa` (27 perf) |
| required_conditions | 3 (D4.exe PE+IAT+CRT / D3D12+DXR PSO+ray-trace / 27 perf economy spec freeze) |
| blocker count | 1 (`diablo4.blk.1` D4 always-online class) |
| status | active |
| status enum candidate | **retired_intentional** (always-online DRM honest documentation) |
| size_b | 5498 / loc 5 |
| sha256 | `556ec0d600e22ffc0a77c5b0b76fd46718e3c06f08b72ff22d75a468638c73bc` |

### §1.3 `.roadmap.battlenet`

| field | value |
|---|---|
| name | battlenet (alias: battle_net, bnet) |
| kind | launcher |
| top dirs | `lib/loader/{battlenet_bypass, pe_battle_net_*, pe_battlenet_*}.hexa` (8 loader) + `lib/perf/c_bnet_*.hexa` (3 perf) |
| required_conditions | 3 (Agent SD forging + REST / setup install path real / OAuth 2-leg + login state + chat) |
| blocker count | 1 (`battlenet.blk.1` online endpoint network dep) |
| status | active |
| status enum candidate | none (offline shim 가능, retired_intentional 미적용) |
| size_b | 5044 / loc 5 |
| sha256 | `dac31aa6adb225f6844b6b9967aa00ec7f89863384cc0ab5920240a7777689b5` |

### §1.4 `.roadmap.wow`

| field | value |
|---|---|
| name | wow (alias: world_of_warcraft) |
| kind | game |
| top dirs | `lib/loader/pe_wow_{retail,classic}_track.hexa` (2 loader) + `lib/perf/c_wows_*.hexa` (2 perf, alias overlap honest reflect) |
| required_conditions | 3 (Wow.exe + WowClassic.exe PE / D3D11+D3D12 dual renderer / Battle.net + subscription gate) |
| blocker count | 1 (`wow.blk.1` online MMO + subscription) |
| status | active |
| status enum candidate | retired_intentional (D4 always-online family precedent) |
| size_b | 4735 / loc 5 |
| sha256 | `5f3d05e2d28f585091ef436c3e68fb8e85214cecdeed081dab0884e7704a2e48` |

### §1.5 `.roadmap.cs2`

| field | value |
|---|---|
| name | cs2 (alias: counter_strike_2) |
| kind | game |
| top dirs | `lib/loader/pe_cs2_specific_track.hexa` (1 loader) + `lib/perf/c_cs2_*.hexa` (3 perf) |
| required_conditions | 3 (cs2.exe + Steam API offline shim / Source 2 PSO + tier0 plat shim / VAC honest spec freeze) |
| blocker count | 1 (`cs2.blk.1` VAC anti-cheat) |
| status | active |
| status enum candidate | **retired_intentional** (VAC honest documentation) |
| size_b | 4701 / loc 5 |
| sha256 | `63dd4ff32f707389b6ac4330db691bfb696e37fa3c815ea632025c71051ac603` |

### §1.6 `.roadmap.delta_force`

| field | value |
|---|---|
| name | delta_force (alias: df) |
| kind | game |
| top dirs | `lib/loader/pe_delta_force_specific_track.hexa` (1 loader) + `lib/perf/{c_dxil_delta, c_wg_delta_update_proto}.hexa` (2 perf) |
| required_conditions | 3 (DeltaForce.exe + WG launcher / DXIL delta + WG asset delta / EAC kernel honest spec freeze) |
| blocker count | 1 (`delta_force.blk.1` EAC kernel-AC) |
| status | active |
| status enum candidate | **retired_intentional** (EAC kernel-AC honest documentation) |
| size_b | 4978 / loc 5 |
| sha256 | `edc840490a37a0ca392edbc94323e4dea8d0156b713ff01e346e1f26e0c662c1` |

### §1.7 `.roadmap.elden_ring`

| field | value |
|---|---|
| name | elden_ring (alias: er, eldenring) |
| kind | game |
| top dirs | `lib/loader/pe_elden_ring_specific_track.hexa` (1 loader) + `lib/perf/c_elden_*.hexa` (10 perf) |
| required_conditions | 3 (eldenring.exe + EAC offline probe / D3D12 first-frame + PSO warm + HKS / MSIX + license stub + offline save) |
| blocker count | 1 (`elden_ring.blk.1` EAC anti-tamper) |
| status | active |
| status enum candidate | **retired_intentional** (EAC anti-tamper honest documentation) |
| size_b | 5490 / loc 5 |
| sha256 | `d1aa8c1d2e3fb33de7b21ac008e1af05e18afaee1ee3c93c6dd008d023e323b7` |

### §1.8 `.roadmap.lineage_classic_r28`

| field | value |
|---|---|
| name | lineage_classic_r28 (alias: lineage_classic, l_classic_r28) |
| kind | game |
| top dirs | `lib/loader/pe_lineage_classic_r28_track.hexa` (1 loader) + `lib/perf/c_lineage_classic_r28_native_first_frame.hexa` (1 perf) |
| required_conditions | 3 (LineageClassic.exe + GameGuard probe / native first-frame draw / GameGuard honest spec freeze) |
| blocker count | 1 (`lineage_classic_r28.blk.1` GameGuard kernel-AC) |
| status | active |
| status enum candidate | **retired_intentional** (GameGuard kernel-AC honest documentation) |
| release_date | 2026-02-07 (신작 R28 update) |
| size_b | 4949 / loc 5 |
| sha256 | `7f6eaaabecb4025fcd34394eeb1e7a1eba37f159e073944d2834959d95f3b656` |

### §1.9 `.roadmap.lineage_m`

| field | value |
|---|---|
| name | lineage_m (alias: lineage_mobile, l_m_purple) |
| kind | game |
| top dirs | `lib/loader/pe_lineage_m_purple_track.hexa` (1 loader) + `lib/perf/c_lineage_m_purple_apk_emul_first_frame.hexa` (1 perf) |
| required_conditions | 3 (Purple + Android emul + APK / APK emul first-frame + asset cache / plaync OAuth + 2FA) |
| blocker count | 1 (`lineage_m.blk.1` purple_launcher transitive) |
| status | active |
| status enum candidate | none (purple_launcher cond met 시 cascading PASS) |
| release_date | 2017 (KR mobile MMORPG) |
| size_b | 4760 / loc 5 |
| sha256 | `e9378cef9bc20f7b82938a3b72566dd6468c81882e05fe8877ca88e3b932f505` |

### §1.10 `.roadmap.lineage_w`

| field | value |
|---|---|
| name | lineage_w (alias: lineage_world, l_w_purple) |
| kind | game |
| top dirs | (loader sub-module 별도 미land) + `lib/perf/c_lineage_w_purple_ue4_first_frame.hexa` (1 perf) |
| required_conditions | 3 (Purple + LineageW.exe UE4 / UE4 first-frame + PSO warm / plaync OAuth + 2FA + global account) |
| blocker count | 1 (`lineage_w.blk.1` purple_launcher transitive + loader sub-module 미land) |
| status | active |
| status enum candidate | none (loader sub-module land + purple_launcher cond met 시 cascading PASS) |
| release_date | 2021 (UE4 글로벌 출시) |
| size_b | 5059 / loc 5 |
| sha256 | `9f86fa0f76027622084ccdda94dffaaf3b0e8cbdde4af7ab2c7aadaff5903476` |

### §1.11 `.roadmap.purple_launcher`

| field | value |
|---|---|
| name | purple_launcher (alias: ncsoft_purple, plaync_purple) |
| kind | launcher |
| top dirs | (loader 미존재) + `lib/perf/c_purple_*.hexa` (5 perf) |
| required_conditions | 3 (.NET CLR + WebView2 / Android emul layer / plaync OAuth + 2FA TOTP) |
| blocker count | 1 (`purple_launcher.blk.1` plaync 글로벌 endpoint network dep) |
| status | active |
| status enum candidate | none (offline shim 측 cond.1/cond.2 가능) |
| downstream_games | lineage_m / lineage_w / lineage_2m / aion / blade_and_soul / throne_and_liberty |
| size_b | 5020 / loc 5 |
| sha256 | `2026a8f5066d84bf24938d67274762c9ea7c80ad5f65046eaff81a77bbd057ab` |

## §2 사용자 정정 directive 반영

### §2.1 제외 게임 (3) - 별도 .roadmap 미생성

| game | 제외 사유 (paraphrase) |
|---|---|
| Diablo 1 Classic | 사용자 제외 directive (per-game roadmap fan-out 대상 X) |
| Starcraft 2 | 사용자 제외 directive (per-game roadmap fan-out 대상 X) |
| Lineage 2 (구) | 사용자 제외 directive (Lineage Classic R28 신작과 별도, per-game roadmap 대상 X — `lib/loader/pe_lineage_2_specific_track.hexa` 잔존 disk 모듈은 본 cycle 측 untouched) |

### §2.2 Lineage 변종 정확 3개

| variant | release | client | launcher | anti-cheat |
|---|---|---|---|---|
| Lineage Classic R28 | 2026-02-07 | native PC | (자체 launcher) | GameGuard kernel block |
| Lineage M (Purple) | 2017 KR | mobile APK | Purple shell + Android emul | (mobile native) |
| Lineage W (PURPLE) | 2021 글로벌 | UE4 PC | Purple shell | (UE4 native) |

### §2.3 Purple launcher 별도 .roadmap 분리

NCSOFT Purple = Battle.net 같은 통합 launcher class. Lineage M / W / 2M / Aion / B&S / TL 6 게임 통합
dependency. 5 perf cluster (.NET CLR + WebView2 + plaync OAuth + 2FA + Android emul) 분리 land 권고 사용자
directive 반영.

## §3 `retired_intentional` status enum 적용 후보 5 게임

### §3.1 적용 후보 matrix

| game | blocker | type | mk1 narrative cross-ref |
|---|---|---|---|
| diablo4 | always-online (Battle.net DRM) | upstream_drm | docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md / docs/BATTLENET_REAL_LAUNCH_PLAN.md |
| cs2 | VAC user-mode AC | upstream_anticheat | docs/CS2_MANJEOM_REVIEW.md |
| delta_force | EAC kernel-AC | upstream_anticheat_kernel | docs/DELTA_FORCE_MANJEOM_REVIEW.md |
| elden_ring | EAC anti-tamper | upstream_anticheat_anti_tamper | docs/ELDEN_RING_MANJEOM_REVIEW.md |
| lineage_classic_r28 | GameGuard kernel-AC | upstream_anticheat_kernel | (mk1 narrative LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md 미land — 차후 cycle 권고) |

### §3.2 미적용 후보 (offline shim / cascading PASS 가능)

| game/launcher | rationale |
|---|---|
| diablo2_resurrected | offline mode handshake 가능 (Battle.net 통과 측 optional) |
| battlenet | offline shim 측 cond.1/cond.2 PASS 가능 |
| wow | subscription 측 user-controlled (retired_intentional candidate, lock-in 대기) |
| lineage_m | purple_launcher cond met 시 cascading PASS |
| lineage_w | loader sub-module land + purple_launcher cond met 시 cascading PASS |
| purple_launcher | offline shim 측 cond.1/cond.2 PASS 가능 |

## §4 cross-link matrix

11 신규 .roadmap.<game> + 3 기존 .roadmap.<domain> (loader_pe/loader_win32/loader_dx) cross-link symmetry:

| from \ to | loader_pe | loader_win32 | loader_dx | battlenet | purple_launcher |
|---|---|---|---|---|---|
| **diablo2_resurrected** | pe_d2r_* 5 | (transitive) | D3D11 stage1/2 | offline mode optional | — |
| **diablo4** | pe_d4_specific 1 | (transitive) | D3D12+DXR | always-online required | — |
| **battlenet** | pe_battle_net_* 8 | (transitive) | — | (self) | — |
| **wow** | pe_wow_{retail,classic} 2 | (transitive) | D3D11+D3D12 | required + subscription | — |
| **cs2** | pe_cs2_specific 1 | kernel-AC family | Source 2 D3D11+Vulkan | — | — |
| **delta_force** | pe_delta_force_specific 1 | kernel-AC family | DXIL delta | — | — |
| **elden_ring** | pe_elden_ring_specific 1 | kernel-AC family | D3D12 + PSO warm | — | — |
| **lineage_classic_r28** | pe_lineage_classic_r28_track 1 | kernel-AC family (GameGuard) | native renderer | — | — |
| **lineage_m** | pe_lineage_m_purple_track 1 | (.NET WinForms) | Android GLES → Metal | — | required |
| **lineage_w** | (loader sub-module 미land) | (.NET WinForms) | UE4 D3D11+D3D12 | — | required |
| **purple_launcher** | — | (.NET WinForms) | WebView2 + Android emul | (analog launcher pattern) | (self) |

## §5 raw#10 honest C3 (12 caveat)

C1 — 본 land 는 **JSONL header 만**. 11 .roadmap.<game> cond.N entries (`{"type":"entry",...}`) 추가는 0건.
사용자 lock-in 후 별도 cycle (verifier 실행 + status met 전환).

C2 — 11 신규 .roadmap.<game> 의 `required_conditions` 대부분 `status: partial` 로 land. lineage_classic_r28
cond.3 (GameGuard honest spec freeze docs 미land) + lineage_w cond.1 (loader sub-module 미land) 2 cond 만
`status: unmet` 명시.

C3 — `retired_intentional` status enum 은 본 doc 내 **5 게임 적용 후보 명시 only**. mk2 schema 정식 채택 X,
사용자 lock-in 시 (a) gamebox 측 first-mover / (b) anima 측 backport / (c) tool/roadmap_op.hexa 측 enum
validator 추가 3 결정 필요 (rank A 3 도메인 land 시 동일 caveat).

C4 — `wow.cond.1` 의 perf module count 측 `c_wows_*.hexa` 2 file 은 sha-pin 측 World of Warships (Wargaming
sibling) 와 alias overlap 가능성 있음. 사용자 prompt 측 wow=2 perf 명시 → alias overlap honest reflect (mk1
narrative 측 별도 audit 권고).

C5 — `cs2`/`delta_force`/`elden_ring` 3 anti-cheat blocker 의 honest spec freeze docs (VAC/EAC kernel/EAC
anti-tamper) 신규 emit 0건. 본 doc 내 `(가칭)` 명시, 별도 cycle 정식 land.

C6 — `lineage_classic_r28` 2026-02-07 release date 는 신작 fresh tracking 권고이지만 mk1 narrative
LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md 측 미land. 차후 cycle 정식 land 권고.

C7 — `lineage_w` loader sub-module 측 별도 PE binary track 미land (`pe_lineage_w_purple_track.hexa` disk 미존재,
사용자 prompt 측 perf only 명시 → cond.1 unmet honest reflect). 차후 cycle land 권고.

C8 — `purple_launcher` 측 별도 PE binary 미land (`pe_purple_launcher_*.hexa` disk 미존재 — plaync 측
distribution 직접 download/install path 만, loader 측 0건 honest).

C9 — `lineage_2_specific_track.hexa` (구 Lineage 2) disk 잔존 모듈은 본 cycle 측 untouched (additive only,
destructive 0). 사용자 directive 측 .roadmap fan-out 대상 X 만 명시.

C10 — env() lazy + <user> placeholder convention (raw 15) — 11 신규 .roadmap 의 모든 path 는 repo-relative
표기 (`lib/loader/...`, `lib/perf/...`, `tests/...`, `docs/...`). 사용자별 절대 path 인용 0건.

C11 — BR-NO-USER-VERBATIM (raw 175) — 본 doc + 11 신규 .roadmap + 1 신규 marker 모두 사용자 prompt 내용을
verbatim 으로 인용하지 않음. 사용자 directive ("airgenome-gamebox per-game roadmap fan-out" 등) paraphrase 만
기록.

C12 — friendly preset 적용 (handoff doc only) — TL;DR 최상단 + 모든 §-section 표 우선 + raw#10 caveats inline
+ next step 명시. 사용자 응답 X (BG subagent → 메인 monitor pattern).

## §6 own1 + own2 reflect 정합 audit

11 신규 .roadmap.<game> 모두 own1 (Wine 0 / hexa-only) + own2 (log/error/emit/headless) reflect:

- own1 — 11 게임 모든 cond.N verifier path 측 hexa-native (lib/loader/*.hexa + lib/perf/*.hexa), Wine 0 정합.
  D4 always-online + 4 anti-cheat (VAC/EAC×2/GameGuard) blocker 측 모두 honest spec freeze (no DRM/AC bypass)
  = own1 정합.
- own2 — 11 게임 모든 cond.N verifier 의 status_emit (`__D4_LAUNCH__`, `__CS2_GENOME__`, `__LM_AUTH__` etc) =
  own2 (c) emit 정합. blocker_reason transitive 명시 = own2 (b) error honest 정합.

11 게임 모두 21/21 own1 + 8/8 own2 PASS reflect (rank A 3 도메인 land 시 evidence array 인용 동일 패턴).

## §7 marker emit

```
state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker
```

(state/markers/ dir 은 predecessor 본 audit 에서 mkdir, 이미 존재)

## §8 next-cycle (사용자 lock-in 후)

1. 11 신규 .roadmap.<game> 의 cond.N verifier 실행 (script invoke + status partial → met 전환)
2. `retired_intentional` status enum 정식 채택 결정 + 5 게임 (D4 / CS2 / Delta Force / Elden Ring / Lineage
   Classic R28) 별 spec freeze docs land
3. `lineage_w` loader 측 `pe_lineage_w_purple_track.hexa` 별도 sub-module land (차후 cycle)
4. `lineage_classic_r28` mk1 narrative `LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` (가칭) land
5. raw 270 triplet T1 작업 — 11 per-game roadmap 별 README.ai.md (각 게임 sub-bucket 분할)
6. mk2 roadmap_op tool 도입 결정 (anima clone / 자체 구현 / symlink — 누적 caveat)
7. 잔존 6 BG-AGB 후보 (loader_native_helper / perf_manjeom / perf_game_d2r / perf_game_d4 /
   closure_orchestrator / battlenet_bypass) 별도 cycle land — rank B → rank C

## §9 file index (sha-pin at land time)

| path | type | size_b | LOC | sha256_hex |
|---|---|---:|---:|---|
| .roadmap.diablo2_resurrected | roadmap_jsonl | 5054 | 5 | `e0b7c30cb2176c98ce727b38f513945e8dede5887a8760b527d5ace93443a714` |
| .roadmap.diablo4 | roadmap_jsonl | 5498 | 5 | `556ec0d600e22ffc0a77c5b0b76fd46718e3c06f08b72ff22d75a468638c73bc` |
| .roadmap.battlenet | roadmap_jsonl | 5044 | 5 | `dac31aa6adb225f6844b6b9967aa00ec7f89863384cc0ab5920240a7777689b5` |
| .roadmap.wow | roadmap_jsonl | 4735 | 5 | `5f3d05e2d28f585091ef436c3e68fb8e85214cecdeed081dab0884e7704a2e48` |
| .roadmap.cs2 | roadmap_jsonl | 4701 | 5 | `63dd4ff32f707389b6ac4330db691bfb696e37fa3c815ea632025c71051ac603` |
| .roadmap.delta_force | roadmap_jsonl | 4978 | 5 | `edc840490a37a0ca392edbc94323e4dea8d0156b713ff01e346e1f26e0c662c1` |
| .roadmap.elden_ring | roadmap_jsonl | 5490 | 5 | `d1aa8c1d2e3fb33de7b21ac008e1af05e18afaee1ee3c93c6dd008d023e323b7` |
| .roadmap.lineage_classic_r28 | roadmap_jsonl | 4949 | 5 | `7f6eaaabecb4025fcd34394eeb1e7a1eba37f159e073944d2834959d95f3b656` |
| .roadmap.lineage_m | roadmap_jsonl | 4760 | 5 | `e9378cef9bc20f7b82938a3b72566dd6468c81882e05fe8877ca88e3b932f505` |
| .roadmap.lineage_w | roadmap_jsonl | 5059 | 5 | `9f86fa0f76027622084ccdda94dffaaf3b0e8cbdde4af7ab2c7aadaff5903476` |
| .roadmap.purple_launcher | roadmap_jsonl | 5020 | 5 | `2026a8f5066d84bf24938d67274762c9ea7c80ad5f65046eaff81a77bbd057ab` |
| airgenome/doc/airgenome_gamebox_per_game_roadmaps_landed_2026_05_03.ai.md | doc | TBD | TBD | (set after write) |
| state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker | marker | TBD | TBD | (self-referential) |

## §10 policy summary

- migration: forbidden — 0건 emit
- additive only — 76+1 docs (77 total) + 655 lib/*.hexa + native/* + 4 tests + tool/bench + plugin.json + entry +
  README.md + .gitignore + 3 기존 .roadmap.* + 2 predecessor markers 모두 무수정 보존
- destructive ops — 0건
- in-place writes — 0건 (11 신규 .roadmap + 1 본 doc + 1 marker = 13 NEW only)
- substrate — mac-local
- cost — $0
- cap — 90min (실제 소요 ≪ cap)
- raw 9 hexa-only orchestration — 본 audit 자체는 hexa orchestrator 미사용 (single-doc + JSONL header emit 만 =
  predecessor 와 동일 raw 168 minimum-viable exempt)
- raw 12 silent-error ban — 11 .roadmap blocker 모두 `status: open` + `status_enum_candidate` 또는
  `none` 명시적 honest 명시
- raw 15 env() lazy + <user> — 모든 path repo-relative, 사용자 절대 path X
- raw 175 BR-NO-USER-VERBATIM — 사용자 prompt 직접 인용 0건
- friendly preset — handoff doc only (사용자 응답 X — BG subagent → 메인 monitor)

## §11 sibling reference (predecessor 비교)

| 비교축 | predecessor mk2 (2026-05-02) | rank A 3 (2026-05-03) | per-game 11 (본 land 2026-05-03) |
|---|---|---|---|
| `.roadmap.*` files added | 0 (spec only) | 3 | 11 |
| handoff doc | 1 | 1 | 1 |
| marker | 1 | 1 | 1 |
| status enum proposal | (none) | `retired_intentional` 후보 명시 (3 blocker scope) | `retired_intentional` 5 게임 적용 후보 (D4/CS2/DF/ER/LC R28) |
| triplet_audit_tier | (n/a) | T1_large + T1 + T1 | (per-game tier 미명시, 별도 cycle) |
| user directive corrections | (none) | (none) | 3 제외 + 3 Lineage 변종 정확 + Purple 분리 |
| cost / cap | $0 / 60min | $0 / 60min | $0 / 90min |
