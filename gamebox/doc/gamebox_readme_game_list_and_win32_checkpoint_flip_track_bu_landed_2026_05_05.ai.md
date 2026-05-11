# gamebox README Game Coverage Section + Win32 Checkpoint Cached Status Flip — Track BU Landed (2026-05-05)

> Track BU on gamebox standalone repo (`dancinlab/gamebox`). Two sub-missions:
> BU1 — README.md NEW "Game Coverage" section (additive insertion).
> BU2 — `.checkpoint.win32_50pct_coverage` + `.checkpoint.win32_full_coverage` cached
> header status field flip `unmet` → `met` (drift fix per Track BO snapshot v2 + Track BR
> derivation guard transition criteria + Track BP Win32 100% milestone).

---

## Context

- Repo: `dancinlab/gamebox` (origin), Apache-2.0, sim-universe sibling layout.
- Predecessor: Track BT closure (full standalone separation 2026-05-05 — `gamebox_initial_extraction_landed.marker`).
- Concurrent-prior: Track BP (Win32 cycle 60-62 — 91.49% → 100.00% milestone breach skeleton-tier).
- Concurrent-prior: Track BR (pre-emptive derivation guard for `.checkpoint.win32_full_coverage` cached `unmet` → `met` transition criteria documented).
- Concurrent-prior: Track BO (`checkpoint_status_snapshot_v2` — derived 1 met / 5 partial / 1 unmet under weighted_threshold percentage-mode; drift_count = 3).
- Forensic verdict (unchanged): `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.

---

## BU1 — README.md Game Coverage section

Inserted between `## Module Inventory` (after Predecessor markers subsection) and
`## hexa-lang upstream`. Net additive: +40 LOC.

Section content:

- **11 .roadmap targets table** (8 games + 3 launchers/runtimes) with cond.1 / cond.2 / cond.3 status mirroring current roadmap header state (no inflation):
  - diablo2_resurrected (game): partial / met / partial — offline single-player viable, online realm Warden L2 long-horizon.
  - diablo4 (game): partial / partial / partial — retired_intentional (always-online DRM).
  - battlenet (launcher): met / met / partial — offline shim PASS, online OAuth gate.
  - purple_launcher (launcher): met / met / partial — offline shim PASS, plaync OAuth gate.
  - lineage_m (game): met / partial / partial — Android emul layer permanent boundary.
  - lineage_w (game): met / partial / partial — UE4 + Purple cascading.
  - lineage_classic_r28 (game): partial / partial / partial — retired_intentional (GameGuard kernel AC).
  - wow / cs2 / delta_force / elden_ring (game): partial / partial / partial — all retired_intentional with respective AC/DRM rationale.
