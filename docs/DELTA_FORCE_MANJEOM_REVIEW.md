# Delta Force (2024 reboot) 만점 검토

> **status**: 검토일 **2026-05-01** / 갱신 **2026-05-01-rev2** (사용자 directive + WebSearch 후속) / **만점 가능성: 불가능 (kernel-level anti-cheat closure path B 차단)**
> 작성: 2026-05-01
> SSOT: docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md (단계 형식 참조)
> 결론 한줄: **closure path B 의 PE map / IAT bind / entry call 가 ACE kernel driver requirement 와 정면 충돌 → install 까지는 부분 가능, gameplay 만점 불가**.

---

## ⚠️ 0.5 정정 / 추가 (rev2 — 2026-05-01 사용자 directive 후속 research)

### 0.5-1. 2025-12 ~ 2026 ACE 강화 사항 (web search update)

기존 §2-1 의 ACE kernel-mode 분석 후 발생한 update — 모두 **차단 강화**, §2-4 결론 변경 X (오히려 강화):

| 사건 | 시점 | 변화 |
|---|---|---|
| **DMA Shield** (Enhanced Hardware DMA Protection) | **2025-12** | Windows Kernel DMA Protection 위에 ACE 추가 layer — 외부 DMA 카드 (PCIe-based cheat hardware) 까지 차단. cheat ecosystem 핵심 vector 제거 |
| **ACE AI Security Framework** | **2026 GDC** (2026-03) | Tencent ACE 가 large-scale replay 분석 + AI model 기반 anti-cheat framework 발표 (Delta Force + Arena Breakout 적용). cheat 패턴 자동 학습 / detection |
| **iOS Hardening** | 2026 GDC | ACE 가 iOS anti-cracking solution 발표 — 향후 mobile 영역까지 확대 |
| **G.T.I. Security 전담 task force** | 활성 | "Game Threat Intelligence" 명칭, ACE 와 Tencent 협업, 1729 cheaters/week ban (효과 입증) |
| Steam Deck 지원 | 2024-12 발언만, 미실현 | LCD model 만 whitelist (other Linux 차단) — Proton bypass 시도도 ACE 가 unsafe 환경 탐지 |

→ **closure path B 영구 차단** 결론 강화. ACE 의 macOS 친화적 변화 0.

### 0.5-2. Multiplayer **보류** 확정 (사용자 directive 2026-05-01)

> 사용자 directive: "multiplayer 는 정의상 거부 때문에 보류"

본 검토 §2-3 의 결론 (own 1 + anti-cheat 우회 거부 정책) 을 사용자가 명시 재확인 → **ACE 우회 / kernel attest 우회 / Tencent ID multiplayer 진입 시도 모두 영구 보류**.

해당 영역에서 보류된 항목:
- ACE kernel attest 우회 시도
- TenSafe.sys / ACE-*.sys driver bypass
- Tencent ID + HWID 위협 행위
- Delta Force online / coop campaign 진입 시도

### 0.5-3. 본 검토에서 진행 가능 영역 (보류 외 — Phase 1-3 + 보호)

§4 candidate list 중 multiplayer 와 분리 가능한 install / login / CDN / 보호 모듈만 진행:

| candidate | 영역 | 진행? |
|---|---|---|
| `c_dforce_pe64_loader_probe` (§4-1) | Phase 1 PE map | ○ — install 단계, AC 비활성 |
| `c_dforce_ue5_iat_classify` (§4-2) | Phase 1 IAT | ○ — UE5 표준 패턴, generic UE5 재사용 |
| `c_dforce_steam_oauth_proxy` (§4-3) | Phase 2 OAuth | ○ — generic 흡수 후보 |
| `c_dforce_tencent_auth_probe` (§4-4) | Phase 2 Tencent ID | ○ — endpoint enumerate (실 인증 진입 X) |
| `c_dforce_pak_layout_predict` (§4-5) | Phase 3 CDN | ○ — UE5 .pak prefetch, generic UE5 재사용 |
| `c_dforce_unsupported_block_doc` (§4-9) | Phase 0 보호 | ○ — Delta Force detect 자동 차단 + BAN 위험 고지 |
| `c_dforce_ace_attest_simulate` (§4-6) | Phase 4 검토 | △ — 실 우회 X, 실패 모드 enumerate 만 (사용자 보류 directive 와 경계 — skeleton 작성 시 우회 의도 없음 명시) |
| `c_dforce_dxr_lumen_warm_skeleton` (§4-7) | Phase 4 skel | △ — generic UE5 + DXR 인프라 (다른 UE5 게임 재사용 우선, DF validated 0) |
| `c_dforce_nanite_meshlet_opt_skeleton` (§4-8) | Phase 4 skel | △ — generic UE5 mesh-cull (D4 재사용) |

