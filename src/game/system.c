/* The game's system object (Game +0x69AC0, vtable 0x46ADF0, ticked every frame) and the
 * objects it embeds: file loader, ... Constructors so far. */
#include "common.h"
#include "game.h"
#include "ptmf.h"

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

extern void *D_0044E978;   /* the Game (set by its base constructor) */
extern void *D_0046BEE0[];

/* Game's base class constructor. */
void *func_0020E7F0(Game *game) {
    D_0044E978 = game;
    game->vtbl = D_0046BEE0;
    game->nextMode = 0;
    game->softResetEnabled = 0;
    game->modeParam = 0;
    return game;
}

/* Element constructors for two arrays in the +0x395D40 object. */
void *func_0020E7D0(void *e) {
    AT(e, 0x8, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x0, s32) = 0;
    AT(e, 0xC, u8) = 0;
    AT(e, 0xD, u8) = 0;
    return e;
}

void *func_0020E7B0(void *e) {
    AT(e, 0x4, s32) = 0;
    AT(e, 0x0, s32) = 0;
    AT(e, 0xC, s32) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0x14, s32) = 0;
    AT(e, 0x10, u8) = 0;
    return e;
}

/* Global pointers to the parts (set by the constructor). */
extern void *D_0044F7F8;    /* the system object */
extern void *D_0044FEB0;    /* +0x40 */
extern void *D_0044FF00;    /* +0x390 */
extern VObject *D_0044E4F0; /* +0x460: the renderer */
extern void *D_0044E980;    /* +0x305280 */
extern void *D_0044FEF8;    /* +0x305280 +0x7C44 */
extern void *D_0044E9A0;    /* +0x30CF40 */
extern void *gFileLoader;   /* +0x319900 */
extern void *D_0044E560;    /* +0x395D44 */

extern void *D_0046ADF0[], *D_0046ADD0[], *D_0046ADB0[], *D_0046ADC4[], *D_0046AD88[], *D_0046AE90[], *D_0046AEB4[];
extern void *D_0046AC50[], *D_0046AF00[], *D_0046AF0C[], *D_0046C740[], *D_0046B050[], *D_0046A1E0[];
extern void *D_0046BF20[], *D_0046BF2C[];
extern u8 D_0047E360[], D_0047E3C0[16], D_0047E3D0[16];
extern const PTMF sGameStateNull;

extern void *func_00115D20(void *p, s32 c, u32 n);   /* memset */
extern void func_002D4680(void *obj, void *arg);
extern void *func_002D4630(void *f);   /* Fader constructor (fader.c) */
extern void func_001B80C0(u8 *r);
extern void func_00100340(void *array, void *(*ctor)(void *), void (*dtor)(void *, s32), u32 size, u32 count);   /* __construct_array */
extern void func_001BEC10(void *, s32), func_001BECA0(void *, s32);   /* the element destructors */

