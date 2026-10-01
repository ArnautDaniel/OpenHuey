/* Leaf functions (batch 3). Most are getters returning the address of a static table
 * (or one entry of it), likely per-class descriptor/state tables. */
#include "common.h"
#include "ptmf.h"

extern u32 D_00410F10[];
extern u8 D_00410F50[];
extern u32 D_00410F38[];
extern u8 D_00410F70[];
extern u8 D_00410FB0[];
extern u8 D_00411180[];
extern u8 D_00411430[];
extern u8 D_004114E0[];
extern u32 D_00411B00[];
extern u8 D_00411B60[];
extern u32 D_0047ABE8[];
extern u8 D_00411B80[];
extern u8 D_00411C60[];
extern u8 D_00411D60[];
extern u8 D_00411FB0[];
extern u32 D_00412390[];
extern u8 D_004123F0[];
extern u32 D_004123D0[];

u32 func_002B5330(void *self, s32 i) {
    return D_00410F10[i];
}

void *func_002B5350(void) {
    return D_00410F50;
}

u32 func_002B5360(void *self, s32 i) {
    return D_00410F38[i];
}

void *func_002B5910(void) {
    return D_00410F70;
}

void *func_002B5920(void) {
    return D_00410FB0;
}

void *func_002B5930(void) {
    return D_00411180;
}

void *func_002B5940(void) {
    return D_00411430;
}

void *func_002B5950(void) {
    return D_004114E0;
}

u32 func_002B5960(void *self, s32 i) {
    return D_00411B00[i];
}

void *func_002B5980(void) {
    return D_00411B60;
}

u32 func_002B5990(void *self, s32 i) {
    return D_0047ABE8[i];
}

void *func_002B5EE0(void) {
    return D_00411B80;
}

void *func_002B5EF0(void) {
    return D_00411C60;
}

void *func_002B5F00(void) {
    return D_00411D60;
}

void *func_002B5F10(void) {
    return D_00411FB0;
}

u32 func_002B5F20(void *self, s32 i) {
    return D_00412390[i];
}

void *func_002B5F40(void) {
    return D_004123F0;
}

u32 func_002B5F50(void *self, s32 i) {
    return D_004123D0[i];
}

void func_002B6130(u8 *self, u8 *src) {
    *(u32 *)(self + 0x10) = *src;
}

extern u8 *D_0045D1F0;

void func_002B6310(u8 *self) {
    *D_0045D1F0 = 0;
    *(u32 *)(self + 0x10) = 0;
}

extern void *D_0044E4F0; /* global manager object (virtual calls) */

/* Tail call: D_0044E4F0->vfunc_0x40(self->0x18, 0x88000, self->0x20, self->0x24, 3) */
s32 func_002B64B0(u8 *self) {
    return VCALL(D_0044E4F0, 0x40, s32 (*)(void *, u32, u32, u32, u32, u32))(
        D_0044E4F0, *(u32 *)(self + 0x18), 0x88000, *(u32 *)(self + 0x20), *(u32 *)(self + 0x24), 3);
}

s32 func_002B64F0(u8 *self) {
    if (self[0x1B4] == 0) {
        return -1;
    }
    return *(s32 *)(self + 0x1C0);
}

void func_002B6E50(u8 *self) {
    *(u32 *)(self + 0x1A8) = 0;
    self[0xA8] = 0;
    *(s32 *)(self + 0x1C0) = -1;
    self[0x11] = 1;
}

/* If active (+0x1C4), fills a small descriptor; returns the active flag. */
u32 func_002BA220(u8 *self, u8 *out) {
    if (self[0x1C4] != 0) {
        *(u16 *)(out + 0x0) = *(u32 *)(self + 0x20);
        *(u16 *)(out + 0x2) = *(u32 *)(self + 0x24);
        *(u32 *)(out + 0x4) = *(u32 *)(self + 0x1B8);
        out[0x8] = self[0x1B5];
        out[0x9] = 0;
    }
    return self[0x1C4];
}

/* If active (+0x1C4), fills a small descriptor; returns the active flag. */
u32 func_002BA6E0(u8 *self, u8 *out) {
    if (self[0x1C4] != 0) {
        *(u16 *)(out + 0x0) = *(u32 *)(self + 0x20);
        *(u16 *)(out + 0x2) = *(u32 *)(self + 0x24);
        *(u32 *)(out + 0x4) = *(u32 *)(self + 0x1B8);
        out[0x8] = self[0x1B5];
        out[0x9] = 0;
    }
    return self[0x1C4];
}

void func_002BB390(u8 *self) {
    *(u32 *)(self + 0x10) = 0x808080;
    *(u32 *)(self + 0x14) = 0xC0808080;
    *(f32 *)(self + 0x50) = 20.0f;
    *(f32 *)(self + 0x54) = 1000.0f;
    *(u32 *)(self + 0x1C) = 0;
    *(u32 *)(self + 0x20) = 0;
    *(u32 *)(self + 0x24) = 0;
    *(u32 *)(self + 0x28) = 0;
    *(u32 *)(self + 0x2C) = 0;
    *(u32 *)(self + 0x30) = 0;
    *(u32 *)(self + 0x34) = 0;
}

