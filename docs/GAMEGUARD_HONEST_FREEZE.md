# GameGuard (INCA Internet) Honest Spec Freeze — kernel-mode anti-cheat reference

> **status**: 작성일 **2026-05-03** / **scope: HONEST SPEC FREEZE ONLY** — 본 문서는 GameGuard kernel-mode anti-cheat 의 **공개 자료 기반 high-level reference** 이며, **bypass / evasion / circumvention 코드 / 패턴 / 시도 0건**.
> 작성: 2026-05-03 (Track-E GameGuard honest freeze cycle, predecessor Track-B 별도 cycle hook)
> SSOT: `.roadmap.lineage_classic_r28` cond.3 (status `partial` 유지, retired_intentional posture documented)
> 결론 한줄: **GameGuard = INCA Internet kernel-mode AC, Apple Silicon macOS 영구 미지원 platform → airgenome-gamebox 측 retired_intentional posture (block-acknowledged honest documentation = PASS-equivalent), 본 freeze doc 는 research-direction 외부 reference 만 명시 (own1 Wine 0 + own2 honest emit 정합)**.

---

## 0. 본 doc 의 목적과 한계 (가장 먼저 명시)

### 0.1 목적 (in-scope)

- GameGuard 의 vendor / product family / platform surface high-level **명세** 동결 (spec freeze)
- airgenome-gamebox 측 `retired_intentional` posture rationale 영구 documentation
- `lineage_classic_r28` cond.3 (`docs/GAMEGUARD_HONEST_FREEZE.md` evidence) 측 cross-link reference

### 0.2 한계 (out-of-scope, 영구)

본 doc 측 다음 카테고리는 **영구 out-of-scope** 이며, 어떤 형태의 implementation 도 airgenome-gamebox 측 import 되지 않음:

- GameGuard kernel driver 측 hook detection bypass (SSDT / inline / IRP hook 회피 방법)
- GameGuard signature scan 측 spoof / evasion 패턴
- GameGuard ticket validation 측 circumvent (HWID 위조 / machine_id 우회)
- GameGuard module enumeration 측 circumvention (PEB Ldr 측 expected DLL list spoof)
- GameGuard process protection 측 circumvent (NtOpenProcess hook 회피)
- 어떤 형태의 cheat / botting / 자동화 signature 패턴
- 사용자 계정 (NCSOFT plaync / KR account / HWID) 측 위협 행위

**본 doc 는 위 카테고리 중 어느 것도 implement 하지 않음**. 외부 공개 자료 (벤더 공식 + Wine/CrossOver community research note) 의 요약 reference 일 뿐, 우회 코드 / 우회 모듈 / 우회 시나리오 작성 0건.

### 0.3 own1 / own2 alignment

- **own1 (Wine 0 / hexa-only)**: 본 doc + airgenome-gamebox 본체 측 Wine / CrossOver / Whisky / GPTK fetch / import / wrapper 0줄. GameGuard 측 driver shim / kernel emulation 측 0줄. NTOSKRNL emulation path 영구 X.
- **own2 (log/error/emit/honest)**: GameGuard 측 honest emit 만 — `__LC_R28_GG__ DETECTED` (kernel mode 측 외부 reference) + `__LC_R28_GG__ RETIRED_INTENTIONAL` (block-acknowledged posture). silent error / 우회 시도 0건.

---

## 1. GameGuard scope (vendor + product family)

### 1.1 vendor 정보

| 항목 | 값 | 근거 (외부 공개) |
|---|---|---|
| 공식 명칭 | **nProtect GameGuard** | INCA Internet 공식 |
| 벤더 | **INCA Internet Co., Ltd.** (KR 본사) | INCA Internet 공식 |
| 첫 release | 2002 (KR online game 시장 초기) | 외부 공개 자료 |
| 현재 product line | nProtect GameGuard (PC) / GameGuard for Mobile / nProtect Online Security | INCA Internet 공식 |
| 주요 customer | NCSOFT (Lineage 시리즈 family) + KR/JP/CN MMO 다수 + 일부 글로벌 MMO | 공식 customer reference (외부 공개) |

