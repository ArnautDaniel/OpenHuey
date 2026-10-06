/* The rooms' event handler classes (the 4-byte objects of the events object, +0x120: one per
 * room, see sRooms in event.c; base D_0046DB80): their destructors and small table getters.
 * Each room's class gives the event system the room's data tables (by slot, see the vtables). */
#include "common.h"
#include "globals.h"
#include "navmesh.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "memcard.h"
#include "pursuer.h"
#include "fiona.h"
#include "hewie.h"
#include "model.h"
#include "overlay.h"
#include "scene_game_members.h"
#include "snd_place.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif
#include "msl.h"

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

#define B5_PI 0x1.921fb60000000p+1f /* 3.14159274 */ /* 0x40490FDB */

void DriftingFlecks_SetParams(u8 *self, f32 *src);

#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

extern void *D_0046F580[];
extern void *D_00471060[];
extern void *D_00479AC0[];
extern void *D_00479B00[];
extern void *D_0047A3D0[];
void *DriftingFlecks_dtor(u8 *o, s32 flags);
void *WindowFlash_dtor(u8 *o, s32 flags);
void *Effect79B00_dtor(u8 *o, s32 flags);
void *Effect7A3D0_dtor(u8 *o, s32 flags);

void WindowFlash_SetParams(u8 *o, s32 *prm);
s32 WindowFlash_Update(u8 *o);
void WindowFlash_Start(u8 *o);

void Effect79B00_Start(void);

/* Tail call of the sub-object's virtual +0x14 with the value at +0x73EDC0. */
/* 0x0016CD30 */
void Progress_SpeechCall(u8 *p, s32 a1, s32 a2, s32 a3) {
    void *obj = p + 0x6FBF00;

    VCALL(obj, 0x14, void (*)(void *, s32, s32, s32))(obj, FLD(p, 0x73EDC0, s32), a2, a3);
}
/* 0x002097E0 */
s32 RoomBase_Phase5Script(void *o) {   /* +0x20 */
    return 0;
}

/* 0x002097F0 */
s32 RoomBase_Phase4Script(void *o) {   /* +0x1C */
    return 0;
}

/* 0x00209820 */
s32 RoomBase_Phase1Script(void *o) {   /* +0x10 */
    return 0;
}

/* 0x0020BCE0 */
s32 RoomBase_Table38(void *o) {   /* +0x38 */
    return 0;
}

/* 0x002A8930 */
s32 RoomBase_ObjectName(void *o) {   /* +0x34 */
    return 0;
}

/* 0x002A8940 */
s32 RoomBase_Condition(void *o) {   /* +0x2C */
    return 0;
}

/* 0x002A8950 */
s32 RoomBase_Command(void *o) {   /* +0x28 */
    return 0;
}

/* 0x002A8960 */
s32 RoomBase_ActionScript(void *o) {   /* +0x24 */
    return 0;
}

/* ---- the rooms' script hooks (pointers to members the event commands call) ---- */

#include "game.h"
#include "actor.h"
#include "progress.h"
#include "sce/libvu0.h"

extern VObject *D_00456DF8;   /* the room's objects: +0x18 (id) the object */

/* ---- more hooks: effects spawned into the effect manager, Fiona nudged, a partner's line ---- */

#include "effectmgr.h"

extern void *D_00479A80[], *D_0047A3D0[], *D_0047A730[];

/* an empty hook */
/* 0x002FCB30 */
void Room_EmptyHook(void) {
}

/* destructor (vtable D_00471060) */
/* 0x00306290 */
void *DriftingFlecks_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00471060;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* 0x003062F0 */
void DriftingFlecks_SetParams(u8 *self, f32 *src) {
    if (src != NULL && src[0] == 0.0f) {
        F32(self, 0x10) = src[1];
        F32(self, 0x14) = src[2];
        F32(self, 0x18) = src[3];
        F32(self, 0x1C) = 1.0f;
        F32(self, 0x30) = src[4];
        F32(self, 0x34) = src[5];
        F32(self, 0x20) = (B5_PI * src[6]) / 180.0f;
        F32(self, 0x24) = (B5_PI * src[7]) / 180.0f;
        F32(self, 0x28) = (B5_PI * src[8]) / 180.0f;
        F32(self, 0x2C) = 0.0f;
    }
}

#include "effectmgr.h"

extern const char *const D_00405618, *const D_0040561C;   /* "kibako" (the box), "a_koushi" (the grate) */

