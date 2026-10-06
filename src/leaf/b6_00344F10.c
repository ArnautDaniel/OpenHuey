#include "common.h"
#include "ptmf.h"
#include "progress.h"

extern u32 D_0043ACB0[];
extern u32 D_0043AFD0[];
extern u32 D_0043B010[];
extern u32 D_0043B020[];
extern u32 D_0043B0D0[];
extern u32 D_0043B1D0[];
extern u32 D_0043B2C0[];
extern u32 D_0043B390[];
extern u32 D_0043B560[];
extern u32 D_0043B588[];
extern u32 D_0043B5A0[];
extern u32 D_0043B5B0[];
extern u32 D_0043B5F0[];
extern u32 D_0043B6E0[];
extern u32 D_0043CDC0[];
extern u32 D_0043DC50[];
extern u32 D_0047AEC0[];
extern u8 D_0043B6A0[], D_0043B660[];
extern u8 D_0043B6C0[], D_0043B680[];
extern u8 D_0043C260[], D_0043C348[], D_0043C2D0[], D_0043C210[], D_0043C338[], D_0043C2B0[];
extern u8 D_0043CC20[], D_0043CD08[], D_0043CC90[], D_0043CBD0[], D_0043CCF8[], D_0043CC70[];
extern u8 D_0043CD80[], D_0043CD40[];
extern u8 D_0043CDA0[], D_0043CD60[];

void *func_00344F10(void) {
    return D_0043ACB0;
}

u32 func_00344F30(void *self, s32 i) {
    return D_0043AFD0[i];
}

void *func_00344F50(void) {
    return D_0043B010;
}

u32 func_00344F60(void *self, s32 i) {
    return D_0047AEC0[i];
}

void *func_00345060(void) {
    return D_0043B020;
}

void *func_00345070(void) {
    return D_0043B0D0;
}

void *func_00345080(void) {
    return D_0043B1D0;
}

void *func_00345090(void) {
    return D_0043B2C0;
}

void *func_003450A0(void) {
    return D_0043B390;
}

u32 func_003450B0(void *self, s32 i) {
    return D_0043B560[i];
}

void *func_003450D0(void) {
    return D_0043B5A0;
}

u32 func_003450E0(void *self, s32 i) {
    return D_0043B588[i];
}

void *func_00345210(void) {
    return D_0043B5B0;
}

void *func_00345220(void) {
    return D_0043B5F0;
}

void func_00345E20(u8 *p) {
    s32 i;

    *(s32 *)(p + 0x1BB0) = 0;
    p[0x1BBC] = 0;
    *(f32 *)(p + 0x1BB4) = 1.0f;
    *(s64 *)(p + 0x1848) = -1;
    *(s32 *)(p + 0x1854) = 0;
    *(s32 *)(p + 0x1858) = 0;
    *(s32 *)(p + 0x185C) = 0;
    *(s32 *)(p + 0x1860) = 0x19;
    *(u16 *)(p + 0x1864) = 0x40;
    *(u16 *)(p + 0x1866) = 0x180;
    *(u16 *)(p + 0x1868) = 0x80;
    *(u16 *)(p + 0x186A) = 0x20;
    *(u16 *)(p + 0x186C) = 0x20;
    *(u16 *)(p + 0x186E) = 0x200;
    *(u16 *)(p + 0x1870) = 0x100;
    p[0x1872] = 0x40;
    p[0x1873] = 1;
    p[0x1874] = 1;
    p[0x1875] = 0x10;
    p[0x1876] = 6;
    for (i = 0; i < 64; i++) {
        ((f32 *)(p + 0x1AB0))[i] = 1.0f;
    }
}

void *func_00346000(void) {
    return D_0043B6E0;
}

/* Writes a position {0, 0, z} for index 0..3. */
void func_00346050(void *self, s32 i, f32 *out) {
    switch (i) {
    case 1: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.be824p+2f /* 6.9767 */; break;
    case 3: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.905f06p+2f /* -6.2558 */; break;
    case 0: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.bdc432p+2f /* -6.9651 */; break;
    case 2: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.ce0418p+2f /* 7.219 */; break;
    }
}

/* Writes a position {x, 0, z} for index 10..15. */
void func_003460F0(void *self, s32 i, f32 *out) {
    switch (i) {
    case 10: case 11: out[0] = 0x1.07c84cp-2f /* 0.2576 */; out[1] = 0.0f; out[2] = 0x1.567fccp+3f /* 10.7031 */; break;
    case 12: case 13: out[0] = 0x1.a4a8c2p+0f /* 1.6432 */; out[1] = 0.0f; out[2] = 0x1.5d182ap+3f /* 10.9092 */; break;
    case 14: out[0] = -0x1.5f06f6p-3f /* -0.1714 */; out[1] = 0.0f; out[2] = -0x1.8f6fd2p+1f /* -3.1206 */; break;
    case 15: out[0] = 0x1.9a0276p-2f /* 0.4004 */; out[1] = 0.0f; out[2] = -0x1.792d78p+1f /* -2.9467 */; break;
    }
}

