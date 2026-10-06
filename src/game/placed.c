/* The placed things (gPlacedThings, Progress +0x6FC340, vtable PlacedThings_vtable): up to 128 actors of
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
#include "scene_game.h"
#include "sound.h"
#include "vecmath.h"
#include "msl.h"
#include "input.h"
#include "daniella.h"
#include "loader.h"
#include "pad.h"
#include "scene.h"
#include "scene_boot.h"
#include "scene_title.h"
#include "system.h"
#include "text.h"
#include "libc.h"
#include "sce/iop.h"
#include "renderer.h"
#include "gl2d.h"
#include "music.h"
#include "camera.h"
#include "heap.h"
#include "char_load.h"
#include "creature.h"
#include "doors.h"
#include "event.h"
#include "gameover.h"
#include "hewie.h"
#include "model.h"
#include "movie.h"
#include "fiona.h"
#include "pause.h"
#include "room_map.h"
#include "draw_leaves.h"
#include "sce/eekernel.h"
#include "cri/adx.h"
#include "subscreen.h"
#include "director.h"
#include "room.h"
#include "lights.h"
#include "sce/intc.h"
#include "effectmgr.h"
#include "ptmf.h"
#include "charaction.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif

extern void *Actor_vtable[];   /* Actor */
extern void *PlacedThings_vtable[], *Thing_vtable[], *BlockPool_vtable[], *D_004699E0[], *D_0046A950[];
extern void *Ball_vtable[], *Thing01_vtable[], *Thing02_vtable[], *Thing03_vtable[], *Thing04_vtable[], *Thing05_vtable[],
    *Thing06_vtable[], *Thing07_vtable[], *Thing08_vtable[], *Thing09_vtable[], *Thing10_vtable[], *ThingShared_vtable[];

#define POOL(m) ((m) + 0xA040)
#define SAVED(p) ((u8 *)(p) + 0xA14)
#define NUM_SAVED 60

extern void *D_00476B50[];

void ThingShared_Frame(u8 *o);

extern void *PlacedObjects_vtable[], *D_0046F390[];
extern VObject *gRoomObjects;          /* the room objects */

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *PlacedThings_ctor(u8 *p);

void PlacedThings_ctorPool(u8 *p);

/* Free an element by address. */
/* Allocate an element of the given size. */
/* Constructor: base vtable, then derived vtable. */
/* 0x00120F40 */
void *Thing_dtor(void *p) {
    if (p != NULL) {
        *(void **)p = Thing_vtable;
        *(void **)p = Actor_vtable;
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
    u8 *t = BlockPool_ElemAt((B0_Pool *)POOL(m), i);

    if (t != NULL && AT(t, 0x28, u8) == 1) {
        return t;
    }
    return NULL;
}

/* destructor (the pool +0xA040 with it) */
/* (possibly dead code: nothing in the game references it) */
/* 0x002D0EE0 */
void *PlacedThings_Destroy(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = PlacedThings_vtable;
        AT(m, 0xA040, void **) = BlockPool_vtable;
        AT(m, 0xA040, void **) = D_004699E0;
        AT(m, 0x0, void **) = D_0046A950;
        gPlacedThings = NULL;
        if ((s16)flags > 0) {
            func_00100490(m);
        }
    }
    return m;
}

/* 0x002D1510 */
void *PlacedThings_ctor(u8 *p) {
    u8 *a = p + 0xA040;

    gPlacedThings = (VObject *)p;
    F(p, 0x0, void *) = PlacedThings_vtable;
    F(a, 0x0, void *) = D_004699E0;
    F(a, 0x4, u32) = 0;
    F(a, 0x8, u32) = 0;
    F(a, 0x0, void *) = BlockPool_vtable;
    F(a, 0xC, u32) = 0;
    F(a, 0x10, u32) = 0;
    F(a, 0x14, u32) = 0;
    return p;
}

/* +0x8 a new thing of `kind` (0..10) from the pool (NULL: none / full) */
/* 0x002D6B00 */
void *PlacedThings_New(u8 *m, u32 kind) {
    static void **const sClass[11] = {
        Ball_vtable, Thing01_vtable, Thing02_vtable, Thing03_vtable, Thing04_vtable, Thing05_vtable,
        Thing06_vtable, Thing07_vtable, Thing08_vtable, Thing09_vtable, Thing10_vtable,
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
        AT(t, 0x0, void **) = Actor_vtable;
        AT(t, 0x20, u32) = kind;
        AT(t, 0x24, u32) = 0x01000000;
        AT(t, 0x0, void **) = sClass[kind];
    }
    return mem;
}

