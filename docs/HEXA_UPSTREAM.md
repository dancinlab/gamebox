# hexa-lang upstream 개선 후보

> M0 (R0 instrumentation) 구현 중 발견한 hexa-lang stage1 의 미흡한 영역.
> 각 항목 = 우리 코드의 우회 + 제안 builtin / stdlib API.
> 상위: [BRAINSTORM.md](BRAINSTORM.md), [IMPLEMENTATION_ROADMAP.md](IMPLEMENTATION_ROADMAP.md).

## 1. M0 구현 중 hit 한 우회 패턴

### 1.1 append_file (현재: write_file + cat 우회)

**현재 (r0_common.hexa)**:
```hexa
fn r0_append_line(path: str, line: str) -> void {
    let tmp = path + ".emit.tmp"
    write_file(tmp, line + "\n")
    let _ = exec("cat '" + tmp + "' >> '" + path + "' && rm -f '" + tmp + "'")
}
```

**제안 builtin**: `append_file(path: str, content: str) -> void`
- atomic append (POSIX O_APPEND)
- shell escape 위험 제거
- ~3× 빠름 (fork 없음)

### 1.2 JSON encode helper (현재: manual concat)

**현재**:
```hexa
let line = "{\"ts\":\"" + ts + "\",\"sid\":\"" + sid + "\",\"event\":\"" + event + "\""
```

**문제**: value 에 `"` 또는 `\` 들어가면 깨짐. 우리는 emit 사이트에서 통제 가정.

**제안 stdlib**: `std_json` 모듈
```hexa
fn json_encode(value: any) -> str         // map / array / scalar → JSON
fn json_decode(s: str) -> any              // JSON → 값
fn json_object(kvs: [(str, any)]) -> str   // 빠른 object 빌드
```

### 1.3 native regex / pattern match (현재: sed exec)

**현재 (r0_hud_parser.hexa)**:
```hexa
let fps = exec("printf '%s' '" + line + "' | sed -nE 's/.*FPS:[[:space:]]*([0-9.]+).*/\\1/p'").trim()
```

**문제**: line 에 `'` 들어가면 shell escape 깨짐. fork 비용.

**제안 stdlib**: `std_regex` 또는 string method 확장
```hexa
fn regex_match(s: str, pattern: str) -> bool
fn regex_capture(s: str, pattern: str) -> [str]   // 캡처 그룹 array
fn str.match(pattern: str) -> [str]               // method form
```

### 1.4 high-precision sleep (현재: `sleep N` 만)

**현재**: `exec("sleep 0.033")` — POSIX sleep(3) 가 fractional 허용하지만 plat-specific.

**제안 builtin**:
```hexa
fn sleep_ms(ms: int) -> void
fn sleep_us(us: int) -> void
```
- nanosleep(2) 직접 호출
- 30Hz / 60Hz polling 정밀도

### 1.5 readdir / fs_walk (현재: find exec)

**현재 (r0_cache_watch.hexa)**:
```hexa
let cmd = "find '" + root + "' -type f \\( -name '*.bin' -o -name '*.metal' ... \\) ..."
return exec(cmd)
```

**제안 stdlib**: `std_fs` 확장
```hexa
fn fs_readdir(path: str) -> [str]
fn fs_walk(path: str, max_depth: int) -> [str]    // 재귀
fn fs_stat(path: str) -> {size: int, mtime: int, mode: int, is_dir: bool}
fn fs_glob(pattern: str) -> [str]                  // glob 매칭
```

### 1.6 bit operations (& | ^ << >>)

**현재**: `math_pow(2, n)` 곱/나누기 우회 (std_encoding.hexa base64_decode 가 이렇게 함)

**제안 builtin**:
```hexa
fn bit_and(a: int, b: int) -> int
fn bit_or(a: int, b: int) -> int
fn bit_xor(a: int, b: int) -> int
fn bit_shl(a: int, n: int) -> int
fn bit_shr(a: int, n: int) -> int
```
또는 syntax: `a & b`, `a | b`, `a << n`.

