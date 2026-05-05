# airgenome-gamebox — Track AK perf_game_d2r extension cycle landed (2026-05-04)

## §1 paraphrase

BG subagent (Track AK, perf_game_d2r extension cycle) extended 6 additional D2R-specific
performance modules with synthetic state machine round-trip rows mirroring the
Track G/U/AI predecessor extension pattern (`__D2R_<DOMAIN>__ PARTIAL <stage>` emit
prefix style). Track G previously extended 2 modules (`c_d2r_first_frame_trace.hexa`,
`c_d2r_dx11_pipeline_warm.hexa`) for cond.3. Track U extended 6 modules (combat /
horadric_cube / mercenary / runeword / set_item / terror_zone) for cond.1. Track AI
extended 6 modules (act_progression / belt_potion_auto / charm_inventory /
hireling_state / horadric_quest / shrine_effect_track) for cond.1. Track AK
complements those by extending 6 OTHER untouched c_d2r_* modules covering character
skill tree / class skill synergy / experience curve predict / item socket recipe /
waypoint activation order / cow level track domains. The 14 prior Track G/U/AI modules
remain read-only territory and were not touched in this cycle.

`.roadmap.diablo2_resurrected` cond.1 evidence array augmented from 26 to 32 rows
(additive +6 — one row per extended module). cond.1 status remains `partial`
(unchanged). cond.2 (met, Track A territory) and cond.3 (partial, Track G territory)
are explicitly preserved verbatim. blocker_reason refreshed to acknowledge the
Track AI+AK 12-module cumulative synthetic state machine round-trip extension while
reaffirming that production smoke remains gated on real D2R.exe binary execution
(Battle.net launcher install dependent — permanent external dependency).

posture = evidence_augment_partial_to_partial (NOT met promotion). cond.1 production
smoke requires the real Battle.net-installed D2R.exe binary, an external dependency
permanently outside own1 boundary. Cumulative D2R perf-module synthetic stage emit
count after Track AK: 36 PARTIAL stage emits (Track U 12 + Track AI 12 + Track AK 12).
Cumulative c_d2r_* perf-module extension count post-Track AK: 20/26.

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

## §3 modules extended (6 / 26 c_d2r_* perf modules; cumulative 20/26 with Track G/U/AI)

| # | module path                                       | LOC before | LOC after | delta | emit count | stages                                      |
|---|---------------------------------------------------|-----------:|----------:|------:|-----------:|---------------------------------------------|
| 1 | lib/perf/c_d2r_char_skill_tree.hexa               | 117        | 151       | +34   | 2          | class_phase, tree_distribution              |
| 2 | lib/perf/c_d2r_class_skill_synergy.hexa           | 191        | 226       | +35   | 2          | synergy_phase, chain_aggregation            |
| 3 | lib/perf/c_d2r_experience_curve_predict.hexa      | 186        | 220       | +34   | 2          | xp_phase, eta_forecast                      |
| 4 | lib/perf/c_d2r_item_socket_recipe.hexa            | 116        | 150       | +34   | 2          | socket_phase, recipe_match                  |
| 5 | lib/perf/c_d2r_waypoint_activation_order.hexa     | 185        | 221       | +36   | 2          | wp_phase, act_distribution                  |
| 6 | lib/perf/c_d2r_cow_level_track.hexa               | 190        | 226       | +36   | 2          | cow_phase, difficulty_open_matrix           |

Total LOC delta: +209 (6 modules × ≈34-36 LOC each, all in 25-50 budget).
Total stage emit: 12 `__D2R_<DOMAIN>__ PARTIAL <stage>` println markers + 12 `r0_emit`
JSONL events (one per stage, 2 stages per module).

Each appended block follows the Track G/U/AI predecessor pattern:

- Inserted between the existing `r0_session_count(sid) >= N` emit-count assertion
  and the `cleanup_*()` call inside the existing `self_test()` function body.
- Two stage-emit pairs per module: one `r0_emit("c_d2r_<module>_synthetic_<stage>", …)`
  + one `println("__D2R_<DOMAIN>__ PARTIAL <stage>")`.
- Synthetic offsets allocated above Track AI's 0x140096000 ceiling: char_skill_tree
  0x140098000, class_skill_synergy 0x14009A000, experience_curve_predict 0x14009C000,
  item_socket_recipe 0x14009E000, waypoint_activation_order 0x1400A0000,
  cow_level_track 0x1400A2000. All offsets are fictitious scaffold pointers — they
  do **not** correspond to real D2GAME.dll RVA values.
- Each emit JSONL payload includes `verdict:"PARTIAL"`,
  `posture:"evidence_augment_partial_to_partial"`, and
  `real_binary_gap:"permanent — Battle.net launcher install dependent"`.

## §4 sha256 pin

