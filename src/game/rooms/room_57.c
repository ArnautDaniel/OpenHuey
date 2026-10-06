/* Room 0x57: its event handler class (vtable D_0046E8C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E8C0[];
extern u8 D_0047ABC0[];

extern u8 D_0040F490[];
extern u8 D_0040F4C0[];
extern u8 D_0040F540[];
extern u8 D_0040F5B0[];
extern u32 D_0040F8C0[];
extern u8 D_0040F8F8[];
extern u32 D_0040F8E8[];

void *func_002B4EF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E8C0, D_0046DB80); }

void *func_002B4F50(void) {
    return D_0040F490;
}

void *func_002B4F60(void) {
    return D_0040F4C0;
}

void *func_002B4F70(void) {
    return D_0040F540;
}

void *func_002B4F80(void) {
    return D_0040F5B0;
}

void *func_002B4F90(void *o) { return D_0047ABC0; }   /* D_0046E8C0 +0x20 */

u32 func_002B4FA0(void *self, s32 i) {
    return D_0040F8C0[i];
}

void *func_002B4FC0(void) {
    return D_0040F8F8;
}

u32 func_002B4FD0(void *self, s32 i) {
    return D_0040F8E8[i];
}
