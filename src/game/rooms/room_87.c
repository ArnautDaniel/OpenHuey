/* Room 0x87: its event handler class (vtable Room87_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room87_vtable[];
extern u8 Room87_Phase5Script_data[];

extern u32 Room87_EnterScript_data[];
extern u32 Room87_CharEnterScript_data[];
extern u32 Room87_Phase1Script_data[];
extern u32 Room87_Phase2Script_data[];
extern u32 Room87_ActionScripts[];

extern PTMF Room87_CmdTable[];

/* 0x0033F4F0 */
void *Room87_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room87_vtable, RoomBase_vtable); }

/* 0x0033F550 */
void *Room87_EnterScript(void) {
    return Room87_EnterScript_data;
}

/* 0x0033F560 */
void *Room87_CharEnterScript(void) {
    return Room87_CharEnterScript_data;
}

/* 0x0033F570 */
void *Room87_Phase1Script(void) {
    return Room87_Phase1Script_data;
}

/* 0x0033F580 */
void *Room87_Phase2Script(void) {
    return Room87_Phase2Script_data;
}

/* 0x0033F590 */
void *Room87_Phase5Script(void *o) { return Room87_Phase5Script_data; }   /* Room87_vtable +0x20 */

/* 0x0033F5A0 */
u32 Room87_ActionScript(void *self, s32 i) {
    return Room87_ActionScripts[i];
}

/* (self->*Room87_CmdTable[i])(a, b) */
/* 0x0033F5C0 */
s32 Room87_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room87_CmdTable[i & 0xFF], a, b);
}

/* 0x0033F5F0 */
s32 Room87_Cmd00(void) { return slam_shake(); }
