# D4 (Diablo IV) Forward Roadmap — Track CS Landed (2026-05-05)

> Track CS on gamebox standalone repo, branch `main`. Doc-only forward-looking
> D4 roadmap that supersedes prior D4 plans. Honest posture: D3D12-to-Metal
> router skeleton-tier on darwin/arm64; always-online Battle.net hard gate; hexa
> interp closure CM-26..CM-30 still blocks runtime launch.
> Sibling cycles in same alphabetic Track family: CQ (battle.net forward
> roadmap) and CR (D2R forward roadmap). CS lands strictly after CQ + CR to
> preserve CQ → CR → CS ordering.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CR D2R forward roadmap 2026-05-05).
- Predecessor (immediate): Track CR (D2R forward roadmap).
- Predecessor (cross-dep): Track CQ (battle.net forward roadmap).
- Predecessor (semantic): `docs/D4_MANJEOM_REVIEW.md` 2026-05-03 (retired_intentional adoption mk1 narrative); `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` 2026-05-01 (6-phase model); `docs/D2R_D4_COMPARISON.md` 2026-04-30 (perf comparison estimate).
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- Documentation tier — no code emission, no module addition, no validated_manjeom promotion. Supersedes any prior aspirational D4 plan that did not honestly account for the always-online hard gate or the D3D12-to-Metal router skeleton state.

---

## Mission

CS1 — Honest forward D4 roadmap doc covering:

- Current honest posture of d4_launch + pe_d4_specific_track + 9 D3D12 module families + swapchain present + DX12 perf telemetry surface
- Always-online posture (Battle.net hard gate; no offline path; retired_intentional honest stance carries)
- Gap inventory blocking first real D4 frame on darwin/arm64
- Phased roadmap (3-5 phases) with scope, inputs, exit criteria, honest tier, invariant impact per phase
- Cross-dependencies on Track CQ (Battle.net) and contrast with Track CR (D2R)
- Open questions

CS2 — handoff doc + marker (this file + Track CS marker).

---

## §1 Current honest posture

### §1.1 d4_launch.hexa surface

- File: `lib/loader/d4_launch.hexa` (~334 LOC).
- Emits: 3 r0 events (`d4_info`, `d4_dxr_check`, `d4_phase_deps`).
- Static description only — no D4.exe spawn, no Battle.net auth tunnel, no BattlEye bypass.
- `d4_info()` declares `name=Diablo IV`, `dx_path=DX12+DXR`, `platform=x86_64`, `dll_deps=KERNEL32+USER32+D3D12+DXGI+WS2_32+CRYPT32+anti_cheat?`.
- `d4_phase_deps()` enumerates 10 prerequisite phase modules (pe_parser → pe_loader → pe_relocations → pe_imports → win32_kernel32 → win32_user32 → win32_ntdll → win32_advapi32 → dx_d3d12 → win32_winsock).
- `d4_dxr_check()` inline-replicates Apple Metal raytracing arch+macOS gate (arm64 + macOS ≥ 14). Returns bool only. No router probe, no real DXR command emission.
- `d4_auth_blockers()` declares Battle.net online auth + BattlEye out of scope.
- `validate_d4_path()` is shell-only (test -f / head -c 2 / strings | grep d3d12.dll) — no real PE parse, no real IAT walk.
- **Honest tier**: skeleton-tier (description + dependency graph + DXR availability gate; no real launch surface).

### §1.2 pe_d4_specific_track.hexa surface

- File: `lib/loader/pe_d4_specific_track.hexa` (~436 LOC).
- 8 D4-specific phase_kind codes: process_attach / fenris_init / cas_storage_load / bnet_token_validate / dx12_device_init / pso_warm / ui_layer / first_frame.
- TSV persistence schema (`step_id`, `phase_kind`, `shader_count`, `elevated`).
- self_test exercises all 8 phase_kinds with synthetic step_ids 820001..820008 and synthetic shader counts (0/0/0/0/64/1024/32/16 = 1136 total).
- Emits ≥9 r0 events (8 record events + 1 stats event).
- **Honest tier**: skeleton-tier — synthetic step records, no observation of a real D4 process, no real shader count, no real bnet token validation.

