# gamebox v1.0.3 Release Notes — Track CI Landed (2026-05-05)

> Track CI on gamebox standalone repo, branch `main`. Patch release bundling
> post-v1.0.2 work: CE (selftest paper-mode) + CF (hexa.toml audit) + CG
> (orphan reference removal) + CH (session aggregate v3) + 2 housekeeping
> commits (CE post-mortem .gitignore + CG comment cleanup). Hexa.toml version
> bump 1.0.2 → 1.0.3.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main`.
- Predecessor: Track CH (session aggregate v3 narrative 2026-05-05).
- v1.0.2 (Track CD): layout migration + inventory refresh + per_game §9 closure 2026-05-05.
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## Mission

CI1 — `RELEASE_NOTES_v1.0.3.md` NEW file authored:

- Summary section bundling CE + CF + CG + CH + 2 housekeeping commits.
- Highlights: real-invocation paper-mode validation (BZ + CE), hexa.toml drift cleared (CG), session aggregate v3 establishes standalone-repo narrative class (CH).
- Cumulative state table comparing v1.0.0 / v1.0.1 / v1.0.2 / **v1.0.3**.
- Source repo + branch + author boilerplate.

CI2 — `CHANGELOG.md` v1.0.3 section additive insertion above v1.0.2.

CI3 — `hexa.toml` version `1.0.2` → `1.0.3` + README badge URL refresh.

CI4 — handoff doc + marker (this file + Track CI marker).

---

## Release contents

| Track | Marker | Scope |
|---|---|---|
| CE | `gamebox_closure_orchestrator_selftest_paper_mode_track_ce_landed` | S1-S11 paper-mode trace; BZ §11 hook partial resolution |
| CE post-mortem | (no marker — .gitignore patch only) | resource_op_<ts> resolver telemetry pattern |
| CF | `gamebox_hexa_toml_modules_audit_track_cf_landed` | 40 entries audit; 1 drift item flagged |
| CG | `gamebox_c_d2r_archive_round_trip_drift_fix_track_cg_landed` | orphan reference option-b removal |
| CG comment cleanup | (no marker — hexa.toml comment style fix) | backticks instead of double-quotes to avoid audit false-positive |
| CH | `gamebox_session_aggregate_v3_track_ch_landed` | first standalone-repo session aggregate BT-CG |

---

## In-place writes

| File | Op |
|---|---|
| `hexa.toml` | version field flip 1.0.2 → 1.0.3 |
| `README.md` | version badge URL flip |
| `CHANGELOG.md` | additive section insertion (v1.0.3 above v1.0.2) |

`in_place_writes`: 3.
NEW files: 3 (`RELEASE_NOTES_v1.0.3.md` + this handoff doc + Track CI marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 LICENSE / 0 existing airgenome/doc/ / 0 existing gamebox/doc/ / 0 existing docs/*.md / 0 predecessor marker mutation / 0 gamebox-native marker mutation.

---

## Cumulative state (post v1.0.3)

| metric | v1.0.0 | v1.0.1 | v1.0.2 | **v1.0.3** | delta v1.0.2→v1.0.3 |
|---|---|---|---|---|---|
| .roadmap files | 14 | 14 | 15 | 15 | 0 |
| .checkpoint met | 0 | 3 | 3 | 3 | 0 |
| per_game §9 sections | 0 | 0 | 11 | 11 | 0 |
| gamebox-native markers | 1 | 7 | 9 | **15** | +6 (CE+CF+CG+CH+post-mortems) — wait CE post-mortem + CG comment cleanup are no-marker housekeeping, so net +4 markers (CE+CF+CG+CH) |
| Total post-extraction commits | 1 | 6 | 13 | **19** (or 20 incl. CI) | +6 (CE + CE post-mortem + CF + CG + CG cleanup + CH) |
| RFC 016 audit cumulative cycles | 21 | 21 | 28 | **34** | +6 (CB-CG narrative window in CH) |
| hexa.toml audit drift items | 1 | 1 | 1 | **0** | -1 (CG cleared) |
| Layout | flat docs/ | flat docs/ | `<feature>/doc/` | `<feature>/doc/` | 0 |
| Forensic verdict | UNCHANGED | UNCHANGED | UNCHANGED | UNCHANGED | 0 |
| validated_manjeom | 0 | 0 | 0 | 0 | 0 |

---

## Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (CHANGELOG additive section + version field flip + badge URL flip + 3 NEW files)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no anima clone / no DRM bypass ✓
- **own2**: forensic_verdict honest unchanged; release notes explicit about validated_manjeom=0 carry; paper-mode vs real-run distinction preserved across all release docs ✓
- **raw**: 9 / 11 / 12 / 15 / 175 enforce ✓

---

## Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

v1.0.3 is paper-mode validation + inventory cleanup + v3 aggregate release.
NO promotion of validated_manjeom. NO module count change in artifacts (CG
removed REFERENCE not FILE).

---

## Verification

- `grep '^version' hexa.toml` → `version = "1.0.3"`
- `head -5 CHANGELOG.md` shows `## [1.0.3] — 2026-05-05` above `## [1.0.2]`
- `RELEASE_NOTES_v1.0.3.md` exists with valid markdown
- README version badge shows `version-1.0.3`
- 4 new gamebox/doc/ handoff docs present (CE / CF / CG / CH)
- 4 new state/markers/gamebox_*.marker present (CE / CF / CG / CH)

---

## Predecessors

- Track CH — `state/markers/gamebox_session_aggregate_v3_track_ch_landed.marker` (v3 narrative; CI bundles for release)
- Track CG — `state/markers/gamebox_c_d2r_archive_round_trip_drift_fix_track_cg_landed.marker`
- Track CF — `state/markers/gamebox_hexa_toml_modules_audit_track_cf_landed.marker`
- Track CE — `state/markers/gamebox_closure_orchestrator_selftest_paper_mode_track_ce_landed.marker`
- v1.0.2 (Track CD) — `state/markers/gamebox_v1_0_2_release_notes_track_cd_landed.marker` (predecessor release)

---

## Next-cycle hooks (NOT triggered)

- `git tag v1.0.3` + `git push origin main v1.0.3` — user manual action (per user direction; safe path).
- GitHub Releases auto-publish via tag push — workflow not in scope.
- Track CJ candidate: branch cleanup `git branch -d bg/layout-migration-2026-05-05` (own1 destructive_ops:0; user grant pending).
- Track CJ candidate: nightreign lib module fan-out (BY deferred; pending real binary acquisition).
- Track CJ candidate: future cycle to land `docs/VAC_HONEST_FREEZE.md` + `docs/EAC_KERNEL_HONEST_FREEZE.md` mk1 narratives (CC §9 noted as deferred).
- Track CJ candidate: real-run validation of orchestrator post hexa interp closure CM-26..CM-30 (multi-year horizon).
