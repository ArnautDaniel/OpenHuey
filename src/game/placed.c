/* The placed things (gPlacedThings, Progress +0x6FC340, vtable D_0046F5C0): up to 128 actors of
 * 0x140 bytes in a block pool (+0xA040) - the items lying about in the rooms, of kinds 0..10.
 * Each: +0x20 kind, +0x28 active, +0x30 room, +0x34 nav triangle, +0x10 position, +0xE4 age.
 * Kinds 0, 2, 3, 5, 7 and 8 are kept with the save (Progress +0xA14: 60 entries of { kind,
 * room, triangle, x, z, age }); at most 10 of a kind lie about (kind 10: 5), the oldest going
 * when another comes. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "memcard.h"
#include "pursuer.h"
#include "item.h"
#include "effects.h"
#include "items.h"
#include "placed.h"
#include "props.h"
#include "pursuer_ai.h"
#include "scene_game_members.h"
#include "snd_place.h"
#include "stalker_math.h"
#include "msl.h"

extern void *D_00469C20[];   /* Actor */
extern void *D_0046F5C0[], *D_00469A00[], *D_004699C0[], *D_004699E0[], *D_0046A950[];
extern void *D_0046F520[], *D_004727E0[], *D_00472840[], *D_004758A0[], *D_00475A80[], *D_00475960[],
    *D_00475900[], *D_004759C0[], *D_00475A20[], *D_00479E70[], *D_00479ED0[], *D_00479500[];

#define POOL(m) ((m) + 0xA040)
#define SAVED(p) ((u8 *)(p) + 0xA14)
#define NUM_SAVED 60

void *Thing_dtor(void *p);
void *Actor_dtor(void *p);

s32 PlacedThings_PoolCall(u8 *p, s32 a1, s32 a2, s32 a3);

extern void *D_00476B50[];
void *func_003156A0(u8 *o, s32 flags);

extern void *D_0046D810[], *D_0046C220[], *D_00469C60[];
extern void *D_00474FD0[];
/* writes {x, 0, z} */
#define B5_SET3(out, x, z) ((out)[0] = (x), (out)[1] = 0.0f, (out)[2] = (z))

void Kind27_DoorOffset(void *self, s32 id, f32 *out);
void Kind27_ActionOffsets(void *self, s32 id, f32 *out);
void Kind27_ExitDone(u8 *self);

void Kind27_FollowPathExit(void);
void Kind27_Arrived(void);
void Kind27_WalkToExit(void);
void Kind27_OnToNextExit(void);
s32 Kind27_PickDestination(void);
void ThingShared_Draw(void);
void Thing10_Draw(void);

void ThingShared_Frame(u8 *o);

/* destructor: own vtable -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character; the model freed for
 * slots 3..5 */
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt) {
    if (c != NULL) {
        c->a.vtbl = vt;
        c->a.vtbl = D_0046D810;
        VCALL(c, 0x10, void (*)(Character *))(c);
        if ((u32)c->a.slot >= 3 && (u32)c->a.slot < 6) {
            void **m = c->motion;

            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                c->motion = NULL;
            }
        }
        c->a.vtbl = D_0046C220;
        VCALL(c, 0x10, void (*)(Character *))(c);
        c->a.vtbl = D_00469C60;
        c->a.vtbl = D_00469C20;
        if ((s16)flags > 0) {
            Actor_Destroy(&c->a);
        }
    }
    return c;
}

Character *Kind27_dtor(Character *c, s32 flags);

void *func_00120D60(B0_Pool *p, u32 i) {
    if (i < p->count && p->used[i] != 0) {
        return p->base + i * p->elemSize;
    }
    return NULL;
}

/* Free an element by address. */
/* Allocate an element of the given size. */
/* Constructor: base vtable, then derived vtable. */
/* 0x00120F40 */
void *Thing_dtor(void *p) {
    if (p != NULL) {
        *(void **)p = D_00469A00;
        *(void **)p = D_00469C20;
    }
    return p;
}
/* kinds kept with the save */
static s32 kept_kind(u32 k) {
    switch (k) {
    case 0: case 2: case 3: case 5: case 7: case 8:
        return 1;
    }
    return 0;
}

/* the active thing in block i (NULL: none or out of range) */
static u8 *placed_active(u8 *m, s32 i) {
    u8 *t = func_00120D60((B0_Pool *)POOL(m), i);

    if (t != NULL && AT(t, 0x28, u8) == 1) {
        return t;
    }
    return NULL;
}

/* destructor (the pool +0xA040 with it) */
/* (possibly dead code: nothing in the game references it) */
void *func_002D0EE0(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = D_0046F5C0;
        AT(m, 0xA040, void **) = D_004699C0;
        AT(m, 0xA040, void **) = D_004699E0;
        AT(m, 0x0, void **) = D_0046A950;
        gPlacedThings = NULL;
        if ((s16)flags > 0) {
            func_00100490(m);
        }
    }
    return m;
}

/* +0x8 a new thing of `kind` (0..10) from the pool (NULL: none / full) */
/* 0x002D6B00 */
void *PlacedThings_New(u8 *m, u32 kind) {
    static void **const sClass[11] = {
        D_0046F520, D_004727E0, D_00472840, D_004758A0, D_00475A80, D_00475960,
        D_00475900, D_004759C0, D_00475A20, D_00479E70, D_00479ED0,
    };
    void *mem;
    u8 *t;

    if (kind >= 11) {
        return NULL;
    }
    mem = VCALL((VObject *)m, 0x28, void *(*)(VObject *, u32))((VObject *)m, 0x140);
    if (mem == NULL) {
        return NULL;
    }
    t = ActorPool_new(0x140, mem);
    if (t != NULL) {
        AT(t, 0x0, void **) = D_00469C20;
        AT(t, 0x20, u32) = kind;
        AT(t, 0x24, u32) = 0x01000000;
        AT(t, 0x0, void **) = sClass[kind];
    }
    return mem;
}

/* the destructor of a thing (D_00479500) */
/* 0x002D6F60 */
void *ThingShared_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* +0x14 another of `kind` came: when 10 (kind 10: 5) lie about, the oldest of them is aged out
 * (+0xE4 -1) */
/* 0x002D6FD0 */
void PlacedThings_AnotherCame(u8 *m, u32 kind) {
    s32 max = 10, n = 0, i;
    u8 *oldest = NULL;

    switch (kind) {
    case 10:
        max = 5;
        break;
    case 0: case 2: case 3: case 5: case 7: case 8:
        break;
    default:
        return;
    }
    for (i = 0; i < 0x80; i++) {
        u8 *t = placed_active(m, i);

        if (t == NULL || AT(t, 0x20, u32) != kind) {
            continue;
        }
        if (n == 0 || AT(oldest, 0xE4, u32) < AT(t, 0xE4, u32)) {
            oldest = t;
        }
        if (++n == max) {
            AT(oldest, 0xE4, s32) = -1;
            return;
        }
    }
}

/* a save entry emptied */
void func_002A7700(s32 *e) {
    e[0] = -1;
    e[1] = -1;
    e[2] = -1;
    e[3] = 0;
    e[4] = 0;
    e[5] = 0;
}

/* +0x1C back from the save */
/* 0x002D7120 */
void PlacedThings_Load(u8 *m) {
    s32 *e = (s32 *)SAVED(gProgress);
    s32 i;

    for (i = 0; i < NUM_SAVED && e[0] != -1; i++, e += 6) {
        u8 *t = VCALL((VObject *)m, 0x8, u8 *(*)(VObject *, s32))((VObject *)m, e[0]);

        if (t != NULL) {
            VCALL((VObject *)t, 0xC, void (*)(VObject *))((VObject *)t);
            AT(t, 0x28, u8) = 1;
            AT(t, 0x30, s32) = e[1];
            AT(t, 0x34, s32) = e[2];
            AT(t, 0x10, s32) = e[3];
            AT(t, 0x18, s32) = e[4];
            AT(t, 0xE4, s32) = e[5];
        }
    }
}

/* +0x18 into the save */
/* 0x002D71F0 */
void PlacedThings_Save(u8 *m) {
    s32 *e = (s32 *)SAVED(gProgress);
    s32 n = 0, i;

    for (i = 0; i < 0x80; i++) {
        u8 *t = placed_active(m, i);

        if (t != NULL && kept_kind(AT(t, 0x20, u32))) {
            e[0] = AT(t, 0x20, u32);
            n++;
            e[1] = AT(t, 0x30, s32);
            e[2] = AT(t, 0x34, s32);
            e[3] = AT(t, 0x10, s32);
            e[4] = AT(t, 0x18, s32);
            e[5] = AT(t, 0xE4, s32);
            e += 6;
        }
    }
    for (; n < NUM_SAVED; n++, e += 6) {
        func_002A7700(e);
    }
}

/* +0x10 the first thing of `kind` from block `from` on (NULL: none) */
/* 0x002D7320 */
void *PlacedThings_FirstOfKind(u8 *m, u32 kind, s32 from) {
    s32 i;

    if (from < 0 || from >= 0x80) {
        return NULL;
    }
    for (i = from; i < 0x80; i++) {
        u8 *t = VCALL((VObject *)m, 0xC, u8 *(*)(VObject *, s32))((VObject *)m, i);

        if (t != NULL && AT(t, 0x20, u32) == kind) {
            return t;
        }
    }
    return NULL;
}

/* +0xC the thing in block i, if active */
/* 0x002D73D0 */
void *PlacedThings_Get(u8 *m, s32 i) {
    u8 *t;

    if (i < 0 || i >= 0x80) {
        return NULL;
    }
    t = func_00120D60((B0_Pool *)POOL(m), i);
    if (t == NULL || AT(t, 0x28, u8) == 0) {
        return NULL;
    }
    return t;
}

/* tail call: member at +0xA040, virtual slot 0x10 */
/* 0x002D7430 */
s32 PlacedThings_PoolCall(u8 *p, s32 a1, s32 a2, s32 a3) {
    u8 *m = p + 0xA040;
    return VCALL(m, 0x10, s32 (*)(void *, s32, s32, s32))(m, a1, a2, a3);
}

/* +0x24 everything gone */
/* 0x002D7450 */
void PlacedThings_Clear(u8 *m) {
    s32 i;

    for (i = 0; i < 0x80; i++) {
        VObject *t = func_00120D60((B0_Pool *)POOL(m), i);

        if (t != NULL) {
            VCALL((VObject *)POOL(m), 0x14, void (*)(VObject *, void *))((VObject *)POOL(m), t);
            if (t != NULL) {
                VCALL(t, 0x8, void (*)(VObject *, s32))(t, 1);
            }
        }
    }
}

/* +0x20: put the kept kinds lying in the current room onto their nav mesh triangle (+0x34;
 * position +0x10, copied to +0x40) */
/* 0x002D69E0 */
void PlacedThings_PlaceKept(u8 *mgr) {
    s32 room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    VObject *nav = (VObject *)gNavMesh;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        u8 *t = placed_active(mgr, i);

        if (t != NULL && kept_kind(AT(t, 0x20, u32))) {
            if (AT(t, 0x30, s32) == room && AT(t, 0x34, s32) != -1) {
                VCALL(nav, 0x14, void (*)(VObject *, s32, f32 *))(nav, AT(t, 0x34, s32), (f32 *)(t + 0x10));
                sceVu0CopyVector((f32 *)(t + 0x40), (f32 *)(t + 0x10));
            }
        }
    }
}

/* ---- kind 0: the ball (vtable D_0046F520), which Fiona throws to train Hewie. +0xB0 its
 * velocity, +0x100 gravity, +0x104 how hard slopes push it, +0x110 its colour (RGBA, 1.0 =
 * full), +0x120 fading out, +0xE0 its motion (0 flying, 1 rolling, 2 dropping through a hole,
 * +0xE8 the frames of it), +0xE1 kicked this contact. It lasts 3 minutes (+0xE4); when it goes
 * and Fiona has no ball, she gets hers (item 0x90) back. ---- */

#include "effectmgr.h"

extern const PTMF sGameStateNull;
extern const PTMF D_00414790, D_004147A0, D_004147B0;   /* +0x50 (virtual), func_002D54D0, func_002D5460 */
extern void *D_0046FC30[], *D_00469D00[];
extern void Thing_Setup(u8 *a);   /* Actor +0xC */
extern void Thing_Frame(u8 *a);   /* Actor +0x30 */
s32 Thing_PutDown(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 r, f32 h);   /* the base's +0x44 */

#define BALL_VEL(b) ((f32 *)((b) + 0xB0))
#define BALL_POS(b) ((f32 *)((b) + 0x10))
#define TRI_HOLE 0x10000000

static f32 ball_sqrt(f32 x) {
    return __builtin_sqrtf(x);
}

static u32 ball_tri_flags(VObject *nm, u32 i) {
    u8 *n = (u8 *)nm;

    if (i < AT(n, 0x8, u32) && AT(n, 0x4, u8 *) != NULL) {
        return AT(AT(n, 0x4, u8 *) + i * 0x50, 0x3C, u32);
    }
    return 0;
}

/* a hole under it: drop through */
static void ball_drop(u8 *b) {
    AT(b, 0xE0, u8) = 2;
    AT(b, 0xE8, s32) = 0;
    AT(b, 0x2A, u8) = 1;
}

/* the destructor: the ball given back when Fiona has none */
/* 0x002D5F40 */
void *Ball_dtor(u8 *b, s32 flags) {
    if (b != NULL) {
        AT(b, 0x0, void **) = D_0046F520;
        if (gSubScreen != NULL) {
            u8 *items = (u8 *)gSubScreen + 8;

            if (!(Items_Count(items, 0x90) & 0xFF)) {
                Items_Give(items, 0x90, 1);
            }
        }
        AT(b, 0x0, void **) = D_00469A00;
        AT(b, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(b);
        }
    }
    return b;
}

/* +0xC set up: blocked by nav flags 0x20020008, in the current room, white, flying */
/* 0x002D5ED0 */
void Ball_Setup(u8 *b) {
    Thing_Setup(b);
    AT(b, 0xC4, s32) = 0;
    AT(b, 0xC0, u32) = 0x20020008;
    AT(b, 0xE0, u8) = 0;
    AT(b, 0x30, s32) = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    AT(b, 0x11C, f32) = 1.0f;
    AT(b, 0x118, f32) = 1.0f;
    AT(b, 0x114, f32) = 1.0f;
    AT(b, 0x110, f32) = 1.0f;
    AT(b, 0x120, u16) = 0;
}

/* +0x2C draw: a 32 x 32 sprite (cell (480, 64)) a unit above it */
/* 0x002D5130 */
void Ball_Draw(u8 *b) {
    QuadRec r __attribute__((aligned(16)));
    QuadDrawer q __attribute__((aligned(16)));

    r.rgba[0] = (s32)(128.0f * AT(b, 0x110, f32));
    r.rgba[1] = (s32)(128.0f * AT(b, 0x114, f32));
    r.rgba[2] = (s32)(64.0f * AT(b, 0x118, f32));
    r.rgba[3] = (s32)(128.0f * AT(b, 0x11C, f32));
    sceVu0CopyVector(r.pos, BALL_POS(b));
    q.count = 1;
    q.frames = 1;
    AT(&r.w, 0, u32) = 0x3F2CCCCD;
    AT(&r.h, 0, u32) = 0x3F2CCCCD;
    q.a = -1;
    q.vtbl = D_0046FC30;
    q.texId = 1;
    r.pos[1] = r.pos[1] + 1.0f;
    q.tex = (u64)-1;
    r.turn = 0.0f;
    q.rec = &r;
    q.cy = 0.5f;
    q.cellX = 0x1E0;
    q.layer = 0x19;
    q.cellW = 0x20;
    q.cellY = 0x40;
    q.cellH = 0x20;
    q.texW = 0x200;
    r.frame = 0;
    q.texH = 0x100;
    q.cx = 0.0f;
    q.texGroup = 0x10;
    q.flags = 0;
    q.palette = 2;
    func_002E56C0((u8 *)&q);
    q.vtbl = D_00469D00;
}

/* the ball bounced at `at` (off the surface with normal n): its velocity mirrored and slowed
 * to 0.7; `to` = what is left of the step from `at` on (0: nothing left) */
