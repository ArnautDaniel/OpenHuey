/* Room 0x103: its event handler class (vtable D_0046FCC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046FCC0[];

extern u8 D_00417840[];
extern u8 D_00417850[];
extern u8 D_00417960[];
extern u8 D_00417A20[];

extern void *D_00417B90[];
extern u8 D_00417BC0[];
extern void *D_00417BB0[];

extern PTMF D_01990EA8[];

void *func_002E5FB0(void *o, s32 flags) { return room_dtor(o, flags, D_0046FCC0, D_0046DB80); }

void *func_002E6010(void) { return D_00417840; }

void *func_002E6020(void) { return D_00417850; }

void *func_002E6030(void) { return D_00417960; }

void *func_002E6040(void) { return D_00417A20; }

void *func_002E6050(void *self, s32 i) { return D_00417B90[i]; }

void *func_002E6070(void) { return D_00417BC0; }

void *func_002E6080(void *self, s32 i) { return D_00417BB0[i]; }

/* (self->*D_01990EA8[i])(a, b) */
s32 func_002E60A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EA8[i & 0xFF], a, b);
}

s32 func_002E60D0(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
