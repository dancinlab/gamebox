---
schema: airgenome-gamebox/docs/airgenome_gamebox_battlenet_online_stack_augment_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_battlenet_online_stack_augment_landed.marker
  roadmap_files_modified:
    - .roadmap.battlenet
  roadmap_files_preserved:
    - .roadmap.purple_launcher
    - .roadmap.lineage_classic_r28
    - .roadmap.lineage_m
    - .roadmap.lineage_w
    - .roadmap.diablo2_resurrected
    - .roadmap.diablo4
    - .roadmap.wow
    - .roadmap.cs2
    - .roadmap.delta_force
    - .roadmap.elden_ring
    - .roadmap.loader_pe
    - .roadmap.loader_win32
    - .roadmap.loader_dx
  predecessor_handoffs:
    - docs/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_warden_honest_freeze_landed_2026_05_03.ai.md
status: LANDED_TRACK_N_BATTLENET_ONLINE_STACK_AUGMENT
related_raws:
  - raw 9    # hexa-only orchestration (4 EXTEND modules + 1 NEW handoff + 1 NEW marker + 1 .roadmap evidence row append, additive only)
  - raw 11   # snake_case
  - raw 12   # silent-error ban (cond.3 status partial 영구 lock 의도)
  - raw 15   # env() lazy + repo-relative paths
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes)
  - raw 168  # minimum-viable additive land
preserved_unchanged:
  - 13 .roadmap.* files
  - all predecessor markers
  - lib/perf/c_bnet_warden_environment_compat.hexa (Track-J territory — read-only reference)
  - all other lib/loader/ + lib/perf/ files
  - all native sources / tests / tool/ untouched
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 5   # 4 .hexa modules append + 1 .roadmap.battlenet cond.3 evidence + blocker_reason
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
online_expansion_lockin:
  L1: true   # EULA/ToS violation risk acknowledged
  L2: true   # Warden detection risk acknowledged
  L3: true   # 1-2 year+ multi-cycle horizon acknowledged
  L4: true   # D2R online single-target lock-in (D4 retired_intentional unchanged)
  L5: true   # Track J/K/L/N parallel scope locked-in
  lockin_iso: 2026-05-03
own1_own2_reflect:
  own1: Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 — hexa-only; real network 0 / OAuth credential 0 / chat protocol bypass 0 / token spoof 0 / endpoint hijack 0 / IPC bypass 0
  own2: log/error/emit/honest — `__BNET_CHAT__ PARTIAL <state>` + `__BNET_LOGIN__ PARTIAL <state>` + `__BNET_TOKEN__ PARTIAL <state>` + `__BNET_IPC__ PARTIAL <verb>` (PARTIAL emit explicit, NOT PASS — production smoke deferred)
production_smoke_gate:
  cond_3_promotion_to_met: false
  reason: real network endpoint smoke (Battle.net live OAuth2 + Agent IPC + chat protocol) NOT exercised this cycle — synthetic state machines + IPC envelope round-trip only
  separate_cycle: user network smoke (multi-year horizon per L3 lock-in)
---

# airgenome-gamebox Track-N battlenet online stack augment landed - 2026-05-03

## TL;DR

Track-N mission: 4-module evidence augment for `.roadmap.battlenet` cond.3 (OAuth 2-leg + chat/login/token state track + Agent IPC live). 4 existing battlenet modules extended additively with synthetic state machine + IPC envelope round-trip rows. cond.3 status remains `partial` (production smoke gate permanent until real network endpoint smoke). online_expansion_lockin L1-L5 carried forward from Track-J/K/L (5/5 true, lockin_iso 2026-05-03).

- 4 modules EXTENDED (additive only — append rows, NEVER remove existing)
- `.roadmap.battlenet` cond.3 evidence count: 7 → 12 (+5)
- handoff + marker land
- cond.3 status `partial` 영구 유지 (NOT promoted to met)

## §1 user_directive_paraphrase

