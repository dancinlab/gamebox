# GAMEBOX-D2R — current state
@title: 😈 GAMEBOX-D2R — "디아2 레저렉션"

@goal: Diablo II Resurrected end-to-end on macOS: D3D11 renderer init, MPQ archive load, D2S savegame, offline + Battle.net online play. ✅ d2r offline/online checkpoints landed.

## prerequisites (engine subsystems · all code present)

- GAMEBOX-LOADER — PE map + file (pe_d2r_mpq_archive_load · pe_d2r_savegame_d2s_format) ✅ code present
- GAMEBOX-WIN32  — kernel32/user32/ntdll shim (stage2 + tier1_real) ✅ code present
- GAMEBOX-GFX    — D3D11→Metal (pe_d2r_d3d11_renderer_init · 12 d3d11_real · router) ✅ code present
- GAMEBOX-NET    — realm protocol / winsock (online; honest-freeze where blocked) ✅ code present
- GAMEBOX-BNET   — Battle.net launcher (pe_real_launch_orchestrator) ✅ code present

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [x] D3D11 renderer init + MPQ archive load + D2S savegame format (pe_d2r_d3d11_renderer_init · pe_d2r_mpq_archive_load · pe_d2r_savegame_d2s_format)
- [x] Offline play checkpoint (Battle.net offline shim) — `.checkpoint.d2r_battlenet_offline_play`
- [x] Online play checkpoint (realm protocol — honest-freeze where blocked) — `.checkpoint.d2r_battlenet_online_play` + docs/D2R_REALM_PROTOCOL_HONEST_FREEZE.md
- [ ] Gameplay tracks (26 c_d2r_* present): act progression, runeword/horadric recipe, drop tables, terror zone — verify each track end-to-end
