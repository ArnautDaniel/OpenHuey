/* Room 0x1E: its event handler class (vtable D_0046E200, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E200[];

extern u8 D_003FD9C0[];
extern u8 D_003FDA40[];
extern u8 D_003FDB00[];
extern u8 D_003FDEC8[];
extern u8 D_003FDEE0[];
extern u32 D_003FE2C0[];
extern u8 D_003FE2F0[];
extern u32 D_0047AAA0[];

void *func_002ADF90(void *o, s32 flags) { return room_dtor(o, flags, D_0046E200, D_0046DB80); }

void *func_002ADFF0(void) {
    return D_003FD9C0;
}

void *func_002AE000(void) {
    return D_003FDA40;
}

void *func_002AE010(void) {
    return D_003FDB00;
}

void *func_002AE020(void) {
    return D_003FDEC8;
}

void *func_002AE030(void) {
    return D_003FDEE0;
}

u32 func_002AE040(void *self, s32 i) {
    return D_003FE2C0[i];
}

void *func_002AE060(void) {
    return D_003FE2F0;
}

u32 func_002AE070(void *self, s32 i) {
    return D_0047AAA0[i];
}
