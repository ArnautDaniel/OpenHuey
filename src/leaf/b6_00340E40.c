#include "common.h"
#include "ptmf.h"
#include "globals.h"

extern u32 D_004365C0[];
extern u32 D_00436600[];
extern u32 D_00436690[];
extern u32 D_00436780[];
extern u32 D_00436A00[];
extern u32 D_00436A40[];
extern u32 D_00436C80[];
extern u32 D_00436CE0[];
extern u32 D_00436CF0[];
extern u32 D_00436D70[];
extern u32 D_00436DE0[];
extern u32 D_004370F0[];
extern u32 D_00437140[];
extern u32 D_00437BF0[];
extern u32 D_00437D20[];
extern u32 D_00437D60[];
extern u32 D_00437D80[];
extern u32 D_00437DC0[];
extern u32 D_00437E40[];
extern u32 D_00437E50[];
extern u32 D_00437F50[];
extern u32 D_00437FD0[];
extern u32 D_00438230[];
extern u32 D_00438290[];
extern u32 D_004386D0[];
extern u32 D_00438700[];
extern u32 D_00438720[];
extern u32 D_00438730[];
extern u32 D_004387A0[];
extern u32 D_004388C0[];
extern u32 D_004389F0[];
extern u32 D_00438A70[];
extern u32 D_00438CC0[];
extern u32 D_00438CF8[];
extern u32 D_00438D10[];
extern u32 D_0047AE78[];
extern u8 D_00461CC0[];

u32 func_00340E40(void *self, s32 i) {
    return D_004365C0[i];
}

void *func_003415E0(void) {
    return D_00436600;
}

void *func_003415F0(void) {
    return D_00436690;
}

void *func_00341600(void) {
    return D_00436780;
}

void *func_00341610(void) {
    return D_00436A00;
}

void *func_00341620(void) {
    return D_00436A40;
}

u32 func_00341630(void *self, s32 i) {
    return D_00436C80[i];
}

void *func_00341650(void) {
    return D_00436CE0;
}

void *func_00341B70(void) {
    return D_00436CF0;
}

void *func_00341B80(void) {
    return D_00436D70;
}

void *func_00341B90(void) {
    return D_00436DE0;
}

void *func_00341BA0(void) {
    return D_004370F0;
}

void *func_00341BB0(void) {
    return D_00437140;
}

u32 func_00341BC0(void *self, s32 i) {
    return D_00437BF0[i];
}

u32 func_00341BE0(void *self, s32 i) {
    return D_00437D20[i];
}

void *func_00343790(void) {
    return D_00437D60;
}

void *func_003437A0(void) {
    return D_00437D80;
}

void *func_003437B0(void) {
    return D_00437DC0;
}

u32 func_003437D0(void *self, s32 i) {
    return D_0047AE78[i];
}

void *func_003437F0(void) {
    return D_00437E40;
}

void *func_00343920(void) {
    return D_00437E50;
}

void *func_00343930(void) {
    return D_00437F50;
}

void *func_00343940(void) {
    return D_00437FD0;
}

void *func_00343950(void) {
    return D_00438230;
}

void *func_00343970(void) {
    return D_00438290;
}

u32 func_00343980(void *self, s32 i) {
    return D_004386D0[i];
}

void *func_003439A0(void) {
    return D_00438720;
}

u32 func_003439B0(void *self, s32 i) {
    return D_00438700[i];
}

void *func_003440B0(void) {
    return D_00438730;
}

void *func_003440C0(void) {
    return D_004387A0;
}

void *func_003440D0(void) {
    return D_004388C0;
}

void *func_003440E0(void) {
    return D_004389F0;
}

void *func_003440F0(void) {
    return D_00438A70;
}

u32 func_00344100(void *self, s32 i) {
    return D_00438CC0[i];
}

void *func_00344120(void) {
    return D_00438D10;
}

u32 func_00344130(void *self, s32 i) {
    return D_00438CF8[i];
}

s32 func_003445A0(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, s32 (*)(void *, const void *, void *, u32, s32))(gFileLoader, D_00461CC0, dest, 0x4000000, 0);
}
