# R5 — D3D11/12 → Metal shader 변환 만점 검토

> **status**: 검토일 **2026-05-01** / **만점 가능성: 부분 가능 (Apple D3DMetal 위임 60% + DIY hexa 40% 기준 — A=5, B=4, C=4, D=2, E=4 → /25 총점 19**)
> 작성: 2026-05-01 (cycle 79 직후 잔여 로드맵 R5)
> SSOT: `docs/REMAINING_ROADMAP.md` (5축×5 채점 정의), `docs/PURPLE_LINEAGE_MANJEOM_REVIEW.md` (형식)
> cross-ref: `docs/SCORING.md` D1 (`c_dxil_persist`, 14/15) + R2 (`c_dxil_to_air_fusion`, 만점 자리), `lib/loader/dx_d3d11.hexa` (cycle Phase 5a 30 method mapping table), `lib/loader/dx_d3d12*.hexa`, `lib/loader/pe_d3d11_compute_shader.hexa`, `lib/loader/pe_d3d12_mesh_shader.hexa`, `lib/loader/pe_d3d12_raytracing.hexa`
> 결론 한줄: **own 1 의 "Apple D3DMetal binary" 항목 덕에 D3D11/12 핵심 path 는 D3DMetal 위임 가능 — DIY hexa 비용은 (1) HLSL→MSL cross-compile fallback (D3DMetal 미지원 shader profile), (2) bindless / argument buffer mapping, (3) DXR / mesh shader 최신 path, (4) PSO genome cache (D2/D3) 만 부담. 단, mesh shader (Lineage 2M / D4 후기 patch) + DXR (D4 / TL UE5) 는 D3DMetal 의 macOS 14+ 한정 + Apple GPU family 7+ 한정 — Mac 별 partial.**

---

## 0. 본 영역 정의 (R5 의 정확한 scope)

### 0-1. R5 대상

`docs/REMAINING_ROADMAP.md` 정의:

> **R5** — D3D11/12 → Metal shader 변환. skeleton만. 만점 도달 시 효과: HLSL → MSL cross-compile + pipeline state 매핑.

본 검토는 R5 의 **만점 도달 path** 를 5축 × 5점 = /25 로 채점.

### 0-2. 작업 분할 (sub-area)

| sub | 영역 | 현 skeleton |
|---|---|---|
| **R5a** | DXBC (D3D11 SM 5.0) → MSL | `lib/loader/dx_d3d11.hexa` 30 method mapping |
| **R5b** | DXIL (D3D12 SM 6.x) → MSL | `lib/loader/dx_d3d12_stage1.hexa`, `dx_d3d12_stage2.hexa` |
| **R5c** | HLSL semantic (SV_*) 매핑 | (없음 — R5 신규) |
| **R5d** | Constant buffer / packing | (없음 — R5 신규) |
| **R5e** | Bindless / descriptor heap → argument buffer | `c_argument_buffer_tier2_map` (이미 존재) |
| **R5f** | Compute shader dispatch | `pe_d3d11_compute_shader.hexa` (cycle 76) |
| **R5g** | Mesh shader (D3D12 SM 6.5) | `pe_d3d12_mesh_shader.hexa` (cycle 76), `c_mesh_shader_cull` |
| **R5h** | Ray tracing (DXR) | `pe_d3d12_raytracing.hexa` (cycle 76), `c_dxr_*` 4 모듈 |
| **R5i** | PSO genome cache (D2/D3 직접 적용) | `c_pso_genome` (D2 만점), `c_frame_cmd_genome` (D3 만점) |
| **R5j** | DXIL → AIR translation cache (R2 만점) | `c_dxil_persist` (D1, 14), `c_dxil_to_air_fusion` (D-list 만점) |

→ **R5j (= SCORING D1 + R2) 는 이미 SCORING 만점 자리 — 본 검토 점수에 100% 산입**.
→ R5e, R5f, R5g, R5h 는 **skeleton 단계 telemetry 모듈로 cycle 76 에 진입 완료** — 실 변환은 미구현.

### 0-3. own 1 적용 결정

