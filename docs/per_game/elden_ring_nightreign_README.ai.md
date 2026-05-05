---
schema: gamebox/docs/per_game/elden_ring_nightreign_readme/ai-native/1
last_updated: 2026-05-05
roadmap_ssot: .roadmap.elden_ring_nightreign
status: T1_BASELINE_LANDED_2026_05_05
related_raws:
  - raw 270  # ai-native readme triplet T1 baseline
  - raw 271  # core+module pattern (per-game sub-bucket)
  - raw 11   # snake_case
  - raw 12   # silent-error ban
  - raw 175  # BR-NO-USER-VERBATIM
---

# elden_ring_nightreign README.ai.md (T1 baseline)

## §1 game identity

| field | value |
|---|---|
| name | elden_ring_nightreign |
| alias | nightreign, ern, elden_ring_nightreign |
| kind | game |
| release_date | 2025-05-30 |
| release_kind | standalone multiplayer co-op spinoff (FromSoftware) |
| base_game_required | false (standalone — does not require Elden Ring base game) |
| sibling_base_game | `.roadmap.elden_ring` (base + Shadow of the Erdtree DLC) |

## §2 cond status snapshot (read from .roadmap.elden_ring_nightreign at land-time 2026-05-05 Track BY)

| cond | desc | status |
|---|---|---|
| elden_ring_nightreign.cond.1 | nightreign.exe PE parse + EAC offline launch probe | retired_intentional (inherited from elden_ring base) |
| elden_ring_nightreign.cond.2 | D3D12 first-frame + PSO warm + FromSoft Dantelion engine surface | retired_intentional (inherited) |
| elden_ring_nightreign.cond.3 | 3-player multiplayer co-op session lifecycle | retired_intentional (online matchmaking + EAC transitive) |

## §3 blocker snapshot

| blocker | type | status | status_enum_adopted | adopted_iso | adopted_via |
|---|---|---|---|---|---|
| elden_ring_nightreign.blk.1 | upstream_anticheat_anti_tamper (eac_anti_tamper) | open | retired_intentional | 2026-05-05 | track_by_nightreign_fan_out_inherited_from_elden_ring_base |
| elden_ring_nightreign.blk.2 | upstream_network_matchmaking (fromsoft_co_op_matchmaking) | open | retired_intentional | 2026-05-05 | track_by_nightreign_fan_out_co_op_matchmaking_class |

retired_intentional_posture: `block_acknowledged_pass_equivalent_inherited_from_elden_ring_base` (blk.1) + `block_acknowledged_pass_equivalent` (blk.2)

## §4 module count (Track BY documentation-only fan-out)

- loader_count: **0** (lib/loader/pe_elden_ring_nightreign_specific_track.hexa fan-out deferred to future cycle Track CA+ candidate)
- perf_count: **0** (lib/perf/c_nightreign_*.hexa fan-out deferred to future cycle)

Rationale: Track BY 2026-05-05 documentation-only fan-out cycle — establishes
.roadmap SSOT + per_game README T1 baseline + cross-link references; lib module
fan-out requires real binary acquisition + EAC kernel driver class re-confirmation
which are deferred to a future cycle. This is the documentation-only honest
scope.

## §5 cross_link to sister roadmaps + manjeom_review docs

sister_roadmaps:
- `.roadmap.loader_pe`
- `.roadmap.loader_win32`
- `.roadmap.loader_dx`
- `.roadmap.elden_ring` (sibling base game + DLC)
- `.roadmap.cs2`
- `.roadmap.delta_force`

base_game_cross_link: `.roadmap.elden_ring` (Elden Ring base 2022-02-25 + Shadow of the Erdtree DLC 2024-06-21)

mk1_narrative_refs:
- `docs/ELDEN_RING_MANJEOM_REVIEW.md` (sibling base game spec freeze; retired_intentional posture inherited)

anticheat_family_cross_ref: elden_ring (EAC anti-tamper base + DLC), cs2 (VAC user-mode AC), delta_force (EAC kernel-AC)

## §6 mk1 narrative pointer

mk1 narrative inherited from sibling base game spec freeze `docs/ELDEN_RING_MANJEOM_REVIEW.md` — own1 Wine 0 + own2 honest emit, no AC bypass / no anti-tamper bypass / no matchmaking server circumvention. EAC anti-tamper retired_intentional formal adoption inherited from elden_ring base via Track C 4-game adoption (2026-05-03).

Nightreign-specific manjeom doc not yet landed — future cycle Track CA+ candidate (multiplayer co-op session lifecycle spec freeze + 3-player matchmaking honest documentation).

## §7 cycle history (markers referencing this game)

- `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` (Track C 4-game retired_intentional 2026-05-03 — base game adopted; nightreign inherits class via Track BY)
- `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker` (this cycle 2026-05-05 — Track BY documentation-only fan-out)

## §8 raw 270 triplet T1 baseline note

This README.ai.md satisfies the raw 270 triplet T1 baseline as of 2026-05-05 (Track BY landing — first-class fan-out from user request "엘든링 나이트폴른 도" + "엘든링은 elden_ring + DLC"). T2 (lint extension), T3 (hierarchy connection direction), T4 (full module-by-module spec freeze) remain future-cycle work. Lib module fan-out (T4 prerequisite) deferred to Track CA+ candidate pending real binary acquisition + EAC class re-confirmation.

## §9 deferred work (Track CA+ candidate)

The following items are deferred to a future cycle and are NOT in scope for Track BY:

- **lib/loader/pe_elden_ring_nightreign_specific_track.hexa** — PE loader for nightreign.exe (1 module, mirror sibling pe_elden_ring_specific_track.hexa structure)
- **lib/perf/c_nightreign_*.hexa** — multiplayer-specific perf modules (estimated 3-5 modules: c_nightreign_co_op_session_init / c_nightreign_matchmaking_probe / c_nightreign_3player_sync_track / c_nightreign_session_terminate / c_nightreign_dx12_first_frame_co_op_variant)
- **EAC kernel driver class re-confirmation** — verify nightreign uses same EAC anti-tamper variant as elden_ring base (not a different EAC kernel driver) before lib module fan-out
- **Real binary acquisition** — nightreign.exe + binary path + IAT real-resolve target enumeration
- **docs/ELDEN_RING_NIGHTREIGN_MANJEOM_REVIEW.md** — multiplayer-specific mk1 narrative spec freeze (3-player co-op session lifecycle + matchmaking honest documentation)

Honest scope: Track BY establishes the .roadmap + per_game README skeleton without
inflating module counts. validated_manjeom = 0 (no real frames evidenced); same
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 forensic verdict carries.