extern const char *const D_00403948, *const D_0040394C;   /* "left", "right" */
extern const char *const D_0040395C, *const D_00403960;   /* "movechair_1", "movechair_2" */

extern const char *const D_004070C0, *const D_004070C4, *const D_004070C8;   /* "sara_l", "sara_r", "tenbin" */

/* ---- room 0x61's light shaft, class D_0047A370 (0x700 bytes): a beam of light (a scrolling
 * texture on a strip between six points, drawn twice) with 16 dust motes rising through it
 * (double-buffered quad records +0x10 + buffer +0x6EC * 0x300, drawn by the quad drawer at
 * +0x610), each with its rise (+0x660 + i * 4) and wobble angle (+0x6A0); +0x6E8 the
 * texture's scroll (0 .. 4096, 1/16 texels), +0x650 where the motes start, +0x6F0 stopped,
 * +0x6F1 the haze on: the whole screen wavers (phases +0x6E0 / +0x6E4) ---- */

#include "texcache.h"

/* ---- class D_0047A390 (0x1A60 bytes), the haze of effect 0x1A60 (Room49_Cmd02): as the light
 * shaft's motes and haze without the beam - 64 motes (records +0x10 + 0xC00 x the current one
 * +0x1A54, the quad drawer at +0x1810) from (-60, 0, -60 + 0.4 x the frames counted at
 * +0x1A50), rising (+0x1848) and wobbling (+0x1948); the haze always on (phases +0x1A48 /
 * +0x1A4C), over the screen at 0x60; stopped by +0x1A58 ---- */

#ifdef HG_NATIVE

#endif

/* ---- room 0x61's wanderers: characters 0x14 / 0x15 / 0x16 each follow a path (0x40 bytes:
 * +0x0 / +0xC the box it stays in, +0x18 points, +0x1C frames between two, +0x20 the frame,
 * +0x24 the point it left, +0x28 the points (s16 x 3 in 1/16), +0x30 where it was) - points one
 * to a cell of a grid of 10-unit cells, at random in the cell, shuffled; a cubic curve from
 * each to the next ---- */

/* ---- class D_0047A3B0 (4 bytes): a glint on character 0x14 (its bone 6) ---- */

extern const char *const D_00403964;   /* "movechair_3" */

/* the lattice ("kousi"): swung open (-90 degrees) while the hook's flag byte is set, shut otherwise */
extern const char *const D_003F99B8[];   /* { "kousi" } */

extern void *D_0046F5A0[], *D_00469D00[], *D_0046FC30[];

static inline void smoke_init(void **o) {
    o[0] = D_0046F5A0;
    o[0x1810 / 4] = D_00469D00;
    ((s32 *)o)[0x1814 / 4] = -1;
    o[0x1810 / 4] = D_0046FC30;
}

/* 0x002AFE90 */
s32 Room24_Smoke(void) {   /* the rising smoke (D_0046F5A0, 0x1C60 bytes) */
    Effect_New(gEffects, 0x1C60, smoke_init);
    return 1;
}

extern void *D_0046FF20[];

/* room 54 step (D_00428040): find the nav mesh's door regions again */
/* 0x0030FA60 */
s32 Room54_DoorRegions(void) {
    VObject *nav = (VObject *)gNavMesh;

    VCALL(nav, 0x4C, void (*)(VObject *))(nav);
    return 1;
}

extern const char *const D_003FC680, *const D_0042A0E8, *const D_00400C38, *const D_0042C354;   /* "fan" (rooms 0x1A / 0x31 / 0x21 / 0x32) */

extern const char *const D_004022B0, *const D_004022B4, *const D_004022B8;   /* "jimen", "kama", "sumi" */

/* room 0x24 (D_004022C8): the kiln's ground, kiln and charcoal glow - byte 3 0 sets them up
 * (+0x74 0, +0x78 1, glow +0x7C 0, phase +0x30 -pi), else the glow pulses (0.5 + cos(phase) /
 * 2, the phase on by 12 degrees) */
