/* Room 0x42: its event handler class (vtable Room42_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room42_vtable[];
extern void *RoomBase_vtable[];

extern u8 Room42_EnterScript_data[];
extern u8 Room42_CharEnterScript_data[];
extern u8 Room42_Phase1Script_data[];
extern u8 Room42_Phase2Script_data[];
extern void *Room42_ActionScripts[];
extern u8 Room42_Table38_data[];

/* 0x0020B8A0 */
void *Room42_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room42_vtable, RoomBase_vtable); }

/* 0x0020B900 */
void *Room42_EnterScript(void) {
    return Room42_EnterScript_data;
}

/* 0x0020B910 */
void *Room42_CharEnterScript(void) {
    return Room42_CharEnterScript_data;
}

/* 0x0020B920 */
void *Room42_Phase1Script(void) {
    return Room42_Phase1Script_data;
}

/* 0x0020B930 */
void *Room42_Phase2Script(void) {
    return Room42_Phase2Script_data;
}

/* 0x0020B940 */
void *Room42_ActionScript(void *self, s32 i) {
    return Room42_ActionScripts[i];
}

/* 0x0020B960 */
void *Room42_Table38(void) {
    return Room42_Table38_data;
}
