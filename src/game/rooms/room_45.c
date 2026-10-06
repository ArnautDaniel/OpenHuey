/* Room 0x45: its event handler class (vtable D_0046E600, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E600[];
extern u8 D_0047AB50[];
extern u8 D_0047AB58[], D_0047AB70[];

extern u8 D_00407E70[];
extern u8 D_00407E90[];
extern u8 D_00407ED0[];
extern u32 D_0047AB60[];
extern u32 D_0047AB68[];

extern PTMF D_01990C68[];

void *func_002B2820(void *o, s32 flags) { return room_dtor(o, flags, D_0046E600, D_0046DB80); }

void *func_002B2880(void) {
    return D_00407E70;
}

void *func_002B2890(void) {
    return D_00407E90;
}

void *func_002B28A0(void *o) { return D_0047AB50; }   /* D_0046E600 +0x10 */

void *func_002B28B0(void) {
    return D_00407ED0;
}

void *func_002B28C0(void *o) { return D_0047AB58; }   /* D_0046E600 +0x20 */

u32 func_002B28D0(void *self, s32 i) {
    return D_0047AB60[i];
}

void *func_002B28F0(void *o) { return D_0047AB70; }   /* D_0046E600 +0x38 */

u32 func_002B2900(void *self, s32 i) {
    return D_0047AB68[i];
}

/* (self->*D_01990C68[i])(a, b) */
s32 func_002B2920(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C68[i & 0xFF], a, b);
}

s32 func_002B2950(void) {
    return 1;
}
