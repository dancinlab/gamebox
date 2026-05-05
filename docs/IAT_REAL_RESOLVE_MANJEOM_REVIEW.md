# IAT 실 resolve 만점 검토 (R3)

> **status**: 작성 **2026-05-01** / **만점 가능성: 부분가능 — direct name-import resolve 만점 도달 가능, forwarder chain + ordinal-only + delay-load + API-set + bind-imports 5종 blocker 합산 시 단독 launch 불가**
> 작성: 2026-05-01
> SSOT: `docs/REMAINING_ROADMAP.md` (R3 영역, 5축×5 채점), `docs/PRIORITY_DIRECTIVE.md` (작동 최우선)
> 결론 한줄: **현 `pe_iat_resolve_stage2.hexa` skeleton 은 dependency 매칭 placeholder (4개 0 반환). 실 resolve 는 (1) Import Directory 의 OriginalFirstThunk(INT) walk → name/ordinal 추출, (2) own2 export table (`pe_export_table.hexa`) lookup → stub_id 매핑, (3) `pe_stub_address_table.hexa` 의 (low, high) 64bit address 결합, (4) FirstThunk(IAT) slot 에 patch — 4단계가 순서 의존. forwarder/ordinal/delay-load/API-set/bind 5 blocker 가 R6 (DLL 실 구현) prereq 와 강결합 → R3 단독 만점은 direct-only path 한정.**

---

## 0. 영역 정의

### 0-1. R3 = IAT (Import Address Table) 실 resolve 의 의미

**Windows PE loader 의 IAT 처리 5 step** (Microsoft PE/COFF spec 기준):

| step | 동작 | 현 구현 | 실 구현 필요 |
|---|---|---|---|
| 1. parse Import Directory | DataDirectory[1] = `IMAGE_DIRECTORY_ENTRY_IMPORT` 의 `IMAGE_IMPORT_DESCRIPTOR[]` walk | `pe_imports.list_dependencies` (DLL 이름만) | per-DLL 의 INT/IAT/Forwarder Chain/Name/TimeDateStamp 5 field 모두 |
| 2. INT (OriginalFirstThunk) walk | Hint/Name 또는 ordinal flag 추출 | ❌ 없음 | name string / ordinal int 분리 추출 |
| 3. function lookup | LoadLibrary + GetProcAddress 등가 | ❌ 없음 | own2 자체 stub address 반환 (DLL 별 함수→주소 lookup) |
| 4. forwarder chain 해석 | export 가 "DLL.Symbol" 문자열인 경우 재귀 lookup | ❌ 없음 | export RVA 가 export section 범위 내 → 문자열 parse + 재귀 |
| 5. IAT slot patch | FirstThunk[i] 에 실 64bit pointer 기록 | placeholder `[0,0,0,0]` | mmap 영역 writable 시 patch, 아니면 mprotect RW → patch → mprotect RX |

→ **현 skeleton 은 step 1 의 dependency 매칭만 (step 2-5 모두 placeholder)**.

### 0-2. 본 검토 범위

본 만점 검토는 **R3 의 step 2-5 실 구현 가능성** 을 5축×5 = /25 만점 채점.
R6 (DLL 함수 실 구현) 과는 강결합 — IAT slot 에 박힐 주소가 own2 자체 함수의 주소이므로 R6 실 구현 없이는 IAT patch 가 의미 없음 (slot 만 채워지고 함수 호출 시 미구현 stub trap).

### 0-3. own1 / own2 정합

- **own1**: Wine `dlls/ntdll/loader.c` 의 `import_dll` / `find_named_export` 로직 0줄. 참조 only — Microsoft PE/COFF spec (`learn.microsoft.com/.../pe-format#import-table`).
- **own2**: 자체 stub address table (`pe_stub_address_table.hexa`) 와 직접 통합. 외부 C runtime/CRT loader 사용 X.

---

## 1. 현 상태 (skeleton 분석)

### 1-1. `pe_iat_resolve_stage2.hexa` (cycle 미상 — R3 placeholder)

| 항목 | 값 |
|---|---|
| 동작 | dependency 매칭 시 `[0,0,0,0]`, 미발견 시 `[-1,-1,-1,-1]` 반환 |
| 검증 | 합성 record_resolution × 3 (Battle.net pe=73001, KERNEL32=224 / USER32=50 / ADVAPI32=53 = 327 fn 합) |
| schema | `pir_log.tsv` 4-key (pe_id, dll_id, fn_count, resolved) |
| 누락 | **INT walk 부재 / lookup 부재 / patch 부재 / forwarder/ordinal 분기 부재** |

