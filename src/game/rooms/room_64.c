/* Room 0x64: its event handler class (vtable D_0046B5F0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B5F0[];
extern void *D_0046DB80[];

extern u8 D_004295A0[];
extern u8 D_00429620[];
extern u8 D_0047AD44[];
extern u8 D_0047AD48[];

void *func_0020B420(void *o, s32 flags) { return room_dtor(o, flags, D_0046B5F0, D_0046DB80); }

void *func_0020B480(void) {
    return D_0047AD44;
}

void *func_0020B490(void) {
    return D_004295A0;
}

void *func_0020B4A0(void) {
    return D_00429620;
}

void *func_0020B4B0(void) {
    return D_0047AD48;
}
