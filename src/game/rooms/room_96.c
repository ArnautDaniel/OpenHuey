/* Room 0x96: its event handler class (vtable D_00479990, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00479990[];
extern u8 D_0047AFF0[];

extern u8 D_004446E0[];
extern u8 D_00444710[];
extern u8 D_00444750[];
extern u8 D_004447C0[];
extern void *D_00444830[];
extern PTMF D_01991A88[];

/* 0x0035D650 */
void *Room96_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00479990, D_0046DB80); }

/* 0x0035D6B0 */
void *Room96_EnterScript(void) {
    return D_004446E0;
}

/* 0x0035D6C0 */
void *Room96_CharEnterScript(void) {
    return D_00444710;
}

/* 0x0035D6D0 */
void *Room96_Phase1Script(void) {
    return D_00444750;
}

/* 0x0035D6E0 */
void *Room96_Phase2Script(void) {
    return D_004447C0;
}

/* 0x0035D6F0 */
void *Room96_Phase5Script(void *o) { return D_0047AFF0; }   /* D_00479990 +0x20 */

/* 0x0035D700 */
void *Room96_ActionScript(void *self, s32 i) {
    return D_00444830[i];
}

/* (self->*D_01991A88[i])(a, b) */
/* 0x0035D720 */
s32 Room96_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A88[i & 0xFF], a, b);
}

/* 0x0035D750 */
s32 Room96_Cmd00(void) { return slam_shake(); }
