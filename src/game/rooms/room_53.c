/* Room 0x53: its event handler class (vtable Room53_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room53_vtable[];

extern u8 Room53_EnterScript_data[];
extern u8 Room53_CharEnterScript_data[];
extern u8 Room53_Phase1Script_data[];
extern u8 Room53_Phase2Script_data[];
extern void *Room53_ActionScripts[];
extern void *Room53_ObjectNames[];
extern u8 Room53_Table38_data[];

/* 0x0030F850 */
void *Room53_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room53_vtable, RoomBase_vtable); }

/* 0x0030F8B0 */
void *Room53_EnterScript(void) {
    return Room53_EnterScript_data;
}

/* 0x0030F8C0 */
void *Room53_CharEnterScript(void) {
    return Room53_CharEnterScript_data;
}

/* 0x0030F8D0 */
void *Room53_Phase1Script(void) {
    return Room53_Phase1Script_data;
}

/* 0x0030F8E0 */
void *Room53_Phase2Script(void) {
    return Room53_Phase2Script_data;
}

/* 0x0030F8F0 */
void *Room53_ActionScript(void *self, s32 i) {
    return Room53_ActionScripts[i];
}

/* 0x0030F910 */
void *Room53_Table38(void) {
    return Room53_Table38_data;
}

/* 0x0030F920 */
void *Room53_ObjectName(void *self, s32 i) {
    return Room53_ObjectNames[i];
}