/* the destructor of a thing (ThingShared_vtable) */
/* 0x002D6F60 */
void *ThingShared_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = ThingShared_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
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
/* 0x002A7700 */
void ThingSave_Clear(s32 *e) {
    e[0] = -1;
    e[1] = -1;
    e[2] = -1;
    e[3] = 0;
    e[4] = 0;
    e[5] = 0;
}

/* (possibly dead code: nothing in the game references it) */
/* destructor (PlacedObjects_vtable): its 64 entries (+0x20, 0xB0 each), then the base (D_0046F390,
 * clearing gRoomObjects) */
/* (possibly dead code: nothing in the game references it) */
/* 0x002D0D60 */
void *PlacedThings_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = PlacedObjects_vtable;
        func_001002C0(o + 0x20, (void *(*)(void *, s32))QuadEntry_dtor, 0xB0, 0x40);
        AT(o, 0x0, void **) = D_0046F390;
        gRoomObjects = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
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
        ThingSave_Clear(e);
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
    t = BlockPool_ElemAt((B0_Pool *)POOL(m), i);
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
        VObject *t = BlockPool_ElemAt((B0_Pool *)POOL(m), i);

        if (t != NULL) {
            VCALL((VObject *)POOL(m), 0x14, void (*)(VObject *, void *))((VObject *)POOL(m), t);
            if (t != NULL) {
                VCALL(t, 0x8, void (*)(VObject *, s32))(t, 1);
            }
        }
    }
}

/* the dynamic actors, drawn each frame (texture cache and gBootMessage reset first): each
 * active one in the current room (+0x2C) */
/* 0x002D74E0 */
void PlacedThings_Draw(u8 *o) {
    s32 room, i;

    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
    room = VCALL(gProgress, 0xC, s32 (*)(void *))(gProgress);
    for (i = 0; i < 0x80; i++) {
        VObject *a = BlockPool_ElemAt((B0_Pool *)(o + 0xA040), i);

        if (a != NULL && AT(a, 0x28, u8) == 1 && AT(a, 0x30, s32) == room) {
            VCALL(a, 0x2C, void (*)(VObject *))(a);
        }
    }
}

/* the dynamic actors, each frame (128 slots in the pool +0xA040): an active one updates
 * (+0x30), a finished one is given back to the pool (+0x14) and destroyed */
