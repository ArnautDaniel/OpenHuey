/* The item manager (gSubScreen): Fiona's inventory and equipment. */
#include "common.h"
#include "game.h"
#include "globals.h"
#include "ptmf.h"
#include "progress.h"
#include "actor.h"
#include "item_classes.h"
#include "items.h"
#include "subscreen.h"

extern void *D_0046C790[], *D_0046F430[];
extern void *D_0046D5F0[];

ItemObj *func_00263220(ItemObj *self);
ItemObj *Item3F_ctor(ItemObj *self, s32 id);

void Item3F_Set(u8 *p, const u8 *src);

/* +0x10: the item in equipment slot `slot` (+0x15E0[slot], its +0xC; -1: empty slot) */
/* 0x00260690 */
s32 Items_Equipped(u8 *items, u8 slot) {
    VObject *e = AT(items, 0x15E0 + slot * 4, VObject *);

    if (e == NULL) {
        return -1;
    }
    return VCALL(e, 0xC, s32 (*)(VObject *))(e);
}

extern void Items_Remove(u8 *o, VObject *it);   /* remove an entry */

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
        Items_Remove(o, it);
    }
}

/* each frame: the 3 lists of up to 64 entries (+0x12E0, 0x100 apart, ending at the first
 * empty slot): each one in use (+0x38 bits 0..1) is used (item_use) */
