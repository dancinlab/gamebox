# `lib/loader/native_helper_bridge.hexa` — Apple-native Helper Bridge Layer Spec Freeze (Track R)

> **status**: 작성일 **2026-05-03** / **scope: APPLE-NATIVE BRIDGE LAYER SPEC FREEZE ONLY** — 본 문서는 `lib/loader/native_helper_bridge.hexa` (hexa loader ↔ Apple-native helper IPC bridge skeleton) 의 **L2 long-horizon Apple-native Metal/macOS API integration** 측 surface 명세 동결.
> 작성: 2026-05-03 (Track-R loader_native_helper rank B+C cycle, sister Track-E GameGuard + Track-J Warden + Track-L D2R realm protocol + Track-P battlenet_bypass own1 disambiguation freeze cluster)
> SSOT: `lib/loader/native_helper_bridge.hexa` 본체 + 본 cycle 측 5 surface partial emit 추가 + `native/helper.mm` (E1+E2 helper subprocess) read-only inventory
> 결론 한줄: **`native_helper_bridge.hexa` = hexa loader 측 Apple SDK direct binding scaffold (own1 정합 PASS), Track R 측 5 Apple-native surface (Metal CommandQueue / MTLDevice / CGDirectDisplay / IOKit / NSObject lifecycle) PARTIAL emit 추가. 실 syscall 발동 영역은 helper.mm (own1 § A 명시 — Apple SDK only). Wine / CrossOver / Mono / CoreCLR / 3rd-party D3D-to-Metal binding 0줄. 본 doc 는 그 surface 명세의 mk1 narrative reference + future-cycle non-goals lock-in**.

---

## 0. 본 doc 의 목적과 한계 (가장 먼저 명시)

### 0.1 목적 (in-scope)

- `lib/loader/native_helper_bridge.hexa` 측 Apple-native helper bridge layer scope 측 mk1 narrative reference + spec freeze
- Track R 측 5 Apple-native surface (Metal CommandQueue / MTLDevice / CGDirectDisplay / IOKit / NSObject lifecycle) 측 PARTIAL emit 추가 측 own1 정합 evidence
- `native/` directory 측 5 Apple-native helper inventory (read-only — 본 cycle 측 native/ source modification 0건) 측 cross-link
- own1 정합 verdict 명시 (Apple platform component 측 = own1 정합 OK / Wine / CrossOver / Mono / CoreCLR / 3rd-party D3D-to-Metal binding 측 영구 out-of-scope)
- `loader_dx` + `loader_pe` + `loader_win32` 측 cross-link reference
- 6 BG-AGB rank B+C unblock 측 Track-P precedent 정합 (loader_native_helper = 본 Track R cycle 측 자체 land 대상)

### 0.2 한계 (out-of-scope, 영구)

본 doc 측 다음 카테고리는 **영구 out-of-scope** 이며, 어떤 형태의 implementation 도 airgenome-gamebox 측 import 되지 않음:

- Wine / CrossOver / Whisky / GPTK 측 source code import / 인용 / port (community wine code 0줄)
- CrossOver patch 측 cherry-pick / DXVK / VKD3D-Proton 측 import (Vulkan 측 binding X — Apple 은 Metal direct binding only)
- Apple D3DMetal binary 외 3rd-party D3D-to-Metal binding (MoltenVK 등) 측 import / 인용
- Mono / CoreCLR / .NET runtime 측 import (native helper 측 Apple Objective-C++ + clang only)
- Apple SDK 외 3rd-party framework 측 link (예: Vulkan SDK / OpenGL via MoltenVK 측 측 측 binding)
- DRM evasion / license circumvention / signing bypass / codesign forge
- Hook chain shim / signature spoof / NOP-patch / memory scan dodge / IAT hijack 측 native helper 측 적용 시도
- 어떤 형태의 anti-cheat (Warden / GameGuard / EAC / BattlEye / VAC) 측 우회 native helper

**본 doc 는 위 카테고리 중 어느 것도 implement 하지 않음**. Apple SDK direct binding via hexa-native FFI scaffold only — 실 syscall 발동 영역은 `native/helper.mm` (E1+E2 helper subprocess, own1 § A 명시), bridge layer 측 책임 = surface enumeration + IPC dispatch contract.

### 0.3 본 cycle 의 changes 측 minimal additive 정합

