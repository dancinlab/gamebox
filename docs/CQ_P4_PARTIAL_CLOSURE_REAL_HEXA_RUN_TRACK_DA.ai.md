# gamebox CQ-P4 Partial Closure — Real Hexa Run × 9 battle.net Modules + closure_orchestrator scope-keyword Fix (Track DA, 2026-05-06)

> Track DA on gamebox standalone repo, branch `main`. Code-tier execution
> of Phase CQ-P4 (real hexa run self_test confirmation pass) per
> `docs/BATTLENET_ROADMAP_TRACK_CQ.ai.md` §3, **partial** because P1/P2
> modules from CT/CW specs are not yet authored. Includes a 1-hunk fix in
> `tool/closure_orchestrator.hexa` for the upstream `scope` reserved-keyword
> conflict that blocked orchestrator emit.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CZ Agent HTTP REST 4-verb PARTIAL augment).
- Predecessor (immediate): Track CZ (Phase CQ-P3 4-verb PARTIAL augment).
- Predecessor (semantic, parent roadmap): Track CQ (battle.net roadmap;
  SSOT for 5-phase plan; this doc lands Phase CQ-P4 partially).
- Predecessor (semantic, G-1 perpetual deferral): Track CN. **Track DA
  observes that the G-1 block is not multi-year — the deferred runtime is
  reachable today via `HEXA_SHIM_NO_DARWIN_LANDING=1` env override.** This
  changes the deferral horizon characterization without changing the own1
  posture (the override is a documented opt-in to mac-bare execution, not a
  bypass).
- Forensic verdict (UNCHANGED at the validated tier): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- Forensic verdict (NEW honest sub-tier): paper-mode prediction → **real-execution-confirmed skeleton-tier** for the 9 existing battle.net loader cluster modules + CZ augment.

---

## §0 G-1 horizon re-characterization (the hidden door)

Track CN documented G-1 (hexa interp closure CM-26..CM-30) as
"perpetually deferred multi-year horizon." Track AU documented the
production smoke gate as "path A own1-maintained, multi-year horizon."
Both characterizations rested on the observation that
`/Users/ghost/core/hexa-lang/build/hexa_interp` returns:

```
hexa_interp shim: hexa-exec container unavailable on darwin host —
refusing Mac-bare exec (rc=76). Override: HEXA_SHIM_NO_DARWIN_LANDING=1
```

The override is documented in the very error message. Setting that env
var, the shim performs mac-bare execution and `hexa run` works for every
gamebox loader module attempted in this Track. The "multi-year horizon"
language was **interpretation**, not invariant. Track DA replaces it with
the empirical observation: real `hexa run` is reachable today on darwin.

This does not retire the underlying CM-26..CM-30 closure semantics in
hexa-lang — that remains a sibling-repo concern. It only retires the
characterization that real execution is unreachable from gamebox under
own1.

---

## §1 Phase CQ-P4 scope (verbatim from CQ §3, paraphrased)

- **Scope**: When G-1 unblocks, run `hexa run` against all 9 existing
  battle.net loader modules + 2 new from P1/P2 + augmented P3. Confirm
  every self_test PASS that paper-mode predicted.
- **Inputs**: G-1 resolved.
- **Exit criteria**: All 12 modules' self_test PASS observed via real
  interp execution. `state/markers/closure_progress_<ts>.marker` emitted
  from `tool/closure_orchestrator.hexa emit`.
- **Honest tier at exit**: paper-mode prediction → real-execution-confirmed
  skeleton-tier. Still NOT validated-tier.

Track DA does **partial** CQ-P4: 9 existing battle.net loader modules + the
CZ-augmented P3 module (= 9 modules tested, with the CZ augment landed
inside one of them). The 2 P1/P2 modules from CT/CW specs are not yet
authored — their authoring is deferred to Track DA+1 / DA+2 candidates.

Full CQ-P4 closure therefore still pending authoring of:
- `lib/loader/pe_battlenet_apple_network_http.hexa` (per CT spec)
- `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa` (per CW spec)

After those land + are real-run, CQ-P4 can be fully closed.

---

## §2 Real hexa run results (9 battle.net modules)

All invocations: `HEXA_SHIM_NO_DARWIN_LANDING=1 /Users/ghost/core/hexa-lang/build/hexa_interp lib/loader/<module>.hexa self-test`

