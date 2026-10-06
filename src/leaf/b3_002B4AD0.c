/* Leaf functions (batch 3). Most are getters returning the address of a static table
 * (or one entry of it), likely per-class descriptor/state tables. */
#include "common.h"
#include "ptmf.h"
#include "globals.h"

extern u8 D_0040E3B0[];
extern u8 D_0040E480[];
extern u8 D_0040E550[];
extern u8 D_0040E680[];
extern u32 D_0040EC60[];
extern u8 D_0040ECF0[];
extern u32 D_0040ECA0[];
extern u8 D_0040ED10[];
extern u8 D_0040ED80[];
extern u8 D_0040EDC0[];
extern u8 D_0040EF30[];
extern u8 D_0040EFB0[];
extern u32 D_0040F430[];
extern u8 D_0040F478[];
extern u8 D_0040F490[];
extern u8 D_0040F4C0[];
extern u8 D_0040F540[];
extern u8 D_0040F5B0[];
extern u32 D_0040F8C0[];
extern u8 D_0040F8F8[];
extern u32 D_0040F8E8[];
extern u8 D_0040F910[];
extern u8 D_0040FA20[];
extern u8 D_0040FAB0[];
extern u8 D_0040FC70[];
extern u32 D_00410030[];
extern u8 D_00410050[];
extern u32 D_0047ABD8[];
extern u8 D_00410070[];
extern u8 D_004100B0[];
extern u8 D_00410110[];
extern u8 D_00410140[];
extern u32 D_00410B30[];
extern u8 D_00410BD0[];
extern u32 D_00410B90[];
extern u8 D_00410BE0[];
extern u8 D_00410D00[];
extern u8 D_00410D90[];
extern u8 D_00410DC0[];

void *func_002B4AD0(void) {
    return D_0040E3B0;
}

void *func_002B4AE0(void) {
    return D_0040E480;
}

void *func_002B4AF0(void) {
    return D_0040E550;
}

void *func_002B4B00(void) {
    return D_0040E680;
}

u32 func_002B4B10(void *self, s32 i) {
    return D_0040EC60[i];
}

void *func_002B4B30(void) {
    return D_0040ECF0;
}


/* 1 unless the object at +0x18 exists and its byte +0x28 is 1. */
s32 func_002B4B70(void) {
    u8 *p = *(u8 **)(gCreatures + 0x18);

    return !(p != NULL && p[0x28] == 1);
}

u32 func_002B4CB0(void *self, s32 i) {
    return D_0040ECA0[i];
}

void *func_002B4D30(void) {
    return D_0040ED10;
}

void *func_002B4D40(void) {
    return D_0040ED80;
}

void *func_002B4D50(void) {
    return D_0040EDC0;
}

void *func_002B4D60(void) {
    return D_0040EF30;
}

void *func_002B4D70(void) {
    return D_0040EFB0;
}

u32 func_002B4D80(void *self, s32 i) {
    return D_0040F430[i];
}

void *func_002B4DA0(void) {
    return D_0040F478;
}

void *func_002B4F50(void) {
    return D_0040F490;
}

void *func_002B4F60(void) {
    return D_0040F4C0;
}

void *func_002B4F70(void) {
    return D_0040F540;
}

void *func_002B4F80(void) {
    return D_0040F5B0;
}

u32 func_002B4FA0(void *self, s32 i) {
    return D_0040F8C0[i];
}

void *func_002B4FC0(void) {
    return D_0040F8F8;
}

u32 func_002B4FD0(void *self, s32 i) {
    return D_0040F8E8[i];
}

void *func_002B5050(void) {
    return D_0040F910;
}

void *func_002B5060(void) {
    return D_0040FA20;
}

void *func_002B5070(void) {
    return D_0040FAB0;
}

void *func_002B5090(void) {
    return D_0040FC70;
}

u32 func_002B50A0(void *self, s32 i) {
    return D_00410030[i];
}

void *func_002B50C0(void) {
    return D_00410050;
}

u32 func_002B50D0(void *self, s32 i) {
    return D_0047ABD8[i];
}

void *func_002B5150(void) {
    return D_00410070;
}

void *func_002B5160(void) {
    return D_004100B0;
}

void *func_002B5170(void) {
    return D_00410110;
}

void *func_002B5180(void) {
    return D_00410140;
}

u32 func_002B5190(void *self, s32 i) {
    return D_00410B30[i];
}

void *func_002B51B0(void) {
    return D_00410BD0;
}

u32 func_002B51C0(void *self, s32 i) {
    return D_00410B90[i];
}

void *func_002B52F0(void) {
    return D_00410BE0;
}

void *func_002B5300(void) {
    return D_00410D00;
}

void *func_002B5310(void) {
    return D_00410D90;
}

void *func_002B5320(void) {
    return D_00410DC0;
}
