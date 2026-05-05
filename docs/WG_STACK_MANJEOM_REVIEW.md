# Wargaming Stack (WoT + WoWS) — gamebox 만점 기준 적용 가능성 검토

> **status**: 검토일 **2026-05-01** / **만점 가능성: 매우 높음** (D2R/D4 와 동급, kernel-AC 부재 + Steam 의존 0)
> 작성: Phase 0 검토 (모듈 구현 X)
> SSOT 보조: `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` 6 phase 모델 재사용
> 결론 한줄: **Wargaming 자체 launcher (WGC) + proprietary user-mode AC + BigWorld→Core DX11 stack 으로 closure path B 가장 친화적인 후보 군. own 1 + 사용자 보류 directive 와 정합 (kernel AC 없음, multiplayer 우회 시도 불필요). D2R/D4 closure 후 즉시 진입 가능**.

---

## 0. 게임 메타데이터 (조사 결과)

### 0-1. World of Tanks (WoT) PC

| 항목 | 값 | 출처 |
|---|---|---|
| 정식 명칭 | World of Tanks (PC) | Wikipedia, Wargaming |
| 개발사 | Wargaming.net (Belarus, 본사 Cyprus 이전) | Wargaming |
| 출시 | 2010-08 (RU) / 2011-04 (NA/EU) — 활발 운영 중 | Wikipedia |
| 엔진 | **BigWorld → Core engine (1.0+ in-house)** — C++ + Python scripting | OC3D, Intel docs, Wargaming |
| Windows binary | **PE32+ x86_64** (Win10/11 64bit only) | Wargaming system requirements |
| 그래픽스 API | **DirectX 11** (DX9 legacy support 종료, DX12 미사용) | Wargaming Update 9.15 / 1.0 patch notes |
| Mesh shader / DXR | DXR 미사용 ("we didn't want to limit our players" 정책) | PCGamesN |
| 시스템 사양 (권장) | i3-9100F / Ryzen 3 3100, 16 GB RAM, GTX 1650 / RX 580, 85 GB SSD | Wargaming 공식 |
| **공식 distribution** | **WGC (Wargaming Game Center)** + Steam (옵션, 별도 binary) | Wargaming |
| **Steam 의존** | **0** — WGC 가 메인, Steam 은 부수적 | Wargaming |
| anti-cheat | **Wargaming proprietary** (user-mode 추정 — "not kernel-level"), 2026 HWID ban 도입 | Steam community thread |
| DRM | WG account binding (server-side), client-side DRM 약함 | Wargaming |
| 인증 | Wargaming.net account OAuth (email + password, optional 2FA) | Wargaming |
| macOS 공식 지원 | **없음** (Windows + console + mobile WoT Blitz) | Wargaming |
| Region | KR/EU/NA/SEA/RU separate servers (WGC region-aware) | Wargaming |
| 한국 status | KR 서버 활성 (`worldoftanks.kr`), local payment | Wargaming KR |

### 0-2. World of Warships (WoWS) PC

| 항목 | 값 | 출처 |
|---|---|---|
| 정식 명칭 | World of Warships (PC) | Wikipedia, Wargaming |
| 개발사 | Wargaming.net (Lesta Studio St. Petersburg, 2024+ 분리 — RU 단독 운영) | Wikipedia |
| 출시 | 2015-09 (글로벌) — 활발 운영 중 | Wikipedia |
| 엔진 | **BigWorld → Core engine** (WoT 와 동일 stack 추정) | Wargaming devblog |
| Windows binary | **PE32+ x86_64** | Wargaming |
| 그래픽스 API | **DirectX 11** (WoT 와 동일) | Wargaming |
| 시스템 사양 (권장) | i5-7400, 16 GB RAM, GTX 1060 6GB, 70 GB | Wargaming 공식 |
| **공식 distribution (3가지)** | **WGC + Steam + Epic Games Store + 직접 다운로드 (worldofwarships.com)** | Wargaming, LEVVVEL |
| **Steam 의존** | **0** — WG 직접 다운로드가 우선 (사용자 보고: WGC > Steam update 속도) | Wargaming |
| anti-cheat | **Wargaming proprietary** (WoT 와 동일 — user-mode 추정) | Steam community |
| 계정 | Wargaming.net account (cross-platform: WGC/Steam/EGS 공통 가능) | Wargaming |
| macOS 공식 지원 | 없음 | Wargaming |

