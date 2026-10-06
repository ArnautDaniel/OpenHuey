/* Room 0xE2: its event handler class (vtable D_0047A4D0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A4D0[];
extern u8 D_0047B0D4[], D_0047B0DC[];
extern char D_0047B308[];
extern const char D_00463938[];

extern u8 D_004488B0[];
extern u8 D_00448990[];
extern u8 D_00448A10[];
extern u8 D_00448CD0[];
extern u8 D_00448D10[];
extern PTMF D_01991D48[];
extern PTMF D_01991D58[];

/* 0x00379290 */
void *RoomE2_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0047A4D0, D_0046DB80); }

/* 0x003792F0 */
void *RoomE2_EnterScript(void) {
    return D_004488B0;
}

/* 0x00379300 */
void *RoomE2_CharEnterScript(void) {
    return D_00448990;
}

/* 0x00379310 */
void *RoomE2_Phase1Script(void) {
    return D_00448A10;
}

/* 0x00379320 */
void *RoomE2_Phase2Script(void *o) { return D_0047B0D4; }   /* D_0047A4D0 +0x14 */

/* 0x00379330 */
void *RoomE2_Phase3Script(void) {
    return D_00448CD0;
}

/* 0x00379340 */
u32 RoomE2_ActionScript(void *o, s32 i) { return ((u32 *)D_0047B0DC)[i]; }   /* D_0047A4D0 +0x24 */

/* 0x00379360 */
void *RoomE2_Table38(void) {
    return D_00448D10;
}

/* (self->*D_01991D58[i])(a, b) */
/* 0x00379370 */
s32 RoomE2_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D58[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x003793A0 */
s32 RoomE2_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991D48[i])(a, b) */
/* 0x003793C0 */
s32 RoomE2_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D48[i & 0xFF], a, b);
}

/* 0x003793F0 */
s32 RoomE2_Cmd00(void) { return clock_draw(D_0047B308, D_00463938); }
