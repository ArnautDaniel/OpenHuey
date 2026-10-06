/* Room 0x8E: its event handler class (vtable D_00477400, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477400[];
extern const char *D_00435978;

extern u32 D_00435050[];
extern u32 D_00435080[];
extern u32 D_004350E0[];
extern u32 D_00435200[];
extern u32 D_00435920[];
extern u32 D_00435970[];

extern PTMF D_01991790[];

void *func_003409C0(void *o, s32 flags) { return room_dtor(o, flags, D_00477400, D_0046DB80); }

void *func_00340A20(void) {
    return D_00435050;
}

void *func_00340A30(void) {
    return D_00435080;
}

void *func_00340A40(void) {
    return D_004350E0;
}

void *func_00340A50(void) {
    return D_00435200;
}

u32 func_00340A60(void *self, s32 i) {
    return D_00435920[i];
}

u32 func_00340A80(void *self, s32 i) {
    return D_00435970[i];
}

/* (self->*D_01991790[i])(a, b) */
s32 func_00340AA0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991790[i & 0xFF], a, b);
}

/* the room object named D_00435978 falling over: byte 3 0 starts it (angle +0x30, speed +0x34
 * and acceleration +0x38 0, jerk +0x3C 0.005); 1 steps them, its tilt +0x10 = (1 - sin(90 -
 * angle)) * pi/2, waiting (2) until the angle reaches 90; 2 puts it down (sin(pi/2)) */
s32 func_00340AD0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kHalfPi = {0x3FC90FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_00435978);
    f32 a;

    switch (cmd[3]) {
    case 0:
        AT(o, 0x30, s32) = 0;
        AT(o, 0x34, s32) = 0;
        AT(o, 0x38, s32) = 0;
        AT(o, 0x3C, u32) = 0x3BA3D70A;   /* 0.005 */
        return 1;
    case 1:
        AT(o, 0x38, f32) = AT(o, 0x38, f32) + AT(o, 0x3C, f32);
        AT(o, 0x34, f32) = AT(o, 0x34, f32) + AT(o, 0x38, f32);
        AT(o, 0x30, f32) = AT(o, 0x30, f32) + AT(o, 0x34, f32);
        a = AT(o, 0x30, f32) < 90.0f ? AT(o, 0x30, f32) : 90.0f;
        AT(o, 0x10, f32) = (1.0f - func_0031C248(kPi.f * (90.0f - a) / 180.0f)) * kHalfPi.f;
        return AT(o, 0x30, f32) < 90.0f ? 2 : 1;
    case 2:
        AT(o, 0x10, f32) = func_0031C248(kHalfPi.f);
        return 1;
    }
    return 1;
}

/* door 0 swung by script variable 0: byte 3 0 sets it to -90; 1 opens it 10 degrees a step to 0
 * (doors +0x74), waiting (2) until there */
s32 func_00340C50(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    VObject *ev;
    s32 a;

    switch (cmd[3]) {
    case 0:
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, -90);
        return 1;
    case 1:
        ev = gEvents;
        a = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0) + 10;
        if (a >= 0) {
            a = 0;
        }
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, a);
        VCALL(gDoors, 0x74, void (*)(VObject *, s32, f32))(gDoors, 0, kPi.f * (f32)a / 180.0f);
        return a < 0 ? 2 : 1;
    }
    return 1;
}
