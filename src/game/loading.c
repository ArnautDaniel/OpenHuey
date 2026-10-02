/* The loading screen: a turning emblem (from GAME_FIX.GFM) and the "Now Loading" box, drawn
 * while a room loads. The emblem is a static model drawn through VU1. */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "gs.h"
#include "sce/libvu0.h"

extern void *D_00469D00[];
#include "task.h"

extern VObject *D_0044E4E8;   /* the texture cache */
extern VObject *D_0044E4B8;   /* the camera */
extern VObject *D_0044E4F0;   /* the renderer */
extern void *D_0046D7A0[];
extern f32 func_002E2D00(f32 x);
extern void func_0026B180(void *o, u32 rgba, s32 a, s32 b);

/* the loading screen, frame `frame`: the camera at a fixed spot, the emblem (`o`, a model object)
 * turning (its rotation from the frame count) drawn in layers 1 and 0x26, the screen dimmed, and
 * the box for the "Now Loading" message (0x808B) */
void func_0033E2A0(u8 *o, s32 frame) {
    VObject *tc = D_0044E4E8, *cam = D_0044E4B8, *r;
    u8 dim[0x10] __attribute__((aligned(16)));
    Task t;
    s32 box[13];
    s32 w, x;
    f32 a;

    VCALL(tc, 0x18, void (*)(VObject *))(tc);
    VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, 0x1.0e9ed6p+6f, 0x1.0948dep+5f, 0x1.12a69ap+6f);
    VCALL(cam, 0x5C, void (*)(VObject *, f32))(cam, 0x1.0c1524p-1f);   /* 30 degrees */
    VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, -0x1.6a315cp+4f, 0x1.cacac0p+3f, 0x1.a37d6cp+2f);
    VCALL(cam, 0x14, void (*)(VObject *))(cam);

    AT(o, 0x10, f32) = 0.0f;
    AT(o, 0x14, f32) = 0.0f;
    AT(o, 0x18, f32) = 0.0f;
    AT(o, 0x1C, f32) = 1.0f;
    AT(o, 0x20, f32) = 0.0f;
    AT(o, 0x24, f32) = 0.0f;
    AT(o, 0x28, f32) = 0.0f;
    AT(o, 0x2C, f32) = 0.0f;
    AT(o, 0x30, f32) = func_002E2D00((f32)frame * 0x1.45b3d6p-6f);
    AT(o, 0x34, f32) = 0.0f;
    AT(o, 0x38, f32) = 0.0f;
    AT(o, 0x3C, f32) = 0.0f;
    AT(o, 0x40, f32) = func_002E2D00((f32)frame * 0x1.750728p-6f);
    AT(o, 0x44, f32) = 0.0f;
    AT(o, 0x48, f32) = 0.0f;
    AT(o, 0x4C, f32) = 0.0f;
    a = (f32)frame * 0x1.b43958p-6f;
    AT(o, 0x50, f32) = func_002E2D00(a);
    AT(o, 0x54, f32) = 0.0f;
    AT(o, 0x58, f32) = 0.0f;
    AT(o, 0x5C, f32) = 0.0f;
    AT(o, 0x60, f32) = 0.0f;
    AT(o, 0x64, f32) = func_002E2D00(a);
    AT(o, 0x68, f32) = 0.0f;
    AT(o, 0x6C, f32) = 0.0f;
    AT(o, 0x70, s32) = 0;
    r = D_0044E4F0;
    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, o, 1, 0);
    AT(o, 0x70, s32) = 1;
    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, o, 0x26, 0);

    AT(dim, 0x0, void **) = D_0046D7A0;
    AT(dim, 0x4, s32) = -1;
    func_0026B180(dim, 0x5A623C32, 0x28, 0);
    VCALL(tc, 0x18, void (*)(VObject *))(tc);

    Task_Construct(&t);
    w = Task_MessageWidth(&t, 0x808B, 0x10);
    x = 0x1B0;
    if (w >= 0x69) {
        x -= (w - 0x68) >> 1;
    }
    x -= w >> 1;
    box[5] = 0;
    box[2] = w + 0x28;
    box[0] = x - 0x14;
    box[3] = 0x20;
    box[1] = 0x198;
    box[7] = 0x20;
    box[4] = 0xFF;
    box[6] = 0x90;
    box[8] = 0x80808080;
    box[9] = 2;
    box[10] = 0x10;
    box[11] = 0x30;
    box[12] = 6;
    VCALL(r, 0x80, void (*)(VObject *, s32 *))(r, box);
    if (t.child != NULL) {
        Task_dtor(t.child, 1);
        t.child = NULL;
    }
    AT(dim, 0x0, void **) = D_00469D00;
}

extern VObject *D_0044E9A0;   /* the VRAM manager */
extern VObject *D_0044E978;   /* the game: +0x1C its fixed models (GAME_FIX.GFM) */
extern u8 D_003AC3F0[];       /* DMA chain: the static model microprogram (MPG) */

