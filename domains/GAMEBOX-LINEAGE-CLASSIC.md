# GAMEBOX-LINEAGE-CLASSIC — current state
@title: 🗡️ GAMEBOX-LINEAGE-CLASSIC — "리니지 클래식 R28"

@goal: Lineage Classic R28 native bring-up on macOS: native first-frame path + R28-specific loader track. ✅ lineage_classic_r28 play checkpoint landed.

ONLINE MMO — playable REQUIRES a live server login/realm connection (NOT offline).
The native client renders locally, but actual play = boot → login → realm/server → in-world.

## prerequisites

- GAMEBOX-PURPLE — DOWNLOAD/INSTALL only: plaync PURPLE is the SOLE acquisition channel
  for the client (no other download path). Runtime does NOT need the Purple shell.
- GAMEBOX-LOADER — PE load of LineageClassic.exe (pe_lineage_classic_r28_track) ✅ code present
- GAMEBOX-WIN32  — Win32 client API shim ✅ code present
- GAMEBOX-GFX    — Metal first frame (c_lineage_classic_r28_native_first_frame) ✅ code present
- GAMEBOX-NET    — ONLINE server login / realm connection (required for play) 🔲 verify

Runtime model (roadmap): "Native client (no Purple shell) — direct executable" — but ONLINE.
PLAYABLE GATE = GameGuard (INCA Internet) kernel anti-cheat, which guards the online login →
retired_intentional / honest-freeze (docs/LINEAGE_CLASSIC_R28_MANJEOM_REVIEW.md). first-frame
checkpoint proves local render only, NOT online playability.

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [x] R28-specific loader track + native first-frame (pe_lineage_classic_r28_track · c_lineage_classic_r28_native_first_frame · 12 markers)
- [x] Local first-frame checkpoint (lineage_classic_r28_play) — `.checkpoint.lineage_classic_r28_play` (LOCAL RENDER only, NOT online play)
- [x] Drift resolution track bv landed (gamebox_lineage_classic_r28_drift_resolution_track_bv)
- [ ] ONLINE login/realm path — server connection → in-world (the real playable gate; GameGuard kernel-AC honest-freeze where blocked)
