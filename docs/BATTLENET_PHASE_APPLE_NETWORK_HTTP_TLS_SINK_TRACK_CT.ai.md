# gamebox Battle.net Phase CQ-P1 — Apple Network HTTP/TLS Sink Audit (Track CT, 2026-05-05)

> Track CT on gamebox standalone repo, branch `main`. Documentation-tier
> deliverable for Phase CQ-P1 (Apple Network framework HTTP/TLS sink module)
> per `docs/BATTLENET_ROADMAP_TRACK_CQ.ai.md` §3. **No new lib/ code.** **No
> validated_manjeom promotion.** Audit + inventory + honest-spec only.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CR D2R roadmap, post Track CQ battle.net roadmap).
- Predecessor (immediate): Track CR (D2R roadmap; depends_on=[CQ]).
- Predecessor (semantic): Track CQ (battle.net roadmap; SSOT for the 5-phase plan
  this doc executes Phase 1 of).
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- Hexa interp closure CM-26..CM-30 still **perpetually deferred** (Track CN).
  Paper-mode derivation only; no real `hexa run` of any module.

Track CT does NOT author the Phase CQ-P1 module. Track CT produces the
**audit / inventory / honest-spec** deliverable that Phase CQ-P1's exit
criteria depend on — i.e., the spec a future authoring cycle (CT+1 candidate)
would consume to write `lib/loader/pe_battlenet_apple_network_http.hexa`.

---

## §0 Why a doc-tier first cycle for CQ-P1

Phase CQ-P1 in the Track CQ roadmap is described at one paragraph of detail
(roadmap §3, Phase CQ-P1). Open Question Q1 is explicitly unresolved:

> Should Phase CQ-P1 author a single combined module, or split into
> request-side + response-side + TLS-handshake-side trackers (mirroring
> pe_battle_net_download_track's TSV multi-table style)?

Authoring before that question is settled risks rework. A doc-tier cycle
that (a) audits the three existing input modules referenced by CQ-P1, (b)
inventories the surface CQ-P1 must instrument, (c) commits to a recommended
schema shape with rationale, and (d) names the honest tier expected at exit
— is strictly cheaper than authoring + revising. CT therefore lands the
spec; CT+1 (or later) can author against it.

---

## §1 Phase CQ-P1 scope (verbatim from CQ §3, paraphrased)

- **Scope**: New `lib/loader/pe_battlenet_apple_network_http.hexa` (or
  analogous). Record/track schema for HTTP request → TLS handshake →
  response chunk → SHA-256 verify against the existing
  `pe_battle_net_download_track.hexa` instrumentation sink. **Not** the
  actual Apple Network framework binding. The **honest instrumentation
  surface** that a real client would emit into.
- **Inputs**: `pe_battlenet_agent_http_rest.hexa` (REST round-trip schema),
  `pe_battle_net_download_track.hexa` (chunk schema),
  `pe_battle_net_oauth_token.hexa` (op schema). All three already landed.
- **Exit criteria**: New module schema-validates `self_test` under
  paper-mode. own1 verdict PASS (no Wine wininet, no Apple-private API
  misuse). own2 emit count meets r0 threshold. CHANGELOG additive entry.
- **Honest tier at exit**: **skeleton-tier** (paper-mode validated, real-run
  blocked by G-1).
- **Invariant impact**: validated_manjeom=0 carries. Wine 0 carries.

Track CT does NOT extend this scope. Track CT does NOT write the module.
Track CT writes the spec and resolves Q1 with a recommendation.

---

## §2 Audit of the three input modules

### §2.1 `lib/loader/pe_battlenet_agent_http_rest.hexa` — REST round-trip schema

Five TSV tables under `persist/pe_battlenet_agent_http_rest/`:

| Table | Columns | Record fn | Key invariants |
|---|---|---|---|
| `request.tsv` | request_id, method, endpoint, status_code, bearer_token_hash | `record_agent_request` | id>0, method/endpoint non-empty, status 0–599 |
| `oauth.tsv` | oauth_id, step_kind (0–3), success (0/1) | `record_oauth_step` | step_kind in {0=challenge, 1=token_get, 2=refresh, 3=revoke} |
| `launch.tsv` | launch_id, game_uid, exe_path, launch_status_code | `record_game_launch` | uid/exe non-empty, status 0–599 |
| `install.tsv` | state_id, game_uid, state_kind (0–5), progress_pct (0–100) | `record_install_state` | state_kind in {not_installed, downloading, installing, installed, updating, error} |
| `response.tsv` | response_id, endpoint, response_size_bytes, content_type | `record_endpoint_response` | size ≥ 0; content_type non-empty |


**Observation for CQ-P1**: agent_http_rest has no TLS-handshake table, no request→response correlation key, no chunk index, no SHA-256 column. The "request" + "response" pair is independent rows. CQ-P1 must add the orthogonal axis: **transport-level evidence** (TLS handshake state, cipher suite class, certificate chain depth, response body integrity hash).

### §2.2 `lib/loader/pe_battle_net_download_track.hexa` — chunk schema

Single TSV `chunks.tsv` under `persist/pe_battle_net_download_track/`:

| Column | Range / enum |
|---|---|
| chunk_id | int > 0 |
| phase_kind | 1=manifest_fetch, 2=chunk_download, 3=integrity_verify (SHA-256), 4=patch_apply, 5=resume_download, 6=cleanup |
| bytes_downloaded | int ≥ 0 |
| cdn_kind | 1=bnetcdn, 2=akamaihd, 3=p2p, 4=local_cache |

`self_test` emits 8 record rows + 1 stats row = 9 minimum, all 6 phase_kinds and ≥3 cdn_kinds covered.

**Observation for CQ-P1**: download_track is the canonical instrumentation **sink** that CQ-P1's HTTP/TLS module must feed into. The integrity_verify (phase_kind=3) row is **already** the SHA-256 verify slot — CQ-P1 should NOT duplicate that semantic. CQ-P1's role is to record the **HTTP/TLS round-trip metadata** that yielded the bytes whose SHA-256 download_track later verifies. Linkage: a `chunk_id` foreign key from CQ-P1's row → download_track's `chunks.tsv`.

### §2.3 `lib/loader/pe_battle_net_oauth_token.hexa` — op schema

Single TSV `ops.tsv` under `persist/pe_battle_net_oauth_token/`:

| Column | Range / enum |
|---|---|
| op_id | int > 0 |
| op_kind | 1=oauth_authorize, 2=token_exchange, 3=bearer_attach, 4=token_refresh, 5=token_revoke, 6=device_code_grant |
| token_lifetime_sec | int ≥ 0 (typical access_token = 86400s) |
| token_size_bytes | int ≥ 0 (length only — **no real token values**, RFC6749 spec reference) |

`self_test` emits ≥7 r0 rows. Endpoints enumerated in module header: `login.battle.net/oauth/{authorize,token,revoke,device}`; bearer attached to `{us,eu,kr}.api.blizzard.com/{sc2/profile,d3/data,wow}`.

**Observation for CQ-P1**: oauth_token already records the token-lifecycle ops. CQ-P1 must NOT re-record OAuth flow steps. CQ-P1's role for OAuth-bearing requests is to record the **transport** (TLS, host, response code, body size) of the HTTPS calls that carry the bearer token — and link to oauth_token's `op_id` when a bearer was attached (op_kind=3). Linkage: optional `op_id` foreign key from CQ-P1's row → oauth_token's `ops.tsv`.

---

## §3 Surface inventory — what CQ-P1 must instrument

A real Apple Network framework HTTP/TLS client (when it exists) emits the
following observable events. CQ-P1's instrumentation sink must accept rows
that name each of these axes honestly without claiming any one is exercised
end-to-end.

### §3.1 Request-side events

- **R-a** request_open: method, host, port (typically 443), path, header-count, body-size
- **R-b** request_send: bytes_sent, sent_time_ms (relative to open)
- **R-c** request_complete: bytes_sent_total

### §3.2 TLS-handshake-side events

- **T-a** tls_open: target_host, target_port, sni_host, alpn_offered (h2 / http/1.1)
- **T-b** tls_handshake_done: tls_version (1.2 / 1.3), cipher_suite_class (aead-gcm / aead-chacha20 / cbc-sha — string class only, no key material), cert_chain_depth, alpn_selected
- **T-c** tls_close: close_kind (graceful / abort / timeout)

### §3.3 Response-side events

- **R'-a** response_headers: status_code, content_length, content_type, transfer_encoding (chunked / identity), cache_kind (cdn-hit / cdn-miss / origin / local — string class only)
- **R'-b** response_body_chunk: bytes_received, chunk_index, link_chunk_id (foreign key to download_track.chunks.tsv when applicable)
- **R'-c** response_complete: bytes_received_total, body_sha256_hex (links to download_track phase_kind=3 verify)

### §3.4 Cross-cutting

- **X-a** host_kind enum: 1=login.battle.net, 2=us.api.blizzard.com, 3=eu.api.blizzard.com, 4=kr.api.blizzard.com, 5=bnetcdn, 6=akamaihd, 7=agent_local (port 1120 loopback), 8=other
- **X-b** purpose_kind enum: 1=oauth_authorize, 2=oauth_token, 3=oauth_revoke, 4=api_call, 5=manifest_fetch, 6=chunk_download, 7=patch_apply, 8=agent_rest, 9=other
- **X-c** request↔response correlation key: an `exchange_id` shared by the request_open / send / complete + tls_* + response_* rows for one logical HTTP exchange

### §3.5 What CQ-P1 must NOT instrument

- **Real token bytes** — only token_size_bytes (length) per oauth_token precedent
- **Real cipher key material / session keys** — only cipher_suite_class string
- **Real certificate bytes / private keys** — only cert_chain_depth
- **Real bearer token values** — only `bearer_attached: 0/1` and optional `op_id` link to oauth_token
- **Real response body** — only bytes_received + sha256_hex when applicable
- **Apple-private SPI** — Network framework public API only (no `__SecGetTrustSettings` or other SPI)

---

## §4 Schema decision — Q1 resolution

Recommendation: **multi-table single-module** (Q1 answered: split into 3 TSV tables, all under one `pe_battlenet_apple_network_http.hexa` module).

Rationale:

| Option | Pros | Cons |
|---|---|---|
| Single combined TSV (one row per exchange) | simpler, fewer files | wide row (15+ cols), mixes concerns, hard to extend per-axis |
| 3 separate modules (request / TLS / response) | strict separation | 3× the loader cluster bloat, 3× G-1 blocked surfaces, 3× CHANGELOG entries |
| **Multi-table single module** (recommended) | 1 module / 3 TSVs, mirrors `pe_battlenet_agent_http_rest.hexa` precedent (which has 5 TSVs in one module), composable, extension-friendly | slightly longer file |

The chosen option mirrors `pe_battlenet_agent_http_rest.hexa` (5 tables in one module) and `pe_battle_net_download_track.hexa` (1 table per module — but its phase enum is the discriminator, equivalent to multi-table). Multi-table single module is the established Track-CO-era idiom.

### §4.1 Recommended TSV layout for CQ-P1 module (spec only, NOT authored)

```
persist/pe_battlenet_apple_network_http/
  request.tsv:   exchange_id, request_open_id, host_kind, purpose_kind,
                 method, path, header_count, body_size, bytes_sent_total
  tls.tsv:       exchange_id, tls_id, target_host, sni_host,
                 tls_version, cipher_suite_class, cert_chain_depth,
                 alpn_selected, close_kind
  response.tsv:  exchange_id, response_id, status_code,
                 content_length, content_type, transfer_encoding,
                 cache_kind, bytes_received_total, body_sha256_hex,
                 link_chunk_id, link_oauth_op_id, bearer_attached
```

All three tables are linked by `exchange_id`. `link_chunk_id` is a foreign
key into `pe_battle_net_download_track.chunks.tsv`; `link_oauth_op_id` is a
foreign key into `pe_battle_net_oauth_token.ops.tsv`. Both foreign keys are
**optional** (0 = unlinked).

### §4.2 Recommended record functions (signatures only, NOT authored)

```
fn record_request(exchange_id: int, request_open_id: int,
                  host_kind: int, purpose_kind: int,
                  method: str, path: str,
                  header_count: int, body_size: int,
                  bytes_sent_total: int) -> void

fn record_tls(exchange_id: int, tls_id: int,
              target_host: str, sni_host: str,
              tls_version: str, cipher_suite_class: str,
              cert_chain_depth: int, alpn_selected: str,
              close_kind: str) -> void

fn record_response(exchange_id: int, response_id: int,
                   status_code: int, content_length: int,
                   content_type: str, transfer_encoding: str,
                   cache_kind: str, bytes_received_total: int,
                   body_sha256_hex: str,
                   link_chunk_id: int, link_oauth_op_id: int,
                   bearer_attached: int) -> void
```

Plus the standard counter / stats / cleanup / self_test helpers per
`pe_battlenet_agent_http_rest.hexa` precedent.

### §4.3 Recommended self_test fan-out (paper-mode targets, NOT authored)

- ≥6 request rows (1 per host_kind 1/2/5/6 + 2 for variety)
- ≥6 tls rows (covering tls_version 1.2 + 1.3 across ≥3 cipher_suite_classes)
- ≥6 response rows (covering status codes 200, 206, 304, 404, 500 spread; ≥2 with link_chunk_id non-zero; ≥2 with link_oauth_op_id non-zero; ≥2 with bearer_attached=1)
- r0_emit total ≥ 19 (6+6+6+1 stats)
- All 18 records share consistent exchange_id grouping (3 exchanges of 6 rows each, OR 6 exchanges of 3 rows each)

---

## §5 Honest tier expected at CQ-P1 exit

Per Track CQ §3 Phase CQ-P1: **skeleton-tier**. Track CT inherits and refines:

- **Surface coverage** at module-author exit: 100% (every recommended record fn present, every column present, every enum populated, self_test schema-validated under paper-mode).
- **Real-run validation** at module-author exit: **0** (G-1 hexa interp blocker; no real `hexa run` possible).
- **Real Apple Network framework binding** at module-author exit: **0** (this module is the **instrumentation sink only**; the actual Network framework client is a separate, post-G-4 deliverable).
- **validated_manjeom** at module-author exit: **0** (no real first-byte over the wire; no real TLS handshake to `*.battle.net:443`; no real CDN bytes verified).

Aggregate posture flips on module landing: SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 → SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 (unchanged; +1 to skeleton module count, 0 change to validated count).

---

## §6 Track CT deliverable — what landed THIS cycle

| File | Op | Purpose |
|---|---|---|
| `docs/BATTLENET_PHASE_APPLE_NETWORK_HTTP_TLS_SINK_TRACK_CT.ai.md` | NEW | this doc |
| `state/markers/battlenet_phase_apple_network_http_tls_sink_track_ct_landed.marker` | NEW | this Track marker |

`in_place_writes`: 0. NEW files: 2. NO lib/ change. NO tool/ change. NO
existing module mutation. NO CHANGELOG / RELEASE_NOTES / README / hexa.toml /
LICENSE touch. NO predecessor marker mutation.

---

## §7 What CT does NOT deliver (deferred to future cycle)

- Authoring of `lib/loader/pe_battlenet_apple_network_http.hexa` itself
  (CT+1 candidate; consumes this spec)
- CHANGELOG additive entry for the new module (lands with the module)
- Schema validation under paper-mode (lands with the module)
- Q2..Q7 from Track CQ (CT addresses Q1 only; Q2-Q7 carry forward unchanged)
- Phase CQ-P2 / CQ-P3 doc-tier specs (separate future Tracks)
- Phase CQ-P4 / CQ-P5 (gated on G-1 + G-2 + G-3 + G-4; not pre-authored)

---

## §8 Cross-references

- `docs/BATTLENET_ROADMAP_TRACK_CQ.ai.md` — parent roadmap, SSOT for the
  5-phase plan
- `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` — Track P naming
  disambiguation (sister doc, not superseded)
- `docs/D2R_ROADMAP_TRACK_CR.ai.md` — sibling roadmap (D2R), depends_on=[CQ];
  Track CR Phase R2 depends on Track CQ Phase 1-3 landing — including the
  module CQ-P1 specifies
- `lib/loader/pe_battlenet_agent_http_rest.hexa` — input module (REST
  round-trip schema; multi-table single-module precedent)
- `lib/loader/pe_battle_net_download_track.hexa` — input module (chunk
  schema; foreign-key target for `link_chunk_id`)
- `lib/loader/pe_battle_net_oauth_token.hexa` — input module (op schema;
  foreign-key target for `link_oauth_op_id`)

---

## §9 Retired_intentional / sideways-PASS posture (carry forward)

Track CT touches NONE of the surfaces below; they remain block-acknowledged
PASS-equivalent per `project_retired_intentional_sideways_pass.md`. Future
authoring of CQ-P1 must NOT violate any of these boundaries:

| Posture | Surface | Source Track |
|---|---|---|
| retired_intentional | D4 (online-only DRM) | P |
| honest_freeze | Warden user-mode anti-cheat | J |
| honest_freeze | GameGuard kernel-mode anti-cheat | E |
| honest_freeze | D2R realm protocol session spoof | L |
| honest_freeze | VAC | CK |
| honest_freeze | EAC kernel + anti-tamper | CL |
| honest_freeze | FromSoft matchmaking | CL cross-class |
| naming_disambiguation | `lib/loader/battlenet_bypass.hexa` "bypass" naming | P |
| perpetual_deferral_ack | hexa interp closure CM-26..CM-30 real-run hook 4 | CN |

---

## §10 Open questions

CT addresses **Q1 only** (CQ §5 Q1 resolved: multi-table single module).
Q2..Q7 carry forward unchanged from Track CQ §5.

CT-specific opens (new):

- **CT-Q1**: Should `cipher_suite_class` be a fixed enum (e.g., 1=aead-gcm /
  2=aead-chacha20 / 3=cbc-sha / 4=other) or free-form string? Enum is more
  honest about category but loses sub-suite detail; string is more
  expressive but harder to validate. Default: enum (mirrors `phase_kind` /
  `cdn_kind` precedent).
- **CT-Q2**: Should `body_sha256_hex` be 64-char hex string or empty when
  unverified? Default: empty string when unverified, 64-char hex when
  verified — `len(body_sha256_hex) in {0, 64}` invariant.
- **CT-Q3**: Foreign-key validation — should the CQ-P1 module verify that
  `link_chunk_id` and `link_oauth_op_id` are present in the linked TSVs at
  record time? Default: NO (foreign-key check is deferred to a future
  cross-module orchestrator; CQ-P1 is a sink, not a validator).

---

## §11 Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (2 NEW files only)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (no user-language directives quoted; intent
  paraphrased only)
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no AC bypass / no DRM bypass / no anima
  clone / no fake PASS / no fake real-run completion / no validated_manjeom
  inflation ✓
