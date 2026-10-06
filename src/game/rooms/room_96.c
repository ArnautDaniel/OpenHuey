/* Room 0x96: its event handler class (vtable Room96_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room96_vtable[];
extern u8 Room96_Phase5Script_data[];

extern u8 Room96_EnterScript_data[];
extern u8 Room96_CharEnterScript_data[];
extern u8 Room96_Phase1Script_data[];
extern u8 Room96_Phase2Script_data[];
extern void *Room96_ActionScripts[];
extern PTMF Room96_CmdTable[];

/* 0x0035D650 */
void *Room96_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room96_vtable, RoomBase_vtable); }

/* 0x0035D6B0 */
void *Room96_EnterScript(void) {
    return Room96_EnterScript_data;
}

/* 0x0035D6C0 */
void *Room96_CharEnterScript(void) {
    return Room96_CharEnterScript_data;
}

/* 0x0035D6D0 */
void *Room96_Phase1Script(void) {
    return Room96_Phase1Script_data;
}

/* 0x0035D6E0 */
void *Room96_Phase2Script(void) {
    return Room96_Phase2Script_data;
}

/* 0x0035D6F0 */
void *Room96_Phase5Script(void *o) { return Room96_Phase5Script_data; }   /* Room96_vtable +0x20 */

/* 0x0035D700 */
void *Room96_ActionScript(void *self, s32 i) {
    return Room96_ActionScripts[i];
}

/* (self->*Room96_CmdTable[i])(a, b) */
/* 0x0035D720 */
s32 Room96_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room96_CmdTable[i & 0xFF], a, b);
}

/* 0x0035D750 */
s32 Room96_Cmd00(void) { return slam_shake(); }
