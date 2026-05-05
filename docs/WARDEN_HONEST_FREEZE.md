# Warden (Blizzard user-mode anti-cheat) Honest Spec Freeze — D2R/D4/WoW/Overwatch reference

> **status**: 작성일 **2026-05-03** / **scope: HONEST SPEC FREEZE ONLY** — 본 문서는 Blizzard Warden user-mode anti-cheat 의 **공개 자료 기반 high-level reference** 이며, **bypass / evasion / circumvention / hook-chain shim / signature spoof / NOP-patch / memory-scan dodge 코드 / 패턴 / 시도 0건**.
> 작성: 2026-05-03 (Track-J Warden honest freeze cycle, sister cycle Track-E GameGuard kernel-mode counterpart + Track-L D2R realm protocol counterpart)
> SSOT: `.roadmap.battlenet` cond.3 (status `partial` 유지, online expansion lock-in posture documented)
> 결론 한줄: **Warden = Blizzard user-mode AC, Apple Silicon macOS 영구 미지원 platform → airgenome-gamebox 측 honest detection-vector tracking posture only (block-acknowledged + 자연 환경 정합 long-horizon research direction). 본 freeze doc 는 research-direction 외부 reference 만 명시 (own1 Wine 0 + own2 honest emit 정합)**.

---

## 0. 본 doc 의 목적과 한계 (가장 먼저 명시)

### 0.1 목적 (in-scope)

- Blizzard Warden user-mode anti-cheat 의 vendor / product family / detection surface high-level **명세** 동결 (spec freeze)
- airgenome-gamebox 측 online expansion lock-in posture (L1-L5) 영구 documentation — D2R online single-target lock-in + multi-year horizon + 자연 환경 정합 long-horizon
- `battlenet.cond.3` (`docs/WARDEN_HONEST_FREEZE.md` evidence) 측 cross-link reference
- Track-J detection-surface tracker module (`lib/perf/c_bnet_warden_environment_compat.hexa`) sister doc

### 0.2 한계 (out-of-scope, 영구)

본 doc 측 다음 카테고리는 **영구 out-of-scope** 이며, 어떤 형태의 implementation 도 airgenome-gamebox 측 import 되지 않음:

- Warden user-mode hook chain bypass / NOP-patch / inline patch 회피
- Warden memory scan 측 dodge / 회피 timing pattern
- Warden module enumeration 측 spoof (PEB Ldr loaded module list 위조)
- Warden process tree fingerprint 측 spoof / 위조
- Warden syscall sequence pattern 측 spoof / replay
- Warden file integrity hash 측 spoof / pre-image attack
- HWID / machine_id 위조 / spoof / forge
- 어떤 형태의 cheat / botting / 자동화 signature pattern
- Battle.net 계정 측 위협 행위 (계정 탈취 / fraud)

**본 doc 는 위 카테고리 중 어느 것도 implement 하지 않음**. 외부 공개 자료 (Glider lawsuit public docs / Wikipedia / community research note) 의 요약 reference 일 뿐, 우회 코드 / 우회 모듈 / 우회 시나리오 작성 0건.

### 0.3 own1 / own2 alignment

- **own1 (Wine 0 / hexa-only)**: 본 doc + airgenome-gamebox 본체 측 Wine / CrossOver / Whisky / GPTK 측 fetch / import / wrapper 0줄. Warden user-mode shim / hook-chain shim 측 0줄.
- **own2 (log/error/emit/honest)**: Warden 측 honest emit 만 — `__BNET_WARDEN__ DETECTION_TRACKED <vector>` (honest tracking posture only, NOT BYPASS/EVADE). silent error / 우회 시도 0건.

---

## 1. Warden scope (vendor + product family)

### 1.1 vendor 정보

| 항목 | 값 | 근거 (외부 공개) |
|---|---|---|
| 공식 명칭 | **Warden** (internal codename, public via Glider lawsuit 2008) | Blizzard 공식 (소송 공개 자료) |
| 벤더 | **Blizzard Entertainment, Inc.** (Activision Blizzard family) | 공식 |
| 첫 공개 reference | 2005 (WoW 출시 시점) / Glider lawsuit 2008 (MDY Industries v. Blizzard) | 외부 공개 자료 |
| 현재 product line | Battle.net launcher 측 통합 + 각 게임 내장 (D2R / D4 / WoW retail+classic / Overwatch / Hearthstone / SC2) | Blizzard 공식 |
| 주요 customer | Blizzard 자체 게임 family 전체 | 공식 |
| **kernel-mode 여부** | **NO — user-mode AC** (GameGuard / EAC / BattlEye 와 다름) | Glider lawsuit 공개 자료 |

### 1.2 적용 게임 (외부 공개 기준)

