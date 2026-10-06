/* Room 0x104: its event handler class (vtable Room104_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room104_vtable[];
extern u8 Room104_EnterScript_data[], Room104_Table38_data[];

extern u8 Room104_CharEnterScript_data[];
extern u8 Room104_Phase1Script_data[];
extern u8 Room104_Phase2Script_data[];
extern void *Room104_ActionScripts[];
extern void *Room104_ObjectNames[];

extern PTMF Room104_CmdTable[];

/* 0x002E6420 */
void *Room104_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room104_vtable, RoomBase_vtable); }

/* 0x002E6480 */
void *Room104_EnterScript(void *o) { return Room104_EnterScript_data; }   /* Room104_vtable +0xC */

/* 0x002E6490 */
void *Room104_CharEnterScript(void) { return Room104_CharEnterScript_data; }

/* 0x002E64A0 */
void *Room104_Phase1Script(void) { return Room104_Phase1Script_data; }

/* 0x002E64B0 */
void *Room104_Phase2Script(void) { return Room104_Phase2Script_data; }

/* 0x002E64C0 */
void *Room104_ActionScript(void *self, s32 i) { return Room104_ActionScripts[i]; }

/* 0x002E64E0 */
void *Room104_Table38(void *o) { return Room104_Table38_data; }   /* Room104_vtable +0x38 */

/* 0x002E64F0 */
void *Room104_ObjectName(void *self, s32 i) { return Room104_ObjectNames[i]; }

/* (self->*Room104_CmdTable[i])(a, b) */
/* 0x002E6510 */
s32 Room104_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room104_CmdTable[i & 0xFF], a, b);
}

/* 0x002E6540 */
s32 Room104_Cmd00(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