### §1.3 D3D12 module family surface (D4 renderer dependencies)

| Module | Path | State |
|---|---|---|
| dx_d3d12 (umbrella) | `lib/loader/dx_d3d12.hexa` | skeleton-tier surface |
| dx_d3d12_stage1 | `lib/loader/dx_d3d12_stage1.hexa` | skeleton-tier |
| dx_d3d12_stage2 | `lib/loader/dx_d3d12_stage2.hexa` | skeleton-tier (DXR feature flagging) |
| pe_d3d12_command_list | `lib/loader/pe_d3d12_command_list.hexa` | skeleton-tier |
| pe_d3d12_command_queue_real | `lib/loader/pe_d3d12_command_queue_real.hexa` | skeleton-tier (name-only `_real`) |
| pe_d3d12_descriptor_heap_real | `lib/loader/pe_d3d12_descriptor_heap_real.hexa` | skeleton-tier |
| pe_d3d12_fence_real | `lib/loader/pe_d3d12_fence_real.hexa` | skeleton-tier |
| pe_d3d12_resource_barrier_real | `lib/loader/pe_d3d12_resource_barrier_real.hexa` | skeleton-tier |
| pe_d3d12_root_signature_real | `lib/loader/pe_d3d12_root_signature_real.hexa` | skeleton-tier |
| pe_d3d12_pipeline_state | `lib/loader/pe_d3d12_pipeline_state.hexa` | skeleton-tier |
| pe_d3d12_swapchain_present_real | `lib/loader/pe_d3d12_swapchain_present_real.hexa` | skeleton-tier |
| pe_d3d12_mesh_shader | `lib/loader/pe_d3d12_mesh_shader.hexa` | skeleton-tier |
| pe_d3d12_raytracing | `lib/loader/pe_d3d12_raytracing.hexa` | skeleton-tier |
| pe_dxgi_swapchain | `lib/loader/pe_dxgi_swapchain.hexa` | skeleton-tier |
| pe_swapchain_present_track | `lib/loader/pe_swapchain_present_track.hexa` | skeleton-tier |

`_real` suffix in filenames denotes naming-convention only (not validated execution). All entries above carry the BL/BM forensic skeleton verdict: surface coverage 100% (Win32 D3D12 API shape captured), validated_manjeom = 0 (no real D3D12 command graph executed end-to-end against a Metal device under a real D4 frame).

### §1.4 D4 perf telemetry surface

| Module | Relevance to D4 |
|---|---|
| `lib/loader/pe_perf_first_frame_breakdown.hexa` | D4 P4 (first frame) telemetry shape |
| `lib/loader/pe_perf_frametime_histogram.hexa` | D4 P5 (gameplay) frametime distribution |
| `lib/loader/pe_perf_gpu_utilization_real.hexa` | D4 GPU residency / occupancy estimate (skeleton; `_real` is naming only) |
| `lib/loader/pe_perf_metalfx_upscale.hexa` | D4 MetalFX upscale eligibility check (skeleton; no real upscale path validated) |
| `lib/loader/pe_perf_session_aggregator.hexa` | D4 30-min session aggregation shape |
| 27 `c_d4_*` perf cluster modules | D4 P5/P6 economy + endgame skeleton — synthetic only per `D4_MANJEOM_REVIEW.md` §4 |

### §1.5 D3D12-to-Metal router state

The conceptual D3D12-to-Metal router (would translate D3D12 commands into Metal commands on darwin/arm64) is **not landed as a single module in this repo**. It is a composition of:

- `pe_d3d12_*` modules (Win32 surface descriptors, skeleton)
- `dx_d3d12_stage{1,2}` (umbrella feature toggles, skeleton)
- Native helper bridge concept per `docs/LOADER_NATIVE_HELPER_BRIDGE.md`
- External Apple `D3DMetal` (referenced in comments only — own1 = no Wine / no GPTK / no D3DMetal embedding; reference-only inclusion in docs)

**Router honest tier**: composition-only at skeleton level. No D3D12 command stream from a real D4 frame has been observed, translated, and submitted to a real Metal device under this repo's own1 path. `validated_manjeom = 0` carries.

---

## §2 Always-online posture

### §2.1 Hard gate restated

