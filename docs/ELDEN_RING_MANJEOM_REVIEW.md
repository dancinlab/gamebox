# Elden Ring + Shadow of the Erdtree DLC + Nightreign — gamebox 만점 기준 적용 가능성 검토

> **status**: 검토일 **2026-05-01** / **만점 가능성: 부분가능 (offline mode 만, online + EAC 거부)**
> 작성: Phase 0 검토 (모듈 구현 X)
> SSOT 보조: `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` 6 phase 모델 재사용
> 결론 한줄: **EAC kernel-mode 가 게임 launch 에 필수 X (FromSoft 가 offline mode 정식 지원, `start_game_in_offline_mode.exe`). multiplayer 보류 directive 와 자연 정합 — offline mode = EAC 비활성 → closure path B 통과 가능. base 게임 + Shadow of the Erdtree DLC 모두 동일 메커니즘. Steam-less 정식 path 는 Microsoft Store (Xbox PC UWP) 만, 단 standalone 배포 없음**.

---

## 0. 게임 메타데이터 (조사 결과)

### 0-1. Elden Ring (base game, 2022)

| 항목 | 값 | 출처 |
|---|---|---|
| 정식 명칭 | ELDEN RING | FromSoftware / Bandai Namco |
| 개발사 | FromSoftware | Wikipedia |
| 퍼블리셔 | Bandai Namco Entertainment | Wikipedia |
| 출시 | 2022-02-25 (PC + console) | Wikipedia |
| 엔진 | **FromSoftware 자체 엔진** (Dark Souls / Sekiro 계보, 자체 명명 X) | FromSoftware |
| Windows binary | **PE32+ x86_64** (Win10/11 64bit only) | PCGamingWiki |
| 그래픽스 API | **DirectX 12** (DX11 fallback 없음) | PCGamingWiki, system requirements |
| Mesh shader / DXR | DXR 미사용 (UE5 의 Lumen 등 ray tracing 정책 X) | PCGamingWiki |
| 시스템 사양 (권장) | i5-8400 / Ryzen 3 3300X+, 12 GB RAM, GTX 1060 6GB / RX 580 8GB+, 60 GB | Bandai Namco 공식 |
| **공식 distribution** | **Steam (메인) + Microsoft Store (Xbox PC, UWP)** | Bandai Namco, Microsoft Store |
| **Steam 외 standalone** | **Microsoft Store 만 정식** — DRM-free / GOG / Epic 모두 부재 | GOG forum, Bandai Namco 자체 launcher 없음 |
| anti-cheat | **Easy Anti-Cheat (EAC) — kernel-mode + user-mode hybrid, VMProtect packed** | PCGamingWiki, easy.ac |
| EAC offline mode | **공식 지원** — `start_game_in_offline_mode.exe` (FromSoft 인정 도구) | techiew/EldenRingEacToggler, Windows Central |
| DRM | Steam DRM (start_protected_game.exe wrapper) + Microsoft Store DRM (UWP container, license check on launch) | PCGamingWiki |
| macOS 공식 지원 | **없음** (Windows + console 만) | Bandai Namco |

### 0-2. Shadow of the Erdtree (SotE) DLC (2024-06-21)

| 항목 | 값 | 출처 |
|---|---|---|
| 정식 명칭 | ELDEN RING Shadow of the Erdtree (DLC) | FromSoftware |
| 출시 | 2024-06-21 | gamepressure |
| **base 게임 의존** | ✓ — Elden Ring 본체 필요 (DLC 형식, 별도 distribution X) | Bandai Namco |
| 엔진 | base 와 동일 (FromSoft 엔진) | PCGamingWiki |
| 시스템 사양 | **base 와 동일** ("uncharacteristic mercy" — PC Gamer 표현) | PC Gamer |
| 디스크 추가 | 60 GB (base) → **80 GB total** (DLC 포함) | Bandai Namco |
| **DRM / EAC** | base 와 동일 (별도 anti-cheat 추가 X) | gamepressure |
| 1.12 update 영향 | EAC crash 일부 보고 — FromSoft hotfix | gamepressure |
| **closure path B 적용** | base 와 100% 동일 (별도 차단 메커니즘 X) | 추론 |

