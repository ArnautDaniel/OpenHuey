/* Room 0x65: its event handler class (vtable Room65_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room65_vtable[];
extern void *RoomBase_vtable[];

extern u8 Room65_CharEnterScript_data[];
extern u8 Room65_Phase1Script_data[];
extern u8 Room65_EnterScript_data[];
extern u8 Room65_Table38_data[];

/* 0x0020B380 */
void *Room65_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room65_vtable, RoomBase_vtable); }

/* 0x0020B3E0 */
void *Room65_EnterScript(void) {
    return Room65_EnterScript_data;
}

/* 0x0020B3F0 */
void *Room65_CharEnterScript(void) {
    return Room65_CharEnterScript_data;
}

/* 0x0020B400 */
void *Room65_Phase1Script(void) {
    return Room65_Phase1Script_data;
}

/* 0x0020B410 */
void *Room65_Table38(void) {
    return Room65_Table38_data;
}
