# airgenome-gamebox — Track AQ perf c_pe_* extension cycle landed (2026-05-04)

## §1 paraphrase

BG subagent (Track AQ, c_pe_* PE-loader perf module extension cycle) extended 6
c_pe_*.hexa performance modules with synthetic state machine round-trip rows
mirroring the Track G/U/AI/AK predecessor extension pattern, but using a
PE-loader-domain emit prefix convention (`__PE_<DOMAIN>__ PARTIAL <stage>`).
The 10-module c_pe_* family is the PE loader perf surface — directly relevant
to D2R cond.1 (PE parse + IAT real-resolve + mainCRTStartup reach) and D4
cond.1 (similar PE parse path). Prior to Track AQ, none of the c_pe_* modules
had been extended in this session; Track AQ extends 6 representative modules
covering: IAT walk (`c_pe_iat_simulation`), relocation apply
(`c_pe_relocation_apply_track`), runtime layout / section map
(`c_pe_runtime_layout_track`), section permission audit
(`c_pe_section_perm_audit`), TLS callback walk (`c_pe_tls_callback_track`),
and segment overlay / packer detection (`c_pe_segment_overlay_predict`).

`.roadmap.diablo2_resurrected` cond.1 evidence array augmented from 32 to 35
rows (additive +3 — three rows aggregating the 6-module Track AQ landing).
cond.1 status remains `partial` (unchanged). cond.2 (met, Track A territory)
and cond.3 (partial, Track G/AP territory) are explicitly preserved verbatim.
blocker_reason refreshed to acknowledge the Track AQ 6-module c_pe_*
cumulative synthetic state machine round-trip extension while reaffirming
that production smoke remains gated on real D2R.exe binary execution
(Battle.net launcher install dependent — permanent external dependency).

posture = evidence_augment_partial_to_partial (NOT met promotion). cond.1
production smoke requires the real Battle.net-installed D2R.exe binary, an
external dependency permanently outside own1 boundary. Cumulative c_pe_*
synthetic stage emit count after Track AQ: 12 PARTIAL stage emits (6 modules
× 2 stages each). Cumulative c_pe_* perf-module extension count post-Track
AQ: 6/10. Track AQ runs in parallel with Track AL/AM/AN/AO/AP and respects
the file-scope partition (no c_d2r_* / c_d4_* / c_metal_* / c_purple_* /
c_bnet_* / c_lineage_* / lib/loader / tool / native / tests touches).

## §2 cycle scope (additive_only)

| Concern               | Value                                           |
|-----------------------|-------------------------------------------------|
| migration             | forbidden                                       |
| changes               | additive_only                                   |
| destructive_ops       | 0                                               |
| cost_usd              | 0                                               |
| substrate             | mac-local                                       |
| cap_minutes           | 90                                              |
| BR_NO_USER_VERBATIM   | true (no verbatim user quote in any artifact)   |
| friendly_preset       | true                                            |
| silent_land           | true (marker pin only, no chat surface)         |
| own1                  | Wine 0 / hexa-only / no real PE binary exec     |
| own2                  | PARTIAL emit explicit, PASS swallow forbidden   |

## §3 modules extended (6 / 10 c_pe_* perf modules; cumulative 6/10)

| # | module path                                       | LOC before | LOC after | delta | emit count | stages                                      |
|---|---------------------------------------------------|-----------:|----------:|------:|-----------:|---------------------------------------------|
| 1 | lib/perf/c_pe_iat_simulation.hexa                 | 212        | 253       | +41   | 2          | iat_walk_phase, resolution_distribution     |
| 2 | lib/perf/c_pe_relocation_apply_track.hexa         | 135        | 172       | +37   | 2          | reloc_walk_phase, type_distribution         |
| 3 | lib/perf/c_pe_runtime_layout_track.hexa           | 123        | 160       | +37   | 2          | section_map_phase, va_distribution          |
| 4 | lib/perf/c_pe_section_perm_audit.hexa             | 129        | 167       | +38   | 2          | perm_audit_phase, policy_distribution       |
| 5 | lib/perf/c_pe_tls_callback_track.hexa             | 144        | 182       | +38   | 2          | tls_walk_phase, suspicion_distribution      |
| 6 | lib/perf/c_pe_segment_overlay_predict.hexa        | 147        | 186       | +39   | 2          | overlay_scan_phase, entropy_distribution    |

Total LOC delta: +230 (6 modules × ≈37-41 LOC each, all in 25-50 budget).
Total stage emit: 12 `__PE_<DOMAIN>__ PARTIAL <stage>` println markers + 12
`r0_emit` JSONL events (one per stage, 2 stages per module).

