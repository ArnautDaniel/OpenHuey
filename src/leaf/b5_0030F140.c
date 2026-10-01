#include "common.h"
#include "ptmf.h"

/* Field access by byte offset into objects whose layout is not yet known. */
#define S16(p, off) (*(s16 *)((u8 *)(p) + (off)))
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define S64(p, off) (*(s64 *)((u8 *)(p) + (off)))
#define PTR(p, off) (*(void * *)((u8 *)(p) + (off)))

extern void *D_00426860[];
extern u8 D_00426890[];
extern u8 D_004268B0[];
extern u8 D_004268F0[];
extern u8 D_00426930[];
extern u8 D_00426950[];
extern void *D_00426D10[];
extern void *D_00426D40[];
extern u8 D_00426D70[];
extern u8 D_00426D80[];
extern u8 D_00426EF0[];
extern u8 D_00426FF0[];
extern u8 D_004272A0[];
extern void *D_00427F90[];
extern void *D_00428050[];
extern u8 D_00428080[];
extern u8 D_004280A0[];
extern u8 D_004281E0[];
extern u8 D_00428220[];
extern u8 D_00428460[];
extern u8 D_004284A0[];
extern void *D_00429090[];
extern void *D_004290C0[];
extern u8 D_004291A0[];
extern u8 D_004291C0[];
extern u8 D_00429210[];
extern u8 D_004292A0[];
extern u8 D_004293E0[];
extern u8 D_00429408[];
extern u8 D_00429418[];
extern void *D_00429540[];
extern u8 D_00429580[];
extern u8 D_00429730[];
extern u8 D_00429770[];
extern u8 D_00429C10[];
extern u8 D_00429C50[];

void *func_0030F140(void) {
    return D_00426890;
}

void *func_0030F150(void *self, s32 i) {
    return D_00426860[i];
}

void *func_0030F8B0(void) {
    return D_004268B0;
}

void *func_0030F8C0(void) {
    return D_004268F0;
}

void *func_0030F8D0(void) {
    return D_00426930;
}

void *func_0030F8E0(void) {
    return D_00426950;
}

void *func_0030F8F0(void *self, s32 i) {
    return D_00426D10[i];
}

void *func_0030F910(void) {
    return D_00426D70;
}

void *func_0030F920(void *self, s32 i) {
    return D_00426D40[i];
}

void *func_0030F9A0(void) {
    return D_00426D80;
}

void *func_0030F9B0(void) {
    return D_00426EF0;
}

void *func_0030F9C0(void) {
    return D_00426FF0;
}

void *func_0030F9D0(void) {
    return D_004272A0;
}

void *func_0030F9E0(void *self, s32 i) {
    return D_00427F90[i];
}

void *func_0030FA00(void) {
    return D_00428080;
}

void *func_0030FA10(void *self, s32 i) {
    return D_00428050[i];
}

void *func_003101D0(void) {
    return D_004280A0;
}

void *func_003101E0(void) {
    return D_004281E0;
}

void *func_003101F0(void) {
    return D_00428220;
}

void *func_00310200(void) {
    return D_00428460;
}

void *func_00310210(void) {
    return D_004284A0;
}

void *func_00310220(void *self, s32 i) {
    return D_00429090[i];
}

void *func_00310240(void) {
    return D_004291A0;
}

void *func_00310A10(void *self, s32 i) {
    return D_004290C0[i];
}

void *func_00310A90(void) {
    return D_004291C0;
}

void *func_00310AA0(void) {
    return D_00429210;
}

void *func_00310AB0(void) {
    return D_004292A0;
}

void *func_00310AC0(void) {
    return D_004293E0;
}

void *func_00310AD0(void *self, s32 i) {
    return D_00429540[i];
}

void *func_00310AF0(void) {
    return D_00429408;
}

void *func_00310B00(void) {
    return D_00429418;
}

void *func_00310B10(void) {
    return D_00429580;
}

void *func_00311AB0(void) {
    return D_00429730;
}

void *func_00311AC0(void) {
    return D_00429770;
}

void func_003140A0(u8 *self) {
    PTR(self, 0x874) = D_00429C10;
}

void func_00314910(u8 *self) {
    S32(self, 0xF60) = 0;
    self[0xF64] = 0;
    S64(self, 0xC18) = -1;
    S32(self, 0xC24) = 0;
    S32(self, 0xC28) = 0;
    S32(self, 0xC2C) = 0;
    S32(self, 0xC30) = 25;
    S16(self, 0xC34) = 0x20;
    S16(self, 0xC36) = 0x6C;
    S16(self, 0xC38) = 0x4C;
    S16(self, 0xC3A) = 8;
    S16(self, 0xC3C) = 8;
    S16(self, 0xC3E) = 0x200;
    S16(self, 0xC40) = 0x100;
    self[0xC42] = 0x40;
    self[0xC43] = 1;
    self[0xC44] = 1;
    self[0xC45] = 0x10;
    self[0xC46] = 0xFF;
}

void *func_00315C40(void) {
    return D_00429C50;
}