/* 0x002AFF80 */
s32 Room24_KilnGlow(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kMinusPi = {0xC0490FDB}, kStep = {0x3E567750}, kPi = {0x40490FDB},
        k2Pi = {0x40C90FDB};
    const char *names[3];
    s32 i;

    names[0] = D_004022B0;
    names[1] = D_004022B4;
    names[2] = D_004022B8;
    if (cmd[3] == 0) {
        VObject *objs = D_00456DF8;

        for (i = 0; i < 3; i++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, names[i]);

            if (o != NULL) {
                AT(o, 0x74, s32) = 0;
                AT(o, 0x78, s32) = 1;
                AT(o, 0x7C, s32) = 0;
                AT(o, 0x30, f32) = kMinusPi.f;
            }
        }
    } else {
        VObject *objs = D_00456DF8;

        for (i = 0; i < 3; i++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, names[i]);
            f32 a;

            if (o == NULL) {
                continue;
            }
            AT(o, 0x7C, f32) = 0.5f + 0.5f * func_0031C058(AT(o, 0x30, f32));
            a = AT(o, 0x30, f32) + kStep.f;
            AT(o, 0x30, f32) = a;
            if (!(a <= kPi.f)) {
                AT(o, 0x30, f32) = a - k2Pi.f;
            }
        }
    }
    return 1;
}

extern const char *const D_003F03FC;   /* "doramukan" (the drum can) */

extern const char *const D_0042C328;   /* "a_fragment0" */

extern const char *const D_00438D00;   /* room 0x6A's object */

extern const char *const D_00438700[];   /* "dial0".."dial2", then (D_0043870C) "dial3".."dial5" lit */

extern void *D_004737D0[];

extern const char *const D_0040C160;   /* an object's name */

/* ---- rooms 0x20 / 0x21 / 0x23 ---- */

extern const char *const D_003FF124;     /* room 0x20's falling object */
extern const char *const D_0040187C;

/* ---- rooms 0x02 .. 0x12 ---- */

extern const char *const D_003F0DBC, *const D_003F0DC0;   /* room 0x03 (and D_003F0DC4) */
extern const char *const D_003F17B4;   /* room 0x04 (and D_003F17B8 / D_003F17C8) */
extern const char *const D_003F17CC, *const D_003F17D0, *const D_003F17D4, *const D_003F17D8, *const D_003F17DC;
extern const char *const D_003F5440[];                  /* room 0x0C (+13: the three pairs) */
extern const char *const D_003F6F48, *const D_003F6F4C, *const D_003F6F50;   /* room 0x0F */
extern void *D_00479AC0[], *D_00471060[], *D_00470E20[];

/* the effect D_00479AC0 (0x10 bytes; +0x4 its frame 0..4, 5 done, +0x8 a turn, +0xC the object):
 * a flash in the shape of a window (half of its outline, D_00444980, mirrored for points 12..23)
 * at the object, turned with it, each frame a fan through some of the outline (D_00444A70: a
 * count, then the points) */
extern const s8 *D_00444A70[];
extern const f32 D_00444980[12][3];
extern void *D_0046D7A0[], *D_00469D00[];
#ifdef HG_NATIVE

/* +0x14 draw: when all of the shape is in view, the screen brightened (Bloom_Start,
 * 0x80808080 in layer 0x28) and the shape added in white into layer 0x26 (the bloom's mask;
 * the original's triangle fan, additive, no depth writes). (Checked against the original with
 * its GS packet rebuilt, 2026-10-06.) */
/* 0x00360BF0 */
void WindowFlash_Draw(u8 *o) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD};   /* 0.1 */
    f32 pt[24][4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    const s8 *shape;
    u8 *obj = AT(o, 0xC, u8 *);
    u8 drawer[0x20] __attribute__((aligned(16)));
    VObject *cam;
    s32 n, k;

    if (AT(o, 0x4, s32) == 5) {
        return;
    }
    shape = D_00444A70[AT(o, 0x4, s32)];
    n = shape[0];
    func_002E3190(m, func_002E2D00(AT(obj, 0x14, f32) + AT(o, 0x8, f32)));
    for (k = 0; k < n; k++) {
        s32 id = shape[k + 1];

        if (id < 12) {
            v[0] = D_00444980[id][0];
            v[1] = D_00444980[id][1];
            v[2] = k01.f + D_00444980[id][2];
        } else {
            v[0] = -D_00444980[23 - id][0];
            v[1] = D_00444980[23 - id][1];
            v[2] = k01.f + D_00444980[23 - id][2];
        }
        func_002E2DA0(v, m, v);
        sceVu0AddVector(pt[k], (f32 *)(obj + 0x20), v);
        pt[k][3] = 1.0f;
    }
    cam = gCamera;
    VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, m);
    for (k = 0; k < n; k++) {
        sceVu0ApplyMatrix(v, m, pt[k]);
        if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
            return;
        }
    }
    AT(drawer, 0x0, void **) = D_0046D7A0;
    AT(drawer, 0x4, s32) = -1;
    Bloom_Start(drawer, 0x80808080, 0x28, 0);
    {
        f32 tri[3][4];
        f32 st[3][2] = {{0}};
        u8 col[3][4];

        for (k = 0; k < 3; k++) {
            col[k][0] = col[k][1] = col[k][2] = col[k][3] = 0x80;
        }
        glr_layer(0x26);
        for (k = 1; k + 1 < n; k++) {   /* the fan */
            sceVu0CopyVector(tri[0], pt[0]);
            sceVu0CopyVector(tri[1], pt[k]);
            sceVu0CopyVector(tri[2], pt[k + 1]);
            AT(&tri[0][3], 0, u32) = AT(&tri[1][3], 0, u32) = AT(&tri[2][3], 0, u32) = 0;
            glr_strip(&m[0][0], 3, &tri[0][0], &st[0][0], &col[0][0], NULL, 0,
                      0x40 | 0x10000 | 0x20000);   /* blended, GLR_PRIM_ADD, GLR_PRIM_NOZW */
        }
        glr_layer(-1);
    }
    AT(drawer, 0x0, void **) = D_00469D00;
}
#endif

