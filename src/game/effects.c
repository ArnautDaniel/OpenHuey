/* The room's effects (made by SceneGame +0xF6CD30 from PAC section 13; 0xA0-byte objects):
 *   D_0046D750 a screen tint, D_0046D7B0 a screen blend, D_0046EB40 the fog (also sets the
 *   camera's depth range), D_0046EC60 a depth range.
 * Vtable: +0xC reset, +0x18 set from the room data (unaligned little-endian words). */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern VObject *D_0044E4B8;   /* the camera */

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
    cam = D_0044E4B8;
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


extern VObject *D_0044FE10;    /* +0x80: the character in slot i (0xFF none) */
extern void *gCharacters[6];
extern void func_001267F0(void *c, s32 light);
extern s32 func_00126800(void *c);

/* +0x10 each frame: the characters this light is on (+0x20 per slot) get its light group
 * (slot << 16 | 0xB); the others it had go back to the default (0xA) */
void func_002BB280(u8 *o) {
    s32 i;

    for (i = 0; i < 6; i++) {
        u32 k = D_0044FE10 != NULL ? (u8)VCALL(D_0044FE10, 0x80, s32 (*)(VObject *, s32))(D_0044FE10, i & 0xFF) : (u8)i;
        s32 group = (i << 16) | 0xB;

        if (k == 0xFF || gCharacters[k] == NULL) {
            continue;
        }
        if (AT(o, 0x20 + i * 4, s32) != 0) {
            func_001267F0(gCharacters[k], group);
        } else if (func_00126800(gCharacters[k]) == group) {
            func_001267F0(gCharacters[k], 0xA);
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


extern VObject *D_0044E4F0;   /* the renderer */

/* a fog drawer: colours, range, queued with the renderer in `layer` */
void func_002BC000(void *drawer, u32 c0, u32 c1, s32 layer, f32 a, f32 b) {
    u8 *d = drawer;

    AT(d, 0x8, u32) = c0;
    AT(d, 0xC, u32) = c1;
    AT(d, 0x10, f32) = a;
    AT(d, 0x14, f32) = b;
    VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, d, layer, 0);
}


extern void *D_0046D7A0[];
extern void func_0026B180(void *drawer, u32 rgba, s32 layer, s32 add);

/* +0x14 draw (a screen tint): when its colour (+0x10) has alpha, a temporary drawer paints it
 * in layer 0x28 (added for modes 1 and 4, +0x14) */
void func_0026B2C0(u8 *o) {
    u8 drawer[0x20] __attribute__((aligned(16)));

    AT(drawer, 0x0, void **) = D_0046D7A0;
    AT(drawer, 0x4, s32) = -1;
    if (AT(o, 0x10, u32) & 0xFF000000) {
        func_0026B180(drawer, AT(o, 0x10, u32), 0x28, AT(o, 0x14, s32) == 1 || AT(o, 0x14, s32) == 4);
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



/* set a screen-colour drawer's colour, layer and argument and hand it to the renderer (+0xC,
 * priority 0x20) */
void func_00269940(void *d, u32 rgba, s32 which, s32 arg) {
    AT(d, 0x8, u32) = rgba;
    AT(d, 0xC, s32) = which;
    AT(d, 0x10, s32) = arg;
    VCALL(D_0044E4F0, 0xC, void (*)(VObject *, void *, s32, s32))(D_0044E4F0, d, 0x20, 0);
}


/* hand a drawer to the renderer (+0xC) in its layer (+0x20); one flagged +0x32 bit 7 then
 * also has the renderer run +0x58 */
void func_002E56C0(u8 *d) {
    VObject *r = D_0044E4F0;

    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, d, AT(d, 0x20, s32), 0);
    if (AT(d, 0x32, u8) & 0x80) {
        VCALL(r, 0x58, void (*)(VObject *))(r);
    }
}


#ifdef HG_NATIVE
extern void glr_strip(const f32 *mvp, s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, const void *tex,
                      u64 tex0, u32 prim);
extern VObject *D_0044E4E8;   /* the texture cache */
extern VObject *D_0044E4B8;   /* the camera */

#define GLR_PRIM_ADD 0x10000u
#define GLR_PRIM_NOZW 0x20000u

/* ---- PC: the quad (sprite) drawer with OpenGL - what func_002E4760 / func_002E3500 send ----
 *
 * The drawer: +0x10 the instances (0x30 each: RGBA as 4 x s32 (0x80 = 1.0), position, size x /
 * y, turn about the view axis, frame), +0x14 its own corners (flag 2), +0x18 / +0x1C the
 * corners' offset, +0x24 the instance count, the texture's frame cells +0x26 / +0x28 first
 * cell, +0x2A / +0x2C cell size, +0x2E / +0x30 texture size, +0x33 frames, flags +0x32 (1 stay
 * upright, 2 own corners, 4 turned a quarter about y, 0x40 additive, 0x80 also a glow pass -
 * func_002E3500, not done), texture id / group +0x34 / +0x35, palette +0x36 (-1: the first;
 * passed to the renderer as TEX0's CSA).
 * Billboards face the camera: x from its up x direction, y the direction x that. */
static s32 gl_sprites(u8 *d) {
    VObject *cam = D_0044E4B8;
    const void *tex = VCALL(D_0044E4E8, 0xC, void *(*)(VObject *, s32, s32))(D_0044E4E8, AT(d, 0x34, s8),
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
                  0x10 | 0x40 | GLR_PRIM_NOZW | (flags & 0x40 ? GLR_PRIM_ADD : 0));
    }
    return 1;
}

s32 func_002E4760(u8 *d) {
    return gl_sprites(d);
}

s32 func_002E3500(u8 *d) {
    return gl_sprites(d);
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
extern VObject *D_0044E4C0;     /* the room effects */
extern void *func_00266C40(void *fx, s32 k);       /* effect slot k */
extern s32 func_00266C70(u8 *fx, s32 n, void *arg);
extern void func_002670F0(void *fx, s32 k);        /* remove effect k */
extern void *func_002672F0(u32 size, void *place); /* placement new */

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
        fx = (u8 *)D_0044E4C0;
        if (AT(fx, 0x14A8, void *) != NULL) {
            VCALL(fx + 0x1400, 0x14, void (*)(void *, void *))(fx + 0x1400, AT(fx, 0x14A8, void *));
            AT(fx, 0x14A8, void *) = NULL;
        }
        mem = VCALL(fx + 0x1400, 0x10, void *(*)(void *, s32))(fx + 0x1400, 0xA0);
        if (mem != NULL) {
            e = func_002672F0(0xA0, mem);
            if (e != NULL) {
                AT(e, 0x0, void **) = D_0046EC60;
            }
            AT(fx, 0x14A8, u8 *) = e;
            VCALL(AT(fx, 0x14A8, u8 *), 0xC, void (*)(u8 *))(AT(fx, 0x14A8, u8 *));
        }
        func_00266C70(fx, 0x1C, o + 0x154);
        return;
    }
    fx = (u8 *)D_0044E4C0;
    e = func_00266C40(fx, 0x1C);
    if (e != NULL) {
        AT(o, 0x164, u8) = 1;
        AT(o, 0x154, f32) = AT(e, 0x50, f32);
        AT(o, 0x158, f32) = AT(e, 0x54, f32);
        AT(o, 0x15C, f32) = AT(e, 0x58, f32);
        AT(o, 0x160, f32) = AT(e, 0x5C, f32);
        func_002670F0(fx, 0x1C);
    }
}


#include "progress.h"

extern void func_002EF480(u8 *fade, f32 level);   /* the screen fade's level, 0..1 */

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
        func_002EF480((u8 *)gProgress + 0x7B8, 0.5f - k0005.f * (a - 16.0f));
        break;
    case 1: case 3: case 5:
        a = 128.0f - AT(f, 0x18, f32) * (f32)AT(f, 0x20, u8);
        if (a <= 0.0f) {
            a = 0.0f;
        }
        func_002EF480((u8 *)gProgress + 0x7B8, 1.0f - k0005.f * (a - 16.0f));
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


/* ---- room effect D_0046FF00 (event command 0x7F): a flickering animated sprite ----
 * +0x10 its quad record { RGBA (4 x s32), position (+0x20), size +0x30 / +0x34, rotation +0x38,
 * frame +0x3C }, +0x40 the quad drawer's settings (texture, layer +0x58, frame strip +0x5C..,
 * frame count +0x6B), +0x70 frame timer, +0x74 rest before the next run, +0x78 slow (long
 * rests) */

extern void *D_0046FF00[], *D_0046D730[], *D_0046FC30[];
extern VObject *D_0044E550;   /* random numbers: +0x10 an integer, +0x18 0..1 */
extern void func_002672E0(void *p);   /* delete (effects' heap) */

/* +0x8 destructor */
u8 *func_002E7BB0(u8 *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x0, void **) = D_0046FF00;
        AT(e, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            func_002672E0(e);
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
    AT(e, 0x74, s32) = sprite_rest(e, D_0044E550);
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
    rnd = D_0044E550;
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
    AT(e, 0x3C, s32) = VCALL(D_0044E550, 0x10, s32 (*)(VObject *))(D_0044E550) & 0xF;
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
            func_002672E0(e);
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
        mgr = D_0044E578;
        slot = Effect_New(mgr, 0x220, spark_init);
        func_002D6090(mgr, slot, &p);
        slot = Effect_New(mgr, 0x220, spark_init);
        func_002D6090(mgr, slot, &p);
        if (AT(e, 0x74, s32) != 2) {
            return;
        }
        slot = Effect_New(mgr, 0x220, spark_init);
        func_002D6090(mgr, slot, &p);
        slot = Effect_New(mgr, 0x220, spark_init);
        func_002D6090(mgr, slot, &p);
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
        AT(e, 0x3C, s32) = VCALL(D_0044E550, 0x10, s32 (*)(VObject *))(D_0044E550) & 0xF;
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
        mgr = D_0044E578;
        slot = Effect_New(mgr, 0x220, spark_init);
        p.pos[0] = AT(e, 0x20, f32);
        p.pos[1] = 2.0f + AT(e, 0x24, f32);
        p.pos[2] = AT(e, 0x28, f32);
        p.pos[3] = 1.0f;
        p.flags = 0x8000;
        func_002D6090(mgr, slot, &p);
        return;
    }
    AT(e, 0x30, f32) = 0.25f;
    AT(e, 0x34, f32) = 1.0f;
    AT(e, 0x38, f32) = 0.0f;
    AT(e, 0x3C, s32) = VCALL(D_0044E550, 0x10, s32 (*)(VObject *))(D_0044E550) & 0xF;
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
    VObject *cam = D_0044E4B8;

    if (from <= VCALL(cam, 0xCC, f32 (*)(VObject *))(cam) && !(to < VCALL(cam, 0xD0, f32 (*)(VObject *))(cam))) {
        return;
    }
    AT(d, 0x8, f32) = a;
    AT(d, 0xC, f32) = from;
    AT(d, 0x10, f32) = to;
    AT(d, 0x14, f32) = b;
    VCALL(D_0044E4F0, 0xC, void (*)(VObject *, u8 *, s32, s32))(D_0044E4F0, d, 0x21, 0);
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
