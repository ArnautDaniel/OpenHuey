/* Mesh drawing (the room's parts, vtable D_0046C770): VU1 packets for the renderer. */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern VObject *D_0044E4F0;   /* the renderer */
extern VObject *D_0044E4B8;   /* the camera */

#ifdef HG_NATIVE
/* the PC renderer (native/platform/glr.h): batches are drawn with OpenGL instead of the VU1 */
extern void glr_strip(const f32 *mvp, s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, const void *tex,
                      u64 tex0, u32 prim);
extern VObject *D_0044E4E8;   /* the texture cache */
static f32 sGlMvp[4][4];   /* the current batch's local-to-clip matrix */
static const void *sGlTex; /* its texture's .TEX entry (NULL: untextured) */
static u64 sGlTex0;
static u32 sGlPrim;
#endif

extern u32 D_0047A960[];   /* the VU1 microprogram chains, by mode (+0x4) */
extern void func_0025DB10(u8 *o, s32 which);
extern u64 func_002B71D0(s32 tex);         /* TEX0 of a texture */
extern void func_0025D970(u8 *o);
extern void func_0025D560(u8 *o);
extern void func_0025C8C0(u8 *o);
extern s32 *func_0025DD80(u8 *o, s32 *batch);   /* write a batch's vertices: the next batch */
extern s32 *func_0025E100(u8 *o, s32 *batch);

#define VIF_STCYCL_1_1 0x01000101u
#define GS_REG_PRIM 0x00
#define GS_REG_TEX0_1 0x06
#define GS_REG_ALPHA_1 0x42
#define GS_REG_ZBUF_1 0x4E

/* vtable +0xC of a mesh (the room's parts, +0x8; part kind +0x18): write its VU1 packets at
 * the renderer's cursor. Per batch (until -1): its texture (+0x80, TEX0 when it changes),
 * colour bits (+0x84..+0x8A) and local matrix (stored transposed at +0x90), then the camera's
 * two matrices times it, the microprogram start, the PRIM/TEX0 setup and the vertices (by
 * +0x4: func_0025E100 or func_0025DD80). Kinds 4/5 switch the z buffer / alpha around it.
 * 0 without a mesh. */
s32 func_0025E2B0(u8 *o) {
    VObject *r;
    VObject *cam;
    s32 *mesh;
    u64 *p;
    f32 a[4][4] __attribute__((aligned(16)));
    f32 b[4][4] __attribute__((aligned(16)));

    if (AT(o, 0x18, s32) == 5) {
        r = D_0044E4F0;
        p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 3);
        p[0] = 0x10000002;
        AT(p, 0x8, u32) = 0;
        AT(p, 0xC, u32) = 0x50000002;
        p[2] = 0x1000000000008001ull;
        p[3] = 0xE;
        p[4] = 0x1310000A0ull;   /* ZBUF_1: no z writes */
        p[5] = GS_REG_ZBUF_1;
    }
    AT(o, 0x4, s32) = AT(o, 0x68, s32);
    AT(o, 0xD4, s32) = 0;
    func_0025DB10(o, 0);
    func_0025DB10(o, 1);
    r = D_0044E4F0;
    p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 1);
    p[0] = 0x50000000 | ((u64)(D_0047A960[AT(o, 0x4, s32)] & 0x0FFFFFFF) << 32);   /* DMA call */
    AT(p, 0x8, u32) = VIF_STCYCL_1_1;
    AT(p, 0xC, u32) = 0;
    p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 3);
    p[0] = 0x10000002;
    AT(p, 0x8, u32) = 0x20000000;   /* STMASK */
    AT(p, 0xC, u32) = 0x50;
    AT(p, 0x10, u32) = 0x30000000;  /* STROW */
    AT(p, 0x14, f32) = 0.0f;
    AT(p, 0x18, f32) = 0.0f;
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x20, f32) = 0.5f;
    AT(p, 0x24, s32) = 0;
    AT(p, 0x28, s32) = 0;
    AT(p, 0x2C, s32) = 0;
    mesh = AT(o, 0x8, s32 *);
    if (mesh == NULL) {
        return 0;
    }
    if (mesh[0] != -1) {
        cam = D_0044E4B8;
        do {
            s32 newTex = 0;
            u32 w;
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
                p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 3);
                p[0] = 0x10000002;
                AT(p, 0x8, u32) = 0;
                AT(p, 0xC, u32) = 0x50000002;
                p[2] = 0x1000000000008001ull;
                p[3] = 0xE;
                p[4] = (u64)(s64)(((mesh[3] >> 8) & 1) + 1) << 2 | 0x40;
                p[5] = GS_REG_ALPHA_1;
            }
            mesh += 4;
            /* TEX0 is sent with every textured batch, looked up only when it changed */
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
            VCALL(cam, 0x44, void (*)(VObject *, f32 (*)[4]))(cam, a);
            VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, b);
            sceVu0MulMatrix(b, b, (f32 (*)[4])(o + 0x90));
            sceVu0MulMatrix(a, a, (f32 (*)[4])(o + 0x90));
