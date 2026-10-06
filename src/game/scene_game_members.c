/* SceneGame's members: constructors and resets reached from SceneGame_ctor. Grouped here until
 * the classes they belong to are identified (then they move to their subsystem's file). */
#include "common.h"
#include "game.h"
#include "navmesh.h"
#include "sce/libvu0.h"

extern void func_00120EC0(void *pool, u8 *base, u32 size, u32 n, u8 *used);   /* BlockPool init */
extern void func_00100340(void *array, void *(*ctor)(void *), void *(*dtor)(void *, s32), u32 size, u32 n);
extern VObject *D_0044E4E8;   /* the texture cache */
extern void *D_0046C320[], *D_0046ECF0[], *D_0046C540[];
extern void *D_0044FE08, *D_00456DF8;
extern VObject *D_0044E558;   /* the doors */
extern VObject *D_0044E568;   /* the rooms */
extern void *func_0021A370(void *, s32);
extern void *func_002D11C0(void *);
extern void *func_002D0CE0(void *, s32);
extern void *func_002D1260(void *);
extern void *func_00221920(void *, s32);

/* (a base-class constructor that does nothing) */
void *func_002D15B0(void *p) {
    return p;
}

void func_0021AFC0(void *p) {
}

/* an array element's constructor (nothing to do) */
void *func_002D1120(void *p) {
    return p;
}

void func_002A8890(u8 *p) {
    AT(p, 0x8, s32) = 0;
    AT(p, 0x4, s32) = 0;
}

/* release a door's (?) pending request: only while it is active (+0x70 == 1) */
void func_002212D0(u8 *p) {
    if (AT(p, 0x70, u8) == 1 && AT(p, 0x0, s32) != 0) {
        AT(p, 0x0, s32) = 0;
    }
}

void func_002A8410(u8 *p) {
    AT(p, 0x0, s32) = 0;
    AT(p, 0x1C, u8) = 0;
    AT(p, 0x18, s32) = 0;
    AT(p, 0x10, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x8, s32) = 0;
    AT(p, 0x4, s32) = 0;
    AT(p, 0x14, s32) = 0;
    AT(p, 0x1D, u8) = 0xFF;
}

/* a pool of 128 blocks of 0x140 bytes */
void func_002D7680(u8 *p) {
    func_00120EC0(p + 0xA040, p + 0x40, 0x140, 0x80, p + 0xA058);
}

void func_002ECB50(u8 *p) {
    AT(p, 0xC, s32) = 0;
    AT(p, 0x4, s32) = 0;
    AT(p, 0x0, s32) = 0;
    AT(p, 0x8, s32) = -1;
    AT(p, 0x10, u8) = 0xFF;
    AT(p, 0x11, u8) = 0;
}

/* drop two pending pointers (each with its partner word) */
void func_0017CD90(u8 *p) {
    if (AT(p, 0x4, s32) != 0) {
        AT(p, 0x4, s32) = 0;
        AT(p, 0x8, s32) = 0;
    }
    if (AT(p, 0xC, s32) != 0) {
        AT(p, 0xC, s32) = 0;
        AT(p, 0x10, s32) = 0;
    }
}

void func_00223D60(u8 *p) {
    AT(p, 0x4, s32) = 0;
    AT(p, 0x50C0, s32) = 0;
    AT(p, 0x50C8, s32) = 0;
    AT(p, 0x50C4, s32) = 0;
    AT(p, 0x50CC, s32) = 0;
}

/* a manager of 5 objects of 0xB0 bytes (vtable D_0046C320, global D_0044FE08) */
void *func_002D10C0(u8 *p) {
    AT(p, 0x0, void **) = D_0046C320;
    D_0044FE08 = p;
    func_00100340(p + 0x10, func_002D1120, func_0021A370, 0xB0, 5);
    return p;
}

/* a manager of 64 objects of 0xB0 bytes (vtable D_0046ECF0, global D_00456DF8) */
void *func_002D1160(u8 *p) {
    AT(p, 0x0, void **) = D_0046ECF0;
    D_00456DF8 = p;
    func_00100340(p + 0x20, func_002D11C0, func_002D0CE0, 0xB0, 0x40);
    return p;
}

/* the doors: 8 of 0x210 bytes (vtable D_0046C540, global D_0044E558) */
void *func_002D1200(u8 *p) {
    AT(p, 0x0, void **) = D_0046C540;
    D_0044E558 = (VObject *)p;
    func_00100340(p + 0x10, func_002D1260, func_00221920, 0x210, 8);
    AT(p, 0x4, s32) = 0;
    return p;
}

void func_002F08E0(u8 *p) {
    AT(p, 0x0, u8) = 0;
    AT(p, 0x1, u8) = 0;
    AT(p, 0x2, u16) = 0;
    AT(p, 0x4, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x10, s32) = 0;
    AT(p, 0x14, s32) = 0;
    AT(p, 0x18, s32) = 0;
    AT(p, 0x8, u16) = 0;
    AT(p, 0x1C, s32) = 0;
    AT(p, 0x20, s32) = 0;
    AT(p, 0x24, s32) = 0;
    AT(p, 0x28, s32) = 0;
    AT(p, 0x2C, s32) = 0;
    AT(p, 0x30, s32) = 0;
    AT(p, 0x38, s32) = 0x80;
    AT(p, 0x40, s32) = 0;
    AT(p, 0x3C, s32) = 0x40;
    AT(p, 0x34, f32) = 1.0f;
}

/* two block pools (10 x 0x1600 and 3 x 0x890 bytes) and a table of 10 */
void func_002E2B20(u8 *p) {
    s32 i;

    func_00120EC0(p + 0xDC40, p + 0x40, 0x1600, 0xA, p + 0xDC58);
    for (i = 0; i < 10; i++) {
        AT(p, i * 4, s32) = 0;
    }
    func_00120EC0(p + 0xF630, p + 0xDC80, 0x890, 3, p + 0xF648);
    AT(p, 0x38681, u8) = 0;
}

/* release the requests of the 8 doors that have one */
void func_00223C90(u8 *p) {
    u8 i;

    if (AT(p, 0x4, void *) == NULL) {
        return;
    }
    for (i = 0; i < 8; i++) {
        u8 *tbl = AT(p, 0x4, u8 *);
        s32 used = tbl != NULL && i < 8 ? AT(tbl, i * 4, s32) : 0;

        if (used) {
            func_002212D0(p + 0x10 + i * 0x210);
        }
    }
    AT(p, 0x4, void *) = NULL;
}

void func_002A7B40(u8 *p) {
    s32 i;

    for (i = 0; i < 8; i++) {
        AT(p, i * 6 + 5, u8) = 0;
        AT(p, i * 6 + 4, u8) = 0;
        AT(p, i * 6 + 3, u8) = 0;
        AT(p, i * 6 + 2, u8) = 0;
        AT(p, i * 6 + 1, u8) = 0;
        AT(p, i * 6 + 0, u8) = 0;
    }
    for (i = 0; i < 5; i++) {
        AT(p, 0x30 + i * 4 + 3, u8) = 0;
        AT(p, 0x30 + i * 4 + 2, u8) = 0;
        AT(p, 0x30 + i * 4 + 1, u8) = 0;
        AT(p, 0x30 + i * 4 + 0, u8) = 0;
    }
    for (i = 0; i < 3; i++) {
        AT(p, 0x44 + i * 0x10, u8) = 0;
        AT(p, 0x45 + i * 0x10, u8) = 0;
        AT(p, 0x46 + i * 0x10, u16) = 0;
        AT(p, 0x48 + i * 0x10, u16) = 0;
        AT(p, 0x4C + i * 0x10, s32) = 0;
        AT(p, 0x50 + i * 0x10, u8) = 0;
        AT(p, 0x80 + i * 0x20, u8) = 0;
        AT(p, 0x82 + i * 0x20, u16) = 0;
        AT(p, 0x84 + i * 0x20, u16) = 0;
        AT(p, 0x88 + i * 0x20, s32) = 0;
        AT(p, 0xE0 + i * 0xC, u8) = 0;
        AT(p, 0xE1 + i * 0xC, u8) = 0;
        AT(p, 0xE2 + i * 0xC, u8) = 0xFF;
        AT(p, 0xE3 + i * 0xC, u8) = 0xFF;
        AT(p, 0xE4 + i * 0xC, s32) = 0;
        AT(p, 0xE8 + i * 0xC, s32) = 0;
    }
    AT(p, 0x144, u8) = 0;
    AT(p, 0x148, f32) = 1.0f;
    AT(p, 0x14C, f32) = 1.0f;
    AT(p, 0x150, f32) = 0.0f;
    for (i = 0; i < 4; i++) {
        AT(p, 0x104 + i * 0x10, u8) = 0;
        AT(p, 0x108 + i * 0x10, s32) = -1;
        AT(p, 0x10C + i * 0x10, s32) = -1;
        AT(p, 0x110 + i * 0x10, u16) = 0xFFFF;
    }
}

extern void func_002ECB50(u8 *p);
extern void func_002F08E0(u8 *p);

/* a progress sub-object's reset */
void func_002A8060(u8 *p) {
    s32 i;

    AT(p, 0x0, s32) = 0;
    AT(p, 0x4, s32) = 0;
    AT(p, 0xA, u8) = 0;
    AT(p, 0x9, u8) = 0;
    AT(p, 0x8, u8) = 0;
    AT(p, 0x10, s32) = 0;
    AT(p, 0xC, s32) = 0;
    for (i = 0; i < 9; i++) {
        AT(p, 0xF8 + i * 4, s32) = 0;
    }
    func_002ECB50(p + 0x75C);
    for (i = 0; i < 0x20; i++) {
        AT(p, 0x14 + i * 4, s32) = 0;
    }
    for (i = 0; i < 0x40; i++) {
        AT(p, 0x94 + i, u8) = 0;
    }
    for (i = 0; i < 9; i++) {
        AT(p, 0xD4 + i * 4, s32) = 0;
    }
    for (i = 0; i < 400; i++) {
        AT(p, 0x11C + i * 4, u32) &= ~0xFFu;   /* (a chain of bit-field clears) */
    }
    for (i = 0; i < 4; i++) {
        AT(p, 0x770 + i * 0x10, u8) = 0;
        AT(p, 0x774 + i * 0x10, s32) = -1;
        AT(p, 0x778 + i * 0x10, s32) = -1;
        AT(p, 0x77C + i * 0x10, u16) = 0xFFFF;
    }
    func_002F08E0(p + 0x7B0);
    AT(p, 0x9D8, f32) = 0.0f;
    AT(p, 0x9DC, f32) = 0.0f;
    AT(p, 0x9E0, f32) = 1.0f;
    AT(p, 0x9E4, f32) = 0.0f;
    AT(p, 0x9E8, f32) = 1.0f;
    AT(p, 0x9EC, f32) = 0.0f;
    AT(p, 0x9F0, f32) = 0.0f;
    AT(p, 0x9F4, f32) = 0.0f;
    AT(p, 0x9F8, f32) = 0.0f;
    AT(p, 0x9FC, f32) = 1.0f;
    AT(p, 0xA00, f32) = 0.0f;
    AT(p, 0xA04, f32) = 0.0f;
    AT(p, 0xA08, f32) = 0.0f;
    for (i = 0; i < 60; i++) {
        AT(p, 0xA0C + i * 0x18, s32) = -1;
        AT(p, 0xA10 + i * 0x18, s32) = -1;
        AT(p, 0xA14 + i * 0x18, s32) = -1;
        AT(p, 0xA18 + i * 0x18, f32) = 0.0f;
        AT(p, 0xA1C + i * 0x18, f32) = 0.0f;
        AT(p, 0xA20 + i * 0x18, s32) = 0;
    }
    for (i = 0; i < 8; i++) {
        AT(p, 0xFAC + i * 2, u16) = 0;
    }
    for (i = 0; i < 4; i++) {
        AT(p, 0xFBC + i, u8) = 0;
    }
}

extern void func_0017CD90(u8 *p);
extern void func_00223C90(u8 *p);
extern void func_002A8890(u8 *p);
extern void func_0021AFC0(void *p);

static inline void clear_if_set(u8 *p, u32 off) {
    if (AT(p, off, s32) != 0) {
        AT(p, off, s32) = 0;
    }
}

/* the room manager (SceneGame +0x73EE80): drop everything for a new room */
void func_00120980(u8 *m) {
    static const u16 offs[16] = {
        0x9980, 0x9984, 0x9988, 0x9994, 0x9998, 0x999C, 0x99A0, 0x99A4,
        0x99A8, 0x99AC, 0x99B0, 0x99B4, 0x99B8, 0x99BC, 0x998C, 0x9990,
    };
    VObject *tc;
    s32 i;

    func_0017CD90(m + 0x3E0);
    for (i = 0; i < 16; i++) {
        clear_if_set(m, offs[i]);
    }
    for (i = 0; i < 2; i++) {
        AT(m, 0x3C0 + i * 4, s32) = -1;   /* the room buffers' rooms */
    }
    func_00223C90(m + 0x1640);
    VCALL((VObject *)(m + 0x6740), 0xC, void (*)(VObject *))((VObject *)(m + 0x6740));
    func_002A8890(m + 0x9360);
    func_0021AFC0(m + 0x9380);
    tc = D_0044E4E8;
    VCALL(tc, 0x14, void (*)(VObject *, s32))(tc, 0);
    VCALL(tc, 0x14, void (*)(VObject *, s32))(tc, 0x15);
}

#include "task.h"

extern void *D_0046A9C0[], *D_00469D00[], *D_0046F350[];

/* a message object with its own dimming overlay (vtable D_0046A9C0): a Task at +0xC, the
 * overlay (D_0046F350) at +0x120 */
void *func_002D13B0(u8 *p) {
    AT(p, 0x0, void **) = D_0046A9C0;
    Task_Construct((Task *)(p + 0xC));
    AT(p, 0x120, void **) = D_00469D00;
    AT(p, 0x124, s32) = -1;
    AT(p, 0x120, void **) = D_0046F350;
    AT(p, 0x144, s32) = 0;
    AT(p, 0x130, s32) = -1;
    AT(p, 0x134, u8) = 0;
    return p;
}

extern const PTMF sGameStateNull;
extern void func_00223D60(u8 *p);
extern void func_00120980(u8 *m);

/* the room manager's constructor body (SceneGame +0x73EE80): no room-load state, then reset */
void func_00120C80(u8 *m) {
    func_00223D60(m + 0x1640);
    VCALL((VObject *)(m + 0x6740), 0x8, void (*)(VObject *))((VObject *)(m + 0x6740));
    AT(m, 0x3D4, PTMF) = sGameStateNull;
    AT(m, 0x3C8, PTMF) = AT(m, 0x3D4, PTMF);
    func_00120980(m);
}

extern void *gCharacters[6];
extern void *gCharPlayer, *gCharPartner, *gCharPursuer;
extern void func_002A8060(u8 *p);
extern void func_002A7B40(u8 *p);
extern void func_002A8410(u8 *p);
extern void func_0026BCC0(void *msg);
extern void func_002D7680(u8 *p);
extern void func_002E2B20(u8 *p);

/* Progress: reset (no characters registered; its tables, the message object, the pools) */
void func_00176780(u8 *prog) {
    s32 i, j;

    gCharacters[0] = NULL;
    gCharPlayer = NULL;
    gCharacters[1] = NULL;
    gCharacters[2] = NULL;
    gCharPartner = NULL;
    gCharacters[3] = NULL;
    gCharacters[4] = NULL;
    gCharPursuer = NULL;
    gCharacters[5] = NULL;
    func_002A8060(prog + 8);
    func_002A7B40(prog + 0xFD0);
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 7; j++) {
            func_002A8410(prog + 0x1134 + i * 0xE0 + j * 0x20);
        }
    }
    func_0026BCC0(prog + 0x6FC218);
    func_002D7680(prog + 0x6FC340);
    func_002E2B20(prog + 0x706440);
}

/* set a 3-word value: +0 = b, +4 = a, +8 = c */
void func_001F40F0(u8 *p, s32 a, s32 b, s32 c) {
    AT(p, 0x4, s32) = a;
    AT(p, 0x0, s32) = b;
    AT(p, 0x8, s32) = c;
}

/* one of the 64 (0xB0 bytes): cleared */
void func_0025FC50(u8 *e) {
    AT(e, 0x0, u8) = 0;
    AT(e, 0x1, u8) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0x70, s32) = 0;
    AT(e, 0x74, s32) = 0;
    AT(e, 0x78, s32) = 0;
    AT(e, 0x7C, s32) = 0;
    AT(e, 0x90, s32) = 0;
    AT(e, 0x94, s32) = 0;
    func_001F40F0(e + 0x98, 0, 0, 0);
}

/* the manager of 64 (D_0046ECF0) +0x8: clear them all */
void func_002C9480(u8 *p) {
    s32 i;

    AT(p, 0x4, s32) = 0;
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x10, s32) = 0;
    for (i = 0; i < 0x40; i++) {
        func_0025FC50(p + 0x20 + i * 0xB0);
    }
}

/* (the 12-byte entries of Progress +0x10B0: reset) */
void func_002A84C0(u8 *e) {
    AT(e, 0x0, u8) = 0;
    AT(e, 0x1, u8) = 0;
    AT(e, 0x2, u8) = 0xFF;
    AT(e, 0x3, u8) = 0xFF;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x8, s32) = 0;
}

/* (the 16-byte entries of Progress +0x1014: reset) */
void func_002A8500(u8 *e) {
    AT(e, 0x0, u8) = 0;
    AT(e, 0x1, u8) = 0;
    AT(e, 0x2, u16) = 0;
    AT(e, 0x4, u16) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0xC, u8) = 0;
}

/* the doors (D_0044E558) +0x48: door `i`'s data buffer (0x2000 bytes each, from +0x10C0) */
void *func_00221E70(u8 *d, s32 i) {
    return d + (i << 13) + 0x10C0;
}

/* ---- SceneGame +0xF6C1C0 (vtable D_0046B300, global D_0044E4C8): the scene's lights (16, set
 * through +0x20; an ambient colour at +0x10) and two VRAM areas (+0x320 / +0x324) ---- */

extern VObject *D_0044E9A0;   /* the VRAM manager */
extern VObject *D_0044E4F0;   /* the renderer */

extern VObject *D_0044E4B8;   /* the camera */

/* clip-space point c inside the view volume (|x|, |y|, |z| within w) */
static s32 clip_inside(const f32 *c) {
    return c[0] <= c[3] && !(c[0] < -c[3]) && c[1] <= c[3] && !(c[1] < -c[3]) && c[2] <= c[3] && !(c[2] < -c[3]);
}

/* a doorway quad (its first four points) is wholly on screen */
s32 func_001F9790(u8 *o, f32 (*q)[4]) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 c[4][4] __attribute__((aligned(16)));
    s32 out = 0;

    VCALL(D_0044E4B8, 0x48, void (*)(VObject *, f32 (*)[4]))(D_0044E4B8, m);
    sceVu0ApplyMatrix(c[0], m, q[0]);
    sceVu0ApplyMatrix(c[1], m, q[1]);
    sceVu0ApplyMatrix(c[2], m, q[2]);
    sceVu0ApplyMatrix(c[3], m, q[3]);
    if (!clip_inside(c[0]) || !clip_inside(c[1]) || !clip_inside(c[2]) || !clip_inside(c[3])) {
        out = 1;
    }
    return !out;
}

