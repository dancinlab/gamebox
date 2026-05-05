# HEXA_UPSTREAM_PROPOSAL §6.2 — std_json / JSONL parser

> Proposal spec doc for hexa-lang stdlib `std_json` 모듈 — JSON decode / encode /
> JSONL line iteration / dotted path get.
> **NOT** an actual hexa-lang PR — sibling repo (`/Users/ghost/core/hexa-lang/`) modification
> is DEFERRED pending user explicit approval per Track AO marker
> `study_only_pr_pending_user_approval`. Track AW lands gamebox-side proposal spec doc
> ONLY; future cycle copies into `hexa-lang/proposals/` format upon greenlight.

## §0 목적 / 한계

본 문서는 hexa-lang stage1 의 JSON parser 부재 — 현재 §1.2 항목은 **encode 만** 제안되어
있고 **decode 는 미제안** — 를 보완하는 stdlib `std_json` 모듈 제안의 정형화. Track AB+AE
누적 21+ hit site (`tool/roadmap_op.hexa` checkpoint header bracket walker + extractor 합산)
가 매 도구 / 매 cycle 마다 동일 패턴 (depth-tracking + escape handling) 을 재구현하는
non-DRY 상태의 정리.

본 문서는 **proposal spec** 이지 실 PR 이 아니다 — hexa-lang sibling repo 의 디렉터리 /
커밋 / 리뷰 작업은 user explicit approval 가 떨어져야 별도 cycle 에서 진행한다.

doc-only / additive / migration 0 / destructive 0 / sibling repo modifications 0.

## §1 problem statement

Track AB (`tool/roadmap_op.hexa` `subcmd_checkpoint_status` 의 depth-aware bracket walker)
+ Track AE (8 신규 helper fn: `parse_uint_safe` / `extract_int_field` /
`checkpoint_extract_aggregation_kind` / `checkpoint_extract_threshold_kind` /
`checkpoint_extract_threshold_value` / `checkpoint_ref_weight` / `stderr_warn` /
`checkpoint_aggregate_weighted_threshold`) 누적 21+ hit site.

각 hit site 패턴:
- `extract_str_after(line, key)` — `"key":"VALUE"` 형식 line-grep 추출.
- `extract_int_field(obj, key, default_val)` — 동일 패턴 int 버전.
- `checkpoint_extract_refs_body(header)` — depth-aware bracket walker (square bracket
  inside string + escape flag 처리).
- `checkpoint_extract_aggregation_kind/threshold_kind/threshold_value` — 각 도구가 동일
  bracket walker 를 cargo-cult 복제.

Track AB caveat C5 (`docs/HEXA_UPSTREAM.md` §6.2 cite) 가 명시:
> "checkpoint_extract_refs_body is a depth-aware bracket walker, not a JSON parser. Same
> trust assumption as the rest of the tool — the header is single-line strict JSON per mk2
> convention. Adversarial inputs (e.g., square brackets inside strings) are handled via
> the in-string + escape flags. Production-grade JSON parsing would require a separate
> library (out of scope for own1 hexa-only)."

→ 본 spec 의 land 가 정확히 own1 정합 stdlib library 를 제공한다. 현 hand-roll 21+ hit
site 는 모두 std_json 으로 collapse.

Track AV §8.2 B-bench 측정: hand_rolled 110 LOC per pass / measured ns/op (hexa 런타임).
hypothetical native (`std_json` simdjson-class extrapolation arm64) 는 6 LOC per pass /
~800 ns/op estimate.

## §2 proposed API — full signature freeze

```hexa
// std_json — JSON decode / encode / JSONL iter / dotted path get
// own1 정합: hexa-only, no FFI, pure hexa stdlib.

fn json_decode(s: str) -> any
    // JSON 문자열 → hexa value (map / array / scalar / bool / null).
    // strict mode: trailing comma / unquoted key / single quote 모두 reject.
    // malformed input 시 null 반환 + stderr warn (silent-error-ban 정합).

fn json_encode(value: any) -> str
    // hexa value → JSON 문자열. quote / escape 자동.
    // map key 는 string only — non-string key 시 stderr warn.
    // pretty-print 미제공 (compact only) — pretty 는 future 확장.

fn jsonl_iter(path: str) -> array
    // JSONL 파일을 line 단위 parsed value array 로 반환.
    // 빈 line skip; malformed line 은 array 에 null 삽입 + stderr warn.
    // 큰 파일은 streaming iterator 가 미래 확장 — 본 spec 은 in-memory.

fn json_path_get(value: any, dotted_path: str) -> any
    // dotted path 로 nested field 추출.
    // 예: json_path_get(v, "blockers.0.status_enum_adopted")
    //     → v["blockers"][0]["status_enum_adopted"]
    // path 비존재 시 null 반환 (warn 없음 — caller 가 null check).
    // array index 는 0-base; "blockers.length" 같은 metadata 는 미제공 — caller 가
    // value array 자체를 받아 length 사용.
```