본 doc 측 D2R online (battlenet 통합) 외 적용 게임 (**모두 airgenome-gamebox 측 spec emit 영역 X — reference only**):

| game family | Warden 적용 여부 | 비고 |
|---|---|---|
| **Diablo 2: Resurrected (D2R) online** | ✓ 적용 (Battle.net realm) | **본 cycle 의 1차 reference target** (D2R online expansion lock-in 정식 채택) |
| Diablo 2: Resurrected offline | △ launcher 측 절차 통과 후 inert (offline mode 측 detection 미감지) | airgenome-gamebox 측 offline shim PASS (cond.1/cond.2 met) |
| Diablo 4 | ✓ 적용 (always-online) | retired_intentional 정식 채택 (별도 Track 측 lock-in, 본 cycle untouched) |
| World of Warcraft (retail+classic) | ✓ 적용 | per-game roadmap 별도 cycle 후보 |
| Overwatch / OW2 | ✓ 적용 | per-game roadmap 별도 cycle 후보 |
| Hearthstone | ✓ 적용 (외부 공개) | per-game roadmap 별도 cycle 후보 |
| StarCraft II | ✓ 적용 (외부 공개) | per-game roadmap 별도 cycle 후보 |

**diablo2_resurrected online** (battlenet realm 통합) 만 본 cycle 측 1차 target 이며, 다른 Blizzard family 게임은 본 doc 의 reference 영역 X (별도 cycle 측 enumerate 권고).

### 1.3 product 컴포넌트 (high-level, 공개 자료 기반)

| 컴포넌트 | mode | 역할 (외부 공개 요약) |
|---|---|---|
| Warden 측 in-process scanner | **user** | 게임 process 측 in-process memory scan + DLL list check + integrity hash |
| Battle.net launcher 측 ticket validation | user | OAuth 2-leg + ticket validate + HWID attest |
| Battle.net Agent 측 install integrity | user | 게임 binary + .pak file integrity hash check |
| Battle.net 측 server-side verification | server | client report 측 server-side cross-check + ban decision |

**위 컴포넌트의 실 구현은 비공개**. 공개 자료 (Glider lawsuit + Wikipedia level) 기반 high-level reference 만 명시. airgenome-gamebox 측 컴포넌트 fetch / import / shim 0줄.

### 1.4 GameGuard 와의 차이 (sister doc cross-link)

| 항목 | Warden (본 doc) | GameGuard (`docs/GAMEGUARD_HONEST_FREEZE.md`) |
|---|---|---|
| mode | **user-mode** | **kernel-mode** |
| 벤더 | Blizzard (in-house) | INCA Internet (3rd party) |
| 적용 platform | Blizzard family (D2R/D4/WoW/OW/HS/SC2) | NCSOFT family (Lineage Classic/Aion/B&S/L2) |
| Apple Silicon macOS 측 platform mismatch | unsupported platform (EULA violation) | unsupported platform (EULA violation + kernel driver 영구 X) |
| airgenome-gamebox posture | online expansion lock-in (L1-L5) — long-horizon natural env 정합 | retired_intentional (Track-B 정식 채택, 영구 X) |
| 본 doc 의 정합 sister | `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` (Track-L 측 network protocol counterpart) | `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` (mk1 narrative) |

**Warden = user-mode** 라는 점이 본 doc 의 핵심 차이 — kernel driver / .sys / SSDT hook 측 부재, 대신 in-process scan + ticket validation + server-side cross-check 측 surface.

---

## 2. Apple-only constraint (macOS 영구 미지원 D2R online)

### 2.1 platform mismatch 본질

Battle.net + D2R 의 공식 지원 platform 은 Windows + (일부 게임 한정) Apple Silicon macOS. 그러나 **D2R 자체는 macOS 측 공식 미지원** (Blizzard 공식 system requirement 측 Windows only):

| 측면 | Apple Silicon macOS 측 mismatch |
|---|---|
| Blizzard EULA / ToS | macOS 측 D2R 실행 = unauthorized platform = ToS 위반 가능 (계정 ban 가능) |
| Battle.net launcher | Battle.net 자체는 macOS 지원 (SC2 / WoW 측 native), but D2R 자체 launch path 측 macOS 부재 |
| Warden detection vector 측 unique fingerprint | 아래 §2.3 별도 enumerate |
| 자연 환경 정합 path | own1 (Wine 0) 측 Wine-style env signature 부재, 그러나 native Mac binary 도 부재 → **unique fingerprint** 가 양방향에서 발생 |

### 2.2 Wine 0 path 영구 (own1 enforcement)

own1 enforcement 정합 — gamebox 측 Wine / CrossOver / Whisky / GPTK 0줄. 따라서 **다음 두 가지 회피 path 측 영구 X**:

- **Wine staging 측 syscall emul import** — Wine 측 NTDLL syscall emul / wineloader.exe 측 LDR_DATA_TABLE entry 측 import 0건
- **CrossOver bottle 측 D2R 환경 import** — CrossOver bottle 측 fetch / import 0건

**그러나** Wine 0 path 가 곧 자연 환경 정합 (natural env alignment) 을 의미하지는 않음. 오히려:
- Wine env signature 측 absent (good for Wine detection)
- 그러나 native Windows process tree / NTDLL syscall path 측도 absent (bad for natural env alignment)
- 결과 = **unique fingerprint** (Wine 도 아니고 Windows 도 아닌 환경)

### 2.3 unique fingerprint enumeration (high-level only)

만약 Warden 이 본 환경 (Apple Silicon macOS native + own1 Wine 0) 에서 scan 한다면 honestly 관측될 unique fingerprint (high-level 명칭만, 회피 방법 / 회피 코드 0건): LDR_DATA_TABLE (`wineloader.exe` 측 entry 부재 + `kernel32.dll` 등 native Windows DLL entry 부재) / process tree shape (parent = launchd, not explorer.exe) / syscall sequence pattern (mach trap + BSD syscall, NTDLL syscall sequence 부재) / memory layout (XNU vm_map vs Windows VAD tree 부재) / HWID source (macOS IOPlatformUUID vs Windows MachineGuid 부재) / file integrity hash (macOS-style signature vs .pak Windows hash 부재).

위 vector 들은 본 cycle 의 sister module `lib/perf/c_bnet_warden_environment_compat.hexa` 측 7 detection-vector enumeration 과 정합 (module 측 row 로 emit). 모두 reference only — bypass X.

### 2.4 online expansion lock-in posture

retired_intentional posture (GameGuard family) 와 달리, Warden 측 본 cycle 의 **online expansion lock-in (a+ path)** 는 다음 5 항목으로 구성:

- **L1**: EULA / ToS violation risk acknowledged (Mac unauthorized platform — 계정 ban 위험)
- **L2**: Warden detection risk acknowledged (long-horizon 자연 환경 정합 측 미달성 시점에서 detection 가능성 acknowledged)
- **L3**: 1-2 year+ multi-cycle horizon acknowledged (자연 환경 정합 = long-horizon work)
- **L4**: D2R online single-target lock-in (D4 retired_intentional unchanged, 본 cycle 측 reverte X)
- **L5**: Track J/K/L scope locked-in for parallel BG land (본 cycle = Track J)

위 5 항목은 본 doc §7 측 SSOT 형태로 영구 documentation 되며, 본 cycle 측 marker 는 본 doc 의 §7 을 cross-reference.

---

## 3. honest research-direction reference (no bypass code)

> **재경고**: 본 §3 는 외부 공개 자료의 high-level **명칭 / 카테고리 요약** reference 만. 본 doc 의 어떤 부분도 detection bypass / hook chain bypass / signature spoof / circumvention 방법을 명시하지 않으며, airgenome-gamebox 측 import / implement 영역 X.

### 3.1 user-mode detection category (high-level naming only)

Warden 이 사용한다고 외부 공개 자료 (Glider lawsuit + Wikipedia + Russinovich Windows Internals) 에 언급된 user-mode detection category — **이름과 일반 설명만**, 실제 구현 / 우회 / 회피 방법 X:

| detection category | 일반 설명 (외부 공개 textbook level) | bypass code in this doc? |
|---|---|---|
| **in-process memory scan** | 게임 process 측 in-process memory periodic scan (signature scan) | **NO** — 이름 / 카테고리 reference 만 |
| **DLL list enumeration** | PEB Ldr 측 loaded module list expected list 검증 | **NO** — 이름 reference 만 |
| **process tree fingerprint** | parent / child process tree shape fingerprint | **NO** — 이름 reference 만 |
| **syscall sequence fingerprint** | NTDLL syscall sequence 측 expected pattern | **NO** — 이름 reference 만 |
| **file integrity hash** | 게임 binary + .pak hash check | **NO** — 이름 reference 만 |
| **anti-debug timing** | RDTSC / NtQueryInformationProcess + ProcessDebugFlags 측 timing | **NO** — 이름 reference 만 |
| **HWID / machine_id attest** | client-side H/W identifier 측 server-side validation | **NO** — 이름 reference 만 |

각 detection category 는 OS textbook (Russinovich Windows Internals / Solomon) 수준에서 공개된 정보이며, 본 doc 는 그 카테고리 명칭만 명기 — 구체 회피 방법 / pattern / 코드 / hook chain 측 0건.

### 3.2 kernel-mode hook 측 부재 (Warden vs GameGuard 차이)

