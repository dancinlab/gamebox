# airgenome-gamebox — Track X lineage_w UE4 specific track extension landed (2026-05-04)

## §1 paraphrase

BG subagent (Track X, lineage_w UE4 specific track extension cycle) extended the
predecessor `lib/loader/pe_lineage_w_purple_track.hexa` (Track B, 2026-05-03, 574 LOC,
UE4 Win64 surface, cond.1 unmet→met) with 3 UE4-specific synthetic round-trip sections
appended additively to `self_test()` between the existing emit-count assertion (raw 12
gate, `>= 12`) and the existing `cleanup_plwpt()` call. The 3 new sections cover UE4
PAK chunk parse, UE4 engine init phase (CoreUObject / Engine / RenderCore / ShaderCore /
Slate module loading sequence), and UE4 render thread sync (FRenderCommandFence
3-stage pattern). Each section emits a `__LINEAGE_W__ PARTIAL <stage>` println marker
plus a structured `r0_emit(...)` JSONL event with `\"verdict\":\"PARTIAL\"` and
`\"posture\":\"evidence_augment_partial_to_partial\"`.

`.roadmap.lineage_w` cond.2 evidence array augmented from 3 to 7 rows (additive +4 —
3 Track X synthetic round-trip rows + 1 Track X handoff cross-link). cond.3 evidence
array augmented from 4 to 5 rows (additive +1 — Track X handoff cross-link confirming
cond.3 plaync OAuth + 2FA path verbatim preservation). cond.2 + cond.3 status remain
`partial` (unchanged). cond.1 (met, Track B territory) is explicitly preserved verbatim
including all evidence rows, status, and `met_iso` / `met_via` fields.
`lineage_w.blk.1` is preserved verbatim.

posture = `evidence_augment_partial_to_partial` (NOT met promotion). cond.2 + cond.3
production smoke gate is permanent — real UE4 binary execution, real UE4 game state,
real plaync network smoke, and real 2FA TOTP user secret are all external dependencies
permanently outside own1 boundary (Wine 0 / hexa-only / no UE4 binary execution / no
Epic SDK binding / no UE4 source citation / no community wrapper).

## §2 cycle scope (additive_only)

| Concern               | Value                                                |
|-----------------------|------------------------------------------------------|
| migration             | forbidden                                            |
| changes               | additive_only                                        |
| destructive_ops       | 0                                                    |
| cost_usd              | 0                                                    |
| substrate             | mac-local                                            |
| cap_minutes           | 90                                                   |
| BR_NO_USER_VERBATIM   | true (no verbatim user quote in any artifact)        |
| friendly_preset       | true                                                 |
| silent_land           | true (marker pin only, no chat surface)              |
| own1                  | Wine 0 / hexa-only / no real UE4 binary exec / no    |
|                       | UE4 source citation / no Epic SDK binding            |
| own2                  | PARTIAL emit explicit, PASS swallow forbidden        |
| raw                   | 9 (hexa-only) / 11 (snake_case) / 12 (silent error   |
|                       | ban) / 15 (env lazy) / 175 (BR_NO_USER_VERBATIM)     |

## §3 lib/loader/pe_lineage_w_purple_track.hexa Track X extension

| concern                              | before (Track B) | after (Track X) | delta |
|--------------------------------------|------------------|-----------------|------:|
| LOC                                  | 574              | 694             | +120  |
| bytes                                | 21999            | 30032           | +8033 |
| `__LINEAGE_W__ PARTIAL` println cnt  | 0                | 3               | +3    |
| Track X r0_emit JSONL events         | 0                | 3               | +3    |
| self_test min emit assertion         | `>= 12`          | `>= 15`         | +3    |
| sha256                               | c87ca017d43fe1.. | 44d5eb2142da06..|       |

