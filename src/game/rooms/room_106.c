/* Room 0x106: its event handler class (vtable D_0046FD80, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046FD80[];

extern u8 D_004182D0[];
extern u8 D_00418430[];
extern u8 D_004184C0[];
extern u8 D_00418730[];
extern void *D_00418AA0[];
extern u8 D_00418AD0[];
extern void *D_0047AC70[];

extern PTMF D_01990ED8[];

void *func_002E6D00(void *o, s32 flags) { return room_dtor(o, flags, D_0046FD80, D_0046DB80); }

void *func_002E6D60(void) { return D_004182D0; }

void *func_002E6D70(void) { return D_00418430; }

void *func_002E6D80(void) { return D_004184C0; }

void *func_002E6D90(void) { return D_00418730; }

void *func_002E6DA0(void *self, s32 i) { return D_00418AA0[i]; }

void *func_002E6DC0(void) { return D_00418AD0; }

void *func_002E6DD0(void *self, s32 i) { return D_0047AC70[i]; }

/* (self->*D_01990ED8[i])(a, b) */
s32 func_002E6DF0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990ED8[i & 0xFF], a, b);
}

s32 func_002E6E20(void) {
    Effect_New(gEffects, 0xC0, effect_C0_init);
    return 1;
}
