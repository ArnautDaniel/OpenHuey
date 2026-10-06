/* Room 0x35: its event handler class (vtable D_00478C40, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00478C40[];
extern u8 D_0047AF98[];

extern u32 D_00443080[];
extern u32 D_00443490[];
extern u32 D_004434B0[];

void *func_00350FF0(void *o, s32 flags) { return room_dtor(o, flags, D_00478C40, D_0046DB80); }

void *func_00351050(void *o) { return D_0047AF98; }   /* D_00478C40 +0xC */

void *func_00351060(void) {
    return D_00443080;
}

u32 func_00351070(void *self, s32 i) {
    return D_00443490[i];
}

u32 func_00351090(void *self, s32 i) {
    return D_004434B0[i];
}