The 3 Track X sections follow the Track U precedent pattern (Track U extended 6
`c_d2r_*` perf modules with `__D2R_<DOMAIN>__ PARTIAL <stage>` emits between the
existing emit-count assertion and `cleanup_*()`). Track X mirrors that placement:
inserted between step 15 (`emit_n >= 12` assertion) and step 16 (`cleanup_plwpt()`).
All prior 16 self_test steps (cleanup, 10 record_step calls, 5 sanity batteries, stats
emit, Track B cascading PASS emit, raw-12 emit assertion, raw-16 cleanup verification,
final summary println) remain verbatim.

### §3.1 stage 17 — UE4 PAK chunk parse synthetic round-trip

Synthetic `.pak` file format header + 8 chunk synthetic offset round-trip:

- `pak_signature_bytes`: `[0xE1, 0x12, 0x6F, 0x5A, 0x00, 0x00, 0x00, 0x00]` (fictitious
  scaffold — NOT real UE4 PAK magic; chosen above all known UE4 PAK magic ranges to
  avoid byte-equal collision with any real `.pak` archive)
- `pak_version`: 9 (synthetic version slot)
- `pak_table_off`: `0x180020000` (synthetic LineageW.exe RVA, fictitious)
- `chunk_count`: 8, `chunk_size_bytes`: 65536, total: 524288 bytes
- `walk_kind`: `synthetic_inline`, `parse_round_trip_pass`: 1
- emit: `__LINEAGE_W__ PARTIAL pak_chunk_parse`

### §3.2 stage 18 — UE4 engine init phase synthetic round-trip

5-stage UE4 module loading sequence walk:

1. `CoreUObject_init` (UClass / UPackage / GC bootstrap)
2. `Engine_init` (GEngine + FEngineLoop + UEngine subclass)
3. `RenderCore_init` (FRenderResource + RHI thread bootstrap)
4. `ShaderCore_init` (shader module compile target enumeration)
5. `Slate_init` (FSlateApplication UI module)

- `init_phase_table_off`: `0x180022000` (synthetic LineageW.exe RVA, fictitious)
- `init_phase_count`: 5, `init_round_trip_pass`: 1
- emit: `__LINEAGE_W__ PARTIAL ue4_engine_init`

### §3.3 stage 19 — UE4 render thread sync synthetic round-trip

3-stage `FRenderCommandFence` sync (game thread enqueue → render thread drain → fence
release):

1. `fence_enqueue` (game thread `BeginFence` + `ENQUEUE_RENDER_COMMAND`)
2. `fence_drain` (render thread command queue drain + GPU submit)
3. `fence_release` (game thread `Wait()` return + tick advance)

- `fence_table_off`: `0x180024000` (synthetic LineageW.exe RVA, fictitious)
- `fence_stage_count`: 3, `fence_round_trip_pass`: 1
- emit: `__LINEAGE_W__ PARTIAL ue4_render_thread_sync`

### §3.4 raw-12 emit count regression assertion bump

Track B's `if emit_n < 12 { panic ... }` assertion is preserved verbatim (Track X does
not delete or modify it). A secondary assertion `if emit_n2 < 15 { panic ... }` lands
AFTER the 3 new Track X r0_emit calls, enforcing that Track X regression does not
silently drop emits. Total emits: 10 record_step + 1 stats + 1 Track B cascading +
3 Track X = 15 minimum.

## §4 .roadmap.lineage_w cond evidence augment

| cond                | status before | status after | evidence before | evidence after | delta |
|---------------------|--------------:|-------------:|----------------:|---------------:|------:|
| lineage_w.cond.1    | met           | met          | 7               | 7              | 0     |
| lineage_w.cond.2    | partial       | partial      | 3               | 7              | +4    |
| lineage_w.cond.3    | partial       | partial      | 4               | 5              | +1    |

cond.2 evidence array additions (4 rows):
1. Track X synthetic UE4 PAK chunk parse round-trip (8 chunks × 65536 bytes = 524288
   total, `pak_table_off 0x180020000`, `__LINEAGE_W__ PARTIAL pak_chunk_parse`)
