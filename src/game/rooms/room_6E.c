/* Room 0x6E: its event handler class (vtable Room6E_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room6E_vtable[];

extern u32 Room6E_EnterScript_data[];
extern u32 Room6E_CharEnterScript_data[];
extern u32 Room6E_Phase1Script_data[];
extern u32 Room6E_Phase2Script_data[];
extern u32 Room6E_Phase3Script_data[];
extern u32 Room6E_ActionScripts[];
extern u32 Room6E_ObjectNames[];
extern u32 Room6E_Table38_data[];

/* 0x00344D80 */
void *Room6E_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room6E_vtable, RoomBase_vtable); }

/* 0x00344DE0 */
void *Room6E_EnterScript(void) {
    return Room6E_EnterScript_data;
}

/* 0x00344DF0 */
void *Room6E_CharEnterScript(void) {
    return Room6E_CharEnterScript_data;
}

/* 0x00344E00 */
void *Room6E_Phase1Script(void) {
    return Room6E_Phase1Script_data;
}

/* 0x00344E10 */
void *Room6E_Phase2Script(void) {
    return Room6E_Phase2Script_data;
}

/* 0x00344E20 */
void *Room6E_Phase3Script(void) {
    return Room6E_Phase3Script_data;
}

/* 0x00344E30 */
u32 Room6E_ActionScript(void *self, s32 i) {
    return Room6E_ActionScripts[i];
}

/* 0x00344E50 */
void *Room6E_Table38(void) {
    return Room6E_Table38_data;
}

/* 0x00344E60 */
u32 Room6E_ObjectName(void *self, s32 i) {
    return Room6E_ObjectNames[i];
}