### 1-2. 인접 skeleton (cycle 61-62)

| 모듈 | 역할 | 실 resolve 와 상호작용 |
|---|---|---|
| `pe_export_table.hexa` (cycle 62) | DLL export 의 fn_count / name_count / forwarded_count 집계 | **lookup source** — IAT lookup 시 export 의 RVA → own2 stub address 매핑 base |
| `pe_delay_load_imports.hexa` (cycle 62) | Delay-Load Directory[13] — lazy DLL load 추적 | **별 path** — 일반 IAT 와 다른 thunk (`__delayLoadHelper2`). first-call 시 LoadLibrary trigger |
| `pe_stub_address_table.hexa` (cycle 61) | stub_id 별 (address_low, address_high) 64bit split | **patch destination** — IAT slot 에 박힐 실 주소 |
| `pe_imports.hexa` (이전 cycle) | Import Directory dependency 이름 추출 | **R3 step 1 만 완료** |

### 1-3. DLL 분량 합산 (Battle.net 기준 — 사용자 컨텍스트 발췌)

| DLL | fn count | name/ordinal | 비고 |
|---|---|---|---|
| KERNEL32 | 224 | name | core API, forwarder 다수 (→ ntdll) |
| USER32 | 50 | name | message pump |
| ADVAPI32 | 53 | name | registry/security, forwarder (→ kernelbase, sechost) |
| GDI32 | 18 | name | 2D — D2R 외 사용도 작음 |
| WS2_32 | 31 | **대부분 ordinal** | Winsock — ordinal-only blocker |
| CRYPT32 | 18 | name | 인증서 |
| WININET | 11 | name | HTTP |
| WINHTTP | 4 | name | HTTPS |
| SHELL32 | 8 | name | shell helper |
| ole32 | 2 | name | COM init |
| OLEAUT32 | 4 | **ordinal** | VARIANT — ordinal-only |
| WINTRUST | 1 | name | signature |
| VERSION | 3 | name | file version |
| MSIMG32 | 1 | name | AlphaBlend |
| RPCRT4 | 3 | name | RPC |
| UIAutomationCore | 5 | name | accessibility |
| **총** | **436** | 16 DLL | ordinal-only = WS2_32 + OLEAUT32 ~35 fn (8% blocker share) |

→ **direct name resolve = 92% / ordinal resolve = 8%** — direct path 우선 진행 시 92% 도달 가능.

---

## 2. 단계 도달성 (만점 시 효과)

### 2-1. R3 만점 = IAT 실 patch 완료 시점

| step | 만점 효과 | 별도 prereq |
|---|---|---|
| 2 (INT walk) | name/ordinal 분리 추출 | 없음 — skeleton 만으로 가능 |
| 3 (lookup) | export table 연결 + stub_id 산출 | **R6 (own2 자체 함수 구현) 부분 동시 진행 필요** |
| 4 (forwarder chain) | KERNEL32→NTDLL 등 redirection 해석 | NTDLL own2 구현 (R6 의 부분집합) |
| 5 (IAT patch) | FirstThunk 슬롯에 64bit ptr 기록 | **R2 (PE 실 mmap RW) prereq** — patch 가능 영역 |

### 2-2. 단독 launch 가능성

R3 만점만으로 **단독 launch 불가**. 이유:
1. R2 (PE mmap) 미완료 시 IAT slot 이 file mapping 영역 → COW 없이 patch 불가
2. R6 (DLL 실 구현) 미완료 시 IAT 가 가리키는 stub 주소가 trap 함수
3. R1 (native bridge) 의 mprotect/dlopen 미완료 시 RW→RX 전환 불가

→ R3 의 의의는 **R1+R2 완료 후 R6 와 동시 진입 시 launch step 의 50% gate 통과** (PE 적재 → 함수 주소 박힘 → entry call 직전).

### 2-3. PRIORITY_DIRECTIVE 정합

- "**작동 우선**" → R3 는 작동 prereq 의 핵심 chain (R1→R2→R3→R6) 중 3번째. ✅ 정합.
- "성능/자원/속도" → IAT resolve 자체는 cold-launch 1 회성. 437 fn × 16 DLL = O(n) walk → 무시 가능. ✅ 정합.
- "편의성 보류" → R3 는 launch prereq, 편의성 영역 아님. ✅ 정합.

---

