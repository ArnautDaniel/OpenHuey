/* Room 0x94: its event handler class (vtable Room94_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room94_vtable[];
extern u8 Room94_Phase5Script_data[];

extern u8 Room94_EnterScript_data[];
extern u8 Room94_CharEnterScript_data[];
extern u8 Room94_Phase1Script_data[];
extern u8 Room94_Phase2Script_data[];
extern void *Room94_ActionScripts[];
extern PTMF Room94_CmdTable[];

/* 0x0035D330 */
void *Room94_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room94_vtable, RoomBase_vtable); }

/* 0x0035D390 */
void *Room94_EnterScript(void) {
    return Room94_EnterScript_data;
}

/* 0x0035D3A0 */
void *Room94_CharEnterScript(void) {
    return Room94_CharEnterScript_data;
}

/* 0x0035D3B0 */
void *Room94_Phase1Script(void) {
    return Room94_Phase1Script_data;
}

/* 0x0035D3C0 */
void *Room94_Phase2Script(void) {
    return Room94_Phase2Script_data;
}

/* 0x0035D3D0 */
void *Room94_Phase5Script(void *o) { return Room94_Phase5Script_data; }   /* Room94_vtable +0x20 */

/* 0x0035D3E0 */
void *Room94_ActionScript(void *self, s32 i) {
    return Room94_ActionScripts[i];
}

/* (self->*Room94_CmdTable[i])(a, b) */
/* 0x0035D400 */
s32 Room94_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room94_CmdTable[i & 0xFF], a, b);
}

/* 0x0035D430 */
s32 Room94_Cmd00(void) { return slam_shake(); }
