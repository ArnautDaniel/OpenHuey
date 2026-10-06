#include "common.h"
extern u8 *gCharPlayer; /* global manager object */
#include "ptmf.h"
#include "progress.h"

/* Field access by byte offset into objects whose layout is not yet known. */
#define S16(p, off) (*(s16 *)((u8 *)(p) + (off)))
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define S64(p, off) (*(s64 *)((u8 *)(p) + (off)))
#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define PTR(p, off) (*(void * *)((u8 *)(p) + (off)))
/* writes {x, 0, z} */
#define B5_SET3(out, x, z) ((out)[0] = (x), (out)[1] = 0.0f, (out)[2] = (z))
/* gFileLoader vtable +0xC: start loading file `name` into `dest` (flags 0x4000000) */
#define FILE_LOAD_ASYNC(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, void *, u32, s32))(gFileLoader, name, dest, 0x4000000, 0)

extern u8 D_00429C90[];
extern u8 D_00429CE0[];
extern u8 D_00429D20[];
extern u8 D_00429D70[];
extern u8 D_00429DB0[];
extern u8 D_00429E40[];
extern u8 D_00429E60[];
extern u8 D_00429F10[];
extern void *D_0042A0A0[];
extern void *D_0042A0E0[];
extern u8 D_0042A0F0[];
extern u8 D_0042A130[];
extern u8 D_0042A2C0[];
extern u8 D_0042A2E0[];
extern u8 D_0042A300[];
extern u8 D_0042A320[];
extern u8 D_0042A340[];
extern u8 D_0042AED0[];
extern u8 D_0042B0B0[];
extern u8 D_0042B160[];
extern void *gTexCache;
extern u8 D_0045FE20[];
extern u8 D_0045FE40[];
extern u8 D_0045FE60[];
extern u8 D_0045FE80[];
extern u8 D_0045FEA0[];
extern void *gFileLoader;

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

void *func_00315C50(void) {
    return D_00429C90;
}

void func_003168E0(u8 *self) {
    S32(self, 0x134) = 0;
    self[0x138] = 0;
}

void *func_00316BC0(void) {
    return D_00429CE0;
}

void *func_00316BD0(void) {
    return D_00429D20;
}

s32 func_0031CB10(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_0045FE20, dest);
}

s32 func_0031CDE0(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_0045FE40, dest);
}

s32 func_0031D020(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_0045FE60, dest);
}

s32 func_0031D180(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_0045FE80, dest);
}

s32 func_0031D600(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_0045FEA0, dest);
}

void *func_0031D800(void) {
    return D_00429D70;
}

void *func_0031D810(void) {
    return D_00429DB0;
}

void func_0031DDF0(u8 *self, u32 id) {
    self[0x11040] = (u8)id;
    S32(self, 0x11044) = 30;
}

/* (possibly dead code: nothing in the game references it) */
s32 func_0031E0B0(void *self) {
    return VCALL(gTexCache, 0x14, s32 (*)(void *, s32))(gTexCache, 0x2D);
}

/* Count down the timer set by func_0031DDF0; id 0xFF = none. */
void func_0031E0D0(u8 *self) {
    if (self[0x11040] != 0xFF) {
        S32(self, 0x11044) -= 1;
        if (S32(self, 0x11044) == 0) {
            self[0x11040] = 0xFF;
        }
    }
}

s32 func_0031E130(u8 *self) {
    return VCALL(gTexCache, 0x10, s32 (*)(void *, void *, s32))(gTexCache, self + 0x40, 0x2D);
}

void *func_0031E220(void) {
    return D_00429E40;
}

void *func_0031E230(void) {
    return D_00429E60;
}

void *func_0031E240(void) {
    return D_00429F10;
}

void *func_0031E250(void *self, s32 i) {
    return D_0042A0A0[i];
}

void *func_0031E270(void *self, s32 i) {
    return D_0042A0E0[i];
}

void *func_0031E6D0(void) {
    return D_0042A0F0;
}

void *func_0031E6E0(void) {
    return D_0042A130;
}

