/* Room 0x54: its event handler class (vtable Room54_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room54_vtable[];

extern u8 Room54_EnterScript_data[];
extern u8 Room54_CharEnterScript_data[];
extern u8 Room54_Phase1Script_data[];
extern u8 Room54_Phase2Script_data[];
extern void *Room54_ActionScripts[];
extern void *Room54_ObjectNames[];
extern u8 Room54_Table38_data[];

extern PTMF Room54_CmdTable[];

/* 0x0030F940 */
void *Room54_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room54_vtable, RoomBase_vtable); }

/* 0x0030F9A0 */
void *Room54_EnterScript(void) {
    return Room54_EnterScript_data;
}

/* 0x0030F9B0 */
void *Room54_CharEnterScript(void) {
    return Room54_CharEnterScript_data;
}

/* 0x0030F9C0 */
void *Room54_Phase1Script(void) {
    return Room54_Phase1Script_data;
}

/* 0x0030F9D0 */
void *Room54_Phase2Script(void) {
    return Room54_Phase2Script_data;
}

/* 0x0030F9E0 */
void *Room54_ActionScript(void *self, s32 i) {
    return Room54_ActionScripts[i];
}

/* 0x0030FA00 */
void *Room54_Table38(void) {
    return Room54_Table38_data;
}

/* 0x0030FA10 */
void *Room54_ObjectName(void *self, s32 i) {
    return Room54_ObjectNames[i];
}

/* (self->*Room54_CmdTable[i])(a, b) */
/* 0x0030FA30 */
s32 Room54_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room54_CmdTable[i & 0xFF], a, b);
}
