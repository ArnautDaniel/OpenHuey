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
