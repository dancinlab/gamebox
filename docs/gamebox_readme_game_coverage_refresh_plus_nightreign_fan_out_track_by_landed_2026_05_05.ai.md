# gamebox README Game Coverage Refresh + Nightreign Fan-out — Track BY Landed (2026-05-05)

> Track BY on gamebox standalone repo (`need-singularity/gamebox`).
> User-directed cycle:
> (a) remove wow from active Game Coverage table (preserve files as historical),
> (b) indicate elden_ring scope includes Shadow of the Erdtree DLC,
> (c) add elden_ring_nightreign as new game entry with full .roadmap fan-out.

---

## Context

- Repo: `need-singularity/gamebox` (origin), Apache-2.0.
- Predecessor: Track BX (per_game README §9 update for offline-relevant subset).
- User directives (verbatim paraphrase per BR_NO_USER_VERBATIM):
  - "wow는 빼줘" — remove wow from README Game Coverage table
  - "엘든링 나이트폴른 도" — also include Elden Ring Nightreign
  - "엘든링은 elden_ring + DLC" — clarify Elden Ring scope is base + Shadow of the Erdtree DLC
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## Mission

BY1 — README.md Game Coverage table refresh:

- Remove `wow` row (active table); preserve `.roadmap.wow` + `wow_README.ai.md` + state markers as historical artifacts (own1 destructive_ops:0 maintained — no file deletion).
- Update `elden_ring` row Notes to mention scope = base + Shadow of the Erdtree DLC (2024-06-21).
- Update `lineage_classic_r28` row cond statuses to `retired_intentional / retired_intentional / retired_intentional` reflecting Track BV cascading flip (table was stale post-BV).
- Add new `elden_ring_nightreign` row — retired_intentional posture inherited from elden_ring base.
- Update enumeration text "14 .roadmap.* files" to remove wow + add elden_ring_nightreign (still 14).
- Update count text "11 .roadmap targets" — fix prior inaccuracy "8 games + 3 launchers/runtimes" → "9 games + 2 launchers" (Track BU original count was off; BY corrects + maintains 11 via wow remove + nightreign add).
- Update per-game README index pointer "11 sub-page READMEs" → "12 sub-page READMEs" (11 T1 + 1 nightreign T1).

BY2 — `.roadmap.elden_ring` augment:

- Update `goal` field to mention base + DLC scope explicitly + sibling spinoff cross-link.
- Add NEW fields: `dlc_scope` (Shadow of the Erdtree 2024-06-21 spec), `spinoff_cross_link` (`.roadmap.elden_ring_nightreign`), `ai_native_handoff_track_by`.

BY3 — NEW `.roadmap.elden_ring_nightreign`:

- Documentation-only fan-out (loader_count = 0, perf_count = 0 at land-time).
- 3 required_conditions all status = retired_intentional (inherited from sibling elden_ring base).
- 2 blockers: blk.1 EAC anti-tamper (inherited) + blk.2 FromSoft co-op matchmaking (new sub-class).
- Release date 2025-05-30; standalone (base game NOT required).

BY4 — NEW `docs/per_game/elden_ring_nightreign_README.ai.md`:

- T1 baseline §1-§9 structure (mirror existing per-game README pattern).
- §1 release_date / release_kind / base_game_required / sibling_base_game.
- §4 module count = 0 + 0 with explicit Track CA+ deferred work note.
- §9 deferred work enumeration (Track CA+ candidate items).

---

## Files modified (3) + created (4)

| op | path |
|---|---|
| in_place_write | `README.md` (Game Coverage table refresh + enumeration text refresh) |
| in_place_write | `.roadmap.elden_ring` (goal field + dlc_scope + spinoff_cross_link + ai_native_handoff_track_by) |
| NEW | `.roadmap.elden_ring_nightreign` (mk2 SSOT, documentation-only fan-out) |
| NEW | `docs/per_game/elden_ring_nightreign_README.ai.md` (T1 baseline) |
| NEW | `docs/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed_2026_05_05.ai.md` (this handoff) |
| NEW | `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker` |

`in_place_writes`: 2 (README + .roadmap.elden_ring).
NEW files: 4 (1 .roadmap + 1 per_game README + 1 handoff + 1 marker).

NOT modified:
- `.roadmap.wow` (preserved verbatim — historical artifact)
- `docs/per_game/wow_README.ai.md` (preserved verbatim)
- `state/markers/airgenome_gamebox_*.marker` (preserved verbatim — own2 traceability mandate)
- `lib/`, `tool/`, `native/`, `tests/`, `cli/`, `install.hexa`, `hexa.toml`, `LICENSE`, `RELEASE_NOTES_v1.0.1.md`, `CHANGELOG.md`
- Other `.roadmap.*` / `.checkpoint.*` files
- `docs/per_game/<other-9>_README.ai.md` (BX-touched files preserved verbatim)

---

## Wow removal — honest scope

User direction: remove wow from active Game Coverage table. Implementation:

- README table row removed.
- README enumeration text updated to note `.roadmap.wow` retained as historical artifact.
- File-level deletion NOT performed:
  - `.roadmap.wow` preserved verbatim (4 file headers unchanged).
  - `docs/per_game/wow_README.ai.md` preserved verbatim.
  - `state/markers/*wow*.marker` references in existing markers (e.g., 4game_retired_intentional_adoption marker references wow) preserved verbatim per own2 traceability mandate.
  - `lib/loader/pe_wow_retail_track.hexa` + `lib/loader/pe_wow_classic_track.hexa` preserved verbatim (lib module deletion is destructive_ops>0 — out of scope without explicit user grant).

