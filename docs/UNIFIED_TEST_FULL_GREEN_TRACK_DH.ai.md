// docs/UNIFIED_TEST_FULL_GREEN_TRACK_DH.ai.md
# gamebox Unified Test Full-Green — 4 Module/Test Repairs (Track DH, 2026-05-09)

> Track DH on gamebox standalone repo, branch `main`. Autonomous-iter
> loop continuation post Track DG. Drove `tests/test.hexa` (unified
> harness) from 6/9 → **9/9 PASS** by repairing 4 module/test issues:
> 2 layout-difference test guards (manifest + entry shim N/A in gamebox
> standalone) + 2 module bugs (`verify` reserved-keyword rename in
> `c_oauth_pkce_state.hexa`; macOS BSD sed `\s` portability fix in
> `c_dxr_ray_coherence.hexa`). **Skeleton-tier verdict carries**;
> validated_manjeom = 0 carries.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track DG own1/own2 test infra fix).
- Predecessor (immediate): Track DG.
- Predecessor (semantic, joined cluster exhaustion): Track DF.
- Trigger: autonomous-iter loop per `feedback_autonomous_iter_loop`.

When DG landed the own1/own2 enforcement gate (3/3 PASS), iter (1)
selected `tests/test.hexa` as the next fastest-arriving target — the
broadest single-shot signal of repo health. It surfaced 3 failures
(manifest, entry, perf 1 module) which iter (2)-(4) drove to 0.

---

## §1 Four issues diagnosed + fixed

### §1.1 `tests/test.hexa` `test_manifest` — absent plugin.json treated as failure

`plugin.json` is the airgenome plugin manifest. Gamebox standalone
doesn't have this file. Fix: existence guard → N/A PASS:
```diff
- if exists != "ok" { t_fail("manifest", "plugin.json missing"); return }
+ if exists != "ok" { t_pass("manifest N/A (gamebox standalone — plugin.json only in airgenome plugin mode)"); return }
```

### §1.2 `tests/test.hexa` `test_entry` — absent entry shim treated as failure

Same root cause as DG's `test_own2.hexa entry` guard. Gamebox
standalone uses `cli/gamebox.hexa` as dispatch entry (no separate
`entry` shim file). Fix: existence guard before executable check.

### §1.3 `lib/perf/c_oauth_pkce_state.hexa` — `verify` reserved-keyword parse error

Pattern matches Track DA's `scope` rename. The hexa interpreter
reserves `verify` as a token (`Token::Verify`), colliding with the
module's `fn verify(verifier, challenge) -> bool`.

Symptom:
```
Parse error at 275:8: unexpected token Verify ('verify')
Parse error at 346:22: unexpected token Verify ('verify')
Parse error at 360:1: expected RBrace, got Eof ('')
```

Fix: rename function and all 7 invocations to `verify_pair`. The
subcmd dispatch string literal `if sub == "verify"` is preserved
(user-facing CLI subcmd surface stays the same).

### §1.4 `lib/perf/c_dxr_ray_coherence.hexa` — BSD sed `\s` non-portable

Symptom: `c_dxr_ray_coherence FAIL [ratio] expected>=900 got=200`

Test fixture: 80 rays in dominant direction + 4 rays in 4 distinct
other directions (total 84 calls, 5 distinct quantized direction
buckets). Expected coherence ratio: 80/84 ≈ 952‰.

Root cause: both regex sites used `\s` to match TAB:
```hexa
let line = exec("grep -E '^N\\s' '" + HISTOGRAM_PATH + "' ...")
let _ = exec("sed -i.bak 's/^N\\s.*$/N\\tcount/' '" + HISTOGRAM_PATH + "' ...")
```

POSIX BRE/ERE does **not** define `\s` — only GNU extensions do. On
this system: `grep` resolves to GNU (so `\s` works in the lookup
branch), but `sed -i.bak` resolves to BSD (so `\s` fails in the update
branch). Asymmetric: grep finds the existing row, sed branch executes,
but sed pattern `^28\s.*$` matches nothing on BSD (treats `\s` as
literal `s`), so the count never increments.

Result: file ends with 5 distinct direction rows, each count=1.
`coherence_ratio()` computes max=1, total=5, ratio = 1*1000/5 = 200.

Fix: replace `\s` with POSIX `[[:space:]]` in both grep and sed
patterns — works on BSD AND GNU:
```diff
- let line = exec("grep -E '^N\\s' ...")
+ let line = exec("grep -E '^N[[:space:]]' ...")
- let _ = exec("sed -i.bak 's/^N\\s.*$/N\\tcount/' ...")
+ let _ = exec("sed -i.bak 's/^N[[:space:]].*$/N\tcount/' ...")
```

After fix, observed: `coherence 952‰` (matches expected). Module test
now PASSes 5 directions / 84 rays / batch=true / emits=2.

---

## §2 Verification — 9 of 9 unified test PASS

`HEXA_SHIM_NO_DARWIN_LANDING=1 hexa run tests/test.hexa` (local mode):