- `lib/loader/native_helper_bridge.hexa` 본체 측 fn / self_test / main / r0_emit 측 modification 0건 — 모두 ADDITIVE rows (5 새 fn + self_test 추가 invariant rows + main subcommand 4건 추가)
- 기존 `bridge_available` / `helper_protocol_version` / `build_request` / `parse_response` / `supported_ops` / `ping_via_metadata` 6 fn 측 modification 0건
- 기존 self_test 측 8 invariant 측 deletion / modification 0건 — 모두 보존, 5 surface invariant rows 추가만
- 기존 main dispatch 4 sub (self-test / ping / available / ops) 측 deletion / modification 0건 — 4 sub 추가만 (surfaces / surface-spec / surface-emit / own1)
- `docs/LOADER_NATIVE_HELPER_BRIDGE.md` (본 doc) NEW
- handoff doc + marker NEW
- destructive_ops 0 / migration X / additive_only

### 0.4 native/ directory 측 read-only posture

본 cycle 측 `native/helper.mm` / `native/build.sh` / `native/d3dmetal_probe.sh` / `native/helper.entitlements` / `native/pe_to_macho_shim.cpp` 측 modification 0건. native source 측 actual extension 은 별도 dedicated cycle (E1 helper extension / kind=23+ IPC frame 추가 / 측 측). 본 Track R 은 hexa loader 측 bridge layer surface 명세 동결 only.

---

## 1. native/ directory inventory + 5 Apple-native APIs

### 1.1 native/ directory 측 file inventory (read-only, 본 cycle 측 modification 0건)

| 항목 | type | 역할 | own1 정합 |
|---|---|---|---|
| `native/helper.mm` | Objective-C++ source (3987 LOC) | E1+E2 helper subprocess (22 IPC kind dispatch — dlopen / mmap / mprotect / pthread_create / dlsym / nsobject / metal_device / signal_register / fpu_reg / sse_xmm / user32_call / gdi32_call / thread_sync_call / nt_exception_call / vtable_dispatch_call / nt_dispatch_chain / cxx_throw_dispatch / sd_forging) | ✓ Apple SDK only (Foundation + AppKit + Metal + MetalKit + CoreGraphics + dlfcn + sys/mman + pthread) |
| `native/build.sh` | bash build script (78 LOC) | clang++ -arch arm64 + codesign + entitlement (E1+E2+E3 build chain) | ✓ Apple toolchain only |
| `native/d3dmetal_probe.sh` | bash probe script (114 LOC) | E4 D3DMetal binary location detect (Apple D3DMetal binary 만 허용 — own1 § A 명시) | ✓ Apple binary only |
| `native/helper.entitlements` | plist entitlements (24 LOC) | E2 codesign entitlements (allow-jit / allow-unsigned-executable-memory / disable-library-validation / allow-dyld-environment-variables) | ✓ Apple codesign only |
| `native/pe_to_macho_shim.cpp` | C++ source (9875 LOC) | E3 PE thin shim launcher (PE → Mach-O thin wrapper) | ✓ Apple toolchain only |
| `native/airgenome_helper` | Mach-O 64-bit arm64 binary (build artifact) | helper.mm 측 빌드 산출물 | ✓ Apple platform binary |
| `native/pe_to_macho_shim` | Mach-O 64-bit arm64 binary (build artifact) | shim.cpp 측 빌드 산출물 | ✓ Apple platform binary |

**total**: 5 source files (helper.mm + build.sh + d3dmetal_probe.sh + helper.entitlements + pe_to_macho_shim.cpp) + 2 build artifacts (Mach-O arm64). 모두 Apple SDK / Apple toolchain only.

### 1.2 5 Apple-native APIs (Track R 측 surface partial emit 대상)

본 Track R cycle 측 hexa loader bridge layer 측 surface 명세 추가:

| # | surface name | Apple SDK API | helper.mm 측 IPC kind | 역할 |
|---|---|---|---|---|
| 1 | `metal_command_queue` | `[MTLDevice newCommandQueue]` + `[queue commandBuffer]` | kind=7 metal_device 후속 | Metal command submission 측 first-frame swapchain prereq |
| 2 | `mtl_device_probe` | `MTLCreateSystemDefaultDevice()` | kind=7 metal_device | Metal device 측 acquire (loader_dx cond.1 first-frame prereq) |
| 3 | `cg_direct_display` | `CGGetActiveDisplayList(max, displays, count)` | (helper 측 CoreGraphics direct call) | Display enumeration (first-frame swapchain target display 측 query) |
| 4 | `iokit_service_match` | `IOServiceMatching("IOAccelerator")` + `IOServiceGetMatchingServices` | (helper 측 IOKit direct call) | Metal device 측 IOKit registry 질의 (GPU 측 vendor / class 측 enumeration) |
| 5 | `ns_object_lifecycle` | `[[NSObject alloc] init]` / `release` / `@autoreleasepool` block | kind=6 nsobject | Foundation NSObject lifetime (objc_msgSend ABI lr/x0/x1 packing) |

