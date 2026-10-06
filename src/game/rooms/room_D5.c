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
extern const char D_00463700[];

extern u8 D_00447300[];
extern u8 D_00447330[];
extern u8 D_00447460[];
extern u8 D_00447550[];
extern u8 D_00447560[];
extern void *D_00447608[];
extern u8 D_00447660[];
extern PTMF D_01991B90[];
extern PTMF D_01991BB8[];

/* 0x0036EA80 */
void *RoomD5_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD5_vtable, RoomBase_vtable); }

/* 0x0036EAE0 */
void *RoomD5_EnterScript(void) {
    return D_00447300;
}

/* 0x0036EAF0 */
void *RoomD5_CharEnterScript(void) {
    return D_00447330;
}

/* 0x0036EB00 */
void *RoomD5_Phase1Script(void) {
    return D_00447460;
}

/* 0x0036EB10 */
void *RoomD5_Phase3Script(void) {
    return D_00447560;
}

/* 0x0036EB20 */
void *RoomD5_Phase5Script(void) {
    return D_00447550;
}

/* 0x0036EB30 */
void *RoomD5_ActionScript(void *self, s32 i) {
    return D_00447608[i];
}

/* 0x0036EB50 */
void *RoomD5_Table38(void) {
    return D_00447660;
}

/* (self->*D_01991BB8[i])(a, b) */
/* 0x0036EB60 */
s32 RoomD5_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991BB8[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036EB90 */
s32 RoomD5_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991B90[i])(a, b) */
/* 0x0036EBB0 */
s32 RoomD5_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B90[i & 0xFF], a, b);
}

/* 0x0036EBE0 */
s32 RoomD5_Cmd02(void) { return clock_draw(D_0047B2B8, D_00463700); }

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
