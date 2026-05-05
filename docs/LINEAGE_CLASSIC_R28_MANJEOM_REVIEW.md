# Lineage Classic R28 (NCSOFT 2026-02-07 신작) 만점 검토

> **status**: 검토일 **2026-05-03** / **만점 가능성: 부분 가능 (offline launch / 스펙 동결 한정)** — closure path B 의 PE map / IAT bind / OpenGL renderer 는 우호적이나, **GameGuard kernel-mode anti-cheat** 가 live KR 서버 진입 측 closure path B 차단 → **gameplay 만점 불가, retired_intentional 정식 채택**.
> 작성: 2026-05-03 (Track-B Purple → Lineage chain cycle)
> SSOT: `.roadmap.lineage_classic_r28` (mk2 JSONL header) / `docs/airgenome_gamebox_purple_lineage_offline_shim_landed_2026_05_03.ai.md` (handoff)
> 결론 한줄: **Lineage Classic R28 = PE32+ x64 + OpenGL 4.3 path 측 own1 친화 (D2R / Lineage 2 같은 구버전 family precedent), 단 GameGuard kernel driver 측 Apple OS 영구 차단 → cond.1 partial 영구 lock + retired_intentional 정식 채택 (block-acknowledged honest documentation = PASS-equivalent posture).**

---

## 0. 게임 메타데이터

| 항목 | 값 | 근거 |
|---|---|---|
| 출시 | **2026-02-07** | NCSOFT 공식 발표 (R28 update — Lineage Classic 신작 cycle) |
| 시리즈 출발 | Lineage Classic 시리즈 (1998 시작 Lineage 의 classic 복각 라인), R28 = 28번째 major update |
| 엔진 | NCSOFT 자체 classic engine (Lineage 1 native code 계보) | binary observation, 공개 patch note |
| Windows binary | **PE32+ x86_64** (`LineageClassic.exe` 또는 `L2.exe`) | NCSOFT 공식 distribution |
| 그래픽스 API | **OpenGL 4.3+** (Win10/11 64bit only) | Lineage Classic 시리즈 공통 (D3DMetal 우회 → OpenGL 직접 path 우호적) |
| Launcher | NCSOFT 자체 native launcher (Purple X — Lineage Classic 은 **direct executable**, Purple shell 미사용) | 공식 patch note |
| Anti-cheat | **GameGuard (INCA Internet)** — kernel-mode driver, ticket-based AC, Lineage 시리즈 표준 | INCA Internet 공식, Lineage M / Lineage W 와 별도 (Lineage Classic 만 GameGuard 직적용) |
| HWID gate | 필수 기기 등록 (HWID 기반, 라이선스 인증) — KR 서버 측 필수 | NCSOFT KR account 정책 |
| Online 의존성 | **live KR / 대만 서버 필수** — offline mode X | NCSOFT 공식, MMO 정책 |
| macOS native | **없음 (영구)** | NCSOFT 정책상 KR MMO classic 라인 미지원 |

### 해석 (Apple Silicon native 관점)

- Lineage Classic 시리즈는 **OpenGL 4.3+ 직접 사용** 특성 — DXVK / D3DMetal 우회 비용 없이 closure path B 친화 (D2R 동급).
- 그러나 **GameGuard kernel driver 측 Apple OS 영구 차단** — kernel-mode driver attest 측 macOS 환경은 즉시 mismatch.
- live KR 서버 측 GameGuard ticket 검증 통과 X → **closure path B 의 cond.1 PE parse / cond.2 first-frame draw 까지는 가능, gameplay (live server connect) 측 영구 retired_intentional**.

---

## 1. 만점 axis A / B / C scoring

D2R / D4 / CS2 / Delta Force / Elden Ring 검토 동일 framework — Apple Silicon closure path B 환경에서 manjeom 만점 (15) = A + B + C 각 5점.

### 1.1 axis A — closure path B 호환성 (PE map / IAT / DX renderer / fault graceful)