/* (+0x10) counts +0x4 up to 5; 0 then */
/* 0x00361220 */
s32 WindowFlash_Update(u8 *o) {
    if (AT(o, 0x4, s32) == 5) {
        return 0;
    }
    AT(o, 0x4, s32)++;
    return 1;
}

/* 0x00361250 */
void WindowFlash_Start(u8 *o) {
    AT(o, 0x4, s32) = -1;
}

/* destructor (vtable D_00479B00) */
/* 0x00361940 */
void *Effect79B00_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479B00;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* 0x003634E0 */
void Effect79B00_Start(void) {
}

/* destructor (vtable D_0047A3D0) */
/* 0x00377CC0 */
void *Effect7A3D0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0047A3D0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* ---- rooms 0x15 .. 0x66 (second batch) ---- */

extern const char *const D_003FB370[];   /* room 0x18's objects */
extern const char *const D_0040C140[];   /* room 0x4F's objects */
extern const char *const D_0047ABEC;     /* room 0x5C's dial */
extern const char *const D_004123E8;     /* room 0x5D's lever */
extern const char *const D_004123D0[];   /* room 0x5D's (+2: four objects) */
extern VObject *D_00456E00;

/* (D_004022E0's table) room effect 0x1F's colour pulsing by script variable 0 */
/* 0x002AFD60 */
s32 Room24_ColourPulse(void) {
    colour_pulse(0, 0x50, 0x24, 0x2A, 0x2A, 0x46);
    return 1;
}

/* ---- room 0x66's fires, room 0x62's swinging object, a room creature class ---- */

extern const char *const D_0047AD08[];   /* room 0x62's objects */

/* ---- creature class D_00471290 (a pursuer-like character) ---- */

/* ---- rooms 0x48 / 0x60 ---- */

/* ---- two more room effects (as Room107_Cmd01 / Room106_Cmd00) ---- */

extern const char *const D_00429130;

#ifdef HG_NATIVE

#endif

#ifdef HG_NATIVE

#endif

#include "charaction.h"

/* ---- three 0x1C30-byte effects D_00479B00 (grey 0x303030) at once, each told its spot ---- */
extern void *D_00479B00[];

#include "input.h"

#ifdef HG_NATIVE
#include "gl2d.h"

#endif

/* ---- room 54 (D_00428030): the sliding blocks - group byte 3 of four room objects
 * (D_00428050, four names a group) pushed to their stops (D_00428010: x, z a group) ---- */
extern const char *D_00428050[];
extern const f32 D_00428010[];
extern VObject *D_00456E00;   /* the room's triangle groups */

/* a grey puff (dust, 0x50 grey, alpha 0x10) at (x, y, z) */
static inline void block_dust(u8 *mgr, s32 slot, f32 x, f32 y, f32 z) {
    struct {
        f32 pos[4];
        s32 kind, a, b, c, d;
    } prm __attribute__((aligned(16)));

    prm.pos[0] = x;
    prm.pos[1] = y;
    prm.pos[3] = 1.0f;
    prm.d = 0x10;
    prm.kind = 1;
    prm.c = 0x50;
    prm.pos[2] = z;
    prm.b = 0x50;
    prm.a = 0x50;
    func_002D6090(mgr, slot, &prm);
}

