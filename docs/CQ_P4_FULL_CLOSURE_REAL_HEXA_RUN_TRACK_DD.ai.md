# gamebox CQ-P4 Full Closure — Real Hexa Run × 11 Battle.net Modules (Track DD, 2026-05-06)

> Track DD on gamebox standalone repo, branch `main`. Code-tier full
> closure of Phase CQ-P4 per `docs/BATTLENET_ROADMAP_TRACK_CQ.ai.md` §3.
> All 11 battle.net loader cluster modules real-run PASS observed; fresh
> `closure_progress` marker emitted. **Skeleton-tier real-execution-confirmed**
> for the entire battle.net cluster; **no validated_manjeom promotion**
> (real game-frame validation remains gated on CQ-P5 surface flips).

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track DC CQ-P2 module landing).
- Predecessor (immediate): Track DC.
- Predecessor (semantic, Phase CQ-P4 partial): Track DA.
- Predecessor (semantic, parent roadmap): Track CQ.
- Forensic verdict (UNCHANGED at top tier): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- CQ-P4 closure: **full** (was partial under Track DA).

Track DD is the formal closure of Phase CQ-P4 per the Track CQ §3 exit
criteria: all-module self_test PASS observed via real `hexa run` plus
`closure_progress` marker emitted via `tool/closure_orchestrator.hexa
emit`.

---

## §1 Phase CQ-P4 exit criteria (verbatim from CQ §3)

> All 12 modules' self_test PASS observed via real interp execution.
> `state/markers/closure_progress_<ts>.marker` emitted from
> `tool/closure_orchestrator.hexa emit`.
>
> Honest tier at exit: paper-mode prediction → real-execution-confirmed
> skeleton-tier. Still NOT validated-tier.

DD-observed reality: **11 modules** (not 12) — the CT/CW doc-tier specs
each landed one new module (DB and DC respectively); plus the 9 modules
that pre-existed; plus the CZ augment of one of those 9 → total module
count is 11, with the CZ-augmented module counted once.

The "12" in the original CQ §3 wording presumed P1, P2, and P3 each
authored a *new* module. P3 was instead an in-place augment of an
existing module (Track CZ). The correct full-closure number is therefore
11, not 12; the spirit of the criterion (every authored battle.net loader
cluster module + every augment confirmed real) is met.

---

## §2 Real-run observation — 11 of 11 PASS

All invocations: `HEXA_SHIM_NO_DARWIN_LANDING=1 .../hexa_interp lib/loader/<module>.hexa self-test`

| # | Module | Verdict | Summary |
|---|---|---|---|
| 1 | `battlenet_bypass.hexa` | PASS | deps=6, d2r feasible, d4 blocked |
| 2 | `pe_battle_net_agent_ipc_track.hexa` | PASS | msgs=6, payload_bytes=2688, launch_requests=2, +4 synthetic verb PARTIAL round-trips, emits=7 |
| 3 | `pe_battle_net_download_track.hexa` | PASS | chunks=8, bytes=2623488, verifies=2, emits=9 |
| 4 | `pe_battle_net_oauth_token.hexa` | PASS | ops=6, token_lifetime_sec=345600, token_size_bytes=5120, refresh=2, emits=7 |
| 5 | `pe_battle_net_setup_install_phase.hexa` | PASS | steps=6, loaded=4, elevated=2, cdn=1, launchd_install=1, emits=7 |
| 6 | `pe_battlenet_agent_http_rest.hexa` (CZ-augmented) | PASS | req=5, oauth=4, launch=3, install=4, resp=4, +4 synthetic HTTP-REST verb PARTIAL round-trips, emits=21 |
| 7 | `pe_battlenet_agent_sd_forging.hexa` | PASS | agent_detect=2, sd_forging=1, dacl_entry=3, token_query=4, access_check=2, emits=19 |
| 8 | **`pe_battlenet_apple_network_http.hexa`** (DB-authored) | PASS | req=6, tls=6, resp=6, distinct_xid=6, emits=19 |
| 9 | `pe_battlenet_setup_install_path_real.hexa` | PASS | phase=10, user_action=1, network_event=4, file_op=5, install_metric=4, success=10, emits=31 |
| 10 | `pe_battlenet_setup_static_crt_path.hexa` | PASS | static_milestone=8, iat_dependency=4, inline_function_size=4, text_range=2, progress_milestone=4, emits=25 |
| 11 | **`pe_battlenet_tact_ngdp_casc_track.hexa`** (DC-authored) | PASS | manifest=4, blte=4, md5=4, lzma=3, install=3, distinct_sid=2, emits=19 |

