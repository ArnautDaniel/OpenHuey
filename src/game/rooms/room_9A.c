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

/* 0x00352C90 */
void *Room9A_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00479380, D_0046DB80); }

/* 0x00352CF0 */
void *Room9A_EnterScript(void *o) { return D_0047AFB0; }   /* D_00479380 +0xC */

/* 0x00352D00 */
void *Room9A_CharEnterScript(void) { return D_00443740; }

/* 0x00352D10 */
void *Room9A_Phase1Script(void) { return D_004437C0; }

/* 0x00352D20 */
void *Room9A_Phase2Script(void *o) { return D_0047AFB8; }   /* D_00479380 +0x14 */

/* 0x00352D30 */
void *Room9A_ActionScript(void *self, s32 i) { return D_0047AFBC[i]; }

/* 0x00352D50 */
void *Room9A_Table38(void) { return D_00443860; }

/* (self->*D_01991A38[i])(a, b) */
/* 0x00352D60 */
s32 Room9A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A38[i & 0xFF], a, b);
}

/* 0x00352D90 */
s32 Room9A_Cmd00(void) { return slam_shake(); }
