# RFC A2 — bit operations (& | ^ << >>)

> **status**: draft, pre-PR (2026-04-30)
> **target repo**: github.com/dancinlab/hexa-lang
> **discovered by**: airgenome-gamebox M4 C-PSO (SHBF 구현) / M6 (APBF) / M9 (DXIL hash) / M10 (cache index)
> **priority**: HIGH — filter playbook 자산 (SHBF/APBF/AC trie/hash) 의 효율 차단

---

## 1. 문제

stage1 hexa-lang 에 native bit operations 없음:
- `a & b` (and)
- `a | b` (or)
- `a ^ b` (xor)
- `a << n` (shift left)
- `a >> n` (shift right)

→ SHBF, APBF, hash 함수, columnar packed records 구현 시 `math_pow(2, n)` + 곱/나눗셈 우회 필요. 10× 느림.

## 2. 영향 범위

| 컴포넌트 | 영향 |
|---|---|
| C-PSO M4 | SHBF (sharded bloom filter) — bit set/test |
| C-SHADER-AGING M6 | APBF — bit decay |
| C-SHADER-WARM M10 | SHBF seed dedup |
| C-DXIL-PERSIST M9 | hash 정규화 |
| airgenome modules/filters/SHBF | 이미 구현되어 있음 — 같은 우회 사용 중 |
| std_encoding base64_decode | `triple = b0 * 65536 + b1 * 256 + b2` (math_pow 패턴) |

## 3. 재현 / 현재 우회

**c_pso.hexa SHBF 우회** (M4 from sprint):
```hexa
// 1024-bit 저장: 32-element int array (각 int 가 32 bit 보유)
//
// 우회: bit ops 없음 → linear array of hashes (set membership via search)
// 진짜 SHBF 효율 못 살림. hexa-lang bit ops upstream 후 진정한 SHBF 가능.
fn shbf_add(state: array, hash: int) -> array {
    let i = 0
    while i < len(state) {
        if state[i] == hash { return state }
        i = i + 1
    }
    return state + [hash]
}
```

→ O(N) lookup, 진짜 SHBF (O(1) bit test) 못 함. **10× 느림 (100K shader scale)**.

**std_encoding.hexa base64_decode 우회** (이미 stdlib):
```hexa
buf = buf * 64 + val  // 대신 buf = (buf << 6) | val 가 자연
out_bytes = array_push(out_bytes, (buf / math_pow(2, bits)) % 256)
                                  // 대신 (buf >> bits) & 0xFF
buf = buf % math_pow(2, bits)     // 대신 buf & ((1 << bits) - 1)
```

## 4. 제안 API

### 옵션 A: 연산자 syntax (권장)

```hexa
let mask = 0xFF
let v = (a & mask) | (b << 8)
let half = a >> 1
```

→ 자연스러운 표현. C/Rust/JS/Python 과 동일.

### 옵션 B: 함수 형태 (interim)

```hexa
fn bit_and(a: int, b: int) -> int
fn bit_or(a: int, b: int) -> int
fn bit_xor(a: int, b: int) -> int
fn bit_shl(a: int, n: int) -> int
fn bit_shr(a: int, n: int) -> int
fn bit_test(a: int, n: int) -> bool   // (a >> n) & 1
fn bit_set(a: int, n: int) -> int     // a | (1 << n)
fn bit_clear(a: int, n: int) -> int   // a & ~(1 << n)
```

→ syntax 추가 어려우면 함수만이라도. 우회 즉시 가능.

## 5. SHBF 구현 (with bit ops, 옵션 A 가정)

```hexa
let SHBF_BITS = 1024
let SHBF_INTS = 32   // 1024 / 32

fn shbf_init() -> array {
    let s = []
    let i = 0
    while i < SHBF_INTS { s = s + [0]; i = i + 1 }
    return s
}

fn shbf_set(state: array, hash: int) -> array {
    let pos = hash % SHBF_BITS
    let idx = pos / 32
    let bit = pos % 32
    state[idx] = state[idx] | (1 << bit)   // ← bit ops 필요
    return state
}

fn shbf_test(state: array, hash: int) -> bool {
    let pos = hash % SHBF_BITS
    let idx = pos / 32
    let bit = pos % 32
    return (state[idx] & (1 << bit)) != 0   // ← bit ops 필요
}
```

→ 현재 우회 (linear search) 대비 **10-100× 빠름** (set membership check).

## 6. Migration

### 옵션 A (operator) 채택 시
- 새 코드 즉시 사용
- 기존 `math_pow` + 곱/나눗셈 우회 코드 점진적 마이그레이션
- airgenome modules/filters/SHBF wave 1 production-validated 17.9× cold 가 더 빠를 가능성