#ifdef HG_NATIVE
            for (i = 0; i < 4; i++) {
                sGlMvp[i][0] = b[i][0];
                sGlMvp[i][1] = b[i][1];
                sGlMvp[i][2] = b[i][2];
                sGlMvp[i][3] = b[i][3];
            }
            sGlTex0 = newTex ? AT(o, 0x10, u64) : 0;
            sGlTex = newTex ? VCALL(D_0044E4E8, 0xC, void *(*)(VObject *, s32, s32))(D_0044E4E8, AT(o, 0x80, s32), 0)
                            : NULL;
            sGlPrim = (newTex << 4) | 0xC | AT(o, 0x84, u8) << 6;
#endif
            p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 9);
            p[0] = 0x10000008;
            AT(p, 0x8, u32) = VIF_STCYCL_1_1;
            AT(p, 0xC, u32) = 0x6C088000;   /* UNPACK V4-32, 8 qwords to VU mem 0 */
            for (i = 0; i < 4; i++) {
                AT(p, 0x10 + i * 0x10, f32) = a[i][0];
                AT(p, 0x14 + i * 0x10, f32) = a[i][1];
                AT(p, 0x18 + i * 0x10, f32) = a[i][2];
                AT(p, 0x1C + i * 0x10, f32) = a[i][3];
            }
            for (i = 0; i < 4; i++) {
                AT(p, 0x50 + i * 0x10, f32) = b[i][0];
                AT(p, 0x54 + i * 0x10, f32) = b[i][1];
                AT(p, 0x58 + i * 0x10, f32) = b[i][2];
                AT(p, 0x5C + i * 0x10, f32) = b[i][3];
            }
            p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 1);
            p[0] = 0x10000000;
            AT(p, 0x8, u32) = 0x14000000;   /* MSCAL 0 */
            AT(p, 0xC, u32) = 0;
            p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, newTex + 3);
            if (p != NULL) {
                p[0] = (u32)((newTex + 2) | 0x10000000);
                AT(p, 0x8, u32) = VIF_STCYCL_1_1;
                AT(p, 0xC, u32) = (newTex + 2) | 0x50000000;
                p[2] = (u64)(s64)(newTex + 1) | 0x8000 | 0x1000000000000000ull;
                p[3] = 0xE;
                p[4] = ((u64)(s64)newTex << 4 | 0xC) | (u64)AT(o, 0x84, u8) << 6;
                p[5] = GS_REG_PRIM;
                if (newTex) {
                    p[6] = AT(o, 0x10, u64);
                    p[7] = GS_REG_TEX0_1;
                }
            }
            if (AT(o, 0x4, s32) == 1) {
                mesh = func_0025DD80(o, mesh);
            } else if (AT(o, 0x4, s32) == 0) {
                mesh = func_0025E100(o, mesh);
            }
        } while (mesh[0] != -1);
    }
    if ((u32)(AT(o, 0x18, s32) - 4) < 2) {
        p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 4);
        p[0] = 0x10000003;
        AT(p, 0x8, u32) = 0;
        AT(p, 0xC, u32) = 0x50000003;
        p[2] = 0x1000000000008002ull;
        p[3] = 0xE;
        p[4] = 0x310000A0;   /* ZBUF_1: z writes again */
        p[5] = GS_REG_ZBUF_1;
        p[6] = 0x44;         /* ALPHA_1: normal blending */
        p[7] = GS_REG_ALPHA_1;
    }
    return 1;
}

