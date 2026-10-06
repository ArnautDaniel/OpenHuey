/* Room 0x1B: its event handler class (vtable D_0046E140, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "msl.h"

extern void *D_0046DB80[];
extern void *D_0046E140[];
extern const char *D_003FCB00[];

extern u8 D_003FC6A0[];
extern u8 D_003FC6F0[];
extern u8 D_003FC770[];
extern u8 D_003FC8F0[];
extern void *D_003FCAD0[];
extern u8 D_003FCB18[];
extern PTMF D_01990978[];

void *func_002AD690(void *o, s32 flags) { return room_dtor(o, flags, D_0046E140, D_0046DB80); }

void *func_002AD6F0(void) {
    return D_003FC6A0;
}

void *func_002AD700(void) {
    return D_003FC6F0;
}

void *func_002AD710(void) {
    return D_003FC770;
}

void *func_002AD720(void) {
    return D_003FC8F0;
}

void *func_002AD730(void *self, s32 i) {
    return D_003FCAD0[i];
}

void *func_002AD750(void) {
    return D_003FCB18;
}

void *func_002AD760(void *self, s32 i) {
    return (void *)D_003FCB00[i];
}

/* (self->*D_01990978[i])(a, b) */
s32 func_002AD780(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990978[i & 0xFF], a, b);
}

/* five pendulums (D_003FCB00[byte 3]) of their own periods and swings: byte 4 0 still at a phase
 * offset (+0x34) 60 x the index, 1 swinging on (+0x30, +0x14) */
s32 func_002AD7B0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003FCB00[cmd[3]]);
    f32 period = 360.0f, swing = 15.0f;

    switch (cmd[3]) {
    case 2:
        break;
    case 0:
        period = 180.0f;
        swing = 5.0f;
        break;
    case 1:
        period = 210.0f;
        swing = 5.0f;
        break;
    case 3:
        period = 180.0f;
        swing = 10.0f;
        break;
    case 4:
        period = 240.0f;
        break;
    }
    switch (cmd[4]) {
    case 0:
        AT(o, 0x30, f32) = 0.0f;
        AT(o, 0x34, f32) = 60.0f * (f32)(u32)cmd[3];
        break;
    case 1:
        AT(o, 0x30, f32) = AT(o, 0x30, f32) + 360.0f / period;
        if (!(AT(o, 0x30, f32) + AT(o, 0x34, f32) < 360.0f)) {
            AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
        }
        AT(o, 0x14, f32) = kPi.f * (swing * func_0031C248(kPi.f * (AT(o, 0x30, f32) + AT(o, 0x34, f32)) / 180.0f)) / 180.0f;
        if (!(AT(o, 0x14, f32) <= kPi.f)) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - kTwoPi.f;
        }
        break;
    }
    return 1;
}
