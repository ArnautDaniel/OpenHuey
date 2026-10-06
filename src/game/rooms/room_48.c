/* Room 0x48: its event handler class (vtable Room48_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "progress.h"
#include "scene_game_members.h"
#include "msl.h"
#include "sce/libvu0.h"

extern void *RoomBase_vtable[];
extern void *Room48_vtable[];
extern void *D_00479890[];
extern u8 D_004252D0[];
extern u8 D_00425460[];
extern u8 D_00425520[];
extern u8 D_004258D0[];
extern u8 D_004259B0[];
extern void *D_00426760[];

extern void *D_00426860[];
extern u8 D_00426890[];

extern PTMF D_01991140[];
extern PTMF D_01991188[];

static void effect_79890_init(void **obj) {
    obj[0] = D_00479890;
}

/* 0x0030F070 */
void *Room48_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room48_vtable, RoomBase_vtable); }

/* 0x0030F0D0 */
void *Room48_EnterScript(void) {
    return D_004252D0;
}

/* 0x0030F0E0 */
void *Room48_CharEnterScript(void) {
    return D_00425460;
}

/* 0x0030F0F0 */
void *Room48_Phase1Script(void) {
    return D_00425520;
}

/* 0x0030F100 */
void *Room48_Phase2Script(void) {
    return D_004258D0;
}

/* 0x0030F110 */
void *Room48_Phase5Script(void) {
    return D_004259B0;
}

/* 0x0030F120 */
void *Room48_ActionScript(void *self, s32 i) {
    return D_00426760[i];
}

/* 0x0030F140 */
void *Room48_Table38(void) {
    return D_00426890;
}

/* 0x0030F150 */
void *Room48_ObjectName(void *self, s32 i) {
    return D_00426860[i];
}

/* (self->*D_01991188[i])(a, b) */
/* 0x0030F170 */
s32 Room48_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991188[i & 0xFF], a, b);
}

/* room 0x48 (Room48_Cond00_ptmf): door 0 of room 0x48 (Progress_CurRoomFlag) */
/* 0x0030F1A0 */
s32 Room48_Cond00(void) {
    return Progress_CurRoomFlag(gProgress, 0x48, 0);
}

/* (self->*D_01991140[i])(a, b) */
/* 0x0030F1C0 */
s32 Room48_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991140[i & 0xFF], a, b);
}

/* room 0x48 (Room48_Cmd05_ptmf): the creatures (10) in play in the current room: the list's +0x2C */
/* 0x0030F1F0 */
s32 Room48_Cmd05(void) {
    u8 *list = gCreatures;
    Progress *g = gProgress;
    s32 i;

    for (i = 0; i < 10; i++) {
        u8 *c = AT(list, i * 4, u8 *);

        if (c != NULL && AT(c, 0x28, u8) == 1
            && AT(c, 0x30, s32) == VCALL((VObject *)g, 0xC, s32 (*)(VObject *))((VObject *)g)) {
            VCALL_AT(list, 0x28, 0x2C, void (*)(u8 *, s32, s32))(list, i & 0xFF, 0);
        }
    }
    return 1;
}

/* room 0x48 (Room48_Cmd04_ptmf): the player's Character_ChooseExit(0) */
/* 0x0030F2B0 */
s32 Room48_Cmd04(void) {
    Character_ChooseExit(gCharPlayer, 0);
    return 1;
}

/* room 0x48 (Room48_Cmd03_ptmf): character 0xFE's model +0x9E8 = -0.15 (byte 3 0) or 0 */
/* 0x0030F2E0 */
s32 Room48_Cmd03(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)Progress_SlotOfId(gProgress, 0xFE)]->motion;

    if (cmd[3] == 0) {
        AT(m, 0x9E8, u32) = 0xBE19999A;
    } else {
        AT(m, 0x9E8, f32) = 0.0f;
    }
    return 1;
}

/* room 0x48 (D_00426818): byte 3 0 starts the wall shadow (D_00479890, its slot in event
 * variable 2); else that one is ended */
/* 0x0030F350 */
s32 Room48_Cmd02(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(gEffects, 8, effect_79890_init);

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 2, slot);
    } else {
        EffectMgr_Start(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 2), NULL);
    }
    return 1;
}

/* room 0x48 (Room48_Cmd01_ptmf): character 0x26's +0xE4 cleared */
/* 0x0030F490 */
s32 Room48_Cmd01(void) {
    AT(gCharacters[(u8)Progress_SlotOfId(gProgress, 0x26)], 0xE4, u8) = 0;
    return 1;
}

/* room 0x48 (D_004267F8): the handler's objects 2 and 3 swing (phases 60 degrees apart) - byte 3
 * 0 sets them still; 1: while slower than 5, Hewie's movement (the squared length of his last
 * step, +0x3C) past 1 makes them swing for 20 frames (the first also creaks: sounds 4 / 5 by
 * turns, event bit 0x11); the tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36 */
/* 0x0030F4E0 */
s32 Room48_Cmd00(VObject *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    VObject *objs = D_00456DF8;
    VObject *ev = gEvents;
    s32 i;

    for (i = 0; i < 2; i++) {
        const char *name = VCALL(self, 0x34, const char *(*)(VObject *, s32))(self, i + 2);
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, name);
        f32 t, a;

        if (o == NULL) {
            continue;
        }
        if (cmd[3] == 0) {
            AT(o, 0x30, s32) = 0;
            AT(o, 0x34, f32) = 60.0f * (f32)i;
            AT(o, 0x38, s32) = 0;
            AT(o, 0x3C, s32) = 0;
            continue;
        }
        if (cmd[3] != 1) {
            continue;
        }
        if (gCharPartner != NULL && AT(o, 0x38, f32) < 5.0f) {
            f32 d[4] __attribute__((aligned(16)));

            sceVu0CopyVector(d, (f32 *)((u8 *)gCharPartner + 0x40));
            sceVu0SubVector(d, d, (f32 *)((u8 *)gCharPartner + 0x10));
            t = AT(o, 0x3C, f32) + (d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
            AT(o, 0x3C, f32) = t;
            if (!(t <= 1.0f)) {
                AT(o, 0x38, f32) = 20.0f;
                AT(o, 0x3C, f32) = 0.0f;
                if (i == 0) {
                    if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 0x11) == 1) {
                        VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0x11);
                        Actor_PlaySound(&gCharPlayer->a, 4, 6, 0, 0, NULL);
                    } else {
                        VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0x11);
                        Actor_PlaySound(&gCharPlayer->a, 5, 6, 0, 0, NULL);
                    }
                }
            }
        }
        t = AT(o, 0x38, f32);
        if (t <= 0.0f) {
            continue;
        }
        AT(o, 0x38, f32) = t - 1.0f;
        if (t - 1.0f < 0.0f) {
            AT(o, 0x38, f32) = 0.0f;
        }
        a = AT(o, 0x30, f32) + 36.0f;
        AT(o, 0x30, f32) = a;
        if (!(a + AT(o, 0x34, f32) < 360.0f)) {
            AT(o, 0x30, f32) = a - 360.0f;
        }
        a = kPi.f * (1.0f + func_0031C248(kPi.f * (AT(o, 0x30, f32) + AT(o, 0x34, f32)) / 180.0f)) / 180.0f;
        AT(o, 0x10, f32) = a;
        if (!(a <= kPi.f)) {
            AT(o, 0x10, f32) = a - k2Pi.f;
        }
    }
    return 1;
}
