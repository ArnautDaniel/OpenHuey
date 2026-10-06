/* Room 0xD0: its event handler class (vtable D_0047A070, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A070[];
extern u8 D_0047B030[], D_0047B040[];
extern char D_0047B290[];
extern const char D_004636D8[];

extern u8 D_004469C0[];
extern u8 D_00446A00[];
extern u8 D_00446A80[];
extern u8 D_00446B00[];
extern u8 D_00446BE0[];
extern PTMF D_01991AC0[];
extern PTMF D_01991AE8[];

void *func_0036DA30(void *o, s32 flags) { return room_dtor(o, flags, D_0047A070, D_0046DB80); }

void *func_0036DA90(void) {
    return D_004469C0;
}

void *func_0036DAA0(void) {
    return D_00446A00;
}

void *func_0036DAB0(void) {
    return D_00446A80;
}

void *func_0036DAC0(void *o) { return D_0047B030; }   /* D_0047A070 +0x14 */

void *func_0036DAD0(void) {
    return D_00446B00;
}

u32 func_0036DAE0(void *o, s32 i) { return ((u32 *)D_0047B040)[i]; }   /* D_0047A070 +0x24 */

void *func_0036DB00(void) {
    return D_00446BE0;
}

/* (self->*D_01991AE8[i])(a, b) */
s32 func_0036DB10(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991AE8[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036DB40(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991AC0[i])(a, b) */
s32 func_0036DB60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991AC0[i & 0xFF], a, b);
}

s32 func_0036DB90(void) { return clock_draw_saved(D_0047B290, D_004636D8); }

s32 func_0036DE60(void) { return clock_save(); }

s32 func_0036DEE0(void) { return clock_stop(); }