BG subagent — Track-N battlenet online stack 4-module evidence augment for airgenome-gamebox: extend self_test() in 4 existing battlenet modules (lib/perf/c_bnet_chat_state.hexa, lib/perf/c_bnet_login_state_track.hexa, lib/perf/c_bnet_token_refresh_track.hexa, lib/loader/pe_battle_net_agent_ipc_track.hexa) with synthetic state machine + IPC round-trip rows + augment .roadmap.battlenet cond.3 evidence (additive only). cond.3 status STAYS `partial` (production smoke gate remains — real network endpoint dependent). online_expansion_lockin (a+ path) L1-L5 (5 items all true) carried forward from Track-J/K/L precedent. own1 (Wine 0 + real network 0 + OAuth credential 0 + chat protocol bypass 0 + token spoof 0 + endpoint hijack 0 + IPC bypass 0) + own2 (PARTIAL emit explicit NOT PASS, silent error 0) 영구 정합. policy: additive only / migration forbidden / destructive 0 / $0 / cap 90min / BR-NO-USER-VERBATIM / friendly preset / silent-land marker.

## §2 4-module diff summary (LOC delta)

| # | path | LOC before | LOC after | LOC delta | emit count (added) | sha256 (after) |
|---|------|-----------:|----------:|----------:|-------------------:|---------------:|
| N1 | `lib/perf/c_bnet_chat_state.hexa` | 179 | 214 | +35 | 4 println + 5 r0_emit | `cfdf64f5a2a3674daa613879dee1f6c031a7207896d5f3c09c809f798512977c` |
| N2 | `lib/perf/c_bnet_login_state_track.hexa` | 131 | 171 | +40 | 5 println + 6 r0_emit | `55b5df35afef7db98dfa5a53b6c8d422df2a6d65c7c9f9a83895f0c59fb506d5` |
| N3 | `lib/perf/c_bnet_token_refresh_track.hexa` | 202 | 243 | +41 | 5 println + 6 r0_emit | `5b36313edcaf7942437814551adc8325bc50050e96399dc8877180b1258efa14` |
| N4 | `lib/loader/pe_battle_net_agent_ipc_track.hexa` | 450 | 492 | +42 | 4 println + 5 r0_emit | `2999e739abba5154beea4126af90a2450800d0b89c5f1327fd920f9769c40241` |

Total LOC delta: +158. Total println added: 18 (`__BNET_CHAT__` ×4 + `__BNET_LOGIN__` ×5 + `__BNET_TOKEN__` ×5 + `__BNET_IPC__` ×4). Total r0_emit added: 22 (per-state rows + matrix summary rows).

### N1 — c_bnet_chat_state.hexa
synthetic chat presence state machine: ONLINE → AWAY → DND → OFFLINE 4 transition. payload = chat_session_id + presence_kind + msg_count_simulated. emit `__BNET_CHAT__ PARTIAL <state>`.

### N2 — c_bnet_login_state_track.hexa
synthetic login state machine: PRE_AUTH → OAUTH_REQUEST → TOKEN_VALIDATE → SESSION_ACTIVE → TOKEN_EXPIRY 5 transition. payload = session_id + oauth_step_kind + token_lifetime_simulated_sec. emit `__BNET_LOGIN__ PARTIAL <state>`.

### N3 — c_bnet_token_refresh_track.hexa
synthetic token refresh state machine: VALID → NEAR_EXPIRY → REFRESH_REQUEST → REFRESH_SUCCESS → REFRESH_FAIL_RETRY 5 transition. payload = token_id + refresh_attempt_count + backoff_ms_simulated. emit `__BNET_TOKEN__ PARTIAL <state>`.

### N4 — pe_battle_net_agent_ipc_track.hexa
synthetic Agent IPC envelope round-trip: 4 verb (install/launch/uninstall/heartbeat) × IPC message envelope. payload = ipc_msg_id + verb_kind + response_code_simulated. emit `__BNET_IPC__ PARTIAL <verb>`. distinct from cond.1 PASS shim (offline shim 측 PASS) — 본 augment 는 cond.3 PARTIAL matrix.

## §3 cond.3 evidence count before/after

| metric | before (post Track-J) | after (post Track-N) | delta |
|---|---:|---:|---:|
| `battlenet.cond.3.evidence` array length | 7 | 12 | +5 |
| `battlenet.cond.3.status` | `partial` | `partial` | unchanged (영구 lock) |
| `battlenet.cond.1.status` | `met` | `met` | unchanged (Track-A territory) |
| `battlenet.cond.2.status` | `met` | `met` | unchanged (Track-A territory) |
| `battlenet.blk.1.status` | `open` | `open` | unchanged |