/* +0x38 add a lit doorway for this frame (six vectors: four corners, its facing, its top
 * middle) when it is wholly on screen and there is room (+0x340, 16 of them, +0x940 vectors
 * used) */
s32 func_001F9FC0(u8 *o, f32 (*q)[4]) {
    s32 i;

    if (!(u8)func_001F9790(o, q) || AT(o, 0x940, s32) >= 0x60) {
        return 0;
    }
    for (i = 0; i < 6; i++) {
        sceVu0CopyVector((f32 *)(o + 0x340 + (AT(o, 0x940, s32) + i) * 16), q[i]);
    }
    AT(o, 0x940, s32) += 6;
    return 1;
}

/* reset: ambient (0, 128, 128, 128), the 16 lights to "none" */
void func_001F9C80(u8 *o) {
    f32 light[12] __attribute__((aligned(16)));   /* (read with lq) */
    s32 i;

    AT(o, 0x9E0, s32) = 0;
    AT(o, 0x10, s32) = 0;
    AT(o, 0x14, f32) = 128.0f;
    AT(o, 0x18, f32) = 128.0f;
    AT(o, 0x1C, f32) = 128.0f;
    light[3] = 1.0f;
    light[0] = 0.0f;
    light[1] = 0.0f;
    light[2] = 0.0f;
    for (i = 4; i < 12; i++) {
        light[i] = 0.0f;
    }
    for (i = 0; i < 16; i++) {
        VCALL((VObject *)o, 0x20, void (*)(VObject *, f32 *, s32))((VObject *)o, light, i);
    }
}

/* a VRAM area for an offscreen picture: its slot, and TEX0 for drawing it (64 x 64? psm 0) */
static void vram_area(u8 *o, u32 slotOff, u32 tex0Off, s32 which) {
    VObject *vram = D_0044E9A0;
    s32 slot = VCALL(vram, 0x14, s32 (*)(VObject *, s32, s32, s32, s32))(vram, 0xFF, 0, 0, 0);

    AT(o, slotOff, s32) = slot;
    if (AT(o, slotOff, s32) != -1) {
        u64 v = VCALL(vram, 0x38, u64 (*)(VObject *, s32))(vram, AT(o, slotOff, s32));

        AT(o, tex0Off, u64) = (v & 0xFFFFFFE000000000ULL) | (0x21B13000 | (6ULL << 32));
        VCALL(D_0044E4F0, 0x94, void (*)(VObject *, s32, s32, s32, s32))(D_0044E4F0, AT(o, slotOff, s32), which, 1, 0);
    }
}

/* start of a room: reset the lights, get the two VRAM areas if not yet held */
void func_001F9D90(u8 *o) {
    func_001F9C80(o);
    if (AT(o, 0x320, s32) == -1) {
        vram_area(o, 0x320, 0x328, 0);
    }
    if (AT(o, 0x324, s32) == -1) {
        vram_area(o, 0x324, 0x330, 1);
    }
    AT(o, 0x944, s32) = 0;
    AT(o, 0x950, u8) = 0;
    AT(o, 0x960, s32) = 0;
    AT(o, 0x964, s32) = 0;
    AT(o, 0x968, s32) = 0;
    AT(o, 0x970, f32) = 1.0f;
    AT(o, 0x990, s32) = 0;
    AT(o, 0x994, s32) = 0;
    AT(o, 0x998, s32) = 0;
    AT(o, 0x9A0, s32) = 0;
    AT(o, 0x9A4, s32) = 0;
    AT(o, 0x9C0, s32) = 0;
    AT(o, 0x9C4, s32) = 0;
    AT(o, 0x9C8, s32) = 0;
    AT(o, 0x9D0, s32) = 0;
    AT(o, 0x9D4, s32) = 0;
}

/* +0x40 the extra light (+0x950) on or off: on, its vector (+0x960) and strength (+0x970);
   off also turns off the two below */
void func_001F96B0(u8 *o, u8 on, const f32 *v, f32 k) {
    AT(o, 0x950, u8) = on;
    if (on == 0) {
        AT(o, 0x980, u8) = 0;
        AT(o, 0x9B0, u8) = 0;
    } else {
        sceVu0CopyVector((f32 *)(o + 0x960), (f32 *)v);
        AT(o, 0x970, f32) = k;
    }
}

/* +0x44 extra light `i` (of 2, 0x30 apart from +0x980) on or off: on, its position (+0x990)
   and two parameters (+0x9A0 / +0x9A4) */
void func_001F9710(u8 *o, s32 i, u8 on, const f32 *pos, f32 a, f32 b) {
    u8 *l = o + i * 0x30;

    AT(l, 0x980, u8) = on;
    if (on != 0) {
        sceVu0CopyVector((f32 *)(l + 0x990), (f32 *)pos);
        AT(l, 0x9A0, f32) = a;
        AT(l, 0x9A4, f32) = b;
    }
}

extern void *D_0046B300[], *D_0046B350[];
extern VObject *D_0044E4C8;   /* the scene's lights */
extern void func_00100490(void *p);   /* operator delete */

/* +0x8: destructor (the global goes) */
void *func_001F9640(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046B300;
        AT(o, 0x0, void **) = D_0046B350;
        D_0044E4C8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* +0x20 set light `i` (3 vectors) */
void func_001FA5E0(u8 *o, const f32 *light, s32 i) {
    f32 *d = (f32 *)(o + 0x20 + i * 0x30);
    s32 k;

    for (k = 0; k < 12; k++) {
        d[k] = light[k];
    }
}

/* +0x1C light i (12 floats) into `out` (the light is returned by value: the caller's buffer
 * comes first) */
void func_001FA680(f32 *out, u8 *o, s32 i) {
    const f32 *l = (const f32 *)(o + 0x20 + i * 0x30);
    s32 k;

    for (k = 0; k < 12; k++) {
        out[k] = l[k];
    }
}

/* +0x18 VRAM area i's TEX0 (+0x328) */
u64 func_001FA6B0(u8 *o, s32 i) {
    return AT(o, 0x328 + i * 8, u64);
}

/* +0x2C a shadow may fall at `pos` on nav triangle `tri`: none of the triangle's lights (+0x4C
 * bits) is switched off for shadows (+0x944), and `pos` lies in front of the nav mesh's plane
 * (+0x2C, by the triangle) as seen from the camera */
s32 func_001FA430(u8 *o, u32 tri, f32 *pos) {
    f32 n[4] __attribute__((aligned(16)));
    f32 eye[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u32 bits = 0;
    u8 *tris;

    if (tri == (u32)-1) {
        return 0;
    }
    if (tri < AT(D_0044E570, 0x8, u32) && (tris = AT(D_0044E570, 0x4, u8 *)) != NULL) {
        bits = AT(tris + tri * 0x50, 0x4C, u32);
    }
    if (bits & AT(o, 0x944, u32)) {
        return 0;
    }
    VCALL((VObject *)D_0044E570, 0x2C, void (*)(VObject *, u32, f32 *))((VObject *)D_0044E570, tri, n);
    VCALL(D_0044E4B8, 0x20, void (*)(VObject *, f32 *))(D_0044E4B8, eye);
    sceVu0SubVector(d, pos, eye);
    return !(sceVu0InnerProduct(n, d) < 0.0f) ^ 1;
}

void func_001F99B0(VObject *l, s32 *out, u32 tri, const f32 *pos);

/* +0x14 the up to 3 lights casting shadows at `pos` on nav triangle `tri` into out[0..2]
 * (none: count 0, the rest -1) */
void func_001FA6C0(u8 *o, const f32 *pos, s32 tri, s32 *out) {
    if (AT(o, 0x10, s32) > 0 && tri != -1) {
        func_001F99B0((VObject *)o, out, tri, pos);
    } else {
        out[0] = 0;
        out[1] = -1;
        out[2] = -1;
    }
}

/* +0x3C the blocker quads (6 vectors each) */
u8 *func_001F9FB0(u8 *o) {
    return o + 0x340;
}

/* +0x40 the blocker quads' vector count (over 0x60: reset) */
s32 func_001F9F80(u8 *o) {
    if (AT(o, 0x940, s32) >= 0x61) {
        AT(o, 0x940, s32) = 0;
    }
    return AT(o, 0x940, s32);
}

/* ---- the lights for a model: the light set the character microprograms use ----
 * A light is 3 vectors: its position, its colour (w the intensity) and (range, falloff, ..).
 * For a model the 3 brightest lights reaching its nav triangle are picked, and given to VU1
 * as a direction matrix (transposed: per light the unit direction from the model to it, w
 * -dir.pos), a colour matrix (rows: colour x intensity; the 4th the ambient) and falloffs.
 * Per vertex the microprogram then makes min(ambient + sum colour * max(dir.N, 0) *
 * max(1 + falloff * (dir.P - dir.L), 0), 128) - see model.c (gl_light_rgba). */

extern void func_0025C6F0(f32 *q, const f32 *axis, f32 angle);   /* rotation about an axis */
extern void func_0025C770(f32 *q, f32 (*m)[4]);                  /* its matrix */

static f32 light_sqrt(f32 x) {
    return __builtin_sqrtf(x);
}

/* light i (12 floats) */
static void light_get(VObject *l, f32 *out, s32 i) {
    VCALL(l, 0x1C, void (*)(f32 *, VObject *, s32))(out, l, i);
}

/* the up to 3 lights brightest at `pos` of those reaching nav triangle `tri` (its +0x4C bits):
 * by their luminance (0.3 R + 0.6 G + 0.1 B) x intensity, within a range (v2.x, if any)
 * fading out linearly; their indices into out[0..2] (-1: none) */
void func_001F99B0(VObject *l, s32 *out, u32 tri, const f32 *pos) {
    f32 best[3][2];
    u32 mask = 0;
    s32 i, k;
    u8 *tris;

    if (tri < AT(D_0044E570, 0x8, u32) && (tris = AT(D_0044E570, 0x4, u8 *)) != NULL) {
        mask = AT(tris + tri * 0x50, 0x4C, u32);
    }
    best[1][0] = -1.0f;
    best[0][0] = -1.0f;
    best[2][0] = -1.0f;
    for (i = 0; i < 16; i++) {
        f32 v[12] __attribute__((aligned(16)));
        f32 idx, score, r;

        if (!(mask & (1u << i))) {
            continue;
        }
        light_get(l, v, i);
        idx = (f32)i;
        score = (0x1.333334p-1f /* 0.6 */ * v[5] + 0x1.333334p-2f /* 0.3 */ * v[4] + 0x1.99999ap-4f /* 0.1 */ * v[6]) * v[7];
        r = v[8];
        if (!(r <= 0.0f)) {
            f32 dy = pos[1] - v[1], dx = pos[0] - v[0], dz = pos[2] - v[2];
            f32 d2 = dy * dy + dx * dx + dz * dz;

            if (!(d2 < r * r)) {
                score = 0.0f;
            } else {
                score = score * ((r - light_sqrt(d2)) / r);
            }
        }
        if (score <= 0.0f) {
            continue;
        }
        for (k = 0; k < 3; k++) {
            if (best[k][0] == -1.0f) {
                best[k][0] = idx;
                best[k][1] = score;
                break;
            }
            if (!(score <= best[k][1])) {
                f32 ti = best[k][0], ts = best[k][1];

                best[k][0] = idx;
                best[k][1] = score;
                idx = ti;
                score = ts;
            }
        }
    }
    out[0] = (s32)best[0][0];
    out[1] = (s32)best[1][0];
    out[2] = (s32)best[2][0];
}

/* the scripted lighting on top (+0x950): the lights' colours scaled (+0x970), the ambient
 * raised (+0x960), and up to two lights from the camera's side (+0x980, 0x30 apart: on, colour
 * +0x10, turned +0x20 up and +0x24 about) into slots 2 and 1 with no falloff */
void func_001FA710(u8 *l, f32 (*dir)[4], f32 (*col)[4], f32 *fall, f32 (*lpos)[4]) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    f32 up[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 tgt[4] __attribute__((aligned(16)));
    f32 eye[4] __attribute__((aligned(16)));
    VObject *cam;
    s32 i, slot = 2;

    q[3] = 0.0f;
    q[2] = 0.0f;
    q[1] = 0.0f;
    q[0] = 0.0f;
    if (AT(l, 0x950, u8) == 0) {
        return;
    }
    cam = D_0044E4B8;
    for (i = 0; i < 3; i++) {
        col[i][0] = col[i][0] * AT(l, 0x970, f32);
        col[i][1] = col[i][1] * AT(l, 0x970, f32);
        col[i][2] = col[i][2] * AT(l, 0x970, f32);
    }
    col[3][0] = col[3][0] + AT(l, 0x960, f32);
    col[3][1] = col[3][1] + AT(l, 0x964, f32);
    col[3][2] = col[3][2] + AT(l, 0x968, f32);
    VCALL(cam, 0x20, void (*)(VObject *, f32 *))(cam, eye);
    VCALL(cam, 0x2C, void (*)(VObject *, f32 *))(cam, tgt);
    for (i = 0; i < 2; i++) {
        u8 *x = l + 0x980 + i * 0x30;

        if (AT(x, 0x0, u8) == 0) {
            continue;
        }
        sceVu0SubVector(d, eye, tgt);
        sceVu0Normalize(d, d);
        up[1] = 1.0f;
        up[2] = 0.0f;
        up[0] = 0.0f;
        sceVu0OuterProduct(axis, up, d);
        sceVu0Normalize(axis, axis);
        func_0025C6F0(q, axis, AT(x, 0x20, f32));
        func_0025C770(q, m);
        sceVu0ApplyMatrix(d, m, d);
        func_0025C6F0(q, up, AT(x, 0x24, f32));
        func_0025C770(q, m);
        sceVu0ApplyMatrix(d, m, d);
        dir[0][slot] = d[0];
        dir[1][slot] = d[1];
        dir[2][slot] = d[2];
        col[slot][0] = AT(x, 0x10, f32);
        col[slot][1] = AT(x, 0x14, f32);
        col[slot][2] = AT(x, 0x18, f32);
        if (lpos != NULL) {
            sceVu0CopyVector(lpos[slot], tgt);
            sceVu0AddVector(lpos[slot], lpos[slot], d);
        }
        fall[slot] = 0.0f;
        slot--;
    }
}

/* one picked light into slot k */
static void light_slot(f32 (*dir)[4], f32 (*col)[4], f32 *fall, f32 (*lpos)[4], s32 k, const f32 *v, const f32 *pos) {
    f32 at[4] __attribute__((aligned(16)));

    sceVu0SubVector(dir[k], (f32 *)v, pos);
    sceVu0Normalize(dir[k], dir[k]);
    sceVu0CopyVector(at, (f32 *)v);
    if (lpos != NULL) {
        sceVu0CopyVector(lpos[k], at);
    }
    dir[k][3] = -sceVu0InnerProduct(dir[k], at);
    col[k][0] = v[4] * v[7];
    col[k][1] = v[5] * v[7];
    col[k][2] = v[6] * v[7];
    col[k][3] = 0.0f;
    fall[k] = v[9];
}

/* the slots from k on empty: no direction, no colour, falloff 1 */
static void light_empty(f32 (*dir)[4], f32 (*col)[4], f32 *fall, s32 k) {
    for (; k < 3; k++) {
        dir[k][0] = 0.0f;
        dir[k][1] = 0.0f;
        dir[k][2] = 0.0f;
        dir[k][3] = 0.0f;
        sceVu0Normalize(dir[k], dir[k]);
        col[k][0] = 0.0f;
        col[k][1] = 0.0f;
        col[k][2] = 0.0f;
        col[k][3] = 0.0f;
        fall[k] = 1.0f;
    }
}

/* +0x10 the light set for a model at `pos` on nav triangle `tri` (none: the room's light 0
 * alone; no position: just a default ambient) into dir / col / fall (and the lights'
 * positions into lpos, if given) */
void func_001FAA00(u8 *l, const f32 *pos, s32 tri, f32 (*dir)[4], f32 (*col)[4], f32 *fall, f32 (*lpos)[4]) {
    f32 v[12] __attribute__((aligned(16)));
    s32 idx[4];
    s32 k = 0;

    if (AT(l, 0x10, s32) > 0 && tri != -1) {
        func_001F99B0((VObject *)l, idx, tri, pos);
        sceVu0UnitMatrix(dir);
        for (k = 0; k < 3 && idx[k] != -1; k++) {
            VCALL((VObject *)l, 0x1C, void (*)(f32 *, VObject *, s32))(v, (VObject *)l, idx[k]);
            light_slot(dir, col, fall, lpos, k, v, pos);
        }
    } else if (pos != NULL) {
        sceVu0UnitMatrix(dir);
        light_get((VObject *)D_0044E4C8, v, 0);
        light_slot(dir, col, fall, lpos, 0, v, pos);
        k = 1;
    } else {
        for (k = 0; k < 3; k++) {
            dir[k][0] = 0.0f;
            dir[k][1] = 0.0f;
            dir[k][2] = 0.0f;
            dir[k][3] = 0.0f;
            col[k][0] = 0.0f;
            col[k][1] = 0.0f;
            col[k][2] = 0.0f;
            col[k][3] = 0.0f;
            fall[k] = 1.0f;
        }
        AT(col[3], 0x0, u32) = 0x42980000;   /* 76 */
        AT(col[3], 0x4, u32) = 0x42640000;   /* 57 */
        AT(col[3], 0x8, u32) = 0x41400000;   /* 12 */
        col[3][3] = 1.0f;
        fall[3] = 0.0f;
        return;
    }
    light_empty(dir, col, fall, k);
    sceVu0TransposeMatrix(dir, dir);
    col[3][0] = AT(l, 0x14, f32);
    col[3][1] = AT(l, 0x18, f32);
    col[3][2] = AT(l, 0x1C, f32);
    col[3][3] = 1.0f;
    fall[3] = 0.0f;
    func_001FA710(l, dir, col, fall, lpos);
}

/* +0x24 light i back to the room's own (its table +0x9E0, if any) */
void func_001FA530(u8 *o, s32 i) {
    f32 l[12] __attribute__((aligned(16)));
    const f32 *src;
    s32 k;

    if (AT(o, 0x9E0, u8 *) == NULL) {
        return;
    }
    src = (const f32 *)(AT(o, 0x9E0, u8 *) + 0x10 + i * 0x30);
    for (k = 0; k < 12; k++) {
        l[k] = src[k];
    }
    VCALL((VObject *)o, 0x20, void (*)(VObject *, f32 *, s32))((VObject *)o, l, i);
}

extern VObject *gFileLoader;
static const char sHmbPck[] = "O_HMB\\HMB_000.PCK";
static const char sHmbTex[] = "O_HMB\\HMB_000.TEX";

/* SceneGame +0x706480 (the creatures' resources, HMB): load the model and its textures (the
 * second argument is unused) */
void func_002E2890(u8 *o, const void *unused) {
    VObject *ld = gFileLoader;

    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sHmbPck, o + 0xF680, 0x10000000, 0);
    VCALL(ld, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(ld, sHmbTex, o + 0x27680, 0x10000000, 0);
}

/* the doors +0x4C: select door buffer `i` (+0x50C0[i]; its first word to +0x50C8[i]); whether
 * it holds anything */
s32 func_00221E80(u8 *d, s32 i) {
    AT(d, 0x50C0 + i * 4, u8 *) = d + (i << 13) + 0x10C0;
    AT(d, 0x50C8 + i * 4, s32) = AT(AT(d, 0x50C0 + i * 4, u8 *), 0, s32);
    return AT(d, 0x50C8 + i * 4, s32) != 0;
}

extern VObject *gFileLoader;
extern char D_0044E4A0[];   /* "ST_%03X\\ST_%03X.PAC" */
extern s32 func_0026EDD0(char *buf, s32 size, const char *fmt, ...);   /* snprintf */
extern char *func_001183C0(char *d, const char *s);                     /* strcpy */

#define ROOM_SLOT_DONE 0x80000000   /* +0x3C0[slot]: the slot's load has been handled */

/* load room `room` (ST_xxx\ST_xxx.PAC) into slot `slot` (+0x99C0, 0x2A0000 each); a load
 * still in flight in that slot is first finished (its handler +0x3C8[slot]) or cancelled */
void func_00120720(u8 *rm, u32 room, s32 slot) {
    char path[0x100];
    char name[0x100];
    u32 *cur;
    PTMF *done;
    s32 busy;
    VObject *loader;

    if (room >= 0x110) {
        AT(rm, 0x3C8 + slot * 12, PTMF) = sGameStateNull;
        return;
    }
    cur = &AT(rm, 0x3C0 + slot * 4, u32);
    busy = 0;
    if (!(*cur & ROOM_SLOT_DONE)) {
        if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, *cur) == 2) {
            busy = 1;
        } else {
            done = &AT(rm, 0x3C8 + slot * 12, PTMF);
            if (ptmf_test(done)) {
                ptmf_scall_1(rm, done, slot);
                busy = 1;
            }
        }
    }
    if (busy && !(*cur & ROOM_SLOT_DONE)) {
        VCALL(gFileLoader, 0x14, void (*)(VObject *, u32))(gFileLoader, *cur);   /* cancel */
        *cur |= ROOM_SLOT_DONE;
        AT(rm, 0x3C8 + slot * 12, PTMF) = sGameStateNull;
    }
    *cur = room;
    func_0026EDD0(name, sizeof(name), D_0044E4A0, room & ~7, room);
    func_001183C0(path, name);
    loader = gFileLoader;
    if (VCALL(loader, 0x30, s32 (*)(VObject *, char *))(loader, path) > 0) {
        VCALL(loader, 0xC, void (*)(VObject *, const void *, void *, u32, s32))(
            loader, path, rm + slot * 0x2A0000 + 0x99C0, room, 0);
    }
    AT(rm, 0x3C8 + slot * 12, PTMF) = sGameStateNull;
}

