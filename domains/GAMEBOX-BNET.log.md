# GAMEBOX-BNET — log

Append-only history sister of `GAMEBOX-BNET.md`. Each entry starts with `## <ISO timestamp> — <header>` (newest on top); body = `- [x]` (done) / `- [ ]` (pending) checkbox tasks.

## 2026-05-31 — domain init (seeded from gamebox 전수조사)

- [x] Domain scaffolded under `domains/` as a node of the GAMEBOX tree (see `domains/GAMEBOX.md`).
- [x] @goal + milestones seeded from `.roadmap.*` SSOT + repo survey (lib/loader · lib/perf · native).
- [ ] Refine milestones against code (mark `[x]` what already landed; checkpoints `.checkpoint.*` are evidence).

## 2026-05-31 — reconcile (launcher non-blocking for D2R)

- [x] Reconciled milestones to evidence: 10 pe_battle(_)net_* modules + orchestrator present; D2R offline+online checkpoints prove the full launch path works end-to-end → 5/6 [x].
- [x] Clarified D2R prerequisite: BNET is checkpoint-proven for D2R, so it does NOT block D2R bring-up. Open item = extend launch path to D4/CS2.
- [ ] Online realm honest-freeze where Warden/anti-cheat blocks (docs/WARDEN_HONEST_FREEZE.md · GAMEGUARD).
