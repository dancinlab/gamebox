# gamebox Battle.net Phase CQ-P2 — TACT/NGDP/CASC Parser Skeleton Audit (Track CW, 2026-05-05)

> Track CW on gamebox standalone repo, branch `main`. Documentation-tier
> deliverable for Phase CQ-P2 (TACT/NGDP/CASC parser skeleton module) per
> `docs/BATTLENET_ROADMAP_TRACK_CQ.ai.md` §3. **No new lib/ code.** **No
> validated_manjeom promotion.** Audit + inventory + honest-spec only.

---

## Context

- Repo: `need-singularity/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CV D4 D3D12 surface honesty audit).
- Predecessor (immediate): Track CV (D4 D3D12 surface honesty audit; depends_on=[CS]).
- Predecessor (semantic, parent roadmap): Track CQ (battle.net roadmap; SSOT
  for the 5-phase plan; this doc executes Phase CQ-P2 of).
- Predecessor (semantic, sibling phase): Track CT (Phase CQ-P1 doc-tier audit
  for the Apple Network HTTP/TLS sink; established the doc-tier idiom this
  Track mirrors).
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- Hexa interp closure CM-26..CM-30 still **perpetually deferred** (Track CN).
  Paper-mode derivation only; no real `hexa run` of any module.

Track CW does NOT author the Phase CQ-P2 module. Track CW produces the
**audit / inventory / honest-spec** deliverable that Phase CQ-P2's exit
criteria depend on — i.e., the spec a future authoring cycle (CW+1
candidate) would consume to write
`lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa`.

---

## §0 Why a doc-tier first cycle for CQ-P2

Phase CQ-P2 in the Track CQ roadmap is described at one paragraph of detail
(roadmap §3, Phase CQ-P2). Open Question Q2 is explicitly unresolved:

> Phase CQ-P2 (TACT/NGDP/CASC) — does the BLTE chunk parser belong with
> download_track (extension) or as its own module (separation of concerns)?
> Track P precedent suggests "do not modify existing modules; author new +
> cross-link" but each new module adds to G-1's blocked-surface count.

Authoring before that question is settled risks rework. A doc-tier cycle
that (a) audits the existing input module referenced by CQ-P2, (b)
inventories the TACT/NGDP/CASC public-spec surface CQ-P2 must instrument
without lifting code, (c) commits to a recommended schema shape with
rationale (separate-module vs. extension), and (d) names the honest tier
expected at exit — is strictly cheaper than authoring + revising. CW
therefore lands the spec; CW+1 (or later) can author against it.

This Track mirrors Track CT (Phase CQ-P1 doc-tier audit) — same cycle
shape, same restraint envelope. CT executed Phase 1 of CQ at doc-tier; CW
executes Phase 2 of CQ at doc-tier.

---

## §1 Phase CQ-P2 scope (verbatim from CQ §3, paraphrased)

- **Scope**: New `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa`.
  TSV-persisted phase tracker for: BLP/manifest fetch → BLTE chunk parse →
  MD5 verify → LZMA inflate → install_path write. Mirror style of
  `pe_battle_net_download_track.hexa`.
- **Inputs**: research-E TACT spec direction (already cited in
  REAL_LAUNCH_PLAN v2 §5 / wave 20). own1 boundary: **no Wine cab /
  wininet code lift**, public spec only.
- **Exit criteria**: schema-validates, r0_emit ≥ threshold, panic format
  honest. own1 verdict PASS.
- **Honest tier at exit**: **skeleton-tier**.
- **Invariant impact**: identical to CQ-P1 — validated_manjeom=0 carries.
  Wine 0 carries. raw#9/11/12/15/175/270/271 enforce.

Track CW does NOT extend this scope. Track CW does NOT write the module.
Track CW writes the spec and resolves Q2 with a recommendation.

---

## §2 Audit of the input module + spec direction

### §2.1 `lib/loader/pe_battle_net_download_track.hexa` — chunk schema (sibling, NOT extended by CW)

Single TSV `chunks.tsv` under `persist/pe_battle_net_download_track/`:

| Column | Range / enum |
|---|---|
| chunk_id | int > 0 |
| phase_kind | 1=manifest_fetch, 2=chunk_download, 3=integrity_verify (SHA-256), 4=patch_apply, 5=resume_download, 6=cleanup |
| bytes_downloaded | int ≥ 0 |
| cdn_kind | 1=bnetcdn, 2=akamaihd, 3=p2p, 4=local_cache |

`self_test` emits 8 record rows + 1 stats row = 9 minimum. All 6
phase_kinds and 3 cdn_kinds (1, 2, 4) covered. SHA-256 verify is the
phase_kind=3 row.

**Observation for CQ-P2**: download_track is the Battle.net **download
protocol observation** layer — generic chunk fetch with SHA-256 integrity.
The format-specific TACT/NGDP/CASC understanding (BLTE compressed-block
container, MD5 keys, LZMA inflation, content_table indirection) is
**NOT** present here. download_track records "a chunk arrived"; CQ-P2
must record "a BLTE block was parsed into N inflated bytes whose MD5
matched the encoding-key reference". These are orthogonal axes.

**Foreign key candidate**: a CQ-P2 row that produced inflated bytes used
by a download_track patch_apply / cleanup phase could carry a
`link_chunk_id` pointer into download_track's chunks.tsv, mirroring the
CT spec's `link_chunk_id` foreign-key pattern. Optional (0=unlinked).

### §2.2 `docs/BATTLENET_REAL_LAUNCH_PLAN.md` — research-E TACT direction

REAL_LAUNCH_PLAN v2 §5 + wave 20 (cycle 127-130) names the TACT/NGDP/CASC
self-implementation direction:

- LZMA decode → `libcompression.dylib` (Apple Compression public API)
- MD5 → CommonCrypto (Apple public API)
- HTTPS fetch → Network.framework (Apple public API; same sink Phase CQ-P1
  instruments)
- Public spec reference only — Wine cab / wininet code lift is **0 lines**

CQ-P2 is therefore an **own1-clean** parser-shape tracker: it observes the
TACT/NGDP/CASC archive structure as documented by the public spec, never
lifts decoder code from Wine/CrossOver/Whisky/GPTK or any third-party
implementation, and produces only **shape evidence** rows — not real
inflated bytes, not real MD5-verified content.

### §2.3 What CT (Phase CQ-P1) already covered that CW does NOT duplicate

- HTTP request / TLS handshake / response chunk metadata ← CT scope, not CW
- response body SHA-256 hex ← CT spec column `body_sha256_hex` (which
  itself foreign-keys into download_track phase_kind=3)
- OAuth bearer attach link ← CT scope, not CW

CW assumes those sinks exist (per CT spec). CW's parser-shape tracker
takes already-fetched bytes (transport observed by CT-spec'd module) and
records the **format-decoding** axis: BLTE block → MD5 verify → LZMA
inflate → install_path write.

---

## §3 Surface inventory — what CQ-P2 must instrument

A real TACT/NGDP/CASC parser (when authored against Apple Compression +
CommonCrypto + Network.framework) emits the following observable events.
CQ-P2's instrumentation sink must accept rows that name each axis honestly
without claiming any one is exercised end-to-end.

### §3.1 Manifest-fetch-side events

- **M-a** manifest_fetch_open: manifest_kind, region (us/eu/kr), version_id
- **M-b** manifest_parse: entry_count, root_hash_kind (md5/cdn), encoding_table_size

### §3.2 BLTE chunk-parse-side events

- **B-a** blte_open: blte_id, header_size, chunk_count_in_block,
  encoding_kind (raw / zlib / lzma / encrypted-passthrough)
- **B-b** blte_chunk: blte_id, chunk_index, encoded_size, decoded_size
- **B-c** blte_close: blte_id, total_decoded_size, integrity_pass (0/1, MD5
  match)

### §3.3 MD5-verify-side events

- **D-a** md5_verify_open: md5_id, key_kind (encoding-key / content-key /
  cdn-key), expected_md5_hex (32 hex chars), key_kind_link_id (optional FK
  to encoding-table row)
- **D-b** md5_verify_done: md5_id, computed_md5_hex (32 hex chars or empty
  when unverified), match (0/1)

### §3.4 LZMA-inflate-side events

- **L-a** lzma_inflate_open: lzma_id, blte_id_link (FK to BLTE), input_size,
  expected_output_size
- **L-b** lzma_inflate_done: lzma_id, actual_output_size,
  inflate_status_code (0=ok / 1=truncated / 2=checksum_mismatch / 3=other)

### §3.5 Install-path-write-side events

- **I-a** install_write_open: install_id, target_subpath (root-relative,
  no real C: drive prefix recorded; fake-c-drive translation done by
  G-FakeCdrive at runtime), bytes_to_write
- **I-b** install_write_done: install_id, bytes_written, write_status_code
  (0=ok / 1=eio / 2=enospc / 3=eperm / 4=other), link_chunk_id (optional FK
  to download_track.chunks.tsv when applicable)

### §3.6 Cross-cutting

- **X-a** archive_kind enum: 1=ngdp_root_manifest / 2=cdn_config /
  3=encoding_table / 4=download_table / 5=blte_block / 6=blp_texture /
  7=indexed_archive / 8=patch_archive / 9=other
- **X-b** game_kind enum: 1=d2r / 2=d4 / 3=sc2 / 4=wow / 5=hs / 6=overwatch
  / 7=other (per existing per-game UID convention)
- **X-c** stage_kind enum: 1=manifest_fetch / 2=blte_parse / 3=md5_verify /
  4=lzma_inflate / 5=install_write / 6=cleanup (mirrors download_track's
  phase_kind shape, but operates one layer up the stack — format decode
  vs. transport)
- **X-d** session correlation: a `session_id` shared by rows belonging to
  one logical install/patch session

### §3.7 What CQ-P2 must NOT instrument

- **Real archive bytes** — only sizes, counts, status codes, and
  fixed-length hex hashes for verification
- **Wine cab / wininet decoder source** — own1 boundary; public spec only,
  code lift = 0
- **Real DRM-relevant content keys** — content-key handling enumerated as
  a `key_kind` enum value but **no real key bytes recorded ever**; CQ-P2
  is a parser-shape tracker, not a key escrow
- **Apple-private SPI** — Apple Compression / CommonCrypto / Network
  framework public API only; no `__SecGetTrustSettings` or
  Compression-private symbols
- **Real BLP texture pixel data** — BLP is enumerated as an archive_kind
  for completeness (BLPs ride inside CASC) but no pixel content decoded
  or recorded
- **Anything that violates retired_intentional / honest_freeze postures
  carried forward in Track CQ §4** — CW does not reopen any such surface

---

## §4 Schema decision — Q2 resolution

Recommendation: **separate module, not extension** (Q2 answered: author
new `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa`; do NOT modify
`pe_battle_net_download_track.hexa`).

Rationale:

| Option | Pros | Cons |
|---|---|---|
| Extend download_track (add BLTE/MD5/LZMA columns to chunks.tsv) | 1 module total; simpler loader cluster count | Wide row (15+ cols), mixes transport-layer with format-decode-layer concerns; violates Track P precedent ("do not modify existing modules; author new + cross-link"); breaks existing download_track self_test invariants; mutates already-landed module |
| New module sharing one TSV with download_track | weird — two modules write the same file | violates module ownership; nondeterministic test output |
| **Separate module, multi-table single-module idiom** (recommended) | Strict separation of transport vs. format-decode concerns; mirrors `pe_battlenet_agent_http_rest.hexa` (5 tables in one module) and CT spec (3 tables in one module) precedents; foreign-key linkage to download_track keeps cross-module composition honest; no mutation of any existing module | Adds 1 to G-1 blocked-surface count; +1 loader cluster module |

The chosen option respects Track P precedent (Track CQ §10 explicitly
cites "do not modify existing modules; author new + cross-link") and
mirrors CT's resolution of the analogous Q1 (multi-table single module).

### §4.1 Recommended TSV layout for CQ-P2 module (spec only, NOT authored)

```
persist/pe_battlenet_tact_ngdp_casc_track/
  manifest.tsv:  session_id, manifest_id, archive_kind, game_kind,
                 region, version_id, entry_count,
                 encoding_table_size, root_hash_kind
  blte.tsv:      session_id, blte_id, header_size, chunk_count_in_block,
                 encoding_kind, chunk_index, encoded_size,
                 decoded_size, total_decoded_size, integrity_pass
  md5.tsv:       session_id, md5_id, key_kind,
                 expected_md5_hex, computed_md5_hex, match,
                 key_kind_link_id
  lzma.tsv:      session_id, lzma_id, blte_id_link,
                 input_size, expected_output_size,
                 actual_output_size, inflate_status_code
  install.tsv:   session_id, install_id, game_kind, stage_kind,
                 target_subpath, bytes_to_write, bytes_written,
                 write_status_code, link_chunk_id