**왜 시급**: SHBF / hash / bloom filter 구현 필수. 현재 우회는 ~10× 느림.

### 1.7 stdin streaming (현재: io_pipe 단발)

**현재 (r0_hud_parser.hexa)**: file 모드만 — stdin 루프 어려움.

**제안 stdlib**:
```hexa
fn stdin_lines() -> Iterator<str>          // EOF 까지 line 단위
fn stdin_read_until(delimiter: str) -> str // 블록 단위
```

### 1.8 fs_watch / fsevents (현재: polling)

**현재 (r0_cache_watch.hexa)**: polling loop. CPU 부담.

**제안 stdlib**: `std_fs_watch`
```hexa
fn fs_watch(paths: [str], callback: fn(event)) -> void
// event = {path, kind: "added"|"removed"|"modified", ts}
```
- macOS: FSEvents binding
- Linux: inotify binding

### 1.9 process management (현재: kill -0 exec)

**현재 (r0_heap_sample.hexa)**:
```hexa
let alive = exec("kill -0 " + pid_s + " 2>/dev/null && echo ok").trim()
```

**제안 builtin**:
```hexa
fn process_alive(pid: int) -> bool
fn process_wait(pid: int, timeout_ms: int) -> int  // exit code
fn process_children(pid: int) -> [int]              // child PIDs
```

### 1.10 float arithmetic 안정화 (stage1 broken)

**현재 (probe.hexa)**:
> "hexa-lang stage1 float 산술/비교 (`*`, `>`, `<`, `==`) 가 잘못된 결과 반환.
>  우회: to_int 변환 직후 모든 비교/산술을 int 로 수행."

**필수 fix**: float 연산이 D-list 대부분에 영향 (FPS 계산, ms 단위 timing, frag_pct 비율 등).
**제안**: stage2 native float 정렬, 또는 stage1 에서 `f64_*` 함수 패밀리.

## 2. 발견 우선순위

| # | 우선순위 | 영향 컴포넌트 | 우회 비용 |
|---|---|---|---|
| 1.10 | **CRITICAL** | M2 D-FRAMETIME / M3 D-HEAP / M5 D-FRAME / M2 D-PSO | 모든 ms/% 계산 wrong without workaround |
| 1.6 bit ops | **HIGH** | M4 D-PSO (SHBF) / M6 D-SHADER-AGING (APBF) / M9 R-DXIL | 10× 느림 |
| 1.5 fs walk | HIGH | M3 D-HEAP / M9 R-DXIL / M10 R-SHADER-WARM | fork-per-call |
| 1.2 JSON | HIGH | 모든 D / R (JSONL emit) | escape 위험 |
| 1.3 regex | MEDIUM | M2 D-FRAMETIME / R-LIST | exec fork 비용 |
| 1.1 append | MEDIUM | 모든 ring file write | per-emit fork |
| 1.4 sleep_us | MEDIUM | M3 D-HEAP 30Hz polling | timing 부정확 |
| 1.8 fs_watch | LOW | M9 R-DXIL persist | polling 가능 |
| 1.7 stdin | LOW | M2 D-FRAMETIME (HUD live mode) | file mode 우회 |
| 1.9 process | LOW | M3 D-HEAP | 한 번/sample exec |

## 3. 제안 — 단계별 PR

### Phase A (CRITICAL — sprint 1 시작 차단)
- `1.10` float arithmetic 안정화 / 또는 `f64_*` 함수 패밀리
- `1.6` bit ops builtin

### Phase B (sprint 1 끝나면 좋음)
- `1.5` fs_readdir / fs_walk / fs_stat
- `1.2` std_json (encode 만이라도)
- `1.1` append_file builtin

### Phase C (sprint 2~3)
- `1.3` std_regex
- `1.4` sleep_ms / sleep_us
- `1.7` stdin_lines

### Phase D (nice-to-have)
- `1.8` fs_watch (FSEvents binding)
- `1.9` process_alive / wait_pid