/* byte 4: 0 the group at its stop; 1 / 2 a step (0.25) along x / z toward it (event 0 cleared,
 * +0x60, when one gets there) and half the time a puff by the third; 3 / 4 the grinding sound
 * (0xC) on / off at the first; 5 the placed things on the group's triangles lifted (+0x28) */
/* 0x0030FA90 */
s32 Room54_GroupStep(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kQ = {0x3E800000};   /* 0.25 */
    VObject *ev, *rnd;
    u32 g = cmd[3];
    u8 *o, *mgr;
    s32 k, slot;
    f32 v, a, b;

    switch (cmd[4]) {
    case 0:
        for (k = 0; k < 4; k++) {
            o = obj_named(D_00428050[g * 4 + k]);
            AT(o, 0x20, f32) = D_00428010[g * 2];
            AT(o, 0x28, f32) = D_00428010[g * 2 + 1];
        }
        break;
    case 1:
    case 2:
        ev = gEvents;
        for (k = 0; k < 4; k++) {
            u32 at = cmd[4] == 1 ? 0x20 : 0x28;
            f32 stop = D_00428010[g * 2 + (cmd[4] == 1 ? 0 : 1)];

            o = obj_named(D_00428050[g * 4 + k]);
            v = AT(o, at, f32) - kQ.f;
            AT(o, at, f32) = v;
            if (v < stop) {
                AT(o, at, f32) = stop;
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0);
            }
        }
        rnd = gRandom;
        if (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 1) {
            break;
        }
        o = obj_named(D_00428050[g * 4 + 2]);
        if (cmd[4] == 1) {
            mgr = gEffects;
            slot = Effect_New(mgr, 0x720, dust_cloud_init);
            v = 6.0f + AT(o, 0x20, f32);
            block_dust(mgr, slot, v, AT(o, 0x24, f32), AT(o, 0x28, f32) + 25.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f));
        } else {
            if (g == 0) {
                a = 30.0f;
                b = 6.0f;
            } else {
                a = 12.0f;
                b = 17.0f;
            }
            mgr = gEffects;
            slot = Effect_New(mgr, 0x720, dust_cloud_init);
            v = AT(o, 0x20, f32) + a * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
            block_dust(mgr, slot, v, AT(o, 0x24, f32), AT(o, 0x28, f32) + b);
        }
        break;
    case 3:
    case 4: {
        f32 pos[4] __attribute__((aligned(16)));

        o = obj_named(D_00428050[g * 4]);
        sceVu0CopyVector(pos, (f32 *)(o + 0x20));
        Sound_PlayBankAt(gSound, cmd[4] == 3 ? 0xC : 0x8000000C, 6, pos, 0, 0);
        break;
    }
    case 5: {
        VObject *list = gPlacedThings, *rm = D_00456E00;

        for (k = 0; k < 0x80; k++) {
            u8 *t = VCALL(list, 0xC, u8 *(*)(VObject *, s32))(list, k);

            if (t != NULL && AT(t, 0x28, u8) == 1
                && VCALL(rm, 0x14, s32 (*)(VObject *, u32, s32))(rm, AT(t, 0x34, u32), g) != 0) {
                AT(t, 0x28, u8) = 0;
            }
        }
        break;
    }
    }
    return 1;
}

/* 0x0032D270 */
void Kind26_MoveTo(u8 *self, s32 a, f32 x, f32 y) {
    S32(self, 0x1624) = 0;
    S32(self, 0x1628) = 1;
    S32(self, 0x162C) = 60;
    F32(self, 0x1634) = F32(self, 0x10);
    F32(self, 0x1638) = F32(self, 0x18);
    F32(self, 0x163C) = x;
    F32(self, 0x1640) = y;
    self[0x16A8] = 1;
    self[0x16A9] = 1;
    S32(self, 0x1630) = a;
}

/* destructor (vtable D_00479AC0) */
/* 0x00360B60 */
void *WindowFlash_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479AC0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* (+0x18) set: { +0xC, +0x8 (f32) }, +0x4 -1 */
/* 0x00360BC0 */
void WindowFlash_SetParams(u8 *o, s32 *prm) {
    if (prm != NULL) {
        AT(o, 0xC, s32) = prm[0];
        AT(o, 0x8, s32) = prm[1];
        AT(o, 0x4, s32) = -1;
    }
}
