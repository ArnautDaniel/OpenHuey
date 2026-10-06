#include "common.h"
#include "ptmf.h"
#include "globals.h"

extern u32 D_00442950[];
extern u32 D_00442C10[];
extern u32 D_00442DA0[];
extern u32 D_00442F48[];
extern u32 D_00442F60[];
extern u32 D_00442FE0[];
extern u32 D_00443070[];
extern u32 D_00443080[];
extern u32 D_00443490[];
extern u32 D_004434B0[];
extern u32 D_0047AF60[];
extern u32 D_0047AF80[];
extern u32 D_0047AF94[];
extern u8 *gProgress;
extern u8 D_00462DF0[];
extern u8 D_00462E10[];
extern u8 D_00462E30[];
extern u8 D_00462E50[];
extern u8 D_00462E70[];
extern u8 D_00462E90[];
extern u8 D_00462EB0[];
extern u8 D_00462ED0[];
extern u8 D_00462EF0[];
extern u8 D_00462F10[];

void *func_0034DC60(void) {
    return D_00442950;
}

u32 func_0034DC80(void *self, s32 i) {
    return D_00442C10[i];
}

u32 func_0034DCA0(void *self, s32 i) {
    return D_0047AF60[i];
}

void func_0034E960(u8 *p) {
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

void *func_00350750(void) {
    return D_00442DA0;
}

u32 func_00350770(void *self, s32 i) {
    return D_00442F48[i];
}

u32 func_00350790(void *self, s32 i) {
    return D_0047AF80[i];
}

s32 func_00350C40(f32 *a) {
    a[1] += 0x1.1df46ap-7f /* 0.008726646 */;
    if (!(a[1] <= 0x1.921fb6p+1f /* 3.1415927 */)) {
        a[1] -= 0x1.921fb6p+2f /* 6.2831855 */;
    }
    a[2] += 0x1.c98712p-10f /* 0.0017453294 */;
    if (!(a[2] <= 0x1.921fb6p+1f /* 3.1415927 */)) {
        a[2] -= 0x1.921fb6p+2f /* 6.2831855 */;
    }
    a[3] -= 0x1.c98712p-10f /* 0.0017453294 */;
    if (a[3] < -0x1.921fb6p+1f /* -3.1415927 */) {
        a[3] += 0x1.921fb6p+2f /* 6.2831855 */;
    }
    return 1;
}

void *func_00350ED0(void) {
    return D_00442F60;
}

void *func_00350EE0(void) {
    return D_00442FE0;
}

u32 func_00350F00(void *self, s32 i) {
    return D_0047AF94[i];
}

void *func_00350F20(void) {
    return D_00443070;
}

void *func_00351060(void) {
    return D_00443080;
}

u32 func_00351070(void *self, s32 i) {
    return D_00443490[i];
}

u32 func_00351090(void *self, s32 i) {
    return D_004434B0[i];
}

s32 func_00351120(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00462DF0, dest, 0x4000000, 0);
}

s32 func_00351150(void) {
    *(u32 *)(gProgress + 0x84) |= 0x800000;
    return 2;
}

s32 func_003511E0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00462E10, dest, 0x4000000, 0);
}

s32 func_00351210(void) {
    *(u32 *)(gProgress + 0x84) |= 0x1000000;
    return 2;
}

s32 func_003512A0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00462E30, dest, 0x4000000, 0);
}

s32 func_003512D0(void) {
    *(u32 *)(gProgress + 0x84) |= 0x2000000;
    return 2;
}

s32 func_00351360(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00462E50, dest, 0x4000000, 0);
}

s32 func_00351520(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00462E70, dest, 0x4000000, 0);
}

s32 func_003516E0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00462E90, dest, 0x4000000, 0);
}

s32 func_00351840(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00462EB0, dest, 0x4000000, 0);
}

s32 func_003518F0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00462ED0, dest, 0x4000000, 0);
}

s32 func_00351B10(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00462EF0, dest, 0x4000000, 0);
}

s32 func_00351C90(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00462F10, dest, 0x4000000, 0);
}

s32 func_00351CC0(void) {
    *(u32 *)(gProgress + 0x84) |= 0x4000000;
    return 2;
}
