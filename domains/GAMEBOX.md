# GAMEBOX — current state
@title: 🕹️ GAMEBOX — "맥에서 윈도우 게임 돌리는 통역기"

@goal: Run Windows PE games natively on macOS / Apple Silicon via a hexa-only stack — PE loader + Win32 API shim + DirectX→Metal — with per-game launch tracks. Umbrella/meta-root domain: carries the 메인구조(engine subsystems) + 런처(launchers) + 게임(per-game) tree below; each branch is a child domain registered in `DOMAINS.tape`.

## tree (20 domains · 1 root + 6 메인구조 + 2 런처 + 11 게임)

```
GAMEBOX  (meta-root)
├─ 🧱 메인구조 (engine subsystems)
│  ├─ GAMEBOX-LOADER     PE load · i386/x64 decode · IAT · SEH · TLS · MSVC CRT · reloc
│  ├─ GAMEBOX-WIN32      kernel32/user32/gdi32/ntdll/advapi32 … ~14 DLL API shim
│  ├─ GAMEBOX-GFX        D3D11/12 → Metal · DXGI · DXIL→AIR shaders
│  ├─ GAMEBOX-NET        winsock · IPC · TLS · OAuth · TACT/NGDP/CASC protocol
│  ├─ GAMEBOX-PERF       FPS overlay · frame pacing · display mode · metal opt
│  └─ GAMEBOX-NATIVE     C helpers: i386_decode · pe_parse · pe_to_macho_shim
├─ 🚀 런처/플랫폼 (launchers)
│  ├─ GAMEBOX-BNET       Battle.net launcher · agent IPC · install/download
│  └─ GAMEBOX-PURPLE     plaync PURPLE launcher · 2FA · OAuth
└─ 🎮 게임 (per-game)
   ├─ GAMEBOX-D2R              Diablo II Resurrected       ✅ checkpoint
   ├─ GAMEBOX-D4               Diablo IV
   ├─ GAMEBOX-ELDEN            Elden Ring
   ├─ GAMEBOX-NIGHTREIGN       Elden Ring Nightreign
   ├─ GAMEBOX-LINEAGE-CLASSIC  Lineage Classic R28         ✅ checkpoint
   ├─ GAMEBOX-LINEAGE-M        Lineage M (purple)          ✅ checkpoint
   ├─ GAMEBOX-LINEAGE-W        Lineage W (purple)          ✅ checkpoint
   ├─ GAMEBOX-CS2              Counter-Strike 2
   ├─ GAMEBOX-DELTA-FORCE      Delta Force
   ├─ GAMEBOX-WOT              World of Tanks (BigWorld)
   └─ GAMEBOX-WOWS             World of Warships (BigWorld)

🛫 sprint (meta · composition)
└─ GAMEBOX-LAUNCHPAD+   targets → D2R + LINEAGE-CLASSIC (빠른 플레이 도달)
```

Edges (`DOMAINS.tape` `@link`): `GAMEBOX --has--> <child>` for the tree above; plus
consumer edges — games `--uses-->` their launcher (D2R/D4/CS2→BNET, Lineage M/W→PURPLE)
and shared-engine reuse (NIGHTREIGN→ELDEN, WOWS→WOT). `WoW` was intentionally dropped.

Code SSOT per branch: `lib/loader/` (458 hexa: pe_* · win32_* · i386_* · pe_d3d* · ipc_*),
`lib/perf/` (245 hexa: c_* tracks), `native/` (C helpers), `.roadmap.<game>` + `.checkpoint.<x>`.

## milestones

- [x] Tree designed + 20 child domains scaffolded under `domains/` (this commit)
- [x] `DOMAINS.tape` roster + `@has`/`@uses` edges authored (navigable via `/domain list`)
- [ ] Engine subsystem domains (LOADER·WIN32·GFX·NET·PERF·NATIVE) milestones reconciled to code
- [ ] Launcher domains (BNET·PURPLE) milestones reconciled to code + checkpoints
- [ ] Per-game domains (11) milestones reconciled to `.roadmap.*` + `.checkpoint.*` (mark landed `[x]`)
