# gamebox v1.0.2 — Layout Migration + Inventory Drift Refresh + per_game §9 Closure (2026-05-05)

## Summary

Patch release bundling post-v1.0.1 work:

- **Layout migration** (`1d28b4c`): canonical `<feature>/doc/` layout —
  `airgenome/doc/` (75 predecessor) + `gamebox/doc/` (8 native incl. CB-CC).
- **Track BX** — per_game README §9 update for offline-relevant subset (6 files: lineage_classic_r28 + d2r + battlenet + lineage_w + lineage_m + purple_launcher).
- **Track BY** — README Game Coverage refresh: wow remove (historical preserve), elden_ring DLC scope (Shadow of the Erdtree 2024-06-21), elden_ring_nightreign documentation-only fan-out (2025-05-30 standalone multiplayer co-op spinoff).
- **Track BZ** — closure_orchestrator real-invocation attempt + paper-mode derivation (hexa interp closure CM-26..CM-30 still missing).
- **Track CA** — RFC 016 reassessment over post-BS 7-cycle interval (0 hits reaffirmed; cumulative BS+CA = 28-cycle null floor).
- **Track CB** — README inventory drift refresh (5 items: .roadmap 14→15, .checkpoint met 1→3, loader 35→59 named/455 .hexa, tests 8→9, markers 75→75+8+5).
- **Track CC** — per_game §9 closure for remaining 4 retired_intentional games (cs2 / delta_force / diablo4 / elden_ring); elden_ring §9 surfaces DLC scope narrative.

## Highlights

- **Layout convention** established: `airgenome/doc/`, `gamebox/doc/`, catch-all `docs/` for legacy planning + 3 subdirs (`hexa_rfc/`, `per_game/`, `research/`).
- **All per_game READMEs** (except `wow` historical) have §9 sections covering post-Track-BU/BV/BY/CA state.
- **elden_ring scope** formally clarified: base game (2022-02-25) + Shadow of the Erdtree DLC (2024-06-21).
- **elden_ring_nightreign** fan-out: documentation-only, 0 lib modules at land-time, deferred to Track CD+ candidate pending real binary acquisition + EAC class re-confirmation.
- **README inventory** accurately reflects on-disk state (455 lib/loader/ .hexa, 9 tests/, 88 total markers across 3 classes).
- **Forensic verdict (UNCHANGED)**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

## Changes

### Added (post-v1.0.1)

