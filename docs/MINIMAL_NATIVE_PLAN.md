# Minimal Native Plan — DIY Wine 대용 (hexa)

> **결정 (2026-04-30)**: gamebox 가 Wine 의존성 0 + D2R/D4 자체 실행 + 만점 충족 + minimal.
> 유일한 path = **hexa-native PE 로더 + Win32 shim** 직접 작성.
> 상위: [.own](.own), [PERF_PLAN.md](PERF_PLAN.md), [HEXA_UPSTREAM.md](HEXA_UPSTREAM.md)

---

## 1. 사면체 분석 — 왜 이 path 인가

| 옵션 | D2R/D4 | Wine 0 | 만점 | minimal | 채택 |
|---|---|---|---|---|---|
| (A) Apple-released GPTK build (apple/apple formula) | ✓ | ✗ Wine source = CrossOver | ✓ | ✓ | ✗ |
| (B) Native Mac 게임 + 만점 redefine | ✗ D2R/D4 X | ✓ | ✓ | ✓ | ✗ (D2R/D4 mandatory) |
| (C) Cloud streaming | ✓ | ✓ | ✗ local 측정 무의미 | ✓ | ✗ |
| (D) Windows VM (Parallels) | ✓ | ✓ Windows 사용 | ✗ VM 위 측정 noise | ✗ heavy | ✗ |
| (E) Observer-only (gamebox launch 미수행) | ✓ (사용자 launch) | ✓ gamebox 안 | ✓ | ✓ | ✗ (자체 실행 mandatory) |
| **(F) DIY hexa PE 로더 + Win32 shim** | **✓** | **✓** | **✓** | **research-scale** | **⭐ 채택** |

→ "minimal" 의미: code volume 이 아니라 *Apple 외 코드 0 줄* 의 minimal. 우리가 작성하는 hexa = Apple syscalls + D3DMetal API 위에서만.

## 2. Phase 카탈로그

| Phase | 무엇 | hexa LoC | ETA (FT) |
|---|---|---|---|
| **0a** | PE/COFF 헤더 parser | ~500 | 1-2 주 |
| **0b** | PE memory loader (mmap + relocations + IAT) | ~1000 | 2-4 주 |
| **1** | x64 ABI shim (Microsoft x64 ↔ System V x64) | ~500 | 2 주 |
| **2** | Win32 API stub table (~2000 함수 declarations) | ~5000 | 1 달 |
| **3** | kernel32 핵심 implementation (file/memory/thread/sync) | ~15000 | 2-3 달 |
| **4** | user32 / gdi32 / advapi32 핵심 | ~10000 | 2 달 |
| **5** | DirectX 11 → D3DMetal binding | ~5000 | 1-2 달 |
| **6** | Winsock 2 → BSD sockets | ~3000 | 1 달 |
| **7** | OpenJK / OpenMW 등 OSS Win 게임 검증 | — | 2 주 |
| **8** | Battle.net 우회 (직접 game.exe 실행) | TBD | 1-2 달 |
| **9** | D2R 첫 frame | — | (Phase 0-7 누적) |
| **10** | D4 (D3D12 + DXR) | ~10000 추가 | (Phase 0-7 + 추가) |

**솔직 ETA**: D2R 첫 frame ≈ 9-12 달, D4 ≈ 12-18 달 (단일 개발자 full-time).

## 3. 만점 기준 재정의 (axis A)

기존 A: "Apple wine64 / D3DMetal / Rosetta 2"
신규 A: **"Apple-only stack — hexa PE loader + macOS syscalls + D3DMetal + Rosetta 2 + 직접 작성한 Win32 shim. 외부 Wine/CrossOver/community 코드 0 줄."**

B (runtime) / C (재해석) 그대로.

→ 만점 컴포넌트 (M2-M14) 는 Phase 9+ (D2R 동작 시점) 부터 측정 가능. 그 전까지는 sit idle (또는 OSS 게임으로 검증).

## 4. minimal first sprint — Phase 0a 즉시

### 0a Scope

**파일**:
- `lib/loader/pe_parser.hexa` — PE 헤더 파싱
- `tests/test_pe_parser.sh` — Battle.net-Setup.exe parse 검증

**API (Phase 0a)**:
```hexa
fn pe_read_dos_header(path: str) -> array       // [signature, e_lfanew]
fn pe_read_nt_headers(path: str, offset: int) -> array  // [signature, num_sections, ...]
fn pe_read_optional_header(path: str, offset: int) -> array  // [magic, entry_point, image_base, ...]
fn pe_read_sections(path: str, offset: int, count: int) -> array  // 각 [name, vaddr, vsize, raw_offset, raw_size]
fn pe_summary(path: str) -> str                  // human-readable 요약
fn self_test() -> void                           // Battle.net-Setup.exe 검증
```