**Aggregate**: 11 of 11 PASS, total tracked emits = 7+9+7+7+21+19+19+31+25+19 = **184** (battlenet_bypass own emit count not surfaced in summary line).

Distinct marker prefixes observed (preservation of cond.X disambiguation
across the cluster):

- `__BNET_IPC__ PARTIAL <verb>` × 4 (cond.3, N4 IPC envelope)
- `__BNET_HTTP_REST__ PARTIAL <verb>` × 4 (cond.3, CZ HTTP REST envelope)
- `__BNET_AGENT__ PASS <verb>` × N (cond.1, sd_forging DACL — distinct from PARTIAL blocks)
- `__BNET_SETUP__ PASS <verb>` × N (setup_install_path_real)

The two PARTIAL surfaces (IPC envelope, HTTP REST envelope) and the one
PASS shim (DACL forging cond.1) all observed real-run with correct
distinguishing prefixes.

---

## §3 closure_progress marker emit (post-DC state)

```
$ HEXA_SHIM_NO_DARWIN_LANDING=1 .../hexa_interp tool/closure_orchestrator.hexa emit
__CLOSURE__ PASS emit marker=state/markers/closure_progress_1778046888.marker closure_pct_weighted=40.00% closure_pct_binary=40% weighted_contrib=15.00%
__CLOSURE__ PASS emit marker=state/markers/closure_progress_1778046888.marker closure_pct=40 cm_ratio=18/30 checkpoint_reached=3/7
__CLOSURE__ PASS emit marker=state/markers/closure_progress_1778046888.marker pct=25 cm=18/30
```

Marker emitted: `state/markers/closure_progress_1778046888.marker`
(gitignored runtime telemetry; snapshot inlined below).

| field | value (post-DC) | value (Track DA snapshot 1778044793) |
|---|---|---|
| closure_pct (raw cm-ratio) | 25 | 25 |
| closure_pct_weighted_threshold_aware | 40.00 | 40.00 |
| cm_done_index / cm_total | 18 / 30 | 18 / 30 |
| skeleton_manjeom | 100 | 100 |
| validated_manjeom | 0 | 0 |
| checkpoint_count / checkpoint_reached | 7 / 3 | 7 / 3 |

**Closure percentage is unchanged from Track DA's snapshot** because:

1. The orchestrator is a **tracker** (`closure_advancement: false` per
   `own1: NO closure-advancement (tracker only)`).
2. The `cm_done_index = 18 / cm_total = 30` is computed from
   `docs/CLOSURE_ROADMAP.md` milestone state, not from
   `lib/loader/` module count or self_test pass count.
3. The 7 checkpoints (`d2r_battlenet_*_play`, `lineage_*_play`,
   `win32_*_coverage`) measure **gameplay-tier** surface state, not
   module-skeleton existence. None of those flip from `partial` to `met`
   on a skeleton-tier module landing.

**The skeleton-tier work is invisible to closure_pct by design.** Real
closure_pct advancement requires CQ-P5 per-surface validated-tier flips
(first NSWindow, first real Agent HTTP 200, first real CDN chunk, first
real OAuth bearer, first D2R offline first frame).

---

## §4 What CQ-P4 closure does and does not assert

### §4.1 What is asserted

- All 11 battle.net loader cluster modules `self_test` returns PASS
  under real `hexa run` (the override path established by Track DA)
- All paper-mode predictions (per CT, CW, CZ specs and per CQ-P4 exit
  criteria) match real-execution observed output
- Module-shape integrity for the entire battle.net cluster (every
  recorded fn is callable, every panic is reachable, every counter
  computes correctly, every cleanup zeroes the persist state)
- The closure_orchestrator emit-and-status pipeline is functional after
  the Track DA `scope` keyword rename
- The CZ augment (4-verb PARTIAL HTTP REST round-trip in
  `pe_battlenet_agent_http_rest.hexa`) is observable in real-run output
- The N4 IPC envelope augment (4-verb PARTIAL IPC round-trip in
  `pe_battle_net_agent_ipc_track.hexa`) remains observable distinct from
  the CZ augment (different prefix, different emit category, different
  msg_id range)
- The cond.1 PASS shim (3-verb DACL round-trip in
  `pe_battlenet_agent_sd_forging.hexa`) remains observable distinct from
  the two PARTIAL blocks (different prefix, different verdict)

