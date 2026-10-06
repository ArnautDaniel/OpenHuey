/* Room 0x9B: its event handler class (vtable Room9B_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room9B_vtable[];

extern u8 Room9B_EnterScript_data[], Room9B_CharEnterScript_data[], Room9B_Phase1Script_data[], Room9B_Phase2Script_data[], Room9B_Phase3Script_data[];
extern void *Room9B_ActionScripts[];
extern u8 Room9B_Table38_data[];
extern void *Room9B_ObjectNames[];

extern PTMF Room9B_CmdTable[];

/* 0x00352E20 */
void *Room9B_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room9B_vtable, RoomBase_vtable); }

/* 0x00352E80 */
void *Room9B_EnterScript(void) { return Room9B_EnterScript_data; }

/* 0x00352E90 */
void *Room9B_CharEnterScript(void) { return Room9B_CharEnterScript_data; }

/* 0x00352EA0 */
void *Room9B_Phase1Script(void) { return Room9B_Phase1Script_data; }

/* 0x00352EB0 */
void *Room9B_Phase2Script(void) { return Room9B_Phase2Script_data; }

/* 0x00352EC0 */
void *Room9B_Phase3Script(void) { return Room9B_Phase3Script_data; }

/* 0x00352ED0 */
void *Room9B_ActionScript(void *self, s32 i) { return Room9B_ActionScripts[i]; }

/* 0x00352EF0 */
void *Room9B_Table38(void) { return Room9B_Table38_data; }

/* 0x00352F00 */
void *Room9B_ObjectName(void *self, s32 i) { return Room9B_ObjectNames[i]; }

/* (self->*Room9B_CmdTable[i])(a, b) */
/* 0x00352F20 */
s32 Room9B_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room9B_CmdTable[i & 0xFF], a, b);
}

/* (as Room2D_Cmd02) three hanging things (+0x34 0..2), pushed by the square of Fiona's step
   past 1, event flag 4 with sounds 4 / 5 */
/* 0x00352F50 */
s32 Room9B_Cmd00(VObject *self, void *a1, u8 *cmd) {
    return hangers_swing(self, cmd, 0, 3, 1.0f, 0, 4, 4, 5);
}