void func_0031E920(u8 *self, u8 *src) {
    U32(self, 0x10) = U32(src, 0x0);
    U32(self, 0x14) = U32(src, 0x4);
    U32(self, 0x18) = U32(src, 0x8);
    U32(self, 0x1C) = U32(src, 0xC);
    F32(self, 0x20) = F32(src, 0x10);
    F32(self, 0x24) = F32(src, 0x14);
    F32(self, 0x28) = F32(src, 0x18);
    F32(self, 0x2C) = 1.0f;
    F32(self, 0x30) = F32(src, 0x1C);
    F32(self, 0x34) = F32(src, 0x1C);
    S32(self, 0x38) = 0;
    S32(self, 0x3C) = 0;
}

void func_0031E9A0(u8 *self) {
    S64(self, 0x48) = -1;
    S32(self, 0x58) = 0;
    S32(self, 0x5C) = 0;
    S32(self, 0x60) = 25;
    S16(self, 0x64) = 1;
    S16(self, 0x66) = 0xA0;
    S16(self, 0x68) = 0x40;
    S16(self, 0x6A) = 0x20;
    S16(self, 0x6C) = 0x20;
    S16(self, 0x6E) = 0x200;
    S16(self, 0x70) = 0x100;
    self[0x72] = 0x40;
    self[0x73] = 1;
    self[0x74] = 1;
    self[0x75] = 0x10;
    self[0x76] = 0xFF;
}

void *func_0031F220(void) {
    return D_0042A340;
}

void func_0031F290(void *self, s32 id, f32 *out) {
    switch (id) {
    case 1:
        B5_SET3(out, 0.0f, 0x1.e49ba60000000p+2f /* 7.572 */);
        break;
    case 3:
        B5_SET3(out, 0.0f, -0x1.b8e21a0000000p+2f /* 6.8888 */);
        break;
    case 0:
        B5_SET3(out, 0.0f, -0x1.5412060000000p+2f /* 5.3136 */);
        break;
    case 2:
        B5_SET3(out, 0.0f, 0x1.fbfb160000000p+2f /* 7.9372 */);
        break;
    }
}

void func_0031F330(void *self, s32 id, f32 *out) {
    switch (id) {
    case 10:
    case 11:
        B5_SET3(out, -0x1.3eab360000000p-5f /* 0.0389 */, 0x1.4cf4f00000000p+3f /* 10.4049 */);
        break;
    case 12:
    case 13:
        B5_SET3(out, 0x1.7652be0000000p-1f /* 0.7311 */, 0x1.a808320000000p+3f /* 13.251 */);
        break;
    case 14:
        B5_SET3(out, -0x1.25a8580000000p+0f /* 1.1471 */, -0x1.42a64c0000000p+1f /* 2.5207 */);
        break;
    case 15:
        B5_SET3(out, -0x1.4fdf3c0000000p-2f /* 0.328 */, -0x1.324a8c0000000p+1f /* 2.3929 */);
        break;
    }
}

f32 func_0031FC10(void) {
    return 1e+01f;
}

f32 func_0031FC20(void) {
    return 3e+01f;
}

f32 func_0031FC30(void) {
    return 12.0f;
}

s32 func_0031FC40(u8 *self) {
    f32 d;

    if (S32(self, 0xC4) == 1) {
        return 0x203;
    }
    if (S32(self, 0x16B8) == 2) {
        return 0x205;
    }
    if (self[0x1544] != 0 && PTR(self, 0x1540) == gCharPlayer) {
        d = F32(self, 0x1588);
        if (d < 100.0f && !(d <= 0.0f)) {
            return 0x206;
        }
    }
    return 0x201;
}

void *func_00320150(void) {
    return b5_prog_flag8000() ? D_0042A320 : D_0042A2E0;
}

void *func_00320190(void) {
    return b5_prog_flag8000() ? D_0042A300 : D_0042A2C0;
}

void func_00320500(u8 *self) {
    self[0xB6] |= 2;
}

void func_00320520(u8 *self) {
    PTR(self, 0x874) = D_0042AED0;
}

void *func_00320EB0(void) {
    return D_0042B0B0;
}

void *func_00320EC0(void) {
    return D_0042B160;
}
