#!/bin/sh
# Differential-test every non-static function defined in a C file (compiled once).
# usage: tools/difftest_file.sh src/path/file.c [runs]   -> one PASS/FAIL line per function
cd "$(dirname "$0")/.."
exec .venv/bin/python tools/difftest.py "$1" ${2:+--runs "$2"}
