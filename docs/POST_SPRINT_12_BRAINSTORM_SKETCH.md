# PSB-12 Sketch — Metal 깊은 영역 III (cycle 21 후)

> **현재 만점**: **62** (D 60 + R 2). PSB-12 채택 시 +N 만점.

---

## PSB-12 신규 만점 후보 sketch

### round 1 (cycle 22 즉시)

**C-RENDER-PASS-LOAD-STORE** (15/15) ⭐
- 한 줄: render pass load/store action 최적화 — clear/load 회피, don't-care store
- 시나리오: D4 GBuffer pass load=Clear store=Store → load=DontCare store=DontCare 가능 시
- A=5, B=5, C=5

**C-MTL-IMAGEBLOCK-SHARED** (15/15) ⭐
- 한 줄: Metal imageblock shared mem — TBDR-specific tile shared memory
- 시나리오: D4 deferred shading G-buffer 를 imageblock 으로 → 외부 RAM round-trip 제거
- A=5 (Metal imageblock TBDR-only), B=5, C=5

**C-METAL-SIMDGROUP-REDUCE** (15/15) ⭐
- 한 줄: SIMD-group level reduction (simd_sum/simd_min/simd_max)
- 시나리오: D4 compute reduction (HDR luminance avg, particle min) → SIMD-group 32-wide reduce
- A=5 (Apple GPU SIMD width 32), B=5, C=5

### round 2 (cycle 23 가능, 만점 추가 +3)

**C-METAL-VISIBILITY-RESULT** (15/15) — occlusion query optimization
**C-MTL-FUNCTION-CONST-VARIANT** (15/15) — function constants pipeline variant
**C-DXIL-VECTOR-SCALARIZE** (15/15) — vec → scalar for Apple GPU register alloc

### round 3 (cycle 24 가능, 만점 추가 +2~3)

**C-METAL-RES-OPT-SAMPLER** (15/15)
**C-METAL-CAPTURE-AUTO** (14/15, B=4)
**C-DRIVERKIT-INPUT** (14/15, C=4)

---

## EXHAUSTION 추적

| PSB | 채택 누적 | 만점 |
|---|---|---|
| PSB-1~11 | 37 | 62 |
| PSB-12 round 1 (예상) | 40 | 65 |
| PSB-12 round 2 (예상) | 43 | 68 |
| PSB-13+ | ? | ? (가능성 잔존) |

→ 진짜 EXHAUSTION 은 PSB-15+ 부근 추정 (만점 70+).

---

*written 2026-05-01 (BG cycle 21 후). PSB-12 sketch.*