extern void func_0025C6F0(f32 *q, f32 *axis, f32 angle);   /* rotation about an axis */
extern void func_0025C770(f32 *q, f32 (*m)[4]);            /* its matrix */

/* the view's side edge `which` (0 left, 1 right): the camera's direction (+0xA0) turned about
 * its up axis (+0xA4) by fov / 1.3, at the look-at distance from the eye -> +0x20 + 16 * which;
 * then that point swung back about y -> +0x40 + 16 * which (the edge planes for culling) */
void func_0025DB10(u8 *o, s32 which) {
    static const union { u32 u; f32 f; } k13 = {0x3FA66666};
    VObject *cam = D_0044E4B8;
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

extern s32 func_0025CB50(u8 *o);
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
        ok = func_0025CB50(o);
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

#define DMA_REF_QWC(bytes) ((u64)(u32)((((bytes) + 15) >> 4) | 0x30000000))
#define DMA_ADDR(p) ((u64)((u32)(p) & 0x0FFFFFFF) << 32)

/* send a batch's vertices to VU1 { count, batch, +0x8 positions (x, y, z, w floats), +0xC
 * texture coordinates (s, t floats), +0x10 colours (RGBA bytes) } (pointers advance): the batch's own data is called first
 * (renderer +0x20, which says whether it is ready), then per 48 vertices a GIF tag and three
 * referenced UNPACKs, each started with MSCNT; a return tag with FLUSHA ends it. */
s32 func_002B7500(u8 *a) {
    VObject *r = D_0044E4F0;
    u64 *call = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 1);
    u8 ready;
    void *dst = VCALL(r, 0x20, void *(*)(VObject *, void *, u8 *))(r, AT(a, 0x8, void *), &ready);
    s32 n;

    call[0] = 0x50000000 | DMA_ADDR(dst);
    AT(call, 0x8, u32) = 0;
    AT(call, 0xC, u32) = 0;
    if (ready == 0) {
        return 1;
    }
    for (n = AT(a, 0x0, s32); n > 0; n -= 48) {
        s32 k = n < 48 ? n : 48;
        u64 *p = VCALL(r, 0x18, u64 *(*)(VObject *, s32))(r, 5);

        p[0] = 0x10000001;
        AT(p, 0x8, u32) = 0x01000101;
        AT(p, 0xC, u32) = 0x6C018000;
        p[2] = (u64)(s64)k | 0x8000 | 0x3000000000000000ull;   /* GIF tag: k x (ST, RGBAQ, XYZF2) */
        p[3] = 0x512;
        p[4] = DMA_REF_QWC(k * 8) | DMA_ADDR(AT(a, 0xC, u8 *));
        AT(p, 0x28, u32) = 0x01000103;
        AT(p, 0x2C, u32) = (k << 16) | 0x74008001;
        AT(a, 0xC, u8 *) += k * 8;
        p[6] = DMA_REF_QWC(k * 4) | DMA_ADDR(AT(a, 0x10, u8 *));
        AT(p, 0x38, u32) = 0;
        AT(p, 0x3C, u32) = (k << 16) | 0x6E00C002;
        AT(a, 0x10, u8 *) += k * 4;
        p[8] = DMA_REF_QWC(k * 16) | DMA_ADDR(AT(a, 0x8, u8 *));
        AT(p, 0x48, u32) = 0;
        AT(p, 0x4C, u32) = (k << 16) | 0x6C008003;
        AT(a, 0x8, u8 *) += k * 16;
        p = VCALL(r, 0x18, u64 *(*)(VObject *, s32))(r, 1);
        p[0] = 0x10000000;
        AT(p, 0x8, u32) = 0x17000000;   /* MSCNT */
        AT(p, 0xC, u32) = 0;
    }
    call = VCALL(r, 0x18, u64 *(*)(VObject *, s32))(r, 1);
    call[0] = 0x60000000;   /* DMA ret */
    AT(call, 0x8, u32) = 0;
    AT(call, 0xC, u32) = 0x13000000;   /* FLUSHA */
    return 1;
}

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

