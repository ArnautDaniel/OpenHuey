/* Leaf functions, batch 4 (func_002D1360..func_002DCA50). */
#include "common.h"
#include "ptmf.h"

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

/* float from its bit pattern */
static inline f32 B4_FLT(u32 bits) {
    union { u32 u; f32 f; } c;
    c.u = bits;
    return c.f;
}

/* gFileLoader->vfunc_0xC(name, dest, 0x4000000, 0): start loading a file */
#define LOAD(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, const char *, void *, s32, s32))(gFileLoader, name, dest, 0x4000000, 0)

extern u8 D_00469D00[], D_0046C770[];
extern u8 D_0046A9B0[];
extern void *gCreatures;
extern u8 D_0046FC00[], D_004699E0[], D_004699C0[];
extern void *gPlacedThings;
extern u8 D_0046F5C0[], D_004699E0[], D_004699C0[];
extern void *gBootMessage;
extern u8 D_0046D7D0[];
extern u8 D_004699E0[], D_0046A1C0[];
extern u8 *gProgress;
extern u8 D_00414390[];
extern u8 D_004143B0[];
extern u8 D_00414430[];
extern void *D_0047AC18[];
extern void *D_0047AC20[];
extern void *gFileLoader;
extern char D_0045D890[];
extern u8 *D_0045D1F0;
extern char D_0045D9E0[];
extern char D_0045DA00[];
extern u8 D_00414840[];
extern s32 gCharPlayer;
extern u8 D_00414820[], D_004147E0[];
extern u8 D_00414800[], D_004147C0[];

void *func_002D1360(u8 *p) {
    F(p, 0x0, void *) = D_00469D00;
    F(p, 0x4, s32) = -1;
    F(p, 0x0, void *) = D_0046C770;
    F(p, 0x7C, u32) = 0;
    F(p, 0x80, u32) = 0;
    F(p, 0x64, s32) = -1;
    p[0x84] = 0;
    p[0x60] = 0;
    F(p, 0x68, u32) = 0;
    p[0x85] = 0;
    return p;
}

void *func_002D1470(u8 *p) {
    F(p, 0x4C, void *) = D_0046A9B0;
    return p;
}

void *func_002D1490(u8 *p) {
    u8 *a = p + 0xDC40;
    u8 *b = p + 0xF630;

    gCreatures = p;
    F(p, 0x28, void *) = D_0046FC00;
    F(a, 0x0, void *) = D_004699E0;
    F(a, 0x4, u32) = 0;
    F(a, 0x8, u32) = 0;
    F(a, 0x0, void *) = D_004699C0;
    F(a, 0xC, u32) = 0;
    F(a, 0x10, u32) = 0;
    F(a, 0x14, u32) = 0;
    F(b, 0x0, void *) = D_004699E0;
    F(b, 0x4, u32) = 0;
    F(b, 0x8, u32) = 0;
    F(b, 0x0, void *) = D_004699C0;
    F(b, 0xC, u32) = 0;
    F(b, 0x10, u32) = 0;
    F(b, 0x14, u32) = 0;
    return p;
}

void *func_002D1510(u8 *p) {
    u8 *a = p + 0xA040;

    gPlacedThings = p;
    F(p, 0x0, void *) = D_0046F5C0;
    F(a, 0x0, void *) = D_004699E0;
    F(a, 0x4, u32) = 0;
    F(a, 0x8, u32) = 0;
    F(a, 0x0, void *) = D_004699C0;
    F(a, 0xC, u32) = 0;
    F(a, 0x10, u32) = 0;
    F(a, 0x14, u32) = 0;
    return p;
}

void *func_002D1560(u8 *p) {
    gBootMessage = p;
    F(p, 0x0, void *) = D_0046D7D0;
    return p;
}

void *func_002D1580(u8 *p) {
    F(p, 0x0, void *) = D_004699E0;
    F(p, 0x4, u32) = 0;
    F(p, 0x8, u32) = 0;
    F(p, 0x0, void *) = D_0046A1C0;
    F(p, 0xC, u32) = 0;
    F(p, 0x10, u32) = 0;
    return p;
}

/* Progress constructor: registers the global instance. */
void *func_002D15C0(u8 *p) {
    gProgress = p;
    return p;
}

void *func_002D2560(void) { return D_00414390; }

void *func_002D2570(void) { return D_004143B0; }

void *func_002D2580(void) { return D_00414430; }

