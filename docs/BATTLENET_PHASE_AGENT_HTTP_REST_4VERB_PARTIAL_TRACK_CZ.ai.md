# gamebox Battle.net Phase CQ-P3 — Agent HTTP REST 4-Verb PARTIAL Augment (Track CZ, 2026-05-06)

> Track CZ on gamebox standalone repo, branch `main`. Code-tier execution of
> Phase CQ-P3 (Agent HTTP REST direct-talk skeleton augment) per
> `docs/BATTLENET_ROADMAP_TRACK_CQ.ai.md` §3. In-place augment of one existing
> module: `lib/loader/pe_battlenet_agent_http_rest.hexa`. **No
> validated_manjeom promotion.** PARTIAL emit explicit, NOT PASS.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CY D4 D3D12-to-Metal router skeleton inventory).
- Predecessor (immediate): Track CY (D4 D3D12-to-Metal router skeleton inventory).
- Predecessor (semantic, parent roadmap): Track CQ (battle.net roadmap;
  SSOT for the 5-phase plan; this doc lands Phase CQ-P3 of).
- Predecessor (semantic, sibling Phase CQ-P1 audit): Track CT.
- Predecessor (semantic, sibling Phase CQ-P2 audit): Track CW.
- Predecessor (semantic, IPC envelope sibling 4-verb PARTIAL pattern source): Track-N4 inside `pe_battle_net_agent_ipc_track.hexa`.
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- Hexa interp closure CM-26..CM-30 still **perpetually deferred** (Track CN).
  Paper-mode derivation only; no real `hexa run` of any module asserted in CZ.

Track CZ executes Phase CQ-P3 directly at code-tier (in-place augment) rather
than via a separate doc-tier audit. Phase CQ-P3 is small enough — 4 verb
synthetic round-trip rows, no new functions, no new tables, no new persist
paths — that authoring against the existing N4 IPC pattern is cheaper than
producing a separate audit doc first. The augment mirrors the N4 IPC pattern
exactly; the N4 doc-tier guidance is already locked in (Track-N4 lockin
2026-05-03).

---

## §0 Why this Track now

Phase CQ-P3 is the last of the three independently-authorable phases (CQ-P1,
CQ-P2, CQ-P3) in the Track CQ roadmap. Phases CQ-P4 and CQ-P5 are gated on
G-1 (hexa interp closure CM-26..CM-30, perpetually deferred per Track CN) and
on G-2..G-4 / G-CDN / G-OAuth / G-Registry / G-FakeCdrive (each independently
unblocked). After CZ:

- CQ-P1 ✓ landed (Track CT)
- CQ-P2 ✓ landed (Track CW)
- CQ-P3 ✓ landed (Track CZ — this doc)
- CQ-P4: blocked on G-1 (perpetual deferral)
- CQ-P5: blocked on G-1 + G-2 + G-3 + G-4 + functional-gap stack

Closure of CQ-P3 therefore closes the **authorable-today skeleton-tier
remainder** for the battle.net plan. No further skeleton-tier work is
authorable until G-1 unblocks.

---

## §1 Phase CQ-P3 scope (verbatim from CQ §3, paraphrased)

- **Scope**: Augment existing `pe_battlenet_agent_http_rest.hexa` self_test
  with synthetic round-trip rows for the launcher-CEF-bypass mid-path
  (research-E core finding). 4-verb augment matching the existing N4
  cond.3 PARTIAL pattern: install / launch / uninstall / heartbeat — but
  **PARTIAL emit explicit**, NOT PASS, until G-4 (real Apple Network
  framework client) lands.
- **Inputs**: existing module + Track-N4 round-trip pattern.
- **Exit criteria**: self_test PARTIAL row count ≥4, distinct-from-cond_1_pass_shim flag set, schema validates.
- **Honest tier at exit**: **skeleton-tier with PARTIAL augmentation** (does
  NOT promote to validated; real Agent endpoint round-trip permanently
  behind production smoke gate).
- **Invariant impact**: own2 PARTIAL discipline explicit; validated_manjeom=0 carries.

Track CZ executes this scope exactly. No scope expansion, no scope reduction.

---

## §2 What CZ adds to the module

The augment is appended to `self_test()` in
`lib/loader/pe_battlenet_agent_http_rest.hexa` **after** the existing
cleanup + post-cleanup zero-stats assertion, and **before** the final
`println` PASS line. This placement matches the N4 IPC track pattern exactly
(synthetic emits are positioned after the assertion-bearing portion of
self_test, so the synthetic emit count never contributes to the existing
`emit_n >= 14` floor — preventing the augment from masking a regression in
the persist-tier emit count).

