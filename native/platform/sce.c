/* Sony SDK functions (libgraph, libdma, libkernl...) the game calls, replaced on PC. */

/* libgraph: entry i (0..9) of a parameter / address table. */
extern unsigned D_003ACD80[];

unsigned func_0010D3B8(unsigned i) { return i < 10 ? D_003ACD80[i] : 0; }

/* libgraph: sceGsResetGraph-like reset (the PC renderer sets itself up in the platform layer) */
void func_0010D3E0(int mode) { (void)mode; }

/* libdbc: DualShock socket manager init */
void func_001EE798(void) {}

/* libgraph: sceGsResetPath */
void func_0010BFB0(void) {}

/* libgraph: sceGsResetGraph(mode, interlace, ntsc/pal, field/frame) */
void func_0010BE10(int mode, int inter, int omode, int ffmd) {
    (void)mode;
    (void)inter;
    (void)omode;
    (void)ffmd;
}
