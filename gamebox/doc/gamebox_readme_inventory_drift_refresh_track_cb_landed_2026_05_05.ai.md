# gamebox README Inventory Drift Refresh — Track CB Landed (2026-05-05)

> Track CB on gamebox standalone repo (`dancinlab/gamebox`), branch
> `bg/layout-migration-2026-05-05`. Doc-only README inventory accuracy
> refresh after Tracks BU through CA accumulated drift in 5 places.
> First track authored under the new `<feature>/doc/` layout (commit `1d28b4c`
> 2026-05-05 layout migration).

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `bg/layout-migration-2026-05-05` (post layout migration `1d28b4c`).
- Predecessor: layout migration commit `1d28b4c refactor(layout 2026-05-05): docs/ flat → <feature>/doc/ feature-grouped`.
- Predecessor (semantic): Track CA (RFC 016 reassessment, 0 hits reaffirmed).
- Driver: README inventory text grew stale across BU (2 checkpoints flipped → 1 met → eventually 3 met after BV), BV (lineage_classic_r28 cascading flip), BY (15 .roadmap files now, was 14), BZ (8 gamebox-native markers + 5 telemetry committed), CA (cumulative state) + 1d28b4c layout migration (75 airgenome handoff docs moved to airgenome/doc/, 7 gamebox handoff docs moved to gamebox/doc/).
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## Mission

CB1 — README.md inventory text accuracy refresh:

- Line 5 (TL;DR header): "14 .roadmap.* SSOT + 7 .checkpoint.* (1 met)" → "15 .roadmap.* SSOT (14 active + 1 historical wow) + 7 .checkpoint.* (3 met post Track BV)"
- Line 118-119 (architecture diagram caption): expand from terse 7-figure to detailed inventory line citing actual file counts (455 .hexa in lib/loader/ across 59 named groups, 15 .roadmap.* with 14 active + 1 historical, 7 .checkpoint.* with 3 met, 9 tests/.hexa with 8 active in hexa.toml + 1 default, 75 airgenome predecessor markers under airgenome/doc/ + 8 gamebox-native under gamebox/doc/ + telemetry gitignored)
- Line 187 (Loaders heading): "(35 modules)" → "(59 named groups across 455 .hexa files)" + clarifying paragraph about extension/specific-track/fn-level subset
- Line 238 area (Predecessor markers section): added NEW "Native markers" subsection enumerating 8 gamebox-native Tracks BT-CA + NEW "Resolver telemetry markers (gitignored)" subsection documenting the auto-generated runtime telemetry class. Updated Predecessor markers section to reference airgenome/doc/ companion path per layout migration.

CB2 — handoff doc + marker (this file + Track CB marker).

---

## Drift items resolved

| Item | Before | After | Source of truth |
|---|---|---|---|
| .roadmap count | 14 | 15 (14 active + 1 historical wow) | `ls .roadmap.* \| wc -l` |
| .checkpoint met count | 1 met (Track BR) | 3 met post Track BV | `.checkpoint.win32_50pct_coverage` + `.checkpoint.win32_full_coverage` (BU) + `.checkpoint.lineage_classic_r28_play` (BV) cached headers |
| Loader file count claim | 35 modules | 59 named groups across 455 .hexa files | `ls lib/loader/*.hexa \| wc -l` = 455; named-group sum = 32 + 6 + 9 + 5 + 5 + 2 = 59 |
| Tests count | 8 | 9 .hexa files (8 active in hexa.toml [test].files + 1 default test.hexa) | `ls tests/*.hexa \| wc -l` = 9 |
| Marker count | 75 predecessor | 75 predecessor + 8 gamebox-native + 5 telemetry (gitignored) | `ls state/markers/airgenome_*` = 75; `ls state/markers/gamebox_*` = 8 |

