/* Room 0x66: its event handler class (vtable D_00470F50, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "model.h"
#include "progress.h"
#include "scene_game_members.h"
#include "stalker_math.h"

extern void *D_0046DB80[];
extern void *D_00470F50[];
extern u8 D_0047ACC8[], D_0047ACE0[];
extern void *D_00477AC0[];
extern void *D_00478B50[];
/* ---- room 66 (D_0041F558): footsteps in the mud - a character callback ---- */
extern u8 D_0041E110[];
extern u8 D_0041E260[];
extern u8 D_0041E360[];
extern u8 D_0041E700[];
extern void *D_0041F4E0[];
extern u8 D_0041F5C0[];
extern PTMF D_01991030[];
extern PTMF D_01991070[];

static void effect_77AC0_init(void **obj) {
    obj[0] = D_00477AC0;
    obj[0x1840 / 4] = D_00469D00;
    ((s32 *)obj)[0x1844 / 4] = -1;
    obj[0x1840 / 4] = D_0046FC30;
    obj[0x1878 / 4] = D_00469D00;
    ((s32 *)obj)[0x187C / 4] = -1;
    obj[0x1878 / 4] = D_0046FC30;
}

static void fire_init(void **obj) {
    obj[0] = D_00478B50;
    obj[0x1810 / 4] = D_00469D00;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = D_0046FC30;
}

/* two brown puffs (D_0046FF20 dust, colour 0x46 / 0x34 / 0x29, alpha 0x20) at the foot, the
 * second 4 behind */
static inline void mud_puff(u8 *mgr, Character *c, f32 *foot, s32 k) {
    s32 slot = Effect_New(mgr, 0x720, dust_cloud_init);
    f32 t[4] __attribute__((aligned(16)));
    struct {
        f32 pos[4];
        s32 kind, a, b, c, d;
    } prm __attribute__((aligned(16)));

    if (k == 1) {
        foot[2] -= 4.0f;
    }
    Vec_TurnY(t, foot, AT(c, 0x54, f32));
    prm.pos[0] = AT(c, 0x10, f32) + t[0];
    prm.pos[1] = AT(c, 0x14, f32) + t[1];
    prm.pos[3] = 1.0f;
    prm.a = 0x46;
    prm.b = 0x34;
    prm.c = 0x29;
    prm.d = 0x20;
    prm.pos[2] = AT(c, 0x18, f32) + t[2];
    prm.kind = 2;
    EffectMgr_Start(mgr, slot, &prm);
}

/* 0x003001B0 */
void *Room66_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00470F50, D_0046DB80); }

/* 0x00300210 */
void *Room66_EnterScript(void) {
    return D_0041E110;
}

/* 0x00300220 */
void *Room66_CharEnterScript(void) {
    return D_0041E260;
}

/* 0x00300230 */
void *Room66_Phase1Script(void) {
    return D_0041E360;
}

/* 0x00300240 */
void *Room66_Phase2Script(void) {
    return D_0041E700;
}

/* 0x00300250 */
void *Room66_Phase5Script(void *o) { return D_0047ACC8; }   /* D_00470F50 +0x20 */

/* 0x00300260 */
void *Room66_ActionScript(void *self, s32 i) {
    return D_0041F4E0[i];
}

/* 0x00300280 */
void *Room66_Table38(void) {
    return D_0041F5C0;
}

/* 0x00300290 */
u32 Room66_ObjectName(void *o, s32 i) { return ((u32 *)D_0047ACE0)[i]; }   /* D_00470F50 +0x34 */

/* (self->*D_01991070[i])(a, b) */
/* 0x003002B0 */
s32 Room66_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991070[i & 0xFF], a, b);
}

/* room 0x66 (D_0041F5B0): the character's x (byte 3 0) or z is at least be32 bytes 4..7 / 1000 */
/* 0x003002E0 */
s32 Room66_CharPast(void *self, u8 *chr, u8 *cmd) {
    f32 v = (f32)(s32)((u32)cmd[4] << 24 | (u32)cmd[5] << 16 | (u32)cmd[6] << 8 | cmd[7]) / 1000.0f;

    if (cmd[3] == 0) {
        return !(AT(chr, 0x10, f32) < v);
    }
    return !(AT(chr, 0x18, f32) < v);
}

/* room 0x66 (D_0041F5A0): in the eight letters of script variables 0 and 1, from the place in
 * variable 2: the next 'L' / 'R' - matched by byte 3 (0 'L', else 'R') is stepped over (1) */
/* 0x003003C0 */
s32 Room66_Letters(void *self, void *a1, u8 *cmd) {
    VObject *ev = gEvents;
    s32 w[2];
    u8 *b = (u8 *)w;
    u32 i;
    s32 r = 0;

    w[0] = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);
    w[1] = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 1);
    i = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 2);
    for (; i < 8; i++) {
        if (b[i] == 'L' || b[i] == 'R') {
            break;
        }
    }
    if (i < 8) {
        if (cmd[3] == 0) {
            if (b[i] == 'L') {
                i++;
                r = 1;
            }
        } else if (b[i] == 'R') {
            i++;
            r = 1;
        }
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 2, i);
    return r;
}

/* (self->*D_01991030[i])(a, b) */
/* 0x003004F0 */
s32 Room66_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991030[i & 0xFF], a, b);
}

/* room 0x66 (D_0041F590): room effect 0x1F's colour pulsing by script variable 9 */
/* 0x00300520 */
s32 Room66_ColourPulse(void) {
    colour_pulse(9, 0x5A, 0x44, 0x46, 0x52, 0x4B);
    return 1;
}

