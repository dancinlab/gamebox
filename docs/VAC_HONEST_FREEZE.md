# Valve Anti-Cheat (VAC) Honest Spec Freeze — user-mode anti-cheat reference

> **status**: 작성일 **2026-05-05** / **scope: HONEST SPEC FREEZE ONLY** — 본 문서는 Valve Anti-Cheat (VAC) user-mode anti-cheat 의 **공개 자료 기반 high-level reference** 이며, **bypass / evasion / circumvention 코드 / 패턴 / 시도 0건**.
> 작성: 2026-05-05 (Track CK VAC honest freeze cycle, predecessor Track C 4-game retired_intentional adoption + Track CC §9 deferred-mk1-narrative hook resolution)
> SSOT: `.roadmap.cs2` cond.3 (status `partial` 유지, retired_intentional posture documented)
> 결론 한줄: **VAC = Valve Corporation user-mode AC + Trusted Mode (CS2 specific), Apple Silicon macOS 측 CS2 native client 영구 미지원 (Steam client는 macOS 존재하나 CS2 macOS native build 없음) → gamebox 측 retired_intentional posture (block-acknowledged honest documentation = PASS-equivalent), 본 freeze doc 는 research-direction 외부 reference 만 명시 (own1 Wine 0 + own2 honest emit 정합)**.

---

## 0. 본 doc 의 목적과 한계 (가장 먼저 명시)

### 0.1 목적 (in-scope)

- VAC 의 vendor / product family / platform surface high-level **명세** 동결 (spec freeze)
- gamebox 측 `retired_intentional` posture rationale 영구 documentation
- `cs2` cond.3 (`docs/VAC_HONEST_FREEZE.md` evidence) 측 cross-link reference
- `c_anticheat_detection_track.hexa` (own1 detection-only, NO bypass) cross-link

### 0.2 한계 (out-of-scope, 영구)

본 doc 측 다음 카테고리는 **영구 out-of-scope** 이며, 어떤 형태의 implementation 도 gamebox 측 import 되지 않음:

- VAC signature scan 측 spoof / evasion 패턴
- VAC2 / VAC3 module enumeration 측 circumvention
- Steam Trusted Mode 측 bypass (CS2 lobby / matchmaking 우회)
- Steam ticket validation 측 circumvent (HWID 위조 / Steam ID 우회)
- VAC ban detection 측 evasion (delayed ban window 회피)
- 어떤 형태의 cheat / wallhack / aimbot / 자동화 signature 패턴
- 사용자 Steam 계정 / 게임 라이브러리 측 위협 행위

**본 doc 는 위 카테고리 중 어느 것도 implement 하지 않음**. 외부 공개 자료 (Valve 공식 + Steam 공식 + community research note) 의 요약 reference 일 뿐, 우회 코드 / 우회 모듈 / 우회 시나리오 작성 0건.

### 0.3 own1 / own2 alignment

- **own1 (Wine 0 / hexa-only)**: 본 doc + gamebox 본체 측 Wine / CrossOver / Whisky / GPTK fetch / import / wrapper 0줄. VAC 측 driver shim / Steam DRM emulation / Trusted Mode bypass 측 0줄.
- **own2 (log/error/emit/honest)**: VAC 측 honest emit 만 — `__CS2_VAC__ DETECTED` (외부 reference) + `__CS2_VAC__ RETIRED_INTENTIONAL` (block-acknowledged posture). silent error / 우회 시도 0건.

---

## 1. VAC scope (vendor + product family)

### 1.1 vendor 정보

| 항목 | 값 | 근거 (외부 공개) |
|---|---|---|
| 공식 명칭 | **Valve Anti-Cheat (VAC)** | Valve / Steam 공식 |
| 벤더 | **Valve Corporation** (US, Bellevue) | Valve 공식 |
| 첫 release | 2002 (Counter-Strike 1.6 시대) | 외부 공개 자료 |
| 현재 product line | VAC (legacy) / VAC2 / VAC3 / Steam Trusted Mode | Valve / Steam 공식 |
| 주요 customer | **Valve 자체 게임** (CS2 / Dota 2 / TF2 / L4D2 등) + **Steamworks 측 외부 게임** (opt-in) | Steam 공식 customer reference |

### 1.2 적용 게임 (외부 공개 기준, gamebox scope 내)

