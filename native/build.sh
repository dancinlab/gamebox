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

echo "[build] done — ./pe_to_macho_shim <pe.exe>  /  ./i386_decode_test <pe.exe> [count]  /  ./i386_cpu_test [pe.exe]  /  ./metal_offscreen_smoke"
