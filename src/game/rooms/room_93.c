/* Room 0x93: its event handler class (vtable Room93_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room93_vtable[];
extern u8 D_0047AFD8[];

extern u8 D_00444170[];
extern u8 D_004441C0[];
extern u8 D_00444200[];
extern u8 D_00444270[];
extern void *D_00444360[];
extern PTMF D_01991A58[];

/* 0x0035D1A0 */
void *Room93_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room93_vtable, RoomBase_vtable); }

/* 0x0035D200 */
void *Room93_EnterScript(void) {
    return D_00444170;
}

/* 0x0035D210 */
void *Room93_CharEnterScript(void) {
    return D_004441C0;
}

/* 0x0035D220 */
void *Room93_Phase1Script(void) {
    return D_00444200;
}

/* 0x0035D230 */
void *Room93_Phase2Script(void) {
    return D_00444270;
}

/* 0x0035D240 */
void *Room93_Phase5Script(void *o) { return D_0047AFD8; }   /* Room93_vtable +0x20 */

/* 0x0035D250 */
void *Room93_ActionScript(void *self, s32 i) {
    return D_00444360[i];
}

/* (self->*D_01991A58[i])(a, b) */
/* 0x0035D270 */
s32 Room93_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A58[i & 0xFF], a, b);
}

/* 0x0035D2A0 */
s32 Room93_SlamShake(void) { return slam_shake(); }
