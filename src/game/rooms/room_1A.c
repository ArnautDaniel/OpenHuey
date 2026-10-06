/* Room 0x1A: its event handler class (vtable Room1A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room1A_vtable[];
extern void *TvScreenB_vtable[];

extern u8 D_003FC060[];
extern u8 D_003FC0A0[];
extern u8 D_003FC110[];
extern u8 D_003FC220[];
extern u8 D_003FC290[];
extern void *D_003FC630[];
extern void *D_003FC678[];
extern u8 D_003FC690[];
extern PTMF D_01990960[];

/* 0x002AD420 */
void *Room1A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room1A_vtable, RoomBase_vtable); }

/* 0x002AD480 */
void *Room1A_EnterScript(void) {
    return D_003FC060;
}

/* 0x002AD490 */
void *Room1A_CharEnterScript(void) {
    return D_003FC0A0;
}

/* 0x002AD4A0 */
void *Room1A_Phase1Script(void) {
    return D_003FC110;
}

/* 0x002AD4B0 */
void *Room1A_Phase2Script(void) {
    return D_003FC220;
}

/* 0x002AD4C0 */
void *Room1A_Phase3Script(void) {
    return D_003FC290;
}

/* 0x002AD4D0 */
void *Room1A_ActionScript(void *self, s32 i) {
    return D_003FC630[i];
}

/* 0x002AD4F0 */
void *Room1A_Table38(void) {
    return D_003FC690;
}

/* 0x002AD500 */
void *Room1A_ObjectName(void *self, s32 i) {
    return D_003FC678[i];
}

/* (self->*D_01990960[i])(a, b) */
/* 0x002AD520 */
s32 Room1A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990960[i & 0xFF], a, b);
}

/* 0x002AD550 */
s32 Room1A_Cmd01(void) {   /* room effect 0 (TvScreenB_vtable) */
    room_effect_slot_new(gRoomEffects, 0, TvScreenB_vtable);
    return 1;
}

/* room 0x1A (D_003FC660): the fan turns */
/* 0x002AD600 */
s32 Room1A_Cmd00(void) {
    fan_turn(D_003FC680);
    return 1;
}
