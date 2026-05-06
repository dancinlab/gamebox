# gamebox CQ-P2 Module Authoring — TACT/NGDP/CASC Parser Skeleton (Track DC, 2026-05-06)

> Track DC on gamebox standalone repo, branch `main`. Code-tier authoring
> of `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa` against Track CW
> spec §4. Real-run PASS observed. **Skeleton-tier landing**; **no
> validated_manjeom promotion**.

---

## Context

- Repo: `need-singularity/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track DB CQ-P1 module landing).
- Predecessor (immediate): Track DB.
- Predecessor (semantic, parent roadmap): Track CQ.
- Predecessor (semantic, authoring spec): Track CW.
- Forensic verdict (UNCHANGED at top tier): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

Track DC authors the module Track CW specified. The module is a TSV-persisted
phase tracker for: BLP/manifest fetch → BLTE chunk parse → MD5 verify →
LZMA inflate → install_path write. **Public-spec implementation direction
only**; the actual format-decoder binding (Apple Compression LZMA +
CommonCrypto MD5 + Network.framework HTTPS) remains a separate, post-G-CDN
deliverable.

---

## §1 What DC delivers

| File | Op | Purpose |
|---|---|---|
| `lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa` | NEW | the CQ-P2 module per CW §4 spec |
| `docs/CQ_P2_MODULE_AUTHOR_TACT_NGDP_CASC_TRACK_DC.ai.md` | NEW | this handoff doc |
| `state/markers/cq_p2_module_author_tact_ngdp_casc_track_dc_landed.marker` | NEW | track marker |

`in_place_writes`: 0. NEW files: 3. Pure additive landing.

---

## §2 Module shape (per CW §4)

5 TSV tables under `persist/pe_battlenet_tact_ngdp_casc_track/`, all linked
by `session_id`:

| Table | Columns |
|---|---|
| `manifest.tsv` | session_id, manifest_id, archive_kind, game_kind, region, version_id, entry_count, encoding_table_size, root_hash_kind |
| `blte.tsv` | session_id, blte_id, header_size, chunk_count_in_block, encoding_kind, chunk_index, encoded_size, decoded_size, total_decoded_size, integrity_pass |
| `md5.tsv` | session_id, md5_id, key_kind, expected_md5_hex, computed_md5_hex, match_result, key_kind_link_id |
| `lzma.tsv` | session_id, lzma_id, blte_id_link, input_size, expected_output_size, actual_output_size, inflate_status_code |
| `install.tsv` | session_id, install_id, game_kind, stage_kind, target_subpath, bytes_to_write, bytes_written, write_status_code, link_chunk_id |

5 record functions: `record_manifest`, `record_blte`, `record_md5`,
`record_lzma`, `record_install`. Counters: `manifest_count`, `blte_count`,
`md5_count`, `lzma_count`, `install_count`, `count_distinct_sessions`,
plus dimension-distinct helpers `count_blte_encoding_kinds`,
`count_md5_key_kinds`, `count_install_stage_kinds`,
`count_manifest_archive_kinds`. Enum helpers: `archive_kind_name`,
`encoding_kind_name`, `key_kind_name`, `stage_kind_name`.

### CW-Q1..Q5 resolutions applied at authoring

- **CW-Q1** (encoding_kind enum): 6-value
  (1=raw / 2=zlib / 3=lzma / 4=encrypted / 5=passthrough / 6=frame_recursive)
- **CW-Q2** (md5 hex length): `expected_md5_hex` always 32-char,
  `computed_md5_hex` len ∈ {0, 32}
- **CW-Q3** (target_subpath): root-relative only — leading `/` and
  Windows drive letter (`X:`) both panic-rejected at record time
- **CW-Q4** (D4 archive_kind): `game_kind` enum has both d2r=1 and d4=2
  for completeness; `self_test` exercises **d2r only** (D4 retired_intentional
  per Track P)
- **CW-Q5** (foreign-key validation): NO record-time validation; cross-
  module orchestration is future concern. FKs (`blte_id_link`,
  `key_kind_link_id`, `link_chunk_id`) check only `>= 0`.

### Reserved-keyword adjustment

`match` is a reserved hexa-lang keyword; the column was named
`match_result` in the function signature and panic strings. The TSV
column header in `match_result`'s persisted form is unchanged from CW
spec semantic (column 6 of `md5.tsv`). This rename is the second
reserved-keyword adjustment of the cycle (Track DA renamed `scope` →
`scope_str` in `closure_orchestrator.hexa`).

---

## §3 Real-run observation

```
$ HEXA_SHIM_NO_DARWIN_LANDING=1 .../hexa_interp lib/loader/pe_battlenet_tact_ngdp_casc_track.hexa self-test
  ✅ pe_battlenet_tact_ngdp_casc_track self_test PASS (manifest=4, blte=4, md5=4, lzma=3, install=3, distinct_sid=2, emits=19, sid=ptnct-1778046410)
