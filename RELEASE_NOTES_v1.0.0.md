# gamebox v1.0.0 — Initial Release (2026-05-05)

## Summary

Initial release of `gamebox` — Apple-native Win32 game runtime (hexa-only, Wine 0, D3DMetal-pinned).

Extracted from `dancinlife/airgenome-gamebox` after 67 BG cycles (2026-05-03 to 2026-05-05).

## Highlights

- **Forensic verdict**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`
- **Win32 fn coverage**: 435/435 (100%) — Track BP milestone 2026-05-05
- **Perf families**: 18+ at 100% manjeom (skeleton-tier, Track BL/BM forensic)
- **hexa-lang upstream**: 4 RFCs merged (rfc_012/_013/_014 PR #47 + rfc_015 PR #48)
- **License**: Apache-2.0

## Posture

- **own1**: Wine 0 / DRM bypass 0 / Warden bypass 0 / anti-cheat bypass 0 / anima clone 0
- **own2**: honest PASS/FAIL/PARTIAL emit; `validated_manjeom = 0` explicit (skeleton-only)
- **L1-L5 online expansion lock-in carries** per `.roadmap.battlenet`

## Caveats (raw#10 C3)

5 explicit honesty caveats — see [README §Caveats](README.md#caveats-raw10-c3).
TL;DR: skeleton-tier means modules compile/probe individually; no real game
frames have been evidenced. Real launch (`gamebox launch <game>`) returns
PARTIAL (`retired_intentional:skeleton_tier_only`) until closure CM-26..CM-30
lands.

## Source repo

Sibling: <https://github.com/dancinlab/sim-universe>
Predecessor (DELETED): `dancinlife/airgenome-gamebox` (history not transferred
per D3 fresh-start policy).

## Author

박민우 <nerve011235@gmail.com>