| layer | own 1 정합 | 출처 |
|---|---|---|
| Apple D3DMetal.framework binary 호출 | ○ — own 1 정의 명시 ("DMG/redist/lib/external/D3DMetal.framework (Apple 공식 binary)") | `lib/loader/dx_d3d11.hexa` 헤더 |
| Microsoft D3D11/D3D12 spec 참조 (문서) | ○ — Microsoft Learn 공식 docs 참조 가능 | 동상 |
| Apple Metal spec 참조 (문서) | ○ — developer.apple.com 공식 docs 참조 가능 | 동상 |
| Apple Metal Shader Converter (공식 toolchain — `metal-shaderconverter`) | ○ — Apple 공식 tool, own 1 의 "Apple D3DMetal binary" 와 같은 등급 | Apple WWDC23+ |
| Wine `d3d11.dll` 구현 | ✗ — own 1 위반 | feedback_own1 |
| DXVK (Vulkan-backed D3D11) | ✗ — Wine community | 동상 |
| VKD3D / VKD3D-Proton | ✗ — Wine community | 동상 |
| MoltenVK (Vulkan→Metal) | ✗ — KhronosGroup community (own 1 외부) | 동상 |
| Apple GPTK (Game Porting Toolkit) | △ — GPTK 자체는 D3DMetal binary 의 dev kit 묶음 → binary 부분만 own 1 OK, 빌드 script 부분 own 1 외부 | own 1 정의 |

→ **본 검토는 (a) D3DMetal.framework binary 호출 + (b) Apple metal-shaderconverter 호출 + (c) DIY hexa cross-compile fallback 3 path 만 own 1 정합으로 인정**.

---

## 1. 5 축 × 5 점 채점

### 축 A — own 1 정합 (Wine 0줄 가능?) → **5/5**

**판정**: 5 — 100% 가능.

**근거**:
- own 1 은 "Apple D3DMetal binary" 명시 — D3DMetal.framework 가 D3D11 / D3D12 → Metal 변환의 90% 처리.
- D3DMetal 미지원 영역 (예: D3D9, geometry shader, work graph, SM 6.7+ 일부 신규 op) 은 DIY hexa cross-compile fallback 으로 처리 — Wine/DXVK/VKD3D/MoltenVK 0줄.
- HLSL bytecode (DXBC/DXIL) parser 는 Microsoft 공식 spec 기반 DIY hexa 작성 가능 (Microsoft `DXBCSignature.h` / `DXILMetadataAnalysis.h` 는 docs only, BSD-style spec).
- MSL emitter 는 Apple Metal Shading Language Specification (PDF, 공식) 기반 DIY hexa.
- **risk**: cross-compile fallback path 가 community DXIL parser (`DirectX-Shader-Compiler` LLVM fork) 코드를 참조해야 정확도 확보 가능 → **참조 X, spec only** 정책 강제 시 정확도 trade-off 필요. 그러나 own 1 우선 → 5점 유지.

**감점 사유**: 없음.

---

### 축 B — 실 동작 도달 (만점 시 단독 launch 가능?) → **4/5**

**판정**: 4 — 단독 launch 가능, 단 D3DMetal 미지원 game 은 fallback path 필수.

**근거**:
- D3DMetal 위임으로 D2R (DX11) / D4 (DX12 + DXR) **first frame 도달 가능** — 이미 `c_d2r_first_frame_trace` / `c_d4_dxr_first_frame_trace` skeleton 존재.
- Lineage 2 (DX9 UE2.5) — D3DMetal 의 DX9 path 는 GPTK 에서 명시 지원, **부분 가능** (단 own 1 cross-ref `docs/PURPLE_LINEAGE_MANJEOM_REVIEW.md` §3 참조).
- CS2 (Source 2 DX11) — `c_cs2_source2_pso_genome` 만점 자리, 도달 가능.
- Elden Ring (DX12 from MS Store) — `c_elden_dx12_first_frame` skeleton, EAC 차단으로 **gameplay 차단** (R5 무관, 차단은 anti-cheat).
- **감점 사유 1점**: D3DMetal 은 macOS 14.4+ + Apple GPU family 7+ 한정 — Intel Mac / Apple GPU family 6 이하는 **fallback DIY path 강제 발동** → 일부 환경에서 단독 launch 시 fallback 코드 cycle 추가 필요. 즉 R5 만점 도달이 곧 모든 macOS 환경 launch 보장 X.

→ R1/R2/R3 (PE 적재 + IAT) 도달 prerequisite — R5 단독은 launch X. 그러나 **R5 가 만점이면 graphics layer 는 차단되지 않음** = "만점 시 launch 가능" 의 substantive 기여.

