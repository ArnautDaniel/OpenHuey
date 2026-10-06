/* Room 0x92: its event handler class (vtable Room92_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "memcard.h"
#include "hewie.h"
#include "lorenzo.h"
#include "model.h"
#include "placed.h"
#include "progress.h"
#include "scene_game_members.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif
#include "sce/libvu0.h"

extern void *RoomBase_vtable[];
extern void *Room92_vtable[];
extern void *DepthRange_vtable[];
extern void *WispColumn_vtable[];
extern const char *D_00437D48[];   /* room objects 10..15 */
extern const char *D_00437D40[];   /* room objects 8, 9 */
extern void *Room92Effect_vtable[];
extern void *Embers_vtable[];
extern void *SparkSpray_vtable[];
extern const char *D_00437D30[];   /* room objects 4, 5 */
extern void *SmokeTrail_vtable[];
extern u32 Room92_EnterScript_data[];
extern u32 Room92_CharEnterScript_data[];
extern u32 Room92_Phase1Script_data[];
extern u32 Room92_Phase2Script_data[];
extern u32 Room92_Phase3Script_data[];
extern u32 Room92_ActionScripts[];
extern u32 Room92_ObjectNames[];

extern void *EffectBase_vtable[];

extern PTMF Room92_CmdTable[];

static void effect_1c60_init(void **obj) {
    obj[0] = WispColumn_vtable;
    obj[0x1810 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = QuadDrawer_vtable;
}

static void effect_7A2F0_init(void **obj) {
    obj[0] = Room92Effect_vtable;
}

static void effect_7A310_init(void **obj) {
    obj[0] = Embers_vtable;
    obj[0xF10 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0xF14 / 4] = -1;
    obj[0xF10 / 4] = QuadDrawer_vtable;
    obj[0xF48 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0xF4C / 4] = -1;
    obj[0xF48 / 4] = QuadDrawer_vtable;
}

static void effect_79F70_init(void **obj) {
    obj[0] = SparkSpray_vtable;
    obj[0x1810 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = QuadDrawer_vtable;
}

static void effect_79F30_init(void **obj) {
    obj[0] = SmokeTrail_vtable;
    obj[0xC10 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = QuadDrawer_vtable;
}

#ifdef HG_NATIVE
#endif

/* 0x00341B10 */
void *Room92_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room92_vtable, RoomBase_vtable); }

/* 0x00341B70 */
void *Room92_EnterScript(void) {
    return Room92_EnterScript_data;
}

/* 0x00341B80 */
void *Room92_CharEnterScript(void) {
    return Room92_CharEnterScript_data;
}

/* 0x00341B90 */
void *Room92_Phase1Script(void) {
    return Room92_Phase1Script_data;
}

/* 0x00341BA0 */
void *Room92_Phase2Script(void) {
    return Room92_Phase2Script_data;
}

/* 0x00341BB0 */
void *Room92_Phase3Script(void) {
    return Room92_Phase3Script_data;
}

/* 0x00341BC0 */
u32 Room92_ActionScript(void *self, s32 i) {
    return Room92_ActionScripts[i];
}

/* 0x00341BE0 */
u32 Room92_ObjectName(void *self, s32 i) {
    return Room92_ObjectNames[i];
}

/* (self->*Room92_CmdTable[i])(a, b) */
/* 0x00341C00 */
s32 Room92_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room92_CmdTable[i & 0xFF], a, b);
}

/* (as Room02_Cmd02)  the 0x10D0-byte effect Embers_vtable: byte 3 0 / 1 one made and sent 1 / 0,
 * its slot kept in script variable 11; 2 that one sent nothing */
/* 0x00341C30 */
s32 Room92_Cmd13(void *self, void *a1, u8 *cmd) {
    u8 *mgr;
    s32 slot, arg;

    switch (cmd[3]) {
    case 0:
    case 1:
        mgr = gEffects;
        slot = Effect_New(mgr, 0x10D0, effect_7A310_init);
        arg = cmd[3] == 0;
        EffectMgr_Start(mgr, slot, &arg);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 11, slot);
        break;
    case 2:
        slot = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 11);
        EffectMgr_Start(gEffects, slot, NULL);
        break;
    }
    return 1;
}

