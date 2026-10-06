/* Room 0x07: its event handler class (vtable D_0046B830, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B830[], *D_0046DB80[];

extern u8 D_003F21E0[];
extern u8 D_003F21F0[];
extern u8 D_003F22D0[];
extern u8 D_003F24B0[];
extern u8 D_003F24F0[];
extern void *D_0047A9A8[];

void *func_0020BB40(void *o, s32 flags) { return room_dtor(o, flags, D_0046B830, D_0046DB80); }

void *func_0020BBA0(void) {
    return D_003F21E0;
}

void *func_0020BBB0(void) {
    return D_003F21F0;
}

void *func_0020BBC0(void) {
    return D_003F22D0;
}

void *func_0020BBD0(void) {
    return D_003F24B0;
}

void *func_0020BBE0(void *self, s32 i) {
    return D_0047A9A8[i];
}

void *func_0020BC00(void) {
    return D_003F24F0;
}
