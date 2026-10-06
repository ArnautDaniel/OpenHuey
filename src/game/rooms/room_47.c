/* Room 0x47: its event handler class (vtable D_00471EA0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00471EA0[];

extern u8 D_00424B50[];
extern u8 D_00424B70[];
extern u8 D_00424C10[];
extern u8 D_00424D70[];
extern void *D_00425290[];
extern u8 D_004252C0[];
extern void *D_0047AD28[];

void *func_0030EF80(void *o, s32 flags) { return room_dtor(o, flags, D_00471EA0, D_0046DB80); }

void *func_0030EFE0(void) {
    return D_00424B50;
}

void *func_0030EFF0(void) {
    return D_00424B70;
}

void *func_0030F000(void) {
    return D_00424C10;
}

void *func_0030F010(void) {
    return D_00424D70;
}

void *func_0030F020(void *self, s32 i) {
    return D_00425290[i];
}

void *func_0030F040(void) {
    return D_004252C0;
}

void *func_0030F050(void *self, s32 i) {
    return D_0047AD28[i];
}
