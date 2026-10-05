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

/* how many of item `id` (+0x34) the lists hold: 0 if none */
u32 func_00260CF0(u8 *o, s32 id) {
    u32 l, i;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 64; i++) {
            VObject *it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);

            if (it == NULL) {
                break;
            }
            if (id == VCALL(it, 0xC, s32 (*)(VObject *))(it)) {
                it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);
                return (u8)VCALL(it, 0x34, s32 (*)(VObject *))(it);
            }
        }
    }
    return 0;
}
