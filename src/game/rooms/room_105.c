/* Room 0x105: its event handler class (vtable Room105_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room105_vtable[];

extern u8 Room105_EnterScript_data[];
extern u8 Room105_CharEnterScript_data[];
extern u8 Room105_Phase1Script_data[];
extern u8 Room105_Phase2Script_data[];
extern void *Room105_ActionScripts[];
extern u8 Room105_Table38_data[];
extern void *Room105_ObjectNames[];

extern PTMF Room105_CmdTable[];

/* 0x002E6890 */
void *Room105_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room105_vtable, RoomBase_vtable); }

/* 0x002E68F0 */
void *Room105_EnterScript(void) { return Room105_EnterScript_data; }

/* 0x002E6900 */
void *Room105_CharEnterScript(void) { return Room105_CharEnterScript_data; }

/* 0x002E6910 */
void *Room105_Phase1Script(void) { return Room105_Phase1Script_data; }

/* 0x002E6920 */
void *Room105_Phase2Script(void) { return Room105_Phase2Script_data; }

/* 0x002E6930 */
void *Room105_ActionScript(void *self, s32 i) { return Room105_ActionScripts[i]; }

/* 0x002E6950 */
void *Room105_Table38(void) { return Room105_Table38_data; }

/* 0x002E6960 */
void *Room105_ObjectName(void *self, s32 i) { return Room105_ObjectNames[i]; }

/* (self->*Room105_CmdTable[i])(a, b) */
/* 0x002E6980 */
s32 Room105_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room105_CmdTable[i & 0xFF], a, b);
}

/* 0x002E69B0 */
s32 Room105_Cmd00(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
