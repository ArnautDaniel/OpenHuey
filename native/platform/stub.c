/* Reaching a function that is still only in the PS2 disassembly (see tools/native_gen.py). */
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

void hg_undecompiled(const char *name, unsigned addr) {
    void *frames[16];
    int n = backtrace(frames, 16);

    fprintf(stderr, "\nundecompiled: %s (PS2 0x%08X)\ncalled from:\n", name, addr);
    backtrace_symbols_fd(frames + 1, n - 1, 2);
    exit(2);
}
