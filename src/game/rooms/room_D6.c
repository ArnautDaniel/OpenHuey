/* Room 0xD6: its event handler class (vtable D_0047A1F0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A1F0[];
extern u8 D_0047B070[], D_0047B078[];
extern char D_0047B2C8[];
extern const char D_00463720[];

extern u8 D_00447680[];
extern u8 D_00447790[];
extern u8 D_00447890[];
extern u8 D_004478B0[];
extern void *D_00447A10[];
extern void *D_00447A50[];
extern u8 D_00447A60[];
extern PTMF D_01991BD0[];
extern PTMF D_01991BE8[];

/* 0x0036EE70 */
void *RoomD6_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0047A1F0, D_0046DB80); }

/* 0x0036EED0 */
void *RoomD6_EnterScript(void *o) { return D_0047B070; }   /* D_0047A1F0 +0xC */

/* 0x0036EEE0 */
void *RoomD6_CharEnterScript(void) {
    return D_00447680;
}

/* 0x0036EEF0 */
void *RoomD6_Phase1Script(void) {
    return D_00447790;
}

/* 0x0036EF00 */
void *RoomD6_Phase2Script(void *o) { return D_0047B078; }   /* D_0047A1F0 +0x14 */

/* 0x0036EF10 */
void *RoomD6_Phase3Script(void) {
    return D_00447890;
}

/* 0x0036EF20 */
void *RoomD6_ActionScript(void *self, s32 i) {
    return D_00447A10[i];
}

/* 0x0036EF40 */
void *RoomD6_Phase5Script(void) {
    return D_004478B0;
}

/* 0x0036EF50 */
void *RoomD6_Table38(void) {
    return D_00447A60;
}

/* 0x0036EF60 */
void *RoomD6_ObjectName(void *self, s32 i) {
    return D_00447A50[i];
}

/* (self->*D_01991BE8[i])(a, b) */
/* 0x0036EF80 */
s32 RoomD6_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991BE8[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036EFB0 */
s32 RoomD6_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991BD0[i])(a, b) */
/* 0x0036EFD0 */
s32 RoomD6_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991BD0[i & 0xFF], a, b);
}

/* 0x0036F000 */
s32 RoomD6_Cmd01(void) { return clock_draw(D_0047B2C8, D_00463720); }

/* (as Room105_Cmd00) pushed by the character slot byte 4 names */
/* 0x0036F150 */
s32 RoomD6_Cmd00(VObject *self, void *a1, u8 *cmd) {
    return swing_three_by(self, cmd, 1);
}
