# hexa-lang upstream ledger — cross-project consistency audit (Track AX)

> 본 문서는 두 sibling project 의 hexa-lang upstream candidate ledger 간 cross-reference / overlap /
> gap / canonical naming difference 를 doc-only 로 audit 한다. sibling project ledger
> (`~/core/airgenome/docs/hexa_lang_upstream_candidates.md`) 는 read-only — 본 cycle 은 절대로
> 수정하지 않음. 본 audit 의 목적은 두 ledger 가 hexa-lang sibling repo (
> `/Users/ghost/core/hexa-lang`) 로 PR 제출되기 전에 cross-project sync 가 필요한 surface 를
> 식별하는 것.

## §0 목적 / 한계

**목적**:
1. gamebox `docs/HEXA_UPSTREAM.md` (post Track AO + AV) 와 airgenome
   `docs/hexa_lang_upstream_candidates.md` (own 5 A 단계) 두 ledger 의 entry 단위 cross-reference
   matrix 작성.
2. 두 ledger 사이 overlap / gap / canonical naming difference 를 식별.
3. hexa-lang sibling repo PR coordination 시점에 cross-project sync 필요 surface 도큐 명시.

**한계 (own 1 / own 2 정합)**:
- doc-only — code / lib / tool / native / test 변경 0.
- airgenome sibling project ledger 는 **read-only**. 본 audit 는 절대 수정하지 않음.
- hexa-lang sibling repo (`/Users/ghost/core/hexa-lang`) 는 본 cycle 에서 touch 0.
- 두 ledger 의 정확한 wording / API 시그니처 / 우회 패턴 cite 는 **현 시점 ledger 내용** 한정 —
  미래 ledger 갱신은 다음 cycle 에서 별도 audit.
- gamebox `docs/HEXA_UPSTREAM.md` 자체도 read-only — 본 audit 는 새 파일에만 추가.

## §1 ledger inventory

### gamebox `docs/HEXA_UPSTREAM.md` (post Track AO + AV, 558 LOC)

| Section | Origin track | Entry count | Coverage |
|---|---|---|---|
| §1.1 - §1.10 | M0 instrumentation discovery (2026-04-30) | 10 | append_file / std_json / std_regex / sleep_us / fs_walk / bit ops / stdin / fs_watch / process / float runtime |
| §4.5.1 - §4.5.3 | M2-M14 자율 구현 발견 | 3 | break keyword / dict literal / oscillation classifier (design issue) |
| §5 | docker runtime 환경 | 1 | docker image 부재 도구 (comm/join/python3) |
| §6.1 - §6.6 | Track AO RETROFIT (2026-05-04) | 6 | x1000 fixed-point / std_json+JSONL / PE struct serializer / marker JSON path / stderr eprintln / pattern matcher (forward) |
| §8.1 - §8.3 | Track AV B-bench (2026-05-04, ref §6.1/6.2/6.3) | 0 신규 (3 escalate) | bench harness 3 + own 5 status 진전 |

**gamebox 합계 distinct entry**: 10 + 3 + 1 + 6 = **20** (§4.5.3 oscillation 은 design issue 로
hexa-lang 자체 issue 가 아니지만 ledger 등재).

### airgenome `docs/hexa_lang_upstream_candidates.md` (own 5 A 단계, 385 LOC)