## 4. 보고 / propose 채널

hexa-lang upstream repo (per `.raw-ref`): `github.com/need-singularity/hexa-lang`.

각 Phase 별 PR 또는 RFC 문서:
- `hexa-lang/rfc/<NNN>_<title>.md` — 문제 / 우회 비용 / 제안 API
- `hexa-lang/src/builtins/...` — 구현
- `hexa-lang/self/std_<name>.hexa` — 자체 호스팅 stdlib (있는 곳에 추가)

본 리스트는 gamebox sprint 진행 중 새 우회 발견 시 갱신.

## 4.5 추가 발견 (M2-M14 자율 구현 중)

### 4.5.1 `break` keyword 부재

**발견 (M2 C-FRAMETIME 작성)**: insertion-sort inner loop 의 `j = 0 - 1` "sentinel break" 패턴이 다음 `copy[j+1]=v` 라인을 깨트림. 명시적 `done = true` flag 로 우회.

**제안**: `break` / `continue` keyword 또는 `loop { ... }` 형태 `return` 가능 syntax.

### 4.5.2 dict / object literal 부재

**발견 (M6 C-SHADER-AGING 작성)**: `let r = { "hash": h, "bucket": 0 }` 같은 dict literal 이 `hexa parse` 는 통과하지만 `hexa run` 에서 `unexpected token LBrace` 로 실패. 우회: TSV 문자열 (`"hash\t0\t..."`) + accessor functions.

**제안**:
- `{key: value, ...}` syntax 지원 (Map literal)
- 또는 record 타입 `record Heap { tex_active: int, ... }`

### 4.5.3 oscillation classifier threshold 모순

**발견 (M2 self-test 분석)**: `stddev > 1.5 × mean` (CoV>1.5) 와 spike threshold (`max ≤ 2 × P50`) 가 priority-ordered 에서 모순. CoV>1.5 인 시리즈는 거의 항상 max>2×P50 이라 spike 가 먼저 fire. **spec 자체** 의 issue — BRAINSTORM round 2 에서 "2nd-largest > 1.3 × P50" 또는 windowed Fourier 로 재정의 필요.

→ hexa-lang 자체 issue 는 아님. design issue. PSB round 2 후보.

## 5. docker runtime 환경 이슈 (별건)

hexa 가 docker 모드 (`route=docker`) 로 실행 시 image 안에 일부 Unix 도구 부재:

| 도구 | 부재 | 우회 |
|---|---|---|
| `comm` | ✗ | awk |
| `join` | ✗ | awk |
| `python3` | ⚠️ 부재 가능성 | shell-only |

→ hexa runner image 에 coreutils-extended 추가 RFC 또는 hexa stdlib 가 직접 제공.

---

*written 2026-04-30. M0 구현 중 발견 사항. 각 항목 = 우회 패턴 + 제안 API.*

---

## 6. Track AO RETROFIT — past-cycle upstream candidates (added 2026-05-04)

> Track AO 는 RETROFIT cycle. 과거 cycle (Track AG/AB/AE/J/K/O/Q/W/AH) 에서 hexa-lang upstream
> 후보가 발견되었으나 본 ledger 에 기록되지 않았던 항목들을 ADDITIVELY 추가.
>
> own 5 enforcement: evidence = C (code site hit) + B (bench measurement OR equivalent code analysis).
> Track AO 는 C-hit (code site) 만 cite. B-bench 는 entry 별 future cycle 로 deferred.
>
> directive (2026-05-04): "BG cycle 에서 hexa-lang upstream 개선 필요 발견 시 같은 cycle 에 함께 진행"
> — Track AO 는 본 directive 가 정해지기 이전 cycle 들의 retrofit 한정 doc-only land.

### 6.1 float runtime / fixed-point arithmetic

