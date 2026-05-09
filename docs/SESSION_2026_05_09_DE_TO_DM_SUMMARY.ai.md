// docs/SESSION_2026_05_09_DE_TO_DM_SUMMARY.ai.md
# Session 2026-05-09 — Tracks DE → DM (9 commits)

> Single autonomous-iter session covering 9 tracks across hexa
> infrastructure, test cleanup, CLI dispatch fix, BSD portability,
> roadmap fixture isolation, **and the first native binary in the
> repo (PE→Mach-O shim + i386 decoder)**.
>
> Top-tier verdict carries unchanged across all 9 tracks:
> `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`. validated_manjeom = 0
> still carries — gated on E5 (first NSWindow), multi-session work
> per `docs/PE_TO_MACHO_SHIM_PLAN.ai.md`.

---

## Track summary table

| Track | Commit | Title | LoC delta |
|---|---|---|---|
| DE | `94be8c9` | full-repo real-execution audit 458/458 PASS | + win32_shell32 3-conflict fix |
| DF | `fd13a8d` | joined battle.net + D2R 20/20 fastest-arriving real run | docs+marker |
| DG | `e06ea78` | own1/own2 test infra fix × 3 greedy-grep repairs | tests/ |
| DH | `702ef47` | unified test 6/9 → 9/9 PASS (3 fixes) | tests + perf |
| DI | `3790d5c` | BSD sed/grep `\s` portability audit × 12 regex | 6 files |
| DJ | `f637b68` | CLI sys_argv → args() fix; launch battlenet emits honest PARTIAL | cli/ |
| DK | `dd8a7e3` | roadmap_op S11 archive fixture isolation fix | tests/ |
| DL | `09f2c65` | **native pe_to_macho_shim E1** (first native binary; real Battle.net-Setup.exe parse + mmap) | +982 |
| DM | `1399b0d` | **native i386 decoder E2** (Battle.net-Setup.exe entry block 9/9 unknown=0) | +829 |

Total: 9 commits. Gross LoC added ~2400 (most in DL/DM native bring-up
and supporting documentation). No deletions of skeleton-tier modules.

---

## Repo state at end of session

### Tier verdict (unchanged)

- Top-tier: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0` carries.
- Sub-tier promoted: skeleton-tier-real-execution-confirmed extended
  from hexa-only (DD/DF) to native+hexa boundary (DL/DM).
- `validated_manjeom = 0` carries. The next genuine carry-flip is at
  E5 (first real NSWindow paint from Battle.net-Setup.exe code paths)
  per the plan doc.

### Test surface

| Test | Result | Notes |
|---|---|---|
| `tests/test_apple_only.hexa` | 21/21 PASS | DG fix |
| `tests/test_own2.hexa` | 8/8 PASS | DG fix |
| `tests/test_closure.hexa` | self_test PASS @ 67% CM-20/CM-30 | DG cascading |
| `tests/test.hexa` (unified) | 9/9 PASS | DH 3 fixes + DI portability |
| `tests/test_roadmap_op_self_impl.hexa` | track_i/m/ab/ae = ready | DK fixture isolation |
| Joined battle.net + D2R cluster | 20/20 PASS | DF (11 + 6 + 3) |
| Full-repo skeleton audit | 458/458 PASS | DE |

### Closure tracker

- `closure_pct_raw = 25` carries (orchestrator is tracker).
- `closure_pct_weighted_threshold_aware = 40.00` carries.
- `cm_done_index / cm_total = 18 / 30` carries.
- `checkpoint_count / checkpoint_reached = 7 / 3` carries.

### own1 invariants (carry)

- Wine = 0 (verified by test_apple_only 21/21).
- DRM bypass = 0.
- Warden bypass = 0.
- anti-cheat bypass = 0.
- anima clone = 0.
- hexa-only (now also: C-from-spec for native/, Apple SDK only).

### own2 invariants (carry)

- Honest PASS / PARTIAL / FAIL emit per module (extended now to native
  shim with `__SHIM__` prefix and `__I386_DECODE__` prefix).
- silent_swallow = 0.
- PARTIAL emits explicit (especially the entire E1/E2 native surface,
  which is intentionally PARTIAL — execution gated on E3+).
- RETIRED_INTENTIONAL emits explicit where applicable
  (`gamebox launch battlenet` per DJ).

---

## What "battle.net 실행" means right now

User asked twice for `battle.net 실행`. Honest answer at end-of-session:

```
$ ./native/pe_to_macho_shim ~/Downloads/Battle.net-Setup.exe
[shim] gamebox pe_to_macho_shim version=0.1.0-e1-harness-scaffold
[shim.parse] machine=0x14C (i386 PE32) sections=5 entry_va=0x5388A6
__SHIM__ PARTIAL pe_to_macho_shim phase=mmap_done sections_mapped=5 jit_mapped=1 plain_mapped=4
__SHIM__ PARTIAL pe_to_macho_shim phase=e1_done validated_manjeom=0 next_phase=e2_i386_decoder

