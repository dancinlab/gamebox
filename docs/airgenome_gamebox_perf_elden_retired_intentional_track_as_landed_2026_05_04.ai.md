# airgenome-gamebox perf_elden retired_intentional Track AS landed (2026-05-04)

## §1 Track AS scope summary

Track AS = c_elden_*.hexa retired_intentional posture annotation cycle (first
Elden Ring extension). Mirrors Track V (D4 7) / Track AJ (D4 6) / Track AL (D4
6) annotation pattern, but adapted for Elden Ring EAC anti-tamper class blocker
(elden_ring.blk.1, status_enum_adopted=retired_intentional, adopted_iso=2026-
05-03 via Track C 4-game retired_intentional adoption land).

6 of 10 c_elden_*.hexa modules annotated additively in this cycle:

1. `lib/perf/c_elden_dx12_first_frame.hexa` (cond.2 engine surface)
2. `lib/perf/c_elden_dx12_pso_warm.hexa` (cond.2 engine surface)
3. `lib/perf/c_elden_fromsoft_hks_script_cache.hexa` (cond.2 engine surface)
4. `lib/perf/c_elden_eac_offline_launch_probe.hexa` (cond.1 EAC surface)
5. `lib/perf/c_elden_eac_kernel_driver_avoid_track.hexa` (cond.1 EAC surface)
6. `lib/perf/c_elden_msix_container_extract.hexa` (cond.3 distribution surface)