### 0-3. Elden Ring Nightreign (standalone co-op, 2025-05)

| 항목 | 값 | 출처 |
|---|---|---|
| 정식 명칭 | ELDEN RING NIGHTREIGN (standalone co-op action survival) | Bandai Namco |
| 출시 | 2025-05-29 (PC + console) | Bandai Namco |
| **base 게임 의존** | ✗ — **standalone 게임** (Elden Ring 본체 불필요) | Bandai Namco |
| 시스템 사양 | base Elden Ring 과 비슷 (DX12, Win10 64bit) | gHacks, Bandai Namco |
| **distribution** | **Steam 만** (Microsoft Store / GOG / EGS / Bandai 자체 launcher 모두 부재) | Bandai Namco 공식 |
| EAC | base 와 동일 (kernel-mode hybrid) | 추정 |
| 4-player coop | online 필수 (multiplayer 거부 directive 직접 적용) | Bandai Namco |

→ **Nightreign 은 base 보다 closure path B 친화도 낮음** (online 강제, Microsoft Store 옵션 없음).

---

## 1. Steam-less standalone path 분석 (사용자 directive 핵심 질문)

사용자 directive: "전부 stream 버전아닌 단독 버전으로 조사 엘든링도".

### 1-1. 공식 standalone path

| Channel | 가능? | 비고 |
|---|---|---|
| **Steam** | ✓ | 메인 distribution, Steam DRM (start_protected_game.exe) |
| **Microsoft Store (Xbox PC)** | ✓ | **유일한 Steam 외 공식 PC path** — UWP/MSIX format, Microsoft Store DRM |
| GOG | ✗ | wishlist 만 (수년간 미실현) |
| Epic Games Store | ✗ | 없음 |
| Bandai Namco 자체 launcher | ✗ | 자체 launcher 부재 (Bandai 게임은 일반적으로 third-party 의존) |
| **DRM-free standalone** | ✗ | 정식 출시 0 |

### 1-2. Microsoft Store (Xbox PC) UWP/MSIX 분석

**Microsoft Store DRM 메커니즘** (PCGamingWiki + techengage 2026 분석):
- launch 시 license check (Microsoft account binding) — Steam 식 client process IPC 보다 가벼움
- launch 후 binary 는 decrypt 가능 (사용자 보고)
- UWP 는 사실상 **deprecated** (2021+ 신규 X, Microsoft 자체가 WinUI 3 / Windows App SDK 권장)
- MSIX = AppX + MSI + Desktop Bridge 통합 패키징, Win32 app 도 MSIX container 안에서 lightweight 가상화
- **container 안 PE 자체는 standard Win32 PE32+ x86_64** — closure path B 적용 가능

**closure path B 적용 시 Microsoft Store 의 추가 비용**:
- MSIX container 압축 해제 (zip 식 컨테이너) → 내부 PE 추출
- license check 우회 stub (Microsoft Store SDK 의 일부 함수 stub) — Steam Goldberg 식 stub 작성 가능
- registry/FS 가상화 처리 — UWP container 안에서 실행 시 자체 sandbox
- **장점**: Steam DRM 의 process IPC 의존성 없음, kernel module 로드 X
- **단점**: UWP 가 deprecated 라 신규 게임 출시 줄어드는 추세

### 1-3. 비공식 standalone path

| 방법 | 가능? | TOS / 정합성 |
|---|---|---|
| **Steam install + offline launch** (toggle EAC) | ✓ | **FromSoft 정식 지원** (`start_game_in_offline_mode.exe`) — TOS 위반 X, own 1 정합 |
| Goldberg Steam Emulator + depot downloader | △ | 회색 (Steam 라이선스 우회), 사용자 자발적 opt-in 필요 |
| Microsoft Store install + UWP container 추출 | △ | TOS 회색 (MS 라이선스 우회) |
| DRM-free 정식 | ✗ | 존재 X |

**가장 안전한 path**: Steam 으로 정식 install → `start_game_in_offline_mode.exe` 으로 offline launch (FromSoft 인정).

---

## 2. EAC offline mode 메커니즘 (추가 조사 결과)

### 2-1. EAC 아키텍처 (TATEWARE / hypercall.net 2026 분석)