| Entry | Topic | Priority |
|---|---|---|
| A1 | `core.fs.clone(src, dst)` clonefile syscall builtin | 🟡 medium |
| A2 | `core.hash.xxh64(path or bytes)` — stdlib already exists, gap = airgenome integration | 🟢 high |
| A3 | `pfs_tail_lines / pfs_tail_bytes` builtin | 🟡 medium |
| A4 | `pfs_now_ns()` 표준화 — bench harness cleanup | 🟢 low |
| A5 | `pfs_writev(path, chunks)` builtin | 🔴 low |
| A6 | `to_int_safe(s)` decimal-tolerant builtin | 🟢 done (3ea7fe69) |
| A7 | `list` O(1) append builtin (현 O(n²) concat) | 🟢 high |
| A8 | hexa wrapper internal stderr redirect 우회 | 🟡 medium → A12 통합 |
| A9 | airgenome `--mode=run-once` argv pass-through | 🟢 high (airgenome side) |
| A10 | `try / catch` syntax 또는 `exec_or` builtin | 🟢 highest |
| A11 | hexa wrapper spawn shell PATH 격리 | 🟡 medium |
| A12 | A8 강화 — stderr redirect 직접 evidence | 🟢 high |
| A13 | `pfs_readdir_sorted / pfs_mtime / pfs_now_sec` builtin | 🟡 medium |
| A14 | `json_field_str / int / float` builtin | 🟢 high |
| A15 | `core.net.unix_socket(path, payload)` builtin | 🟡 medium |

**airgenome 합계 distinct entry**: **15** (A1..A15; 15 후보 이지만 ledger 우선순위 표는 16 후보로
표기 — A8/A12 통합 기준으로 distinct 15).

### 합계 비교

| Project | Entry 수 | Discovery 출처 | own 5 진행 단계 |
|---|---|---|---|
| gamebox | 20 | M0 instrumentation + 자율 구현 + Track AO RETROFIT + Track AV B-bench | C-hit (Track AO) + B-bench (§6.1/6.2/6.3 Track AV) |
| airgenome | 15 | wave 1-4 (modules/harvest, claude, transport, process gate) + bench_site | A 단계 — own 5 C+B 측정 후 escalate |

## §2 cross-reference matrix

> 표기: gamebox §X ↔ airgenome AY. "N/A" = 매칭 entry 없음. concept 동일하지만 wording / API 시그니처
> 다른 경우 §5 canonical naming differences 에서 별도 cite.

| gamebox § | gamebox topic | airgenome match | airgenome topic | 관계 |
|---|---|---|---|---|
| §1.1 | append_file builtin | (없음) | — | gap (gamebox only) |
| §1.2 | std_json (encode 만 제안) | A14 | json_field_str/int/float builtin | overlap (concept) — naming diff (§5.1) |
| §1.3 | std_regex / pattern match | (없음) | — | gap (gamebox only) |
| §1.4 | sleep_ms / sleep_us | (없음) | — | gap (gamebox only) |
| §1.5 | fs_readdir / fs_walk / fs_stat / fs_glob | A13 | pfs_readdir_sorted / pfs_mtime / pfs_now_sec | overlap — naming diff (§5.2) |
| §1.6 | bit ops builtin (& \| ^ << >>) | (없음) | — | gap (gamebox only) |
| §1.7 | stdin streaming Iterator | (없음) | — | gap (gamebox only) |
| §1.8 | fs_watch (FSEvents/inotify) | (없음) | — | gap (gamebox only) |
| §1.9 | process management (kill -0 / wait) | (없음) | — | gap (gamebox only) |
| §1.10 | float runtime stage1 broken | (없음) | — | gap (gamebox only — but §6.1 의 prerequisite) |
| §4.5.1 | break keyword 부재 | (없음) | — | gap (gamebox only — language syntax) |
| §4.5.2 | dict / object literal 부재 | (없음) | — | gap (gamebox only — language syntax) |
| §4.5.3 | oscillation classifier (design) | (없음) | — | gap — gamebox 자체도 hexa-lang issue 아님 cite |
| §5 | docker runtime 도구 부재 | (없음) | — | gap (gamebox only — runtime image) |
| §6.1 | x1000 fixed-point / float runtime | (없음) | — | gap (gamebox only — Track AO retrofit / §1.10 의 사후 evidence) |
| §6.2 | std_json + JSONL + json_path_get | A14 | json_field_str/int/float builtin | overlap — gamebox 가 더 일반화 (§5.1 / §5.3) |
| §6.3 | PE struct serializer (struct_pack) | (없음) | — | gap (gamebox only — Win32 loader 영역) |
| §6.4 | marker JSON path read | A14 | json_field_str/int/float builtin | overlap — gamebox 의 §6.2 sub-entry, airgenome 의 A14 와 동일 surface |
| §6.5 | stderr eprintln channel | A8 + A12 | hexa wrapper stderr redirect 우회 | partial overlap — naming diff (§5.4) |
| §6.6 | pattern matcher (forward Warden) | (없음) | — | gap (gamebox only — forward-looking) |

