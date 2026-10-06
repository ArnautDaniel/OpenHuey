/* Room 0xE7: its event handler class (vtable D_0047A610, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A610[];
extern u8 D_0047B118[], D_0047B11C[];
extern char D_0047B330[];
extern const char D_00463A38[];

extern u8 D_00449AD0[];
extern u8 D_00449B20[];
extern u8 D_00449BB0[];
extern u8 D_00449CE0[];
extern u8 D_00449D00[];
extern void *D_00449E90[];
extern u8 D_00449EC0[];
extern PTMF D_01991E18[];
extern PTMF D_01991E28[];

void *func_0037A5F0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A610, D_0046DB80); }

void *func_0037A650(void) {
    return D_00449AD0;
}

void *func_0037A660(void) {
    return D_00449B20;
}

void *func_0037A670(void) {
    return D_00449BB0;
}

void *func_0037A680(void *o) { return D_0047B118; }   /* D_0047A610 +0x14 */

void *func_0037A690(void) {
    return D_00449CE0;
}

void *func_0037A6A0(void *self, s32 i) {
    return D_00449E90[i];
}

void *func_0037A6C0(void) {
    return D_00449D00;
}

void *func_0037A6D0(void) {
    return D_00449EC0;
}

u32 func_0037A6E0(void *o, s32 i) { return ((u32 *)D_0047B11C)[i]; }   /* D_0047A610 +0x34 */

/* (self->*D_01991E28[i])(a, b) */
s32 func_0037A700(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E28[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0037A730(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991E18[i])(a, b) */
s32 func_0037A750(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E18[i & 0xFF], a, b);
}

s32 func_0037A780(void) { return clock_draw(D_0047B330, D_00463A38); }
