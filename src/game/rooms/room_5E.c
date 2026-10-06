/* Room 0x5E: its event handler class (vtable Room5E_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room5E_vtable[];
extern void *RoomBase_vtable[];

extern u8 Room5E_EnterScript_data[];
extern u8 Room5E_CharEnterScript_data[];
extern u8 Room5E_Phase1Script_data[];
extern u8 Room5E_Phase2Script_data[];
extern u8 Room5E_Table38_data[];
extern void *Room5E_ActionScripts[];

/* 0x0020B5A0 */
void *Room5E_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room5E_vtable, RoomBase_vtable); }

/* 0x0020B600 */
void *Room5E_EnterScript(void) {
    return Room5E_EnterScript_data;
}

/* 0x0020B610 */
void *Room5E_CharEnterScript(void) {
    return Room5E_CharEnterScript_data;
}

/* 0x0020B620 */
void *Room5E_Phase1Script(void) {
    return Room5E_Phase1Script_data;
}

/* 0x0020B630 */
void *Room5E_Phase2Script(void) {
    return Room5E_Phase2Script_data;
}

/* 0x0020B640 */
void *Room5E_ActionScript(void *self, s32 i) {
    return Room5E_ActionScripts[i];
}

/* 0x0020B660 */
void *Room5E_Table38(void) {
    return Room5E_Table38_data;
}