| airgenome A | airgenome topic | gamebox match | 관계 |
|---|---|---|---|
| A1 | `core.fs.clone` clonefile | (없음) | gap (airgenome only) |
| A2 | `core.hash.xxh64` stdlib integration | (없음) | gap (airgenome only — stdlib 이미 존재) |
| A3 | `pfs_tail_lines / bytes` | (없음) | gap (airgenome only) |
| A4 | `pfs_now_ns` 표준화 | (없음) | gap (airgenome only — bench harness cleanup) |
| A5 | `pfs_writev` builtin | (없음) | gap (airgenome only) |
| A6 | `to_int_safe` decimal-tolerant | (없음) | gap (airgenome only — landed) |
| A7 | `list` O(1) append | (없음) | gap (airgenome only — interpreter perf) |
| A8 | hexa wrapper stderr redirect | §6.5 (loose) | partial overlap — naming diff |
| A9 | airgenome --mode=run-once argv | N/A | airgenome-side fix, hexa-lang 측 변경 0 |
| A10 | try/catch / exec_or builtin | (없음) | gap (airgenome only) |
| A11 | hexa wrapper PATH 격리 | (없음) | gap (airgenome only — runtime/wrapper) |
| A12 | A8 강화 stderr 직접 evidence | §6.5 (loose) | partial overlap — naming diff |
| A13 | pfs_readdir / mtime / now_sec | §1.5 | overlap — naming diff (§5.2) |
| A14 | json_field_str / int / float | §1.2 / §6.2 / §6.4 | overlap — naming diff (§5.1) |
| A15 | core.net.unix_socket | (없음) | gap (airgenome only) |

## §3 overlap entries

두 ledger 모두에서 다뤄지는 entry. concept 동일 — wording / API 시그니처는 §5 에서 cite.

### §3.1 std_json / json_field — gamebox §1.2 + §6.2 + §6.4 ↔ airgenome A14

- **gamebox §1.2** (M0 cycle): encode 중심 — `json_encode / json_decode / json_object`.
- **gamebox §6.2** (Track AO retrofit): decode + path_get 까지 확장 — `json_decode / jsonl_iter /
  json_path_get`. C-hit 21+ (Track AB+AE).
- **gamebox §6.4** (Track AO retrofit): marker JSON path read — `json_path_get` 으로 통합 권고. C-hit 1.
- **airgenome A14**: 단일 field 추출 중심 — `json_field_str / json_field_int / json_field_float`.
  jsonl-heavy filter 광범위 영향, 5+ local jq_field helper DRY 위반 cite.
- **분류**: concept overlap (둘 다 JSON 처리 builtin / stdlib). API surface 는 다름 — gamebox 가 더
  일반화 (decode + path_get + iter), airgenome 이 더 narrow (field-specific).

### §3.2 fs_readdir / pfs_readdir — gamebox §1.5 ↔ airgenome A13

- **gamebox §1.5**: `fs_readdir / fs_walk / fs_stat / fs_glob` — 4 함수 묶음, M3 D-HEAP / M9 R-DXIL /
  M10 R-SHADER-WARM 영향 cite.
- **airgenome A13**: `pfs_readdir_sorted / pfs_mtime / pfs_now_sec` — 3 함수 묶음, claude.hexa
  per-instance 3-fork 호출 chain 영향.
- **분류**: concept overlap (filesystem stdlib 확장). 함수 분할이 다름 — gamebox §1.5 의 fs_stat 가
  airgenome A13 의 pfs_mtime 을 포함, gamebox §1.5 의 fs_readdir 가 airgenome A13 의
  pfs_readdir_sorted 와 거의 동등 (sort 옵션 차이).