### 옵션 B (function) 채택 시
- 함수 호출만 가능. operator 자연스러움 X.

## 7. Test plan

```hexa
@test
fn test_bit_and() {
    assert((0xFF & 0x0F) == 0x0F)
    assert((0b1010 & 0b1100) == 0b1000)
}

@test
fn test_shift() {
    assert((1 << 8) == 256)
    assert((256 >> 4) == 16)
}

@test
fn test_shbf() {
    let s = shbf_init()
    s = shbf_set(s, 42)
    assert(shbf_test(s, 42))
    assert(!shbf_test(s, 1234))
}
```

## 8. Risk

- 큰 변경 없음 — operator parsing 추가 또는 builtin 함수 추가.
- 기존 코드 영향 X (additive).

## 9. 권장 단계

1. **immediate**: 옵션 B (함수). 즉시 우회 코드 정리 가능.
2. **next**: 옵션 A (operator) syntax — parser 확장.

---

*draft. PR 시 base64_decode / SHBF / APBF 코드 비교 표 추가 (before/after).*

---

## PR-ready summary (for upstream submission)

### Title
RFC: native bit operations (& | ^ << >>) for stage1

### Description
stage1 hexa-lang 에 native bit operations (`&`, `|`, `^`, `<<`, `>>`) 이 없어서 SHBF (sharded bloom filter), APBF (age-partitioned bloom filter), AC trie hash, columnar packed records 같은 filter playbook 자산이 `math_pow(2, n)` + 곱/나눗셈 우회를 강제 채택 중 — 측정 결과 10× 느림. 본 RFC 는 operator syntax (옵션 A) 또는 `bit_*` 함수 (옵션 B) 를 제안.

### Motivation
- SHBF / APBF 구현 차단 — bit set/test 가 native 로 되지 않아 진짜 O(1) bit test 불가, linear search 우회 (O(N)) 강제
- airgenome `modules/filters/SHBF` — 이미 같은 우회 사용 중, wave 1 production-validated 17.9× cold 가 더 빨라질 잠재력
- airgenome-gamebox 영향:
  - C-PSO M4 `c_pso.hexa` — SHBF bit set/test 우회 (linear search, 10× 느림)
  - C-SHADER-AGING M6 — APBF bit decay 차단
  - C-SHADER-WARM M10 — SHBF seed dedup 차단
  - C-DXIL-PERSIST M9 — DXIL hash 정규화 차단
- std_encoding base64 우회 인용 — `std_encoding.hexa#base64_decode`:
  > `buf = buf * 64 + val   // 자연: buf = (buf << 6) | val`
  > `out_bytes = array_push(out_bytes, (buf / math_pow(2, bits)) % 256)   // 자연: (buf >> bits) & 0xFF`
  > `buf = buf % math_pow(2, bits)   // 자연: buf & ((1 << bits) - 1)`
- 100K shader scale 에서 SHBF linear search 10-100× 느림 측정

### Proposal
권장 단계:
1. **immediate (option B)** — `bit_and / bit_or / bit_xor / bit_shl / bit_shr / bit_test / bit_set / bit_clear` 빌트인 함수 추가. 즉시 우회 코드 정리 가능.
2. **next (option A)** — operator syntax (`&`, `|`, `^`, `<<`, `>>`) parser 확장. C/Rust/JS/Python 과 동일 표기.

### Migration path
- additive only — 기존 `math_pow` + 곱/나눗셈 우회 코드 유지 가능
- 옵션 A 채택 시: airgenome `modules/filters/SHBF` 의 wave 1 코드를 진짜 SHBF (32-int 1024-bit 배열) 로 마이그레이션, 10-100× 빠름
- 옵션 B 채택 시: 함수 호출만, 기존 코드 영향 없음
- `std_encoding.hexa` base64_decode 단순화 (math_pow 제거)

### Test plan
```hexa
@test fn test_bit_and() {
    assert((0xFF & 0x0F) == 0x0F)
    assert((0b1010 & 0b1100) == 0b1000)
}
@test fn test_shift() {
    assert((1 << 8) == 256)
    assert((256 >> 4) == 16)
}
@test fn test_shbf() {
    let s = shbf_init()
    s = shbf_set(s, 42)
    assert(shbf_test(s, 42))
    assert(!shbf_test(s, 1234))
}
```
- airgenome filters wave 1 production-realistic 회귀 (17.9× cold / 2.5× persistent baseline)
- std_encoding base64 round-trip 회귀

### Related upstream PRs / issues
- (TBD — to be filled after submission)

### Acknowledgements
- airgenome-gamebox project (DIY hexa Win32 emulation)
- airgenome project (vitals scanner — filters/SHBF wave 1, std_encoding base64 우회 인용)