- **Per-game README index pointer**: `docs/per_game/` (11 sub-page READMEs, raw 270 triplet T1 baseline 2026-05-03).
- **3 cross-cutting loader roadmap pointers**: loader_pe / loader_win32 (435/435 = 100% skeleton-tier cycle 27→62) / loader_dx (D3DMetal binary licence transitive blocker).
- **7 cross-roadmap checkpoint table**: 5 partial AND-aggregated game/launcher integration checkpoints + 2 weighted_threshold win32_*pct_coverage now both **met** post-BU flip.
- **Honest scope footer**: validated_manjeom = 0 explicit (raw#10 C3 — 0 games run a real frame yet; all skeleton-tier, real-game first-frame requires production smoke gate Track AU recommended path A own1 multi-year horizon).

---

## BU2 — Cached header status flip x2

### `.checkpoint.win32_full_coverage` (was: unmet 30.34/100)

| Field | Before | After |
|---|---|---|
| `status` | `unmet` | `met` |
| `progress` | `30.34/100` | `100/100` |
| `progress_source` | `airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker (cycle_35_percent=30.34)` | `airgenome_gamebox_win32_cycle60_62_bundled_increment_landed.marker (cycle_62_percent=100.00)` |
| `met_iso` | (absent) | `2026-05-05` (NEW) |
| `met_via` | (absent) | `track_bp_win32_cycle60_62_full_coverage_milestone` (NEW) |
| `blockers` | (8-item list) | renamed `blockers_resolved` — list preserved verbatim for traceability |
| `evidence` | (absent) | NEW — 3 entries (Track BP / Track BR / Track BU) with marker pointers |
| `predecessors` | 4 markers | 7 markers (added BR guard + BP cycle 60-62 + this BU marker) |
| `since` | `2026-05-04` | `2026-05-04` (PRESERVED — original lock-in date) |

Rationale: Track BR derivation guard documented `cached_header_state_NOT_modified_in_this_cycle` with explicit `drift_acknowledgement` flagging Track BO snapshot drift_count=3 entry. Track BP cycle 60-62 satisfies `transition_criteria_checklist`: win32_fn_coverage_ge_100pct_435_435 ✓ / track_bp_marker_threshold_100pct_breached_true ✓ / track_bp_marker_win32_full_coverage_milestone_true ✓. BU is the optional `cached_header_status_field_refresh_to_met` drift-fix cycle.

### `.checkpoint.win32_50pct_coverage` (was: unmet 30.34/50)

| Field | Before | After |
|---|---|---|
| `status` | `unmet` | `met` |
| `progress` | `30.34/50` | `100/50` |
| `progress_source` | `airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker (cycle_35_percent=30.34)` | `airgenome_gamebox_win32_cycle60_62_bundled_increment_landed.marker (cycle_62_percent=100.00)` |
| `met_iso` | (absent) | `2026-05-05` (NEW) |
| `met_via` | (absent) | `track_bl_win32_50pct_breach_then_track_bp_full_coverage_milestone` (NEW) |
| `blockers` | (5-item list) | renamed `blockers_resolved` — list preserved verbatim |
| `evidence` | (absent) | NEW — 4 entries (Track BL / Track BO / Track BP / Track BU) |
| `predecessors` | 4 markers | 7 markers |
| `since` | `2026-05-04` | `2026-05-04` (PRESERVED) |

Rationale: Track BO snapshot v2 derived this checkpoint `met` under weighted_threshold percentage-mode (91.49 >= 50) but cached header was stale `unmet` (drift_count item). BU resolves drift; Track BP cycle 60-62 reinforcement (100% well above 50% threshold).

---

## Drift resolution — Track BO drift_count = 3 → 1 (2 of 3 resolved)

| # | Drift Item | Status | Resolution Cycle |
|---|---|---|---|
| 1 | `.checkpoint.win32_50pct_coverage` cached unmet, derived met | RESOLVED | Track BU |
| 2 | `.checkpoint.win32_full_coverage` cached unmet, derived partial pre-BP / met post-BP | RESOLVED | Track BU (post Track BP completion) |
| 3 | `.checkpoint.lineage_classic_r28_play` retired_intentional sideways-PASS semantics | DEFERRED | separate later cycle (different semantic class — retired_intentional ≠ unmet) |

---

## In-place writes

| File | Op | sha256 (post-edit) |
|---|---|---|
| `README.md` | additive section insertion (+40 LOC) | `d5c61998d7e975434cd51427bcd213496ef001ffe643bf1bd36d8090a5242283` |
| `.checkpoint.win32_full_coverage` | JSONL header field flip + augment | `837500552b7fa4de46a84f6bbf78f6b1561f8e1c6d1914c9a7c8e081a8300e16` |
| `.checkpoint.win32_50pct_coverage` | JSONL header field flip + augment | `64885921b671d7de75b010de2e942dabb19b0d7ebfda2f0f9b3128af10fd72d0` |

`in_place_writes`: 3 (README.md additive section + 2 .checkpoint header field flips).
NEW files: 2 (this handoff doc + Track BU marker).
0 .roadmap mutation / 0 lib/ / 0 tool/ / 0 native/ / 0 tests/ / 0 cli/ / 0 install.hexa / 0 hexa.toml / 0 LICENSE / 0 CHANGELOG / 0 existing docs/*.md / 0 predecessor marker mutation.

---

## Policy compliance

- **migration**: forbidden ✓
- **changes**: additive_only ✓ (section insertion + JSONL field-level flip per Track G additive_at_field-level precedent)
- **destructive_ops**: 0 ✓ (own1 destructive_ops:0 RESTORED post Track BT cycle-scoped exception)
- **cost_usd**: 0 ✓ (mac-local)
- **cap_minutes**: 90 ✓
- **BR_NO_USER_VERBATIM**: ✓ (no Korean user verbatim quotes)
- **friendly_preset / silent-land**: ✓
- **own1**: Wine 0 / hexa-only / no anima clone / no DRM bypass ✓
- **own2**: forensic_verdict honest unchanged (`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`); validated_manjeom 0 explicit in README scope ✓
- **raw**: 9 / 11 / 12 / 15 / 175 enforce ✓

---

## Forensic verdict (UNCHANGED)

```
SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0
```

The win32_full_coverage met flip refers ONLY to skeleton-tier fn surface coverage
(435/435 fn shape present, all individually compile/probe). It does NOT promote
`validated_manjeom` (still 0 — no real game frames evidenced). README Caveats §1
remains the authoritative honest scope statement.

---

## Verification

- `grep -c "Game Coverage" README.md` → `1`
- `grep -c '"status":"met"' .checkpoint.win32_full_coverage` → `1`
- `grep -c '"status":"met"' .checkpoint.win32_50pct_coverage` → `1`
- both .checkpoint files JSONL parse OK (python3 json.loads round-trip)

---

## Predecessors

- Track BT — `state/markers/gamebox_initial_extraction_landed.marker` (full standalone separation closure)
- Track BR — `state/markers/airgenome_gamebox_checkpoint_win32_full_coverage_met_derivation_guard_track_br_landed.marker` (transition criteria documented)
- Track BO — `state/markers/airgenome_gamebox_checkpoint_status_snapshot_v2_track_bo_landed.marker` (drift_count=3 source)
- Track BP — `state/markers/airgenome_gamebox_win32_cycle60_62_bundled_increment_landed.marker` (Win32 100% milestone)
