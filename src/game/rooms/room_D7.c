/* Room 0xD7: its event handler class (vtable RoomD7_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomD7_vtable[];
extern u8 RoomD7_Phase2Script_data[];
extern u8 RoomD7_Phase5Script_data[], RoomD7_ActionScripts[];
extern s32 D_0047B2D8;
extern char D_0047B2D0[];
extern const char str_59_59_8[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern u8 RoomD7_EnterScript_data[];
extern u8 RoomD7_CharEnterScript_data[];
extern u8 RoomD7_Phase1Script_data[];
extern u8 RoomD7_Phase3Script_data[];
extern u8 RoomD7_Table38_data[];
extern PTMF RoomD7_CondTable[];
extern PTMF RoomD7_CmdTable[];

/* 0x0036F4A0 */
void *RoomD7_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD7_vtable, RoomBase_vtable); }

/* 0x0036F500 */
void *RoomD7_EnterScript(void) {
    return RoomD7_EnterScript_data;
}

/* 0x0036F510 */
void *RoomD7_CharEnterScript(void) {
    return RoomD7_CharEnterScript_data;
}

/* 0x0036F520 */
void *RoomD7_Phase1Script(void) {
    return RoomD7_Phase1Script_data;
}

/* 0x0036F530 */
void *RoomD7_Phase2Script(void *o) { return RoomD7_Phase2Script_data; }   /* RoomD7_vtable +0x14 */

/* 0x0036F540 */
void *RoomD7_Phase3Script(void) {
    return RoomD7_Phase3Script_data;
}

/* 0x0036F550 */
void *RoomD7_Phase5Script(void *o) { return RoomD7_Phase5Script_data; }   /* RoomD7_vtable +0x20 */

/* 0x0036F560 */
u32 RoomD7_ActionScript(void *o, s32 i) { return ((u32 *)RoomD7_ActionScripts)[i]; }   /* RoomD7_vtable +0x24 */

/* 0x0036F580 */
void *RoomD7_Table38(void) {
    return RoomD7_Table38_data;
}

/* (self->*RoomD7_CmdTable[i])(a, b) */
/* 0x0036F590 */
s32 RoomD7_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD7_CmdTable[i & 0xFF], a, b);
}

/* as Room107_Cmd01 */
/* 0x0036F5C0 */
s32 RoomD7_Cmd02(void) {
    Effect_New(gEffects, 0x10, effect_10_init);
    return 1;
}

/* room 0xD7: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
 * (clock_draw; a frame hook). */
/* 0x0036F690 */
s32 RoomD7_Cmd01(void) { return clock_draw(D_0047B2D0, str_59_59_8); }

/* as Room107_Cmd00 */
/* 0x0036F7E0 */
s32 RoomD7_Cmd00(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B2D8, cmd, 2.0f); }

/* (self->*RoomD7_CondTable[i])(a, b) */
/* 0x0036F880 */
s32 RoomD7_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD7_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036F8B0 */
s32 RoomD7_Cond01(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* room 0xD7: a noise of loudness 0x20 or more was made in this room last frame (the progress'
 * noise requests kept at +0x10D4). */
/* 0x0036F8D0 */
s32 RoomD7_Cond00(void) {
    u8 *e = (u8 *)gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0xD7 && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}
