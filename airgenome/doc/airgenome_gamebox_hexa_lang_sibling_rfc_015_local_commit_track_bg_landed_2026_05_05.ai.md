# Track BG — hexa-lang sibling rfc_015 stderr_eprintln local-commit landing (2026-05-05)

## §0 Purpose

Track BC 가 land 한 §6.5 PR-ready proposal spec doc (`docs/HEXA_UPSTREAM_PROPOSAL_6_5_STDERR_EPRINTLN.md`,
231 LOC) 의 sibling repo 측 actual local commit landing. user 의 2026-05-04 staged approval
carry 로 LOCAL COMMIT 만 unblock — push + PR open 은 별도 explicit confirmation pending
(memory `feedback_hexa_lang_pr_approval.md` 정합).

본 cycle 은 Track AY (PR #47, 2026-05-04T11:48:58Z merged) 의 sibling worktree 패턴을
rfc_015 단일 RFC 로 반복.

본 cycle 의 disk write 영역:

- **airgenome-gamebox** (this repo): NEW handoff doc + NEW marker — 2 files.
- **hexa-lang** (sibling, via separate worktree at `/tmp/hexa-lang-track-bg-worktree`):
  1 NEW proposal + 1 local commit on NEW branch `proposal/upstream-track-bg-rfc-015-2026-05-05`.

destructive_ops 0 / push 0 / PR open 0 / user WIP modification 0 / migration 0.

## §1 Pre-flight state (sibling repo)

`cd /Users/ghost/core/hexa-lang && git status` (pre-cycle):

- branch: `diag/orpheus-selftest-sigkill` (user WIP — preserved)
- modified: NONE this cycle (Track AY 의 user WIP `stdlib/http.hexa` / `tool/pkg/registry.tsv`
  modifications 가 본 cycle 시점엔 user 의 별도 commit 또는 stash 로 처리되어 working tree
  clean 상태 — 본 cycle pre-flight 에 modified entry 없음).
- untracked: `test/regression/array_idx_assign_repro/state/`,
  `test/regression/import_alias_repro/state/` (2 entry).

`origin/main` HEAD at fetch: `bdbd287a` (Track AY merged PR #47:
`proposal: float fixed-point + std_json + PE struct serializer (3 stdlib additions) (#47)`).

origin pre-existing branch check: `proposal/upstream-track-bg-rfc-015-2026-05-05` 미존재
(NO_EXISTING_BG_BRANCH grep negative).

## §2 Worktree creation strategy

User WIP branch 보존 mandate 정합으로 main worktree (`/Users/ghost/core/hexa-lang`) 의
`git switch` 는 금지 — `git worktree add` 로 별도 디렉터리에 새 branch checkout. user 의
branch context 가 1 byte도 영향받지 않는다 (Track AY pattern follow).

```
cd /Users/ghost/core/hexa-lang
git fetch origin
git worktree add /tmp/hexa-lang-track-bg-worktree \
    -b proposal/upstream-track-bg-rfc-015-2026-05-05 origin/main
```

worktree creation result: `Preparing worktree (new branch ...) / branch ... set up to
track 'origin/main'. / HEAD is now at bdbd287a proposal: float fixed-point + std_json +
PE struct serializer (3 stdlib additions) (#47)`.

## §3 Naming convention adaptation

`/Users/ghost/core/hexa-lang/proposals/` 의 latest pre-existing 는 `rfc_014_core_bin_struct_pack.md`
(Track AY merged PR #47 의 land 결과). Track BG 는 `rfc_015` 로 number 이어 land.

| gamebox source                                                  | hexa-lang sibling target                              |
| --------------------------------------------------------------- | ----------------------------------------------------- |
| `docs/HEXA_UPSTREAM_PROPOSAL_6_5_STDERR_EPRINTLN.md` (231 LOC) | `proposals/rfc_015_core_io_eprintln.md` (232 LOC)     |

content 는 source 의 §0-§9 구조 (problem statement / API freeze / impl outline / 8 test
cases / breaking changes / alternatives / submission spec / dependencies / 11 caveats) 를
RFC 형식 header (Status / Date / Severity / Priority / Source / Family / Discovery
pipeline) 로 wrap. rfc_012/_013/_014 의 동일 구조 정합.

## §4 Local commit

**branch**: `proposal/upstream-track-bg-rfc-015-2026-05-05` (forked from `origin/main` bdbd287a).

**commit SHA**: `e1fd4b7a48a704ae0f6df650102a55bb0db1e0c1`.

**commit summary line**: `proposal: core.io.eprintln + stderr_write (stderr explicit channel)`.

**files changed**:

- `proposals/rfc_015_core_io_eprintln.md` (new, 232 LOC)

**diff stats** (per `git commit` output): `1 file changed, 232 insertions(+)`.

## §5 Sibling proposal SHA256 (verification anchor)

worktree path `/tmp/hexa-lang-track-bg-worktree`:

| RFC | path | sha256 |
| --- | --- | --- |
| rfc_015 | `proposals/rfc_015_core_io_eprintln.md` | `c180a830be27a2e82d5f41ae21d09698fe20adec0428067bc668746e2efc5376` |

## §6 Push + PR deferral spec

**push_status**: `pending_user_explicit_confirmation`
**pr_open_status**: `pending_user_explicit_confirmation`

User 의 staged approval (2026-05-04 carry) 은 LOCAL COMMIT 만 unblock. push 와 `gh pr create`
는 각각 별도 explicit 확인이 필요 (memory `feedback_hexa_lang_pr_approval.md` 정합).

verify (post-cycle): `git ls-remote origin proposal/upstream-track-bg-rfc-015-2026-05-05`
→ empty stdout (브랜치 origin 에 미존재 = no push).

## §7 Future cycle workflow (after user explicit push approval)

1. user 가 explicit 하게 "push 해" / "PR 열어" 류 명시 직접 지시.
2. `cd /tmp/hexa-lang-track-bg-worktree`
3. `git push origin proposal/upstream-track-bg-rfc-015-2026-05-05 -u`
4. `gh pr create --base main --head proposal/upstream-track-bg-rfc-015-2026-05-05 \
   --title "proposal: core.io.eprintln + stderr_write (1 stdlib addition)"`
   - body: link gamebox source spec doc (§6.5) + Track BG commit + cross-ref PR #47
     merged 2026-05-04T11:48:58Z (rfc_012/_013/_014).
5. PR review 는 hexa-lang reviewer 의 own decision — gamebox cycle 은 here-end.
6. PR close (merge or reject) 후: `cd /Users/ghost/core/hexa-lang && git worktree remove
   /tmp/hexa-lang-track-bg-worktree`.
7. 이후 marker / handoff 는 별도 cycle 에서 land.

## §8 User WIP protection verification

post-cycle `cd /Users/ghost/core/hexa-lang && git status`:

- branch: `diag/orpheus-selftest-sigkill` (UNCHANGED)
- modified: NONE this cycle (working tree clean for tracked files)
- untracked: `test/regression/array_idx_assign_repro/state/`,
  `test/regression/import_alias_repro/state/` (UNCHANGED list)

→ user WIP modifications by Track BG: **0**.

main worktree `proposals/rfc_011_exec_date_to_native_time_migration.md` SHA256 verified
unchanged pre/post cycle: `8925ef9782f048c6a53a6a544d3249219cf8ba8dd25ad4d0da15b858e8a74e00`
(spot-check for proposals/ disk integrity).

## §9 Files landed in airgenome-gamebox (this repo)

본 cycle 의 gamebox-측 disk write 영역:

| file | kind | LOC |
| --- | --- | --- |
| `airgenome/doc/airgenome_gamebox_hexa_lang_sibling_rfc_015_local_commit_track_bg_landed_2026_05_05.ai.md` | new | (this doc) |
| `state/markers/airgenome_gamebox_hexa_lang_sibling_rfc_015_local_commit_track_bg_landed.marker` | new | json |

기타 gamebox 파일 수정 0. 기존 docs/ HEXA_UPSTREAM*.md 영향 0. lib/ tool/ tests/ 영향 0.
airgenome sibling project (`/Users/ghost/core/airgenome`) 영향 0.

## §10 Caveats (≥6, 11 listed)

C1. Track BG 는 sibling repo `proposals/` 에 1 RFC 를 추가하는 LOCAL COMMIT 까지만. push +
    PR open 은 별도 cycle (user explicit confirmation 필요).
C2. naming convention 적응: gamebox source `HEXA_UPSTREAM_PROPOSAL_6_5_STDERR_EPRINTLN.md`
    → sibling `rfc_015_core_io_eprintln.md`. content 는 §0-§9 + 11 C-caveat 보존,
    header 만 RFC 형식으로 wrap.
C3. worktree path `/tmp/hexa-lang-track-bg-worktree` 는 reboot 시 휘발 가능 (`/tmp/` 는
    macOS 에서 reboot 또는 30일 미사용 시 cleanup) — push 전 reboot 발생 시 worktree 재생성
    필요 (commit SHA `e1fd4b7a...` 는 sibling repo `.git/` 에 보존되어 있어 재현 가능).
C4. Track BG 는 Track BC §6.5 PR-ready spec 의 sibling 측 land — Track BC §6.4 RETIRED
    (rfc_013 json_path_get 에 subsume) 와 Track BC §6.6 DEFERRED (pattern_matcher
    forward-looking, C-hit 0) 는 본 cycle scope 외.
C5. RFC 015 의 `Source` field 는 airgenome-gamebox 의 Track AE / AB 를 cite — sibling
    repo reviewer 입장에서 외부 repo cite 는 review burden 이지만 own2 honest evidence
    정합으로 명시 (reviewer 가 evidence 검증 필요 시 gamebox 측 spec doc + roadmap_op.hexa
    line 1788 stderr_warn fn 을 read-only 로 access).
C6. RFC 의 §2 API signature 는 freeze 형식이지만 sibling repo 의 actual stage1 syntax
    (예: `fn` 키워드 / type annotation / native_syscall_write builtin 명) 정합은 reference
    impl land cycle 에서 검증 — 본 RFC 는 spec only (impl land 는 future cycle).
C7. RFC 015 의 `core.io` 네이밍은 hexa-lang convention — sibling repo 의 actual convention
    (예: `std_io` vs `core.io`) 은 PR review 시 reviewer cross-check 필요. fallback:
    `core.io.eprintln` → `std_io.eprintln` rename 가능 (reviewer 협의).
C8. native_syscall_write builtin 가 hexa-lang stage1 에 존재하지 않을 수 있음 — `exec`
    builtin 외 별도 syscall dispatch infrastructure 의 land 가 prerequisite. reference
    impl land cycle 에서 dependency 별도 검증.
C9. local commit SHA `e1fd4b7a48a704ae0f6df650102a55bb0db1e0c1` 는 sibling repo `.git/`
    (path `/Users/ghost/core/hexa-lang/.git/`) 의 object 로 보존 — main worktree 의
    `git log proposal/upstream-track-bg-rfc-015-2026-05-05` 로 확인 가능. push 없이도
    sibling repo `.git/` 자체에 commit 이 보존되어 worktree 삭제 후에도 commit reuse 가능.
C10. user WIP branch `diag/orpheus-selftest-sigkill` 의 untracked 2 entry
     (`test/regression/*/state/`) 도 modification 0 — 본 cycle 은 main worktree 의
     디스크 영역 read-only. (Track AY 시점의 stdlib/http.hexa / tool/pkg/registry.tsv
     modified 상태가 본 cycle 시점엔 working tree 에 없음 — user 가 별도 처리.)
C11. Track BG handoff + marker LOC 는 250 LOC 이하 target — 본 doc 은 ~150 LOC, marker 는
     ~110 LOC json. 합산 ~260 LOC 으로 cycle scope 적정.

---

*Track BG sibling RFC land, 2026-05-05. gamebox-side: 2 NEW files (handoff + marker).
hexa-lang sibling: 1 NEW proposal + 1 local commit on NEW branch
`proposal/upstream-track-bg-rfc-015-2026-05-05` (worktree at
`/tmp/hexa-lang-track-bg-worktree`). push + PR open: pending_user_explicit_confirmation.
user WIP modifications: 0.*
