/* The creatures (character slots 7..9, 0x1600 bytes; their manager is D_0044F258): Character
 * with their own block at +0x1540. Two classes: D_0046FAA0 (here) and D_00474080, both on the
 * creature base D_0046FB50. Their state is kept across rooms in Progress +0x878 (36 bytes per
 * slot, func_002DE840). */
#include "common.h"
#include "game.h"
#include "actor.h"
#include "progress.h"
#include "sce/libvu0.h"

extern void *D_0046FAA0[], *D_0046FB50[], *D_00469C60[], *D_00469C20[];
extern void func_00124DA0(Actor *a);
extern void func_00127660(Character *c);
extern s32 func_00125AD0(Character *c, u32 tri, const f32 *heading, f32 *pos);
extern void func_00125CC0(Character *c);

#define CR(c) ((u8 *)(c) + 0x1540)   /* the creature's own block */

/* ---- the creature base (D_0046FB50): defaults ---- */

void func_002E2260(Character *c) {   /* +0xA8 */
}

s32 func_002E2270(Character *c) {    /* +0x3C */
    return 1;
}

void func_002E2280(Character *c) {   /* +0xA4 */
}

void func_002E2290(Character *c) {   /* +0xA0 */
}

void func_002E22A0(Character *c) {   /* +0x9C */
}

void func_002E22B0(Character *c) {   /* +0x38 */
}

void func_002E22C0(Character *c) {   /* +0x34 */
}

void func_002E22D0(Character *c) {   /* +0x40 */
}

void func_002E22E0(Character *c) {   /* +0x30 */
}

void func_002E22F0(Character *c) {   /* +0x2C */
}

/* +0x10 */
void func_002E2300(Character *c) {
    func_00124DA0(&c->a);
}

/* +0xC */
void func_002E2310(Character *c) {
    func_00127660(c);
}

/* operator delete for objects put in place (nothing to free) */
void func_002E2320(void *p) {
}

/* placement new */
void *func_002E2330(u32 size, void *place) {
    return place;
}

/* ---- the creature (D_0046FAA0) ---- */

/* +0x8 destructor */
Character *func_002DE490(Character *c, s32 flags) {
    if (c != NULL) {
        AT(c, 0x0, void **) = D_0046FAA0;
        if (c != NULL) {
            AT(c, 0x0, void **) = D_0046FB50;
            if (c != NULL) {
                AT(c, 0x0, void **) = D_00469C60;
                if (c != NULL) {
                    AT(c, 0x0, void **) = D_00469C20;
                }
            }
        }
        if ((s16)flags > 0) {
            func_002E2320(c);
        }
    }
    return c;
}

void func_002E2020(Character *c) {   /* +0x10 */
}

void func_002E2010(Character *c) {   /* +0x1C */
}

void func_002E2000(Character *c) {   /* +0x20 */
}

void func_002DFE00(Character *c) {   /* +0x84 */
}

void func_002DFDF0(Character *c) {   /* +0x88 */
}

void func_002E0510(Character *c) {   /* a state with nothing to do */
}

/* +0x28 put on triangle `tri` (func_00125AD0), remembering it as the previous one and the
   position (+0x38 / +0x40) */
s32 func_002E19A0(Character *c, u32 tri, const f32 *heading, f32 *pos) {
    s32 r = func_00125AD0(c, tri, heading, pos);

    c->a.prevNavTri = tri;
    sceVu0CopyVector(c->a.prevPos, c->a.pos);
    return r;
}

/* +0x5C reset (func_00125CC0): not hit (+0xB), no target (+0x9 0xFF), mode 2 (+0xC) */
void func_002E0BF0(Character *c) {
    u8 *k = CR(c);

    func_00125CC0(c);
    AT(k, 0xB, u8) = 0;
    AT(k, 0x9, u8) = 0xFF;
    AT(k, 0xC, s32) = 2;
}

/* +0x40: copies of its orientation and position into locals (left unused) */
void func_002E1340(Character *c) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));

    sceVu0CopyMatrix(m, c->a.rot);
    sceVu0CopyVector(v, c->a.pos);
}

extern Character *gCharacters[];
extern Character *gCharPlayer;
extern VObject *D_0044E550;   /* random numbers: +0x10 an integer */
extern VObject *D_0044E568;   /* the rooms */
extern s32 func_00126F80(Character *c, s32 target, s32 unused2, s32 side, s32 unused4);