/* 0x00260EC0 */
void Items_Update(u8 *o) {
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
/* 0x00260BB0 */
s32 Items_UseId(u8 *o, s32 id) {
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
/* 0x00260CF0 */
u32 Items_Count(u8 *o, s32 id) {
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

/* ---- giving an item (Items_Give): the inventory's three groups (ids under 0x40, under 0xA0,
 * the rest) of 64 slots each (+0x12E0 + group * 0x100); objects 0x18 bytes from its pool
 * (+0x1208) ---- */

typedef void *(*ItemCtor)(void *self);

static ItemCtor const kItemCtors[0xAD] = {
    [0x0] = (void * (*)(void *))func_002648D0,
    [0x1] = (void * (*)(void *))func_002648A0,
    [0x2] = (void * (*)(void *))func_00264870,
    [0x3] = (void * (*)(void *))func_00264840,
    [0x4] = (void * (*)(void *))func_00264810,
    [0x5] = (void * (*)(void *))func_002647E0,
    [0x6] = (void * (*)(void *))func_002647B0,
    [0x7] = (void * (*)(void *))func_00264780,
    [0x8] = (void * (*)(void *))func_00264750,
    [0x9] = (void * (*)(void *))func_00264720,
    [0xA] = (void * (*)(void *))func_002646F0,
    [0xB] = (void * (*)(void *))func_002646C0,
    [0xC] = (void * (*)(void *))func_00264690,
    [0xD] = (void * (*)(void *))func_00264660,
    [0xE] = (void * (*)(void *))func_00264630,
    [0xF] = (void * (*)(void *))func_00264600,
    [0x10] = (void * (*)(void *))func_002645D0,
    [0x11] = (void * (*)(void *))func_002645A0,
    [0x12] = (void * (*)(void *))func_00264570,
    [0x13] = (void * (*)(void *))func_00264540,
    [0x14] = (void * (*)(void *))func_00264510,
    [0x15] = (void * (*)(void *))func_002644E0,
    [0x16] = (void * (*)(void *))func_002644B0,
    [0x17] = (void * (*)(void *))func_00264480,
    [0x18] = (void * (*)(void *))func_00264450,
    [0x19] = (void * (*)(void *))func_00264420,
    [0x1A] = (void * (*)(void *))func_002643F0,
    [0x1B] = (void * (*)(void *))func_002643C0,
    [0x1C] = (void * (*)(void *))func_00264390,
    [0x1D] = (void * (*)(void *))func_00264360,
    [0x1E] = (void * (*)(void *))func_00264330,
    [0x1F] = (void * (*)(void *))func_00264300,
    [0x20] = (void * (*)(void *))func_002642D0,
    [0x21] = (void * (*)(void *))func_002642A0,
    [0x22] = (void * (*)(void *))func_00264270,
    [0x23] = (void * (*)(void *))func_00264240,
    [0x24] = (void * (*)(void *))func_00264210,
    [0x25] = (void * (*)(void *))func_002641E0,
    [0x26] = (void * (*)(void *))func_002641B0,
    [0x27] = (void * (*)(void *))func_00264180,
    [0x28] = (void * (*)(void *))func_00264150,
    [0x29] = (void * (*)(void *))func_00264120,
    [0x3E] = (void * (*)(void *))func_00264090,
    [0x40] = (void * (*)(void *))func_00264030,
    [0x41] = (void * (*)(void *))func_00264000,
    [0x42] = (void * (*)(void *))func_00263FD0,
    [0x43] = (void * (*)(void *))func_00263FA0,
    [0x44] = (void * (*)(void *))func_00263F70,
    [0x45] = (void * (*)(void *))func_00263F40,
    [0x46] = (void * (*)(void *))func_00263F10,
    [0x47] = (void * (*)(void *))func_00263EE0,
    [0x48] = (void * (*)(void *))func_00263EB0,
    [0x49] = (void * (*)(void *))func_00263E80,
    [0x4A] = (void * (*)(void *))func_00263E50,
    [0x4B] = (void * (*)(void *))func_00263E20,
    [0x4C] = (void * (*)(void *))func_00263D90,
    [0x60] = (void * (*)(void *))func_00263D60,
    [0x61] = (void * (*)(void *))func_00263D30,
    [0x62] = (void * (*)(void *))func_00263D00,
    [0x63] = (void * (*)(void *))func_00263CD0,
    [0x64] = (void * (*)(void *))func_00263CA0,
    [0x65] = (void * (*)(void *))func_00263C70,
    [0x66] = (void * (*)(void *))func_00263BE0,
    [0x70] = (void * (*)(void *))func_00263BB0,
    [0x71] = (void * (*)(void *))func_00263B80,
    [0x72] = (void * (*)(void *))func_00263B50,
    [0x73] = (void * (*)(void *))func_00263B20,
    [0x74] = (void * (*)(void *))func_00263AF0,
    [0x75] = (void * (*)(void *))func_00263A60,
    [0x80] = (void * (*)(void *))func_00263A30,
    [0x81] = (void * (*)(void *))func_00263A00,
    [0x82] = (void * (*)(void *))func_002639D0,
    [0x83] = (void * (*)(void *))func_00263940,
    [0x86] = (void * (*)(void *))func_00263910,
    [0x87] = (void * (*)(void *))func_002638E0,
    [0x88] = (void * (*)(void *))func_002638B0,
    [0x89] = (void * (*)(void *))func_00263820,
    [0x8A] = (void * (*)(void *))func_002637F0,
    [0x8B] = (void * (*)(void *))func_002637C0,
    [0x8C] = (void * (*)(void *))func_00263790,
    [0x8D] = (void * (*)(void *))func_00263700,
    [0x90] = (void * (*)(void *))func_002636D0,
    [0x91] = (void * (*)(void *))func_002636A0,
    [0x92] = (void * (*)(void *))func_00263670,
    [0x93] = (void * (*)(void *))func_00263640,
    [0x94] = (void * (*)(void *))func_00263610,
    [0x95] = (void * (*)(void *))func_002635E0,
    [0x97] = (void * (*)(void *))func_002635B0,
    [0x98] = (void * (*)(void *))func_00263580,
    [0x9B] = (void * (*)(void *))func_002634F0,
    [0xA0] = (void * (*)(void *))func_002634C0,
    [0xA1] = (void * (*)(void *))func_00263490,
    [0xA2] = (void * (*)(void *))func_00263460,
    [0xA3] = (void * (*)(void *))func_00263430,
    [0xA4] = (void * (*)(void *))func_00263400,
    [0xA5] = (void * (*)(void *))func_002633D0,
    [0xA6] = (void * (*)(void *))func_002633A0,
    [0xA7] = (void * (*)(void *))func_00263370,
    [0xA8] = (void * (*)(void *))func_00263340,
    [0xA9] = (void * (*)(void *))func_00263310,
    [0xAA] = (void * (*)(void *))func_002632E0,
    [0xAB] = (void * (*)(void *))func_002632B0,
    [0xAC] = (void *(*)(void *))func_00263220,
};

/* give n of item id: added to one already held if that kind stacks (+0x18), else made in the
 * group's first free slot with its count set (+0x2C, n - 1). The item, or NULL if it wouldn't
 * take them or the group is full */
/* 0x00261090 */
void *Items_Give(u8 *items, u32 id, s32 n) {
    u8 *grp = items + (id < 0x40 ? 0 : id < 0xA0 ? 1 : 2) * 0x100;
    void *o;
    u32 i;

    for (i = 0; i < 0x40; i++) {
        o = AT(grp, 0x12E0 + i * 4, void *);
        if (o == NULL) {
            break;
        }
        if (id == VCALL(o, 0xC, u32 (*)(void *))(o) && (VCALL(o, 0x18, u32 (*)(void *))(o) & 0xFF) == 1) {
            return VCALL(o, 0x2C, s32 (*)(void *, s32))(o, n) != 0 ? o : NULL;
        }
    }
    if (i == 0x40) {
        return NULL;
    }
    o = NULL;
    if (id < 0xAD && (kItemCtors[id] != NULL || id == 0x3F)) {
        void *pool = items + 0x1208;

        o = VCALL(pool, 0x10, void *(*)(void *, u32))(pool, 0x18);
        if (o != NULL) {
            void *p = func_0025FF00(0x18, o);

            if (p != NULL) {
                if (id == 0x3F) {
                    Item3F_ctor(p, (s32)o);   /* (sic: no id passed - a1 still the object) */
                } else {
                    kItemCtors[id](p);
                }
            }
        }
    }
    AT(grp, 0x12E0 + i * 4, void *) = o;
#ifdef HG_NATIVE
    if (o == NULL) {   /* an id with no class (the PS2 calls through NULL) */
        return NULL;
    }
#endif
    VCALL(o, 0x2C, s32 (*)(void *, s32))(o, (u8)((n & 0xFF) - 1));
    return o;
}

ItemObj *func_00263220(ItemObj *self) {
    self->vtbl = D_0046C790;
    self->id = 0xAC;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046D5F0;
    return self;
}

/* item 0x3F's class takes its id as an argument; Items_Give calls it without one, so the id
 * is whatever a1 held: the object's own address (see there) */
/* 0x00264060 */
ItemObj *Item3F_ctor(ItemObj *self, s32 id) {
    self->vtbl = D_0046C790;
    self->id = id;
    self->flag = 0;
    self->data = 0;
    self->vtbl = D_0046F430;
    return self;
}

/* 0x002D3A60 */
void Item3F_Set(u8 *p, const u8 *src) {
    u32 i;

    for (i = 0; i < 8; i++) {
        p[0x10 + i] = src[i];
    }
}

/* item `i` of list `l`'s +0x20 (0 for an empty place) */
/* 0x002604E0 */
s32 Items_Field20(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return 0;
    }
    return VCALL(it, 0x20, s32 (*)(VObject *))(it);
}

extern const s8 D_003EA918[];   /* by item kind (+0x10) */

/* item `i` of list `l`: D_003EA918 of its kind (+0x10); -1 for an empty place */
/* 0x00260630 */
s32 Items_KindSlot(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return -1;
    }
    return D_003EA918[VCALL(it, 0x10, s32 (*)(VObject *))(it)];
}

/* item `id` (+0xC) in the lists: its +0x28 with `arg` */
/* 0x00260170 */
void Items_Notify(u8 *o, s32 id, s32 arg) {
    u32 l, i;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 64; i++) {
            VObject *it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);

            if (it == NULL) {
                break;
            }
            if (id == VCALL(it, 0xC, s32 (*)(VObject *))(it)) {
                it = AT(o, 0x12E0 + l * 0x100 + i * 4, VObject *);
                VCALL(it, 0x28, void (*)(VObject *, s32))(it, arg);
                return;
            }
        }
    }
}

