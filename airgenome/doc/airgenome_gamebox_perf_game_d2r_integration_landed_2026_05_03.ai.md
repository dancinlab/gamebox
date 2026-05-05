# airgenome-gamebox — Track U perf_game_d2r rank B+C integration cycle landed (2026-05-03)

## §1 paraphrase

BG subagent (Track U, perf_game_d2r rank B+C integration cycle) extended 6 D2R-specific
performance modules with synthetic state machine round-trip rows mirroring the Track G
predecessor extension pattern (`__D2R_<DOMAIN>__ PARTIAL <stage>` emit prefix style).
Track G previously extended 2 modules (`c_d2r_first_frame_trace.hexa`,
`c_d2r_dx11_pipeline_warm.hexa`) for cond.3 (D3D11 renderer init + first-frame draw +
dx11 pipeline warm). Track U complements that work by extending 6 OTHER D2R-core
gameplay state modules covering combat / horadric / mercenary / runeword / set-item /
terror-zone domains. The two Track G modules remain read-only territory and were not
touched in this cycle.

`.roadmap.diablo2_resurrected` cond.1 evidence array augmented from 14 to 20 rows
(additive +6 — one row per extended module). cond.1 status remains `partial`
(unchanged). cond.2 (met, Track A territory) and cond.3 (partial, Track G territory)
are explicitly preserved verbatim. blocker_reason refreshed to acknowledge the
6-module synthetic state machine round-trip extension while noting that production
smoke remains gated on real D2R.exe binary execution (Battle.net launcher install
dependent — permanent external dependency).

posture = evidence_augment_partial_to_partial (NOT met promotion). Track A cond.2 met
promotion path (synthetic + opensource MPQ-listfile-min byte-equal) does NOT apply
here because cond.1 production smoke requires the real Battle.net-installed D2R.exe
binary, an external dependency permanently outside own1 boundary.

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

## §3 modules extended (6 / 26 c_d2r_* perf modules)

| # | module path                                       | LOC before | LOC after | delta | emit count | stages                                      |
|---|---------------------------------------------------|-----------:|----------:|------:|-----------:|---------------------------------------------|
| 1 | lib/perf/c_d2r_combat_state_track.hexa            | 126        | 157       | +31   | 2          | phase_cycle, transition_matrix              |
| 2 | lib/perf/c_d2r_horadric_cube_recipe.hexa          | 196        | 227       | +31   | 2          | cube_phase, recipe_match                    |
| 3 | lib/perf/c_d2r_mercenary_equip_optimize.hexa      | 191        | 224       | +33   | 2          | merc_phase, equip_optimize                  |
| 4 | lib/perf/c_d2r_runeword_recipe.hexa               | 165        | 196       | +31   | 2          | runeword_phase, rune_sequence               |
| 5 | lib/perf/c_d2r_set_item_complete_bonus.hexa       | 186        | 218       | +32   | 2          | setitem_phase, complete_scan                |
| 6 | lib/perf/c_d2r_terror_zone_track.hexa             | 163        | 195       | +32   | 2          | rotation_phase, zone_walk                   |

Total LOC delta: +190 (6 modules × ≈32 LOC each, all in 25-50 budget).
Total stage emit: 12 `__D2R_<DOMAIN>__ PARTIAL <stage>` println markers + 12 `r0_emit`
JSONL events (one per stage, 2 stages per module).

Each appended block follows the Track G predecessor pattern:
- Comment header explaining cond.1 evidence augment + permanent partial lock
- 2 stage descriptions (state machine phase walk + scaffold synthetic walk)
- 2 `r0_emit(...)` calls with `\"verdict\":\"PARTIAL\"` and `\"posture\":\"evidence_augment_partial_to_partial\"`
- 2 `println(\"__D2R_<DOMAIN>__ PARTIAL <stage>\")` markers
- Insertion is additive: between the existing emit-count assertion and `cleanup_*()`,
  preserving every prior row verbatim

## §4 .roadmap.diablo2_resurrected cond evidence augment

| cond                            | status before | status after | evidence before | evidence after | delta |
|---------------------------------|--------------:|-------------:|----------------:|---------------:|------:|
| diablo2_resurrected.cond.1      | partial       | partial      | 14              | 20             | +6    |
| diablo2_resurrected.cond.2      | met           | met          | 9               | 9              | 0     |
| diablo2_resurrected.cond.3      | partial       | partial      | 13              | 13             | 0     |