/* SceneGame +0xF29740 (gSceneGameF29740): reset for a new room */
void func_001AABC0(u8 *o) {
    AT(o, 0x4, s32) = 0;
}


/* SceneGame +0xF6CD30: reset its pool of 32 0xA0-byte entries and the 32 slots +0x1438 */
void func_00267250(u8 *o) {
    s32 i;

    func_00120EC0(o + 0x1400, o, 0xA0, 0x20, o + 0x1418);
    for (i = 0; i < 32; i++) {
        AT(o, 0x1438 + i * 4, s32) = 0;
    }
}

/* operator delete for the room effects' pool: nothing (the pool is dropped at once) */
void func_002672E0(void *p) {
}

extern void func_00169260(void *, void *, u32, void *, s32);   /* heap init */

/* SceneGame +0xF6E200: reset its 64 KB heap (+0x10000, 0x802 blocks) and its 0x400 slots
 * (+0x18034) */
void func_002D6330(u8 *o) {
    u32 i;

    func_00169260(o + 0x10000, o, 0x10000, o + 0x10014, 0x802);
    for (i = 0; i < 0x400; i++) {
        AT(o, 0x18034 + i * 4, s32) = 0;
    }
    AT(o, 0x19034, u8) = 0;
}

/* the effect heap's free: nothing (effects sit in its arena, dropped all at once) */
void func_002D63B0(void *p) {
}

/* whether room slot `slot` is still loading (its handler +0x3C8[slot] runs once the file is
 * in) */
s32 func_00120660(u8 *rm, s32 slot) {
    u32 tag = AT(rm, 0x3C0 + slot * 4, u32);
    PTMF *done;

    if (tag & ROOM_SLOT_DONE) {
        return 0;
    }
    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, tag) == 2) {
        return 1;
    }
    done = &AT(rm, 0x3C8 + slot * 12, PTMF);
    if (!ptmf_test(done)) {
        return 0;
    }
    ptmf_scall_1(rm, done, slot);
    return 1;
}

extern VObject *gBootMessage;

/* SceneGame +0x706480: hook its message data (+0x27680) up to message slot 6 (result at
 * +0x38681) */
void func_002E2820(u8 *o) {
    AT(o, 0x38680, u8) = 6;
    AT(o, 0x38681, u8) = VCALL(gBootMessage, 0x8, u32 (*)(VObject *, u32, void *))(
        gBootMessage, AT(o, 0x38680, u8), o + 0x27680);
}

extern VObject *D_0044E4D0;
extern VObject *D_0044E4C0;
extern VObject *D_0044E4B8;   /* the camera */
extern u8 D_0047B350;
extern u8 *D_01991EC4;   /* the current room's section 10 */
extern void func_0017CC00(void *o, void *a, void *b, void *c);
extern void func_00223B60(u8 *d, u8 *sec);
extern s32 func_002A88A0(u8 *o, u8 *sec);
extern void func_0025D370(u8 *rm, u8 *sec);
extern void func_0025E0D0(u8 *rm);
extern void func_00266CD0(u8 *o, u8 *sec);
extern void func_0021B040(void *o);

/* A room PAC starts with 17 section offsets (0: none); the room manager keeps pointers to
 * them at +0x9980.. */
#define ROOM_SEC(rm, at) AT(rm, at, u8 *)

static u8 *room_section(u8 *pac, s32 i) {
    u32 off = AT(pac, i * 4, u32);

    return off != 0 ? pac + off : NULL;
}

/* make room slot `slot` the current room: find its sections and hand them to the model
 * set (+0x3E0), the camera, lights, collision (+0x1640), texture cache, ... */
void func_0011FFB0(u8 *rm, s32 slot) {
    u8 *pac = rm + slot * 0x2A0000 + 0x99C0;
    VObject *o;

    ROOM_SEC(rm, 0x9980) = room_section(pac, 0);
    if (ROOM_SEC(rm, 0x9980) == NULL) {
        ROOM_SEC(rm, 0x9994) = NULL;
    }
    if (ROOM_SEC(rm, 0x9980) != NULL) {
        ROOM_SEC(rm, 0x9984) = room_section(pac, 1);
        ROOM_SEC(rm, 0x99BC) = room_section(pac, 16);
        if (ROOM_SEC(rm, 0x9984) != NULL) {
            func_0017CC00(rm + 0x3E0, ROOM_SEC(rm, 0x9980), ROOM_SEC(rm, 0x9984),
                          ROOM_SEC(rm, 0x99BC));
        }
        o = (VObject *)(rm + 0x3E0);
        VCALL(o, 0x4C, void (*)(VObject *))(o);
        ROOM_SEC(rm, 0x9988) = room_section(pac, 2);
        if (D_0044E4D0 != NULL) {
            VCALL(D_0044E4D0, 0xC, void (*)(VObject *, void *))(D_0044E4D0, ROOM_SEC(rm, 0x9988));
        }
        ROOM_SEC(rm, 0x9994) = room_section(pac, 4);
        ROOM_SEC(rm, 0x99A0) = room_section(pac, 7);
        func_00223B60(rm + 0x1640, ROOM_SEC(rm, 0x99A0));
        ROOM_SEC(rm, 0x99A4) = room_section(pac, 8);
        ROOM_SEC(rm, 0x99B4) = room_section(pac, 14);
        func_002A88A0(rm + 0x9360, ROOM_SEC(rm, 0x99B4));
    }
    ROOM_SEC(rm, 0x9998) = room_section(pac, 5);
    ROOM_SEC(rm, 0x999C) = room_section(pac, 6);
    if (AT(pac, 0xC, u32) == 0) {
        ROOM_SEC(rm, 0x998C) = NULL;
        ROOM_SEC(rm, 0x9990) = NULL;
    } else {
        ROOM_SEC(rm, 0x998C) = pac + AT(pac, 0xC, u32);
        func_0025D370(rm, ROOM_SEC(rm, 0x998C));
        if (AT(pac, 0x24, u32) != 0) {
            ROOM_SEC(rm, 0x9990) = pac + AT(pac, 0x24, u32);
            VCALL(D_0044E4E8, 0x10, void (*)(VObject *, void *, s32))(D_0044E4E8, ROOM_SEC(rm, 0x9990), 0);
        }
        func_0025E0D0(rm);
        AT(rm, 0x35C, u8) = 0;
    }
    VCALL(D_0044E4C8, 0xC, void (*)(VObject *, void *))(D_0044E4C8, ROOM_SEC(rm, 0x9994));
    o = (VObject *)(rm + 0x1640);
    VCALL(o, 0x24, void (*)(VObject *, void *))(o, ROOM_SEC(rm, 0x99A4));
    VCALL(D_0044E4B8, 0x78, void (*)(VObject *, void *))(D_0044E4B8, ROOM_SEC(rm, 0x9998));
    if (AT(pac, 0x28, u32) != 0) {
        ROOM_SEC(rm, 0x99A8) = pac + AT(pac, 0x28, u32);
        D_0047B350 = 1;
    } else {
        ROOM_SEC(rm, 0x99A8) = NULL;
    }
    D_01991EC4 = ROOM_SEC(rm, 0x99A8);
    if (AT(pac, 0x2C, u32) != 0) {
        ROOM_SEC(rm, 0x99AC) = pac + AT(pac, 0x2C, u32);
        VCALL(D_0044E4E8, 0x10, void (*)(VObject *, void *, s32))(D_0044E4E8, ROOM_SEC(rm, 0x99AC), 0x15);
    } else {
        ROOM_SEC(rm, 0x99AC) = NULL;
        VCALL(D_0044E4E8, 0x14, void (*)(VObject *, s32))(D_0044E4E8, 0x15);
    }
    ROOM_SEC(rm, 0x99B0) = room_section(pac, 12);
    ROOM_SEC(rm, 0x99B8) = room_section(pac, 15);
    o = (VObject *)(rm + 0x6740);
    VCALL(o, 0x10, void (*)(VObject *, void *, void *))(o, ROOM_SEC(rm, 0x99B0), ROOM_SEC(rm, 0x99B8));
    VCALL(o, 0x14, void (*)(VObject *))(o);
    func_00266CD0((u8 *)D_0044E4C0, room_section(pac, 13));
    func_0021B040(rm + 0x9380);
    VCALL(D_0044E4F0, 0x1C, void (*)(VObject *))(D_0044E4F0);
}

extern void func_0010E5F0(f32 *out, const f32 *v);   /* libvu0: copy x, y, z */

/* the doors: take the room's doors (PAC section 7: 8 offsets, 0 = no door). Each door
 * (+0x10, 0x210 each): +0x4 index, +0x8 its id (-1: none, else +0x70 set), +0x10 / +0x20
 * two points, +0x30 / +0x40 a third (w 0), +0x72 present */
void func_00223B60(u8 *d, u8 *sec) {
    u8 i;

    AT(d, 0x4, u8 *) = sec;
    if (sec == NULL) {
        return;
    }
    for (i = 0; i < 8; i++) {
        u8 *e = d + 0x10 + i * 0x210;
        u8 *src;

        AT(e, 0x70, u8) = 0;
        if (AT(sec, i * 4, u32) == 0) {
            continue;
        }
        src = sec + AT(sec, i * 4, u32);
        AT(e, 0x4, u8) = i;
        AT(e, 0x8, s32) = AT(src, 0, s32);
        if (AT(e, 0x8, s32) != -1) {
            AT(e, 0x70, u8) = 1;
        }
        func_0010E5F0((f32 *)(e + 0x10), (f32 *)(src + 0x4));
        AT(e, 0x1C, f32) = 1.0f;
        func_0010E5F0((f32 *)(e + 0x20), (f32 *)(src + 0x10));
        AT(e, 0x2C, f32) = 1.0f;
        func_0010E5F0((f32 *)(e + 0x40), (f32 *)(src + 0x1C));
        AT(e, 0x4C, f32) = 0.0f;
        sceVu0CopyVector((f32 *)(e + 0x30), (f32 *)(e + 0x40));
        AT(e, 0x72, u8) = 1;
    }
}

/* room manager +0x9360: take PAC section 14 (+0x4; its count at +0x8) */
s32 func_002A88A0(u8 *o, u8 *sec) {
    if (sec != NULL) {
        AT(o, 0x8, s32) = AT(sec, 0, s32);
        AT(o, 0x4, u8 *) = sec;
    } else {
        AT(o, 0x8, s32) = 0;
        AT(o, 0x4, u8 *) = NULL;
    }
    return 0;
}

/* the room manager: take up to 8 areas from PAC section 3 (+0x10: offset of a list of
 * 0xC0-byte entries, -1 terminated). Per area: 7 bytes at +0xD8 (+0x8 bits 16..23, the low and
 * high nibble of +0xC, 0, the low nibble again, the size of its (x, z) quad in 1/256: width,
 * depth) and the quad (4 points in 0..1 from entry +0x50) at +0x140 and +0x240 (0x20 each) */
void func_0025D370(u8 *rm, u8 *sec) {
    u8 *e;
    s32 j;
    s32 k;

    if (AT(sec, 0x10, u32) == 0) {
        return;
    }
    e = sec + AT(sec, 0x10, u32);
    if (AT(e, 0, s32) == -1) {
        return;
    }
    for (j = 0; ; ) {
        u8 *b = rm + 0xD8 + j * 7;
        f32 *q = &AT(rm, 0x140 + j * 0x20, f32);
        f32 minX = 1.0f, maxX = 0.0f, minZ = 1.0f, maxZ = 0.0f;

        b[0] = (AT(e, 0x8, u32) & 0xFF0000) >> 16;
        b[1] = AT(e, 0xC, u32) & 0xF;
        b[2] = (AT(e, 0xC, u32) & 0xF0) >> 4;
        b[3] = 0;
        b[4] = b[1];
        for (k = 0; k < 4; k++) {
            f32 x = AT(e, 0x50 + k * 8, f32);
            f32 z;

            if (!(minX <= x)) {
                minX = x;
            }
            if (maxX < x) {
                maxX = x;
            }
            q[k * 2] = x;
            z = AT(e, 0x54 + k * 8, f32);
            if (!(minZ <= z)) {
                minZ = z;
            }
            if (maxZ < z) {
                maxZ = z;
            }
            q[k * 2 + 1] = z;
        }
        b[5] = (u8)(u32)(maxX * 256.0f - minX * 256.0f);
        b[6] = (u8)(u32)(maxZ * 256.0f - minZ * 256.0f);
        for (k = 0; k < 8; k++) {
            AT(rm, 0x240 + j * 0x20 + k * 4, f32) = q[k];
        }
        if (++j >= 8) {
            return;
        }
        e += 0xC0;
        if (AT(e, 0, s32) == -1) {
            return;
        }
    }
}

/* the room manager: reset the area state */
void func_0025E0D0(u8 *rm) {
    AT(rm, 0x6C, s32) = 0;
    AT(rm, 0x70, s32) = 0;
    AT(rm, 0x74, s32) = 0;
    AT(rm, 0x78, s32) = 0;
    AT(rm, 0x4, s32) = 0;
    AT(rm, 0x68, s32) = 0;
    AT(rm, 0x60, u8) = 0;
    AT(rm, 0x8B, u8) = 0;
}

/* the lights +0xC: take the room's lights (PAC section 4: count, ambient colour, then 0x30
 * bytes per light, set through +0x20); no section: reset */
void func_001FAF70(VObject *l, u8 *sec) {
    f32 light[12];
    s32 i, k;
    u8 *p;

    if (sec == NULL) {
        func_001F9C80((u8 *)l);
        return;
    }
    AT(l, 0x9E0, u8 *) = sec;
    AT(l, 0x10, s32) = AT(sec, 0x0, s32);
    AT(l, 0x14, f32) = AT(sec, 0x4, f32);
    AT(l, 0x18, f32) = AT(sec, 0x8, f32);
    AT(l, 0x1C, f32) = AT(sec, 0xC, f32);
    p = sec + 0x10;
    for (i = 0; i < AT(l, 0x10, s32); i++) {
        for (k = 0; k < 12; k++) {
            light[k] = AT(p, k * 4, f32);
        }
        p += 0x30;
        VCALL(l, 0x20, void (*)(VObject *, f32 *, s32))(l, light, i);
    }
}

extern void func_0025EEC0(u8 *o);
extern void func_00221880(u8 *door);

/* the doors +0x24: PAC section 8 (one offset per present door, in door order; entries
 * placed 0x10 apart): reset each present door and point it at its entry, then +0x7C */
void func_00222F60(VObject *d, u8 *sec) {
    u8 i;
    s32 n = 0;

    if (sec == NULL) {
        return;
    }
    for (i = 0; i < 8; i++) {
        u8 *tbl = AT(d, 0x4, u8 *);
        u8 *e;

        if (tbl == NULL || AT(tbl, i * 4, s32) == 0) {
            continue;
        }
        e = (u8 *)d + i * 0x210;
        func_0025EEC0(e + 0x90);
        e += 0x10;
        func_00221880(e);
        /* (the original then skips doors whose byte +0x70 equals -1, which a byte never is) */
        AT(e, 0x0, u8 *) = sec + AT(sec, n * 4, u32) + n * 0x10;
        n++;
    }
    VCALL(d, 0x7C, void (*)(VObject *))(d);
}

extern s32 func_00118278(const char *a, const char *b);   /* strcmp */

/* a placed object released: its state cleared and its animation key (+0x98) reset */
void func_0025FC10(u8 *obj) {
    AT(obj, 0x0, u8) = 0;
    AT(obj, 0x1, u8) = 0;
    AT(obj, 0x8, s32) = 0;
    AT(obj, 0x70, s32) = 0;
    AT(obj, 0x74, s32) = 0;
    AT(obj, 0x78, s32) = 0;
    AT(obj, 0x7C, s32) = 0;
    AT(obj, 0x90, s32) = 0;
    AT(obj, 0x94, s32) = 0;
    func_001F40F0(obj + 0x98, 0, 0, 0);
}

/* room manager +0x6740 +0x20: each placed object in use released and its bit cleared */
void func_002C9010(u8 *o) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        u8 *obj = o + 0x20 + i * 0xB0;
        u32 k;

        if (!(AT(o, 0xC + (i >> 5) * 4, u32) & (1u << (i & 0x1F)))) {
            continue;
        }
        func_0025FC10(obj);
        k = (u32)(obj - (o + 0x20)) / 0xB0;
        AT(o, 0xC + (k >> 5) * 4, u32) &= ~(1u << (k & 0x1F));
    }
}

/* room manager +0x6740 +0x1C: entry k of the named record in its table (+0x8: count, then
 * offsets; each record a name, +0x10 its count, +0x14 the offset of its 0x10-byte entries) */
u8 *func_002C90F0(u8 *o, const char *name, u32 k) {
    u8 *tbl = AT(o, 0x8, u8 *);
    u32 j;

    if (tbl == NULL) {
        return NULL;
    }
    for (j = 0; j < AT(tbl, 0x0, u32); j++) {
        u8 *e = AT(o, 0x8, u8 *) + AT(tbl, 0x4 + j * 4, u32);

        if (func_00118278((const char *)e, name) == 0) {
            if (k < AT(e, 0x10, u32)) {
                return e + AT(e, 0x14, u32) + k * 0x10;
            }
            return NULL;
        }
    }
    return NULL;
}

