# airgenome-gamebox — Track AR perf c_oauth_* + c_install_* extension landing (2026-05-04)

## §1. Mission paraphrase
Track AR cycle: extend OAuth + Install perf families (lib/perf/c_oauth_*.hexa family 4 + lib/perf/c_install_*.hexa family 6 — none extended in any prior cycle per Track T inventory). Pick all 4 c_oauth_* + 4 c_install_* (8 modules total) and append synthetic state machine round-trip rows mirroring Track N predecessor pattern (`__OAUTH_<DOMAIN>__ PARTIAL <state>` + `__INSTALL_<DOMAIN>__ PARTIAL <state>` emit prefix style). 35 PARTIAL emit stage 추가 across 8 modules. .roadmap.battlenet cond.3 evidence array augment (12 -> 16, additive +4 rows referencing Track AR oauth+install scaffold) + blocker_reason refresh + ai_native_handoff_cond_3_oauth_install_extension_track_ar field add. cond.3 status `partial` 영구 유지 — production smoke gated on user network smoke (blk.1 transitive permanent external gate). cond.1 (met, Track A 8 evidence rows) + cond.2 (met, Track A 8 evidence rows) untouched. blk.1 untouched.

## §2. Modules extended (8 = 4 oauth + 4 install)

| # | path | before LOC | after LOC | delta | emit prefix | states |
|---|------|-----------|-----------|-------|-------------|--------|
| 1 | lib/perf/c_oauth_pkce_state.hexa             | 195 | 232 | +37 | `__OAUTH_PKCE__`         | VERIFIER_GEN / CHALLENGE_DERIVE / AUTH_REQUEST / TOKEN_EXCHANGE (4) |
| 2 | lib/perf/c_oauth_device_flow.hexa            | 201 | 238 | +37 | `__OAUTH_DEVICE__`       | USER_CODE_ISSUE / USER_VERIFY_PENDING / POLL_APPROVAL / TOKEN_DELIVERED (4) |
| 3 | lib/perf/c_oauth_flow_proxy.hexa             | 178 | 219 | +41 | `__OAUTH_PROXY__`        | AUTH_URL_BUILD / REDIRECT_RECV / CODE_EXCHANGE / REFRESH_FLOW / REVOKE (5) |
| 4 | lib/perf/c_oauth_redirect_capture_track.hexa | 122 | 162 | +40 | `__OAUTH_REDIRECT__`     | STATE_GENERATE / REDIRECT_AWAIT / STATE_VERIFY / CODE_CAPTURE / CSRF_REJECT (5) |
| 5 | lib/perf/c_install_path_detect.hexa          | 240 | 277 | +37 | `__INSTALL_PATH_DETECT__`| CANDIDATE_ENUMERATE / SIZE_PROBE / BEST_PATH_PICK / REGISTER (4) |
| 6 | lib/perf/c_install_path_resolve_track.hexa   | 111 | 147 | +36 | `__INSTALL_PATH_RESOLVE__`| ENV_CAPTURE / TEMPLATE_SUBST / PATH_VALIDATE / MISSING_FALLBACK (4) |
| 7 | lib/perf/c_install_progress_bar.hexa         | 133 | 173 | +40 | `__INSTALL_PROGRESS__`   | PE_PARSE / IAT_BIND / DOWNLOAD / EXTRACT / REGISTER (5) |
| 8 | lib/perf/c_install_signature_verify.hexa     | 147 | 183 | +36 | `__INSTALL_SIGVERIFY__`  | PUBLISHER_LOOKUP / THUMBPRINT_MATCH / CHAIN_VALIDATE / TRUST_VERDICT (4) |

Cumulative: c_oauth_* family 4/4 extended (full coverage). c_install_* family 4/6 extended (untouched 2: c_install_disk_space_check, c_install_progress_eta — next-cycle candidates). Total 35 PARTIAL state rows added (oauth 18 + install 17), +304 LOC across 8 modules.

