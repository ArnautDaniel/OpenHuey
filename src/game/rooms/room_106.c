/* Room 0x106: its event handler class (vtable Room106_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room106_vtable[];

extern u8 D_004182D0[];
extern u8 D_00418430[];
extern u8 D_004184C0[];
extern u8 D_00418730[];
extern void *D_00418AA0[];
extern u8 D_00418AD0[];
extern void *D_0047AC70[];

extern PTMF D_01990ED8[];

/* 0x002E6D00 */
void *Room106_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room106_vtable, RoomBase_vtable); }

/* 0x002E6D60 */
void *Room106_EnterScript(void) { return D_004182D0; }

/* 0x002E6D70 */
void *Room106_CharEnterScript(void) { return D_00418430; }

/* 0x002E6D80 */
void *Room106_Phase1Script(void) { return D_004184C0; }

/* 0x002E6D90 */
void *Room106_Phase2Script(void) { return D_00418730; }

/* 0x002E6DA0 */
void *Room106_ActionScript(void *self, s32 i) { return D_00418AA0[i]; }

/* 0x002E6DC0 */
void *Room106_Table38(void) { return D_00418AD0; }

/* 0x002E6DD0 */
void *Room106_ObjectName(void *self, s32 i) { return D_0047AC70[i]; }

/* (self->*D_01990ED8[i])(a, b) */
/* 0x002E6DF0 */
s32 Room106_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990ED8[i & 0xFF], a, b);
}

/* 0x002E6E20 */
s32 Room106_Cmd00(void) {
    Effect_New(gEffects, 0xC0, effect_C0_init);
    return 1;
}
