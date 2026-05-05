# Track AY — hexa-lang sibling PR local-commit landing (2026-05-04)

## §0 Purpose

Track AW 가 land 한 3 PR-ready proposal spec doc (`docs/HEXA_UPSTREAM_PROPOSAL_6_1_FLOAT_FIXED_POINT.md`,
`_6_2_STD_JSON.md`, `_6_3_PE_STRUCT.md`) 의 sibling repo 측 actual local commit landing.
user 승인 (2026-05-04 "승인") 으로 LOCAL COMMIT 만 unblock — push + PR open 은 별도 explicit
confirmation pending (memory `feedback_hexa_lang_pr_approval.md` 정합).

본 cycle 의 disk write 영역:

- **airgenome-gamebox** (this repo): NEW handoff doc + NEW marker — 2 files.
- **hexa-lang** (sibling, via separate worktree at `/tmp/hexa-lang-track-ay-worktree`):
  3 NEW proposals + 1 local commit on NEW branch `proposal/upstream-track-ao-aw-2026-05-04`.

destructive_ops 0 / push 0 / PR open 0 / user WIP modification 0 / migration 0.

## §1 Pre-flight state (sibling repo)

`cd /Users/ghost/core/hexa-lang && git status` (pre-cycle):

- branch: `diag/orpheus-selftest-sigkill` (user WIP — preserved)
- uncommitted modified: `stdlib/http.hexa`, `tool/pkg/registry.tsv`
- untracked: `stdlib/hf_hub.hexa`, `stdlib/ieee754.hexa`, `stdlib/sentencepiece.hexa`,
  `test/regression/array_idx_assign_repro/state/`, `test/regression/import_alias_repro/state/`

Pre-cycle SHA256 of user WIP files:

- `stdlib/http.hexa` → `cd14caec9ed060285c9d4447b861b09ce51af7d155d0e767c2648eb81a5fe852`
- `tool/pkg/registry.tsv` → `588064a984af89c11607daf35eb8c8b8b37e51d6eb8a3493e98e1838609de1b6`

## §2 Worktree creation strategy

User WIP 보존 mandate 정합으로 main worktree (`/Users/ghost/core/hexa-lang`) 의 `git switch`
는 금지 — `git worktree add` 로 별도 디렉터리에 새 branch checkout. 이렇게 하면 user 의
uncommitted 변경 / branch context 가 1 byte도 영향받지 않는다.

```
cd /Users/ghost/core/hexa-lang
git fetch origin
git worktree add /tmp/hexa-lang-track-ay-worktree -b proposal/upstream-track-ao-aw-2026-05-04 origin/main
```

origin/main HEAD: `49683560aeac9f8f2ac6aacc8d0f43b0fe1881cc` (`merge: FIX-2 IEEE-754 reinterpret
builtins unblocked (SAFE-1, 22/22 selftest PASS)`).

## §3 Naming convention adaptation

`/Users/ghost/core/hexa-lang/proposals/` 의 기존 RFC 들은 `rfc_NNN_<short_name>.md` 형식
(latest pre-existing: `rfc_011_exec_date_to_native_time_migration.md`). gamebox 측 source
spec doc 의 `HEXA_UPSTREAM_PROPOSAL_6_1_FLOAT_FIXED_POINT.md` 류 naming 은 sibling 에 적합하지
않으므로 hexa-lang 측 convention 을 따라 `rfc_012` / `rfc_013` / `rfc_014` 로 land.

| gamebox source                                          | hexa-lang sibling target                              |
| ------------------------------------------------------- | ----------------------------------------------------- |
| `docs/HEXA_UPSTREAM_PROPOSAL_6_1_FLOAT_FIXED_POINT.md` | `proposals/rfc_012_core_math_fixed_point.md`         |
| `docs/HEXA_UPSTREAM_PROPOSAL_6_2_STD_JSON.md`          | `proposals/rfc_013_std_json_decode_path.md`          |
| `docs/HEXA_UPSTREAM_PROPOSAL_6_3_PE_STRUCT.md`         | `proposals/rfc_014_core_bin_struct_pack.md`          |

content 는 source 의 §0-§9 구조 (problem statement / API freeze / impl outline / 12 test cases /
breaking changes / alternatives / submission spec / dependencies / 11 caveats) 를 RFC 형식
header (Status / Date / Severity / Priority / Source / Family / Discovery pipeline) 로 wrap.

