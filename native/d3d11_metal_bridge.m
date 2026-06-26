/*
 * native/d3d11_metal_bridge.m — D3D11-interface-shaped API over Apple Metal
 *
 * own1: gamebox's own D3D11 *interface shape* routed to Apple Metal headless
 * offscreen rendering. NO Wine, NO DXVK, NO d3d11.dll source. NO DRM, NO Warden,
 * NO anti-cheat bypass. Plain Metal backend; gamebox-authored code only.
 *
 * F-NSWINDOW-E5 r11 — 구현됨·미배선 (dead-until-wired to PE via interpreter).
 *
 * Architecture:
 *   GBMetalContext (ObjC, ARC-managed) holds all Metal resources: device, queue,
 *   library, pipeline state, back-buffer texture, triangle color buffer.
 *   Each COM object (ID3D11Device, ID3D11DeviceContext, IDXGISwapChain,
 *   ID3D11RenderTargetView, ID3D11Texture2D) is a heap-allocated C struct with
 *   a leading vtable pointer (lpVtbl) + an _impl void* that holds a
 *   CFBridgingRetain reference to the shared GBMetalContext.
 *   COM Release() calls CFBridgingRelease(_impl) + free(self) — when all COM
 *   objects are released the ARC retain count on GBMetalContext drops to 0 and
 *   it deallocs naturally.
 *
 * Render model (offscreen, headless):
 *   ClearRenderTargetView  → Metal render pass: loadAction=Clear, 0 draws, commit+wait
 *   Draw(n, start)         → Metal render pass: loadAction=Load, draw n vertices, commit+wait
 *   Present                → no-op (headless; content already in MTLTexture)
 *   gamebox_d3d11_readback → [MTLTexture getBytes:...] into caller buffer
 *
 * Triangle color: red (1,0,0,1) hardcoded in triColorBuf.
 * Clear color: supplied by caller to ClearRenderTargetView (RGBA float[4]).
 *
 * Build:
 *   SDK="$(xcrun --sdk macosx --show-sdk-path)"
 *   clang -arch arm64 -isysroot "$SDK" -fobjc-arc -O2 -Wall -Wextra \
 *         -framework Metal -framework Foundation \
 *         -o /tmp/d3d11_bridge_test \
 *         native/d3d11_metal_bridge_test.c native/d3d11_metal_bridge.m
 */

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#include "d3d11_metal_bridge.h"
#include <stdlib.h>
#include <string.h>

/* ── Inline MSL (identical geometry to metal_offscreen_smoke.m) ── */
static NSString * const kShaderSrc = @
"#include <metal_stdlib>\n"
"using namespace metal;\n"
"\n"
"struct VertexOut {\n"
"    float4 position [[position]];\n"
"    float4 color;\n"
"};\n"
"\n"
"vertex VertexOut vtx_main(\n"
"    uint vid [[vertex_id]],\n"
"    constant float4 *colors [[buffer(1)]]\n"
") {\n"
"    /* unit-NDC triangle covering upper-left quadrant — same as r10 smoke */\n"
"    const float2 pos[3] = {\n"
"        float2(-0.5f,  0.5f),\n"
"        float2( 0.5f,  0.5f),\n"
"        float2(-0.5f, -0.5f)\n"
"    };\n"
"    VertexOut o;\n"
"    o.position = float4(pos[vid], 0.0f, 1.0f);\n"
"    o.color    = colors[0];\n"
"    return o;\n"
"}\n"
"\n"
"fragment float4 frag_main(VertexOut in [[stage_in]]) {\n"
"    return in.color;\n"
"}\n";

/* ════════════════════════════════════════════════════════════════
   GBMetalContext — shared Metal resource container (ARC-managed)
   ════════════════════════════════════════════════════════════════ */

