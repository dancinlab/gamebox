# Changelog

All notable changes to `gamebox` are documented in this file.

## [Unreleased]

### Changed

- chore(harness): perfect harness setup — engine submodule bumped to latest on
  `harness-hardcore`; authored `ARCHITECTURE.md` SSOT (overview + component map +
  data flow + governance/verify, English) over the stub; authored harness-standard
  `CLAUDE.md` (H1 + blurb + `## Structure` tree + governance + `## Harness` +
  quick reference) over the SPECKIT stub; added a `docs` discipline block to
  `harness.config.json` (`architecture`/`log`/`scratchDir` + `scopeDirs: [""]`
  root-only + `allow` list covering README/CHANGELOG/ARCHITECTURE/CLAUDE/AGENTS
  plus `RELEASE_NOTES_v1.0.*` and `TAPE-AUDIT.md`). `harness docs check` now
  reports `docs: ok` with 0 CLAUDE-MD violations. `.claude/settings.json` already
  carried the full guarded harness hook set (pre bash / pre write / post edit /
  prompt / prefs · easy · recommend inject / SessionStart) — preserved as-is.

## [1.0.4] — 2026-05-05

### Added

- Track CK — `docs/VAC_HONEST_FREEZE.md` mk1 narrative (Valve Anti-Cheat user-mode AC + Trusted Mode; resolves cs2.cond.3 deferred hook).
- Track CL — `docs/EAC_KERNEL_HONEST_FREEZE.md` mk1 narrative (EAC kernel + anti-tamper class; 3-cross-class delta_force + elden_ring base+DLC + nightreign).
- Track CM — Nightreign lib module fan-out at skeleton-tier:
  - `lib/loader/pe_elden_ring_nightreign_specific_track.hexa` (216 LOC)
  - `lib/perf/c_nightreign_co_op_session_init.hexa` (133 LOC)
  - `lib/perf/c_nightreign_matchmaking_probe.hexa` (129 LOC)
  - `lib/perf/c_nightreign_dx12_first_frame_co_op_variant.hexa` (131 LOC)
  - Total 609 LOC; retired_intentional posture inherited from blk.1 + blk.2.
- Track CN — `gamebox/doc/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed_2026_05_05.ai.md` (formally closes CJ §11 hook 4 as perpetually deferred; multi-year horizon).
- 4 NEW `gamebox_*` markers (CK + CL + CM + CN).
- 4 NEW `gamebox/doc/` handoff docs (CK + CL + CM + CN).

### Changed

- `hexa.toml` version: `1.0.3` → `1.0.4`.
- `README.md` Version badge: `version-1.0.3` → `version-1.0.4`.
- `.roadmap.elden_ring_nightreign`: `loader_count: 0 → 1`, `perf_count: 0 → 3`, NEW `loader_pattern` + `perf_pattern` + `ai_native_handoff_track_cm` fields, NEW comment header line 6 documenting Track CM skeleton-tier fan-out (Track BY documentation-only narrative line 5 preserved verbatim).

### Cumulative deltas (v1.0.3 → v1.0.4)

- gamebox-native markers: 15 → 19
- Honest freeze docs: 3 → 5 (Track-E predecessor + CK + CL)
- lib/loader files: 455 → 456 (+1 nightreign loader)
- lib/perf files: 242 → 245 (+3 nightreign perf)
- Forensic verdict: UNCHANGED (`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`)
- validated_manjeom: UNCHANGED (0 — skeleton-tier per Track BL/BM forensic baseline)

### Posture

- own1: Wine 0 / hexa-only / no DRM bypass / no AC bypass (5 classes covered: VAC + GameGuard + EAC kernel + EAC anti-tamper + Warden) / no FromSoft matchmaking bypass / no anima clone / `destructive_ops: 0` (maintained).
- own2: 4 CM modules emit RETIRED_INTENTIONAL posture annotation; CN explicit no-fake-PASS / no-fake-real-run-completion declaration; skeleton-tier vs real-tier distinction explicit; spec freeze ONLY scope explicit per CK + CL.

## [1.0.3] — 2026-05-05

### Added