/* System object constructor. */
void *func_0020E340(u8 *s) {
    u8 *p;
    s32 i;

    AT(s, 0x0, void **) = D_0046ADF0;
    D_0044F7F8 = s;
    AT(s, 0x20, void **) = D_0046AD88;
    D_0044FEB0 = s + 0x40;
    AT(s, 0x40, void **) = D_0046ADD0;
    func_002D4680(s + 0x40, D_0047E360);
    for (i = 0; i < 16; i++) {
        D_0047E3C0[i] = i;
    }
    for (i = 0; i < 16; i++) {
        D_0047E3D0[i] = i;
    }
    AT(s, 0x40, void **) = D_0046ADB0;
    AT(s, 0x58, void **) = D_0046ADC4;
    func_002D4630(s + 0x300);

    D_0044FF00 = s + 0x390;
    AT(s, 0x390, void **) = D_0046AE90;
    AT(s, 0x39C, void **) = D_0046AEB4;
    D_0044E4F0 = (VObject *)(s + 0x460);
    AT(s, 0x3A4, PTMF) = sGameStateNull;
    AT(s, 0x460, void **) = D_0046AC50;
    func_001B80C0(s + 0x460);

    p = s + 0x305280;
    D_0044E980 = p;
    AT(p, 0x0, void **) = D_0046AF00;
    AT(p, 0x4, s32) = 0;
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x10, u8) = 0;
    AT(p, 0x124, void **) = D_0046AF0C;
    AT(p, 0x7C48, u8) = 0;
    D_0044FEF8 = p + 0x7C44;
    AT(p, 0x7C44, void **) = D_0046C740;
    func_00115D20(p + 0x7C4C, 0, 0x20);
    func_00115D20(p + 0x7C6C, 0, 0x30);

    D_0044E9A0 = s + 0x30CF40;
    AT(s, 0x30CF40, void **) = D_0046B050;
    gFileLoader = s + 0x319900;
    AT(s, 0x319900, void **) = D_0046A1E0;

    p = s + 0x395D40;
    D_0044E560 = p + 4;
    AT(p, 0x0, void **) = D_0046BF20;
    AT(p, 0x4, void **) = D_0046BF2C;
    func_00100340(p + 0x84, func_0020E7D0, func_001BEC10, 0x10, 8);
    func_00100340(p + 0x108, func_0020E7B0, func_001BECA0, 0x18, 8);
    AT(p, 0x80, s32) = 0;
    AT(p, 0x104, s8) = -1;
    AT(p, 0x7EC, s32) = 0x100;
    AT(p, 0x7F0, s32) = 0x100;
    AT(p, 0x7F4, s32) = 0x80;
    AT(p, 0x7F8, s32) = 0x80;
    AT(p, 0x7DC, u8 *) = p + 0x1DC;
    AT(p, 0x7E0, u8 *) = p + 0x3DC;
    AT(p, 0x7E4, u8 *) = p + 0x5DC;
    AT(p, 0x7E8, u8 *) = p + 0x6DC;
    return s;
}

extern u32 func_0010D3B8(u32 i);   /* libgraph: parameter / address table entry i (0..9) */
extern u32 _fbss;                 /* first word of .bss: libgraph table entry 2 */

/* Renderer state (system +0x460) defaults: 640x512 display, draw buffers, colour 0x80808080. */
void func_001B80C0(u8 *r) {
    u32 i;

    AT(r, 0x304BE8, s32) = 0;
    AT(r, 0x304BEC, s32) = 0x88000;
    AT(r, 0x304BF0, s32) = 0x50000;
    AT(r, 0x304BF4, u32) = 0x80808080;
    AT(r, 0x304BF8, s32) = 0;
    AT(r, 0x304BFC, s16) = 640;
    AT(r, 0x304BFE, s16) = 512;
    AT(r, 0x304C00, s16) = 512;
    AT(r, 0x304C02, s16) = 448;
    AT(r, 0x304C04, u8) = 0;
    AT(r, 0x304C05, u8) = 0;
    AT(r, 0x304C06, u8) = 0x20;
    AT(r, 0x304C07, u8) = 0;
    AT(r, 0x304C08, u8) = 0;
    AT(r, 0x304C09, u8) = 2;
    AT(r, 0x304BB0, u32) = func_0010D3B8(1);
    _fbss = func_0010D3B8(2);
    AT(r, 0x304BB4, u8) = 0;
    AT(r, 0x304BB5, u8) = 0;
    ((void (*)(u8 *))(*(void ***)r)[0x1C / 4])(r);
    for (i = 0; i < 11; i++) {
        AT(r, 0x304BB8 + i * 4, s32) = -1;
    }
    AT(r, 0x304BE4, s32) = -1;
    AT(r, 0x304D58, s32) = 0;
    AT(r, 0x304DDC, s32) = 0;
}

