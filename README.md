# gamebox — Apple-native Win32 Game Runtime

> Apple-native Win32 game runtime. Hexa-only, Wine 0, D3DMetal-pinned.
> Skeleton-tier 100% Win32 fn coverage (435/435) + 18+ perf families at 100%
> manjeom + 14 .roadmap.* SSOT + 7 .checkpoint.* (1 met) + 4 hexa-lang upstream
> RFCs merged.
> **Forensic verdict (2026-05-05): SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0**
> — 67 BG cycles (2026-05-03 to 2026-05-05) cumulative skeleton work; raw#10
> honest C3: validated_manjeom = 0 (no real game frames evidenced).

[![License: Apache-2.0](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](LICENSE)
[![Version](https://img.shields.io/badge/version-1.0.0-informational.svg)](CHANGELOG.md)
[![GitHub release](https://img.shields.io/github/v/release/need-singularity/gamebox?display_name=tag&sort=semver)](https://github.com/need-singularity/gamebox/releases)
[![Forensic: SKELETON_TIER_100PCT_WIN32](https://img.shields.io/badge/forensic-SKELETON__TIER__100PCT__WIN32-brightgreen.svg)](#forensic-verdict)
[![Win32 fn coverage: 435/435](https://img.shields.io/badge/win32__fn__coverage-435%2F435-success.svg)](#module-inventory)
[![Perf families 100%: 18+](https://img.shields.io/badge/perf__families__100pct-18%2B-success.svg)](#module-inventory)
[![hexa-lang RFCs merged: 4](https://img.shields.io/badge/hexa--lang__rfcs__merged-4-blue.svg)](#hexa-lang-upstream)

> **Distribution**: GitHub canonical at <https://github.com/need-singularity/gamebox>.
> CLI tooling installed via `hx install gamebox` from the hexa-lang registry,
> or `git clone` directly. (Sibling repo: <https://github.com/need-singularity/sim-universe>.)

---

## What is gamebox?

`gamebox` is an Apple-native Win32 game runtime. The goal is to run unmodified
Win32 PE binaries on macOS Apple Silicon **without Wine** — using D3DMetal for
graphics, hexa-lang for the loader / perf / dispatch layer, and a strict own1
posture (no DRM bypass, no Warden bypass, no anti-cheat bypass, no anima
clone).

It combines:

1. **PE/Win32 loader** — 435/435 Win32 fn skeleton coverage across 32 PE *_core
   modules (advapi32, kernel32, ntdll, user32, gdi32, shell32, ole32, ws2_32,
   wininet, crypt32, rpcrt4, etc.). Calling-convention adapters cdecl / stdcall /
   fastcall / vectorcall / x64-MS / arm64.
2. **Battle.net subsystem** — Agent IPC + OAuth + setup install phase + HTTP REST +
   SD forging + bypass own1 (D2R offline shim).
3. **DirectX bridge** — d3d11 / d3d12 stage1 / stage2 dispatch; D3DMetal-pinned
   path (no Wine).
4. **Perf layer** — 242 c_*_*.hexa modules across 104 unique families;
   18+ families at 100% manjeom (skeleton-tier; validated_manjeom = 0 per raw#10).
5. **Tools** — `roadmap_op.hexa` (14 subcmd, anima parity broken intentional per
   own1) + `closure_orchestrator.hexa` (weighted_threshold awareness, mk2
   checkpoint integration).
6. **CLI** — `cli/gamebox.hexa` (status / list / probe / launch / selftest with
   own2 honest PASS/FAIL/PARTIAL emit).

The name reflects the **gamebox-as-substrate** framing — a hexa-only runtime
substrate for Win32 games on Apple. **This is NOT** Wine, NOT a DRM bypass, NOT
an anti-cheat bypass, NOT a Warden bypass, NOT an anima clone. See [Caveats](#caveats-raw10-c3)
for full honesty disclosure.

---

## Architecture

```
                        ┌─────────────────────────────────────┐
                        │           gamebox CLI               │
                        │       (cli/gamebox.hexa)            │
                        │  status  list  probe  launch        │
                        │            selftest                 │
                        └─────────────────┬───────────────────┘
                                          │
            ┌─────────────────────────────┼──────────────────────────────┐
            │                             │                              │
            ▼                             ▼                              ▼
   ┌────────────────┐          ┌────────────────────┐         ┌──────────────────┐
   │  PE/Win32      │          │       DX bridge    │         │     perf layer   │
   │   loaders      │          │   (D3DMetal-pin)   │         │   (242 modules,  │
   ├────────────────┤          ├────────────────────┤         │    104 families) │
   │ 32 *_core      │          │  d3d11             │         ├──────────────────┤
   │  └ advapi32    │          │  d3d11_stage1      │         │ 18+ families     │
   │  └ kernel32    │          │  d3d12             │         │  at 100% manjeom │
   │  └ ntdll       │          │  d3d12_stage1      │         │  (skeleton-tier) │
   │  └ user32      │          │  d3d12_stage2      │         │ raw#10 honest C3:│
   │  └ ws2_32      │          └────────┬───────────┘         │  validated_manjeom│
   │  └ wininet     │                   │                     │  = 0             │
   │  └ etc.        │                   │                     └────────┬─────────┘
   │ calling conv:  │                   │                              │
   │  cdecl/std/fast│                   │                              │
   │  vector/x64    │                   │                              │
   │  /to_arm64     │                   │                              │
   │ runtime:       │                   │                              │
   │  dll_resolve_  │                   │                              │
   │   master       │                   │                              │
   │  iat_runtime_  │                   │                              │
   │   bind         │                   │                              │
   │  i386_call_    │                   │                              │
   │   frame        │                   │                              │
   │  native_helper │                   │                              │
   │  oss_validate  │                   │                              │
   ├────────────────┤                   │                              │
   │ Battle.net     │                   │                              │
   │  └ agent IPC   │                   │                              │
   │  └ OAuth       │                   │                              │
   │  └ setup phase │                   │                              │
   │  └ bypass(own1)│                   │                              │
   ├────────────────┤                   │                              │
   │ launchers:     │                   │                              │
   │  d2r_launch    │                   │                              │
   │  d4_launch     │                   │                              │
   └───────┬────────┘                   │                              │
           │                            │                              │
           └────────────────────────────┼──────────────────────────────┘
                                        │
                                        ▼
                              ┌──────────────────────┐
                              │   tool/ (2 hexa CLI) │
                              │  roadmap_op.hexa     │
                              │  closure_orchestrator│
                              └──────────────────────┘
```

35 loaders / 242 perf modules / 2 tools / 14 roadmaps / 7 checkpoints / 8 tests /
75 predecessor markers (`airgenome_gamebox_*`).
raw#9 STRICT (hexa-only, no .py creation in this repo).

---

## Installation

### Via `hx` (recommended)

```bash
hx install gamebox          # global, pulls latest from registry
hx install gamebox@1.0.0    # pin specific version
gamebox --version           # → 1.0.0
```

> gamebox is registered alongside sibling `sim-universe` in the hexa-lang
> package registry. `hx install gamebox` pulls from
> <https://github.com/need-singularity/gamebox> and installs the standalone CLI
> under `$HX_HOME/bin/gamebox`.

### Via git clone (works today)

```bash
git clone https://github.com/need-singularity/gamebox.git ~/.gamebox
export GAMEBOX_ROOT=~/.gamebox
export PATH="$GAMEBOX_ROOT/cli:$PATH"

# Run any subcommand:
hexa run $GAMEBOX_ROOT/cli/gamebox.hexa selftest --quick
```

### Apple platform requirement

gamebox is Apple-native; the canonical platform is macOS Apple Silicon (arm64)
with Game Porting Toolkit / D3DMetal SDK installed. Skeleton-tier loaders /
perf modules import as a hexa lib without D3DMetal — only real launches require
D3DMetal. Set `GAMEBOX_D3DMETAL_SDK=/path/to/D3DMetal` for explicit override.

---

## Quickstart

```bash
# Package version + forensic verdict + inventory snapshot
gamebox status

# Enumerate loaders / perf families / tools
gamebox list loaders
gamebox list perf
gamebox list tools

# Skeleton-tier import probe (own2 PARTIAL emit)
gamebox probe

# Real launch dispatch (RETIRED_INTENTIONAL skeleton-tier — own1 + validated_manjeom 0)
gamebox launch d2r

# Quick self-validation (own2 PASS/FAIL emit)
gamebox selftest --quick
```

For machine-parseable output, every subcmd emits:
`__GAMEBOX__ <PASS|FAIL|PARTIAL> <subcmd> <reason>`.

---

## Module Inventory

### Loaders — `lib/loader/` (35 modules)

| Group | Modules | Description |
|---|---|---|
| PE *_core | 32 | advapi32 / kernel32 / ntdll / user32 / gdi32 / shell32 / ole32 / ws2_32 / wininet / crypt32 / rpcrt4 / etc. — 435/435 Win32 fn skeleton coverage |
| Calling conv | 6 | cdecl / stdcall / fastcall / vectorcall / x64-MS / to_arm64 |
| Battle.net | 9 | agent IPC / OAuth / setup install phase / HTTP REST / SD forging / bypass (own1) |
| DX bridge | 5 | d3d11 / d3d11_stage1 / d3d12 / d3d12_stage1 / d3d12_stage2 (D3DMetal-pin) |
| Runtime | 5 | dll_resolve_master / iat_runtime_bind / i386_call_frame / native_helper_bridge / oss_validate |
| Launchers | 2 | d2r_launch / d4_launch |

### Perf — `lib/perf/` (242 modules, 104 unique families)

18+ families at 100% manjeom (skeleton-tier per Track BL/BM forensic):

| Family | Manjeom | Notes |
|---|---|---|
| `c_abi_calling_convention_track` | 100% | calling-convention abstraction |
| `c_anticheat_detection_track` | 100% | own1: detection track only, NO bypass |
| `c_audio_dsp` / `c_audio_mix_state` | 100% | audio DSP + mix state |
| `c_bnet_login_state_track` / `_token_refresh_track` | 100% | Battle.net login + token refresh |
| `c_bnet_warden_environment_compat` | 100% | own1: compat scaffold, NO Warden bypass |
| `c_bigworld_core_dx11_first_frame` / `_shader_persist` | 100% | UE3-derived bigworld engine first-frame |
| `c_cdn_chunk_assemble_track` / `_mirror_select` | 100% | CDN chunk assembly + mirror select |
| `c_d2r_archive_round_trip` | 100% | D2R MPQ archive round-trip (cond1) |
| `c_cs2_source2` / `_steam_api64` / `_tier0` | 100% | CS2 / Source2 / Steam API skeletons |
| `c_amx_matrix_multiply` | 100% | Apple AMX matrix multiply |
| `c_argument_buffer_tier2_map` | 100% | Metal argument-buffer Tier-2 mapping |
| `c_ca_display_link_vsync` | 100% | CADisplayLink vsync |

(Full list: 242 files in `lib/perf/`, snake_case.)

### Tools — `tool/` (2)

| Tool | Subcmds | Description |
|---|---|---|
| `roadmap_op.hexa` | 14 | roadmap CRUD + emit; anima parity **broken intentional** per own1 (no anima clone) |
| `closure_orchestrator.hexa` | 3 + selftest | closure progression tracker, weighted_threshold awareness, mk2 checkpoint integration |

### Roadmaps + Checkpoints (mk2 SSOT)

- 14 `.roadmap.*` files: battlenet, cs2, delta_force, diablo2_resurrected, diablo4,
  elden_ring, lineage_classic_r28, lineage_m, lineage_w, loader_dx, loader_pe,
  loader_win32, purple_launcher, wow.
- 7 `.checkpoint.*` files: d2r_battlenet_offline_play, d2r_battlenet_online_play,
  lineage_classic_r28_play, lineage_m_purple_play, lineage_w_purple_play,
  win32_50pct_coverage, win32_full_coverage. **1 met**: `win32_50pct_coverage`
  (Track BR derivation guard).

### Predecessor markers — `state/markers/airgenome_gamebox_*.marker` (75)

Per **own2** mandate, 75 markers from the airgenome-gamebox 67-cycle session
(2026-05-03 to 2026-05-05) are preserved verbatim under their original
`airgenome_gamebox_*` prefix to maintain traceability + cycle history attribution.

---

## hexa-lang upstream

4 RFCs merged this session into <https://github.com/need-singularity/hexa-lang>:

- **rfc_012**: snake_case enforcement (PR #47)
- **rfc_013**: silent_error_ban (PR #47)
- **rfc_014**: env_lazy (PR #47)
- **rfc_015**: pattern matcher re-assessment (PR #48)

PR #47 squash-merged 2026-05-04 (Track AY). PR #48 merged 2026-05-04 (Track BD/BG).

---

## Caveats (raw#10 honest C3)

> The forensic verdict `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` carries
> 5 explicit honesty caveats. **Read these before assuming gamebox runs games.**

1. **`validated_manjeom = 0`**. Skeleton-tier means the loader / perf modules
   are present and individually compile / probe; **no real game frames have
   been evidenced**. Closure milestones CM-26..CM-30 (per
   `docs/CLOSURE_ROADMAP.md` §5) — hexa interp full launch + first-frame
   render + sustained-frame render — are **NOT** done.

2. **own1 invariant carries**. Wine 0 / DRM bypass 0 / Warden bypass 0 /
   anti-cheat bypass 0 / anima clone 0. `c_bnet_warden_environment_compat`
   and `c_anticheat_detection_track` are **detection-track scaffolds only** —
   they do not bypass Warden or anti-cheat. `battlenet_bypass.hexa` is the
   D2R offline-shim disambiguation only (own1 PASS verdict in
   `airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker`).

3. **L1-L5 online-expansion lock-in carries**. Per
   `.roadmap.battlenet`: L1 EULA / L2 Warden / L3 multi-year /
   L4 D2R-single / L5 parallel are explicit lock-in conditions. Real online
   play remains gated.

4. **anima parity broken intentional**. `tool/roadmap_op.hexa` 14-subcmd CLI
   does **not** clone airgenome's anima system per own1. Only roadmap
   CRUD + emit are operational; anima fields are read-through, not
   re-implemented.

5. **hexa interp closure missing**. The hexa stdlib used by gamebox CLI
   covers `exec` / `env` / file-stat probes (sufficient for `status` / `list` /
   `selftest`). Real launch via `cli/gamebox.hexa launch <game>` returns
   PARTIAL (`retired_intentional:skeleton_tier_only`) until hexa interp
   closure CM-26..CM-30 lands.

---

## Forensic verdict

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

- **skeleton_manjeom = 100%**: 435/435 Win32 fn coverage + 18+ perf families
  at 100% (Track BL/BM forensic). Win32 100% milestone (Track BP) attained
  2026-05-05.
- **validated_manjeom = 0%**: no real game frames; closure CM-26..CM-30 not done.
- **provenance**: 67 BG cycles cumulative session (2026-05-03 to 2026-05-05).
- **session predecessor**: `dancinlife/airgenome-gamebox` (DELETED 2026-05-05
  per Option B + closure cycle directive; history not transferred per D3 fresh-start
  policy).

---

## License

Apache-2.0 — see [LICENSE](LICENSE).

Author: 박민우 <nerve011235@gmail.com>
Repository: <https://github.com/need-singularity/gamebox>
