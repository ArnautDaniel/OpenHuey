/* Room 0x101: its event handler class (vtable Room101_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room101_vtable[];
extern void *RoomBase_vtable[];

extern u8 Room101_CharEnterScript_data[];
extern u8 Room101_Phase1Script_data[];
extern u8 Room101_Table38_data[];
extern u8 Room101_EnterScript_data[];

/* destructors */
/* 0x0020B090 */
void *Room101_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room101_vtable, RoomBase_vtable); }

/* 0x0020B0F0 */
void *Room101_EnterScript(void) {
    return Room101_EnterScript_data;
}

/* 0x0020B100 */
void *Room101_CharEnterScript(void) {
    return Room101_CharEnterScript_data;
}

/* 0x0020B110 */
void *Room101_Phase1Script(void) {
    return Room101_Phase1Script_data;
}

/* 0x0020B120 */
void *Room101_Table38(void) {
    return Room101_Table38_data;
}