본 doc 측 cs2 외 적용 추정 게임 (**모두 gamebox 측 spec emit 영역 X — reference only**):

| game family | 적용 여부 | 비고 |
|---|---|---|
| **Counter-Strike 2** (Valve, 2023) | ✓ 적용 (VAC + Trusted Mode) | **본 cycle 의 1차 reference target** (cs2 retired_intentional 정식 채택, Track C) |
| Dota 2 (Valve) | ✓ 적용 (VAC) | gamebox per-game roadmap 측 별도 cycle 후보 |
| Team Fortress 2 (Valve) | ✓ 적용 (VAC) | per-game roadmap 측 별도 cycle 후보 |
| Source 2 engine 외부 게임 | △ Steamworks opt-in | 별도 enumerate cycle |
| Steamworks 측 외부 multiplayer 게임 다수 | ✓ opt-in (VAC Secure server) | per-game roadmap 측 enumerate 별도 cycle |

**cs2** 만 본 cycle 측 1차 target 이며, 다른 Valve / Steamworks family 게임은 본 doc 의 reference 영역 X (별도 cycle 측 enumerate 권고).

### 1.3 product 컴포넌트 (high-level, 공개 자료 기반)

| 컴포넌트 | mode | 역할 (외부 공개 요약) |
|---|---|---|
| `steamservice.exe` / `steamservice.dll` | user | Steam service host, VAC engine 측 launcher integration |
| `vac` 측 dynamic loader (런타임 fetch) | user | VAC module 측 server-side delivery (외부 공개: 측정 server-driven module dispatch) |
| Steam client (`steam.exe`) | user | parent process — VAC integration host + Trusted Mode gate |
| `steamclient64.dll` / `tier0_s64.dll` | user | Source 2 engine integration shim |
| **CS2 Trusted Mode** flag | user | CS2 specific — non-Trusted module 측 lobby/matchmaking gate |

**위 컴포넌트의 실 구현은 비공개**. 공개 자료 기반 high-level reference 만 명시. gamebox 측 컴포넌트 fetch / import / shim 0줄.

### 1.4 VAC 측 user-mode 본질 (vs kernel AC)

GameGuard / EAC kernel / Warden 와 달리 VAC 는 **user-mode AC** — kernel driver 의존 0. 의미:

- VAC 자체는 SCManager 통한 driver load path 미사용
- Steam service (user-mode) + CS2 process (user-mode) 측 상호 module enumerate
- Steam Trusted Mode = 추가 gate (CS2 specific): 비-Trusted module load 감지 시 lobby reject

**user-mode 본질 측 platform constraint 측 약함** (GameGuard kernel 보다 platform 종속도 낮음). 그러나 다른 영구 boundary 존재 (§2 참조).

---

## 2. Apple-only constraint (CS2 macOS 영구 미지원)

### 2.1 platform mismatch 본질

VAC 자체는 user-mode 이므로 platform mismatch 측 GameGuard kernel 만큼 강하지 않음. 그러나 **CS2 의 macOS native client 측 영구 미지원** 이 핵심 boundary:

| boundary | rationale (외부 공개) |
|---|---|
| **CS2 macOS native binary 측 미존재** | 2023 CS:GO → CS2 transition 시 Valve 측 macOS 32-bit native client retire (CS:GO 의 마지막 macOS build 가 CS2 launch 시 deprecated) |
| **CS2 Linux native client 측 미존재** | 동일 transition 측 Linux native build 도 retire |
| **64-bit macOS 측 Source 2 engine port 부재** | Apple Silicon 64-bit 측 Source 2 native build 영구 X (Valve 측 macOS gaming priority 매우 낮음) |
| **Steam macOS client 자체는 존재** | macOS Steam client 는 정상 동작; 단 CS2 game library 측 macOS "Not Available" 표시 |
| **VAC 측 Apple SIP / TCC 호환성 비공개** | macOS XNU 측 user-mode service 제약 (sandbox / TCC permission) 측 VAC integration 영향 비공개 |

### 2.2 Wine 0 path 영구

own1 enforcement 정합 — gamebox 측 Wine / CrossOver / Whisky / GPTK 0줄. CS2 측 Windows binary 측 Apple Silicon translation:

- Wine / CrossOver Hack 측 CS2 binary 측 부분 동작 reports 외부 community 측 존재 — gamebox **import 0건, 영구 미사용**
- CS2 + VAC + Trusted Mode 측 Wine layer 통과 시 ban 위험 (community report 기반) — gamebox 측 미관여 (own1 정합)
- Apple GPTK (Game Porting Toolkit) 측 일부 Source 2 게임 graphics translation 가능 — VAC Trusted Mode 측 별도 boundary

### 2.3 retired_intentional posture rationale

Apple Silicon macOS 환경에서 CS2 native client + VAC + Trusted Mode 통과 path = **영구 0** (CS2 macOS native binary 부재 + VAC ban 위험 + Trusted Mode gate). 따라서:

- gamebox 측 `cs2.cond.1` (cs2.exe PE parse + Steam API 64 offline shim) — production smoke 측 **retired_intentional 영구**
- gamebox 측 `cs2.cond.2` (Source 2 PSO genome harvest + tier0 plat shim) — 동일 retired_intentional
- gamebox 측 `cs2.cond.3` (VAC honest spec freeze) — **본 doc 측 evidence augment, status partial 영구 lock**
- block-acknowledged honest documentation = **PASS-equivalent** posture
- mk2 status enum `retired_intentional` 정식 채택 (2026-05-03 Track-C 측 cs2 lock-in 완료, 본 freeze doc 는 cond.3 측 evidence augment)

---

## 3. honest research-direction reference (no bypass code)

> **재경고**: 본 §3 는 외부 공개 자료의 high-level **명칭 / 카테고리 요약** reference 만. 본 doc 의 어떤 부분도 detection bypass / module evasion / signature spoof 방법을 명시하지 않으며, gamebox 측 import / implement 영역 X.

### 3.1 VAC user-mode scan category (high-level naming only)

VAC 가 사용한다고 외부 공개 자료에 언급된 user-mode scan category — **이름과 일반 설명만**:

| category | 일반 설명 (외부 공개) | bypass 정보 |
|---|---|---|
| **module enumeration** | 게임 process 측 loaded module list 측 known-bad signature 비교 | **없음 (본 doc 측 명시 X)** |
| **signature scan** | 메모리 영역 측 known cheat byte pattern scan | **없음** |
| **handle enumeration** | 게임 process 측 외부 process handle (OpenProcess) 측 enumerate | **없음** |
| **delayed ban window** | detection 시 즉시 ban 대신 일정 기간 지연 ban (의도적 정책) | **없음 (delayed ban 자체는 정책, evasion target 아님)** |

**위 category 의 bypass / evasion 방법은 본 doc 에서 다루지 않음**. 외부 공개 자료의 일반 설명만 reference.

### 3.2 CS2 Trusted Mode (high-level)

CS2 Trusted Mode 는 VAC 외 추가 layer:

- Trusted Mode flag = on 시: 알려지지 않은 module load = lobby reject
- Trusted Mode flag = off 시: VAC ban 위험 (외부 community report)
- Steam community matchmaking + competitive lobby 측 Trusted Mode 강제

**Trusted Mode bypass 방법은 본 doc 에서 다루지 않음**. CS2 native macOS 측 부재 + Trusted Mode 측 영구 boundary = retired_intentional posture 정합.

### 3.3 c_anticheat_detection_track 측 own1 detection-only posture

gamebox `lib/perf/c_anticheat_detection_track.hexa` 측 100% manjeom (skeleton-tier per BL/BM forensic). own1 enforcement:

- detection 측 honest emit (`__AC_DETECTION__` event prefix)
- bypass / evasion 측 0건
- VAC / GameGuard / EAC / Warden 측 cross-class detection scaffold (skeleton-tier; 실 game frame 측 측정 X)

본 module 측 VAC honest detection track 측 cross-link target. 실제 동작은 hexa interp closure CM-26..CM-30 land 후 (multi-year horizon).

---

## 4. cross-link to sibling honest freeze docs

VAC 측 sibling honest freeze docs 측 동일 own1 + own2 정합:

| doc | scope | track |
|---|---|---|
| `docs/GAMEGUARD_HONEST_FREEZE.md` | INCA Internet kernel-mode AC (NCSOFT family) | Track-E 2026-05-03 |
| `docs/WARDEN_HONEST_FREEZE.md` | Battle.net Warden user-mode AC (Blizzard family) | Track-E 2026-05-03 |
| `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` | D2R online realm protocol research-direction freeze | Track-E 2026-05-03 |
| **`docs/VAC_HONEST_FREEZE.md`** (this) | **Valve Anti-Cheat user-mode AC + Trusted Mode** | **Track CK 2026-05-05** |
| `docs/EAC_KERNEL_HONEST_FREEZE.md` (CL parallel) | Easy Anti-Cheat kernel + anti-tamper class | Track CL 2026-05-05 (sibling cycle) |

---

## 5. retired_intentional posture lock-in (gamebox)

본 doc 의 결론 ↔ gamebox `.roadmap.cs2` 정합:

- `cs2.blk.1` `status_enum_adopted: retired_intentional` (Track-C 4game retired_intentional adoption 2026-05-03)
- `cs2.cond.1/.2/.3` 측 `block_acknowledged_pass_equivalent` posture 영구 — 본 doc 측 cond.3 evidence row augment
- 사용자 lock-in 시점 + 본 freeze doc 측 land = **VAC retired_intentional posture 정식 lock-in 완료** (Track CK closure)

---

## 6. caveats (≥6 honest items)

C1. **본 doc 는 spec freeze ONLY**. bypass code / module / pattern / 시나리오 0건. 외부 공개 자료의 high-level reference 만.

C2. **VAC 의 user-mode 본질 측 GameGuard kernel 와 다름**. 그러나 CS2 의 macOS native binary 부재 + Trusted Mode gate 측 영구 boundary 측 동일 retired_intentional posture 정합.

C3. **VAC ban 위험 측 own1 정합**. Wine / CrossOver layer 통과 시 community report 측 ban 위험 reports 존재 — gamebox 측 미관여 (own1 Wine 0 + own2 honest emit, no AC bypass).

C4. **Steam 계정 위협 행위 0건**. 본 doc 의 어떤 부분도 사용자 Steam 계정 / library / inventory 측 위협 행위 명시 X.

C5. **Trusted Mode bypass 0건**. CS2 lobby/matchmaking 측 Trusted Mode 측 영구 우회 시도 X (own1).

C6. **delayed ban window 측 reference만**. delayed ban 자체는 Valve 측 정책 — 본 doc 측 evasion target 아님 (단순 reference).

C7. **c_anticheat_detection_track skeleton-tier**. own1 detection-only posture (Track BL/BM forensic 100% manjeom skeleton-tier — `validated_manjeom = 0` per raw#10 honest C3, 실 frame 측 측정 X).

C8. **CS2 macOS native build 부재 측 Valve 측 platform priority 결정 사항**. Apple Silicon 측 future port 가능성은 현재 alpha priority 낮음 (외부 공개 community discussion); gamebox 측 wait-and-see retired_intentional posture 정합.

C9. **본 doc 의 스타일 + 구조 측 GAMEGUARD_HONEST_FREEZE.md 측 mirror**. own2 traceability + cross-link consistency.

C10. **본 doc 측 CK marker land 측 final closure** = `.roadmap.cs2 cond.3` 측 partial 영구 lock 정합.

---

## 7. predecessors

- Track C (4-game retired_intentional adoption) — `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` (cs2 측 retired_intentional 정식 채택)
- Track AT (cs2 perf retired_intentional) — `state/markers/airgenome_gamebox_perf_cs2_retired_intentional_track_at_landed.marker` (cs2 perf scope retired_intentional annotation)
- Track CC §9 (per_game §9 closure) — `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_cc_landed.marker` (cs2 §9 측 본 doc deferred future-cycle 측 flagged)
- Track-E sibling freeze docs — GAMEGUARD / WARDEN / D2R_REALM_PROTOCOL honest freeze (2026-05-03)
- `docs/CS2_MANJEOM_REVIEW.md` mk1 narrative spec freeze (predecessor REUSE; CS2-specific manjeom doc)

---

## 8. forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

본 doc 측 honest spec freeze — `validated_manjeom` 측 변동 X. CS2 측 real frame 측 측정 X (CS2 macOS native binary 부재 + retired_intentional posture 영구 lock-in). README §Caveats item 2 (own1 invariant carries) + item 3 (L1-L5 lock-in) 정합.