### 0-3. WGC (Wargaming Game Center) — 자체 launcher 분석

| 항목 | 값 | 출처 |
|---|---|---|
| 형식 | Windows native PE32+ x86_64 multi-process app | Wargaming |
| 핵심 binary | `wgc.exe` (메인), `wgc_renderer_host.exe` (UI renderer), `WargamingErrorMonitor.exe`, game-specific installer (`world_of_tanks_install_*.exe`) | wargaming-net-game-center.software.informer.com |
| **추정 아키텍처** | **Electron / Chromium multi-process** (`*_renderer_host.exe` 명명 패턴 = Chrome multi-process, Electron 식 main+renderer 분리) | 명명 패턴 추론 |
| 통신 protocol | HTTPS + Wargaming.net API ("simple HTTP queries, almost unnoticeable resource usage") | Wargaming support |
| OAuth | WG account OAuth (Authorization Code + Token flow 추정) | Wargaming |
| update protocol | WG 자체 chunk delta update | Wargaming |
| anti-cheat | WGC 자체에는 anti-cheat 없음 (게임 client 안에 통합) | 추정 |

→ **WGC = .NET / WPF (PURPLE 식) 보다 더 가벼운 Electron/Chromium-based PC launcher**. closure path B 적용 시 PURPLE shell prerequisite 5 모듈 (CN-149~153) 의 일부 (특히 OAuth, WebView2/Chromium) 가 직접 흡수 가능.

---

## 1. 6 phase 별 만점 가능성 매트릭스 (D2R/D4 형식)

### Phase 1 — WGC install

**시나리오**: `worldoftanks.com` 또는 `worldofwarships.com` → installer.exe 다운로드 → WGC 설치 → game catalog 표시.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-WGC-PE64-MAP | 5 | 4 | 5 | 14 | D4 PE64 80% 재사용 |
| C-WGC-CHROMIUM-MULTI-PROC-TRACK | 5 | 5 | 5 | **15** | Electron/Chromium main+renderer 패턴 — `wgc_renderer_host.exe` IPC 추적 (PURPLE WebView2 와 generic) |

**phase 결론**: **부분 가능** (만점 +1, 보조 +1). install 단계는 anti-cheat 비활성.

### Phase 2 — WG OAuth + WGC shell 부팅

**시나리오**: Wargaming.net account login → OAuth Authorization Code → token cache → WGC main 화면 (game catalog).

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-WG-OAUTH-PROXY | 5 | 5 | 5 | **15** | Battle.net / Steam / PLAYNC OAuth 와 동형 — generic OAuth 모듈 흡수 후보 |
| C-WGC-CHROMIUM-IPC-WARM | 5 | 5 | 5 | **15** | Chromium multi-process IPC warm-up (PURPLE WebView2 와 공유 가능) |

**phase 결론**: **가능** (만점 +2). lobby crash 위험 PURPLE 보다 낮음 (Electron/Chromium 안정적).

### Phase 3 — game CDN download

**시나리오**: WGC 안에서 사용자가 WoT/WoWS 선택 → CDN download (Wargaming 자체 CDN, ~85GB/70GB) → SHA verify → install.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-WG-CDN-CHUNK-PARALLEL | 5 | 4 | 5 | 14 | C-CDN-PARALLEL-CHUNK 재사용 (D4 / PURPLE 과 generic) |
| C-WG-DELTA-UPDATE-PROTO | 5 | 5 | 5 | **15** | WGC 자체 chunk delta update protocol (Steam content depot 와 다른 방식, 자체 reverse 필요) |