cond.1 evidence array additions (6 rows, one per Track U module):
1. combat_state_track synthetic — phase_count 4 / transition_table_off 0x140080000 / valid 46 / invalid 3
2. horadric_cube_recipe synthetic — phase_count 3 / recipe_table_off 0x140082000 / matches 3 / ingredients 8
3. mercenary_equip_optimize synthetic — phase_count 4 / merc_table_off 0x140084000 / classes 4 / dps_total 380
4. runeword_recipe synthetic — phase_count 3 / runeword_table_off 0x140086000 / runewords 8 / crafts 3 / success 2
5. set_item_complete_bonus synthetic — phase_count 3 / setitem_table_off 0x140088000 / sets 3 / complete 2 / bonus 70
6. terror_zone_track synthetic — phase_count 4 / rotation_period_s 3600 / tz_table_off 0x14008A000 / zones 10 / kills 6

blocker_reason refreshed (in-place, additive intent on the value text):
- before: "synthetic D2R.exe PE round-trip scaffold landed (... 6 stage PARTIAL emit) — real D2R.exe binary smoke ... permanent ..."
- after: "5-7 D2R perf modules synthetic state machine round-trip extended (Track U:
  combat/horadric/mercenary/runeword/set-item/terror-zone 6 c_d2r_* modules — 12
  PARTIAL emit stage 누적); production smoke gated on real D2R.exe binary (Battle.net
  launcher install dependent — permanent). cond.1 status partial 영구 유지, met 승격
  X. evidence_augment_partial_to_partial posture, NOT met promotion (Track A cond.2
  met 승격 과 다름)"

cond.1 also gains a new field `ai_native_handoff_cond_1_perf_game_d2r_integration`
pointing at this handoff doc §3, mirroring the Track G `ai_native_handoff_cond_3_*`
field-add pattern.

## §5 predecessors

| handoff                                                                                            | relation                                                                            |
|----------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------|
| state/markers/airgenome_gamebox_track_g_field_norm_d2r_cond3_landed.marker                         | Track G — schema + emit prefix style + module extension pattern reference           |
| state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker                 | Track P — BG-AGB rank B+C unblock predecessor (perf_game_d2r unblock evidence)      |
| state/markers/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed.marker                       | cond.1 evidence array original 14-row landing (Track D, this cycle augments to 20)  |
| state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker                           | cond.2 (met, Track A) baseline preserved by Track U                                 |

## §6 honest C3 caveats

C1. cond.1 met promotion is NOT possible via Track U (or any synthetic-only path). real
D2R.exe production binary smoke requires Battle.net launcher install — an external
dependency permanently outside own1 (Wine 0 / hexa-only / no DRM bypass). cond.1 is
locked at `partial` in perpetuity from this cycle's vantage point.

C2. Track U does NOT modify cond.2 (met, Track A territory — MPQ + d2s round-trip) or
cond.3 (partial, Track G territory — D3D11 renderer + first-frame draw). Both
conditions are preserved verbatim including all evidence rows, status, and
blocker_reason.

C3. evidence_augment_partial_to_partial is explicitly distinct from met promotion. cond.2
became met because synthetic + opensource MPQ-listfile-min produces byte-equal
round-trip; cond.1 has no opensource D2R.exe binary equivalent (the binary is the
proprietary product itself), so synthetic round-trip remains evidence-only forever.

C4. The 12 new `r0_emit` events all carry `\"verdict\":\"PARTIAL\"` and
`\"posture\":\"evidence_augment_partial_to_partial\"` to enforce raw 12 silent error
ban (PASS swallow forbidden — explicit PARTIAL marker on every synthetic stage).

C5. Module selection rationale: combat / horadric / mercenary / runeword / set-item /
terror-zone are representative D2R-core gameplay state machines (post-Phase-4 launch,
Phase-5 endgame loop). 4 of the 26 c_d2r_* modules in the candidate list were not
selected (terror_zone was selected; act_progression / cow_level_track / save_compat
etc. remain candidates for Track V/W). unique_drop_table was deferred (RNG-driven, not
a clean state machine — better suited to a future loot-domain track).

