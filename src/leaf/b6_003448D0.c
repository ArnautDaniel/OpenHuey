#include "common.h"
#include "ptmf.h"

extern u32 D_00438D30[];
extern u32 D_00438D60[];
extern u32 D_00438E20[];
extern u32 D_00438F10[];
extern u32 D_00439130[];
extern u32 D_00439150[];
extern u32 D_00439160[];
extern u32 D_00439220[];
extern u32 D_00439320[];
extern u32 D_004396D0[];
extern u32 D_00439AA0[];
extern u32 D_00439AF0[];
extern u32 D_00439B10[];
extern u32 D_00439C00[];
extern u32 D_00439D00[];
extern u32 D_00439F10[];
extern u32 D_00439FB0[];
extern u32 D_0043A0E0[];
extern u32 D_0043A100[];
extern u32 D_0043A120[];
extern u32 D_0043A1A0[];
extern u32 D_0043A230[];
extern u32 D_0043A280[];
extern u32 D_0043A2B0[];
extern u32 D_0043A880[];
extern u32 D_0043A8B0[];
extern u32 D_0043A8D0[];
extern u32 D_0043A8F0[];
extern u32 D_0043AA40[];
extern u32 D_0043AB10[];
extern u32 D_0047AE90[];
extern u32 D_0047AEA8[];
extern u32 D_0047AEB0[];
extern u8 *gCharPursuer;
extern u8 *gProgress;

void *func_003448D0(void) {
    return D_00438D30;
}

void *func_003448E0(void) {
    return D_00438D60;
}

void *func_003448F0(void) {
    return D_00438E20;
}

void *func_00344900(void) {
    return D_00438F10;
}

u32 func_00344920(void *self, s32 i) {
    return D_00439130[i];
}

void *func_00344940(void) {
    return D_00439150;
}

u32 func_00344950(void *self, s32 i) {
    return D_0047AE90[i];
}

void *func_003449D0(void) {
    return D_00439160;
}

void *func_003449E0(void) {
    return D_00439220;
}

void *func_003449F0(void) {
    return D_00439320;
}

void *func_00344A00(void) {
    return D_004396D0;
}

u32 func_00344A20(void *self, s32 i) {
    return D_00439AA0[i];
}

void *func_00344A40(void) {
    return D_00439AF0;
}

u32 func_00344A50(void *self, s32 i) {
    return D_0047AEA8[i];
}

/* 1 if the object at gCharPursuer exists, is active (+0x28) and is in mode 4 or 5 (+0xE8). */
s32 func_00344AA0(void) {
    u8 *p = gCharPursuer;
    s32 mode;

    if (p == NULL || p[0x28] == 0) {
        return 0;
    }
    mode = *(s32 *)(p + 0xE8);
    return mode == 4 || mode == 5;
}

s32 func_00344B10(void) {
    u8 *p = gCharPursuer;
    s32 mode;

    if (p == NULL || p[0x28] == 0) {
        return 0;
    }
    mode = *(s32 *)(p + 0xE8);
    if (mode == 4 || mode == 5) {
        return 0;
    }
    return gProgress[0x1130] != 0xFE;
}

void *func_00344BF0(void) {
    return D_00439B10;
}

void *func_00344C00(void) {
    return D_00439C00;
}

void *func_00344C10(void) {
    return D_00439D00;
}

void *func_00344C20(void) {
    return D_00439F10;
}

void *func_00344C30(void) {
    return D_00439FB0;
}

u32 func_00344C40(void *self, s32 i) {
    return D_0047AEB0[i];
}

void *func_00344C60(void) {
    return D_0043A100;
}

u32 func_00344C70(void *self, s32 i) {
    return D_0043A0E0[i];
}

s32 func_00344CC0(void) {
    u8 *p = gCharPursuer;

    if (p == NULL || p[0x28] == 0) {
        return 0;
    }
    return *(s32 *)(p + 0xE8) == 0;
}

s32 func_00344D10(void) {
    u8 *p = gCharPursuer;

    if (p == NULL || p[0x28] == 0 || *(s32 *)(p + 0xE8) == 0) {
        return 0;
    }
    return gProgress[0x1130] != 0xFE;
}

void *func_00344DE0(void) {
    return D_0043A120;
}

void *func_00344DF0(void) {
    return D_0043A1A0;
}

void *func_00344E00(void) {
    return D_0043A230;
}

void *func_00344E10(void) {
    return D_0043A280;
}

void *func_00344E20(void) {
    return D_0043A2B0;
}

u32 func_00344E30(void *self, s32 i) {
    return D_0043A880[i];
}

void *func_00344E50(void) {
    return D_0043A8D0;
}

u32 func_00344E60(void *self, s32 i) {
    return D_0043A8B0[i];
}

void *func_00344EE0(void) {
    return D_0043A8F0;
}

void *func_00344EF0(void) {
    return D_0043AA40;
}

void *func_00344F00(void) {
    return D_0043AB10;
}
