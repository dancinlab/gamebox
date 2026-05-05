# airgenome-gamebox hexa-lang upstream cross-project audit (Track AX) — 2026-05-04

> Track AX 는 두 sibling project 의 hexa-lang upstream candidate ledger 를 cross-reference 하여
> overlap / gap / canonical naming difference 를 식별하는 doc-only audit cycle. sibling project
> ledger (`~/core/airgenome/docs/hexa_lang_upstream_candidates.md`) 는 read-only.

## 1. user directive paraphrase (BR no verbatim)

두 sibling project (airgenome-gamebox + airgenome) 가 각각 maintain 하는 hexa-lang upstream
candidate ledger 사이의 cross-project 정합 audit. 두 ledger 의 entry 단위 cross-reference matrix +
overlap entry 식별 + gap entry 식별 + canonical naming difference 식별 + own 5 status 비교 + PR
coordination posture 도큐 + ≥8 caveat. doc-only / additive / migration 0 / destructive 0 / sibling
project ledger 0 modifications. hexa-lang sibling repo touch 0.

## 2. 두 ledger inventory 요약

| Project | Ledger path | LOC | Entry 수 | Discovery 출처 |
|---|---|---|---|---|
| airgenome-gamebox (this repo) | `docs/HEXA_UPSTREAM.md` | 558 | 20 | M0 instrumentation + 자율 구현 + Track AO RETROFIT + Track AV B-bench |
| airgenome (sibling, read-only) | `~/core/airgenome/docs/hexa_lang_upstream_candidates.md` | 385 | 15 | wave 1-4 (modules/harvest, claude, transport, process gate) + bench_site |

## 3. cross-reference matrix 결과

- **overlap entries**: 3 (gamebox §1.2/§6.2/§6.4 ↔ airgenome A14, gamebox §1.5 ↔ airgenome A13,
  gamebox §6.5 ↔ airgenome A8/A12).
- **gap entries**: 26 (gamebox-only 15 + airgenome-only 11).
- **canonical naming differences**: 4 (§5.1 std_json vs json_field_*, §5.2 fs_* vs pfs_*, §5.3
  jsonl_iter 미정의, §5.4 stderr language-level vs wrapper-level).

## 4. own 5 status 비교

- gamebox: 3 entry C+B (Track AV PR-ready pending), 3 C-only deferred, 1 forward-looking, 14
  discovery-only.
- airgenome: 1 landed (A6), 4 bench measured (A1/A2/A3/A5), ~8 C-hit multiple, 2 airgenome-side fix.
- 양쪽 모두: PR submission pending user explicit approval.

## 5. PR coordination posture

본 audit 는 hexa-lang sibling repo (`/Users/ghost/core/hexa-lang`) PR queue 측에 변경 0. cross-project
sync 가 필요한 surface 3개 식별 (§3.1 std_json + §3.2 filesystem prefix + §3.3 stderr layer). 다음
sync cycle 에서 canonical naming convention 합의 후 PR 작성 권고. 첫 PR 후보는 airgenome A2 (xxh64
— sibling repo stdlib 이미 존재) 가 가장 risk-free.

## 6. 새 파일 / 변경 요약

| 파일 | op | LOC | 비고 |
|---|---|---|---|
| `docs/HEXA_UPSTREAM_CROSS_PROJECT_AUDIT.md` | NEW | 396 | §0-§9 audit 본문 |
| `airgenome/doc/airgenome_gamebox_hexa_upstream_cross_project_audit_track_ax_landed_2026_05_04.ai.md` | NEW | 본 파일 | handoff |
| `state/markers/airgenome_gamebox_hexa_upstream_cross_project_audit_track_ax_landed.marker` | NEW | — | sha256 pinned |

기존 ledger 변경 0:
- `docs/HEXA_UPSTREAM.md` — read-only (Track AO/AV territory).
- `~/core/airgenome/docs/hexa_lang_upstream_candidates.md` — read-only (sibling project).
- `state/markers/airgenome_gamebox_hexa_upstream_retrofit_track_ao_landed.marker` — read-only.
- `state/markers/airgenome_gamebox_hexa_upstream_b_bench_track_av_landed.marker` — read-only.
- hexa-lang sibling repo — touch 0.

## 7. caveats (요약 — full list 는 audit doc §8 참고)

1. C1 — sibling project ledger read-only, 한 줄도 수정 안 함.
2. C2 — ledger 갱신 시 cross-reference matrix 부분 stale.
3. C3 — concept overlap 판정 주관성 (audit 관점).
4. C4 — canonical naming 권고 권한 부재 (sibling repo maintainer 결정).
5. C5 — gamebox §1.x M0-cycle entry 의 stale 가능성 (sibling repo touch 0 정책).
6. C6 — airgenome A6 landed 상태 sibling repo 검증 안 함.
7. C7 — airgenome wave 4 panic / wave 1 timeout 의 gamebox 측 부재.
8. C8 — gamebox §6.3 Win32 loader 영역 airgenome 외.
9. C9 — gamebox §1.10 / §6.1 float runtime 의존성.
10. C10 — Track AV B-bench native estimate 의 unmeasurable 정합.
11. C11 — airgenome A9 / A11 의 hexa-lang 측 변경 부재.
12. C12 — 본 audit 자체 own 5 ledger 항목 아님.
13. C13 — sibling repo PR queue 측 stale 가능성 (다른 sister project entry 가능).
14. C14 — airgenome ledger 의 own 5 ban 정합 검증 안 함.
15. C15 — 두 ledger 의 wording 정합 시점 차이.

## 8. forward-looking

- 두 ledger 중 하나라도 entry 추가 / 삭제 / wording 변경 시 → 본 audit 재실행 cycle (Track AX successor).
- hexa-lang sibling repo land 시 → ledger + audit 갱신.
- user explicit PR approval 시 → audit 가 PR coordination 도큐로 전환 (canonical naming 합의 후).
- 다른 sister project (anima/n6/nexus/hive) ledger 합류 시 → N-way audit 확장.

## 9. policy 정합 cite

- migration: forbidden.
- changes: additive_only / in_place_writes=0.
- destructive_ops: 0.
- cost_usd: 0.
- substrate: mac-local.
- cap_minutes: 90 — within.
- BR no user verbatim: true (paraphrase 정합).
- friendly_preset: true.
- silent-land: true.
- own 1: hexa-lang sibling 0 touch / airgenome sibling project ledger 0 modification / Wine 0.
- own 2: cross-project audit honest — naming gap + scope difference + sibling repo PR posture
  명시 acknowledge.
- raw 9: doc-only.
- raw 10: ≥8 caveats (≥15 land).
- raw 11: snake_case marker keys.
- raw 12: silent-error-ban (audit 의 read-only 한계 명시).
- raw 15: env lazy.
- raw 175: BR no user verbatim 정합.

## 10. verification

- `wc -l docs/HEXA_UPSTREAM_CROSS_PROJECT_AUDIT.md` = 396 (250-400 range).
- marker JSON parses (별도 marker 파일).
- marker `airgenome_ledger_modified: false` (audit-only).
- sha256 pinned (audit doc + handoff + marker self).

---

*written 2026-05-04. Track AX cross-project audit handoff. doc-only / additive / migration 0 /
destructive 0 / sibling project ledger modifications 0 / hexa-lang sibling repo touch 0.*
