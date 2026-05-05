# D2R Phase — Install Path Detect + .d2s Savegame Folder Probe (Paper-Mode Pre-Flight, Track CU Landed 2026-05-05)

> Track CU on gamebox standalone repo, branch `main`. First executable doc-tier
> phase from the Track CR D2R roadmap (`docs/D2R_ROADMAP_TRACK_CR.ai.md` §12
> "Next-cycle hooks" — explicit future-CR+1 candidate). Paper-mode pre-flight
> path inventory only — does NOT trigger filesystem probes, does NOT require
> hexa interp closure (CM-26..CM-30 perpetual deferral preserved per Track CN),
> does NOT require Battle.net agent acquisition (Track CR Phase R2 / Track CQ
> Phase 1-3), does NOT require D2R install on disk. depends_on=[CR, CT];
> CT cross-dep cardinality only — D2R offline path does not require bnet auth.

---

## §0 Executable phase selection rationale

The Track CR roadmap (`docs/D2R_ROADMAP_TRACK_CR.ai.md`) defines 3 forward
phases (R1 / R2 / R3) and 2 sideways-PASS rails (S1 / S2). Track CU
audited each for doc-tier executability today:

| phase | gating | doc-tier executable today? |
|---|---|---|
| R1 hexa interp closure | CM-26..CM-30 sibling repo (multi-year per Track CN) | NO — perpetually deferred runtime gate |
| R2 battle.net agent acquisition | Track CQ Phase 1-3 (i386 PE map + WININET CDN runtime) | NO — depends on real install path artefacts |
| R3 D2R first frame | R1 + R2 + D3DMetal binary licence | NO — gated on R1+R2 |
| S1 online realm sideways-PASS | Track L SSOT inert | NO — modification policy forbids re-open |
| S2 skeleton perf carry | R3 + 30+ session evidence | NO — validated promotion gate not reached |

The CR §12 explicitly enumerates a **future-CR+1 doc-tier candidate**
that bypasses every runtime / external-binary gate above:

> Future Track CR+1 candidate: D2R install path detect + .d2s savegame
> folder probe (paper-mode pre-flight) — does NOT require R1+R2 (probe
> only emits paths)

Track CU adopts this exact phase as its scope. Output is a path-inventory
specification that downstream cycles (when D2R install lands via R2) can
consume to drive real probe runs without re-deriving canonical layout.
Until then, the spec carries skeleton-tier honest emission only — no
filesystem traversal, no path existence assertions, no `met` promotion.

---

## §1 Scope (paper-mode pre-flight = path inventory spec only)

**In scope (Track CU)**:
- Canonical D2R install path candidates on darwin/arm64 under Battle.net
  agent install conventions (paper-mode reference; no probe execution).
- `.d2s` savegame folder canonical layout per Battle.net agent + classic
  Diablo 2 conventions (paper-mode; no file enumeration).
- MPQ archive expected file-set per `pe_d2r_mpq_archive_load.hexa`
  archive_kind enum (8 archives) — file-name canonical + override
  precedence reference.
- Paper-mode self-test contract (probe entry point signatures + verdict
  schema) for downstream cycle when R2 lands D2R on disk.

**Out of scope (Track CU)**:
- Real filesystem probe execution (no `stat`, no `ls`, no `test -d`).
- D2R.exe PE32+ inspection (gated on R2 — install required).
- MPQ header read / Storm.dll wrapper invocation (gated on R2 + R3).
- `.d2s` AES-256 decrypt / character data parse (gated on R3 + Phase R3
  hexa-only AES path verification — D2R AES variant per CR §6 OQ#3 still
  open).
- Online realm savegame cloud-sync probe (Track L sideways-PASS inert —
  out of scope perpetually).
- Battle.net Setup.exe install path (Track CQ scope; Track CU references
  but does NOT duplicate).

---

## §2 Canonical D2R install path candidates (paper-mode)

Battle.net agent on darwin installs games under user-controlled root
(default `/Applications/Diablo II Resurrected/` or user-selected path).
Tracking 4 candidate roots in priority order:

| priority | candidate root | source |
|---|---|---|
| 1 | `/Applications/Diablo II Resurrected/` | Battle.net agent darwin default |
| 2 | `${HOME}/Applications/Diablo II Resurrected/` | user-scoped non-admin install |
| 3 | `${BLIZZARD_INSTALL_ROOT}/Diablo II Resurrected/` | env-override (Battle.net agent setting) |
| 4 | user-supplied path via `validate_d2r_path` (already in `lib/loader/d2r_launch.hexa`) | manual override |

Under any candidate root, expected child layout (paper-mode reference;
NOT verified against live install — probe execution = R2 cycle):

```
<root>/
├── Diablo II Resurrected.app/                  (macOS app bundle wrapper if Battle.net agent darwin variant)
│   └── Contents/
│       └── MacOS/                              (potential native macOS launcher placeholder)
├── Diablo II Resurrected.exe                   (PE32+ x86_64 binary — R3 entry target)
│   ├── (or under <root>/x86_64/ subfolder per Battle.net layout)
├── data/
│   ├── data/                                   (legacy Diablo 2 base data carry-over)
│   ├── hd/                                     (D2R HD asset overlay)
│   └── local/                                  (locale-specific files)
├── mods/                                       (community mod folder — Track CU honors but does NOT enumerate; own1)
├── Save/                                       (.d2s savegame folder per §3)
├── Patch_D2.mpq                                (override priority highest per pe_d2r_mpq_archive_load archive_kind=7)
├── d2data.mpq                                  (archive_kind=1)
├── d2exp.mpq                                   (archive_kind=2 — Lord of Destruction)
├── d2sfx.mpq                                   (archive_kind=3)
├── d2speech.mpq                                (archive_kind=4)
├── d2music.mpq                                 (archive_kind=5)
├── d2char.mpq                                  (archive_kind=6 — HD character art)
└── hd_textures.mpq                             (archive_kind=8 — D2R HD remaster pack)
```

Honest annotation: classic D2 + LoD MPQ family carries over; D2R adds
`hd_textures.mpq` + repurposes `d2char.mpq` as HD asset bundle. Patch
override precedence: `Patch_D2.mpq` > all base archives (verified by
Storm.dll archive search order; cited from public Mike O'Brien MPQ format
spec, not from third-party MPQ extractor source).

---

## §3 `.d2s` savegame folder canonical layout (paper-mode)

Per `lib/loader/pe_d2r_savegame_d2s_format.hexa` op_kind=1/2 (read/write)
+ section_kind=1..7 (header_magic / character_data / skills /
stash_personal / stash_shared / quests / waypoints).

Save folder under D2R install root or per-user OS path:

| candidate | path | scope |
|---|---|---|
| install-local | `<install_root>/Save/` | Battle.net agent default (offline SP) |
| user-OS darwin | `${HOME}/Library/Application Support/Diablo II Resurrected/Save/` | macOS conventional (D2R darwin variant) |
| Battle.net cloud-shadow | local cache for cloud-sync (op_kind=5/6) | Track L sideways-PASS — out of scope |

Expected file set under `Save/`:

```
Save/
├── <character_name>.d2s          (per-character savegame; AES-256 encrypted in D2R)
├── <character_name>.key          (legacy D2 keychain — D2R may carry-over)
├── <character_name>.ma0..ma7     (legacy D2 mercenary slots — carry-over)
├── SharedStashSoftCoreV2.d2i     (shared stash softcore — section_kind=5)
├── SharedStashHardCoreV2.d2i     (shared stash hardcore)
└── settings.json                 (D2R UI / control bindings)
```

Honest annotation:
- `.d2s` header magic 0xAA55AA55 + version 0x60 (D2R variant) per
  `pe_d2r_savegame_d2s_format` self-test synthetic round-trip (cond.2
  PASS at synthetic format level; real-load = R3 + AES path).
- AES-256 encryption layer per Track CR §6 OQ#3 — vanilla AES-256-CBC vs
  Blizzard variant remains open question (out-of-scope per Track L
  spirit if proves to require RE; gamebox lib AES-256 primitive applies
  to vanilla path only).
- Character data 16 byte block (level / class / etc) per
  `pe_d2r_savegame_d2s_format` section_kind=2 contract.

---

## §4 Probe entry point signature contract (downstream consumer schema)

