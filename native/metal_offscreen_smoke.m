/*
 * native/metal_offscreen_smoke.m — headless Metal offscreen render + readback smoke
 *
 * F-NSWINDOW-E5 r10: E5 substrate proof (NOT a game frame; validated_manjeom stays 0).
 *
 * PURPOSE: empirically verify that headless (no display) Metal offscreen render +
 * pixel readback works on this machine and on the Blacksmith CI runner. Converts an
 * asserted claim ("③ display not needed, Metal renders headless") into an executed,
 * CI-gated fact — break-walls + verify-done discipline.
 *
 * own1: plain Apple Metal (the project's own render target). No Wine, no DRM, no game
 * asset. This is a gamebox-authored triangle, NOT a game frame driven through the
 * interpreter. validated_manjeom remains 0.
 *
 * Build (arm64 macOS):
 *   SDK="$(xcrun --sdk macosx --show-sdk-path)"
 *   clang -arch arm64 -isysroot "$SDK" -fobjc-arc -O2 -Wall -Wextra \
 *         -framework Metal -framework Foundation \
 *         -o /tmp/metal_offscreen_smoke native/metal_offscreen_smoke.m
 *
 * Output protocol:
 *   __METAL_SMOKE__ SKIP reason=no_device                       (exit 0 — no GPU)
 *   __METAL_SMOKE__ PASS device=<name> px_clear=<hex> px_triangle=<hex> nonuniform=1
 *   __METAL_SMOKE__ FAIL reason=...                             (exit 1)
 */

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import <simd/simd.h>

/* ── geometry ── */
#define TEX_W 64
#define TEX_H 64

/* ── inline MSL source ── */
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
"    /* unit-NDC triangle covering upper-left quadrant */\n"
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

