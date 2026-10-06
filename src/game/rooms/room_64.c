/* Room 0x64: its event handler class (vtable Room64_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room64_vtable[];
extern void *RoomBase_vtable[];

extern u8 Room64_CharEnterScript_data[];
extern u8 Room64_Phase1Script_data[];
extern u8 Room64_EnterScript_data[];
extern u8 Room64_Table38_data[];

/* 0x0020B420 */
void *Room64_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room64_vtable, RoomBase_vtable); }

/* 0x0020B480 */
void *Room64_EnterScript(void) {
    return Room64_EnterScript_data;
}

/* 0x0020B490 */
void *Room64_CharEnterScript(void) {
    return Room64_CharEnterScript_data;
}

/* 0x0020B4A0 */
void *Room64_Phase1Script(void) {
    return Room64_Phase1Script_data;
}

/* 0x0020B4B0 */
void *Room64_Table38(void) {
    return Room64_Table38_data;
}
