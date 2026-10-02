/* Mesh drawing (the room's parts, vtable D_0046C770): VU1 packets for the renderer. */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern VObject *D_0044E4F0;   /* the renderer */
extern VObject *D_0044E4B8;   /* the camera */

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
