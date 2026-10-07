/* Room 0x85: its event handler class (vtable Room85_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room85_vtable[];
extern u8 Room85_Table38_data[];

extern u32 Room85_EnterScript_data[];
extern u32 Room85_CharEnterScript_data[];
extern u32 Room85_Phase1Script_data[];
extern u32 Room85_Phase2Script_data[];
extern u32 Room85_ActionScripts[];
extern u32 Room85_ObjectNames[];

extern PTMF Room85_CmdTable[];

/* 0x0033F340 */
void *Room85_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room85_vtable, RoomBase_vtable); }

/* 0x0033F3A0 */
void *Room85_EnterScript(void) {
    return Room85_EnterScript_data;
}

/* 0x0033F3B0 */
void *Room85_CharEnterScript(void) {
    return Room85_CharEnterScript_data;
}

/* 0x0033F3C0 */
void *Room85_Phase1Script(void) {
    return Room85_Phase1Script_data;
}

/* 0x0033F3D0 */
void *Room85_Phase2Script(void) {
    return Room85_Phase2Script_data;
}

/* 0x0033F3E0 */
u32 Room85_ActionScript(void *self, s32 i) {
    return Room85_ActionScripts[i];
}

/* 0x0033F400 */
void *Room85_Table38(void *o) { return Room85_Table38_data; }   /* Room85_vtable +0x38 */

/* 0x0033F410 */
u32 Room85_ObjectName(void *self, s32 i) {
    return Room85_ObjectNames[i];
}

/* (self->*Room85_CmdTable[i])(a, b) */
/* 0x0033F430 */
s32 Room85_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room85_CmdTable[i & 0xFF], a, b);
}

/* room 0x85: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
 * shake of 0.5 (slam_shake; a frame hook). */
/* 0x0033F460 */
s32 Room85_Cmd00(void) { return slam_shake(); }
