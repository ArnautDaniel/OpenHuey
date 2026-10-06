/* Leaf functions, batch 4 (func_002E7300..func_002E79B0). */
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

extern void *D_004193A0[];
extern u8 D_004193D0[];
extern u8 D_00419400[];
extern u8 D_00419530[];
extern u8 D_00419638[];
extern void *D_004196E0[];
extern u8 D_00419710[];
extern u8 D_00419730[];
extern u8 D_00419770[];
extern u8 D_00419870[];
extern u8 D_00419950[];
extern void *D_00419A30[];
extern u8 D_00419A60[];
extern u8 *gProgress;
extern u8 D_00419A80[];
extern u8 D_00419AA0[];
extern u8 D_00419AE0[];
extern u8 D_00419B70[];
extern void *D_00419D78[];
extern u8 D_00419DA8[];
extern void *D_0047AC88[];

void *func_002E7300(void *self, s32 i) { return D_004193A0[i]; }

void *func_002E74C0(void) { return D_004193D0; }

void *func_002E74D0(void) { return D_00419400; }

void *func_002E74E0(void) { return D_00419530; }

void *func_002E74F0(void) { return D_00419638; }

void *func_002E7500(void *self, s32 i) { return D_004196E0[i]; }

void *func_002E7520(void) { return D_00419710; }

void *func_002E7700(void) { return D_00419730; }

void *func_002E7710(void) { return D_00419770; }

void *func_002E7720(void) { return D_00419870; }

void *func_002E7730(void) { return D_00419950; }

void *func_002E7750(void *self, s32 i) { return D_00419A30[i]; }

void *func_002E7770(void) { return D_00419A60; }

s32 func_002E7880(void) {
    u8 *e = gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0x10A && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}

void *func_002E7940(void) { return D_00419A80; }

void *func_002E7950(void) { return D_00419AA0; }

void *func_002E7960(void) { return D_00419AE0; }

void *func_002E7970(void) { return D_00419B70; }

void *func_002E7980(void *self, s32 i) { return D_00419D78[i]; }

void *func_002E79A0(void) { return D_00419DA8; }

void *func_002E79B0(void *self, s32 i) { return D_0047AC88[i]; }

/* (as func_002E7880) last frame's noise requests (gProgress +0x10D4) of kind 0xD8 / 0xD7 and
   loudness 0x20 or more */
s32 func_0036FC90(void) {
    u8 *e = gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0xD8 && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}

s32 func_0036F8D0(void) {
    u8 *e = gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0xD7 && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}
