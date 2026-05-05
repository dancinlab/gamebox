# PSB-11 Sketch — re-opened territory (cycle 19 EXHAUSTION 재검토)

> **status**: EXHAUSTION 재검토. PSB-11 = cycle 19 의 EXHAUSTION 선언이 *premature* 했음을 인정하고 깊이 재탐색.
> **상위**: [POST_SPRINT_10_BRAINSTORM_SKETCH.md](POST_SPRINT_10_BRAINSTORM_SKETCH.md)
> **현재 만점**: **56** (D 54 + R 2). PSB-11 채택 시 +N 만점.

---

## 1. EXHAUSTION 재검토 — 놓친 영역

cycle 19 의 EXHAUSTION 선언은 PSB-10 round 2 (3 보조) 만 보고 결정.
실제 더 깊은 영역 재탐색:

### 누락 만점 영역
1. **Acceleration Structure compaction** — Metal AS 빌드 후 free space recover (prefix sum)
2. **Metal Function Pointers** — GPU-side vtable indirect dispatch (M2+, ICB 와 별개)
3. **DXIL → AIR translation fusion** — adjacent ops fuse (mul+add → fma)
4. **Private heap prewarm** — pre-warm private heap pages
5. **NSProcessInfo low-power awareness** — battery/thermal mode adaptive
6. **os_log runtime streaming** — macOS-only structured trace

→ EXHAUSTION 도달 부정 — 영역 더 남음.

---

## 2. PSB-11 신규 만점 후보 sketch

### round 1 (cycle 20 즉시 진행)

**C-RT-AS-COMPACTION** (15/15 만점) ⭐
- 한 줄: Metal Acceleration Structure compaction — post-build free-space recover
- 시나리오: D4 RT scene rebuild 후 AS 30%+ 여유 공간 → compact
- 무기: prefix sum + Metal AS compaction API
- A=5 (Metal AS), B=5, C=5

**C-MTL-FUNCTION-POINTER-VTABLE** (15/15 만점) ⭐
- 한 줄: Metal function pointer (M2+) — GPU vtable indirect dispatch
- 시나리오: D4 spell/effect dispatch — material vtable 안 함수 indirect call
- 무기: vtable resolve + columnar (function call frequency)
- A=5 (Metal function pointers, M2+), B=5, C=5

**C-DXIL-TO-AIR-FUSION** (15/15 만점) ⭐
- 한 줄: DXIL → AIR translation, adjacent op fusion (mul+add → fma)
- 시나리오: D4 shader DXIL bytecode 안 mul+add 패턴 → AIR fma 단일 instruction
- 무기: DXIL pattern matcher + columnar (fusion histogram)
- A=5 (DXIL → AIR translation, Apple GPTK/직접 변환), B=5, C=5

### round 2 (cycle 21 가능)

**C-METAL-PRIVATE-HEAP-PREWARM** (15/15 만점)
- 한 줄: pre-warm private heap pages (commit before first access)
- A=5, B=5, C=5

**C-NSPROCESSINFO-LOWPOWER** (15/15 만점)
- 한 줄: NSProcessInfo low-power mode → adaptive quality (resolution/effects)
- A=5 (NSProcessInfo macOS), B=5, C=5

**C-OS-LOG-RUNTIME-STREAM** (15/15 만점)
- 한 줄: macOS os_log structured streaming → runtime trace recovery
- A=5 (os_log), B=5, C=5

→ 채택 시 만점 +6 가능 (cycle 20+21).

### round 3 (포화 확인)

- C-METAL-CAPTURE-MANAGER (14/15, B=4 — debug only)
- C-DRIVERKIT-INPUT-LATENCY (14/15, C=4)
- C-DISPATCH-IO-CHANNEL (14/15, B=4)

---

## 3. EXHAUSTION 추적 (재진단)

### 누적 PSB 결과
| PSB | sketch 후보 | 채택 | 만점 추가 |
|---|---|---|---|
| PSB-1 ~ 10 | 누적 31 | 31 | +44 (cycle 11→19) |
| **PSB-11 (현재)** | **3 round1 / 6 만점 / 9 total** | **TBD** | **+6 (잠재)** |

### 종료 조건 진행
- PSB-10 EXHAUSTION 선언 후 재검토 → +6 만점 영역 발견
- PSB-11 round 1+2 채택 → 만점 **62**
- PSB-12 가능성: 영역 재검토 시 +2~3 가능
- PSB-13+ : 진짜 포화 가능

### 누적 만점 시점별 (확정 + 예측)
- cycle 19 (PSB-10): 56 ← 현재 ✅
- **cycle 20 (PSB-11 round 1): 59**
- cycle 21 (PSB-11 round 2): **62**
- cycle 22+ : PSB-12 가능성 재검토

---

## 4. 우선순위 (cycle 20)

| Sprint | 후보 | cycle |
|---|---|---|
| S34 | C-RT-AS-COMPACTION | cycle 20 |
| S35 | C-MTL-FUNCTION-POINTER-VTABLE | cycle 20 |
| S36 | C-DXIL-TO-AIR-FUSION | cycle 20 |

---

## 5. EXHAUSTION 정직성 v2

이전 cycle 19 EXHAUSTION 선언은 **premature** 였음:
- PSB-10 round 2 의 보조 후보만 검토하고 깊이 재탐색 부족
- Apple Silicon API surface 안 미탐색 영역 다수 (AS compaction, function pointer, AIR fusion 등)

→ 진짜 EXHAUSTION 은 **PSB-12+ 부근** (만점 64-65 부근).

---

*written 2026-05-01 (BG cycle 19 EXHAUSTION 후 재검토). PSB-11 sketch — re-opened territory.*
