# D2R Roadmap — Track CR Landed (2026-05-05)

> Track CR on gamebox standalone repo, branch `main`. Forward-looking honest
> roadmap for Diablo II: Resurrected (D2R) on darwin/arm64 via airgenome-gamebox
> stack. **Supersedes** prior D2R sections in `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md`
> + `docs/D2R_D4_COMPARISON.md` for the *forward-direction phasing* (those docs
> retained as reference for predecessor estimate tables + comparison surface).
> Online realm protocol honest-frozen per `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md`
> — sideways-PASS posture. Single-player / offline launch path remains the live
> scope.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CQ battle.net roadmap; depends_on=[CQ]).
- Predecessor (immediate): Track CQ (battle.net agent + bnet runtime forward roadmap).
- Predecessor (semantic): Track L (D2R realm protocol honest freeze 2026-05-03), Track CN (perpetual deferral ack), Track CO (v1.0.4 release).
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## §1 Current honest posture (per-module)

The five `lib/loader/pe_d2r_*.hexa` modules + `lib/loader/d2r_launch.hexa` carry
the surface area for D2R. All emit synthetic-inline scaffold round-trips and
emit `partial`/`PASS` verdicts at the **paper / synthetic** level only. None
have produced a real D2R frame on darwin/arm64.

| module | path | tier | notes |
|---|---|---|---|
| `d2r_launch` | `lib/loader/d2r_launch.hexa` | skeleton | meta-data + scenario list + `validate_d2r_path` synthetic; no real PE map / IAT bind / entry call |
| `pe_d2r_specific_track` | `lib/loader/pe_d2r_specific_track.hexa` | skeleton (cond.1 partial) | 7-phase chain tracker (process_attach → first_frame); synthetic Battle.net agent markers + .pdata SEH walk + VS_VERSIONINFO emit; real binary mmap pending Battle.net install |
| `pe_d2r_real_launch_phase` | `lib/loader/pe_d2r_real_launch_phase.hexa` | skeleton (cond.1 partial) | 8-phase chain (bnet_launch_request → server_handshake_or_offline) with IPC count tracker; synthetic PE parse + IAT real-resolve walk + mainCRTStartup reach; real D2R.exe spawn pending |
| `pe_d2r_d3d11_renderer_init` | `lib/loader/pe_d2r_d3d11_renderer_init.hexa` | skeleton (cond.3 partial) | 8 init steps (device_create..lighting_precompute); synthetic ID3D11Device + DXGI swapchain + first-frame submit simulation; real GPU submit pending loader_dx blk.1 D3DMetal binary licence transitive |
| `pe_d2r_mpq_archive_load` | `lib/loader/pe_d2r_mpq_archive_load.hexa` | skeleton (cond.2 PASS, MPQ format round-trip) | 8 archive_kinds (d2data..hd_textures + Patch_D2 override); synthetic 100-byte MPQ header round-trip byte-equal; real Storm.dll wrapper pending |
| `pe_d2r_savegame_d2s_format` | `lib/loader/pe_d2r_savegame_d2s_format.hexa` | skeleton (cond.2 PASS, .d2s format round-trip) | 6 op_kinds × 7 section_kinds; synthetic header magic 0xAA55AA55 + version 0x60 + 5-section round-trip; real .d2s file IO pending |

Skeleton vs validated semantics (per `project_skeleton_vs_validated_tier.md`):
surface coverage = 100% across the 6 modules (round-trip emit fan-out, byte-equal
synthetic verdicts, all paper-mode invariants honored), but **no real game frame
observable on darwin/arm64**. validated_manjeom carries forward = 0 per
raw#10 honest C3.

---

## §2 Realm freeze acknowledgment (sideways-PASS posture)

Per `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` (Track L 2026-05-03):

- D2R online realm protocol = **honest-frozen** at high-level layer naming reference only (Auth / Realm Selection / Game Realm / Warden cross-layer; LOGIN → ... → GAME_LOOP state-machine name level).
- Wire-protocol implementation / packet construction / MITM / replay / private realm server / traffic shaping evasion / packet decryption tooling — **永구 out-of-scope**.
- own1 enforcement: Wine 0 / community wrapper 0 / protocol-impl 0 / MITM 0 / replay 0 / private server 0.
- L1-L5 lock-in (EULA / Warden detection / multi-year horizon / D2R single-target / Track J/K/L parallel) explicit + user-acknowledged 2026-05-03.

