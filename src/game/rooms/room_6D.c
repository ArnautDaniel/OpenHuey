/* Room 0x6D: its event handler class (vtable D_00477690, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477690[];

extern u32 D_00439B10[];
extern u32 D_00439C00[];
extern u32 D_00439D00[];
extern u32 D_00439F10[];
extern u32 D_00439FB0[];
extern u32 D_0043A0E0[];
extern u32 D_0043A100[];
extern u32 D_0047AEB0[];

extern PTMF D_01991930[];

void *func_00344B90(void *o, s32 flags) { return room_dtor(o, flags, D_00477690, D_0046DB80); }

void *func_00344BF0(void) {
    return D_00439B10;
}

void *func_00344C00(void) {
    return D_00439C00;
}

void *func_00344C10(void) {
    return D_00439D00;
}

void *func_00344C20(void) {
    return D_00439F10;
}

void *func_00344C30(void) {
    return D_00439FB0;
}

u32 func_00344C40(void *self, s32 i) {
    return D_0047AEB0[i];
}

void *func_00344C60(void) {
    return D_0043A100;
}

u32 func_00344C70(void *self, s32 i) {
    return D_0043A0E0[i];
}

/* (self->*D_01991930[i])(a, b) */
s32 func_00344C90(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991930[i & 0xFF], a, b);
}

s32 func_00344CC0(void) {
    u8 *p = (u8 *)gCharPursuer;

    if (p == NULL || p[0x28] == 0) {
        return 0;
    }
    return *(s32 *)(p + 0xE8) == 0;
}

s32 func_00344D10(void) {
    u8 *p = (u8 *)gCharPursuer;

    if (p == NULL || p[0x28] == 0 || *(s32 *)(p + 0xE8) == 0) {
        return 0;
    }
    return ((u8 *)gProgress)[0x1130] != 0xFE;
}