```

All five tables are linked by `session_id`. `link_chunk_id` is a foreign
key into `pe_battle_net_download_track.chunks.tsv`. `blte_id_link` is an
intra-module FK from `lzma.tsv` → `blte.tsv`. `key_kind_link_id` is an
optional intra-module FK from `md5.tsv` → `manifest.tsv` (encoding-table
row). All foreign keys are **optional** (0 = unlinked).

### §4.2 Recommended record functions (signatures only, NOT authored)

```
fn record_manifest(session_id: int, manifest_id: int,
                   archive_kind: int, game_kind: int,
                   region: str, version_id: str,
                   entry_count: int, encoding_table_size: int,
                   root_hash_kind: str) -> void

fn record_blte(session_id: int, blte_id: int,
               header_size: int, chunk_count_in_block: int,
               encoding_kind: str, chunk_index: int,
               encoded_size: int, decoded_size: int,
               total_decoded_size: int, integrity_pass: int) -> void

fn record_md5(session_id: int, md5_id: int, key_kind: str,
              expected_md5_hex: str, computed_md5_hex: str,
              match: int, key_kind_link_id: int) -> void

fn record_lzma(session_id: int, lzma_id: int, blte_id_link: int,
               input_size: int, expected_output_size: int,
               actual_output_size: int, inflate_status_code: int) -> void

