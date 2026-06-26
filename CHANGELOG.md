# Changelog

All notable changes to `gamebox` are documented in this file.

## [Unreleased]

### Added

- feat(battlenet cond.3): `pe_battle_net_oauth_token` self_test 가 op_kind 6
  `device_code_grant`(`/oauth/device` RFC8628) 을 비로소 exercise — 헤더에
  문서화됐으나 self_test 미커버였던 갭을 닫음(6 op_kind 전부 round-trip). 합성
  데이터·token 실값 0·length-only·실 네트워크 0(own1 준수). 어서션 정합:
  op_count 6→7, total_token_lifetime_sec 345600→432000, total_token_size_bytes
  5120→6144, count_by_op_kind(6) 0→1, stats `[7,432000,6144,2]`, emit ≥8.
  cond.3 status 는 partial 유지(실 OAuth 엔드포인트 network smoke 게이트 = blk.1).

- docs(CLAUDE.md): CI(Blacksmith) 규율 박제 — `## CI — builds run on Blacksmith,
  NOT locally` 섹션 추가(워크플로/러너/툴체인/실행 내용 + "로컬 빌드 말고 push
  하라" 규칙) + Structure 트리에 `.github/workflows/` 항목. 같은 결함(로컬 빌드로
  Mac 죽음) 재발 방지를 위해 거버넌스 SSOT 에 명시.


- ci(blacksmith): `.github/workflows/ci.yml` — gamebox 빌드+테스트를
  Blacksmith 클라우드 Apple-Silicon 러너(`blacksmith-6vcpu-macos-15`)에서 실행.
  gamebox 는 Apple-native(Win32 PE + D3DMetal)라 컴파일+apple-only 테스트에
  macOS 호스트가 필요한데, 그걸 로컬 dev Mac 에서 돌리면 242+ perf 모듈 +
  8 테스트가 메모리를 스파이크해 머신이 죽었다. 이제 push 마다 클라우드 Mac 이
  빌드/테스트를 맡고 로컬 박스는 `git push` 만 한다. 러너는 macos-15 핀
  (hexa-lang `release.yml` 와 동일 근거 — SDK min-OS 베이스라인 안정). hexa
  툴체인은 릴리스 바이너리를 canonical 설치 한 줄(`install.sh`)로 깐다. D3DMetal
  SDK 부재는 warn-only(skeleton-tier 로더는 importable). same-org 라
  Blacksmith 활성화 별도 작업 불필요(hexa-lang 이 이미 사용).

### Changed

- docs(architecture): restructure `ARCHITECTURE.json` into a deeper `children`
  tree per harness governance rule c4 — a LOSSLESS re-shape (restructure only,
  no content change). Decomposed 10 over-long dump cells (root `summary` 527c +
  root `note` 731c + 8 node `summary` fields 263–531c) that piled multiple
  distinct facts / ` · `-joined lists into one field. Each piled fact is now its
  own child node (short role line stays on the parent `summary`; each remaining
  sentence/list becomes a `{name, summary}` `ℹ️` info child). Node count
  26 → 47. The two C-PORT milestones >250c are single coherent done-records
  (`milestones[]` is already the per-item list) and stay intact — not dumps.
  Losslessness verified by non-whitespace char-multiset before vs after =
  identical (0 lost / 0 added; only scaffolding node-name labels added).
  `ARCHITECTURE.html` viewer + `python3 serve.py` stay renderable (generic over
  the `children` tree; new nodes use only `name`+`summary`).

