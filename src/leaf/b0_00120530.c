/* Leaf functions (batch 0), 0x00120530 - 0x0012BFF0. */
#include "common.h"
#include "ptmf.h"
#include "progress.h"

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern u8 D_00469A00[]; /* vtable (base class) */
extern u8 D_00469C20[]; /* vtable (derived class) */
extern u8 D_003AF250[];
extern u8 D_003AFAE0[];
extern u8 D_003EC3E0[]; /* table of 28-byte entries */
extern u8 D_003AF1D0[];
extern u8 D_003AF1F0[];
extern u8 D_003AF210[];
extern u8 D_003AF230[];

void func_00120530(void *p, s32 a, s32 b) {
    FLD(p, 0x4, s32) = a;
    FLD(p, 0x8, s32) = b;
}

/* Fixed-size pool: +0x4 base, +0xC element size, +0x10 count, +0x14 in-use flags. */
typedef struct B0_Pool {
    u32 unk0;
    u8 *base;
    u32 unk8;
    u32 elemSize;
    u32 count;
    u8 *used;
} B0_Pool;

void *func_00120D60(B0_Pool *p, u32 i) {
    if (i < p->count && p->used[i] != 0) {
        return p->base + i * p->elemSize;
    }
    return NULL;
}

/* Free an element by address. */
/* Allocate an element of the given size. */
/* Constructor: base vtable, then derived vtable. */
void *func_00120F40(void *p) {
    if (p != NULL) {
        *(void **)p = D_00469A00;
        *(void **)p = D_00469C20;
    }
    return p;
}

/* Copy the translation column of a matrix (rows at +0xC4) into a vec4 (w = 0). */
s32 func_00121B40(void *p, f32 a, f32 b) {
    f32 f10, fc, d, lo, hi;
    s32 n;

    if (a == b) {
        return 0;
    }
    f10 = FLD(p, 0x10, f32);
    fc = FLD(p, 0xC, f32);
    if (f10 == b) {
        n = 0;
    } else {
        n = (s32)(((a * (f10 - b)) / f10) / (b - a)) + 1;
    }
    d = b - a;
    lo = 65536.0f * ((f32)n - ((a * (f10 - b)) / f10) / d);
    hi = 65536.0f * ((f32)n + ((a * (b - fc)) / fc) / d);
    if (lo < 0.0f || !(lo <= 16777215.0f)) {
        return 0;
    }
    if (hi < 0.0f || !(hi <= 16777215.0f)) {
        return 0;
    }
    if (hi - lo < 65536.0f) {
        return 0;
    }
    FLD(p, 0x18, f32) = lo;
    FLD(p, 0x1C, f32) = hi;
    FLD(p, 0x28, f32) = a;
    FLD(p, 0x2C, f32) = b;
    return 1;
}

/* Count 32-byte records up to a -1 terminator. */
/* Move the point at +0x50 and translate +0x40 by the same delta. */
/* Move the point at +0x40 and translate +0x50 by the same delta. */
/* Tail call of virtual function 0xC (arguments passed through). */
void *func_00122B30(void *p) {
    if (p != NULL) {
        *(void **)p = D_00469C20;
    }
    return p;
}

/* Count down the timer at +0x14C8 by |n|; 1 (and clamp to 0) when it runs out. */
s32 func_00124ED0(void *p, s32 n) {
    if (n <= 0) {
        n = -n;
    }
    FLD(p, 0x14C8, s32) -= n;
    if (FLD(p, 0x14C8, s32) > 0) {
        return 0;
    }
    FLD(p, 0x14C8, s32) = 0;
    return 1;
}

u32 func_00126EC0(void *p) {
    if (FLD(p, 0xF8, s32) == 6) {
        s32 k = FLD(p, 0xFC, s32);

        if (k == 0x16) {
            return FLD(p, 0x14C0, u16);
        }
        if (k == 0x17) {
            s32 i = FLD(p, 0x1388, s32);

            if (i < FLD(p, 0x1384, s32)) {
                return FLD(p, 0x138C + i * 2, u16);
            }
        }
    }
    return 0xFFFF;
}

void *func_001278C0(void) {
    return D_003AF250;
}