static inline __attribute__((always_inline)) s32 ball_bounce(u8 *b, f32 *to, f32 *at, f32 *n, f32 keep) {
    f32 *v = BALL_VEL(b);
    f32 d[4] __attribute__((aligned(16)));
    f32 speed, all, left, k;

    speed = v[1] * v[1] + v[0] * v[0] + v[2] * v[2];
    AT(b, 0xBC, f32) = 1.0f;
    speed = ball_sqrt(speed);
    sceVu0Normalize(v, v);
    AT(b, 0xBC, f32) = 1.0f;
    sceVu0ScaleVector(d, n, 2.0f * sceVu0InnerProduct(n, v));
    sceVu0SubVector(v, d, v);
    sceVu0ScaleVector(v, v, keep * -speed);
    AT(b, 0xBC, f32) = 1.0f;
    sceVu0SubVector(d, BALL_POS(b), to);
    all = ball_sqrt(d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
    sceVu0SubVector(d, BALL_POS(b), at);
    left = ball_sqrt(d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
    if (all <= 0.0f) {
        return 0;
    }
    k = 1.0f - left / all;
    if (k <= 0.0f) {
        return 0;
    }
    sceVu0ScaleVector(d, v, k);
    sceVu0AddVector(to, at, d);
    return 1;
}

s32 func_002D5290(u8 *b, f32 *to, f32 *at, f32 *n) {
    return ball_bounce(b, to, at, n, 0x1.666666p-1f /* 0.7 */);
}

/* the dropping state: falling through for 31 frames, then gone */
void func_002D5460(u8 *b) {
    if (AT(b, 0x28, u8) == 0) {
        return;
    }
    sceVu0AddVector(BALL_VEL(b), (f32 *)(b + 0x100), BALL_VEL(b));
    sceVu0AddVector(BALL_POS(b), BALL_VEL(b), BALL_POS(b));
    if (++AT(b, 0xE8, u32) >= 31) {
        AT(b, 0x28, u8) = 0;
    }
}

/* the rolling state: moved on the nav mesh, slowed by friction (0.03 x the slope's flatness)
 * and pushed down the slope */
void func_002D54D0(u8 *b) {
    VObject *nm;
    f32 *v = BALL_VEL(b);
    f32 fr[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 c, flat, vx, vy, vz, k;

    if (AT(b, 0x28, u8) == 0) {
        return;
    }
    Actor_Move((Actor *)b, v);
    nm = (VObject *)gNavMesh;
    if (ball_tri_flags(nm, AT(b, 0x34, u32)) & TRI_HOLE) {
        ball_drop(b);
        return;
    }
    sceVu0SubVector(v, BALL_POS(b), (f32 *)(b + 0x40));
    c = VCALL(nm, 0x30, f32 (*)(VObject *, u32, f32 *))(nm, AT(b, 0x34, u32), n);
    flat = ball_sqrt(1.0f - c * c);
    vy = v[1];
    vz = v[2];
    vx = v[0];
    sceVu0Normalize(fr, v);
    sceVu0ScaleVector(fr, fr, 0x1.eb851ep-6f /* 0.03 */ * flat);
    if (fr[1] * fr[1] + fr[0] * fr[0] + fr[2] * fr[2] <= vy * vy + vx * vx + vz * vz) {
        sceVu0SubVector(v, v, fr);
    } else {
        v[2] = 0.0f;
        v[1] = 0.0f;
        v[0] = 0.0f;
    }
    k = c * AT(b, 0x104, f32);
    if (k <= 0.0f) {
        k = -k;
    }
    sceVu0ScaleVector(n, n, k);
    sceVu0AddVector(v, n, v);
}

/* +0x50 the flying state: under gravity along the nav mesh (+0x44 the step, which stops at
 * walls / floors with their normal); it bounces off what it hits (a bounce sound once), comes
 * to roll on floors and gentle slopes, and drops through holes. Off the mesh, it tries once
 * more the other way (0.8 back), then falls to the floor (+0x40) */
static inline __attribute__((always_inline)) void ball_fly(u8 *b, s32 check, s32 (*wall)(u8 *, f32 *, f32 *, f32 *), s32 sound) {
    f32 *v = BALL_VEL(b);
    f32 to[4] __attribute__((aligned(16)));
    f32 out[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 fl[4] __attribute__((aligned(16)));
    VObject *nm;
    s32 bounced = 0, again = 0;
    u32 tri, prev, hit;

    if (check && AT(b, 0x28, u8) == 0) {
        return;
    }
    sceVu0AddVector(v, (f32 *)(b + 0x100), v);
    nm = (VObject *)gNavMesh;
    for (;;) {
        sceVu0AddVector(to, v, BALL_POS(b));
        tri = AT(b, 0x34, u32);
        for (;;) {
            prev = tri;
            tri = VCALL(nm, 0x44, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, f32 *, u32))(
                nm, tri & 0xFFFF, out, BALL_POS(b), to, n, AT(b, 0xC0, u32));
            if (tri == (u32)-1) {
                break;
            }
            hit = tri & 0xF0000000;
            if (hit == 0) {
                sceVu0CopyVector(fl, out);
                VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, fl);
                if (out[1] < fl[1]) {
                    AT(b, 0xE0, u8) = 1;
                    out[1] = fl[1];
                }
                goto landed;
            }
            if (hit == 0x80000000 && (ball_tri_flags(nm, tri & 0xFFFF) & TRI_HOLE)) {
                AT(b, 0x34, u32) = tri;
                sceVu0CopyVector(BALL_POS(b), out);
                ball_drop(b);
                return;
            }
            if (!wall(b, to, out, n)) {
                VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, AT(b, 0x34, u32), BALL_POS(b));
                AT(b, 0xE0, u8) = 1;
                return;
            }
            if (hit == 0x80000000) {
                f32 d = sceVu0InnerProduct(n, v);

                if (d <= 0.0f) {
                    d = -d;
                }
                if (d < 0.5f) {
                    tri &= 0xFFFF;
                    VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, out);
                    AT(b, 0xE0, u8) = 1;
                }
            }
            if (AT(b, 0xE0, u8) == 1) {
                goto landed;
            }
            bounced = sound;
        }
        if (again) {
            break;
        }
        again = 1;
        sceVu0ScaleVector(v, v, -0x1.99999a0000000p-1f /* 0.8 */);
    }
    fl[1] = to[1];
    tri = VCALL(nm, 0x40, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, u32))(
        nm, prev & 0xFFFF, out, BALL_POS(b), to, AT(b, 0xC0, u32));
    if (tri != (u32)-1) {
        AT(b, 0x34, u32) = tri;
        sceVu0CopyVector(BALL_POS(b), out);
        AT(b, 0x14, f32) = fl[1];
    } else {
        VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, AT(b, 0x34, u32), BALL_POS(b));
        AT(b, 0xE0, u8) = 1;
    }
    return;
landed:
    AT(b, 0x34, u32) = tri;
    sceVu0CopyVector(BALL_POS(b), out);
    if (bounced) {
        Actor_PlaySound((Actor *)b, 0x7C, 5, 0, 0, NULL);
    }
}

/* 0x002D56D0 */
void Ball_Flying(u8 *b) {
    ball_fly(b, 1, func_002D5290, 1);
}

/* +0x4C the motion state for this frame (none outside the current room), and the fade */
/* 0x002D5A50 */
void Ball_MotionState(u8 *b) {
    if (AT(b, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        AT(b, 0xA0, PTMF) = sGameStateNull;
        return;
    }
    AT(b, 0x38, u32) = AT(b, 0x34, u32);
    sceVu0CopyVector((f32 *)(b + 0x40), BALL_POS(b));
    switch (AT(b, 0xE0, u8)) {
    case 0:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_00414790);
        break;
    case 1:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_004147A0);
        break;
    default:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_004147B0);
        break;
    }
    if (AT(b, 0x120, u16) != 0) {
        AT(b, 0x11C, f32) = AT(b, 0x11C, f32) - 0x1.555556p-3f /* 1/6 */;
        if (AT(b, 0x11C, f32) <= 0.0f) {
            AT(b, 0x11C, f32) = 0.0f;
            AT(b, 0x28, u8) = 0;
        }
    }
}

static inline __attribute__((always_inline)) u32 thing_put(u8 *b, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    u32 r = Thing_PutDown(b, tri, pos, rot, rr, h) & 0xFF;

    if (r == 1 && gSubScreen != NULL) {
        VCALL(gSubScreen, 0x1C, void (*)(VObject *))(gSubScreen);
    }
    return r;
}

/* +0x44 the base's; when it returns 1, the items are told (+0x1C) */
/* 0x002D5C10 */
u32 Ball_PutDown(u8 *b, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    return thing_put(b, tri, pos, rot, rr, h);
}

static inline __attribute__((always_inline)) s32 thing_step_tri(u8 *b, u8 *a, f32 *to) {
    f32 from[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 out[4] __attribute__((aligned(16)));
    u32 r;

    if (a == NULL) {
        return -1;
    }
    sceVu0CopyVector(from, BALL_POS(a));
    from[1] = to[1];
    r = VCALL(gNavMesh, 0x44, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, f32 *, u32))(
        (VObject *)gNavMesh, AT(a, 0x34, u32), out, from, to, n, AT(b, 0xC0, u32));
    if (r & 0xF0000000) {
        return -1;
    }
    return r;
}

/* +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`, -1 when
 * the step is stopped */
/* 0x002D5C70 */
s32 Ball_LandTri(u8 *b, u8 *a, f32 *to) {
    return thing_step_tri(b, a, to);
}

/* when Fiona touches it (+0x74, within 5) it is kicked the way she faces (2 ahead, `up` up),
 * once per touch */
static inline __attribute__((always_inline)) void thing_kick_up(u8 *b, f32 up) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 f[4] __attribute__((aligned(16)));
    VObject *fiona;

    fiona = (VObject *)gCharPlayer;
    if (fiona == NULL || AT(fiona, 0x28, u8) != 1 || AT(fiona, 0x29, u8) != 0) {
        return;
    }
    if (VCALL(fiona, 0x74, s32 (*)(VObject *, f32 *))(fiona, f) == 0) {
        AT(b, 0xE1, u8) = 0;
        return;
    }
    if (AT(b, 0xE1, u8) != 0) {
        return;
    }
    sceVu0SubVector(f, f, BALL_POS(b));
    if (25.0f < f[1] * f[1] + f[0] * f[0] + f[2] * f[2]) {
        return;
    }
    sceVu0CopyVector(f, (f32 *)((u8 *)fiona + 0x50));
    f[3] = 1.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrix(m, m, f);
    f[2] = 2.0f;
    f[3] = 1.0f;
    f[0] = 0.0f;
    f[1] = 0.0f;
    sceVu0ApplyMatrix(f, m, f);
    f[1] = f[1] + up;
    sceVu0AddVector(BALL_VEL(b), BALL_VEL(b), f);
    AT(b, 0x14, f32) = AT(b, 0x14, f32) + 0x1.99999a0000000p-4f /* 0.1 */;
    AT(b, 0xE0, u8) = 0;
    AT(b, 0xE1, u8) = 1;
}

static inline __attribute__((always_inline)) void thing_kick(u8 *b) {
    thing_kick_up(b, 2.0f);
}

/* +0x30 each frame: gone after 3 minutes; in Fiona's room, when she touches it (+0x74, within
 * 5) it is kicked the way she faces (2 ahead, 2 up), once per touch */
/* 0x002D5D10 */
void Ball_Frame(u8 *b) {
    Thing_Frame(b);
    if (AT(b, 0x28, u8) == 0) {
        return;
    }
    if (AT(b, 0xE4, u32) >= 5400) {
        AT(b, 0x28, u8) = 0;
        return;
    }
    if (AT(b, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return;
    }
    thing_kick(b);
}

/* ---- the things' base class (D_00469A00, over the actor): +0x10 position, +0x34 nav tri,
 * +0x50 turn (angles), +0xB0 a point in front, +0xEC / +0xF0 its two sizes, +0xE4 its age
 * (frames; -1 kept) ---- */

#include "ptmf.h"

extern const PTMF D_003AF1B8;   /* a thing's resting state */

/* (possibly unused by the vtables) place it: nav tri, position, turn, front point */
void func_00120F90(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 *front) {
    AT(o, 0x34, u32) = tri;
    sceVu0CopyVector((f32 *)(o + 0x10), pos);
    sceVu0CopyVector((f32 *)(o + 0x50), rot);
    sceVu0CopyVector((f32 *)(o + 0xB0), front);
}

/* +0x48 where it lands on the mesh: none (-1) */
/* 0x00120FF0 */
s32 Thing_LandTri(void) {
    return -1;
}

static inline __attribute__((always_inline)) void thing_set_front(u8 *o, f32 y, f32 r) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));

    sceVu0UnitMatrix(m);
    sceVu0RotMatrix(m, m, (f32 *)(o + 0x50));
    v[3] = 1.0f;
    v[0] = 0.0f;
    v[2] = r;
    v[1] = y;
    sceVu0ApplyMatrix((f32 *)(o + 0xB0), m, v);
}

/* +0x44 put it down at pos (turned rot, sizes r / h) where the mesh takes it (+0x48): its spot
 * and front point (r ahead); 0 if the mesh doesn't take it */
/* 0x00121000 */
s32 Thing_PutDown(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 r, f32 h) {
    f32 at[4] __attribute__((aligned(16)));
    s32 t;

    sceVu0CopyVector(at, pos);
    t = VCALL(o, 0x48, s32 (*)(void *, u32, f32 *, f32 *, f32, f32))(o, tri, at, rot, r, h);
    if (t == -1) {
        return 0;
    }
    sceVu0CopyVector((f32 *)(o + 0x10), at);
    sceVu0CopyVector((f32 *)(o + 0x50), rot);
    AT(o, 0xEC, f32) = r;
    AT(o, 0xF0, f32) = h;
    thing_set_front(o, 0.0f, r);
    AT(o, 0x34, s32) = t;
    return 1;
}

/* +0x40 put it at pos (off the mesh): its front point r ahead and 3 up */
/* 0x00121100 */
void Thing_PutAt(u8 *o, f32 *pos, f32 *rot, f32 r, f32 h) {
    sceVu0CopyVector((f32 *)(o + 0x10), pos);
    sceVu0CopyVector((f32 *)(o + 0x50), rot);
    AT(o, 0xEC, f32) = r;
    AT(o, 0xF0, f32) = h;
    thing_set_front(o, 3.0f, r);
    AT(o, 0x34, s32) = -1;
}

/* +0x4C to rest (state D_003AF1B8) */
/* 0x001211B0 */
void Thing_MotionState(u8 *o) {
    ptmf_set(&AT(o, 0xA0, PTMF), &D_003AF1B8);
}

/* +0x30 a frame: older by one while the game runs (not in a cutscene +0x38, not paused by the
 * events +0x50, not progress flag 8; -1 stays), then +0x4C and its state */
/* 0x00121220 */
void Thing_Frame(u8 *o) {
    if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) == 0
        && (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) & 0xFF) == 0
        && (Progress_TestFlag(gProgress, 8) & 0xFF) == 0) {
        s32 age = AT(o, 0xE4, s32) + 1;

        if (age != 0) {
            AT(o, 0xE4, s32) = age;
        }
    }
    VCALL(o, 0x4C, void (*)(void *))(o);
    if (ptmf_test(&AT(o, 0xA0, PTMF))) {
        ptmf_scall(o, &AT(o, 0xA0, PTMF));
    }
}

/* +0x2C nothing */
/* 0x001212F0 */
void Thing_Draw(void) {
}

/* +0xC set up: the actor's, then sizes 0, its bounce (0, -0.2, 0, 1) at +0x100, not held */
/* 0x00121300 */
void Thing_Setup(u8 *o) {
    Actor_Reset((Actor *)o);
    AT(o, 0xF0, s32) = 0;
    AT(o, 0xEC, s32) = 0;
    AT(o, 0x100, s32) = 0;
    AT(o, 0x104, u32) = 0xBE4CCCCD;
    AT(o, 0x108, s32) = 0;
    AT(o, 0x10C, f32) = 1.0f;
    AT(o, 0xC0, s32) = 0;
    AT(o, 0xE0, u8) = 0;
    AT(o, 0xE4, s32) = 0;
    AT(o, 0xE1, u8) = 0;
}

/* Count 32-byte records up to a -1 terminator. */
/* Move the point at +0x50 and translate +0x40 by the same delta. */
/* Move the point at +0x40 and translate +0x50 by the same delta. */
/* Tail call of virtual function 0xC (arguments passed through). */
/* 0x00122B30 */
void *Actor_dtor(void *p) {
    if (p != NULL) {
        *(void **)p = D_00469C20;
    }
    return p;
}

/* ---- kind 1 (D_004727E0): like the ball (kind 0) it flies, falls and is kicked, but it can't
 * bounce - whatever it hits, or a pursuer walking into it, bursts it in a purple splash ---- */

extern void *D_004727E0[];
extern const PTMF D_00429C28, D_00429C38;   /* +0x50 (virtual), func_00314CE0 */

/* +0x8 destructor */
/* 0x00314990 */
void *Thing01_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004727E0;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* burst at `at`: a drop splash of colour (r, g, b) and `size`, and its sound */
static inline void drop_splash(u8 *o, f32 *at, s32 r, s32 g, s32 b, f32 size) {
    u8 *mgr = gEffects;
    struct {
        s32 rgb[3];
        f32 pos[3];
        f32 size;
    } sp;
    s32 slot = Effect_New(mgr, 0xF70, DropSplash_Init);

    sp.rgb[0] = r;
    sp.rgb[1] = g;
    sp.rgb[2] = b;
    sp.pos[0] = at[0];
    sp.pos[1] = at[1];
    sp.pos[2] = at[2];
    sp.size = size;
    EffectMgr_Start(mgr, slot, &sp);
    Actor_PlaySound((Actor *)o, 0x8D, 5, 0, 0, NULL);
}

/* burst at `at`: a purple drop splash (size 1) and its sound */
void func_00314A00(u8 *o, f32 *at) {
    drop_splash(o, at, 0x40, 0x20, 0x80, 1.0f);
}

/* a half-size sprite (cell (128, 64)) in its colour (+0x110..: scaled by kr / kg / kb; kb 0
   for no blue) */
static inline void half_sprite(u8 *o, f32 kr, f32 kg, f32 kb) {
    QuadRec r __attribute__((aligned(16)));
    QuadDrawer q __attribute__((aligned(16)));

    r.rgba[0] = (s32)(kr * AT(o, 0x110, f32));
    r.rgba[1] = (s32)(kg * AT(o, 0x114, f32));
    r.rgba[2] = kb == 0.0f ? 0 : (s32)(kb * AT(o, 0x118, f32));
    r.rgba[3] = (s32)(128.0f * AT(o, 0x11C, f32));
    sceVu0CopyVector(r.pos, BALL_POS(o));
    r.w = 0.5f;
    r.h = 0.5f;
    r.turn = 0.0f;
    r.frame = 0;
    q.vtbl = D_0046FC30;
    q.a = -1;
    q.tex = (u64)-1;
    q.rec = &r;
    q.corners = 0;
    q.cx = 0.0f;
    q.cy = 0.0f;
    q.layer = 0x19;
    q.count = 1;
    q.cellX = 0x80;
    q.cellY = 0x40;
    q.cellW = 0x20;
    q.cellH = 0x20;
    q.texW = 0x200;
    q.texH = 0x100;
    q.flags = 0;
    q.frames = 1;
    q.texId = 1;
    q.texGroup = 0x10;
    q.palette = -1;
    func_002E56C0((u8 *)&q);
    q.vtbl = D_00469D00;
}

/* +0x2C draw: a half-size sprite in its colour */
/* 0x00314B90 */
void Thing01_Draw(u8 *o) {
    half_sprite(o, 64.0f, 32.0f, 128.0f);
}

/* the dropping state (D_00429C38): falling through for 31 frames, then gone */
void func_00314CE0(u8 *o) {
    sceVu0AddVector(BALL_VEL(o), (f32 *)(o + 0x100), BALL_VEL(o));
    sceVu0AddVector(BALL_POS(o), BALL_VEL(o), BALL_POS(o));
    if (++AT(o, 0xE8, u32) >= 31) {
        AT(o, 0x28, u8) = 0;
    }
}

/* +0x50 the flying state: under gravity, slowed sideways to 0.95, one step along the nav mesh;
 * landing on a floor above its spot's height ends there (still flying), a hole lets it drop,
 * anything else bursts it */
