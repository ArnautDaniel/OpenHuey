#include "common.h"
#include "ptmf.h"
#include "progress.h"
#include "globals.h"
#include "actor.h"

/* Field access by byte offset into objects whose layout is not yet known. */
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define B5_PI 0x1.921fb60000000p+1f /* 3.14159274 */ /* 0x40490FDB */

extern u8 D_00420B40[];
extern u8 D_00420B80[];
extern u8 D_00420CD0[];
extern u8 D_00420D80[];
extern void *D_004210D0[];
extern void *D_00421100[];
extern u8 D_00421110[];
extern u8 D_00421130[];
extern u8 D_004211D0[];
extern u8 D_00421250[];
extern u8 D_00421400[];
extern u8 D_00421420[];
extern void *D_00421BB0[];
extern void *D_00421BF0[];
extern u8 D_00421C50[];
extern u8 D_00421C60[];
extern u8 D_00421CD0[];
extern u8 D_00421D50[];
extern u8 D_00421EF0[];
extern u8 D_00421FC0[];
extern void *D_004222D0[];
extern u8 D_00422320[];
extern u8 D_00422360[];
extern u8 D_004223A0[];
extern u8 D_004223E0[];
extern void *D_0047AD08[];

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

void *func_00305EF0(void) {
    return D_00420B40;
}

void *func_00305F00(void) {
    return D_00420B80;
}

void *func_00305F10(void) {
    return D_00420CD0;
}

void *func_00305F20(void) {
    return D_00420D80;
}

void *func_00305F30(void *self, s32 i) {
    return D_004210D0[i];
}

void *func_00305F50(void) {
    return D_00421110;
}

void *func_00305F60(void *self, s32 i) {
    return D_00421100[i];
}

void func_003062F0(u8 *self, f32 *src) {
    if (src != NULL && src[0] == 0.0f) {
        F32(self, 0x10) = src[1];
        F32(self, 0x14) = src[2];
        F32(self, 0x18) = src[3];
        F32(self, 0x1C) = 1.0f;
        F32(self, 0x30) = src[4];
        F32(self, 0x34) = src[5];
        F32(self, 0x20) = (B5_PI * src[6]) / 180.0f;
        F32(self, 0x24) = (B5_PI * src[7]) / 180.0f;
        F32(self, 0x28) = (B5_PI * src[8]) / 180.0f;
        F32(self, 0x2C) = 0.0f;
    }
}

void *func_003089F0(void) {
    return D_00421130;
}

void *func_00308A00(void) {
    return D_004211D0;
}

void *func_00308A10(void) {
    return D_00421250;
}

void *func_00308A20(void) {
    return D_00421400;
}

void *func_00308A30(void) {
    return D_00421420;
}

void *func_00308A40(void *self, s32 i) {
    return D_00421BB0[i];
}

void *func_00308A60(void) {
    return D_00421C50;
}

void *func_00308A70(void *self, s32 i) {
    return D_00421BF0[i];
}

s32 func_00308AC0(void *self, u8 *obj) {
    U32(obj, 0x104) = U32(gCharPlayer, 0x34);
    S32(obj, 0x108) = 0x204;
    obj[0xE1] = 0;
    S32(obj, 0xF4) = 6;
    return 1;
}

void *func_00308B50(void) {
    return D_00421C60;
}

void *func_00308B60(void) {
    return D_00421CD0;
}

void *func_00308B70(void) {
    return D_00421D50;
}

void *func_00308B80(void) {
    return D_00421EF0;
}

void *func_00308B90(void) {
    return D_00421FC0;
}

void *func_00308BA0(void *self, s32 i) {
    return D_004222D0[i];
}

void *func_00308BC0(void) {
    return D_00422320;
}

void *func_00308BD0(void *self, s32 i) {
    return D_0047AD08[i];
}

void *func_00308FD0(void) {
    return D_004223E0;
}

void *func_00309450(void) {
    return b5_prog_flag8000() ? D_004223A0 : D_00422360;
}
