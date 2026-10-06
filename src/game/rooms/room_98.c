/* Room 0x98: its event handler class (vtable D_00478C00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00478C00[];
extern u8 D_0047AF88[], D_0047AF90[];

extern u32 D_00442F60[];
extern u32 D_00442FE0[];
extern u32 D_00443070[];
extern u32 D_0047AF94[];

extern PTMF D_01991A18[];

/* 0x00350E60 */
void *Room98_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00478C00, D_0046DB80); }

/* 0x00350EC0 */
void *Room98_EnterScript(void *o) { return D_0047AF88; }   /* D_00478C00 +0xC */

/* 0x00350ED0 */
void *Room98_CharEnterScript(void) {
    return D_00442F60;
}

/* 0x00350EE0 */
void *Room98_Phase1Script(void) {
    return D_00442FE0;
}

/* 0x00350EF0 */
void *Room98_Phase2Script(void *o) { return D_0047AF90; }   /* D_00478C00 +0x14 */

/* 0x00350F00 */
u32 Room98_ActionScript(void *self, s32 i) {
    return D_0047AF94[i];
}

/* 0x00350F20 */
void *Room98_Table38(void) {
    return D_00443070;
}

/* (self->*D_01991A18[i])(a, b) */
/* 0x00350F30 */
s32 Room98_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A18[i & 0xFF], a, b);
}

/* 0x00350F60 */
s32 Room98_SlamShake(void) { return slam_shake(); }
