/* Room 0x2F: its event handler class (vtable Room2F_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "item.h"
#include "pursuer.h"

extern void *RoomBase_vtable[];
extern void *Room2F_vtable[];

extern u8 D_00412950[], D_004129A0[], D_00412A30[], D_00412B60[], D_00412B90[];
extern void *D_00412E60[];
extern u8 D_00412E90[];
extern void *D_0047ABFC[];

extern PTMF D_01990E38[];

/* 0x002CC950 */
void *Room2F_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room2F_vtable, RoomBase_vtable); }

/* 0x002CC9B0 */
void *Room2F_EnterScript(void) { return D_00412950; }

/* 0x002CC9C0 */
void *Room2F_CharEnterScript(void) { return D_004129A0; }

/* 0x002CC9D0 */
void *Room2F_Phase1Script(void) { return D_00412A30; }

/* 0x002CC9E0 */
void *Room2F_Phase2Script(void) { return D_00412B60; }

/* 0x002CC9F0 */
void *Room2F_Phase5Script(void) { return D_00412B90; }

/* 0x002CCA00 */
void *Room2F_ActionScript(void *self, s32 i) { return D_00412E60[i]; }

/* 0x002CCA20 */
void *Room2F_Table38(void) { return D_00412E90; }

/* 0x002CCA30 */
void *Room2F_ObjectName(void *self, s32 i) { return D_0047ABFC[i]; }

/* (self->*D_01990E38[i])(a, b) */
/* 0x002CCA50 */
s32 Room2F_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E38[i & 0xFF], a, b);
}

/* room 0x2F (Room2F_Cond00_ptmf): the pursuer's Pursuer_GrabHewieBehind */
/* 0x002CCA80 */
s32 Room2F_Cond00(void) {
    return Pursuer_GrabHewieBehind((Pursuer *)gCharPursuer);
}
