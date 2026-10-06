/* Room 0x28: its event handler class (vtable Room28_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "pursuer.h"

extern void *RoomBase_vtable[];
extern void *Room28_vtable[];

extern u8 Room28_EnterScript_data[];
extern u8 Room28_CharEnterScript_data[];
extern u8 Room28_Phase1Script_data[];
extern u8 Room28_Phase2Script_data[];
extern u8 Room28_Phase5Script_data[];
extern u32 Room28_ActionScripts[];
extern u8 Room28_Table38_data[];
extern u32 Room28_ObjectNames[];

extern PTMF Room28_CondTable[];

/* 0x002B0CF0 */
void *Room28_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room28_vtable, RoomBase_vtable); }

/* 0x002B0D50 */
void *Room28_EnterScript(void) {
    return Room28_EnterScript_data;
}

/* 0x002B0D60 */
void *Room28_CharEnterScript(void) {
    return Room28_CharEnterScript_data;
}

/* 0x002B0D70 */
void *Room28_Phase1Script(void) {
    return Room28_Phase1Script_data;
}

/* 0x002B0D80 */
void *Room28_Phase2Script(void) {
    return Room28_Phase2Script_data;
}

/* 0x002B0D90 */
void *Room28_Phase5Script(void) {
    return Room28_Phase5Script_data;
}

/* 0x002B0DA0 */
u32 Room28_ActionScript(void *self, s32 i) {
    return Room28_ActionScripts[i];
}

/* 0x002B0DC0 */
void *Room28_Table38(void) {
    return Room28_Table38_data;
}

/* 0x002B0DD0 */
u32 Room28_ObjectName(void *self, s32 i) {
    return Room28_ObjectNames[i];
}

/* (self->*Room28_CondTable[i])(a, b) */
/* 0x002B0DF0 */
s32 Room28_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room28_CondTable[i & 0xFF], a, b);
}

/* the pursuer's Pursuer_GrabHewieBehind */
/* 0x002B0E20 */
s32 Room28_Cond00(void) {
    return Pursuer_GrabHewieBehind((Pursuer *)gCharPursuer);
}
