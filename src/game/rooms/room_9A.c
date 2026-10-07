/* Room 0x9A: its event handler class (vtable Room9A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room9A_vtable[];
extern u8 Room9A_EnterScript_data[], Room9A_Phase2Script_data[];

extern u8 Room9A_CharEnterScript_data[], Room9A_Phase1Script_data[];
extern void *Room9A_ActionScripts[];
extern u8 Room9A_Table38_data[];

extern PTMF Room9A_CmdTable[];

/* 0x00352C90 */
void *Room9A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room9A_vtable, RoomBase_vtable); }

/* 0x00352CF0 */
void *Room9A_EnterScript(void *o) { return Room9A_EnterScript_data; }   /* Room9A_vtable +0xC */

/* 0x00352D00 */
void *Room9A_CharEnterScript(void) { return Room9A_CharEnterScript_data; }

/* 0x00352D10 */
void *Room9A_Phase1Script(void) { return Room9A_Phase1Script_data; }

/* 0x00352D20 */
void *Room9A_Phase2Script(void *o) { return Room9A_Phase2Script_data; }   /* Room9A_vtable +0x14 */

/* 0x00352D30 */
void *Room9A_ActionScript(void *self, s32 i) { return Room9A_ActionScripts[i]; }

/* 0x00352D50 */
void *Room9A_Table38(void) { return Room9A_Table38_data; }

/* (self->*Room9A_CmdTable[i])(a, b) */
/* 0x00352D60 */
s32 Room9A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room9A_CmdTable[i & 0xFF], a, b);
}

/* room 0x9A: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
 * shake of 0.5 (slam_shake; a frame hook). */
/* 0x00352D90 */
s32 Room9A_Cmd00(void) { return slam_shake(); }
