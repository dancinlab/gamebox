# DOMAINS.tape — gamebox domain roster + tree graph (g67 cross-domain)
# Roster rows (`@domain NAME := "path"`) are AUTHORITATIVE for the /domain CLI;
# every other line (@V · @link · #) is ignored by the roster parser, so the
# GAMEBOX tree below rides along without disturbing `domain list`.
# Tree carrier (g3 minimal): @link <parent> --has--> <child>   # <role>
@V := "domains" :: index

# --- roster (nodes) — GAMEBOX meta-root + 메인구조 + 런처 + 게임 ---
@domain GAMEBOX := "domains/GAMEBOX.md"

# 🧱 메인구조 (engine subsystems)
@domain GAMEBOX-LOADER := "domains/GAMEBOX-LOADER.md"
@domain GAMEBOX-WIN32  := "domains/GAMEBOX-WIN32.md"
@domain GAMEBOX-GFX    := "domains/GAMEBOX-GFX.md"
@domain GAMEBOX-NET    := "domains/GAMEBOX-NET.md"
@domain GAMEBOX-PERF   := "domains/GAMEBOX-PERF.md"
@domain GAMEBOX-NATIVE := "domains/GAMEBOX-NATIVE.md"

# 🚀 런처/플랫폼 (launchers)
@domain GAMEBOX-BNET   := "domains/GAMEBOX-BNET.md"
@domain GAMEBOX-PURPLE := "domains/GAMEBOX-PURPLE.md"

# 🎮 게임 (per-game)
@domain GAMEBOX-D2R             := "domains/GAMEBOX-D2R.md"
@domain GAMEBOX-D4              := "domains/GAMEBOX-D4.md"
@domain GAMEBOX-ELDEN           := "domains/GAMEBOX-ELDEN.md"
@domain GAMEBOX-NIGHTREIGN      := "domains/GAMEBOX-NIGHTREIGN.md"
@domain GAMEBOX-LINEAGE-CLASSIC := "domains/GAMEBOX-LINEAGE-CLASSIC.md"
@domain GAMEBOX-LINEAGE-M       := "domains/GAMEBOX-LINEAGE-M.md"
@domain GAMEBOX-LINEAGE-W       := "domains/GAMEBOX-LINEAGE-W.md"
@domain GAMEBOX-CS2             := "domains/GAMEBOX-CS2.md"
@domain GAMEBOX-DELTA-FORCE     := "domains/GAMEBOX-DELTA-FORCE.md"
@domain GAMEBOX-WOT             := "domains/GAMEBOX-WOT.md"
@domain GAMEBOX-WOWS            := "domains/GAMEBOX-WOWS.md"

# --- tree edges (GAMEBOX --has--> child · g67) ---
@link GAMEBOX --has--> GAMEBOX-LOADER            # 메인구조: PE load · i386/x64 · IAT · SEH · TLS · MSVC CRT
@link GAMEBOX --has--> GAMEBOX-WIN32             # 메인구조: Win32 API shim (~14 DLLs)
@link GAMEBOX --has--> GAMEBOX-GFX               # 메인구조: DirectX 11/12 → Metal · DXIL→AIR
@link GAMEBOX --has--> GAMEBOX-NET               # 메인구조: winsock · IPC · TLS · OAuth · TACT/NGDP/CASC
@link GAMEBOX --has--> GAMEBOX-PERF              # 메인구조: FPS overlay · frame pacing · display mode · metal opt
@link GAMEBOX --has--> GAMEBOX-NATIVE            # 메인구조: C helpers (i386_decode · pe_parse · pe_to_macho)
@link GAMEBOX --has--> GAMEBOX-BNET              # 런처: Battle.net (D2R · D4 · CS2 host)
@link GAMEBOX --has--> GAMEBOX-PURPLE            # 런처: plaync PURPLE (Lineage M/W host)
@link GAMEBOX --has--> GAMEBOX-D2R               # 게임: Diablo II Resurrected
@link GAMEBOX --has--> GAMEBOX-D4                # 게임: Diablo IV
@link GAMEBOX --has--> GAMEBOX-ELDEN             # 게임: Elden Ring
@link GAMEBOX --has--> GAMEBOX-NIGHTREIGN        # 게임: Elden Ring Nightreign
@link GAMEBOX --has--> GAMEBOX-LINEAGE-CLASSIC   # 게임: Lineage Classic R28
@link GAMEBOX --has--> GAMEBOX-LINEAGE-M         # 게임: Lineage M (purple)
@link GAMEBOX --has--> GAMEBOX-LINEAGE-W         # 게임: Lineage W (purple)
@link GAMEBOX --has--> GAMEBOX-CS2               # 게임: Counter-Strike 2
@link GAMEBOX --has--> GAMEBOX-DELTA-FORCE       # 게임: Delta Force
@link GAMEBOX --has--> GAMEBOX-WOT               # 게임: World of Tanks (BigWorld)
@link GAMEBOX --has--> GAMEBOX-WOWS              # 게임: World of Warships (BigWorld)

# --- consumer edges (games/launchers --uses--> subsystems · g67) ---
@link GAMEBOX-D2R   --uses--> GAMEBOX-BNET       # launches via Battle.net
@link GAMEBOX-D4    --uses--> GAMEBOX-BNET
@link GAMEBOX-CS2   --uses--> GAMEBOX-BNET
@link GAMEBOX-LINEAGE-M --uses--> GAMEBOX-PURPLE # launches via plaync PURPLE
@link GAMEBOX-LINEAGE-W --uses--> GAMEBOX-PURPLE
@link GAMEBOX-NIGHTREIGN --uses--> GAMEBOX-ELDEN # shares Elden Ring engine base
@link GAMEBOX-WOWS  --uses--> GAMEBOX-WOT         # shares BigWorld engine + WG net stack

# --- engine prerequisite edges (game --uses--> subsystem · what a game must stand on) ---
# D2R prerequisites (all subsystem code present: mapper/IAT/SEH/TLS real · kernel32 stage2 · d2r_d3d11 · winsock · launch orchestrator)
@link GAMEBOX-D2R --uses--> GAMEBOX-LOADER       # pe_d2r_mpq_archive_load · pe_d2r_savegame_d2s_format (PE map + file)
@link GAMEBOX-D2R --uses--> GAMEBOX-WIN32        # kernel32/user32/ntdll API shim
@link GAMEBOX-D2R --uses--> GAMEBOX-GFX          # pe_d2r_d3d11_renderer_init · c_d2r_dx11_pipeline_warm (D3D11→Metal)
@link GAMEBOX-D2R --uses--> GAMEBOX-NET          # realm protocol / winsock (online play · honest-freeze where blocked)
# LINEAGE-CLASSIC prerequisites (native client — no launcher)
@link GAMEBOX-LINEAGE-CLASSIC --uses--> GAMEBOX-LOADER  # pe_lineage_classic_r28_track (PE load)
@link GAMEBOX-LINEAGE-CLASSIC --uses--> GAMEBOX-WIN32   # Win32 client API shim
@link GAMEBOX-LINEAGE-CLASSIC --uses--> GAMEBOX-GFX     # c_lineage_classic_r28_native_first_frame (Metal first frame)
@link GAMEBOX-LINEAGE-CLASSIC --uses--> GAMEBOX-PURPLE  # DOWNLOAD/INSTALL only (plaync PURPLE is the sole acquisition channel); runtime is native direct .exe "no Purple shell" (roadmap) — runtime blocker = GameGuard kernel AC, not Purple
