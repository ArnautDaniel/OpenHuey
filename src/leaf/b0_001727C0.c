/* Leaf functions (batch 0), 0x001727C0 - 0x001762A0: room object constructors.
 * All share one shape: base class vtable 0x469C20, then 0x469C60 (fields +0x20 arg,
 * +0x24 = 0x2000000, +0x1380 = 0), then the room class vtable and its id byte at +0x153C. */
#include "common.h"

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern void *D_00469C20[];
extern void *D_00469C60[];
extern void *D_00478FF0[];
extern void *D_00477790[];
extern void *D_00476C10[];
extern void *D_004767D0[];
extern void *D_004764A0[];
extern void *D_00476120[];
extern void *D_00475DB0[];
extern void *D_00474C10[];
extern void *D_00474130[];
extern void *D_00473CD0[];
extern void *D_00478770[];
extern void *D_004738A0[];
extern void *D_0046A620[];
extern void *D_00474560[];
extern void *D_00474890[];
extern void *D_004734A0[];
extern void *D_00473110[];
extern void *D_00472C30[];
extern void *D_004728A0[];
extern void *D_0046A2F0[];
extern void *D_004723B0[];
extern void *D_00472020[];
extern void *D_0046FF80[];
extern void *D_004718F0[];
extern void *D_00470720[];
extern void *D_00479B20[];
extern void *D_004715C0[];
extern void *D_00471290[];
extern void *D_0046F6B0[];
extern void *D_00478160[];
extern void *D_00477E30[];
extern void *D_00477AE0[];
extern void *D_0046BBB0[];
extern void *D_00474FD0[];
extern void *D_0046F020[];
extern void *D_00470A90[];
extern void *D_00469D10[];

static inline void *b0_RoomCtor(void *p, u32 id, s32 arg, void **vtbl) {
    FLD(p, 0x0, void **) = D_00469C20;
    FLD(p, 0x20, s32) = arg;
    FLD(p, 0x24, s32) = 0x2000000;
    FLD(p, 0x0, void **) = D_00469C60;
    FLD(p, 0x1380, s32) = 0;
    FLD(p, 0x153C, u8) = (u8)id;
    FLD(p, 0x0, void **) = vtbl;
    return p;
}

void *func_001727C0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x26, arg, D_00478FF0);
}

void *func_00172910(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x21, arg, D_00477790);
}

void *func_00172960(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x20, arg, D_00476C10);
}

void *func_001729B0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1F, arg, D_004767D0);
}

void *func_00172A00(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1E, arg, D_004764A0);
}

void *func_00172A50(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1D, arg, D_00476120);
}

void *func_00172AA0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1C, arg, D_00475DB0);
}

void *func_00172AF0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1A, arg, D_00474C10);
}

void *func_00172B40(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x19, arg, D_00474130);
}

void *func_00172B90(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x18, arg, D_00473CD0);
}

void *func_00172BE0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x25, arg, D_00478770);
}

void *func_00172C30(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x17, arg, D_004738A0);
}

void *func_00172C80(void *p, u32 id, u32 arg) {
    return b0_RoomCtor(p, id, (u8)arg, D_0046A620);
}

void *func_00172DE0(void *p, s32 arg, u32 id) {
    return b0_RoomCtor(p, id, arg, D_00474560);
}

void *func_00172E20(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x14, arg, D_00474890);
}

void *func_00172E70(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x13, arg, D_004734A0);
}

void *func_00172EC0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x12, arg, D_00473110);
}

void *func_00172F10(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x11, arg, D_00472C30);
}

void *func_00172F60(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x10, arg, D_004728A0);
}

void *func_00172FB0(void *p, u32 id, u32 arg) {
    return b0_RoomCtor(p, id, (u8)arg, D_0046A2F0);
}

void *func_00173110(void *p, s32 arg, u32 id) {
    return b0_RoomCtor(p, id, arg, D_004723B0);
}

void *func_00173150(void *p, s32 arg) {
    return b0_RoomCtor(p, 0xD, arg, D_00472020);
}

void *func_001731A0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x8, arg, D_0046FF80);
}

void *func_001731F0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0xC, arg, D_004718F0);
}

void *func_00173240(void *p, s32 arg) {
    return b0_RoomCtor(p, 0xB, arg, D_00470720);
}

void *func_00173290(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x27, arg, D_00479B20);
}

void *func_001732E0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0xA, arg, D_004715C0);
}

void *func_00173330(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x9, arg, D_00471290);
}

void *func_00173380(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x4, arg, D_0046F6B0);
}

void *func_001733D0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x24, arg, D_00478160);
}

void *func_00173420(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x23, arg, D_00477E30);
}

void *func_00173470(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x22, arg, D_00477AE0);
}

void *func_001734C0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x3, arg, D_0046BBB0);
}

void *func_00173510(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1B, arg, D_00474FD0);
}

void *func_00173560(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x7, arg, D_0046F020);
}

void *func_001735B0(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x6, arg, D_00470A90);
}

void *func_00173600(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x2, arg, D_00469D10);
}

