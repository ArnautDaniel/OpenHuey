/* Room 0x30: its event handler class (vtable Room30_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "item.h"
#include "pursuer.h"

extern void *RoomBase_vtable[];
extern void *Room30_vtable[];

extern u8 Room30_EnterScript_data[], Room30_CharEnterScript_data[], Room30_Phase1Script_data[], Room30_Phase2Script_data[], Room30_Phase5Script_data[];
extern void *Room30_ActionScripts[];
extern u8 Room30_Table38_data[];
extern void *Room30_ObjectNames[];

extern PTMF Room30_CondTable[];

/* 0x002CCA90 */
void *Room30_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room30_vtable, RoomBase_vtable); }

/* 0x002CCAF0 */
void *Room30_EnterScript(void) { return Room30_EnterScript_data; }

/* 0x002CCB00 */
void *Room30_CharEnterScript(void) { return Room30_CharEnterScript_data; }

/* 0x002CCB10 */
void *Room30_Phase1Script(void) { return Room30_Phase1Script_data; }

/* 0x002CCB20 */
void *Room30_Phase2Script(void) { return Room30_Phase2Script_data; }

/* 0x002CCB30 */
void *Room30_Phase5Script(void) { return Room30_Phase5Script_data; }

/* 0x002CCB40 */
void *Room30_ActionScript(void *self, s32 i) { return Room30_ActionScripts[i]; }

/* 0x002CCB60 */
void *Room30_Table38(void) { return Room30_Table38_data; }

/* 0x002CCB70 */
void *Room30_ObjectName(void *self, s32 i) { return Room30_ObjectNames[i]; }

/* (self->*Room30_CondTable[i])(a, b) */
/* 0x002CCB90 */
s32 Room30_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room30_CondTable[i & 0xFF], a, b);
}

/* room 0x30 (Room30_Cond00_ptmf): the pursuer's Pursuer_GrabHewieBehind */
/* 0x002CCBC0 */
s32 Room30_Cond00(void) {
    return Pursuer_GrabHewieBehind((Pursuer *)gCharPursuer);
}