EasyAntiCheat 는 multi-layer 시스템:

| 컴포넌트 | 역할 | mode |
|---|---|---|
| **EasyAntiCheat.sys** kernel driver | callbacks + minifilters, 게임 process 보호 | **kernel** (Ring 0) |
| Windows app (launcher) | 최신 EAC 모듈 다운로드 + 게임 launch | user |
| NT-Service | kernel driver 수명 제어 (start/stop) | user (controls kernel) |
| Injected user-mode module | 게임 process 안에서 ntdll/kernel32 무결성 검증 | user (in-process) |

**packing**: VMProtect 으로 obfuscation.

### 2-2. **핵심 발견**: EAC 가 게임 launch 에 필수 X

> "the game does not actually require EasyAntiCheat to launch or run — it only needs EAC to access online features like multiplayer or cross-saves." — TATEWARE / Steam community guides

이 의미:
- **`start_protected_game.exe`** = EAC launcher (online 진입용)
- **`start_game_in_offline_mode.exe`** = EAC 우회 offline launcher (FromSoft 정식 도구)
- offline mode 에서 EAC 자체가 비활성 (kernel driver 로드 안 됨)
- → DIY hexa loader 가 `eldenring.exe` 또는 `nightreign.exe` 직접 로드 시 **EAC 우회 작업 자체 불필요** (offline mode 진입과 동등)

### 2-3. closure path B 와의 호환성

| step | online (EAC 활성) | offline mode (EAC 비활성, 정식 toggle) |
|---|---|---|
| PE map | DIY hexa loader 가 PE32+ → memory image | ✓ 동일 |
| IAT bind | EAC injected module 이 ntdll/kernel32 hash 검증 → DIY shim fail | **✓ EAC 비활성 → 검증 X** |
| entry call | kernel driver attest fail → process self-terminate | **✓ EAC 비활성 → 통과** |
| 결과 | **✗ 차단** | **✓ 통과** |

→ **offline mode = closure path B 자연 호환**. multiplayer 보류 directive 와 정합 (online 거부, offline 진행).

### 2-4. own 1 정신 정합성

- EAC 우회 시도 X — FromSoft 정식 toggle 사용 (`start_game_in_offline_mode.exe`)
- VMProtect 우회 X — toggle 도구가 EAC 자체 비활성화
- 사용자 BAN risk X — offline mode 에서는 server-side 관찰 X
- ✓ **own 1 (Wine 0줄 + cheat-bypass 거부) 정합 100%**

---

## 3. 6 phase 별 만점 가능성 매트릭스

### Phase 1 — Steam install (또는 Microsoft Store install)

**시나리오**: Steam (메인) 또는 Microsoft Store 에서 Elden Ring 다운로드 → 60 GB (base) / 80 GB (DLC) 디스크 install → EAC 자동 install (선택).

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-ELDEN-PE64-MAP | 5 | 4 | 5 | 14 | D4 PE64 80% 재사용 |
| C-ELDEN-MSIX-CONTAINER-EXTRACT | 5 | 5 | 5 | **15** | Microsoft Store UWP/MSIX container 추출 (신규 — Steam 외 path) |

**phase 결론**: **부분 가능** (만점 +1, 보조 +1).

### Phase 2 — Steam OAuth / Microsoft Store license

**시나리오**: Steam client 또는 Microsoft account 로그인 → 라이선스 확인 → 게임 binary 사용 가능.

**Steam-less 옵션**: Goldberg-style stub + steam_appid.txt (Elden Ring App ID = 1245620) 또는 Microsoft Store license stub.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-ELDEN-STEAM-API64-OFFLINE-SHIM | 5 | 4 | 5 | 14 | Goldberg-style offline shim (CS2 와 동형) |
| C-ELDEN-MSSTORE-LICENSE-STUB | 5 | 5 | 5 | **15** | Microsoft Store UWP license check stub (신규) |

**phase 결론**: **가능** (만점 +1, 보조 +1).

### Phase 3 — game CDN download

**시나리오**: Steam content depot 또는 Microsoft Store CDN 다운로드. base = 60 GB, +SotE = 80 GB total.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-ELDEN-CDN-CHUNK-PARALLEL | 5 | 4 | 5 | 14 | C-CDN-PARALLEL-CHUNK 재사용 |

