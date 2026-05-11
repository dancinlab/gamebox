# gamebox Battle.net Roadmap — Track CQ Landed (2026-05-05)

> Track CQ on gamebox standalone repo, branch `main`. Doc-only cycle: produce
> a forward-looking, honest battle.net roadmap that **supersedes** prior
> battle.net plans. No module mutation; no validated_manjeom promotion; no
> hexa interp dependency assumed-resolved. All inflation refused.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CP final session closure marker 2026-05-05).
- Predecessor (immediate): Track CP (final session closure marker — zero remaining items).
- Predecessor (release bundle): Track CO (v1.0.4 release notes bundling CK/CL/CM/CN).
- Predecessor (deferral semantics): Track CN (hexa interp closure CM-26..CM-30 perpetual deferral ack — multi-year horizon).
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

This roadmap **supersedes** the following prior battle.net planning docs (each retains
historical value but is no longer the SSOT plan):

- `docs/BATTLENET_DAY1_PLAN.md` (2026-05-01 cycle 103, 8360-inst plateau era)
- `docs/BATTLENET_REAL_LAUNCH_PLAN.md` (2026-05-02 cycle 91+, with v2 Agent-bypass research-E supplement)
- `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` battle.net section (Phase 1-3 portion only — D2R/D4 specific frame phases out of scope here)

`docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` (Track P) is **not** superseded — it
is a permanent disambiguation reference, sister to the new doc.

---

## §0 Why this roadmap (purpose framing)

Two prior plans exist at finer granularity (Day 1-N, R3-R5 wave nomenclature) and
were authored at a **shim-cpp + inst-counter** abstraction layer (8360 inst plateau
era). The state of the world has since changed:

- Layout migrated to `<feature>/doc/` (Track CD).
- Loader cluster fanout settled: `lib/loader/(battlenet_bypass + 4×pe_battle_net_* + 4×pe_battlenet_*)` = 9 modules, all skeleton-tier with self_test only.
- Honest freeze family complete (VAC/Warden/EAC/GameGuard/D2R-realm) — anti-cheat surface acknowledged, never bypassed.
- Hexa interp closure CM-26..CM-30 explicitly marked **perpetually deferred** (Track CN). Real `hexa run` of any roadmap module is blocked at the runtime layer; **paper-mode derivation only**.

A forward-looking roadmap therefore needs to (a) describe **what the 9 battle.net loader modules actually do today vs. what they only enumerate**, (b) name the gaps honestly, (c) phase the work without false ETAs, (d) acknowledge the perpetual hexa-interp blocker explicitly.

---

## §1 Current honest posture (per-module skeleton vs validated)

### §1.1 The 9 battle.net loader cluster — module-by-module honest tier

All 9 modules pass their self_test under the schema-validated paper-mode lens. None
emit a real game frame. None have been confirmed-PASS by an actual `hexa run`
because the hexa interp binary is missing (Track CN perpetual deferral).

