#ifndef ROOMS_H
#define ROOMS_H

/* What the rooms share (rooms/common.c): their effects, objects, clocks and swinging props.
 * Each room's own hooks and tables are in rooms/room_XX.c (XX: the room number). The small
 * helpers the original inlines into each room's hooks are defined here. */
#include "common.h"
#include "globals.h"
#include "navmesh.h"
#include "game.h"
#include "actor.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "effectmgr.h"
#include "texcache.h"
#include "charaction.h"
#include "pursuer.h"

extern void func_00100490(void *p);   /* operator delete */
extern VObject *D_00456DF8;   /* the room's objects: +0x18 (id) the object */
extern f32 func_0031C248(f32 x);   /* sinf */
extern void func_00122C20(Actor *a, s32 id, s32 arg2, s32 arg3, s32 arg4, const f32 *pos);
extern void func_002D6170(u8 *mgr, s32 slot);
extern void *D_00472F60[];
extern s32 func_00266C70(u8 *fx, s32 n, void *arg);
extern void func_002670F0(u8 *fx, s32 n);   /* effect slot n gone */
extern const char *const D_00405618, *const D_0040561C;   /* "kibako" (the box), "a_koushi" (the grate) */
extern const char *const D_00403948, *const D_0040394C;   /* "left", "right" */
extern const char *const D_0040395C, *const D_00403960;   /* "movechair_1", "movechair_2" */
extern const char *const D_004070C0, *const D_004070C4, *const D_004070C8;   /* "sara_l", "sara_r", "tenbin" */
extern f32 func_0031C248(f32 x);   /* sinf */
extern const char *const D_00403964;   /* "movechair_3" */
/* the lattice ("kousi"): swung open (-90 degrees) while the hook's flag byte is set, shut otherwise */
extern const char *const D_003F99B8[];   /* { "kousi" } */
extern u32 D_0047E36C;   /* menu buttons pressed this frame (MENU_*) */
extern u32 D_0047E364;   /* menu buttons, repeating */
extern const char *const D_003FC680, *const D_0042A0E8, *const D_00400C38, *const D_0042C354;   /* "fan" (rooms 0x1A / 0x31 / 0x21 / 0x32) */
extern const char *const D_004022B0, *const D_004022B4, *const D_004022B8;   /* "jimen", "kama", "sumi" */
extern const char *const D_003F03FC;   /* "doramukan" (the drum can) */
extern const char *const D_0042C328;   /* "a_fragment0" */
extern const char *const D_00438D00;   /* room 0x6A's object */
extern const char *const D_00438700[];   /* "dial0".."dial2", then (D_0043870C) "dial3".."dial5" lit */
extern u32 D_0047E36C;                   /* menu buttons pressed (MENU_*) */
extern u32 D_0047E364;                   /* menu buttons repeating */
extern const char *const D_0040C160;   /* an object's name */
extern const char *const D_003FF124;     /* room 0x20's falling object */
extern const char *const D_0040187C;
extern const char *const D_003F0DBC, *const D_003F0DC0;   /* room 0x03 (and D_003F0DC4) */
extern const char *const D_003F17B4;   /* room 0x04 (and D_003F17B8 / D_003F17C8) */
extern const char *const D_003F17CC, *const D_003F17D0, *const D_003F17D4, *const D_003F17D8, *const D_003F17DC;
extern const char *const D_003F5440[];                  /* room 0x0C (+13: the three pairs) */
extern const char *const D_003F6F48, *const D_003F6F4C, *const D_003F6F50;   /* room 0x0F */
extern const char *const D_003FB370[];   /* room 0x18's objects */
extern const char *const D_0040C140[];   /* room 0x4F's objects */
extern const char *const D_0047ABEC;     /* room 0x5C's dial */
extern const char *const D_004123E8;     /* room 0x5D's lever */
extern const char *const D_004123D0[];   /* room 0x5D's (+2: four objects) */
extern const char *const D_0047AD08[];   /* room 0x62's objects */
extern const char *const D_00429130;
extern f32 func_0031C248(f32 x);   /* sinf */
/* ---- the slam shake: frame hooks of Lorenzo's (kind 0xA) rooms shake the camera (+0x6C, 0.5)
 * when his slam lands ---- */
