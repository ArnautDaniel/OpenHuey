/* Room 0x65: its event handler class (vtable D_0046B5B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B5B0[];
extern void *D_0046DB80[];

extern u8 D_00429640[];
extern u8 D_00429700[];
extern u8 D_0047AD50[];
extern u8 D_0047AD58[];

void *func_0020B380(void *o, s32 flags) { return room_dtor(o, flags, D_0046B5B0, D_0046DB80); }

void *func_0020B3E0(void) {
    return D_0047AD50;
}

void *func_0020B3F0(void) {
    return D_00429640;
}

void *func_0020B400(void) {
    return D_00429700;
}

void *func_0020B410(void) {
    return D_0047AD58;
}
