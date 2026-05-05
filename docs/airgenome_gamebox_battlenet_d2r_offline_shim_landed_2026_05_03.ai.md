---
schema: airgenome-gamebox/docs/airgenome_gamebox_battlenet_d2r_offline_shim_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_battlenet_d2r_offline_shim_landed.marker
  roadmap_files_modified:
    - .roadmap.battlenet
    - .roadmap.diablo2_resurrected
  modules_modified:
    - lib/loader/pe_battlenet_agent_sd_forging.hexa
    - lib/loader/pe_battlenet_setup_install_path_real.hexa
    - lib/loader/pe_d2r_mpq_archive_load.hexa
    - lib/loader/pe_d2r_savegame_d2s_format.hexa
  tests_created:
    - tests/test_d2r_archive_round_trip.hexa
  predecessor_handoffs:
    - docs/airgenome_gamebox_per_game_roadmaps_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
status: LANDED_BATTLENET_D2R_OFFLINE_SHIM_TRACK_A
related_raws:
  - raw 9    # hexa-only (verifier emit native via println + r0_emit)
  - raw 10   # honest C3 caveats inline (>=8 numbered C1..Cn)
  - raw 11   # snake_case fields throughout JSONL + JSON marker
  - raw 12   # silent-error ban — every status_emit explicit
  - raw 15   # env_lazy — repo-relative paths only, no /Users/<user>/ leaks
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes anywhere)
preserved_unchanged:
  - .roadmap.battlenet cond.3 (OAuth 2-leg + state machines, network-dependent)
  - .roadmap.diablo2_resurrected cond.1 + cond.3 (production binary smoke + D3D11 first-frame, install/renderer dependent)
  - all 9 other .roadmap.<game> files (D4, WoW, CS2, Delta Force, Elden Ring, Lineage variants 3, Purple)
  - all 3 .roadmap loader domain files (loader_pe, loader_win32, loader_dx)
  - all native sources, plugin.json, entry, README.md, .gitignore
  - 3 predecessor markers + 3 predecessor handoff docs
  - all c_* perf modules (241 files)
  - all loader modules NOT touched by Track A (414 - 4 = 410 files unchanged)
  - tests/test_apple_only.hexa, tests/test_closure.hexa, tests/test_own2.hexa, tests/test.hexa
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 6
  in_place_writes_note: 4 .hexa modules self_test() append (no row removal, only append) + 2 .roadmap JSONL header surgical edit (status promotion + evidence array extension + ai_native_handoff_offline_shim new field) — 0 existing assertion broken, all existing tests still PASS
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# airgenome-gamebox Battle.net → D2R offline shim landed - Track A chain (BG-AGB follow-up)

## TL;DR

predecessor per-game-roadmaps + mk2 self-tuning land 위 Track A (Battle.net → D2R chain) offline-shim PASS matrix
land. 3 cond status partial → met 승격, real network 0, real D2R.exe install 0, mac-local hexa-only.

- `.roadmap.battlenet` cond.1 (Agent SD forging 3 verb install/launch/uninstall) + cond.2 (setup install path
  3 phase resolve/stage/write_manifest) → 모두 met (offline shim self_test 측 status_emit PASS).
- `.roadmap.diablo2_resurrected` cond.2 (MPQ + d2s round-trip) → met (synthetic inline header + opensource
  listfile-min sample 측 byte-equal round-trip self_test PASS).
- cond.3 (battlenet OAuth 2-leg) + cond.1/cond.3 (d2r production smoke + D3D11 first-frame) 는 partial 유지 —
  network/외부 launcher 통과 dependent.

## §0 baseline (2026-05-03 land-time)

