/* Room 0x85: its event handler class (vtable D_00477280, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477280[];
extern u8 D_0047AE30[];

extern u32 D_00432700[];
extern u32 D_00432730[];
extern u32 D_004327D0[];
extern u32 D_004328A0[];
extern u32 D_00432AE0[];
extern u32 D_0047AE28[];

extern PTMF D_019916E8[];

/* 0x0033F340 */
void *Room85_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00477280, D_0046DB80); }

/* 0x0033F3A0 */
void *Room85_EnterScript(void) {
    return D_00432700;
}

/* 0x0033F3B0 */
void *Room85_CharEnterScript(void) {
    return D_00432730;
}

/* 0x0033F3C0 */
void *Room85_Phase1Script(void) {
    return D_004327D0;
}

/* 0x0033F3D0 */
void *Room85_Phase2Script(void) {
    return D_004328A0;
}

/* 0x0033F3E0 */
u32 Room85_ActionScript(void *self, s32 i) {
    return D_00432AE0[i];
}

/* 0x0033F400 */
void *Room85_Table38(void *o) { return D_0047AE30; }   /* D_00477280 +0x38 */

/* 0x0033F410 */
u32 Room85_ObjectName(void *self, s32 i) {
    return D_0047AE28[i];
}

/* (self->*D_019916E8[i])(a, b) */
/* 0x0033F430 */
s32 Room85_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916E8[i & 0xFF], a, b);
}

/* 0x0033F460 */
s32 Room85_Cmd00(void) { return slam_shake(); }
