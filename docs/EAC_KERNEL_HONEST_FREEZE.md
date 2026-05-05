# Easy Anti-Cheat (EAC) Kernel + Anti-Tamper Honest Spec Freeze

> **status**: 작성일 **2026-05-05** / **scope: HONEST SPEC FREEZE ONLY** — 본 문서는 Easy Anti-Cheat (EAC) kernel-mode AC + anti-tamper variant 의 **공개 자료 기반 high-level reference** 이며, **bypass / evasion / circumvention 코드 / 패턴 / 시도 0건**.
> 작성: 2026-05-05 (Track CL EAC kernel honest freeze cycle, predecessor Track C 4-game retired_intentional adoption + Track CC §9 deferred-mk1-narrative hook resolution; sibling cycle to Track CK VAC freeze)
> SSOT: `.roadmap.delta_force` cond.3 + `.roadmap.elden_ring` cond.1 (status `partial` 유지, retired_intentional posture documented) + `.roadmap.elden_ring_nightreign` blk.1 (inherited)
> 결론 한줄: **EAC = Epic Online Services / Epic Games subsidiary kernel-mode AC + anti-tamper variant. Apple Silicon macOS 영구 미지원 platform (kernel driver class 동일 GameGuard mismatch + anti-tamper EOS handshake 영구 가능 X) → gamebox 측 retired_intentional posture (delta_force kernel + elden_ring anti-tamper + elden_ring_nightreign inherited 3-game class), 본 freeze doc 는 research-direction 외부 reference 만 명시 (own1 Wine 0 + own2 honest emit 정합)**.

---

## 0. 본 doc 의 목적과 한계 (가장 먼저 명시)

### 0.1 목적 (in-scope)

- EAC 의 vendor / product family / platform surface high-level **명세** 동결 (spec freeze)
- gamebox 측 `retired_intentional` posture rationale 영구 documentation (3 game cross-class)
- `delta_force` cond.3 + `elden_ring` cond.1 + `elden_ring_nightreign` blk.1 측 cross-link reference
- `c_anticheat_detection_track.hexa` (own1 detection-only, NO bypass) cross-link
- `c_elden_eac_kernel_driver_avoid_track.hexa` (own1 explicit honest "kernel driver avoid track") cross-link

### 0.2 한계 (out-of-scope, 영구)

본 doc 측 다음 카테고리는 **영구 out-of-scope** 이며, 어떤 형태의 implementation 도 gamebox 측 import 되지 않음:

- EAC kernel driver (`EasyAntiCheat.sys`) 측 hook detection bypass (SSDT / inline / IRP hook 회피 방법)
- EAC anti-tamper signature scan 측 spoof / evasion 패턴
- EAC EOS (Epic Online Services) ticket validation 측 circumvent (HWID 위조 / EOS account 우회)
- EAC module enumeration 측 circumvention (PEB Ldr 측 expected DLL list spoof)
- EAC process protection 측 circumvent (NtOpenProcess hook 회피)
- EAC kernel callback 측 우회 (PsSetCreateProcessNotifyRoutine* 우회)
- 어떤 형태의 cheat / botting / 자동화 signature 패턴
- 사용자 Epic Games 계정 / EOS account / game library 측 위협 행위
- FromSoft Online matchmaking server (elden_ring / nightreign multiplayer co-op) 측 우회

**본 doc 는 위 카테고리 중 어느 것도 implement 하지 않음**. 외부 공개 자료 (Epic 공식 + EOS 공식 + Wine/Proton community research note) 의 요약 reference 일 뿐, 우회 코드 / 우회 모듈 / 우회 시나리오 작성 0건.

### 0.3 own1 / own2 alignment

- **own1 (Wine 0 / hexa-only)**: 본 doc + gamebox 본체 측 Wine / CrossOver / Whisky / GPTK / Proton fetch / import / wrapper 0줄. EAC 측 driver shim / kernel emulation / anti-tamper bypass 측 0줄. NTOSKRNL emulation path 영구 X. EOS handshake emulation 영구 X.
- **own2 (log/error/emit/honest)**: EAC 측 honest emit 만 — `__ELDEN_EAC_OFFLINE_PROBE__` (외부 reference) + `__ELDEN_EAC_KERNEL_DRIVER_AVOID__ RETIRED_INTENTIONAL` (block-acknowledged posture, **own1 측 explicit "avoid" track 명시 — bypass 아닌 honest detection avoid**). silent error / 우회 시도 0건.