### §3.3 stderr / eprintln — gamebox §6.5 ↔ airgenome A8 + A12

- **gamebox §6.5** (Track AO retrofit): `core.io.eprintln(s) / stderr_write(s)` builtin 제안. C-hit 2
  (Track AE + AB hand-rolled `stderr_warn`).
- **airgenome A8 + A12**: hexa wrapper script 의 internal stderr redirect 우회 — 다른 surface
  (wrapper-level vs language-level). A12 가 직접 evidence 추가 (`/tmp/.hexa-runtime/cache_err.<ts>.tmp`).
- **분류**: partial overlap. **두 surface 가 사실 다른 layer** — gamebox §6.5 는 hexa stage1
  language-level (println 이 stdout 만 가는 issue), airgenome A8/A12 는 hexa wrapper script-level
  (자식 process stderr 가 wrapper 의 tmp file 로 격리되는 issue). hexa-lang sibling repo PR 시 두
  layer 모두 다뤄야 정합.

### overlap 합계

| 분류 | 갯수 |
|---|---|
| concept overlap (API surface 일부 다름) | 3 (§3.1 / §3.2 / §3.3) |

## §4 gap entries

한 쪽 ledger 에만 있는 entry. cross-project sync 의 첫 surface 후보.

### §4.1 gamebox-only gaps (gamebox 에 있고 airgenome 에 없음)

| § | Topic | 이유 (왜 airgenome 에 없는가) |
|---|---|---|
| §1.1 | append_file | airgenome 은 `tool/bench/bench_site3.hexa` 에서 이미 builtin 으로 land — 다른 cycle |
| §1.3 | std_regex / pattern match | airgenome 측 use case (modules/harvest) 에서 regex 사용 적음 |
| §1.4 | sleep_ms / sleep_us | airgenome 측 polling 측정 cycle 부재 |
| §1.6 | bit ops | airgenome 측 SHBF / hash / bloom filter 미사용 |
| §1.7 | stdin streaming | airgenome 측 stdin loop 미사용 |
| §1.8 | fs_watch | airgenome 측 fs_watch 측정 부재 |
| §1.9 | process management | airgenome 측 kill -0 우회 부재 (다른 surface — claude per-instance) |
| §1.10 | float runtime stage1 broken | airgenome 은 float 우회를 wave 4 to_int_safe 로 처리 (A6) |
| §4.5.1 | break keyword | airgenome 측 ledger 는 language syntax issue 등재 안 함 |
| §4.5.2 | dict / object literal | 동상 |
| §4.5.3 | oscillation classifier | gamebox 자체 design issue, airgenome 과 무관 |
| §5 | docker runtime 도구 부재 | airgenome 은 mac-local 만 측정 |
| §6.1 | x1000 fixed-point | airgenome 은 closure_pct 계산 부재 — gamebox closure_orchestrator 고유 |
| §6.3 | PE struct serializer | airgenome 은 Win32 loader 영역 미사용 — gamebox 고유 |
| §6.6 | pattern matcher (forward Warden) | airgenome 은 Warden 영역 0 |

### §4.2 airgenome-only gaps (airgenome 에 있고 gamebox 에 없음)