/* (as Room49_Cmd02)  byte 3 0: the 0x14-byte effect Room92Effect_vtable spawned, its slot in script
 * variable 10; 1 / 2: it is sent (byte 4, byte 3 - 1) */
/* 0x00341EF0 */
s32 Room92_Cmd12(void *self, void *a1, u8 *cmd) {
    f32 msg[2];
    s32 slot;

    switch (cmd[3]) {
    case 0:
        slot = Effect_New(gEffects, 0x14, effect_7A2F0_init);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 10, slot);
        break;
    case 1:
    case 2:
        slot = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 10);
        msg[0] = (f32)cmd[4];
        msg[1] = (f32)(cmd[3] - 1);
        EffectMgr_Start(gEffects, slot, msg);
        break;
    }
    return 1;
}

/* (as Room49_Cmd01)  the depth range (effect 0x1C) from cutscene frame 825: 1 / 1 + 0.3 a frame
 * up to 21 / 80 + 0.7 a frame up to 120 / 160 */
/* 0x003420A0 */
s32 Room92_Cmd11(void) {
    static const union { u32 u; f32 f; } kNear = {0x3E999999}, kFar = {0x3F333333};
    u8 *fx = gRoomEffects;
    f32 t = (f32)(VCALL(gCutscene, 0x34, s32 (*)(VObject *))(gCutscene) - 825);
    f32 r[4] __attribute__((aligned(16)));

    room_effect_slot_new(fx, 0x1C, DepthRange_vtable);
    r[0] = 1.0f;
    r[1] = 1.0f + kNear.f * t;
    if (!(r[1] <= 21.0f)) {
        r[1] = 21.0f;
    }
    r[2] = 80.0f + kFar.f * t;
    if (!(r[2] <= 120.0f)) {
        r[2] = 120.0f;
    }
    r[3] = 160.0f;
    RoomEffects_Send(fx, 0x1C, r);
    return 1;
}

/* the placed things of kind 9 / 10 (+0x20) all reset (+0x28) */
/* 0x00342230 */
s32 Room92_Cmd10(void) {
    VObject *list = gPlacedThings;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        u8 *o = VCALL(list, 0xC, u8 *(*)(VObject *, s32))(list, i);

        if (o != NULL && (u32)(AT(o, 0x20, s32) - 9) < 2) {
            AT(o, 0x28, u8) = 0;
        }
    }
    return 1;
}

/* a noise at (-70 or 70 by byte 3, 14, 0): byte 4 0 / 1 / 2 kind 1 / 2 / 4 */
/* 0x003422B0 */
s32 Room92_Cmd09(void *self, void *a1, u8 *cmd) {
    f32 pos[4] __attribute__((aligned(16)));
    u32 which;

    pos[0] = cmd[3] == 0 ? -70.0f : 70.0f;
    pos[1] = 14.0f;
    pos[2] = 0.0f;
    pos[3] = 1.0f;
    switch (cmd[4]) {
    case 2:
        which = 4;
        break;
    case 1:
        which = 2;
        break;
    case 0:
        which = 1;
        break;
    default:
        which = (u32)(unsigned long)a1;   /* never set on the PS2: the register still holds a1 */
        break;
    }
    Progress_Noise(gProgress, pos, which & 0xFF, 2, 5, 0, 10.0f);
    return 1;
}

/* room objects 8 / 9 raised (+0x24 down 0.2 a call to 0) while the event manager's +0x58 test 4 /
 * 5 holds, else lowered back (up 0.4 a call to 0.7) */
