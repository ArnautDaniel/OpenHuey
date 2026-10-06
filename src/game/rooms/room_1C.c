/* Room 0x1C: its event handler class (vtable Room1C_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"
#include "snd_place.h"

extern void *RoomBase_vtable[];
extern void *Room1C_vtable[];
extern void *DepthRange_vtable[];

extern u32 Room1C_ActionScripts[];
extern u8 Room1C_Table38_data[];
extern u32 Room1C_ObjectNames[];

extern u8 Room1C_EnterScript_data[];
extern u8 Room1C_CharEnterScript_data[];
extern u8 Room1C_Phase1Script_data[];
extern u8 Room1C_Phase2Script_data[];
extern PTMF Room1C_CmdTable[];

/* 0x002AD9F0 */
void *Room1C_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room1C_vtable, RoomBase_vtable); }

/* 0x002ADA50 */
void *Room1C_EnterScript(void) {
    return Room1C_EnterScript_data;
}

/* 0x002ADA60 */
void *Room1C_CharEnterScript(void) {
    return Room1C_CharEnterScript_data;
}

/* 0x002ADA70 */
void *Room1C_Phase1Script(void) {
    return Room1C_Phase1Script_data;
}

/* 0x002ADA80 */
void *Room1C_Phase2Script(void) {
    return Room1C_Phase2Script_data;
}

/* 0x002ADA90 */
u32 Room1C_ActionScript(void *self, s32 i) {
    return Room1C_ActionScripts[i];
}

/* 0x002ADAB0 */
void *Room1C_Table38(void) {
    return Room1C_Table38_data;
}

/* 0x002ADAC0 */
u32 Room1C_ObjectName(void *self, s32 i) {
    return Room1C_ObjectNames[i];
}

/* (self->*Room1C_CmdTable[i])(a, b) */
/* 0x002ADAE0 */
s32 Room1C_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room1C_CmdTable[i & 0xFF], a, b);
}

/* room 0x1C (D_003FD350): a sound (0xC0000000, bank 6) at the room's effect 1 */
/* 0x002ADB10 */
s32 Room1C_Cmd01(void) {
    u8 *e = RoomEffects_Get(gRoomEffects, 1);
    f32 at[4] __attribute__((aligned(16)));

    at[0] = AT(e, 0x20, f32);
    at[1] = AT(e, 0x24, f32);
    at[2] = AT(e, 0x28, f32);
    Sound_PlayBankAt(gSound, 0xC0000000, 6, at, 0, 0);
    return 1;
}

/* room 0x1C (Room1C_Cmd00_ptmf): room effect 0x1C (a depth range) with the cutscene from frame 0x14A:
 * near 1 .. 1 + 1.4 t (at most 67.6), far 48.6 + 4 t (at most 230) */
/* 0x002ADB70 */
s32 Room1C_Cmd00(void) {
    f32 t = (f32)(VCALL(gCutscene, 0x34, s32 (*)(VObject *))(gCutscene) - 0x14A);
    f32 r[4];
    f32 v;

    room_effect_slot_new(gRoomEffects, 0x1C, DepthRange_vtable);
    r[0] = 1.0f;
    v = 1.0f + 0x1.6666660000000p+0f /* 1.4 */ * t;
    r[1] = v <= 0x1.0e66660000000p+6f /* 67.6 */ ? v : 0x1.0e66660000000p+6f /* 67.6 */;
    r[2] = v <= 0x1.0e66660000000p+6f /* 67.6 */ ? v : 0x1.0e66660000000p+6f /* 67.6 */;
    v = 0x1.84cccc0000000p+5f /* 48.6 */ + 4.0f * t;
    r[3] = v <= 230.0f ? v : 230.0f;
    RoomEffects_Send(gRoomEffects, 0x1C, r);
    return 1;
}
