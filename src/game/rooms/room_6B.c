/* Room 0x6B: its event handler class (vtable Room6B_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room6B_vtable[];
extern u8 Room6B_Phase5Script_data[];

extern u32 Room6B_EnterScript_data[];
extern u32 Room6B_CharEnterScript_data[];
extern u32 Room6B_Phase1Script_data[];
extern u32 Room6B_Phase2Script_data[];
extern u32 Room6B_ActionScripts[];
extern u32 Room6B_Table38_data[];
extern u32 Room6B_ObjectNames[];

/* 0x00344870 */
void *Room6B_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room6B_vtable, RoomBase_vtable); }

/* 0x003448D0 */
void *Room6B_EnterScript(void) {
    return Room6B_EnterScript_data;
}

/* 0x003448E0 */
void *Room6B_CharEnterScript(void) {
    return Room6B_CharEnterScript_data;
}

/* 0x003448F0 */
void *Room6B_Phase1Script(void) {
    return Room6B_Phase1Script_data;
}

/* 0x00344900 */
void *Room6B_Phase2Script(void) {
    return Room6B_Phase2Script_data;
}

/* 0x00344910 */
void *Room6B_Phase5Script(void *o) { return Room6B_Phase5Script_data; }   /* Room6B_vtable +0x20 */

/* 0x00344920 */
u32 Room6B_ActionScript(void *self, s32 i) {
    return Room6B_ActionScripts[i];
}

/* 0x00344940 */
void *Room6B_Table38(void) {
    return Room6B_Table38_data;
}

/* 0x00344950 */
u32 Room6B_ObjectName(void *self, s32 i) {
    return Room6B_ObjectNames[i];
}
