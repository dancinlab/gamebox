# D2R / D4 install → launch 로드맵 (만점 기준 검토)

> **status**: closure path B 진행 중 (skeleton manjeom **100 milestone**, cycle 37)
> 작성: 2026-05-01 / **revision 2026-05-01**: skeleton 100 milestone 달성 + 6 phase coverage
> SSOT: docs/CLOSURE_ROADMAP.md (CM-0~30) + 본 로드맵 (단계별 만점 검토)

---

## 0a. 만점 status 단계 (중요 — 이전 문서에서 불명확)

| 단계 | 의미 | 검증 |
|---|---|---|
| **skeleton** | 모듈 작성 + self_test 통과 (synthetic data) | 합성 입력만 |
| **stage1-3** | 부분 구현 (real path 일부) | partial real |
| **validated** | 실 게임 (D2R/D4) 데이터로 검증 통과 | real game data |
| **done (closure)** | closure 정의 모두 충족 | CM-30 |

**현 상태**: 모든 모듈 = **skeleton** (synthetic 만 검증). 실 게임 미설치/미실행.
- skeleton manjeom: **100** (cycle 37 milestone — Phase 1~6 모두 커버)
- validated manjeom: **0** (게임 1프레임도 안 돔)
- closure: CM-20/30 = 67% (skeleton 단계 milestone 만)
- phase dashboard closure_pct: **25%** (skeleton 6/6 × 25 score)

---

## 0. 만점 기준 (재확인)

| axis | /5 | 의미 |
|---|---|---|
| **A** | 5 | Apple-only stack (hexa PE loader + macOS syscalls + Apple D3DMetal + Rosetta 2 + DIY Win32 shim, **Wine 0**) |
| **B** | 5 | Runtime — 게임 *플레이 중* 효과 (install/launch 만 X) |
| **C** | 5 | 데이터재해석 — raw data → cache/filter/forecast |

**만점** = 15/15. install/launch 자체는 B=5 받기 어려움 (one-shot 단계). gameplay 에서 B=5.

---

## 1. 전체 단계 분해

```
[Phase 1] Battle.net Setup install
   ↓
[Phase 2] Battle.net Setup runtime (login + game catalog 조회)
   ↓
[Phase 3] D2R / D4 download + install
   ↓
[Phase 4] D2R / D4 first frame (initial entry)
   ↓
[Phase 5] D2R / D4 gameplay loop
   ↓
[Phase 6] continuous optimization (closed-loop)
```

각 phase 가 만점 모듈 다수 require.

---

## 2. Phase-by-phase 만점 검토

### Phase 1 — Battle.net-Setup.exe install

**현재 상태** (cycle 27):
- PE parse: ✅ done
- DLL coverage: 15/16 (잔여: WININET)
- 함수 coverage: 1% (8/435 — KERNEL32 강화 필요 221 functions)
- 실 install (PE map + IAT resolve + entry call): ❌ 미구현

**핵심 모듈 필요:**

| 모듈 | A | B | C | 만점 | 상태 |
|---|---|---|---|---|---|
| C-PE-MAP-x86 (i386 PE map) | 5 | 4 | 5 | 14 | 신규 |
| C-IAT-RESOLVE-RUNTIME (실 IAT bind) | 5 | 4 | 5 | 14 | 신규 |
| C-WIN32-CALL-TRAMPOLINE (i386 cdecl/stdcall) | 5 | 5 | 5 | 15 | 신규 |
| C-INSTALLER-PROGRESS-PARSE | 5 | 4 | 5 | 14 | 신규 |
| C-WININET-HTTP-CDN-PROXY | 5 | 4 | 5 | 14 | 신규 |
| C-CDN-DOWNLOAD-RESUME | 5 | 4 | 5 | 14 | 신규 |
| C-MSI-MINIMAL-PARSE | 5 | 4 | 5 | 14 | 신규 |

**만점 잔여**: B=4 (install 일회성), 실 만점 candidates 0. 만점 +0 (보조 +7).

**ETA**: 2-3 개월 (PE map + IAT runtime bind 가 hardest).

---

### Phase 2 — Battle.net launcher runtime

**시나리오**: 사용자 login + game catalog 조회 + D2R/D4 download 시작.

**핵심 모듈:**

