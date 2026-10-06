/* Room 0x05: its event handler class (vtable Room05_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room05_vtable[], *RoomBase_vtable[];

extern u8 Room05_EnterScript_data[];
extern u8 Room05_CharEnterScript_data[];
extern u8 Room05_Phase1Script_data[];
extern u8 Room05_Phase2Script_data[];
extern u8 Room05_Phase5Script_data[];
extern void *Room05_ActionScripts[];

/* 0x0020BC10 */
void *Room05_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room05_vtable, RoomBase_vtable); }

/* 0x0020BC70 */
void *Room05_EnterScript(void) {
    return Room05_EnterScript_data;
}

/* 0x0020BC80 */
void *Room05_CharEnterScript(void) {
    return Room05_CharEnterScript_data;
}

/* 0x0020BC90 */
void *Room05_Phase1Script(void) {
    return Room05_Phase1Script_data;
}

/* 0x0020BCA0 */
void *Room05_Phase2Script(void) {
    return Room05_Phase2Script_data;
}

/* 0x0020BCB0 */
void *Room05_Phase5Script(void) {
    return Room05_Phase5Script_data;
}

/* 0x0020BCC0 */
void *Room05_ActionScript(void *self, s32 i) {
    return Room05_ActionScripts[i];
}
