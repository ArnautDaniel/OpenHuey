/* SceneGame's members: constructors and resets reached from SceneGame_ctor. Grouped here until
 * the classes they belong to are identified (then they move to their subsystem's file). */
#include "common.h"
#include "game.h"

extern void func_00120EC0(void *pool, u8 *base, u32 size, u32 n, u8 *used);   /* BlockPool init */
extern void func_00100340(void *array, void *(*ctor)(void *), void *(*dtor)(void *, s32), u32 size, u32 n);
extern VObject *D_0044E4E8;   /* the texture cache */
extern void *D_0046C320[], *D_0046ECF0[], *D_0046C540[];
extern void *D_0044FE08, *D_00456DF8, *D_0044E558;
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
    D_0044E558 = p;
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
