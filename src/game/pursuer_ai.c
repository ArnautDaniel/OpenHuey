/* Pursuer methods and helpers, code 0x211C80..0x219460, and the stalker vtables' defaults
 * (0x179600..0x179970). See include/pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"

extern VObject *D_0044E550;   /* random numbers */
extern VObject *D_0044E558;   /* doors */
extern VObject *D_0044E568;   /* rooms */

/* ---- defaults shared by the stalker vtables (0x179600..0x179970) ---- */


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

/* ---- batch 3 ---- */

extern Character *gCharacters[6];
extern Character *gCharPartner;   /* Hewie */


/* vtable +0x9C: offset of the point beside a door, by side (Lorenzo's wheelchair etc. differ) */
void func_00179780(Pursuer *p, s32 side, f32 *out) {
    switch (side) {
    case 0:
        AT(out, 0x0, u32) = 0;
        AT(out, 0x4, u32) = 0;
        AT(out, 0x8, u32) = 0xC0C00000;   /* -6.0 */
        break;
    case 1:
        AT(out, 0x0, u32) = 0x3F23D70A;   /* 0.64 */
        AT(out, 0x4, u32) = 0;
        AT(out, 0x8, u32) = 0x41266666;   /* 10.4 */
        break;
    case 2:
        AT(out, 0x0, u32) = 0xBED4AF4F;   /* -0.4154 */
        AT(out, 0x4, u32) = 0;
        AT(out, 0x8, u32) = 0x41202F1B;   /* 10.0115 */
        break;
    case 3:
        AT(out, 0x0, u32) = 0x3F23D70A;   /* 0.64 */
        AT(out, 0x4, u32) = 0;
        AT(out, 0x8, u32) = 0xC0F23055;   /* -7.568 */
        break;
    }
}

/* vtable +0x128 (Debilitas: own): the stand animation by +0x16C8 */
void func_00179620(Pursuer *p) {
    u8 k = PU(p, 0x16C8, u8);

    switch (k) {
    case 0:
    case 1:
    case 2:
    case 4:
        func_00297B40(p, VCALL(p, 0x328, s32 (*)(Pursuer *, u32))(p, k), 0);
        break;
    case 3: {
        u8 j = PU(p, 0x16C9, u8);

        if (j != 0 && j != 2) {
            func_00297B40(p, VCALL(p, 0x328, s32 (*)(Pursuer *, u32))(p, k), 0);
        } else {
            func_00297B40(p, VCALL(p, 0x324, s32 (*)(Pursuer *, u32))(p, k), 0);
        }
        break;
    }
    }
}

/* path length from node `a` to node `b` of `room`, through the room's table (-1 none) */
f32 func_00211F70(Pursuer *p, s32 room, u32 a, u32 b) {
    VObject *rm = D_0044E568;
    s16 n = VCALL(rm, 0x38, s32 (*)(VObject *, u32, s32))(rm, b, room);
    f32 d;

    if (n == -1) {
        return -1.0f;
    }
    d = n;
    if ((a & 0xFFFF) != (b & 0xFFFF)) {
        n = VCALL(rm, 0x38, s32 (*)(VObject *, u32, s32))(rm, a, room);
        if (n == -1) {
            return -1.0f;
        }
        d += n;
    }
    return d;
}

/* is `pos` of room `room` where the room's spawn point is, and reachable? */
s32 func_00212190(Pursuer *p, s32 room) {
    VObject *rm = D_0044E568;
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    u32 tri = VCALL(rm, 0x30, u32 (*)(VObject *, s32, f32 *))(rm, room, a);

    VCALL(rm, 0x34, void (*)(VObject *, s32, f32 *))(rm, room, b);
    if (tri != func_00124480(&p->c.a, a, 0)) {
        return 0;
    }
    return (func_00123C60(&p->c.a, room, b) & 0xFF) != 0;
}

/* the first open door (0..4) while Fiona is hiding (move mode 3), -1 none */
s32 func_002131A0(void) {
    if (gCharPlayer->moveMode == 3) {
        Progress *pr = gProgress;
        s32 i;

        for (i = 0; (u32)i < 5; i++) {
            s32 valid = i >= 0 && (u32)i < AT(D_0044E570, 0x14, u32);

            if ((valid & 0xFF) == 1 && (func_00177A20(pr, i & 0xFF, 0) & 0xFF & 1)) {
                return i;
            }
        }
    }
    return -1;
}

/* path length to triangle `tri` / point `pos`, from the nearest walkable triangle if blocked */
f32 func_00213C60(Pursuer *p, u32 tri, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));
    u32 flags;

    if (tri < AT(D_0044E570, 0x8, u32) && AT(D_0044E570, 0x4, u8 *) != NULL) {
        flags = AT(AT(D_0044E570, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = 0;
    }
    if (p->c.a.navMask & flags) {
        tri = func_00216E00(p, tri, pos, v);
    } else {
        sceVu0CopyVector(v, pos);
    }
    if (tri == (u32)-1) {
        return -1.0f;
    }
    return func_001257B0(&p->c, tri, v, VCALL(p, 0xA8, u32 (*)(Pursuer *))(p));
}

/* how far to character `c` on foot: straight if in sight on its triangle, else by path */
f32 func_00213D40(Pursuer *p, Character *c) {
    f32 v[4] __attribute__((aligned(16)));
    void *nm;
    u32 tri;

    if (p->c.a.room != c->a.room) {
        return -1.0f;
    }
    tri = c->a.navTri;
    nm = D_0044E570;
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, const f32 *))(nm, tri, c->a.pos) == 4) {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, tri, v);
        if (tri == func_00124480(&p->c.a, v, p->c.a.navMask)) {
            return func_00124490(&p->c.a, c->a.pos);
        }
    }
    return VCALL(p, 0xD4, f32 (*)(Pursuer *, u32, const f32 *))(p, tri, c->a.pos);
}

