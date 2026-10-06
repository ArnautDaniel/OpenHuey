#include "common.h"
#include "globals.h"
extern u8 *gCharPlayer; /* global manager object */
#include "ptmf.h"
#include "progress.h"
#include "item.h"

/* Field access by byte offset into objects whose layout is not yet known. */
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
/* writes {x, 0, z} */
#define B5_SET3(out, x, z) ((out)[0] = (x), (out)[1] = 0.0f, (out)[2] = (z))
/* gFileLoader vtable +0xC: start loading file `name` into `dest` (flags 0x4000000) */
#define FILE_LOAD_ASYNC(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, void *, u32, s32))(gFileLoader, name, dest, 0x4000000, 0)

extern u8 D_0042E440[];
extern u8 D_0042E460[];
extern u8 D_0042E480[];
extern u8 D_0042E4A0[];
extern u8 D_00460F00[];
extern u8 D_00460F20[];
extern u8 D_00460F40[];
extern u8 D_00460F60[];
extern u8 D_00460F80[];
extern u8 D_00460FA0[];
extern u8 D_00460FC0[];

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

/* gProgress +0x7E0 += d; +0x7D0 += d - (+0x7D4) unless that is negative */
static inline void b5_add_7E0(f32 d) {
    u8 *p = (u8 *)gProgress;
    f32 r;

    F32(p, 0x7E0) += d;
    r = d - F32(p, 0x7D4);
    if (!(r < 0.0f)) {
        F32(p, 0x7D0) += r;
    }
}

void func_0032F6D0(void *self, s32 id, f32 *out) {
    switch (id) {
    case 1:
        B5_SET3(out, 0.0f, 0x1.4ccccc0000000p+3f /* 10.4 */);
        break;
    case 3:
        B5_SET3(out, 0.0f, -0x1.dc2f840000000p+2f /* 7.4404 */);
        break;
    case 0:
        B5_SET3(out, 0.0f, -0x1.8d89380000000p+2f /* 6.2115 */);
        break;
    case 2:
        B5_SET3(out, 0.0f, 0x1.9276c80000000p+2f /* 6.2885 */);
        break;
    }
}

void func_0032F770(void *self, s32 id, f32 *out) {
    switch (id) {
    case 10:
    case 11:
        B5_SET3(out, 0x1.e2f8380000000p-1f /* 0.9433 */, 0x1.6807600000000p+3f /* 11.2509 */);
        break;
    case 12:
    case 13:
        B5_SET3(out, 0x1.1656040000000p+1f /* 2.1745 */, 0x1.e1573e0000000p+3f /* 15.0419 */);
        break;
    case 14:
        B5_SET3(out, -0x1.9c98600000000p+0f /* 1.6117 */, -0x1.15e00e0000000p+2f /* 4.3418 */);
        break;
    case 15:
        B5_SET3(out, 0x1.2a30560000000p-6f /* 0.0182 */, -0x1.00346e0000000p+0f /* 1.0008 */);
        break;
    }
}

void func_0032F820(u8 *self) {
    self[0x16EE] = 1;
}

u32 func_003310A0(void) {
    return 0x2C020068;
}

f32 func_003310D0(void) {
    return 0x1.3333340000000p-1f /* 0.6 */;
}

f32 func_003310F0(void) {
    return 0x1.6666660000000p+0f /* 1.4 */;
}

f32 func_00331110(void) {
    return 24.0f;
}

f32 func_00331120(void) {
    return 16.0f;
}

f32 func_00331130(void) {
    return 2e+01f;
}

f32 func_00331140(void) {
    return 1e+01f;
}

f32 func_00331150(void) {
    return 2e+01f;
}

f32 func_00331160(void) {
    return 0x1.eb851e0000000p-4f /* 0.12 */;
}

f32 func_00331180(void) {
    return 6e+01f;
}

f32 func_00331190(void) {
    return 0x1.1df46a0000000p-3f /* 0.13962634 */;
}

f32 func_003311B0(void) {
    return 0x1.aceea00000000p-5f /* 0.05235988 */;
}

void func_003311D0(u8 *self, s32 t) {
    S32(self, 0x1660) = t != 0 ? t : 900;
}

void func_003311F0(u8 *self) {
    S32(self, 0x1660) = 600;
}

void *func_00331200(void) {
    return b5_prog_flag8000() ? D_0042E4A0 : D_0042E460;
}

void *func_00331240(void) {
    return b5_prog_flag8000() ? D_0042E480 : D_0042E440;
}

u32 func_003314C0(void) {
    return 0x80000005;
}

