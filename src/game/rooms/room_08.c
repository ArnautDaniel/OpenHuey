/* Room 0x08: its event handler class (vtable Room08_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "progress.h"
#include "msl.h"
#include "sce/libvu0.h"

extern void *RoomBase_vtable[];
extern void *Room08_vtable[];

extern u8 Room08_EnterScript_data[];
extern u8 Room08_CharEnterScript_data[];
extern u8 Room08_Phase1Script_data[];
extern u8 Room08_Phase2Script_data[];
extern u8 Room08_Phase3Script_data[];
extern u8 Room08_Phase5Script_data[];
extern void *Room08_ActionScripts[];
extern void *Room08_ObjectNames[];
extern u8 Room08_Table38_data[];
extern PTMF Room08_CmdTable[];
extern PTMF Room08_CondTable[];

/* 0x002AA440 */
void *Room08_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room08_vtable, RoomBase_vtable); }

/* 0x002AA4A0 */
void *Room08_EnterScript(void) {
    return Room08_EnterScript_data;
}

/* 0x002AA4B0 */
void *Room08_CharEnterScript(void) {
    return Room08_CharEnterScript_data;
}

/* 0x002AA4C0 */
void *Room08_Phase1Script(void) {
    return Room08_Phase1Script_data;
}

/* 0x002AA4D0 */
void *Room08_Phase2Script(void) {
    return Room08_Phase2Script_data;
}

/* 0x002AA4E0 */
void *Room08_Phase3Script(void) {
    return Room08_Phase3Script_data;
}

/* 0x002AA4F0 */
void *Room08_Phase5Script(void) {
    return Room08_Phase5Script_data;
}

/* 0x002AA500 */
void *Room08_ActionScript(void *self, s32 i) {
    return Room08_ActionScripts[i];
}

/* 0x002AA520 */
void *Room08_Table38(void) {
    return Room08_Table38_data;
}

/* 0x002AA530 */
void *Room08_ObjectName(void *self, s32 i) {
    return Room08_ObjectNames[i];
}

/* (self->*Room08_CondTable[i])(a, b) */
/* 0x002AA550 */
s32 Room08_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room08_CondTable[i & 0xFF], a, b);
}

/* room 0x08 (Room08_Cond01_ptmf): the stalker is there but not about */
/* 0x002AA580 */
s32 Room08_Cond01(void) {
    u8 *c = (u8 *)gCharSlot2;

    return c != NULL && AT(c, 0x28, u8) == 0;
}

/* door be16 cmd[3..4]: Progress_DoorOpen */
/* 0x002AA5B0 */
s32 Room08_Cond00(void *self, void *a1, u8 *cmd) {
    return Progress_DoorOpen(gProgress, (cmd[3] << 8 | cmd[4]) & 0xFFFF);
}

/* (self->*Room08_CmdTable[i])(a, b) */
/* 0x002AA5D0 */
s32 Room08_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room08_CmdTable[i & 0xFF], a, b);
}

/* room 0x08 (D_003F31D8): the hanging object named by the handler's string 0xA - byte 3 0 sets
 * it still (+0x30 / +0x38 0, travel +0x3C 0.9); 1: the player's travel (+0x3C, its last move's
 * length) past 5 makes it creak (sounds 4 / 5 by turns, event bit 7) and swing for 20 frames:
 * its tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36 a frame */
/* 0x002AA600 */
s32 Room08_Cmd01(VObject *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k09 = {0x3F666666}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    const char *name = VCALL(self, 0x34, const char *(*)(VObject *, s32))(self, 0xA);
    u8 *o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, name);
    f32 t, a;

    if (o == NULL) {
        return 1;
    }
    if (cmd[3] == 0) {
        AT(o, 0x30, s32) = 0;
        AT(o, 0x38, s32) = 0;
        AT(o, 0x3C, f32) = k09.f;
        return 1;
    }
    if (cmd[3] != 1) {
        return 1;
    }
    if (gCharPlayer != NULL) {
        f32 d[4] __attribute__((aligned(16)));

        sceVu0CopyVector(d, (f32 *)((u8 *)gCharPlayer + 0x40));
        sceVu0SubVector(d, (f32 *)((u8 *)gCharPlayer + 0x10), d);
        t = AT(o, 0x3C, f32) + __builtin_sqrtf(d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
        AT(o, 0x3C, f32) = t;
        if (!(t <= 5.0f)) {
            VObject *ev = gEvents;

            AT(o, 0x38, f32) = 20.0f;
            AT(o, 0x3C, f32) = 0.0f;
            if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 7) == 1) {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 7);
                Actor_PlaySound(&gCharPlayer->a, 4, 6, 0, 0, NULL);
            } else {
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 7);
                Actor_PlaySound(&gCharPlayer->a, 5, 6, 0, 0, NULL);
            }
        }
    }
    t = AT(o, 0x38, f32);
    if (t <= 0.0f) {
        return 1;
    }
    AT(o, 0x38, f32) = t - 1.0f;
    if (t - 1.0f < 0.0f) {
        AT(o, 0x38, f32) = 0.0f;
    }
    a = AT(o, 0x30, f32) + 36.0f;
    AT(o, 0x30, f32) = a;
    if (!(a < 360.0f)) {
        AT(o, 0x30, f32) = a - 360.0f;
    }
    a = kPi.f * (1.0f + func_0031C248(kPi.f * AT(o, 0x30, f32) / 180.0f)) / 180.0f;
    AT(o, 0x10, f32) = a;
    if (!(a <= kPi.f)) {
        AT(o, 0x10, f32) = a - k2Pi.f;
    }
    return 1;
}

/* room 0x08 (Room08_Cmd00_ptmf): the cutscene director's +0x6C 3 (byte 3 0) or 2 */
/* 0x002AA8C0 */
s32 Room08_Cmd00(void *self, void *a1, u8 *cmd) {
    VCALL(gCutscene, 0x6C, void (*)(VObject *, s32))(gCutscene, cmd[3] == 0 ? 3 : 2);
    return 1;
}
