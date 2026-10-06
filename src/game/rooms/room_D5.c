/* Room 0xD5: its event handler class (vtable D_0047A1B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *D_0046DB80[];
extern void *D_0047A1B0[];
extern s32 func_0032D150(Character *c);
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

void *func_0036EA80(void *o, s32 flags) { return room_dtor(o, flags, D_0047A1B0, D_0046DB80); }

void *func_0036EAE0(void) {
    return D_00447300;
}

void *func_0036EAF0(void) {
    return D_00447330;
}

void *func_0036EB00(void) {
    return D_00447460;
}

void *func_0036EB10(void) {
    return D_00447560;
}

void *func_0036EB20(void) {
    return D_00447550;
}

void *func_0036EB30(void *self, s32 i) {
    return D_00447608[i];
}

void *func_0036EB50(void) {
    return D_00447660;
}

/* (self->*D_01991BB8[i])(a, b) */
s32 func_0036EB60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991BB8[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036EB90(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991B90[i])(a, b) */
s32 func_0036EBB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B90[i & 0xFF], a, b);
}

s32 func_0036EBE0(void) { return clock_draw(D_0047B2B8, D_00463700); }

/* as func_002E7560 */
s32 func_0036ED30(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B2C0, cmd, 1.0f); }

/* (as func_002E7600)  character kind 0x1A: byte 3 0 starts func_0032D270(2, -6, 257); else waits (2) until
 * func_0032D150 says done */
s32 func_0036EDD0(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[func_001770D0(gProgress, 0x1A) & 0xFF];

    if (cmd[3] == 0) {
        func_0032D270((u8 *)c, 2, -6.0f, 257.0f);
        return 1;
    }
    return func_0032D150(c) == 0 ? 2 : 1;
}