### 1.3 5 surface 측 own1 정합 verdict

5 surface 모두 **Apple SDK direct binding** — Apple platform component 측 = own1 정합 OK.

- ✓ `MTLCreateSystemDefaultDevice` = Metal.framework (Apple)
- ✓ `[MTLDevice newCommandQueue]` = Metal.framework Objective-C msgSend (Apple)
- ✓ `CGGetActiveDisplayList` = CoreGraphics.framework (Apple)
- ✓ `IOServiceMatching` / `IOServiceGetMatchingServices` = IOKit.framework (Apple)
- ✓ `[[NSObject alloc] init]` / `@autoreleasepool` = Foundation.framework (Apple)

Wine / CrossOver / Mono / CoreCLR / Vulkan / MoltenVK / 3rd-party D3D-to-Metal binding 측 0줄 — 5 surface 어디에도 occurrence X.

### 1.4 PARTIAL emit pattern

본 cycle 측 emit format:

```
__NATIVE_BRIDGE__ PARTIAL <surface>
```

- `__NATIVE_BRIDGE__` = sentinel prefix (other domain 측 sentinel 와 분리 — `__LOADER_DX__` / `__BNET_WARDEN__` / `__BNET_LAUNCH__` / `__GAMEGUARD__` 등)
- `PARTIAL` = honest status (own2 정직 emit — surface 측 declaration only, 실 syscall 발동 X / first-frame target 측 미달 / game install dependent)
- `<surface>` = `metal_command_queue` / `mtl_device_probe` / `cg_direct_display` / `iokit_service_match` / `ns_object_lifecycle` 5 중 1

5 surface 측 self_test 측 invariant verification + emit (5건 emit / surface).

---

## 2. own1 정합 (Apple platform component = OK, Wine/CrossOver wrapper 0)

### 2.1 own1 정합 verdict — PASS

본 Track R cycle 측 추가 5 surface 모두 **Apple SDK direct binding via hexa-native FFI scaffold** — own1 정합 PASS.

| 항목 | 값 |
|---|---|
| Wine source import | 0줄 |
| CrossOver patch cherry-pick | 0줄 |
| Whisky / GPTK staging patch import | 0줄 |
| Mono / CoreCLR runtime import | 0줄 |
| Vulkan SDK / MoltenVK 측 binding | 0줄 |
| 3rd-party D3D-to-Metal binding (Apple D3DMetal binary 외) | 0줄 |
| Apple SDK direct binding (Metal / Foundation / AppKit / CoreGraphics / IOKit) | ✓ 100% |
| hexa-native FFI scaffold (loader bridge layer) | ✓ surface declaration only |
| 실 syscall 발동 영역 | helper.mm (E1+E2 subprocess, own1 § A 명시) — bridge layer 측 책임 X |

### 2.2 own1 = "Apple platform component = OK" 정합 evidence

- evidence 1: **`MTLCreateSystemDefaultDevice` 측 vendor**: Apple Metal.framework — Apple platform component, own1 정합 OK
- evidence 2: **`[[NSObject alloc] init]` 측 vendor**: Apple Foundation.framework — Apple platform component, own1 정합 OK
- evidence 3: **`CGGetActiveDisplayList` 측 vendor**: Apple CoreGraphics.framework — Apple platform component, own1 정합 OK
- evidence 4: **`IOServiceMatching` 측 vendor**: Apple IOKit.framework — Apple platform component, own1 정합 OK
- evidence 5: **`[MTLDevice newCommandQueue]` 측 vendor**: Apple Metal.framework Objective-C msgSend — Apple platform component, own1 정합 OK
- evidence 6: **bridge layer 측 syscall 발동 X**: hexa loader 측 sandboxed bash exec only — bridge layer 측 surface declaration + IPC dispatch contract only, 실 syscall 발동 X (helper.mm 측 책임)
- evidence 7: **Wine / CrossOver / GPTK / Whisky 측 occurrence audit**: bridge module 측 occurrence 0건 (`grep -ic 'wine\|crossover\|gptk\|whisky' lib/loader/native_helper_bridge.hexa = 0`)
- evidence 8: **own1 alignment fn 측 명시**: `surface_own1_alignment()` → `"apple_sdk_direct_binding_only"` (self_test 측 invariant verification)

