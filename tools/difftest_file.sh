#!/bin/sh
# Differential-test every function defined in a C file.
# usage: tools/difftest_file.sh src/path/file.c [runs]   -> one PASS/FAIL line per function
cd "$(dirname "$0")/.."
file=$1
runs=${2:-200}
fail=0
for fn in $(.venv/bin/python - "$file" <<'PY'
import re, sys
src = open(sys.argv[1]).read()
for m in re.finditer(r"^(?!static\b)[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{", src, re.M):
    print(m.group(1))
PY
); do
    out=$(.venv/bin/python tools/difftest.py "$file" "$fn" --runs "$runs" 2>&1)
    line=$(echo "$out" | grep -E "^(PASS|FAIL)" | head -1)
    if echo "$line" | grep -q "^PASS"; then
        echo "$line" | sed -E 's/runs identical \(.*\), return=/ok, return=/' | cut -c1-110
    else
        fail=1
        echo "FAIL $fn"
        echo "$out" | grep -vE "^seed" | head -6 | sed 's/^/    /'
    fi
done
exit $fail