@interface GBMetalContext : NSObject
@property (nonatomic, strong) id<MTLDevice>              device;
@property (nonatomic, strong) id<MTLCommandQueue>        queue;
@property (nonatomic, strong) id<MTLLibrary>             lib;
@property (nonatomic, strong) id<MTLRenderPipelineState> pso;
@property (nonatomic, strong) id<MTLTexture>             backBuffer;  /* offscreen swapchain */
@property (nonatomic, strong) id<MTLBuffer>              triColorBuf; /* red (1,0,0,1) */
@property (nonatomic, assign) NSUInteger                 width;
@property (nonatomic, assign) NSUInteger                 height;
@end

@implementation GBMetalContext
@end

/* ── Helper: extract GBMetalContext from _impl void* ── */
static inline GBMetalContext *get_ctx(void *impl) {
    return (__bridge GBMetalContext *)impl;
}

/* ════════════════════════════════════════════════════════════════
   Forward declarations for vtable function pointers
   (allows static vtable structs to reference function addresses
   before their full definitions appear in the file)
   ════════════════════════════════════════════════════════════════ */

/* ID3D11Texture2D */
static HRESULT tex2d_QueryInterface(ID3D11Texture2D *self, void *riid, void **ppv);
static UINT    tex2d_AddRef        (ID3D11Texture2D *self);
static UINT    tex2d_Release       (ID3D11Texture2D *self);

/* ID3D11RenderTargetView */
static HRESULT rtv_QueryInterface(ID3D11RenderTargetView *self, void *riid, void **ppv);
static UINT    rtv_AddRef        (ID3D11RenderTargetView *self);
static UINT    rtv_Release       (ID3D11RenderTargetView *self);

/* ID3D11DeviceContext */
static HRESULT ctx_QueryInterface    (ID3D11DeviceContext *self, void *riid, void **ppv);
static UINT    ctx_AddRef            (ID3D11DeviceContext *self);
static UINT    ctx_Release           (ID3D11DeviceContext *self);
static void    ctx_OMSetRenderTargets(ID3D11DeviceContext *self, UINT n,
                                      ID3D11RenderTargetView *const *views, void *dsv);
static void    ctx_ClearRenderTargetView(ID3D11DeviceContext *self,
                                         ID3D11RenderTargetView *rtv,
                                         const FLOAT color[4]);
static void    ctx_Draw              (ID3D11DeviceContext *self, UINT vc, UINT start);

/* IDXGISwapChain */
static HRESULT sc_QueryInterface(IDXGISwapChain *self, void *riid, void **ppv);
static UINT    sc_AddRef        (IDXGISwapChain *self);
static UINT    sc_Release       (IDXGISwapChain *self);
static HRESULT sc_GetBuffer     (IDXGISwapChain *self, UINT buf, void *riid, void **ppSurf);
static HRESULT sc_Present       (IDXGISwapChain *self, UINT sync, UINT flags);

/* ID3D11Device */
static HRESULT dev_QueryInterface       (ID3D11Device *self, void *riid, void **ppv);
static UINT    dev_AddRef               (ID3D11Device *self);
static UINT    dev_Release              (ID3D11Device *self);
static HRESULT dev_CreateRenderTargetView(ID3D11Device *self, ID3D11Texture2D *res,
                                          void *desc, ID3D11RenderTargetView **ppRTV);

/* ════════════════════════════════════════════════════════════════
   Static vtable instances (one per COM interface)
   ════════════════════════════════════════════════════════════════ */

static const ID3D11Texture2DVtbl s_tex2dVtbl = {
    .QueryInterface = tex2d_QueryInterface,
    .AddRef         = tex2d_AddRef,
    .Release        = tex2d_Release,
};

static const ID3D11RenderTargetViewVtbl s_rtvVtbl = {
    .QueryInterface = rtv_QueryInterface,
    .AddRef         = rtv_AddRef,
    .Release        = rtv_Release,
};

