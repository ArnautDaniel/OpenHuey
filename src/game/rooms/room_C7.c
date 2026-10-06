/* Room 0xC7: its event handler class (vtable D_004760E0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "input.h"
#include "ptmf.h"
#include "memcard.h"
#include "actor.h"
#include "progress.h"
#include "pursuer.h"
#include "scene_game_members.h"
#include "sce/libvu0.h"

extern void *D_0046DB80[];
extern void *D_004760E0[];
extern u8 D_0047ADF8[];
extern void *D_0047A3F0[];
extern void *D_004799D0[];
extern void *D_004798B0[];
extern f32 D_0047E3A0[4];   /* left stick */
extern u8 D_0042F500[];
extern u8 D_0042F590[];
extern u8 D_0042F5D0[];
extern u8 D_0042F7B0[];
extern u8 D_0042F860[];
extern u8 D_0042F9C0[];
extern void *D_00430700[];
extern void *D_004307F0[];

extern void *D_0046F580[];

extern PTMF D_01991660[];
extern PTMF D_019916C0[];

static void effect_7A3F0_init(void **obj) {
    obj[0] = D_0047A3F0;
}

static void effect_799d0_init(void **obj) {
    obj[0] = D_004799D0;
}

static void effect_798B0_init(void **obj) {
    obj[0] = D_004798B0;
}

#ifdef HG_NATIVE

#include "gl2d.h"

#endif

/* 0x00339CE0 */
void *RoomC7_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_004760E0, D_0046DB80); }

/* 0x00339D40 */
void *RoomC7_EnterScript(void) {
    return D_0042F500;
}

/* 0x00339D50 */
void *RoomC7_CharEnterScript(void) {
    return D_0042F590;
}

/* 0x00339D60 */
void *RoomC7_Phase1Script(void) {
    return D_0042F5D0;
}

/* 0x00339D70 */
void *RoomC7_Phase2Script(void) {
    return D_0042F7B0;
}

/* 0x00339D80 */
void *RoomC7_Phase3Script(void) {
    return D_0042F860;
}

/* 0x00339D90 */
void *RoomC7_Phase5Script(void) {
    return D_0042F9C0;
}

/* 0x00339DA0 */
void *RoomC7_ActionScript(void *self, s32 i) {
    return D_00430700[i];
}

/* 0x00339DC0 */
void *RoomC7_Table38(void *o) { return D_0047ADF8; }   /* D_004760E0 +0x38 */

/* 0x00339DD0 */
void *RoomC7_ObjectName(void *self, s32 i) {
    return D_004307F0[i];
}

/* (self->*D_019916C0[i])(a, b) */
/* 0x00339DF0 */
s32 RoomC7_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019916C0[i & 0xFF], a, b);
}

/* a room callback: the pursuer's func_0029A710 */
/* 0x00339E20 */
s32 RoomC7_Cond01(void) { return func_0029A710((Pursuer *)gCharPursuer); }

/* script variables 7 / 8 (the player's spot) in 151..269 / 171..219 */
/* 0x00339E30 */
s32 RoomC7_Cond00(void) {
    VObject *ev = gEvents;
    u16 x = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 7);
    u16 z = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 8);

    return x >= 0x97 && x < 0x10E && z >= 0xAB && z < 0xDC;
}

/* (self->*D_01991660[i])(a, b) */
/* 0x00339ED0 */
s32 RoomC7_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991660[i & 0xFF], a, b);
}

/* (as Room48_Cmd02) byte 3 0 starts the effect D_0047A3F0 (its slot in event variable 9);
   else that one is ended (EffectMgr_Remove) */
/* 0x00339F00 */
s32 RoomC7_Cmd06(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(gEffects, 0x10, effect_7A3F0_init);

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 9, slot);
    } else {
        EffectMgr_Remove(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 9));
    }
    return 1;
}

/* (as Room23_Cmd00)  the kind-0xB character's model +0xCC8: 0 (byte 3 1) or -0.02 */
/* 0x0033A040 */
s32 RoomC7_Cmd05(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)Progress_SlotOfId(gProgress, 0xB)]->motion;

    if (cmd[3] == 1) {
        AT(m, 0xCC8, s32) = 0;
    } else {
        AT(m, 0xCC8, u32) = 0xBCA3D70A;   /* -0.02 */
    }
    return 1;
}