**phase 결론**: **가능** (만점 +1, 보조 +1).

### Phase 4 — game launch + first frame (DX11 + BigWorld)

**시나리오**: WGC → WoT/WoWS .exe → BigWorld init → DX11 device → first frame.

**proprietary user-mode AC 영향 분석**:
- Wargaming 자체 anti-cheat = **user-mode 추정** (kernel driver 부재 보고)
- DIY hexa loader 의 fake env (PEB, KUSER_SHARED_DATA 등) 가 user-mode AC 의 무결성 검증 통과 가능성 **~50%** (D2R 보다는 위험, kernel AC 보다는 훨씬 안전)
- HWID ban (2026) 은 **multiplayer 영역** — 사용자 보류 directive 적용 시 실 multiplayer 진입 X → HWID ban 위험 X

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-WOT-DX11-FIRST-FRAME | 5 | 5 | 5 | **15** | WoT first frame trace (DX11 + BigWorld → Core) |
| C-WOWS-DX11-FIRST-FRAME | 5 | 5 | 5 | **15** | WoWS first frame trace (동일 stack) |
| C-BIGWORLD-PYTHON-SCRIPT-CACHE | 5 | 5 | 5 | **15** | BigWorld Python scripting layer 추적 (game logic 캐싱) |

**phase 결론**: **가능 (offline / training room)**, **online (multiplayer)** 은 사용자 보류 directive 적용 → skeleton-only.

### Phase 5 — gameplay (training room / replay)

**시나리오**: WoT/WoWS 의 **training room** (혼자 + bot 가능) 또는 **replay 재생** 모드 — multiplayer 거부 directive 와 정합.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-WOT-TRAINING-ROOM-PROFILE | 5 | 5 | 5 | **15** | training room (single + bots) frame trace — multiplayer 거부와 분리 |
| C-WOWS-REPLAY-PLAYBACK | 5 | 5 | 5 | **15** | WoWS replay 재생 (자체 .wowsreplay 파일, offline) |
| C-BIGWORLD-CORE-SHADER-PERSIST | 5 | 5 | 5 | **15** | Core engine shader cache 영구화 (cold-launch 가속) |

**phase 결론**: **가능** (offline path 만, multiplayer 보류 directive 적용).

### Phase 6 — live tune

**phase 결론**: **가능** (training room / replay 30+ 시간 측정 가능).

### 매트릭스 요약

| Phase | 만점 가능성 | ETA | 차단 사유 |
|---|---|---|---|
| 1. WGC install | 부분 (만점 +1, 보조 +1) | 0.3개월 | — |
| 2. WG OAuth + WGC | 가능 (+2 만점) | 0.5개월 | — |
| 3. CDN download | 가능 (+1 만점, +1 보조) | 0.3개월 | — |
| 4. first frame | **가능 (offline / training)** | 1개월 | proprietary user-mode AC (~50% pass) |
| 5. gameplay | **가능 (training room / replay)** | 1개월 | multiplayer 보류 directive (HWID ban 위험 회피) |
| 6. live tune | 가능 | 0.5개월 | — |

→ **만점 도달 가능 (training/replay)**: 만점 +9, 보조 +2.
→ **만점 가능성 비율**: ~85% (6 phase 중 5+ phase 도달 가능).
→ **D2R/D4 와 동급** (D2R 의 single-player + offline equivalent).

---

## 2. anti-cheat 분석

### 2-1. Wargaming proprietary AC (user-mode 추정)

| 비교 | Wargaming AC | nProtect GameGuard | EAC | ACE |
|---|---|---|---|---|
| Mode | **user-mode 추정** | kernel | kernel + user | kernel + user |
| Wine 호환 | 보고됨 (Lutris compat list) | unfixable | 일부 user-mode 만 | 미호환 |
| HWID ban (2026 도입) | ✓ — multiplayer 한정 | ✓ | ✓ | ✓ |
| airgenome 호환 | **~50% 추정** (user-mode integrity 검증만 통과 시) | 0% | 0% | 0% |

