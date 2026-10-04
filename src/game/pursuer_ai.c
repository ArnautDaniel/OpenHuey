/* Pursuer methods and helpers, code 0x211C80..0x219460, and the stalker vtables' defaults
 * (0x179600..0x179970). See include/pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"

extern VObject *D_0044E558;   /* doors */
extern VObject *D_0044E568;   /* rooms */

/* ---- defaults shared by the stalker vtables (0x179600..0x179970) ---- */

extern void func_0029FB20(Pursuer *p);

void func_00179600(Pursuer *p) {
    func_0029FB20(p);
}

/* vtable +0x11C */
s32 func_00179610(Pursuer *p) {
    return 0;
}

/* vtable +0x2AC .. +0x2B8: nothing */
void func_00179710(Pursuer *p) {
}

void func_00179720(Pursuer *p) {
}

void func_00179730(Pursuer *p) {
}

void func_00179740(Pursuer *p) {
}

/* vtable +0x2C0: timer +0x1660 to 600 frames (10 s) */
void func_00179750(Pursuer *p) {
    PU(p, 0x1660, s32) = 600;
}

/* vtable +0x2C8: timer +0x1660 to `frames`, 0 = 900 (15 s) */
void func_00179760(Pursuer *p, s32 frames) {
    PU(p, 0x1660, s32) = frames != 0 ? frames : 900;
}

/* vtable +0xA0 / +0xA4: turn rates, 4 and 8 degrees (in radians) */
f32 func_00179830(Pursuer *p) {
    return 0x1.1df46a0000000p-4f /* 0.06981317 */;
}

f32 func_00179850(Pursuer *p) {
    return 0x1.1df46a0000000p-3f /* 0.13962634 */;
}

/* vtable +0x2DC .. +0x2FC: distances and factors */
f32 func_00179870(Pursuer *p) {
    return 60.0f;
}

f32 func_00179880(Pursuer *p) {
    return 0x1.eb851e0000000p-4f /* 0.12 */;
}

f32 func_001798A0(Pursuer *p) {
    return 20.0f;
}

f32 func_001798B0(Pursuer *p) {
    return 20.0f;
}

f32 func_001798C0(Pursuer *p) {
    return 20.0f;
}

f32 func_001798D0(Pursuer *p) {
    return 16.0f;
}

f32 func_001798E0(Pursuer *p) {
    return 24.0f;
}

f32 func_001798F0(Pursuer *p) {
    return 0x1.6666660000000p+0f /* 1.4 */;
}

f32 func_00179910(Pursuer *p) {
    return 0x1.3333340000000p-1f /* 0.6 */;
}

s32 func_00179930(Pursuer *p) {
    return -1;
}

s32 func_00179940(Pursuer *p) {
    return -1;
}

/* vtable +0x314 */
s32 func_00179950(Pursuer *p) {
    return 0;
}

extern void func_00212400(Pursuer *p);

/* vtable +0xE8 */
void func_00179960(Pursuer *p) {
    func_00212400(p);
}

/* ---- 0x211C80..0x219460 ---- */

/* vtable +0xE4: nothing */
void func_00212540(Pursuer *p) {
}

/* shut / open door `door` (doors vtable +0x1C) */
s32 func_00212DC0(Pursuer *p, s32 door) {
    return VCALL(D_0044E558, 0x1C, s32 (*)(VObject *, s32, s32, s32))(D_0044E558, door, 0, 0x60000);
}

s32 func_00212DE0(Pursuer *p, s32 door) {
    return VCALL(D_0044E558, 0x1C, s32 (*)(VObject *, s32, s32, s32))(D_0044E558, door, 1, 0x60000);
}

/* the room's side behind the exit the pursuer heads for (rooms vtable +0x50) */
s32 func_00217340(Pursuer *p) {
    return VCALL(D_0044E568, 0x50, s32 (*)(VObject *, s32, u32, s32))(D_0044E568, p->c.a.room, p->c.door, 1);
}

/* vtable +0xC8: reacting to a noise */
s32 func_002181C0(Pursuer *p) {
    return p->c.heardSlot != 0xFF;
}

/* vtable +0xA8: blocking nav triangle flags */
u32 func_00219450(Pursuer *p) {
    return 0x2C020028;
}

/* ---- batch 2 ---- */