When R2 lands and `validate_d2r_path` (already in `d2r_launch.hexa`) is
extended to a full filesystem probe, the downstream cycle should adopt
this signature contract (paper-mode spec; NO emission today):

```
fn d2r_probe_install_root(candidate: str) -> array
  // returns [exists_flag<bool>, mpq_count<int>, save_dir_present<bool>]
  // exists_flag = root candidate is a directory
  // mpq_count = count of expected 8 MPQ archive_kinds present
  // save_dir_present = Save/ subdir present (offline SP gate)
  // own1: NO descent into mods/ (community surface — not enumerated)
  // own2: silent_error 0 — missing root returns [false, 0, false] honestly

fn d2r_probe_save_dir(save_root: str) -> array
  // returns [d2s_count<int>, shared_stash_count<int>, settings_present<bool>]
  // d2s_count = enumeration of *.d2s under save_root (op_kind=1 read surface)
  // shared_stash_count = SharedStash{SoftCore,HardCore}V2.d2i present count
  // settings_present = settings.json presence flag
  // own1: NO file content read — only path enumeration
  // own2: silent_error 0 — missing save_root returns [0, 0, false] honestly

fn d2r_probe_mpq_inventory(install_root: str) -> array
  // returns array of [archive_kind<int>, present<bool>, size_mb<int>]
  // archive_kind = 1..8 per pe_d2r_mpq_archive_load enum
  // present = mpq file path exists under install_root
  // size_mb = file size / 1MB (no header read; file_size only)
  // own1: NO Storm.dll wrapper, NO MPQ header parse
  // own2: synthetic round-trip in pe_d2r_mpq_archive_load already PASS at format level
```

These signatures are paper-mode reference only. Track CU does NOT add
them to `lib/loader/d2r_launch.hexa` or any pe_d2r module — that is
Track CU+1 (post-R2 install) work. own1 enforced: no premature emission.

---

## §5 Honest tier (skeleton-tier carry, no validated_manjeom promotion)

Track CU = **documentation-tier** only. No module surface change. No
filesystem touch. No real probe. The path inventory spec exists as
forward-reference for downstream cycle when R2 lands real install on
disk. Until then:

- `pe_d2r_savegame_d2s_format` cond.2 = PASS at synthetic format level
  carries (no flip).
- `pe_d2r_mpq_archive_load` cond.2 = PASS at synthetic round-trip level
  carries (no flip).
- `pe_d2r_specific_track` cond.1 + `pe_d2r_real_launch_phase` cond.1 =
  partial carries (no `partial→met-candidate` flip — that requires R2
  install).
- `pe_d2r_d3d11_renderer_init` cond.3 = partial sideways-PASS (Track L
  pattern; D3DMetal binary licence permanent transitive).
- validated_manjeom = 0 carries.
- Wine count = 0.
- Forensic verdict UNCHANGED: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## §6 Cross-deps (CR + CT cardinality only)

Track CU depends_on = [CR, CT]:

| predecessor | reason |
|---|---|
| CR | D2R roadmap doc-tier predecessor (this phase explicitly cited in CR §12 as future-CR+1 candidate) |
| CT | track sequence cardinality only — D2R offline single-player path does NOT require Battle.net authentication / online realm; CT (whatever its scope under battle.net Track CQ followup) does not block D2R offline probe spec; cited solely to preserve alphabetic Track ordering CQ→CR→CS→CT→CU |

D2R offline SP path independence: `validate_d2r_path` already accepts a
manual path override (priority 4 in §2). Real install via Battle.net
agent (Track CR Phase R2 / Track CQ Phase 1-3) is one acquisition route;
manual user-supplied install (e.g. existing Battle.net install on
external volume, or Boot Camp dual-boot mount) is another. Both feed
the same probe entry point contract in §4.

---

## §7 Hard invariants carried forward

- **own1**: Wine 0 / hexa-only / no community wrapper / no MPQ extractor
  third-party tool / no community .d2s parser / no AC bypass / no DRM
  bypass / no MITM / no realm protocol implementation / no anima clone /
  no descent into mods/ folder.
