/* Room 0x84: its event handler class (vtable D_00477240, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477240[];
extern u8 D_0047AE18[];

extern u32 D_00432420[];
extern u32 D_00432440[];
extern u32 D_004324A0[];
extern u32 D_004326D0[];
extern u32 D_004326E8[];
extern u32 D_0047AE20[];

extern PTMF D_019916D8[];

/* 0x0033F190 */
void *Room84_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00477240, D_0046DB80); }

/* 0x0033F1F0 */
void *Room84_EnterScript(void) {
    return D_00432420;
}

/* 0x0033F200 */
void *Room84_CharEnterScript(void) {
    return D_00432440;
}

/* 0x0033F210 */
void *Room84_Phase1Script(void) {
    return D_004324A0;
}

/* 0x0033F220 */
void *Room84_Phase2Script(void *o) { return D_0047AE18; }   /* D_00477240 +0x14 */

/* 0x0033F230 */
u32 Room84_ActionScript(void *self, s32 i) {
    return D_0047AE20[i];
}

/* 0x0033F250 */
void *Room84_Table38(void) {
    return D_004326E8;
}

/* 0x0033F260 */
u32 Room84_ObjectName(void *self, s32 i) {
    return D_004326D0[i];
}

/* (self->*D_019916D8[i])(a, b) */
/* 0x0033F280 */
s32 Room84_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916D8[i & 0xFF], a, b);
}

/* 0x0033F2B0 */
s32 Room84_Cmd00(void) { return slam_shake(); }
