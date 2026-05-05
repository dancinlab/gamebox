# RFC A1 — stage1 float arithmetic 안정화

> **status**: draft, pre-PR (2026-04-30)
> **target repo**: github.com/need-singularity/hexa-lang
> **discovered by**: airgenome (probe.hexa workaround) + airgenome-gamebox (M2-M14 자율 sprint)
> **priority**: CRITICAL — D-list 만점 컴포넌트 대부분 차단

---

## 1. 문제

stage1 hexa-lang interpreter 에서 float 연산 `* / > < ==` 가 잘못된 결과 반환.

**airgenome probe.hexa#assess() 주석 (인용)**:
> "hexa-lang stage1 float 산술/비교 (`*`, `>`, `<`, `==`) 가 잘못된 결과 반환.
>  우회: to_int 변환 직후 모든 비교/산술을 int 로 수행. 1% 정밀도 손실 허용."

## 2. 영향 범위

| 컴포넌트 | 영향 |
|---|---|
| C-FRAMETIME (M2) | gpu_ms / cpu_ms 단위 fp 비교 → 정확도 우려 |
| C-HEAP (M3) | frag_pct = (used / total) * 100 → 오류 |
| C-FRAME (M5) | Holt smoothing factor (alpha=0.6 etc.) |
| C-PSO (M4) | hash 정규화 |
| airgenome forecast.hexa | Holt 이중 지수평활 |

→ 모든 컴포넌트가 *int math 우회* 사용 중. 1% 정밀도 손실 누적 위험.

## 3. 재현

```hexa
let a = 0.6
let b = 0.4
let c = a * b   // 기대: 0.24, 실제 stage1: 잘못된 값
println(to_string(c))
```

probe.hexa 가 명시적 우회 주석 남김 — 재현 가능.

## 4. 우회 패턴 (현재)

**Holt 1초 ahead forecast** (c_frame.hexa from sprint):
```hexa
// alpha, beta ∈ [0..100] 정수 (percent)
let level_t = (alpha * value + (100 - alpha) * one_step) / 100
let trend_t = (beta * (level_t - prev_level) + (100 - beta) * prev_trend) / 100
```

→ float 0.6 대신 int 60. 100 으로 나눔. 정밀도 손실 1%.

## 5. 제안 API

### 옵션 A: stage1 fix (권장)

float 연산 빌트인 fix. 사용자 코드 변경 없음.

### 옵션 B: `f64_*` 함수 패밀리 (interim)

stage1 fix 어려우면 명시적 함수:
```hexa
fn f64_mul(a: float, b: float) -> float
fn f64_div(a: float, b: float) -> float
fn f64_add(a: float, b: float) -> float
fn f64_sub(a: float, b: float) -> float
fn f64_lt(a: float, b: float) -> bool
fn f64_gt(a: float, b: float) -> bool
fn f64_eq(a: float, b: float) -> bool   // ε-tolerant
```

→ 사용자가 명시적 호출. probe.hexa 주석 같은 우회 코드 제거 가능.

### 옵션 C: stage2 native float

stage1 의 float 자체를 deprecate. stage2 에서 IEEE 754 native 정렬.
→ longer term, 가장 깨끗.

## 6. Migration

### 옵션 A 채택 시
- 모든 `to_int + percent` 우회 코드를 *원래 float* 로 복구 가능 (점진적)
- airgenome probe.hexa#assess() 주석 제거 가능
- airgenome-gamebox c_frame / c_shader_warm_pp / c_correlate 의 Holt int math 단순화

### 옵션 B 채택 시
- 새로 작성하는 코드만 `f64_*` 사용
- 기존 우회 코드는 유지 (자율 마이그레이션)

## 7. Test plan

### 정확도 회귀
- `0.1 + 0.2 == 0.3` (ε-tolerant)
- `1e10 * 1e-10 ≈ 1.0`
- `(1/3) * 3 ≈ 1.0`

### probe.hexa 검증
- assess() 주석 제거 후 동일 테스트 결과 확인

### Holt forecast 정확도
- airgenome modules/forecast.hexa M6 self-test pass after migration