- **own2**: forensic verdict honest unchanged; skeleton-tier vs
  validated-tier distinction preserved; perpetual deferral semantics
  carried; validated_manjeom=0 carries ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓

---

## §12 Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

Track CT is doc-tier audit/inventory/honest-spec for Phase CQ-P1. NO module
authored / NO lib change / NO state mutation / NO validated_manjeom
promotion / NO hexa interp dependency assumed-resolved.

---

## §13 Predecessors

- Track CR — `state/markers/d2r_roadmap_track_cr_landed.marker` (immediate predecessor; D2R roadmap)
- Track CQ — `state/markers/battlenet_roadmap_track_cq_landed.marker` (parent roadmap; CT executes its Phase 1)
- Track CN — `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker` (G-1 perpetual deferral source)
- Track CO — `state/markers/gamebox_v1_0_4_release_notes_track_co_landed.marker` (release bundle predecessor)
- Track P — `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` (sister disambiguation doc, permanent reference)

---

## §14 Next-cycle hooks

- **(NEW Track CT+1 candidate)**: author `lib/loader/pe_battlenet_apple_network_http.hexa` against §4 spec; validate paper-mode self_test; CHANGELOG additive entry — produces skeleton-tier landed module per CQ-P1 exit criteria
- **(future, separate)**: Track CT+2 candidate doc-tier spec for Phase CQ-P2 (TACT/NGDP/CASC parser skeleton)
- **(future, separate)**: Track CT+3 candidate doc-tier spec for Phase CQ-P3 (Agent HTTP REST 4-verb PARTIAL augment)
- **(perpetually deferred)**: G-1 hexa interp closure CM-26..CM-30 (Track CN)
- **(gated, NOT pre-authored)**: Phase CQ-P4 / CQ-P5

CT does NOT pre-author the CQ-P1 module. CT does NOT promise CT+1 timing.
CT does NOT touch CQ-P2..CQ-P5 detail.