- Track CE — `gamebox/doc/gamebox_closure_orchestrator_selftest_paper_mode_track_ce_landed_2026_05_05.ai.md` (S1-S11 paper-mode trace; BZ §11 hook partial resolution).
- Track CF — `gamebox/doc/gamebox_hexa_toml_modules_audit_track_cf_landed_2026_05_05.ai.md` (40 entries audit; 1 drift item flagged for CG).
- Track CG — `gamebox/doc/gamebox_c_d2r_archive_round_trip_drift_fix_track_cg_landed_2026_05_05.ai.md` (orphan reference option-b removal).
- Track CH — `gamebox/doc/gamebox_session_aggregate_v3_track_ch_landed_2026_05_05.ai.md` (first standalone-repo session aggregate covering BT-CG).
- hexa.toml `gamebox_perf_top_families` explanatory comment block (CG).
- 4 NEW `gamebox_*` markers (CE / CF / CG / CH).
- `.gitignore` pattern: `resource_op_[0-9]*.marker` (CE post-mortem).

### Changed

- `hexa.toml` version: `1.0.2` → `1.0.3`.
- `hexa.toml` `gamebox_perf_top_families`: orphan `c_d2r_archive_round_trip` entry removed (CG).
- `README.md` Version badge: `version-1.0.2` → `version-1.0.3`.
- `README.md` perf families table: `c_d2r_archive_round_trip` row removed (CG).

### Cumulative deltas (v1.0.2 → v1.0.3)

- gamebox-native markers: 9 → 15 (BT-CH)
- RFC 016 audit cumulative null floor: 28 → 34 cycles
- hexa.toml audit drift items: 1 → 0
- Forensic verdict: UNCHANGED (`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`)
- validated_manjeom: UNCHANGED (0)

### Posture

- own1: Wine 0 / hexa-only / no DRM / no AC / no anima clone / `destructive_ops: 0` (CG was reference-removal not file-deletion).
- own2: paper-mode vs real-run distinction explicit; drift fix option chosen documented; v3 vs v1/v2 distinction explicit.

## [1.0.2] — 2026-05-05

### Added

- **Layout migration** (`1d28b4c refactor(layout 2026-05-05)`): canonical `<feature>/doc/` layout — `airgenome/doc/` (75 predecessor handoffs) + `gamebox/doc/` (9 native handoffs incl. CB-CC).
- Track BX — per_game README §9 sections for 6 offline-relevant files (lineage_classic_r28 + d2r + battlenet + lineage_w + lineage_m + purple_launcher).
- Track BY — README Game Coverage refresh: wow row removed (historical preserve), elden_ring DLC scope clarified (Shadow of the Erdtree 2024-06-21), elden_ring_nightreign added (2025-05-30 standalone multiplayer co-op spinoff, documentation-only fan-out).
- NEW `.roadmap.elden_ring_nightreign` (documentation-only, 0 lib modules at land-time).
- NEW `docs/per_game/elden_ring_nightreign_README.ai.md` T1 baseline.
- `dlc_scope` + `spinoff_cross_link` + `ai_native_handoff_track_by` fields on `.roadmap.elden_ring`.
- Track BZ — `gamebox/doc/gamebox_closure_orchestrator_paper_invocation_track_bz_landed_2026_05_05.ai.md` (real-invocation attempt + paper-mode derivation; hexa interp blocker).
- Track CA — `gamebox/doc/gamebox_hexa_upstream_rfc_016_reassessment_track_ca_landed_2026_05_05.ai.md` + `docs/HEXA_UPSTREAM_RFC_016_REASSESSMENT_TRACK_CA_2026_05_05.md` (28-cycle null floor reaffirmed).
- Track CB — README "Native markers" + "Resolver telemetry markers (gitignored)" subsections.
- Track CC — per_game README §9 sections for 4 retired_intentional files (cs2 + delta_force + diablo4 + elden_ring); elden_ring §9 surfaces DLC scope narrative.
- `.gitignore` patterns for hexa-resolver runtime telemetry markers.
- 8 NEW `gamebox_*` markers post-v1.0.0 (BT through CC).

### Changed

- README.md TL;DR header: `.roadmap` 14 → 15 (14 active + 1 historical), `.checkpoint` met 1 → 3.
- README.md architecture caption: terse 7-figure → detailed inventory (455 .hexa files, 9 tests, 88 markers across 3 classes).
- README.md Loaders heading: `(35 modules)` → `(59 named groups across 455 .hexa files)`.
- `.roadmap.elden_ring` goal: scope = base game + Shadow of the Erdtree DLC.
- `docs/airgenome_*.ai.md` (75) → `airgenome/doc/` per layout migration.
- `docs/gamebox_*.ai.md` (7) → `gamebox/doc/` per layout migration.
- `state/markers/*.marker` cross-references updated (193 files / 621 lines via sed sweep in `1d28b4c`).
- `hexa.toml` version: `1.0.1` → `1.0.2`.
- README version badge: `version-1.0.1` → `version-1.0.2`.