---

## 1. EAC scope (vendor + product family)

### 1.1 vendor 정보

| 항목 | 값 | 근거 (외부 공개) |
|---|---|---|
| 공식 명칭 | **Easy Anti-Cheat (EAC)** | Epic Games / EOS 공식 |
| 벤더 | **Easy Anti-Cheat** (Epic Online Services subsidiary, originally Kamu Solutions Oy 2018 acquisition) | Epic Games 공식 |
| 첫 release | 2006 (originally Kamu Solutions, FIN) | 외부 공개 자료 |
| Epic 인수 | 2018 | Epic Games 공식 발표 |
| 현재 product line | EAC kernel (legacy) / EAC for EOS (Epic Online Services integrated) / EAC anti-tamper (FromSoft variant) | EOS 공식 |
| 주요 customer | **FromSoftware** (Elden Ring + Sekiro + nightreign) + **Wargaming/Tencent/TiMi** (Delta Force + Squad + Battlefield 2042 등) + **Apex Legends** + **Fortnite (Epic 자체)** + 다수 multiplayer FPS / battle royale | EOS / EAC 공식 customer reference |

### 1.2 적용 게임 (외부 공개 기준, gamebox scope 내)

본 doc 측 gamebox 1차 reference target — **모두 gamebox 측 spec emit 영역 X — reference only**:

| game family | 적용 여부 | EAC 변종 | 비고 |
|---|---|---|---|
| **Delta Force** (TiMi/Wargaming/Tencent, 2024) | ✓ 적용 | EAC kernel | **delta_force.cond.3** 측 1차 reference target (Track-C 4game retired_intentional 정식 채택) |
| **Elden Ring** (FromSoft, 2022 + Shadow of the Erdtree DLC 2024-06-21) | ✓ 적용 | EAC anti-tamper variant | **elden_ring.cond.1** 측 1차 reference target (Track-C retired_intentional + Track BY DLC scope clarification) |
| **Elden Ring Nightreign** (FromSoft, 2025-05-30) | ✓ 적용 (EAC kernel + EAC matchmaking) | EAC anti-tamper inherited + co-op session lifecycle | **elden_ring_nightreign blk.1** (Track BY 2026-05-05 documentation-only fan-out, EAC class inherited) |
| Apex Legends (Respawn/EA) | ✓ 적용 (EAC kernel + EOS) | gamebox per-game roadmap 측 별도 cycle 후보 |
| Fortnite (Epic) | ✓ 적용 (EAC + Battleye dual) | per-game roadmap 측 별도 cycle 후보 |
| 기타 EOS / Steam multiplayer 게임 다수 | ✓ opt-in (EAC for EOS) | per-game roadmap 측 enumerate 별도 cycle |

**delta_force + elden_ring + elden_ring_nightreign** = 3-game cross-class 본 cycle 측 1차 target. 다른 EAC family 게임은 본 doc 의 reference 영역 X (별도 cycle 측 enumerate 권고).

### 1.3 product 컴포넌트 (high-level, 공개 자료 기반)

| 컴포넌트 | mode | 역할 (외부 공개 요약) |
|---|---|---|
| `EasyAntiCheat.sys` | **kernel** | kernel-mode driver core, 게임 process 보호 + signature scan + kernel callback registration |
| `EasyAntiCheat_x64.dll` / `EasyAntiCheat_x86.dll` | user | user-mode component, EAC engine + EOS handshake |
| `EasyAntiCheatLauncher.exe` | user | launcher integration shim (game launch wrapper) |
| `EasyAntiCheat_EOS.dll` (EOS variant) | user | EAC for EOS — Epic Online Services integration shim |
| `*.des` / config | user | client-side config (게임별 customization) |

**위 컴포넌트의 실 구현은 비공개**. 공개 자료 기반 high-level reference 만 명시. gamebox 측 컴포넌트 fetch / import / shim 0줄.

### 1.4 EAC variant matrix (delta_force / elden_ring / nightreign)

