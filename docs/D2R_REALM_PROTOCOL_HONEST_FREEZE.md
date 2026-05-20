# D2R Online Realm Protocol Honest Spec Freeze — Battle.net + Diablo II: Resurrected online realm surface

> **status**: 작성일 **2026-05-03** / **scope: HONEST SPEC FREEZE ONLY** — 본 문서는 Battle.net 인증 + Diablo II: Resurrected (D2R) online realm 측 **공개 자료 기반 high-level layer-naming reference** 이며, **wire-protocol implementation / packet construction / MITM (man-in-the-middle) interception / replay attack / traffic shaping evasion / 사설 realm server 코드 / 패턴 / 시도 0건**.
> 작성: 2026-05-03 (Track L D2R realm protocol honest freeze cycle, sister Track E `GAMEGUARD_HONEST_FREEZE.md` + Track J `WARDEN_HONEST_FREEZE.md`)
> SSOT: `.roadmap.battlenet` cond.3 (OAuth 2-leg + chat + token refresh + login state) + `.roadmap.diablo2_resurrected` cond.1 (production binary smoke transitive)
> 결론 한줄: **D2R online realm = Battle.net 인증 + realm matchmaking + game realm TCP/UDP packet stream + Warden cross-layer anti-cheat. macOS = Blizzard 공식 미지원 platform (D2R Mac client 미존재) → realm protocol implementation = ToS violation 직격 surface → airgenome-gamebox 측 high-level layer-naming reference 외 영역 영구 out-of-scope (own1 Wine 0 + protocol-impl 0 + MITM 0 + replay 0 + own2 honest emit 정합)**.

---

## 0. 본 doc 의 목적과 한계 (가장 먼저 명시)

### 0.1 목적 (in-scope)

- D2R online realm 측 **layer 구분 + stage 전이 명칭** high-level 동결 (research-direction reference at HIGH LEVEL ONLY)
- airgenome-gamebox 측 protocol implementation 영역 영구 out-of-scope rationale documentation
- `.roadmap.battlenet` cond.3 + `.roadmap.diablo2_resurrected` cond.1 측 cross-link reference
- online expansion lock-in (L1 EULA / L2 Warden / L3 multi-year horizon / L4 D2R single-target / L5 Track J/K/L 병행) 측 SSOT

### 0.2 한계 (out-of-scope, 영구)

본 doc 측 다음 카테고리는 **영구 out-of-scope** 이며, 어떤 형태의 implementation 도 airgenome-gamebox 측 import 되지 않음:

- **wire-protocol packet 측 field offset / byte sequence / encryption scheme / handshake byte 순서** 측 명시
- **MITM (man-in-the-middle) proxy 구성** / SSL/TLS 가로채기 / 인증서 위조 / certificate-pinning 우회 방법
- **replay attack** 패턴 / packet capture 후 재전송 도구 / 시간차 replay 회피
- **packet injection** / 위조 packet 생성 / 임의 client→server 메시지 작성
- **traffic shaping evasion** / Warden 측 client-side anomaly 검출 회피 timing 패턴
- **packet decryption tooling** / TLS keylog / 세션 키 추출 / handshake 측 secret material recovery
- **사설 realm server (private server / custom realm)** 측 server-side reimplementation / open-source server fork
- **wire-protocol implementation guide** (어떤 stage / 어떤 layer / 어떤 transport 든 영구 X)
- 어떤 형태의 cheat / botting / 자동화 / dupe / item duplication signature
- 사용자 계정 (Battle.net account / 2FA / authenticator) 측 위협 행위
- TPP (third-party program) 정의에 해당하는 어떤 자동화 / 보조 / 분석 도구

**본 doc 는 위 카테고리 중 어느 것도 implement / cite (구체 코드 / 구체 패턴) / encourage 하지 않음**. 외부 공개 자료의 **layer 명칭 + stage 전이 명칭 only** 수준에서만 reference, 어떤 packet structure / wire format detail / MITM 절차 / replay 절차 / private server 디자인도 본 doc 에 포함되지 않음.

### 0.3 own1 / own2 alignment

