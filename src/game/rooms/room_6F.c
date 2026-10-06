/* Room 0x6F: its event handler class (vtable D_00477710, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477710[];
extern u8 D_0047AEBC[];

extern u32 D_0043A8F0[];
extern u32 D_0043AA40[];
extern u32 D_0043AB10[];

extern u32 D_0043ACB0[];
extern u32 D_0043AFD0[];
extern u32 D_0043B010[];
extern u32 D_0047AEC0[];

extern PTMF D_01991948[];
extern PTMF D_01991958[];

/* 0x00344E80 */
void *Room6F_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00477710, D_0046DB80); }

/* 0x00344EE0 */
void *Room6F_EnterScript(void) {
    return D_0043A8F0;
}

/* 0x00344EF0 */
void *Room6F_CharEnterScript(void) {
    return D_0043AA40;
}

/* 0x00344F00 */
void *Room6F_Phase1Script(void) {
    return D_0043AB10;
}

/* 0x00344F10 */
void *Room6F_Phase2Script(void) {
    return D_0043ACB0;
}

/* 0x00344F20 */
void *Room6F_Phase5Script(void *o) { return D_0047AEBC; }   /* D_00477710 +0x20 */

/* 0x00344F30 */
u32 Room6F_ActionScript(void *self, s32 i) {
    return D_0043AFD0[i];
}

/* 0x00344F50 */
void *Room6F_Table38(void) {
    return D_0043B010;
}

/* 0x00344F60 */
u32 Room6F_ObjectName(void *self, s32 i) {
    return D_0047AEC0[i];
}

/* (self->*D_01991958[i])(a, b) */
/* 0x00344F80 */
s32 Room6F_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991958[i & 0xFF], a, b);
}

/* 0x00344FB0 */
s32 Room6F_Cond00(void) {
    return 0;
}

/* (self->*D_01991948[i])(a, b) */
/* 0x00344FC0 */
s32 Room6F_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991948[i & 0xFF], a, b);
}

/* 0x00344FF0 */
s32 Room6F_Cmd00(void) {
    return 0x1;
}
