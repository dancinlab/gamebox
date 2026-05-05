# airgenome-gamebox — Track AI perf_game_d2r extension cycle landed (2026-05-04)

## §1 paraphrase

BG subagent (Track AI, perf_game_d2r extension cycle) extended 6 additional D2R-specific
performance modules with synthetic state machine round-trip rows mirroring the Track G/U
predecessor extension pattern (`__D2R_<DOMAIN>__ PARTIAL <stage>` emit prefix style).
Track G previously extended 2 modules (`c_d2r_first_frame_trace.hexa`,
`c_d2r_dx11_pipeline_warm.hexa`) for cond.3. Track U extended 6 modules
(combat / horadric_cube / mercenary / runeword / set_item / terror_zone) for cond.1.
Track AI complements those by extending 6 OTHER untouched c_d2r_* modules covering
act progression / belt potion auto / charm inventory / hireling state / horadric quest /
shrine effect track domains. The 8 prior Track G/U modules remain read-only territory
and were not touched in this cycle.

`.roadmap.diablo2_resurrected` cond.1 evidence array augmented from 20 to 26 rows
(additive +6 — one row per extended module). cond.1 status remains `partial`
(unchanged). cond.2 (met, Track A territory) and cond.3 (partial, Track G territory)
are explicitly preserved verbatim. blocker_reason refreshed to acknowledge the
Track AI 6-module synthetic state machine round-trip extension while reaffirming that
production smoke remains gated on real D2R.exe binary execution (Battle.net launcher
install dependent — permanent external dependency).

posture = evidence_augment_partial_to_partial (NOT met promotion). cond.1 production
smoke requires the real Battle.net-installed D2R.exe binary, an external dependency
permanently outside own1 boundary. Cumulative D2R perf-module synthetic stage emit
count after Track AI: 24 PARTIAL stage emits (Track U 12 + Track AI 12).

## §2 cycle scope (additive_only)

| Concern               | Value                                           |
|-----------------------|-------------------------------------------------|
| migration             | forbidden                                       |
| changes               | additive_only                                   |
| destructive_ops       | 0                                               |
| cost_usd              | 0                                               |
| substrate             | mac-local                                       |
| cap_minutes           | 90                                              |
| BR_NO_USER_VERBATIM   | true (no verbatim user quote in any artifact)   |
| friendly_preset       | true                                            |
| silent_land           | true (marker pin only, no chat surface)         |
| own1                  | Wine 0 / hexa-only / no real D2R binary exec    |
| own2                  | PARTIAL emit explicit, PASS swallow forbidden   |

## §3 modules extended (6 / 26 c_d2r_* perf modules; cumulative 14/26 with Track G/U)

| # | module path                                       | LOC before | LOC after | delta | emit count | stages                                      |
|---|---------------------------------------------------|-----------:|----------:|------:|-----------:|---------------------------------------------|
| 1 | lib/perf/c_d2r_act_progression.hexa               | 181        | 213       | +32   | 2          | act_phase, difficulty_matrix                |
| 2 | lib/perf/c_d2r_belt_potion_auto.hexa              | 232        | 266       | +34   | 2          | belt_phase, consume_rate_matrix             |
| 3 | lib/perf/c_d2r_charm_inventory.hexa               | 123        | 157       | +34   | 2          | charm_phase, grid_occupancy                 |
| 4 | lib/perf/c_d2r_hireling_state.hexa                | 107        | 141       | +34   | 2          | hire_phase, attr_matrix                     |
| 5 | lib/perf/c_d2r_horadric_quest.hexa                | 168        | 203       | +35   | 2          | quest_phase, pct_progression                |
| 6 | lib/perf/c_d2r_shrine_effect_track.hexa           | 196        | 231       | +35   | 2          | shrine_phase, buff_aggregation              |

Total LOC delta: +204 (6 modules × ≈34 LOC each, all in 25-50 budget).
Total stage emit: 12 `__D2R_<DOMAIN>__ PARTIAL <stage>` println markers + 12 `r0_emit`
JSONL events (one per stage, 2 stages per module).

