#!/usr/bin/env bash
# Runs the engine-independent unit tests (no Geode SDK needed).
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT="$(mktemp -d)"
trap 'rm -rf "$OUT"' EXIT

CXX="${CXX:-g++}"
FLAGS=(-std=c++23 -Wall -Wextra -Wpedantic)

echo "==> test_before_frame"
"$CXX" "${FLAGS[@]}" "$ROOT/tests/test_before_frame.cpp" -o "$OUT/test_before_frame"
"$OUT/test_before_frame"

echo "==> test_i18n"
"$CXX" "${FLAGS[@]}" "$ROOT/tests/test_i18n.cpp" "$ROOT/src/i18n.cpp" -o "$OUT/test_i18n"
"$OUT/test_i18n" "$ROOT"

echo "==> validating JSON"
python3 -m json.tool "$ROOT/mod.json" > /dev/null
echo "mod.json is valid"

echo "All checks passed"
