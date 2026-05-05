# airgenome-gamebox perf D2R + D4 completion (Track BA) landed 2026-05-04

## §1 mission

Track BA closes the c_d2r_* perf family at 26/26 (100%) and pushes c_d4_*
retired_intentional posture annotation to 25/27 (~92.6%).

* 6 c_d2r_* extended additively (synthetic state machine round-trip evidence,
  PARTIAL) — terminal cycle for c_d2r_* coverage.
* 6 c_d4_* annotated (RETIRED_INTENTIONAL d4_economy_synthetic posture).
* `.roadmap.diablo2_resurrected` cond.1 evidence augment (+6 rows, partial→partial).
* `.roadmap.diablo4` cond.3 evidence augment (+5 rows, partial→partial; blk.1
  verbatim preserved).

policy floor: own1 (Wine 0 / hexa-only / synthetic only / no DRM bypass / no
Warden bypass / no always-online evasion) + own2 (PARTIAL/RETIRED_INTENTIONAL
emit explicit / status not inflated / silent_error_ban). additive only,
destructive 0, cost 0, mac-local, cap 90 min, BR_NO_USER_VERBATIM,
friendly_preset, silent-land. raw 9/11/12/15/175 reflect.

## §2 c_d2r_* extension (Track BA, 6 modules)

cumulative_c_d2r_extended_after_track_ba = **26/26 (100%)** —
Track G 2 + Track U 6 + Track AI 6 + Track AK 6 + Track BA 6 = 26.

| module | LOC delta | emit stages |
|---|---|---|
| `c_d2r_ancients_spawn_pattern.hexa` | +39 | `__D2R_ANCIENTS__ PARTIAL ancients_phase` + `immunity_distribution` |
| `c_d2r_gamble_npc_roll.hexa` | +38 | `__D2R_GAMBLE__ PARTIAL gamble_phase` + `quality_buy_matrix` |
| `c_d2r_merc_aura_combo.hexa` | +37 | `__D2R_MERC_AURA__ PARTIAL aura_phase` + `combo_success_matrix` |
| `c_d2r_offline_profile_cache.hexa` | +38 | `__D2R_PROFILE__ PARTIAL profile_phase` + `overwrite_matrix` |
| `c_d2r_save_compat.hexa` | +38 | `__D2R_SAVE__ PARTIAL save_phase` + `validity_matrix` |
| `c_d2r_unique_drop_table.hexa` | +39 | `__D2R_UNIQUE_DROP__ PARTIAL drop_phase` + `tier_mf_matrix` |

evidence schema:
`{stage, phase_count, <table>_off, <kind>_count, *_synthetic, matrix_kind, shim_kind, cond=diablo2_resurrected.cond.1, verdict=PARTIAL, posture=evidence_augment_partial_to_partial, real_binary_gap=permanent — Battle.net launcher install dependent, track=BA}`

cond.1 status remains **partial** (real D2R.exe binary smoke gated on Battle.net
launcher install — permanent external dependency). met promotion = NO.

## §3 c_d4_* retired_intentional annotation (Track BA, 6 modules)

cumulative_c_d4_annotated_after_track_ba = **25/27 (~92.6%)** —
Track V 7 + Track AJ 6 + Track AL 6 + Track BA 6 = 25. remaining 2 modules
(`seasonal_pass_track`, `world_event_calendar`) deferred to future cycle.

| module | LOC delta | emit |
|---|---|---|
| `c_d4_dxr_bvh_compact.hexa` | +26 | `__D4_DXR_BVH_COMPACT__ RETIRED_INTENTIONAL d4_economy_synthetic` |
| `c_d4_living_steel_economy.hexa` | +24 | `__D4_LIVING_STEEL_ECONOMY__ RETIRED_INTENTIONAL d4_economy_synthetic` |
| `c_d4_nightmare_sigil_track.hexa` | +24 | `__D4_NIGHTMARE_SIGIL__ RETIRED_INTENTIONAL d4_economy_synthetic` |
| `c_d4_occultist_enchant_roll.hexa` | +24 | `__D4_OCCULTIST_ENCHANT__ RETIRED_INTENTIONAL d4_economy_synthetic` |
| `c_d4_whisper_bounty_optimize.hexa` | +24 | `__D4_WHISPER_BOUNTY__ RETIRED_INTENTIONAL d4_economy_synthetic` |
| `c_d4_xp_curve_predict.hexa` | +24 | `__D4_XP_CURVE__ RETIRED_INTENTIONAL d4_economy_synthetic` |