---

### 축 C — 자원 영향 (perf / memory cost) → **4/5**

**판정**: 4 — D3DMetal binary 자체는 무료 (Apple 제공), DIY hexa fallback + cache 영역만 cost.

**근거**:
- D3DMetal 위임 path: shader 변환 cost 는 D3DMetal binary 내부 (Apple 책임), gamebox 측 추가 cost 0.
- DXIL → AIR translation cache (R5j = SCORING R2): **2nd launch zone hitch 제거** — net positive (cost 가 아니라 saving). `c_dxil_persist` 는 첫 launch 에 1회 columnar persist (수 MB), 이후 건너뜀.
- PSO genome cache (R5i = SCORING D2/D3): airgenome harvest 결과 직접 적용 — 첫 launch 에 PSO 미리 build → first frame hitch 제거. **memory 비용**: PSO 100~500 개 × 2~10KB = 0.2~5MB persist + warm in-memory 동량.
- HLSL→MSL cross-compile fallback (D3DMetal 미지원 shader): 변환 cost ~수 ms / shader, 1회. cache 후 0.
- Bindless argument buffer (`c_argument_buffer_tier2_map`): tier 2 mapping 은 build 시 1회, runtime 0.
- **감점 사유 1점**: DXR shader (D4 ray gen / closest hit / miss) 는 정적 분석으로 PSO genome 생성 비용 큼 — `c_d4_dxr_bvh_compact` + `c_dxr_inline_rt_shader` 가 cycle 단 작업이지만 첫 launch 시 BVH compaction 까지 묶이면 **cold-start 1~2초 추가 가능**. 만점 후에도 cold launch 만 살짝 길어짐.

→ **steady-state / 2nd-launch perf**: 만점 시 native Metal 과 사실상 동일 (D2R/D4 native Mac port 가 없으므로 비교는 가설).

---

### 축 D — 구현 난이도 (역, 낮을수록 점수 ↑) → **2/5**

**판정**: 2 — 가장 큰 신규 cost. R4 (binary translation, 8+ cycle) 다음으로 큰 영역.

**근거** (R5 sub 별 cycle 견적):

| sub | 견적 cycle | 비고 |
|---|---|---|
| R5a DXBC parser + MSL emit (vertex/pixel) | 1.5 cycle | DXBC spec 단순 (bytecode container) |
| R5b DXIL parser (LLVM IR fork) + MSL emit | 2 cycle | DXIL = LLVM 3.7 fork — IR parser 가 큼. Apple metal-shaderconverter 위임 시 0.5 cycle 로 단축 |
| R5c SV_* semantic 매핑 | 0.5 cycle | 30 SV_* → Metal `[[attribute]]` table |
| R5d Constant buffer packing | 0.5 cycle | HLSL packing rule (16-byte boundary) → MSL `constant T &` |
| R5e Bindless / argument buffer (이미 skeleton) | 0.5 cycle | `c_argument_buffer_tier2_map` 위에 D3D12 descriptor heap mapping |
| R5f Compute shader (skeleton 존재) | 0.5 cycle | thread group → `[[threads_per_threadgroup]]` |
| R5g Mesh shader (Lineage 2M / D4 patch) | 1 cycle | macOS 13+ Apple GPU family 7+ 한정. fallback = vertex shader 강등 |
| R5h DXR (D4 / TL UE5) | 1 cycle | Metal Ray Tracing 4 매핑. macOS 13+ 한정 |
| R5i PSO genome cache (이미 만점) | 0 cycle | `c_pso_genome` SCORING D2 = 15/15 |
| R5j DXIL → AIR cache (이미 만점 자리) | 0 cycle | `c_dxil_to_air_fusion` SCORING |
| **합계** | **6 cycle** | REMAINING_ROADMAP §ETA estimate (R5 = 6 cycle) 와 일치 |

→ 6 cycle 은 **RR 5축 정의 "8+ cycle 필요 = 1점, 1 cycle 이내 = 5점"** 에서 약 2점 위치 (3 cycle = 4점, 5 cycle = 3점, 8+ = 1점). 6 cycle ≈ **2점**.

**감점 사유**: 4-5 cycle 큼. Apple metal-shaderconverter 가 안정적이라면 **R5b (DXIL → MSL)** 단축으로 4 cycle = 3점 까지 상승 가능 (조건부).