extern void *D_0044E4F0;

/* Stores its arguments, then tail-calls D_0044E4F0->vfunc_0xC(self, c, 0). */
s32 func_002BC000(u8 *self, u32 a, u32 b, u32 c, f32 x, f32 y) {
    *(u32 *)(self + 0x8) = a;
    *(u32 *)(self + 0xC) = b;
    *(f32 *)(self + 0x10) = x;
    *(f32 *)(self + 0x14) = y;
    return VCALL(D_0044E4F0, 0xC, s32 (*)(void *, void *, u32, u32))(D_0044E4F0, self, c, 0);
}

/* Merges two descriptors into the one at self->0x11C: words 4..C are ORed,
 * bytes 0x10..0x16, the word at 0x18 and bytes 0x1C..0x4B are copied from a. */
void func_002BC9C0(u8 *self, u8 *a, u8 *b) {
    s32 i, j;

    *(u32 *)(*(u8 **)(self + 0x11C) + 0x4) = *(u32 *)(a + 0x4) | *(u32 *)(b + 0x4);
    *(u32 *)(*(u8 **)(self + 0x11C) + 0x8) = *(u32 *)(a + 0x8) | *(u32 *)(b + 0x8);
    *(u32 *)(*(u8 **)(self + 0x11C) + 0xC) = *(u32 *)(a + 0xC) | *(u32 *)(b + 0xC);
    {
        u8 *d = *(u8 **)(self + 0x11C);

        d[0x10] = a[0x10];
        d[0x11] = a[0x11];
        d[0x12] = a[0x12];
        d[0x13] = a[0x13];
        d[0x14] = a[0x14];
        d[0x15] = a[0x15];
        d[0x16] = a[0x16];
        *(u32 *)(d + 0x18) = *(u32 *)(a + 0x18);
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            u8 *d = *(u8 **)(self + 0x11C) + i * 12 + j * 4;
            u8 *s = a + i * 12 + j * 4;

            d[0x1C] = s[0x1C];
            d[0x1D] = s[0x1D];
            d[0x1E] = s[0x1E];
            d[0x1F] = s[0x1F];
        }
    }
}

extern u8 *D_0044E978;

void func_002BFB00(u8 *self, u32 a, u32 b) {
    *(u8 **)(self + 0x11C) = D_0044E978 + 0x20;
    *(u32 *)(self + 0x120) = a;
    *(u32 *)(self + 0x124) = b;
}

void func_002BFE40(u8 *self) {
    self[0xA38] = 1;
}

void func_002C0700(u8 *self) {
    self[0x30] = 5;
}

void func_002C0710(u8 *self) {
    self[0x30] = 5;
}

/* If active (+0x1C4), fills a small descriptor; returns the active flag. */
u32 func_002C6250(u8 *self, u8 *out) {
    if (self[0x1C4] != 0) {
        *(u16 *)(out + 0x0) = *(u32 *)(self + 0x20);
        *(u16 *)(out + 0x2) = *(u32 *)(self + 0x24);
        *(u32 *)(out + 0x4) = *(u32 *)(self + 0x1B8);
        out[0x8] = self[0x1B5];
        out[0x9] = 1;
    }
    return self[0x1C4];
}

void func_002C6540(u8 *self, f32 *v) {
    if (v != NULL) {
        *(f32 *)(self + 0x50) = v[0];
        *(f32 *)(self + 0x54) = v[1];
        *(f32 *)(self + 0x58) = v[2];
        *(f32 *)(self + 0x5C) = v[3];
    }
}

void func_002C6630(u8 *self) {
    *(f32 *)(self + 0x50) = 1.0f;
    *(f32 *)(self + 0x54) = 1.0f;
    *(f32 *)(self + 0x58) = 2000.0f;
    *(f32 *)(self + 0x5C) = 2000.0f;
}

s32 func_002C8D90(u8 *self) {
    s32 *v = *(s32 **)(self + 0x4);

    return v[0] + v[1] + v[2] + v[3];
}

/* Tail call to this->vfunc_0x8() */
s32 func_002C9470(void *self) {
    return VCALL(self, 0x8, s32 (*)(void *))(self);
}

extern f32 D_00412900;
extern f32 D_00412904;
extern f32 D_00412908;

void func_002C94E0(u8 *self, u32 a) {
    *(u32 *)(self + 0x14) = a;
    *(f32 *)(self + 0x2A0) = D_00412900;
    *(f32 *)(self + 0x2A4) = D_00412904;
    *(f32 *)(self + 0x2A8) = D_00412908;
    self[0x204] = 0;
}

s32 func_002C95D0(u8 *self) {
    u16 *p = *(u16 **)(self + 0x18);

    if (p == NULL) {
        return -1;
    }
    return *p;
}

void func_002C95F0(u8 *self, u32 a) {
    *(u32 *)(self + 0x10) = *(u32 *)(self + 0xC);
    *(u32 *)(self + 0xC) = a;
}
