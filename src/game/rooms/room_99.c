/* Room 0x99: its event handler class (vtable D_00479340, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00479340[];
extern u8 D_0047AFA0[], D_0047AFA8[];

extern u8 D_00443560[], D_004435E0[], D_00443680[];
extern void *D_00443708[];
extern u8 D_00443728[];

extern PTMF D_01991A28[];

void *func_00352AF0(void *o, s32 flags) { return room_dtor(o, flags, D_00479340, D_0046DB80); }

void *func_00352B50(void *o) { return D_0047AFA0; }   /* D_00479340 +0xC */

void *func_00352B60(void) { return D_00443560; }

void *func_00352B70(void) { return D_004435E0; }

void *func_00352B80(void *o) { return D_0047AFA8; }   /* D_00479340 +0x14 */

void *func_00352B90(void) { return D_00443680; }

void *func_00352BA0(void *self, s32 i) { return D_00443708[i]; }

void *func_00352BC0(void) { return D_00443728; }

/* (self->*D_01991A28[i])(a, b) */
s32 func_00352BD0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A28[i & 0xFF], a, b);
}

s32 func_00352C00(void) { return slam_shake(); }