/* Heap (vtable 0x46A1C0) setup: memory, size, block table, block count; then its init (+0xC). */
void func_00169260(VObject *h, void *base, u32 size, void *blocks, s32 count) {
    AT(h, 0x4, void *) = base;
    AT(h, 0x8, u32) = size;
    AT(h, 0xC, void *) = blocks;
    AT(h, 0x10, s32) = count;
    VCALL(h, 0xC, void (*)(VObject *, void *, u32, void *, s32))(h, base, size, blocks, count);
}

extern void *D_0046BF08[], *D_004699E0[], *D_0046A1C0[];
extern void *D_0044E960;   /* the scene table */

/* Game +0x400A00: the scene table (4 scene pointers) and the scene heap after it (Game.sceneHeap,
 * 0x10D9000 bytes from +0x40). */
void *func_0020E280(u8 *t) {
    VObject *heap = (VObject *)(t + 0x10D9040);
    s32 i;

    D_0044E960 = t;
    AT(t, 0x0, void **) = D_0046BF08;
    heap->vtbl = D_004699E0;
    AT(heap, 0x4, s32) = 0;
    AT(heap, 0x8, s32) = 0;
    heap->vtbl = D_0046A1C0;
    AT(heap, 0xC, s32) = 0;
    AT(heap, 0x10, s32) = 0;
    for (i = 0; i < 4; i++) {
        AT(t, 0x4 + i * 4, void *) = NULL;
    }
    func_00169260(heap, t + 0x40, 0x10D9000, t + 0x10D9054, 10);
    return t;
}

extern void *D_00469A60[];
extern void *D_0044E4B8;   /* Game +0x14D9B00 */

void *func_0020E260(VObject *o) {
    D_0044E4B8 = o;
    o->vtbl = D_00469A60;
    return o;
}

/* Object pool element constructors. */
void *func_0020E240(void *e) {
    AT(e, 0x0, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x8, s32) = 0;
    return e;
}

void *func_0020E210(void *e) {
    AT(e, 0xC, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0x10, s32) = 0;
    return e;
}

void *func_0020E190(void *e) {
    AT(e, 0x0, s32) = 0;
    AT(e, 0x4, s32) = 0;
    return e;
}

void *func_0020E180(void *e) {
    AT(e, 0x44, s32) = 0;
    AT(e, 0x48, s32) = 0;
    return e;
}

extern void *D_004562B0, *D_004562A8;   /* the two pools */
extern void func_0020D970(void *, s32), func_0020D9C0(void *, s32);
extern void func_0020D8D0(void *, s32), func_0020D920(void *, s32);

/* Game +0x14D9DD0: pool of 64 x 0xC and 462 x 0x14 entries. */
void *func_0020E1A0(u8 *p) {
    D_004562B0 = p;
    func_00100340(p, func_0020E240, func_0020D970, 0xC, 0x40);
    func_00100340(p + 0x300, func_0020E210, func_0020D9C0, 0x14, 0x1CE);
    return p;
}

/* Game +0x14DC530: pool of 32 x 0xC and 632 x 0x50 entries. */
void *func_0020E110(u8 *p) {
    D_004562A8 = p;
    func_00100340(p, func_0020E190, func_0020D8D0, 0xC, 0x20);
    func_00100340(p + 0x180, func_0020E180, func_0020D920, 0x50, 0x278);
    return p;
}

extern void *D_0046B1D0[];
extern void *D_0044E4E8;      /* Game +0x14E8C90 */

/* reset: 64 empty slots; the ids of the renderer's 10 layers (renderer +0x3C) */
static inline void Slots_Reset(u8 *o) {
    s32 i;

    for (i = 0; i < 64; i++) {
        u8 *e = o + 4 + i * 0xC;

        AT(e, 0x0, s32) = 0;
        AT(e, 0x4, s16) = -1;
        AT(e, 0x6, s16) = -1;
        AT(e, 0x9, s8) = -1;
        AT(e, 0x8, s8) = -1;
    }
    for (i = 0; i < 10; i++) {
        AT(o, 0x304 + i * 4, s32) = -1;
    }
    for (i = 0; i < 10; i++) {
        AT(o, 0x304 + i * 4, s32) = VCALL(D_0044E4F0, 0x3C, s32 (*)(VObject *, s32))(D_0044E4F0, i);
    }
}