**중요**: Warden 은 **user-mode AC** 이므로 GameGuard 의 SSDT / IRP / inline kernel hook 측 카테고리는 본 doc 측 영역 X. user-mode 측 IAT / EAT / inline user hook / page protect manipulation 측 사용 가능성 (외부 공개 자료 추정) 은 존재 — 본 doc 측 명칭 reference 만, 회피 방법 / 회피 코드 / 회피 pattern 측 본 doc + 본 모듈 측 영구 0줄.

### 3.3 본 doc 의 정확한 stance 명시

위 detection category + user-mode hook category 는 외부 공개 자료에 명시되어 있으며, 본 doc 는 그 카테고리들을 **명칭 / high-level 설명 수준에서만 reference**. 다음 영역은 본 doc + airgenome-gamebox 측 영구 X:

- 각 detection category 의 회피 방법 / 회피 코드 / 회피 pattern
- 각 user-mode hook category 의 spoof / 우회 / 시간차 회피 방법
- hook chain shim / IAT-EAT 위조 / inline user hook NOP-patch
- ticket validation 우회 path
- HWID / machine_id 위조 / spoof path
- memory scan dodge / signature spoof / pre-image attack
- 어떤 형태의 Warden 측 우회 / 회피 / circumvent 시도

본 doc 측 `bypass` / `evasion` / `circumvent` / `nop-patch` / `hook chain shim` / `memory scan dodge` 단어는 **항상 negation context** (e.g. "본 doc 는 ~~bypass code 를 포함하지 않음", "evasion 은 영구 out-of-scope", "circumvention 은 own1 위반", "nop-patch 측 영구 X", "hook chain shim 측 영구 X", "memory scan dodge 측 영구 X") 에서만 사용됨.

### 3.4 path forward = 자연 환경 정합 (NOT detection circumvention)

본 doc 의 핵심 메시지: Warden detection 통과 path 는 **회피 (bypass / evasion / circumvention) 가 아닌 자연 환경 정합** (natural environment alignment). 즉:

- **회피 path (out-of-scope, 영구 X)**: hook chain shim / signature spoof / memory scan dodge / NOP-patch / module list 위조
- **정합 path (in-scope, long-horizon)**: Win32 syscall perfection (Track-K 측 별도 cycle territory) — 실 Windows 환경과의 syscall / module / process tree fingerprint 자연 정합

**Track-K** = `lib/loader/pe_*.hexa` 측 syscall perfection (`pe_winsock_ws2_32` / `pe_wininet_https` / `pe_schannel_tls`) 측 long-horizon work — 본 cycle 측 untouched, 별도 cycle 측 land 권고. 본 doc 의 §6 (future-cycle non-goals) 측 in-scope 영역 명시.

### 3.5 Wine / community research direction 측 import 0건

KR / EN community + Wine staging branch 측 일부 사용자가 시도한다고 외부 공개 보고된 영역 (gamebox **import / 인용 / cite 0건** — 본 §3.5 는 영역 명시 only):

- Wine staging branch 측 일부 patch (kernel emul partial)
- CrossOver 측 일부 Blizzard 게임 동작 보고 (개별 사용자)
- 일부 community blog 측 Warden 동작 분석 보고 / Glider 후속 cheat tool 분석

**gamebox stance**: 위 영역의 코드 / 패턴 / patch / blog post 측 import 0건. 본 doc 도 URL / 자료 cite 측 vendor 공식 + 공개 textbook + 공식 lawsuit document 만 (§5 출처 참조). community bypass 자료 cite 0건.

---

## 4. own1 + own2 alignment (재확인)

### 4.1 own1 enforcement

| own1 항목 | airgenome-gamebox 측 reflect |
|---|---|
| Wine 0 | Wine wine-* binary / patch / module 0줄 — Warden 영역도 동일 |
| CrossOver 0 | CrossOver bottle / module 0줄 |
| Whisky 0 | Whisky wrapper 0줄 |
| GPTK 0 | Apple GPTK metal translation layer 0줄 |
| hexa-only | airgenome-gamebox 본체 + 본 doc + 본 module 측 hexa 외 import 0줄 |
| Warden bypass code 0 | 본 doc 측 + lib/ 측 bypass / shim / circumvent 코드 0줄 (§3 의 reference 는 명칭 only) |
| hook chain shim 0 | IAT/EAT/inline user hook 측 shim 0줄 |
| signature spoof 0 | HWID / machine_id 위조 코드 0줄 |
| memory scan dodge 0 | scan 시간차 회피 / signature 위조 코드 0줄 |
| NOP-patch 0 | function prologue 측 NOP / jmp 삽입 코드 0줄 |

### 4.2 own2 enforcement

