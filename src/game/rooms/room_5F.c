/* Room 0x5F: its event handler class (vtable D_0046B630, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B630[];
extern void *D_0046DB80[];

extern u8 D_0041B130[];
extern u8 D_0041B1B0[];
extern u8 D_0041B270[];
extern u8 D_0041B370[];
extern u8 D_0041B450[];
extern u8 D_0041B560[];
extern void *D_0047ACA0[];

void *func_0020B4C0(void *o, s32 flags) { return room_dtor(o, flags, D_0046B630, D_0046DB80); }

void *func_0020B520(void) {
    return D_0041B130;
}

void *func_0020B530(void) {
    return D_0041B1B0;
}

void *func_0020B540(void) {
    return D_0041B270;
}

void *func_0020B550(void) {
    return D_0041B370;
}

void *func_0020B560(void) {
    return D_0041B450;
}

void *func_0020B570(void *self, s32 i) {
    return D_0047ACA0[i];
}

void *func_0020B590(void) {
    return D_0041B560;
}
