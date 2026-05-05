# 잔여 로드맵 — skeleton 128 모듈 이후 (2026-05-01)

> **status**: 작성 2026-05-01 (cycle 79 commit f20aaa6 직후)
> **컨텍스트**: cycle 64–79 16 cycle / 128 skeleton 모듈 모두 PASS + push 완료. 그러나 **skeleton = TSV 추적 layer** ≠ 실 동작 layer. 본 문서는 "실 launch 가능 까지 잔여 분량" 등록 + 만점 검토 cross-ref.

## ❌ 실 작동 prereq (실 launch 까지 핵심 blocker)

| # | 영역 | 만점 검토 | 현 상태 | 만점 도달 시 효과 |
|---|---|---|---|---|
| **R1** | stage7 native bridge 실 호출 | [STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md](STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md) | skeleton (cycle 63 wrappers) | dlopen/mmap/pthread_create 실 호출 → 첫 native code 실행 |
| **R2** | PE binary 실 mmap 적재 | [PE_LOADER_REAL_MMAP_MANJEOM_REVIEW.md](PE_LOADER_REAL_MMAP_MANJEOM_REVIEW.md) | `pe_loader_stage2` skeleton | `.text/.data` 실 mmap + reloc → entry point 도달 |
| **R3** | IAT 실 resolve | [IAT_REAL_RESOLVE_MANJEOM_REVIEW.md](IAT_REAL_RESOLVE_MANJEOM_REVIEW.md) | `pe_iat_resolve_stage2` skeleton | 실 stub 주소 patch + forwarder/ordinal → DLL 호출 동작 |
| **R4** | x86_64 → arm64 binary translation | [BINARY_TRANSLATION_MANJEOM_REVIEW.md](BINARY_TRANSLATION_MANJEOM_REVIEW.md) | codegen 추적기만 (cycle 67 `pe_jit_codegen_track`) | 실 JIT engine — Rosetta 미사용 시 |
| **R5** | D3D11/12 → Metal shader 변환 | [D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md](D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md) | skeleton만 | HLSL → MSL cross-compile + pipeline state 매핑 |
| **R6** | 220+ DLL stub 함수 실 구현 | [WIN32_DLL_REAL_IMPL_MANJEOM_REVIEW.md](WIN32_DLL_REAL_IMPL_MANJEOM_REVIEW.md) | KERNEL32/USER32/etc. stub만 | 각 함수 실 동작 (GetSystemTime 실 시간 반환 등) |

## ⚠️ 정비 항목

| # | 영역 | 만점 검토 | 현 상태 |
|---|---|---|---|
| **M1** | `win32_abi_i386` self-test FAIL 수정 | [WIN32_ABI_I386_FAIL_FIX_MANJEOM_REVIEW.md](WIN32_ABI_I386_FAIL_FIX_MANJEOM_REVIEW.md) | pre-existing FAIL, 미수정 |
| **M2** | Folder split refactor | [FOLDER_SPLIT_REFACTOR_MANJEOM_REVIEW.md](FOLDER_SPLIT_REFACTOR_MANJEOM_REVIEW.md) | 보류됨 (사용자 결정 대기) |

## ✅ 보류 (PRIORITY_DIRECTIVE 후순위)

| 항목 | 상태 | 근거 |
|---|---|---|
| Gameplay 편의성 (build optimizer/drop calc/MF/charm packing 등) | 보류 | PRIORITY_DIRECTIVE.md "hell mode 어쩌구 편의성관련" |
| Hell mode / quality-of-life feature | 보류 | 동일 |
| 실 게임 install / launch 실험 | 보류 | 사용자 directive "실제 작동실험 외 나머지 모두 구현" |

## 의존성 그래프

```
R1 (stage7 native bridge)
   ├── R2 (PE 실 mmap) — R1 의 mmap/mprotect 의존
   │     └── R3 (IAT 실 resolve) — R2 의 적재 영역 의존
   │           └── R6 (DLL stub 실 구현) — R3 의 함수 주소 patch 의존
   └── R4 (binary translation) — R1 의 mmap RWX 의존
   
R5 (D3D → Metal shader) — 독립 (R6 와 약결합)
M1 (FAIL 수정) — 독립 (소규모)
M2 (folder split) — 독립 (refactor only)
```

## ETA / cycle 수 estimate (BG agent 병렬 가정)

| 영역 | 예상 cycle | bg 병렬 |
|---|---|---|
| R1 | 4 cycle | dlopen/mmap/mprotect/pthread 실 호출 모듈별 |
| R2 | 3 cycle | section mapper / reloc applier / TLS init |
| R3 | 2 cycle | direct/forwarder/ordinal/delay-load |
| R4 | 8+ cycle | 가장 큼 (x86 decoder + arm64 emit + signal/SEH 통합) |
| R5 | 6 cycle | HLSL parser + MSL emitter + pipeline state translator |
| R6 | 12+ cycle | DLL 함수 ~20개씩 실 구현 (cycle 당) |
| M1 | 1 cycle | FAIL 위치 + 수정 |
| M2 | 2 cycle | 폴더 구조 + import path 일괄 |

## 만점 검토 채점 축 (본 문서 잔여 영역 전용)

각 영역별 만점 검토 docs는 다음 5축 / **/25** 만점 사용:

| 축 | 정의 | 1점 | 5점 |
|---|---|---|---|
| **A. own1 정합** | Wine 0줄 유지 가능? | community 코드 필수 | 100% DIY hexa |
| **B. 실 동작 도달** | 만점 시 launch 가능? | 별도 layer 필요 | 단독 launch 가능 |
| **C. 자원 영향** | perf/메모리 cost | 큰 overhead | 무시할 수준 |
| **D. 구현 난이도 (역)** | 낮을수록 점수 높음 | 8+ cycle 필요 | 1 cycle 이내 |
| **E. 의존성 (역)** | 적을수록 점수 높음 | 5+ 다른 영역 prereq | 독립 실행 |

→ 5×5 = **25 만점**. 15 이상 = sprint 후보. 10–14 = 후순위. <10 = 폐기 검토.

---

*written 2026-05-01 cycle 79 직후. 사용자 directive: "잔여 로드맵 등록 만점 검토 병렬 bg 기준".*
