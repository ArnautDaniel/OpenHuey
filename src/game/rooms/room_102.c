/* Room 0x102: its event handler class (vtable D_0046FC80, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046FC80[];

extern u8 D_004174B0[];
extern u8 D_004174C0[];
extern u8 D_004175D0[];
extern u8 D_00417690[];
extern void *D_00417800[];
extern u8 D_00417830[];
extern void *D_00417820[];

extern PTMF D_01990E98[];

void *func_002E5B40(void *o, s32 flags) { return room_dtor(o, flags, D_0046FC80, D_0046DB80); }

void *func_002E5BA0(void) { return D_004174B0; }

void *func_002E5BB0(void) { return D_004174C0; }

void *func_002E5BC0(void) { return D_004175D0; }

void *func_002E5BD0(void) { return D_00417690; }

void *func_002E5BE0(void *self, s32 i) { return D_00417800[i]; }

void *func_002E5C00(void) { return D_00417830; }

void *func_002E5C10(void *self, s32 i) { return D_00417820[i]; }

/* (self->*D_01990E98[i])(a, b) */
s32 func_002E5C30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E98[i & 0xFF], a, b);
}

s32 func_002E5C60(VObject *self, void *a1, u8 *cmd) { return swing_three(self, cmd); }