/* 0x00314D40 */
void Thing01_Flying(u8 *o) {
    static const union { u32 u; f32 f; } kDrag = {0x3F733333};
    f32 *v = BALL_VEL(o);
    f32 to[4] __attribute__((aligned(16)));
    f32 out[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 fl[4] __attribute__((aligned(16)));
    VObject *nm;
    u32 tri, hit;

    sceVu0AddVector(v, (f32 *)(o + 0x100), v);
    v[0] = v[0] * kDrag.f;
    v[2] = v[2] * kDrag.f;
    sceVu0AddVector(to, v, BALL_POS(o));
    nm = (VObject *)gNavMesh;
    tri = VCALL(nm, 0x44, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, f32 *, u32))(
        nm, AT(o, 0x34, u16), out, BALL_POS(o), to, n, AT(o, 0xC0, u32));
    if (tri == (u32)-1) {
        AT(o, 0x28, u8) = 0;
        func_00314A00(o, out);
        return;
    }
    hit = tri & 0xF0000000;
    if (hit == 0) {
        sceVu0CopyVector(fl, out);
        VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, fl);
        if (out[1] < fl[1]) {
            AT(o, 0x28, u8) = 0;
            func_00314A00(o, fl);
            return;
        }
        AT(o, 0x34, u32) = tri;
        sceVu0CopyVector(BALL_POS(o), out);
        return;
    }
    if (hit == 0x80000000 && (ball_tri_flags(nm, tri & 0xFFFF) & TRI_HOLE)) {
        AT(o, 0x34, u32) = tri;
        sceVu0CopyVector(BALL_POS(o), out);
        ball_drop(o);
        return;
    }
    AT(o, 0x28, u8) = 0;
    func_00314A00(o, out);
}

/* +0x4C the motion state for this frame (none outside the current room), and the fade */
/* 0x00314F20 */
void Thing01_MotionState(u8 *o) {
    if (AT(o, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        AT(o, 0xA0, PTMF) = sGameStateNull;
        return;
    }
    AT(o, 0x38, u32) = AT(o, 0x34, u32);
    sceVu0CopyVector((f32 *)(o + 0x40), BALL_POS(o));
    switch (AT(o, 0xE0, u8)) {
    case 0:
        ptmf_set(&AT(o, 0xA0, PTMF), &D_00429C28);
        break;
    default:
        ptmf_set(&AT(o, 0xA0, PTMF), &D_00429C38);
        break;
    }
    if (AT(o, 0x120, u16) != 0) {
        AT(o, 0x11C, f32) = AT(o, 0x11C, f32) - 0x1.555556p-3f /* 1/6 */;
        if (AT(o, 0x11C, f32) <= 0.0f) {
            AT(o, 0x11C, f32) = 0.0f;
            AT(o, 0x28, u8) = 0;
        }
    }
}

/* +0x44 the base's; when it returns 1, the items are told (+0x1C) */
/* 0x003150A0 */
u32 Thing01_PutDown(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    return thing_put(o, tri, pos, rot, rr, h);
}

/* +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`, -1 when
 * the step is stopped */
/* 0x00315100 */
s32 Thing01_LandTri(u8 *o, u8 *a, f32 *to) {
    return thing_step_tri(o, a, to);
}

/* +0x30 each frame, while the game runs (not in a cutscene, not paused by the events, not
 * progress flags 8 / 0x20) and it is in the current room: a pursuer standing over it (within
 * his radius + 1, his height span +-1) bursts it (an alert to the progress); then Fiona's kick */
static inline __attribute__((always_inline)) void thing_stomped(u8 *o, s16 a, s16 b, void (*burst)(u8 *, f32 *)) {
    Progress *p;
    u8 *c;
    f32 d[4] __attribute__((aligned(16)));

    Thing_Frame(o);
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) != 0) {
        return;
    }
    if ((VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) & 0xFF) == 1) {
        return;
    }
    p = gProgress;
    if ((Progress_TestFlag(p, 8) & 0xFF) == 1) {
        return;
    }
    if (Progress_TestFlag(p, 0x20) != 0) {
        return;
    }
    if (AT(o, 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return;
    }
    c = (u8 *)gCharPursuer;
    if (c != NULL && AT(c, 0x28, u8) == 1 && AT(c, 0x29, u8) == 0) {
        f32 y = AT(c, 0x14, f32);
        f32 top = 1.0f + (y + AT(c, 0xCC, f32));

        if (!(AT(o, 0x14, f32) <= y - 1.0f) && AT(o, 0x14, f32) < top) {
            sceVu0SubVector(d, (f32 *)(c + 0x10), BALL_POS(o));
            if (__builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) < 1.0f + AT(c, 0xC8, f32)) {
                Progress_Noise(p, BALL_POS(o), 4, 7, a, b, 0.0f);
                burst(o, BALL_POS(o));
                AT(o, 0x28, u8) = 0;
            }
        }
    }
    thing_kick(o);
}

/* 0x003151A0 */
void Thing01_Frame(u8 *o) {
    thing_stomped(o, 5, 4, func_00314A00);
}

/* +0xC set up: falling (-0.1), blocked by nav flags 0x20020008, in the current room, white */
/* 0x003154B0 */
void Thing01_Setup(u8 *o) {
    Thing_Setup(o);
    AT(o, 0x100, s32) = 0;
    AT(o, 0x104, u32) = 0xBDCCCCCD;
    AT(o, 0x108, s32) = 0;
    AT(o, 0x10C, f32) = 1.0f;
    AT(o, 0xC4, s32) = 0;
    AT(o, 0xC0, u32) = 0x20020008;
    AT(o, 0xE0, u8) = 0;
    AT(o, 0x30, s32) = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    AT(o, 0x11C, f32) = 1.0f;
    AT(o, 0x118, f32) = 1.0f;
    AT(o, 0x114, f32) = 1.0f;
    AT(o, 0x110, f32) = 1.0f;
    AT(o, 0x120, u16) = 0;
}

/* ---- kind 2 (D_00472840, over the shared thing class D_00479500): set down at a random turn;
 * it goes off (sound 0x8E and a D_00474FB0 burst at it) when the shared checks say so ---- */

extern void *D_00472840[], *D_00479500[], *D_00476B50[];
extern void ThingShared_Setup(u8 *o);   /* D_00479500 +0xC */
extern void func_003546B0(u8 *o);
extern s32 func_00354910(u8 *o);
extern void func_00354C90(u8 *o);
extern s32 func_003544C0(u8 *o);
extern s32 func_00354D50(u8 *o);
extern void func_00354AF0(u8 *o);

/* +0x8 destructor */
/* 0x00315540 */
void *Thing02_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00472840;
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* its model (a D_00476B50 drawer, settings a / b / c) turned by +0x132 / 256 of a full turn
   - 180 */
static inline void turned_model(u8 *o, s32 a, s32 b, s32 c) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    struct {
        void **vtbl;
        s32 a;
        u8 rest[0x38];
    } d __attribute__((aligned(16)));
    f32 rot[4] __attribute__((aligned(16)));

    rot[0] = 0.0f;
    rot[1] = kPi.f * (360.0f * ((f32)(u32)AT(o, 0x132, u16) / 256.0f) - 180.0f) / 180.0f;
    rot[2] = 0.0f;
    d.vtbl = D_00476B50;
    d.a = -1;
    ModelDraw_Fill((u8 *)&d, BALL_POS(o), rot, a, b, c);
    d.vtbl = D_00469D00;
}

/* +0x2C draw */
/* 0x003155C0 */
void Thing02_Draw(u8 *o) {
    turned_model(o, 1, 0, 1);
}

/* destructor (vtable D_00476B50) */
void *func_003156A0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00476B50;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

static inline __attribute__((always_inline)) void kind2_burst(u8 *o, u32 size, void (*init)(void **)) {
    u8 *mgr;

    Actor_PlaySound((Actor *)o, 0x8E, 5, 0, 0, NULL);
    mgr = gEffects;
    EffectMgr_Start(mgr, Effect_New(mgr, size, init), o + 0x10);
}

/* +0x30 each frame, while the game runs: the shared checks (func_00354910: 1 / 2 set off -
 * the burst only in the current room; else func_003544C0 / func_00354D50 set it off, or it
 * waits, func_00354AF0) */
static inline __attribute__((always_inline)) void thing_watch(u8 *o, u32 size, void (*init)(void **)) {
    Progress *p;
    s32 r, a, b;

    ThingShared_Frame(o);
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) != 0) {
        return;
    }
    if ((VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) & 0xFF) == 1) {
        return;
    }
    p = gProgress;
    if ((Progress_TestFlag(p, 8) & 0xFF) == 1) {
        return;
    }
    func_003546B0(o);
    r = func_00354910(o);
    if (r == 1 || r == 2) {
        if (r == 2 && Progress_TestFlag(p, 0x20) == 0) {
            func_00354C90(o);
        }
        if (AT(o, 0x30, s32) == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
            kind2_burst(o, size, init);
        }
        AT(o, 0x28, u8) = 0;
        return;
    }
    if (Progress_TestFlag(p, 0x20) != 0) {
        return;
    }
    a = func_003544C0(o) & 0xFF;
    b = func_00354D50(o) & 0xFF;
    if (a == 0 && b == 0) {
        func_00354AF0(o);
        return;
    }
    Actor_PlaySound((Actor *)o, 0x8E, 5, 0, 0, NULL);
    if (b != 0) {
        func_00354C90(o);
    }
    {
        u8 *mgr = gEffects;

        EffectMgr_Start(mgr, Effect_New(mgr, size, init), o + 0x10);
    }
    AT(o, 0x28, u8) = 0;
}

/* 0x00315700 */
void Thing02_Frame(u8 *o) {
    thing_watch(o, 0xFD0, ShoveBurst_Init);
}

/* the shared class's set up, its settings (+0x122..+0x12E), a random turn */
static inline void thing_setup(u8 *o, s16 a, s16 b, s16 c, s16 d, s16 e, s16 f, s16 g) {
    ThingShared_Setup(o);
    AT(o, 0x122, s16) = a;
    AT(o, 0x124, s16) = b;
    AT(o, 0x126, s16) = c;
    AT(o, 0x128, s16) = d;
    AT(o, 0x12A, s16) = e;
    AT(o, 0x12C, s16) = f;
    AT(o, 0x12E, s16) = g;
    AT(o, 0x132, u16) = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 0xFF;
}

/* +0xC set up */
/* 0x00315AB0 */
void Thing02_Setup(u8 *o) {
    thing_setup(o, 100, 20, 15, 900, 10, 10, 2);
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern void *D_00479E70[];
extern void *D_00479ED0[];
extern void *D_00475900[];
extern void func_003345D0(u8 *o, f32 *at);
extern void *D_004759C0[];
extern void *D_00475A20[];
extern void func_00336290(u8 *o, f32 *at);

/* (as func_002D5290)  the ball bounced at `at` (off the surface with normal n): its velocity mirrored and slowed
 * to 0.7; `to` = what is left of the step from `at` on (0: nothing left) */
s32 func_00354FF0(u8 *b, f32 *to, f32 *at, f32 *n) {
    f32 *v = BALL_VEL(b);
    f32 d[4] __attribute__((aligned(16)));
    f32 speed, all, left, k;

    speed = v[1] * v[1] + v[0] * v[0] + v[2] * v[2];
    AT(b, 0xBC, f32) = 1.0f;
    speed = ball_sqrt(speed);
    sceVu0Normalize(v, v);
    AT(b, 0xBC, f32) = 1.0f;
    sceVu0ScaleVector(d, n, 2.0f * sceVu0InnerProduct(n, v));
    sceVu0SubVector(v, d, v);
    sceVu0ScaleVector(v, v, 0x1.666666p-1f /* 0.7 */ * -speed);
    AT(b, 0xBC, f32) = 1.0f;
    sceVu0SubVector(d, BALL_POS(b), to);
    all = ball_sqrt(d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
    sceVu0SubVector(d, BALL_POS(b), at);
    left = ball_sqrt(d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
    if (all <= 0.0f) {
        return 0;
    }
    k = 1.0f - left / all;
    if (k <= 0.0f) {
        return 0;
    }
    sceVu0ScaleVector(d, v, k);
    sceVu0AddVector(to, at, d);
    return 1;
}

/* (as func_00314CE0)  the dropping state (D_00429C38): falling through for 31 frames, then gone */
void func_003551C0(u8 *o) {
    sceVu0AddVector(BALL_VEL(o), (f32 *)(o + 0x100), BALL_VEL(o));
    sceVu0AddVector(BALL_POS(o), BALL_VEL(o), BALL_POS(o));
    if (++AT(o, 0xE8, u32) >= 31) {
        AT(o, 0x28, u8) = 0;
    }
}

/* (as ThingShared_dtor)  the destructor of a thing (D_00479E70) */
/* 0x003671B0 */
void *Thing09_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479E70;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as func_002D54D0)  the rolling state: moved on the nav mesh, slowed by friction (0.03 x the slope's flatness)
 * and pushed down the slope */
void func_003674C0(u8 *b) {
    VObject *nm;
    f32 *v = BALL_VEL(b);
    f32 fr[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 c, flat, vx, vy, vz, k;

    if (AT(b, 0x28, u8) == 0) {
        return;
    }
    Actor_Move((Actor *)b, v);
    nm = (VObject *)gNavMesh;
    if (ball_tri_flags(nm, AT(b, 0x34, u32)) & TRI_HOLE) {
        ball_drop(b);
        return;
    }
    sceVu0SubVector(v, BALL_POS(b), (f32 *)(b + 0x40));
    c = VCALL(nm, 0x30, f32 (*)(VObject *, u32, f32 *))(nm, AT(b, 0x34, u32), n);
    flat = ball_sqrt(1.0f - c * c);
    vy = v[1];
    vz = v[2];
    vx = v[0];
    sceVu0Normalize(fr, v);
    sceVu0ScaleVector(fr, fr, 0x1.eb851ep-6f /* 0.03 */ * flat);
    if (fr[1] * fr[1] + fr[0] * fr[0] + fr[2] * fr[2] <= vy * vy + vx * vx + vz * vz) {
        sceVu0SubVector(v, v, fr);
    } else {
        v[2] = 0.0f;
        v[1] = 0.0f;
        v[0] = 0.0f;
    }
    k = c * AT(b, 0x104, f32);
    if (k <= 0.0f) {
        k = -k;
    }
    sceVu0ScaleVector(n, n, k);
    sceVu0AddVector(v, n, v);
}

/* (as ThingShared_dtor)  the destructor of a thing (D_00479ED0) */
/* 0x00367E20 */
void *Thing10_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479ED0;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* 0x00367E90 */
void Thing10_Draw(void) {
}

/* (as func_002D54D0)  the rolling state: moved on the nav mesh, slowed by friction (0.03 x the slope's flatness)
 * and pushed down the slope */
static inline __attribute__((always_inline)) void ball_roll(u8 *b, s32 check) {
    VObject *nm;
    f32 *v = BALL_VEL(b);
    f32 fr[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 c, flat, vx, vy, vz, k;

    if (check && AT(b, 0x28, u8) == 0) {
        return;
    }
    Actor_Move((Actor *)b, v);
    nm = (VObject *)gNavMesh;
    if (ball_tri_flags(nm, AT(b, 0x34, u32)) & TRI_HOLE) {
        ball_drop(b);
        return;
    }
    sceVu0SubVector(v, BALL_POS(b), (f32 *)(b + 0x40));
    c = VCALL(nm, 0x30, f32 (*)(VObject *, u32, f32 *))(nm, AT(b, 0x34, u32), n);
    flat = ball_sqrt(1.0f - c * c);
    vy = v[1];
    vz = v[2];
    vx = v[0];
    sceVu0Normalize(fr, v);
    sceVu0ScaleVector(fr, fr, 0x1.eb851ep-6f /* 0.03 */ * flat);
    if (fr[1] * fr[1] + fr[0] * fr[0] + fr[2] * fr[2] <= vy * vy + vx * vx + vz * vz) {
        sceVu0SubVector(v, v, fr);
    } else {
        v[2] = 0.0f;
        v[1] = 0.0f;
        v[0] = 0.0f;
    }
    k = c * AT(b, 0x104, f32);
    if (k <= 0.0f) {
        k = -k;
    }
    sceVu0ScaleVector(n, n, k);
    sceVu0AddVector(v, n, v);
}

void func_003681D0(u8 *b) {
    ball_roll(b, 1);
}

/* (as Ball_Setup)  +0xC set up: blocked by nav flags 0x20020008, in the current room, white, flying */
/* 0x00368B40 */
void Thing10_Setup(u8 *b) {
    Thing_Setup(b);
    AT(b, 0xC4, s32) = 0;
    AT(b, 0xC0, u32) = 0x20020008;
    AT(b, 0xE0, u8) = 0;
    AT(b, 0x30, s32) = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    AT(b, 0x11C, f32) = 1.0f;
    AT(b, 0x118, f32) = 1.0f;
    AT(b, 0x114, f32) = 1.0f;
    AT(b, 0x110, f32) = 1.0f;
    AT(b, 0x120, u16) = 0;
}

/* (as ThingShared_dtor)  the destructor of a thing (D_004758A0) */
/* 0x00333240 */
void *Thing03_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004758A0;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as func_00314CE0)  the dropping state (D_00429C38): falling through for 31 frames, then gone */
void func_00333880(u8 *o) {
    sceVu0AddVector(BALL_VEL(o), (f32 *)(o + 0x100), BALL_VEL(o));
    sceVu0AddVector(BALL_POS(o), BALL_VEL(o), BALL_POS(o));
    if (++AT(o, 0xE8, u32) >= 31) {
        AT(o, 0x28, u8) = 0;
    }
}

/* (as Ball_PutDown)  +0x44 the base's; when it returns 1, the items are told (+0x1C) */
/* 0x003341D0 */
u32 Thing03_PutDown(u8 *b, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    return thing_put(b, tri, pos, rot, rr, h);
}

/* (as Ball_LandTri)  +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`, -1 when
 * the step is stopped */
/* 0x00334230 */
s32 Thing03_LandTri(u8 *b, u8 *a, f32 *to) {
    return thing_step_tri(b, a, to);
}

/* (as ThingShared_dtor)  the destructor of a thing (D_00475900) */
/* 0x00334560 */
void *Thing06_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00475900;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as func_00314CE0)  the dropping state (D_00429C38): falling through for 31 frames, then gone */
void func_003348A0(u8 *o) {
    sceVu0AddVector(BALL_VEL(o), (f32 *)(o + 0x100), BALL_VEL(o));
    sceVu0AddVector(BALL_POS(o), BALL_VEL(o), BALL_POS(o));
    if (++AT(o, 0xE8, u32) >= 31) {
        AT(o, 0x28, u8) = 0;
    }
}

/* (as Thing01_Flying)  +0x50 the flying state: under gravity, slowed sideways to 0.95, one step along the nav mesh;
 * landing on a floor above its spot's height ends there (still flying), a hole lets it drop,
 * anything else bursts it */
/* 0x00334900 */
void Thing06_Flying(u8 *o) {
    static const union { u32 u; f32 f; } kDrag = {0x3F733333};
    f32 *v = BALL_VEL(o);
    f32 to[4] __attribute__((aligned(16)));
    f32 out[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 fl[4] __attribute__((aligned(16)));
    VObject *nm;
    u32 tri, hit;

    sceVu0AddVector(v, (f32 *)(o + 0x100), v);
    v[0] = v[0] * kDrag.f;
    v[2] = v[2] * kDrag.f;
    sceVu0AddVector(to, v, BALL_POS(o));
    nm = (VObject *)gNavMesh;
    tri = VCALL(nm, 0x44, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, f32 *, u32))(
        nm, AT(o, 0x34, u16), out, BALL_POS(o), to, n, AT(o, 0xC0, u32));
    if (tri == (u32)-1) {
        AT(o, 0x28, u8) = 0;
        func_003345D0(o, out);
        return;
    }
    hit = tri & 0xF0000000;
    if (hit == 0) {
        sceVu0CopyVector(fl, out);
        VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, fl);
        if (out[1] < fl[1]) {
            AT(o, 0x28, u8) = 0;
            func_003345D0(o, fl);
            return;
        }
        AT(o, 0x34, u32) = tri;
        sceVu0CopyVector(BALL_POS(o), out);
        return;
    }
    if (hit == 0x80000000 && (ball_tri_flags(nm, tri & 0xFFFF) & TRI_HOLE)) {
        AT(o, 0x34, u32) = tri;
        sceVu0CopyVector(BALL_POS(o), out);
        ball_drop(o);
        return;
    }
    AT(o, 0x28, u8) = 0;
    func_003345D0(o, out);
}

/* (as Ball_PutDown)  +0x44 the base's; when it returns 1, the items are told (+0x1C) */
/* 0x00334C60 */
u32 Thing06_PutDown(u8 *b, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    return thing_put(b, tri, pos, rot, rr, h);
}

/* (as Ball_LandTri)  +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`, -1 when
 * the step is stopped */
/* 0x00334CC0 */
s32 Thing06_LandTri(u8 *b, u8 *a, f32 *to) {
    return thing_step_tri(b, a, to);
}

/* (as Thing01_Setup)  +0xC set up: falling (-0.1), blocked by nav flags 0x20020008, in the current room, white */
/* 0x00335070 */
void Thing06_Setup(u8 *o) {
    Thing_Setup(o);
    AT(o, 0x100, s32) = 0;
    AT(o, 0x104, u32) = 0xBDCCCCCD;
    AT(o, 0x108, s32) = 0;
    AT(o, 0x10C, f32) = 1.0f;
    AT(o, 0xC4, s32) = 0;
    AT(o, 0xC0, u32) = 0x20020008;
    AT(o, 0xE0, u8) = 0;
    AT(o, 0x30, s32) = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    AT(o, 0x11C, f32) = 1.0f;
    AT(o, 0x118, f32) = 1.0f;
    AT(o, 0x114, f32) = 1.0f;
    AT(o, 0x110, f32) = 1.0f;
    AT(o, 0x120, u16) = 0;
}

/* (as Thing02_dtor)  +0x8 destructor */
/* 0x00335100 */
void *Thing05_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00475960;
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as Thing02_dtor)  +0x8 destructor */
/* 0x003356C0 */
void *Thing07_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004759C0;
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as Thing02_dtor)  +0x8 destructor */
/* 0x00335C80 */
void *Thing08_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00475A20;
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as ThingShared_dtor)  the destructor of a thing (D_00475A80) */
/* 0x00336220 */
void *Thing04_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00475A80;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as func_00314CE0)  the dropping state (D_00429C38): falling through for 31 frames, then gone */
void func_00336550(u8 *o) {
    sceVu0AddVector(BALL_VEL(o), (f32 *)(o + 0x100), BALL_VEL(o));
    sceVu0AddVector(BALL_POS(o), BALL_VEL(o), BALL_POS(o));
    if (++AT(o, 0xE8, u32) >= 31) {
        AT(o, 0x28, u8) = 0;
    }
}