/* room manager +0x6740 +0x34 */
void func_002C9460(u8 *o) {
}

/* room manager +0x6740 +0x18: the placed object named `name` (its +0x70), or NULL */
u8 *func_002C91D0(u8 *o, const char *name) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        u8 *obj = o + 0x20 + i * 0xB0;

#ifdef HG_NATIVE
        if (AT(obj, 0x70, const char *) == NULL) {   /* unnamed (the PS2 compares with RAM at 0) */
            continue;
        }
#endif
        if ((AT(o, 0xC + (i >> 5) * 4, u32) & (1u << (i & 0x1F))) &&
            func_00118278(AT(obj, 0x70, const char *), name) == 0) {
            return obj;
        }
    }
    return NULL;
}

extern void func_0025FC50(u8 *o);
extern void func_0025F970(u8 *o, u8 *def);
extern void func_0025F910(u8 *o, u8 *def);
extern void func_0025F8B0(u8 *o, u8 *def);
extern void func_0025F850(u8 *o, u8 *def);

/* room manager +0x6740 +0x14: create the room's placed objects from its table (+0x4: four
 * counts, one per kind, then offsets from +0x20), each in a free slot of 64 (0xB0 each from
 * +0x20, use bitmap +0xC) */
void func_002C92A0(u8 *o) {
    u8 *h = AT(o, 0x4, u8 *);
    s32 end0, end1, end2, total;
    s32 i, j;

    if (h == NULL) {
        return;
    }
    end0 = AT(h, 0x0, s32);
    end1 = end0 + AT(h, 0x4, s32);
    end2 = end1 + AT(h, 0x8, s32);
    total = end2 + AT(h, 0xC, s32);
    for (i = 0; i < total; i++) {
        u8 *slot = NULL;
        u8 *def;

        for (j = 0; j < 0x40; j++) {
            u32 *used = &AT(o, 0xC + (j >> 5) * 4, u32);

            if (!(*used & (1 << (j & 0x1F)))) {
                *used |= 1 << (j & 0x1F);
                slot = o + j * 0xB0 + 0x20;
                func_0025FC50(slot);
                break;
            }
        }
        if (slot == NULL) {
            continue;
        }
        def = h + AT(h, 0x20 + i * 4, u32);
        if (i < end0) {
            func_0025F970(slot, def);
        } else if (i < end1) {
            func_0025F910(slot, def);
        } else if (i < end2) {
            func_0025F8B0(slot, def);
        } else {
            func_0025F850(slot, def);
        }
    }
}

/* ---- the room's placed objects (room manager +0x6740 slots, 0xB0 each): +0x4 kind (0..3),
 * +0x10 / +0x20 two vectors from the definition, +0x70 the definition ---- */

static void placed_init(u8 *o, s32 kind, u8 *def) {
    AT(o, 0x4, s32) = kind;
    AT(o, 0x48, s32) = AT(o, 0x4, s32);
    AT(o, 0x70, u8 *) = def;
    sceVu0CopyVector((f32 *)(o + 0x10), (f32 *)(def + 0x10));
    sceVu0CopyVector((f32 *)(o + 0x20), (f32 *)(def + 0x20));
}

void func_0025F970(u8 *o, u8 *def) {
    placed_init(o, 0, def);
}

void func_0025F910(u8 *o, u8 *def) {
    placed_init(o, 1, def);
}

void func_0025F8B0(u8 *o, u8 *def) {
    placed_init(o, 2, def);
}

void func_0025F850(u8 *o, u8 *def) {
    placed_init(o, 3, def);
}

/* back to the definition's two vectors */
void func_0025F810(u8 *o) {
    u8 *def = AT(o, 0x70, u8 *);

    AT(o, 0x10, f32) = AT(def, 0x10, f32);
    AT(o, 0x14, f32) = AT(def, 0x14, f32);
    AT(o, 0x18, f32) = AT(def, 0x18, f32);
    AT(o, 0x20, f32) = AT(def, 0x20, f32);
    AT(o, 0x24, f32) = AT(def, 0x24, f32);
    AT(o, 0x28, f32) = AT(def, 0x28, f32);
}


/* start animation `id` of the object (+0x94 its data from the placed objects' +0x1C, played
   by +0x98 from the start) */
void func_0025F9D0(u8 *o, s32 id) {
    u8 *a;

    AT(o, 0x94, u8 *) = VCALL((VObject *)D_00456DF8, 0x1C, u8 *(*)(void *, u8 *, s32))(D_00456DF8, AT(o, 0x70, u8 *), id);
    a = AT(o, 0x94, u8 *);
    if (a != NULL) {
        func_001F40F0(o + 0x98, AT(a, 0x4, s32), (s32)(a + AT(a, 0x8, s32)), AT(a, 0x0, s32));
        AT(o, 0x90, s32) = 0;
    }
}

extern void *D_0046D750[], *D_0046D7B0[], *D_0046EB40[], *D_0046EC60[];   /* the 4 effect classes */
extern void *func_002672F0(u32 size, void *place);   /* placement new */

/* (re)create effect `slot` of class `vtbl` from the pool (+0x1400) */
static s32 effect_create(u8 *o, s32 slot, void **vtbl) {
    VObject *pool = (VObject *)(o + 0x1400);
    VObject *e;
    void *mem;

    if (AT(o, slot, void *) != NULL) {
        VCALL(pool, 0x14, void (*)(VObject *, void *))(pool, AT(o, slot, void *));
        AT(o, slot, void *) = NULL;
    }
    mem = VCALL(pool, 0x10, void *(*)(VObject *, u32))(pool, 0xA0);
    if (mem == NULL) {
        return 0;
    }
    e = func_002672F0(0xA0, mem);
    if (e != NULL) {
        e->vtbl = vtbl;
    }
    AT(o, slot, VObject *) = e;
    e = AT(o, slot, VObject *);
    VCALL(e, 0xC, void (*)(VObject *))(e);
    return 1;
}

/* SceneGame +0xF6CD30 (D_0044E4C0): the room's effects from PAC section 13 (offsets at
 * +0x8.. +0x14 for the four effect slots +0x14B4.. +0x14A8; the third gets 4 words and 3
 * zeros); no section: the camera's default depth range */
void func_00266CD0(u8 *o, u8 *sec) {
    static const u16 sSlots[4] = {0x14B4, 0x14B0, 0x14AC, 0x14A8};
    void **classes[4];
    s32 k;

    classes[0] = D_0046D750;
    classes[1] = D_0046D7B0;
    classes[2] = D_0046EB40;
    classes[3] = D_0046EC60;
    if (sec == NULL) {
        VCALL(D_0044E4B8, 0xC0, void (*)(VObject *, f32, f32))(D_0044E4B8, 20.0f, 1000.0f);
        return;
    }
    for (k = 0; k < 4; k++) {
        u8 *data;
        VObject *e;

        if (AT(sec, 0x8 + k * 4, u32) == 0) {
            continue;
        }
        if (!effect_create(o, sSlots[k], classes[k])) {
            continue;
        }
        data = sec + AT(sec, 0x8 + k * 4, u32);
        e = AT(D_0044E4C0, sSlots[k], VObject *);
        if (k == 2) {
            u32 args[7];

            args[0] = AT(data, 0x0, u32);
            args[1] = AT(data, 0x4, u32);
            args[2] = AT(data, 0x8, u32);
            args[3] = AT(data, 0xC, u32);
            args[4] = 0;
            args[5] = 0;
            args[6] = 0;
            if (e != NULL) {
                VCALL(e, 0x18, void (*)(VObject *, void *))(e, args);
            }
        } else if (e != NULL) {
            VCALL(e, 0x18, void (*)(VObject *, void *))(e, data);
        }
    }
}

/* placement new (the effects' copy) */
void *func_002672F0(u32 size, void *place) {
    return place;
}

/* SceneGame +0xF6E200: per-room reset (its heap +0x10000 via +0xC, the 0x400 slots +0x18034) */
void func_002D6100(u8 *o) {
    VObject *heap = (VObject *)(o + 0x10000);
    u32 i;

    VCALL(heap, 0xC, void (*)(VObject *))(heap);
    for (i = 0; i < 0x400; i++) {
        AT(o, 0x18034 + i * 4, s32) = 0;
    }
    AT(o, 0x19034, u8) = 0;
}

/* remove effect `slot` (0..0x3FF): its heap +0x14 (destroy) and the slot emptied */
void func_002D6170(u8 *o, s32 slot) {
    void **p;

    if (slot < 0 || (u32)slot >= 0x400) {
        return;
    }
    p = (void **)(o + 0x18034) + slot;
    if (*p != NULL) {
        VObject *heap = (VObject *)(o + 0x10000);

        VCALL(heap, 0x14, void (*)(VObject *, void *))(heap, *p);
        *p = NULL;
    }
}

extern u8 *D_00420B20[];     /* per map: its rooms (0x18-byte entries, -1 terminated); NULL ends */
extern void **D_0041F950[];  /* per map: its pages (by the entry's +0x4) */

/* SceneGame +0x101EBC0 (the map): find which map and page show room `room` (+0x108 the room,
 * +0x10C/+0x10D the map, +0x10E/+0x10F the page; -1: none) */
void func_00305520(u8 *m, s32 room) {
    s32 i;
    u8 *e;

    AT(m, 0x108, s32) = -1;
    AT(m, 0x10D, s8) = -1;
    AT(m, 0x10C, s8) = -1;
    AT(m, 0x10F, s8) = -1;
    AT(m, 0x10E, s8) = -1;
    if (room == -1 || (u32)room >= 0x110) {
        return;
    }
    for (i = 0; D_00420B20[i] != NULL; i++) {
        for (e = D_00420B20[i]; AT(e, 0, s32) != -1; e += 0x18) {
            if (AT(e, 0, s32) == room && D_0041F950[i] != NULL &&
                D_0041F950[i][AT(e, 4, s8)] != NULL) {
                AT(m, 0x108, s32) = room;
                AT(m, 0x10D, s8) = i;
                AT(m, 0x10C, s8) = i;
                AT(m, 0x10F, s8) = AT(e, 4, s8);
                AT(m, 0x10E, s8) = AT(e, 4, s8);
                return;
            }
        }
    }
}

/* the map turned to page `page` if its room (+0x108) is shown there too */
void func_00303E60(u8 *m, s8 page) {
    u8 *e;

    if (AT(m, 0x108, s32) == -1 || AT(m, 0x10D, s8) == -1 || AT(m, 0x10F, s8) == page) {
        return;
    }
    e = D_00420B20[AT(m, 0x10C, s8)];
    if (e == NULL) {
        return;
    }
    for (; AT(e, 0, s32) != -1; e += 0x18) {
        if (AT(e, 0, s32) == AT(m, 0x108, s32) && AT(e, 4, s8) == page) {
            AT(m, 0x10E, s8) = page;
            AT(m, 0x10F, s8) = page;
            return;
        }
    }
}

/* ---- the map screen (the sub screen's map page): map +0x10C (+0x10D the one the player is
 * in), page +0x10E (+0x10F), room +0x108; the page's picture loaded to +0x140 ---- */

#include "input.h"
#include "progress.h"

extern u16 *D_0041F8B0[];      /* per map: each page's title message */
extern u8 D_00420570[];        /* the alternative room entries (0x18 each) */
extern VObject *D_0044E4E8;    /* the texture cache */
extern VObject *D_0044E560;    /* the sound driver */
extern f32 func_0031C058(f32 x);   /* cosf */
extern f32 func_0031C248(f32 x);   /* sinf */
extern u32 Text_LineWidth(Task *t, u8 *text, s32 glyphW);   /* (a u16, masked here as the original does) */

#define MAP_CUR(m) AT(m, 0x10C, s8)
#define MAP_PAGE(m) AT(m, 0x10E, s8)
#define MAP_PICTURE_AREA 0x6000000   /* the file loader's area for the page's picture */

/* the maps the player has (progress +0x84 bits 22..26 as bits 0..4, as func_00303F00) */
static inline u8 map_owned(void) {
    u32 b = AT(gProgress, 0x84, u32);
    u8 v = 0;

    if (b & 0x400000) {
        v |= 1;
    }
    if (b & 0x800000) {
        v |= 2;
    }
    if (b & 0x1000000) {
        v |= 4;
    }
    if (b & 0x2000000) {
        v |= 8;
    }
    if (b & 0x4000000) {
        v |= 0x10;
    }
    return v;
}

/* can map `map` be shown: one the player has, with pages */
static inline s32 map_shown(s8 map) {
    u8 owned;

    if (map == -1) {
        return 0;
    }
    owned = map_owned();
    if (owned == 0) {
        return 0;
    }
    if (D_0041F950[map] == NULL) {
        return 0;
    }
    return (owned & (1 << map)) ? 1 : 0;
}

/* the current map's last page */
static inline void map_last_page(u8 *m) {
    MAP_PAGE(m) = 0;
    while (D_0041F950[MAP_CUR(m)][MAP_PAGE(m) + 1] != NULL) {
        MAP_PAGE(m)++;
    }
}

/* the first map from 0 that can be shown; none: back to `map` / `page` */
static inline void map_first(u8 *m, s8 map, s8 page) {
    MAP_CUR(m) = 0;
    for (;;) {
        if (D_0041F950[MAP_CUR(m)] == NULL) {
            MAP_CUR(m) = map;
            *(volatile s8 *)&MAP_PAGE(m) = page;   /* (stored again, unchanged, as the original) */
            return;
        }
        if (map_shown(MAP_CUR(m))) {
            return;
        }
        MAP_CUR(m)++;
    }
}

/* Left / right on the map: the previous / next page, past the ends the previous / next map the
 * player has (round), its last / first page. A new page has its picture loaded: 1. */
s32 func_00303F90(u8 *m) {
    u32 pad = D_0047E36C;
    s8 map = MAP_CUR(m), page = MAP_PAGE(m);

    if (pad & MENU_LEFT) {
        if (map_shown(map) && page != 0) {
            MAP_PAGE(m)--;
        } else if (map == -1) {
            map_first(m, map, page);
            if (MAP_CUR(m) == -1) {
                return 0;
            }
            map_last_page(m);
        } else {
            for (;;) {
                if (MAP_CUR(m) != 0) {
                    MAP_CUR(m)--;
                } else {
                    do {
                        MAP_CUR(m)++;
                    } while (D_0041F950[MAP_CUR(m) + 1] != NULL);
                }
                if (MAP_CUR(m) == map || map_shown(MAP_CUR(m))) {
                    break;
                }
            }
            map_last_page(m);
        }
    } else if (pad & MENU_RIGHT) {
        if (map_shown(map) && D_0041F950[map][page + 1] != NULL) {
            MAP_PAGE(m)++;
        } else if (map == -1) {
            map_first(m, map, page);
            if (MAP_CUR(m) == -1) {
                return 0;
            }
            MAP_PAGE(m) = 0;
        } else {
            for (;;) {
                MAP_CUR(m)++;
                if (D_0041F950[MAP_CUR(m)] == NULL) {
                    MAP_CUR(m) = 0;
                }
                if (MAP_CUR(m) == map || map_shown(MAP_CUR(m))) {
                    break;
                }
            }
            MAP_PAGE(m) = 0;
        }
    }
    if (map == MAP_CUR(m) && page == MAP_PAGE(m)) {
        return 0;
    }
    VCALL(gFileLoader, 0xC, void (*)(VObject *, void *, void *, u32, s32))(
        gFileLoader, D_0041F950[MAP_CUR(m)][MAP_PAGE(m)], m + 0x140, MAP_PICTURE_AREA, 0);
    return 1;
}

/* the "you are here" arrow: Fiona's spot on the page of the map she is on (the room's entry:
 * +0x8 / +0xC x / z scale, +0x10 / +0x14 x / y offset; map 2's rooms 0x100..0x105 take theirs
 * from D_00420570 by the events' +0x70), a 32 x 32 arrow (texture group 0x18 #0, 0x1C0, 0x60)
 * turned to her heading, layer 0x30 */
void func_003048C0(u8 *m) {
    s32 room = AT(m, 0x108, s32);
    s8 map = MAP_CUR(m), page = MAP_PAGE(m);
    u8 *e;
    s32 found = 0;
    f32 fx, fy, a, s, c;
    s32 x0, y0, x1, y1, x2, y2, x3, y3;

    if (room == -1 || map == -1 || page == -1 || gCharPlayer == NULL || AT(gCharPlayer, 0x28, u8) == 0) {
        return;
    }
    for (e = D_00420B20[map]; AT(e, 0, s32) != -1; e += 0x18) {
        if (AT(e, 0, s32) == room && AT(e, 4, s8) == page) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return;
    }
    if (map == 2 && (u32)room >= 0x100 && (u32)room < 0x106) {
        s32 n = VCALL(D_0044E4D0, 0x70, s32 (*)(VObject *))(D_0044E4D0);

        if (n < 0) {
            return;
        }
        e = D_00420570 + (n + 0xE) * 0x18;
    }
    fx = AT(e, 0x10, f32) + 512.0f * (AT(gCharPlayer, 0x10, f32) / AT(e, 0x8, f32)) / 640.0f;
    fy = 128.0f + (AT(e, 0x14, f32) + AT(gCharPlayer, 0x18, f32) / AT(e, 0xC, f32));
    a = -AT(gCharPlayer, 0x54, f32);
    /* the corners (-16, 16) (16, 16) (-16, -16) (16, -16) turned by a, x squeezed 512 / 640 */
    s = func_0031C248(a);
    c = func_0031C058(a);
    x0 = (s32)(fx + 512.0f * (-16.0f * c - 16.0f * s) / 640.0f);
    s = func_0031C248(a);
    c = func_0031C058(a);
    y0 = (s32)(fy + (16.0f * c + -16.0f * s));
    s = func_0031C248(a);
    c = func_0031C058(a);
    x1 = (s32)(fx + 512.0f * (16.0f * c - 16.0f * s) / 640.0f);
    s = func_0031C248(a);
    c = func_0031C058(a);
    y1 = (s32)(fy + (16.0f * c + 16.0f * s));
    s = func_0031C248(a);
    c = func_0031C058(a);
    x2 = (s32)(fx + 512.0f * (-16.0f * c - -16.0f * s) / 640.0f);
    s = func_0031C248(a);
    c = func_0031C058(a);
    y2 = (s32)(fy + (-16.0f * c + -16.0f * s));
    s = func_0031C248(a);
    c = func_0031C058(a);
    x3 = (s32)(fx + 512.0f * (16.0f * c - -16.0f * s) / 640.0f);
    s = func_0031C248(a);
    c = func_0031C058(a);
    y3 = (s32)(fy + (-16.0f * c + 16.0f * s));
    VCALL(D_0044E4F0, 0x84, void (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, u32,
                                     s32, s32, s32, s32))(
        D_0044E4F0, x0, y0, x1, y1, x2, y2, x3, y3, 0x1C0, 0x60, 0x20, 0x20, 0x20808080, 0, 0x18, 0x30, 6);
}

/* 1 while the page's picture is still loading; else, when the page can be shown, its
 * picture's texture made resident (texture cache +0x10) */