- 본 doc 측 emit 패턴: `__BNET_WARDEN__ DETECTION_TRACKED <vector>` (honest tracking posture only, NOT BYPASS/EVADE/PASS_via_circumvention)
- `lib/perf/c_bnet_warden_environment_compat.hexa` self_test() 측 7 vector row emit (Track-J 측 본 cycle land):
  ```
  __BNET_WARDEN__ DETECTION_TRACKED memory_scan_periodicity
  __BNET_WARDEN__ DETECTION_TRACKED loaded_dll_list_check
  __BNET_WARDEN__ DETECTION_TRACKED process_tree_fingerprint
  __BNET_WARDEN__ DETECTION_TRACKED syscall_pattern_fingerprint
  __BNET_WARDEN__ DETECTION_TRACKED file_integrity_hash
  __BNET_WARDEN__ DETECTION_TRACKED hwid_machine_id_attest
  __BNET_WARDEN__ DETECTION_TRACKED anti_debug_timing
  ```
- silent error 0건 — 모든 Warden 측 detect / lock-in posture 는 honest emit 으로 노출
- swallowed error 0건 — block-acknowledged posture 측 graceful emit, 우회 시도 X

### 4.3 block-acknowledged risk per L2 lock-in

L2 lock-in (Warden detection risk acknowledged) = 본 doc 측 정식 acknowledge. 본 cycle 의 시점 (2026-05-03) 에서 Warden 검출 시 D2R online 측 ban 가능성은 high (unsupported platform 인 macOS 에서 D2R online 시도). L3 lock-in (1-2 year+ multi-cycle horizon) 에 따라 자연 환경 정합 (Track-K) 측 long-horizon work 가 완료되기 전까지 detection / ban risk 는 정식 acknowledged.

---

## 5. Cross-link table

본 doc 의 reference 측 cross-link (각 path repo-relative):

### 5.1 battlenet family

| path | role | relation |
|---|---|---|
| `.roadmap.battlenet` | game roadmap (mk2 SSOT) | cond.3 evidence row 측 본 freeze doc 추가 (Track-J land) |
| `.roadmap.diablo2_resurrected` | game roadmap (downstream) | D2R online expansion lock-in 측 cond.1 cross-reference (Track-J 측 untouched, 별도 Track L 측 D2R realm protocol counterpart land) |
| `lib/perf/c_bnet_warden_environment_compat.hexa` | perf sub-module (NEW Track-J) | 7 vector DETECTION_TRACKED self_test |
| `lib/perf/c_bnet_chat_state.hexa` | perf sub-module (Track-A 시점) | sister bnet perf module (untouched) |
| `lib/perf/c_bnet_login_state_track.hexa` | perf sub-module (Track-A) | sister (untouched) |
| `lib/perf/c_bnet_token_refresh_track.hexa` | perf sub-module (Track-A) | sister (untouched) |

### 5.2 sister freeze docs (3 cluster)

| path | role | mode |
|---|---|---|
| `docs/GAMEGUARD_HONEST_FREEZE.md` | sister freeze doc (Track-E) | **kernel-mode AC** (NCSOFT family) |
| `docs/WARDEN_HONEST_FREEZE.md` (본 doc) | freeze doc (Track-J) | **user-mode AC** (Blizzard family) |
| `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` | sister freeze doc (Track-L) | **network protocol** (D2R realm) |

3 freeze doc cluster 정합 — 각 doc 은 다른 surface (kernel AC / user AC / network protocol) 측 honest documentation, 모두 own1 (Wine 0) + own2 (honest emit) 정합.

### 5.3 retired_intentional adoption family vs online expansion lock-in family

| posture | example | freeze doc |
|---|---|---|
| **retired_intentional** (영구 X) | lineage_classic_r28 / d4 / cs2 (candidate) / delta_force (candidate) / elden_ring (candidate) | `docs/GAMEGUARD_HONEST_FREEZE.md` (kernel AC family) + 5 mk1 narrative |
| **online expansion lock-in** (a+ path, multi-year horizon) | **diablo2_resurrected online (본 cycle Track-J)** | **`docs/WARDEN_HONEST_FREEZE.md` (본 doc)** + Track-L sister freeze doc |

D2R online = 첫 online expansion lock-in 정식 채택 사례. retired_intentional 와는 다른 path — block-acknowledged but **자연 환경 정합 측 long-horizon work 가 in-scope** 라는 점이 핵심 차이.

### 5.4 predecessor markers + handoff

| marker / doc | role |
|---|---|
| `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` | per-game 11 fan-out (battlenet 진입점) |
| `state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker` | Track-A (offline shim cond.1/cond.2 met, 본 cycle 의 cond.3 source) |
| `state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker` | parallel — d4 retired_intentional (L4 unchanged 영역) |
| `state/markers/airgenome_gamebox_gameguard_honest_freeze_landed.marker` | sister cycle (Track-E, kernel-mode counterpart) |
| `docs/airgenome_gamebox_warden_honest_freeze_landed_2026_05_03.ai.md` | **본 cycle handoff (Track-J)** |
| `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker` | **본 cycle marker (Track-J)** |

