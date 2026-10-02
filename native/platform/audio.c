/* Audio: silent for now. These replace the game's sound setup, which is all CRI ADX / SPU2 (IOP)
 * plumbing; the game-side sound logic gets decompiled when audio is done properly. */

/* ADX sound system init (system +0x305280, PS2 0x001AACD0) */
void func_001AACD0(void *snd) { (void)snd; }

/* SPU2 sound driver init (system +0x395D40, PS2 0x002102E0) */
void func_002102E0(void *drv) { (void)drv; }
