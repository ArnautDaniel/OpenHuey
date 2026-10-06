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

void *func_00344E80(void *o, s32 flags) { return room_dtor(o, flags, D_00477710, D_0046DB80); }

void *func_00344EE0(void) {
    return D_0043A8F0;
}

void *func_00344EF0(void) {
    return D_0043AA40;
}

void *func_00344F00(void) {
    return D_0043AB10;
}

void *func_00344F10(void) {
    return D_0043ACB0;
}

void *func_00344F20(void *o) { return D_0047AEBC; }   /* D_00477710 +0x20 */

u32 func_00344F30(void *self, s32 i) {
    return D_0043AFD0[i];
}

void *func_00344F50(void) {
    return D_0043B010;
}

u32 func_00344F60(void *self, s32 i) {
    return D_0047AEC0[i];
}

/* (self->*D_01991958[i])(a, b) */
s32 func_00344F80(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991958[i & 0xFF], a, b);
}

s32 func_00344FB0(void) {
    return 0;
}

/* (self->*D_01991948[i])(a, b) */
s32 func_00344FC0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991948[i & 0xFF], a, b);
}

s32 func_00344FF0(void) {
    return 0x1;
}