---

## 6. future-cycle non-goals + in-scope (영구 lock)

본 doc 는 spec freeze 이며, 다음 영역은 **airgenome-gamebox lifetime 동안 영구 out-of-scope**:

### 6.1 user-mode bypass work (영구 X)

- Warden user-mode hook chain bypass / shim
- NOP-patch / inline patch 회피
- memory scan dodge / signature spoof / pre-image attack
- ticket validation 측 spoof / 우회
- HWID / machine_id 위조 / spoof
- module enumeration 측 spoof (PEB Ldr loaded module list 위조)
- process tree fingerprint / syscall sequence pattern / file integrity hash 측 spoof
- anti-debug 측 회피 (RDTSC timing spoof / NtQueryInformationProcess hook)
- Warden detection circumvention 의 어떤 형태도

### 6.2 platform-level work (영구 X)

- Apple Silicon kernel 측 Windows kernel emulation (NTOSKRNL 등 — Warden 은 user-mode 이지만 platform-level emulation 측 일관성 X)
- Rosetta 2 측 dynamic translation 측 detection 회피 (Rosetta 2 자체는 fine, but Warden detection 회피 목적 사용 X)
- macOS 측 Warden 동등 기능 implement (의도적으로 X)

### 6.3 in-scope (영구 영역, long-horizon work)

- ✓ **자연 환경 정합 (natural environment alignment)** — Win32 syscall perfection 측 Track-K cycle territory (별도 cycle, 본 cycle 측 untouched). 회피가 아닌, 실 Windows 환경과의 syscall / module / process tree fingerprint **honest 정합**.
- ✓ honest detection-vector tracking emit (`__BNET_WARDEN__ DETECTION_TRACKED <vector>` 측 client-side honest report) — 본 cycle module land 완료
- ✓ EULA / ToS violation risk acknowledgment maintenance (L1 lock-in)
- ✓ Warden detection risk acknowledgment maintenance (L2 lock-in)
- ✓ multi-year horizon acknowledgment (L3 lock-in)
- ✓ D2R online single-target lock-in maintenance (L4 — D4 retired_intentional unchanged)
- ✓ Track J/K/L parallel scope maintenance (L5)
- ✓ external public reference 의 high-level naming summary (본 doc 의 §3 수준)
- ✓ cross-link maintenance (sister freeze docs + Track-L D2R realm protocol counterpart + Track-K syscall perfection)

**중요**: in-scope 의 "자연 환경 정합" 은 Warden detection circumvention 과 **본질적으로 다름** — circumvention 은 detection 자체를 회피 (out-of-scope, 영구 X) / 정합은 detection 측 관측 surface 가 honest 하게 실 Windows 환경과 일치 (in-scope, long-horizon).

---

## 7. EULA / ToS risk acknowledgment (L1 + L2 lock-in record, SSOT)

본 §7 = airgenome-gamebox 측 online expansion lock-in 5 항목 (L1-L5) 의 **persistent SSOT**. 본 cycle marker (`state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker`) 는 본 §7 을 cross-reference.

| 항목 | 내용 |
|---|---|
| **L1** EULA/ToS risk acknowledged | Mac = Blizzard D2R 공식 미지원 platform → unauthorized platform → 계정 ban 가능 (Warden detection 시점에). 사용자 정식 acknowledged (2026-05-03). 회피 path 영구 X (회피하지 않고 acknowledged). |
| **L2** Warden detection risk acknowledged | Warden = user-mode AC multi-vector. 자연 환경 정합 (Track-K) long-horizon 미달성 시점 detection 가능성 high. 사용자 정식 acknowledged. detection 회피 (bypass / evasion) 영구 out-of-scope (§6.1). |
| **L3** 1-2 year+ multi-cycle horizon acknowledged | 자연 환경 정합 = long-horizon work. 본 cycle (Track-J) 는 long-horizon 의 **첫 cycle 일부** (detection-surface honest tracker + freeze doc). 단기 (1-3 month) D2R online PASS 기대 X. |
| **L4** D2R online single-target lock-in | 1차 target = **D2R online only**. 다른 Blizzard family (D4/WoW/OW) 측 본 cycle spec emit X. **D4 retired_intentional unchanged** — `state/markers/airgenome_gamebox_d4_retired_intentional_adoption_landed.marker` 영구 정합 lock-in. |
| **L5** Track J/K/L parallel scope locked-in | Track-J (본 cycle, Warden freeze + battlenet cond.3) + Track-K (loader Win32 syscall perfection — `pe_winsock_ws2_32` / `pe_wininet_https` / `pe_schannel_tls`, long-horizon) + Track-L (D2R realm protocol freeze). 각 track file scope disjoint — raw 9 hexa-only orchestration 정합. |
| **lockin_iso** | 2026-05-03 (5 항목 동시 lock-in 시점). 본 § = 5 항목 SSOT, marker 측 본 § cross-reference. |