s32 func_00304D70(u8 *m) {
    if (VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, MAP_PICTURE_AREA) == 2) {
        return 1;
    }
    if (map_shown(MAP_CUR(m))) {
        VCALL(D_0044E4E8, 0x10, void (*)(VObject *, void *, s32))(D_0044E4E8, m + 0x140, 0x28);
    }
    return 0;
}

/* the map page each frame: with any map, left / right (func_00303F90) with a sound on a new
 * page; the page's picture (texture group 0x28) with the arrow when it is Fiona's own page,
 * and its title centred at the top (no map or page: message 0x85) */
void func_00304F50(u8 *m) {
    u8 *text;

    if (func_00304D70(m)) {
        return;
    }
    if (map_owned() != 0 && func_00303F90(m)) {
        VCALL(D_0044E560, 0x14, void (*)(VObject *, s32, s32))(D_0044E560, 0x2A, 5);
        return;
    }
    if (MAP_CUR(m) == -1 || MAP_PAGE(m) == -1 || !map_shown(MAP_CUR(m))) {
        text = Task_MessageText(m + 4, 0x85);
    } else {
        text = Task_MessageText(m + 4, D_0041F8B0[MAP_CUR(m)][MAP_PAGE(m)]);
    }
    if (map_shown(MAP_CUR(m))) {
        if (MAP_CUR(m) != -1 && MAP_PAGE(m) != -1) {
            VCALL(D_0044E4F0, 0x7C, s32 (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32,
                                            s32))(D_0044E4F0, 0, 0x80, 0x200, 0x100, 0, 0, 0x200, 0x100, 0x80808080,
                                                  0, 0x28, 0x30, 0);
        }
        if (MAP_CUR(m) == AT(m, 0x10D, s8) && MAP_PAGE(m) == AT(m, 0x10F, s8)) {
            func_003048C0(m);
        }
    }
    if (text != NULL) {
        u32 w = Text_LineWidth((Task *)(m + 4), text, 0x10) & 0xFFFF;

        Task_ShowText((Task *)(m + 4), 0xA6 - (w >> 1), 0x48, 0x80, text, 0x80, 0x33, 0x10, 0x15);
    }
}

/* back to the map / page the player is in (+0x10D / +0x10F) and, when it can be shown, its
 * picture loaded */
void func_00305380(u8 *m) {
    MAP_CUR(m) = AT(m, 0x10D, s8);
    MAP_PAGE(m) = AT(m, 0x10F, s8);
    if (AT(m, 0x108, s32) == -1 || AT(m, 0x10D, s8) == -1 || AT(m, 0x10F, s8) == -1) {
        return;
    }
    if (!map_shown(MAP_CUR(m))) {
        return;
    }
    VCALL(gFileLoader, 0xC, void (*)(VObject *, void *, void *, u32, s32))(
        gFileLoader, D_0041F950[MAP_CUR(m)][MAP_PAGE(m)], m + 0x140, MAP_PICTURE_AREA, 0);
}

/* ---- the saved game state (0xFC0 bytes, kept at SceneGame +0x48 and in the save): its
 * assignment, as the compiler made it - field by field, the padding left alone ---- */

static void copy_words(u8 *d, const u8 *s, u32 off, u32 n) {
    u32 i;

    for (i = 0; i < n; i++) {
        AT(d, off + i * 4, u32) = AT(s, off + i * 4, u32);
    }
}

/* a sub-record (+0x75C): four words and three bytes */
u8 *func_002A8020(u8 *d, const u8 *s) {
    AT(d, 0x0, s32) = AT(s, 0x0, s32);
    AT(d, 0x4, s32) = AT(s, 0x4, s32);
    AT(d, 0x8, s32) = AT(s, 0x8, s32);
    AT(d, 0xC, s32) = AT(s, 0xC, s32);
    AT(d, 0x10, u8) = AT(s, 0x10, u8);
    AT(d, 0x11, u8) = AT(s, 0x11, u8);
    AT(d, 0x12, u8) = AT(s, 0x12, u8);
    return d;
}

/* a sub-record (+0x7B0) */
u8 *func_002A7F70(u8 *d, const u8 *s) {
    AT(d, 0x0, u8) = AT(s, 0x0, u8);
    AT(d, 0x1, u8) = AT(s, 0x1, u8);
    AT(d, 0x2, s16) = AT(s, 0x2, s16);
    AT(d, 0x4, u32) = AT(s, 0x4, u32);
    AT(d, 0x8, s16) = AT(s, 0x8, s16);
    copy_words(d, s, 0xC, 13);
    AT(d, 0x40, s32) = AT(s, 0x40, s32);
    AT(d, 0x44, u8) = AT(s, 0x44, u8);
    return d;
}

/* the whole state; the copy's 400 words at +0x11C lose their bit 0 */
void func_002A7C70(const u8 *s, u8 *d) {
    u32 i;

    copy_words(d, s, 0x0, 0x94 / 4);
    for (i = 0; i < 0x40; i++) {
        AT(d, 0x94 + i, u8) = AT(s, 0x94 + i, u8);
    }
    copy_words(d, s, 0xD4, (0x75C - 0xD4) / 4);
    func_002A8020(d + 0x75C, s + 0x75C);
    copy_words(d, s, 0x770, 0x10);
    func_002A7F70(d + 0x7B0, s + 0x7B0);
    copy_words(d, s, 0x7F8, (0xFAC - 0x7F8) / 4);
    for (i = 0; i < 8; i++) {
        AT(d, 0xFAC + i * 2, s16) = AT(s, 0xFAC + i * 2, s16);
    }
    for (i = 0; i < 4; i++) {
        AT(d, 0xFBC + i, u8) = AT(s, 0xFBC + i, u8);
    }
    for (i = 0; i < 400; i++) {
        AT(d, 0x11C + i * 4, u32) &= ~1u;
    }
}

/* four bytes cleared */
void func_002A76E0(u8 *p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
}

s32 func_002A8AB0(void) {
    return 0;
}

/* ---- room manager +0x9360 (D_00456E00): the room's triangle groups (PAC section 14: count,
 * then offsets of {n, triangle indices}) whose nav mesh flags scripts switch ---- */


/* set (`clear` 0) or clear (1) flag bits `bits` on the triangles of group `g` (-1: no group) */
s32 func_002A8730(u8 *o, s32 clear, u32 g, u32 bits) {
    u8 *sec = AT(o, 0x4, u8 *);
    u8 *grp;
    u32 n, i;

    if (sec == NULL || g >= AT(o, 0x8, u32)) {
        return -1;
    }
    grp = sec + AT(sec, 0x4 + g * 4, u32);
    n = AT(grp, 0, u32);
    if (clear == 1) {
        for (i = 0; i < n; i++) {
            u32 t = AT(grp, 4 + i * 4, u32);
            u8 *tri = t < AT(D_0044E570, 0x8, u32) && AT(D_0044E570, 0x4, u8 *) != NULL
                          ? AT(D_0044E570, 0x4, u8 *) + t * 0x50 : NULL;

            AT(tri, 0x3C, u32) &= ~bits;
        }
    } else if (clear == 0) {
        for (i = 0; i < n; i++) {
            u32 t = AT(grp, 4 + i * 4, u32);
            u8 *tri = t < AT(D_0044E570, 0x8, u32) && AT(D_0044E570, 0x4, u8 *) != NULL
                          ? AT(D_0044E570, 0x4, u8 *) + t * 0x50 : NULL;

            AT(tri, 0x3C, u32) |= bits;
        }
    }
    return 0;
}

/* +0xC set flag bits on group g */
s32 func_002A8590(u8 *o, u32 g, u32 bits) {
    return func_002A8730(o, 0, g, bits);
}

/* +0x10 clear them */
s32 func_002A85B0(u8 *o, u32 g, u32 bits) {
    return func_002A8730(o, 1, g, bits);
}

extern void *D_0046DB40[], *D_0046DB60[];
extern VObject *D_00456E00;
extern void func_00100490(void *p);   /* operator delete */

/* the nav mesh triangle t's flag word (NULL->flags, as the original, past the end) */
static u32 *tri_flags_word(u32 t) {
    u8 *tri = t < AT(D_0044E570, 0x8, u32) && AT(D_0044E570, 0x4, u8 *) != NULL
                  ? AT(D_0044E570, 0x4, u8 *) + t * 0x50 : NULL;

    return &AT(tri, 0x3C, u32);
}

/* +0x18 set flag bits on triangle t (-1: none, 0 done) */
s32 func_002A85D0(u8 *o, u32 t, u32 bits) {
    if (t == (u32)-1 || t >= AT(D_0044E570, 0x8, u32)) {
        return -1;
    }
    *tri_flags_word(t) |= bits;
    return 0;
}

/* +0x1C clear them */
s32 func_002A8640(u8 *o, u32 t, u32 bits) {
    if (t == (u32)-1 || t >= AT(D_0044E570, 0x8, u32)) {
        return -1;
    }
    *tri_flags_word(t) &= ~bits;
    return 0;
}

/* +0x14 whether group g holds triangle t */
s32 func_002A86B0(u8 *o, u32 t, u32 g) {
    u8 *sec, *grp;
    u32 n, i;

    if (g >= AT(o, 0x8, u32)) {
        return 0;
    }
    sec = AT(o, 0x4, u8 *);
    grp = sec + AT(sec, 0x4 + g * 4, u32);
    n = AT(grp, 0, u32);
    for (i = 0; i < n; i++) {
        if (AT(grp, 4 + i * 4, u32) == t) {
            return 1;
        }
    }
    return 0;
}

/* +0x8 destructor */
void *func_002A8520(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046DB40;
        AT(o, 0x0, void **) = D_0046DB60;
        D_00456E00 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the base's destructor */
void *func_002A88D0(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046DB60;
        D_00456E00 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

#include "progress.h"

extern s32 func_00178610(Progress *p, u32 d);
extern s32 func_00178200(Progress *p, u32 d, s32 side);

/* the doors +0x80: refresh exit `exit`'s door from its state in the progress: locked (+0x40
 * says it can be) -> flags 0x5000000, else 0x1000000 / 0x4000000 per side closed */
void func_002220C0(VObject *doors, s32 exit) {
    Progress *p = gProgress;
    s32 room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    u32 d;
    u32 flags = 0;

    if ((u8)VCALL(D_0044E558, 0x40, s32 (*)(VObject *, s32))(D_0044E558, exit) != 1) {
        return;
    }
    d = VCALL(D_0044E568, 0x10, u32 (*)(VObject *, s32, s32))(D_0044E568, room, exit) & 0xFFFF;
    if (d == 0xFFFF) {
        return;
    }
    VCALL(doors, 0x20, void (*)(VObject *, s32, s32, u32))(doors, exit, 0, 0x5000000);
    if ((u8)func_00178610(p, d) == 1) {
        flags = 0x5000000;
    } else {
        if (!(u8)func_00178200(p, d, 1)) {
            flags = 0x1000000;
        }
        if (!(u8)func_00178200(p, d, 2)) {
            flags |= 0x4000000;
        }
    }
    if (flags != 0) {
        VCALL(doors, 0x1C, void (*)(VObject *, s32, s32, u32))(doors, exit, 0, flags);
    }
}

/* the doors +0x40: whether the room has a door at exit `exit` (section 7's entry) */
s32 func_00221DA0(VObject *doors, u8 exit) {
    u8 *tbl = AT(doors, 0x4, u8 *);

    if (tbl == NULL || exit >= 8) {
        return 0;
    }
    return AT(tbl, exit * 4, s32) != 0;
}

/* the doors +0x84: set or clear bit (a + 1 + b) in each present door's bits (+0x120) */
void func_00221FD0(VObject *doors, s32 set, u8 a, s32 b) {
    u32 bit = (u32)(a + 1) + b;
    u32 mask = 1 << (bit & 0x1F);
    u8 *base = (u8 *)doors + (bit >> 5) * 4;
    u8 i;

    for (i = 0; i < 8; i++) {
        u8 *tbl = AT(doors, 0x4, u8 *);

        if (tbl == NULL || AT(tbl, i * 4, s32) == 0) {
            continue;
        }
        if (set) {
            AT(base + i * 0x210, 0x120, u32) |= mask;
        } else {
            AT(base + i * 0x210, 0x120, u32) &= ~mask;
        }
    }
}

/* SceneGame +0xF6CD30: all 32 effect slots back to the pool (+0x1400) */
void func_00267080(u8 *fx) {
    s32 n;

    for (n = 0; n < 0x20; n++) {
        VObject **slot = &AT(fx, 0x1438 + n * 4, VObject *);

        if (*slot != NULL) {
            VCALL((VObject *)(fx + 0x1400), 0x14, void (*)(VObject *, void *))((VObject *)(fx + 0x1400), *slot);
            *slot = NULL;
        }
    }
}

/* SceneGame +0xF6CD30: effect slot n (+0x1438[n]) back to the pool (+0x1400) */
void func_002670F0(u8 *fx, s32 n) {
    VObject **slot;

    if (n >= 0x20) {
        return;
    }
    slot = &AT(fx, 0x1438 + n * 4, VObject *);
    if (*slot != NULL) {
        VCALL((VObject *)(fx + 0x1400), 0x14, void (*)(VObject *, void *))((VObject *)(fx + 0x1400), *slot);
        *slot = NULL;
    }
}

/* SceneGame +0xF6CD30: pass `arg` to effect slot n (+0x1438[n], its +0x18); 0: none */
s32 func_00266C70(u8 *o, s32 n, void *arg) {
    VObject *e;

    if (n >= 0x20) {
        return 0;
    }
    e = AT(o, 0x1438 + n * 4, VObject *);
    if (e == NULL) {
        return 0;
    }
    VCALL(e, 0x18, void (*)(VObject *, void *))(e, arg);
    return 1;
}

/* placement new (the effect manager's copy) */
void *func_002D63C0(u32 size, void *place) {
    return place;
}

/* the effect manager: start the object in slot `slot` with `params` (its +0x18); 0: no object */
s32 func_002D6090(u8 *mgr, s32 slot, void *params) {
    VObject *o;

    if (slot < 0 || (u32)slot >= 0x400) {
        return 0;
    }
    o = AT(mgr, 0x18034 + slot * 4, VObject *);
    if (o == NULL) {
        return 0;
    }
    VCALL(o, 0x18, void (*)(VObject *, void *))(o, params);
    return 1;
}

/* SceneGame +0x706480: the active creatures (10 slots) enter the room (+0x38) */
void func_002E2650(u8 *o) {
    s32 i;

    for (i = 0; i < 10; i++) {
        VObject *c = AT(o, i * 4, VObject *);

        if (c != NULL && AT(c, 0x28, u8) == 1) {
            VCALL(c, 0x38, void (*)(VObject *))(c);
        }
    }
}

/* the doors +0x2C: whether `pos` is at door k (within 5 vertically and 20 across of its point
 * +0x20) */
s32 func_00222D30(VObject *doors, u32 k, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));
    u8 *tbl = AT(doors, 0x4, u8 *);
    f32 dy;

    k &= 0xFF;
    if (tbl == NULL || k >= 8 || AT(tbl, k * 4, s32) == 0) {
        return 0;
    }
    sceVu0SubVector(v, (f32 *)((u8 *)doors + k * 0x210 + 0x30), (f32 *)pos);
    dy = v[1];
    if (dy <= 0.0f) {
        dy = -dy;
    }
    if (!(dy <= 5.0f)) {
        return 0;
    }
    return __builtin_sqrtf(v[2] * v[2] + v[0] * v[0]) <= 20.0f;
}

extern void func_002E3190(f32 m[4][4], f32 angle);           /* rotation about y */
extern void func_002E2DA0(f32 *out, f32 m[4][4], const f32 *v);   /* m * v */

/* the doors +0x18: which side of door k `pos` is on (1: in front, along its facing +0x44; 0:
 * behind; -1: no door) */
s32 func_002230A0(VObject *doors, u32 k, const f32 *pos) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 diff[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    u8 *tbl = AT(doors, 0x4, u8 *);
    u8 *e;

    k &= 0xFF;
    if (tbl == NULL || k >= 8 || AT(tbl, k * 4, s32) == 0) {
        return -1;
    }
    e = (u8 *)doors + k * 0x210 + 0x10;
    dir[0] = 0.0f;
    dir[1] = 0.0f;
    dir[2] = 1.0f;   /* (w is left unset, as in the original) */
    func_002E3190(m, AT(e, 0x44, f32));
    func_002E2DA0(dir, m, dir);
    sceVu0SubVector(diff, (f32 *)pos, (f32 *)(e + 0x20));
    return !(sceVu0InnerProduct(dir, diff) < 0.0f);
}

extern VObject *D_00456DF0;
extern u8 *D_0044E958;
extern u8 *D_0044E980;   /* the BGM player */
extern VObject *D_0044E560;   /* the sound driver */
extern void func_002EF9E0(void *o);
extern void func_002D1FD0(void *bgm);
extern void func_002B6340(void *movie);

static f32 clamp01(f32 v) {
    if (v < 0.0f) {
        v = 0.0f;
    }
    return v <= 1.0f ? v : 1.0f;
}

/* SceneGame +0xA20 per frame: a timed heal (+0x0 for +0x4 frames), a timed reset of +0x8, the
 * volume fade (+0x10: in / out over 30 frames of +0x18, +0x14 left) passed to the sound, the
 * menus and the movie player, a timed heart-rate setting (+0x1C for +0x20 frames) and two more
 * timers */
void func_002A7720(u8 *o) {
    f32 old;

    if (AT(o, 0x4, s32) > 0) {
        AT(o, 0x4, s32)--;
        if (!(AT(o, 0x0, f32) < 0.0f)) {
            func_002EF9E0((u8 *)gProgress + 0x7B8);
        } else {
            f32 x = AT(o, 0x0, f32);

            if (x <= 0.0f) {
                x = -x;
            }
            if (!(x < 0.0f)) {
                AT(gProgress, 0x7E4, f32) = AT(gProgress, 0x7E4, f32) + x;
            }
        }
    }
    if (AT(o, 0xC, s32) > 0) {
        AT(o, 0xC, s32)--;
        if (AT(o, 0xC, s32) <= 0) {
            AT(o, 0x8, f32) = 1.0f;
        }
    }
    old = AT(o, 0x10, f32);
    if (AT(o, 0x14, s32) != 0) {
        s32 left = AT(o, 0x14, s32);
        s32 total = AT(o, 0x18, s32);

        AT(o, 0x10, f32) = 0.0f;
        if (!(left < total - 30)) {
            AT(o, 0x10, f32) = 1.0f - (f32)(total - left) / 30.0f;
        } else if (left < 31) {
            AT(o, 0x10, f32) = 1.0f - (f32)left / 30.0f;
        }
        AT(o, 0x10, f32) = clamp01(AT(o, 0x10, f32));
        AT(o, 0x14, s32)--;
    } else {
        AT(o, 0x10, f32) = 1.0f;   /* no fade: full volume */
    }
    if (old != AT(o, 0x10, f32)) {
        VCALL(D_0044E560, 0xAC, void (*)(VObject *, f32))(D_0044E560, AT(o, 0x10, f32));
        AT(D_0044E980, 0x120, f32) = clamp01(AT(o, 0x10, f32));
        func_002D1FD0(D_0044E980);   /* the music player, passed through a0 */
        if (D_00456DF0 != NULL) {
            VCALL(D_00456DF0, 0x24, void (*)(VObject *, f32))(D_00456DF0, AT(o, 0x10, f32));
        }
        if (D_0044E958 != NULL) {
            AT(D_0044E958, 0x1D4, f32) = clamp01(AT(o, 0x10, f32));
            func_002B6340(D_0044E958);   /* the movie, passed through a0 */
        }
    }
    if (AT(o, 0x20, s32) != 0) {
        AT(o, 0x20, s32)--;
        if (gCharPlayer != NULL) {
            f32 v = AT(o, 0x1C, f32);   /* her heart rate */

            if (v < 0.0f) {
                v = 0.0f;
            } else if (!(v <= 100.0f)) {
                v = 100.0f;
            }
            AT(gCharPlayer, 0x1AD5F4, f32) = v;
        }
    }
    if (AT(o, 0x28, s32) != 0) {
        AT(o, 0x28, s32)--;
    } else {
        AT(o, 0x24, f32) = 1.0f;
    }
    if (AT(o, 0x2C, s32) != 0) {
        AT(o, 0x2C, s32)--;
    }
    if (AT(o, 0x30, s32) != 0) {
        AT(o, 0x30, s32)--;
    }
}