### §2.1 4-verb synthetic round-trip rows

Each verb emits one `println` line of the form `__BNET_HTTP_REST__ PARTIAL <verb>`
plus one `r0_emit("pe_battlenet_agent_http_rest_synthetic_round_trip", ...)` row
with this JSON shape:

| field | per-verb value |
|---|---|
| `stage` | `agent_http_rest_envelope_round_trip` |
| `http_session` | `http-rest-synth-<unix_epoch>` (one session for all 4 verbs) |
| `http_msg_id` | 910001 (install) / 910002 (launch) / 910003 (uninstall) / 910004 (heartbeat) |
| `verb_kind` | one of `install` / `launch` / `uninstall` / `heartbeat` |
| `endpoint_simulated` | `http://localhost:1120/agent/<verb>` (synthetic — no real network) |
| `http_method_simulated` | `POST` for install/launch/uninstall, `GET` for heartbeat |
| `response_code_simulated` | 200 for install/launch, 204 for uninstall/heartbeat |
| `transport_kind` | `synthetic_inline` |
| `cond` | `battlenet.cond.3` |
| `verdict` | `PARTIAL` |

The **install** row additionally carries `posture` and `production_smoke_gap`
fields documenting the gating chain:

- `posture`: `evidence_augment_partial_to_partial`
- `production_smoke_gap`: `permanent_until_g4_apple_network_real_client + g1_hexa_interp_perpetual_deferral`

### §2.2 Closing matrix row

A single closing emit
(`pe_battlenet_agent_http_rest_synthetic_round_trip_matrix`) records the
augment summary:

| field | value |
|---|---|
| `verbs_total` | 4 |
| `verbs_partial` | 4 |
| `verbs_pass` | 0 |
| `http_session` | `<same as per-verb rows>` |
| `transport_kind` | `synthetic_inline` |
| `network` | `none` |
| `cond` | `battlenet.cond.3` |
| `verdict` | `PARTIAL` |
| `track` | `CZ` |
| `phase` | `CQ-P3` |
| `lockin_iso` | `2026-05-06` |
| `distinct_from_cond_1_pass_shim` | `true` |
| `distinct_from_n4_ipc_envelope` | `true` |
| `validated_manjeom` | `0` |

Two distinctness flags:

- `distinct_from_cond_1_pass_shim`: prevents inflation against the
  `pe_battlenet_agent_sd_forging.hexa` cond.1 PASS shim (3-verb DACL
  round-trip), which IS PASS. The HTTP REST 4-verb augment is NOT a
  cond.1-class PASS — the production smoke gate is permanent.
- `distinct_from_n4_ipc_envelope`: prevents conflation with the sibling
  Track-N4 IPC envelope synthetic round-trip block inside
  `pe_battle_net_agent_ipc_track.hexa`. Both are PARTIAL, both are 4-verb,
  both target cond.3 — but they instrument **different surfaces**: N4 is
  the IPC envelope (named pipe / TCP loopback / websocket / https
  transport-layer), CZ is the HTTP REST envelope (REST endpoint + method +
  status code application-layer).

### §2.3 What CZ does NOT add

- No new persist tables (the existing 5 TSVs are untouched: request /
  oauth / launch / install / response).
- No new `record_*` functions.
- No new fn-level helpers.
- No mutation of the persist-tier emit count assertion (`emit_n >= 14`
  retained verbatim).
- No mutation of the 5 persist-tier post-record count assertions
  (`agent_request_count() != 5`, etc., retained verbatim).
- No promotion of `validated_manjeom`.
- No real network bytes, no real OAuth credentials, no real Bearer tokens
  beyond the existing test-only `abc123` placeholder string.

---

## §3 Honest constraints (own1 / own2 carry)

### §3.1 own1 §A enforcement