- **own1 (Wine 0 / hexa-only)**: 본 doc + airgenome-gamebox 본체 측 Wine / CrossOver / Whisky / GPTK / community wrapper 0줄. D2R 측 protocol-impl 코드 / MITM 도구 / replay 도구 / 사설 server 0줄. Battle.net 측 session 위조 / token forge 0줄.
- **own2 (log/error/emit/honest)**: 본 doc 의 모든 lock-in 항목 (L1-L5) + 모든 out-of-scope 항목 explicit emit. swallowed risk 0건, silent error 0건. 실 network operation 본 cycle 측 0건 (honest emit posture).

---

## 1. D2R online realm protocol scope (high-level layer naming only)

> **재경고**: 본 §1 은 layer 의 **명칭 / 역할 한 줄 요약** 만 제시. 어떤 packet structure / field offset / encryption scheme / handshake byte sequence 도 본 doc 에 포함되지 않음.

### 1.1 layer 4-divisions (high-level naming only)

| layer | 명칭 (high-level) | 역할 한 줄 요약 (vendor 공식 + 공개 자료 기반) |
|---|---|---|
| L0 | **Auth Layer** (Battle.net authentication) | Battle.net 계정 측 OAuth 2-leg flow (login + token refresh) — vendor 공식 endpoint 기반 |
| L1 | **Realm Selection Layer** | 인증 token 으로 realm list 조회 + 진입할 realm 선택 (region / latency 등) |
| L2 | **Game Realm Layer** | 선택된 realm 측 character list / game create+join / game-loop 측 stream |
| L3 | **Anti-Cheat Cross-Layer (Warden)** | 위 3 layer 모두 측 cross-layer 검증 (Track J `WARDEN_HONEST_FREEZE.md` 측 sister doc) |

위 layer 4-division 은 **vendor 공식 자료 + 공개 textbook level 자료 기반 추상 분류** — 실 wire 측 layer 경계 / handshake 순서 / packet 측 layer 식별 방법 등은 본 doc 측 명시 영구 X.

### 1.2 Auth Layer (L0) — Battle.net OAuth 2-leg (high-level only)

| 항목 | 본 doc 측 stance |
|---|---|
| OAuth 2-leg flow 명칭 | 명칭 reference (vendor 공식 OAuth doc 기반) |
| token refresh 흐름 | 흐름 명칭 reference (vendor 공식) |
| chat surface | 명칭 reference (Battle.net friend / chat 기능 존재 사실) |
| login state | 명칭 reference (login/logout 상태 존재 사실) |
| **endpoint URL 명시** | **본 doc 측 X** (vendor 공식 portal 측 URL 은 시기 변동) |
| **packet field offset** | **영구 X** |
| **token signing 알고리즘 detail** | **영구 X** |
| **2FA / authenticator 우회** | **영구 X** |

### 1.3 Realm Selection Layer (L1) — high-level only

| 항목 | 본 doc 측 stance |
|---|---|
| realm list 조회 명칭 | reference only |
| region selection 명칭 | reference only |
| **realm list 측 wire format** | **영구 X** |
| **선택 packet structure** | **영구 X** |

### 1.4 Game Realm Layer (L2) — high-level only

| 항목 | 본 doc 측 stance |
|---|---|
| character list / character create 명칭 | reference only (UI flow 존재 사실) |
| game create / game join 명칭 | reference only |
| game-loop stream 존재 사실 | reference only (실시간 ARPG = 지속 stream 필요 사실 자체) |
| **transport (TCP/UDP) wire format** | **영구 X** |
| **action packet field map** | **영구 X** |
| **server tick rate / sync 알고리즘** | **영구 X** |

### 1.5 Anti-Cheat Cross-Layer Warden (L3) — Track J sister

Warden 은 위 L0/L1/L2 모두 측 cross-layer 로 동작하는 anti-cheat 검증 layer. **세부 내용은 Track J `WARDEN_HONEST_FREEZE.md` 측 sister doc 측면 reference** — 본 doc §1.5 측 명시 영역은:

- **명칭 / 카테고리 reference 만** (Warden cross-layer 존재 사실)
- 실 검출 vector / 검출 timing / 회피 회피 영구 X (Track J + 본 doc §3 모두 동일 stance)

### 1.6 stage 전이 (name-level only)

D2R online 측 stage 전이는 외부 공개 자료에서 다음 명칭 level 로 언급됨:

`LOGIN → REALM_LIST → REALM_JOIN → CHARACTER_LIST → GAME_JOIN → GAME_LOOP`

**본 doc 의 §1.6 의 위 6 stage 명칭은 high-level state-machine name level only** — 각 stage 측:

- packet sequence / handshake byte / 측 wire 측 식별 — **영구 X**
- stage 측 timeout / retry 회피 패턴 — **영구 X**
- stage 전이 trigger 측 client-side 위조 — **영구 X**

---

## 2. Apple-only constraint (macOS D2R 공식 미지원)

### 2.1 Blizzard EULA / 공식 platform 측 mismatch

- D2R 공식 client = Windows + Console (PS / Xbox / Switch). **macOS native client 영구 미존재**.
- Mac 환경에서 D2R online play 시도 = **Blizzard EULA 측 unsupported platform 사용** = ToS violation surface (L1 lock-in 인지).
- realm protocol implementation 시도 자체 = unsupported platform + reverse-engineered wrapper = 직격 ToS violation path.

### 2.2 Wine / CrossOver D2R community wrapper 측 own1 stance

own1 enforcement 정합 — gamebox 측 Wine / CrossOver / Whisky / GPTK / community wrapper / D2R 전용 patch 0줄. 다음 영역은 **research direction citation only at name level**, 코드 / 패턴 / patch list 측 import 영구 X:

- CrossOver 측 D2R 동작 보고 (개별 사용자 경험) — 본 doc 측 cite / import 0건
- Wine staging branch 측 일부 D2R 관련 patch — 본 doc 측 cite / import 0건
- KR / EN community blog 측 D2R Mac 동작 보고 — 본 doc 측 cite / import 0건

본 doc 의 stance: 위 community 영역은 **존재 사실 명시 only**, 그 영역의 코드 / pattern / patch list / blog post 측 cite / import 영구 X.

### 2.3 retired_intentional 적용 여부 (Track J sister 와 분기)

- Track E `GAMEGUARD_HONEST_FREEZE.md` (lineage_classic_r28 family) = `retired_intentional` 정식 채택 (kernel-AC 직접 차단)
- 본 Track L `D2R_REALM_PROTOCOL_HONEST_FREEZE.md` (D2R family) = **retired_intentional 정식 채택 X** (online expansion lock-in path 측 user-confirm 진입), online surface 의 high-level layer reference 만 documentation. 실 protocol implementation 영역은 영구 out-of-scope 이지만, mk2 status enum 측 `retired_intentional` 정식 marker 는 본 cycle 측 부여 X — D2R 의 status 정합은 별도 cycle 측 user lock-in 후 결정.

---

## 3. honest research-direction reference (no protocol implementation)

> **재경고**: 본 §3 는 외부 공개 자료의 **카테고리 명칭 + 일반 분류** 수준 reference 만. 본 doc 의 어떤 부분도 wire-protocol implementation / packet construction / MITM / replay / private server design 을 명시하지 않으며, airgenome-gamebox 측 import / implement 영역 영구 X.

### 3.1 public protocol research 영역 (research-direction citation only at name level)

외부 공개 자료에 D2R / 구 D2 / Battle.net 관련 reverse-engineered open-source 측면 일부 server / protocol 분석 project 가 존재한다고 보고됨. 본 doc 는 그 영역의 **존재 사실 명시 only**:

| 카테고리 | 본 doc 측 stance |
|---|---|
| 구 Diablo II 측 community OSS server 측면 (e.g. "Project Diablo 2", "Slashdiablo" 등 명칭은 community 측 알려진 사례) | **research direction citation only at name level** — 코드 / packet table / wire format / handshake / encryption 측 cite / import 영구 X |
| D2R 측 reverse-engineered protocol 분석 영역 | **본 doc 측 specific document / blog post / repo URL cite 0건** — 영역 존재 사실 명시 only |
| public Wireshark capture / dissector 영역 | **본 doc 측 dissector code / capture file / 측 cite 0건** — 영역 존재 사실 명시 only |

### 3.2 Wireshark-equivalent observation surface (high-level packet category names only)

외부 공개 자료에 D2R online traffic 측 다음과 같은 **packet category 명칭** 이 분류되어 있다고 알려져 있음 — 본 doc 측 **명칭 + 일반 분류 only**, field-level / byte-level 명시 영구 X:

| packet category 명칭 (high-level) | 본 doc 측 stance |
|---|---|
| auth-related | 명칭 reference only — field map X |
| realm-list-related | 명칭 reference only — field map X |
| character-related | 명칭 reference only — field map X |
| game-create / game-join related | 명칭 reference only — field map X |
| in-game action related | 명칭 reference only — field map X |
| keep-alive / heartbeat related | 명칭 reference only — field map X |
| anti-cheat (Warden) related | 명칭 reference only — Track J sister doc reference, field map X |

위 카테고리는 **OS textbook + 공개 자료 level 의 분류 일반론** 수준이며, 본 doc 측 어떤 카테고리에 대해서도 wire format / field offset / encryption scheme / 회피 방법 measurement 영구 X.

### 3.3 stage 전이 (name-level only — §1.6 의 reference 재명시)

`LOGIN → REALM_LIST → REALM_JOIN → CHARACTER_LIST → GAME_JOIN → GAME_LOOP` — **state-machine name level only** reference. 각 stage 측 packet 측 wire-level 식별 / 전이 측 packet 위조 / 전이 측 timing 우회 측 영구 X.

### 3.4 명시적 out-of-scope 선언 (재명시, raw 12 silent_error_ban 정합)

본 gamebox 는 다음 영역을 **영구 제공하지 않음** (NOT 제공, NOR 제공 시도, NOR encourage):

- wire-protocol implementation
- packet construction code
- MITM (man-in-the-middle) proxy 구성
- replay attack 도구
- private server (custom realm server) reimplementation
- traffic shaping evasion 도구
- packet decryption tooling
- TLS keylog / session key 추출 도구
- 인증서 위조 / certificate-pinning 우회 도구
- token forge / OAuth credential 위조 도구
- 어떤 형태의 자동화 / botting / dupe 도구

**위 모든 항목은 OUT OF SCOPE PERMANENTLY**. 본 doc 측 위 단어들의 등장은 **항상 negation context** (e.g. "본 doc 는 ~~MITM 을 명시하지 않음", "replay attack 은 영구 out-of-scope") 에서만 사용됨.

---

## 4. own1 + own2 alignment (재확인)

### 4.1 own1 enforcement

| own1 항목 | airgenome-gamebox 측 reflect |
|---|---|
| Wine 0 | Wine wine-* binary / patch / module 0줄 |
| CrossOver 0 | CrossOver bottle / module 0줄 |
| Whisky 0 | Whisky wrapper 0줄 |
| GPTK 0 | Apple GPTK metal translation layer 0줄 |
| community D2R wrapper 0 | D2R 전용 community patch / D2R 전용 wrapper / D2R 전용 launcher 측 fetch / import 0줄 |
| hexa-only | airgenome-gamebox 본체 + 본 doc 측 hexa 외 import 0줄 |
| protocol implementation 0 | wire-protocol packet construction / parsing / encryption 코드 0줄 |
| MITM tooling 0 | proxy / 인증서 위조 / TLS 가로채기 코드 0줄 |
| replay tooling 0 | packet capture-and-resend / replay attack 코드 0줄 |
| private realm server 0 | 사설 realm server / custom realm reimplementation 0줄 |
| traffic shaping evasion 0 | timing manipulation / Warden client-side anomaly 회피 코드 0줄 |

### 4.2 own2 enforcement

- 본 cycle 측 실 network operation 0건 (honest emit posture: 본 cycle 의 land 는 doc-only freeze)
- silent error 0건 — 모든 lock-in (L1-L5) explicit emit + 모든 out-of-scope 항목 explicit emit
- swallowed risk 0건 — L1 EULA risk + L2 Warden detection risk + L3 multi-year horizon + L4 D2R single-target + L5 Track J/K/L lock-in 모두 marker top-level 측 explicit field

### 4.3 block-acknowledged risk record

| lock-in | 항목 | acknowledged |
|---|---|---|
| **L1** | EULA / ToS violation risk (Mac unsupported + reverse-engineered protocol surface) | ✓ 사용자 확인 (2026-05-03) |
| **L2** | Warden cross-layer detection risk of realm-level anomalies | ✓ 사용자 확인 (2026-05-03) |
| **L3** | 1-2 year+ multi-cycle horizon (online expansion 자체가 multi-cycle scope) | ✓ 사용자 확인 (2026-05-03) |
| **L4** | D2R online single-target lock-in (D4 retired_intentional unchanged) | ✓ 사용자 확인 (2026-05-03) |
| **L5** | Track J/K/L 병행 BG land scope lock-in | ✓ 사용자 확인 (2026-05-03) |

