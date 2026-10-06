/* Room 0x1A: its event handler class (vtable Room1A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room1A_vtable[];
extern void *TvScreenB_vtable[];

extern u8 Room1A_EnterScript_data[];
extern u8 Room1A_CharEnterScript_data[];
extern u8 Room1A_Phase1Script_data[];
extern u8 Room1A_Phase2Script_data[];
extern u8 Room1A_Phase3Script_data[];
extern void *Room1A_ActionScripts[];
extern void *Room1A_ObjectNames[];
extern u8 Room1A_Table38_data[];
extern PTMF Room1A_CmdTable[];

/* 0x002AD420 */
void *Room1A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room1A_vtable, RoomBase_vtable); }

/* 0x002AD480 */
void *Room1A_EnterScript(void) {
    return Room1A_EnterScript_data;
}

/* 0x002AD490 */
void *Room1A_CharEnterScript(void) {
    return Room1A_CharEnterScript_data;
}

/* 0x002AD4A0 */
void *Room1A_Phase1Script(void) {
    return Room1A_Phase1Script_data;
}

/* 0x002AD4B0 */
void *Room1A_Phase2Script(void) {
    return Room1A_Phase2Script_data;
}

/* 0x002AD4C0 */
void *Room1A_Phase3Script(void) {
    return Room1A_Phase3Script_data;
}

/* 0x002AD4D0 */
void *Room1A_ActionScript(void *self, s32 i) {
    return Room1A_ActionScripts[i];
}

/* 0x002AD4F0 */
void *Room1A_Table38(void) {
    return Room1A_Table38_data;
}

/* 0x002AD500 */
void *Room1A_ObjectName(void *self, s32 i) {
    return Room1A_ObjectNames[i];
}

/* (self->*Room1A_CmdTable[i])(a, b) */
/* 0x002AD520 */
s32 Room1A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room1A_CmdTable[i & 0xFF], a, b);
}

/* 0x002AD550 */
s32 Room1A_Cmd01(void) {   /* room effect 0 (TvScreenB_vtable) */
    room_effect_slot_new(gRoomEffects, 0, TvScreenB_vtable);
    return 1;
}

/* room 0x1A (D_003FC660): the fan turns */
/* 0x002AD600 */
s32 Room1A_Cmd00(void) {
    fan_turn(pstr_fan);
    return 1;
}
