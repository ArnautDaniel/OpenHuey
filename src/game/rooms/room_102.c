/* Room 0x102: its event handler class (vtable Room102_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room102_vtable[];

extern u8 Room102_EnterScript_data[];
extern u8 Room102_CharEnterScript_data[];
extern u8 Room102_Phase1Script_data[];
extern u8 Room102_Phase2Script_data[];
extern void *Room102_ActionScripts[];
extern u8 Room102_Table38_data[];
extern void *Room102_ObjectNames[];

extern PTMF Room102_CmdTable[];

/* 0x002E5B40 */
void *Room102_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room102_vtable, RoomBase_vtable); }

/* 0x002E5BA0 */
void *Room102_EnterScript(void) { return Room102_EnterScript_data; }

/* 0x002E5BB0 */
void *Room102_CharEnterScript(void) { return Room102_CharEnterScript_data; }

/* 0x002E5BC0 */
void *Room102_Phase1Script(void) { return Room102_Phase1Script_data; }

/* 0x002E5BD0 */
void *Room102_Phase2Script(void) { return Room102_Phase2Script_data; }

/* 0x002E5BE0 */
void *Room102_ActionScript(void *self, s32 i) { return Room102_ActionScripts[i]; }

/* 0x002E5C00 */
void *Room102_Table38(void) { return Room102_Table38_data; }

/* 0x002E5C10 */
void *Room102_ObjectName(void *self, s32 i) { return Room102_ObjectNames[i]; }

/* (self->*Room102_CmdTable[i])(a, b) */
/* 0x002E5C30 */
s32 Room102_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room102_CmdTable[i & 0xFF], a, b);
}

/* 0x002E5C60 */
s32 Room102_Cmd00(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
