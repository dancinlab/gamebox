# PSB-6 Sketch — Apple Silicon native deep (cycle 12 직후)

> **status**: pre-emptive sketch. PSB-6 = cycle 12 (PSB-5 round 1 채택 3/3) 완료 후 진행.
> **상위**: [POST_SPRINT_5_BRAINSTORM_SKETCH.md](POST_SPRINT_5_BRAINSTORM_SKETCH.md), [CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md)
> **현재 만점**: **37** (D 35 + R 2). PSB-6 채택 시 +N 만점.

---

## 1. 채점 기준 (3축, 각 /5, 합 /15) — 동일

| axis | 의미 |
|---|---|
| A. Apple-only stack | hexa PE loader + macOS syscalls + Apple D3DMetal binary + Rosetta 2 + DIY Win32 shim. Wine 0. |
| B. Runtime | 게임 플레이 *중* 효과 (launch/install 만 X) |
| C. 데이터재해석 | raw 데이터를 cache/filter/forecast-ready 로 reproject |

---

## 2. PSB-6 신규 만점 후보 sketch (Apple Silicon native deep)

### round 1 (Apple Silicon native 영역 deep 탐색)

**C-NEON-SSE-XLAT-CACHE** (15/15 만점) ⭐
- 한 줄: x86 SSE/AVX → ARM NEON instruction translation cache (Win32 shim level)
- 시나리오: D2R/D4 inline asm/SSE intrinsic 함수 — 동일 SSE pattern 반복 → NEON 변환 결과 cache
- 무기: APBF (SSE pattern dedup) + columnar (NEON instruction histogram)
- A=5 (Apple NEON 직접, Rosetta 와 별개 — Win32 shim 안 inline asm), B=5, C=5
- 의존: win32_msvcrt + Win32 shim
- 작업: 4-5일

**C-UNIFIED-MEMORY-ZERO-COPY** (15/15 만점) ⭐
- 한 줄: macOS unified memory (CPU/GPU shared) → CPU-prepared buffer 직접 GPU 사용 (no copy)
- 시나리오: D4 vertex/index buffer upload 시 CPU staging buffer 없이 GPU 직접 read
- 무기: zero-copy detection + columnar (buffer access pattern)
- A=5 (Apple Silicon UMA architecture 직접), B=5, C=5
- 의존: dx_d3d12_stage2 + Metal heap
- 작업: 4-5일

**C-MESH-SHADER-CULL** (15/15 만점) ⭐
- 한 줄: Metal mesh shader (M3+) culling acceleration → DX12 mesh shader 매핑
- 시나리오: D4 high-poly enemy/boss mesh — mesh shader meshlet culling
- 무기: cluster (meshlet AABB) + columnar (visibility state)
- A=5 (Metal mesh shader, M3+ 전용), B=5, C=5
- 의존: dx_d3d12_stage2 (Mesh shader DXIL)
- 작업: 5-6일

### round 2 (Apple Neural Engine + macOS specific)

**C-CORE-ML-LOD-SELECT** (15/15 만점)
- 한 줄: CoreML on Apple Neural Engine → LOD 선택 per object (view+attention)
- 시나리오: D4 scene 안 수백 mesh — 어느 mesh 가 attention 받을지 ANE 추론 → LOD 차등화
- 무기: CoreML model + ANE inference + cluster (object attention)
- A=5 (Apple Neural Engine 직접), B=5, C=5
- 의존: 없음 (CoreML standalone)
- 작업: 6-7일

**C-ARGUMENT-BUFFER-TIER2-MAP** (15/15 만점)
- 한 줄: Metal Argument Buffer Tier 2 (M3+) ↔ DX12 descriptor heap 1:1 매핑 최적화
- 시나리오: D4 의 1000+ texture descriptor → tier 2 argument buffer 단일 binding
- 무기: descriptor delta (c_dxil_delta 와 유사) + APBF (descriptor dedup)
- A=5 (Metal Argument Buffer Tier 2, Apple-only), B=5, C=5
- 의존: dx_d3d12_stage2
- 작업: 4-5일

### round 3 (macOS infrastructure)

**C-DRIVER-KIT-USER-HOOK** (14/15 보조, A=5/B=5/C=4)
- 한 줄: DriverKit user-space driver hook → input device latency reduce
- A=5 (DriverKit Apple-only), B=5 (input every frame), C=4 (실제 데이터 reproject 부분)
- 작업: 5일