**Track CR posture for online realm**: block-acknowledged = **PASS-equivalent** (sideways-PASS / `retired_intentional`-adjacent). Per `project_retired_intentional_sideways_pass.md`: do not re-open. Track CR makes **0 modifications** to the realm freeze surface; cites it as inert reference only.

**Live scope for Track CR**: SP / offline launch path on darwin/arm64. Real D2R single-player frame is the only forward target.

---

## §3 Gap inventory (real D2R offline frame on darwin/arm64)

| gap | module(s) gating | external dependency | own1 maintainable? | sideways-PASS option |
|---|---|---|---|---|
| Hexa interp closure CM-26..CM-30 | all 6 D2R modules + `tool/closure_orchestrator.hexa` + `cli/gamebox.hexa` | sibling repo `hexa-lang` (multi-year horizon per Track AU path A) | yes (own1-maintained) | no — runtime launch blocker, paper-mode derivation acceptable for status only |
| Battle.net agent install (Phase 1-3 from CQ roadmap) | `pe_d2r_real_launch_phase` (bnet_launch_request) | external CDN / Battle.net Setup.exe i386 PE map runtime | yes via Track CQ phasing | no — required for D2R.exe acquisition |
| MPQ archive **real** load (Storm.dll wrapper) | `pe_d2r_mpq_archive_load` | D2R install (~30GB MPQ archives on disk) | yes (synthetic round-trip already PASS; real-load = mmap + decompress) | partial — synthetic format already round-trip byte-equal |
| D2R.exe x86_64 PE32+ entry call | `pe_d2r_real_launch_phase` (d2r_exe_spawn → tls_callback_fire) | x86_64 Win64 ABI ↔ macOS SysV ABI adapter (win32_abi from d2r_phase_deps) | yes (own1-maintained) | no |
| D3D11 → Metal real first-frame submit | `pe_d2r_d3d11_renderer_init` (phase 8 lighting_precompute) | D3DMetal binary licence (loader_dx blk.1 transitive — permanent external dependency) | **NO** (D3DMetal licence is permanent external) | sideways-PASS via `partial` verdict freeze (cond.3 evidence augment, no `met` promotion) |
| .d2s savegame real IO + AES-256 decrypt | `pe_d2r_savegame_d2s_format` | D2R install (savegame folder) + AES-256 primitive (already in lib/) | yes | partial — synthetic 5-section round-trip already PASS |
| Battle.net realm online play | (online realm modules — none in scope) | ToS violation surface | n/a | sideways-PASS via Track L freeze (永구 out-of-scope) |

---

## §4 Phased roadmap (3 forward phases + 2 sideways-PASS rails)

### Phase R1 — Hexa interp closure dependency (perpetually deferred, multi-year)

- **Scope**: gate event for any real-run validation of `tool/closure_orchestrator.hexa` (status / list / emit / selftest) + `cli/gamebox.hexa` (status / list / probe / launch / selftest) + the 6 D2R modules' real `hexa run` self_test invocations.
- **Inputs**: sibling repo `hexa-lang` build pipeline; CM-26..CM-30 milestones in `docs/CLOSURE_ROADMAP.md` §5.
- **Exit criteria**: `/Users/ghost/core/hexa-lang/build/hexa_interp` binary present + functional; CM-26..CM-30 land event observed.
- **Honest tier at exit**: still **skeleton** for D2R modules — Phase R1 only unblocks `hexa run` of self_tests; does NOT touch real D2R frame surface. validated_manjeom = 0 carries.
- **Invariant impact**: own1 maintained (no Wine / no Proton / no GPTK shortcut). own2 honest emit preserved (paper-mode → real-run transition documented per Track CN protocol).

### Phase R2 — Battle.net agent acquisition path (depends on Track CQ Phase 1-3)

- **Scope**: real Battle.net agent install on darwin/arm64 such that D2R.exe + MPQ archives + .d2s save folder land on disk.
- **Inputs**: Track CQ Phase 1 (Battle.net-Setup.exe i386 PE map + IAT runtime bind + WININET + CDN download), Track CQ Phase 2 (OAuth proxy + product DB cache + CDN mirror select), Track CQ Phase 3 (D2R-specific download + install + SHA verify).
- **Exit criteria**: D2R.exe (x86_64 PE32+) + d2data.mpq / d2exp.mpq / Patch_D2.mpq / hd_textures.mpq / d2char.mpq / etc. + savegame folder present on disk at expected paths.
- **Honest tier at exit**: still **skeleton** for D2R modules — install completion does not produce a frame. `pe_d2r_specific_track` cond.1 + `pe_d2r_real_launch_phase` cond.1 may flip from `partial` to `partial→met-candidate` (real binary smoke pending), but no `met` promotion until Phase R3.
- **Invariant impact**: own1 = Wine 0 (Track CQ phasing maintains hexa-only). own2 = honest emit (install completion logged honestly without frame claims).