→ ✓ 6 candidates 즉시 진행, △ 3 candidates 우선순위 낮음 (단순 generic UE5 인프라 가치만)

### 0.5-4. 신규 candidates 보류 (rev2 web search 발견)

Web search 에서 발견한 다음 신규 candidates 는 **사용자 보류 directive 영역** (multiplayer 본체 분석) → 영구 보류:
- ~~`c_dforce_dma_shield_track`~~ — DMA Shield 추적 = multiplayer 진입 의도와 분리 곤란 → 보류
- ~~`c_dforce_ace_ai_security_track`~~ — AI security framework 추적 = multiplayer 영역 → 보류

### 0.5-5. cycle 50 batch 후보 (사용자 보류 directive 반영)

multiplayer / kernel attest 영역 모두 제외, Phase 1-3 + 보호 모듈만:

**cycle 50 (6 모듈 — DF Phase 1-3 + 보호)**:
- CN-157 c_dforce_pe64_loader_probe (14, P1)
- CN-158 c_dforce_ue5_iat_classify (14, P1)
- CN-159 c_dforce_steam_oauth_proxy (15, P2)
- CN-160 c_dforce_tencent_auth_probe (15, P2)
- CN-161 c_dforce_pak_layout_predict (15, P3)
- CN-162 c_dforce_unsupported_block_doc (5, P0 보호)

→ 6 modules. 8-batch 채우려면 다른 영역 (CS2 offline / generic 인프라) 2 모듈 합치거나 6 모듈 batch 진행.

### 0.5-6. 출처 추가 (rev2 web search)

- DMA Shield 발표 (Delta Force Garena 공식): https://deltaforce.garena.com/en/news/system/W5CAW5
- ACE GDC 2026 AI Security Framework + iOS Hardening (Games Press): https://www.gamespress.com/Anti-Cheat-Expert-ACE-Unveils-Industry-First-Anti-Cracking-iOS-Hardeni
- ACE Anti-Cheat Expert (PCGamingWiki): https://www.pcgamingwiki.com/wiki/Anti-Cheat_Expert
- 1729 cheaters/week ban (DualShockers): https://www.dualshockers.com/delta-force-bans-cheaters-backlash-invasive-anti-cheat/
- G.T.I. Security 공식: https://www.playdeltaforce.com/en/anti-cheat.html

---

---

## 0. 게임 메타데이터 (조사 결과)

