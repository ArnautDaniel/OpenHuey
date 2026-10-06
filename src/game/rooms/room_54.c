/* Room 0x54: its event handler class (vtable D_00471F60, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00471F60[];

extern u8 D_00426D80[];
extern u8 D_00426EF0[];
extern u8 D_00426FF0[];
extern u8 D_004272A0[];
extern void *D_00427F90[];
extern void *D_00428050[];
extern u8 D_00428080[];

extern PTMF D_019911A0[];

void *func_0030F940(void *o, s32 flags) { return room_dtor(o, flags, D_00471F60, D_0046DB80); }

void *func_0030F9A0(void) {
    return D_00426D80;
}

void *func_0030F9B0(void) {
    return D_00426EF0;
}

void *func_0030F9C0(void) {
    return D_00426FF0;
}

void *func_0030F9D0(void) {
    return D_004272A0;
}

void *func_0030F9E0(void *self, s32 i) {
    return D_00427F90[i];
}

void *func_0030FA00(void) {
    return D_00428080;
}

void *func_0030FA10(void *self, s32 i) {
    return D_00428050[i];
}

/* (self->*D_019911A0[i])(a, b) */
s32 func_0030FA30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019911A0[i & 0xFF], a, b);
}
