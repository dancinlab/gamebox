# GAMEBOX-LAUNCHPAD+ — current state
@title: 🛫 GAMEBOX-LAUNCHPAD+ — "두 게임 빠른 출시대"

@goal: Get GAMEBOX-D2R + GAMEBOX-LINEAGE-CLASSIC to PLAYABLE ("도달") as fast as possible. Focused sprint meta-domain (`+` = composition) over the two near-ready titles + their shared prerequisites. DONE = both titles flipped to playable in their own domains.

## composed targets (rollup)

```
GAMEBOX-LAUNCHPAD+  (sprint meta)
├─ targets ─▶ GAMEBOX-D2R              prereqs ✅ green · offline+online checkpoint
│                                       남은 일: 게임플레이 루프 도달 · online=Warden freeze
└─ targets ─▶ GAMEBOX-LINEAGE-CLASSIC  ONLINE MMO · local first-frame ✅ · online play 🔲
                                        남은 일: 서버 로그인→렐름→인월드 · GameGuard 관문
                                        (다운로드=PURPLE deferred · 클라 확보 전제)
```

BOTH titles are ONLINE games — "playable" needs a live server connection, gated by kernel
anti-cheat (D2R=Warden · Classic=GameGuard). Neither is offline. The honest-freeze on those
kernel-AC layers is the hard wall for full online play.

Shared prerequisites (all code present): GAMEBOX-LOADER · GAMEBOX-WIN32 · GAMEBOX-GFX · GAMEBOX-NET.
D2R also GAMEBOX-BNET (launch). Classic download: GAMEBOX-PURPLE (deferred).

## "도달(playable)" bar (goal predicate)

boot → title → character/login → **online server/realm** → in-world controllable loop, held for
a smoke session, captured as a per-title checkpoint. Because both titles are online, the realistic
near-term bar = reach the online-login boundary; full in-world online play is gated by kernel
anti-cheat honest-freeze (Warden / GameGuard) and may be a hard stop, recorded honestly.

## milestones

- [ ] Define + record the per-title "playable" smoke bar (boot→title→char→in-world) as checkpoints
- [ ] D2R: offline playable loop confirmed end-to-end (beyond launch checkpoint → real play); online Warden boundary documented
- [ ] LINEAGE-CLASSIC (ONLINE MMO): drive to online-login/realm boundary (local first-frame already ✅); GameGuard kernel-AC honest-freeze documented as the play gate; client assume-acquired (PURPLE download deferred)
- [ ] Shared prereqs (LOADER·WIN32·GFX·NET) confirmed non-blocking for both during the sprint
- [ ] Both titles flipped to playable in their own domains → LAUNCHPAD+ goal met