- **own2**: honest emit / silent_error 0 / validated_manjeom = 0 carry /
  skeleton-tier vs real-tier distinction preserved / Track L sideways-
  PASS preserved / no premature module emission (paper-mode spec only).
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce.
- **Forensic verdict (UNCHANGED)**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- **Hexa interp closure CM-26..CM-30**: perpetually deferred per Track
  CN; paper-mode derivation acceptable for status checks; runtime launch
  blocked. Track CU output is paper-mode pre-flight spec — does NOT
  require hexa interp closure.

---

## §8 In-place writes

| File | Op |
|---|---|
| (none) | — |

`in_place_writes`: 0 (cleanest doc-only sub-phase cycle).
NEW files: 2 (this handoff doc + Track CU marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0
native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE
/ 0 CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/
/ 0 existing gamebox/doc/ / 0 existing docs/*.md / 0 predecessor marker
mutation / 0 gamebox-native marker mutation / 0 lib/loader/d2r_*.hexa
or pe_d2r_*.hexa.

---

## §9 Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (cleanest cycle — 2 NEW files only; 0 in-place writes)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (no user Korean directives quoted; intent paraphrased only)
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no AC bypass / no DRM bypass / no MITM
  / no realm protocol implementation / no anima clone / no community
  MPQ tool / no community .d2s parser ✓
- **own2**: forensic_verdict honest unchanged; D2R real frame surface =
  0 explicit; sideways-PASS posture for online realm explicit;
  validated_manjeom = 0 carries; skeleton-tier vs real-tier distinction
  preserved; paper-mode-only spec (no premature module emission) ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓

---

## §10 Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CU is paper-mode pre-flight spec doc only. NO module count change / NO
state mutation / NO validated_manjeom promotion / NO online realm scope
re-opening / NO probe execution. Real D2R install probe + .d2s savegame
enumeration on darwin/arm64 = perpetually deferred sub-phase pending
R2 install landing.

---

## §11 Predecessors

- Track CT — `state/markers/<CT marker>` (immediate track sequence
  predecessor; cardinality only — D2R offline does not require bnet auth)
- Track CR — `state/markers/d2r_roadmap_track_cr_landed.marker` (D2R
  roadmap SSOT; this phase explicit in §12 as future-CR+1 doc-tier
  candidate)
- Track CQ — `state/markers/battlenet_roadmap_track_cq_landed.marker`
  (battle.net forward roadmap; downstream Phase R2 dependency)
- Track CO — `state/markers/gamebox_v1_0_4_release_notes_track_co_landed.marker`
  (v1.0.4 release predecessor)
- Track CN — `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker`
  (R1 hexa interp perpetual deferral lineage)
- Track L — `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` (online realm
  honest freeze — sideways-PASS source preserved)
- `lib/loader/d2r_launch.hexa` — `validate_d2r_path` synthetic surface
  cited in §2 priority 4 + §4 entry point contract baseline
- `lib/loader/pe_d2r_savegame_d2s_format.hexa` — section_kind enum cited
  in §3 + op_kind enum cited in §4
- `lib/loader/pe_d2r_mpq_archive_load.hexa` — archive_kind enum cited
  in §2 file set + §4 mpq inventory contract

---

## §12 Next-cycle hooks (NOT triggered)

- (gated on Track CR Phase R2 = Track CQ Phase 1-3 install landing):
  real probe entry-point emission per §4 signature contract — adds
  `d2r_probe_install_root` / `d2r_probe_save_dir` /
  `d2r_probe_mpq_inventory` to a new `lib/loader/d2r_probe.hexa` module
  or extends `d2r_launch.hexa` (decision deferred to that cycle).
- (gated on R3 + AES path verification): `.d2s` AES-256 decrypt sub-
  phase — Track CR §6 OQ#3 (vanilla CBC vs Blizzard variant) must
  resolve first; if Blizzard variant + RE required, sideways-PASS via
  Track L spirit applies.
- (perpetually deferred per Track CN): hexa interp closure CM-26..CM-30
  unblocks `hexa run` of probe self_test invocations — does NOT change
  paper-mode spec validity.
- (sideways-PASS S1 inert per Track L): online realm cloud-sync save
  probe — perpetually out-of-scope unless user re-opens.
- Future Track CU+1 candidate: D4 install path probe paper-mode spec
  (analogous sub-phase under Track CS D4 roadmap §12 if such a hook
  exists; if not, novel sub-phase audit candidate for D4 always-online
  posture context).