| variant | game family | kernel driver | anti-tamper | EOS handshake | matchmaking |
|---|---|---|---|---|---|
| **EAC kernel (legacy)** | delta_force, Apex (older) | ✓ | △ | △ | (별도 server) |
| **EAC anti-tamper (FromSoft)** | elden_ring base + Shadow of the Erdtree DLC | ✓ | ✓ (anti-tamper specialized) | △ | (FromSoft server) |
| **EAC + co-op matchmaking** | elden_ring_nightreign | ✓ inherited | ✓ inherited | △ | (FromSoft co-op session lifecycle, new sub-class) |
| **EAC for EOS (modern)** | Fortnite, Apex (newer), 기타 EOS 통합 게임 | ✓ | △ | ✓ (EOS 통합) | (EOS matchmaking) |

본 doc 측 1차 target = 3 variant 전부 (gamebox scope 내 delta_force kernel + elden_ring anti-tamper + nightreign inherited+co-op).

---

## 2. Apple-only constraint (macOS 영구 미지원, 3 cross-class)

### 2.1 platform mismatch 본질

EAC 의 kernel-mode driver 는 **Windows kernel API** 측 설계 — `NTOSKRNL` / `KeAttachProcess` / `PsLookupProcessByProcessId` / `IoCreateDevice` / `IRP_MJ_DEVICE_CONTROL` / `PsSetCreateProcessNotifyRoutine*` 등 Windows kernel-only primitive 의존. macOS 측 GameGuard 와 **동일 mismatch 영구** (참조: `docs/GAMEGUARD_HONEST_FREEZE.md` §2.1):

| Windows kernel surface | macOS Apple Silicon 측 mismatch |
|---|---|
| Driver loading (`SCManager` + `*.sys`) | macOS = SIP + signed driver (Apple notarized) 만 load 가능. Windows `*.sys` 직접 load path 영구 X |
| NTOSKRNL.exe primitive | macOS XNU kernel = Mach + BSD hybrid, Windows NT primitive 1:1 mapping 영구 X |
| SSDT (System Service Descriptor Table) | macOS = mach trap table + BSD syscall, SSDT 자체 미존재 |
| PEB / TEB / ETHREAD / EPROCESS | macOS = task / thread / proc struct, 1:1 mapping 영구 X |
| `PsSetCreateProcessNotifyRoutine*` | macOS = `kauth_listen_scope(KAUTH_SCOPE_PROCESS, ...)`, primitive 다름 + EAC 측 expected callback semantics mismatch |
| Driver signing (Microsoft EV cert) | macOS = Apple Developer ID + notarization, vendor 다름 |

### 2.2 EAC anti-tamper variant 측 추가 mismatch (elden_ring)

EAC 의 anti-tamper variant (FromSoft 측 elden_ring 적용) 는 위 kernel mismatch 외 **anti-tamper engine integrity check** 추가:

- 게임 binary 측 IAT integrity check (Windows PE format)
- 게임 binary 측 .text 영역 hash baseline (FromSoft engine specific)
- runtime 측 module hash 측 server-side validation (EOS handshake)
- Wine / Proton / GPTK 측 PE-format-translation = anti-tamper detection (외부 community report)

macOS Apple Silicon 측 PE binary 측 native execution path 영구 X (Wine 0 own1 정합) → anti-tamper integrity check 통과 path 영구 0.

### 2.3 nightreign 측 multiplayer co-op session 추가 boundary

elden_ring_nightreign 는 EAC anti-tamper inherited + 추가 multiplayer-specific surface:

- 3-player co-op session lifecycle = FromSoft online matchmaking server 의존
- co-op session init / sync state / session terminate 측 server-side validation
- offline mode 측 nightreign 측 매우 제한적 (외부 community report — single-player practice mode 정도)

**nightreign multiplayer surface = 별도 boundary** — EAC 외 + FromSoft matchmaking server gate 추가. 이 또한 own1 정합 (no FromSoft matchmaking server bypass).

### 2.4 Wine 0 path 영구

own1 enforcement 정합 — gamebox 측 Wine / CrossOver / Whisky / GPTK / Proton 0줄. EAC 측 emulation 가능성:

- Proton-EAC 측 일부 Linux-side support (Valve + Epic 측 Proton 측 EAC integration 2021-2022 announce) — gamebox **import 0건, 영구 미사용**
- Wine staging 측 EAC kernel driver shim 시도 — community 측 partial reports, gamebox **import 0건**
- Apple GPTK 측 EAC integration 부재 (graphics translation only, kernel AC scope 외)

Proton-EAC 의 Linux-only path 측 macOS 측 적용 불가 (Apple kernel driver model 다름).

### 2.5 retired_intentional posture rationale (3 cross-class)

