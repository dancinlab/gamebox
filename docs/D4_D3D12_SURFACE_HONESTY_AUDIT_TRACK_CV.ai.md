# D4 D3D12 Surface Honesty Audit — Track CV Landed (2026-05-05)

> Track CV on gamebox standalone repo, branch `main`. Doc-only execution of
> phase D4-CS-α (D3D12 surface honesty audit) from the Track CS D4 forward
> roadmap. Per-module honest tier annotation across the 15 D3D12-family
> modules touched by D4. Honest posture: every module skeleton-tier;
> `_real` filename suffix is naming-only and does NOT denote validated
> execution; `validated_manjeom = 0` carries across the board; D3D12-to-Metal
> router stays composition-only at skeleton level. CV lands strictly after
> sibling cycles CT and CU to preserve CT → CU → CV ordering.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CU 2026-05-05).
- Direct predecessor (alphabetic): Track CU.
- Direct predecessor (cross-dep): Track CT.
- Semantic predecessor: Track CS (`docs/D4_ROADMAP_TRACK_CS.ai.md`, commit
  41ddb75) — phase D4-CS-α exit criteria source.
- Phase executed: **D4-CS-α** per Track CS §4.1 — "1 audit doc landed; each
  module annotated skeleton/stage1/stage2; validated_manjeom column = 0
  across the board".
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- Documentation tier — no code emission, no module addition, no
  validated_manjeom promotion. Per-module honest tier annotation only.

---

## Mission

CV1 — Honest per-module audit across the 15 D3D12-family modules listed in
Track CS §1.3:

- 13 `dx_d3d12*` + `pe_d3d12_*` modules (per CS roadmap line "13 D3D12
  family modules")
- plus `pe_dxgi_swapchain` (DXGI surface, included in CS §1.3 table)
- plus `pe_swapchain_present_track` (Metal-side present timing tracker,
  included in CS §1.3 table because it is the gamebox half of the
  D3D12-Present → CAMetalLayer-present chain)

For each module: file path, honest tier, `_real` naming-only annotation
(where applicable), `validated_manjeom` status, real D3D12 stream
observation evidence, reference relationship to `lib/loader/d4_launch.hexa`
and `lib/loader/pe_d4_specific_track.hexa`.

CV2 — handoff doc + marker (this file + Track CV marker).

---

## §1 Audit table — per-module honest tier

Legend:
- **Tier** — `skeleton` / `stage1` / `stage2` / `composition` (for the
  D3D12-to-Metal router conceptual aggregate).
- **`_real` naming-only** — `Y` if the filename ends with `_real` and the
  module is nonetheless skeleton (i.e. naming convention only, not
  validated execution); `—` if the filename has no `_real` suffix.
- **vM** — `validated_manjeom` value carried (must be 0 across the board
  per Track CS §1.3 and forensic verdict).
- **Real-stream evidence** — what real D3D12 command stream from a real D4
  frame this module has observed. Must be `none` (synthetic-only) for
  every row.
- **d4_launch ref** — referenced by name from `lib/loader/d4_launch.hexa`?
  (`Y` / `—` / `comment-only`).
- **pe_d4 ref** — referenced by name from
  `lib/loader/pe_d4_specific_track.hexa`? (`Y` / `—` / `comment-only`).

| # | Module | Path | Tier | `_real` naming-only | vM | Real-stream evidence | d4_launch ref | pe_d4 ref |
|---|---|---|---|---|---|---|---|---|
| 1 | dx_d3d12 | `lib/loader/dx_d3d12.hexa` | skeleton (mapping table; all 32 entries `status="skeleton"`) | — | 0 | none (32-entry name → Metal-equiv mapping table only; no MTLDevice alloc, no MTLAccelerationStructure build, no CAMetalLayer binding — module header lines 27-30) | Y (added to phase deps; phase_deps panic guard checks `dx_d3d12` present — d4_launch lines 73, 200-206) | comment-only |
| 2 | dx_d3d12_stage1 | `lib/loader/dx_d3d12_stage1.hexa` | stage1 (synthetic handle simulation; not real D3DMetal call) | — | 0 | none (synthetic handle table only — header lines 24-26: "*synthetic handle table* 으로 D3D12 + DXR 동작만 검증") | — | — |
| 3 | dx_d3d12_stage2 | `lib/loader/dx_d3d12_stage2.hexa` | stage2 (synthetic DXR command execution recording on top of stage1 handles) | — | 0 | none (synthetic execution recording — header lines 28-31: "실제 DXR commands 단계 — synthetic execution recording") | — | — |
| 4 | pe_d3d12_command_list | `lib/loader/pe_d3d12_command_list.hexa` | skeleton (CommandList op tracker; "실 ID3D12CommandList 호출 X (stage6)") | — | 0 | none (TSV row tracker; no real ID3D12CommandList instance — header line 8) | — | — |
| 5 | pe_d3d12_command_queue_real | `lib/loader/pe_d3d12_command_queue_real.hexa` | skeleton | Y (header line 9 declares "실 ID3D12CommandQueue 호출 X (D3DMetal 위임 stage)") | 0 | none (op-id tracker; no real ID3D12CommandQueue instance) | — | — |
| 6 | pe_d3d12_descriptor_heap_real | `lib/loader/pe_d3d12_descriptor_heap_real.hexa` | skeleton | Y (no real ID3D12DescriptorHeap call; tracker only) | 0 | none (heap_id/heap_kind/descriptor_count/shader_visible tracker only) | — | — |
| 7 | pe_d3d12_fence_real | `lib/loader/pe_d3d12_fence_real.hexa` | skeleton | Y (header line 13 declares "실 ID3D12Fence 호출 X (D3DMetal 위임 stage)") | 0 | none (op-id tracker for Signal/Wait/SetEventOnCompletion; no real MTLEvent / MTLSharedEvent created) | — | — |
| 8 | pe_d3d12_mesh_shader | `lib/loader/pe_d3d12_mesh_shader.hexa` | skeleton (DispatchMesh tracker; "실 ID3D12GraphicsCommandList6 호출 X (stage6)") | — | 0 | none (synthetic dispatch_id / shader_kind / thread_groups / primitive_count rows only — header line 8) | — | — |
| 9 | pe_d3d12_pipeline_state | `lib/loader/pe_d3d12_pipeline_state.hexa` | skeleton (PSO tracker; "실 ID3D12Device 호출 X (D3DMetal 위임 stage)") | — | 0 | none (pso_id / pso_kind / shader_count / compile_us tracker only — header line 13) | — | comment-only (header line 12 cross-refs "R5 D3D12 phase 2 (cycle 81 pe_d3d12_pipeline_state) 강결합" but no import / use binding) |
| 10 | pe_d3d12_raytracing | `lib/loader/pe_d3d12_raytracing.hexa` | skeleton (DXR op tracker; "실 DispatchRays / BuildRaytracingAccelerationStructure 호출 X") | — | 0 | none (BLAS/TLAS build, AS update/copy, DispatchRays op_id tracker — header lines 7-9) | — | — |
| 11 | pe_d3d12_resource_barrier_real | `lib/loader/pe_d3d12_resource_barrier_real.hexa` | skeleton | Y (header line 12 declares "실 ID3D12GraphicsCommandList 호출 X (D3DMetal 위임 stage)") | 0 | none (barrier_id / barrier_kind / before_state / after_state tracker only) | — | — |
| 12 | pe_d3d12_root_signature_real | `lib/loader/pe_d3d12_root_signature_real.hexa` | skeleton | Y (header line 15 declares "실 ID3D12RootSignature 호출 X (D3DMetal 위임 stage)") | 0 | none (signature_id / param_kind / register_space / descriptor_count tracker only) | — | — |
| 13 | pe_d3d12_swapchain_present_real | `lib/loader/pe_d3d12_swapchain_present_real.hexa` | skeleton | Y (header line 9 declares "실 IDXGISwapChain 호출 X (D3DMetal 위임 stage)") | 0 | none (present_id / sync_interval / flags / actual_us tracker only; no CAMetalLayer nextDrawable call) | — | — |
| 14 | pe_dxgi_swapchain | `lib/loader/pe_dxgi_swapchain.hexa` | skeleton (DXGI op tracker; "IDXGISwapChain 인스턴스 생성 / Present 호출 X") | — | 0 | none (op_id / op_kind / sync_interval / present_flags tracker only — header line 8) | — | — |
| 15 | pe_swapchain_present_track | `lib/loader/pe_swapchain_present_track.hexa` | skeleton (Metal-side present timing tracker; "실 Metal API 호출 X") | — | 0 | none (present_id / target_us / actual_us / vsync row tracker only; no CVDisplayLink / CAMetalDrawable hook — header lines 5-9) | — | — |

**Row count**: 15 modules audited. Tier breakdown: 13 skeleton + 1 stage1 +
1 stage2. Real-stream observation evidence: `none` across all 15 (every
module is a TSV-row tracker or a synthetic handle/execution recorder; no
real D3D12 frame from a real D4 process has been observed under this
repo's own1 path). `_real` naming-only = `Y` for 7 of 15 modules (5/6/7/11/
12/13 + the cross-stage 5: command_queue_real, descriptor_heap_real,
fence_real, resource_barrier_real, root_signature_real, swapchain_present_real
— and counting the `_real` suffix on filename only).

---

## §2 Aggregate posture — validated_manjeom = 0 across the board

Per Track CS §1.3 and the forensic verdict
`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`:

- All 15 D3D12-family modules carry `validated_manjeom = 0`.
- Surface coverage 100% (Microsoft D3D12 / DXGI / DXR API shape captured
  in TSV schemas, op-kind enums, state machines).
- Validated execution 0 — no real D3D12 command graph has been executed
  end-to-end against a real Metal device under a real D4 frame in this
  repo's own1 path.
- `_real` filename suffix on 6 modules (`pe_d3d12_command_queue_real`,
  `_descriptor_heap_real`, `_fence_real`, `_resource_barrier_real`,
  `_root_signature_real`, `_swapchain_present_real`) is **naming
  convention only** — each module's own header explicitly declares
  "실 호출 X (D3DMetal 위임 stage)" or equivalent. CV reaffirms this.
- The two stages on `dx_d3d12` family (stage1 = handle simulation,
  stage2 = DXR command recording on top of stage1) are honest tier
  promotions over the base `dx_d3d12.hexa` mapping table, but neither
  reaches validated execution. Both remain pre-validated tiers.

**Aggregate honest posture statement**: 15 D3D12-family modules form a
100% Win32-D3D12-shape skeleton + DXR stage1/stage2 synthetic recorders
on top, with zero real D3D12 stream observed and zero real Metal device
call issued under own1.

---

## §3 D3D12-to-Metal router note — composition-only

Per Track CS §1.5: the D3D12-to-Metal router is **not landed as a single
module**. It is a composition of:

- the 13 `dx_d3d12*` + `pe_d3d12_*` modules audited in §1 (rows 1-13),
- `pe_dxgi_swapchain` for the DXGI half of the swapchain bridge (row 14),
- `pe_swapchain_present_track` for the Metal-side present timing
  observation (row 15),
- `docs/LOADER_NATIVE_HELPER_BRIDGE.md` (concept; not embedded code),
- external Apple D3DMetal redist (reference-only in module comments;
  own1 explicitly forbids embedding — `dx_d3d12.hexa` line 4 enforces
  "Wine / CrossOver / dxvk / vkd3d / community 코드 0 줄").

CV does not change this composition. The router stays **composition-only
at skeleton level**. No new module is added in CV. No module is promoted
from skeleton to validated. The composition's overall honest tier remains:

> Composition-only-at-skeleton-level — every constituent piece is
> skeleton (or stage1/stage2 synthetic), and the composition's
> end-to-end validated execution against a real D4 frame is gated on
> G2 per Track CS §3 (multi-year horizon, depends on G1 hexa interp
> closure first).

---

## §4 Real-frame gap — G2 from Track CS roadmap

Per Track CS §3 gap inventory:

- **G2 — D3D12-to-Metal router validated execution**: skeleton →
  validated promotion. Multi-year. Depends on G1 (hexa interp closure
  CM-26..CM-30) + real frame capture infrastructure. CV confirms G2
  unresolved at the per-module level — every audited module is gated by
  the same G2 + G1 chain.
- **G1 — hexa interp closure CM-26..CM-30**: per Track CN, perpetually
  deferred. CV honors this. No CV phase exit is "first real D4 frame"
  or "real Metal device call from a D3D12 module".
- **G3 — mesh shader real path** (`pe_d3d12_mesh_shader` row 8):
  unresolved; depends on G2 + Apple Metal mesh shader maturity per
  device.
- **G4 — raytracing real path** (`pe_d3d12_raytracing` row 10 +
  `dx_d3d12_stage2` row 3): unresolved; depends on G2 + per-device DXR
  tier confirmation. Note `d4_launch::dxr_supported()` returns coarse
  arm64 + macOS≥14 boolean only — does not validate real DXR command
  submission.

**No CV phase resolves G1, G2, G3, G4, or any other gap from CS §3.** CV
is documentation-tier only. It enumerates the 15 modules' honest tier
state and reaffirms the gap.

---

## §5 Open questions

1. **Should `_real` filename suffix be renamed to drop the misleading
   convention?** CV preserves filenames (additive_only, no rename) but
   notes that 6 files carry the suffix as naming-only. Future cycle could
   propose a rename pass; CV does not propose this now (out of additive_only
   scope).
2. **Should `dx_d3d12_stage{1,2}` be re-tiered to "synthetic-stage" rather
   than `stage1`/`stage2`?** The current `stage1`/`stage2` nomenclature
   accurately describes pipeline progression but does not explicitly
   convey "still synthetic". CV uses both column annotations to capture
   the nuance.
3. **D3D12 ExecuteIndirect → Metal ICB mapping under D4** (carried from
   CS §6.6): not separately covered by any of the 15 audited modules.
   `dx_d3d12.hexa` mapping table (32 entries) does not include
   `ExecuteIndirect` per its own enumeration (Device 8 + Resource 6 +
   CommandList 10 + DXR 5 + SwapChain 3 = 32; ExecuteIndirect absent).
   Open per CS §6.6.
4. **Should there be a per-module `validated_manjeom` marker**, or is the
   single aggregate 0-across-the-board sufficient? CV uses the aggregate
   form (one column in §1 table). Per-module marker proliferation deferred.
5. **DXGI tearing flag (`ALLOW_TEARING = 0x200`) honest path under
   CAMetalLayer**: `pe_d3d12_swapchain_present_real` row 13 enumerates the
   flag in its header (line 21) but does not validate the real
   `presentDrawable:afterMinimumDuration:0` Metal call. Open per G8.
6. **`pe_swapchain_present_track` (row 15) Metal-side coverage**: the
   tracker shape (target_us / actual_us / vsync) is correct for frame
   pacing analysis, but no real CVDisplayLink / CAMetalDrawable hook
   exists. Open per G10 frame-pacing-under-DXR-mixed-workload.
7. **Cross-coverage with `pe_d4_specific_track.hexa` `dx12_device_init`
   phase_kind**: pe_d4 §1.2 (CS roadmap) declares synthetic shader
   counts for dx12_device_init and pso_warm phases; none of the 15
   D3D12-family modules are imported into pe_d4 (pe_d4 self_test uses
   inline synthetic step records 820001..820008). Open per "should pe_d4
   import any of the 15 audited modules" question.

---

## §6 Predecessors

- Track CU — `state/markers/<CU marker>.marker` (alphabetic
  predecessor; CT+CU sibling pair preceded CV per Track CS roadmap
  ordering)
- Track CT — `state/markers/<CT marker>.marker` (alphabetic +
  cross-dep predecessor)
- Track CS — `state/markers/d4_roadmap_track_cs_landed.marker` (semantic
  predecessor; phase D4-CS-α exit criteria source)
- Track CR — `state/markers/gamebox_d2r_roadmap_track_cr_landed.marker`
  (sibling cycle from same roadmap family; D2R contrast)
- Track CQ —
  `state/markers/gamebox_battlenet_roadmap_track_cq_landed.marker`
  (sibling cycle from same roadmap family; Battle.net cross-dep)
- Track CN —
  `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker`
  (G1 perpetual deferral — gates G2 which gates the audited modules'
  promotion)
- Track CL —
  `state/markers/gamebox_eac_kernel_honest_freeze_track_cl_landed.marker`
  (anti-cheat freeze; cross-class precedent for D4 BattlEye G7)
- Track CK —
  `state/markers/gamebox_vac_honest_freeze_track_ck_landed.marker`
  (anti-cheat freeze cross-class precedent)
- `docs/D4_ROADMAP_TRACK_CS.ai.md` (commit 41ddb75; phase D4-CS-α source)
- `docs/D4_MANJEOM_REVIEW.md` (mk1 narrative; `retired_intentional`
  source)

---

## §7 In-place writes

| File | Op |
|---|---|
| (none) | — |

`in_place_writes`: 0 (cleanest doc-only audit).
NEW files: 2 (this handoff doc + Track CV marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/
/ 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0
CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/ / 0
existing gamebox/doc/ / 0 existing docs/*.md mutation / 0 predecessor
marker mutation / 0 gamebox-native marker mutation.

---

## §8 Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (2 NEW files only)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (no user directive verbatim quoted)
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no DRM bypass / no anti-cheat bypass /
  no always-online evasion / no Apple D3DMetal embed / no anima clone ✓
- **own2**: forensic_verdict honest unchanged; per-module honest tier
  explicit; `_real` naming-only annotation explicit on 6 modules;
  validated_manjeom=0 column explicit on all 15 rows; no progress
  claims; no fake validated promotion; no first-frame promise ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓

---

## §9 Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CV is the per-module honest annotation execution of phase D4-CS-α from
Track CS. NO module count change / NO state mutation / NO
validated_manjeom promotion / NO real D4 frame claim / NO bypass path.
The D3D12-to-Metal router stays composition-only at skeleton level.
Always-online retired_intentional posture (D4 G6) carries via Track CS.
Anti-cheat (D4 G7 BattlEye) carries via cross-class freeze docs (CL/CK).
Real-tier validation perpetually deferred behind hexa interp closure
(G1) and structurally interrupted by always-online + anti-cheat (G6 +
G7).

---

## §10 Supersedes

- (does NOT supersede) `docs/D4_ROADMAP_TRACK_CS.ai.md` — CS remains
  authoritative roadmap; CV is the doc-tier execution of CS §4.1
  phase D4-CS-α only.
- (does NOT supersede) `docs/D4_MANJEOM_REVIEW.md` — that mk1 narrative
  remains the authoritative `retired_intentional` adoption record.
- (does NOT supersede) `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` — that
  6-phase model schema retained.
- (informally extends) Track CS §1.3 D3D12 module family table by
  adding per-module `_real` naming-only annotation, real-stream
  evidence column, and d4_launch / pe_d4 reference-relationship column.

---

## §11 Next-cycle hooks (NOT triggered in CV)

- Phase D4-CS-β candidate: D3D12-to-Metal router skeleton inventory doc
  (consumes CV §3 composition note as input)
- Phase D4-CS-γ candidate: D4 always-online honest freeze doc (sibling
  to EAC + VAC freezes)
- Phase D4-CS-δ candidate: 27 c_d4_* perf cluster honest annotation doc
- (perpetually deferred per CN): G1 hexa interp closure CM-26..CM-30 →
  unblocks G5 + G2 → unblocks audited modules' promotion path
- (out-of-scope under own1): G6 Battle.net always-online entry / G7
  BattlEye live anti-cheat surface
- (deferred): `_real` filename rename pass (open question §5.1)
- (deferred): per-module `validated_manjeom` marker proliferation (open
  question §5.4)
