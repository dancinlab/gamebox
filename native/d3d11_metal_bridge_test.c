/*
 * native/d3d11_metal_bridge_test.c — D3D11→Metal bridge native driver test
 *
 * F-NSWINDOW-E5 r11: Proves the D3D11-interface-shaped Metal bridge works by
 * calling it AS A D3D11 APPLICATION WOULD — through vtable function pointers
 * in C COM style: obj->lpVtbl->Method(obj, args...).
 *
 * own1: gamebox's own bridge; no Wine, no real d3d11.dll, no game asset.
 * validated_manjeom stays 0 — the bridge is proven by this native test, but not
 * yet driven through the i386 interpreter (that is r12).
 * Label: 구현됨·미배선 (dead-until-wired to PE via interpreter, r12 target).
 *
 * Test flow (mirrors a real D3D11 app startup):
 *   1. gamebox_D3D11CreateDeviceAndSwapChain(64, 64, &sc, &dev, &ctx)
 *   2. sc->lpVtbl->GetBuffer(sc, 0, NULL, &tex)          — get back buffer
 *   3. dev->lpVtbl->CreateRenderTargetView(dev, tex, NULL, &rtv) — wrap as RTV
 *   4. ctx->lpVtbl->OMSetRenderTargets(ctx, 1, &rtv, NULL)       — bind RTV
 *   5. ctx->lpVtbl->ClearRenderTargetView(ctx, rtv, blue)        — clear to blue
 *   6. ctx->lpVtbl->Draw(ctx, 3, 0)                              — red triangle
 *   7. sc->lpVtbl->Present(sc, 0, 0)                             — "present"
 *   8. gamebox_d3d11_readback(sc, pixels, size)                  — read pixels
 *   9. Pixel assertions: clear region = 0x0000FFFF (blue), triangle = 0xFF0000FF (red)
 *
 * Output protocol:
 *   __D3D11_BRIDGE__ SKIP reason=no_device               (exit 0 — no MTLDevice)
 *   __D3D11_BRIDGE__ PASS px_clear=<hex> px_triangle=<hex> nonuniform=1  (exit 0)
 *   __D3D11_BRIDGE__ FAIL reason=...                     (exit 1)
 *
 * Build + run:
 *   SDK="$(xcrun --sdk macosx --show-sdk-path)"
 *   clang -arch arm64 -isysroot "$SDK" -fobjc-arc -O2 -Wall -Wextra \
 *         -framework Metal -framework Foundation \
 *         -o /tmp/d3d11_bridge_test \
 *         native/d3d11_metal_bridge_test.c native/d3d11_metal_bridge.m \
 *   && /tmp/d3d11_bridge_test
 */

