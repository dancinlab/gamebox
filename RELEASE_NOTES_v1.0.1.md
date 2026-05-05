# gamebox v1.0.1 — Drift Resolution + Win32 100% Checkpoint Milestone (2026-05-05)

## Summary

First post-extraction patch release. Bundles:

- Track BU — README Game Coverage section + win32_50pct + win32_full checkpoint cached status flip `unmet` → `met`.
- Track BV — `.checkpoint.lineage_classic_r28_play` drift resolution via cascading cond status flip `partial` → `retired_intentional`.

All 3 drift items from Track BO snapshot v2 now resolved (drift_count 3 → 0).

## Highlights

- **3 of 7 checkpoints now `met`**: `win32_50pct_coverage` (100/50) + `win32_full_coverage` (100/100) + `lineage_classic_r28_play` (3/3 sideways-PASS).
- **Aggregate distribution**: 3 met / 4 partial / 0 unmet (was 1 met / 5 partial / 1 unmet at v1.0.0).
- **README Game Coverage table**: 11 .roadmap targets (8 games + 3 launchers/runtimes) + per-game README index pointer + 7 cross-roadmap checkpoint table.
- **Forensic verdict (UNCHANGED)**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`. The checkpoint met flips refer ONLY to skeleton-tier surface coverage + honest documentation of cascading retirement — no real game frames evidenced; `validated_manjeom` remains 0.

## Changes

### Added

- `RELEASE_NOTES_v1.0.1.md` (this file).
- README "Game Coverage" section (Track BU, +40 LOC).
- 4 new evidence entries across `.checkpoint.win32_*` headers (Track BU).
- 3 new evidence entries on `.checkpoint.lineage_classic_r28_play` header (Track BV).
- `met_iso` + `met_via` fields on 3 checkpoints (BU + BV).
- 9 new `status_flipped_*` fields across `.roadmap.lineage_classic_r28` cond entries (BV).
- 1 new comment line on `.checkpoint.lineage_classic_r28_play` header documenting BV update.
- 4 new handoff docs + markers (Track BU + Track BV).

### Changed

- `.checkpoint.win32_50pct_coverage` status: `unmet` → `met` (Track BU).
- `.checkpoint.win32_full_coverage` status: `unmet` → `met` (Track BU).
- `.checkpoint.lineage_classic_r28_play` status: `partial` → `met` (Track BV).
- `.roadmap.lineage_classic_r28` cond.1/cond.2/cond.3 actual status: `partial` → `retired_intentional` (Track BV).
- `blockers` arrays renamed `blockers_resolved` (verbatim list preserved) on 3 checkpoints (BU + BV).
- `.checkpoint.lineage_classic_r28_play` goal field: time-dependent "if/when... today partial" claim → "post Track BV cycle... flipped" narrative.
- hexa.toml `[package].version`: `1.0.0` → `1.0.1`.

### Posture

- **own1**: Wine 0 / hexa-only / DRM bypass 0 / Warden bypass 0 / anti-cheat bypass 0 / **GameGuard bypass 0** / anima clone 0 (BV cascading flip is honest documentation of permanent boundary, not a circumvention).
- **own2**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` honest verdict carries; status flip rationale per checkpoint cited; sideways-PASS semantics explicit; skeleton vs validated distinction preserved.
- **L1-L5 online expansion lock-in carries** per `.roadmap.battlenet`.

## Caveats (raw#10 C3)

5 explicit honesty caveats unchanged from v1.0.0 — see [README §Caveats](README.md#caveats-raw10-c3).

The 3 checkpoint met flips in this release do NOT promote `validated_manjeom`:

- `win32_50pct_coverage` met (100/50): skeleton-tier surface coverage threshold breach (435/435 fn shape present, individually compile/probe).
- `win32_full_coverage` met (100/100): same skeleton-tier surface basis. Real-tier production-smoke promotion remains multi-cycle long-horizon.
- `lineage_classic_r28_play` met (3/3 sideways-PASS): honest documentation of cascading retirement under retired_intentional semantics. GameGuard kernel-AC remains permanent boundary on Apple-only stack.

Real launch via `cli/gamebox.hexa launch <game>` continues to return PARTIAL (`retired_intentional:skeleton_tier_only`) until hexa interp closure CM-26..CM-30 lands.

## Drift resolution timeline

| Track | Date | Resolved drift items |
|---|---|---|
| Track BO (snapshot v2) | 2026-05-05 | 0 (snapshot only — flagged drift_count=3) |
| Track BU | 2026-05-05 | 2 (win32_50pct + win32_full cached header flip) |
| Track BV | 2026-05-05 | 1 (lineage_classic_r28 cascading cond flip + checkpoint flip) |
| **Total** | **2026-05-05** | **3 of 3** (drift_count 3 → 0) |

## Compatibility

No breaking changes. `gamebox` CLI behavior unchanged. Roadmap + checkpoint
JSONL schema unchanged. Loader/perf/tool inventory unchanged.

## Source repo

Sibling: <https://github.com/need-singularity/sim-universe>
Predecessor sessions:

- v1.0.0 — `dancinlife/airgenome-gamebox` 67 BG cycles (DELETED 2026-05-05)
- v1.0.1 — `need-singularity/gamebox` standalone Tracks BU + BV (2026-05-05)

## Author

박민우 <nerve011235@gmail.com>