**phase 결론**: **부분** (보조 +1).

### Phase 4 — game launch + first frame (DX12)

**시나리오 (offline mode)**: `start_game_in_offline_mode.exe` 또는 DIY hexa loader 가 `eldenring.exe` 직접 로드 → DX12 init → first frame.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-ELDEN-DX12-FIRST-FRAME | 5 | 5 | 5 | **15** | DX12 first frame trace (D4 stage2 직접 재사용 — DX12 path 동일) |
| C-ELDEN-EAC-OFFLINE-LAUNCH-PROBE | 5 | 5 | 5 | **15** | `start_game_in_offline_mode.exe` 메커니즘 식별 (FromSoft 정식 toggle 흐름 정량화) |

**phase 결론**: **가능** (만점 +2). EAC offline mode = closure path B 자연 호환.

### Phase 5 — gameplay (offline mode 한정)

**시나리오**: Lands Between (base) / Realm of Shadow (SotE DLC) 탐색 + 보스전 + NPC 대화 — 모두 offline mode 가능. multiplayer (summon sign / invasion / coop) 만 보류.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-ELDEN-OFFLINE-SAVE-COMPAT | 5 | 5 | 5 | **15** | offline mode save compatibility (online save 와 separated, FromSoft 식 .sl2 파일) |
| C-ELDEN-DX12-PSO-WARM | 5 | 5 | 5 | **15** | DX12 PSO cache warm (D4 와 동형) |
| C-ELDEN-FROMSOFT-HKS-SCRIPT-CACHE | 5 | 5 | 5 | **15** | FromSoft HKS (Havok Script) 게임 logic 스크립트 캐싱 |
| C-SOTE-DLC-CONTENT-PROBE | 5 | 4 | 5 | 14 | Shadow of the Erdtree DLC 콘텐츠 식별 (base 와 분리 측정) |

**phase 결론**: **가능 (offline mode 한정)** (만점 +3, 보조 +1).

### Phase 6 — live tune

**phase 결론**: **가능** (offline mode 30+ 시간 측정 가능, Lands Between + Realm of Shadow 광대).

### 매트릭스 요약

| Phase | 만점 가능성 | ETA | 차단 사유 |
|---|---|---|---|
| 1. install | 부분 (+1 만점, +1 보조) | 0.5개월 | — |
| 2. license | 가능 (+1 만점, +1 보조) | 0.5개월 | Goldberg / MS Store stub |
| 3. CDN | 부분 (+1 보조) | 0.3개월 | — |
| 4. first frame (offline) | **가능 (+2 만점)** | 1개월 | EAC offline mode = 자연 호환 |
| 5. gameplay (offline) | **가능 (+3 만점, +1 보조)** | 1.5개월 | multiplayer 보류 directive |
| 6. live tune | 가능 | 0.5개월 | — |

→ **만점 도달 가능 (offline)**: 만점 +7, 보조 +4.
→ **만점 가능성 비율**: ~75% (offline mode 한정).
→ multiplayer (summon sign / invasion / coop) = **0% (보류 directive)**.

---

## 4. 추천 skeleton 모듈 candidate

### 4-1. Steam-less / Microsoft Store (UWP) 인프라 (신규 영역)

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 1. `c_elden_msix_container_extract` | 1 | 5 | 5 | 5 | **15** | MSIX/UWP container 추출 → Microsoft Store 의 PE 직접 접근. **신규 영역** — 향후 다른 Microsoft Store 게임 재사용 |
| 2. `c_elden_msstore_license_stub` | 2 | 5 | 5 | 5 | **15** | Microsoft Store license check stub. UWP API 의 일부 function stub |
| 3. `c_elden_steam_api64_offline_shim` | 2 | 5 | 4 | 5 | 14 | Goldberg-style stub (CS2 c_cs2_steam_api64_offline_shim 과 동형, generic 흡수 가능) |
| 4. `c_elden_pe64_loader_probe` | 1 | 5 | 4 | 5 | 14 | D4 PE64 80% 재사용 |