→ **kernel driver 부재** = closure path B 의 본질 차단 없음. user-mode integrity 검증 (PEB, ntdll/kernel32 hash 등) 만 통과 시 entry call 도달 가능.

### 2-2. multiplayer 보류 directive 와의 정합성

사용자 directive ("multiplayer 는 정의상 거부 때문에 보류") 를 WoT/WoWS 에 적용:

- **online battle (multiplayer 진입)**: ✗ 보류 — HWID ban 위험 + own 1 정신
- **training room (single + bot)**: ✓ 진행 — 자체 player 만 사용, multiplayer 진입 X
- **replay 재생** (WoWS): ✓ 진행 — offline .wowsreplay 파일 재생
- **garage / 항만 (lobby UI)**: ✓ 진행 — battle 진입 X

→ training room + replay 만으로도 phase 4-6 만점 도달 가능.

---

## 3. closure path B 호환성

| step | D2R | D4 | **WoT** | **WoWS** |
|---|---|---|---|---|
| PE map | i386 | x86_64 | **x86_64 (Core engine)** | x86_64 (동일) |
| IAT bind | DX11 | DX12+DXR | **DX11 + BigWorld + Python** | DX11 + BigWorld + Python |
| entry call | offline 가능 | online DRM | **WG account binding (offline 가능 — training)** | 동일 |
| anti-cheat | 없음 | 없음 (DRM) | **proprietary user-mode (~50% pass)** | 동일 |
| DLL coverage prerequisite | 일반 + DX11 | 일반 + DX12 | **일반 + DX11 + Python embed (Python 27/3x DLL)** | 동일 |
| DXR / Mesh shader | 없음 | DXR done | **없음 (정책)** | 없음 |
| Apple D3DMetal 매핑 | DX11 stage1 | DX12 stage2 | **DX11 stage1 직접 재사용** | DX11 stage1 직접 재사용 |
| WGC launcher (Chromium IPC) | — | — | **신규 — Electron/Chromium multi-proc IPC** | 동일 |

**핵심 발견**:
- 그래픽스 stack = **D2R DX11 stage1 직접 100% 재사용**
- Python embed = 신규 (BigWorld 의 Python scripting layer) — Lineage 외 game 처음
- WGC = Electron/Chromium = PURPLE WebView2 와 generic 흡수 가능
- AC 본질 차단 없음 (kernel 부재) → entry call 도달 확률 매우 높음

**호환성 점수**: **~90%** (D2R 수준 또는 그 이상).

---

## 4. 추천 skeleton 모듈 candidate

### 4-1. WGC shell prerequisite (양 게임 공통 — generic Electron/Chromium 인프라)

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 1. `c_wgc_pe64_loader_probe` | 1 | 5 | 4 | 5 | 14 | D4 PE64 + PURPLE (.NET) 패턴 융합 |
| 2. `c_wgc_chromium_multi_proc_track` | 1-2 | 5 | 5 | 5 | **15** | `wgc.exe` ↔ `wgc_renderer_host.exe` IPC 추적 — Chromium multi-process generic |
| 3. `c_wg_oauth_proxy` | 2 | 5 | 5 | 5 | **15** | WG account OAuth — Battle.net / Steam / PLAYNC / Tencent 와 동형 (generic OAuth 흡수 후보 5번째 launcher) |
| 4. `c_wg_cdn_chunk_parallel` | 1, 3 | 5 | 4 | 5 | 14 | C-CDN-PARALLEL-CHUNK 재사용 (D4 / PURPLE / Tencent 와 generic) |
| 5. `c_wg_delta_update_proto` | 3 | 5 | 5 | 5 | **15** | WGC chunk delta update — Steam content depot 와 다른 방식, 신규 |