/* a new item 0x3F (one) set from `src` (Item3F_Set); the item, NULL if none */
/* 0x00261040 */
void *Items_NewItem3F(u8 *items, const u8 *src) {
    u8 *it = Items_Give(items, 0x3F, 1);

    if (it != NULL) {
        Item3F_Set(it, src);
    }
    return it;
}

/* item `i` of list `l` equipped: into its kind's slot (D_003EA918, +0x15E0) if it has one */
/* 0x00260840 */
void Items_Equip(u8 *items, u8 l, u8 i) {
    VObject **e = &AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);
    s32 k = *e == NULL ? -1 : D_003EA918[VCALL(*e, 0x10, s32 (*)(VObject *))(*e)];

    if (k >= 0) {
        AT(items, 0x15E0 + k * 4, VObject *) = *e;
    }
}

/* item `i` of list `l` unequipped: its kind's slot (+0x15E0) cleared if it holds it */
/* 0x002607A0 */
void Items_Unequip(u8 *items, u8 l, u8 i) {
    VObject **e = &AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);
    s32 k = *e == NULL ? -1 : D_003EA918[VCALL(*e, 0x10, s32 (*)(VObject *))(*e)];

    if (k >= 0 && AT(items, 0x15E0 + k * 4, VObject *) == *e) {
        AT(items, 0x15E0 + k * 4, VObject *) = NULL;
    }
}