## §3 reference impl outline (pseudo-hexa, 50-100 LOC estimate)

```hexa
// std_json — reference implementation skeleton

fn json_decode(s: str) -> any {
    let p = Parser{ src: s, pos: 0 }
    skip_ws(p)
    let v = parse_value(p)
    skip_ws(p)
    if p.pos != s.length { stderr_warn("json_decode: trailing_garbage"); return null }
    return v
}

fn parse_value(p: Parser) -> any {
    let c = peek(p)
    if c == "{" { return parse_object(p) }
    if c == "[" { return parse_array(p) }
    if c == "\"" { return parse_string(p) }
    if c == "t" || c == "f" { return parse_bool(p) }
    if c == "n" { return parse_null(p) }
    return parse_number(p)
}

// parse_object: { "key": value, ... } — escape / depth tracking
// parse_array:  [ value, value, ... ] — recursive
// parse_string: "..." with \" \\ \n \t \uXXXX escape
// parse_number: int / decimal / scientific (decimal 은 §6.1 fixed_point 와 별개 raw str)

fn json_encode(value: any) -> str { ... }   // ~25 LOC
fn jsonl_iter(path: str) -> array { ... }   // ~15 LOC
fn json_path_get(value, dotted_path) -> any { ... }   // ~20 LOC
```

총 추정 ~90 LOC (parser + encoder + path walker 합산). 큰 파일 streaming 은 future 확장.

## §4 test cases (12 cases)

1. `json_decode("{\"a\":1}") == {a: 1}` — basic object
2. `json_decode("[1,2,3]") == [1, 2, 3]` — basic array
3. `json_decode("\"hello\"") == "hello"` — string scalar
4. `json_decode("true") == true` — bool
5. `json_decode("null") == null` — null
6. `json_decode("{\"a\":[1,{\"b\":2}]}")` — nested
7. `json_decode("{\"s\":\"with\\\"quote\"}")` — escape handling
8. `json_encode({a: 1, b: [2, 3]}) == "{\"a\":1,\"b\":[2,3]}"` — encode round-trip
9. `json_path_get(v, "a.b.c") == 42` — dotted path
10. `json_path_get(v, "a.0.b") == "x"` — array index in path
11. `json_path_get(v, "missing.key") == null` — missing path → null
12. `jsonl_iter("file.jsonl")[2]["status"] == "ok"` — JSONL iter + index

추가 edge: trailing comma reject, single quote reject, unicode `\uXXXX`, empty object
`{}`, empty array `[]`, deeply nested (>32 depth).

## §5 breaking changes — none (additive new module)

`std_json` 는 신규 모듈. 기존 `json_object` (§1.2 의 encode-only 제안) 가 future 에 land
하더라도 본 모듈은 `json_decode` + `json_path_get` + `jsonl_iter` 추가만 — `json_encode`
sig 는 §1.2 의 `json_encode(value: any) -> str` 와 정합.

기존 user 코드 영향 0. opt-in `import std_json`.

## §6 alternative considered

**Option A**: regex-based JSON parser.
**Verdict**: REJECTED. (i) hexa stage1 의 regex 부재 (§1.3 항목) — regex stdlib 가 land
하기 전엔 self-bootstrap 불가. (ii) regex JSON parsing 은 nested depth + escape handling
이 정확하지 않음 (well-known anti-pattern). bracket walker (현재 hand-roll) 가 정작 더
robust — 본 spec 의 reference impl 은 bracket walker 의 generalization.

**Option B**: simdjson FFI binding.
**Verdict**: REJECTED. own1 wine_0_hexa_only 정합 위반 — 외부 C lib FFI 는 sibling repo
의 stage1 본격 build pipeline 변경 필요. stage2 native 까지 deferred 가 합리적.