```

CW expected_self_test_thresholds_paper_mode vs DC observed:

| Target | CW spec | DC observed |
|---|---|---|
| manifest_rows_min | 3 | 4 ✓ |
| blte_rows_min | 4 | 4 ✓ |
| md5_rows_min | 4 | 4 ✓ |
| lzma_rows_min | 3 | 3 ✓ |
| install_rows_min | 3 | 3 ✓ |
| r0_emit_total_min | 18 | 19 ✓ |
| encoding_kinds_covered_min | 3 | 4 ✓ (raw/zlib/lzma/passthrough) |
| key_kinds_covered_min | 2 | 2 ✓ (ekey/ckey) |
| stage_kinds_covered_min | 2 | 3 ✓ (fetch/verify/write) |
| archive_kinds_covered_min | 2 | 4 ✓ (blp_manifest/encoding_table/root/install) |
| session_ids_distinct_min | 2 | 2 ✓ (1001, 1002) |
| integrity_pass_zero_and_one_both_exercised | true | ✓ (blte_id 3 = 0, others = 1) |
| match_zero_and_one_both_exercised | true | ✓ (md5_id 3,4 = 0; md5_id 1,2 = 1) |
| linked_chunk_id_rows_min | 1 | 2 ✓ (install_id 1, 2 link chunk_id=1) |
| linked_blte_id_rows_min | 1 | 2 ✓ (lzma_id 1 → blte 3, lzma_id 2 → blte 2) |
| game_kinds_exercised | ["d2r"] | ["d2r"] ✓ |
| game_kinds_NOT_exercised | ["d4"] | ["d4"] ✓ (D4 retired_intentional) |

All CW spec targets met or exceeded.

---

## §4 own1 / own2 / raw enforcement

### §4.1 own1 §A

| constraint | DC posture |
|---|---|
| Wine 0 | ✓ — no Wine cab/wininet code lift |
| Apple-private SPI 0 | ✓ |
| Real archive bytes recorded 0 | ✓ — only sizes (encoded/decoded) and integrity_pass |
| Real content key bytes 0 | ✓ — only `key_kind` enum + 32-char hex (length-only validated) |
| Real BLP pixel data 0 | ✓ |
| Real LZMA decoded payload 0 | ✓ — only sizes and inflate_status_code |
| GPTk / Whisky / CrossOver 0 | ✓ |
| DRM bypass 0 | ✓ |
| Anti-cheat bypass 0 | ✓ |
| hexa pure | ✓ |

### §4.2 own2 carry

| constraint | DC posture |
|---|---|
| honest_emit | ✓ — module documented as "instrumentation sink"; format-decoder binding is separate |
| validated_manjeom=0 explicit | ✓ |
| skeleton-tier vs validated-tier distinction preserved | ✓ |
| no progress claims, no inflation | ✓ — DC does not claim real CDN bytes, real LZMA inflate, real MD5 verify of real content |
| retired_intentional carry (D4) | ✓ — game_kind enum includes D4 for completeness; self_test does NOT exercise D4 |

### §4.3 raw

raw#9 / raw#11 / raw#12 / raw#15 / raw#175 / raw#270 / raw#271 — all enforce.

### §4.4 BR_NO_USER_VERBATIM

User directive paraphrased throughout. No verbatim Korean directive quoted.

---

## §5 Aggregate posture after DC

- Battle.net loader cluster module count: **11** (was 10 post-DB, was 9 pre-DB)
- skeleton-tier coverage: 100%
- real-execution-confirmed sub-tier: 11 of 11
- validated_manjeom: 0
- top-tier forensic verdict: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` carries unchanged

CQ-P4 closure: ready for full closure under Track DD (re-run all 11
modules + emit fresh closure_progress marker).

---

## §6 Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: per §4.1 ✓
- **own2**: per §4.2 ✓
- **raw**: 9 / 11 / 12 / 15 / 175 / 270 / 271 ✓

---

## §7 Predecessors

- Track DB — `state/markers/cq_p1_module_author_apple_network_http_track_db_landed.marker`
- Track DA — `state/markers/cq_p4_partial_closure_real_hexa_run_track_da_landed.marker`
- Track CW — `state/markers/battlenet_phase_tact_ngdp_casc_parser_track_cw_landed.marker` (authoring spec consumed)
- Track CQ — parent roadmap

---

## §8 Next-cycle hooks

- **(authorable now, immediate)**: Track DD — full CQ-P4 closure: re-run
  all 11 modules + emit fresh closure_progress marker, formally close CQ-P4
- **(after DD)**: per-surface CQ-P5 flips
- **(carry forward)**: retired_intentional / honest_freeze postures

---

*Track DC closure recorded 2026-05-06. CQ-P2 module landed real-run PASS.
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 carries unchanged. own1 /
own2 / raw 9·11·12·15·175·270·271 enforce.*