| 항목 | 값 | 출처 |
|---|---|---|
| 정식 명칭 | Delta Force (2024 reboot, 별칭 Hawk Ops / Black Hawk Down) | Wikipedia |
| 개발사 | Team Jade (TiMi Studio Group, Tencent 산하) | Wikipedia, Gematsu |
| 퍼블리셔 | TiMi Studio Group / Tencent | Wikipedia |
| 엔진 | **Unreal Engine 5** (Lumen GI, Virtual Shadows, Ray-traced reflections) | DSOGaming, Epiccarry |
| 출시 (PC OBT) | **2024-12-05** | Niche Gamer, Gematsu |
| 출시 (mobile iOS/Android) | 2025-04-21 | Wikipedia |
| 출시 (PS5/XSX) | 2025-08-19 | Wikipedia |
| Windows binary 포맷 | **PE32+ x86_64** (Win10 64-bit only — 32-bit 미지원) | gamesystemrequirements.com, Dexerto |
| 그래픽스 API | **DirectX 12** (DX11 fallback 없음 — UE5 의존) | gamesystemrequirements.com, esports.gg |
| Mesh shader / DXR | DXR (ray-traced reflections) 사용 — UE5 Lumen 경로. Mesh shader 사용여부 미확인 (추정: UE5 Nanite 활성화 시 mesh shader fallback 가능) | DSOGaming |
| Upscaler | DLSS 4, FSR 3.1, XeSS 2 (DX12 + UE5 plugin) | DSOGaming, Epiccarry |
| anti-cheat | **ACE (Anti-Cheat Expert) — Tencent kernel driver** (`ACE-*.sys`, `C:\Windows\System32\drivers`) | 80.lv, PCGamingWiki, Wikipedia (ACE) |
| 안티치트 mode | **Ring 0 (kernel-level)**. 게임 종료 후에도 driver 잔류 (uninstall 시 manual 삭제 필요했으나 patch 후 자동 제거) | 80.lv, Steam Community |
| ACE user-mode variant | 일부 ACE 변종은 user-mode 만으로 작동 (Wine Staging 10.5+) — **단 Delta Force 는 kernel variant 사용으로 보고됨** | PCGamingWiki, GamingOnLinux |
| 온라인 의무 | **항상 온라인 필수**. Black Hawk Down "campaign" 도 online lobby 요구 (4-player coop, 진정한 offline 불가) | gamepressure, PCGamesN |
| DRM | TenCenter / Tencent server-side auth (Battle.net 식 OAuth 와 유사 — 추정) | 추정 |
| macOS 공식 지원 | **없음** (Windows / mobile / 콘솔 만) | Steam page |
| macOS 비공식 | CrossOver / Porting Kit / Whisky 시도 — **kernel ACE 로 인해 모두 실패 또는 BAN risk**. AppleGamingWiki page 존재 (403 — fetch 실패, 추정: 미실행) | CodeWeavers, applegamingwiki |
| Steam Deck / Linux | Team Jade 가 "검토하겠다" 발언 (2024-12) — 현재 미지원. ACE 가 Proton 을 unsafe 로 탐지 | GamingOnLinux |
| 예상 DLL set (UE5 + Tencent stack — **추정**) | KERNEL32, USER32, GDI32, ADVAPI32, OLE32, OLEAUT32, SHELL32, SHLWAPI, WS2_32, WININET, WINHTTP, CRYPT32, BCRYPT, DBGHELP, MSVCP140, VCRUNTIME140, UCRTBASE, D3D12, DXGI, DXCORE, dxcompiler, XINPUT1_4, XAUDIO2_9, NTDLL, **+ ACE-base.dll / ACE-game.dll / TenSafe.sys (kernel)** | UE5 표준 + ACE PCGamingWiki 추정 |

**비고**: NovaLogic 시리즈 (1998 원작 / Black Hawk Down 2003) 와 별개 — 2024 reboot 은 완전 신규 codebase, Tencent/UE5 기반.

---

## 1. 6 phase 별 만점 가능성 매트릭스 (D2R/D4 형식)

### Phase 1 — Tencent / Steam launcher install

**시나리오**: Steam (또는 Tencent 자체 launcher) 가 Delta Force 다운로드 + 디스크 설치.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-DFORCE-PE64-MAP (PE32+ x86_64 map) | 5 | 4 | 5 | 14 | D4 와 동일 — D4 인프라 재사용 가능 |
| C-DFORCE-IAT-RESOLVE (UE5 표준 import 해상) | 5 | 4 | 5 | 14 | D4 IAT 와 ~80% overlap (추정) |
| C-DFORCE-INSTALL-PROBE (Steam manifest / Tencent installer 파싱) | 5 | 4 | 5 | 14 | 신규 |

**phase 결론**: **부분 가능** (보조 +3). install 단계는 anti-cheat 비활성 — 가능.
**ETA (이론)**: 0.5 개월 (D4 PE/IAT 인프라 재사용 시).

---

### Phase 2 — login / launcher runtime

**시나리오**: Steam OAuth (또는 Tencent ID) → 게임 catalog → 다운로드 시작.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-DFORCE-STEAM-OAUTH-PROXY | 5 | 5 | 5 | **15** | C-OAUTH-FLOW-PROXY (Battle.net) 와 동형 — generic 화 가능 |
| C-DFORCE-TENCENT-AUTH-PROXY | 5 | 5 | 5 | **15** | Tencent ID flow (추정 — RSA + token cache) |

**phase 결론**: **가능** (만점 +2 후보, 단 Battle.net OAuth 모듈과 generic 합치면 추가 +0).
**ETA**: 0.5-1 개월.

---

