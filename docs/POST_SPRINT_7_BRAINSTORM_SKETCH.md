# PSB-7 Sketch — Metal/macOS deep III (cycle 14 직후)

> **status**: pre-emptive sketch. PSB-7 = cycle 14 (PSB-6 round 2 채택 2/2) 완료 후 진행.
> **상위**: [POST_SPRINT_6_BRAINSTORM_SKETCH.md](POST_SPRINT_6_BRAINSTORM_SKETCH.md)
> **현재 만점**: **42** (D 40 + R 2). PSB-7 채택 시 +N 만점.

---

## 1. 채점 기준 (3축, 각 /5, 합 /15) — 동일

---

## 2. PSB-7 신규 만점 후보 sketch (Metal/macOS deep III)

### round 1 (Metal TBDR/VRS/aliasing — 깊은 영역)

**C-TILE-MEMORY-SHADER** (15/15 만점) ⭐
- 한 줄: Metal tile shader (TBDR) on-chip memory access pattern → DX12 render pass remap
- 시나리오: D4 deferred lighting GBuffer pass — TBDR tile memory 안 G/D/L attachment fusion
- 무기: tile usage histogram + columnar (attachment access pattern)
- A=5 (Apple Silicon TBDR Metal-only), B=5 (매 frame), C=5
- 의존: dx_d3d12_stage2 + Metal render pass
- 작업: 5-6일

**C-VRS-RATE-ADAPTIVE** (15/15 만점) ⭐
- 한 줄: Metal Variable Rate Shading — region attention 기반 shading rate adaptive
- 시나리오: D4 시야 중앙 1×1, 주변 2×2, 외곽 4×4 → 셰이더 비용 ↓
- 무기: c_core_ml_lod_select (CN-33) attention map 재사용 + region cluster
- A=5 (Metal VRS, M2+), B=5, C=5
- 의존: dx_d3d12_stage2 (VRS extension) + CN-33
- 작업: 4-5일

**C-HEAP-ALIAS-SCHED** (15/15 만점) ⭐
- 한 줄: Metal heap aliasing — frame 안 mutually exclusive resource → memory alias
- 시나리오: D4 G-buffer A 와 SSR temp B 같은 frame 안 동시에 안 씀 → heap 같은 영역 alias
- 무기: temporal exclusivity graph + Metal heap alias placement
- A=5, B=5, C=5
- 의존: dx_d3d12_stage2 + Metal heap
- 작업: 5-6일

### round 2 (Apple-specific advanced)

**C-GPU-FAMILY-SHADER-VARIANT** (15/15 만점)
- 한 줄: M1/M2/M3/M4 별 best shader variant 선택 (apple gpu family hint)
- 시나리오: D4 같은 effect → M1 variant A / M3 variant B (mesh shader 활용 등)
- 무기: GPU family detection + columnar (variant performance histogram)
- A=5, B=5, C=5
- 의존: 없음
- 작업: 4일

**C-CA-DISPLAY-LINK-VSYNC** (15/15 만점)
- 한 줄: macOS CADisplayLink vsync timing → DX present 매핑
- 시나리오: D4 swapchain present 시점 CADisplayLink 정밀 timing 기반 frame pacing
- 무기: CADisplayLink callback + Holt forecast (vsync interval)
- A=5 (macOS CADisplayLink), B=5, C=5
- 의존: c_frame_pacing_adaptive (CN-22) 와 보완
- 작업: 3-4일

### round 3 (감소 추세 — 보조)

**C-SYSCTL-MEM-PRESSURE-PROACTIVE** (14/15 보조, A=4)
- 한 줄: macOS sysctl 으로 system memory pressure 미리 감지
- A=4 (sysctl), B=5, C=5
- 작업: 2-3일

**C-CG-CONTEXT-HUD-OFFLOAD** (14/15 보조, B=4)
- 한 줄: HUD CGContext 으로 그려서 Metal zero-copy
- A=5, B=4 (HUD only), C=5
- 작업: 3일

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
| PSB-6 round 1+2 | 5 | 5 | +5 |
| **PSB-7 (현재)** | **3 round1 / 5 만점 / 7 total** | **TBD** | **+3 (잠재)** |

### 종료 조건 진행
- 현재까지: PSB-1~6 모두 채택 → 0 streak = 0
- **PSB-7 round 1 잠재 +3** → 채택 시 만점 **45**
- **PSB-7 round 2 잠재 +2** → 만점 **47** (cycle 16)
- **PSB-8 예상 +1~2** (감소)
- **PSB-9 예상 +0~1** (포화)
- **PSB-10 예상 +0** → **EXHAUSTION 인정** (cycle 18 부근)

### 누적 만점 시점별 (확정 + 예측)
- cycle 12: 37 → cycle 13: 40 → cycle 14: 42 ← 현재 ✅
- **cycle 15 (PSB-7 round 1): 45**
- cycle 16 (PSB-7 round 2): **47**
- cycle 17 (PSB-8): **48-49**
- cycle 18 (PSB-9): **49**
- cycle 19 (PSB-10): **49** (EXHAUSTION 선언)

---

## 4. 우선순위 배치 (S23 ~ S25, cycle 15 즉시 진행)

| Sprint | 후보 | 작업일 | 우선 | cycle |
|---|---|---|---|---|
| S23 | C-TILE-MEMORY-SHADER | 5-6 | high (TBDR 핵심) | cycle 15 |
| S24 | C-VRS-RATE-ADAPTIVE | 4-5 | high (CN-33 시너지) | cycle 15 |
| S25 | C-HEAP-ALIAS-SCHED | 5-6 | high (memory ↓) | cycle 15 |
| S26 | C-GPU-FAMILY-SHADER-VARIANT | 4 | medium | cycle 16 |
| S27 | C-CA-DISPLAY-LINK-VSYNC | 3-4 | medium | cycle 16 |

cycle 15 작업일: ~15일 (병렬 시 ~5일).

---

## 5. closure 영향

- **closure 진행률**: 70% (현재) → ~95% (PSB-7 sprint 후)
- **만점 컴포넌트**: 42 (현재) → 47 (PSB-7 후)

EXHAUSTION 도달 후 final closure 시점 만점: **~49** 추정.

---

## 6. 한계 + 정직성

- M2+/M3+ 전용 features (VRS, mesh shader, ABT2) — older Apple Silicon hardware fallback 필요
- TBDR 활용은 Apple GPU 전용 — Intel Mac/discrete GPU 변환 시 차이
- closure 의 game launch 부분은 sketch 외 영역

---

## 7. 다음 단계

1. **closure path 계속** — Phase 9/10 D2R/D4 launch + sprint 측정
2. PSB-7 round 1 (CN-35/36/37) **cycle 15 즉시 진행**
3. cycle 16 round 2 → 만점 47
4. cycle 17-19 PSB-8/9/10 → EXHAUSTION 도달

---

*written 2026-05-01 (BG cycle 14 종료 후). PSB-7 sketch — Metal/macOS deep III.*
