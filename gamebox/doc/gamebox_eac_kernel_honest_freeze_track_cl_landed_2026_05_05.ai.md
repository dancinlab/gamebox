# gamebox EAC Kernel + Anti-Tamper Honest Spec Freeze — Track CL Landed (2026-05-05)

> Track CL on gamebox standalone repo, branch `main`. Sibling cycle to Track CK
> (VAC freeze). Resolves Track CC §9 deferred mk1 narrative hook for
> `delta_force.cond.3` + `elden_ring.cond.1` + `elden_ring_nightreign.blk.1`
> (3-game cross-class). Honest research-direction reference doc; NO bypass /
> NO evasion / NO circumvention code.

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0.
- Branch: `main` (post Track CK VAC freeze 2026-05-05).
- Predecessor (immediate): Track CK (VAC honest freeze; sibling cycle).
- Predecessor (semantic): Track CC §9 (delta_force + elden_ring §9 noted `docs/EAC_KERNEL_HONEST_FREEZE.md` mk1 narrative deferred future-cycle).
- Predecessor (template): Track-E sibling honest freeze docs — GAMEGUARD / WARDEN / D2R_REALM_PROTOCOL (2026-05-03) + Track CK VAC (2026-05-05 same-day).
- Predecessor (BY fan-out): Track BY (elden_ring_nightreign documentation-only fan-out, EAC inherited).
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## Mission

CL1 — `docs/EAC_KERNEL_HONEST_FREEZE.md` NEW file authored:

- §0 목적 / 한계 (3 subsections + extra out-of-scope categories for FromSoft matchmaking + EOS handshake)
- §1 EAC scope (vendor + product family + 3-game applied + components + variant matrix)
- §2 Apple-only constraint (5 subsections: kernel mismatch + anti-tamper variant + nightreign multiplayer + Wine 0 + retired_intentional rationale)
- §3 honest research-direction (high-level naming only, NO bypass code; 5 subsections: kernel hook + anti-tamper + EOS handshake + c_elden_eac_kernel_driver_avoid_track + c_anticheat_detection_track)
- §4 cross-link to sibling honest freeze docs (5 sibling docs: GAMEGUARD / WARDEN / D2R_REALM_PROTOCOL / VAC / EAC self)
- §5 retired_intentional posture lock-in (3 cross-class: delta_force + elden_ring base+DLC + nightreign inherited+co-op)
- §6 caveats (14 honest items — extended from VAC's 10 due to 3-game cross-class scope)
- §7 predecessors
- §8 forensic verdict (UNCHANGED)
- §9 closure declaration (3 cross-class)

CL2 — handoff doc + marker (this file + Track CL marker).

---

## Mirror pattern + EAC-specific extensions

Mirror structure of `docs/GAMEGUARD_HONEST_FREEZE.md` (Track-E 2026-05-03) + `docs/VAC_HONEST_FREEZE.md` (Track CK 2026-05-05). EAC-specific extensions:

| section | extension rationale |
|---|---|
| §1.4 EAC variant matrix | 4 variants documented (kernel legacy / anti-tamper / inherited+co-op / EAC for EOS) — multi-variant scope unique to EAC |
| §2.2 anti-tamper variant additional mismatch | FromSoft anti-tamper IAT integrity + .text hash boundaries — beyond plain kernel mismatch |
| §2.3 nightreign multiplayer co-op session boundary | NEW sub-class (FromSoft online matchmaking) — no precedent in GAMEGUARD/VAC |
| §3.4 c_elden_eac_kernel_driver_avoid_track | gamebox-specific own1 explicit "avoid" posture (not bypass) |
| §5 3-cross-class lock-in | delta_force + elden_ring base+DLC + nightreign — broader than single-game freezes |
| §9 closure declaration | 3-cross-class final lock-in |

CL is longer (~440 LOC) than CK (~250 LOC) due to:
1. 3-game cross-class scope (vs CK's cs2-only)
2. EAC variant matrix (4 variants documented)
3. nightreign multiplayer-specific NEW boundary
4. anti-tamper variant detailed mismatch beyond plain kernel

---

## In-place writes

| File | Op |
|---|---|
| (none) | — |

`in_place_writes`: 0 (cleanest doc-only cycle).
NEW files: 3 (`docs/EAC_KERNEL_HONEST_FREEZE.md` + this handoff doc + Track CL marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0 CHANGELOG / 0 README / 0 RELEASE_NOTES / 0 existing airgenome/doc/ / 0 existing gamebox/doc/ / 0 existing docs/*.md / 0 predecessor marker mutation / 0 gamebox-native marker mutation.

---

## Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (cleanest cycle — 3 NEW files only; 0 in-place writes)
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no AC bypass / no anti-tamper bypass / no EAC kernel driver injection / no EOS handshake circumvention / no FromSoft matchmaking bypass / no anima clone ✓
- **own2**: forensic_verdict honest unchanged; spec freeze ONLY scope explicit; bypass code 0건 명시; 3-cross-class retired_intentional posture lock-in explicit; c_elden_eac_kernel_driver_avoid_track own1 "avoid" posture explicit ✓
- **raw**: 9 / 11 / 12 / 15 / 175 enforce ✓

---

## Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

CL is honest spec freeze documentation only. NO bypass code / NO module / NO pattern.
NO promotion of validated_manjeom. delta_force / elden_ring / nightreign 측 real
frame 측 측정 X (3 cross-class EAC kernel + anti-tamper retired_intentional posture
영구 lock-in).

---

## Verification

- `ls docs/EAC_KERNEL_HONEST_FREEZE.md` → exists
- `grep -c "bypass\|evasion\|circumvention" docs/EAC_KERNEL_HONEST_FREEZE.md` → multiple, but all in **negative** context ("0건", "NO bypass", "본 doc 에서 다루지 않음", "영구 X")
- `grep -c "retired_intentional" docs/EAC_KERNEL_HONEST_FREEZE.md` → multiple references (3-cross-class posture lock-in)
- `grep -c "own1" docs/EAC_KERNEL_HONEST_FREEZE.md` → multiple references
- `grep -c "kernel_driver_avoid" docs/EAC_KERNEL_HONEST_FREEZE.md` → 1+ (own1 explicit "avoid" posture)
- `grep -c "Shadow of the Erdtree" docs/EAC_KERNEL_HONEST_FREEZE.md` → 2+ (DLC scope cross-link to BY)
- `grep -c "nightreign" docs/EAC_KERNEL_HONEST_FREEZE.md` → multiple (inherited+co-op cross-class)

---

## Predecessors

- Track CK — `state/markers/gamebox_vac_honest_freeze_track_ck_landed.marker` (sibling VAC cycle, same-day batch)
- Track CC — `state/markers/gamebox_per_game_readme_post_bu_bv_update_track_cc_landed.marker` (delta_force + elden_ring §9 deferred mk1 narrative hooks resolved by CL)
- Track C — `state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker` (delta_force + elden_ring retired_intentional adoption)
- Track AS — `state/markers/airgenome_gamebox_perf_elden_retired_intentional_track_as_landed.marker` (c_elden_eac_kernel_driver_avoid_track annotation)
- Track BE — `state/markers/airgenome_gamebox_perf_d4_elden_completion_track_be_landed.marker` (c_elden_* family 100% completion)
- Track BM — `state/markers/airgenome_gamebox_perf_remaining_family_inventory_v2_track_bm_landed.marker` (c_sote_* Shadow of the Erdtree DLC scaffold)
- Track BY — `state/markers/gamebox_readme_game_coverage_refresh_plus_nightreign_fan_out_track_by_landed.marker` (nightreign documentation-only fan-out + DLC scope)
- Track-E (GAMEGUARD honest freeze) — `state/markers/airgenome_gamebox_gameguard_honest_freeze_landed.marker` (template predecessor)
- Track-E (WARDEN honest freeze) — `state/markers/airgenome_gamebox_warden_honest_freeze_landed.marker` (sibling pattern)
- `docs/DELTA_FORCE_MANJEOM_REVIEW.md` + `docs/ELDEN_RING_MANJEOM_REVIEW.md` (mk1 narrative spec freeze, predecessor REUSE)

---

## Next-cycle hooks (NOT triggered)

- Track CM candidate: nightreign lib module fan-out (1 loader + 3-5 perf modules) — pending real binary acquisition + EAC class re-confirmation
- Future cycle: Apex Legends / Fortnite / 기타 EAC family 게임 측 per-game roadmap fan-out (별도 cycle)
- Future cycle: EAC for EOS variant 측 추가 reference (외부 공개 자료 추가 시 evidence augment, NOT bypass)
- Future cycle: v1.0.4 release notes bundling CK + CL (post-v1.0.3 patch decision deferred to user direction)