static const ID3D11DeviceContextVtbl s_ctxVtbl = {
    .QueryInterface       = ctx_QueryInterface,
    .AddRef               = ctx_AddRef,
    .Release              = ctx_Release,
    .OMSetRenderTargets   = ctx_OMSetRenderTargets,
    .ClearRenderTargetView = ctx_ClearRenderTargetView,
    .Draw                 = ctx_Draw,
};

static const IDXGISwapChainVtbl s_scVtbl = {
    .QueryInterface = sc_QueryInterface,
    .AddRef         = sc_AddRef,
    .Release        = sc_Release,
    .GetBuffer      = sc_GetBuffer,
    .Present        = sc_Present,
};

static const ID3D11DeviceVtbl s_devVtbl = {
    .QueryInterface         = dev_QueryInterface,
    .AddRef                 = dev_AddRef,
    .Release                = dev_Release,
    .CreateRenderTargetView = dev_CreateRenderTargetView,
};

/* ════════════════════════════════════════════════════════════════
   ID3D11Texture2D vtable implementations
   ════════════════════════════════════════════════════════════════ */

static HRESULT tex2d_QueryInterface(ID3D11Texture2D *self, void *riid, void **ppv) {
    (void)self; (void)riid; (void)ppv;
    return E_NOTIMPL;
}
static UINT tex2d_AddRef(ID3D11Texture2D *self) { (void)self; return 1; }
static UINT tex2d_Release(ID3D11Texture2D *self) {
    if (!self) return 0;
    CFBridgingRelease(self->_impl);
    free(self);
    return 0;
}

/* ════════════════════════════════════════════════════════════════
   ID3D11RenderTargetView vtable implementations
   ════════════════════════════════════════════════════════════════ */

static HRESULT rtv_QueryInterface(ID3D11RenderTargetView *self, void *riid, void **ppv) {
    (void)self; (void)riid; (void)ppv;
    return E_NOTIMPL;
}
static UINT rtv_AddRef(ID3D11RenderTargetView *self) { (void)self; return 1; }
static UINT rtv_Release(ID3D11RenderTargetView *self) {
    if (!self) return 0;
    CFBridgingRelease(self->_impl);
    free(self);
    return 0;
}

/* ════════════════════════════════════════════════════════════════
   ID3D11DeviceContext vtable implementations
   ════════════════════════════════════════════════════════════════ */

static HRESULT ctx_QueryInterface(ID3D11DeviceContext *self, void *riid, void **ppv) {
    (void)self; (void)riid; (void)ppv;
    return E_NOTIMPL;
}
static UINT ctx_AddRef(ID3D11DeviceContext *self) { (void)self; return 1; }
static UINT ctx_Release(ID3D11DeviceContext *self) {
    if (!self) return 0;
    CFBridgingRelease(self->_impl);
    free(self);
    return 0;
}

static void ctx_OMSetRenderTargets(
    ID3D11DeviceContext    *self,
    UINT                    NumViews,
    ID3D11RenderTargetView *const *ppViews,
    void                   *pDepthStencilView)
{
    /*
     * In this bridge there is exactly one render target: the swapchain back buffer.
     * OMSetRenderTargets is a no-op here — ClearRenderTargetView and Draw always
     * use GBMetalContext.backBuffer. The pRenderTargetView passed to
     * ClearRenderTargetView is also ignored (backBuffer is used directly).
     * r12 note: if multiple render targets are ever needed, store the selected
     * texture pointer in GBMetalContext and use it in the render passes.
     */
    (void)self; (void)NumViews; (void)ppViews; (void)pDepthStencilView;
}

