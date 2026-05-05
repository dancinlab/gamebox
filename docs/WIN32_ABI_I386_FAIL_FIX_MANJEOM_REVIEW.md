# win32_abi_i386 pre-existing FAIL 수정 — 만점 검토

> **status**: 작성 2026-05-01 / 만점: **23/25** / 결론 한줄: **1 line typo (`s.length()` → `len(s)`) 수정으로 loader self-test set 100% PASS 회복 가능 — 1 cycle / 단일 BG agent / 독립**.

---

## 0. 영역 정의 (FAIL 모듈, 영향 범위)

- **모듈 path**: `/Users/ghost/core/airgenome-gamebox/lib/loader/win32_abi_i386.hexa`
- **역할**: Win32 i386 (32-bit) calling convention shim — cdecl/stdcall/fastcall/thiscall arg passing + name demangling + arch detect (PE Machine 0x14C).
- **REMAINING_ROADMAP 위치**: M1 (정비 항목, R1-R6 prereq 와 독립).
- **영향 범위 (만점 회복 시 unblock 되는 PE binary)**:
  - **Battle.net-Setup.exe** — i386 PE32 (모듈 헤더 주석에 명시).
  - 기타 32-bit Windows 설치 binary (legacy installer 다수).
  - Lineage 2 (chronicle 별 32bit l2.exe — `PURPLE_LINEAGE_MANJEOM_REVIEW.md` §3 cross-link).
  - D2R i386 인프라 (이미 운용 중, 본 모듈은 ABI 보강).
- **FAIL 식별 시점**: cycle 64 commit 시 loader self-test 전체 set 실행 — pre-existing (cycle 64 도입 모듈과 무관, 이전 cycle 잔여물).

---

## 1. 현 상태

### 1-1. self-test 실 출력 (실측)

명령:
```sh
docker exec hexa-exec /bin/sh -lc "cd /Users/ghost/core/airgenome-gamebox && hexa run lib/loader/win32_abi_i386.hexa self-test 2>&1 | tail -10"
```

실 출력:
```
Runtime error: unknown method .length() on string
panic: win32_abi_i386 FAIL [demangle cdecl] got=
error: uncaught throw
```

→ **Hexa runtime 에는 `str.length()` 메서드가 존재하지 않음**. Hexa 의 string size 는 free function `len(s)` 만 지원.
→ runtime panic 이후 demangle() 가 빈 문자열 반환 → assertion `if d_cdecl != "GetLastError"` 트리거 → `[demangle cdecl] got=` (got 뒤가 빈 문자열).

### 1-2. 직접 code inspect 결과 (의심 line + 함수)

**의심 location**: `lib/loader/win32_abi_i386.hexa:76` 의 `demangle()` 함수.

```hexa
fn demangle(name: str) -> str {
    if name == "" { return "" }
    let s = name
    // strip leading _ or @
    let first = s.substr(0, 1)
    if first == "_" || first == "@" {
        s = s.substr(1, s.length() - 1)   // ← line 76 — .length() 미존재
    }
    ...
}
```

**근거 — 같은 codebase 의 sister module 비교**:

| 모듈 | string size 호출 방식 | 동작 |
|---|---|---|
| `lib/loader/win32_abi.hexa` (x86_64 sister) | `len(ms_saved)`, `len(sv_saved)` (free function) | PASS |
| `lib/loader/win32_abi_i386.hexa:76` | `s.length()` (method call) | **FAIL** |
| `lib/loader/win32_rpcrt4.hexa:69` | `u.length()` (method call) | **별도 FAIL — 본 작업 범위 외** |

→ **single-line typo**: `s.substr(1, s.length() - 1)` → `s.substr(1, len(s) - 1)`.

**부수 검증**: 동일 함수 다른 줄 (line 74 `s.substr(0, 1)`, line 91 `name.substr(0, 1)`) 은 정상 — `.substr()` 는 Hexa 에서 method 로 지원됨. 깨진 부분은 `.length()` 만.

**ABI shim 본체 (cdecl/stdcall/fastcall arg passing) 자체는 정상** — convention table / lookup / infer / arch detect / stack_arg_size 모두 logic 이 올바르고 hexa method 사용도 정확. **단 1줄 string method 오기**가 demangle 첫 step 에서 panic 을 내고 그 뒤 모든 assertion 도달 불가하게 만든 것.

---

## 2. 만점 도달 효과

- **loader self-test set 100% PASS 회복** — cycle 64 commit gate 의 lingering FAIL 1건 제거. CI/commit gate 에서 "pre-existing FAIL 1, 본 cycle 도입 모듈 0" 이라는 noise 제거.
- **i386 PE binary 진척 unblock** — Battle.net-Setup.exe i386 install path / Lineage 2 chronicle (32bit l2.exe) / 기타 legacy installer ABI shim 신뢰 상태 회복. 본 모듈이 FAIL 인 동안은 i386 closure path B 의 ABI 단계가 untrusted 상태.
- **R3 (IAT 실 resolve) prereq 강화** — IAT 가 i386 PE 의 export name 을 demangle 해 lookup 할 때 본 모듈을 호출. demangle() 가 panic 을 내는 한 i386 IAT resolve 자체가 시작 불가.
- **own 1 정합 유지** — Wine source 인용 0줄 / Microsoft cdecl/stdcall/fastcall MSDN 만 참조 (모듈 헤더 주석 그대로 유지). fix 가 typo 수정이므로 신규 코드 inflow 없음.

