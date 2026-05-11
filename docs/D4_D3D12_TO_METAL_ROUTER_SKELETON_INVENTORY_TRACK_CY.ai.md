# D4 D3D12-to-Metal Router Skeleton Inventory — Track CY Landed (2026-05-05)

> Track CY on gamebox standalone repo, branch `main`. Doc-only execution of
> phase D4-CS-β (D3D12-to-Metal router skeleton inventory) from the Track CS
> D4 forward roadmap. Per-router-link composition map across 7 Metal-side
> modules + 15 D3D12-side surface modules audited under Track CV (α phase).
> Honest posture: every router link is composition-only at skeleton level;
> no D3DMetal binary embedded; no Apple private SPI used beyond public
> Metal / MetalFX / Foundation / IOKit / CoreGraphics; `validated_manjeom = 0`
> carries; D4 frame translation feasibility paper-mode only — gated on G1
> hexa interp closure (CM-26..CM-30, perpetually deferred per CN) plus G2
> router validated execution (multi-year horizon per CS §3). CY lands
> strictly after sibling cycles CW and CX to preserve CW → CX → CY ordering.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CX 2026-05-05).
- Direct predecessor (alphabetic): Track CX.
- Direct predecessor (parallel sibling): Track CW (CW + CX ran in parallel
  before CY).
- Semantic predecessor (α phase complement): Track CV
  (`docs/D4_D3D12_SURFACE_HONESTY_AUDIT_TRACK_CV.ai.md`, commit 7513be9) —
  CY is the β complement (router-side, not surface-side). CV audited the
  D3D12 surface; CY inventories the D3D12-to-Metal router composition.
- Semantic predecessor (roadmap source): Track CS
  (`docs/D4_ROADMAP_TRACK_CS.ai.md`, commit 41ddb75) — phase D4-CS-β exit
  criteria source per §4.2: "1 router inventory doc landed; gap list with
  priority + own1-compatible resolution path per gap; explicit no Apple
  D3DMetal embedding reaffirmation".
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- Documentation tier — no code emission, no module addition, no
  `validated_manjeom` promotion. Per-router-link honest tier annotation
  only.

---

## Mission

CY1 — Honest per-router-link inventory across the D3D12-to-Metal router
composition. The router is **not a single module** (per CS §1.5 and CV
§3); it is a composition of:

- D3D12 surface side: 15 modules audited under CV §1 (rows 1-15).
- Metal side: 7 modules in `lib/loader/` matching `metal|router|d3dmetal`
  pattern.
- Cross-cutting concept: `docs/LOADER_NATIVE_HELPER_BRIDGE.md` (Track R
  spec freeze; native/helper.mm IPC bridge concept).
- External reference (NOT embedded): Apple D3DMetal binary from GPTK
  4.0/4.1/5.0 (link target only — own1 § A allows reference; embedding
  forbidden).
- External reference (NOT used directly): Apple private SPIs beyond
  public Metal / MetalFX / Foundation / IOKit / CoreGraphics framework
  surfaces.

For each router link: D3D12 source module → Metal-side target module
path, link kind (delegate / converter / fallback / passthrough),
honest tier (skeleton / stage1 / composition-only / stub), real-stream
evidence, own1 stance.

CY2 — handoff doc + marker (this file + Track CY marker).

---

## §1 Router composition map — per-link inventory

The conceptual D3D12-to-Metal router has **N = 7 distinct Metal-side
links** identified in `lib/loader/` under the `metal|router|d3dmetal`
pattern. Each link is paired with one or more D3D12 surface modules
audited in CV §1. The link kind enumeration is taken from
`pe_d3d_to_metal_router.hexa` (the routing decision tracker module
itself, row 1 below).

Legend:
- **Link** — Metal-side module file path.
- **D3D12 surface peer(s)** — CV §1 row(s) supplying the D3D12 side of
  the bridge.
- **Link kind** — per `pe_d3d_to_metal_router::route_kind` enum:
  `1=d3dmetal_delegate` / `2=metal_shaderconverter` /
  `3=diy_hexa_fallback` / `4=passthrough` (or N/A for non-router-decision
  modules).
- **Tier** — `skeleton` / `composition-only` / `stub` (none reach
  stage1/stage2/validated).
- **Real-stream evidence** — what real D3D12 → Metal command stream this
  link has carried under a real D4 frame. Must be `none` for every row.
- **own1 stance** — `apple_sdk_only` (public Metal/MetalFX/Foundation/
  IOKit/CoreGraphics only) / `gptk_link_target` (D3DMetal binary as weak
  link target, NOT embedded) / `helper_ipc_bridge` (deferred to
  `native/helper.mm` IPC, public Apple SDK only).