| # | Test | Pre-DH | Post-DH |
|---|---|---|---|
| 1 | manifest | FAIL plugin.json missing | PASS N/A (standalone layout) |
| 2 | entry | FAIL not executable | PASS N/A (standalone layout) |
| 3 | own 1 enforcement (test_apple_only.hexa) | PASS (DG-fix) | PASS 21/21 |
| 4 | own 2 enforcement (test_own2.hexa) | PASS (DG-fix) | PASS 8/8 |
| 5 | r0 self-test (M0) | PASS 5/5 | PASS 5/5 |
| 6 | perf self-test (159 modules) | FAIL 1 module | PASS 159/159 |
| 7 | loader self-test (40 modules) | PASS 40/40 | PASS 40/40 |
| 8 | closure validation harness | PASS in-progress 67% (CM-20/CM-30) | PASS in-progress 67% (CM-20/CM-30) |
| 9 | deprecated subcommands | PASS (correct rejection) | PASS (correct rejection) |

**Aggregate**: pass=9 fail=0 (was 6/3).

---

## §3 What DH asserts and does not assert

### §3.1 What is asserted

- Unified test harness runs to clean PASS for the first time in
  gamebox standalone's life (since extraction from airgenome-gamebox).
- Two layout-difference tests now adapt correctly to the standalone
  CLI layout (no plugin.json, no entry shim — these are airgenome-mode
  only).
- One `verify` reserved-keyword parse error fixed by surgical rename
  (preserving subcmd dispatch string literal).
- One macOS BSD sed `\s` portability bug fixed by POSIX `[[:space:]]`
  substitution.
- `closure_status` text reports `in-progress: 67% (CM-20/CM-30 done)`
  unchanged from DD/DF/DG.
- Track DG's own1/own2 gate carries. Track DF's joined battle.net +
  D2R 20/20 carries.

### §3.2 What is NOT asserted

- **No real network traffic** — same as DD/DF/DG.
- **No real game frame** — `validated_manjeom = 0` carries.
- **No `closure_pct` advancement** — orchestrator carries 25/40 from DD/DF.
- **No D4 surface re-engagement** — D4 retired_intentional carries.

### §3.3 Honest sub-tier promotion

DH does not promote any sub-tier. It restores the broadest test signal
(unified harness 9/9 PASS) which had been red since extraction. No new
modules, no new features.

---

## §4 What DH delivers

| File | Op | Purpose |
|---|---|---|
| `tests/test.hexa` | edit (2 guards) | manifest + entry N/A guards for standalone layout. |
| `lib/perf/c_oauth_pkce_state.hexa` | edit (rename) | `verify` → `verify_pair` (function + 7 invocations); subcmd string literal preserved. |
| `lib/perf/c_dxr_ray_coherence.hexa` | edit (2 portability fixes) | `\s` → `[[:space:]]` in grep + sed patterns. |
| `docs/UNIFIED_TEST_FULL_GREEN_TRACK_DH.ai.md` | new | This Track DH doc. |
| `state/markers/unified_test_full_green_track_dh_landed.marker` | new | DH landed marker. |

Total: 4 hexa edits across 3 files; 2 new docs/markers.

---

## §5 carries (UNCHANGED from Track DG)

- `validated_manjeom = 0` carries.
- `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` top-tier verdict carries.
- `closure_pct_weighted = 40.00%` / `closure_pct_binary = 40%` carries.
- `cm_ratio = 18/30` carries.
- `checkpoint_reached = 3/7` carries.
- own1 invariants carry — Wine 0, DRM bypass 0, Warden bypass 0, anti-cheat bypass 0, anima clone 0.
- own2 invariants carry — every module honest-emits PASS/FAIL on completion; silent_swallow 0; PARTIAL emits explicit; RETIRED_INTENTIONAL emits explicit (where applicable).
- Joined battle.net + D2R cluster 20/20 PASS (DF) carries.
- own1/own2 enforcement gate 3/3 PASS (DG) carries.

---

## §6 next steps (post-DH, gated, NOT done in DH)

- **CQ-P5 per-surface validated-tier flips** — same gate as DD/DF/DG.
- **D4 surface re-engagement** — gated on D4-stack G-2/G-3/G-4.
- **CQ-P6+ orchestration** — requires CQ-P5 first.

DH does not advance any of the above. DH is a test infra + module
portability cleanup that lets future closure declarations cite a
clean unified test PASS.

---

## §7 references

- Track DG: `docs/OWN1_OWN2_TEST_INFRA_FIX_TRACK_DG.ai.md`
- Track DF: `docs/BATTLENET_D2R_REAL_HEXA_RUN_TRACK_DF.ai.md`
- Track DA: `docs/CQ_P4_PARTIAL_CLOSURE_REAL_HEXA_RUN_TRACK_DA.ai.md` (precedent for `scope` reserved-keyword rename)
- Memory: `feedback_autonomous_iter_loop.md` (own iter pattern), `project_track_df_loop_state_2026_05_09.md` (loop state)

---

*written 2026-05-09 (Track DH). Unified test 6/9 → 9/9 PASS.
validated_manjeom=0 carries; top-tier verdict
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0 carries.*