Apple Silicon macOS 환경에서 EAC kernel attest + anti-tamper integrity 통과 path = **영구 0** (3 cross-class 모두). 따라서:

- **delta_force**: cond.1/cond.2/cond.3 production smoke 측 **retired_intentional 영구**
- **elden_ring**: cond.1/cond.2/cond.3 production smoke 측 **retired_intentional 영구** (base + Shadow of the Erdtree DLC 동일 EAC class 정합)
- **elden_ring_nightreign**: cond.1/cond.2/cond.3 + blk.1 (EAC inherited) + blk.2 (FromSoft co-op matchmaking 새 sub-class) — Track BY 2026-05-05 documentation-only fan-out
- block-acknowledged honest documentation = **PASS-equivalent** posture
- mk2 status enum `retired_intentional` 정식 채택 (2026-05-03 Track-C 측 delta_force + elden_ring lock-in + 2026-05-05 Track BY 측 nightreign 측 inherited fan-out, 본 freeze doc 는 cond evidence augment + cross-class narrative spec freeze)

---

## 3. honest research-direction reference (no bypass code)

> **재경고**: 본 §3 는 외부 공개 자료의 high-level **명칭 / 카테고리 요약** reference 만. 본 doc 의 어떤 부분도 detection bypass / hook circumvention / signature spoof / anti-tamper integrity bypass / EOS handshake circumvention 방법을 명시하지 않으며, gamebox 측 import / implement 영역 X.

### 3.1 EAC kernel hook category (high-level naming only)

EAC 가 사용한다고 외부 공개 자료에 언급된 kernel-level hook category — **이름과 일반 설명만**, 실제 구현 / 우회 / 회피 방법 X (참조: `docs/GAMEGUARD_HONEST_FREEZE.md` §3.1 mirror, EAC 측 동일 class):

| category | 일반 설명 (외부 공개) | bypass 정보 |
|---|---|---|
| **process callback registration** | `PsSetCreateProcessNotifyRoutine*` 측 process create/exit 감시 | **없음 (본 doc 측 명시 X)** |
| **thread callback registration** | `PsSetCreateThreadNotifyRoutine` 측 thread create/exit 감시 | **없음** |
| **image load callback** | `PsSetLoadImageNotifyRoutine` 측 module load 감시 | **없음** |
| **object callback (kernel object protection)** | `ObRegisterCallbacks` 측 process/thread handle access 감시 | **없음** |
| **kernel APC dispatch monitoring** | KeInsertQueueApc 측 user-mode APC injection 감시 | **없음** |

**위 category 의 bypass / evasion 방법은 본 doc 에서 다루지 않음**. 외부 공개 자료의 일반 설명만 reference.

### 3.2 EAC anti-tamper category (FromSoft variant)

EAC anti-tamper (elden_ring family) 측 user-mode integrity check 추가 — **이름과 일반 설명만**:

| category | 일반 설명 (외부 공개) | bypass 정보 |
|---|---|---|
| **PE IAT integrity** | game binary IAT (Import Address Table) 측 pre-load baseline 비교 | **없음** |
| **.text section hash** | game binary .text 영역 측 SHA-X hash baseline | **없음** |
| **runtime module hash** | runtime 측 loaded module 측 hash + server-side validation | **없음** |
| **debugger detection** | NtQueryInformationProcess / IsDebuggerPresent / RDTSC timing 등 다중 path | **없음** |
| **Wine/Proton detection** | Wine-specific NTDLL syscall pattern / 환경 변수 측 detection | **없음** |

### 3.3 EOS handshake (high-level)

EAC for EOS variant 측 추가 layer:

- EOS account ticket = client-side (game) ↔ server-side (EOS) handshake
- EAC module integrity = EOS server-validated
- Game launch flow = EAC launcher → EOS auth → game process

**EOS handshake circumvention 방법은 본 doc 에서 다루지 않음**. EOS account 측 위협 행위 0건 (own1).

### 3.4 c_elden_eac_kernel_driver_avoid_track 측 own1 explicit honest "avoid" posture

gamebox `lib/perf/c_elden_eac_kernel_driver_avoid_track.hexa` 측 100% manjeom (skeleton-tier per BL/BM forensic). Track-AS 2026-05-04 측 retired_intentional posture annotation 추가:

- `__ELDEN_EAC_KERNEL_DRIVER_AVOID__ RETIRED_INTENTIONAL elden_ring_eac_synthetic` honest emit
- "avoid" 측 own1 측 explicit 명시 — bypass 아닌 honest detection avoid posture
- 의미: gamebox 측 EAC kernel driver 측 통과 시도 X, 단순 detection 측 honest emit 만

본 module 측 EAC honest detection-avoid posture cross-link target. 실제 동작은 hexa interp closure CM-26..CM-30 land 후 (multi-year horizon).

### 3.5 c_anticheat_detection_track 측 own1 detection-only posture (cross-link)

gamebox `lib/perf/c_anticheat_detection_track.hexa` 측 cross-class detection scaffold (VAC + GameGuard + EAC + Warden 모두 cross-link). own1 detection-only — bypass / evasion 0건.

---

## 4. cross-link to sibling honest freeze docs

EAC 측 sibling honest freeze docs 측 동일 own1 + own2 정합:

| doc | scope | track |
|---|---|---|
| `docs/GAMEGUARD_HONEST_FREEZE.md` | INCA Internet kernel-mode AC (NCSOFT family) | Track-E 2026-05-03 |
| `docs/WARDEN_HONEST_FREEZE.md` | Battle.net Warden user-mode AC (Blizzard family) | Track-E 2026-05-03 |
| `docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md` | D2R online realm protocol research-direction freeze | Track-E 2026-05-03 |
| `docs/VAC_HONEST_FREEZE.md` | Valve Anti-Cheat user-mode AC + Trusted Mode | Track CK 2026-05-05 |
| **`docs/EAC_KERNEL_HONEST_FREEZE.md`** (this) | **Easy Anti-Cheat kernel + anti-tamper class (3 game cross-class)** | **Track CL 2026-05-05** |

---

## 5. retired_intentional posture lock-in (gamebox, 3 cross-class)

본 doc 의 결론 ↔ gamebox 정합 (3 game cross-class):

### 5.1 delta_force

- `delta_force.blk.1` `status_enum_adopted: retired_intentional` (Track-C 4game adoption 2026-05-03)
- `delta_force.cond.1/.2/.3` 측 `block_acknowledged_pass_equivalent` posture 영구 — 본 doc 측 cond.3 evidence row augment

### 5.2 elden_ring (base + Shadow of the Erdtree DLC)

- `elden_ring.blk.1` `status_enum_adopted: retired_intentional` (Track-C 4game adoption 2026-05-03)
- `elden_ring.cond.1/.2/.3` 측 `block_acknowledged_pass_equivalent` posture 영구
- DLC scope (Track BY 2026-05-05): 동일 EAC class — 별도 cond entries 미필요
- 본 doc 측 cond.1 evidence row augment

### 5.3 elden_ring_nightreign

- `elden_ring_nightreign.blk.1` (EAC anti-tamper inherited) + `blk.2` (FromSoft co-op matchmaking new sub-class) `status_enum_adopted: retired_intentional` (Track BY 2026-05-05 documentation-only fan-out)
- `elden_ring_nightreign.cond.1/.2/.3` 측 모두 retired_intentional inherited
- Lib module fan-out deferred future-cycle (`lib/loader/pe_elden_ring_nightreign_specific_track.hexa` + `lib/perf/c_nightreign_*.hexa`) — 본 doc 측 mk1 narrative spec freeze 측 evidence row

본 doc 측 Track CL closure = **EAC kernel + anti-tamper retired_intentional posture 정식 lock-in 완료** (3 game cross-class).

---

## 6. caveats (≥6 honest items)

C1. **본 doc 는 spec freeze ONLY**. bypass code / module / pattern / 시나리오 0건. 외부 공개 자료의 high-level reference 만.

C2. **EAC 의 kernel-mode 본질 측 GameGuard 와 동일 mismatch class**. macOS Apple Silicon 측 NTOSKRNL primitive 1:1 mapping 영구 X. 그러나 anti-tamper variant + EOS handshake 측 추가 boundary 존재.

C3. **3-game cross-class 측 동일 retired_intentional posture**. delta_force kernel + elden_ring anti-tamper + nightreign inherited+co-op — 모두 own1 Wine 0 + own2 honest emit 정합.

C4. **Proton-EAC Linux-only path 측 macOS 적용 불가**. Valve + Epic 측 Proton-EAC integration (2021-2022) 측 Linux-side만; Apple kernel driver model 다름.

C5. **FromSoft Online matchmaking server 측 nightreign 별도 boundary**. EAC 외 추가 sub-class — own1 매칭 server bypass 0건.

