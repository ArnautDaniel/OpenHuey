/* Room 0x68: its event handler class (vtable D_00471E60, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00471E60[];

extern u8 D_00424880[];
extern u8 D_00424890[];
extern u8 D_00424990[];
extern u8 D_00424AC0[];
extern u8 D_00424AF0[];
extern void *D_0047AD20[];
extern void *D_0047AD24[];

void *func_0030EE90(void *o, s32 flags) { return room_dtor(o, flags, D_00471E60, D_0046DB80); }

void *func_0030EEF0(void) {
    return D_00424880;
}

void *func_0030EF00(void) {
    return D_00424890;
}

void *func_0030EF10(void) {
    return D_00424990;
}

void *func_0030EF20(void) {
    return D_00424AC0;
}

void *func_0030EF30(void) {
    return D_00424AF0;
}

void *func_0030EF40(void *self, s32 i) {
    return D_0047AD20[i];
}

void *func_0030EF60(void *self, s32 i) {
    return D_0047AD24[i];
}