C6. The 2 Track G modules (`c_d2r_first_frame_trace.hexa`, `c_d2r_dx11_pipeline_warm.hexa`)
were read for pattern reference but NOT modified (read-only territory). git diff
shows zero lines added or removed for those two files.

C7. PARTIAL emit prefixes use D2R-domain-specific tags
(`__D2R_COMBAT__` / `__D2R_HORADRIC__` / `__D2R_MERC__` / `__D2R_RUNEWORD__` /
`__D2R_SETITEM__` / `__D2R_TERROR__`) parallel to Track G's
`__D2R_TRACE__` / `__D2R_WARM__`. All follow the verifier `status_emit` schema family
established by cond.1's `__D2R_LAUNCH__` and cond.3's `__D2R_RENDERER__`.

C8. in_place_writes on Track U: 6 module files (additive only, prepend before
`cleanup_*()` and after the existing emit assertion) + 1 `.roadmap.diablo2_resurrected`
JSONL surgical edit (cond.1 evidence array extension 14→20 + blocker_reason value
refresh + new `ai_native_handoff_cond_1_perf_game_d2r_integration` field). 0 row
deletion across all 7 modified files. additive_only spirit preserved.

C9. own1 enforcement: zero Wine / CrossOver / Whisky / GPTK references. Zero real D2R
binary execution attempt. Zero Battle.net launcher invocation. Zero DRM / Warden /
account-auth bypass. The synthetic byte-array offsets (`0x140080000` etc.) are
inline-only literals and do NOT correspond to any reverse-engineered real binary
layout — they are fictitious scaffold pointers chosen above the cond.3 RVA range to
avoid clash with Track G's `0x14007xxxx` family.

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

C12. Track U does not depend on any test harness creation (unlike Track G which created
`tests/test_d2r_d3d11_synthetic_scaffold.hexa`). The 6 modules' existing `self_test()`
serves as the verifier — adding a separate test harness would exceed cap_minutes and
duplicate the in-module assertions already in place (`r0_session_count(sid) >= N`
already enforces emit count regression).

C13. cond.1 evidence count delta of +6 reflects the per-module evidence row pattern
(one row per extended module) rather than per-stage (which would have been +12). This
keeps the evidence array proportional to module-level changes, mirroring Track G's
+9 delta against 3 extended modules (~3 rows per module).

## §7 verifier emit log expected samples

```
__D2R_COMBAT__ PARTIAL phase_cycle
__D2R_COMBAT__ PARTIAL transition_matrix
__D2R_HORADRIC__ PARTIAL cube_phase
__D2R_HORADRIC__ PARTIAL recipe_match
__D2R_MERC__ PARTIAL merc_phase
__D2R_MERC__ PARTIAL equip_optimize
__D2R_RUNEWORD__ PARTIAL runeword_phase
__D2R_RUNEWORD__ PARTIAL rune_sequence
__D2R_SETITEM__ PARTIAL setitem_phase
__D2R_SETITEM__ PARTIAL complete_scan
__D2R_TERROR__ PARTIAL rotation_phase
__D2R_TERROR__ PARTIAL zone_walk
```

Each module additionally retains its prior `self_test PASS` summary line. r0_emit
session counts are bumped by 2 per module above the prior assertion floor — original
assertions still pass since they assert `>=` (not `==`).

## §8 next-cycle action candidates

- Track V — extend remaining D2R perf modules (act_progression / cow_level_track /
  experience_curve_predict / shrine_effect_track / belt_potion_auto / charm_inventory)
  to bring cond.1 evidence row coverage to ≥ 26 rows
- Track W — synthetic state machine round-trip for cond.2 (MPQ + d2s) post-met
  evidence augment (purely cosmetic — cond.2 already met)
- Track X — cross-game evidence count audit script comparing cond.1 evidence row
  density across d2r / d4 / cs2 / wow per-game roadmaps
- Track Y — `tests/test_d2r_perf_synthetic_state_machine_round_trip.hexa` harness
  driving all 6 Track U modules (deferred from this cycle to honor cap_minutes)
- production smoke remains gated on user-side Battle.net install of real D2R.exe
  (permanent external dependency; not Track-Z-resolvable from within own1)