### Phase 3 — CDN download + verify

**시나리오**: ~88GB asset CDN 다운로드 + SHA verify + UE5 pak 파일 layout.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| C-DFORCE-PAK-LAYOUT-PREDICT (UE5 .pak prefetch) | 5 | 5 | 5 | **15** | C-DISK-LAYOUT-PREDICT 와 유사 — UE5 specific |
| C-DFORCE-CDN-PARALLEL-CHUNK | 5 | 4 | 5 | 14 | C-CDN-PARALLEL-CHUNK 재사용 |

**phase 결론**: **가능** (만점 +1).
**ETA**: 0.5 개월 (인프라 재사용).

---

### Phase 4 — first frame (DX12 + UE5 + DXR)

**시나리오**: Delta Force.exe entry → UE5 init → D3D12 device → DXR pipeline → 첫 frame.

**ACE kernel driver 차단 위험 (CRITICAL)**:
- ACE `TenSafe.sys` 또는 `ACE-*.sys` 가 게임 entry 직후 game process 에 **PsSetCreateProcessNotifyRoutine** + **ObRegisterCallbacks** 등 kernel callback 등록.
- airgenome-gamebox 는 macOS native — Windows kernel driver 절대 load 불가.
- ACE 가 `ntdll!NtQueryVirtualMemory`, `kernel32!GetModuleHandle` 등을 통해 PE 의 **proper Windows kernel 환경** 검증 → DIY hexa loader 의 fake env 즉시 탐지.
- 결과: **game process 가 attest 실패로 self-terminate 또는 server kick**.

| 모듈 후보 | A | B | C | 만점 | 차단 가능성 |
|---|---|---|---|---|---|
| C-DFORCE-UE5-FIRST-FRAME-TRACE | 5 | 5 | 5 | **15** | **차단됨** — ACE pre-check 실패 |
| C-DFORCE-D3D12-PIPELINE-WARM | 5 | 5 | 5 | **15** | **차단됨** |
| C-DFORCE-DXR-LUMEN-WARM | 5 | 5 | 5 | **15** | **차단됨** |
| C-DFORCE-NANITE-MESHLET-OPT | 5 | 5 | 5 | **15** | **차단됨** |

**phase 결론**: **불가능** (kernel ACE 가 closure path B entry call 검증을 통과하지 못함).
**ETA**: ∞ (anti-cheat 우회 = TOS 위반 + BAN, gamebox identity 와 정면 모순).

---

### Phase 5 — gameplay 만점

**phase 결론**: **불가능** (Phase 4 차단으로 실 frame 0 → validated 0).
gameplay loop 절대 도달 불가.

---

### Phase 6 — live tune

**phase 결론**: **불가능** (Phase 5 미도달).

---

### 매트릭스 요약

| Phase | 만점 가능성 | ETA | 차단 사유 |
|---|---|---|---|
| 1. install | 부분 (보조 +3) | 0.5개월 | — |
| 2. login | 가능 (+2 만점 후보) | 0.5-1개월 | — |
| 3. CDN download | 가능 (+1 만점) | 0.5개월 | — |
| 4. first frame | **불가능** | ∞ | **ACE kernel driver** |
| 5. gameplay | **불가능** | ∞ | Phase 4 종속 |
| 6. live tune | **불가능** | ∞ | Phase 5 종속 |

→ **최종 도달 가능 만점: +3 (Phase 2-3 만점), 보조 +3 (Phase 1)**.
→ **만점 가능성 비율: ~20%** (전체 6 phase 중 1.5 phase 가능).

---

## 2. anti-cheat 차단 위험 평가

### 2-1. ACE 의 작동 mode

| 변종 | 작동 | airgenome 호환 |
|---|---|---|
| **kernel mode (Delta Force 가 사용)** | `ACE-*.sys` driver, Ring 0, kernel callback | **불가능** — macOS native + DIY hexa loader 환경에서 Windows kernel driver load 불가 |
| user mode (일부 다른 ACE 게임) | DLL injection + memory scan, Ring 3 | 이론상 가능 — 단 Delta Force 는 사용 안 함 (출처 확인) |

### 2-2. ACE 가 closure path B 에 미치는 효과

