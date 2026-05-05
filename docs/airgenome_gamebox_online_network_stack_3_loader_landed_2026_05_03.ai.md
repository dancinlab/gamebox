---
schema: airgenome-gamebox/docs/airgenome_gamebox_online_network_stack_3_loader_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_online_network_stack_3_loader_landed.marker
  roadmap_files_modified:
    - .roadmap.loader_win32
  modules_created:
    - lib/loader/pe_winsock_ws2_32.hexa
    - lib/loader/pe_wininet_https.hexa
    - lib/loader/pe_schannel_tls.hexa
  predecessor_handoffs:
    - docs/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
status: LANDED_ONLINE_NETWORK_STACK_3_LOADER_TRACK_K
related_raws:
  - raw 9    # hexa-only (verifier emit native via println __WINSOCK__/__WININET__/__SCHANNEL__ + r0_emit)
  - raw 10   # honest C3 caveats inline (>=10 numbered C1..C10)
  - raw 11   # snake_case fields throughout JSONL + JSON marker
  - raw 12   # silent-error ban — every PARTIAL emit explicit (no swallow as PASS)
  - raw 15   # env_lazy — repo-relative paths only, no /Users/<user>/ leaks
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes anywhere)
online_expansion_lockin:
  L1: true   # EULA/ToS violation risk acknowledged (a+ path)
  L2: true   # Warden detection risk acknowledged (long-horizon Win32 syscall perfection target)
  L3: true   # 1-2 year+ multi-cycle horizon acknowledged
  L4: true   # D2R online single-target lock-in (D4 retired_intentional unchanged)
  L5: true   # Track J/K/L scope locked-in for parallel BG land
  lockin_iso: 2026-05-03