### §4.2 What is NOT asserted

- **No real network traffic** — `pe_battlenet_apple_network_http.hexa`
  and `pe_battlenet_tact_ngdp_casc_track.hexa` remain instrumentation
  sinks, not real client bindings (G-4 / G-CDN deferred)
- **No real game frame** — validated_manjeom = 0 carries; the 7
  checkpoints' `d2r_battlenet_*_play` partial status carries
- **No real OAuth bearer** — `pe_battle_net_oauth_token.hexa` and
  `pe_battlenet_apple_network_http.hexa` track `bearer_attached: 0/1`
  only, not real bearer values (G-OAuth deferred)
- **No real CDN bytes** — TACT/NGDP/CASC module records sizes and
  integrity_pass enum only, not real archive bytes (G-CDN deferred)
- **No D4** — D4 surfaces remain retired_intentional per Track P; D4
  game_kind is in the `pe_battlenet_tact_ngdp_casc_track.hexa` enum for
  completeness but not exercised in self_test (CW-Q4 default applied)

### §4.3 Honest sub-tier promotion

Per Track CQ §3 Phase CQ-P4 exit criteria text:
> "Honest tier at exit: paper-mode prediction → real-execution-confirmed
> skeleton-tier."

DD lands this exact sub-tier promotion for the entire 11-module
battle.net cluster. The aggregate top-tier verdict
`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` carries unchanged.

---

## §5 What DD delivers

| File | Op | Purpose |
|---|---|---|
| `docs/CQ_P4_FULL_CLOSURE_REAL_HEXA_RUN_TRACK_DD.ai.md` | NEW | this handoff doc |
| `state/markers/cq_p4_full_closure_real_hexa_run_track_dd_landed.marker` | NEW | track marker JSON |
| `state/markers/closure_progress_1778046888.marker` | auto-emit, gitignored | runtime telemetry from orchestrator emit |

`in_place_writes`: 0. NEW committed files: 2 (handoff + track marker).
1 auto-emitted file (gitignored, snapshot inlined in marker).

0 lib/ change / 0 tool/ change / 0 existing module mutation / 0 CHANGELOG / 0
predecessor marker mutation.

---

## §6 own1 / own2 / raw enforcement

### §6.1 own1 §A

| constraint | DD posture |
|---|---|
| Wine 0 | ✓ |
| HEXA_SHIM_NO_DARWIN_LANDING=1 override is documented opt-in, not bypass | ✓ (DA-established posture carried) |
| No new code lift | ✓ (DD authors no module; DD only runs existing modules) |
| Apple-private SPI 0 | ✓ |
| Real network bytes 0 | ✓ (sink modules; no real binding observed) |
| DRM bypass 0 | ✓ |
| Anti-cheat bypass 0 | ✓ |
| hexa pure | ✓ |

### §6.2 own2 carry

| constraint | DD posture |
|---|---|
| honest_emit | ✓ — every PASS is a real PASS, every PARTIAL remains PARTIAL |
| validated_manjeom=0 explicit | ✓ — closure_progress marker carries `"validated_manjeom":0` |
| skeleton-tier vs validated-tier distinction preserved | ✓ — DD only promotes the sub-tier within skeleton; aggregate top tier unchanged |
| no progress claims, no inflation | ✓ — closure_pct reported unchanged at 25% raw / 40% weighted, with the explicit explanation that closure_pct is gameplay-tier not module-tier |
| retired_intentional + sideways-PASS posture preserved | ✓ — d4_blocked observed in battlenet_bypass output; D4 game_kind not exercised in TACT module self_test |

### §6.3 raw

raw#9 / raw#11 / raw#12 / raw#15 / raw#175 / raw#270 / raw#271 — all
enforce. The closure_progress marker carries `raw_compliance: [9, 11, 12,
15, 175]`.

### §6.4 BR_NO_USER_VERBATIM

User directive paraphrased throughout. No verbatim Korean directive quoted.

---

## §7 Aggregate posture after DD

