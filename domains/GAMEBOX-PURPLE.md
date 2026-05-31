# GAMEBOX-PURPLE — current state
@title: 💜 GAMEBOX-PURPLE — "퍼플 런처"

@goal: plaync PURPLE launcher/platform for Lineage M/W: 2FA TOTP, plaync OAuth proxy, WebView2 warm, .NET CLR probe, Android-emulation layer for mobile titles.

Consumers: GAMEBOX-LINEAGE-M · GAMEBOX-LINEAGE-W (launch via plaync PURPLE) — both
checkpoint-proven, so PURPLE is non-blocking for M/W. ALSO GAMEBOX-LINEAGE-CLASSIC
depends on PURPLE for DOWNLOAD/INSTALL only (sole acquisition channel); Classic's
runtime is native ("no Purple shell"), so PURPLE does not gate Classic execution.

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [x] plaync OAuth proxy + 2FA TOTP proxy (c_purple_plaync_oauth_proxy · c_purple_2fa_totp_proxy)
- [x] WebView2 warm + .NET CLR probe (c_purple_webview2_warm · c_purple_dotnet_clr_probe)
- [x] Android emulation layer track (c_purple_android_emul_layer_track — Lineage M APK)
- [x] purple launcher offline shim → Lineage M/W purple checkpoints (`.checkpoint.lineage_m_purple_play` + `.checkpoint.lineage_w_purple_play`)
