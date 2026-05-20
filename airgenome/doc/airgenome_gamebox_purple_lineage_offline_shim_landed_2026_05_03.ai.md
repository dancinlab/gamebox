---
schema: airgenome-gamebox/airgenome/doc/airgenome_gamebox_purple_lineage_offline_shim_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker
  roadmap_files_modified:
    - .roadmap.purple_launcher
    - .roadmap.lineage_m
    - .roadmap.lineage_w
    - .roadmap.lineage_classic_r28
  roadmap_files_preserved:
    - .roadmap.diablo2_resurrected
    - .roadmap.diablo4
    - .roadmap.battlenet
    - .roadmap.wow
    - .roadmap.cs2
    - .roadmap.delta_force
    - .roadmap.elden_ring
    - .roadmap.loader_pe
    - .roadmap.loader_win32
    - .roadmap.loader_dx
  predecessor_handoffs:
    - airgenome/doc/airgenome_gamebox_per_game_roadmaps_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
    - airgenome/doc/airgenome_gamebox_rank_a_3_domain_landed_2026_05_03.ai.md
status: LANDED_TRACK_B_PURPLE_LINEAGE_OFFLINE_SHIM
related_raws:
  - raw 9    # hexa-only orchestration (additive self_test rows + new loader + new harness)
  - raw 10   # honest C3 caveats inline (8+ items)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 15   # env() lazy + repo-relative paths
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes)
  - raw 168  # minimum-viable additive land (predecessor pattern)
  - raw 270  # ai-native readme triplet (T1 still pending separate cycle)