/* which way to turn to face `heading`: 1 left beyond `a`, 0 right beyond -`a`, 0xFF within;
 * bit 1 when more than `b` off (if `b` >= `a`) */
u32 func_00213EC0(Pursuer *p, f32 heading, f32 a, f32 b) {
    f32 d;
    u32 r;

    if (a <= 0.0f) {
        a = -a;
    }
    if (b <= 0.0f) {
        b = -b;
    }
    d = func_002E2D00(heading - p->c.a.angle[1]);
    if (d <= a) {
        if (!(d < -a)) {
            return 0xFF;
        }
        r = 0;
    } else {
        r = 1;
    }
    if (b < a) {
        return r;
    }
    if (d <= 0.0f) {
        d = -d;
    }
    if (d <= b) {
        return r;
    }
    return (r | 2) & 0xFF;
}

/* turn towards `heading` by at most `step`; the angle left */
f32 func_002140A0(Pursuer *p, f32 heading, f32 step) {
    f32 cur = p->c.a.angle[1];
    f32 d = func_002E2D00(heading - cur);
    f32 s = step <= 0.0f ? -step : step;
    f32 ad = d <= 0.0f ? -d : d;
    f32 h;

    if (ad <= s) {
        h = heading;
    } else if (!(d < 0.0f)) {
        h = func_002E2D00(cur + step);
    } else {
        h = func_002E2D00(cur - step);
    }
    p->c.a.angle[1] = h;
    func_002E3190(p->c.a.rot, h);
    return func_002E2D00(heading - h);
}

/* step along the path; the stride from the animation's root motion if `step` <= 0 */
s32 func_00214890(Pursuer *p, u32 *triOut, f32 *posOut, f32 step) {
    if (step <= 0.0f) {
        f32 v[4] __attribute__((aligned(16)));
        u8 *m;

        func_001F6370(p->c.motion, v, 0.0f);
        m = p->c.motion;
        step = v[2] * VCALL(m, 0x44, f32 (*)(void *, Pursuer *))(m, p);
    }
    if (step < 0.0f) {
        return -1;
    }
    return func_001273D0(&p->c, triOut, posOut, step);
}

/* path length to triangle `tri` / point `pos` (null: the triangle's centre), -1 none */
f32 func_00214B90(Pursuer *p, u32 tri, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));

    if (pos != NULL) {
        sceVu0CopyVector(v, pos);
    } else {
        VCALL(D_0044E570, 0xC, void (*)(void *, u32, f32 *))(D_0044E570, tri, v);
    }
    if (func_00127140(&p->c, 0, tri, v) <= 0) {
        return -1.0f;
    }
    if (func_001270F0(&p->c) > 0) {
        f32 d = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);

        func_00127060(&p->c);
        return d;
    }
    func_00127060(&p->c);
    return -1.0f;
}

/* are the pursuer and `c` in the same room but on different sides? */
s32 func_00217370(Pursuer *p, Character *c) {
    s32 room = p->c.a.room;

    if (room == c->a.room && p->c.door < 8 && (c->door & 0xFF) < 8) {
        VObject *rm = D_0044E568;
        s32 a = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, room, p->c.door, 1);
        s32 b = -1;

        if (a != -1) {
            if (c != NULL) {
                b = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, c->a.room, c->door, 1);
            }
            if (b != -1) {
                return b != a;
            }
        }
    }
    return 0;
}

/* is character `slot` in the pursuer's room or a neighbouring one? */
s32 func_00217460(Pursuer *p, s32 slot) {
    s32 room = gCharacters[slot]->a.room;
    VObject *rm;
    u32 i;

    if (room == p->c.a.room) {
        return 1;
    }
    rm = D_0044E568;
    for (i = 0; i < 8; i = (i + 1) & 0xFF) {
        if (room == VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i)) {
            return 1;
        }
    }
    return 0;
}

/* who can the pursuer see (bit per character slot 0..2)? */
s32 func_00217920(Pursuer *p) {
    s32 seen = 0;
    u32 i;

    for (i = 0; i < 3; i = (i + 1) & 0xFF) {
        Character *c = gCharacters[i & 0xFF];

        if (c != NULL && (i & 0xFF) != (u32)p->c.a.slot && c->a.active != 0 &&
            (func_001241F0(&p->c.a, &c->a, 1.0f, 0.0f) & 0xFF) == 1) {
            seen = (seen | ((1 << (i & 0xFF)) & 0xFF)) & 0xFF;
        }
    }
    return seen;
}

