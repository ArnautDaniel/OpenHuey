/* Room 0x106: its event handler class (vtable Room106_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room106_vtable[];

extern u8 Room106_EnterScript_data[];
extern u8 Room106_CharEnterScript_data[];
extern u8 Room106_Phase1Script_data[];
extern u8 Room106_Phase2Script_data[];
extern void *Room106_ActionScripts[];
extern u8 Room106_Table38_data[];
extern void *Room106_ObjectNames[];

extern PTMF Room106_CmdTable[];

/* 0x002E6D00 */
void *Room106_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room106_vtable, RoomBase_vtable); }

/* 0x002E6D60 */
void *Room106_EnterScript(void) { return Room106_EnterScript_data; }

/* 0x002E6D70 */
void *Room106_CharEnterScript(void) { return Room106_CharEnterScript_data; }

/* 0x002E6D80 */
void *Room106_Phase1Script(void) { return Room106_Phase1Script_data; }

/* 0x002E6D90 */
void *Room106_Phase2Script(void) { return Room106_Phase2Script_data; }

/* 0x002E6DA0 */
void *Room106_ActionScript(void *self, s32 i) { return Room106_ActionScripts[i]; }

/* 0x002E6DC0 */
void *Room106_Table38(void) { return Room106_Table38_data; }

/* 0x002E6DD0 */
void *Room106_ObjectName(void *self, s32 i) { return Room106_ObjectNames[i]; }

/* (self->*Room106_CmdTable[i])(a, b) */
/* 0x002E6DF0 */
s32 Room106_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room106_CmdTable[i & 0xFF], a, b);
}

/* 0x002E6E20 */
s32 Room106_Cmd00(void) {
    Effect_New(gEffects, 0xC0, effect_C0_init);
    return 1;
}
