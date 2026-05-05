# `tool/roadmap_op.hexa` 도입 결정 매트릭스 — 3-way tradeoff (A clone / B self-impl / C symlink)

> **status**: 결정 doc (decision-only) / 작성 **2026-05-03** / 본 cycle 은 **구현 X**, 매트릭스 + 권고 + 사용자 lock-in 대기
> **scope**: 11 .roadmap.<game> + 3 .roadmap.<loader-domain> = 14 mk2 JSONL roadmap header file 측 CLI 운영 도구 도입 결정
> **결론 (요약, confidence MEDIUM-HIGH)**: **Option B (self-impl)** 권고 — own1 (Wine 0 / 외부 의존 0) + own2 (honest emit, repo-local SSOT) 정합. **Option A (anima clone)** 은 dev cost 측 prohibitive 시 fallback. **Option C (symlink)** 은 cross-repo path coupling + standalone clone safety 측 brittleness 측 제외 권고.
> **lock-in pending**: 사용자 A/B/C 선택 + (B 선택 시) 구현 cycle 추정 / (A 선택 시) anima version + branch reference / (C 선택 시) symlink target 절대경로 정책

---

## §1 Background — 왜 `tool/roadmap_op.hexa` 가 필요한가

### §1.1 현재 상태 (manual / ad-hoc 편집)

본 repo 의 mk2 roadmap 시스템은 다음 14 개 JSONL header file 로 구성되어 있다 (predecessor `airgenome_gamebox_per_game_roadmaps_landed.marker` 측 11 개 + 기존 3 개 loader-domain).

- per-game (11): `.roadmap.diablo2_resurrected` / `.roadmap.diablo4` / `.roadmap.battlenet` / `.roadmap.wow` / `.roadmap.cs2` / `.roadmap.delta_force` / `.roadmap.elden_ring` / `.roadmap.lineage_classic_r28` / `.roadmap.lineage_m` / `.roadmap.lineage_w` / `.roadmap.purple_launcher`
- loader-domain (3): `.roadmap.loader_pe` / `.roadmap.loader_win32` / `.roadmap.loader_dx`

각 file 측 운영 (cond status promotion partial→met / evidence row append / blocker status_enum_adopted 정식 채택 flag setting / cross-link object update / cycle hash pinning) 은 **모두 hand-editing** 으로 진행되고 있다.

### §1.2 누적 risk (predecessor caveat 측 인용)

- `airgenome_gamebox_self_mk2_tuning_landed.marker` C8: "anima tool/roadmap_op.hexa (573 LOC at that time, 11 subcmd) is absent in gamebox — user lock-in needed"
- `airgenome_gamebox_per_game_roadmaps_landed.marker` next_cycle_actions: "tool/roadmap_op.hexa adoption decision (anima clone / self-impl / symlink — accumulating caveat)"
- `airgenome_gamebox_rank_a_3_domain_landed.marker` next_cycle_actions: "tool/roadmap_op.hexa adoption decision (anima clone / self-impl / symlink)"
- `airgenome_gamebox_purple_lineage_offline_shim_landed.marker` / `airgenome_gamebox_gameguard_honest_freeze_landed.marker`: "mk2 roadmap_op tool 도입 결정 (anima clone / 자체 구현 / symlink — 누적 caveat C8 of self_mk2 predecessor)"
- `airgenome_gamebox_4game_retired_intentional_adoption_landed.marker`: "`tool/roadmap_op.hexa` 측 enum validator 추가 결정"

### §1.3 hand-editing 측 노출된 field-format 정합성 risk

본 cycle 와 병렬 / 직전 cycle 측 다음 두 정합성 issue 가 발견되었다:

1. **lineage_classic_r28 status field bool vs string drift** (Track G 측 정상화 진행) — `status_enum_adopted: true` (bool, 잘못) vs `status_enum_adopted: "retired_intentional"` (string, 정합) 측 hand-editing 측 누적 drift.
2. **3 cond evidence row append 측 quote escaping inconsistency** — 일부 row 측 single quote, 일부 double quote, 일부 mixed (JSONL 측 strict double-quote required).

→ **CLI tool 부재** = 위 같은 schema field-format inconsistency 가 **silent-error ban (raw 12)** 위반으로 누적되는 구조적 root cause. tool 도입 시 schema validate + atomic write 측 위 risk 제거 가능.

