# RFC A3 — dict / record literal stage1 fix

> **status**: draft, pre-PR (2026-04-30)
> **target repo**: github.com/dancinlab/hexa-lang
> **discovered by**: airgenome-gamebox M6 C-SHADER-AGING (자율 sprint)
> **priority**: HIGH — record-style data 가 복잡한 컴포넌트 차단

---

## 1. 문제

`hexa parse` 는 dict literal 통과:
```hexa
let r = { "hash": 12345, "bucket": 0, "size_kb": 32 }
```

→ parser 가 syntactically accept.

그러나 `hexa run` 실행 시 *`unexpected token LBrace ('{')`* 로 실패.

→ **parse 와 run 의 동작 불일치**. parse-only test 는 통과지만 실제 실행은 안 됨. 디버깅 어려움.

## 2. 영향 범위

| 컴포넌트 | 영향 |
|---|---|
| C-SHADER-AGING M6 | 각 shader record 가 4-field — TSV 우회 사용 |
| C-CORRELATE M14 | multi-source label fusion record — array of array 우회 |
| C-PSO M4 | 8-field PSO state — 평행 변수 우회 |
| airgenome modules/harvest.hexa | per-process 60-byte 게놈 record — struct 사용 |

→ struct 는 사용 가능하지만 anonymous record literal 이 안 됨.

## 3. 재현

```hexa
fn main() -> void {
    let r = { "hash": 12345, "bucket": 0 }   // hexa parse OK
    println(to_string(r))                     // hexa run FAIL
}
```

```
$ hexa parse t.hexa
OK: t.hexa parses cleanly

$ hexa run t.hexa
unexpected token LBrace ('{')
```

## 4. 현재 우회 (M6 sprint)

**c_shader_aging.hexa**:
```hexa
// dict literal 안되므로 TSV string + accessor functions
fn rec_pack(hash: int, last_used: int, bucket: int, size_kb: int) -> str {
    return to_string(hash) + "\t" + to_string(last_used) + "\t"
         + to_string(bucket) + "\t" + to_string(size_kb)
}

fn rec_hash(rec: str) -> int {
    return to_int(rec.split("\t")[0])
}

fn rec_last_used(rec: str) -> int {
    return to_int(rec.split("\t")[1])
}
// ... 등등 4 accessor
```

→ 4 field 마다 4 accessor function. record literal 1 줄로 가능한 것이 ~30 줄.

## 5. 제안

### 옵션 A: dict literal stage1 fix (권장)

`hexa parse` 통과하는 건 그대로 `hexa run` 도 동작.

```hexa
let r = { "hash": 12345, "bucket": 0 }
let h = r["hash"]   // 또는 r.hash
```

semantics: `{...}` = mutable map (string key → any value).

### 옵션 B: record syntax (struct literal)

이미 struct 가 있으니 anonymous instantiation 추가:
```hexa
struct Heap {
    tex_active: int,
    frag_pct: int
}

let h = Heap { tex_active: 1500, frag_pct: 12 }   // 이미 가능?
```

→ 명시 struct 만 OK 면 ad-hoc record 안 됨. anonymous tuple/record syntax 필요:
```hexa
let r = { hash: 12345, bucket: 0 }   // anonymous record
let h = r.hash
```

### 옵션 C: parse-time error (interim)

옵션 A/B 어려우면, `hexa parse` 에서 `{...}` literal 도 reject. parse OK / run FAIL 의 *동작 불일치* 가 가장 큰 issue.

→ 적어도 parse 가 정확히 reject 하면 디버깅 명확.

## 6. Migration

### 옵션 A 채택 시
- M6 c_shader_aging.hexa: TSV 우회 → dict literal 단순화 (~30 줄 → ~5 줄)
- M14 c_correlate.hexa: array of array → array of dict
- 기존 코드: 우회는 그대로 두되 새 코드는 dict 사용

### 옵션 B 채택 시
- struct 정의 + anonymous instantiation
- 더 type-safe

### 옵션 C 채택 시
- parse-error 만 fix (작은 patch)
- 우회는 계속 사용

## 7. Test plan

### 옵션 A
```hexa
@test
fn test_dict_literal() {
    let r = { "k": 1, "v": "hello" }
    assert(r["k"] == 1)
    assert(r["v"] == "hello")
}
```