/* item `i` of list `l`'s equipment: -1 its kind has no slot (or no item), 0 the slot is empty,
 * 1 it is the one equipped, 2 another is */
/* 0x002606E0 */
s32 Items_EquipState(u8 *items, u8 l, u8 i) {
    VObject **e = &AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);
    s32 k = *e == NULL ? -1 : D_003EA918[VCALL(*e, 0x10, s32 (*)(VObject *))(*e)];
    VObject *eq;

    if (k < 0) {
        return -1;
    }
    eq = AT(items, 0x15E0 + k * 4, VObject *);
    if (eq == NULL) {
        return 0;
    }
    return eq == *e ? 1 : 2;
}

extern void *D_0046C790[];   /* a pool entry */

/* item `it` taken out: off any equipment slot, out of its list (the rest moved up), destroyed
 * back to a bare pool entry and returned to the pool (+0x1208 vtable +0x14) */
/* 0x002608D0 */
void Items_Remove(u8 *items, VObject *it) {
    u32 g, i;
    s32 found;

    if (it == NULL) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (AT(items, 0x15E0 + i * 4, VObject *) == it) {
            AT(items, 0x15E0 + i * 4, VObject *) = NULL;
        }
    }
    for (g = 0; g < 3; g++) {
        found = 0;
        for (i = 0; i < 0x40; i++) {
            if (AT(items, 0x12E0 + g * 0x100 + i * 4, VObject *) == it) {
                found = 1;
                break;
            }
        }
        if (found) {
            break;
        }
    }
    if (i < 0x40) {
        u8 *grp = items + g * 0x100;

        for (; i < 0x3F; i++) {
            VObject *next = AT(grp, 0x12E0 + (i + 1) * 4, VObject *);

            if (next == NULL) {
                break;
            }
            AT(grp, 0x12E0 + i * 4, VObject *) = next;
        }
        AT(grp, 0x12E0 + i * 4, VObject *) = NULL;
    }
    VCALL(it, 0x8, void (*)(VObject *, s32))(it, 1);
    {
        u8 *b = func_0025FF00(0x18, it);

        if (b != NULL) {
            AT(b, 0x0, void **) = D_0046C790;
            AT(b, 0x4, s32) = -1;
            AT(b, 0x8, u8) = 0;
            AT(b, 0x10, u64) = 0;
        }
    }
    VCALL(items + 0x1208, 0x14, void (*)(void *, VObject *))(items + 0x1208, it);
}

extern void Items_Remove(u8 *items, VObject *it);   /* an item taken out of the lists */

/* one of item `it` used: a counted one (+0x18 1) with 2 or more (+0x34) loses one (+0x30), else
 * it goes (Items_Remove) */
static inline void item_use_one(u8 *items, VObject *it) {
    if ((u8)VCALL(it, 0x18, s32 (*)(VObject *))(it) == 1 && VCALL(it, 0x34, u32 (*)(VObject *))(it) >= 2) {
        VCALL(it, 0x30, void (*)(VObject *))(it);
    } else {
        Items_Remove(items, it);
    }
}

/* the item in equipment slot `slot` used once */
/* 0x00260A60 */
void Items_UseEquipped(u8 *items, u8 slot) {
    VObject *it = AT(items, 0x15E0 + slot * 4, VObject *);

    if (it != NULL) {
        item_use_one(items, it);
    }
}

/* (out of line, `l` / `i` kept in their registers as the original leaves them for difftest) */
static __attribute__((noinline)) void item_use_one_at(u8 *items, u32 l, u32 i, VObject *it) {
    (void)l;
    (void)i;
    item_use_one(items, it);
}

