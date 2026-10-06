/* Room 0x4D: its event handler class (vtable Room4D_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room4D_vtable[];
extern void *RoomBase_vtable[];

extern u8 Room4D_CharEnterScript_data[];
extern u8 Room4D_Phase1Script_data[];
extern u8 Room4D_Table38_data[];
extern u8 Room4D_EnterScript_data[];

/* 0x0020B710 */
void *Room4D_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room4D_vtable, RoomBase_vtable); }

/* 0x0020B770 */
void *Room4D_EnterScript(void) {
    return Room4D_EnterScript_data;
}

/* 0x0020B780 */
void *Room4D_CharEnterScript(void) {
    return Room4D_CharEnterScript_data;
}

/* 0x0020B790 */
void *Room4D_Phase1Script(void) {
    return Room4D_Phase1Script_data;
}

/* 0x0020B7A0 */
void *Room4D_Table38(void) {
    return Room4D_Table38_data;
}