Each appended block follows the Track G/U/AI/AK predecessor pattern:

- Inserted between the existing `r0_session_count(sid) >= N` emit-count
  assertion and the `cleanup_*()` call inside the existing `self_test()`
  function body.
- Two stage-emit pairs per module: one `r0_emit("c_pe_<module>_synthetic_<stage>", …)`
  + one `println("__PE_<DOMAIN>__ PARTIAL <stage>")`.
- Synthetic offsets allocated in a c_pe_* dedicated band 0x140110000 →
  0x14011C000 in 0x2000 increments (iat_simulation 0x140110000,
  relocation_apply_track 0x140112000, runtime_layout_track 0x140114000,
  section_perm_audit 0x140116000, tls_callback_track 0x140118000 +
  callback_array 0x14011A000, segment_overlay_predict 0x14011C000). The
  band is intentionally above Track AK's 0x1400A2000 ceiling and well
  above the 0x14008C000-0x140096000 Track AI band — no collision with
  any prior c_d2r_* / c_metal_* offset allocation. All offsets are
  fictitious scaffold pointers; they do **not** correspond to real PE
  loader RVA values.
- Each emit JSONL payload includes `verdict:"PARTIAL"`,
  `posture:"evidence_augment_partial_to_partial"`, and
  `real_binary_gap:"permanent — Battle.net launcher install dependent"`.
- The emit prefix convention deliberately differs from the Track U/AI/AK
  `__D2R_<DOMAIN>__` style: c_pe_* modules are PE-loader-domain (not D2R
  game-domain), so `__PE_IAT__`, `__PE_RELOC__`, `__PE_LAYOUT__`,
  `__PE_PERM__`, `__PE_TLS__`, `__PE_OVERLAY__` prefixes are used. This
  matches each module's existing per-domain emit conventions
  (e.g. `pe_reloc_apply` / `pe_section_map` / `pe_tls_suspect` are the
  pre-existing event-type stems).

## §4 sha256 pin

| path                                                  | sha256                                                             |
|-------------------------------------------------------|--------------------------------------------------------------------|
| lib/perf/c_pe_iat_simulation.hexa                     | `6cb9a102c93ec1b293a322f32b3264f0620c7484b7de6f005705b0229aaf3678` |
| lib/perf/c_pe_relocation_apply_track.hexa             | `4118c81f888a3c8cc13b93cc6bca42b39ca33ea047a3c56f72979ccd4eb8ba7f` |
| lib/perf/c_pe_runtime_layout_track.hexa               | `b7462ec2466c5627fbec132c6e6147d4beff1c3d0e985feaea821a2816e45328` |
| lib/perf/c_pe_section_perm_audit.hexa                 | `67f48db0574e6591abc20a3038bfb170959d2bc82a8c45e257d565f3bfa73d6a` |
| lib/perf/c_pe_tls_callback_track.hexa                 | `e184c9e17ca79d9c3ba43ead1c2a8523770b0d6bf5cc2b2506b3b85b9f1280ab` |
| lib/perf/c_pe_segment_overlay_predict.hexa            | `d6e16699e319307237f291a92e1b66cdee1aaa8d4d123789a749211114697aad` |
| .roadmap.diablo2_resurrected                          | `ddfb5ab5563d7cc70d631031113b743a9876c399255bc4311f933b66f65180b8` |

## §5 cond evidence summary

| cond id                    | status before | status after | evidence before | evidence after | delta |
|----------------------------|---------------|--------------|----------------:|---------------:|------:|
| diablo2_resurrected.cond.1 | partial       | partial      | 32              | 35             | +3    |
| diablo2_resurrected.cond.2 | met           | met          | 9               | 9              | 0     |
| diablo2_resurrected.cond.3 | partial       | partial      | 16              | 16             | 0     |

cond.2 is byte-identical (Track A territory untouched). cond.3 is byte-identical
relative to its in-flight Track AP state at Track AQ landing time (Track AP
parallel territory, c_metal_* augment cycle, untouched by Track AQ).

The +3 evidence rows were chosen to aggregate the 6-module Track AQ landing
into a digestible cond.1 narrative — one row per pair-of-domains (IAT walk +
relocation + runtime layout) + one row per pair-of-domains (section perm
audit + TLS callback) + one row for the segment overlay scaffold. Each row
explicitly cites the module path, emit prefix, stage names, and synthetic
payload offsets so any future audit can reconstruct the full per-module
extension scope from the roadmap alone.