posture schema (mirrors Track A2 + Track V + Track AJ + Track AL):
`{stage:d4_economy_synthetic, domain, posture:retired_intentional, production_smoke:no_attempt, drm_bypass:0, always_online_evade:0, blocker_ref:diablo4.blk.1, blocker_class:always_online_drm, adoption_iso:2026-05-03, cond:diablo4.cond.3, verdict:RETIRED_INTENTIONAL, posture_kind:block_acknowledged_pass_equivalent, track:BA}`

`c_d4_dxr_bvh_compact` additionally annotates `cross_block_ref:loader_dx.blk.1`
(D3DMetal binary licence, DXR ray-trace path is a direct loader_dx cond.2
dependency).

cond.3 status remains **partial**, retired_intentional posture per blk.1
permanent lock.

## §4 .roadmap edits

### `.roadmap.diablo2_resurrected` cond.1 (+6 evidence rows)

* one row per Track BA module documenting synthetic state machine round-trip.
* terminal row marks `cumulative_c_d2r_extended_after_track_ba 26/26 (100%) reach`.
* status: partial → partial. cond.2 (met) and cond.3 (partial) untouched.
  `blocker_reason` text untouched. blk.1 untouched.

### `.roadmap.diablo4` cond.3 (+5 evidence rows)

* `track_ba_retired_intentional_posture_annotation_extension_landed_6_modules`
* `track_ba_retired_intentional_emit_kind_explicit_repetition_4th`
* `track_ba_d4_economy_synthetic_no_attempt_explicit_repeat_3rd`
* `track_ba_cond_3_status_partial_unchanged_blk_1_preserved_4th_repeat`
* `track_ba_cumulative_annotation_coverage_25_of_27_c_d4_modules`
* status: partial → partial. cond.1 + cond.2 evidence untouched. blk.1 verbatim
  preserved (`status_enum_adopted=retired_intentional`, `adopted_iso=2026-05-03`,
  `retired_intentional_posture=block_acknowledged_pass_equivalent`).

## §5 track isolation

Track BA touched ONLY the 6 c_d2r_* + 6 c_d4_* modules listed above plus the
two roadmap evidence sections. Read-only territories preserved:

* Track G c_d2r_* (2): `first_frame_trace`, `dx11_pipeline_warm`
* Track U c_d2r_* (6): `combat_state_track`, `horadric_cube_recipe`,
  `mercenary_equip_optimize`, `runeword_recipe`, `set_item_complete_bonus`,
  `terror_zone_track`
* Track AI c_d2r_* (6): `act_progression`, `belt_potion_auto`,
  `charm_inventory`, `hireling_state`, `horadric_quest`, `shrine_effect_track`
* Track AK c_d2r_* (6): `char_skill_tree`, `class_skill_synergy`,
  `cow_level_track`, `experience_curve_predict`, `item_socket_recipe`,
  `waypoint_activation_order`
* Track V c_d4_* (7): `helltide_chest_route`, `dungeon_seed_track`,
  `glyph_paragon_level`, `masterworking_amplify`, `tempering_affix_reroll`,
  `world_boss_spawn_window`, `world_tier_unlock`
* Track AJ c_d4_* (6): `paragon_track`, `pet_companion_track`,
  `pit_tier_speed_track`, `pso_warmup_priority`, `seasonal_mechanic_timer`,
  `codex_unlock`
* Track AL c_d4_* (6): `aspect_imprint_track`, `dungeon_sigil_inventory`,
  `dxr_first_frame_trace`, `helltide_event`, `infernal_horde_wave`,
  `loot_drop_predict`
* Track BB parallel: `c_metal_*`, `c_pe_*`, `c_install_*` not touched.
* Track AZ parallel: `lib/loader/*` not touched.
* Track BC parallel: `docs/HEXA_UPSTREAM.md` not touched.

## §6 verification

* 12 modules show LOC growth (range +24..+39).
* both roadmaps parse as JSONL via `python3 json.loads`. cond status invariants:
  d2r cond.1=partial, cond.2=met, cond.3=partial. d4 cond.1=partial,
  cond.2=partial, cond.3=partial. d4 blk.1.status_enum_adopted=retired_intentional verbatim.
* marker JSON parses with `cumulative_c_d2r_extended_after_track_ba` and
  `cumulative_c_d4_annotated_after_track_ba` fields present.
* sha256 pinned for all 12 hexa modules + 2 roadmap files in marker.

## §7 cross-refs

* docs/airgenome_gamebox_perf_game_d2r_extension_track_ai_landed_2026_05_04.ai.md (Track AI)
* docs/airgenome_gamebox_perf_game_d2r_extension_track_ak_landed_2026_05_04.ai.md (Track AK)
* docs/airgenome_gamebox_d4_retired_intentional_adoption_landed_2026_05_03.ai.md (Track A2 / blk.1 source)
* state/markers/airgenome_gamebox_perf_d2r_d4_completion_track_ba_landed.marker (this cycle pin)