## 3. 5축 채점 /25 (REMAINING_ROADMAP.md 기준)

| 축 | 정의 | 점수 | 근거 |
|---|---|---|---|
| **A. own1 정합** | Wine 0줄 유지 가능? | **5** | Microsoft PE/COFF spec 만으로 INT/IAT/Forwarder field 모두 명세. Wine `loader.c` import_dll 참조 불필요 — IAT format 은 spec 그대로 |
| **B. 실 동작 도달** | 만점 시 launch 가능? | **2** | R3 단독으로 launch 불가 (R2 mmap + R6 stub 실 구현 prereq). direct name resolve 92% 도달은 가능하나 ordinal-only 8% + forwarder chain blocker 가 entry call 도달 차단 가능 |
| **C. 자원 영향** | perf/메모리 cost | **5** | 1회성 cold-launch 비용. 436 fn × O(log n) name lookup ≈ ms 단위. IAT slot patch 도 4KB page 단위 mprotect → 무시 가능 |
| **D. 구현 난이도 (역)** | 낮을수록 ↑ | **3** | direct name resolve = 1-2 cycle. forwarder chain = +1 cycle. ordinal lookup = +0.5 cycle. delay-load thunk = +1 cycle. API-set redirection = +1 cycle. bind-imports fallback = +0.5 cycle. **합계 ~5 cycle** (REMAINING_ROADMAP 의 R3 estimate "2 cycle" 보다 큼 — blocker 5종 미반영) |
| **E. 의존성 (역)** | 적을수록 ↑ | **2** | R1 (mprotect/mmap) + R2 (PE 적재 영역) + R6 (own2 stub 구현) **3개 prereq**. R3 는 의존성 측면에서 허브에 가까움 |

**합계: 5+2+5+3+2 = 17 / 25**

→ **15 이상 → sprint 후보 (REMAINING_ROADMAP 기준)**.
→ 단 B (실 동작) + E (의존성) 가 낮음 → R1+R2+R6 와 동기화된 batch 진입 권장.

---

## 4. blocker (실 구현 시 등장하는 5종)

### 4-1. Forwarder export chain

**현상**: `KERNEL32!HeapAlloc` 의 export RVA 가 `IMAGE_EXPORT_DIRECTORY` 의 export section 범위 내를 가리키고, 그 위치에 ASCII 문자열 `"NTDLL.RtlAllocateHeap"` 가 있는 경우.

**해결**:
1. export RVA 가 export section 범위 내인지 검사 (`is_forwarder_rva()`)
2. 문자열 parse → DLL.Symbol 분리
3. NTDLL own2 export table 재귀 lookup
4. 무한 재귀 방지 (depth ≤ 4)

**예상 분량**: KERNEL32 224 fn 중 ~30 fn 이 forwarder (NTDLL/KERNELBASE/API-MS-WIN-CORE redirect). own2 NTDLL stub 도 R6 영역과 strong coupling.

### 4-2. Ordinal-only export (WS2_32, OLEAUT32)

**현상**: WS2_32 의 `WSAStartup`, `socket`, `connect` 등은 **ordinal 만** import (Hint/Name 부재). PE 의 INT entry 가 high bit set (`IMAGE_ORDINAL_FLAG`) 으로 ordinal 표현.

**해결**:
1. INT entry 의 high bit 검사 → ordinal 분기
2. own2 export table 의 `AddressOfFunctions[ordinal - Base]` lookup
3. `pe_export_table.hexa` 의 `total_names == 0` 인 DLL (skeleton 의 dll_id=88004 = "Ordinal-only" 480 fn) 패턴이 정확히 이 case

**예상 분량**: WS2_32 31 + OLEAUT32 4 = **35 fn** (전체의 8%).

### 4-3. Delay-load IAT (별도 thunk)

**현상**: PE Optional Header DataDirectory[13] = `IMAGE_DELAYLOAD_DESCRIPTOR`. 일반 IAT 와 다른 path — first-call 시 `__delayLoadHelper2` thunk 가 LoadLibrary + GetProcAddress 발동.

**해결**:
1. DataDirectory[13] walk (already skeleton: `pe_delay_load_imports.hexa`)
2. `__delayLoadHelper2` 자체를 own2 함수로 구현 — IAT 의 placeholder 가 이 thunk 를 가리키게 patch
3. first-call 시점에 R3 의 lookup path 재사용

**예상 분량**: Battle.net delay-load 4 DLL × ~5 fn = ~20 fn (skeleton self-test 의 40 fn vs 실 분량은 PE 별).