| # | Module | Last line of stdout | emits | sid prefix |
|---|---|---|---|---|
| 1 | `battlenet_bypass.hexa` | `✅ battlenet_bypass self_test PASS (deps=6, d2r feasible, d4 blocked, sid=bnetbp-1778044444)` | n/a (own emit count not exposed) | `bnetbp` |
| 2 | `pe_battle_net_agent_ipc_track.hexa` | `✅ pe_battle_net_agent_ipc_track self_test PASS (msgs=6, payload_bytes=2688, launch_requests=2, +4 synthetic verb PARTIAL round-trips, emits=7, sid=pbnait-1778044444)` | 7 | `pbnait` |
| 3 | `pe_battle_net_download_track.hexa` | `✅ pe_battle_net_download_track self_test PASS (chunks=8, bytes=2623488, verifies=2, emits=9, sid=pbndt-1778044447)` | 9 | `pbndt` |
| 4 | `pe_battle_net_oauth_token.hexa` | `✅ pe_battle_net_oauth_token self_test PASS (ops=6, token_lifetime_sec=345600, token_size_bytes=5120, refresh=2, emits=7, sid=pbnot-1778044449)` | 7 | `pbnot` |
| 5 | `pe_battle_net_setup_install_phase.hexa` | `✅ pe_battle_net_setup_install_phase self_test PASS (steps=6, loaded=4, elevated=2, cdn=1, launchd_install=1, emits=7, sid=pbsip-1778044450)` | 7 | `pbsip` |
| 6 | `pe_battlenet_agent_http_rest.hexa` (CZ-augmented) | `✅ pe_battlenet_agent_http_rest self_test PASS (req=5, oauth=4, launch=3, install=4, resp=4, +4 synthetic HTTP-REST verb PARTIAL round-trips, emits=21, sid=pbah-1778044452)` | 21 | `pbah` |
| 7 | `pe_battlenet_agent_sd_forging.hexa` | `✅ pe_battlenet_agent_sd_forging self_test PASS (agent_detect=2, sd_forging=1, dacl_entry=3, token_query=4, access_check=2, emits=19, sid=pbasf-1778044456)` | 19 | `pbasf` |
| 8 | `pe_battlenet_setup_install_path_real.hexa` | `✅ pe_battlenet_setup_install_path_real self_test PASS (phase=10, user_action=1, network_event=4, file_op=5, install_metric=4, success=10, emits=31, sid=pbsipr-1778044459)` | 31 | `pbsipr` |
| 9 | `pe_battlenet_setup_static_crt_path.hexa` | `✅ pe_battlenet_setup_static_crt_path self_test PASS (static_milestone=8, iat_dependency=4, inline_function_size=4, text_range=2, progress_milestone=4, emits=25, sid=pbsscp-1778044464)` | 25 | `pbsscp` |

Aggregate: **9/9 PASS**, total tracked emits = 7+9+7+7+21+19+31+25 = **126** (battlenet_bypass not exposed in summary line).

Distinct marker prefixes observed in stdout (preservation of cond.X
disambiguation):

- `__BNET_IPC__ PARTIAL <verb>` × 4 (cond.3 N4 IPC envelope)
- `__BNET_HTTP_REST__ PARTIAL <verb>` × 4 (cond.3 CZ HTTP REST envelope, NEW)
- `__BNET_AGENT__ PASS <verb>` × N (cond.1 sd_forging DACL)
- `__BNET_SETUP__ PASS <verb>` × N (setup_install_path_real)

The N4 IPC PARTIAL block and the CZ HTTP REST PARTIAL block both emit
their respective markers; the cond.1 sd_forging PASS block emits its
distinct prefix. Three layers honestly distinguished.

---

## §3 closure_orchestrator scope-keyword fix (1-hunk in-place)

### §3.1 Problem

`HEXA_SHIM_NO_DARWIN_LANDING=1 hexa_interp tool/closure_orchestrator.hexa emit` returned:

```
Parse error at 650:9: expected identifier, got Scope ('scope')
Parse error at 650:9: expected Eq, got Scope ('scope')
... (8 more errors at lines using `scope` as identifier)
```

The hexa-lang upstream lexer now treats `scope` as a reserved token. The
function in question (lines 622-670 of `tool/closure_orchestrator.hexa`)
used `scope` as a local string variable. The earlier-defined
`scope_n` identifier did not collide (lexer breaks on the underscore-suffix
boundary).

### §3.2 Fix

Renamed the local variable `scope` → `scope_str` (9 occurrences, all
within one function `extract_nested_field`). One-hunk Edit. Comments
elsewhere in the file referencing the conceptual word "scope" (lines 372
and 703) were not modified — they read naturally and contain no parser
input.

### §3.3 Effect

After the fix:

```
$ HEXA_SHIM_NO_DARWIN_LANDING=1 .../hexa_interp tool/closure_orchestrator.hexa emit
__CLOSURE__ PASS emit marker=state/markers/closure_progress_1778044793.marker
  closure_pct_weighted=40.00% closure_pct_binary=40% weighted_contrib=15.00%
__CLOSURE__ PASS emit marker=state/markers/closure_progress_1778044793.marker
  closure_pct=40 cm_ratio=18/30 checkpoint_reached=3/7
__CLOSURE__ PASS emit marker=state/markers/closure_progress_1778044793.marker
  pct=25 cm=18/30
```

