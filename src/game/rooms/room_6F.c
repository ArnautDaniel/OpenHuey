/* Room 0x6F: its event handler class (vtable Room6F_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room6F_vtable[];
extern u8 Room6F_Phase5Script_data[];

extern u32 Room6F_EnterScript_data[];
extern u32 Room6F_CharEnterScript_data[];
extern u32 Room6F_Phase1Script_data[];

extern u32 Room6F_Phase2Script_data[];
extern u32 Room6F_ActionScripts[];
extern u32 Room6F_Table38_data[];
extern u32 Room6F_ObjectNames[];

extern PTMF Room6F_CmdTable[];
extern PTMF Room6F_CondTable[];

/* 0x00344E80 */
void *Room6F_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room6F_vtable, RoomBase_vtable); }

/* 0x00344EE0 */
void *Room6F_EnterScript(void) {
    return Room6F_EnterScript_data;
}

/* 0x00344EF0 */
void *Room6F_CharEnterScript(void) {
    return Room6F_CharEnterScript_data;
}

/* 0x00344F00 */
void *Room6F_Phase1Script(void) {
    return Room6F_Phase1Script_data;
}

/* 0x00344F10 */
void *Room6F_Phase2Script(void) {
    return Room6F_Phase2Script_data;
}

/* 0x00344F20 */
void *Room6F_Phase5Script(void *o) { return Room6F_Phase5Script_data; }   /* Room6F_vtable +0x20 */

/* 0x00344F30 */
u32 Room6F_ActionScript(void *self, s32 i) {
    return Room6F_ActionScripts[i];
}

/* 0x00344F50 */
void *Room6F_Table38(void) {
    return Room6F_Table38_data;
}

/* 0x00344F60 */
u32 Room6F_ObjectName(void *self, s32 i) {
    return Room6F_ObjectNames[i];
}

/* (self->*Room6F_CondTable[i])(a, b) */
/* 0x00344F80 */
s32 Room6F_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room6F_CondTable[i & 0xFF], a, b);
}

/* 0x00344FB0 */
s32 Room6F_Cond00(void) {
    return 0;
}

/* (self->*Room6F_CmdTable[i])(a, b) */
/* 0x00344FC0 */
s32 Room6F_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room6F_CmdTable[i & 0xFF], a, b);
}

/* 0x00344FF0 */
s32 Room6F_Cmd00(void) {
    return 0x1;
}