---

## 5. Cross-link table

본 doc 의 reference 측 cross-link (각 path repo-relative):

### 5.1 sister freeze docs (kernel-AC + cross-layer AC + realm protocol triplet)

| path | role | relation |
|---|---|---|
| `docs/GAMEGUARD_HONEST_FREEZE.md` | Track E grandsire — kernel-AC honest spec freeze (lineage_classic_r28 family) | sister precedent — style mirror source |
| `docs/WARDEN_HONEST_FREEZE.md` | Track J sister — Warden cross-layer anti-cheat honest spec freeze (D2R/WoW/Battle.net family) | sister parallel BG (본 cycle 시점 parallel 진행) |
| `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` | **본 cycle land — Track L, D2R online realm protocol layer reference** | self |

### 5.2 .roadmap cross-link (read-only reference, 본 cycle 측 modification 0건)

| path | cond / blocker | 본 doc 와의 relation |
|---|---|---|
| `.roadmap.battlenet` | cond.3 (OAuth 2-leg + chat + token refresh + login state) | 본 doc §1.2 Auth Layer 측 reference target |
| `.roadmap.diablo2_resurrected` | cond.1 (production binary smoke transitive) | 본 doc §1.4 Game Realm Layer 측 reference target |

본 cycle 은 **doc-only freeze land** 이므로 위 두 .roadmap 측 modification 영구 X (Track I cleanest-cycle posture 정합, in_place_writes=0).

### 5.3 Track K sister loaders (network surface cross-link)

| path | role | relation |
|---|---|---|
| `lib/loader/pe_winsock_ws2_32.hexa` | Track K — ws2_32 winsock loader skeleton (network primitive) | sister (parallel BG) — 본 doc §1.4 transport reference |
| `lib/loader/pe_wininet_https.hexa` | Track K — wininet HTTPS loader skeleton | sister (parallel BG) — 본 doc §1.2 OAuth Auth Layer reference |
| `lib/loader/pe_schannel_tls.hexa` | Track K — schannel TLS loader skeleton | sister (parallel BG) — 본 doc §1.2 + §1.4 transport encryption reference |

위 Track K 측 loader 모듈은 **network primitive 측 PE-side skeleton 한정** (실 network operation X, 실 packet construction X, 본 cycle 측 untouched parallel BG).

### 5.4 predecessor markers + handoff

| marker / doc | role |
|---|---|
| `state/markers/airgenome_gamebox_gameguard_honest_freeze_landed.marker` | Track E predecessor — kernel-AC freeze precedent (style mirror source) |
| `state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker` | Battle.net + D2R offline shim parallel cycle |
| `state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker` | D4 retired_intentional family (L4 lock-in 측 D4 unchanged 정합) |
| `state/markers/airgenome_gamebox_roadmap_op_self_impl_landed.marker` | Track I — cleanest-cycle posture (in_place_writes=0) reference |
| `airgenome/doc/airgenome_gamebox_d2r_realm_protocol_honest_freeze_landed_2026_05_03.ai.md` | **본 cycle handoff (Track L)** |
| `state/markers/airgenome_gamebox_d2r_realm_protocol_honest_freeze_landed.marker` | **본 cycle marker (Track L)** |

---

## 6. future-cycle non-goals (영구 out-of-scope)

본 doc 는 spec freeze 이며, 다음 영역은 **airgenome-gamebox lifetime 동안 영구 out-of-scope**:

### 6.1 wire-protocol 측 work (영구 X)

- ❌ wire-protocol packet construction code
- ❌ field offset map / byte sequence 명시
- ❌ encryption scheme detail / handshake byte sequence
- ❌ transport-level (TCP/UDP) packet 측 parsing / forging
- ❌ stage 전이 측 packet 위조

### 6.2 MITM / replay / injection 측 work (영구 X)

