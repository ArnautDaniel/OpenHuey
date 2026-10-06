/* Batch 7 leaf functions, 0x00351D50.. : resource-table getters, small state objects. */
#include "common.h"
#include "ptmf.h"
#include "globals.h"

#define B7_W(p, off)  (*(s32 *)((u8 *)(p) + (off)))
#define B7_H(p, off)  (*(s16 *)((u8 *)(p) + (off)))
#define B7_B(p, off)  (*(u8 *)((u8 *)(p) + (off)))
#define B7_F(p, off)  (*(f32 *)((u8 *)(p) + (off)))
#define B7_D(p, off)  (*(s64 *)((u8 *)(p) + (off)))


extern char D_00462F30[];
extern u8 D_004434D0[], D_00443510[], D_00443560[], D_004435E0[], D_00443680[];
extern void *D_00443708[];
extern u8 D_00443728[], D_00443740[], D_004437C0[];
extern void *D_0047AFBC[];
extern u8 D_00443860[], D_00443870[], D_00443890[], D_00443910[], D_00443AB0[], D_00443B00[];
extern void *D_00443DD0[];
extern u8 D_00443E10[];
extern void *D_00443E00[];
extern u8 D_004631C0[], D_004631E0[];
extern u8 D_00462FC0[], D_00462FE0[], D_00463000[], D_00463020[], D_00463040[], D_00463060[], D_00463080[];
extern u8 D_004630C0[], D_004630E0[], D_00463100[], D_00463120[], D_00463140[], D_00463160[], D_00463180[];
extern u8 D_00443E20[];
extern void *D_0047AFC8[];
extern u8 D_00443E60[];

/* Starts loading a file named D_00462F30 into dest. */
s32 func_00351D50(void *self, void *dest) {
    void *loader = gFileLoader;

    return VCALL(loader, 0xC, s32 (*)(void *, const char *, void *, s32, s32))(loader, D_00462F30, dest,
                                                                              0x4000000, 0);
}

void *func_00352030(void) { return D_004434D0; }
void *func_00352040(void) { return D_00443510; }

/* Initialises a render/texture setting block at +0x198. */
void func_00352A70(u8 *p) {
    B7_W(p, 0x210) = 0;
    B7_W(p, 0x214) = 0;
    B7_D(p, 0x198) = -1;
    B7_W(p, 0x1A4) = 0;
    B7_W(p, 0x1A8) = 0;
    B7_W(p, 0x1AC) = 0;
    B7_W(p, 0x1B0) = 0x19;
    B7_H(p, 0x1B4) = 4;
    B7_H(p, 0x1B6) = 0x6C;
    B7_H(p, 0x1B8) = 0x4C;
    B7_H(p, 0x1BA) = 8;
    B7_H(p, 0x1BC) = 8;
    B7_H(p, 0x1BE) = 0x200;
    B7_H(p, 0x1C0) = 0x100;
    B7_B(p, 0x1C2) = 0x40;
    B7_B(p, 0x1C3) = 1;
    B7_B(p, 0x1C4) = 1;
    B7_B(p, 0x1C5) = 0x10;
    B7_B(p, 0x1C6) = 0xFF;
}

void *func_00352B60(void) { return D_00443560; }
void *func_00352B70(void) { return D_004435E0; }
void *func_00352B90(void) { return D_00443680; }
void *func_00352BA0(void *self, s32 i) { return D_00443708[i]; }
void *func_00352BC0(void) { return D_00443728; }
void *func_00352D00(void) { return D_00443740; }
void *func_00352D10(void) { return D_004437C0; }
void *func_00352D30(void *self, s32 i) { return D_0047AFBC[i]; }
void *func_00352D50(void) { return D_00443860; }
void *func_00352E80(void) { return D_00443870; }
void *func_00352E90(void) { return D_00443890; }
void *func_00352EA0(void) { return D_00443910; }
void *func_00352EB0(void) { return D_00443AB0; }
void *func_00352EC0(void) { return D_00443B00; }
void *func_00352ED0(void *self, s32 i) { return D_00443DD0[i]; }
void *func_00352EF0(void) { return D_00443E10; }
void *func_00352F00(void *self, s32 i) { return D_00443E00[i]; }
void *func_00353DB0(void) { return D_004631C0; }
void *func_00353DC0(void) { return D_004631E0; }
s32 func_00353DD0(void) { return 0x41000; }

void func_00353DE0(u8 *p, f32 x, f32 y, f32 z) {
    B7_F(p, 0xBD0) = x;
    B7_F(p, 0xBD4) = y;
    B7_F(p, 0xBD8) = z;
}

void func_00353DF0(u8 *p) { B7_B(p, 0xDE) |= 2; }
void func_00353E00(u8 *p) { B7_B(p, 0xDE) &= ~2; }

void *func_00353E60(void *self, u32 i) {
    switch (i) {
    case 0: return D_00462FC0;
    case 1: return D_00462FE0;
    case 2: return D_00463000;
    case 3: return D_00463020;
    case 4: return D_00463040;
    case 5: return D_00463060;
    case 6: return D_00463080;
    }
    return NULL;
}