---

### 축 E — 의존성 (역, 적을수록 점수 ↑) → **4/5**

**판정**: 4 — R6 (DLL stub) 와 약결합, R1/R2/R3 와 무관.

**근거**:
- `docs/REMAINING_ROADMAP.md` §의존성 그래프 명시: "R5 (D3D → Metal shader) — **독립** (R6 와 약결합)"
- R5 가 만점이어도 R1 (stage7 native bridge) / R2 (PE mmap) / R3 (IAT) / R4 (binary translation) 가 prereq — **R5 자체의 의존성** 은 D3D11.dll / D3D12.dll IAT bind (R3 의 sub) + native bridge dispatch (R1 의 sub) 까지가 전부.
- D3DMetal.framework load (dlopen) — R1 의 `dlopen` 만점 시 자동 해결.
- 본 R5 가 다른 영역에 prereq 부담 X — D-list (D1) / R-list (R2) 만점 자리 대상 아님.
- **감점 사유 1점**: R6 (DLL stub 실 구현) 의 D3D11.dll / D3D12.dll / DXGI.dll stub 가 R5 의 entry point — R6 가 stub 만 반환하면 R5 path 가 호출되지 않음. 즉 **R5 의 진짜 활성화는 R6 의 D3D 영역 stub 가 D3DMetal trampoline 으로 patch 된 후**.

---

### 1-1. 종합 채점 표

| 축 | 점수 | 1줄 근거 |
|---|---|---|
| A. own 1 정합 | **5** | D3DMetal binary + Apple metal-shaderconverter + DIY hexa fallback — Wine 0줄 가능 |
| B. 실 동작 도달 | **4** | D2R/D4/CS2 first frame 가능, GPU family 6 이하 + Intel Mac 만 fallback path 필수 |
| C. 자원 영향 | **4** | D3DMetal 위임 cost 0, DXIL→AIR cache 는 net positive, DXR cold-start 만 +1~2초 |
| D. 구현 난이도 (역) | **2** | 6 cycle (8+=1, 1=5 spectrum 에서 2점 위치) — 가장 큰 신규 cost |
| E. 의존성 (역) | **4** | 독립 (R6 약결합만), R1/R2/R3 의 sub 일부 prereq 흡수 |
| **합계** | **19/25** | sprint 후보 (15 이상 cutoff). |

→ **결론: 19/25 — sprint 진입 가능 + R5j (D1+R2) 의 SCORING 만점 자리 2 개 합산 시 실효 가치 상승**.

---

## 2. 핵심 blocker 후보 별 분석

### 2-1. DXBC (D3D11) vs DXIL (D3D12) — 후자 LLVM IR 기반

| 항목 | DXBC (D3D11 SM 5.0) | DXIL (D3D12 SM 6.x) |
|---|---|---|
| format | binary container ("DXBC" magic) | LLVM 3.7 IR + custom metadata + DXIL container |
| parse 난이도 | 낮음 (op 200+) — DIY hexa 1.5 cycle | 높음 (LLVM IR full parser) — DIY hexa 2+ cycle |
| Apple metal-shaderconverter 지원 | △ (간접 — D3D11 path 는 D3DMetal 내부) | ○ (공식 지원 — `metal-shaderconverter --input dxil`) |
| own 1 분리 | D3DMetal binary 위임 가능 | metal-shaderconverter 위임 가능 (Apple binary) |
| DIY hexa fallback 필요 | rare op (geometry shader 등) | rare op (work graph SM 6.8) |

→ **결론: DXIL 은 metal-shaderconverter 100% 위임 가능 → 실 구현 cost 0.5 cycle 로 단축 가능. DXBC 는 D3DMetal 자체에 묶임 (분리 X) — 이 경로가 깨지는 game 은 거의 없음 (DX11 게임 대다수 D3DMetal 호환)**.

### 2-2. Metal shader compilation chain

```
HLSL (.hlsl source)
   ↓ Microsoft DXC / FXC (Windows-side, gamebox 무관 — 게임 빌드 시점)
DXBC / DXIL (.cso bytecode — game 배포 binary 내장)
   ↓ ★ R5 의 input ★
   ├─ path α: D3DMetal.framework 내부 변환 (D3D11 / D3D12 normal path)
   ├─ path β: Apple metal-shaderconverter (DXIL → MSL source)
   └─ path γ: DIY hexa cross-compile (D3DMetal 미지원 op 만)
   ↓
MSL (.metal source)
   ↓ Apple metal compiler (Xcode metal toolchain, runtime 가능)
AIR (Apple Intermediate Representation, .metallib)
   ↓ on-device GPU driver (Apple SoC firmware)
GPU machine code (per-family — A14 / M1 / M2 / M3 / M4)
```

