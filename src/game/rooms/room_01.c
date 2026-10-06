/* Room 0x01: its event handler class (vtable D_0046B8B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B8B0[], *D_0046DB80[];

extern u8 D_003EE770[];
extern u8 D_003EE820[];
extern u8 D_003EE920[];
extern u8 D_003EEB90[];
extern u8 D_003EEBC0[];
extern u8 D_003EEC00[];
extern void *D_003EEDC0[];
extern u8 D_003EEDD8[];

void *func_0020BCF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B8B0, D_0046DB80); }

void *func_0020BD50(void) {
    return D_003EE770;
}

void *func_0020BD60(void) {
    return D_003EE820;
}

void *func_0020BD70(void) {
    return D_003EE920;
}

void *func_0020BD80(void) {
    return D_003EEB90;
}

void *func_0020BD90(void) {
    return D_003EEBC0;
}

void *func_0020BDA0(void) {
    return D_003EEC00;
}

void *func_0020BDB0(void *self, s32 i) {
    return D_003EEDC0[i];
}

void *func_0020BDD0(void) {
    return D_003EEDD8;
}
