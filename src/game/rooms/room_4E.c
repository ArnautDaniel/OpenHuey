/* Room 0x4E: its event handler class (vtable D_0046E740, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E740[];
extern const char *D_0040B508;
extern void *D_00478BE0[];
extern u8 D_0040AE90[];
extern u8 D_0040AF40[];
extern u8 D_0040B040[];
extern u8 D_0040B160[];
extern u8 D_0040B200[];
extern u32 D_0040B4B0[];
extern u8 D_0040B510[];
extern u32 D_0040B500[];

extern PTMF D_01990D10[];

static void effect_78BE0_init(void **obj) {
    obj[0] = D_00478BE0;
}

void *func_002B3970(void *o, s32 flags) { return room_dtor(o, flags, D_0046E740, D_0046DB80); }

void *func_002B39D0(void) {
    return D_0040AE90;
}

void *func_002B39E0(void) {
    return D_0040AF40;
}

void *func_002B39F0(void) {
    return D_0040B040;
}

void *func_002B3A00(void) {
    return D_0040B160;
}

void *func_002B3A10(void) {
    return D_0040B200;
}

u32 func_002B3A20(void *self, s32 i) {
    return D_0040B4B0[i];
}

void *func_002B3A40(void) {
    return D_0040B510;
}

u32 func_002B3A50(void *self, s32 i) {
    return D_0040B500[i];
}

/* (self->*D_01990D10[i])(a, b) */
s32 func_002B3A70(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D10[i & 0xFF], a, b);
}

/* room 0x4E (D_0040B4F8): the 0x10-byte effect D_00478BE0 made */
s32 func_002B3AA0(void) {
    Effect_New(gEffects, 0x10, effect_78BE0_init);
    return 1;
}

s32 func_002B3B70(void *self, void *a1, u8 *cmd) {
    return var0_anim(cmd, D_0040B508);
}

/* room 0x4E (D_0040B4D8): a lit quad at x -44 .. -36, z 60, from 54 to 71 */
s32 func_002B3D80(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0xC2300000, 0x428E0000, 0x42700000, 0x3F800000, 0xC2100000, 0x428E0000, 0x42700000, 0x3F800000,
        0xC2300000, 0x42580000, 0x42700000, 0x3F800000, 0xC2100000, 0x42580000, 0x42700000, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x80);
}