/* per kind (+0x1571): +0x8 a byte, +0x0 a float, +0x6 a short */
typedef struct CreatureKind {
    u8 b;
    u8 pad[3];
    f32 f;
    s16 s;
    u8 pad2[2];
} CreatureKind;

extern const CreatureKind D_004164F0[];

/* its rest time (+0x80) when it isn't out (+0x82): 1800 .. 7200 frames */
static inline void creature_rest(u8 *k) {
    AT(k, 0x80, s16) = ((VCALL(D_0044E550, 0x10, s32 (*)(VObject *))(D_0044E550) & 3) + 1) * 1800;
}

/* +0xA0 set up: mode `mode` (+0xC), kind `kind` (+0x31) with its table entry, strength
 * `str` x10 (+0x4); from save slot `slot` (-1: none) when that one is in use (+0x88A): its
 * state (+0x20), strength, out (+0x82, then a rest time), +0x2E, +0x87, +0x9A. Kind 0x24:
 * +0x14 -9. (+0x64 first: the Character's reset, a1..a3 passed on) */
void func_002DE8E0(Character *c, s32 a1, s32 a2, s32 mode, s32 kind, s32 str, s32 slot) {
    u8 *k = CR(c);
    const CreatureKind *t;

    VCALL(c, 0x64, void (*)(Character *, s32, s32, s32))(c, a1, a2, mode);
    AT(k, 0xC, s32) = mode;
    AT(k, 0x82, u8) = 0;
    AT(k, 0x80, s16) = 0;
    AT(k, 0x2A, u8) = 0;
    c->unk14C4 = 0;
    AT(k, 0x2E, u8) = 0;
    AT(k, 0x31, u8) = kind;
    AT(k, 0x4, s16) = (s16)str * 10;
    t = &D_004164F0[AT(k, 0x31, u8)];
    AT(k, 0x8, u8) = t->b;
    AT(k, 0x0, f32) = t->f;
    AT(k, 0x6, s16) = t->s;
    if (slot != -1) {
        u8 *e = (u8 *)gProgress + slot * 36 + 0x878;

        if (AT(e, 0x12, u8) != 0) {
            AT(k, 0x20, s16) = AT(e, 0xC, s16);
            AT(k, 0x4, s16) = AT(e, 0xF, u8) * 10;
            AT(k, 0x82, u8) = AT(e, 0x10, u8);
            AT(k, 0x2E, u8) = AT(e, 0x11, u8);
            AT(k, 0x87, u8) = AT(e, 0x13, u8);
            AT(k, 0x9A, u8) = AT(e, 0x14, u8);
            if (AT(k, 0x82, u8) != 0) {
                creature_rest(k);
            }
        }
    }
    if (AT(k, 0x31, u8) == 0x24) {
        AT(k, 0x14, f32) = -9.0f;
    }
}

/* +0x9C set up to come after Fiona: (+0x64 with mode 2) +0x2A whether someone (slots 2..5) is
 * up in her room; out, with a rest time; a path to her room (func_00126F80) and its length
 * through the doors on it (rooms +0x38 by `a1`) into +0x14C4, the last door +0x14C0 */
void func_002DEA80(Character *c, s32 a1, s32 a2) {
    u8 *k = CR(c);
    u32 i;
    s32 n;

    VCALL(c, 0x64, void (*)(Character *, s32, s32, s32))(c, a1, a2, 2);
    AT(k, 0x2A, u8) = 0;
    for (i = 2; i < 6; i = (i + 1) & 0xFF) {
        Character *o = gCharacters[i & 0xFF];

        if (o != NULL && o->a.active == 1 && o->a.disabled == 0 && gCharPlayer->a.room == gCharacters[i & 0xFF]->a.room) {
            AT(k, 0x2A, u8) = 1;
        }
    }
    AT(k, 0x2E, u8) = 0;
    AT(k, 0x82, u8) = 1;
    creature_rest(k);
    if (func_00126F80(c, gCharPlayer->a.room, -1, AT(k, 0xC, s32), -1) == -1) {
        return;
    }
    for (n = 0; n < c->unk1384; n++) {
        VObject *rooms = D_0044E568;
        u16 door = AT(c->unk138C, n * 2, u16);

        AT(&c->unk14C4, 0, f32) += (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, door, a1);
        c->unk14C0 = AT(c->unk138C, n * 2, u16);
    }
}