Each appended block follows the Track G/U predecessor pattern:
- Comment header explaining cond.1 evidence augment + permanent partial lock
- 2 stage descriptions (state machine phase walk + scaffold synthetic walk)
- 2 `r0_emit(...)` calls with `\"verdict\":\"PARTIAL\"` and `\"posture\":\"evidence_augment_partial_to_partial\"`
- 2 `println(\"__D2R_<DOMAIN>__ PARTIAL <stage>\")` markers
- Insertion is additive: between the existing emit-count assertion and `cleanup_*()`,
  preserving every prior row verbatim

Synthetic byte-array offsets allocated above Track U's `0x14008x` family to avoid
clash:
- 0x14008C000 — act progression table (Track AI)
- 0x14008E000 — belt potion table (Track AI)
- 0x140090000 — charm grid table (Track AI)
- 0x140092000 — hireling table (Track AI)
- 0x140094000 — horadric quest table (Track AI)
- 0x140096000 — shrine effect table (Track AI)

## §4 .roadmap.diablo2_resurrected cond evidence augment

| cond                            | status before | status after | evidence before | evidence after | delta |
|---------------------------------|--------------:|-------------:|----------------:|---------------:|------:|
| diablo2_resurrected.cond.1      | partial       | partial      | 20              | 26             | +6    |
| diablo2_resurrected.cond.2      | met           | met          | 9               | 9              | 0     |
| diablo2_resurrected.cond.3      | partial       | partial      | 13              | 13             | 0     |

cond.1 evidence array additions (6 rows, one per Track AI module):
1. act_progression synthetic — phase_count 5 / act_table_off 0x14008C000 / slot_total 15 / completed 8
2. belt_potion_auto synthetic — phase_count 4 / belt_table_off 0x14008E000 / slots 4 / types 4 / consumes 6 / rate_pm 2
3. charm_inventory synthetic — phase_count 3 / charm_table_off 0x140090000 / grid 40 / occupied 8 / free 32 / placed 5
4. hireling_state synthetic — phase_count 4 / hire_table_off 0x140092000 / types 4 / active 4 / dead_initial 1 / dead_post_revive 0
5. horadric_quest synthetic — phase_count 6 / quest_table_off 0x140094000 / steps 6 / done_partial 4 / done_full 6 / pct 0->66->100
6. shrine_effect_track synthetic — phase_count 4 / shrine_table_off 0x140096000 / kinds 9 / active 3 / dur_us 912M / buff_pct 300 / avg 100

blocker_reason refreshed (in-place, additive intent on the value text):
- before: "5-7 D2R perf modules synthetic state machine round-trip extended (Track U:
  combat/horadric/mercenary/runeword/set-item/terror-zone 6 c_d2r_* modules — 12
  PARTIAL emit stage 누적); production smoke ... permanent ..."
- after: "6 D2R perf modules synthetic state machine round-trip extended (Track AI:
  act_progression / belt_potion_auto / charm_inventory / hireling_state /
  horadric_quest / shrine_effect_track 6 c_d2r_* modules — 12 PARTIAL emit stage
  추가, Track U 12 stage 누적 위에 24 stage 누적 도달); production smoke gated on
  real D2R.exe binary (Battle.net launcher install dependent — permanent). cond.1
  status partial 영구 유지, met 승격 X. evidence_augment_partial_to_partial posture,
  NOT met promotion (Track A cond.2 met 승격 과 다름)"

cond.1 also gains a new field `ai_native_handoff_cond_1_perf_game_d2r_extension_track_ai`
pointing at this handoff doc §3, mirroring the Track G/U `ai_native_handoff_cond_*`
field-add pattern.

## §5 predecessors

| handoff                                                                                            | relation                                                                            |
|----------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------|
| state/markers/airgenome_gamebox_perf_game_d2r_integration_landed.marker                            | Track U — schema + emit prefix style + module extension pattern (cond.1 14->20)     |
| state/markers/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed.marker                         | Track G — schema parent (cond.3 evidence augment partial->partial precedent)        |
| state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker                 | Track P — BG-AGB rank B+C unblock predecessor (perf_game_d2r unblock evidence)      |
| state/markers/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed.marker                       | Track D — cond.1 evidence array original 14-row landing                             |
| state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker                           | Track A — cond.2 (met) baseline preserved by Track AI                               |

