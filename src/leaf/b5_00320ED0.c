#include "common.h"
#include "ptmf.h"
#include "progress.h"

/* Field access by byte offset into objects whose layout is not yet known. */
#define S16(p, off) (*(s16 *)((u8 *)(p) + (off)))
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define S64(p, off) (*(s64 *)((u8 *)(p) + (off)))
#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define PTR(p, off) (*(void * *)((u8 *)(p) + (off)))
/* gFileLoader vtable +0xC: start loading file `name` into `dest` (flags 0x4000000) */
#define FILE_LOAD_ASYNC(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, void *, void *, u32, s32))(gFileLoader, name, dest, 0x4000000, 0)

extern u8 D_0042B200[];
extern u8 D_0042B520[];
extern void *D_0042C200[];
extern void *D_0042C2F0[];
extern u8 D_0042C360[];
extern u8 D_0042C380[];
extern u8 D_0042C3C0[];
extern u8 D_0042C6A0[];
extern u8 D_0042C6E0[];
extern u8 D_0042C730[];
extern u8 D_0042C770[];
extern u8 D_0042C790[];
extern u8 D_0042C810[];
extern u8 D_0042C860[];
extern u8 D_0042C900[];
extern u8 D_0042C940[];
extern u8 D_0042C990[];
extern u8 D_0042C9D0[];
extern u8 D_0042CA20[];
extern u8 D_0042CB60[];
extern u8 D_0042CC00[];
extern u8 D_0042D0C0[];
extern void *D_0042E310[];
extern void *D_0042E3F0[];
extern u8 D_0042E410[];
extern u8 D_0042E4C0[];
extern u8 D_00460BE0[];
extern void *D_0047ADB4[];
extern void *D_0047ADB8[];
extern u32 D_0047E36C;
extern void *gFileLoader;

void *func_00320ED0(void) {
    return D_0042B200;
}

void *func_00320EE0(void) {
    return D_0042B520;
}

void *func_00320EF0(void *self, s32 i) {
    return D_0042C200[i];
}

void *func_00320F10(void) {
    return D_0042C360;
}

void *func_00320F20(void *self, s32 i) {
    return D_0042C2F0[i];
}

void *func_00321750(void) {
    return D_0042C380;
}

void *func_00321760(void) {
    return D_0042C3C0;
}

void func_00322A60(u8 *self) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        ((volatile u8 *)self)[0x118 + i] = 0xFF;
    }
    self[0x158] = 0;
}

void func_00322B30(u8 *self) {
    if ((D_0047E36C >> 5) & 1) {
        self[0x158] = 1;
    }
}

void func_00324790(u8 *self, s32 unused, u32 v) {
    S32(self, 0x1540) = v < 3 ? (s32)v : -1;
}

s32 func_003247C0(u8 *self) {
    return self[0x15AF] != 0;
}

/* Saves this enemy's state into gProgress slot `slot` (36-byte records at +0x878). */
void func_00324C00(u8 *self, s32 slot, u32 b12) {
    u8 *rec = (u8 *)gProgress + 0x878 + slot * 36;

    U32(rec, 0x0) = U32(self, 0x30);
    U32(rec, 0x4) = U32(self, 0x1540);
    U32(rec, 0x8) = U32(self, 0x34);
    S16(rec, 0xC) = S16(self, 0x1584);
    rec[0xE] = self[0x15AD];
    rec[0xF] = (u8)(S16(self, 0x1588) / 10);
    rec[0x10] = self[0x15AE];
    rec[0x11] = self[0x15AF];
    rec[0x12] = self[0x15A9] == 4 ? 0 : (u8)b12;
    rec[0x13] = self[0x15B0];
    rec[0x14] = self[0x15A2];
    U32(rec, 0x18) = U32(self, 0x14C8);
    F32(rec, 0x1C) = F32(self, 0x54);
    F32(rec, 0x20) = F32(self, 0x1568);
}

void *func_0032C350(void) {
    return D_0042C6A0;
}

void *func_0032C360(void) {
    return D_0042C6E0;
}

void func_0032C5E0(u8 *self) {
    PTR(self, 0x874) = D_0042C730;
}