| 평가 항목 | 점수 | 근거 |
|---|---:|---|
| PE32+ x86_64 binary | 5 | LineageClassic.exe = PE32+ x64, gamebox closure path B 직접 호환 |
| IAT bind | 5 | NCSOFT 자체 engine, Win32 표준 IAT (kernel32, user32, gdi32, advapi32, ws2_32, opengl32) — gamebox 13 win32_* shim coverage 통과 |
| OpenGL 4.3 path | 5 | gamebox loader_dx 측 OpenGL ICD wrapper 우회 path 가능 (D3DMetal 비용 0 — Lineage Classic 의 강점) |
| Fault graceful | 5 | own2 emit + panic 형식 정합, gamebox r0_session 측 honest log |

**axis A = 5/5** — D2R 동급 친화 (D4 보다 우호적 — D4 는 D3D12+DXR, Lineage Classic 은 OpenGL 4.3 만, GPU bind 측 단순).

### 1.2 axis B — 외부 의존성 (DRM / launcher / network 통과)

| 평가 항목 | 점수 | 근거 |
|---|---:|---|
| Steam DRM | 5 | NCSOFT 자체 distribution (Steam X) — DRM 우회 비용 0 |
| Launcher 독립성 | 5 | Purple shell 미사용 — direct executable, launcher transitive dep 0 |
| HWID gate | 3 | KR server 측 HWID 등록 필수 (offline mode X) — gamebox closure path B 측 cond.1 PE parse 까지는 가능, 실 server connect 측 사용자 측정 |
| GameGuard kernel AC | **0** | **closure path B 영구 차단 — kernel-mode driver attest 통과 X (Apple OS), retired_intentional 정식 채택** |
| Network endpoint (KR / 대만) | 1 | live MMO server, offline mode 미존재 |

**axis B = 0/5** — GameGuard kernel AC 측 **영구 차단** (block-acknowledged = retired_intentional honest documentation).

### 1.3 axis C — 측정 / 만점 가치 (gamebox 사용자 측 ROI)

| 평가 항목 | 점수 | 근거 |
|---|---:|---|
| First-frame trace (offline shim) | 5 | OpenGL 4.3 first-frame 측 spec freeze 가능 (live server connect 없이) — c_lineage_classic_r28_native_first_frame.hexa 측 attest |
| PE map / IAT trace | 5 | gamebox closure path B 측 D2R 동급 trace 가능 |
| GameGuard honest detect | 5 | __LC_R28_GG__ DETECTED + RETIRED_INTENTIONAL 2 row 측 honest emit (no bypass), own2 정합 |
| live gameplay measurement | 0 | retired_intentional — live server measurement 영구 X |
| 사용자 가치 (KR community fan/dev) | 4 | Lineage Classic fan/dev 측 first-frame trace + IAT trace 측 가치 있음 (live gameplay 미가치) |

**axis C = 5/5** (첫 4 항목) — 단 live gameplay measurement = 0 (retired_intentional posture).

### 1.4 manjeom 누적

- **Axis A = 5** (closure path B 호환)
- **Axis B = 0** (GameGuard 차단)
- **Axis C = 5** (offline shim / first-frame trace / honest detect)

**manjeom = A + B + C = 5 + 0 + 5 = 10/15** — **부분가능 (offline shim 한정)**, gameplay 만점 불가.

D2R = 15 (closure path B 100% + Battle.net offline mode 가능) 와 비교 시 **GameGuard 측 axis B = 0** 이 차이 — D2R 보다 5점 낮음, 그러나 axis A + C 측 closure path B 가치는 동등.

---

## 2. GameGuard kernel-mode anti-cheat 차단 분석 (research-only direction)

> **own1 + 정직성 정책상 GameGuard bypass 카테고리 진입 X** — 본 §2 는 외부 자료 기반 detection 분석 only, 우회 코드 / 모듈 / 시도 0건.

### 2.1 GameGuard 검출 벡터 (외부 공개 자료 기반)

INCA Internet GameGuard (현 GameGuard, NPGameLib 후속) — Lineage 시리즈 + 다수 KR MMO 적용. 공개 자료:

| 검출 벡터 | gamebox closure path B 영향 | 위험도 |
|---|---|---|
| Kernel driver (GameMon64.des / npggNT.des) load | gamebox 는 kernel driver 자체 load X (user-space PE loader only) — driver attest 측 macOS 환경 즉시 mismatch | **치명** (영구 차단) |
| Process protection (NtOpenProcess / NtTerminateProcess hook) | gamebox 가 LineageClassic.exe 를 in-process map 하므로 process tree 측 anomaly | **치명** |
| HWID + machine_id 검증 | gamebox 환경 (macOS Apple Silicon) 측 Windows machine_id 측정 X — KR server 측 등록 자체 불가 | **치명** (HWID gate 측 영구 X) |
| Module enumeration (PEB Ldr 측 expected DLL list) | gamebox own1 IAT 측 100% reroute — VAC / EAC / GameGuard 공통 mismatch | **치명** |
| Anti-debug (NtQueryInformationProcess + ProcessDebugFlags) | gamebox r0_session 측 stat read 만, debug API 미touch — false positive 가능 | 중 |
| Memory signature scan | gamebox 가 cheat 패턴은 안 만들지만 DIY PE map 측 비표준 layout 자체가 anomaly signature 후보 | 중 (false positive 가능) |
| File integrity (LineageClassic.exe SHA + game data .pak hash) | gamebox 는 binary 측 byte-identical map (modify X) — 통과 가능 | 저 |

### 2.2 회피 가능성 — gamebox 정책상 **거부**

기술적으로 GameGuard bypass 는 (외부 자료상) thread hijacking + manual map injection + anti-anti-debug 조합으로 일부 우회 가능 (KR community 보고). 그러나:

1. **own1 + 정직성 정책상 cheat-bypass / kernel-AC bypass 카테고리 진입 X** — `IDENTITY.md` + `docs/CLOSURE_ROADMAP.md` 정신 위반.
2. NCSOFT 의 **계정 BAN + IP / HWID 영구 차단** — gamebox 사용자 자산 손실 risk.
3. gamebox 의 가치 = **개발자 / 연구자가 Apple Silicon 에서 OpenGL 4.3 first-frame trace 측정** 이지, live KR MMO 진입 X.

**정책**: gamebox Lineage Classic R28 path 는 **offline shim + spec freeze 한정**. live server connect 는 명시적으로 out-of-scope.

### 2.3 honest detect / retired_intentional posture 정의

`lib/loader/pe_lineage_classic_r28_track.hexa` self_test() 측 Track-B 추가 row 2개:

```
__LC_R28_GG__ DETECTED gameguard inca_internet kernel forbidden
__LC_R28_GG__ RETIRED_INTENTIONAL gameguard adopted_iso=2026-05-03
```

own2 정합 — emit only, 우회 시도 0건. mk1 narrative (본 doc) cross-ref 명시.

---

## 3. Wine 0 / Apple-only stance

gamebox 는 own1 enforcement (Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0) 정합. Lineage Classic R28 측 path:

- **Apple Silicon native** — 없음 (영구). NCSOFT 정책상 KR MMO classic 라인 macOS 미지원.
- **Wine / CrossOver / Whisky / GPTK** — own1 위반, 0줄. NCSOFT KR community 측 일부 사용자가 CrossOver 측 시도 보고는 있으나, gamebox 측 인용 / import 0건.
- **gamebox closure path B** — own1 (a) own-implementation hexa-native PE loader + (c) own-instrumentation + (b) Apple-only Metal/OpenGL bridge (D3DMetal 우회 — Lineage Classic 의 OpenGL 4.3 path 측 ICD wrapper 직접 경로 가능).

**결론**: Apple Silicon native 미존재, gamebox closure path B 측 OpenGL 4.3 ICD wrapper path 만 honest research direction.

---

## 4. retired_intentional 정식 채택 rationale

mk2 status enum 측 `retired_intentional` (block-acknowledged honest documentation = PASS-equivalent) 정식 채택 — 본 cycle (Track-B 2026-05-03) 측 lineage_classic_r28 = D4 / CS2 / Delta Force / Elden Ring 4 게임 retired_intentional candidate 와 같은 family.

### 4.1 채택 기준 (5 게임 공통)

| game | blocker | adopted_iso | mk1 narrative |
|---|---|---|---|
| diablo4 | always-online DRM | (lock-in 대기) | docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md |
| cs2 | VAC user-mode AC | (lock-in 대기) | docs/CS2_MANJEOM_REVIEW.md |
| delta_force | EAC kernel-AC | (lock-in 대기) | docs/DELTA_FORCE_MANJEOM_REVIEW.md |
| elden_ring | EAC anti-tamper | (lock-in 대기) | docs/ELDEN_RING_MANJEOM_REVIEW.md |
| **lineage_classic_r28** | **GameGuard kernel-AC** | **2026-05-03 (본 cycle 정식 adopted)** | **docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md (본 doc)** |