### §1.4 mk2 status enum spec freeze 측 의존

- predecessor `4game_retired_intentional_adoption_landed.marker` next_cycle_actions: enum validator 추가 결정 — tool 부재 = enum freeze 정식화 측 도구 backing 없음.

---

## §2 3-way option analysis

### §2.1 Option A — anima clone

**정의**: `nexus/anima/` (혹은 sibling `../anima/`) 측 `tool/roadmap_op.hexa` 측 file 단위 복제. 본 repo `tool/roadmap_op.hexa` 측 한 번 sync, 이후 본 repo 가 SSOT 측 보유.

**Pros**:
- 검증된 codebase (809 LOC, 11 subcmd, anima 측 long-term 운영). audit cycle 측 burden 감소.
- 적은 dev cost — 1 cycle (clone + path 재설정 + selftest 재실행) 측 land 가능 추정.
- 11 subcmd dispatcher 측 본 repo 측 즉시 적용 가능 (add / update / remove / list / show / link / domains / verify / render / selftest / track-new).

**Cons**:
- **anima 의존 확장** — 이전까지 본 repo 는 anima 와 own-domain 분리 (per `airgenome_gamebox_self_mk2_tuning_landed.marker`: `anima_sister: false`).
- **version coupling** — anima 측 후속 변경 시 sync drift 측 누적 risk. 매 cycle 측 "본 repo tool 측 anima 측 어느 commit?" 추적 burden.
- **hexa dialect compatibility risk** — anima 측 `tool/_roadmap_repo_resolver.hexa` 등 helper file 측 의존, 본 repo 측 hexa dialect / stdlib 측 미세 차이 시 selftest fail 가능. (anima 측 `REPO_ROOT_DEFAULT = "/Users/<user>/core/anima"` 같은 default 측 본 repo 측 부적합 — 수정 필요.)
- **upstream license / attribution** — anima 측 license 측 본 repo plugin 측 호환 여부 별도 audit 필요.

### §2.2 Option B — self-impl (자체 구현)

**정의**: 본 repo 측 `tool/roadmap_op.hexa` 측 self-implementation. anima 측 file 측 참조 없이 본 repo 스펙 측 처음부터 작성. (anima 측 sibling reference 측 inspiration 으로만 활용 가능, 그러나 file 측 import / clone X.)

**Pros**:
- **zero external dependency** — own1 (Wine 0 / 외부 runtime 0) 정의 측 정합. tool 자체는 admin tool 이므로 own1 의 게임-실행 측면 위반 X 이지만, **own1 의 "외부 repo / runtime 의존 0" 원칙** 측 정합.
- **repo-local SSOT** — 본 repo standalone clone 시 즉시 동작. cross-repo path coupling 0.
- **hexa-native** — 본 repo `lib/loader/*.hexa` + `lib/perf/*.hexa` 와 동일 hexa dialect / stdlib 측 sync. dialect drift risk 0.
- **scope tailored** — 본 repo 측 14 file (per-game 11 + loader-domain 3) 측 정합 도구 측 처음부터 설계 가능. anima 측 6-domain 가정 (clm/eeg/akida/qrng/sim/meta) 측 부적합 영역 수정 burden 0.
- **future-proof schema field add** — 본 repo 가 first-mover 인 schema field (`status_enum_adopted`, `retired_intentional_posture`, `adoption_evidence`, `adopted_iso`, `blocker_class`) 측 validator 측 본 repo 측 자유롭게 추가 가능 — anima 측 round-trip 의존 없음.

**Cons**:
- **dev cost** — 11 subcmd dispatcher 측 처음부터 구현 측 ≥ 2 cycle 추정 (subcmd skeleton 1 cycle + selftest 1 cycle, 단 minimum-viable subset 측 1 cycle 가능).
- **drift risk vs anima sibling** — 본 repo + anima 측 mk2 roadmap schema 측 분리되면 cross-repo audit 측 schema diff 측 burden 누적 가능.
- **test burden duplicates anima** — anima 측 selftest S1..S11 측 본 repo 측 재구현 burden (다만 본 repo scope 측 축소 시 selftest 측 축소 가능).
- **bug surface** — 새 코드 측 bug 측 노출 surface area 증가 (mitigate: minimum-viable subset 측 land + cycle-by-cycle 확장).

