/* Room 0x84: its event handler class (vtable Room84_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room84_vtable[];
extern u8 Room84_Phase2Script_data[];

extern u32 Room84_EnterScript_data[];
extern u32 Room84_CharEnterScript_data[];
extern u32 Room84_Phase1Script_data[];
extern u32 Room84_ObjectNames[];
extern u32 Room84_Table38_data[];
extern u32 Room84_ActionScripts[];

extern PTMF Room84_CmdTable[];

/* 0x0033F190 */
void *Room84_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room84_vtable, RoomBase_vtable); }

/* 0x0033F1F0 */
void *Room84_EnterScript(void) {
    return Room84_EnterScript_data;
}

/* 0x0033F200 */
void *Room84_CharEnterScript(void) {
    return Room84_CharEnterScript_data;
}

/* 0x0033F210 */
void *Room84_Phase1Script(void) {
    return Room84_Phase1Script_data;
}

/* 0x0033F220 */
void *Room84_Phase2Script(void *o) { return Room84_Phase2Script_data; }   /* Room84_vtable +0x14 */

/* 0x0033F230 */
u32 Room84_ActionScript(void *self, s32 i) {
    return Room84_ActionScripts[i];
}

/* 0x0033F250 */
void *Room84_Table38(void) {
    return Room84_Table38_data;
}

/* 0x0033F260 */
u32 Room84_ObjectName(void *self, s32 i) {
    return Room84_ObjectNames[i];
}

/* (self->*Room84_CmdTable[i])(a, b) */
/* 0x0033F280 */
s32 Room84_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room84_CmdTable[i & 0xFF], a, b);
}

/* room 0x84: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
 * shake of 0.5 (slam_shake; a frame hook). */
/* 0x0033F2B0 */
s32 Room84_Cmd00(void) { return slam_shake(); }
