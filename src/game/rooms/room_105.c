/* Room 0x105: its event handler class (vtable Room105_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room105_vtable[];

extern u8 D_00417F40[];
extern u8 D_00417F50[];
extern u8 D_00418060[];
extern u8 D_00418120[];
extern void *D_00418290[];
extern u8 D_004182C0[];
extern void *D_004182B0[];

extern PTMF D_01990EC8[];

/* 0x002E6890 */
void *Room105_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room105_vtable, RoomBase_vtable); }

/* 0x002E68F0 */
void *Room105_EnterScript(void) { return D_00417F40; }

/* 0x002E6900 */
void *Room105_CharEnterScript(void) { return D_00417F50; }

/* 0x002E6910 */
void *Room105_Phase1Script(void) { return D_00418060; }

/* 0x002E6920 */
void *Room105_Phase2Script(void) { return D_00418120; }

/* 0x002E6930 */
void *Room105_ActionScript(void *self, s32 i) { return D_00418290[i]; }

/* 0x002E6950 */
void *Room105_Table38(void) { return D_004182C0; }

/* 0x002E6960 */
void *Room105_ObjectName(void *self, s32 i) { return D_004182B0[i]; }

/* (self->*D_01990EC8[i])(a, b) */
/* 0x002E6980 */
s32 Room105_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EC8[i & 0xFF], a, b);
}

/* 0x002E69B0 */
s32 Room105_Cmd00(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