/* is the point `dist` away in direction `angle` from the target's heading walkable? */
s32 func_00217ED0(Pursuer *p, f32 angle, f32 dist) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    u32 tri;

    func_002E2C10(v, func_002E2D00(angle + func_001244D0(&p->c.a, p->target->a.pos)));
    sceVu0ScaleVector(v, v, dist);
    sceVu0AddVector(w, p->c.a.pos, v);
    tri = func_00124480(&p->c.a, w, p->c.a.navMask);
    if (tri != (u32)-1) {
        u32 flags;

        if (tri < AT(D_0044E570, 0x8, u32) && AT(D_0044E570, 0x4, u8 *) != NULL) {
            flags = AT(AT(D_0044E570, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
        } else {
            flags = 0;
        }
        if (!(p->c.a.navMask & flags)) {
            return 1;
        }
    }
    return 0;
}

/* probe the 8 directions around Fiona, 20 units out (results to +0x1548) */
void func_00218110(Pursuer *p) {
    s32 i;

    for (i = 0; i < 8; i++) {
        f32 h = func_001244D0(&p->c.a, gCharPlayer->a.pos);

        PU(p, 0x1548 + i * 4, s32) = func_00217D30(p, func_002E2D00(0x1.921fb60000000p+2f /* 6.2831855 */ * (f32)i / 8.0f + h), 20.0f);
    }
}

/* is Hewie close enough to be caught (in reach and on the same walkable triangle)? */
s32 func_00218940(Pursuer *p) {
    Character *h = gCharPartner;

    if (func_00218430(p, h) != 0) {
        return 1;
    }
    if (!(Progress_TestFlag(gProgress, 0xB) & 0xFF)) {
        f32 d = PU(p, 0x158C, f32);
        s32 near = 0;

        if (d < 20.0f && !(d <= 0.0f)) {
            near = 1;
        }
        if ((p->c.a.unk2B == 0) & 0xFF & (near & 0xFF)) {
            u32 tri = h->a.navTri;

            if (func_00124480(&p->c.a, h->a.pos, 0x40080) == tri) {
                return 1;
            }
        }
    }
    return 0;
}

/* drop the first route entry; the new current one, -1 none */
s32 func_00218B60(Pursuer *p) {
    u8 n = PU(p, 0x1621, u8);
    u8 i;

    if (n < PU(p, 0x1620, u8) || n == 0xFF) {
        return -1;
    }
    {
        u32 k;

        for (k = 0; k < PU(p, 0x1621, u8); k++) {
            PU(p, 0x15E0 + k * 8, s32) = PU(p, 0x15E8 + k * 8, s32);
            PU(p, 0x15E4 + k * 8, u8) = PU(p, 0x15EC + k * 8, u8);
        }
    }
    PU(p, 0x1621, u8)--;
    if (PU(p, 0x1620, u8) > 0) {
        PU(p, 0x1620, u8)--;
    }
    i = PU(p, 0x1620, u8);
    n = PU(p, 0x1621, u8);
    if (n >= i && n != 0xFF) {
        return PU(p, 0x15E0 + i * 8, s32);
    }
    return -1;
}

/* head for the room / side next to Hewie's (exit `exit` of his room); else keep the goal */
void func_00218C90(Pursuer *p, u32 exit) {
    Character *h = gCharPartner;
    VObject *rm = D_0044E568;
    s32 room = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, h->a.room, exit);
    s32 side = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, room, VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, h->a.room, exit) & 0xFF, 1);

    if (func_00126F80(&p->c, room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = side;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* the same next to Fiona's room */
void func_00218D80(Pursuer *p, u32 exit) {
    Character *f = gCharPlayer;
    VObject *rm = D_0044E568;
    s32 room = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, f->a.room, exit);
    s32 side = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, room, VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, f->a.room, exit) & 0xFF, 1);

    if (func_00126F80(&p->c, room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = side;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* ---- batch 4 ---- */


/* door / exit `exit`: what to do with it (vtable +0xF0 to go through); 2 / 1 / 0 */
s32 func_00211CF0(Pursuer *p, s32 exit) {
    Progress *pr;

    switch (func_00211E00(p, exit) & 0xFF) {
    case 2:
        return 2;
    case 6:
        pr = gProgress;
        func_00178DB0(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        func_00178750(pr, p->c.a.room, exit);
        /* fallthrough */
    case 4:
        if (!(VCALL(D_0044E568, 0x78, s32 (*)(VObject *, s32, s32))(D_0044E568, p->c.a.room, exit) & 0xFF)) {
            return 1;
        }
        /* fallthrough */
    case 5:
        VCALL(p, 0xF0, void (*)(Pursuer *, s32))(p, exit);
        return 1;
    default:
        return 0;
    }
}

/* path length between the room nodes of `a` and `b` (rooms +0x10 / +0x38), -1 none */
f32 func_00212060(Pursuer *p, s32 room, s32 a, s32 b) {
    VObject *rm = D_0044E568;
    u32 na = VCALL(rm, 0x10, u32 (*)(VObject *, s32, s32))(rm, room, a) & 0xFFFF;
    u32 nb = VCALL(rm, 0x10, u32 (*)(VObject *, s32, s32))(rm, room, b) & 0xFFFF;
    s16 n = VCALL(rm, 0x38, s32 (*)(VObject *, u32, s32))(rm, nb, room);
    f32 d;

    if (n == -1) {
        return -1.0f;
    }
    d = n;
    if ((na & 0xFFFF) != (nb & 0xFFFF)) {
        n = VCALL(rm, 0x38, s32 (*)(VObject *, u32, s32))(rm, na, room);
        if (n == -1) {
            return -1.0f;
        }
        d += n;
    }
    return d;
}

/* coming into room `room`: is Fiona at the spawn point there (+0x1624)? */
void func_00212240(Pursuer *p, s32 room) {
    VObject *rm = D_0044E568;
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));

    PU(p, 0x1624, s32) = 0;
    VCALL(rm, 0x30, u32 (*)(VObject *, s32, f32 *))(rm, room, a);
    VCALL(rm, 0x34, void (*)(VObject *, s32, f32 *))(rm, room, b);
    if ((func_00123C60(&p->c.a, room, b) & 0xFF) == 1) {
        if (func_00124480(&p->c.a, a, -1) != (u32)-1) {
            p->c.unk124 = p->c.unk128;
        }
        if (!(func_00178300(gProgress, p->c.a.room, room, *(u8 *)&p->c.a.slot) & 0xFF)) {
            return;
        }
        if ((func_00123C60(&p->c.a, room, gCharPlayer->a.pos) & 0xFF) == 1) {
            PU(p, 0x1624, s32) = 1;
        }
    }
}

/* heading through exit `exit` (from its inner to its outer point) */
f32 func_00212730(Pursuer *p, s32 exit) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));

    if (!(VCALL(D_0044E568, 0x78, s32 (*)(VObject *, s32, s32))(D_0044E568, p->c.a.room, exit) & 0xFF)) {
        VObject *rm = D_0044E568;
        void *nm = D_0044E570;

        VCALL(nm, 0xC, void (*)(void *, s32, f32 *))(nm, VCALL(rm, 0x24, s32 (*)(VObject *, s32))(rm, exit), a);
        VCALL(nm, 0xC, void (*)(void *, s32, f32 *))(nm, VCALL(rm, 0x28, s32 (*)(VObject *, s32))(rm, exit), b);
    } else {
        VObject *rm = D_0044E568;

        VCALL(rm, 0x30, u32 (*)(VObject *, s32, f32 *))(rm, exit, a);
        VCALL(rm, 0x34, void (*)(VObject *, s32, f32 *))(rm, exit, b);
    }
    sceVu0SubVector(d, a, b);
    return func_002E2BC0(d);
}