/* ---- the countdown's start / stop hooks ---- */
extern void func_00136620(void *h);   /* Hewie restarted (hewie.c) */
extern void func_002A76E0(u8 *p);     /* four bytes cleared */


typedef struct GreyMsg {
    f32 a, b, c, d, e;
    u32 rgb;
    s16 spot;
} GreyMsg;

extern void *D_00479B00[];
extern void *D_00471060[];
extern void *D_00479AC0[];
extern void *D_0046FF20[];
extern void *D_00469D00[];
extern void *D_0046FC30[];
extern void *D_004737D0[];
extern void *func_002672F0(u32 size, void *place);
extern s32 func_001770D0(Progress *p, s32 kind);   /* the slot of character kind (0xFF) */
extern void *D_0047A3D0[];
extern void *D_00479A80[];

/* a room's class: its vtable, then the base's */
static inline void *room_dtor(void *o, s32 flags, void **own, void **base) {
    if (o != NULL) {
        AT(o, 0x0, void **) = own;
        if (o != NULL) {
            AT(o, 0x0, void **) = base;
        }
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* three hanging things (the room's +0x34 (0..2) objects) swinging, by the command's byte 3:
 * 0 at rest (push +0x3C 0.9); 1 Fiona's movement pushes them (the squared step) - past 1 one
 * swings for 20 frames (+0x38) and the first toggles event flag 3 with a sound (Fiona's 1 /
 * 2); 2 a swing step: +0x30 on by 36 degrees, its tilt +0x10 = (1 + sin) degrees in radians */
static inline __attribute__((always_inline)) s32 swing_three_by(VObject *self, u8 *cmd, s32 byIndex) {
    VObject *objs = D_00456DF8, *ev = gEvents;
    s32 i;

    for (i = 0; i < 3; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, s32))(objs, VCALL(self, 0x34, s32 (*)(VObject *, s32))(self, i));

        if (o == NULL) {
            continue;
        }
        switch (cmd[3]) {
        case 0:
            AT(o, 0x30, s32) = 0;
            AT(o, 0x38, s32) = 0;
            AT(o, 0x3C, f32) = 0x1.ccccccp-1f /* 0.9 */;
            break;
        case 1:
            if (AT(o, 0x38, f32) <= 8.0f) {
                Character *ch = byIndex ? gCharacters[cmd[4]] : gCharPlayer;
                f32 d[4] __attribute__((aligned(16)));

                if (ch == NULL) {
                    break;
                }
                sceVu0CopyVector(d, ch->a.prevPos);
                sceVu0SubVector(d, ch->a.pos, d);
                AT(o, 0x3C, f32) = AT(o, 0x3C, f32) + (d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
                if (!(AT(o, 0x3C, f32) <= 1.0f)) {
                    AT(o, 0x38, f32) = 20.0f;
                    AT(o, 0x3C, s32) = 0;
                    if (i == 0) {
                        if ((VCALL(ev, 0x58, u32 (*)(VObject *, s32))(ev, 3) & 0xFF) == 1) {
                            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 3);
                            func_00122C20(&ch->a, 1, 6, 0, 0, NULL);
                        } else {
                            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 3);
                            func_00122C20(&ch->a, 2, 6, 0, 0, NULL);
                        }
                    }
                }
            }
            break;
        case 2:
            if (!(AT(o, 0x38, f32) <= 0.0f)) {
                f32 t;

                AT(o, 0x38, f32) = AT(o, 0x38, f32) - 1.0f;
                if (AT(o, 0x38, f32) < 0.0f) {
                    AT(o, 0x38, s32) = 0;
                }
                AT(o, 0x30, f32) = AT(o, 0x30, f32) + 36.0f;
                if (!(AT(o, 0x30, f32) < 360.0f)) {
                    AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
                }
                t = 0x1.921fb6p+1f * (1.0f + func_0031C248(0x1.921fb6p+1f * AT(o, 0x30, f32) / 180.0f)) / 180.0f;
                AT(o, 0x10, f32) = t;
                if (!(t <= 0x1.921fb6p+1f)) {
                    AT(o, 0x10, f32) = t - 0x1.921fb6p+2f;
                }
            }
            break;
        }
    }
    return 1;
}