/* 0x00342370 */
s32 Room92_Cmd08(void) {
    static const union { u32 u; f32 f; } kDown = {0x3E4CCCCD}, kTop = {0x3F333333}, kUp = {0x3ECCCCCD};
    VObject *objs = gRoomObjects, *ev = gEvents;
    s32 k;

    for (k = 8; k < 10; k++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00437D40[k - 8]);
        u8 on = 0;
        f32 y;

        if (k == 8) {
            if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 4) == 1) {
                on = 1;
            }
        } else if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 5) == 1) {
            on = 1;
        }
        if (o == NULL) {
            continue;
        }
        y = AT(o, 0x24, f32);
        if (on == 1) {
            if (!(y <= 0.0f)) {
                y -= kDown.f;
                AT(o, 0x24, f32) = y;
                if (y < 0.0f) {
                    AT(o, 0x24, f32) = 0.0f;
                }
            }
        } else if (y < kTop.f) {
            y += kUp.f;
            AT(o, 0x24, f32) = y;
            if (!(y <= kTop.f)) {
                AT(o, 0x24, f32) = kTop.f;
            }
        }
    }
    return 1;
}

/* (as slam_shake, both of Lorenzo's forms: 0xA and 0x27) */
/* 0x00342500 */
s32 Room92_Cmd07(void) {
    if (gCharPursuer == NULL) {
        return 1;
    }
    if (AT(gCharPursuer, 0x153C, u8) == 0xA) {
        if (Lorenzo2_SlamImpact((Pursuer *)gCharSlot2) != 0) {
            VCALL(gCamera, 0x6C, void (*)(VObject *, f32))(gCamera, 0.5f);
        }
    } else if (AT(gCharPursuer, 0x153C, u8) == 0x27) {
        if (Kind39_SlamImpact((Pursuer *)gCharSlot2) != 0) {
            VCALL(gCamera, 0x6C, void (*)(VObject *, f32))(gCamera, 0.5f);
        }
    }
    return 1;
}

/* the 0x20E0-byte effect SparkSpray_vtable (sent byte 3): byte 3 0 / 1 one made, its slot in script
 * variable 2 / 3; 2 / 3 the one in variable 2 / 3 (if any) sent nothing */
/* 0x003425D0 */
s32 Room92_Cmd06(void *self, void *a1, u8 *cmd) {
    u8 *mgr;
    u8 arg = cmd[3];
    s32 slot;

    switch (cmd[3]) {
    case 0:
    case 1:
        mgr = gEffects;
        slot = Effect_New(mgr, 0x20E0, effect_79F70_init);
        EffectMgr_Start(mgr, slot, &arg);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, cmd[3] + 2, slot);
        break;
    case 2:
    case 3:
        slot = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, cmd[3]);
        if (slot != -1) {
            EffectMgr_Start(gEffects, slot, NULL);
        }
        break;
    }
    return 1;
}

/* the things that fell below -30: placed things of kind 9 are reset (+0x28) and each counts down
 * script variable 6 (and the event manager's +0x5C); room objects 10..15 that did get +0 set */
/* 0x003428D0 */
s32 Room92_Cmd05(void) {
    VObject *ev = gEvents, *list, *objs;
    s32 n = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 6);
    s32 i;

    list = gPlacedThings;
    for (i = 0; i < 0x80; i++) {
        u8 *o = VCALL(list, 0xC, u8 *(*)(VObject *, s32))(list, i);

        if (o != NULL && AT(o, 0x20, s32) == 9 && AT(o, 0x14, f32) < -30.0f) {
            AT(o, 0x28, u8) = 0;
            VCALL(ev, 0x5C, void (*)(VObject *))(ev);
            n--;
        }
    }
    objs = gRoomObjects;
    for (i = 10; i < 16; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00437D48[i - 10]);

        if (AT(o, 0x24, f32) < -30.0f) {
            AT(o, 0x0, u8) = 1;
        }
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 6, n);
    return 1;
}

/* (as Room49_Cmd02) byte 3 0: the effect WispColumn_vtable spawned (told 1), its slot in event var
   0; 1: it is told 0 */
/* 0x00342A30 */
s32 Room92_Cmd04(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0: {
        u8 *mgr = gEffects;
        s32 slot = Effect_New(mgr, 0x1C60, effect_1c60_init);

        EffectMgr_Start(mgr, slot, (void *)1);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, slot);
        break;
    }
    case 1: {
        s32 slot = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0);

        if (slot != -1) {
            EffectMgr_Start(gEffects, slot, NULL);
        }
        break;
    }
    }
    return 1;
}

