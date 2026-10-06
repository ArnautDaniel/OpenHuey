/* Room 0x1B: its event handler class (vtable Room1B_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "msl.h"

extern void *RoomBase_vtable[];
extern void *Room1B_vtable[];
extern const char *Room1B_ObjectNames[];

extern u8 Room1B_EnterScript_data[];
extern u8 Room1B_CharEnterScript_data[];
extern u8 Room1B_Phase1Script_data[];
extern u8 Room1B_Phase2Script_data[];
extern void *Room1B_ActionScripts[];
extern u8 Room1B_Table38_data[];
extern PTMF Room1B_CmdTable[];

/* 0x002AD690 */
void *Room1B_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room1B_vtable, RoomBase_vtable); }

/* 0x002AD6F0 */
void *Room1B_EnterScript(void) {
    return Room1B_EnterScript_data;
}

/* 0x002AD700 */
void *Room1B_CharEnterScript(void) {
    return Room1B_CharEnterScript_data;
}

/* 0x002AD710 */
void *Room1B_Phase1Script(void) {
    return Room1B_Phase1Script_data;
}

/* 0x002AD720 */
void *Room1B_Phase2Script(void) {
    return Room1B_Phase2Script_data;
}

/* 0x002AD730 */
void *Room1B_ActionScript(void *self, s32 i) {
    return Room1B_ActionScripts[i];
}

/* 0x002AD750 */
void *Room1B_Table38(void) {
    return Room1B_Table38_data;
}

/* 0x002AD760 */
void *Room1B_ObjectName(void *self, s32 i) {
    return (void *)Room1B_ObjectNames[i];
}

/* (self->*Room1B_CmdTable[i])(a, b) */
/* 0x002AD780 */
s32 Room1B_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room1B_CmdTable[i & 0xFF], a, b);
}

/* five pendulums (Room1B_ObjectNames[byte 3]) of their own periods and swings: byte 4 0 still at a phase
 * offset (+0x34) 60 x the index, 1 swinging on (+0x30, +0x14) */
/* 0x002AD7B0 */
s32 Room1B_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    u8 *o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, Room1B_ObjectNames[cmd[3]]);
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
