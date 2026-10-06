/* Leaf functions (batch 3). Most are getters returning the address of a static table
 * (or one entry of it), likely per-class descriptor/state tables. */
#include "common.h"
#include "ptmf.h"

extern u8 D_00409950[];
extern u32 D_00409938[];
extern u8 D_00409980[];
extern u8 D_00409A80[];
extern u8 D_00409AD0[];
extern u8 D_0040A020[];
extern u32 D_0040AB80[];
extern u8 D_0040AC60[];
extern u32 D_0040AC10[];
extern u8 D_0040AE90[];
extern u8 D_0040AF40[];
extern u8 D_0040B040[];
extern u8 D_0040B160[];
extern u8 D_0040B200[];
extern u32 D_0040B4B0[];
extern u8 D_0040B510[];
extern u32 D_0040B500[];
extern u8 D_0040B520[];
extern u8 D_0040B5C0[];
extern u8 D_0040B680[];
extern u8 D_0040B8A0[];
extern u32 D_0040C0A0[];
extern u8 D_0040C170[];
extern u32 D_0040C140[];
extern u8 D_0040C190[];
extern u8 D_0040C270[];
extern u8 D_0040C2C0[];
extern u8 D_0040C420[];
extern u8 D_0040C4C0[];
extern u32 D_0040CEB0[];
extern u8 D_0040CF30[];
extern u32 D_0040CF10[];
extern u8 D_0040CF40[];
extern u8 D_0040D000[];
extern u8 D_0040D060[];
extern u8 D_0040D1F0[];
extern u32 D_0040E2E0[];
extern u8 D_0040E3A0[];
extern u32 D_0040E360[];

void *func_002B30D0(void) {
    return D_00409950;
}

u32 func_002B30E0(void *self, s32 i) {
    return D_00409938[i];
}

extern u8 *gCreatures;

/* 1 unless the object at +0x18 exists and its byte +0x28 is 1. */
s32 func_002B3130(void) {
    u8 *p = *(u8 **)(gCreatures + 0x18);

    return !(p != NULL && p[0x28] == 1);
}

void *func_002B35A0(void) {
    return D_00409980;
}

void *func_002B35B0(void) {
    return D_00409A80;
}

void *func_002B35C0(void) {
    return D_00409AD0;
}

void *func_002B35D0(void) {
    return D_0040A020;
}

u32 func_002B35E0(void *self, s32 i) {
    return D_0040AB80[i];
}

void *func_002B3600(void) {
    return D_0040AC60;
}

u32 func_002B3610(void *self, s32 i) {
    return D_0040AC10[i];
}

void *func_002B39D0(void) {
    return D_0040AE90;
}

void *func_002B39E0(void) {
    return D_0040AF40;
}

void *func_002B39F0(void) {
    return D_0040B040;
}

void *func_002B3A00(void) {
    return D_0040B160;
}

void *func_002B3A10(void) {
    return D_0040B200;
}

u32 func_002B3A20(void *self, s32 i) {
    return D_0040B4B0[i];
}

void *func_002B3A40(void) {
    return D_0040B510;
}

u32 func_002B3A50(void *self, s32 i) {
    return D_0040B500[i];
}

void *func_002B3F70(void) {
    return D_0040B520;
}

void *func_002B3F80(void) {
    return D_0040B5C0;
}

void *func_002B3F90(void) {
    return D_0040B680;
}

void *func_002B3FA0(void) {
    return D_0040B8A0;
}

u32 func_002B3FB0(void *self, s32 i) {
    return D_0040C0A0[i];
}

void *func_002B3FD0(void) {
    return D_0040C170;
}

u32 func_002B3FE0(void *self, s32 i) {
    return D_0040C140[i];
}

void *func_002B46F0(void) {
    return D_0040C190;
}

void *func_002B4700(void) {
    return D_0040C270;
}

void *func_002B4710(void) {
    return D_0040C2C0;
}

void *func_002B4720(void) {
    return D_0040C420;
}

void *func_002B4730(void) {
    return D_0040C4C0;
}

u32 func_002B4740(void *self, s32 i) {
    return D_0040CEB0[i];
}

void *func_002B4760(void) {
    return D_0040CF30;
}

u32 func_002B4770(void *self, s32 i) {
    return D_0040CF10[i];
}

void *func_002B48A0(void) {
    return D_0040CF40;
}

void *func_002B48B0(void) {
    return D_0040D000;
}

void *func_002B48C0(void) {
    return D_0040D060;
}

void *func_002B48D0(void) {
    return D_0040D1F0;
}

u32 func_002B48F0(void *self, s32 i) {
    return D_0040E2E0[i];
}

void *func_002B4910(void) {
    return D_0040E3A0;
}

u32 func_002B4920(void *self, s32 i) {
    return D_0040E360[i];
}
