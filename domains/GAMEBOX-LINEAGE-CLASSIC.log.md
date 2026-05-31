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

## 2026-05-31 — correction: PURPLE is the download channel (runtime native)

- [x] CORRECTED prior "no launcher dep" claim. User domain knowledge: Lineage Classic
      client is DOWNLOADABLE ONLY via plaync PURPLE (sole acquisition channel).
- [x] Runtime confirmed native by roadmap: "Native client (no Purple shell) — direct
      executable". So execution does NOT require Purple; runtime blocker = GameGuard
      kernel AC (INCA Internet) → retired_intentional / honest-freeze.
- [x] Wired @link LINEAGE-CLASSIC --uses--> PURPLE scoped to download/install in DOMAINS.tape.

## 2026-05-31 — correction: Classic is an ONLINE MMO (not offline)

- [x] CORRECTED offline framing. Lineage Classic R28 is an online game — playable requires
      live server login/realm connection. Native client renders locally but is NOT offline-playable.
- [x] Wired @link LINEAGE-CLASSIC --uses--> GAMEBOX-NET (online login/realm).
- [x] Reframed playable gate: GameGuard (INCA) kernel AC guards the ONLINE login → honest-freeze.
      first-frame checkpoint = local render proxy only, NOT online play.