/* the room's effect n made anew as class vtbl (any old one dropped) */
static __attribute__((unused)) void room_effect_slot_new(u8 *fx, s32 n, void **vtbl) {
    VObject *pool = (VObject *)(fx + 0x1400);
    VObject **slot = &AT(fx, 0x1438 + n * 4, VObject *);
    void *mem;

    if (*slot != NULL) {
        VCALL(pool, 0x14, void (*)(VObject *, void *))(pool, *slot);
        *slot = NULL;
    }
    mem = VCALL(pool, 0x10, void *(*)(VObject *, u32))(pool, 0xA0);
    if (mem != NULL) {
        VObject *e = func_002672F0(0xA0, mem);

        if (e != NULL) {
            e->vtbl = vtbl;
        }
        *slot = e;
        VCALL(*slot, 0xC, void (*)(VObject *))(*slot);
    }
}

/* a lit quad (room effect n) with corners q[0..15] and colour value `c`, by byte 3: 1 removed,
 * 2 on, else off */
static inline __attribute__((always_inline)) s32 lit_quad_in(s32 n, u8 *cmd, const u32 *corners, u32 c) {
    u8 *fx;
    u32 q[20] __attribute__((aligned(16)));
    s32 i;

    if (cmd[3] == 1) {
        func_002670F0(gRoomEffects, n);
        return 1;
    }
    fx = gRoomEffects;
    room_effect_slot_new(fx, n, D_00472F60);
    for (i = 0; i < 16; i++) {
        q[i] = corners[i];
    }
    q[0x10] = cmd[3] == 2 ? 0x3F800000 : 0;
    q[0x11] = c;
    q[0x12] = 0;
    q[0x13] = c;
    func_00266C70(fx, n, q);
    return 1;
}

static __attribute__((unused)) f32 hook_sqrt(f32 x) {
    return __builtin_sqrtf(x);
}

/* two hanging things (the room's +0x34 (4, 5) objects, half a swing apart: +0x34 60 x i
 * degrees), by byte 3: 0 at rest; 1 Fiona's movement pushes them (her step's length) - past 5
 * they swing for 20 frames, and the first toggles event flag 3 with a sound (Fiona's 1 / 2) -
 * and a swing step (+0x30 on by 36 degrees, the tilt +0x10 (1 + sin) degrees in radians) */
static inline __attribute__((always_inline)) s32 hangers_swing(VObject *self, u8 *cmd, s32 base, s32 n, f32 push,
                                                              s32 root, s32 flag, s32 sndOn, s32 sndOff) {
    VObject *objs = D_00456DF8, *ev = gEvents;
    f32 d[4] __attribute__((aligned(16)));
    s32 i;

    for (i = 0; i < n; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, s32))(objs, VCALL(self, 0x34, s32 (*)(VObject *, s32))(self, i + base));

        if (o == NULL) {
            continue;
        }
        switch (cmd[3]) {
        case 0:
            AT(o, 0x30, f32) = 0.0f;
            AT(o, 0x34, f32) = 60.0f * (f32)i;
            AT(o, 0x38, f32) = 0.0f;
            AT(o, 0x3C, f32) = 0.0f;
            break;
        case 1:
            if (gCharPlayer != NULL) {
                sceVu0CopyVector(d, gCharPlayer->a.prevPos);
                sceVu0SubVector(d, gCharPlayer->a.pos, d);
                if (root) {
                    AT(o, 0x3C, f32) = AT(o, 0x3C, f32) + hook_sqrt(d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
                } else {
                    AT(o, 0x3C, f32) = AT(o, 0x3C, f32) + (d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
                }
                if (!(AT(o, 0x3C, f32) <= push)) {
                    AT(o, 0x38, f32) = 20.0f;
                    AT(o, 0x3C, f32) = 0.0f;
                    if (i == 0) {
                        if ((VCALL(ev, 0x58, u32 (*)(VObject *, s32))(ev, flag) & 0xFF) == 1) {
                            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, flag);
                            func_00122C20(&gCharPlayer->a, sndOn, 6, 0, 0, NULL);
                        } else {
                            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, flag);
                            func_00122C20(&gCharPlayer->a, sndOff, 6, 0, 0, NULL);
                        }
                    }
                }
            }
            if (!(AT(o, 0x38, f32) <= 0.0f)) {
                f32 t;

                AT(o, 0x38, f32) = AT(o, 0x38, f32) - 1.0f;
                if (AT(o, 0x38, f32) < 0.0f) {
                    AT(o, 0x38, f32) = 0.0f;
                }
                AT(o, 0x30, f32) = AT(o, 0x30, f32) + 36.0f;
                if (!(AT(o, 0x30, f32) + AT(o, 0x34, f32) < 360.0f)) {
                    AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
                }
                t = 0x1.921fb6p+1f * (1.0f + func_0031C248(0x1.921fb6p+1f * (AT(o, 0x30, f32) + AT(o, 0x34, f32)) / 180.0f)) / 180.0f;
                AT(o, 0x10, f32) = t;
                if (!(t <= 0x1.921fb6p+1f)) {
                    AT(o, 0x10, f32) = t - 0x1.921fb6p+2f;
                }
            }
            break;
        }
    }
    return 1;
}