closure path B 의 3 step:
1. **PE map** — DIY hexa loader 가 PE32+ → memory image 매핑.
2. **IAT bind** — Windows DLL 의 함수를 hexa shim 으로 fix-up.
3. **entry call** — `_DllMainCRTStartup` 또는 `WinMainCRTStartup` 호출.

ACE 는 step 3 직후 (또는 직전) 다음을 검증:
- `KUSER_SHARED_DATA` (0x7FFE0000) 의 시그니처 + Windows build number.
- `PEB->Ldr` linked list 의 모듈들이 정식 Windows DLL 인지 (DIY shim 식별 risk).
- syscall stub 의 hash (`ntdll!Nt*` 가 Windows MS-signed 인지).
- `TenSafe.sys` driver 가 process 에 attached 되어 있는지 — **driver 자체가 kernel mode 라 macOS 에서 절대 load 불가**.
- self-integrity checksum (PE map 시 .text 영역 hash).

→ airgenome-gamebox 의 hexa loader 환경에서 위 5개 검증 모두 **실패 보장**.
→ game process 는 **연결 시도 즉시 server-side BAN** 가능 (HWID 기반).

### 2-3. BAN risk

- 사용자 계정 (Tencent ID + Steam) **영구 BAN** 가능.
- HWID BAN — Mac UUID / serial 이 ACE blacklist 등록 → 향후 다른 ACE 게임 (PUBG Mobile, Honor of Kings PC 등) 까지 영향.
- airgenome-gamebox identity ("own 1: Wine 0줄") 와 anti-cheat 우회는 **상충** — anti-cheat bypass 는 own 0 (회피) 정신과 다른 방향.

### 2-4. 결론

**Delta Force 의 kernel-level ACE 는 closure path B 와 본질적으로 호환 불가**. user-mode shim 으로 절대 우회 불가능 (Windows kernel API 의존).

---

## 3. closure path B 호환성

| step | D2R | D4 | **Delta Force** |
|---|---|---|---|
| PE32 i386 / PE32+ x86_64 map | i386 | x86_64 | **x86_64** (D4 path 재사용 가능) |
| IAT bind (UE5 / Tencent stack) | DX11 | DX12+DXR | **DX12+DXR+UE5+ACE-DLL** |
| entry call | offline 가능 | online-only DRM | **online-only + ACE kernel attest** |
| anti-cheat | 없음 (offline mode) | 없음 (DRM 만) | **kernel ACE — 차단** |
| DLL coverage prerequisite | 일반 Win + DX11 | 일반 Win + DX12 | 일반 Win + DX12 + DLSS/FSR/XeSS plugin + **ACE proprietary DLL** |
| DXR / Mesh shader | 없음 | DXR done | DXR (Lumen) — D4 stage2 인프라 재사용 |
| Apple D3DMetal 매핑 | DX11 stage1 done | DX12 stage2 done | **stage2 재사용 가능 — 단 Phase 4 도달 불가** |

**핵심 발견**:
- 그래픽스 stack 만 보면 **D4 인프라 90% 재사용 가능** (DX12+DXR+UE5).
- 그러나 ACE kernel attest 가 entry call 통과 **불가능**하게 만듦.
- Apple Silicon native 환경 자체가 ACE 입장에서 "비정상 환경" — driver 부재만으로 실패.

---

## 4. 추천 skeleton 모듈 candidate (CN 번호 미정)

다음은 **install / login / CDN phase 까지 만 의미 있는** skeleton candidate. Phase 4 이후는 모듈 작성해도 validated 도달 불가하므로 후순위.