/* item `i` of list `l` used once */
/* 0x00260B00 */
void Items_UseOne(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it != NULL) {
        item_use_one_at(items, l, i, it);
    }
}

/* item `i` of list `l` used (vtable +0x3C, its result returned); when that gives bit 0 or 1,
 * one of it is spent */
/* 0x00260DD0 */
u32 Items_Use(u8 *items, u8 l, u8 i) {
    VObject **e = &AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);
    u32 r = 0;

    if (*e != NULL) {
        r = VCALL(*e, 0x3C, u32 (*)(VObject *))(*e) & 0xFF;
        if ((r & 3) && *e != NULL) {
            item_use_one(items, *e);
        }
    }
    return r;
}

/* list `l` sorted by vtable +0x1C (a bubble sort up to its first empty place) */
/* 0x0025FF80 */
void Items_Sort(u8 *items, u8 l) {
    u8 *grp = items + l * 0x100;
    u32 n, pass, j;

    for (n = 0; n < 0x40; n++) {
        if (AT(grp, 0x12E0 + n * 4, VObject *) == NULL) {
            break;
        }
    }
    if (n == 0) {
        return;
    }
    for (pass = 0; pass < n - 1; pass++) {
        u32 lim = n - 1 - pass;

        for (j = 0; j < lim; j++) {
            VObject **s = &AT(grp, 0x12E0 + j * 4, VObject *);
            u32 a = VCALL(s[0], 0x1C, u32 (*)(VObject *))(s[0]);
            u32 b = VCALL(s[1], 0x1C, u32 (*)(VObject *))(s[1]);

            if (b < a) {
                VObject *t = s[0];

                s[0] = s[1];
                s[1] = t;
            }
        }
    }
}

/* item `i` of list `l` started being used (vtable +0x28, with `arg`) */
/* 0x00260250 */
void Items_StartUse(u8 *items, u8 l, u8 i, void *arg) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it != NULL) {
        VCALL(it, 0x28, void (*)(VObject *, void *))(it, arg);
    }
}

/* item `i` of list `l`: its kind (vtable +0x10); -1 for an empty place */
/* 0x00260420 */
s32 Items_Kind(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return -1;
    }
    return VCALL(it, 0x10, s32 (*)(VObject *))(it);
}

/* item `i` of list `l`: what can be done with it (vtable +0x14: 1 use, 2 equip, 4 examine;
 * 0x80000000 its note can change); 0 for an empty place */
/* 0x002603C0 */
u32 Items_Actions(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return 0;
    }
    return VCALL(it, 0x14, u32 (*)(VObject *))(it);
}

/* item `i` of list `l`: its id (vtable +0xC); -1 for an empty place */
/* 0x00260480 */
s32 Items_Id(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return -1;
    }
    return VCALL(it, 0xC, s32 (*)(VObject *))(it);
}

/* item `i` of list `l`: how many (vtable +0x34; 0 for an empty place) */
/* 0x00260300 */
s32 Items_HowMany(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return 0;
    }
    return VCALL(it, 0x34, s32 (*)(VObject *))(it);
}

/* item `i` of list `l`: is it counted (vtable +0x18; 0 for an empty place) */
/* 0x00260360 */
u8 Items_IsCounted(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return 0;
    }
    return VCALL(it, 0x18, s32 (*)(VObject *))(it);
}

/* item `i` of list `l`: its 8 bytes of data (vtable +0x24; NULL for an empty place) */
/* 0x002602A0 */
u64 *Items_Data(u8 *items, u8 l, u8 i) {
    VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

    if (it == NULL) {
        return NULL;
    }
    return VCALL(it, 0x24, u64 *(*)(VObject *))(it);
}

/* how many items 0x3F the lists hold */
/* 0x00260540 */
s32 Items_CountItem3F(u8 *items) {
    u32 l, i;
    s32 n = 0;

    for (l = 0; l < 3; l++) {
        for (i = 0; i < 64; i++) {
            VObject *it = AT(items, 0x12E0 + l * 0x100 + i * 4, VObject *);

            if (it == NULL) {
                break;
            }
            if (VCALL(it, 0xC, s32 (*)(VObject *))(it) == 0x3F) {
                n++;
            }
        }
    }
    return n;
}
