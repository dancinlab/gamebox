/*
 * native/d3d11_metal_bridge.h — D3D11-interface-shaped API routed to Apple Metal
 *
 * own1: gamebox's own D3D11 *interface shape* over Apple Metal (headless offscreen
 * MTLTexture). NO Wine, NO DXVK, NO d3d11.dll source copied. NO DRM, NO Warden,
 * NO anti-cheat bypass. This is a plain Metal backend authored for gamebox's own
 * Win32 game runtime (F-NSWINDOW-E5 r11).
 *
 * Status: 구현됨·미배선 (dead-until-wired)
 *   Proven by native/d3d11_metal_bridge_test.c (native driver test through vtable
 *   function pointers). NOT yet driven through the i386 interpreter — that is r12:
 *   r12 = minimal own1 D3D11 test PE (self-authored i386 PE, zero game asset) +
 *   interpreter IAT autobind wiring → this bridge → validated_manjeom>0 attempt.
 *
 * COM calling shape:
 *   Real D3D11 on Windows x86 uses __stdcall (callee-cleans-stack, right-to-left).
 *   On arm64 macOS (this host), all functions use the C calling convention.
 *   For r12, the interpreter IAT autobind shim boundary handles the stdcall→C
 *   translation at the import boundary.
 *   C COM usage pattern: obj->lpVtbl->Method(obj, arg1, arg2, ...)
 *   Identical to Win32 COM C usage (MSDN "Using COM Objects in C").
 *
 * Vtable layout — COMPACT (gamebox bridge, NOT real d3d11.dll offsets):
 *   Real d3d11.dll vtables have many methods between IUnknown (indices 0-2) and
 *   the methods implemented here. This bridge uses compact vtables: IUnknown stubs
 *   at [0],[1],[2] plus implemented methods consecutively.
 *   r12's self-authored test PE MUST use THESE compact indices (not real d3d11.dll
 *   offsets). If a future goal targets real commercial D3D11 games, the vtable must
 *   be expanded with NULL stub entries at unimplemented positions to match d3d11.dll.
 *
 *   ID3D11Device compact vtable:
 *     [0] QueryInterface       stub  → E_NOTIMPL
 *     [1] AddRef               stub  → 1
 *     [2] Release              impl  → frees object + releases shared context ref
 *     [3] CreateRenderTargetView  impl (real d3d11.dll index: 9)
 *
 *   ID3D11DeviceContext compact vtable:
 *     [0] QueryInterface       stub  → E_NOTIMPL
 *     [1] AddRef               stub  → 1
 *     [2] Release              impl
 *     [3] OMSetRenderTargets   impl  (real d3d11.dll index: 33)
 *     [4] ClearRenderTargetView  impl (real d3d11.dll index: 50)
 *     [5] Draw                 impl  (real d3d11.dll index: 13)
 *
 *   IDXGISwapChain compact vtable:
 *     [0] QueryInterface  stub → E_NOTIMPL
 *     [1] AddRef          stub → 1
 *     [2] Release         impl
 *     [3] GetBuffer       impl (real IDXGISwapChain index: 9)
 *     [4] Present         impl (real IDXGISwapChain index: 8)
 *
 *   ID3D11RenderTargetView compact vtable:
 *     [0] QueryInterface  stub → E_NOTIMPL
 *     [1] AddRef          stub → 1
 *     [2] Release         impl
 *
 *   ID3D11Texture2D compact vtable:
 *     [0] QueryInterface  stub → E_NOTIMPL
 *     [1] AddRef          stub → 1
 *     [2] Release         impl
 *
 * Texture geometry: GB_D3D_TEX_W × GB_D3D_TEX_H BGRA8Unorm offscreen MTLTexture.
 * Pixel format BGRA8 — in-memory order is [B, G, R, A] per pixel.
 * pixel_at helper (in test) converts to 0xRRGGBBAA for readability.
 */

#pragma once
#include <stdint.h>
#include <stddef.h>

/* ── Texture dimensions (match metal_offscreen_smoke.m) ── */
#define GB_D3D_TEX_W  64
#define GB_D3D_TEX_H  64

/* ── Win32 / COM basic types (gamebox-local — no windows.h dependency) ── */
typedef int32_t   HRESULT;
typedef uint32_t  UINT;
typedef uint32_t  DWORD;
typedef float     FLOAT;

#define S_OK          ((HRESULT)0)
#define E_FAIL        ((HRESULT)(int32_t)0x80004005)
#define E_NOTIMPL     ((HRESULT)(int32_t)0x80004001)
#define E_OUTOFMEMORY ((HRESULT)(int32_t)0x8007000E)