## §3. cond.3 evidence augment (battlenet OAuth 2-leg + setup install Phase 1 cross-link)
- **before**: 12 evidence rows (Track J Warden honest freeze + Track L D2R realm protocol freeze + Track N 4-module bnet synthetic round-trip 보존).
- **after**: 16 evidence rows (Track AR +4 additive: oauth pkce/device row + oauth flow_proxy/redirect row + install path_detect/resolve cross-link row + install progress/signature cross-link row).
- **status**: partial -> partial (NOT met promotion, evidence_augment_partial_to_partial posture).
- **blocker_reason refresh**: acknowledges Track AR 8-module c_oauth_*+c_install_* synthetic state machine round-trip (35 PARTIAL emit stage 누적). Track N 4-module bnet round-trip 보존 explicit. Production smoke + 실 network endpoint smoke gated on blk.1 transitive permanent gate.
- **handoff field added**: `ai_native_handoff_cond_3_oauth_install_extension_track_ar`.
- **untouched**: cond.1 (met, Track A 8 evidence rows) verbatim, cond.2 (met, Track A 8 evidence rows) verbatim, blockers array (battlenet.blk.1 open) verbatim, cross_link verbatim, header line 1-4 comments verbatim.

## §4. Cross-link to Battle.net cond.2 setup install + cond.3 OAuth flows
c_oauth_* family supports Battle.net cond.3 OAuth 2-leg flow:
- PKCE state (RFC 7636) — modern launcher OAuth public client (S256 challenge derivation).
- Device flow (RFC 8628) — TV/CLI device authorization grant (Battle.net Console / Steam Big Picture).
- Flow proxy — full OAuth lifecycle (auth URL → redirect → code exchange → refresh → revoke).
- Redirect capture — loopback redirect URI listener + state CSRF validation.

c_install_* family cross-links Battle.net cond.2 setup install Phase 1 path:
- Path detect — Battle.net.app + game.exe install path candidate scan + size probe.
- Path resolve — %PROGRAMFILES%/%LOCALAPPDATA%/%PROGRAMDATA%/%APPDATA% env substitution for Battle.net Setup install layout.
- Progress bar — PE_PARSE→IAT_BIND→DOWNLOAD→EXTRACT→REGISTER weighted phases for Battle.net-Setup.exe install staging.
- Signature verify — Authenticode publisher/thumbprint trust verdict for Battle.net installer integrity.

The c_install_* extensions are cond.2-territory artifacts referenced under cond.3 evidence cross-link (cond.2 met status preserved via Track A; this cycle augments cond.3 evidence only without modifying cond.2 evidence array).

## §5. Policy compliance
- `migration` forbidden / `additive_only` / `destructive_ops` 0 / `cost_usd` 0 / `substrate` mac-local / `cap_minutes` 90.
- `BR_NO_USER_VERBATIM`: 0 verbatim user quotes in any artifact (handoff §1 paraphrase only, marker user_directive_paraphrase paraphrase only).
- own1: Wine 0 / hexa-only / NO real OAuth endpoint binding / NO real installer launch / NO real PE binary parse / NO real Authenticode chain build / NO real X.509 OCSP/CRL probe / NO real device polling HTTP / NO real loopback bind / NO real disk write / synthetic round-trip ONLY (35 PARTIAL emit stage = 8 modules × 4-5 transition).
- own2: 35 PARTIAL emit explicit (35 println markers + 35 r0_emit round-trip events with verdict PARTIAL + posture evidence_augment_partial_to_partial + 8 round-trip matrix summary emits = 43 total new emits). PASS swallow forbidden (cond.3 status partial 영구 유지).

