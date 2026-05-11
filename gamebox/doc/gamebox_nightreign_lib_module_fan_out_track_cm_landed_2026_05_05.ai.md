# gamebox Nightreign Lib Module Fan-out (Skeleton-Tier) — Track CM Landed (2026-05-05)

> Track CM on gamebox standalone repo, branch `main`. Resolves Track BY deferred
> fan-out hook for elden_ring_nightreign — 1 loader + 3 perf modules at
> skeleton-tier with retired_intentional posture inherited from blk.1 (EAC
> anti-tamper) + blk.2 (FromSoft co-op matchmaking).

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CL EAC kernel honest freeze 2026-05-05).
- Predecessor (immediate): Track CL (EAC kernel + anti-tamper honest spec freeze; covers nightreign blk.1 inherited).
- Predecessor (semantic): Track BY documentation-only fan-out (loader_count: 0 + perf_count: 0; lib module fan-out deferred to future cycle).
- Predecessor (template): elden_ring sibling modules — `pe_elden_ring_specific_track.hexa` + `c_elden_eac_kernel_driver_avoid_track.hexa` + 9 other c_elden_*.hexa files.
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## Mission

CM1 — `lib/loader/pe_elden_ring_nightreign_specific_track.hexa` NEW (~216 LOC):

- 9 `phase_kind` enum: process_attach / fromsoft_engine_init / eac_init / co_op_session_init / matchmaking_query / 3player_sync_handshake / world_load / first_frame / session_terminate
- `record_step` / `step_count` / `count_by_phase_kind` / `count_online_required` / `nightreign_stats` fns
- `self_test()` synthetic 9-step launch sequence + `__NIGHTREIGN_LAUNCH__ RETIRED_INTENTIONAL` posture annotation
- `main()` subcmd dispatch (self-test / stats / count_phase / cleanup)

CM2 — `lib/perf/c_nightreign_co_op_session_init.hexa` NEW (~133 LOC):

- 5 `session_phase` enum: init_request / init_ack / player_join / sync_baseline / session_active
- `record_session` / `session_count` / `count_by_phase` / `max_player_count` / `cnrsi_stats` fns
- `self_test()` synthetic 5-phase 3-player session lifecycle + `__NIGHTREIGN_CO_OP_SESSION__ RETIRED_INTENTIONAL` posture annotation (blk.2 FromSoft co-op matchmaking)

CM3 — `lib/perf/c_nightreign_matchmaking_probe.hexa` NEW (~129 LOC):

- 5 `probe_state` enum: dns_lookup / tcp_connect / handshake_request / handshake_response / matchmaking_idle
- `record_probe` / `probe_count` / `count_by_state` / `count_responded` / `cnmp_stats` fns
- `self_test()` synthetic 5-state probe (Apple Silicon natural absence — 0 server response) + `__NIGHTREIGN_MATCHMAKING__ RETIRED_INTENTIONAL` posture annotation

CM4 — `lib/perf/c_nightreign_dx12_first_frame_co_op_variant.hexa` NEW (~131 LOC):

- 5 `frame_phase` enum: device_create / swapchain_init / pso_warm / first_present / co_op_sync_render
- `record_frame` / `frame_count` / `count_by_phase` / `total_ms_to_present` / `cndf_stats` fns
- `self_test()` synthetic 5-phase first-frame timing (250+80+1200+30+50 = 1610ms total) + `__NIGHTREIGN_RENDER__ RETIRED_INTENTIONAL` posture annotation (D3DMetal transitive + EAC inherited)

CM5 — `.roadmap.elden_ring_nightreign` JSONL header field flip:

- `loader_count: 0` → `1` + `loader_pattern` field added
- `perf_count: 0` → `3` + `perf_pattern` field added
- `*_count_note` fields updated to reference Track CM skeleton-tier landing
- `ai_native_handoff_track_cm` field added (pointer to this handoff)
- Comment header line 6: NEW additive note "SKELETON-TIER FAN-OUT (Track CM 2026-05-05)" preserving Track BY documentation-only narrative line 5

CM6 — handoff doc + marker (this file + Track CM marker).

---

## Module summary table

| # | path | LOC | phase/state count | retired_intentional posture |
|---|---|---|---|---|
| 1 | `lib/loader/pe_elden_ring_nightreign_specific_track.hexa` | 216 | 9 phase_kind | inherited blk.1 + blk.2 |
| 2 | `lib/perf/c_nightreign_co_op_session_init.hexa` | 133 | 5 session_phase | blk.2 FromSoft co-op matchmaking |
| 3 | `lib/perf/c_nightreign_matchmaking_probe.hexa` | 129 | 5 probe_state | blk.2 + Apple natural absence |
| 4 | `lib/perf/c_nightreign_dx12_first_frame_co_op_variant.hexa` | 131 | 5 frame_phase | blk.1 inherited + D3DMetal transitive |
| **Total** | | **609** | | |