**Option C**: per-tool hand-roll bracket walker (현재 상태).
**Verdict**: REJECTED. Track AB+AE 21+ hit site × 평균 ~22 LOC = ~462 LOC 등가 누적
hand-roll. 신규 도구 land 시마다 cargo-cult 복제. cumulative future N 도구 → 확장성 0.

## §7 sibling PR submission spec

**디렉터리 layout** (sibling repo `/Users/ghost/core/hexa-lang/` 가 hosts 했을 때):
- `proposals/PROPOSAL_STD_JSON.md` — 본 문서의 hexa-lang 측 사본.
- `src/std/json.hexa` — reference impl (§3 outline land 형태).
- `tests/std/test_json.hexa` — §4 의 12+ test case + edge.

**커밋 메시지 template**:
```
add std_json — JSON decode / encode / JSONL iter / path get

Track AB+AE-driven proposal: roadmap_op checkpoint header bracket walker
+ 8 extractor helpers (~462 LOC equivalent across 21+ hit sites)
generalized into stdlib.

- json_decode / json_encode / jsonl_iter / json_path_get
- additive: complements existing §1.2 json_encode-only proposal
- no breaking changes
- alternative regex / simdjson FFI rejected: bracket walker simpler + own1 정합
```

**리뷰 checklist**:
- API signatures match §2 freeze.
- 12 test cases pass + edge (escape / unicode / trailing comma reject).
- depth tracking 정확 (>32 nested).
- malformed input → null + warn (silent-error-ban 정합).
- json_path_get array index 0-base.

## §8 dependencies — none

hexa stdlib internal 모듈만 사용. 기존 builtin 의 string substring / index / `stderr_warn`
(§6.5 future) 사용 — 본 제안과 독립 dependency 가 아닌 평행 진행 가능.

§1.2 의 `json_encode` 단독 제안과 conflict 없음 — 본 모듈은 4 fn 모두 addition.

## §9 caveats (≥6, 11 listed)

C1. 본 문서는 proposal spec — actual hexa-lang PR 아님. sibling repo 작업은 user
    explicit approval pending.
C2. json_decode 는 strict mode only — JSON5 / JSONC (trailing comma / comment) 미지원.
    domain-specific extension 은 caller 가 pre-process.
C3. jsonl_iter 는 in-memory — 큰 파일 (수 GB) 은 OOM 가능. streaming iterator 는 future
    확장 (`std_json.jsonl_stream`).
C4. json_path_get 의 dotted path 는 escape 미지원 — key 에 `.` 들어가면 ambiguous.
    workaround: caller 가 nested map access 직접 수행.
C5. Track AB caveat C5 의 "production-grade JSON parsing 은 own1 hexa-only scope 외" 진단을
    본 spec 이 정정 — own1 정합 stdlib (FFI 0) 로 production-grade parser 가 가능.
C6. Track AV §8.2 의 hypothetical native ns/op 800 estimate 는 simdjson-class arm64
    extrapolation — own 2 honest 정합으로 estimate 명시. 실제 hand_rolled 가 더 빠를
    수도 (~600 byte 작은 입력) / 느릴 수도 — bench 진가는 LOC delta (110 → 6).
C7. 21+ hit count 의 462 LOC 추정은 Track AB+AE 만 측정 — 다른 도구 (예: closure_orchestrator
    의 `read_progress_field_from_marker` §6.4) 는 별도 hit 으로 미포함. §6.4 가 본
    모듈의 sub-entry 로 통합 권고.
C8. Track AW 는 doc-only — `lib/` / `tool/` / `tests/` 어떤 파일도 수정 0.
C9. `std_json` 네이밍은 hexa-lang convention 의 `std_encoding` (기존) 와 정합. sibling
    repo 의 actual convention 은 PR 시점에 cross-check.
C10. 본 제안 land 시 roadmap_op 의 8 extractor + bracket walker 는 **deprecate 아님** —
     migration 0 정책 정합으로 호환 layer 유지, 신규 코드만 새 API 사용 권장.
C11. own 5 status: C-hit 21+ (Track AB+AE) + B-bench 110→6 LOC measured (Track AV §8.2) =
     `c_plus_b_pr_ready_pending_user_approval`. Track AW 는 본 status 의 spec doc 형태로
     정형화 한정 — status 자체 진전 없음.

---

*Track AW PR-ready proposal spec doc, written 2026-05-04. doc-only / additive /
migration 0 / destructive 0 / sibling repo modifications 0.*
