/* Room 0x87: its event handler class (vtable D_004772C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_004772C0[];
extern u8 D_0047AE38[];

extern u32 D_00432B10[];
extern u32 D_00432B80[];
extern u32 D_00432BC0[];
extern u32 D_00432C10[];
extern u32 D_00432C80[];

extern PTMF D_019916F8[];

/* 0x0033F4F0 */
void *Room87_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_004772C0, D_0046DB80); }

/* 0x0033F550 */
void *Room87_EnterScript(void) {
    return D_00432B10;
}

/* 0x0033F560 */
void *Room87_CharEnterScript(void) {
    return D_00432B80;
}

/* 0x0033F570 */
void *Room87_Phase1Script(void) {
    return D_00432BC0;
}

/* 0x0033F580 */
void *Room87_Phase2Script(void) {
    return D_00432C10;
}

/* 0x0033F590 */
void *Room87_Phase5Script(void *o) { return D_0047AE38; }   /* D_004772C0 +0x20 */

/* 0x0033F5A0 */
u32 Room87_ActionScript(void *self, s32 i) {
    return D_00432C80[i];
}

/* (self->*D_019916F8[i])(a, b) */
/* 0x0033F5C0 */
s32 Room87_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916F8[i & 0xFF], a, b);
}

/* 0x0033F5F0 */
s32 Room87_Cmd00(void) { return slam_shake(); }
