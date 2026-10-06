/* Mesh drawing (the room's parts, vtable D_0046C770; placed objects' models) with OpenGL. */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern VObject *gRenderer;   /* the renderer */
extern VObject *gCamera;   /* the camera */

#ifdef HG_NATIVE
/* the PC renderer (native/platform/glr.h): batches are drawn with OpenGL instead of the VU1 */
extern void glr_strip(const f32 *mvp, s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, const void *tex,
                      u64 tex0, u32 prim);
extern VObject *gTexCache;   /* the texture cache */
static f32 sGlMvp[4][4];   /* the current batch's local-to-clip matrix */
static const void *sGlTex; /* its texture's .TEX entry (NULL: untextured) */
static u64 sGlTex0;
static u32 sGlPrim;
static u32 sGlKindPrim;    /* the kind-4 part's blending (func_0025C8C0): GLR_PRIM_ADD | NOZW */
#define GLR_PRIM_ADD 0x10000u
#define GLR_PRIM_NOZW 0x20000u
#endif

void func_0025C8C0(u8 *o);                 /* a kind-4 part's batch */
extern void func_0025DB10(u8 *o, s32 which);
extern u64 func_002B71D0(s32 tex);         /* TEX0 of a texture */
void func_0025D970(u8 *o);
extern void func_0025D560(u8 *o);
s32 *func_0025DD80(u8 *o, s32 *batch);   /* the lit layout's batch writer: the next batch */
extern s32 *func_0025E100(u8 *o, s32 *batch);


#ifdef HG_NATIVE
/* vtable +0xC of a mesh (the room's parts, +0x8; part kind +0x18), drawn with OpenGL. Per batch
 * (until -1): its texture (+0x80, TEX0 looked up only when it changed), colour bits
 * (+0x84..+0x8A) and local matrix (stored transposed at +0x90); the camera's clip matrix times
 * it and the batch's state (texture, PRIM bits) go to the vertex writer (by +0x4: func_0025E100,
 * or the lit layout func_0025DD80). Kind 4 parts blend as their flip book says
 * (func_0025C8C0); kind 5 parts draw without depth writes, each batch normal or additive by
 * its header's bit 8. 0 without a mesh. */
s32 func_0025E2B0(u8 *o) {
    VObject *cam;
    s32 *mesh;
    f32 b[4][4] __attribute__((aligned(16)));

    AT(o, 0x4, s32) = AT(o, 0x68, s32);
    AT(o, 0xD4, s32) = 0;
    func_0025DB10(o, 0);
    func_0025DB10(o, 1);
    mesh = AT(o, 0x8, s32 *);
    if (mesh == NULL) {
        return 0;
    }
    if (mesh[0] != -1) {
        cam = gCamera;
        do {
            s32 newTex = 0;
            u32 w, kind5 = 0;
            s32 i;

            AT(o, 0x7C, s32) = mesh[0];
            AT(o, 0x80, s32) = mesh[1];
            w = mesh[2];
            AT(o, 0x84, u8) = w;
            AT(o, 0x85, u8) = (s32)(w & 0xFF00) >> 8;
            AT(o, 0x86, u16) = 1 << (AT(o, 0x85, u8) & 0xF);
            AT(o, 0x88, u16) = 1 << ((AT(o, 0x85, u8) & 0xF0) >> 4);
            AT(o, 0x8A, u8) = (w & 0xFF000000) >> 24;
            if (AT(o, 0x18, s32) == 5) {
                kind5 = GLR_PRIM_NOZW | ((mesh[3] >> 8) & 1 ? GLR_PRIM_ADD : 0);
            }
            mesh += 4;
            if (AT(o, 0x80, s32) != -1) {
                newTex = 1;
                if (AT(o, 0x80, s32) != AT(o, 0x64, s32)) {
                    AT(o, 0x10, u64) = func_002B71D0(AT(o, 0x80, s32));
                }
            }
            for (i = 0; i < 4; i++) {
                AT(o, 0x90 + i * 4, f32) = AT(mesh, 0x0, f32);
                AT(o, 0xA0 + i * 4, f32) = AT(mesh, 0x4, f32);
                AT(o, 0xB0 + i * 4, f32) = AT(mesh, 0x8, f32);
                AT(o, 0xC0 + i * 4, f32) = AT(mesh, 0xC, f32);
                mesh += 4;
            }
            if (AT(o, 0x60, u8) != 0) {
                func_0025D970(o);
            } else {
                AT(o, 0xD0, u8) = 0xFF;
            }
            if (AT(o, 0x85, u8) != 0) {
                func_0025D560(o);
            }
            if (AT(o, 0x18, s32) == 4) {
                func_0025C8C0(o);
            }
            VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, b);
            sceVu0MulMatrix(b, b, (f32 (*)[4])(o + 0x90));
            for (i = 0; i < 4; i++) {
                sGlMvp[i][0] = b[i][0];
                sGlMvp[i][1] = b[i][1];
                sGlMvp[i][2] = b[i][2];
                sGlMvp[i][3] = b[i][3];
            }
            sGlTex0 = newTex ? AT(o, 0x10, u64) : 0;
            sGlTex = newTex ? VCALL(gTexCache, 0xC, void *(*)(VObject *, s32, s32))(gTexCache, AT(o, 0x80, s32), 0)
                            : NULL;
            sGlPrim = (newTex << 4) | 0xC | AT(o, 0x84, u8) << 6 | (AT(o, 0x18, s32) == 4 ? sGlKindPrim : kind5);
            if (AT(o, 0x4, s32) == 1) {
                mesh = func_0025DD80(o, mesh);
            } else if (AT(o, 0x4, s32) == 0) {
                mesh = func_0025E100(o, mesh);
            }
        } while (mesh[0] != -1);
    }
    return 1;
}
#endif