/* (as Thing01_Flying)  +0x50 the flying state: under gravity, slowed sideways to 0.95, one step along the nav mesh;
 * landing on a floor above its spot's height ends there (still flying), a hole lets it drop,
 * anything else bursts it */
/* 0x003365B0 */
void Thing04_Flying(u8 *o) {
    static const union { u32 u; f32 f; } kDrag = {0x3F733333};
    f32 *v = BALL_VEL(o);
    f32 to[4] __attribute__((aligned(16)));
    f32 out[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 fl[4] __attribute__((aligned(16)));
    VObject *nm;
    u32 tri, hit;

    sceVu0AddVector(v, (f32 *)(o + 0x100), v);
    v[0] = v[0] * kDrag.f;
    v[2] = v[2] * kDrag.f;
    sceVu0AddVector(to, v, BALL_POS(o));
    nm = (VObject *)gNavMesh;
    tri = VCALL(nm, 0x44, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, f32 *, u32))(
        nm, AT(o, 0x34, u16), out, BALL_POS(o), to, n, AT(o, 0xC0, u32));
    if (tri == (u32)-1) {
        AT(o, 0x28, u8) = 0;
        func_00336290(o, out);
        return;
    }
    hit = tri & 0xF0000000;
    if (hit == 0) {
        sceVu0CopyVector(fl, out);
        VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, fl);
        if (out[1] < fl[1]) {
            AT(o, 0x28, u8) = 0;
            func_00336290(o, fl);
            return;
        }
        AT(o, 0x34, u32) = tri;
        sceVu0CopyVector(BALL_POS(o), out);
        return;
    }
    if (hit == 0x80000000 && (ball_tri_flags(nm, tri & 0xFFFF) & TRI_HOLE)) {
        AT(o, 0x34, u32) = tri;
        sceVu0CopyVector(BALL_POS(o), out);
        ball_drop(o);
        return;
    }
    AT(o, 0x28, u8) = 0;
    func_00336290(o, out);
}

/* (as Ball_PutDown)  +0x44 the base's; when it returns 1, the items are told (+0x1C) */
/* 0x00336910 */
u32 Thing04_PutDown(u8 *b, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    return thing_put(b, tri, pos, rot, rr, h);
}

/* (as Ball_LandTri)  +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`, -1 when
 * the step is stopped */
/* 0x00336970 */
s32 Thing04_LandTri(u8 *b, u8 *a, f32 *to) {
    return thing_step_tri(b, a, to);
}

/* (as Thing01_Setup)  +0xC set up: falling (-0.1), blocked by nav flags 0x20020008, in the current room, white */
/* 0x00336D20 */
void Thing04_Setup(u8 *o) {
    Thing_Setup(o);
    AT(o, 0x100, s32) = 0;
    AT(o, 0x104, u32) = 0xBDCCCCCD;
    AT(o, 0x108, s32) = 0;
    AT(o, 0x10C, f32) = 1.0f;
    AT(o, 0xC4, s32) = 0;
    AT(o, 0xC0, u32) = 0x20020008;
    AT(o, 0xE0, u8) = 0;
    AT(o, 0x30, s32) = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    AT(o, 0x11C, f32) = 1.0f;
    AT(o, 0x118, f32) = 1.0f;
    AT(o, 0x114, f32) = 1.0f;
    AT(o, 0x110, f32) = 1.0f;
    AT(o, 0x120, u16) = 0;
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern const PTMF D_00443E30;
extern const PTMF D_00443E40;
extern const PTMF D_00443E50;
extern const PTMF D_00445B60;
extern const PTMF D_00445B70;
extern const PTMF D_00445B80;
extern const PTMF D_00445B90;
extern const PTMF D_00445BA0;
extern const PTMF D_00445BB0;
extern const PTMF D_0042F230;
extern const PTMF D_0042F240;
extern const PTMF D_0042F250;
extern const PTMF D_0042F260;

/* (as Ball_MotionState)  +0x4C the motion state for this frame (none outside the current room), and the fade */
/* 0x00355780 */
void ThingShared_MotionState(u8 *b) {
    if (AT(b, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        AT(b, 0xA0, PTMF) = sGameStateNull;
        return;
    }
    AT(b, 0x38, u32) = AT(b, 0x34, u32);
    sceVu0CopyVector((f32 *)(b + 0x40), BALL_POS(b));
    switch (AT(b, 0xE0, u8)) {
    case 0:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_00443E30);
        break;
    case 1:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_00443E40);
        break;
    default:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_00443E50);
        break;
    }
    if (AT(b, 0x120, u16) != 0) {
        AT(b, 0x11C, f32) = AT(b, 0x11C, f32) - 0x1.555556p-3f /* 1/6 */;
        if (AT(b, 0x11C, f32) <= 0.0f) {
            AT(b, 0x11C, f32) = 0.0f;
            AT(b, 0x28, u8) = 0;
        }
    }
}

/* +0xE8 counted, then Thing_Frame */
/* 0x00355940 */
void ThingShared_Frame(u8 *o) {
    AT(o, 0xE8, s32)++;
    Thing_Frame(o);
}

/* 0x00355950 */
void ThingShared_Draw(void) {
}

/* (as Ball_MotionState)  +0x4C the motion state for this frame (none outside the current room), and the fade */
/* 0x00367A00 */
void Thing09_MotionState(u8 *b) {
    if (AT(b, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        AT(b, 0xA0, PTMF) = sGameStateNull;
        return;
    }
    AT(b, 0x38, u32) = AT(b, 0x34, u32);
    sceVu0CopyVector((f32 *)(b + 0x40), BALL_POS(b));
    switch (AT(b, 0xE0, u8)) {
    case 0:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_00445B60);
        break;
    case 1:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_00445B70);
        break;
    default:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_00445B80);
        break;
    }
    if (AT(b, 0x120, u16) != 0) {
        AT(b, 0x11C, f32) = AT(b, 0x11C, f32) - 0x1.555556p-3f /* 1/6 */;
        if (AT(b, 0x11C, f32) <= 0.0f) {
            AT(b, 0x11C, f32) = 0.0f;
            AT(b, 0x28, u8) = 0;
        }
    }
}

/* (as Ball_MotionState)  +0x4C the motion state for this frame (none outside the current room), and the fade */
/* 0x00368710 */
void Thing10_MotionState(u8 *b) {
    if (AT(b, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        AT(b, 0xA0, PTMF) = sGameStateNull;
        return;
    }
    AT(b, 0x38, u32) = AT(b, 0x34, u32);
    sceVu0CopyVector((f32 *)(b + 0x40), BALL_POS(b));
    switch (AT(b, 0xE0, u8)) {
    case 0:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_00445B90);
        break;
    case 1:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_00445BA0);
        break;
    default:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_00445BB0);
        break;
    }
    if (AT(b, 0x120, u16) != 0) {
        AT(b, 0x11C, f32) = AT(b, 0x11C, f32) - 0x1.555556p-3f /* 1/6 */;
        if (AT(b, 0x11C, f32) <= 0.0f) {
            AT(b, 0x11C, f32) = 0.0f;
            AT(b, 0x28, u8) = 0;
        }
    }
}