| Module | Surface today | Honest tier | Notes |
|---|---|---|---|
| `lib/loader/battlenet_bypass.hexa` | launcher-absence enumeration + feasibility matrix (D2R feasible / D4 NOT feasible — honest BLOCKED report) | **skeleton-tier**, own1 verdict PASS per Track P | Naming legacy "bypass" disambiguated permanently in Track P. NOT DRM evasion / NOT account auth bypass / NOT Warden circumvention. |
| `lib/loader/pe_battle_net_agent_ipc_track.hexa` | Agent IPC msg row tracker (4 transport_kind × 7 rpc_kind, TSV persist). Track-N synthetic 4-verb round-trip (PARTIAL emit) | **skeleton-tier** | Real `http://localhost:1120` round-trip permanently behind production smoke gate (Track-N cond.3 PARTIAL). |
| `lib/loader/pe_battle_net_download_track.hexa` | Download chunk tracker (6 phase_kind × 4 cdn_kind, TSV persist). manifest / chunk / SHA-256 verify / patch_apply / resume / cleanup phases. | **skeleton-tier** | No real CDN fetch. No actual SHA-256 of real bytes. |
| `lib/loader/pe_battle_net_oauth_token.hexa` | OAuth2 op tracker (6 op_kind: authorize / exchange / bearer / refresh / revoke / device_code). Token length-only (no real token values). | **skeleton-tier** | RFC6749 spec reference only. No real OAuth handshake. |
| `lib/loader/pe_battle_net_setup_install_phase.hexa` | Setup.exe install phase tracker (process_attach → copy → launchd_install → agent_spawn → CDN download → state register). | **skeleton-tier** | No real Setup.exe execution. Win32 service → macOS launchd plist mapping is enumerated, not exercised. |
| `lib/loader/pe_battlenet_agent_http_rest.hexa` | Agent.exe HTTP REST round-trip tracker (research-E reference: port 1120, User-Agent `phoenix-agent/1.0`, OAuth + launch + install endpoints) | **skeleton-tier** | Apple Network framework client not yet implemented; this module is the future instrumentation sink only. |
| `lib/loader/pe_battlenet_agent_sd_forging.hexa` | Agent.exe Security Descriptor shape tracker (DACL 3-entry, owner=Administrators, mask=0x1F01FF, MakeAbsoluteSD ↔ MakeSelfRelativeSD round-trip). 3-verb cond.1 PASS shim. | **skeleton-tier** | CrossOver Hack 23881 = algorithm-direction reference only, code lift 0. own1 maintained. |
| `lib/loader/pe_battlenet_setup_install_path_real.hexa` | Setup.exe end-to-end milestone tracker (PE validate → CRT init → WinMain → first dialog → install button → server connect → CDN → extract → complete). | **skeleton-tier** | No real install. |
| `lib/loader/pe_battlenet_setup_static_crt_path.hexa` | Static-CRT inline path tracker (PE validate → entry_point → security_cookie → _initterm → __scrt_common_main_seh → GetCommandLineA inline → WinMain → kernel32 first import → RegisterClassW → CreateWindowExW). | **skeleton-tier** | Battle.net Setup binary uses static-CRT linkage (MSVCR*.dll IAT entries 0). This module enumerates the inline byte-walk milestones but does not interpret them. |

### §1.2 Aggregate posture (all 9 modules)

- skeleton manjeom: full surface coverage, all self_test PASS schema-validated
- validated manjeom: **0** (no real game frame, no real Agent round-trip, no real OAuth handshake, no real CDN fetch)
- own1 carries: Wine 0 / DRM bypass 0 / account auth bypass 0 / Warden circumvention 0 / GameGuard bypass 0 / EAC kernel bypass 0 / VAC bypass 0 / FromSoft matchmaking bypass 0 / hexa-only / community wrapper 0
- own2 carries: honest emit, validated_manjeom=0 explicit, skeleton-tier vs real-tier distinction preserved

This is the honest baseline. The roadmap below describes **incremental** work needed
to flip individual surfaces from skeleton to validated, **without** bypassing the
hexa interp closure blocker.

---

## §2 Gap inventory (what is missing for a real darwin/arm64 bnet client launch)

A real Battle.net client launch on darwin/arm64 — under own1 — requires the
following independent capabilities. None are present today.

### §2.1 Runtime-tier blockers (hard prerequisites)

| Gap | What it blocks | Unblocking event |
|---|---|---|
| **G-1**: hexa interp binary missing (`/Users/ghost/core/hexa-lang/build/hexa_interp`) | All real `hexa run` of every module in this roadmap. Schema-validated paper-mode is the only available verification today. | hexa-lang sibling repo CM-26..CM-30 closure (Track CN multi-year horizon) |
| **G-2**: i386 PE32 interpreter beyond 8360-inst plateau | Real Battle.net Setup.exe entry → CRT init → WinMain → first NSWindow. Day1Plan §1 noted GetProcAddress fake-fnptr indirect-call dispatch back as the primary plateau cause. | shim-cpp work (out of hexa scope today; deferred until G-1 resolves so we can validate the i386 interp correctness end-to-end) |
| **G-3**: USER32/GDI32 → AppKit IPC bridge real round-trip | First NSWindow display from a real PE32 RegisterClassExA + CreateWindowExA call. helper.mm has the alloc path; shim has IPC kind=14/15 wiring; **call site reach = 0** today. | G-2 resolves (interp reaches USER32 IAT entries) |
| **G-4**: Apple Network framework HTTP/TLS client (real) | TLS handshake to `*.battle.net:443`, real CDN chunk fetch (G-CDN below), real Agent HTTP REST round-trip (G-Agent below). | independent of G-1; can be authored against pe_battlenet_agent_http_rest.hexa instrumentation sink. |