5 evidence rows added:
1. `lib/perf/c_bnet_chat_state.hexa self_test extended (Track-N synthetic chat presence state machine round-trip)`
2. `lib/perf/c_bnet_login_state_track.hexa self_test extended (Track-N synthetic login state machine round-trip)`
3. `lib/perf/c_bnet_token_refresh_track.hexa self_test extended (Track-N synthetic token refresh state machine round-trip)`
4. `lib/loader/pe_battle_net_agent_ipc_track.hexa self_test extended (Track-N synthetic Agent IPC envelope round-trip)`
5. Track-N cross-link to Track-J Warden cross-layer + Track-L D2R realm protocol freeze (online_expansion_lockin L1-L5 acknowledged)

`blocker_reason` refreshed to acknowledge "synthetic state machines + IPC round-trip landed; production smoke + real network endpoint smoke remain gated on user network smoke (separate cycle)".

## §4 online_expansion_lockin L1-L5 ack

Carried forward from Track-J/K/L precedent (already user-acknowledged):

- **L1: true** — EULA/ToS violation risk acknowledged
- **L2: true** — Warden detection risk acknowledged
- **L3: true** — 1-2 year+ multi-cycle horizon acknowledged
- **L4: true** — D2R online single-target lock-in (D4 retired_intentional unchanged)
- **L5: true** — Track J/K/L/N parallel scope locked-in
- **lockin_iso: 2026-05-03**

6 redundant boolean ack fields (per Track-J/K/L precedent) emitted in marker top-level:
- `eula_violation_risk_acknowledged: true`
- `warden_detection_risk_acknowledged: true`
- `multi_year_horizon_acknowledged: true`
- `d2r_single_target_lockin: true`
- `d4_retired_intentional_unchanged: true`
- `track_jkln_parallel_lockin: true`

## §5 production smoke gate caveat

**cond.3 met requires real network — NOT this cycle.** This Track-N augment is synthetic state machine + IPC envelope round-trip ONLY. cond.3 promotion to `met` is permanently gated on:

1. real us.battle.net OAuth2 endpoint round-trip (TLS + token signing + session cookie)
2. real Battle.net Agent http://localhost:1120 endpoint round-trip (TCP loopback + IPC envelope)
3. real XMPP-like presence server round-trip (chat protocol)
4. real RFC 6749 grant_type=refresh_token round-trip (refresh_token bearer + exponential backoff timer)
5. user network smoke (separate cycle, multi-year horizon per L3)
6. Win32 syscall perfection (Track-K loader cycle + cycle 2+, long-horizon)
7. 자연 환경 정합 (natural environment alignment)

**none of these are exercised this cycle** — Track-N is evidence augment only (cond.3 partial → partial, additive).

## §6 caveats (≥8)

C1. **own1 + real network 0**: 본 cycle 측 4 module 모두 `socket()` / `connect()` / `send()` / `recv()` / `WSAStartup` / `gethostbyname` / `getaddrinfo` 등 실 network 측 syscall 0건. synthetic constant emit only.

C2. **own1 + Wine 0**: Wine / CrossOver / Whisky / GPTK 의 chat / oauth / token / IPC code path 인용 0건. hexa-native loop only.

C3. **own1 + token spoof 0**: 실 OAuth2 access_token / refresh_token / bearer token / signing key (RS256/ES256) 측 spoof / forge 0건. payload 측 `tok-synth-<unix_ts>` synthetic id only.

C4. **own1 + chat protocol bypass 0**: 실 XMPP / Battle.net presence protocol 측 frame format / handshake / tls-on-stream 우회 0건. presence_kind 측 enum constant emit only.

C5. **own1 + IPC endpoint spoof 0**: 실 http://localhost:1120 endpoint 측 port hijack / DNS spoof / loopback rebind 0건. endpoint_simulated 측 string constant emit only (실 connection X).