### 1.2 적용 게임 (외부 공개 기준)

본 doc 측 lineage_classic_r28 외 적용 추정 게임 (**모두 airgenome-gamebox 측 spec emit 영역 X — reference only**):

| game family | 적용 여부 | 비고 |
|---|---|---|
| **Lineage Classic R28** (NCSOFT, 2026-02-07) | ✓ 적용 | **본 cycle 의 1차 reference target** (lineage_classic_r28 retired_intentional 정식 채택) |
| Lineage 2 (구) | ✓ 적용 | repo 측 `pe_lineage_2_specific_track.hexa` legacy module 잔존 (Track-B untouched) |
| Aion (NCSOFT) | ✓ 적용 (외부 공개) | airgenome-gamebox per-game roadmap 측 별도 cycle 후보 |
| Blade & Soul (NCSOFT) | ✓ 적용 (외부 공개) | per-game roadmap 측 별도 cycle 후보 |
| 기타 KR MMO (다수) | ✓ 적용 (외부 공개) | per-game roadmap 측 enumerate 별도 cycle |
| Lineage M / Lineage W (NCSOFT, Purple shell) | △ Purple shell layer 측 검출, GameGuard 직적용 X | Purple shell 통합 (별도 family) |

**lineage_classic_r28** 만 본 cycle 측 1차 target 이며, 다른 NCSOFT family 게임은 본 doc 의 reference 영역 X (별도 cycle 측 enumerate 권고).

### 1.3 product 컴포넌트 (high-level, 공개 자료 기반)

| 컴포넌트 | mode | 역할 (외부 공개 요약) |
|---|---|---|
| `npggNT.des` / `GameMon64.des` | **kernel** | kernel-mode driver core, 게임 process 보호 + signature scan |
| `NPGameMon.sys` (구 명칭) | kernel | legacy kernel driver (구 release line) |
| `GameGuard.des` / `npgmup.des` | user | user-mode component, ticket validation + launcher integration |
| `*.ini` / config | user | client-side config (게임별 customization) |

**위 컴포넌트의 실 구현은 비공개**. 공개 자료 기반 high-level reference 만 명시. airgenome-gamebox 측 컴포넌트 fetch / import / shim 0줄.

---

## 2. Apple-only constraint (macOS 영구 미지원)

### 2.1 platform mismatch 본질

GameGuard 의 kernel-mode driver 는 **Windows kernel API** 측 설계 — `NTOSKRNL` / `KeAttachProcess` / `PsLookupProcessByProcessId` / `IoCreateDevice` / `IRP_MJ_DEVICE_CONTROL` 등 Windows kernel-only primitive 의존. macOS 측 다음 mismatch 영구:

| Windows kernel surface | macOS Apple Silicon 측 mismatch |
|---|---|
| Driver loading (`SCManager` + `*.sys`) | macOS = SIP + signed driver (Apple notarized) 만 load 가능. Windows `*.sys` 직접 load path 영구 X |
| NTOSKRNL.exe primitive | macOS XNU kernel = Mach + BSD hybrid, Windows NT primitive 1:1 mapping 영구 X |
| SSDT (System Service Descriptor Table) | macOS = mach trap table + BSD syscall, SSDT 자체 미존재 |
| PEB / TEB / ETHREAD / EPROCESS | macOS = task / thread / proc struct, 1:1 mapping 영구 X |
| `KdDisableDebugger` / kernel debugger | macOS = kdp + dtrace, primitive 다름 |
| Driver signing (Microsoft EV cert) | macOS = Apple Developer ID + notarization, vendor 다름 |

### 2.2 Wine 0 path 영구

own1 enforcement 정합 — gamebox 측 Wine / CrossOver / Whisky / GPTK 0줄. 따라서 **NTOSKRNL emulation path 영구 X** (Wine 의 `wine-*` syscall emul / `ntoskrnl.exe` user-mode shim 측 import 0건).

GameGuard kernel driver 측 emulation 가능성:
- Wine 측 wine-staging 일부 patch (KR community 일부 시도) — gamebox **import 0건, 영구 미사용**
- DXVK / VKD3D 같은 graphics layer 무관 (GameGuard 는 kernel mode AC, graphics surface 와 별도)
- Apple GPTK (Game Porting Toolkit) 도 kernel AC 측 영역 무관 (graphics translation 만)

