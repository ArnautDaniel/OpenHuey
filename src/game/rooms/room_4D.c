/* Room 0x4D: its event handler class (vtable D_0046B6F0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B6F0[];
extern void *D_0046DB80[];

extern u8 D_0040AC70[];
extern u8 D_0040AD70[];
extern u8 D_0040AE80[];
extern u8 D_0047AB90[];

void *func_0020B710(void *o, s32 flags) { return room_dtor(o, flags, D_0046B6F0, D_0046DB80); }

void *func_0020B770(void) {
    return D_0047AB90;
}

void *func_0020B780(void) {
    return D_0040AC70;
}

void *func_0020B790(void) {
    return D_0040AD70;
}

void *func_0020B7A0(void) {
    return D_0040AE80;
}
