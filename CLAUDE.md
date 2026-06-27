# gamebox

Apple-native Win32 game runtime — run unmodified Win32 PE binaries on macOS
Apple Silicon **without Wine** (Wine 0), via D3DMetal graphics + a hexa-only
loader / perf / dispatch layer, under a strict **own1** posture (no DRM /
Warden / anti-cheat bypass, no anima clone). The PE→D3D11→Metal pipeline is
proven end-to-end with a self-authored own1 D3D11 PE (`validated_manjeom =
first_own1_authored_d3d11_pe_to_metal_frame`, an infrastructure milestone) — but
**no commercial game frame is evidenced yet**. Full design lives in
[ARCHITECTURE.md](ARCHITECTURE.md); history in [CHANGELOG.jsonl](CHANGELOG.jsonl).

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
├─ CHANGELOG.jsonl            — append-only history
├─ harness.config.json     — harness config (lockdown, lint, docs discipline)
├─ .github/workflows/      — CI (ci.yml — github-hosted macOS build+test)
└─ .harness-engine/        — dancinlab harness submodule (governance engine)
```

## Governance

- **own1 invariant** — Wine 0 / DRM bypass 0 / Warden bypass 0 / anti-cheat
  bypass 0 / anima clone 0. Detection-track modules are scaffolds only.
- **own2 honesty** — subcommands emit explicit PASS/FAIL/PARTIAL reasons;
  predecessor markers preserved verbatim.
- **Single-doc discipline** — architecture goes to `ARCHITECTURE.md` (overwrite,
  SSOT); history to `CHANGELOG.jsonl` (append); transient output to
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

## CI — cloud macOS runner, NOT local

- do: build on GitHub-hosted `macos-15` (Apple-Silicon) via `.github/workflows/ci.yml` job `build-test` (push to `main`/`master` + PR + dispatch); gamebox is PUBLIC so these macOS minutes are FREE.
- do: just `git push` then check `gh pr checks` / `gh run list` — CI installs released `hexa`, runs the 8 `self-test` files + native i386 (`i386_cpu_test`) + Metal/D3D11 substrate smokes.
- dont: use the paid Blacksmith runner (`blacksmith-6vcpu-macos-15`) — removed on owner instruction for cost (deliberate per-repo override of the `blacksmith-ci` default).
- dont: run heavy `hexa test` / full builds on the dev Mac (242+ perf modules crash it); use `hexa test` for the suite (vacuous — use `hexa run <file> self-test`); unpin from `macos-15`.

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