/* (not with progress flag 0xAF) with flag `flag` set and item 0x238 held, a sound (0xC, 5) */
static inline s32 item238_sound(u32 bit) {
    Progress *p = gProgress;

    if (!(AT(p, 0x30, u32) & 0x8000) && (AT(p, 0xE4, u32) & bit) &&
        VCALL(gSubScreen, 0xC, s32 (*)(VObject *, s32))(gSubScreen, 0x238) != 0) {
        VCALL(gSound, 0x14, void (*)(VObject *, u32, u32))(gSound, 0xC, 5);
    }
    return 1;
}

/* the room object D_003F6F64's +0x24 toward 1 (event variable 0 unset) or 0 (set): byte 3 0 at
 * once, else by 0.2 a step */
static inline s32 var_fade(const char *name, s32 var, u8 *cmd) {
    static const union { u32 u; f32 f; } kStep = {0x3E4CCCCD};   /* 0.2 */
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);

    if (o == NULL) {
        return 1;
    }
    if (cmd[3] == 0) {
        if (VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, var) == 0) {
            AT(o, 0x24, f32) = 1.0f;
        } else {
            AT(o, 0x24, f32) = 0.0f;
        }
    } else if (VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, var) == 0) {
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + kStep.f;
        if (!(AT(o, 0x24, f32) <= 1.0f)) {
            AT(o, 0x24, f32) = 1.0f;
        }
    } else {
        AT(o, 0x24, f32) = AT(o, 0x24, f32) - kStep.f;
        if (AT(o, 0x24, f32) < 0.0f) {
            AT(o, 0x24, f32) = 0.0f;
        }
    }
    return 1;
}

/* room object `name`'s animation by event var 0 (12..): byte 3 0 forward (+0x74) to frame
 * (var - 12) / 18, 1 back (+0x78) to (var - 12) / 16, 2 / 3 back at 0 / 1; +0x7C kept 0..1 */
static inline __attribute__((always_inline)) s32 var0_anim(u8 *cmd, const char *name) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
    u32 v;

    if (o == NULL) {
        return 1;
    }
    v = VCALL(gEvents, 0x34, u32 (*)(VObject *, s32))(gEvents, 0);
    switch (cmd[3]) {
    case 0:
        if (v < 0xC) {
            v = 0xC;
        }
        if (v >= 0x1F) {
            v = 0x1E;
        }
        AT(o, 0x74, s32) = 1;
        AT(o, 0x78, s32) = 0;
        AT(o, 0x7C, f32) = (f32)(v - 0xC) / 18.0f;
        break;
    case 1:
        if (v < 0xC) {
            v = 0xC;
        }
        if (v >= 0x1D) {
            v = 0x1C;
        }
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(v - 0xC) / 16.0f;
        break;
    case 2:
    case 3:
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(cmd[3] - 2);
        break;
    }
    if (!(AT(o, 0x7C, f32) <= 1.0f)) {
        AT(o, 0x7C, f32) = 1.0f;
    }
    if (AT(o, 0x7C, f32) < 0.0f) {
        AT(o, 0x7C, f32) = 0.0f;
    }
    return 1;
}

/* a dial `o` on progress var `var` (0..6, 30 degrees each, from `off`): byte 3 of `step` 0 set
 * to it (`hide` also clears its +0), 1 turned by left / right (event +0x60 1 when changed, 0 when
 * confirmed / cancelled), 2 turning to it a degree a step (event +0x5C 1 there), 3 wait (2) */