---

## 8. raw#10 honest C3 caveats (≥ 10 items, 더 높은 risk surface 반영)

C1 — **bypass / evasion / circumvention / hook-chain shim / NOP-patch / memory-scan dodge 0건 명시**: 본 doc 의 어떤 부분도 Warden 우회 / 회피 / circumvent / hook chain shim / NOP-patch / memory scan dodge 방법을 제공하지 않음. §3 의 detection category / user-mode hook category reference 는 OS textbook + Glider lawsuit 공개 자료 수준의 명칭과 일반 설명만이며, 회피 방법 / 회피 코드 / 회피 pattern 0건. 모든 `bypass` / `evasion` / `circumvent` / `nop-patch` / `hook chain shim` / `memory scan dodge` 단어는 negation context 에서만 사용.

C2 — **외부 공개 자료 의존 + 추정 영역 명시**: Warden 의 실 구현은 Blizzard 비공개. 본 doc 의 컴포넌트 / detection category / user-mode hook category reference 는 외부 공개 자료 (Glider lawsuit / Wikipedia / OS textbook) 수준이며, 비공개 영역의 정확한 구현은 알려져 있지 않음. 본 doc 의 일부 추정 (e.g. "macOS 영구 mismatch") 도 platform 자체의 정의 차이 + 외부 공개 자료 기반의 추정.

C3 — **D2R online 외 적용 게임 enumerate 측 별도 cycle 권고**: §1.2 의 다른 Blizzard 게임 (D4 / WoW / OW / HS / SC2) 측 Warden 적용 여부는 외부 공개 reference 만이며, 본 cycle 측 spec emit X. 별도 cycle 측 per-game roadmap fan-out 시 추가 enumerate 권고. **D4 retired_intentional** 측 별도 Track 측 lock-in 완료, 본 cycle 측 reverte X.

C4 — **online expansion lock-in posture 의 의미 영구 lock**: L1-L5 5 항목은 mk2 schema 측 새 posture (retired_intentional 와 다름). block-acknowledged but **자연 환경 정합 측 long-horizon work 가 in-scope** 라는 점이 핵심. 본 doc §7 = 5 항목의 persistent SSOT. 다른 Blizzard 게임 측 동일 posture lock-in 권고는 **별도 cycle** 측 evaluate (D2R online 첫 사례, 다른 게임 측 자동 적용 X).

C5 — **Wine / community research direction 측 import 0건**: §3.5 측 KR / EN community + Wine staging 측 외부 시도 영역은 본 doc 측 cite 0건, gamebox 측 import 0건. 본 doc 는 그 영역의 존재 명시 only, 그 영역의 코드 / pattern / blog post 측 cite / import 영구 X.

C6 — **자연 환경 정합 (natural env alignment) ≠ detection circumvention 정합 명시**: §3.4 + §6.3 측 in-scope 영역 "자연 환경 정합" 은 Warden detection circumvention 과 **본질적으로 다름**. circumvention = detection 자체 회피 (out-of-scope, 영구 X) / 정합 = detection 측 관측 surface 가 honest 하게 실 Windows 환경과 일치 (in-scope, long-horizon, Track-K territory). 두 path 의 차이는 본 doc 의 핵심 메시지.

C7 — **cond.3 status 승격 영역 X**: 본 doc 의 land 는 `.roadmap.battlenet.cond.3` 측 evidence row 추가 (additive only) 에 그치며, status `partial` → `met` 승격 X. 이유: cond.3 측 production smoke (network endpoint live) + 자연 환경 정합 (Track-K + cycle 2+) 측 met gates 영구 미달성 — Track-J 측 Warden honest freeze + detection-surface tracker 만으로는 cond.3 met 카테고리 영구 X (additive only).

C8 — **own1 / own2 영구 enforcement**: 본 doc 측 + lib/perf/c_bnet_warden_environment_compat.hexa 측 + airgenome-gamebox 본체 측 own1 (Wine 0 / hexa-only / Warden bypass code 0 / hook chain shim 0 / signature spoof 0 / NOP-patch 0 / memory scan dodge 0) + own2 (honest emit `__BNET_WARDEN__ DETECTION_TRACKED <vector>` / silent error 0) 영구 정합. 본 freeze doc 는 그 enforcement 의 1 차 standalone reference (user-mode AC family 측).