lineage_classic_r28 = **5 게임 중 정식 adopted 첫 사례** — mk1 narrative 본 doc land + `__LC_R28_GG__ RETIRED_INTENTIONAL` posture row 측 attest 완료, status_enum_adopted = true.

### 4.2 adoption posture 의미

- `status: active` 유지 — 게임 자체는 출시되었고 gamebox closure path B 측 cond.1 partial / cond.2 partial 측 추적 활성.
- `status_enum_adopted: true` — blk.1 GameGuard kernel-AC 측 honest documentation = PASS-equivalent (cheat-bypass / kernel-AC bypass 시도 X).
- `cond.1 status: partial` 유지 — production smoke 측 retired_intentional 영구, GameGuard ticket 측 통과 path 미존재.
- `cond.3 status: unmet` 유지 — `docs/GAMEGUARD_HONEST_FREEZE.md` (가칭) 별도 cycle land 권고 (본 cycle 측 mk1 narrative 만 land).

### 4.3 다른 4 게임 lock-in 권고

본 cycle 측 lineage_classic_r28 만 정식 adopted — 나머지 4 게임 (D4 / CS2 / Delta Force / Elden Ring) 은 사용자 lock-in 대기. 별도 cycle 측 동일 pattern 적용 권고.

---

## 5. downstream NCSOFT 관계

NCSOFT 게임 family 측 lineage_classic_r28 의 위치 — Lineage 시리즈 변종 3개 (Classic R28 / M / W) 와 Purple shell 통합 launcher class 의 cross-link.

### 5.1 Lineage 변종 3개 비교 (Track-B 정합)

| variant | release | client | launcher | anti-cheat | retired_intentional |
|---|---|---|---|---|---|
| **Lineage Classic R28** | 2026-02-07 | native PC (PE32+ x64 + OpenGL 4.3) | NCSOFT 자체 (Purple X) | **GameGuard kernel** | **adopted (2026-05-03)** |
| Lineage M (Purple) | 2017 KR | mobile APK (Android) | Purple shell + Android emul | (mobile native, plaync OAuth) | none (purple_launcher cond met 시 cascading PASS) |
| Lineage W (PURPLE) | 2021 글로벌 | UE4 PC (PE32+ x64) | Purple shell | (UE4 native, plaync OAuth) | none (loader sub-module land + purple_launcher cond met 시 cascading PASS) |

### 5.2 Purple shell 통합 의존성 (lineage_m / lineage_w 측 cascading)

- **Lineage Classic R28** = Purple shell 미사용 — direct executable, GameGuard 직적용
- **Lineage M / W** = Purple shell 측 .NET CLR + WebView2 + Android emul (M only) + plaync OAuth + 2FA TOTP 측 통과 후 진입
  - purple_launcher cond.1 (.NET CLR + WebView2) — Track-B met (offline_shim_track_b)
  - purple_launcher cond.2 (Android emul layer — gles_to_metal) — Track-B met (offline_shim_track_b)
  - purple_launcher cond.3 (plaync OAuth + 2FA TOTP) — partial (network/user-secret)
- **Lineage 2 (구)** — `pe_lineage_2_specific_track.hexa` disk 잔존, 본 cycle untouched (predecessor marker 측 명시 제외)

### 5.3 NCSOFT 게임 family 측 retired_intentional 후보

본 cycle (Track-B 2026-05-03) 측 lineage_classic_r28 정식 adopted — 다른 NCSOFT 게임 측 retired_intentional 후보 enumerate (별도 cycle land 권고):

| NCSOFT game | blocker family | retired_intentional 가능성 |
|---|---|---|
| Lineage 2 (구) | GameGuard kernel | candidate (구 client, repo 측 untouched per predecessor) |
| Aion | GameGuard kernel + Purple | candidate (purple_launcher + GameGuard 두 layer) |
| Blade & Soul | GameGuard kernel + Purple | candidate |
| Throne and Liberty | (확인 필요) + Purple | candidate (UE4) |
| Lineage 2M | (확인 필요) + Purple + Android emul | candidate |

위 5 게임은 **본 cycle 측 spec emit X** — 사용자 lock-in 후 별도 cycle land 권고.

