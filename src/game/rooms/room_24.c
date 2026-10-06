/* Room 0x24: its event handler class (vtable Room24_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room24_vtable[];
extern u8 Room24_Phase5Script_data[];

extern u8 Room24_EnterScript_data[];
extern u8 Room24_CharEnterScript_data[];
extern u8 Room24_Phase1Script_data[];
extern u8 Room24_Phase2Script_data[];
extern u32 Room24_ActionScripts[];
extern u8 Room24_Table38_data[];
extern u32 Room24_ObjectNames[];

extern PTMF Room24_CmdTable[];

/* 0x002AFC30 */
void *Room24_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room24_vtable, RoomBase_vtable); }

/* 0x002AFC90 */
void *Room24_EnterScript(void) {
    return Room24_EnterScript_data;
}

/* 0x002AFCA0 */
void *Room24_CharEnterScript(void) {
    return Room24_CharEnterScript_data;
}

/* 0x002AFCB0 */
void *Room24_Phase1Script(void) {
    return Room24_Phase1Script_data;
}

/* 0x002AFCC0 */
void *Room24_Phase2Script(void) {
    return Room24_Phase2Script_data;
}

/* 0x002AFCD0 */
void *Room24_Phase5Script(void *o) { return Room24_Phase5Script_data; }   /* Room24_vtable +0x20 */

/* 0x002AFCE0 */
u32 Room24_ActionScript(void *self, s32 i) {
    return Room24_ActionScripts[i];
}

/* 0x002AFD00 */
void *Room24_Table38(void) {
    return Room24_Table38_data;
}

/* 0x002AFD10 */
u32 Room24_ObjectName(void *self, s32 i) {
    return Room24_ObjectNames[i];
}

/* (self->*Room24_CmdTable[i])(a, b) */
/* 0x002AFD30 */
s32 Room24_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room24_CmdTable[i & 0xFF], a, b);
}