$ ./native/i386_decode_test ~/Downloads/Battle.net-Setup.exe 32
[disasm] 005388A6  E8 AE 09 00 00            call   0x539259    # mainCRTStartup wrapper
[disasm] 005388AB  E9 7A FE FF FF            jmp    0x53872A
[disasm] 005388B0  55                        push   ebp
[disasm] 005388B1  8B EC                     mov    ebp, esp
[disasm] 005388B3  FF 75 08                  push   [ebp+8]
[disasm] 005388B6  E8 F0 02 00 00            call   0x538BAB
[disasm] 005388BB  59                        pop    ecx
[disasm] 005388BC  5D                        pop    ebp
[disasm] 005388BD  C3                        ret
__I386_DECODE__ PARTIAL phase=disasm decoded=9 unknown=0
```

**Decoded entry:** standard MSVC `mainCRTStartup → __scrt_common_main →
SEH wrapper`. 100% coverage for this prologue. The shim parses and
mmaps the binary correctly. The decoder reads 9-of-9 instructions
without unknowns.

**Not yet executing** any of those instructions. That's E3 onwards.

---

## Honest disclaimer (carries from PE_TO_MACHO_SHIM_PLAN.ai.md §9)

Doing the full job — first NSWindow + first install dialog clickable
from Battle.net-Setup.exe — is roughly **60-100 sessions of focused
work** per the phase plan (E3 interpreter, E4 kernel32 shim, E5 user32
+ gdi32 + Cocoa bridge for first NSWindow). Reaching D2R first
gameplay frame is **likely 200-400 sessions**.

DL+DM landed the first 1.5 of those phases. We don't pretend
otherwise. validated_manjeom advancement happens at E5, not before.

---

## Memory state at session close

- `feedback_autonomous_iter_loop.md` — own iter pattern (carries).
- `project_track_df_to_dh_2026_05_09.md` — earlier-session state
  (now superseded by this doc; can be left as-is for history).

This summary doc is the canonical resume point. Future cycles can
pick up at **E3 (i386 minimal interpreter)** as the next planned
phase.

---

## Files added / modified this session (full list)

**docs/**
- `BATTLENET_D2R_REAL_HEXA_RUN_TRACK_DF.ai.md` (DF)
- `OWN1_OWN2_TEST_INFRA_FIX_TRACK_DG.ai.md` (DG)
- `UNIFIED_TEST_FULL_GREEN_TRACK_DH.ai.md` (DH)
- `PE_TO_MACHO_SHIM_PLAN.ai.md` (DL — full E1..E5+ plan)
- `SESSION_2026_05_09_DE_TO_DM_SUMMARY.ai.md` (this file)

**tests/** (DG, DH, DK)
- `test_apple_only.hexa` (DG: 2 grep filter fixes)
- `test_own2.hexa` (DG: entry guard + headless grep tightening)
- `test.hexa` (DH: manifest + entry N/A guards)
- `test_roadmap_op_self_impl.hexa` (DK: archive fail fixture isolation)

**lib/** (DH, DI)
- `lib/perf/c_oauth_pkce_state.hexa` (DH: verify → verify_pair rename)
- `lib/perf/c_dxr_ray_coherence.hexa` (DH: `\s` → `[[:space:]]`)
- `lib/perf/c_neon_sse_xlat_cache.hexa` (DI: `\s` → `[[:space:]]`)
- `lib/perf/c_texture_streaming_predict.hexa` (DI)
- `lib/perf/c_remediate_plan.hexa` (DI)
- `lib/perf/c_zone_prefetch_io.hexa` (DI)
- `lib/loader/win32_winsock_stage1.hexa` (DI)
- `lib/loader/win32_kernel32_stage3.hexa` (DI)
- `lib/loader/win32_shell32.hexa` (DE: 3-conflict fix)

**cli/** (DJ)
- `cli/gamebox.hexa` (DJ: sys_argv → args() main dispatch)

**native/** (DL, DM — ENTIRELY NEW)
- `native/pe_to_macho_shim.c` (DL: ~170 LoC C shim main)
- `native/pe_parse.h` / `native/pe_parse.c` (DL: ~150 LoC PE-COFF reader)
- `native/i386_decode.h` / `native/i386_decode.c` (DM: ~370 LoC decoder)
- `native/i386_decode_test.c` (DM: ~95 LoC test driver)
- `native/entitlements.plist` (DL)
- `native/build.sh` (DL: clang + ad-hoc codesign; DM extended)
- `native/README.md` (DL)

**state/markers/** (per-track landed markers, all 9 tracks)

**`.gitignore`**: extended for `native/i386_decode_test{,.dSYM/}` (DM)

---

*written 2026-05-09 at session close. Tracks DE → DM committed.*
