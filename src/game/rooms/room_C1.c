/* Room 0xC1: its event handler class (vtable RoomC1_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "stalker_progress.h"

extern void *RoomBase_vtable[];
extern void *RoomC1_vtable[];

extern u32 RoomC1_EnterScript_data[];
extern u32 RoomC1_CharEnterScript_data[];
extern u32 RoomC1_Phase1Script_data[];
extern u32 RoomC1_Phase2Script_data[];
extern u32 RoomC1_Phase3Script_data[];
extern u32 RoomC1_Table38_data[];
extern u32 RoomC1_ActionScripts[];
extern u32 RoomC1_ObjectNames[];

extern PTMF RoomC1_CondTable[];

/* 0x0034A360 */
void *RoomC1_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomC1_vtable, RoomBase_vtable); }

/* 0x0034A3C0 */
void *RoomC1_EnterScript(void) {
    return RoomC1_EnterScript_data;
}

/* 0x0034A3D0 */
void *RoomC1_CharEnterScript(void) {
    return RoomC1_CharEnterScript_data;
}

/* 0x0034A3E0 */
void *RoomC1_Phase1Script(void) {
    return RoomC1_Phase1Script_data;
}

/* 0x0034A3F0 */
void *RoomC1_Phase2Script(void) {
    return RoomC1_Phase2Script_data;
}

/* 0x0034A400 */
void *RoomC1_Phase3Script(void) {
    return RoomC1_Phase3Script_data;
}

/* 0x0034A410 */
u32 RoomC1_ActionScript(void *self, s32 i) {
    return RoomC1_ActionScripts[i];
}

/* 0x0034A430 */
void *RoomC1_Table38(void) {
    return RoomC1_Table38_data;
}

/* 0x0034A440 */
u32 RoomC1_ObjectName(void *self, s32 i) {
    return RoomC1_ObjectNames[i];
}

/* (self->*RoomC1_CondTable[i])(a, b) */
/* 0x0034A460 */
s32 RoomC1_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomC1_CondTable[i & 0xFF], a, b);
}

/* 0x0034A490 */
s32 RoomC1_Cond00(void) {
    return Countdown_Seconds((u8 *)gProgress + 0x764) == 0;
}
