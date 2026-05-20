# gamebox v1.0.3 — Paper-Mode Validation + Inventory Cleanup + v3 Aggregate (2026-05-05)

## Summary

Patch release bundling post-v1.0.2 work:

- **Track CE** — closure_orchestrator selftest --quick paper-mode trace (S1-S11 stage-by-stage; LOW-to-MEDIUM all-pass prediction; BZ §11 hook partial resolution).
- **Track CF** — hexa.toml [modules] audit (40 entries, 39/40 OK, 1 drift item flagged).
- **Track CG** — `c_d2r_archive_round_trip` orphan reference removal (option b: hexa.toml + README perf families row); actual D2R MPQ archive functionality preserved in loader-class.
- **Track CH** — gamebox standalone session aggregate v3 narrative (first post-extraction aggregate; 14 named tracks BT-CG + 4 housekeeping commits + 1 layout migration = 18 cumulative).
- 2 housekeeping commits (CE post-mortem .gitignore + CG comment cleanup).

## Highlights

- **Real-invocation paper-mode** validated for `closure_orchestrator status` (BZ) + `selftest --quick` (CE). Both blocked by hexa interp closure CM-26..CM-30 missing; paper derivations carry consistent semantics.
- **hexa.toml audit drift cleared**: 1 → 0 (Track CG `c_d2r_archive_round_trip` orphan reference removal).
- **Session aggregate v3** establishes standalone-repo narrative class (first post-extraction aggregate, mirroring v1 Track AN + v2 Track BK predecessor airgenome session aggregates).
- **Forensic verdict (UNCHANGED)**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`. validated_manjeom = 0 carries.

## Changes

### Added (post-v1.0.2)

- `RELEASE_NOTES_v1.0.3.md` (this file).
- `gamebox/doc/gamebox_closure_orchestrator_selftest_paper_mode_track_ce_landed_2026_05_05.ai.md` (CE).
- `gamebox/doc/gamebox_hexa_toml_modules_audit_track_cf_landed_2026_05_05.ai.md` (CF).
- `gamebox/doc/gamebox_c_d2r_archive_round_trip_drift_fix_track_cg_landed_2026_05_05.ai.md` (CG).
- `gamebox/doc/gamebox_session_aggregate_v3_track_ch_landed_2026_05_05.ai.md` (CH).
- 4 NEW `gamebox_*` markers (CE + CF + CG + CH).
- `.gitignore` patterns: `resource_op_[0-9]*.marker` (CE post-mortem).
- hexa.toml `gamebox_perf_top_families` explanatory comment block (CG, references handoff doc).

### Changed

- `hexa.toml` `[package].version`: `1.0.2` → `1.0.3`.
- `hexa.toml` `gamebox_perf_top_families`: `c_d2r_archive_round_trip` orphan entry removed (CG).
- `README.md` Version badge: `version-1.0.2` → `version-1.0.3`.
- `README.md` perf families table: `c_d2r_archive_round_trip` row removed (CG).
- `CHANGELOG.md`: v1.0.3 section additive insertion above v1.0.2.

### Removed

- None. CG removed REFERENCES to `c_d2r_archive_round_trip` in inventory artifacts; no FILE deletion. Actual D2R MPQ archive functionality preserved in loader-class:
  - `lib/loader/pe_d2r_mpq_archive_load.hexa` (preserved)
  - `lib/loader/pe_d2r_savegame_d2s_format.hexa` (preserved)
  - `tests/test_d2r_archive_round_trip.hexa` (preserved, spawns 2 loader modules above)

### Posture

- **own1**: Wine 0 / hexa-only / DRM bypass 0 / Warden bypass 0 / anti-cheat bypass 0 / anima clone 0 / `destructive_ops: 0` (maintained throughout CE-CH; CG was reference-removal not file-deletion).
- **own2**: PASS/FAIL/PARTIAL/RETIRED_INTENTIONAL emit honest. validated_manjeom = 0 carries. Paper-mode vs real-run distinction explicit in CE + BZ. Drift fix rationale per option documented in CG.


5 explicit honesty caveats unchanged. CE adds an honest-deferred-run posture
to the orchestrator selftest surface (same blocker class as `cli/gamebox.hexa
launch <game>` — README §Caveats item 5 hexa interp closure CM-26..CM-30
missing).

## Cumulative state (post v1.0.3)

| metric | v1.0.0 | v1.0.1 | v1.0.2 | **v1.0.3** |
|---|---|---|---|---|
| .roadmap files | 14 | 14 | 15 | **15** (no change) |
| .checkpoint met | 0 | 3 | 3 | **3** (no change) |
| per_game §9 sections | 0 | 0 | 11 | **11** (no change) |
| gamebox-native markers | 1 | 7 | 9 | **15** (BT-CH) |
| RFC 016 audit cycles cumulative | 21 | 21 | 28 | **34** (CH narrative window CB-CG) |
| hexa.toml audit drift items | 1 | 1 | 1 | **0** (CG cleared) |
| Layout | flat docs/ | flat docs/ | `<feature>/doc/` | `<feature>/doc/` (no change) |
| Forensic verdict | UNCHANGED | UNCHANGED | UNCHANGED | UNCHANGED |

## Compatibility

No breaking changes. `gamebox` CLI behavior unchanged. Roadmap + checkpoint
JSONL schema unchanged. Loader/perf/tool inventory unchanged.

## Source repo

- Sibling: <https://github.com/dancinlab/sim-universe>
- Branch: `main` (post fast-forward merge of `bg/layout-migration-2026-05-05`).

Predecessor releases:
- v1.0.0 — initial extraction (BT)
- v1.0.1 — drift resolution + win32 milestone (BU + BV)
- v1.0.2 — layout migration + inventory refresh + per_game §9 closure (1d28b4c + BX + BY + BZ + CA + CB + CC)
- **v1.0.3 — paper-mode validation + inventory cleanup + v3 aggregate (CE + CF + CG + CH)** (this release)

## Author

박민우 <nerve011235@gmail.com>