fn record_install(session_id: int, install_id: int,
                  game_kind: int, stage_kind: int,
                  target_subpath: str, bytes_to_write: int,
                  bytes_written: int, write_status_code: int,
                  link_chunk_id: int) -> void
```

Plus the standard counter / stats / cleanup / self_test helpers per
`pe_battlenet_agent_http_rest.hexa` precedent.

### §4.3 Recommended self_test fan-out (paper-mode targets, NOT authored)

- ≥3 manifest rows (covering ≥2 archive_kinds, ≥1 game_kind = d2r per
  Track P feasibility verdict; D4 NOT exercised — retired_intentional
  posture)
- ≥4 blte rows (covering ≥3 encoding_kinds: raw / zlib / lzma; ≥1 with
  integrity_pass=1 and ≥1 with integrity_pass=0 to exercise both
  branches)
- ≥4 md5 rows (covering ≥2 key_kinds; ≥2 with match=1 and ≥1 with match=0;
  hex strings 32 chars when verified, empty when unverified — invariant
  `len(expected_md5_hex) == 32`, `len(computed_md5_hex) in {0, 32}`)
- ≥3 lzma rows (covering ≥2 inflate_status_codes; ≥1 with `blte_id_link`
  non-zero)
- ≥3 install rows (covering ≥2 stage_kinds; ≥1 with `link_chunk_id`
  non-zero; only game_kind=d2r exercised in self_test, retired_intentional
  surfaces NOT exercised)
- r0_emit total ≥ 18 (3+4+4+3+3+1 stats = 18 minimum)
- All 17 records share consistent session_id grouping (≥2 sessions to
  exercise multi-session composition)

---

## §5 Honest tier expected at CQ-P2 exit

Per Track CQ §3 Phase CQ-P2: **skeleton-tier**. Track CW inherits and
refines:

- **Surface coverage** at module-author exit: 100% (every recommended
  record fn present, every column present, every enum populated, self_test
  schema-validated under paper-mode).
- **Real-run validation** at module-author exit: **0** (G-1 hexa interp
  blocker; no real `hexa run` possible).
- **Real Apple Compression / CommonCrypto / Network.framework binding** at
  module-author exit: **0** (this module is the **instrumentation sink
  only**; the actual TACT/NGDP/CASC parser implementation is a separate,
  post-G-CDN deliverable).
- **Real BLTE bytes inflated / real MD5 verified / real LZMA decoded** at
  module-author exit: **0**.
- **validated_manjeom** at module-author exit: **0** (no real CDN bytes
  fetched; no real archive parsed; no real install written).

Aggregate posture flips on module landing:
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 →
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 (unchanged; +1 to skeleton
module count, 0 change to validated count). Same posture flip pattern as
CT-spec'd module.

---

## §6 Track CW deliverable — what landed THIS cycle

| File | Op | Purpose |
|---|---|---|
| `docs/BATTLENET_PHASE_TACT_NGDP_CASC_PARSER_TRACK_CW.ai.md` | NEW | this doc |
| `state/markers/battlenet_phase_tact_ngdp_casc_parser_track_cw_landed.marker` | NEW | this Track marker |

`in_place_writes`: 0. NEW files: 2. NO lib/ change. NO tool/ change. NO
existing module mutation. NO CHANGELOG / RELEASE_NOTES / README /
hexa.toml / LICENSE touch. NO predecessor marker mutation.

---

## §7 What CW does NOT deliver (deferred to future cycle)

- Authoring of `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa` itself
  (CW+1 candidate; consumes this spec)
- CHANGELOG additive entry for the new module (lands with the module)
- Schema validation under paper-mode (lands with the module)
- Real Apple Compression / CommonCrypto / Network.framework binding (G-CDN
  per Track CQ §2.2; not authored at module-spec land)
- Q3..Q7 from Track CQ (CW addresses Q2 only; Q3-Q7 carry forward)
- Phase CQ-P3 doc-tier spec (separate future Track)
- Phase CQ-P4 / CQ-P5 (gated on G-1 + G-2 + G-3 + G-4; not pre-authored)
- Any CT+1 work (CT-spec'd Apple Network HTTP/TLS sink module is a
  separate authoring track, not bundled with CW)

---

## §8 Cross-references

- `docs/BATTLENET_ROADMAP_TRACK_CQ.ai.md` — parent roadmap, SSOT for the
  5-phase plan
- `docs/BATTLENET_PHASE_APPLE_NETWORK_HTTP_TLS_SINK_TRACK_CT.ai.md` —
  sister doc-tier audit for Phase CQ-P1; format and tone mirror; resolves
  CQ Q1 (CW resolves CQ Q2 in the analogous shape)
- `docs/BATTLENET_REAL_LAUNCH_PLAN.md` — research-E TACT direction (§5,
  wave 20); historical reference, superseded by Track CQ for the
  forward-looking plan
- `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` — Track P naming
  disambiguation (sister doc, not superseded)
- `docs/D2R_ROADMAP_TRACK_CR.ai.md` — sibling roadmap (D2R), depends_on=[CQ];
  Track CR Phase R2 depends on Track CQ Phase 1-3 landing — including the
  module CQ-P2 specifies
- `lib/loader/pe_battle_net_download_track.hexa` — sibling module
  (transport-layer chunk schema); foreign-key target for `link_chunk_id`;
  NOT mutated by CW
- `lib/loader/pe_battlenet_agent_http_rest.hexa` — multi-table
  single-module idiom precedent (5 TSVs in one module)

---

## §9 Retired_intentional / sideways-PASS posture (carry forward)

Track CW touches NONE of the surfaces below; they remain
block-acknowledged PASS-equivalent per
`project_retired_intentional_sideways_pass.md`. Future authoring of
CQ-P2 must NOT violate any of these boundaries:

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

CW-specific reinforcements: the TACT/NGDP/CASC parser is a **content
distribution format** observation surface only. It does not perform
account auth, does not forge OAuth bearers, does not bypass DRM
license checks, and never decrypts content-keys it does not legitimately
possess. The `key_kind` enum value `content_key` exists to enumerate the
full TACT key taxonomy honestly; the module never records or persists
real content key bytes.

---

## §10 Open questions

CW addresses **Q2 only** (CQ §5 Q2 resolved: separate module, not
extension). Q3..Q7 carry forward unchanged from Track CQ §5. Q1 was
resolved by Track CT.

CW-specific opens (new):

- **CW-Q1**: Should `encoding_kind` (BLTE per-chunk codec) be a fixed
  enum or free-form string? BLTE encoding codes are documented public-spec
  single-byte tags: N=raw / Z=zlib / 4=lzma / E=encrypted /
  F=frame-recursive. Default: enum (1=raw / 2=zlib / 3=lzma /
  4=encrypted_passthrough / 5=frame_recursive / 6=other) for cross-module
  parity with download_track's `phase_kind` / `cdn_kind` precedent.
- **CW-Q2**: Should `expected_md5_hex` and `computed_md5_hex` be 32-char
  hex strings or empty-when-unverified? Default: `expected_md5_hex` always
  32 (a verify request without an expected hash is meaningless);
  `computed_md5_hex` = empty when not yet verified, 32 when verified —
  invariant `len(expected_md5_hex) == 32`,
  `len(computed_md5_hex) in {0, 32}`. Mirrors CT-Q2 pattern for SHA-256.
- **CW-Q3**: Should `target_subpath` strip any platform-specific prefix at
  record time (root-relative only, no `~/.airgenome/...` or
  `C:\Program Files\...` prefix)? Default: YES (root-relative only;
  G-FakeCdrive layer handles platform translation; recording an absolute
  prefix would couple the module to the deployment environment).
- **CW-Q4**: Should the module enumerate D4 archive_kinds at all (per
  retired_intentional posture)? Default: NO — `game_kind=d4` is in the
  enum for completeness (D4 IS a Battle.net product), but self_test does
  NOT exercise game_kind=d4 rows; only d2r is exercised. This mirrors
  `battlenet_bypass.hexa`'s feasibility matrix (D2R feasible / D4 NOT
  feasible).
- **CW-Q5**: Foreign-key validation — should the CQ-P2 module verify that
  `link_chunk_id`, `blte_id_link`, `key_kind_link_id` are present in the
  linked tables at record time? Default: NO (foreign-key check is deferred
  to a future cross-module orchestrator; CQ-P2 is a sink, not a
  validator). Mirrors CT-Q3.

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
  clone / no fake PASS / no fake real-run completion / no
  validated_manjeom inflation / no Wine cab/wininet code lift ✓
- **own2**: forensic verdict honest unchanged; skeleton-tier vs
  validated-tier distinction preserved; perpetual deferral semantics
  carried; validated_manjeom=0 carries ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 enforce ✓

---

## §12 Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

Track CW is doc-tier audit/inventory/honest-spec for Phase CQ-P2. NO
module authored / NO lib change / NO state mutation / NO
validated_manjeom promotion / NO hexa interp dependency
assumed-resolved.

---

## §13 Predecessors

- Track CV — `state/markers/d4_d3d12_surface_honesty_audit_track_cv_landed.marker` (immediate predecessor; D4 D3D12 surface honesty audit)
- Track CT — `state/markers/battlenet_phase_apple_network_http_tls_sink_track_ct_landed.marker` (sibling phase doc-tier; Phase CQ-P1 audit; format mirror for this CW deliverable)
- Track CQ — `state/markers/battlenet_roadmap_track_cq_landed.marker` (parent roadmap; Track CW executes its Phase CQ-P2 at doc-tier)
- Track CN — `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker` (G-1 perpetual deferral source; primary blocker for any real hexa run of CW-spec'd module)
- Track CO — `state/markers/gamebox_v1_0_4_release_notes_track_co_landed.marker` (v1.0.4 release bundle predecessor)
- Track P — `docs/BATTLENET_BYPASS_OWN1_DISAMBIGUATION.md` (permanent disambiguation reference; sister doc, never superseded)

---

## §14 Next-cycle hooks

- **(NEW Track CW+1 candidate)**: author `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa` against §4 spec; validate paper-mode self_test; CHANGELOG additive entry — produces skeleton-tier landed module per CQ-P2 exit criteria
- **(future, separate)**: doc-tier spec for Phase CQ-P3 (Agent HTTP REST 4-verb PARTIAL augment of existing pe_battlenet_agent_http_rest.hexa)
- **(perpetually deferred)**: G-1 hexa interp closure CM-26..CM-30 (Track CN)
- **(gated, NOT pre-authored)**: Phase CQ-P4 / CQ-P5

CW does NOT pre-author the CQ-P2 module. CW does NOT promise CW+1 timing.
CW does NOT touch CQ-P3..CQ-P5 detail.
