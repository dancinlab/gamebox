# HEXA_UPSTREAM_PROPOSAL §6.5 — stderr explicit warn channel (eprintln)

> Proposal spec doc for hexa-lang `core.io` 모듈 신규 builtin 2 fn —
> `eprintln(s: str) -> void` + `stderr_write(s: str) -> int`.
> **NOT** an actual hexa-lang PR — sibling repo (`/Users/ghost/core/hexa-lang/`)
> modification 은 user explicit approval 까지 deferred. Track BC 는 gamebox 측
> proposal spec doc 한정 land — PR #47 merged (rfc_012 / rfc_013 / rfc_014,
> 2026-05-04T11:48:58Z) 의 sibling repo 측 추가 RFC submission 은 user 의 별도
> 명시적 동의가 떨어져야 진행. own 1 wine 0 / hexa-lang sibling NOT modified.

## §0 목적 / 한계

본 문서는 hexa-lang stage1 의 first-class stderr channel API 부재 — `println` 이
stdout 만 가짐 — 를 stdlib `core.io.eprintln` + `core.io.stderr_write` builtin 2-fn
신규 추가로 보완하는 PR-ready proposal spec.

Track AE (`tool/roadmap_op.hexa` line 1788 `stderr_warn` fn) + Track AB 측 유사
hand-roll 패턴 누적 2 hit. 추가로 Track AG 의 silent-error-ban 정합 path 에서
`stderr_warn` 호출이 implicit 의존성 (예: rfc_012 의 `fixed_point_x1000` 이
overflow 시 stderr warn — Track AW 측 spec doc §3 cite).

doc-only / additive / migration 0 / destructive 0 / sibling repo modifications 0 /
in_place_writes 0.

## §1 problem statement

Track AE 가 `tool/roadmap_op.hexa` 에 `stderr_warn(msg: str) -> void` 헬퍼를 정의
— hexa stage1 의 `println` 이 stdout 만 가므로 우회 (현재 구현은 `exec("echo ...
>&2")` shell escape).

본 패턴이 도구마다 별도로 정의:
- `tool/roadmap_op.hexa` 의 `stderr_warn` (Track AE).
- Track AB 측 유사 hand-roll (`tool/closure_orchestrator.hexa` 등의 implicit warn
  패턴 — `println` 으로 stdout 출력 후 caller 가 stdout 측 noise 에 민감).

shell escape 우회의 측정 가능한 부담:
- `exec("echo ...")` 의 fork 비용: hexa stage1 의 `exec` builtin 이 `posix_spawn` 또는
  `fork+execve` 를 호출 — 매 warn 당 ms order overhead.
- shell quoting 위험: msg 내 special char (예: `"`, `$`, backtick) 의 escape 누락 시
  shell injection 위험.
- silent-error-ban (raw 12) 정합으로 매 silent-failure path 가 stderr warn 발산 —
  hot path 에 stderr warn 이 누적 호출되면 fork cost 가 누적.

각 도구의 hand-roll 측 hit site 패턴:
- `stderr_warn(msg)` — `exec("echo " + shell_escape(msg) + " >&2")` 형태.
- `eprintln_inline_msg(msg)` — 일부 inline (도구 내 단일 use) 패턴.
- 기타 silent-error-ban 정합 path 의 implicit warn (Track AG `fixed_point_x1000`
  zero-denom warn / rfc_013 `json_decode` malformed warn / rfc_013 `jsonl_iter`
  malformed line warn / rfc_014 `struct_pack` overflow warn — 모두 stderr 의존
  but stage1 first-class API 부재).

→ first-class `core.io.eprintln` builtin 가 land 시:
- shell escape fork cost 0 (direct fd=2 write).
- shell injection 위험 0.
- silent-error-ban 정합 path 의 stderr warn 호출 cost 가 fork → syscall write 1회
  로 collapse.

## §2 proposed API — full signature freeze

```hexa
// core.io — first-class stdout / stderr channel builtins
// own1 정합: hexa-only, no FFI, native syscall (write(2)) 직접 호출.

fn eprintln(s: str) -> void
    // stderr 로 string + newline 출력.
    // println 의 stderr 버전 — stdout 측 정상 출력과 분리.
    // syscall write(2, buf, len) 직접 호출 (shell escape 우회 X).
    // 실패 시 silent (stderr 자체 실패는 escalate 불가).

fn stderr_write(s: str) -> int
    // stderr 로 string 직접 write — newline 자동 추가 X.
    // returns: bytes written (성공 시), -1 (실패 시 — fd=2 closed 등 edge case).
    // raw 데이터 / structured warn / progress bar 등 newline 제어 필요 시 사용.
    // eprintln 은 본 fn 의 wrapper: stderr_write(s) + stderr_write("\n").
```