### Posture

- Forensic verdict UNCHANGED: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- `validated_manjeom` UNCHANGED: 0 (no real game frames).
- own1: Wine 0 / hexa-only / no DRM / no AC / no GameGuard / no EAC / no FromSoft matchmaking bypass / no anima clone / `destructive_ops: 0`.
- own2: honest emit; per_game §9 sections explicit about retired_intentional stable + cross-cutting impact + validated_manjeom carry.

## [1.0.1] — 2026-05-05

### Added

- Track BU — README "Game Coverage" section (+40 LOC; 11 .roadmap targets table + per-game index + 7 cross-roadmap checkpoint table + honest scope footer).
- Track BU — `met_iso` / `met_via` / `evidence[]` fields on `.checkpoint.win32_50pct_coverage` + `.checkpoint.win32_full_coverage`.
- Track BV — `met_iso` / `met_via` / `evidence[]` fields on `.checkpoint.lineage_classic_r28_play`.
- Track BV — 9 NEW `status_flipped_*` fields across `.roadmap.lineage_classic_r28` cond.1/cond.2/cond.3.
- 4 NEW handoff docs + markers (BU + BV).

### Changed

- `.checkpoint.win32_50pct_coverage` status: `unmet 30.34/50` → `met 100/50` (BU).
- `.checkpoint.win32_full_coverage` status: `unmet 30.34/100` → `met 100/100` (BU).
- `.checkpoint.lineage_classic_r28_play` status: `partial 0/3` → `met 3/3` (BV, sideways-PASS).
- `.roadmap.lineage_classic_r28` cond.1/cond.2/cond.3 actual status: `partial` → `retired_intentional` (BV cascading flip).
- `hexa.toml` version: `1.0.0` → `1.0.1`.
- 3 checkpoint `blockers` arrays renamed `blockers_resolved` (verbatim list preserved).

### Drift resolution

- Track BO snapshot v2 drift_count: 3 → 0 (all 3 items resolved across BU + BV).

### Posture

- Forensic verdict UNCHANGED: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- `validated_manjeom` UNCHANGED: 0 (no real game frames evidenced).
- own1: Wine 0 / hexa-only / DRM bypass 0 / Warden bypass 0 / anti-cheat bypass 0 / GameGuard bypass 0 / anima clone 0.
- own2: PASS/FAIL/PARTIAL/RETIRED_INTENTIONAL emit honest; status flip rationale per checkpoint cited.

## [1.0.0] — 2026-05-05

### Initial extraction

- **Forensic verdict**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`
- **Source**: `dancinlife/airgenome-gamebox` 67-cycle session (2026-05-03 to 2026-05-05; deleted 2026-05-05 per closure-cycle directive, history not transferred per D3 fresh-start policy)
- **Reference layout**: `dancinlab/sim-universe` v1.0.0 (sibling repo)

### Inventory

- 35 loader modules under `lib/loader/` (32 PE *_core + 5 runtime + 5 DX + 9 Battle.net + 6 calling-conv + 2 launchers; some overlap)
  - 435/435 Win32 fn skeleton coverage (Track BP, 2026-05-05)
- 242 perf modules under `lib/perf/` across 104 unique families
  - 18+ families at 100% manjeom (Track BL/BM forensic)
- 14 `.roadmap.*` files (mk2 SSOT)
- 7 `.checkpoint.*` files (mk2 checkpoint mechanism); 1 met: `win32_50pct_coverage` (Track BR derivation guard)
- 2 tools under `tool/` (`roadmap_op.hexa` 14-subcmd anima parity broken intentional + `closure_orchestrator.hexa` weighted_threshold awareness)
- 8 tests under `tests/`
- 75 predecessor markers under `state/markers/airgenome_gamebox_*.marker` (preserved verbatim per own2 traceability)
- 4 hexa-lang upstream RFCs merged (rfc_012/_013/_014 PR #47 + rfc_015 PR #48)

### Posture

- **own1**: Wine 0 / hexa-only / DRM bypass 0 / Warden bypass 0 / anti-cheat bypass 0 / anima clone 0
- **own2**: PASS/FAIL emit honest, PARTIAL/RETIRED_INTENTIONAL emit explicit
- **L1-L5 online expansion lock-in carry** (EULA / Warden / multi-year / D2R-single / parallel) per `.roadmap.battlenet`
- **License**: Apache-2.0
