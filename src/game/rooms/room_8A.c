/* Room 0x8A: its event handler class (vtable Room8A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room8A_vtable[];

extern u32 Room8A_EnterScript_data[];
extern u32 Room8A_CharEnterScript_data[];
extern u32 Room8A_Phase1Script_data[];
extern u32 Room8A_Phase2Script_data[];
extern u32 Room8A_ActionScripts[];
extern u32 Room8A_Table38_data[];
extern u32 Room8A_ObjectNames[];

/* 0x0033F840 */
void *Room8A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room8A_vtable, RoomBase_vtable); }

/* 0x0033F8A0 */
void *Room8A_EnterScript(void) {
    return Room8A_EnterScript_data;
}

/* 0x0033F8B0 */
void *Room8A_CharEnterScript(void) {
    return Room8A_CharEnterScript_data;
}

/* 0x0033F8C0 */
void *Room8A_Phase1Script(void) {
    return Room8A_Phase1Script_data;
}

/* 0x0033F8D0 */
void *Room8A_Phase2Script(void) {
    return Room8A_Phase2Script_data;
}

/* 0x0033F8E0 */
u32 Room8A_ActionScript(void *self, s32 i) {
    return Room8A_ActionScripts[i];
}

/* 0x0033F900 */
void *Room8A_Table38(void) {
    return Room8A_Table38_data;
}

/* 0x0033F910 */
u32 Room8A_ObjectName(void *self, s32 i) {
    return Room8A_ObjectNames[i];
}
