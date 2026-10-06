/* Room 0x98: its event handler class (vtable D_00478C00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00478C00[];
extern u8 D_0047AF88[], D_0047AF90[];

extern u32 D_00442F60[];
extern u32 D_00442FE0[];
extern u32 D_00443070[];
extern u32 D_0047AF94[];

extern PTMF D_01991A18[];

void *func_00350E60(void *o, s32 flags) { return room_dtor(o, flags, D_00478C00, D_0046DB80); }

void *func_00350EC0(void *o) { return D_0047AF88; }   /* D_00478C00 +0xC */

void *func_00350ED0(void) {
    return D_00442F60;
}

void *func_00350EE0(void) {
    return D_00442FE0;
}

void *func_00350EF0(void *o) { return D_0047AF90; }   /* D_00478C00 +0x14 */

u32 func_00350F00(void *self, s32 i) {
    return D_0047AF94[i];
}

void *func_00350F20(void) {
    return D_00443070;
}

/* (self->*D_01991A18[i])(a, b) */
s32 func_00350F30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A18[i & 0xFF], a, b);
}

s32 func_00350F60(void) { return slam_shake(); }
