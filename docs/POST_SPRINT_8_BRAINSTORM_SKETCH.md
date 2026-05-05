# PSB-8 Sketch — Metal advanced (ICB/AMX/MPS, cycle 16 직후)

> **status**: pre-emptive sketch. PSB-8 = cycle 16 (PSB-7 채택 5/5) 완료 후 진행.
> **상위**: [POST_SPRINT_7_BRAINSTORM_SKETCH.md](POST_SPRINT_7_BRAINSTORM_SKETCH.md)
> **현재 만점**: **47** (D 45 + R 2). PSB-8 채택 시 +N 만점.

---

## 2. PSB-8 신규 만점 후보 sketch (Metal advanced)

### round 1 (ICB/AMX/MPS — Apple Silicon advanced)

**C-INDIRECT-CMD-BUFFER** (15/15 만점) ⭐
- 한 줄: Metal Indirect Command Buffer (M2+) — GPU-side cmd encoding, CPU 부담 제거
- 시나리오: D4 mass particle/instance — CPU encode 1만 draw 명령 → GPU 안 ICB 자체 생성
- 무기: ICB build pattern + columnar (cmd structure histogram)
- A=5, B=5, C=5

**C-AMX-MATRIX-MULTIPLY** (15/15 만점) ⭐
- 한 줄: Apple AMX matrix coprocessor — in-game AI inference (enemy AI) offload
- 시나리오: D4 boss AI 의 attention/decision matrix multiply → AMX
- 무기: AMX dispatch + columnar (matrix dim histogram)
- A=5 (Apple AMX 직접, undocumented but accessible), B=5, C=5

**C-MPS-GRAPH-POSTPROCESS** (15/15 만점) ⭐
- 한 줄: Metal Performance Shaders graph — bloom/SSAO/depth-of-field 일부 ML upscale
- 시나리오: D4 1080p render → MPS graph 4K upscale (FSR/DLSS 대안 macOS)
- 무기: MPS graph compose + columnar (scale factor)
- A=5 (MPS Apple-only), B=5, C=5

### round 2 (감소 추세 — 영역 거의 소진)

**C-FRAGMENT-INTERLOCK-BLEND** (14/15 보조, B=4)
- 한 줄: Metal fragment interlock — custom OIT (order-independent transparency)
- A=5, B=4 (transparency only), C=5

**C-METAL-SDF-RENDER** (13/15 보조, B=3)
- 한 줄: Metal SDF API — vector UI/text rendering
- A=5, B=3 (UI only), C=5

---

## 3. EXHAUSTION 추적 (업데이트)

### 누적 PSB 결과
| PSB | sketch 후보 | 채택 | 만점 추가 |
|---|---|---|---|
| PSB-1 ~ 7 | 누적 25 | 25 | +25 |
| **PSB-8 (현재)** | **3 round1 / 5 total** | **TBD** | **+3 (잠재)** |

### 종료 조건 진행
- 새 만점 0 × 3 sprint 연속 = EXHAUSTION
- **PSB-8 round 1 잠재 +3** → 만점 **50**
- **PSB-9 예상 +1~2** (감소)
- **PSB-10 예상 +0~1** (포화)
- **PSB-11 예상 +0** → **EXHAUSTION 인정** (cycle 19 부근)

### 누적 만점 시점별 (확정 + 예측)
- cycle 16: 47 ← 현재 ✅
- **cycle 17 (PSB-8 round 1): 50**
- cycle 18 (PSB-9): **51-52**
- cycle 19 (PSB-10): **52** (포화)
- cycle 20 (PSB-11): **52** (EXHAUSTION 선언)

---

## 4. 우선순위 배치 (S28 ~ S30, cycle 17 즉시 진행)

| Sprint | 후보 | 작업일 | 우선 | cycle |
|---|---|---|---|---|
| S28 | C-INDIRECT-CMD-BUFFER | 4-5 | high (massive draws) | cycle 17 |
| S29 | C-AMX-MATRIX-MULTIPLY | 5-6 | high (AI offload) | cycle 17 |
| S30 | C-MPS-GRAPH-POSTPROCESS | 5-6 | high (FSR/DLSS 대안) | cycle 17 |

cycle 17 작업일: ~14일 (병렬 시 ~5일).

---

## 5. closure 영향

- **만점 컴포넌트**: 47 (현재) → 50 (PSB-8 후) → 52 (final)
- **closure 진행률**: 70% (현재) → ~96% (PSB-8 후)

EXHAUSTION 도달 후 final closure 시점 만점: **~52** 추정 (이전 49 추정에서 상향).

---

## 6. 한계 + 정직성

- AMX 는 undocumented — Apple 의 미래 변경 가능성
- MPS graph 는 macOS 13+ 의존
- ICB 는 M2+ 의존 (M1 fallback 필요)

---

## 7. 다음 단계

1. **closure path 계속** — Phase 9/10 D2R/D4 launch + sprint 측정
2. PSB-8 round 1 (CN-40/41/42) **cycle 17 즉시 진행**
3. cycle 18-20 PSB-9/10/11 → EXHAUSTION 도달

---

*written 2026-05-01 (BG cycle 16 종료 후). PSB-8 sketch — Metal advanced.*
