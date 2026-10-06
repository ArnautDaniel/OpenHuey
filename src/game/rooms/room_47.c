/* Room 0x47: its event handler class (vtable Room47_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room47_vtable[];

extern u8 Room47_EnterScript_data[];
extern u8 Room47_CharEnterScript_data[];
extern u8 Room47_Phase1Script_data[];
extern u8 Room47_Phase2Script_data[];
extern void *Room47_ActionScripts[];
extern u8 Room47_Table38_data[];
extern void *Room47_ObjectNames[];

/* 0x0030EF80 */
void *Room47_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room47_vtable, RoomBase_vtable); }

/* 0x0030EFE0 */
void *Room47_EnterScript(void) {
    return Room47_EnterScript_data;
}

/* 0x0030EFF0 */
void *Room47_CharEnterScript(void) {
    return Room47_CharEnterScript_data;
}

/* 0x0030F000 */
void *Room47_Phase1Script(void) {
    return Room47_Phase1Script_data;
}

/* 0x0030F010 */
void *Room47_Phase2Script(void) {
    return Room47_Phase2Script_data;
}

/* 0x0030F020 */
void *Room47_ActionScript(void *self, s32 i) {
    return Room47_ActionScripts[i];
}

/* 0x0030F040 */
void *Room47_Table38(void) {
    return Room47_Table38_data;
}

/* 0x0030F050 */
void *Room47_ObjectName(void *self, s32 i) {
    return Room47_ObjectNames[i];
}
