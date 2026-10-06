/* The room's effects (made by SceneGame +0xF6CD30 from PAC section 13; 0xA0-byte objects):
 *   D_0046D750 a screen tint, D_0046D7B0 a screen blend, D_0046EB40 the fog (also sets the
 *   camera's depth range), D_0046EC60 a depth range.
 * Vtable: +0xC reset, +0x18 set from the room data (unaligned little-endian words). */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "memcard.h"
#include "ptmf.h"
#include "pursuer.h"
#include "charaction.h"
#include "input.h"
#include "gl2d.h"
#include "effects.h"
#include "fiona.h"
#include "hewie.h"
#include "model.h"
#include "overlay.h"
#include "scene_game_members.h"
#include "skeleton.h"
#include "snd_place.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif
#include "msl.h"

extern void *D_0046D730[];
extern void *D_0046D790[];
extern void *D_0046D7A0[];
extern void *D_0046EB60[];
extern void *D_0046EC60[];
void *func_00267480(u8 *o, s32 flags);
void *func_00269970(u8 *o, s32 flags);
void *func_002BB220(u8 *o, s32 flags);
void *func_002C64E0(u8 *o, s32 flags);

extern u8 D_0041B5F0[];
extern u8 D_00444B10[];
void *Debilitas2_MotionFiles(void);
void *Kind39_MotionFiles(void);

#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

#define B5_PI 0x1.921fb60000000p+1f /* 3.14159274 */ /* 0x40490FDB */

void Effect71000_SetParams(u8 *self, f32 *src);

#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

void FloorSplat_Start(u8 *self);

void Fire_Start(u8 *p);

#define B7_W(p, off)  (*(s32 *)((u8 *)(p) + (off)))

#define B7_H(p, off)  (*(s16 *)((u8 *)(p) + (off)))

#define B7_B(p, off)  (*(u8 *)((u8 *)(p) + (off)))

#define B7_D(p, off)  (*(s64 *)((u8 *)(p) + (off)))

void Wisps_Start(u8 *p);
void FloorGlow_SetParams(u8 *p, s32 *src);
s32 FloorGlow_Update(u8 *p);
void FloorGlow_Start(u8 *p);

extern void *D_00479B20[];
/* Field access by byte offset into objects whose layout is not yet known. */
#define S16(p, off) (*(s16 *)((u8 *)(p) + (off)))

#define S64(p, off) (*(s64 *)((u8 *)(p) + (off)))

void DropletFlash_Start(u8 *self);

extern const char *const D_00405618, *const D_0040561C;   /* "kibako" (the box), "a_koushi" (the grate) */

static f32 shaft_rnd(VObject *rnd) {
    return VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
}

#define HAZE2_REC(o, buf, i) ((o) + (buf) * 0xC00 + (i) * 0x30 + 0x10)

void func_003767C0(u8 *o, s32 i);
void Room49Effect_Start(u8 *o);
s32 Room49Effect_Update(u8 *o);
#ifdef HG_NATIVE
void Room49Effect_Draw(u8 *o);
#endif

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

Character *Kind39_dtor(Character *c, s32 flags);

/* an effect's quad drawer (at `drawer`) given the current one of its records (`size` apart from
 * +0x10, the index at `idx`), and drawn */
static inline __attribute__((always_inline)) void quad_step(u8 *o, u32 drawer, u32 idx, u32 size) {
    AT(o, drawer + 0x10, u8 *) = o + AT(o, idx, s32) * size + 0x10;
    func_002E56C0(o + drawer);
}

void OneDrip_Draw(u8 *o);
void SmokeTrail_Draw(u8 *o);
void SparkSpray_Draw(u8 *o);

static u32 rd32(const u8 *p) {
    return p[0] | (p[1] << 8) | (p[2] << 16) | ((u32)p[3] << 24);
}

/* tint +0xC */
void func_002674F0(u8 *e) {
    AT(e, 0x10, s32) = 0;
    AT(e, 0x14, s32) = 0;
    AT(e, 0x18, s32) = 0;
}

/* tint +0x18: two colours and a mode byte */
void func_00267370(u8 *e, const u8 *d) {
    if (d == NULL) {
        return;
    }
    AT(e, 0x10, u32) = rd32(d);
    AT(e, 0x14, u32) = rd32(d + 4);
    AT(e, 0x18, u32) = d[8];
}

/* blend +0xC */
void func_0026B480(u8 *e) {
    AT(e, 0x10, u32) = 0x80004080;
    AT(e, 0x14, s32) = 1;
    AT(e, 0x30, s32) = 0;
}

/* blend +0x18: colour and mode (mode 2 and 3/4 use fixed colours) */
void func_0026B240(u8 *e, const u8 *d) {
    if (d == NULL) {
        return;
    }
    AT(e, 0x10, u32) = rd32(d);
    AT(e, 0x14, u32) = d[4];
    if (AT(e, 0x14, u32) == 2) {
        AT(e, 0x10, u32) = 0x80004080;
    } else if (AT(e, 0x14, u32) - 3 < 2) {
        AT(e, 0x10, u32) = 0x40404040;
    }
}

/* fog +0xC */
void func_002BB390(u8 *e) {
    s32 i;

    AT(e, 0x10, u32) = 0x808080;
    AT(e, 0x14, u32) = 0xC0808080;
    AT(e, 0x50, f32) = 20.0f;
    AT(e, 0x54, f32) = 1000.0f;
    for (i = 0; i < 7; i++) {
        AT(e, 0x1C + i * 4, s32) = 0;
    }
}

/* fog +0x18: two colours, the depth range (also the camera's), a colour and 6 bytes */
void func_002BB030(u8 *e, const u8 *d) {
    VObject *cam;
    s32 i;

    if (d == NULL) {
        return;
    }
    cam = gCamera;
    AT(e, 0x10, u32) = rd32(d);
    AT(e, 0x14, u32) = rd32(d + 4);
    AT(e, 0x18, u32) = rd32(d + 8);
    AT(e, 0x50, f32) = AT(e, 0x18, f32);
    AT(e, 0x18, u32) = rd32(d + 12);
    AT(e, 0x54, f32) = AT(e, 0x18, f32);
    VCALL(cam, 0xC0, void (*)(VObject *, f32, f32))(cam, AT(e, 0x50, f32), AT(e, 0x54, f32));
    AT(e, 0x1C, u32) = rd32(d + 16);
    for (i = 0; i < 6; i++) {
        AT(e, 0x20 + i * 4, u32) = d[20 + i];
    }
}

/* depth range +0xC */
void func_002C6630(u8 *e) {
    AT(e, 0x50, f32) = 1.0f;
    AT(e, 0x54, f32) = 1.0f;
    AT(e, 0x58, f32) = 2000.0f;
    AT(e, 0x5C, f32) = 2000.0f;
}

/* depth range +0x18 */
void func_002C6540(u8 *e, const f32 *d) {
    if (d == NULL) {
        return;
    }
    AT(e, 0x50, f32) = d[0];
    AT(e, 0x54, f32) = d[1];
    AT(e, 0x58, f32) = d[2];
    AT(e, 0x5C, f32) = d[3];
}

/* +0x10 each frame: the characters this light is on (+0x20 per slot) get its light group
 * (slot << 16 | 0xB); the others it had go back to the default (0xA) */
void func_002BB280(u8 *o) {
    s32 i;

    for (i = 0; i < 6; i++) {
        u32 k = gCutscene != NULL ? (u8)VCALL(gCutscene, 0x80, s32 (*)(VObject *, s32))(gCutscene, i & 0xFF) : (u8)i;
        s32 group = (i << 16) | 0xB;

        if (k == 0xFF || gCharacters[k] == NULL) {
            continue;
        }
        if (AT(o, 0x20 + i * 4, s32) != 0) {
            Character_Set152C(gCharacters[k], group);
        } else if (Character_Get152C(gCharacters[k]) == group) {
            Character_Set152C(gCharacters[k], 0xA);
        }
    }
}

/* +0x10 each frame: a pulsing colour (+0x10 RGBA, direction +0x30): modes 3 / 4 a grey
 * breathing between 0x20 and 0x80, mode 2 red and alpha between 0x80 and 0xC0 */
void func_0026B350(u8 *o) {
    s32 mode = AT(o, 0x14, s32);

    if (mode == 2) {
        if (AT(o, 0x30, s32) == 0) {
            AT(o, 0x10, u32) += 0x10000010;
            if (AT(o, 0x10, u8) >= 0xC0) {
                AT(o, 0x10, u32) = 0xC00040C0;
                AT(o, 0x30, s32) = 1;
            }
        } else {
            AT(o, 0x10, u32) += 0xEFFFFFF0;
            if (AT(o, 0x10, u8) < 0x81) {
                AT(o, 0x10, u32) = 0x80004080;
                AT(o, 0x30, s32) = 0;
            }
        }
    } else if ((u32)(mode - 3) < 2) {
        if (AT(o, 0x30, s32) == 0) {
            AT(o, 0x10, u32) += 0x01010101;
            if (AT(o, 0x10, u8) >= 0x80) {
                AT(o, 0x10, u32) = 0x80808080;
                AT(o, 0x30, s32) = 1;
            }
        } else {
            AT(o, 0x10, u32) += 0xFEFEFEFF;
            if (AT(o, 0x10, u8) < 0x21) {
                AT(o, 0x10, u32) = 0x20202020;
                AT(o, 0x30, s32) = 0;
            }
        }
    }
}

/* +0x10 for effects that don't change */
void func_002674E0(u8 *o) {
}

extern void *D_0046EB60[], *D_00469D00[];
extern void func_002BC000(void *drawer, u32 c0, u32 c1, s32 layer, f32 a, f32 b);

/* +0x14 draw (the fog): a temporary drawer object paints its colours (+0x10, +0x14) over the
 * range +0x50..+0x54 in layer 0x21 (9 while +0x1C) */
void func_002BB1A0(u8 *o) {
    u8 drawer[0x20] __attribute__((aligned(16)));

    AT(drawer, 0x0, void **) = D_0046EB60;
    AT(drawer, 0x4, s32) = -1;
    func_002BC000(drawer, AT(o, 0x10, u32), AT(o, 0x14, u32), AT(o, 0x1C, s32) != 0 ? 9 : 0x21,
                  AT(o, 0x50, f32), AT(o, 0x54, f32));
    AT(drawer, 0x0, void **) = D_00469D00;
}

/* destructor (vtable D_0046EB60) */
void *func_002BB220(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046EB60;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* a fog drawer: colours, range, queued with the renderer in `layer` */
void func_002BC000(void *drawer, u32 c0, u32 c1, s32 layer, f32 a, f32 b) {
    u8 *d = drawer;

    AT(d, 0x8, u32) = c0;
    AT(d, 0xC, u32) = c1;
    AT(d, 0x10, f32) = a;
    AT(d, 0x14, f32) = b;
    VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, d, layer, 0);
}

/* destructor (vtable D_0046EC60) */
void *func_002C64E0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046EC60;
        AT(o, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            RoomEffects_delete(o);
        }
    }
    return o;
}

extern void *D_0046D7A0[];

/* +0x14 draw (a screen bloom): when its colour (+0x10) has alpha, a temporary bloom drawer
 * (Bloom_Start) in layer 0x28, subtracting for modes 1 and 4 (+0x14) */
void func_0026B2C0(u8 *o) {
    u8 drawer[0x20] __attribute__((aligned(16)));

    AT(drawer, 0x0, void **) = D_0046D7A0;
    AT(drawer, 0x4, s32) = -1;
    if (AT(o, 0x10, u32) & 0xFF000000) {
        Bloom_Start(drawer, AT(o, 0x10, u32), 0x28, AT(o, 0x14, s32) == 1 || AT(o, 0x14, s32) == 4);
    }
    AT(drawer, 0x0, void **) = D_00469D00;
}

extern void *D_0046D790[];
extern void func_00269940(void *drawer, u32 rgba, s32 which, s32 arg);

/* +0x14 draw of a two-colour screen effect: a temporary drawer paints each colour (+0x10 first,
 * then +0x14) that has alpha, both with +0x18 */
void func_002673E0(u8 *o) {
    u8 drawer[0x20] __attribute__((aligned(16)));

    AT(drawer, 0x0, void **) = D_0046D790;
    AT(drawer, 0x4, s32) = -1;
    if (AT(o, 0x10, u32) & 0xFF000000) {
        func_00269940(drawer, AT(o, 0x10, u32), 0, AT(o, 0x18, s32));
    }
    if (AT(o, 0x14, u32) & 0xFF000000) {
        func_00269940(drawer, AT(o, 0x14, u32), 1, AT(o, 0x18, s32));
    }
    AT(drawer, 0x0, void **) = D_00469D00;
}

/* destructor (vtable D_0046D790) */
void *func_00267480(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D790;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* set a screen-colour drawer's colour, layer and argument and hand it to the renderer (+0xC,
 * priority 0x20) */
void func_00269940(void *d, u32 rgba, s32 which, s32 arg) {
    AT(d, 0x8, u32) = rgba;
    AT(d, 0xC, s32) = which;
    AT(d, 0x10, s32) = arg;
    VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, d, 0x20, 0);
}

/* destructor (vtable D_0046D7A0) */
void *func_00269970(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D7A0;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* hand a drawer to the renderer (+0xC) in its layer (+0x20); one flagged +0x32 bit 7 then
 * also has the renderer run +0x58 */
void func_002E56C0(u8 *d) {
    VObject *r = gRenderer;

    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, d, AT(d, 0x20, s32), 0);
    if (AT(d, 0x32, u8) & 0x80) {
        VCALL(r, 0x58, void (*)(VObject *))(r);
    }
}

#ifdef HG_NATIVE

#define GLR_PRIM_ADD 0x10000u
#define GLR_PRIM_NOZW 0x20000u
#define GLR_PRIM_GLOW 0x40000u

/* ---- PC: the quad (sprite) drawer with OpenGL - what func_002E4760 / func_002E3500 send ----
 *
 * The drawer: +0x10 the instances (0x30 each: RGBA as 4 x s32 (0x80 = 1.0), position, size x /
 * y, turn about the view axis, frame), +0x14 its own corners (flag 2), +0x18 / +0x1C the
 * corners' offset, +0x24 the instance count, the texture's frame cells +0x26 / +0x28 first
 * cell, +0x2A / +0x2C cell size, +0x2E / +0x30 texture size, +0x33 frames, flags +0x32 (1 stay
 * upright, 2 own corners, 4 turned a quarter about y, 0x40 additive, 0x80 also a glow pass -
 * func_002E3500: each sprite drawn again into the renderer's 128 x 112 glow buffer, against the
 * scene's depth copied down to that size; renderer +0x58 then adds that buffer over the frame), texture id / group +0x34 / +0x35, palette +0x36 (-1: the first;
 * passed to the renderer as TEX0's CSA).
 * Billboards face the camera: x from its up x direction, y the direction x that. */
static s32 gl_sprites(u8 *d, s32 glow) {
    VObject *cam = gCamera;
    const void *tex = VCALL(gTexCache, 0xC, void *(*)(VObject *, s32, s32))(gTexCache, AT(d, 0x34, s8),
                                                                              AT(d, 0x35, s8));
    s16 cells[64][2];
    f32 corner[4][4] __attribute__((aligned(16)));
    f32 basis[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    s32 frames = AT(d, 0x33, s8), n = 0, count = AT(d, 0x24, s16), i, k;
    s16 cw = AT(d, 0x2A, s16), ch = AT(d, 0x2C, s16), tw = AT(d, 0x2E, s16), th = AT(d, 0x30, s16);
    u8 flags = AT(d, 0x32, u8);
    u8 *rec = AT(d, 0x10, u8 *);

    if (tex == NULL || rec == NULL || tw <= 0 || th <= 0) {
        return 0;
    }
    /* the frame cells: along rows from the first cell, wrapping at the texture's width */
    if (frames == 1) {
        cells[0][0] = AT(d, 0x26, s16);
        cells[0][1] = AT(d, 0x28, s16);
        n = 1;
    } else {
        s32 row;

        for (row = 0; n < frames && n < 64 && AT(d, 0x28, s16) + (row + 1) * ch <= th; row++) {
            s32 x = row == 0 ? AT(d, 0x26, s16) : 0;

            for (; n < frames && n < 64 && x + cw <= tw; x += cw) {
                cells[n][0] = x;
                cells[n][1] = AT(d, 0x28, s16) + row * ch;
                n++;
            }
        }
    }
    if (n == 0) {
        return 0;
    }
    if (flags & 2) {
        for (k = 0; k < 4; k++) {
            sceVu0CopyVector(corner[k], AT(d, 0x14, f32 *) + k * 4);
        }
    } else {
        f32 cx = AT(d, 0x18, f32), cy = AT(d, 0x1C, f32);

        for (k = 0; k < 4; k++) {
            corner[k][0] = (k & 1 ? 1.0f : -1.0f) + cx;
            corner[k][1] = (k & 2 ? 1.0f : -1.0f) + cy;
            corner[k][2] = 0.0f;
            corner[k][3] = 1.0f;
        }
    }
    sceVu0UnitMatrix(basis);
    if (!(flags & 2)) {
        VCALL(cam, 0xA0, void (*)(VObject *, f32 *))(cam, basis[2]);
        if (flags & 1) {
            basis[2][1] = 0.0f;
            sceVu0Normalize(basis[2], basis[2]);
        }
        VCALL(cam, 0xA4, void (*)(VObject *, f32 *))(cam, basis[0]);
        sceVu0OuterProduct(basis[0], basis[0], basis[2]);
        sceVu0Normalize(basis[0], basis[0]);
        sceVu0OuterProduct(basis[1], basis[2], basis[0]);
        sceVu0Normalize(basis[1], basis[1]);
        basis[0][3] = basis[1][3] = basis[2][3] = 0.0f;
        if (flags & 4) {
            f32 r[4][4] __attribute__((aligned(16)));

            sceVu0UnitMatrix(r);
            sceVu0RotMatrixY(r, r, 0x1.921fb60000000p+0f /* 1.5707964 */);
            sceVu0MulMatrix(basis, r, basis);
        }
    }
    VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    for (i = 0; i < count; i++, rec += 0x30) {
        f32 m[4][4] __attribute__((aligned(16)));
        f32 xyzw[4][4] __attribute__((aligned(16)));
        f32 st[4][2];
        u8 rgba[4][4];
        s32 f = AT(rec, 0x2C, s32);
        f32 u0, v0, u1, v1;

        if (f < 0 || f >= n) {
            f = 0;
        }
        sceVu0UnitMatrix(m);
        m[0][0] = AT(rec, 0x20, f32);
        m[1][1] = AT(rec, 0x24, f32);
        sceVu0RotMatrixZ(m, m, AT(rec, 0x28, f32));
        sceVu0MulMatrix(m, basis, m);
        sceVu0TransMatrix(m, m, (f32 *)(rec + 0x10));
        u0 = (f32)cells[f][0] / tw;
        v0 = (f32)cells[f][1] / th;
        u1 = (f32)(cells[f][0] + cw) / tw;
        v1 = (f32)(cells[f][1] + ch) / th;
        for (k = 0; k < 4; k++) {
            s32 c;

            sceVu0ApplyMatrix(xyzw[k], m, corner[k]);
            AT(&xyzw[k][3], 0, u32) = 0;
            st[k][0] = k & 1 ? u1 : u0;
            st[k][1] = k & 2 ? v1 : v0;
            for (c = 0; c < 4; c++) {
                s32 v = AT(rec, c * 4, s32);

                rgba[k][c] = v < 0 ? 0 : v > 0xFF ? 0xFF : v;
            }
        }
        glr_strip(&clip[0][0], 4, &xyzw[0][0], &st[0][0], &rgba[0][0], tex,
                  1ull << 34 | (u64)(AT(d, 0x36, s8) == -1 ? 0 : AT(d, 0x36, s8) & 0x1F) << 56,
                  0x10 | 0x40 | GLR_PRIM_NOZW | (flags & 0x40 ? GLR_PRIM_ADD : 0) | (glow ? GLR_PRIM_GLOW : 0));
    }
    return 1;
}

s32 func_002E4760(u8 *d) {
    return gl_sprites(d, 0);
}

s32 func_002E3500(u8 *d) {
    return gl_sprites(d, 1);
}
#else
extern s32 func_002E3500(u8 *d);
extern s32 func_002E4760(u8 *d);
#endif

/* +0xC draw of the quad drawer: flagged ones (+0x32 bit 7) outside layer 0x17 by
 * func_002E3500, the rest by func_002E4760 */
s32 func_002E5660(u8 *d) {
    if ((AT(d, 0x32, u8) & 0x80) && AT(d, 0x20, s32) != 0x17) {
        return (u8)func_002E3500(d);
    }
    return (u8)func_002E4760(d);
}

extern void *D_0046EC60[];      /* the effect 0x1C kind */

/* hold (`on`) or give back screen effect 0x1C (+0x69): held, its colour (+0x50..+0x5C) is
 * kept at +0x154 and the effect removed; given back, a new one (D_0046EC60, from the effects'
 * heap +0x1400, at +0x14A8) is started with that colour */
void func_002241C0(u8 *o, s32 on) {
    u8 *fx;
    u8 *e;
    void *mem;

    AT(o, 0x69, u8) = on;
    if (AT(o, 0x69, u8) == 0) {
        if (AT(o, 0x164, u8) == 0) {
            return;
        }
        AT(o, 0x164, u8) = 0;
        fx = (u8 *)gRoomEffects;
        if (AT(fx, 0x14A8, void *) != NULL) {
            VCALL(fx + 0x1400, 0x14, void (*)(void *, void *))(fx + 0x1400, AT(fx, 0x14A8, void *));
            AT(fx, 0x14A8, void *) = NULL;
        }
        mem = VCALL(fx + 0x1400, 0x10, void *(*)(void *, s32))(fx + 0x1400, 0xA0);
        if (mem != NULL) {
            e = RoomEffects_new(0xA0, mem);
            if (e != NULL) {
                AT(e, 0x0, void **) = D_0046EC60;
            }
            AT(fx, 0x14A8, u8 *) = e;
            VCALL(AT(fx, 0x14A8, u8 *), 0xC, void (*)(u8 *))(AT(fx, 0x14A8, u8 *));
        }
        RoomEffects_Send(fx, 0x1C, o + 0x154);
        return;
    }
    fx = (u8 *)gRoomEffects;
    e = RoomEffects_Get(fx, 0x1C);
    if (e != NULL) {
        AT(o, 0x164, u8) = 1;
        AT(o, 0x154, f32) = AT(e, 0x50, f32);
        AT(o, 0x158, f32) = AT(e, 0x54, f32);
        AT(o, 0x15C, f32) = AT(e, 0x58, f32);
        AT(o, 0x160, f32) = AT(e, 0x5C, f32);
        RoomEffects_Release(fx, 0x1C);
    }
}

#include "progress.h"

/* a screen fade's frame (`kind`: 0 / 2 / 4 in, 1 / 3 / 5 out, 6 / 7 half way in / out): its
 * alpha from the rate (+0x18) times the frames so far (+0x20) - up to 128 (64 for 6 / 7) -
 * also setting the screen fade's level for 0..5; the colour (+0xF8) black with that alpha,
 * white for 2 / 3; then a frame more */
void func_002CF3A0(u8 *f, s32 kind) {
    static const union { u32 u; f32 f; } k0005 = {0x3BA3D70A};
    u8 k = kind;
    f32 a = 0.0f;
    u32 c = 0;

    switch (k) {
    case 0: case 2: case 4:
        a = AT(f, 0x18, f32) * (f32)AT(f, 0x20, u8);
        if (!(a < 128.0f)) {
            a = 128.0f;
        }
        ScreenFade_Level((u8 *)gProgress + 0x7B8, 0.5f - k0005.f * (a - 16.0f));
        break;
    case 1: case 3: case 5:
        a = 128.0f - AT(f, 0x18, f32) * (f32)AT(f, 0x20, u8);
        if (a <= 0.0f) {
            a = 0.0f;
        }
        ScreenFade_Level((u8 *)gProgress + 0x7B8, 1.0f - k0005.f * (a - 16.0f));
        break;
    case 6:
        a = AT(f, 0x18, f32) * (f32)AT(f, 0x20, u8);
        if (!(a < 64.0f)) {
            a = 64.0f;
        }
        break;
    case 7:
        a = 64.0f - AT(f, 0x18, f32) * (f32)AT(f, 0x20, u8);
        if (a <= 0.0f) {
            a = 0.0f;
        }
        break;
    }
    switch (k) {
    case 2: case 3:
        c = (u32)(u8)(s32)a << 24 | 0xFFFFFF;
        break;
    case 0: case 1: case 4: case 5: case 6: case 7:
        c = (u32)(u8)(s32)a << 24;
        break;
    }
    AT(f, 0xF8, u32) = c;
    AT(f, 0x20, u8)++;
}

u32 func_002D6010(u8 *p) { return p[0x19034]; }

/* ---- room effect D_0046FF00 (event command 0x7F): a flickering animated sprite ----
 * +0x10 its quad record { RGBA (4 x s32), position (+0x20), size +0x30 / +0x34, rotation +0x38,
 * frame +0x3C }, +0x40 the quad drawer's settings (texture, layer +0x58, frame strip +0x5C..,
 * frame count +0x6B), +0x70 frame timer, +0x74 rest before the next run, +0x78 slow (long
 * rests) */

extern void *D_0046FF00[], *D_0046D730[], *D_0046FC30[];

/* +0x8 destructor */
u8 *func_002E7BB0(u8 *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x0, void **) = D_0046FF00;
        AT(e, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            RoomEffects_delete(e);
        }
    }
    return e;
}

/* a rest of 0.5 .. 1.2 s (slow: 2.5 .. 4 s) */
static s32 sprite_rest(u8 *e, VObject *rnd) {
    if (AT(e, 0x78, s32) == 0) {
        return (s32)(30.0f * (0.5f + 0x1.6666660000000p-1f /* 0.7 */ * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)));
    }
    return (s32)(30.0f * (2.5f + 1.5f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)));
}

/* +0x18 start at arg's position (its +0x10: slow); NULL: just keep going */
void func_002E7C10(u8 *e, f32 *arg) {
    if (arg == NULL) {
        return;
    }
    sceVu0CopyVector((f32 *)(e + 0x20), arg);
    AT(e, 0x78, s32) = AT(arg, 0x10, s32);
    AT(e, 0x74, s32) = sprite_rest(e, gRandom);
}

/* hand a quad drawer (D_0046FC30) made from an effect's settings (+0x40..+0x6E) to the
 * renderer */
static void fx_quad_submit(u8 *e) {
    struct {
        void **vtbl;
        s32 a;
        u64 tex;
        u8 *rec;
        s32 b;
        f32 c, d;
        s32 layer;
        s16 s[7];
        s8 k[5];
    } q __attribute__((aligned(8)));

    q.a = -1;
    q.vtbl = D_0046FC30;
    q.tex = AT(e, 0x40, u64);
    q.rec = AT(e, 0x48, u8 *);
    q.b = AT(e, 0x4C, s32);
    q.c = AT(e, 0x50, f32);
    q.d = AT(e, 0x54, f32);
    q.layer = AT(e, 0x58, s32);
    q.s[0] = AT(e, 0x5C, s16);
    q.s[1] = AT(e, 0x5E, s16);
    q.s[2] = AT(e, 0x60, s16);
    q.s[3] = AT(e, 0x62, s16);
    q.s[4] = AT(e, 0x64, s16);
    q.s[5] = AT(e, 0x66, s16);
    q.s[6] = AT(e, 0x68, s16);
    q.k[0] = AT(e, 0x6A, s8);
    q.k[1] = AT(e, 0x6B, s8);
    q.k[2] = AT(e, 0x6C, s8);
    q.k[3] = AT(e, 0x6D, s8);
    q.k[4] = AT(e, 0x6E, s8);
    func_002E56C0((u8 *)&q);
    q.vtbl = D_00469D00;
}

/* +0x14 draw, unless resting */
void func_002E7D10(u8 *e) {
    if (AT(e, 0x74, s32) == 0) {
        fx_quad_submit(e);
    }
}

/* +0x10 update: rest, else step the frame each frame; after the last one start over at a new
 * random rotation and rest */
void func_002E7DF0(u8 *e) {
    static const union { u32 u; f32 f; } kTwoPi = {0x40C90FDB};
    VObject *rnd;

    if (AT(e, 0x74, s32) != 0) {
        AT(e, 0x74, s32) -= 1;
        return;
    }
    AT(e, 0x70, s8) -= 1;
    if (AT(e, 0x70, s8) != 0) {
        return;
    }
    AT(e, 0x70, s8) = 1;
    AT(e, 0x3C, s32) += 1;
    if (AT(e, 0x3C, s32) < AT(e, 0x6B, s8)) {
        return;
    }
    AT(e, 0x3C, s32) = 0;
    rnd = gRandom;
    AT(e, 0x38, f32) = kTwoPi.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    AT(e, 0x74, s32) = sprite_rest(e, rnd);
}

/* +0xC set up: grey half-transparent, size 1.6, a random first frame, the 16-frame strip of
 * 32 x 32 cells in layer 0x19 */
void func_002E7F60(u8 *e) {
    AT(e, 0x10, s32) = 0x80;
    AT(e, 0x14, s32) = 0x80;
    AT(e, 0x18, s32) = 0x80;
    AT(e, 0x1C, s32) = 0x40;
    AT(e, 0x20, f32) = 0.0f;
    AT(e, 0x24, f32) = 0.0f;
    AT(e, 0x28, f32) = 0.0f;
    AT(e, 0x2C, f32) = 1.0f;
    AT(e, 0x30, f32) = 0x1.99999a0000000p+0f /* 1.6 */;
    AT(e, 0x34, f32) = 0x1.99999a0000000p+0f /* 1.6 */;
    AT(e, 0x38, f32) = 0.0f;
    AT(e, 0x3C, s32) = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 0xF;
    AT(e, 0x40, s64) = -1;
    AT(e, 0x48, u8 *) = e + 0x10;
    AT(e, 0x4C, s32) = 0;
    AT(e, 0x50, s32) = 0;
    AT(e, 0x54, s32) = 0;
    AT(e, 0x58, s32) = 0x19;
    AT(e, 0x5C, s16) = 1;
    AT(e, 0x5E, s16) = 0;
    AT(e, 0x60, s16) = 0;
    AT(e, 0x62, s16) = 0x20;
    AT(e, 0x64, s16) = 0x20;
    AT(e, 0x66, s16) = 0x200;
    AT(e, 0x68, s16) = 0x100;
    AT(e, 0x6A, s8) = 0;
    AT(e, 0x6B, s8) = 0x10;
    AT(e, 0x6C, s8) = 1;
    AT(e, 0x6D, s8) = 0x10;
    AT(e, 0x6E, s8) = -1;
    AT(e, 0x70, s8) = 1;
    AT(e, 0x74, s32) = 0;
}

/* ---- room effect D_0046FF20 (0x718 bytes): a burst of 16 sprites in two buffers of quad
 * records (+0x10 + 0x300 x the current one +0x710), their velocities at +0x648 (12 each),
 * drawn by the quad drawer at +0x610. Kinds (+0x714): 1 swirling motes that brighten until
 * their alpha reaches +0x70C and then fade (red one below +0x708 marks the fading), 2
 * spreading dust, 3 a rising puff (dark, or light when +0x708 is set), others drifting
 * dust ---- */

#include "effectmgr.h"

extern void *D_0046FF20[], *D_0046F580[];

#define BURST_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x300) + (i))
#define BURST_VEL(e, i) ((f32 *)((e) + 0x648) + (i) * 3)

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x002E8060 */
u8 *SpriteBurst_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0046FF20;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

static f32 burst_rnd(VObject *rnd) {
    return VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
}

static s32 burst_int(VObject *rnd) {
    return VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd);
}

/* +0x18 start: arg { position, +0x10 kind, +0x14 RGB (kind 3: light), +0x20 the alpha /
 * spread } */
/* 0x002E80F0 */
void SpriteBurst_SetParams(u8 *e, u8 *arg) {
    VObject *rnd;
    QuadRec *r;
    f32 *v;
    s32 i;

    AT(e, 0x714, s32) = AT(arg, 0x10, s32);
    AT(e, 0x708, s32) = AT(arg, 0x14, s32);
    AT(e, 0x70C, s32) = AT(arg, 0x20, s32);
    switch (AT(e, 0x714, s32)) {
    case 1:
        rnd = gRandom;
        for (i = 0; i < 16; i++) {
            f32 dx, dz, s;

            r = BURST_REC(e, AT(e, 0x710, s32), i);
            v = BURST_VEL(e, i);
            r->rgba[0] = AT(arg, 0x14, s32);
            r->rgba[1] = AT(arg, 0x18, s32);
            r->rgba[2] = AT(arg, 0x1C, s32);
            r->rgba[3] = 1;
            dx = burst_rnd(rnd) - 0.5f;
            dz = burst_rnd(rnd) - 0.5f;
            r->pos[0] = AT(arg, 0x0, f32) + 2.0f * dx;
            r->pos[1] = AT(arg, 0x4, f32);
            r->pos[2] = AT(arg, 0x8, f32) + 2.0f * dz;
            r->pos[3] = 1.0f;
            s = 2.0f + 2.0f * burst_rnd(rnd);
            r->w = s;
            r->h = s;
            r->turn = 0x1.921fb60000000p+1f /* 3.14159265 */ * (360.0f * (burst_rnd(rnd) - 0.5f)) / 180.0f;
            r->frame = 0;
            v[0] = 0x1.99999a0000000p-5f /* 0.05 */ * dx;
            v[1] = 0x1.47ae140000000p-7f /* 0.01 */;
            v[2] = 0x1.99999a0000000p-5f /* 0.05 */ * dz;
        }
        break;
    case 2: {
        f32 spread = 0x1.47ae140000000p-7f /* 0.01 */ * (f32)AT(e, 0x70C, s32);

        rnd = gRandom;
        for (i = 0; i < 16; i++) {
            f32 dx, dy, dz;

            r = BURST_REC(e, AT(e, 0x710, s32), i);
            v = BURST_VEL(e, i);
            r->rgba[0] = AT(arg, 0x14, s32);
            r->rgba[1] = AT(arg, 0x18, s32);
            r->rgba[2] = AT(arg, 0x1C, s32);
            r->rgba[3] = (burst_int(rnd) & 0x3F) + 0x10;
            dx = burst_rnd(rnd) - 0.5f;
            dy = burst_rnd(rnd) - 0.5f;
            dz = burst_rnd(rnd) - 0.5f;
            r->pos[0] = AT(arg, 0x0, f32) + 4.0f * dx;
            r->pos[1] = AT(arg, 0x4, f32) + dy;
            r->pos[2] = AT(arg, 0x8, f32) + 4.0f * dz;
            r->pos[3] = 1.0f;
            r->w = 2.0f;
            r->h = 2.0f;
            r->turn = 0.0f;
            r->frame = 0;
            v[0] = spread * dx;
            v[1] = 0x1.47ae140000000p-7f /* 0.01 */;
            v[2] = spread * dz;
        }
        break;
    }
    case 3: {
        s32 alpha = 0x10;
        f32 tall, big;

        AT(e, 0x618, s64) = -1;
        AT(e, 0x628, s32) = 0;
        AT(e, 0x62C, s32) = 0;
        AT(e, 0x630, s32) = 0x19;
        AT(e, 0x634, s16) = 0x10;
        AT(e, 0x636, s16) = 0xC0;
        AT(e, 0x638, s16) = 0x40;
        AT(e, 0x63A, s16) = 0x20;
        AT(e, 0x63C, s16) = 0x20;
        AT(e, 0x63E, s16) = 0x200;
        AT(e, 0x640, s16) = 0x100;
        AT(e, 0x643, s8) = 1;
        AT(e, 0x644, s8) = 1;
        AT(e, 0x645, s8) = 0x10;
        AT(e, 0x646, s8) = 3;
        if (AT(arg, 0x14, s32) == 0) {
            big = 3.0f;
            tall = 2.0f;
            AT(e, 0x642, s8) = 0x20;
        } else {
            AT(e, 0x642, s8) = 0;
            big = 2.0f;
            alpha = 0x80;
            tall = 1.0f;
        }
        rnd = gRandom;
        for (i = 0; i < 16; i++) {
            f32 dx, up, dz, s;

            r = BURST_REC(e, AT(e, 0x710, s32), i);
            v = BURST_VEL(e, i);
            r->rgba[0] = 0x10;
            r->rgba[1] = 0x10;
            r->rgba[2] = 0x10;
            r->rgba[3] = alpha;
            dx = burst_rnd(rnd) - 0.5f;
            up = burst_rnd(rnd);
            dz = burst_rnd(rnd) - 0.5f;
            r->pos[0] = AT(arg, 0x0, f32) + 6.0f * dx;
            r->pos[1] = AT(arg, 0x4, f32) + 10.0f * up * tall;
            r->pos[2] = AT(arg, 0x8, f32) + 6.0f * dz;
            r->pos[3] = 1.0f;
            s = burst_rnd(rnd);
            s = big + big * s;
            r->w = s;
            r->h = s;
            r->turn = 0.0f;
            r->frame = 0;
            v[0] = 0x1.99999a0000000p-5f /* 0.05 */ * dx;
            v[1] = 0x1.99999a0000000p-4f /* 0.1 */ + 0x1.3333340000000p-2f /* 0.3 */ * up;
            v[2] = 0x1.99999a0000000p-5f /* 0.05 */ * dz;
        }
        break;
    }
    default:
        rnd = gRandom;
        for (i = 0; i < 16; i++) {
            f32 dx, dy, dz;

            r = BURST_REC(e, AT(e, 0x710, s32), i);
            v = BURST_VEL(e, i);
            r->rgba[0] = AT(arg, 0x14, s32);
            r->rgba[1] = AT(arg, 0x18, s32);
            r->rgba[2] = AT(arg, 0x1C, s32);
            r->rgba[3] = (burst_int(rnd) & 0x3F) + 0x10;
            dx = burst_rnd(rnd) - 0.5f;
            dy = burst_rnd(rnd) - 0.5f;
            dz = burst_rnd(rnd) - 0.5f;
            r->pos[0] = AT(arg, 0x0, f32) + 4.0f * dx;
            r->pos[1] = AT(arg, 0x4, f32) + 4.0f * dy;
            r->pos[2] = AT(arg, 0x8, f32) + 4.0f * dz;
            r->pos[3] = 1.0f;
            r->w = 2.0f;
            r->h = 2.0f;
            r->turn = 0.0f;
            r->frame = 0;
            v[0] = 0x1.47ae140000000p-7f /* 0.01 */ * dx;
            v[1] = 0x1.47ae140000000p-7f /* 0.01 */ + 0x1.99999a0000000p-4f /* 0.1 */ * (0.5f + dy);
            v[2] = 0x1.47ae140000000p-7f /* 0.01 */ * dz;
        }
        break;
    }
}

/* +0x14 draw the current buffer */
/* 0x002E8800 */
void SpriteBurst_Draw(u8 *e) {
    AT(e, 0x620, QuadRec *) = BURST_REC(e, AT(e, 0x710, s32), 0);
    func_002E56C0(e + 0x610);
}

/* the turn by a half degree either way, kept to -pi..pi (the backwards turn as the original:
 * below pi it goes up a full turn) */
static void burst_turn(QuadRec *r, s32 back) {
    if (back) {
        r->turn = r->turn - 0x1.1df46ap-7f /* pi / 360 */;
        if (r->turn < 0x1.921fb6p+1f) {
            r->turn = r->turn + 0x1.921fb6p+2f;
        }
    } else {
        r->turn = r->turn + 0x1.1df46ap-7f;
        if (!(r->turn <= 0x1.921fb6p+1f)) {
            r->turn = r->turn - 0x1.921fb6p+2f;
        }
    }
}

static void burst_move(QuadRec *r, f32 *v) {
    r->pos[0] = r->pos[0] + v[0];
    r->pos[1] = r->pos[1] + v[1];
    r->pos[2] = r->pos[2] + v[2];
}

static void burst_fade(QuadRec *r, VObject *rnd, s32 extra) {
    r->rgba[3] = r->rgba[3] - ((burst_int(rnd) & 3) + extra);
    if (r->rgba[3] < 0) {
        r->rgba[3] = 0;
    }
}

/* +0x10 update: flip the buffers, carrying each sprite over and moving it; 0 when all have
 * gone */
/* 0x002E8830 */
s32 SpriteBurst_Update(u8 *e) {
    VObject *rnd = gRandom;
    s32 done = 1;
    s32 i;

    AT(e, 0x710, s32) ^= 1;
    for (i = 0; i < 16; i++) {
        u32 *src = (u32 *)BURST_REC(e, AT(e, 0x710, s32) ^ 1, i);
        u32 *dst = (u32 *)BURST_REC(e, AT(e, 0x710, s32), i);
        QuadRec *r;
        f32 *v = BURST_VEL(e, i);
        s32 k;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = BURST_REC(e, AT(e, 0x710, s32), i);
        switch (AT(e, 0x714, s32)) {
        case 1:
            if (r->w <= 0.0f || r->rgba[3] <= 0) {
                break;
            }
            done = 0;
            r->w = r->w - 0x1.47ae140000000p-8f /* 0.005 */ * burst_rnd(rnd);
            if (r->w < 0.0f) {
                r->w = 0.0f;
            }
            r->h = r->w;
            r->turn = r->turn + 0x1.921fb6p+1f * (0.5f * burst_rnd(rnd)) / 180.0f;
            if (!(r->turn <= 0x1.921fb6p+1f)) {
                r->turn = r->turn - 0x1.921fb6p+2f;
            }
            burst_move(r, v);
            if (r->rgba[0] == AT(e, 0x708, s32)) {
                r->rgba[3] += (burst_int(rnd) & 1) + 1;
                if (!(r->rgba[3] < AT(e, 0x70C, s32))) {
                    r->rgba[0] -= 1;
                    r->rgba[3] = AT(e, 0x70C, s32);
                }
            } else {
                r->rgba[3] -= burst_int(rnd) & 1;
                if (r->rgba[3] < 0) {
                    r->rgba[3] = 0;
                }
            }
            break;
        case 2:
            if (r->rgba[3] <= 0) {
                break;
            }
            r->w = r->w + 0x1.99999a0000000p-5f /* 0.05 */;
            r->h = r->w;
            done = 0;
            burst_turn(r, v[0] <= 0.0f);
            v[0] = v[0] * 0x1.e666660000000p-1f /* 0.95 */;
            v[2] = v[2] * 0x1.e666660000000p-1f /* 0.95 */;
            burst_move(r, v);
            burst_fade(r, rnd, 1);
            break;
        case 3:
            if (!(r->pos[1] < 80.0f)) {
                break;
            }
            done = 0;
            r->w = r->w + 0x1.47ae140000000p-5f /* 0.04 */;
            r->h = r->w;
            burst_turn(r, 0);
            burst_move(r, v);
            burst_fade(r, rnd, 0);
            break;
        default:
            if (r->rgba[3] <= 0) {
                break;
            }
            done = 0;
            r->w = r->w + 0x1.47ae140000000p-5f /* 0.04 */;
            r->h = r->w;
            burst_turn(r, 0);
            burst_move(r, v);
            burst_fade(r, rnd, 0);
            break;
        }
    }
    return !done;
}

/* +0xC set up: the drawer's settings (a 16-frame strip of 32 x 32 cells at a random column
 * 0 / 32, row 64) */
/* 0x002E9040 */
void SpriteBurst_Start(u8 *e) {
    AT(e, 0x710, s32) = 0;
    AT(e, 0x618, s64) = -1;
    AT(e, 0x628, s32) = 0;
    AT(e, 0x62C, s32) = 0;
    AT(e, 0x630, s32) = 0x19;
    AT(e, 0x634, s16) = 0x10;
    AT(e, 0x636, s16) = (burst_int(gRandom) & 1) << 5;
    AT(e, 0x638, s16) = 0x40;
    AT(e, 0x63A, s16) = 0x20;
    AT(e, 0x63C, s16) = 0x20;
    AT(e, 0x63E, s16) = 0x200;
    AT(e, 0x640, s16) = 0x100;
    AT(e, 0x642, s8) = 0;
    AT(e, 0x643, s8) = 1;
    AT(e, 0x644, s8) = 1;
    AT(e, 0x645, s8) = 0x10;
    AT(e, 0x646, s8) = -1;
}

/* ---- room effect D_0046FF40 (event command 0x86): a flame; kind (+0x78) 0 a candle (32
 * frames: a loop, then a flare / snuff played on command), others a 16-frame fire (kind 4 is
 * kind 1 drawn differently, kind 3 another blend) of which kind 1 throws a spark (D_00479320)
 * above it; commanded (+0x74 1 / 2) the fire throws two / four sparks and the candle plays its
 * second half (2: shrinking away) ----
 * the record and drawer settings as D_0046FF00's */

extern void *D_0046FF40[], *D_00479320[];
#include "effectmgr.h"

/* +0x8 destructor */
u8 *func_002E90F0(u8 *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x0, void **) = D_0046FF40;
        AT(e, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            RoomEffects_delete(e);
        }
    }
    return e;
}

/* a spark: its quad drawer embedded at +0x190 */
static void spark_init(void **obj) {
    obj[0] = D_00479320;
    obj[0x190 / 4] = D_00469D00;
    ((s32 *)obj)[0x194 / 4] = -1;
    obj[0x190 / 4] = D_0046FC30;
}

/* +0x18 start (arg { position, +0x10 the command, +0x14 the kind }) */
void func_002E9150(u8 *e, u8 *arg) {
    u8 *mgr;
    s32 slot;

    if (arg == NULL) {
        return;
    }
    AT(e, 0x74, s32) = AT(arg, 0x10, s32);
    if (AT(e, 0x74, s32) != 0) {
        struct {
            f32 pos[4];
            s32 kind;
        } p __attribute__((aligned(16)));

        if (AT(e, 0x78, s32) == 0) {
            AT(e, 0x3C, s32) = 15;
            return;
        }
        if (AT(e, 0x78, s32) != 1) {
            return;
        }
        p.pos[0] = AT(e, 0x20, f32);
        p.pos[1] = 1.5f + AT(e, 0x24, f32);
        p.pos[2] = AT(e, 0x28, f32);
        p.pos[3] = 1.0f;
        p.kind = 1;
        mgr = gEffects;
        slot = Effect_New(mgr, 0x220, spark_init);
        EffectMgr_Start(mgr, slot, &p);
        slot = Effect_New(mgr, 0x220, spark_init);
        EffectMgr_Start(mgr, slot, &p);
        if (AT(e, 0x74, s32) != 2) {
            return;
        }
        slot = Effect_New(mgr, 0x220, spark_init);
        EffectMgr_Start(mgr, slot, &p);
        slot = Effect_New(mgr, 0x220, spark_init);
        EffectMgr_Start(mgr, slot, &p);
        return;
    }
    AT(e, 0x20, f32) = AT(arg, 0x0, f32);
    AT(e, 0x24, f32) = AT(arg, 0x4, f32);
    AT(e, 0x28, f32) = AT(arg, 0x8, f32);
    AT(e, 0x2C, f32) = 1.0f;
    AT(e, 0x78, s32) = AT(arg, 0x14, s32);
    AT(e, 0x10, s32) = 0x80;
    AT(e, 0x14, s32) = 0x80;
    AT(e, 0x18, s32) = 0x80;
    AT(e, 0x1C, s32) = 0x80;
    AT(e, 0x20, f32) = AT(arg, 0x0, f32);
    AT(e, 0x24, f32) = AT(arg, 0x4, f32);
    AT(e, 0x28, f32) = AT(arg, 0x8, f32);
    AT(e, 0x2C, f32) = 1.0f;
    if (AT(e, 0x78, s32) != 0) {
        struct {
            f32 pos[4];
            s32 flags;
        } p __attribute__((aligned(16)));

        AT(e, 0x30, f32) = 1.0f;
        AT(e, 0x34, f32) = 2.0f;
        AT(e, 0x38, f32) = 0.0f;
        AT(e, 0x3C, s32) = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 0xF;
        AT(e, 0x40, s64) = -1;
        AT(e, 0x48, u8 *) = e + 0x10;
        AT(e, 0x4C, s32) = 0;
        AT(e, 0x50, s32) = 0;
        AT(e, 0x54, f32) = -1.0f;
        AT(e, 0x58, s32) = 0x19;
        AT(e, 0x5C, s16) = 1;
        AT(e, 0x5E, s16) = 0;
        AT(e, 0x60, s16) = 0x80;
        AT(e, 0x62, s16) = 0x10;
        AT(e, 0x64, s16) = 0x20;
        AT(e, 0x66, s16) = 0x200;
        AT(e, 0x68, s16) = 0x100;
        if (AT(e, 0x78, s32) == 4) {
            AT(e, 0x6A, s8) = -0x7B;
            AT(e, 0x78, s32) = 1;
        } else {
            AT(e, 0x6A, s8) = -0x7F;
        }
        AT(e, 0x6B, s8) = 0x10;
        AT(e, 0x6C, s8) = 1;
        AT(e, 0x6D, s8) = 0x10;
        AT(e, 0x6E, s8) = (AT(e, 0x78, s32) == 3) + 4;
        if (AT(e, 0x78, s32) != 1) {
            return;
        }
        mgr = gEffects;
        slot = Effect_New(mgr, 0x220, spark_init);
        p.pos[0] = AT(e, 0x20, f32);
        p.pos[1] = 2.0f + AT(e, 0x24, f32);
        p.pos[2] = AT(e, 0x28, f32);
        p.pos[3] = 1.0f;
        p.flags = 0x8000;
        EffectMgr_Start(mgr, slot, &p);
        return;
    }
    AT(e, 0x30, f32) = 0.25f;
    AT(e, 0x34, f32) = 1.0f;
    AT(e, 0x38, f32) = 0.0f;
    AT(e, 0x3C, s32) = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 0xF;
    AT(e, 0x40, s64) = -1;
    AT(e, 0x48, u8 *) = e + 0x10;
    AT(e, 0x4C, s32) = 0;
    AT(e, 0x50, s32) = 0;
    AT(e, 0x54, f32) = -1.0f;
    AT(e, 0x58, s32) = 0x19;
    AT(e, 0x5C, s16) = 1;
    AT(e, 0x5E, s16) = 0;
    AT(e, 0x60, s16) = 0x20;
    AT(e, 0x62, s16) = 0x10;
    AT(e, 0x64, s16) = 0x20;
    AT(e, 0x66, s16) = 0x200;
    AT(e, 0x68, s16) = 0x100;
    AT(e, 0x6A, s8) = -0x7F;
    AT(e, 0x6B, s8) = 0x20;
    AT(e, 0x6C, s8) = 1;
    AT(e, 0x6D, s8) = 0x10;
    AT(e, 0x6E, s8) = 1;
}

/* +0x14 draw */
void func_002E98E0(u8 *e) {
    fx_quad_submit(e);
}

/* +0x10 update: kinds other than 0 loop their 16 frames; kind 0 (a candle) loops the first
 * half of its 32 frames, and once started (+0x74 1 / 2) plays the second half at half speed
 * and goes back to looping, +0x74 2 shrinking it as it goes (height 1/8 per 4 frames past 12) */
void func_002E99B0(u8 *e) {
    if (AT(e, 0x78, s32) != 0) {
        AT(e, 0x70, s8) -= 1;
        if (AT(e, 0x70, s8) != 0) {
            return;
        }
        AT(e, 0x70, s8) = 1;
        AT(e, 0x3C, s32) += 1;
        if (!(AT(e, 0x3C, s32) < 0x10)) {
            AT(e, 0x3C, s32) = 0;
        }
        return;
    }
    AT(e, 0x70, s8) -= 1;
    if (AT(e, 0x70, s8) == 0) {
        AT(e, 0x70, s8) = 1;
        AT(e, 0x3C, s32) += 1;
        if (AT(e, 0x74, s32) == 0) {
            if (!(AT(e, 0x3C, s32) < AT(e, 0x6B, s8) - 0x10)) {
                AT(e, 0x3C, s32) = 0;
            }
        } else {
            AT(e, 0x70, s8) = 2;
            if (!(AT(e, 0x3C, s32) < AT(e, 0x6B, s8))) {
                AT(e, 0x3C, s32) = 0;
                AT(e, 0x70, s8) = 1;
                AT(e, 0x74, s32) = 0;
            }
        }
    }
    if (AT(e, 0x74, s32) == 2) {
        AT(e, 0x34, f32) = 0.125f * (f32)(((AT(e, 0x3C, s32) - 12) >> 2) + AT(e, 0x70, s8));
    } else {
        AT(e, 0x34, f32) = 1.0f;
    }
}

/* +0xC set up: frame timer 1, no slot (+0x7C) */
void func_002E9AE0(u8 *e) {
    AT(e, 0x70, s8) = 1;
    AT(e, 0x7C, s32) = -1;
}

/* D_0046EC60 +0x10: nothing */
void func_002C6620(void) {
}

extern void *D_0046EC80[];

/* hand a depth-band drawer (`d`: +0x8 .. +0x14 a, from, to, b) to the renderer (layer 0x21)
 * unless the band covers all of the camera's depth range (+0xCC near, +0xD0 far) */
void func_002C86F0(u8 *d, f32 a, f32 from, f32 to, f32 b) {
    VObject *cam = gCamera;

    if (from <= VCALL(cam, 0xCC, f32 (*)(VObject *))(cam) && !(to < VCALL(cam, 0xD0, f32 (*)(VObject *))(cam))) {
        return;
    }
    AT(d, 0x8, f32) = a;
    AT(d, 0xC, f32) = from;
    AT(d, 0x10, f32) = to;
    AT(d, 0x14, f32) = b;
    VCALL(gRenderer, 0xC, void (*)(VObject *, u8 *, s32, s32))(gRenderer, d, 0x21, 0);
}

/* D_0046EC60 +0x14 draw: its band (+0x50 .. +0x5C) through a D_0046EC80 drawer */
void func_002C6570(u8 *e) {
    struct {
        void **vtbl;
        s32 a;
        f32 v[4];
    } d;

    d.a = -1;
    d.vtbl = D_0046EC80;
    func_002C86F0((u8 *)&d, AT(e, 0x50, f32), AT(e, 0x54, f32), AT(e, 0x58, f32), AT(e, 0x5C, f32));
    d.vtbl = D_00469D00;
}

/* D_00472F60 +0xC set up: nothing */
void func_00319B10(void) {
}

/* ---- room effect D_00472F60 (room 0x60's floor effect 0x1B): a reflecting floor quad
 * (corners +0x50..+0x8C) of strength +0x14 (0 off); +0x10 the reflection covers the screen
 * rather than the quad, +0x18 it mirrors top to bottom rather than left to right. Its draw
 * (+0x14, func_00317D40) renders the characters and creatures above it again from the
 * camera reflected in its plane ---- */

/* `pos` is in view (clip space) and, unless the reflection covers the screen (+0x10), its
 * screen position lies within the quad's screen bounds widened by mx / my (my shifted by half
 * when the quad mirrors top to bottom, +0x18) */
static s32 refl_near_quad(u8 *e, f32 *pos, f32 mx, f32 my) {
    VObject *cam = gCamera;
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 x0 = 0.0f, x1 = 0.0f, y0 = 0.0f, y1 = 0.0f;
    s32 i;

    VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    sceVu0ApplyMatrix(v, clip, pos);
    if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
        return 0;
    }
    if (AT(e, 0x10, s32) != 0) {
        return 1;
    }
    VCALL(cam, 0x44, void (*)(VObject *, f32 (*)[4]))(cam, m);
    for (i = 0; i < 4; i++) {
        sceVu0ApplyMatrix(v, m, (f32 *)(e + 0x50) + i * 4);
        v[3] = 1.0f / v[3];
        v[0] *= v[3];
        v[1] *= v[3];
        if (i == 0) {
            x0 = x1 = v[0];
            y0 = y1 = v[1];
            continue;
        }
        if (!(x0 <= v[0])) {
            x0 = v[0];
        } else if (x1 < v[0]) {
            x1 = v[0];
        }
        if (!(y0 <= v[1])) {
            y0 = v[1];
        } else if (y1 < v[1]) {
            y1 = v[1];
        }
    }
    sceVu0ApplyMatrix(v, m, pos);
    v[3] = 1.0f / v[3];
    v[0] *= v[3];
    v[1] *= v[3];
    if (AT(e, 0x18, s32) != 0) {
        v[1] = v[1] + 0.5f * my;
    }
    return x0 <= v[0] + mx && !(x1 < v[0] - mx) && y0 <= v[1] + my && !(y1 < v[1] - my);
}

/* character slot `i` (shown, +0x28 set, +0x29 not 1) is to be reflected: its root bone near the
 * quad (margins: Fiona 10 x 30, Hewie 20 x 15, others 20 x 30) */
s32 func_00317920(u8 *e, s32 i) {
    u8 *c = (u8 *)gCharacters[i];
    f32 pos[4] __attribute__((aligned(16)));

    if (c == NULL || AT(c, 0x28, u8) == 0 || AT(c, 0x29, u8) == 1) {
        return 0;
    }
    sceVu0CopyVector(pos, Skel_Bone(AT(AT(c, 0xF0, u8 *), 0x810, void *), 0) + 12);
    return refl_near_quad(e, pos, i == 0 ? 10.0f : 20.0f, i == 1 ? 15.0f : 30.0f);
}

/* creature `i` (shown) is to be reflected, unless Fiona's +0xE2 is set: creatures 7.. by their
 * model's root bone (margins 5 x 10), the rest by their position 10 up (5 x 15) */
s32 func_003175B0(u8 *e, s32 i) {
    u8 *o = ((u8 **)gCreatures)[i];
    f32 pos[4] __attribute__((aligned(16)));
    f32 my;

    if (o == NULL || AT(o, 0x28, u8) == 0 || AT(o, 0x29, u8) == 1 || AT(gCharPlayer, 0xE2, u8) != 0) {
        return 0;
    }
    if (i >= 7) {
        sceVu0CopyVector(pos, Skel_Bone(AT(AT(o, 0xF0, u8 *), 0x810, void *), 0) + 12);
        my = 10.0f;
    } else {
        sceVu0CopyVector(pos, (f32 *)(o + 0x10));
        my = 15.0f;
        pos[1] += 10.0f;
    }
    return refl_near_quad(e, pos, 5.0f, my);
}

#ifdef HG_NATIVE
#define GLR_PRIM_MASK 0x100000u

typedef struct ReflCamera {   /* camera +0x88's set (CameraSet) */
    f32 eye[4];
    f32 target[4];
    f32 fov;
    s32 unk24;
} ReflCamera;

/* mark the reflection's mask with quad `q` (4 corners, strip order) where it is in front */
static void refl_mask_quad(const f32 *q) {
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 xyzw[4][4] __attribute__((aligned(16)));
    f32 st[4][2] = {{0}};
    u32 rgba[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 k;

    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
    for (k = 0; k < 4; k++) {
        sceVu0CopyVector(xyzw[k], (f32 *)q + k * 4);
        AT(&xyzw[k][3], 0, u32) = k < 2 ? 0x8000 : 0;
    }
    glr_mask_clear();
    glr_strip(&clip[0][0], 4, &xyzw[0][0], &st[0][0], (const u8 *)rgba, NULL, 0, GLR_PRIM_MASK);
}

/* every corner of `q` (4) is in view (clip space) */
static s32 refl_quad_in_view(const f32 *q) {
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    s32 k;

    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
    for (k = 0; k < 4; k++) {
        sceVu0ApplyMatrix(v, clip, (f32 *)q + k * 4);
        if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
            return 0;
        }
    }
    return 1;
}

/* the reflecting floor's side panels: a fixed quad on each side (which by the effect's kind
 * +0x1C and the side, the sign of `dx`) shows the reflection again at half strength, moved by
 * dx (the floor quad's width on screen, 1/16 pixels) */
void func_00316DE0(u8 *e, f32 dx) {
    static const u32 kQuads[4][4][4] = {
        {{0x42480000, 0x41F00000, 0xC1AA6F35, 0x3F800000}, {0x42480000, 0x41F00000, 0xC1E60000, 0x3F800000},
         {0x42480000, 0x3FA00000, 0xC1AA6F35, 0x3F800000}, {0x42480000, 0x3FA00000, 0xC1E60000, 0x3F800000}},
        {{0x41FA6F35, 0x41F00000, 0xC1200000, 0x3F800000}, {0x421B0000, 0x41F00000, 0xC1200000, 0x3F800000},
         {0x41FA6F35, 0x3FC00000, 0xC1200000, 0x3F800000}, {0x421B0000, 0x3FC00000, 0xC1200000, 0x3F800000}},
        {{0x41200000, 0x41B00000, 0x426AC866, 0x3F800000}, {0x41200000, 0x41B00000, 0x424D0000, 0x3F800000},
         {0x41200000, 0x00000000, 0x426AC866, 0x3F800000}, {0x41200000, 0x00000000, 0x424D0000, 0x3F800000}},
        {{0xC10B2196, 0x41B00000, 0x428C0000, 0x3F800000}, {0xBFA00000, 0x41B00000, 0x428C0000, 0x3F800000},
         {0xC10B2196, 0x00000000, 0x428C0000, 0x3F800000}, {0xBFA00000, 0x00000000, 0x428C0000, 0x3F800000}},
    };
    f32 q[4][4] __attribute__((aligned(16)));
    s32 k = (AT(e, 0x1C, u32) == 0x20000000 ? 0 : 2) + (dx < 0.0f ? 0 : 1), j;

    for (j = 0; j < 16; j++) {
        AT(&q[0][0], j * 4, u32) = kQuads[k][j / 4][j % 4];
    }
    if (!refl_quad_in_view(&q[0][0])) {
        return;
    }
    refl_mask_quad(&q[0][0]);
    glr_refl(0, AT(e, 0x14, s32) >> 1, 0, 1, dx / 16.0f);
}

/* `out` = b + (a - b) / 2 */
static void refl_mid(f32 *out, const f32 *a, const f32 *b) {
    sceVu0SubVector(out, (f32 *)a, (f32 *)b);
    sceVu0ScaleVector(out, out, 0.5f);
    sceVu0AddVector(out, out, (f32 *)b);
}

/* +0x14 draw (with OpenGL): unless the quad is facing away or out of view (+0x10 0), the
 * camera is reflected in its plane (eye and target, roll negated, the half-size matrices made
 * current) and each character (func_00317920) and creature (func_003175B0) near it is drawn
 * again into renderer layer 0x17 - its draw layer switched for the call (characters with +0xE4
 * cleared). With the camera back, if anything was drawn the reflection is blended over the
 * quad (or the screen, +0x10) at strength +0x14, mirrored per +0x18 (layer 0x18); kinds with
 * +0x1C add side panels (func_00316DE0) */
void func_00317D40(u8 *e) {
    VObject *cam = gCamera, *tc = gTexCache;
    f32 *q = (f32 *)(e + 0x50);
    f32 scr[4][4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 m1[4][4] __attribute__((aligned(16)));
    f32 m2[4][4] __attribute__((aligned(16)));
    ReflCamera saved __attribute__((aligned(16)));
    ReflCamera mirrored __attribute__((aligned(16)));
    f32 d, t, roll;
    s32 i, k, drawn = 0;

    if (AT(e, 0x14, s32) == 0) {
        return;
    }
    if (AT(e, 0x10, s32) == 0) {
        if ((u8)VCALL(cam, 0xD4, s32 (*)(VObject *, f32 *))(cam, q) == 1 &&
            (u8)VCALL(cam, 0xD4, s32 (*)(VObject *, f32 *))(cam, q + 4) == 1 &&
            (u8)VCALL(cam, 0xD4, s32 (*)(VObject *, f32 *))(cam, q + 8) == 1 &&
            (u8)VCALL(cam, 0xD4, s32 (*)(VObject *, f32 *))(cam, q + 12) == 1) {
            f32 p[9][4] __attribute__((aligned(16)));

            refl_mid(p[0], q + 4, q);
            refl_mid(p[1], q + 8, q);
            refl_mid(p[2], q + 8, q + 4);
            refl_mid(p[3], q + 12, q + 8);
            refl_mid(p[4], q + 4, q + 12);
            refl_mid(p[5], p[1], p[0]);
            refl_mid(p[6], p[3], p[1]);
            refl_mid(p[7], p[4], p[3]);
            refl_mid(p[8], p[0], p[4]);
            for (k = 0; k < 9 && (u8)VCALL(cam, 0xD4, s32 (*)(VObject *, f32 *))(cam, p[k]); k++) {
            }
            if (k == 5) {
                return;
            }
        }
        if (!refl_quad_in_view(q)) {
            return;
        }
        VCALL(cam, 0x44, void (*)(VObject *, f32 (*)[4]))(cam, m1);
        for (k = 0; k < 4; k++) {
            sceVu0ApplyMatrix(scr[k], m1, q + k * 4);
            scr[k][3] = 1.0f / scr[k][3];
            scr[k][0] *= scr[k][3];
            scr[k][1] *= scr[k][3];
        }
        if ((scr[1][0] - scr[0][0]) * (scr[2][1] - scr[0][1]) - (scr[1][1] - scr[0][1]) * (scr[2][0] - scr[0][0]) > 0.0f) {
            return;   /* facing away */
        }
    }
    sceVu0SubVector(a, q + 4, q);
    sceVu0SubVector(b, q + 8, q);
    sceVu0OuterProduct(n, a, b);
    sceVu0Normalize(n, n);
    VCALL(cam, 0x20, void (*)(VObject *, f32 *))(cam, saved.eye);
    VCALL(cam, 0x2C, void (*)(VObject *, f32 *))(cam, saved.target);
    saved.fov = VCALL(cam, 0x64, f32 (*)(VObject *))(cam);
    saved.unk24 = VCALL(gCamDirector, 0x24, s32 (*)(VObject *))(gCamDirector);
    mirrored = saved;
    d = sceVu0InnerProduct(n, q);
    t = sceVu0InnerProduct(n, saved.eye) - d;
    if (t == 0.0f) {
        return;
    }
    sceVu0ScaleVector(mirrored.eye, n, 2.0f * t);
    sceVu0SubVector(mirrored.eye, saved.eye, mirrored.eye);
    t = sceVu0InnerProduct(n, saved.target) - d;
    if (t == 0.0f) {
        return;
    }
    sceVu0ScaleVector(mirrored.target, n, 2.0f * t);
    sceVu0SubVector(mirrored.target, saved.target, mirrored.target);
    roll = VCALL(cam, 0x74, f32 (*)(VObject *))(cam);
    VCALL(cam, 0x88, void (*)(VObject *, ReflCamera *))(cam, &mirrored);
    VCALL(cam, 0x70, void (*)(VObject *, f32))(cam, -roll);
    VCALL(cam, 0x14, void (*)(VObject *))(cam);
    VCALL(cam, 0x4C, void (*)(VObject *, f32 (*)[4]))(cam, m1);
    VCALL(cam, 0x58, void (*)(VObject *, f32 (*)[4]))(cam, m2);
    VCALL(cam, 0x50, void (*)(VObject *, f32 (*)[4]))(cam, m1);
    VCALL(cam, 0x54, void (*)(VObject *, f32 (*)[4]))(cam, m2);
    for (i = 0; i < 6; i++) {
        u8 *c = (u8 *)gCharacters[i];

        if ((u8)func_00317920(e, i) == 1) {
            s32 layer;
            u8 e4;

            VCALL(tc, 0x18, void (*)(VObject *))(tc);
            layer = Character_Get152C((Character *)c);
            Character_Set152C((Character *)c, 0x17);
            e4 = AT(c, 0xE4, u8);
            AT(c, 0xE4, u8) = 0;
            VCALL(c, 0x2C, void (*)(u8 *))(c);
            drawn = 1;
            Character_Set152C((Character *)c, layer);
            AT(c, 0xE4, u8) = e4;
        }
    }
    for (i = 0; i < 10; i++) {
        u8 *o = ((u8 **)gCreatures)[i];

        if ((u8)func_003175B0(e, i) == 1) {
            s32 layer;

            VCALL(tc, 0x18, void (*)(VObject *))(tc);
            layer = Character_Get152C((Character *)o);
            Character_Set152C((Character *)o, 0x17);
            VCALL(o, 0x2C, void (*)(u8 *))(o);
            drawn = 1;
            Character_Set152C((Character *)o, layer);
        }
    }
    VCALL(cam, 0x88, void (*)(VObject *, ReflCamera *))(cam, &saved);
    VCALL(cam, 0x70, void (*)(VObject *, f32))(cam, roll);
    VCALL(cam, 0x14, void (*)(VObject *))(cam);
    VCALL(tc, 0x18, void (*)(VObject *))(tc);
    if (!drawn) {
        return;
    }
    glr_layer(0x18);
    if (AT(e, 0x10, s32) == 0) {
        refl_mask_quad(q);
    }
    glr_refl(1, AT(e, 0x14, s32), AT(e, 0x18, s32) != 0, AT(e, 0x10, s32) == 0, 0.0f);
    if (AT(e, 0x1C, u32) != 0 && AT(e, 0x10, s32) == 0) {
        f32 dx = (f32)((s32)(scr[1][0] * 16.0f) - (s32)(scr[0][0] * 16.0f));

        if (dx != 0.0f) {
            func_00316DE0(e, dx);
            func_00316DE0(e, -dx);
        }
    }
    glr_layer(-1);
}
#endif

/* +0x10 update: nothing */
void func_00319B00(void) {
}

/* +0x18 start: the quad's four corners (+0x50), then from the argument: +0x40 (a float, as a
 * whole number) -> +0x10, the strength byte +0x44 -> +0x14 (its top bits -> +0x1C), +0x48 ->
 * +0x18, which the renderer is told (+0x8C) */
void func_00317C70(u8 *e, u8 *arg) {
    s32 k;

    if (arg == NULL) {
        return;
    }
    for (k = 0; k < 16; k++) {
        AT(e, 0x50 + k * 4, f32) = AT(arg, k * 4, f32);
    }
    AT(e, 0x10, s32) = (s32)AT(arg, 0x40, f32);
    AT(e, 0x14, s32) = AT(arg, 0x44, u8);
    AT(e, 0x1C, u32) = AT(arg, 0x44, u32) & 0xF0000000;
    AT(e, 0x18, s32) = AT(arg, 0x48, s32);
    VCALL(gRenderer, 0x8C, void (*)(VObject *, s32))(gRenderer, AT(e, 0x18, s32));
}

/* ---- D_004795A0 (0x10 bytes; room 0x60's event object 1): a glow whose strength (+0x8)
 * follows a level (+0x4): up to the level and back down to 30 (+0xC 0 / 1), or for level 0x80
 * slowly up to 0x38 (2); 0xFF done (its +0xC / +0x10 / +0x18 in src/leaf/b7_00351D50.c). Its
 * draw (+0x14, FloorGlow_Draw) is the light's glow ---- */

extern void *D_004795A0[], *D_0046F580[];

/* +0x8 destructor */
/* 0x00358210 */
u8 *FloorGlow_dtor(u8 *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x0, void **) = D_004795A0;
        AT(e, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            EffectMgr_free(e);
        }
    }
    return e;
}

/* 0x00358270 */
void FloorGlow_SetParams(u8 *p, s32 *src) {
    if (src == NULL) {
        B7_W(p, 0x4) = 0;
        return;
    }
    if (*src != B7_W(p, 0x4)) {
        B7_W(p, 0x4) = *src;
        B7_W(p, 0xC) = (B7_W(p, 0x4) == 0x80) ? 2 : 0;
    }
}

#ifdef HG_NATIVE
#define GLR_PRIM_ADD 0x10000u
#define GLR_PRIM_NOZW 0x20000u

/* +0x14 draw (when +0x4 and +0x8 are set): the light's glow at the room's middle, added at
 * strength +0x8 fading to nothing at the rims, depth tested without writes (layer 0x19): a
 * pool on the floor (radius 20, 8 sides, y 0.3) and a cone from 16 below up to a ring of 16
 * at y 20.2; only when all of it is in view */
/* 0x003582D0 */
void FloorGlow_Draw(u8 *e) {
    f32 p[26][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 r = 20.0f * func_0031C248(0x1.921fb60000000p-1f /* 0.7853982 */);
    u32 centre = (u32)AT(e, 0x8, s32) << 24 | 0xFFFFFF;
    s32 i;

    if (AT(e, 0x4, s32) == 0 || AT(e, 0x8, s32) == 0) {
        return;
    }
    {
        static const f32 kPool[9][2] = {{0, 0}, {20, 0}, {1, 1}, {0, 20}, {-1, 1}, {-20, 0}, {-1, -1}, {0, -20}, {1, -1}};

        for (i = 0; i < 9; i++) {   /* (+-1 stand for +-r, the diagonals) */
            p[i][0] = kPool[i][0] == 1 ? r : kPool[i][0] == -1 ? -r : kPool[i][0];
            p[i][1] = 0x1.3333340000000p-2f /* 0.3 */;
            p[i][2] = kPool[i][1] == 1 ? r : kPool[i][1] == -1 ? -r : kPool[i][1];
            p[i][3] = 1.0f;
        }
    }
    p[9][0] = 0.0f;
    p[9][1] = -16.0f;
    p[9][2] = 0.0f;
    p[9][3] = 1.0f;
    for (i = 0; i < 16; i++) {
        f32 a = (0x1.921fb60000000p+1f /* 3.1415927 */ * (22.5f * (f32)i)) / 180.0f;

        p[10 + i][0] = 20.0f * func_0031C248(a);
        AT(&p[10 + i][1], 0, u32) = 0x41A1999A;   /* 20.2 */
        p[10 + i][2] = 20.0f * func_0031C058(a);
        p[10 + i][3] = 1.0f;
    }
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
    for (i = 0; i < 26; i++) {
        f32 v[4] __attribute__((aligned(16)));

        sceVu0ApplyMatrix(v, clip, p[i]);
        if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
            return;
        }
    }
    glr_layer(0x19);
    for (i = 0; i < 24; i++) {   /* the two fans as triangles: centre, rim i, rim i + 1 */
        s32 fan = i >= 8, k = fan ? i - 8 : i, n = fan ? 16 : 8;
        s32 c = fan ? 9 : 0, a = (fan ? 10 : 1) + k, b = (fan ? 10 : 1) + (k + 1) % n;
        f32 xyzw[3][4] __attribute__((aligned(16)));
        f32 st[3][2] = {{0}};
        u32 rgba[3];

        sceVu0CopyVector(xyzw[0], p[c]);
        sceVu0CopyVector(xyzw[1], p[a]);
        sceVu0CopyVector(xyzw[2], p[b]);
        AT(&xyzw[0][3], 0, u32) = 0x8000;
        AT(&xyzw[1][3], 0, u32) = 0x8000;
        AT(&xyzw[2][3], 0, u32) = 0;
        rgba[0] = centre;
        rgba[1] = rgba[2] = fan ? 0x000000 : 0x808080;   /* the rims' alpha 0 */
        glr_strip(&clip[0][0], 3, &xyzw[0][0], &st[0][0], (const u8 *)rgba, NULL, 0,
                  0x40 | GLR_PRIM_ADD | GLR_PRIM_NOZW);
    }
    glr_layer(-1);
}
#endif

/* Fade step: +4 speed, +8 value, +C state (0 up to +4, 1 down to 30, 2 up to 56, 0xFF done). */
/* 0x00358B30 */
s32 FloorGlow_Update(u8 *p) {
    switch (B7_W(p, 0xC)) {
    case 0:
        B7_W(p, 0x8) += B7_W(p, 0x4) >> 4;
        if (B7_W(p, 0x4) < B7_W(p, 0x8)) {
            B7_W(p, 0x8) = B7_W(p, 0x4);
            B7_W(p, 0xC) = 1;
        }
        break;
    case 1:
        B7_W(p, 0x8) -= B7_W(p, 0x4) >> 4;
        if (B7_W(p, 0x8) < 0x1E) {
            B7_W(p, 0x8) = 0x1E;
            B7_W(p, 0xC) = 0xFF;
        }
        break;
    case 2:
        B7_W(p, 0x8) += B7_W(p, 0x4) >> 5;
        if (B7_W(p, 0x8) >= 0x38) {
            B7_W(p, 0x8) = 0x38;
            B7_W(p, 0xC) = 0xFF;
        }
        break;
    }
    return 1;
}

/* 0x00358C10 */
void FloorGlow_Start(u8 *p) {
    B7_W(p, 0x4) = 0;
    B7_W(p, 0x8) = 0;
    B7_W(p, 0xC) = 0;
}

/* ---- D_00479320 (0x218 bytes): 4 wisps rising and swirling about a point (+0x1D0), in two
 * buffers of sprite instances (+0x10 + 0xC0 x the current one +0x210; 0x30 each), per wisp a
 * rise speed (+0x1E0), angle (+0x1F0) and radius (+0x200) about the point; drawn by the quad
 * drawer at +0x190 (set up by Wisps_Start). +0x214 its kind: low 12 bits 0 a lasting flame
 * (slow, respawning), else a burst; bit 0x8000 it ends with room effect 0 ---- */

extern void *D_00479320[];

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x003521F0 */
u8 *Wisps_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479320;
    AT(o, 0x190, void **) = D_0046FC30;
    AT(o, 0x190, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* wisp i anew (`again`: when respawning): orange, half to fully faded in, at a random angle
 * and radius (0..0.5) about the point, rising 0.2..0.3 a frame (a burst: 0.01..0.26), size
 * 0.2..0.3 and a random turn; a first one (not `again`) also starts a random height up */
void func_00352280(u8 *o, s32 i, s32 again) {
    static const union { u32 u; f32 f; } k02 = {0x3E4CCCCD}, k01 = {0x3DCCCCCD}, k001 = {0x3C23D70A},
        k025 = {0x3E800000}, kM0025 = {0xBCCCCCCD}, k005 = {0x3D4CCCCD}, kPi = {0x40490FDB};
    VObject *rnd = gRandom;
    u8 *r = o + AT(o, 0x210, s32) * 0xC0 + i * 0x30 + 0x10;
    f32 *speed = (f32 *)(o + 0x1E0 + i * 4);
    s32 up = 0;

    if (!(AT(o, 0x214, s32) & 0xFFF)) {
        *speed = k02.f + k01.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    } else {
        *speed = k001.f + k025.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    }
    rnd = gRandom;
    AT(o, 0x1F0 + i * 4, f32) = (kPi.f * (360.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f))) / 180.0f;
    AT(o, 0x200 + i * 4, f32) = 0.5f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    AT(r, 0x0, s32) = 0x80;
    AT(r, 0x4, s32) = 0x40;
    AT(r, 0x8, s32) = 0x10;
    AT(r, 0xC, s32) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0x3F) + 0x40;
    if (!again) {
        up = (s32)VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom);
        AT(r, 0xC, s32) = AT(r, 0xC, s32) - (s32)(128.0f * (f32)up);
        if (AT(r, 0xC, s32) < 0) {
            AT(r, 0xC, s32) = 0;
        }
        AT(o, 0x1E0 + i * 4, f32) = AT(o, 0x1E0 + i * 4, f32) + kM0025.f * (f32)up;
        if (AT(o, 0x1E0 + i * 4, f32) < k005.f) {
            *speed = k005.f;
        }
    }
    AT(r, 0x10, f32) = AT(o, 0x1D0, f32) + AT(o, 0x200 + i * 4, f32) * func_0031C058(AT(o, 0x1F0 + i * 4, f32));
    AT(r, 0x14, f32) = AT(o, 0x1D4, f32) + (f32)up;
    AT(r, 0x18, f32) = AT(o, 0x1D8, f32) + AT(o, 0x200 + i * 4, f32) * func_0031C248(AT(o, 0x1F0 + i * 4, f32));
    AT(r, 0x1C, f32) = 1.0f;
    rnd = gRandom;
    AT(r, 0x20, f32) = k02.f + k01.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    AT(r, 0x24, f32) = AT(r, 0x20, f32);
    AT(r, 0x28, f32) = (kPi.f * (360.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f))) / 180.0f;
    AT(r, 0x2C, s32) = 0;
}

/* +0x18 start: at the point `arg` (its vector), of kind arg +0x10, the 4 wisps placed (a
 * lasting flame's as first ones) */
/* 0x00352620 */
void Wisps_SetParams(u8 *o, u8 *arg) {
    s32 i;

    if (arg == NULL) {
        return;
    }
    sceVu0CopyVector((f32 *)(o + 0x1D0), (f32 *)arg);
    AT(o, 0x214, s32) = AT(arg, 0x10, s32);
    if (!(AT(o, 0x214, s32) & 0xFFF)) {
        for (i = 0; i < 4; i++) {
            func_00352280(o, i, 0);
        }
    } else {
        for (i = 0; i < 4; i++) {
            func_00352280(o, i, 1);
        }
    }
}

/* +0x14 draw: the current buffer through the quad drawer */
/* 0x003526D0 */
void Wisps_Draw(u8 *o) {
    AT(o, 0x1A0, u8 *) = o + AT(o, 0x210, s32) * 0xC0 + 0x10;
    func_002E56C0(o + 0x190);
}

/* +0x10 update: into the other buffer, each wisp fading by 4..7; a faded one comes back anew
 * (a lasting flame) or stays out; a live one swirls (its radius growing 0.02 / a burst 0.04,
 * its angle by up to 15 degrees), rises (its speed falling by 0.015 to 0.05) and is placed
 * about the point. 0 once all are out (with bit 0x8000: also once room effect 0 is gone) */
/* 0x00352700 */
s32 Wisps_Update(u8 *o) {
    static const union { u32 u; f32 f; } k002 = {0x3CA3D70A}, k004 = {0x3D23D70A}, kSlow = {0xBC75C28F},
        k005 = {0x3D4CCCCD}, kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    VObject *rnd = gRandom;
    s32 done = 1, i, k;

    AT(o, 0x210, s32) ^= 1;
    for (i = 0; i < 4; i++) {
        u8 *r = o + AT(o, 0x210, s32) * 0xC0 + i * 0x30 + 0x10;
        u8 *from = o + (AT(o, 0x210, s32) ^ 1) * 0xC0 + i * 0x30 + 0x10;
        f32 *ang = (f32 *)(o + 0x1F0 + i * 4);

        for (k = 0; k < 12; k++) {
            AT(r, k * 4, u32) = AT(from, k * 4, u32);
        }
        if (AT(r, 0xC, s32) > 0) {
            AT(r, 0xC, s32) = AT(r, 0xC, s32) - ((VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 3) + 4);
        }
        if (AT(r, 0xC, s32) <= 0) {
            if (!(AT(o, 0x214, s32) & 0xFFF)) {
                func_00352280(o, i, 1);
                done = 0;
            } else {
                AT(r, 0xC, s32) = 0;
            }
            continue;
        }
        done = 0;
        if (!(AT(o, 0x214, s32) & 0xFFF)) {
            AT(o, 0x200 + i * 4, f32) = AT(o, 0x200 + i * 4, f32) + k002.f;
        } else {
            AT(o, 0x200 + i * 4, f32) = AT(o, 0x200 + i * 4, f32) + k004.f;
        }
        *ang = *ang + (kPi.f * (30.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f))) / 180.0f;
        AT(o, 0x1E0 + i * 4, f32) = AT(o, 0x1E0 + i * 4, f32) + kSlow.f;
        if (AT(o, 0x1E0 + i * 4, f32) < k005.f) {
            AT(o, 0x1E0 + i * 4, f32) = k005.f;
        }
        AT(r, 0x14, f32) = AT(r, 0x14, f32) + AT(o, 0x1E0 + i * 4, f32);
        if (*ang < -kPi.f) {
            *ang = *ang + kTwoPi.f;
        } else if (!(*ang <= kPi.f)) {
            *ang = *ang - kTwoPi.f;
        }
        AT(r, 0x10, f32) = AT(o, 0x1D0, f32) + AT(o, 0x200 + i * 4, f32) * func_0031C058(*ang);
        AT(r, 0x18, f32) = AT(o, 0x1D8, f32) + AT(o, 0x200 + i * 4, f32) * func_0031C248(*ang);
    }
    if ((AT(o, 0x214, s32) & 0x8000) && RoomEffects_Get(gRoomEffects, 0) == NULL) {
        done = 1;
    }
    return done != 1;
}

/* Initialises a render/texture setting block at +0x198. */
/* 0x00352A70 */
void Wisps_Start(u8 *p) {
    B7_W(p, 0x210) = 0;
    B7_W(p, 0x214) = 0;
    B7_D(p, 0x198) = -1;
    B7_W(p, 0x1A4) = 0;
    B7_W(p, 0x1A8) = 0;
    B7_W(p, 0x1AC) = 0;
    B7_W(p, 0x1B0) = 0x19;
    B7_H(p, 0x1B4) = 4;
    B7_H(p, 0x1B6) = 0x6C;
    B7_H(p, 0x1B8) = 0x4C;
    B7_H(p, 0x1BA) = 8;
    B7_H(p, 0x1BC) = 8;
    B7_H(p, 0x1BE) = 0x200;
    B7_H(p, 0x1C0) = 0x100;
    B7_B(p, 0x1C2) = 0x40;
    B7_B(p, 0x1C3) = 1;
    B7_B(p, 0x1C4) = 1;
    B7_B(p, 0x1C5) = 0x10;
    B7_B(p, 0x1C6) = 0xFF;
}

/* ---- D_0046F5A0 (0x1C60 bytes; room 0x24): rising smoke, 64 particles in two buffers of
 * sprite instances (+0x10 + 0xC00 x the current one +0x1C50: RGBA, position, size, turn,
 * frame - 0x30 each), their velocities at +0x1850 (16 each), drawn by the quad drawer at
 * +0x1810 ---- */

extern void *D_0046F5A0[];

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x002D63F0 */
u8 *RisingSmoke_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0046F5A0;
    AT(o, 0x1810, void **) = D_0046FC30;
    AT(o, 0x1810, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* particle i (anew: `again`, faded in a random 0..11 steps) at the source (56.5, 13 + the
 * steps, -1.4, spread 1.1), dim purple, size 0.4, drifting up 0.07..0.17 a frame */
void func_002D6480(u8 *o, s32 i, s32 again) {
    static const union { u32 u; f32 f; } k11 = {0x3F8CCCCD}, kZ = {0xBFB33333}, k04 = {0x3ECCCCCD},
        kSpread = {0x3CF5C28F}, kRise = {0x3D8F5C29}, kRiseVar = {0x3DCCCCCD};
    VObject *rnd = gRandom;
    u8 *r = o + AT(o, 0x1C50, s32) * 0xC00 + i * 0x30 + 0x10;
    u8 *v;
    s32 k = 0;

    AT(r, 0x0, s32) = 0x50;
    AT(r, 0x4, s32) = 0x40;
    AT(r, 0x8, s32) = 0x50;
    AT(r, 0xC, s32) = 0x50;
    AT(r, 0x10, f32) = 56.5f + k11.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    if (!again) {
        k = (s32)(12.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd));
        AT(r, 0xC, s32) = AT(r, 0xC, s32) - (s32)(10.0f * (f32)k);
        if (AT(r, 0xC, s32) < 0) {
            AT(r, 0xC, s32) = 0;
        }
    }
    AT(r, 0x14, f32) = 13.0f + (f32)k;
    rnd = gRandom;
    AT(r, 0x18, f32) = kZ.f + k11.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    AT(r, 0x1C, f32) = 1.0f;
    AT(r, 0x20, f32) = k04.f;
    AT(r, 0x24, f32) = k04.f;
    AT(r, 0x28, f32) = 0.0f;
    AT(r, 0x2C, s32) = 0;
    v = o + i * 16;
    AT(v, 0x1850, f32) = kSpread.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    AT(v, 0x1854, f32) = kRise.f + kRiseVar.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    AT(v, 0x1858, f32) = kSpread.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
}

/* +0x14 draw: the current buffer through the quad drawer */
/* 0x002D6700 */
void RisingSmoke_Draw(u8 *o) {
    AT(o, 0x1820, u8 *) = o + AT(o, 0x1C50, s32) * 0xC00 + 0x10;
    func_002E56C0(o + 0x1810);
}

/* +0x10 update: into the other buffer, each particle grown (0.05), turned (3 degrees) and
 * moved; anew above height 25 or once faded */
/* 0x002D6730 */
s32 RisingSmoke_Update(u8 *o) {
    static const union { u32 u; f32 f; } kGrow = {0x3D4CCCCD}, kTurn = {0x3D567750}, kPi = {0x40490FDB},
        kTwoPi = {0x40C90FDB};
    s32 i, k;

    AT(o, 0x1C50, s32) ^= 1;
    for (i = 0; i < 0x40; i++) {
        u8 *r = o + AT(o, 0x1C50, s32) * 0xC00 + i * 0x30 + 0x10;
        u8 *from = o + (AT(o, 0x1C50, s32) ^ 1) * 0xC00 + i * 0x30 + 0x10;
        u8 *v = o + i * 16;

        for (k = 0; k < 12; k++) {
            AT(r, k * 4, u32) = AT(from, k * 4, u32);
        }
        r = o + AT(o, 0x1C50, s32) * 0xC00 + i * 0x30 + 0x10;
        AT(r, 0x20, f32) = AT(r, 0x20, f32) + kGrow.f;
        AT(r, 0x24, f32) = AT(r, 0x24, f32) + kGrow.f;
        AT(r, 0x28, f32) = AT(r, 0x28, f32) + kTurn.f;
        if (!(AT(r, 0x28, f32) <= kPi.f)) {
            AT(r, 0x28, f32) = AT(r, 0x28, f32) - kTwoPi.f;
        }
        AT(r, 0x10, f32) = AT(r, 0x10, f32) + AT(v, 0x1850, f32);
        AT(r, 0x14, f32) = AT(r, 0x14, f32) + AT(v, 0x1854, f32);
        AT(r, 0x18, f32) = AT(r, 0x18, f32) + AT(v, 0x1858, f32);
        if (!(AT(r, 0x14, f32) < 25.0f)) {
            func_002D6480(o, i, 1);
        }
        if (AT(r, 0xC, s32) > 0) {
            AT(r, 0xC, s32) -= 1;
        } else {
            func_002D6480(o, i, 1);
        }
    }
    return 1;
}

/* +0xC set up: the quad drawer's settings (64 instances of one 32 x 32 cell at (0, 64) of
 * texture 1 / 0x10, layer 0x19), buffer 0, every particle placed */
/* 0x002D6920 */
void RisingSmoke_Start(u8 *o) {
    s32 i;

    AT(o, 0x1C50, s32) = 0;
    AT(o, 0x1818, s64) = -1;
    AT(o, 0x1828, s32) = 0;
    AT(o, 0x182C, s32) = 0;
    AT(o, 0x1830, s32) = 0x19;
    AT(o, 0x1834, s16) = 0x40;
    AT(o, 0x1836, s16) = 0;
    AT(o, 0x1838, s16) = 0x40;
    AT(o, 0x183A, s16) = 0x20;
    AT(o, 0x183C, s16) = 0x20;
    AT(o, 0x183E, s16) = 0x200;
    AT(o, 0x1840, s16) = 0x100;
    AT(o, 0x1842, s8) = 0;
    AT(o, 0x1843, s8) = 1;
    AT(o, 0x1844, s8) = 1;
    AT(o, 0x1845, s8) = 0x10;
    AT(o, 0x1846, s8) = -1;
    for (i = 0; i < 0x40; i++) {
        func_002D6480(o, i, 0);
    }
}

/* ---- D_00476BB0 (0x7460 bytes; room 0x4F): 256 motes of dust drifting down through a shaft
 * (around x 38, z -11, from up to 50 high), in two buffers of quad records (+0x10 + 0x3000 x the
 * current one +0x7450), velocities at +0x6050 (16 each), the quad drawer at +0x6010, each one's
 * fade direction at +0x7050 (bit 31: fading out) ---- */

#define DUST_REC(o, buf, i) ((QuadRec *)((o) + 0x10 + (buf) * 0x3000) + (i))
#define DUST_VEL(o, i) ((f32 *)((o) + 0x6050 + (i) * 0x10))
#define DUST_FADE(o, i) AT(o, 0x7050 + (i) * 4, u32)

/* mote `i` (re)started somewhere up the shaft (narrower the higher), 2 x 2, drifting a little
 * and sinking 0.01..0.11 a frame; not `again` (the first round): at a random alpha and fade */
void func_0033BE90(u8 *o, s32 i, s32 again) {
    static const union { u32 u; f32 f; } k16 = {0x41800000}, k50 = {0x42480000}, k005 = {0x3D4CCCCD};   /* multiplied first */
    VObject *rnd = gRandom;
    QuadRec *r = DUST_REC(o, AT(o, 0x7450, s32), i);
    f32 *v = DUST_VEL(o, i);
    f32 h, x;

    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = 0;
    h = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    x = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    r->pos[0] = 38.0f + (k16.f * (1.0f - h)) * (x - 0.5f);
    r->pos[1] = k50.f * h;
    r->pos[2] = -11.0f + 4.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    r->pos[3] = 1.0f;
    r->w = 2.0f;
    r->h = 2.0f;
    r->turn = 0.0f;
    r->frame = 0;
    v[0] = k005.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    v[1] = -0x1.47ae14p-7f - 0x1.99999ap-4f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);   /* -0.01 - 0.1 x */
    v[2] = k005.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    DUST_FADE(o, i) = 0;
    if (!again) {
        rnd = gRandom;
        r->rgba[3] = VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0x1F;
        DUST_FADE(o, i) = VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0x80000000;
    }
}

/* +0x10 update: flip the buffers; each mote carried over, growing 0.02, turning half a degree,
 * moving, slowing its fall under 6 and restarted under -3; on the even frames fading in to
 * 0x21 then out to 0 (and restarted) */
/* 0x0033C140 */
s32 DustShaft_Update(u8 *o) {
    s32 i, k;

    AT(o, 0x7450, s32) ^= 1;
    for (i = 0; i < 256; i++) {
        QuadRec *r;
        f32 *v = DUST_VEL(o, i);

        for (k = 0; k < 12; k++) {
            ((u32 *)DUST_REC(o, AT(o, 0x7450, s32), i))[k] = ((u32 *)DUST_REC(o, AT(o, 0x7450, s32) ^ 1, i))[k];
        }
        r = DUST_REC(o, AT(o, 0x7450, s32), i);
        r->w = r->w + 0x1.47ae14p-6f;   /* 0.02 */
        r->h = r->h + 0x1.47ae14p-6f;
        r->turn = r->turn + 0x1.1df46ap-7f;   /* half a degree */
        if (!(r->turn <= 0x1.921fb6p+1f)) {
            r->turn = r->turn - 0x1.921fb6p+2f;
        }
        r->pos[0] = r->pos[0] + v[0];
        r->pos[1] = r->pos[1] + v[1];
        r->pos[2] = r->pos[2] + v[2];
        if (r->pos[1] < -3.0f) {
            func_0033BE90(o, i, 1);
        } else if (r->pos[1] < 6.0f) {
            v[1] = v[1] * 0x1.cccccc0000000p-1f;   /* 0.9 */
        }
        if (AT(o, 0x7450, s32) != 0) {
            continue;
        }
        if (!(DUST_FADE(o, i) & 0x80000000)) {
            r->rgba[3]++;
            if (r->rgba[3] >= 0x21) {
                DUST_FADE(o, i) = 0x80000000;
            }
        } else if (r->rgba[3] > 0) {
            r->rgba[3]--;
        } else {
            func_0033BE90(o, i, 1);
        }
    }
    return 1;
}

/* +0xC set up: buffer 0, the drawer (256 quads of a 32 x 32 cell at (32, 64), blended, the
 * first palette, layer 0x19), every mote started */
/* 0x0033C3C0 */
void DustShaft_Start(u8 *o) {
    s32 i;

    AT(o, 0x7450, s32) = 0;
    AT(o, 0x6018, s64) = -1;
    AT(o, 0x6024, s32) = 0;
    AT(o, 0x6028, s32) = 0;
    AT(o, 0x602C, s32) = 0;
    AT(o, 0x6030, s32) = 0x19;
    AT(o, 0x6034, s16) = 0x100;
    AT(o, 0x6036, s16) = 0x20;
    AT(o, 0x6038, s16) = 0x40;
    AT(o, 0x603A, s16) = 0x20;
    AT(o, 0x603C, s16) = 0x20;
    AT(o, 0x603E, s16) = 0x200;
    AT(o, 0x6040, s16) = 0x100;
    AT(o, 0x6042, s8) = 0x40;
    AT(o, 0x6043, s8) = 1;
    AT(o, 0x6044, s8) = 1;
    AT(o, 0x6045, s8) = 0x10;
    AT(o, 0x6046, s8) = -1;
    for (i = 0; i < 256; i++) {
        func_0033BE90(o, i, 0);
    }
}

/* the effect manager's objects' +0x18 for those that take nothing */
/* 0x002D63E0 */
void EffectBase_SetParams(void) {
}

/* ... and +0x1C: -1 */
/* 0x002D63D0 */
s32 EffectBase_Query(void) {
    return -1;
}

/* ---- room effect D_0046FF60: shards - 16 small random boxes thrown up from a point (+0xE10)
 * that tumble, fall and bounce on its height until they settle; kind (+0xE38) 0 / 3 a gentle
 * spray, 2 / 3 bigger, slower turning pieces thrown higher, 4 thrown down ---- */

extern void *D_0046FF60[], *D_0046F580[];

typedef struct Shard {
    /* 0x00 */ f32 corner[8][4];   /* a random box: the signs of x, y, z by corner */
    /* 0x80 */ f32 scale[4];
    /* 0x90 */ f32 rot[3];         /* radians */
    /* 0x9C */ u8 pad9C[4];
    /* 0xA0 */ f32 pos[4];
    /* 0xB0 */ f32 vel[4];
    /* 0xC0 */ f32 spin[3];        /* degrees per frame */
    /* 0xCC */ u8 padCC[4];
    /* 0xD0 */ s32 alive;
    /* 0xD4 */ f32 floor;          /* (D_00479B00) the floor under it */
    /* 0xD8 */ s16 delay;          /* (D_00479B00) frames before it shows */
    /* 0xDA */ u8 alpha;           /* (D_00479B00) */
    /* 0xDB */ u8 bounce;          /* (D_00479B00) it bounces off its floor */
    /* 0xDC */ u8 landed;          /* (D_00479B00) */
    /* 0xDD */ u8 padDD[3];
} Shard;

_Static_assert(sizeof(Shard) == 0xE0, "Shard");

#define SHARD(e, i) ((Shard *)((u8 *)(e) + 0x10) + (i))
#define SHARD_RND() VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom)

/* +0x8 destructor */
/* 0x002E9B00 */
u8 *Effect6FF60_dtor(u8 *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x0, void **) = D_0046FF60;
        if (e != NULL) {
            AT(e, 0x0, void **) = D_0046F580;
        }
        if ((s16)flags > 0) {
            EffectMgr_free(e);
        }
    }
    return e;
}

/* +0xC set up: nothing */
/* 0x002EB380 */
void Effect6FF60_Start(u8 *e) {
}

/* throw shard `i`: a random box (each corner's coordinates 0..1 with its signs), size, turn,
 * position about the origin, velocity and spin by the kind */
void func_002E9B60(u8 *e, s32 i) {
    static const s8 sx[8] = {1, -1, 1, -1, 1, -1, 1, -1};
    static const s8 sy[8] = {1, 1, -1, -1, 1, 1, -1, -1};
    static const s8 sz[8] = {1, 1, 1, 1, -1, -1, -1, -1};
    VObject *rnd = gRandom;
    Shard *s = SHARD(e, i);
    s16 kind;
    f32 dot;
    s32 k;

    for (k = 0; k < 8; k++) {
        f32 v;

        v = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        s->corner[k][0] = sx[k] < 0 ? -v : v;
        v = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        s->corner[k][1] = sy[k] < 0 ? -v : v;
        v = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        s->corner[k][2] = sz[k] < 0 ? -v : v;
        s->corner[k][3] = 1.0f;
    }
    for (k = 0; k < 3; k++) {
        s->scale[k] = 0x1.99999ap-4f /* 0.1 */ + 0x1.ccccccp-1f /* 0.9 */ * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    }
    for (k = 0; k < 3; k++) {
        s->rot[k] = 6.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    }
    s->pos[0] = AT(e, 0xE10, f32) + 4.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    s->pos[1] = AT(e, 0xE14, f32) + 8.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    s->pos[2] = AT(e, 0xE18, f32) + 4.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    s->pos[3] = 1.0f;
    for (k = 0; k < 3; k++) {
        s->spin[k] = 22.5f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    }
    s->alive = 1;
    if ((u32)(AT(e, 0xE38, s16) - 2) < 2) {
        rnd = gRandom;
        for (k = 0; k < 3; k++) {
            s->scale[k] = 0.5f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        }
        s->pos[1] = AT(e, 0xE14, f32) + 15.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        for (k = 0; k < 3; k++) {
            s->spin[k] = 10.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        }
    }

    /* the box's lopsidedness against its size sets how hard it is thrown up */
    kind = AT(e, 0xE38, s16);
    rnd = gRandom;
    s->vel[0] = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f;
    if (kind == 0 || kind == 3) {
        s->vel[0] = 0x1.99999ap-2f /* 0.4 */ * s->vel[0];
    }
    {
        Shard *c = s;
        f32 zs = c->corner[0][2] + c->corner[1][2] + c->corner[2][2] + c->corner[3][2] - c->corner[4][2] -
                 c->corner[5][2] - c->corner[6][2] - c->corner[7][2];
        f32 xs = c->corner[0][0] - c->corner[1][0] + c->corner[2][0] - c->corner[3][0] + c->corner[4][0] -
                 c->corner[5][0] + c->corner[6][0] - c->corner[7][0];
        f32 ys = c->corner[1][1] + c->corner[0][1] - c->corner[2][1] - c->corner[3][1] + c->corner[4][1] +
                 c->corner[5][1] - c->corner[6][1] - c->corner[7][1];

        dot = (c->scale[1] * ys + c->scale[0] * xs) + c->scale[2] * zs;
    }
    if (kind == 4) {
        s->vel[1] = -0x1.47ae14p-6f /* -0.02 */ * (21.0f - dot);
        s->vel[2] = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f;
    } else if (kind == 0 || kind == 3) {
        s->vel[1] = 0x1.99999ap-4f /* 0.1 */ + 0x1.99999ap-5f /* 0.05 */ * (21.0f - dot);
        s->vel[2] = 0x1.99999ap-2f /* 0.4 */ * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    } else {
        s->vel[1] = 0x1.99999ap-4f /* 0.1 */ + 0x1.47ae14p-7f /* 0.01 */ * (21.0f - dot);
        s->vel[2] = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f;
    }
}

/* +0x18 start (arg { origin, +0x10.. its colour etc., +0x24 u16, +0x26 the kind, +0x28 the
 * floor below the origin }) */
/* 0x002EA5B0 */
void Effect6FF60_SetParams(u8 *e, u8 *arg) {
    s32 i;

    if (arg == NULL) {
        return;
    }
    sceVu0CopyVector((f32 *)(e + 0xE10), (f32 *)arg);
    AT(e, 0xE20, f32) = AT(arg, 0x10, f32);
    AT(e, 0xE24, f32) = AT(arg, 0x14, f32);
    AT(e, 0xE28, f32) = AT(arg, 0x18, f32);
    AT(e, 0xE2C, f32) = AT(arg, 0x1C, f32);
    AT(e, 0xE30, s32) = AT(arg, 0x20, s32);
    AT(e, 0xE34, s32) = AT(arg, 0x24, u16);
    AT(e, 0xE38, s16) = AT(arg, 0x26, s16);
    for (i = 0; i < 16; i++) {
        func_002E9B60(e, i);
    }
    AT(e, 0xE14, f32) += AT(arg, 0x28, f32);
}

/* +0x10 update: the shards tumble and fall; under the origin's height (+0xE14) one still
 * falling fast bounces (at half its sideways speed, 0.3 of its fall), else it settles. 1 while
 * any moves. */
/* 0x002EB1C0 */
s32 Effect6FF60_Update(u8 *e) {
    s32 any = 0, i, k;

    for (i = 0; i < 16; i++) {
        Shard *s = SHARD(e, i);

        if (s->alive == 0) {
            continue;
        }
        any = 1;
        for (k = 0; k < 3; k++) {
            s->rot[k] += 0x1.921fb6p+1f /* pi */ * s->spin[k] / 180.0f;
            if (!(s->rot[k] <= 0x1.921fb6p+1f)) {
                s->rot[k] -= 0x1.921fb6p+2f /* 2pi */;
            }
        }
        s->pos[0] += s->vel[0];
        s->pos[1] += s->vel[1];
        s->pos[2] += s->vel[2];
        s->vel[1] += -0x1.99999ap-4f /* -0.1 */;
        if (s->pos[1] < AT(e, 0xE14, f32)) {
            if (s->vel[1] < -0.5f) {
                s->pos[1] = AT(e, 0xE14, f32);
                s->vel[0] *= 0.5f;
                s->vel[1] = 0x1.333334p-2f /* 0.3 */ * -s->vel[1];
                s->vel[2] *= 0.5f;
            } else {
                s->alive = 0;
            }
        }
    }
    return any != 0;
}

/* the faces of a shard's box (corner indices, in strip order) */
static const u8 kShardFace[6][4] = {
    {0, 1, 2, 3}, {1, 5, 3, 7}, {5, 4, 7, 6}, {4, 0, 6, 2}, {0, 1, 4, 5}, {6, 7, 2, 3},
};

#ifdef HG_NATIVE

/* the shards as textured boxes, drawn with OpenGL (from field base fb: texture fb+0x14, its cell fb+0x0 / fb+0x4,
 * size fb+0x8 / fb+0xC, colour fb+0x10; `n` shards `stride` bytes apart from +0x10; `kind` 1: dead ones
 * skipped, 2: the falling stones - waiting ones skipped, the rest blended at their alpha faded in from
 * 10 to 20 away from the camera); a shard with a corner off screen is skipped */
static inline void shards_draw(u8 *e, u32 fb, s32 n, u32 stride, s32 kind) {
    VObject *tc = gTexCache, *cam;
    u8 *tex;
    u32 slot;
    u64 tex0;
    f32 screen[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 eye[4] __attribute__((aligned(16)));
    s32 i, k;

    slot = VCALL(tc, 0x8, u32 (*)(VObject *, s32, s32))(tc, AT(e, fb + 0x14, s32), 0);
    if (slot == (u32)-1) {
        return;
    }
    tex = VCALL(tc, 0xC, u8 *(*)(VObject *, s32, s32))(tc, AT(e, fb + 0x14, s32), 0);
    if (slot & 0x80000000) {
        slot &= 0x7FFFFFFF;
        if (!(VCALL(gRenderer, 0x44, u32 (*)(VObject *, u32, u8 *, s32))(gRenderer, slot, tex, 1) & 0xFF)) {
            return;
        }
    }
    tex0 = VCALL(gVram, 0x28, u64 (*)(VObject *, u32, u32, u32, u32, u32))(gVram, slot, tex[0],
                                                                              AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
    cam = gCamera;
    VCALL(cam, 0x44, void (*)(VObject *, f32 (*)[4]))(cam, screen);
    VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    if (kind == 2) {
        VCALL(cam, 0x20, void (*)(VObject *, f32 *))(cam, eye);
    }
    for (i = 0; i < n; i++) {
        static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD};   /* 0.1, multiplied first */
        Shard *s = (Shard *)(e + 0x10 + i * stride);
        f32 m[4][4] __attribute__((aligned(16)));
        f32 world[8][4] __attribute__((aligned(16)));
        u32 rgbaWord = AT(e, fb + 0x10, u32), prim = 0x10;
        s32 off = 0;

        if (kind == 1 && s->alive == 0) {
            continue;
        }
        if (kind == 2) {
            f32 d[4] __attribute__((aligned(16)));
            f32 dist, f;
            u32 a;

            if (s->delay != 0) {
                continue;
            }
            sceVu0SubVector(d, s->pos, eye);
            dist = ee_sqrtf(sceVu0InnerProduct(d, d));
            if (dist < 10.0f) {
                f = 0.0f;
            } else {
                f = k01.f * (dist - 10.0f);
                if (!(f <= 1.0f)) {
                    f = 1.0f;
                }
            }
            a = (u32)((f32)s->alpha * f);
            if (a == 0) {
                continue;
            }
            rgbaWord = (rgbaWord & 0xFFFFFF) | a << 24;
            prim = 0x10 | 0x40;
        }
        sceVu0UnitMatrix(m);
        m[0][0] = s->scale[0];
        m[1][1] = s->scale[1];
        m[2][2] = s->scale[2];
        sceVu0RotMatrix(m, m, s->rot);
        sceVu0TransMatrix(m, m, s->pos);
        for (k = 0; k < 8; k++) {
            sceVu0ApplyMatrix(world[k], m, s->corner[k]);
        }
        for (k = 0; k < 8; k++) {
            f32 c[4] __attribute__((aligned(16)));
            f32 w;

            sceVu0ApplyMatrix(c, clip, world[k]);
            w = c[3];
            if (!(c[0] <= w) || c[0] < -w || !(c[1] <= w) || c[1] < -w || !(c[2] <= w) || c[2] < -w) {
                off = 1;
                break;
            }
        }
        if (off) {
            continue;
        }
        {
            u8 rgba[4][4];
            s32 f;

            for (k = 0; k < 4; k++) {
                AT(rgba[k], 0, u32) = rgbaWord;
            }
            for (f = 0; f < 6; f++) {
                f32 xyzw[4][4] __attribute__((aligned(16)));
                f32 st[4][2];

                for (k = 0; k < 4; k++) {
                    sceVu0CopyVector(xyzw[k], world[kShardFace[f][k]]);
                    AT(&xyzw[k][3], 0, u32) = 0;
                    st[k][0] = k & 1 ? AT(e, fb, f32) + AT(e, fb + 0x8, f32) : AT(e, fb, f32);
                    st[k][1] = k & 2 ? AT(e, fb + 0x4, f32) + AT(e, fb + 0xC, f32) : AT(e, fb + 0x4, f32);
                }
                glr_strip(&clip[0][0], 4, &xyzw[0][0], &st[0][0], &rgba[0][0], tex, tex0, prim);
            }
        }
    }
}

/* +0x14 draw: the shards (texture +0xE34, its cell +0xE20 / +0xE24, size +0xE28 / +0xE2C,
 * colour +0xE30) */
/* 0x002EA660 */
void Effect6FF60_Draw(u8 *e) {
    shards_draw(e, 0xE20, 16, sizeof(Shard), 1);
}
#endif

/* ---- D_004799D0 (0xD40 bytes): debris - 16 pieces (Shards without the alive flag, 0xD0 apart
 * from +0x10) dropped in a 4 x 4 grid, turned by +0xD38, from about a point (+0xD10) up near the
 * ceiling; they tumble and fall (the bigger, the faster) until the last one passes 500. Start
 * kinds 0..2 three spots (3..5 the same with pieces a tenth the size, +0xD3C) ---- */

#define DEBRIS(e, i) ((Shard *)((u8 *)(e) + 0x10 + (i) * 0xD0))

/* throw piece `i`: a random box (each coordinate 0.7 or -0.3 by its sign, less 0.4 x random),
 * size 0.5..3.5, turn -pi..pi, its grid spot (column i % 4, row i / 4) jittered, spin, and a
 * small push along the heading */
void func_0035D840(u8 *e, s32 i) {
    static const union { u32 u; f32 f; } kPos = {0x3F333333}, kNeg = {0xBE99999A}, k04 = {0x3ECCCCCD},
                                          kPi = {0x40490FDB}, kCol = {0x40551EB8}, k02 = {0x3E4CCCCD},
                                          k01 = {0x3DCCCCCD};
    static const s8 sx[8] = {1, -1, 1, -1, 1, -1, 1, -1};
    static const s8 sy[8] = {1, 1, 1, 1, -1, -1, -1, -1};
    static const s8 sz[8] = {1, 1, -1, -1, 1, 1, -1, -1};
    VObject *rnd = gRandom;
    Shard *s = DEBRIS(e, i);
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    s32 k;

    for (k = 0; k < 8; k++) {
        s->corner[k][0] = (sx[k] > 0 ? kPos.f : kNeg.f) - k04.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        s->corner[k][1] = (sy[k] > 0 ? kPos.f : kNeg.f) - k04.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        s->corner[k][2] = (sz[k] > 0 ? kPos.f : kNeg.f) - k04.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        s->corner[k][3] = 1.0f;
    }
    for (k = 0; k < 3; k++) {
        s->scale[k] = 0.5f + 3.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    }
    if (AT(e, 0xD3C, u8) == 1) {
        s->scale[0] *= k01.f;
        s->scale[1] *= k01.f;
        s->scale[2] *= k01.f;
    }
    rnd = gRandom;
    for (k = 0; k < 3; k++) {
        s->rot[k] = kPi.f - 2.0f * (kPi.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd));
    }
    v[0] = -5.0f + kCol.f * (f32)(i % 4);
    v[1] = 1.0f + 2.0f * (f32)(i / 4);
    v[0] = v[0] + (2.0f - 4.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd));
    v[1] = v[1] + (1.0f - 2.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd));
    v[2] = 1.0f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    v[3] = 1.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, AT(e, 0xD38, f32));
    sceVu0ApplyMatrix(v, m, v);
    s->pos[0] = AT(e, 0xD10, f32) + v[0];
    s->pos[1] = AT(e, 0xD14, f32) + v[1];
    s->pos[2] = AT(e, 0xD18, f32) + v[2];
    s->pos[3] = 1.0f;
    for (k = 0; k < 3; k++) {
        s->spin[k] = 5.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    }
    v[0] = 0.0f;
    v[1] = k02.f + k02.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    v[3] = 0.0f;
    v[2] = k02.f + k04.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    sceVu0ApplyMatrix(v, m, v);
    sceVu0CopyVector(s->vel, v);
}

/* +0x18 start (arg: the kind): its spot (+0xD10) and heading (+0xD38), the texture's cell
 * (0, 0.32) / size 0.68 / colour / texture 4, the small-piece flag, then all 16 thrown */
/* 0x0035E2C0 */
void Debris_SetParams(u8 *e, u8 *arg) {
    s32 i;

    if (arg == NULL) {
        return;
    }
    switch (arg[0]) {
    case 0:
    case 3:
        AT(e, 0xD10, u32) = 0x3FC00000;   /* 1.5 */
        AT(e, 0xD14, u32) = 0x441B0000;   /* 620 */
        AT(e, 0xD18, u32) = 0x429A0000;   /* 77 */
        AT(e, 0xD1C, u32) = 0x3F800000;
        AT(e, 0xD38, s32) = 0;
        break;
    case 1:
    case 4:
        AT(e, 0xD10, u32) = 0xC2620000;   /* -56.5 */
        AT(e, 0xD14, u32) = 0x441B0000;
        AT(e, 0xD18, u32) = 0x42500000;   /* 52 */
        AT(e, 0xD1C, u32) = 0x3F800000;
        AT(e, 0xD38, u32) = 0xBF29C91F;   /* -0.663 */
        break;
    case 2:
    case 5:
        AT(e, 0xD10, u32) = 0xC0A00000;   /* -5 */
        AT(e, 0xD14, u32) = 0x441B0000;
        AT(e, 0xD18, u32) = 0xC29A0000;   /* -77 */
        AT(e, 0xD38, u32) = 0x40490FDB;   /* pi */
        AT(e, 0xD1C, u32) = 0x3F800000;
        break;
    }
    AT(e, 0xD20, s32) = 0;
    AT(e, 0xD24, u32) = 0x3EA3D70A;   /* 0.32 */
    AT(e, 0xD28, u32) = 0x3F2E147B;   /* 0.68 */
    AT(e, 0xD2C, u32) = 0x3F2E147B;
    AT(e, 0xD30, u32) = 0x80161414;
    AT(e, 0xD34, s32) = 4;
    AT(e, 0xD3C, u8) = arg[0] >= 3;
    for (i = 0; i < 16; i++) {
        func_0035D840(e, i);
    }
}

#ifdef HG_NATIVE
/* +0x14 draw (OpenGL) */
/* 0x0035E420 */
void Debris_Draw(u8 *e) {
    shards_draw(e, 0xD20, 16, 0xD0, 0);
}
#endif

/* +0x10 update: the pieces tumble and fall, the bigger the faster (each frame 0.01 x their
 * size's volume plus 0.09 more); 0 once the last one is below 500 */
/* 0x0035EF70 */
s32 Debris_Update(u8 *e) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k2Pi = {0x40C90FDB}, k001 = {0x3C23D70A},
                                          kFall = {0xBDB851EC};
    s32 on = 1, i, k;

    for (i = 0; i < 16; i++) {
        Shard *s = DEBRIS(e, i);

        on = 1;
        for (k = 0; k < 3; k++) {
            s->rot[k] += kPi.f * s->spin[k] / 180.0f;
            if (!(s->rot[k] <= kPi.f)) {
                s->rot[k] -= k2Pi.f;
            }
        }
        s->pos[0] += s->vel[0];
        s->pos[1] += s->vel[1];
        s->pos[2] += s->vel[2];
        s->vel[1] = s->vel[1] + k001.f * (s->scale[2] * (-s->scale[0] * s->scale[1])) + kFall.f;
        if (s->pos[1] < 500.0f) {
            on = 0;
        }
    }
    return on != 0;
}

/* ---- D_00479B00 (0x1C30 bytes): falling stones - 32 pieces (Shards, 0xE0 apart from +0x10)
 * dropped from a height (+0x1C10) onto random floor triangles, each after its own wait, some
 * already partway down at the start; they tumble, fall (the bigger the faster), bounce once off
 * floors that allow it and fade out on the ground, then start over. +0x1C14 .. +0x1C20 the
 * texture's cell and size, +0x1C24 the colour, +0x1C28 the texture ---- */

/* (re)start piece `s`: on a random nav triangle's centre (its floor kept), at the drop height,
 * a random box (0.6 / -0.4 by the corner's signs, less 0.2 x random), size 0.1..0.5, a random
 * turn and spin; waiting up to a second, or (at the start, `first`, 4 in 10) already falling
 * from partway down */
void func_003619A0(u8 *e, Shard *s, s32 first) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k10 = {0x41200000}, k30 = {0x41F00000};   /* multiplied first */
    static const s8 sx[8] = {1, -1, 1, -1, 1, -1, 1, -1};
    static const s8 sy[8] = {1, 1, 1, 1, -1, -1, -1, -1};
    static const s8 sz[8] = {1, 1, -1, -1, 1, 1, -1, -1};
    f32 eye[4] __attribute__((aligned(16)));
    VObject *rnd, *nav;
    u32 tri;
    s32 k;

    s->landed = 0;
    s->alpha = 0x80;
    s->vel[0] = 0.0f;
    s->vel[1] = 0.0f;
    s->vel[2] = 0.0f;
    s->vel[3] = 0.0f;
    VCALL(gCamera, 0x20, void (*)(VObject *, f32 *))(gCamera, eye);
    rnd = gRandom;
    nav = (VObject *)gNavMesh;
    tri = (u32)((f32)AT(nav, 0x8, s32) * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd));
    VCALL(nav, 0xC, void (*)(VObject *, u32, f32 *))(nav, tri, s->pos);
    s->floor = s->pos[1];
    s->pos[1] = AT(e, 0x1C10, f32);
    if (first) {
        if (VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd) < 0x1.333334p-1f /* 0.6 */) {
            f32 h;

            s->delay = 0;
            rnd = gRandom;
            h = AT(e, 0x1C10, f32) - s->floor;
            s->pos[1] = s->pos[1] - h * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
            s->vel[1] = -1.0f - 2.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        } else {
            s->delay = (s32)(k30.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd));
        }
    } else {
        s->delay = (s32)(k30.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd));
    }
    {
        u8 *tris = AT(nav, 0x4, u8 *);
        u32 fl = (tri < AT(nav, 0x8, u32) && tris != NULL) ? AT(tris + tri * 0x50, 0x3C, u32) : 0;

        s->bounce = !(fl & 0x10000000);
    }
    rnd = gRandom;
    for (k = 0; k < 8; k++) {
        s->corner[k][0] = (sx[k] > 0 ? 0x1.333334p-1f : -0x1.99999ap-2f) - 0x1.99999ap-3f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        s->corner[k][1] = (sy[k] > 0 ? 0x1.333334p-1f : -0x1.99999ap-2f) - 0x1.99999ap-3f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        s->corner[k][2] = (sz[k] > 0 ? 0x1.333334p-1f : -0x1.99999ap-2f) - 0x1.99999ap-3f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        s->corner[k][3] = 1.0f;
    }
    for (k = 0; k < 3; k++) {
        s->scale[k] = 0x1.99999ap-4f + 0x1.99999ap-2f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);   /* 0.1 + 0.4 x */
    }
    for (k = 0; k < 3; k++) {
        s->rot[k] = kPi.f - 2.0f * (kPi.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd));
    }
    for (k = 0; k < 3; k++) {
        s->spin[k] = k10.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    }
}

/* +0x18 start: arg { the drop height, the texture's cell u / v, size w / h (floats), colour,
 * +0x18 the texture (u16) }; all 32 started */
/* 0x00362400 */
void Effect79B00_SetParams(u8 *e, u8 *arg) {
    s32 i;

    if (arg == NULL) {
        return;
    }
    AT(e, 0x1C10, f32) = AT(arg, 0x0, f32);
    AT(e, 0x1C14, f32) = AT(arg, 0x4, f32);
    AT(e, 0x1C18, f32) = AT(arg, 0x8, f32);
    AT(e, 0x1C1C, f32) = AT(arg, 0xC, f32);
    AT(e, 0x1C20, f32) = AT(arg, 0x10, f32);
    AT(e, 0x1C24, u32) = AT(arg, 0x14, u32);
    AT(e, 0x1C28, s32) = AT(arg, 0x18, u16);
    for (i = 0; i < 32; i++) {
        func_003619A0(e, SHARD(e, i), 1);
    }
}

#ifdef HG_NATIVE
/* +0x14 draw (OpenGL) */
/* 0x003624A0 */
void Effect79B00_Draw(u8 *e) {
    shards_draw(e, 0x1C14, 32, sizeof(Shard), 2);
}
#endif

/* +0x10 update: each piece waiting counts down; else it tumbles, moves and falls (0.1 + 0.4 x
 * its volume more a frame); on its floor it lands - a bouncing one thrown back up at a fifth
 * of its fall, tilted up to 54 degrees off upright in a random direction; a landed one fades
 * by 8 and starts over once gone. Always 1 */
/* 0x00363130 */
s32 Effect79B00_Update(u8 *e) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k2Pi = {0x40C90FDB}, k005 = {0x3D4CCCCD},
                                          k02 = {0x3E4CCCCD};   /* multiplied first */
    VObject *rnd = gRandom;
    s32 i, k;

    for (i = 0; i < 32; i++) {
        Shard *s = SHARD(e, i);

        if (s->delay != 0) {
            s->delay--;
            continue;
        }
        for (k = 0; k < 3; k++) {
            s->rot[k] = s->rot[k] + kPi.f * s->spin[k] / 180.0f;
            if (!(s->rot[k] <= kPi.f)) {
                s->rot[k] = s->rot[k] - k2Pi.f;
            }
        }
        s->pos[0] = s->pos[0] + s->vel[0];
        s->pos[1] = s->pos[1] + s->vel[1];
        s->pos[2] = s->pos[2] + s->vel[2];
        s->vel[1] = s->vel[1] - (0x1.99999ap-4f + 8.0f * (s->scale[2] * ((k005.f * s->scale[0]) * s->scale[1])));
        if (s->landed) {
            if (s->alpha < 8) {
                func_003619A0(e, s, 0);
            } else {
                s->alpha -= 8;
            }
            continue;
        }
        if (s->pos[1] < s->floor) {
            s->landed = 1;
            if (s->bounce == 1) {
                f32 m[4][4] __attribute__((aligned(16)));
                f32 v[4] __attribute__((aligned(16)));
                f32 up;

                s->pos[1] = s->floor;
                v[0] = 0.0f;
                up = s->vel[1];
                if (up <= 0.0f) {
                    up = -up;
                }
                v[2] = 0.0f;
                v[3] = 0.0f;
                v[1] = k02.f * up;
                sceVu0UnitMatrix(m);
                sceVu0RotMatrixX(m, m, 0x1.e28c76p-1f /* 0.3 pi */ - 0x1.333334p-1f * (kPi.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)));
                sceVu0ApplyMatrix(v, m, v);
                sceVu0UnitMatrix(m);
                sceVu0RotMatrixY(m, m, kPi.f - 2.0f * (kPi.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)));
                sceVu0ApplyMatrix(v, m, v);
                s->vel[0] = v[0];
                s->vel[1] = v[1];
                s->vel[2] = v[2];
            }
        }
    }
    return 1;
}

/* 0x003634F0 */
Character *Kind39_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00479B20); }

/* 0x00363600 */
void *Kind39_MotionFiles(void) {
    return D_00444B10;
}

/* ---- D_00479600 (0x80 bytes): a strike's mark - between two points of a model (+0x50 from
 * its bone +0x98, +0x60 from its bone +0x9C; none: two points by Fiona's bone 0x23), a frame
 * (+0x10, its z axis from the second point to the first), drawn as a glow at the first point
 * and a streak 4 long beyond it; after 2 frames (+0x70) the model gives off smoke (D_00479800)
 * and the mark is done (+0x78). +0x74 the model ---- */

extern void *D_00479800[];

/* the two points: the model's bones, or Fiona's bone 0x23 at (-3.5, 0, 1) and (-1, 0, 1) */
static inline void mark_points(u8 *o) {
    u8 *c = AT(o, 0x74, u8 *);

    if (c != NULL) {
        s32 bone = VCALL(c, 0x98, s32 (*)(u8 *))(c);

        sceVu0CopyVector((f32 *)(o + 0x50), Skel_Bone(AT(c, 0x810, void *), bone) + 12);
        bone = VCALL(c, 0x9C, s32 (*)(u8 *))(c);
        sceVu0CopyVector((f32 *)(o + 0x60), Skel_Bone(AT(c, 0x810, void *), bone) + 12);
    } else {
        f32 m[4][4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));

        sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(AT(AT(gCharPlayer, 0xF0, u8 *), 0x810, void *), 0x23));
        v[0] = -3.5f;
        v[2] = 1.0f;
        v[3] = 1.0f;
        v[1] = 0.0f;
        sceVu0ApplyMatrix((f32 *)(o + 0x50), m, v);
        v[0] = -1.0f;
        v[2] = 1.0f;
        v[3] = 1.0f;
        v[1] = 0.0f;
        sceVu0ApplyMatrix((f32 *)(o + 0x60), m, v);
    }
}

/* +0x18 start: arg { 0xFF and a model; or: no game - done; non-zero - the stalker's model;
 * zero - Fiona }: the points and the frame (y up, or z when the line is vertical) */
/* 0x0035A290 */
void StrikeMark_SetParams(u8 *o, s32 *arg) {
    f32 *m = (f32 *)(o + 0x10);

    AT(o, 0x78, u8) = 0;
    if (arg[0] == 0xFF) {
        if (arg[1] == 0) {
            AT(o, 0x78, u8) = 1;
        } else {
            AT(o, 0x74, s32) = arg[1];
        }
    } else if (gProgress == NULL) {
        AT(o, 0x78, u8) = 1;
    } else if (arg[0] != 0) {
        AT(o, 0x74, u32) = AT(gCharSlot2, 0xF0, u32);
    } else {
        AT(o, 0x74, s32) = 0;
    }
    if (AT(o, 0x78, u8)) {
        return;
    }
    mark_points(o);
    sceVu0UnitMatrix((f32 (*)[4])m);
    sceVu0SubVector(m + 8, (f32 *)(o + 0x50), (f32 *)(o + 0x60));
    sceVu0Normalize(m + 8, m + 8);
    if (m[9] < 1.0f && !(m[9] <= -1.0f)) {
        m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f;
    } else {
        m[4] = 0.0f;
        m[5] = 0.0f;
        m[6] = 1.0f;
    }
    m[7] = 0.0f;
    sceVu0OuterProduct(m, m + 4, m + 8);
    sceVu0Normalize(m, m);
    sceVu0OuterProduct(m + 4, m + 8, m);
    sceVu0Normalize(m + 4, m + 4);
}

static inline void mark_smoke_init(void **obj) {
    obj[0] = D_00479800;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* +0x10 update: 0 once done; after the wait the smoke from the model, and done */
/* 0x0035AD40 */
s32 StrikeMark_Update(u8 *o) {
    u8 *mgr;

    if (AT(o, 0x78, u8) == 1) {
        return 0;
    }
    AT(o, 0x70, s32)--;
    if (AT(o, 0x70, s32) > 0) {
        return 1;
    }
    mgr = gEffects;
    EffectMgr_Start(mgr, Effect_New(mgr, 0x6E0, mark_smoke_init), AT(o, 0x74, void *));
    return 0;
}

#ifdef HG_NATIVE
/* +0x14 draw (not while the effects are paused or once done): in the mark's frame a glow at the
 * first point (a 2 x 2 quad across x / y, cell (320, 128) 32 x 32, additive) and the streak -
 * two quads 2 wide from 0.2 behind to 3.8 beyond it, one across y, one across x (cell (256,
 * 128) 64 x 32) - then, with the first point in view, a faint grey band over the screen's
 * bottom 64 lines at its depth (the original's eight 64-wide scissored sprites, layer 0x1E) */
/* 0x0035A4F0 */
void StrikeMark_Draw(u8 *o) {
    static const f32 kGlow[4][4] = {{-1.0f, 1.0f, 0.3f, 1.0f}, {1.0f, 1.0f, 0.3f, 1.0f},
                                    {-1.0f, -1.0f, 0.3f, 1.0f}, {1.0f, -1.0f, 0.3f, 1.0f}};
    static const f32 kStreakY[4][4] = {{0.0f, 1.0f, -0.2f, 1.0f}, {0.0f, 1.0f, 3.8f, 1.0f},
                                       {0.0f, -1.0f, -0.2f, 1.0f}, {0.0f, -1.0f, 3.8f, 1.0f}};
    static const f32 kStreakX[4][4] = {{1.0f, 0.0f, -0.2f, 1.0f}, {1.0f, 0.0f, 3.8f, 1.0f},
                                       {-1.0f, 0.0f, -0.2f, 1.0f}, {-1.0f, 0.0f, 3.8f, 1.0f}};
    QuadDrawer d;
    QuadRec rec __attribute__((aligned(16)));
    f32 corner[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 c[4] __attribute__((aligned(16)));
    s32 k;

    if (func_002D6010(gEffects) != 0 || AT(o, 0x78, u8) == 1) {
        return;
    }
    rec.rgba[0] = 0x80;
    rec.rgba[1] = 0x46;
    rec.rgba[2] = 0x46;
    rec.rgba[3] = 0x80;
    sceVu0CopyVector(rec.pos, (f32 *)(o + 0x50));
    rec.w = 1.0f;
    rec.h = 1.0f;
    rec.turn = 0.0f;
    rec.frame = 0;
    d.vtbl = D_0046FC30;
    d.a = -1;
    d.tex = (u64)-1;
    d.rec = &rec;
    AT(&d, 0x14, f32 *) = corner[0];
    d.cx = 0.0f;
    d.cy = 0.0f;
    d.layer = 0x19;
    d.count = 1;
    d.cellX = 0x140;
    d.cellY = 0x80;
    d.cellW = 0x20;
    d.cellH = 0x20;
    d.texW = 0x200;
    d.texH = 0x100;
    d.flags = 0x42;
    d.frames = 1;
    d.texId = 1;
    d.texGroup = 0x10;
    d.palette = -1;
    for (k = 0; k < 4; k++) {
        sceVu0ApplyMatrix(corner[k], (f32 (*)[4])(o + 0x10), kGlow[k]);
    }
    func_002E56C0((u8 *)&d);
    d.cellX = 0x100;
    d.cellY = 0x80;
    d.cellW = 0x40;
    d.cellH = 0x20;
    for (k = 0; k < 4; k++) {
        sceVu0ApplyMatrix(corner[k], (f32 (*)[4])(o + 0x10), kStreakY[k]);
    }
    func_002E56C0((u8 *)&d);
    for (k = 0; k < 4; k++) {
        sceVu0ApplyMatrix(corner[k], (f32 (*)[4])(o + 0x10), kStreakX[k]);
    }
    func_002E56C0((u8 *)&d);
    d.vtbl = D_00469D00;

    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
    sceVu0ApplyMatrix(c, clip, (f32 *)(o + 0x50));
    if (!(c[0] <= c[3]) || c[0] < -c[3] || !(c[1] <= c[3]) || c[1] < -c[3] || !(c[2] <= c[3]) || c[2] < -c[3]) {
        return;
    }
    {
        /* the band in PS2 clip space (w 1) at the point's depth: GS pixel (x, y) is clip
         * (x - 256, y - 224) / 2047 */
        static const f32 kIdentity[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        static const f32 kX[2] = {0.0f, 512.0f}, kY[2] = {384.0f, 448.0f};
        f32 xyzw[4][4];
        f32 st[4][2] = {{0}};
        u8 col[4][4];

        for (k = 0; k < 4; k++) {
            xyzw[k][0] = (kX[k & 1] - 256.0f) / 2047.0f;
            xyzw[k][1] = (kY[k >> 1] - 224.0f) / 2047.0f;
            xyzw[k][2] = c[2] / c[3];
            AT(&xyzw[k][3], 0, u32) = 0;
            col[k][0] = col[k][1] = col[k][2] = 0x80;
            col[k][3] = 8;
        }
        glr_layer(0x1E);
        glr_strip(kIdentity, 4, &xyzw[0][0], &st[0][0], &col[0][0], NULL, 0, 0x40 | 0x20000);   /* blended, no z writes */
        glr_layer(-1);
    }
}
#endif

/* ---- D_00470A50 (0x3858 bytes): 128 motes rising from around (0, 70, 35), in two buffers of
 * quad records (+0x10 + 0x1800 x the current one +0x3850), their velocities at +0x3050 (16
 * each), drawn by the quad drawer at +0x3010; a mote is renewed past height 130 or, counted on
 * the even frames, when it has faded out ---- */

extern void *D_00470A50[];

#define MOTE_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x1800) + (i))
#define MOTE_VEL(e, i) ((f32 *)((e) + 0x3050 + (i) * 0x10))

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x002F9210 */
u8 *RisingMotes_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00470A50;
    AT(o, 0x3010, void **) = D_0046FC30;
    AT(o, 0x3010, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* mote i anew: at the bottom (again), or at first somewhere up the column, bigger and fainter
 * the higher */
void func_002F92A0(u8 *e, s32 i, s32 again) {
    VObject *rnd = gRandom;
    QuadRec *r = MOTE_REC(e, AT(e, 0x3850, s32), i);
    f32 *v = MOTE_VEL(e, i);
    s32 up = 0;

    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = 0x38;
    r->pos[0] = 4.0f * (burst_rnd(rnd) - 0.5f);
    r->w = 1.0f;
    r->h = 1.0f;
    if (!again) {
        up = (s32)(60.0f * burst_rnd(rnd));
        r->rgba[3] = r->rgba[3] - (up >> 1);
        if (r->rgba[3] < 0) {
            r->rgba[3] = 0;
        }
        r->w = 1.0f + 0x1.47ae140000000p-5f /* 0.04 */ * (f32)up;
        r->h = r->w;
    }
    rnd = gRandom;
    r->pos[1] = 70.0f + (f32)up;
    r->pos[2] = 35.0f + 4.0f * (burst_rnd(rnd) - 0.5f);
    r->pos[3] = 1.0f;
    r->turn = 0.0f;
    r->frame = 0;
    v[0] = 0x1.eb851e0000000p-6f /* 0.03 */ * (burst_rnd(rnd) - 0.5f);
    v[1] = 0x1.1eb8520000000p-4f /* 0.07 */ + 0x1.99999a0000000p-2f /* 0.4 */ * burst_rnd(rnd);
    v[2] = 0x1.eb851e0000000p-6f /* 0.03 */ * (burst_rnd(rnd) - 0.5f);
}

/* +0x14 draw the current buffer */
/* 0x002F9520 */
void RisingMotes_Draw(u8 *e) {
    AT(e, 0x3020, QuadRec *) = MOTE_REC(e, AT(e, 0x3850, s32), 0);
    func_002E56C0(e + 0x3010);
}

/* +0x10 update: flip the buffers, each mote carried over, growing, turning (3 degrees) and
 * moving */
/* 0x002F9550 */
s32 RisingMotes_Update(u8 *e) {
    s32 i, k;

    AT(e, 0x3850, s32) ^= 1;
    for (i = 0; i < 128; i++) {
        u32 *src = (u32 *)MOTE_REC(e, AT(e, 0x3850, s32) ^ 1, i);
        u32 *dst = (u32 *)MOTE_REC(e, AT(e, 0x3850, s32), i);
        QuadRec *r;
        f32 *v = MOTE_VEL(e, i);

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = MOTE_REC(e, AT(e, 0x3850, s32), i);
        r->w = r->w + 0x1.47ae140000000p-5f /* 0.04 */;
        r->h = r->h + 0x1.47ae140000000p-5f /* 0.04 */;
        r->turn = r->turn + 0x1.aceeap-5f /* 3 degrees */;
        if (!(r->turn <= 0x1.921fb6p+1f)) {
            r->turn = r->turn - 0x1.921fb6p+2f;
        }
        r->pos[0] = r->pos[0] + v[0];
        r->pos[1] = r->pos[1] + v[1];
        r->pos[2] = r->pos[2] + v[2];
        if (!(r->pos[1] < 130.0f)) {
            func_002F92A0(e, i, 1);
        }
        if (AT(e, 0x3850, s32) == 0) {
            if (r->rgba[3] > 0) {
                r->rgba[3] -= 1;
            } else {
                func_002F92A0(e, i, 1);
            }
        }
    }
    return 1;
}

/* +0xC set up: the drawer's settings (a 16-frame strip of 32 x 32 cells, row 64, layer 0x19),
 * every mote placed */
/* 0x002F9750 */
void RisingMotes_Start(u8 *e) {
    s32 i;

    AT(e, 0x3850, s32) = 0;
    AT(e, 0x3018, s64) = -1;
    AT(e, 0x3024, s32) = 0;
    AT(e, 0x3028, s32) = 0;
    AT(e, 0x302C, s32) = 0;
    AT(e, 0x3030, s32) = 0x19;
    AT(e, 0x3034, s16) = 0x80;
    AT(e, 0x3036, s16) = 0;
    AT(e, 0x3038, s16) = 0x40;
    AT(e, 0x303A, s16) = 0x20;
    AT(e, 0x303C, s16) = 0x20;
    AT(e, 0x303E, s16) = 0x200;
    AT(e, 0x3040, s16) = 0x100;
    AT(e, 0x3042, s8) = 0;
    AT(e, 0x3043, s8) = 1;
    AT(e, 0x3044, s8) = 1;
    AT(e, 0x3045, s8) = 0x10;
    AT(e, 0x3046, s8) = -1;
    for (i = 0; i < 128; i++) {
        func_002F92A0(e, i, 0);
    }
}

/* ---- D_00470A70 (0xE58 bytes): 32 orange sparks drifting up from around (276.5, 6, -145.9),
 * in two buffers of quad records (+0x10 + 0x600 x the current one +0xE50), velocities at +0xC50
 * (16 each), the quad drawer (additive) at +0xC10; a spark is renewed when it fades, falls back
 * or reaches height 50 - unless the burst is ending (+0xE54), when it just goes out ---- */

extern void *D_00470A70[];

#define SPARK_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x600) + (i))
#define SPARK_VEL(e, i) ((f32 *)((e) + 0xC50 + (i) * 0x10))

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x002F9810 */
u8 *OrangeSparks_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00470A70;
    AT(o, 0xC10, void **) = D_0046FC30;
    AT(o, 0xC10, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* spark i anew: at the bottom (again), or at first somewhere up, fainter and slower the
 * higher */
void func_002F98A0(u8 *e, s32 i, s32 again) {
    VObject *rnd = gRandom;
    f32 *v = SPARK_VEL(e, i);
    QuadRec *r;
    s32 up = 0;

    v[0] = 0x1.99999a0000000p-4f /* 0.1 */ * (burst_rnd(rnd) - 0.5f);
    v[1] = 0x1.99999a0000000p-5f /* 0.05 */ + 0.5f * burst_rnd(rnd);
    v[2] = 0x1.99999a0000000p-4f /* 0.1 */ * (burst_rnd(rnd) - 0.5f);
    r = SPARK_REC(e, AT(e, 0xE50, s32), i);
    r->rgba[0] = 0x80;
    r->rgba[1] = 0x58;
    r->rgba[2] = 0x10;
    r->rgba[3] = 0x80;
    r->pos[0] = 276.5f + 4.0f * (burst_rnd(rnd) - 0.5f);
    if (!again) {
        up = (s32)(10.0f * burst_rnd(rnd));
        r->rgba[3] = r->rgba[3] - (up << 4);
        if (r->rgba[3] < 0) {
            r->rgba[3] = 0;
        }
        v[1] = v[1] + -0x1.47ae140000000p-7f /* 0.01 */ * (f32)up;
        if (v[1] < 0.0f) {
            v[1] = 0x1.99999a0000000p-4f /* 0.1 */;
        }
    }
    r->pos[1] = 6.0f + (f32)up;
    r->pos[2] = -0x1.23cccc0000000p+7f /* 145.9 */ + 4.0f * (burst_rnd(rnd) - 0.5f);
    r->pos[3] = 1.0f;
    r->w = 1.0f;
    r->h = 1.0f;
    r->turn = 0.0f;
    r->frame = 0;
}

/* +0x18 a gust (arg: ending): blown back (x -0.2, z -0.3 at least) and up, or flung away as
 * it ends */
/* 0x002F9B40 */
void OrangeSparks_SetParams(u8 *e, s32 *arg) {
    s32 i;

    AT(e, 0xE54, s32) = *arg;
    if (AT(e, 0xE54, s32) == 0) {
        for (i = 0; i < 32; i++) {
            f32 *v = SPARK_VEL(e, i);

            v[0] = v[0] - 0x1.99999a0000000p-3f /* 0.2 */;
            if (v[0] < -0x1.99999a0000000p-3f /* 0.2 */) {
                v[0] = -0x1.99999a0000000p-3f /* 0.2 */;
            }
            v[1] = v[1] - -0x1.47ae140000000p-7f /* 0.01 */;
            if (v[1] < 0x1.99999a0000000p-4f /* 0.1 */) {
                v[1] = 0x1.99999a0000000p-4f /* 0.1 */;
            }
            v[2] = v[2] - 0x1.3333340000000p-2f /* 0.3 */;
            if (v[2] < -0x1.3333340000000p-2f /* 0.3 */) {
                v[2] = -0x1.3333340000000p-2f /* 0.3 */;
            }
        }
    } else {
        for (i = 0; i < 32; i++) {
            f32 *v = SPARK_VEL(e, i);

            v[0] = v[0] * 2.0f;
            v[1] = v[1] - -0x1.47ae140000000p-7f /* 0.01 */;
            v[2] = v[2] * 3.0f;
        }
    }
}

/* +0x14 draw the current buffer */
/* 0x002F9D80 */
void OrangeSparks_Draw(u8 *e) {
    AT(e, 0xC20, QuadRec *) = SPARK_REC(e, AT(e, 0xE50, s32), 0);
    func_002E56C0(e + 0xC10);
}

/* spark i spent: out when ending, else renewed */
static s32 spark_spent(u8 *e, QuadRec *r, s32 i) {
    if (AT(e, 0xE54, s32) != 0) {
        r->rgba[3] = 0;
        return 0;
    }
    func_002F98A0(e, i, 1);
    return 1;
}

/* +0x10 update: flip the buffers, each spark carried over, drifting (x / z +0.01, y -0.01 a
 * frame) and fading; 0 when all are out */
/* 0x002F9DB0 */
s32 OrangeSparks_Update(u8 *e) {
    s32 done = 1;
    s32 i, k;

    AT(e, 0xE50, s32) ^= 1;
    for (i = 0; i < 32; i++) {
        u32 *src = (u32 *)SPARK_REC(e, AT(e, 0xE50, s32) ^ 1, i);
        u32 *dst = (u32 *)SPARK_REC(e, AT(e, 0xE50, s32), i);
        f32 *v = SPARK_VEL(e, i);
        QuadRec *r;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        v[0] = v[0] + 0x1.47ae140000000p-7f /* 0.01 */;
        r = SPARK_REC(e, AT(e, 0xE50, s32), i);
        v[1] = v[1] + -0x1.47ae140000000p-7f /* 0.01 */;
        if (v[1] < 0.0f && spark_spent(e, r, i)) {
            done = 0;
        }
        v[2] = v[2] + 0x1.47ae140000000p-7f /* 0.01 */;
        r->pos[0] = r->pos[0] + v[0];
        r->pos[1] = r->pos[1] + v[1];
        r->pos[2] = r->pos[2] + v[2];
        if (!(r->pos[1] < 50.0f) && spark_spent(e, r, i)) {
            done = 0;
        }
        if (r->rgba[3] > 0) {
            done = 0;
            r->rgba[3] -= 4;
        } else if (spark_spent(e, r, i)) {
            done = 0;
        }
    }
    return !done;
}

/* +0xC set up: the drawer's settings (additive; a 16-frame strip of 32 x 32 cells at (96, 64),
 * layer 0x19), every spark placed */
/* 0x002F9FF0 */
void OrangeSparks_Start(u8 *e) {
    s32 i;

    AT(e, 0xE50, s32) = 0;
    AT(e, 0xE54, s32) = 0;
    AT(e, 0xC18, s64) = -1;
    AT(e, 0xC24, s32) = 0;
    AT(e, 0xC28, s32) = 0;
    AT(e, 0xC2C, s32) = 0;
    AT(e, 0xC30, s32) = 0x19;
    AT(e, 0xC34, s16) = 0x20;
    AT(e, 0xC36, s16) = 0x60;
    AT(e, 0xC38, s16) = 0x40;
    AT(e, 0xC3A, s16) = 0x20;
    AT(e, 0xC3C, s16) = 0x20;
    AT(e, 0xC3E, s16) = 0x200;
    AT(e, 0xC40, s16) = 0x100;
    AT(e, 0xC42, s8) = 0x40;
    AT(e, 0xC43, s8) = 1;
    AT(e, 0xC44, s8) = 1;
    AT(e, 0xC45, s8) = 0x10;
    AT(e, 0xC46, s8) = -1;
    for (i = 0; i < 32; i++) {
        func_002F98A0(e, i, 0);
    }
}

/* 0x002FA1C0 */
void *Debilitas2_MotionFiles(void) {
    return D_0041B5F0;
}

/* ---- D_00470E00 (0xC0 bytes): one sprite (+0x10 + 0x30 x the current one +0xB8), slowly
 * growing and sinking, with its velocity at +0xA8 and the quad drawer at +0x70; each update
 * (only every other call, +0xBC) fades it a little ---- */

extern void *D_00470E00[];

#define ONE_REC(e, buf) ((QuadRec *)((e) + 0x10) + (buf))

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x002FCDC0 */
u8 *SinkingSprite_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00470E00;
    AT(o, 0x70, void **) = D_0046FC30;
    AT(o, 0x70, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* +0x18 start: arg { RGBA (4 x s32), position, velocity }, size 0.2 .. 0.4 */
/* 0x002FCE50 */
void SinkingSprite_SetParams(u8 *e, u8 *arg) {
    QuadRec *r;
    f32 s;

    if (arg == NULL) {
        return;
    }
    r = ONE_REC(e, AT(e, 0xB8, s32));
    r->rgba[0] = AT(arg, 0x0, s32);
    r->rgba[1] = AT(arg, 0x4, s32);
    r->rgba[2] = AT(arg, 0x8, s32);
    r->rgba[3] = AT(arg, 0xC, s32);
    r->pos[0] = AT(arg, 0x10, f32);
    r->pos[1] = AT(arg, 0x14, f32);
    r->pos[2] = AT(arg, 0x18, f32);
    r->pos[3] = 1.0f;
    s = 0x1.99999a0000000p-3f /* 0.2 */ + 0x1.99999a0000000p-3f /* 0.2 */ * burst_rnd(gRandom);
    r->w = s;
    r->h = s;
    r->turn = 0.0f;
    r->frame = 0;
    AT(e, 0xA8, f32) = AT(arg, 0x1C, f32);
    AT(e, 0xAC, f32) = AT(arg, 0x20, f32);
    AT(e, 0xB0, f32) = AT(arg, 0x24, f32);
}

/* +0x14 draw the current buffer */
/* 0x002FCF40 */
void SinkingSprite_Draw(u8 *e) {
    AT(e, 0x80, QuadRec *) = ONE_REC(e, AT(e, 0xB8, s32));
    func_002E56C0(e + 0x70);
}

/* +0x10 update: every other call 0 (gone); else carried over and, while visible, faded by
 * 0..3, falling faster (to about -0.05 a frame), moved and grown */
/* 0x002FCF70 */
s32 SinkingSprite_Update(u8 *e) {
    u32 *src, *dst;
    QuadRec *r;
    s32 k;

    if (AT(e, 0xBC, u8) == 1) {
        return 0;
    }
    AT(e, 0xBC, u8) = 1;
    AT(e, 0xB8, s32) ^= 1;
    src = (u32 *)ONE_REC(e, AT(e, 0xB8, s32) ^ 1);
    dst = (u32 *)ONE_REC(e, AT(e, 0xB8, s32));
    for (k = 0; k < 12; k++) {
        dst[k] = src[k];
    }
    r = ONE_REC(e, AT(e, 0xB8, s32));
    if (r->rgba[3] > 0) {
        VObject *rnd;

        AT(e, 0xBC, u8) = 0;
        rnd = gRandom;
        r->rgba[3] -= burst_int(rnd) & 3;
        if (r->rgba[3] < 0) {
            r->rgba[3] = 0;
        }
        AT(e, 0xAC, f32) = AT(e, 0xAC, f32) - 0x1.99999a0000000p-6f /* 0.025 */;
        if (AT(e, 0xAC, f32) < -0x1.99999a0000000p-5f /* 0.05 */) {
            AT(e, 0xAC, f32) = -0x1.99999a0000000p-5f /* 0.05 */ + 0x1.47ae140000000p-6f /* 0.02 */ * (burst_rnd(rnd) - 0.5f);
        }
        r->pos[0] = r->pos[0] + AT(e, 0xA8, f32);
        r->pos[1] = r->pos[1] + AT(e, 0xAC, f32);
        r->pos[2] = r->pos[2] + AT(e, 0xB0, f32);
        r->w = r->w + 0x1.0624de0000000p-10f /* 0.001 */;
        r->h = r->w;
    }
    return 1;
}

/* +0xC set up: the drawer's settings (one 32 x 32 cell at (64, 64), blended 0x20, layer
 * 0x19) */
/* 0x002FD150 */
void SinkingSprite_Start(u8 *e) {
    AT(e, 0xB8, s32) = 0;
    AT(e, 0xBC, u8) = 0;
    AT(e, 0x78, s64) = -1;
    AT(e, 0x84, s32) = 0;
    AT(e, 0x88, s32) = 0;
    AT(e, 0x8C, s32) = 0;
    AT(e, 0x90, s32) = 0x19;
    AT(e, 0x94, s16) = 1;
    AT(e, 0x96, s16) = 0x40;
    AT(e, 0x98, s16) = 0x40;
    AT(e, 0x9A, s16) = 0x20;
    AT(e, 0x9C, s16) = 0x20;
    AT(e, 0x9E, s16) = 0x200;
    AT(e, 0xA0, s16) = 0x100;
    AT(e, 0xA2, s8) = 0x20;
    AT(e, 0xA3, s8) = 1;
    AT(e, 0xA4, s8) = 1;
    AT(e, 0xA5, s8) = 0x10;
    AT(e, 0xA6, s8) = -1;
}

/* ---- D_0047A6D0 (as D_00470E00): one dust mote in the fog's colour, drifting along a heading
 * (+0xA8 / +0xAC), fading in to 0x40 over its life (+0xB0) and out again (+0xB4 its alpha),
 * fainter within 64 of the camera ---- */

/* +0xC set up: the drawer's settings (one 32 x 32 cell at (32, 64), blended 0x40, layer 0x19,
 * palette 2) */
/* 0x0037B920 */
void DustMote_Start(u8 *e) {
    AT(e, 0xB8, s32) = 0;
    AT(e, 0xBC, u8) = 0;
    AT(e, 0x78, s64) = -1;
    AT(e, 0x84, s32) = 0;
    AT(e, 0x88, s32) = 0;
    AT(e, 0x8C, s32) = 0;
    AT(e, 0x90, s32) = 0x19;
    AT(e, 0x94, s16) = 1;
    AT(e, 0x96, s16) = 0x20;
    AT(e, 0x98, s16) = 0x40;
    AT(e, 0x9A, s16) = 0x20;
    AT(e, 0x9C, s16) = 0x20;
    AT(e, 0x9E, s16) = 0x200;
    AT(e, 0xA0, s16) = 0x100;
    AT(e, 0xA2, s8) = 0x40;
    AT(e, 0xA3, s8) = 1;
    AT(e, 0xA4, s8) = 1;
    AT(e, 0xA5, s8) = 0x10;
    AT(e, 0xA6, s8) = 2;
}

/* +0x18 start: arg { position, heading (+0x10), already going (+0x14) } - or none: stopped. The
 * colour a quarter of the fog's (room effect 0x1D; else grey 0x10), placed within 5 of the
 * point, 10..20 wide and 5..7 high, moving 0.1 a frame along the heading, living 150..181
 * frames; one already going starts part way through its life */
/* 0x0037B370 */
void DustMote_SetParams(u8 *e, u8 *arg) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD};
    VObject *rnd;
    QuadRec *r;
    u8 *fog;

    if (arg == NULL) {
        AT(e, 0xBC, u8) = 1;
        return;
    }
    r = ONE_REC(e, AT(e, 0xB8, s32));
    fog = RoomEffects_Get(gRoomEffects, 0x1D);
    if (fog != NULL) {
        u32 c = AT(fog, 0x14, u32);

        r->rgba[0] = (s32)(c & 0xFF) >> 2;
        r->rgba[1] = (s32)(c & 0xFF00) >> 10;
        r->rgba[2] = (s32)(c & 0xFF0000) >> 18;
    } else {
        r->rgba[2] = 0x10;
        r->rgba[1] = 0x10;
        r->rgba[0] = 0x10;
    }
    r->rgba[3] = 0;
    sceVu0CopyVector(r->pos, (f32 *)arg);
    rnd = gRandom;
    r->pos[0] = r->pos[0] + 10.0f * (burst_rnd(rnd) - 0.5f);
    r->pos[2] = r->pos[2] + 10.0f * (burst_rnd(rnd) - 0.5f);
    r->w = 10.0f + 10.0f * burst_rnd(rnd);
    r->h = 5.0f + 2.0f * burst_rnd(rnd);
    r->turn = 0.0f;
    r->frame = 0;
    AT(e, 0xA8, f32) = k01.f * func_0031C248(AT(arg, 0x10, f32));
    AT(e, 0xAC, f32) = k01.f * func_0031C058(AT(arg, 0x10, f32));
    AT(e, 0xB0, s32) = (burst_int(rnd) & 0x1F) + 0x96;
    if (AT(arg, 0x14, s32) != 0) {
        f32 t = burst_rnd(rnd);
        s32 n;

        if (t < k01.f) {
            r->rgba[3] = (s32)(640.0f * t);
        } else {
            r->rgba[3] = 0x40;
        }
        n = (s32)((f32)AT(e, 0xB0, s32) * t);
        AT(e, 0xB0, s32) -= n;
        r->pos[0] = r->pos[0] + AT(e, 0xA8, f32) * (f32)(n + r->rgba[3] * 2);
        r->pos[2] = r->pos[2] + AT(e, 0xAC, f32) * (f32)(n + r->rgba[3] * 2);
    }
    AT(e, 0xB4, s32) = r->rgba[3];
}

/* +0x10 update (0 once gone): carried over and moved; fading in (by 0 / 1 a frame) to 0x40, then
 * living out its time, then fading out; fainter by the distance squared / 4096 within 64 of
 * the camera */
/* 0x0037B710 */
s32 DustMote_Update(u8 *e) {
    f32 eye[4] __attribute__((aligned(16)));
    u32 *src, *dst;
    QuadRec *r;
    f32 dx, dy, dz, d2;
    s32 k;

    if (AT(e, 0xBC, u8) == 1) {
        return 0;
    }
    AT(e, 0xB8, s32) ^= 1;
    src = (u32 *)ONE_REC(e, AT(e, 0xB8, s32) ^ 1);
    dst = (u32 *)ONE_REC(e, AT(e, 0xB8, s32));
    for (k = 0; k < 12; k++) {
        dst[k] = src[k];
    }
    r = ONE_REC(e, AT(e, 0xB8, s32));
    r->pos[0] = r->pos[0] + AT(e, 0xA8, f32);
    r->pos[2] = r->pos[2] + AT(e, 0xAC, f32);
    if (AT(e, 0xB0, s32) != 0) {
        if (AT(e, 0xB4, s32) < 0x40) {
            AT(e, 0xB4, s32) += burst_int(gRandom) & 1;
            if (AT(e, 0xB4, s32) >= 0x41) {
                AT(e, 0xB4, s32) = 0x40;
            }
        } else {
            AT(e, 0xB0, s32)--;
        }
    } else {
        AT(e, 0xB4, s32) -= burst_int(gRandom) & 1;
        if (AT(e, 0xB4, s32) <= 0) {
            return 0;
        }
    }
    r->rgba[3] = AT(e, 0xB4, s32);
    VCALL(gCamera, 0x20, void (*)(VObject *, f32 *))(gCamera, eye);
    dy = r->pos[1] - eye[1];
    dx = r->pos[0] - eye[0];
    dz = r->pos[2] - eye[2];
    d2 = dy * dy + dx * dx + dz * dz;
    if (d2 < 4096.0f) {
        r->rgba[3] = (s32)((f32)r->rgba[3] * (d2 / 4096.0f));
    }
    return 1;
}

/* +0x14 draw (not while stopped, nor during a cut: the director's +0x38) */
/* 0x0037B680 */
void DustMote_Draw(u8 *e) {
    if (AT(e, 0xBC, u8) == 1) {
        return;
    }
    if ((u8)VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) == 1) {
        return;
    }
    AT(e, 0x80, QuadRec *) = ONE_REC(e, AT(e, 0xB8, s32));
    func_002E56C0(e + 0x70);
}

/* ---- D_0047A6F0 (0x48 bytes): a source of dust motes (D_0047A6D0) at +0x30 heading +0x40 - eight
 * timers (+0x4) each starting a mote when it runs out (every 300 frames); stopped by +0x44 ---- */

extern void *D_0047A6D0[];

/* a mote's start arguments */
typedef struct {
    f32 pos[4];
    f32 heading;
    s32 going;   /* already part way through its life */
} MoteArgs;

static inline void Mote_Init(void **obj) {
    obj[0] = D_0047A6D0;
    obj[0x70 / 4] = D_00469D00;
    ((s32 *)obj)[0x74 / 4] = -1;
    obj[0x70 / 4] = D_0046FC30;
}

/* a mote started at the source */
static inline void mote_start(u8 *e, u8 *mgr, s32 going) {
    MoteArgs a __attribute__((aligned(16)));
    s32 slot = Effect_New(mgr, 0xC0, Mote_Init);

    sceVu0CopyVector(a.pos, (f32 *)(e + 0x30));
    a.heading = AT(e, 0x40, f32);
    a.going = going;
    EffectMgr_Start(mgr, slot, &a);
}

/* +0x18 start: arg { position, heading in degrees } (none: stopped); six motes at once (already
 * going), their timers and two more at random */
/* 0x0037BA00 */
void DustMoteSource_SetParams(u8 *e, f32 *arg) {
    u8 *mgr;
    VObject *rnd;
    s32 i;

    if (arg == NULL) {
        AT(e, 0x44, u8) = 1;
        return;
    }
    sceVu0CopyVector((f32 *)(e + 0x30), arg);
    AT(e, 0x40, f32) = Angle_Wrap(0x1.921fb6p+1f * arg[4] / 180.0f);
    rnd = gRandom;
    mgr = gEffects;
    for (i = 0; i < 8; i++) {
        AT(e, 0x4 + i * 4, s32) = (s32)(300.0f * burst_rnd(rnd));
        if (i < 6) {
            AT(e, 0x4 + i * 4, s32) = (s32)(300.0f * burst_rnd(rnd));
            mote_start(e, mgr, 1);
        } else {
            AT(e, 0x4 + i * 4, s32) = (s32)(30.0f * burst_rnd(rnd));
        }
    }
}

/* +0x10 update (0 once stopped): each timer counts down; run out, a new mote and 300 more */
/* 0x0037BCA0 */
s32 DustMoteSource_Update(u8 *e) {
    u8 *mgr;
    s32 i;

    if (AT(e, 0x44, u8) == 1) {
        return 0;
    }
    mgr = gEffects;
    for (i = 0; i < 8; i++) {
        if (AT(e, 0x4 + i * 4, s32) != 0) {
            AT(e, 0x4 + i * 4, s32)--;
        } else {
            AT(e, 0x4 + i * 4, s32) = 0x12C;
            mote_start(e, mgr, 0);
        }
    }
    return 1;
}

/* ---- D_0047A750 (0x6F8 bytes): a ring of light spreading over the ground from a point (+0x650),
 * its radius (+0x660) growing at a slowing speed (+0x664) and its alpha (+0x668) fading, its
 * width (+0x66C) following the alpha; with 16 sparks (quad records +0x10 + 0x300 x the current
 * one +0x6F0, the quad drawer at +0x610) spiralling (+0x6B0) up or down (+0x670); stopped (+0x6F4)
 * once nothing is left ---- */

#define RING_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x300) + (i))

/* +0xC set up: the drawer's settings (4 frames of 32 x 32 from (0x80, 0) of texture 0x10, layer
 * 0x19, blended 0x40) */
/* 0x0037E170 */
void LightRing_Start(u8 *e) {
    AT(e, 0x6F0, s32) = 0;
    AT(e, 0x6F4, u8) = 0;
    AT(e, 0x618, s64) = -1;
    AT(e, 0x624, s32) = 0;
    AT(e, 0x628, s32) = 0;
    AT(e, 0x62C, s32) = 0;
    AT(e, 0x630, s32) = 0x19;
    AT(e, 0x634, s16) = 0x10;
    AT(e, 0x636, s16) = 0x80;
    AT(e, 0x638, s16) = 0;
    AT(e, 0x63A, s16) = 0x20;
    AT(e, 0x63C, s16) = 0x20;
    AT(e, 0x63E, s16) = 0x200;
    AT(e, 0x640, s16) = 0x100;
    AT(e, 0x642, s8) = 0x40;
    AT(e, 0x643, s8) = 4;
    AT(e, 0x644, s8) = 1;
    AT(e, 0x645, s8) = 0x10;
    AT(e, 0x646, s8) = -1;
}

/* +0x18 start: arg { position, falling (+0x10) } (none: stopped). The ring at the point, radius
 * 3, alpha 0x80, speed 4 (falling: -1, else raised 5 first); the sparks around the point, 0.2..0.6
 * big, turned at random, spinning, 10..20 up and moving 0.1..0.35 a frame (down when falling) */
/* 0x0037D130 */
void LightRing_SetParams(u8 *e, f32 *arg) {
    static const union { u32 u; f32 f; } kFifth = {0x3E4CCCCD}, kTwoFifths = {0x3ECCCCCD}, kTenth = {0x3DCCCCCD},
        kQuarter = {0x3E800000};
    VObject *rnd;
    s32 i;

    if (arg == NULL) {
        AT(e, 0x6F4, u8) = 1;
        return;
    }
    sceVu0CopyVector((f32 *)(e + 0x650), arg);
    AT(e, 0x660, f32) = 3.0f;
    AT(e, 0x668, s32) = 0x80;
    AT(e, 0x66C, s32) = 0;
    if (AT(arg, 0x10, s32) != 0) {
        AT(e, 0x664, f32) = -1.0f;
    } else {
        AT(e, 0x654, f32) = AT(e, 0x654, f32) + 5.0f;
        AT(e, 0x664, f32) = 4.0f;
    }
    rnd = gRandom;
    for (i = 0; i < 16; i++) {
        QuadRec *r = RING_REC(e, AT(e, 0x6F0, s32), i);

        r->rgba[0] = 0x80;
        r->rgba[1] = 0x80;
        r->rgba[2] = 0x80;
        r->rgba[3] = burst_int(rnd) & 0x7F;
        sceVu0CopyVector(r->pos, arg);
        r->pos[0] = r->pos[0] + 2.0f * (burst_rnd(rnd) - 0.5f);
        r->pos[2] = r->pos[2] + 2.0f * (burst_rnd(rnd) - 0.5f);
        r->w = 0.0f + kFifth.f + kTwoFifths.f * burst_rnd(rnd);
        r->h = r->w;
        r->turn = 0x1.921fb6p+1f * (360.0f * (burst_rnd(rnd) - 0.5f)) / 180.0f;
        r->frame = burst_int(rnd) & 3;
        AT(e, 0x6B0 + i * 4, f32) = 0x1.921fb6p+1f * (360.0f * (burst_rnd(rnd) - 0.5f)) / 180.0f;
        r->pos[1] = r->pos[1] + (0.0f + 20.0f - 10.0f * burst_rnd(rnd));
        if (AT(arg, 0x10, s32) != 0) {
            AT(e, 0x670 + i * 4, f32) = -(0.0f + kTenth.f + kQuarter.f * burst_rnd(rnd));
        } else {
            AT(e, 0x670 + i * 4, f32) = 0.0f + kTenth.f + kQuarter.f * burst_rnd(rnd);
        }
    }
}

/* +0x10 update (0 once stopped): the ring spreads, slowing (by 2 to 0.2, then by 0.01 to 0.1) and
 * fading by 7; its width 7 / 4.5 / 3 x its alpha's share; each spark still seen carried over,
 * resized and turned at random, spiralling and moving up or down, fading by 1..4 */
/* 0x0037DD20 */
s32 LightRing_Update(u8 *e) {
    static const union { u32 u; f32 f; } kFifth = {0x3E4CCCCD}, kTwoFifths = {0x3ECCCCCD}, kTenth = {0x3DCCCCCD},
        kHundredth = {0x3C23D70A}, kTwentieth = {0x3D4CCCCD}, kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    VObject *rnd;
    s32 i, k;

    if (AT(e, 0x6F4, u8) == 1) {
        return 0;
    }
    AT(e, 0x6F4, u8) = 1;
    AT(e, 0x6F0, s32) ^= 1;
    if (AT(e, 0x664, f32) <= 0.0f) {
        AT(e, 0x668, s32) = 0;
    } else {
        s32 a = AT(e, 0x668, s32);
        f32 sp;

        if (a == 0x80) {
            AT(e, 0x66C, f32) = 7.0f * ((f32)a / 128.0f);
        } else if (a >= 0x79) {
            AT(e, 0x66C, f32) = 4.5f * ((f32)a / 128.0f);
        } else {
            AT(e, 0x66C, f32) = 3.0f * ((f32)a / 128.0f);
        }
        AT(e, 0x660, f32) = AT(e, 0x660, f32) + AT(e, 0x664, f32);
        if (AT(e, 0x664, f32) <= kFifth.f) {
            sp = AT(e, 0x664, f32) - kHundredth.f;
            AT(e, 0x664, f32) = sp;
            if (sp < kTenth.f) {
                AT(e, 0x664, f32) = kTenth.f;
            }
        } else {
            sp = AT(e, 0x664, f32) - 2.0f;
            AT(e, 0x664, f32) = sp;
            if (sp < kFifth.f) {
                AT(e, 0x664, f32) = kFifth.f;
            }
        }
        AT(e, 0x668, s32) -= 7;
    }
    if (AT(e, 0x668, s32) < 0) {
        AT(e, 0x668, s32) = 0;
    }
    rnd = gRandom;
    for (i = 0; i < 16; i++) {
        u32 *src = (u32 *)RING_REC(e, AT(e, 0x6F0, s32) ^ 1, i);
        u32 *dst = (u32 *)RING_REC(e, AT(e, 0x6F0, s32), i);
        QuadRec *r;
        f32 *spin = &AT(e, 0x6B0 + i * 4, f32);

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = RING_REC(e, AT(e, 0x6F0, s32), i);
        if (r->rgba[3] <= 0) {
            continue;
        }
        AT(e, 0x6F4, u8) = 0;
        r->w = 0.0f + kFifth.f + kTwoFifths.f * burst_rnd(rnd);
        r->h = r->w;
        r->turn = kPi.f * (360.0f * (burst_rnd(rnd) - 0.5f)) / 180.0f;
        *spin = *spin + kPi.f * (90.0f * burst_rnd(rnd)) / 180.0f;
        if (!(*spin <= kPi.f)) {
            *spin = *spin - kTwoPi.f;
        }
        r->pos[0] = r->pos[0] + kTwentieth.f * func_0031C248(*spin);
        r->pos[1] = r->pos[1] - AT(e, 0x670 + i * 4, f32);
        r->pos[2] = r->pos[2] + kTwentieth.f * func_0031C058(*spin);
        r->rgba[3] -= (burst_int(rnd) & 3) + 1;
        if (r->rgba[3] < 0) {
            r->rgba[3] = 0;
        }
        r->frame = burst_int(rnd) & 3;
    }
    return 1;
}

#ifdef HG_NATIVE
#include "texcache.h"

/* +0x14 draw (not while the effects are paused), with OpenGL: the ring - a strip of 16 sides
 * round the point between radius - width and radius, its inner edge at the ring's alpha and the
 * outer one clear, showing the 16 x 16 texels at (200, 70) of texture 1 (group 0x10), added
 * without depth writes - then the sparks */
/* 0x0037D4E0 */
void LightRing_Draw(u8 *e) {
    extern void glr_strip(const f32 *mvp, s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, const void *tex,
                          u64 tex0, u32 prim);
    extern u32 func_002D6010(u8 *mgr);   /* the effects are paused */

    if (func_002D6010(gEffects) != 0) {
        return;
    }
    if (AT(e, 0x668, s32) != 0) {
        f32 clip[4][4] __attribute__((aligned(16)));
        f32 xyzw[34][4] __attribute__((aligned(16)));
        f32 st[34][2];
        u8 rgba[34][4];
        u8 *tex = NULL;
        u64 tex0 = 0;
        s32 i, slot = TexCache_Resident(1, 0x10, 0x19, &tex);
        f32 outer = AT(e, 0x660, f32), inner = outer - AT(e, 0x66C, f32);
        u32 lit = ((u32)AT(e, 0x668, s32) << 24) | 0x808080;
        f32 tw = 1.0f, th = 1.0f;

        if (slot != -1) {
            tex0 = VCALL(gVram, 0x28, u64 (*)(VObject *, u32, u32, u32, u32, u32))(gVram, slot, tex[0],
                                                                                      AT(tex, 4, u16), AT(tex, 6, u16),
                                                                                      tex[1]);
            tw = AT(tex, 4, u16);
            th = AT(tex, 6, u16);
        } else {
            tex = NULL;
        }
        for (i = 0; i < 34; i++) {
            s32 k = i / 2 % 16, out = i & 1;
            f32 a = 0x1.921fb6p+1f * (22.5f * (f32)k) / 180.0f, r = out ? outer : inner;

            xyzw[i][0] = AT(e, 0x650, f32) + r * func_0031C248(a);
            xyzw[i][1] = AT(e, 0x654, f32);
            xyzw[i][2] = AT(e, 0x658, f32) + r * func_0031C058(a);
            AT(&xyzw[i][3], 0, u32) = i < 2 ? 0x8000 : 0;
            st[i][0] = (k & 1 ? 216.5f : 200.5f) / tw;
            st[i][1] = (out ? 86.5f : 70.5f) / th;
            AT(rgba[i], 0, u32) = out ? 0 : lit;
        }
        VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
        glr_strip(&clip[0][0], 34, &xyzw[0][0], &st[0][0], &rgba[0][0], tex, tex0,
                  0xC | (tex != NULL ? 0x10 : 0) | 0x40 | 0x10000 | 0x20000);
    }
    AT(e, 0x620, QuadRec *) = RING_REC(e, AT(e, 0x6F0, s32), 0);
    func_002E56C0(e + 0x610);
}
#endif

/* ---- D_0047A350 (0x130 bytes): Lorenzo's spark (Lorenzo2_BlowSparks) - two quad records (+0x10 +
 * 0x60 x the current one +0x128) growing (+0x120 a frame), turning and drifting (+0x108, 12
 * each) as their alpha fades (+0x124 a frame) and their colour dims; the quad drawer at +0xD0;
 * stopped (+0x12C) once both are gone ---- */

#define SPARK2_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x60) + (i))

/* +0xC set up: the drawer's settings (a 32 x 32 cell at (0x180, 0x80), blended 0x40, layer
 * 0x19, palette 9) */
/* 0x00374AA0 */
void LorenzoSpark_Start(u8 *e) {
    AT(e, 0x128, s32) = 0;
    AT(e, 0x12C, u8) = 0;
    AT(e, 0xD8, s64) = -1;
    AT(e, 0xE4, s32) = 0;
    AT(e, 0xE8, s32) = 0;
    AT(e, 0xEC, s32) = 0;
    AT(e, 0xF0, s32) = 0x19;
    AT(e, 0xF4, s16) = 2;
    AT(e, 0xF6, s16) = 0x180;
    AT(e, 0xF8, s16) = 0x80;
    AT(e, 0xFA, s16) = 0x20;
    AT(e, 0xFC, s16) = 0x20;
    AT(e, 0xFE, s16) = 0x200;
    AT(e, 0x100, s16) = 0x100;
    AT(e, 0x102, s8) = 0x40;
    AT(e, 0x103, s8) = 1;
    AT(e, 0x104, s8) = 1;
    AT(e, 0x105, s8) = 0x10;
    AT(e, 0x106, s8) = 9;
}

/* +0x18 start: arg { position, rise (+0x10), size (+0x14), growth (+0x18), life (+0x1C) } (none:
 * stopped). Both records at the point, white, the size, turned at random, rising 0.1 x rise and
 * drifting sideways (0.1 / 0.05 at most); growing (growth - 1) x size a frame and fading 0x80 /
 * life a frame (at least 1) */
/* 0x003744B0 */
void LorenzoSpark_SetParams(u8 *e, f32 *arg) {
    static const union { u32 u; f32 f; } kTenth = {0x3DCCCCCD}, kFifth = {0x3E4CCCCD};
    f32 at[4] __attribute__((aligned(16)));
    VObject *rnd;
    f32 size, up;
    s32 i;

    if (arg == NULL) {
        AT(e, 0x12C, u8) = 1;
        return;
    }
    sceVu0CopyVector(at, arg);
    size = arg[5];
    AT(e, 0x120, f32) = (arg[6] - 1.0f) * size;
    AT(e, 0x124, s32) = AT(arg, 0x1C, s32);
    if (AT(e, 0x124, s32) != 0) {
        AT(e, 0x124, s32) = 0x80 / AT(e, 0x124, s32);
    }
    if (AT(e, 0x124, s32) == 0) {
        AT(e, 0x124, s32) = 1;
    }
    rnd = gRandom;
    up = kTenth.f * arg[4];
    for (i = 0; i < 2; i++) {
        QuadRec *r = SPARK2_REC(e, AT(e, 0x128, s32), i);
        f32 *v = &AT(e, 0x108 + i * 12, f32);

        sceVu0CopyVector(r->pos, at);
        r->rgba[0] = 0x80;
        r->rgba[1] = 0x80;
        r->rgba[2] = 0x80;
        r->rgba[3] = 0x80;
        r->w = size;
        r->h = size;
        r->turn = 0x1.921fb6p+1f * (360.0f * (burst_rnd(rnd) - 0.5f)) / 180.0f;
        r->frame = 0;
        if (i == 0) {
            v[0] = kFifth.f * (burst_rnd(rnd) - 0.5f);
            v[1] = up;
            v[2] = kFifth.f * (burst_rnd(rnd) - 0.5f);
        } else {
            v[0] = kTenth.f * (burst_rnd(rnd) - 0.5f);
            v[1] = up;
            v[2] = kTenth.f * (burst_rnd(rnd) - 0.5f);
        }
    }
}

/* +0x10 update (0 once both are gone): each record still seen grows, turns (up to 3 degrees,
 * the way it drifts), moves, fades and dims - out once black */
/* 0x003747D0 */
s32 LorenzoSpark_Update(u8 *e) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    VObject *rnd;
    s32 i, k;

    if (AT(e, 0x12C, u8) == 1) {
        return 0;
    }
    rnd = gRandom;
    AT(e, 0x128, s32) ^= 1;
    AT(e, 0x12C, u8) = 1;
    for (i = 0; i < 2; i++) {
        u32 *src = (u32 *)SPARK2_REC(e, AT(e, 0x128, s32) ^ 1, i);
        u32 *dst = (u32 *)SPARK2_REC(e, AT(e, 0x128, s32), i);
        QuadRec *r;
        f32 *v = &AT(e, 0x108 + i * 12, f32);

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = SPARK2_REC(e, AT(e, 0x128, s32), i);
        if (r->rgba[3] <= 0) {
            continue;
        }
        AT(e, 0x12C, u8) = 0;
        r->w = r->w + AT(e, 0x120, f32);
        r->h = r->w;
        if (v[0] <= 0.0f) {
            r->turn = r->turn - kPi.f * (3.0f * burst_rnd(rnd)) / 180.0f;
        } else {
            r->turn = r->turn + kPi.f * (3.0f * burst_rnd(rnd)) / 180.0f;
        }
        r->pos[0] = r->pos[0] + v[0];
        r->pos[1] = r->pos[1] + v[1];
        r->pos[2] = r->pos[2] + v[2];
        r->rgba[3] -= AT(e, 0x124, s32);
        if (r->rgba[3] < 0) {
            r->rgba[3] = 0;
        }
        for (k = 0; k < 3; k++) {
            r->rgba[k]--;
            if (r->rgba[k] < 0) {
                r->rgba[k] = 0;
            }
        }
        if (r->rgba[0] + r->rgba[1] + r->rgba[2] == 0) {
            r->rgba[3] = 0;
        }
    }
    return 1;
}

/* +0x14 draw the current buffer, unless the effects are paused or both are gone */
/* 0x00374750 */
void LorenzoSpark_Draw(u8 *e) {
    if (func_002D6010(gEffects) == 0 && AT(e, 0x12C, u8) == 0) {
        AT(e, 0xE0, QuadRec *) = SPARK2_REC(e, AT(e, 0x128, s32), 0);
        func_002E56C0(e + 0xD0);
    }
}

/* ---- D_00470E20 (0x1C58 bytes): 64 smoke puffs rising from around (15.5, 11, 22.5), in two
 * buffers of quad records (+0x10 + 0xC00 x the current one +0x1C50), velocities at +0x1850 (16
 * each), the quad drawer at +0x1810; a puff fades from height 20 (every other frame) and is
 * renewed when gone or at height 22; stopped (+0x1C54) by its +0x18 ---- */

extern void *D_00470E20[];

#define PUFF_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0xC00) + (i))
#define PUFF_VEL(e, i) ((f32 *)((e) + 0x1850 + (i) * 0x10))

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x002FD1D0 */
u8 *SmokePuffs_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00470E20;
    AT(o, 0x1810, void **) = D_0046FC30;
    AT(o, 0x1810, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* puff i anew: at the bottom (again), or at first somewhere up, bigger and fainter the
 * higher */
void func_002FD260(u8 *e, s32 i, s32 again) {
    VObject *rnd = gRandom;
    QuadRec *r = PUFF_REC(e, AT(e, 0x1C50, s32), i);
    f32 *v = PUFF_VEL(e, i);
    s32 up = 0;

    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = 0x20;
    r->pos[0] = 15.5f + 0x1.19999a0000000p+0f /* 1.1 */ * (burst_rnd(rnd) - 0.5f);
    r->w = 0x1.3333340000000p-2f /* 0.3 */;
    if (!again) {
        up = (s32)(12.0f * burst_rnd(rnd));
        r->rgba[3] = r->rgba[3] - up;
        if (r->rgba[3] < 0) {
            r->rgba[3] = 0;
        }
        r->w = r->w * (f32)up;
    }
    rnd = gRandom;
    r->pos[1] = 11.0f + (f32)up;
    r->pos[2] = 22.5f + 0x1.19999a0000000p+0f /* 1.1 */ * (burst_rnd(rnd) - 0.5f);
    r->pos[3] = 1.0f;
    r->h = r->w;
    r->turn = 0.0f;
    r->frame = 0;
    v[0] = 0x1.eb851e0000000p-6f /* 0.03 */ * (burst_rnd(rnd) - 0.5f);
    v[1] = 0x1.47ae140000000p-5f /* 0.04 */ + 0x1.99999a0000000p-5f /* 0.05 */ * burst_rnd(rnd);
    v[2] = 0x1.eb851e0000000p-6f /* 0.03 */ * (burst_rnd(rnd) - 0.5f);
}

/* +0x18 stop */
/* 0x002FD4E0 */
void SmokePuffs_SetParams(u8 *e) {
    AT(e, 0x1C54, u8) = 1;
}

/* +0x14 draw the current buffer */
/* 0x002FD4F0 */
void SmokePuffs_Draw(u8 *e) {
    AT(e, 0x1820, QuadRec *) = PUFF_REC(e, AT(e, 0x1C50, s32), 0);
    func_002E56C0(e + 0x1810);
}

/* +0x10 update (0 once stopped): flip the buffers, each puff carried over, growing, turning (1
 * degree) and moving */
/* 0x002FD520 */
s32 SmokePuffs_Update(u8 *e) {
    s32 i, k;

    if (AT(e, 0x1C54, u8) == 1) {
        return 0;
    }
    AT(e, 0x1C50, s32) ^= 1;
    for (i = 0; i < 64; i++) {
        u32 *src = (u32 *)PUFF_REC(e, AT(e, 0x1C50, s32) ^ 1, i);
        u32 *dst = (u32 *)PUFF_REC(e, AT(e, 0x1C50, s32), i);
        f32 *v = PUFF_VEL(e, i);
        QuadRec *r;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = PUFF_REC(e, AT(e, 0x1C50, s32), i);
        r->w = r->w + 0x1.47ae140000000p-6f /* 0.02 */;
        r->h = r->h + 0x1.47ae140000000p-6f /* 0.02 */;
        r->turn = r->turn + 0x1.1df46ap-6f /* 1 degree */;
        if (!(r->turn <= 0x1.921fb6p+1f)) {
            r->turn = r->turn - 0x1.921fb6p+2f;
        }
        r->pos[0] = r->pos[0] + v[0];
        r->pos[1] = r->pos[1] + v[1];
        r->pos[2] = r->pos[2] + v[2];
        if (!(r->pos[1] < 22.0f)) {
            func_002FD260(e, i, 1);
        }
        if (r->rgba[3] > 0) {
            if (!(r->pos[1] < 20.0f) && AT(e, 0x1C50, s32) != 0) {
                r->rgba[3] -= 4;
                if (r->rgba[3] < 0) {
                    r->rgba[3] = 0;
                }
            }
        } else {
            func_002FD260(e, i, 1);
        }
    }
    return 1;
}

/* +0xC set up: the drawer's settings (a 16-frame strip of 32 x 32 cells, row 64, layer 0x19),
 * every puff placed */
/* 0x002FD760 */
void SmokePuffs_Start(u8 *e) {
    s32 i;

    AT(e, 0x1C50, s32) = 0;
    AT(e, 0x1C54, u8) = 0;
    AT(e, 0x1818, s64) = -1;
    AT(e, 0x1828, s32) = 0;
    AT(e, 0x182C, s32) = 0;
    AT(e, 0x1830, s32) = 0x19;
    AT(e, 0x1834, s16) = 0x40;
    AT(e, 0x1836, s16) = 0;
    AT(e, 0x1838, s16) = 0x40;
    AT(e, 0x183A, s16) = 0x20;
    AT(e, 0x183C, s16) = 0x20;
    AT(e, 0x183E, s16) = 0x200;
    AT(e, 0x1840, s16) = 0x100;
    AT(e, 0x1842, s8) = 0;
    AT(e, 0x1843, s8) = 1;
    AT(e, 0x1844, s8) = 1;
    AT(e, 0x1845, s8) = 0x10;
    AT(e, 0x1846, s8) = -1;
    for (i = 0; i < 64; i++) {
        func_002FD260(e, i, 0);
    }
}

/* ---- D_00470E40 (0x7F8 bytes): a swarm of up to 16 specks (+0x634) buzzing about a centre
 * (+0x7E0) within a spread (+0x7F0), in two buffers of quad records (+0x10 + 0x300 x the
 * current one +0x7F4), velocities at +0x648 and their pulls back to the centre at +0x708 (12
 * each), alphas at +0x7C8, the quad drawer at +0x610; hidden when the camera is within 10, dim
 * within 20 ---- */

extern void *D_00470E40[];

#define SWARM_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x300) + (i))
#define SWARM_VEL(e, i) ((f32 *)((e) + 0x648 + (i) * 0xC))
#define SWARM_ACC(e, i) ((f32 *)((e) + 0x708 + (i) * 0xC))

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x002FD820 */
u8 *SpeckSwarm_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00470E40;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

static s32 clamp_byte(s32 c) {
    if (c < 0) {
        return 0;
    }
    return c >= 0x100 ? 0xFF : c;
}

/* a speck's new heading: a velocity of up to 0.05 x the spread each way, and a pull back of
 * an eighth of it */
static void swarm_heading(u8 *e, s32 i, VObject *rnd) {
    f32 *v = SWARM_VEL(e, i), *a = SWARM_ACC(e, i);

    v[0] = 0x1.99999a0000000p-4f /* 0.1 */ * (AT(e, 0x7F0, f32) * (burst_rnd(rnd) - 0.5f));
    v[1] = 0x1.99999a0000000p-4f /* 0.1 */ * (AT(e, 0x7F0, f32) * (burst_rnd(rnd) - 0.5f));
    v[2] = 0x1.99999a0000000p-4f /* 0.1 */ * (AT(e, 0x7F0, f32) * (burst_rnd(rnd) - 0.5f));
    a[0] = -0.25f * (0.5f * v[0]);
    a[1] = -0.25f * (0.5f * v[1]);
    a[2] = -0.25f * (0.5f * v[2]);
}

/* +0x18 start: arg { f32 count (16 at most), centre, spread, RGB, alpha (each speck up to 15
 * / 7 off, from 8 / 4 below) } */
/* 0x002FD8B0 */
void SpeckSwarm_SetParams(u8 *e, u8 *arg) {
    VObject *rnd;
    s32 i, r0, g0, b0, a0;

    if (arg == NULL) {
        return;
    }
    AT(e, 0x634, s16) = (s32)AT(arg, 0x0, f32);
    if (AT(e, 0x634, s16) > 0x10) {
        AT(e, 0x634, s16) = 0x10;
    }
    AT(e, 0x7E0, f32) = AT(arg, 0x4, f32);
    AT(e, 0x7E4, f32) = AT(arg, 0x8, f32);
    AT(e, 0x7E8, f32) = AT(arg, 0xC, f32);
    AT(e, 0x7EC, f32) = 1.0f;
    AT(e, 0x7F0, f32) = AT(arg, 0x10, f32);
    r0 = AT(arg, 0x14, s32) - 8;
    g0 = AT(arg, 0x18, s32) - 8;
    b0 = AT(arg, 0x1C, s32) - 8;
    a0 = AT(arg, 0x20, s32) - 4;
    rnd = gRandom;
    for (i = 0; i < AT(e, 0x634, s16); i++) {
        QuadRec *r = SWARM_REC(e, AT(e, 0x7F4, s32), i);
        u8 *alpha = e + 0x7C8 + i;
        f32 s;

        r->rgba[0] = r0 + (burst_int(rnd) & 0xF);
        r->rgba[0] = clamp_byte(r->rgba[0]);
        r->rgba[1] = g0 + (burst_int(rnd) & 0xF);
        r->rgba[1] = clamp_byte(r->rgba[1]);
        r->rgba[2] = b0 + (burst_int(rnd) & 0xF);
        r->rgba[2] = clamp_byte(r->rgba[2]);
        *alpha = a0 + (burst_int(rnd) & 7);
        if (*alpha > 0x80) {
            *alpha = 0x80;
        }
        r->pos[0] = AT(e, 0x7E0, f32) + AT(e, 0x7F0, f32) * (burst_rnd(rnd) - 0.5f);
        r->pos[1] = AT(e, 0x7E4, f32) + AT(e, 0x7F0, f32) * (burst_rnd(rnd) - 0.5f);
        r->pos[2] = AT(e, 0x7E8, f32) + AT(e, 0x7F0, f32) * (burst_rnd(rnd) - 0.5f);
        r->pos[3] = 1.0f;
        s = 0x1.99999a0000000p-5f /* 0.05 */ + 0x1.47ae140000000p-7f /* 0.01 */ * (AT(e, 0x7F0, f32) * burst_rnd(rnd));
        r->w = s;
        r->h = s;
        r->turn = 0x1.921fb6p+2f * (burst_rnd(rnd) - 0.5f);
        r->frame = 0;
        swarm_heading(e, i, rnd);
    }
}

/* +0x14 draw the current buffer */
/* 0x002FDD40 */
void SpeckSwarm_Draw(u8 *e) {
    AT(e, 0x620, QuadRec *) = SWARM_REC(e, AT(e, 0x7F4, s32), 0);
    func_002E56C0(e + 0x610);
}

/* one axis: the velocity pulled, the speck moved, the pull turned back once past the centre */
static void swarm_axis(f32 *pos, f32 *v, f32 *a, f32 centre) {
    *v = *v + *a;
    *pos = *pos + *v;
    if (*a < 0.0f) {
        if (*pos - centre < 0.0f) {
            *a = *a * -1.0f;
        }
    } else if (!(*pos - centre <= 0.0f)) {
        *a = *a * -1.0f;
    }
}

/* +0x10 update: flip the buffers; each speck carried over, now and then (1 in 10) a new
 * heading, moved, turned at random, its alpha by the camera's distance */
/* 0x002FDD70 */
s32 SpeckSwarm_Update(u8 *e) {
    f32 cam[4] __attribute__((aligned(16)));
    VObject *rnd;
    f32 dx, dy, dz, d2;
    s32 shift, i, k;

    AT(e, 0x7F4, s32) ^= 1;
    VCALL(gCamera, 0x20, void (*)(VObject *, f32 *))(gCamera, cam);
    dy = AT(e, 0x7E4, f32) - cam[1];
    dx = AT(e, 0x7E0, f32) - cam[0];
    dz = AT(e, 0x7E8, f32) - cam[2];
    d2 = dy * dy + dx * dx + dz * dz;
    if (d2 < 100.0f) {
        shift = 8;
    } else if (d2 < 400.0f) {
        shift = 1;
    } else {
        shift = 0;
    }
    rnd = gRandom;
    for (i = 0; i < AT(e, 0x634, s16); i++) {
        u32 *src = (u32 *)SWARM_REC(e, AT(e, 0x7F4, s32) ^ 1, i);
        u32 *dst = (u32 *)SWARM_REC(e, AT(e, 0x7F4, s32), i);
        f32 *v = SWARM_VEL(e, i), *a = SWARM_ACC(e, i);
        QuadRec *r;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = SWARM_REC(e, AT(e, 0x7F4, s32), i);
        if (burst_rnd(rnd) < 0x1.99999a0000000p-4f /* 0.1 */) {
            swarm_heading(e, i, rnd);
        }
        swarm_axis(&r->pos[0], &v[0], &a[0], AT(e, 0x7E0, f32));
        swarm_axis(&r->pos[1], &v[1], &a[1], AT(e, 0x7E4, f32));
        swarm_axis(&r->pos[2], &v[2], &a[2], AT(e, 0x7E8, f32));
        r->turn = 0x1.921fb6p+2f * (burst_rnd(rnd) - 0.5f);
        r->rgba[3] = AT(e, 0x7C8 + i, u8) >> shift;
    }
    return 1;
}

/* +0xC set up: the drawer's settings (a 4 x 4 cell at (238, 78), blended 0x20, palette 2,
 * layer 0x19) */
/* 0x002FE230 */
void SpeckSwarm_Start(u8 *e) {
    AT(e, 0x7F4, s32) = 0;
    AT(e, 0x618, s64) = -1;
    AT(e, 0x624, s32) = 0;
    AT(e, 0x628, s32) = 0;
    AT(e, 0x62C, s32) = 0;
    AT(e, 0x630, s32) = 0x19;
    AT(e, 0x636, s16) = 0xEE;
    AT(e, 0x638, s16) = 0x4E;
    AT(e, 0x63A, s16) = 4;
    AT(e, 0x63C, s16) = 4;
    AT(e, 0x63E, s16) = 0x200;
    AT(e, 0x640, s16) = 0x100;
    AT(e, 0x642, s8) = 0x20;
    AT(e, 0x643, s8) = 1;
    AT(e, 0x644, s8) = 1;
    AT(e, 0x645, s8) = 0x10;
    AT(e, 0x646, s8) = 2;
}

/* ---- D_00470E60 (0x4E0 bytes): a splash - up to 10 specks (+0x3F4) thrown up and away from
 * a character (the pursuer, the partner or Fiona), in two buffers of quad records (+0x10 +
 * 0x1E0 x the current one +0x4D8), velocities at +0x408 (12 each) and their sideways drags at
 * +0x480 (x, z), the quad drawer at +0x3D0; each speck bounces off the ground (+0x4D0),
 * fading, until gone; every other call (+0x4DC) is a rest ---- */

extern void *D_00470E60[];

#define SPLASH_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x1E0) + (i))
#define SPLASH_VEL(e, i) ((f32 *)((e) + 0x408 + (i) * 0xC))
#define SPLASH_DRAG(e, i) ((f32 *)((e) + 0x480 + (i) * 8))

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x002FE2B0 */
u8 *Splash_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00470E60;
    AT(o, 0x3D0, void **) = D_0046FC30;
    AT(o, 0x3D0, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* +0x18 start: arg { f32 who (0xFE the pursuer, 1 the partner, else Fiona), f32 count (10 at
 * most), the point (its height the ground), spread, RGB (each speck up to 15 off, from 8
 * below), alpha (up to 7, from 4 below) } */
/* 0x002FE340 */
void Splash_SetParams(u8 *e, f32 *arg) {
    f32 dir[4] __attribute__((aligned(16)));
    VObject *rnd;
    s32 who, i, r0, g0, b0, a0;
    f32 x, y, z;

    if (arg == NULL) {
        return;
    }
    AT(e, 0x3F4, s16) = (s32)arg[1];
    who = (s32)arg[0];
    if (AT(e, 0x3F4, s16) > 10) {
        AT(e, 0x3F4, s16) = 10;
    }
    x = arg[2];
    y = arg[3];
    AT(e, 0x4D0, f32) = y;
    z = arg[4];
    AT(e, 0x4D4, f32) = arg[5];
    r0 = AT(arg, 0x18, s32) - 8;
    g0 = AT(arg, 0x1C, s32) - 8;
    b0 = AT(arg, 0x20, s32) - 8;
    a0 = AT(arg, 0x24, s32) - 4;
    rnd = gRandom;
    for (i = 0; i < AT(e, 0x3F4, s16); i++) {
        QuadRec *r = SPLASH_REC(e, AT(e, 0x4D8, s32), i);
        f32 *v = SPLASH_VEL(e, i), *d = SPLASH_DRAG(e, i);
        void *c;
        f32 s;

        r->rgba[0] = r0 + (burst_int(rnd) & 0xF);
        r->rgba[0] = clamp_byte(r->rgba[0]);
        r->rgba[1] = g0 + (burst_int(rnd) & 0xF);
        r->rgba[1] = clamp_byte(r->rgba[1]);
        r->rgba[2] = b0 + (burst_int(rnd) & 0xF);
        r->rgba[2] = clamp_byte(r->rgba[2]);
        r->rgba[3] = a0 + (burst_int(rnd) & 7);
        if (r->rgba[3] < 0) {
            r->rgba[3] = 0;
        } else if (r->rgba[3] > 0x80) {
            r->rgba[3] = 0x80;
        }
        r->pos[0] = x + AT(e, 0x4D4, f32) * (burst_rnd(rnd) - 0.5f);
        r->pos[1] = y;
        r->pos[2] = z + AT(e, 0x4D4, f32) * (burst_rnd(rnd) - 0.5f);
        r->pos[3] = 1.0f;
        s = 0x1.99999a0000000p-3f /* 0.2 */ + 0x1.47ae140000000p-7f /* 0.01 */ * (AT(e, 0x4D4, f32) * burst_rnd(rnd));
        r->w = s;
        r->h = s;
        r->turn = 0x1.921fb6p+2f * (burst_rnd(rnd) - 0.5f);
        r->frame = 0;
        c = who == 0xFE ? gCharPursuer : who == 1 ? gCharPartner : gCharPlayer;
        if (c != NULL) {
            sceVu0SubVector(dir, r->pos, (f32 *)((u8 *)c + 0x10));
        }
        sceVu0Normalize(dir, dir);
        v[0] = dir[0] * burst_rnd(rnd);
        v[1] = 1.0f + 2.0f * burst_rnd(rnd);
        v[2] = dir[2] * burst_rnd(rnd);
        d[0] = -0x1.47ae140000000p-7f /* 0.01 */ * v[0];
        d[1] = -0x1.47ae140000000p-7f /* 0.01 */ * v[2];
    }
}

/* +0x14 draw the current buffer */
/* 0x002FE800 */
void Splash_Draw(u8 *e) {
    AT(e, 0x3E0, QuadRec *) = SPLASH_REC(e, AT(e, 0x4D8, s32), 0);
    func_002E56C0(e + 0x3D0);
}

/* +0x10 update: flip the buffers; every other call 0 (and once all are gone); each visible
 * speck carried over, falling (hard while rising), jittered, moved, and on the ground faded
 * by 0x20 and thrown up again */
/* 0x002FE830 */
s32 Splash_Update(u8 *e) {
    VObject *rnd;
    s32 i, k;

    AT(e, 0x4D8, s32) ^= 1;
    if (AT(e, 0x4DC, u8) == 1) {
        return 0;
    }
    AT(e, 0x4DC, u8) = 1;
    rnd = gRandom;
    for (i = 0; i < AT(e, 0x3F4, s16); i++) {
        u32 *src = (u32 *)SPLASH_REC(e, AT(e, 0x4D8, s32) ^ 1, i);
        u32 *dst = (u32 *)SPLASH_REC(e, AT(e, 0x4D8, s32), i);
        f32 *v = SPLASH_VEL(e, i), *d = SPLASH_DRAG(e, i);
        QuadRec *r;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = SPLASH_REC(e, AT(e, 0x4D8, s32), i);
        if (r->rgba[3] <= 0) {
            continue;
        }
        AT(e, 0x4DC, u8) = 0;
        if (!(v[1] <= 0.0f)) {
            v[1] = v[1] + 0.5f * (-0.5f - burst_rnd(rnd));
        } else {
            v[1] = v[1] + 0x1.47ae140000000p-7f /* 0.01 */ * (burst_rnd(rnd) - 1.0f);
        }
        v[0] = v[0] + (d[0] + 0x1.99999a0000000p-4f /* 0.1 */ * (burst_rnd(rnd) - 0.5f));
        v[2] = v[2] + (d[1] + 0x1.99999a0000000p-4f /* 0.1 */ * (burst_rnd(rnd) - 0.5f));
        r->pos[0] = r->pos[0] + v[0];
        r->pos[1] = r->pos[1] + v[1];
        r->pos[2] = r->pos[2] + v[2];
        if (r->pos[1] < AT(e, 0x4D0, f32)) {
            r->rgba[3] -= 0x20;
            if (r->rgba[3] < 0) {
                r->rgba[3] = 0;
            } else {
                v[0] = v[0] + 0x1.99999a0000000p-4f /* 0.1 */ * (burst_rnd(rnd) - 0.5f);
                v[1] = 1.0f + burst_rnd(rnd);
                v[2] = v[2] + 0x1.99999a0000000p-4f /* 0.1 */ * (burst_rnd(rnd) - 0.5f);
                d[0] = -0x1.47ae140000000p-7f /* 0.01 */ * v[0];
                d[1] = -0x1.47ae140000000p-7f /* 0.01 */ * v[2];
            }
        }
    }
    return 1;
}

/* +0xC set up: the drawer's settings (a 4 x 4 cell at (238, 78), blended 0x20, palette 2,
 * layer 0x19) */
/* 0x002FEBD0 */
void Splash_Start(u8 *e) {
    AT(e, 0x4D8, s32) = 0;
    AT(e, 0x4DC, u8) = 0;
    AT(e, 0x3D8, s64) = -1;
    AT(e, 0x3E4, s32) = 0;
    AT(e, 0x3E8, s32) = 0;
    AT(e, 0x3EC, s32) = 0;
    AT(e, 0x3F0, s32) = 0x19;
    AT(e, 0x3F6, s16) = 0xEE;
    AT(e, 0x3F8, s16) = 0x4E;
    AT(e, 0x3FA, s16) = 4;
    AT(e, 0x3FC, s16) = 4;
    AT(e, 0x3FE, s16) = 0x200;
    AT(e, 0x400, s16) = 0x100;
    AT(e, 0x402, s8) = 0x20;
    AT(e, 0x403, s8) = 1;
    AT(e, 0x404, s8) = 1;
    AT(e, 0x405, s8) = 0x10;
    AT(e, 0x406, s8) = 2;
}

/* ---- D_00470F30 (0xE60 bytes): a spray of blood - 32 dark red drops, in two buffers of quad
 * records (+0x10 + 0x600 x the current one +0xE50), velocities at +0xC50 (16 each), the quad
 * drawer at +0xC10; they shrink and fall, and on the floor of their nav triangle (+0xE54, -1:
 * none - no floor) half of them leave a splat (D_00472BF0) ---- */

extern void *D_00470F30[], *D_00472BF0[];

#define BLOOD_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x600) + (i))
#define BLOOD_VEL(e, i) ((f32 *)((e) + 0xC50 + (i) * 0x10))

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x002FF6D0 */
u8 *BloodSpray_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00470F30;
    AT(o, 0xC10, void **) = D_0046FC30;
    AT(o, 0xC10, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* +0x18 start: arg { the point, +0x10 the triangle << 8, or by whose (0 Fiona, 1 the partner,
 * 0xFE the pursuer) triangle; +0x14 heavy (bigger, brighter, faster drops) } */
/* 0x002FF760 */
void BloodSpray_SetParams(u8 *e, u8 *arg) {
    f32 p[4] __attribute__((aligned(16)));
    VObject *rnd;
    u32 who;
    s32 tri, heavy, i;

    if (arg == NULL) {
        return;
    }
    sceVu0CopyVector(p, (f32 *)arg);
    who = AT(arg, 0x10, u32);
    tri = who >> 8;
    if (tri == 0) {
        void *c;

        switch (who) {
        case 0xFE: c = gCharPursuer; break;
        case 0: c = gCharPlayer; break;
        case 1: c = gCharPartner; break;
        default: c = NULL; break;
        }
        tri = c != NULL ? AT(c, 0x34, s32) : -1;
    }
    AT(e, 0xE54, s32) = tri;
    heavy = AT(arg, 0x14, s32) != 0;
    rnd = gRandom;
    for (i = 0; i < 32; i++) {
        QuadRec *r = BLOOD_REC(e, AT(e, 0xE50, s32), i);
        f32 *v = BLOOD_VEL(e, i);
        f32 s;

        r->rgba[0] = 0x30;
        r->rgba[1] = 0;
        r->rgba[2] = 0;
        r->rgba[3] = (burst_int(rnd) & 0x1F) + (heavy ? 0x60 : 0x40);
        r->pos[0] = p[0] + 0.5f * (burst_rnd(rnd) - 0.5f);
        r->pos[1] = p[1] + 0.5f * burst_rnd(rnd);
        r->pos[2] = p[2] + 0.5f * (burst_rnd(rnd) - 0.5f);
        r->pos[3] = 1.0f;
        s = (heavy ? 3.0f : 1.0f) + 2.0f * burst_rnd(rnd);
        r->w = s;
        r->h = s;
        r->turn = 0.0f;
        r->frame = 0;
        if (heavy) {
            v[0] = burst_rnd(rnd) - 0.5f;
            v[1] = 0x1.99999a0000000p-2f /* 0.4 */ + 2.0f * burst_rnd(rnd) / r->w;
            v[2] = burst_rnd(rnd) - 0.5f;
        } else {
            v[0] = 0.5f * (burst_rnd(rnd) - 0.5f);
            v[1] = 0x1.99999a0000000p-3f /* 0.2 */ + 2.0f * burst_rnd(rnd) / r->w;
            v[2] = 0.5f * (burst_rnd(rnd) - 0.5f);
        }
    }
}

/* +0x14 draw the current buffer, unless the effects are paused or all are gone */
/* 0x002FFC70 */
void BloodSpray_Draw(u8 *e) {
    if (func_002D6010(gEffects) == 0 && AT(e, 0xE58, u8) == 0) {
        AT(e, 0xC20, QuadRec *) = BLOOD_REC(e, AT(e, 0xE50, s32), 0);
        func_002E56C0(e + 0xC10);
    }
}

static void splat_init(void **obj) {
    obj[0] = D_00472BF0;
    obj[0x70 / 4] = D_00469D00;
    ((s32 *)obj)[0x74 / 4] = -1;
    obj[0x70 / 4] = D_0046FC30;
}

/* +0x10 update (0 once all are gone): flip the buffers; each live drop carried over,
 * shrinking, falling (slower once falling), stopped by the floor; a drop gone leaves a splat
 * half the time */
/* 0x002FFCF0 */
s32 BloodSpray_Update(u8 *e) {
    struct {
        f32 pos[4];
        s32 tri;
        s32 zero;
    } sp __attribute__((aligned(16)));
    f32 floor[4] __attribute__((aligned(16)));
    VObject *nm, *rnd;
    u8 *mgr;
    s32 i, k;

    if (AT(e, 0xE58, u8) == 1) {
        return 0;
    }
    AT(e, 0xE58, u8) = 1;
    nm = (VObject *)gNavMesh;
    rnd = gRandom;
    mgr = gEffects;
    AT(e, 0xE50, s32) ^= 1;
    for (i = 0; i < 32; i++) {
        u32 *src = (u32 *)BLOOD_REC(e, AT(e, 0xE50, s32) ^ 1, i);
        u32 *dst = (u32 *)BLOOD_REC(e, AT(e, 0xE50, s32), i);
        f32 *v = BLOOD_VEL(e, i);
        QuadRec *r;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = BLOOD_REC(e, AT(e, 0xE50, s32), i);
        if (r->w <= 0.0f || r->rgba[3] <= 0) {
            continue;
        }
        AT(e, 0xE58, u8) = 0;
        r->w = r->w - 0x1.47ae140000000p-6f /* 0.02 */;
        if (r->w < 0.0f) {
            r->w = 0.0f;
        }
        r->h = r->w;
        v[1] = v[1] + (v[1] < 0.0f ? -0x1.99999a0000000p-5f /* 0.05 */ : -0x1.99999a0000000p-4f /* 0.1 */);
        if (AT(e, 0xE54, s32) == -1) {
            r->pos[0] = r->pos[0] + v[0];
            r->pos[1] = r->pos[1] + v[1];
            r->pos[2] = r->pos[2] + v[2];
        } else {
            VCALL(nm, 0xC, void (*)(VObject *, s32, f32 *))(nm, AT(e, 0xE54, s32), floor);
            if (r->pos[1] + v[1] < floor[1]) {
                r->rgba[3] = 0;
                r->w = 0.0f;
            } else {
                r->pos[0] = r->pos[0] + v[0];
                r->pos[1] = r->pos[1] + v[1];
                r->pos[2] = r->pos[2] + v[2];
            }
        }
        r->rgba[3] -= 4;
        if (r->rgba[3] < 0) {
            r->rgba[3] = 0;
        }
        if ((r->rgba[3] == 0 || r->w == 0.0f) && AT(e, 0xE54, s32) != -1 && !(burst_int(rnd) & 1)) {
            s32 slot = Effect_New(mgr, 0x140, splat_init);

            sp.pos[0] = r->pos[0];
            sp.pos[1] = r->pos[1];
            sp.pos[2] = r->pos[2];
            sp.pos[3] = r->pos[3];
            sp.tri = AT(e, 0xE54, s32);
            sp.zero = 0;
            EffectMgr_Start(mgr, slot, &sp);
        }
    }
    return 1;
}

/* +0xC set up: the drawer's settings (a 16-frame strip of 32 x 32 cells at (96, 64), layer
 * 0x19) */
/* 0x00300130 */
void BloodSpray_Start(u8 *e) {
    AT(e, 0xE50, s32) = 0;
    AT(e, 0xE58, u8) = 0;
    AT(e, 0xC18, s64) = -1;
    AT(e, 0xC24, s32) = 0;
    AT(e, 0xC28, s32) = 0;
    AT(e, 0xC2C, s32) = 0;
    AT(e, 0xC30, s32) = 0x19;
    AT(e, 0xC34, s16) = 0x20;
    AT(e, 0xC36, s16) = 0x60;
    AT(e, 0xC38, s16) = 0x40;
    AT(e, 0xC3A, s16) = 0x20;
    AT(e, 0xC3C, s16) = 0x20;
    AT(e, 0xC3E, s16) = 0x200;
    AT(e, 0xC40, s16) = 0x100;
    AT(e, 0xC42, s8) = 0;
    AT(e, 0xC43, s8) = 1;
    AT(e, 0xC44, s8) = 1;
    AT(e, 0xC45, s8) = 0x10;
    AT(e, 0xC46, s8) = -1;
}

/* ---- D_00471000 (0x60 bytes; event command 0xA9): a 2 x 2 quad (texture group 0x10, cell
 * (64, 0) 64 x 64, palette 1) wandering about its place: +0x10 the place, +0x20 its turn, +0x30
 * / +0x34 the range (x, z), +0x38 / +0x3C the offset now, +0x40 / +0x44 where it heads, +0x48 /
 * +0x4C its step (its facing too), +0x50 frames to wait ---- */

extern void *D_00471000[];

/* +0x8 destructor */
/* 0x003055F0 */
void *Effect71000_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00471000;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046F580;
        }
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* Copies a light/pose record when its type field (src[0]) is 0; angles are given in degrees. */
/* 0x00305650 */
void Effect71000_SetParams(u8 *self, f32 *src) {
    if (src != NULL && src[0] == 0.0f) {
        F32(self, 0x10) = src[1];
        F32(self, 0x14) = src[2];
        F32(self, 0x18) = src[3];
        F32(self, 0x1C) = 1.0f;
        F32(self, 0x30) = src[4] - 1.0f;
        F32(self, 0x34) = src[5] - 1.0f;
        F32(self, 0x20) = (B5_PI * src[6]) / 180.0f;
        F32(self, 0x24) = (B5_PI * src[7]) / 180.0f;
        F32(self, 0x28) = (B5_PI * src[8]) / 180.0f;
        F32(self, 0x2C) = 0.0f;
    }
}

/* +0xC set up: at its place, a random step (-0.25..0.25), waiting 1 or 31 frames */
/* 0x00305DD0 */
void Effect71000_Start(u8 *o) {
    VObject *rnd;

    AT(o, 0x38, s32) = 0;
    AT(o, 0x3C, s32) = 0;
    AT(o, 0x40, f32) = AT(o, 0x38, f32);
    AT(o, 0x44, f32) = AT(o, 0x3C, f32);
    rnd = gRandom;
    AT(o, 0x48, f32) = 0.5f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    AT(o, 0x4C, f32) = 0.5f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    AT(o, 0x50, s32) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 1) * 30 + 1;
}

/* one axis of the walk: on by the step, kept in the range (a side reached becomes the target),
 * stopping at the target */
static inline __attribute__((always_inline)) void wander_axis(u8 *o, s32 cur, s32 to, s32 step, s32 range) {
    f32 x, r, v;

    AT(o, cur, f32) = AT(o, cur, f32) + AT(o, step, f32);
    r = AT(o, range, f32);
    x = AT(o, cur, f32);
    if (!(x < r)) {
        AT(o, cur, f32) = r;
        AT(o, to, f32) = r;
        return;
    }
    r = -r;
    if (x <= r) {
        AT(o, cur, f32) = r;
        AT(o, to, f32) = r;
        return;
    }
    v = AT(o, step, f32);
    if (!(v <= 0.0f)) {
        if (AT(o, to, f32) - x < 0.0f) {
            AT(o, cur, f32) = AT(o, to, f32);
        }
    } else if (v < 0.0f && !(AT(o, to, f32) - x <= 0.0f)) {
        AT(o, cur, f32) = AT(o, to, f32);
    }
}

/* +0x10 update: waiting, or walking toward the target (then waiting 1..91 frames); there, 1 in
 * 8 a new target (somewhere in the range, or back home) a tenth of the way a step, else a nudge
 * and a short wait */
/* 0x003059C0 */
s32 Effect71000_Update(u8 *o) {
    f32 cx, tx;

    if (AT(o, 0x50, s32) != 0) {
        AT(o, 0x50, s32)--;
        return 1;
    }
    cx = AT(o, 0x38, f32);
    tx = AT(o, 0x40, f32);
    if (tx == cx && AT(o, 0x44, f32) == AT(o, 0x3C, f32)) {
        VObject *r1 = gRandom;

        if (!(VCALL(r1, 0x10, s32 (*)(VObject *))(r1) & 7)) {
            if (AT(o, 0x40, f32) == 0.0f && AT(o, 0x44, f32) == 0.0f) {
                VObject *r = gRandom;

                AT(o, 0x40, f32) = 2.0f * AT(o, 0x30, f32) * (VCALL(r, 0x18, f32 (*)(VObject *))(r) - 0.5f);
                AT(o, 0x44, f32) = 2.0f * AT(o, 0x34, f32) * (VCALL(r, 0x18, f32 (*)(VObject *))(r) - 0.5f);
            } else {
                AT(o, 0x40, s32) = 0;
                AT(o, 0x44, s32) = 0;
            }
            AT(o, 0x48, f32) = 0x1.99999a0000000p-4f /* 0.1 */ * (AT(o, 0x40, f32) - AT(o, 0x38, f32));
            AT(o, 0x4C, f32) = 0x1.99999a0000000p-4f /* 0.1 */ * (AT(o, 0x44, f32) - AT(o, 0x3C, f32));
        } else {
            VObject *r = gRandom;

            AT(o, 0x50, s32) = (VCALL(r, 0x10, s32 (*)(VObject *))(r) & 1) * 30 + 1;
            if (VCALL(r, 0x10, s32 (*)(VObject *))(r) & 1) {
                AT(o, 0x48, f32) = AT(o, 0x48, f32) + 1.0f;
            } else {
                AT(o, 0x48, f32) = AT(o, 0x48, f32) - 1.0f;
            }
            if (VCALL(r1, 0x10, s32 (*)(VObject *))(r1) & 1) {
                AT(o, 0x4C, f32) = AT(o, 0x4C, f32) + 1.0f;
            } else {
                AT(o, 0x4C, f32) = AT(o, 0x4C, f32) - 1.0f;
            }
        }
        return 1;
    }
    if (tx != cx) {
        wander_axis(o, 0x38, 0x40, 0x48, 0x30);
    }
    if (AT(o, 0x44, f32) != AT(o, 0x3C, f32)) {
        wander_axis(o, 0x3C, 0x44, 0x4C, 0x34);
    }
    if (AT(o, 0x40, f32) == AT(o, 0x38, f32) && AT(o, 0x44, f32) == AT(o, 0x3C, f32)) {
        AT(o, 0x50, s32) = (VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 3) * 30 + 1;
    }
    return 1;
}

/* +0x14 draw: the quad facing along its step, at the offset, turned and placed */
/* 0x00305700 */
void Effect71000_Draw(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kHalfPi = {0x3FC90FDB};
    f32 m[4][4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 c[4][4] __attribute__((aligned(16)));
    QuadRec r __attribute__((aligned(16)));
    QuadDrawer q __attribute__((aligned(16)));

    sceVu0UnitMatrix(m);
    dir[0] = AT(o, 0x48, f32);
    dir[1] = 0.0f;
    dir[2] = AT(o, 0x4C, f32);
    dir[3] = 0.0f;
    sceVu0Normalize(dir, dir);
    if (dir[2] != 0.0f) {
        if (!(dir[2] <= 0.0f)) {
            sceVu0RotMatrixY(m, m, func_0031BDB0(dir[0] / dir[2]) - kHalfPi.f);
        } else {
            sceVu0RotMatrixY(m, m, kPi.f + (func_0031BDB0(dir[0] / dir[2]) - kHalfPi.f));
        }
    }
    at[0] = AT(o, 0x38, f32);
    at[1] = 0.0f;
    at[2] = AT(o, 0x3C, f32);
    at[3] = 1.0f;
    sceVu0TransMatrix(m, m, at);
    sceVu0RotMatrix(m, m, (f32 *)(o + 0x20));
    sceVu0TransMatrix(m, m, (f32 *)(o + 0x10));
    r.rgba[0] = 0x80;
    r.rgba[1] = 0x80;
    r.rgba[2] = 0x80;
    r.rgba[3] = 0x80;
    c[0][2] = -1.0f;
    r.pos[3] = 1.0f;
    r.w = 1.0f;
    r.h = 1.0f;
    c[0][0] = 1.0f;
    c[0][3] = 1.0f;
    r.pos[0] = 0.0f;
    r.pos[1] = 0.0f;
    r.pos[2] = 0.0f;
    r.turn = 0.0f;
    r.frame = 0;
    c[0][1] = 0.0f;
    sceVu0ApplyMatrix(c[0], m, c[0]);
    c[1][0] = 1.0f;
    c[1][2] = 1.0f;
    c[1][3] = 1.0f;
    c[1][1] = 0.0f;
    sceVu0ApplyMatrix(c[1], m, c[1]);
    c[2][3] = 1.0f;
    c[2][0] = -1.0f;
    c[2][2] = -1.0f;
    c[2][1] = 0.0f;
    sceVu0ApplyMatrix(c[2], m, c[2]);
    c[3][0] = -1.0f;
    c[3][2] = 1.0f;
    c[3][3] = 1.0f;
    c[3][1] = 0.0f;
    sceVu0ApplyMatrix(c[3], m, c[3]);
    q.a = -1;
    q.count = 1;
    q.tex = (u64)-1;
    q.cellX = 0x40;
    q.cellW = 0x40;
    q.vtbl = D_0046FC30;
    q.cellH = 0x40;
    q.rec = &r;
    q.frames = 1;
    q.corners = (s32)c;
    q.palette = 1;
    q.layer = 0x19;
    q.texW = 0x200;
    q.cx = 0.0f;
    q.texH = 0x100;
    q.cy = 0.0f;
    q.cellY = 0;
    q.flags = 2;
    q.texId = 0;
    q.texGroup = 0x10;
    func_002E56C0((u8 *)&q);
    q.vtbl = D_00469D00;
}

/* ---- D_00479AE0 (0x40 bytes): a splash ring on water - a flat ring at +0x10 (16 sides, 0.2
 * wide) growing from radius 0.2 by +0x2C a frame, its colour +0x20 (RGB, and the alpha it
 * fades in to), fading in by 16 a frame (while +0x30) then out by 4 (alpha now +0x24); +0x31
 * set once it is gone ---- */

extern void *D_00479AE0[];

/* +0x8 destructor */
/* 0x00361260 */
void *SplashRing_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479AE0;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046F580;
        }
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* +0xC set up */
/* 0x00361930 */
void SplashRing_Start(u8 *o) {
    AT(o, 0x31, u8) = 0;
}

/* +0x10 update (0 once it has faded out) */
/* 0x003618B0 */
s32 SplashRing_Update(u8 *o) {
    if (AT(o, 0x30, u8) == 1) {
        AT(o, 0x24, u16) += 0x10;
        if (AT(o, 0x23, u8) < AT(o, 0x24, u16)) {
            AT(o, 0x24, u16) = AT(o, 0x23, u8);
            AT(o, 0x30, u8) = 0;
        }
    } else {
        if (AT(o, 0x24, u16) < 4) {
            AT(o, 0x31, u8) = 1;
            return 0;
        }
        AT(o, 0x24, u16) -= 4;
    }
    AT(o, 0x28, f32) = AT(o, 0x28, f32) + AT(o, 0x2C, f32);
    return 1;
}

/* +0x18 start: at params' position (+0x0) with its colour (+0x10) and growth (+0x14) */
/* 0x003612C0 */
void SplashRing_SetParams(u8 *o, const u8 *params) {
    if (params == NULL) {
        return;
    }
    sceVu0CopyVector((f32 *)(o + 0x10), (f32 *)params);
    AT(o, 0x20, u8) = params[0x10];
    AT(o, 0x21, u8) = params[0x11];
    AT(o, 0x22, u8) = params[0x12];
    AT(o, 0x23, u8) = params[0x13];
    AT(o, 0x2C, f32) = AT(params, 0x14, f32);
    AT(o, 0x24, u16) = 0;
    AT(o, 0x28, u32) = 0x3E4CCCCD;   /* 0.2 */
    AT(o, 0x30, u8) = 1;
}

#ifdef HG_NATIVE
/* +0x14 draw: the ring as a closed strip (outer and inner point by turns, 22.5 degrees apart),
 * flat in its colour at alpha +0x24, added without depth writes (layer 0x19); only when all of
 * it is in view */
/* 0x00361340 */
void SplashRing_Draw(u8 *o) {
    f32 p[34][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    u8 rgba[34][4];
    f32 st[34][2] = {{0}};
    s32 i;

    if (AT(o, 0x31, u8) == 1) {
        return;
    }
    for (i = 0; i < 16; i++) {
        f32 a = (0x1.921fb60000000p+1f /* 3.1415927 */ * (22.5f * (f32)i)) / 180.0f;
        f32 s = func_0031C248(a), c = func_0031C058(a);
        f32 r = AT(o, 0x28, f32), r2 = r - 0x1.99999a0000000p-3f /* 0.2 */;

        p[i * 2][0] = AT(o, 0x10, f32) + r * s;
        p[i * 2][1] = AT(o, 0x14, f32);
        p[i * 2][2] = AT(o, 0x18, f32) - r * c;
        p[i * 2][3] = 1.0f;
        p[i * 2 + 1][0] = AT(o, 0x10, f32) + r2 * s;
        p[i * 2 + 1][1] = AT(o, 0x14, f32);
        p[i * 2 + 1][2] = AT(o, 0x18, f32) - r2 * c;
        p[i * 2 + 1][3] = 1.0f;
    }
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
    for (i = 0; i < 32; i++) {
        f32 v[4] __attribute__((aligned(16)));

        sceVu0ApplyMatrix(v, clip, p[i]);
        if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
            return;
        }
    }
    sceVu0CopyVector(p[32], p[0]);
    sceVu0CopyVector(p[33], p[1]);
    for (i = 0; i < 34; i++) {
        AT(&p[i][3], 0, u32) = 0;
        rgba[i][0] = AT(o, 0x20, u8);
        rgba[i][1] = AT(o, 0x21, u8);
        rgba[i][2] = AT(o, 0x22, u8);
        rgba[i][3] = (u8)AT(o, 0x24, u16);
    }
    glr_layer(0x19);
    glr_strip(&clip[0][0], 34, &p[0][0], &st[0][0], &rgba[0][0], NULL, 0, 0x40 | GLR_PRIM_ADD | GLR_PRIM_NOZW);
    glr_layer(-1);
}
#endif

/* ---- D_00479AA0 (0x720 bytes): a spray of up to 16 droplets (+0x710 of them), double-
 * buffered (+0x10 + buffer +0x648 * 0x300, a quad record of 0x30 each) and drawn by the quad
 * drawer at +0x610 (texture group 0x10, cell (0x6C, 0x4C) 8 x 8, additive with glow); per
 * droplet its rise speed (+0x64C + i * 4, less +0x70C gravity a frame), heading (+0x68C) and
 * outward speed (+0x6CC). It ends (+0x714) once a droplet has faded out or falls faster than
 * 10 ---- */

extern void *D_00479AA0[];

/* its start parameters: where, colour, how many; sizes, distances out, rise and outward
 * speeds as base + random * spread; the height above `pos` they start at and the gravity */
typedef struct SprayParams {
    f32 pos[4];
    u8 rgba[4];
    s32 n;
    f32 size, sizeRnd;
    f32 dist, distRnd;
    f32 rise, riseRnd;
    f32 speed, speedRnd;
    f32 lift;
    f32 gravity;
} SprayParams;

#define SPRAY_DROP(o, i) ((o) + AT(o, 0x648, s32) * 0x300 + (i) * 0x30 + 0x10)

/* +0x8 destructor (the quad drawer at +0x610 inlined) */
/* 0x003604E0 */
u8 *DropletSpray_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479AA0;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* +0xC set up: the quad drawer's settings */
/* 0x00360AE0 */
void DropletSpray_Start(u8 *o) {
    AT(o, 0x648, s32) = 0;
    AT(o, 0x714, u8) = 0;
    AT(o, 0x618, s64) = -1;
    AT(o, 0x624, s32) = 0;
    AT(o, 0x628, s32) = 0;
    AT(o, 0x62C, s32) = 0;
    AT(o, 0x630, s32) = 0x19;
    AT(o, 0x634, s16) = 0x10;
    AT(o, 0x636, s16) = 0x6C;
    AT(o, 0x638, s16) = 0x4C;
    AT(o, 0x63A, s16) = 8;
    AT(o, 0x63C, s16) = 8;
    AT(o, 0x63E, s16) = 0x200;
    AT(o, 0x640, s16) = 0x100;
    AT(o, 0x642, u8) = 0xC0;
    AT(o, 0x643, u8) = 1;
    AT(o, 0x644, u8) = 1;
    AT(o, 0x645, u8) = 0x10;
    AT(o, 0x646, u8) = 0xFF;
}

/* +0x18 start: n droplets fanned evenly round (each heading jittered by up to 30% of the
 * share), out from pos at their distance and `lift` above it, turned at random */
/* 0x00360570 */
void DropletSpray_SetParams(u8 *o, const SprayParams *sp) {
    static const union { u32 u; f32 f; } k03 = {0x3E99999A}, kPi = {0x40490FDB};
    VObject *rng;
    f32 share, jitter;
    s32 i;

    if (sp == NULL) {
        return;
    }
    AT(o, 0x710, s32) = sp->n;
    AT(o, 0x634, s16) = AT(o, 0x710, s32);
    AT(o, 0x70C, f32) = sp->gravity;
    share = 360.0f / (f32)AT(o, 0x710, s32);
    if (AT(o, 0x710, s32) <= 0) {
        return;
    }
    rng = gRandom;
    jitter = k03.f * share;
    for (i = 0; i < AT(o, 0x710, s32); i++) {
        u8 *p = SPRAY_DROP(o, i);
        f32 off[4] __attribute__((aligned(16)));
        f32 m[4][4] __attribute__((aligned(16)));
        f32 a;

        AT(p, 0x0, s32) = sp->rgba[0];
        AT(p, 0x4, s32) = sp->rgba[1];
        AT(p, 0x8, s32) = sp->rgba[2];
        AT(p, 0xC, s32) = sp->rgba[3];
#define RND() VCALL(rng, 0x1C, f32 (*)(VObject *))(rng)
        off[0] = 0.0f;
        off[1] = 0.0f;
        off[3] = 0.0f;
        off[2] = sp->dist + sp->distRnd * RND();
        a = Angle_Wrap(kPi.f * (jitter - 2.0f * (k03.f * (share * RND())) + (f32)i * share) / 180.0f);
        Mtx_TurnY(m, a);
        Mtx_ApplyVector(off, m, off);
        sceVu0AddVector((f32 *)(p + 0x10), (f32 *)sp->pos, off);
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + sp->lift;
        AT(p, 0x1C, f32) = 1.0f;
        AT(p, 0x20, f32) = AT(p, 0x24, f32) = sp->size + sp->sizeRnd * RND();
        AT(p, 0x28, f32) = kPi.f * (360.0f * (VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0.5f)) / 180.0f;
        AT(p, 0x2C, s32) = 0;
        AT(o, 0x64C + i * 4, f32) = sp->rise + sp->riseRnd * RND();
        AT(o, 0x6CC + i * 4, f32) = sp->speed + sp->speedRnd * RND();
#undef RND
        AT(o, 0x68C + i * 4, f32) = a;
    }
}

/* +0x10 update (0 once it has ended): swap buffers, carry each droplet over and move it, its
 * alpha down by one */
/* 0x003608F0 */
s32 DropletSpray_Update(u8 *o) {
    s32 i, k;

    if (AT(o, 0x714, u8) == 1) {
        return 0;
    }
    AT(o, 0x714, u8) = 0;
    AT(o, 0x648, s32) ^= 1;
    for (i = 0; i < AT(o, 0x710, s32); i++) {
        u32 buf = AT(o, 0x648, u32);
        u32 *dst = &AT(o, 0x10 + buf * 0x300 + i * 0x30, u32);
        u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0x300 + i * 0x30, u32);
        f32 v[4] __attribute__((aligned(16)));
        f32 m[4][4] __attribute__((aligned(16)));
        u8 *p;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        v[0] = 0.0f;
        v[1] = 0.0f;
        p = SPRAY_DROP(o, i);
        v[2] = AT(o, 0x6CC + i * 4, f32);
        v[3] = 0.0f;
        Mtx_TurnY(m, AT(o, 0x68C + i * 4, f32));
        Mtx_ApplyVector(v, m, v);
        AT(o, 0x64C + i * 4, f32) = AT(o, 0x64C + i * 4, f32) - AT(o, 0x70C, f32);
        AT(p, 0x10, f32) = AT(p, 0x10, f32) + v[0];
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + AT(o, 0x64C + i * 4, f32);
        AT(p, 0x18, f32) = AT(p, 0x18, f32) + v[2];
        AT(p, 0xC, s32)--;
        if (AT(p, 0xC, s32) == 0 || AT(o, 0x64C + i * 4, f32) < -10.0f) {
            AT(o, 0x714, u8) = 1;
        }
    }
    return 1;
}

/* +0x14 draw (until it has ended) */
/* 0x003608A0 */
void DropletSpray_Draw(u8 *o) {
    if (AT(o, 0x714, u8) != 0) {
        return;
    }
    AT(o, 0x620, u8 *) = SPRAY_DROP(o, 0);
    func_002E56C0(o + 0x610);
}

/* ---- the drips that make those: D_00479A60 (0x640 bytes) 14 drops falling from fixed spots
 * of room 0xC0's ceiling (y 40) to the floor under them, D_00479A80 (0xC0 bytes) one drop at
 * (-226, -100) from y 30 into water at 0. Each lands as a spray (and the single one with a
 * ring) and starts over after a random wait ---- */

/* the ring's start parameters (D_00479AE0) */
typedef struct RingParams {
    f32 pos[4];
    u8 rgba[4];
    f32 size;
} RingParams;

static inline void ring_init(void **obj) {
    obj[0] = D_00479AE0;
}

static inline void spray_init(void **obj) {
    obj[0] = D_00479AA0;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

#define DRIPS_REC(o, buf, i) ((QuadRec *)((o) + 0x10 + (buf) * 0x2A0) + (i))

/* (re)start drop `i` within 20 of its spot (x, z) at y 40, falling 1.5..2.5 a frame after up to
 * a second; the floor triangle under it kept (+0x5FC) */
void func_0035F640(u8 *o, s32 i) {
    static const f32 kSpots[14][2] = {
        {85.0f, -146.0f}, {92.0f, -194.0f}, {83.0f, -237.0f}, {65.0f, -272.0f}, {39.0f, -284.0f},
        {-7.0f, -291.0f}, {-47.0f, -284.0f}, {-78.0f, -256.0f}, {-96.0f, -221.0f}, {-101.0f, -187.0f},
        {-82.0f, -166.0f}, {-39.0f, -172.0f}, {3.0f, -175.0f}, {39.0f, -166.0f},
    };
    static const union { u32 u; f32 f; } k360 = {0x43B40000}, kPi = {0x40490FDB}, k60 = {0x42700000};   /* multiplied first */
    VObject *rnd;
    QuadRec *r;
    f32 x = kSpots[i][0], z = kSpots[i][1];

    r = DRIPS_REC(o, AT(o, 0x5C0, s32), i);
    r->rgba[0] = 0x20;
    r->rgba[1] = 0x20;
    r->rgba[2] = 0x20;
    r->rgba[3] = 0x70;
    rnd = gRandom;
    r->pos[0] = (20.0f + x) - 40.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
    r->pos[1] = 40.0f;
    r->pos[2] = (20.0f + z) - 40.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
    r->pos[3] = 1.0f;
    r->w = 0.5f;
    r->h = 0.5f;
    r->turn = kPi.f * (k360.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f)) / 180.0f;
    r->frame = 0;
    AT(o, 0x588 + i * 4, f32) = 1.5f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    AT(o, 0x5C4 + i * 4, s32) = (s32)(k60.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd));
    AT(o, 0x5FC + i * 4, s32) = VCALL(gNavMesh, 0x3C, s32 (*)(VObject *, f32 *, s32))((VObject *)gNavMesh, r->pos, 0);
}

/* +0x10 update: flip the buffers; each drop carried over, waiting, or falling until under its
 * floor (none: below 0, restarted quietly): there a spray of 16 (and a drip sound, one of three
 * in turn, if the driver has room), and it starts over */
/* 0x0035F9E0 */
s32 CeilingDrips_Update(u8 *o) {
    VObject *nav = (VObject *)gNavMesh;
    u8 *mgr = gEffects;
    VObject *snd = gSound;
    f32 g[4] __attribute__((aligned(16)));
    s32 i, k;

    AT(o, 0x5C0, s32) ^= 1;
    for (i = 0; i < 14; i++) {
        QuadRec *r;

        for (k = 0; k < 12; k++) {
            ((u32 *)DRIPS_REC(o, AT(o, 0x5C0, s32), i))[k] = ((u32 *)DRIPS_REC(o, AT(o, 0x5C0, s32) ^ 1, i))[k];
        }
        if (AT(o, 0x5C4 + i * 4, s32) != 0) {
            AT(o, 0x5C4 + i * 4, s32)--;
            continue;
        }
        r = DRIPS_REC(o, AT(o, 0x5C0, s32), i);
        r->pos[1] = r->pos[1] - AT(o, 0x588 + i * 4, f32);
        if (AT(o, 0x5FC + i * 4, s32) == -1) {
            if (r->pos[1] < 0.0f) {
                func_0035F640(o, i);
            }
            continue;
        }
        sceVu0CopyVector(g, r->pos);
        VCALL(nav, 0x14, void (*)(VObject *, s32, f32 *))(nav, AT(o, 0x5FC + i * 4, s32), g);
        if (r->pos[1] < g[1]) {
            SprayParams sp __attribute__((aligned(16)));

            sceVu0CopyVector(sp.pos, g);
            sp.rgba[0] = 0x20;
            sp.rgba[1] = 0x20;
            sp.rgba[2] = 0x20;
            sp.rgba[3] = 0x10;
            sp.n = 0x10;
            sp.size = 0x1.99999ap-2f;      /* 0.4 */
            sp.sizeRnd = 0x1.99999ap-2f;
            sp.dist = 0x1.99999ap-4f;      /* 0.1 */
            sp.distRnd = 0x1.99999ap-3f;   /* 0.2 */
            sp.rise = 0x1.99999ap-4f;
            sp.riseRnd = 0x1.99999ap-3f;
            sp.speed = 0x1.99999ap-4f;
            sp.speedRnd = 0x1.99999ap-3f;
            sp.lift = 0.5f;
            sp.gravity = 0x1.99999ap-4f;
            EffectMgr_Start(mgr, Effect_New(mgr, 0x720, spray_init), &sp);
            func_0035F640(o, i);
            if ((VCALL(snd, 0xA4, s32 (*)(VObject *, s32))(snd, 6) & 0xFF) == 1) {
                Sound_PlayBankAt(snd, AT(o, 0x634, s32) | 0x40000000, 6, sp.pos, 0, 0);
                AT(o, 0x634, s32)++;
                if (AT(o, 0x634, s32) >= 3) {
                    AT(o, 0x634, s32) = 0;
                }
            }
        }
    }
    return 1;
}

/* +0xC set up: the drawer (14 quads of an 8 x 8 cell at (108, 76), additive with glow), every
 * drop started */
/* 0x0035FDD0 */
void CeilingDrips_Start(u8 *o) {
    s32 i;

    AT(o, 0x5C0, s32) = 0;
    AT(o, 0x558, s64) = -1;
    AT(o, 0x564, s32) = 0;
    AT(o, 0x568, s32) = 0;
    AT(o, 0x56C, s32) = 0;
    AT(o, 0x570, s32) = 0x19;
    AT(o, 0x574, s16) = 0xE;
    AT(o, 0x576, s16) = 0x6C;
    AT(o, 0x578, s16) = 0x4C;
    AT(o, 0x57A, s16) = 8;
    AT(o, 0x57C, s16) = 8;
    AT(o, 0x57E, s16) = 0x200;
    AT(o, 0x580, s16) = 0x100;
    AT(o, 0x582, s8) = -0x40;
    AT(o, 0x583, s8) = 1;
    AT(o, 0x584, s8) = 1;
    AT(o, 0x585, s8) = 0x10;
    AT(o, 0x586, s8) = -1;
    for (i = 0; i < 14; i++) {
        func_0035F640(o, i);
    }
    AT(o, 0x634, s32) = 0;
}

#define DRIP_REC(o, buf) ((QuadRec *)((o) + 0x10 + (buf) * 0x30))

/* (re)start the single drop within 8 of (-226, -100) at y 30, falling 1.5..2.5 a frame after
 * 30..120 frames */
void func_0035FF30(u8 *o) {
    static const union { u32 u; f32 f; } k360 = {0x43B40000}, kPi = {0x40490FDB}, k90 = {0x42B40000};   /* multiplied first */
    VObject *rnd = gRandom;
    QuadRec *r = DRIP_REC(o, AT(o, 0xAC, s32));

    r->rgba[0] = 0x20;
    r->rgba[1] = 0x20;
    r->rgba[2] = 0x20;
    r->rgba[3] = 0x70;
    r->pos[0] = -226.0f - 8.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
    r->pos[1] = 30.0f;
    r->pos[2] = -100.0f - 8.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
    r->pos[3] = 1.0f;
    r->w = 0.5f;
    r->h = 0.5f;
    r->turn = kPi.f * (k360.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f)) / 180.0f;
    r->frame = 0;
    AT(o, 0xA8, f32) = 1.5f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    AT(o, 0xB0, s32) = (s32)(k90.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) + 30;
}

/* 0x003600D0 */
void OneDrip_Draw(u8 *o) {
    quad_step(o, 0x70, 0xAC, 0x30);
}

/* +0x10 update: flip the buffers; the drop waiting or falling; once below 0 a ring and a
 * single droplet where it hit (at 0.1), and it starts over */
/* 0x00360100 */
s32 OneDrip_Update(u8 *o) {
    QuadRec *r;
    s32 k;

    AT(o, 0xAC, s32) ^= 1;
    for (k = 0; k < 12; k++) {
        ((u32 *)DRIP_REC(o, AT(o, 0xAC, s32)))[k] = ((u32 *)DRIP_REC(o, AT(o, 0xAC, s32) ^ 1))[k];
    }
    if (AT(o, 0xB0, s32) != 0) {
        AT(o, 0xB0, s32)--;
        return 1;
    }
    r = DRIP_REC(o, AT(o, 0xAC, s32));
    r->pos[1] = r->pos[1] - AT(o, 0xA8, f32);
    if (r->pos[1] < 0.0f) {
        RingParams rp __attribute__((aligned(16)));
        SprayParams sp __attribute__((aligned(16)));
        u8 *mgr;

        sceVu0CopyVector(rp.pos, r->pos);
        rp.pos[1] = 0x1.99999ap-4f;   /* 0.1 */
        rp.rgba[0] = 0x40;
        rp.rgba[1] = 0x40;
        rp.rgba[2] = 0x40;
        rp.rgba[3] = 0x30;
        rp.size = 0x1.333334p-3f;     /* 0.15 */
        mgr = gEffects;
        EffectMgr_Start(mgr, Effect_New(mgr, 0x40, ring_init), &rp);
        sceVu0CopyVector(sp.pos, r->pos);
        sp.pos[1] = 0x1.99999ap-4f;
        sp.rgba[0] = 0x20;
        sp.rgba[1] = 0x20;
        sp.rgba[2] = 0x20;
        sp.rgba[3] = 0x30;
        sp.n = 1;
        sp.size = 0x1.99999ap-2f;     /* 0.4 */
        sp.sizeRnd = 0x1.99999ap-3f;  /* 0.2 */
        sp.dist = 0.0f;
        sp.distRnd = 0.0f;
        sp.rise = 0x1.333334p-2f;     /* 0.3 */
        sp.riseRnd = 0x1.99999ap-3f;
        sp.speed = 0.0f;
        sp.speedRnd = 0.0f;
        sp.lift = 0.5f;
        sp.gravity = 0x1.99999ap-4f;
        EffectMgr_Start(mgr, Effect_New(mgr, 0x720, spray_init), &sp);
        func_0035FF30(o);
    }
    return 1;
}

/* +0xC set up: the drawer (one quad of an 8 x 8 cell at (108, 76), additive with glow), the
 * drop started */
/* 0x00360460 */
void OneDrip_Start(u8 *o) {
    AT(o, 0xAC, s32) = 0;
    AT(o, 0x78, s64) = -1;
    AT(o, 0x84, s32) = 0;
    AT(o, 0x88, s32) = 0;
    AT(o, 0x8C, s32) = 0;
    AT(o, 0x90, s32) = 0x19;
    AT(o, 0x94, s16) = 1;
    AT(o, 0x96, s16) = 0x6C;
    AT(o, 0x98, s16) = 0x4C;
    AT(o, 0x9A, s16) = 8;
    AT(o, 0x9C, s16) = 8;
    AT(o, 0x9E, s16) = 0x200;
    AT(o, 0xA0, s16) = 0x100;
    AT(o, 0xA2, s8) = -0x40;
    AT(o, 0xA3, s8) = 1;
    AT(o, 0xA4, s8) = 1;
    AT(o, 0xA5, s8) = 0x10;
    AT(o, 0xA6, s8) = -1;
    func_0035FF30(o);
}

/* ---- D_00479560 (room effect 0x1A of room 0x32): the mirror fragment's reflection - the
 * placed object +0x10 ("a_fragment0"), its kind +0x14 and alpha +0x18, size / drop / strength
 * +0x50.. ---- */

extern void *D_00479560[];

/* +0x8 destructor */
void *func_003559D0(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479560;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046D730;
        }
        if ((s16)flags > 0) {
            RoomEffects_delete(o);
        }
    }
    return o;
}

/* +0xC set up, +0x10 update: nothing */
void func_003570B0(void) {
}

void func_003570A0(void) {
}

/* +0x18 start: the parameters; the renderer's reflection pass on (+0x8C (0)) */
void func_00355A30(u8 *o, const u8 *params) {
    if (params == NULL) {
        return;
    }
    AT(o, 0x50, f32) = AT(params, 0x0, f32);
    AT(o, 0x54, f32) = AT(params, 0x4, f32);
    AT(o, 0x58, f32) = AT(params, 0x8, f32);
    AT(o, 0x5C, s32) = 0;
    AT(o, 0x10, s32) = AT(params, 0xC, s32);
    AT(o, 0x14, s32) = AT(params, 0x10, s32);
    AT(o, 0x18, s32) = AT(params, 0x14, s32);
    VCALL(gRenderer, 0x8C, void (*)(VObject *, s32))(gRenderer, 0);
}

#ifdef HG_NATIVE

/* +0x14 draw: the fragment's reflection pass (renderer +0x20.. / camera; not ported yet) */
void func_00355AA0(u8 *o) {
    (void)o;
    glr_todo("mirror fragment reflection (func_00355AA0)");
}
#endif

/* ---- D_00479890 (8 bytes, room 0x48): a soft shadow on the wall at x 127 (z -114 .. -130,
 * y 1 .. 26) - eight black quads stepping 0.1 off the wall, alpha 0x80 down to 0x10; +0x4 set
 * (its start) it is gone ---- */

extern void *D_00479890[];

/* +0x8 destructor */
/* 0x0035C9F0 */
void *WallShadow_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479890;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046F580;
        }
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* +0xC set up */
/* 0x0035CE30 */
void WallShadow_Start(u8 *o) {
    AT(o, 0x4, u8) = 0;
}

/* +0x10 update: 0 once gone */
/* 0x0035CE20 */
s32 WallShadow_Update(u8 *o) {
    return (AT(o, 0x4, u8) ^ 1) != 0;
}

/* +0x18 start: gone */
/* 0x0035CA50 */
void WallShadow_SetParams(u8 *o) {
    AT(o, 0x4, u8) = 1;
}

#ifdef HG_NATIVE
/* +0x14 draw (until gone, and only when all of it is in view): the eight quads, layer 0x1E */
/* 0x0035CA60 */
void WallShadow_Draw(u8 *o) {
    f32 p[4][4] __attribute__((aligned(16))) = {
        {127.0f, 26.0f, -114.0f, 1.0f}, {127.0f, 26.0f, -130.0f, 1.0f},
        {127.0f, 1.0f, -114.0f, 1.0f}, {127.0f, 1.0f, -130.0f, 1.0f},
    };
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 st[4][2] = {{0}};
    u8 rgba[4][4];
    s32 i, k;

    if (AT(o, 0x4, u8) == 1) {
        return;
    }
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
    for (i = 0; i < 4; i++) {
        f32 v[4] __attribute__((aligned(16)));

        sceVu0ApplyMatrix(v, clip, p[i]);
        if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
            return;
        }
    }
    glr_layer(0x1E);
    for (k = 0; k < 8; k++) {
        f32 xyzw[4][4] __attribute__((aligned(16)));

        for (i = 0; i < 4; i++) {
            sceVu0CopyVector(xyzw[i], p[i]);
            AT(&xyzw[i][3], 0, u32) = 0;
            rgba[i][0] = 0;
            rgba[i][1] = 0;
            rgba[i][2] = 0;
            rgba[i][3] = (u8)((8 - k) << 4);
        }
        glr_strip(&clip[0][0], 4, &xyzw[0][0], &st[0][0], &rgba[0][0], NULL, 0, 0x40 | GLR_PRIM_NOZW);
        for (i = 0; i < 4; i++) {
            p[i][0] -= 0x1.99999a0000000p-4f /* 0.1 */;
        }
    }
    glr_layer(-1);
}
#endif

/* ---- D_00472BF0 (0x140 bytes): a splat on the floor - one quad (two buffers of a record at
 * +0x10 + 0x30 x the current one +0x134) drawn as four corners (+0xF0..+0x120, the drawer at
 * +0x70 takes them) in the floor's frame (+0xB0: side, normal, along), growing (+0x130) while
 * it fades; +0x138 gone, +0x139 its kind (0 a blood splat, else a dark stain) ---- */

#define SPLAT_REC(e) ((QuadRec *)((e) + 0x10) + AT(e, 0x134, s32))

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x003161F0 */
u8 *FloorSplat_dtor(u8 *e, s32 flags) {
    if (e == NULL) {
        return e;
    }
    AT(e, 0x0, void **) = D_00472BF0;
    AT(e, 0x70, void **) = D_0046FC30;
    AT(e, 0x70, void **) = D_00469D00;
    AT(e, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(e);
    }
    return e;
}

/* the quad drawer for a splat of the kind: its corners, layer 0x19, one 8 x 8 cell at (108, 76)
 * (blood, mode 2) or 32 x 32 at (320, 128) (stain, mode 0x42) */
static inline __attribute__((always_inline)) void splat_drawer(u8 *e, s32 stain) {
    QuadDrawer *q = (QuadDrawer *)(e + 0x70);

    q->tex = -1;
    AT(e, 0x84, u8 *) = e + 0xF0;
    q->cx = 0.0f;
    q->cy = 0.0f;
    q->layer = 0x19;
    q->count = 1;
    if (!stain) {
        q->cellX = 0x6C;
        q->cellY = 0x4C;
        q->cellW = 8;
        q->cellH = 8;
    } else {
        q->cellX = 0x140;
        q->cellY = 0x80;
        q->cellW = 0x20;
        q->cellH = 0x20;
    }
    q->texW = 0x200;
    q->texH = 0x100;
    q->flags = stain ? 0x42 : 2;
    q->frames = 1;
    q->texId = 1;
    q->texGroup = 0x10;
    q->palette = -1;
}

static inline void splat_corner(u8 *e, s32 off, f32 x, f32 z) {
    AT(e, off, f32) = x;
    AT(e, off + 4, f32) = 0.0f;
    AT(e, off + 8, f32) = z;
    AT(e, off + 12, f32) = 1.0f;
}

/* +0x18 start (arg: position, nav tri +0x10, kind +0x14): blood dark red with some alpha, a
 * stain brown; dropped onto the floor below (nav +0x40, from the point to the record) - none
 * or a hole: gone - lifted half the floor normal (+0x2C) along it, its frame built round the
 * normal unless that is straight up */
/* 0x00316280 */
void FloorSplat_SetParams(u8 *e, u8 *arg) {
    f32 at[4] __attribute__((aligned(16)));
    f32 out[4] __attribute__((aligned(16)));
    QuadRec *r;
    u32 tri, t;
    u8 *nm;

    sceVu0CopyVector(at, (f32 *)arg);
    tri = AT(arg, 0x10, u32);
    AT(e, 0x139, u8) = AT(arg, 0x14, u8);
    if (arg == NULL) {
        return;
    }
    splat_drawer(e, AT(e, 0x139, u8) != 0);
    if (tri == (u32)-1) {
        AT(e, 0x138, u8) = 1;
        return;
    }
    r = SPLAT_REC(e);
    if (AT(e, 0x139, u8) != 0) {
        r->rgba[0] = 0x40;
        r->rgba[1] = 0x10;
        r->rgba[2] = 8;
        r->rgba[3] = 0x20;
    } else {
        r->rgba[0] = 0x20;
        r->rgba[1] = 0;
        r->rgba[2] = 0;
        r->rgba[3] = (VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 0x1F) + 0x20;
    }
    r->pos[0] = at[0];
    r->pos[1] = at[1];
    r->pos[2] = at[2];
    AT(&r->pos[3], 0, f32) = 1.0f;
    r->w = 1.0f;
    r->h = 1.0f;
    r->turn = 0.0f;
    r->frame = 0;
    AT(e, 0x130, f32) = 2.0f;
    splat_corner(e, 0xF0, -2.0f, -2.0f);
    splat_corner(e, 0x100, 2.0f, -2.0f);
    splat_corner(e, 0x110, -2.0f, 2.0f);
    splat_corner(e, 0x120, 2.0f, 2.0f);
    nm = (u8 *)gNavMesh;
    t = VCALL((VObject *)nm, 0x40, u32 (*)(VObject *, u32, f32 *, f32 *, f32 *, s32))(
        (VObject *)nm, tri, out, at, r->pos, 0);
    if (t == (u32)-1) {
        AT(e, 0x138, u8) = 1;
        return;
    }
    {
        u32 fl = 0;   /* (the original reads a NULL record for a triangle out of range) */

        if (t < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) {
            fl = AT(AT(nm, 0x4, u8 *) + t * 0x50, 0x3C, u32);
        }
        if (fl & 0x10000000) {
            AT(e, 0x138, u8) = 1;
            return;
        }
    }
    sceVu0UnitMatrix((f32 (*)[4])(e + 0xB0));
    sceVu0CopyVector(r->pos, out);
    nm = (u8 *)gNavMesh;
    VCALL((VObject *)nm, 0x14, void (*)(VObject *, u32, f32 *))((VObject *)nm, t, r->pos);
    VCALL((VObject *)nm, 0x2C, void (*)(VObject *, u32, f32 *))((VObject *)nm, t, (f32 *)(e + 0xC0));
    sceVu0CopyVector(out, (f32 *)(e + 0xC0));
    sceVu0ScaleVector(out, out, 0.5f);
    sceVu0AddVector(r->pos, out, r->pos);
    sceVu0Normalize((f32 *)(e + 0xC0), (f32 *)(e + 0xC0));
    if (AT(e, 0xC4, f32) == 1.0f) {
        return;
    }
    AT(e, 0xB0, f32) = 0.0f;
    AT(e, 0xB4, f32) = -1.0f;
    AT(e, 0xB8, f32) = 0.0f;
    AT(e, 0xBC, f32) = 0.0f;
    sceVu0OuterProduct((f32 *)(e + 0xB0), (f32 *)(e + 0xC0), (f32 *)(e + 0xB0));
    sceVu0Normalize((f32 *)(e + 0xB0), (f32 *)(e + 0xB0));
    sceVu0OuterProduct((f32 *)(e + 0xD0), (f32 *)(e + 0xC0), (f32 *)(e + 0xB0));
    sceVu0Normalize((f32 *)(e + 0xD0), (f32 *)(e + 0xD0));
}

/* +0x14 draw, until gone */
/* 0x00316660 */
void FloorSplat_Draw(u8 *e) {
    if (AT(e, 0x138, u8) != 0) {
        return;
    }
    AT(e, 0x80, QuadRec *) = SPLAT_REC(e);
    func_002E56C0(e + 0x70);
}

/* +0x10 update (0 once gone): flip the buffers (the new one copied from the old); while seen
 * it fades (blood by 0..3 a frame, a stain one in four frames) and grows by 0.1, its corners
 * laid out again in the floor's frame */
/* 0x003166B0 */
s32 FloorSplat_Update(u8 *e) {
    QuadRec *r;

    if (AT(e, 0x138, u8) == 1) {
        return 0;
    }
    AT(e, 0x138, u8) = 1;
    AT(e, 0x134, s32) ^= 1;
    *SPLAT_REC(e) = ((QuadRec *)(e + 0x10))[AT(e, 0x134, s32) ^ 1];
    r = SPLAT_REC(e);
    if (r->rgba[3] > 0) {
        f32 s;

        AT(e, 0x138, u8) = 0;
        if (AT(e, 0x139, u8) != 0) {
            if ((VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 3) == 1) {
                r->rgba[3] -= 1;
            }
        } else {
            r->rgba[3] -= VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 3;
        }
        if (r->rgba[3] < 0) {
            r->rgba[3] = 0;
        }
        AT(e, 0x130, f32) = AT(e, 0x130, f32) + 0x1.99999ap-4f /* 0.1 */;
        s = AT(e, 0x130, f32);
        splat_corner(e, 0xF0, -s, -s);
        splat_corner(e, 0x100, s, -s);
        splat_corner(e, 0x110, -s, s);
        splat_corner(e, 0x120, s, s);
        sceVu0ApplyMatrix((f32 *)(e + 0xF0), (f32 (*)[4])(e + 0xB0), (f32 *)(e + 0xF0));
        sceVu0ApplyMatrix((f32 *)(e + 0x100), (f32 (*)[4])(e + 0xB0), (f32 *)(e + 0x100));
        sceVu0ApplyMatrix((f32 *)(e + 0x110), (f32 (*)[4])(e + 0xB0), (f32 *)(e + 0x110));
        sceVu0ApplyMatrix((f32 *)(e + 0x120), (f32 (*)[4])(e + 0xB0), (f32 *)(e + 0x120));
    }
    return 1;
}

/* 0x003168E0 */
void FloorSplat_Start(u8 *self) {
    S32(self, 0x134) = 0;
    self[0x138] = 0;
}

/* ---- the marker effect D_00470F90 (Lorenzo's, room 0x66's): a flickering glow with sparks.
 *   +0x4 what it follows (a model)   +0x8 its size (1.5 and over: a fixed glow colour)
 *   +0xC a slot to look for room from   +0x10 its state (1 on, 2 ending, 3 ended, 4 resized)
 *   +0x14 / +0x18 two angles   +0x1C done   +0x1D on   +0x1E the second kind ---- */

extern void *D_00479E50[];

static void marker_spark_init(void **obj) {
    obj[0] = D_00479E50;
    obj[0xD0 / 4] = D_00469D00;
    ((s32 *)obj)[0xD4 / 4] = -1;
    obj[0xD0 / 4] = D_0046FC30;
    obj[0x108 / 4] = D_00469D00;
    ((s32 *)obj)[0x10C / 4] = -1;
    obj[0x108 / 4] = D_0046FC30;
}

/* a spark (D_00479E50, 0x170 bytes) in the first free slot from `from` (-1: from 0); -1 if none */
static s32 spark_new(u8 *mgr, s32 from) {
    void *mem = VCALL(EFFECT_HEAP(mgr), 0x10, void *(*)(VObject *, u32))(EFFECT_HEAP(mgr), 0x170);
    s32 i;

    if (mem == NULL) {
        return -1;
    }
    for (i = from == -1 ? 0 : from; i < EFFECT_NUM_SLOTS; i++) {
        if (EFFECT_SLOTS(mgr)[i] == NULL) {
            void **obj = EffectMgr_new(0x170, mem);

            if (obj != NULL) {
                marker_spark_init(obj);
            }
            EFFECT_SLOTS(mgr)[i] = obj;
            VCALL(EFFECT_SLOTS(mgr)[i], 0xC, void (*)(void **))(EFFECT_SLOTS(mgr)[i]);
            return i;
        }
    }
    return -1;
}

typedef struct SparkPrm {
    f32 size;     /* the marker's +0x8 */
    u32 follow;   /* +0x4 (or Fiona's model) */
    s32 kind;
    f32 scale;
    u32 from;     /* +0xC */
    f32 flag;
    f32 second;   /* +0x1E */
} SparkPrm;

static s32 rnd_int(void) {
    return VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom);
}

/* ---- the spark itself (D_00479E50, 0x170 bytes): two quads - a glow (records +0x10 / +0x40,
 * drawer +0xD0, velocity +0x150) and a flare (records +0x70 / +0xA0, drawer +0x108, velocity
 * +0x15C), the current frame +0x168; at a bone (+0x144) of a model (+0x140), size +0x148, the
 * marker's slot +0x14C; +0x16C gone, +0x16D placed at the bone again next frame, +0x16E
 * flickering ---- */

#define SPARK_GLOW(o, buf) ((QuadRec *)((o) + 0x10 + (buf) * 0x30))
#define SPARK_FLARE(o, buf) ((QuadRec *)((o) + 0x70 + (buf) * 0x30))

/* one quad of a spark flickered by `k`: its colour scaled, thrown 20 x its velocity x k, grown
 * by k */
static inline void spark_flicker(QuadRec *r, const f32 *v, f32 k) {
    r->rgba[0] = (s32)((f32)r->rgba[0] * k);
    r->rgba[1] = (s32)((f32)r->rgba[1] * k);
    r->rgba[2] = (s32)((f32)r->rgba[2] * k);
    r->rgba[3] = (s32)((f32)r->rgba[3] * k);
    r->pos[0] = r->pos[0] + 20.0f * (v[0] * k);
    r->pos[1] = r->pos[1] + 20.0f * (v[1] * k);
    r->pos[2] = r->pos[2] + 20.0f * (v[2] * k);
    r->w = r->w + k;
    r->h = r->w;
}

/* both quads moved to the bone; flickering: by a random amount */
void func_00366000(u8 *o) {
    f32 p[4] __attribute__((aligned(16)));
    f32 k;

    AT(o, 0x16D, u8) = 0;
    sceVu0CopyVector(p, Skel_Bone(AT(AT(o, 0x140, u8 *), 0x810, void *), AT(o, 0x144, s32)) + 12);
    sceVu0CopyVector(SPARK_GLOW(o, AT(o, 0x168, s32))->pos, p);
    sceVu0CopyVector(SPARK_FLARE(o, AT(o, 0x168, s32))->pos, p);
    if (!AT(o, 0x16E, u8)) {
        return;
    }
    k = VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom);
    spark_flicker(SPARK_GLOW(o, AT(o, 0x168, s32)), (f32 *)(o + 0x150), k);
    spark_flicker(SPARK_FLARE(o, AT(o, 0x168, s32)), (f32 *)(o + 0x15C), k);
}

/* a random turn (-pi..pi) and the first frame */
static inline void spark_turn(QuadRec *r, VObject *rnd) {
    static const union { u32 u; f32 f; } k360 = {0x43B40000}, kPi = {0x40490FDB};   /* multiplied first */

    r->turn = kPi.f * (k360.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f)) / 180.0f;
    r->frame = 0;
}

static inline void spark_white(QuadRec *r, s32 alpha) {
    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = alpha;
}

/* +0x18 start: arg (SparkPrm) { size, model, bone, scale, the marker's slot, flickering, other
 * cells }: a size 0.49 or under gives a small glow and a tiny rising flare (cell (192, 64),
 * palette 3), else a glow and a flare dimmed under size 1; other cells: (320, 160) 64 x 64,
 * palette 7. None: gone */
/* 0x00366270 */
void Spark_SetParams(u8 *o, SparkPrm *arg) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD}, k02 = {0x3E4CCCCD}, k04 = {0x3ECCCCCD};   /* multiplied first */
    VObject *rnd;
    QuadRec *r;
    f32 size;

    if (arg == NULL) {
        AT(o, 0x16C, u8) = 1;
        return;
    }
    size = arg->size;
    AT(o, 0x16D, u8) = 1;
    AT(o, 0x140, u32) = arg->follow;
    AT(o, 0x144, s32) = arg->kind;
    AT(o, 0x148, f32) = arg->scale * size;
    if (AT(o, 0x148, f32) < 0x1.99999ap-4f) {
        AT(o, 0x148, f32) = 0x1.99999ap-4f;   /* 0.1 */
    }
    AT(o, 0x14C, u32) = arg->from;
    if (size <= 0x1.f5c290p-2f) {   /* 0.49 */
        rnd = gRandom;
        r = SPARK_GLOW(o, AT(o, 0x168, s32));
        spark_white(r, 0x40);
        r->w = 1.0f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        r->h = r->w;
        spark_turn(r, rnd);
        AT(o, 0x150, f32) = k01.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0x154, u32) = 0x3DCCCCCD;   /* 0.1 */
        AT(o, 0x158, f32) = k01.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0x110, s64) = -1;
        AT(o, 0x11C, s32) = 0;
        AT(o, 0x120, s32) = 0;
        AT(o, 0x124, s32) = 0;
        AT(o, 0x128, s32) = 0x19;
        AT(o, 0x12C, s16) = 1;
        AT(o, 0x12E, s16) = 0xC0;
        AT(o, 0x130, s16) = 0x40;
        AT(o, 0x132, s16) = 0x20;
        AT(o, 0x134, s16) = 0x20;
        AT(o, 0x136, s16) = 0x200;
        AT(o, 0x138, s16) = 0x100;
        AT(o, 0x13A, s8) = 0;
        AT(o, 0x13B, s8) = 1;
        AT(o, 0x13C, s8) = 1;
        AT(o, 0x13D, s8) = 0x10;
        AT(o, 0x13E, s8) = 3;
        r = SPARK_FLARE(o, AT(o, 0x168, s32));
        spark_white(r, 0x80);
        r->w = 0x1.99999ap-3f + 0x1.99999ap-3f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);   /* 0.2 + 0.2 x */
        r->h = r->w;
        spark_turn(r, rnd);
        AT(o, 0x15C, f32) = k01.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0x160, f32) = k04.f * (0.5f - AT(o, 0x148, f32));
        AT(o, 0x164, f32) = k01.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0x148, f32) = 1.0f;
    } else {
        r = SPARK_GLOW(o, AT(o, 0x168, s32));
        spark_white(r, 0x80);
        if (size < 1.0f) {
            r->rgba[3] = (s32)((f32)r->rgba[3] * size);
        }
        rnd = gRandom;
        r->w = 0.5f + 0.5f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        r->h = r->w;
        spark_turn(r, rnd);
        AT(o, 0x150, f32) = k02.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0x154, f32) = 0.25f;
        AT(o, 0x158, f32) = k02.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        r = SPARK_FLARE(o, AT(o, 0x168, s32));
        spark_white(r, 0x80);
        if (size < 1.0f) {
            r->rgba[3] = (s32)((f32)r->rgba[3] * size);
        }
        rnd = gRandom;
        r->w = 1.0f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        r->h = r->w;
        spark_turn(r, rnd);
        AT(o, 0x15C, f32) = k01.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        AT(o, 0x160, u32) = 0x3DCCCCCD;   /* 0.1 */
        AT(o, 0x164, f32) = k01.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    }
    AT(o, 0x16E, u8) = AT(arg, 0x14, u32) != 0;   /* (the float's bits) */
    if (arg->second == 0.0f) {
        return;
    }
    AT(o, 0xF6, s16) = 0x140;
    AT(o, 0xF8, s16) = 0xA0;
    AT(o, 0xFA, s16) = 0x40;
    AT(o, 0xFC, s16) = 0x40;
    AT(o, 0x106, s8) = 7;
    if (size <= 0x1.f5c290p-2f) {
        return;
    }
    AT(o, 0x12E, s16) = 0x140;
    AT(o, 0x130, s16) = 0xA0;
    AT(o, 0x132, s16) = 0x40;
    AT(o, 0x134, s16) = 0x40;
    AT(o, 0x13E, s8) = 7;
}

/* the spark's shared checks: none outside cutscenes with no stalker; gone with its marker */
static inline s32 spark_live(u8 *o) {
    if (gCharSlot2 == NULL && (u8)VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) == 0) {
        return -1;
    }
    if (EffectMgr_Query(gEffects, AT(o, 0x14C, s32)) == 3) {
        AT(o, 0x16C, u8) = 1;
    }
    return 0;
}

/* +0x14 draw (not while paused): placed at the bone if due, the flare then the glow */
/* 0x00366910 */
void Spark_Draw(u8 *o) {
    if (func_002D6010(gEffects) != 0) {
        return;
    }
    if (spark_live(o) < 0) {
        return;
    }
    if (AT(o, 0x16C, u8)) {
        return;
    }
    if (AT(o, 0x16D, u8) == 1) {
        func_00366000(o);
    }
    AT(o, 0x118, QuadRec *) = SPARK_FLARE(o, AT(o, 0x168, s32));
    func_002E56C0(o + 0x108);
    AT(o, 0xE0, QuadRec *) = SPARK_GLOW(o, AT(o, 0x168, s32));
    func_002E56C0(o + 0xD0);
}

/* one quad's step while it shows: grown by up to `grow`, turned up to 3 degrees (the way the
 * glow drifts), moved, faded by 2 + up to 7 / size and darkened by 1 / size; out once black */
static inline void spark_step(u8 *o, QuadRec *r, const f32 *v, f32 grow) {
    static const union { u32 u; f32 f; } k3 = {0x40400000}, kPi = {0x40490FDB};   /* multiplied first */
    VObject *rnd = gRandom;

    AT(o, 0x16C, u8) = 0;
    r->w = r->w + grow * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    r->h = r->w;
    if (!(AT(o, 0x150, f32) <= 0.0f)) {
        r->turn = r->turn + kPi.f * (k3.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)) / 180.0f;
    } else {
        r->turn = r->turn - kPi.f * (k3.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)) / 180.0f;
    }
    r->pos[0] = r->pos[0] + v[0];
    r->pos[1] = r->pos[1] + v[1];
    r->pos[2] = r->pos[2] + v[2];
    r->rgba[3] = r->rgba[3] - ((s32)((f32)(VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 7) / AT(o, 0x148, f32)) + 2);
    if (r->rgba[3] < 0) {
        r->rgba[3] = 0;
    }
    r->rgba[0] = r->rgba[0] - (s32)(1.0f / AT(o, 0x148, f32));
    if (r->rgba[0] < 0) {
        r->rgba[0] = 0;
    }
    r->rgba[1] = r->rgba[1] - (s32)(1.0f / AT(o, 0x148, f32));
    if (r->rgba[1] < 0) {
        r->rgba[1] = 0;
    }
    r->rgba[2] = r->rgba[2] - (s32)(1.0f / AT(o, 0x148, f32));
    if (r->rgba[2] < 0) {
        r->rgba[2] = 0;
    }
    if (r->rgba[0] + r->rgba[1] + r->rgba[2] == 0) {
        r->rgba[3] = 0;
    }
}

/* +0x10 update: placed at the bone if due; flip the frames, each quad carried over and
 * stepped while it shows (the glow growing up to 0.1, the flare 0.2); 0 once both are out */
/* 0x00366A30 */
s32 Spark_Update(u8 *o) {
    s32 k;

    if (spark_live(o) < 0) {
        return 0;
    }
    if (AT(o, 0x16C, u8) == 1) {
        return 0;
    }
    if (AT(o, 0x16D, u8) == 1) {
        func_00366000(o);
    }
    AT(o, 0x168, s32) ^= 1;
    AT(o, 0x16C, u8) = 1;
    for (k = 0; k < 12; k++) {
        ((u32 *)SPARK_GLOW(o, AT(o, 0x168, s32)))[k] = ((u32 *)SPARK_GLOW(o, AT(o, 0x168, s32) ^ 1))[k];
    }
    if (SPARK_GLOW(o, AT(o, 0x168, s32))->rgba[3] > 0) {
        spark_step(o, SPARK_GLOW(o, AT(o, 0x168, s32)), (f32 *)(o + 0x150), 0x1.99999ap-4f);
    }
    for (k = 0; k < 12; k++) {
        ((u32 *)SPARK_FLARE(o, AT(o, 0x168, s32)))[k] = ((u32 *)SPARK_FLARE(o, AT(o, 0x168, s32) ^ 1))[k];
    }
    if (SPARK_FLARE(o, AT(o, 0x168, s32))->rgba[3] > 0) {
        spark_step(o, SPARK_FLARE(o, AT(o, 0x168, s32)), (f32 *)(o + 0x15C), 0x1.99999ap-3f);
    }
    return 1;
}

/* a spark drawer's settings: one quad of a 32 x 32 cell at (384, 128), blended, palette 6,
 * layer 0x19 */
static inline void spark_drawer(u8 *d) {
    AT(d, 0x8, s64) = -1;
    AT(d, 0x14, s32) = 0;
    AT(d, 0x18, s32) = 0;
    AT(d, 0x1C, s32) = 0;
    AT(d, 0x20, s32) = 0x19;
    AT(d, 0x24, s16) = 1;
    AT(d, 0x26, s16) = 0x180;
    AT(d, 0x28, s16) = 0x80;
    AT(d, 0x2A, s16) = 0x20;
    AT(d, 0x2C, s16) = 0x20;
    AT(d, 0x2E, s16) = 0x200;
    AT(d, 0x30, s16) = 0x100;
    AT(d, 0x32, s8) = 0x40;
    AT(d, 0x33, s8) = 1;
    AT(d, 0x34, s8) = 1;
    AT(d, 0x35, s8) = 0x10;
    AT(d, 0x36, s8) = 6;
}

/* +0xC set up: frame 0, both drawers */
/* 0x003670E0 */
void Spark_Start(u8 *o) {
    AT(o, 0x168, s32) = 0;
    AT(o, 0x16C, u8) = 0;
    AT(o, 0x16D, u8) = 0;
    spark_drawer(o + 0xD0);
    spark_drawer(o + 0x108);
}

/* the marker's sparks: one of 27 kinds and two more of some kinds (the second kind: other
 * kinds, and now and then one on Fiona) */
void func_003012B0(u8 *o, s32 flag) {
    u8 *mgr;
    SparkPrm prm;
    s32 slot, k;

    prm.follow = AT(o, 0x4, u32);
    prm.from = AT(o, 0xC, u32);
    prm.flag = (f32)flag;
    prm.second = (f32)AT(o, 0x1E, u8);
    mgr = gEffects;
    if (AT(o, 0x1E, u8) == 0) {
        slot = spark_new(mgr, AT(o, 0xC, s32));
        k = (u32)rnd_int() % 27;
        prm.size = AT(o, 0x8, f32);
        prm.scale = 1.0f;
        prm.kind = k;
        EffectMgr_Start(mgr, slot, &prm);
        if ((rnd_int() & 1) == 0) {
            slot = spark_new(mgr, AT(o, 0xC, s32));
            k = rnd_int() & 3;
            if (k < 2) {
                k += 6;
            }
            prm.scale = 1.5f;
            prm.size = AT(o, 0x8, f32);
            prm.kind = k;
            EffectMgr_Start(mgr, slot, &prm);
        } else {
            slot = spark_new(mgr, AT(o, 0xC, s32));
            k = (rnd_int() & 3) + 3;
            k = k == 6 ? 5 : k * k;
            prm.size = AT(o, 0x8, f32);
            prm.kind = k;
            prm.scale = k < 11 ? 1.5f : 1.0f;
            EffectMgr_Start(mgr, slot, &prm);
        }
        return;
    }
    slot = spark_new(mgr, AT(o, 0xC, s32));
    k = (rnd_int() & 3) + 3;
    k = k == 6 ? 5 : k == 4 ? 0x14 : k * k;
    prm.size = AT(o, 0x8, f32);
    prm.scale = 1.0f;
    prm.kind = k;
    EffectMgr_Start(mgr, slot, &prm);
    slot = spark_new(mgr, AT(o, 0xC, s32));
    k = rnd_int() & 3;
    if (k < 2) {
        k += 6;
    }
    prm.size = AT(o, 0x8, f32);
    prm.scale = 1.0f;
    prm.kind = k;
    EffectMgr_Start(mgr, slot, &prm);
    if (rnd_int() & 1) {
        slot = spark_new(mgr, AT(o, 0xC, s32));
        k = (rnd_int() & 3) + 2;
        prm.size = AT(o, 0x8, f32);
        prm.follow = AT(gCharSlot4, 0xF0, u32);
        prm.scale = 1.0f;
        prm.kind = k;
        EffectMgr_Start(mgr, slot, &prm);
        return;
    }
    k = (u32)rnd_int() % 27;
    if (k >= 13 && k < 17) {
        return;
    }
    slot = spark_new(mgr, AT(o, 0xC, s32));
    prm.scale = 1.0f;
    prm.size = AT(o, 0x8, f32);
    prm.kind = k;
    EffectMgr_Start(mgr, slot, &prm);
}

/* +0x1C its state */
s32 func_00301D30(u8 *o) {
    return AT(o, 0x10, s32);
}

/* +0x18 set: NULL done; else { state, ... } - 5 / 6 on (the first / second kind), 1 on with
 * { 1, what it follows (0: the stalker's model, in the game), size, from slot } and twenty
 * rounds of sparks, 4 resized { 4, size } */
void func_00301D40(u8 *o, s32 *prm) {
    s32 k;

    if (prm == NULL) {
        AT(o, 0x1C, u8) = 1;
        return;
    }
    AT(o, 0x10, s32) = prm[0];
    k = AT(o, 0x10, s32);
    if ((u32)(k - 5) < 2) {
        AT(o, 0x1E, u8) = k == 5 ? 0 : 1;
        AT(o, 0x1D, u8) = 0;
        AT(o, 0x10, s32) = 1;
    } else if (k == 1) {
        AT(o, 0x1D, u8) = 1;
    }
    k = AT(o, 0x10, s32);
    if (k == 1) {
        u32 f = prm[1];
        s32 i;

        if (f == 0 && gProgress != NULL) {
            f = AT(gCharSlot2, 0xF0, u32);
        }
        AT(o, 0x4, u32) = f;
        AT(o, 0x8, s32) = prm[2];
        AT(o, 0xC, s32) = prm[3];
        for (i = 0; i < 20; i++) {
            func_003012B0(o, 1);
        }
    } else if (k == 4) {
        AT(o, 0x8, s32) = prm[1];
    }
}

/* +0x10 each frame (unless the camera director is busy with nobody in slot 2): unless done, the
 * glow flickers (renderer +0x78: 0.4 x (1 + sin a) x size, a turning by up to 10 degrees at
 * random; size 1.5 and over a fixed colour), b turns by up to 45 degrees, sparks while on; an
 * ending marker is done. 0 when not run */
s32 func_003039A0(u8 *o) {
    VObject *rnd;
    f32 r, v;
    u32 a, c;

    if (gCharSlot2 == NULL && (u8)VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) == 0) {
        return 0;
    }
    if (AT(o, 0x1C, u8) == 1) {
        return 0;
    }
    rnd = gRandom;
    r = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    AT(o, 0x14, f32) = AT(o, 0x14, f32) + 0x1.921fb6p+1f /* pi */ * (10.0f * r) / 180.0f;
    AT(o, 0x14, f32) = Angle_Wrap(AT(o, 0x14, f32));
    v = 0x1.99999a0000000p-2f /* 0.4 */ * (1.0f + func_0031C248(AT(o, 0x14, f32)));
    if (AT(o, 0x8, f32) < 1.0f) {
        v *= AT(o, 0x8, f32);
    }
    a = (u32)(128.0f * v);
    c = a | (a << 24 | (a >> 1) << 8);
    if (!(AT(o, 0x8, f32) <= 1.5f)) {
        c = 0x80004080;
    }
    VCALL(gRenderer, 0x78, void (*)(VObject *, u32))(gRenderer, c);
    r = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    AT(o, 0x18, f32) = AT(o, 0x18, f32) + 0x1.921fb6p+1f /* pi */ * (45.0f * r) / 180.0f;
    AT(o, 0x18, f32) = Angle_Wrap(AT(o, 0x18, f32));
    if (AT(o, 0x10, s32) != 2 && AT(o, 0x10, s32) != 3 && !(AT(o, 0x8, f32) <= 0.0f)) {
        func_003012B0(o, 0);
    }
    if (AT(o, 0x10, s32) == 2) {
        AT(o, 0x1C, u8) = 1;
        AT(o, 0x10, s32) = 3;
    }
    return 1;
}

/* +0xC init */
void func_00303C10(u8 *o) {
    AT(o, 0x18, s32) = 0;
    AT(o, 0x14, s32) = 0;
    AT(o, 0x1C, u8) = 0;
    AT(o, 0x1D, u8) = 1;
    AT(o, 0x1E, u8) = 0;
}

#ifdef HG_NATIVE

/* +0x14 draw (PC; the PS2 sends GS packets): attached to the stalker in slot 2 while it hides
 * (+0x29) it ends (state 3; done if it went to another room); a stalker idle with the camera
 * director free ends it too. Otherwise, while on (+0x1D) and not done, its glow round the root
 * of the model it follows: 32 spheres (radius 10 + 0.3 k, the later ones 0.3 + 0.3 sin(+0x18)
 * more) counted at half size where they show in front of the scene, blurred (the four diagonal
 * neighbours at 0x40 / 0x30 / 0x20 / 0x10), and added over the screen in orange (0x80, 0x60,
 * 0x30) at 64 x min(+0x8, 1) / 128 - layer 0xB */
void func_00301E70(u8 *o) {
    VObject *cam;
    f32 c[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    f32 f[3], r[3], n, sx, sy, scale, z;
    f32 size;

    if (func_002D6010(gEffects) != 0) {
        return;
    }
    if (gCharSlot2 == NULL && !(u8)VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector)) {
        return;
    }
    if (AT(o, 0x10, s32) == 3) {
        AT(o, 0x10, s32) = 1;
    }
    if (gCharSlot2 != NULL && AT(o, 0x4, u8 *) == AT(gCharSlot2, 0xF0, u8 *) && AT(gCharSlot2, 0x29, u8) == 1) {
        AT(o, 0x10, s32) = 3;
        if (VCALL((VObject *)gProgress, 0xC, s32 (*)(VObject *))((VObject *)gProgress) == AT(gCharSlot2, 0x30, s32)) {
            return;
        }
        AT(o, 0x1C, u8) = 1;
    }
    if (gCharSlot2 != NULL && !(u8)VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector)
        && AT(gCharSlot2, 0x28, u8) == 0) {
        AT(o, 0x10, s32) = 3;
        AT(o, 0x1C, u8) = 1;
    }
    if (AT(o, 0x1D, u8) == 0 || AT(o, 0x1C, u8) == 1) {
        return;
    }
    sceVu0CopyVector(c, Skel_Bone(AT(AT(o, 0x4, u8 *), 0x810, void *), 0) + 12);
    c[3] = 1.0f;
    cam = gCamera;
    VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, m);
    sceVu0ApplyMatrix(q, m, c);
    z = q[3];
    if (!(z > 0.0f)) {
        return;
    }
    /* the view's forward (the clip w's gradient) and a world direction across it */
    f[0] = m[0][3];
    f[1] = m[1][3];
    f[2] = m[2][3];
    r[0] = f[2];
    r[1] = 0.0f;
    r[2] = -f[0];
    n = r[0] * r[0] + r[2] * r[2];
    if (!(n > 0.0f)) {
        r[0] = 1.0f;
        r[2] = 0.0f;
        n = 1.0f;
    }
    n = 1.0f / __builtin_sqrtf(n);
    VCALL(cam, 0x44, void (*)(VObject *, f32 (*)[4]))(cam, m);
    sceVu0ApplyMatrix(q, m, c);
    sx = q[0] / q[3];
    sy = q[1] / q[3];
    c[0] += r[0] * n;
    c[2] += r[2] * n;
    sceVu0ApplyMatrix(q, m, c);
    scale = __builtin_sqrtf((q[0] / q[3] - sx) * (q[0] / q[3] - sx) + (q[1] / q[3] - sy) * (q[1] / q[3] - sy));
    size = AT(o, 0x8, f32);
    if (!(size <= 1.0f)) {
        size = 1.0f;
    }
    glr_marker(sx - 1792.0f, sy - 1824.0f, z, scale, 0.3f + 0.3f * func_0031C248(AT(o, 0x18, f32)),
               (s32)(u32)(64.0f * size));
}
#endif

/* ---- D_00471060 (0x840 bytes, room 0x0F): 64 drifting flecks, each a quad, in parallel arrays
 * (4 bytes apart): +0x38 x, +0x138 z, +0x238 / +0x338 their drift, +0x438.. colour bytes (r, g,
 * b; +0x4F8 alpha), +0x538 / +0x638 size, +0x738 frames to the next nudge. The area: +0x30
 * half width, +0x34 depth; placed at +0x10, turned by +0x20 ---- */

#define FL(o, a, i) AT(o, (a) + (i) * 4, f32)

/* +0xC: each fleck at x -2..2 drifting out (by up to 0.05 x), forward 0.1..0.3, white, size
 * 0.4..0.8 by 0.4..0.8 */
/* 0x003069B0 */
void DriftingFlecks_Start(u8 *o) {
    VObject *rnd = gRandom;
    s32 i;

    for (i = 0; i < 64; i++) {
        f32 r = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);

        FL(o, 0x38, i) = 4.0f * (r - 0.5f);
        FL(o, 0x138, i) = 0.0f;
        r = VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        FL(o, 0x238, i) = 0x1.99999a0000000p-5f /* 0.05 */ * FL(o, 0x38, i) * r;
        r = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        FL(o, 0x338, i) = 0x1.99999a0000000p-4f /* 0.1 */ + 0x1.99999a0000000p-3f /* 0.2 */ * r;
        AT(o, 0x438 + i, u8) = 0x80;
        AT(o, 0x478 + i, u8) = 0x80;
        AT(o, 0x4B8 + i, u8) = 0x80;
        AT(o, 0x4F8 + i, u8) = 0x80;
        r = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        FL(o, 0x538, i) = 0x1.99999a0000000p-2f /* 0.4 */ + 0x1.99999a0000000p-2f /* 0.4 */ * r;
        r = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        FL(o, 0x638, i) = 0x1.99999a0000000p-2f /* 0.4 */ + 0x1.99999a0000000p-2f /* 0.4 */ * r;
        AT(o, 0x738 + i * 4, s32) = 8;
    }
}

/* +0x10: each fleck drifts (nudged now and then by up to 0.05 each way); out of the area it
 * fades 0x30 a frame. 0 once all are gone */
/* 0x003067B0 */
s32 DriftingFlecks_Update(u8 *o) {
    VObject *rnd = gRandom;
    s32 any = 0;
    s32 i;

    for (i = 0; i < 64; i++) {
        f32 x;

        if (AT(o, 0x738 + i * 4, s32) != 0) {
            AT(o, 0x738 + i * 4, s32)--;
        } else {
            if ((VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 3) == 0) {
                AT(o, 0x738 + i * 4, s32) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) + 4) & 0xF;
            }
            FL(o, 0x238, i) = FL(o, 0x238, i) + 0x1.99999a0000000p-4f /* 0.1 */ * (VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd) - 0.5f);
            FL(o, 0x338, i) = FL(o, 0x338, i) + 0x1.99999a0000000p-4f /* 0.1 */ * (VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd) - 0.5f);
        }
        FL(o, 0x38, i) = FL(o, 0x38, i) + FL(o, 0x238, i);
        FL(o, 0x138, i) = FL(o, 0x138, i) + FL(o, 0x338, i);
        x = FL(o, 0x38, i);
        if (x < AT(o, 0x30, f32) && !(x <= -AT(o, 0x30, f32)) && FL(o, 0x138, i) < AT(o, 0x34, f32)
            && !(FL(o, 0x138, i) < 0.0f)) {
            any = 1;
        } else if (AT(o, 0x4F8 + i, u8) != 0) {
            AT(o, 0x4F8 + i, u8) -= 0x30;
            if (AT(o, 0x4F8 + i, s8) > 0) {
                any = 1;
            } else {
                AT(o, 0x4F8 + i, u8) = 0;
            }
        }
    }
    return any;
}

/* +0x14: each fleck a unit quad scaled by its size, turned along its drift, at its place in
 * the area, drawn by a quad drawer (D_0046FC30) */
/* 0x003063A0 */
void DriftingFlecks_Draw(u8 *o) {
    s32 i;

    for (i = 0; i < 64; i++) {
        f32 m[4][4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));
        f32 t[4] __attribute__((aligned(16)));
        f32 c[4][4] __attribute__((aligned(16)));
        struct {
            s32 col[4];
            f32 uv[8];
        } rec __attribute__((aligned(16)));
        u8 q[0x40] __attribute__((aligned(16)));

        sceVu0UnitMatrix(m);
        m[0][0] = FL(o, 0x538, i);
        m[2][2] = FL(o, 0x638, i);
        v[0] = FL(o, 0x238, i);
        v[1] = 0.0f;
        v[2] = FL(o, 0x338, i);
        v[3] = 0.0f;
        sceVu0Normalize(v, v);
        if (v[2] != 0.0f) {
            if (!(v[2] <= 0.0f)) {
                sceVu0RotMatrixY(m, m, func_0031BDB0(v[0] / v[2]) - 0x1.921fb6p+0f /* pi / 2 */);
            } else {
                sceVu0RotMatrixY(m, m, 0x1.921fb6p+1f /* pi */ + (func_0031BDB0(v[0] / v[2]) - 0x1.921fb6p+0f));
            }
        }
        t[1] = 0.0f;
        t[0] = FL(o, 0x38, i);
        t[3] = 1.0f;
        t[2] = FL(o, 0x138, i);
        sceVu0TransMatrix(m, m, t);
        sceVu0RotMatrix(m, m, (f32 *)(o + 0x20));
        sceVu0TransMatrix(m, m, (f32 *)(o + 0x10));
        rec.col[0] = AT(o, 0x438 + i, u8);
        rec.col[1] = AT(o, 0x478 + i, u8);
        rec.col[2] = AT(o, 0x4B8 + i, u8);
        rec.col[3] = AT(o, 0x4F8 + i, u8);
        c[0][0] = 1.0f;
        c[0][2] = -1.0f;
        rec.uv[0] = 0.0f;
        rec.uv[1] = 0.0f;
        rec.uv[2] = 0.0f;
        rec.uv[3] = 1.0f;
        rec.uv[4] = 1.0f;
        rec.uv[5] = 1.0f;
        rec.uv[6] = 0.0f;
        rec.uv[7] = 0.0f;
        c[0][3] = 1.0f;
        c[0][1] = 0.0f;
        sceVu0ApplyMatrix(c[0], m, c[0]);
        c[1][1] = 0.0f;
        c[1][0] = 1.0f;
        c[1][2] = 1.0f;
        c[1][3] = 1.0f;
        sceVu0ApplyMatrix(c[1], m, c[1]);
        c[2][1] = 0.0f;
        c[2][0] = -1.0f;
        c[2][2] = -1.0f;
        c[2][3] = 1.0f;
        sceVu0ApplyMatrix(c[2], m, c[2]);
        c[3][1] = 0.0f;
        c[3][0] = -1.0f;
        c[3][2] = 1.0f;
        c[3][3] = 1.0f;
        sceVu0ApplyMatrix(c[3], m, c[3]);
        AT(q, 0x18, s32) = 0;
        AT(q, 0x4, s32) = -1;
        AT(q, 0x8, s32) = -1;
        AT(q, 0xC, s32) = -1;
        AT(q, 0x0, void **) = D_0046FC30;
        AT(q, 0x10, void *) = &rec;
        AT(q, 0x14, void *) = c;
        AT(q, 0x20, s32) = 0x19;
        AT(q, 0x1C, s32) = 0;
        AT(q, 0x26, u16) = 0x80;
        AT(q, 0x24, u16) = 1;
        AT(q, 0x2E, u16) = 0x200;
        AT(q, 0x30, u16) = 0x100;
        AT(q, 0x33, u8) = 1;
        AT(q, 0x35, u8) = 0x10;
        AT(q, 0x28, u16) = 0;
        AT(q, 0x2A, u16) = 0x40;
        AT(q, 0x2C, u16) = 0x40;
        AT(q, 0x32, u8) = 2;
        AT(q, 0x36, u8) = 2;
        AT(q, 0x34, u8) = 0;
        func_002E56C0(q);
        AT(q, 0x0, void **) = D_00469D00;
    }
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern void *D_0047A390[];
extern void *D_0047A6D0[];
extern void *D_0047A710[];
extern void *D_0047A730[];
extern void *D_00479F30[];
extern void *D_00479F50[];
extern void *D_00479F70[];
extern void *D_00478B50[];
extern void *D_00479A80[];

/* (as RisingSmoke_dtor)  +0x8 destructor (the quad drawer's inlined) */
/* 0x00376730 */
u8 *Room49Effect_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0047A390;
    AT(o, 0x1810, void **) = D_0046FC30;
    AT(o, 0x1810, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* mote i (re)started: rising 0.1 .. 0.3 a frame, grey, alpha 0x40 .. 0x7F, at the start (its
 * depth moving on with the frames), sized 3/4 of its rise, a random turn */
void func_003767C0(u8 *o, s32 i) {
    static const union { u32 u; f32 f; } kTenth = {0x3DCCCCCD}, kFifth = {0x3E4CCCCD}, kTwoFifths = {0x3ECCCCCD},
        kTwoPi = {0x40C90FDB}, kThreeQuarters = {0x3F400000};
    VObject *rnd;
    u8 *r;

    if (AT(o, 0x1A58, u8) == 1) {
        return;
    }
    rnd = gRandom;
    AT(o, 0x1848 + i * 4, f32) = 0.0f + kTenth.f + kFifth.f * shaft_rnd(rnd);
    AT(o, 0x1948 + i * 4, f32) = kTwoPi.f * (shaft_rnd(rnd) - 0.5f);
    r = HAZE2_REC(o, AT(o, 0x1A54, s32), i);
    AT(r, 0x0, s32) = 0x80;
    AT(r, 0x4, s32) = 0x80;
    AT(r, 0x8, s32) = 0x80;
    AT(r, 0xC, s32) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0x3F) + 0x40;
    AT(r, 0x10, f32) = -60.0f;
    AT(r, 0x14, f32) = 0.0f;
    AT(r, 0x18, f32) = 0.0f + -60.0f + kTwoFifths.f * (f32)AT(o, 0x1A50, s32);
    AT(r, 0x1C, f32) = 1.0f;
    AT(r, 0x20, f32) = AT(r, 0x24, f32) = kThreeQuarters.f * AT(o, 0x1848 + i * 4, f32);
    AT(r, 0x28, f32) = 0x1.921fb6p+1f /* pi */ * (360.0f * (VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd) - 0.5f)) / 180.0f;
    AT(r, 0x2C, s32) = 0;
}

#ifdef HG_NATIVE
/* D_0047A390's +0x14 draw (PC; the PS2 sends GS packets): unless stopped, the motes, then the
 * haze as room 0x61's (phases +0x1A48 / +0x1A4C) over the screen at 0x60 */
/* 0x00376990 */
void Room49Effect_Draw(u8 *o) {
    if (AT(o, 0x1A58, u8) == 1) {
        return;
    }
    AT(o, 0x1820, u8 *) = HAZE2_REC(o, AT(o, 0x1A54, s32), 0);
    func_002E56C0(o + 0x1810);
    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
    glr_haze_fix(AT(o, 0x1A48, f32), 2.0f * func_0031C248(AT(o, 0x1A4C, f32)), 0x60);
}
#endif

/* +0x10 update: the buffers swapped, the frames counted; each mote turns, wobbles 0.1 about its
 * angle and rises, fading by 0 or 1 on every other frame (to 0); one frame in 8 each one gone
 * comes back one time in 16. The haze's phases on by 3 .. 5 and 1 .. 3 degrees */
/* 0x00377590 */
s32 Room49Effect_Update(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k2Pi = {0x40C90FDB}, kTenth = {0x3DCCCCCD};
    VObject *rnd;
    f32 t;
    s32 i, k;

    if (AT(o, 0x1A58, u8) == 1) {
        return 0;
    }
    rnd = gRandom;
    AT(o, 0x1A54, s32) ^= 1;
    AT(o, 0x1A50, s32)++;
    for (i = 0; i < 64; i++) {
        s32 cur = AT(o, 0x1A54, s32);
        u32 *dst = (u32 *)HAZE2_REC(o, cur, i);
        u32 *src = (u32 *)HAZE2_REC(o, cur ^ 1, i);
        u8 *r;
        f32 a;

        for (k = 0; k < 12; k++) {
            *dst++ = *src++;
        }
        r = HAZE2_REC(o, AT(o, 0x1A54, s32), i);
        AT(r, 0x28, f32) = AT(r, 0x28, f32) + 0.5f * (kPi.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd) / 180.0f);
        a = AT(o, 0x1948 + i * 4, f32) + kPi.f * (90.0f * shaft_rnd(rnd)) / 180.0f;
        AT(o, 0x1948 + i * 4, f32) = a;
        if (!(a <= kPi.f)) {
            AT(o, 0x1948 + i * 4, f32) = a - k2Pi.f;
        }
        AT(r, 0x10, f32) = AT(r, 0x10, f32) + kTenth.f * func_0031C248(AT(o, 0x1948 + i * 4, f32));
        AT(r, 0x14, f32) = AT(r, 0x14, f32) + AT(o, 0x1848 + i * 4, f32);
        AT(r, 0x18, f32) = AT(r, 0x18, f32) + kTenth.f * func_0031C058(AT(o, 0x1948 + i * 4, f32));
        if (AT(o, 0x1A54, s32) == 0) {
            AT(r, 0xC, s32) = AT(r, 0xC, s32) - (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 1);
            if (AT(r, 0xC, s32) < 0) {
                AT(r, 0xC, s32) = 0;
            }
        }
    }
    if ((VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 7) == 0) {
        for (i = 0; i < 64; i++) {
            if (AT(HAZE2_REC(o, AT(o, 0x1A54, s32), i), 0xC, s32) == 0 &&
                (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0xF) == 0) {
                func_003767C0(o, i);
            }
        }
    }
    rnd = gRandom;
    t = AT(o, 0x1A48, f32) + kPi.f * (3.0f + 2.0f * shaft_rnd(rnd)) / 180.0f;
    AT(o, 0x1A48, f32) = t;
    AT(o, 0x1A48, f32) = Angle_Wrap(t);
    t = AT(o, 0x1A4C, f32) + kPi.f * (1.0f + 2.0f * shaft_rnd(rnd)) / 180.0f;
    AT(o, 0x1A4C, f32) = t;
    AT(o, 0x1A4C, f32) = Angle_Wrap(t);
    return 1;
}

/* +0xC set up: the motes' drawer (layer 0x19, texture group 0x40 cell (0x1A0, 0x40) 32 x 32 of
 * 512 x 256, 5 frames), every mote started hidden, the haze at random phases */
/* 0x00377970 */
void Room49Effect_Start(u8 *o) {
    VObject *rnd;
    s32 i;

    AT(o, 0x1A58, u8) = 0;
    AT(o, 0x1A54, s32) = 0;
    AT(o, 0x1A50, s32) = 0;
    AT(o, 0x1818, s64) = -1;
    AT(o, 0x1824, s32) = 0;
    AT(o, 0x1828, s32) = 0;
    AT(o, 0x182C, s32) = 0;
    AT(o, 0x1830, s32) = 0x19;
    AT(o, 0x1834, s16) = 0x40;
    AT(o, 0x1836, s16) = 0x1A0;
    AT(o, 0x1838, s16) = 0x40;
    AT(o, 0x183A, s16) = 0x20;
    AT(o, 0x183C, s16) = 0x20;
    AT(o, 0x183E, s16) = 0x200;
    AT(o, 0x1840, s16) = 0x100;
    AT(o, 0x1842, s8) = 0x40;
    AT(o, 0x1843, s8) = 1;
    AT(o, 0x1844, s8) = 1;
    AT(o, 0x1845, s8) = 0x10;
    AT(o, 0x1846, s8) = 5;
    for (i = 0; i < 64; i++) {
        func_003767C0(o, i);
        AT(HAZE2_REC(o, AT(o, 0x1A54, s32), i), 0xC, s32) = 0;
    }
    rnd = gRandom;
    AT(o, 0x1A48, f32) = 0x1.921fb6p+1f /* pi */ * (360.0f * (shaft_rnd(rnd) - 0.5f)) / 180.0f;
    AT(o, 0x1A4C, f32) = 0x1.921fb6p+1f /* pi */ * (360.0f * (shaft_rnd(rnd) - 0.5f)) / 180.0f;
}

/* (as SinkingSprite_dtor)  +0x8 destructor (the quad drawer's inlined) */
/* 0x0037B2E0 */
u8 *DustMote_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0047A6D0;
    AT(o, 0x70, void **) = D_0046FC30;
    AT(o, 0x70, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* (as OrangeSparks_dtor)  +0x8 destructor (the quad drawer's inlined) */
/* 0x0037BE80 */
u8 *DropletFlash_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0047A710;
    AT(o, 0xC10, void **) = D_0046FC30;
    AT(o, 0xC10, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* as StrandSplash_Start */
/* 0x0037C520 */
void DropletFlash_Start(u8 *self) {
    S32(self, 0xF60) = 0;
    self[0xF64] = 0;
    S64(self, 0xC18) = -1;
    S32(self, 0xC24) = 0;
    S32(self, 0xC28) = 0;
    S32(self, 0xC2C) = 0;
    S32(self, 0xC30) = 25;
    S16(self, 0xC34) = 0x20;
    S16(self, 0xC36) = 0x6C;
    S16(self, 0xC38) = 0x4C;
    S16(self, 0xC3A) = 8;
    S16(self, 0xC3C) = 8;
    S16(self, 0xC3E) = 0x200;
    S16(self, 0xC40) = 0x100;
    self[0xC42] = 0x40;
    self[0xC43] = 1;
    self[0xC44] = 1;
    self[0xC45] = 0x10;
    self[0xC46] = 0xFF;
}

/* (as RisingMotes_dtor)  +0x8 destructor (the quad drawer's inlined) */
/* 0x0037C5A0 */
u8 *AshFlakes_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0047A730;
    AT(o, 0x3010, void **) = D_0046FC30;
    AT(o, 0x3010, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* ---- D_0047A730 (0x4478 bytes): 128 ash flakes about (-80, -20, -10), in two buffers of
 * quad records (+0x10 + 0x1800 x the current one +0x4470), turned by angles (+0x3060, 16 each)
 * at their spins (+0x3860, 12 each), moving by their velocities (+0x3E60, 12 each) and a wind
 * (+0x4460); dead ones (alpha 0) come back three a frame, spread wider as the frames count up
 * (+0x446C); the quad drawer at +0x3010 ---- */

#define ASH_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x1800) + (i))
#define ASH_BUF(e) AT(e, 0x4470, s32)

/* flake i anew in the current buffer: shown (alpha 0x80) unless `spread` is 0, its depth spread
 * by 2 x `spread` */
void func_0037C630(u8 *e, s32 i, s32 spread) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD};   /* 0.1: multiplied first, as the EE did */
    VObject *rnd = gRandom;
    QuadRec *r = ASH_REC(e, ASH_BUF(e), i);
    s32 k;

    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = spread != 0 ? 0x80 : 0;
    r->pos[0] = AT(e, 0x3050, f32) + 20.0f * (burst_rnd(rnd) - 0.5f);
    r->pos[1] = AT(e, 0x3054, f32) - 10.0f * burst_rnd(rnd);
    r->pos[2] = AT(e, 0x3058, f32) + (2.0f * (f32)spread) * (burst_rnd(rnd) - 0.5f);
    r->pos[3] = 1.0f;
    r->w = 1.0f;
    r->h = 1.0f;
    r->turn = 0.0f;
    r->frame = burst_int(rnd) & 0xF;
    AT(e, 0x3E60 + i * 12, f32) = k01.f * (burst_rnd(rnd) - 0.5f);
    AT(e, 0x3E64 + i * 12, f32) = k01.f * burst_rnd(rnd);
    AT(e, 0x3E68 + i * 12, f32) = k01.f * (burst_rnd(rnd) - 0.5f);
    for (k = 0; k < 3; k++) {
        AT(e, 0x3060 + i * 16 + k * 4, f32) = 0x1.921fb6p+1f * (180.0f * burst_rnd(rnd)) / 180.0f;
    }
    for (k = 0; k < 3; k++) {
        AT(e, 0x3860 + i * 12 + k * 4, f32) = 10.0f + 22.5f * (burst_rnd(rnd) - 0.5f);
    }
}

/* +0x14 draw: each flake a 2 x 2 quad in the xz plane turned by its angles */
/* 0x0037C9E0 */
void AshFlakes_Draw(u8 *e) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 c[4][4] __attribute__((aligned(16)));
    s32 i;

    AT(e, 0x3024, f32 *) = c[0];
    for (i = 0; i < 128; i++) {
        sceVu0UnitMatrix(m);
        sceVu0RotMatrix(m, m, (f32 *)(e + 0x3060 + i * 0x10));
        c[0][0] = 1.0f;  c[0][1] = 0.0f; c[0][2] = -1.0f; c[0][3] = 1.0f;
        sceVu0ApplyMatrix(c[0], m, c[0]);
        c[1][0] = 1.0f;  c[1][1] = 0.0f; c[1][2] = 1.0f;  c[1][3] = 1.0f;
        sceVu0ApplyMatrix(c[1], m, c[1]);
        c[2][0] = -1.0f; c[2][1] = 0.0f; c[2][2] = -1.0f; c[2][3] = 1.0f;
        sceVu0ApplyMatrix(c[2], m, c[2]);
        c[3][0] = -1.0f; c[3][1] = 0.0f; c[3][2] = 1.0f;  c[3][3] = 1.0f;
        sceVu0ApplyMatrix(c[3], m, c[3]);
        AT(e, 0x3020, QuadRec *) = ASH_REC(e, ASH_BUF(e), i);
        func_002E56C0(e + 0x3010);
    }
}

/* +0x10 update (0 once stopped, +0x4474): flip the buffers; each shown flake carried over,
 * rising a little faster at random, moving with the wind, turning, its frame counting to
 * +0x3043; then up to three dead ones back */
/* 0x0037CB90 */
s32 AshFlakes_Update(u8 *e) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kNegPi = {0xC0490FDB}, kTwoPi = {0x40C90FDB};
    VObject *rnd = gRandom;
    s32 i, k, n;

    if (AT(e, 0x4474, u8) == 1) {
        return 0;
    }
    ASH_BUF(e) ^= 1;
    for (i = 0; i < 128; i++) {
        u32 *src = (u32 *)ASH_REC(e, ASH_BUF(e) ^ 1, i);
        u32 *dst = (u32 *)ASH_REC(e, ASH_BUF(e), i);
        QuadRec *r;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = ASH_REC(e, ASH_BUF(e), i);
        if (r->rgba[3] == 0) {
            continue;
        }
        AT(e, 0x3E64 + i * 12, f32) = AT(e, 0x3E64 + i * 12, f32) + 0.01f * burst_rnd(rnd);
        r->pos[0] = r->pos[0] + (AT(e, 0x3E60 + i * 12, f32) + AT(e, 0x4460, f32));
        r->pos[1] = r->pos[1] + (AT(e, 0x3E64 + i * 12, f32) + AT(e, 0x4464, f32));
        r->pos[2] = r->pos[2] + (AT(e, 0x3E68 + i * 12, f32) + AT(e, 0x4468, f32));
        for (k = 0; k < 3; k++) {
            f32 *a = &AT(e, 0x3060 + i * 16 + k * 4, f32);

            *a = *a + kPi.f * AT(e, 0x3860 + i * 12 + k * 4, f32) / 180.0f;
            if (!(*a <= kPi.f)) {
                *a = *a - kTwoPi.f;
            } else if (*a < kNegPi.f) {
                *a = *a + kTwoPi.f;
            }
        }
        if (++r->frame >= AT(e, 0x3043, s8)) {
            r->frame = 0;
        }
    }
    AT(e, 0x446C, f32) = AT(e, 0x446C, f32) + 1.0f;
    for (i = 0, n = 3; i < 128; i++) {
        if (ASH_REC(e, ASH_BUF(e), i)->rgba[3] == 0) {
            func_0037C630(e, i, (s32)AT(e, 0x446C, f32));
            if (--n == 0) {
                break;
            }
        }
    }
    return 1;
}

/* +0xC set up: the drawer's settings (16 frames of 32 x 32, row 0xE0, palette 11), the spot,
 * the wind (0.2, 2, 0), every flake hidden */
/* 0x0037CFA0 */
void AshFlakes_Start(u8 *e) {
    s32 i;

    ASH_BUF(e) = 0;
    AT(e, 0x4474, u8) = 0;
    AT(e, 0x446C, s32) = 0;
    AT(e, 0x3018, s64) = -1;
    AT(e, 0x3028, s32) = 0;
    AT(e, 0x302C, s32) = 0;
    AT(e, 0x3030, s32) = 0x19;
    AT(e, 0x3034, s16) = 1;
    AT(e, 0x3036, s16) = 0;
    AT(e, 0x3038, s16) = 0xE0;
    AT(e, 0x303A, s16) = 0x20;
    AT(e, 0x303C, s16) = 0x20;
    AT(e, 0x303E, s16) = 0x200;
    AT(e, 0x3040, s16) = 0x100;
    AT(e, 0x3042, u8) = 2;
    AT(e, 0x3043, u8) = 0x10;
    AT(e, 0x3044, u8) = 1;
    AT(e, 0x3045, u8) = 0x10;
    AT(e, 0x3046, u8) = 0xB;
    AT(e, 0x3050, f32) = -80.0f;
    AT(e, 0x3054, f32) = -20.0f;
    AT(e, 0x3058, f32) = -10.0f;
    AT(e, 0x305C, f32) = 1.0f;
    AT(e, 0x4460, u32) = 0x3E4CCCCD;   /* 0.2 */
    AT(e, 0x4464, f32) = 2.0f;
    AT(e, 0x4468, f32) = 0.0f;
    for (i = 0; i < 128; i++) {
        func_0037C630(e, i, 0);
    }
}

/* (as OrangeSparks_dtor)  +0x8 destructor (the quad drawer's inlined) */
/* 0x00368BB0 */
u8 *SmokeTrail_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479F30;
    AT(o, 0xC10, void **) = D_0046FC30;
    AT(o, 0xC10, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* ---- D_00479F30 (0xE60 bytes): a trail of 32 smoke puffs rising from an object (+0xE50, its
 * position at +0x10; done once its +0x28 clears), in two buffers of quad records (+0x10 +
 * 0x600 x the current one +0xE58), velocities at +0xC50 (16 each), the quad drawer at +0xC10;
 * +0xE54 off: no new puffs, gone once all have faded ---- */

#define PUFF_REC(o, buf, i) ((QuadRec *)((o) + 0x10 + (buf) * 0x600) + (i))
#define PUFF_VEL(o, i) ((f32 *)((o) + 0xC50 + (i) * 0x10))

/* (re)start puff `i` at the object, 3 x 3, at a random turn and drift; `stagger`: its alpha
 * 0x20 less its index (the first round) */
void func_00368C40(u8 *o, s32 i, s32 stagger) {
    static const union { u32 u; f32 f; } k180 = {0x43340000}, kPi = {0x40490FDB};   /* multiplied first */
    QuadRec *r = PUFF_REC(o, AT(o, 0xE58, s32), i);
    VObject *rnd;
    f32 *v;

    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = 0x20;
    if (stagger) {
        r->rgba[3] -= i;
    }
    sceVu0CopyVector(r->pos, AT(o, 0xE50, f32 *) + 4);
    r->pos[1] += 2.0f;
    r->w = 3.0f;
    r->h = 3.0f;
    rnd = gRandom;
    r->turn = kPi.f * (k180.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) / 180.0f;
    r->frame = 0;
    v = PUFF_VEL(o, i);
    v[0] = -0x1.99999ap-3f + 0x1.99999ap-2f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);   /* -0.2 + 0.4 x */
    v[1] = 0x1.99999ap-3f + 0x1.99999ap-2f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
    v[2] = -0x1.99999ap-3f + 0x1.99999ap-2f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
}

/* +0x18 start: from object `src` (all 32 puffs, staggered); none: off */
/* 0x00368E10 */
void SmokeTrail_SetParams(u8 *o, u8 *src) {
    s32 i;

    if (src == NULL) {
        AT(o, 0xE54, u8) = 1;
        return;
    }
    AT(o, 0xE50, u8 *) = src;
    AT(o, 0xE54, u8) = 0;
    for (i = 0; i < 32; i++) {
        func_00368C40(o, i, 1);
    }
}

/* 0x00368E80 */
void SmokeTrail_Draw(u8 *o) {
    quad_step(o, 0xC10, 0xE58, 0x600);
}

/* +0x10 update: flip the buffers; each puff carried over, fading (a faded one restarts while
 * on), shrinking, turning a degree, drifting; 0 once off and all faded */
/* 0x00368EB0 */
s32 SmokeTrail_Update(u8 *o) {
    u8 done = 1;
    s32 i, k;

    if (!AT(o, 0xE54, u8) && AT(AT(o, 0xE50, u8 *), 0x28, u8) == 0) {
        AT(o, 0xE54, u8) = 1;
    }
    AT(o, 0xE58, s32) ^= 1;
    for (i = 0; i < 32; i++) {
        u32 *src = (u32 *)PUFF_REC(o, AT(o, 0xE58, s32) ^ 1, i);
        u32 *dst = (u32 *)PUFF_REC(o, AT(o, 0xE58, s32), i);
        QuadRec *r;
        f32 *v;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = PUFF_REC(o, AT(o, 0xE58, s32), i);
        if (r->rgba[3] != 0) {
            r->rgba[0] -= 2;
            r->rgba[1] -= 2;
            r->rgba[2] -= 2;
            r->rgba[3] -= 1;
            done = 0;
            if (!AT(o, 0xE54, u8) && r->rgba[3] == 0) {
                func_00368C40(o, i, 0);
            }
        }
        r->w -= 0x1.99999ap-5f;   /* 0.05 */
        r->turn += 0x1.1df46ap-6f;   /* a degree */
        if (!(r->turn <= 0x1.921fb6p+1f)) {
            r->turn -= 0x1.921fb6p+2f;
        }
        v = PUFF_VEL(o, i);
        r->pos[0] += v[0];
        r->pos[1] += v[1];
        r->pos[2] += v[2];
    }
    if (AT(o, 0xE54, u8) == 1 && done == 1) {
        return 0;
    }
    return 1;
}

/* +0xC set up: the drawer's settings (32 quads of a 32 x 32 cell at (384, 128), blended,
 * palette 6, layer 0x19) */
/* 0x003690F0 */
void SmokeTrail_Start(u8 *o) {
    AT(o, 0xE58, s32) = 0;
    AT(o, 0xC18, s64) = -1;
    AT(o, 0xC28, s32) = 0;
    AT(o, 0xC2C, s32) = 0;
    AT(o, 0xC30, s32) = 0x19;
    AT(o, 0xC34, s16) = 0x20;
    AT(o, 0xC36, s16) = 0x180;
    AT(o, 0xC38, s16) = 0x80;
    AT(o, 0xC3A, s16) = 0x20;
    AT(o, 0xC3C, s16) = 0x20;
    AT(o, 0xC3E, s16) = 0x200;
    AT(o, 0xC40, s16) = 0x100;
    AT(o, 0xC42, s8) = 0;
    AT(o, 0xC43, s8) = 1;
    AT(o, 0xC44, s8) = 1;
    AT(o, 0xC45, s8) = 0x10;
    AT(o, 0xC46, s8) = 6;
}

/* (as RisingSmoke_dtor)  +0x8 destructor (the quad drawer's inlined) */
/* 0x00369170 */
u8 *WispColumn_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479F50;
    AT(o, 0x1810, void **) = D_0046FC30;
    AT(o, 0x1810, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* ---- D_00479F50 (0x1C58 bytes): a column of 64 large (20 x 20) wisps rising 5 a frame from
 * -20 to 60 and starting over, in two buffers of quad records (+0x10 + 0xC00 x the current one
 * +0x1C54), velocities at +0x1850 (16 each), the quad drawer at +0x1810; +0x1C51 their alpha,
 * +0x1C50 off: the alpha runs down to 0; gone at once in a cutscene unless the stalker is
 * kind 0xC ---- */

#define WISP_REC(o, buf, i) ((QuadRec *)((o) + 0x10 + (buf) * 0xC00) + (i))
#define WISP_VEL(o, i) ((f32 *)((o) + 0x1850 + (i) * 0x10))

/* (re)start wisp `i` at the bottom, turned at random; `stagger`: 5 lower for every 4th */
void func_00369200(u8 *o, s32 i, s32 stagger) {
    static const union { u32 u; f32 f; } k180 = {0x43340000}, kPi = {0x40490FDB};   /* multiplied first */
    QuadRec *r = WISP_REC(o, AT(o, 0x1C54, s32), i);
    VObject *rnd;
    f32 *v;

    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = AT(o, 0x1C51, u8);
    r->pos[0] = 0.0f;
    r->pos[1] = -20.0f;
    r->pos[2] = 0.0f;
    if (stagger) {
        r->pos[1] = r->pos[1] - 5.0f * (f32)(i >> 2);
    }
    r->pos[3] = 1.0f;
    r->w = 20.0f;
    r->h = 20.0f;
    rnd = gRandom;
    r->turn = kPi.f * (k180.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) / 180.0f;
    r->frame = 0;
    v = WISP_VEL(o, i);
    v[0] = 0.0f;
    v[1] = 5.0f;
    v[2] = 0.0f;
}

/* +0x18 start (alpha 0x10, all 64 staggered); none: off */
/* 0x00369330 */
void WispColumn_SetParams(u8 *o, void *on) {
    s32 i;

    if (on == NULL) {
        AT(o, 0x1C50, u8) = 1;
        return;
    }
    for (i = 0; i < 64; i++) {
        func_00369200(o, i, 1);
    }
    AT(o, 0x1C51, u8) = 0x10;
    AT(o, 0x1C50, u8) = 0;
}

/* +0x14 draw (while the alpha is up) */
/* 0x003693A0 */
void WispColumn_Draw(u8 *o) {
    if (AT(o, 0x1C51, u8)) {
        AT(o, 0x1820, u8 *) = o + AT(o, 0x1C54, s32) * 0xC00 + 0x10;
        func_002E56C0(o + 0x1810);
    }
}

/* +0x10 update: flip the buffers; each wisp carried over at the current alpha, turning 10
 * degrees, rising, restarted past 60; 0 once faded out */
/* 0x003693F0 */
s32 WispColumn_Update(u8 *o) {
    s32 i, k;

    if (AT(gCharPursuer, 0x153C, u8) != 0xC && VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector)) {
        AT(o, 0x1C50, u8) = 1;
        AT(o, 0x1C51, u8) = 0;
        return 0;
    }
    if (AT(o, 0x1C50, u8) == 1) {
        AT(o, 0x1C51, u8)--;
        if (AT(o, 0x1C51, u8) == 0) {
            return 0;
        }
    }
    AT(o, 0x1C54, s32) ^= 1;
    for (i = 0; i < 64; i++) {
        u32 *src = (u32 *)WISP_REC(o, AT(o, 0x1C54, s32) ^ 1, i);
        u32 *dst = (u32 *)WISP_REC(o, AT(o, 0x1C54, s32), i);
        QuadRec *r;
        f32 *v;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = WISP_REC(o, AT(o, 0x1C54, s32), i);
        r->rgba[3] = AT(o, 0x1C51, u8);
        r->turn += 0x1.657186p-3f;   /* 10 degrees */
        if (!(r->turn <= 0x1.921fb6p+1f)) {
            r->turn -= 0x1.921fb6p+2f;
        }
        v = WISP_VEL(o, i);
        r->pos[0] += v[0];
        r->pos[1] += v[1];
        r->pos[2] += v[2];
        if (!(r->pos[1] < 60.0f)) {
            func_00369200(o, i, 0);
        }
    }
    return 1;
}

/* +0xC set up: the drawer's settings (64 quads of a 32 x 32 cell at (384, 128), blended 0x80,
 * palette 6, layer 0x19) */
/* 0x00369610 */
void WispColumn_Start(u8 *o) {
    AT(o, 0x1C54, s32) = 0;
    AT(o, 0x1818, s64) = -1;
    AT(o, 0x1828, s32) = 0;
    AT(o, 0x182C, s32) = 0;
    AT(o, 0x1830, s32) = 0x19;
    AT(o, 0x1834, s16) = 0x40;
    AT(o, 0x1836, s16) = 0x180;
    AT(o, 0x1838, s16) = 0x80;
    AT(o, 0x183A, s16) = 0x20;
    AT(o, 0x183C, s16) = 0x20;
    AT(o, 0x183E, s16) = 0x200;
    AT(o, 0x1840, s16) = 0x100;
    AT(o, 0x1842, s8) = -0x80;
    AT(o, 0x1843, s8) = 1;
    AT(o, 0x1844, s8) = 1;
    AT(o, 0x1845, s8) = 0x10;
    AT(o, 0x1846, s8) = 6;
}

/* (as RisingSmoke_dtor)  +0x8 destructor (the quad drawer's inlined) */
/* 0x00369690 */
u8 *SparkSpray_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479F70;
    AT(o, 0x1810, void **) = D_0046FC30;
    AT(o, 0x1810, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* ---- D_00479F70 (0x20E0 bytes): 64 sparks sprayed from one side (+0x20D4: 0 the left at
 * x -75, else the right at x 75; y 14) across and down, in two buffers of quad records (+0x10
 * + 0xC00 x the current one +0x20D8), velocities at +0x1850 and their pulls at +0x1C50 (16
 * each), start delays at +0x2050 (s16), the quad drawer at +0x1810; +0x20D0 off: no new
 * sparks, gone once all have faded ---- */

#define SPRAY_REC(o, buf, i) ((QuadRec *)((o) + 0x10 + (buf) * 0xC00) + (i))
#define SPRAY_VEL(o, i) ((f32 *)((o) + 0x1850 + (i) * 0x10))
#define SPRAY_PULL(o, i) ((f32 *)((o) + 0x1C50 + (i) * 0x10))
#define SPRAY_DELAY(o, i) AT(o, 0x2050 + (i) * 2, s16)

/* (re)start spark `i` on side `side`: up to 8 off the nozzle at a random angle about x, 5 x 5;
 * `delayed`: held back `i` + 1 frames, hidden */
void func_00369720(u8 *o, s32 i, s32 side, s32 delayed) {
    static const union { u32 u; f32 f; } k180 = {0x43340000}, kPi = {0x40490FDB};   /* multiplied first */
    f32 m[4][4] __attribute__((aligned(16)));
    f32 off[4] __attribute__((aligned(16)));
    VObject *rnd = gRandom;
    QuadRec *r = SPRAY_REC(o, AT(o, 0x20D8, s32), i);
    f32 *v, *a, ang;

    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = 0x40;
    off[0] = 0.0f;
    off[1] = 2.0f + 6.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
    off[2] = 0.0f;
    off[3] = 0.0f;
    ang = -0x1.921fb6p+1f + 2.0f * (kPi.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd));
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixX(m, m, ang);
    sceVu0ApplyMatrix(off, m, off);
    v = SPRAY_VEL(o, i);
    a = SPRAY_PULL(o, i);
    if (side == 0) {
        r->pos[0] = -75.0f;
        r->pos[1] = 14.0f;
        r->pos[2] = 0.0f;
        v[0] = 4.0f + VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        v[1] = -0.5f;
        v[2] = -0x1.99999ap-4f + 0x1.99999ap-3f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        a[0] = -0x1.333334p-3f;   /* -0.15 */
        a[1] = 0x1.99999ap-4f;    /* 0.1 */
        a[2] = 0.0f;
    } else {
        r->pos[0] = 75.0f;
        r->pos[1] = 14.0f;
        r->pos[2] = 0.0f;
        v[0] = -4.0f - VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        v[1] = -0.5f;
        v[2] = -0x1.99999ap-4f + 0x1.99999ap-3f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        a[0] = 0x1.333334p-3f;
        a[1] = 0x1.99999ap-4f;
        a[2] = 0.0f;
    }
    sceVu0AddVector(r->pos, r->pos, off);
    r->pos[3] = 1.0f;
    r->w = 5.0f;
    r->h = 5.0f;
    r->turn = kPi.f * (k180.f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) / 180.0f;
    r->frame = 0;
    if (delayed) {
        SPRAY_DELAY(o, i) = i + 1;
        r->rgba[3] = 0;
    } else {
        SPRAY_DELAY(o, i) = 0;
    }
}

/* +0x18 start: arg { side } (all 64, delayed in turn); none: off */
/* 0x00369A60 */
void SparkSpray_SetParams(u8 *o, u8 *arg) {
    s32 side, i;

    if (arg == NULL) {
        AT(o, 0x20D0, u8) = 1;
        return;
    }
    side = arg[0];
    AT(o, 0x20D4, s32) = side;
    for (i = 0; i < 64; i++) {
        func_00369720(o, i, side, 1);
    }
    AT(o, 0x20D0, u8) = 0;
}

/* 0x00369AE0 */
void SparkSpray_Draw(u8 *o) {
    quad_step(o, 0x1810, 0x20D8, 0xC00);
}

/* +0x10 update (gone at once in a cutscene): flip the buffers; each spark carried over, a
 * held one counting down to its start, a shown one fading (restarted when out, while on),
 * growing by 0.5, turning 10 degrees, pulled and moved; 0 once off and all faded */
/* 0x00369B10 */
s32 SparkSpray_Update(u8 *o) {
    u8 done = 1;
    s32 i, k;

    if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector)) {
        AT(o, 0x20D0, u8) = 1;
        return 0;
    }
    AT(o, 0x20D8, s32) ^= 1;
    for (i = 0; i < 64; i++) {
        u32 *src = (u32 *)SPRAY_REC(o, AT(o, 0x20D8, s32) ^ 1, i);
        u32 *dst = (u32 *)SPRAY_REC(o, AT(o, 0x20D8, s32), i);
        QuadRec *r;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = SPRAY_REC(o, AT(o, 0x20D8, s32), i);
        if (SPRAY_DELAY(o, i) != 0) {
            SPRAY_DELAY(o, i)--;
            if (SPRAY_DELAY(o, i) == 0) {
                func_00369720(o, i, AT(o, 0x20D4, s32), 0);
            }
        } else if (r->rgba[3] != 0) {
            r->rgba[3]--;
            done = 0;
            if (!AT(o, 0x20D0, u8) && r->rgba[3] == 0) {
                func_00369720(o, i, AT(o, 0x20D4, s32), 0);
            }
        }
        r->w += 0.5f;
        r->h += 0.5f;
        r->turn += 0x1.657186p-3f;   /* 10 degrees */
        if (!(r->turn <= 0x1.921fb6p+1f)) {
            r->turn -= 0x1.921fb6p+2f;
        }
        sceVu0AddVector(SPRAY_VEL(o, i), SPRAY_VEL(o, i), SPRAY_PULL(o, i));
        sceVu0AddVector(r->pos, r->pos, SPRAY_VEL(o, i));
    }
    if (AT(o, 0x20D0, u8) == 1 && done == 1) {
        return 0;
    }
    return 1;
}

/* +0xC set up: the drawer's settings (64 quads of a 32 x 32 cell at (0, 64), blended 0x40,
 * the first palette, layer 0x19) */
/* 0x00369D80 */
void SparkSpray_Start(u8 *o) {
    AT(o, 0x20D8, s32) = 0;
    AT(o, 0x1818, s64) = -1;
    AT(o, 0x1828, s32) = 0;
    AT(o, 0x182C, s32) = 0;
    AT(o, 0x1830, s32) = 0x19;
    AT(o, 0x1834, s16) = 0x40;
    AT(o, 0x1836, s16) = 0;
    AT(o, 0x1838, s16) = 0x40;
    AT(o, 0x183A, s16) = 0x20;
    AT(o, 0x183C, s16) = 0x20;
    AT(o, 0x183E, s16) = 0x200;
    AT(o, 0x1840, s16) = 0x100;
    AT(o, 0x1842, s8) = 0x40;
    AT(o, 0x1843, s8) = 1;
    AT(o, 0x1844, s8) = 1;
    AT(o, 0x1845, s8) = 0x10;
    AT(o, 0x1846, s8) = -1;
}

/* (as RisingSmoke_dtor)  +0x8 destructor (the quad drawer's inlined) */
/* 0x0034E0E0 */
u8 *Fire_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00478B50;
    AT(o, 0x1810, void **) = D_0046FC30;
    AT(o, 0x1810, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}

/* ---- D_00478B50 (0x1C60 bytes; room 0x66): a fire - 64 flames (records +0x10 + 0xC00 x the
 * current one +0x1C50, velocities +0x1850, the drawer +0x1810) at one of the room's spots
 * (+0x1C58 twice its index: x / z pairs at D_00442D60 for a small fire, D_00442D80 for a big
 * one, +0x1C54); +0x1C5C put out: no new flames ---- */

extern f32 D_00442D60[], D_00442D64[];   /* the small fires' spots (x, z pairs) */
extern f32 D_00442D80[], D_00442D84[];   /* the big fires' */

#define FIRE_REC(o, buf, i) ((QuadRec *)((o) + 0x10 + (buf) * 0xC00) + (i))
#define FIRE_VEL(o, i) ((f32 *)((o) + 0x1850 + (i) * 0x10))

/* flame `i` (re)started round the spot - small: within 7.5 across, 3..10 big, rising 0.02 ..
 * 0.07; big: 12 up and within 5 across, 3..6 big, rising 0.02..0.12 - drifting out, alpha
 * 0x10 (`hidden`: 0) */
void func_0034E170(u8 *o, s32 i, s32 hidden) {
    static const union { u32 u; f32 f; } k2 = {0x40000000}, k005 = {0x3D4CCCCD};   /* multiplied first */
    QuadRec *r = FIRE_REC(o, AT(o, 0x1C50, s32), i);
    f32 *v = FIRE_VEL(o, i);
    VObject *rnd;
    f32 a, b, c;
    s32 spot;

    if (AT(o, 0x1C54, s32) == 0) {
        r->rgba[0] = 0x46;
        r->rgba[1] = 0x2B;
        r->rgba[2] = 0x1E;
        r->rgba[3] = hidden ? 0 : 0x10;
        rnd = gRandom;
        a = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f;
        b = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f;
        c = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f;
        spot = AT(o, 0x1C58, s32);
        r->pos[0] = D_00442D60[spot] + 15.0f * a;
        r->pos[1] = k2.f * b;
        r->pos[2] = D_00442D64[AT(o, 0x1C58, s32)] + 15.0f * c;
        r->pos[3] = 1.0f;
        r->w = 3.0f + 7.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        r->h = r->w;
        r->turn = 0.0f;
        r->frame = 0;
        v[0] = k005.f * a;
        v[1] = 0x1.47ae14p-6f + 0x1.99999ap-5f * (0.5f + b);   /* 0.02 + 0.05 (..) */
        v[2] = k005.f * c;
    } else {
        r->rgba[0] = 0x46;
        r->rgba[1] = 0x30;
        r->rgba[2] = 0x1A;
        r->rgba[3] = hidden ? 0 : 0x10;
        rnd = gRandom;
        a = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f;
        b = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f;
        c = VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f;
        spot = AT(o, 0x1C58, s32);
        r->pos[0] = D_00442D80[spot] + 10.0f * a;
        r->pos[1] = 12.0f + 20.0f * b;
        r->pos[2] = D_00442D84[AT(o, 0x1C58, s32)] + 10.0f * c;
        r->pos[3] = 1.0f;
        r->w = 3.0f + 3.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        r->h = r->w;
        r->turn = 0.0f;
        r->frame = 0;
        v[0] = k005.f * a;
        v[1] = 0x1.47ae14p-6f + 0x1.99999ap-4f * (0.5f + b);   /* 0.02 + 0.1 (..) */
        v[2] = k005.f * c;
    }
}

/* +0x18 start: arg { the spot (negative: put out), big }: every flame started, those past 16
 * (small) or 32 (big) hidden */
/* 0x0034E500 */
void Fire_SetParams(u8 *o, s32 *arg) {
    s32 i;

    if (arg == NULL) {
        return;
    }
    if (arg[0] < 0) {
        AT(o, 0x1C5C, u8) = 1;
        return;
    }
    AT(o, 0x1C58, s32) = arg[0] * 2;
    AT(o, 0x1C54, s32) = arg[1];
    for (i = 0; i < 64; i++) {
        if (AT(o, 0x1C54, s32) == 0) {
            func_0034E170(o, i, i >= 16);
        } else {
            func_0034E170(o, i, i >= 32);
        }
    }
}

/* +0x10 update: flip the buffers; each flame showing carried over, growing 0.04, turning half
 * a degree, rising, fading in by up to 7 (to 0x60 small / 0x40 big, then marked) and out by up
 * to 7 (big: 3); unless put out, one hidden flame a frame restarted. 0 once none shows */
/* 0x0034E620 */
s32 Fire_Update(u8 *o) {
    VObject *rnd = gRandom;
    u8 done = 1;
    s32 i, k;

    AT(o, 0x1C50, s32) ^= 1;
    for (i = 0; i < 64; i++) {
        QuadRec *r;
        f32 *v = FIRE_VEL(o, i);

        for (k = 0; k < 12; k++) {
            ((u32 *)FIRE_REC(o, AT(o, 0x1C50, s32), i))[k] = ((u32 *)FIRE_REC(o, AT(o, 0x1C50, s32) ^ 1, i))[k];
        }
        r = FIRE_REC(o, AT(o, 0x1C50, s32), i);
        if (r->rgba[3] == 0) {
            continue;
        }
        done = 0;
        r->w = r->w + 0x1.47ae14p-5f;   /* 0.04 */
        r->h = r->w;
        r->turn = r->turn + 0x1.1df46ap-7f;   /* half a degree */
        if (!(r->turn <= 0x1.921fb6p+1f)) {
            r->turn = r->turn - 0x1.921fb6p+2f;
        }
        r->pos[0] = r->pos[0] + v[0];
        r->pos[1] = r->pos[1] + v[1];
        r->pos[2] = r->pos[2] + v[2];
        if (AT(o, 0x1C54, s32) == 0) {
            if (r->rgba[0] == 0x46) {
                r->rgba[3] = r->rgba[3] + (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 7);
                if (r->rgba[3] >= 0x61) {
                    r->rgba[3] = 0x60;
                    r->rgba[0]++;
                }
            } else {
                r->rgba[3] = r->rgba[3] - (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 7);
                if (r->rgba[3] < 0) {
                    r->rgba[3] = 0;
                }
            }
        } else if (r->rgba[0] == 0x46) {
            r->rgba[3] = r->rgba[3] + (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 7);
            if (r->rgba[3] >= 0x41) {
                r->rgba[3] = 0x40;
                r->rgba[0]++;
            }
        } else {
            r->rgba[3] = r->rgba[3] - (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 3);
            if (r->rgba[3] < 0) {
                r->rgba[3] = 0;
            }
        }
    }
    if (!AT(o, 0x1C5C, u8)) {
        for (i = 0; i < 64; i++) {
            if (FIRE_REC(o, AT(o, 0x1C50, s32), i)->rgba[3] == 0) {
                func_0034E170(o, i, 0);
                done = 0;
                break;
            }
        }
    }
    return done != 1;
}

/* 0x0034E960 */
void Fire_Start(u8 *p) {
    *(s32 *)(p + 0x1C50) = 0;
    p[0x1C5C] = 0;
    *(s64 *)(p + 0x1818) = -1;
    *(s32 *)(p + 0x1824) = 0;
    *(s32 *)(p + 0x1828) = 0;
    *(s32 *)(p + 0x182C) = 0;
    *(s32 *)(p + 0x1830) = 0x19;
    *(u16 *)(p + 0x1834) = 0x40;
    *(u16 *)(p + 0x1836) = 0x20;
    *(u16 *)(p + 0x1838) = 0x40;
    *(u16 *)(p + 0x183A) = 0x20;
    *(u16 *)(p + 0x183C) = 0x20;
    *(u16 *)(p + 0x183E) = 0x200;
    *(u16 *)(p + 0x1840) = 0x100;
    p[0x1842] = 0;
    p[0x1843] = 1;
    p[0x1844] = 1;
    p[0x1845] = 0x10;
    p[0x1846] = 0xFF;
}

/* (as RisingSmoke_Draw)  +0x14 draw: the current buffer through the quad drawer */
/* 0x0034E5F0 */
void Fire_Draw(u8 *o) {
    AT(o, 0x1820, u8 *) = o + AT(o, 0x1C50, s32) * 0xC00 + 0x10;
    func_002E56C0(o + 0x1810);
}

/* (as SinkingSprite_dtor)  +0x8 destructor (the quad drawer's inlined) */
/* 0x0035FEA0 */
u8 *OneDrip_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479A80;
    AT(o, 0x70, void **) = D_0046FC30;
    AT(o, 0x70, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        EffectMgr_free(o);
    }
    return o;
}