| A | Topic | 이유 (왜 gamebox 에 없는가) |
|---|---|---|
| A1 | core.fs.clone (clonefile) | gamebox 는 cp -c 측정 부재 — modules/harvest 가 gamebox 영역 외 |
| A2 | core.hash.xxh64 stdlib integration | gamebox 는 fingerprint() 호출 부재 — gamebox 의 hash 사용은 다른 surface (xxh64 사용 안 함) |
| A3 | pfs_tail_lines | gamebox 는 tail -2000 site 부재 — airgenome modules/harvest last_for_pid 고유 |
| A4 | pfs_now_ns 표준화 | gamebox 는 bench harness cleanup 작업 cycle 부재 |
| A5 | pfs_writev | gamebox 는 append_ring_batch 부재 |
| A6 | to_int_safe | gamebox 는 ps -axo cpu% 우회 부재 (다른 surface — closure_pct x1000) |
| A7 | list O(1) append | gamebox 는 100K dataset build 없음 — wave 1 F58 고유 |
| A9 | --mode=run-once argv | airgenome-side, hexa-lang 측 변경 0 |
| A10 | try/catch / exec_or | gamebox 는 try/catch site 부재 — airgenome 5 filter 고유 |
| A11 | hexa wrapper PATH 격리 | gamebox 는 BusyBox PATH 측정 부재 |
| A15 | core.net.unix_socket | gamebox 는 unix socket 사용 부재 — airgenome transport filter 고유 |

### gap 합계

| 분류 | 갯수 |
|---|---|
| gamebox-only (airgenome 에 없음) | 15 |
| airgenome-only (gamebox 에 없음) | 11 |
| **gap 총합** | **26** |

## §5 canonical naming differences

같은 concept, 다른 이름 / 다른 API surface. cross-project PR 시 정합 필요.

### §5.1 JSON 처리 — `std_json` (gamebox) vs `json_field_*` (airgenome)

- gamebox §1.2: `json_encode / json_decode / json_object`
- gamebox §6.2: `json_decode / jsonl_iter / json_path_get`
- airgenome A14: `json_field_str / json_field_int / json_field_float`
- **canonical 권고**: hexa-lang sibling repo 측에서 `std_json` 모듈로 통합 — `json_decode` 는 generic
  decoder, `json_field_str/int/float` 는 single-field shortcut wrapper, `json_path_get` 는 nested
  path reader. 세 layer 모두 land 시 두 ledger 의 use case 모두 cover.

### §5.2 filesystem 확장 — `fs_*` (gamebox) vs `pfs_*` (airgenome)

- gamebox §1.5: `fs_readdir / fs_walk / fs_stat / fs_glob`
- airgenome A13: `pfs_readdir_sorted / pfs_mtime / pfs_now_sec`
- airgenome A1/A3/A4/A5: `pfs_clone / pfs_tail_lines / pfs_now_ns / pfs_writev_append`
- **canonical 권고**: airgenome 측 `pfs_*` prefix 가 `portable_fs.hexa` 의 namespace prefix —
  hexa-lang sibling repo 가 이미 `stdlib/portable_fs.hexa` 를 가지므로 `pfs_*` 가 canonical 후보로 더
  유리. gamebox §1.5 의 `fs_*` 도 동일 namespace 로 alias 가능 — sibling repo PR 시 prefix 통일 필요.

### §5.3 JSONL iteration — `jsonl_iter` (gamebox) vs (airgenome 미정의)

- gamebox §6.2: `jsonl_iter(path) -> array` — 라인 단위 parsed values.
- airgenome 측: jsonl 처리는 `json_field_*` 로 line-by-line 추출 (전체 array 빌드 안 함).
- **분류**: gamebox 가 array 빌드 (memory-heavy), airgenome 이 streaming (memory-light). API
  분리가 정당 — `std_json` 안에 `json_decode_line(s) / json_field_str(line, key) / jsonl_iter(path)
  -> Iterator` 세 변종 land 권고.

### §5.4 stderr 처리 — `eprintln` (gamebox §6.5) vs `HEXA_NO_INTERNAL_REDIRECT` (airgenome A8/A12)

- gamebox §6.5: language-level — `core.io.eprintln(s) / stderr_write(s)` builtin.
- airgenome A8/A12: wrapper-level — `HEXA_NO_INTERNAL_REDIRECT=1` 환경 변수 또는 `hexa.real`
  직접 호출.
- **canonical 권고**: 두 layer 모두 land 필요. language-level builtin (gamebox) + wrapper-level
  passthrough (airgenome) 가 정합 — 한쪽만 land 하면 다른 layer 우회 잔존.

### naming diff 합계

