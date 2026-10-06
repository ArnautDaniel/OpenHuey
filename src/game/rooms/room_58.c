/* Room 0x58: its event handler class (vtable D_0046E900, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E900[];
extern u8 D_0047ABD0[];

extern u8 D_0040F910[];
extern u8 D_0040FA20[];
extern u8 D_0040FAB0[];
extern u8 D_0040FC70[];
extern u32 D_00410030[];
extern u8 D_00410050[];
extern u32 D_0047ABD8[];

void *func_002B4FF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E900, D_0046DB80); }

void *func_002B5050(void) {
    return D_0040F910;
}

void *func_002B5060(void) {
    return D_0040FA20;
}

void *func_002B5070(void) {
    return D_0040FAB0;
}

void *func_002B5080(void *o) { return D_0047ABD0; }   /* D_0046E900 +0x20 */

void *func_002B5090(void) {
    return D_0040FC70;
}

u32 func_002B50A0(void *self, s32 i) {
    return D_00410030[i];
}

void *func_002B50C0(void) {
    return D_00410050;
}

u32 func_002B50D0(void *self, s32 i) {
    return D_0047ABD8[i];
}
