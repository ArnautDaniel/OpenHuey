/* Room 0x87: its event handler class (vtable D_004772C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_004772C0[];
extern u8 D_0047AE38[];

extern u32 D_00432B10[];
extern u32 D_00432B80[];
extern u32 D_00432BC0[];
extern u32 D_00432C10[];
extern u32 D_00432C80[];

extern PTMF D_019916F8[];

void *func_0033F4F0(void *o, s32 flags) { return room_dtor(o, flags, D_004772C0, D_0046DB80); }

void *func_0033F550(void) {
    return D_00432B10;
}

void *func_0033F560(void) {
    return D_00432B80;
}

void *func_0033F570(void) {
    return D_00432BC0;
}

void *func_0033F580(void) {
    return D_00432C10;
}

void *func_0033F590(void *o) { return D_0047AE38; }   /* D_004772C0 +0x20 */

u32 func_0033F5A0(void *self, s32 i) {
    return D_00432C80[i];
}

/* (self->*D_019916F8[i])(a, b) */
s32 func_0033F5C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916F8[i & 0xFF], a, b);
}

s32 func_0033F5F0(void) { return slam_shake(); }