| 분류 | 갯수 |
|---|---|
| canonical naming difference (sibling PR 시 정합 필요) | 4 (§5.1 / §5.2 / §5.3 / §5.4) |

## §6 own 5 status 비교

### gamebox own 5 status (post Track AO + AV)

- §6.1 / §6.2 / §6.3 — `c_plus_b_pr_ready_pending_user_approval` (Track AV escalated).
- §6.4 / §6.5 — `c_only_b_deferred` (Track AV scope 외).
- §6.6 — `forward_looking_c_hit_zero` (multi-year L2).
- §1.1-§1.10 / §4.5.1-§4.5.3 / §5 — `discovery_only` (M0 / 자율 구현 / docker).
- 합계: 3 C+B, 3 C-only, 1 forward, 14 discovery-only.

### airgenome own 5 status (own 5 A 단계, ledger cite)

- A6 — landed (3ea7fe69, to_int_safe).
- A1 / A2 / A3 / A5 — bench measured (bench_site*).
- A7 / A10 / A12 / A13 / A14 / A15 — C-hit multiple sites (highest A10 / A14).
- A4 / A9 — airgenome-side fix (hexa-lang 측 변경 0).
- A8 / A11 — C-hit (workaround / wrapper).
- 합계: 1 landed, 4 bench measured, ~8 C-hit multiple, 2 airgenome-side.

### 진행 단계 비교

| 측면 | gamebox | airgenome |
|---|---|---|
| ledger 시작 시점 | 2026-04-30 (M0) | wave 1 cycle |
| C-hit cite 진행 | Track AO retrofit 6 entry | A 단계 entry 마다 |
| B-bench 측정 | Track AV high-hit 3 entry | bench_site* A1-A5 |
| PR escalation | pending user approval | pending user approval |
| 양쪽 모두 PR 제출 | not yet | not yet |

## §7 PR coordination posture

두 sibling project 의 ledger 가 hexa-lang sibling repo (`/Users/ghost/core/hexa-lang`) 의 PR queue
로 합류하기 전 cross-project sync 필요 surface.

### §7.1 sync 필요 surface

1. **§5.1 JSON canonical naming**: gamebox 의 `std_json` (decode + path_get + iter) + airgenome 의
   `json_field_str/int/float` 가 같은 sibling repo 모듈 내에 통합되어야. 두 ledger 가 별도 RFC 로
   제출하면 naming conflict 가능성.
2. **§5.2 filesystem prefix**: `pfs_*` vs `fs_*` 정합 — sibling repo 의 `stdlib/portable_fs.hexa` 가
   기존 `pfs_*` 사용중이라 prefix 통일 필요.
3. **§5.4 stderr layer**: language-level builtin (gamebox §6.5) + wrapper-level passthrough
   (airgenome A8/A12) 양쪽 land. PR 두 개로 분리 가능하지만 cross-reference 필수.

### §7.2 next-cycle hooks

- **다음 sync cycle**: 두 ledger 모두 사용자 explicit PR approval 받기 전에 cross-project naming
  convention 합의 cycle 필요. 본 audit 이 그 첫 surface.
- **PR submission 순서 권고**: airgenome A6 (done, landed) 이외 항목은 모두 pending. 첫 PR 후보는
  A2 (xxh64 — sibling repo 의 stdlib 이 이미 존재) 가 가장 risk-free. 본 audit 가 제기한 §3.1 / §3.2 /
  §3.3 overlap 은 다음 cycle 의 cross-reference 와 함께 PR 작성 필요.
- **future cycle 후크**: 두 ledger 가 새 entry 추가될 때마다 본 audit 의 cross-reference matrix
  갱신. ledger 갱신은 각 project 의 own cycle 에서 — audit 는 별도 cycle (Track AX 이후 AY/AZ).

### §7.3 hexa-lang sibling repo 측 posture

