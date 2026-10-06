/* Room 0xE3: its event handler class (vtable D_0047A510, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A510[];
extern u8 D_0047B0E0[], D_0047B0E4[];
extern char D_0047B310[];
extern const char D_00463958[];

extern u8 D_00448D20[];
extern u8 D_00448DA0[];
extern u8 D_00448E30[];
extern u8 D_00448F30[];
extern u8 D_00449010[];
extern void *D_00449210[];
extern u8 D_00449240[];
extern PTMF D_01991D68[];
extern PTMF D_01991D78[];

/* 0x00379540 */
void *RoomE3_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0047A510, D_0046DB80); }

/* 0x003795A0 */
void *RoomE3_EnterScript(void) {
    return D_00448D20;
}

/* 0x003795B0 */
void *RoomE3_CharEnterScript(void) {
    return D_00448DA0;
}

/* 0x003795C0 */
void *RoomE3_Phase1Script(void) {
    return D_00448E30;
}

/* 0x003795D0 */
void *RoomE3_Phase2Script(void *o) { return D_0047B0E0; }   /* D_0047A510 +0x14 */

/* 0x003795E0 */
void *RoomE3_ActionScript(void *self, s32 i) {
    return D_00449210[i];
}

/* 0x00379600 */
void *RoomE3_Phase3Script(void) {
    return D_00448F30;
}

/* 0x00379610 */
void *RoomE3_Phase5Script(void) {
    return D_00449010;
}

/* 0x00379620 */
void *RoomE3_Table38(void) {
    return D_00449240;
}

/* 0x00379630 */
u32 RoomE3_ObjectName(void *o, s32 i) { return ((u32 *)D_0047B0E4)[i]; }   /* D_0047A510 +0x34 */

/* (self->*D_01991D78[i])(a, b) */
/* 0x00379650 */
s32 RoomE3_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D78[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x00379680 */
s32 RoomE3_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991D68[i])(a, b) */
/* 0x003796A0 */
s32 RoomE3_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D68[i & 0xFF], a, b);
}

/* 0x003796D0 */
s32 RoomE3_Cmd00(void) { return clock_draw(D_0047B310, D_00463958); }