/* (as Thing01_MotionState)  +0x4C the motion state for this frame (none outside the current room), and the fade */
/* 0x00334AE0 */
void Thing06_MotionState(u8 *o) {
    if (AT(o, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        AT(o, 0xA0, PTMF) = sGameStateNull;
        return;
    }
    AT(o, 0x38, u32) = AT(o, 0x34, u32);
    sceVu0CopyVector((f32 *)(o + 0x40), BALL_POS(o));
    switch (AT(o, 0xE0, u8)) {
    case 0:
        ptmf_set(&AT(o, 0xA0, PTMF), &D_0042F230);
        break;
    default:
        ptmf_set(&AT(o, 0xA0, PTMF), &D_0042F240);
        break;
    }
    if (AT(o, 0x120, u16) != 0) {
        AT(o, 0x11C, f32) = AT(o, 0x11C, f32) - 0x1.555556p-3f /* 1/6 */;
        if (AT(o, 0x11C, f32) <= 0.0f) {
            AT(o, 0x11C, f32) = 0.0f;
            AT(o, 0x28, u8) = 0;
        }
    }
}

/* (as Thing01_MotionState)  +0x4C the motion state for this frame (none outside the current room), and the fade */
/* 0x00336790 */
void Thing04_MotionState(u8 *o) {
    if (AT(o, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        AT(o, 0xA0, PTMF) = sGameStateNull;
        return;
    }
    AT(o, 0x38, u32) = AT(o, 0x34, u32);
    sceVu0CopyVector((f32 *)(o + 0x40), BALL_POS(o));
    switch (AT(o, 0xE0, u8)) {
    case 0:
        ptmf_set(&AT(o, 0xA0, PTMF), &D_0042F250);
        break;
    default:
        ptmf_set(&AT(o, 0xA0, PTMF), &D_0042F260);
        break;
    }
    if (AT(o, 0x120, u16) != 0) {
        AT(o, 0x11C, f32) = AT(o, 0x11C, f32) - 0x1.555556p-3f /* 1/6 */;
        if (AT(o, 0x11C, f32) <= 0.0f) {
            AT(o, 0x11C, f32) = 0.0f;
            AT(o, 0x28, u8) = 0;
        }
    }
}

/* (as func_002D5290) */
s32 func_003336B0(u8 *b, f32 *to, f32 *at, f32 *n) {
    return ball_bounce(b, to, at, n, 0x1.99999ap-3f /* 0.2 */);
}

/* (as func_002D5290) */
s32 func_00367F80(u8 *b, f32 *to, f32 *at, f32 *n) {
    return ball_bounce(b, to, at, n, 0x1.99999ap-3f /* 0.2 */);
}

/* (as func_002D5290) */
s32 func_003672A0(u8 *b, f32 *to, f32 *at, f32 *n) {
    return ball_bounce(b, to, at, n, 0x1.99999ap-2f /* 0.4 */);
}

/* ---- the shared thing class (D_00479500) and the other kinds over it (code 0x333000..0x337800):
   their settings, splashes, sprites and models through the helpers above ---- */

/* D_00479500 +0xC set up: the actor's, blocked by nav flags 0x20020008, in the current room,
   white */
/* 0x00355960 */
void ThingShared_Setup(u8 *o) {
    Thing_Setup(o);
    AT(o, 0xE8, s32) = 0;
    AT(o, 0xC4, s32) = 0;
    AT(o, 0xC0, u32) = 0x20020008;
    AT(o, 0xE0, u8) = 0;
    AT(o, 0x30, s32) = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    AT(o, 0x11C, f32) = 1.0f;
    AT(o, 0x118, f32) = 1.0f;
    AT(o, 0x114, f32) = 1.0f;
    AT(o, 0x110, f32) = 1.0f;
    AT(o, 0x120, u16) = 0;
}

/* 0x00335C00 */
void Thing07_Setup(u8 *o) {
    thing_setup(o, 100, 100, 45, 3600, 20, 10, 2);
}

/* 0x00335640 */
void Thing05_Setup(u8 *o) {
    thing_setup(o, 100, 50, 30, 1800, 15, 10, 2);
}

/* 0x003361B0 */
void Thing08_Setup(u8 *o) {
    thing_setup(o, 0, 0, 0, 450, 10, 10, 0);
}

/* bursts: an orange splash (size 1), a grey one (size 2) */
void func_00336290(u8 *o, f32 *at) {
    drop_splash(o, at, 0x80, 0x64, 0, 1.0f);
}

void func_003345D0(u8 *o, f32 *at) {
    drop_splash(o, at, 0x80, 0x80, 0x80, 2.0f);
}

/* +0x2C draws: sprites in its colour (white; no blue) */
/* 0x00334760 */
void Thing06_Draw(u8 *o) {
    half_sprite(o, 128.0f, 128.0f, 128.0f);
}

/* 0x00336420 */
void Thing04_Draw(u8 *o) {
    half_sprite(o, 128.0f, 100.0f, 0.0f);
}

/* +0x2C draws: its turned model */
/* 0x00335180 */
void Thing05_Draw(u8 *o) {
    turned_model(o, 1, 1, 1);
}

/* 0x00335740 */
void Thing07_Draw(u8 *o) {
    turned_model(o, 1, 2, 1);
}

/* 0x00335D00 */
void Thing08_Draw(u8 *o) {
    turned_model(o, 3, 0, 1);
}

/* ---- more of the same in the other kinds: the flying / rolling states without the active
   check or the bounce sound, the stomped and watched checks with their own bursts ---- */

/* 0x00355410 */
void ThingShared_Flying(u8 *b) {
    ball_fly(b, 0, func_00354FF0, 1);
}

/* 0x003676C0 */
void Thing09_Flying(u8 *b) {
    ball_fly(b, 1, func_003672A0, 0);
}

/* 0x003683D0 */
void Thing10_Flying(u8 *b) {
    ball_fly(b, 1, func_00367F80, 0);
}

void func_003338E0(u8 *b) {
    ball_roll(b, 0);
}

void func_00355220(u8 *b) {
    ball_roll(b, 0);
}

/* 0x00334D60 */
void Thing06_Frame(u8 *o) {
    thing_stomped(o, 0x14, 8, func_003345D0);
}

/* 0x00336A10 */
void Thing04_Frame(u8 *o) {
    thing_stomped(o, 0xA, 6, func_00336290);
}

/* bursts of four quad drawers (0xFC0 bytes, D_0047A010, drawers from +0xB50; 0x22D0 bytes,
   D_0047A030, drawers from +0x1A50) */
extern void *D_0047A010[], *D_0047A030[];

static inline void Burst4_Init(void **obj, void **vtbl, u32 at) {
    s32 i;

    obj[0] = vtbl;
    for (i = 0; i < 4; i++) {
        obj[(at + i * 0x38) / 4] = D_00469D00;
        ((s32 *)obj)[(at + i * 0x38 + 4) / 4] = -1;
        obj[(at + i * 0x38) / 4] = D_0046FC30;
    }
}

/* their destructors: the four drawers back down to the drawer base (last first), then the
   effect base, then (flags > 0) delete */
extern void *D_0046F580[];

static inline u8 *Burst4_Destroy(u8 *o, void **vtbl, u32 at, s32 flags) {
    s32 i;

    if (o != NULL) {
        AT(o, 0x0, void **) = vtbl;
        for (i = 3; i >= 0; i--) {
            u8 *e = o + at + i * 0x38;

            if (e != NULL) {
                AT(e, 0x0, void **) = D_0046FC30;
                AT(e, 0x0, void **) = D_00469D00;
            }
        }
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* 0x0036A860 */
u8 *BurstA_dtor(u8 *o, s32 flags) {
    return Burst4_Destroy(o, D_0047A010, 0xB50, flags);
}

/* 0x0036BCC0 */
u8 *BurstB_dtor(u8 *o, s32 flags) {
    return Burst4_Destroy(o, D_0047A030, 0x1A50, flags);
}

/* their draws (unless the effects are paused): each drawer's record (+0x10) the current frame's
   (+0xFAC / +0x22BC) in its block, then drawn */

static inline void Burst4_Draw(u8 *o, u32 at, s32 cur, const u32 *base, const u32 *stride) {
    s32 i;

    for (i = 0; i < 4; i++) {
        u8 *d = o + at + i * 0x38;

        AT(d, 0x10, u8 *) = o + cur * stride[i] + base[i];
        func_002E56C0(d);
    }
}

/* 0x0036B130 */
void BurstA_Draw(u8 *o) {
    static const u32 base[4] = {0x10, 0x610, 0x670, 0xAF0}, stride[4] = {0x300, 0x30, 0x240, 0x30};

    if (func_002D6010(gEffects) != 0) {
        return;
    }
    Burst4_Draw(o, 0xB50, AT(o, 0xFAC, s32), base, stride);
}

/* 0x0036C7B0 */
void BurstB_Draw(u8 *o) {
    static const u32 base[4] = {0x10, 0xC10, 0xD30, 0x1930}, stride[4] = {0x600, 0x90, 0x600, 0x90};

    if (func_002D6010(gEffects) != 0) {
        return;
    }
    Burst4_Draw(o, 0x1A50, AT(o, 0x22BC, s32), base, stride);
}

/* a drawer's fixed set up: no texture yet, layer 0x19, the sprite cells on the 512x256 sheet
   (texture 1 / group 0x10, the first palette) */
static inline void burst_quad(QuadDrawer *q, f32 cy, s16 count, s16 x, s16 y, s16 w, s16 h, s8 flags, s8 frames) {
    q->tex = (u64)-1;
    q->corners = 0;
    q->cx = 0.0f;
    q->cy = cy;
    q->layer = 0x19;
    q->count = count;
    q->cellX = x;
    q->cellY = y;
    q->cellW = w;
    q->cellH = h;
    q->texW = 0x200;
    q->texH = 0x100;
    q->flags = flags;
    q->frames = frames;
    q->texId = 1;
    q->texGroup = 0x10;
    q->palette = -1;
}

/* the 0xFC0-byte burst's set up: frame 0, its four drawers (the last with its corners at +0xF60) */
/* 0x0036BB00 */
void BurstA_Start(u8 *o) {
    AT(o, 0xFAC, s32) = 0;
    AT(o, 0xFB0, u8) = 0;
    AT(o, 0xFA0, s32) = 0;
    AT(o, 0xFA4, s32) = 0;
    burst_quad((QuadDrawer *)(o + 0xB50), 0.0f, 0x10, 0x20, 0x40, 0x20, 0x20, 0x40, 1);
    burst_quad((QuadDrawer *)(o + 0xB88), -1.0f, 1, 0, 0xA0, 0x20, 0x40, 0x41, 0xA);
    burst_quad((QuadDrawer *)(o + 0xBC0), 0.0f, 0xC, 0xE, 0x6E, 4, 4, 0x40, 1);
    burst_quad((QuadDrawer *)(o + 0xBF8), 0.0f, 1, 0xC0, 0x60, 0x20, 0x20, 0x43, 0);
    AT(o, 0xC0C, u8 *) = o + 0xF60;
}

/* a quad record (0x30 bytes): its cell 0x48 x 0x30 / 0x10 frames, alpha, position, size, turn */
static inline void burst_rec(u8 *r, s32 alpha) {
    AT(r, 0x0, s32) = 0x48;
    AT(r, 0x4, s32) = 0x30;
    AT(r, 0x8, s32) = 0x10;
    if (alpha >= 0) {
        AT(r, 0xC, s32) = alpha;
    }
}

/* burst B's records: cell 0x7F x 0x41, `frames` frames */
static inline void burst_rec7f(u8 *r, s32 frames, s32 alpha) {
    AT(r, 0x0, s32) = 0x7F;
    AT(r, 0x4, s32) = 0x41;
    AT(r, 0x8, s32) = frames;
    if (alpha >= 0) {
        AT(r, 0xC, s32) = alpha;
    }
}

/* +0x18 start (arg: the point): in the current frame (+0xFAC), 16 pieces thrown out (records
 * +0x10, velocities +0xC30, falls +0xCF0, drifts +0xEA0 / +0xEE0), the flash (+0x610), 12 puffs
 * rising (records +0x670, velocities +0xDB0, rates +0xE70, heights +0xE40) and the ring on the
 * ground (+0xAF0, corners +0xF60, size +0xFA8), turned to the floor under the player (+0xF20
 * .. +0xF40: across, its normal, along) */
/* 0x0036A980 */
void BurstA_SetParams(u8 *o, f32 *arg) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD}, k005 = {0x3D4CCCCD}, kPi = {0x40490FDB},
                                          k002 = {0x3CA3D70A};
    VObject *rnd;
    f32 p[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    u8 *r;
    u32 tri;
    s32 i;
    f32 s;

    if (arg == NULL) {
        return;
    }
    p[0] = arg[0];
    p[1] = arg[1];
    p[2] = arg[2];
    p[3] = 1.0f;
    sceVu0CopyVector(q, p);
    rnd = gRandom;
    for (i = 0; i < 16; i++) {
        r = o + 0x10 + AT(o, 0xFAC, s32) * 0x300 + i * 0x30;
        burst_rec(r, -1);
        AT(r, 0xC, s32) = (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0x3F) + 0x40;
        AT(r, 0x10, f32) = p[0] + 4.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x14, f32) = 1.0f + p[1];
        AT(r, 0x18, f32) = p[2] + 4.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x1C, f32) = 1.0f;
        AT(r, 0x20, f32) = 4.0f + 4.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(r, 0x24, f32) = AT(r, 0x20, f32);
        AT(r, 0x28, f32) = kPi.f * (360.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f)) / 180.0f;
        AT(r, 0x2C, s32) = 0;
        AT(o, 0xC30 + i * 0xC, f32) = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0xC34 + i * 0xC, f32) = k01.f + k01.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(o, 0xC38 + i * 0xC, f32) = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        s = k01.f + k005.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(o, 0xCF0 + i * 0xC, f32) = -(AT(o, 0xC30 + i * 0xC, f32) * s);
        AT(o, 0xCF4 + i * 0xC, f32) = -(0.5f * (AT(o, 0xC34 + i * 0xC, f32) * s));
        AT(o, 0xCF8 + i * 0xC, f32) = -(AT(o, 0xC38 + i * 0xC, f32) * s);
        AT(o, 0xEA0 + i * 4, f32) = AT(o, 0xC30 + i * 0xC, f32) / 8.0f;
        AT(o, 0xEE0 + i * 4, f32) = AT(o, 0xC38 + i * 0xC, f32) / 8.0f;
    }
    r = o + 0x610 + AT(o, 0xFAC, s32) * 0x30;
    burst_rec(r, 0x80);
    AT(r, 0x10, f32) = p[0];
    AT(r, 0x14, f32) = p[1];
    AT(r, 0x18, f32) = p[2];
    AT(r, 0x1C, f32) = 1.0f;
    AT(r, 0x20, f32) = 9.0f;
    AT(r, 0x24, f32) = 14.0f;
    AT(r, 0x28, f32) = 0.0f;
    AT(r, 0x2C, s32) = 0;
    for (i = 0; i < 12; i++) {
        r = o + 0x670 + AT(o, 0xFAC, s32) * 0x240 + i * 0x30;
        burst_rec(r, i % 2 == 0 ? 0x40 : 0x80);
        AT(r, 0x10, f32) = p[0] + 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x14, f32) = 1.0f + p[1];
        AT(r, 0x18, f32) = p[2] + 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x1C, f32) = 1.0f;
        AT(r, 0x20, f32) = k01.f + 0.5f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(r, 0x24, f32) = AT(r, 0x20, f32);
        AT(r, 0x2C, s32) = 0;
        AT(r, 0x28, s32) = 0;
        AT(o, 0xDB0 + i * 0xC, f32) = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0xDB4 + i * 0xC, f32) = 3.5f * (k01.f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)) - 0.5f;
        AT(o, 0xDB8 + i * 0xC, f32) = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0xE70 + i * 4, f32) = k002.f + k01.f * AT(o, 0xDB4 + i * 0xC, f32);
        AT(o, 0xE40 + i * 4, f32) = AT(r, 0x14, f32);
    }
    r = o + 0xAF0 + AT(o, 0xFAC, s32) * 0x30;
    burst_rec(r, 0x80);
    AT(r, 0x10, f32) = p[0];
    AT(r, 0x14, f32) = p[1];
    AT(r, 0x18, f32) = p[2];
    AT(r, 0x1C, f32) = 1.0f;
    AT(r, 0x20, f32) = 1.0f;
    AT(r, 0x24, f32) = 1.0f;
    AT(r, 0x28, f32) = 0.0f;
    AT(r, 0x2C, s32) = 0;
    AT(o, 0xFA8, f32) = 2.0f;
    for (i = 0; i < 4; i++) {
        AT(o, 0xF60 + i * 0x10, f32) = i & 1 ? 2.0f : -2.0f;
        AT(o, 0xF64 + i * 0x10, f32) = 0.0f;
        AT(o, 0xF68 + i * 0x10, f32) = i & 2 ? 2.0f : -2.0f;
        AT(o, 0xF6C + i * 0x10, f32) = 1.0f;
    }
    tri = Actor_TriOf((Actor *)gCharPlayer, p);
    if (tri == (u32)-1) {
        return;
    }
    sceVu0UnitMatrix((f32 (*)[4])(o + 0xF20));
    v[2] = 0.0f;
    v[3] = 1.0f;
    v[1] = 0.0f;
    v[0] = 0.0f;
    VCALL(gNavMesh, 0x2C, void (*)(VObject *, u32, f32 *))((VObject *)gNavMesh, tri, (f32 *)(o + 0xF30));
    sceVu0CopyVector(v, (f32 *)(o + 0xF30));
    sceVu0ScaleVector(v, v, k01.f);
    sceVu0AddVector((f32 *)(r + 0x10), v, (f32 *)(r + 0x10));
    sceVu0Normalize((f32 *)(o + 0xF30), (f32 *)(o + 0xF30));
    if (AT(o, 0xF34, f32) == 1.0f) {
        return;
    }
    AT(o, 0xF20, s32) = 0;
    AT(o, 0xF24, u32) = 0xBF800000;   /* -1 */
    AT(o, 0xF28, s32) = 0;
    AT(o, 0xF2C, s32) = 0;
    sceVu0OuterProduct((f32 *)(o + 0xF20), (f32 *)(o + 0xF30), (f32 *)(o + 0xF20));
    sceVu0Normalize((f32 *)(o + 0xF20), (f32 *)(o + 0xF20));
    sceVu0OuterProduct((f32 *)(o + 0xF40), (f32 *)(o + 0xF30), (f32 *)(o + 0xF20));
    sceVu0Normalize((f32 *)(o + 0xF40), (f32 *)(o + 0xF40));
}

/* a nav triangle's flags (+0x3C of its 0x50 bytes; 0 past the count +0x8 or with no table +0x4) */
static inline u32 nav_tri_flags(VObject *nav, u32 tri) {
    u8 *tris = AT(nav, 0x4, u8 *);

    return (tri < AT(nav, 0x8, u32) && tris != NULL) ? AT(tris + tri * 0x50, 0x3C, u32) : 0;
}

/* a quad record carried over from the other frame buffer (12 words) */
static inline void rec_copy(u8 *dst, const u8 *src) {
    s32 k;

    for (k = 0; k < 12; k++) {
        AT(dst, k * 4, u32) = AT(src, k * 4, u32);
    }
}

/* the ring's corner (x * 1.5 size, 0, z size) turned to face the camera */
static inline void ring_corner(u8 *o, u32 at, f32 x, f32 z) {
    f32 c[4] __attribute__((aligned(16)));
    f32 t[4] __attribute__((aligned(16)));

    c[1] = 0.0f;
    c[0] = 1.5f * x;
    c[3] = 1.0f;
    c[2] = z;
    Vec_TurnY(t, c, VCALL(gCamera, 0x68, f32 (*)(VObject *))(gCamera));
    AT(o, at, f32) = t[0];
    AT(o, at + 4, f32) = t[1];
    AT(o, at + 8, f32) = t[2];
    AT(o, at + 0xC, f32) = 1.0f;
}

/* where a burst keeps its parts (A: 0xFC0 bytes, one flash and ring; B: 0x22D0, three) */
typedef struct BurstShape {
    u32 cur, done, age;                                   /* frame buffer, all gone, frames */
    s32 nPiece; u32 piece, pieceBuf, vel, fall, driftX, driftZ;
    s32 nFlash; u32 flash, flashBuf, flashFrames;
    s32 nPuff; u32 puff, puffBuf, puffVel, puffRate;
    s32 nRing; u32 ring, ringBuf, size, corners, frame;
    f32 driftDiv;                                         /* the push grows by itself / this */
    s32 fallAge;                                          /* frames the falls apply */
    f32 liftEven, liftOdd;                                /* the even / odd pieces' rise x fall */
    f32 grow;                                             /* the pieces' growth a frame (0: none) */
} BurstShape;

/* +0x10 update (the frame buffers swapped, each record carried over): the pieces spin, drift
 * (their push growing by itself / driftDiv, over 4 frames), then shrink their cells, fade and
 * fall (for fallAge frames by their falls; rising liftEven / liftOdd x their fall); the flashes
 * fade and play their frames once; the puffs rise and fade, out when under the floor; the rings
 * fade and grow (the first by 1, the others 0.5), facing the camera. 0 once all of it was
 * gone. */
static inline __attribute__((always_inline)) s32 burst_update(u8 *o, const BurstShape *b) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    VObject *rnd, *nav;
    f32 g[4] __attribute__((aligned(16)));
    u8 *r;
    s32 i;

