/* Room 0x16: its event handler class (vtable Room16_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room16_vtable[], *RoomBase_vtable[];

extern u8 Room16_EnterScript_data[];
extern u8 Room16_CharEnterScript_data[];
extern u8 Room16_Phase1Script_data[];
extern u8 Room16_Phase2Script_data[];
extern u8 Room16_Phase3Script_data[];
extern u8 Room16_Table38_data[];
extern void *Room16_ActionScripts[];

/* 0x0020BA60 */
void *Room16_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room16_vtable, RoomBase_vtable); }

/* 0x0020BAC0 */
void *Room16_EnterScript(void) {
    return Room16_EnterScript_data;
}

/* 0x0020BAD0 */
void *Room16_CharEnterScript(void) {
    return Room16_CharEnterScript_data;
}

/* 0x0020BAE0 */
void *Room16_Phase1Script(void) {
    return Room16_Phase1Script_data;
}

/* 0x0020BAF0 */
void *Room16_Phase2Script(void) {
    return Room16_Phase2Script_data;
}

/* 0x0020BB00 */
void *Room16_Phase3Script(void) {
    return Room16_Phase3Script_data;
}

/* 0x0020BB10 */
void *Room16_ActionScript(void *self, s32 i) {
    return Room16_ActionScripts[i];
}

/* 0x0020BB30 */
void *Room16_Table38(void) {
    return Room16_Table38_data;
}
