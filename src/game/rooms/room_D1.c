/* Room 0xD1: its event handler class (vtable RoomD1_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomD1_vtable[];
extern u8 RoomD1_Phase2Script_data[], RoomD1_ActionScripts[];
extern char D_0047B298[];
extern const char str_59_59_2[];

extern u8 RoomD1_EnterScript_data[];
extern u8 RoomD1_CharEnterScript_data[];
extern u8 RoomD1_Phase1Script_data[];
extern u8 RoomD1_Phase3Script_data[];
extern u8 RoomD1_Table38_data[];
extern PTMF RoomD1_CmdTable[];
extern PTMF RoomD1_CondTable[];

/* 0x0036DF30 */
void *RoomD1_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD1_vtable, RoomBase_vtable); }

/* 0x0036DF90 */
void *RoomD1_EnterScript(void) {
    return RoomD1_EnterScript_data;
}

/* 0x0036DFA0 */
void *RoomD1_CharEnterScript(void) {
    return RoomD1_CharEnterScript_data;
}

/* 0x0036DFB0 */
void *RoomD1_Phase1Script(void) {
    return RoomD1_Phase1Script_data;
}

/* 0x0036DFC0 */
void *RoomD1_Phase2Script(void *o) { return RoomD1_Phase2Script_data; }   /* RoomD1_vtable +0x14 */

/* 0x0036DFD0 */
void *RoomD1_Phase3Script(void) {
    return RoomD1_Phase3Script_data;
}

/* 0x0036DFE0 */
u32 RoomD1_ActionScript(void *o, s32 i) { return ((u32 *)RoomD1_ActionScripts)[i]; }   /* RoomD1_vtable +0x24 */

/* 0x0036E000 */
void *RoomD1_Table38(void) {
    return RoomD1_Table38_data;
}

/* (self->*RoomD1_CondTable[i])(a, b) */
/* 0x0036E010 */
s32 RoomD1_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD1_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036E040 */
s32 RoomD1_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomD1_CmdTable[i])(a, b) */
/* 0x0036E060 */
s32 RoomD1_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD1_CmdTable[i & 0xFF], a, b);
}

/* room 0xD1: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
 * (clock_draw; a frame hook). */
/* 0x0036E090 */
s32 RoomD1_Cmd01(void) { return clock_draw(D_0047B298, str_59_59_2); }

/* as Room106_Cmd00 */
/* 0x0036E1E0 */
s32 RoomD1_Cmd00(void) {
    Effect_New(gEffects, 0xC0, effect_C0_init);
    return 1;
}