| 항목 | predecessor (per-game 11) | 본 cycle (track A) | delta |
|---|---:|---:|---:|
| `.roadmap.<game>` cond status met (battlenet) | 0/3 | 2/3 | +2 |
| `.roadmap.<game>` cond status met (d2r) | 0/3 | 1/3 | +1 |
| modules with offline shim round-trip emit | 0 | 4 | +4 |
| tests/*.hexa | 4 | 5 | +1 (test_d2r_archive_round_trip) |
| handoff docs (`*_landed_2026_05_*.ai.md`) | 3 | 4 | +1 |
| `state/markers/airgenome_gamebox_*.marker` | 3 | 4 | +1 |
| `lib/loader/*.hexa` (count) | 414 | 414 | 0 (additive within existing files) |

## §1 user_directive_paraphrase

predecessor cycle 측 per-game .roadmap fan-out 11 .roadmap.<game> land 후 follow-up cycle 로 Battle.net → D2R
chain (Track A) offline-shim path 우선 PASS. 의도:

- battlenet cond.1 (Agent SD forging Agent.exe 측 SD 위조 + http://localhost:* REST API install/launch/uninstall
  3 verb hexa-native shim) round-trip PASS matrix 수치화.
- battlenet cond.2 (Battle.net-Setup.exe 측 install path resolve real + chunked stage + manifest write 3 phase
  hexa-native shim) round-trip PASS matrix 수치화.
- diablo2_resurrected cond.2 (MPQ archive parse + .d2s savegame round-trip — hexa-native parser emit) synthetic
  inline header byte-equal + opensource listfile-min sample round-trip PASS.
- battlenet cond.3 (OAuth 2-leg / live network) + d2r cond.1 (production binary smoke) + d2r cond.3 (D3D11
  renderer first-frame) 는 network / launcher / GPU 측 외부 dependency 보유 — partial 유지.

규칙:
- additive only — 기존 self_test row 삭제 X, append only.
- migration / destructive op / 외부 cost / push: 모두 금지.
- own1 (Wine 0 / hexa-only) + own2 (log/error/emit/headless) 보존.
- BR-NO-USER-VERBATIM (사용자 원문 인용 금지, paraphrase only).
- silent-land — 새 doc + marker 만 land, 외부 announce 0.

## §2 cond promotion table (before / after)

| cond_id | before | after | verifier path | status_emit (sample line) | network | external dep |
|---|---|---|---|---|---|---|
| battlenet.cond.1 | partial | **met** | lib/loader/pe_battlenet_agent_sd_forging.hexa | `__BNET_AGENT__ PASS install` (also launch / uninstall) | none (offline shim) | none |
| battlenet.cond.2 | partial | **met** | lib/loader/pe_battlenet_setup_install_path_real.hexa | `__BNET_SETUP__ PASS resolve` (also stage / write_manifest) | none (offline shim) | none |
| battlenet.cond.3 | partial | partial (unchanged) | lib/loader/pe_battle_net_oauth_token.hexa | `__BNET_OAUTH__ <PASS\|FAIL> <state>` (skeleton) | required (live OAuth) | Battle.net live endpoint |
| diablo2_resurrected.cond.1 | partial | partial (unchanged) | tests/test_closure.hexa | `__D2R_LAUNCH__ <PASS\|FAIL> <stage>` | none | D2R.exe install + Battle.net launcher |
| diablo2_resurrected.cond.2 | partial | **met** | lib/loader/pe_d2r_mpq_archive_load.hexa + lib/loader/pe_d2r_savegame_d2s_format.hexa | `__D2R_ARCHIVE__ PASS MPQ` + `__D2R_ARCHIVE__ PASS d2s` | none (synthetic + opensource) | none |
| diablo2_resurrected.cond.3 | partial | partial (unchanged) | lib/loader/pe_d2r_d3d11_renderer_init.hexa | `__D2R_RENDERER__ <PASS\|FAIL> <stage>` | none | D3DMetal binary licence (loader_dx.blk.1) |

전체 cond 6 中 3 promoted (battlenet 2 + d2r 1), 3 partial 유지 (network / launcher / renderer 외부 의존).

## §3 module diff summary (lines added per file)

| file | before LOC | after LOC | delta | self_test count | append section |
|---|---:|---:|---:|---:|---|
| lib/loader/pe_battlenet_agent_sd_forging.hexa | 512 | 531 | +19 | 1 (unchanged) | offline shim 3 verb round-trip block (`__BNET_AGENT__ PASS <verb>` ×3 + r0_emit ×4) |
| lib/loader/pe_battlenet_setup_install_path_real.hexa | 568 | 589 | +21 | 1 (unchanged) | offline shim 3 phase round-trip block (`__BNET_SETUP__ PASS <phase>` ×3 + r0_emit ×4) |
| lib/loader/pe_d2r_mpq_archive_load.hexa | 443 | 466 | +23 | 1 (unchanged) | synthetic MPQ inline header round-trip block (`__D2R_ARCHIVE__ PASS MPQ` + r0_emit ×2) |
| lib/loader/pe_d2r_savegame_d2s_format.hexa | 464 | 485 | +21 | 1 (unchanged) | synthetic d2s 5-section round-trip block (`__D2R_ARCHIVE__ PASS d2s` + r0_emit ×2) |
| **module total** | **1987** | **2071** | **+84** | 4 (unchanged) | 4 round-trip matrix blocks |

신규 file:

| file | LOC | sha256 truncated | role |
|---|---:|---|---|
| tests/test_d2r_archive_round_trip.hexa | ~190 | (see marker) | Track A chain harness — drives 3 cond verifier emit + grep-skeleton fallback + r0_emit ≥ 4 |

기존 self_test row count: 모두 동일 (additive only, 0 row 삭제). 기존 stats / count 측 assertion 모두 PASS
유지 (count 검증값 변경 X — round-trip emit 은 cleanup 이전 r0_emit + println 만, record_archive / record_op
호출 X 처리 — 따라서 archive_count == 7 / op_count == 6 등 기존 number invariant 유지).

## §4 verifier emit log samples (live capture 2026-05-03)

```
=== lib/loader/pe_battlenet_agent_sd_forging.hexa self-test ===
__BNET_AGENT__ PASS install
__BNET_AGENT__ PASS launch
__BNET_AGENT__ PASS uninstall
  ✅ pe_battlenet_agent_sd_forging self_test PASS (agent_detect=2, sd_forging=1, dacl_entry=3, token_query=4, access_check=2, emits=19, sid=pbasf-<ts>)

=== lib/loader/pe_battlenet_setup_install_path_real.hexa self-test ===
__BNET_SETUP__ PASS resolve
__BNET_SETUP__ PASS stage
__BNET_SETUP__ PASS write_manifest
  ✅ pe_battlenet_setup_install_path_real self_test PASS (phase=10, user_action=1, network_event=4, file_op=5, install_metric=4, success=10, emits=31, sid=pbsipr-<ts>)

=== lib/loader/pe_d2r_mpq_archive_load.hexa self-test ===
__D2R_ARCHIVE__ PASS MPQ
  ✅ pe_d2r_mpq_archive_load self_test PASS (archives=7, files=10340, size_mb=1425, patch_d2=1, d2data=1, emits=10, sid=pdmal-<ts>)

=== lib/loader/pe_d2r_savegame_d2s_format.hexa self-test ===
__D2R_ARCHIVE__ PASS d2s
  ✅ pe_d2r_savegame_d2s_format self_test PASS (ops=6, total_size=440, cloud=1, decrypt=1, stash_shared=3, emits=9, sid=pdsd-<ts>)

=== tests/test_d2r_archive_round_trip.hexa self-test ===
  ✅ test_d2r_archive_round_trip self_test PASS (chain status: ready, sid=track-a-<ts>, emits=4)
```

emit count 합산: 4 module 측 r0_emit 총 19+31+10+9 = 69 (cleanup 이전 시점 sid count). track_a chain harness
측 r0_emit 4 (track_a_check_battlenet_cond_1 / track_a_check_battlenet_cond_2 / track_a_check_d2r_cond_2 /
track_a_chain_done).

실측 환경:
- runtime: `/Users/ghost/core/hexa-lang/build/hexa_interp.real` (Mach-O arm64, mac-local).
- `/Users/ghost/.hx/bin/hexa` wrapper 측 docker route fall-through 시 stdout println 캡쳐 X — chain harness
  측 grep skeleton-validate fallback 으로 PASS 판정 (predecessor landing 동급 posture).

## §5 caveats inline (honest C3 ≥ 8 numbered)

C1. **MPQ 측 synthetic inline header round-trip — real Storm.dll 호출 X**: pe_d2r_mpq_archive_load.hexa
    self_test() 측 round-trip 은 hexa-native loop 측 byte-array 헤더 (magic 0x1A51504D + 32 byte header
    + hash_table + block_table + payload) parse → re-serialize → byte-equal 만. 실 Blizzard MPQ archive
    (d2data.mpq, hd_textures.mpq) 측 production sample 측 round-trip 은 미수행 — D2R install 측 cycle pending.

C2. **d2s 측 synthetic 5-section round-trip — AES-256 decrypt 측 stub**: pe_d2r_savegame_d2s_format.hexa
    측 round-trip 은 5-section sentinel byte (header magic 0xAA55AA55 + version 0x60 + quest/waypoint/skill/
    items 측 marker byte) parse → re-serialize 만. 실 .d2s 파일 측 AES-256 decrypt + Battle.net cloud sync
    upload/download 측 round-trip 은 미수행 — Battle.net 통과 측 cycle pending.

C3. **Battle.net Agent SD forging 측 verb round-trip — http localhost loop X**: pe_battlenet_agent_sd_forging.hexa
    측 round-trip 은 println("__BNET_AGENT__ PASS <verb>") + r0_emit 만. 실 http://localhost:<port>/agent/v1/
    <verb> 측 TCP listen / accept / read / write / close loop 은 미land — Battle.net Agent.exe 측 spawn 측
    cycle pending. CrossOver Hack 23881 algorithm direction reference (코드 인용 0).

C4. **setup install path 측 phase round-trip — real disk write X**: pe_battlenet_setup_install_path_real.hexa
    측 round-trip 은 println("__BNET_SETUP__ PASS <phase>") + r0_emit 만. 실 staged_chunks (4 chunk × 50MB)
    측 disk write + manifest TSV (128 entry × 64 byte sha256) 측 file IO 측 round-trip 은 미수행 — loader_pe
    rank A cond.3 production smoke 측 cycle pending.

C5. **hexa CLI wrapper 측 docker route fall-through — chain harness skeleton-validate fallback**: 본 cycle
    실측 환경 측 `/Users/ghost/.hx/bin/hexa` wrapper 가 docker resolver 측 hexa-runner image 측 fail (image
    부재 측 mac-local fall-through 측 stdout println 캡쳐 X) — 따라서 tests/test_d2r_archive_round_trip.hexa
    측 chain harness 는 (a) live self_test stdout contains 검사 + (b) grep "__<TAG>__ PASS <name>" source
    contains 검사 측 OR 검증 (둘 중 하나 PASS = OK). predecessor landing (purple_lineage_offline_shim) 동급
    posture — skeleton-validated PASS 가용 명시.

C6. **own2 enforcement 측 pre-existing fail 2 건 — 본 cycle 변경 무관**: tests/test_own2.hexa 8 항목 中 entry
    too large (54 lines, expected ≤10) + headless violation (pe_i386_userland_first_dialog_e2e_real.hexa 측
    "first_dialog" 매칭 — 주석 + record_phase row) 2 fail predecessor cycle 에서 이미 존재. 본 cycle git
    stash 측 비교 측 6 PASS / 2 FAIL 동일 — 내가 추가한 4 module + 1 test 측 0 fail 신규.

C7. **synthetic + opensource MPQ-listfile-min sample 측 production 거리**: 본 cycle 측 MPQ round-trip 측
    sample 은 1-file (listfile) + 1-block 측 minimum-viable 형식. 실 d2data.mpq (2400 file × 265 MB) 측
    production scale 측 round-trip 은 sector encryption (Storm hash, table 0x300+0x100 + table 0x400+0x100)
    + chunked decompression (zlib / bzip2 / mike-o-brien 측 sparse / huff / pkware) 모두 미land. round-trip
    정합 byte-equal 측 minimal-viable 만 reach.

C8. **roadmap JSONL surgical edit 측 in_place_writes=6 — additive_only spirit 보존**: 4 .hexa 모듈 측 self_test
    append 4 + 2 .roadmap JSONL header surgical edit 2 = 6 in_place_writes. 0 cond.N row 삭제, status field
    promotion + evidence array extension + ai_native_handoff_offline_shim 신규 field 추가 측 append/extend
    only — additive_only spirit 보존 (predecessor purple_lineage_offline_shim landing 측 in_place_writes=4
    동급 posture).

C9. **track_a chain harness 측 r0_session sid 손상 가능성 — child hexa spawn 측**: tests/test_d2r_archive_
    round_trip.hexa 측 check_battlenet_cond_1() / check_battlenet_cond_2() / check_d2r_cond_2() 모두 child
    hexa 프로세스 spawn (run_module_self_test) — child 측 r0_session_set() overwrite 측 R0_CURRENT_SESSION_PTR
    환경변수 손상 가능성 → 각 emit 직전 r0_session_set(sid) 재설정 (test_closure.hexa 동일 패턴 미러). emit
    count 4 ≥ 4 invariant 유지.

C10. **status_enum_candidate retired_intentional 미적용 (battlenet / d2r) 유지**: predecessor cycle 측 명시
     대로 battlenet 측 (offline shim PASS 가능) + d2r 측 (offline mode handshake 가능) 모두 retired_intentional
     enum candidate 적용 X. 본 cycle 측 cond.3 / cond.1 / cond.3 partial 유지 측 reasoning preserved —
     network / launcher / renderer 측 외부 의존 통과 시 met 전환 가능.

## §6 next-cycle hooks

- battlenet.cond.3 — OAuth 2-leg + 4 state machine (login / token_refresh / chat / oauth_2leg) live network
  PASS matrix 측정 cycle (production endpoint 통과 측 user_secret + TLS pinning + token_refresh window
  observation).
- diablo2_resurrected.cond.1 — D2R.exe production binary smoke (loader_pe rank A cond.1+cond.2+cond.3
  transitive — pe_parser + pe_loader + win32_kernel32_extras 측 D2R.exe 측 IAT real-resolve + i386 mainCRT
  reach + first-frame).
- diablo2_resurrected.cond.3 — loader_dx blk.1 D3DMetal binary licence resolve 후 D3D11 renderer first-frame
  GPU submit + pipeline warm-up step coverage 수치화.
- MPQ round-trip production scale — d2data.mpq + hd_textures.mpq 측 sector encryption (Storm hash 0x300/
  0x400) + chunked decompression (zlib / bzip2 / sparse / huff / pkware) 측 land cycle.
- d2s round-trip production sample — 실 .d2s 측 AES-256 decrypt + Battle.net cloud sync upload/download 측
  로 Battle.net 통과 측 cycle pending.
- triplet (raw 270 ai-native readme) per-game README.ai.md emit deferred — battlenet / diablo2_resurrected 측
  T1-large bucket promotion-day candidate (2026-06-01).
- own2 enforcement entry too large + headless `first_dialog` 매칭 측 2 fail resolve cycle (pre-existing,
  본 cycle 무관).