| # | Link | Path | D3D12 surface peer(s) (CV §1 row) | Link kind | Tier | Real-stream evidence | own1 stance |
|---|---|---|---|---|---|---|---|
| 1 | pe_d3d_to_metal_router | `lib/loader/pe_d3d_to_metal_router.hexa` | rows 1-13 (entire dx_d3d12 + pe_d3d12_* family); 14 (DXGI); 15 (present) | 1+2+3+4 (decision tracker — emits route_kind per source method) | skeleton (TSV row tracker; "실 Metal API 호출 X (later phase)" — header line 15) | none (decision tracker only; no real D3D12 method intercepted, no real Metal API issued) | gptk_link_target (header line 12 declares D3DMetal binary delegation 명시 허용; module itself does NOT embed) |
| 2 | pe_d3dmetal_link_status | `lib/loader/pe_d3dmetal_link_status.hexa` | rows 1, 5-7, 11-13 (D3D12 family modules whose `_real` headers declare "D3DMetal 위임 stage") | 1 (d3dmetal_delegate weak-link readiness probe) | skeleton (TSV row tracker for link_id / phase_kind / link_kind / success — header line 27) | none (no real `dlopen("D3DMetal.dylib")` call; phase_kind 1-5 enumerate gptk_locate / d3dmetal_dylib_path / weak_link_verify / metal_device_bridge / binding_test as synthetic phases) | gptk_link_target (header line 4: "Apple GPTk D3DMetal binary 를 link target 으로 둠 (own1 § A 명시 허용 범위)") |
| 3 | pe_metal_buffer_pool | `lib/loader/pe_metal_buffer_pool.hexa` | rows 4-6 (command_list, command_queue_real, descriptor_heap_real — buffer allocation peers); also peers `pe_d3d12_resource_barrier_real` (row 11) for buffer state transitions | N/A (resource-side tracker; no route_kind) | skeleton (TSV row tracker for buffer_id / usage_kind / size_bytes / reuse_count — header line 17) | none (no real `MTLDevice newBufferWithLength` call; "실제 MTLBuffer 할당 / reuse policy 는 native helper layer (stage 후속) 책임" — header line 4) | helper_ipc_bridge (deferred to native/helper.mm; public Metal SDK only) |
| 4 | pe_metal_device_weak_link_real | `lib/loader/pe_metal_device_weak_link_real.hexa` | rows 1-2 (dx_d3d12, dx_d3d12_stage1 — device acquire peers); 13-15 (swapchain_present_real, dxgi_swapchain, swapchain_present_track — present chain peers) | 1+4 (delegate via helper IPC kind=7 metal_device + passthrough handle bridge) | skeleton (TSV row tracker for frame_id / phase_kind / device_obtained / elevated; `_real` filename = naming-only; header line 1 declares "phase 13 first frame: helper IPC kind=7 metal_device 호출") | none (no real `MTLCreateSystemDefaultDevice()` call from this module; phase_kind 1-7 metal_device_request → device_handle_received → command_queue_create → metal_layer_attach → drawable_acquire → command_buffer_commit → present_drawable enumerated as synthetic phases) | apple_sdk_only (header line 5: "Apple Metal SDK only — third-party Wine/CrossOver/community Metal wrapper 없음. Apple 의 Metal framework 는 own1 § A 명시 허용 1st-party SDK") |
| 5 | pe_metal_texture_residency | `lib/loader/pe_metal_texture_residency.hexa` | row 6 (descriptor_heap_real for SRV/UAV residency peer); row 11 (resource_barrier_real for texture state transitions); row 13 (swapchain_present_real for backbuffer texture peer) | N/A (resource-side tracker; no route_kind) | skeleton (TSV row tracker for texture_id / format_kind / size_kb / gpu_resident — header line 16; "실제 MTLTexture / heap api 호출 X" — header line 3) | none (no real `MTLDevice newTextureWithDescriptor` call; format_kind enum 1-4 rgba8/bgra8/bc7/astc enumerated as synthetic format codes) | apple_sdk_only (public Metal SDK only; no IOSurface private SPI used) |
| 6 | pe_native_metal_device_wrapper | `lib/loader/pe_native_metal_device_wrapper.hexa` | rows 1-2 (dx_d3d12, dx_d3d12_stage1 — device init wrapper peers); cross-link to c_metal_pipeline_archive (cycle 49+ peer per header line 5) | 1 (d3dmetal_delegate via Apple D3DMetal binary mapping per header line 4) | skeleton (TSV row tracker for call_id / device_kind / family_tier / success — header line 41 schema) | none (no real `MTLCreateSystemDefaultDevice()` call from this wrapper; device_kind enum apple_silicon/intel_discrete/external_gpu and family_tier enum A14/M1/M2/M3/M4 enumerated as synthetic device taxonomy) | gptk_link_target (header line 3-5 declares D3D11/D3D12 device → Apple D3DMetal binary 통해 매핑; binary itself NOT embedded) |
| 7 | pe_perf_metalfx_upscale | `lib/loader/pe_perf_metalfx_upscale.hexa` | row 13 (swapchain_present_real — pre-present upscale peer); row 15 (swapchain_present_track — Metal-side present timing peer); also CS §1.4 D4 perf telemetry surface | N/A (perf-side tracker; no route_kind; MetalFX is post-router) | skeleton (TSV row tracker for upscale_id / scaler_kind / input_resolution / output_resolution — header line 3 schema; `record_upscale` panic guards on 1≤scaler_kind≤3) | none (no real `MTLFXSpatialScaler` / `MTLFXTemporalScaler` instance created or `encodeToCommandBuffer:` call issued; scaler_kind enum 1-3 is synthetic taxonomy only) | apple_sdk_only (header line 2: "Apple MetalFX framework only. NVIDIA DLSS / AMD FSR 인용 X") |