static void ctx_ClearRenderTargetView(
    ID3D11DeviceContext    *self,
    ID3D11RenderTargetView *pRTV,
    const FLOAT             ColorRGBA[4])
{
    /*
     * Clear the back buffer with ColorRGBA (RGBA float[4], same order as real D3D11).
     * Executed as a standalone render pass: loadAction=Clear, no draw commands,
     * storeAction=Store, commit + waitUntilCompleted.
     */
    (void)pRTV;  /* always uses ctx.backBuffer */
    if (!self) return;
    GBMetalContext *ctx = get_ctx(self->_impl);
    if (!ctx) return;

    @autoreleasepool {
        MTLRenderPassDescriptor *rpDesc = [MTLRenderPassDescriptor new];
        rpDesc.colorAttachments[0].texture     = ctx.backBuffer;
        rpDesc.colorAttachments[0].loadAction  = MTLLoadActionClear;
        rpDesc.colorAttachments[0].clearColor  =
            MTLClearColorMake(ColorRGBA[0], ColorRGBA[1], ColorRGBA[2], ColorRGBA[3]);
        rpDesc.colorAttachments[0].storeAction = MTLStoreActionStore;

        id<MTLCommandBuffer> cmd = [ctx.queue commandBuffer];
        id<MTLRenderCommandEncoder> enc = [cmd renderCommandEncoderWithDescriptor:rpDesc];
        /* No draw commands — clear only */
        [enc endEncoding];
        [cmd commit];
        [cmd waitUntilCompleted];
    }
}

static void ctx_Draw(
    ID3D11DeviceContext *self,
    UINT                 VertexCount,
    UINT                 StartVertexLocation)
{
    /*
     * Draw VertexCount vertices (starting at StartVertexLocation) using the
     * bridge's red-triangle pipeline. loadAction=Load preserves the prior clear.
     * Triangle color is hardcoded red (1,0,0,1) via ctx.triColorBuf.
     * r12 note: to support arbitrary colors, add a constant-buffer argument and
     * update triColorBuf before the render pass.
     */
    if (!self) return;
    GBMetalContext *ctx = get_ctx(self->_impl);
    if (!ctx) return;

    @autoreleasepool {
        MTLRenderPassDescriptor *rpDesc = [MTLRenderPassDescriptor new];
        rpDesc.colorAttachments[0].texture     = ctx.backBuffer;
        rpDesc.colorAttachments[0].loadAction  = MTLLoadActionLoad;  /* preserve clear */
        rpDesc.colorAttachments[0].storeAction = MTLStoreActionStore;

        id<MTLCommandBuffer> cmd = [ctx.queue commandBuffer];
        id<MTLRenderCommandEncoder> enc = [cmd renderCommandEncoderWithDescriptor:rpDesc];
        [enc setRenderPipelineState:ctx.pso];
        [enc setVertexBuffer:ctx.triColorBuf offset:0 atIndex:1];
        [enc drawPrimitives:MTLPrimitiveTypeTriangle
               vertexStart:StartVertexLocation
               vertexCount:VertexCount];
        [enc endEncoding];
        [cmd commit];
        [cmd waitUntilCompleted];
    }
}

/* ════════════════════════════════════════════════════════════════
   IDXGISwapChain vtable implementations
   ════════════════════════════════════════════════════════════════ */

static HRESULT sc_QueryInterface(IDXGISwapChain *self, void *riid, void **ppv) {
    (void)self; (void)riid; (void)ppv;
    return E_NOTIMPL;
}
static UINT sc_AddRef(IDXGISwapChain *self) { (void)self; return 1; }
static UINT sc_Release(IDXGISwapChain *self) {
    if (!self) return 0;
    CFBridgingRelease(self->_impl);
    free(self);
    return 0;
}

static HRESULT sc_GetBuffer(
    IDXGISwapChain *self,
    UINT            Buffer,
    void           *riid,
    void          **ppSurface)
{
    /*
     * Returns the back-buffer surface as an ID3D11Texture2D.
     * Buffer must be 0 (only one back buffer in this bridge).
     * riid is ignored (always returns our ID3D11Texture2D shape).
     */
    (void)riid;
    if (!self || !ppSurface) return E_FAIL;
    if (Buffer != 0) return E_FAIL;

    GBMetalContext *ctx = get_ctx(self->_impl);

    ID3D11Texture2D *tex = (ID3D11Texture2D *)calloc(1, sizeof(ID3D11Texture2D));
    if (!tex) return E_OUTOFMEMORY;

    tex->lpVtbl = &s_tex2dVtbl;
    tex->_impl  = (void *)CFBridgingRetain(ctx);
    *ppSurface  = tex;
    return S_OK;
}