D4 requires Battle.net authentication for **every session including all single-player content**. There is no `start_in_offline_mode.exe` analogue, no `start_game_in_offline_mode.exe` analogue. Every D4 launch attempt issues a server-side validation handshake that, on failure, terminates the session before the first frame.

### §2.2 retired_intentional posture (carried forward from `D4_MANJEOM_REVIEW.md`)

Per the predecessor mk1 narrative (2026-05-03), `.roadmap.diablo4` blk.1 carries `status_enum_adopted: retired_intentional` and `retired_intentional_posture: block_acknowledged_pass_equivalent`. CS does not amend this. The retired_intentional + sideways-PASS semantic (block-acknowledged honest documentation = PASS-equivalent posture) governs the offline-launch question: not pursued, not bypassed, honestly documented as out-of-scope under own1.

### §2.3 No bypass roadmap

This roadmap explicitly does **not** include any phase whose exit criterion is "Battle.net auth bypass" / "OAuth token forge" / "Agent SD process spoof" / "server-side validation bypass" / "BattlEye bypass". Such a phase would violate own1 (no DRM evasion / no anti-cheat bypass) and the IDENTITY.md spirit. Community-reported workarounds are noted in `D4_MANJEOM_REVIEW.md` §2.2 only as out-of-scope reference; this CS doc carries the same boundary.

### §2.4 Account-safety boundary

D4 Battle.net account ban is permanent. gamebox's value to a developer or researcher is reading the static D3D12 surface of a legitimately-installed `Diablo IV.exe`, not entering a live always-online session via any non-Blizzard-sanctioned path. CS preserves this boundary.

---

## §3 Gap inventory — what blocks first real D4 frame on darwin/arm64

| Gap | Class | Resolution path under own1 |
|---|---|---|
| G1 hexa interp closure CM-26..CM-30 | runtime-launch blocker | Sibling repo `hexa-lang` work; perpetually deferred per Track CN |
| G2 D3D12-to-Metal router validated execution | skeleton → validated promotion | Multi-year; depends on G1 + real frame capture infrastructure |
| G3 Mesh shader (`pe_d3d12_mesh_shader`) real path | skeleton → stage1+ | Apple Metal mesh shader maturity + G2 |
| G4 Raytracing (`pe_d3d12_raytracing`) real path | skeleton → stage1+ | Apple Metal raytracing tier confirmation per device + G2 |
| G5 PE32+ x86_64 D4.exe load + IAT resolve runtime | Phase 1-class | kernel32 stage4 strengthening per `D2R_D4_INSTALL_LAUNCH_ROADMAP.md` §2 P1 |
| G6 Battle.net Agent SD invocation under own1 | always-online | Out-of-scope under retired_intentional posture; sibling Track CQ provides only honest-spec path |
| G7 BattlEye anti-cheat surface | anti-cheat class | `docs/EAC_KERNEL_HONEST_FREEZE.md` predecessor freeze applies (cross-class); honest spec only |
| G8 MetalFX upscale eligibility under D4 frame | perf path | Skeleton; resolution after G2 |
| G9 GPU residency / heap behavior under D4 working set | perf path | Skeleton; resolution after G2 |
| G10 Frame pacing under D4 always-online + DXR mixed workload | perf path | Skeleton; resolution after G2 |

**Critical chain**: G1 → G5 → G2 → {G3, G4, G8, G9, G10}. G6 + G7 are out-of-scope under own1 and never resolve in this repo. Therefore the chain to first real D4 frame is **structurally interrupted at G6/G7** even if G1..G5 + G2..G4 land.

This is the honest reality and the reason `D4_MANJEOM_REVIEW.md` adopted `retired_intentional`. CS does not propose to undo that adoption.

---

## §4 Phased roadmap (4 phases, all skeleton-or-doc tier)

CS proposes only phases that respect own1 + the always-online retired_intentional posture. No phase exit criterion is "first live D4 session." All phases below are achievable under own1 without violating the always-online boundary.

### §4.1 Phase D4-CS-α — D3D12 surface honesty audit (doc-tier)