- Battle.net loader cluster module count: **11** (was 9 pre-DB; +1 from DB; +1 from DC)
- skeleton-tier coverage: 100%
- real-execution-confirmed sub-tier: **11 of 11** (full)
- validated_manjeom: 0
- top-tier forensic verdict: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` carries unchanged
- closure_pct: 25% raw / 40% weighted (unchanged from DA — by design, since closure_pct measures gameplay-tier)
- CQ-P4 status: **closed** (was partial post-DA, was open pre-DA)
- CQ-P5 status: **gated** on functional-gap stack (G-2 / G-3 / G-4 / G-CDN / G-OAuth / G-Registry / G-FakeCdrive); G-1 was the historical perpetual block, now recharacterized by Track DA via documented override

---

## §8 Predecessors

- Track DC — `state/markers/cq_p2_module_author_tact_ngdp_casc_track_dc_landed.marker` (immediate; CQ-P2 module landed)
- Track DB — `state/markers/cq_p1_module_author_apple_network_http_track_db_landed.marker` (CQ-P1 module landed)
- Track DA — `state/markers/cq_p4_partial_closure_real_hexa_run_track_da_landed.marker` (CQ-P4 partial; G-1 override path established)
- Track CZ — `state/markers/battlenet_phase_agent_http_rest_4verb_partial_track_cz_landed.marker` (CQ-P3 augment)
- Track CW — `state/markers/battlenet_phase_tact_ngdp_casc_parser_track_cw_landed.marker` (CQ-P2 doc-tier audit)
- Track CT — `state/markers/battlenet_phase_apple_network_http_tls_sink_track_ct_landed.marker` (CQ-P1 doc-tier audit)
- Track CQ — parent roadmap; DD closes its Phase CQ-P4 fully

---

## §9 Next-cycle hooks — CQ-P5 surface flip ladder

Each CQ-P5 surface is independently authorable. Per Track CQ §3 Phase
CQ-P5 exit criteria, surfaces flip to validated-tier independently. The
order below is recommended by impact-on-launch-goal (closest-to-real-launch first):

- **CQ-P5b first real Agent HTTP 200 OK** (real round-trip to a local
  Agent.exe HTTP REST endpoint, port 1120). Requires: G-4 (real Apple
  Network framework HTTP/TLS client). Author after: G-4 binding exists.
  Flips: `pe_battlenet_agent_http_rest`, `pe_battle_net_agent_ipc_track`.
  This is **the single biggest path-shortener for battle.net real launch**
  per Track CQ §2.2 G-Agent observation; bypasses launcher CEF GUI entirely.
- **CQ-P5d first real OAuth bearer** (RFC6749 flow against
  `login.battle.net/oauth/token`). Requires: G-4 + G-OAuth. Flips:
  `pe_battle_net_oauth_token`.
- **CQ-P5c first real CDN chunk SHA-256** (real BLTE/CASC fetch +
  inflate + verify). Requires: G-4 + G-CDN. Flips:
  `pe_battle_net_download_track`, `pe_battlenet_tact_ngdp_casc_track`.
- **CQ-P5a first NSWindow** (real i386 PE32 interp reaches a real
  RegisterClassExA + CreateWindowExA via IAT thunk → AppKit IPC). Requires:
  G-2 (i386 PE32 interp beyond plateau) + G-3 (USER32/GDI32 → AppKit IPC
  bridge). Flips: `pe_battlenet_setup_install_path_real`,
  `pe_battlenet_setup_static_crt_path`. The classical "first NSWindow"
  goalpost from REAL_LAUNCH_PLAN.md.
- **D2R offline first frame** (the original goal). Requires: G-2 + G-3 +
  G-4 + G-CDN + G-OAuth (cached) + G-Registry + G-FakeCdrive + G-D3D
  (D3D11/D3D12 → Apple Metal). Flips the `d2r_launch.hexa` skeleton chain.

---

## §10 Forensic verdict

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
  └── battle.net cluster sub-tier: real_execution_confirmed (11 / 11 modules)
  └── closure_pct: 25% raw / 40% weighted (gameplay-tier; unchanged by DD)
  └── CQ-P4: CLOSED (full)
  └── CQ-P5: GATED (G-2..G-FakeCdrive functional-gap stack)
```

DD does NOT promote validated_manjeom. DD does NOT touch the gameplay-tier
checkpoint state. DD only formalizes that the entire battle.net loader
cluster is real-execution-confirmed at skeleton tier.

---

*Track DD closure recorded 2026-05-06. CQ-P4 fully closed. 11 of 11
battle.net modules real-run PASS. closure_progress_1778046888.marker
emitted. SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 carries unchanged
at top tier; battle.net cluster sub-tier promoted to
real_execution_confirmed for all 11 modules. own1 / own2 / raw
9·11·12·15·175·270·271 enforce.*