Annotation format: r0_emit('c_elden_<domain>_retired_intentional', JSON with
verdict=RETIRED_INTENTIONAL + posture_kind=block_acknowledged_pass_equivalent +
cond=elden_ring.cond.<n>) + println('__ELDEN_<DOMAIN>__ RETIRED_INTENTIONAL
<stage>') marker line. blocker_class = eac_anti_tamper. Each module
self_test() final cleanup_*() 직전 +27-29 LOC additive insertion.

## §2 Policy compliance

- migration: forbidden / additive_only / destructive 0 / cost 0
- mac-local / cap 90min / silent_land / BR_NO_USER_VERBATIM / friendly_preset
- own1: Wine 0 / CrossOver 0 / GPTK 0 / DRM bypass 0 / EAC bypass 0 / anti-
  tamper bypass 0 — honest natural absence acknowledgment only
- own2: r0_emit('c_elden_<domain>_retired_intentional', ...) + println marker
  line — silent_swallow X — verdict explicit RETIRED_INTENTIONAL kind (NOT
  PASS / NOT PARTIAL / NOT FAIL — distinct posture kind)
- raw 9 / 11 / 12 / 15 / 175 정합

## §3 Files modified (6 perf modules + 1 roadmap)

### §3.1 lib/perf/c_elden_dx12_first_frame.hexa

- loc_before: 187, loc_after: 216, loc_delta: +29
- diff_kind: pure_insertion
- marker_line: `__ELDEN_DX12_FIRST_FRAME__ RETIRED_INTENTIONAL elden_ring_engine_synthetic`
- sha256: 4f7c94b8a0d3120eaf353803b9f191775d5bd99995e70428b0222adc5a8658bb
- cond_ref: elden_ring.cond.2

### §3.2 lib/perf/c_elden_dx12_pso_warm.hexa

- loc_before: 187, loc_after: 214, loc_delta: +27
- diff_kind: pure_insertion
- marker_line: `__ELDEN_DX12_PSO_WARM__ RETIRED_INTENTIONAL elden_ring_engine_synthetic`
- sha256: e04fde15466958f11c1a6ae88206ea4c362da8e0b1f7d9ab50be3a517d1d09af
- cond_ref: elden_ring.cond.2

### §3.3 lib/perf/c_elden_fromsoft_hks_script_cache.hexa

- loc_before: 186, loc_after: 213, loc_delta: +27
- diff_kind: pure_insertion
- marker_line: `__ELDEN_HKS_SCRIPT_CACHE__ RETIRED_INTENTIONAL elden_ring_engine_synthetic`
- sha256: c0e83e6910881818c35be4abce665b37b5a43284b93c526677dd9613dde448ce
- cond_ref: elden_ring.cond.2

### §3.4 lib/perf/c_elden_eac_offline_launch_probe.hexa

- loc_before: 187, loc_after: 214, loc_delta: +27
- diff_kind: pure_insertion
- marker_line: `__ELDEN_EAC_OFFLINE_PROBE__ RETIRED_INTENTIONAL elden_ring_eac_synthetic`
- sha256: c0e7c801d0b32489ac787c01b92b52ffad450005e4e166d61571eae4f20fd309
- cond_ref: elden_ring.cond.1

### §3.5 lib/perf/c_elden_eac_kernel_driver_avoid_track.hexa

- loc_before: 177, loc_after: 205, loc_delta: +28
- diff_kind: pure_insertion
- marker_line: `__ELDEN_EAC_KERNEL_DRIVER_AVOID__ RETIRED_INTENTIONAL elden_ring_eac_synthetic`
- sha256: 7c8f43866181e55ca70dc89da71bccd165a87dd7d5b51465ea1ba5d34f648bd0
- cond_ref: elden_ring.cond.1

### §3.6 lib/perf/c_elden_msix_container_extract.hexa

- loc_before: 184, loc_after: 211, loc_delta: +27
- diff_kind: pure_insertion
- marker_line: `__ELDEN_MSIX_CONTAINER__ RETIRED_INTENTIONAL elden_ring_dist_synthetic`
- sha256: 1003f22d7faa0778e98a27d4ae05848ed89d39b2b730aabc3d2cee4d7a141b7c
- cond_ref: elden_ring.cond.3

### §3.7 .roadmap.elden_ring

- edit_kind: additive_field_level_cond_evidence (cond.1 + cond.2 + cond.3)
- bytes_before: 6615, bytes_after: 7639, bytes_delta: +1024
- sha256_before: 049e09948a51ad5ef2450c8f80141aaa9e4d48ad9d0ebfd68838025121be4e0a
- sha256_after: c921daad2b3af19e57e1fb1aacc685fb06550f6958c143a4a1747a0fbddfa75a
- cond.1 evidence: +2 rows (eac_offline_launch_probe + eac_kernel_driver_avoid_track annotation)
- cond.2 evidence: +3 rows (dx12_first_frame + dx12_pso_warm + fromsoft_hks_script_cache annotation)
- cond.3 evidence: +1 row (msix_container_extract annotation)
- cond.1 / cond.2 / cond.3 status: partial (영구 유지)
- blk.1 fields: verbatim preserved (Track C territory, modification 0건)

## §4 Annotation schema (synthetic posture row column header)

Each module emits a column-header-conformant retired_intentional posture row
inside self_test() before cleanup_*():

```
domain                "<elden_domain>"
posture               "retired_intentional"
production_smoke      "no_attempt"
drm_bypass            0          (own1 Wine 0)
ac_bypass             0          (own1 honest acknowledgment)
anti_tamper_bypass    0          (EAC anti-tamper boundary)
blocker_ref           "elden_ring.blk.1"
blocker_class         "eac_anti_tamper"
adoption_iso          "2026-05-03"   (Track C 4-game retired_intentional)
cond                  "elden_ring.cond.<n>"
verdict               "RETIRED_INTENTIONAL"
posture_kind          "block_acknowledged_pass_equivalent"
```

## §5 Cumulative coverage

- Track AS modules: 6
- c_elden_* modules total: 10
- cumulative_c_elden_annotated_after_track_as: 6/10 (60%)
- remaining_untouched: 4 (msstore_license_stub + offline_save_compat +
  online_entry_block + pe64_loader_probe) — 별도 future cycle scope
- retired_intentional emit occurrences this cycle: 12 (6 modules × 2 emits per
  module: 1 r0_emit verdict=RETIRED_INTENTIONAL + 1 println marker line)

## §6 Predecessor chain

| Track | Marker | Relation |
|---|---|---|
| Track C | `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` | blk.1 retired_intentional formal adoption (status_enum_adopted, adopted_iso=2026-05-03, retired_intentional_posture=block_acknowledged_pass_equivalent). Track AS 측 posture annotation 측 본 adoption 정합 reflect. |
| Track P | `state/markers/airgenome_gamebox_battlenet_bypass_own1_disambiguation_landed.marker` | bg_agb_rank_b_c_unblock_predecessor — perf_game_elden_ring own1 review gate 통과 (EAC anti-tamper retired_intentional + honest natural absence). |
| Track V | `state/markers/airgenome_gamebox_perf_game_d4_retired_intentional_posture_landed.marker` | schema source (D4 첫 7-module retired_intentional posture annotation cycle, sister game). |
| Track AL | `state/markers/airgenome_gamebox_perf_game_d4_retired_intentional_extension_track_al_landed.marker` | schema mirror predecessor (D4 third 6-module extension cycle, identical synthetic posture row + r0_emit + println marker line schema). |
| Per-game roadmaps | `state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker` | fan_out_source — .roadmap.elden_ring 10 c_elden_* perf module enumeration source. |

## §7 Pattern reference

Reference module: `lib/perf/c_d4_dxr_first_frame_trace.hexa` (Track AL
predecessor). Identical annotation block schema — synthetic posture row column
header (domain / posture / production_smoke / drm_bypass / ac_bypass /
anti_tamper_bypass / blocker_ref / adoption_iso) + r0_emit('c_elden_<domain>_
retired_intentional', JSON with verdict=RETIRED_INTENTIONAL +
posture_kind=block_acknowledged_pass_equivalent + cond=elden_ring.cond.<n>) +
println('__ELDEN_<DOMAIN>__ RETIRED_INTENTIONAL <stage>'). Track AS 6 modules
측 본 schema 동일 적용 (D4 always_online_evade=0 → Elden Ring ac_bypass=0 +
anti_tamper_bypass=0 column adapted).

## §8 Verification

- 6 modules show LOC growth (+27-29 each, pure_insertion only)
- roadmap JSONL parses (1 entry, blocker[0]=elden_ring.blk.1 verbatim preserved)
- marker JSON parses
- sha256 pinned (each modified file)
- cond.1 / cond.2 / cond.3 status partial 영구 유지
- blk.1 fields modification 0건 (Track C territory)
- 4 c_elden_* modules untouched (msstore_license_stub + offline_save_compat +
  online_entry_block + pe64_loader_probe) — file scope strict
- parallel BG territory (Track AM = .roadmap.loader_win32, parallel game cycles)
  modification 0건
- lib/loader/ + native/ + tool/ + tests/ modification 0건

## §9 Honest C3 caveats

- C1 본 cycle 측 6 modules production live game state 측정 시도 0건 — synthetic
  state row append + RETIRED_INTENTIONAL emit only. Elden Ring live session
  진입 X (EAC kernel driver Apple Silicon load 불가, production binary smoke
  X). honest natural absence path acknowledgment only.
- C2 cond.1 / cond.2 / cond.3 status partial 영구 유지 — retired_intentional
  posture per blk.1 영구 lock. cond promotion 시도 X.
- C3 blk.1 fields (status_enum_adopted / adopted_iso / retired_intentional_
  posture / adoption_evidence / blocker_class / status / desc / resolution_path
  / blocker_reason) 측 본 cycle 측 modification 0건 — Track C territory
  preserved verbatim.
- C4 4 c_elden_* untouched modules (msstore_license_stub / offline_save_compat
  / online_entry_block / pe64_loader_probe) 측 본 cycle 측 retired_intentional
  posture annotation 미적용 — 별도 future cycle scope. 누적 coverage 6/10
  (60%).
- C5 본 cycle 측 parallel BG territory (Track AM = .roadmap.loader_win32 /
  parallel game retired_intentional cycles) 측 modification 0건 — file scope
  분리. lib/loader/ + native/ + tool/ + tests/ 측 modification 0건.
- C6 hexa CLI runtime 측 본 cycle 측 invocation X — schema_validated_only
  (lib/perf/ 측 본 cycle 측 6 file additive append only, content-level
  execution X). self_test() runtime verification 측 별도 future cycle.
- C7 RETIRED_INTENTIONAL emit kind 측 PASS / PARTIAL / FAIL 와 명확 분리
  (distinct posture kind) — silent_swallow X / verdict explicit. Track C +
  Track V + Track AJ + Track AL + Track J Warden DETECTION_TRACKED pattern
  mirror.
- C8 EAC bypass / anti-tamper circumvention 측 본 cycle 측 시도 0건 — own1
  honest natural absence acknowledgment. EAC kernel driver (EasyAntiCheat.sys)
  Apple Silicon load 불가 = 단순 환경 부재 (FromSoft 정식 mechanism `start_
  game_in_offline_mode.exe` 와 동등 효과 honest detect, no bypass).
- C9 hexa upstream directive — c_elden_*.hexa 측 hexa-lang upstream hits 0건
  탐지 → docs/hexa_upstream_track_as_<topic>.md emit X. docs/HEXA_UPSTREAM.md
  modification 0건.
- C10 본 cycle 측 git commit / push / branch 시도 0건 (silent-land) — operator
  level commit decision deferred. raw 175 BR_NO_USER_VERBATIM 정합.
- C11 prompt 측 5-6 selection range 측 6 modules selection (max range) — 4
  remaining (msstore_license_stub / offline_save_compat / online_entry_block /
  pe64_loader_probe) 측 future cycle 측 ~40% completion.

## §10 raw compliance

- raw 9 hexa-only: 6 module additive self_test append + 1 roadmap field-level
  cond evidence append + 1 NEW handoff + 1 NEW marker
- raw 10 honest c3: 11 caveats inline (C1-C11) >= 8 minimum threshold
- raw 11 snake_case: all field names snake_case
- raw 12 silent_error_ban: verdict explicit RETIRED_INTENTIONAL (NOT PASS / NOT
  PARTIAL / NOT FAIL); cond.1/2/3 status partial 명시; blk.1 retired_
  intentional 영구 lock 명시; honest natural absence emit posture explicit
- raw 15 env_lazy: all paths repo-relative
- raw 175 br_no_user_verbatim: 0 verbatim user quotes in any file

## §11 Next cycle actions

- 별도 cycle 측 4 c_elden_* remaining modules retired_intentional posture
  annotation extend (msstore_license_stub / offline_save_compat /
  online_entry_block / pe64_loader_probe) — final ~40%
- 별도 cycle 측 sister game (delta_force EAC kernel-AC, cs2 VAC user-mode AC)
  retired_intentional posture annotation extension
- 별도 cycle 측 closure_orchestrator level RETIRED_INTENTIONAL verdict
  aggregation across 4-game retired_intentional adopted set
- 별도 cycle 측 .roadmap.elden_ring cond.1 / cond.2 / cond.3 evidence further
  augment
- 별도 cycle 측 self_test() runtime verification — 6 annotated module hexa CLI
  invocation 검증

status: landed
