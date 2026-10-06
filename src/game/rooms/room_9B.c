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

/* 0x00352E20 */
void *Room9B_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_004793C0, D_0046DB80); }

/* 0x00352E80 */
void *Room9B_EnterScript(void) { return D_00443870; }

/* 0x00352E90 */
void *Room9B_CharEnterScript(void) { return D_00443890; }

/* 0x00352EA0 */
void *Room9B_Phase1Script(void) { return D_00443910; }

/* 0x00352EB0 */
void *Room9B_Phase2Script(void) { return D_00443AB0; }

/* 0x00352EC0 */
void *Room9B_Phase3Script(void) { return D_00443B00; }

/* 0x00352ED0 */
void *Room9B_ActionScript(void *self, s32 i) { return D_00443DD0[i]; }

/* 0x00352EF0 */
void *Room9B_Table38(void) { return D_00443E10; }

/* 0x00352F00 */
void *Room9B_ObjectName(void *self, s32 i) { return D_00443E00[i]; }

/* (self->*D_01991A48[i])(a, b) */
/* 0x00352F20 */
s32 Room9B_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A48[i & 0xFF], a, b);
}

/* (as Room2D_Cmd02) three hanging things (+0x34 0..2), pushed by the square of Fiona's step
   past 1, event flag 4 with sounds 4 / 5 */
/* 0x00352F50 */
s32 Room9B_Cmd00(VObject *self, void *a1, u8 *cmd) {
    return hangers_swing(self, cmd, 0, 3, 1.0f, 0, 4, 4, 5);
}