#define PI 0x1.921fb6p+1f
#define TWO_PI 0x1.921fb6p+2f
#define DMA_ADDR(p) ((u64)((u32)(p) & 0x0FFFFFFF) << 32)

/* an angle into -pi..pi */
static inline f32 wrap_angle(f32 a) {
    a = a - TWO_PI * (f32)(s32)(a / TWO_PI);
    if (!(a <= PI)) {
        a -= TWO_PI;
    }
    return a;
}

/* Draw the loading emblem `o` (renderer object: +0x10 position, +0x20 rotation, +0x30.. the
 * parts' extra rotations, +0x70 1: only the last part). Its parts are chained, each turned and
 * moved relative to the previous; each is sent to VU1 in batches of 64 vertices: a GIF tag, UVs
 * (V2-16), colours (V4-8), positions (V3-16, differences from the part's start, +0x20) and strip
 * flags (S-8 masked into position w). */
s32 func_0033D9F0(u8 *o) {
    VObject *tc = D_0044E4E8, *r, *vram, *cam;
    f32 world[4][4] __attribute__((aligned(16)));
    f32 view[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 rel[4][4] __attribute__((aligned(16)));
    f32 extra[4][4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 trans[4] __attribute__((aligned(16)));
    f32 rot[4] __attribute__((aligned(16)));
    s32 slot, curTex, count, n, nv, tex;
    u8 *texh, *model, *part, *uv, *col, *pos, *flags;
    u64 *q;

    slot = VCALL(tc, 0x8, s32 (*)(VObject *, s32, s32))(tc, 2, 0x10);
    if (slot == -1) {
        return 0;
    }
    texh = VCALL(tc, 0xC, u8 *(*)(VObject *, s32, s32))(tc, 2, 0x10);
    if (slot & 0x80000000) {
        slot &= 0x7FFFFFFF;
        if (!(u8)VCALL(D_0044E4F0, 0x44, s32 (*)(VObject *, s32, void *, s32))(D_0044E4F0, slot, texh, -1)) {
            return 0;
        }
    }
    r = D_0044E4F0;
    q = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 1);
    if (q == NULL) {
        return 0;
    }
    q[0] = DMA_ADDR(D_003AC3F0) | 0x50000000;   /* DMA call: the microprogram */
    AT(q, 0x8, u32) = 0x01000101;                /* STCYCL 1, 1 */
    AT(q, 0xC, u32) = 0;
    q = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 4);
    if (q == NULL) {
        return 0;
    }
    q[0] = 0x10000003;
    AT(q, 0x8, u32) = 0;
    AT(q, 0xC, u32) = 0x50000003;                /* DIRECT 3 */
    q[2] = 0x8002 | (0x10000000ULL << 32);
    q[3] = 0xE;
    vram = D_0044E9A0;
    q[4] = VCALL(vram, 0x2C, u64 (*)(VObject *, s32, s32, s32, s32))(vram, slot, AT(texh, 4, u16), AT(texh, 6, u16), texh[1]);
    q[5] = GS_TEX0_1;
    q[6] = 0x5C;                                 /* PRIM: triangle strip, Gouraud, textured, blended */
    q[7] = GS_PRIM;
    curTex = -1;
    model = VCALL(D_0044E978, 0x1C, u8 *(*)(VObject *))(D_0044E978);
    if (AT(model, 0x0, s32) <= 0) {
        return 1;
    }
    part = model + AT(model, 0x0, s32);
    n = 0;
    count = AT(part, 0x8, s32);
    if (count == 0) {
        return 1;
    }
    cam = D_0044E4B8;
    do {
        count--;
        uv = part + AT(part, 0x10, s32);
        col = part + AT(part, 0x14, s32);
        pos = part + AT(part, 0x18, s32);
        nv = AT(part, 0x0, s32);
        flags = part + AT(part, 0x1C, s32);
        if (n == 0) {
            sceVu0CopyVector(rot, (f32 *)(o + 0x20));
            sceVu0UnitMatrix(m);
            sceVu0RotMatrix(m, m, rot);
            sceVu0CopyVector(trans, (f32 *)(o + 0x10));
        } else {
            sceVu0AddVector(rot, rot, (f32 *)(part + 0x30));
            rot[0] = wrap_angle(rot[0]);
            rot[1] = wrap_angle(rot[1]);
            rot[2] = wrap_angle(rot[2]);
            sceVu0UnitMatrix(rel);
            sceVu0RotMatrix(rel, rel, rot);
            sceVu0UnitMatrix(extra);
            if (n > 0) {
                sceVu0RotMatrix(extra, extra, (f32 *)(o + 0x30 + (n - 1) * 0x10));
            }
            sceVu0MulMatrix(rel, rel, extra);
            sceVu0MulMatrix(m, m, rel);
            sceVu0AddVector(trans, trans, (f32 *)(part + 0x40));
        }
        n++;
        if (AT(o, 0x70, s32) != 0 && count != 0) {
            flags += nv;
        } else {
            sceVu0TransMatrix(world, m, trans);
            tex = AT(part, 0x4, s32);
            if (curTex != tex) {
                curTex = tex;
                q = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 3);
                if (q == NULL) {
                    return 0;
                }
                q[0] = 0x10000002;
                AT(q, 0x8, u32) = 0;
                AT(q, 0xC, u32) = 0x50000002;
                q[2] = 0x8001 | (0x10000000ULL << 32);
                q[3] = 0xE;
                q[4] = VCALL(vram, 0x34, u64 (*)(VObject *, s32, s32, s32, s32))(vram, slot, tex, texh[0], texh[1]);
                q[5] = 0x16;                     /* TEX2_1 */
            }
            VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, view);
            sceVu0MulMatrix(view, view, world);
            VCALL(cam, 0x44, void (*)(VObject *, f32 (*)[4]))(cam, clip);
            sceVu0MulMatrix(clip, clip, world);
            q = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 0xA);
            if (q == NULL) {
                return 0;
            }
            q[0] = 0x10000008;
            AT(q, 0x8, u32) = 0x01000101;
            AT(q, 0xC, u32) = 0x6C088000;            /* UNPACK V4-32 x 8 to 0 (+TOPS) */
            sceVu0CopyMatrix((void *)(q + 2), clip);
            sceVu0CopyMatrix((void *)(q + 10), view);
            q[18] = 0x10000000;
            AT(q, 0x98, u32) = 0x14000000;           /* MSCAL 0 */
            AT(q, 0x9C, u32) = 0;
            q = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 2);
            if (q == NULL) {
                return 0;
            }
            q[0] = DMA_ADDR(part + 0x20) | 0x30000001;   /* ref: the position base */
            AT(q, 0x8, u32) = 0x01000103;            /* STCYCL 3, 1 */
            AT(q, 0xC, u32) = 0x30000000;            /* STROW (the referenced quadword) */
            q[2] = 0x10000000;
            AT(q, 0x18, u32) = 0x20000000;           /* STMASK */
            AT(q, 0x1C, u32) = 0x3F;                 /*   x y z kept, w from the data */
            while (nv > 0) {
                s32 k = nv < 0x40 ? nv : 0x40;
                u32 qwFlags = (k + 15) >> 4, qw4 = (k * 4 + 15) >> 4, qw6 = (k * 6 + 15) >> 4;

                q = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 7);
                if (q == NULL) {
                    return 0;
                }
                q[0] = 0x10000001;
                AT(q, 0x8, u32) = 0;
                AT(q, 0xC, u32) = 0x6C018000;        /* UNPACK V4-32 x 1 to 0: */
                q[2] = (u64)(s64)k | 0x8000 | (0x30000000ULL << 32);   /* the GIF tag: k x (ST RGBAQ XYZ2), EOP */
                q[3] = 0x512;
                q[4] = (u64)(qw4 | 0x30000000) | DMA_ADDR(uv);
                AT(q, 0x28, u32) = 0;
                AT(q, 0x2C, u32) = (k << 16) | 0x6500C001;   /* UNPACK V2-16 unsigned to 1 */
                uv += k * 4;
                q[6] = (u64)(qw4 | 0x30000000) | DMA_ADDR(col);
                AT(q, 0x38, u32) = 0;
                AT(q, 0x3C, u32) = (k << 16) | 0x6E00C002;   /* UNPACK V4-8 unsigned to 2 */
                col += k * 4;
                q[8] = (u64)(qw6 | 0x30000000) | DMA_ADDR(pos);
                AT(q, 0x48, u32) = 0x05000002;               /* STMOD difference */
                AT(q, 0x4C, u32) = (k << 16) | 0x69008003;   /* UNPACK V3-16 to 3 */
                pos += k * 6;
                q[10] = (u64)(qwFlags | 0x30000000) | DMA_ADDR(flags);
                AT(q, 0x58, u32) = 0x05000000;               /* STMOD normal */
                AT(q, 0x5C, u32) = (k << 16) | 0x72008003;   /* UNPACK S-8 masked to 3 */
                flags += k;
                q[12] = 0x10000000;
                AT(q, 0x68, u32) = 0x17000000;               /* MSCNT */
                AT(q, 0x6C, u32) = 0;
                nv -= 0x40;
            }
            q = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 1);
            if (q == NULL) {
                return 0;
            }
            q[0] = 0x10000000;
            AT(q, 0x8, u32) = 0x13000000;                    /* FLUSHA */
            AT(q, 0xC, u32) = 0;
        }
        part = (u8 *)(((u32)flags + 15) & ~15u);
    } while (count != 0);
    return 1;
}
