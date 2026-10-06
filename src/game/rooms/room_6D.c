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

/* 0x00344B90 */
void *Room6D_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00477690, D_0046DB80); }

/* 0x00344BF0 */
void *Room6D_EnterScript(void) {
    return D_00439B10;
}

/* 0x00344C00 */
void *Room6D_CharEnterScript(void) {
    return D_00439C00;
}

/* 0x00344C10 */
void *Room6D_Phase1Script(void) {
    return D_00439D00;
}

/* 0x00344C20 */
void *Room6D_Phase2Script(void) {
    return D_00439F10;
}

/* 0x00344C30 */
void *Room6D_Phase3Script(void) {
    return D_00439FB0;
}

/* 0x00344C40 */
u32 Room6D_ActionScript(void *self, s32 i) {
    return D_0047AEB0[i];
}

/* 0x00344C60 */
void *Room6D_Table38(void) {
    return D_0043A100;
}

/* 0x00344C70 */
u32 Room6D_ObjectName(void *self, s32 i) {
    return D_0043A0E0[i];
}

/* (self->*D_01991930[i])(a, b) */
/* 0x00344C90 */
s32 Room6D_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991930[i & 0xFF], a, b);
}

/* 0x00344CC0 */
s32 Room6D_Cond01(void) {
    u8 *p = (u8 *)gCharPursuer;

    if (p == NULL || p[0x28] == 0) {
        return 0;
    }
    return *(s32 *)(p + 0xE8) == 0;
}

/* 0x00344D10 */
s32 Room6D_Cond00(void) {
    u8 *p = (u8 *)gCharPursuer;

    if (p == NULL || p[0x28] == 0 || *(s32 *)(p + 0xE8) == 0) {
        return 0;
    }
    return ((u8 *)gProgress)[0x1130] != 0xFE;
}
