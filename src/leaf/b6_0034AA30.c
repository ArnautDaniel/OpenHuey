#include "common.h"
#include "ptmf.h"

extern s32 gCharPlayer;
extern u32 D_0043FA80[];
extern u32 D_0043FC10[];
extern u32 D_0043FCC0[];
extern u32 D_00440080[];
extern u32 D_004400E0[];
extern u32 D_00440100[];
extern u32 D_00440140[];
extern u32 D_004401B0[];
extern u32 D_00440210[];
extern u32 D_00440250[];
extern u32 D_00440E00[];
extern u32 D_00441110[];
extern u32 D_00441140[];
extern u32 D_00441170[];
extern u32 D_004411A0[];
extern u32 D_004412A0[];
extern u32 D_00441400[];
extern u32 D_00441500[];
extern u32 D_00441510[];
extern u32 D_00441520[];
extern u32 D_00441560[];
extern u32 D_00441620[];
extern u32 D_004416A0[];
extern u32 D_004417A8[];
extern u32 D_00441850[];
extern u32 D_0047AF14[];
extern u32 D_0047AF18[];
extern u32 D_0047AF38[];
extern u32 D_0047AF3C[];
extern u8 *gProgress;
extern u8 D_00441810[], D_004417D0[];
extern u8 D_00441830[], D_004417F0[];

void *func_0034AA30(void) {
    return D_0043FA80;
}

void *func_0034AA40(void) {
    return D_0043FC10;
}

void *func_0034AA50(void) {
    return D_0043FCC0;
}

u32 func_0034AA60(void *self, s32 i) {
    return D_00440080[i];
}

u32 func_0034AA90(void *self, s32 i) {
    return D_004400E0[i];
}

void *func_0034B070(void) {
    return D_00440100;
}

void *func_0034B080(void) {
    return D_00440140;
}

u32 func_0034B090(void *self, s32 i) {
    return D_0047AF14[i];
}

u32 func_0034B0C0(void *self, s32 i) {
    return D_0047AF18[i];
}

void *func_0034B140(void) {
    return D_004401B0;
}

void *func_0034B150(void) {
    return D_00440210;
}

void *func_0034B160(void) {
    return D_00440250;
}

void *func_0034B170(void) {
    return D_00440E00;
}

u32 func_0034B190(void *self, s32 i) {
    return D_00441110[i];
}

u32 func_0034B1C0(void *self, s32 i) {
    return D_00441140[i];
}

void *func_0034B520(void) {
    return D_00441170;
}

void *func_0034B530(void) {
    return D_004411A0;
}

void *func_0034B540(void) {
    return D_004412A0;
}

void *func_0034B550(void) {
    return D_00441400;
}

u32 func_0034B570(void *self, s32 i) {
    return D_00441500[i];
}

void *func_0034B590(void) {
    return D_00441510;
}

u32 func_0034B5A0(void *self, s32 i) {
    return D_0047AF38[i];
}

void *func_0034B620(void) {
    return D_00441520;
}

void *func_0034B630(void) {
    return D_00441560;
}

void *func_0034B640(void) {
    return D_00441620;
}

void *func_0034B650(void) {
    return D_004416A0;
}

u32 func_0034B660(void *self, s32 i) {
    return D_004417A8[i];
}

u32 func_0034B690(void *self, s32 i) {
    return D_0047AF3C[i];
}

void *func_0034B8C0(void) {
    return D_00441850;
}

void func_0034B8D0(void *self, s32 i, f32 *out) {
    switch (i) {
    case 1: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.e49ba6p+2f /* 7.572 */; break;
    case 3: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.b8e21ap+2f /* -6.8888 */; break;
    case 0: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.541206p+2f /* -5.3136 */; break;
    case 2: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.fbfb16p+2f /* 7.9372 */; break;
    }
}

void func_0034B970(void *self, s32 i, f32 *out) {
    switch (i) {
    case 10: case 11: out[0] = -0x1.3eab36p-5f /* -0.0389 */; out[1] = 0.0f; out[2] = 0x1.4cf4fp+3f /* 10.4049 */; break;
    case 12: case 13: out[0] = 0x1.7652bep-1f /* 0.7311 */; out[1] = 0.0f; out[2] = 0x1.a80832p+3f /* 13.251 */; break;
    case 14: out[0] = -0x1.25a858p+0f /* -1.1471 */; out[1] = 0.0f; out[2] = -0x1.42a64cp+1f /* -2.5207 */; break;
    case 15: out[0] = -0x1.4fdf3cp-2f /* -0.328 */; out[1] = 0.0f; out[2] = -0x1.324a8cp+1f /* -2.3929 */; break;
    }
}

f32 func_0034D250(void) {
    return 7.0f;
}

f32 func_0034D260(void) {
    return 2e+01f;
}

f32 func_0034D270(void) {
    return 12.0f;
}

s32 func_0034D280(u8 *p) {
    f32 d;

    if (*(s32 *)(p + 0xC4) == 1) {
        return 0x203;
    }
    if (*(s32 *)(p + 0x16B8) == 2) {
        return 0x205;
    }
    if (p[0x1544] != 0 && *(s32 *)(p + 0x1540) == gCharPlayer) {
        d = *(f32 *)(p + 0x1588);
        if (d < 100.0f && !(d <= 0.0f)) {
            return 0x206;
        }
    }
    return 0x201;
}

void *func_0034D980(void) {
    return (*(u32 *)(gProgress + 0x30) & 0x8000) ? D_00441830 : D_004417F0;
}

void *func_0034D9C0(void) {
    return (*(u32 *)(gProgress + 0x30) & 0x8000) ? D_00441810 : D_004417D0;
}
