/* SceneGame's members: constructors and resets reached from SceneGame_ctor. Grouped here until
 * the classes they belong to are identified (then they move to their subsystem's file). */
#include "common.h"
#include "game.h"
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

/* +0x20 set light `i` (3 vectors) */
void func_001FA5E0(u8 *o, const f32 *light, s32 i) {
    f32 *d = (f32 *)(o + 0x20 + i * 0x30);
    s32 k;

    for (k = 0; k < 12; k++) {
        d[k] = light[k];
    }
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
extern VObject *D_0044E4C8;   /* the scene's lights */
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

/* ---- room manager +0x9360 (D_00456E00): the room's triangle groups (PAC section 14: count,
 * then offsets of {n, triangle indices}) whose nav mesh flags scripts switch ---- */

extern u8 *D_0044E570;   /* the nav mesh (room manager +0x3E0): +0x4 triangles, +0x8 count */

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
extern void func_002D1FD0(void);
extern void func_002B6340(void);

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
        func_002D1FD0();
        if (D_00456DF0 != NULL) {
            VCALL(D_00456DF0, 0x24, void (*)(VObject *, f32))(D_00456DF0, AT(o, 0x10, f32));
        }
        if (D_0044E958 != NULL) {
            AT(D_0044E958, 0x1D4, f32) = clamp01(AT(o, 0x10, f32));
            func_002B6340();
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
extern void func_002EBED0(u8 *o);
extern s32 func_00177200(Progress *p, u32 slot);

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
        pu = gCharPursuer;
        if (pu == NULL) {
            return;
        }
        if (AT(pu, 0x28, u8)) {
            AT(o, 0x8, s32) = AT(pu, 0x30, s32);
            AT(o, 0x0, u32) = func_0029CE50(pu) & 0x7FFFFFFF;
            AT(o, 0xC, s32) = 0;
            AT(o, 0x11, u8) = 1;
        }
        func_00177200(p, AT(pu, 0x20, u32));
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
extern void func_0021D8F0(void *ov, s32 layer, s32 alpha);

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
