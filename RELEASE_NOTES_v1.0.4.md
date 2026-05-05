# gamebox v1.0.4 — Honest Freeze Family Complete + Nightreign Skeleton-Tier Fan-out + Perpetual Deferral Ack (2026-05-05)

## Summary

Patch release bundling post-v1.0.3 work — final closure cycle of the standalone-repo session:

- **Track CK** — `docs/VAC_HONEST_FREEZE.md` mk1 narrative (Valve Anti-Cheat user-mode AC + Trusted Mode honest spec freeze)
- **Track CL** — `docs/EAC_KERNEL_HONEST_FREEZE.md` mk1 narrative (EAC kernel + anti-tamper class honest spec freeze; 3-game cross-class delta_force + elden_ring base+DLC + nightreign inherited)
- **Track CM** — Nightreign lib module fan-out at skeleton-tier (1 loader + 3 perf modules, 609 LOC; resolves Track BY deferred hook)
- **Track CN** — Hexa interp closure CM-26..CM-30 perpetual deferral acknowledgment (formally closes CJ §11 hook 4 with explicit multi-year horizon; honest no-action)

## Highlights

- **Honest freeze family complete (5 docs)**: GAMEGUARD (Track-E) + WARDEN (Track-E) + D2R_REALM_PROTOCOL (Track-E) + **VAC (CK)** + **EAC_KERNEL (CL)** — covers all major AC classes in gamebox scope.
- **Nightreign fan-out complete at skeleton-tier**: 4 modules with retired_intentional posture (inherited blk.1 EAC + blk.2 FromSoft co-op matchmaking). Real-tier perpetually deferred per Track CN.
- **Hook 4 perpetually closed**: real-run validation requires hexa interp closure CM-26..CM-30 (multi-year horizon, sibling repo dependency). CN documents future cycle resumption protocol.
- **Forensic verdict (UNCHANGED)**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`. validated_manjeom = 0 carries.

## Changes

### Added (post-v1.0.3)

- `RELEASE_NOTES_v1.0.4.md` (this file).
- `docs/VAC_HONEST_FREEZE.md` (Track CK; ~250 LOC; Valve Anti-Cheat spec freeze).
- `docs/EAC_KERNEL_HONEST_FREEZE.md` (Track CL; ~440 LOC; EAC kernel + anti-tamper 3-cross-class).
- `lib/loader/pe_elden_ring_nightreign_specific_track.hexa` (Track CM; 216 LOC; 9 phase_kind).
- `lib/perf/c_nightreign_co_op_session_init.hexa` (Track CM; 133 LOC; 5 session_phase).
- `lib/perf/c_nightreign_matchmaking_probe.hexa` (Track CM; 129 LOC; 5 probe_state, Apple natural absence).
- `lib/perf/c_nightreign_dx12_first_frame_co_op_variant.hexa` (Track CM; 131 LOC; 5 frame_phase, D3DMetal transitive).
- 4 NEW `gamebox_*` markers (CK + CL + CM + CN).
- 4 NEW `gamebox/doc/` handoff docs (CK + CL + CM + CN).

### Changed

- `hexa.toml` `[package].version`: `1.0.3` → `1.0.4`.
- `README.md` Version badge: `version-1.0.3` → `version-1.0.4`.
- `.roadmap.elden_ring_nightreign`: `loader_count: 0` → `1`, `perf_count: 0` → `3`, NEW `loader_pattern` + `perf_pattern` + `ai_native_handoff_track_cm` fields, NEW comment header line 6 documenting CM skeleton-tier fan-out (Track BY documentation-only narrative line 5 preserved verbatim).
- `CHANGELOG.md`: v1.0.4 section additive insertion above v1.0.3.

### Removed

- None. CM added 4 lib modules; no file removal. own1 `destructive_ops: 0` maintained throughout.

### Posture

- **own1**: Wine 0 / hexa-only / DRM bypass 0 / Warden bypass 0 / VAC bypass 0 / Trusted Mode bypass 0 / EAC kernel bypass 0 / EAC anti-tamper bypass 0 / GameGuard bypass 0 / EOS handshake circumvention 0 / FromSoft matchmaking bypass 0 / co-op session emul 0 / handshake spoof 0 / anima clone 0 / `destructive_ops: 0` (maintained throughout CK-CN).
- **own2**: PASS/FAIL/PARTIAL/RETIRED_INTENTIONAL emit honest. validated_manjeom = 0 carries. Skeleton-tier vs real-tier distinction explicit. CM 4 modules each emit RETIRED_INTENTIONAL posture annotation. CN explicit no-fake-PASS / no-fake-real-run-completion declaration.

## Caveats (raw#10 C3) — UNCHANGED from v1.0.0

5 explicit honesty caveats unchanged. CM modules are skeleton-tier — synthetic
self_test PASSes with hardcoded data; real `nightreign.exe` binary measurement
deferred. CN explicitly marks all real-run validation perpetually deferred
until hexa interp closure CM-26..CM-30 land event (multi-year horizon per
Track AU production smoke gate path A own1-maintained).

## Cumulative state (post v1.0.4)

| metric | v1.0.0 | v1.0.1 | v1.0.2 | v1.0.3 | **v1.0.4** |
|---|---|---|---|---|---|
| .roadmap files | 14 | 14 | 15 | 15 | **15** (no change) |
| .checkpoint met | 0 | 3 | 3 | 3 | **3** (no change) |
| per_game §9 sections | 0 | 0 | 11 | 11 | **11** (no change) |
| gamebox-native markers | 1 | 7 | 9 | 15 | **19** (CK+CL+CM+CN add 4) |
| Honest freeze docs | 3 | 3 | 3 | 3 | **5** (CK + CL add 2) |
| lib/loader files | 455 | 455 | 455 | 455 | **456** (CM adds 1 nightreign loader) |
| lib/perf files | 242 | 242 | 242 | 242 | **245** (CM adds 3 nightreign perf) |
| Layout | flat docs/ | flat docs/ | `<feature>/doc/` | `<feature>/doc/` | `<feature>/doc/` (no change) |
| Forensic verdict | UNCHANGED | UNCHANGED | UNCHANGED | UNCHANGED | UNCHANGED |
| validated_manjeom | 0 | 0 | 0 | 0 | **0** (carries) |

## Compatibility

No breaking changes. `gamebox` CLI behavior unchanged. Roadmap + checkpoint
JSONL schema unchanged. New CM 4 nightreign modules are additive; existing
modules unchanged.

## Source repo

- Sibling: <https://github.com/need-singularity/sim-universe>
- Branch: `main` (final closure session post Track CJ).

Predecessor releases:
- v1.0.0 (BT) — initial extraction
- v1.0.1 (BW) — drift resolution + win32 milestone (BU + BV)
- v1.0.2 (CD) — layout migration + inventory refresh + per_game §9 closure
- v1.0.3 (CI) — paper-mode validation + inventory cleanup + v3 aggregate (CE + CF + CG + CH)
- **v1.0.4 (CO) — honest freeze family complete + nightreign skeleton-tier fan-out + perpetual deferral ack (CK + CL + CM + CN)** (this release)

## Author

박민우 <nerve011235@gmail.com>
