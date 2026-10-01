#!/bin/sh
# Run the differential tester on every decompiled function (list in tools/difftest_list.txt).
# usage: tools/difftest_all.sh [runs]
cd "$(dirname "$0")/.."
runs=${1:-300}
fail=0
while read -r src fn opts; do
    case "$src" in ""|\#*) continue;; esac
    out=$(.venv/bin/python tools/difftest.py "$src" "$fn" --runs "$runs" $opts 2>&1)
    echo "$out" | tail -1 | cut -c1-130
    echo "$out" | grep -q "^PASS" || { fail=1; echo "$out" | head -5; }
done < tools/difftest_list.txt
exit $fail
