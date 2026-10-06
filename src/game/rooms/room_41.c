/* Room 0x41: its event handler class (vtable D_0046B7B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B7B0[], *D_0046DB80[];

extern u8 D_0041CA90[];
extern u8 D_0041CB20[];
extern u8 D_0041CC00[];
extern u8 D_0041CD90[];
extern u8 D_0041CDF0[];
extern u8 D_0041CEE8[];
extern void *D_0041D100[];
extern u8 D_0041D120[];

void *func_0020B970(void *o, s32 flags) { return room_dtor(o, flags, D_0046B7B0, D_0046DB80); }

void *func_0020B9D0(void) {
    return D_0041CA90;
}

void *func_0020B9E0(void) {
    return D_0041CB20;
}

void *func_0020B9F0(void) {
    return D_0041CC00;
}

void *func_0020BA00(void) {
    return D_0041CD90;
}

void *func_0020BA10(void) {
    return D_0041CDF0;
}

void *func_0020BA20(void) {
    return D_0041CEE8;
}

void *func_0020BA30(void *self, s32 i) {
    return D_0041D100[i];
}

void *func_0020BA50(void) {
    return D_0041D120;
}
