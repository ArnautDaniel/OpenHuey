/* Room 0xD9: its event handler class (vtable D_0047A2B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A2B0[];
extern u8 D_0047B0A8[];
extern void func_00124F20(void *c, s32 a);
extern char D_0047B2F0[];
extern const char D_00463738[];

extern u8 D_00447E80[];
extern u8 D_00447E90[];
extern u8 D_00447F40[];
extern u8 D_00447F60[];
extern u8 D_00447FC8[];
extern PTMF D_01991C90[];
extern PTMF D_01991CB8[];

void *func_0036FCF0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A2B0, D_0046DB80); }

void *func_0036FD50(void) {
    return D_00447E80;
}

void *func_0036FD60(void) {
    return D_00447E90;
}

void *func_0036FD70(void) {
    return D_00447F40;
}

void *func_0036FD80(void) {
    return D_00447F60;
}

u32 func_0036FD90(void *o, s32 i) { return ((u32 *)D_0047B0A8)[i]; }   /* D_0047A2B0 +0x24 */

void *func_0036FDB0(void) {
    return D_00447FC8;
}

/* (self->*D_01991CB8[i])(a, b) */
s32 func_0036FDC0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991CB8[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036FDF0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991C90[i])(a, b) */
s32 func_0036FE10(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C90[i & 0xFF], a, b);
}

/* (as func_0030F2B0)  room 0x48 (D_00426830): the player's func_00124F20(0) */
s32 func_0036FE40(void) {
    func_00124F20(gCharPlayer, 0);
    return 1;
}

s32 func_0036FE70(void) { return clock_draw(D_0047B2F0, D_00463738); }

s32 func_0036FFC0(void) { return clock_start(); }