→ **R5 의 책임 영역**: HLSL bytecode → AIR.
→ **gamebox 측 work**: path α (위임), path β (Apple tool 호출 wrapper), path γ (DIY hexa) 의 router + fallback chain.

### 2-3. HLSL semantic (SV_*) 매핑

| HLSL semantic | Metal attribute | 비고 |
|---|---|---|
| SV_Position | [[position]] | 동형 |
| SV_VertexID | [[vertex_id]] | 동형 |
| SV_InstanceID | [[instance_id]] | 동형 |
| SV_PrimitiveID | [[primitive_id]] | 동형 |
| SV_DispatchThreadID | [[thread_position_in_grid]] | 동형 |
| SV_GroupID | [[threadgroup_position_in_grid]] | 동형 |
| SV_GroupThreadID | [[thread_position_in_threadgroup]] | 동형 |
| SV_GroupIndex | [[thread_index_in_threadgroup]] | 동형 |
| SV_Target0..7 | [[color(N)]] | 동형 |
| SV_Depth | [[depth(any)]] | 동형 |
| SV_DepthGreaterEqual | [[depth(greater)]] | 약간 다름 |
| SV_DepthLessEqual | [[depth(less)]] | 약간 다름 |
| SV_RenderTargetArrayIndex | [[render_target_array_index]] | Apple GPU family 5+ 한정 |
| SV_ViewportArrayIndex | [[viewport_array_index]] | 동상 |
| SV_SampleIndex | [[sample_id]] | 동형 |
| SV_Coverage | [[sample_mask]] | 동형 |
| SV_StencilRef | [[stencil]] | macOS 12+ 한정 |
| SV_GSInstanceID | (없음) | geometry shader 자체가 mesh shader 강등 path 필요 |
| SV_DomainLocation / SV_TessFactor / SV_InsideTessFactor | (Metal tessellation) | post-stage 직접 mapping X — patch_control_point 재구성 |
| SV_ShadingRate | [[fragment_shading_rate]] | macOS 13+ |

→ 30 SV_* 의 mapping table — **R5c, 0.5 cycle. 단순 lookup table** (이미 `dx_d3d11.hexa` 의 30-method table 패턴 재사용 가능).

### 2-4. Constant buffer layout (HLSL packing rule vs MSL)

| 항목 | HLSL | MSL |
|---|---|---|
| packing 단위 | 16-byte (float4) boundary | natural alignment (struct field 별) |
| `cbuffer` → struct | `cbuffer Camera : register(b0) { float4x4 view; }` | `struct Camera { float4x4 view; }` + `constant Camera &cam [[buffer(0)]]` |
| float3 다음 float | 4-byte 차이 발생 (HLSL 은 float3 가 16-byte boundary 끝까지 padding X if next fits, MSL 은 자연 alignment) | 직접 mapping 시 layout mismatch — explicit `packed_float3` 강제 필요 |
| matrix layout | row-major default (HLSL) / column-major (D3D 호출 시 transpose) | column-major default (MSL) — `column_major float4x4` 명시 |
| array of struct | tightly packed | array stride 가 다를 수 있음 |

→ **R5d, 0.5 cycle. 잘 알려진 변환** — 단 Lineage 2 (UE2.5 + 구식 HLSL 1.x) 같은 legacy 는 구조체 misalignment risk 큼 → fuzz test 필요.

### 2-5. Bindless resource (D3D12 descriptor heap → Metal argument buffer)

| 항목 | D3D12 | Metal |
|---|---|---|
| primary | descriptor heap (CPU 가 GPU 가 visible 한 table 제공) | argument buffer (struct of resources, encoded into MTLBuffer) |
| tier | tier 1 / 2 / 3 (resource 종류 별 제약) | tier 1 / 2 (Apple GPU family 별) |
| 매핑 | D3D12 tier 3 → Metal argument buffer tier 2 (Apple GPU 7+) | tier 2 미만 = fallback 강등 (개별 binding) |
| 현 skeleton | `c_argument_buffer_tier2_map` (이미 SCORING 진입 — perf 모듈) |