- 본 audit 는 sibling repo 를 touch 하지 않음. PR queue 측 변경 0.
- gamebox Track AO marker `study_only_pr_pending_user_approval` 그대로 유지.
- airgenome ledger 의 `사용자 명시 승인 후` 정책 그대로 유지.
- 본 cycle 은 evidence 추가 한정 — PR 제출은 별도 cycle 에서 user explicit greenlight 후.

## §8 caveats

honest acknowledgment of audit 의 한계 / risk / 가정. ≥8 entry 의무 (raw 10).

1. **C1 — sibling project ledger read-only**: airgenome `docs/hexa_lang_upstream_candidates.md` 는
   본 cycle 에서 한 줄도 수정하지 않음. cross-reference 는 read-only 측정만.
2. **C2 — ledger 갱신 시 audit 무효**: 두 ledger 중 하나라도 다음 cycle 에서 entry 추가 / 삭제 / wording
   변경되면 본 audit 의 cross-reference matrix (§2 / §3 / §4) 가 부분적으로 stale. next-cycle 에서
   재audit 필요 (§9).
3. **C3 — concept overlap 판정 주관성**: §3.1 / §3.2 / §3.3 의 overlap 판정은 audit 관점 — 두 ledger
   원작자가 동의 안 할 수 있음. 본 audit 는 hexa-lang sibling repo PR coordination 관점에서 "same
   stdlib module 으로 통합 가능한가" 기준으로 판정.
4. **C4 — naming canonical 권고 권한 부재**: §5 의 canonical naming 권고는 audit 의견 — 실제
   canonical naming 은 hexa-lang sibling repo maintainer 가 결정. 본 audit 는 cross-project 정합 surface
   를 식별만 할 뿐 canonical naming 을 enforce 하지 않음.
5. **C5 — gamebox §1.x M0-cycle 발견 entry 의 stale 가능성**: M0 cycle (2026-04-30) 의 §1.x 항목은
   당시 hexa stage1 상태 cite. 그동안 hexa-lang sibling repo 에서 일부 항목이 land 했는지 본 audit 는
   확인하지 않았음 (sibling repo touch 0 정책). 예: §1.1 append_file 은 airgenome 측 ledger
   commentary ("airgenome A5 caller comment") 에서 "이미 builtin 으로 land" 라고 cite — gamebox
   §1.1 은 stale 가능성.
6. **C6 — airgenome A6 landed 상태**: airgenome ledger 가 A6 (`to_int_safe`) 를 "🟢 done (3ea7fe69)"
   로 cite. gamebox 측에서는 to_int_safe 사용 부재 — sibling repo 의 actual 상태 검증 안 함.
7. **C7 — airgenome wave 4 panic / wave 1 timeout 의 gamebox 측 부재**: airgenome 의 A6 (wave 4
   panic) / A7 (wave 1 100K timeout) 의 evidence 는 airgenome 측 측정 cycle — gamebox 에서는 동등 측정
   부재. 가능성: 같은 issue 가 gamebox 에도 잠재해 있지만 site discovery 안 됨.
8. **C8 — gamebox §6.3 (PE struct serializer) airgenome 영역 외**: gamebox §6.3 의 110+ C-hit 은 11
   loader (Winsock + WININET + SCHANNEL + KERNEL32 + USER32 + GDI32 + ADVAPI32 + NTDLL + OLE32 +
   OLEAUT32 + MSVCRT + SHELL32 + COMCTL32 + DXGI). airgenome 측은 Win32 loader 영역 0 — 본 entry 는
   gamebox-only 가 아니라 "gamebox + future cross-project user" 일 가능성.
9. **C9 — gamebox §1.10 / §6.1 의 float runtime 의존성**: §6.1 (x1000 fixed-point) 은 §1.10 (float
   runtime stage1 broken) 의 사후 evidence. §1.10 이 fix 되어야 §6.1 의 native baseline 측정 가능 —
   §8.1 Track AV bench 도 hand_rolled 만 측정 가능 cite.
