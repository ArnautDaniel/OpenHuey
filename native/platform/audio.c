/* Audio: silent for now. These replace the game's sound setup, which is all CRI ADX / SPU2 (IOP)
 * plumbing; the game-side sound logic gets decompiled when audio is done properly. */

/* ADX sound system init (system +0x305280, PS2 0x001AACD0) */
void func_001AACD0(void *snd) { (void)snd; }

/* SPU2 sound driver init (system +0x395D40, PS2 0x002102E0) */
void func_002102E0(void *drv) { (void)drv; }

/* The SPU sound driver (system +0x395D40, vtables 0x46BF20 / 0x46BF2C): its methods send
 * commands to the IOP sound driver. Silent for now: every method does nothing and returns 0. */
int func_0020E000(void) { return 0; }
int func_00210160(void) { return 0; }
int func_00210120(void) { return 0; }
int func_00210070(void) { return 0; }
int func_0020FE30(void) { return 0; }
int func_0020FD90(void) { return 0; }
int func_0020FD20(void) { return 0; }
int func_0020FC40(void) { return 0; }
int func_0020FBD0(void) { return 0; }
int func_0020FB50(void) { return 0; }
int func_0020FB20(void) { return 0; }
int func_0020FAB0(void) { return 0; }
int func_0020FA30(void) { return 0; }
int func_0020F9C0(void) { return 0; }
int func_0020F950(void) { return 0; }
int func_0020F8E0(void) { return 0; }
int func_0020F870(void) { return 0; }
int func_0020F810(void) { return 0; }
int func_0020F740(void) { return 0; }
int func_0020F6C0(void) { return 0; }
int func_0020F570(void) { return 0; }
int func_0020F4D0(void) { return 0; }
int func_0020F1B0(void) { return 0; }
int func_0020F0D0(void) { return 0; }
int func_0020F070(void) { return 0; }
int func_0020E8C0(void) { return 0; }
int func_0020E8A0(void) { return 0; }
int func_0020F000(void) { return 0; }
int func_0020EFD0(void) { return 0; }
int func_0020EF50(void) { return 0; }
int func_0020EED0(void) { return 0; }
int func_0020EE60(void) { return 0; }
int func_0020EDF0(void) { return 0; }
int func_0020ED30(void) { return 0; }
int func_0020ECF0(void) { return 0; }
int func_0020EC70(void) { return 0; }
int func_0020EC00(void) { return 0; }
int func_0020EB70(void) { return 0; }
int func_0020EB30(void) { return 0; }
int func_0020E880(void) { return 0; }
int func_0020E9A0(void) { return 0; }
int func_0020E950(void) { return 0; }
int func_0020E8D0(void) { return 0; }
int func_0020FEA0(void) { return 0; }
int func_0020E9F0(void) { return 0; }
int func_0020E870(void) { return 0; }

/* per-frame tick of the ADX sound system: nothing to do yet */
void func_001AACC0(void *snd) { (void)snd; }

/* sound driver per-frame tick */
void func_00210230(void *drv) { (void)drv; }

/* CRI ADX streams (ADXT): silent. Creating one fails; the rest accept a NULL handle. */
void *ADXT_Create(int maxch, void *work, int size) { (void)maxch; (void)work; (void)size; return 0; }
void ADXT_Destroy(void *adxt) { (void)adxt; }
void ADXT_SetReloadSct(void *adxt, int n) { (void)adxt; (void)n; }
void ADXT_SetOutVol(void *adxt, int vol) { (void)adxt; (void)vol; }
void ADXT_SetOutPan(void *adxt, int ch, int pan) { (void)adxt; (void)ch; (void)pan; }
void ADXT_SetLpFlg(void *adxt, int on) { (void)adxt; (void)on; }
void ADXT_SetWaitPlayStart(void *adxt, int on) { (void)adxt; (void)on; }
/* ADX: mono / stereo output (PS2 0x001D4750) */
void func_001D4750(int mono) { (void)mono; }