### §2.3 Option C — symlink

**정의**: 본 repo `tool/roadmap_op.hexa` 측 anima `tool/roadmap_op.hexa` 측 symlink (e.g., `ln -s ../../anima/tool/roadmap_op.hexa tool/roadmap_op.hexa`). 본 repo 측 file 0 byte (link only).

**Pros**:
- **zero copy** — anima 가 SSOT, 본 repo 측 영구 sync (자동).
- **single SSOT** — anima 측 update 측 본 repo 즉시 반영, drift 0.
- **dev cost** — 1 line shell command (`ln -s ...`).

**Cons**:
- **cross-repo path coupling** — 본 repo 측 anima 측 절대경로 / 상대경로 측 hard-coded. 사용자 측 layout 측 변경 시 (e.g., `~/work/airgenome-gamebox` + `~/anima` → `~/projects/airgenome-gamebox` + `~/projects/anima`) symlink 측 broken.
- **standalone clone safety 측 위반** — 본 repo 측 git clone 측 anima 측 부재 시 tool 측 fail. plugin distribution 측 치명 — 사용자 측 plugin 만 install 시 tool 측 dead.
- **symlink-in-git brittleness** — git 측 symlink 측 OS 별 (Windows 측 특히) 취급 차이 측 portability risk. macOS 측 정상 동작 가정 측 우호적이지만 raw 15 (env_lazy) 측 portability 위반 가능.
- **dialect drift risk** — anima 측 hexa dialect 측 본 repo 측 부정합 시 symlink 측 즉시 선언적 fail 점프. self-impl 측 본 repo 측 dialect 정합 측 control 가능 vs symlink 측 control 불가.
- **cross-repo cycle-by-cycle audit burden** — 본 repo cycle 측 anima 측 현재 commit 측 frozen point 측 marker 측 pin 필요 (symlink 자체는 "current HEAD" 측 mutable). marker 측 reproducibility 측 위반 가능.

---

## §3 결정 매트릭스 (low=1 / med=3 / high=5; 점수 낮을수록 정합)

> **scoring direction**: 본 매트릭스 측 모든 criteria 측 **낮을수록 better**. 즉 dev_cost 1 = 매우 낮음 (good), drift_risk 5 = 매우 높음 (bad), SSOT_purity 측 reverse score (5 = 매우 낮음 = bad). 명료성 측 매 row 측 "good direction" column 명시.

| criterion | A: anima clone | B: self-impl | C: symlink | good direction | 비고 |
|---|---|---|---|---|---|
| **dev_cost** | 1 (low) | 3 (med) | 1 (low) | 낮을수록 better | A clone = 1 cycle 추정; B = ≥ 2 cycle (minimum-viable subset 측 1 cycle 가능 단정 측 honest 하지 않음); C = 1 line |
| **external_dependency_count** | 3 (med) | 1 (low) | 5 (high) | 낮을수록 better | A = anima 측 file frozen-copy 의존 (sync 시점만); B = 0; C = anima 측 영구 의존 |
| **drift_risk** | 5 (high) | 3 (med) | 1 (low) | 낮을수록 better | A clone 측 sync drift 누적; B = 본 repo 측 자체 evolution; C = symlink = 영구 sync (drift 0) |
| **test_burden** | 3 (med) | 5 (high) | 1 (low) | 낮을수록 better | A = clone 직후 selftest 한 번; B = 본 repo selftest 작성 (S1..Sn); C = anima selftest 측 inherit |
| **standalone_clone_safety** | 1 (low risk) | 1 (low risk) | 5 (high risk) | 낮을수록 better | A + B = 본 repo standalone safe; C = anima 측 부재 시 fail |
| **hexa_native_alignment** | 3 (med) | 1 (low risk) | 3 (med) | 낮을수록 better (1 = 완전 정합) | A clone = anima dialect 측 본 repo 측 minor patch 필요; B = 본 repo dialect first-class; C = anima dialect mismatch 측 control X |
| **SSOT_purity** | 3 (med) | 1 (low) | 5 (high) | 낮을수록 better (1 = 본 repo 측 SSOT) | A clone = sync 시점 측 본 repo SSOT 단 anima 측 ghost 의존; B = 본 repo SSOT pure; C = anima 측 SSOT (본 repo 측 client 만) |
| **version_coupling** | 5 (high) | 1 (low) | 5 (high) | 낮을수록 better | A clone = anima 측 어느 commit 측 frozen 추적 burden; B = 0; C = anima HEAD 측 영구 coupled |
| **plugin_distribution_safety** | 1 (low) | 1 (low) | 5 (high) | 낮을수록 better | A + B = self-contained plugin install 측 정상; C = anima 측 미배포 시 plugin dead |
| **own1_alignment (Wine 0 / 외부 runtime 0)** | 3 (partial) | 1 (full) | 3 (partial) | 낮을수록 better (1 = 완전 정합) | tool 자체는 게임-실행 X 라 own1 영향 적지만 own1 의 "외부 repo 의존 0" 정신 측 B 가 정합 |
| **own2_alignment (honest emit + log/error 명시)** | 3 (med) | 1 (low) | 3 (med) | 낮을수록 better | A = anima 측 own2 measure 측 별도 audit 필요; B = 본 repo own2 first-class; C = symlink 측 own2 control X |
| **합계 (낮을수록 better, 11 criteria)** | **31** | **19** | **35** | — | B (self-impl) 측 19 = 최소 (best) |

