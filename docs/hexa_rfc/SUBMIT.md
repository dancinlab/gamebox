# RFC Submit Guide — hexa-lang upstream PR

> RFC A1/A2/A3 를 github.com/dancinlab/hexa-lang 으로 제출하는 절차.

## 사전 준비

1. **github.com 계정** + Personal Access Token (gh CLI 권한)
2. **fork** github.com/dancinlab/hexa-lang
3. **gh CLI** 설치 확인: `gh auth status`

## RFC 제출 (각 RFC 동일)

### Step 1: branch 생성

```bash
cd ~/core/hexa-lang
git checkout -b rfc/A1-float-stage1   # A2, A3 도 동일 패턴
```

### Step 2: RFC 파일 복사

```bash
mkdir -p doc/rfcs
cp ~/core/airgenome-gamebox/docs/hexa_rfc/RFC_A1_float_stage1.md doc/rfcs/A1-float-stage1.md
git add doc/rfcs/A1-float-stage1.md
```

### Step 3: commit

```bash
git -c commit.gpgsign=false commit -m "RFC A1: stage1 float arithmetic stabilization

discovered by airgenome-gamebox project (DIY hexa Win32 emulation).
17 hexa modules currently use int-percent workaround due to broken float
arithmetic in stage1 interpreter. RFC proposes f64_* function builtins
(option B, immediate) → stage1 fix (option A) → stage2 native (option C).

source: github.com/dancinlife/airgenome-gamebox/docs/hexa_rfc/RFC_A1_float_stage1.md
"
```

### Step 4: push + PR

```bash
git push -u origin rfc/A1-float-stage1

gh pr create \
  --title "RFC: stage1 float arithmetic stabilization" \
  --body-file <(sed -n '/## PR-ready summary/,$p' ~/core/airgenome-gamebox/docs/hexa_rfc/RFC_A1_float_stage1.md) \
  --label "rfc,stage1,priority/critical"
```

### Step 5: PR linkage

- PR description 끝에 추가:
  - source: github.com/dancinlife/airgenome-gamebox commit 0477ae1+
  - related: gamebox closure roadmap CM-7 (HEXA Phase A)

## 3 RFC 통합 PR (옵션 — 1개로 묶기)

```bash
git checkout -b rfc/phase-A
cp ~/core/airgenome-gamebox/docs/hexa_rfc/RFC_A1*.md doc/rfcs/A1-float-stage1.md
cp ~/core/airgenome-gamebox/docs/hexa_rfc/RFC_A2*.md doc/rfcs/A2-bit-ops.md
cp ~/core/airgenome-gamebox/docs/hexa_rfc/RFC_A3*.md doc/rfcs/A3-dict-literal.md
cp ~/core/airgenome-gamebox/docs/hexa_rfc/README.md doc/rfcs/README.md
git add doc/rfcs/
git -c commit.gpgsign=false commit -m "RFC Phase A: 3 stage1 mandates (float / bit ops / dict literal)

stage1 limitations discovered during airgenome-gamebox sprint:
- A1 (CRITICAL): float arithmetic broken
- A2 (HIGH):     no native bit ops (& | ^ << >>)
- A3 (HIGH):     dict literal parse-vs-runtime inconsistency

each RFC includes PR-ready summary + motivation + migration + test plan.
"
git push -u origin rfc/phase-A
gh pr create --title "RFC Phase A: 3 stage1 mandates" --body-file doc/rfcs/README.md
```

## PR 후 후속

- PR ID 받으면 `airgenome-gamebox/docs/hexa_rfc/RFC_A{1,2,3}.md` 의 "Related upstream PRs / issues" 섹션 갱신
- airgenome-gamebox commit + push: "docs(rfc): A1/A2/A3 upstream PR linked (#XXX/XXX/XXX)"

## 자동화 스크립트 (옵션)

`scripts/submit_rfc.sh` — 위 절차 자동화. NOT in this repo (gamebox 는 own 1 hexa-only) — hexa-lang/scripts/ 안에 작성 권장.