- ❌ MITM (man-in-the-middle) proxy 구성
- ❌ TLS / SSL 가로채기 / 인증서 위조 / certificate-pinning 우회
- ❌ replay attack 도구 / packet capture-and-resend
- ❌ packet injection / 위조 packet 생성

### 6.3 server-side reimplementation 측 work (영구 X)

- ❌ 사설 realm server / private server / custom realm 서버 작성
- ❌ open-source D2/D2R 서버 fork / 패치 / 측 import
- ❌ realm matchmaking 측 server-side 우회 endpoint

### 6.4 traffic / token 측 work (영구 X)

- ❌ traffic shaping evasion (Warden client-side anomaly 회피 timing)
- ❌ packet decryption tooling
- ❌ session key / TLS keylog / handshake secret material 추출
- ❌ token forge / OAuth credential 위조

### 6.5 in-scope (영구 영역, 본 doc 가 다루는 영역)

- ✓ high-level layer naming reference (Auth / Realm Selection / Game Realm / Warden cross-layer)
- ✓ stage 전이 state-machine name level reference (LOGIN → ... → GAME_LOOP)
- ✓ Track K sister network loader skeleton 측 cross-link (실 network operation 0)
- ✓ Track J Warden sister 측 cross-link
- ✓ EULA/ToS risk + Warden detection risk + multi-year horizon block-acknowledged 영구 documentation (L1-L5 SSOT)
- ✓ retired_intentional posture 적용 영역 분기 (D2R 는 본 cycle 측 retired_intentional 정식 채택 X — 별도 cycle 측 user lock-in 후 결정)

---

## 7. EULA/ToS risk acknowledgment (L1+L4 lock-in record, 영구 SSOT)

### 7.1 L1 — Blizzard EULA / ToS violation risk

- D2R 공식 client = Windows + Console only, **macOS unsupported**.
- 비공식 platform (Mac via Wine/CrossOver/community wrapper) 측 D2R online play 시도 = Blizzard EULA 측 unsupported platform 사용 = ToS violation surface.
- realm protocol implementation 시도 자체 = reverse-engineered wrapper 위에서 Blizzard 공식 endpoint 와 통신 = 직격 ToS violation path.
- airgenome-gamebox 측 위 영역 모두 영구 out-of-scope. 본 doc 의 §1 layer naming reference 는 high-level 명칭 only (실 통신 / 실 endpoint / 실 wire format X).

### 7.2 L4 — D2R online single-target lock-in (D4 unchanged)

- 본 online expansion 의 lock-in target 은 **D2R online single-target 한정**.
- D4 는 별도 cycle 측 `retired_intentional` 정식 채택 (always-online DRM family) — Track L 본 cycle 측 D4 측 status 변동 영구 X.
- 다른 Blizzard 게임 (WoW / Overwatch / Hearthstone / SC2 등) 도 본 cycle 측 scope 영역 X.

### 7.3 L2 — Warden cross-layer detection risk

- D2R online realm 측 anti-cheat = Battle.net Warden (cross-layer, L0/L1/L2 모두 측 검증).
- realm-level anomaly (예: stage 전이 timing 이상 / packet ordering 이상 / handshake 측 client-side 이상) 는 Warden 측 검출 surface 측면.
- 본 doc 는 위 검출 surface 측 회피 / 우회 / spoof 측 영구 out-of-scope. 검출 가능성 자체는 사용자 acknowledged.
- 세부 Warden 측 카테고리는 Track J `WARDEN_HONEST_FREEZE.md` sister doc 측 reference.

### 7.4 L3 — multi-year horizon

- online expansion 의 horizon 은 1-2년+ multi-cycle scope 사용자 acknowledged.
- 본 cycle (Track L) 은 그 long horizon 의 **첫 번째 doc-only freeze land** 일 뿐, 실 network operation / 실 protocol implementation 본 cycle 영구 X.

### 7.5 L5 — Track J/K/L 병행 BG land scope

- Track J = Warden honest freeze (anti-cheat sister doc)
- Track K = network primitive loader skeleton (PE-side, 실 network 0)
- Track L = D2R realm protocol honest freeze (본 cycle)
- 위 3 track 병행 BG land 사용자 acknowledged. 본 cycle 은 Track L 한정, Track J/K 측 modification 영구 X.

---


