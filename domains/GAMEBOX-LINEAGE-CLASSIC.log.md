# GAMEBOX-LINEAGE-CLASSIC — log

Append-only history sister of `GAMEBOX-LINEAGE-CLASSIC.md`. Each entry starts with `## <ISO timestamp> — <header>` (newest on top); body = `- [x]` (done) / `- [ ]` (pending) checkbox tasks.

## 2026-05-31 — domain init (seeded from gamebox 전수조사)

- [x] Domain scaffolded under `domains/` as a node of the GAMEBOX tree (see `domains/GAMEBOX.md`).
- [x] @goal + milestones seeded from `.roadmap.*` SSOT + repo survey (lib/loader · lib/perf · native).
- [ ] Refine milestones against code (mark `[x]` what already landed; checkpoints `.checkpoint.*` are evidence).

## 2026-05-31 — prerequisite wiring + reconcile

- [x] Wired engine prerequisite @uses edges in DOMAINS.tape: LINEAGE-CLASSIC --uses--> LOADER · WIN32 · GFX (native client — no launcher dep).
- [x] Reconciled milestones to evidence: r28 loader track + native first-frame (12 markers) + play checkpoint + drift-resolution track bv landed → 3/4 [x]. Client protocol/login left open.
- [x] Prerequisite readiness: LOADER/WIN32/GFX code present. LINEAGE-CLASSIC is startable (native first-frame already checkpointed).