## §3 reference impl outline (pseudo-hexa, ~30-50 LOC estimate)

```hexa
// core.io — reference implementation skeleton
// 본 모듈은 hexa stage1 의 native syscall 측 builtin 으로 구현 (FFI X).

fn stderr_write(s: str) -> int {
    // hexa stage1 가 native syscall write(2) 를 dispatch.
    // fd=2 (stderr) 가 closed 또는 broken pipe 시 -1 반환.
    let len = str_byte_len(s)
    let bytes_written = native_syscall_write(2, s, len)
    return bytes_written
}

fn eprintln(s: str) -> void {
    // stderr_write 의 wrapper + newline.
    // 실패는 silent — stderr 자체 실패는 caller 가 처리 불가.
    let _ = stderr_write(s)
    let _ = stderr_write("\n")
    return
}

// 추가 helper (informational, public API 외):
fn eprintln_with_prefix(prefix: str, msg: str) -> void {
    // ANSI color / log level prefix 등 caller 측 wrapper 후보.
    // 본 spec 외 — caller 가 임의 prefix 합성 가능.
    let _ = stderr_write(prefix)
    let _ = stderr_write(": ")
    let _ = stderr_write(msg)
    let _ = stderr_write("\n")
    return
}
```

총 추정 ~30-50 LOC (native syscall dispatch wrapper + helper 포함).

## §4 test cases (8 cases)

1. `eprintln("hello") -> void` — basic stderr emit (stdout 측 영향 0).
2. `eprintln("multi\nline") -> void` — embedded newline 정상 처리.
3. `stderr_write("raw") == 3` — bytes written 반환 (3 byte ASCII).
4. `stderr_write("한글") == 6` — UTF-8 multibyte 정상 (한글 1 char = 3 byte × 2 = 6).
5. `stderr_write("") == 0` — empty string 정상.
6. `eprintln("") -> void` — empty string + newline (1 byte stderr write).
7. caller side: `println("ok"); eprintln("warn")` — stdout 측 "ok\n" + stderr 측
   "warn\n" 가 분리 emit (test harness 가 fd 별 redirect 로 검증).
8. shell escape uneeded: `eprintln("$VAR \"quoted\" `cmd`")` 가 shell 측 expansion
   없이 literal 출력 — fork-based shell escape 우회의 부재 검증.

추가 edge: fd=2 closed (process 가 stderr close 후 호출) → `stderr_write` 가 -1
반환 + caller 가 silent (raw 12 silent-error-ban 정합으로 caller 측 escalate 부담은
caller 책임).

## §5 breaking changes — none (additive new builtin)

`core.io.eprintln` + `core.io.stderr_write` 는 신규 builtin. 기존 hexa stage1 의 어떤
builtin 도 변경 / 제거하지 않음. `println` 동작 변경 0 — stdout 측 emit 그대로 유지.

기존 user 측 hand-roll `stderr_warn` (Track AE/AB) 는 land 후에도 호환 — caller 가
신규 `eprintln` 으로 점진 migrate 가능 (migration 0 정책 정합).

## §6 alternative considered

**Option A**: 현재 shell escape 우회 (`exec("echo ... >&2")`) 유지.
**Verdict**: REJECTED. fork cost 매 warn 당 ms order — silent-error-ban 정합 path
에서 hot loop 시 누적 부담. shell injection 위험도 별도 hand-roll mitigation 필요.

**Option B**: `println` 에 channel parameter 추가 (`println(s: str, fd: int)`).
**Verdict**: REJECTED. breaking change — 기존 `println(s: str)` 호출 단일-인자
signature 의 default fd parameter 추가가 stage1 grammar 변경 필요. additive 정합 X.

**Option C**: stderr 전용 신규 builtin 2-fn (`eprintln` + `stderr_write`) — 본 제안.
**Verdict**: ADOPTED. additive / breaking change 0 / fork cost 0 / shell injection 0
/ minimal API surface (2 fn).

**Option D** (forward-looking, scope 외): `core.io.fd_write(fd: int, s: str) -> int`
generic write — `stderr_write` 는 `fd_write(2, s)` wrapper 로 collapse 가능. own 1
minimal stdlib 정합으로 본 spec 은 stderr 전용 2-fn 까지만 — fd_write generic 은
future 확장.

## §7 sibling PR submission spec

**디렉터리 layout** (sibling repo `/Users/ghost/core/hexa-lang/` 가 PR open 시):
- `proposals/rfc_015_core_io_eprintln.md` — 본 문서의 sibling repo 측 사본 (rfc_011
  ~ rfc_014 naming convention 정합 — Track AY marker `proposals_naming_convention_adaptation`
  cite).
- `src/core/io.hexa` — reference impl (§3 outline 의 land 형태) — 기존 `core.io`
  모듈에 신규 builtin 2 fn 추가.
