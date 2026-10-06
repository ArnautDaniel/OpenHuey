/* Leaf functions (batch 3). Most are getters returning the address of a static table
 * (or one entry of it), likely per-class descriptor/state tables. */
#include "common.h"
#include "ptmf.h"
#include "globals.h"

extern u32 D_00410F10[];
extern u8 D_00410F50[];
extern u32 D_00410F38[];
extern u8 D_00410F70[];
extern u8 D_00410FB0[];
extern u8 D_00411180[];
extern u8 D_00411430[];
extern u8 D_004114E0[];
extern u32 D_00411B00[];
extern u8 D_00411B60[];
extern u32 D_0047ABE8[];
extern u8 D_00411B80[];
extern u8 D_00411C60[];
extern u8 D_00411D60[];
extern u8 D_00411FB0[];
extern u32 D_00412390[];
extern u8 D_004123F0[];
extern u32 D_004123D0[];

u32 func_002B5330(void *self, s32 i) {
    return D_00410F10[i];
}

void *func_002B5350(void) {
    return D_00410F50;
}

u32 func_002B5360(void *self, s32 i) {
    return D_00410F38[i];
}

void *func_002B5910(void) {
    return D_00410F70;
}

void *func_002B5920(void) {
    return D_00410FB0;
}

void *func_002B5930(void) {
    return D_00411180;
}

void *func_002B5940(void) {
    return D_00411430;
}

void *func_002B5950(void) {
    return D_004114E0;
}

u32 func_002B5960(void *self, s32 i) {
    return D_00411B00[i];
}

void *func_002B5980(void) {
    return D_00411B60;
}

u32 func_002B5990(void *self, s32 i) {
    return D_0047ABE8[i];
}

void *func_002B5EE0(void) {
    return D_00411B80;
}

void *func_002B5EF0(void) {
    return D_00411C60;
}

void *func_002B5F00(void) {
    return D_00411D60;
}

void *func_002B5F10(void) {
    return D_00411FB0;
}

u32 func_002B5F20(void *self, s32 i) {
    return D_00412390[i];
}

void *func_002B5F40(void) {
    return D_004123F0;
}

u32 func_002B5F50(void *self, s32 i) {
    return D_004123D0[i];
}

/* Stores its arguments, then tail-calls gRenderer->vfunc_0xC(self, c, 0). */
/* Merges two descriptors into the one at self->0x11C: words 4..C are ORed,
 * bytes 0x10..0x16, the word at 0x18 and bytes 0x1C..0x4B are copied from a. */