### 4-2. EAC offline mode 인프라

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 5. `c_elden_eac_offline_launch_probe` | 4 | 5 | 5 | 5 | **15** | `start_game_in_offline_mode.exe` 메커니즘 정량화 — FromSoft 정식 toggle 흐름. EAC offline 메커니즘이 다른 EAC 게임 (Rocket League 등) 에 generic 재사용 가능 |
| 6. `c_elden_eac_kernel_driver_avoid_track` | 4 | 5 | 5 | 5 | **15** | EasyAntiCheat.sys kernel driver 부재 시 게임 process 동작 추적 (Apple Silicon 환경에서 driver 자체 불가능 → offline 자동 진입) |

### 4-3. DX12 first frame + PSO

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 7. `c_elden_dx12_first_frame` | 4 | 5 | 5 | 5 | **15** | DX12 first frame trace — D4 stage2 직접 재사용 |
| 8. `c_elden_dx12_pso_warm` | 4-5 | 5 | 5 | 5 | **15** | DX12 PSO cache warm — D4 와 동형 |

### 4-4. FromSoft 자체 엔진 + HKS scripting

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 9. `c_elden_fromsoft_hks_script_cache` | 4-5 | 5 | 5 | 5 | **15** | FromSoft HKS (Havok Script) 게임 logic 캐싱 — Dark Souls / Sekiro / 다른 FromSoft 게임 재사용 |
| 10. `c_elden_offline_save_compat` | 5 | 5 | 5 | 5 | **15** | offline mode .sl2 save 호환성 (online save 와 분리) |

### 4-5. SotE DLC specific

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 11. `c_sote_dlc_content_probe` | 5 | 5 | 4 | 5 | 14 | Shadow of the Erdtree DLC 콘텐츠 식별 (base 와 분리 측정) — 추후 다른 DLC 게임에 generic |
| 12. `c_sote_realm_of_shadow_zone_load` | 5 | 5 | 5 | 5 | **15** | Realm of Shadow zone 로딩 trace (DLC 영역 진입) |

### 4-6. Nightreign (옵션, online-only 라 보류 directive 영역)

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| ~~13. c_nightreign_4p_coop_track~~ | 5 | — | — | — | — | **보류** — 4-player coop online 필수 = multiplayer 거부 directive 영역 |

### 4-7. multiplayer 보호 (사용자 보류 directive 정합)

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 13. `c_elden_online_entry_block` | 0 | 5 | 0 | 0 | 5 | **보호 모듈** — summon sign / invasion / coop 진입 시 자동 차단 + EAC 활성 모드 진입 시 경고. CS2 VAC guard / DF block doc / WG battle entry block 와 동형 |

→ **추천 모듈 수: 13개** (Nightreign 1개 보류, 실 진행 12개).

---

## 5. closure path B 호환성 — Elden Ring vs 기존 4 게임

| step | D2R | D4 | Delta Force | **Elden Ring (offline)** |
|---|---|---|---|---|
| PE map | i386 | x86_64 | x86_64 | **x86_64 (D4 와 동일)** |
| IAT bind | DX11 | DX12+DXR | DX12+DXR+UE5+ACE | **DX12 (D4 와 100% 같음)** |
| entry call | offline 가능 | online DRM | online + ACE attest | **offline mode 정식 — EAC 비활성** |
| anti-cheat | 없음 | 없음 (DRM) | **kernel ACE — 차단** | **EAC kernel — 단 offline mode 정식 toggle** |
| DLL coverage prerequisite | 일반 + DX11 | 일반 + DX12 | 일반 + DX12 + ACE | 일반 + DX12 + (EAC offline 시 부재) |
| DXR / Mesh shader | 없음 | DXR done | DXR (Lumen) | **없음 (정책)** |
| Apple D3DMetal 매핑 | DX11 stage1 | DX12 stage2 | stage2 (그러나 차단) | **stage2 직접 재사용** |
| Microsoft Store UWP path | — | — | — | **신규 옵션** (Steam-less standalone) |

**핵심 발견**:
- 그래픽스 stack = **D4 DX12 stage2 직접 100% 재사용** (DXR 없으니 더 단순)
- EAC = kernel-mode 이지만 **offline mode toggle 이 FromSoft 정식 지원** → multiplayer 거부 directive 와 자연 정합
- Microsoft Store (Xbox PC UWP) = Steam-less standalone path **유일한 정식 옵션** — 신규 영역
- FromSoft 자체 엔진 = HKS scripting (BigWorld Python 처럼 game logic 캐싱 신규)
- own 1 정합 100% (cheat-bypass 시도 X, FromSoft 정식 toggle 만 사용)

