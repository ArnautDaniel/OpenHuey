/* Room 0x6C: its event handler class (vtable D_00477650, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477650[];
extern u8 D_0047AE98[];

extern u32 D_00439160[];
extern u32 D_00439220[];
extern u32 D_00439320[];
extern u32 D_004396D0[];
extern u32 D_00439AA0[];
extern u32 D_00439AF0[];
extern u32 D_0047AEA8[];

extern PTMF D_01991910[];

void *func_00344970(void *o, s32 flags) { return room_dtor(o, flags, D_00477650, D_0046DB80); }

void *func_003449D0(void) {
    return D_00439160;
}

void *func_003449E0(void) {
    return D_00439220;
}

void *func_003449F0(void) {
    return D_00439320;
}

void *func_00344A00(void) {
    return D_004396D0;
}

void *func_00344A10(void *o) { return D_0047AE98; }   /* D_00477650 +0x20 */

u32 func_00344A20(void *self, s32 i) {
    return D_00439AA0[i];
}

void *func_00344A40(void) {
    return D_00439AF0;
}

u32 func_00344A50(void *self, s32 i) {
    return D_0047AEA8[i];
}

/* (self->*D_01991910[i])(a, b) */
s32 func_00344A70(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991910[i & 0xFF], a, b);
}

/* 1 if the object at gCharPursuer exists, is active (+0x28) and is in mode 4 or 5 (+0xE8). */
s32 func_00344AA0(void) {
    u8 *p = (u8 *)gCharPursuer;
    s32 mode;

    if (p == NULL || p[0x28] == 0) {
        return 0;
    }
    mode = *(s32 *)(p + 0xE8);
    return mode == 4 || mode == 5;
}

s32 func_00344B10(void) {
    u8 *p = (u8 *)gCharPursuer;
    s32 mode;

    if (p == NULL || p[0x28] == 0) {
        return 0;
    }
    mode = *(s32 *)(p + 0xE8);
    if (mode == 4 || mode == 5) {
        return 0;
    }
    return ((u8 *)gProgress)[0x1130] != 0xFE;
}