/* turn to the root motion's direction (rotated to the walk mesh slope through triangle +0x34) */
void func_00213B60(Pursuer *p, u32 mask) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    u8 *m;
    u32 tri;
    f32 h;

    if (mask == (u32)-1) {
        mask = p->c.a.navMask;
    }
    m = p->c.motion;
    VCALL(m, 0x60, void (*)(void *, f32 *))(m, a);
    tri = func_00124480(&p->c.a, a, 0);
    h = func_001244D0(&p->c.a, a);
    if (tri != (u32)-1) {
        f32 r;

        VCALL(D_0044E570, 0x40, void (*)(void *, u32, f32 *, f32 *, f32 *, u32))(D_0044E570, p->c.a.navTri, b, p->c.a.pos, a, mask);
        r = func_002E2D00(p->c.a.angle[1] + func_002E2D00(func_001244D0(&p->c.a, b) - h));
        p->c.a.angle[1] = r;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, r);
    }
}

/* which way to turn to face point `pos` (see func_00213EC0) */
u32 func_00213FA0(Pursuer *p, const f32 *pos, f32 a, f32 b) {
    f32 heading = func_001244D0(&p->c.a, pos);
    f32 d;
    u32 r;

    if (a <= 0.0f) {
        a = -a;
    }
    if (b <= 0.0f) {
        b = -b;
    }
    d = func_002E2D00(heading - p->c.a.angle[1]);
    if (d <= a) {
        if (!(d < -a)) {
            return 0xFF;
        }
        r = 0;
    } else {
        r = 1;
    }
    if (!(b < a)) {
        if (d <= 0.0f) {
            d = -d;
        }
        if (!(d <= b)) {
            r = (r | 2) & 0xFF;
        }
    }
    return r & 0xFF;
}

/* can an eye at `from` facing `heading` see `to`: within `range` and `half` an angle either side */
s32 func_002181D0(Pursuer *p, const f32 *from, const f32 *to, f32 heading, f32 range, f32 half) {
    f32 d[4] __attribute__((aligned(16)));
    f32 dist, dx, dz, a;

    sceVu0SubVector(d, from, to);
    d[3] = 0.0f;
    dist = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    dx = to[0] - from[0];
    dz = to[2] - from[2];
    if (dx == 0.0f && dz == 0.0f) {
        return 0;
    }
    a = func_0031C5C0(dx, dz);
    if (!(dist <= range)) {
        return 0;
    }
    a = a - heading;
    if (!((func_002E2D00(a) <= 0.0f ? -func_002E2D00(a) : func_002E2D00(a)) <= half)) {
        return 0;
    }
    return 1;
}

/* the same between two actors */
s32 func_00218300(Pursuer *p, Actor *from, Actor *to, f32 heading, f32 range, f32 half) {
    f32 d[4] __attribute__((aligned(16)));
    f32 dist, dx, dz, a;

    sceVu0SubVector(d, from->pos, to->pos);
    d[3] = 0.0f;
    dist = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    dx = to->pos[0] - from->pos[0];
    dz = to->pos[2] - from->pos[2];
    if (dx == 0.0f && dz == 0.0f) {
        return 0;
    }
    a = func_0031C5C0(dx, dz);
    if (!(dist <= range)) {
        return 0;
    }
    a = a - heading;
    if (!((func_002E2D00(a) <= 0.0f ? -func_002E2D00(a) : func_002E2D00(a)) <= half)) {
        return 0;
    }
    return 1;
}

/* is Fiona within reach to be caught (seen, or 20 units on a walkable line; 10 with progress
 * flag 0xA; never with flag 9 or while her +0x1AD630 is set)? */
