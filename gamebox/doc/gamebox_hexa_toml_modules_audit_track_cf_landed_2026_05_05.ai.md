# gamebox hexa.toml [modules] Audit — Track CF Landed (2026-05-05)

> Track CF on gamebox standalone repo, branch `bg/layout-migration-2026-05-05`.
> Read-only audit of `hexa.toml [modules]` declarations against actual on-disk
> files in `lib/loader/`, `lib/perf/`, `tool/`. Doc-only cycle: identifies
> 1 drift item (stale entry referencing missing file); defers fix to future
> cycle pending user direction.

---

## Context

- Repo: `need-singularity/gamebox` (origin), Apache-2.0.
- Branch: `bg/layout-migration-2026-05-05`.
- Predecessor: Track CE (closure_orchestrator selftest paper-mode 2026-05-05).
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## §1 hexa.toml [modules] inventory

`hexa.toml [modules]` declares **40 unique entries** across 7 named subsections:

| Subsection | Entry count | Coverage |
|---|---|---|
| `gamebox_loader_pe_core` | 10 | PE loaders + 10 calling-conv + ACL + APC + AVRT |
| `gamebox_loader_pe_battlenet` | 9 | Battle.net surface (8 pe_*battlenet/battle_net + 1 battlenet_bypass) |
| `gamebox_loader_dx` | 5 | dx_d3d11/_stage1 + dx_d3d12/_stage1/_stage2 |
| `gamebox_loader_runtime` | 5 | dll_resolve_master + iat_runtime_bind + i386_call_frame + native_helper_bridge + oss_validate |
| `gamebox_loader_launchers` | 2 | d2r_launch + d4_launch |
| `gamebox_perf_top_families` | 7 | curated subset of 18+ families at 100% manjeom |
| `gamebox_tools` | 2 | roadmap_op + closure_orchestrator |
| **Total** | **40** | curated subset |

Note: hexa.toml [modules] declarations are an **explicitly curated subset**, NOT exhaustive enumeration:

- 31 loader entries vs 455 actual `lib/loader/*.hexa` files (~7% coverage)
- 7 perf entries vs 242 actual `lib/perf/*.hexa` files (~3% coverage)
- 2 tool entries vs 2 actual `tool/*.hexa` files (100% coverage)

The curation surfaces canonical entry points (PE *_core + named groups) for `hx install gamebox` consumers. Extension/specific-track/fn-level modules under `lib/loader/` are imported by these top-level modules transitively.

---

## §2 existence audit — 39/40 OK + 1 missing

| status | count | files |
|---|---|---|
| OK (file exists on disk) | 39 | (39 entries, all present) |
| **MISS (file referenced but missing)** | **1** | **`lib/perf/c_d2r_archive_round_trip.hexa`** |

### §2.1 The 1 drift item

```
hexa.toml gamebox_perf_top_families = [
    ...,
    "lib/perf/c_d2r_archive_round_trip.hexa",  // ← MISS
    ...
]
```

Search results:

- `ls lib/perf/c_d2r_archive*` → no match
- `ls lib/perf/c_d2r_mpq*` → no match
- `ls lib/perf/ | grep -iE "d2r.*round|round.*d2r"` → no match
- `grep -l "c_d2r_archive_round_trip" lib/loader/*.hexa lib/perf/*.hexa tool/*.hexa cli/*.hexa tests/*.hexa` → no matches outside hexa.toml itself
- `tests/test_d2r_archive_round_trip.hexa` exists but only imports `r0_common`, not the missing perf module

**Cross-link**: README §Module Inventory perf families table (line ~211) also references `c_d2r_archive_round_trip` with "100% | D2R MPQ archive round-trip (cond1)" — same name in user-facing inventory.

**Possible cause** (informational, not asserted):

- Original airgenome-gamebox session may have had this file; not transferred at extraction (Track BT). 
- OR name in README/hexa.toml is a typo for an existing file (e.g., `pe_d2r_mpq_archive_load.hexa` exists in `lib/loader/` but is loader-class, not perf-class).
- OR the file was renamed during airgenome session lifecycle and hexa.toml/README references weren't updated.

This audit does NOT assert which. Future cycle (Track CG candidate) can either:

- (a) Create `lib/perf/c_d2r_archive_round_trip.hexa` if intent is to land the perf module
- (b) Remove the stale `hexa.toml gamebox_perf_top_families` entry + README perf families row if intent is to retire the reference
- (c) Investigate predecessor airgenome history to determine canonical name

---

## §3 in-place writes

| File | Op |
|---|---|
| (none) | — |

`in_place_writes`: 0 (cleanest doc-only audit cycle).
NEW files: 2 (this handoff doc + Track CF marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml (drift item documented but NOT fixed in CF; deferred to future cycle pending user direction) / 0 LICENSE / 0 CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/ / 0 existing gamebox/doc/ / 0 existing docs/*.md / 0 predecessor marker mutation / 0 gamebox-native marker mutation.

---

## §4 policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (cleanest cycle — 2 NEW files only)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no anima clone / no DRM bypass ✓
- **own2**: forensic_verdict honest unchanged; drift item honestly enumerated + 3 alternative resolution paths documented; deferred-fix posture explicit ✓
- **raw**: 9 / 11 / 12 / 15 / 175 enforce ✓

---

## §5 forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CF is read-only audit; no module count change in artifacts, no
validated_manjeom promotion. Audit confirms 39/40 hexa.toml entries
correspond to existing on-disk files; the 1 drift item documented as
honest inventory gap.

---

## §6 caveats

C1. **Curated subset not exhaustive.** hexa.toml [modules] is intentionally a curated subset (~7% loader coverage / ~3% perf coverage). The 396 unlisted lib/loader/ files + 235 unlisted lib/perf/ files are imported transitively or used as extension/specific-track surface; not surfaced through `hx install` directly. CF does NOT recommend listing all files — would defeat the curation purpose.

C2. **Drift fix deferred — own1 destructive_ops:0 + user direction.** CF documents the drift but does not modify hexa.toml. Either creating the file (additive) OR removing the stale entry (additive_at_field-level per Track G precedent) requires user confirmation of intent.

C3. **README perf families table cross-link.** The same `c_d2r_archive_round_trip` name appears in README §Module Inventory; future fix should keep both in sync (same approach: create file, remove both refs, or rename if typo).

C4. **No predecessor history audit.** This audit reads current state only; does not investigate airgenome predecessor session marker history to determine if the file ever existed. Out of scope.

C5. **`tests/test_d2r_archive_round_trip.hexa` is independent.** The test file exists and imports only `r0_common` — does not depend on the missing `lib/perf/c_d2r_archive_round_trip.hexa`. Test integrity not affected by the drift item.

---

## §7 verification

- `grep -oE '"(lib/loader|lib/perf|tool)/[^"]+\.hexa"' hexa.toml | sort -u | wc -l` → 40
- 31 loader / 7 perf / 2 tool entries
- 39 OK + 1 MISS
- Missing file: `lib/perf/c_d2r_archive_round_trip.hexa`
- README cross-reference: line ~211 "c_d2r_archive_round_trip"
- No internal hexa code references the missing module (only hexa.toml + README)

---

## §8 predecessors

- Track CE — `state/markers/gamebox_closure_orchestrator_selftest_paper_mode_track_ce_landed.marker` (selftest paper-mode; CF is sibling housekeeping audit)
- Track CD — `state/markers/gamebox_v1_0_2_release_notes_track_cd_landed.marker` (v1.0.2 release; CF post-release)
- Track CB — `state/markers/gamebox_readme_inventory_drift_refresh_track_cb_landed.marker` (README inventory drift refresh; CF audits the parallel hexa.toml inventory)
- Track BT — `state/markers/gamebox_initial_extraction_landed.marker` (initial extraction baseline; CF references possible cause of drift item)

---

## §9 next-cycle hooks (NOT triggered)

- Track CG candidate (a): create `lib/perf/c_d2r_archive_round_trip.hexa` if intent is to land the perf module (matches D2R cond.1 MPQ archive round-trip narrative; landed test exists at `tests/test_d2r_archive_round_trip.hexa`)
- Track CG candidate (b): remove stale `hexa.toml gamebox_perf_top_families` entry + README perf families row if intent is to retire reference
- Track CG candidate (c): investigate airgenome predecessor marker history for `c_d2r_archive_round_trip` references to determine canonical lifecycle (rename / never-existed / extracted-but-deleted)
- Future cycle: periodic re-audit as new lib modules land (especially after nightreign Track CG fan-out)