10. **C10 — Track AV B-bench native estimate 의 unmeasurable 정합**: gamebox §8.1 / §8.2 / §8.3 의
    `hypothetical_native_estimate` 는 "estimated_native_baseline_unmeasurable_in_hexa_stage1" 또는
    "_arm64_extrapolation" verdict — own 2 honest 정합 명시. airgenome 측 bench (A1-A5) 는 실제 CLI
    fork 측정 (cp -c / xxh64sum / tail) — 측정 정합 더 가까움. cross-project bench 비교 시 정합 차이
    cite 필요.
11. **C11 — airgenome A9 / A11 의 hexa-lang 측 변경 부재**: A9 (--mode=run-once argv) 는 airgenome
    측 fix, A11 (hexa wrapper PATH 격리) 는 wrapper script 영역 — hexa-lang sibling repo 의 stdlib
    PR 과 별도. 본 audit 가 "PR coordination" 단어를 쓸 때 두 종류의 PR (stdlib + wrapper) 분리 cite.
12. **C12 — Track AX 본 audit 자체의 own 5 status**: 본 audit 는 doc-only / additive / migration 0 /
    destructive 0. own 5 evidence = "두 ledger 의 cross-reference matrix" — C / B 측정 아님 (audit
    의 목적이 두 ledger 의 합산 own 5 status 시각화). 본 audit 자체는 own 5 ledger 항목이 아님.
13. **C13 — sibling repo PR queue 측 stale 가능성**: hexa-lang sibling repo 의 PR queue 가 두
    ledger 외 다른 source (다른 sister project — anima/n6/nexus/hive) 의 entry 도 받을 수 있음.
    본 audit 는 두 ledger 만 cross-reference — sibling repo PR queue 전체 audit 아님.
14. **C14 — airgenome ledger 의 own 5 ban 정합**: airgenome ledger 는 "Bun% 직접 인용 금지 (own 5
    ban)" cite. 본 audit 는 airgenome ledger 의 ban 정합 검증 안 함 — read-only audit 한정.
15. **C15 — 두 ledger 의 wording 정합 시점 차이**: gamebox ledger 는 2026-04-30 (M0) + 2026-05-04
    (Track AO/AV) 시점. airgenome ledger 는 wave 1-4 시점 (정확한 날짜 ledger 본문 부재). 두 ledger
    의 시점 정합 시 본 audit 도 시점-aware 하게 cross-reference 필요.

## §9 forward-looking

미래 cycle 에서 두 ledger 가 갱신될 때 본 audit 의 후속 cycle 후크.

### §9.1 다음 audit cycle trigger

- 두 ledger 중 한쪽이라도 entry 추가 / 삭제 / wording 변경 시 → 본 audit 재실행 cycle.
- hexa-lang sibling repo 가 entry 중 하나라도 land 시 → ledger 갱신 + audit 갱신.
- user explicit PR approval 떨어지면 → audit 가 PR coordination 도큐로 전환 (canonical naming 합의
  까지 land 한 후).

### §9.2 confidence-graded cross-reference 미래 가능성

- 본 audit 는 "concept overlap / gap / naming diff" 3-tuple 분류만. 미래 cycle 에서 confidence
  grade (high / medium / low) 추가 가능 — 예: §3.1 std_json overlap 은 high confidence (둘 다
  JSON 모듈), §3.3 stderr overlap 은 medium confidence (다른 layer).

### §9.3 hexa-lang sibling repo 측 PR queue audit 후크

- 본 audit 는 두 ledger 만. sibling repo PR queue 전체 audit 는 별도 cycle (sibling repo
  read-only audit cycle). 두 ledger + sibling repo PR queue 까지 합치면 3-way audit — Track AX 의
  successor.

### §9.4 다른 sister project 의 ledger 합류 가능성

- anima / n6 / nexus / hive 에 동등 ledger 가 land 하면 N-way audit 필요. 본 audit 는 2-way (gamebox
  + airgenome) — 확장은 future cycle.

---

*written 2026-05-04. Track AX cross-project audit. doc-only / additive / migration 0 / destructive 0
/ sibling project ledger modifications 0.*
