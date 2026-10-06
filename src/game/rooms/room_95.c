/* Room 0x95: its event handler class (vtable Room95_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room95_vtable[];
extern u8 D_0047AFE8[];

extern u8 D_004444E0[];
extern u8 D_00444530[];
extern u8 D_00444570[];
extern u8 D_004445D0[];
extern void *D_004446B0[];
extern PTMF D_01991A78[];

/* 0x0035D4C0 */
void *Room95_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room95_vtable, RoomBase_vtable); }

/* 0x0035D520 */
void *Room95_EnterScript(void) {
    return D_004444E0;
}

/* 0x0035D530 */
void *Room95_CharEnterScript(void) {
    return D_00444530;
}

/* 0x0035D540 */
void *Room95_Phase1Script(void) {
    return D_00444570;
}

/* 0x0035D550 */
void *Room95_Phase2Script(void) {
    return D_004445D0;
}

/* 0x0035D560 */
void *Room95_Phase5Script(void *o) { return D_0047AFE8; }   /* Room95_vtable +0x20 */

/* 0x0035D570 */
void *Room95_ActionScript(void *self, s32 i) {
    return D_004446B0[i];
}

/* (self->*D_01991A78[i])(a, b) */
/* 0x0035D590 */
s32 Room95_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A78[i & 0xFF], a, b);
}

/* 0x0035D5C0 */
s32 Room95_Cmd00(void) { return slam_shake(); }
