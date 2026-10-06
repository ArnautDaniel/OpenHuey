/* Room 0x8E: its event handler class (vtable Room8E_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "msl.h"

extern void *RoomBase_vtable[];
extern void *Room8E_vtable[];
extern const char *D_00435978;

extern u32 Room8E_EnterScript_data[];
extern u32 Room8E_CharEnterScript_data[];
extern u32 Room8E_Phase1Script_data[];
extern u32 Room8E_Phase2Script_data[];
extern u32 Room8E_ActionScripts[];
extern u32 Room8E_ObjectNames[];

extern PTMF Room8E_CmdTable[];

/* 0x003409C0 */
void *Room8E_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room8E_vtable, RoomBase_vtable); }

/* 0x00340A20 */
void *Room8E_EnterScript(void) {
    return Room8E_EnterScript_data;
}

/* 0x00340A30 */
void *Room8E_CharEnterScript(void) {
    return Room8E_CharEnterScript_data;
}

/* 0x00340A40 */
void *Room8E_Phase1Script(void) {
    return Room8E_Phase1Script_data;
}

/* 0x00340A50 */
void *Room8E_Phase2Script(void) {
    return Room8E_Phase2Script_data;
}

/* 0x00340A60 */
u32 Room8E_ActionScript(void *self, s32 i) {
    return Room8E_ActionScripts[i];
}

/* 0x00340A80 */
u32 Room8E_ObjectName(void *self, s32 i) {
    return Room8E_ObjectNames[i];
}

/* (self->*Room8E_CmdTable[i])(a, b) */
/* 0x00340AA0 */
s32 Room8E_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room8E_CmdTable[i & 0xFF], a, b);
}

/* the room object named D_00435978 falling over: byte 3 0 starts it (angle +0x30, speed +0x34
 * and acceleration +0x38 0, jerk +0x3C 0.005); 1 steps them, its tilt +0x10 = (1 - sin(90 -
 * angle)) * pi/2, waiting (2) until the angle reaches 90; 2 puts it down (sin(pi/2)) */
/* 0x00340AD0 */
s32 Room8E_Cmd01(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kHalfPi = {0x3FC90FDB};
    u8 *o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, D_00435978);
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
/* 0x00340C50 */
s32 Room8E_Cmd00(void *self, void *a1, u8 *cmd) {
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