void *func_0032C6F0(void) {
    return D_0042C770;
}

void *func_0032C700(void) {
    return D_0042C790;
}

void *func_0032C710(void) {
    return D_0042C810;
}

void *func_0032C720(void *self, s32 i) {
    return D_0047ADB4[i];
}

void *func_0032C740(void) {
    return D_0042C860;
}

void *func_0032C750(void *self, s32 i) {
    return D_0047ADB8[i];
}

void *func_0032CAD0(void) {
    return D_0042C900;
}

void *func_0032CAE0(void) {
    return D_0042C940;
}

s32 func_0032CD00(void *self, void *dest) {
    return FILE_LOAD_ASYNC(D_00460BE0, dest);
}

void *func_0032CF10(void) {
    return D_0042C990;
}

void *func_0032CF20(void) {
    return D_0042C9D0;
}

void func_0032D270(u8 *self, s32 a, f32 x, f32 y) {
    S32(self, 0x1624) = 0;
    S32(self, 0x1628) = 1;
    S32(self, 0x162C) = 60;
    F32(self, 0x1634) = F32(self, 0x10);
    F32(self, 0x1638) = F32(self, 0x18);
    F32(self, 0x163C) = x;
    F32(self, 0x1640) = y;
    self[0x16A8] = 1;
    self[0x16A9] = 1;
    S32(self, 0x1630) = a;
}

void func_0032D3E0(u8 *self, s32 a, f32 x, f32 y) {
    S32(self, 0x1624) = 0;
    S32(self, 0x1628) = 1;
    S32(self, 0x162C) = 60;
    F32(self, 0x1634) = F32(self, 0x10);
    F32(self, 0x1638) = F32(self, 0x18);
    F32(self, 0x163C) = x;
    F32(self, 0x1640) = y;
    self[0x16A8] = 1;
    self[0x16A9] = 1;
    S32(self, 0x1630) = a;
}

void *func_0032DC50(void) {
    return D_0042CA20;
}

void *func_0032DC60(void) {
    return D_0042CB60;
}

void *func_0032DC70(void) {
    return D_0042CC00;
}

void *func_0032DC80(void) {
    return D_0042D0C0;
}

void *func_0032DC90(void *self, s32 i) {
    return D_0042E310[i];
}

void *func_0032DCB0(void) {
    return D_0042E410;
}

void *func_0032DCC0(void *self, s32 i) {
    return D_0042E3F0[i];
}

void func_0032F4E0(u8 *self) {
    S32(self, 0xFC8) = 0;
    self[0xFCC] = 0;
    S32(self, 0xFC0) = 0;
    S32(self, 0xFC4) = 0;

    S64(self, 0xC18) = -1;
    S32(self, 0xC24) = 0;
    S32(self, 0xC28) = 0;
    S32(self, 0xC2C) = 0;
    S32(self, 0xC30) = 25;
    S16(self, 0xC34) = 0x10;
    S16(self, 0xC36) = 0x20;
    S16(self, 0xC38) = 0x40;
    S16(self, 0xC3A) = 0x20;
    S16(self, 0xC3C) = 0x20;
    S16(self, 0xC3E) = 0x200;
    S16(self, 0xC40) = 0x100;
    self[0xC42] = 0x40;
    self[0xC43] = 1;
    self[0xC44] = 1;
    self[0xC45] = 0x10;
    self[0xC46] = 0xFF;

    S64(self, 0xC50) = -1;
    S32(self, 0xC5C) = 0;
    S32(self, 0xC60) = 0;
    S32(self, 0xC64) = 0;
    S32(self, 0xC68) = 25;
    S16(self, 0xC6C) = 0x10;
    S16(self, 0xC6E) = 0xE;
    S16(self, 0xC70) = 0x6E;
    S16(self, 0xC72) = 4;
    S16(self, 0xC74) = 4;
    S16(self, 0xC76) = 0x200;
    S16(self, 0xC78) = 0x100;
    self[0xC7A] = 0x40;
    self[0xC7B] = 1;
    self[0xC7C] = 1;
    self[0xC7D] = 0x10;
    self[0xC7E] = 0xFF;
}

void *func_0032F6C0(void) {
    return D_0042E4C0;
}
