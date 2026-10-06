/* Room 0x101: its event handler class (vtable D_0046B4B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B4B0[];
extern void *D_0046DB80[];

extern u8 D_00417320[];
extern u8 D_00417420[];
extern u8 D_004174A0[];
extern u8 D_0047AC58[];

/* destructors */
void *func_0020B090(void *o, s32 flags) { return room_dtor(o, flags, D_0046B4B0, D_0046DB80); }

void *func_0020B0F0(void) {
    return D_0047AC58;
}

void *func_0020B100(void) {
    return D_00417320;
}

void *func_0020B110(void) {
    return D_00417420;
}

void *func_0020B120(void) {
    return D_004174A0;
}