static HRESULT sc_Present(IDXGISwapChain *self, UINT SyncInterval, UINT Flags) {
    /*
     * Headless bridge — the back buffer is an offscreen MTLTexture; there is no
     * display connection. Present is a no-op: the render commands were already
     * committed + waited in ClearRenderTargetView / Draw. Pixels are ready for
     * gamebox_d3d11_readback immediately after Present returns.
     */
    (void)self; (void)SyncInterval; (void)Flags;
    return S_OK;
}

/* ════════════════════════════════════════════════════════════════
   ID3D11Device vtable implementations
   ════════════════════════════════════════════════════════════════ */

static HRESULT dev_QueryInterface(ID3D11Device *self, void *riid, void **ppv) {
    (void)self; (void)riid; (void)ppv;
    return E_NOTIMPL;
}
static UINT dev_AddRef(ID3D11Device *self) { (void)self; return 1; }
static UINT dev_Release(ID3D11Device *self) {
    if (!self) return 0;
    CFBridgingRelease(self->_impl);
    free(self);
    return 0;
}

static HRESULT dev_CreateRenderTargetView(
    ID3D11Device           *self,
    ID3D11Texture2D        *pResource,
    void                   *pDesc,
    ID3D11RenderTargetView **ppRTV)
{
    /*
     * Creates an ID3D11RenderTargetView that wraps the texture.
     * pDesc=NULL → full-resource view (only supported value in this bridge).
     * The RTV shares the GBMetalContext with all other COM objects;
     * ClearRenderTargetView uses ctx.backBuffer regardless of which texture
     * pResource wraps (they always refer to the same back buffer in this bridge).
     */
    (void)pResource; (void)pDesc;
    if (!self || !ppRTV) return E_FAIL;

    GBMetalContext *ctx = get_ctx(self->_impl);

    ID3D11RenderTargetView *rtv =
        (ID3D11RenderTargetView *)calloc(1, sizeof(ID3D11RenderTargetView));
    if (!rtv) return E_OUTOFMEMORY;

    rtv->lpVtbl = &s_rtvVtbl;
    rtv->_impl  = (void *)CFBridgingRetain(ctx);
    *ppRTV      = rtv;
    return S_OK;
}

/* ════════════════════════════════════════════════════════════════
   Bridge entry: gamebox_D3D11CreateDeviceAndSwapChain
   ════════════════════════════════════════════════════════════════ */