/* Picks one of four table sets depending on story flag 0x8000 (gProgress+0x30) and `alt`. */
void func_00346E10(u8 *p, s32 alt) {
    if (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) {
        if (alt) {
            *(s32 *)(p + 0x16B8) = 2;
            *(void **)(p + 0x1730) = D_0043CC20;
            *(void **)(p + 0x1748) = D_0043CD08;
            *(void **)(p + 0x1740) = D_0043CC90;
        } else {
            *(s32 *)(p + 0x16B8) = 0;
            *(void **)(p + 0x1730) = D_0043CBD0;
            *(void **)(p + 0x1748) = D_0043CCF8;
            *(void **)(p + 0x1740) = D_0043CC70;
        }
    } else {
        if (alt) {
            *(s32 *)(p + 0x16B8) = 2;
            *(void **)(p + 0x1730) = D_0043C260;
            *(void **)(p + 0x1748) = D_0043C348;
            *(void **)(p + 0x1740) = D_0043C2D0;
        } else {
            *(s32 *)(p + 0x16B8) = 0;
            *(void **)(p + 0x1730) = D_0043C210;
            *(void **)(p + 0x1748) = D_0043C338;
            *(void **)(p + 0x1740) = D_0043C2B0;
        }
    }
}

f32 func_00346F20(void) {
    return 15.0f;
}

f32 func_00346F30(void) {
    return 5.0f;
}

f32 func_00346F40(void) {
    return 12.0f;
}

void *func_00347290(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043B6C0 : D_0043B680;
}

void *func_003472D0(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043B6A0 : D_0043B660;
}

/* Advances three angles (+0x4 by 0.5 deg, +0x8 by 0.1 deg, +0xC by -0.1 deg), wrapped to [-pi, pi]. */
s32 func_003476A0(f32 *a) {
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

void *func_003479E0(void) {
    return D_0043CDC0;
}

void func_00347A30(void *self, s32 i, f32 *out) {
    switch (i) {
    case 1: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.be824p+2f /* 6.9767 */; break;
    case 3: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.905f06p+2f /* -6.2558 */; break;
    case 0: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.bdc432p+2f /* -6.9651 */; break;
    case 2: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.ce0418p+2f /* 7.219 */; break;
    }
}

void func_00347AD0(void *self, s32 i, f32 *out) {
    switch (i) {
    case 10: case 11: out[0] = 0x1.07c84cp-2f /* 0.2576 */; out[1] = 0.0f; out[2] = 0x1.567fccp+3f /* 10.7031 */; break;
    case 12: case 13: out[0] = 0x1.a4a8c2p+0f /* 1.6432 */; out[1] = 0.0f; out[2] = 0x1.5d182ap+3f /* 10.9092 */; break;
    case 14: out[0] = -0x1.5f06f6p-3f /* -0.1714 */; out[1] = 0.0f; out[2] = -0x1.8f6fd2p+1f /* -3.1206 */; break;
    case 15: out[0] = 0x1.9a0276p-2f /* 0.4004 */; out[1] = 0.0f; out[2] = -0x1.792d78p+1f /* -2.9467 */; break;
    }
}

f32 func_00348300(void) {
    return 15.0f;
}

f32 func_00348310(void) {
    return 5.0f;
}

f32 func_00348320(void) {
    return 12.0f;
}

void *func_00348620(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043CDA0 : D_0043CD60;
}

void *func_00348660(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043CD80 : D_0043CD40;
}

void *func_00348970(void) {
    return D_0043DC50;
}

void func_003489C0(void *self, s32 i, f32 *out) {
    switch (i) {
    case 1: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.be824p+2f /* 6.9767 */; break;
    case 3: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.905f06p+2f /* -6.2558 */; break;
    case 0: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.bdc432p+2f /* -6.9651 */; break;
    case 2: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.ce0418p+2f /* 7.219 */; break;
    }
}

void func_00348A60(void *self, s32 i, f32 *out) {
    switch (i) {
    case 10: case 11: out[0] = 0x1.07c84cp-2f /* 0.2576 */; out[1] = 0.0f; out[2] = 0x1.567fccp+3f /* 10.7031 */; break;
    case 12: case 13: out[0] = 0x1.a4a8c2p+0f /* 1.6432 */; out[1] = 0.0f; out[2] = 0x1.5d182ap+3f /* 10.9092 */; break;
    case 14: out[0] = -0x1.5f06f6p-3f /* -0.1714 */; out[1] = 0.0f; out[2] = -0x1.8f6fd2p+1f /* -3.1206 */; break;
    case 15: out[0] = 0x1.9a0276p-2f /* 0.4004 */; out[1] = 0.0f; out[2] = -0x1.792d78p+1f /* -2.9467 */; break;
    }
}

f32 func_00349290(void) {
    return 15.0f;
}
