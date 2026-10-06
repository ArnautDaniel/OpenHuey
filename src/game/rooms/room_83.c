/* Room 0x83: its event handler class (vtable D_00477200, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477200[];

extern u32 D_00431DE0[];
extern u32 D_00431E20[];
extern u32 D_00431E80[];
extern u32 D_00431EF0[];
extern u32 D_00431F90[];
extern u32 D_004323B0[];
extern u32 D_004323F0[];
extern u32 D_00432408[];

void *func_0033F090(void *o, s32 flags) { return room_dtor(o, flags, D_00477200, D_0046DB80); }

void *func_0033F0F0(void) {
    return D_00431DE0;
}

void *func_0033F100(void) {
    return D_00431E20;
}

void *func_0033F110(void) {
    return D_00431E80;
}

void *func_0033F120(void) {
    return D_00431EF0;
}

void *func_0033F130(void) {
    return D_00431F90;
}

u32 func_0033F140(void *self, s32 i) {
    return D_004323B0[i];
}

void *func_0033F160(void) {
    return D_00432408;
}

u32 func_0033F170(void *self, s32 i) {
    return D_004323F0[i];
}
