/* Room 0x68: its event handler class (vtable Room68_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room68_vtable[];

extern u8 Room68_EnterScript_data[];
extern u8 Room68_CharEnterScript_data[];
extern u8 Room68_Phase1Script_data[];
extern u8 Room68_Phase2Script_data[];
extern u8 Room68_Phase4Script_data[];
extern void *Room68_ActionScripts[];
extern void *Room68_ObjectNames[];

/* 0x0030EE90 */
void *Room68_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room68_vtable, RoomBase_vtable); }

/* 0x0030EEF0 */
void *Room68_EnterScript(void) {
    return Room68_EnterScript_data;
}

/* 0x0030EF00 */
void *Room68_CharEnterScript(void) {
    return Room68_CharEnterScript_data;
}

/* 0x0030EF10 */
void *Room68_Phase1Script(void) {
    return Room68_Phase1Script_data;
}

/* 0x0030EF20 */
void *Room68_Phase2Script(void) {
    return Room68_Phase2Script_data;
}

/* 0x0030EF30 */
void *Room68_Phase4Script(void) {
    return Room68_Phase4Script_data;
}

/* 0x0030EF40 */
void *Room68_ActionScript(void *self, s32 i) {
    return Room68_ActionScripts[i];
}

/* 0x0030EF60 */
void *Room68_ObjectName(void *self, s32 i) {
    return Room68_ObjectNames[i];
}