void *func_00353EF0(void *self, u32 i) {
    switch (i) {
    case 0: return D_004630C0;
    case 1: return D_004630E0;
    case 2: return D_00463100;
    case 3: return D_00463120;
    case 4: return D_00463140;
    case 5: return D_00463160;
    case 6: return D_00463180;
    }
    return NULL;
}

void func_003544A0(u8 *p) {
    B7_H(p, 0x840) = 0x10;
    *(void **)(p + 0x844) = D_00443E20;
}

void func_00358270(u8 *p, s32 *src) {
    if (src == NULL) {
        B7_W(p, 0x4) = 0;
        return;
    }
    if (*src != B7_W(p, 0x4)) {
        B7_W(p, 0x4) = *src;
        B7_W(p, 0xC) = (B7_W(p, 0x4) == 0x80) ? 2 : 0;
    }
}

/* Fade step: +4 speed, +8 value, +C state (0 up to +4, 1 down to 30, 2 up to 56, 0xFF done). */
s32 func_00358B30(u8 *p) {
    switch (B7_W(p, 0xC)) {
    case 0:
        B7_W(p, 0x8) += B7_W(p, 0x4) >> 4;
        if (B7_W(p, 0x4) < B7_W(p, 0x8)) {
            B7_W(p, 0x8) = B7_W(p, 0x4);
            B7_W(p, 0xC) = 1;
        }
        break;
    case 1:
        B7_W(p, 0x8) -= B7_W(p, 0x4) >> 4;
        if (B7_W(p, 0x8) < 0x1E) {
            B7_W(p, 0x8) = 0x1E;
            B7_W(p, 0xC) = 0xFF;
        }
        break;
    case 2:
        B7_W(p, 0x8) += B7_W(p, 0x4) >> 5;
        if (B7_W(p, 0x8) >= 0x38) {
            B7_W(p, 0x8) = 0x38;
            B7_W(p, 0xC) = 0xFF;
        }
        break;
    }
    return 1;
}

void func_00358C10(u8 *p) {
    B7_W(p, 0x4) = 0;
    B7_W(p, 0x8) = 0;
    B7_W(p, 0xC) = 0;
}

void func_00358C80(u8 *p, u8 *src) {
    if (src != NULL) {
        B7_W(p, 0x4) = *src;
    }
}

s32 func_00359200(u8 *p) { return B7_W(p, 0x4) >= 0; }
void func_00359210(u8 *p) { B7_W(p, 0x4) = -1; }

/* Initialises two render setting blocks at +0x2418 and +0x2450. */
void func_0035A170(u8 *p) {
    B7_W(p, 0x2910) = 0;
    B7_W(p, 0x2914) = 0;
    B7_D(p, 0x2418) = -1;
    B7_W(p, 0x2424) = 0;
    B7_W(p, 0x2428) = 0;
    B7_W(p, 0x242C) = 0;
    B7_W(p, 0x2430) = 0x19;
    B7_H(p, 0x2434) = 0x40;
    B7_H(p, 0x2436) = 0x1A0;
    B7_H(p, 0x2438) = 0x40;
    B7_H(p, 0x243A) = 0x20;
    B7_H(p, 0x243C) = 0x20;
    B7_H(p, 0x243E) = 0x200;
    B7_H(p, 0x2440) = 0x100;
    B7_B(p, 0x2442) = 0;
    B7_B(p, 0x2443) = 1;
    B7_B(p, 0x2444) = 1;
    B7_B(p, 0x2445) = 0x10;
    B7_B(p, 0x2446) = 2;
    B7_D(p, 0x2450) = -1;
    B7_W(p, 0x245C) = 0;
    B7_W(p, 0x2460) = 0;
    B7_W(p, 0x2464) = 0;
    B7_W(p, 0x2468) = 0x19;
    B7_H(p, 0x246C) = 0x20;
    B7_H(p, 0x246E) = 0;
    B7_H(p, 0x2470) = 0x40;
    B7_H(p, 0x2472) = 0x20;
    B7_H(p, 0x2474) = 0x20;
    B7_H(p, 0x2476) = 0x200;
    B7_H(p, 0x2478) = 0x100;
    B7_B(p, 0x247A) = 0;
    B7_B(p, 0x247B) = 1;
    B7_B(p, 0x247C) = 1;
    B7_B(p, 0x247D) = 0x10;
    B7_B(p, 0x247E) = 0xFF;
}

void func_0035AE90(u8 *p) {
    B7_B(p, 0x78) = 0;
    B7_W(p, 0x74) = 0;
    B7_W(p, 0x70) = 2;
}

void *func_0035AF20(void) { return D_00443E60; }
void *func_0035AF30(void *self, s32 i) { return D_0047AFC8[i]; }

extern u8 D_00463220[], D_00463240[], D_00463260[], D_00463280[], D_004632A0[], D_004632C0[], D_004632E0[];

/* (as func_00353EF0) */
void *func_0035B060(void *self, u32 i) {
    switch (i) {
    case 0: return D_00463220;
    case 1: return D_00463240;
    case 2: return D_00463260;
    case 3: return D_00463280;
    case 4: return D_004632A0;
    case 5: return D_004632C0;
    case 6: return D_004632E0;
    }
    return NULL;
}