/* (as Room2A_Cmd03) the 0xD40-byte effect D_004799D0 started with byte 3 */
/* 0x0033A0B0 */
s32 RoomC7_Cmd04(void *self, void *a1, u8 *cmd) {
    u8 *mgr = gEffects;
    u8 b = cmd[3];

    EffectMgr_Start(mgr, Effect_New(mgr, 0xD40, effect_799d0_init), &b);
    return 1;
}

/* (as RoomC0_Cmd01)  byte 3 2 up from frame 1268 (2.79 a frame, light 0x23) and 3 from frame 25
 * (4.27, light 0xF), held at 0x80; 1 the fade fully on with light 0xA, else off with light 0x11
 * (+0x64) */
/* 0x0033A1C0 */
s32 RoomC7_Cmd03(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k2 = {0x40328F5C}, k3 = {0x4088A3D7};
    u32 t = VCALL(gCutscene, 0x34, s32 (*)(VObject *))(gCutscene);
    u32 a, s = 0;

    switch (cmd[3]) {
    case 2:
    case 3:
        a = cmd[3] == 2 ? (u32)(k2.f * (f32)(t - 1268)) : (u32)(k3.f * (f32)(t - 25));
        if (a > 0x80) {
            a = 0x80;
        }
        AT(gCharSlot2, 0xE4, u8) = 0;
        VCALL(gRenderer, 0x70, void (*)(VObject *, u32))(gRenderer, a << 24 | 0x808080);
        Character_Set152C(gCharSlot2, cmd[3] == 2 ? 0x23 : 0xF);
        return 1;
    case 1:
        s = 0xFF;
        break;
    }
    if (s >= 0xFF) {
        Character_Set152C(gCharSlot2, 0xA);
        s = 0xFF;
    } else {
        Character_Set152C(gCharSlot2, 0x11);
    }
    VCALL(gRenderer, 0x64, void (*)(VObject *, u32, s32))(gRenderer, s << 24 | 0x808080, 0);
    return 1;
}

/* a cursor effect (D_004798B0) at (x, y) kept in script variables 7 / 8, its slot in 6: byte 3 0
 * puts it at (246, 242); 1 moves it 4 a frame by the stick or the d-pad (x 0..492, y 0..420),
 * waiting (2) until confirm (event 4 +0x5C) or cancel (+0x60); 2 ends it */
/* 0x0033A470 */
s32 RoomC7_Cmd02(void *self, void *a1, u8 *cmd) {
    VObject *ev;
    s32 slot;
    f32 v[4] __attribute__((aligned(16)));
    f32 a;
    u16 pos[2];

    switch (cmd[3]) {
    case 0:
        slot = Effect_New(gEffects, 0xC, effect_798B0_init);
        ev = gEvents;
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 6, slot);
        pos[0] = 0xF6;
        pos[1] = 0xF2;
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 7, pos[0]);
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 8, pos[1]);
        EffectMgr_Start(gEffects, slot, pos);
        return 1;
    case 1:
        break;
    case 2:
        slot = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 6);
        EffectMgr_Start(gEffects, slot, NULL);
        return 1;
    default:
        return 1;
    }
    ev = gEvents;
    slot = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 6);
    pos[0] = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 7);
    pos[1] = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 8);
    sceVu0CopyVector(v, D_0047E3A0);
    v[0] += (f32)(((D_0047E374 & PAD_RIGHT) != 0) - ((D_0047E374 & PAD_LEFT) != 0));
    v[2] += (f32)(((D_0047E374 & PAD_DOWN) != 0) - ((D_0047E374 & PAD_UP) != 0));
    a = v[0];
    if (a <= 0.0f) {
        a = -a;
    }
    if (!(a <= 0.5f)) {
        if (v[0] < 0.0f) {
            pos[0] = pos[0] < 4 ? 0 : pos[0] - 4;
        } else {
            pos[0] += 4;
            if (pos[0] >= 0x1ED) {
                pos[0] = 0x1EC;
            }
        }
    }
    a = v[2];
    if (a <= 0.0f) {
        a = -a;
    }
    if (!(a <= 0.5f)) {
        if (v[2] < 0.0f) {
            pos[1] = pos[1] < 4 ? 0 : pos[1] - 4;
        } else {
            pos[1] += 4;
            if (pos[1] >= 0x1A5) {
                pos[1] = 0x1A4;
            }
        }
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 7, pos[0]);
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 8, pos[1]);
    EffectMgr_Start(gEffects, slot, pos);
    if (D_0047E36C & MENU_CONFIRM) {
        VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 4);
        return 1;
    }
    if (D_0047E36C & MENU_CANCEL) {
        VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 4);
        return 1;
    }
    return 2;
}