### §3.1 합계 해석

- **B self-impl = 19** (best, 11 criteria 평균 1.7 — 거의 모든 항목 low)
- **A anima clone = 31** (medium, 11 criteria 평균 2.8 — drift / version_coupling / external_dependency 측 높음)
- **C symlink = 35** (worst, 11 criteria 평균 3.2 — standalone_clone_safety / external_dependency / SSOT_purity / version_coupling / plugin_distribution_safety 측 모두 high)

### §3.2 매트릭스 측 비대칭 측 honest disclosure (own2 정합)

- **B self-impl 측 single weakness**: dev_cost 측 3 (med) + test_burden 측 5 (high). 다만 minimum-viable subset (e.g., add / update / list / show / verify 5 subcmd) 측 1 cycle 측 land 가능 — 11 subcmd full 측 즉시 land 측 강제 X.
- **A clone 측 single strength**: dev_cost 측 가장 낮음 (clone + path 재설정만). 다만 version_coupling 측 cycle-by-cycle 측 누적 burden 측 dev_cost 측 절약 측 상쇄.
- **C symlink 측 single strength**: drift_risk = 1 (자동 sync). 다만 standalone_clone_safety + plugin_distribution_safety 측 high 측 disqualifying.

---

## §4 권고 — Option B (self-impl), confidence MEDIUM-HIGH

### §4.1 권고 측 근거

1. **own1 정합 우선 (Wine 0 / 외부 runtime 0 정신 측 외부 repo 의존 0 측 확장)** — B 측 external_dependency_count 1 (low) + plugin_distribution_safety 1 (low) + standalone_clone_safety 1 (low) 측 본 repo 측 plugin distribution 모델 측 정합.
2. **own2 정합 (honest emit, repo-local SSOT)** — B 측 SSOT_purity 1 (best) + own2_alignment 1 (best). hexa-native 측 본 repo lib/loader + lib/perf 측 dialect 측 sync 측 control 가능.
3. **future-proof schema** — 본 repo first-mover schema field (`status_enum_adopted` / `retired_intentional_posture` / `adoption_evidence` / `adopted_iso` / `blocker_class`) 측 validator 측 본 repo 측 자유롭게 추가 가능 — anima 측 round-trip 의존 0.
4. **매트릭스 합계 19 (best)** — 11 criteria 측 honest scoring 측 명료한 winner.

### §4.2 confidence level — MEDIUM-HIGH

- **MEDIUM-HIGH (not HIGH)** 이유:
  - dev_cost 측 3 (med) — 사용자 측 implementation cycle budget 측 prohibitive 시 A clone 측 fallback 가능.
  - test_burden 측 5 (high) — 본 repo selftest 측 작성 측 별도 cycle 측 burden 측 사용자 lock-in 측 explicit acknowledgement 필요.
  - 11 subcmd full impl 측 ≥ 2 cycle 추정 — minimum-viable subset 측 부분 land 시 1 cycle 가능.

### §4.3 fallback 시나리오