static inline __attribute__((always_inline)) s32 dial_step(u32 step, u8 *o, u32 var, s32 off, s32 hide) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kDeg = {0x3C8EFA35};

    switch (step) {
    case 0:
        if (hide) {
            AT(o, 0x0, u8) = 0;
        }
        AT(o, 0x14, f32) = kPi.f * (f32)((s32)(Progress_GetVar(gProgress, var) & 0xFF) * 30 + off) / 180.0f;
        break;
    case 1:
        if (((D_0047E36C >> 4) & 1) | ((D_0047E36C >> 5) & 1)) {
            VCALL(gEvents, 0x60, void (*)(VObject *, s32))(gEvents, 0);
        } else {
            Progress *p = gProgress;
            u8 v = Progress_GetVar(p, var);

            if ((D_0047E364 >> 3) & 1) {
                if (v != 0) {
                    v = v - 1;
                }
            } else if (((D_0047E364 >> 1) & 1) && v < 6) {
                v = v + 1;
            }
            if (v != (u8)Progress_GetVar(p, var)) {
                AT(p, 0x9C + var, u8) = v;
                VCALL(gEvents, 0x60, void (*)(VObject *, s32))(gEvents, 1);
            }
        }
        break;
    case 2: {
        f32 deg = 180.0f * AT(o, 0x14, f32) / kPi.f;
        f32 d = deg - (f32)((s32)(Progress_GetVar(gProgress, var) & 0xFF) * 30 + off);

        if (!(d <= 1.0f)) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - kDeg.f;
        } else if (d < -1.0f) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) + kDeg.f;
        } else {
            VCALL(gEvents, 0x5C, void (*)(VObject *, s32))(gEvents, 1);
        }
        break;
    }
    case 3:
        return 2;
    }
    return 1;
}

/* frame (v - lo) / div (v kept in lo..hi) forward (fwd) or back; Fiona's sound 0 as it starts */
static inline __attribute__((always_inline)) void var0_frame(u8 *o, u32 v, u32 lo, u32 hi, f32 div, s32 fwd) {
    if (v == lo) {
        func_00122C20(&gCharPlayer->a, 0, 6, 0, 0, NULL);
    }
    if (v < lo) {
        v = lo;
    }
    if (v > hi) {
        v = hi;
    }
    AT(o, 0x74, s32) = fwd;
    AT(o, 0x78, s32) = !fwd;
    AT(o, 0x7C, f32) = (f32)(v - lo) / div;
}

/* the room object D_003FA078's animation by event var 0 (byte 3 picks the range: 0 back over
 * 43..54, 1 / 2 / 5 forward over 12..21, 16..28, 11..18; 3 / 4 back at 0 / 1), +0x7C kept 0..1 */
static inline s32 var0_obj_anim(const char *name, u8 *cmd) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
    u32 v;

    if (o == NULL) {
        return 1;
    }
    v = VCALL(gEvents, 0x34, u32 (*)(VObject *, s32))(gEvents, 0);
    switch (cmd[3]) {
    case 0:
        var0_frame(o, v, 0x2B, 0x36, 11.0f, 0);
        break;
    case 1:
        var0_frame(o, v, 0xC, 0x15, 9.0f, 1);
        break;
    case 2:
        var0_frame(o, v, 0x10, 0x1C, 12.0f, 1);
        break;
    case 3:
    case 4:
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(cmd[3] - 3);
        break;
    case 5:
        var0_frame(o, v, 0xB, 0x12, 7.0f, 1);
        break;
    }
    if (!(AT(o, 0x7C, f32) <= 1.0f)) {
        AT(o, 0x7C, f32) = 1.0f;
    }
    if (AT(o, 0x7C, f32) < 0.0f) {
        AT(o, 0x7C, f32) = 0.0f;
    }
    return 1;
}

/* the room's object `name` turns 1.15 degrees a frame */
static inline __attribute__((always_inline)) void fan_turn(const char *name) {
    static const union { u32 u; f32 f; } kStep = {0x3CA46C8A}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);

    if (o != NULL) {
        f32 a = AT(o, 0x14, f32) + kStep.f;

        AT(o, 0x14, f32) = a;
        if (!(a <= kPi.f)) {
            AT(o, 0x14, f32) = a - k2Pi.f;
        }
    }
}