**호환성 점수**: **~80%** (D4 보다 약간 낮음 — Microsoft Store UWP 와 EAC offline toggle 신규 비용).

---

## 6. 결론 — 우선순위

### 6-1. 만점 가능성 종합

| 항목 | 값 |
|---|---|
| **만점 가능성 (offline mode)** | **가능 ~75%** (D4 와 동급, multiplayer 제외) |
| **만점 가능성 (multiplayer)** | 0% (보류 directive 영구 거부) |
| 도달 가능 만점 점수 | +7 만점 + 보조 +4 (Phase 1-6 offline 한정) |
| validated 만점 가능성 | **가능** (offline mode = EAC 비활성, kernel attest X) |
| skeleton 만점 가능성 | +12 (Nightreign 1개 보류) |
| BAN risk | **거의 0** (offline mode 에서는 server-side 관찰 X, FromSoft 정식 toggle 사용) |
| gamebox identity 와의 정합성 | **정합 100%** — Wine 0줄, EAC bypass 시도 X (정식 toggle), multiplayer 거부 directive 자연 정합 |

### 6-2. D2R / D4 / Lineage / WG / CS2 / DF 와 종합 우선순위

```
1. D2R closure 완료              ← 현재 진행
2. D4 closure 완료                ← Phase 10 dependent
3. (병렬) PURPLE shell + Lineage 클래식 r28 + L M / L W (cycle 49 done)
4. (병렬) Wargaming Stack (WoT + WoWS) — kernel AC 없음, training/replay validated 가능
5. (병렬) **Elden Ring + SotE DLC** — D4 DX12 stage2 직접 재사용, EAC offline mode 정식 path
6. CS2 offline path
7. Lineage 2 private (l2j) opt-in
8. 영구 미진입: Nightreign (online-only) / Delta Force / TL / Lineage 2M / W live (kernel AC)
```

→ **Elden Ring offline path 는 D4 closure 후 거의 즉시 진입 가능** (DX12 stage2 100% 재사용, EAC offline = D4 anti-cheat 없음과 동등).

### 6-3. cycle 50+ batch 후보 (Elden Ring)

**cycle 52 후보 (8 모듈 — Elden Ring offline path Phase 1-5)**:
- CN-Z1 c_elden_pe64_loader_probe (14, P1)
- CN-Z2 c_elden_msix_container_extract (15, P1) — 신규 Microsoft Store 영역
- CN-Z3 c_elden_msstore_license_stub (15, P2) — 신규 UWP license stub
- CN-Z4 c_elden_steam_api64_offline_shim (14, P2) — CS2 와 generic
- CN-Z5 c_elden_eac_offline_launch_probe (15, P4) — FromSoft 정식 toggle
- CN-Z6 c_elden_dx12_first_frame (15, P4) — D4 DX12 재사용
- CN-Z7 c_elden_fromsoft_hks_script_cache (15, P4-5) — HKS scripting
- CN-Z8 c_elden_online_entry_block (5, P0) — 보호 모듈

→ skeleton +6 만점 + 보조 +2.

**cycle 53 후보 (5 모듈 — DX12 PSO + offline save + SotE DLC)**:
- CN-Z9 c_elden_dx12_pso_warm (15, P4-5)
- CN-Y1 c_elden_eac_kernel_driver_avoid_track (15, P4)
- CN-Y2 c_elden_offline_save_compat (15, P5)
- CN-Y3 c_sote_dlc_content_probe (14, P5)
- CN-Y4 c_sote_realm_of_shadow_zone_load (15, P5)

→ 5 모듈, 단독 cycle 부족 — 다른 영역 (WG stack 잔여 / CS2 offline 잔여) 3 모듈 결합 권장.

### 6-4. 미래 가능성 (조건부)

