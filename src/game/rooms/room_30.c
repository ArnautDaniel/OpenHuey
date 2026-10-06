/* Room 0x30: its event handler class (vtable Room30_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "item.h"
#include "pursuer.h"

extern void *RoomBase_vtable[];
extern void *Room30_vtable[];

extern u8 D_00412EA0[], D_00412F00[], D_00412FB0[], D_00413160[], D_004131A0[];
extern void *D_00413490[];
extern u8 D_004134C0[];
extern void *D_0047AC00[];

extern PTMF D_01990E48[];

/* 0x002CCA90 */
void *Room30_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room30_vtable, RoomBase_vtable); }

/* 0x002CCAF0 */
void *Room30_EnterScript(void) { return D_00412EA0; }

/* 0x002CCB00 */
void *Room30_CharEnterScript(void) { return D_00412F00; }

/* 0x002CCB10 */
void *Room30_Phase1Script(void) { return D_00412FB0; }

/* 0x002CCB20 */
void *Room30_Phase2Script(void) { return D_00413160; }

/* 0x002CCB30 */
void *Room30_Phase5Script(void) { return D_004131A0; }

/* 0x002CCB40 */
void *Room30_ActionScript(void *self, s32 i) { return D_00413490[i]; }

/* 0x002CCB60 */
void *Room30_Table38(void) { return D_004134C0; }

/* 0x002CCB70 */
void *Room30_ObjectName(void *self, s32 i) { return D_0047AC00[i]; }

/* (self->*D_01990E48[i])(a, b) */
/* 0x002CCB90 */
s32 Room30_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E48[i & 0xFF], a, b);
}

/* room 0x30 (D_004134B0): the pursuer's Pursuer_GrabHewieBehind */
/* 0x002CCBC0 */
s32 Room30_Cond00(void) {
    return Pursuer_GrabHewieBehind((Pursuer *)gCharPursuer);
}
