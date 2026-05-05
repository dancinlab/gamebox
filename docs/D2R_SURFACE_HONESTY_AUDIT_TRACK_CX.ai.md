# D2R Surface Honesty Audit — Track CX Landed (2026-05-05)

> Track CX on gamebox standalone repo, branch `main`. Doc-tier per-module
> honest annotation audit of the **D2R surface** (`lib/loader/d2r_launch.hexa`
> + 5 × `lib/loader/pe_d2r_*.hexa`). Analog of phase D4-CS-alpha (Track CV)
> applied to D2R rather than D4. Bypasses every runtime / external-binary /
> install / hexa interp gate (R1/R2/R3 from Track CR; CM-26..CM-30 from
> Track CN). Paper-mode tier table only — no module surface change, no
> validated_manjeom promotion, no real-stream evidence claim. depends_on
> = [CR, CU, CW] (CW for ordering only; CR = D2R roadmap SSOT; CU =
> immediate D2R sibling cycle predecessor).

---

## §0 Executable phase selection rationale

Track CR (`docs/D2R_ROADMAP_TRACK_CR.ai.md` §4) defines 3 forward phases
(R1 / R2 / R3) and 2 sideways-PASS rails (S1 / S2). Track CU executed
the path-inventory paper-mode sub-phase (`D2R_PHASE_INSTALL_PATH_PROBE_TRACK_CU.ai.md`).
Track CX audited remaining doc-tier deliverables for executability today:

| candidate | doc-tier executable today? | redundant with CU? |
|---|---|---|
| MPQ archive surface honest annotation (per `archive_kind` enum) | YES — module already authored | partial — CU §2 cites archive enum file-set; per-module audit is distinct |
| `.d2s` savegame format honest annotation (`pe_d2r_savegame_d2s_format` audit) | YES — module already authored | partial — CU §3 cites `section_kind`; per-module audit is distinct |
| D3D11 renderer init honest annotation (`pe_d2r_d3d11_renderer_init` audit) | YES — module already authored | NO — CU did not touch renderer surface |
| D2R perf cluster honest annotation (`c_d2r_*` perf modules) | partial — perf modules carry from cycle 37 milestone S2 sideways-PASS | NO — separate from CU scope |
| D2R x CQ cross-link spec (no bnet auth requirement for SP offline) | YES — formal documentation of independence | redundant — CU §6 already documents CT cardinality only |
| D2R real_launch_phase skeleton inventory (`pe_d2r_real_launch_phase` audit) | YES — module already authored | NO — CU did not audit launch chain |
| **per-module honest annotation audit (all 6 D2R modules)** | **YES — composite of above 5 candidates** | **NO — CU never produced a per-module tier table** |

Track CX adopts the **composite per-module honest annotation audit** as
its scope. This is the direct D2R analog of Track CV phase D4-CS-alpha
(15-module D3D12 surface audit). Output: per-module tier table covering
all 6 D2R modules, cond.X status mapped to CR §1, synthetic-only /
real-stream evidence annotation, validated_manjeom column = 0 across the
board, gap inventory referenced to CR §3 R1/R2/R3 phasing. No module
mutation. No new module emission. No filesystem touch.

Skipped alternatives:

| skipped phase | reason |
|---|---|
| R1 hexa interp closure | perpetually deferred multi-year per Track CN runtime gate |
| R2 battle.net agent acquisition | depends on Track CQ Phase 1-3 install artefacts |
| R3 D2R first frame | gated on R1 + R2 + D3DMetal binary licence |
| S1 online realm sideways-PASS | Track L SSOT modification policy forbids re-open |
| S2 skeleton perf carry | validated promotion gate R3 + 30+ session evidence not reached |
| stand-alone MPQ honest annotation | subsumed under composite §3 row |
| stand-alone `.d2s` honest annotation | subsumed under composite §3 row |
| stand-alone D3D11 honest annotation | subsumed under composite §3 row |
| stand-alone real_launch_phase honest annotation | subsumed under composite §3 row |
| stand-alone D2R x CQ cross-link spec | redundant with CU §6 cardinality note |
| `c_d2r_*` perf cluster annotation | S2 sideways-PASS carry; deferred until R3 + session evidence cycle |