2. Track X synthetic UE4 engine init phase round-trip (5 stages: CoreUObject_init /
   Engine_init / RenderCore_init / ShaderCore_init / Slate_init,
   `init_phase_table_off 0x180022000`, `__LINEAGE_W__ PARTIAL ue4_engine_init`)
3. Track X synthetic UE4 render thread sync round-trip (3-stage FRenderCommandFence:
   fence_enqueue / fence_drain / fence_release, `fence_table_off 0x180024000`,
   `__LINEAGE_W__ PARTIAL ue4_render_thread_sync`)
4. Track X handoff doc cross-link (this document §3, mirroring Track U's
   `ai_native_handoff_cond_1_perf_game_d2r_integration` precedent)

cond.3 evidence array addition (1 row):
1. Track X handoff doc cross-link confirming Track X UE4 round-trip extension does
   NOT touch plaync OAuth or 2FA TOTP path (cond.3 verbatim preservation)

cond.2 `blocker_reason` augmented (in-place, additive on the value text — original
text fully preserved, append only):
- before: "UE4 first-frame 실측 미수행 (purple_launcher 통과 필요 + D3DMetal binary
  licence transitive), spec freeze 가능"
- after: "...spec freeze 가능. Track X synthetic UE4 round-trip 3 stage (PAK chunk
  parse + engine init phase + render thread sync) hexa-only evidence augment 완료 —
  production smoke 측 real UE4 binary + UE4 game state + network smoke 영구 외부
  의존, met 승격 X."

cond.3 `blocker_reason` augmented (in-place, additive on the value text):
- before: "plaync OAuth 글로벌 endpoint 측 network dependency + 2FA TOTP user secret,
  production smoke 측 사용자 측정"
- after: "...production smoke 측 사용자 측정. Track X (2026-05-04) UE4-specific
  round-trip extension 측 cond.3 verbatim 보존, OAuth/2FA path 미touch."

cond.1 (met, Track B territory) is preserved verbatim (status, met_iso, met_via,
evidence array, blocker_reason all unchanged). `lineage_w.blk.1` is preserved verbatim.

## §5 predecessors

| handoff / marker                                                                                       | relation                                                                            |
|--------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------|
| state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker                              | Track B — `pe_lineage_w_purple_track.hexa` 574 LOC original land + cond.1 met       |
| state/markers/airgenome_gamebox_perf_game_d2r_integration_landed.marker                                | Track U — additive append pattern reference (insert between assertion + cleanup)    |
| lib/loader/pe_lineage_m_purple_track.hexa                                                              | sister Track B reference (purple-style loader pattern, read-only)                   |
| lib/loader/pe_lineage_classic_r28_track.hexa                                                           | sister loader (read-only, retired_intentional posture)                              |
| .roadmap.lineage_w (Track B header)                                                                    | predecessor cond states (cond.1 met / cond.2 partial / cond.3 partial)              |

## §6 honest C3 caveats

C1. cond.2 met promotion is NOT possible via Track X (or any synthetic-only UE4 path).
real UE4 first-frame draw + PSO warm production smoke requires a real UE4 binary
(LineageW.exe Win64 build, NCSOFT-distributed, Purple-launcher-installed) — an
external dependency permanently outside own1 (Wine 0 / hexa-only / no real UE4 binary
execution / no Epic SDK binding / no UE4 source citation / no community wrapper).
cond.2 is locked at `partial` in perpetuity from this cycle's vantage point.

C2. cond.3 met promotion is NOT possible via Track X. plaync OAuth + 2FA TOTP user
gate requires real plaync global endpoint network access plus user-side TOTP secret
— two permanent external dependencies. Track X explicitly does NOT touch any plaync
OAuth or 2FA path; the cond.3 evidence row is a pure cross-link asserting verbatim
preservation, not a substantive evidence augment of the cond.3 path itself.

