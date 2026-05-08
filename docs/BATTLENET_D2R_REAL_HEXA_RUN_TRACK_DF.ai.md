// docs/BATTLENET_D2R_REAL_HEXA_RUN_TRACK_DF.ai.md
# gamebox Battle.net + D2R Fastest-Arriving Real Hexa Run — Exhaustion (Track DF, 2026-05-09)

> Track DF on gamebox standalone repo, branch `main`. User directive
> "battle.net + d2r 가장 빠르게 도착할 흐름 kick (고갈시까지) 후 진행" —
> kicked the fastest-arriving real-execution path (single-shot per-module
> `HEXA_SHIM_NO_DARWIN_LANDING=1 hexa run <module> self-test`) across all
> 11 battle.net loader cluster modules + 6 D2R loader cluster modules + 3
> D2R-related test harnesses, observed 20 of 20 PASS, then ran
> `closure_orchestrator.hexa emit` to land a fresh `closure_progress`
> marker. **Skeleton-tier real-execution-confirmed for the joined
> battle.net + D2R cluster.** No `validated_manjeom` promotion (real game
> frame remains gated on CQ-P5 surface flips).

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track DE full-repo audit landing).
- Predecessor (immediate): Track DE.
- Predecessor (semantic, battle.net cluster full closure): Track DD.
- Predecessor (semantic, parent battle.net roadmap): Track CQ.
- Forensic verdict (UNCHANGED at top tier): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- Trigger: user directive (verbatim Korean: "battle.net + d2r 가장 빠르게 도착할 흐름 kick (고갈시까지) 후 진행").

Track DF is the joined battle.net + D2R real-run sweep. Track DD already
landed the 11 battle.net modules in their cluster-internal full-closure
form. Track DF revisits that cluster + extends to the 6 D2R loader
modules (which have not previously been swept as a single-pass real-run
batch) + extends further to the 3 D2R-related test harnesses, and emits
a fresh post-DE `closure_progress` marker. The "fastest-arriving" path
is one `hexa run` per module — no batched test runner, no orchestrator
self-test, no marker accumulation between modules — to minimize per-module
latency and maximize per-step verifiability.

---

## §1 Real-run observation — 20 of 20 PASS

Invocation pattern: `HEXA_SHIM_NO_DARWIN_LANDING=1 hexa run <path> self-test`
(remote `hexa run` on resource host `ubu-1`, per `~/core/resource/bin/hexa-r`).

### §1.1 Battle.net loader cluster (11 of 11 PASS)

| # | Module | Verdict | Summary |
|---|---|---|---|
| 1 | `lib/loader/battlenet_bypass.hexa` | PASS | deps=6, d2r feasible, d4 blocked, sid=bnetbp-1778263403 |
| 2 | `lib/loader/pe_battle_net_agent_ipc_track.hexa` | PASS | msgs=6, payload_bytes=2688, launch_requests=2, +4 synthetic IPC PARTIAL round-trips, emits=7 |
| 3 | `lib/loader/pe_battle_net_download_track.hexa` | PASS | chunks=8, bytes=2623488, verifies=2, emits=9 |
| 4 | `lib/loader/pe_battle_net_oauth_token.hexa` | PASS | ops=6, token_lifetime_sec=345600, token_size_bytes=5120, refresh=2, emits=7 |
| 5 | `lib/loader/pe_battle_net_setup_install_phase.hexa` | PASS | steps=6, loaded=4, elevated=2, cdn=1, launchd_install=1, emits=7 |
| 6 | `lib/loader/pe_battlenet_agent_http_rest.hexa` (CZ-augmented) | PASS | req=5, oauth=4, launch=3, install=4, resp=4, +4 synthetic HTTP-REST PARTIAL round-trips, emits=21 |
| 7 | `lib/loader/pe_battlenet_agent_sd_forging.hexa` | PASS | agent_detect=2, sd_forging=1, dacl_entry=3, token_query=4, access_check=2, emits=19 |
| 8 | `lib/loader/pe_battlenet_apple_network_http.hexa` (DB-authored) | PASS | req=6, tls=6, resp=6, distinct_xid=6, emits=19 |
| 9 | `lib/loader/pe_battlenet_setup_install_path_real.hexa` | PASS | phase=10, user_action=1, network_event=4, file_op=5, install_metric=4, success=10, emits=31 |
| 10 | `lib/loader/pe_battlenet_setup_static_crt_path.hexa` | PASS | static_milestone=8, iat_dependency=4, inline_function_size=4, text_range=2, progress_milestone=4, emits=25 |
| 11 | `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa` (DC-authored) | PASS | manifest=4, blte=4, md5=4, lzma=3, install=3, distinct_sid=2, emits=19 |

