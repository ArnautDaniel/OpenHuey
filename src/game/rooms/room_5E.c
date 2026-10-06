/* Room 0x5E: its event handler class (vtable D_0046B670, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B670[];
extern void *D_0046DB80[];

extern u8 D_00412410[];
extern u8 D_00412430[];
extern u8 D_00412510[];
extern u8 D_004125E0[];
extern u8 D_00412688[];
extern void *D_0047ABF0[];

void *func_0020B5A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B670, D_0046DB80); }

void *func_0020B600(void) {
    return D_00412410;
}

void *func_0020B610(void) {
    return D_00412430;
}

void *func_0020B620(void) {
    return D_00412510;
}

void *func_0020B630(void) {
    return D_004125E0;
}

void *func_0020B640(void *self, s32 i) {
    return D_0047ABF0[i];
}

void *func_0020B660(void) {
    return D_00412688;
}
