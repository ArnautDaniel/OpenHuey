/* Room 0x82: its event handler class (vtable D_0046B530, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B530[];
extern void *D_0046DB80[];

extern u8 D_004314B0[];
extern u8 D_004315E0[];
extern u8 D_00431750[];
extern u8 D_00431A60[];
extern void *D_00431DB0[];
extern u8 D_00431DD0[];

void *func_0020B1D0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B530, D_0046DB80); }

void *func_0020B230(void) {
    return D_004314B0;
}

void *func_0020B240(void) {
    return D_004315E0;
}

void *func_0020B250(void) {
    return D_00431750;
}

void *func_0020B260(void) {
    return D_00431A60;
}

void *func_0020B270(void *self, s32 i) {
    return D_00431DB0[i];
}

void *func_0020B290(void) {
    return D_00431DD0;
}
