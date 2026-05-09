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
TARGET="pe_to_macho_shim"
SOURCES="pe_to_macho_shim.c pe_parse.c"
ENTITLEMENTS="entitlements.plist"

echo "[build] compiling $TARGET (arch=arm64, sdk=macosx)"
"$CC" \
    -arch arm64 \
    -isysroot "$(xcrun --sdk macosx --show-sdk-path)" \
    -O2 \
    -Wall -Wextra -Wpedantic \
    -Wno-unused-parameter \
    -std=c11 \
    -o "$TARGET" \
    $SOURCES

echo "[build] codesigning $TARGET (ad-hoc + entitlements)"
codesign \
    --force \
    --sign - \
    --entitlements "$ENTITLEMENTS" \
    --options runtime \
    --timestamp=none \
    "$TARGET"

echo "[build] verifying signature"
codesign --verify --verbose=2 "$TARGET" || true
codesign -d --entitlements - "$TARGET" 2>&1 | head -20 || true

echo "[build] done — ./$TARGET <pe.exe>"
