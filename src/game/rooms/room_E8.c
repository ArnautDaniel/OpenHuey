/* Room 0xE8: its event handler class (vtable D_0047A650, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A650[];
extern u8 D_0047B120[];
extern u8 D_0047B128[], D_0047B130[];
extern char D_0047B338[];
extern const char D_00463A40[];

extern u8 D_00449EE0[];
extern u8 D_00449F60[];
extern u8 D_00449FB0[];
extern u8 D_0044A2A0[];
extern PTMF D_01991E38[];
extern PTMF D_01991E48[];

/* 0x0037A8D0 */
void *RoomE8_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0047A650, D_0046DB80); }

/* 0x0037A930 */
void *RoomE8_EnterScript(void *o) { return D_0047B120; }   /* D_0047A650 +0xC */

/* 0x0037A940 */
void *RoomE8_CharEnterScript(void) {
    return D_00449EE0;
}

/* 0x0037A950 */
void *RoomE8_Phase1Script(void) {
    return D_00449F60;
}

/* 0x0037A960 */
void *RoomE8_Phase2Script(void *o) { return D_0047B128; }   /* D_0047A650 +0x14 */

/* 0x0037A970 */
void *RoomE8_Phase3Script(void) {
    return D_00449FB0;
}

/* 0x0037A980 */
u32 RoomE8_ActionScript(void *o, s32 i) { return ((u32 *)D_0047B130)[i]; }   /* D_0047A650 +0x24 */

/* 0x0037A9A0 */
void *RoomE8_Table38(void) {
    return D_0044A2A0;
}

/* (self->*D_01991E48[i])(a, b) */
/* 0x0037A9B0 */
s32 RoomE8_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E48[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0037A9E0 */
s32 RoomE8_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991E38[i])(a, b) */
/* 0x0037AA00 */
s32 RoomE8_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E38[i & 0xFF], a, b);
}

/* 0x0037AA30 */
s32 RoomE8_Cmd00(void) { return clock_draw(D_0047B338, D_00463A40); }
