#include "common.h"
#include "ptmf.h"
#include "progress.h"
#include "globals.h"
#include "actor.h"

#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

extern u8 D_0042B200[];
extern u8 D_0042B520[];
extern void *D_0042C200[];
extern void *D_0042C2F0[];
extern u8 D_0042C360[];
extern u8 D_0042C380[];
extern u8 D_0042C3C0[];
extern u8 D_0042C770[];
extern u8 D_0042C790[];
extern u8 D_0042C810[];
extern u8 D_0042C860[];
extern u8 D_0042C900[];
extern u8 D_0042C940[];
extern void *D_0047ADB4[];
extern void *D_0047ADB8[];

void *func_00320ED0(void) {
    return D_0042B200;
}

void *func_00320EE0(void) {
    return D_0042B520;
}

void *func_00320EF0(void *self, s32 i) {
    return D_0042C200[i];
}

void *func_00320F10(void) {
    return D_0042C360;
}

void *func_00320F20(void *self, s32 i) {
    return D_0042C2F0[i];
}

void *func_00321750(void) {
    return D_0042C380;
}

void *func_00321760(void) {
    return D_0042C3C0;
}

void *func_0032C6F0(void) {
    return D_0042C770;
}

void *func_0032C700(void) {
    return D_0042C790;
}

void *func_0032C710(void) {
    return D_0042C810;
}

void *func_0032C720(void *self, s32 i) {
    return D_0047ADB4[i];
}

void *func_0032C740(void) {
    return D_0042C860;
}

void *func_0032C750(void *self, s32 i) {
    return D_0047ADB8[i];
}

void *func_0032CAD0(void) {
    return D_0042C900;
}

void *func_0032CAE0(void) {
    return D_0042C940;
}

void func_0032D270(u8 *self, s32 a, f32 x, f32 y) {
    S32(self, 0x1624) = 0;
    S32(self, 0x1628) = 1;
    S32(self, 0x162C) = 60;
    F32(self, 0x1634) = F32(self, 0x10);
    F32(self, 0x1638) = F32(self, 0x18);
    F32(self, 0x163C) = x;
    F32(self, 0x1640) = y;
    self[0x16A8] = 1;
    self[0x16A9] = 1;
    S32(self, 0x1630) = a;
}

void func_0032D3E0(u8 *self, s32 a, f32 x, f32 y) {
    S32(self, 0x1624) = 0;
    S32(self, 0x1628) = 1;
    S32(self, 0x162C) = 60;
    F32(self, 0x1634) = F32(self, 0x10);
    F32(self, 0x1638) = F32(self, 0x18);
    F32(self, 0x163C) = x;
    F32(self, 0x1640) = y;
    self[0x16A8] = 1;
    self[0x16A9] = 1;
    S32(self, 0x1630) = a;
}

/* ---- character 0x1A (an animal in the garden): +0x1624 state, +0x1628 just entered it,
 * +0x162C timer, +0x1630 its sound, +0x1634 / +0x1638 home (x, z), +0x163C / +0x1640 where it
 * runs off to; +0x16A8 / +0x16A9 its feet down last frame. D_01991600: who it watches ---- */