## §4 Local commit

**branch**: `proposal/upstream-track-ao-aw-2026-05-04` (forked from `origin/main` 49683560).

**commit SHA**: `14218c0b2e915f1f44a03ee34932757ee2a54d25`.

**commit summary line**: `proposal: float fixed-point + std_json + PE struct serializer (3 stdlib additions)`.

**files changed**:

- `proposals/rfc_012_core_math_fixed_point.md` (new, 158 LOC)
- `proposals/rfc_013_std_json_decode_path.md` (new, 175 LOC)
- `proposals/rfc_014_core_bin_struct_pack.md` (new, 197 LOC)

**diff stats** (per `git commit` output): `3 files changed, 646 insertions(+)`.

## §5 Sibling proposals SHA256 (verification anchor)

worktree path `/tmp/hexa-lang-track-ay-worktree`:

| RFC | path | sha256 |
| --- | --- | --- |
| rfc_012 | `proposals/rfc_012_core_math_fixed_point.md` | `a09287793e1ba94677ed5c9e667378e646ce64cf343ed4071ceec713d5ddfce8` |
| rfc_013 | `proposals/rfc_013_std_json_decode_path.md` | `909f999535cb098d5114772908c433b18137f22746f7306a6dc9e3c825de115c` |
| rfc_014 | `proposals/rfc_014_core_bin_struct_pack.md` | `4922d24d26bed9d5a18c89fabdf12d26c5918baa8088d514c2e7bac663b72a46` |

## §6 Push + PR deferral spec

**push_status**: `pending_user_explicit_confirmation`
**pr_open_status**: `pending_user_explicit_confirmation`

User 의 2026-05-04 "승인" 은 LOCAL COMMIT 만 unblock 한 staged approval. push 와 `gh pr create`
는 각각 별도 explicit 확인이 필요 (memory `feedback_hexa_lang_pr_approval.md` 정합 — 한국어
"승인" 단어 단독으로는 push 까지 갈 수 없다는 historical guidance).

verify (post-cycle): `cd /tmp/hexa-lang-track-ay-worktree && git log origin/proposal/upstream-track-ao-aw-2026-05-04`
→ `fatal: ambiguous argument ... unknown revision` (브랜치 origin 에 미존재 = no push).

## §7 Future cycle workflow (after user explicit push approval)

1. user 가 explicit 하게 "push 해" / "PR 열어" 류 명시 직접 지시.
2. `cd /tmp/hexa-lang-track-ay-worktree`
3. `git push origin proposal/upstream-track-ao-aw-2026-05-04 -u`
4. `gh pr create --base main --head proposal/upstream-track-ao-aw-2026-05-04 --title "proposal: 3 stdlib additions (float fixed-point + std_json + core.bin)"`
   - body: link gamebox source spec doc + Track AY commit history + cross-project audit (Track AX) note re: airgenome A14 overlap.
5. PR review 는 hexa-lang reviewer 의 own decision — gamebox cycle 은 here-end.
6. PR close (merge or reject) 후: `cd /Users/ghost/core/hexa-lang && git worktree remove /tmp/hexa-lang-track-ay-worktree`.
7. 이후 marker / handoff 는 별도 cycle 에서 land.

## §8 User WIP protection verification

post-cycle SHA256 check:

```
cd /Users/ghost/core/hexa-lang
shasum -a 256 stdlib/http.hexa tool/pkg/registry.tsv
```

result (post-cycle, identical to pre-cycle):

- `stdlib/http.hexa` → `cd14caec9ed060285c9d4447b861b09ce51af7d155d0e767c2648eb81a5fe852` (UNCHANGED)
- `tool/pkg/registry.tsv` → `588064a984af89c11607daf35eb8c8b8b37e51d6eb8a3493e98e1838609de1b6` (UNCHANGED)

`cd /Users/ghost/core/hexa-lang && git status` (post-cycle):

- branch: `diag/orpheus-selftest-sigkill` (UNCHANGED)
- modified: `stdlib/http.hexa`, `tool/pkg/registry.tsv` (UNCHANGED list)
- untracked: 동일 5 entry 그대로 유지

→ user WIP modifications by Track AY: **0**.

## §9 Files landed in airgenome-gamebox (this repo)

본 cycle 의 gamebox-측 disk write 영역:

| file | kind | LOC |
| --- | --- | --- |
| `airgenome/doc/airgenome_gamebox_hexa_lang_sibling_pr_local_commit_track_ay_landed_2026_05_04.ai.md` | new | (this doc) |
| `state/markers/airgenome_gamebox_hexa_lang_sibling_pr_local_commit_track_ay_landed.marker` | new | json |

기타 gamebox 파일 수정 0. 기존 docs/ HEXA_UPSTREAM*.md 영향 0. lib/ tool/ tests/ 영향 0.

## §10 Caveats (≥8, 11 listed)

C1. Track AY 는 sibling repo `proposals/` 에 3 RFC 를 추가하는 LOCAL COMMIT 까지만. push +
    PR open 은 별도 cycle (user explicit confirmation 필요).
C2. naming convention 적응: gamebox source `HEXA_UPSTREAM_PROPOSAL_6_*.md` → sibling
    `rfc_012/013/014_*.md`. content 는 §0-§9 + 11 C-caveat 보존, header 만 RFC 형식으로 wrap.
C3. worktree path `/tmp/hexa-lang-track-ay-worktree` 는 reboot 시 휘발 가능 (`/tmp/` 는 macOS
    에서 reboot 또는 30일 미사용 시 cleanup) — push 전 reboot 발생 시 worktree 재생성 필요
    (commit SHA `14218c0b...` 는 sibling repo `.git/` 에 보존되어 있어 재현 가능).
C4. Track AX 의 airgenome A14 overlap 진단을 RFC 013 (std_json) §9 에 informational note 로
    삽입 — reviewer 가 PR review 시 cross-check 권장. airgenome 측 ledger 자체는 본 cycle
    수정 0 (out-of-scope).
C5. RFC 012/013/014 의 `Source` field 는 airgenome-gamebox 의 Track AG / AB / AE / K / O /
    Q / W / AH / AM 을 cite — sibling repo reviewer 입장에서 외부 repo cite 는 review burden
    이지만 own2 honest evidence 정합으로 명시 (reviewer 가 evidence 검증 필요 시 gamebox 측
    spec doc + B-bench harness 를 read-only 로 access).
C6. RFC 의 §2 API signature 는 freeze 형식이지만 sibling repo 의 actual stage1 syntax (예:
    `fn` 키워드 / type annotation / `import` syntax) 정합은 reference impl land cycle 에서
    검증 — 본 RFC 는 spec only (impl land 는 future cycle).
C7. RFC 014 의 `pe_section_header_synth` 는 PE-specific helper — pure stdlib 가 platform-
    specific symbol 을 host 하는 것에 대해 reviewer 가 거부 가능. fallback: `pe_section_header_synth`
    를 `core.bin` 에서 `core.bin.pe` 또는 별도 `std_pe` 모듈로 분리 (PR review 시 협의).
C8. RFC 013 의 `jsonl_iter` 가 in-memory 만 — streaming variant (`jsonl_stream`) 는 future
    확장 으로 명시. large-file (>수 GB) 사용은 본 RFC scope 외 (caller 가 line-by-line read).
C9. local commit SHA `14218c0b...` 는 sibling repo `.git/` (path `/Users/ghost/core/hexa-lang/.git/`)
    의 object 로 보존 — main worktree 의 `git log proposal/upstream-track-ao-aw-2026-05-04`
    로 확인 가능. push 없이도 sibling repo `.git/` 자체에 commit 이 보존되어 있어 worktree
    삭제 후에도 commit reuse 가능.
C10. user WIP branch `diag/orpheus-selftest-sigkill` 의 untracked 5 entry (`stdlib/hf_hub.hexa`
     등) 도 modification 0 — 본 cycle 은 main worktree 의 디스크 영역 read-only.
C11. Track AY handoff + marker LOC 는 250 LOC 이하 target — 본 doc 은 ~190 LOC, marker 는
     ~75 LOC json. 합산 ~265 LOC 으로 cycle scope 적정.

---

*Track AY sibling PR local commit landing, 2026-05-04. gamebox-side: 2 NEW files
(handoff + marker). hexa-lang sibling: 3 NEW proposals + 1 local commit on NEW branch
`proposal/upstream-track-ao-aw-2026-05-04` (worktree at `/tmp/hexa-lang-track-ay-worktree`).
push + PR open: pending_user_explicit_confirmation. user WIP modifications: 0.*
