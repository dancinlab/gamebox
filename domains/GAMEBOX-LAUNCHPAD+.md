# GAMEBOX-LAUNCHPAD+ — current state
@title: 🛫 GAMEBOX-LAUNCHPAD+ — "두 게임 빠른 출시대"

@goal: Get GAMEBOX-D2R + GAMEBOX-LINEAGE-CLASSIC to PLAYABLE ("도달") as fast as possible. Focused sprint meta-domain (`+` = composition) over the two near-ready titles + their shared prerequisites. DONE = both titles flipped to playable in their own domains.

## composed targets (rollup)

```
GAMEBOX-LAUNCHPAD+  (sprint meta)
├─ targets ─▶ GAMEBOX-D2R              prereqs ✅ green · offline+online checkpoint
│                                       남은 일: 게임플레이 루프 도달 · online=Warden freeze
└─ targets ─▶ GAMEBOX-LINEAGE-CLASSIC  prereqs ✅ green · native first-frame checkpoint
                                        남은 일: 런타임 플레이 도달 · GameGuard freeze
                                        (다운로드=PURPLE deferred · 클라 확보 전제)
```

Shared prerequisites (both depend, all code present): GAMEBOX-LOADER · GAMEBOX-WIN32 · GAMEBOX-GFX.
D2R also: GAMEBOX-NET (online) · GAMEBOX-BNET (launch). Classic download: GAMEBOX-PURPLE (deferred).

## "도달(playable)" bar (goal predicate)

boot → title screen → character/login → in-world controllable loop (move + core interaction)
held for a smoke session, captured as a per-title checkpoint. Online/anti-cheat-gated content
is OUT of the playable bar where honest-freeze applies (Warden for D2R online, GameGuard for Classic).

## milestones

- [ ] Define + record the per-title "playable" smoke bar (boot→title→char→in-world) as checkpoints
- [ ] D2R: offline playable loop confirmed end-to-end (beyond launch checkpoint → real play); online Warden boundary documented
- [ ] LINEAGE-CLASSIC: native runtime playable loop (first-frame → in-world); GameGuard kernel-AC boundary documented; client assume-acquired (PURPLE download deferred)
- [ ] Shared prereqs (LOADER·WIN32·GFX) confirmed non-blocking for both during the sprint
- [ ] Both titles flipped to playable in their own domains → LAUNCHPAD+ goal met
