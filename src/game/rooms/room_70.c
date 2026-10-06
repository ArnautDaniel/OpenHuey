/* Room 0x70: its event handler class (vtable D_00477750, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477750[];

extern u32 D_0043B020[];
extern u32 D_0043B0D0[];
extern u32 D_0043B1D0[];
extern u32 D_0043B2C0[];
extern u32 D_0043B390[];
extern u32 D_0043B560[];
extern u32 D_0043B588[];
extern u32 D_0043B5A0[];

void *func_00345000(void *o, s32 flags) { return room_dtor(o, flags, D_00477750, D_0046DB80); }

void *func_00345060(void) {
    return D_0043B020;
}

void *func_00345070(void) {
    return D_0043B0D0;
}

void *func_00345080(void) {
    return D_0043B1D0;
}

void *func_00345090(void) {
    return D_0043B2C0;
}

void *func_003450A0(void) {
    return D_0043B390;
}

u32 func_003450B0(void *self, s32 i) {
    return D_0043B560[i];
}

void *func_003450D0(void) {
    return D_0043B5A0;
}

u32 func_003450E0(void *self, s32 i) {
    return D_0043B588[i];
}