### 2.3 own1 NOT-IN-SCOPE 영구 (Track R 측 명시)

- ❌ Wine source code import / 인용 / port — 영구 out-of-scope
- ❌ CrossOver patch cherry-pick — 영구 out-of-scope
- ❌ Mono / CoreCLR runtime import — 영구 out-of-scope
- ❌ Apple D3DMetal binary 외 3rd-party D3D-to-Metal binding (MoltenVK / DXVK / VKD3D-Proton 측) — 영구 out-of-scope
- ❌ Vulkan SDK 측 link — 영구 out-of-scope (Apple 은 Metal direct binding only)
- ❌ DRM evasion / signing bypass / codesign forge — 영구 out-of-scope
- ❌ Anti-cheat (Warden / GameGuard / EAC / BattlEye / VAC) 측 우회 native helper — 영구 out-of-scope (Track-J + Track-E + Track-P sister cluster 정합)

---

## 3. Future-cycle non-goals (영구 out-of-scope)

본 Track R cycle 측 명시적 NOT-DOING (영구 out-of-scope, future cycle 측 implementation X):

### 3.1 Wine source / CrossOver patch / DXVK / VKD3D-Proton import

- Wine source code (winelib / wine-staging / wine-mono / wine-gecko 등) 측 import / 인용 / port 측 0건
- CrossOver / Whisky / GPTK staging patch cherry-pick 측 0건
- DXVK / VKD3D-Proton 측 import (Vulkan-based D3D translation) 측 0건
- 본 Track R 측 5 Apple-native surface 측 어디에도 Wine source 측 reference X

### 3.2 3rd-party D3D-to-Metal binding (MoltenVK 등)

- MoltenVK 측 import (Vulkan-on-Metal binding) 측 0건
- moltenvk-priv / vkapi 측 link 측 0건
- Apple D3DMetal binary 외 3rd-party D3D translation 측 fetch / wrap 측 0건
- 본 Track R 측 5 Apple-native surface 측 어디에도 Vulkan / MoltenVK reference X

### 3.3 Mono / CoreCLR / .NET runtime import

- Mono runtime (libmono-2.0 / mono-sgen 등) 측 link 측 0건
- CoreCLR (.NET runtime) 측 import 측 0건
- C# managed code 측 binding 측 0건
- 본 Track R 측 5 Apple-native surface 측 어디에도 Mono / CoreCLR reference X

### 3.4 Apple SDK 외 3rd-party framework binding

- Vulkan SDK 측 link 측 0건 (Apple 은 Metal direct binding only)
- OpenGL via MoltenGL 측 binding 측 0건
- SDL / GLFW / 측 측 측 cross-platform abstraction layer 측 binding 측 0건
- 본 Track R 측 5 Apple-native surface 측 어디에도 3rd-party framework reference X

### 3.5 hexa-native FFI scaffold limitation

본 Track R cycle 측 land 영역 = bridge layer **surface declaration only**. 다음 영역은 별도 dedicated cycle (Track R 외부):

- 실 IPC frame schema 추가 (kind=23+ Apple-native surface dispatch) — `native/helper.mm` extension 영역
- 실 MTLCreateSystemDefaultDevice 호출 round-trip — helper.mm kind=7 metal_device 측 이미 존재 (본 cycle 측 surface declaration only)
- 실 IOKit registry 질의 round-trip — helper.mm kind=23+ 추가 시점 별도 cycle
- 실 CGGetActiveDisplayList 호출 round-trip — helper.mm 측 CoreGraphics direct call 추가 시점 별도 cycle

---

## 4. Cross-link to loader_dx + loader_pe + loader_win32

### 4.1 loader_dx cross-link

- `.roadmap.loader_dx` 측 cross_link.loader_native_helper_dependency 명시: `"native/helper.mm Metal layer bridge (CAMetalLayer + MTLDevice + MTLCommandQueue)"`
- 본 Track R 측 surface 1 (`metal_command_queue`) + surface 2 (`mtl_device_probe`) 측 = `loader_dx.cond.1` (D3D11 stage2 first-frame) + `loader_dx.cond.2` (D3D12 + DXR PSO genome harvest live) 측 prerequisite
- loader_dx.cond.1 측 first-frame swapchain 측 → bridge layer surface 1 + 2 + 3 (CGDirectDisplay) 측 IPC dispatch contract 측 정합

### 4.2 loader_pe cross-link

