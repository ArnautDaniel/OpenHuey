/* Room 0xD4: its event handler class (vtable RoomD4_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomD4_vtable[];
extern u8 RoomD4_EnterScript_data[];
extern char D_0047B2B0[];
extern const char str_59_59_5[];

extern u8 RoomD4_CharEnterScript_data[];
extern u8 RoomD4_Phase1Script_data[];
extern u8 RoomD4_Phase3Script_data[];
extern u8 RoomD4_Table38_data[];
extern PTMF RoomD4_CmdTable[];
extern PTMF RoomD4_CondTable[];

/* 0x0036E800 */
void *RoomD4_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD4_vtable, RoomBase_vtable); }

/* 0x0036E860 */
void *RoomD4_EnterScript(void *o) { return RoomD4_EnterScript_data; }   /* RoomD4_vtable +0xC */

/* 0x0036E870 */
void *RoomD4_CharEnterScript(void) {
    return RoomD4_CharEnterScript_data;
}

/* 0x0036E880 */
void *RoomD4_Phase1Script(void) {
    return RoomD4_Phase1Script_data;
}

/* 0x0036E890 */
void *RoomD4_Phase3Script(void) {
    return RoomD4_Phase3Script_data;
}

/* 0x0036E8A0 */
void *RoomD4_Table38(void) {
    return RoomD4_Table38_data;
}

/* (self->*RoomD4_CondTable[i])(a, b) */
/* 0x0036E8B0 */
s32 RoomD4_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD4_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036E8E0 */
s32 RoomD4_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomD4_CmdTable[i])(a, b) */
/* 0x0036E900 */
s32 RoomD4_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD4_CmdTable[i & 0xFF], a, b);
}

/* room 0xD4: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
 * (clock_draw; a frame hook). */
/* 0x0036E930 */
s32 RoomD4_Cmd00(void) { return clock_draw(D_0047B2B0, str_59_59_5); }
