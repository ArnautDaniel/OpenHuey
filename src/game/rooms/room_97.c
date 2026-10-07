/* Room 0x97: its event handler class (vtable Room97_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room97_vtable[];
extern u8 Room97_Phase2Script_data[];

extern u32 Room97_EnterScript_data[];
extern u32 Room97_CharEnterScript_data[];
extern u32 Room97_Phase1Script_data[];
extern u32 Room97_Table38_data[];
extern u32 Room97_ActionScripts[];

extern PTMF Room97_CmdTable[];

/* 0x00343730 */
void *Room97_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room97_vtable, RoomBase_vtable); }

/* 0x00343790 */
void *Room97_EnterScript(void) {
    return Room97_EnterScript_data;
}

/* 0x003437A0 */
void *Room97_CharEnterScript(void) {
    return Room97_CharEnterScript_data;
}

/* 0x003437B0 */
void *Room97_Phase1Script(void) {
    return Room97_Phase1Script_data;
}

/* 0x003437C0 */
void *Room97_Phase2Script(void *o) { return Room97_Phase2Script_data; }   /* Room97_vtable +0x14 */

/* 0x003437D0 */
u32 Room97_ActionScript(void *self, s32 i) {
    return Room97_ActionScripts[i];
}

/* 0x003437F0 */
void *Room97_Table38(void) {
    return Room97_Table38_data;
}

/* (self->*Room97_CmdTable[i])(a, b) */
/* 0x00343800 */
s32 Room97_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room97_CmdTable[i & 0xFF], a, b);
}

/* room 0x97: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
 * shake of 0.5 (slam_shake; a frame hook). */
/* 0x00343830 */
s32 Room97_Cmd00(void) { return slam_shake(); }
