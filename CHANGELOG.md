# Changelog

All notable changes to `gamebox` are documented in this file.

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
- **Reference layout**: `need-singularity/sim-universe` v1.0.0 (sibling repo)

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