### §1.2 D2R loader cluster (6 of 6 PASS)

| # | Module | Verdict | Summary |
|---|---|---|---|
| 12 | `lib/loader/d2r_launch.hexa` | PASS | 11 phase deps, 10 required, 5 scenarios, sid=d2rlnch-1778263525 |
| 13 | `lib/loader/pe_d2r_d3d11_renderer_init.hexa` | PASS | steps=8, resources=25, pso=3, elevated=1, +3 PARTIAL renderer markers (device_init / swapchain_create / first_frame_submit_simulated), emits=9 |
| 14 | `lib/loader/pe_d2r_mpq_archive_load.hexa` | PASS | archives=7, files=10340, size_mb=1425, patch_d2=1, d2data=1, +1 PASS archive marker (MPQ), emits=10 |
| 15 | `lib/loader/pe_d2r_real_launch_phase.hexa` | PASS | steps=8, elevated=1, ipc=10, bnet_launch=1, d2r_exe_spawn=1, +3 PARTIAL launch markers (pe_parse / iat_resolve / maincrt_reach), emits=9 |
| 16 | `lib/loader/pe_d2r_savegame_d2s_format.hexa` | PASS | ops=6, total_size=440, cloud=1, decrypt=1, stash_shared=3, +1 PASS archive marker (d2s), emits=9 |
| 17 | `lib/loader/pe_d2r_specific_track.hexa` | PASS | steps=7, loaded=4, elevated=1, first_frame=1, d2_dll_load=1, +3 PARTIAL track markers (bnet_marker / exception_handler / version_stamp), emits=8 |

### §1.3 D2R-related tests (3 of 3 PASS)

| # | Test | Verdict | Summary |
|---|---|---|---|
| 18 | `tests/test_d2r_archive_round_trip.hexa` | PASS | chain status: ready, sid=track-a-1778263581, emits=4 |
| 19 | `tests/test_d2r_d3d11_synthetic_scaffold.hexa` | PASS | chain status: ready, sid=track-g-1778263623, emits=5 |
| 20 | `tests/test_d2r_pe_synthetic_round_trip.hexa` | PASS | chain status: ready, sid=track-d-1778263648, emits=4 |

**Aggregate**: 20 of 20 PASS across the joined battle.net + D2R cluster
(11 + 6 + 3). Distinct marker prefixes preserved across the 20 modules:

- `__BNET_IPC__ PARTIAL <verb>` × 4 (cond.3, IPC envelope; module 2)
- `__BNET_HTTP_REST__ PARTIAL <verb>` × 4 (cond.3, HTTP REST envelope; module 6)
- `__BNET_AGENT__ PASS <verb>` × N (cond.1, sd_forging DACL; module 7 — distinct from PARTIAL blocks)
- `__BNET_SETUP__ PASS <verb>` × N (setup_install_path_real; module 9)
- `__D2R_RENDERER__ PARTIAL <step>` × 3 (D3D11 init / swapchain / first frame; module 13)
- `__D2R_ARCHIVE__ PASS <kind>` × 2 (MPQ / d2s; modules 14 & 16)
- `__D2R_LAUNCH__ PARTIAL <step>` × 3 (PE parse / IAT resolve / mainCRT reach; module 15)
- `__D2R_TRACK__ PARTIAL <stamp>` × 3 (bnet marker / exception handler / version stamp; module 17)

All 20 invocations honest-emit on completion (own2). No silent_swallow.

---

## §2 closure_progress marker emit (post-DE state)

