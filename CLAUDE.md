# gamebox

Apple-native Win32 game runtime — run unmodified Win32 PE binaries on macOS
Apple Silicon **without Wine** (Wine 0), via D3DMetal graphics + a hexa-only
loader / perf / dispatch layer, under a strict **own1** posture (no DRM /
Warden / anti-cheat bypass, no anima clone). Skeleton-tier: modules compile and
probe, but no real game frames are evidenced (`validated_manjeom = 0`). Full
design lives in [ARCHITECTURE.md](ARCHITECTURE.md); history in
[CHANGELOG.md](CHANGELOG.md).

## Structure

```
gamebox/
├─ cli/gamebox.hexa        — CLI entry (status / list / probe / launch / selftest; own2 PASS/FAIL/PARTIAL emit)
├─ lib/                    — runtime
│  ├─ detect.hexa          — game detection
│  ├─ dispatch.hexa        — subcommand dispatcher
│  ├─ loader/              — PE *_core + calling-conv + IAT/DLL bind + Battle.net + DX bridge + launchers
│  └─ perf/                — 242+ c_*_*.hexa perf modules (104 families, 18+ at 100% skeleton manjeom)
├─ native/                 — Apple-native helper shims
├─ tool/                   — roadmap_op.hexa + closure_orchestrator.hexa
├─ tests/                  — hexa selftest
├─ docs/                   — roadmaps, forensic freezes, per-game READMEs
├─ airgenome/              — predecessor session artifacts (own2 traceability)
├─ gamebox/                — native lifecycle session artifacts
├─ state/                  — markers + telemetry
├─ .roadmap.*              — per-game / launcher / loader SSOT (14 active + 1 historical)
├─ .checkpoint.*           — cross-roadmap forensic checkpoint snapshots
├─ hexa.toml               — hx manifest (name, version, entry)
├─ ARCHITECTURE.md         — architecture SSOT (update-in-place)
├─ CHANGELOG.md            — append-only history
├─ harness.config.json     — harness config (lockdown, lint, docs discipline)
├─ .github/workflows/      — CI (ci.yml — Blacksmith macOS build+test)
└─ .harness-engine/        — dancinlab harness submodule (governance engine)
```

## Governance

- **own1 invariant** — Wine 0 / DRM bypass 0 / Warden bypass 0 / anti-cheat
  bypass 0 / anima clone 0. Detection-track modules are scaffolds only.
- **own2 honesty** — subcommands emit explicit PASS/FAIL/PARTIAL reasons;
  predecessor markers preserved verbatim.
- **Single-doc discipline** — architecture goes to `ARCHITECTURE.md` (overwrite,
  SSOT); history to `CHANGELOG.md` (append); transient output to
  `scripts/scratch/`. Don't scatter `*-report.md` / dated notes at the repo root.
- **Protected branches** — `main` / `master`. Open a PR from a feature branch.

## Harness

This repo is governed by the dancinlab harness (`.harness-engine/` submodule,
profile `hardcore`). Hooks in `.claude/settings.json` delegate to the engine on
every Bash / Write / Edit / prompt, guarded by
`[ -x .harness-engine/bin/harness ] && … || true` so the repo stays usable even
without the engine checked out.

Run via `bash .harness-engine/bin/harness <cmd>` (or, with an external tsx,
`HARNESS_REPO_ROOT="$PWD" <tsx> .harness-engine/cli/index.ts <cmd>`).

## CI — builds run on Blacksmith, NOT locally

gamebox is Apple-native (Win32 PE + D3DMetal), so its hexa compile + the
apple-only test suite need a macOS host. Running that locally (242+ perf
modules + 8 test files) spikes memory and **crashes the dev Mac** — so CI runs
on a Blacksmith cloud Apple-Silicon runner instead.

- **Workflow** — `.github/workflows/ci.yml`, job `build-test`, on every
  push to `main`/`master` + every PR + `workflow_dispatch`.
- **Runner** — `blacksmith-6vcpu-macos-15` (native Apple-Silicon cloud Mac).
  Pinned to `macos-15` (NOT `-26`) — same rationale as hexa-lang `release.yml`:
  the Mach-O min-OS baseline rides the builder SDK, so the pin keeps it stable.
  same-org (`dancinlab`) Blacksmith is already enabled (hexa-lang uses it), so
  no extra activation is needed.
- **Toolchain** — the released `hexa` binary, installed via the canonical
  one-liner (`install.sh` → `~/.hx/bin`). gamebox is a downstream consumer, not
  the compiler repo, so it installs the release — it does not build hexa.
- **What it runs** — `hexa test` over the 8 `[test].files` in `hexa.toml`
  (own2 PASS/FAIL honest), then a gamebox CLI smoke (`status` / `selftest
  --quick`, warn-only since D3DMetal SDK is absent on the runner).

**Rule for working here**: don't run heavy `hexa test` / builds on the local
Mac — just `git push` and let Blacksmith build. The dev box should only ever
push. Check results with `gh pr checks` / `gh run list`.

## Quick reference

| Command | Purpose |
|---|---|
| `harness verify` | Run configured checks (wraps `hexa verify`) |
| `harness docs check` | Single-doc discipline gate (architecture SSOT + quickref) |
| `harness lint` | Staged-L0 + freshness + convergence checks |
| `harness audit` | 6-axis self-scorecard |
| `harness folders scan` | Per-subfolder CLAUDE.md coverage |
| `harness update --hooks` | Bump engine submodule + refresh hooks |
| `gamebox status` | Package version + forensic verdict + inventory |
| `gamebox selftest [--quick]` | Self-validation (own2 PASS/FAIL emit) |
