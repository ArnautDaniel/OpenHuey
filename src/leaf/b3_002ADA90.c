/* Leaf functions (batch 3). Most are getters returning the address of a static table
 * (or one entry of it), likely per-class descriptor/state tables. */
#include "common.h"
#include "ptmf.h"

extern u32 D_003FD310[];
extern u8 D_003FD360[];
extern u32 D_0047AA78[];
extern u8 D_003FD370[];
extern u8 D_003FD420[];
extern u8 D_003FD470[];
extern u8 D_003FD580[];
extern u32 D_003FD950[];
extern u8 D_003FD660[];
extern u8 D_003FD9B0[];
extern u8 D_003FD9C0[];
extern u8 D_003FDA40[];
extern u8 D_003FDB00[];
extern u8 D_003FDEC8[];
extern u8 D_003FDEE0[];
extern u32 D_003FE2C0[];
extern u8 D_003FE2F0[];
extern u32 D_0047AAA0[];
extern u8 D_003FE300[];
extern u8 D_003FE350[];
extern u8 D_003FE3D0[];
extern u8 D_003FE420[];
extern u32 D_0047AAB0[];
extern u8 D_003FE450[];
extern u32 D_0047AAB8[];
extern u8 D_003FE460[];
extern u8 D_003FE540[];
extern u8 D_003FE680[];
extern u8 D_003FE9C0[];
extern u8 D_003FEA60[];
extern u8 D_003FEAD0[];
extern u32 D_003FF030[];
extern u8 D_003FF130[];
extern u8 D_003FF150[];
extern u32 D_003FF110[];
extern u8 D_003FF170[];
extern u8 D_003FF200[];
extern u8 D_003FF340[];
extern u8 D_003FF650[];
extern u8 D_003FF810[];

u32 func_002ADA90(void *self, s32 i) {
    return D_003FD310[i];
}

void *func_002ADAB0(void) {
    return D_003FD360;
}

u32 func_002ADAC0(void *self, s32 i) {
    return D_0047AA78[i];
}

void *func_002ADD70(void) {
    return D_003FD370;
}

void *func_002ADD80(void) {
    return D_003FD420;
}

void *func_002ADD90(void) {
    return D_003FD470;
}

void *func_002ADDA0(void) {
    return D_003FD580;
}

u32 func_002ADDB0(void *self, s32 i) {
    return D_003FD950[i];
}

void *func_002ADDD0(void) {
    return D_003FD660;
}

void *func_002ADDF0(void) {
    return D_003FD9B0;
}

void *func_002ADFF0(void) {
    return D_003FD9C0;
}

void *func_002AE000(void) {
    return D_003FDA40;
}

void *func_002AE010(void) {
    return D_003FDB00;
}

void *func_002AE020(void) {
    return D_003FDEC8;
}

void *func_002AE030(void) {
    return D_003FDEE0;
}

u32 func_002AE040(void *self, s32 i) {
    return D_003FE2C0[i];
}

void *func_002AE060(void) {
    return D_003FE2F0;
}

u32 func_002AE070(void *self, s32 i) {
    return D_0047AAA0[i];
}

void *func_002AE0F0(void) {
    return D_003FE300;
}

void *func_002AE100(void) {
    return D_003FE350;
}

void *func_002AE110(void) {
    return D_003FE3D0;
}

void *func_002AE120(void) {
    return D_003FE420;
}

u32 func_002AE130(void *self, s32 i) {
    return D_0047AAB0[i];
}

void *func_002AE150(void) {
    return D_003FE450;
}

u32 func_002AE160(void *self, s32 i) {
    return D_0047AAB8[i];
}

void *func_002AE3F0(void) {
    return D_003FE460;
}

void *func_002AE400(void) {
    return D_003FE540;
}

void *func_002AE410(void) {
    return D_003FE680;
}

void *func_002AE420(void) {
    return D_003FE9C0;
}

void *func_002AE430(void) {
    return D_003FEA60;
}

void *func_002AE440(void) {
    return D_003FEAD0;
}

u32 func_002AE450(void *self, s32 i) {
    return D_003FF030[i];
}

void *func_002AE470(void) {
    return D_003FF130;
}

void *func_002AE480(void) {
    return D_003FF150;
}

u32 func_002AE490(void *self, s32 i) {
    return D_003FF110[i];
}

void *func_002AF020(void) {
    return D_003FF170;
}

void *func_002AF030(void) {
    return D_003FF200;
}

void *func_002AF040(void) {
    return D_003FF340;
}

void *func_002AF050(void) {
    return D_003FF650;
}

void *func_002AF060(void) {
    return D_003FF810;
}