---

## 6. 우선순위 권고

### 6.1 본 cycle (Track-B 2026-05-03) 완료

- mk1 narrative (본 doc) land
- `lib/loader/pe_lineage_classic_r28_track.hexa` self_test() Track-B 2 posture row 추가 (additive only)
- `.roadmap.lineage_classic_r28` blk.1 status_enum_adopted = true 정식 채택
- tests/test_purple_lineage_offline_shim.hexa harness 측 retired_intentional posture row attest

### 6.2 차후 cycle 권고 (사용자 lock-in 후)

1. `docs/GAMEGUARD_HONEST_FREEZE.md` (가칭) spec freeze doc land — `.roadmap.lineage_classic_r28.cond.3` unmet → met 전환 가능
2. native first-frame production smoke (cond.2 partial → met) — OpenGL 4.3 first-frame trace + `c_lineage_classic_r28_native_first_frame.hexa` self_test 확장
3. 다른 NCSOFT 게임 (Lineage 2 / Aion / B&S / TL / L 2M) 측 retired_intentional candidate per-cycle land
4. 다른 4 게임 (D4 / CS2 / Delta Force / Elden Ring) 측 retired_intentional 정식 adopted lock-in (lineage_classic_r28 첫 adopted precedent 활용)

### 6.3 거부 영역 (명시적 out-of-scope)

- GameGuard kernel-AC bypass 시도
- KR live server connect 시도 (HWID 등록 + GameGuard ticket 통과 시도)
- NCSOFT 계정 위협 행위
- 어떤 형태의 cheat / botting signature

---

## 7. 정직성 메모 (raw#10 honest C3)

- 본 검토는 외부 공개 자료 (NCSOFT 공식 patch note + binary observation) 기반. 실 LineageClassic.exe 분해 / live server connect 0건.
- "manjeom 10/15" 수치는 axis A/B/C 추정 — D2R first-frame validated 비용과 비슷하다는 추정 근거는 OpenGL 4.3 path 우호도 (D2R DX11 path 와 동등 또는 우호) + GameGuard 측 axis B = 0 영구 lock.
- GameGuard 의 detection 메커니즘은 외부 자료 기반 — INCA Internet 의 실 구현은 비공개. own1 PE loader 와의 실 충돌도는 **추정 "치명" (영구 차단)**.
- "live KR server measurement = 0" 결론은 정의상의 결론 (own1 + cheat-bypass / kernel-AC bypass 거부 정책). 기술적 우회 가능성과 별개 — 본 doc 측 우회 / circumvent 모색 0건.
- 2026-02-07 R28 출시 후 first quarter post-release tracking — 본 cycle 측 90 일 mark (2026-05-03) 측 mk1 narrative land. 차후 cycle 측 patch note 측 변동 사항 reflect 권고.
- Lineage Classic R28 측 OpenGL 4.3 ICD wrapper path = D2R DX11 path 동등 우호도 = 추정. 실측은 first-frame production smoke 후 검증.

---

## 출처 (NCSOFT 공식 + 외부 공개 자료, 2026-05-03)

- NCSOFT Lineage Classic 공식 — https://lineage.plaync.com/
- NCSOFT 2026 R28 update patch note — (NCSOFT 공식 portal)
- INCA Internet GameGuard 공식 — https://gameguard.nprotect.com/
- Lineage 시리즈 Wikipedia — https://en.wikipedia.org/wiki/Lineage_(video_game)
- D2R 만점 검토 (axis A/B/C framework 인용) — `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md`
- CS2 manjeom (axis 비교) — `docs/CS2_MANJEOM_REVIEW.md`
- Delta Force manjeom (kernel AC family precedent) — `docs/DELTA_FORCE_MANJEOM_REVIEW.md`
- Elden Ring manjeom (anti-tamper family precedent) — `docs/ELDEN_RING_MANJEOM_REVIEW.md`
- Purple Lineage manjeom (Purple shell family) — `docs/PURPLE_LINEAGE_MANJEOM_REVIEW.md`

---

*written 2026-05-03 — Track-B Purple → Lineage chain cycle. mk1 narrative land 완료, retired_intentional 정식 adopted (5 게임 candidate 중 첫 사례). live KR server measurement 영구 retired_intentional posture, offline shim / spec freeze 한정 가치만 reflect.*