extern Progress *gProgress;
extern Character *gCharPlayer;    /* Fiona */
extern VObject *D_0044E4D0;       /* room objects */
extern void *D_0044E570;          /* nav mesh */
extern VObject *gSceneGameF29740; /* path planner */

extern void func_00178DB0(Progress *pr, s32 room, s32 a2, u32 slot);
extern void func_00178C10(Progress *pr, s32 room, s32 a2, u32 slot);
extern void func_00178A90(Progress *pr, s32 room, s32 door, s32 a3);
extern void func_002E2C10(f32 *out, f32 angle);   /* unit vector of a heading */
extern u32 func_00124480(Actor *a, const f32 *target, u32 mask);
extern s32 func_00123470(void *self, u32 tri, f32 *pos);
extern f32 *func_0017CE80(u8 *skel, s32 bone);
extern s32 func_00127140(Character *c, s32 kind, u32 goalTri, const f32 *goal);
extern s32 func_001270A0(Character *c);
extern void func_00127060(Character *c);
extern s32 func_00126F80(Character *c, s32 target, s32 unused2, s32 side, s32 unused4);
extern u32 func_00123710(void *self, s32 door, s32 side, const f32 *ofs, f32 *out);

/* `a2` for the pursuer in its room (progress) */
void func_00211C80(Pursuer *p, s32 a2) {
    Progress *pr = gProgress;

    func_00178DB0(pr, p->c.a.room, a2, *(u8 *)&p->c.a.slot);
    func_00178C10(pr, p->c.a.room, a2, *(u8 *)&p->c.a.slot);
}

/* door `door` shut, the other side ... (doors +0x20 / +0x1C, progress) */
void func_00212CA0(Pursuer *p, u32 door) {
    VObject *d = D_0044E558;
    Progress *pr;

    VCALL(d, 0x20, void (*)(VObject *, u32, s32, s32))(d, door, 1, 0x60000);
    VCALL(d, 0x1C, void (*)(VObject *, u32, s32, s32))(d, door, 0, 0x60000);
    pr = gProgress;
    func_00178A90(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), door, 0xFF);
}

void func_00212D30(Pursuer *p, u32 door) {
    VObject *d = D_0044E558;
    Progress *pr;

    VCALL(d, 0x20, void (*)(VObject *, u32, s32, s32))(d, door, 0, 0x60000);
    VCALL(d, 0x1C, void (*)(VObject *, u32, s32, s32))(d, door, 1, 0x60000);
    pr = gProgress;
    func_00178C10(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), door, 0xFF);
}

/* plan a path to the point beside door `door` (vtable +0x9C offset); 1 if +0xDC agrees */
s32 func_00212F40(Pursuer *p, u32 door, u32 side) {
    f32 ofs[4] __attribute__((aligned(16)));

    VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, side & 0xFF, ofs);
    PU(p, 0x15C4, s32) = func_00123710(p, door & 0xFF, side & 0xFF, ofs, (f32 *)((u8 *)p + 0x15D0));
    if (PU(p, 0x15C4, s32) == -1) {
        return 0;
    }
    return (VCALL(p, 0xDC, s32 (*)(Pursuer *))(p) & 0xFF) == 1;
}

/* plan a path to triangle +0x15A4 / point +0x15B0; its length to +0x1590 (-1: none) */
s32 func_00212360(Pursuer *p) {
    if (func_00127140(&p->c, 0, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0)) <= 0) {
        return 0;
    }
    if (func_001270A0(&p->c) > 0) {
        PU(p, 0x1590, f32) = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        func_00127060(&p->c);
        return 1;
    }
    PU(p, 0x1590, f32) = -1.0f;
    func_00127060(&p->c);
    return 0;
}

/* height of the bone vtable +0x80 of the model above its base (+0x804), at least 3 */
void func_00213E30(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    u8 *m = p->c.motion;
    s32 bone = VCALL(m, 0x80, s32 (*)(void *))(m);
    f32 h;

    sceVu0CopyVector(v, func_0017CE80(MOTION_AT(p, 0x810, u8 *), bone) + 0xC);
    h = v[1] - MOTION_AT(p, 0x804, f32);
    if (h < 3.0f) {
        h = 3.0f;
    }
    p->c.a.height = h;
}