### Phase R3 — D2R single-player offline real first-frame attempt (own1-maintained, gated on R1+R2)

- **Scope**: real D2R.exe spawn → TLS callback fire → D2 DLL family load (D2.dll / D2Game.dll / D2Common.dll / D2Win.dll / D2Multi.dll / D2Sound.dll) → Storm.dll MPQ init → savegame load → D3D11 device init → first frame Present (offline mode, online realm = sideways-PASS).
- **Inputs**: Phase R1 done (hexa interp), Phase R2 done (D2R installed), D3DMetal binary licence (external dependency — permanent transitive), x86_64 Win64 ABI adapter (own1 hexa-only).
- **Exit criteria**: at least one real D2R single-player offline frame Present on darwin/arm64 (P99 frame time measurable; FPS counter visible via MTL_HUD_ENABLED=1 or D6 frametime_jsonl).
- **Honest tier at exit**:
  - On D3D11 first-frame Present success → `pe_d2r_d3d11_renderer_init` cond.3 partial→partial (D3DMetal binary licence remains permanent external; sideways-PASS at `partial` per Track L pattern).
  - On real D2R.exe spawn + entry → `pe_d2r_real_launch_phase` cond.1 partial→met-candidate; `pe_d2r_specific_track` cond.1 partial→met-candidate.
  - validated_manjeom: increments only if measurable real frame data persists ≥ 30 minutes across ≥ 3 sessions per `project_skeleton_vs_validated_tier.md`.
- **Invariant impact**: own1 strict — Wine 0, no GPTK shortcut, no community wrapper. own2 = honest emit (frame achievement logged with measurement; no inflation; D3DMetal external dependency explicit; online realm sideways-PASS per Track L unchanged).

### Phase S1 (sideways) — Online realm honest-frozen (Track L)

- **Status**: PASS-equivalent posture, perpetually inert. `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` SSOT. NO modification by Track CR. NO modification by future cycles unless user explicitly re-opens (would require new lock-in event with EULA/ToS acknowledgment).

### Phase S2 (sideways) — Cosmetics / runtime perf modules (skeleton-tier carry)

- **Status**: 70+ skeleton-tier perf modules from cycle 37 milestone (`docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` §0a) carry as **skeleton** with synthetic verdicts only. Real-tier validation = Phase R3 + 30+ session measurement. validated_manjeom currently 0; remains 0 until R3 + session evidence.

---

## §5 Cross-deps with battle.net (Track CQ)

D2R launches via Battle.net agent (D2R.exe is spawned by the Battle.net Agent process per `pe_d2r_real_launch_phase` phase 1 = `bnet_launch_request`). Track CR Phase R2 directly depends on Track CQ phases:

| Track CR phase | Track CQ phase gate | gating reason |
|---|---|---|
| R2 (agent acquisition) | CQ Phase 1 (Battle.net-Setup install) | i386 PE map + IAT runtime bind + WININET CDN download infra |
| R2 (agent acquisition) | CQ Phase 2 (Battle.net runtime) | OAuth proxy + product DB cache + CDN mirror select for D2R catalog entry |
| R2 (agent acquisition) | CQ Phase 3 (D2R-specific download) | D2R asset binary acquisition (~30GB) on disk |
| R3 (D2R first frame) | CQ Phase 4 (agent → D2R.exe spawn IPC ack) | bnet_launch_request IPC handshake with Battle.net Agent (4 IPC frames per `pe_d2r_real_launch_phase` step 820001) |

If Track CQ phases stall, Track CR Phase R2 + R3 stall. Track CR makes no claim about CQ ETA; refer to Track CQ doc for own ETA estimates.

---

## §6 Open questions