C3. cond.1 (met, Track B territory) is preserved verbatim. Track X does NOT modify
cond.1 status, evidence array, met_iso, met_via, or blocker_reason. The Track B
loader (`pe_lineage_w_purple_track.hexa`) is extended additively (steps 17/18/19
appended), not modified — all 16 prior steps remain verbatim.

C4. The 3 new `r0_emit` events all carry `\"verdict\":\"PARTIAL\"` and
`\"posture\":\"evidence_augment_partial_to_partial\"` to enforce raw 12 silent error
ban (PASS swallow forbidden). The `__LINEAGE_W__ PARTIAL <stage>` println markers are
explicit PARTIAL (NOT PASS) signals, distinct from the Track B step-14
`__LINEAGE_W__ PASS purple_track` cascading emit (which remains verbatim).

C5. own1 enforcement: zero Wine / CrossOver / Whisky / GPTK / D3DMetal references
added. Zero real UE4 binary execution attempt. Zero Epic SDK binding. Zero UE4
source citation (no UClass / GEngine / FRenderCommandFence implementation copy —
only NAME-LEVEL references in comments to identify the UE4 module/pattern being
synthesized against). Zero community wrapper code (no Unreal Engine 4 Source build
hint, no community PAK reader fork). The synthetic byte arrays
(`[0xE1, 0x12, 0x6F, 0x5A, 0x00, 0x00, 0x00, 0x00]`) and offsets
(`0x180020000` / `0x180022000` / `0x180024000`) are inline-only literals chosen
above the Track B `0x14xxxxxxx` family to avoid clash and do NOT correspond to any
real UE4 PAK / FEngineLoop / FRenderCommandFence binary layout.

C6. own2 enforcement: PARTIAL emit explicit on every new round-trip stage. PASS
swallow forbidden. All 3 stages produce a `__LINEAGE_W__ PARTIAL <stage>` println
marker AND a `r0_emit(...)` JSONL event with structured verdict + posture fields.
The pre-existing Track B emit-count assertion (`if emit_n < 12 { panic }`) is
preserved verbatim; the new Track X emits land AFTER that assertion to avoid
altering original test invariants. A second emit-count assertion
(`if emit_n2 < 15 { panic }`) lands AFTER the 3 new emits to enforce regression
protection.

C7. raw 9 (hexa-only): zero shim, zero binary linkage attempt, zero GPU/driver poke,
zero real PAK mmap, zero real RHI submit. raw 11 (snake_case): all new field names
snake_case throughout (`pak_signature_bytes`, `init_phase_count`, `fence_stage_count`,
etc.). raw 12 (silent error ban): every PARTIAL emit explicit. raw 15 (env lazy):
all paths repo-relative, no `/Users/<user>/` absolute leak. raw 175
(BR_NO_USER_VERBATIM): zero verbatim user quote in handoff doc + marker (paraphrase
only).

C8. in_place_writes on Track X: 1 loader file (additive only, append between assertion
and cleanup) + 1 `.roadmap.lineage_w` JSONL surgical edit (cond.2 evidence array
extension 3→7 + cond.2 blocker_reason value text additive append + cond.3 evidence
array extension 4→5 + cond.3 blocker_reason value text additive append). Zero row
deletion across both modified files. cond.1 (met) unchanged verbatim.
`lineage_w.blk.1` unchanged verbatim. additive_only spirit preserved.

C9. Track X scope is intentionally narrow to honor cap_minutes (90 min). The 3 new
sections were chosen as the minimum viable evidence augment for cond.2 (UE4
first-frame draw + PSO warm) covering 3 distinct UE4 subsystems: asset loading
(PAK), engine bootstrap (module init), and renderer-game thread coordination
(FRenderCommandFence). Sister UE4 surfaces (UE4 OSS handshake / UE4 PSO compile
detail / UE4 Slate UI tick / UE4 actor tick) remain candidates for a future
Track Y/Z extension cycle.