```
$ HEXA_SHIM_NO_DARWIN_LANDING=1 hexa run tool/closure_orchestrator.hexa emit
__CLOSURE__ PASS emit marker=state/markers/closure_progress_1778263683.marker closure_pct_weighted=40.00% closure_pct_binary=40% weighted_contrib=15.00%
__CLOSURE__ PASS emit marker=state/markers/closure_progress_1778263683.marker closure_pct=40 cm_ratio=18/30 checkpoint_reached=3/7
__CLOSURE__ PASS emit marker=state/markers/closure_progress_1778263683.marker pct=25 cm=18/30
```

Marker emitted: `state/markers/closure_progress_1778263683.marker`
(gitignored runtime telemetry; snapshot inlined below).

| field | value (post-DE / DF emit) | value (Track DD snapshot 1778046888) |
|---|---|---|
| closure_pct (raw cm-ratio) | 25 | 25 |
| closure_pct_weighted_threshold_aware | 40.00 | 40.00 |
| cm_done_index / cm_total | 18 / 30 | 18 / 30 |
| skeleton_manjeom | 100 | 100 |
| validated_manjeom | 0 | 0 |
| checkpoint_count / checkpoint_reached | 7 / 3 | 7 / 3 |

**Closure percentage carries unchanged from Track DD.** Reasons (as in
Track DD §3) — orchestrator is a tracker, cm-ratio is roadmap-derived
not module-derived, and the 7 gameplay-tier checkpoints don't flip on
skeleton-tier module landings. The skeleton-tier work is invisible to
`closure_pct` by design. Real `closure_pct` advancement requires CQ-P5
per-surface validated-tier flips.

---

## §3 What DF asserts and does not assert

### §3.1 What is asserted

- All 11 battle.net cluster modules + all 6 D2R cluster modules + all 3
  D2R-related test harnesses `self_test` return PASS under the
  fastest-arriving real-run override path.
- Track DD's 11-module battle.net cluster real-run reproducibility is
  preserved post-Track DE (which landed a 3-conflict fix in
  `pe_win32_shell32.hexa` plus a bulk lib/loader audit). No regression
  from DE in the battle.net cluster.
- The 6 D2R loader modules now have their own joined-batch real-run
  observation on record (DD covered battle.net only; DA/CG/CJ touched
  individual D2R surfaces but no prior track ran the 6-module D2R cluster
  as a single sweep).
- The 3 D2R-related test harnesses (archive_round_trip / d3d11
  synthetic scaffold / pe synthetic round trip) are real-run executable
  under the same override pattern. This extends the validated-shape
  surface from loader modules to test modules.
- The `closure_orchestrator emit` pipeline is still functional post-DE
  (no orchestrator regression). Marker schema fields all populate per
  the `airgenome-gamebox/markers/closure_progress/1` schema.

### §3.2 What is NOT asserted

- **No real network traffic** — battle.net modules 8 & 11 remain
  instrumentation sinks, not real client bindings.
- **No real game frame** — `validated_manjeom = 0` carries; the 7
  checkpoints' `d2r_battlenet_*_play` partial status carries.
- **No D2R install + launch sequence** — d2r_launch / pe_d2r_real_launch_phase
  exercise scenario / step graphs only, not an actual `D2R.exe` spawn.
  CQ-P5 + Phase 9-onwards real-bytes work is gated separately.
- **No D4** — DF does not touch D4 surfaces. D4 game_kind in the TACT
  module's enum is unexercised here (CW-Q4 default applies).
- **No `closure_pct` advancement** — same architectural constraint as
  Track DD §3. The fact that `cm_ratio = 18/30` carries unchanged is the
  expected outcome, not a regression.

### §3.3 Honest sub-tier promotion

Per Track CQ §3 Phase CQ-P4 exit criteria, the sub-tier of "paper-mode
prediction → real-execution-confirmed skeleton-tier" is preserved for
the entire battle.net cluster (DD landing) and is now also held by the
6 D2R loader cluster modules + 3 D2R test harnesses (DF landing). The
top-tier verdict `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`
carries unchanged.

---

## §4 What DF delivers

| File | Op | Purpose |
|---|---|---|
| `docs/BATTLENET_D2R_REAL_HEXA_RUN_TRACK_DF.ai.md` | new | This Track DF doc. |
| `state/markers/closure_progress_1778263683.marker` | runtime emit (gitignored) | Fresh post-DE/DF closure marker (snapshot inlined in §2). |