| constraint | CZ posture |
|---|---|
| Wine 0 | ✓ — no Wine wininet / cab / msvcrt code lift |
| GPTk / Whisky / CrossOver 0 | ✓ — no fork code lift |
| DRM bypass 0 | ✓ — no license circumvention |
| Account auth bypass 0 | ✓ — `abc123` is a placeholder string for self_test only, not a real bearer token |
| Warden bypass 0 | ✓ — anti-cheat surface untouched |
| VAC bypass 0 | ✓ — anti-cheat surface untouched |
| EAC kernel + anti-tamper bypass 0 | ✓ |
| GameGuard bypass 0 | ✓ |
| FromSoft matchmaking bypass 0 | ✓ |
| Endpoint spoof 0 | ✓ — `http://localhost:1120/agent/<verb>` is documented as synthetic, not a target for real interception |
| Port hijack 0 | ✓ — no socket bind, no listen, no connect |
| Apple-private SPI 0 | ✓ — no Apple SPI invoked |
| hexa pure | ✓ — only Hexa stdlib + repo `r0_common` use |

### §3.2 own2 carry

| constraint | CZ posture |
|---|---|
| honest_emit | ✓ — PARTIAL is emitted as `PARTIAL`, never `PASS` |
| validated_manjeom=0 explicit | ✓ — closing matrix row carries `"validated_manjeom":0` |
| skeleton-tier vs validated-tier distinction preserved | ✓ — augment-tier classification is `skeleton-tier with PARTIAL augmentation`; the module's overall posture remains skeleton-tier |
| perpetual deferral semantics carried for G-1 | ✓ — `production_smoke_gap` field cites `g1_hexa_interp_perpetual_deferral` explicitly |
| no progress claims, no inflation | ✓ — augment does not claim PASS, real round-trip, real network, or real launch |
| retired_intentional + sideways-PASS posture preserved | ✓ — D4 is not exercised in the synthetic verbs (D4 retired_intentional from Track P + Track CW); the verbs are game-agnostic at the URL path layer (no game UID encoded in the synthetic endpoint paths) |

### §3.3 raw enforcement

raw#9 (Wine 0) / raw#11 (honest emit) / raw#12 (validated_manjeom=0
carries) / raw#15 (env lazy resolve / repo-relative paths — no new env
references) / raw#175 (silent-land + minimum-viable additive) / raw#270
(skeleton-tier vs validated-tier distinction) / raw#271 (retired_intentional
+ sideways-PASS posture preserved). All seven enforce.

### §3.4 BR_NO_USER_VERBATIM