- **Scope**: enumerate every `pe_d3d12_*` module's actual surface coverage vs Apple Metal capability matrix; mark `_real` filenames as naming-only where applicable; produce a per-module honest tier table.
- **Inputs**: 9 `pe_d3d12_*` modules + `dx_d3d12_stage{1,2}` + `pe_dxgi_swapchain` + `pe_swapchain_present_track`.
- **Exit criteria**: 1 audit doc landed (`docs/D4_D3D12_SURFACE_HONESTY_AUDIT.md` candidate); each module annotated skeleton/stage1/stage2; validated_manjeom column = 0 across the board.
- **Honest tier**: documentation-tier (no code emission, no validated_manjeom promotion).
- **Invariant impact**: own1 carries (Wine 0); own2 carries (honest tier explicit); raw#9/11/12/15/175/270/271 enforce.

### §4.2 Phase D4-CS-β — D3D12-to-Metal router skeleton inventory (doc-tier)

- **Scope**: identify every gamebox component that, in composition, would form the D3D12-to-Metal router; map each to the equivalent Apple Metal API; mark missing pieces; honest assessment of how much of "D4 frame translation" is currently expressible.
- **Inputs**: §4.1 output + `docs/LOADER_NATIVE_HELPER_BRIDGE.md` + `docs/D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md`.
- **Exit criteria**: 1 router inventory doc landed; gap list with priority + own1-compatible resolution path per gap; explicit "no Apple D3DMetal embedding" reaffirmation.
- **Honest tier**: documentation-tier.
- **Invariant impact**: own1 (no D3DMetal embed) explicit; own2 honest; raw#9/11/12/15/175/270/271 enforce.

### §4.3 Phase D4-CS-γ — Always-online posture freeze (doc-tier)

- **Scope**: formalize the `retired_intentional` adoption at a freeze level equivalent to `EAC_KERNEL_HONEST_FREEZE.md` and `VAC_HONEST_FREEZE.md`; explicit Battle.net auth + BattlEye boundary; explicit non-bypass declaration; cross-link to Track CQ output.
- **Inputs**: `docs/D4_MANJEOM_REVIEW.md` (predecessor) + Track CQ output (Battle.net forward roadmap) + `docs/EAC_KERNEL_HONEST_FREEZE.md` (template).
- **Exit criteria**: 1 honest-freeze doc landed (`docs/D4_ALWAYS_ONLINE_HONEST_FREEZE.md` candidate); 0 bypass code; 0 evasion phase; explicit declaration of non-pursuit.
- **Honest tier**: documentation-tier (honest freeze).
- **Invariant impact**: own1 (Wine 0 / DRM bypass 0 / anti-cheat bypass 0) explicit; own2 honest; raw#9/11/12/15/175/270/271 enforce.

### §4.4 Phase D4-CS-δ — Skeleton perf cluster c_d4_* honest annotation (doc-tier)

- **Scope**: 27 `c_d4_*` perf modules per `D4_MANJEOM_REVIEW.md` §4 each get a one-line honest annotation: synthetic-only / no live game state observed / always-online retired_intentional governs validation eligibility.
- **Inputs**: 27 `c_d4_*` modules (already landed at skeleton level per predecessor cycles).
- **Exit criteria**: 1 annotation table doc landed; every module's honest tier explicit; validated_manjeom = 0 for all 27.
- **Honest tier**: documentation-tier.
- **Invariant impact**: own1 carries; own2 honest; raw#9/11/12/15/175/270/271 enforce.

### §4.5 Phases NOT proposed (and why)

- "Phase D4-CS-ε first real frame" — **NOT PROPOSED**. Blocked by G1 + G6 + G7. Would require either (a) hexa interp closure event (sibling repo, multi-year horizon), or (b) own1 violation (out-of-scope).
- "Phase D4-CS-ζ MetalFX upscale validation" — **NOT PROPOSED**. Depends on G2 (router validated). Skeleton state preserved.
- "Phase D4-CS-η 30-hour live session telemetry" — **NOT PROPOSED**. Requires live always-online entry, which is retired_intentional out-of-scope.

---

## §5 Cross-dependencies

### §5.1 Dependency on Track CQ (Battle.net)

D4 P1+P2 surface (Battle.net Setup install + Agent SD runtime + OAuth catalog) is **inherited** from Track CQ output. CS does not re-author Battle.net surface. Specifically:

- D4-CS-γ honest freeze cross-links to CQ's Battle.net always-online stance.
- D4 G6 (Battle.net Agent SD under own1) is delegated to CQ scope.
- CQ provides the upstream `.roadmap.battlenet` honest posture; CS only consumes it for D4 framing.

If CQ adopts a different posture for Battle.net (e.g., honest-spec freeze rather than retired_intentional), CS's §4.3 still applies because D4's retired_intentional is **D4-specific** (driven by always-online for game session, not just for launcher). The two postures are independent per `D4_MANJEOM_REVIEW.md` §6.

### §5.2 Contrast with Track CR (D2R)

| Axis | D2R (CR scope) | D4 (CS scope) |
|---|---|---|
| Renderer | DX11 → Metal direct path | DX12 + DXR → Metal router (skeleton) |
| Always-online | Optional (offline mode handshake exists) | Mandatory (no offline path) |
| `.roadmap` status | `none` (offline path expressible) | `retired_intentional` (block-acknowledged PASS-equivalent) |
| First-frame feasibility under own1 | Theoretically open (post G1 + G5) | Structurally interrupted at G6/G7 |
| 27 perf cluster relevance | partial (live game state reachable in offline) | synthetic-only (live state retired) |
| Battle.net launcher dependency | shared (Track CQ) | shared (Track CQ) |
| Anti-cheat surface | none observed | BattlEye (server-side; cross-class with EAC freeze) |

**Shared infrastructure**: PE32+ x86_64 load path, IAT resolve runtime, kernel32/user32/advapi32/ws2_32 surfaces, swapchain present skeleton, perf telemetry shape. The asymmetry is in renderer (DX11 vs DX12+DXR) and in always-online (D2R has an off-ramp; D4 does not).

### §5.3 Cross-class with anti-cheat freeze docs

D4's BattlEye surface inherits from the cross-class anti-cheat freeze posture established in `EAC_KERNEL_HONEST_FREEZE.md` (Track CL) and `VAC_HONEST_FREEZE.md` (Track CK). D4-CS-γ does not re-author anti-cheat freeze; it cross-links.

---

## §6 Open questions

1. **MetalFX upscale honest status under D4 workload**: skeleton-tier per `pe_perf_metalfx_upscale.hexa`; no real D4 frame has driven a real MetalFX upscale; eligibility per Apple Silicon device matrix not validated against D4 specifically. Resolution gated on G2 + G8.
2. **GPU residency under D4 working set**: D4 working set is reportedly ~7.5–11 GB peak per `D2R_D4_COMPARISON.md` §2 (estimate). Apple Silicon UMA residency model has not been validated against this size under a real D4 frame on this repo. Resolution gated on G2 + G9.
3. **Frame pacing under DXR mixed workload**: D4 uses DXR tier 1.1 in mixed-with-raster mode. Apple Metal raytracing pacing characteristics under such a mix have not been observed under this repo's own1 path. Resolution gated on G2 + G10.
4. **Mesh shader real path on Apple Silicon**: `pe_d3d12_mesh_shader.hexa` is skeleton; Apple Metal mesh shader maturity per device generation (M1 vs M2 vs M3 vs M4) has not been honestly tabulated under D4 specifically.
5. **Raytracing real path tier on Apple Silicon**: `d4_dxr_check()` returns true on arm64 + macOS ≥ 14, but this is a coarse availability gate. Per-device tier (BVH refit cost, intersection shader cost) has not been honestly tabulated under D4.
6. **D3D12 indirect command (ExecuteIndirect) Metal mapping under D4**: D4 reportedly uses heavy indirect dispatch in open-world streaming. The Metal indirect command buffer (ICB) mapping has not been validated against a real D4 indirect stream.
7. **Always-online retired_intentional vs honest-freeze**: §4.3 proposes a freeze doc; should it formally replace `retired_intentional` enum or supplement it? Open per current schema.
8. **Cross-link cardinality with Battle.net (Track CQ)**: how tight should the CS ↔ CQ cross-link be? Current proposal: §4.3 cross-links once at freeze level; deeper coupling deferred.
9. **27 c_d4_* perf cluster honest annotation format**: §4.4 proposes one-line annotations; should each module also receive its own honest-tier marker? Deferred.
10. **D4 PE+IAT runtime path under hexa interp closure**: G5 requires kernel32 stage4 strengthening which itself requires hexa interp real-run closure (G1). Composite blocker; resolution path is structurally G1 first, then G5.