## §6. Caveats (10)
- **C1**: cond.3 met 승격 영구 X — production smoke + 실 us.battle.net OAuth2 endpoint + 실 token validate + 실 device verify + 실 redirect listener round-trip 측 모두 network-dependent (blk.1 transitive permanent gate). Track AR extension is evidence_augment only, NOT met promotion path.
- **C2**: c_oauth_*/c_install_* synthetic state machine payloads (verifier_len_simulated=43, poll_interval_simulated_sec=5, token_lifetime_simulated_sec=3600, candidate_count_simulated=3, env_var_count_simulated=4, weight_pct_simulated=10/65/20/5, publisher_count_simulated=5, state_len_simulated=32) are inline literal scaffolds, NOT real RFC 7636/8628 protocol values from real OAuth flows. Distinct from c_bnet_* (Track N) state machine payloads (token_lifetime_sec=3600 / msg_count / refresh_attempt_count) — Track AR scaffold values are fictitious test corpus.
- **C3**: 4/4 c_oauth_* extended (full family coverage). 4/6 c_install_* extended; untouched 2/6: c_install_disk_space_check + c_install_progress_eta. These remain skeleton-tier per Track T inventory and are next-cycle candidates.
- **C4**: append goes between emit-count assertion and `cleanup_*()` gate (every module's self_test) — additive_only spirit preserved (no row removal, no self_test body modification, no main() change, no fn signature change). Per-module delta range 36-41 LOC; total +304 LOC across 8 modules.
- **C5**: hexa-lang upstream candidates: 0 hits discovered in this cycle. Each module already uses hexa-native primitives (r0_emit, exec, write_file, panic, to_string, to_int, len, println, sha256 via shell awk pipeline). No OAuth/install API hand-binding gap surfaced (synthetic emit only, no real OAuth2 client lib / WinVerifyTrust / Authenticode parse / device flow polling primitive needed). docs/hexa_upstream_track_ar_*.md NOT created (per user 2026-05-04 directive — no entry if no hits).
- **C6**: in_place_writes=9 (8 c_oauth_*+c_install_*.hexa + 1 .roadmap.battlenet JSONL header surgical edit on cond.3 only) — Track AP predecessor (in_place_writes=7) 동급 posture. additive_only spirit preserved, 0 row removal across all artifacts.
- **C7**: Track AR territory ban: c_d2r_* / c_d4_* / c_metal_* / c_pe_* / c_purple_* / c_bnet_* / c_lineage_* / c_elden_* / c_cs2_* / loader / tool / native / tests / .roadmap (other than .battlenet cond.3) untouched. Parallel BG (AM/AP/AQ/AS/AT/AU) territory respected.
- **C8**: docs/HEXA_UPSTREAM.md NOT modified (Track AO completed but per-cycle isolation policy maintained — race condition guard).
- **C9**: cumulative_c_oauth_extended_after_track_ar = 4/4. cumulative_c_install_extended_after_track_ar = 4/6. Earlier all 4+6 = 10 modules were skeleton-tier (none extended in any prior cycle per Track T inventory). Track AR is the FIRST extension cycle for both families.
- **C10**: emit_count assertion threshold unchanged in each module (6, 5, 4, 4, 3, 4, 7, 6 respectively) — append occurs after assertion, so extra 5+5+6+6+5+5+6+5 = 43 r0_emit events (35 state emits + 8 matrix summary emits) per module are bonus (actual counts will exceed thresholds). Existing self_test body verification 0 line modify; only println success message updated to mention `+N synthetic ... PARTIAL transitions` count.

## §7. Predecessors
- `lib/perf/c_bnet_login_state_track.hexa` (Track N) — schema parent (synthetic state machine round-trip pattern + `__BNET_LOGIN__ PARTIAL <state>` emit prefix style precedent for `__OAUTH_<DOMAIN>__` / `__INSTALL_<DOMAIN>__`).
- `lib/perf/c_bnet_token_refresh_track.hexa` (Track N) — pattern reference (5-state machine round-trip with backoff_ms_simulated parallel to oauth_flow_proxy 5-state).
- `lib/perf/c_bnet_chat_state.hexa` (Track N) — pattern reference (4-state machine round-trip parallel to oauth_pkce/oauth_device 4-state).
- `state/markers/airgenome_gamebox_perf_metal_extension_track_ap_landed.marker` (Track AP) — closest in-cycle precedent (additive only, modules_extended_count=6, evidence augment partial->partial, in_place_writes=7).
- `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` (Track P) — bg_agb_rank_b_c_unblock_predecessor (perf_oauth + perf_install candidate consumed by Track AR — first extension cycle).

## §8. Next-cycle actions
- Track AS (or follow-up) — extend remaining 2 untouched c_install_* modules (disk_space_check / progress_eta) to reach 6/6 cumulative.
- Track AT — D4 cond.2 (always-online) cross-game roadmap evidence augment cross-link to Track AR Battle.net OAuth scaffold (D4 always-online required, OAuth + install dependency transitive).
- Track AU — WoW retail/classic OAuth 2-leg + install path cross-link from Track AR.
- production smoke remains gated on blk.1 transitive (network-dependent permanent external gate; not Track-AZ-resolvable from within own1).
- battlenet.cond.3 real us.battle.net OAuth2 endpoint + real Battle.net-Setup.exe install + real Authenticode chain build (network + binary licence resolve transitive — no synthetic-to-real shortcut available).
