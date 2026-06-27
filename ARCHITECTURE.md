# gamebox — Architecture (SSOT · update-in-place)

> Single source of truth for the **final** architecture (component-level prose).
> Update (overwrite) this file in place when the design changes — it is **not**
> append-only. History and per-change decisions live in
> [CHANGELOG.jsonl](CHANGELOG.jsonl). The **domain tree** (per-subsystem / per-game
> goals + milestones) is `ARCHITECTURE.json` (tree SSOT, human viewer
> `ARCHITECTURE.html` via `python3 serve.py`) — it retired the former
> `DOMAINS.tape` + `domains/` snapshots on 2026-06-18.

## Overview

`gamebox` is an **Apple-native Win32 game runtime** — it aims to run unmodified
Win32 PE binaries on macOS Apple Silicon **without Wine** (Wine 0), using
D3DMetal for graphics, [hexa-lang](https://github.com/dancinlab/hexa-lang) for
the loader / perf / dispatch layer, and a strict **own1** posture: no DRM
bypass, no Warden bypass, no anti-cheat bypass, no anima clone.

The runtime is skeleton-tier: every loader and perf module is present and
individually compiles / probes, but **no real game frames have been evidenced**
(`validated_manjeom = 0`). The honest forensic verdict is
`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`. See
[README.md](README.md#caveats-raw10-c3) for the full honesty disclosure.

## Component map

The CLI (`cli/gamebox.hexa`) dispatches into three runtime pillars plus the
tooling layer. Each subcommand emits a machine-parseable line
`__GAMEBOX__ <PASS|FAIL|PARTIAL> <subcmd> <reason>`.

| Component | Path | Role |
|---|---|---|
| CLI | `cli/gamebox.hexa` | Entry point — `status` / `list` / `probe` / `launch` / `selftest` with own2 honest PASS/FAIL/PARTIAL emit |
| Dispatch | `lib/dispatch.hexa` | Subcommand dispatcher |
| Detect | `lib/detect.hexa` | Game detection |
| PE / Win32 loader | `lib/loader/` (`*_core`) | 435/435 Win32 fn skeleton coverage across 32 PE `*_core` modules (advapi32, kernel32, ntdll, user32, gdi32, shell32, ole32, ws2_32, wininet, crypt32, rpcrt4, …) |
| Calling-convention adapters | `lib/loader/` | cdecl / stdcall / fastcall / vectorcall / x64-MS / to_arm64 |
| Runtime binding | `lib/loader/` | `dll_resolve_master` / `iat_runtime_bind` / `i386_call_frame` / `native_helper_bridge` / `oss_validate` |
| Battle.net subsystem | `lib/loader/` | Agent IPC / OAuth / setup install phase / HTTP REST / SD forging / bypass (own1 — D2R offline shim only) |
| DirectX bridge | `lib/loader/` | d3d11 / d3d11_stage1 / d3d12 / d3d12_stage1 / d3d12_stage2 — D3DMetal-pinned (no Wine) |
| Launchers | `lib/loader/` | `d2r_launch` / `d4_launch` |
| Perf layer | `lib/perf/` | 242+ `c_*_*.hexa` modules across 104 unique families; 18+ families at 100% manjeom (skeleton-tier) |
| Native helpers | `native/` | Apple-native helper shims |
| Tools | `tool/` | `roadmap_op.hexa` (14 subcmds; anima parity broken-intentional per own1) + `closure_orchestrator.hexa` (weighted_threshold awareness, mk2 checkpoint integration) |
| Roadmap SSOT | `.roadmap.*` | Per-game / per-launcher / per-loader SSOT (14 active + 1 historical `wow`) |
| Checkpoint SSOT | `.checkpoint.*` | Cross-roadmap forensic checkpoint snapshots (7 files; 3 met post Track BV) |
| Session artifacts | `airgenome/`, `gamebox/`, `state/` | Preserved predecessor + native lifecycle markers and handoff docs (own2 traceability mandate) |

## Data flow

```
unmodified Win32 PE binary
        │
        ▼
cli/gamebox.hexa  ──►  lib/dispatch.hexa  ──►  lib/detect.hexa (game id)
        │
        ├─► lib/loader/   PE *_core + calling-conv adapters + IAT/DLL runtime bind
        │                 + Battle.net (agent IPC / OAuth / offline shim)
        │                 + launchers (d2r / d4)
        │
        ├─► lib/loader/   DX bridge (d3d11 / d3d12 stage1/2)  ──►  D3DMetal (pinned)
        │
        └─► lib/perf/     242+ perf modules (104 families)
                          │
                          ▼
                  __GAMEBOX__ <PASS|FAIL|PARTIAL> <subcmd> <reason>
```

Real launch (`gamebox launch <game>`) returns PARTIAL
(`retired_intentional:skeleton_tier_only`) until hexa-interp closure milestones
CM-26..CM-30 (full launch + first-frame + sustained-frame render) land.

## Governance & verify

- **own1 invariant** — Wine 0 / DRM bypass 0 / Warden bypass 0 / anti-cheat
  bypass 0 / anima clone 0. `c_bnet_warden_environment_compat` and
  `c_anticheat_detection_track` are detection-track scaffolds only.
- **own2 honesty** — every subcommand emits an explicit PASS/FAIL/PARTIAL
  reason; predecessor markers are preserved verbatim for traceability.
- **Harness** — this repo is governed by the dancinlab harness
  (`.harness-engine/` submodule, `harness.config.json`). Architecture changes
  update **this file** (SSOT); history goes to `CHANGELOG.jsonl`; transient output
  goes under `scripts/scratch/`. Run `bash .harness-engine/bin/harness verify`
  (wraps `hexa verify`) and `bash .harness-engine/bin/harness docs check`
  before shipping. See [CLAUDE.md](CLAUDE.md) for the harness quick reference.
