/* Room 0xD0: its event handler class (vtable RoomD0_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomD0_vtable[];
extern u8 RoomD0_Phase2Script_data[], RoomD0_ActionScripts[];
extern char D_0047B290[];
extern const char D_004636D8[];

extern u8 RoomD0_EnterScript_data[];
extern u8 RoomD0_CharEnterScript_data[];
extern u8 RoomD0_Phase1Script_data[];
extern u8 RoomD0_Phase3Script_data[];
extern u8 RoomD0_Table38_data[];
extern PTMF RoomD0_CmdTable[];
extern PTMF RoomD0_CondTable[];

/* 0x0036DA30 */
void *RoomD0_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD0_vtable, RoomBase_vtable); }

/* 0x0036DA90 */
void *RoomD0_EnterScript(void) {
    return RoomD0_EnterScript_data;
}

/* 0x0036DAA0 */
void *RoomD0_CharEnterScript(void) {
    return RoomD0_CharEnterScript_data;
}

/* 0x0036DAB0 */
void *RoomD0_Phase1Script(void) {
    return RoomD0_Phase1Script_data;
}

/* 0x0036DAC0 */
void *RoomD0_Phase2Script(void *o) { return RoomD0_Phase2Script_data; }   /* RoomD0_vtable +0x14 */

/* 0x0036DAD0 */
void *RoomD0_Phase3Script(void) {
    return RoomD0_Phase3Script_data;
}

/* 0x0036DAE0 */
u32 RoomD0_ActionScript(void *o, s32 i) { return ((u32 *)RoomD0_ActionScripts)[i]; }   /* RoomD0_vtable +0x24 */

/* 0x0036DB00 */
void *RoomD0_Table38(void) {
    return RoomD0_Table38_data;
}

/* (self->*RoomD0_CondTable[i])(a, b) */
/* 0x0036DB10 */
s32 RoomD0_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD0_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036DB40 */
s32 RoomD0_ProgressCall(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomD0_CmdTable[i])(a, b) */
/* 0x0036DB60 */
s32 RoomD0_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD0_CmdTable[i & 0xFF], a, b);
}

/* 0x0036DB90 */
s32 RoomD0_ClockDrawSaved(void) { return clock_draw_saved(D_0047B290, D_004636D8); }

/* 0x0036DE60 */
s32 RoomD0_ClockSave(void) { return clock_save(); }

/* 0x0036DEE0 */
s32 RoomD0_ClockStop(void) { return clock_stop(); }
