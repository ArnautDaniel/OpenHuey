/* The item manager (D_0044E988): Fiona's inventory and equipment. */
#include "common.h"
#include "game.h"

/* +0x10: the item in equipment slot `slot` (+0x15E0[slot], its +0xC; -1: empty slot) */
s32 func_00260690(u8 *items, u8 slot) {
    VObject *e = AT(items, 0x15E0 + slot * 4, VObject *);

    if (e == NULL) {
        return -1;
    }
    return VCALL(e, 0xC, s32 (*)(VObject *))(e);
}

#include "progress.h"
#include "sce/libvu0.h"

extern VObject *D_0044E570;   /* the nav mesh */
extern void *func_00120D60(void *pool, u32 i);   /* BlockPool: block i if in use */

/* (D_0044F260, the placed things: 128 blocks of 0x140 in the pool +0xA040) +0x20: put the
 * active ones of kinds 0, 2, 3, 5, 7, 8 that are in the current room onto their nav mesh
 * triangle (+0x34; position +0x10, copied to +0x40) */
void func_002D69E0(u8 *mgr) {
    s32 room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    VObject *nav = D_0044E570;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        u8 *t = func_00120D60(mgr + 0xA040, i);

        if (t == NULL || AT(t, 0x28, u8) != 1) {
            continue;
        }
        switch (AT(t, 0x20, u32)) {
        case 0: case 2: case 3: case 5: case 7: case 8:
            if (AT(t, 0x30, s32) == room && AT(t, 0x34, s32) != -1) {
                VCALL(nav, 0x14, void (*)(VObject *, s32, f32 *))(nav, AT(t, 0x34, s32), (f32 *)(t + 0x10));
                sceVu0CopyVector((f32 *)(t + 0x40), (f32 *)(t + 0x10));
            }
            break;
        }
    }
}


extern void func_002608D0(u8 *o, VObject *it);   /* remove an entry */

/* entry `i` of list `l`: one tick (+0x30) while it is set (+0x18) and has 2 or more left
   (+0x34), otherwise it is removed */
static void item_use(u8 *o, u32 l, u32 i) {
    VObject *it = AT(o, 0x12E0 + (l & 0xFF) * 0x100 + (i & 0xFF) * 4, VObject *);

    if (it == NULL) {
        return;
    }
    if ((u8)VCALL(it, 0x18, s32 (*)(VObject *))(it) == 1 && !(VCALL(it, 0x34, u32 (*)(VObject *))(it) < 2)) {
        VCALL(it, 0x30, void (*)(VObject *))(it);
    } else {
        func_002608D0(o, it);
    }
}

/* each frame: the 3 lists of up to 64 entries (+0x12E0, 0x100 apart, ending at the first
 * empty slot): each one in use (+0x38 bits 0..1) is used (item_use) */
void func_00260EC0(u8 *o) {
    u32 l, i;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 64; i++) {
            VObject *it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);

            if (it == NULL) {
                break;
            }
            if (!((u8)VCALL(it, 0x38, s32 (*)(VObject *))(it) & 3)) {
                continue;
            }
            item_use(o, l, i);
        }
    }
}

/* use item `id` (+0xC) wherever it is in the lists: 1 if it was there */
s32 func_00260BB0(u8 *o, s32 id) {
    u32 l, i;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 64; i++) {
            VObject *it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);

            if (it == NULL) {
                break;
            }
            if (id == VCALL(it, 0xC, s32 (*)(VObject *))(it)) {
                item_use(o, l, i);
                return 1;
            }
        }
    }
    return 0;
}