## §6 caveats (≥6 inline; this cycle delivers 14)

- **C1.** cond.1 measurement boundary: synthetic state machine round-trip is
  evidence-only. The cond.1 verifier requires real D2R.exe production smoke
  (`__D2R_LAUNCH__ PASS <stage>` from `tests/test_closure.hexa` against the
  real Battle.net-installed binary). No synthetic, opensource, or hexa-native
  scaffold can fulfill this — D2R.exe IS the proprietary product, no
  equivalent exists. Therefore cond.1 status `partial` is permanent and
  Track AQ does **NOT** promote cond.1 to met.
- **C2.** evidence_augment_partial_to_partial vs met_promotion (Track A
  precedent): Track A cond.2 met promotion was possible because synthetic +
  opensource MPQ-listfile byte-equal round-trip is functionally equivalent
  to real production parsing. cond.1 has no such opensource D2R.exe
  equivalent, so the posture is strictly evidence augmentation, not met
  promotion. Identical posture to Track G cond.3 / Track U+AI+AK cond.1.
- **C3.** module selection rationale: Track AQ selected 6 of 10 c_pe_*
  modules covering the canonical PE-loader perf surface — IAT walk
  (`c_pe_iat_simulation`), relocation apply (`c_pe_relocation_apply_track`),
  runtime layout (`c_pe_runtime_layout_track`), section perm audit
  (`c_pe_section_perm_audit`), TLS callback (`c_pe_tls_callback_track`),
  and segment overlay (`c_pe_segment_overlay_predict`). The 4 remaining
  untouched c_pe_* modules (`c_pe_imports_growth_chart`,
  `c_pe_install_coverage`, `c_pe_parse_cache`, `c_pe_reloc_optimize`) are
  available for a future Track that would reach 10/10 c_pe_* coverage.
- **C4.** synthetic offset allocation: Track AQ uses 0x140110000 →
  0x14011C000 in 0x2000 increments, all above Track AI/AK c_d2r_* band
  ceiling (0x1400A2000) and entirely separate from Track AP c_metal_*
  band (0x180000000+). None of these offsets correspond to real loader
  RVA values; they are fictitious scaffold pointers selected only for
  in-emit-payload identity tagging. Real PE-loader RVA resolution is gated
  on the same Battle.net launcher install boundary as cond.1.
- **C5.** in-place writes count: 7 (6 c_pe_*.hexa modules `self_test()`
  append + 1 surgical JSONL field edit on `.roadmap.diablo2_resurrected`
  cond.1 evidence array + blocker_reason value refresh + new
  `ai_native_handoff_cond_1_perf_pe_extension_track_aq` field). 0 cond.N
  rows removed, 0 cond.2/cond.3 modifications, append/extend only —
  additive_only spirit preserved (matches Track AI/AK predecessor
  in_place_writes=7).
- **C6.** own1 enforcement: every appended block uses hexa-native `r0_emit`
  + `println` only. Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 / community
  wrapper 0 / DRM bypass 0 / PE binary execution 0 / Battle.net launcher
  invocation 0. All synthetic byte-array offsets are inline literals — no
  real binary RVA lookup, no real `pe_imports.hexa` exec, no real `awk`
  per-PE tally on a real loader trace.
- **C7.** own2 enforcement: every PARTIAL emit is explicit
  (`verdict:"PARTIAL"`, `__PE_<DOMAIN>__ PARTIAL <stage>`). PASS swallow
  is absolutely forbidden — cond.1 status is honestly reported as
  `partial` in the blocker_reason value refresh.
- **C8.** raw 9 / 11 / 12 / 15 / 175 compliance: all field names
  snake_case (raw 11), all paths repo-relative (raw 15), 0 verbatim user
  quotes (raw 175), PARTIAL emit explicit (raw 12), hexa-only emit
  (raw 9). 14 caveats inline (raw 10).
- **C9.** test harness deferral: no `tests/test_pe_perf_synthetic_state_machine_round_trip_track_aq.hexa`
  was created in this cycle — the brief explicitly bans tool/, native/,
  tests/ writes. A future Track could add a single harness driving all 6
  Track AQ modules + the 4 remaining c_pe_* modules together.