void func_001278D0(void *p, s32 kind, f32 *out) {
    f32 z;

    switch (kind) {
    case 0:
        z = -0x1.8d89380000000p+2f /* 6.2115 */;
        break;
    case 1:
        z = 0x1.4ccccc0000000p+3f /* 10.4 */;
        break;
    case 2:
        z = 0x1.9276c80000000p+2f /* 6.2885 */;
        break;
    case 3:
        z = -0x1.dc2f840000000p+2f /* 7.4404 */;
        break;
    default:
        return;
    }
    out[0] = 0.0f;
    FLD(out, 4, s32) = 0;
    out[2] = z;
}

void func_00127970(void *p, s32 kind, f32 *out) {
    f32 x, z;

    switch (kind) {
    case 10:
    case 11:
        x = 0x1.e2f8380000000p-1f /* 0.9433 */;
        z = 0x1.6807600000000p+3f /* 11.2509 */;
        break;
    case 12:
    case 13:
        x = 0x1.1656040000000p+1f /* 2.1745 */;
        z = 0x1.e1573e0000000p+3f /* 15.0419 */;
        break;
    case 14:
        x = -0x1.9c98600000000p+0f /* 1.6117 */;
        z = -0x1.15e00e0000000p+2f /* 4.3418 */;
        break;
    case 15:
        x = 0x1.2a30560000000p-6f /* 0.0182 */;
        z = -0x1.00346e0000000p+0f /* 1.0008 */;
        break;
    default:
        return;
    }
    out[0] = x;
    FLD(out, 4, s32) = 0;
    out[2] = z;
}

void *func_00127A20(void) {
    return D_003AFAE0;
}

void func_00127A30(void *p) {
    FLD(p, 0x16EE, u8) = 1;
}

void func_00127B20(void *p, u32 id) {
    u8 *e;

    FLD(p, 0x1758, u32) = id;
    if (id & 0x1000) {
        e = FLD(p, 0x1714, u8 *) + (id & 0xFFF) * 28;
    } else {
        e = D_003EC3E0 + id * 28;
    }
    FLD(p, 0x175C, s32) = FLD(e, 0xC, s32);
    FLD(p, 0xF8, s32) = FLD(e, 0x10, s32);
    FLD(p, 0xFC, s32) = FLD(e, 0x14, s32);
}

void func_00127B90(void *p) {
    FLD(p, 0x1660, s32) = FLD(p, 0x16D4, s32);
}

void func_00127BA0(void *p) {
    FLD(p, 0x1664, s32) = FLD(p, 0x16D0, s32);
}

f32 func_00127BD0(void) {
    return 10.0f;
}

f32 func_00127BE0(void) {
    return 5.0f;
}

f32 func_00127BF0(void) {
    return 20.0f;
}

void func_00127C10(void *p, s32 on) {
    FLD(p, 0x16B8, s32) = on ? 2 : 0;
}

s32 func_00127C30(void *p) {
    return FLD(p, 0x20, s32) == 2;
}

s32 func_0012BE50(void) {
    return 0x2C020068;
}

f32 func_0012BE80(void) {
    return 0x1.3333340000000p-1f /* 0.6 */;
}

f32 func_0012BEA0(void) {
    return 0x1.6666660000000p+0f /* 1.4 */;
}

f32 func_0012BEC0(void) {
    return 24.0f;
}

f32 func_0012BED0(void) {
    return 16.0f;
}

f32 func_0012BEE0(void) {
    return 20.0f;
}

f32 func_0012BEF0(void) {
    return 10.0f;
}

f32 func_0012BF00(void) {
    return 20.0f;
}

f32 func_0012BF10(void) {
    return 0x1.eb851e0000000p-4f /* 0.12 */;
}

f32 func_0012BF30(void) {
    return 60.0f;
}

f32 func_0012BF40(void) {
    return 0x1.1df46a0000000p-3f /* 0.13962634 */; /* 8 degrees in radians */
}

f32 func_0012BF60(void) {
    return 0x1.aceea00000000p-5f /* 0.052359879 */; /* 3 degrees in radians */
}

void func_0012BF80(void *p, s32 t) {
    FLD(p, 0x1660, s32) = t != 0 ? t : 900;
}

void func_0012BFA0(void *p) {
    FLD(p, 0x1660, s32) = 600;
}

void *func_0012BFB0(void) {
    if (FLD(gProgress, 0x30, u32) & 0x8000) {
        return D_003AF230;
    }
    return D_003AF1F0;
}

void *func_0012BFF0(void) {
    if (FLD(gProgress, 0x30, u32) & 0x8000) {
        return D_003AF210;
    }
    return D_003AF1D0;
}
