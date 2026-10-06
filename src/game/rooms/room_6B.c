/* Room 0x6B: its event handler class (vtable D_00477610, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477610[];
extern u8 D_0047AE84[];

extern u32 D_00438D30[];
extern u32 D_00438D60[];
extern u32 D_00438E20[];
extern u32 D_00438F10[];
extern u32 D_00439130[];
extern u32 D_00439150[];
extern u32 D_0047AE90[];

void *func_00344870(void *o, s32 flags) { return room_dtor(o, flags, D_00477610, D_0046DB80); }

void *func_003448D0(void) {
    return D_00438D30;
}

void *func_003448E0(void) {
    return D_00438D60;
}

void *func_003448F0(void) {
    return D_00438E20;
}

void *func_00344900(void) {
    return D_00438F10;
}

void *func_00344910(void *o) { return D_0047AE84; }   /* D_00477610 +0x20 */

u32 func_00344920(void *self, s32 i) {
    return D_00439130[i];
}

void *func_00344940(void) {
    return D_00439150;
}

u32 func_00344950(void *self, s32 i) {
    return D_0047AE90[i];
}
