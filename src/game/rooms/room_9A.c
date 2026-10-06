/* Room 0x9A: its event handler class (vtable D_00479380, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00479380[];
extern u8 D_0047AFB0[], D_0047AFB8[];

extern u8 D_00443740[], D_004437C0[];
extern void *D_0047AFBC[];
extern u8 D_00443860[];

extern PTMF D_01991A38[];

void *func_00352C90(void *o, s32 flags) { return room_dtor(o, flags, D_00479380, D_0046DB80); }

void *func_00352CF0(void *o) { return D_0047AFB0; }   /* D_00479380 +0xC */

void *func_00352D00(void) { return D_00443740; }

void *func_00352D10(void) { return D_004437C0; }

void *func_00352D20(void *o) { return D_0047AFB8; }   /* D_00479380 +0x14 */

void *func_00352D30(void *self, s32 i) { return D_0047AFBC[i]; }

void *func_00352D50(void) { return D_00443860; }

/* (self->*D_01991A38[i])(a, b) */
s32 func_00352D60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A38[i & 0xFF], a, b);
}

s32 func_00352D90(void) { return slam_shake(); }