/* the placed thing 10 brought back (list +0x14 / +0x8, its +0xC, +0x28 on) and put on one of five
 * spots round a circle (script variable 1, then on by 2 of 5): turned to the spot's angle (with a
 * little random), 2.1 up and 1.5..2 out on triangle 0x3B; then effect SmokeTrail_vtable on it */
/* 0x00342BC0 */
s32 Room92_Cmd03(void) {
    static const union { u32 u; f32 f; } kStep = {0x3FA0D97C}, kJit = {0x3F80ADFD}, kHalf = {0x3F00ADFD};
    VObject *list = gPlacedThings, *ev, *rnd;
    u8 *t;
    s32 n, k;
    f32 a;
    f32 one[4] __attribute__((aligned(16)));
    f32 rot[4] __attribute__((aligned(16)));
    f32 front[4] __attribute__((aligned(16)));
    f32 off[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));

    VCALL(list, 0x14, void (*)(VObject *, s32))(list, 10);
    t = VCALL(list, 0x8, u8 *(*)(VObject *, s32))(list, 10);
    if (t == NULL) {
        return 1;
    }
    VCALL((VObject *)t, 0xC, void (*)(VObject *))((VObject *)t);
    AT(t, 0x28, u8) = 1;
    ev = gEvents;
    n = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 1);
    one[0] = 1.0f;
    one[1] = 1.0f;
    one[2] = 1.0f;
    one[3] = 1.0f;
    rnd = gRandom;
    a = kJit.f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd) + kStep.f * (f32)n;
    a = Angle_Wrap(a - kHalf.f);
    rot[0] = 0.0f;
    rot[1] = 0.0f;
    rot[2] = a;
    Mtx_TurnY(m, a);
    off[0] = 0.0f;
    off[1] = 0x1.0ccccc0000000p+1f /* 2.1 */;
    off[2] = 1.5f + 0.5f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd);
    off[3] = 1.0f;
    sceVu0ApplyMatrix(front, m, off);
    Thing_Place(t, 0x3B, one, rot, front);
    k = n + 2;
    if (k >= 5) {
        k -= 5;
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, k);
    EffectMgr_Start(gEffects, Effect_New(gEffects, 0xE60, effect_79F30_init), t);
    return 1;
}

/* up to 6 things out (script variable 6 counts them): 1..3 more placed things of kind 9 (+0x8),
 * each tied to the first room object 10..15 flagged (+0 = 1) (+0x122 its index), set up (+0xC,
 * +0x28 on) and dropped at a random spot 40..50 out and 25..40 up in any direction that lands on
 * the nav mesh (+0x3C) */
/* 0x00342E80 */
s32 Room92_Cmd02(void) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    VObject *ev = gEvents, *rnd, *list, *objs, *nav;
    s32 n = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 6);
    s32 cnt, i;

    if (n >= 6) {
        return 1;
    }
    rnd = gRandom;
    cnt = (s32)(3.0f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd)) + 1;
    if (6 - n < cnt) {
        cnt = 6 - n;
    }
    list = gPlacedThings;
    objs = gRoomObjects;
    nav = (VObject *)gNavMesh;
    for (i = 0; i < cnt; i++) {
        u8 *t = VCALL(list, 0x8, u8 *(*)(VObject *, s32))(list, 9);
        s32 k, tri;
        f32 off[4] __attribute__((aligned(16)));
        f32 p[4] __attribute__((aligned(16)));
        f32 m[4][4] __attribute__((aligned(16)));

        if (t == NULL) {
            continue;
        }
        n++;
        for (k = 10; k < 16; k++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00437D48[k - 10]);

            if (o[0] == 1) {
                break;
            }
        }
        AT(t, 0x122, s16) = k - 10;
        VCALL((VObject *)t, 0xC, void (*)(VObject *))((VObject *)t);
        AT(t, 0x28, u8) = 1;
        do {
            f32 r, ang;

            r = VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd);
            off[0] = 0.0f;
            ang = kPi.f - 2.0f * (kPi.f * r);
            off[1] = 40.0f + 10.0f * VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd);
            r = VCALL(rnd, 0x20, f32 (*)(VObject *))(rnd);
            off[3] = 0.0f;
            off[2] = 25.0f + 15.0f * r;
            sceVu0UnitMatrix(m);
            sceVu0RotMatrixY(m, m, ang);
            sceVu0ApplyMatrix(p, m, off);
            tri = VCALL(nav, 0x3C, s32 (*)(VObject *, f32 *, s32))(nav, p, 0);
        } while (tri == -1);
        AT(t, 0x34, s32) = tri;
        sceVu0CopyVector((f32 *)(t + 0x10), p);
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 6, n);
    return 1;
}

