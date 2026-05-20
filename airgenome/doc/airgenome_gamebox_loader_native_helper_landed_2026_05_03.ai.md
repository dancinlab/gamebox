# airgenome-gamebox — Track-R `loader_native_helper` Bridge Layer Landed (2026-05-03)

> **TL;DR**: `lib/loader/native_helper_bridge.hexa` (Phase 1 stage1 native helper bridge skeleton, baseline 106 LOC) 측 Apple-native bridge layer 측 5 surface (Metal CommandQueue / MTLDevice probe / CGDirectDisplay query / IOKit service match / Foundation NSObject lifecycle) 측 PARTIAL emit additive 추가 (+184 LOC, 106→290 LOC, 3609→12370 bytes). emit format `__NATIVE_BRIDGE__ PARTIAL <surface>` (own2 정직 emit). `docs/LOADER_NATIVE_HELPER_BRIDGE.md` mk1 narrative NEW (292 LOC, 7 sections + §6 출처, 8 honest C3 caveats). own1 정합 verdict = **PASS** (Apple SDK direct binding via hexa-native FFI scaffold only — Wine / CrossOver / Whisky / GPTK / Mono / CoreCLR / Vulkan / MoltenVK / 3rd-party D3D-to-Metal binding 0줄). `native/` directory 측 modification 0건 (read-only inventory only). 6 BG-AGB rank B+C candidate 중 `loader_native_helper` 측 본 cycle 측 land — Track-P precedent 정합. additive_only / destructive_ops 0 / migration X / cost $0 / cap 90min / silent-land marker.

## §0 baseline

- predecessor handoff (immediate): `airgenome/doc/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed_2026_05_03.ai.md` (Track-P own1 disambiguation, 6 BG-AGB rank B+C unblock 명시)
- predecessor marker: `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker`
- predecessor mk1 narrative: `docs/STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md` (cycle 63 14/25 만점 검토 — Track R 측 narrative ancestor)
- per-game roadmaps source marker: `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` (next_cycle_actions 측 `loader_native_helper` 측 본 cycle 자체)
- 본 cycle 측 `.roadmap.*` touch X (Track-R 측 .roadmap 측 modification 0건)
- 본 cycle 측 `native/*` touch X (read-only inventory only)

## §1 user_directive_paraphrase

BG subagent — Track-R `loader_native_helper` rank B+C cycle land for airgenome-gamebox. `lib/loader/native_helper_bridge.hexa` (existing 106 LOC stage1 native helper bridge skeleton) 측 EXTEND additively — 5 Apple-native surface (Metal CommandQueue 생성 / MTLDevice probe / CGDirectDisplay query / IOKit framework probe / Foundation NSObject lifecycle) 측 PARTIAL emit 추가. emit format `__NATIVE_BRIDGE__ PARTIAL <surface>`. `docs/LOADER_NATIVE_HELPER_BRIDGE.md` (NEW, 200-350 LOC mk1 narrative spec freeze, similar to GAMEGUARD/WARDEN/D2R_REALM_PROTOCOL freeze 측). sections: §0 목적/한계, §1 native/ directory inventory + 5 Apple-native APIs, §2 own1 정합 (Apple platform component = OK, Wine/CrossOver wrapper 0), §3 future-cycle non-goals (영구), §4 cross-link to loader_dx + loader_pe + loader_win32, §5 caveats ≥8. handoff doc + marker NEW. policy: additive_only / migration forbidden / destructive_ops 0 / $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker.

## §2 module bridge layer extension summary

### §2.1 LOC delta on `lib/loader/native_helper_bridge.hexa`

- before (Track-R 전 baseline): 106 LOC, 3609 bytes
- after (Track-R 후): 290 LOC, 12370 bytes
- delta: +184 LOC, +8761 bytes
- diff kind: pure insertion (additive only — 기존 fn / self_test / main / r0_emit content modification 0건)

### §2.2 5 Apple-native surface 추가 (additive)

| # | surface | spec 함수 (hexa) | helper.mm 측 IPC kind 측 future round-trip | own1 정합 |
|---|---|---|---|---|
| 1 | `metal_command_queue` | `metal_command_queue_spec()` | kind=7 metal_device 후속 (`[device newCommandQueue]` + `[queue commandBuffer]`) | ✓ Apple Metal.framework |
| 2 | `mtl_device_probe` | `mtl_device_probe_spec()` | kind=7 metal_device (`MTLCreateSystemDefaultDevice`) | ✓ Apple Metal.framework |
| 3 | `cg_direct_display` | `cg_direct_display_spec()` | (helper 측 CoreGraphics direct call — `CGGetActiveDisplayList`) | ✓ Apple CoreGraphics.framework |
| 4 | `iokit_service_match` | `iokit_service_match_spec()` | (helper 측 IOKit direct call — `IOServiceMatching` + `IOServiceGetMatchingServices`) | ✓ Apple IOKit.framework |
| 5 | `ns_object_lifecycle` | `ns_object_lifecycle_spec()` | kind=6 nsobject (`[[NSObject alloc] init]` / `release` / `@autoreleasepool`) | ✓ Apple Foundation.framework |

