/* Room 0xD1: its event handler class (vtable D_0047A0B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A0B0[];
extern u8 D_0047B048[], D_0047B050[];
extern char D_0047B298[];
extern const char D_004636E0[];

extern u8 D_00446C00[];
extern u8 D_00446C10[];
extern u8 D_00446C90[];
extern u8 D_00446CF0[];
extern u8 D_00446D40[];
extern PTMF D_01991B00[];
extern PTMF D_01991B18[];

/* 0x0036DF30 */
void *RoomD1_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0047A0B0, D_0046DB80); }

/* 0x0036DF90 */
void *RoomD1_EnterScript(void) {
    return D_00446C00;
}

/* 0x0036DFA0 */
void *RoomD1_CharEnterScript(void) {
    return D_00446C10;
}

/* 0x0036DFB0 */
void *RoomD1_Phase1Script(void) {
    return D_00446C90;
}

/* 0x0036DFC0 */
void *RoomD1_Phase2Script(void *o) { return D_0047B048; }   /* D_0047A0B0 +0x14 */

/* 0x0036DFD0 */
void *RoomD1_Phase3Script(void) {
    return D_00446CF0;
}

/* 0x0036DFE0 */
u32 RoomD1_ActionScript(void *o, s32 i) { return ((u32 *)D_0047B050)[i]; }   /* D_0047A0B0 +0x24 */

/* 0x0036E000 */
void *RoomD1_Table38(void) {
    return D_00446D40;
}

/* (self->*D_01991B18[i])(a, b) */
/* 0x0036E010 */
s32 RoomD1_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B18[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036E040 */
s32 RoomD1_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991B00[i])(a, b) */
/* 0x0036E060 */
s32 RoomD1_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B00[i & 0xFF], a, b);
}

/* 0x0036E090 */
s32 RoomD1_Cmd01(void) { return clock_draw(D_0047B298, D_004636E0); }

/* as Room106_Cmd00 */
/* 0x0036E1E0 */
s32 RoomD1_Cmd00(void) {
    Effect_New(gEffects, 0xC0, effect_C0_init);
    return 1;
}