- **C10.** parallel BG territory scope ban: Track AQ touches **only**
  lib/perf/c_pe_*.hexa + .roadmap.diablo2_resurrected (cond.1) + handoff
  doc + marker. ANY c_d2r_* / c_d4_* / c_metal_* / c_purple_* / c_bnet_* /
  c_lineage_* untouched (Track AI/AJ/AK/AL/AP/G/U/V/B/J/N territory). ANY
  lib/loader untouched (Track AM parallel). ANY tool/, native/, tests/
  untouched. ANY .roadmap OTHER THAN .diablo2_resurrected cond.1 untouched.
  ANY existing docs/*.md read-only. `docs/HEXA_UPSTREAM.md` explicitly
  unmodified (Track AO concurrent territory — race condition avoided per
  brief directive).
- **C11.** cumulative state after Track AQ: 6/10 c_pe_* modules extended.
  4 modules remain (`c_pe_imports_growth_chart`, `c_pe_install_coverage`,
  `c_pe_parse_cache`, `c_pe_reloc_optimize`). Cumulative c_pe_* synthetic
  stage emit count: 12 PARTIAL stages = 6 modules × 2 stages each.
- **C12.** evidence count progression: 14 (Track D land) → 20 (Track U
  +6) → 26 (Track AI +6) → 32 (Track AK +6) → 35 (Track AQ +3). The
  Track AQ +3 row count differs from prior +6 cycles because Track AQ
  aggregates 6 module-landings into 3 cond.1 evidence rows (two pair-of-
  domain rows + one segment-overlay row); per-module granularity is
  preserved at the marker / handoff level, while cond.1 evidence stays
  digestible.
- **C13.** retired_intentional posture is **NOT** applicable to cond.1:
  cond.1 is not block-acknowledged dead-end (D2R production smoke is
  technically achievable via Battle.net launcher install), it is
  permanently `partial` because the launcher install is outside own1
  boundary. retired_intentional candidate status remains `none`.
- **C14.** D4 cond.1 cross-link not yet exercised: c_pe_* modules cover
  PE-loader perf surface that is also relevant to D4 cond.1 (similar PE
  parse path). Track AQ touches only `.roadmap.diablo2_resurrected`
  cond.1 per file-scope brief; a future cycle could augment
  `.roadmap.diablo4` cond.1 with the same Track AQ landing as
  cross-reference (the modules themselves do not need re-extension —
  they are game-agnostic PE-loader perf trackers).

## §7 verifier emit log samples (2026-05-04)

```
__PE_IAT__ PARTIAL iat_walk_phase
__PE_IAT__ PARTIAL resolution_distribution
__PE_RELOC__ PARTIAL reloc_walk_phase
__PE_RELOC__ PARTIAL type_distribution
__PE_LAYOUT__ PARTIAL section_map_phase
__PE_LAYOUT__ PARTIAL va_distribution
__PE_PERM__ PARTIAL perm_audit_phase
__PE_PERM__ PARTIAL policy_distribution
__PE_TLS__ PARTIAL tls_walk_phase
__PE_TLS__ PARTIAL suspicion_distribution
__PE_OVERLAY__ PARTIAL overlay_scan_phase
__PE_OVERLAY__ PARTIAL entropy_distribution
```

12 println markers + 12 `r0_emit` JSONL events (one pair per stage, 2
stages per module, 6 modules).

## §8 next-cycle actions

- Track AR-pe-final — extend remaining 4 untouched c_pe_* modules
  (`c_pe_imports_growth_chart`, `c_pe_install_coverage`,
  `c_pe_parse_cache`, `c_pe_reloc_optimize`) — would reach 10/10 c_pe_*
  coverage.
- Track AS-d4-cross-link — augment `.roadmap.diablo4` cond.1 with the
  same Track AQ c_pe_* landing as cross-reference (D4 cond.1 PE parse
  path direct relevance).
- Track AT — single test harness `tests/test_pe_perf_synthetic_state_machine_round_trip.hexa`
  driving all 6 Track AQ modules + the 4 remaining c_pe_* modules
  together — currently deferred per this-cycle scope ban on tests/.
- production smoke remains gated on user-side Battle.net install of real
  D2R.exe (permanent external dependency; not Track-AZ-resolvable from
  within own1).
- diablo2_resurrected.cond.1 real production binary smoke (Battle.net
  launcher install resolve transitive — no synthetic-to-real shortcut
  available).
- own2 enforcement entry-too-large + headless first_dialog 매칭 측 2
  fail resolve cycle (pre-existing inherited, this cycle 0 new fail).
- HEXA_UPSTREAM.md retrofit cycle (after Track AO concurrent settles)
  to aggregate any per-cycle hexa-lang upstream files; Track AQ did not
  produce a per-cycle upstream file as no clear hexa-lang stdlib /
  builtin / wraround patterns surfaced during this c_pe_* extension.
