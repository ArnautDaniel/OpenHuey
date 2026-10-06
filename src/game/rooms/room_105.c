/* Room 0x105: its event handler class (vtable D_0046FD40, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046FD40[];

extern u8 D_00417F40[];
extern u8 D_00417F50[];
extern u8 D_00418060[];
extern u8 D_00418120[];
extern void *D_00418290[];
extern u8 D_004182C0[];
extern void *D_004182B0[];

extern PTMF D_01990EC8[];

void *func_002E6890(void *o, s32 flags) { return room_dtor(o, flags, D_0046FD40, D_0046DB80); }

void *func_002E68F0(void) { return D_00417F40; }

void *func_002E6900(void) { return D_00417F50; }

void *func_002E6910(void) { return D_00418060; }

void *func_002E6920(void) { return D_00418120; }

void *func_002E6930(void *self, s32 i) { return D_00418290[i]; }

void *func_002E6950(void) { return D_004182C0; }

void *func_002E6960(void *self, s32 i) { return D_004182B0[i]; }

/* (self->*D_01990EC8[i])(a, b) */
s32 func_002E6980(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EC8[i & 0xFF], a, b);
}

s32 func_002E69B0(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
