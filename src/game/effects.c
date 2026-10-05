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


/* ---- room effect D_0046FF20 (0x718 bytes): a burst of 16 sprites in two buffers of quad
 * records (+0x10 + 0x300 x the current one +0x710), their velocities at +0x648 (12 each),
 * drawn by the quad drawer at +0x610. Kinds (+0x714): 1 swirling motes that brighten until
 * their alpha reaches +0x70C and then fade (red one below +0x708 marks the fading), 2
 * spreading dust, 3 a rising puff (dark, or light when +0x708 is set), others drifting
 * dust ---- */

#include "effectmgr.h"

extern void *D_0046FF20[], *D_0046F580[];
extern void func_002D63B0(void *p);   /* free (the effect manager's heap) */

#define BURST_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x300) + (i))
#define BURST_VEL(e, i) ((f32 *)((e) + 0x648) + (i) * 3)

/* +0x8 destructor (the quad drawer's inlined) */
u8 *func_002E8060(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0046FF20;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
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
void func_002E80F0(u8 *e, u8 *arg) {
    VObject *rnd;
    QuadRec *r;
    f32 *v;
    s32 i;

    AT(e, 0x714, s32) = AT(arg, 0x10, s32);
    AT(e, 0x708, s32) = AT(arg, 0x14, s32);
    AT(e, 0x70C, s32) = AT(arg, 0x20, s32);
    switch (AT(e, 0x714, s32)) {
    case 1:
        rnd = D_0044E550;
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

        rnd = D_0044E550;
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
        rnd = D_0044E550;
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
        rnd = D_0044E550;
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
void func_002E8800(u8 *e) {
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
s32 func_002E8830(u8 *e) {
    VObject *rnd = D_0044E550;
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
void func_002E9040(u8 *e) {
    AT(e, 0x710, s32) = 0;
    AT(e, 0x618, s64) = -1;
    AT(e, 0x628, s32) = 0;
    AT(e, 0x62C, s32) = 0;
    AT(e, 0x630, s32) = 0x19;
    AT(e, 0x634, s16) = 0x10;
    AT(e, 0x636, s16) = (burst_int(D_0044E550) & 1) << 5;
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

/* ---- room effect D_00472F60 (room 0x60's floor effect 0x1B): a floor quad of some strength
 * (+0x14; 0 off); its draw (+0x14, func_00317D40) is a GL TODO ---- */

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
    VCALL(D_0044E4F0, 0x8C, void (*)(VObject *, s32))(D_0044E4F0, AT(e, 0x18, s32));
}

/* ---- D_004795A0 (0x10 bytes; room 0x60's event object 1): a glow whose strength (+0x8)
 * follows a level (+0x4): up to the level and back down to 30 (+0xC 0 / 1), or for level 0x80
 * slowly up to 0x38 (2); 0xFF done (its +0xC / +0x10 / +0x18 in src/leaf/b7_00351D50.c). Its
 * draw (+0x14, func_003582D0) is a GL TODO ---- */

extern void *D_004795A0[], *D_0046F580[];
extern void func_002D63B0(void *p);   /* free (the effect manager's heap) */

/* +0x8 destructor */
u8 *func_00358210(u8 *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x0, void **) = D_004795A0;
        AT(e, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(e);
        }
    }
    return e;
}



/* ---- D_0046F5A0 (0x1C60 bytes; room 0x24): rising smoke, 64 particles in two buffers of
 * sprite instances (+0x10 + 0xC00 x the current one +0x1C50: RGBA, position, size, turn,
 * frame - 0x30 each), their velocities at +0x1850 (16 each), drawn by the quad drawer at
 * +0x1810 ---- */

extern void *D_0046F5A0[];

/* +0x8 destructor (the quad drawer's inlined) */
u8 *func_002D63F0(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0046F5A0;
    AT(o, 0x1810, void **) = D_0046FC30;
    AT(o, 0x1810, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* particle i (anew: `again`, faded in a random 0..11 steps) at the source (56.5, 13 + the
 * steps, -1.4, spread 1.1), dim purple, size 0.4, drifting up 0.07..0.17 a frame */
void func_002D6480(u8 *o, s32 i, s32 again) {
    static const union { u32 u; f32 f; } k11 = {0x3F8CCCCD}, kZ = {0xBFB33333}, k04 = {0x3ECCCCCD},
        kSpread = {0x3CF5C28F}, kRise = {0x3D8F5C29}, kRiseVar = {0x3DCCCCCD};
    VObject *rnd = D_0044E550;
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
    rnd = D_0044E550;
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
void func_002D6700(u8 *o) {
    AT(o, 0x1820, u8 *) = o + AT(o, 0x1C50, s32) * 0xC00 + 0x10;
    func_002E56C0(o + 0x1810);
}

/* +0x10 update: into the other buffer, each particle grown (0.05), turned (3 degrees) and
 * moved; anew above height 25 or once faded */
s32 func_002D6730(u8 *o) {
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
void func_002D6920(u8 *o) {
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

/* the effect manager's objects' +0x18 for those that take nothing */
void func_002D63E0(void) {
}

/* ... and +0x1C: -1 */
s32 func_002D63D0(void) {
    return -1;
}

/* ---- room effect D_0046FF60: shards - 16 small random boxes thrown up from a point (+0xE10)
 * that tumble, fall and bounce on its height until they settle; kind (+0xE38) 0 / 3 a gentle
 * spray, 2 / 3 bigger, slower turning pieces thrown higher, 4 thrown down ---- */

extern void *D_0046FF60[], *D_0046F580[];
extern void func_002D63B0(void *p);

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
    /* 0xD4 */ u8 padD4[0xC];
} Shard;

_Static_assert(sizeof(Shard) == 0xE0, "Shard");

#define SHARD(e, i) ((Shard *)((u8 *)(e) + 0x10) + (i))
#define SHARD_RND() VCALL(D_0044E550, 0x18, f32 (*)(VObject *))(D_0044E550)

/* +0x8 destructor */
u8 *func_002E9B00(u8 *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x0, void **) = D_0046FF60;
        if (e != NULL) {
            AT(e, 0x0, void **) = D_0046F580;
        }
        if ((s16)flags > 0) {
            func_002D63B0(e);
        }
    }
    return e;
}

/* +0xC set up: nothing */
void func_002EB380(u8 *e) {
}

/* throw shard `i`: a random box (each corner's coordinates 0..1 with its signs), size, turn,
 * position about the origin, velocity and spin by the kind */
void func_002E9B60(u8 *e, s32 i) {
    static const s8 sx[8] = {1, -1, 1, -1, 1, -1, 1, -1};
    static const s8 sy[8] = {1, 1, -1, -1, 1, 1, -1, -1};
    static const s8 sz[8] = {1, 1, 1, 1, -1, -1, -1, -1};
    VObject *rnd = D_0044E550;
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
        rnd = D_0044E550;
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
    rnd = D_0044E550;
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
void func_002EA5B0(u8 *e, u8 *arg) {
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
s32 func_002EB1C0(u8 *e) {
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

#ifndef HG_NATIVE
extern void sceVu0FTOI4Vector(s32 *out, const f32 *in);
#else
extern void glr_strip(const f32 *mvp, s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, const void *tex,
                      u64 tex0, u32 prim);
#endif
extern VObject *D_0044E9A0;   /* the VRAM manager */
extern VObject *D_0044E4E8;   /* the texture cache */

/* +0x14 draw: the shards as textured boxes (texture +0xE34, its cell +0xE20 / +0xE24, size
 * +0xE28 / +0xE2C, colour +0xE30); a shard with a corner off screen is skipped */
void func_002EA660(u8 *e) {
    VObject *tc = D_0044E4E8, *r, *cam;
    u8 *tex;
    u32 slot;
    u64 tex0;
    f32 screen[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    s32 i, k;

    slot = VCALL(tc, 0x8, u32 (*)(VObject *, s32, s32))(tc, AT(e, 0xE34, s32), 0);
    if (slot == (u32)-1) {
        return;
    }
    tex = VCALL(tc, 0xC, u8 *(*)(VObject *, s32, s32))(tc, AT(e, 0xE34, s32), 0);
    if (slot & 0x80000000) {
        slot &= 0x7FFFFFFF;
        if (!(VCALL(D_0044E4F0, 0x44, u32 (*)(VObject *, u32, u8 *, s32))(D_0044E4F0, slot, tex, 1) & 0xFF)) {
            return;
        }
    }
    r = D_0044E4F0;
#ifndef HG_NATIVE
    {
        u64 *p = VCALL(r, 0x10, u64 *(*)(VObject *, s32, s32))(r, 5, 1);

        if (p == NULL) {
            return;
        }
        p[0] = 0x10000004;
        ((u32 *)p)[2] = 0;
        ((u32 *)p)[3] = 0x50000004;
        p[2] = 0x8003 | (u64)0x10000000 << 32;
        p[3] = 0xE;
        p[4] = VCALL(D_0044E9A0, 0x28, u64 (*)(VObject *, u32, u32, u32, u32, u32))(D_0044E9A0, slot, tex[0],
                                                                                  AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
        p[5] = 6;          /* TEX0_1 */
        p[6] = 0x14;       /* PRIM: textured triangle strip */
        p[7] = 0;
        ((u32 *)p)[16] = AT(e, 0xE30, u32);
        ((f32 *)p)[17] = 1.0f;
        p[9] = 1;          /* RGBAQ */
        tex0 = 0;
    }
#else
    tex0 = VCALL(D_0044E9A0, 0x28, u64 (*)(VObject *, u32, u32, u32, u32, u32))(D_0044E9A0, slot, tex[0],
                                                                              AT(tex, 4, u16), AT(tex, 6, u16), tex[1]);
#endif
    cam = D_0044E4B8;
    VCALL(cam, 0x44, void (*)(VObject *, f32 (*)[4]))(cam, screen);
    VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    for (i = 0; i < 16; i++) {
        Shard *s = SHARD(e, i);
        f32 m[4][4] __attribute__((aligned(16)));
        f32 world[8][4] __attribute__((aligned(16)));
        s32 off = 0;

        if (s->alive == 0) {
            continue;
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
#ifndef HG_NATIVE
        {
            s32 xyz[8][4] __attribute__((aligned(16)));
            u64 *p;
            s32 f;

            for (k = 0; k < 8; k++) {
                f32 v[4] __attribute__((aligned(16)));
                f32 q;

                sceVu0ApplyMatrix(v, screen, world[k]);
                q = 1.0f / v[3];
                v[0] *= q;
                v[1] *= q;
                v[3] = q;
                v[2] *= q;
                sceVu0FTOI4Vector(xyz[k], v);
                xyz[k][2] /= 16;
            }
            p = VCALL(r, 0x10, u64 *(*)(VObject *, s32, s32))(r, 0x32, 1);
            if (p == NULL) {
                return;
            }
            p[0] = 0x10000031;
            ((u32 *)p)[2] = 0;
            ((u32 *)p)[3] = 0x50000031;
            p[2] = 0x8030 | (u64)0x10000000 << 32;   /* 48 registers: ST, XYZ per vertex */
            p[3] = 0xE;
            p += 4;
            for (f = 0; f < 6; f++) {
                for (k = 0; k < 4; k++) {
                    s32 *v = xyz[kShardFace[f][k]];

                    ((f32 *)p)[0] = k & 1 ? AT(e, 0xE20, f32) + AT(e, 0xE28, f32) : AT(e, 0xE20, f32);
                    ((f32 *)p)[1] = k & 2 ? AT(e, 0xE24, f32) + AT(e, 0xE2C, f32) : AT(e, 0xE24, f32);
                    p[1] = 2;   /* ST */
                    p[2] = (s64)v[0] | (s64)v[1] << 16 | (s64)v[2] << 32;
                    p[3] = k < 2 ? 0xD : 5;   /* XYZ3 for the first two, then XYZ2 (draws) */
                    p += 4;
                }
            }
        }
#else
        {
            u8 rgba[4][4];
            s32 f;

            for (k = 0; k < 4; k++) {
                AT(rgba[k], 0, u32) = AT(e, 0xE30, u32);
            }
            for (f = 0; f < 6; f++) {
                f32 xyzw[4][4] __attribute__((aligned(16)));
                f32 st[4][2];

                for (k = 0; k < 4; k++) {
                    sceVu0CopyVector(xyzw[k], world[kShardFace[f][k]]);
                    AT(&xyzw[k][3], 0, u32) = 0;
                    st[k][0] = k & 1 ? AT(e, 0xE20, f32) + AT(e, 0xE28, f32) : AT(e, 0xE20, f32);
                    st[k][1] = k & 2 ? AT(e, 0xE24, f32) + AT(e, 0xE2C, f32) : AT(e, 0xE24, f32);
                }
                glr_strip(&clip[0][0], 4, &xyzw[0][0], &st[0][0], &rgba[0][0], tex, tex0, 0x10);
            }
        }
#endif
    }
    (void)tex0;
}


/* ---- D_00470A50 (0x3858 bytes): 128 motes rising from around (0, 70, 35), in two buffers of
 * quad records (+0x10 + 0x1800 x the current one +0x3850), their velocities at +0x3050 (16
 * each), drawn by the quad drawer at +0x3010; a mote is renewed past height 130 or, counted on
 * the even frames, when it has faded out ---- */

extern void *D_00470A50[];

#define MOTE_REC(e, buf, i) ((QuadRec *)((e) + 0x10 + (buf) * 0x1800) + (i))
#define MOTE_VEL(e, i) ((f32 *)((e) + 0x3050 + (i) * 0x10))

/* +0x8 destructor (the quad drawer's inlined) */
u8 *func_002F9210(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00470A50;
    AT(o, 0x3010, void **) = D_0046FC30;
    AT(o, 0x3010, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* mote i anew: at the bottom (again), or at first somewhere up the column, bigger and fainter
 * the higher */
void func_002F92A0(u8 *e, s32 i, s32 again) {
    VObject *rnd = D_0044E550;
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
    rnd = D_0044E550;
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
void func_002F9520(u8 *e) {
    AT(e, 0x3020, QuadRec *) = MOTE_REC(e, AT(e, 0x3850, s32), 0);
    func_002E56C0(e + 0x3010);
}

/* +0x10 update: flip the buffers, each mote carried over, growing, turning (3 degrees) and
 * moving */
s32 func_002F9550(u8 *e) {
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
void func_002F9750(u8 *e) {
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
u8 *func_002F9810(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00470A70;
    AT(o, 0xC10, void **) = D_0046FC30;
    AT(o, 0xC10, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* spark i anew: at the bottom (again), or at first somewhere up, fainter and slower the
 * higher */
void func_002F98A0(u8 *e, s32 i, s32 again) {
    VObject *rnd = D_0044E550;
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
void func_002F9B40(u8 *e, s32 *arg) {
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
void func_002F9D80(u8 *e) {
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
s32 func_002F9DB0(u8 *e) {
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
void func_002F9FF0(u8 *e) {
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


/* ---- D_00470E00 (0xC0 bytes): one sprite (+0x10 + 0x30 x the current one +0xB8), slowly
 * growing and sinking, with its velocity at +0xA8 and the quad drawer at +0x70; each update
 * (only every other call, +0xBC) fades it a little ---- */

extern void *D_00470E00[];

#define ONE_REC(e, buf) ((QuadRec *)((e) + 0x10) + (buf))

/* +0x8 destructor (the quad drawer's inlined) */
u8 *func_002FCDC0(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00470E00;
    AT(o, 0x70, void **) = D_0046FC30;
    AT(o, 0x70, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* +0x18 start: arg { RGBA (4 x s32), position, velocity }, size 0.2 .. 0.4 */
void func_002FCE50(u8 *e, u8 *arg) {
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
    s = 0x1.99999a0000000p-3f /* 0.2 */ + 0x1.99999a0000000p-3f /* 0.2 */ * burst_rnd(D_0044E550);
    r->w = s;
    r->h = s;
    r->turn = 0.0f;
    r->frame = 0;
    AT(e, 0xA8, f32) = AT(arg, 0x1C, f32);
    AT(e, 0xAC, f32) = AT(arg, 0x20, f32);
    AT(e, 0xB0, f32) = AT(arg, 0x24, f32);
}

/* +0x14 draw the current buffer */
void func_002FCF40(u8 *e) {
    AT(e, 0x80, QuadRec *) = ONE_REC(e, AT(e, 0xB8, s32));
    func_002E56C0(e + 0x70);
}

/* +0x10 update: every other call 0 (gone); else carried over and, while visible, faded by
 * 0..3, falling faster (to about -0.05 a frame), moved and grown */
s32 func_002FCF70(u8 *e) {
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
        rnd = D_0044E550;
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
void func_002FD150(u8 *e) {
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