NOT modified:
- 242 perf modules — matches `ls lib/perf/*.hexa | wc -l` ✓
- 2 tools — matches `ls tool/*.hexa | wc -l` ✓
- 7 .checkpoint.* — matches `ls .checkpoint.* | wc -l` ✓
- 18+ perf families at 100% — preserved (BL/BM forensic baseline, no new BG cycles invalidating)
- 4 hexa-lang upstream RFCs merged — preserved
- 67 BG cycles (predecessor session count) — preserved verbatim (frozen at extraction)
- Module Inventory table groups + counts — table itself accurate (32 + 6 + 9 + 5 + 5 + 2 = 59); only the heading "(35 modules)" was stale

---

## In-place writes

| File | Op |
|---|---|
| `README.md` | inventory text refresh in 4 locations (line 5 TL;DR + line 118-119 architecture caption + line 187 Loaders heading + line 238 area Predecessor markers section additive expansion) |

`in_place_writes`: 1 (README.md only).
NEW files: 2 (this handoff doc at `gamebox/doc/` + Track CB marker at `state/markers/`).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0 CHANGELOG / 0 RELEASE_NOTES / 0 existing airgenome/doc/*.md / 0 existing gamebox/doc/*.md / 0 existing docs/**/*.md / 0 predecessor marker mutation / 0 gamebox-native marker mutation.

---

## Layout migration awareness

This is the first track authored under the new `<feature>/doc/` layout per
commit `1d28b4c` (2026-05-05). Convention going forward:

- Handoff docs under `gamebox/doc/` (gamebox-native) or `airgenome/doc/` (predecessor)
- Markers remain at `state/markers/`
- Catch-all docs (legacy planning + 3 subdirs) remain under `docs/`
- Future canonical: `<feature>/core/` + `<feature>/module/` to follow as features mature

This handoff doc + future track docs land at `gamebox/doc/`.

---

## Policy compliance

- **migration**: forbidden ✓ (CB does NOT introduce new migrations; CB awareness of 1d28b4c migration is read-only reference)
- **changes**: additive_only ✓
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no anima clone / no DRM bypass ✓
- **own2**: forensic_verdict honest unchanged; inventory drift items honestly enumerated; resolver-telemetry historical-preservation explicit; validated_manjeom=0 carries ✓
- **raw**: 9 / 11 / 12 / 15 / 175 enforce ✓

---

## Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CB is documentation accuracy refresh only — no module count change in the
artifacts themselves, no validated_manjeom promotion. The README now
accurately reflects on-disk state instead of land-time-frozen counts.

---

## Verification

- `grep -c "15 .roadmap.\*" README.md` → 1
- `grep -c "3 met post Track BV" README.md` → 1
- `grep -c "455 .hexa files" README.md` → 1
- `grep -c "59 named loader groups" README.md` → 1
- `grep -c "Native markers" README.md` → 1
- `grep -c "Resolver telemetry markers" README.md` → 1
- `grep -c "(35 modules)" README.md` → 0 (replaced)
- README references `airgenome/doc/` for predecessor companion path

---

## Predecessors

- layout migration commit `1d28b4c` — established `<feature>/doc/` layout 2026-05-05
- Track CA — `state/markers/gamebox_hexa_upstream_rfc_016_reassessment_track_ca_landed.marker` (RFC 016 0-hits reaffirmed)
- Track BZ — `state/markers/gamebox_closure_orchestrator_paper_invocation_track_bz_landed.marker` (resolver telemetry markers committed; gitignored)
- Track BY — `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker` (drove .roadmap count 14 → 15)
- Track BV — `state/markers/gamebox_lineage_classic_r28_drift_resolution_track_bv_landed.marker` (drove checkpoint met count 1 → 3 cumulative with BU)
- Track BU — `state/markers/gamebox_readme_game_list_and_win32_checkpoint_flip_track_bu_landed.marker` (drove win32 checkpoint flips)

---

## Next-cycle hooks (NOT triggered)

- Track CC: per_game §9 closure for remaining 5 (cs2, delta_force, diablo4, elden_ring, elden_ring_nightreign)
- Track CD: v1.0.2 release notes bundling BX through CC
- Track CE: closure_orchestrator selftest paper-mode (BZ §11 hook)
- Track CF: hexa.toml [modules] audit + alignment
- Future cycle: branch merge strategy for `bg/layout-migration-2026-05-05` → `main` (user decision)