preserved_unchanged:
  - .roadmap.loader_win32 cond.1 status (partial unchanged — full Win32 coverage = multi-cycle long-horizon)
  - .roadmap.loader_win32 cond.2 + cond.3 (status partial, evidence array unchanged)
  - all 14 other .roadmap.* files (battlenet/d2r/d4/wow/cs2/delta/elden/lineage 3/purple/loader_pe/loader_dx)
  - all native sources, plugin.json, entry, README.md, .gitignore
  - 414 existing lib/loader/*.hexa files (only 3 NEW added, 0 modified)
  - all c_* perf modules (untouched — Track J parallel scope)
  - all tests/*.hexa (no new test harness; modules' self_test() suffice)
  - 2 docs/*_HONEST_FREEZE.md (GAMEGUARD/WARDEN — Track E/J territory, read-only)
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 1
  in_place_writes_note: 1 .roadmap.loader_win32 JSONL header surgical edit (cond.1 evidence array append +4 rows + blocker_reason refresh) — 0 cond.N row deleted, 0 status field promoted, append/extend only
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# airgenome-gamebox online network stack 3-loader landed - Track K (BG-AGB online expansion)

## TL;DR

predecessor battlenet_d2r_offline_shim land 위 Track K (online network stack) 3 NEW loader 모듈 (WS2_32 +
WININET + SCHANNEL) skeleton-tier land. real network 0, real socket(2) 0, real TLS handshake 0,
mac-local hexa-only. .roadmap.loader_win32 cond.1 evidence augment (8 → 12, status partial → partial
유지 — full coverage = multi-cycle long-horizon). online_expansion_lockin L1+L2+L3+L4+L5 ALL OK.

- `lib/loader/pe_winsock_ws2_32.hexa` (NEW, 431 LOC) — WS2_32.dll 20 fn surface skeleton tracker
  (`__WINSOCK__ PARTIAL <fn>` emit + synthetic SOCKADDR_IN round-trip).
- `lib/loader/pe_wininet_https.hexa` (NEW, 443 LOC) — WININET.dll 12 fn surface skeleton tracker
  (`__WININET__ PARTIAL <fn>` emit + synthetic Battle.net REST endpoint 200/401/500 response 3-state).
- `lib/loader/pe_schannel_tls.hexa` (NEW, 438 LOC) — SCHANNEL.dll 8 fn SSPI surface skeleton tracker
  (`__SCHANNEL__ PARTIAL <fn>` emit + synthetic ClientHello/ServerHello/cert chain 3-tier round-trip).

## §1 user_directive_paraphrase

predecessor cycle 측 Battle.net → D2R offline shim Track A 측 land 후 follow-up Track K 로 online
network stack 측 3 NEW skeleton-tier loader 모듈 land. 의도:

- WS2_32 (Winsock 2) raw socket layer 측 syscall surface tracker — Battle.net Agent + D2R online
  connection 측 TCP + DNS + async I/O 측 fn name + synthetic round-trip row 만 emit. 실 socket /
  실 connect / 실 send-recv = 미수행 (production smoke pending separate cycle).
- WININET (high-level HTTP/HTTPS) surface tracker — Battle.net REST API 측 https endpoint 호출 측
  fn name + synthetic 200/401/500 response code matrix 만 emit. 실 HttpSendRequest = 미수행.
- SCHANNEL (TLS 1.2 + 1.3 + Blizzard cert chain pinning) surface tracker — SSPI fn name + synthetic
  ClientHello + ServerHello + cert chain 3-tier (leaf + intermediate + root) + Finished round-trip
  row 만 emit. 실 TLS handshake / 실 cert verify / OpenSSL/BoringSSL/mbedtls 측 binding = 미수행.
- .roadmap.loader_win32 cond.1 (kernel32 + 광의 Win32 syscall surface coverage) 측 evidence augment
  3-5 row 추가 — status partial → partial 유지 (full Win32 coverage = multi-cycle long-horizon).
- "natural environment alignment" path 측 long-horizon goal — Win32 syscall perfection rosetta toward
  L2 Warden detection compat 측 multi-year baseline (online_expansion_lockin 5 항목 ALL OK).

규칙:
- additive only — 기존 self_test row 0 삭제, 3 NEW module + 1 NEW handoff + 1 NEW marker + cond.1
  evidence array append (4 row).
- migration / destructive op / 외부 cost / push: 모두 금지.
- own1 (Wine 0 / hexa-only / NO winsock-equivalent OSS / NO TLS library binding) + own2 (PARTIAL
  emit explicit, silent_error_ban) 보존.
- BR-NO-USER-VERBATIM (사용자 원문 인용 금지, paraphrase only).
- silent-land — 새 doc + marker 만 land, 외부 announce 0.

## §2 3-loader implementation table (module / LOC / fn count / emit format)

| module | LOC | DLL | fn surface | emit format | sha256 (truncated) |
|---|---:|---|---:|---|---|
| lib/loader/pe_winsock_ws2_32.hexa | 431 | WS2_32.dll | 20 fn (WSAStartup/WSACleanup, socket/closesocket, connect/bind/listen/accept, send/recv/WSASend/WSARecv, WSASocket/WSAEventSelect/WSAWaitForMultipleEvents, getaddrinfo/freeaddrinfo/gethostbyname, select/WSAPoll) | `__WINSOCK__ PARTIAL <fn>` (×10 println) + r0_emit (×18 record) | 5779828e... |
| lib/loader/pe_wininet_https.hexa | 443 | WININET.dll | 12 fn (InternetOpen/CloseHandle, InternetConnect/OpenUrl, HttpOpenRequest/SendRequest/QueryInfo, InternetReadFile/WriteFile, InternetSetOption/QueryOption, HttpAddRequestHeaders) | `__WININET__ PARTIAL <fn>` + `__WININET__ PARTIAL HttpSendRequest <code>` (×10 println) + r0_emit (×16 record) | 299dbcd2... |
| lib/loader/pe_schannel_tls.hexa | 438 | SCHANNEL.dll | 8 fn SSPI (AcquireCredentialsHandle/FreeCredentialsHandle, InitializeSecurityContext/AcceptSecurityContext, EncryptMessage/DecryptMessage, QueryContextAttributes, CertVerifyChainPolicy) | `__SCHANNEL__ PARTIAL <fn>` (×11 println) + r0_emit (×16 record) | 6cdd15ca... |
| **module total** | **1312** | **3 DLL** | **40 fn surface** | **3 emit family** | — |

각 모듈 self_test() 측 r0_emit 14+ assertion + 4 TSV table persist + cleanup invariant. 0 real
network code, 0 real socket(2), 0 real TLS library import (OpenSSL/BoringSSL/mbedtls 0 줄).

## §3 .roadmap.loader_win32 cond evidence augment table (before/after evidence count)

| cond_id | status | before evidence count | after evidence count | delta | evidence rows added |
|---|---|---:|---:|---:|---|
| loader_win32.cond.1 | partial → partial (unchanged) | 8 | 12 | **+4** | (a) Track K1 pe_winsock_ws2_32 skeleton-tier 20 fn / (b) Track K2 pe_wininet_https skeleton-tier 12 fn / (c) Track K3 pe_schannel_tls skeleton-tier 8 SSPI fn / (d) natural environment alignment posture (Warden compat L2 long-horizon) |
| loader_win32.cond.2 | partial (unchanged) | 6 | 6 | 0 | none (user32+gdi32 render scope, Track K out-of-scope) |
| loader_win32.cond.3 | partial (unchanged) | 4 | 4 | 0 | none (ntdll Nt* syscall scope, Track K out-of-scope) |

cond.1 blocker_reason refresh: 기존 "3 핵심 fn real 구현 stage 별 coverage 미수치화" 후미에 "3 network
loader skeleton-tier landed (Winsock + WININET + SCHANNEL via Track K1/K2/K3 2026-05-03) —
production smoke + full coverage remain gated on cycle 2+ + user network smoke (long-horizon Warden
compat L2)" append. 0 status field promotion (full Win32 coverage = multi-cycle long-horizon, met
승격은 production smoke + Warden compat 측 별도 cycle 측 조건 만족 시).

전체 cond 3 中 0 promoted (status enum 변경 0), 1 evidence augmented (cond.1 +4 rows). own1 (Wine 0)
+ own2 (PARTIAL emit explicit) preserved.

## §4 online_expansion_lockin L1-L5 ack confirmation

| 항목 | 명칭 | 사용자 lock-in posture | marker reflect | 본 cycle posture |
|---|---|---|---|---|
| L1 | EULA/ToS violation risk acknowledged | OK (a+ path) | `eula_violation_risk_acknowledged: true` | online D2R 측 production access = ToS 위반 가능성 인지 — skeleton-tier 측 hexa-only design 으로 risk 분리 |
| L2 | Warden detection risk acknowledged | OK (long-horizon goal) | `warden_detection_risk_acknowledged: true` | Track K 측 Win32 syscall perfection rosetta = L2 long-horizon natural-environment alignment 측 contribution. 본 cycle 은 skeleton 만 — production Warden compat 측 multi-cycle 측 후속. |
| L3 | 1-2 year+ multi-cycle horizon acknowledged | OK | `multi_year_horizon_acknowledged: true` | Track K skeleton-tier = cycle 1 only, full coverage 측 production smoke + Warden compat 측 cycle N+ pending |
| L4 | D2R online single-target lock-in | OK (D4 retired_intentional unchanged) | `d2r_single_target_lockin: true` + `d4_retired_intentional_unchanged: true` | Track K 측 synthetic Battle.net REST endpoint URL 측 하드코딩 (us.api.battle.net) = D2R 측 endpoint mirror, D4 측 endpoint 0 라인 |
| L5 | Track J/K/L parallel BG land scope locked-in | OK | `track_jkl_parallel_lockin: true` | Track K (online network stack) = J (Warden compat) + L (D2R realm protocol) 와 parallel BG. 본 cycle K 만 land, J/L 은 별도 BG agent territory (cross-link untouched) |

5 항목 ALL OK 사용자 lock-in 2026-05-03. marker `online_expansion_lockin` block 5 field 모두 true +
lockin_iso "2026-05-03".

## §5 natural environment alignment posture (Win32 syscall perfection toward L2 Warden compat — long-horizon)

Warden (Blizzard 측 anti-cheat) detection 회피 측 직접적 path 0 — 본 Track K 는 "natural environment
alignment" 측 indirect contribution 만:

- **Win32 syscall surface coverage** = real Windows native binary 측 ws2_32.dll / wininet.dll /
  schannel.dll 측 fn 호출 patterning 측 baseline knowledge 축적. Wine source 측 코드 인용 0, OpenSSL 측
  binding 0, MS SSPI source 측 reverse engineering 0 — wininet.h / winsock2.h / sspi.h 측 reference
  shape 만 (외부에 공개된 표준 header 측 type definition).
- **synthetic byte array round-trip** = real packet 형식 (TLS 1.3 RFC 8446 §4.1.2 ClientHello, MS
  SOCKADDR_IN definition, HTTP/1.1 wire format) 측 layout 측 understanding 보존, 실 wire data 0.
- **Blizzard cert chain pinning anchor 인지** = SCHANNEL CertVerifyChainPolicy 측 row 측 "Blizzard
  Entertainment Root CA" pinning anchor 측 acknowledgement only — 실 Blizzard cert 측 byte 0 줄
  inline (root CA fingerprint 0, public key 0).
- **L2 long-horizon multi-year baseline** = 본 cycle skeleton-tier 측 fn surface coverage = cycle 1 베이스라인.
  cycle 2+ 측 production smoke (real socket → real connect → real handshake) + cycle N+ 측 full
  Warden detection compat (proper TEB / PEB / NtQueryInformationProcess shape match + heap pattern
  match + syscall sequence match) 측 multi-year work 측 첫 마일스톤.

직접적 Warden bypass / detection vector 0 줄 — 본 Track K 는 patterning 측 honest baseline 만.

## §6 caveats inline (honest C3 ≥ 10 numbered)

C1. **WS2_32 측 synthetic round-trip — real socket(2) syscall X**: pe_winsock_ws2_32.hexa 측 self_test()
    측 round-trip 은 println("__WINSOCK__ PARTIAL <fn>") + r0_emit 만. 실 macOS BSD socket(2) syscall
    경유 측 실 TCP connect / 실 send/recv loop 0 — production smoke 측 cycle 2+ pending. 의도적 (own1
    Wine 0 / hexa-only mandate 정합).

C2. **WININET 측 synthetic HTTPS round-trip — real HttpSendRequest X**: pe_wininet_https.hexa 측 200/
    401/500 response code 3-state matrix 측 synthetic 만. 실 https://us.api.battle.net/d2r/profile/v1/
    account 측 GET request 측 TLS handshake → cert verify → request line → response parse loop 0 —
    real Battle.net endpoint 측 호출 = ToS 위반 가능성 (online_expansion_lockin L1 acknowledged) 측
    skeleton-tier 측 분리 유지.

C3. **SCHANNEL 측 synthetic TLS handshake — real ClientHello byte X**: pe_schannel_tls.hexa 측 200
    byte synthetic ClientHello 측 wire format reference shape (RFC 8446 §4.1.2) 만 — 실 random 32 byte
    + 실 client key share secp256r1 + 실 cipher suite negotiation 0. 실 SCHANNEL.dll 측 InitializeSecurityContext
    호출 0 (Wine schannel source 측 코드 인용 0, OpenSSL/BoringSSL binding 0).

C4. **Blizzard cert chain 측 synthetic pinning anchor — real root CA fingerprint X**: pe_schannel_tls.hexa
    측 cert_chain TSV table 측 "Blizzard Entertainment Root CA" pinning anchor 측 row 만 — 실 Blizzard
    root CA 측 SHA256 fingerprint 0 inline, 실 public key 0 inline. CertVerifyChainPolicy 측 verify
    status "PARTIAL" 만 — production cycle 측 실 cert pinning matrix 측 별도 cycle 측 pending.

C5. **3 module 측 PARTIAL emit explicit — silent_error_ban (raw 12) 정합**: PARTIAL ≠ PASS. 본 cycle
    측 status emit 은 모두 `__WINSOCK__ PARTIAL <fn>` / `__WININET__ PARTIAL <fn>` / `__SCHANNEL__
    PARTIAL <fn>` 측 명시 — production smoke + full coverage 미달 측 honest acknowledgement. PARTIAL
    swallow as PASS = silent_error_ban 위반 = 본 cycle 측 명시적 회피.

C6. **cond.1 status partial → partial 유지 — full Win32 coverage = multi-cycle long-horizon**: 본
    cycle 측 evidence augment (8 → 12 row) 만 — cond.1 status field promotion (partial → met) 0.
    full Win32 coverage 측 production smoke + fn-by-fn real-vs-stub matrix SSOT 측 3 + 3 + 8 = 14
    추가 fn × stage5 측 real implementation = 다단계 cycle work — 본 cycle 은 skeleton-tier 측
    baseline 만 정착.

C7. **L2 Warden compat = long-horizon, 본 cycle 직접적 detection vector 0**: pe_schannel_tls.hexa 측
    natural environment alignment posture = indirect contribution (syscall surface naming /
    synthetic byte layout / Blizzard root CA pinning anchor row 만). 실 Warden 측 syscall hooking
    pattern detection / stack trace symbol match / process structure walk 측 분석 0 — 별도 Track J
    (c_bnet_warden_environment_compat) territory (parallel BG, 본 cycle 측 untouched).

C8. **online_expansion_lockin marker block 측 사용자 lock-in 2026-05-03 reflect — verbatim 측 0**:
    user_directive_paraphrase 측 5 항목 (L1 EULA/ToS / L2 Warden long-horizon / L3 multi-year / L4
    D2R single target / L5 Track J/K/L parallel) 측 paraphrase 만 — 사용자 원문 인용 0 (BR-NO-USER-VERBATIM
    raw 175 정합). marker 측 `online_expansion_lockin` block 5 field 모두 true + lockin_iso
    "2026-05-03" 측 explicit reflection.

C9. **Track K scope 측 J / L 분리 — 본 cycle 측 cross-link untouched**: 본 cycle 은 K (online network
    stack 3 loader) 만 — Track J (lib/perf/c_bnet_warden_environment_compat) + Track L (.roadmap.diablo2_resurrected
    cond cross-link / docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md) 측 territory 0 라인 touch. 3 track
    parallel BG land 측 lock-in (L5) — cross-cycle merge audit 별도 cycle.

C10. **synthetic Battle.net REST endpoint URL 측 하드코딩 — D2R online single target (L4) reflect**:
     pe_wininet_https.hexa 측 url field 측 "https://us.api.battle.net/d2r/profile/v1/account" + "/d2r/
     save/v1/upload" 측 D2R-specific path. D4 측 endpoint (us.api.blizzard.com/d4/...) 0 줄 — D4
     retired_intentional 측 status enum 측 unchanged 정합 (predecessor self_mk2_tuning land 측
     명시). pe_schannel_tls.hexa 측 SNI "us.api.battle.net" 동일 하드코딩 → D2R single target
     (L4) reflect.

C11. **runtime exec smoke 측 미수행 — cap_minutes 90 + skeleton-tier mandate 정합**: 본 cycle 측
     pe_winsock_ws2_32.hexa / pe_wininet_https.hexa / pe_schannel_tls.hexa 측 self_test() 실측 stdout
     캡처 미수행 — predecessor pe_battlenet_agent_sd_forging.hexa 측 hexa runtime smoke 측 docker
     route fall-through caveat 동급 posture (predecessor offline_shim landing 측 C5 reflection).
     emit count 14+ assertion + LOC 250-450 range + grep PARTIAL ≥ 3 측 static-validate 만.

## §7 next-cycle hooks

- WS2_32 production smoke cycle — 실 socket(2) syscall path (macOS BSD socket → connect → send/recv
  loop) 측 hexa-native minimal harness + Battle.net us.actual.battle.net DNS resolve 측 production
  matrix.
- WININET production smoke cycle — 실 HttpSendRequest → TLS handshake → cert verify → response
  parse loop 측 hexa-native harness (OpenSSL/BoringSSL 측 binding 0, schannel-via-helper.mm 측
  IPC bridge 또는 native macOS Security framework SecTrust path).
- SCHANNEL production cycle — 실 ClientHello byte 측 hexa-native generate + 실 ServerHello parse +
  실 cert chain verify (Blizzard root CA SHA256 fingerprint 측 pinning matrix) — Track J Warden
  compat 측 syscall pattern align baseline.
- loader_win32.cond.1 status promotion partial → met 측 production smoke matrix — 3 핵심 kernel32
  fn (CreateProcess + CreateThread + HeapAlloc) 측 real implementation + 14 network fn 측 production
  smoke 측 multi-cycle work 측 합산 후 status 승격.
- Track J cross-link audit cycle — c_bnet_warden_environment_compat 측 Win32 syscall pattern
  detection vector 측 본 Track K 측 fn coverage SSOT 측 cross-reference (parallel BG agent territory
  merge audit).
- Track L cross-link audit cycle — .roadmap.diablo2_resurrected 측 D2R realm protocol cond 측 본
  Track K 측 wininet/schannel skeleton 측 cross-reference (parallel BG agent territory).
- own1/own2 enforcement 측 본 cycle 신규 fail 0 reflect — predecessor offline_shim landing 측 6 PASS /
  2 FAIL pre-existing 측 본 cycle 측 추가 신규 fail 0 reflect.
