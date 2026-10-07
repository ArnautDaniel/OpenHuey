/* Room 0xE8: its event handler class (vtable RoomE8_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomE8_vtable[];
extern u8 RoomE8_EnterScript_data[];
extern u8 RoomE8_Phase2Script_data[], RoomE8_ActionScripts[];
extern char D_0047B338[];
extern const char str_59_59_19[];

extern u8 RoomE8_CharEnterScript_data[];
extern u8 RoomE8_Phase1Script_data[];
extern u8 RoomE8_Phase3Script_data[];
extern u8 RoomE8_Table38_data[];
extern PTMF RoomE8_CmdTable[];
extern PTMF RoomE8_CondTable[];

/* 0x0037A8D0 */
void *RoomE8_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE8_vtable, RoomBase_vtable); }

/* 0x0037A930 */
void *RoomE8_EnterScript(void *o) { return RoomE8_EnterScript_data; }   /* RoomE8_vtable +0xC */

/* 0x0037A940 */
void *RoomE8_CharEnterScript(void) {
    return RoomE8_CharEnterScript_data;
}

/* 0x0037A950 */
void *RoomE8_Phase1Script(void) {
    return RoomE8_Phase1Script_data;
}

/* 0x0037A960 */
void *RoomE8_Phase2Script(void *o) { return RoomE8_Phase2Script_data; }   /* RoomE8_vtable +0x14 */

/* 0x0037A970 */
void *RoomE8_Phase3Script(void) {
    return RoomE8_Phase3Script_data;
}

/* 0x0037A980 */
u32 RoomE8_ActionScript(void *o, s32 i) { return ((u32 *)RoomE8_ActionScripts)[i]; }   /* RoomE8_vtable +0x24 */

/* 0x0037A9A0 */
void *RoomE8_Table38(void) {
    return RoomE8_Table38_data;
}

/* (self->*RoomE8_CondTable[i])(a, b) */
/* 0x0037A9B0 */
s32 RoomE8_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE8_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0037A9E0 */
s32 RoomE8_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomE8_CmdTable[i])(a, b) */
/* 0x0037AA00 */
s32 RoomE8_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE8_CmdTable[i & 0xFF], a, b);
}

/* room 0xE8: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
 * (clock_draw; a frame hook). */
/* 0x0037AA30 */
s32 RoomE8_Cmd00(void) { return clock_draw(D_0047B338, str_59_59_19); }
