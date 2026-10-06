/* Room 0x41: its event handler class (vtable Room41_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room41_vtable[], *RoomBase_vtable[];

extern u8 Room41_EnterScript_data[];
extern u8 Room41_CharEnterScript_data[];
extern u8 Room41_Phase1Script_data[];
extern u8 Room41_Phase2Script_data[];
extern u8 Room41_Phase3Script_data[];
extern u8 Room41_Phase5Script_data[];
extern void *Room41_ActionScripts[];
extern u8 Room41_Table38_data[];

/* 0x0020B970 */
void *Room41_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room41_vtable, RoomBase_vtable); }

/* 0x0020B9D0 */
void *Room41_EnterScript(void) {
    return Room41_EnterScript_data;
}

/* 0x0020B9E0 */
void *Room41_CharEnterScript(void) {
    return Room41_CharEnterScript_data;
}

/* 0x0020B9F0 */
void *Room41_Phase1Script(void) {
    return Room41_Phase1Script_data;
}

/* 0x0020BA00 */
void *Room41_Phase2Script(void) {
    return Room41_Phase2Script_data;
}

/* 0x0020BA10 */
void *Room41_Phase3Script(void) {
    return Room41_Phase3Script_data;
}

/* 0x0020BA20 */
void *Room41_Phase5Script(void) {
    return Room41_Phase5Script_data;
}

/* 0x0020BA30 */
void *Room41_ActionScript(void *self, s32 i) {
    return Room41_ActionScripts[i];
}

/* 0x0020BA50 */
void *Room41_Table38(void) {
    return Room41_Table38_data;
}