### 4-4. API-set redirection (api-ms-win-core-*)

**현상**: Windows 7+ 의 API set schema. `api-ms-win-core-processthreads-l1-1-0.dll` 같은 가상 DLL 이름이 import 됨. 실제로는 KERNELBASE 나 NTDLL 로 redirect (런타임 schema lookup).

**해결**:
1. Win10+ `apisetschema.dll` 의 schema 를 own2 자체로 hardcode (사실상 정적 mapping)
2. `api-ms-win-core-*` import 발견 시 schema lookup → 실 DLL 이름 치환
3. 그 후 일반 INT walk path 재사용

**예상 분량**: Battle.net (Windows 10+ 빌드) 16 DLL 중 직접 등장은 적음 (대부분 `KERNEL32` 으로 forwarding 됨), 그러나 `.NET CLR` PE 등 modern 빌드에서는 50+ api-set entry 가능.

**리스크**: schema 가 Windows 버전마다 다름 (Win10 22H2 vs Win11 23H2 vs Server 2025). own2 hardcode 시 Windows 버전 fingerprint 누락 가능.

### 4-5. Bind imports (timestamp / 충돌 시 fallback)

**현상**: `BIND_IMAGE` (editbin /bind) 으로 미리 IAT 에 절대 주소 박혀 있음. PE Optional Header `BoundImport.TimeDateStamp` 가 DLL 의 실 timestamp 와 일치하면 IAT 그대로 사용, 다르면 정상 resolve fallback.

**해결**:
1. `IMAGE_BOUND_IMPORT_DESCRIPTOR` walk → TimeDateStamp 비교
2. own2 환경에서는 own2 DLL stub 의 timestamp 가 원본과 절대 일치 X → **항상 fallback path** 사용
3. bound IAT 무시 + 일반 IAT resolve 적용

**예상 분량**: 코드 1 cycle. 단순 timestamp 검사 + skip.

### 4-6. blocker 종합 — 만점 도달 시간 분석

| blocker | direct-only path | full path (5종 모두) |
|---|---|---|
| INT walk (name) | 1 cycle | 1 cycle |
| stub address lookup | 1 cycle | 1 cycle |
| IAT slot patch | 0.5 cycle | 0.5 cycle |
| forwarder chain | — | 1 cycle |
| ordinal-only | — | 0.5 cycle |
| delay-load thunk | — | 1 cycle |
| API-set schema | — | 1 cycle |
| bind-imports fallback | — | 0.5 cycle |
| **합계** | **2.5 cycle** | **6.5 cycle** |

→ REMAINING_ROADMAP 의 "R3 = 2 cycle" 은 **direct-only 일 때만** 정확. blocker 5종 포함 시 6-7 cycle.

---

## 5. 의존성

```
R1 (native bridge)
   ├── mprotect / mmap RW→RX 전환 → R3 의 IAT slot patch prereq
   └── dlopen 시 own2 자체 image base — R3 lookup 불필요 (own2 직접)

R2 (PE 실 mmap)
   └── .idata section 적재 영역 → R3 step 1 (Import Directory walk) prereq
   └── .text section RVA → R3 step 5 (forwarder RVA 검증) prereq

R3 (본 영역)
   ├── 산출: IAT slot patched → R6 의 stub 함수 실 호출 가능
   └── 산출: forwarder chain 검증 → NTDLL/KERNELBASE redirect 동작

R6 (DLL 실 구현)
   ├── 437 fn 의 own2 native ARM64 코드 — R3 의 lookup target
   └── R3 와 strong coupling — R6 진척도 = R3 의 useful resolve 비율
```

→ **R3 단독 진입 = 의미 작음**. R1+R2 완료 + R6 의 KERNEL32/USER32 ~30 fn 동시 진척 시 R3 batch 진입 권장.

---

## 6. ETA + bg 분해

### 6-1. 직렬 ETA: 6.5 cycle (full path), 2.5 cycle (direct-only)

### 6-2. bg 병렬 분해 (REMAINING_ROADMAP "2 cycle" 정합용)

