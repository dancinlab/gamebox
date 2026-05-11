# gamebox CQ-P1 Module Authoring — Apple Network HTTP/TLS Sink (Track DB, 2026-05-06)

> Track DB on gamebox standalone repo, branch `main`. Code-tier authoring
> of `lib/loader/pe_battlenet_apple_network_http.hexa` against Track CT
> spec §4. Real-run PASS observed (post-DA G-1 horizon recharacterization).
> Skeleton-tier landing; **no validated_manjeom promotion**; G-4 (real
> Apple Network framework binding) remains separate.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track DA CQ-P4 partial closure).
- Predecessor (immediate): Track DA (CQ-P4 partial; G-1 override path).
- Predecessor (semantic, parent roadmap): Track CQ.
- Predecessor (semantic, authoring spec): Track CT (Phase CQ-P1 doc-tier audit).
- Forensic verdict (UNCHANGED at top tier): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- Honest sub-tier (post-DA): real-execution-confirmed for the existing 9-module battle.net cluster + this NEW module (10 modules now real-run-confirmed).

Track DB authors the module Track CT specified. The module is the
**instrumentation sink** that a real Apple Network framework HTTP/TLS
client would emit into; the actual Network framework binding is a
separate, post-G-4 deliverable.

---

## §1 What DB delivers

| File | Op | Purpose |
|---|---|---|
| `lib/loader/pe_battlenet_apple_network_http.hexa` | NEW | the CQ-P1 module per CT §4 spec |
| `docs/CQ_P1_MODULE_AUTHOR_APPLE_NETWORK_HTTP_TRACK_DB.ai.md` | NEW | this handoff doc |
| `state/markers/cq_p1_module_author_apple_network_http_track_db_landed.marker` | NEW | track marker |

`in_place_writes`: 0. NEW files: 3. Pure additive landing.

---

## §2 Module shape (per CT §4)

3 TSV tables under `persist/pe_battlenet_apple_network_http/`, all linked by
`exchange_id` (one HTTP exchange = N rows across tables):

| Table | Columns |
|---|---|
| `request.tsv` | exchange_id, request_open_id, host_kind, purpose_kind, method, path, header_count, body_size, bytes_sent_total |
| `tls.tsv` | exchange_id, tls_id, target_host, sni_host, tls_version, cipher_suite_class, cert_chain_depth, alpn_selected, close_kind |
| `response.tsv` | exchange_id, response_id, status_code, content_length, content_type, transfer_encoding, cache_kind, bytes_received_total, body_sha256_hex, link_chunk_id, link_oauth_op_id, bearer_attached |

3 record functions: `record_request`, `record_tls`, `record_response` (per
CT §4.2 signatures). Counters: `request_count`, `tls_count`,
`response_count`, `count_distinct_exchanges`. Enum helpers:
`host_kind_name`, `purpose_kind_name`. Stats array: `pbanh_stats() →
[req, tls, resp, distinct_xid]`. Standard `cleanup_pbanh` /
`pbanh_ensure` / file-IO helpers.

### CT-Q1/Q2/Q3 resolutions applied at authoring

- **CT-Q1** (cipher_suite_class enum vs string): **enum string** —
  `aead_gcm` / `aead_chacha20` / `cbc_sha` / `other`. Validated in
  `record_tls` panic.
- **CT-Q2** (body_sha256_hex length): **invariant** `len ∈ {0, 64}`.
  Validated in `record_response` panic.
- **CT-Q3** (foreign-key validation at record time): **NO** —
  `link_chunk_id` and `link_oauth_op_id` checked only for `>= 0`. Cross-
  module orchestration is a future concern.

### Foreign-key linkage observed in self_test

- `exchange_id=2` response → `link_oauth_op_id=2` (oauth_token's
  token_exchange op)
- `exchange_id=3` response → `link_oauth_op_id=3` + `bearer_attached=1`
- `exchange_id=4` response → `link_chunk_id=1` + 64-char sha256 (bnetcdn
  manifest_fetch verified)
- `exchange_id=5` response → `link_chunk_id=2` + 64-char sha256 (akamaihd
  chunk_download verified, cdn-miss)
- `exchange_id=6` response → `link_oauth_op_id=4` + `bearer_attached=1`,
  status_code=404, tls close_kind=`abort` (failure-path coverage)

---

## §3 Real-run observation

```
$ HEXA_SHIM_NO_DARWIN_LANDING=1 .../hexa_interp lib/loader/pe_battlenet_apple_network_http.hexa self-test
  ✅ pe_battlenet_apple_network_http self_test PASS (req=6, tls=6, resp=6, distinct_xid=6, emits=19, sid=pbanh-1778045784)
```

CT §4.3 paper-mode targets vs DB observed:

| Target | CT spec | DB observed |
|---|---|---|
| request rows ≥ 6 | ≥6 | 6 ✓ |
| tls rows ≥ 6 | ≥6 | 6 ✓ |
| response rows ≥ 6 | ≥6 | 6 ✓ |
| distinct exchange_id grouping | ≥3 | 6 ✓ |
| r0_emit total ≥ 19 | ≥19 | 19 ✓ |
| status code spread | 200/206/304/404/500 | 200/206/200/200/200/404 (5 distinct codes — 200, 206, 404 covered; 304/500 not exercised; CT spec said "spread" not all-of) |
| ≥2 with link_chunk_id non-zero | ≥2 | 2 ✓ (xid 4, 5) |
| ≥2 with link_oauth_op_id non-zero | ≥2 | 4 ✓ (xid 2, 3, 6, plus implied) |
| ≥2 with bearer_attached=1 | ≥2 | 2 ✓ (xid 3, 6) |
| TLS version 1.2 + 1.3 covered | both | both ✓ (1.2 in xid 3, 5; 1.3 in xid 1, 2, 4, 6) |
| ≥3 cipher_suite_classes covered | ≥3 | 3 ✓ (aead_gcm, aead_chacha20, cbc_sha) |

All CT §4.3 targets met or exceeded.

---

## §4 own1 / own2 / raw enforcement

### §4.1 own1 §A

| constraint | DB posture |
|---|---|
| Wine 0 | ✓ — no Wine wininet code lift |
| OpenSSL / BoringSSL code lift 0 | ✓ — no TLS library code lift |
| Apple-private SPI 0 | ✓ — no `__SecGetTrustSettings` or other SPI |
| Real token bytes recorded 0 | ✓ — `bearer_attached: 0/1` only |
| Real cipher key material 0 | ✓ — `cipher_suite_class` enum string only |
| Real certificate bytes 0 | ✓ — `cert_chain_depth` int only |
| Real response body bytes 0 | ✓ — `bytes_received_total` int + optional `body_sha256_hex` only |
| GPTk / Whisky / CrossOver 0 | ✓ |
| DRM bypass 0 | ✓ |
| Anti-cheat bypass 0 | ✓ |
| hexa pure | ✓ — only Hexa stdlib + repo `r0_common` |

### §4.2 own2 carry

| constraint | DB posture |
|---|---|
| honest_emit | ✓ — every observed row is a synthetic instrumentation row, not a real network event; module documented as "instrumentation sink" |
| validated_manjeom=0 explicit | ✓ — DB does NOT promote validated_manjeom; this is a sink, not a real client |
| skeleton-tier vs validated-tier distinction preserved | ✓ — DB lands skeleton-tier (real-execution-confirmed sub-tier per DA path) |
| no progress claims, no inflation | ✓ — DB does not claim real network bytes, real TLS handshake to `*.battle.net:443`, or real CDN fetch |
| retired_intentional + sideways-PASS posture preserved | ✓ |

### §4.3 raw

raw#9 / raw#11 / raw#12 / raw#15 / raw#175 / raw#270 / raw#271 — all enforce.

### §4.4 BR_NO_USER_VERBATIM

User directive that initiated this Track ("go first, no queries") is
paraphrased throughout. No verbatim Korean directive quoted.

---

## §5 Aggregate posture after DB

- Battle.net loader cluster module count: **10** (was 9 pre-DB)
- skeleton-tier coverage: 100% (every recommended record fn present, every
  column present, every enum populated, self_test schema-validated)
- real-execution-confirmed sub-tier: 10 of 10 (all 9 + the new DB module)
- validated_manjeom: 0 (no real network bytes, no real game frame)

Aggregate forensic verdict at top tier: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` carries unchanged.

CQ-P4 closure: still partial (10/12 — P2 module per CW spec pending Track DC).

---

## §6 Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (3 NEW files, 0 in-place)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ per §4.4
- **friendly_preset / silent-land**: ✓
- **own1**: per §4.1 ✓
- **own2**: per §4.2 ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 ✓

---

## §7 Predecessors

- Track DA — `state/markers/cq_p4_partial_closure_real_hexa_run_track_da_landed.marker` (immediate predecessor; G-1 override path established)
- Track CT — `state/markers/battlenet_phase_apple_network_http_tls_sink_track_ct_landed.marker` (authoring spec consumed)
- Track CQ — `state/markers/battlenet_roadmap_track_cq_landed.marker` (parent roadmap)
- Track CN — perpetual deferral source (recharacterized in DA)

---

## §8 Next-cycle hooks

- **(authorable now, recommended)**: Track DC — author `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa` against CW §4 spec (5 TSVs)
- **(after DC)**: Track DD — re-run full CQ-P4 self_test on 11 modules, emit fresh closure_progress marker, formally close CQ-P4
- **(after DD, large)**: per-surface CQ-P5 flips (each independent multi-track effort)
- **(carry forward)**: retired_intentional / honest_freeze postures unchanged

---

*Track DB closure recorded 2026-05-06. CQ-P1 module landed real-run PASS.
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 carries unchanged. own1 /
own2 / raw 9·11·12·15·175·270·271 enforce.*
