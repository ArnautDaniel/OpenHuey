/* Reaching a function that is still only in the PS2 disassembly (see tools/native_gen.py).
 *
 *   HG_SKIP_STUBS=1   don't stop: report each such function once and return 0 (a survey of
 *                     what a path still needs; results are unreliable past the first one:
 *                     a skipped float-returning function unbalances the x87 stack, so later
 *                     float maths turns into NaN) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "port.h"

int hg_undecompiled(const char *name, unsigned addr) {
    static const char *seen[4096];
    static int nseen = -1;
    int i;

    if (nseen < 0) {
        nseen = getenv("HG_SKIP_STUBS") != NULL ? 0 : -2;
    }
    if (nseen >= 0) {
        for (i = 0; i < nseen; i++) {
            if (seen[i] == name) {
                return 0;
            }
        }
        if (nseen < 4096) {
            seen[nseen++] = name;
        }
        fprintf(stderr, "skipped undecompiled: %s (PS2 0x%08X)\n", name, addr);
        hg_backtrace(1, 2);
        return 0;
    }
    fprintf(stderr, "\nundecompiled: %s (PS2 0x%08X)\ncalled from:\n", name, addr);
    hg_backtrace(1, 15);
    exit(2);
}
