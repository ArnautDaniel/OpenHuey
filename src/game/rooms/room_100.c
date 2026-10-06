/* Room 0x100: its event handler class (vtable Room100_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room100_vtable[];
extern void *RoomBase_vtable[];

extern u8 Room100_CharEnterScript_data[];
extern u8 Room100_Phase1Script_data[];
extern u8 Room100_Table38_data[];
extern u8 Room100_EnterScript_data[];

/* 0x0020B130 */
void *Room100_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room100_vtable, RoomBase_vtable); }

/* 0x0020B190 */
void *Room100_EnterScript(void) {
    return Room100_EnterScript_data;
}

/* 0x0020B1A0 */
void *Room100_CharEnterScript(void) {
    return Room100_CharEnterScript_data;
}

/* 0x0020B1B0 */
void *Room100_Phase1Script(void) {
    return Room100_Phase1Script_data;
}

/* 0x0020B1C0 */
void *Room100_Table38(void) {
    return Room100_Table38_data;
}
