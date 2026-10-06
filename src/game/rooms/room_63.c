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

/* 0x00308990 */
void *Room63_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00471210, D_0046DB80); }

/* 0x003089F0 */
void *Room63_EnterScript(void) {
    return D_00421130;
}

/* 0x00308A00 */
void *Room63_CharEnterScript(void) {
    return D_004211D0;
}

/* 0x00308A10 */
void *Room63_Phase1Script(void) {
    return D_00421250;
}

/* 0x00308A20 */
void *Room63_Phase2Script(void) {
    return D_00421400;
}

/* 0x00308A30 */
void *Room63_Phase5Script(void) {
    return D_00421420;
}

/* 0x00308A40 */
void *Room63_ActionScript(void *self, s32 i) {
    return D_00421BB0[i];
}

/* 0x00308A60 */
void *Room63_Table38(void) {
    return D_00421C50;
}

/* 0x00308A70 */
void *Room63_ObjectName(void *self, s32 i) {
    return D_00421BF0[i];
}

/* (self->*D_01991098[i])(a, b) */
/* 0x00308A90 */
s32 Room63_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991098[i & 0xFF], a, b);
}

/* 0x00308AC0 */
s32 Room63_Cmd00(void *self, u8 *obj) {
    U32(obj, 0x104) = U32(gCharPlayer, 0x34);
    S32(obj, 0x108) = 0x204;
    obj[0xE1] = 0;
    S32(obj, 0xF4) = 6;
    return 1;
}