### 2.3 retired_intentional posture rationale

Apple Silicon macOS 환경에서 GameGuard kernel attest 통과 path = **영구 0**. 따라서:

- gamebox 측 `lineage_classic_r28.cond.1` (LineageClassic.exe + GameGuard probe) — production smoke 측 **retired_intentional 영구**
- block-acknowledged honest documentation = **PASS-equivalent** posture
- mk2 status enum `retired_intentional` 정식 채택 (2026-05-03 Track-B 측 lineage_classic_r28 첫 사례 lock-in 완료, 본 freeze doc 는 cond.3 측 evidence augment)

---

## 3. honest research-direction reference (no bypass code)

> **재경고**: 본 §3 는 외부 공개 자료의 high-level **명칭 / 카테고리 요약** reference 만. 본 doc 의 어떤 부분도 detection bypass / hook circumvention / signature spoof 방법을 명시하지 않으며, airgenome-gamebox 측 import / implement 영역 X.

### 3.1 kernel hook category (high-level naming only)

GameGuard 가 사용한다고 외부 공개 자료에 언급된 kernel-level hook category — **이름과 일반 설명만**, 실제 구현 / 우회 / 회피 방법 X:

| hook category | 일반 설명 (Wikipedia / public textbook level) | bypass code in this doc? |
|---|---|---|
| **SSDT hook** | System Service Descriptor Table 측 syscall pointer 변경 | **NO** — 이름 / 카테고리 reference만 |
| **inline hook** | function prologue 측 jmp instruction 삽입 | **NO** — 이름 reference 만 |
| **IRP hook** | I/O Request Packet 측 dispatch routine 가로채기 | **NO** — 이름 reference 만 |
| **IAT hook** | Import Address Table 측 entry 변경 (user-mode) | **NO** — 이름 reference 만 |
| **EAT hook** | Export Address Table 측 entry 변경 | **NO** — 이름 reference 만 |
| **page protect manipulation** | memory page 측 protect bit 변경 | **NO** — 이름 reference 만 |

각 hook category 는 OS textbook (Russinovich Windows Internals / Solomon) 수준에서 공개된 정보이며, 본 doc 는 그 카테고리 명칭만 명기 — 구체 회피 방법 / pattern / 코드 / IOCTL 호출 방법 X.

### 3.2 detection vector (high-level naming only)

GameGuard 측 client process 검출 vector 는 외부 공개 자료에 일부 언급됨 — 본 doc 는 카테고리 이름과 일반 설명만, 회피 방법 0건:

| detection vector | 일반 설명 | 본 doc 의 stance |
|---|---|---|
| process protection | client process 측 NtOpenProcess / NtTerminateProcess 차단 | reference only — bypass X |
| module enumeration | PEB Ldr 측 loaded module list 측 expected list 검증 | reference only — bypass X |
| memory signature scan | client memory 측 알려진 cheat 패턴 scan | reference only — bypass X |
| anti-debug | NtQueryInformationProcess + ProcessDebugFlags / RDTSC timing 측 검출 | reference only — bypass X |
| HWID + machine_id | client 측 H/W identifier 측 server-side validation | reference only — bypass X |
| file integrity | 게임 binary + .pak hash check | reference only — bypass X |
| anti-DLL injection | unauthorized DLL inject 측 차단 | reference only — bypass X |

### 3.3 본 doc 의 정확한 stance 명시

위 hook category + detection vector 는 외부 공개 자료에 명시되어 있으며, 본 doc 는 그 카테고리들을 **명칭 / high-level 설명 수준에서만 reference**. 다음 영역은 본 doc + airgenome-gamebox 측 영구 X:

- 각 hook category 의 회피 방법 / 회피 코드 / 회피 pattern
- 각 detection vector 의 spoof / 우회 / 시간차 회피 방법
- kernel driver shim / NTOSKRNL emulation
- ticket validation 우회 path
- HWID / machine_id 위조 / spoof path
- 어떤 형태의 GameGuard 측 우회 / 회피 / circumvent 시도

