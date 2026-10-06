/* The placed things (D_0044F260, Progress +0x6FC340, vtable D_0046F5C0): up to 128 actors of
 * 0x140 bytes in a block pool (+0xA040) - the items lying about in the rooms, of kinds 0..10.
 * Each: +0x20 kind, +0x28 active, +0x30 room, +0x34 nav triangle, +0x10 position, +0xE4 age.
 * Kinds 0, 2, 3, 5, 7 and 8 are kept with the save (Progress +0xA14: 60 entries of { kind,
 * room, triangle, x, z, age }); at most 10 of a kind lie about (kind 10: 5), the oldest going
 * when another comes. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "sce/libvu0.h"

extern VObject *D_0044E570;   /* the nav mesh */
extern void *func_00120D60(void *pool, u32 i);   /* BlockPool: block i if in use */
extern void *func_00121370(u32 size, void *place);   /* placement new */
extern void *D_00469C20[];   /* Actor */
extern void *D_0046F5C0[], *D_00469A00[], *D_004699C0[], *D_004699E0[], *D_0046A950[];
extern void *D_0046F520[], *D_004727E0[], *D_00472840[], *D_004758A0[], *D_00475A80[], *D_00475960[],
    *D_00475900[], *D_004759C0[], *D_00475A20[], *D_00479E70[], *D_00479ED0[], *D_00479500[];
extern VObject *D_0044F260;
extern void func_00100490(void *p);   /* operator delete */
extern void func_00121360(void *p);   /* delete (the pool's: nothing) */

#define POOL(m) ((m) + 0xA040)
#define SAVED(p) ((u8 *)(p) + 0xA14)
#define NUM_SAVED 60

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
    u8 *t = func_00120D60(POOL(m), i);

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
        D_0044F260 = NULL;
        if ((s16)flags > 0) {
            func_00100490(m);
        }
    }
    return m;
}

/* +0x8 a new thing of `kind` (0..10) from the pool (NULL: none / full) */
void *func_002D6B00(u8 *m, u32 kind) {
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
    t = func_00121370(0x140, mem);
    if (t != NULL) {
        AT(t, 0x0, void **) = D_00469C20;
        AT(t, 0x20, u32) = kind;
        AT(t, 0x24, u32) = 0x01000000;
        AT(t, 0x0, void **) = sClass[kind];
    }
    return mem;
}

/* the destructor of a thing (D_00479500) */
void *func_002D6F60(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
        }
    }
    return o;
}

/* +0x14 another of `kind` came: when 10 (kind 10: 5) lie about, the oldest of them is aged out
 * (+0xE4 -1) */