#include "d3d11_metal_bridge.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ── Pixel helper: BGRA8 buffer → 0xRRGGBBAA (same as metal_offscreen_smoke.m) ── */
static uint32_t pixel_at(const uint8_t *buf, int x, int y, int w) {
    const uint8_t *p = buf + ((size_t)y * (size_t)w + (size_t)x) * 4;
    uint8_t b = p[0], g = p[1], r = p[2], a = p[3];
    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

int main(void) {

    /* ── Step 1: CreateDeviceAndSwapChain ── */
    IDXGISwapChain     *sc  = NULL;
    ID3D11Device       *dev = NULL;
    ID3D11DeviceContext*dxc = NULL;

    HRESULT hr = gamebox_D3D11CreateDeviceAndSwapChain(
        GB_D3D_TEX_W, GB_D3D_TEX_H,
        &sc, &dev, &dxc);

    if (FAILED(hr)) {
        /*
         * E_FAIL from gamebox_D3D11CreateDeviceAndSwapChain means
         * MTLCreateSystemDefaultDevice() returned nil — no Metal GPU.
         * Honest ground-truth: SKIP, not a test failure.
         */
        printf("__D3D11_BRIDGE__ SKIP reason=no_device\n");
        fflush(stdout);
        return 0;
    }

    if (!sc || !dev || !dxc) {
        printf("__D3D11_BRIDGE__ FAIL reason=null_com_objects\n");
        fflush(stdout);
        return 1;
    }

    /* ── Step 2: GetBuffer(0) → ID3D11Texture2D back buffer ── */
    ID3D11Texture2D *tex = NULL;
    hr = sc->lpVtbl->GetBuffer(sc, 0, NULL, (void **)&tex);
    if (FAILED(hr) || !tex) {
        printf("__D3D11_BRIDGE__ FAIL reason=GetBuffer hr=0x%08X\n", (unsigned)hr);
        fflush(stdout);
        return 1;
    }

    /* ── Step 3: CreateRenderTargetView ── */
    ID3D11RenderTargetView *rtv = NULL;
    hr = dev->lpVtbl->CreateRenderTargetView(dev, tex, NULL, &rtv);
    if (FAILED(hr) || !rtv) {
        printf("__D3D11_BRIDGE__ FAIL reason=CreateRenderTargetView hr=0x%08X\n",
               (unsigned)hr);
        fflush(stdout);
        return 1;
    }

    /* ── Step 4: OMSetRenderTargets ── */
    dxc->lpVtbl->OMSetRenderTargets(dxc, 1, &rtv, NULL);

    /* ── Step 5: ClearRenderTargetView — blue (RGBA 0,0,1,1) ── */
    FLOAT blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    dxc->lpVtbl->ClearRenderTargetView(dxc, rtv, blue);

    /* ── Step 6: Draw — red triangle (3 vertices, start=0) ── */
    dxc->lpVtbl->Draw(dxc, 3, 0);

    /* ── Step 7: Present ── */
    hr = sc->lpVtbl->Present(sc, 0, 0);
    if (FAILED(hr)) {
        printf("__D3D11_BRIDGE__ FAIL reason=Present hr=0x%08X\n", (unsigned)hr);
        fflush(stdout);
        return 1;
    }

    /* ── Step 8: Readback ── */
    size_t   bufsize = (size_t)GB_D3D_TEX_W * (size_t)GB_D3D_TEX_H * 4;
    uint8_t *pixels  = (uint8_t *)malloc(bufsize);
    if (!pixels) {
        printf("__D3D11_BRIDGE__ FAIL reason=readback_malloc\n");
        fflush(stdout);
        return 1;
    }

    hr = gamebox_d3d11_readback(sc, pixels, bufsize);
    if (FAILED(hr)) {
        free(pixels);
        printf("__D3D11_BRIDGE__ FAIL reason=readback hr=0x%08X\n", (unsigned)hr);
        fflush(stdout);
        return 1;
    }

    /* ── Step 9: Pixel assertions ──
     *
     * Triangle vertices at NDC: (-0.5,0.5), (0.5,0.5), (-0.5,-0.5).
     * In 64×64 texture (Metal origin = top-left, Y-down):
     *   pixel_x = (ndc_x + 1) / 2 * 64
     *   pixel_y = (1 - ndc_y) / 2 * 64
     *
     * Safe triangle interior sample (same as metal_offscreen_smoke.m):
     *   tri_x = TEX_W/4 = 16,  tri_y = TEX_H/4 = 16
     *   (sits at / near the top-left vertex, within the filled region)
     *
     * Clear region (definitely outside triangle):
     *   clr_x = TEX_W-1 = 63, clr_y = TEX_H-1 = 63 (bottom-right corner)
     *
     * Expected:
     *   px_clear    = 0x0000FFFF  (blue:  BGRA8=[0xFF,0x00,0x00,0xFF] → RRGGBBAA)
     *   px_triangle = 0xFF0000FF  (red:   BGRA8=[0x00,0x00,0xFF,0xFF] → RRGGBBAA)
     *   nonuniform  = 1           (two distinct colors → real render happened)
     */
    int tri_x = GB_D3D_TEX_W / 4;   /* 16 — inside triangle */
    int tri_y = GB_D3D_TEX_H / 4;   /* 16 */
    int clr_x = GB_D3D_TEX_W - 1;   /* 63 — clear region */
    int clr_y = GB_D3D_TEX_H - 1;   /* 63 */

    uint32_t px_tri = pixel_at(pixels, tri_x, tri_y, GB_D3D_TEX_W);
    uint32_t px_clr = pixel_at(pixels, clr_x, clr_y, GB_D3D_TEX_W);
    free(pixels);

    int clr_ok    = (px_clr == 0x0000FFFFu);
    int tri_ok    = (px_tri == 0xFF0000FFu);
    int nonuniform = (px_tri != px_clr);

    if (!clr_ok || !tri_ok || !nonuniform) {
        printf("__D3D11_BRIDGE__ FAIL reason=pixel_assertion"
               " px_clear=0x%08X(expected=0x0000FFFF)"
               " px_triangle=0x%08X(expected=0xFF0000FF)"
               " nonuniform=%d\n",
               px_clr, px_tri, nonuniform);
        fflush(stdout);
        return 1;
    }

    /* ── Release COM objects (in reverse order) ── */
    rtv->lpVtbl->Release(rtv);
    tex->lpVtbl->Release(tex);
    dxc->lpVtbl->Release(dxc);
    dev->lpVtbl->Release(dev);
    sc->lpVtbl->Release(sc);

    printf("__D3D11_BRIDGE__ PASS"
           " px_clear=0x%08X px_triangle=0x%08X nonuniform=1\n",
           px_clr, px_tri);
    fflush(stdout);
    return 0;
}