---

## 3. 5축 채점 /25

`docs/REMAINING_ROADMAP.md` §"만점 검토 채점 축" 의 5축 / 5점 만점.

| 축 | 정의 | 점수 | 근거 |
|---|---|---|---|
| **A. own1 정합** | Wine 0줄 유지 가능? | **5** | typo 수정만 — 신규 코드 inflow 0. MSDN cdecl/stdcall/fastcall 참조 그대로 유지. |
| **B. 실 동작 도달** | 만점 시 launch 가능? | **3** | 단독 launch 불가 (R1-R6 prereq 미해결). 단 i386 ABI shim 자체는 다시 신뢰 상태 → IAT/loader 가 i386 binary 진입 시 즉시 사용 가능. |
| **C. 자원 영향** | perf/메모리 cost | **5** | `len(s)` 호출은 `.length()` 와 동등 cost. 무시할 수준. |
| **D. 구현 난이도 (역)** | 1 cycle 이내 = 5 | **5** | **1 line edit**. test 1 회 실행으로 검증 종료. 1 cycle 미만 작업. |
| **E. 의존성 (역)** | 적을수록 = 5 | **5** | 완전 독립 — 다른 영역 prereq 0. M1 단독 cycle 가능. |
| **합계** | | **23/25** | |

**감점 사유**: B 축 -2 — 본 fix 는 self-test 회복까지만 도달, 실 launch 는 R1-R6 의 다른 manjeom-review 항목 누적이 필요.

→ **>15 = sprint 후보** 기준 충족, 즉시 진행 권장.

---

## 4. 핵심 blocker

- **없음** (typo 수정).
- 잠재 risk:
  - Hexa runtime 이 향후 `.length()` 를 method 로 추가하더라도 `len(s)` 는 영구 호환 → 회귀 risk 0.
  - `win32_rpcrt4.hexa:69` 에 동일 패턴 잔존 — **본 작업 범위 외 / 별도 cycle 권장** (사용자 제약: "본 path 외 수정 X").

---

## 5. 의존성 (none, 독립)

| 종류 | 항목 | 상태 |
|---|---|---|
| upstream prereq | 없음 | — |
| downstream unblock | i386 IAT resolve / Battle.net-Setup.exe / Lineage 2 32bit chronicle | 본 fix 후 ABI 신뢰 회복 |
| 다른 만점 검토 의존 | 없음 (R1-R6 와 직교) | — |
| Hexa runtime 변경 필요 | 없음 (`len()` 는 이미 codebase 전반에서 사용 중) | — |

→ **REMAINING_ROADMAP §"의존성 그래프"** 의 `M1 (FAIL 수정) — 독립 (소규모)` 와 정합.

---

## 6. ETA / cycle (1 cycle, 단일 BG agent)

| step | 내용 | 시간 |
|---|---|---|
| 1 | `lib/loader/win32_abi_i386.hexa:76` 단일 line edit (`s.length() - 1` → `len(s) - 1`) | <1 분 |
| 2 | `docker exec hexa-exec ... self-test` 재실행 — PASS 확인 | <1 분 |
| 3 | loader self-test 전체 set 회귀 (다른 모듈 영향 0 확인) | <5 분 |
| 4 | commit (own 1 정합 / single-line / unrelated module 미수정) | <1 분 |

→ **총 1 cycle 미만 / 단일 BG agent / 외부 의존 0**.
→ REMAINING_ROADMAP §"ETA / cycle 수 estimate" 의 `M1 = 1 cycle` 와 정합.

---

## 7. 결론

- **FAIL root cause**: `lib/loader/win32_abi_i386.hexa:76` 의 `s.length()` — Hexa runtime 미지원 method. Sister module `win32_abi.hexa` 는 `len()` free function 사용 (정상). single-line typo.
- **fix**: `s.substr(1, s.length() - 1)` → `s.substr(1, len(s) - 1)`. 1 line.
- **만점**: **23/25** — A/C/D/E 모두 5 만점, B 만 -2 (실 launch 까지는 R1-R6 누적 필요).
- **권장**: 다음 cycle 즉시 진행. 단일 BG agent / 1 cycle 이내 / 외부 의존 0 / 회귀 risk 0.
- **본 검토 외 후속 권장 (별도 cycle)**: `win32_rpcrt4.hexa:69` 동일 패턴도 동일 fix 가능 — 본 path 외 제약 준수로 본 cycle 에서는 미수정.

---

*written 2026-05-01. cycle 64 pre-existing self-test FAIL 검토. 본 cycle 은 검토 docs 만 — 실 fix 는 별도 cycle (M1).*