C1 — **wire-protocol implementation / packet construction / MITM / replay / private server / traffic shaping evasion / packet decryption 0건 명시**: 본 doc 의 어떤 부분도 위 카테고리 중 어느 것도 implement / cite / encourage 하지 않음. 본 doc 의 §1-§3 측 layer 명칭 / packet category 명칭 / stage 전이 명칭 reference 는 OS textbook 수준 + vendor 공식 자료 수준 + name-level only. 본 doc 측 위 forbidden 단어들의 등장은 **항상 negation context** (NO / 영구 X / 영구 out-of-scope / forbidden list / not provided) 에서만 사용. realm protocol 직접 시도는 ToS violation 직격 path 이므로 본 caveat 는 freeze doc 류에서 가장 강한 stance 로 명시됨.

C2 — **외부 공개 자료 의존 + 추정 영역 명시**: D2R 의 실 wire-format / handshake / encryption 측 내부 구현은 Blizzard 비공개. 본 doc 의 layer 4-division (Auth / Realm Selection / Game Realm / Warden cross-layer) 는 vendor 공식 자료 + 공개 textbook 수준의 추상 분류이며, 실 wire 측 layer 경계 / handshake 순서 / packet 측 layer 식별 방법 등은 본 doc 측 명시 영구 X. 본 doc 의 일부 추정 (e.g. "stage 전이 LOGIN → REALM_LIST → ... → GAME_LOOP") 도 일반 ARPG online architecture 의 textbook level 추정이지 실 D2R 측 정확한 stage 명칭 / 측 packet level 식별 / 측 trigger 측 wire 측 식별이 아님.

C3 — **community D2R wrapper / Wine / CrossOver 영역 측 import 0건**: §2.2 측 community wrapper (CrossOver D2R 동작 보고 / Wine staging D2R 측 patch / KR/EN community blog) 영역은 본 doc 측 cite 0건, gamebox 측 import 0건. 본 doc 는 그 영역의 **존재 사실 명시 only**, 그 영역의 코드 / pattern / patch list / blog post / repo URL 측 cite / import 영구 X.

C4 — **public protocol research project 측 cite 0건 (research direction citation only at name level)**: §3.1 측 community OSS server 측면 ("Project Diablo 2", "Slashdiablo" 류 명칭은 community 측 알려진 사례) — 본 doc 는 그 영역의 존재 사실만 명시, 해당 project 의 protocol table / wire format / repo URL / 측 cite 영구 X. 본 doc 의 stance 는 **research direction citation only at name level**, code import / wire format detail import 영구 X.

C5 — **L1-L5 lock-in 5 항목 모두 explicit emit (raw 12 silent_error_ban 정합)**: 본 doc §7 + marker top-level `online_expansion_lockin: { L1, L2, L3, L4, L5 }` block 측 5 항목 모두 explicit field. swallowed risk 0건. 사용자 acknowledged 측 lockin_iso = 2026-05-03.

C6 — **실 network operation 본 cycle 측 0건**: 본 cycle 의 land 는 **doc-only freeze**. 실 Battle.net endpoint connect 0건, 실 D2R realm packet 측 측정 0건, 실 protocol round-trip 0건. honest emit posture (Track I cleanest-cycle posture 정합).

C7 — **D2R `retired_intentional` 정식 채택 영역 X (Track E lineage_classic_r28 family 와 분기)**: Track E `GAMEGUARD_HONEST_FREEZE.md` 는 lineage_classic_r28 측 `retired_intentional` 정식 채택 evidence. 본 Track L 은 D2R 측 `retired_intentional` 정식 채택 X — online expansion lock-in path (사용자 user-confirm 진입) 측 high-level layer reference 만 documentation, mk2 status enum 측 marker 부여는 별도 cycle 측 user lock-in 후 결정.

C8 — **Track J `WARDEN_HONEST_FREEZE.md` sister 의 본 cycle 시점 land 시점 분기 가능**: Track J 는 본 cycle 과 parallel BG 진행. 본 doc 의 §5.1 cross-link table 측 Track J 측 reference 는 "parallel sister" 측면, 본 cycle 시점 Track J 의 land 완료 여부 (시점 차이) 는 본 doc 의 정합성에 영향 X — 두 doc 모두 land 후 cross-reference 가 양방향으로 충족됨.

