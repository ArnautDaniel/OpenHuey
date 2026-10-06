#include "common.h"
#include "ptmf.h"
#include "progress.h"

extern u32 D_0043EA90[];
extern u32 D_0043EAE0[];
extern u32 D_0043EBA0[];
extern u32 D_0043EC20[];
extern u32 D_0043EC80[];
extern u32 D_0043ECC0[];
extern u32 D_0043ED80[];
extern u32 D_0043EF00[];
extern u32 D_0043EF40[];
extern u32 D_0043F0B0[];
extern u32 D_0043F0C0[];
extern u32 D_0043F1A0[];
extern u32 D_0043F230[];
extern u32 D_0043F480[];
extern u32 D_0043F4D0[];
extern u32 D_0043F880[];
extern u32 D_0043F8D0[];
extern u32 D_0043F8F0[];
extern u32 D_0043F9F0[];
extern u32 D_00462870[];
extern u32 D_00462890[];
extern u32 D_0047AEF0[];
extern u32 D_0047AEF4[];
extern u32 D_0047AEF8[];
extern u32 D_0047AF00[];
extern u8 D_0043DC10[], D_0043DBD0[];
extern u8 D_0043DC30[], D_0043DBF0[];
extern u8 D_0043EA80[];
extern u8 D_00462670[], D_00462690[], D_004626B0[], D_004626D0[], D_004626F0[], D_00462710[], D_00462730[];
extern u8 D_00462770[], D_00462790[], D_004627B0[], D_004627D0[], D_004627F0[], D_00462810[], D_00462830[];

f32 func_003492A0(void) {
    return 5.0f;
}

f32 func_003492B0(void) {
    return 12.0f;
}

void *func_003495B0(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043DC30 : D_0043DBF0;
}

void *func_003495F0(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043DC10 : D_0043DBD0;
}

void *func_003498D0(void) {
    return D_00462870;
}

void *func_003498E0(void) {
    return D_00462890;
}

s32 func_003498F0(void) {
    return 0x41000;
}

void func_00349900(u8 *p, u32 b, f32 f) {
    *(f32 *)(p + 0xD1C) = f;
    p[0xD20] = (u8)b;
}

void func_00349910(u8 *p, f32 x, f32 y, f32 z) {
    *(f32 *)(p + 0xD90) = x;
    *(f32 *)(p + 0xD94) = y;
    *(f32 *)(p + 0xD98) = z;
}

void func_00349920(u8 *p) {
    p[0xCA] |= 2;
}

void func_00349930(u8 *p) {
    p[0xCA] &= ~2;
}

void *func_00349990(void *self, u32 i) {
    switch (i) {
    case 0: return D_00462670;
    case 1: return D_00462690;
    case 2: return D_004626B0;
    case 3: return D_004626D0;
    case 4: return D_004626F0;
    case 5: return D_00462710;
    case 6: return D_00462730;
    }
    return NULL;
}

void *func_00349A20(void *self, u32 i) {
    switch (i) {
    case 0: return D_00462770;
    case 1: return D_00462790;
    case 2: return D_004627B0;
    case 3: return D_004627D0;
    case 4: return D_004627F0;
    case 5: return D_00462810;
    case 6: return D_00462830;
    }
    return NULL;
}

void func_0034A120(u8 *p) {
    *(u16 *)(p + 0x840) = 0x10;
    *(void **)(p + 0x844) = D_0043EA80;
}

void *func_0034A1A0(void) {
    return D_0043EA90;
}

void *func_0034A1B0(void) {
    return D_0043EAE0;
}

void *func_0034A1C0(void) {
    return D_0043EBA0;
}

void *func_0034A1D0(void) {
    return D_0043EC20;
}

u32 func_0034A1E0(void *self, s32 i) {
    return D_0047AEF0[i];
}

u32 func_0034A200(void *self, s32 i) {
    return D_0047AEF4[i];
}

void *func_0034A3C0(void) {
    return D_0043EC80;
}

void *func_0034A3D0(void) {
    return D_0043ECC0;
}

void *func_0034A3E0(void) {
    return D_0043ED80;
}

void *func_0034A3F0(void) {
    return D_0043EF00;
}

void *func_0034A400(void) {
    return D_0043EF40;
}

u32 func_0034A410(void *self, s32 i) {
    return D_0047AEF8[i];
}

void *func_0034A430(void) {
    return D_0043F0B0;
}

u32 func_0034A440(void *self, s32 i) {
    return D_0047AF00[i];
}

void *func_0034A520(void) {
    return D_0043F0C0;
}

void *func_0034A530(void) {
    return D_0043F1A0;
}

void *func_0034A540(void) {
    return D_0043F230;
}

void *func_0034A550(void) {
    return D_0043F480;
}

void *func_0034A560(void) {
    return D_0043F4D0;
}

u32 func_0034A570(void *self, s32 i) {
    return D_0043F880[i];
}

u32 func_0034A5A0(void *self, s32 i) {
    return D_0043F8D0[i];
}

void *func_0034AA10(void) {
    return D_0043F8F0;
}

void *func_0034AA20(void) {
    return D_0043F9F0;
}