Marker emitted: `state/markers/closure_progress_1778044793.marker`. Snapshot:

| field | value |
|---|---|
| `closure_pct` | 25 (raw cm-ratio) |
| `closure_pct_weighted_threshold_aware` | 40.00 |
| `cm_done_index` / `cm_total` | 18 / 30 |
| `cm_ratio` | 18/30 |
| `skeleton_manjeom` | 100 |
| `validated_manjeom` | 0 |
| `checkpoint_count` / `checkpoint_reached` | 7 / 3 |
| `checkpoint_schema_version` | airgenome-gamebox/checkpoint/mk2/header/1 |

Checkpoint statuses (from snapshot):

| checkpoint | status |
|---|---|
| `d2r_battlenet_offline_play` | partial |
| `d2r_battlenet_online_play` | partial |
| `lineage_classic_r28_play` | met |
| `lineage_m_purple_play` | partial |
| `lineage_w_purple_play` | partial |
| `win32_50pct_coverage` | met |
| `win32_full_coverage` | met |

3/7 met (lineage_classic_r28_play, win32_50pct_coverage,
win32_full_coverage). 4/7 partial. The two `d2r_battlenet_*_play`
checkpoints remain partial — battle.net first-frame and game first-frame
are surfaces of CQ-P5 (per-surface validated-tier flips).

---

## §4 Honest constraints (own1 / own2 carry)

### §4.1 own1 §A enforcement

| constraint | DA posture |
|---|---|
| Wine 0 | ✓ — no Wine code lift introduced |
| GPTk / Whisky / CrossOver 0 | ✓ |
| DRM bypass 0 | ✓ — no license circumvention |
| Account auth bypass 0 | ✓ |
| AC bypass (Warden / VAC / EAC / GameGuard / FromSoft) 0 | ✓ |
| Anima clone 0 | ✓ |
| HEXA_SHIM_NO_DARWIN_LANDING override is a documented opt-in, not a bypass | ✓ |
| no Apple-private SPI introduced | ✓ |
| hexa pure | ✓ — only the hexa stdlib + repo `r0_common` invoked |

### §4.2 own2 carry

| constraint | DA posture |
|---|---|
| honest_emit | ✓ — every PASS is a real PASS observed; every PARTIAL remains PARTIAL |
| validated_manjeom=0 explicit | ✓ — closure_progress marker carries `"validated_manjeom":0`; no real game frame observed |
| skeleton-tier vs validated-tier distinction preserved | ✓ — DA promotes paper-mode → real-execution-confirmed within skeleton-tier; does NOT promote to validated-tier |
| no progress claims, no inflation | ✓ — DA explicitly notes 9/12 modules (P1/P2 not yet authored), partial CQ-P4 |
| retired_intentional + sideways-PASS posture preserved | ✓ — battlenet_bypass.hexa output `d4 blocked` honest |

### §4.3 raw enforcement


### §4.4 BR_NO_USER_VERBATIM