| candidate | phase | A | B | C | 만점 | 재사용도 |
|---|---|---|---|---|---|---|
| 1. **c_dforce_pe64_loader_probe** | 1 | 5 | 4 | 5 | 14 | D4 PE64 loader 와 80% 공유 |
| 2. **c_dforce_ue5_iat_classify** | 1 | 5 | 4 | 5 | 14 | UE5 표준 IAT 패턴 분류 (다른 UE5 게임 재사용) |
| 3. **c_dforce_steam_oauth_proxy** | 2 | 5 | 5 | 5 | **15** | C-OAUTH-FLOW-PROXY generic화로 흡수 가능 |
| 4. **c_dforce_tencent_auth_probe** | 2 | 5 | 5 | 5 | **15** | Tencent endpoint enumerate (추정 RSA / TLS pinning) |
| 5. **c_dforce_pak_layout_predict** | 3 | 5 | 5 | 5 | **15** | UE5 .pak prefetch — 다른 UE5 게임 재사용 |
| 6. **c_dforce_ace_attest_simulate** | 4 | 5 | 3 | 5 | 13 | **검토 only** — 실제 attest 통과 시도 X (TOS 회피). 실패 모드 enumerate + 문서화 만. |
| 7. **c_dforce_dxr_lumen_warm_skeleton** | 4 | 5 | 5 | 5 | **15** | **skeleton 만** — validated 불가. UE5 Lumen warmup 모델 (다른 UE5 게임 재사용) |
| 8. **c_dforce_nanite_meshlet_opt_skeleton** | 4 | 5 | 5 | 5 | **15** | **skeleton 만**. Apple Silicon mesh shader path (D4 mesh-cull 모듈 재사용) |
| 9. **c_dforce_unsupported_block_doc** | 0 | 5 | 0 | 0 | 5 | **방어용 모듈** — Delta Force detect 시 launch 차단 + 사용자 안내 (BAN 위험 회피) |

**현실적 권장**:
- candidate **1, 2, 3, 4, 5** 만 즉시 가치 있음 (install + login + CDN 까지).
- candidate **6** 은 attest 시뮬레이션 — 실 attest 우회 X, 실패 enumerate 만.
- candidate **7, 8** 은 skeleton 만점 누적용 — **validated 절대 불가** (정직성 표시 필수).
- candidate **9** = **권장 — gamebox identity 보호 module** (ACE 게임 자동 거부).

→ 추천 모듈 수: **9개** (단, 실 가치 있는 것은 5개).

---

## 5. 결론 — 우선순위

### 5-1. 만점 가능성 종합

| 항목 | 값 |
|---|---|
| **만점 가능성** | **불가능 (gameplay) / 부분가능 (install~login~CDN)** |
| 도달 가능 만점 점수 | +3 (Phase 2-3) + 보조 +3 (Phase 1) |
| validated 만점 가능성 | **0** (Phase 4 차단으로 실 frame 도달 불가) |
| skeleton 만점 가능성 | +5~7 (재사용성 모듈 위주) |
| BAN risk | **높음** — Tencent ID + HWID blacklist |
| gamebox identity 와의 정합성 | **불일치** — anti-cheat 우회 시도는 own 1 (Wine 0줄) 정신과 다른 방향 |

### 5-2. D2R / D4 와의 우선순위

**권장: D2R / D4 끝나기 전까지 Delta Force 진입 X. 후순위 (또는 영구 미진입)**.

이유:
1. **kernel ACE = closure path B 본질 차단**. D2R (anti-cheat 없음) / D4 (online DRM 만, kernel anti-cheat 없음) 와 차원이 다른 장벽.
2. ACE 우회 = TOS + BAN risk + own 1 identity 훼손.
3. install/login/CDN phase 만 진입해도 +3 만점 — D2R/D4 phase 2-3 만점 모듈 generic 화로 80% 흡수됨.
4. skeleton 모듈 일부 (UE5 pak / DXR Lumen / Nanite) 는 **다른 UE5 게임 (예: Stalker 2, Black Myth: Wukong) 에 재사용** — Delta Force 자체보다 generic UE5 phase 4 인프라로 의미 있음.

### 5-3. 병렬 가능성

병렬 진행은 **권장하지 않음**:
- Phase 1-3 만 의미 있고, 그 모듈들은 D2R/D4 phase 2-3 generic 화로 흡수 가능.
- Phase 4+ 진입 시점부터는 무한 차단 — 노력 대비 ROI = 0.

### 5-4. 미래 가능성 (조건부)

다음 조건 중 하나 이상 충족 시 재검토:
- **Team Jade 가 ACE user-mode variant 배포** (현재 일부 ACE 게임만 user-mode) — 가능성 낮음 (Tencent 정책상).
- **Steam Deck / Linux 공식 지원 발표** — Team Jade 가 2024-12 검토 발언만 함, 미실현.
- **D4 closure 후 generic UE5 + DXR + DLSS 인프라 완성** 시 — 그때까지 대기.

### 5-5. 즉시 권장 action

