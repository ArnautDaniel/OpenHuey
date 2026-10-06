/* Room 0x4B: its event handler class (vtable D_0046E6C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E6C0[];
extern const char *D_00409940;

extern u8 D_00408B90[];
extern u8 D_00408CD0[];
extern u8 D_00408E50[];
extern u8 D_004090C0[];
extern u8 D_004092E0[];
extern u8 D_004092C0[];
extern u32 D_004098D0[];

extern u8 D_00409950[];
extern u32 D_00409938[];

extern PTMF D_01990CB0[];
extern PTMF D_01990CC8[];

void *func_002B2FF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E6C0, D_0046DB80); }

void *func_002B3050(void) {
    return D_00408B90;
}

void *func_002B3060(void) {
    return D_00408CD0;
}

void *func_002B3070(void) {
    return D_00408E50;
}

void *func_002B3080(void) {
    return D_004090C0;
}

void *func_002B3090(void) {
    return D_004092E0;
}

void *func_002B30A0(void) {
    return D_004092C0;
}

u32 func_002B30B0(void *self, s32 i) {
    return D_004098D0[i];
}

void *func_002B30D0(void) {
    return D_00409950;
}

u32 func_002B30E0(void *self, s32 i) {
    return D_00409938[i];
}

/* (self->*D_01990CC8[i])(a, b) */
s32 func_002B3100(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990CC8[i & 0xFF], a, b);
}

/* 1 unless the object at +0x18 exists and its byte +0x28 is 1. */
s32 func_002B3130(void) {
    u8 *p = *(u8 **)(gCreatures + 0x18);

    return !(p != NULL && p[0x28] == 1);
}

/* (self->*D_01990CB0[i])(a, b) */
s32 func_002B3170(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990CB0[i & 0xFF], a, b);
}

s32 func_002B31A0(void *self, void *a1, u8 *cmd) {
    return var0_anim(cmd, D_00409940);
}

/* room 0x4B (D_00409910): a lit quad at x -63.65 .. -55.65, z 104.5, from 4 to 21 */
s32 func_002B33B0(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0xC27E999A, 0x41A80000, 0x42D10000, 0x3F800000, 0xC25E999A, 0x41A80000, 0x42D10000, 0x3F800000,
        0xC27E999A, 0x40800000, 0x42D10000, 0x3F800000, 0xC25E999A, 0x40800000, 0x42D10000, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x80);
}