## 8. Risk

- 옵션 A: stage1 internals 변경 — 다른 부수효과 가능성. 회귀 테스트 필수.
- 옵션 B: API 추가만 — 안전. 나쁘지 않음.
- 옵션 C: 큰 변경 — stage2 plan 일치 필요.

## 9. 권장 단계

1. **immediate**: 옵션 B 추가 (`f64_*` 함수). 새 코드 즉시 사용 가능.
2. **next**: 옵션 A 검토 (stage1 fix 가능한지).
3. **long-term**: 옵션 C (stage2 native).

---

*draft. airgenome maintainer 검토 후 hexa-lang upstream PR.*

---

## PR-ready summary (for upstream submission)

### Title
RFC: stage1 float arithmetic stabilization

### Description
stage1 hexa-lang interpreter 의 float 연산 (`*`, `/`, `>`, `<`, `==`) 이 잘못된 결과를 반환하여 downstream 프로젝트들이 int-percent workaround 를 강제로 채택 중. airgenome (vitals scanner) 의 `probe.hexa#assess()` 가 명시적 우회 주석을 남겼고, airgenome-gamebox 의 17 개 hexa 모듈이 동일 우회로 1% 정밀도 손실을 누적 중. 본 RFC 는 stage1 fix (옵션 A), `f64_*` interim 함수 (옵션 B), stage2 native float (옵션 C) 의 3 단계 stabilization 경로를 제안.

### Motivation
- airgenome `modules/probe.hexa#assess()` — 명시적 워크어라운드 주석 인용:
  > "hexa-lang stage1 float 산술/비교 (`*`, `>`, `<`, `==`) 가 잘못된 결과 반환. 우회: to_int 변환 직후 모든 비교/산술을 int 로 수행. 1% 정밀도 손실 허용."
- airgenome-gamebox (commit 0477ae1+) — 17 hexa modules use int-percent workaround
- C-FRAMETIME (M2) `c_frametime.hexa` — gpu_ms / cpu_ms 단위 fp 비교 정확도 우려
- C-HEAP (M3) `c_heap.hexa` — `frag_pct = (used / total) * 100` 직접 계산 불가
- C-PSO (M4) `c_pso.hexa` — hash 정규화 fp 경로 차단
- C-FRAME (M5) — Holt smoothing factor (alpha=0.6 등) int*100 우회
- airgenome `modules/forecast.hexa` — Holt 이중 지수평활 int math 우회

### Proposal
권장 단계:
1. **immediate (option B)** — `f64_mul / f64_div / f64_add / f64_sub / f64_lt / f64_gt / f64_eq` 빌트인 함수 추가. ε-tolerant equality. 새 코드 즉시 사용 가능.
2. **next (option A)** — stage1 interpreter 의 float 연산 builtin fix. 기존 사용자 코드 그대로 동작.
3. **long-term (option C)** — stage2 IEEE 754 native float 정렬, stage1 float deprecate.

### Migration path
- additive only — 기존 int-percent 우회 코드 유지 가능
- 옵션 A 채택 시: airgenome `probe.hexa#assess()` 주석 제거, gamebox c_frame / c_shader_warm_pp / c_correlate 의 Holt int math 단순화 가능
- 옵션 B 채택 시: 새 코드만 `f64_*` 호출, 기존 우회는 자율 마이그레이션

### Test plan
- `0.1 + 0.2 == 0.3` (ε-tolerant)
- `1e10 * 1e-10 ≈ 1.0`
- `(1/3) * 3 ≈ 1.0`
- airgenome `probe.hexa#assess()` 주석 제거 후 동일 테스트 결과 확인
- airgenome `modules/forecast.hexa` M6 self-test pass after migration
- gamebox 17 modules 의 int-percent → float 점진적 회귀 테스트

### Related upstream PRs / issues
- (TBD — to be filled after submission)

### Acknowledgements
- airgenome-gamebox project (DIY hexa Win32 emulation)
- airgenome project (vitals scanner — probe.hexa workaround discoverer)
