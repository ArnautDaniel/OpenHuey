/* Room 0xC3: its event handler class (vtable RoomC3_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"
#include "snd_place.h"
#include "stalker_progress.h"
#include "msl.h"

extern void *RoomBase_vtable[];
extern void *RoomC3_vtable[];
extern u8 RoomC3_Table38_data[];
extern const char *pstr_doramukan_2[];
extern const char *RoomC3_ObjectNames;
extern u32 RoomC3_EnterScript_data[];
extern u32 RoomC3_CharEnterScript_data[];

extern u32 RoomC3_Phase1Script_data[];
extern u32 RoomC3_Phase2Script_data[];
extern u32 RoomC3_Phase5Script_data[];
extern u32 RoomC3_ActionScripts[];

extern PTMF RoomC3_CmdTable[];
extern PTMF RoomC3_CondTable[];

/* (the other door: turned the other way, its creak at (45 sin, 142, 45 cos)) */
static inline void swing_to2(u8 *o, f32 a) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 pos[4] __attribute__((aligned(16)));

    AT(o, 0x14, f32) = kPi.f * a / 180.0f;
    pos[1] = 142.0f;
    pos[0] = 45.0f * func_0031C248(AT(o, 0x14, f32));
    pos[3] = 1.0f;
    pos[2] = 45.0f * func_0031C058(AT(o, 0x14, f32));
    Sound_PlayBankAt(gSound, 0x80000002, 6, pos, 0, 0);
}

/* 0x0034A9B0 */
void *RoomC3_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomC3_vtable, RoomBase_vtable); }

/* 0x0034AA10 */
void *RoomC3_EnterScript(void) {
    return RoomC3_EnterScript_data;
}

/* 0x0034AA20 */
void *RoomC3_CharEnterScript(void) {
    return RoomC3_CharEnterScript_data;
}

/* 0x0034AA30 */
void *RoomC3_Phase1Script(void) {
    return RoomC3_Phase1Script_data;
}

/* 0x0034AA40 */
void *RoomC3_Phase2Script(void) {
    return RoomC3_Phase2Script_data;
}

/* 0x0034AA50 */
void *RoomC3_Phase5Script(void) {
    return RoomC3_Phase5Script_data;
}

/* 0x0034AA60 */
u32 RoomC3_ActionScript(void *self, s32 i) {
    return RoomC3_ActionScripts[i];
}

/* 0x0034AA80 */
void *RoomC3_Table38(void *o) { return RoomC3_Table38_data; }   /* RoomC3_vtable +0x38 */

/* 0x0034AA90 */
u32 RoomC3_ObjectName(void *self, s32 i) {
    return ((u32 *)&RoomC3_ObjectNames)[i];   /* its table of names (one is reached by name too) */
}

/* (self->*RoomC3_CondTable[i])(a, b) */
/* 0x0034AAB0 */
s32 RoomC3_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomC3_CondTable[i & 0xFF], a, b);
}

/* 0x0034AAE0 */
s32 RoomC3_Cond00(void) {
    return Countdown_Seconds((u8 *)gProgress + 0x764) == 0;
}

/* (self->*RoomC3_CmdTable[i])(a, b) */
/* 0x0034AB10 */
s32 RoomC3_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomC3_CmdTable[i & 0xFF], a, b);
}

/* sound 3 (bank 6) at the room object named pstr_doramukan_2[0] */
/* 0x0034AB40 */
s32 RoomC3_Cmd02(void) {
    u8 *o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, pstr_doramukan_2[0]);

    if (o != NULL) {
        Sound_PlayBankAt(gSound, 3, 6, (f32 *)(o + 0x20), 0, 0);
    }
    return 1;
}

/* the player (active, +0xE0 clear) put in action 0xB / 0x21 / 0xFF unless held (7); then
 * progress +0x7B8 gets 50 */
/* 0x0034ABB0 */
s32 RoomC3_Cmd01(void) {
    CharAction act;

    if (gCharPlayer == NULL || AT(gCharPlayer, 0x28, u8) == 0 || AT(gCharPlayer, 0xE0, u8) != 0) {
        return 1;
    }
    func_002A8410((u8 *)&act);
    act.state = 0xB;
    act.a = 0x21;
    act.b = 0xFF;
    if (AT(gCharPlayer, 0x14E8, s32) != 7) {
        char_set_action((u8 *)gCharPlayer, &act);
    }
    Threat_Raise((u8 *)gProgress + 0x7B8, 50.0f);
    return 1;
}

/* (as RoomC2_Cmd00, the room object named RoomC3_ObjectNames, opening to -pi/2) */
/* 0x0034AD00 */
s32 RoomC3_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    u8 *o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, RoomC3_ObjectNames);
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
        AT(o, 0x14, u32) = 0xBFC90FDB;   /* -pi/2 */
        break;
    case 2:
        e = func_0031C248(kPi.f * (-90.0f + 180.0f * t) / 180.0f);
        swing_to2(o, -45.0f * (1.0f + e));
        break;
    case 3:
        e = func_0031C248(kPi.f * (-90.0f + 180.0f * t) / 180.0f);
        swing_to2(o, -90.0f - -45.0f * (1.0f + e));
        break;
    }
    return 1;
}
