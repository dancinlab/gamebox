# GAMEBOX-BNET — current state
@title: 🚀 GAMEBOX-BNET — "배틀넷 런처"

@goal: Battle.net launcher/platform bring-up: agent IPC, install/setup path, download (TACT/NGDP/CASC), OAuth token, and cold→warm launch orchestration for D2R/D4/CS2/SC2.

Consumers: GAMEBOX-D2R · GAMEBOX-D4 · GAMEBOX-CS2 (launch via Battle.net). D2R launch
path is checkpoint-proven (offline + online), so BNET is non-blocking for D2R bring-up.

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [x] Battle.net agent IPC + HTTP REST 4-verb + agent SD forging (pe_battle_net_agent_ipc_track · pe_battlenet_agent_http_rest · pe_battlenet_agent_sd_forging)
- [x] Setup/install path real + static-CRT path (pe_battlenet_setup_install_path_real · pe_battlenet_setup_static_crt_path · pe_battle_net_setup_install_phase)
- [x] TACT/NGDP/CASC download track + CDN mirror select (pe_battlenet_tact_ngdp_casc_track · pe_battle_net_download_track)
- [x] OAuth token + apple-network HTTP transport (pe_battle_net_oauth_token · pe_battlenet_apple_network_http)
- [x] Real launch orchestrator → D2R offline + online checkpoints (pe_real_launch_orchestrator · `.checkpoint.d2r_battlenet_offline_play` + `.checkpoint.d2r_battlenet_online_play`); online honest-freeze where Warden/anti-cheat blocks
- [ ] Extend launch path to D4 / CS2 (only D2R checkpoint-proven so far)
