/* Leaf functions (batch 3). Most are getters returning the address of a static table
 * (or one entry of it), likely per-class descriptor/state tables. */
#include "common.h"
#include "ptmf.h"

extern u32 D_004038B0[];
extern u8 D_00403970[];
extern u32 D_00403940[];
extern u8 D_00403980[];
extern u8 D_004039B0[];
extern u8 D_00403A10[];
extern u8 D_00403C10[];
extern u8 D_00403C30[];
extern u32 D_00403F40[];
extern u8 D_00403F80[];
extern u32 D_0047AB34[];
extern u8 D_00403F90[];
extern u8 D_00404040[];
extern u8 D_00404080[];
extern u8 D_00404100[];
extern u8 D_00404198[];
extern u32 D_004046C0[];
extern u8 D_00404730[];
extern u32 D_00404710[];
extern u8 D_00404740[];
extern u8 D_004048E0[];
extern u8 D_00404A20[];
extern u8 D_00404CE0[];
extern u8 D_00404E00[];
extern u32 D_00405570[];
extern u8 D_00405620[];
extern u32 D_00405610[];
extern u8 D_00405640[];
extern u8 D_004056D0[];
extern u8 D_00405730[];
extern u8 D_00405850[];
extern u32 D_00405A70[];
extern u8 D_00405AA0[];
extern u8 D_00405AC0[];
extern u8 D_00405B80[];
extern u8 D_00405BD0[];
extern u8 D_00405E30[];
extern u32 D_00406470[];
extern u8 D_00406530[];
extern u32 D_00406510[];

u32 func_002B0460(void *self, s32 i) {
    return D_004038B0[i];
}

void *func_002B0480(void) {
    return D_00403970;
}

u32 func_002B0490(void *self, s32 i) {
    return D_00403940[i];
}

void *func_002B0D50(void) {
    return D_00403980;
}

void *func_002B0D60(void) {
    return D_004039B0;
}

void *func_002B0D70(void) {
    return D_00403A10;
}

void *func_002B0D80(void) {
    return D_00403C10;
}

void *func_002B0D90(void) {
    return D_00403C30;
}

u32 func_002B0DA0(void *self, s32 i) {
    return D_00403F40[i];
}

void *func_002B0DC0(void) {
    return D_00403F80;
}

u32 func_002B0DD0(void *self, s32 i) {
    return D_0047AB34[i];
}

void *func_002B0E90(void) {
    return D_00403F90;
}

void *func_002B0EA0(void) {
    return D_00404040;
}

void *func_002B0EB0(void) {
    return D_00404080;
}

void *func_002B0EC0(void) {
    return D_00404100;
}

void *func_002B0ED0(void) {
    return D_00404198;
}

u32 func_002B0EE0(void *self, s32 i) {
    return D_004046C0[i];
}

void *func_002B0F00(void) {
    return D_00404730;
}

u32 func_002B0F10(void *self, s32 i) {
    return D_00404710[i];
}

void *func_002B1100(void) {
    return D_00404740;
}

void *func_002B1110(void) {
    return D_004048E0;
}

void *func_002B1120(void) {
    return D_00404A20;
}

void *func_002B1130(void) {
    return D_00404CE0;
}

void *func_002B1140(void) {
    return D_00404E00;
}

u32 func_002B1150(void *self, s32 i) {
    return D_00405570[i];
}

void *func_002B1170(void) {
    return D_00405620;
}

u32 func_002B1180(void *self, s32 i) {
    return D_00405610[i];
}

void *func_002B1660(void) {
    return D_00405640;
}

void *func_002B1670(void) {
    return D_004056D0;
}

void *func_002B1680(void) {
    return D_00405730;
}

void *func_002B1690(void) {
    return D_00405850;
}

u32 func_002B16A0(void *self, s32 i) {
    return D_00405A70[i];
}

void *func_002B16C0(void) {
    return D_00405AA0;
}

void *func_002B17F0(void) {
    return D_00405AC0;
}

void *func_002B1800(void) {
    return D_00405B80;
}

void *func_002B1810(void) {
    return D_00405BD0;
}

void *func_002B1820(void) {
    return D_00405E30;
}

u32 func_002B1830(void *self, s32 i) {
    return D_00406470[i];
}

void *func_002B1850(void) {
    return D_00406530;
}

u32 func_002B1860(void *self, s32 i) {
    return D_00406510[i];
}