HRESULT gamebox_D3D11CreateDeviceAndSwapChain(
    UINT                  width,
    UINT                  height,
    IDXGISwapChain      **ppSwapChain,
    ID3D11Device        **ppDevice,
    ID3D11DeviceContext **ppImmediateContext)
{
    if (!ppSwapChain || !ppDevice || !ppImmediateContext) return E_FAIL;
    *ppSwapChain        = NULL;
    *ppDevice           = NULL;
    *ppImmediateContext = NULL;

    @autoreleasepool {

        /* ── 1. MTLDevice ── */
        id<MTLDevice> mtlDev = MTLCreateSystemDefaultDevice();
        if (!mtlDev) {
            return E_FAIL;  /* caller interprets as SKIP (no GPU) */
        }

        /* ── 2. Shared Metal context ── */
        GBMetalContext *ctx = [[GBMetalContext alloc] init];
        ctx.device = mtlDev;
        ctx.width  = width;
        ctx.height = height;

        /* ── 3. Command queue ── */
        ctx.queue = [mtlDev newCommandQueue];
        if (!ctx.queue) return E_FAIL;

        /* ── 4. Offscreen back-buffer texture (BGRA8, RenderTarget+ShaderRead, Shared) ── */
        MTLTextureDescriptor *td = [MTLTextureDescriptor
            texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm
            width:width
            height:height
            mipmapped:NO];
        td.usage       = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
        td.storageMode = MTLStorageModeShared;  /* CPU-accessible without blit */
        ctx.backBuffer = [mtlDev newTextureWithDescriptor:td];
        if (!ctx.backBuffer) return E_FAIL;

        /* ── 5. Compile inline MSL library ── */
        NSError *err = nil;
        ctx.lib = [mtlDev newLibraryWithSource:kShaderSrc options:nil error:&err];
        if (!ctx.lib) return E_FAIL;

        id<MTLFunction> vtxFn  = [ctx.lib newFunctionWithName:@"vtx_main"];
        id<MTLFunction> fragFn = [ctx.lib newFunctionWithName:@"frag_main"];
        if (!vtxFn || !fragFn) return E_FAIL;

        /* ── 6. Render pipeline state ── */
        MTLRenderPipelineDescriptor *rpd = [MTLRenderPipelineDescriptor new];
        rpd.vertexFunction                  = vtxFn;
        rpd.fragmentFunction                = fragFn;
        rpd.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;

        ctx.pso = [mtlDev newRenderPipelineStateWithDescriptor:rpd error:&err];
        if (!ctx.pso) return E_FAIL;

        /* ── 7. Triangle color buffer: red (1,0,0,1) ── */
        float red[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        ctx.triColorBuf = [mtlDev newBufferWithBytes:red
                                              length:sizeof(red)
                                             options:MTLResourceStorageModeShared];
        if (!ctx.triColorBuf) return E_FAIL;

        /* ── 8. Allocate COM object shells ── */
        IDXGISwapChain     *sc  = (IDXGISwapChain *)    calloc(1, sizeof(IDXGISwapChain));
        ID3D11Device       *dev = (ID3D11Device *)       calloc(1, sizeof(ID3D11Device));
        ID3D11DeviceContext*dxc = (ID3D11DeviceContext *)calloc(1, sizeof(ID3D11DeviceContext));
        if (!sc || !dev || !dxc) {
            free(sc); free(dev); free(dxc);
            return E_OUTOFMEMORY;
        }

        /* ── 9. Wire vtables + bridge-retain the shared context into each COM object ── */
        sc->lpVtbl  = &s_scVtbl;   sc->_impl  = (void *)CFBridgingRetain(ctx);
        dev->lpVtbl = &s_devVtbl;  dev->_impl = (void *)CFBridgingRetain(ctx);
        dxc->lpVtbl = &s_ctxVtbl;  dxc->_impl = (void *)CFBridgingRetain(ctx);

        *ppSwapChain        = sc;
        *ppDevice           = dev;
        *ppImmediateContext = dxc;
        return S_OK;

    } /* @autoreleasepool — ctx local ARC ref drops here; bridge retains keep it alive */
}

/* ════════════════════════════════════════════════════════════════
   Diagnostic helper: gamebox_d3d11_readback
   ════════════════════════════════════════════════════════════════ */

HRESULT gamebox_d3d11_readback(
    IDXGISwapChain *swapchain,
    uint8_t        *out_pixels,
    size_t          size)
{
    if (!swapchain || !out_pixels) return E_FAIL;
    GBMetalContext *ctx = get_ctx(swapchain->_impl);
    if (!ctx || !ctx.backBuffer) return E_FAIL;

    size_t needed = (size_t)ctx.width * (size_t)ctx.height * 4;
    if (size < needed) return E_FAIL;

    @autoreleasepool {
        [ctx.backBuffer getBytes:out_pixels
                    bytesPerRow:(size_t)ctx.width * 4
                     fromRegion:MTLRegionMake2D(0, 0, ctx.width, ctx.height)
                    mipmapLevel:0];
    }
    return S_OK;
}
