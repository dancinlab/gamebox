# gamebox c_d2r_archive_round_trip Drift Fix (option b) — Track CG Landed (2026-05-05)

> Track CG on gamebox standalone repo (`need-singularity/gamebox`), branch `main`.
> Fix Track CF deferred drift item. Investigation revealed orphan reference;
> chose option (b) — remove stale references from `hexa.toml [modules]` +
> README perf families table. The actual D2R MPQ archive round-trip
> functionality is covered by `lib/loader/pe_d2r_mpq_archive_load.hexa`
> (loader-class, NOT perf-class), exercised by `tests/test_d2r_archive_round_trip.hexa`.

---

## Context

- Repo: `need-singularity/gamebox` (origin), Apache-2.0.
- Branch: `main` (post fast-forward merge of `bg/layout-migration-2026-05-05`).
- Predecessor: Track CF (hexa.toml audit identified the drift item).
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## §1 investigation

Track CF flagged `lib/perf/c_d2r_archive_round_trip.hexa` as listed-but-missing in `hexa.toml gamebox_perf_top_families`. CG investigates to choose between 3 fix options (CF §9):

### §1.1 reference search results

| location | search command | matches |
|---|---|---|
| state/markers/airgenome_*.marker (75 predecessor) | grep | **0** |
| airgenome/doc/*.ai.md (75 predecessor handoffs) | grep | **0** |
| gamebox/doc/*.ai.md (8 native handoffs BU-CF) | grep | 1 (CF marker only — the audit doc that flagged the drift) |
| docs/*.md (catch-all legacy planning) | grep | 0 |
| .roadmap.* / .checkpoint.* | grep | 0 |
| lib/loader/*.hexa | grep | 0 |
| lib/perf/*.hexa | grep | 0 |
| tool/*.hexa | grep | 0 |
| cli/*.hexa | grep | 0 |
| tests/*.hexa | grep | 0 |
| README.md | grep | 1 (line 229 perf families table row) |
| hexa.toml | grep | 1 (line 109 gamebox_perf_top_families) |

**Conclusion**: 2 references total — both in user-facing inventory artifacts (`README.md` + `hexa.toml`). Zero references in:
- predecessor airgenome marker history (75 files — comprehensive search)
- internal hexa code (loaders / perf / tools / cli / tests)
- roadmap / checkpoint mk2 SSOT files

### §1.2 actual D2R MPQ archive functionality

Search for what actually implements D2R MPQ archive round-trip:

- `lib/loader/pe_d2r_mpq_archive_load.hexa` — **EXISTS** (loader-class)
- `lib/loader/pe_d2r_savegame_d2s_format.hexa` — **EXISTS** (loader-class)
- `tests/test_d2r_archive_round_trip.hexa` — **EXISTS**, spawns the 2 loader modules above for self-test

The test file's actual harness:

```hexa
let path_mpq = "lib/loader/pe_d2r_mpq_archive_load.hexa"
let path_d2s = "lib/loader/pe_d2r_savegame_d2s_format.hexa"
let out_mpq = run_module_self_test(path_mpq)
let out_d2s = run_module_self_test(path_d2s)
```

The functionality is real, lives in the loader layer, and is correctly covered by the existing test. The `lib/perf/c_d2r_archive_round_trip.hexa` reference was a stale enumeration that never matched reality.

### §1.3 decision matrix

| option | viable? | rationale |
|---|---|---|
| (a) create the missing perf module | ❌ NO | Would invent perf module content with no design predecessor; risk of fabricating implementation that doesn't reflect intent. The functionality lives in loader-class, not perf-class. |
| **(b) remove stale references** | ✅ **YES** | Honest cleanup. Both refs are user-facing inventory only — removing them aligns README + hexa.toml with on-disk reality. The actual functionality (loader + test) is preserved. |
| (c) airgenome predecessor history investigation | ⚠️ PARTIAL | §1.1 already searched 75 predecessor markers + 75 predecessor handoff docs; 0 hits. Investigation complete: predecessor never had this file either. Therefore (c) reduces to (b). |

**CG choice: option (b).**

---

## §2 in-place writes

| File | Op |
|---|---|
| `hexa.toml` | remove orphan entry from `gamebox_perf_top_families` array; add explanatory comment block referencing this Track CG handoff |
| `README.md` | remove `c_d2r_archive_round_trip` row from perf families table (line 229 area) |

`in_place_writes`: 2.
NEW files: 2 (this handoff doc + Track CG marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 LICENSE / 0 CHANGELOG / 0 RELEASE_NOTES / 0 existing airgenome/doc/ / 0 existing gamebox/doc/ / 0 existing docs/*.md / 0 predecessor marker mutation / 0 gamebox-native marker mutation.

NOT modified:
- `lib/loader/pe_d2r_mpq_archive_load.hexa` (preserved — actual functionality)
- `lib/loader/pe_d2r_savegame_d2s_format.hexa` (preserved)
- `tests/test_d2r_archive_round_trip.hexa` (preserved — correctly tests the loader modules)
- `.roadmap.diablo2_resurrected` cond.2 (preserved — verified by the existing test file)

---

## §3 perf families "18+" claim — preserved

README line 218 states "18+ families at 100% manjeom (skeleton-tier per Track BL/BM forensic)". This claim is unaffected by CG:

- The orphan `c_d2r_archive_round_trip` entry was NOT one of the 18 real families (it was a stale enumeration that never had a backing implementation).
- Track BL/BM forensic counted real families with on-disk presence.
- Removing the orphan row does NOT decrement the real family count.

---

## §4 policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (per Track G additive_at_field-level precedent for JSONL-style array element removal; explanatory comment ADDED in hexa.toml; README table row removal is the symmetric "ungate" of the corresponding hexa.toml entry — both treat the same orphan reference as a coherent unit)
- **destructive_ops**: 0 ✓ (no FILE deletion; only stale REFERENCE removal — actual functionality files preserved)
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no anima clone / no DRM bypass ✓
- **own2**: forensic_verdict honest unchanged; investigation results enumerated; option choice rationale documented; "18+ families" claim preservation explicit ✓
- **raw**: 9 / 11 / 12 / 15 / 175 enforce ✓

---

## §5 forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CG removes a stale inventory enumeration without affecting any module count
in `lib/`, perf family count, validated_manjeom, or any forensic baseline.

---

## §6 verification

- `grep -c "c_d2r_archive_round_trip" hexa.toml` → 1 (the explanatory comment line; orphan entry removed)
- `grep -c "c_d2r_archive_round_trip" README.md` → 0 (row removed)
- `grep -c "c_d2r_archive_round_trip" gamebox/doc/*.ai.md` → references only in CF + CG handoff docs (audit + fix)
- `ls lib/loader/pe_d2r_mpq_archive_load.hexa` → still exists
- `ls lib/loader/pe_d2r_savegame_d2s_format.hexa` → still exists
- `ls tests/test_d2r_archive_round_trip.hexa` → still exists
- hexa.toml audit re-run after CG: **40 → 39 listed entries; 39/39 OK** (zero drift items)

---

## §7 predecessors

- Track CF — `state/markers/gamebox_hexa_toml_modules_audit_track_cf_landed.marker` (audit identified the drift item)
- Track BT — `state/markers/gamebox_initial_extraction_landed.marker` (extraction baseline; orphan reference present at extraction time per §1.1 zero predecessor hits)

---

## §8 next-cycle hooks (NOT triggered)

- Track CH: gamebox standalone session aggregate v3 narrative covering BT through CG
- Future cycle: re-run hexa.toml [modules] audit periodically (especially after lib module land cycles like nightreign Track CG+ candidate)
- v1.0.3 release notes: post-CG patch decision deferred to user direction (CG is small enough to fold into next minor release rather than separate v1.0.3)
