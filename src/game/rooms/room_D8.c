/* Room 0xD8: its event handler class (vtable RoomD8_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomD8_vtable[];
extern u8 RoomD8_Phase2Script_data[], RoomD8_Phase5Script_data[], RoomD8_ActionScripts[];
extern s32 D_0047B2E8;
extern char D_0047B2E0[];
extern const char str_59_59_9[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern u8 RoomD8_EnterScript_data[];
extern u8 RoomD8_CharEnterScript_data[];
extern u8 RoomD8_Phase1Script_data[];
extern u8 RoomD8_Phase3Script_data[];
extern u8 RoomD8_Table38_data[];
extern PTMF RoomD8_CondTable[];
extern PTMF RoomD8_CmdTable[];

/* 0x0036F930 */
void *RoomD8_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD8_vtable, RoomBase_vtable); }

/* 0x0036F990 */
void *RoomD8_EnterScript(void) {
    return RoomD8_EnterScript_data;
}

/* 0x0036F9A0 */
void *RoomD8_CharEnterScript(void) {
    return RoomD8_CharEnterScript_data;
}

/* 0x0036F9B0 */
void *RoomD8_Phase1Script(void) {
    return RoomD8_Phase1Script_data;
}

/* 0x0036F9C0 */
void *RoomD8_Phase2Script(void *o) { return RoomD8_Phase2Script_data; }   /* RoomD8_vtable +0x14 */

/* 0x0036F9D0 */
void *RoomD8_Phase3Script(void) {
    return RoomD8_Phase3Script_data;
}

/* 0x0036F9E0 */
void *RoomD8_Phase5Script(void *o) { return RoomD8_Phase5Script_data; }   /* RoomD8_vtable +0x20 */

/* 0x0036F9F0 */
u32 RoomD8_ActionScript(void *o, s32 i) { return ((u32 *)RoomD8_ActionScripts)[i]; }   /* RoomD8_vtable +0x24 */

/* 0x0036FA10 */
void *RoomD8_Table38(void) {
    return RoomD8_Table38_data;
}

/* (self->*RoomD8_CmdTable[i])(a, b) */
/* 0x0036FA20 */
s32 RoomD8_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD8_CmdTable[i & 0xFF], a, b);
}

/* room 0xD8: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
 * (clock_draw; a frame hook). */
/* 0x0036FA50 */
s32 RoomD8_Cmd01(void) { return clock_draw(D_0047B2E0, str_59_59_9); }

/* as Room109_Cmd01 */
/* 0x0036FBA0 */
s32 RoomD8_Cmd00(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B2E8, cmd, 1.0f); }

/* (self->*RoomD8_CondTable[i])(a, b) */
/* 0x0036FC40 */
s32 RoomD8_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD8_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036FC70 */
s32 RoomD8_Cond01(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (as Room10A_Cond00) last frame's noise requests (gProgress +0x10D4) of kind 0xD8 / 0xD7 and
   loudness 0x20 or more */
/* 0x0036FC90 */
s32 RoomD8_Cond00(void) {
    u8 *e = (u8 *)gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0xD8 && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}