| bg slot | 작업 | 의존 | cycle |
|---|---|---|---|
| **bg-A** | INT walk (name) + IAT slot patch direct path | R2 | 1.5 |
| **bg-B** | Ordinal-only resolve (WS2_32/OLEAUT32) — own2 ordinal table | A 부분 | 0.5 |
| **bg-C** | Forwarder chain — export RVA in-range 검증 + 재귀 lookup | A + own2 NTDLL skeleton | 1.0 |
| **bg-D** | Delay-load thunk (`__delayLoadHelper2` own2 구현) | A | 1.0 |
| **bg-E** | API-set schema hardcode (Win11 23H2 baseline) | A | 1.0 |
| **bg-F** | Bind-imports fallback (timestamp 비교 + skip) | A | 0.5 |

→ 병렬 가정 시: **max(1.5, 1.0+0.5, 1.0, 1.0, 1.0, 0.5) = 1.5 cycle (longest path = A)**.
→ REMAINING_ROADMAP 의 "2 cycle" 추정이 **bg 병렬 + direct-only path 가정 시 정합**.

### 6-3. cycle 진입 조건

- **선결**: R1 의 mprotect 실 호출 + R2 의 .idata mmap 완료
- **동시 진입 권장**: R6 의 KERNEL32 30 fn (forwarder chain target = NTDLL 의 30 fn 도 동시) — direct-only 보다 forwarder full path 의 가치 큼

---

## 7. 결론

### 7-1. 채점 종합

| 항목 | 값 |
|---|---|
| **/25 점수** | **17 / 25** (A=5, B=2, C=5, D=3, E=2) |
| **만점 가능성 (direct-only)** | **부분가능** — 92% fn (name-import) 도달 가능 |
| **만점 가능성 (full path 5종)** | **가능 (조건부)** — R1+R2+R6 batch 동기 진입 시 |
| **단독 launch 도달** | **불가** — R6 진척 동시 필요 |
| **own1 정합** | ✅ 100% (Microsoft PE spec 만 참조) |
| **PRIORITY_DIRECTIVE 정합** | ✅ 작동 prereq chain 의 3번째, 편의성 무관 |

### 7-2. 권장 path

1. **direct-only path 우선**: bg-A 1.5 cycle 로 name-import 92% 도달 → KERNEL32/USER32/ADVAPI32 (327 fn = 75% 점유) 즉시 활용 가능
2. **bg-C (forwarder) + bg-B (ordinal) 병렬** — KERNEL32→NTDLL redirect 와 WS2_32 ordinal 은 Battle.net 핵심 path
3. **bg-D (delay-load) 후순위** — Battle.net 의 delay-load 분량 작음 (~20 fn). 첫 launch 시 trigger 안되는 path 다수
4. **bg-E (API-set) + bg-F (bind) 마지막** — API-set 은 .NET PE 등에서만 critical, bind 는 1줄 fallback

### 7-3. R3 의 본질

R3 는 **단독 sprint 가 아니라 R1+R2+R6 의 cross-cutting** — REMAINING_ROADMAP 의 의존성 그래프

```
R1 → R2 → R3 → R6
```

가 정확하나, **R6 가 R3 와 사실상 동시 진행** (R3 의 lookup target 이 R6 의 함수). 따라서:

- **단독 진입 시**: 17/25 — direct-only path 만 도달, blocker 5종 미해결
- **R6 동기 진입 시**: 22/25 (B=4, E=4 로 상승) — 실 launch chain 완성

→ **사용자 directive ("실제 작동실험 외 나머지 모두 구현") 정합 시 R3 + R6 batch 가 가장 가치 있음**.

### 7-4. 즉시 권장 action

1. bg-A (direct-only INT walk + patch) skeleton 작성 → R3 의 17/25 → 19/25 상승 (D=4 로 — 실 구현 부분 진척)
2. R6 의 KERNEL32 30 fn (`HeapAlloc`, `GetProcessHeap`, `LoadLibraryA`, `GetProcAddress`, `VirtualAlloc`, `VirtualProtect` 등 launch 핵심) 우선 own2 구현 — R3 의 forwarder target 기반 마련
3. bg-B (ordinal) 와 bg-C (forwarder) 를 cycle 80+ batch 후보로 등록
4. 본 문서를 `docs/REMAINING_ROADMAP.md` R3 row 의 "만점 검토" link target 으로 cross-ref

---

*written 2026-05-01. R3 = IAT 실 resolve 만점 검토. 17/25 — direct-only path 로 92% fn 도달 가능, blocker 5종 (forwarder/ordinal/delay-load/API-set/bind) 은 R6 와 strong coupling. R1+R2+R6 batch 진입 시 22/25 도달 가능. own1 정합 100%, Microsoft PE spec 만 참조. PRIORITY_DIRECTIVE 정합 (작동 prereq chain 3번째).*