**C-CORE-IMAGE-POSTPROCESS-OFFLOAD** (14/15 보조, B=4)
- 한 줄: CoreImage 으로 D4 postprocess (bloom, SSAO) 일부 offload
- A=5, B=4 (postprocess only), C=5
- 작업: 4-5일

**C-AVFOUNDATION-CUTSCENE-DECODE** (13/15 보조, B=3)
- 한 줄: AVFoundation HEVC/AV1 hardware decode → cutscene 재생 GPU offload
- A=5, B=3 (cutscene only), C=5
- 작업: 3-4일

---

## 3. EXHAUSTION 추적 (업데이트)

### 누적 PSB 결과
| PSB | sketch 후보 | 채택 | 만점 추가 |
|---|---|---|---|
| PSB-1 | 4 | 4 | +4 |
| PSB-2 | 3 | 3 | +3 |
| PSB-3 | 3 | 3 | +3 |
| PSB-4 | 5 | 5 | +5 |
| PSB-5 round 1 | 3 | 3 | +3 |
| **PSB-6 (현재)** | **3 round1 / 5 만점 / 8 total** | **TBD** | **+3 (잠재)** |

### 종료 조건 진행
- 새 만점 0 × 3 sprint 연속 = EXHAUSTION
- 현재까지: PSB-1~5 모두 채택 → 0 streak = 0
- **PSB-6 round 1 잠재 +3** → 채택 시 만점 **40**, EXHAUSTION 미도달
- **PSB-6 round 2 잠재 +2** → 만점 **42** (cycle 14 가능)
- **PSB-7 예상 +1~2** (감소)
- **PSB-8 예상 +0** → **EXHAUSTION 인정** (cycle 16 부근)

### 누적 만점 시점별 (확정 + 예측)
- 시작 (BG cycle 0): 12
- PSB-1 (cycle 5-6): 20
- PSB-2 (cycle 8): 23
- PSB-3 (cycle 9): 26
- PSB-4 (cycle 10): 29 → cycle 11: 34
- **PSB-5 round 1 (cycle 12): 37** ← 직전
- **PSB-6 round 1 (cycle 13): 40** ← 본 sketch 즉시 진행
- PSB-6 round 2 (cycle 14): **42**
- PSB-7 (cycle 15): **43-44** (감소)
- PSB-8 (cycle 16): **44** (EXHAUSTION 선언)

---

## 4. 우선순위 배치 (S18 ~ S20, cycle 13 즉시 진행)

| Sprint | 후보 | 작업일 | 우선 | cycle |
|---|---|---|---|---|
| S18 | C-NEON-SSE-XLAT-CACHE | 4-5 | high (D2R/D4 SSE-heavy) | cycle 13 |
| S19 | C-UNIFIED-MEMORY-ZERO-COPY | 4-5 | high (Apple Silicon UMA 핵심) | cycle 13 |
| S20 | C-MESH-SHADER-CULL | 5-6 | high (M3+ 전용 영역) | cycle 13 |
| S21 | C-CORE-ML-LOD-SELECT | 6-7 | medium (ANE 처음 사용) | cycle 14 |
| S22 | C-ARGUMENT-BUFFER-TIER2-MAP | 4-5 | medium | cycle 14 |

cycle 13 작업일: ~14일 (병렬 시 ~5일).

---

## 5. closure 영향

- **closure ETA**: PSB-6 round 1 채택 시 +0.5 달
- **closure 진행률**: 70% (현재) → ~92% (PSB-6 sprint 후)
- **만점 컴포넌트**: 37 (현재) → 40-42 (PSB-6 후)

EXHAUSTION 도달 후 final closure 시점 만점: **~44** 추정.

---

## 6. 한계 + 정직성

이 sketch 의 모든 후보는:
- D4 / D2R 실제 측정 *전* 이론적 가정
- macOS / Metal API 변경 시 영역 변동
- M3+ 전용 (mesh shader, ABT2) 후보들은 hardware 의존

→ 본 sketch 는 *방향성* 만 보여줌. PSB-6 실제 진행 시 sketch 확정/수정.

---

## 7. 다음 단계

1. **closure path 계속** — Phase 9/10 D2R/D4 launch + sprint 측정 (CM-21~28)
2. PSB-6 round 1 (CN-30/31/32) **cycle 13 즉시 진행**
3. cycle 14 round 2 (CN-33/34) — ANE + ABT2 → 만점 42
4. cycle 15-16 PSB-7/8 → EXHAUSTION 도달

---

*written 2026-05-01 (BG cycle 12 종료 후). PSB-6 sketch — Apple Silicon native deep, 만점 5 후보 sketch.*