static __attribute__((unused)) void glow4_init(void **obj) {
    obj[0] = D_004737D0;
    obj[0x40 / 4] = D_00469D00;
    ((s32 *)obj)[0x44 / 4] = -1;
    obj[0x40 / 4] = D_0046FC30;
}

/* byte 4 0: the effect D_004737D0 (grey, 0x18, size 30) at one of four spots by byte 3 (-332 /
 * -368, 100.8, -12 / 55 / 165 / 165), its slot in event var byte 3; else that effect removed */
static inline s32 glow4_spot(u8 *cmd, u32 first) {
    static const union { u32 u; f32 f; } kY = {0x42C9999A};   /* 100.8 */
    u8 k = cmd[3];

    if (cmd[4] == 0) {
        u8 *mgr = gEffects;
        s32 slot = Effect_New(mgr, 0x80, glow4_init);
        s32 p[8] __attribute__((aligned(16)));

        p[0] = 0x80;
        p[1] = 0x80;
        p[2] = 0x80;
        p[3] = 0x18;
        switch ((u32)(k - first)) {
        case 0:
            AT(&p[4], 0, f32) = -332.0f;
            AT(&p[5], 0, f32) = kY.f;
            AT(&p[6], 0, f32) = -12.0f;
            break;
        case 1:
            AT(&p[4], 0, f32) = -368.0f;
            AT(&p[5], 0, f32) = kY.f;
            AT(&p[6], 0, f32) = 55.0f;
            break;
        case 2:
            AT(&p[4], 0, f32) = -368.0f;
            AT(&p[5], 0, f32) = kY.f;
            AT(&p[6], 0, f32) = 165.0f;
            break;
        case 3:
            AT(&p[4], 0, f32) = -332.0f;
            AT(&p[5], 0, f32) = kY.f;
            AT(&p[6], 0, f32) = 165.0f;
            break;
        }
        AT(&p[7], 0, f32) = 30.0f;
        func_002D6090(mgr, slot, p);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, k, slot);
    } else {
        func_002D6170(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, k));
    }
    return 1;
}

static inline u8 *room_obj(const char *name) {
    return VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
}

/* an object's angle field `off` set, if it is there */
static inline void obj_angle(const char *name, u32 off, u32 bits) {
    u8 *o = room_obj(name);

    if (o != NULL) {
        AT(o, off, u32) = bits;
    }
}

static __attribute__((unused)) void effect_479ac0_init(void **obj) {
    obj[0] = D_00479AC0;
}

/* an effect of class D_00479AC0 (0x10 bytes) on object `o` with value `f` */
static inline void obj_effect(u8 *o, u32 f) {
    struct {
        u8 *o;
        u32 f;
    } prm;
    s32 slot = Effect_New(gEffects, 0x10, effect_479ac0_init);

    prm.f = f;
    prm.o = o;
    func_002D6090(gEffects, slot, &prm);
}

/* room effect 0x1F's colour pulsing with script variable `var` (0..0x60 round): three channels
 * a third of the way apart, each 8 x (distance from the middle - 16), then the rest */
static inline __attribute__((always_inline)) void colour_pulse(s32 var, u8 c3, u8 c4, u8 c5, u8 c6, u8 c7) {
    VObject *ev = gEvents;
    s32 v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, var);
    u8 c[9];
    s32 k;

    for (k = 0; k < 3; k++) {
        s32 d = 0x30 - (v + k * 0x20) % 0x60;

        if (d <= 0) {
            d = -d;
        }
        d = d < 0x11 ? 0 : d - 0x10;
        d <<= 3;
        c[k] = d < 0x100 ? d : 0xFF;
    }
    c[3] = c3;
    c[7] = c7;
    c[4] = c4;
    c[5] = c5;
    c[6] = c6;
    c[8] = 0;
    func_00266C70(gRoomEffects, 0x1F, c);
    v++;
    if ((u32)v > 0x60) {
        v = 0;
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, var, v);
}

