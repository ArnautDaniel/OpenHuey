/* Room 0x63: its event handler class (vtable D_00471210, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00471210[];

extern u8 D_00421130[];
extern u8 D_004211D0[];
extern u8 D_00421250[];
extern u8 D_00421400[];
extern u8 D_00421420[];
extern void *D_00421BB0[];
extern void *D_00421BF0[];
extern u8 D_00421C50[];

/* Field access by byte offset into objects whose layout is not yet known. */
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

extern PTMF D_01991098[];

void *func_00308990(void *o, s32 flags) { return room_dtor(o, flags, D_00471210, D_0046DB80); }

void *func_003089F0(void) {
    return D_00421130;
}

void *func_00308A00(void) {
    return D_004211D0;
}

void *func_00308A10(void) {
    return D_00421250;
}

void *func_00308A20(void) {
    return D_00421400;
}

void *func_00308A30(void) {
    return D_00421420;
}

void *func_00308A40(void *self, s32 i) {
    return D_00421BB0[i];
}

void *func_00308A60(void) {
    return D_00421C50;
}

void *func_00308A70(void *self, s32 i) {
    return D_00421BF0[i];
}

/* (self->*D_01991098[i])(a, b) */
s32 func_00308A90(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991098[i & 0xFF], a, b);
}

s32 func_00308AC0(void *self, u8 *obj) {
    U32(obj, 0x104) = U32(gCharPlayer, 0x34);
    S32(obj, 0x108) = 0x204;
    obj[0xE1] = 0;
    S32(obj, 0xF4) = 6;
    return 1;
}
