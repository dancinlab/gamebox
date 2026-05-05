# HEXA upstream RFCs — Phase A

> **scope**: gamebox 가 발견한 hexa-lang stage1 limitations 의 upstream proposal 모음.
> **target repo**: github.com/need-singularity/hexa-lang
> **source project**: github.com/dancinlife/airgenome-gamebox (DIY hexa Win32 emulation)
> **discovered during**: M0 R0 instrumentation + M2-M14 만점 컴포넌트 sprint + Phase 0a PE parser

## RFCs

| RFC | Title | Priority | Status |
|---|---|---|---|
| [A1](RFC_A1_float_stage1.md) | stage1 float arithmetic stabilization | CRITICAL | draft, pre-PR |
| [A2](RFC_A2_bit_ops.md) | native bit operations (& \| ^ << >>) | HIGH | draft, pre-PR |
| [A3](RFC_A3_dict_literal.md) | dict / record literal parse-vs-runtime consistency | HIGH | draft, pre-PR |

## 발견 경로

각 RFC 는 gamebox 의 다음 작업 중 발견:
- **A1 float**: airgenome `probe.hexa#assess()` 의 explicit workaround comment + gamebox 17 hexa modules 의 int-percent 우회
- **A2 bit ops**: M4 C-PSO SHBF, M6 C-SHADER-AGING APBF, std_encoding base64_decode 의 math_pow 우회
- **A3 dict literal**: M6 C-SHADER-AGING 작성 중 hexa parse 통과 / hexa run 실패 발견 (~30 줄 우회 비용)

## PR submit 절차

1. **각 RFC 검토** — `RFC_A{1,2,3}_*.md` 의 PR-ready summary 섹션 확인
2. **upstream fork** — github.com/need-singularity/hexa-lang fork
3. **branch** — `rfc/A1-float-stage1`, `rfc/A2-bit-ops`, `rfc/A3-dict-literal`
4. **commit** — 각 RFC 를 hexa-lang 의 적절한 RFC 디렉토리로 (예: `doc/rfcs/<NNN>-*.md` 또는 동등 위치)
5. **PR description** — RFC 의 PR-ready summary 섹션을 PR description 으로 사용
6. **PR labels** — `rfc`, `stage1`, `priority/critical|high`
7. **PR linkage** — gamebox issue 참조: https://github.com/dancinlife/airgenome-gamebox

## 채택 후 효과

- **A1 채택 시**: airgenome `probe.hexa#assess()` workaround 제거 가능, gamebox 17 modules 의 int-percent → float 마이그레이션
- **A2 채택 시**: 진짜 SHBF (32-int 1024-bit array) 구현 가능, 100K shader scale 에서 10-100× 빠름
- **A3 채택 시**: c_shader_aging.hexa TSV 우회 (~30 줄) → dict literal (~5 줄), c_correlate.hexa array-of-array → array-of-dict

## 후속 RFC (Phase B/C/D — 발견 누적)

본 Phase A 외 추가 후보 (docs/HEXA_UPSTREAM.md 참조):
- B1 append_file builtin
- B2 std_json encode/decode
- C1 std_regex
- C2 sleep_ms / sleep_us
- C3 stdin_lines streaming
- D1 fs_watch (FSEvents binding)
- D2 process_alive / wait_pid

각각 발견 시 추가 RFC 작성.

## 참조

- gamebox `.own` own 1: DIY hexa PE 로더 + Win32 shim, Wine 0
- gamebox `.own` own 2: log/error/emit/headless mandate
- airgenome `modules/probe.hexa` — float workaround discoverer
- airgenome `modules/filters/SHBF` — bit ops 우회 인용 (wave 1 production-validated 17.9× cold)
- hexa-lang `self/std_encoding.hexa#base64_decode` — math_pow 우회 인용

---

*written 2026-04-30, gamebox commit 0477ae1+ 기준.*
