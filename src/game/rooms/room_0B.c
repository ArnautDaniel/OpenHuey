/* Room 0x0B: its event handler class (vtable D_0046DDC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DDC0[];
extern u8 D_0047A9C8[];

extern u8 D_003F43C0[];
extern u8 D_003F43E0[];
extern u8 D_003F4470[];
extern void *D_003F4620[];
extern PTMF D_01990848[];

void *func_002AB030(void *o, s32 flags) { return room_dtor(o, flags, D_0046DDC0, D_0046DB80); }

void *func_002AB090(void) {
    return D_003F43C0;
}

void *func_002AB0A0(void) {
    return D_003F43E0;
}

void *func_002AB0B0(void) {
    return D_003F4470;
}

void *func_002AB0C0(void *self, s32 i) {
    return D_003F4620[i];
}

/* entry `i` of a table of the room's */
u32 func_002AB0E0(void *o, s32 i) { return ((u32 *)D_0047A9C8)[i]; }   /* D_0046DDC0 +0x34 */

/* (self->*D_01990848[i])(a, b) */
s32 func_002AB100(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990848[i & 0xFF], a, b);
}

/* room 0x0B (D_003F4640): the player is 20 .. 120 from (x, z) = s16 bytes 3..4, 5..6 */
s32 func_002AB130(void *self, void *a1, u8 *cmd) {
    f32 dx = (f32)(s16)(cmd[3] << 8 | cmd[4]) - gCharPlayer->a.pos[0];
    f32 dz = (f32)(s16)(cmd[5] << 8 | cmd[6]) - gCharPlayer->a.pos[2];
    f32 d = ee_sqrtf(dz * dz + dx * dx);

    return !(d < 20.0f) && d <= 120.0f;
}