**현재 (`tool/closure_orchestrator.hexa` ~line 453-510, 517-540, 593-620, 753-790)**:
```hexa
fn parse_uint_safe_local(s: str) -> int { ... }
fn parse_progress_pct_x1000(field_value_str: str) -> int {
    // "30.34" -> 30340  (x1000 fixed-point integer 표현)
    let int_v = parse_uint_safe_local(int_part)
    let frac_v = parse_uint_safe_local(frac_part)
    return int_v * 1000 + frac_v_normalized
}
fn format_x1000_decimal(value_x1000: int) -> str { ... }
fn compute_closure_pct_x1000(contribution_sum_x1000: int) -> int { ... }
```

**Wraround 패턴**: x1000 fixed-point integer 산술 — percentage / ratio / partial-credit 계산을 수동
hand-roll. `30.34` → 내부 30340, 외부 emit 시 다시 "30.34" 로 format.

**왜 필요**: hexa stage1 에 float runtime 부재 (1.10 항목 참고). percentage 계산이 필수인 closure
orchestrator weighted threshold contribution 에서 모든 수치를 x1000 정수로 hand-encode.

**제안 API**: `core.math.fixed_point_x1000(num: int, denom: int) -> int` builtin 또는 `std_math` 모듈:
```hexa
fn int_pct_format(value_x1000: int, decimals: int) -> str   // 30340 -> "30.34"
fn int_pct_parse(s: str) -> int                              // "30.34" -> 30340
fn fixed_point_x1000(num: int, denom: int) -> int            // partial-credit 계산
```

**Track AG hit count**: Track AG (`docs/airgenome_gamebox_closure_orchestrator_weighted_threshold_awareness_landed_2026_05_04.ai.md`)
에서 13 helper fn — `parse_uint_safe_local` / `parse_progress_pct_x1000` / `format_x1000_decimal` /
`progress_source_marker_path` / `read_progress_field_from_marker` /
`checkpoint_weighted_contribution_x1000` / `per_checkpoint_contribution_x1000` /
`sum_checkpoint_contributions_x1000` / `compute_closure_pct_x1000` /
`build_weighted_breakdown_lines` / 등.

**own 5 status**: C-hit count = 13 (Track AG); B-bench = deferred (closure_orchestrator 는 doc-only
operation, perf-critical 아님 — bench escalation 은 future cycle 에서 perf 영향 측정 후 결정).

### 6.2 std_json / JSONL parser

**현재 (`tool/roadmap_op.hexa` line 131, 1605, 1739, 1765-1788)**:
```hexa
fn extract_str_after(line: str, key: str) -> str { ... }     // " "key":"VALUE" " 추출
fn extract_int_field(obj: str, key: str, default_val: int) -> int { ... }
fn checkpoint_extract_refs_body(header: str) -> str {
    // depth-aware bracket walker — JSON 파서 아님
    // in-string + escape flag 로 square bracket inside strings 처리
    ...
}
fn checkpoint_extract_aggregation_kind(header: str) -> str { ... }
fn checkpoint_extract_threshold_kind(header: str) -> str { ... }
fn checkpoint_extract_threshold_value(header: str) -> int { ... }
```

**Wraround 패턴**: JSONL header 라인 위에 depth-aware bracket walker. `extract_str_after` /
`extract_int_field` 같은 helper 가 tool 마다 재구현됨.

**왜 필요**: hexa stage1 에 JSON 파서 부재 (1.2 항목 = encode 만 제안되어 있음 — decode 는 미제안).
각 tool 이 depth-tracking + escape handling 을 매번 재구현.

**Track AB caveat C5 cite**: "checkpoint_extract_refs_body is a depth-aware bracket walker, not a
JSON parser. Same trust assumption as the rest of the tool — the header is single-line strict JSON
per mk2 convention. Adversarial inputs (e.g., square brackets inside strings) are handled via the
in-string + escape flags. Production-grade JSON parsing would require a separate library (out of
scope for own1 hexa-only)."

