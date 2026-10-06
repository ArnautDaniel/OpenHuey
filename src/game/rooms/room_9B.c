/* Room 0x9B: its event handler class (vtable D_004793C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_004793C0[];

extern u8 D_00443870[], D_00443890[], D_00443910[], D_00443AB0[], D_00443B00[];
extern void *D_00443DD0[];
extern u8 D_00443E10[];
extern void *D_00443E00[];

extern PTMF D_01991A48[];

void *func_00352E20(void *o, s32 flags) { return room_dtor(o, flags, D_004793C0, D_0046DB80); }

void *func_00352E80(void) { return D_00443870; }

void *func_00352E90(void) { return D_00443890; }

void *func_00352EA0(void) { return D_00443910; }

void *func_00352EB0(void) { return D_00443AB0; }

void *func_00352EC0(void) { return D_00443B00; }

void *func_00352ED0(void *self, s32 i) { return D_00443DD0[i]; }

void *func_00352EF0(void) { return D_00443E10; }

void *func_00352F00(void *self, s32 i) { return D_00443E00[i]; }

/* (self->*D_01991A48[i])(a, b) */
s32 func_00352F20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A48[i & 0xFF], a, b);
}

/* (as func_002B1A00) three hanging things (+0x34 0..2), pushed by the square of Fiona's step
   past 1, event flag 4 with sounds 4 / 5 */
s32 func_00352F50(VObject *self, void *a1, u8 *cmd) {
    return hangers_swing(self, cmd, 0, 3, 1.0f, 0, 4, 4, 5);
}
