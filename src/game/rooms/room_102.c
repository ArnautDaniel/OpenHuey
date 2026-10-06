/* Room 0x102: its event handler class (vtable Room102_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room102_vtable[];

extern u8 D_004174B0[];
extern u8 D_004174C0[];
extern u8 D_004175D0[];
extern u8 D_00417690[];
extern void *D_00417800[];
extern u8 D_00417830[];
extern void *D_00417820[];

extern PTMF D_01990E98[];

/* 0x002E5B40 */
void *Room102_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room102_vtable, RoomBase_vtable); }

/* 0x002E5BA0 */
void *Room102_EnterScript(void) { return D_004174B0; }

/* 0x002E5BB0 */
void *Room102_CharEnterScript(void) { return D_004174C0; }

/* 0x002E5BC0 */
void *Room102_Phase1Script(void) { return D_004175D0; }

/* 0x002E5BD0 */
void *Room102_Phase2Script(void) { return D_00417690; }

/* 0x002E5BE0 */
void *Room102_ActionScript(void *self, s32 i) { return D_00417800[i]; }

/* 0x002E5C00 */
void *Room102_Table38(void) { return D_00417830; }

/* 0x002E5C10 */
void *Room102_ObjectName(void *self, s32 i) { return D_00417820[i]; }

/* (self->*D_01990E98[i])(a, b) */
/* 0x002E5C30 */
s32 Room102_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E98[i & 0xFF], a, b);
}

/* 0x002E5C60 */
s32 Room102_Cmd00(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
