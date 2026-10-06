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

/* 0x002E6420 */
void *Room104_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046FD00, D_0046DB80); }

/* 0x002E6480 */
void *Room104_EnterScript(void *o) { return D_0047AC60; }   /* D_0046FD00 +0xC */

/* 0x002E6490 */
void *Room104_CharEnterScript(void) { return D_00417BD0; }

/* 0x002E64A0 */
void *Room104_Phase1Script(void) { return D_00417CE0; }

/* 0x002E64B0 */
void *Room104_Phase2Script(void) { return D_00417DA0; }

/* 0x002E64C0 */
void *Room104_ActionScript(void *self, s32 i) { return D_00417F10[i]; }

/* 0x002E64E0 */
void *Room104_Table38(void *o) { return D_0047AC68; }   /* D_0046FD00 +0x38 */

/* 0x002E64F0 */
void *Room104_ObjectName(void *self, s32 i) { return D_00417F30[i]; }

/* (self->*D_01990EB8[i])(a, b) */
/* 0x002E6510 */
s32 Room104_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EB8[i & 0xFF], a, b);
}

/* 0x002E6540 */
s32 Room104_Cmd00(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