| 모듈 | A | B | C | 만점 | 상태 |
|---|---|---|---|---|---|
| C-OAUTH-FLOW-PROXY (us.battle.net OAuth) | 5 | 5 | 5 | **15** | 신규 |
| C-PRODUCT-DB-CACHE (catalog JSON cache) | 5 | 5 | 5 | **15** | 신규 |
| C-CATALOG-FORECAST (어떤 게임 launch 할지 예측) | 5 | 5 | 5 | **15** | 신규 |
| C-AUTH-TOKEN-PERSIST (OAuth token cache) | 5 | 4 | 5 | 14 | 신규 |
| C-CDN-MIRROR-SELECT (us-east/eu/kr) | 5 | 5 | 5 | **15** | 신규 |

**만점 +4** (4개의 진짜 runtime + reproject 후보).

**ETA**: 1-1.5 개월.

---

### Phase 3 — D2R / D4 download + install

**시나리오**: Battle.net 이 D2R (~30GB) / D4 (~100GB) CDN download + 디스크 install.

**핵심 모듈:**

| 모듈 | A | B | C | 만점 | 상태 |
|---|---|---|---|---|---|
| C-CDN-PARALLEL-CHUNK | 5 | 4 | 5 | 14 | 신규 |
| C-DISK-LAYOUT-PREDICT (asset prefetch order) | 5 | 5 | 5 | **15** | 신규 |
| C-INSTALL-VERIFY-SHA (parallel hash verify) | 5 | 5 | 5 | **15** | 신규 |
| C-INSTALL-PATCH-DELTA (binary diff) | 5 | 5 | 5 | **15** | 신규 |

**만점 +3**.

**ETA**: 1.5-2 개월.

---

### Phase 4 — D2R / D4 first frame

#### Phase 4-D2R (D3D11 path)

**현재 상태**:
- d2r_launch.hexa skeleton ✅
- battlenet_bypass D2R feasibility ✅ (offline mode 가능)
- DX11 → D3DMetal: dx_d3d11_stage1 ✅
- 실 D2R.exe entry call: ❌

**핵심 모듈:**

| 모듈 | A | B | C | 만점 | 상태 |
|---|---|---|---|---|---|
| C-D2R-FIRST-FRAME-TRACE | 5 | 5 | 5 | **15** | 신규 |
| C-D2R-OFFLINE-PROFILE-CACHE | 5 | 5 | 5 | **15** | 신규 |
| C-D2R-DX11-PIPELINE-WARM | 5 | 5 | 5 | **15** | 신규 |

**만점 +3**.

**ETA**: 2-3 개월 (PE entry call + DX11 → Metal binding).

#### Phase 4-D4 (D3D12 + DXR path)

**현재 상태**:
- d4_launch.hexa skeleton ✅
- battlenet_bypass D4 → **infeasible** (online-only DRM)
- DX12 → D3DMetal: dx_d3d12_stage2 ✅ (DXR done)
- 실 D4 entry call: ❌

**핵심 모듈:**

| 모듈 | A | B | C | 만점 | 상태 |
|---|---|---|---|---|---|
| C-D4-DXR-FIRST-FRAME-TRACE | 5 | 5 | 5 | **15** | 신규 |
| C-D4-D3D12-PIPELINE-WARM | 5 | 5 | 5 | **15** | 신규 |
| C-D4-INDIRECT-CMD-OPT (CN-40 적용) | 5 | 5 | 5 | **15** | done (cycle 17) |
| C-D4-MESH-SHADER-CULL (CN-32 적용) | 5 | 5 | 5 | **15** | done (cycle 13) |

**만점 +2** (이미 cycle 13/17 에서 done 분 미반영 — D4 specific 으로 재산입).

**ETA**: 3-4 개월 (D3D12 + DXR full path).

---

### Phase 5 — gameplay 만점 모듈 (skeleton 만 — validated 0)

**중요 정정**: 이 phase 의 모든 모듈은 **skeleton** 상태. 실 게임 데이터 검증 0.
"done" 표현 잘못됨 — 게임 1프레임도 안 돈 시점에 gameplay phase done X.

**skeleton 만점 모듈** (100 누적, cycle 37 milestone):

