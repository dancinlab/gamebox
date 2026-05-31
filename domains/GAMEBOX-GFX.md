# GAMEBOX-GFX — current state
@title: 🎨 GAMEBOX-GFX — "그래픽 변환기"

@goal: DirectX → Metal translation: D3D11 + D3D12 command/resource/pipeline mapped to Metal, DXGI swapchain present, DXBC/DXIL→AIR shader cross-compile.

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [ ] D3D11 real path: buffer/texture/draw/state objects → Metal
- [ ] D3D12 real path: command queue/list, descriptor heap, fence, swapchain present
- [ ] DXGI factory + swapchain present + D3D→Metal router
- [ ] Shader xlate DXBC/DXIL → AIR + SV mapping
- [ ] Advanced: DXR raytracing, mesh shader, compute dispatch
