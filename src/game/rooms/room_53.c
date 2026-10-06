/* Room 0x53: its event handler class (vtable D_00471F20, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00471F20[];

extern u8 D_004268B0[];
extern u8 D_004268F0[];
extern u8 D_00426930[];
extern u8 D_00426950[];
extern void *D_00426D10[];
extern void *D_00426D40[];
extern u8 D_00426D70[];

void *func_0030F850(void *o, s32 flags) { return room_dtor(o, flags, D_00471F20, D_0046DB80); }

void *func_0030F8B0(void) {
    return D_004268B0;
}

void *func_0030F8C0(void) {
    return D_004268F0;
}

void *func_0030F8D0(void) {
    return D_00426930;
}

void *func_0030F8E0(void) {
    return D_00426950;
}

void *func_0030F8F0(void *self, s32 i) {
    return D_00426D10[i];
}

void *func_0030F910(void) {
    return D_00426D70;
}

void *func_0030F920(void *self, s32 i) {
    return D_00426D40[i];
}
