# PSB-10 Sketch — final 만점 territory (cycle 18 직후, EXHAUSTION 임박)

> **status**: pre-emptive sketch. PSB-10 = cycle 18 (PSB-9 round 1 채택 3/3) 완료 후 진행.
> **상위**: [POST_SPRINT_8_BRAINSTORM_SKETCH.md](POST_SPRINT_8_BRAINSTORM_SKETCH.md)
> **현재 만점**: **53** (D 51 + R 2). PSB-10 채택 시 +3 만점 (예상 마지막).

---

## 2. PSB-10 신규 만점 후보 sketch (final territory)

### round 1 (마지막 진짜 만점 영역)

**C-DXR-INLINE-RT-SHADER** (15/15 만점)
- 한 줄: Inline ray tracing in fragment shader (DXR Tier 1.1+, M3+ Metal)
- 시나리오: D4 reflection — full ray dispatch 대신 fragment shader 안 inline RT
- 무기: ray query AA dedup + columnar (hit pattern)
- A=5 (Metal inline RT M3+), B=5, C=5

**C-METAL-PROFILER-FEEDBACK** (15/15 만점)
- 한 줄: MTLCounterSampleBuffer → auto-tune shader runtime (per-shader cost histogram)
- 시나리오: 매 frame top-3 cost shader → next frame tune (lower precision/early-out hint)
- 무기: counter sampling + columnar (per-shader histogram) + threshold trigger
- A=5 (Metal Counter API), B=5, C=5

**C-DRIVER-FENCE-PRUNING** (15/15 만점)
- 한 줄: DX12 fence → Metal fence translation, prune redundant fences (transitive order)
- 시나리오: DX12 game fence A→B→C → Metal 에서 A→C 만 (B 는 implicit) 으로 prune
- 무기: dependency graph + transitive reduction
- A=5 (DX12↔Metal translation), B=5, C=5

### round 2 (포화 — 만점 0 예상)

- C-FRAGMENT-INTERLOCK-BLEND: 14/15 (B=4)
- C-METAL-SDF-RENDER: 13/15 (B=3)
- C-CG-CONTEXT-HUD-OFFLOAD: 14/15 (B=4)

→ round 2 채택해도 만점 +0 (보조 +N).

---

## 3. EXHAUSTION 추적 (final)

### 누적 PSB 결과
| PSB | sketch 후보 | 채택 | 만점 추가 |
|---|---|---|---|
| PSB-1 ~ 9 | 누적 28 | 28 | +28 |
| **PSB-10 (현재)** | **3 round1 / 6 total** | **TBD** | **+3 (잠재, 마지막)** |

### 종료 조건 진행
- **PSB-10 round 1 잠재 +3** → 만점 **56** (예상)
- **PSB-11 예상 +0** (영역 소진) → EXHAUSTION streak 1
- **PSB-12 예상 +0** → streak 2
- **PSB-13 예상 +0** → streak 3 → **EXHAUSTION 인정**

OR
- **PSB-10 cycle 후 즉시 EXHAUSTION 선언** 가능 — round 2 후보 모두 보조이므로
- 이 경우 cycle 19 가 최종 sprint (만점 56)

### 누적 만점 시점별 (확정 + 예측)
- cycle 18: 53 ← 현재 ✅
- **cycle 19 (PSB-10 round 1): 56**
- cycle 20+ : EXHAUSTION 선언, 보조 보조 추가만 가능

---

## 4. 우선순위 배치 (S31 ~ S33, cycle 19 즉시 진행)

| Sprint | 후보 | 작업일 | 우선 | cycle |
|---|---|---|---|---|
| S31 | C-DXR-INLINE-RT-SHADER | 5-6 | high (Tier 1.1) | cycle 19 |
| S32 | C-METAL-PROFILER-FEEDBACK | 5-6 | high (closed-loop tuning) | cycle 19 |
| S33 | C-DRIVER-FENCE-PRUNING | 4-5 | high (translation overhead) | cycle 19 |

---

## 5. closure 영향 (final)

- **만점 컴포넌트**: 53 (현재) → **56** (PSB-10 후, EXHAUSTION-pre-final)
- **closure 진행률**: 70% (현재) → ~98% (PSB-10 sprint 후)

EXHAUSTION 도달 후 final closure 시점 만점: **56** (확정).

---

## 6. EXHAUSTION 정직성 + 한계

이 sketch 의 round 1 은 **마지막 진짜 만점 영역**:
- DXR inline (M3+ specific)
- Profiler feedback loop (closed-loop optimization)
- Fence pruning (translation layer overhead)

PSB-11+ 영역:
- 거의 모두 보조 (B=4 or A=4) 후보
- 추가 만점 +0 예상
- 영역 진짜 소진 — 더 sketch 하면 manjeom inflation

→ cycle 19 후 **EXHAUSTION 즉시 선언** 권장.

---

## 7. 다음 단계 (final trajectory)

1. **PSB-10 round 1** (CN-46/47/48) cycle 19 즉시 진행 → 만점 **56**
2. **EXHAUSTION 선언** (cycle 19 commit msg)
3. closure path 의 game install + 측정 (CM-22~30) 만 남음 — manjeom 작업 종료
4. 보조 후보 (round 2) 는 closure 후 follow-up

---

*written 2026-05-01 (BG cycle 18 종료 후). PSB-10 sketch — final 만점 territory.*