→ **R5e, 0.5 cycle**. `c_argument_buffer_tier2_map.hexa` 가 이미 존재 — D3D12 측 descriptor heap entry 를 Metal argument buffer slot 으로 변환하는 logic 추가만.

### 2-6. Tessellation / geometry shader → Metal mesh shader

| 항목 | D3D11 GS | D3D11/12 Tessellation | Metal mesh shader |
|---|---|---|---|
| Apple 지원 | 없음 (geometry shader 자체가 Metal 미존재) | 있음 (post-tess vertex stage) | 있음 (macOS 13+ Apple GPU family 7+ 한정) |
| fallback path | compute shader 로 강등 (vertex 생성) | vertex + tess | vertex + skinning compute |
| Lineage 2 (DX9 UE2.5) | (geometry shader 미사용 — DX9 시대) | 동상 | N/A |
| D4 후기 patch | (mesh shader 사용 추정) | 동상 | mesh path 진입 |
| Lineage 2M / W (UE4) | rare (대부분 vertex) | rare | N/A |

→ **R5g, 1 cycle**. mesh shader path 는 macOS 13+ + GPU family 7+ 한정 — 그 외는 vertex+compute 강등 fallback.
→ 현 skeleton: `pe_d3d12_mesh_shader.hexa` (cycle 76) + `c_mesh_shader_cull` perf.

### 2-7. Ray tracing (DXR → Metal Ray Tracing 4)

| 항목 | DXR (D3D12 ray tracing) | Metal Ray Tracing 4 |
|---|---|---|
| Apple 지원 | macOS 13+ Apple GPU family 7+ (M2 이상 효과적) |
| acceleration structure | TLAS / BLAS (D3D12) | MTLAccelerationStructure (Metal) — primitive 호환 |
| ray gen / closest hit / miss / any hit | 5 stage (raygen, miss, closesthit, anyhit, intersection) | 동형 mapping |
| inline ray tracing | DXR 1.1+ (HLSL `RayQuery`) | Metal `intersection_query` (macOS 13+) |
| BVH compaction | DXR `CopyAccelerationStructure(MODE=COMPACT)` | Metal `MTLAccelerationStructureRefitDescriptor` (refit 만, true compact 는 rebuild) |

→ **R5h, 1 cycle**. 현 skeleton: `pe_d3d12_raytracing.hexa` (cycle 76) + `c_d4_dxr_bvh_compact` + `c_dxr_inline_rt_shader` + `c_dxr_blas_refit_track` + `c_dxr_ray_coherence` 4 perf 모듈.
→ **D4 (DXR + Lumen 일부) / TL UE5 (Lumen full)** 가 본 path 의 1순위 — 단 TL 은 anti-cheat 차단으로 R5 무관.

### 2-8. D3DMetal 위임 가능 영역 vs DIY hexa 영역 분리

| 영역 | D3DMetal 위임 | DIY hexa fallback | router |
|---|---|---|---|
| D3D11 vertex / pixel shader | **위임** | 거의 사용 X | dx_d3d11.hexa |
| D3D11 compute shader | **위임** | rare | pe_d3d11_compute_shader.hexa |
| D3D11 geometry shader | △ 부분 (D3DMetal 의 emul) | 강등 fallback (compute) | dx_d3d11.hexa Shader 6 |
| D3D11 hull / domain (tessellation) | **위임** | rare | dx_d3d11.hexa Shader 6 |
| D3D12 vertex / pixel / compute | **위임** | 거의 사용 X | dx_d3d12.hexa |
| D3D12 mesh shader (SM 6.5+) | **위임** (macOS 13+ + family 7+) | 강등 (vertex+compute) | pe_d3d12_mesh_shader.hexa |
| DXR (D3D12 ray tracing) | **위임** (macOS 13+ + family 7+) | 강등 (compute screen-space approximation) | pe_d3d12_raytracing.hexa |
| HLSL legacy (DX9 UE2.5 — Lineage 2) | △ (D3DMetal 의 D3D9 path — GPTK 명시) | 강등 (DXBC re-translation) | (R5 신규) |
| SM 6.7+ wave intrinsics | △ | DIY hexa lowering (Apple GPU SIMD group) | metal-shaderconverter |
| SM 6.8 work graph | ✗ (D3DMetal 미지원) | DIY hexa lowering (compute graph 강등) | DIY |

