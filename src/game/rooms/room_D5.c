/* Room 0xD5: its event handler class (vtable RoomD5_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *RoomBase_vtable[];
extern void *RoomD5_vtable[];
extern s32 Kind26_MoveDone(Character *c);
/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern s32 D_0047B2C0;
extern char D_0047B2B8[];
extern const char str_59_59_6[];

extern u8 RoomD5_EnterScript_data[];
extern u8 RoomD5_CharEnterScript_data[];
extern u8 RoomD5_Phase1Script_data[];
extern u8 RoomD5_Phase5Script_data[];
extern u8 RoomD5_Phase3Script_data[];
extern void *RoomD5_ActionScripts[];
extern u8 RoomD5_Table38_data[];
extern PTMF RoomD5_CmdTable[];
extern PTMF RoomD5_CondTable[];

/* 0x0036EA80 */
void *RoomD5_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD5_vtable, RoomBase_vtable); }

/* 0x0036EAE0 */
void *RoomD5_EnterScript(void) {
    return RoomD5_EnterScript_data;
}

/* 0x0036EAF0 */
void *RoomD5_CharEnterScript(void) {
    return RoomD5_CharEnterScript_data;
}

/* 0x0036EB00 */
void *RoomD5_Phase1Script(void) {
    return RoomD5_Phase1Script_data;
}

/* 0x0036EB10 */
void *RoomD5_Phase3Script(void) {
    return RoomD5_Phase3Script_data;
}

/* 0x0036EB20 */
void *RoomD5_Phase5Script(void) {
    return RoomD5_Phase5Script_data;
}

/* 0x0036EB30 */
void *RoomD5_ActionScript(void *self, s32 i) {
    return RoomD5_ActionScripts[i];
}

/* 0x0036EB50 */
void *RoomD5_Table38(void) {
    return RoomD5_Table38_data;
}

/* (self->*RoomD5_CondTable[i])(a, b) */
/* 0x0036EB60 */
s32 RoomD5_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD5_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036EB90 */
s32 RoomD5_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomD5_CmdTable[i])(a, b) */
/* 0x0036EBB0 */
s32 RoomD5_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD5_CmdTable[i & 0xFF], a, b);
}

/* room 0xD5: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
 * (clock_draw; a frame hook). */
/* 0x0036EBE0 */
s32 RoomD5_Cmd02(void) { return clock_draw(D_0047B2B8, str_59_59_6); }

/* as Room109_Cmd01 */
/* 0x0036ED30 */
s32 RoomD5_Cmd01(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B2C0, cmd, 1.0f); }

/* (as Room109_Cmd00)  character kind 0x1A: byte 3 0 starts Kind26_MoveTo(2, -6, 257); else waits (2) until
 * Kind26_MoveDone says done */
/* 0x0036EDD0 */
s32 RoomD5_Cmd00(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[Progress_SlotOfId(gProgress, 0x1A) & 0xFF];

    if (cmd[3] == 0) {
        Kind26_MoveTo((u8 *)c, 2, -6.0f, 257.0f);
        return 1;
    }
    return Kind26_MoveDone(c) == 0 ? 2 : 1;
}