1. **candidate 9 (`c_dforce_unsupported_block_doc`) 만 우선 작성** — Delta Force 자동 거부 + 사용자에게 BAN 위험 고지. gamebox 사용자 보호 측면에서 가치 있음.
2. **D2R/D4 closure 집중**. Delta Force Phase 1-3 모듈은 D2R/D4 generic 화로 자연 흡수 대기.
3. 본 문서를 docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md "한계" 섹션에 cross-link.

---

## 6. 출처 (WebSearch 결과 인용)

- Delta Force (2025 video game) — Wikipedia: https://en.wikipedia.org/wiki/Delta_Force_(2025_video_game)
- Delta Force reboot launches via early access in Q4 2024 — Niche Gamer: https://nichegamer.com/delta-force-reboot-launches-q4-2024/
- Delta Force launches in Early Access for PC in Q4 2024 — Gematsu: https://www.gematsu.com/2024/08/delta-force-launches-in-early-access-for-pc-in-q4-2024
- Delta Force Steam page: https://store.steampowered.com/app/2507950/Delta_Force/
- Anti-Cheat Expert — Wikipedia: https://en.wikipedia.org/wiki/Anti-Cheat_Expert
- Anti-Cheat Expert — PCGamingWiki: https://www.pcgamingwiki.com/wiki/Anti-Cheat_Expert
- Delta Force Criticized for Including a Kernel-Level Anti-Cheat — 80.lv: https://80.lv/articles/delta-force-installs-kernel-level-spyware-anti-cheat-on-your-pc
- Delta Force: Hawk Ops Sparks Huge Controversy With Intrusive Kernel Level Anti-Cheat — BlackFlix: https://www.blackflix.com/delta-force-game-launches-on-steam-to-controversy-over-kernel-level-anti-cheat-software
- Delta Force System Requirements — gamesystemrequirements.com: https://gamesystemrequirements.com/game/delta-force-hawk-ops
- Delta Force: Hawk Ops PC system requirements — Dexerto: https://www.dexerto.com/gaming/delta-force-hawk-ops-pc-system-requirements-file-size-recommended-minimum-specs-2855418/
- Delta Force: Hawk Ops Demo PC performance — gamegpu: https://en.gamegpu.com/mmorpg-/-онлайн-игры/delta-force-hawk-ops-demo-test-gpu-cpu
- Delta Force: Black Hawk Down PC Performance Analysis — DSOGaming: https://www.dsogaming.com/pc-performance-analyses/delta-force-black-hawk-down-pc-performance-analysis/
- Delta Force optimization graphics settings — Epiccarry: https://epiccarry.com/blogs/delta-force-graphics-settings/
- Delta Force 2024 — AppleGamingWiki: https://www.applegamingwiki.com/wiki/Delta_Force_2024 (fetch 403, URL 만 인용)
- Will Delta Force run on Mac or Linux? — CodeWeavers CrossOver: https://www.codeweavers.com/compatibility/crossover/delta-force
- Delta Force developer mentions Steam Deck / Linux — GamingOnLinux: https://www.gamingonlinux.com/2024/12/delta-force-developer-mentions-they-will-look-at-supporting-steam-deck-linux/
- Anti-Cheat Expert (ACE) compatibility list — GamingOnLinux: https://www.gamingonlinux.com/anticheat/vendor/anti-cheat-expert-ace/
- Delta Force Black Hawk Down campaign solo — gamepressure: https://www.gamepressure.com/newsroom/how-to-play-delta-force-black-hawk-down-campaign-solo-single-play/z879ec
- Tough and grounded, Delta Force Black Hawk Down campaign — PCGamesN: https://www.pcgamesn.com/delta-force/single-player-campaign

---

*written 2026-05-01. Delta Force (2024 reboot) 만점 검토 — kernel-level ACE 로 인한 closure path B 본질 차단 결론. D2R/D4 우선, Delta Force 후순위 (또는 영구 미진입).*

*revised 2026-05-01-rev2 — §0.5 정정/추가. 2025-12 DMA Shield + 2026 GDC ACE AI Security Framework + iOS Hardening 반영 (모두 차단 강화). 사용자 directive: multiplayer 정의상 거부 → 영구 보류 확정. cycle 50 batch 6 candidates (Phase 1-3 + 보호) 만 진행, multiplayer 추적 신규 candidates 보류.*
