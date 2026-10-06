/* Room 0x5B: its event handler class (vtable Room5B_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room5B_vtable[];
extern void *RoomBase_vtable[];

extern u8 D_0041B020[];
extern u8 D_0041B080[];
extern u8 D_0041B100[];
extern u8 D_0041B120[];

/* 0x0020B670 */
void *Room5B_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room5B_vtable, RoomBase_vtable); }

/* 0x0020B6D0 */
void *Room5B_EnterScript(void) {
    return D_0041B020;
}

/* 0x0020B6E0 */
void *Room5B_CharEnterScript(void) {
    return D_0041B080;
}

/* 0x0020B6F0 */
void *Room5B_Phase1Script(void) {
    return D_0041B100;
}

/* 0x0020B700 */
void *Room5B_Table38(void) {
    return D_0041B120;
}