본 doc 측 `bypass` / `evasion` / `circumvent` 단어는 **항상 negation context** (e.g. "본 doc 는 ~~bypass code 를 포함하지 않음", "evasion 은 영구 out-of-scope", "circumvention 은 own1 위반") 에서만 사용됨.

### 3.4 Wine / CrossOver community research direction (외부 reference, import 0건)

KR community + Wine / CrossOver 측 일부 사용자가 시도한다고 외부 공개 보고된 영역 (gamebox **import / 인용 / cite 0건** — 본 §3.4 는 영역 명시 only):

- Wine staging branch 측 일부 patch (kernel emul partial)
- CrossOver 측 일부 NCSOFT 게임 동작 보고 (개별 사용자)
- 일부 KR community blog 측 GameGuard 동작 분석 보고

**gamebox stance**: 위 영역의 코드 / 패턴 / patch / blog post 측 import 0건. 본 doc 도 URL / 자료 cite 측 vendor 공식 + 공개 textbook 만 (§5 출처 참조). community bypass 자료 cite 0건.

---

## 4. own1 + own2 alignment (재확인)

### 4.1 own1 enforcement

| own1 항목 | airgenome-gamebox 측 reflect |
|---|---|
| Wine 0 | Wine wine-* binary / patch / module 0줄 — GameGuard 영역도 동일 |
| CrossOver 0 | CrossOver bottle / module 0줄 |
| Whisky 0 | Whisky wrapper 0줄 |
| GPTK 0 | Apple GPTK metal translation layer 0줄 (graphics 영역과 무관, GameGuard 영역도 동일) |
| hexa-only | airgenome-gamebox 본체 + 본 doc 측 hexa 외 import 0줄 |
| GameGuard bypass code 0 | 본 doc 측 + lib/ 측 bypass / shim / circumvent 코드 0줄 (§3 의 reference 는 명칭 only) |
| kernel driver fetch 0 | NTOSKRNL emulation / Windows .sys driver fetch 0줄 |
| signature spoof 0 | HWID / machine_id 위조 코드 0줄 |

### 4.2 own2 enforcement

- 본 doc 측 emit 패턴: `__GAMEGUARD__ KERNEL_MODE_RETIRED_INTENTIONAL` (posture-level emit, lib/ 측 honest log)
- `lib/loader/pe_lineage_classic_r28_track.hexa` self_test 측 Track-B 추가 row (Track-B Purple-Lineage offline-shim cycle land 완료):
  ```
  __LC_R28_GG__ DETECTED gameguard inca_internet kernel forbidden
  __LC_R28_GG__ RETIRED_INTENTIONAL gameguard adopted_iso=2026-05-03
  ```
- silent error 0건 — 모든 GameGuard 측 detect / retired posture 는 honest emit 으로 노출
- swallowed error 0건 — block 발생 시 graceful retired_intentional emit, 우회 시도 X

### 4.3 block-acknowledged posture

`retired_intentional` = mk2 status enum 의 정식 status 측 채택 (2026-05-03 Track-B 측 lineage_classic_r28 첫 사례 lock-in). 본 freeze doc 는 그 posture 의 **영구 documentation** 측 standalone reference.

- `status: active` 유지 (게임 자체는 출시 + tracking 활성)
- `status_enum_adopted: true` (blk.1 GameGuard kernel-AC 측 block-acknowledged honest documentation)
- `cond.1 status: partial` 유지 (production smoke 측 retired_intentional posture)
- `cond.3 status: partial` (본 freeze doc 의 land 측 evidence row 추가, status 승격 X — block-acknowledged retired_intentional posture 자체가 retired_intentional 카테고리이므로 met 승격 영구 X)

---

## 5. Cross-link table

본 doc 의 reference 측 cross-link (각 path repo-relative):

### 5.1 lineage_classic_r28 family