### 4-2. BigWorld + Core engine 공통 (양 게임)

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 6. `c_bigworld_core_dx11_first_frame` | 4 | 5 | 5 | 5 | **15** | BigWorld → Core engine DX11 first frame trace |
| 7. `c_bigworld_python_script_cache` | 4-5 | 5 | 5 | 5 | **15** | Python scripting layer 캐싱 (BigWorld 특이 — game logic 가속) |
| 8. `c_bigworld_core_shader_persist` | 4-5 | 5 | 5 | 5 | **15** | Core engine shader cache 영구화 |

### 4-3. WoT specific

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 9. `c_wot_training_room_profile` | 5 | 5 | 5 | 5 | **15** | training room (single + bots) frame trace — multiplayer 보류 directive 정합 |
| 10. `c_wot_garage_warmup` | 4-5 | 5 | 5 | 5 | **15** | garage (lobby) UI warm-up — battle 진입 X |

### 4-4. WoWS specific

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 11. `c_wows_replay_playback` | 5 | 5 | 5 | 5 | **15** | .wowsreplay 파일 재생 — offline 100% 가능 |
| 12. `c_wows_port_warmup` | 4-5 | 5 | 5 | 5 | **15** | port (항만) UI warm-up |

### 4-5. multiplayer 보호 (사용자 보류 directive 정합)

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 13. `c_wg_battle_entry_block` | 0 | 5 | 0 | 0 | 5 | **보호 모듈** — battle 진입 시 자동 차단 + HWID ban 위험 고지. CS2 VAC guard / DF block doc 와 동형 |

→ **추천 모듈 수: 13개** (실 가치: 모두).

---

## 5. 결론 — 우선순위

### 5-1. 만점 가능성 종합

| 항목 | 값 |
|---|---|
| **만점 가능성 (training / replay)** | **가능 ~85%** (D2R/D4 와 동급) |
| **만점 가능성 (multiplayer)** | 보류 (사용자 directive 영구 거부) |
| 도달 가능 만점 점수 | +9 만점 + 보조 +2 (Phase 1-6 training 한정) |
| validated 만점 가능성 | **가능** — kernel AC 부재, user-mode 통과 가능성 ~50% |
| skeleton 만점 가능성 | +13 (모두 작성 가능) |
| BAN risk | **낮음 (training/replay 한정)** — multiplayer 보류 시 HWID ban risk 0 |
| gamebox identity 와의 정합성 | **정합** — Wine 0줄, kernel AC 우회 시도 X, multiplayer 거부 directive 자연 정합 |

### 5-2. D2R / D4 / Lineage / CS2 / DF 와 종합 우선순위

```
1. D2R closure 완료              ← 현재 진행
2. D4 closure 완료                ← Phase 10 dependent
3. (병렬) PURPLE shell + Lineage 클래식 r28 + L M / L W (cycle 49 done)
4. (병렬) **Wargaming Stack (WoT + WoWS)** — kernel AC 없음, training/replay validated 가능
5. CS2 offline path (cycle 51-52)
6. Lineage 2 private (l2j) opt-in
7. 영구 미진입: Delta Force / TL / Lineage 2M / W live (kernel AC)
```

→ **WG stack 은 D4 closure 직후 즉시 진입 가능** — generic OAuth/Chromium IPC 인프라 흡수 측면에서 가장 우선순위 높음 (5번째 launcher generic 화).

### 5-3. cycle 50+ batch 후보 (WG stack)

**cycle 50 후보 (8 모듈 — WGC shell prerequisite + BigWorld core)**:
- CN-X1 c_wgc_pe64_loader_probe (14)
- CN-X2 c_wgc_chromium_multi_proc_track (15)
- CN-X3 c_wg_oauth_proxy (15)
- CN-X4 c_wg_cdn_chunk_parallel (14)
- CN-X5 c_wg_delta_update_proto (15)
- CN-X6 c_bigworld_core_dx11_first_frame (15)
- CN-X7 c_bigworld_python_script_cache (15)
- CN-X8 c_wg_battle_entry_block (5 — 보호)

