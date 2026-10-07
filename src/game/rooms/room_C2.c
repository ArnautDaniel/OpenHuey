/* Room 0xC2: its event handler class (vtable RoomC2_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "sound.h"
#include "progress.h"
#include "msl.h"

extern void *RoomBase_vtable[];
extern void *RoomC2_vtable[];
extern u8 RoomC2_Table38_data[];
extern const char *RoomC2_ObjectNames;
extern u32 RoomC2_EnterScript_data[];
extern u32 RoomC2_CharEnterScript_data[];
extern u32 RoomC2_Phase1Script_data[];
extern u32 RoomC2_Phase2Script_data[];
extern u32 RoomC2_Phase5Script_data[];
extern u32 RoomC2_ActionScripts[];

extern PTMF RoomC2_CmdTable[];
extern PTMF RoomC2_CondTable[];

/* the turn (+0x14) of the room object named RoomC2_ObjectNames and the creak (sound 0x80000002, bank 6)
 * at its edge, 45 out at height 152 */
static inline void swing_to(u8 *o, f32 a) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 pos[4] __attribute__((aligned(16)));

    AT(o, 0x14, f32) = kPi.f * a / 180.0f;
    pos[1] = 152.0f;
    pos[0] = -45.0f * msl_cosf(AT(o, 0x14, f32));
    pos[2] = -45.0f * -msl_sinf(AT(o, 0x14, f32));
    pos[3] = 1.0f;
    Sound_PlayBankAt(gSound, 0x80000002, 6, pos, 0, 0);
}

/* 0x0034A4C0 */
void *RoomC2_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomC2_vtable, RoomBase_vtable); }

/* 0x0034A520 */
void *RoomC2_EnterScript(void) {
    return RoomC2_EnterScript_data;
}

/* 0x0034A530 */
void *RoomC2_CharEnterScript(void) {
    return RoomC2_CharEnterScript_data;
}

/* 0x0034A540 */
void *RoomC2_Phase1Script(void) {
    return RoomC2_Phase1Script_data;
}

/* 0x0034A550 */
void *RoomC2_Phase2Script(void) {
    return RoomC2_Phase2Script_data;
}

/* 0x0034A560 */
void *RoomC2_Phase5Script(void) {
    return RoomC2_Phase5Script_data;
}

/* 0x0034A570 */
u32 RoomC2_ActionScript(void *self, s32 i) {
    return RoomC2_ActionScripts[i];
}

/* 0x0034A590 */
void *RoomC2_Table38(void *o) { return RoomC2_Table38_data; }   /* RoomC2_vtable +0x38 */

/* 0x0034A5A0 */
u32 RoomC2_ObjectName(void *self, s32 i) {
    return ((u32 *)&RoomC2_ObjectNames)[i];   /* its table of names (one is reached by name too) */
}

/* (self->*RoomC2_CondTable[i])(a, b) */
/* 0x0034A5C0 */
s32 RoomC2_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomC2_CondTable[i & 0xFF], a, b);
}

/* room 0xC2: the summoner's countdown (progress +0x764) has run out. */
/* 0x0034A5F0 */
s32 RoomC2_Cond00(void) {
    return Countdown_Seconds((u8 *)gProgress + 0x764) == 0;
}

/* (self->*RoomC2_CmdTable[i])(a, b) */
/* 0x0034A620 */
s32 RoomC2_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomC2_CmdTable[i & 0xFF], a, b);
}

/* byte 3 to the player's Character_ChooseExit while she's active */
/* 0x0034A650 */
s32 RoomC2_Cmd01(void *self, void *a1, u8 *cmd) {
    if (gCharPlayer != NULL && AT(gCharPlayer, 0x28, u8) != 0) {
        Character_ChooseExit(gCharPlayer, cmd[3]);
    }
    return 1;
}

/* byte 3 0 / 1: shut / open (pi/2); 2 / 3 opening / shutting by script variable 0 (0..120 frames,
 * eased by a sine) */
/* 0x0034A6A0 */
s32 RoomC2_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    u8 *o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, RoomC2_ObjectNames);
    f32 t, e;

    if (o == NULL) {
        return 1;
    }
    t = (f32)(u32)VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0) / 120.0f;
    switch (cmd[3]) {
    case 0:
        AT(o, 0x14, s32) = 0;
        break;
    case 1:
        AT(o, 0x14, u32) = 0x3FC90FDB;   /* pi/2 */
        break;
    case 2:
        e = msl_sinf(kPi.f * (-90.0f + 180.0f * t) / 180.0f);
        swing_to(o, 45.0f * (1.0f + e));
        break;
    case 3:
        e = msl_sinf(kPi.f * (-90.0f + 180.0f * t) / 180.0f);
        swing_to(o, 90.0f - 45.0f * (1.0f + e));
        break;
    }
    return 1;
}