| path                                                  | sha256                                                             |
|-------------------------------------------------------|--------------------------------------------------------------------|
| lib/perf/c_d2r_char_skill_tree.hexa                   | `8d4d6bcb86a226c6529e7cf045a598ef39abe3298bad9f1602c5ff0f755688a1` |
| lib/perf/c_d2r_class_skill_synergy.hexa               | `353ed32b30f1157fb2f4600f8b3de1a002d0f94e6f39ffa652fec3296cc1d251` |
| lib/perf/c_d2r_experience_curve_predict.hexa          | `1350dbe5461cb4257e013e7eb4714ecb149aef939ee090af4013ee94a9a2bd36` |
| lib/perf/c_d2r_item_socket_recipe.hexa                | `f5b7848c7567a0c0257e092ac2eb3b581a198bbc521b8c50395cb418b9a44592` |
| lib/perf/c_d2r_waypoint_activation_order.hexa         | `ceca3a5658cb84d182afc4c356df33184e9a0354d16b151410f2dc8942715e32` |
| lib/perf/c_d2r_cow_level_track.hexa                   | `4fab5fafcd5a1b49796ab8c29d286ee4d0311583b58aa30ceb80b59770a699cd` |
| .roadmap.diablo2_resurrected                          | `9e1710cc2dcf07c082731acc026e8db70728149c317129d1df4ec1c04b87617c` |

## §5 cond evidence summary

| cond id                    | status before | status after | evidence before | evidence after | delta |
|----------------------------|---------------|--------------|----------------:|---------------:|------:|
| diablo2_resurrected.cond.1 | partial       | partial      | 26              | 32             | +6    |
| diablo2_resurrected.cond.2 | met           | met          | 9               | 9              | 0     |
| diablo2_resurrected.cond.3 | partial       | partial      | 13              | 13             | 0     |

cond.2 and cond.3 are byte-identical (Track A and Track G territories untouched).

## §6 caveats (≥8 inline)

- **C1.** cond.1 measurement boundary: synthetic state machine round-trip is
  evidence-only. The cond.1 verifier requires real D2R.exe production smoke
  (`__D2R_LAUNCH__ PASS <stage>` from `tests/test_closure.hexa` against the real
  Battle.net-installed binary). No synthetic, opensource, or hexa-native scaffold
  can fulfill this — D2R.exe IS the proprietary product, no equivalent exists.
  Therefore cond.1 status `partial` is permanent and Track AK does **NOT** promote
  cond.1 to met.
- **C2.** evidence_augment_partial_to_partial vs met_promotion (Track A precedent):
  Track A cond.2 met promotion was possible because synthetic + opensource MPQ-listfile
  byte-equal round-trip is functionally equivalent to real production parsing.
  cond.1 has no such opensource D2R.exe equivalent, so the posture is strictly
  evidence augmentation, not met promotion.
- **C3.** module selection rationale: Track AK selected the 6 RECOMMENDED candidates
  from the mission brief (char_skill_tree / class_skill_synergy / experience_curve_predict /
  item_socket_recipe / waypoint_activation_order / cow_level_track). All 6 candidates
  were verified untouched (sha256 differed from any prior extension state — they had
  no `Track AK` or `__D2R_<DOMAIN>__` markers). The 6 alternative candidates
  (ancients_spawn_pattern / gamble_npc_roll / merc_aura_combo / offline_profile_cache /
  save_compat / unique_drop_table) remain available for a future Track (would reach
  26/26 c_d2r_* coverage).
- **C4.** synthetic offset allocation: Track AK uses 0x140098000 → 0x1400A2000 in
  0x2000 increments, all above Track AI's 0x14008C000 → 0x140096000 range. None of
  these offsets correspond to real D2GAME.dll RVA values; they are fictitious scaffold
  pointers selected only for in-emit-payload identity tagging. Real D2GAME.dll RVA
  resolution is gated on the same Battle.net launcher install boundary as cond.1.
- **C5.** in-place writes count: 7 (6 c_d2r_*.hexa modules `self_test()` append + 1
  surgical JSONL field edit on `.roadmap.diablo2_resurrected` cond.1 evidence array
  + blocker_reason value refresh + new
  `ai_native_handoff_cond_1_perf_game_d2r_extension_track_ak` field). 0 cond.N rows
  removed, 0 cond.2/cond.3 modifications, append/extend only — additive_only spirit
  preserved (matches Track AI predecessor in_place_writes=7).
- **C6.** own1 enforcement: every appended block uses hexa-native `r0_emit` +
  `println` only. Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / community wrapper 0 /
  DRM bypass 0 / D2R binary execution 0 / Battle.net launcher invocation 0. All
  synthetic byte-array offsets are inline literals — no real binary RVA lookup.
