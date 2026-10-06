/* Room 0x5B: its event handler class (vtable D_0046B6B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B6B0[];
extern void *D_0046DB80[];

extern u8 D_0041B020[];
extern u8 D_0041B080[];
extern u8 D_0041B100[];
extern u8 D_0041B120[];

void *func_0020B670(void *o, s32 flags) { return room_dtor(o, flags, D_0046B6B0, D_0046DB80); }

void *func_0020B6D0(void) {
    return D_0041B020;
}

void *func_0020B6E0(void) {
    return D_0041B080;
}

void *func_0020B6F0(void) {
    return D_0041B100;
}

void *func_0020B700(void) {
    return D_0041B120;
}