### §2.3 추가 fn list (additive only)

- `apple_surfaces() -> array` (5 surface 명세 list)
- `surface_partial_emit(surface) -> str` (emit format builder)
- `metal_command_queue_spec() -> str` (surface 1 spec)
- `mtl_device_probe_spec() -> str` (surface 2 spec)
- `cg_direct_display_spec() -> str` (surface 3 spec)
- `iokit_service_match_spec() -> str` (surface 4 spec)
- `ns_object_lifecycle_spec() -> str` (surface 5 spec)
- `surface_dispatch(surface) -> str` (surface name → spec dispatch)
- `surface_count() -> int` (5 invariant)
- `surface_own1_alignment() -> str` (own1 정합 invariant `apple_sdk_direct_binding_only`)

### §2.4 self_test 측 추가 invariant rows (additive only)

기존 8 invariant 측 deletion / modification 0건. 5 surface 측 invariant + 추가 1 unknown surface dispatch invariant + 1 own1 alignment invariant = 7 추가 invariant rows. 5 surface 측 PARTIAL emit (`println(s1_emit)` ~ `println(s5_emit)` — 5건). track_r 측 r0_emit 1건 추가 (`native_bridge_track_r_partial`).

### §2.5 main dispatch 측 추가 sub (additive only)

기존 4 sub (self-test / ping / available / ops) 측 deletion / modification 0건. 4 sub 추가:

- `surfaces` — 5 surface 명세 list 출력
- `surface-spec <name>` — surface 측 spec 함수 dispatch
- `surface-emit <name>` — surface 측 PARTIAL emit format builder
- `own1` — own1 정합 alignment value (`apple_sdk_direct_binding_only`) 출력

### §2.6 emit count

- `__NATIVE_BRIDGE__ PARTIAL <surface>` emit format pattern: 8 occurrences (literal in module: 1 builder + 5 invariant string check + 2 컴멘트 reference)
- self_test 측 actual `println` 측 emit 발동: 5건 (s1~s5 surface 별 1건)
- r0_emit 추가: 1건 (`native_bridge_track_r_partial`)

### §2.7 sha256 pin

- before: `edd5f15ca04577b8449de9921b243acf03ca51ec571bc7eabaad5ecdf2dd25cc`
- after: `52050420d719b7c7d390a82b87621206cf2db59d4d8cdbfc538e6227167a10e7`

## §3 freeze doc summary

### §3.1 path + LOC

- path: `docs/LOADER_NATIVE_HELPER_BRIDGE.md`
- LOC: 292 (200-350 range 정합)
- bytes: 22324
- sha256: `b0881a4296f7c0dcd9405edddea1bfd4147880b9ad7be850701914409cdd218b`

### §3.2 sections

| # | section | 역할 |
|---|---|---|
| §0 | 본 doc 의 목적과 한계 | 가장 먼저 명시 — in-scope / out-of-scope 영구 / minimal additive 정합 / native/ directory read-only posture |
| §1 | native/ directory inventory + 5 Apple-native APIs | 5 native source file inventory (helper.mm + build.sh + d3dmetal_probe.sh + helper.entitlements + pe_to_macho_shim.cpp) + 2 build artifact + 5 surface 명세 |
| §2 | own1 정합 (Apple platform component = OK, Wine/CrossOver wrapper 0) | own1 정합 verdict PASS + 8 evidence + NOT-IN-SCOPE 영구 |
| §3 | Future-cycle non-goals (영구 out-of-scope) | Wine source / CrossOver patch / DXVK / VKD3D-Proton / MoltenVK / Mono / CoreCLR / Vulkan SDK / 3rd-party D3D-to-Metal binding 영구 out-of-scope |
| §4 | Cross-link to loader_dx + loader_pe + loader_win32 | 4 cross-link + 5 sister freeze cluster + 6 BG-AGB rank B+C unblock confirmation |
| §6 | 출처 | Apple official references + sister mk1 narrative |

### §3.3 8 honest C3 caveats summary

