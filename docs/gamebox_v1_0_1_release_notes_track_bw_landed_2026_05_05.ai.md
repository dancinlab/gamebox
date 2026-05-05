# gamebox v1.0.1 Release Notes + CHANGELOG + Version Bump — Track BW Landed (2026-05-05)

> Track BW on gamebox standalone repo (`need-singularity/gamebox`). Patch
> release bundles BU + BV deltas under v1.0.1. Three sub-missions:
> BW1 — `RELEASE_NOTES_v1.0.1.md` NEW file authored.
> BW2 — `CHANGELOG.md` v1.0.1 section additive insertion.
> BW3 — `hexa.toml` `[package].version` 1.0.0 → 1.0.1; README version badge refresh.

---

## Context

- Repo: `need-singularity/gamebox` (origin), Apache-2.0.
- Predecessor: Track BV (lineage_classic_r28 drift resolution, drift_count 1 → 0).
- Predecessor: Track BU (Game Coverage section + 2 win32 checkpoint flips, drift_count 3 → 1).
- v1.0.0 (Track BT): initial extraction from airgenome-gamebox 67 BG cycles 2026-05-05.
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## BW1 — `RELEASE_NOTES_v1.0.1.md` (NEW)

Mirror pattern: `RELEASE_NOTES_v1.0.0.md`. Sections: Summary / Highlights /
Changes (Added / Changed / Posture) / Caveats / Drift resolution timeline /
Compatibility / Source repo / Author.

Key Highlights surfaced:

- 3 of 7 checkpoints now met (was 1 of 7 at v1.0.0 — Track BO derived).
- Aggregate distribution 3 met / 4 partial / 0 unmet (was 1/5/1).
- README Game Coverage table inventory (Track BU).
- Forensic verdict UNCHANGED — `validated_manjeom = 0` carries.

Honest scope explicit: 3 met flips refer ONLY to skeleton-tier surface coverage
+ honest documentation of cascading retirement; no real game frames evidenced;
real launch returns PARTIAL until closure CM-26..CM-30.

---

## BW2 — `CHANGELOG.md` v1.0.1 entry (additive)

Insert v1.0.1 section above v1.0.0 (semver convention: most recent first).
Subsections: Added / Changed / Drift resolution / Posture. Mirrors RELEASE_NOTES
content at higher level.

---

## BW3 — version bump

| File | Field | Before | After |
|---|---|---|---|
| `hexa.toml` | `[package].version` | `"1.0.0"` | `"1.0.1"` |
| `README.md` | Version badge | `version-1.0.0-informational` | `version-1.0.1-informational` |

GitHub release badge `![GitHub release](...)` picks up automatically from
`git tag v1.0.1` push (deferred to post-merge user action; this cycle does
not push tags).

---

## In-place writes

| File | Op |
|---|---|
| `hexa.toml` | version field flip 1.0.0 → 1.0.1 |
| `README.md` | version badge URL flip |
| `CHANGELOG.md` | additive section insertion (v1.0.1 above v1.0.0) |

`in_place_writes`: 3.
NEW files: 3 (`RELEASE_NOTES_v1.0.1.md` + this handoff doc + Track BW marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 LICENSE / 0 existing docs/*.md / 0 predecessor marker mutation.

---

## Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (CHANGELOG additive section + version field flip + badge URL flip)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no anima clone / no DRM bypass ✓
- **own2**: forensic_verdict honest unchanged; release notes explicit about validated_manjeom=0 carry; skeleton vs validated distinction preserved across all release docs ✓
- **raw**: 9 / 11 / 12 / 15 / 175 enforce ✓

---

## Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

v1.0.1 is a documentation + drift-fix release. NO promotion of validated_manjeom.

---

## Verification

- `grep '^version' hexa.toml` → `version = "1.0.1"`
- `head -10 CHANGELOG.md` shows `## [1.0.1] — 2026-05-05` above `## [1.0.0]`
- `RELEASE_NOTES_v1.0.1.md` exists with valid markdown
- README version badge shows `version-1.0.1`

---

## Predecessors

- Track BV — `state/markers/gamebox_lineage_classic_r28_drift_resolution_track_bv_landed.marker` (drift_count 1 → 0; lineage_classic_r28 cascading flip + checkpoint sideways-PASS)
- Track BU — `state/markers/gamebox_readme_game_list_and_win32_checkpoint_flip_track_bu_landed.marker` (drift_count 3 → 1; win32_50pct + win32_full flip + Game Coverage section)
- Track BT — `state/markers/gamebox_initial_extraction_landed.marker` (v1.0.0 baseline)

---

## Next-cycle hook (NOT triggered)

- `git tag v1.0.1 && git push origin v1.0.1` — user action; this cycle does not push.
- GitHub Releases auto-publish via tag push (workflow not in scope).