#define CUR AT(o, b->cur, s32)
    if (AT(o, b->done, u8) == 1) {
        return 0;
    }
    AT(o, b->done, u8) = 1;
    rnd = gRandom;
    CUR ^= 1;
    for (i = 0; i < b->nPiece; i++) {
        f32 a;

        rec_copy(o + b->piece + CUR * b->pieceBuf + i * 0x30, o + b->piece + (CUR ^ 1) * b->pieceBuf + i * 0x30);
        r = o + b->piece + CUR * b->pieceBuf + i * 0x30;
        if (AT(r, 0xC, s32) <= 0) {
            continue;
        }
        AT(o, b->done, u8) = 0;
        if (b->grow != 0.0f) {
            AT(r, 0x20, f32) = AT(r, 0x20, f32) + b->grow;
            AT(r, 0x24, f32) = AT(r, 0x20, f32);
        }
        a = AT(r, 0x28, f32) + kPi.f * (2.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)) / 180.0f;
        AT(r, 0x28, f32) = a;
        if (!(a < kPi.f)) {
            AT(r, 0x28, f32) = a - k2Pi.f;
        }
        AT(r, 0x10, f32) = AT(r, 0x10, f32) + (AT(o, b->vel + i * 0xC, f32) + AT(o, b->driftX + i * 4, f32));
        AT(r, 0x18, f32) = AT(r, 0x18, f32) + (AT(o, b->vel + 8 + i * 0xC, f32) + AT(o, b->driftZ + i * 4, f32));
        if (AT(o, b->age, s32) < 4) {
            AT(o, b->driftX + i * 4, f32) = AT(o, b->driftX + i * 4, f32) + AT(o, b->driftX + i * 4, f32) / b->driftDiv;
            AT(o, b->driftZ + i * 4, f32) = AT(o, b->driftZ + i * 4, f32) + AT(o, b->driftZ + i * 4, f32) / b->driftDiv;
            continue;
        }
        if (AT(r, 0x0, s32) != 0 && --AT(r, 0x0, s32) < 0) {
            AT(r, 0x0, s32) = 0;
        }
        if (AT(r, 0x4, s32) != 0 && --AT(r, 0x4, s32) < 0) {
            AT(r, 0x4, s32) = 0;
        }
        if (AT(r, 0x8, s32) != 0 && --AT(r, 0x8, s32) < 0) {
            AT(r, 0x8, s32) = 0;
        }
        AT(r, 0xC, s32) = AT(r, 0xC, s32) - ((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 7) + 1);
        if (AT(r, 0xC, s32) < 0) {
            AT(r, 0xC, s32) = 0;
        }
        if (AT(o, b->age, s32) < b->fallAge) {
            AT(o, b->vel + i * 0xC, f32) = AT(o, b->vel + i * 0xC, f32) + AT(o, b->fall + i * 0xC, f32);
            AT(o, b->vel + 4 + i * 0xC, f32) = AT(o, b->vel + 4 + i * 0xC, f32) + AT(o, b->fall + 4 + i * 0xC, f32);
            AT(o, b->vel + 8 + i * 0xC, f32) = AT(o, b->vel + 8 + i * 0xC, f32) + AT(o, b->fall + 8 + i * 0xC, f32);
        }
        if (i % 2 == 0) {
            AT(r, 0x14, f32) = AT(r, 0x14, f32) + b->liftEven * AT(o, b->vel + 4 + i * 0xC, f32);
        } else {
            AT(r, 0x14, f32) = AT(r, 0x14, f32) + b->liftOdd * AT(o, b->vel + 4 + i * 0xC, f32);
        }
        AT(o, b->driftX + i * 4, s32) = 0;
        AT(o, b->driftZ + i * 4, s32) = 0;
    }
    AT(o, b->age, s32)++;

    for (i = 0; i < b->nFlash; i++) {
        rec_copy(o + b->flash + CUR * b->flashBuf + i * 0x30, o + b->flash + (CUR ^ 1) * b->flashBuf + i * 0x30);
        r = o + b->flash + CUR * b->flashBuf + i * 0x30;
        if (AT(r, 0xC, s32) > 0) {
            AT(o, b->done, u8) = 0;
            if (--AT(r, 0xC, s32) < 0) {
                AT(r, 0xC, s32) = 0;
            }
            if (!(++AT(r, 0x2C, s32) < AT(o, b->flashFrames, s8))) {
                AT(r, 0xC, s32) = 0;
                AT(r, 0x2C, s32) = 0;
            }
        }
    }

    nav = (VObject *)gNavMesh;
    for (i = 0; i < b->nPuff; i++) {
        u32 tri;

        rec_copy(o + b->puff + CUR * b->puffBuf + i * 0x30, o + b->puff + (CUR ^ 1) * b->puffBuf + i * 0x30);
        r = o + b->puff + CUR * b->puffBuf + i * 0x30;
        AT(r, 0x10, f32) = AT(r, 0x10, f32) + AT(o, b->puffVel + i * 0xC, f32);
        AT(r, 0x14, f32) = AT(r, 0x14, f32) + AT(o, b->puffVel + 4 + i * 0xC, f32);
        AT(r, 0x18, f32) = AT(r, 0x18, f32) + AT(o, b->puffVel + 8 + i * 0xC, f32);
        AT(o, b->puffVel + 4 + i * 0xC, f32) = AT(o, b->puffVel + 4 + i * 0xC, f32) - AT(o, b->puffRate + i * 4, f32);
        tri = Actor_TriOf((Actor *)gCharPlayer, (f32 *)(r + 0x10));
        sceVu0CopyVector(g, (f32 *)(r + 0x10));
        VCALL(nav, 0x14, void (*)(VObject *, u32, f32 *))(nav, tri, g);
        if (!(nav_tri_flags(nav, tri) & 0x10000000) && AT(r, 0x14, f32) < g[1]) {
            AT(r, 0xC, s32) = 0;
            continue;
        }
        AT(r, 0xC, s32) = AT(r, 0xC, s32) - ((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 7) + 1);
        if (AT(r, 0xC, s32) < 0) {
            AT(r, 0xC, s32) = 0;
        }
    }

    for (i = 0; i < b->nRing; i++) {
        u32 c = b->corners + i * 0x40;

        rec_copy(o + b->ring + CUR * b->ringBuf + i * 0x30, o + b->ring + (CUR ^ 1) * b->ringBuf + i * 0x30);
        r = o + b->ring + CUR * b->ringBuf + i * 0x30;
        if (AT(r, 0xC, s32) <= 0) {
            continue;
        }
        AT(o, b->done, u8) = 0;
        AT(r, 0xC, s32) = AT(r, 0xC, s32) - 13;
        if (AT(r, 0xC, s32) < 0) {
            AT(r, 0xC, s32) = 0;
        }
        AT(o, b->size, f32) = AT(o, b->size, f32) + (i == 0 ? 1.0f : 0.5f);
        ring_corner(o, c, -AT(o, b->size, f32), -AT(o, b->size, f32));
        ring_corner(o, c + 0x10, AT(o, b->size, f32), -AT(o, b->size, f32));
        ring_corner(o, c + 0x20, -AT(o, b->size, f32), AT(o, b->size, f32));
        ring_corner(o, c + 0x30, AT(o, b->size, f32), AT(o, b->size, f32));
        sceVu0ApplyMatrix((f32 *)(o + c), (f32 (*)[4])(o + b->frame), (f32 *)(o + c));
        sceVu0ApplyMatrix((f32 *)(o + c + 0x10), (f32 (*)[4])(o + b->frame), (f32 *)(o + c + 0x10));
        sceVu0ApplyMatrix((f32 *)(o + c + 0x20), (f32 (*)[4])(o + b->frame), (f32 *)(o + c + 0x20));
        sceVu0ApplyMatrix((f32 *)(o + c + 0x30), (f32 (*)[4])(o + b->frame), (f32 *)(o + c + 0x30));
    }
#undef CUR
    return 1;
}

/* 0x0036B200 */
s32 BurstA_Update(u8 *o) {
    static const BurstShape kA = {0xFAC, 0xFB0, 0xFA4, 16, 0x10, 0x300, 0xC30, 0xCF0, 0xEA0, 0xEE0,
                                  1, 0x610, 0x30, 0xBBB, 12, 0x670, 0x240, 0xDB0, 0xE70,
                                  1, 0xAF0, 0x30, 0xFA8, 0xF60, 0xF20, 2.0f, 8, 4.0f, 8.0f};

    return burst_update(o, &kA);
}

/* 0x0036C880 */
s32 BurstB_Update(u8 *o) {
    static const BurstShape kB = {0x22BC, 0x22C0, 0x22B4, 32, 0x10, 0x600, 0x1B30, 0x1CB0, 0x20B0, 0x2130,
                                  3, 0xC10, 0x90, 0x1ABB, 32, 0xD30, 0x600, 0x1E30, 0x2030,
                                  3, 0x1930, 0x90, 0x22B8, 0x21F0, 0x21B0, 2.0f, 8, 4.0f, 8.0f};

    return burst_update(o, &kB);
}

/* ---- the shove burst D_00474FB0 (0xFD0 bytes, ShoveBurst_Init): where a kind-2 thing breaks
 * or a character is shoved - 16 pieces (records +0x10 + 0x300 x the current one +0xFC8,
 * velocities +0xC80, falls +0xD40, drifts +0xF40 / +0xF80) and 16 puffs (records +0x610,
 * velocities +0xE00, rates +0xF00, heights +0xEC0), +0xFC4 its frames, +0xFCC all gone ---- */

/* +0x18 start (arg: the point): the pieces thrown out round it (2..4 big, brown), the puffs
 * rising from it (every other one half alpha) */
/* 0x0032E950 */
void ThingBurst_SetParams(u8 *o, f32 *arg) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD}, k005 = {0x3D4CCCCD}, kPi = {0x40490FDB},
                                          k360 = {0x43B40000}, k15 = {0x3FC00000}, k25 = {0x40200000};   /* multiplied first */
    VObject *rnd;
    f32 x, y, z, sc;
    s32 i;

    if (arg == NULL) {
        return;
    }
    rnd = gRandom;
    x = arg[0];
    y = 1.0f + arg[1];
    z = arg[2];
    for (i = 0; i < 16; i++) {
        u8 *r = o + 0x10 + AT(o, 0xFC8, s32) * 0x300 + i * 0x30;

        AT(r, 0x0, s32) = 0x5F;
        AT(r, 0x4, s32) = 0x48;
        AT(r, 0x8, s32) = 0x1C;
        AT(r, 0xC, s32) = (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0x3F) + 0x40;
        AT(r, 0x10, f32) = x + 4.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x14, f32) = y;
        AT(r, 0x18, f32) = z + 4.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x1C, f32) = 1.0f;
        AT(r, 0x20, f32) = 2.0f + 2.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(r, 0x24, f32) = AT(r, 0x20, f32);
        AT(r, 0x28, f32) = kPi.f * (k360.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f)) / 180.0f;
        AT(r, 0x2C, s32) = 0;
        AT(o, 0xC80 + i * 0xC, f32) = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0xC84 + i * 0xC, f32) = k01.f + k01.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(o, 0xC88 + i * 0xC, f32) = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        sc = k01.f + k005.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(o, 0xD40 + i * 0xC, f32) = -(AT(o, 0xC80 + i * 0xC, f32) * sc);
        AT(o, 0xD44 + i * 0xC, f32) = -(0.5f * (AT(o, 0xC84 + i * 0xC, f32) * sc));
        AT(o, 0xD48 + i * 0xC, f32) = -(AT(o, 0xC88 + i * 0xC, f32) * sc);
        AT(o, 0xF40 + i * 4, f32) = AT(o, 0xC80 + i * 0xC, f32) / 8.0f;
        AT(o, 0xF80 + i * 4, f32) = AT(o, 0xC88 + i * 0xC, f32) / 8.0f;
    }
    for (i = 0; i < 16; i++) {
        u8 *r = o + 0x610 + AT(o, 0xFC8, s32) * 0x300 + i * 0x30;

        AT(r, 0x0, s32) = 0x5F;
        AT(r, 0x4, s32) = 0x48;
        AT(r, 0x8, s32) = 0x1C;
        AT(r, 0xC, s32) = i % 2 == 0 ? 0x40 : 0x80;
        AT(r, 0x10, f32) = x + 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x14, f32) = y;
        AT(r, 0x18, f32) = z + 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x1C, f32) = 1.0f;
        AT(r, 0x20, f32) = k01.f + 0.5f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(r, 0x24, f32) = AT(r, 0x20, f32);
        AT(r, 0x2C, s32) = 0;
        AT(r, 0x28, s32) = 0;
        AT(o, 0xE00 + i * 0xC, f32) = k15.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0xE04 + i * 0xC, f32) = k25.f * (k01.f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)) - 0.5f;
        AT(o, 0xE08 + i * 0xC, f32) = k15.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0xF00 + i * 4, f32) = 0x1.47ae14p-6f + k01.f * AT(o, 0xE04 + i * 0xC, f32);   /* 0.02 + 0.1 x */
        AT(o, 0xEC0 + i * 4, f32) = AT(r, 0x14, f32);
    }
}

/* +0x14 draw (not while the effects are paused): the pieces, then the puffs */
/* 0x0032EEB0 */
void ThingBurst_Draw(u8 *o) {
    if (func_002D6010(gEffects) == 0) {
        AT(o, 0xC20, u8 *) = o + AT(o, 0xFC8, s32) * 0x300 + 0x10;
        func_002E56C0(o + 0xC10);
        AT(o, 0xC58, u8 *) = o + AT(o, 0xFC8, s32) * 0x300 + 0x610;
        func_002E56C0(o + 0xC48);
    }
}

/* +0x10 update: as the bursts' (the pieces growing 0.05, their push by a sixth, falls for 10
 * frames, rising 2 / 4 x their fall), no flash or ring */
/* 0x0032EF30 */
s32 ThingBurst_Update(u8 *o) {
    static const BurstShape kC = {0xFC8, 0xFCC, 0xFC4, 16, 0x10, 0x300, 0xC80, 0xD40, 0xF40, 0xF80,
                                  0, 0, 0, 0, 16, 0x610, 0x300, 0xE00, 0xF00,
                                  0, 0, 0, 0, 0, 0, 6.0f, 10, 2.0f, 4.0f, 0x1.99999ap-5f /* 0.05 */};

    return burst_update(o, &kC);
}

/* 0x0032F5B0 */
Character *Kind27_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00474FD0); }

/* 0x0032F6D0 */
void Kind27_DoorOffset(void *self, s32 id, f32 *out) {
    switch (id) {
    case 1:
        B5_SET3(out, 0.0f, 0x1.4ccccc0000000p+3f /* 10.4 */);
        break;
    case 3:
        B5_SET3(out, 0.0f, -0x1.dc2f840000000p+2f /* 7.4404 */);
        break;
    case 0:
        B5_SET3(out, 0.0f, -0x1.8d89380000000p+2f /* 6.2115 */);
        break;
    case 2:
        B5_SET3(out, 0.0f, 0x1.9276c80000000p+2f /* 6.2885 */);
        break;
    }
}

/* 0x0032F770 */
void Kind27_ActionOffsets(void *self, s32 id, f32 *out) {
    switch (id) {
    case 10:
    case 11:
        B5_SET3(out, 0x1.e2f8380000000p-1f /* 0.9433 */, 0x1.6807600000000p+3f /* 11.2509 */);
        break;
    case 12:
    case 13:
        B5_SET3(out, 0x1.1656040000000p+1f /* 2.1745 */, 0x1.e1573e0000000p+3f /* 15.0419 */);
        break;
    case 14:
        B5_SET3(out, -0x1.9c98600000000p+0f /* 1.6117 */, -0x1.15e00e0000000p+2f /* 4.3418 */);
        break;
    case 15:
        B5_SET3(out, 0x1.2a30560000000p-6f /* 0.0182 */, -0x1.00346e0000000p+0f /* 1.0008 */);
        break;
    }
}

/* 0x0032F820 */
void Kind27_ExitDone(u8 *self) {
    self[0x16EE] = 1;
}

/* 0x0032F830 */
void Kind27_FollowPathExit(void) {
}

/* 0x0032F840 */
void Kind27_Arrived(void) {
}

/* 0x0032F850 */
void Kind27_WalkToExit(void) {
}

/* 0x0032F860 */
void Kind27_OnToNextExit(void) {
}

/* 0x0032F870 */
s32 Kind27_PickDestination(void) {
    return 0;
}

extern f32 D_004469A8, D_004469AC, D_004469B0;   /* the three points' offsets across the view */

/* +0x18 start (arg: the point): three points across the view from it (offsets D_004469A8..),
 * each with a flash (+0xC10; the first bigger) and a ring on the ground (+0x1930, corners
 * +0x21F0, the drawer +0x1AF8 showing three); 32 pieces (+0x10, velocities +0x1B30, falls
 * +0x1CB0, drifts +0x20B0 / +0x2130) and 32 puffs (+0xD30, velocities +0x1E30, rates +0x2030,
 * heights +0x1FB0) shared round them */
/* 0x0036BDE0 */
void BurstB_SetParams(u8 *o, f32 *arg) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD}, k005 = {0x3D4CCCCD}, kPi = {0x40490FDB},
                                          k002 = {0x3CA3D70A}, k13 = {0x3FA66666};
    VObject *rnd, *cam, *nav;
    f32 xs[3];
    f32 q[4] __attribute__((aligned(16)));
    f32 pts[3][4] __attribute__((aligned(16)));
    f32 c[4] __attribute__((aligned(16)));
    f32 t[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    u8 *r;
    u32 tri;
    s32 i;
    f32 sp;

    if (arg == NULL) {
        return;
    }
    q[0] = arg[0];
    xs[0] = D_004469A8;
    xs[1] = D_004469AC;
    q[1] = arg[1];
    xs[2] = D_004469B0;
    cam = gCamera;
    q[2] = arg[2];
    q[3] = 1.0f;
    for (i = 0; i < 3; i++) {
        r = o + 0xC10 + AT(o, 0x22BC, s32) * 0x90 + i * 0x30;
        burst_rec7f(r, 0x1E, 0x80);
        c[2] = 0.0f;
        c[3] = 1.0f;
        c[0] = xs[i];
        c[1] = 0.0f;
        Vec_TurnY(t, c, VCALL(cam, 0x68, f32 (*)(VObject *))(cam));
        AT(r, 0x10, f32) = q[0] + t[0];
        AT(r, 0x14, f32) = q[1] + t[1];
        AT(r, 0x18, f32) = q[2] + t[2];
        AT(r, 0x1C, f32) = 1.0f;
        sceVu0CopyVector(pts[i], (f32 *)(r + 0x10));
        AT(r, 0x20, f32) = 9.0f;
        AT(r, 0x24, f32) = 14.0f;
        if (i == 0) {
            AT(r, 0x20, f32) = 12.0f;
            AT(r, 0x24, f32) = 17.0f;
        }
        AT(r, 0x28, s32) = 0;
        AT(r, 0x2C, s32) = 0;
    }
    rnd = gRandom;
    for (i = 0; i < 32; i++) {
        r = o + 0x10 + AT(o, 0x22BC, s32) * 0x600 + i * 0x30;
        burst_rec7f(r, 0x14, -1);
        AT(r, 0xC, s32) = (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0x3F) + 0x40;
        sceVu0CopyVector(q, pts[i % 3]);
        AT(r, 0x10, f32) = q[0] + 4.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x14, f32) = 1.0f + q[1];
        AT(r, 0x18, f32) = q[2] + 4.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x1C, f32) = 1.0f;
        AT(r, 0x20, f32) = 4.0f + 4.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(r, 0x24, f32) = AT(r, 0x20, f32);
        AT(r, 0x28, f32) = kPi.f * (360.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f)) / 180.0f;
        AT(r, 0x2C, s32) = 0;
        AT(o, 0x1B30 + i * 0xC, f32) = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0x1B34 + i * 0xC, f32) = k01.f + k01.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(o, 0x1B38 + i * 0xC, f32) = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        sp = k01.f + k005.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        AT(o, 0x1CB0 + i * 0xC, f32) = -(AT(o, 0x1B30 + i * 0xC, f32) * sp);
        AT(o, 0x1CB4 + i * 0xC, f32) = -(0.5f * (AT(o, 0x1B34 + i * 0xC, f32) * sp));
        AT(o, 0x1CB8 + i * 0xC, f32) = -(AT(o, 0x1B38 + i * 0xC, f32) * sp);
        AT(o, 0x20B0 + i * 4, f32) = AT(o, 0x1B30 + i * 0xC, f32) / 8.0f;
        AT(o, 0x2130 + i * 4, f32) = AT(o, 0x1B38 + i * 0xC, f32) / 8.0f;
    }
    for (i = 0; i < 32; i++) {
        r = o + 0xD30 + AT(o, 0x22BC, s32) * 0x600 + i * 0x30;
        burst_rec7f(r, 0x1E, i % 2 == 0 ? 0x40 : 0x80);
        sceVu0CopyVector(q, pts[i % 3]);
        AT(r, 0x10, f32) = q[0] + 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x14, f32) = 1.0f + q[1];
        AT(r, 0x18, f32) = q[2] + 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x1C, f32) = 1.0f;
        AT(r, 0x20, f32) = k01.f + k13.f * (0.5f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd));
        AT(r, 0x24, f32) = AT(r, 0x20, f32);
        AT(r, 0x2C, s32) = 0;
        AT(r, 0x28, s32) = 0;
        AT(o, 0x1E30 + i * 0xC, f32) = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0x1E34 + i * 0xC, f32) = 3.5f * (k01.f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)) - 0.5f;
        AT(o, 0x1E38 + i * 0xC, f32) = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0x2030 + i * 4, f32) = k002.f + k01.f * AT(o, 0x1E34 + i * 0xC, f32);
        AT(o, 0x1FB0 + i * 4, f32) = AT(r, 0x14, f32);
    }
    nav = (VObject *)gNavMesh;
    for (i = 0; i < 3; i++) {
        u8 *corners = o + 0x21F0 + i * 0x40;
        s32 k;

        burst_quad((QuadDrawer *)(o + 0x1AF8), 0.0f, 3, 0xC0, 0x60, 0x20, 0x20, 0x43, 0);
        AT(o, 0x1B0C, u8 *) = corners;
        r = o + 0x1930 + AT(o, 0x22BC, s32) * 0x90 + i * 0x30;
        burst_rec7f(r, 0x1E, 0x80);
        sceVu0CopyVector(q, pts[i]);
        AT(r, 0x10, f32) = q[0];
        AT(r, 0x14, f32) = q[1];
        AT(r, 0x18, f32) = q[2];
        AT(r, 0x1C, f32) = 1.0f;
        AT(r, 0x20, f32) = 1.0f;
        AT(r, 0x24, f32) = 1.0f;
        AT(r, 0x28, s32) = 0;
        AT(r, 0x2C, s32) = 0;
        AT(o, 0x22B8, f32) = 1.0f;
        for (k = 0; k < 4; k++) {
            AT(corners, k * 0x10, f32) = k & 1 ? 2.0f : -2.0f;
            AT(corners, k * 0x10 + 4, f32) = 0.0f;
            AT(corners, k * 0x10 + 8, f32) = k & 2 ? 2.0f : -2.0f;
            AT(corners, k * 0x10 + 0xC, f32) = 1.0f;
        }
        tri = Actor_TriOf((Actor *)gCharPlayer, q);
        if (tri == (u32)-1) {
            continue;
        }
        sceVu0UnitMatrix((f32 (*)[4])(o + 0x21B0));
        v[2] = 0.0f;
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[3] = 1.0f;
        VCALL(nav, 0x2C, void (*)(VObject *, u32, f32 *))(nav, tri, (f32 *)(o + 0x21C0));
        sceVu0CopyVector(v, (f32 *)(o + 0x21C0));
        sceVu0ScaleVector(v, v, k01.f);
        sceVu0AddVector((f32 *)(r + 0x10), v, (f32 *)(r + 0x10));
        sceVu0Normalize((f32 *)(o + 0x21C0), (f32 *)(o + 0x21C0));
        if (AT(o, 0x21C4, f32) == 1.0f) {
            continue;
        }
        AT(o, 0x21B0, s32) = 0;
        AT(o, 0x21B4, u32) = 0xBF800000;   /* -1 */
        AT(o, 0x21B8, s32) = 0;
        AT(o, 0x21BC, s32) = 0;
        sceVu0OuterProduct((f32 *)(o + 0x21B0), (f32 *)(o + 0x21C0), (f32 *)(o + 0x21B0));
        sceVu0Normalize((f32 *)(o + 0x21B0), (f32 *)(o + 0x21B0));
        sceVu0OuterProduct((f32 *)(o + 0x21D0), (f32 *)(o + 0x21C0), (f32 *)(o + 0x21B0));
        sceVu0Normalize((f32 *)(o + 0x21D0), (f32 *)(o + 0x21D0));
    }
}