/* 0x00343180 */
s32 Room92_Cmd01(void) {
    static const s16 spot[3] = {2, 7, 5};
    static const f32 b[3] = {0.0f, 0.0f, 0.5f}, c[3] = {0.0f, 0.0f, 0.0f}, d[3] = {0.5f, 0.5f, 0.5f};

    return grey_three(50.0f, spot, b, c, d);
}

/* room objects 4 / 5 spinning (+0x10) at a speed (+0x30) eased by script variable 4 / 5: byte 3 0
 * sets them up (speed and base 4 degrees, top 20); else each frame (unless the progress' +0x54
 * says no): state 0 slows by 5% of the base to 0, 1 speeds by 2% up to the base, 2 by 10% up to
 * the top, 3 jumps to the base */
/* 0x003434A0 */
s32 Room92_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k005 = {0x3D4CCCCD}, k002 = {0x3CA3D70A}, k01 = {0x3DCCCCCD},
                                          kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    VObject *objs = gRoomObjects, *ev;
    s32 k;

    if (cmd[3] == 0) {
        for (k = 4; k < 6; k++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00437D30[k - 4]);

            if (o != NULL) {
                AT(o, 0x30, u32) = 0x3D8EFA35;   /* 4 degrees */
                AT(o, 0x34, u32) = 0x3D8EFA35;
                AT(o, 0x38, u32) = 0x3EB2B8C2;   /* 20 degrees */
            }
        }
        return 1;
    }
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    ev = gEvents;
    for (k = 4; k < 6; k++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00437D30[k - 4]);
        s32 state = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, k == 4 ? 4 : 5);
        f32 v;

        if (o == NULL) {
            continue;
        }
        switch (state) {
        case 0:
            v = AT(o, 0x30, f32) - k005.f * AT(o, 0x34, f32);
            AT(o, 0x30, f32) = v;
            if (v < 0.0f) {
                AT(o, 0x30, f32) = 0.0f;
            }
            break;
        case 1:
            AT(o, 0x30, f32) = AT(o, 0x30, f32) + k002.f * AT(o, 0x34, f32);
            if (!(AT(o, 0x30, f32) <= AT(o, 0x34, f32))) {
                AT(o, 0x30, f32) = AT(o, 0x34, f32);
            }
            break;
        case 2:
            AT(o, 0x30, f32) = AT(o, 0x30, f32) + k01.f * AT(o, 0x34, f32);
            if (!(AT(o, 0x30, f32) <= AT(o, 0x38, f32))) {
                AT(o, 0x30, f32) = AT(o, 0x38, f32);
            }
            break;
        case 3:
            AT(o, 0x30, f32) = AT(o, 0x34, f32);
            break;
        }
        v = AT(o, 0x10, f32) + AT(o, 0x30, f32);
        AT(o, 0x10, f32) = v;
        if (!(v <= kPi.f)) {
            AT(o, 0x10, f32) = v - k2Pi.f;
        }
    }
    return 1;
}

/* destructor (vtable Room92Effect_vtable) */
/* 0x00370030 */
void *Room92Effect_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Room92Effect_vtable;
        AT(o, 0x0, void **) = EffectBase_vtable;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* an effect message: +0xC its word 0 (word 1 set: +0x8 too); a 0 there becomes 3 / 3 and +0x10 */