/* Game +0x14E8C90 (shut down by func_001F4100): constructor */
void *func_001F4600(u8 *o) {
    AT(o, 0x0, void **) = D_0046B1D0;
    D_0044E4E8 = o;
    Slots_Reset(o);
    return o;
}

/* ... init (from Game_Init) */
void func_001F44D0(u8 *o) {
    Slots_Reset(o);
}

/* ---- system init (vtable +0xC, from Game_Init) ---- */

#include "ps2hw.h"

extern const char D_0044FEB8[], D_0044FEC8[], D_0044FED8[], D_0044FEE8[];   /* SIO2MAN, SIO2D, DBCMAN, LIBSD .IRX */
extern void func_001BC220(void *iop);                        /* IOP: reset, set up module loading */
extern s32 func_001BC0F0(void *iop, const char *module, s32, s32, s32);   /* IOP: load a module */
extern void func_0010D3E0(s32 mode);                          /* libgraph: reset */
extern void func_001EE798(void);                              /* libdbc: init */
extern void func_001AACD0(void *obj);
extern void func_002102E0(void *obj);
extern void func_001BE6A0(void *pads);
extern void func_00226570(void *obj);
extern void func_001B83D0(void *renderer, s32 n);
extern void func_001B8250(void *renderer);
extern s32 func_0026BE80(s32 cause, s32 (*handler)(s32), s32 next);   /* AddIntcHandler */
extern s32 func_0026CCE8(s32 cause);                                   /* EnableIntc */
extern s32 func_001BEDA0(s32 cause), func_001BED80(s32 cause);         /* vblank start / end handlers */
extern void func_0016C530(void *loader);
extern void func_00169680(void *loader);
extern u8 D_0047B204, D_0047B208;   /* vblank start / end seen */
extern u32 D_0047B20C;              /* vblank count */

void func_001BF080(u8 *s) {
    func_001BC220(s + 0x20);
    func_0010D3E0(1);
    AT(s, 0x4, s32) = func_001BC0F0(s + 0x20, D_0044FEB8, 0, 0, 0);
    AT(s, 0x8, s32) = func_001BC0F0(s + 0x20, D_0044FEC8, 0, 0, 0);
    AT(s, 0xC, s32) = func_001BC0F0(s + 0x20, D_0044FED8, 0, 0, 0);
    func_001EE798();
    AT(s, 0x10, s32) = func_001BC0F0(s + 0x20, D_0044FEE8, 0, 0, 0);
    func_001AACD0(s + 0x305280);
    func_002102E0(s + 0x395D40);
    func_001BE6A0(s + 0x40);
    func_00226570(s + 0x390);
    func_001B83D0(s + 0x460, 2);
    VCALL(s + 0x30CF40, 0xC, void (*)(void *))(s + 0x30CF40);
    func_001B8250(s + 0x460);
    /* timer 0 and 1: count on the horizontal blank */
    HW_WRITE32(0x10000010, 0x82);
    HW_WRITE32(0x10000000, 0);
    HW_WRITE32(0x10000810, 0x82);
    D_0047B20C = 0;
    D_0047B204 = 0;
    AT(s, 0x14, s32) = func_0026BE80(2, func_001BEDA0, 0);
    func_0026CCE8(2);
    D_0047B208 = 0;
    AT(s, 0x18, s32) = func_0026BE80(3, func_001BED80, 0);
    func_0026CCE8(3);
    AT(s, 0x1C, s32) = D_0047B20C;
    func_0016C530(s + 0x319900);
    func_00169680(s + 0x319900);
}
