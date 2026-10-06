/* Room 0x57: its event handler class (vtable Room57_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room57_vtable[];
extern u8 Room57_Phase5Script_data[];

extern u8 Room57_EnterScript_data[];
extern u8 Room57_CharEnterScript_data[];
extern u8 Room57_Phase1Script_data[];
extern u8 Room57_Phase2Script_data[];
extern u32 Room57_ActionScripts[];
extern u8 Room57_Table38_data[];
extern u32 Room57_ObjectNames[];

/* 0x002B4EF0 */
void *Room57_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room57_vtable, RoomBase_vtable); }

/* 0x002B4F50 */
void *Room57_EnterScript(void) {
    return Room57_EnterScript_data;
}

/* 0x002B4F60 */
void *Room57_CharEnterScript(void) {
    return Room57_CharEnterScript_data;
}

/* 0x002B4F70 */
void *Room57_Phase1Script(void) {
    return Room57_Phase1Script_data;
}

/* 0x002B4F80 */
void *Room57_Phase2Script(void) {
    return Room57_Phase2Script_data;
}

/* 0x002B4F90 */
void *Room57_Phase5Script(void *o) { return Room57_Phase5Script_data; }   /* Room57_vtable +0x20 */

/* 0x002B4FA0 */
u32 Room57_ActionScript(void *self, s32 i) {
    return Room57_ActionScripts[i];
}

/* 0x002B4FC0 */
void *Room57_Table38(void) {
    return Room57_Table38_data;
}

/* 0x002B4FD0 */
u32 Room57_ObjectName(void *self, s32 i) {
    return Room57_ObjectNames[i];
}