s32 func_00218A30(Pursuer *p) {
    Character *f = gCharPlayer;
    Progress *pr;
    f32 reach, d;
    s32 near;

    if (AT(f, 0x1AD630, u8) != 0) {
        return 0;
    }
    if (func_00218430(p, f) != 0) {
        return 1;
    }
    pr = gProgress;
    if (Progress_TestFlag(pr, 9) != 0) {
        return 0;
    }
    reach = Progress_TestFlag(pr, 0xA) != 0 ? 10.0f : 20.0f;
    d = PU(p, 0x1588, f32);
    near = 0;
    if (d < reach && !(d <= 0.0f)) {
        near = 1;
    }
    if ((p->c.a.unk2B == 0) & 0xFF & (near & 0xFF)) {
        u32 tri = f->a.navTri;

        if (func_00124480(&p->c.a, f->a.pos, 0x40080) == tri) {
            return 1;
        }
    }
    return 0;
}

/* ---- batch 5 ---- */

extern void *D_0046C220[], *D_00469C60[], *D_00469C20[];

/* vtable +0x8: NPC destructor (-> Character) */
Pursuer *func_001710D0(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_0046C220;
        VCALL(p, 0x10, void (*)(Pursuer *))(p);
        if (p != NULL) {
            p->c.a.vtbl = D_00469C60;
            if (p != NULL) {
                p->c.a.vtbl = D_00469C20;
            }
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* vtable +0xC: NPC init */
void func_00219460(Pursuer *p) {
    u32 i;

    p->c.a.unkC4 = 0;
    func_00127660(&p->c);
    p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    for (i = 0; i < 13; i++) {
        p->c.unk148C[i] = 0;
    }
    p->c.door = 0xFF;
    p->c.unk14C0 = 0xFFFF;
    p->c.unk14C4 = 0;
    p->c.pathReq->unk4 = 6;
    p->c.pathReq->mask = p->c.a.navMask;
    PU(p, 0x15A4, s32) = -1;
    PU(p, 0x15C4, s32) = -1;
    PU(p, 0x1594, s32) = -1;
    PU(p, 0x1598, s32) = -1;
    PU(p, 0x159C, s32) = -1;
    PU(p, 0x15C0, u8) = 0xFF;
    sceVu0CopyVector((f32 *)((u8 *)p + 0x1570), p->c.a.angle);
    PU(p, 0x1568, s32) = 0;
    PU(p, 0x1544, u8) = 0;
    PU(p, 0x1545, u8) = 0;
    PU(p, 0x1546, u8) = 0;
}

/* vtable +0xE8: head for a random other room (10 tries to avoid the played one) */
s32 func_00212400(Pursuer *p) {
    Progress *pr = gProgress;
    u32 tries = 0;

    do {
        PU(p, 0x1594, s32) = VCALL(pr, 0x38, s32 (*)(Progress *, s32))(pr, -1);
        PU(p, 0x1598, s32) = -1;
        if (PU(p, 0x1594, s32) == VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
            tries = (tries + 1) & 0xFF;
            if (tries < 10) {
                PU(p, 0x1594, s32) = p->c.a.room;
            }
        }
    } while (PU(p, 0x1594, s32) == p->c.a.room);
    if (PU(p, 0x1594, s32) != -1 && func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1) {
        return 1;
    }
    do {
        PU(p, 0x1594, s32) = VCALL(pr, 0x38, s32 (*)(Progress *, s32))(pr, p->c.a.room);
    } while (PU(p, 0x1594, s32) == p->c.a.room);
    if (PU(p, 0x1594, s32) != -1) {
        return func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1;
    }
    return 0;
}

/* plan a path to `pos` / triangle `tri` from the point beside door `door` (side 1 first, then 0);
 * the side that works, 0xFF none */
u32 func_00212E00(Pursuer *p, u32 tri, const f32 *pos, u32 door) {
    VObject *pl;
    u32 side = 1;

    p->c.pathReq->unk0 = 0;
    p->c.pathReq->goalTri = tri;
    sceVu0CopyVector(p->c.pathReq->goalPos, pos);
    pl = gSceneGameF29740;
    for (;;) {
        f32 ofs[4] __attribute__((aligned(16)));
        f32 at[4] __attribute__((aligned(16)));
        u32 t;

        VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, side & 0xFF, ofs);
        t = func_00123710(p, door & 0xFF, side & 0xFF, ofs, at);
        if (t != (u32)-1) {
            p->c.pathReq->startTri = t;
            sceVu0CopyVector(p->c.pathReq->startPos, at);
            p->c.pathId = VCALL(pl, 0xC, s32 (*)(VObject *, PathRequest *, s32))(pl, p->c.pathReq, 0);
            if (p->c.pathId != -1) {
                if (VCALL(pl, 0x14, s32 (*)(VObject *))(pl) >= 0) {
                    func_00127060(&p->c);
                    return side;
                }
                func_00127060(&p->c);
            }
        }
        side = (side == 0) & 0xFF;
        if (side != 0) {
            return 0xFF;
        }
    }
}

/* vtable +0xAC: go to triangle `tri` / point `pos` of room `room` (-1 the played one) */
void func_00219310(Pursuer *p, u32 tri, const f32 *pos, s32 room) {
    void *nm;

    if (room == -1) {
        room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    }
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        PU(p, 0x15A4, u32) = tri;
    }
    nm = D_0044E570;
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, const f32 *))(nm, tri, pos) == 3) {
        sceVu0CopyVector((f32 *)((u8 *)p + 0x15B0), pos);
    } else {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, tri, (f32 *)((u8 *)p + 0x15B0));
    }
    if (func_00126F80(&p->c, room, -1, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = -1;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* a random walkable triangle of the played room (not blocked, not flagged 0x100000 without
 * 0x200000... ); -1 if the pursuer is elsewhere */
u32 func_00214940(Pursuer *p) {
    if (p->c.a.room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        VObject *rnd = D_0044E550;
        s32 last = AT(D_0044E570, 0x8, s32) - 1;
        s32 n = last + 1;

        for (;;) {
            u32 tri;
            u32 flags;

            tri = last > 0 ? (s32)((f32)n * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) : 0;
            if (tri < AT(D_0044E570, 0x8, u32) && AT(D_0044E570, 0x4, u8 *) != NULL) {
                flags = AT(AT(D_0044E570, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
            } else {
                flags = 0;
            }
            if (flags & p->c.a.navMask) {
                continue;
            }
            if (!(flags & 0x100000) || !(flags & 0x200000)) {
                return tri;
            }
        }
    }
    return -1;
}

/* can the pursuer walk straight to `pos` (over triangles without flag 0x4000)? */
s32 func_00217110(Pursuer *p, const f32 *pos) {
    u32 tri = func_00124480(&p->c.a, pos, 0);

    if (tri == (u32)-1) {
        void *nm = D_0044E570;
        u32 t = p->c.a.navTri;

        for (;;) {
            u8 *e = t < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL ? AT(nm, 0x4, u8 *) + t * 0x50 : NULL;
            s32 r;

            if (AT(e, 0x3C, u32) & 0x4000) {
                return 0;
            }
            r = VCALL(nm, 0x20, s32 (*)(void *, u32, const f32 *, const f32 *))(nm, t, p->c.a.pos, pos);
            if (r == 3 || r == 4) {
                return 0;
            }
            t = AT(e + r * 4, 0x30, u32);
            if (t == (u32)-1) {
                return 1;
            }
        }
    }
    return (func_00122C90(&p->c.a, p->c.a.navTri, tri, p->c.a.pos, pos, 0) & 0xFF) == 0;
}

/* vtable +0x... : who's around (+0x1544 Fiona, +0x1545 Hewie, +0x1546 noise) */
void func_00217680(Pursuer *p) {
    Progress *pr = gProgress;
    s32 room = p->c.a.room;

    if (room != VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
    } else {
        PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
        if (p->c.moveMode != 2 && (func_00212850(p) & 0xFF) != 0xFF) {
            PU(p, 0x1544, u8) = 1;
        } else if (p->c.a.room != gCharPlayer->a.room) {
            PU(p, 0x1544, u8) = 0;
        } else {
            PU(p, 0x1544, u8) = VCALL(p, 0xC0, s32 (*)(Pursuer *))(p);
        }
        if (VCALL(pr, 0xC, s32 (*)(Progress *))(pr) != gCharPartner->a.room) {
            PU(p, 0x1545, u8) = 0;
        } else {
            PU(p, 0x1545, u8) = VCALL(p, 0xC4, s32 (*)(Pursuer *))(p);
        }
    }
    PU(p, 0x1546, u8) = VCALL(p, 0xC8, s32 (*)(Pursuer *))(p);
    VCALL(p, 0xCC, void (*)(Pursuer *))(p);
}

/* the same, unless the progress byte +0x1FBEC1 is set (then func_00217680) */
void func_002177D0(Pursuer *p) {
    Progress *pr = gProgress;

    if (AT(pr, 0x1FBEC1, u8) == 0) {
        s32 room = p->c.a.room;

        if (room != VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
            PU(p, 0x1544, u8) = 0;
            PU(p, 0x1545, u8) = 0;
        } else {
            PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
            if (p->c.moveMode != 2 && (func_00212850(p) & 0xFF) != 0xFF) {
                PU(p, 0x1544, u8) = 1;
            } else {
                PU(p, 0x1544, u8) = VCALL(p, 0xC0, s32 (*)(Pursuer *))(p);
            }
            if (VCALL(pr, 0xC, s32 (*)(Progress *))(pr) != gCharPartner->a.room) {
                PU(p, 0x1545, u8) = 0;
            } else {
                PU(p, 0x1545, u8) = VCALL(p, 0xC4, s32 (*)(Pursuer *))(p);
            }
        }
        PU(p, 0x1546, u8) = VCALL(p, 0xC8, s32 (*)(Pursuer *))(p);
        VCALL(p, 0xCC, void (*)(Pursuer *))(p);
        return;
    }
    func_00217680(p);
}

/* is there room `dist` to the side of the target (90 degrees one way, then the other)? */
s32 func_00217FC0(Pursuer *p, f32 dist) {
    f32 a = 0x1.921fb60000000p+0f /* 1.5707964 */;

    for (;;) {
        f32 v[4] __attribute__((aligned(16)));
        f32 w[4] __attribute__((aligned(16)));
        u32 tri;
        s32 ok = 0;

        func_002E2C10(v, func_002E2D00(a + func_001244D0(&p->c.a, p->target->a.pos)));
        sceVu0ScaleVector(v, v, dist);
        sceVu0AddVector(w, p->c.a.pos, v);
        tri = func_00124480(&p->c.a, w, p->c.a.navMask);
        if (tri != (u32)-1) {
            u32 flags;

            if (tri < AT(D_0044E570, 0x8, u32) && AT(D_0044E570, 0x4, u8 *) != NULL) {
                flags = AT(AT(D_0044E570, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
            } else {
                flags = 0;
            }
            if (!(p->c.a.navMask & flags)) {
                ok = 1;
            }
        }
        if ((ok & 0xFF) == 1) {
            return 1;
        }
        a = -a;
        if (!(a < 0.0f)) {
            return 0;
        }
    }
}

/* ---- batch 6 ---- */


/* path length to the nearest walkable point of triangle +0x15C4 / point +0x15D0 (+0x1590) */
s32 func_00216B20(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u32 tri = func_00216E00(p, PU(p, 0x15C4, u32), (f32 *)((u8 *)p + 0x15D0), v);
    f32 len;

    if (tri == (u32)-1) {
        PU(p, 0x1590, f32) = -1.0f;
        return 0;
    }
    sceVu0CopyVector(w, v);
    if (func_00127140(&p->c, 0, tri, w) <= 0) {
        len = -1.0f;
    } else if (func_001270F0(&p->c) > 0) {
        len = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        func_00127060(&p->c);
    } else {
        func_00127060(&p->c);
        len = -1.0f;
    }
    PU(p, 0x1590, f32) = len;
    if (len < 0.0f) {
        return 0;
    }
    sceVu0SubVector(d, v, (f32 *)((u8 *)p + 0x15D0));
    d[3] = 0.0f;
    PU(p, 0x1590, f32) += __builtin_sqrtf(sceVu0InnerProduct(d, d));
    return 1;
}

/* the same for the goal triangle +0x15A4 / point +0x15B0 */
s32 func_00216C90(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u32 tri = func_00216E00(p, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0), v);
    f32 len;

    if (tri == (u32)-1) {
        PU(p, 0x1590, f32) = -1.0f;
        return 0;
    }
    sceVu0CopyVector(w, v);
    if (func_00127140(&p->c, 0, tri, w) <= 0) {
        len = -1.0f;
    } else if (func_001270F0(&p->c) > 0) {
        len = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        func_00127060(&p->c);
    } else {
        func_00127060(&p->c);
        len = -1.0f;
    }
    PU(p, 0x1590, f32) = len;
    if (len < 0.0f) {
        return 0;
    }
    sceVu0SubVector(d, v, (f32 *)((u8 *)p + 0x15B0));
    d[3] = 0.0f;
    PU(p, 0x1590, f32) += __builtin_sqrtf(sceVu0InnerProduct(d, d));
    return 1;
}

/* can the pursuer see point `pos` (on triangle `tri`): in its view (+0x1580 range, +0x1584
 * angle, heading +0x1574) and nothing in the way? */
s32 func_002187D0(Pursuer *p, u32 tri, const f32 *pos) {
    f32 half = PU(p, 0x1584, f32);
    f32 range = PU(p, 0x1580, f32);
    f32 heading = PU(p, 0x1574, f32);
    f32 d[4] __attribute__((aligned(16)));
    f32 dist, dx, dz, a;
    s32 in = 0;

    sceVu0SubVector(d, p->c.a.pos, pos);
    d[3] = 0.0f;
    dist = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    dx = pos[0] - p->c.a.pos[0];
    dz = pos[2] - p->c.a.pos[2];
    if (!(dx == 0.0f && dz == 0.0f)) {
        a = func_0031C5C0(dx, dz);
        if (dist <= range) {
            a = a - heading;
            if ((func_002E2D00(a) <= 0.0f ? -func_002E2D00(a) : func_002E2D00(a)) <= half) {
                in = 1;
            }
        }
    }
    if (!(in & 0xFF)) {
        return 0;
    }
    return func_00122C90(&p->c.a, p->c.a.navTri, tri, p->c.a.pos, pos, 0);
}

/* what is exit `exit` like for the pursuer: 2 its own way in, 3 closed to it, 4 open, 5 / 6
 * it must open it (6: from the other side) */
u32 func_00211E00(Pursuer *p, s32 exit) {
    Progress *pr = gProgress;
    VObject *rm;

    if ((Progress_CurRoomFlag(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 2;
    }
    if (!(func_00178300(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot) & 0xFF)) {
        return 3;
    }
    if ((func_001785B0(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 3;
    }
    rm = D_0044E568;
    if (!(VCALL(rm, 0x78, s32 (*)(VObject *, s32, s32))(rm, p->c.a.room, exit) & 0xFF)) {
        return 4;
    }
    if ((func_00178980(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 4;
    }
    if ((func_00178840(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return (VCALL(rm, 0x4C, s32 (*)(VObject *, s32, s32))(rm, p->c.a.room, exit) & 0xFF) == 1 ? 6 : 3;
    }
    return 5;
}

/* the triangle `dist` away in direction `heading`: -1 none, -2 blocked, -3 flag 1, -4 at a
 * door, -5 at a room point */
u32 func_00217D30(Pursuer *p, f32 heading, f32 dist) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    void *nm;
    u32 tri, flags, i;

    func_002E2C10(v, func_002E2D00(heading));
    sceVu0ScaleVector(v, v, dist);
    sceVu0AddVector(w, p->c.a.pos, v);
    tri = func_00124480(&p->c.a, w, 0);
    if (tri == (u32)-1) {
        return -1;
    }
    nm = D_0044E570;
    if (tri < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) {
        flags = AT(AT(nm, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = 0;
    }
    if (flags & p->c.a.navMask) {
        return -2;
    }
    if (flags & 1) {
        return -3;
    }
    for (i = 0; i < 8; i++) {
        if (((VCALL(D_0044E558, 0x2C, s32 (*)(VObject *, u32, f32 *))(D_0044E558, i & 0xFF, w) != 0) ^ 1) == 0) {
            return -4;
        }
    }
    for (i = 0; i < 5; i++) {
        if (VCALL(nm, 0x50, s32 (*)(void *, u32, f32 *))(nm, i, w) != 0) {
            return -5;
        }
    }
    return tri;
}

/* who of the characters can the pursuer reach / see by the progress tables (+0x30 / +0x2C) and is
 * in front of it (within 90 degrees): a bit per slot */
s32 func_002179F0(Pursuer *p, s32 a1, f32 f) {
    Progress *pr = gProgress;
    s32 bits = 0;
    u32 i;

    for (i = 0; i < 3; i = (i + 1) & 0xFF) {
        u32 s = i & 0xFF;
        Character **c = &gCharacters[s];

        if (*c != NULL && s != (u32)p->c.a.slot && (*c)->a.active != 0 &&
            (VCALL(pr, 0x30, s32 (*)(Progress *, u32, s32, u32, f32))(pr, p->c.a.slot & 0xFF, a1, i, f) & 0xFF) == 1) {
            f32 d = func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos)) <= 0.0f
                        ? -func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos))
                        : func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos));

            if (d < 0x1.921fb60000000p+0f /* 1.5707964 */) {
                bits = (bits | ((1 << s) & 0xFF)) & 0xFF;
            }
        }
    }
    return bits;
}

s32 func_00217B90(Pursuer *p, s32 a1, f32 f) {
    Progress *pr = gProgress;
    s32 bits = 0;
    u32 i;

    for (i = 0; i < 3; i = (i + 1) & 0xFF) {
        u32 s = i & 0xFF;
        Character **c = &gCharacters[s];

        if (*c != NULL && s != (u32)p->c.a.slot && (*c)->a.active != 0 &&
            (VCALL(pr, 0x2C, s32 (*)(Progress *, u32, s32, u32, f32))(pr, p->c.a.slot & 0xFF, a1, i, f) & 0xFF) == 1) {
            f32 d = func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos)) <= 0.0f
                        ? -func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos))
                        : func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos));

            if (d < 0x1.921fb60000000p+0f /* 1.5707964 */) {
                bits = (bits | ((1 << s) & 0xFF)) & 0xFF;
            }
        }
    }
    return bits;
}

/* plan a path from triangle `tri` / `pos` (-1: where the pursuer is) to door `door`, side 0 then
 * 2; the door's triangle (+0x104 side, +0x1568 heading, +0x110 point), -1 none */
s32 func_002134E0(Pursuer *p, u32 door, s32 tri, const f32 *pos) {
    f32 from[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    VObject *d, *pl;
    s8 side;

    if (tri != -1) {
        sceVu0CopyVector(from, pos);
    } else {
        tri = p->c.a.navTri;
        sceVu0CopyVector(from, p->c.a.pos);
    }
    d = D_0044E558;
    pl = gSceneGameF29740;
    side = 0;
    p->c.pathReq->unk0 = 0;
    for (;;) {
        s32 t = VCALL(d, 0x14, s32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(d, door, side, at, dir, 1);

        p->c.pathReq->startTri = tri;
        sceVu0CopyVector(p->c.pathReq->startPos, from);
        p->c.pathReq->goalTri = t;
        sceVu0CopyVector(p->c.pathReq->goalPos, at);
        p->c.pathId = VCALL(pl, 0xC, s32 (*)(VObject *, PathRequest *, s32))(pl, p->c.pathReq, 0);
        if (p->c.pathId != -1 && VCALL(pl, 0x14, s32 (*)(VObject *))(pl) > 0) {
            p->c.unk104[0] = side;
            PU(p, 0x1568, f32) = dir[1];
            sceVu0CopyVector(p->c.unk110, at);
            return t;
        }
        func_00127060(&p->c);
        side = side != 2 ? 2 : 0;
        if (side == 0) {
            return -1;
        }
    }
}

/* find a door (0..4) to go through, from side `side` (0 / 1, other values: either); door to
 * +0x100, side to +0x104 */
s32 func_00212FE0(Pursuer *p, s32 side) {
    s8 s = side;
    u32 either = (s != 1 && s != 0) ? 1 : 0;
    u32 i;

    if ((either & 0xFF) == 1) {
        s = 0;
    }
    for (i = 0; i < 5; i++) {
        u32 retry = either & 0xFF;
        s32 valid = (s32)i >= 0 && i < AT(D_0044E570, 0x14, u32);

        if ((valid & 0xFF) != 1) {
            continue;
        }
        for (;;) {
            f32 ofs[4] __attribute__((aligned(16)));
            u32 sd = s != 0;
            s32 ok;

            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, sd, ofs);
            PU(p, 0x15C4, s32) = func_00123710(p, i & 0xFF, sd & 0xFF, ofs, (f32 *)((u8 *)p + 0x15D0));
            ok = PU(p, 0x15C4, s32) != -1 && (VCALL(p, 0xDC, s32 (*)(Pursuer *))(p) & 0xFF) == 1;
            if ((ok & 0xFF) == 1) {
                p->c.unk100 = i;
                p->c.unk104[0] = s;
                return 1;
            }
            if ((retry & 0xFF) != 1) {
                break;
            }
            retry = 0;
            s ^= 1;
        }
    }
    return 0;
}
