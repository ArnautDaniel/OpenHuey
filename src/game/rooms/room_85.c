/* Room 0x85: its event handler class (vtable D_00477280, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477280[];
extern u8 D_0047AE30[];

extern u32 D_00432700[];
extern u32 D_00432730[];
extern u32 D_004327D0[];
extern u32 D_004328A0[];
extern u32 D_00432AE0[];
extern u32 D_0047AE28[];

extern PTMF D_019916E8[];

void *func_0033F340(void *o, s32 flags) { return room_dtor(o, flags, D_00477280, D_0046DB80); }

void *func_0033F3A0(void) {
    return D_00432700;
}

void *func_0033F3B0(void) {
    return D_00432730;
}

void *func_0033F3C0(void) {
    return D_004327D0;
}

void *func_0033F3D0(void) {
    return D_004328A0;
}

u32 func_0033F3E0(void *self, s32 i) {
    return D_00432AE0[i];
}

void *func_0033F400(void *o) { return D_0047AE30; }   /* D_00477280 +0x38 */

u32 func_0033F410(void *self, s32 i) {
    return D_0047AE28[i];
}

/* (self->*D_019916E8[i])(a, b) */
s32 func_0033F430(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916E8[i & 0xFF], a, b);
}

s32 func_0033F460(void) { return slam_shake(); }
