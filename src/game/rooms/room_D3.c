/* Room 0xD3: its event handler class (vtable RoomD3_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomD3_vtable[];
extern u8 RoomD3_EnterScript_data[];
extern u8 RoomD3_Phase2Script_data[], RoomD3_ActionScripts[];
extern char D_0047B2A8[];
extern const char str_59_59_4[];

extern u8 RoomD3_CharEnterScript_data[];
extern u8 RoomD3_Phase1Script_data[];
extern u8 RoomD3_Phase3Script_data[];
extern u8 RoomD3_Table38_data[];
extern PTMF RoomD3_CmdTable[];
extern PTMF RoomD3_CondTable[];

/* 0x0036E550 */
void *RoomD3_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD3_vtable, RoomBase_vtable); }

/* 0x0036E5B0 */
void *RoomD3_EnterScript(void *o) { return RoomD3_EnterScript_data; }   /* RoomD3_vtable +0xC */

/* 0x0036E5C0 */
void *RoomD3_CharEnterScript(void) {
    return RoomD3_CharEnterScript_data;
}

/* 0x0036E5D0 */
void *RoomD3_Phase1Script(void) {
    return RoomD3_Phase1Script_data;
}

/* 0x0036E5E0 */
void *RoomD3_Phase2Script(void *o) { return RoomD3_Phase2Script_data; }   /* RoomD3_vtable +0x14 */

/* 0x0036E5F0 */
void *RoomD3_Phase3Script(void) {
    return RoomD3_Phase3Script_data;
}

/* 0x0036E600 */
u32 RoomD3_ActionScript(void *o, s32 i) { return ((u32 *)RoomD3_ActionScripts)[i]; }   /* RoomD3_vtable +0x24 */

/* 0x0036E620 */
void *RoomD3_Table38(void) {
    return RoomD3_Table38_data;
}

/* (self->*RoomD3_CondTable[i])(a, b) */
/* 0x0036E630 */
s32 RoomD3_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD3_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036E660 */
s32 RoomD3_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomD3_CmdTable[i])(a, b) */
/* 0x0036E680 */
s32 RoomD3_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD3_CmdTable[i & 0xFF], a, b);
}

/* room 0xD3: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
 * (clock_draw; a frame hook). */
/* 0x0036E6B0 */
s32 RoomD3_Cmd00(void) { return clock_draw(D_0047B2A8, str_59_59_4); }