void *func_002D25A0(void *self, s32 i) { return D_0047AC18[i]; }

void *func_002D25C0(void *self, s32 i) { return D_0047AC20[i]; }

s32 func_002D3A20(void *self, void *dest) { return LOAD(D_0045D890, dest); }

void func_002D3A60(u8 *p, const u8 *src) {
    u32 i;

    for (i = 0; i < 8; i++) {
        p[0x10 + i] = src[i];
    }
}

/* Starts a 16.16 fade of channel i from `from` to `to` over `frames` (12 bits) frames. */
/* The original null-checks the address of each member (inlined constructors). */
void func_002D4680(void *self, u8 *p) {
    u32 a = (u32)p;
    s32 i;

    p[0] = 0;
    if (a + 0x4 != 0) F(p, 0x4, u16) = 0;
    if (a + 0x8 != 0) F(p, 0x8, u16) = 0;
    if (a + 0xC != 0) F(p, 0xC, u16) = 0;
    if (a + 0x14 != 0) F(p, 0x14, u16) = 0;
    if (a + 0x18 != 0) F(p, 0x18, u16) = 0;
    if (a + 0x1C != 0) F(p, 0x1C, u16) = 0;
    if (a + 0x20 != 0) F(p, 0x20, u16) = 0;
    if (a + 0x24 != 0) {
        for (i = 0; i < 16; i++) {
            p[0x24 + i] = 0;
        }
    }
    F(p, 0x50, u32) = 0;
    F(p, 0x40, u32) = 0;
    F(p, 0x54, u32) = 0;
    F(p, 0x44, u32) = 0;
    F(p, 0x58, u32) = 0;
    F(p, 0x48, u32) = 0;
    F(p, 0x5C, u32) = 0;
    F(p, 0x4C, u32) = 0;
}

void func_002D6000(u8 *p, u32 v) { p[0x19034] = (u8)v; }

u32 func_002D6010(u8 *p) { return p[0x19034]; }

/* tail call: member at +0xA040, virtual slot 0x10 */
s32 func_002D7430(u8 *p, s32 a1, s32 a2, s32 a3) {
    u8 *m = p + 0xA040;
    return VCALL(m, 0x10, s32 (*)(void *, s32, s32, s32))(m, a1, a2, a3);
}

void func_002D7710(u8 *p, const u8 *src) { F(p, 0x10, u32) = *src; }

void func_002D78F0(u8 *p) {
    *D_0045D1F0 = 0;
    F(p, 0x10, u32) = 0;
}

s32 func_002D7980(void *self, void *dest) { return LOAD(D_0045D9E0, dest); }

s32 func_002D7A30(void *self, void *dest) { return LOAD(D_0045DA00, dest); }

void *func_002D7B80(void) { return D_00414840; }

void func_002D7B90(void *self, s32 id, u32 *out) {
    u32 z;

    switch (id) {
    case 1: z = 0x40F24DD3; break;
    case 3: z = 0xC0DC710D; break;
    case 0: z = 0xC0AA0903; break;
    case 2: z = 0x40FDFD8B; break;
    default: return;
    }
    out[0] = 0;
    out[1] = 0;
    out[2] = z;
}

void func_002D7C30(void *self, s32 id, u32 *out) {
    u32 x, z;

    switch (id) {
    case 10: case 11: x = 0xBD1F559B; z = 0x41267A78; break;
    case 12: case 13: x = 0x3F3B295F; z = 0x41540419; break;
    case 14: x = 0xBF92D42C; z = 0xC0215326; break;
    case 15: x = 0xBEA7EF9E; z = 0xC0192546; break;
    default: return;
    }
    out[0] = x;
    out[1] = 0;
    out[2] = z;
}

f32 func_002DB970(void) { return 20.0f; }

f32 func_002DB980(void) { return 12.0f; }

s32 func_002DBA10(u8 *p) {
    f32 d;

    if (F(p, 0xC4, s32) == 1) {
        return 0x203;
    }
    if (p[0x1544] != 0 && F(p, 0x1540, s32) == gCharPlayer) {
        d = F(p, 0x1588, f32);
        if (d < 100.0f && !(d <= 0.0f)) {
            return 0x206;
        }
    }
    return 0x201;
}

void *func_002DC460(void) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? D_00414820 : D_004147E0;
}

void *func_002DC4A0(void) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? D_00414800 : D_004147C0;
}

void func_002DCA50(void *self, u32 *out) {
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
}
