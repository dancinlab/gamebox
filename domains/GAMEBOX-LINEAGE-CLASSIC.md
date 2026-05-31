# GAMEBOX-LINEAGE-CLASSIC — current state
@title: 🗡️ GAMEBOX-LINEAGE-CLASSIC — "리니지 클래식 R28"

@goal: Lineage Classic R28 native bring-up on macOS: native first-frame path + R28-specific loader track. ✅ lineage_classic_r28 play checkpoint landed.

## prerequisites (engine subsystems · native client — no launcher)

- GAMEBOX-LOADER — PE load (pe_lineage_classic_r28_track) ✅ code present
- GAMEBOX-WIN32  — Win32 client API shim ✅ code present
- GAMEBOX-GFX    — Metal first frame (c_lineage_classic_r28_native_first_frame) ✅ code present

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [x] R28-specific loader track + native first-frame (pe_lineage_classic_r28_track · c_lineage_classic_r28_native_first_frame · 12 markers)
- [x] Play checkpoint (lineage_classic_r28_play) — `.checkpoint.lineage_classic_r28_play`
- [x] Drift resolution track bv landed (gamebox_lineage_classic_r28_drift_resolution_track_bv)
- [ ] Client protocol / login path — verify online login end-to-end against .roadmap.lineage_classic_r28
