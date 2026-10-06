/* Room 0x6E: its event handler class (vtable D_004776D0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_004776D0[];

extern u32 D_0043A120[];
extern u32 D_0043A1A0[];
extern u32 D_0043A230[];
extern u32 D_0043A280[];
extern u32 D_0043A2B0[];
extern u32 D_0043A880[];
extern u32 D_0043A8B0[];
extern u32 D_0043A8D0[];

void *func_00344D80(void *o, s32 flags) { return room_dtor(o, flags, D_004776D0, D_0046DB80); }

void *func_00344DE0(void) {
    return D_0043A120;
}

void *func_00344DF0(void) {
    return D_0043A1A0;
}

void *func_00344E00(void) {
    return D_0043A230;
}

void *func_00344E10(void) {
    return D_0043A280;
}

void *func_00344E20(void) {
    return D_0043A2B0;
}

u32 func_00344E30(void *self, s32 i) {
    return D_0043A880[i];
}

void *func_00344E50(void) {
    return D_0043A8D0;
}

u32 func_00344E60(void *self, s32 i) {
    return D_0043A8B0[i];
}