- **B 측 dev cost prohibitive 시** → **A anima clone** 측 fallback. anima 측 file 측 본 repo 측 1-time copy 후 본 repo 측 SSOT 측 freeze. 단 anima 측 어느 commit 측 frozen 측 marker 측 explicit pin 필요.
- **A 측 anima dialect compatibility 측 fail 시** → **B 측 minimum-viable subset (5 subcmd)** 측 land 후 cycle-by-cycle 측 11 subcmd 측 확장.
- **C symlink 측 권고 X** — standalone_clone_safety / plugin_distribution_safety 측 high risk 측 disqualifying. 단, 사용자 측 명시적 lock-in 시에만 land.

### §4.4 권고 안 측 단정 측 회피 (own2 정합)

- 본 § 측 권고 측 사용자 측 final lock-in 측 대체 X — 매트릭스 + 권고 + confidence level + fallback 측 제시 측 사용자 측 informed decision 측 enable 만.
- 본 cycle 측 **구현 X** (decision-only). 구현 cycle 측 사용자 lock-in 후 별도 cycle 측 진행.

---

## §5 사용자 lock-in 체크리스트 (decision pending)

> **상태**: ⏳ awaiting user lock-in

다음 항목 측 사용자 측 명시적 lock-in 필요 — 본 cycle 후속 implementation cycle 측 시작 전:

- [ ] **option choice**: A (anima clone) / B (self-impl) / C (symlink) 측 1 개 선택
- [ ] **(if A)**: anima 측 어느 version / branch / commit-sha 측 frozen reference (e.g., `anima@2026-05-02 commit abc123` 측 file frozen-copy 측 본 repo 측 freeze)
- [ ] **(if A)**: anima 측 license 측 본 repo plugin 측 호환 여부 audit 결과
- [ ] **(if B)**: implementation cycle 추정 — minimum-viable (5 subcmd: add/update/list/show/verify) 측 1 cycle / 11 subcmd full 측 ≥ 2 cycle, 사용자 측 어느 scope 측 land 시작
- [ ] **(if B)**: schema field 측 validator 추가 측 우선순위 (e.g., `status_enum_adopted` enum validator first)
- [ ] **(if C)**: symlink target 측 절대경로 / 상대경로 정책 (e.g., `../anima/tool/roadmap_op.hexa` 상대경로 측 cross-OS portability vs `/Users/<user>/core/anima/tool/roadmap_op.hexa` 측 raw 15 위반)
- [ ] **(if C)**: standalone clone safety 측 위반 측 사용자 측 명시적 acknowledge — plugin distribution 측 anima 측 부재 시 tool dead 측 인지
- [ ] **selftest scope**: S1..S11 (11 subcmd 정합) 측 본 repo 측 land 시점
- [ ] **schema migrate 의무 X 측 명시 acknowledge**: 본 repo policy `migration: forbidden` 측 정합 — tool 측 in-place modify 측 0 vs additive only

---

## §6 cross-link to predecessor markers (roadmap_op 누적 caveat trail)

본 issue 측 누적 caveat 가 다음 markers 측 명시되어 있다 (timeline 순):

- **`state/markers/airgenome_gamebox_self_mk2_tuning_landed.marker`** (2026-05-02)
  - `mk2_roadmap_op_tool_present: false`
  - `anima_sister: false`
  - C8: "anima tool/roadmap_op.hexa (573 LOC, 11 subcmd) is absent in gamebox — user lock-in needed: (a) clone from anima, (b) build gamebox-local impl, (c) symlink"
  - F2 follow-up: "mk2 roadmap_op tool decision (clone/build/symlink) per C8"
- **`state/markers/airgenome_gamebox_rank_a_3_domain_landed.marker`** (2026-05-03)
  - next_cycle_actions: "tool/roadmap_op.hexa adoption decision (anima clone / self-impl / symlink)"
- **`state/markers/airgenome_gamebox_per_game_roadmaps_landed.marker`** (2026-05-03)
  - next_cycle_actions: "tool/roadmap_op.hexa adoption decision (anima clone / self-impl / symlink — accumulating caveat)"
- **`state/markers/airgenome_gamebox_purple_lineage_offline_shim_landed.marker`** (2026-05-03)
  - "mk2 roadmap_op tool 도입 결정 (anima clone / 자체 구현 / symlink — 누적 caveat C8 of self_mk2 predecessor)"
- **`state/markers/airgenome_gamebox_gameguard_honest_freeze_landed.marker`** (2026-05-03)
  - "mk2 roadmap_op tool 도입 결정 (anima clone / 자체 구현 / symlink — 누적 caveat C8 of self_mk2 predecessor)"
