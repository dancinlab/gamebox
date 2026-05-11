# gamebox v1.0.2 Release Notes — Track CD Landed (2026-05-05)

> Track CD on gamebox standalone repo, branch `bg/layout-migration-2026-05-05`.
> Patch release bundling post-v1.0.1 work: layout migration `1d28b4c` + Tracks
> BX, BY, BZ, CA, CB, CC. Hexa.toml version bump 1.0.1 → 1.0.2.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `bg/layout-migration-2026-05-05`.
- Predecessor: Track CC (per_game §9 closure 2026-05-05).
- v1.0.1 (Track BW): drift resolution + win32 milestone bundle 2026-05-05.
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## Mission

CD1 — `RELEASE_NOTES_v1.0.2.md` NEW file authored:

- Summary section bundling layout migration + 6 Tracks (BX, BY, BZ, CA, CB, CC).
- Highlights surfacing canonical `<feature>/doc/` layout, all per_game §9 closures, elden_ring DLC scope, nightreign fan-out, README inventory accuracy refresh.
- Cumulative state table comparing v1.0.0 / v1.0.1 / v1.0.2.
- Path migration note for external consumers (sed sweep applied to 193 files / 621 lines).
- Source repo + branch + author boilerplate.

CD2 — `CHANGELOG.md` v1.0.2 section additive insertion above v1.0.1.

CD3 — `hexa.toml` version `1.0.1` → `1.0.2` + README badge URL refresh.

CD4 — handoff doc + marker (this file + Track CD marker).

---

## Release contents

| Track | Marker | Scope |
|---|---|---|
| `1d28b4c` | (commit, no marker) | Layout migration: `docs/<prefix>_*.ai.md` → `<prefix>/doc/` |
| BX | `gamebox_per_game_readme_post_bu_bv_update_track_bx_landed` | per_game §9 for 6 offline-relevant files |
| BY | `gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed` | wow remove + elden_ring DLC + nightreign fan-out |
| BZ | `gamebox_closure_orchestrator_paper_invocation_track_bz_landed` | closure_orchestrator real-invocation attempt + paper-mode derivation |
| CA | `gamebox_hexa_upstream_rfc_016_reassessment_track_ca_landed` | RFC 016 reassessment 28-cycle null floor reaffirmed |
| CB | `gamebox_readme_inventory_drift_refresh_track_cb_landed` | README inventory text accuracy refresh (5 drift items) |
| CC | `gamebox_per_game_readme_post_bu_bv_update_track_cc_landed` | per_game §9 closure for 4 retired_intentional games |

---

## In-place writes

| File | Op |
|---|---|
| `hexa.toml` | version field flip 1.0.1 → 1.0.2 |
| `README.md` | version badge URL flip |
| `CHANGELOG.md` | additive section insertion (v1.0.2 above v1.0.1) |

`in_place_writes`: 3.
NEW files: 3 (`RELEASE_NOTES_v1.0.2.md` + this handoff doc + Track CD marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 LICENSE / 0 existing airgenome/doc/ / 0 existing gamebox/doc/ / 0 existing docs/*.md / 0 predecessor marker mutation / 0 gamebox-native marker mutation.

---

## Cumulative state (post v1.0.2)

| metric | v1.0.0 | v1.0.1 | v1.0.2 |
|---|---|---|---|
| .roadmap files | 14 | 14 | **15** (14 active + 1 historical wow) |
| .checkpoint met | 0 | 3 | 3 (CB inventory drift fix; same met count) |
| per_game §9 sections | 0 | 0 | **11** (BX 6 + BY 1 + CC 4) |
| gamebox-native markers | 1 (BT) | 7 (BT-CA) | **9** (BT-CC) |
| RFC 016 audit cycles cumulative | 21 (BS) | 21 (BS) | **28** (BS+CA) cumulative null floor |
| Layout | flat `docs/` | flat `docs/` | **`<feature>/doc/`** |

---

## Policy compliance

- **migration**: forbidden ✓ (CD does NOT introduce migration; layout migration was `1d28b4c`, separate commit by user)
- **changes**: additive_only ✓
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

v1.0.2 is documentation + inventory refresh + layout migration release. NO
promotion of validated_manjeom. NO module count change in artifacts.

---

## Verification

- `grep '^version' hexa.toml` → `version = "1.0.2"`
- `head -5 CHANGELOG.md` shows `## [1.0.2] — 2026-05-05` above `## [1.0.1]`
- `RELEASE_NOTES_v1.0.2.md` exists with valid markdown
- README version badge shows `version-1.0.2`

---

## Predecessors

- Track CC — `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_cc_landed.marker`
- Track CB — `state/markers/gamebox_readme_inventory_drift_refresh_track_cb_landed.marker`
- Track CA — `state/markers/gamebox_hexa_upstream_rfc_016_reassessment_track_ca_landed.marker`
- Track BZ — `state/markers/gamebox_closure_orchestrator_paper_invocation_track_bz_landed.marker`
- Track BY — `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker`
- Track BX — `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_bx_landed.marker`
- layout migration commit `1d28b4c`
- v1.0.1 (Track BW) — `state/markers/gamebox_v1_0_1_release_notes_track_bw_landed.marker`

---

## Next-cycle hooks (NOT triggered)

- `git tag v1.0.2 && git push origin v1.0.2` — user action; this cycle does not push tags.
- Branch merge `bg/layout-migration-2026-05-05` → `main` — user decision.
- GitHub Releases auto-publish via tag push — workflow not in scope.
- Track CE: closure_orchestrator selftest paper-mode (BZ §11 hook).
- Track CF: hexa.toml [modules] audit + alignment.