- chore(architecture): retire `DOMAINS.tape` + `domains/` (22 domain `.md`
  snapshots + 22 `.log.md` logs) into a single `ARCHITECTURE.json` tree SSOT
  (hexa-codex/anima pattern), with the `ARCHITECTURE.html` viewer +
  `python3 serve.py` static server (copied + adapted from `dancinlab/anima`).
  Each domain's `@goal` → node `summary`, its `- [ ]/[x]` milestones →
  `milestones[]`, and the DOMAINS.tape `@link` edges → per-node `edges[]`
  (`--has-->` tree + `--uses-->`/`--targets-->` cross-edges preserved). Tree =
  22 domains (1 meta-root GAMEBOX + 6 메인구조 LOADER/WIN32/GFX/NET/PERF/NATIVE +
  C-PORT nested under NATIVE + 2 런처 BNET/PURPLE + 1 sprint-meta LAUNCHPAD+ +
  11 게임), grouped under 4 section headers → 26 JSON nodes. `ARCHITECTURE.md`
  stays as the component-level prose SSOT and now points to the JSON tree.
  `native/i386_decode.hexa` C-PORT cross-link repointed from `domains/C-PORT.md`
  → the C-PORT node in `ARCHITECTURE.json` (provenance string left intact).
  Full per-domain `.log.md` history is recoverable via git log; the substantive
  entries folded below.

  Domain-log digest (folded from `domains/*.log.md`):
  - All 22 domains scaffolded 2026-05-31 (domain init, seeded from the gamebox
    전수조사 — @goal + milestones from `.roadmap.*` + repo survey of
    lib/loader · lib/perf · native).
  - **C-PORT** (2026-06-03): M1 inventory+classify (4 native/*.c, 869 LOC;
    Tier-A 437 · Tier-B 113 · Tier-C 319) → M2 decode_modrm_disp port
    (RUNEQ 768/768, sha256 b4388479) → M3 decode_prefixes + i386_op_name +
    rd_s32/rd_s16 (RUNEQ 1069/1069, sha256 8e6acbb8) → M4 stateful
    i386_decode_one (RUNEQ 1042/1042, sha256 b2853d0a). Both M2 and M4 caught a
    real write-before-fail out-param divergence, fixed → byte-identical.
  - **GAMEBOX-D2R**: prerequisite @uses edges wired (LOADER/WIN32/GFX/NET +
    BNET); reconciled 3/4 — D3D11/MPQ/D2S code present + offline/online
    checkpoints landed; gameplay-tracks (26 c_d2r_*) left open.
  - **GAMEBOX-BNET**: reconciled 5/6 — 10 pe_battle(_)net_* + orchestrator
    present, D2R offline+online checkpoints prove the path; D2R non-blocking;
    D4/CS2 launch extension open.
  - **GAMEBOX-PURPLE**: reconciled 4/4 — 7 c_purple_* + Lineage M/W purple play
    checkpoints; scope = M/W; Classic depends on PURPLE for download only.
  - **GAMEBOX-LINEAGE-CLASSIC**: wired LOADER/WIN32/GFX @uses; reconciled 3/4
    (r28 loader + native first-frame + play checkpoint + drift-resolution track
    bv); two corrections recorded — PURPLE is the download channel (runtime
    native, "no Purple shell"), and Classic is an ONLINE MMO (not offline);
    playable gate = GameGuard (INCA) kernel AC honest-freeze.
  - **GAMEBOX-LAUNCHPAD+**: meta-domain init (`+` composition) targeting D2R +
    LINEAGE-CLASSIC; @has + @targets edges wired; shared "playable" smoke bar
    (boot→title→char→in-world) defined with online/anti-cheat honest-freeze.

### Removed

- `DOMAINS.tape` (domain roster + tree graph) — superseded by
  `ARCHITECTURE.json` (`meta` + per-node `edges[]`).
- `domains/` (22 `*.md` domain snapshots + 22 `*.log.md` logs) — `*.md` folded
  into `ARCHITECTURE.json` nodes; `*.log.md` digested into the entry above
  (full text recoverable via git log).

- chore(harness): perfect harness setup — engine submodule bumped to latest on
  `harness-hardcore`; authored `ARCHITECTURE.md` SSOT (overview + component map +
  data flow + governance/verify, English) over the stub; authored harness-standard
  `CLAUDE.md` (H1 + blurb + `## Structure` tree + governance + `## Harness` +
  quick reference) over the SPECKIT stub; added a `docs` discipline block to
  `harness.config.json` (`architecture`/`log`/`scratchDir` + `scopeDirs: [""]`
  root-only + `allow` list covering README/CHANGELOG/ARCHITECTURE/CLAUDE/AGENTS
  plus `RELEASE_NOTES_v1.0.*` and `TAPE-AUDIT.md`). `harness docs check` now
  reports `docs: ok` with 0 CLAUDE-MD violations. `.claude/settings.json` already
  carried the full guarded harness hook set (pre bash / pre write / post edit /
  prompt / prefs · easy · recommend inject / SessionStart) — preserved as-is.

## [1.0.4] — 2026-05-05

### Added

- Track CK — `docs/VAC_HONEST_FREEZE.md` mk1 narrative (Valve Anti-Cheat user-mode AC + Trusted Mode; resolves cs2.cond.3 deferred hook).
- Track CL — `docs/EAC_KERNEL_HONEST_FREEZE.md` mk1 narrative (EAC kernel + anti-tamper class; 3-cross-class delta_force + elden_ring base+DLC + nightreign).
- Track CM — Nightreign lib module fan-out at skeleton-tier:
  - `lib/loader/pe_elden_ring_nightreign_specific_track.hexa` (216 LOC)
  - `lib/perf/c_nightreign_co_op_session_init.hexa` (133 LOC)
  - `lib/perf/c_nightreign_matchmaking_probe.hexa` (129 LOC)
  - `lib/perf/c_nightreign_dx12_first_frame_co_op_variant.hexa` (131 LOC)
  - Total 609 LOC; retired_intentional posture inherited from blk.1 + blk.2.
- Track CN — `gamebox/doc/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed_2026_05_05.ai.md` (formally closes CJ §11 hook 4 as perpetually deferred; multi-year horizon).
- 4 NEW `gamebox_*` markers (CK + CL + CM + CN).
- 4 NEW `gamebox/doc/` handoff docs (CK + CL + CM + CN).

### Changed

- `hexa.toml` version: `1.0.3` → `1.0.4`.
- `README.md` Version badge: `version-1.0.3` → `version-1.0.4`.
- `.roadmap.elden_ring_nightreign`: `loader_count: 0 → 1`, `perf_count: 0 → 3`, NEW `loader_pattern` + `perf_pattern` + `ai_native_handoff_track_cm` fields, NEW comment header line 6 documenting Track CM skeleton-tier fan-out (Track BY documentation-only narrative line 5 preserved verbatim).

### Cumulative deltas (v1.0.3 → v1.0.4)

- gamebox-native markers: 15 → 19
- Honest freeze docs: 3 → 5 (Track-E predecessor + CK + CL)
- lib/loader files: 455 → 456 (+1 nightreign loader)
- lib/perf files: 242 → 245 (+3 nightreign perf)
- Forensic verdict: UNCHANGED (`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`)
- validated_manjeom: UNCHANGED (0 — skeleton-tier per Track BL/BM forensic baseline)

### Posture

- own1: Wine 0 / hexa-only / no DRM bypass / no AC bypass (5 classes covered: VAC + GameGuard + EAC kernel + EAC anti-tamper + Warden) / no FromSoft matchmaking bypass / no anima clone / `destructive_ops: 0` (maintained).
- own2: 4 CM modules emit RETIRED_INTENTIONAL posture annotation; CN explicit no-fake-PASS / no-fake-real-run-completion declaration; skeleton-tier vs real-tier distinction explicit; spec freeze ONLY scope explicit per CK + CL.

## [1.0.3] — 2026-05-05

### Added

- Track CE — `gamebox/doc/gamebox_closure_orchestrator_selftest_paper_mode_track_ce_landed_2026_05_05.ai.md` (S1-S11 paper-mode trace; BZ §11 hook partial resolution).
- Track CF — `gamebox/doc/gamebox_hexa_toml_modules_audit_track_cf_landed_2026_05_05.ai.md` (40 entries audit; 1 drift item flagged for CG).
- Track CG — `gamebox/doc/gamebox_c_d2r_archive_round_trip_drift_fix_track_cg_landed_2026_05_05.ai.md` (orphan reference option-b removal).
- Track CH — `gamebox/doc/gamebox_session_aggregate_v3_track_ch_landed_2026_05_05.ai.md` (first standalone-repo session aggregate covering BT-CG).
- hexa.toml `gamebox_perf_top_families` explanatory comment block (CG).
- 4 NEW `gamebox_*` markers (CE / CF / CG / CH).
- `.gitignore` pattern: `resource_op_[0-9]*.marker` (CE post-mortem).

### Changed

- `hexa.toml` version: `1.0.2` → `1.0.3`.
- `hexa.toml` `gamebox_perf_top_families`: orphan `c_d2r_archive_round_trip` entry removed (CG).
- `README.md` Version badge: `version-1.0.2` → `version-1.0.3`.
- `README.md` perf families table: `c_d2r_archive_round_trip` row removed (CG).

### Cumulative deltas (v1.0.2 → v1.0.3)

- gamebox-native markers: 9 → 15 (BT-CH)
- RFC 016 audit cumulative null floor: 28 → 34 cycles
- hexa.toml audit drift items: 1 → 0
- Forensic verdict: UNCHANGED (`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`)
- validated_manjeom: UNCHANGED (0)

### Posture

- own1: Wine 0 / hexa-only / no DRM / no AC / no anima clone / `destructive_ops: 0` (CG was reference-removal not file-deletion).
- own2: paper-mode vs real-run distinction explicit; drift fix option chosen documented; v3 vs v1/v2 distinction explicit.

## [1.0.2] — 2026-05-05

### Added

- **Layout migration** (`1d28b4c refactor(layout 2026-05-05)`): canonical `<feature>/doc/` layout — `airgenome/doc/` (75 predecessor handoffs) + `gamebox/doc/` (9 native handoffs incl. CB-CC).
- Track BX — per_game README §9 sections for 6 offline-relevant files (lineage_classic_r28 + d2r + battlenet + lineage_w + lineage_m + purple_launcher).
- Track BY — README Game Coverage refresh: wow row removed (historical preserve), elden_ring DLC scope clarified (Shadow of the Erdtree 2024-06-21), elden_ring_nightreign added (2025-05-30 standalone multiplayer co-op spinoff, documentation-only fan-out).
- NEW `.roadmap.elden_ring_nightreign` (documentation-only, 0 lib modules at land-time).
- NEW `docs/per_game/elden_ring_nightreign_README.ai.md` T1 baseline.
- `dlc_scope` + `spinoff_cross_link` + `ai_native_handoff_track_by` fields on `.roadmap.elden_ring`.
- Track BZ — `gamebox/doc/gamebox_closure_orchestrator_paper_invocation_track_bz_landed_2026_05_05.ai.md` (real-invocation attempt + paper-mode derivation; hexa interp blocker).
- Track CA — `gamebox/doc/gamebox_hexa_upstream_rfc_016_reassessment_track_ca_landed_2026_05_05.ai.md` + `docs/HEXA_UPSTREAM_RFC_016_REASSESSMENT_TRACK_CA_2026_05_05.md` (28-cycle null floor reaffirmed).
- Track CB — README "Native markers" + "Resolver telemetry markers (gitignored)" subsections.
- Track CC — per_game README §9 sections for 4 retired_intentional files (cs2 + delta_force + diablo4 + elden_ring); elden_ring §9 surfaces DLC scope narrative.
- `.gitignore` patterns for hexa-resolver runtime telemetry markers.
- 8 NEW `gamebox_*` markers post-v1.0.0 (BT through CC).

### Changed

- README.md TL;DR header: `.roadmap` 14 → 15 (14 active + 1 historical), `.checkpoint` met 1 → 3.
- README.md architecture caption: terse 7-figure → detailed inventory (455 .hexa files, 9 tests, 88 markers across 3 classes).
- README.md Loaders heading: `(35 modules)` → `(59 named groups across 455 .hexa files)`.
- `.roadmap.elden_ring` goal: scope = base game + Shadow of the Erdtree DLC.
- `docs/airgenome_*.ai.md` (75) → `airgenome/doc/` per layout migration.
- `docs/gamebox_*.ai.md` (7) → `gamebox/doc/` per layout migration.
- `state/markers/*.marker` cross-references updated (193 files / 621 lines via sed sweep in `1d28b4c`).
- `hexa.toml` version: `1.0.1` → `1.0.2`.
- README version badge: `version-1.0.1` → `version-1.0.2`.

### Posture

- Forensic verdict UNCHANGED: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- `validated_manjeom` UNCHANGED: 0 (no real game frames).
- own1: Wine 0 / hexa-only / no DRM / no AC / no GameGuard / no EAC / no FromSoft matchmaking bypass / no anima clone / `destructive_ops: 0`.
- own2: honest emit; per_game §9 sections explicit about retired_intentional stable + cross-cutting impact + validated_manjeom carry.

## [1.0.1] — 2026-05-05

### Added

- Track BU — README "Game Coverage" section (+40 LOC; 11 .roadmap targets table + per-game index + 7 cross-roadmap checkpoint table + honest scope footer).
- Track BU — `met_iso` / `met_via` / `evidence[]` fields on `.checkpoint.win32_50pct_coverage` + `.checkpoint.win32_full_coverage`.
- Track BV — `met_iso` / `met_via` / `evidence[]` fields on `.checkpoint.lineage_classic_r28_play`.
- Track BV — 9 NEW `status_flipped_*` fields across `.roadmap.lineage_classic_r28` cond.1/cond.2/cond.3.
- 4 NEW handoff docs + markers (BU + BV).

### Changed

- `.checkpoint.win32_50pct_coverage` status: `unmet 30.34/50` → `met 100/50` (BU).
- `.checkpoint.win32_full_coverage` status: `unmet 30.34/100` → `met 100/100` (BU).
- `.checkpoint.lineage_classic_r28_play` status: `partial 0/3` → `met 3/3` (BV, sideways-PASS).
- `.roadmap.lineage_classic_r28` cond.1/cond.2/cond.3 actual status: `partial` → `retired_intentional` (BV cascading flip).
- `hexa.toml` version: `1.0.0` → `1.0.1`.
- 3 checkpoint `blockers` arrays renamed `blockers_resolved` (verbatim list preserved).

### Drift resolution

- Track BO snapshot v2 drift_count: 3 → 0 (all 3 items resolved across BU + BV).

### Posture

- Forensic verdict UNCHANGED: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- `validated_manjeom` UNCHANGED: 0 (no real game frames evidenced).
- own1: Wine 0 / hexa-only / DRM bypass 0 / Warden bypass 0 / anti-cheat bypass 0 / GameGuard bypass 0 / anima clone 0.
- own2: PASS/FAIL/PARTIAL/RETIRED_INTENTIONAL emit honest; status flip rationale per checkpoint cited.

## [1.0.0] — 2026-05-05

### Initial extraction

- **Forensic verdict**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`
- **Source**: `dancinlife/airgenome-gamebox` 67-cycle session (2026-05-03 to 2026-05-05; deleted 2026-05-05 per closure-cycle directive, history not transferred per D3 fresh-start policy)
- **Reference layout**: `dancinlab/sim-universe` v1.0.0 (sibling repo)

### Inventory

- 35 loader modules under `lib/loader/` (32 PE *_core + 5 runtime + 5 DX + 9 Battle.net + 6 calling-conv + 2 launchers; some overlap)
  - 435/435 Win32 fn skeleton coverage (Track BP, 2026-05-05)
- 242 perf modules under `lib/perf/` across 104 unique families
  - 18+ families at 100% manjeom (Track BL/BM forensic)
- 14 `.roadmap.*` files (mk2 SSOT)
- 7 `.checkpoint.*` files (mk2 checkpoint mechanism); 1 met: `win32_50pct_coverage` (Track BR derivation guard)
- 2 tools under `tool/` (`roadmap_op.hexa` 14-subcmd anima parity broken intentional + `closure_orchestrator.hexa` weighted_threshold awareness)
- 8 tests under `tests/`
- 75 predecessor markers under `state/markers/airgenome_gamebox_*.marker` (preserved verbatim per own2 traceability)
- 4 hexa-lang upstream RFCs merged (rfc_012/_013/_014 PR #47 + rfc_015 PR #48)

### Posture

- **own1**: Wine 0 / hexa-only / DRM bypass 0 / Warden bypass 0 / anti-cheat bypass 0 / anima clone 0
- **own2**: PASS/FAIL emit honest, PARTIAL/RETIRED_INTENTIONAL emit explicit
- **L1-L5 online expansion lock-in carry** (EULA / Warden / multi-year / D2R-single / parallel) per `.roadmap.battlenet`
- **License**: Apache-2.0