/* room 0x66 (D_0041F580): byte 4 0 lights a fire (D_00478B50, kind byte 3), its slot in script
 * variable 8; else that fire put out (-1) */
/* 0x00300650 */
s32 Room66_Fire(void *self, void *a1, u8 *cmd) {
    if (cmd[4] == 0) {
        s32 slot = Effect_New(gEffects, 0x1C60, fire_init);
        s32 prm[4] = {0, 0, 0, 0};   /* (zeroed past the two words, as the original's stack) */

        prm[0] = cmd[3];
        prm[1] = 1;
        EffectMgr_Start(gEffects, slot, prm);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 8, slot);
    } else {
        s32 off[4] = {-1, 0, 0, 0};

        EffectMgr_Start(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 8), off);
    }
    return 1;
}

/* room 0x66 (D_0041F570): character 8 sinks 0.1 a frame: a fire (kind byte 3) as it starts
 * (slot in variable 7), put out below -24; done (1) below -25, else wait (2) */
/* 0x003007E0 */
s32 Room66_Sink(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[(u8)Progress_SlotOfId(gProgress, 8)];
    f32 y = c->a.pos[1] - 0x1.99999a0000000p-4f /* 0.1 */;

    c->a.pos[1] = y;
    if (y <= -0x1.99999a0000000p-3f /* 0.2 */) {
        if (y < -24.0f) {
            VObject *ev = gEvents;
            s32 s = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 7);

            if (s >= 0) {
                s32 off[4] = {-1, 0, 0, 0};

                EffectMgr_Start(gEffects, s, off);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 7, -1);
            }
        }
    } else {
        s32 slot = Effect_New(gEffects, 0x1C60, fire_init);
        s32 prm[4] = {0, 0, 0, 0};   /* (zeroed past the two words, as the original's stack) */

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 7, slot);
        prm[0] = cmd[3];
        prm[1] = 0;
        EffectMgr_Start(gEffects, slot, prm);
    }
    return c->a.pos[1] < -25.0f ? 1 : 2;
}

/* room 66 (D_0041F568): byte 4 0 starts the 0x1BC0-byte effect D_00477AC0 (parameters from
 * byte 3), its slot kept in event variable byte 3 + 3; else that effect is sent 0xFF (stop) */
/* 0x00300A20 */
s32 Room66_Effect(void *self, void *a1, u8 *cmd) {
    if (cmd[4] == 0) {
        u8 *mgr = gEffects;
        s32 slot = Effect_New(mgr, 0x1BC0, effect_77AC0_init);

        EffectMgr_Start(mgr, slot, cmd + 3);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, (cmd[3] + 3) & 0xFF, slot);
    } else {
        s32 slot = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, (cmd[3] + 3) & 0xFF);
        u8 stop = 0xFF;

        EffectMgr_Start(gEffects, slot, &stop);
    }
    return 1;
}

/* byte 3 0: the character's +0xE1 / +0xF4 cleared; 1 / 2 / else Character_RootMoveMasked / Character_RootMove /
 * Character_RootTurn, then: a foot (model +0x64) coming down while she stands (model +0x550 not above
 * 0) steps (voice 4 on motion 0x201, else 3) and, each foot, puffs mud; event flags 5 / 6 keep
 * which feet are down */
/* 0x00300BC0 */
s32 Room66_CharHook(void *self, Character *c, u8 *cmd) {
    VObject *m, *ev;
    u8 *mgr;
    u8 l, r, wasL, wasR, fl, fr;
    f32 posL[4] __attribute__((aligned(16)));
    f32 posR[4] __attribute__((aligned(16)));
    s32 k;

    switch (cmd[3]) {
    case 0:
        AT(c, 0xE1, u8) = 0;
        AT(c, 0xF4, s32) = 0;
        break;
    case 1:
        Character_RootMoveMasked(c);
        break;
    case 2:
        Character_RootMove(c);
        break;
    default:
        Character_RootTurn(c);
        break;
    }
    if (cmd[3] == 0) {
        return 1;
    }
    m = c->motion;
    l = VCALL(m, 0x64, s32 (*)(VObject *, s32, s32))(m, 0, 0);
    m = c->motion;
    r = VCALL(m, 0x64, s32 (*)(VObject *, s32, s32))(m, 1, 0);
    ev = gEvents;
    wasL = VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 5);
    wasR = VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 6);
    if (AT(c->motion, 0x550, f32) <= 0.0f && ((!wasL && l) || (!wasR && r))) {
        Actor_PlaySound(&c->a, AT(c->motion, 0x55C, s32) == 0x201 ? 4 : 3, 6, 0, 0, NULL);
    }
    fl = Motion_FootPos(c->motion, posL, 0, 0.0f, 1.0f);
    fr = Motion_FootPos(c->motion, posR, 1, 0.0f, 1.0f);
    mgr = gEffects;
    for (k = 0; k < 2; k++) {
        if (AT(c->motion, 0x550, f32) <= 0.0f && !wasL && fl) {
            mud_puff(mgr, c, posL, k);
        }
        if (AT(c->motion, 0x550, f32) <= 0.0f && !wasR && fr) {
            mud_puff(mgr, c, posR, k);
        }
    }
    if (l) {
        VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 5);
    } else {
        VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 5);
    }
    if (r) {
        VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 6);
    } else {
        VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 6);
    }
    return 1;
}
