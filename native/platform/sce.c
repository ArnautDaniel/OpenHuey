/* Sony SDK functions (libgraph, libdma, libkernl...) the game calls, replaced on PC. */

/* libgraph: entry i (0..9) of a parameter / address table. */
extern unsigned D_003ACD80[];

unsigned func_0010D3B8(unsigned i) { return i < 10 ? D_003ACD80[i] : 0; }