---

## §1 Scope (per-module honest annotation tier table only)

**In scope (Track CX)**:
- Enumerate every D2R surface module's actual coverage vs darwin/arm64
  real-frame target.
- Annotate each module with: tier (skeleton / partial / met) / cond.X
  status (per CR §1) / synthetic-only flag / real-stream evidence /
  validated_manjeom (0 across the board) / G-gap mapping (per CR §3).
- Aggregate: 6-module surface coverage 100% at synthetic / partial level;
  0 real frame on darwin/arm64; SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
  forensic verdict carries.

**Out of scope (Track CX)**:
- Module mutation / new module emission (paper-mode audit only).
- Filesystem traversal / install probe (Track CU scope; CX cites but does
  NOT duplicate).
- Real D2R.exe PE32+ inspection (gated on R2).
- MPQ header read / Storm.dll wrapper invocation (gated on R3).
- `.d2s` AES-256 decrypt / character data parse (gated on R3 + AES path
  open question per CR §6 OQ#3).
- Online realm wire-protocol surface (Track L sideways-PASS inert —
  perpetually out-of-scope).
- Battle.net agent install path (Track CQ scope; CX cites but does NOT
  duplicate).
- `c_d2r_*` perf modules (cycle 37 milestone S2 carry; separate audit
  cycle deferred).
- Validated tier promotion (gated on R3 + 30+ session evidence per
  `project_skeleton_vs_validated_tier.md`).

---

## §2 Module inventory (6 modules under audit)

| # | name | path | LOC | author cycle | cond.X reference |
|---|---|---|---|---|---|
| 1 | `d2r_launch` | `lib/loader/d2r_launch.hexa` | 307 | Phase 9 launch path skeleton | (umbrella; meta-data + scenarios + `validate_d2r_path`) |
| 2 | `pe_d2r_specific_track` | `lib/loader/pe_d2r_specific_track.hexa` | 495 | cycle 82 phase tracker | cond.1 (D2R-specific 7-phase chain) |
| 3 | `pe_d2r_real_launch_phase` | `lib/loader/pe_d2r_real_launch_phase.hexa` | 518 | post-cycle-82 8-phase tracker | cond.1 (Battle.net Agent IPC chain) |
| 4 | `pe_d2r_d3d11_renderer_init` | `lib/loader/pe_d2r_d3d11_renderer_init.hexa` | 493 | renderer init phase tracker | cond.3 (D3D11 first-frame partial sideways-PASS) |
| 5 | `pe_d2r_mpq_archive_load` | `lib/loader/pe_d2r_mpq_archive_load.hexa` | 466 | MPQ format round-trip | cond.2 (MPQ format synthetic round-trip PASS) |
| 6 | `pe_d2r_savegame_d2s_format` | `lib/loader/pe_d2r_savegame_d2s_format.hexa` | 485 | .d2s format round-trip | cond.2 (.d2s format synthetic round-trip PASS) |

Total LOC: 2,764 hexa across 6 files. All 6 files present + authored +
self-test scaffold complete (synthetic / paper-mode level). 0 real D2R
binary parse. 0 real Storm.dll wrapper invocation. 0 real `.d2s` file IO.
0 real ID3D11Device CreateDevice. 0 real Battle.net Agent IPC.

---

## §3 Per-module honest annotation tier table

| row | name | tier | cond.X | synthetic_only | real_stream_evidence | validated_manjeom | sideways_pass_at_partial | G-gap |
|---|---|---|---|---|---|---|---|---|
| 1 | `d2r_launch` | skeleton | umbrella (no cond) | YES | none | 0 | n/a | gates on R2 install presence for `validate_d2r_path` real probe |
| 2 | `pe_d2r_specific_track` | skeleton (cond.1 partial) | cond.1 | YES | none | 0 | NO (target = met-candidate via R3) | gates on R2 (real D2R.exe present) + R3 (real launch chain observed) |
| 3 | `pe_d2r_real_launch_phase` | skeleton (cond.1 partial) | cond.1 | YES | none | 0 | NO (target = met-candidate via R3) | gates on R2 (Battle.net Agent install) + R3 (real IPC handshake) |
| 4 | `pe_d2r_d3d11_renderer_init` | skeleton (cond.3 partial) | cond.3 | YES | none | 0 | **YES** — partial→partial sideways-PASS at D3DMetal binary licence permanent transitive | gates on R3 + D3DMetal binary licence (loader_dx blk.1 transitive) |
| 5 | `pe_d2r_mpq_archive_load` | skeleton (cond.2 PASS at synthetic round-trip) | cond.2 | YES | none | 0 | partial — synthetic 100-byte MPQ header round-trip already byte-equal | gates on R2 (MPQ archives on disk) for real-load (mmap + decompress) |
| 6 | `pe_d2r_savegame_d2s_format` | skeleton (cond.2 PASS at synthetic round-trip) | cond.2 | YES | none | 0 | partial — synthetic header magic 0xAA55AA55 + version 0x60 + 5-section round-trip already byte-equal | gates on R3 + AES-256 path (vanilla CBC vs Blizzard variant — CR §6 OQ#3 open) |

**Tier breakdown**:
- skeleton (umbrella, no cond): 1
- skeleton (cond.1 partial): 2
- skeleton (cond.2 PASS at synthetic round-trip level): 2
- skeleton (cond.3 partial sideways-PASS): 1
- partial→partial sideways-PASS (Track L pattern carrier): 1 (pe_d2r_d3d11_renderer_init)
- met / validated: 0
- validated_manjeom: 0 across all 6

**Synthetic-only flag**: YES across all 6. No real stream observation
recorded by any module's persist directory. self_test scaffold round-trip
emits exclusively synthetic / paper-mode payloads (e.g. `submit_kind:
synthetic_inline`, `gpu_submit_ts_ns: 0`, `verdict: scaffold_byte_equal`,
`shim_kind: synthetic_inline`).

**Real-stream evidence**: NONE across all 6 modules. No `r0_emit` row
captures real D2R binary observation. All cond.X partial / PASS verdicts
are at the **paper / synthetic** level only per CR §1 honest posture.

---

## §4 Per-module phase enum coverage detail

Per-module enum coverage from each module's header comment. Coverage =
each enum value gets ≥ 1 record in self_test scaffold; verdict = synthetic
round-trip byte-equal at paper level.

### §4.1 `d2r_launch` (umbrella)

- `d2r_info()` — game identity (name / dx / platform / dll deps).
- `d2r_phase_deps()` — Phase 0a..6 module dependency matrix.
- `d2r_dx_requirements()` — DirectX / GPU requirements string.
- `d2r_scenarios()` — startup → game entry scenario list.
- `validate_d2r_path()` — D2R.exe path validation (synthetic; absent →
  `[false, false, false]`).
- Real launch trampoline (PE map → IAT resolve → entry jump): Phase 9
  successor; NOT in current module.

### §4.2 `pe_d2r_specific_track` (7-phase chain)

phase_kind code:

| phase_kind | meaning | self_test step_id | dll_loaded | elevated |
|---|---|---|---|---|
| 1 | process_attach (D2R.exe entry + TLS_CALLBACK) | 130001 | 0 | 1 |
| 2 | d2_dll_load (D2*.dll family load) | 130002 | 1 | 0 |
| 3 | mpq_init (Storm.dll MPQ archive init) | 130003 | 1 | 0 |
| 4 | net_layer_init (Battle.net IPC + DRM token) | 130004 | 1 | 0 |
| 5 | ui_layer_init (D2Win.dll resource + cinematic) | (per module self_test) | 1 | 0 |
| 6 | savegame_load (character.d2s mmap) | (per module self_test) | 0 | 0 |
| 7 | first_frame (D3D11 Present first call) | (per module self_test) | 0 | 0 |

`first_frame_reached()` = `count_by_phase_kind(7)` ≥ 1; synthetic = TRUE
in self_test. Real D2R.exe execution: NEVER. Real first frame on
darwin/arm64: NEVER.

### §4.3 `pe_d2r_real_launch_phase` (8-phase + IPC chain)

phase_kind code:

| phase_kind | meaning | self_test step_id | elevated | ipc_count |
|---|---|---|---|---|
| 1 | bnet_launch_request (Battle.net Agent → D2R.exe spawn request) | 820001 | 1 | 4 |
| 2 | cas_chunk_validate (CAS Content Addressable Storage validate) | 820002 | 0 | 3 |
| 3 | d2r_exe_spawn (D2R.exe x86_64 PE32+ entry) | 820003 | 0 | 2 |
| 4 | tls_callback_fire (cycle 72 cross-ref) | 820004 | 0 | 0 |
| 5 | d3d11_device_init (cycle 81 cross-ref) | 820005 | 0 | 0 |
| 6 | savegame_decrypt (.d2s file AES-256) | (per module self_test) | 0 | 0 |
| 7 | ui_first_paint | (per module self_test) | 0 | 0 |
| 8 | server_handshake_or_offline | (per module self_test) | 0 | 0 |

Total `ipc_count` synthetic in self_test = 4 + 3 + 2 + 0 + 0 + 0 + 0 + 0
= 9 (Battle.net Agent IPC frames at synthetic round-trip level).

Cross-ref dependencies:
- Phase 4 → cycle 72 TLS_CALLBACK module.
- Phase 5 → cycle 81 `pe_d3d11_buffer_create_real`.

Real Battle.net Agent IPC: NEVER. Real D2R.exe spawn on darwin/arm64:
NEVER. AES-256 .d2s decrypt: synthesized only; Blizzard AES variant
verification = open per CR §6 OQ#3.

### §4.4 `pe_d2r_d3d11_renderer_init` (8 init steps + cond.3 sideways-PASS)

phase_kind code:

| phase_kind | meaning | self_test step_id | resource_count | elevated |
|---|---|---|---|---|
| 1 | device_create (ID3D11Device CreateDevice → MTLDevice) | 420001 | 1 | 1 |
| 2 | swapchain_init (DXGI swapchain — cycle 76 cross-ref) | 420002 | 2 | 0 |
| 3 | render_target_view (RTV — cycle 84 cross-ref) | 420003 | 2 | 0 |
| 4 | depth_stencil (DSV — cycle 84 cross-ref) | 420004 | 1 | 0 |
| 5 | character_pso (character render pipeline) | 420005 | 4 | 0 |
| 6 | monster_pso (monster render pipeline) | 420006 | 4 | 0 |
| 7 | ui_pso (UI render pipeline) | 420007 | 3 | 0 |
| 8 | lighting_precompute (lighting precompute frame 0) | 420008 | 8 | 0 |

`pso_count` = 5 + 6 + 7 = 3 (character + monster + UI) at synthetic round-
trip level.

Synthetic D3D11 scaffold (per module §12-A..D):
- (a) device_init synthetic: `adapter_count: 1`, `driver_type: 0x0001`
  (HARDWARE), `feature_level_max: 0xB000` (D3D_FEATURE_LEVEL_11_0),
  `create_device_flags: 0x00000002` (D3D11_CREATE_DEVICE_DEBUG),
  `device_ptr_off: 0x140070000` (synthetic ID3D11Device VTable RVA).
- (b) swapchain_create synthetic: `swapchain_buf_count: 2`,
  `swapchain_format: 0x0000001C` (DXGI_FORMAT_R8G8B8A8_UNORM),
  `swapchain_width: 1280`, `swapchain_height: 720` (D2R menu default),
  `swapchain_ptr_off: 0x140072000`.
- (c) first_frame_submit synthetic: `cmd_list_count: 1`,
  `submit_kind: synthetic_inline`, `gpu_submit_ts_ns: 0` (synthetic; real
  ts blocked by loader_dx blk.1 D3DMetal binary licence transitive).
- (d) round-trip summary: `verdict: PARTIAL`,
  `posture: evidence_augment_partial_to_partial`, real_gpu_gap permanent
  per loader_dx blk.1 transitive D3DMetal binary licence.

**Sideways-PASS at partial (Track L pattern)**: cond.3 stays at `partial`
forever — evidence augment only — no `met` promotion. D3DMetal binary
licence is a **permanent external transitive dependency** (CR §3 row 5).
own1 strict interpretation forbids embedding the binary; sideways-PASS
posture is the honest closure per Track L precedent.

### §4.5 `pe_d2r_mpq_archive_load` (8 archive_kinds + Patch_D2 override)

archive_kind code:

| archive_kind | meaning | example self_test record | file_count | size_mb |
|---|---|---|---|---|
| 1 | d2data (2D base assets — legacy classic Diablo 2 data) | 140001 | 2400 | 265 |
| 2 | d2exp (Lord of Destruction expansion data) | 140002 | 1800 | 210 |
| 3 | d2sfx (sound effects archive) | 140003 | 900 | 85 |
| 4 | d2speech (voice / speech archive) | 140004 | 600 | 120 |
| 5 | d2music (BGM / soundtrack archive) | (per module self_test) | (synthetic) | (synthetic) |
| 6 | d2char (HD character art archive) | (per module self_test) | (synthetic) | (synthetic) |
| 7 | patch_d2 (Patch_D2.mpq override priority highest) | (per module self_test) | (synthetic) | (synthetic) |
| 8 | hd_textures (D2R remaster HD texture pack) | (per module self_test) | (synthetic) | (synthetic) |

Override precedence: `patch_d2` (kind=7) > all base archives. Cited from
Mike O'Brien public MPQ format spec; NOT from third-party MPQ extractor
source.

Synthetic 100-byte MPQ header round-trip: PASS at byte-equal level.
Real Storm.dll wrapper invocation: NEVER. Real mmap + decompress + filename
hash: NEVER.

own1 enforcement: 타사 MPQ tool / community 코드 0 줄. Module tracker only —
no real Storm.dll call.

CR §6 OQ#2 open: hexa-native MPQ reader (multi-cycle work) vs Storm.dll
real-load as transitive Win32 surface (covered by win32_kernel32 /
win32_user32 phase deps).

### §4.6 `pe_d2r_savegame_d2s_format` (6 op_kinds × 7 section_kinds)

op_kind code:

| op_kind | meaning | gate |
|---|---|---|
| 1 | read (.d2s local read) | R2 (savegame folder present) |
| 2 | write (.d2s local write) | R2 + R3 |
| 3 | decrypt (AES-256 decrypt) | R3 + AES path verification |
| 4 | encrypt (AES-256 encrypt) | R3 + AES path verification |
| 5 | cloud_upload (Battle.net cloud sync upload) | **Track L sideways-PASS — out of scope perpetually** |
| 6 | cloud_download (Battle.net cloud sync download) | **Track L sideways-PASS — out of scope perpetually** |

section_kind code:

| section_kind | meaning |
|---|---|
| 1 | header_magic (0xAA55AA55 + version 0x60) |
| 2 | character_data (level / class / 16-byte block) |
| 3 | skills (skill tree allocation) |
| 4 | stash_personal (per-character personal stash) |
| 5 | stash_shared (cross-character shared stash) |
| 6 | quests (quest progress) |
| 7 | waypoints (unlocked waypoints) |

Synthetic 5-section round-trip at byte-equal: PASS.

Real `.d2s` file IO: NEVER. Real AES-256 decrypt: NEVER (vanilla CBC vs
Blizzard variant per CR §6 OQ#3 still open). Cloud sync (op_kind 5/6):
**permanently out-of-scope** per Track L sideways-PASS posture; CX makes
0 modifications to this surface.

CR §6 OQ#3 open: gamebox lib AES-256 primitive applies to vanilla CBC
path only; if D2R uses Blizzard-specific variant, decrypt path requires
RE that may fall under Track L spirit (out-of-scope perpetually).

---

## §5 Aggregate honest posture (carries from CR §1 unchanged)

| dimension | value | note |
|---|---|---|
| modules audited | 6 | all 5 pe_d2r_* + d2r_launch umbrella |
| total LOC | 2,764 | per `wc -l lib/loader/d2r_launch.hexa lib/loader/pe_d2r_*.hexa` |
| tier breakdown skeleton | 6/6 | all skeleton (with partial / PASS sub-status at synthetic level) |
| tier breakdown stage1/stage2 | 0 | unlike D4 D3D12 (CV had 1+1); D2R has no staged module |
| tier breakdown met / validated | 0 | per CR §1 honest posture |
| validated_manjeom (per module) | 0 | aggregate carry |
| real_stream_evidence (per module) | none | aggregate carry |
| sideways-PASS at partial | 1 (pe_d2r_d3d11_renderer_init cond.3) | Track L pattern carrier |
| synthetic round-trip PASS at format level | 2 (MPQ + .d2s) | byte-equal at synthetic level only |
| Wine count | 0 | aggregate carry |
| forensic verdict | SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 | UNCHANGED |

**Real D2R offline single-player frame on darwin/arm64**: 0 frames observed.
Surface coverage 100% across the 6 modules at synthetic / paper level; no
real game frame ever produced.

---

## §6 Gap inventory mapping (per CR §3)

CX maps each module to the Track CR §3 gap inventory rows:

| CR §3 gap | gating module(s) | sideways-PASS option | resolution path |
|---|---|---|---|
| Hexa interp closure CM-26..CM-30 | all 6 modules + tool/cli | NO — runtime launch blocker | Track CN perpetual deferral; paper-mode derivation acceptable for status only |
| Battle.net agent install (CQ Phase 1-3) | `pe_d2r_real_launch_phase` (phase 1) + `d2r_launch` (validate path) | NO — required for D2R.exe acquisition | Track CR Phase R2 |
| MPQ archive **real** load (Storm.dll wrapper) | `pe_d2r_mpq_archive_load` | partial — synthetic format round-trip already PASS | Track CR Phase R2 + Storm.dll wrapper as transitive Win32 surface (CR §6 OQ#2) |
| D2R.exe x86_64 PE32+ entry call | `pe_d2r_real_launch_phase` (phase 3-4) + `pe_d2r_specific_track` (phase 1) | NO | Track CR Phase R3 + win32_abi adapter |
| D3D11 → Metal real first-frame submit | `pe_d2r_d3d11_renderer_init` (phase 8) | **YES** — partial sideways-PASS at D3DMetal binary licence permanent transitive | Track L precedent; cond.3 stays `partial` forever |
| .d2s savegame real IO + AES-256 decrypt | `pe_d2r_savegame_d2s_format` (op_kind 1-4 + section_kind 1-7) | partial — synthetic 5-section round-trip already PASS | Track CR Phase R3 + AES path resolution (CR §6 OQ#3) |
| Battle.net realm online play | (no module in scope) | n/a | Track L sideways-PASS — permanently out-of-scope |
| Cloud-sync .d2s upload/download | `pe_d2r_savegame_d2s_format` (op_kind 5/6) | n/a | Track L sideways-PASS — permanently out-of-scope |

Of 8 gaps in CR §3 + 1 cloud-sync gap (CX-introduced classification):
- 1 sideways-PASS at partial (Track L pattern carrier): D3D11 first-frame
  submit (D3DMetal binary licence permanent).
- 2 partial → met-candidate possible at R3: real D2R.exe entry +
  real-load MPQ.
- 2 perpetually out-of-scope (Track L sideways-PASS): online realm +
  cloud-sync.
- 3 gated on multi-year R1 / R2 / R3 phases: hexa interp closure / bnet
  agent install / .d2s AES path.

CX modifies **0** of these gaps. CX is documentation-tier only.

---

## §7 Cross-deps (CR + CU + CW cardinality only)

Track CX depends_on = [CR, CU, CW]:

| predecessor | reason |
|---|---|
| CR | D2R roadmap doc-tier predecessor; CX cites CR §1 cond.X status / §3 gap inventory / §4 phase R1/R2/R3 / §6 open questions |
| CU | immediate D2R sibling cycle predecessor; CX cites CU path inventory + signature contract but does NOT duplicate; CX's per-module audit is a complementary doc-tier deliverable to CU's path inventory |
| CW | track sequence cardinality only; CX preserves CW → CX ordering per polled-wait protocol |

Cross-class precedent (NOT a depends_on but cited as pattern source):
- Track L (`docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md`) — sideways-PASS at
  partial pattern source for cond.3 D3D11 renderer.
- Track CV (`docs/D4_D3D12_SURFACE_HONESTY_AUDIT_TRACK_CV.ai.md`) —
  per-module honest annotation audit pattern source (D4-CS-alpha applied
  to D2R).
- Track CN — hexa interp closure perpetual deferral lineage.

D2R offline SP path independence vs Track CT (Apple Network bnet auth):
documented in CU §6; CX inherits without modification.

---

## §8 Hard invariants carried forward

- **own1**: Wine 0 / hexa-only / no community wrapper / no community MPQ
  extractor / no community .d2s parser / no community D2R wrapper / no
  CrossOver / no AC bypass / no DRM bypass / no MITM / no realm protocol
  implementation / no replay / no private server / no anima clone / no
  apple D3DMetal embed / no descent into mods/ folder.
- **own2**: honest emit / silent_error 0 / validated_manjeom = 0 carry /
  skeleton-tier vs real-tier distinction preserved / Track L sideways-
  PASS preserved for online realm + cloud-sync / no real-frame inflation
  / no fake PASS / no fake validated promotion / no premature module
  emission / paper-mode audit only.
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce.
- **Forensic verdict (UNCHANGED)**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- **Hexa interp closure CM-26..CM-30**: perpetually deferred per Track
  CN; paper-mode derivation acceptable for status checks; runtime launch
  blocked. CX output is documentation-tier per-module audit — does NOT
  require hexa interp closure.

---

## §9 In-place writes

| File | Op |
|---|---|
| (none) | — |

`in_place_writes`: 0 (cleanest doc-only audit cycle).
NEW files: 2 (this handoff doc + Track CX marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0
native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE /
0 CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/ /
0 existing gamebox/doc/ / 0 existing docs/*.md / 0 predecessor marker
mutation / 0 gamebox-native marker mutation / 0 lib/loader/d2r_*.hexa
or pe_d2r_*.hexa.

---

## §10 Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (cleanest cycle — 2 NEW files only; 0 in-place writes)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (no user Korean directives quoted; intent paraphrased only)
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no AC bypass / no DRM bypass / no MITM /
  no realm protocol implementation / no anima clone / no community MPQ
  tool / no community .d2s parser / no apple D3DMetal embed ✓
- **own2**: forensic_verdict honest unchanged; D2R real frame surface =
  0 explicit; sideways-PASS posture for online realm + cloud-sync explicit;
  validated_manjeom = 0 per-module + aggregate explicit; skeleton-tier vs
  real-tier distinction preserved per-module; paper-mode-only audit
  (no module mutation, no premature emission) ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓

---

## §11 Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CX is per-module honest annotation audit doc only. NO module count change
/ NO state mutation / NO validated_manjeom promotion / NO online realm
scope re-opening / NO cloud-sync scope re-opening / NO probe execution /
NO filesystem touch. Real D2R offline SP frame on darwin/arm64 =
perpetually deferred multi-cycle horizon (R1 + R2 + R3 chain) per Track
CR. D2R surface coverage 100% at synthetic / paper level across 6
modules; 0 real game frame ever produced; sideways-PASS at partial
posture for cond.3 D3D11 renderer carries (D3DMetal binary licence
permanent external transitive).

---

## §12 Predecessors

- Track CW — `state/markers/<CW marker>` (immediate track sequence
  predecessor; cardinality only; CW polling enforced before CX commit
  per harness protocol)
- Track CV — `state/markers/d4_d3d12_surface_honesty_audit_track_cv_landed.marker`
  (sibling per-module honest annotation audit pattern source; D4
  D3D12 surface analog applied to D2R surface in CX)
- Track CU — `state/markers/d2r_phase_install_path_probe_track_cu_landed.marker`
  (immediate D2R sibling cycle predecessor; CX complementary to CU)
- Track CR — `state/markers/d2r_roadmap_track_cr_landed.marker` (D2R
  roadmap SSOT; CX consumes §1 cond.X status + §3 gap inventory + §4
  R1/R2/R3 phasing + §6 open questions)
- Track CT — `state/markers/<CT marker>` (Apple Network bnet auth
  cardinality independence per CU §6 inheritance)
- Track CS — `state/markers/d4_roadmap_track_cs_landed.marker` (D4
  forward roadmap; phase D4-CS-alpha pattern source for CV)
- Track CQ — `state/markers/<CQ marker>` (battle.net forward roadmap;
  CR Phase R2 dependency)
- Track CO — `state/markers/gamebox_v1_0_4_release_notes_track_co_landed.marker`
  (v1.0.4 release predecessor)
- Track CN — `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker`
  (R1 hexa interp perpetual deferral lineage)
- Track L — `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` (online realm
  honest freeze; sideways-PASS at partial pattern source for cond.3 D3D11
  renderer; cloud-sync .d2s out-of-scope source)
- `lib/loader/d2r_launch.hexa` + `lib/loader/pe_d2r_*.hexa` (5 files) —
  6-module surface under audit; cited per row in §3 + §4

---

## §13 Next-cycle hooks (NOT triggered)

- (gated on Track CR Phase R2 / CQ Phase 1-3 install landing): real-load
  flip from cond.2 PASS at synthetic round-trip → cond.2 partial→met-
  candidate for `pe_d2r_mpq_archive_load` + `pe_d2r_savegame_d2s_format`
  upon real archive / `.d2s` IO observation.
- (gated on Track CR Phase R3 + R1): real first-frame Present flip from
  cond.3 partial sideways-PASS → cond.3 partial-with-evidence (still
  partial; D3DMetal binary licence permanent — no `met` promotion).
- (perpetually deferred per Track CN): hexa interp closure CM-26..CM-30
  unblocks `hexa run` of all 6 modules' self_test invocations — does NOT
  change paper-mode audit validity.
- (sideways-PASS S1 inert per Track L): online realm wire-protocol +
  cloud-sync .d2s — perpetually out-of-scope.
- (sideways-PASS S2 inert): `c_d2r_*` perf cluster honest annotation
  cycle — deferred until R3 + 30+ session evidence threshold.
- Future Track CX+1 candidate: D2R real_launch_phase → CQ Phase 1-3
  cross-link spec (formal documentation of which CQ phase landings
  unblock which D2R `pe_d2r_real_launch_phase` phase_kinds);
  alternatively D4-side per-module honest annotation extension (CV
  follow-up to cover any non-D3D12 D4 modules outside CV's 15-module
  scope).
