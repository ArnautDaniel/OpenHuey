/* Room 0xC1: its event handler class (vtable D_004785B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "stalker_progress.h"

extern void *D_0046DB80[];
extern void *D_004785B0[];

extern u32 D_0043EC80[];
extern u32 D_0043ECC0[];
extern u32 D_0043ED80[];
extern u32 D_0043EF00[];
extern u32 D_0043EF40[];
extern u32 D_0043F0B0[];
extern u32 D_0047AEF8[];
extern u32 D_0047AF00[];

extern PTMF D_01991978[];

/* 0x0034A360 */
void *RoomC1_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_004785B0, D_0046DB80); }

/* 0x0034A3C0 */
void *RoomC1_EnterScript(void) {
    return D_0043EC80;
}

/* 0x0034A3D0 */
void *RoomC1_CharEnterScript(void) {
    return D_0043ECC0;
}

/* 0x0034A3E0 */
void *RoomC1_Phase1Script(void) {
    return D_0043ED80;
}

/* 0x0034A3F0 */
void *RoomC1_Phase2Script(void) {
    return D_0043EF00;
}

/* 0x0034A400 */
void *RoomC1_Phase3Script(void) {
    return D_0043EF40;
}

/* 0x0034A410 */
u32 RoomC1_ActionScript(void *self, s32 i) {
    return D_0047AEF8[i];
}

/* 0x0034A430 */
void *RoomC1_Table38(void) {
    return D_0043F0B0;
}

/* 0x0034A440 */
u32 RoomC1_ObjectName(void *self, s32 i) {
    return D_0047AF00[i];
}

/* (self->*D_01991978[i])(a, b) */
/* 0x0034A460 */
s32 RoomC1_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991978[i & 0xFF], a, b);
}

/* 0x0034A490 */
s32 RoomC1_Cond00(void) {
    return Countdown_Seconds((u8 *)gProgress + 0x764) == 0;
}