- **`state/markers/airgenome_gamebox_4game_retired_intentional_adoption_landed.marker`** (2026-05-03)
  - "`tool/roadmap_op.hexa` 측 enum validator 추가 결정"

→ 본 doc 측 land 측 위 6 markers 측 누적 caveat 측 **decision matrix 측 explicit 정리 단계 도달** (구현 X, 사용자 lock-in 대기). 후속 implementation cycle 측 별도 marker 측 land 후 위 6 markers 측 caveat trail 측 closed.

---

## §7 caveats (≥ 6 honest items)

- **C1 (matrix scoring 주관성)**: § 3 measurement criteria 측 11 항목 측 score 측 own1 / own2 정합 측 lens 측 주관 측 반영 — 사용자 측 다른 weight 측 적용 시 winner 측 다를 수 있음. 매트릭스 측 honest 한 starting point 측 제시이지 절대 측정 X.
- **C2 (anima version 측 frozen pin 측 부재)**: 본 cycle 측 `../anima/tool/roadmap_op.hexa` 측 sibling presence 측 확인 (809 LOC, 31859 bytes, 2026-05-02 mtime) — 단 본 doc 측 anima 측 commit-sha 측 pin 0. A option 측 land 시 사용자 측 commit-sha 측 명시 측 marker 측 frozen pin 필요.
- **C3 (B self-impl 측 dev cost 측 추정 측 underdetermined)**: § 4.2 측 11 subcmd full 측 ≥ 2 cycle 추정 측 본 repo 측 hexa stdlib 측 actual coverage 측 측정 X 측 추정. minimum-viable 5 subcmd 측 1 cycle 추정 측 동일 — 사용자 측 본 추정 측 conservative / aggressive 측 판단 lock-in 필요.
- **C4 (symlink portability 측 macOS 가정)**: § 2.3 측 symlink 측 macOS 측 정상 동작 가정 — Linux 측 동작 가정 — Windows 측 git symlink 측 OS-별 차이 측 raw 15 (env_lazy) 측 portability 위반 가능. plugin distribution 측 cross-OS scope 측 본 repo policy 측 사용자 측 lock-in 필요.
- **C5 (own1 측 admin tool 정의 측 본 repo 측 first-class 명시 X)**: tool 자체 측 게임-실행 X 측 own1 (Wine 0) 측 직접 위반 X — 단 own1 의 "외부 repo / runtime 의존 0" 정신 측 B self-impl 측 정합. 본 doc 측 own1 의미 측 admin tool 측 확장 측 사용자 측 lock-in 시 명시 ack 필요.
- **C6 (mk2 status enum spec freeze 측 의존)**: tool 측 enum validator 측 add 측 요구 (per `4game_retired_intentional_adoption_landed.marker` next_cycle_actions) — 단 enum spec freeze 측 본 repo 측 별도 cycle 측 land 미완. tool implementation cycle 측 enum spec freeze 측 동시 / 선행 land 측 사용자 측 sequencing 결정 필요.
- **C7 (raw 15 env_lazy 위반 측 anima default)**: anima 측 `REPO_ROOT_DEFAULT = "/Users/<user>/core/anima"` 측 hard-coded — A clone 측 land 시 본 repo 측 path 측 patch 필요 (raw 15 측 정합 회복). C symlink 측 land 시 patch 측 control X 측 raw 15 측 영구 위반 risk.
- **C8 (test 측 본 cycle 측 emit X)**: 본 cycle 측 decision doc only — tool selftest / round-trip test / schema validator test 측 0건. 본 cycle 측 raw 9 (audit-only no impl emit) + raw 168 (minimum-viable exempt) 측 정합 — 단 implementation cycle 측 selftest 측 필수 land.
- **C9 (rate-limit recovery 우선순위 측 본 doc 측 disk write 측 priority)**: 본 cycle 측 BG subagent 측 rate-limit recovery 시점 측 disk write 측 priority — report 측 deferred 가능. 본 doc + 후속 handoff doc + marker 측 disk land 측 cycle 측 success criteria.

---

(decision doc end. § 1-7, 11 criteria scoring, recommendation B confidence MEDIUM-HIGH, lock-in 측 사용자 대기.)
