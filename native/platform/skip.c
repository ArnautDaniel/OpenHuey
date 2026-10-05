/* Game functions not decompiled yet that the PC build skips on purpose (each logged once):
 * self-contained effects off the path being worked on, and the stalkers. Each definition goes away when
 * the function is decompiled in src/. */
#include <stdio.h>

static void skipped(const char *name) {
    static const char *seen[32];
    static int n;
    int i;

    for (i = 0; i < n; i++) {
        if (seen[i] == name) {
            return;
        }
    }
    if (n < 32) {
        seen[n++] = name;
    }
    fprintf(stderr, "skip: %s (not decompiled yet)\n", name);
}

/* for game code that holds back natively until something it needs is in C */
void hg_skipped(const char *name) {
    skipped(name);
}



