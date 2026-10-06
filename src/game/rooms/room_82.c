/* Room 0x82: its event handler class (vtable Room82_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room82_vtable[];
extern void *RoomBase_vtable[];

extern u8 Room82_EnterScript_data[];
extern u8 Room82_CharEnterScript_data[];
extern u8 Room82_Phase1Script_data[];
extern u8 Room82_Phase2Script_data[];
extern void *Room82_ActionScripts[];
extern u8 Room82_Table38_data[];

/* 0x0020B1D0 */
void *Room82_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room82_vtable, RoomBase_vtable); }

/* 0x0020B230 */
void *Room82_EnterScript(void) {
    return Room82_EnterScript_data;
}

/* 0x0020B240 */
void *Room82_CharEnterScript(void) {
    return Room82_CharEnterScript_data;
}

/* 0x0020B250 */
void *Room82_Phase1Script(void) {
    return Room82_Phase1Script_data;
}

/* 0x0020B260 */
void *Room82_Phase2Script(void) {
    return Room82_Phase2Script_data;
}

/* 0x0020B270 */
void *Room82_ActionScript(void *self, s32 i) {
    return Room82_ActionScripts[i];
}

/* 0x0020B290 */
void *Room82_Table38(void) {
    return Room82_Table38_data;
}
