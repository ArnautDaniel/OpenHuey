/* Sony SDK functions (libgraph, libdma, libkernl...) the game calls, replaced on PC. */

/* sceDmaGetChan(i): the register block of DMA channel i (0 VIF0, 1 VIF1, 2 GIF, ...). The game
 * pokes CHCR directly, so each channel gets a fake register block. */
static unsigned sDmaRegs[10][0x40];

unsigned func_0010D3B8(unsigned i) { return i < 10 ? (unsigned)sDmaRegs[i] : 0; }

static int dma_channel(const void *chan) {
    int i;

    for (i = 0; i < 10; i++) {
        if (chan == sDmaRegs[i]) {
            return i;
        }
    }
    return -1;
}

/* sceDmaSend(chan, tag): the packets get interpreted by the PC GS (to do); dropped for now */
void func_0010D6E8(void *chan, void *tag) {
    (void)dma_channel(chan);
    (void)tag;
}

/* sceDmaSync(chan, mode, timeout): transfers complete immediately on PC */
int func_0010D988(void *chan, int mode, int timeout) {
    (void)chan;
    (void)mode;
    (void)timeout;
    return 0;
}

/* FlushCache(mode): no caches to write back on PC */
void FlushCache(int mode) { (void)mode; }

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

/* sceGsDefDispEnv(disp, psm, w, h, dx, dy): PMODE, SMODE2, DISPFB, DISPLAY, BGCOLOR. The PS2
 * version derives DISPLAY from the video timing tables; the PC GS only needs the frame buffer
 * (DISPFB) and the shown size, so DISPLAY is just (w - 1, h - 1) at 1x. */
void sceGsDefDispEnv(unsigned long long *disp, short psm, short w, short h, short dx, short dy) {
    (void)dx;
    (void)dy;
    disp[0] = 0x66;
    disp[1] = 2;   /* SMODE2: interlaced, frame mode */
    disp[2] = (unsigned long long)(psm & 0xF) << 15 | (unsigned long long)(((w + 63) / 64) & 0x3F) << 9;
    disp[3] = (unsigned long long)(h - 1) << 44 | (unsigned long long)(w - 1) << 32;
    disp[4] = 0;
}