**Row count**: 7 router-side Metal modules inventoried. **Tier
breakdown**: 7 skeleton / 0 stage1 / 0 stage2 / 0 composition-only-named
/ 0 stub-only / 0 validated. **Aggregate router posture**: 7 router
links × 15 D3D12 surface peers (CV §1) = composition-only at skeleton
level across the entire bridge. **Real-stream observation evidence**:
`none` across all 7 (every module is a TSV-row tracker; no real D3D12
command from a real D4 frame has crossed any link to a real Metal API
call under this repo's own1 path).

---

## §2 Per-router-link honest tier — explicit annotation

| # | Link | Honest tier (CY annotation) | `_real` naming-only? | validated_manjeom |
|---|---|---|---|---|
| 1 | pe_d3d_to_metal_router | skeleton (routing decision tracker; declares the route_kind taxonomy 1=d3dmetal_delegate / 2=metal_shaderconverter / 3=diy_hexa_fallback / 4=passthrough but does not execute any of them) | — | 0 |
| 2 | pe_d3dmetal_link_status | skeleton (D3DMetal weak-link readiness phase tracker; declares phase_kind 1-5 sequence but does not perform real `dlopen` / `dlsym`) | — | 0 |
| 3 | pe_metal_buffer_pool | skeleton (Metal buffer reuse pool event tracker; defers real MTLBuffer alloc to native helper layer) | — | 0 |
| 4 | pe_metal_device_weak_link_real | skeleton (`_real` suffix is naming-only; header explicitly declares phase 13 helper IPC kind=7 metal_device call as future, not current) | Y (filename `_real` carries no validated execution) | 0 |
| 5 | pe_metal_texture_residency | skeleton (Metal GPU texture residency tracker; declares format taxonomy but does not call MTLTexture API) | — | 0 |
| 6 | pe_native_metal_device_wrapper | skeleton (Metal device init wrapper TSV recorder; declares device_kind/family_tier taxonomy but does not call `MTLCreateSystemDefaultDevice()`) | — | 0 |
| 7 | pe_perf_metalfx_upscale | skeleton (MetalFX upscaler event tracker; declares scaler_kind 1-3 enum but does not instantiate MTLFXSpatialScaler / MTLFXTemporalScaler) | — | 0 |

**Aggregate**: 7/7 skeleton, 1/7 `_real` naming-only (row 4),
`validated_manjeom = 0` carries on all 7. No router link has been
promoted from skeleton to stage1, composition-only-named, or validated
under this CY cycle. CY does not propose any promotion. The aggregate
router remains **composition-only at skeleton level** per Track CS §1.5
and CV §3.

---

## §3 Gap list — own1-compatible resolution path

Per Track CS §3 gap inventory, restricted to the 7 router links above:

| Gap | Class | Affected router link(s) | own1-compatible resolution path |
|---|---|---|---|
| RG1 (= CS G1) hexa interp closure CM-26..CM-30 | runtime-launch blocker | all 7 (every link's TSV recording requires the hexa loader to actually run real-tier code under hexa interp) | sibling repo `hexa-lang` work; perpetually deferred per Track CN; CY honors deferral |
| RG2 (= CS G2) router validated execution | skeleton → validated promotion | 1, 2, 4, 6 (decision tracker + D3DMetal link status + device weak link + native device wrapper) | depends on RG1 + real D4 frame capture; multi-year horizon; CY does NOT propose path closure |
| RG3 (= CS G3) mesh shader real path | skeleton → stage1+ | 1 (decision tracker route_kind=2 metal_shaderconverter for DXIL→MSL→AIR mesh path) | Apple Metal mesh shader maturity per device; Apple SDK public surface only; no third-party converter |
| RG4 (= CS G4) DXR real path on Apple | skeleton → stage1+ | 1 (decision tracker DXR routes); also coupled to CV row 3 dx_d3d12_stage2 (DXR command recording) | Apple Metal raytracing tier per device; Apple SDK public `MTLAccelerationStructure` only; no Apple private SPI for BVH |
| RG5 D3DMetal weak link runtime | skeleton → real `dlopen` | 2 (link_status), 6 (device wrapper) | Apple D3DMetal binary path probe + weak link via `dlopen` ONLY (NOT embedded); per `native/d3dmetal_probe.sh` E4 reference; depends on RG1 |
| RG6 Metal buffer pool real allocator | skeleton → real `MTLDevice newBufferWithLength` | 3 (buffer pool) | public Metal SDK API; deferred to `native/helper.mm` IPC bridge per LOADER_NATIVE_HELPER_BRIDGE.md; no MoltenVK / no third-party pool |
| RG7 Metal texture residency real query | skeleton → real `MTLHeap` / `MTLTexture` | 5 (texture residency) | public Metal SDK API + `MTLResource setPurgeableState:` public; no IOSurface private SPI; no Apple Driver private API |
| RG8 (= CS G8) MetalFX real upscale | skeleton → real `MTLFXSpatialScaler` instantiate + encode | 7 (metalfx upscale) | public MetalFX framework only; no DLSS / no FSR; depends on RG2 |
| RG9 IOKit GPU enumeration | skeleton → real `IOServiceMatching("IOAccelerator")` | 6 (device wrapper family_tier resolution) | public IOKit per LOADER_NATIVE_HELPER_BRIDGE.md §1.2 row 4; via `native/helper.mm` IPC; no Apple driver private kext call |
| RG10 (= CS G6) Battle.net always-online entry | structurally interrupted | none of the 7 router links can resolve this | OUT-OF-SCOPE under own1 retired_intentional posture per Track CS §2 |
| RG11 (= CS G7) BattlEye anti-cheat surface | structurally interrupted | none of the 7 router links | OUT-OF-SCOPE under own1 anti-cheat-bypass-0 posture; cross-class with EAC freeze (CL) and VAC freeze (CK) |

**Critical chain (router-specific)**: RG1 → RG5 → RG2 → {RG3, RG4, RG6,
RG7, RG8, RG9}. RG10 + RG11 are structurally interrupted (out-of-scope)
and never resolve in this repo. The router thus cannot reach validated
execution against a real D4 frame even if RG1..RG9 land, because the D4
session itself cannot reach first-frame under own1 (RG10 + RG11 block).

This is the same honest reality CS §3 observed at roadmap level; CY
restates it at the per-router-link granularity.

---

## §4 Explicit no-D3DMetal-embed reaffirmation

Per Track CS §4.2 phase D4-CS-β exit criterion ("explicit no Apple
D3DMetal embedding reaffirmation"):

CY explicitly reaffirms — at the router-link granularity — that **no
Apple D3DMetal binary is embedded in this repo**. Specifically:

- Row 1 `pe_d3d_to_metal_router.hexa` header line 12 declares the
  D3DMetal binary as a delegation target ("D3DMetal binary 명시 허용 —
  MoltenVK / DXVK / VKD3D 인용 X") — this is **reference-only** in
  module comments; the module itself contains 0 lines of D3DMetal source
  and 0 bytes of D3DMetal binary.
- Row 2 `pe_d3dmetal_link_status.hexa` header line 4-5 declares D3DMetal
  as a **weak link target only** ("Apple GPTk D3DMetal binary 를 link
  target 으로 둠 (own1 § A 명시 허용 범위)") — the module records the
  phase_kind sequence (gptk_locate → d3dmetal_dylib_path →
  weak_link_verify → metal_device_bridge → binding_test) as TSV rows;
  no real `dlopen` is performed; no D3DMetal binary is bundled with
  this repo.
- Row 6 `pe_native_metal_device_wrapper.hexa` header line 3-5 declares
  D3D11/D3D12 device → Apple D3DMetal binary mapping as the conceptual
  routing — again reference-only; the wrapper itself records device_kind
  / family_tier as TSV rows, with no D3DMetal symbol resolved.

**Reaffirmation statement**: gamebox repo at Track CY landing time
contains:

- 0 bytes of Apple D3DMetal binary code in any source / lib / native
  artifact.
- 0 lines of D3DMetal source code (it is closed-source Apple binary
  redistributable per GPTK; gamebox cannot legally embed it even if it
  wanted to).
- 0 lines of MoltenVK source.
- 0 lines of DXVK / VKD3D-Proton source.
- 0 lines of Wine / CrossOver / Whisky / GPTK source.
- 0 references to Apple private SPI (e.g. private `IOAccelerator` ioctl,
  private Metal compiler internals, private `MTLDeviceSPI`, private
  CAMetalLayer attributes) beyond public Metal / MetalFX / Foundation /
  IOKit / CoreGraphics framework surfaces enumerated in
  `docs/LOADER_NATIVE_HELPER_BRIDGE.md` §1.2.

This reaffirmation matches own1 § A's posture: D3DMetal binary is
**named** as a permitted weak-link target, not embedded; everything
else is third-party and is excluded.

---

## §5 D4 frame translation feasibility — paper-mode analysis

CY produces a **paper-mode** assessment of how much of "D4 frame
translation" is currently expressible. Paper-mode = derivation from
module headers + CV audit + LOADER_NATIVE_HELPER_BRIDGE concepts; no
runtime execution claim.

### §5.1 Per-D4-frame-stage feasibility matrix

| D4 frame stage | D3D12 source method (representative) | Router link kind | Current expressibility (CY paper-mode) |
|---|---|---|---|
| Device acquire | `D3D12CreateDevice` | 1 d3dmetal_delegate | composition-only (rows 2 + 6 + 4 enumerate the phase sequence; 0 real `MTLCreateSystemDefaultDevice()` issued) |
| Command queue create | `ID3D12Device::CreateCommandQueue` | 1 d3dmetal_delegate | composition-only (row 4 phase_kind 3 declares `command_queue_create`; 0 real `[MTLDevice newCommandQueue]` issued) |
| Resource (buffer) create | `ID3D12Device::CreateCommittedResource` (buffer) | 1 d3dmetal_delegate | composition-only (row 3 declares buffer pool; 0 real `[MTLDevice newBufferWithLength:options:]` issued) |
| Resource (texture) create | `ID3D12Device::CreateCommittedResource` (tex2D) | 1 d3dmetal_delegate | composition-only (row 5 declares format taxonomy; 0 real `[MTLDevice newTextureWithDescriptor:]` issued) |
| Descriptor heap | `ID3D12Device::CreateDescriptorHeap` | 1 d3dmetal_delegate | composition-only (CV row 6 + row 5 here for SRV/UAV residency peer; 0 real argument buffer instantiated) |
| Root signature | `ID3D12Device::CreateRootSignature` | 1 d3dmetal_delegate | composition-only (CV row 12; no Metal-side equivalent in 7-link set; argument buffer mapping deferred to RG6 + RG7) |
| PSO create (graphics) | `ID3D12Device::CreateGraphicsPipelineState` | 2 metal_shaderconverter | composition-only (CV row 9 PSO tracker + row 1 dx_d3d12 mapping table; 0 real DXIL→MSL→AIR cross-compile issued; depends on RG3) |
| PSO create (mesh) | `ID3D12Device2::CreatePipelineState` (mesh) | 2 metal_shaderconverter | composition-only (CV row 8 + row 1 here for mesh dispatch); RG3 gates real path |
| Resource barrier | `ID3D12GraphicsCommandList::ResourceBarrier` | 1 d3dmetal_delegate | composition-only (CV row 11 + row 5 here for texture state transitions); 0 real `MTLBlitCommandEncoder` synchronization issued |
| Command list close | `ID3D12GraphicsCommandList::Close` | 1 d3dmetal_delegate | composition-only (CV row 4); 0 real `[MTLCommandBuffer commit]` issued |
| Command queue execute | `ID3D12CommandQueue::ExecuteCommandLists` | 1 d3dmetal_delegate | composition-only (CV row 5 + row 4 here for queue execute); 0 real `[MTLCommandQueue commit]` issued |
| Fence signal | `ID3D12CommandQueue::Signal` | 1 d3dmetal_delegate | composition-only (CV row 7); 0 real `[MTLEvent signalValue:]` issued |
| Swapchain present | `IDXGISwapChain::Present` | 1 d3dmetal_delegate + 4 passthrough | composition-only (CV rows 13+14+15 + row 4 here for present chain); 0 real `[CAMetalLayer nextDrawable]` + `presentDrawable:` issued |
| MetalFX upscale (post-present prep) | (no D3D12 method — synth peer) | N/A (post-router) | composition-only (row 7 declares scaler taxonomy); 0 real `MTLFXSpatialScaler` / `MTLFXTemporalScaler` instantiated |
| DXR build AS | `ID3D12GraphicsCommandList4::BuildRaytracingAccelerationStructure` | 1 d3dmetal_delegate | composition-only (CV row 10 + row 1 here); RG4 gates real `MTLAccelerationStructure` build |
| DXR dispatch rays | `ID3D12GraphicsCommandList4::DispatchRays` | 1+2 (delegate + converter for shader binding table) | composition-only (CV row 10); RG4 gates real `[MTLComputeCommandEncoder dispatchThreads:]` ray dispatch |
| Indirect command (ExecuteIndirect) | `ID3D12GraphicsCommandList::ExecuteIndirect` | 1 d3dmetal_delegate | NOT YET MAPPED (CV §5 open question 3 — `ExecuteIndirect` absent from `dx_d3d12.hexa` 32-entry mapping table); router decision tracker would need extension |

**Paper-mode summary**: 16/17 D4 frame stages have a router link
identified at **composition-only-skeleton** level. 1/17 (`ExecuteIndirect`
indirect dispatch) is unmapped at the router decision tracker level
(open per CV §5.3). 0/17 stages have validated real-tier execution. The
fraction of "D4 frame translation expressible at validated tier" is
**0/17 = 0%** under this repo's own1 path at CY landing time.

### §5.2 Two-gate feasibility statement

Real-tier D4 frame translation is gated on:

- **G1** (= CS G1 = RG1) hexa interp closure CM-26..CM-30 — perpetually
  deferred per Track CN; without G1 closure, the hexa loader cannot
  actually execute any of the 7 router links beyond paper-mode
  description. CY honors this deferral.
- **G2** (= CS G2 = RG2) router validated execution — multi-year
  horizon; depends on G1 + real frame capture infrastructure. CY does
  not propose G2 closure.

**Composite gate**: G1 ∧ G2 must both close before any of the 17 frame
stages above can move from composition-only-skeleton to validated tier.
Even if the composite closes, RG10 + RG11 (Battle.net always-online +
BattlEye) structurally interrupt the chain to **first real D4 frame
in a live session** — these are out-of-scope under own1 retired_intentional
posture and never close in this repo (per CS §2 and §3).

Therefore CY's honest paper-mode assessment: **D4 frame translation
expressibility under this repo's own1 boundary is 0% validated and
~94% composition-only-skeleton (16/17 stages mapped) at CY landing
time, with no path to validated promotion within this repo's scope**.

---

## §6 Open questions

1. **`ExecuteIndirect` router mapping** (carried from CV §5.3 + CS §6.6):
   not yet enumerated in `pe_d3d_to_metal_router.hexa` route_kind table
   nor in `dx_d3d12.hexa` 32-entry mapping table. Should the router
   decision tracker be extended with an indirect dispatch entry, mapping
   to Metal `MTLIndirectCommandBuffer` (ICB)? Open; CY does not propose
   the addition (additive_only doc-tier scope only).
2. **Argument buffer tier-2 mapping coverage**: CV §1 D3D12 surface
   audit included `pe_d3d12_root_signature_real` (row 12) but the 7
   Metal-side modules above do not include a dedicated argument buffer
   handler. `c_argument_buffer_tier2_map` (referenced in
   D3D_TO_METAL_SHADER_MANJEOM_REVIEW R5e) exists outside the 7-link set.
   Should the router inventory be widened to include `c_*` argument
   buffer modules? Open; CY scoped to the `metal|router|d3dmetal`
   pattern only.
3. **Helper IPC kind=7 metal_device runtime closure**: row 4
   `pe_metal_device_weak_link_real` declares phase_kind sequence as a
   **synthetic** trace; the real call would route through
   `native/helper.mm` IPC kind=7 per LOADER_NATIVE_HELPER_BRIDGE §1.2
   row 2. The bridge layer surface is frozen (Track R) but the real
   IPC call has not been made under a D4 frame. Resolution gated on G1
   + RG5.
4. **D3DMetal version ladder (gptk_4_0 / gptk_4_1 / gptk_5_0)**: row 2
   declares 3 GPTK version codes; no honest annotation exists for which
   of these would be the gamebox baseline. Apple GPTK 5.0 (2025-WWDC-era)
   would be the candidate but is not declared anywhere in the 7-link
   set. Open per Apple GPTK release cadence.
5. **MetalFX scaler taxonomy under D4**: row 7 declares scaler_kind 1-3
   enum but no honest annotation maps these to Apple's actual MetalFX
   APIs (`MTLFXSpatialScaler`, `MTLFXTemporalScaler`,
   `MTLFXTemporalDenoisedScaler`). CV §6.1 already noted MetalFX
   eligibility under D4 is unvalidated. Open.
6. **Texture residency vs MTLHeap suballocation**: row 5 declares format
   taxonomy 1-4 (rgba8 / bgra8 / bc7 / astc) but no representation of
   `MTLHeap` suballocation strategy. D4 working set ~7.5-11 GB peak per
   `D2R_D4_COMPARISON.md` §2 — heap suballocation strategy is critical
   but unmapped. Open per CS §6.2 (resolution gated on RG2 + RG7 + G9).
7. **Buffer pool reuse policy**: row 3 declares reuse_count column but
   no policy enumeration (LRU / generational / per-frame-ring). Open;
   resolution path is RG6 (helper IPC bridge to public Metal SDK).
8. **Cross-coverage with `native/helper.mm` IPC kinds**: per
   LOADER_NATIVE_HELPER_BRIDGE §1.1, helper.mm exposes 22 IPC kinds
   (dlopen / mmap / mprotect / pthread_create / dlsym / nsobject /
   metal_device / signal_register / fpu_reg / sse_xmm / user32_call /
   gdi32_call / thread_sync_call / nt_exception_call / vtable_dispatch_call
   / nt_dispatch_chain / cxx_throw_dispatch / sd_forging / ...). Of
   these, only kind=7 metal_device is referenced from the 7-link router
   set (via row 4). Should the router inventory cross-reference more
   helper IPC kinds? Open; CY scoped to direct Metal/D3DMetal-tagged
   modules only.
9. **`_real` filename rename** (carried from CV §5.1): row 4
   `pe_metal_device_weak_link_real.hexa` carries the misleading suffix.
   CY preserves the filename (additive_only) but reaffirms it is
   naming-only; no validated execution implied.
10. **Shader converter (DXIL → MSL → AIR) module location**: route_kind
    code 2 (`metal_shaderconverter`) is declared in row 1 but no
    dedicated Metal-side module in the 7-link set implements it. R5j
    candidate (`c_dxil_persist` D1, `c_dxil_to_air_fusion` R2) lives
    outside the `metal|router|d3dmetal` pattern. Should the router
    inventory be widened to include the shader converter family? Open;
    CY scoped to the search pattern only.

---

## §7 Predecessors

- Track CX — `state/markers/<CX marker>.marker` (alphabetic predecessor;
  parallel sibling with CW; landed before CY)
- Track CW — `state/markers/<CW marker>.marker` (alphabetic predecessor;
  parallel sibling with CX; landed before CY)
- Track CV — `state/markers/d4_d3d12_surface_honesty_audit_track_cv_landed.marker`
  (semantic α-phase predecessor; D3D12 surface honesty audit; CY is the
  β complement on the router side)
- Track CU — `state/markers/<CU marker>.marker` (alphabetic predecessor)
- Track CT — `state/markers/<CT marker>.marker` (alphabetic predecessor)
- Track CS — `state/markers/d4_roadmap_track_cs_landed.marker`
  (semantic predecessor; phase D4-CS-β exit criteria source)
- Track CR — `state/markers/gamebox_d2r_roadmap_track_cr_landed.marker`
  (sibling roadmap family member; D2R contrast)
- Track CQ —
  `state/markers/gamebox_battlenet_roadmap_track_cq_landed.marker`
  (sibling roadmap family member; Battle.net cross-dep for RG10)
- Track CN —
  `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker`
  (G1 / RG1 perpetual deferral source — gates G2 / RG2 — gates all 7
  router links)
- Track CL —
  `state/markers/gamebox_eac_kernel_honest_freeze_track_cl_landed.marker`
  (anti-cheat freeze cross-class precedent; RG11 BattlEye coverage)
- Track CK —
  `state/markers/gamebox_vac_honest_freeze_track_ck_landed.marker`
  (anti-cheat freeze cross-class precedent)
- `docs/D4_D3D12_SURFACE_HONESTY_AUDIT_TRACK_CV.ai.md` (commit 7513be9;
  α-phase audit; CY is β complement)
- `docs/D4_ROADMAP_TRACK_CS.ai.md` (commit 41ddb75; phase D4-CS-β source)
- `docs/D4_MANJEOM_REVIEW.md` (mk1 narrative; `retired_intentional`
  source)
- `docs/LOADER_NATIVE_HELPER_BRIDGE.md` (Track R spec freeze;
  native/helper.mm IPC bridge concept; row 4 + row 8 cross-references)
- `docs/D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md` (R5 honest review; CS §4.2
  declared input)

---

## §8 In-place writes

| File | Op |
|---|---|
| (none) | — |

`in_place_writes`: 0 (cleanest doc-only inventory).
NEW files: 2 (this handoff doc + Track CY marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/
/ 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0
CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/ / 0
existing gamebox/doc/ / 0 existing docs/*.md mutation / 0 predecessor
marker mutation / 0 gamebox-native marker mutation.

---

## §9 Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (2 NEW files only)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (no user directive verbatim quoted)
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no DRM bypass / no anti-cheat bypass /
  no always-online evasion / **no Apple D3DMetal embed** (§4 explicit
  reaffirmation) / no anima clone / no Apple private SPI direct use
  beyond public Metal/MetalFX/Foundation/IOKit/CoreGraphics ✓
- **own2**: forensic_verdict honest unchanged; per-router-link honest
  tier explicit; `_real` naming-only annotation explicit on row 4;
  validated_manjeom=0 column explicit on all 7 rows; no progress
  claims; no fake validated promotion; no first-frame promise ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓

---

## §10 Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CY is the per-router-link honest annotation execution of phase D4-CS-β
from Track CS. NO module count change / NO state mutation / NO
validated_manjeom promotion / NO real D4 frame claim / NO bypass path /
NO D3DMetal embed / NO Apple private SPI use. The D3D12-to-Metal router
stays composition-only at skeleton level across all 7 Metal-side links.
Always-online retired_intentional posture (D4 G6 / RG10) carries via
Track CS. Anti-cheat (D4 G7 / RG11 BattlEye) carries via cross-class
freeze docs (CL/CK). Real-tier validation perpetually deferred behind
hexa interp closure (G1 / RG1) and structurally interrupted by
always-online + anti-cheat (RG10 + RG11).

---

## §11 Supersedes

- (does NOT supersede) `docs/D4_ROADMAP_TRACK_CS.ai.md` — CS remains
  authoritative roadmap; CY is the doc-tier execution of CS §4.2 phase
  D4-CS-β only.
- (does NOT supersede) `docs/D4_D3D12_SURFACE_HONESTY_AUDIT_TRACK_CV.ai.md`
  — CV remains the α-phase D3D12 surface audit; CY is the β complement
  (router-side, not surface-side).
- (does NOT supersede) `docs/D4_MANJEOM_REVIEW.md` — that mk1 narrative
  remains the authoritative `retired_intentional` adoption record.
- (does NOT supersede) `docs/LOADER_NATIVE_HELPER_BRIDGE.md` — that
  Track R spec freeze remains the authoritative bridge-layer surface
  spec; CY only cross-references it.
- (does NOT supersede) `docs/D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md` —
  that R5 review remains the authoritative shader-side analysis; CY
  cross-references R5j candidates only.
- (informally extends) Track CV §3 router-composition note by enumerating
  the 7 Metal-side router links explicitly with per-link tier and gap
  list mapped to RG1..RG11.

---

## §12 Next-cycle hooks (NOT triggered in CY)

- Phase D4-CS-γ candidate: D4 always-online honest freeze doc (sibling
  to EAC + VAC freezes; consumes CY §4 no-D3DMetal-embed reaffirmation
  as input)
- Phase D4-CS-δ candidate: 27 c_d4_* perf cluster honest annotation doc
- (perpetually deferred per CN): G1 / RG1 hexa interp closure
  CM-26..CM-30 → unblocks RG2..RG9 → unblocks router validated promotion
- (out-of-scope under own1): RG10 Battle.net always-online entry / RG11
  BattlEye live anti-cheat surface
- (deferred): `ExecuteIndirect` router mapping addition (open question §6.1)
- (deferred): argument buffer / shader converter module family widening
  (open questions §6.2 + §6.10)
- (deferred): MetalFX scaler taxonomy honest mapping (open question §6.5)
- (deferred): MTLHeap suballocation strategy (open question §6.6)