→ **결론: ~85% D3DMetal 위임 + ~15% DIY hexa fallback** — own 1 정합 + cycle cost 최소화.

---

## 3. 추천 모듈 candidate (cycle 80+ 후보)

### 3-1. R5 신규 모듈 (DIY hexa)

| candidate | sub | A | B | C | D | E | /25 | 비고 |
|---|---|---|---|---|---|---|---|---|
| `c_d3d_to_msl_router` | R5 router | 5 | 5 | 5 | 4 | 4 | **23** | path α/β/γ 결정 router. shader profile 별 분기 |
| `c_dxbc_parse_track` | R5a | 5 | 4 | 5 | 3 | 5 | 22 | DXBC container parse (DIY, Microsoft spec) |
| `c_dxil_parse_track` | R5b | 5 | 4 | 5 | 2 | 5 | 21 | DXIL = LLVM IR — metal-shaderconverter 위임 시 5 |
| `c_msl_emit_track` | R5a/b | 5 | 4 | 5 | 3 | 4 | 21 | MSL 텍스트 emit (path γ fallback) |
| `c_hlsl_sv_semantic_map` | R5c | 5 | 4 | 5 | 5 | 5 | **24** | 30 SV_* lookup table — `dx_d3d11.hexa` 패턴 재사용 |
| `c_cbuffer_packing_predict` | R5d | 5 | 4 | 5 | 4 | 5 | 23 | HLSL packing → MSL packed_float3 변환 prediction |
| `c_d3d12_descriptor_heap_argbuf_map` | R5e | 5 | 4 | 5 | 4 | 4 | 22 | descriptor heap → argument buffer slot mapping (`c_argument_buffer_tier2_map` 흡수) |
| `c_metal_shader_converter_invoke_track` | R5b | 5 | 4 | 5 | 5 | 4 | 23 | Apple metal-shaderconverter wrapper telemetry |
| `c_d3dmetal_delegate_route_track` | R5 router | 5 | 5 | 5 | 5 | 4 | **24** | path α 위임 결정 telemetry |
| `c_dxr_acceleration_struct_translate` | R5h | 5 | 4 | 4 | 3 | 4 | 20 | TLAS/BLAS → MTLAccelerationStructure |
| `c_mesh_shader_lowering_compute_fallback` | R5g | 5 | 4 | 4 | 3 | 4 | 20 | mesh shader → compute+vertex 강등 (family 6 이하) |
| `c_geometry_shader_compute_lowering` | R5a | 5 | 3 | 5 | 3 | 4 | 20 | DX11 geometry shader → compute lowering |

→ **추천 12개**. 평균 22/25 — sprint 후보 충분.

### 3-2. SCORING 만점 자리 cross-ref (이미 존재)

| 모듈 | SCORING 위치 | R5 sub |
|---|---|---|
| `c_dxil_persist` | D-list D1 (14/15) | R5j |
| `c_dxil_to_air_fusion` | D-list 만점 자리 | R5j |
| `c_pso_genome` | D-list D2 (15/15) | R5i |
| `c_frame_cmd_genome` | D-list D3 (15/15) | R5i |
| `c_metal_heap_hexagon` | D-list D5 (15/15) | R5e (heap layout) |
| `c_shader_warm` | R-list R1 (10/10) | R5j (warm-up) |
| `c_argument_buffer_tier2_map` | (perf, 만점 후보) | R5e |
| `c_dxr_inline_rt_shader` / `c_dxr_blas_refit_track` / `c_dxr_ray_coherence` / `c_d4_dxr_bvh_compact` | (DXR 4 perf 모듈) | R5h |
| `c_mesh_shader_cull` | (perf) | R5g |
| `c_dxil_delta` / `c_dxil_vector_scalarize` | (perf) | R5b |

→ **이미 R5 영역에 7개 만점 + 4 perf 모듈 존재** — R5 만점 검토는 신규 모듈만 작성하면 됨.

### 3-3. cycle 80+ 추천 batch

**cycle 80 (8 모듈 — R5 핵심 router + sub 시작)**:
1. `c_d3d_to_msl_router` (23)
2. `c_d3dmetal_delegate_route_track` (24)
3. `c_hlsl_sv_semantic_map` (24)
4. `c_cbuffer_packing_predict` (23)
5. `c_metal_shader_converter_invoke_track` (23)
6. `c_dxbc_parse_track` (22)
7. `c_dxil_parse_track` (21)
8. `c_msl_emit_track` (21)