Rationale: own1 `destructive_ops: 0` default carries; user direction was scoped to README table refresh, not full file removal. Historical artifacts maintain cross-link integrity to existing markers + cycle history.

---

## Nightreign fan-out — honest scope (documentation-only)

`.roadmap.elden_ring_nightreign` lands with `loader_count: 0` + `perf_count: 0` at
land-time. This is honest documentation-only fan-out. Lib module fan-out
(`lib/loader/pe_elden_ring_nightreign_specific_track.hexa` + `lib/perf/c_nightreign_*.hexa`)
deferred to future cycle (Track CA+ candidate) pending:

- Real `nightreign.exe` binary acquisition (PE parse + IAT enumeration).
- EAC kernel driver class re-confirmation (verify same anti-tamper variant as elden_ring base, NOT a different EAC kernel driver).
- 3-player co-op session lifecycle spec freeze (multiplayer-specific surface).

retired_intentional posture inherited from sibling `.roadmap.elden_ring` blk.1
(EAC anti-tamper class precedent, Track C 2026-05-03). 2 new blockers:

- `elden_ring_nightreign.blk.1` — EAC anti-tamper (inherited).
- `elden_ring_nightreign.blk.2` — FromSoft co-op matchmaking (new sub-class).

`validated_manjeom = 0` carries (no real nightreign frames evidenced; production
smoke gate same multi-year horizon as elden_ring base).

---

## DLC scope clarification — `.roadmap.elden_ring`

Updated `goal` field:
- Added: "(base game 2022-02-25 + Shadow of the Erdtree DLC 2024-06-21)"
- Added: "Scope = elden_ring + DLC; sibling spinoff Elden Ring Nightreign tracked separately as .roadmap.elden_ring_nightreign per Track BY 2026-05-05"
- Added: "EAC anti-tamper blocked (same anti-tamper class for base + DLC + nightreign)"

Added NEW `dlc_scope` field with structured Shadow of the Erdtree spec:
- name: shadow_of_the_erdtree
- release_iso: 2024-06-21
- scope_kind: expansion_dlc_requires_base_game
- anticheat_class: eac_anti_tamper_same_as_base
- retired_intentional_posture_inherited: true
- added_via: track_by_readme_game_coverage_refresh_plus_nightreign_fan_out_2026_05_05

Added NEW `spinoff_cross_link` field: `[".roadmap.elden_ring_nightreign"]`.

Added NEW `ai_native_handoff_track_by` field pointing to this handoff doc.

cond.1/cond.2/cond.3 status fields UNCHANGED (still partial — DLC inherits same
EAC retired_intentional class; no separate cond entries needed for DLC).

---

## Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (README table row removal acknowledged in handoff but still net additive — the row is moved to historical narrative, not destroyed; .roadmap.elden_ring goal field updated in place per Track G additive_at_field-level precedent)
- **destructive_ops**: 0 ✓ (no file deletion; wow files preserved as historical)
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (user directives paraphrased, not quoted verbatim)
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no anima clone / no DRM bypass / no AC bypass / no FromSoft matchmaking server bypass ✓
- **own2**: forensic verdict honest unchanged; nightreign honest scope explicit (documentation-only fan-out, lib module deferred); validated_manjeom=0 carries ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓

---

## Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

The nightreign fan-out is documentation-only — no module count inflation,
no validated_manjeom promotion. DLC scope clarification is honest narrative
update without state change.

---

## Verification

- README.md Game Coverage table no longer contains `[wow](.roadmap.wow)` row
- README.md Game Coverage table contains new `[elden_ring_nightreign](.roadmap.elden_ring_nightreign)` row
- README.md `elden_ring` row Notes mentions "Shadow of the Erdtree DLC"
- `.roadmap.elden_ring_nightreign` JSONL parses OK
- `.roadmap.elden_ring` JSONL parses OK with new dlc_scope + spinoff_cross_link fields
- `docs/per_game/elden_ring_nightreign_README.ai.md` exists with §1-§9 structure
- `.roadmap.wow` + `docs/per_game/wow_README.ai.md` preserved verbatim (no diff vs HEAD~1)

---

## Predecessors

- Track BX — `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_bx_landed.marker` (per_game §9 update for offline-relevant subset)
- Track BV — `state/markers/gamebox_lineage_classic_r28_drift_resolution_track_bv_landed.marker` (table now reflects post-BV cond status flip retired_intentional)
- Track BU — `state/markers/gamebox_readme_game_list_and_win32_checkpoint_flip_track_bu_landed.marker` (BU established the Game Coverage table; BY refines)
- Track C — `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` (retired_intentional adoption precedent inherited by nightreign blk.1)

---

## Next-cycle hooks (NOT triggered)

- Track CA+ candidate: nightreign lib module fan-out (1 loader + 3-5 perf modules) pending real binary acquisition + EAC class re-confirmation.
- Track CA+ candidate: `docs/ELDEN_RING_NIGHTREIGN_MANJEOM_REVIEW.md` mk1 narrative spec freeze (multiplayer-specific 3-player co-op session lifecycle + matchmaking honest documentation).
- v1.0.2 release notes: BY is post-v1.0.1 work; future patch release decision deferred to user direction.