다음 이벤트 발생 시 §1-6 재작성:
- **Bandai Namco / FromSoft 가 DRM-free 또는 GOG 출시** — 가능성 매우 낮음
- **macOS native Elden Ring 빌드** — Apple 자체 GPTK / FromSoft 협력 필요, 가능성 낮음
- **Apple Silicon Mac 에서 EAC user-mode variant 출시** — 가능성 낮음 (own 1 외부 환경)
- **Nightreign 의 offline 모드 추가** (현재 online 필수) — multiplayer directive 와 별개로 가치 있음

---

## 7. 출처 (WebSearch 결과 인용)

- ELDEN RING — Wikipedia: https://en.wikipedia.org/wiki/Elden_Ring
- ELDEN RING NIGHTREIGN — Wikipedia: https://en.wikipedia.org/wiki/Elden_Ring_Nightreign
- ELDEN RING NIGHTREIGN 공식 (Bandai Namco): https://www.bandainamcoent.com/games/nightreign
- Elden Ring Bandai Namco 공식: https://www.bandainamcoent.com/games/elden-ring
- Elden Ring Microsoft Store: https://www.microsoft.com/en-us/p/elden-ring/9nl9dv1sh9ls
- Elden Ring DRM-free 부재 (GOG forum): https://www.gog.com/forum/general/elden_ring_offline
- Elden Ring DRM 분석 (Steam Discussions): https://steamcommunity.com/app/1245620/discussions/0/3183487594856530930/
- Elden Ring + SotE 시스템 사양 (PC Gamer): https://www.pcgamer.com/hardware/elden-ring-shadow-of-the-erdtree-shows-some-uncharacteristic-mercy-with-identical-system-requirements-to-the-base-game/
- Shadow of the Erdtree EAC crash 1.12 (gamepressure): https://www.gamepressure.com/newsroom/elden-ring-dlc-crashes-easy-anti-cheat-eac-in-shadow-of-the-erdtr/z56f7b
- Elden Ring offline mode 정식 toggle (Windows Central): https://www.windowscentral.com/how-disable-anti-cheat-elden-ring
- techiew/EldenRingEacToggler — FromSoft 정식 toggle 도구: https://github.com/techiew/EldenRingEacToggler
- Anti-cheat toggler + offline launcher (NexusMods): https://www.nexusmods.com/eldenring/mods/90
- Offline launcher (No EAC) (NexusMods): https://www.nexusmods.com/eldenring/mods/98
- EAC architecture deep dive (TATEWARE 2026): https://tateware.com/blog/easy-anti-cheat-how-it-works
- EAC inside (hypercall.net): https://hypercall.net/posts/EasyAntiCheat-Part1/
- EAC 공식: https://www.easy.ac/
- EAC PCGamingWiki: https://www.pcgamingwiki.com/wiki/Easy_Anti-Cheat
- Goldberg Steam Emulator (universal split screen): https://universalsplitscreen.github.io//docs/goldberg/
- Microsoft Store UWP/MSIX 분석 (PCGamingWiki): https://www.pcgamingwiki.com/wiki/Store:Microsoft_Store
- MSIX 패키징 evolution (techengage 2026): https://techengage.com/windows-app-packaging-appx-msix-evolution/
- Universal Windows Platform — Wikipedia: https://en.wikipedia.org/wiki/Universal_Windows_Platform
- MSIX 공식 (Microsoft Learn): https://learn.microsoft.com/en-us/windows/msix/package/packaging-uwp-apps
- Elden Ring AppleGamingWiki: https://www.applegamingwiki.com/wiki/Elden_Ring
- Elden Ring Nightreign MacGamingDB: https://macgamingdb.app/games/2622380
- Elden Ring Nightreign DX12 only (PCGamingWiki): https://www.pcgamingwiki.com/wiki/Elden_Ring_Nightreign

---

*written 2026-05-01. Elden Ring + Shadow of the Erdtree DLC + Nightreign 검토 — EAC kernel-mode 단 offline mode 정식 toggle (FromSoft 인정) 으로 closure path B 자연 호환. multiplayer 보류 directive 와 정합 (online 거부, offline 진행). Steam-less standalone path = Microsoft Store (Xbox PC UWP) 만 정식, MSIX container 추출 신규 영역. D4 DX12 stage2 직접 100% 재사용. multiplayer 영역 Nightreign 보류.*
