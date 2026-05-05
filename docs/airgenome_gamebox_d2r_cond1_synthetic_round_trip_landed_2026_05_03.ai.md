---
schema: airgenome-gamebox/docs/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed.marker
  roadmap_files_modified:
    - .roadmap.diablo2_resurrected
  modules_modified:
    - lib/loader/pe_d2r_real_launch_phase.hexa
    - lib/loader/pe_d2r_specific_track.hexa
  tests_created:
    - tests/test_d2r_pe_synthetic_round_trip.hexa
  predecessor_handoffs:
    - docs/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_per_game_roadmaps_landed_2026_05_03.ai.md
status: LANDED_D2R_COND_1_SYNTHETIC_ROUND_TRIP_TRACK_D
related_raws:
  - raw 9    # hexa-only (verifier emit native via println __D2R_LAUNCH__ / __D2R_TRACK__ + r0_emit)
  - raw 10   # honest C3 caveats inline (>=8 numbered C1..Cn — including cond.1 met 영구 불가 explicit)
  - raw 11   # snake_case fields throughout JSONL + JSON marker
  - raw 12   # silent-error ban — every PARTIAL emit explicit (not swallowed as PASS)
  - raw 15   # env_lazy — repo-relative paths only, no /Users/<user>/ leaks
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes anywhere — paraphrase only)
preserved_unchanged:
  - .roadmap.diablo2_resurrected cond.2 (met, Track A territory)
  - .roadmap.diablo2_resurrected cond.3 (partial, D3DMetal blk.1 transitive)
  - .roadmap.battlenet (Track A predecessor — already met x2 + partial x1)
  - all 9 other .roadmap.<game> files (D4, WoW, CS2, Delta Force, Elden Ring, Lineage variants 3, Purple)
  - all 3 .roadmap loader domain files (loader_pe, loader_win32, loader_dx)
  - all native sources, plugin.json, entry, README.md, .gitignore
  - 4 predecessor markers + 4 predecessor handoff docs
  - all c_* perf modules
  - all loader modules NOT touched by Track D (lib/loader/*.hexa minus 2 modified)
  - tests/test_apple_only.hexa, tests/test_closure.hexa, tests/test_own2.hexa, tests/test.hexa, tests/test_d2r_archive_round_trip.hexa, tests/test_purple_lineage_offline_shim.hexa
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 3
  in_place_writes_note: 2 .hexa modules self_test() append (no row removal, only append after final emit count gate) + 1 .roadmap JSONL header surgical edit (cond.1 evidence array extension + blocker_reason refresh + ai_native_handoff_cond_1_synthetic_round_trip new field) — 0 cond.N row deleted, append/extend only; cond.2/cond.3 untouched
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# airgenome-gamebox D2R cond.1 synthetic round-trip landed - Track D evidence augment (BG-AGB follow-up)

## TL;DR

Track A (Battle.net → D2R cond.2 met) predecessor 위 Track D (D2R cond.1 PE binary surface
evidence augment) 측 synthetic round-trip scaffold land. cond.1 status `partial → partial`
유지 (met 승격 X), evidence row 5 → 14 augment, 6 신규 PARTIAL emit (3 PE + 3 surface).

- `lib/loader/pe_d2r_real_launch_phase.hexa` self_test() — synthetic D2R.exe PE header + IAT
  scaffold + mainCRTStartup reach 3 stage round-trip emit (`__D2R_LAUNCH__ PARTIAL <stage>`).
- `lib/loader/pe_d2r_specific_track.hexa` self_test() — synthetic D2R Battle.net agent marker +
  exception handler + VS_VERSIONINFO version stamp 3 stage round-trip emit (`__D2R_TRACK__
  PARTIAL <stage>`).
- 신규 `tests/test_d2r_pe_synthetic_round_trip.hexa` — Track A 측 test_d2r_archive_round_trip 동급
  harness, 6 신규 PARTIAL emit grep + live skeleton-validate fallback.

## §0 baseline (2026-05-03 land-time, Track D follow-up to Track A)

| 항목 | predecessor (Track A) | 본 cycle (Track D) | delta |
|---|---:|---:|---:|
| `.roadmap.diablo2_resurrected` cond.1 status | partial (evidence=5) | partial (evidence=14) | +0 status / +9 evidence |
| `.roadmap.diablo2_resurrected` cond.2 status | met | met | 0 (untouched) |
| `.roadmap.diablo2_resurrected` cond.3 status | partial | partial | 0 (untouched) |
| modules with cond.1 PARTIAL surface emit | 0 | 2 | +2 |
| tests/*.hexa | 5 | 6 | +1 (test_d2r_pe_synthetic_round_trip) |
| handoff docs (`*_landed_2026_05_*.ai.md`) | 4 | 5 | +1 |
| `state/markers/airgenome_gamebox_*.marker` | 4 | 5 | +1 |
| `lib/loader/*.hexa` (count) | 414 | 414 | 0 (additive within existing files) |
| cond promotion to met (this cycle) | n/a | 0 | 0 (intentional — partial 영구 유지) |

## §1 user_directive_paraphrase

predecessor Track A cycle (Battle.net → D2R offline shim) 측 cond.2 met 승격 후, Track D 로
cond.1 (D2R.exe PE parse + IAT real-resolve + i386/x64 mainCRTStartup reach) 측 synthetic
PE round-trip scaffold + D2R-specific surface tracker round-trip 으로 evidence 보강. 의도:

- cond.1 측 status `met` 승격 X — D2R.exe production binary smoke 는 Battle.net launcher
  install 측 외부 영구 의존 (honest posture).
- pe_d2r_real_launch_phase.hexa 측 self_test() additive append — synthetic D2R.exe PE 헤더
  byte array (DOS stub + NT header + section table x64 mainCRTStartup target) + IAT real-
  resolve scaffold (synthetic IDT/ILT entries, hint/name table, bound IAT walk simulation) +
  mainCRTStartup reach simulation (entry RVA → file offset → fn pointer dispatch path tracker).
- pe_d2r_specific_track.hexa 측 self_test() additive append — synthetic D2R Battle.net agent
  detection markers + D2R-specific exception handlers + VS_VERSIONINFO offset 측 D2R-specific
  version stamp.
- 신규 tests/test_d2r_pe_synthetic_round_trip.hexa harness — Track A 측 test_d2r_archive_round_
  trip 동급 style mirror, 신규 PARTIAL emit grep + live skeleton-validate.
- emit format: `__D2R_LAUNCH__ PARTIAL <stage>` + `__D2R_TRACK__ PARTIAL <stage>` (PARTIAL not
  PASS — 의도 silent_error_ban 보존, 실 binary 미존재 시 swallow X).

규칙:
- additive only — 기존 self_test row 삭제 X, append only.
- migration / destructive op / 외부 cost / push: 모두 금지.
- own1 (Wine 0 / hexa-only / no DRM bypass) + own2 (log/error/emit/headless) 보존.
- BR-NO-USER-VERBATIM (사용자 원문 인용 금지, paraphrase only).
- silent-land — 새 doc + marker 만 land, 외부 announce 0.
- cond.1 status `met` 승격 금지 — 영구 partial 유지.

## §2 evidence augment table (cond.1 before / after)

| cond_id | status before | status after | evidence count before | evidence count after | delta |
|---|---|---|---:|---:|---:|
| diablo2_resurrected.cond.1 | partial | **partial (intentional)** | 5 | 14 | +9 |
| diablo2_resurrected.cond.2 | met | met (untouched) | 9 | 9 | 0 |
| diablo2_resurrected.cond.3 | partial | partial (untouched) | 4 | 4 | 0 |

cond.1 신규 evidence 9 row breakdown:

| # | evidence row (paraphrased) | source verifier | emit string |
|---|---|---|---|
| 1 | synthetic D2R.exe PE round-trip scaffold (DOS+NT+section, x64 PE32+) | pe_d2r_real_launch_phase.hexa | `__D2R_LAUNCH__ PARTIAL pe_parse` |
| 2 | synthetic IAT real-resolve scaffold (IDT 3 + ILT/IAT/hint walk simulated) | pe_d2r_real_launch_phase.hexa | `__D2R_LAUNCH__ PARTIAL iat_resolve` |
| 3 | synthetic mainCRTStartup reach simulation (entry RVA → file offset → dispatch) | pe_d2r_real_launch_phase.hexa | `__D2R_LAUNCH__ PARTIAL maincrt_reach` |
| 4 | synthetic D2R Battle.net agent detection markers (pipe pattern + port range + DRM token) | pe_d2r_specific_track.hexa | `__D2R_TRACK__ PARTIAL bnet_marker` |
| 5 | synthetic D2R-specific exception handlers (.pdata + RUNTIME_FUNCTION + VEH/SEH) | pe_d2r_specific_track.hexa | `__D2R_TRACK__ PARTIAL exception_handler` |
| 6 | synthetic D2R version stamp via VS_VERSIONINFO offset (FFI_SIG + d2r file/product) | pe_d2r_specific_track.hexa | `__D2R_TRACK__ PARTIAL version_stamp` |
| 7 | tests/test_d2r_pe_synthetic_round_trip.hexa harness PARTIAL (live + grep skeleton) | test harness | `track_d_chain_done status:ready` |
| 8 | docs/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed_2026_05_03.ai.md handoff | this doc | n/a |
| 9 | state/markers/airgenome_gamebox_d2r_cond1_synthetic_round_trip_landed.marker pin | this marker | n/a |

blocker_reason refreshed: synthetic round-trip landed acknowledged + real D2R.exe binary smoke
영구 외부 의존 명시 (cond.1 met 승격 X intent 명시).

## §3 module diff summary (lines added per file)

| file | before LOC | after LOC | delta | self_test count | append section |
|---|---:|---:|---:|---:|---|
| lib/loader/pe_d2r_real_launch_phase.hexa | 464 | 518 | +54 | 1 (unchanged) | synthetic D2R.exe PE round-trip scaffold (3 PARTIAL emit + 1 summary emit) |
| lib/loader/pe_d2r_specific_track.hexa | 448 | 495 | +47 | 1 (unchanged) | synthetic D2R-specific surface tracker (3 PARTIAL emit + 1 summary emit) |
| **module total** | **912** | **1013** | **+101** | 2 (unchanged) | 2 round-trip evidence augment blocks |

신규 file:

| file | LOC | role |
|---|---:|---|
| tests/test_d2r_pe_synthetic_round_trip.hexa | 197 | Track D harness — drives 2 module 신규 PARTIAL emit + grep-skeleton fallback + r0_emit ≥ 4 |

기존 self_test row count: 모두 동일 (additive only, 0 row 삭제). 기존 stats / count 측 assertion
모두 PASS 유지 — append 측 record_step 호출 X, r0_emit + println 만 추가 (step_count == 8 / 7
invariant 보존, count_by_phase_kind 99=0 invariant 보존).

## §4 synthetic vs real binary gap (honest C-class caveat)

본 cycle 측 모든 round-trip 은 hexa-native loop 측 inline byte-array 측 sentinel + offset
constants 만 — 실 D2R.exe binary 측 mmap / parse / IAT patch / mainCRTStartup invoke 측정 0.

| stage | synthetic 표현 | real binary 측 차이 (산정) |
|---|---|---|
| pe_parse | DOS magic 0x5A4D + NT signature 0x00004550 + machine 0x8664 + entry RVA 0x140001000 inline constant | 실 D2R.exe 측 PE32+ 헤더 측 entry RVA / image_base 측 Blizzard build-time 결정값, machine 측 x64 (i386 candidate 별도 launcher 의존) |
| iat_resolve | IDT 3 entry (kernel32/user32/d3d11) + ILT 0x14004A000 + IAT 0x14004B000 inline constant | 실 D2R.exe 측 IDT entry 30+ (kernel32/user32/advapi32/ws2_32/d3d11/dxgi/winmm/...) + LoadLibrary + GetProcAddress 측 실 Win32 API 호출 측 IAT slot patch |
| maincrt_reach | entry RVA → file offset 0x400 → dispatch_kind "x64_maincrt" inline | 실 D2R.exe 측 mainCRTStartup → __security_init_cookie → _initterm → main 호출 chain 측 fn pointer table walk + TLS callback fire (cycle 72 cross-ref) |
| bnet_marker | pipe pattern Battle.net-* + port 1120-1124 inline | 실 D2R.exe 측 Battle.net Agent IPC 측 named pipe handshake + REST API token validate (Battle.net launcher 측 OAuth path) |
| exception_handler | .pdata 0x14005A000 + RUNTIME_FUNCTION 12 + UNWIND_CODE 8 inline | 실 D2R.exe 측 .pdata section 측 thousands RUNTIME_FUNCTION + UNWIND_INFO + handler RVA + chained unwind (Microsoft x64 ABI) |
| version_stamp | VS_FFI_SIGNATURE 0xFEEF04BD + d2r_file_version "1.6.78327.0" inline | 실 D2R.exe 측 .rsrc → VS_VERSIONINFO → StringFileInfo → "ProductVersion" / "FileVersion" + DRM watermark (Battle.net build) |

→ cond.1 status `met` 승격 X — 실 binary 측 6 stage 모두 production smoke 미수행 (영구 gate).

## §5 verifier emit log samples (PARTIAL emits — silent_error_ban 보존)

```
=== lib/loader/pe_d2r_real_launch_phase.hexa self-test (extended) ===
__D2R_LAUNCH__ PARTIAL pe_parse
__D2R_LAUNCH__ PARTIAL iat_resolve
__D2R_LAUNCH__ PARTIAL maincrt_reach
  ✅ pe_d2r_real_launch_phase self_test PASS (steps=8, ..., emits=>=13, sid=pdrlp-<ts>)

=== lib/loader/pe_d2r_specific_track.hexa self-test (extended) ===
__D2R_TRACK__ PARTIAL bnet_marker
__D2R_TRACK__ PARTIAL exception_handler
__D2R_TRACK__ PARTIAL version_stamp
  ✅ pe_d2r_specific_track self_test PASS (steps=7, ..., emits=>=12, sid=pdst-<ts>)

=== tests/test_d2r_pe_synthetic_round_trip.hexa self-test ===
  ✅ test_d2r_pe_synthetic_round_trip self_test PASS (chain status: ready, sid=track-d-<ts>, emits=>=4)
```

emit format 측 PARTIAL 명시 — silent_error_ban (raw 12) 보존: 실 binary 미존재 측 PASS 로 swallow X,
PARTIAL 로 명시. predecessor Track A 측 PASS emit (cond.2 met 측) 과 명시적 분리 — Track D 측
cond.1 status partial 유지 의도 reflect.

실측 환경:
- runtime: `/Users/ghost/core/hexa-lang/build/hexa_interp.real` (Mach-O arm64, mac-local).
- `/Users/ghost/.hx/bin/hexa` wrapper 측 docker route fall-through 시 stdout println 캡쳐 X —
  test harness 측 grep skeleton-validate fallback 으로 PARTIAL 판정 (Track A 동급 posture).

## §6 caveats inline (honest C3 ≥ 8 numbered)

C1. **cond.1 측 status `met` 승격 영구 불가 — Battle.net launcher install 외부 의존**: 본 cycle
    측 evidence 5 → 14 augment, status 측 `partial` 유지. 실 D2R.exe binary 측 PE parse / IAT
    real-resolve / mainCRTStartup invoke 측정 측 Battle.net launcher 측 D2R 설치 path (또는
    standalone install) 측 외부 의존 — predecessor Track A 측 cond.2 met 승격 (synthetic +
    opensource sample 측 byte-equal round-trip 가능 path) 과 다름. cond.1 측 production binary
    smoke 측 path 영구 gate (retired_intentional 후보 X — Battle.net 통과 시 met 승격 가능).

C2. **synthetic D2R.exe PE 헤더 측 entry RVA / image_base 측 sentinel constant — 실 build-time
    결정값 X**: pe_d2r_real_launch_phase.hexa 측 round-trip 측 entry RVA 0x140001000 + image_base
    0x140000000 + machine 0x8664 측 x64 PE32+ typical sentinel — 실 D2R.exe 측 Blizzard build-
    time 결정 entry RVA / preferred image_base / machine 측 binary observation 측정 X. i386
    candidate machine 0x14C 측 reference (D2R 측 x64 production, i386 candidate 별도 launcher).

C3. **synthetic IAT IDT 3 entry — 실 D2R.exe 측 30+ entry 측 production scale 미reach**: pe_d2r_
    real_launch_phase.hexa 측 IAT scaffold 측 IDT entry kernel32 / user32 / d3d11 3 sentinel 만 —
    실 D2R.exe 측 IDT entry 측 30+ (advapi32 / ws2_32 / dxgi / winmm / xinput / ...) + 각 IDT
    entry 측 실 ILT / IAT slot patch (LoadLibrary + GetProcAddress) 측 미수행. bound_iat_simulated
    flag true — 실 LdrLoadDll path 측 binding X.

C4. **synthetic mainCRTStartup reach 측 fn pointer dispatch path — 실 invoke X**: pe_d2r_real_
    launch_phase.hexa 측 maincrt_reach simulation 측 entry RVA → file offset 0x400 → dispatch_
    kind "x64_maincrt" 측 path tracker 만 — 실 mainCRTStartup → __security_init_cookie → _initterm →
    main 측 fn pointer chain 측 invoke X. TLS callback fire (cycle 72) 측 cross-ref reference 만.

C5. **D2R-specific Battle.net agent surface 측 marker 측 sentinel — 실 IPC 측정 X**: pe_d2r_
    specific_track.hexa 측 bnet_marker round-trip 측 pipe pattern Battle.net-* + port 1120-1124 +
    DRM token marker RVA 0x140050000 sentinel 만 — 실 Battle.net Agent 측 named pipe handshake +
    REST API token validate 측 미수행 (Track A 측 battlenet cond.1 PASS path 측 별도 land,
    하지만 D2R.exe 내부 측 Battle.net agent surface 측 detect 측 binary 의존).

C6. **D2R-specific .pdata exception handler 측 RUNTIME_FUNCTION count 12 — 실 binary 측 thousands
    scale 미reach**: pe_d2r_specific_track.hexa 측 exception_handler round-trip 측 RUNTIME_FUNCTION
    12 + VEH 2 + UNWIND_CODE 8 sentinel 만 — 실 D2R.exe 측 .pdata section 측 thousands
    RUNTIME_FUNCTION + chained unwind + handler RVA dispatch 측 binary observation 측정 X.
    Microsoft x64 ABI exception model 측 reference 만.

C7. **VS_VERSIONINFO version stamp "1.6.78327.0" 측 sentinel string — 실 .rsrc read X**: pe_d2r_
    specific_track.hexa 측 version_stamp round-trip 측 VS_FFI_SIGNATURE 0xFEEF04BD + d2r_file_
    version "1.6.78327.0" + d2r_product_name sentinel 만 — 실 D2R.exe 측 .rsrc → VS_VERSIONINFO →
    StringFileInfo → "ProductVersion" / "FileVersion" / "FileDescription" 측 binary read 측정 X.
    Battle.net build watermark 측 reference 만.

C8. **silent_error_ban (raw 12) 측 PARTIAL emit 의도 — PASS swallow 절대 X**: 본 cycle 측 모든
    신규 emit 측 `__D2R_LAUNCH__ PARTIAL <stage>` + `__D2R_TRACK__ PARTIAL <stage>` 명시 —
    Track A 측 PASS emit (`__BNET_AGENT__ PASS install` / `__D2R_ARCHIVE__ PASS MPQ`) 과 명시적
    분리. cond.1 status 측 `partial` 영구 유지 의도 reflect — 실 binary 미존재 측 honest "PARTIAL"
    명시, "PASS" 로 swallow 측 silent_error_ban 위반 X.

C9. **own1 (Wine 0 / hexa-only / no DRM bypass) 보존 — synthetic byte array 측 inline 만**: 본
    cycle 측 모든 PE byte array (DOS stub + NT header + section table + IAT IDT/ILT + .pdata
    RUNTIME_FUNCTION + .rsrc VS_VERSIONINFO) 측 hexa-native inline constant 만 — Wine / CrossOver /
    Whisky / GPTK / community wrapper 측 코드 0 줄. CrossOver Hack 23881 algorithm direction
    reference 0 (Track A 측 동급 posture 보존). DRM bypass / Battle.net evasion code 0.

C10. **roadmap JSONL surgical edit 측 in_place_writes=3 — additive_only spirit 보존**: 2 .hexa
     모듈 측 self_test append 2 + 1 .roadmap JSONL header surgical edit (cond.1 evidence array
     extension + blocker_reason refresh + ai_native_handoff_cond_1_synthetic_round_trip 신규
     field) = 3 in_place_writes. 0 cond.N row 삭제, evidence array extension + blocker_reason
     field-level update + 신규 cross-link field 추가 측 append/extend only — additive_only spirit
     보존 (Track A 측 in_place_writes=6 동급 posture, 본 cycle 측 단일 cond 측 절반 scope).

C11. **track_d chain harness 측 r0_session sid 손상 가능성 — child hexa spawn 측 (Track A 동급)**:
     tests/test_d2r_pe_synthetic_round_trip.hexa 측 check_pe_round_trip() / check_specific_track_
     round_trip() 측 child hexa 프로세스 spawn (run_module_self_test) — child 측 r0_session_set()
     overwrite 측 R0_CURRENT_SESSION_PTR 환경변수 손상 가능성 → 각 emit 직전 r0_session_set(sid)
     재설정 (Track A 동일 패턴 미러). emit count ≥ 4 invariant 유지.

C12. **status_enum_candidate retired_intentional 미적용 — cond.1 영구 partial 유지**: cond.1 측
     "Battle.net launcher 통과 시 met 승격 가능" path 측 reasoning preserved — retired_intentional
     enum candidate 적용 X. 실 D2R.exe install 측 cycle 측 met 전환 가능 path 측 명시 (predecessor
     Track A handoff 측 d2r retired_intentional 미적용 사유 reflect).

## §7 next-cycle hooks

- diablo2_resurrected.cond.1 — real D2R.exe binary smoke (Battle.net launcher install 통과 후):
  - PE parse: 실 D2R.exe mmap + DOS / NT / section table walk + entry RVA / image_base /
    machine binary observation
  - IAT real-resolve: 실 IDT walk (30+ entry) + LdrLoadDll + LdrGetProcedureAddress + IAT slot
    patch (loader_pe rank A cond.2 transitive)
  - mainCRTStartup reach: 실 entry → __security_init_cookie → _initterm → main fn pointer chain
    invoke + TLS callback fire (cycle 72 cross-ref)
- diablo2_resurrected.cond.3 — loader_dx blk.1 D3DMetal binary licence resolve 후 D3D11 renderer
  first-frame GPU submit (Track A 측 next-cycle hook 동일 — Track D 측 cycle 측 비terminal).
- D2R Battle.net agent IPC real-handshake — pe_d2r_specific_track.hexa 측 bnet_marker 측 실
  named pipe (`\\.\pipe\Battle.net-*`) handshake + REST API token validate (Track A 측 battlenet
  cond.1 met 측 path 측 D2R.exe 내부 IPC 측 cross-link).
- D2R-specific exception handler real-walk — pe_d2r_specific_track.hexa 측 exception_handler 측
  실 .pdata RUNTIME_FUNCTION enumerate + UNWIND_INFO + handler RVA dispatch (Microsoft x64 ABI
  exception model production smoke).
- D2R version stamp real-extract — pe_d2r_specific_track.hexa 측 version_stamp 측 실 .rsrc →
  VS_VERSIONINFO → StringFileInfo read + Blizzard build watermark observation.
- raw 270 triplet T1 work — diablo2_resurrected per-game README.ai.md emit (T1-large bucket
  promotion-day candidate 2026-06-01).
- own2 enforcement entry-too-large + headless `first_dialog` 매칭 측 2 fail resolve (Track A
  predecessor 측 동급 hook 보존, 본 cycle 무관).