→ skeleton +7 만점 + 보조 +1.

**cycle 51 후보 (4 모듈 — game-specific)**:
- CN-X9 c_bigworld_core_shader_persist (15)
- CN-Y1 c_wot_training_room_profile (15)
- CN-Y2 c_wot_garage_warmup (15)
- CN-Y3 c_wows_replay_playback (15)
- CN-Y4 c_wows_port_warmup (15)

→ 단독 cycle 부족 (5 모듈), 다른 영역 (CS2 offline / generic 인프라) 3 모듈 결합 권장.

### 5-4. 미래 가능성 (조건부)

다음 이벤트 발생 시 §1-5 재작성:
- **Lesta Studio (WoWS RU 단독) 가 WoWS 를 NA/EU 에서 분리** — 사용자 자산 영향 가능
- **Wargaming 이 macOS native 빌드 발표** — 매우 가능성 낮음
- **Apple Silicon Mac 에서 Wargaming 공식 지원** — 가능성 낮음
- **Core engine UE5 transition** — Lineage 2 NEXT GEN 처럼 — 가능성 낮음

---

## 6. 출처 (WebSearch 결과 인용)

- World of Tanks — Wikipedia: https://en.wikipedia.org/wiki/World_of_Tanks
- BigWorld engine — PCGamingWiki: https://www.pcgamingwiki.com/wiki/Engine:Bigworld
- Wargaming Game Center 공식 페이지: https://wargaming.net/en/wgc
- WoT 시스템 사양 공식: https://worldoftanks.com/en/content/guide/general/system-requirements/
- WoT Update 9.15 DX11 추가: https://worldoftanks.asia/en/news/general-news/technical-core-3-0/
- WoT 1.0 Core engine + DXR 비채택 정책: https://www.pcgamesn.com/world-of-tanks/dxr-windows-10-dx12-ray-tracing
- WoT Multi-core 41% FPS gain (BigWorld 한계): https://www.gamedev.net/news/world-of-tanks-multi-core-sees-41-fps-increase-r813/
- WoT 1.0+ CPU optimization (Intel docs): https://www.intel.com/content/dam/develop/external/us/en/documents/world-of-tanks-1-0plus-enrich-user-experience-with-cpu-optimized-graphics-and-physics.pdf
- WoT anti-cheat user-mode 추정 (Steam community): https://steamcommunity.com/app/1407200/discussions/0/787664130474528268/
- WGC how it utilizes PC resources: https://wargaming.net/support/en/products/wgc/article/15746/
- WGC executable files 분석 (informer): https://wargaming-net-game-center.software.informer.com/
- WGC installation guide: https://worldoftanks.com/en/content/guide/general/wgc_guide/
- World of Warships — Wikipedia: https://en.wikipedia.org/wiki/World_of_Warships
- WoWS 3가지 distribution path (LEVVVEL): https://levvvel.com/how-to-download-world-of-warships/
- WoWS install guide (Wargaming support): https://wargaming.net/support/en/products/wows/article/15536/
- WoWS Steam FAQ (account migration / cross-play): https://wargaming.net/support/en/products/wows/article/10690/
- WoWS proprietary anti-cheat (Steam community): https://steamcommunity.com/app/552990/discussions/0/3424438414131089012/
- WoWS export automation + BigWorld 사용 확인: https://cabel-set.ru/en/lentochnyjj-fundament/kak-my-delaem-world-of-warships-avtomatizaciya-eksporta-i-verifikaciya-kontenta/

---

*written 2026-05-01. WoT + WoWS 통합 검토 — Wargaming proprietary user-mode AC + WGC standalone (Steam 의존 0) + BigWorld→Core DX11 stack. closure path B 가장 친화적 후보 군으로 D4 closure 직후 즉시 진입 가능. multiplayer 보류 directive 와 정합 (training room + replay 만으로도 phase 4-6 도달).*