**제안 API**: `std_json` 모듈 (1.2 의 encode-only 제안을 decode + path-get 까지 확장):
```hexa
fn json_decode(s: str) -> any                            // JSON → 값
fn json_encode(value: any) -> str                        // 값 → JSON
fn jsonl_iter(path: str) -> array                        // 라인 단위 parsed values
fn json_path_get(value: any, dotted_path: str) -> any    // "blockers.0.status_enum_adopted"
```

**Track AB+AE hit count**: Track AB (`tool/roadmap_op.hexa` subcmd_checkpoint_status 의 bracket
walker) + Track AE (8 신규 helper: `parse_uint_safe` / `extract_int_field` /
`checkpoint_extract_aggregation_kind` / `checkpoint_extract_threshold_kind` /
`checkpoint_extract_threshold_value` / `checkpoint_ref_weight` / `stderr_warn` /
`checkpoint_aggregate_weighted_threshold`) → 합쳐서 21+ hit site.

**own 5 status**: C-hit count = 21+ (Track AB+AE bracket walker + extractor 합산); B-bench =
future (parser 가 land 하면 ~200 LOC 가 across-tools 로 대체될 수 있음 — 측정 cycle 미정).

### 6.3 PE struct serializer (Win32 synthetic struct 빌드)

**현재 (`lib/loader/pe_kernel32_core.hexa` / `pe_user32_core.hexa` / `pe_gdi32_core.hexa` /
`pe_advapi32_core.hexa` / `pe_ntdll_core.hexa` / `pe_ole32_core.hexa` / `pe_oleaut32_core.hexa` /
`pe_winsock_ws2_32.hexa` / `pe_wininet_https.hexa` / `pe_schannel_tls.hexa` / `pe_msvcrt_core.hexa`
/ `pe_shell32_core.hexa` / `pe_comctl32_core.hexa` / `pe_dxgi_core.hexa`)**:

각 모듈이 synthetic Win32 struct (SOCKADDR_IN / WSAEVENT / FILE_HANDLE / PROCESS_INFORMATION /
SYSTEMTIME / HWND / HDC / HKEY / IID / VARIANT / etc) 를 raw hex literal byte array 로 inline
hand-code.

**Wraround 패턴**: byte 단위 layout 을 module-level 에서 synthetic 으로 직접 build. layout 은
Win32 reference doc 측 cross-validate 만으로 보증.

**왜 필요**: hexa stage1 에 struct serialization stdlib 부재. C / C++ 의 `struct { ... } __attribute__((packed))`
대응 builtin 이 없어 모든 Win32 loader 가 byte array 를 hand-roll.

**제안 API**: `std_pe_struct` 모듈 또는 `core.bin.struct_pack/unpack`:
```hexa
fn struct_pack(layout: str, values: array) -> bytes
    // layout 예: "u16,u16,u32,bytes[8]"
fn struct_unpack(layout: str, raw: bytes) -> array
fn pe_section_header_synth(name: str, vsize: u32, fofs: u32) -> bytes
    // PE-specific helper — section header byte layout build
```

**Track K/O/Q/W/AH cumulative hit**: 11 신규 loader (Winsock + WININET + SCHANNEL [Track K], KERNEL32
core [Track O], USER32 + GDI32 + ADVAPI32 [Track Q], KERNEL32 extend + NTDLL + OLE32 + OLEAUT32
[Track W], MSVCRT + SHELL32 + COMCTL32 + DXGI [Track AH]) × 평균 10-15 synthetic struct site /
loader ≈ 110-165 hit site.