C9 — **Battle.net 계정 / KR/EN live server 영역 영구 out-of-scope**: 본 doc 측 / airgenome-gamebox 본체 측 Blizzard Battle.net 계정 측 OAuth / live server connect / HWID 등록 / 계정 위협 행위 시도 0건. live server measurement 측 영역은 사용자 lock-in (L1+L2 acknowledged) 후 사용자 자체 책임 영역.

C10 — **외부 cite 측 vendor 공식 / 공개 textbook / 공식 lawsuit 한정**: 본 doc 의 §9 (출처) 측 cite 는 Blizzard 공식 + Wikipedia / OS textbook + Glider lawsuit (MDY Industries v. Blizzard, 2008) 공개 자료만. KR / EN community blog / Wine staging patch / CrossOver bottle 측 cite 0건. 우회 자료 cite 영구 X.

C11 — **D4 retired_intentional unchanged + d2r online lock-in 동시 양립**: L4 lock-in 의 핵심 = D2R online single-target lock-in **이면서** D4 retired_intentional **unchanged**. 두 posture 가 동시 양립 (mk2 schema 측 status_enum + online_expansion_lockin 두 영역 분리 가능). 본 cycle 측 D4 측 reverte X — `.roadmap.diablo4` 측 untouched.

C12 — **Track J/K/L parallel scope 측 file scope disjoint**: 본 cycle (Track-J) 의 file scope 는 본 prompt 측 명시한 5 file (`lib/perf/c_bnet_warden_environment_compat.hexa` 신규 + `docs/WARDEN_HONEST_FREEZE.md` 신규 + `docs/airgenome_gamebox_warden_honest_freeze_landed_2026_05_03.ai.md` 신규 handoff + `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker` 신규 marker + `.roadmap.battlenet` cond.3 surgical edit) 한정. Track-K (loader 측) + Track-L (D2R realm 측) 측 file 측 untouched 영구 정합.

C13 — **본 doc 의 path / cross-link 측 repo-relative 정합 (raw 15 env_lazy)**: 본 doc 측 모든 path 는 repo-relative — `lib/...`, `docs/...`, `.roadmap.<game>`, `state/markers/...`. 사용자별 absolute path 0건, env() 측 lazy resolve 정합.

C14 — **본 cycle 의 silent-land + raw 168 minimum-viable additive**: 본 cycle 측 hexa CLI runtime emit 미verify (predecessor cycle posture 동일), schema-validated only. sha256 pin + python3 json.tool parse + wc -l + grep audit 측 verification.

---

## 9. 출처 (외부 공개 자료, 2026-05-03)

본 doc 측 cite 는 vendor 공식 + Wikipedia / OS textbook level 자료 + 공식 lawsuit document 한정. 우회 / community bypass 자료 cite 영구 X.

- Blizzard Entertainment 공식 — https://www.blizzard.com/
- Battle.net 공식 — https://www.battle.net/
- Diablo 2: Resurrected 공식 — https://diablo2.blizzard.com/
- MDY Industries, LLC v. Blizzard Entertainment, Inc. (2008-2010, Glider lawsuit public records) — Warden 측 공개 자료 1차 출처
- Warden anti-cheat Wikipedia — https://en.wikipedia.org/wiki/Warden_(software)
- Windows Internals (Russinovich / Solomon) — user-mode hook category 일반 설명 reference
- airgenome-gamebox sister freeze doc (kernel-mode counterpart) — `docs/GAMEGUARD_HONEST_FREEZE.md` (Track-E 2026-05-03 land 완료)
- airgenome-gamebox sister freeze doc (network protocol counterpart) — `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` (Track-L 측 parallel BG, 본 cycle 측 untouched)
- airgenome-gamebox sister manjeom (always-online DRM family) — `docs/D4_MANJEOM_REVIEW.md`
- airgenome-gamebox Track-A handoff (predecessor) — `docs/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md`
- airgenome-gamebox Track-E handoff (sister cycle) — `docs/airgenome_gamebox_gameguard_honest_freeze_landed_2026_05_03.ai.md`

---

*written 2026-05-03 — Track-J Warden honest freeze cycle. spec freeze land 완료 (no bypass / no evasion / no circumvention / no hook-chain shim / no NOP-patch / no memory-scan dodge / no signature spoof). battlenet cond.3 측 evidence augment additive (block-acknowledged online expansion lock-in posture L1-L5 영구 정합). airgenome-gamebox 측 own1 (Wine 0) + own2 (honest emit `__BNET_WARDEN__ DETECTION_TRACKED <vector>`) 영구 정합 reflect. 자연 환경 정합 (Track-K Win32 syscall perfection) 측 long-horizon work in-scope, detection circumvention 영구 out-of-scope.*