1. **D3DMetal binary licence transitive permanence**: is the D3DMetal binary licence path acceptable as a permanent transitive external (sideways-PASS at `partial` for cond.3) or does own1 strict interpretation require a fully hexa-native D3D11 → Metal translation layer (multi-year horizon, possibly infeasible)?
2. **MPQ Storm.dll wrapper**: synthetic round-trip is byte-equal at PASS, but real-load = mmap + decompress + filename hash. Does own1 require a hexa-native MPQ reader (multi-cycle work) or is Storm.dll real-load acceptable as transitive Win32 surface (already covered by win32_kernel32 / win32_user32 phase deps)?
3. **AES-256 .d2s decrypt**: gamebox lib already has AES-256 primitive (verify path). Does D2R use vanilla AES-256-CBC or a Blizzard-specific variant requiring further reverse-engineering (out-of-scope per Track L spirit)?
4. **D2R offline mode availability**: D2R installer checks Battle.net agent presence at launch. Is full offline single-player mode reachable without active Battle.net session (Blizzard public docs suggest yes for SP, but agent presence required)? If not, R3 Phase reduces to Battle.net agent + offline-toggle UX path (still SP, no realm).
5. **Hexa interp closure CM-26..CM-30 ETA**: per Track CN, perpetually deferred multi-year horizon. Does any user-direction trigger override exist that would unblock R1 sooner?
6. **m1/m2/m3/m4 hardware variance**: `docs/D2R_D4_COMPARISON.md` §6 documents hardware tier corrections. Phase R3 measurement protocol pinning to M2 Pro 16GB baseline acceptable, or fan out to M1/M3/M4 across multiple sessions?
7. **`retired_intentional` formal adoption for D2R online**: Track L deferred formal `retired_intentional` mk2 status adoption pending separate user lock-in cycle. Does Track CR's sideways-PASS posture suffice indefinitely, or does a future cycle need to formalize the mk2 enum status flip?

---

## §7 Hard invariants carried forward

- **own1**: Wine 0 / hexa-only / no community wrapper / no AC bypass / no DRM bypass / no MITM / no realm protocol implementation / no anima clone.
- **own2**: honest emit / silent error 0 / validated_manjeom = 0 carry / skeleton-tier vs real-tier distinction preserved / Track L sideways-PASS posture for online realm preserved.
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce.
- **Forensic verdict (UNCHANGED)**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- **Hexa interp closure CM-26..CM-30**: perpetually deferred per Track CN; paper-mode derivation acceptable for status checks; runtime launch blocked.

---

## §8 In-place writes

| File | Op |
|---|---|
| (none) | — |

`in_place_writes`: 0 (cleanest doc-only roadmap cycle).
NEW files: 2 (this handoff doc + Track CR marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0 CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/ / 0 existing gamebox/doc/ / 0 existing docs/*.md / 0 predecessor marker mutation / 0 gamebox-native marker mutation / 0 lib/loader/d2r_*.hexa or pe_d2r_*.hexa.

---

## §9 Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (cleanest cycle — 2 NEW files only; 0 in-place writes)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (no user Korean directives quoted; intent paraphrased only)
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no AC bypass / no DRM bypass / no MITM / no realm protocol implementation / no anima clone ✓
- **own2**: forensic_verdict honest unchanged; D2R real frame surface = 0 explicit; sideways-PASS posture for online realm explicit; validated_manjeom = 0 carries; skeleton-tier vs real-tier distinction preserved ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓

---

## §10 Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CR is forward-looking honest roadmap doc only. NO module count change / NO state mutation / NO validated_manjeom promotion / NO online realm scope re-opening. Real D2R single-player offline frame on darwin/arm64 = perpetually deferred multi-cycle horizon (R1 + R2 + R3 chain).

---

## §11 Predecessors

- Track CQ — `state/markers/gamebox_battlenet_roadmap_track_cq_landed.marker` (immediate predecessor; battle.net forward roadmap; Track CR depends_on)
- Track CO — `state/markers/gamebox_v1_0_4_release_notes_track_co_landed.marker` (v1.0.4 release predecessor)
- Track CN — `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker` (R1 perpetual deferral lineage)
- Track L — `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` (online realm honest freeze; sideways-PASS source)
- `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` (predecessor estimate tables; superseded for forward phasing by this doc)
- `docs/D2R_D4_COMPARISON.md` (predecessor comparison surface; retained as reference)
- `lib/loader/d2r_launch.hexa` + `lib/loader/pe_d2r_*.hexa` (5 files) — surface modules cited throughout

---

## §12 Next-cycle hooks (NOT triggered)

- (perpetually deferred R1): hexa interp closure CM-26..CM-30 land event — sibling repo dependency
- (gated on Track CQ R2 phases): Battle.net agent acquisition path — Track CQ phase landings
- (gated on R1 + R2): Phase R3 D2R single-player offline real first-frame attempt
- (sideways-PASS S1 inert): online realm — no future cycle unless user re-opens with EULA acknowledgment
- Future Track CR+1 candidate: D2R install path detect + .d2s savegame folder probe (paper-mode pre-flight) — does NOT require R1+R2 (probe only emits paths)