**검증 기준**:
1. DOS header magic = 0x5A4D ("MZ")
2. NT headers signature = 0x00004550 ("PE\0\0")
3. Optional header magic = 0x10B (PE32) 또는 0x20B (PE32+)
4. 섹션 수 = NumberOfSections (FileHeader)
5. 엔트리포인트 + 이미지베이스 추출

### 0b Scope (다음)

PE memory loader. mmap, relocations, IAT 패치.

---

## 5. .own own 1 (4번째 정정)

```
own 1 new "DIY hexa PE 로더 + Win32 shim — Wine/CrossOver/community 코드 0 줄"
  rule
    (a) gamebox 의 모든 foreign-binary 실행 path 는 hexa-native PE 로더 사용
    (b) Win32 API shim 직접 작성 — winehq.org / CrossOver source 단 1줄도 import 금지
    (c) Apple binary (D3DMetal.framework, libd3dshared.dylib) 와 macOS syscalls 만 위에서 작동
    (d) Rosetta 2 (Apple) 사용 가능 — x86_64 game binary 명령어 변환 layer
  scope: lib/loader/ 전체 + 모든 launch path
  enforcement
    (1) lib/loader/*.hexa 의 import 가 시스템 빌트인 + r0_common 만 허용
    (2) wine64 / wineserver / crossover binary 호출 grep 시 self-test FAIL
    (3) /Applications/Whisky.app, gcenx tap, CrossOver.app 부재 검증
  exception 없음 (hard rule)
  why
    Wine = winehq.org community / CodeWeavers commercial. third-party.
    minimal + D2R/D4 + Wine 0 + 만점 = DIY PE 로더 만이 교집합.
    research-scale (months/years) 이지만 *minimal 코드 의존* 의 진정한 의미.
  honest
    이 own = 1년+ 연구 프로젝트 commit. Phase 0a 즉시 시작 가능, Phase 9 (D2R 첫 frame) 까지 9-12 달.
    그 동안 만점 컴포넌트 (M2-M14) 는 idle 또는 OSS 게임으로만 검증.
```

---

## 6. minimum 진행 계획

### 즉시 (오늘)
- [x] docs/MINIMAL_NATIVE_PLAN.md (이 파일)
- [x] .own own 1 update (DIY PE 로더)
- [ ] lib/loader/pe_parser.hexa skeleton + Battle.net-Setup.exe 검증
- [ ] commit + push

### 다음 1-2 주 (Phase 0a)
- PE 헤더 fully parsed (DOS + NT + Optional + Section)
- Import 테이블 / Export 테이블 parsing
- 다양한 PE 검증 (작은 Win32 app + Battle.net-Setup.exe + D4.exe header 만)
- self-test 95%+ accuracy

### 다음 2-4 주 (Phase 0b)
- mmap sections (read-only / read-write / execute permissions)
- Base relocation 적용
- IAT (Import Address Table) 패치
- Entry point 점프 가능 상태 (return 즉시 X 검증)

### 다음 2 달 (Phase 1-2)
- x64 ABI shim
- Win32 API stub 테이블
- "Hello World" Win32 app 실행

---

## 7. 만점 컴포넌트 동안 무엇을 하는가

Phase 0-8 동안 D2R/D4 안 도므로 만점 측정 X. 그 동안:
- M0-M14 컴포넌트 self-test 만 유지 (synthetic 입력)
- OSS Win 게임 (OpenJK 등) 으로 Phase 7 부터 만점 측정 가능
- HEXA upstream (float / bit ops / dict literal) PR 진행 — Phase 0-7 효율 +10-30%

---

## 8. risk

| risk | mitigation |
|---|---|
| Win32 API 가 너무 광대해 D2R 까지 못 도달 | OSS Win 게임 부분 작동 시점에 minimal viable 선언 |
| hexa-lang stage1 한계 (float, bit ops, dict) | HEXA_UPSTREAM Phase A PR 우선 |
| Anti-cheat 우회 불가능 (D2R/D4 가 BattlEye 사용 시) | scope 외 — D2R/D4 anti-cheat 정책 변경 대기 |
| DRM / 인증 (Battle.net 의존) | direct game.exe 실행 시도, 안 되면 scope 외 |
| Apple D3DMetal API 변경 | macOS major version pinned 검증 |

---

## 9. 참고

- Wine 자체: https://gitlab.winehq.org/wine/wine — 3M+ LoC C code 참고용 (코드 import X, API spec 참고용)
- Microsoft Win32 API docs: https://learn.microsoft.com/en-us/windows/win32/api/
- PE/COFF spec: https://learn.microsoft.com/en-us/windows/win32/debug/pe-format
- Apple D3DMetal docs: developer.apple.com (Game Porting Toolkit)
- Apple Mach-O reference: macOS Internals

→ Wine code 직접 참조 X (own 1 enforcement). Microsoft 공식 spec / API docs 만 참조.

---

*written 2026-04-30. minimal native path 결정. 9-12 달 연구 프로젝트로 commit.*