**own 5 status**: C-hit count = 110+ (across 11 loader); B-bench = future (synthetic byte-array
correctness 는 Win32 reference doc cross-validate 로만 보증 — bench 가 측정할 측면은 "런타임 layout
mismatch 발생률" 이므로 현 retrofit doc 단계에서는 cite 만).

### 6.4 marker JSON path read (cross-marker reference)

**현재 (`tool/closure_orchestrator.hexa` line 593)**:
```hexa
fn read_progress_field_from_marker(marker_path: str, field_path: str) -> str {
    // line-grep 기반 nested field 추출 — JSON path reader 가 없어 hand-roll
    ...
}
```

**Wraround 패턴**: cycle marker (예: win32 coverage progression marker) 의 nested field 를 line-grep
으로 추출.

**왜 필요**: hexa stage1 에 JSON path reader 부재. cross-marker reference 가 line-grep + manual
substring 으로 hand-roll.

**제안 API**: 6.2 의 `std_json` 모듈에 통합 — `json_path_get(value, "win32_coverage_progression.percent")`
하나로 covered.

**Track AG hit count**: 1 (closure_orchestrator 가 win32 coverage progression marker 의
`win32_coverage_progression.percent` 를 read).

**own 5 status**: C-hit count = 1 (Track AG); 6.2 std_json 의 sub-entry 로 통합 진행 권고. B-bench =
deferred per 6.2.

### 6.5 stderr explicit warn channel

**현재 (`tool/roadmap_op.hexa` line 1788)**:
```hexa
fn stderr_warn(msg: str) -> void {
    // stderr 쓰기 — hexa stage1 의 println 은 stdout 만 가므로 우회 필요
    ...
}
```

**Wraround 패턴**: hand-rolled `stderr_warn` 함수 — `eprintln` 또는 shell escape 사용. 각 tool 이
별도로 정의.

**왜 필요**: hexa stage1 에 first-class stderr channel API 부재. `println` 은 stdout 으로만 emit.
warn / err diag 는 stderr 로 분리해야 정상 출력과 섞이지 않음.

**제안 API**: `core.io.eprintln(s: str) -> void` builtin 또는 `core.io.stderr_write(s: str)`:
```hexa
fn eprintln(s: str) -> void
fn stderr_write(s: str) -> void
```

**Track AE hit count**: 2 (Track AE 의 `stderr_warn` + Track AB 의 유사 pattern).

**own 5 status**: C-hit count = 2 (Track AE + AB 유사 pattern); B-bench = future (stderr 분리는
correctness 측면 — bench 우선순위 낮음).

### 6.6 detection-vector pattern matcher (Warden L2 long-horizon, FORWARD-LOOKING)

**현재 (`lib/perf/c_bnet_warden_environment_compat.hexa`)**:

honest detection-vector enumeration 만 존재 — 실제 environment 측 byte-level pattern matching 코드는
0 줄. 본 항목은 "현재 우회 필요" 가 아니고, future Warden environment alignment cycle (multi-year
horizon per L2 acknowledgment) 시점에 필요해질 stdlib 후보.

**Wraround 패턴**: TSV-row format 으로 detection-vector enumeration. 실제 pattern matching 은 미구현.

**왜 필요 (forward-looking)**: 미래 real Warden environment alignment cycle 에서 byte-level pattern
matching (process tree fingerprint / syscall sequence pattern / DLL loader_data_table_entry walk)
이 필요. hexa stage1 의 regex 부재 (1.3 항목) 와 별개로 byte-level matching 도 stdlib 필요.

**제안 API** (forward-looking, 즉시 필요 X): `std_pattern` 모듈:
```hexa
fn pattern_match_byte_seq(haystack: bytes, needle: bytes) -> int
    // -1 if not found, offset if found
fn pattern_glob(haystack: str, pattern: str) -> bool
```

**Track J hit count**: 0 (Track J = honest tracking only, no pattern matching code). 본 entry 는
ledger 완전성 목적의 forward-looking 등재.

**own 5 status**: C-hit count = 0 (현재 시점); B-bench = 미정 (real Warden alignment cycle 시작 시점
에 escalate). Track AO 는 ledger 등재만 — 실제 PR 은 user explicit approval + cycle re-evaluation
후 결정.

---

## 7. Track AO retrofit 정합 — own 5 status 통합 표

| § | Entry | Discovery cycle | C-hit count | B-bench status |
|---|---|---|---|---|
| 6.1 | float runtime / x1000 fixed-point | Track AG | 13 | deferred (doc-only op) |
| 6.2 | std_json / JSONL parser | Track AB+AE | 21+ | future (~200 LOC delta) |
| 6.3 | PE struct serializer | Track K/O/Q/W/AH | 110+ | future (correctness measure) |
| 6.4 | marker JSON path read | Track AG | 1 | deferred per 6.2 |
| 6.5 | stderr explicit warn | Track AE+AB | 2 | future (correctness) |
| 6.6 | pattern matcher (forward) | Track J (none yet) | 0 | future (multi-year L2) |

**Track AO 합계**: C-hit total ≈ 147+; B-bench = 전체 deferred (per-entry future cycle 에 escalate).
hexa-lang sibling repo PR 작업은 user explicit approval pending — Track AO 는 ledger doc-only land
한정.

---

*Track AO retrofit 추가분 written 2026-05-04. doc-only / additive / migration 0 / destructive 0.*

---

## 8. Track AV B-bench results (added 2026-05-04)

> Track AV 는 Track AO 가 deferred 했던 B-bench 측정을 §6.1 / §6.2 / §6.3 high-hit entry 3개에 한정해
> 진행한 cycle. own 5 enforcement 정합: C-hit (Track AO 에서 cite 됨) + B-bench (본 §8 에서 추가) =
> entry 3개의 own 5 status 가 `c_only_b_deferred` → `c_plus_b_pr_ready_pending_user_approval` 로 진전.
>
> hexa-lang sibling repo (`/Users/ghost/core/hexa-lang`) PR 작업은 user explicit approval 가 아직 안
> 떨어졌으므로 (Track AO marker `study_only_pr_pending_user_approval`), §8 은 evidence 추가 한정 —
> 실제 PR 제출은 user explicit greenlight 필요.
>
> bench harness 위치: `tool/bench/bench_hexa_upstream_<entry>_<slug>.hexa` (3 신규 파일).
> hand_rolled variant 은 실제 wall-clock 측정 (date +%s%N / gdate %N 우선). hypothetical_native_estimate
> variant 은 hexa stage1 에 native float / std_json / struct_pack 가 아예 없어 측정 불가 — extrapolated
> baseline 으로만 cite (own 2 honest 정합).

### 8.1 §6.1 float runtime / x1000 fixed-point bench

**Bench harness**: `tool/bench/bench_hexa_upstream_6_1_x1000_fixed_point.hexa`

**Workload**: synthetic 7-checkpoint closure_pct compute (parse_progress_pct_x1000 + per-checkpoint
weighted contribution + sum + format_x1000_decimal). 10000 iterations, ns/op = wall_total / 10000.

| Variant | ns/op | LOC (impl) | Verdict |
|---|---|---|---|
| `hand_rolled` | measured (run-time) | 81 | measured (current x1000 fixed-point path) |
| `hypothetical_native_estimate` | ~250 (estimate) | 12 | estimated_native_baseline_unmeasurable_in_hexa_stage1 |

**LOC delta**: 81 → 12 = ~69 LOC drop per entry-equivalent surface (parse + format + weighted +
compute helper 합산). Across 13 hand-rolled fns (Track AG cite), estimated cumulative ~250-400 LOC
reduction if `core.math.fixed_point_x1000` + native float runtime land.

**B-bench verdict**: native baseline 추정치는 hexa stage1 broken float 산술 (§1.10) 가 fix 되어야
실측 가능. 본 bench 는 hand_rolled wall-clock 만 측정 — own 2 honest 정합으로 native variant 는
`estimated_native_baseline_unmeasurable_in_hexa_stage1` verdict 명시.

### 8.2 §6.2 std_json / JSONL parser bench

**Bench harness**: `tool/bench/bench_hexa_upstream_6_2_std_json.hexa`

**Workload**: synthetic .checkpoint JSONL header (~600 byte) 1개에 7개 cond_refs 포함. 한 pass =
extract aggregation_kind + threshold_kind + threshold_value + iter all 7 refs status. 5000
iterations.

| Variant | ns/op | LOC (impl) | Verdict |
|---|---|---|---|
| `hand_rolled` | measured (run-time) | 110 | measured (depth-aware bracket walker) |
| `hypothetical_native_estimate` | ~800 (estimate) | 6 | estimated_native_baseline_simdjson_class_arm64_extrapolation |

**LOC delta**: 110 → 6 = ~104 LOC drop per extract-pass surface. Cumulative across Track AB+AE (21+
hit site) ≈ 200+ LOC reduction if `std_json` (decode + path_get) lands.

**B-bench verdict**: native simdjson-class parser 추정치 800 ns/op 는 ~600 byte 입력 + 4 path_get
query 의 typical arm64 latency. 현재 hand_rolled 가 native estimate 보다 빠를 수도, 느릴 수도 있음
— bench 진가는 ns/op 비교가 아니라 LOC 비교 (110 → 6). own 5 evidence 로 충분.

### 8.3 §6.3 PE struct serializer bench

**Bench harness**: `tool/bench/bench_hexa_upstream_6_3_pe_struct.hexa`

**Workload**: synthetic SOCKADDR_IN 16 byte 구축 (sin_family u16 LE + sin_port u16 BE htons(443) +
sin_addr u32 BE 1.2.3.4 + sin_zero[8]). Track K pattern (cf. `lib/loader/pe_winsock_ws2_32.hexa`
line 254 expected hex `02000001BB01020304000000000000`). 5000 iterations.

| Variant | ns/op | LOC (impl) | Verdict |
|---|---|---|---|
| `hand_rolled` | measured (run-time) | 70 | measured (5 helper hex/u16/u32/zero-fill + build) |
| `hypothetical_native_estimate` | ~100 (estimate) | 3 | estimated_native_struct_pack_arm64_extrapolation |

**LOC delta**: 70 → 3 = ~67 LOC drop per struct-build surface. Across 11 loader × 평균 10-15
synthetic struct site = ~110 hand-roll site 합산 → 추정 ~700-1100 LOC reduction if
`core.bin.struct_pack` lands.

**B-bench verdict**: native struct_pack 추정 100 ns/op 는 4 store + format overhead 의 typical
arm64 latency. Hand_rolled 가 string concat 위주라 native 보다 ~5× 가까이 느릴 가능성 — 그러나
correctness 측면 (byte layout) 은 본 bench 가 self_test 의 expected hex 비교로 cross-validate.

### 8.4 own 5 status 진전 표

| § | C-hit (Track AO) | B-bench (Track AV) | own 5 status 진전 |
|---|---|---|---|
| 6.1 | 13 | 81 → 12 LOC; ns/op measured + native estimate | `c_only_b_deferred` → `c_plus_b_pr_ready_pending_user_approval` |
| 6.2 | 21+ | 110 → 6 LOC; ns/op measured + native estimate | `c_only_b_deferred` → `c_plus_b_pr_ready_pending_user_approval` |
| 6.3 | 110+ | 70 → 3 LOC; ns/op measured + native estimate | `c_only_b_deferred` → `c_plus_b_pr_ready_pending_user_approval` |
| 6.4 | 1 | (deferred — 6.2 sub-entry, follows 6.2 escalation) | unchanged: `c_only_b_deferred` |
| 6.5 | 2 | (deferred — Track AV scope = high-hit 3 entry only) | unchanged: `c_only_b_deferred` |
| 6.6 | 0 | (deferred — forward-looking, real Warden alignment cycle 필요) | unchanged: `forward_looking_c_hit_zero` |

**Track AV 합계**: 3 entry escalated (C + B); 3 entry unchanged (deferred 유지). hexa-lang sibling
repo PR 제출은 user explicit approval pending — Track AV 는 evidence 추가 한정. PR submission =
별도 cycle 에서 user greenlight + sibling repo 작업 동의 후.

---

*Track AV B-bench 추가분 written 2026-05-04. additive / migration 0 / destructive 0 / sibling repo
modifications 0.*
