# 우선순위 directive (2026-05-01)

> 사용자 directive 명시:
> - "hell mode 어쩌구 편의성관련 있는데 현재는 그런거 추구하는 상황은 아님"
> - "작동 원할하고, 성능, 자원, 속도 우선이야"

## 핵심 원칙

**우선**:
1. **작동 (correctness)** — 실 launch 가능성, 실 frame 도달, 실 게임 진행
2. **성능 (perf)** — frametime, throughput, latency
3. **자원 (resource)** — 메모리, CPU, GPU 사용량 효율
4. **속도 (speed)** — cold-launch, frame rate, load time

**후순위 (현재 추구 X)**:
- gameplay 편의성 모듈 (build optimizer, drop calculator, immunity break chain, charm packing, gambling RNG, MF multiplier, set bonus, XP curve forecast 등)
- 사용자 quality-of-life feature
- 정량화-only 모듈 (실 launch 와 무관)

## 영향

### 이전 cycle 회고 (편의성 위주였던 영역)
다음 cycle 들은 **편의성 / quality-of-life** 영역이라 본 directive 와 일부 misaligned:
- cycle 53: D2R class skill synergy, horadric cube recipe, mercenary equip optimize, D4 glyph paragon level, tempering affix reroll, masterworking amplify, world event calendar
- cycle 54: D2R waypoint activation, shrine effect, ancients spawn, D4 occultist enchant roll, living steel economy, infernal horde wave, whisper bounty optimize
- cycle 55: D2R gamble NPC roll, set item complete bonus, experience curve predict, D4 world boss spawn window, dungeon sigil inventory, seasonal mechanic timer
- 일부 PSB-12~16 후보 (gameplay 편의 위주)

이 모듈들은 **historical record 로 docs/ 와 lib/perf/ 에 보존**. 현 directive 는 **신규 작업** 에 적용.

### 이후 cycle 방향 (작동/성능/자원/속도 직접 영향)
- ✅ **CM-21 진입 후 cycle 58~61**: Battle.net loader stub + stage2 PE loader skeleton + Apple-native bridges = **작동** 직접 prereq → 정합
- ✅ **cycle 62~**: PE format completeness, stage4 실 구현, mmap/dyld 실 호출 = **작동/속도** 직접
- ✅ **frame perf 인프라** (cycle 56-57): display mode + macOS menubar + FPS = **성능 측정** 인프라 → 정합
- ⚠️ **신규 D2R/D4 gameplay PSB-17+ 후보** = 미보류 (이 directive 와 어긋남)

### cycle 62 후보 (직접 정합)
| 후보 | 정합도 |
|---|---|
| PE format completeness (export/delay-load/resource/manifest/signature/debug/tls_data/CFG) | ✅ **작동** prereq |
| stage4 실 mmap/dlsym/pthread_create | ✅ **작동** 핵심 |
| 폴더 split refactor | ✅ 유지보수 (간접 영향) |
| 신규 게임 gameplay 모듈 | ❌ 편의성 (보류) |
| memory pool optimization | ✅ **자원** |
| frame compositor optimization | ✅ **속도** |
| GPU memory residency tracking | ✅ **자원** |

## 결정 기준 (going forward)

신규 cycle 진입 시 4 기준 self-check:
1. 이 작업이 **실 launch / 실 frame** 가능성을 직접 향상시키는가?
2. **frametime / throughput / latency** 정량 개선?
3. **메모리 / CPU / GPU** 사용량 줄이기?
4. **cold-launch / load time / 응답** 단축?

→ 4 기준 중 **하나 이상 ○** = 진행
→ **모두 X** = gameplay 편의성 → 보류

---

*written 2026-05-01, 사용자 directive 즉시 반영.*
*기록 위치 = repo docs (사용자 explicit 요청: "메모리말고" — Claude memory 아닌 docs 에 기록).*