- `.roadmap.loader_pe` 측 cross_link 측 native_helper bridge 측 reference
- `lib/loader/pe_native_helper_invoke.hexa` (392 LOC, IPC dispatch gate) 측 bridge layer 측 sister module
- `lib/loader/pe_native_dlopen_wrapper.hexa` / `pe_native_dlsym_wrapper.hexa` / `pe_native_mmap_wrapper.hexa` / `pe_native_mprotect_wrapper.hexa` / `pe_native_pthread_wrapper.hexa` / `pe_native_metal_device_wrapper.hexa` / `pe_native_nsobject_lifetime.hexa` / `pe_native_callback_dispatch.hexa` 측 9 sister wrapper module
- 본 Track R 측 surface 5 (`ns_object_lifecycle`) 측 = `pe_native_nsobject_lifetime.hexa` 측 sister
- 본 Track R 측 surface 2 (`mtl_device_probe`) 측 = `pe_native_metal_device_wrapper.hexa` 측 sister

### 4.3 loader_win32 cross-link

- `.roadmap.loader_win32` 측 win32 stub cluster 측 → bridge layer 측 IPC dispatch (kind=14 user32_call / kind=15 gdi32_call / kind=17 thread_sync_call / kind=18 nt_exception_call / kind=22 sd_forging) 측 정합
- 본 Track R 측 surface 5 (`ns_object_lifecycle`) 측 = win32 측 GDI object / window handle 측 native bridge prereq

### 4.4 sister freeze / disambiguation cluster cross-link

- Track-E `docs/GAMEGUARD_HONEST_FREEZE.md` (kernel-mode AC honest freeze)
- Track-J `docs/WARDEN_HONEST_FREEZE.md` (user-mode AC honest freeze)
- Track-L `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` (network protocol honest freeze)
- Track-P `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` (own1 naming disambiguation)
- 본 Track-R `docs/LOADER_NATIVE_HELPER_BRIDGE.md` (Apple-native bridge surface freeze) — 5번째 sister doc

### 4.5 stage7 native bridge mk1 narrative cross-link

- `docs/STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md` (cycle 63 시점 14/25 만점 검토) — 본 Track R 측 mk1 narrative predecessor
- `docs/MINIMAL_NATIVE_PLAN.md` — minimal native plan reference

### 4.6 6 BG-AGB rank B+C unblock confirmation

본 Track R cycle 은 Track-P 측 6 BG-AGB rank B+C unblock candidate 중 **`loader_native_helper`** rank B 항목 측 land. 나머지 5 candidate (perf_manjeom / perf_game_d2r / perf_game_d4 / closure_orchestrator / battlenet_bypass) 측 별도 cycle 측 land 권고.

| candidate | rank | status (이번 Track R 시점) | next |
|---|---|---|---|
| loader_native_helper | B | **landed (본 Track R)** | - |
| perf_manjeom | B | unblocked (Track-P precedent) | 별도 cycle |
| perf_game_d2r | B | unblocked (Track-P precedent) | 별도 cycle |
| perf_game_d4 | C | unblocked (Track-P precedent + D4 retired_intentional 정합) | 별도 cycle |
| closure_orchestrator | C | unblocked (Track-P precedent) | 별도 cycle |
| battlenet_bypass | (self) | landed (Track-P) | - |

---


### C1 — bridge layer = surface declaration only, 실 syscall 발동 X

본 Track R cycle 측 추가 5 Apple-native surface 측 = bridge layer **surface declaration only**. 실 syscall 발동은 `native/helper.mm` (E1+E2 subprocess) 측 책임. hexa loader 측 sandboxed bash exec only — Mach-O arm64 syscall 직접 발동 path X. 사용자 production 측 helper.mm 측 build + codesign + entitlement 측 적용 확인 필수.

### C2 — `native/helper.mm` 측 modification 0건 (read-only)

본 cycle 측 `native/helper.mm` (3987 LOC) / `native/build.sh` / `native/d3dmetal_probe.sh` / `native/helper.entitlements` / `native/pe_to_macho_shim.cpp` 측 modification 0건. native source 측 actual extension (kind=23+ IPC frame 추가) 은 별도 dedicated cycle. 본 Track R 측 = bridge layer surface 명세 동결 only — native side 측 implementation 측 도달 X.

### C3 — 5 surface 측 first-frame round-trip 미land

