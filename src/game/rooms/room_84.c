/* Room 0x84: its event handler class (vtable D_00477240, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477240[];
extern u8 D_0047AE18[];

extern u32 D_00432420[];
extern u32 D_00432440[];
extern u32 D_004324A0[];
extern u32 D_004326D0[];
extern u32 D_004326E8[];
extern u32 D_0047AE20[];

extern PTMF D_019916D8[];

void *func_0033F190(void *o, s32 flags) { return room_dtor(o, flags, D_00477240, D_0046DB80); }

void *func_0033F1F0(void) {
    return D_00432420;
}

void *func_0033F200(void) {
    return D_00432440;
}

void *func_0033F210(void) {
    return D_004324A0;
}

void *func_0033F220(void *o) { return D_0047AE18; }   /* D_00477240 +0x14 */

u32 func_0033F230(void *self, s32 i) {
    return D_0047AE20[i];
}

void *func_0033F250(void) {
    return D_004326E8;
}

u32 func_0033F260(void *self, s32 i) {
    return D_004326D0[i];
}

/* (self->*D_019916D8[i])(a, b) */
s32 func_0033F280(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916D8[i & 0xFF], a, b);
}

s32 func_0033F2B0(void) { return slam_shake(); }