/* a batch's view-dependent placement: parallax (+0x86 = 2 / 4 / 8 / 16: moved sideways by
 * 0.2 / 0.25 / 0.33 / 0.5 of the camera's offset +0x20) and billboards (+0x88 = 2 upright, 4
 * facing the eye) */
void func_0025D560(u8 *o) {
    static const union { u32 u; f32 f; } k02 = {0x3E4CCCCD}, k033 = {0x3EA8F5C3};
    VObject *cam = D_0044E4B8;
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
        mesh_face_eye(o, D_0044E4B8, 0);
    }
}


extern f32 func_002E2D00(f32 angle);   /* wrapped into -pi..pi */
extern void func_002B72D0(s32 flags, s32 mode, s32 tex, u64 tex0);   /* the GS state of a part */
extern s32 func_002B7780(void *part);   /* send a rigid part's vertices to VU1 */
extern void *D_003A58A0[];   /* the rigid-part VU1 microprogram */


/* a placed object's rigid model (+0x30), the plain case: its matrix (scale 32, turned by +0x20 /
 * +0x24 / +0x28, at +0x10), the camera's view and clip matrices times it, the turn alone for
 * the lighting and a fixed light set go to VU1, then the part's streams */
void func_00267D60(u8 *o) {
    static const union { u32 u; f32 f; } k0001 = {0x3A83126F};   /* 0.001 */
    VObject *r = D_0044E4F0;
    VObject *cam;
    f32 unit[4][4] __attribute__((aligned(16)));
    f32 light[4][4] __attribute__((aligned(16)));
    f32 world[4][4] __attribute__((aligned(16)));
    f32 turn[4][4] __attribute__((aligned(16)));
    f32 view[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 amb[4] __attribute__((aligned(16)));
    struct {
        s32 n;
        void *start;    /* the positions' s32 start (VIF row) */
        void *pos;      /* s16 deltas */
        void *uv;
        void *normal;
        void *strip;
    } a;
    u64 *p;
    u64 tex0;

    p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 1);
    p[0] = 0x50000000 | (u64)((u32)D_003A58A0 & 0x0FFFFFFF) << 32;   /* DMA call */
    AT(p, 0x8, u32) = 0x01000101;   /* STCYCL 1, 1 */
    AT(p, 0xC, u32) = 0;
    sceVu0UnitMatrix(unit);
    light[0][0] = 64.0f;
    light[0][1] = 64.0f;
    light[0][2] = 64.0f;
    light[1][0] = 0.0f;
    light[0][3] = 1.0f;
    light[1][1] = 0.0f;
    light[1][2] = 0.0f;
    light[1][3] = 1.0f;
    light[2][0] = 0.0f;
    light[2][1] = 0.0f;
    light[2][2] = 0.0f;
    light[2][3] = 1.0f;
    light[3][3] = 1.0f;
    light[3][0] = 128.0f;
    light[3][1] = 128.0f;
    light[3][2] = 128.0f;
    amb[0] = k0001.f;
    amb[1] = k0001.f;
    amb[2] = k0001.f;
    amb[3] = 0.0f;
    AT(o, 0x20, f32) = func_002E2D00(AT(o, 0x20, f32));
    AT(o, 0x24, f32) = func_002E2D00(AT(o, 0x24, f32));
    AT(o, 0x28, f32) = func_002E2D00(AT(o, 0x28, f32));
    sceVu0UnitMatrix(world);
    world[2][2] = 32.0f;
    world[1][1] = 32.0f;
    world[0][0] = 32.0f;
    sceVu0RotMatrixX(world, world, AT(o, 0x20, f32));
    sceVu0RotMatrixY(world, world, AT(o, 0x24, f32));
    sceVu0RotMatrixZ(world, world, AT(o, 0x28, f32));
    sceVu0TransMatrix(world, world, (f32 *)(o + 0x10));
    cam = D_0044E4B8;
    VCALL(cam, 0x44, void (*)(VObject *, f32 (*)[4]))(cam, view);
    VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    sceVu0CopyMatrix(turn, world);
    turn[3][2] = 0.0f;
    turn[3][1] = 0.0f;
    turn[3][0] = 0.0f;
    sceVu0MulMatrix(turn, unit, turn);
    sceVu0MulMatrix(view, view, world);
    sceVu0MulMatrix(clip, clip, world);
    p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 0x13);
    p[0] = 0x10000011;              /* DMA cnt 0x11 */
    AT(p, 0x8, u32) = 0x01000101;   /* STCYCL 1, 1 */
    AT(p, 0xC, u32) = 0x6C118000;   /* UNPACK V4-32 0x11 to 0 */
    sceVu0CopyMatrix((f32 (*)[4])(p + 2), view);
    sceVu0CopyMatrix((f32 (*)[4])(p + 10), clip);
    sceVu0CopyMatrix((f32 (*)[4])(p + 18), turn);
    sceVu0CopyMatrix((f32 (*)[4])(p + 26), light);
    sceVu0CopyVector((f32 *)(p + 34), amb);
    p[36] = 0x10000000;   /* DMA cnt 0 */
    p[37] = 0x14000000;   /* MSCAL 0 */
    a.n = 0;
    a.start = NULL;
    a.pos = NULL;
    a.uv = NULL;
    a.normal = NULL;
    a.strip = NULL;
    tex0 = func_002B71D0(AT(AT(o, 0x30, u8 *), 0x44, s32));
    func_002B72D0(AT(o, 0x40, u8), 0, AT(AT(o, 0x30, u8 *), 0x44, s32), tex0);
    a.start = AT(o, 0x30, u8 *) + 0x60;
    a.n = AT(AT(o, 0x30, u8 *), 0x40, s32);
    a.pos = AT(o, 0x30, u8 *) + AT(AT(o, 0x30, u8 *), 0x54, s32);
    a.uv = AT(o, 0x30, u8 *) + AT(AT(o, 0x30, u8 *), 0x4C, s32);
    a.normal = AT(o, 0x30, u8 *) + AT(AT(o, 0x30, u8 *), 0x50, s32);
    a.strip = AT(o, 0x30, u8 *) + AT(AT(o, 0x30, u8 *), 0x58, s32);
    func_002B7780(&a);
}

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
    VCALL(D_0044E4B8, 0x48, void (*)(VObject *, f32 (*)[4]))(D_0044E4B8, mvp);
    sceVu0MulMatrix(mvp, mvp, world);
}

static void obj_strip(u8 *o, f32 (*mvp)[4], s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba) {
    u8 *model = AT(o, 0x30, u8 *);
    s32 tex = AT(model, 0x44, s32);

    glr_strip(&mvp[0][0], n, xyzw, st, rgba,
              tex == -1 ? NULL : VCALL(D_0044E4E8, 0xC, void *(*)(VObject *, s32, s32))(D_0044E4E8, tex, 0),
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
    default:
        glr_todo("placed object, morphing model (func_002677C0 / func_00267560)");
        break;
    }
}
#endif

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