---

## §7 In-place writes

| File | Op |
|---|---|
| (none) | — |

`in_place_writes`: 0 (cleanest doc-only forward roadmap).
NEW files: 2 (this handoff doc + Track CS marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0 CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/ / 0 existing gamebox/doc/ / 0 existing docs/*.md mutation / 0 predecessor marker mutation / 0 gamebox-native marker mutation.

---

## §8 Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (2 NEW files only)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (no user directive verbatim quoted)
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no DRM bypass / no anti-cheat bypass / no always-online evasion / no Apple D3DMetal embed / no anima clone ✓
- **own2**: forensic_verdict honest unchanged; D3D12 surface honesty preserved; always-online retired_intentional posture explicit; skeleton-tier vs validated-tier distinction preserved; validated_manjeom=0 carries; no progress claims, no fake PASS, no first-frame promise ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓

---

## §9 Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CS is a forward-looking honest roadmap doc. NO module count change / NO state mutation / NO validated_manjeom promotion / NO real D4 frame claim / NO bypass path. D3D12-to-Metal router stays skeleton-tier. Always-online retired_intentional stance carries. Real-tier validation perpetually deferred behind hexa interp closure (G1) and structurally interrupted by always-online + anti-cheat (G6 + G7).

---

## §10 Supersedes

- (informally) any prior aspirational D4 roadmap that did not honestly account for the always-online hard gate
- (informally) any prior D4 plan that implied a D3D12-to-Metal router validated path on darwin/arm64 within this repo's own1 boundary
- (does NOT supersede) `docs/D4_MANJEOM_REVIEW.md` — that mk1 narrative remains the authoritative `retired_intentional` adoption record; CS extends it forward
- (does NOT supersede) `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` — that 6-phase model remains the schema; CS narrows D4-specific phases to honest-tier-achievable subset
- (does NOT supersede) `docs/D2R_D4_COMPARISON.md` — that perf comparison estimate remains the qualitative baseline

---

## §11 Predecessors

- Track CR — `state/markers/gamebox_d2r_roadmap_track_cr_landed.marker` (sibling cycle; D2R forward roadmap; ordering predecessor)
- Track CQ — `state/markers/gamebox_battlenet_roadmap_track_cq_landed.marker` (sibling cycle; Battle.net forward roadmap; cross-dep)
- Track CP — `state/markers/gamebox_final_session_closure_track_cp_landed.marker` (alphabetic predecessor)
- Track CO — `state/markers/gamebox_v1_0_4_release_notes_track_co_landed.marker` (v1.0.4 release predecessor)
- Track CN — `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker` (G1 perpetual deferral source)
- Track CL — `state/markers/gamebox_eac_kernel_honest_freeze_track_cl_landed.marker` (anti-cheat freeze cross-class)
- Track CK — `state/markers/gamebox_vac_honest_freeze_track_ck_landed.marker` (anti-cheat freeze cross-class)
- `docs/D4_MANJEOM_REVIEW.md` (mk1 narrative; `retired_intentional` adoption source)
- `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` (6-phase model; phase definitions)
- `docs/D2R_D4_COMPARISON.md` (perf comparison estimate)

---

## §12 Next-cycle hooks (NOT triggered in CS)

- Phase D4-CS-α candidate: D3D12 surface honesty audit doc
- Phase D4-CS-β candidate: D3D12-to-Metal router skeleton inventory doc
- Phase D4-CS-γ candidate: D4 always-online honest freeze doc (sibling to EAC + VAC freezes)
- Phase D4-CS-δ candidate: 27 c_d4_* perf cluster honest annotation doc
- (perpetually deferred per CN): G1 hexa interp closure CM-26..CM-30 → unblocks G5 and downstream
- (out-of-scope under own1): G6 Battle.net always-online entry / G7 BattlEye live anti-cheat surface