C9 — **Track K `pe_winsock_ws2_32.hexa` / `pe_wininet_https.hexa` / `pe_schannel_tls.hexa` 측 land 시점 분기 가능**: Track K 는 본 cycle 과 parallel BG 진행. 본 doc 의 §5.3 cross-link table 측 Track K loader 측 reference 는 "parallel sister network primitive skeleton" 측면, 본 cycle 시점 Track K 의 land 완료 여부 (시점 차이) 는 본 doc 의 정합성에 영향 X — Track K 는 PE-side network primitive skeleton (실 network 0) 한정.

C10 — **own1 / own2 영구 enforcement (high-risk surface 특별 강조)**: 본 doc 측 + airgenome-gamebox 본체 측 own1 (Wine 0 / hexa-only / community wrapper 0 / protocol-impl 0 / MITM 0 / replay 0 / private server 0 / traffic shaping evasion 0) + own2 (honest emit / silent error 0 / 모든 L1-L5 lock-in explicit) 영구 정합. realm protocol 직접 시도는 다른 freeze doc (kernel-AC / cross-layer AC) 보다도 ToS violation 측 가장 직접적 surface 이므로, own1/own2 enforcement stance 는 본 cycle 의 가장 강한 lock 으로 명시됨.

C11 — **외부 cite 측 vendor 공식 / 공개 textbook level 한정**: 본 doc 의 §9 (출처) 측 cite 는 Blizzard 공식 + Battle.net 공식 + Wikipedia / OS textbook level 자료만. community OSS server / community blog / community wrapper / Wine staging patch / CrossOver bottle / 측 cite 영구 X.

C12 — **path / cross-link 측 repo-relative 정합 (raw 15 env_lazy)**: 본 doc 측 모든 path 는 repo-relative — `lib/...`, `docs/...`, `.roadmap.<game>`, `state/markers/...`. 사용자별 absolute path 0건, env() 측 lazy resolve 정합.

C13 — **본 cycle in_place_writes=0 cleanest-cycle posture (Track I mirror)**: 본 land 는 **3 NEW files only** (freeze doc + handoff + marker). 0 .roadmap 편집 / 0 lib/ 편집 / 0 native/ 편집 / 0 existing-file modifications. Track I `roadmap_op_self_impl` 의 cleanest-cycle posture 정합.

---

## 9. 출처 (외부 공개 자료, 2026-05-03)

본 doc 측 cite 는 vendor 공식 + Wikipedia / OS textbook level 자료 한정. community OSS server / community wrapper / 측 cite 영구 X.

- Blizzard Entertainment 공식 — https://www.blizzard.com/
- Battle.net 공식 — https://www.battle.net/
- Diablo II: Resurrected 공식 — https://diablo2.blizzard.com/
- Blizzard EULA / ToS — https://www.blizzard.com/legal/
- Diablo II: Resurrected Wikipedia — https://en.wikipedia.org/wiki/Diablo_II:_Resurrected
- Battle.net Wikipedia — https://en.wikipedia.org/wiki/Battle.net
- airgenome-gamebox sister doc (kernel-AC family precedent) — `docs/GAMEGUARD_HONEST_FREEZE.md`
- airgenome-gamebox sister doc (cross-layer AC family) — `docs/WARDEN_HONEST_FREEZE.md` (Track J parallel BG)
- airgenome-gamebox Track I cleanest-cycle posture reference — `state/markers/airgenome_gamebox_roadmap_op_self_impl_landed.marker`
- airgenome-gamebox Track-B parallel — `airgenome/doc/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md`
- airgenome-gamebox D4 retired_intentional family precedent — `airgenome/doc/airgenome_gamebox_d4_retired_intentional_adoption_landed_2026_05_03.ai.md`

---

*written 2026-05-03 — Track L D2R realm protocol honest freeze cycle. spec freeze land 완료 (no wire-protocol implementation / no packet construction / no MITM / no replay / no private realm server / no traffic shaping evasion / no packet decryption tooling). high-level layer naming reference + state-machine name-level stage 전이 + L1-L5 lock-in SSOT 한정. airgenome-gamebox 측 own1 (Wine 0 + protocol-impl 0 + MITM 0 + replay 0 + private server 0) + own2 (honest emit + 모든 lock-in explicit) 영구 정합 reflect.*