/* script variable byte 3 down by the player's hit (1 from the weak blow 0x1A, else 5), not below 0 */
static inline s32 var_down_by_hit(u8 *cmd) {
    VObject *ev = gEvents;
    s32 v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, cmd[3]);
    s32 k = AT(gCharPlayer, 0xFC, s32) == 0x1A ? 1 : 5;

    if (k > 0) {
        v -= k;
        if (v < 0) {
            v = 0;
        }
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[3], v);
    }
    return 1;
}

/* ---- the countdown clock (progress +0xFC4 time up, +0xFC5 / +0xFC6 minutes / seconds) ----
 * Each room's frame hook draws it at (431, 395) through the event manager's text +0x78: "59:59"
 * (and the flag pinned to 1) while time is up, else "MM:SS" built in its own buffer. */
static inline s32 clock_draw(char *buf, const char *full) {
    u8 *p = (u8 *)gProgress;
    VObject *ev;

    if (p[0xFC4] > 0) {
        ev = gEvents;
        VCALL(ev, 0x78, void (*)(VObject *, s32, s32, s32, const char *, s32, s32, s32, s32))(
            ev, 0x1AF, 0x18B, 0, full, 0x80, 0x30, 0x10, 0x15);
        p[0xFC4] = 1;
        return 1;
    }
    buf[0] = p[0xFC5] / 10 + '0';
    buf[2] = ':';
    buf[1] = p[0xFC5] % 10 + '0';
    buf[3] = p[0xFC6] / 10 + '0';
    buf[5] = 0;
    buf[4] = p[0xFC6] % 10 + '0';
    ev = gEvents;
    VCALL(ev, 0x78, void (*)(VObject *, s32, s32, s32, const char *, s32, s32, s32, s32))(
        ev, 0x1AF, 0x18B, 0, buf, 0x80, 0x30, 0x10, 0x15);
    return 1;
}

static inline s32 slam_shake(void) {
    if (gCharPursuer == NULL || AT(gCharPursuer, 0x153C, u8) != 0xA) {
        return 1;
    }
    if (func_0030BB70((Pursuer *)gCharSlot2) != 0) {
        VCALL(gCamera, 0x6C, void (*)(VObject *, f32))(gCamera, 0.5f);
    }
    return 1;
}

/* the countdown on (+0x1FBEC1), Hewie restarted, the camera director +0x40 (14), the clock
 * (+0xFC4..) zeroed */
static inline s32 clock_start(void) {
    u8 *p = (u8 *)gProgress;

    p[0x1FBEC1] = 1;
    func_00136620(gCharPartner);
    VCALL(gCamDirector, 0x40, void (*)(VObject *, f32))(gCamDirector, 14.0f);
    func_002A76E0(p + 0xFC4);
    return 1;
}

/* the countdown off, the camera director +0x40 (-1) */
static inline s32 clock_stop(void) {
    AT(gProgress, 0x1FBEC1, u8) = 0;
    VCALL(gCamDirector, 0x40, void (*)(VObject *, f32))(gCamDirector, -1.0f);
    return 1;
}

/* the countdown clock (+0xFC4..+0xFC6) saved in script variables 0..2 */
static inline s32 clock_save(void) {
    u8 *p = (u8 *)gProgress;
    VObject *ev = gEvents;

    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, p[0xFC4]);
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, p[0xFC5]);
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 2, p[0xFC6]);
    return 1;
}

static __attribute__((unused)) void effect_79B00_init(void **obj) {
    obj[0] = D_00479B00;
}

static inline void grey_send(u8 *mgr, GreyMsg *m, s16 spot, f32 b, f32 c, f32 d) {
    m->b = b;
    m->spot = spot;
    m->c = c;
    m->d = d;
    m->e = d;
    func_002D6090(mgr, Effect_New(mgr, 0x1C30, effect_79B00_init), m);
}

static inline s32 grey_three(f32 a, const s16 *spot, const f32 *b, const f32 *c, const f32 *d) {
    u8 *mgr = gEffects;
    GreyMsg m;
    s32 i;

    m.a = a;
    m.rgb = 0x303030;
    for (i = 0; i < 3; i++) {
        grey_send(mgr, &m, spot[i], b[i], c[i], d[i]);
    }
    return 1;
}