C6. **EOS account 위협 행위 0건**. 본 doc 의 어떤 부분도 사용자 Epic Games 계정 / EOS account / game library 측 위협 행위 명시 X.

C7. **anti-tamper integrity bypass 0건**. PE IAT / .text section hash / runtime module hash 측 evasion 방법 0건.

C8. **EAC kernel callback bypass 0건**. PsSetCreateProcessNotifyRoutine* / image load / object callback 측 우회 방법 0건.

C9. **`c_elden_eac_kernel_driver_avoid_track` 측 own1 explicit "avoid" posture**. bypass 아닌 honest detection avoid; gamebox 측 EAC kernel driver 측 통과 시도 X.

C10. **`c_anticheat_detection_track` 측 cross-class scaffold**. VAC + GameGuard + EAC + Warden 모두 cross-link. own1 detection-only (Track BL/BM forensic 100% manjeom skeleton-tier — `validated_manjeom = 0` per raw#10 honest C3, 실 frame 측 측정 X).

C11. **본 doc 의 스타일 + 구조 측 GAMEGUARD_HONEST_FREEZE.md + VAC_HONEST_FREEZE.md mirror**. own2 traceability + cross-link consistency.

C12. **Shadow of the Erdtree DLC 측 별도 cond entries 미필요**. DLC 동일 EAC class — Track BY scope clarification 측 narrative ref 만.

C13. **nightreign lib module fan-out deferred**. real binary acquisition + EAC class re-confirmation + 3-player co-op session lifecycle spec freeze 측 가능 시 future-cycle (CM+ candidate).

C14. **본 doc 측 CL marker land 측 final closure** = `.roadmap.delta_force cond.3` + `.roadmap.elden_ring cond.1` + `.roadmap.elden_ring_nightreign blk.1` 측 partial 영구 lock 정합.

---

## 7. predecessors

- Track C (4-game retired_intentional adoption) — `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` (delta_force + elden_ring 측 retired_intentional 정식 채택)
- Track AS (elden_ring perf retired_intentional) — `state/markers/airgenome_gamebox_perf_elden_retired_intentional_track_as_landed.marker` (c_elden_eac_kernel_driver_avoid_track + c_elden_eac_offline_launch_probe annotation)
- Track BE (d4 + elden completion) — `state/markers/airgenome_gamebox_perf_d4_elden_completion_track_be_landed.marker` (c_elden_* family 100% completion 4 modules)
- Track BM (perf v2) — `state/markers/airgenome_gamebox_perf_remaining_family_inventory_v2_track_bm_landed.marker` (c_sote_* DLC scaffold)
- Track BY (nightreign fan-out) — `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker` (nightreign documentation-only fan-out, EAC inherited)
- Track CC §9 — `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_cc_landed.marker` (delta_force + elden_ring §9 측 본 doc deferred future-cycle 측 flagged)
- Track CK (VAC sibling cycle) — `state/markers/gamebox_vac_honest_freeze_track_ck_landed.marker` (sibling cycle, VAC + EAC parallel cycle batch)
- Track-E sibling freeze docs — GAMEGUARD / WARDEN / D2R_REALM_PROTOCOL honest freeze (2026-05-03)
- `docs/DELTA_FORCE_MANJEOM_REVIEW.md` + `docs/ELDEN_RING_MANJEOM_REVIEW.md` mk1 narrative spec freeze (predecessor REUSE)

---

## 8. forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

본 doc 측 honest spec freeze — `validated_manjeom` 측 변동 X. delta_force / elden_ring / nightreign 측 real frame 측 측정 X (3 cross-class 모두 EAC kernel + anti-tamper retired_intentional posture 영구 lock-in). README §Caveats item 2 (own1 invariant carries) + item 3 (L1-L5 lock-in) 정합.

---

## 9. closure declaration (3 cross-class)

> **EAC kernel + anti-tamper honest spec freeze 측 final lock-in 완료** (Track CL 2026-05-05). delta_force kernel + elden_ring anti-tamper (base + Shadow of the Erdtree DLC) + elden_ring_nightreign inherited+co-op 3-game cross-class 측 retired_intentional posture 영구. own1 Wine 0 + own2 honest emit + no AC bypass / no kernel driver injection / no anti-tamper bypass / no EOS handshake circumvention / no FromSoft matchmaking bypass.
