/* Room 0x4A: its event handler class (vtable Room4A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room4A_vtable[];

extern u8 Room4A_EnterScript_data[];
extern u8 Room4A_CharEnterScript_data[];
extern u8 Room4A_Phase1Script_data[];
extern u8 Room4A_Phase2Script_data[];
extern u8 Room4A_Phase5Script_data[];
extern u32 Room4A_ActionScripts[];
extern u8 Room4A_Table38_data[];
extern u32 Room4A_ObjectNames[];

/* 0x002B2EF0 */
void *Room4A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room4A_vtable, RoomBase_vtable); }

/* 0x002B2F50 */
void *Room4A_EnterScript(void) {
    return Room4A_EnterScript_data;
}

/* 0x002B2F60 */
void *Room4A_CharEnterScript(void) {
    return Room4A_CharEnterScript_data;
}

/* 0x002B2F70 */
void *Room4A_Phase1Script(void) {
    return Room4A_Phase1Script_data;
}

/* 0x002B2F80 */
void *Room4A_Phase2Script(void) {
    return Room4A_Phase2Script_data;
}

/* 0x002B2F90 */
void *Room4A_Phase5Script(void) {
    return Room4A_Phase5Script_data;
}

/* 0x002B2FA0 */
u32 Room4A_ActionScript(void *self, s32 i) {
    return Room4A_ActionScripts[i];
}

/* 0x002B2FC0 */
void *Room4A_Table38(void) {
    return Room4A_Table38_data;
}

/* 0x002B2FD0 */
u32 Room4A_ObjectName(void *self, s32 i) {
    return Room4A_ObjectNames[i];
}