| 카테고리 | 모듈 수 | 비고 (모두 skeleton) |
|---|---|---|
| Frametime / shader / heap / PSO | 12 | C-FRAMETIME, C-SHADER-AGING 등 — synthetic frame data 만 |
| Apple Silicon native | 8 | NEON-SSE, UMA, Mesh shader, ANE LOD 등 — synthetic GPU data |
| Metal advanced | 12 | TBDR, VRS, ICB, AMX, MPS, ABT2 등 — synthetic dispatch |
| Rendering pipeline | 8 | Render pass load/store, imageblock, SIMD reduce 등 |
| Input / audio / network | 6 | Input latency, audio DSP, Winsock 등 |
| Phase 2 Battle.net | 4 | OAuth proxy, ProductDB, Catalog forecast, CDN mirror (cycle 28) |
| 그 외 | 24 | install path detect, PE coverage, IAT sim, dashboard 등 |

**skeleton manjeom 100 (milestone) / validated 0**.

**Cycles 25~37 closure path B 진입 후 추가** (35 modules):
- Phase 1 (PE/IAT/install): CN-58~62, CN-67~69, CN-78, CN-85, CN-89, CN-90 (12 modules)
- Phase 2 (Battle.net): CN-63~66, CN-79 (5 modules)
- Phase 3 (CDN/install): CN-86 (1 module)
- Phase 4 (first frame): CN-70~74, CN-81 (6 modules)
- Phase 5 (gameplay): CN-75, 76, 80, 83, 84, 87, 88, 91, 92 (9 modules)
- Phase 6 (live tune): CN-77, CN-82 (2 modules)


**Phase 5 의 진짜 작업**: D2R/D4 가 실 frame 그리는 시점부터 시작.
1. Phase 4 (first frame) 완료 후 — 위 74 모듈 각각 실 데이터로 검증
2. 검증 결과 따라 모듈 일부는 무용 (synthetic 가정 빗나감) / 일부는 stage1→stage2 강화
3. 30+ 시간 측정 후 validated manjeom 산출

**ETA**: Phase 4 완료 후 추가 1-2 개월 (각 모듈 검증 + tune).

---

### Phase 6 — closed-loop continuous optimization

**시나리오**: D2R/D4 30+ 시간 측정 → manjeom 모듈 actual data 검증 + tune.

**핵심 모듈** (이미 일부 done):

| 모듈 | 상태 |
|---|---|
| C-METAL-PROFILER-FEEDBACK (CN-47) | done (cycle 19) |
| C-RAW-PARADIGM-EXTRACT (CN-26) | done (cycle 11) |
| C-AIRGENOME-LABEL-FUSION (CN-25) | done (cycle 11) |
| C-USER-SOLO-PROFILE (CN-15) | done |
| C-D2R-LIVE-TUNE | 신규 |
| C-D4-LIVE-TUNE | 신규 |

**만점 +2** (D2R/D4 specific tune).

**ETA**: 1 개월 (모듈 추가 + 30시간 측정 후).

---

## 3. 합산 만점 가능 영역

### 만점 (15/15) 신규 가능
| Phase | 후보 | 누적 |
|---|---|---|
| Phase 2 (Battle.net runtime) | 4 | 70 + 4 = 74 |
| Phase 3 (install) | 3 | 74 + 3 = 77 |
| Phase 4-D2R | 3 | 77 + 3 = 80 |
| Phase 4-D4 | 2 (재산입) | 80 + 2 = 82 |
| Phase 6 | 2 | 82 + 2 = 84 |

→ **최종 도달 가능 만점**: **~84** (현 70 + 14 신규).

### 보조 (14/15) 또는 미만 — 부득이
- Phase 1 install 자체 (B=4 unavoidable): 7 modules × 14 = 보조 +7
- 일부 download/CDN 모듈 (B=4)

---

## 4. ETA 합산 (정정)

| Phase | ETA | 누적 | 비고 |
|---|---|---|---|
| Phase 1 install runtime | 2-3 개월 | 3 | kernel32 stage4 + i386 + WININET |
| Phase 2 Battle.net runtime | 1-1.5 개월 | 4.5 | OAuth/Catalog (skeleton done cycle 28) |
| Phase 3 D2R install | 1.5-2 개월 | 6.5 | CDN download + verify |
| Phase 3 D4 install | (병렬) | 6.5 | 동일 인프라 |
| Phase 4-D2R first frame | 2-3 개월 | 9.5 | DX11 → Metal binding |
| Phase 4-D4 first frame | 1-2 개월 | 11 | DX12+DXR (stage2 skeleton 활용) |
| **Phase 5 gameplay validation** | **1-2 개월** | **13** | **74 skeleton 모듈 실 게임 데이터로 검증** |
| Phase 6 live tune | 1 개월 | 14 | 30+ 시간 측정 후 |