본 cycle 측 5 surface (metal_command_queue / mtl_device_probe / cg_direct_display / iokit_service_match / ns_object_lifecycle) 측 PARTIAL emit only — 실 first-frame round-trip (D2R 또는 D4 측 game install + first-frame trace) 측 미land. emit format = `__NATIVE_BRIDGE__ PARTIAL <surface>` (own2 정직 emit, status PARTIAL honest reflect).

### C4 — D3DMetal binary licence transitive blocker

`loader_dx.blk.1` (D3DMetal binary licence + D4 always-online cross-blocked) 측 transitive 측 본 Track R 측 5 surface 측 일부 (특히 surface 1 metal_command_queue + surface 2 mtl_device_probe) 측 first-frame round-trip 측 영향. 본 cycle 측 surface declaration only — licence boundary 측 honest documentation 만 (Apple D3DMetal binary licence 측 README License section 인용 영역 영구).

### C5 — hexa CLI runtime emit 미수행

본 cycle 측 hexa CLI absent 환경 (predecessor cycle posture) — schema validate (python3 json.loads + sha256) only. `lib/loader/native_helper_bridge.hexa` 측 self_test 측 실 hexa runtime 측 verification 측 사용자 lock-in 후 별도 production smoke 시점 권고.

### C6 — additive_only spirit lock-in

본 cycle 측 `lib/loader/native_helper_bridge.hexa` 측 modification = ADDITIVE only (5 새 fn + self_test 추가 invariant rows + main subcommand 4건 추가). 기존 6 fn / 8 invariant / 4 main sub 측 deletion / modification 0건. git diff 측 pure insertion (no `-` line in diff body, except possibly 마지막 line 측 fall-through usage line 측 newline 측 정합).

### C7 — own1 = "Apple platform component = OK" 정합 boundary

본 doc 측 own1 정합 verdict = PASS 의 핵심 boundary = **"Apple SDK direct binding via hexa-native FFI scaffold = OK"**. 즉 Apple Metal.framework / Foundation.framework / AppKit.framework / CoreGraphics.framework / IOKit.framework 측 direct binding 은 own1 정합 OK. 단 Apple platform component 측 binding 의 정합 여부 측 사용자 lock-in 시점 측 ack 권고 — community wine wrapper / 3rd-party D3D-to-Metal binding 측 측 측 boundary 측 명시적 ack 필요.

### C8 — Track R 측 future-cycle non-goals 영구 lock-in

본 doc §3 측 future-cycle non-goals (Wine source / CrossOver patch / DXVK / VKD3D-Proton / MoltenVK / Mono / CoreCLR / Vulkan SDK / 3rd-party D3D-to-Metal binding) 측 영구 out-of-scope. 사용자 lock-in 시점 측 본 list 측 ack 권고. 본 list 측 추가 항목 측 미래 cycle 측 발견 시 본 doc 측 §3 측 추가만 (additive only — list 측 항목 측 deletion X).

---

## 6. 출처

본 doc 측 외부 reference 출처:

1. Apple Metal Programming Guide (Apple developer.apple.com/metal) — `MTLCreateSystemDefaultDevice` / `MTLDevice` / `MTLCommandQueue` / `MTLCommandBuffer` 측 official spec
2. Apple Foundation Reference (developer.apple.com/foundation) — `NSObject` lifecycle / `@autoreleasepool` 측 official spec
3. Apple AppKit Reference (developer.apple.com/appkit) — `NSApplication` / `NSWindow` 측 official spec
4. Apple CoreGraphics Reference (developer.apple.com/coregraphics) — `CGDirectDisplay` / `CGGetActiveDisplayList` 측 official spec
5. Apple IOKit Reference (developer.apple.com/iokit) — `IOServiceMatching` / `IOServiceGetMatchingServices` 측 official spec
6. Apple Game Porting Toolkit (developer.apple.com/games/game-porting-toolkit) — D3DMetal binary 측 high-level reference (own1 § A — Apple D3DMetal binary 만 허용 명시)
7. Apple Silicon W^X / JIT documentation — `pthread_jit_write_protect_np` API 사용
8. `docs/STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md` (cycle 63 14/25 만점 검토 — 본 Track R 측 mk1 narrative predecessor)
9. `docs/MINIMAL_NATIVE_PLAN.md` — minimal native plan reference
10. `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` — Track-P 측 6 BG-AGB rank B+C unblock candidate 측 본 Track R cycle 측 source

본 doc + airgenome-gamebox 본체 측 Wine / CrossOver / Whisky / GPTK source 측 fetch / import / wrapper 0줄. Apple SDK direct binding via hexa-native FFI scaffold only.
