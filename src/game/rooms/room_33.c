/* Room 0x33: its event handler class (vtable D_00478B80, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00478B80[];
extern u8 D_0047AF70[], D_0047AF78[];

extern u32 D_00442DA0[];
extern u32 D_00442F48[];
extern u32 D_0047AF80[];

void *func_003506E0(void *o, s32 flags) { return room_dtor(o, flags, D_00478B80, D_0046DB80); }

void *func_00350740(void *o) { return D_0047AF70; }   /* D_00478B80 +0xC */

void *func_00350750(void) {
    return D_00442DA0;
}

void *func_00350760(void *o) { return D_0047AF78; }   /* D_00478B80 +0x10 */

u32 func_00350770(void *self, s32 i) {
    return D_00442F48[i];
}

u32 func_00350790(void *self, s32 i) {
    return D_0047AF80[i];
}