No code changes. DF is a real-run sweep + documentation track only —
existing modules executed under the established override path. Per
own1 + raw#168 minimum-viable, no module rewrites, no orchestrator
patches, no roadmap edits.

---

## §5 carries (UNCHANGED from Track DE)

- `validated_manjeom = 0` carries (no real game frame).
- `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` top-tier verdict
  carries.
- `closure_pct_weighted = 40.00%` / `closure_pct_binary = 40%` carries.
- `cm_ratio = 18/30` carries.
- `checkpoint_reached = 3/7` carries.
- own1 invariants carry — Wine 0, DRM bypass 0, Warden bypass 0,
  anti-cheat bypass 0, anima clone 0.
- own2 invariants carry — every module honest-emits PASS/FAIL on completion;
  silent_swallow 0; PARTIAL emits explicit; RETIRED_INTENTIONAL emits
  explicit (where applicable).

---

## §6 user-directive trace

| step | event | verbatim user input (or system response) |
|---|---|---|
| 0 | trigger | "battle.net + d2r 가장 빠르게 도착할 흐름 kick (고갈시까지) 후 진행" |
| 1 | identify fastest path | (system) Track DA/DD/DE established `HEXA_SHIM_NO_DARWIN_LANDING=1 hexa run <module> self-test` as fastest single-shot real-run path |
| 2 | sweep battle.net 11 | (system) 11 of 11 PASS |
| 3 | sweep d2r 6 | (system) 6 of 6 PASS |
| 4 | sweep d2r tests 3 | (system) 3 of 3 PASS — exhaustion observed at 20 modules |
| 5 | emit closure marker | (system) `__CLOSURE__ PASS emit ... pct=25 cm=18/30` |
| 6 | document + commit | (this Track DF doc + commit landing) |

The literal Korean phrase "고갈시까지" ("until exhaustion") is
interpreted here as "until no further joined-cluster battle.net + D2R
loader / test module remains unswept under the fastest-arriving
single-shot real-run path." Exhaustion observed at module 20 of 20.
The follow-on D2R surface tracks (CG / CJ / archive round-trip
synthetic) are already ALL-PASS at the test-harness level (see §1.3 +
prior CG/CJ landings); no further fastest-path real-run target remained
within the joined battle.net + D2R scope.

---

## §7 next steps (post-DF, gated, NOT done in DF)

- **CQ-P5 per-surface validated-tier flips** (NSWindow / Agent HTTP 200
  / CDN chunk / OAuth bearer / D2R first frame) — requires real
  Battle.net Setup binary + real network + real D2R install.
- **D4 surface re-engagement** — currently retired_intentional per Track
  P. Re-engagement gated on D4-stack G-2/G-3/G-4 being unstuck.
- **CQ-P6+ orchestration** (battle.net + D2R + D4 cross-cluster gameplay
  trace) — requires CQ-P5 first.

DF does not advance any of the above. DF is the joined real-run
exhaustion sweep + documentation track for the existing skeleton-tier
joined battle.net + D2R cluster.

---

## §8 references

- Track DD: `docs/CQ_P4_FULL_CLOSURE_REAL_HEXA_RUN_TRACK_DD.ai.md` (battle.net 11-module full closure)
- Track DA: `docs/CQ_P4_PARTIAL_CLOSURE_REAL_HEXA_RUN_TRACK_DA.ai.md` (battle.net 9-module partial closure + scope rename)
- Track DE: `docs/FULL_REPO_REAL_EXECUTION_AUDIT_TRACK_DE.ai.md` (full-repo audit 458/458)
- Track CQ: `docs/BATTLENET_ROADMAP_TRACK_CQ.ai.md` (parent battle.net roadmap)
- D2R install/launch roadmap: `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` (referenced from closure marker `ssot_d2r_d4_roadmap`)
- Closure roadmap SSOT: `docs/CLOSURE_ROADMAP.md` (referenced from closure marker `ssot_closure_roadmap`)

---

*written 2026-05-09 (Track DF). Joined battle.net + D2R fastest-arriving
real-run sweep, exhaustion at 20 modules, closure_pct carries 25 / 40
weighted, validated_manjeom = 0 carries, top-tier verdict
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 carries.*