C6. **own2 + PARTIAL emit explicit**: 18 println 모두 `PARTIAL` 키워드 명시 (NOT `PASS`). cond.1 측 PASS shim (Track-A `pe_battlenet_agent_sd_forging.hexa __BNET_AGENT__ PASS install/launch/uninstall`) 와 명확 distinct — production smoke gate 미달성 정합.

C7. **own2 + silent error 0**: `.roadmap.battlenet.blk.1` status `open` 영구 유지. cond.3 status `partial` 영구 lock (의도 — silent succeed X). cond.3 promotion to `met` 측 production smoke gate 명시.

C8. **production smoke gate permanent**: cond.3 met 승격 measure 측 real network endpoint smoke + Win32 syscall perfection (Track-K) + 자연 환경 정합 long-horizon work 측 multi-cycle horizon. 본 cycle 측 augment only.

C9. **synthetic vs real distinction**: 4 module self_test 측 augment row 모두 `transport_kind: synthetic_inline` / `network: none` 명시. 실 transport (named_pipe / tcp_loopback / websocket / https) 측 measurement 측 production cycle 시점 별도 측정.

C10. **N4 distinct_from_cond_1_pass_shim flag**: pe_battle_net_agent_ipc_track.hexa 측 N4 augment row matrix 측 `distinct_from_cond_1_pass_shim: true` 명시. cond.1 측 PASS shim (offline shim) 과 cond.3 측 PARTIAL augment 명확 분리.

C11. **track parallel scope**: Track-J (Warden) + Track-K (network 3 loader) + Track-L (D2R realm freeze) 측 모듈 / 문서 본 cycle 측 untouched (read-only reference). Track-N 측 only 4 module + 1 roadmap evidence + 1 handoff + 1 marker land.

C12. **emit count not asserted in self_test**: append rows 측 `emit_n` minimum 측 panic 측 추가 X (기존 self_test panic 측정 preserve). 실 emit 측 r0_session 측 cumulative count 자동 반영 (cleanup 후 cleanup directive 별 흐름 N1-N3 / cleanup 후 흐름 N4 — N4 는 cleanup 이후 augment rows 측 emit, 기존 emit_n>=7 panic preserve).

C13. **mk1 narrative freeze doc untouched**: docs/WARDEN_HONEST_FREEZE.md (Track-J) + docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md (Track-L) 본 cycle untouched. Track-N 측 only handoff + marker (mk2 schema land).

C14. **append-after-cleanup pattern (N1-N3) vs append-after-cleanup-validated (N4)**: N1/N2/N3 측 augment rows 측 cleanup_*() 호출 직전 위치, N4 측 augment rows 측 cleanup_pbnait + 검증 이후 위치 (기존 self_test 흐름 보존 정합). 두 경우 모두 기존 panic 검증 측 preserve.

## §7 next-cycle hooks

1. **battlenet cond.3 production smoke** — 사용자 network smoke + Battle.net live OAuth2/IPC/chat endpoint 통과 시점 (multi-cycle 후, L3 horizon 정합) → met 승격 candidate
2. **Track-K loader 측 Win32 syscall perfection** — pe_winsock_ws2_32 / pe_wininet_https / pe_schannel_tls 측 자연 환경 정합 long-horizon work
3. **Track-J Warden detection-vector tracker live measurement** — 사용자 lock-in (L1+L2 acknowledged) 후 실 D2R launch 측 vector emit 측정 (사용자 자체 책임 영역)
4. **Track-L D2R realm protocol freeze** 측 cross-cycle alignment — realm_login_track / realm_chat_proxy 측 sister freeze doc 측 cross-link refresh 별도 cycle
5. **다른 user-mode online stack augment** — D4 always-online / WoW realm 측 sister augment 별도 cycle (현재 D4 retired_intentional, WoW 측 cond.1 미land)
6. **raw 270 triplet T1** — per-game roadmap 별 README.ai.md (각 게임 sub-bucket 분할) — 별도 cycle
7. **fan-out: VAC / EAC / BattlEye** 측 honest freeze 별도 cycle — Track-J Warden 정합 pattern reuse
8. **synthetic state machine pattern reuse** — 다른 launcher (purple / lineage) 측 chat/login/token state augment 별도 cycle (online_expansion_lockin scope 정합 시)