- **C7.** own2 enforcement: every PARTIAL emit is explicit
  (`verdict:"PARTIAL"`, `__D2R_<DOMAIN>__ PARTIAL <stage>`). PASS swallow is
  absolutely forbidden — cond.1 status is honestly reported as `partial` in the
  blocker_reason value refresh.
- **C8.** raw 9 / 11 / 12 / 15 / 175 compliance: all field names snake_case
  (raw 11), all paths repo-relative (raw 15), 0 verbatim user quotes (raw 175),
  PARTIAL emit explicit (raw 12), hexa-only emit (raw 9). 14 caveats inline (raw 10).
- **C9.** test harness deferral: no `tests/test_d2r_perf_synthetic_state_machine_round_trip_track_ak.hexa`
  was created in this cycle — the brief explicitly bans tool/, native/, tests/ writes
  (Track AN parallel = docs only). A future Track could add a single harness driving
  all 18 (Track U + AI + AK) modules.
- **C10.** parallel BG territory scope ban: Track AK touches **only** lib/perf/c_d2r_*.hexa
  + .roadmap.diablo2_resurrected (cond.1) + handoff doc + marker. lib/loader (Track AM
  parallel = loader_win32) is untouched. lib/perf/c_d4_* (Track AL parallel = diablo4)
  is untouched. .roadmap.diablo4 / .roadmap.loader_win32 are untouched. tool/ /
  native/ / tests/ (Track AN parallel = docs only) are untouched.
- **C11.** cumulative state after Track AK: 20/26 c_d2r_* modules extended. 6
  modules remain (ancients_spawn_pattern / gamble_npc_roll / merc_aura_combo /
  offline_profile_cache / save_compat / unique_drop_table). Cumulative D2R perf-module
  synthetic stage emit count: 36 PARTIAL stages = 12 (Track U) + 12 (Track AI) + 12
  (Track AK).
- **C12.** evidence count progression: 14 (Track D land) → 20 (Track U +6) → 26
  (Track AI +6) → 32 (Track AK +6). cond.1 evidence array growth has been monotonic
  additive across all 4 cycles (D / U / AI / AK).
- **C13.** retired_intentional posture is **NOT** applicable to cond.1: cond.1 is
  not block-acknowledged dead-end (D2R production smoke is technically achievable via
  Battle.net launcher install), it is permanently `partial` because the launcher
  install is outside own1 boundary. retired_intentional candidate status remains
  `none`.
- **C14.** raw_compliance distinct-from-Track A: Track A cond.2 met promotion path
  (synthetic + opensource MPQ byte-equal) is fundamentally different from cond.1.
  Track AK does **NOT** invoke a met-promotion reasoning chain anywhere — every
  emit, every blocker_reason refresh, every marker field reaffirms partial-permanent.

## §7 verifier emit log samples (2026-05-04)

```
__D2R_SKILL__ PARTIAL class_phase
__D2R_SKILL__ PARTIAL tree_distribution
__D2R_SYNERGY__ PARTIAL synergy_phase
__D2R_SYNERGY__ PARTIAL chain_aggregation
__D2R_XP__ PARTIAL xp_phase
__D2R_XP__ PARTIAL eta_forecast
__D2R_SOCKET__ PARTIAL socket_phase
__D2R_SOCKET__ PARTIAL recipe_match
__D2R_WP__ PARTIAL wp_phase
__D2R_WP__ PARTIAL act_distribution
__D2R_COW__ PARTIAL cow_phase
__D2R_COW__ PARTIAL difficulty_open_matrix
```

12 println markers + 12 `r0_emit` JSONL events (one pair per stage, 2 stages per
module, 6 modules).

## §8 next-cycle actions

- Track AM-d2r-final — extend remaining 6 untouched c_d2r_* modules
  (ancients_spawn_pattern / gamble_npc_roll / merc_aura_combo / offline_profile_cache /
  save_compat / unique_drop_table) — would reach 26/26 cond.1 evidence row coverage
  post-Track AK's 20/26.
- Track AN-d2r — single test harness `tests/test_d2r_perf_synthetic_state_machine_round_trip.hexa`
  driving all 20 (Track U + AI + AK) modules together — currently deferred per
  this-cycle scope ban on tests/.
- Track AO — cross-game evidence count audit script comparing cond.1 evidence row
  density across d2r/d4/cs2/wow per-game roadmaps.
- production smoke remains gated on user-side Battle.net install of real D2R.exe
  (permanent external dependency; not Track-AZ-resolvable from within own1).
- diablo2_resurrected.cond.1 real production binary smoke (Battle.net launcher
  install resolve transitive — no synthetic-to-real shortcut available).
- own2 enforcement entry-too-large + headless first_dialog 매칭 측 2 fail resolve
  cycle (pre-existing inherited, this cycle 0 new fail).