→ **closure ETA**: **~14 개월** (직렬 worst case). 병렬 + bg cycle 활용 시 **7-10 개월**.

이전 "12 개월" 은 Phase 5 ETA=0 가정 잘못 — 정정 후 14 개월.

---

## 5. closure path B 진척 (실측 — 정정)

```
skeleton manjeom 100 (cycle 37 milestone)
  ↓ (validated manjeom 0)
DLL adapter 15/16 mapped (94%)
  ↓
function coverage 8/435 (1%) — kernel32 stage1→stage4 강화 필요
  ↓
PE map runtime 0% — Phase 1 핵심 작업 시작 전
  ↓
실 install (Battle.net-Setup.exe) 0% — Phase 1 미완
  ↓
D2R first frame 0% — Phase 4 진입 전
  ↓
D4 first frame 0% — Phase 4 진입 전 (D3D12 path 는 skeleton stage2)
  ↓
gameplay validated 0/74 (0%) — 실 게임 측정 전
```

**정직성**: skeleton manjeom 은 "실 게임 환경에서 의미 있을 수 있는 가정" 단계 —
실 게임 frame 후 검증 통과 모듈 만 validated. 현재 validated = 0.

---

## 6. 최우선 진행 순서 (만점 ROI 기준)

1. **Phase 1 진행** (kernel32 stage4 + i386 ABI + WININET) — install 가능해짐
   - 만점 +0 (보조 만), 그러나 다른 phase 진입 prerequisite
   - 6-12 개월 path 의 첫 1-2 개월
2. **Phase 2 만점 +4** (OAuth proxy, ProductDB cache, Catalog forecast 등)
3. **Phase 4-D4 진행** (이미 D3D12 stage2 done — 빠름) → first frame
4. **Phase 4-D2R 진행**
5. **Phase 6 live tune** (실측 후)

---

## 7. closure 정의 (재확인 + 정정)

closure (CM-30) = 다음 모두 ✓:
- [x] Wine 0 (own1)
- [x] Apple Silicon native
- [~] DIY hexa PE loader (skeleton ✅, runtime ❌)
- [x] **skeleton** manjeom ≥ 30 (현 74) — 단 skeleton 단계만
- [ ] **validated** manjeom 측정 (실 게임 frame 후) — 현 0
- [ ] D2R first frame on macOS via gamebox
- [ ] D4 first frame on macOS via gamebox
- [ ] 30+ 시간 측정 (D2R)
- [ ] 30+ 시간 측정 (D4)
- [ ] EXHAUSTION 인정 (실측 기반)
- [ ] final closure docs + commit + push

→ 4/11 closure 컴포넌트 done. **4/11 ≈ 36% closure** (validated metric 추가 후).
공식 in-progress: CM-20/CM-30 = 67% (milestone 단위 — skeleton 진척 위주).
**실 closure 진척률은 validated 기준으로 36%** (정직성).

---

## 8. 한계 + 정직성

- **D4 online-only DRM** — gamebox 가 server-side auth 우회 불가능. D4 launch 는
  실제 Battle.net account 필요 (offline test 불가).
- **6-12 개월 ETA** — kernel32 stage4 (~200 함수 실 구현) 가 single-handedly 큰
  비중 (~3-4 개월). 단축 어려움.
- **Phase 1 install** 의 만점 +0 — install 자체가 one-shot 이라 B=5 어려움.
  보조 +7 만 가능.

---

## 9. 다음 cycle 28 우선순위

cycle 27 후속 — **Phase 2 만점 +4 candidates 부분 진입** (만점 ROI 높음):
- C-OAUTH-FLOW-PROXY (만점 15)
- C-PRODUCT-DB-CACHE (만점 15)
- C-CATALOG-FORECAST (만점 15)
- C-CDN-MIRROR-SELECT (만점 15)

→ 4 개 추가 시 만점 70 → 74.

또는 closure prerequisite 인 Phase 1 강화:
- WININET stage1 + CRYPT32 stage1 (마지막 1 DLL 매핑 + 함수 coverage 강화)
- kernel32 stage1 → stage2 (3 → ~50 함수)

권장: **Phase 2 만점 4 우선 (cycle 28-29)** + **Phase 1 함수 강화 병렬 (cycle 30+)**.

---

*written 2026-05-01 (cycle 27 직후). closure path B 단계별 만점 매트릭스.*