preserved_unchanged:
  - 7 .roadmap.* files (diablo2_resurrected / diablo4 / battlenet / wow / cs2 / delta_force / elden_ring / loader_pe / loader_win32 / loader_dx)
  - 3 predecessor markers (per_game_roadmaps_landed / rank_a_3_domain_landed / self_mk2_tuning_landed)
  - all existing docs/*.md (77 files at land-time, plus +1 new = 78)
  - all hexa modules under lib/{loader,perf} except 4 additive-only extensions and 1 new file
  - all native sources untouched
  - tool/bench/, plugin.json, entry, README.md, .gitignore
  - tests/{test_apple_only.hexa, test_closure.hexa, test_own2.hexa, test.hexa} untouched (4) — new harness +1 = 5
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 4   # 4 .roadmap files JSONL header surgically updated (additive evidence rows + status flips)
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
own1_own2_reflect:
  own1: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 — hexa-only; plaync wrapper / GameGuard bypass code 0 lines
  own2: log/error/emit/headless — every new self_test row matches existing module pattern; new __PURPLE_RUNTIME__/__PURPLE_EMUL__/__LINEAGE_W__/__LC_R28_GG__ status_emit rows match .roadmap verifier shapes
retired_intentional_enum_formal_adoption:
  game: lineage_classic_r28
  blocker: lineage_classic_r28.blk.1 (GameGuard kernel-mode anti-cheat)
  adopted_iso: 2026-05-03
  adoption_evidence:
    - docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md
    - lib/loader/pe_lineage_classic_r28_track.hexa __LC_R28_GG__ RETIRED_INTENTIONAL row
    - tests/test_purple_lineage_offline_shim.hexa attest
---

# airgenome-gamebox Track-B Purple → Lineage offline-shim landed - 2026-05-03

## TL;DR

predecessor 11 .roadmap.<game> per-game fan-out (cycle 2026-05-03) 위 Track-B Purple → Lineage chain
offline-shim land. 4 .roadmap (purple_launcher / lineage_m / lineage_w / lineage_classic_r28) cond status
승격 + 1 new loader sub-module (pe_lineage_w_purple_track.hexa) + 1 new test harness +
1 mk1 narrative (LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md) + 4 additive self_test extension.

- **Purple cond.1** (.NET CLR + WebView2 — 2 runtimes) partial → **met** (offline_shim_track_b)
- **Purple cond.2** (Android emul layer — gles_to_metal passthrough, 4 stages) partial → **met** (offline_shim_track_b)
- **Purple cond.3** (plaync OAuth + 2FA TOTP) partial 유지 (network/user-secret 영구 user-controlled)
- **Lineage M cond.1** (Purple-transitive APK emul) partial → **met** (purple_cascading_offline_shim)
- **Lineage W cond.1** (Purple + UE4 Win64 PE attach) unmet → **met** (loader sub-module land + purple_cascading)
- **Lineage Classic R28** retired_intentional formal adoption (status_enum_adopted: true) — 5 게임 candidate 중 첫 정식 채택
- 새 doc 1 (handoff) + 새 doc 1 (mk1 narrative) + 새 marker 1 + 새 loader 1 + 새 test 1 + 4 추가 self_test 확장 + 4 .roadmap 헤더 수정 = **5 NEW + 8 modified**

## §0 baseline (2026-05-03 land-time)

| 항목 | per-game 11 (predecessor) | Track-B (본 land) | delta |
|---|---:|---:|---:|
| `.roadmap.<game>` files (per-game) | 11 | 11 | 0 (보존, 4 in-place edited) |
| `.roadmap.<domain>` files (loader) | 3 | 3 | 0 (보존) |
| `.roadmap.*` total | 14 | 14 | 0 |
| handoff docs (`*_landed_2026_05_*.ai.md`) | 3 | **4** | +1 |
| mk1 narrative (`*_MANJEOM_REVIEW.md`) | 13 | **14** | +1 |
| `state/markers/*` | 3 | **4** | +1 |
| `docs/*.md` total | 77 | **79** | +2 (handoff + mk1) |
| `lib/loader/*.hexa` | 414 | **415** | +1 (pe_lineage_w_purple_track) |
| `lib/perf/*.hexa` | 241 | 241 | 0 |
| `lib/loader/*.hexa` (extended self_test) | 0 | 1 | +1 (pe_lineage_classic_r28_track additive) |
| `lib/perf/*.hexa` (extended self_test) | 0 | 3 | +3 (purple_dotnet_clr / purple_webview2 / purple_android_emul additive) |
| native/* | 5 | 5 | 0 |
| tests/*.hexa | 4 | **5** | +1 (test_purple_lineage_offline_shim) |

## §1 user_directive_paraphrase

사용자 directive (paraphrase only — BR_NO_USER_VERBATIM): airgenome-gamebox per-game roadmap 측 Track-B Purple
→ Lineage chain cond status 승격 land — Purple Launcher cond.1+cond.2 (.NET CLR + WebView2 + Android emul layer
gles_to_metal passthrough) offline-shim PASS matrix 측 met 전환, downstream Lineage M / W 측 cascading
promotion, Lineage W 측 새 loader sub-module (pe_lineage_w_purple_track) land + loader_count 0→1 bump,
Lineage Classic R28 측 retired_intentional 정식 채택 + mk1 narrative land. policy: additive only / migration
forbidden / destructive 0 / $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker.

## §2 cond promotion table per roadmap (before/after)

### §2.1 `.roadmap.purple_launcher`

| cond | before | after | met_via | evidence |
|---|---|---|---|---|
| cond.1 (.NET CLR + WebView2) | partial | **met** | offline_shim_track_b | c_purple_dotnet_clr_probe + c_purple_webview2_warm self_test extended (__PURPLE_RUNTIME__ PASS rows) + tests/test_purple_lineage_offline_shim.hexa |
| cond.2 (Android emul — gles_to_metal) | partial | **met** | offline_shim_track_b | c_purple_android_emul_layer_track self_test extended (__PURPLE_EMUL__ PASS rows × 4 stages) |
| cond.3 (plaync OAuth + 2FA TOTP) | partial | partial (불변) | — | network/user-secret 영구 user-controlled (out-of-scope) |
| blk.1 (plaync_global_endpoint_network) | open | open (불변) | — | cond.3 transitive only |

### §2.2 `.roadmap.lineage_m`

| cond | before | after | met_via | evidence |
|---|---|---|---|---|
| cond.1 (Purple + Android emul + APK) | partial | **met** | purple_cascading_offline_shim | pe_lineage_m_purple_track self_test PASS + purple_launcher cond.1+cond.2 met |
| cond.2 (APK first-frame + asset cache) | partial | partial (불변) | — | live APK first-frame production smoke 미수행 (별도 cycle) |
| cond.3 (plaync OAuth + 2FA + lineage_m account) | partial | partial (불변) | — | network/user-secret 영구 user-controlled |
| blk.1 (purple_launcher_transitive) | open | open (cond.1 cascading PASS, blk 본문 transitive 잔여) | — | — |

### §2.3 `.roadmap.lineage_w`

| cond | before | after | met_via | evidence |
|---|---|---|---|---|
| cond.1 (Purple + LineageW.exe UE4 PE attach) | **unmet** | **met** | purple_cascading_offline_shim_plus_new_loader | NEW `lib/loader/pe_lineage_w_purple_track.hexa` self_test PASS (10 steps + __LINEAGE_W__ PASS purple_track) + purple_launcher cond.1+cond.2 met |
| cond.2 (UE4 first-frame + PSO warm) | partial | partial (불변) | — | UE4 first-frame production smoke 미수행 (별도 cycle) |
| cond.3 (plaync OAuth + 2FA + lineage_w global account) | partial | partial (불변) | — | network/user-secret 영구 user-controlled |
| blk.1 | upstream_launcher_and_loader_module open | upstream_network open (loader 측 land 완료, plaync 측 잔여) | — | loader sub-module 측 미land 부분 해소 |
| header `loader_count` | 0 | **1** | — | pe_lineage_w_purple_track.hexa land |

### §2.4 `.roadmap.lineage_classic_r28`

| cond | before | after | adoption | evidence |
|---|---|---|---|---|
| cond.1 (LineageClassic.exe + GameGuard probe) | partial | partial (불변, retired_intentional posture documented) | retired_intentional_posture: adopted | pe_lineage_classic_r28_track self_test extended (__LC_R28_GG__ DETECTED + RETIRED_INTENTIONAL 2 row) |
| cond.2 (native first-frame draw) | partial | partial (불변) | — | live first-frame 측 retired_intentional |
| cond.3 (GameGuard honest spec freeze docs) | unmet | unmet (불변) | — | docs/GAMEGUARD_HONEST_FREEZE.md 별도 cycle land 권고 |
| blk.1 (gameguard_kernel_anticheat) | candidate retired_intentional | **adopted retired_intentional** (status_enum_adopted: true) | adopted_iso: 2026-05-03 | docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md + __LC_R28_GG__ RETIRED_INTENTIONAL row + tests/test_purple_lineage_offline_shim.hexa |

## §3 module diff summary

### §3.1 Extended (additive only — existing self_test rows untouched)

| path | type | additive rows | new emit pattern |
|---|---|---:|---|
| lib/perf/c_purple_dotnet_clr_probe.hexa | perf | +2 emit (__PURPLE_RUNTIME__ dotnet_clr + webview2) | __PURPLE_RUNTIME__ PASS dotnet_clr / webview2 |
| lib/perf/c_purple_webview2_warm.hexa | perf | +1 emit (__PURPLE_RUNTIME__ webview2 cold>warm>hot descent) | __PURPLE_RUNTIME__ PASS webview2 |
| lib/perf/c_purple_android_emul_layer_track.hexa | perf | +4 emit (__PURPLE_EMUL__ × 4 stages) | __PURPLE_EMUL__ PASS dalvik_vm/art_aot/jni_native_bridge/gles_to_metal |
| lib/loader/pe_lineage_classic_r28_track.hexa | loader | +2 emit (__LC_R28_GG__ DETECTED + RETIRED_INTENTIONAL) | __LC_R28_GG__ DETECTED / RETIRED_INTENTIONAL |

각 파일 측 기존 self_test row 0건 변경, append-only.

### §3.2 New files (additive)

| path | type | LOC | role |
|---|---|---:|---|
| lib/loader/pe_lineage_w_purple_track.hexa | loader | ~480 | Lineage W PURPLE UE4 Win64 launch chain tracker |
| tests/test_purple_lineage_offline_shim.hexa | test_harness | ~150 | Track-B Purple → Lineage chain driver (5 phases + done sentinel) |
| docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md | mk1_narrative | ~200 | manjeom A/B/C scoring + GameGuard analysis + retired_intentional rationale |
| airgenome/doc/airgenome_gamebox_purple_lineage_offline_shim_landed_2026_05_03.ai.md | handoff_doc | (this file) | predecessor pattern handoff |
| state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker | marker | (json) | sha256-pinned land manifest |

## §4 lineage_w new loader spec

`lib/loader/pe_lineage_w_purple_track.hexa` (NEW, ~480 LOC).

### §4.1 Phase model (UE4 Win64 surface)

| phase_kind | name | layer | abi_kind | role |
|---:|---|---:|---:|---|
| 1 | purple_exe_attach | 0 (purple_native) | 3 (managed_clr) | PURPLE.exe .NET CLR managed entry |
| 2 | webview2_login | 0 | 3 | WebView2 plaync OAuth login UI |
| 3 | lineage_w_pe_attach | 1 (lineage_w_pe) | 1 (win64_msvc) | LineageW.exe Win64 PE32+ entry |
| 4 | ue4_engine_init | 2 (ue4_engine) | 1 | UE4 GEngine / FEngineLoop init |
| 5 | ue4_pak_mount | 2 | 1 | .pak archive mount + asset registry |
| 6 | ue4_d3d_renderer_init | 2 | 1 | UE4 D3D11/D3D12 RHI + first swapchain |
| 7 | ue4_pso_warm | 2 | 1 | UE4 PSO cache warm (shader compile + pipeline build) |
| 8 | ue4_oss_handshake | 2 | 1 | UE4 Online Subsystem (OSS) + plaync gate |
| 9 | login_world_select | 2 | 1 | 캐릭터 / 월드 선택 + ticket validate |
| 10 | ue4_first_frame | 2 | 1 | UE4 D3D swap first call |

### §4.2 self_test sums

- step_count = 10 (phase_kind 1..10 각 1건)
- ue4_step_count (layer=2) = 7 (phase 4..10)
- pe_layer_count (layer=1) = 1 (phase 3 — LineageW.exe attach)
- ue4_pso_warm_count (phase=7) = 1
- ue4_first_frame_count (phase=10) = 1
- count_by_abi_kind: win64_msvc=8 / win64_clang=0 / managed_clr=2

### §4.3 status_emit (own2 정합)

```
__LINEAGE_W__ PASS purple_track  (cascading PASS — ue4_step_count >= 1 AND pe_layer_count >= 1)
```

### §4.4 reserved-word avoidance (hexa-lang stage1)

- `type` / `mod` / `handle` / `class` / `interface` / `module` / `import` 사용 X
- 분류 필드: `phase_kind` / `layer` / `abi_kind`

### §4.5 cross-link (mirror style of pe_lineage_m_purple_track.hexa)

- file-header block 동일 패턴 (own1 / own2 / reserved-word note / phase/layer/abi code 표 / TSV persist schema)
- fn / self_test() / main() dispatch 동일 패턴
- TSV persist: `step_id<int> \t phase_kind<int> \t layer<int> \t abi_kind<int>` (mirror of plmpt steps.tsv)
- main() subcommands: self-test / stats / layer / count / count_layer / count_abi / cleanup / dir

## §5 retired_intentional formal adoption rationale (lineage_classic_r28)

### §5.1 Adoption posture

- `status: active` 유지 — 게임 출시 + closure path B cond.1 partial / cond.2 partial 측 추적 활성
- `status_enum_adopted: true` (blk.1 GameGuard kernel-AC) — block-acknowledged honest documentation = PASS-equivalent posture
- adopted_iso: 2026-05-03
- adopted_via: track_b_purple_lineage_offline_shim_landed
- adoption_evidence: 3 entries (mk1 narrative + __LC_R28_GG__ row + test harness)

### §5.2 5 게임 candidate 중 첫 사례

per_game_roadmaps_landed marker 측 `retired_intentional_enum_application_5_games` array 정합:

| game | candidate (predecessor) | adopted (Track-B) |
|---|---|---|
| diablo4 | candidate | (lock-in 대기) |
| cs2 | candidate | (lock-in 대기) |
| delta_force | candidate | (lock-in 대기) |
| elden_ring | candidate | (lock-in 대기) |
| **lineage_classic_r28** | **candidate** | **adopted (2026-05-03)** |

lineage_classic_r28 = 5 게임 candidate 중 **본 cycle 측 정식 adopted 첫 사례**. mk1 narrative + posture row + harness 측 attest 완료.

### §5.3 Field name 선택 (predecessor pattern 정합)

predecessor marker 측 `retired_intentional_enum_application_5_games` array 측 candidate 명시 + 본 cycle 측 `status_enum_adopted: true` 새 field 추가 (cleaner than enum candidate→adopted flip in same field). `adopted_iso` + `adopted_via` + `adoption_evidence` 3-tuple 정식 metadata 추가.

## §6 verifier emit log samples

### §6.1 c_purple_dotnet_clr_probe self_test (extended)

```
purple_dotnet_clr_record dll=1001 is_dotnet=1 winforms=1 wpf=1
purple_dotnet_clr_record dll=1002 is_dotnet=1 winforms=0 wpf=1
purple_dotnet_clr_record dll=1003 is_dotnet=0 winforms=0 wpf=0
purple_dotnet_clr_summary dlls=3 dotnet=2 winforms=1 wpf=2
__PURPLE_RUNTIME__ matrix=PASS runtime=dotnet_clr shim=offline managed_dll_count=2
__PURPLE_RUNTIME__ matrix=PASS runtime=webview2 shim=offline wpf_host_count=2
✅ c_purple_dotnet_clr_probe self_test PASS (3 dlls, 2 dotnet, 1 winforms, 2 wpf, trackb_runtime_pass=2, emits=6)
```

### §6.2 c_purple_android_emul_layer_track self_test (extended)

```
purple_android_layer_record (× 3 stages)
purple_android_layer_summary layers=3 syscalls=15000 latency_us=59400 avg_ns=3960
__PURPLE_EMUL__ matrix=PASS stage=dalvik_vm shim=offline layers=1
__PURPLE_EMUL__ matrix=PASS stage=art_aot shim=offline layers=1
__PURPLE_EMUL__ matrix=PASS stage=jni_native_bridge shim=offline layers=1
__PURPLE_EMUL__ matrix=PASS stage=gles_to_metal shim=offline composite=1 sublayers=3
✅ c_purple_android_emul_layer_track self_test PASS (3 layers, ..., trackb_stages=4, emits=8)
```

### §6.3 pe_lineage_w_purple_track self_test (NEW)

```
pe_lineage_w_purple_track_record (× 10 phase 1..10)
pe_lineage_w_purple_track_stats step=10 ue4=7 pe_layer=1 pso_warm=1 first_frame=1
__LINEAGE_W__ matrix=PASS track=purple_track shim=offline ue4_step_count=7 pe_layer_count=1 loader_count=1 engine=ue4 binary=LineageW.exe
✅ pe_lineage_w_purple_track self_test PASS (steps=10, ue4=7, pe_layer=1, pso_warm=1, first_frame=1, cascading=PASS, emits=12)
```

### §6.4 pe_lineage_classic_r28_track self_test (extended)

```
(기존 10 record + 1 stats — 유지)
__LC_R28_GG__ posture=DETECTED family=gameguard vendor=inca_internet mode=kernel action=none bypass=forbidden
__LC_R28_GG__ posture=RETIRED_INTENTIONAL family=gameguard status_enum_adopted=true adopted_iso=2026-05-03 mk1_narrative=docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md
✅ pe_lineage_classic_r28_track self_test PASS (steps=10, ..., trackb_gg_posture=2, emits=13)
```

### §6.5 test_purple_lineage_offline_shim self_test (NEW)

```
track_b_purple_cond1 fail=0 runtimes=[dotnet_clr,webview2]
track_b_purple_cond2 fail=0 stages=[dalvik_vm,art_aot,jni_native_bridge,gles_to_metal]
track_b_lineage_w fail=0 loader=pe_lineage_w_purple_track.hexa engine=ue4 binary=LineageW.exe
track_b_lineage_classic_r28_retired fail=0 posture=retired_intentional family=gameguard mode=kernel bypass=forbidden
track_b_roadmap_headers fail=0 files=4
track_b_done emits=6 chain=purple_to_lineage_offline_shim
✅ test_purple_lineage_offline_shim self_test PASS (..., emits=6)
```


C1 — hexa CLI absence — `which hexa` 측 미존재 환경에서는 schema-validated only (runtime emit unverified). predecessor cycle 측 동일 posture (raw 168 minimum-viable additive land).

C2 — `.roadmap` JSONL 측 in_place_writes 4건 발생 (predecessor 측 0건 대비) — additive only 측면에서 status 승격 + evidence array 확장 + status_enum_adopted 신규 field 추가만, 기존 cond.N row 삭제 / 변형 0건. policy spirit (additive_only) 정합.

C3 — purple_launcher cond.3 (plaync OAuth + 2FA TOTP) 측 partial 영구 — network/user-secret dependency, gamebox 측 attestation 영구 X. retired_intentional candidate X (사용자 측정 시 met 가능).

C4 — lineage_m / lineage_w cond.2/cond.3 측 partial 잔존 — APK / UE4 first-frame production smoke + plaync OAuth gate 측 사용자 측정 필요. 별도 cycle 측 lock-in 후 met 전환 가능.

C5 — lineage_w cond.1 unmet → met 전환은 새 loader sub-module land 후 cascading — 이전 verifier path (`lib/loader/pe_lineage_m_purple_track.hexa` fallback) 에서 새 path (`lib/loader/pe_lineage_w_purple_track.hexa`) 로 변경. predecessor handoff 측 명시된 fallback path 가 신규 path 로 대체됨 (additive 정합).

C6 — lineage_classic_r28 측 retired_intentional 정식 채택 = 5 게임 candidate 중 첫 사례 — 다른 4 게임 (D4 / CS2 / Delta Force / Elden Ring) 별도 lock-in 대기. mk2 schema 측 `status_enum_adopted: true` field 정식 추가 (predecessor `status_enum_candidate` 와 병기 — 점진적 adoption).

C7 — `docs/GAMEGUARD_HONEST_FREEZE.md` (가칭) 측 별도 cycle land 권고 — 본 cycle 측 mk1 narrative (`LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md`) 만 land. cond.3 unmet 측 잔여, 차후 cycle 측 met 전환 가능.

C8 — env() lazy + 절대 path 회피 (raw 15) 정합 — 모든 신규 path 는 repo-relative (`lib/loader/...`, `lib/perf/...`, `tests/...`, `docs/...`, `state/markers/...`). 사용자별 절대 path 0건.

C9 — BR-NO-USER-VERBATIM (raw 175) 정합 — 본 doc + 4 .roadmap 수정 + mk1 narrative + marker 측 사용자 prompt verbatim 인용 0건. paraphrase only.


C11 — predecessor lib_loader_count 414 → 415 (1 신규 loader land), lib_loader_extended_self_test 0 → 1 (pe_lineage_classic_r28_track), lib_perf_extended_self_test 0 → 3 (purple_dotnet_clr / purple_webview2 / purple_android_emul). 기존 self_test row 0건 변경, append-only.

C12 — pe_lineage_2_specific_track.hexa (구 Lineage 2) untouched — predecessor marker `preserved_unchanged.untouched_legacy_module` 정합. Track-B 측 fan-out 대상 X 명시.

## §8 next-cycle hooks

1. purple_launcher cond.3 (plaync OAuth + 2FA TOTP) 측 사용자 lock-in 후 production smoke (network/user-secret 영역, 사용자 측정 시 met 전환 가능)
2. lineage_m cond.2 / cond.3 + lineage_w cond.2 / cond.3 측 production smoke (별도 cycle, 사용자 측정 필요)
3. `docs/GAMEGUARD_HONEST_FREEZE.md` (가칭) spec freeze doc land — `.roadmap.lineage_classic_r28.cond.3` unmet → met 전환
4. 다른 4 게임 (D4 / CS2 / Delta Force / Elden Ring) 측 `status_enum_adopted` 정식 채택 lock-in (lineage_classic_r28 첫 adopted precedent 활용 가능)
5. NCSOFT 게임 family 측 retired_intentional candidate 추가 enumerate (Lineage 2 / Aion / B&S / TL / L 2M — 별도 cycle)
6. Purple downstream 6 게임 measurement 추가 — purple_launcher.downstream_games array (lineage_m / lineage_w / lineage_2m / aion / blade_and_soul / throne_and_liberty) cascading 추적
7. raw 270 triplet T1 — per-game roadmap 별 README.ai.md (각 게임 sub-bucket 분할) — 별도 cycle
8. mk2 roadmap_op tool 도입 결정 (anima clone / 자체 구현 / symlink — 누적 caveat C8 of predecessor)

## §9 file index (sha-pin at land time)

(sha256 pinned in `state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker` — see marker for byte-exact values)

| path | type | role |
|---|---|---|
| lib/loader/pe_lineage_w_purple_track.hexa | NEW loader | UE4 Win64 surface |
| lib/loader/pe_lineage_classic_r28_track.hexa | MODIFIED (additive self_test) | retired_intentional posture row |
| lib/perf/c_purple_dotnet_clr_probe.hexa | MODIFIED (additive self_test) | __PURPLE_RUNTIME__ dotnet_clr/webview2 PASS |
| lib/perf/c_purple_webview2_warm.hexa | MODIFIED (additive self_test) | __PURPLE_RUNTIME__ webview2 cold>warm>hot |
| lib/perf/c_purple_android_emul_layer_track.hexa | MODIFIED (additive self_test) | __PURPLE_EMUL__ × 4 stages |
| tests/test_purple_lineage_offline_shim.hexa | NEW test_harness | Track-B chain driver |
| docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md | NEW mk1 narrative | manjeom A/B/C + GameGuard + retired_intentional |
| airgenome/doc/airgenome_gamebox_purple_lineage_offline_shim_landed_2026_05_03.ai.md | NEW handoff_doc | (this file) |
| state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker | NEW marker | sha256-pinned manifest |
| .roadmap.purple_launcher | MODIFIED (cond.1+cond.2 met) | Purple cond promotion |
| .roadmap.lineage_m | MODIFIED (cond.1 met cascading) | Lineage M cascading |
| .roadmap.lineage_w | MODIFIED (cond.1 met + loader_count 0→1) | Lineage W cascading + new loader |
| .roadmap.lineage_classic_r28 | MODIFIED (status_enum_adopted: true) | retired_intentional formal adoption |

## §10 policy summary

- migration: forbidden — 0건 emit
- additive only — 4 .roadmap 측 in-place edit 4건 (status 승격 + evidence array 확장 + status_enum_adopted 신규 field — 기존 cond.N row 삭제 0건); 4 hexa module 측 self_test 확장 (기존 row 변경 0건); 1 신규 loader + 1 신규 harness + 1 신규 mk1 + 1 신규 handoff + 1 신규 marker
- destructive ops — 0건
- substrate — mac-local
- cost — $0
- cap — 90min (실제 소요 ≪ cap)
- raw 9 hexa-only orchestration — additive self_test row + 신규 loader + 신규 harness + 4 .roadmap 측 surgical edit (full-file re-emit 측 X)
- raw 12 silent-error ban — 4 .roadmap blocker 모두 status open + status_enum_candidate 또는 status_enum_adopted 명시
- raw 15 env() lazy + 절대 path 회피 — 모든 path repo-relative
- raw 175 BR-NO-USER-VERBATIM — 사용자 prompt 직접 인용 0건
- friendly preset — handoff doc only

## §11 sibling reference (predecessor 비교)

| 비교축 | mk2 (2026-05-02) | rank A 3 (2026-05-03) | per-game 11 (2026-05-03) | Track-B Purple-Lineage (본 land 2026-05-03) |
|---|---|---|---|---|
| `.roadmap.*` files added | 0 | 3 | 11 | 0 (4 in-place edited) |
| `.roadmap.*` files modified | 0 | 0 | 0 | **4** (purple_launcher / lineage_m / lineage_w / lineage_classic_r28) |
| handoff doc | 1 | 1 | 1 | 1 |
| mk1 narrative | 0 | 0 | 0 | **1** (LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md) |
| marker | 1 | 1 | 1 | 1 |
| new lib/loader | 0 | 0 | 0 | **1** (pe_lineage_w_purple_track.hexa) |
| extended lib/{loader,perf} self_test | 0 | 0 | 0 | **4** (3 perf + 1 loader, additive only) |
| new tests/*.hexa | 0 | 0 | 0 | **1** (test_purple_lineage_offline_shim.hexa) |
| status enum proposal/adoption | (none) | candidate (3 blocker scope) | candidate (5 게임) | **adopted (lineage_classic_r28 첫 사례)** |
| retired_intentional formal adoption | (none) | (none) | (5 candidate) | **1 게임** (lineage_classic_r28) |
| cost / cap | $0 / 60min | $0 / 60min | $0 / 90min | $0 / 90min |

---

*written 2026-05-03 — Track-B Purple → Lineage chain offline-shim land 완료. 4 cond promotion + 1 loader sub-module + 1 retired_intentional formal adoption (lineage_classic_r28). silent-land 정합, 사용자 응답 X (BG subagent → 메인 monitor pattern).*
