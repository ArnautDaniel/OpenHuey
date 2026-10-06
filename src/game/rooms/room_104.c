/* Room 0x104: its event handler class (vtable D_0046FD00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046FD00[];
extern u8 D_0047AC60[], D_0047AC68[];

extern u8 D_00417BD0[];
extern u8 D_00417CE0[];
extern u8 D_00417DA0[];
extern void *D_00417F10[];
extern void *D_00417F30[];

extern PTMF D_01990EB8[];

void *func_002E6420(void *o, s32 flags) { return room_dtor(o, flags, D_0046FD00, D_0046DB80); }

void *func_002E6480(void *o) { return D_0047AC60; }   /* D_0046FD00 +0xC */

void *func_002E6490(void) { return D_00417BD0; }

void *func_002E64A0(void) { return D_00417CE0; }

void *func_002E64B0(void) { return D_00417DA0; }

void *func_002E64C0(void *self, s32 i) { return D_00417F10[i]; }

void *func_002E64E0(void *o) { return D_0047AC68; }   /* D_0046FD00 +0x38 */

void *func_002E64F0(void *self, s32 i) { return D_00417F30[i]; }

/* (self->*D_01990EB8[i])(a, b) */
s32 func_002E6510(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EB8[i & 0xFF], a, b);
}

s32 func_002E6540(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