extern VObject *D_0044E4F8;   /* the camera director (interface) */
extern s32 func_0029A8C0(u8 *pu, s32);
extern u32 func_0029CE50(u8 *pu);
extern s32 func_002EC170(u8 *o);
extern s32 func_002EBED0(u8 *o);
extern s32 func_00177200(Progress *p, u32 slot);

/* the summoner `o` takes the pursuer as it is now (when active: its room +0x8, its state +0,
   kind `kind`, waited 0); then progress slot refresh (func_00177200) */
static inline void summoner_take(u8 *o, u8 kind) {
    u8 *pu = gCharPursuer;

    if (pu == NULL) {
        return;
    }
    if (AT(pu, 0x28, u8)) {
        AT(o, 0x8, s32) = AT(pu, 0x30, s32);
        AT(o, 0x0, u32) = func_0029CE50(pu) & 0x7FFFFFFF;
        AT(o, 0xC, s32) = 0;
        AT(o, 0x11, u8) = kind;
    }
    func_00177200(gProgress, AT(pu, 0x20, u32));
}

void func_002EC470(u8 *o, u8 kind) {
    summoner_take(o, kind);
}

/* the summoner's cooldown (+0x4) to `sec` seconds / less by `sec` (not below 0) */
void func_002EC450(u8 *o, s32 sec) {
    AT(o, 0x4, u32) = sec * 30;
}

void func_002EC3C0(u8 *o, s32 sec) {
    u32 d = sec * 30;

    AT(o, 0x4, u32) = AT(o, 0x4, u32) >= AT(o, 0x4, u32) - d ? AT(o, 0x4, u32) - d : 0;
}

extern VObject *D_0044E550;    /* random numbers: +0x18 / +0x1C -> 0..1 */
extern void *func_00114FA8(u32 size);   /* malloc */
extern void func_00114FD0(void *p);     /* free */
extern s32 func_00126F30(void *c, s32 from, s32 to, s32 a3, s32 a4, s32 fromPt, s32 toPt, s32 mode);   /* a route (>= 0) */
extern void func_0029CEE0(void *pu, s32 room, u32 found, s32 plan, s32 side);   /* the pursuer comes into a room */

/* bring the pursuer in for the summoner `o`: a random room out of the progress's list for the
 * current one (+0x3C) - `near` 0: one not next to it; else one next to it (once per exit
 * leading there) that the rooms allow (+0x88); either way not the current room and with its
 * progress bit clear - then the first of its exits (rooms +0x74) with a route from Fiona's
 * exit point to the exit's point (func_00126F30; next-door: route mode 1, the point not 0 / 1;
 * else mode 2); it comes in there with plan `plan` (and +0's summoned bit). 1 if it came */
static s32 summon_via(u8 *o, s32 near, s32 plan) {
    Progress *p = gProgress;
    VObject *rooms = D_0044E568;
    s32 cur = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    s32 *list = func_00114FA8(0x104);
    u32 n, m = 0, i, e;
    u8 *pu, *pl;
    s32 from, to = -1, ok = 0;

    if (list == NULL) {
        return 0;
    }
    n = VCALL(p, 0x3C, u32 (*)(Progress *, s32 *, s32))(p, list, cur);
    if (n == 0) {
        func_00114FD0(list);
        return 0;
    }
    for (i = 0; i < n; i++) {
        s32 c = list[i];

        if (c == -1 || c == cur) {
            continue;
        }
        if (!near) {
            for (e = 0; e < 8; e++) {
                if (list[i] == VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, cur, e & 0xFF)) {
                    break;
                }
            }
            if (e == 8 && (u8)Progress_IsBitClear(p, list[i])) {
                list[m++] = list[i];
            }
        } else if ((u8)Progress_IsBitClear(p, c) && (u8)VCALL(rooms, 0x88, s32 (*)(VObject *, s32, s32))(rooms, list[i], cur)) {
            for (e = 0; e < 8; e++) {
                if (list[i] == VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, cur, e & 0xFF)) {
                    list[m++] = list[i];
                }
            }
        }
    }
    if (m == 0) {
        func_00114FD0(list);
        return 0;
    }
    i = (u8)(u32)((f32)m * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550));
    pu = gCharPursuer;
    pl = gCharPlayer;
    from = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, AT(pl, 0x30, s32), AT(pl, 0x14D4, u8), 1);
    for (e = 0; e < 8; e++) {
        if (VCALL(rooms, 0x74, s32 (*)(VObject *, s32, u32))(rooms, list[i], e & 0xFF) == 0) {
            continue;
        }
        to = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, list[i], e & 0xFF, 1);
        if (func_00126F30(pu, AT(pl, 0x30, s32), list[i], AT(pu, 0x20, s32), 1, from, to, near ? 1 : 2) >= 0 &&
            (!near || (to != 0 && to != 1))) {
            ok = 1;
            break;
        }
    }
    if (ok) {
        func_0029CEE0(pu, list[i], (AT(o, 0x0, u32) & 0x80000000) != 0, plan, to);
    }
    func_00114FD0(list);
    return ok;
}

s32 func_002EB390(u8 *o) {
    return summon_via(o, 0, 0);
}

s32 func_002EB730(u8 *o) {
    return summon_via(o, 1, 0);
}

s32 func_002EBB00(u8 *o) {
    return summon_via(o, 1, 1);
}

/* each frame with the pursuer offstage (Progress flag 0: this stage has one): the first time
 * (flag 1) +0 its time away; while flag 2 is clear and that runs, counting down (+0xC counting
 * up); then it tries to come in, by one of the three ways from a random first one onwards
 * (func_002EBB00 / func_002EB730 / func_002EB390); having come, the wait restarts, +0x10 its
 * mode, flag 2 off and the cooldown +0x4 its +0x2D4 seconds. 1 if it came */
s32 func_002EC170(u8 *o) {
    u8 *pu = gCharPursuer;
    Progress *p = gProgress;
    u32 k;
    s32 ok = 0;

    if (!Progress_TestFlag(p, 0)) {
        return 0;
    }
    if (!Progress_TestFlag(p, 1)) {
        AT(o, 0x0, u32) = func_0029CE50(pu);
        AT(o, 0xC, u32) = 0;
        Progress_SetFlag(p, 1);
    }
    if (AT(pu, 0x28, u8) != 0) {
        return 0;
    }
    if (!Progress_TestFlag(p, 2) && (AT(o, 0x0, u32) & 0x7FFFFFFF) != 0) {
        AT(o, 0x0, u32)--;
        if (AT(o, 0xC, u32) + 1 != 0) {
            AT(o, 0xC, u32)++;
        }
        return 0;
    }
    k = (u8)(u32)(3.0f * VCALL(D_0044E550, 0x18, f32 (*)(VObject *))(D_0044E550));
    do {
        switch (k++) {
        case 0:
            ok = (u8)func_002EBB00(o);
            break;
        case 1:
            ok = (u8)func_002EB730(o);
            break;
        case 2:
            ok = (u8)func_002EB390(o);
            break;
        }
    } while ((k & 0xFF) < 3 && !ok);
    if (ok == 1) {
        AT(o, 0xC, u32) = 0;
        AT(o, 0x10, u8) = AT(pu, 0x16C8, u8);
        Progress_ClearFlag(p, 2);
        AT(o, 0x4, u32) = VCALL((VObject *)pu, 0x2D4, s32 (*)(void *))(pu) * 30;
    }
    return ok;
}

/* each frame with the pursuer in play: the cooldown runs down; its mode (+0x16C8) changed: the
 * wait restarts; Fiona in its room, or next door where it isn't hunting her (progress +0x64
 * not 4 or func_0029A8C0): likewise; else the wait counts up, and after 5 s hunting (+0xC4 2:
 * kind 2) or in mode 4 (kind 1), or with the cooldown over 3 s in mode 3 (kind 0), the
 * summoner takes it back. 1 if it did */
s32 func_002EBED0(u8 *o) {
    u8 *pu = gCharPursuer;
    u8 *pl;
    u8 mode;
    s32 go = 0, kind = 0;

    if (pu == NULL || AT(pu, 0x28, u8) == 0) {
        return 0;
    }
    if (AT(o, 0x4, u32) != 0) {
        AT(o, 0x4, u32)--;
    }
    mode = AT(pu, 0x16C8, u8);
    if (mode != AT(o, 0x10, u8)) {
        AT(o, 0x10, u8) = mode;
        AT(o, 0xC, u32) = 0;
        return 0;
    }
    pl = gCharPlayer;
    if (pl != NULL && AT(pl, 0x28, u8) != 0 && AT(pl, 0x30, s32) != -1) {
        s32 pr = AT(pl, 0x30, s32), ur = AT(pu, 0x30, s32);
        u32 e;

        if (pr == ur) {
            AT(o, 0xC, u32) = 0;
            return 0;
        }
        for (e = 0; e < 8; e++) {
            if (ur == VCALL(D_0044E568, 0x18, s32 (*)(VObject *, s32, u32))(D_0044E568, pr, e & 0xFF) &&
                ((u8)VCALL(gProgress, 0x64, s32 (*)(Progress *))(gProgress) != 4 || !func_0029A8C0(pu, -1))) {
                AT(o, 0xC, u32) = 0;
                return 0;
            }
        }
    }
    if (AT(o, 0xC, u32) + 1 != 0) {
        AT(o, 0xC, u32)++;
    }
    if (AT(pu, 0xC4, s32) == 2 && AT(o, 0xC, u32) >= 151) {
        go = 1;
        kind = 2;
    }
    if (!go && mode == 4 && AT(o, 0xC, u32) >= 151) {
        go = 1;
        kind = 1;
    }
    if (!go && AT(o, 0x4, u32) != 0) {
        return 0;
    }
    if (!go && mode == 3 && AT(o, 0xC, u32) >= 91) {
        go = 1;
        kind = 0;
    }
    if (go) {
        summoner_take(o, kind);
    }
    return go;
}

/* SceneGame +0x7A4 at the start of play in a room: whether the pursuer comes in (the progress
 * +0x28 says the room allows it; then by the stage, +0x64 4) or is placed elsewhere */
void func_002EC940(u8 *o) {
    u8 *pu = gCharPursuer;
    Progress *p, *q;

    if (pu == NULL || !(AT(pu, 0xD0, u8) != 0 || AT(pu, 0xD1, u8) != 0)) {
        return;
    }
    p = gProgress;
    if (Progress_TestFlag(p, 0x17)) {
        return;
    }
    if (Progress_TestFlag(p, 0x18) && AT(pu, 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return;
    }
    if (AT(p, 0x1FBEC1, u8) != 0) {
        return;
    }
    q = gProgress;
    if (VCALL(q, 0x28, s32 (*)(Progress *, s32, s32))(q, VCALL(q, 0xC, s32 (*)(Progress *))(q), 2)) {
        if (AT(gCharPlayer, 0x30, s32) == AT(pu, 0x30, s32)) {
            return;
        }
        if ((u8)VCALL(p, 0x64, s32 (*)(Progress *))(p) != 4) {
            return;
        }
        if (!func_0029A8C0(pu, -1)) {
            return;
        }
        summoner_take(o, 1);
        return;
    }
    if (VCALL(D_0044E4F8, 0x38, s32 (*)(VObject *))(D_0044E4F8)) {
        return;
    }
    if (!(u8)func_002EC170(o)) {
        func_002EBED0(o);
    }
}

/* the lights +0x34 (each frame): clear +0x940 */
void func_001FA0D0(u8 *o) {
    AT(o, 0x940, s32) = 0;
}

/* clear a character request: { u8 kind, s32 a = -1, s32 b = -1, u16 c = 0xFFFF } */
void func_002A84A0(u8 *r) {
    AT(r, 0x0, u8) = 0;
    AT(r, 0x4, s32) = -1;
    AT(r, 0x8, s32) = -1;
    AT(r, 0xC, u16) = 0xFFFF;
}

/* clear a character's own request: { u8 kind, u16, s16, f32 } */
void func_002A84E0(u8 *r) {
    AT(r, 0x0, u8) = 0;
    AT(r, 0x2, u16) = 0;
    AT(r, 0x4, u16) = 0;
    AT(r, 0x8, s32) = 0;
}


extern void *func_00266C40(void *effects, s32 kind);   /* the effect of a kind, if any */
extern void func_002239C0(void *doors);
extern void func_0021AC10(void *o);

/* draw the room, each frame: the room mesh's parts (PAC section header +0x998C: offsets of
 * the opaque part, two alpha parts drawn in layer 8 under the 0x1D effect (fog) else 0x19,
 * and the parts for layers 0x26 and 0x1F) are queued with the renderer (+0xC) as the room
 * object (+0x8 mesh, +0x18 part); the lights are reset (+0x18) before each pass. Then the
 * extra object +0x340 (flag 0x80), the doors, +0x9380 and +0x6740. */
void func_0011FB20(u8 *rm, s32 slot) {
    VObject *lights;
    void *fx;
    s32 o0, o1, o2, o4, o5;
    s32 layer;

    if (AT(rm, 0x998C, u8 *) == NULL) {
        return;
    }
    lights = D_0044E4E8;
    VCALL(lights, 0x18, void (*)(VObject *))(lights);
    AT(rm, 0x64, s32) = -1;
    o0 = AT(AT(rm, 0x998C, u8 *), 0x0, s32);
    o2 = AT(AT(rm, 0x998C, u8 *), 0x8, s32);
    o4 = AT(AT(rm, 0x998C, u8 *), 0x10, s32);
    o5 = AT(AT(rm, 0x998C, u8 *), 0x14, s32);
    o1 = AT(AT(rm, 0x998C, u8 *), 0x4, s32);
    if (o0 != 0) {
        AT(rm, 0x8, u8 *) = AT(rm, 0x998C, u8 *) + o0;
        AT(rm, 0x18, s32) = 0;
        VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, rm, 1, 0);
    }
    VCALL(lights, 0x18, void (*)(VObject *))(lights);
    AT(rm, 0x64, s32) = -1;
    fx = D_0044E4C0 != NULL ? func_00266C40(D_0044E4C0, 0x1D) : NULL;
    layer = (fx != NULL && AT(fx, 0x1C, s32) != 0) ? 8 : 0x19;
    if (o1 != 0) {
        AT(rm, 0x8, u8 *) = AT(rm, 0x998C, u8 *) + o1;
        AT(rm, 0x18, s32) = 1;
        VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, rm, layer, 0);
    }
    if (o4 != 0) {
        AT(rm, 0x8, u8 *) = AT(rm, 0x998C, u8 *) + o4;
        AT(rm, 0x18, s32) = 4;
        VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, rm, layer, 0);
        AT(rm, 0x8B, u8) = 1;
    }
    if (o2 != 0) {
        VCALL(lights, 0x18, void (*)(VObject *))(lights);
        AT(rm, 0x64, s32) = -1;
        AT(rm, 0x8, u8 *) = AT(rm, 0x998C, u8 *) + o2;
        AT(rm, 0x18, s32) = 2;
        VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, rm, 0x26, 0);
    }
    if (o5 != 0) {
        VCALL(lights, 0x18, void (*)(VObject *))(lights);
        AT(rm, 0x64, s32) = -1;
        AT(rm, 0x8, u8 *) = AT(rm, 0x998C, u8 *) + o5;
        AT(rm, 0x18, s32) = 5;
        VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, rm, 0x1F, 0);
    }
    VCALL(lights, 0x18, void (*)(VObject *))(lights);
    AT(rm, 0x358, s32) = -1;
    if (AT(rm, 0x35C, u8) & 0x80) {
        VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, rm + 0x340, 1, 0);
    }
    func_002239C0(rm + 0x1640);
    func_0021AC10(rm + 0x9380);
    VCALL(rm + 0x6740, 0x24, void (*)(void *))(rm + 0x6740);
}


/* the creatures, each frame (10 slots): an active one moves (+0x30) unless the player is in a
 * special state; an inactive one gets +0x10, then the manager's +0x28 (its vtable at +0x28)
 * for its slot */
void func_002E2A60(u8 *o) {
    s32 i;

    for (i = 0; i < 10; i++) {
        VObject *c = AT(o, i * 4, VObject *);

        if (c == NULL) {
            continue;
        }
        if (AT(c, 0x28, u8) != 0) {
            if (AT(gCharPlayer, 0xE2, u8) == 0) {
                VCALL(c, 0x30, void (*)(VObject *))(c);
            }
        } else {
            VCALL(c, 0x10, void (*)(VObject *))(c);
            ((void (*)(u8 *, s32))AT(AT(o, 0x28, u8 *), 0x28, void *))(o, i & 0xFF);
        }
    }
}


extern void func_00223A90(void *doors);
extern void func_0021AFD0(void *o);

extern void func_002238F0(VObject *doors);   /* the doors released */
extern void func_0021ABC0(u8 *obstacles);   /* the obstacles released */
extern void func_00267080(u8 *fx);           /* all the room's effects back to the pool */

/* the room left: its doors, obstacles, placed objects (+0x6740 +0x20) and effects released,
 * and the texture cache's room groups (0, 0x15) dropped */
void func_0011FF30(u8 *rm) {
    VObject *tc;

    func_002238F0((VObject *)(rm + 0x1640));
    func_0021ABC0(rm + 0x9380);
    VCALL((VObject *)(rm + 0x6740), 0x20, void (*)(VObject *))((VObject *)(rm + 0x6740));
    func_00267080((u8 *)D_0044E4C0);
    tc = D_0044E4E8;
    VCALL(tc, 0x14, void (*)(VObject *, s32))(tc, 0);
    VCALL(tc, 0x14, void (*)(VObject *, s32))(tc, 0x15);
}

/* the room, each frame: the doors (with a room loaded), +0x9380, +0x6740 (+0x28), and
 * whether Progress +0x54 lets the alpha parts show (+0x8B) */
void func_0011FEB0(u8 *rm) {
    if (AT(rm, 0x998C, u8 *) != NULL) {
        func_00223A90(rm + 0x1640);
    }
    func_0021AFD0(rm + 0x9380);
    VCALL(rm + 0x6740, 0x28, void (*)(void *))(rm + 0x6740);
    AT(rm, 0x8B, u8) = VCALL(gProgress, 0x54, s32 (*)(void *))(gProgress);
}


extern void func_00221300(u8 *door);