- C1 — bridge layer = surface declaration only, 실 syscall 발동 X (helper.mm 책임)
- C2 — `native/helper.mm` 측 modification 0건 (read-only inventory only)
- C3 — 5 surface 측 first-frame round-trip 미land (PARTIAL emit only)
- C4 — D3DMetal binary licence transitive blocker (loader_dx.blk.1)
- C5 — hexa CLI runtime emit 미수행 (schema validate only)
- C6 — additive_only spirit lock-in (pure insertion, no `-` line in diff body)
- C7 — own1 = "Apple platform component = OK" 정합 boundary 측 사용자 lock-in 시점 ack 권고
- C8 — Track R 측 future-cycle non-goals 영구 lock-in (additive only — list 측 항목 측 deletion X)

## §4 own1 정합 verdict

### §4.1 verdict — PASS

본 Track R cycle 측 추가 5 Apple-native surface 모두 **Apple SDK direct binding via hexa-native FFI scaffold** — own1 정합 PASS.

### §4.2 evidence summary

- Apple Metal.framework direct binding (surface 1 + 2)
- Apple Foundation.framework direct binding (surface 5)
- Apple CoreGraphics.framework direct binding (surface 3)
- Apple IOKit.framework direct binding (surface 4)
- Wine / CrossOver / Whisky / GPTK occurrence audit: 0건 (`grep -ic 'wine\|crossover\|gptk\|whisky' lib/loader/native_helper_bridge.hexa = 0`)
- own1 alignment fn 측 명시: `surface_own1_alignment()` → `"apple_sdk_direct_binding_only"`

### §4.3 own1 NOT-IN-SCOPE 영구

Wine / CrossOver / Whisky / GPTK / Mono / CoreCLR / Vulkan / MoltenVK / 3rd-party D3D-to-Metal binding (Apple D3DMetal binary 외) / DRM evasion / signing bypass / anti-cheat 우회 — 영구 out-of-scope.

## §5 6 BG-AGB rank B+C unblock confirmation

본 Track R cycle 은 Track-P 측 6 BG-AGB rank B+C unblock candidate 중 **`loader_native_helper`** rank B 항목 측 land. predecessor marker (`state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` 측 `bg_agb_rank_b_c_unblock = true` + `bg_agb_rank_b_c_unblock_candidates[0]` = `loader_native_helper` rank B unblocked) 정합.

| candidate | rank | status | 비고 |
|---|---|---|---|
| **loader_native_helper** | **B** | **landed (본 Track R)** | 본 cycle 자체 land |
| perf_manjeom | B | unblocked | 별도 cycle |
| perf_game_d2r | B | unblocked | 별도 cycle |
| perf_game_d4 | C | unblocked | D4 retired_intentional 정합 |
| closure_orchestrator | C | unblocked | 별도 cycle |
| battlenet_bypass | (self) | landed (Track-P) | predecessor cycle |

**bg_agb_rank_b_c_unblock_predecessor**: `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` (Track-P, 본 cycle 측 unblock source).


### C1 — bridge layer = surface declaration only

본 Track R cycle 측 추가 5 Apple-native surface 측 = bridge layer **surface declaration only**. 실 syscall 발동은 `native/helper.mm` (E1+E2 subprocess) 측 책임.

### C2 — `native/` directory 측 modification 0건

본 cycle 측 `native/helper.mm` (3987 LOC) / `native/build.sh` / `native/d3dmetal_probe.sh` / `native/helper.entitlements` / `native/pe_to_macho_shim.cpp` 측 modification 0건. native source 측 actual extension 은 별도 dedicated cycle.

### C3 — 5 surface 측 first-frame round-trip 미land

본 cycle 측 5 surface 측 PARTIAL emit only — 실 first-frame round-trip 측 미land.

### C4 — D3DMetal binary licence transitive blocker

`loader_dx.blk.1` (D3DMetal binary licence + D4 always-online cross-blocked) 측 transitive 측 본 Track R 측 5 surface 측 일부 first-frame round-trip 측 영향.

### C5 — hexa CLI runtime emit 미수행

본 cycle 측 hexa CLI absent 환경 — schema validate (python3 json.loads + sha256) only.

### C6 — additive_only spirit lock-in

본 cycle 측 `lib/loader/native_helper_bridge.hexa` 측 modification = ADDITIVE only. 기존 6 fn / 8 invariant / 4 main sub 측 deletion / modification 0건.

### C7 — own1 정합 boundary 측 ack 권고

own1 정합 verdict = PASS 의 핵심 boundary = **"Apple SDK direct binding via hexa-native FFI scaffold = OK"**. 사용자 lock-in 시점 측 본 boundary ack 권고.

### C8 — Track R 측 future-cycle non-goals 영구 lock-in

