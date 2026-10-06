/* Room 0x101: its event handler class (vtable Room101_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room101_vtable[];
extern void *RoomBase_vtable[];

extern u8 D_00417320[];
extern u8 D_00417420[];
extern u8 D_004174A0[];
extern u8 D_0047AC58[];

/* destructors */
/* 0x0020B090 */
void *Room101_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room101_vtable, RoomBase_vtable); }

/* 0x0020B0F0 */
void *Room101_EnterScript(void) {
    return D_0047AC58;
}

/* 0x0020B100 */
void *Room101_CharEnterScript(void) {
    return D_00417320;
}

/* 0x0020B110 */
void *Room101_Phase1Script(void) {
    return D_00417420;
}

/* 0x0020B120 */
void *Room101_Table38(void) {
    return D_004174A0;
}
