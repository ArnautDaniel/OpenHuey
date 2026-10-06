/* Room 0x83: its event handler class (vtable Room83_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room83_vtable[];

extern u32 Room83_EnterScript_data[];
extern u32 Room83_CharEnterScript_data[];
extern u32 Room83_Phase1Script_data[];
extern u32 Room83_Phase2Script_data[];
extern u32 Room83_Phase3Script_data[];
extern u32 Room83_ActionScripts[];
extern u32 Room83_ObjectNames[];
extern u32 Room83_Table38_data[];

/* 0x0033F090 */
void *Room83_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room83_vtable, RoomBase_vtable); }

/* 0x0033F0F0 */
void *Room83_EnterScript(void) {
    return Room83_EnterScript_data;
}

/* 0x0033F100 */
void *Room83_CharEnterScript(void) {
    return Room83_CharEnterScript_data;
}

/* 0x0033F110 */
void *Room83_Phase1Script(void) {
    return Room83_Phase1Script_data;
}

/* 0x0033F120 */
void *Room83_Phase2Script(void) {
    return Room83_Phase2Script_data;
}

/* 0x0033F130 */
void *Room83_Phase3Script(void) {
    return Room83_Phase3Script_data;
}

/* 0x0033F140 */
u32 Room83_ActionScript(void *self, s32 i) {
    return Room83_ActionScripts[i];
}

/* 0x0033F160 */
void *Room83_Table38(void) {
    return Room83_Table38_data;
}

/* 0x0033F170 */
u32 Room83_ObjectName(void *self, s32 i) {
    return Room83_ObjectNames[i];
}