/* is nav triangle `tri` blocked for the pursuer? */
s32 func_00214A90(Pursuer *p, u32 tri) {
    u32 flags;

    if (tri < AT(D_0044E570, 0x8, u32) && AT(D_0044E570, 0x4, u8 *) != NULL) {
        flags = AT(AT(D_0044E570, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = 0;
    }
    return (p->c.a.navMask & flags) != 0;
}

/* plan a path to the room object behind the exit the pursuer heads for */
s32 func_00214AF0(Pursuer *p) {
    VObject *o = VCALL(D_0044E4D0, 0x64, VObject *(*)(VObject *))(D_0044E4D0);
    s32 *t = VCALL(o, 0x3C, s32 *(*)(VObject *))(o);

    if (t != NULL) {
        s32 r = t[p->c.door];

        if (r != -1) {
            PU(p, 0x1594, s32) = r;
            PU(p, 0x1598, s32) = -1;
            if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0) {
                return 1;
            }
        }
    }
    return 0;
}

/* reached the room +0x1594 (and side +0x1598, -1 any)? */
s32 func_00217260(Pursuer *p) {
    s32 room = p->c.a.room;

    if (PU(p, 0x1594, s32) == room) {
        if (PU(p, 0x1598, s32) != -1) {
            s32 side = VCALL(D_0044E568, 0x50, s32 (*)(VObject *, s32, u32, s32))(D_0044E568, room, p->c.door, 1);

            if (side != -1) {
                return side == PU(p, 0x1598, s32);
            }
        }
        return 1;
    }
    return 0;
}

/* the side of character `c`'s room behind its exit, -1 without one */
s32 func_002172F0(Pursuer *p, Character *c) {
    if (c != NULL) {
        return VCALL(D_0044E568, 0x50, s32 (*)(VObject *, s32, u32, s32))(D_0044E568, c->a.room, c->door, 1);
    }
    return -1;
}

/* in the room being played? */
s32 func_00217510(Pursuer *p) {
    return p->c.a.room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
}

/* is Fiona panicking (fear over 90, or her state 0xE)? */
s32 func_00217560(void) {
    s32 r = 1;

    if (AT(gCharPlayer, 0x1AD5F4, f32) <= 90.0f) {
        r = 0;
    }
    if (r == 0) {
        r = AT(gCharPlayer, 0x1AD580, s32) == 0xE;
    }
    return r;
}

/* is `b` on about the same floor as `a` (up to 15 above, 10 below)? */
s32 func_002175B0(Actor *a, Actor *b) {
    s32 r = 0;

    if (b->pos[1] <= a->pos[1] + 15.0f && !(b->pos[1] + 10.0f < a->pos[1])) {
        r = 1;
    }
    return r;
}

/* the walkable triangle one height ahead (along the heading); 0 if none */
s32 func_00217600(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    u32 tri;

    func_002E2C10(v, p->c.a.angle[1]);
    sceVu0ScaleVector(v, v, p->c.a.height);
    sceVu0AddVector(v, v, p->c.a.pos);
    tri = func_00124480(&p->c.a, v, p->c.a.navMask);
    if (tri == (u32)-1) {
        return 0;
    }
    return func_00123470(p, tri, v);
}

/* add nav triangle `tri` to the route list (+0x15E0, 8 entries of 8 bytes) */
s32 func_00218C20(Pursuer *p, u32 tri) {
    u8 n;

    if (PU(p, 0x1621, u8) == 0xFF) {
        PU(p, 0x1621, u8) = 0;
        PU(p, 0x1620, u8) = 0;
    }
    n = PU(p, 0x1621, u8);
    if (n < 8 && tri < AT(D_0044E570, 0x8, u32)) {
        PU(p, 0x15E0 + n * 8, u32) = tri;
        PU(p, 0x1621, u8)++;
        return 1;
    }
    return 0;
}

/* aim at the current entry of the route list: triangle +0x15A4, its centre to +0x15B0 */
void func_00218E70(Pursuer *p) {
    u8 i = PU(p, 0x1620, u8);

    if (i < PU(p, 0x1621, u8)) {
        PU(p, 0x15A4, s32) = PU(p, 0x15E0 + i * 8, s32);
        VCALL(D_0044E570, 0xC, void (*)(void *, s32, f32 *, Pursuer *))(D_0044E570, PU(p, 0x15E0 + PU(p, 0x1620, u8) * 8, s32), (f32 *)((u8 *)p + 0x15B0), p);
    }
}
