/* Room 0x09: its event handler class (vtable D_0046DD40, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DD40[];

extern u8 D_003F3240[];
extern u8 D_003F32D0[];
extern u8 D_003F3330[];
extern u8 D_003F3400[];
extern u8 D_003F34A0[];
extern u8 D_003F34D8[];
extern void *D_003F3C00[];
extern void *D_003F3C60[];
extern u8 D_003F3C80[];
extern PTMF D_01990818[];

void *func_002AA920(void *o, s32 flags) { return room_dtor(o, flags, D_0046DD40, D_0046DB80); }

void *func_002AA980(void) {
    return D_003F3240;
}

void *func_002AA990(void) {
    return D_003F32D0;
}

void *func_002AA9A0(void) {
    return D_003F3330;
}

void *func_002AA9B0(void) {
    return D_003F3400;
}

void *func_002AA9C0(void) {
    return D_003F34A0;
}

void *func_002AA9D0(void) {
    return D_003F34D8;
}

void *func_002AA9E0(void *self, s32 i) {
    return D_003F3C00[i];
}

void *func_002AAA00(void) {
    return D_003F3C80;
}

void *func_002AAA10(void *self, s32 i) {
    return D_003F3C60[i];
}

/* (self->*D_01990818[i])(a, b) */
s32 func_002AAA30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990818[i & 0xFF], a, b);
}

/* room 0x09 (D_003F3C50): the pursuer (about, not in state 2, in mode 2, 6 or 7) is in another
 * room than 9 (the player about too) */
s32 func_002AAA60(void) {
    Character *s = gCharPursuer, *p = gCharPlayer;
    u8 k;

    if (s == NULL || s->a.active == 0 || p == NULL || p->a.active == 0 || AT(s, 0xC4, s32) == 2) {
        return 0;
    }
    k = AT(s, 0x153C, u8);
    if (k != 2 && k != 6 && k != 7) {
        return 0;
    }
    return AT(s, 0x30, s32) != 9;
}
