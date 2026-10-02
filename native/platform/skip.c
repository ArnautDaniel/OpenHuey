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

/* load stalker / event character `id` (0x28 kinds) into character slot 2: not yet - 0 ("not
 * loaded"), so scripts go on as if it can't come. Each kind is a class of its own (the
 * stalkers' AI); they come later. */
int func_00171160(void *p, unsigned id) {
    (void)p;
    (void)id;
    skipped("func_00171160 stalker load");
    return 0;
}

/* load event character `id` (0x28 kinds) into slot `slot` (3..5): not yet, 0 as above */
int func_0016D6D0(void *p, unsigned id, unsigned slot) {
    (void)p;
    (void)id;
    (void)slot;
    skipped("func_0016D6D0 event character load");
    return 0;
}

/* Progress +0x48: start the music player of kind `mode` (0..3; at Scene +0x106503C and
 * D_00456DF0): not yet - no player, so the music commands do nothing */
void func_0039A8E0(void *g, unsigned mode) {
    (void)g;
    (void)mode;
    skipped("func_0039A8E0 music player");
}

/* play sound effect `id` (bank, volume, pitch) at a position through the sound system: not
 * yet - the sound engine comes with the PC audio */
void func_002FF650(void *snd, int id, int bank, const float *pos, int vol, int pitch) {
    (void)snd;
    (void)id;
    (void)bank;
    (void)pos;
    (void)vol;
    (void)pitch;
    skipped("func_002FF650 3D sound effect");
}
