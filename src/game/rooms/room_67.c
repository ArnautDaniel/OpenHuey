/* Room 0x67: its event handler class (vtable Room67_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room67_vtable[];
extern void *RoomBase_vtable[];

extern u8 Room67_EnterScript_data[];
extern u8 Room67_CharEnterScript_data[];
extern u8 Room67_Phase1Script_data[];
extern u8 Room67_Phase5Script_data[];
extern u8 Room67_Phase2Script_data[];
extern void *Room67_ActionScripts[];
extern u8 Room67_Table38_data[];

/* 0x0020B2A0 */
void *Room67_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room67_vtable, RoomBase_vtable); }

/* 0x0020B300 */
void *Room67_EnterScript(void) {
    return Room67_EnterScript_data;
}

/* 0x0020B310 */
void *Room67_CharEnterScript(void) {
    return Room67_CharEnterScript_data;
}

/* 0x0020B320 */
void *Room67_Phase1Script(void) {
    return Room67_Phase1Script_data;
}

/* 0x0020B330 */
void *Room67_Phase2Script(void) {
    return Room67_Phase2Script_data;
}

/* 0x0020B340 */
void *Room67_Phase5Script(void) {
    return Room67_Phase5Script_data;
}

/* 0x0020B350 */
void *Room67_ActionScript(void *self, s32 i) {
    return Room67_ActionScripts[i];
}

/* 0x0020B370 */
void *Room67_Table38(void) {
    return Room67_Table38_data;
}
