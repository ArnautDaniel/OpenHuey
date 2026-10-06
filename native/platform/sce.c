#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
/* Sony SDK functions (libgraph, libdma, libkernl...) the game calls, replaced on PC. */

/* sceDmaGetChan(i): the register block of DMA channel i (0 VIF0, 1 VIF1, 2 GIF, ...). The game
 * pokes CHCR directly, so each channel gets a fake register block. */
static unsigned sDmaRegs[10][0x40];

unsigned func_0010D3B8(unsigned i) { return i < 10 ? (unsigned)sDmaRegs[i] : 0; }

/* The GS and DMA side: on PC everything is drawn with OpenGL (glr.c) as the game asks for it,
 * so the packets the game still sends - the frame's display / drawing environments and its
 * clear and copy, VRAM image transfers - have nothing left to do. */

/* sceDmaSend(chan, tag) */
void func_0010D6E8(void *chan, void *tag) {
    (void)chan;
    (void)tag;
}

/* sceGsPutDispEnv(disp) */
void func_0010C440(unsigned long long *disp) {
    (void)disp;
}

/* sceGsPutDrawEnv(packet) */
void sceGsPutDrawEnv(unsigned long long *giftag) {
    (void)giftag;
}

/* sceGsSyncPath: everything is done immediately on PC */
int sceGsSyncPath(int mode, int timeout) {
    (void)mode;
    (void)timeout;
    return 0;
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

/* sceGsSetDefLoadImage: the transfer packet (libgraph's sceGsLoadImage, 6 quadwords): a GIF tag,
 * BITBLTBUF / TRXPOS / TRXREG / TRXDIR, and the IMAGE tag for the data */
int sceGsSetDefLoadImage(void *lp, short dbp, short dbw, short dpsm, short x, short y, short w, short h) {
    uint64_t *q = lp;
    int psm = dpsm & 0x3F;
    int bits = (psm == 0x00 || psm == 0x30) ? 32 : (psm == 0x01 || psm == 0x31) ? 24
             : (psm == 0x13 || psm == 0x1B) ? 8 : (psm == 0x14 || psm == 0x24 || psm == 0x2C) ? 4 : 16;
    uint32_t qwc = (uint32_t)(((long)w * h * bits / 8 + 15) / 16);

    q[0] = 4 | (1ULL << 60);   /* 4 A+D */
    q[1] = 0xE;
    q[2] = ((uint64_t)(dbp & 0x3FFF) << 32) | ((uint64_t)(dbw & 0x3F) << 48) | ((uint64_t)psm << 56);
    q[3] = 0x50;
    q[4] = ((uint64_t)(x & 0x7FF) << 32) | ((uint64_t)(y & 0x7FF) << 48);
    q[5] = 0x51;
    q[6] = (uint64_t)(w & 0xFFF) | ((uint64_t)(h & 0xFFF) << 32);
    q[7] = 0x52;
    q[8] = 0;
    q[9] = 0x53;
    q[10] = (qwc & 0x7FFF) | (1ULL << 15) | (2ULL << 58);   /* IMAGE, EOP */
    q[11] = 0;
    return 0;
}

/* sceGsExecLoadImage */
int sceGsExecLoadImage(void *lp, const void *src) {
    (void)lp;
    (void)src;
    return 0;
}

/* sceCdReadClock(clock): the date and time now, in BCD - { status, second, minute, hour, 0,
 * day, month, year (from 2000) } - as the save headers keep it */
#include <time.h>

static unsigned char bcd(int v) { return (unsigned char)(((v / 10) % 10) << 4 | (v % 10)); }

int func_00110878(unsigned char *clock) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    clock[0] = 0;
    clock[1] = bcd(t->tm_sec);
    clock[2] = bcd(t->tm_min);
    clock[3] = bcd(t->tm_hour);
    clock[4] = 0;
    clock[5] = bcd(t->tm_mday);
    clock[6] = bcd(t->tm_mon + 1);
    clock[7] = bcd(t->tm_year % 100);
    return 1;
}

/* SDK shutdown at the game scene's end (libsd / SIF RPC clients closed, their semaphores
 * and the module freed): nothing on PC */
void func_001CA850(void) {}
void func_001C8478(void) {}
void func_001C8648(void *p) { (void)p; }

/* an SDK thread call made before interrupts are enabled again: nothing on PC */
void func_001CC5B0(int a) { (void)a; }
