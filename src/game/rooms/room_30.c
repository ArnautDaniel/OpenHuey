/* Room 0x30: its event handler class (vtable D_0046EE00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "item.h"

extern void *D_0046DB80[];
extern void *D_0046EE00[];

extern u8 D_00412EA0[], D_00412F00[], D_00412FB0[], D_00413160[], D_004131A0[];
extern void *D_00413490[];
extern u8 D_004134C0[];
extern void *D_0047AC00[];

extern PTMF D_01990E48[];

void *func_002CCA90(void *o, s32 flags) { return room_dtor(o, flags, D_0046EE00, D_0046DB80); }

void *func_002CCAF0(void) { return D_00412EA0; }

void *func_002CCB00(void) { return D_00412F00; }

void *func_002CCB10(void) { return D_00412FB0; }

void *func_002CCB20(void) { return D_00413160; }

void *func_002CCB30(void) { return D_004131A0; }

void *func_002CCB40(void *self, s32 i) { return D_00413490[i]; }

void *func_002CCB60(void) { return D_004134C0; }

void *func_002CCB70(void *self, s32 i) { return D_0047AC00[i]; }

/* (self->*D_01990E48[i])(a, b) */
s32 func_002CCB90(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E48[i & 0xFF], a, b);
}

/* room 0x30 (D_004134B0): the pursuer's func_0029A710 */
s32 func_002CCBC0(void) {
    return func_0029A710((Pursuer *)gCharPursuer);
}
