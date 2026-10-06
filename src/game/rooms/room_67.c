/* Room 0x67: its event handler class (vtable D_0046B570, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B570[];
extern void *D_0046DB80[];

extern u8 D_0041DD20[];
extern u8 D_0041DD90[];
extern u8 D_0041DE10[];
extern u8 D_0041DF60[];
extern u8 D_0041DF70[];
extern void *D_0041E0E0[];
extern u8 D_0041E0F0[];

void *func_0020B2A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B570, D_0046DB80); }

void *func_0020B300(void) {
    return D_0041DD20;
}

void *func_0020B310(void) {
    return D_0041DD90;
}

void *func_0020B320(void) {
    return D_0041DE10;
}

void *func_0020B330(void) {
    return D_0041DF70;
}

void *func_0020B340(void) {
    return D_0041DF60;
}

void *func_0020B350(void *self, s32 i) {
    return D_0041E0E0[i];
}

void *func_0020B370(void) {
    return D_0041E0F0;
}