void func_002D6FD0(u8 *m, u32 kind) {
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
void func_002D7120(u8 *m) {
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
void func_002D71F0(u8 *m) {
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
void *func_002D7320(u8 *m, u32 kind, s32 from) {
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
void *func_002D73D0(u8 *m, s32 i) {
    u8 *t;

    if (i < 0 || i >= 0x80) {
        return NULL;
    }
    t = func_00120D60(POOL(m), i);
    if (t == NULL || AT(t, 0x28, u8) == 0) {
        return NULL;
    }
    return t;
}

/* +0x24 everything gone */
void func_002D7450(u8 *m) {
    s32 i;

    for (i = 0; i < 0x80; i++) {
        VObject *t = func_00120D60(POOL(m), i);

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
void func_002D69E0(u8 *mgr) {
    s32 room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    VObject *nav = D_0044E570;
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

extern VObject *D_0044E988;   /* the items: +0x8 the list */
extern u32 func_00260CF0(void *list, s32 item);   /* how many */
extern void func_00261090(void *list, s32 item, s32 n);   /* given */
extern VObject *gCharPlayer;
extern const PTMF sGameStateNull;
extern const PTMF D_00414790, D_004147A0, D_004147B0;   /* +0x50 (virtual), func_002D54D0, func_002D5460 */
extern void *D_0046FC30[], *D_00469D00[];
extern void func_002E56C0(u8 *drawer);
extern void func_00121300(u8 *a);   /* Actor +0xC */
extern void func_00121220(u8 *a);   /* Actor +0x30 */
s32 func_00121000(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 r, f32 h);   /* the base's +0x44 */
extern void func_001247E0(u8 *a, const f32 *delta);   /* moved on the nav mesh */
extern void func_00122C20(u8 *a, s32 id, s32 arg2, s32 arg3, s32 arg4, const f32 *pos);   /* a sound */

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
void *func_002D5F40(u8 *b, s32 flags) {
    if (b != NULL) {
        AT(b, 0x0, void **) = D_0046F520;
        if (D_0044E988 != NULL) {
            u8 *items = (u8 *)D_0044E988 + 8;

            if (!(func_00260CF0(items, 0x90) & 0xFF)) {
                func_00261090(items, 0x90, 1);
            }
        }
        AT(b, 0x0, void **) = D_00469A00;
        AT(b, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(b);
        }
    }
    return b;
}

/* +0xC set up: blocked by nav flags 0x20020008, in the current room, white, flying */
void func_002D5ED0(u8 *b) {
    func_00121300(b);
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
void func_002D5130(u8 *b) {
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
    func_001247E0(b, v);
    nm = D_0044E570;
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
    nm = D_0044E570;
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
        func_00122C20(b, 0x7C, 5, 0, 0, NULL);
    }
}

void func_002D56D0(u8 *b) {
    ball_fly(b, 1, func_002D5290, 1);
}

/* +0x4C the motion state for this frame (none outside the current room), and the fade */
void func_002D5A50(u8 *b) {
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
    u32 r = func_00121000(b, tri, pos, rot, rr, h) & 0xFF;

    if (r == 1 && D_0044E988 != NULL) {
        VCALL(D_0044E988, 0x1C, void (*)(VObject *))(D_0044E988);
    }
    return r;
}

/* +0x44 the base's; when it returns 1, the items are told (+0x1C) */
u32 func_002D5C10(u8 *b, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
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
    r = VCALL(D_0044E570, 0x44, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, f32 *, u32))(
        D_0044E570, AT(a, 0x34, u32), out, from, to, n, AT(b, 0xC0, u32));
    if (r & 0xF0000000) {
        return -1;
    }
    return r;
}

/* +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`, -1 when
 * the step is stopped */
s32 func_002D5C70(u8 *b, u8 *a, f32 *to) {
    return thing_step_tri(b, a, to);
}

/* when Fiona touches it (+0x74, within 5) it is kicked the way she faces (2 ahead, 2 up),
 * once per touch */
static inline __attribute__((always_inline)) void thing_kick(u8 *b) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 f[4] __attribute__((aligned(16)));
    VObject *fiona;

    fiona = gCharPlayer;
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
    f[1] = f[1] + 2.0f;
    sceVu0AddVector(BALL_VEL(b), BALL_VEL(b), f);
    AT(b, 0x14, f32) = AT(b, 0x14, f32) + 0x1.99999a0000000p-4f /* 0.1 */;
    AT(b, 0xE0, u8) = 0;
    AT(b, 0xE1, u8) = 1;
}

/* +0x30 each frame: gone after 3 minutes; in Fiona's room, when she touches it (+0x74, within
 * 5) it is kicked the way she faces (2 ahead, 2 up), once per touch */
void func_002D5D10(u8 *b) {
    func_00121220(b);
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

extern void func_00124DB0(void *a);   /* the actor's set-up */
extern VObject *D_0044E4F8, *D_0044E4D0;
extern const PTMF D_003AF1B8;   /* a thing's resting state */
extern void sceVu0RotMatrix(f32 (*out)[4], f32 (*m)[4], const f32 *rot);

/* (possibly unused by the vtables) place it: nav tri, position, turn, front point */
void func_00120F90(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 *front) {
    AT(o, 0x34, u32) = tri;
    sceVu0CopyVector((f32 *)(o + 0x10), pos);
    sceVu0CopyVector((f32 *)(o + 0x50), rot);
    sceVu0CopyVector((f32 *)(o + 0xB0), front);
}

/* +0x48 where it lands on the mesh: none (-1) */
s32 func_00120FF0(void) {
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
s32 func_00121000(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 r, f32 h) {
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
void func_00121100(u8 *o, f32 *pos, f32 *rot, f32 r, f32 h) {
    sceVu0CopyVector((f32 *)(o + 0x10), pos);
    sceVu0CopyVector((f32 *)(o + 0x50), rot);
    AT(o, 0xEC, f32) = r;
    AT(o, 0xF0, f32) = h;
    thing_set_front(o, 3.0f, r);
    AT(o, 0x34, s32) = -1;
}

/* +0x4C to rest (state D_003AF1B8) */
void func_001211B0(u8 *o) {
    ptmf_set(&AT(o, 0xA0, PTMF), &D_003AF1B8);
}

/* +0x30 a frame: older by one while the game runs (not in a cutscene +0x38, not paused by the
 * events +0x50, not progress flag 8; -1 stays), then +0x4C and its state */
void func_00121220(u8 *o) {
    if (VCALL(D_0044E4F8, 0x38, s32 (*)(VObject *))(D_0044E4F8) == 0
        && (VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) & 0xFF) == 0
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
void func_001212F0(void) {
}

/* +0xC set up: the actor's, then sizes 0, its bounce (0, -0.2, 0, 1) at +0x100, not held */
void func_00121300(u8 *o) {
    func_00124DB0(o);
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


/* ---- kind 1 (D_004727E0): like the ball (kind 0) it flies, falls and is kicked, but it can't
 * bounce - whatever it hits, or a pursuer walking into it, bursts it in a purple splash ---- */

extern void *D_004727E0[];
extern const PTMF D_00429C28, D_00429C38;   /* +0x50 (virtual), func_00314CE0 */
extern void func_00177FA0(Progress *p, const f32 *pos, u32 which, u8 kind, s16 a, s16 b, f32 f);
extern VObject *gCharPursuer;

/* +0x8 destructor */
void *func_00314990(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004727E0;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
        }
    }
    return o;
}

/* burst at `at`: a drop splash of colour (r, g, b) and `size`, and its sound */
static inline void drop_splash(u8 *o, f32 *at, s32 r, s32 g, s32 b, f32 size) {
    u8 *mgr = D_0044E578;
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
    func_002D6090(mgr, slot, &sp);
    func_00122C20(o, 0x8D, 5, 0, 0, NULL);
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
void func_00314B90(u8 *o) {
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
void func_00314D40(u8 *o) {
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
    nm = D_0044E570;
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
void func_00314F20(u8 *o) {
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
u32 func_003150A0(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    return thing_put(o, tri, pos, rot, rr, h);
}

/* +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`, -1 when
 * the step is stopped */
s32 func_00315100(u8 *o, u8 *a, f32 *to) {
    return thing_step_tri(o, a, to);
}

/* +0x30 each frame, while the game runs (not in a cutscene, not paused by the events, not
 * progress flags 8 / 0x20) and it is in the current room: a pursuer standing over it (within
 * his radius + 1, his height span +-1) bursts it (an alert to the progress); then Fiona's kick */
static inline __attribute__((always_inline)) void thing_stomped(u8 *o, s16 a, s16 b, void (*burst)(u8 *, f32 *)) {
    Progress *p;
    u8 *c;
    f32 d[4] __attribute__((aligned(16)));

    func_00121220(o);
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    if (VCALL(D_0044E4F8, 0x38, s32 (*)(VObject *))(D_0044E4F8) != 0) {
        return;
    }
    if ((VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) & 0xFF) == 1) {
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
                func_00177FA0(p, BALL_POS(o), 4, 7, a, b, 0.0f);
                burst(o, BALL_POS(o));
                AT(o, 0x28, u8) = 0;
            }
        }
    }
    thing_kick(o);
}

void func_003151A0(u8 *o) {
    thing_stomped(o, 5, 4, func_00314A00);
}

/* +0xC set up: falling (-0.1), blocked by nav flags 0x20020008, in the current room, white */
void func_003154B0(u8 *o) {
    func_00121300(o);
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
extern VObject *D_0044E550;
extern void func_00355940(u8 *o);   /* D_00479500 +0x30 */
extern void func_00355960(u8 *o);   /* D_00479500 +0xC */
extern void func_003546B0(u8 *o);
extern s32 func_00354910(u8 *o);
extern void func_00354C90(u8 *o);
extern s32 func_003544C0(u8 *o);
extern s32 func_00354D50(u8 *o);
extern void func_00354AF0(u8 *o);
extern void func_0033BCB0(void *drawer, f32 *pos, f32 *rot, s32 a, s32 b, s32 c);

/* +0x8 destructor */
void *func_00315540(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00472840;
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
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
    func_0033BCB0(&d, BALL_POS(o), rot, a, b, c);
    d.vtbl = D_00469D00;
}

/* +0x2C draw */
void func_003155C0(u8 *o) {
    turned_model(o, 1, 0, 1);
}

static inline __attribute__((always_inline)) void kind2_burst(u8 *o, u32 size, void (*init)(void **)) {
    u8 *mgr;

    func_00122C20(o, 0x8E, 5, 0, 0, NULL);
    mgr = D_0044E578;
    func_002D6090(mgr, Effect_New(mgr, size, init), o + 0x10);
}

/* +0x30 each frame, while the game runs: the shared checks (func_00354910: 1 / 2 set off -
 * the burst only in the current room; else func_003544C0 / func_00354D50 set it off, or it
 * waits, func_00354AF0) */
static inline __attribute__((always_inline)) void thing_watch(u8 *o, u32 size, void (*init)(void **)) {
    Progress *p;
    s32 r, a, b;

    func_00355940(o);
    if (AT(o, 0x28, u8) == 0) {
        return;
    }
    if (VCALL(D_0044E4F8, 0x38, s32 (*)(VObject *))(D_0044E4F8) != 0) {
        return;
    }
    if ((VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) & 0xFF) == 1) {
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
    func_00122C20(o, 0x8E, 5, 0, 0, NULL);
    if (b != 0) {
        func_00354C90(o);
    }
    {
        u8 *mgr = D_0044E578;

        func_002D6090(mgr, Effect_New(mgr, size, init), o + 0x10);
    }
    AT(o, 0x28, u8) = 0;
}

void func_00315700(u8 *o) {
    thing_watch(o, 0xFD0, ShoveBurst_Init);
}

/* the shared class's set up, its settings (+0x122..+0x12E), a random turn */
static inline void thing_setup(u8 *o, s16 a, s16 b, s16 c, s16 d, s16 e, s16 f, s16 g) {
    func_00355960(o);
    AT(o, 0x122, s16) = a;
    AT(o, 0x124, s16) = b;
    AT(o, 0x126, s16) = c;
    AT(o, 0x128, s16) = d;
    AT(o, 0x12A, s16) = e;
    AT(o, 0x12C, s16) = f;
    AT(o, 0x12E, s16) = g;
    AT(o, 0x132, u16) = VCALL(D_0044E550, 0x10, s32 (*)(VObject *))(D_0044E550) & 0xFF;
}

/* +0xC set up */
void func_00315AB0(u8 *o) {
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

/* (as func_002D6F60)  the destructor of a thing (D_00479E70) */
void *func_003671B0(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479E70;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
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
    func_001247E0(b, v);
    nm = D_0044E570;
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

/* (as func_002D6F60)  the destructor of a thing (D_00479ED0) */
void *func_00367E20(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479ED0;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
        }
    }
    return o;
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
    func_001247E0(b, v);
    nm = D_0044E570;
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

/* (as func_002D5ED0)  +0xC set up: blocked by nav flags 0x20020008, in the current room, white, flying */
void func_00368B40(u8 *b) {
    func_00121300(b);
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

/* (as func_002D6F60)  the destructor of a thing (D_004758A0) */
void *func_00333240(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004758A0;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
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

/* (as func_002D5C10)  +0x44 the base's; when it returns 1, the items are told (+0x1C) */
u32 func_003341D0(u8 *b, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    return thing_put(b, tri, pos, rot, rr, h);
}

/* (as func_002D5C70)  +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`, -1 when
 * the step is stopped */
s32 func_00334230(u8 *b, u8 *a, f32 *to) {
    return thing_step_tri(b, a, to);
}

/* (as func_002D6F60)  the destructor of a thing (D_00475900) */
void *func_00334560(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00475900;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
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

/* (as func_00314D40)  +0x50 the flying state: under gravity, slowed sideways to 0.95, one step along the nav mesh;
 * landing on a floor above its spot's height ends there (still flying), a hole lets it drop,
 * anything else bursts it */
void func_00334900(u8 *o) {
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
    nm = D_0044E570;
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

/* (as func_002D5C10)  +0x44 the base's; when it returns 1, the items are told (+0x1C) */
u32 func_00334C60(u8 *b, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    return thing_put(b, tri, pos, rot, rr, h);
}

/* (as func_002D5C70)  +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`, -1 when
 * the step is stopped */
s32 func_00334CC0(u8 *b, u8 *a, f32 *to) {
    return thing_step_tri(b, a, to);
}

/* (as func_003154B0)  +0xC set up: falling (-0.1), blocked by nav flags 0x20020008, in the current room, white */
void func_00335070(u8 *o) {
    func_00121300(o);
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

/* (as func_00315540)  +0x8 destructor */
void *func_00335100(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00475960;
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
        }
    }
    return o;
}

/* (as func_00315540)  +0x8 destructor */
void *func_003356C0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004759C0;
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
        }
    }
    return o;
}

/* (as func_00315540)  +0x8 destructor */
void *func_00335C80(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00475A20;
        AT(o, 0x0, void **) = D_00479500;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
        }
    }
    return o;
}

/* (as func_002D6F60)  the destructor of a thing (D_00475A80) */
void *func_00336220(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00475A80;
        AT(o, 0x0, void **) = D_00469A00;
        AT(o, 0x0, void **) = D_00469C20;
        if ((s16)flags > 0) {
            func_00121360(o);
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

/* (as func_00314D40)  +0x50 the flying state: under gravity, slowed sideways to 0.95, one step along the nav mesh;
 * landing on a floor above its spot's height ends there (still flying), a hole lets it drop,
 * anything else bursts it */
void func_003365B0(u8 *o) {
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
    nm = D_0044E570;
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

/* (as func_002D5C10)  +0x44 the base's; when it returns 1, the items are told (+0x1C) */
u32 func_00336910(u8 *b, u32 tri, f32 *pos, f32 *rot, f32 rr, f32 h) {
    return thing_put(b, tri, pos, rot, rr, h);
}

/* (as func_002D5C70)  +0x48 the triangle reached stepping from actor a's spot (at height to.y) to `to`, -1 when
 * the step is stopped */
s32 func_00336970(u8 *b, u8 *a, f32 *to) {
    return thing_step_tri(b, a, to);
}

/* (as func_003154B0)  +0xC set up: falling (-0.1), blocked by nav flags 0x20020008, in the current room, white */
void func_00336D20(u8 *o) {
    func_00121300(o);
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

/* (as func_002D5A50)  +0x4C the motion state for this frame (none outside the current room), and the fade */
void func_00355780(u8 *b) {
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

/* (as func_002D5A50)  +0x4C the motion state for this frame (none outside the current room), and the fade */
void func_00367A00(u8 *b) {
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

/* (as func_002D5A50)  +0x4C the motion state for this frame (none outside the current room), and the fade */
void func_00368710(u8 *b) {
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

/* (as func_00314F20)  +0x4C the motion state for this frame (none outside the current room), and the fade */
void func_00334AE0(u8 *o) {
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

/* (as func_00314F20)  +0x4C the motion state for this frame (none outside the current room), and the fade */
void func_00336790(u8 *o) {
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
void func_00355960(u8 *o) {
    func_00121300(o);
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

void func_00335C00(u8 *o) {
    thing_setup(o, 100, 100, 45, 3600, 20, 10, 2);
}

void func_00335640(u8 *o) {
    thing_setup(o, 100, 50, 30, 1800, 15, 10, 2);
}

void func_003361B0(u8 *o) {
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
void func_00334760(u8 *o) {
    half_sprite(o, 128.0f, 128.0f, 128.0f);
}

void func_00336420(u8 *o) {
    half_sprite(o, 128.0f, 100.0f, 0.0f);
}

/* +0x2C draws: its turned model */
void func_00335180(u8 *o) {
    turned_model(o, 1, 1, 1);
}

void func_00335740(u8 *o) {
    turned_model(o, 1, 2, 1);
}

void func_00335D00(u8 *o) {
    turned_model(o, 3, 0, 1);
}

/* ---- more of the same in the other kinds: the flying / rolling states without the active
   check or the bounce sound, the stomped and watched checks with their own bursts ---- */

void func_00355410(u8 *b) {
    ball_fly(b, 0, func_00354FF0, 1);
}

void func_003676C0(u8 *b) {
    ball_fly(b, 1, func_003672A0, 0);
}

void func_003683D0(u8 *b) {
    ball_fly(b, 1, func_00367F80, 0);
}

void func_003338E0(u8 *b) {
    ball_roll(b, 0);
}

void func_00355220(u8 *b) {
    ball_roll(b, 0);
}

void func_00334D60(u8 *o) {
    thing_stomped(o, 0x14, 8, func_003345D0);
}

void func_00336A10(u8 *o) {
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

static inline void Burst4A_Init(void **obj) {
    Burst4_Init(obj, D_0047A010, 0xB50);
}

static inline void Burst4B_Init(void **obj) {
    Burst4_Init(obj, D_0047A030, 0x1A50);
}

void func_00335260(u8 *o) {
    thing_watch(o, 0xFC0, Burst4A_Init);
}

void func_00335820(u8 *o) {
    thing_watch(o, 0x22D0, Burst4B_Init);
}