- `tests/core/test_io_eprintln.hexa` — §4 의 8 test case.

**커밋 메시지 template**:
```
add core.io.eprintln + stderr_write — first-class stderr channel

Track AE+AB-driven proposal: stderr_warn hand-roll (2 hit) + silent-error-ban
정합 path 의 implicit warn 의존을 stdlib first-class API 로 collapse.

- eprintln(s: str) -> void / stderr_write(s: str) -> int
- additive: no breaking changes (println 동작 unchanged)
- workaround for shell-escape-based stderr emit (fork cost / shell injection 우회)
- alternative `println(s, fd)` rejected: breaking change
```

**리뷰 checklist**:
- API signatures match §2 freeze (`eprintln(s) -> void`, `stderr_write(s) -> int`).
- 8 test cases pass (incl. edge: empty / UTF-8 multibyte / fd=2 closed).
- native syscall write(2) dispatch verified (fork-based shell escape 우회 X).
- silent-error-ban (raw 12) 정합 — fd=2 실패 시 caller 측 silent 가 보장.
- 기존 `println` 동작 unchanged (regression test).

## §8 dependencies

hexa-lang internal native syscall dispatch (write(2)). 외부 C lib / FFI 0. 기존
builtin 중 변경 없음 — 신규 2-fn 추가만.

PR #47 merged (rfc_012 / rfc_013 / rfc_014) 의 land 후 본 §6.5 spec 의 actual stage1
import 가능 시점은 sibling repo 의 native syscall dispatch infrastructure (현재 `exec`
builtin 와 비교 시 separate path) 의 land timing 의존.

## §9 caveats (≥6, 11 listed)

C1. 본 문서는 proposal spec — actual hexa-lang PR 아님. sibling repo 작업은 user
    explicit approval pending. PR #47 (rfc_012 / rfc_013 / rfc_014) merged 후 §6.5
    의 sibling repo 측 RFC submission 은 별도 user 동의 필요.
C2. PR #47 merged 2026-05-04T11:48:58Z cite 는 Track BC mission directive 의
    user-provided context — sibling repo `git log` verification 은 own 1 hexa-lang
    sibling NOT modified 정합으로 본 cycle scope 외.
C3. `stderr_write` 의 fd=2 closed edge case 는 caller 측 silent-error-ban 정합 —
    본 fn 자체가 -1 반환 + caller 가 escalate 불가 (raw 12 silent-error-ban 의
    "stderr 자체 실패는 silent" 측 예외 항목).
C4. shell escape 우회의 fork cost 측정값 (ms order) 은 own 2 honest estimate —
    stage1 native syscall dispatch land 시 actual ns/op 측정은 future B-bench cycle.
    Track AV §8 측 bench harness 와 별도 — §6.5 는 본 cycle 에서 B-bench 진행 X.
C5. UTF-8 multibyte byte count 검증 (test case 4: "한글" == 6 byte) 은 hexa stage1
    의 `str_byte_len` builtin 정합 가정 — UTF-16 / UTF-32 환경에서는 byte count 다름.
C6. `eprintln` 의 newline char 가 platform-specific X (`\n` LF only) — Windows CRLF
    환경에서도 `\n` 유지. shell tool / log aggregator 측 호환은 caller 가 ANSI 처리.
C7. `println` 와의 ordering: stdout 과 stderr 가 별도 buffer — `println("a"); eprintln("b")`
    호출 순서가 actual emit 순서와 일치 X (OS / runtime buffering 의존). 순서 의존
    caller 는 explicit flush 또는 단일 channel 사용.
C8. own 5 status: §6.5 = `c_hit_2_b_bench_deferred_pr_ready_pending_user_approval`.
    Track BC marker `next_3_rfcs_status.6_5: "pr_ready_pending_user_approval"` 정착.
    C-hit 2 (Track AE + AB) / B-bench deferred (correctness 측면 — bench 우선순위
    낮음 per HEXA_UPSTREAM.md §6.5 cite).
C9. 본 spec 의 land 가 closure_orchestrator / roadmap_op / 미래 도구 의 stderr_warn
    hand-roll 을 deprecate 강제 X — migration 0 정책 정합으로 호환 layer 유지, 신규
    코드만 신규 API 사용 권장.
C10. raw 10 caveats ≥6 — 본 doc 11 caveats 로 여유.
C11. Track BC 는 doc-only — `lib/` / `tool/` / `tests/` / sibling repo / 기존 docs
     모두 read-only / modifications 0.

---

*Track BC §6.5 PR-ready proposal spec doc, written 2026-05-04. doc-only / additive /
migration 0 / destructive 0 / sibling repo modifications 0 / in_place_writes 0.*