### 옵션 C (interim)
```bash
echo 'let r = { "k": 1 }' > t.hexa
hexa parse t.hexa  # expected: ERROR (not OK)
```

## 8. Risk

- 옵션 A: parser + runtime 양쪽 변경. 큰 작업.
- 옵션 B: struct 만 사용 — runtime 영향 적음.
- 옵션 C: parser 에서 reject 만 — 가장 작은 patch.

## 9. 권장 단계

1. **immediate**: 옵션 C (parse 가 reject) — 디버깅 혼란 제거.
2. **near-term**: 옵션 A (dict literal 동작) — 표현력 향상.
3. **long-term**: 옵션 B (typed struct anon) — type safety.

---

*draft. M6 sprint 의 우회 코드 인용 추가 후 PR.*

---

## PR-ready summary (for upstream submission)

### Title
RFC: dict / record literal parse-vs-runtime consistency

### Description
`hexa parse` 는 dict literal `{ "k": v, ... }` 를 syntactically accept 하지만 `hexa run` 시 `unexpected token LBrace ('{')` 로 실패 — parse-vs-runtime 동작 불일치. record-style data 가 필요한 모든 컴포넌트가 TSV string + accessor 우회를 강제 채택 중. 본 RFC 는 dict literal stage1 fix (옵션 A), anonymous record syntax (옵션 B), 또는 parse-time reject (옵션 C, interim) 를 제안.

### Motivation
- parse OK / run FAIL — 디버깅 혼란의 가장 큰 원인
- airgenome-gamebox 영향:
  - C-SHADER-AGING M6 `c_shader_aging.hexa` — 4-field shader record TSV 우회 (~30 줄 비용, dict literal 1 줄로 가능)
  - C-CORRELATE M14 — multi-source label fusion 의 array-of-array 우회
  - C-PSO M4 — 8-field PSO state 평행 변수 우회
- airgenome `modules/harvest.hexa` — per-process 60-byte 게놈 record struct 사용 중 (anonymous literal 불가)
- `c_shader_aging.hexa` TSV 우회 코드 인용 (~30 줄 비용):
  > ```hexa
  > fn rec_pack(hash: int, last_used: int, bucket: int, size_kb: int) -> str {
  >     return to_string(hash) + "\t" + to_string(last_used) + "\t"
  >          + to_string(bucket) + "\t" + to_string(size_kb)
  > }
  > fn rec_hash(rec: str) -> int { return to_int(rec.split("\t")[0]) }
  > fn rec_last_used(rec: str) -> int { return to_int(rec.split("\t")[1]) }
  > // ... 4 field 마다 4 accessor function
  > ```

### Proposal
권장 단계:
1. **immediate (option C)** — `hexa parse` 가 `{...}` literal 도 정확히 reject. parse-vs-run 동작 일치. 작은 patch.
2. **near-term (option A)** — dict literal 실행 동작: `let r = { "k": 1 }; let v = r["k"]`. semantics: mutable map (string key → any).
3. **long-term (option B)** — anonymous record / typed struct literal 로 type safety.

### Migration path
- additive only — 기존 TSV/array-of-array 우회 코드 유지 가능
- 옵션 C 채택 시: parse 가 명확히 reject — debugging 혼란 즉시 제거
- 옵션 A 채택 시: c_shader_aging.hexa TSV 우회 (~30 줄) → dict literal (~5 줄) 단순화, c_correlate.hexa array-of-array → array-of-dict
- 옵션 B 채택 시: struct 정의 + anonymous instantiation, 더 type-safe

### Test plan
```hexa
// option A
@test fn test_dict_literal() {
    let r = { "k": 1, "v": "hello" }
    assert(r["k"] == 1)
    assert(r["v"] == "hello")
}
```
```bash
# option C interim
echo 'let r = { "k": 1 }' > t.hexa
hexa parse t.hexa  # expected: ERROR (not OK)
hexa run   t.hexa  # expected: ERROR (consistent)
```
- M6 c_shader_aging.hexa TSV 우회 → dict literal 회귀 테스트
- M14 c_correlate.hexa array-of-array → array-of-dict 회귀 테스트

### Related upstream PRs / issues
- (TBD — to be filled after submission)

### Acknowledgements
- airgenome-gamebox project (DIY hexa Win32 emulation — M6 sprint 우회 코드 발견)
- airgenome project (vitals scanner — modules/harvest.hexa struct 패턴 참조)
