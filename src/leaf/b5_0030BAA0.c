#include "common.h"
#include "ptmf.h"
#include "progress.h"

/* Field access by byte offset into objects whose layout is not yet known. */
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define PTR(p, off) (*(void * *)((u8 *)(p) + (off)))

extern u8 D_00422440[];
extern u8 D_00422460[];
extern u8 D_00422480[];
extern u8 D_004224A0[];
extern u8 D_00423020[];
extern u8 D_00423070[];
extern u8 D_004238B0[];
extern u8 D_00423900[];
extern u8 D_00423950[];
extern u8 D_00423970[];
extern u8 D_00423990[];
extern u8 D_004239E0[];
extern u8 D_00423B30[];
extern u8 D_00423B70[];
extern u8 D_00424250[];
extern u8 D_00424490[];
extern u8 D_004246C0[];
extern u8 D_00424880[];
extern u8 D_00424890[];
extern u8 D_00424990[];
extern u8 D_00424AC0[];
extern u8 D_00424AF0[];
extern u8 D_00424B50[];
extern u8 D_00424B70[];
extern u8 D_00424C10[];
extern u8 D_00424D70[];
extern void *D_00425290[];
extern u8 D_004252C0[];
extern u8 D_004252D0[];
extern u8 D_00425460[];
extern u8 D_00425520[];
extern u8 D_004258D0[];
extern u8 D_004259B0[];
extern void *D_00426760[];
extern void *D_0044E560;
extern void *D_0047AD20[];
extern void *D_0047AD24[];
extern void *D_0047AD28[];
extern u8 D_019910C8[];
extern u8 D_019910D8[];

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

void func_0030BAA0(u8 *self, s32 alt) {
    if (b5_prog_flag8000()) {
        if (alt) {
            S32(self, 0x16B8) = 2;
            PTR(self, 0x1730) = D_00423900;
            PTR(self, 0x1748) = D_019910D8;
            PTR(self, 0x1740) = D_00423970;
            PTR(self, 0x173C) = D_004239E0;
        } else {
            S32(self, 0x16B8) = 0;
            PTR(self, 0x1730) = D_004238B0;
            PTR(self, 0x1748) = D_019910C8;
            PTR(self, 0x1740) = D_00423950;
            PTR(self, 0x173C) = D_00423990;
        }
    } else if (alt) {
        S32(self, 0x16B8) = 2;
        PTR(self, 0x1730) = D_00423070;
    } else {
        S32(self, 0x16B8) = 0;
        PTR(self, 0x1730) = D_00423020;
    }
}

f32 func_0030BC60(void) {
    return b5_prog_flag8000() ? 45.0f : 2e+01f;
}

f32 func_0030BCA0(void) {
    return b5_prog_flag8000() ? 8.0f : 5.0f;
}

f32 func_0030BCE0(void) {
    return b5_prog_flag8000() ? 3e+01f : 1e+01f;
}

f32 func_0030BD20(u8 *self) {
    if (b5_prog_flag8000()) {
        return S32(self, 0x16B8) == 2 ? 18.0f : 40.0f;
    }
    return S32(self, 0x16B8) == 2 ? 18.0f : 70.0f;
}

f32 func_0030BDA0(u8 *self) {
    if (b5_prog_flag8000()) {
        return S32(self, 0x16B8) == 2 ? 12.0f : 30.0f;
    }
    return S32(self, 0x16B8) == 2 ? 12.0f : 60.0f;
}

void *func_0030C1B0(void) {
    return b5_prog_flag8000() ? D_004224A0 : D_00422460;
}

void *func_0030C1F0(void) {
    return b5_prog_flag8000() ? D_00422480 : D_00422440;
}

void *func_0030C4F0(void) {
    return D_00423B30;
}

void *func_0030C500(void) {
    return D_00423B70;
}

void func_0030C510(u8 *self) {
    self[0x16EE] = 1;
}

f32 func_0030CBB0(void) {
    return 18.0f;
}

f32 func_0030CBC0(void) {
    return 14.0f;
}

s32 func_0030CF80(void *self) {
    return VCALL(D_0044E560, 0x10, s32 (*)(void *, s32, s32))(D_0044E560, 0, 0x400000);
}

void func_0030D2A0(u8 *self) {
    PTR(self, 0x874) = D_00424250;
}

void func_0030DC20(u8 *self) {
    PTR(self, 0x874) = D_00424490;
}

void func_0030E2E0(u8 *self) {
    PTR(self, 0x874) = D_004246C0;
}

void *func_0030EEF0(void) {
    return D_00424880;
}

void *func_0030EF00(void) {
    return D_00424890;
}

void *func_0030EF10(void) {
    return D_00424990;
}

void *func_0030EF20(void) {
    return D_00424AC0;
}

void *func_0030EF30(void) {
    return D_00424AF0;
}

void *func_0030EF40(void *self, s32 i) {
    return D_0047AD20[i];
}

void *func_0030EF60(void *self, s32 i) {
    return D_0047AD24[i];
}

void *func_0030EFE0(void) {
    return D_00424B50;
}

void *func_0030EFF0(void) {
    return D_00424B70;
}

void *func_0030F000(void) {
    return D_00424C10;
}

void *func_0030F010(void) {
    return D_00424D70;
}

void *func_0030F020(void *self, s32 i) {
    return D_00425290[i];
}

void *func_0030F040(void) {
    return D_004252C0;
}

void *func_0030F050(void *self, s32 i) {
    return D_0047AD28[i];
}

void *func_0030F0D0(void) {
    return D_004252D0;
}

void *func_0030F0E0(void) {
    return D_00425460;
}

void *func_0030F0F0(void) {
    return D_00425520;
}

void *func_0030F100(void) {
    return D_004258D0;
}

void *func_0030F110(void) {
    return D_004259B0;
}

void *func_0030F120(void *self, s32 i) {
    return D_00426760[i];
}