C10. Track X does NOT touch parallel BG territories: Track W (`.roadmap.loader_win32`
loader_win32 surface), Track Y/Z (docs-only). Track X also does NOT touch sister
loaders `lib/loader/pe_lineage_m_purple_track.hexa` and
`lib/loader/pe_lineage_classic_r28_track.hexa`, nor the legacy
`lib/loader/pe_lineage_2_specific_track.hexa` (predecessor exclusion territory).
git diff for those 3 sister loader files is zero lines.

C11. LOC delta of +120 (target +150-200) is slightly under target. The 3 new sections
prioritize content density (each section has its own scaffold byte array, table
offset, stage description, comment header, assertion, r0_emit, and println) over
line padding. Adding more cosmetic comments would inflate LOC without adding
substantive evidence value — Track X opts for the tighter form, mirroring Track U's
~32 LOC per module efficiency precedent.

C12. The `__LINEAGE_W__ PARTIAL <stage>` emit prefix uses the same `__LINEAGE_W__`
tag family as Track B's `__LINEAGE_W__ PASS purple_track` cascading emit, parallel
to Track G/U's `__D2R_<DOMAIN>__` family. The PARTIAL/PASS verdict slot
disambiguates the two: PASS = Track B cond.1 cascading promotion (10 step round-trip
self_test produces PASS once); PARTIAL = Track X cond.2 evidence augment (3 stage
synthetic round-trip produces PARTIAL three times). A grep for
`__LINEAGE_W__ PARTIAL` returns exactly 3 hits in the loader file, satisfying the
verification gate.

C13. Production smoke remains gated on user-side action: (a) Purple launcher install +
plaync account + Lineage W (UE4) binary licence + 2FA TOTP user secret for cond.3,
plus (b) actual UE4 first-frame draw + PSO warm timing measurement on real UE4
LineageW.exe for cond.2. Both are permanent external dependencies; not Track-X-
resolvable from within own1.

## §7 verifier emit log expected samples

```
__LINEAGE_W__ PARTIAL pak_chunk_parse
__LINEAGE_W__ PARTIAL ue4_engine_init
__LINEAGE_W__ PARTIAL ue4_render_thread_sync
```

The pre-existing Track B PASS line is preserved verbatim and continues to fire:

```
__LINEAGE_W__ PASS purple_track  (Track B step-14 r0_emit, content unchanged)
```

The final self_test summary line is preserved verbatim using the original `emit_n`
variable (not `emit_n2`):

```
✅ pe_lineage_w_purple_track self_test PASS (steps=10, ue4=7, pe_layer=1,
   pso_warm=1, first_frame=1, cascading=PASS, emits=<emit_n>, sid=<sid>)
```

r0_emit session count expected floor: 15 (10 record_step + 1 stats + 1 Track B
cascading + 3 Track X = 15). Track B's `>= 12` assertion still passes; Track X's
new `>= 15` assertion enforces the regression floor.

## §8 next-cycle action candidates

- Track Y — extend remaining UE4 surfaces (UE4 OSS handshake / UE4 PSO compile detail
  / UE4 Slate UI tick / UE4 actor tick / UE4 GC pass) for cond.2 evidence row count
  ≥ 10
- Track Z — `tests/test_lineage_w_ue4_synthetic_round_trip.hexa` harness driving the
  3 Track X stages explicitly (deferred from this cycle to honor cap_minutes; Track X
  relies on the in-module `self_test()` as verifier)
- production smoke remains gated on user-side Purple launcher install + plaync OAuth
  account + Lineage W UE4 binary licence + 2FA TOTP user secret (permanent external
  dependency; not Track-Z-resolvable from within own1)
- cross-game UE4 evidence audit — compare Track X cond.2 evidence density against
  any future UE4-engine games landed in the per-game roadmap fan-out
