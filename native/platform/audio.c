/* Audio: silent for now. These replace the game's sound setup, which is all CRI ADX / SPU2 (IOP)
 * plumbing; the game-side sound logic gets decompiled when audio is done properly. */

/* ADX sound system init (system +0x305280, PS2 0x001AACD0) */
void func_001AACD0(void *snd) { (void)snd; }


/* per-frame tick of the ADX sound system: nothing to do yet */
void func_001AACC0(void *snd) { (void)snd; }

/* CRI ADX streams (ADXT): adx.c */

extern void hg_skipped(const char *what);   /* skip.c */

/* the sound manager's positioned voice (PS2 0x002FF600: the 3D placement 0x2FF4B0, then
 * method +0xB8) - the pursuers' footsteps and voices: silent until the sound engine is on PC */
void func_002FF600(void *snd, int id, int a2, int a3, int a4, int a5) {
    (void)snd;
    (void)id;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    hg_skipped("func_002FF600 positioned sound");
}