| path | role | relation |
|---|---|---|
| `.roadmap.lineage_classic_r28` | game roadmap (mk2 SSOT) | cond.3 evidence row 측 본 freeze doc 추가 (Track-E land) |
| `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` | mk1 narrative (manjeom review) | sister doc — Track-B 2026-05-03 land 완료, GameGuard 측 axis B = 0 영구 lock 명시 |
| `lib/loader/pe_lineage_classic_r28_track.hexa` | loader sub-module | self_test 측 `__LC_R28_GG__ DETECTED` + `__LC_R28_GG__ RETIRED_INTENTIONAL` row (Track-B additive) |
| `lib/perf/c_lineage_classic_r28_native_first_frame.hexa` | perf sub-module | OpenGL 4.3 first-frame trace (cond.2 partial, retired_intentional posture transitive) |

### 5.2 retired_intentional adoption family

| game | adopted? | mk1 narrative | blocker family |
|---|---|---|---|
| **lineage_classic_r28** | ✓ adopted (2026-05-03) | `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` | **GameGuard kernel** (본 freeze doc reference) |
| diablo4 | candidate (lock-in 대기) | `docs/D4_MANJEOM_REVIEW.md` | always-online DRM |
| cs2 | candidate (lock-in 대기) | `docs/CS2_MANJEOM_REVIEW.md` | VAC user-mode AC |
| delta_force | candidate (lock-in 대기) | `docs/DELTA_FORCE_MANJEOM_REVIEW.md` | EAC kernel-AC |
| elden_ring | candidate (lock-in 대기) | `docs/ELDEN_RING_MANJEOM_REVIEW.md` | EAC anti-tamper |

lineage_classic_r28 = 5 게임 candidate 중 정식 adopted 첫 사례 (Track-B 2026-05-03). 본 freeze doc 는 그 family 의 1 차 freeze reference.

### 5.3 predecessor markers + handoff

