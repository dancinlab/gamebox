# GAMEBOX-D2R — log

Append-only history sister of `GAMEBOX-D2R.md`. Each entry starts with `## <ISO timestamp> — <header>` (newest on top); body = `- [x]` (done) / `- [ ]` (pending) checkbox tasks.

## 2026-05-31 — domain init (seeded from gamebox 전수조사)

- [x] Domain scaffolded under `domains/` as a node of the GAMEBOX tree (see `domains/GAMEBOX.md`).
- [x] @goal + milestones seeded from `.roadmap.*` SSOT + repo survey (lib/loader · lib/perf · native).
- [ ] Refine milestones against code (mark `[x]` what already landed; checkpoints `.checkpoint.*` are evidence).

## 2026-05-31 — prerequisite wiring + reconcile

- [x] Wired engine prerequisite @uses edges in DOMAINS.tape: D2R --uses--> LOADER · WIN32 · GFX · NET (BNET already present).
- [x] Reconciled milestones to evidence: D3D11/MPQ/savegame code present + offline/online checkpoints landed → 3/4 [x]. Gameplay-tracks (26 c_d2r_*) left open pending per-track e2e verify.
- [x] Prerequisite readiness probe: all 5 subsystems have real code (mapper/IAT/SEH/TLS real · kernel32 stage2 · d2r_d3d11 + 12 d3d11_real + router · winsock 5 · launch orchestrator). D2R is startable.
