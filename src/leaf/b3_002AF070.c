/* Leaf functions (batch 3). Most are getters returning the address of a static table
 * (or one entry of it), likely per-class descriptor/state tables. */
#include "common.h"
#include "ptmf.h"

extern u32 D_00400AD0[];
extern u8 D_00400C40[];
extern u32 D_00400C00[];
extern u8 D_00400C60[];
extern u8 D_00400C90[];
extern u8 D_00400CD0[];
extern u8 D_00400D90[];
extern u8 D_00400E30[];
extern u32 D_00401070[];
extern u8 D_00401090[];
extern u32 D_0047AAF8[];
extern u8 D_004010A0[];
extern u8 D_004010E0[];
extern u8 D_00401140[];
extern u8 D_004011B0[];
extern u32 D_00401800[];
extern u8 D_00401880[];
extern u32 D_00401860[];
extern u8 D_00401890[];
extern u8 D_00401950[];
extern u8 D_004019D0[];
extern u8 D_00401B50[];
extern u32 D_00402270[];
extern u8 D_00402320[];
extern u32 D_004022F0[];
extern u8 D_00402330[];
extern u8 D_00402420[];
extern u8 D_004024F0[];
extern u8 D_00402700[];
extern u8 D_004027E0[];
extern u8 D_00402880[];
extern u32 D_00402CD0[];
extern u8 D_00402D30[];
extern u32 D_0047AB20[];
extern u8 D_00402D50[];
extern u8 D_00402E10[];
extern u8 D_00402EC0[];
extern u8 D_00402FA0[];

u32 func_002AF070(void *self, s32 i) {
    return D_00400AD0[i];
}

void *func_002AF090(void) {
    return D_00400C40;
}

u32 func_002AF0A0(void *self, s32 i) {
    return D_00400C00[i];
}

extern u8 *gCharPlayer; /* global object; +0xF0 -> sub-object with flag byte at +0xAC */

/* Sets bit 1 of the flag byte three times (inlined setter calls); returns 1. */
s32 func_002AF4A0(void) {
    u8 *obj = gCharPlayer;

    (*(u8 **)(obj + 0xF0))[0xAC] |= 2;
    (*(u8 **)(obj + 0xF0))[0xAC] |= 2;
    (*(u8 **)(obj + 0xF0))[0xAC] |= 2;
    return 1;
}

void *func_002AF840(void) {
    return D_00400C60;
}

void *func_002AF850(void) {
    return D_00400C90;
}

void *func_002AF860(void) {
    return D_00400CD0;
}

void *func_002AF870(void) {
    return D_00400D90;
}

void *func_002AF880(void) {
    return D_00400E30;
}

u32 func_002AF890(void *self, s32 i) {
    return D_00401070[i];
}

void *func_002AF8B0(void) {
    return D_00401090;
}

u32 func_002AF8C0(void *self, s32 i) {
    return D_0047AAF8[i];
}

void *func_002AF940(void) {
    return D_004010A0;
}

void *func_002AF950(void) {
    return D_004010E0;
}

void *func_002AF960(void) {
    return D_00401140;
}

void *func_002AF970(void) {
    return D_004011B0;
}

u32 func_002AF980(void *self, s32 i) {
    return D_00401800[i];
}

void *func_002AF9A0(void) {
    return D_00401880;
}

u32 func_002AF9B0(void *self, s32 i) {
    return D_00401860[i];
}

void *func_002AFC90(void) {
    return D_00401890;
}

void *func_002AFCA0(void) {
    return D_00401950;
}

void *func_002AFCB0(void) {
    return D_004019D0;
}

void *func_002AFCC0(void) {
    return D_00401B50;
}

u32 func_002AFCE0(void *self, s32 i) {
    return D_00402270[i];
}

void *func_002AFD00(void) {
    return D_00402320;
}

u32 func_002AFD10(void *self, s32 i) {
    return D_004022F0[i];
}

void *func_002B0150(void) {
    return D_00402330;
}

void *func_002B0160(void) {
    return D_00402420;
}

void *func_002B0170(void) {
    return D_004024F0;
}

void *func_002B0180(void) {
    return D_00402700;
}

void *func_002B0190(void) {
    return D_004027E0;
}

void *func_002B01A0(void) {
    return D_00402880;
}

u32 func_002B01B0(void *self, s32 i) {
    return D_00402CD0[i];
}

void *func_002B01D0(void) {
    return D_00402D30;
}

u32 func_002B01E0(void *self, s32 i) {
    return D_0047AB20[i];
}

extern u8 *gCharPlayer;

s32 func_002B0230(void) {
    u8 *obj = gCharPlayer;

    if (obj == NULL || gCharPlayer[0x28] != 1 || *(s32 *)(gCharPlayer + 0xF8) != 4 ||
        *(s32 *)(gCharPlayer + 0x100) != 0xFF) {
        return 0;
    }
    return 1;
}

void *func_002B0420(void) {
    return D_00402D50;
}

void *func_002B0430(void) {
    return D_00402E10;
}

void *func_002B0440(void) {
    return D_00402EC0;
}

void *func_002B0450(void) {
    return D_00402FA0;
}