| marker / doc | role |
|---|---|
| `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` | per-game 11 fan-out (lineage_classic_r28 진입점) |
| `state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker` | Track-B (retired_intentional 정식 채택, 본 freeze doc separate-cycle hook 명시) |
| `airgenome/doc/airgenome_gamebox_per_game_roadmaps_landed_2026_05_03.ai.md` | per-game handoff |
| `airgenome/doc/airgenome_gamebox_purple_lineage_offline_shim_landed_2026_05_03.ai.md` | Track-B handoff (본 doc next-cycle hook #3 명시) |
| `airgenome/doc/airgenome_gamebox_gameguard_honest_freeze_landed_2026_05_03.ai.md` | **본 cycle handoff (Track-E)** |
| `state/markers/airgenome_gamebox_gameguard_honest_freeze_landed.marker` | **본 cycle marker (Track-E)** |

### 5.4 sister manjeom reference

각 mk1 narrative 의 axis B = 0 lock-in 패턴 정합 (각 게임 별 blocker class 다름):

- `docs/CS2_MANJEOM_REVIEW.md` — VAC + Trusted Mode user-mode AC (axis B = 0 multiplayer)
- `docs/DELTA_FORCE_MANJEOM_REVIEW.md` — ACE kernel-AC (axis B = 0)
- `docs/ELDEN_RING_MANJEOM_REVIEW.md` — EAC anti-tamper (axis B = 0)
- `docs/D4_MANJEOM_REVIEW.md` — always-online DRM (axis B = 0)
- `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` — **GameGuard kernel-AC (axis B = 0, 본 freeze doc 의 1 차 reference target)**

각 게임 별 blocker family 는 다르나 (VAC / EAC / ACE / always-online DRM / GameGuard), `retired_intentional` posture rationale 은 동일 — closure path B 측 영구 차단 + own1 violation 회피 + cheat-bypass 거부.

---

## 6. future-cycle non-goals (영구 out-of-scope)

본 doc 는 spec freeze 이며, 다음 영역은 **airgenome-gamebox lifetime 동안 영구 out-of-scope**:

### 6.1 kernel-level work (영구 X)

- ❌ GameGuard kernel driver shim / wrapper / port 작업
- ❌ NTOSKRNL emulation / Windows kernel primitive port
- ❌ SSDT / IRP / inline hook detection 우회
- ❌ kernel driver signature 위조 / spoof
- ❌ Apple Silicon kernel 측 GameGuard 동작 emulation 시도
- ❌ Wine kernel emul 측 import / cite

### 6.2 user-mode bypass work (영구 X)

- ❌ GameGuard user-mode component (`GameGuard.des` / `npgmup.des`) 측 우회
- ❌ ticket validation 측 spoof
- ❌ HWID / machine_id 위조
- ❌ module enumeration 측 spoof (PEB Ldr loaded module list 위조)
- ❌ anti-debug 측 회피 (RDTSC timing spoof / NtQueryInformationProcess hook)
- ❌ memory signature scan 측 회피 pattern

### 6.3 platform-level work (영구 X)

- ❌ Apple Silicon kernel 측 driver port (SIP 우회 / unsigned driver load path)
- ❌ Rosetta 2 측 Windows .sys 측 dynamic translation
- ❌ macOS 측 GameGuard 동등 기능 implement (의도적으로 X — gamebox 의 가치는 honest research direction 한정)

### 6.4 in-scope (영구 영역)

- ✓ honest detection emit (`__LC_R28_GG__ DETECTED` 측 client-side honest report)
- ✓ retired_intentional posture maintenance (`__LC_R28_GG__ RETIRED_INTENTIONAL` 측 block-acknowledged emit)
- ✓ external public reference 의 high-level naming summary (본 doc 의 §3 수준)
- ✓ mk1 narrative + mk2 .roadmap retired_intentional rationale documentation
- ✓ cross-link maintenance (다른 retired_intentional candidate 게임 측 family precedent)

---


C1 — **bypass / evasion / circumvention 0건 명시**: 본 doc 의 어떤 부분도 GameGuard 우회 / 회피 / circumvent 방법을 제공하지 않음. §3 의 hook category / detection vector reference 는 OS textbook 수준의 명칭과 일반 설명만이며, 회피 방법 / 회피 코드 / 회피 pattern 0건. 모든 `bypass` / `evasion` / `circumvent` 단어는 negation context 에서만 사용.

C2 — **외부 공개 자료 의존 + 추정 영역 명시**: GameGuard 의 실 구현은 INCA Internet 비공개. 본 doc 의 컴포넌트 / hook category / detection vector reference 는 외부 공개 자료 (벤더 공식 / Wikipedia / OS textbook) 수준이며, 비공개 영역의 정확한 구현은 알려져 있지 않음. 본 doc 의 일부 추정 (e.g. "kernel driver attest 측 macOS 영구 mismatch") 도 platform 자체의 정의 차이 + 외부 공개 자료 기반의 추정.

C3 — **lineage_classic_r28 외 적용 게임 enumerate 측 별도 cycle 권고**: §1.2 의 다른 NCSOFT 게임 (Aion / B&S / Lineage 2 / 등) 측 GameGuard 적용 여부는 외부 공개 reference 만이며, 본 cycle 측 spec emit X. 별도 cycle 측 per-game roadmap fan-out 시 추가 enumerate 권고.

C4 — **`retired_intentional` posture 의 의미 영구 lock**: block-acknowledged honest documentation = PASS-equivalent posture 는 mk2 schema 측 정식 status enum 채택 (Track-B 2026-05-03). 본 freeze doc 는 그 posture 의 standalone reference 일 뿐, posture 자체의 의미는 Track-B 측 lock-in 완료. 다른 4 게임 (D4 / CS2 / Delta Force / Elden Ring) 측 별도 cycle 측 동일 패턴 lock-in 권고.

C5 — **Wine / CrossOver community research direction 측 import 0건**: §3.4 측 KR community + Wine / CrossOver 측 외부 시도 영역은 본 doc 측 cite 0건, gamebox 측 import 0건. 본 doc 는 그 영역의 존재 명시 only, 그 영역의 코드 / pattern / blog post 측 cite / import 영구 X.

C6 — **mk1 narrative cross-link 의존**: 본 freeze doc 는 mk1 narrative `LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md` (Track-B 2026-05-03 land 완료) 의 sister 위치. mk1 narrative 측 axis A/B/C scoring + `manjeom = 5 + 0 + 5 = 10/15` 결론 + GameGuard 측 axis B = 0 영구 lock 명시 측 cross-reference.

C7 — **cond.3 status 승격 영역 X**: 본 doc 의 land 는 `.roadmap.lineage_classic_r28.cond.3` 측 evidence row 추가 (additive only) 에 그치며, status `partial` → `met` 승격 X. 이유: cond.3 자체가 "GameGuard honest spec freeze documentation" 인데, `retired_intentional` posture 의 본질이 block-acknowledged 이므로 `met` 카테고리 승격은 cond.3 의미 정의상 영구 X. evidence row 만 추가 (Track-B 측 적용한 패턴과 동일).

C8 — **own1 / own2 영구 enforcement**: 본 doc 측 + airgenome-gamebox 본체 측 own1 (Wine 0 / hexa-only / kernel-AC bypass code 0) + own2 (honest emit / silent error 0) 영구 정합. 본 freeze doc 는 그 enforcement 의 1 차 standalone reference (kernel-AC family 측).

C9 — **NCSOFT 계정 / KR live server 영역 영구 out-of-scope**: 본 doc 측 / airgenome-gamebox 본체 측 NCSOFT plaync OAuth / KR live server connect / HWID 등록 / 계정 위협 행위 시도 0건. live server measurement 측 영역은 mk1 narrative 측 명시한 retired_intentional posture 와 정합.

C10 — **외부 cite 측 vendor 공식 / 공개 textbook 한정**: 본 doc 의 §8 (출처) 측 cite 는 INCA Internet 공식 + Wikipedia / OS textbook 수준 자료만. KR community blog / Wine staging patch / CrossOver bottle 측 cite 0건. 우회 자료 cite 영구 X.

C11 — **2026-02-07 Lineage Classic R28 출시 후 시점**: 본 doc 작성 시점 (2026-05-03) 은 R28 출시 후 first quarter post-release tracking (90 일 mark). 차후 GameGuard release 측 컴포넌트 구성 변동 시 본 freeze doc 측 별도 cycle 측 reference update 권고.

C12 — **본 doc 의 path / cross-link 측 repo-relative 정합 (raw 15 env_lazy)**: 본 doc 측 모든 path 는 repo-relative — `lib/...`, `docs/...`, `.roadmap.<game>`, `state/markers/...`. 사용자별 absolute path 0건, env() 측 lazy resolve 정합.

---

## 8. 출처 (외부 공개 자료, 2026-05-03)

본 doc 측 cite 는 vendor 공식 + Wikipedia / OS textbook level 자료 한정. 우회 / community bypass 자료 cite 영구 X.

- INCA Internet 공식 — https://gameguard.nprotect.com/
- INCA Internet 회사 정보 — https://www.inca.co.kr/
- NCSOFT Lineage Classic 공식 — https://lineage.plaync.com/
- NCSOFT 2026 R28 update patch note — (NCSOFT 공식 portal)
- Lineage 시리즈 Wikipedia — https://en.wikipedia.org/wiki/Lineage_(video_game)
- Windows Internals (Russinovich / Solomon) — kernel hook category 일반 설명 reference
- airgenome-gamebox sister doc — `docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md`
- airgenome-gamebox sister manjeom (kernel AC family precedent) — `docs/DELTA_FORCE_MANJEOM_REVIEW.md` (ACE), `docs/CS2_MANJEOM_REVIEW.md` (VAC)
- airgenome-gamebox sister manjeom (anti-tamper family precedent) — `docs/ELDEN_RING_MANJEOM_REVIEW.md` (EAC)
- airgenome-gamebox sister manjeom (always-online DRM family precedent) — `docs/D4_MANJEOM_REVIEW.md`
- airgenome-gamebox Track-B handoff (predecessor) — `airgenome/doc/airgenome_gamebox_purple_lineage_offline_shim_landed_2026_05_03.ai.md`

---

*written 2026-05-03 — Track-E GameGuard honest freeze cycle. spec freeze land 완료 (no bypass / no evasion / no circumvention). lineage_classic_r28 cond.3 측 evidence augment additive (block-acknowledged retired_intentional posture 영구 정합). airgenome-gamebox 측 own1 (Wine 0) + own2 (honest emit) 영구 정합 reflect.*
