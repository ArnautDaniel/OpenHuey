/* Room 0x24: its event handler class (vtable D_0046E380, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E380[];
extern u8 D_0047AB10[];

extern u8 D_00401890[];
extern u8 D_00401950[];
extern u8 D_004019D0[];
extern u8 D_00401B50[];
extern u32 D_00402270[];
extern u8 D_00402320[];
extern u32 D_004022F0[];

extern PTMF D_01990B10[];

void *func_002AFC30(void *o, s32 flags) { return room_dtor(o, flags, D_0046E380, D_0046DB80); }

void *func_002AFC90(void) {
    return D_00401890;
}

void *func_002AFCA0(void) {
    return D_00401950;
}

void *func_002AFCB0(void) {
    return D_004019D0;
}

void *func_002AFCC0(void) {
    return D_00401B50;
}

void *func_002AFCD0(void *o) { return D_0047AB10; }   /* D_0046E380 +0x20 */

u32 func_002AFCE0(void *self, s32 i) {
    return D_00402270[i];
}

void *func_002AFD00(void) {
    return D_00402320;
}

u32 func_002AFD10(void *self, s32 i) {
    return D_004022F0[i];
}

/* (self->*D_01990B10[i])(a, b) */
s32 func_002AFD30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B10[i & 0xFF], a, b);
}
