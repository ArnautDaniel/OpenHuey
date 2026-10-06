/* Room 0xE1: its event handler class (vtable D_0047A490, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A490[];
extern u8 D_0047B0C8[], D_0047B0D0[];
extern char D_0047B300[];
extern const char D_00463930[];

extern u8 D_00448590[];
extern u8 D_004485E0[];
extern u8 D_00448660[];
extern u8 D_00448740[];
extern u8 D_004488A0[];
extern PTMF D_01991D28[];
extern PTMF D_01991D38[];

/* 0x00378FE0 */
void *RoomE1_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0047A490, D_0046DB80); }

/* 0x00379040 */
void *RoomE1_EnterScript(void) {
    return D_00448590;
}

/* 0x00379050 */
void *RoomE1_CharEnterScript(void) {
    return D_004485E0;
}

/* 0x00379060 */
void *RoomE1_Phase1Script(void) {
    return D_00448660;
}

/* 0x00379070 */
void *RoomE1_Phase2Script(void *o) { return D_0047B0C8; }   /* D_0047A490 +0x14 */

/* 0x00379080 */
void *RoomE1_Phase3Script(void) {
    return D_00448740;
}

/* 0x00379090 */
u32 RoomE1_ActionScript(void *o, s32 i) { return ((u32 *)D_0047B0D0)[i]; }   /* D_0047A490 +0x24 */

/* 0x003790B0 */
void *RoomE1_Table38(void) {
    return D_004488A0;
}

/* (self->*D_01991D38[i])(a, b) */
/* 0x003790C0 */
s32 RoomE1_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D38[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x003790F0 */
s32 RoomE1_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991D28[i])(a, b) */
/* 0x00379110 */
s32 RoomE1_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D28[i & 0xFF], a, b);
}

/* 0x00379140 */
s32 RoomE1_Cmd00(void) { return clock_draw(D_0047B300, D_00463930); }