/* the 0x22D0-byte burst's set up: frame 0, the first three drawers */
/* 0x0036D210 */
void BurstB_Start(u8 *o) {
    AT(o, 0x22BC, s32) = 0;
    AT(o, 0x22C0, u8) = 0;
    AT(o, 0x22B0, s32) = 0;
    AT(o, 0x22B4, s32) = 0;
    burst_quad((QuadDrawer *)(o + 0x1A50), 0.0f, 0x20, 0x20, 0x40, 0x20, 0x20, 0x40, 1);
    burst_quad((QuadDrawer *)(o + 0x1A88), -1.0f, 3, 0, 0xA0, 0x20, 0x40, 0x41, 0xA);
    burst_quad((QuadDrawer *)(o + 0x1AC0), 0.0f, 0x20, 0xE, 0x6E, 4, 4, 0x40, 1);
}

static inline void Burst4A_Init(void **obj) {
    Burst4_Init(obj, D_0047A010, 0xB50);
}

static inline void Burst4B_Init(void **obj) {
    Burst4_Init(obj, D_0047A030, 0x1A50);
}

/* 0x00335260 */
void Thing05_Frame(u8 *o) {
    thing_watch(o, 0xFC0, Burst4A_Init);
}

/* 0x00335820 */
void Thing07_Frame(u8 *o) {
    thing_watch(o, 0x22D0, Burst4B_Init);
}

/* ---- the thing the second stalker (gCharSlot2) is drawn to (code 0x3332B0..0x3344A0) and
   more of the 0x367000 kinds ---- */

extern VObject *gSceneGameF29740;
extern VObject *D_00456DF8;          /* the room objects */

/* the second stalker (kinds 2, 6, 7, 0x1B) and the thing: when he comes into its room away
   from Fiona it makes a noise (once per room, +0x122) and goes unless he's alerted (+0x16C9 >=
   5). In Fiona's room, when he is searching (+0x16C8 1..3) and can reach it, he notices it
   (a noise; at level 4 unless busy, func_0029A850, he goes for it and takes it when within 10
   of it or of his path's end) */
void func_003332B0(u8 *o) {
    u8 *s = (u8 *)gCharSlot2;

    if (s == NULL || AT(s, 0x28, u8) == 0) {
        return;
    }
    switch (AT(s, 0x153C, u8)) {
    case 0x1B:
    case 7:
    case 6:
    case 2:
        break;
    default:
        return;
    }
    if (AT(s, 0x30, s32) != AT(o, 0x30, s32)) {
        return;
    }
    if (AT(o, 0x30, s32) != AT(gCharPlayer, 0x30, s32)) {
        if (AT(o, 0x122, u16) == (u16)AT(s, 0x30, s32)) {
            return;
        }
        AT(o, 0x122, u16) = AT(s, 0x30, s32);
        Noise_Make((u8 *)gProgress + 0x7A8, 0x1F, AT(o, 0x30, s32), AT(o, 0x34, u32), 0xFFFF);
        if (AT(s, 0x16C9, u8) < 5) {
            AT(o, 0x28, u8) = 0;
        }
        return;
    }
    switch (AT(s, 0x16C8, u8)) {
    case 1:
    case 3:
    case 2:
        break;
    default:
        return;
    }
    if (!(func_002187D0((Pursuer *)s, AT(o, 0x34, u32), (f32 *)(o + 0x10)) & 0xFF)) {
        return;
    }
    if (Character_PathLength((Character *)s, AT(o, 0x34, u32), (f32 *)(o + 0x10), -1) <= 0.0f) {
        return;
    }
    if (AT(s, 0x16C9, u8) != 4) {
        Noise_Make((u8 *)gProgress + 0x7A8, 0x1F, AT(o, 0x30, s32), AT(o, 0x34, u32), 0xFFFF);
        return;
    }
    if (func_0029A850((Pursuer *)s) != 0) {
        return;
    }
    VCALL(s, 0xAC, void (*)(void *, u32, f32 *, s32))(s, AT(o, 0x34, u32), (f32 *)(o + 0x10), -1);
    func_0027E5A0((Pursuer *)s, AT(o, 0x34, u32));
    if (Actor_Distance((Actor *)o, (f32 *)(s + 0x10)) < 10.0f) {
        AT(o, 0x28, u8) = 0;
        return;
    }
    if (VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(gSceneGameF29740,
            (f32 *)(s + 0x10), AT(s, 0x128, s32), AT(s, 0x124, s32), s + 0x12C) < 10.0f) {
        AT(o, 0x28, u8) = 0;
    }
}

/* +0x30 each frame (as Ball_Frame): gone after 2.5 minutes; the stalker's check; in Fiona's
   room her kick */
/* 0x003342D0 */
void Thing03_Frame(u8 *b) {
    Thing_Frame(b);
    if (AT(b, 0x28, u8) == 0) {
        return;
    }
    if (AT(b, 0xE4, u32) >= 9000) {
        AT(b, 0x28, u8) = 0;
        return;
    }
    func_003332B0(b);
    if (AT(b, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return;
    }
    thing_kick(b);
}

/* +0x2C draw: its model lying flat (-90 degrees), 0.3 up, turned by +0x124 / 256; in its last
   8 frames (age 8993..9000) faded out */
/* 0x00333510 */
void Thing03_Draw(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kDown = {0xBFC90FDB};
    struct {
        void **vtbl;
        s32 a;
        u8 rest[0x38];
    } d __attribute__((aligned(16)));
    f32 pos[4] __attribute__((aligned(16)));
    f32 rot[4] __attribute__((aligned(16)));
    s32 late;

    sceVu0CopyVector(pos, (f32 *)(o + 0x10));
    pos[1] += 0x1.333334p-2f;   /* 0.3 */
    rot[1] = 0.0f;
    rot[0] = kDown.f;
    rot[2] = kPi.f * (360.0f * ((f32)(u32)AT(o, 0x124, u16) / 256.0f) - 180.0f) / 180.0f;
    d.vtbl = D_00476B50;
    d.a = -1;
    late = AT(o, 0xE4, s32) - 8992;
    if (late > 0) {
        VObject *tc = gTexCache;

        VCALL(tc, 0x18, void (*)(VObject *))(tc);
        VCALL(gRenderer, 0x70, void (*)(VObject *, u32))(gRenderer, (u32)(8 - late) << 28 | 0x808080);
        ModelDraw_Fill((u8 *)&d, pos, rot, 2, 0, 0xF);
        VCALL(tc, 0x18, void (*)(VObject *))(tc);
    } else {
        ModelDraw_Fill((u8 *)&d, pos, rot, 2, 0, 1);
    }
    d.vtbl = D_00469D00;
}

/* +0xC set up: the actor's, blocked by nav flags 0x20020008, in the current room, white, the
   pursuer's room (0xFFFF none) and a random turn */
/* 0x003344A0 */
void Thing03_Setup(u8 *o) {
    Thing_Setup(o);
    AT(o, 0xC4, s32) = 0;
    AT(o, 0xC0, u32) = 0x20020008;
    AT(o, 0xE0, u8) = 0;
    AT(o, 0x30, s32) = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    AT(o, 0x11C, f32) = 1.0f;
    AT(o, 0x118, f32) = 1.0f;
    AT(o, 0x114, f32) = 1.0f;
    AT(o, 0x110, f32) = 1.0f;
    AT(o, 0x120, u16) = 0;
    if (gCharPursuer != NULL) {
        AT(o, 0x122, u16) = AT(gCharPursuer, 0x30, s32);
    } else {
        AT(o, 0x122, u16) = 0xFFFF;
    }
    AT(o, 0x124, u16) = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 0xFF;
}

extern const s8 D_0047B018[6];

/* its room object: the key D_0047B018 with +0x122 added to its fifth byte */
static inline u8 *thing_room_obj(u8 *o) {
    u8 key[6];
    s32 i;

    for (i = 0; i < 6; i += 2) {
        key[i] = D_0047B018[i];
        key[i + 1] = D_0047B018[i + 1];
    }
    key[4] += AT(o, 0x122, u8);
    return VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, u8 *))(D_00456DF8, key);
}

/* its room object kept at its position */
/* 0x00367220 */
void Thing09_Draw(u8 *o) {
    sceVu0CopyVector((f32 *)(thing_room_obj(o) + 0x20), (f32 *)(o + 0x10));
}

/* +0xC set up: the actor's, blocked by nav flags 0x20020008, in the current room, white; its
   room object's flag cleared */
/* 0x00367D50 */
void Thing09_Setup(u8 *o) {
    Thing_Setup(o);
    AT(o, 0xC4, s32) = 0;
    AT(o, 0xC0, u32) = 0x20020008;
    AT(o, 0xE0, u8) = 0;
    AT(o, 0x30, s32) = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    AT(o, 0x11C, f32) = 1.0f;
    AT(o, 0x118, f32) = 1.0f;
    AT(o, 0x114, f32) = 1.0f;
    AT(o, 0x110, f32) = 1.0f;
    AT(o, 0x120, u16) = 0;
    *thing_room_obj(o) = 0;
}

/* falling: the velocity +0xB0 gains gravity (+0x100) and moves it */
void func_00367470(u8 *o) {
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    sceVu0AddVector((f32 *)(o + 0xB0), (f32 *)(o + 0x100), (f32 *)(o + 0xB0));
    sceVu0AddVector((f32 *)(o + 0x10), (f32 *)(o + 0xB0), (f32 *)(o + 0x10));
}

/* the same, gone once below -30 */
void func_00368150(u8 *o) {
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    sceVu0AddVector((f32 *)(o + 0xB0), (f32 *)(o + 0x100), (f32 *)(o + 0xB0));
    sceVu0AddVector((f32 *)(o + 0x10), (f32 *)(o + 0xB0), (f32 *)(o + 0x10));
    if (AT(o, 0x14, f32) < -30.0f) {
        AT(o, 0x28, u8) = 0;
    }
}

/* ---- the shared thing class (D_00479500, code 0x3544C0..0x355960): who it touches, when it
   goes off, Fiona's kick, its noise ---- */

#include "charaction.h"

/* actor c (feet cy, top ctop) and the span oy..otop overlap */
static inline s32 thing_spans(f32 cy, f32 ctop, f32 oy, f32 otop) {
    if (ctop < otop && !(ctop <= oy)) {
        return 1;
    }
    return otop < ctop && !(otop <= cy);
}

/* c's position within `r` of o's across the floor */
static inline s32 thing_near(u8 *c, u8 *o, f32 r) {
    f32 d[4] __attribute__((aligned(16)));

    sceVu0SubVector(d, (f32 *)(c + 0x10), (f32 *)(o + 0x10));
    return __builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) < r;
}

/* the stalkers (slots 7..9) standing on it (1 high, within their radius) get action 4 (sub 1,
   0xFF, its kind +0x126); 1 if any did */
s32 func_003544C0(u8 *o) {
    s32 hit = 0;
    s32 i;

    for (i = 7; i < 10; i++) {
        u8 *c = ((u8 **)gCreatures)[i];

        if (c == NULL || AT(c, 0x28, u8) != 1 || AT(o, 0x30, s32) != AT(c, 0x30, s32)) {
            continue;
        }
        if (!thing_spans(AT(c, 0x14, f32), AT(c, 0x14, f32) + AT(c, 0xCC, f32), AT(o, 0x14, f32), 1.0f + AT(o, 0x14, f32))) {
            continue;
        }
        if (thing_near(c, o, AT(c, 0xC8, f32))) {
            CharAction act;

            act.state = 4;
            act.a = 1;
            act.b = 0xFF;
            act.c = AT(o, 0x126, u16);
            act.d = 0;
            act.e = 0.0f;
            act.f = 0;
            act.g = 0;
            act.h = 0;
            act.i = 0;
            if (AT(c, 0x14E8, s32) != 7) {
                char_set_action(c, &act);
            }
            hit = 1;
        }
    }
    return hit;
}

/* once armed (age +0xE4 >= +0x128): the stalkers within its reach (+0x12C high, +0x12A across)
   get action 4 facing it */
void func_003546B0(u8 *o) {
    s32 i;

    if (AT(o, 0xE4, u32) < AT(o, 0x128, u16)) {
        return;
    }
    for (i = 7; i < 10; i++) {
        u8 *c = ((u8 **)gCreatures)[i];
        CharAction act;

        if (c == NULL || AT(c, 0x28, u8) != 1 || AT(o, 0x30, s32) != AT(c, 0x30, s32)) {
            continue;
        }
        if (!thing_spans(AT(c, 0x14, f32), AT(c, 0x14, f32) + AT(c, 0xCC, f32), AT(o, 0x14, f32),
                         AT(o, 0x14, f32) + (f32)(u32)AT(o, 0x12C, u16))) {
            continue;
        }
        if (!thing_near(c, o, (f32)(u32)AT(o, 0x12A, u16))) {
            continue;
        }
        act.state = 4;
        act.d = 0;
        act.a = 1;
        act.b = 0xFF;
        act.c = AT(o, 0x126, u16);
        act.e = Actor_HeadingTo((Actor *)c, (f32 *)(o + 0x10));
        act.f = 0;
        act.g = 0;
        act.h = 0;
        act.i = 0;
        if (AT(c, 0x14E8, s32) != 7) {
            char_set_action(c, &act);
        }
    }
}

/* once armed: 0 not yet, 2 set off - by the pursuer in its reach in the current room, or
   elsewhere in his room by a +0x122 % roll - else 1 */
s32 func_00354910(u8 *o) {
    u8 *p;
    s32 room;

    if (AT(o, 0xE4, u32) < AT(o, 0x128, u16)) {
        return 0;
    }
    p = (u8 *)gCharPursuer;
    if (p == NULL || AT(p, 0x28, u8) != 1) {
        return 1;
    }
    room = AT(o, 0x30, s32);
    if (room != AT(gCharPursuer, 0x30, s32)) {
        return 1;
    }
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        if (thing_spans(AT(p, 0x14, f32), AT(p, 0x14, f32) + AT(p, 0xCC, f32), AT(o, 0x14, f32),
                        AT(o, 0x14, f32) + (f32)(u32)AT(o, 0x12C, u16)) &&
            thing_near(p, o, (f32)(u32)AT(o, 0x12A, u16))) {
            return 2;
        }
        return 1;
    }
    if ((s32)(100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) < AT(o, 0x122, u16)) {
        return 2;
    }
    return 1;
}