- `RELEASE_NOTES_v1.0.2.md` (this file).
- 7 per_game README §9 sections (BX 6 + CC 4 — wait, BX 6 covered offline-relevant, CC 4 covered retired_intentional; that's 6+4=10, but lineage_classic_r28 is in BX scope = 6+4 = 10 §9 sections total but elden_ring_nightreign already had §9 from BY = 11 total per_game READMEs with §9 across all releases).
  - Actually: BX added §9 to 6 files (lineage_classic_r28, d2r, battlenet, lineage_w, lineage_m, purple_launcher).
  - BY created elden_ring_nightreign with §1-§9 land-time.
  - CC added §9 to 4 files (cs2, delta_force, diablo4, elden_ring).
  - Total: 11 per_game files have §9. Only `wow` doesn't (historical).
- New `.roadmap.elden_ring_nightreign` (BY).
- New `docs/per_game/elden_ring_nightreign_README.ai.md` T1 baseline (BY).
- `dlc_scope` + `spinoff_cross_link` + `ai_native_handoff_track_by` fields on `.roadmap.elden_ring` (BY).
- 6 NEW `gamebox_*` markers post-extraction (BV+BW+BX+BY+BZ+CA from v1.0.1; CB+CC added in this v1.0.2 → 8 total post-extraction native markers BT through CC).
- README "Native markers" subsection enumerating BT-CC (CB).
- README "Resolver telemetry markers (gitignored)" subsection (CB).
- `.gitignore` patterns for hexa-resolver runtime telemetry (`closure_orchestrator_<ts>` / `remote_preflight_<ts>` / `resource_lint_<ts>` / `closure_progress_<ts>`).

### Changed

- README.md TL;DR header: `14 .roadmap.* SSOT + 7 .checkpoint.* (1 met)` → `15 .roadmap.* SSOT (14 active + 1 historical wow) + 7 .checkpoint.* (3 met post Track BV)`.
- README.md architecture caption: terse 7-figure → detailed inventory (455 .hexa lib/loader/, 9 tests, 88 markers across 3 classes).
- README.md Loaders heading: `(35 modules)` → `(59 named groups across 455 .hexa files)`.
- README.md Predecessor markers section: extended with Native markers + Resolver telemetry subsections.
- `.roadmap.elden_ring` goal field: scope clarified to mention base + Shadow of the Erdtree DLC (BY).
- `docs/airgenome_*.ai.md` → `airgenome/doc/airgenome_*.ai.md` (75 files moved per `1d28b4c`).
- `docs/gamebox_*.ai.md` → `gamebox/doc/gamebox_*.ai.md` (7 files moved per `1d28b4c`; CB+CC added 2 more under new layout).
- `state/markers/*.marker` cross-references updated to reflect new layout (193 files / 621 lines updated via sed in `1d28b4c`).
- `hexa.toml` version: `1.0.1` → `1.0.2`.
- README version badge: `version-1.0.1` → `version-1.0.2`.

### Removed

- None. All "removals" preserve files as historical artifacts:
  - `wow` removed from active README Game Coverage table (BY); `.roadmap.wow` + `docs/per_game/wow_README.ai.md` + lib loaders preserved verbatim.
  - own1 `destructive_ops: 0` maintained throughout (Track BT exception was one-time).

### Posture

- **own1**: Wine 0 / hexa-only / DRM bypass 0 / Warden bypass 0 / anti-cheat bypass 0 (VAC + EAC kernel + EAC anti-tamper + GameGuard kernel + always-online DRM + FromSoft matchmaking) / anima clone 0 / `destructive_ops: 0`.
- **own2**: PASS/FAIL/PARTIAL/RETIRED_INTENTIONAL emit honest; status flip rationale per checkpoint cited; sideways-PASS semantics explicit; skeleton vs validated distinction preserved across all docs; `validated_manjeom = 0` carries.
- **L1-L5 online expansion lock-in carries** per `.roadmap.battlenet`.

## Caveats (raw#10 C3) — UNCHANGED from v1.0.0

5 explicit honesty caveats unchanged from v1.0.0 — see [README §Caveats](README.md#caveats-raw10-c3).

The 3 checkpoint met flips (carried over from v1.0.1) + 4 new per_game §9
sections + DLC scope clarification + nightreign documentation-only fan-out
do NOT promote `validated_manjeom`:

- `win32_50pct_coverage` met (100/50): skeleton-tier surface threshold breach (BU).
- `win32_full_coverage` met (100/100): same skeleton-tier surface basis (BU).
- `lineage_classic_r28_play` met (3/3 sideways-PASS): honest documentation of cascading retirement (BV).
- elden_ring DLC scope (BY): narrative clarification only; no module count change.
- elden_ring_nightreign fan-out (BY): documentation-only with 0 lib modules at land-time.

Real launch via `cli/gamebox.hexa launch <game>` continues to return PARTIAL
(`retired_intentional:skeleton_tier_only`) until hexa interp closure CM-26..CM-30
lands. closure_orchestrator real invocation also blocked by same class (BZ
documented this with paper-mode derivation as honest deferred-run).

## Cumulative state (post v1.0.2)

| metric | v1.0.0 | v1.0.1 | v1.0.2 |
|---|---|---|---|
| .roadmap files | 14 | 14 | 15 (14 active + 1 historical wow) |
| .checkpoint met | 0 (pre-flip) | 3 | 3 (CB inventory drift fix; same met count) |
| per_game §9 sections | 0 | 0 | 11 (BX 6 + BY 1 + CC 4) |
| gamebox-native markers | 1 (BT) | 7 (BT-CA) | 9 (BT-CC) |
| RFC 016 audit cycles | 21 (BS) | 21 (BS) | 28 (BS+CA) cumulative null floor |
| Cross-cutting forensic verdict | UNCHANGED | UNCHANGED | UNCHANGED |

## Compatibility

No breaking changes. `gamebox` CLI behavior unchanged. Roadmap + checkpoint
JSONL schema unchanged. Loader/perf/tool inventory unchanged.

**Path migration note**: any code or scripts referencing `docs/airgenome_*.ai.md`
or `docs/gamebox_*.ai.md` paths must update to `airgenome/doc/` or `gamebox/doc/`
respectively. Sed sweep applied to 193 files / 621 lines in commit `1d28b4c`;
external consumers should mirror.

## Source repo

Sibling: <https://github.com/dancinlab/sim-universe>
Branch: `bg/layout-migration-2026-05-05` (CB+CC+CD landed here; merge to `main` at user discretion).

Predecessor releases:
- v1.0.0 — initial extraction from `dancinlife/airgenome-gamebox` 67 BG cycles
- v1.0.1 — drift resolution + win32 milestone bundle (BU + BV)
- v1.0.2 — layout migration + inventory drift refresh + per_game §9 closure (this release; bundles BX + BY + BZ + CA + CB + CC)

## Author

박민우 <nerve011235@gmail.com>