The user directive that initiated the cycle ("foreground continue until
remainder zero, no queries") is paraphrased in this doc and in the marker.
No verbatim Korean directive quoted.

---

## §4 In-place write inventory

| File | Op | Note |
|---|---|---|
| `lib/loader/pe_battlenet_agent_http_rest.hexa` | augment | `self_test()` only; new lines appended after cleanup + zero-stats assertion, before final `println` |
| `docs/BATTLENET_PHASE_AGENT_HTTP_REST_4VERB_PARTIAL_TRACK_CZ.ai.md` | create | this handoff doc |
| `state/markers/battlenet_phase_agent_http_rest_4verb_partial_track_cz_landed.marker` | create | track marker JSON |

`in_place_writes`: 1 (the .hexa file). NEW files: 2 (handoff + marker).

0 .roadmap mutation / 0 .checkpoint mutation / 0 other lib/ / 0 tool/ / 0
native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0
CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/ / 0
existing gamebox/doc/ / 0 existing docs/*.md mutation / 0 predecessor marker
mutation / 0 gamebox-native marker mutation / 0 other lib/loader/pe_battlenet_*
or pe_battle_net_* mutation.

---

## §5 Verification posture (paper-mode)

Per Track CN perpetual deferral, real `hexa run` is unavailable
(`hexa_interp shim: hexa-exec container unavailable on darwin host —
refusing Mac-bare exec rc=76`). Verification therefore is paper-mode:

- **shape conformance**: the augment block mirrors
  `pe_battle_net_agent_ipc_track.hexa` lines 393-433 (Track-N4 cond.3
  PARTIAL block) line-for-line in structural shape; the only differences
  are (a) prefix marker `__BNET_HTTP_REST__` vs `__BNET_IPC__`, (b) emit
  category names `pe_battlenet_agent_http_rest_synthetic_round_trip*` vs
  `pe_battle_net_agent_ipc_track_synthetic_round_trip*`, (c)
  `http_method_simulated` field present in CZ (absent from N4, since N4 is
  IPC envelope not HTTP), (d) `http_msg_id` range 910001-910004 vs N4
  `ipc_msg_id` range 900001-900004 (preserves disjoint ID ranges).
- **own1/own2 line audit**: every line of the augment block has been
  manually reviewed and compared against §3.1 / §3.2 row-for-row above.
- **distinctness flags**: `distinct_from_cond_1_pass_shim` and
  `distinct_from_n4_ipc_envelope` both set true in the closing matrix
  emit.
- **non-regression of persist-tier**: the augment is positioned after the
  existing assertion block (`emit_n >= 14`, all 5 count assertions, all 3
  name assertions, post-cleanup zero-stats assertion) — no existing
  assertion is mutated; if the assertion-bearing portion regressed under
  real `hexa run`, the panic would fire before any synthetic emit.

CZ does **not** assert or claim that the augment has been observed under
real `hexa run`. CZ asserts only that the augment is paper-mode
shape-conformant and own1/own2 line-audited.

---

## §6 policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (1 in-place augment, 2 new files; no
  deletion, no rename, no schema-migration)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (per §3.4)
- **friendly_preset / silent-land**: ✓
- **own1**: per §3.1 ✓
- **own2**: per §3.2 ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 ✓ per §3.3

---

## §7 forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

Track CZ is a skeleton-tier code-augment cycle. NO module count change (the
9 battle.net loader cluster module count is unchanged — CZ augments one
existing module). NO state mutation outside the in-place augment + 2 new
files. NO validated_manjeom promotion. NO hexa interp dependency
assumed-resolved. Real-tier validation of CQ-P3 remains gated on G-4 (real
Apple Network framework HTTP/TLS client) and on G-1 (hexa interp closure)
per Track CQ documented.

---

## §8 predecessors

- Track CY — `state/markers/d4_d3d12_to_metal_router_skeleton_inventory_track_cy_landed.marker` (immediate predecessor; D4 D3D12-to-Metal router skeleton inventory)
- Track CW — `state/markers/battlenet_phase_tact_ngdp_casc_parser_track_cw_landed.marker` (sibling phase doc-tier; Phase CQ-P2 audit)
- Track CT — `state/markers/battlenet_phase_apple_network_http_tls_sink_track_ct_landed.marker` (sibling phase doc-tier; Phase CQ-P1 audit)
- Track CQ — `state/markers/battlenet_roadmap_track_cq_landed.marker` (parent roadmap; Track CZ executes its Phase CQ-P3 at code-tier)
- Track CN — `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker` (G-1 perpetual deferral source; primary blocker for any real `hexa run` of the augmented module)
- Track P — `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` (permanent disambiguation reference; sister doc, never superseded)

---

## §9 closure-of-authorable-skeleton-remainder posture

CZ closes the last authorable-today skeleton-tier item in the Track CQ
roadmap. After CZ:

- **authorable-today skeleton-tier remainder**: 0
- **G-1-blocked items remaining**: CQ-P4 (real `hexa run` self_test
  confirmation × 12 modules) + CQ-P5 (per-surface validated-tier flips)
- **G-1 status**: perpetually deferred per Track CN; multi-year horizon

CZ does NOT pre-author CQ-P4 or CQ-P5 details — they materialize when their
gating events arrive. CZ does NOT assert any cycle-closure beyond the
specific augment landing.

The aggregate posture statement
`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` carries unchanged. The
"100PCT" coverage portion already accounted for the 9-module battle.net
cluster surface; CZ does not change cluster module count, and the existing
module's surface coverage (5 TSVs + cleanup + counts + name lookups) is
preserved verbatim.

---

## §10 next-cycle hooks

- **(authorable-today, optional)**: no further skeleton-tier battle.net
  authorable items remain after CZ. Adjacent surfaces (D2R per-game §9
  closure track, D4 D3D12-to-Metal router skeleton landing under Track
  CY+1) continue under their own per-game tracks.
- **(perpetually deferred)**: G-1 hexa interp closure CM-26..CM-30 — sibling
  repo dependency, multi-year horizon (Track CN carry).
- **(gated, NOT pre-authored)**: Phase CQ-P4 — real `hexa run` self_test
  confirmation × 12 modules. Materialize when G-1 trigger event arrives
  per Track CN future cycle resumption protocol.
- **(gated, NOT pre-authored)**: Phase CQ-P5 — per-surface validated-tier
  flips. Materialize when G-1 + G-2 + G-3 + G-4 + G-CDN + G-OAuth +
  G-Registry + G-FakeCdrive resolve per surface.

---

*Track CZ closure recorded 2026-05-06. CQ-P3 augment paper-mode
shape-conformant. SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 carries
unchanged. own1 / own2 / raw 9·11·12·15·175·270·271 enforce.*