/* 0x002D75C0 */
void PlacedThings_Update(u8 *o) {
    s32 i;

    for (i = 0; i < 0x80; i++) {
        VObject *a = BlockPool_ElemAt((B0_Pool *)(o + 0xA040), i);

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

/* a pool of 128 blocks of 0x140 bytes */
/* 0x002D7680 */
void PlacedThings_ctorPool(u8 *p) {
    BlockPool_Init((BlockPool *)(p + 0xA040), p + 0x40, 0x140, 0x80, p + 0xA058);
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

/* ---- kind 0: the ball (vtable Ball_vtable), which Fiona throws to train Hewie. +0xB0 its
 * velocity, +0x100 gravity, +0x104 how hard slopes push it, +0x110 its colour (RGBA, 1.0 =
 * full), +0x120 fading out, +0xE0 its motion (0 flying, 1 rolling, 2 dropping through a hole,
 * +0xE8 the frames of it), +0xE1 kicked this contact. It lasts 3 minutes (+0xE4); when it goes
 * and Fiona has no ball, she gets hers (item 0x90) back. ---- */


extern const PTMF sGameStateNull;
extern const PTMF D_00414790, Ball_StateRolling_ptmf, Ball_StateDropping_ptmf;   /* +0x50 (virtual), Ball_StateRolling, Ball_StateDropping */
extern void *QuadDrawer_vtable[], *Helper469D00_vtable[];
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
        AT(b, 0x0, void **) = Ball_vtable;
        if (gSubScreen != NULL) {
            u8 *items = (u8 *)gSubScreen + 8;

            if (!(Items_Count(items, 0x90) & 0xFF)) {
                Items_Give(items, 0x90, 1);
            }
        }
        AT(b, 0x0, void **) = Thing_vtable;
        AT(b, 0x0, void **) = Actor_vtable;
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
    q.vtbl = QuadDrawer_vtable;
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
    Drawer_Submit((u8 *)&q);
    q.vtbl = Helper469D00_vtable;
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

/* 0x002D5290 */
s32 Ball_Bounce(u8 *b, f32 *to, f32 *at, f32 *n) {
    return ball_bounce(b, to, at, n, 0x1.666666p-1f /* 0.7 */);
}

/* the dropping state: falling through for 31 frames, then gone */
/* 0x002D5460 */
void Ball_StateDropping(u8 *b) {
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
/* 0x002D54D0 */
void Ball_StateRolling(u8 *b) {
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
    ball_fly(b, 1, Ball_Bounce, 1);
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
        ptmf_set(&AT(b, 0xA0, PTMF), &Ball_StateRolling_ptmf);
        break;
    default:
        ptmf_set(&AT(b, 0xA0, PTMF), &Ball_StateDropping_ptmf);
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

/* ---- the things' base class (Thing_vtable, over the actor): +0x10 position, +0x34 nav tri,
 * +0x50 turn (angles), +0xB0 a point in front, +0xEC / +0xF0 its two sizes, +0xE4 its age
 * (frames; -1 kept) ---- */


extern const PTMF D_003AF1B8;   /* a thing's resting state */

/* (possibly unused by the vtables) place it: nav tri, position, turn, front point */
/* 0x00120F90 */
void Thing_Place(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 *front) {
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

/* ---- kind 1 (Thing01_vtable): like the ball (kind 0) it flies, falls and is kicked, but it can't
 * bounce - whatever it hits, or a pursuer walking into it, bursts it in a purple splash ---- */

extern void *Thing01_vtable[];
extern const PTMF D_00429C28, Thing01_StateDropping_ptmf;   /* +0x50 (virtual), Thing01_StateDropping */

/* +0x8 destructor */
/* 0x00314990 */
void *Thing01_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Thing01_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
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
/* 0x00314A00 */
void Thing01_Burst(u8 *o, f32 *at) {
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
    q.vtbl = QuadDrawer_vtable;
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
    Drawer_Submit((u8 *)&q);
    q.vtbl = Helper469D00_vtable;
}

/* +0x2C draw: a half-size sprite in its colour */
/* 0x00314B90 */
void Thing01_Draw(u8 *o) {
    half_sprite(o, 64.0f, 32.0f, 128.0f);
}

/* the dropping state (Thing01_StateDropping_ptmf): falling through for 31 frames, then gone */
/* 0x00314CE0 */
void Thing01_StateDropping(u8 *o) {
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
        Thing01_Burst(o, out);
        return;
    }
    hit = tri & 0xF0000000;
    if (hit == 0) {
        sceVu0CopyVector(fl, out);
        VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, fl);
        if (out[1] < fl[1]) {
            AT(o, 0x28, u8) = 0;
            Thing01_Burst(o, fl);
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
    Thing01_Burst(o, out);
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
        ptmf_set(&AT(o, 0xA0, PTMF), &Thing01_StateDropping_ptmf);
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
    thing_stomped(o, 5, 4, Thing01_Burst);
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

/* ---- kind 2 (Thing02_vtable, over the shared thing class ThingShared_vtable): set down at a random turn;
 * it goes off (sound 0x8E and a ThingBurst_vtable burst at it) when the shared checks say so ---- */

extern void *Thing02_vtable[], *ThingShared_vtable[], *D_00476B50[];
extern void ThingShared_Setup(u8 *o);   /* ThingShared_vtable +0xC */
extern void ThingShared_ReachStalkers(u8 *o);
extern s32 ThingShared_Armed(u8 *o);
extern void ThingShared_Noise(u8 *o);
extern s32 ThingShared_StompStalkers(u8 *o);
extern s32 ThingShared_PursuerOn(u8 *o);
extern void ThingShared_Kick(u8 *o);

/* +0x8 destructor */
/* 0x00315540 */
void *Thing02_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Thing02_vtable;
        AT(o, 0x0, void **) = ThingShared_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
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
    d.vtbl = Helper469D00_vtable;
}

/* +0x2C draw */
/* 0x003155C0 */
void Thing02_Draw(u8 *o) {
    turned_model(o, 1, 0, 1);
}

static inline __attribute__((always_inline)) void kind2_burst(u8 *o, u32 size, void (*init)(void **)) {
    u8 *mgr;

    Actor_PlaySound((Actor *)o, 0x8E, 5, 0, 0, NULL);
    mgr = gEffects;
    EffectMgr_Start(mgr, Effect_New(mgr, size, init), o + 0x10);
}

/* +0x30 each frame, while the game runs: the shared checks (ThingShared_Armed: 1 / 2 set off -
 * the burst only in the current room; else ThingShared_StompStalkers / ThingShared_PursuerOn set it off, or it
 * waits, ThingShared_Kick) */
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
    ThingShared_ReachStalkers(o);
    r = ThingShared_Armed(o);
    if (r == 1 || r == 2) {
        if (r == 2 && Progress_TestFlag(p, 0x20) == 0) {
            ThingShared_Noise(o);
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
    a = ThingShared_StompStalkers(o) & 0xFF;
    b = ThingShared_PursuerOn(o) & 0xFF;
    if (a == 0 && b == 0) {
        ThingShared_Kick(o);
        return;
    }
    Actor_PlaySound((Actor *)o, 0x8E, 5, 0, 0, NULL);
    if (b != 0) {
        ThingShared_Noise(o);
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
extern void *Thing09_vtable[];
extern void *Thing10_vtable[];
extern void *Thing06_vtable[];
extern void Thing06_Burst(u8 *o, f32 *at);
extern void *Thing07_vtable[];
extern void *Thing08_vtable[];
extern void Thing04_Burst(u8 *o, f32 *at);

/* (as Ball_Bounce)  the ball bounced at `at` (off the surface with normal n): its velocity mirrored and slowed
 * to 0.7; `to` = what is left of the step from `at` on (0: nothing left) */
/* 0x00354FF0 */
s32 Thing02_Bounce(u8 *b, f32 *to, f32 *at, f32 *n) {
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

/* (as Thing01_StateDropping)  the dropping state (Thing01_StateDropping_ptmf): falling through for 31 frames, then gone */
/* 0x003551C0 */
void Thing02_StateDropping(u8 *o) {
    sceVu0AddVector(BALL_VEL(o), (f32 *)(o + 0x100), BALL_VEL(o));
    sceVu0AddVector(BALL_POS(o), BALL_VEL(o), BALL_POS(o));
    if (++AT(o, 0xE8, u32) >= 31) {
        AT(o, 0x28, u8) = 0;
    }
}

/* (as ThingShared_dtor)  the destructor of a thing (Thing09_vtable) */
/* 0x003671B0 */
void *Thing09_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Thing09_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as Ball_StateRolling)  the rolling state: moved on the nav mesh, slowed by friction (0.03 x the slope's flatness)
 * and pushed down the slope */
/* 0x003674C0 */
void Thing09_StateRolling(u8 *b) {
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

/* (as ThingShared_dtor)  the destructor of a thing (Thing10_vtable) */
/* 0x00367E20 */
void *Thing10_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Thing10_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* 0x00367E90 */
void Thing10_Draw(void) {
}

/* (as Ball_StateRolling)  the rolling state: moved on the nav mesh, slowed by friction (0.03 x the slope's flatness)
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

/* 0x003681D0 */
void Thing10_StateRolling(u8 *b) {
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

/* (as ThingShared_dtor)  the destructor of a thing (Thing03_vtable) */
/* 0x00333240 */
void *Thing03_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Thing03_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as Thing01_StateDropping)  the dropping state (Thing01_StateDropping_ptmf): falling through for 31 frames, then gone */
/* 0x00333880 */
void Thing03_StateDropping(u8 *o) {
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

/* (as ThingShared_dtor)  the destructor of a thing (Thing06_vtable) */
/* 0x00334560 */
void *Thing06_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Thing06_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as Thing01_StateDropping)  the dropping state (Thing01_StateDropping_ptmf): falling through for 31 frames, then gone */
/* 0x003348A0 */
void Thing06_StateDropping(u8 *o) {
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
        Thing06_Burst(o, out);
        return;
    }
    hit = tri & 0xF0000000;
    if (hit == 0) {
        sceVu0CopyVector(fl, out);
        VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, fl);
        if (out[1] < fl[1]) {
            AT(o, 0x28, u8) = 0;
            Thing06_Burst(o, fl);
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
    Thing06_Burst(o, out);
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
        AT(o, 0x0, void **) = Thing05_vtable;
        AT(o, 0x0, void **) = ThingShared_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
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
        AT(o, 0x0, void **) = Thing07_vtable;
        AT(o, 0x0, void **) = ThingShared_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
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
        AT(o, 0x0, void **) = Thing08_vtable;
        AT(o, 0x0, void **) = ThingShared_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as ThingShared_dtor)  the destructor of a thing (Thing04_vtable) */
/* 0x00336220 */
void *Thing04_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Thing04_vtable;
        AT(o, 0x0, void **) = Thing_vtable;
        AT(o, 0x0, void **) = Actor_vtable;
        if ((s16)flags > 0) {
            ActorPool_delete(o);
        }
    }
    return o;
}

/* (as Thing01_StateDropping)  the dropping state (Thing01_StateDropping_ptmf): falling through for 31 frames, then gone */
/* 0x00336550 */
void Thing04_StateDropping(u8 *o) {
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
        Thing04_Burst(o, out);
        return;
    }
    hit = tri & 0xF0000000;
    if (hit == 0) {
        sceVu0CopyVector(fl, out);
        VCALL(nm, 0x14, void (*)(VObject *, u32, f32 *))(nm, tri, fl);
        if (out[1] < fl[1]) {
            AT(o, 0x28, u8) = 0;
            Thing04_Burst(o, fl);
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
    Thing04_Burst(o, out);
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
extern const PTMF Thing02_StateRolling_ptmf;
extern const PTMF Thing02_StateDropping_ptmf;
extern const PTMF D_00445B60;
extern const PTMF Thing09_StateRolling_ptmf;
extern const PTMF Thing09_StateFalling_ptmf;
extern const PTMF D_00445B90;
extern const PTMF Thing10_StateRolling_ptmf;
extern const PTMF Thing10_StateFalling_ptmf;
extern const PTMF D_0042F230;
extern const PTMF Thing06_StateDropping_ptmf;
extern const PTMF D_0042F250;
extern const PTMF Thing04_StateDropping_ptmf;

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
        ptmf_set(&AT(b, 0xA0, PTMF), &Thing02_StateRolling_ptmf);
        break;
    default:
        ptmf_set(&AT(b, 0xA0, PTMF), &Thing02_StateDropping_ptmf);
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
        ptmf_set(&AT(b, 0xA0, PTMF), &Thing09_StateRolling_ptmf);
        break;
    default:
        ptmf_set(&AT(b, 0xA0, PTMF), &Thing09_StateFalling_ptmf);
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
        ptmf_set(&AT(b, 0xA0, PTMF), &Thing10_StateRolling_ptmf);
        break;
    default:
        ptmf_set(&AT(b, 0xA0, PTMF), &Thing10_StateFalling_ptmf);
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
        ptmf_set(&AT(o, 0xA0, PTMF), &Thing06_StateDropping_ptmf);
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
        ptmf_set(&AT(o, 0xA0, PTMF), &Thing04_StateDropping_ptmf);
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

/* (as Ball_Bounce) */
/* 0x003336B0 */
s32 Thing03_Bounce(u8 *b, f32 *to, f32 *at, f32 *n) {
    return ball_bounce(b, to, at, n, 0x1.99999ap-3f /* 0.2 */);
}

/* (as Ball_Bounce) */
/* 0x00367F80 */
s32 Thing10_Bounce(u8 *b, f32 *to, f32 *at, f32 *n) {
    return ball_bounce(b, to, at, n, 0x1.99999ap-3f /* 0.2 */);
}

/* (as Ball_Bounce) */
/* 0x003672A0 */
s32 Thing09_Bounce(u8 *b, f32 *to, f32 *at, f32 *n) {
    return ball_bounce(b, to, at, n, 0x1.99999ap-2f /* 0.4 */);
}

/* ---- the shared thing class (ThingShared_vtable) and the other kinds over it (code 0x333000..0x337800):
   their settings, splashes, sprites and models through the helpers above ---- */

/* ThingShared_vtable +0xC set up: the actor's, blocked by nav flags 0x20020008, in the current room,
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
/* 0x00336290 */
void Thing04_Burst(u8 *o, f32 *at) {
    drop_splash(o, at, 0x80, 0x64, 0, 1.0f);
}

/* 0x003345D0 */
void Thing06_Burst(u8 *o, f32 *at) {
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
    ball_fly(b, 0, Thing02_Bounce, 1);
}

/* 0x003676C0 */
void Thing09_Flying(u8 *b) {
    ball_fly(b, 1, Thing09_Bounce, 0);
}

/* 0x003683D0 */
void Thing10_Flying(u8 *b) {
    ball_fly(b, 1, Thing10_Bounce, 0);
}

/* 0x003338E0 */
void Thing03_StateRolling(u8 *b) {
    ball_roll(b, 0);
}

/* 0x00355220 */
void Thing02_StateRolling(u8 *b) {
    ball_roll(b, 0);
}

/* 0x00334D60 */
void Thing06_Frame(u8 *o) {
    thing_stomped(o, 0x14, 8, Thing06_Burst);
}

/* 0x00336A10 */
void Thing04_Frame(u8 *o) {
    thing_stomped(o, 0xA, 6, Thing04_Burst);
}

/* bursts of four quad drawers (0xFC0 bytes, BurstA_vtable, drawers from +0xB50; 0x22D0 bytes,
   BurstB_vtable, drawers from +0x1A50) */
extern void *BurstA_vtable[], *BurstB_vtable[];

static inline void Burst4_Init(void **obj, void **vtbl, u32 at) {
    s32 i;

    obj[0] = vtbl;
    for (i = 0; i < 4; i++) {
        obj[(at + i * 0x38) / 4] = Helper469D00_vtable;
        ((s32 *)obj)[(at + i * 0x38 + 4) / 4] = -1;
        obj[(at + i * 0x38) / 4] = QuadDrawer_vtable;
    }
}

/* their draws (unless the effects are paused): each drawer's record (+0x10) the current frame's
   (+0xFAC / +0x22BC) in its block, then drawn */

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

static inline void Burst4A_Init(void **obj) {
    Burst4_Init(obj, BurstA_vtable, 0xB50);
}

static inline void Burst4B_Init(void **obj) {
    Burst4_Init(obj, BurstB_vtable, 0x1A50);
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

/* the second stalker (kinds 2, 6, 7, 0x1B) and the thing: when he comes into its room away
   from Fiona it makes a noise (once per room, +0x122) and goes unless he's alerted (+0x16C9 >=
   5). In Fiona's room, when he is searching (+0x16C8 1..3) and can reach it, he notices it
   (a noise; at level 4 unless busy, Pursuer_ChasingHewie, he goes for it and takes it when within 10
   of it or of his path's end) */
/* 0x003332B0 */
void Thing03_StalkerCheck(u8 *o) {
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
    if (!(Npc_SeesPoint((Pursuer *)s, AT(o, 0x34, u32), (f32 *)(o + 0x10)) & 0xFF)) {
        return;
    }
    if (Character_PathLength((Character *)s, AT(o, 0x34, u32), (f32 *)(o + 0x10), -1) <= 0.0f) {
        return;
    }
    if (AT(s, 0x16C9, u8) != 4) {
        Noise_Make((u8 *)gProgress + 0x7A8, 0x1F, AT(o, 0x30, s32), AT(o, 0x34, u32), 0xFFFF);
        return;
    }
    if (Pursuer_ChasingHewie((Pursuer *)s) != 0) {
        return;
    }
    VCALL(s, 0xAC, void (*)(void *, u32, f32 *, s32))(s, AT(o, 0x34, u32), (f32 *)(o + 0x10), -1);
    Pursuer_SetGoalTri((Pursuer *)s, AT(o, 0x34, u32));
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
    Thing03_StalkerCheck(b);
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
    d.vtbl = Helper469D00_vtable;
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
    return VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, u8 *))(gRoomObjects, key);
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
/* 0x00367470 */
void Thing09_StateFalling(u8 *o) {
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    sceVu0AddVector((f32 *)(o + 0xB0), (f32 *)(o + 0x100), (f32 *)(o + 0xB0));
    sceVu0AddVector((f32 *)(o + 0x10), (f32 *)(o + 0xB0), (f32 *)(o + 0x10));
}

/* the same, gone once below -30 */
/* 0x00368150 */
void Thing10_StateFalling(u8 *o) {
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    sceVu0AddVector((f32 *)(o + 0xB0), (f32 *)(o + 0x100), (f32 *)(o + 0xB0));
    sceVu0AddVector((f32 *)(o + 0x10), (f32 *)(o + 0xB0), (f32 *)(o + 0x10));
    if (AT(o, 0x14, f32) < -30.0f) {
        AT(o, 0x28, u8) = 0;
    }
}

/* ---- the shared thing class (ThingShared_vtable, code 0x3544C0..0x355960): who it touches, when it
   goes off, Fiona's kick, its noise ---- */


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
/* 0x003544C0 */
s32 ThingShared_StompStalkers(u8 *o) {
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
/* 0x003546B0 */
void ThingShared_ReachStalkers(u8 *o) {
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
/* 0x00354910 */
s32 ThingShared_Armed(u8 *o) {
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
/* 0x00354AF0 */
void ThingShared_Kick(u8 *o) {
    if (AT(o, 0x30, s32) != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return;
    }
    thing_kick(o);
}

/* its noise (kind +0x12E, loudness +0x126), 0x8000 (startling) by a +0x124 % roll */
/* 0x00354C90 */
void ThingShared_Noise(u8 *o) {
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
/* 0x00354D50 */
s32 ThingShared_PursuerOn(u8 *o) {
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

/* (as ThingShared_Kick) a kind's frame: Fiona's kick with less lift (0.6) */
/* 0x00367BC0 */
void Thing09_Frame(u8 *o) {
    Thing_Frame(o);
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    thing_kick_up(o, 0x1.333334p-1f);
}

/* 1 when the pursuer stands on it (half a unit high) */
/* 0x00367EA0 */
s32 Thing10_PursuerOn(u8 *o) {
    u8 *p = (u8 *)gCharPursuer;

    if (p == NULL || AT(p, 0x28, u8) != 1 || AT(p, 0x29, u8) != 0) {
        return 0;
    }
    if (!thing_spans(AT(p, 0x14, f32), AT(p, 0x14, f32) + AT(p, 0xCC, f32), AT(o, 0x14, f32), 0.5f + AT(o, 0x14, f32))) {
        return 0;
    }
    return thing_near(p, o, AT(p, 0xC8, f32));
}

extern const PTMF D_0042F200, Thing03_StateRolling_ptmf, Thing03_StateDropping_ptmf;

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
        ptmf_set(&AT(b, 0xA0, PTMF), &Thing03_StateRolling_ptmf);
        break;
    default:
        ptmf_set(&AT(b, 0xA0, PTMF), &Thing03_StateDropping_ptmf);
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
    if (Thing10_PursuerOn(o) != 0) {
        Sound_PlayBankAt(gSound, 4, 6, (f32 *)(o + 0x10), 0, 0);
        Progress_Noise(p, (f32 *)(o + 0x10), 4, 2, 0x28, 0, 0.0f);
        AT(o, 0x28, u8) = 0;
    }
}

extern void *ThingPuff_vtable[];

/* its burst (0x7A0 bytes, ThingPuff_vtable, a quad drawer at +0x610) */
static inline void Burst1_Init(void **obj) {
    obj[0] = ThingPuff_vtable;
    obj[0x610 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = QuadDrawer_vtable;
}

/* +0x30 each frame, while the game runs: the shared checks (ThingShared_Armed) set it off - in the
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
    r = ThingShared_Armed(o);
    if (r != 1 && r != 2) {
        ThingShared_Kick(o);
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

extern void *SpriteBurst_vtable[];

/* its puff (0x720 bytes, SpriteBurst_vtable, a quad drawer at +0x610) */
static inline void Puff_Init(void **obj) {
    obj[0] = SpriteBurst_vtable;
    obj[0x610 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = QuadDrawer_vtable;
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
            if (!Thing03_Bounce(b, to, out, n)) {
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

/* ---- the burst ThingPuff_vtable (0x7A0 bytes, Burst1_Init): 16 puffs thrown up from a point and
 * falling back, in two buffers of quad records (+0x10 + 0x300 x the current one +0x790),
 * velocities at +0x648 (12 each), falls at +0x748, start heights at +0x708, the quad drawer at
 * +0x610; +0x794 all gone ---- */