/* 0x00370090 */
void Room92Effect_SetParams(u8 *o, s32 *m) {
    if (m == NULL) {
        return;
    }
    AT(o, 0xC, f32) = AT(m, 0x0, f32);
    if (m[1] != 0) {
        AT(o, 0x8, f32) = AT(o, 0xC, f32);
    }
    if (AT(o, 0xC, f32) == 0.0f) {
        AT(o, 0xC, u32) = 0x40400000;   /* 3 */
        AT(o, 0x8, u32) = 0x40400000;
        AT(o, 0x10, u8) = 1;
    }
}

#ifdef HG_NATIVE

/* +0x14 draw (PC; the PS2 sends GS packets): unless done (+0x11), and while on (+0x10) or with
 * the world's origin in view, a heat haze over the screen - the screen halved, blended half
 * with itself in 32 wavering columns (column k moved down 16 sin(a) (+0x8 + (1 + cos a) / 2)
 * / 16, a = +0x4 + 90 degrees a column), over the screen by an alpha ramp (0x20 at the edges,
 * 0x60 in the middle) at half, in layer 0x2A */
/* 0x00370100 */
void Room92Effect_Draw(u8 *o) {
    VObject *cam = gCamera;

    if (AT(o, 0x11, u8) == 1) {
        return;
    }
    if (AT(o, 0x10, u8) == 0) {
        f32 m[4][4] __attribute__((aligned(16)));
        f32 p[4] __attribute__((aligned(16)));
        f32 c[4] __attribute__((aligned(16)));

        p[0] = 0.0f;
        p[1] = 0.0f;
        p[2] = 0.0f;
        p[3] = 1.0f;
        VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, m);
        sceVu0ApplyMatrix(c, m, p);
        if (!(c[0] <= c[3]) || c[0] < -c[3] || !(c[1] <= c[3]) || c[1] < -c[3] || !(c[2] <= c[3]) || c[2] < -c[3]) {
            return;
        }
    }
    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
    glr_haze2(AT(o, 0x4, f32), AT(o, 0x8, f32));
}

#endif

/* its update (Room92Effect_Start's): done (+0x11) stops it (0); else the angle (+0x4) turns on by
 * 50..70 degrees, and +0x8 eases toward +0xC by 0.1 */
/* 0x00370F00 */
s32 Room92Effect_Update(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTenth = {0x3DCCCCCD};
    f32 r;

    if (AT(o, 0x11, u8) == 1) {
        return 0;
    }
    r = VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom);
    AT(o, 0x4, f32) = AT(o, 0x4, f32) + kPi.f * (50.0f + 20.0f * r) / 180.0f;
    AT(o, 0x4, f32) = Angle_Wrap(AT(o, 0x4, f32));
    if (AT(o, 0x8, f32) != AT(o, 0xC, f32)) {
        if (AT(o, 0x8, f32) <= AT(o, 0xC, f32)) {
            AT(o, 0x8, f32) = AT(o, 0x8, f32) + kTenth.f;
            if (!(AT(o, 0x8, f32) <= AT(o, 0xC, f32))) {
                AT(o, 0x8, f32) = AT(o, 0xC, f32);
            }
        } else {
            AT(o, 0x8, f32) = AT(o, 0x8, f32) - kTenth.f;
            if (AT(o, 0x8, f32) < AT(o, 0xC, f32)) {
                AT(o, 0x8, f32) = AT(o, 0xC, f32);
            }
        }
    }
    return 1;
}

/* an effect set up: +0x4 a random angle (-pi..pi), +0x8 / +0xC 2, +0x10 on, +0x11 off */
/* 0x00371030 */
void Room92Effect_Start(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 r;

    AT(o, 0x11, u8) = 0;
    r = VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom);
    AT(o, 0x4, f32) = kPi.f * (360.0f * (r - 0.5f)) / 180.0f;
    AT(o, 0x8, u32) = 0x40000000;   /* 2 */
    AT(o, 0xC, u32) = 0x40000000;
    AT(o, 0x10, u8) = 1;
}