/* the doors, each frame: each of the 8 with a definition (+0x4 table) updates */
void func_00223A90(void *d) {
    u8 *doors = d;
    u32 k;

    if (AT(doors, 0x4, void **) == NULL) {
        return;
    }
    for (k = 0; k < 8; k++) {
        void **tbl = AT(doors, 0x4, void **);

        if (tbl != NULL && tbl[k] != NULL) {
            func_00221300(doors + 0x10 + k * 0x210);
        }
    }
}


extern void func_0025FA50(u8 *obj);

/* +0x28 the placed objects, each frame: every active one (bit set in +0xC, 64 slots of 0xB0
 * from +0x20) updates */
void func_002C8DC0(u8 *o) {
    s32 i;

    for (i = 0; i < 64; i++) {
        if (AT(o, 0xC + (i >> 5) * 4, u32) & (1u << (i & 0x1F))) {
            func_0025FA50(o + 0x20 + i * 0xB0);
        }
    }
}


extern void func_001F36B0(void *track, f32 *out, f32 t);

/* a placed object's animation, each frame: its track (+0x98 { keys, format, count }) at the
 * frame +0x90 gives its position (+0x10; format 9: +0x20); at the end it loops (+0x1) or
 * stops */
void func_0025FA50(u8 *obj) {
    if (AT(obj, 0x94, s32) == 0 || AT(obj, 0x98, void *) == NULL) {
        return;
    }
    if (AT(obj, 0x9C, u16) == 9) {
        func_001F36B0(obj + 0x98, (f32 *)(obj + 0x20), (f32)AT(obj, 0x90, s32));
    } else {
        func_001F36B0(obj + 0x98, (f32 *)(obj + 0x10), (f32)AT(obj, 0x90, s32));
    }
    AT(obj, 0x90, s32)++;
    if (AT(obj, 0x90, s32) < AT(obj, 0xA0, s32)) {
        return;
    }
    if (AT(obj, 0x1, u8) != 0) {
        AT(obj, 0x90, s32) = 0;
    } else {
        AT(obj, 0x94, s32) = 0;
        AT(obj, 0xA0, s32) = 0;
        AT(obj, 0x98, s32) = 0;
        AT(obj, 0x9C, s32) = 0;
    }
}


/* the play time { hours, minutes, seconds, frames (30 a second) }, each frame; it stops at
 * 99:59:59 */
void func_002A7630(u8 *t) {
    if (t[0] == 99 && t[1] == 59 && t[2] == 59) {
        return;
    }
    if (++t[3] < 30) {
        return;
    }
    t[3] = 0;
    if (++t[2] < 60) {
        return;
    }
    t[2] = 0;
    if (++t[1] < 60) {
        return;
    }
    t[1] = 0;
    t[0]++;
}


extern VObject *func_00120D60(void *pool, s32 i);   /* the pool's object i (NULL if free) */

/* the dynamic actors, each frame (128 slots in the pool +0xA040): an active one updates
 * (+0x30), a finished one is given back to the pool (+0x14) and destroyed */
void func_002D75C0(u8 *o) {
    s32 i;

    for (i = 0; i < 0x80; i++) {
        VObject *a = func_00120D60(o + 0xA040, i);

        if (a == NULL) {
            continue;
        }
        if (AT(a, 0x28, u8) != 0) {
            VCALL(a, 0x30, void (*)(VObject *))(a);
        } else {
            VCALL(o + 0xA040, 0x14, void (*)(void *, VObject *))(o + 0xA040, a);
            if (a != NULL) {
                VCALL(a, 0x8, void (*)(VObject *, s32))(a, 1);
            }
        }
    }
}


/* the effects, each frame: each of the 32 at +0x1438 updates (+0x10) */
void func_002671F0(u8 *o) {
    s32 i;

    for (i = 0; i < 32; i++) {
        VObject *e = AT(o, 0x1438 + i * 4, VObject *);

        if (e != NULL) {
            VCALL(e, 0x10, void (*)(VObject *))(e);
        }
    }
}


extern s32 func_002CC5A0(u8 *o, s32 group);
extern s32 func_002C9930(u8 *o, u32 k);

/* +0x80 the group for slot `i`: groups 1 and 2 and those (3..31) any entry of the table +0x18
 * ({u16 count at +2}, masks every 12 bytes from +0x24) uses, in order; looked up through
 * func_002CC5A0 and func_002C9930. Without the director's +0x38 or a table: i itself */
s32 func_002C9730(u8 *o, s32 i) {
    s32 list[32];
    s32 n = 0, b;

    if (D_0044E4F8 == NULL || !VCALL(D_0044E4F8, 0x38, s32 (*)(void *))(D_0044E4F8) ||
        AT(o, 0x18, u8 *) == NULL) {
        return i;
    }
    for (b = 1; b < 32; b++) {
        u32 mask = 0;
        s32 k, cnt;

        if (b == 1 || b == 2) {
            list[n++] = b;
            continue;
        }
        cnt = AT(AT(o, 0x18, u8 *), 0x2, u16);
        for (k = 0; k < cnt; k++) {
            mask |= AT(AT(o, 0x18, u8 *), 0x24 + k * 12, u32);
        }
        if (mask & (1u << b)) {
            list[n++] = b;
        }
    }
    return (u8)func_002C9930(o, (u8)func_002CC5A0(o, list[i & 0xFF]));
}


/* the effects, each frame: each live one (0x400 slots at +0x18034) runs (+0x10); a finished
 * one goes back to the heap (+0x10000, +0x14) */
void func_002D6280(u8 *mgr) {
    u32 i;

    for (i = 0; i < 0x400; i++) {
        VObject *e = AT(mgr, 0x18034 + i * 4, VObject *);

        if (e == NULL || (u8)VCALL(e, 0x10, s32 (*)(VObject *))(e)) {
            continue;
        }
        VCALL(mgr + 0x10000, 0x14, void (*)(void *, VObject *))(mgr + 0x10000, AT(mgr, 0x18034 + i * 4, VObject *));
        AT(mgr, 0x18034 + i * 4, VObject *) = NULL;
    }
}


/* the effect in slot k (of 32 at +0x1438), NULL past the end */
void *func_00266C40(void *effects, s32 k) {
    u8 *o = effects;

    if (k >= 32) {
        return NULL;
    }
    return AT(o, 0x1438 + k * 4, void *);
}


extern void func_00220E80(u8 *door);

/* the doors, drawn each frame: each of the 8 with a definition (+0x4 table) */
void func_002239C0(void *d) {
    u8 *doors = d;
    u32 k;

    if (AT(doors, 0x4, void **) == NULL) {
        return;
    }
    for (k = 0; k < 8; k++) {
        void **tbl = AT(doors, 0x4, void **);

        if (tbl != NULL && tbl[k] != NULL) {
            func_00220E80(doors + 0x10 + k * 0x210);
        }
    }
}


extern void func_0025FB10(u8 *obj, s32 layer);

/* the placed objects, drawn each frame in three passes (layers 1, 0x19, 0x26; the texture
 * cache +0x18 and gBootMessage +0x20 reset before each): every active one (bit in +0xC) that
 * isn't hidden (+0x0) */
void func_002C8E50(u8 *o) {
    static const s32 kLayers[3] = {1, 0x19, 0x26};
    VObject *tc = D_0044E4E8;
    VObject *msg;
    s32 pass, i;

    for (pass = 0; pass < 3; pass++) {
        if (pass == 0) {
            VCALL(tc, 0x18, void (*)(VObject *))(tc);
            msg = gBootMessage;
            VCALL(msg, 0x20, void (*)(VObject *))(msg);
        } else {
            VCALL(tc, 0x18, void (*)(VObject *))(tc);
            VCALL(msg, 0x20, void (*)(VObject *))(msg);
        }
        for (i = 0; i < 64; i++) {
            u8 *obj = o + 0x20 + i * 0xB0;

            if ((AT(o, 0xC + (i >> 5) * 4, u32) & (1u << (i & 0x1F))) && AT(obj, 0x0, u8) == 0) {
                func_0025FB10(obj, kLayers[pass]);
            }
        }
    }
}


/* draw a placed object in pass `layer` (layer by its name +0x70: "g_..." 0x26, "a_..." 0x19
 * (alpha, +0x80 set), others 1): its model (+0x40) takes its position and rotation */
void func_0025FB10(u8 *obj, s32 layer) {
    const u8 *name;
    s32 own;

    sceVu0CopyVector((f32 *)(obj + 0x50), (f32 *)(obj + 0x20));
    sceVu0CopyVector((f32 *)(obj + 0x60), (f32 *)(obj + 0x10));
    name = AT(obj, 0x70, const u8 *);
#ifdef HG_NATIVE
    if (name == NULL) {
        name = (const u8 *)"";   /* the PS2 reads its address 0 here */
    }
#endif
    AT(obj, 0x80, u8) = 0;
    if ((name[0] == 'g' || name[0] == 'G') && name[1] == '_') {
        own = 0x26;
    } else if ((name[0] == 'a' || name[0] == 'A') && name[1] == '_') {
        own = 0x19;
        AT(obj, 0x80, u8) = 1;
    } else {
        own = 1;
    }
    if (own == layer) {
        VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, obj + 0x40, layer, 0);
    }
}


/* the creatures, drawn each frame (texture cache +0x18 and gBootMessage +0x20 reset first):
 * each active, visible one (+0x2C), unless the player is in a special state */
void func_002E29A0(u8 *o) {
    s32 i;

    VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
    VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
    for (i = 0; i < 10; i++) {
        VObject *c = AT(o, i * 4, VObject *);

        if (c == NULL || AT(gCharPlayer, 0xE2, u8) != 0) {
            continue;
        }
        if (AT(c, 0x28, u8) == 1 && AT(c, 0x29, u8) == 0) {
            VCALL(c, 0x2C, void (*)(VObject *))(c);
        }
    }
}


/* the dynamic actors, drawn each frame (texture cache and gBootMessage reset first): each
 * active one in the current room (+0x2C) */
void func_002D74E0(u8 *o) {
    s32 room, i;

    VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
    VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
    room = VCALL(gProgress, 0xC, s32 (*)(void *))(gProgress);
    for (i = 0; i < 0x80; i++) {
        VObject *a = func_00120D60(o + 0xA040, i);

        if (a != NULL && AT(a, 0x28, u8) == 1 && AT(a, 0x30, s32) == room) {
            VCALL(a, 0x2C, void (*)(VObject *))(a);
        }
    }
}


/* the effects, drawn each frame (texture cache and gBootMessage reset first): each of the 32
 * (+0x14) */
void func_00267160(u8 *o) {
    s32 i;

    VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
    VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
    for (i = 0; i < 32; i++) {
        VObject *e = AT(o, 0x1438 + i * 4, VObject *);

        if (e != NULL) {
            VCALL(e, 0x14, void (*)(VObject *))(e);
        }
    }
}


/* the effects, drawn each frame (texture cache +0x18 and gBootMessage +0x20 reset first): each
 * live one of the 0x400 slots at +0x18034 draws (+0x14) */
void func_002D61E0(u8 *mgr) {
    u32 i;

    VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
    VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
    for (i = 0; i < 0x400; i++) {
        VObject *e = AT(mgr, 0x18034 + i * 4, VObject *);

        if (e != NULL) {
            VCALL(e, 0x14, void (*)(VObject *))(e);
        }
    }
}


/* the screen fade's level (+0x34), clamped to 0..1 */
void func_002EF480(u8 *fade, f32 t) {
    if (t < 0.0f) {
        t = 0.0f;
    } else if (!(t <= 1.0f)) {
        t = 1.0f;
    }
    AT(fade, 0x34, f32) = t;
}


extern void *D_0045D1F0;   /* the screen overlay (Scene +0x105344C) */
extern void func_0021E1B0(void *ov);
extern void func_0021D290(void *ov);
extern void func_0021D8F0(void *ov, s32 limit, s32 amount);   /* the panic tint (palette 5 below `limit`) */

/* the screen fade, each frame (`mode` 2: brighten): while not fully up (+0x34 < 1) the camera
 * shake stops and the brightness (+0x38) is reset to 0x80; at full level the renderer takes
 * the brightness (mode 2 raises it by 4 up to 0x80, mode 1 leaves it alone) and a dim one
 * (<= 0x10) steps the overlay; states 4 / 5 stop the shake too (4 with +0x2 < 0 also ends the
 * overlay); the overlay is tinted by +0x40 times the level */
void func_002F0340(u8 *fade, s32 mode) {
    VObject *cam;

    if (AT(fade, 0x0, u8) == 5 || AT(fade, 0x0, u8) == 4) {
        if (AT(fade, 0x0, u8) == 4 && AT(fade, 0x2, s16) < 0 && AT(fade, 0x34, f32) == 1.0f) {
            func_0021E1B0(D_0045D1F0);
        }
        if (AT(fade, 0x34, f32) != 1.0f) {
            cam = D_0044E4B8;
            VCALL(cam, 0x6C, void (*)(VObject *, f32))(cam, 0.0f);
        }
    }
    if (AT(fade, 0x38, s32) != 0x80) {
        if (AT(fade, 0x34, f32) == 1.0f) {
            if (mode != 1) {
                if (mode == 2) {
                    AT(fade, 0x38, s32) += 4;
                    if (AT(fade, 0x38, s32) > 0x80) {
                        AT(fade, 0x38, s32) = 0x80;
                    }
                }
                VCALL(D_0044E4F0, 0x60, void (*)(VObject *, u8))(D_0044E4F0, AT(fade, 0x38, u8));
            }
            if (AT(fade, 0x38, s32) <= 0x10) {
                func_0021D290(D_0045D1F0);
            }
        } else {
            cam = D_0044E4B8;
            VCALL(cam, 0x6C, void (*)(VObject *, f32))(cam, 0.0f);
            AT(fade, 0x38, s32) = 0x80;
        }
    }
    if (AT(fade, 0x40, s32) != 0) {
        func_0021D8F0(D_0045D1F0, 0x30, (s32)((f32)AT(fade, 0x40, s32) * AT(fade, 0x34, f32)));
    }
}


/* a noise for the pursuer to hear (the loudest this frame wins): loudness `loud` (u8), in room
 * `room`, at triangle `tri` - or at door `door` (0xFFFF: none) */
void func_002A8440(u8 *n, s32 loud, s32 room, s32 tri, s32 door) {
    if (loud == 0 || room == -1 || (u8)loud < AT(n, 0x0, u8)) {
        return;
    }
    AT(n, 0x0, u8) = loud;
    AT(n, 0x4, s32) = room;
    if ((u16)door != 0xFFFF) {
        AT(n, 0x8, s32) = -1;
        AT(n, 0xC, u16) = door;
    } else {
        AT(n, 0x8, s32) = tri;
        AT(n, 0xC, u16) = 0xFFFF;
    }
}


extern VObject *D_0044E550;    /* random numbers: +0x18 -> 0..1 */
extern u8 D_0047AC90[];        /* per noise level: summon chance, hunted chance (percent) */
extern u32 D_00419DC0[];       /* the seconds before the pursuer can be summoned, by kind */
extern s32 func_001788F0(void *p, u32 door);   /* a door is open (u8) */
extern void func_00177630(Progress *p, s32 n);
extern s32 func_00177620(Progress *p);

/* a frame's loudest noise `n` (func_002A8440) against the summoner `o` (+0xC frames waited,
 * +0x11 its kind): in the current room only. Its level 0..3 by loudness (one less away from
 * doors with every door of the room shut). With the pursuer waiting offstage (+0xD0 / +0xD1,
 * not active), Progress flag 0 without flag 2, at least 3 s and its kind's wait gone, the
 * level's chance (percent) summons it (o +0 = 0x80000000); otherwise the level's second
 * chance - on the rest - sets condition 6 (hunted), as does mode 1 with any chance */
void func_002EC4F0(u8 *o, u8 *n) {
    Progress *p;
    VObject *rooms;
    u8 *pu;
    u8 *t;
    u32 i, d;
    s32 open = 0, lvl, call = 0, roll;

    if (n == NULL) {
        return;
    }
    p = gProgress;
    if (AT(n, 0x4, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return;
    }
    rooms = D_0044E568;
    for (i = 0; i < 8; i++) {
        d = (u16)VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, AT(n, 0x4, s32), i & 0xFF);
        if (d < 0x190 && (u8)func_001788F0(p, d) == 1) {
            open = 1;
            break;
        }
    }
    lvl = AT(n, 0x0, u8) >= 0x80 ? 4 : AT(n, 0x0, u8) >= 0x60 ? 3 : AT(n, 0x0, u8) >= 0x40 ? 2
        : AT(n, 0x0, u8) >= 0x20 ? 1 : 0;
    if (AT(n, 0xC, u16) == 0xFFFF && !open && (s8)lvl < 4) {
        lvl = (s8)(lvl - 1);
    }
    if ((s8)lvl >= 4) {
        lvl = 3;
    }
    if ((s8)lvl < 0) {
        lvl = 0;
    }
    t = D_0047AC90 + (s8)lvl * 2;
    pu = (u8 *)gCharPursuer;
    if (pu != NULL && (AT(pu, 0xD0, u8) != 0 || AT(pu, 0xD1, u8) != 0)) {
        if (AT(pu, 0x28, u8) != 0) {
            return;
        }
        roll = 0;
        if (t[0] != 0) {
            if (t[0] == 100 || 100.0f * VCALL(D_0044E550, 0x18, f32 (*)(VObject *))(D_0044E550) <= (f32)t[0]) {
                roll = 1;
            }
        }
        call = roll && Progress_TestFlag(p, 0) && !Progress_TestFlag(p, 2);
        if (call) {
            u32 secs = (u16)(AT(o, 0xC, u32) / 30);

            if (secs < 3 || secs < D_00419DC0[AT(o, 0x11, u8)]) {
                call = 0;
            }
        }
    }
    if (call) {
        AT(o, 0x0, u32) = 0x80000000;
        return;
    }
    if (t[1] != 0 &&
        (100.0f - (f32)t[0]) * VCALL(D_0044E550, 0x18, f32 (*)(VObject *))(D_0044E550) <= (f32)t[1]) {
        func_00177630(p, 6);
        return;
    }
    if ((u8)func_00177620(p) == 1 && t[0] + t[1] != 0) {
        func_00177630(p, 6);
    }
}


extern void func_00220D10(u8 *door, s32 sound, s32 arg);   /* a door sound */
extern void func_00178C10(Progress *p, s32 room, s32 door, s32 arg);   /* door is open */
extern void func_00178A90(Progress *p, s32 room, s32 door, s32 arg);   /* door is shut */

extern s32 func_0025EEE0(u8 *obj);   /* the drawn object was on screen */
extern void func_00278D60(u8 *fx, u32 tri, f32 *pos, f32 *rot);

/* the doorway (12 x 22, at depth z) through the door's matrix m, as the lights take it (+0x38):
 * its corners (top far, top near, bottom far, bottom near along x by `x0` -> `x1`), its facing
 * (z sign, normalised) and its top middle */