### §2.2 Functional gaps (each independently deliverable as a phase)

| Gap | Sketch |
|---|---|
| **G-CDN** | TACT/NGDP/CASC parser (Apple Compression LZMA + CommonCrypto MD5 + Network framework HTTPS). own1: own implementation, Wine cab/wininet 0. Pre-research direction-only documented in REAL_LAUNCH_PLAN v2. Authoring not started. |
| **G-Agent** | Agent.exe HTTP REST direct talk (port 1120, phoenix-agent/1.0). Single biggest path-shortener — bypasses launcher CEF GUI entirely while remaining own1 compliant (Agent IPC is a public protocol surface, not a bypass). Authoring not started. |
| **G-OAuth** | RFC6749 OAuth2 flow proxy (login.battle.net/oauth/{authorize,token,revoke,device}). Bearer token attach for us.api.blizzard.com endpoints. Authoring not started; spec reference only in pe_battle_net_oauth_token.hexa. |
| **G-Registry** | fake registry simulation (in-memory hash → ~/.airgenome/gamebox/fake_registry.tsv). Setup.exe writes HKLM\SOFTWARE\Blizzard Entertainment\Battle.net\* keys. Skeleton enumerated; runtime not exercised. |
| **G-FakeCdrive** | path translation: `C:\Program Files\Battle.net\` → `~/.airgenome/gamebox/c_drive/Program Files/Battle.net/`. CreateFileA → fake HANDLE wrap (fd \| 0x80000000). Skeleton enumerated; runtime not exercised. |
| **G-D3D** | D3D11/D3D12 → Apple Metal binding for D2R / D4 first frame. own1: Apple D3DMetal binary OK, Wine fork (GPTK/Whisky/CrossOver) refused. dx_d3d11_stage1 / dx_d3d12_stage2 skeleton landed; real frame draw 0. |

### §2.3 Permanent non-goals (carried forward)

These were settled by Track P + Track J + Track L + Track E + Track CK + Track CL,
and remain perpetually out-of-scope. This roadmap does **not** open them:

- DRM evasion / license circumvention / always-online check spoof (D4 retired_intentional posture)
- Account authentication bypass / OAuth bearer forge / pre-image attack
- Warden user-mode anti-cheat circumvention / hook chain shim / signature spoof / NOP-patch
- GameGuard kernel-mode anti-cheat bypass
- EAC kernel + anti-tamper bypass (Track CL honest freeze)
- VAC bypass (Track CK honest freeze)
- FromSoft matchmaking bypass (Track CL cross-class)
- D2R realm protocol session spoof (Track L freeze)
- Wine / CrossOver / Whisky / GPTK staging patch import / cite

---

## §3 Phased roadmap — 5 phases ordered by dependency

Each phase: scope, inputs, exit criteria, **honest tier expected at exit**, invariant impact.

> **Important**: Phases CQ-P1..CQ-P3 are independently authorable today even
> while G-1 (hexa interp) remains deferred — they author Hexa source modules
> that schema-validate but do not real-run. Phases CQ-P4..CQ-P5 require G-1 to
> have resolved first.

### Phase CQ-P1 — Apple Network framework HTTP/TLS client module

- **Scope**: New `lib/loader/pe_battlenet_apple_network_http.hexa` (or analogous) — record/track schema for HTTP request → TLS handshake → response chunk → SHA-256 verify against the existing pe_battle_net_download_track.hexa instrumentation sink. Not the actual Network framework binding; the **honest instrumentation surface** that a real client would emit into.
- **Inputs**: pe_battlenet_agent_http_rest.hexa (REST round-trip schema), pe_battle_net_download_track.hexa (chunk schema), pe_battle_net_oauth_token.hexa (op schema). All three already landed.
- **Exit criteria**: New module schema-validates self_test under paper-mode. own1 verdict PASS (no Wine wininet, no Apple-private API misuse). own2 emit count meets r0 threshold. CHANGELOG additive entry.
- **Honest tier at exit**: **skeleton-tier** (paper-mode validated, real-run blocked by G-1).
- **Invariant impact**: validated_manjeom=0 carries. Wine 0 carries. raw#9/11/12/15/175/270/271 enforce.

### Phase CQ-P2 — TACT/NGDP/CASC parser skeleton

- **Scope**: New `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa`. TSV-persisted phase tracker for: BLP/manifest fetch → BLTE chunk parse → MD5 verify → LZMA inflate → install_path write. Mirror style of pe_battle_net_download_track.hexa.
- **Inputs**: research-E TACT spec direction (already cited in REAL_LAUNCH_PLAN v2). own1 boundary: **no Wine cab / wininet code lift**, public spec only.
- **Exit criteria**: schema-validates, r0_emit ≥ threshold, panic format honest. own1 verdict PASS.
- **Honest tier at exit**: **skeleton-tier**.
- **Invariant impact**: identical to P1.

### Phase CQ-P3 — Agent HTTP REST direct-talk skeleton augment

- **Scope**: Augment existing `pe_battlenet_agent_http_rest.hexa` self_test with synthetic round-trip rows for the launcher-CEF-bypass mid-path (research-E core finding). 4-verb augment matching the existing N4 cond.3 PARTIAL pattern: install / launch / uninstall / heartbeat — but **PARTIAL emit explicit**, NOT PASS, until G-4 (real Apple Network framework client) lands.
- **Inputs**: existing module + Track-N4 round-trip pattern.
- **Exit criteria**: self_test PARTIAL row count ≥4, distinct-from-cond_1_pass_shim flag set, schema validates.
- **Honest tier at exit**: **skeleton-tier with PARTIAL augmentation** (does NOT promote to validated; real Agent endpoint round-trip permanently behind production smoke gate).
- **Invariant impact**: own2 PARTIAL discipline explicit; validated_manjeom=0 carries.

### Phase CQ-P4 — Real `hexa run` self_test confirmation pass (gated on G-1)

- **Scope**: When (and only when) the hexa-lang sibling repo lands CM-26..CM-30 and the hexa interp binary becomes present + functional, run `hexa run` against all 9 existing battle.net loader modules + the 2 new ones from P1/P2 + the augmented P3. Confirm every self_test PASS that paper-mode predicted.
- **Inputs**: G-1 resolved.
- **Exit criteria**: All 12 modules' self_test PASS observed via real interp execution. `state/markers/closure_progress_<ts>.marker` emitted from `tool/closure_orchestrator.hexa emit`.
- **Honest tier at exit**: **paper-mode prediction → real-execution-confirmed skeleton-tier**. Still **NOT validated-tier**: a successful self_test PASS demonstrates module shape integrity, not real-game first-frame behavior.
- **Invariant impact**: G-1 perpetual-deferral marker formally retired (replaced by a CQ-P4 land marker if/when triggered). validated_manjeom remains 0 (would be promoted only by P5).

### Phase CQ-P5 — Real first-byte / first-NSWindow / first-frame attempts (gated on G-1 + G-2 + G-3)

- **Scope**: Battle.net Setup.exe first NSWindow display attempt under real i386 PE32 interp. First TLS first-byte via real Apple Network framework. First Agent HTTP REST 200 OK for a real local Agent.exe (if installed). First D2R offline-mode first frame.
- **Inputs**: G-1 + G-2 + G-3 + G-4 + G-CDN + G-OAuth + G-Registry + G-FakeCdrive resolved.
- **Exit criteria** (per surface, each independently flippable):
  - first NSWindow on screen → flip CQ-P5a from skeleton to validated for module set {pe_battlenet_setup_install_path_real, pe_battlenet_setup_static_crt_path}
  - first real Agent HTTP 200 OK → flip CQ-P5b for {pe_battlenet_agent_http_rest, pe_battle_net_agent_ipc_track}
  - first real CDN chunk SHA-256 verified → flip CQ-P5c for {pe_battle_net_download_track, pe_battlenet_tact_ngdp_casc_track}
  - first real OAuth bearer attached to a us.api.blizzard.com call → flip CQ-P5d for {pe_battle_net_oauth_token}
  - first D2R offline first frame → flip the d2r_launch.hexa skeleton chain (per-game §9 closure path)
- **Honest tier at exit**: per-surface **validated-tier** flips. Aggregate posture moves from `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` toward `MIXED_TIER_VALIDATED_PARTIAL`. Full `VALIDATED_TIER_*` posture would still require 30+ hour gameplay measurement (per D2R_D4_INSTALL_LAUNCH_ROADMAP §6 Phase 5/6).
- **Invariant impact**: own1 Wine 0 still maintained throughout — every flip MUST be re-verified for own1 compliance at flip time. validated_manjeom promotion is per-surface and explicit; no aggregate inflation.

---

## §4 retired_intentional / sideways-PASS / honest-freeze posture (carry forward, do not reopen)

These are **already block-acknowledged** and behave as PASS-equivalent under the
mk2 checkpoint validator rule + cascading-flip pattern. Future Tracks should NOT
reopen them as work items:

| Posture | Surface | Source Track |
|---|---|---|
| retired_intentional | D4 (online-only DRM, server-side auth blocker — battlenet_bypass `bypass_feasibility("d4")` returns false honestly) | Track P + per-game §9 closure |
| retired_intentional | CS2 / Delta Force / Elden Ring / Lineage Classic R28 (cross-class anti-cheat surfaces — not battle.net family but related closure semantic) | Tracks CK/CL/etc. |
| honest_freeze | Warden user-mode anti-cheat | Track J |
| honest_freeze | GameGuard kernel-mode anti-cheat | Track E |
| honest_freeze | D2R realm protocol session spoof | Track L |
| honest_freeze | VAC | Track CK |
| honest_freeze | EAC kernel + anti-tamper | Track CL |
| honest_freeze | FromSoft matchmaking | Track CL cross-class |
| naming_disambiguation | `lib/loader/battlenet_bypass.hexa` "bypass" naming | Track P |
| perpetual_deferral_ack | hexa interp closure CM-26..CM-30 real-run validation hook 4 | Track CN |

All of the above carry sideways-PASS posture: forward Tracks must respect them as
"closed by acknowledgment" and may not author work that violates their boundary
(e.g., authoring a Warden hook chain shim is permanently forbidden, not deferred).

---

## §5 Open questions (no false confidence)

The following are explicit open questions. Each has no committed answer yet:

- **Q1**: Should Phase CQ-P1 (Apple Network framework HTTP/TLS sink) author a single combined module, or split into request-side + response-side + TLS-handshake-side trackers (mirroring pe_battle_net_download_track's TSV multi-table style)? Schema verbosity vs. composability tradeoff.
- **Q2**: Phase CQ-P2 (TACT/NGDP/CASC) — does the BLTE chunk parser belong with download_track (extension) or as its own module (separation of concerns)? Track P precedent suggests "do not modify existing modules; author new + cross-link" but each new module adds to G-1's blocked-surface count.
- **Q3**: Will the hexa interp closure CM-26..CM-30 land event ever actually arrive in a horizon that justifies CQ-P4..P5 pre-authoring effort? Track CN explicitly admits multi-year horizon. Pre-authoring P4/P5 detail today risks documentation rot. This roadmap deliberately keeps P4/P5 high-level.
- **Q4**: For the D2R offline path specifically — battlenet_bypass declares D2R feasible via offline-mode + cached profile, but does the actual D2R offline mode work with a token cache the user manually populates from a separate Battle.net install on a real Windows machine? own1 boundary: we never extract or forge tokens; we only **path-spec** the cache file location. Validating end-to-end requires user-side action that gamebox cannot automate.
- **Q5**: D4 is permanently retired_intentional (online-only). Should this roadmap include any D4-adjacent skeleton work at all (e.g., for the documentation-tier benefit of keeping the DXR pipeline knowledge fresh), or is the retired_intentional posture absolute? Current default: absolute.
- **Q6**: Should this roadmap supersede `D2R_D4_INSTALL_LAUNCH_ROADMAP.md` Phase 1-3 portion entirely, or only the battle.net-specific portion? The doc covers per-game frame phases (Phase 4-D2R / 4-D4 / Phase 5 gameplay) which are out of scope here. Default: supersede battle.net-specific portion only; per-game frame phases continue under per_game/§9 closure track.
- **Q7**: Naming convention for any new modules from P1/P2 — `pe_battlenet_*` vs `pe_battle_net_*` (existing cluster has both spellings, 4 each). New modules should pick one and be consistent with their semantic neighbor. Track CQ does **not** rename existing modules (additive_only).

---

## §6 in-place writes

| File | Op |
|---|---|
| (none) | — |

`in_place_writes`: 0 (cleanest doc-only roadmap supersede). NEW files: 2 (this handoff doc + Track CQ marker).

0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0 CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/ / 0 existing gamebox/doc/ / 0 existing docs/*.md mutation / 0 predecessor marker mutation / 0 gamebox-native marker mutation.

The 3 superseded docs remain on disk unchanged (historical reference). Future
readers should land on this Track CQ doc first.

---

## §7 policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (2 NEW files only)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (user directive for forward-looking roadmap paraphrased; no Korean directive verbatim quoted)
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no AC bypass / no DRM bypass / no anima clone / **no fake PASS / no fake real-run completion / no validated_manjeom inflation** ✓
- **own2**: forensic verdict honest unchanged; skeleton-tier vs validated-tier distinction preserved per phase; perpetual deferral semantics carried; validated_manjeom=0 carries ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓

---

## §8 forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

Track CQ is honest documentation-only roadmap supersede. NO module count change /
NO state mutation / NO validated_manjeom promotion / NO hexa interp dependency
assumed-resolved. Real-tier validation per phase remains gated as documented.

---

## §9 predecessors

- Track CP — `state/markers/gamebox_final_session_closure_track_cp_landed.marker` (immediate predecessor; final session closure marker)
- Track CO — `state/markers/gamebox_v1_0_4_release_notes_track_co_landed.marker` (v1.0.4 release bundle)
- Track CN — `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker` (perpetual deferral ack for hexa interp; primary blocker for CQ-P4..P5)
- Track CL — `state/markers/gamebox_eac_kernel_honest_freeze_track_cl_landed.marker`
- Track CK — `state/markers/gamebox_vac_honest_freeze_track_ck_landed.marker`
- Track P — `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` (sister doc; permanent disambiguation reference)
- Track AU — `state/markers/airgenome_gamebox_production_smoke_gate_design_track_au_landed.marker` (path A own1-maintained, multi-year horizon)

---

## §10 superseded docs (historical reference, not deleted)

| Path | Era | Supersession |
|---|---|---|
| `docs/BATTLENET_DAY1_PLAN.md` | 2026-05-01, cycle 103, 8360-inst plateau | superseded by §3 phased roadmap |
| `docs/BATTLENET_REAL_LAUNCH_PLAN.md` | 2026-05-02, cycle 91+, with v2 research-E supplement | superseded by §3 phased roadmap; v2 Agent-bypass insight preserved as G-Agent in §2.2 |
| `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` (battle.net portion) | 2026-05-01, cycle 27 | superseded for Phase 1-3 battle.net section only; Phase 4-6 per-game frame phases continue under per_game/§9 closure track |

The 3 superseded docs are not removed (additive_only). Track CQ becomes the SSOT
forward-looking battle.net plan; readers should consult Track CQ first and the
prior docs only for archaeological context.

---

## §11 next-cycle hooks

- **(perpetually deferred)**: G-1 hexa interp closure CM-26..CM-30 — sibling repo dependency, multi-year horizon (Track CN carry)
- **(NEW Track CQ+1 candidate)**: Phase CQ-P1 authoring (Apple Network framework HTTP/TLS sink module, schema-only)
- **(NEW Track CQ+2 candidate)**: Phase CQ-P2 authoring (TACT/NGDP/CASC parser skeleton module)
- **(NEW Track CQ+3 candidate)**: Phase CQ-P3 augmentation (Agent HTTP REST 4-verb PARTIAL augment in existing pe_battlenet_agent_http_rest.hexa)
- **(gated, NOT pre-authored)**: Phase CQ-P4 / CQ-P5 — materialize at G-1 trigger event time, per Track CN future cycle resumption protocol

CQ does NOT pre-author CQ-P4 / CQ-P5 details — they materialize when their gating
events arrive. CQ does NOT promise CQ-P1 / CQ-P2 / CQ-P3 timing — author when
prioritized.
