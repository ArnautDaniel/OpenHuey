/* Room 0x70: its event handler class (vtable Room70_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room70_vtable[];

extern u32 Room70_EnterScript_data[];
extern u32 Room70_CharEnterScript_data[];
extern u32 Room70_Phase1Script_data[];
extern u32 Room70_Phase2Script_data[];
extern u32 Room70_Phase5Script_data[];
extern u32 Room70_ActionScripts[];
extern u32 Room70_ObjectNames[];
extern u32 Room70_Table38_data[];

/* 0x00345000 */
void *Room70_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room70_vtable, RoomBase_vtable); }

/* 0x00345060 */
void *Room70_EnterScript(void) {
    return Room70_EnterScript_data;
}

/* 0x00345070 */
void *Room70_CharEnterScript(void) {
    return Room70_CharEnterScript_data;
}

/* 0x00345080 */
void *Room70_Phase1Script(void) {
    return Room70_Phase1Script_data;
}

/* 0x00345090 */
void *Room70_Phase2Script(void) {
    return Room70_Phase2Script_data;
}

/* 0x003450A0 */
void *Room70_Phase5Script(void) {
    return Room70_Phase5Script_data;
}

/* 0x003450B0 */
u32 Room70_ActionScript(void *self, s32 i) {
    return Room70_ActionScripts[i];
}

/* 0x003450D0 */
void *Room70_Table38(void) {
    return Room70_Table38_data;
}

/* 0x003450E0 */
u32 Room70_ObjectName(void *self, s32 i) {
    return Room70_ObjectNames[i];
}