본 doc §3 측 future-cycle non-goals 영구 out-of-scope. 사용자 lock-in 시점 측 본 list 측 ack 권고.

### C9 — emit count 측 PARTIAL 측 honest reflect

5 surface 측 emit format `__NATIVE_BRIDGE__ PARTIAL <surface>` — `PARTIAL` status 측 own2 정직 emit (실 syscall 발동 X / first-frame target 측 미달 / game install dependent — `MET` / `DONE` 측 사용 X).

### C10 — sister freeze cluster 측 5번째 sister doc

본 Track R 측 `docs/LOADER_NATIVE_HELPER_BRIDGE.md` 는 Track-E GameGuard + Track-J Warden + Track-L D2R realm protocol + Track-P battlenet_bypass own1 disambiguation 측 4 sister freeze / disambiguation cluster 측 5번째 sister doc. style mirror.

## §7 next-cycle hooks

- 별도 cycle 측 5 BG-AGB rank B+C 나머지 candidate (perf_manjeom / perf_game_d2r / perf_game_d4 / closure_orchestrator) 측 land 권고
- 별도 cycle 측 `native/helper.mm` 측 actual extension (kind=23+ IPC frame 추가 — Apple-native surface dispatch native side 측 round-trip)
- 별도 cycle 측 `lib/loader/pe_native_metal_device_wrapper.hexa` (184 LOC) + `lib/loader/pe_native_nsobject_lifetime.hexa` (160 LOC) 측 sister wrapper module 측 Track R 측 5 surface 와 cross-link annotation 추가 (additive only)
- 별도 cycle 측 `.roadmap.loader_dx` cond.1 evidence row 측 본 doc cross-link 추가 (in-place edit, 별도 cycle 측 land 권고)
- 별도 cycle 측 STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md 측 14/25 만점 → 재평가 (Track R 측 surface declaration land 후)
- 사용자 lock-in 후 production smoke — 사용자 정식 lock-in (L1+L2 + Track-R Apple-native bridge surface 명세 ack) 후 helper.mm build + codesign + entitlement 적용 + first-frame round-trip 측 verify (별도 cycle, 사용자 자체 책임 영역)

## §8 file index

### §8.1 files_created (3)

| path | type | LOC | bytes | sha256 |
|---|---|---|---|---|
| `docs/LOADER_NATIVE_HELPER_BRIDGE.md` | mk1 narrative spec freeze | 292 | 22324 | `b0881a4296f7c0dcd9405edddea1bfd4147880b9ad7be850701914409cdd218b` |
| `airgenome/doc/airgenome_gamebox_loader_native_helper_landed_2026_05_03.ai.md` | handoff doc | (this) | (this) | (self-referential) |
| `state/markers/airgenome_gamebox_loader_native_helper_landed.marker` | marker | (next) | (next) | (self-referential) |

### §8.2 files_modified (1)

| path | edit_kind | LOC before | LOC after | LOC delta | sha256 before | sha256 after |
|---|---|---|---|---|---|---|
| `lib/loader/native_helper_bridge.hexa` | additive_extend (5 surface fn + self_test invariant rows + main subcommand 4건) | 106 | 290 | +184 | `edd5f15ca04577b8449de9921b243acf03ca51ec571bc7eabaad5ecdf2dd25cc` | `52050420d719b7c7d390a82b87621206cf2db59d4d8cdbfc538e6227167a10e7` |

### §8.3 native/ directory (read-only, modification 0건)

| path | type | LOC | own1 정합 |
|---|---|---|---|
| `native/helper.mm` | Objective-C++ source | 3987 | ✓ Apple SDK only |
| `native/build.sh` | bash build script | 78 | ✓ Apple toolchain only |
| `native/d3dmetal_probe.sh` | bash probe script | 114 | ✓ Apple binary only |
| `native/helper.entitlements` | plist entitlements | 24 | ✓ Apple codesign only |
| `native/pe_to_macho_shim.cpp` | C++ source | 9875 | ✓ Apple toolchain only |

## §9 policy summary

| 항목 | 값 |
|---|---|
| migration | forbidden |
| changes | additive_only |
| in_place_writes | 0 |
| destructive_ops | 0 |
| cost_usd | 0 |
| substrate | mac-local |
| cap_minutes | 90 |
| br_no_user_verbatim | true |
| friendly_preset | true |
| silent_land_marker | true |
| own1_verdict | pass |
| own2_partial_emit | explicit |
| files_created | 3 |
| files_modified | 1 (lib/loader/native_helper_bridge.hexa, additive only) |
| native_directory_modified | 0 |
| roadmap_files_modified | 0 |
| bg_agb_rank_b_c_unblock_predecessor | `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` |