## §6 honest C3 caveats

C1. cond.1 met promotion is NOT possible via Track AI (or any synthetic-only path).
real D2R.exe production binary smoke requires Battle.net launcher install — an external
dependency permanently outside own1 (Wine 0 / hexa-only / no DRM bypass). cond.1 is
locked at `partial` in perpetuity from this cycle's vantage point. cumulative Track G+U+AI
work brings the synthetic state-machine evidence row count to 26 but does NOT alter the
real-binary gating posture.

C2. Track AI does NOT modify cond.2 (met, Track A territory — MPQ + d2s round-trip),
cond.3 (partial, Track G territory — D3D11 renderer + first-frame draw), nor any of the
8 prior Track G/U modules (`c_d2r_first_frame_trace.hexa`, `c_d2r_dx11_pipeline_warm.hexa`,
`c_d2r_combat_state_track.hexa`, `c_d2r_horadric_cube_recipe.hexa`,
`c_d2r_mercenary_equip_optimize.hexa`, `c_d2r_runeword_recipe.hexa`,
`c_d2r_set_item_complete_bonus.hexa`, `c_d2r_terror_zone_track.hexa`). All are preserved
verbatim.

C3. evidence_augment_partial_to_partial is explicitly distinct from met promotion. cond.2
became met because synthetic + opensource MPQ-listfile-min produces byte-equal
round-trip; cond.1 has no opensource D2R.exe binary equivalent (the binary is the
proprietary product itself), so synthetic round-trip remains evidence-only forever.

C4. The 12 new `r0_emit` events all carry `\"verdict\":\"PARTIAL\"` and
`\"posture\":\"evidence_augment_partial_to_partial\"` to enforce raw 12 silent error
ban (PASS swallow forbidden — explicit PARTIAL marker on every synthetic stage).

C5. Module selection rationale: act progression / belt potion auto / charm inventory /
hireling state / horadric quest / shrine effect track are representative D2R-core
gameplay loop state machines that Track G/U did not cover — all 6 chosen from the
remaining 18 untouched c_d2r_* modules. The selection focuses on player-facing FSMs
(campaign progression, consumable lifecycle, inventory tetris, mercenary AI, quest
progression, buff timer) rather than data-table modules
(unique_drop_table, item_socket_recipe, ancients_spawn_pattern) which are RNG / lookup
oriented and better suited to a future loot-domain track.

C6. The 8 Track G/U modules were read for pattern reference but NOT modified (read-only
territory). git diff shows zero lines added or removed for those eight files. The Track AI
6 modules were also verified pristine (no prior synthetic_inline section) before edit.

C7. PARTIAL emit prefixes use D2R-domain-specific tags
(`__D2R_ACT__` / `__D2R_BELT__` / `__D2R_CHARM__` / `__D2R_HIRE__` /
`__D2R_QUEST__` / `__D2R_SHRINE__`) parallel to Track U's
`__D2R_COMBAT__` / `__D2R_HORADRIC__` / `__D2R_MERC__` / `__D2R_RUNEWORD__` /
`__D2R_SETITEM__` / `__D2R_TERROR__` and Track G's `__D2R_TRACE__` / `__D2R_WARM__`. All
follow the verifier `status_emit` schema family established by cond.1's `__D2R_LAUNCH__`
and cond.3's `__D2R_RENDERER__`.

C8. in_place_writes on Track AI: 6 module files (additive only, prepend before
`cleanup_*()` and after the existing emit assertion) + 1 `.roadmap.diablo2_resurrected`
JSONL surgical edit (cond.1 evidence array extension 20→26 + blocker_reason value
refresh + new `ai_native_handoff_cond_1_perf_game_d2r_extension_track_ai` field). 0 row
deletion across all 7 modified files. additive_only spirit preserved.

C9. own1 enforcement: zero Wine / CrossOver / Whisky / GPTK references. Zero real D2R
binary execution attempt. Zero Battle.net launcher invocation. Zero DRM / Warden /
account-auth bypass. The synthetic byte-array offsets (`0x14008C000`..`0x140096000`) are
inline-only literals and do NOT correspond to any reverse-engineered real binary
layout — they are fictitious scaffold pointers chosen ABOVE Track U's `0x14008x` range
to avoid clash.

