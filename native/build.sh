#!/bin/sh
# native/build.sh — build pe_to_macho_shim on Apple Silicon macOS.
#
# own1: clang + Apple SDK only. No Wine, no community toolchain.
#
# usage: cd native && ./build.sh
# output: native/pe_to_macho_shim (codesigned ad-hoc with JIT
# entitlement). Run e.g.:
#   ./pe_to_macho_shim ~/Downloads/Battle.net-Setup.exe

set -eu

cd "$(dirname "$0")"

CC="${CC:-clang}"
SDK="$(xcrun --sdk macosx --show-sdk-path)"
CFLAGS="-arch arm64 -isysroot $SDK -O2 -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-gnu-zero-variadic-macro-arguments -std=c11"
ENTITLEMENTS="entitlements.plist"

build_signed() {
    local target="$1"
    shift
    echo "[build] compiling $target (arch=arm64, sdk=macosx)"
    "$CC" $CFLAGS -o "$target" "$@"
    echo "[build] codesigning $target (ad-hoc + entitlements)"
    codesign \
        --force \
        --sign - \
        --entitlements "$ENTITLEMENTS" \
        --options runtime \
        --timestamp=none \
        "$target"
}

build_signed "pe_to_macho_shim" pe_to_macho_shim.c pe_parse.c
build_signed "i386_decode_test" i386_decode_test.c i386_decode.c pe_parse.c
build_signed "i386_cpu_test" i386_cpu_test.c i386_cpu.c i386_decode.c pe_parse.c

# metal_offscreen_smoke — Objective-C + Metal; own1: plain Apple Metal,
# no game asset, substrate proof (validated_manjeom stays 0).
# Does NOT use -std=c11 / -Wpedantic (ObjC, not C11).
echo "[build] compiling metal_offscreen_smoke (arch=arm64, sdk=macosx, ObjC+Metal)"
"$CC" -arch arm64 -isysroot "$SDK" -fobjc-arc -O2 -Wall -Wextra \
    -framework Metal -framework Foundation \
    -o metal_offscreen_smoke metal_offscreen_smoke.m
echo "[build] codesigning metal_offscreen_smoke (ad-hoc)"
codesign --force --sign - --timestamp=none metal_offscreen_smoke

echo "[build] verifying signatures"
codesign --verify --verbose=2 pe_to_macho_shim || true
codesign --verify --verbose=2 i386_decode_test || true
codesign -d --entitlements - pe_to_macho_shim 2>&1 | head -10 || true

codesign --verify --verbose=2 i386_cpu_test || true
codesign --verify --verbose=2 metal_offscreen_smoke || true

# d3d11_metal_bridge_test — D3D11-interface-shaped Metal bridge test (E5 r11).
# own1: gamebox's own D3D11 *interface shape* over Apple Metal.
# No Wine, no DXVK, no d3d11.dll source. Plain Metal backend.
# 구현됨·미배선 (dead-until-wired to PE via interpreter, r12 target).
echo "[build] compiling d3d11_metal_bridge_test (arch=arm64, sdk=macosx, ObjC+Metal)"
"$CC" -arch arm64 -isysroot "$SDK" -fobjc-arc -O2 -Wall -Wextra \
    -framework Metal -framework Foundation \
    -o d3d11_bridge_test \
    d3d11_metal_bridge_test.c d3d11_metal_bridge.m
echo "[build] codesigning d3d11_bridge_test (ad-hoc)"
codesign --force --sign - --timestamp=none d3d11_bridge_test
codesign --verify --verbose=2 d3d11_bridge_test || true

# e5_end_to_end_test — F-NSWINDOW-E5 r12: PE→D3D11→Metal end-to-end (own1).
# Self-authored i386 PE32 + interpreter IAT autobind → D3D11 bridge shims →
# d3d11_metal_bridge.m → Apple Metal headless → pixel readback.
# own1: no Wine, no DXVK, no game asset, no DRM.
echo "[build] compiling e5_end_to_end_test (arch=arm64, sdk=macosx, ObjC+Metal)"
"$CC" -arch arm64 -isysroot "$SDK" -fobjc-arc -O2 -Wall -Wextra \
    -framework Metal -framework Foundation \
    -o e5_end_to_end_test \
    e5_end_to_end_test.c i386_cpu.c i386_decode.c pe_parse.c d3d11_metal_bridge.m
echo "[build] codesigning e5_end_to_end_test (ad-hoc)"
codesign --force --sign - --timestamp=none e5_end_to_end_test
codesign --verify --verbose=2 e5_end_to_end_test || true

echo "[build] done — ./pe_to_macho_shim <pe.exe>  /  ./i386_decode_test <pe.exe> [count]  /  ./i386_cpu_test [pe.exe]  /  ./metal_offscreen_smoke  /  ./d3d11_bridge_test  /  ./e5_end_to_end_test"
