# gamebox per_game README §9 Post-Track-BU/BV Update — Track BX Landed (2026-05-05)

> Track BX on gamebox standalone repo (`dancinlab/gamebox`). Doc-only
> additive cycle: 6 per_game/<game>_README.ai.md files extended with NEW §9
> sections documenting post-Track-BU + post-Track-BV state changes that affect
> each game (direct impact for lineage_classic_r28; indirect impact via
> cross-cutting loader_win32 milestone for the other 5).

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Predecessor: Track BW (v1.0.1 release notes; surfaced BU + BV deltas at package level).
- BX scope: per-game documentation refresh — surface BU + BV at the per-game
  level so cross-link traceability is complete (each game's README.ai.md
  reflects the latest state without requiring a reader to traverse the
  checkpoint snapshot or release notes).
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## Mission

BX1 — Append §9 (post-Track-BU/BV update) to 6 per_game README files:

1. `docs/per_game/lineage_classic_r28_README.ai.md` — direct impact (BV cascading flip)
2. `docs/per_game/diablo2_resurrected_README.ai.md` — indirect (cross-cutting loader)
3. `docs/per_game/battlenet_README.ai.md` — indirect (cross-cutting loader)
4. `docs/per_game/purple_launcher_README.ai.md` — indirect (cross-cutting loader)
5. `docs/per_game/lineage_w_README.ai.md` — indirect (cross-cutting loader)
6. `docs/per_game/lineage_m_README.ai.md` — indirect (cross-cutting loader)

§9 structure (each file):

- 1-line scope statement (direct vs indirect impact)
- (lineage_classic_r28 only) cond status table land-time vs post-BV
- Event impact table (BP / BU / checkpoints — each game's relevant subset)
- Honest scope footer (`validated_manjeom = 0` carry)
- Markers added to cycle history (4 entries: BP / BU / BW / BX)

NOT modified: §1 game identity / §2 cond status snapshot land-time / §3 blocker
snapshot land-time / §4 module count / §5 cross_link / §6 mk1 narrative pointer /
§7 cycle history / §8 raw 270 triplet T1 baseline note.

The 5 retired_intentional games (cs2 / delta_force / elden_ring / wow / diablo4)
are out of scope for BX — their cond statuses were already retired_intentional
at land-time (no BV-style flip needed; no checkpoint flip impact). Future cycle
may add §9 sections for completeness if requested.

---

## Files modified (6)

| # | path | section added | impact_kind |
|---|---|---|---|
| 1 | `docs/per_game/lineage_classic_r28_README.ai.md` | §9 post-Track-BV update — direct impact | direct (BV cascading flip executed) |
| 2 | `docs/per_game/diablo2_resurrected_README.ai.md` | §9 post-Track-BU/BV update — indirect | indirect (cross-cutting loader_win32 + offline_play checkpoint) |
| 3 | `docs/per_game/battlenet_README.ai.md` | §9 post-Track-BU/BV update — indirect | indirect (cross-cutting loader_win32 + offline_play / online_play checkpoints) |
| 4 | `docs/per_game/purple_launcher_README.ai.md` | §9 post-Track-BU/BV update — indirect | indirect (cross-cutting loader_win32 + lineage_m_purple_play / lineage_w_purple_play checkpoints) |
| 5 | `docs/per_game/lineage_w_README.ai.md` | §9 post-Track-BU/BV update — indirect | indirect (cross-cutting loader_win32 + lineage_w_purple_play checkpoint) |
| 6 | `docs/per_game/lineage_m_README.ai.md` | §9 post-Track-BU/BV update — indirect | indirect (cross-cutting loader_win32 + lineage_m_purple_play checkpoint) |

---

## In-place writes

| File | Op |
|---|---|
| 6 × `docs/per_game/<game>_README.ai.md` | additive §9 section append (each file) |

`in_place_writes`: 6.
NEW files: 2 (this handoff doc + Track BX marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0 CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing-non-per_game-docs/*.md / 0 predecessor marker mutation.

---

## Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (each §9 is purely append after §8; §1-§8 untouched)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no anima clone / no DRM bypass ✓
- **own2**: forensic_verdict honest unchanged; each §9 explicit about validated_manjeom=0 carry; cross-cutting impact framed honestly (skeleton-tier surface only) ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓ (T1 baseline preserved; §9 is post-T1 supplementary update)

---

## Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

§9 sections explicitly carry `validated_manjeom = 0` — no real frames evidenced
for any of the 6 games covered.

---

## Verification

- 6 files have `## §9` heading present (`grep -c "## §9" docs/per_game/<game>_README.ai.md` = 1 each)
- 6 files reference `gamebox_per_game_readme_post_bu_bv_update_track_bx_landed.marker`
- §1-§8 untouched on all 6 files (additive append only)

---

## Predecessors

- Track BW — `state/markers/gamebox_v1_0_1_release_notes_track_bw_landed.marker` (v1.0.1 patch release surfaces BU + BV at package level; BX surfaces them at per-game level)
- Track BV — `state/markers/gamebox_lineage_classic_r28_drift_resolution_track_bv_landed.marker` (BV directly modified lineage_classic_r28 roadmap + checkpoint; BX documents this in lineage_classic_r28_README.ai.md §9)
- Track BU — `state/markers/gamebox_readme_game_list_and_win32_checkpoint_flip_track_bu_landed.marker` (BU added README Game Coverage section + 2 win32 checkpoint flips; BX documents cross-cutting impact for 5 games + 1 launcher)
- Track-T1 baseline — `state/markers/airgenome_gamebox_per_game_readme_t1_baseline_landed.marker` (initial 11-file T1 baseline 2026-05-03; BX is the first post-T1 supplementary update for offline-relevant subset)

---

## Out of scope (deferred)

- 5 retired_intentional games (cs2 / delta_force / elden_ring / wow / diablo4) §9 sections — these games had retired_intentional adopted at land-time (Track-B); BU/BV/BW had no direct or indirect cond status impact on them. May be added in future cycle for completeness.
- T2 / T3 / T4 promotion (lint extension / hierarchy connection direction / full module-by-module spec freeze) — remains future-cycle work per raw 270 promotion-day 2026-06-01 schedule.
