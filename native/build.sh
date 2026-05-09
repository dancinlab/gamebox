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

echo "[build] verifying signatures"
codesign --verify --verbose=2 pe_to_macho_shim || true
codesign --verify --verbose=2 i386_decode_test || true
codesign -d --entitlements - pe_to_macho_shim 2>&1 | head -10 || true

echo "[build] done — ./pe_to_macho_shim <pe.exe>  /  ./i386_decode_test <pe.exe> [count]"
