/* Room 0x44: its event handler class (vtable D_0046B730, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B730[];
extern void *D_0046DB80[];

extern u8 D_00415BA0[];
extern u8 D_00415C90[];
extern u8 D_00415DB0[];
extern u8 D_00415FA0[];
extern u8 D_00416030[];
extern u8 D_00416130[];
extern void *D_004164C0[];
extern u8 D_004164D0[];

void *func_0020B7B0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B730, D_0046DB80); }

void *func_0020B810(void) {
    return D_00415BA0;
}

void *func_0020B820(void) {
    return D_00415C90;
}

void *func_0020B830(void) {
    return D_00415DB0;
}

void *func_0020B840(void) {
    return D_00415FA0;
}

void *func_0020B850(void) {
    return D_00416030;
}

void *func_0020B860(void) {
    return D_00416130;
}

void *func_0020B870(void *self, s32 i) {
    return D_004164C0[i];
}

void *func_0020B890(void) {
    return D_004164D0;
}
