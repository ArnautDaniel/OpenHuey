/* Leaf functions (batch 3). Most are getters returning the address of a static table
 * (or one entry of it), likely per-class descriptor/state tables. */
#include "common.h"
#include "ptmf.h"

extern u8 D_00406550[];
extern u8 D_00406630[];
extern u8 D_00406770[];
extern u8 D_00406930[];
extern u32 D_00407050[];
extern u8 D_004070D0[];
extern u32 D_00407090[];
extern u8 D_00407AE0[];
extern u8 D_00407B70[];
extern u8 D_00407C00[];
extern u8 D_00407CB0[];
extern u32 D_00407E20[];
extern u8 D_00407E50[];
extern u8 D_00407E70[];
extern u8 D_00407E90[];
extern u8 D_00407ED0[];
extern u32 D_0047AB60[];
extern u32 D_0047AB68[];
extern u8 D_00407F30[];
extern u8 D_00407F80[];
extern u8 D_00407FE0[];
extern u8 D_004080A0[];
extern u32 D_00408610[];
extern u8 D_00408690[];
extern u32 D_00408670[];
extern u8 D_004086A0[];
extern u8 D_00408700[];
extern u8 D_00408780[];
extern u8 D_00408840[];
extern u8 D_00408880[];
extern u32 D_00408B40[];
extern u8 D_00408B78[];
extern u32 D_00408B68[];
extern u8 D_00408B90[];
extern u8 D_00408CD0[];
extern u8 D_00408E50[];
extern u8 D_004090C0[];
extern u8 D_004092E0[];
extern u8 D_004092C0[];
extern u32 D_004098D0[];

void *func_002B2390(void) {
    return D_00406550;
}

void *func_002B23A0(void) {
    return D_00406630;
}

void *func_002B23B0(void) {
    return D_00406770;
}

void *func_002B23C0(void) {
    return D_00406930;
}

u32 func_002B23D0(void *self, s32 i) {
    return D_00407050[i];
}

void *func_002B23F0(void) {
    return D_004070D0;
}

u32 func_002B2400(void *self, s32 i) {
    return D_00407090[i];
}

void *func_002B2640(void) {
    return D_00407AE0;
}

void *func_002B2650(void) {
    return D_00407B70;
}

void *func_002B2660(void) {
    return D_00407C00;
}

void *func_002B2670(void) {
    return D_00407CB0;
}

u32 func_002B2690(void *self, s32 i) {
    return D_00407E20[i];
}

void *func_002B26B0(void) {
    return D_00407E50;
}

void *func_002B2880(void) {
    return D_00407E70;
}

void *func_002B2890(void) {
    return D_00407E90;
}

void *func_002B28B0(void) {
    return D_00407ED0;
}

u32 func_002B28D0(void *self, s32 i) {
    return D_0047AB60[i];
}

u32 func_002B2900(void *self, s32 i) {
    return D_0047AB68[i];
}

void *func_002B29C0(void) {
    return D_00407F30;
}

void *func_002B29D0(void) {
    return D_00407F80;
}

void *func_002B29E0(void) {
    return D_00407FE0;
}

void *func_002B29F0(void) {
    return D_004080A0;
}

u32 func_002B2A00(void *self, s32 i) {
    return D_00408610[i];
}

void *func_002B2A20(void) {
    return D_00408690;
}

u32 func_002B2A30(void *self, s32 i) {
    return D_00408670[i];
}

void *func_002B2F50(void) {
    return D_004086A0;
}

void *func_002B2F60(void) {
    return D_00408700;
}

void *func_002B2F70(void) {
    return D_00408780;
}

void *func_002B2F80(void) {
    return D_00408840;
}

void *func_002B2F90(void) {
    return D_00408880;
}

u32 func_002B2FA0(void *self, s32 i) {
    return D_00408B40[i];
}

void *func_002B2FC0(void) {
    return D_00408B78;
}

u32 func_002B2FD0(void *self, s32 i) {
    return D_00408B68[i];
}

void *func_002B3050(void) {
    return D_00408B90;
}

void *func_002B3060(void) {
    return D_00408CD0;
}

void *func_002B3070(void) {
    return D_00408E50;
}

void *func_002B3080(void) {
    return D_004090C0;
}

void *func_002B3090(void) {
    return D_004092E0;
}

void *func_002B30A0(void) {
    return D_004092C0;
}

u32 func_002B30B0(void *self, s32 i) {
    return D_004098D0[i];
}
