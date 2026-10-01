/* Leaf functions, batch 4 (func_002E6050..func_002E72F0). */
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

extern void *D_00417B90[];
extern u8 D_00417BC0[];
extern void *D_00417BB0[];
extern u8 D_00417BD0[];
extern u8 D_00417CE0[];
extern u8 D_00417DA0[];
extern void *D_00417F10[];
extern void *D_00417F30[];
extern u8 D_00417F40[];
extern u8 D_00417F50[];
extern u8 D_00418060[];
extern u8 D_00418120[];
extern void *D_00418290[];
extern u8 D_004182C0[];
extern void *D_004182B0[];
extern u8 D_004182D0[];
extern u8 D_00418430[];
extern u8 D_004184C0[];
extern u8 D_00418730[];
extern void *D_00418AA0[];
extern u8 D_00418AD0[];
extern void *D_0047AC70[];
extern u8 D_00418AF0[];
extern u8 D_00418B60[];
extern u8 D_00418BA0[];
extern u8 D_00418BF0[];
extern void *D_00418D70[];
extern u8 D_00418DB0[];
extern u8 *gProgress;
extern u8 D_00418DC0[];
extern u8 D_00418E40[];
extern u8 D_00418F40[];
extern u8 D_00419050[];
extern void *D_00419370[];
extern u8 D_004190F0[];
extern u8 D_004193B0[];

void *func_002E6050(void *self, s32 i) { return D_00417B90[i]; }

void *func_002E6070(void) { return D_00417BC0; }

void *func_002E6080(void *self, s32 i) { return D_00417BB0[i]; }

void *func_002E6490(void) { return D_00417BD0; }

void *func_002E64A0(void) { return D_00417CE0; }

void *func_002E64B0(void) { return D_00417DA0; }

void *func_002E64C0(void *self, s32 i) { return D_00417F10[i]; }

void *func_002E64F0(void *self, s32 i) { return D_00417F30[i]; }

void *func_002E68F0(void) { return D_00417F40; }

void *func_002E6900(void) { return D_00417F50; }

void *func_002E6910(void) { return D_00418060; }

void *func_002E6920(void) { return D_00418120; }

void *func_002E6930(void *self, s32 i) { return D_00418290[i]; }

void *func_002E6950(void) { return D_004182C0; }

void *func_002E6960(void *self, s32 i) { return D_004182B0[i]; }

void *func_002E6D60(void) { return D_004182D0; }

void *func_002E6D70(void) { return D_00418430; }

void *func_002E6D80(void) { return D_004184C0; }

void *func_002E6D90(void) { return D_00418730; }

void *func_002E6DA0(void *self, s32 i) { return D_00418AA0[i]; }

void *func_002E6DC0(void) { return D_00418AD0; }

void *func_002E6DD0(void *self, s32 i) { return D_0047AC70[i]; }

void *func_002E6F70(void) { return D_00418AF0; }

void *func_002E6F80(void) { return D_00418B60; }

void *func_002E6F90(void) { return D_00418BA0; }

void *func_002E6FA0(void) { return D_00418BF0; }

void *func_002E6FC0(void *self, s32 i) { return D_00418D70[i]; }

void *func_002E6FE0(void) { return D_00418DB0; }

s32 func_002E71C0(void) {
    u8 *e = gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0x107 && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}

void *func_002E7280(void) { return D_00418DC0; }

void *func_002E7290(void) { return D_00418E40; }

void *func_002E72A0(void) { return D_00418F40; }

void *func_002E72B0(void) { return D_00419050; }

void *func_002E72C0(void *self, s32 i) { return D_00419370[i]; }

void *func_002E72E0(void) { return D_004190F0; }

void *func_002E72F0(void) { return D_004193B0; }