#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#define FAILED(hr)    ((HRESULT)(hr) < 0)

/* ── Forward declarations for all COM objects ── */
typedef struct ID3D11Device           ID3D11Device;
typedef struct ID3D11DeviceContext    ID3D11DeviceContext;
typedef struct IDXGISwapChain         IDXGISwapChain;
typedef struct ID3D11RenderTargetView ID3D11RenderTargetView;
typedef struct ID3D11Texture2D        ID3D11Texture2D;

/* ══════════════════════════════════════════════════════════════
   Vtable type definitions — one per COM interface
   ══════════════════════════════════════════════════════════════ */

/* ── ID3D11Device vtable ── */
typedef struct ID3D11DeviceVtbl {
    /* [0] IUnknown::QueryInterface — stub */
    HRESULT (*QueryInterface)(ID3D11Device *self, void *riid, void **ppv);
    /* [1] IUnknown::AddRef — stub (returns 1) */
    UINT    (*AddRef)        (ID3D11Device *self);
    /* [2] IUnknown::Release — implemented (frees object) */
    UINT    (*Release)       (ID3D11Device *self);
    /* [3] ID3D11Device::CreateRenderTargetView
     *     pDesc: NULL → full-resource view (only supported value here)
     *     Real d3d11.dll vtable index: 9 */
    HRESULT (*CreateRenderTargetView)(
        ID3D11Device           *self,
        ID3D11Texture2D        *pResource,
        void                   *pDesc,
        ID3D11RenderTargetView **ppRTView
    );
} ID3D11DeviceVtbl;

struct ID3D11Device {
    const ID3D11DeviceVtbl *lpVtbl;
    void                   *_impl;  /* opaque — bridge-retained GBMetalContext* */
};

/* ── ID3D11DeviceContext vtable ── */
typedef struct ID3D11DeviceContextVtbl {
    /* [0] stub */ HRESULT (*QueryInterface)(ID3D11DeviceContext *self, void *riid, void **ppv);
    /* [1] stub */ UINT    (*AddRef)        (ID3D11DeviceContext *self);
    /* [2] impl */ UINT    (*Release)       (ID3D11DeviceContext *self);
    /* [3] ID3D11DeviceContext::OMSetRenderTargets
     *     pDepthStencilView: ignored (no depth in this bridge)
     *     Real index: 33 */
    void (*OMSetRenderTargets)(
        ID3D11DeviceContext     *self,
        UINT                     NumViews,
        ID3D11RenderTargetView  *const *ppRenderTargetViews,
        void                    *pDepthStencilView
    );
    /* [4] ID3D11DeviceContext::ClearRenderTargetView
     *     ColorRGBA: float[4] in RGBA order (same as real D3D11)
     *     Real index: 50 */
    void (*ClearRenderTargetView)(
        ID3D11DeviceContext    *self,
        ID3D11RenderTargetView *pRenderTargetView,
        const FLOAT             ColorRGBA[4]
    );
    /* [5] ID3D11DeviceContext::Draw
     *     Draws VertexCount vertices starting at StartVertexLocation.
     *     Triangle color is bridge-fixed to red (1,0,0,1) — r12 wiring will
     *     add constant-buffer support for arbitrary colors.
     *     Real index: 13 */
    void (*Draw)(
        ID3D11DeviceContext *self,
        UINT                 VertexCount,
        UINT                 StartVertexLocation
    );
} ID3D11DeviceContextVtbl;

struct ID3D11DeviceContext {
    const ID3D11DeviceContextVtbl *lpVtbl;
    void                          *_impl;  /* bridge-retained GBMetalContext* */
};

/* ── IDXGISwapChain vtable ── */
typedef struct IDXGISwapChainVtbl {
    /* [0] stub */ HRESULT (*QueryInterface)(IDXGISwapChain *self, void *riid, void **ppv);
    /* [1] stub */ UINT    (*AddRef)        (IDXGISwapChain *self);
    /* [2] impl */ UINT    (*Release)       (IDXGISwapChain *self);
    /* [3] IDXGISwapChain::GetBuffer
     *     Buffer=0 → back buffer (only supported value)
     *     riid: ignored (always returns ID3D11Texture2D shape)
     *     Real IDXGISwapChain index: 9 */
    HRESULT (*GetBuffer)(
        IDXGISwapChain *self,
        UINT            Buffer,
        void           *riid,
        void          **ppSurface
    );
    /* [4] IDXGISwapChain::Present
     *     Offscreen: finalizes pending render; no display flip.
     *     SyncInterval, Flags: ignored in this bridge (headless).
     *     Real IDXGISwapChain index: 8 */
    HRESULT (*Present)(
        IDXGISwapChain *self,
        UINT            SyncInterval,
        UINT            Flags
    );
} IDXGISwapChainVtbl;