s32 func_003314E0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00460F00, dest);
}

s32 func_00331510(void) {
    item_meters(-25.0f, -450);
    return 1;
}

u32 func_00331660(void) {
    return 0x80000005;
}

s32 func_00331680(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00460F20, dest);
}

s32 func_003316B0(void) {
    item_meters(-100.0f, -1800);
    return 1;
}

u32 func_00331800(void) {
    return 0x80000005;
}

s32 func_00331820(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00460F40, dest);
}

s32 func_00331850(void) {
    b5_add_7E0(25.0f);
    return 1;
}

u32 func_00331920(void) {
    return 0x80000005;
}

s32 func_00331940(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00460F60, dest);
}

s32 func_00331970(void) {
    b5_add_7E0(100.0f);
    return 1;
}

u32 func_00331A40(void) {
    return 0x80000005;
}

s32 func_00331A60(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00460F80, dest);
}

extern u8 *gCharPartner;
extern void func_00138AD0(void *h, s32 mode, s32 time);

#define B5_HANDY(o) (VCALL(o, 0x40, s32 (*)(void *))(o) & 0xFF)

/* use: composure +25, she drinks (event 0x8D) */
s32 func_00331A90(void) {
    item_composure(25.0f);
    item_event(gEvents, 0, 0x8D, gCharPlayer);
    return 5;
}

u32 func_00331BD0(void) {
    return 0x80000005;
}

s32 func_00331BF0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00460FA0, dest);
}

/* use: composure +100 */
s32 func_00331C20(void) {
    item_composure(100.0f);
    item_event(gEvents, 0, 0x8D, gCharPlayer);
    return 5;
}

u32 func_00331D50(void) {
    return 0x80000005;
}

s32 func_00331D70(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00460FC0, dest);
}

/* use: Progress +0x9E8 2.0 for 1800 frames (+0x9EC) */
s32 func_00331DA0(void) {
    F32(gProgress, 0x9E8) = 2.0f;
    S32(gProgress, 0x9EC) = 1800;
    item_event(gEvents, 0, 0x8D, gCharPlayer);
    return 5;
}

/* use: unless it still runs, Progress +0x9F4 / +0x9F8 1800 frames */
s32 func_00331EE0(void) {
    if (S32(gProgress, 0x9F4) != 0) {
        return 0;
    }
    S32(gProgress, 0x9F8) = 1800;
    S32(gProgress, 0x9F4) = 1800;
    item_event(gEvents, 0, 0x8D, gCharPlayer);
    return 5;
}

/* use: composure -50, Progress +0x7D8 +50 */
s32 func_00332030(void) {
    item_composure(-50.0f);
    F32(gProgress, 0x7D8) += 50.0f;
    item_event(gEvents, 0, 0x8D, gCharPlayer);
    return 5;
}

/* use: Hewie heals 20; already full, he trusts her a little less */
s32 func_003324A0(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    if (item_hewie_heal(20)) {
        item_trust(-1);
    }
    return item_give_hewie(gEvents, 0x8F);
}

/* use: Hewie heals 100; trust -1 */
s32 func_003326D0(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    item_hewie_heal(100);
    item_trust(-1);
    return item_give_hewie(gEvents, 0x8F);
}

/* use: Hewie's +0x94 (50); trust +3 */
s32 func_003328E0(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    VCALL(gCharPartner, 0x94, void (*)(u8 *, s32))(gCharPartner, 50);
    item_trust(3);
    return item_give_hewie(gEvents, 0x90);
}

/* use: as func_003324A0 */
s32 func_00332AD0(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    if (item_hewie_heal(20)) {
        item_trust(-1);
    }
    return item_give_hewie(gEvents, 0x8F);
}

/* use: as func_003326D0 */
s32 func_00332D10(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    item_hewie_heal(100);
    item_trust(-1);
    return item_give_hewie(gEvents, 0x8F);
}

/* use: Hewie waits (func_00138AD0 mode 3, 450 frames; Progress +0xA10 too); trust +20 */
s32 func_00332F30(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    func_00138AD0(gCharPartner, 3, 450);
    S32(gProgress, 0xA10) = 450;
    item_trust(20);
    return item_give_hewie(gEvents, 0x91);
}

/* use: Hewie's health 1 (back on his feet); trust +3 */
s32 func_00333120(void *o) {
    if (!B5_HANDY(o)) {
        return 0;
    }
    S32(gCharPartner, 0x14C8) = 1;
    item_trust(3);
    return item_give_hewie(gEvents, 0x90);
}
