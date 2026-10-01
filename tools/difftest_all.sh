#!/bin/sh
# Run the differential tester on every decompiled function (list in tools/difftest_list.txt).
# usage: tools/difftest_all.sh [runs]   (default: 30 for straight-line functions, 200 otherwise)
cd "$(dirname "$0")/.."
exec nice -n 19 .venv/bin/python tools/difftest.py --list tools/difftest_list.txt ${1:+--runs "$1"}