extern void func_0025C6F0(f32 *q, f32 *axis, f32 angle);   /* rotation about an axis */
extern void func_0025C770(f32 *q, f32 (*m)[4]);            /* its matrix */

/* the view's side edge `which` (0 left, 1 right): the camera's direction (+0xA0) turned about
 * its up axis (+0xA4) by fov / 1.3, at the look-at distance from the eye -> +0x20 + 16 * which;
 * then that point swung back about y -> +0x40 + 16 * which (the edge planes for culling) */
void func_0025DB10(u8 *o, s32 which) {
    static const union { u32 u; f32 f; } k13 = {0x3FA66666};
    VObject *cam = gCamera;
    f32 eye[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    f32 up[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 r[4][4] __attribute__((aligned(16)));
    f32 t[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 e[4] __attribute__((aligned(16)));
    f32 len, ang;

    q[3] = q[2] = q[1] = q[0] = 0.0f;
    VCALL(cam, 0x24, void (*)(VObject *, f32 *))(cam, eye);
    VCALL(cam, 0x2C, void (*)(VObject *, f32 *))(cam, at);
    VCALL(cam, 0xA0, void (*)(VObject *, f32 *))(cam, dir);
    VCALL(cam, 0xA4, void (*)(VObject *, f32 *))(cam, up);
    sceVu0SubVector(t, at, eye);
    len = __builtin_sqrtf(sceVu0InnerProduct(t, t));
    ang = VCALL(cam, 0x64, f32 (*)(VObject *))(cam) / k13.f;
    func_0025C6F0(q, up, which == 0 ? ang : -ang);
    m[3][3] = 1.0f;
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;   /* unset in the original (garbage times w); no NaN / infinity on PC */
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    func_0025C770(q, m);
    sceVu0ApplyMatrix(t, m, dir);
    sceVu0ScaleVector(t, t, len);
    sceVu0AddVector(p, eye, t);
    AT(o, 0x20 + (which & 0xFF) * 16, f32) = p[0];
    AT(o, 0x24 + (which & 0xFF) * 16, f32) = p[1];
    AT(o, 0x28 + (which & 0xFF) * 16, f32) = p[2];
    AT(o, 0x2C + (which & 0xFF) * 16, f32) = p[3];
    sceVu0UnitMatrix(r);
    sceVu0SubVector(t, eye, p);
    sceVu0InnerProduct(t, t);
    sceVu0RotMatrixY(r, r, which == 0 ? -ang : ang);
    sceVu0ApplyMatrix(t, r, t);
    sceVu0AddVector(e, p, t);
    AT(o, 0x40 + (which & 0xFF) * 16, f32) = e[0];
    AT(o, 0x44 + (which & 0xFF) * 16, f32) = e[1];
    AT(o, 0x48 + (which & 0xFF) * 16, f32) = e[2];
    AT(o, 0x4C + (which & 0xFF) * 16, f32) = e[3];
}

s32 func_0025CB50(u8 *o, const f32 *st);   /* a kind-4 batch's animated texture coordinates */
s32 func_002B7500(u8 *batch);   /* send a batch's vertices to VU1 */

/* mode 0 batch writer: the batch's vertex count (+0x7C; count & 3 gives the padding) locates
 * its sections - texture coordinates (s, t), colours (RGBA bytes), positions (x, y, z, w) -
 * and it is sent unless hidden (+0xD0 not 0xFF, or its group +0x8A is off in the mask +0x6C;
 * kind 4 parts take their texture coordinates from func_0025CB50). Returns the next batch. */
s32 *func_0025E100(u8 *o, s32 *batch) {
    struct {
        s32 n;
        s32 *batch;
        f32 *xyz;   /* x, y, z, w per vertex */
        void *st;   /* texture coordinates (s, t floats) */
        u8 *rgba;   /* colours (0x80 = 1.0) */
    } a;
    s32 n = AT(o, 0x7C, s32);
    u32 fmt = n & 3;
    u8 *rgba = (u8 *)batch + n * 8;
    u8 *xyz;
    s32 ok = 1;
    s32 show;

    if (fmt == 3 || fmt == 1) {
        rgba += 8;
    }
    xyz = rgba + n * 4;
    switch (fmt) {
    case 3:
        xyz += 4;
        break;
    case 2:
        xyz += 8;
        break;
    case 1:
        xyz += 0xC;
        break;
    }
    a.n = 0;
    a.xyz = NULL;
    a.st = NULL;
    a.rgba = NULL;
    a.rgba = rgba;
    a.n = AT(o, 0x7C, s32);
    a.xyz = (f32 *)xyz;
    a.batch = batch;
    if (AT(o, 0x18, s32) == 4) {
        ok = func_0025CB50(o, (const f32 *)batch);
        a.st = o + 0x240 + (AT(o, 0xD4, s32) - 1) * 32;
    } else {
        a.st = batch;
    }
    if (AT(o, 0xD0, u8) == 0xFF) {
        u8 g = AT(o, 0x8A, u8);

        show = g == 0 || (AT(o, 0x6C + (g >> 5) * 4, u32) & (1u << (g & 0x1F)));
        if (show && ok == 1) {
#ifdef HG_NATIVE
            glr_strip(&sGlMvp[0][0], a.n, a.xyz, (f32 *)a.st, a.rgba, sGlTex, sGlTex0, sGlPrim);
#else
            func_002B7500((u8 *)&a);
#endif
        }
    }
    AT(o, 0x64, s32) = AT(o, 0x80, s32);
    return (s32 *)(xyz + AT(o, 0x7C, s32) * 16);
}

#ifdef HG_NATIVE
#include <stdlib.h>

extern void glr_todo(const char *what);

/* mode 1 (the lit layout) batch writer, with OpenGL: the vertex count (+0x7C; count & 3 the
 * padding) locates its sections - texture coordinates (s, t, q floats), colours (two words a
 * vertex, not decoded yet: drawn grey), positions (x, y, z, w with the GS flags) - drawn unless
 * hidden as in mode 0. No surveyed room uses it. Returns the next batch. */
s32 *func_0025DD80(u8 *o, s32 *batch) {
    static f32 *st;
    static u8 *rgba;
    static s32 cap;
    s32 n = AT(o, 0x7C, s32), i;
    u8 *stq = (u8 *)batch;
    u8 *col = stq + n * 12 + (n & 3) * 4;
    u8 *xyz = col + n * 8 + ((n & 3) == 1 || (n & 3) == 3 ? 8 : 0);

    if (n <= 0) {
        AT(o, 0x64, s32) = AT(o, 0x80, s32);
        return batch;
    }
    glr_todo("room mesh: the lit layout's colours (mode 1)");
    if (AT(o, 0xD0, u8) == 0xFF) {
        u8 g = AT(o, 0x8A, u8);

        if ((g == 0 || (AT(o, 0x6C + (g >> 5) * 4, u32) & (1u << (g & 0x1F)))) && n > 0) {
            if (n > cap) {
                cap = n;
                st = realloc(st, cap * 8);
                rgba = realloc(rgba, cap * 4);
            }
            for (i = 0; i < n; i++) {
                st[i * 2] = AT(stq, i * 12, f32);
                st[i * 2 + 1] = AT(stq, i * 12 + 4, f32);
                AT(rgba, i * 4, u32) = 0x80808080;
            }
            glr_strip(&sGlMvp[0][0], n, (const f32 *)xyz, st, rgba, sGlTex, sGlTex0, sGlPrim);
        }
    }
    AT(o, 0x64, s32) = AT(o, 0x80, s32);
    return (s32 *)(xyz + n * 16);
}
#endif

/* a quadword copy done inline (lq / sq), not through libvu0 */
static inline void vec_set(f32 *d, const f32 *s) {
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

/* face batch matrix `m` (+0x90, rows x / y / z / translation) towards the eye: z towards it
 * (upright: in the floor plane, y stays up; else y from the camera's up) */
static void mesh_face_eye(u8 *o, VObject *cam, s32 upright) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 eye[4] __attribute__((aligned(16)));
    f32 t[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 z[4] __attribute__((aligned(16)));
    f32 x[4] __attribute__((aligned(16)));
    f32 y[4] __attribute__((aligned(16)));

    sceVu0UnitMatrix(m);
    VCALL(cam, 0x24, void (*)(VObject *, f32 *))(cam, eye);
    if (!upright) {
        VCALL(cam, 0xA4, void (*)(VObject *, f32 *))(cam, m[1]);
    }
    t[0] = AT(o, 0xC0, f32);
    t[1] = AT(o, 0xC4, f32);
    t[2] = AT(o, 0xC8, f32);
    t[3] = AT(o, 0xCC, f32);
    sceVu0SubVector(v, eye, t);
    if (upright) {
        v[1] = 0.0f;
    }
    sceVu0Normalize(v, v);
    vec_set(z, v);
    sceVu0CopyVector(m[2], z);
    if (upright) {
        sceVu0OuterProduct(v, m[1], z);
    } else {
        sceVu0OuterProduct(v, z, m[1]);
    }
    sceVu0Normalize(v, v);
    vec_set(x, v);
    sceVu0CopyVector(m[0], x);
    sceVu0OuterProduct(v, z, x);
    sceVu0Normalize(v, v);
    vec_set(y, v);
    sceVu0CopyVector(m[1], y);
    sceVu0CopyVector(m[3], (f32 *)(o + 0xC0));
    sceVu0CopyMatrix((f32 (*)[4])(o + 0x90), m);
}

/* the side of line a -> b that point p (x, z) is on (twice the signed area) */
static inline f32 side_of(f32 ax, f32 az, f32 bx, f32 bz, f32 px, f32 pz) {
    return ax * (bz - pz) + px * (az - bz) + bx * (pz - az);
}

/* a batch out of view: its place (+0xC0 / +0xC8) against the view's edges (func_0025DB10: the
 * side points +0x20 / +0x30, swung back +0x40 / +0x50) seen from the eye - outside both side
 * edges, or outside all three of the edges behind; +0xD0 0xFF hides it (else 0) */
void func_0025D970(u8 *o) {
    f32 eye[4] __attribute__((aligned(16)));
    f32 px = AT(o, 0xC0, f32), pz = AT(o, 0xC8, f32);
    f32 ax = AT(o, 0x20, f32), az = AT(o, 0x28, f32), bx = AT(o, 0x30, f32), bz = AT(o, 0x38, f32);
    f32 cx = AT(o, 0x40, f32), cz = AT(o, 0x48, f32), dx = AT(o, 0x50, f32), dz = AT(o, 0x58, f32);
    u8 sides = 0, back = 0;

    VCALL(gCamera, 0x24, void (*)(VObject *, f32 *))(gCamera, eye);
    if (!(side_of(eye[0], eye[2], ax, az, px, pz) <= 0.0f)) {
        sides++;
    }
    if (side_of(eye[0], eye[2], bx, bz, px, pz) < 0.0f) {
        sides++;
    }
    if (!(side_of(cx, cz, ax, az, px, pz) <= 0.0f)) {
        back++;
    }
    if (side_of(dx, dz, bx, bz, px, pz) < 0.0f) {
        back++;
    }
    if (side_of(cx, cz, dx, dz, px, pz) < 0.0f) {
        back++;
    }
    AT(o, 0xD0, u8) = 0;
    if (back == 3) {
        AT(o, 0xD0, u8) = 0xFF;
    }
    if (sides == 2) {
        AT(o, 0xD0, u8) = 0xFF;
    }
}

/* a batch's view-dependent placement: parallax (+0x86 = 2 / 4 / 8 / 16: moved sideways by
 * 0.2 / 0.25 / 0.33 / 0.5 of the camera's offset +0x20) and billboards (+0x88 = 2 upright, 4
 * facing the eye) */
void func_0025D560(u8 *o) {
    static const union { u32 u; f32 f; } k02 = {0x3E4CCCCD}, k033 = {0x3EA8F5C3};
    VObject *cam = gCamera;
    f32 ofs[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    f32 up[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 k;

    VCALL(cam, 0x20, void (*)(VObject *, f32 *))(cam, ofs);
    VCALL(cam, 0xA0, void (*)(VObject *, f32 *))(cam, dir);
    VCALL(cam, 0xA4, void (*)(VObject *, f32 *))(cam, up);
    sceVu0OuterProduct(side, dir, up);
    sceVu0Normalize(side, side);
    switch (AT(o, 0x86, u16)) {
    case 2:
        k = k02.f;
        break;
    case 4:
        k = 0.25f;
        break;
    case 8:
        k = k033.f;
        break;
    case 16:
        k = 0.5f;
        break;
    default:
        k = 0.0f;
        break;
    }
    if (k != 0.0f) {
        AT(o, 0xC0, f32) = AT(o, 0xC0, f32) + (k * ofs[0]) * side[0];
        AT(o, 0xC8, f32) = AT(o, 0xC8, f32) + (k * ofs[2]) * side[2];
    }
    if (AT(o, 0x88, u16) == 2) {
        mesh_face_eye(o, cam, 1);
    } else if (AT(o, 0x88, u16) == 4) {
        mesh_face_eye(o, gCamera, 0);
    }
}


extern f32 func_002E2D00(f32 angle);   /* wrapped into -pi..pi */


extern void func_00267D60(u8 *o);
extern void func_00267AB0(u8 *o);
extern void func_002677C0(u8 *o);
extern void func_00267560(u8 *o);

#ifdef HG_NATIVE
#include <stdlib.h>

extern void glr_todo(const char *what);

/* ---- PC: placed objects' models with OpenGL (what func_00267560 .. func_00267D60 send) ----
 *
 * The model (+0x30) holds one part: +0x40 vertex count, +0x44 texture id (-1 none), streams at
 * the offsets +0x4C (texture coordinates), +0x50 (normals / colours), +0x54 (positions), +0x58
 * (strip flags) from the model, +0x60 the positions' start. The object's flags +0x40 bit 0
 * blend it. Its matrix: scaled, turned by +0x20 / +0x24 / +0x28 (X, Y, Z), at +0x10. */

/* the object's local-to-clip matrix (the camera's clip matrix times its placement) */
static void obj_mvp(u8 *o, f32 scale, f32 (*mvp)[4]) {
    f32 world[4][4] __attribute__((aligned(16)));

    sceVu0UnitMatrix(world);
    world[0][0] = world[1][1] = world[2][2] = scale;
    sceVu0RotMatrixX(world, world, AT(o, 0x20, f32));
    sceVu0RotMatrixY(world, world, AT(o, 0x24, f32));
    sceVu0RotMatrixZ(world, world, AT(o, 0x28, f32));
    sceVu0TransMatrix(world, world, (f32 *)(o + 0x10));
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, mvp);
    sceVu0MulMatrix(mvp, mvp, world);
}

static void obj_strip(u8 *o, f32 (*mvp)[4], s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba) {
    u8 *model = AT(o, 0x30, u8 *);
    s32 tex = AT(model, 0x44, s32);

    glr_strip(&mvp[0][0], n, xyzw, st, rgba,
              tex == -1 ? NULL : VCALL(gTexCache, 0xC, void *(*)(VObject *, s32, s32))(gTexCache, tex, 0),
              tex == -1 ? 0 : func_002B71D0(tex), 0xC | (tex != -1 ? 0x10 : 0) | (AT(o, 0x40, u8) & 1 ? 0x40 : 0));
}

/* a rigid part (func_00267D60): compressed streams, model scale 32 - s16 position deltas from
 * the s32 start, / 4096; u16 texture coordinates / 32768; u8 strip flags (1: no triangle).
 * The original lights it on VU1 (ambient 128); drawn unlit at 1.0 for now. */
static void gl_rigid(u8 *o) {
    static f32 *xyzw, *st;
    static u8 *rgba;
    static s32 cap;
    f32 mvp[4][4] __attribute__((aligned(16)));
    u8 *model = AT(o, 0x30, u8 *);
    s32 n = AT(model, 0x40, s32), i;
    const s32 *start = (const s32 *)(model + 0x60);
    const s16 *d = (const s16 *)(model + AT(model, 0x54, s32));
    const u16 *uv = (const u16 *)(model + AT(model, 0x4C, s32));
    const u8 *strip = model + AT(model, 0x58, s32);
    s32 x = start[0], y = start[1], z = start[2];

    if (n <= 0) {
        return;
    }
    if (n > cap) {
        cap = n;
        xyzw = realloc(xyzw, cap * 16);
        st = realloc(st, cap * 8);
        rgba = realloc(rgba, cap * 4);
    }
    for (i = 0; i < n; i++) {
        x += d[i * 3];
        y += d[i * 3 + 1];
        z += d[i * 3 + 2];
        xyzw[i * 4] = x / 4096.0f;
        xyzw[i * 4 + 1] = y / 4096.0f;
        xyzw[i * 4 + 2] = z / 4096.0f;
        AT(&xyzw[i * 4 + 3], 0, u32) = strip[i] & 1 ? 0x8000 : 0;
        st[i * 2] = uv[i * 2] / 32768.0f;
        st[i * 2 + 1] = uv[i * 2 + 1] / 32768.0f;
        AT(rgba, i * 4, u32) = 0x80808080;
    }
    obj_mvp(o, 32.0f, mvp);
    obj_strip(o, mvp, n, xyzw, st, rgba);
}

/* a part in the room's batch layout (func_00267AB0): float positions with GS flags words,
 * float texture coordinates, RGBA colours; model scale 1 */
static void gl_batch(u8 *o) {
    f32 mvp[4][4] __attribute__((aligned(16)));
    u8 *model = AT(o, 0x30, u8 *);

    obj_mvp(o, 1.0f, mvp);
    obj_strip(o, mvp, AT(model, 0x40, s32), (const f32 *)(model + AT(model, 0x54, s32)),
              (const f32 *)(model + AT(model, 0x4C, s32)), model + AT(model, 0x50, s32));
}

/* a morphing part (func_00267560 / func_002677C0): two key frames, +0x34 and +0x38, blended by
 * +0x3C (frame table at the model's +0x70 per frame, 8 bytes each: +0x4 its s16 positions /
 * 4096, +0x0 its RGBA colours, or for the lit kind s16 normals / 32768), u16 texture
 * coordinates / 32768 (+0x4C), u8 strip flags (+0x50); model scale 32. The lit kind's light is
 * fixed (axis directions, colour 64, ambient 128) and so always comes out at 0x80. */
static void gl_morph(u8 *o, s32 lit) {
    static f32 *xyzw, *st;
    static u8 *rgba;
    static s32 cap;
    f32 mvp[4][4] __attribute__((aligned(16)));
    u8 *model = AT(o, 0x30, u8 *);
    s32 n = AT(model, 0x40, s32), i, c;
    f32 t = AT(o, 0x3C, f32), w = 1.0f - t;
    const s16 *pa = (const s16 *)(model + AT(model, AT(o, 0x34, s32) * 8 + 0x74, s32));
    const s16 *pb = (const s16 *)(model + AT(model, AT(o, 0x38, s32) * 8 + 0x74, s32));
    const u8 *ca = model + AT(model, AT(o, 0x34, s32) * 8 + 0x70, s32);
    const u8 *cb = model + AT(model, AT(o, 0x38, s32) * 8 + 0x70, s32);
    const u16 *uv = (const u16 *)(model + AT(model, 0x4C, s32));
    const u8 *strip = model + AT(model, 0x50, s32);

    if (n <= 0) {
        return;
    }
    if (n > cap) {
        cap = n;
        xyzw = realloc(xyzw, cap * 16);
        st = realloc(st, cap * 8);
        rgba = realloc(rgba, cap * 4);
    }
    for (i = 0; i < n; i++) {
        for (c = 0; c < 3; c++) {
            xyzw[i * 4 + c] = pa[i * 3 + c] / 4096.0f * w + pb[i * 3 + c] / 4096.0f * t;
        }
        AT(&xyzw[i * 4 + 3], 0, u32) = strip[i] & 1 ? 0x8000 : 0;
        st[i * 2] = uv[i * 2] / 32768.0f;
        st[i * 2 + 1] = uv[i * 2 + 1] / 32768.0f;
        for (c = 0; c < 4; c++) {
            rgba[i * 4 + c] = lit ? (c < 3 ? 0x80 : 0x7F) : (u8)(s32)((f32)ca[i * 4 + c] * w + (f32)cb[i * 4 + c] * t);
        }
    }
    obj_mvp(o, 32.0f, mvp);
    obj_strip(o, mvp, n, xyzw, st, rgba);
}

static void gl_placed_object(u8 *o) {
    AT(o, 0x20, f32) = func_002E2D00(AT(o, 0x20, f32));
    AT(o, 0x24, f32) = func_002E2D00(AT(o, 0x24, f32));
    AT(o, 0x28, f32) = func_002E2D00(AT(o, 0x28, f32));
    switch (AT(o, 0x8, u32) & 3) {
    case 0:
        gl_rigid(o);
        break;
    case 2:
        gl_batch(o);
        break;
    case 1:
        gl_morph(o, 1);
        break;
    default:
        gl_morph(o, 0);
        break;
    }
}
#endif

#ifdef HG_NATIVE
/* ---- PC: a positioned room mesh (doors and the like; the original func_0025F0A0 builds VU1
 * packets with func_0025EF40 / func_0025EB70 per batch) drawn with OpenGL ----
 *
 * The mesh (+0x8) is the room's batch list: per batch a header { vertex count, texture id (-1
 * none), flags (bits 0..7 the GS PRIM ABE etc., 24..31 a group shown by the mask +0x90) }, a
 * 4 x 4 matrix (columns), then the vertices in the room's mode 0 layout (func_0025E100); -1
 * ends it. The object stands at +0x70 turned by +0x80. Mode +0x68 1 (the lit layout) isn't
 * read yet. */
s32 func_0025F0A0(u8 *o) {
    VObject *cam = gCamera;
    f32 t[4][4] __attribute__((aligned(16)));
    f32 r[4][4] __attribute__((aligned(16)));
    f32 w[4][4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 mvp[4][4] __attribute__((aligned(16)));
    s32 *mesh = AT(o, 0x8, s32 *);
    s32 i;

    AT(o, 0x4, s32) = AT(o, 0x68, s32);
    if (mesh == NULL) {
        return 0;
    }
    if (AT(o, 0x4, s32) != 0) {
        glr_todo("positioned room mesh, lit layout (func_0025EB70)");
        return 1;
    }
    sceVu0UnitMatrix(t);
    sceVu0TransMatrix(t, t, (f32 *)(o + 0x70));
    sceVu0UnitMatrix(r);
    sceVu0RotMatrix(r, r, (f32 *)(o + 0x80));
    sceVu0MulMatrix(w, t, r);
    while (mesh[0] != -1) {
        s32 n = mesh[0], tex = mesh[1];
        u32 flags = (u32)mesh[2];
        u8 g = flags >> 24;
        u8 *st, *rgba, *xyz;

        AT(o, 0xA0, s32) = n;
        AT(o, 0xA4, s32) = tex;
        AT(o, 0xA8, s32) = flags & 0xFF;
        AT(o, 0xAC, u8) = g;
        for (i = 0; i < 4; i++) {
            m[0][i] = AT(mesh, 0x10 + i * 16, f32);
            m[1][i] = AT(mesh, 0x14 + i * 16, f32);
            m[2][i] = AT(mesh, 0x18 + i * 16, f32);
            m[3][i] = AT(mesh, 0x1C + i * 16, f32);
        }
        mesh += 0x50 / 4;
        sceVu0MulMatrix(m, w, m);
        VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, mvp);
        sceVu0MulMatrix(mvp, mvp, m);
        st = (u8 *)mesh;
        rgba = st + n * 8 + ((n & 3) == 3 || (n & 3) == 1 ? 8 : 0);
        xyz = rgba + n * 4 + ((n & 3) == 3 ? 4 : (n & 3) == 2 ? 8 : (n & 3) == 1 ? 0xC : 0);
        if (g == 0 || (AT(o, 0x90 + (g >> 5) * 4, u32) & (1u << (g & 0x1F)))) {
            glr_strip(&mvp[0][0], n, (const f32 *)xyz, (const f32 *)st, rgba,
                      tex == -1 ? NULL : VCALL(gTexCache, 0xC, void *(*)(VObject *, s32, s32))(gTexCache, tex, 0),
                      tex == -1 ? 0 : func_002B71D0(tex), (tex != -1 ? 0x10 : 0) | 0xC | (flags & 0xFF) << 6);
        }
        mesh = (s32 *)(xyz + n * 16);
    }
    return 1;
}
#endif

/* a positioned room mesh's last batch is shown: its group (+0xAC) is 0 or on in the mask
 * (+0x90) */
s32 func_0025EEE0(u8 *o) {
    u8 g = AT(o, 0xAC, u8);

    return g == 0 || (AT(o, 0x90 + (g >> 5) * 4, u32) & (1u << (g & 0x1F))) != 0;
}

/* vtable +0xC of a placed object's model: its packets by its flags (+0x8 bit 0, bit 1) */
s32 func_00268090(u8 *o) {
#ifdef HG_NATIVE
    gl_placed_object(o);
    return 1;
#endif
    if (AT(o, 0x8, u32) & 1) {
        if (AT(o, 0x8, u32) & 2) {
            func_00267560(o);
        } else {
            func_002677C0(o);
        }
    } else if (AT(o, 0x8, u32) & 2) {
        func_00267AB0(o);
    } else {
        func_00267D60(o);
    }
    return 1;
}


/* a kind-4 batch's texture coordinates, a flip book: its entry (+0xD8 + 7 x +0xD4: frames,
 * frame time, type (2: stop on the last frame), frame, timer, u step, v step in 1/256) steps a
 * frame when its timer runs out (unless +0x8B holds it; the coordinates in use, +0x240 + 32 x
 * index, are last frame's +0x140 ones), each frame moving the batch's own u (st) on by the u
 * step; past 1 they wrap back by whole units (and down to 0 at least) and v moves on by that
 * many v steps. Hidden batches (group +0x8A off in the mask +0x6C) are just passed. Moves on
 * to the next batch (+0xD4); 1 */
s32 func_0025CB50(u8 *o, const f32 *st) {
    static const union { u32 u; f32 f; } k256th = {0x3B800000}, k16th = {0x3C800000};
    u8 *ent = o + 0xD8 + AT(o, 0xD4, s32) * 7;
    u8 g = AT(o, 0x8A, u8);
    f32 *uv;
    f32 m, fw;
    s32 k, whole;

    if (g != 0 && !(AT(o, 0x6C + (g >> 5) * 4, u32) & (1u << (g & 0x1F)))) {
        AT(o, 0xD4, s32) += 1;
        return 1;
    }
    if (AT(o, 0x8B, u8) == 0) {
        ent[4] -= 1;
        for (k = 0; k < 8; k++) {
            AT(o, 0x240 + AT(o, 0xD4, s32) * 32 + k * 4, f32) = AT(o, 0x140 + AT(o, 0xD4, s32) * 32 + k * 4, f32);
        }
    }
    if (ent[4] != 0) {
        AT(o, 0xD4, s32) += 1;
        return 1;
    }
    ent[3] += 1;
    if (!(ent[3] < ent[0])) {
        ent[3] = ent[2] == 2 ? ent[0] - 1 : 0;
    }
    ent[4] = ent[1];
    for (k = 0; k < 4; k++) {
        uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
        uv[k * 2] = st[k * 2] + (f32)ent[3] * ((f32)ent[5] * k256th.f);
        uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
        uv[k * 2 + 1] = st[k * 2 + 1];
    }
    uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
    if (uv[0] <= 1.0f && uv[2] <= 1.0f && uv[4] <= 1.0f && uv[6] <= 1.0f) {
        AT(o, 0xD4, s32) += 1;
        return 1;
    }
    m = uv[0];
    for (k = 1; k < 4; k++) {
        if (m < uv[k * 2]) {
            m = uv[k * 2];
        }
    }
    whole = (s32)(m - k16th.f);
    fw = (f32)whole;
    for (k = 0; k < 4; k++) {
        uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
        uv[k * 2] = uv[k * 2] - fw;
    }
    uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
    if (uv[0] < 0.0f || uv[2] < 0.0f || uv[4] < 0.0f || uv[6] < 0.0f) {
        m = uv[0];
        for (k = 1; k < 4; k++) {
            if (!(m <= uv[k * 2])) {
                m = uv[k * 2];
            }
        }
        for (k = 0; k < 4; k++) {
            uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
            uv[k * 2] = uv[k * 2] - m;
        }
    }
    for (k = 0; k < 4; k++) {
        uv = (f32 *)(o + 0x140 + AT(o, 0xD4, s32) * 32);
        uv[k * 2 + 1] = uv[k * 2 + 1] + (f32)whole * ((f32)ent[6] * k256th.f);
    }
    AT(o, 0xD4, s32) += 1;
    return 1;
}

#ifdef HG_NATIVE
/* a kind-4 room part's batch (its entry +0xD8 + 7 x +0xD4, type at +2): types 1 / 3 / 4 / 6
 * face the camera (1 and 6 staying upright) - the batch matrix (+0x90) turned to the view about
 * its own place; then its blending (for the batch's GL draw): types 4..6 additive (+0x84 on) without
 * depth writes, the others normal with depth writes */
void func_0025C8C0(u8 *o) {
    u8 *ent = o + 0xD8 + AT(o, 0xD4, s32) * 7;
    u8 type = ent[2];

    if (type == 1 || type == 3 || type == 4 || type == 6) {
        VObject *cam = gCamera;
        f32 m[4][4] __attribute__((aligned(16)));
        f32 b[4][4] __attribute__((aligned(16)));

        sceVu0CopyMatrix(m, (f32 (*)[4])(o + 0x90));
        m[3][0] = 0.0f;   /* turned about its own place */
        m[3][1] = 0.0f;
        m[3][2] = 0.0f;
        sceVu0UnitMatrix(b);
        VCALL(cam, 0xA0, void (*)(VObject *, f32 *))(cam, b[2]);
        if (ent[2] == 1 || ent[2] == 6) {
            b[2][1] = 0.0f;
            sceVu0Normalize(b[2], b[2]);
        }
        VCALL(cam, 0xA4, void (*)(VObject *, f32 *))(cam, b[1]);
        sceVu0OuterProduct(b[0], b[1], b[2]);
        sceVu0Normalize(b[0], b[0]);
        sceVu0OuterProduct(b[1], b[0], b[2]);
        sceVu0Normalize(b[1], b[1]);
        sceVu0MulMatrix(m, b, m);
        sceVu0TransMatrix((f32 (*)[4])(o + 0x90), m, (f32 *)(o + 0xC0));
    }
    if ((u8)(ent[2] - 4) < 3) {
        AT(o, 0x84, u8) = 1;
        sGlKindPrim = GLR_PRIM_ADD | GLR_PRIM_NOZW;
    } else {
        sGlKindPrim = 0;
    }
}
#endif

/* reset a room mesh drawer: no matrix row (+0x90), no mesh (+0x4), parallax off (+0x60, +0x68) */
void func_0025EEC0(u8 *o) {
    AT(o, 0x90, s32) = 0;
    AT(o, 0x94, s32) = 0;
    AT(o, 0x98, s32) = 0;
    AT(o, 0x9C, s32) = 0;
    AT(o, 0x4, s32) = 0;
    AT(o, 0x68, s32) = 0;
    AT(o, 0x60, u8) = 0;
}
