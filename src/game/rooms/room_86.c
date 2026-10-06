/* Room 0x86: its event handler class (vtable Room86_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room86_vtable[];

extern u8 Room86_EnterScript_data[];
extern u8 Room86_CharEnterScript_data[];
extern u8 Room86_Phase1Script_data[];
extern u8 Room86_Table38_data[];
extern void *Room86_ActionScripts[];
extern void *Room86_ObjectNames[];

extern PTMF Room86_CmdTable[];

/* 0x0032C690 */
void *Room86_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room86_vtable, RoomBase_vtable); }

/* 0x0032C6F0 */
void *Room86_EnterScript(void) {
    return Room86_EnterScript_data;
}

/* 0x0032C700 */
void *Room86_CharEnterScript(void) {
    return Room86_CharEnterScript_data;
}

/* 0x0032C710 */
void *Room86_Phase1Script(void) {
    return Room86_Phase1Script_data;
}

/* 0x0032C720 */
void *Room86_ActionScript(void *self, s32 i) {
    return Room86_ActionScripts[i];
}

/* 0x0032C740 */
void *Room86_Table38(void) {
    return Room86_Table38_data;
}

/* 0x0032C750 */
void *Room86_ObjectName(void *self, s32 i) {
    return Room86_ObjectNames[i];
}

/* (self->*Room86_CmdTable[i])(a, b) */
/* 0x0032C770 */
s32 Room86_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room86_CmdTable[i & 0xFF], a, b);
}

/* 0x0032C7A0 */
s32 Room86_Cmd00(void) { return slam_shake(); }