/* ── helpers ── */
static uint32_t pixel_at(const uint8_t *buf, int x, int y) {
    /* BGRA8 → return as 0xRRGGBBAA for readability */
    const uint8_t *p = buf + (y * TEX_W + x) * 4;
    uint8_t b = p[0], g = p[1], r = p[2], a = p[3];
    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

int main(void) {
    @autoreleasepool {

        /* ── 1. device ── */
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        if (!device) {
            printf("__METAL_SMOKE__ SKIP reason=no_device\n");
            fflush(stdout);
            return 0;
        }

        /* ── 2. offscreen texture (64×64 BGRA8, RenderTarget+ShaderRead) ── */
        MTLTextureDescriptor *td = [MTLTextureDescriptor
            texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm
            width:TEX_W
            height:TEX_H
            mipmapped:NO];
        td.usage         = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
        td.storageMode   = MTLStorageModeShared;   /* CPU-readable without blit */

        id<MTLTexture> tex = [device newTextureWithDescriptor:td];
        if (!tex) {
            printf("__METAL_SMOKE__ FAIL reason=texture_alloc\n");
            fflush(stdout);
            return 1;
        }

        /* ── 3. compile inline MSL library ── */
        NSError *err = nil;
        MTLCompileOptions *opts = [MTLCompileOptions new];
        id<MTLLibrary> lib = [device newLibraryWithSource:kShaderSrc
                                                  options:opts
                                                    error:&err];
        if (!lib) {
            printf("__METAL_SMOKE__ FAIL reason=shader_compile err=%s\n",
                   err ? err.localizedDescription.UTF8String : "unknown");
            fflush(stdout);
            return 1;
        }

        id<MTLFunction> vtxFn  = [lib newFunctionWithName:@"vtx_main"];
        id<MTLFunction> fragFn = [lib newFunctionWithName:@"frag_main"];
        if (!vtxFn || !fragFn) {
            printf("__METAL_SMOKE__ FAIL reason=function_lookup\n");
            fflush(stdout);
            return 1;
        }

        /* ── 4. render pipeline ── */
        MTLRenderPipelineDescriptor *rpd = [MTLRenderPipelineDescriptor new];
        rpd.vertexFunction                            = vtxFn;
        rpd.fragmentFunction                          = fragFn;
        rpd.colorAttachments[0].pixelFormat           = MTLPixelFormatBGRA8Unorm;

        id<MTLRenderPipelineState> pso = [device newRenderPipelineStateWithDescriptor:rpd
                                                                                error:&err];
        if (!pso) {
            printf("__METAL_SMOKE__ FAIL reason=pso err=%s\n",
                   err ? err.localizedDescription.UTF8String : "unknown");
            fflush(stdout);
            return 1;
        }

        /* ── 5. command queue ── */
        id<MTLCommandQueue> queue = [device newCommandQueue];
        if (!queue) {
            printf("__METAL_SMOKE__ FAIL reason=command_queue\n");
            fflush(stdout);
            return 1;
        }

        /* ── 6. render pass: clear to blue, draw triangle in red ── */
        /*
         * clear color  = BGRA (0.0, 0.0, 1.0, 1.0) = pure blue  → stored BGRA8 = FF FF 00 00
         * triangle col = RGBA (1.0, 0.0, 0.0, 1.0) = pure red   → stored BGRA8 = FF 00 00 FF
         *
         * We'll read back pixel (0,0) (upper-left corner = in the triangle) and pixel
         * (TEX_W-1, TEX_H-1) (lower-right corner = definitely clear).
         */
        MTLRenderPassDescriptor *rpDesc = [MTLRenderPassDescriptor new];
        rpDesc.colorAttachments[0].texture     = tex;
        rpDesc.colorAttachments[0].loadAction  = MTLLoadActionClear;
        /* blue clear: R=0 G=0 B=1 A=1 */
        rpDesc.colorAttachments[0].clearColor  = MTLClearColorMake(0.0, 0.0, 1.0, 1.0);
        rpDesc.colorAttachments[0].storeAction = MTLStoreActionStore;

        /* triangle color buffer: red (R=1 G=0 B=0 A=1) */
        float triColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        id<MTLBuffer> colorBuf = [device newBufferWithBytes:triColor
                                                     length:sizeof(triColor)
                                                    options:MTLResourceStorageModeShared];
        if (!colorBuf) {
            printf("__METAL_SMOKE__ FAIL reason=color_buffer_alloc\n");
            fflush(stdout);
            return 1;
        }

        id<MTLCommandBuffer> cmd = [queue commandBuffer];
        id<MTLRenderCommandEncoder> enc = [cmd renderCommandEncoderWithDescriptor:rpDesc];
        [enc setRenderPipelineState:pso];
        [enc setVertexBuffer:colorBuf offset:0 atIndex:1];
        [enc drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
        [enc endEncoding];
        [cmd commit];
        [cmd waitUntilCompleted];

        /* ── 7. readback ── */
        size_t rowBytes = (size_t)TEX_W * 4;
        size_t bufSize  = rowBytes * TEX_H;
        uint8_t *pixels = (uint8_t *)malloc(bufSize);
        if (!pixels) {
            printf("__METAL_SMOKE__ FAIL reason=readback_malloc\n");
            fflush(stdout);
            return 1;
        }
        [tex getBytes:pixels
          bytesPerRow:rowBytes
           fromRegion:MTLRegionMake2D(0, 0, TEX_W, TEX_H)
          mipmapLevel:0];

        /*
         * In NDC the triangle covers roughly:
         *   x ∈ [-0.5, 0.5]  → pixel columns [0,TEX_W/4]..[3*TEX_W/4]  → col 0..48 ish
         *   y ∈ [-0.5, 0.5]  → pixel rows    [TEX_H/4]..[3*TEX_H/4]
         *
         * The upper-left vertex is at NDC (-0.5, 0.5) → pixel (TEX_W/4, 0) in Metal's
         * top-left convention. A safe centroid of the triangle is ≈ (-0.17, 0.17)
         * → pixel col ≈ TEX_W*(0.5-0.17) ≈ 21, row ≈ TEX_H*(0.5-0.17) ≈ 21.
         */
        int tri_x = TEX_W / 4;      /* ≈ 16 — safely inside the triangle */
        int tri_y = TEX_H / 4;      /* ≈ 16 */
        int clr_x = TEX_W - 1;      /* bottom-right — clear region */
        int clr_y = TEX_H - 1;

        uint32_t px_tri = pixel_at(pixels, tri_x, tri_y);
        uint32_t px_clr = pixel_at(pixels, clr_x, clr_y);

        free(pixels);

        /* ── 8. assertions ── */
        /* clear pixel must be blue (RRGGBBAA = 0000FFFF) */
        int clr_ok = (px_clr == 0x0000FFFFU);
        /* triangle pixel must be red (RRGGBBAA = FF0000FF) */
        int tri_ok = (px_tri == 0xFF0000FFU);
        /* pixels must differ (non-uniform buffer — real render) */
        int nonuniform = (px_tri != px_clr);

        if (!clr_ok || !tri_ok || !nonuniform) {
            printf("__METAL_SMOKE__ FAIL reason=pixel_assertion"
                   " px_clear=0x%08X(expected=0x0000FFFF)"
                   " px_triangle=0x%08X(expected=0xFF0000FF)"
                   " nonuniform=%d\n",
                   px_clr, px_tri, nonuniform);
            fflush(stdout);
            return 1;
        }

        printf("__METAL_SMOKE__ PASS device=%s px_clear=0x%08X px_triangle=0x%08X nonuniform=1\n",
               device.name.UTF8String, px_clr, px_tri);
        fflush(stdout);
        return 0;
    }
}