static void door_portal(sceVu0FMATRIX m, f32 x0, f32 x1, f32 z) {
    f32 q[6][4] __attribute__((aligned(16)));
    s32 i;

    q[0][0] = x0; q[0][1] = 22.0f; q[0][2] = z; q[0][3] = 1.0f;
    q[1][0] = x1; q[1][1] = 22.0f; q[1][2] = z; q[1][3] = 1.0f;
    q[2][0] = x0; q[2][1] = 0.0f; q[2][2] = z; q[2][3] = 1.0f;
    q[3][0] = x1; q[3][1] = 0.0f; q[3][2] = z; q[3][3] = 1.0f;
    for (i = 0; i < 4; i++) {
        sceVu0ApplyMatrix(q[i], m, q[i]);
    }
    q[4][0] = 0.0f; q[4][1] = 0.0f; q[4][2] = z == 0.0f ? 1.0f : -1.0f; q[4][3] = 0.0f;
    sceVu0ApplyMatrix(q[4], m, q[4]);
    sceVu0Normalize(q[4], q[4]);
    q[5][0] = 6.0f; q[5][1] = 22.0f; q[5][2] = z; q[5][3] = 1.0f;
    sceVu0ApplyMatrix(q[5], m, q[5]);
    VCALL(D_0044E4C8, 0x38, void (*)(VObject *, f32 *))(D_0044E4C8, q[0]);
}

/* draw a shown door (+0x70) that has a model (+0x0): its draw object (+0x80) placed from the
 * door's position / rotation; when it is on screen and open, its effect (+0x190, when +0x72)
 * and, open past 78.75 degrees, the doorway on both sides for the lights */
void func_00220E80(u8 *door) {
    u8 *obj = door + 0x80;
    sceVu0FMATRIX m;

    if (AT(door, 0x70, u8) != 1 || AT(door, 0x0, void *) == NULL) {
        return;
    }
    sceVu0CopyVector((f32 *)(obj + 0x70), (f32 *)(door + 0x10));
    sceVu0CopyVector((f32 *)(obj + 0x80), (f32 *)(door + 0x30));
    AT(obj, 0x64, s32) = -1;
    AT(obj, 0x68, s32) = 0;
    AT(obj, 0x8, void *) = AT(door, 0x0, void *);
    AT(obj, 0x18, s32) = 0;
    VCALL(D_0044E4F0, 0xC, void (*)(VObject *, u8 *, s32, s32))(D_0044E4F0, obj, 1, 0);
    if (!func_0025EEE0(obj) || !(AT(door, 0x64, f32) < 0.0f)) {
        return;
    }
    if (AT(door, 0x72, u8) == 1) {
        func_00278D60(door + 0x190, AT(door, 0x8, u32), (f32 *)(door + 0x10), (f32 *)(door + 0x30));
    }
    if (!(AT(door, 0x64, f32) <= -78.75f)) {
        return;
    }
    sceVu0UnitMatrix(m);
    sceVu0RotMatrix(m, m, (f32 *)(door + 0x30));
    sceVu0TransMatrix(m, m, (f32 *)(door + 0x10));
    door_portal(m, 12.0f, 0.0f, 0.0f);
    door_portal(m, 0.0f, 12.0f, -1.0f);
}

/* the door's angle +0x34: its rest angle +0x44 turned by +0x64 degrees, kept above -pi */
static void door_turn(u8 *door) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kMinusPi = {0xC0490FDB}, kTwoPi = {0x40C90FDB};
    f32 a = AT(door, 0x64, f32);

    AT(door, 0x64, f32) = a;
    AT(door, 0x34, f32) = AT(door, 0x44, f32) + kPi.f * a / 180.0f;
    if (AT(door, 0x34, f32) < kMinusPi.f) {
        AT(door, 0x34, f32) = AT(door, 0x34, f32) + kTwoPi.f;
    }
}

/* the door's swing ended open (`open`) or shut: it stops (+0x60) and the progress learns of it
 * when the door (+0x4) leads somewhere from the current room */
static void door_settle(u8 *door, s32 open) {
    Progress *p;
    s32 room;

    AT(door, 0x60, s32) = 0;
    p = gProgress;
    room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    if ((VCALL(D_0044E568, 0x10, u32 (*)(VObject *, s32, u32))(D_0044E568, room, AT(door, 0x4, u8)) & 0xFFFF) ==
        0xFFFF) {
        return;
    }
    p = gProgress;
    room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    if (open) {
        func_00178C10(p, room, AT(door, 0x4, u8), 0xFF);
    } else {
        func_00178A90(p, room, AT(door, 0x4, u8), 0xFF);
    }
}

/* a door's swing, each frame (+0x60: 1 along a list of angles +0x5C (+0x54 of +0x58 done),
 * 2 by 5 degrees a frame, 3 slammed by 15; +0x68 0 opening to -90 degrees (+0x64), else
 * closing to 0), while it is shown (+0x70); the creak / latch sounds once (+0x71) */
void func_00221300(u8 *door) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kMinusPi = {0xC0490FDB}, kTwoPi = {0x40C90FDB};
    f32 step;

    switch (AT(door, 0x60, s32)) {
    case 1: {
        f32 *key;

        if (AT(door, 0x70, u8) != 1 || (key = AT(door, 0x5C, f32 *)) == NULL) {
            return;
        }
        AT(door, 0x64, f32) = *key;
        AT(door, 0x34, f32) = AT(door, 0x44, f32) + kPi.f * *key / 180.0f;
        if (AT(door, 0x34, f32) < kMinusPi.f) {
            AT(door, 0x34, f32) = AT(door, 0x34, f32) + kTwoPi.f;
        }
        AT(door, 0x54, s32) += 1;
        if (AT(door, 0x54, s32) == AT(door, 0x58, s32)) {
            AT(door, 0x5C, s32) = 0;
            AT(door, 0x60, s32) = 0;
        } else {
            AT(door, 0x5C, s32) += 4;
        }
        if (AT(door, 0x71, u8) != 0) {
            return;
        }
        if (AT(door, 0x68, s32) == 1) {
            if (!(AT(door, 0x64, f32) <= -6.0f)) {
                AT(door, 0x71, u8) = 1;
                func_00220D10(door, 0x28, 2);
            }
        } else if (AT(door, 0x64, f32) < 0.0f) {
            AT(door, 0x71, u8) = 1;
            func_00220D10(door, 0x27, 1);
        }
        return;
    }
    case 2:
        if (AT(door, 0x70, u8) != 1) {
            return;
        }
        if (AT(door, 0x68, s32) == 0) {
            AT(door, 0x64, f32) = AT(door, 0x64, f32) - 5.0f;
            if (AT(door, 0x64, f32) < -90.0f) {
                AT(door, 0x64, f32) = -90.0f;
                door_settle(door, 1);
            }
        } else {
            AT(door, 0x64, f32) = AT(door, 0x64, f32) + 5.0f;
            if (AT(door, 0x71, u8) == 0 && !(AT(door, 0x64, f32) <= -6.0f)) {
                AT(door, 0x71, u8) = 1;
                func_00220D10(door, 0x28, 0);
            }
            if (!(AT(door, 0x64, f32) <= 0.0f)) {
                AT(door, 0x64, f32) = 0.0f;
                door_settle(door, 0);
            }
        }
        door_turn(door);
        return;
    case 3:
        if (AT(door, 0x70, u8) != 1) {
            return;
        }
        step = 15.0f;
        if (AT(door, 0x68, s32) == 0) {
            AT(door, 0x64, f32) = AT(door, 0x64, f32) - step;
            if (AT(door, 0x64, f32) < -90.0f) {
                AT(door, 0x64, f32) = -90.0f;
                door_settle(door, 1);
            }
        } else {
            AT(door, 0x64, f32) = AT(door, 0x64, f32) + step;
            if (!(AT(door, 0x64, f32) <= 0.0f)) {
                AT(door, 0x71, u8) = 1;
                func_00220D10(door, 0x28, 4);
                AT(door, 0x64, f32) = 0.0f;
                door_settle(door, 0);
            }
        }
        door_turn(door);
        return;
    }
}

/* reset a door: not opening (+0x5C), no frame (+0x60) */
void func_00221880(u8 *door) {
    AT(door, 0x5C, s32) = 0;
    AT(door, 0x60, s32) = 0;
}


extern s32 func_00178980(Progress *p, s32 room, s32 exit);   /* that door is open (u8) */

/* +0x7C the room's doors (0x210 each from +0x10) as the room comes in: each present one
 * (doors +0x40) that leads somewhere (rooms +0x10) stands open (-90 degrees, +0x64, its angle
 * +0x34 a quarter turn from +0x44) or shut, its two sides' passage set to match (+0x20 /
 * +0x1C); then each door's +0x80 */
void func_00222230(VObject *o) {
    static const union { u32 u; f32 f; } kHalfPi = {0x3FC90FDB}, kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    Progress *p = gProgress;
    s32 room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    VObject *doors = D_0044E558;
    VObject *rooms = D_0044E568;
    u32 i;

    for (i = 0; i < 8; i++) {
        u8 *e = (u8 *)o + i * 0x210 + 0x10;

        if ((u8)VCALL(doors, 0x40, s32 (*)(VObject *, u32))(doors, i & 0xFF) == 1 &&
            (u16)VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, room, i & 0xFF) != 0xFFFF) {
            if ((u8)func_00178980(p, room, i & 0xFF) == 1) {
                AT(e, 0x64, f32) = -90.0f;
                AT(e, 0x34, f32) = AT(e, 0x44, f32) + -kHalfPi.f;
                if (AT(e, 0x34, f32) < -kPi.f) {
                    AT(e, 0x34, f32) = AT(e, 0x34, f32) + kTwoPi.f;
                }
                VCALL(o, 0x20, void (*)(VObject *, u32, s32, s32))(o, i & 0xFF, 0, 0x60000);
                VCALL(o, 0x1C, void (*)(VObject *, u32, s32, s32))(o, i & 0xFF, 1, 0x60000);
            } else {
                AT(e, 0x64, f32) = 0.0f;
                AT(e, 0x34, f32) = AT(e, 0x44, f32) + 0.0f;
                if (AT(e, 0x34, f32) < -kPi.f) {
                    AT(e, 0x34, f32) = AT(e, 0x34, f32) + kTwoPi.f;
                }
                VCALL(o, 0x20, void (*)(VObject *, u32, s32, s32))(o, i & 0xFF, 1, 0x60000);
                VCALL(o, 0x1C, void (*)(VObject *, u32, s32, s32))(o, i & 0xFF, 0, 0x60000);
            }
        }
        VCALL(o, 0x80, void (*)(VObject *, u32))(o, i & 0xFF);
    }
}

/* a door's side `side` (0 / 1) passage: its walk-mesh triangles (the door's section entry +0x28:
 * a count and the triangles for side 0, then side 1's) get `flags` set (mode 0, shut) or
 * cleared (mode 1, open); -1 for no such door or side */
s32 func_00223630(VObject *o, s32 mode, u32 door, s32 side, u32 flags) {
    u8 *tbl = AT(o, 0x4, u8 *);
    s32 off = 0, n, k;
    u32 *tri;
    NavMesh *nm;

    if (tbl != NULL && (u8)door < 8) {
        off = AT(tbl, (u8)door * 4, s32);
    }
    if (off == 0 || side < 0 || side >= 2) {
        return -1;
    }
    n = AT(tbl + off, 0x28, s32);
    tri = (u32 *)(tbl + off + 0x2C);
    if (side != 0) {
        tri += n;
        n = *tri++;
    }
    nm = D_0044E570;
    if (mode == 1) {
        for (k = 0; k < n; k++) {
            NavTri *t = NavMesh_Tri(nm, *tri++);

#ifdef HG_NATIVE
            if (t == NULL) {
                continue;
            }
#endif
            t->flags &= ~flags;
        }
    } else if (mode == 0) {
        for (k = 0; k < n; k++) {
            NavTri *t = NavMesh_Tri(nm, *tri++);

#ifdef HG_NATIVE
            if (t == NULL) {
                continue;
            }
#endif
            t->flags |= flags;
        }
    }
    return 0;
}

/* +0x1C shut door `door`'s side `side` (passage flags set) */
s32 func_00221B40(VObject *o, u32 door, s32 side, u32 flags) {
    return func_00223630(o, 0, door, side, flags);
}

/* +0x20 open it (cleared) */
s32 func_00221B60(VObject *o, u32 door, s32 side, u32 flags) {
    return func_00223630(o, 1, door, side, flags);
}

/* ---- small leftovers (2026-10-05) ---- */

#ifdef HG_NATIVE
#define CORE_SYNC_EI()
#else
#define CORE_SYNC_EI() __asm__ volatile("sync\n\tei")
#endif

extern void func_001CC5B0(s32 a);

/* an empty method returning 0 */
s32 func_0011FF20(void) {
    return 0;
}

/* an empty method */
void func_00120F70(void) {
}

/* an empty method */
void func_00267300(void) {
}

/* is the loader's request for file slot k (+0x3C0, -1 none) done (its state 2)? */
s32 func_00120540(u8 *o, s32 k) {
    s32 req = AT(o, 0x3C0 + k * 4, s32);

    if (req == -1) {
        return 0;
    }
    return VCALL(gFileLoader, 0x28, s32 (*)(void *, u32))(gFileLoader, (u32)req & 0x7FFFFFFF) == 2 ? 1 : 0;
}

/* func_001CC5B0(0), then interrupts back on; 0 */
s32 func_001AAC30(void) {
    func_001CC5B0(0);
    CORE_SYNC_EI();
    return 0;
}

/* reset 64 entries of 12 (+0x4 0, +0x8 / +0xA -1, +0xC / +0xD 0xFF) and ten words at +0x304
 * to -1 */
void func_001F4100(u8 *o) {
    u8 *e = o;
    u32 i;

    for (i = 0; i < 0x40; i++, e += 0xC) {
        AT(e, 0x4, s32) = 0;
        AT(e, 0x8, s16) = -1;
        AT(e, 0xA, s16) = -1;
        AT(e, 0xD, s8) = -1;
        AT(e, 0xC, s8) = -1;
    }
    for (i = 0; i < 10; i++) {
        AT(o, 0x304 + i * 4, s32) = -1;
    }
}

/* release its two VRAM slots (+0x320 / +0x324, the VRAM manager +0x1C) and mark them -1 */
/* (possibly dead code: nothing in the game references it) */
void func_001F9D20(u8 *o) {
    VObject *vram = D_0044E9A0;

    VCALL(vram, 0x1C, void (*)(VObject *, s32))(vram, AT(o, 0x320, s32));
    AT(o, 0x320, s32) = -1;
    VCALL(vram, 0x1C, void (*)(VObject *, s32))(vram, AT(o, 0x324, s32));
    AT(o, 0x324, s32) = -1;
}

void func_001F9F70(u8 *o, s32 v) {
    AT(o, 0x944, s32) = v;
}

u8 *func_001FA520(u8 *o) {
    return o + 0x14;
}

/* the current entry's block (+0x120 + +0x560 * 4) */
u8 *func_001FB150(u8 *o) {
    return o + AT(o, 0x560, s32) * 4 + 0x120;
}

/* destructor of a class with no vtable of its own */
/* (possibly dead code: nothing in the game references it) */
void *func_0020E820(void *o, s32 flags) {
    if (o != NULL && (s16)flags > 0) {
        func_00100490(o);
    }
    return o;
}

/* the renderer's +0x5C */
/* (possibly dead code: nothing in the game references it) */
void func_00267140(void) {
    VCALL(D_0044E4F0, 0x5C, void (*)(VObject *))(D_0044E4F0);
}

extern void func_001CA850(void);
extern void func_001C8478(void);
extern void func_001C8648(void *p);
extern s32 RemoveIntcHandler(s32 cause, s32 id);
extern u8 D_0044FE18[];
extern void *D_0046AF00[], *D_0046AF0C[], *D_0046C740[], *D_0046AED0[], *D_0046AD88[], *D_0046AEC0[];
extern void *D_0044FEF8;

/* start loading file slot k (+0x3C0) unless it already is (bit 31): the loader +0x14, the slot
 * marked, its callback (+0x3C8, a PTMF each) back to none */
void func_001205A0(u8 *o, s32 k) {
    u32 *slot = (u32 *)(o + 0x3C0) + k;

    if (*slot & 0x80000000) {
        return;
    }
    VCALL(gFileLoader, 0x14, void (*)(void *, u32))(gFileLoader, *slot);
    *slot |= 0x80000000;
    AT(o, 0x3C8 + k * 12, PTMF) = sGameStateNull;
}

/* (possibly dead code: nothing in the game references it) */
/* clear +0xC700 and the 20 words after it */
/* (possibly dead code: nothing in the game references it) */
void func_0017D1B0(u8 *o) {
    s32 i;

    AT(o, 0xC700, s32) = 0;
    for (i = 0; i < 20; i++) {
        AT(o, 0xC704 + i * 4, s32) = 0;
    }
}

/* shut down: the member at +0x7C44 (+0x14), the sound side (func_001CA850, func_001C8478,
 * func_001C8648(D_0044FE18)) and its interrupt handler (+0x12C, cause 3) */
void func_001AAC60(u8 *o) {
    VObject *m = (VObject *)(o + 0x7C44);

    VCALL(m, 0x14, void (*)(VObject *))(m);
    func_001CA850();
    func_001C8478();
    func_001C8648(D_0044FE18);
    RemoveIntcHandler(3, AT(o, 0x12C, s32));
}

/* destructor (D_0046AF00): its members at +0x7C44 (D_0046AED0, clearing D_0044FEF8) and +0x124
 * (D_0046AD88), then the base (D_0046AEC0, clearing D_0044E980) */
void *func_001BF6C0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AF00;
        AT(o, 0x124, void **) = D_0046AF0C;
        AT(o, 0x7C44, void **) = D_0046C740;
        AT(o, 0x7C44, void **) = D_0046AED0;
        AT(o, 0x7C48, u8) = 0;
        D_0044FEF8 = NULL;
        AT(o, 0x124, void **) = D_0046AD88;
        AT(o, 0x0, void **) = D_0046AEC0;
        AT(o, 0x4, s32) = 0;
        AT(o, 0x8, s32) = 0;
        AT(o, 0xC, s32) = 0;
        D_0044E980 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* effect `slot`'s +0x1C (-1: no such effect) */
s32 func_002D6020(u8 *o, s32 slot) {
    void *e;

    if (slot < 0 || (u32)slot >= 0x400) {
        return -1;
    }
    e = AT(o, 0x18034 + slot * 4, void *);
    if (e == NULL) {
        return -1;
    }
    return VCALL(e, 0x1C, s32 (*)(void *))(e);
}

extern void *D_0046BF08[], *D_0046A1C0[], *D_004699E0[];
extern void *D_0044E960;   /* the scene table */

/* the scene table's destructor (D_0046BF08): its four scenes handed back to the scene heap
 * (+0x10D9040, +0x14) and destroyed, the heap's vtables, D_0044E960 cleared */
void *func_0020DA10(u8 *t, s32 flags) {
    s32 i;

    if (t == NULL) {
        return t;
    }
    AT(t, 0x0, void **) = D_0046BF08;
    for (i = 0; i < 4; i++) {
        void **slot = (void **)(t + 4) + i;

        if (*slot != NULL) {
            VObject *heap = (VObject *)(t + 0x10D9040);

            VCALL(heap, 0x14, void (*)(VObject *, void *))(heap, *slot);
            if (*slot != NULL) {
                VCALL(*slot, 0x8, void (*)(void *, s32))(*slot, 1);
            }
            *slot = NULL;
        }
    }
    AT(t, 0x10D9040, void **) = D_0046A1C0;
    AT(t, 0x10D9040, void **) = D_004699E0;
    D_0044E960 = NULL;
    if ((s16)flags > 0) {
        func_00100490(t);
    }
    return t;
}
