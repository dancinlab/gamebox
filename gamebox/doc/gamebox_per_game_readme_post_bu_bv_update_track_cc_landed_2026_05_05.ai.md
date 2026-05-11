# gamebox per_game §9 Closure for Remaining 4 retired_intentional Games — Track CC Landed (2026-05-05)

> Track CC on gamebox standalone repo (`dancinlab/gamebox`), branch
> `bg/layout-migration-2026-05-05`. Closes BX scope gap by appending §9
> sections to the 4 per_game READMEs that BX explicitly excluded
> (retired_intentional games: cs2 / delta_force / diablo4 / elden_ring).
> elden_ring_nightreign already has §9 from Track BY land-time so no
> append needed.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `bg/layout-migration-2026-05-05`.
- Predecessor: Track CB (README inventory drift refresh 2026-05-05).
- Driver: BX explicitly excluded the 5 retired_intentional games (cs2 / delta_force / elden_ring / wow / diablo4) from §9 append. `wow` removed from active table per Track BY direction. CC closes the remaining 4.
- elden_ring_nightreign already has §9 from BY land-time (T1 baseline §1-§9) — no append needed in CC.
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## Mission

CC1 — Append §9 (post-Track-BU/BV/BY/CA update) to 4 per_game README files:

| # | path | impact_kind | special note |
|---|---|---|---|
| 1 | `docs/per_game/cs2_README.ai.md` | indirect (cross-cutting loader_win32 + Track BM/BQ c_steam_* family completion) | retired_intentional stable (VAC AC) |
| 2 | `docs/per_game/delta_force_README.ai.md` | indirect (cross-cutting loader_win32) | retired_intentional stable (EAC kernel AC) |
| 3 | `docs/per_game/diablo4_README.ai.md` | indirect (cross-cutting loader_win32 + Track BA/BE c_d4_* family completion) | retired_intentional first-mover (always-online DRM) |
| 4 | `docs/per_game/elden_ring_README.ai.md` | **direct (Track BY DLC scope clarification + nightreign sibling spinoff)** + indirect (cross-cutting loader_win32 + Track BE/BM/BQ c_elden_* + c_sote_*) | DLC scope formally surfaced in §9 |

§9 structure (each file):

- Scope statement: cond statuses unchanged (retired_intentional stable)
- Event impact table (BP / BU / per-track BE/BM/BQ/AT family completion / BY README + DLC + 1d28b4c layout migration)
- Honest scope footer (`validated_manjeom = 0` carry; no AC/DRM bypass)
- Markers added to cycle history (4-7 entries depending on game; cs2/delta_force/diablo4 = 4 entries each; elden_ring = 7 entries due to denser perf cycle history)

NOT modified: §1-§8 on all 4 files (additive append only).

---

## Notable elden_ring §9 detail

elden_ring §9 specifically surfaces Track BY's DLC scope clarification:

> **Scope clarification (Track BY 2026-05-05)**: `.roadmap.elden_ring` scope =
> **base game (2022-02-25) + Shadow of the Erdtree DLC (2024-06-21)**. DLC
> inherits same EAC anti-tamper class as base; no separate cond entries needed
> for DLC. Sibling spinoff Elden Ring Nightreign (2025-05-30 standalone
> multiplayer co-op) tracked separately as `.roadmap.elden_ring_nightreign`...

This is the canonical user-facing surface for the DLC scope decision (BY had it
in marker JSON; CC promotes it to per_game README narrative).

---

## In-place writes

| File | Op |
|---|---|
| `docs/per_game/cs2_README.ai.md` | additive §9 section append |
| `docs/per_game/delta_force_README.ai.md` | additive §9 section append |
| `docs/per_game/diablo4_README.ai.md` | additive §9 section append |
| `docs/per_game/elden_ring_README.ai.md` | additive §9 section append (with DLC scope narrative) |

`in_place_writes`: 4.
NEW files: 2 (this handoff doc + Track CC marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0 CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/*.md / 0 existing gamebox/doc/*.md / 0 existing docs/*.md (catch-all) / 0 predecessor marker mutation / 0 gamebox-native marker mutation / 0 elden_ring_nightreign_README.ai.md (already has §9 from BY — preserved verbatim).

---

## Out of scope (preserved)

- `wow_README.ai.md` — historical artifact post Track BY (wow removed from active table); no §9 append (would be inconsistent with historical-only status)
- `elden_ring_nightreign_README.ai.md` — already has §9 from BY land-time

---

## Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (4 §9 appends; §1-§8 untouched on all 4 files)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no anima clone / no DRM bypass / no AC bypass ✓
- **own2**: forensic_verdict honest unchanged; each §9 explicit about validated_manjeom=0 carry; retired_intentional stable status explicit; skeleton vs validated distinction preserved ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓ (T1 baseline preserved on all 4 files)

---

## Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CC §9 sections explicitly carry `validated_manjeom = 0` for each game. No
real frames evidenced. retired_intentional posture stable across all 4 games
(VAC / EAC kernel / always-online DRM / EAC anti-tamper).

---

## Verification

- 4 files have NEW `## §9` heading (`grep -c "## §9" docs/per_game/<game>_README.ai.md` = 1 each)
- 4 files reference `gamebox_per_game_readme_post_bu_bv_update_track_cc_landed.marker`
- elden_ring §9 mentions "Shadow of the Erdtree DLC (2024-06-21)" and "Elden Ring Nightreign"
- §1-§8 untouched on all 4 files (additive append only)

---

## Predecessors

- Track CB — `state/markers/gamebox_readme_inventory_drift_refresh_track_cb_landed.marker` (README inventory drift refresh; CC operates on the per_game subset)
- Track BY — `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker` (DLC scope clarification + nightreign fan-out; surfaced in elden_ring §9)
- Track BX — `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_bx_landed.marker` (predecessor §9 cycle for offline-relevant subset; CC closes the retired_intentional subset)
- Track BV — `state/markers/gamebox_lineage_classic_r28_drift_resolution_track_bv_landed.marker` (drift resolution; lineage_classic_r28 §9 already has BV update from BX)
- Track BU — `state/markers/gamebox_readme_game_list_and_win32_checkpoint_flip_track_bu_landed.marker` (Game Coverage table baseline)
- layout migration commit `1d28b4c` — established `<feature>/doc/` layout

---

## Next-cycle hooks (NOT triggered)

- Track CD: v1.0.2 release notes bundling BX-CC
- Track CE: closure_orchestrator selftest paper-mode
- Track CF: hexa.toml [modules] audit
- Future cycle: `docs/VAC_HONEST_FREEZE.md` + `docs/EAC_KERNEL_HONEST_FREEZE.md` mk1 narrative spec freeze (deferred per cs2/delta_force §9 notes)
