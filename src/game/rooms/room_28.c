/* Room 0x28: its event handler class (vtable D_0046E440, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E440[];

extern u8 D_00403980[];
extern u8 D_004039B0[];
extern u8 D_00403A10[];
extern u8 D_00403C10[];
extern u8 D_00403C30[];
extern u32 D_00403F40[];
extern u8 D_00403F80[];
extern u32 D_0047AB34[];

extern PTMF D_01990B98[];

void *func_002B0CF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E440, D_0046DB80); }

void *func_002B0D50(void) {
    return D_00403980;
}

void *func_002B0D60(void) {
    return D_004039B0;
}

void *func_002B0D70(void) {
    return D_00403A10;
}

void *func_002B0D80(void) {
    return D_00403C10;
}

void *func_002B0D90(void) {
    return D_00403C30;
}

u32 func_002B0DA0(void *self, s32 i) {
    return D_00403F40[i];
}

void *func_002B0DC0(void) {
    return D_00403F80;
}

u32 func_002B0DD0(void *self, s32 i) {
    return D_0047AB34[i];
}

/* (self->*D_01990B98[i])(a, b) */
s32 func_002B0DF0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B98[i & 0xFF], a, b);
}

/* the pursuer's func_0029A710 */
s32 func_002B0E20(void) {
    return func_0029A710((Pursuer *)gCharPursuer);
}