/* (as Room0C_Cmd01, the player only) */
/* 0x0033A8E0 */
s32 RoomC7_Cmd01(void *self, void *a1, u8 *cmd) {
    return var_down_by_hit(cmd);
}

/* the screen fade (renderer +0x70) by script variable 1 with Hewie's light 0xF: byte 3 0 clear
 * (0x808080), 2 full (0x80808080); 1 fades in by 0x10 a call and 3 back out, waiting (2), Hewie's
 * +0xE4 set once there */
/* 0x0033A980 */
s32 RoomC7_Cmd00(void *self, void *a1, u8 *cmd) {
    VObject *ev;
    u32 c;

    switch (cmd[3]) {
    case 0:
        AT(gCharPartner, 0xE4, u8) = 0;
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, u32))(gEvents, 1, 0x808080);
        Character_Set152C(gCharPartner, 0xF);
        return 1;
    case 1:
        ev = gEvents;
        c = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 1);
        VCALL(gRenderer, 0x70, void (*)(VObject *, u32))(gRenderer, c);
        if (c != 0x80808080) {
            AT(gCharPartner, 0xE4, u8) = 0;
            VCALL(ev, 0x30, void (*)(VObject *, s32, u32))(ev, 1, c + 0x10000000);
            Character_Set152C(gCharPartner, 0xF);
            return 2;
        }
        AT(gCharPartner, 0xE4, u8) = 1;
        return 1;
    case 2:
        AT(gCharPartner, 0xE4, u8) = 0;
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, u32))(gEvents, 1, 0x80808080);
        Character_Set152C(gCharPartner, 0xF);
        return 1;
    case 3:
        ev = gEvents;
        c = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 1);
        VCALL(gRenderer, 0x70, void (*)(VObject *, u32))(gRenderer, c);
        if (c != 0x808080) {
            AT(gCharPartner, 0xE4, u8) = 0;
            VCALL(ev, 0x30, void (*)(VObject *, s32, u32))(ev, 1, c - 0x10000000);
            Character_Set152C(gCharPartner, 0xF);
            return 2;
        }
        AT(gCharPartner, 0xE4, u8) = 1;
        return 1;
    }
    return 1;
}

/* destructor (vtable D_004798B0) */
/* 0x0035CE40 */
void *RoomC7Cursor_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004798B0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* (+0x18) set: { u16, u16 } into +0x6 / +0x8 and on (+0x4 0); none: off (+0x4 1) */
/* 0x0035CEA0 */
void RoomC7Cursor_SetParams(u8 *o, u16 *prm) {
    if (prm == NULL) {
        AT(o, 0x4, u8) = 1;
        return;
    }
    AT(o, 0x4, u8) = 0;
    AT(o, 0x6, u16) = prm[0];
    AT(o, 0x8, u16) = prm[1];
}

#ifdef HG_NATIVE

/* the cursor's +0x14 draw (+0x4 1: off): texels 176..256 x 0..112 of texture 0xC shown 20 x 28
 * at its point (+0x6, +0x8), opaque */
/* 0x0035CEE0 */
void RoomC7Cursor_Draw(u8 *o) {
    u8 *tex;
    s32 x, y;

    if (AT(o, 0x4, u8) == 1) {
        return;
    }
    if (TexCache_Resident(0xC, 0, 0x30, &tex) == -1) {
        return;
    }
    x = AT(o, 0x6, u16);
    y = AT(o, 0x8, u16);
    gl2d_sprite(0x30, x, y, x + 20, y + 28, tex, 176, 0, 256, 112, 0x80808080, 0, gl2d_blend(0x8000000064ull));
}

#endif

/* (+0x10) still on */
/* 0x0035D180 */
s32 RoomC7Cursor_Update(u8 *o) {
    return AT(o, 0x4, u8) != 1;
}

/* 0x0035D190 */
void RoomC7Cursor_Start(u8 *o) {
    AT(o, 0x4, u8) = 0;
}

/* destructor (vtable D_004799D0) */
/* 0x0035D7E0 */
void *Debris_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004799D0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* 0x0035F0F0 */
void Debris_Start(void) {
}

/* destructor (vtable D_0047A3F0) */
/* 0x00377FF0 */
void *BackdropModel_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0047A3F0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}
