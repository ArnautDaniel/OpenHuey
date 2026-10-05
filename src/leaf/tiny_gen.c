/* Small methods and room callbacks still left in the PS2 disassembly that are one of four
 * plain shapes - an empty method, a constant (or 0) returned, or the progress object's
 * method called with the caller's arguments - generated from their instructions
 * (2026-10-05). */
#include "common.h"
#include "game.h"
#include "progress.h"

s32 func_00344FB0(void) {
    return 0;
}

s32 func_00344FF0(void) {
    return 0x1;
}

s32 func_0035B010(void) {
    return 0x3;
}

s32 func_0035B020(void) {
    return 0x7;
}

s32 func_0035B030(void) {
    return 0xE;
}

s32 func_0035B040(void) {
    return 0x18;
}

s32 func_0035B050(void) {
    return 0xB;
}

void func_0035F0F0(void) {
}

void func_0036A580(void) {
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036DB40(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036E040(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036E3B0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036E660(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036E8E0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036EB90(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036EFB0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036F8B0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036FC70(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036FDF0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_003789B0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_003790F0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_003793A0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_00379680(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_00379950(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_00379D60(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0037A400(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0037A730(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0037A9E0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0037AC90(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

void func_00322560(void) {
}

void func_00322A00(void) {
}

void func_00322B70(void) {
}

void func_003244C0(void) {
}

void func_0032F830(void) {
}

void func_0032F840(void) {
}

void func_0032F850(void) {
}

void func_0032F860(void) {
}

s32 func_0032F870(void) {
    return 0;
}

s32 func_003310B0(void) {
    return 0xE;
}

s32 func_003310C0(void) {
    return 0xD;
}

void func_00331440(void) {
}

s32 func_00345FF0(void) {
    return 0x22;
}

s32 func_003479D0(void) {
    return 0x23;
}

s32 func_00353E10(void) {
    return 0x3;
}

s32 func_00353E20(void) {
    return 0x7;
}

s32 func_00353E30(void) {
    return 0x11;
}

s32 func_00353E40(void) {
    return 0x21;
}

s32 func_00353E50(void) {
    return 0xA;
}

void func_003634E0(void) {
}

void func_0037BC90(void) {
}

void func_002CD760(void) {
}

void func_002CD770(void) {
}

void func_002CD780(void) {
}

void func_002CD790(void) {
}

s32 func_002CD7A0(void) {
    return 0;
}

s32 func_002CEFA0(void) {
    return 0xE;
}

s32 func_002CEFB0(void) {
    return 0xD;
}

void func_002CF380(void) {
}

void func_0033E730(void) {
}

void func_00226808(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_00226868(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_00226870(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_00226878(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_00226880(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_00226888(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_00226890(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_00226898(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_002268A0(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_002268A8(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_002268B0(void) {
}

/* (possibly dead code: nothing in the game references it) */
void func_002268B8(void) {
}

s32 func_0031FBF0(void) {
    return 0x11;
}

s32 func_0031FC00(void) {
    return 0x10;
}

s32 func_0031E990(void) {
    return 0x1;
}

void func_00325D60(void) {
}

void func_0032A0D0(void) {
}

s32 func_00336F80(void) {
    return 0x3;
}

s32 func_00336F90(void) {
    return 0x7;
}

s32 func_00336FA0(void) {
    return 0x1C;
}

s32 func_00336FB0(void) {
    return 0x3B;
}

s32 func_00336FC0(void) {
    return 0x17;
}

void func_00355950(void) {
}

void func_00367E90(void) {
}

void func_00320510(void) {
}

s32 func_00320530(void) {
    return 0x2D;
}

s32 func_00320540(void) {
    return 0x2C;
}

s32 func_00320550(void) {
    return 0x3;
}

s32 func_00320560(void) {
    return 0x7;
}

s32 func_00320570(void) {
    return 0x1E;
}

s32 func_00320580(void) {
    return 0x28;
}

s32 func_0032C5F0(void) {
    return 0x3;
}

s32 func_0032C600(void) {
    return 0x6;
}

s32 func_0032C610(void) {
    return 0xB;
}

s32 func_0032C620(void) {
    return 0xE;
}

s32 func_00346F00(void) {
    return 0xC;
}

s32 func_00346F10(void) {
    return 0xB;
}

s32 func_003482E0(void) {
    return 0xC;
}

s32 func_003482F0(void) {
    return 0xB;
}

s32 func_00348960(void) {
    return 0x24;
}

s32 func_00349270(void) {
    return 0xC;
}

s32 func_00349280(void) {
    return 0xB;
}

s32 func_00349940(void) {
    return 0x3;
}

s32 func_00349950(void) {
    return 0x7;
}

s32 func_00349960(void) {
    return 0xF;
}

s32 func_00349970(void) {
    return 0x1F;
}

s32 func_00349980(void) {
    return 0xA;
}

s32 func_0034D230(void) {
    return 0x11;
}

s32 func_0034D240(void) {
    return 0x10;
}

s32 func_003658D0(void) {
    return 0xA;
}

s32 func_003658E0(void) {
    return 0x9;
}