**cycle 81 (4 모듈 — R5 신규 path)**:
9. `c_d3d12_descriptor_heap_argbuf_map` (22)
10. `c_dxr_acceleration_struct_translate` (20)
11. `c_mesh_shader_lowering_compute_fallback` (20)
12. `c_geometry_shader_compute_lowering` (20)

→ **2 cycle / 12 모듈 = R5 신규 영역 완전 cover**. R5 의 6 cycle estimate 중 4 cycle 은 sub-implementation (실 변환 logic) — skeleton 만으로는 2 cycle 충분.

---

## 4. 결론

### 4-1. /25 종합 점수

**19/25** — sprint 후보 (15 cutoff 통과).

| 축 | 점수 |
|---|---|
| A. own 1 정합 | 5 |
| B. 실 동작 도달 | 4 |
| C. 자원 영향 | 4 |
| D. 구현 난이도 (역) | 2 |
| E. 의존성 (역) | 4 |
| **합계** | **19** |

### 4-2. 우선순위 / 즉시 권장 action

**1순위 (cycle 80 batch)**: R5 router + path 분리 8 모듈 작성 — `c_d3d_to_msl_router` + `c_d3dmetal_delegate_route_track` 가 핵심 (path α/β/γ 결정).

**2순위 (cycle 81 batch)**: R5 신규 fallback path 4 모듈 — descriptor heap, DXR translate, mesh / geometry lowering.

**3순위 (cycle 82+)**: R5j (D1+R2) 만점 자리 검증 — `c_dxil_persist` (D1, 14) → 15 승급, `c_dxil_to_air_fusion` 만점 검증.

**보류**: HLSL legacy DX9 path (Lineage 2 UE2.5) — `docs/PURPLE_LINEAGE_MANJEOM_REVIEW.md` 의 GameGuard 차단으로 live 만점 불가, private 서버 한정. R5 본 검토에서 secondary.

### 4-3. 다른 R 영역과의 우선순위

`docs/REMAINING_ROADMAP.md` 6 영역 중 R5 의 위치:

| 영역 | 종합 (예상) | 정합 |
|---|---|---|
| R1 stage7 native bridge | TBD (만점 검토 별도) | foundational |
| R2 PE 실 mmap | TBD | foundational |
| R3 IAT 실 resolve | TBD | foundational |
| R4 binary translation | 낮음 (8+ cycle, D=1) | optional (Rosetta 2 위임 가능) |
| **R5 D3D → Metal shader** | **19/25** | **선택적이지만 own 1 의 D3DMetal 명시로 cost 낮음** |
| R6 220+ DLL stub 실 구현 | TBD (12+ cycle, D=1) | foundational + heavy |

→ **R5 는 R1/R2/R3 다음 sprint priority**. R4 (binary translation) 보다 우선 — Rosetta 2 위임 가능 (own 1 명시) 하지만 R5 는 D3DMetal 위임 가능 + cycle 적음 + game launch 영향 큼.

### 4-4. 재검토 trigger

다음 조건 발생 시 R5 점수 재검토:
- Apple metal-shaderconverter 가 SM 6.8 work graph 지원 (현재 미지원) → D=2 → D=3 상승
- D3DMetal binary 가 macOS 15+ 에서 mesh shader / DXR 정식 GA → B=4 → B=5 상승
- 다른 community 출신 cross-compile 라이브러리 (SPIRV-Cross 등) own 1 면제 — 가능성 낮음
- 사용자 directive 로 Lineage 2 UE2.5 / DX9 path 우선순위 상승 → R5a / R5d secondary 확장
- D2R / D4 first frame 실 작동 검증 (R1/R2/R3 만점 후) → R5 의 B 축 5 점 검증 가능

---

*written 2026-05-01 cycle 79 직후. R5 D3D11/12 → Metal shader 만점 검토 — own 1 의 D3DMetal binary 명시로 ~85% Apple 위임 + ~15% DIY hexa fallback 구조. 19/25 sprint 후보. cycle 80-81 batch 12 모듈로 R5 신규 영역 cover. SCORING D1+R2+D2+D3+D5+R1 6 만점 자리 cross-ref.*