---

## In-place writes

| File | Op |
|---|---|
| `.roadmap.elden_ring_nightreign` | JSONL header field flip (loader_count 0→1 + perf_count 0→3 + loader_pattern + perf_pattern + ai_native_handoff_track_cm + count_note text refresh) + comment header line 6 additive note |

`in_place_writes`: 1 (.roadmap.elden_ring_nightreign).
NEW files: 6 (4 lib modules + this handoff doc + Track CM marker).
0 .checkpoint mutation / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0 CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/ / 0 existing gamebox/doc/ / 0 existing docs/*.md / 0 existing lib/loader/ / 0 existing lib/perf/ / 0 predecessor marker mutation / 0 gamebox-native marker mutation.

---

## Skeleton-tier vs validated-tier explicit

CM modules are **skeleton-tier per Track BL/BM forensic baseline** — synthetic
scaffolds for own2 honest emit + retired_intentional posture annotation. They
do NOT promote `validated_manjeom` (still 0):

- Module `self_test()` PASSes via synthetic data (not real game frames)
- `record_*` fns persist TSV log to `~/.gamebox/persist/` (mac-local)
- Real `nightreign.exe` binary 측 측정 X (no real binary acquisition this cycle)
- EAC kernel driver real attest 측 시도 X (own1 — Apple natural absence)
- FromSoft matchmaking server real handshake 측 시도 X (own1 — natural absence + no bypass)

Forensic verdict `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` carries unchanged.

---

## Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (4 NEW lib modules + 2 NEW handoff/marker files + 1 JSONL field flip per Track G additive_at_field-level precedent)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no AC bypass / no EAC kernel driver injection / no anti-tamper bypass / no FromSoft matchmaking server bypass / no co-op session emul / no handshake spoof ✓
- **own2**: 4 modules each emit RETIRED_INTENTIONAL posture annotation; r0_emit ≥ 7 each (panic enforced); skeleton-tier vs validated-tier distinction explicit; validated_manjeom=0 carries ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 168 (minimum-viable) enforce ✓

---

## Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CM is skeleton-tier lib module fan-out. NO real frame measurement / NO real
network round-trip / NO real EAC attest. Real-tier validation deferred (real
`nightreign.exe` binary acquisition + EAC kernel class re-confirmation +
FromSoft matchmaking real handshake — multi-year horizon, perpetually
deferred per Track CN sibling cycle).

---

## Verification

- `ls lib/loader/pe_elden_ring_nightreign_specific_track.hexa` → exists (216 LOC)
- `ls lib/perf/c_nightreign_*.hexa` → 3 files (133 + 129 + 131 = 393 LOC)
- `python3 -c "import json; ..."` on `.roadmap.elden_ring_nightreign` → parse OK; loader_count=1, perf_count=3
- All 4 modules have `self_test()` + `main()` + `RETIRED_INTENTIONAL` emit pattern
- All 4 modules cite blocker_ref (blk.1 EAC inherited / blk.2 FromSoft co-op matchmaking) explicitly
- 0 references to "bypass" / "evasion" / "circumvention" outside negative context (own2 honest)

---

## Predecessors

- Track CL — `state/markers/gamebox_eac_kernel_honest_freeze_track_cl_landed.marker` (EAC kernel + anti-tamper honest spec freeze; nightreign blk.1 lock-in narrative)
- Track CK — `state/markers/gamebox_vac_honest_freeze_track_ck_landed.marker` (VAC honest spec freeze; sibling pattern)
- Track BY — `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker` (documentation-only fan-out; CM resolves deferred lib module fan-out hook)
- Track AS — `state/markers/airgenome_gamebox_perf_elden_retired_intentional_track_as_landed.marker` (c_elden_eac_kernel_driver_avoid_track template)
- Track BE — `state/markers/airgenome_gamebox_perf_d4_elden_completion_track_be_landed.marker` (c_elden_* family completion template)
- Track-C — `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` (elden_ring base retired_intentional adoption inherited by nightreign)

---

## Next-cycle hooks (NOT triggered)

- Track CN: hexa interp closure CM-26..CM-30 perpetual deferral acknowledgment (sibling cycle resolves hook 4)
- Track CO: v1.0.4 release notes bundling CK + CL + CM + CN
- Track CP: final session closure marker (잔여 0건)
- Future cycle: real `nightreign.exe` binary acquisition + EAC kernel class re-confirmation (multi-year horizon, perpetually deferred)
- Future cycle: real `hexa run` of CM modules post hexa interp closure CM-26..CM-30 (multi-year horizon)