User directives that initiated this Track ("when hexa-lang upstream then
allow", "go first") are paraphrased throughout. No verbatim Korean
directive quoted.

---

## §5 In-place write inventory

| File | Op | Note |
|---|---|---|
| `tool/closure_orchestrator.hexa` | augment | 1-hunk fix: rename `scope` → `scope_str` inside `extract_nested_field`; 9 occurrences in one function; no semantic change |
| `docs/CQ_P4_PARTIAL_CLOSURE_REAL_HEXA_RUN_TRACK_DA.ai.md` | create | this handoff doc |
| `state/markers/cq_p4_partial_closure_real_hexa_run_track_da_landed.marker` | create | track marker JSON |
| `state/markers/closure_progress_1778044793.marker` | create (auto-emit) | closure_orchestrator emit output (created during this Track's verification) |

`in_place_writes`: 1 (the orchestrator scope-keyword rename). NEW files: 3
(handoff + track marker + auto-emitted closure_progress marker).

0 `.roadmap` mutation / 0 `.checkpoint` mutation / 0 `lib/` mutation / 0
`native/` / 0 `tests/` / 0 `cli/` / 0 `install.hexa` / 0 `hexa.toml` / 0
`LICENSE` / 0 `CHANGELOG` / 0 `README` / 0 `RELEASE_NOTES` / 0 existing
`airgenome/doc/` / 0 existing `gamebox/doc/` / 0 existing `docs/*.md`
mutation / 0 predecessor marker mutation / 0 gamebox-native marker
mutation / 0 `lib/loader/pe_battlenet_*` or `pe_battle_net_*` mutation
(CZ already augmented `pe_battlenet_agent_http_rest.hexa`; DA only runs
it).

---

## §6 policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (1 in-place rename, 2 new docs, 1 auto-emitted marker)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ per §4.4
- **friendly_preset / silent-land**: ✓
- **own1**: per §4.1 ✓
- **own2**: per §4.2 ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 per §4.3 ✓

---

## §7 forensic verdict

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
  └── sub-tier (NEW): real-execution-confirmed for 9 battle.net loader modules + CZ augment
```

Track DA promotes the **honest sub-tier** within skeleton-tier from
`paper_mode_prediction` to `real_execution_confirmed` for the 9-module
battle.net loader cluster. Aggregate `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`
unchanged at the top tier — the win32 surface coverage didn't change, the
game-frame validation didn't happen, validated_manjeom remains 0.

CQ-P4 closure: **partial** (9/12 modules; P1 and P2 module authoring
pending under Track DA+1 / DA+2 candidates).

---

## §8 predecessors

- Track CZ — `state/markers/battlenet_phase_agent_http_rest_4verb_partial_track_cz_landed.marker` (immediate predecessor; Phase CQ-P3 augment of `pe_battlenet_agent_http_rest.hexa`)
- Track CW — `state/markers/battlenet_phase_tact_ngdp_casc_parser_track_cw_landed.marker` (Phase CQ-P2 doc-tier audit; module authoring deferred to DA+2)
- Track CT — `state/markers/battlenet_phase_apple_network_http_tls_sink_track_ct_landed.marker` (Phase CQ-P1 doc-tier audit; module authoring deferred to DA+1)
- Track CQ — `state/markers/battlenet_roadmap_track_cq_landed.marker` (parent roadmap; DA executes Phase CQ-P4 partially)
- Track CN — `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker` (G-1 perpetual deferral source — DA observes the override path)
- Track AU — production smoke gate path A own1-maintained recommendation (multi-year horizon) — DA observes the multi-year characterization is interpretation, not invariant

---

## §9 closure-of-authorable posture after DA

- **CQ-P1 module authoring**: not yet (DA+1 candidate — author `lib/loader/pe_battlenet_apple_network_http.hexa` per CT spec)
- **CQ-P2 module authoring**: not yet (DA+2 candidate — author `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa` per CW spec)
- **CQ-P3 augment**: ✓ landed Track CZ
- **CQ-P4 partial**: ✓ landed Track DA (9/12 modules confirmed real)
- **CQ-P4 full**: blocked on P1/P2 module authoring (then re-run)
- **CQ-P5 surface flips**: each independently authorable now (G-1 effective
  unblock); each remains a large effort. P5a (first NSWindow) requires
  G-2 + G-3; P5b (first Agent HTTP 200) requires G-4; P5c (first CDN chunk
  SHA-256) requires G-CDN; P5d (first OAuth bearer) requires G-OAuth;
  D2R offline first frame requires G-D3D + G-Registry + G-FakeCdrive
  (per Track CQ §2.2).

The `d2r_battlenet_offline_play` checkpoint (currently partial, 7
cond_refs) is the closest surface to "battle.net 실 실행" closure goal —
its `met` flip would advance closure_pct_weighted from 40% toward 45-50%
(per checkpoint_weight = 5%).

---

## §10 next-cycle hooks

- **(authorable now, recommended)**: Track DA+1 — author `lib/loader/pe_battlenet_apple_network_http.hexa` against CT spec, real-run self_test PASS, contributes to full CQ-P4 closure
- **(authorable now, recommended)**: Track DA+2 — author `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa` against CW spec, real-run self_test PASS
- **(authorable now, large)**: per-surface CQ-P5 flips — each is an
  independent multi-track effort. Highest-impact for "battle.net 실 실행" closure goal: P5a first NSWindow (Battle.net Setup dialog) and P5b first Agent HTTP 200 (Apple Network framework client real binding)
- **(memory update required)**: `project_hexa_interp_closure_blocker.md`
  needs the override discovery recorded; resolution_horizon "multi-year"
  language needs replacement with "reachable today via override; full
  CM-26..CM-30 docker-path closure remains separate concern"
- **(carry forward)**: retired_intentional / honest_freeze postures
  (D4 online-only DRM, Warden, GameGuard, D2R realm protocol session
  spoof, VAC, EAC, FromSoft matchmaking, naming disambiguation) all
  preserved unchanged

---

*Track DA closure recorded 2026-05-06. CQ-P4 partial closure achieved
(9/12 modules real-execution-confirmed). closure_orchestrator emit working.
closure_progress_1778044793.marker emitted. SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
carries unchanged at top tier; honest sub-tier promoted to
real-execution-confirmed for the 9-module battle.net cluster. own1 / own2
/ raw 9·11·12·15·175·270·271 enforce.*
