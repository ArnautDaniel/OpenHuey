/* Room 0x2F: its event handler class (vtable D_0046EDC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "item.h"
#include "pursuer.h"

extern void *D_0046DB80[];
extern void *D_0046EDC0[];

extern u8 D_00412950[], D_004129A0[], D_00412A30[], D_00412B60[], D_00412B90[];
extern void *D_00412E60[];
extern u8 D_00412E90[];
extern void *D_0047ABFC[];

extern PTMF D_01990E38[];

void *func_002CC950(void *o, s32 flags) { return room_dtor(o, flags, D_0046EDC0, D_0046DB80); }

void *func_002CC9B0(void) { return D_00412950; }

void *func_002CC9C0(void) { return D_004129A0; }

void *func_002CC9D0(void) { return D_00412A30; }

void *func_002CC9E0(void) { return D_00412B60; }

void *func_002CC9F0(void) { return D_00412B90; }

void *func_002CCA00(void *self, s32 i) { return D_00412E60[i]; }

void *func_002CCA20(void) { return D_00412E90; }

void *func_002CCA30(void *self, s32 i) { return D_0047ABFC[i]; }

/* (self->*D_01990E38[i])(a, b) */
s32 func_002CCA50(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E38[i & 0xFF], a, b);
}

/* room 0x2F (D_00412E80): the pursuer's func_0029A710 */
s32 func_002CCA80(void) {
    return func_0029A710((Pursuer *)gCharPursuer);
}
