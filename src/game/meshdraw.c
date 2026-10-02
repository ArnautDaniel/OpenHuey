/* Mesh drawing (the room's parts, vtable D_0046C770): VU1 packets for the renderer. */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern VObject *D_0044E4F0;   /* the renderer */
extern VObject *D_0044E4B8;   /* the camera */

#ifdef HG_NATIVE
/* the PC renderer (native/platform/glr.h): batches are drawn with OpenGL instead of the VU1 */
extern void glr_strip(const f32 mvp[16], s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, u64 tex0,
                      u32 prim);
static f32 sGlMvp[4][4];   /* the current batch's local-to-clip matrix */
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
            glr_strip(&sGlMvp[0][0], a.n, a.xyz, (f32 *)a.st, a.rgba, sGlTex0, sGlPrim);
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
