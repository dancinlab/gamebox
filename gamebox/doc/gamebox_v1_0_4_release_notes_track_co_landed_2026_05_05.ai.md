# gamebox v1.0.4 Release Notes — Track CO Landed (2026-05-05)

> Track CO on gamebox standalone repo, branch `main`. Patch release bundling
> post-v1.0.3 final closure work: CK (VAC freeze) + CL (EAC kernel freeze) +
> CM (nightreign skeleton-tier fan-out) + CN (perpetual deferral ack).
> hexa.toml version bump 1.0.3 → 1.0.4.

---

## Context

- Repo: `need-singularity/gamebox` (origin), Apache-2.0.
- Branch: `main`.
- Predecessor: Track CN (hexa interp closure perpetual deferral ack 2026-05-05).
- v1.0.3 (Track CI): paper-mode validation + inventory cleanup + v3 aggregate 2026-05-05.
- Forensic verdict (UNCHANGED): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## Mission

CO1 — `RELEASE_NOTES_v1.0.4.md` NEW file authored:

- Summary section bundling CK + CL + CM + CN.
- Highlights: honest freeze family complete (5 docs), nightreign fan-out complete at skeleton-tier, hook 4 perpetually closed.
- Cumulative state table v1.0.0..v1.0.4.
- Source repo + branch + author boilerplate.

CO2 — `CHANGELOG.md` v1.0.4 section additive insertion above v1.0.3.

CO3 — `hexa.toml` version `1.0.3` → `1.0.4` + README badge URL refresh.

CO4 — handoff doc + marker (this file + Track CO marker).

---

## Release contents

| Track | Marker | Scope |
|---|---|---|
| CK | `gamebox_vac_honest_freeze_track_ck_landed` | VAC user-mode AC + Trusted Mode honest spec freeze |
| CL | `gamebox_eac_kernel_honest_freeze_track_cl_landed` | EAC kernel + anti-tamper 3-cross-class honest spec freeze |
| CM | `gamebox_nightreign_lib_module_fan_out_track_cm_landed` | nightreign 1 loader + 3 perf skeleton-tier (609 LOC) |
| CN | `gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed` | hook 4 perpetual deferral acknowledgment |

---

## In-place writes

| File | Op |
|---|---|
| `hexa.toml` | version field flip 1.0.3 → 1.0.4 |
| `README.md` | version badge URL flip |
| `CHANGELOG.md` | additive section insertion (v1.0.4 above v1.0.3) |

`in_place_writes`: 3.
NEW files: 3 (`RELEASE_NOTES_v1.0.4.md` + this handoff doc + Track CO marker).
0 .roadmap mutation / 0 .checkpoint mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 LICENSE / 0 existing airgenome/doc/ / 0 existing gamebox/doc/ / 0 existing docs/*.md / 0 predecessor marker mutation / 0 gamebox-native marker mutation.

---

## Cumulative state (post v1.0.4)

| metric | v1.0.0 | v1.0.1 | v1.0.2 | v1.0.3 | **v1.0.4** | delta v1.0.3→v1.0.4 |
|---|---|---|---|---|---|---|
| .roadmap files | 14 | 14 | 15 | 15 | 15 | 0 |
| .checkpoint met | 0 | 3 | 3 | 3 | 3 | 0 |
| per_game §9 sections | 0 | 0 | 11 | 11 | 11 | 0 |
| gamebox-native markers | 1 | 7 | 9 | 15 | **19** | +4 (CK+CL+CM+CN) |
| Honest freeze docs | 3 | 3 | 3 | 3 | **5** | +2 (CK+CL) |
| lib/loader files | 455 | 455 | 455 | 455 | **456** | +1 (CM nightreign loader) |
| lib/perf files | 242 | 242 | 242 | 242 | **245** | +3 (CM nightreign perf) |
| Forensic verdict | UNCHANGED | UNCHANGED | UNCHANGED | UNCHANGED | UNCHANGED | 0 |
| validated_manjeom | 0 | 0 | 0 | 0 | 0 | 0 |

---

## Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓
- **destructive_ops**: 0 ✓
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no anima clone / no DRM bypass ✓
- **own2**: forensic_verdict honest unchanged; release notes explicit about validated_manjeom=0 carry; honest freeze family complete narrative explicit; perpetual deferral honest ✓
- **raw**: 9 / 11 / 12 / 15 / 175 enforce ✓

---

## Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

v1.0.4 is final closure cycle bundle. NO promotion of validated_manjeom. NO
real-game first-frame surface. honest freeze family complete + nightreign
skeleton-tier fan-out + perpetual deferral ack.

---

## Verification

- `grep '^version' hexa.toml` → `version = "1.0.4"`
- `head -5 CHANGELOG.md` shows `## [1.0.4] — 2026-05-05` above `## [1.0.3]`
- `RELEASE_NOTES_v1.0.4.md` exists with valid markdown
- README version badge shows `version-1.0.4`
- 4 new gamebox/doc/ handoff docs (CK / CL / CM / CN) present
- 4 new state/markers/gamebox_*.marker (CK / CL / CM / CN) present

---

## Predecessors

- Track CN — `state/markers/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed.marker`
- Track CM — `state/markers/gamebox_nightreign_lib_module_fan_out_track_cm_landed.marker`
- Track CL — `state/markers/gamebox_eac_kernel_honest_freeze_track_cl_landed.marker`
- Track CK — `state/markers/gamebox_vac_honest_freeze_track_ck_landed.marker`
- Track CI — `state/markers/gamebox_v1_0_3_release_notes_track_ci_landed.marker` (v1.0.3 predecessor)

---

## Next-cycle hooks (NOT triggered)

- `git tag v1.0.4` + push to origin (this cycle CO will tag + push as part of session closure)
- Track CP candidate: final session closure marker (잔여 0건) — sibling cycle to CO
- Future cycle: real-run validation post hexa interp closure CM-26..CM-30 (perpetually deferred per CN)