/* in the current room, Fiona's kick */
void func_00354AF0(u8 *o) {
    if (AT(o, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return;
    }
    thing_kick(o);
}

/* its noise (kind +0x12E, loudness +0x126), 0x8000 (startling) by a +0x124 % roll */
void func_00354C90(u8 *o) {
    /* the loudness goes as the u16 it is (the callee takes its low half) */
    void (*noise)(Progress *, const f32 *, u32, u32, u32, s32, f32) =
        (void (*)(Progress *, const f32 *, u32, u32, u32, s32, f32))Progress_Noise;

    if ((s32)(100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) < AT(o, 0x124, u16)) {
        noise(gProgress, (f32 *)(o + 0x10), 4, AT(o, 0x12E, u8), AT(o, 0x126, u16), -0x8000, 0.0f);
    } else {
        noise(gProgress, (f32 *)(o + 0x10), 4, AT(o, 0x12E, u8), AT(o, 0x126, u16), 0, 0.0f);
    }
}

/* 1 when the pursuer stands on it in the current room */
s32 func_00354D50(u8 *o) {
    u8 *p;

    if (AT(o, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return 0;
    }
    p = (u8 *)gCharPursuer;
    if (p == NULL || AT(p, 0x28, u8) != 1 || AT(p, 0x29, u8) != 0) {
        return 0;
    }
    if (!thing_spans(AT(p, 0x14, f32), AT(p, 0x14, f32) + AT(p, 0xCC, f32), AT(o, 0x14, f32), 1.0f + AT(o, 0x14, f32))) {
        return 0;
    }
    return thing_near(p, o, AT(p, 0xC8, f32));
}

/* +0x44 put down (the base's): when placed (1), on the floor at least, and the items told */
/* 0x00354E70 */
u32 ThingShared_PutDown(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    u32 r = Thing_PutDown(o, tri, pos, rot, rr, h) & 0xFF;
    f32 fl[4] __attribute__((aligned(16)));

    if (r != 1) {
        return r;
    }
    sceVu0CopyVector(fl, (f32 *)(o + 0x10));
    VCALL(gNavMesh, 0x14, void (*)(VObject *, u32, f32 *))((VObject *)gNavMesh, AT(o, 0x34, u32), fl);
    if (AT(o, 0x14, f32) < fl[1]) {
        AT(o, 0x14, f32) = fl[1];
    }
    if (gSubScreen != NULL) {
        VCALL(gSubScreen, 0x1C, void (*)(VObject *))(gSubScreen);
    }
    return r;
}

/* +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`: a wall hit
   (0x40000000) -1, another hit stops `to` there */
/* 0x00354F20 */
s32 ThingShared_LandTri(u8 *b, u8 *a, f32 *to) {
    f32 from[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 out[4] __attribute__((aligned(16)));
    u32 r = -1;

    if (a != NULL) {
        sceVu0CopyVector(from, BALL_POS(a));
        from[1] = to[1];
        r = VCALL(gNavMesh, 0x44, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, f32 *, u32))(
            (VObject *)gNavMesh, AT(a, 0x34, u32), out, from, to, n, AT(b, 0xC0, u32));
    }
    if (r & 0xF0000000) {
        if (r & 0x40000000) {
            return -1;
        }
        sceVu0CopyVector(to, out);
        r &= 0xFFFF;
    }
    return r;
}

/* (as func_00354AF0) a kind's frame: Fiona's kick with less lift (0.6) */
/* 0x00367BC0 */
void Thing09_Frame(u8 *o) {
    Thing_Frame(o);
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    thing_kick_up(o, 0x1.333334p-1f);
}

/* 1 when the pursuer stands on it (half a unit high) */
s32 func_00367EA0(u8 *o) {
    u8 *p = (u8 *)gCharPursuer;

    if (p == NULL || AT(p, 0x28, u8) != 1 || AT(p, 0x29, u8) != 0) {
        return 0;
    }
    if (!thing_spans(AT(p, 0x14, f32), AT(p, 0x14, f32) + AT(p, 0xCC, f32), AT(o, 0x14, f32), 0.5f + AT(o, 0x14, f32))) {
        return 0;
    }
    return thing_near(p, o, AT(p, 0xC8, f32));
}

extern const PTMF D_0042F200, D_0042F210, D_0042F220;

/* (as Thing10_MotionState) +0x4C the motion state for this frame (none outside the current room),
   +0x13E 0xFF when it hasn't moved since last frame, and the fade */
/* 0x00333FA0 */
void Thing03_MotionState(u8 *b) {
    f32 s;

    if (AT(b, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        AT(b, 0xA0, PTMF) = sGameStateNull;
        return;
    }
    AT(b, 0x38, u32) = AT(b, 0x34, u32);
    AT(b, 0x13E, u16) = 0;
    s = AT(b, 0x18, f32) - AT(b, 0x48, f32) + ((AT(b, 0x10, f32) - AT(b, 0x40, f32)) + (AT(b, 0x14, f32) - AT(b, 0x44, f32)));
    if (s <= 0.0f) {
        s = -s;
    }
    if (0.0f == s) {
        AT(b, 0x13E, u16) = 0xFF;
    }
    sceVu0CopyVector((f32 *)(b + 0x40), BALL_POS(b));
    switch (AT(b, 0xE0, u8)) {
    case 0:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_0042F200);
        break;
    case 1:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_0042F210);
        break;
    default:
        ptmf_set(&AT(b, 0xA0, PTMF), &D_0042F220);
        break;
    }
    if (AT(b, 0x120, u16) != 0) {
        AT(b, 0x11C, f32) = AT(b, 0x11C, f32) - 0x1.555556p-3f /* 1/6 */;
        if (AT(b, 0x11C, f32) <= 0.0f) {
            AT(b, 0x11C, f32) = 0.0f;
            AT(b, 0x28, u8) = 0;
        }
    }
}

/* +0x30 each frame, while the game runs: gone after 7.5 s; Fiona's kick (0.4 up); the pursuer
   standing on it sets it off (sound 4, a level-0x28 noise) */
/* 0x003688D0 */
void Thing10_Frame(u8 *o) {
    Progress *p;

    Thing_Frame(o);
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) != 0) {
        return;
    }
    if ((VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) & 0xFF) == 1) {
        return;
    }
    p = gProgress;
    if ((Progress_TestFlag(p, 8) & 0xFF) == 1) {
        return;
    }
    if (AT(o, 0xE4, u32) >= 450) {
        AT(o, 0x28, u8) = 0;
        return;
    }
    thing_kick_up(o, 0x1.99999ap-2f);   /* 0.4 */
    if (func_00367EA0(o) != 0) {
        Sound_PlayBankAt(gSound, 4, 6, (f32 *)(o + 0x10), 0, 0);
        Progress_Noise(p, (f32 *)(o + 0x10), 4, 2, 0x28, 0, 0.0f);
        AT(o, 0x28, u8) = 0;
    }
}

extern void *D_0047A050[];

/* its burst (0x7A0 bytes, D_0047A050, a quad drawer at +0x610) */
static inline void Burst1_Init(void **obj) {
    obj[0] = D_0047A050;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* +0x30 each frame, while the game runs: the shared checks (func_00354910) set it off - in the
   current room a sound and a level-0x4F noise (at the door whose event spot it is on, else its
   triangle) and its burst; in a neighbouring room, through the door to the current room when
   that is open, its sound heard from there, and the noise; it is gone. Otherwise Fiona's kick */
/* 0x00335DE0 */
void Thing08_Frame(u8 *o) {
    VObject *ev, *rooms;
    Progress *p;
    s32 r, cur;
    u32 d, tri;

    ThingShared_Frame(o);
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) != 0) {
        return;
    }
    ev = gEvents;
    if ((VCALL(ev, 0x50, s32 (*)(VObject *))(ev) & 0xFF) == 1) {
        return;
    }
    p = gProgress;
    if ((Progress_TestFlag(p, 8) & 0xFF) == 1) {
        return;
    }
    r = func_00354910(o);
    if (r != 1 && r != 2) {
        func_00354AF0(o);
        return;
    }
    if (AT(o, 0x30, s32) == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        u8 *mgr;

        Actor_PlaySound((Actor *)o, 0, 5, 0, 0, NULL);
        tri = AT(o, 0x34, u32);
        rooms = gRooms;
        for (d = 0; d < 8; d = (d + 1) & 0xFF) {
            u32 door = VCALL(rooms, 0x48, u32 (*)(VObject *, s32, u32))(rooms, AT(o, 0x30, s32), d) & 0xFFFF;

            if (door != 0xFFFF &&
                VCALL(ev, 0x10, s32 (*)(VObject *, f32 *, u32, u32))(ev, (f32 *)(o + 0x10), door, AT(o, 0x34, u32)) != 0) {
                tri = VCALL(rooms, 0x28, u32 (*)(VObject *, u32))(rooms, d);
                break;
            }
        }
        Noise_Make((u8 *)p + 0x7A8, 0x4F, AT(o, 0x30, s32), tri, 0xFFFF);
        mgr = gEffects;
        EffectMgr_Start(mgr, Effect_New(mgr, 0x7A0, Burst1_Init), o + 0x10);
        AT(o, 0x28, u8) = 0;
        return;
    }
    cur = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    rooms = gRooms;
    for (d = 0; d < 8; d = (d + 1) & 0xFF) {
        if (cur == VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, AT(o, 0x30, s32), d)) {
            break;
        }
    }
    if ((d & 0xFF) != 8) {
        u32 door = VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, AT(o, 0x30, s32), d) & 0xFFFF;

        if ((Progress_DoorOpen(p, door) & 0xFF) == 1) {
            f32 at[4] __attribute__((aligned(16)));

            if ((Actor_PosInCurrentRoom((Actor *)o, at) & 0xFF) == 1) {
                Actor_PlaySound((Actor *)o, 0, 5, 0, 0, at);
            }
        }
    }
    Noise_Make((u8 *)p + 0x7A8, 0x4F, AT(o, 0x30, s32), AT(o, 0x34, u32), 0xFFFF);
    AT(o, 0x28, u8) = 0;
}

extern void *D_0046FF20[];

/* its puff (0x720 bytes, D_0046FF20, a quad drawer at +0x610) */
static inline void Puff_Init(void **obj) {
    obj[0] = D_0046FF20;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* (as ball_fly) +0x50 the flying state of this kind: always active; into a hole it settles
   there (state 2, +0x2A); a glancing wall hit drops it to the floor with a puff (size 1, kind
   2, grey 0x50, 16); it tries once more 0.2 back; the bounce sound only when it has moved
   (+0x13E 0) - and any wall bounce counts, even the last */
/* 0x00333AD0 */
void Thing03_Flying(u8 *b) {
    f32 *v = BALL_VEL(b);
    f32 to[4] __attribute__((aligned(16)));
    f32 out[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 fl[4] __attribute__((aligned(16)));
    VObject *nm;
    u8 *mgr;
    s32 bounced = 0, again = 0;
    u32 tri, prev, hit;

    sceVu0AddVector(v, (f32 *)(b + 0x100), v);
    nm = (VObject *)gNavMesh;
    mgr = gEffects;
    for (;;) {
        sceVu0AddVector(to, v, BALL_POS(b));
        tri = AT(b, 0x34, u32);
        for (;;) {
            prev = tri;
            tri = VCALL(nm, 0x44, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, f32 *, u32))(
                nm, tri & 0xFFFF, out, BALL_POS(b), to, n, AT(b, 0xC0, u32));
            if (tri == (u32)-1) {
                break;
            }
            hit = tri & 0xF0000000;
            if (hit == 0) {
                sceVu0CopyVector(fl, out);
                VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, fl);
                if (out[1] < fl[1]) {
                    AT(b, 0xE0, u8) = 1;
                    out[1] = fl[1];
                }
                goto landed;
            }
            if (hit == 0x80000000 && (ball_tri_flags(nm, tri & 0xFFFF) & TRI_HOLE)) {
                AT(b, 0x34, u32) = tri;
                sceVu0CopyVector(BALL_POS(b), out);
                AT(b, 0xE0, u8) = 2;
                AT(b, 0xE8, s32) = 0;
                AT(b, 0x2A, u8) = 1;
                return;
            }
            if (!func_003336B0(b, to, out, n)) {
                VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, AT(b, 0x34, u32), BALL_POS(b));
                AT(b, 0xE0, u8) = 1;
                return;
            }
            if (hit == 0x80000000) {
                f32 d = sceVu0InnerProduct(n, v);

                if (d <= 0.0f) {
                    d = -d;
                }
                if (d < 0.5f) {
                    struct {
                        f32 pos[3];
                        f32 size;
                        s32 kind;
                        s32 rgb[3];
                        s32 count;
                    } sp;
                    s32 slot;

                    tri &= 0xFFFF;
                    VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, out);
                    AT(b, 0xE0, u8) = 1;
                    slot = Effect_New(mgr, 0x720, Puff_Init);
                    sp.pos[0] = AT(b, 0x10, f32);
                    sp.pos[1] = AT(b, 0x14, f32);
                    sp.pos[2] = AT(b, 0x18, f32);
                    sp.size = 1.0f;
                    sp.count = 0x10;
                    sp.kind = 2;
                    sp.rgb[2] = 0x50;
                    sp.rgb[1] = 0x50;
                    sp.rgb[0] = 0x50;
                    EffectMgr_Start(mgr, slot, &sp);
                }
            }
            bounced = 1;
            if (AT(b, 0xE0, u8) == 1) {
                goto landed;
            }
        }
        if (again) {
            break;
        }
        again = 1;
        sceVu0ScaleVector(v, v, -0x1.99999ap-3f /* 0.2 */);
    }
    fl[1] = to[1];
    tri = VCALL(nm, 0x40, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, u32))(
        nm, prev & 0xFFFF, out, BALL_POS(b), to, AT(b, 0xC0, u32));
    if (tri != (u32)-1) {
        AT(b, 0x34, u32) = tri;
        sceVu0CopyVector(BALL_POS(b), out);
        AT(b, 0x14, f32) = fl[1];
    } else {
        VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, AT(b, 0x34, u32), BALL_POS(b));
        AT(b, 0xE0, u8) = 1;
    }
    return;
landed:
    AT(b, 0x34, u32) = tri;
    sceVu0CopyVector(BALL_POS(b), out);
    if (bounced && AT(b, 0x13E, u16) == 0) {
        Actor_PlaySound((Actor *)b, 0x7C, 5, 0, 0, NULL);
    }
}

/* ---- the burst D_0047A050 (0x7A0 bytes, Burst1_Init): 16 puffs thrown up from a point and
 * falling back, in two buffers of quad records (+0x10 + 0x300 x the current one +0x790),
 * velocities at +0x648 (12 each), falls at +0x748, start heights at +0x708, the quad drawer at
 * +0x610; +0x794 all gone ---- */

#define PUFF1_REC(o, buf, i) ((o) + 0x10 + (buf) * 0x300 + (i) * 0x30)
#define PUFF1_VEL(o, i) ((f32 *)((o) + 0x648 + (i) * 0xC))

/* +0x18 start (arg: the point): each puff a 0x48 x 0x30 cell (16 frames), every other one half
 * alpha, within 1 across of the point and 1 up, 0.1..2.1 big, thrown out and up */
/* 0x0036D3F0 */
void ThingPuff_SetParams(u8 *o, f32 *arg) {
    static const union { u32 u; f32 f; } k05 = {0x3F000000}, k15 = {0x3FC00000}, k25 = {0x40200000};   /* multiplied first */
    VObject *rnd;
    f32 x, y, z;
    s32 i;

    if (arg == NULL) {
        return;
    }
    rnd = gRandom;
    x = arg[0];
    y = 1.0f + arg[1];
    z = arg[2];
    for (i = 0; i < 16; i++) {
        u8 *r = PUFF1_REC(o, AT(o, 0x790, s32), i);
        f32 *v = PUFF1_VEL(o, i);

        AT(r, 0x0, s32) = 0x48;
        AT(r, 0x4, s32) = 0x30;
        AT(r, 0x8, s32) = 0x10;
        AT(r, 0xC, s32) = i % 2 == 0 ? 0x40 : 0x80;
        AT(r, 0x10, f32) = x + 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x14, f32) = y;
        AT(o, 0x708 + i * 4, f32) = y;
        AT(r, 0x18, f32) = z + 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(r, 0x1C, f32) = 1.0f;
        AT(r, 0x20, f32) = 0x1.99999ap-4f + 2.0f * (k05.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd));
        AT(r, 0x24, f32) = AT(r, 0x20, f32);
        AT(r, 0x2C, s32) = 0;
        AT(r, 0x28, s32) = 0;
        v[0] = k15.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        v[1] = k25.f * (0x1.99999ap-4f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)) - 0.5f;
        v[2] = k15.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0x748 + i * 4, f32) = 0x1.47ae14p-6f + 0x1.99999ap-4f * v[1];   /* 0.02 + 0.1 x */
    }
}

/* +0x14 draw (not while the effects are paused) */
/* 0x0036D6C0 */
void ThingPuff_Draw(u8 *o) {
    if (func_002D6010(gEffects) == 0) {
        AT(o, 0x620, u8 *) = o + AT(o, 0x790, s32) * 0x300 + 0x10;
        func_002E56C0(o + 0x610);
    }
}

/* +0x10 update: flip the buffers; each puff still showing carried over, moved and pulled down,
 * out once under the floor (where the floor counts), else fading by 1..8; 0 once all gone */
/* 0x0036D720 */
s32 ThingPuff_Update(u8 *o) {
    VObject *nav, *rnd;
    f32 g[4] __attribute__((aligned(16)));
    s32 i;

    if (AT(o, 0x794, u8) == 1) {
        return 0;
    }
    AT(o, 0x794, u8) = 1;
    nav = (VObject *)gNavMesh;
    rnd = gRandom;
    AT(o, 0x790, s32) ^= 1;
    for (i = 0; i < 16; i++) {
        u8 *r;
        f32 *v = PUFF1_VEL(o, i);
        u32 tri;

        rec_copy(PUFF1_REC(o, AT(o, 0x790, s32), i), PUFF1_REC(o, AT(o, 0x790, s32) ^ 1, i));
        r = PUFF1_REC(o, AT(o, 0x790, s32), i);
        if (AT(r, 0xC, s32) <= 0) {
            continue;
        }
        AT(o, 0x794, u8) = 0;
        AT(r, 0x10, f32) = AT(r, 0x10, f32) + v[0];
        AT(r, 0x14, f32) = AT(r, 0x14, f32) + v[1];
        AT(r, 0x18, f32) = AT(r, 0x18, f32) + v[2];
        v[1] = v[1] - AT(o, 0x748 + i * 4, f32);
        tri = Actor_TriOf((Actor *)gCharPlayer, (f32 *)(r + 0x10));
        sceVu0CopyVector(g, (f32 *)(r + 0x10));
        VCALL(nav, 0x14, void (*)(VObject *, u32, f32 *))(nav, tri, g);
        if (!(nav_tri_flags(nav, tri) & 0x10000000) && AT(r, 0x14, f32) < g[1]) {
            AT(r, 0xC, s32) = 0;
            continue;
        }
        AT(r, 0xC, s32) = AT(r, 0xC, s32) - ((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 7) + 1);
        if (AT(r, 0xC, s32) < 0) {
            AT(r, 0xC, s32) = 0;
        }
    }
    return 1;
}

/* +0xC set up: frame 0, the drawer (16 quads of a 4 x 4 cell at (14, 110), blended) */
/* 0x0036D9A0 */
void ThingPuff_Start(u8 *o) {
    AT(o, 0x790, s32) = 0;
    AT(o, 0x794, u8) = 0;
    AT(o, 0x788, s32) = 0;
    AT(o, 0x78C, s32) = 0;
    burst_quad((QuadDrawer *)(o + 0x610), 0.0f, 0x10, 0xE, 0x6E, 4, 4, 0x40, 1);
}