C10. own2 enforcement: PARTIAL emit explicit on every new round-trip stage. PASS
swallow forbidden — every synthetic stage produces a `__D2R_<DOMAIN>__ PARTIAL <stage>`
println marker AND a `r0_emit(...)` JSONL event with structured verdict + posture
fields. Existing module assertions (`if cnt < N { panic }`) are preserved; new emits
land AFTER those assertions to avoid altering original test invariants.

C11. raw 9 (hexa-only): zero shim, zero binary linkage attempt, zero GPU / driver
poke. raw 11 (snake_case): all new field names snake_case throughout. raw 12 (silent
error ban): every PARTIAL emit explicit. raw 15 (env lazy): all paths repo-relative,
no `/Users/<user>/` absolute leak in any artifact. raw 175 (BR_NO_USER_VERBATIM): zero
verbatim user quote in handoff doc + marker (paraphrase only).

C12. Track AI does not depend on any test harness creation. The 6 modules' existing
`self_test()` serves as the verifier — adding a separate test harness would exceed
cap_minutes and duplicate the in-module assertions already in place
(`r0_session_count(sid) >= N` already enforces emit count regression). tests/ remains
untouched (Track AG parallel territory).

C13. cond.1 evidence count delta of +6 reflects the per-module evidence row pattern
(one row per extended module) rather than per-stage (which would have been +12),
mirroring Track U's same pattern. Cumulative evidence growth: original 14 (Track D) → 20
(Track U) → 26 (Track AI). Real-binary gap remains permanent.

C14. Parallel BG territories untouched: Track AG (tool/closure_orchestrator), Track AH
(lib/loader, .roadmap.loader_win32), Track AJ (lib/perf/c_d4_*, .roadmap.diablo4). Track AI
operates strictly within `lib/perf/c_d2r_*` + `.roadmap.diablo2_resurrected` (cond.1 only)
+ this handoff doc + 1 marker.

## §7 verifier emit log expected samples

```
__D2R_ACT__ PARTIAL act_phase
__D2R_ACT__ PARTIAL difficulty_matrix
__D2R_BELT__ PARTIAL belt_phase
__D2R_BELT__ PARTIAL consume_rate_matrix
__D2R_CHARM__ PARTIAL charm_phase
__D2R_CHARM__ PARTIAL grid_occupancy
__D2R_HIRE__ PARTIAL hire_phase
__D2R_HIRE__ PARTIAL attr_matrix
__D2R_QUEST__ PARTIAL quest_phase
__D2R_QUEST__ PARTIAL pct_progression
__D2R_SHRINE__ PARTIAL shrine_phase
__D2R_SHRINE__ PARTIAL buff_aggregation
```

Each module additionally retains its prior `self_test PASS` summary line. r0_emit
session counts are bumped by 2 per module above the prior assertion floor — original
assertions still pass since they assert `>=` (not `==`).

## §8 next-cycle action candidates

- Track AK — extend remaining 12 untouched D2R perf modules (ancients_spawn_pattern /
  char_skill_tree / class_skill_synergy / cow_level_track / experience_curve_predict /
  gamble_npc_roll / item_socket_recipe / merc_aura_combo / offline_profile_cache /
  save_compat / unique_drop_table / waypoint_activation_order) — would reach
  full 26/26 cond.1 evidence row coverage post-Track AI's 14/26
- Track AL — synthetic state machine round-trip for cond.2 (MPQ + d2s) post-met
  evidence augment (purely cosmetic — cond.2 already met)
- Track AM — cross-game evidence count audit script comparing cond.1 evidence row
  density across d2r / d4 / cs2 / wow per-game roadmaps after Track AI delta
- Track AN — `tests/test_d2r_perf_synthetic_state_machine_round_trip_track_ai.hexa`
  harness driving all 6 Track AI modules (deferred from this cycle to honor cap_minutes
  and tests/ scope ban)
- production smoke remains gated on user-side Battle.net install of real D2R.exe
  (permanent external dependency; not Track-AZ-resolvable from within own1)