/* the clock frozen: while event flag 1 (+0x58) is set, the time saved in script variables
 * (clock_save: 0 time up -> "59:59", 1 / 2 minutes / seconds) instead of the running one */
static inline s32 clock_draw_saved(char *buf, const char *full) {
    VObject *ev = gEvents;
    u32 t;

    if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 1) == 0) {
        return clock_draw(buf, full);
    }
    if (VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0) != 0) {
        VCALL(ev, 0x78, void (*)(VObject *, s32, s32, s32, const char *, s32, s32, s32, s32))(
            ev, 0x1AF, 0x18B, 0, full, 0x80, 0x30, 0x10, 0x15);
        return 1;
    }
    ev = gEvents;
    buf[0] = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 1) / 10 + '0';
    t = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 1);
    buf[2] = ':';
    buf[1] = t % 10 + '0';
    buf[3] = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 2) / 10 + '0';
    t = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 2);
    buf[5] = 0;
    buf[4] = t % 10 + '0';
    VCALL(ev, 0x78, void (*)(VObject *, s32, s32, s32, const char *, s32, s32, s32, s32))(
        ev, 0x1AF, 0x18B, 0, buf, 0x80, 0x30, 0x10, 0x15);
    return 1;
}

static __attribute__((unused)) s32 swing_three(VObject *self, u8 *cmd) {
    return swing_three_by(self, cmd, 0);
}

static __attribute__((unused)) void effect_C0_init(void **obj) {
    obj[0] = D_00479A80;
    obj[0x70 / 4] = D_00469D00;
    ((s32 *)obj)[0x74 / 4] = -1;
    obj[0x70 / 4] = D_0046FC30;
}

static __attribute__((unused)) void effect_10_init(void **obj) {
    obj[0] = D_0047A3D0;
}

/* byte 3 0: a 90-frame countdown starts; else while it runs the character kind 0xE moves by
 * (dx, 3) on +0x14 / +0x18 (2) */
static __attribute__((unused)) s32 room_nudge(s32 *count, u8 *cmd, f32 dx) {
    Character *c;

    if (cmd[3] == 0) {
        *count = 90;
        return 1;
    }
    if (--*count == 0) {
        return 1;
    }
    c = gCharacters[func_001770D0(gProgress, 0xE) & 0xFF];
    AT(c, 0x14, f32) = AT(c, 0x14, f32) + dx;
    AT(c, 0x18, f32) = AT(c, 0x18, f32) + 3.0f;
    return 2;
}

/* the usual one, room effect 0x1B */
static __attribute__((unused)) s32 lit_quad(u8 *cmd, const u32 *corners, u32 c) {
    return lit_quad_in(0x1B, cmd, corners, c);
}

static __attribute__((unused)) f32 shaft_rnd(VObject *rnd) {
    return VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
}

/* a lit quad at x 0.49 .. 9.5, z 69.5 .. 60.5, from the floor to 22 (rooms 0x21 and 0x32) */
static const u32 sQuadWindow[16] = {
    0x3EFB2FEC, 0x41B00000, 0x428B0113, 0x3F800000, 0x4118089A, 0x41B00000, 0x4271F660, 0x3F800000,
    0x3EFB2FEC, 0x00000000, 0x428B0113, 0x3F800000, 0x4118089A, 0x00000000, 0x4271F660, 0x3F800000,
};

/* a lit quad (room effect 0x1A) at x 40.5 .. 49.5, z -10.5 .. -19.5, from 1.5 to 30 (rooms 0x21
 * and 0x32) */
static const u32 sQuadDoor[16] = {
    0x4221F660, 0x41F00000, 0xC127F766, 0x3F800000, 0x42460227, 0x41F00000, 0xC19C1340, 0x3F800000,
    0x4221F660, 0x3FC00000, 0xC127F766, 0x3F800000, 0x42460227, 0x3FC00000, 0xC19C1340, 0x3F800000,
};

static __attribute__((unused)) void dust_cloud_init(void **obj) {
    obj[0] = D_0046FF20;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

static __attribute__((unused)) void effect_471060_init(void **obj) {
    obj[0] = D_00471060;
}

/* the room object by name, out of line (keeps a2 untouched for difftest) */
static __attribute__((noinline, unused)) u8 *obj_named(const char *name) {
    return VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
}

#endif /* ROOMS_H */