struct IDXGISwapChain {
    const IDXGISwapChainVtbl *lpVtbl;
    void                     *_impl;  /* bridge-retained GBMetalContext* */
};

/* ── ID3D11RenderTargetView vtable (no methods beyond IUnknown) ── */
typedef struct ID3D11RenderTargetViewVtbl {
    /* [0] stub */ HRESULT (*QueryInterface)(ID3D11RenderTargetView *self, void *riid, void **ppv);
    /* [1] stub */ UINT    (*AddRef)        (ID3D11RenderTargetView *self);
    /* [2] impl */ UINT    (*Release)       (ID3D11RenderTargetView *self);
} ID3D11RenderTargetViewVtbl;

struct ID3D11RenderTargetView {
    const ID3D11RenderTargetViewVtbl *lpVtbl;
    void                             *_impl;  /* bridge-retained GBMetalContext* */
};

/* ── ID3D11Texture2D vtable (no methods beyond IUnknown) ── */
typedef struct ID3D11Texture2DVtbl {
    /* [0] stub */ HRESULT (*QueryInterface)(ID3D11Texture2D *self, void *riid, void **ppv);
    /* [1] stub */ UINT    (*AddRef)        (ID3D11Texture2D *self);
    /* [2] impl */ UINT    (*Release)       (ID3D11Texture2D *self);
} ID3D11Texture2DVtbl;

struct ID3D11Texture2D {
    const ID3D11Texture2DVtbl *lpVtbl;
    void                      *_impl;  /* bridge-retained GBMetalContext* */
};

/* ══════════════════════════════════════════════════════════════
   Bridge entry points (implemented in d3d11_metal_bridge.m)
   ══════════════════════════════════════════════════════════════ */

/*
 * gamebox_D3D11CreateDeviceAndSwapChain
 *
 * Simplified equivalent of the real Win32 D3D11CreateDeviceAndSwapChain (12 params):
 *
 *   HRESULT D3D11CreateDeviceAndSwapChain(
 *     IDXGIAdapter*,              [ignored] select GPU adapter
 *     D3D_DRIVER_TYPE,            [ignored] hardware / reference / null
 *     HMODULE,                    [ignored] software rasterizer module
 *     UINT,                       [ignored] device creation flags
 *     const D3D_FEATURE_LEVEL*,   [ignored] requested feature levels
 *     UINT,                       [ignored] count of feature levels
 *     UINT,                       [ignored] D3D11_SDK_VERSION
 *     const DXGI_SWAP_CHAIN_DESC*,[simplified to width+height]
 *     IDXGISwapChain**,           out → ppSwapChain
 *     ID3D11Device**,             out → ppDevice
 *     D3D_FEATURE_LEVEL*,         [ignored] out: selected feature level
 *     ID3D11DeviceContext**       out → ppImmediateContext
 *   );
 *
 * Bridge simplification: the first 7 params + DXGI_SWAP_CHAIN_DESC are collapsed
 * to (width, height). The three output pointers have identical semantics.
 * Returns S_OK on success; E_FAIL if MTLCreateSystemDefaultDevice() returns nil
 * (no GPU — same as the SKIP path in metal_offscreen_smoke.m).
 */
HRESULT gamebox_D3D11CreateDeviceAndSwapChain(
    UINT                  width,
    UINT                  height,
    IDXGISwapChain      **ppSwapChain,
    ID3D11Device        **ppDevice,
    ID3D11DeviceContext **ppImmediateContext
);

/*
 * gamebox_d3d11_readback
 *
 * Non-D3D11 diagnostic helper: reads back the swapchain's offscreen back-buffer
 * pixels into out_pixels (caller-allocated). Format: BGRA8 (4 bytes/pixel),
 * row-major, width×height pixels. size must be >= GB_D3D_TEX_W*GB_D3D_TEX_H*4.
 *
 * Converts pixel (x,y) to 0xRRGGBBAA like pixel_at in metal_offscreen_smoke.m:
 * bytes = [B,G,R,A] → return (R<<24)|(G<<16)|(B<<8)|A.
 * (The caller reads raw bytes; this function just fills the buffer.)
 * Returns S_OK on success.
 */
HRESULT gamebox_d3d11_readback(
    IDXGISwapChain *swapchain,
    uint8_t        *out_pixels,
    size_t          size
);
