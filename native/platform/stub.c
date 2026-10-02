/* Reaching a function that is still only in the PS2 disassembly (see tools/native_gen.py).
 *
 *   HG_SKIP_STUBS=1   don't stop: report each such function once and return 0 (a survey of
 *                     what a path still needs; results are unreliable past the first one) */
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hg_undecompiled(const char *name, unsigned addr) {
    static const char *seen[4096];
    static int nseen = -1;
    void *frames[16];
    int n, i;

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
        n = backtrace(frames, 3);
        fprintf(stderr, "skipped undecompiled: %s (PS2 0x%08X)\n", name, addr);
        backtrace_symbols_fd(frames + 1, n - 1, 2);
        return 0;
    }
    n = backtrace(frames, 16);
    fprintf(stderr, "\nundecompiled: %s (PS2 0x%08X)\ncalled from:\n", name, addr);
    backtrace_symbols_fd(frames + 1, n - 1, 2);
    exit(2);
}
