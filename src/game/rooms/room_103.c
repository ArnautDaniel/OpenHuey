/* Room 0x103: its event handler class (vtable Room103_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room103_vtable[];

extern u8 Room103_EnterScript_data[];
extern u8 Room103_CharEnterScript_data[];
extern u8 Room103_Phase1Script_data[];
extern u8 Room103_Phase2Script_data[];

extern void *Room103_ActionScripts[];
extern u8 Room103_Table38_data[];
extern void *Room103_ObjectNames[];

extern PTMF Room103_CmdTable[];

/* 0x002E5FB0 */
void *Room103_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room103_vtable, RoomBase_vtable); }

/* 0x002E6010 */
void *Room103_EnterScript(void) { return Room103_EnterScript_data; }

/* 0x002E6020 */
void *Room103_CharEnterScript(void) { return Room103_CharEnterScript_data; }

/* 0x002E6030 */
void *Room103_Phase1Script(void) { return Room103_Phase1Script_data; }

/* 0x002E6040 */
void *Room103_Phase2Script(void) { return Room103_Phase2Script_data; }

/* 0x002E6050 */
void *Room103_ActionScript(void *self, s32 i) { return Room103_ActionScripts[i]; }

/* 0x002E6070 */
void *Room103_Table38(void) { return Room103_Table38_data; }

/* 0x002E6080 */
void *Room103_ObjectName(void *self, s32 i) { return Room103_ObjectNames[i]; }

/* (self->*Room103_CmdTable[i])(a, b) */
/* 0x002E60A0 */
s32 Room103_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room103_CmdTable[i & 0xFF], a, b);
}

/* room 0x103: three hanging things that Fiona pushes as she walks by, swinging (swing_three). */
/* 0x002E60D0 */
s32 Room103_Cmd00(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
