/* Room 0x01: its event handler class (vtable Room01_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room01_vtable[], *RoomBase_vtable[];

extern u8 Room01_EnterScript_data[];
extern u8 Room01_CharEnterScript_data[];
extern u8 Room01_Phase1Script_data[];
extern u8 Room01_Phase2Script_data[];
extern u8 Room01_Phase3Script_data[];
extern u8 Room01_Phase5Script_data[];
extern void *Room01_ActionScripts[];
extern u8 Room01_Table38_data[];

/* 0x0020BCF0 */
void *Room01_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room01_vtable, RoomBase_vtable); }

/* 0x0020BD50 */
void *Room01_EnterScript(void) {
    return Room01_EnterScript_data;
}

/* 0x0020BD60 */
void *Room01_CharEnterScript(void) {
    return Room01_CharEnterScript_data;
}

/* 0x0020BD70 */
void *Room01_Phase1Script(void) {
    return Room01_Phase1Script_data;
}

/* 0x0020BD80 */
void *Room01_Phase2Script(void) {
    return Room01_Phase2Script_data;
}

/* 0x0020BD90 */
void *Room01_Phase3Script(void) {
    return Room01_Phase3Script_data;
}

/* 0x0020BDA0 */
void *Room01_Phase5Script(void) {
    return Room01_Phase5Script_data;
}

/* 0x0020BDB0 */
void *Room01_ActionScript(void *self, s32 i) {
    return Room01_ActionScripts[i];
}

/* 0x0020BDD0 */
void *Room01_Table38(void) {
    return Room01_Table38_data;
}
