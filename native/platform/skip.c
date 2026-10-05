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

/* the full-screen blur / dim effect (0x5A0 qwords of offset sprites; D_0046D7A0's draw) */
int func_002699D0(void *o) {
    (void)o;
    skipped("func_002699D0 full-screen blur");
    return 1;
}

/* renderer +0x58: the frame's post-process (copy to a smaller buffer and blend back, in strips;
 * once per frame, layer 0x29) */
int func_001BA260(void *r) {
    (void)r;
    skipped("func_001BA260 frame post-process");
    return 1;
}


