/* Room 0xE4: its event handler class (vtable D_0047A550, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A550[];
extern u8 D_0047B0E8[], D_0047B0F0[];
extern const char *D_00449540;   /* a room object's name */
extern char D_0047B318[];
extern const char D_004639B8[];

extern u8 D_00449250[];
extern u8 D_004492C0[];
extern u8 D_00449380[];
extern u8 D_004494F0[];
extern u8 D_00449558[];
extern PTMF D_01991D90[];
extern PTMF D_01991DA8[];

/* 0x00379820 */
void *RoomE4_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0047A550, D_0046DB80); }

/* 0x00379880 */
void *RoomE4_EnterScript(void) {
    return D_00449250;
}

/* 0x00379890 */
void *RoomE4_CharEnterScript(void) {
    return D_004492C0;
}

/* 0x003798A0 */
void *RoomE4_Phase1Script(void) {
    return D_00449380;
}

/* 0x003798B0 */
void *RoomE4_Phase2Script(void *o) { return D_0047B0E8; }   /* D_0047A550 +0x14 */

/* 0x003798C0 */
void *RoomE4_Phase3Script(void) {
    return D_004494F0;
}

/* 0x003798D0 */
u32 RoomE4_ActionScript(void *o, s32 i) { return ((u32 *)D_0047B0F0)[i]; }   /* D_0047A550 +0x24 */

/* 0x003798F0 */
void *RoomE4_Table38(void) {
    return D_00449558;
}

/* 0x00379900 */
void *RoomE4_ObjectName(void *self, s32 i) {
    return ((void **)&D_00449540)[i];   /* its table of names (one is reached by name too) */
}

/* (self->*D_01991DA8[i])(a, b) */
/* 0x00379920 */
s32 RoomE4_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991DA8[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x00379950 */
s32 RoomE4_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991D90[i])(a, b) */
/* 0x00379970 */
s32 RoomE4_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D90[i & 0xFF], a, b);
}

/* 0x003799A0 */
s32 RoomE4_Cmd01(void) { return clock_draw(D_0047B318, D_004639B8); }

/* (as Room0F_Cmd00) the room object D_00449540 by event variable 2 */
/* 0x00379AF0 */
s32 RoomE4_Cmd00(void *self, void *a1, u8 *cmd) {
    return var_fade(D_00449540, 2, cmd);
}
