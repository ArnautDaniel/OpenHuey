/* Objects the room scripts spawn into the effect manager (SceneGame +0xF6E200 slots). */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "ptmf.h"
#include "progress.h"
#include "pursuer.h"
#include "memcard.h"
#include "effects.h"
#include "hewie.h"
#include "loading.h"
#include "props.h"
#include "pursuer_ai.h"
#include "scene_game_members.h"
#include "skeleton.h"
#include "snd_place.h"
#include "stalker_math.h"
#include "stalker_progress.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif
#include "msl.h"

extern void *D_00474000[], *D_0046FC30[], *D_00469D00[], *D_0046F580[];

extern void *D_0046D810[], *D_0046C220[], *D_00469C60[], *D_00469C20[];
extern const char *const D_0042C358;
extern void *D_00476C10[];
extern const PTMF D_00430A90;
extern void *D_00477AE0[];
extern void *D_00477E30[];
extern u8 D_00430A10[];
extern u8 D_00430A50[];
void *Kind32_ModelFiles(void);
void *Kind32_MotionFiles(void);

extern u32 D_0043B6E0[];
extern u32 D_0043CDC0[];
void Room66Effect_Start(u8 *p);
void *Kind34_MotionFiles(void);
void Kind34_DoorOffset(void *self, s32 i, f32 *out);
void Kind34_ActionOffsets(void *self, s32 i, f32 *out);
s32 Room55Effect_Update(f32 *a);
void *Kind35_MotionFiles(void);

s32 func_00345FF0(void);
s32 func_003479D0(void);

void Kind34_FilesLoaded(Pursuer *p);

extern void *D_00478B70[];
void *func_00347640(u8 *o, s32 flags);

s32 Room4EEffect_Update(f32 *a);

/* an effect's quad drawer (at `drawer`) given the current one of its records (`size` apart from
 * +0x10, the index at `idx`), and drawn */
static inline __attribute__((always_inline)) void quad_step(u8 *o, u32 drawer, u32 idx, u32 size) {
    AT(o, drawer + 0x10, u8 *) = o + AT(o, idx, s32) * size + 0x10;
    func_002E56C0(o + drawer);
}

void DustShaft_Draw(u8 *o);
void CeilingDrips_Draw(u8 *o);
void Embers_Start(u8 *o);

/* destructor: own vtable -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character; the model freed for
 * slots 3..5 */
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt) {
    if (c != NULL) {
        c->a.vtbl = vt;
        c->a.vtbl = D_0046D810;
        VCALL(c, 0x10, void (*)(Character *))(c);
        if ((u32)c->a.slot >= 3 && (u32)c->a.slot < 6) {
            void **m = c->motion;

            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                c->motion = NULL;
            }
        }
        c->a.vtbl = D_0046C220;
        VCALL(c, 0x10, void (*)(Character *))(c);
        c->a.vtbl = D_00469C60;
        c->a.vtbl = D_00469C20;
        if ((s16)flags > 0) {
            func_00124E40(&c->a);
        }
    }
    return c;
}

/* in play: func_00124890(-1) */
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p) {
    if (func_00217510(p) != 0) {
        func_00124890(&p->c.a, -1);
    }
}

/* the action 5 taken (+0x14E8): in play +0x8C, the state st, +0x114 1; the action cleared */
static inline __attribute__((always_inline)) void creature_act5(Pursuer *p, const PTMF *st) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((u8)func_00217510(p) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), st);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* its slot's progress entry (func_00177870) 1: SlotCmd_Cancel; -1 */
static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p) {
    Progress *g = gProgress;

    if ((u8)func_00177870(g, *(u8 *)&p->c.a.slot) == 1) {
        SlotCmd_Cancel(g, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

Character *Kind32_dtor(Character *c, s32 flags);
void Kind32_ShowUp(Pursuer *p);
void Kind32_EventState(Pursuer *p);
s32 Kind32_GrabOrder(Pursuer *p);
Character *Kind34_dtor(Character *c, s32 flags);
Character *Kind35_dtor(Character *c, s32 flags);

/* (class D_00474000, room 0x2A) +0x8 destructor (the quad drawer at +0x610 inlined) */
/* 0x00321910 */
u8 *Room2AWisps_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00474000;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* (class D_00474000, room 0x2A) +0xC reset: a random delay (0x5A..0x79) and its settings */
/* 0x00322430 */
void Room2AWisps_Start(u8 *o) {
    AT(o, 0x8EC, s32) = (VCALL(gRandom, 0x10, u32 (*)(VObject *))(gRandom) & 0x1F) + 0x5A;
    AT(o, 0x8F0, s32) = 0;
    AT(o, 0x8F4, s32) = 0;
    AT(o, 0x8F8, u8) = 0;
    AT(o, 0x618, s64) = -1;
    AT(o, 0x628, s32) = 0;
    AT(o, 0x62C, s32) = 0;
    AT(o, 0x630, s32) = 0x19;
    AT(o, 0x634, s16) = 1;
    AT(o, 0x636, s16) = 0;
    AT(o, 0x638, s16) = 0xE0;
    AT(o, 0x63A, s16) = 0x20;
    AT(o, 0x63C, s16) = 0x20;
    AT(o, 0x63E, s16) = 0x200;
    AT(o, 0x640, s16) = 0x100;
    AT(o, 0x642, u8) = 2;
    AT(o, 0x643, u8) = 0x10;
    AT(o, 0x644, u8) = 1;
    AT(o, 0x645, u8) = 0x10;
    AT(o, 0x646, u8) = 0xA;
}

/* placement new: its memory */
void *func_00322570(u32 size, void *mem) {
    return mem;
}

extern void func_003219A0(u8 *o, s32 i);

typedef union {
    u32 u;
    f32 f;
} F32Bits;

/* +0x18 start: at its fixed spot by the room's (211.28, 2.125, -220.567), 16 particles spread
 * up to 100 to one side */
/* 0x00321D20 */
void Room2AWisps_SetParams(u8 *o, f32 *params) {
    static const F32Bits kX = {0x435347AE}, kZ = {0xC35C9127}, kSpeed = {0xBDCCCCCD};
    VObject *rng;
    s32 i;

    if (params == NULL) {
        return;
    }
    sceVu0CopyVector((f32 *)(o + 0x650), params);
    AT(o, 0x650, f32) = kX.f;
    AT(o, 0x654, f32) = 2.125f;
    AT(o, 0x658, f32) = kZ.f;
    AT(o, 0x65C, f32) = 1.0f;
    rng = gRandom;
    for (i = 0; i < 16; i++) {
        u8 *e;
        f32 r;

        func_003219A0(o, i);
        e = o + AT(o, 0x8F4, s32) * 0x300 + i * 0x30 + 0x10;
        r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
        AT(e, 0x10, f32) = (AT(o, 0x650, f32) - 20.0f) + 100.0f * r;
    }
    AT(o, 0x8E0, f32) = 0.0f;
    AT(o, 0x8E4, f32) = 0.0f;
    AT(o, 0x8E8, f32) = kSpeed.f;
}

/* particle i of the current set (+0x10 + set * 0x300, 0x30 each): grey, scattered around the
 * spot, with random spin (+0x820 + i * 12), angles (+0x660 + i * 16) and sizes (+0x760) */
void func_003219A0(u8 *o, s32 i) {
    static const F32Bits k02 = {0x3E4CCCCD}, k005 = {0x3D4CCCCD}, k001 = {0x3C23D70A},
                         kM005 = {0xBD4CCCCD}, kPi = {0x40490FDB};
    VObject *rng = gRandom;
    u8 *p = o + AT(o, 0x8F4, s32) * 0x300 + i * 0x30 + 0x10;
    s32 k;

    AT(p, 0x0, s32) = 0x80;
    AT(p, 0x4, s32) = 0x80;
    AT(p, 0x8, s32) = 0x80;
    AT(p, 0xC, s32) = 0x80;
#define RND() VCALL(rng, 0x18, f32 (*)(VObject *))(rng)
    AT(p, 0x10, f32) = (AT(o, 0x650, f32) - 10.0f) + 10.0f * (RND() - 0.5f);
    AT(p, 0x14, f32) = AT(o, 0x654, f32) + RND();
    AT(p, 0x18, f32) = (20.0f + AT(o, 0x658, f32)) + 150.0f * RND();
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x20, f32) = 1.0f;
    AT(p, 0x24, f32) = 1.0f;
    AT(p, 0x28, f32) = 0.0f;
    AT(p, 0x2C, u32) = VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0xF;
    AT(o, 0x820 + i * 12, f32) = k02.f + k005.f * RND();
    AT(o, 0x824 + i * 12, f32) = k001.f * RND();
    AT(o, 0x828 + i * 12, f32) = kM005.f * RND();
    for (k = 0; k < 3; k++) {
        AT(o, 0x660 + i * 16 + k * 4, f32) = kPi.f * (180.0f * RND()) / 180.0f;
    }
    for (k = 0; k < 3; k++) {
        AT(o, 0x760 + i * 12 + k * 4, f32) = 10.0f + 22.5f * (RND() - 0.5f);
    }
#undef RND
}

/* (class D_00478BC0) +0xC reset: three random angles in -pi..pi */
/* 0x00350A10 */
void Effect78BC0_Start(u8 *o) {
    static const F32Bits kPi = {0x40490FDB};
    VObject *rng = gRandom;
    s32 k;

    for (k = 0; k < 3; k++) {
        f32 r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);

        AT(o, 0x4 + k * 4, f32) = kPi.f * (360.0f * (r - 0.5f)) / 180.0f;
    }
}

/* ---- class D_00478BC0 (room 0x2A, 0x14 bytes): a model turning on three axes; +0x4/+0x8/+0xC
 * the angles, +0x10 the model (0x2C or 0x30) ---- */

extern void *D_00478BC0[], *D_0046F580[], *D_00478B70[], *D_00469D00[];

/* +0x8 destructor */
/* 0x003507B0 */
u8 *Effect78BC0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00478BC0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* +0x10 update: turn */
/* the three angles turn (+0x4 by stepX, +0x8 by stepYZ, +0xC back by stepYZ; wrapped) */
static inline __attribute__((always_inline)) s32 spin_step(u8 *o, u32 stepX, u32 stepYZ) {
    static const F32Bits kPi = {0x40490FDB}, kMinusPi = {0xC0490FDB}, k2Pi = {0x40C90FDB};
    F32Bits sx, syz;

    sx.u = stepX;
    syz.u = stepYZ;
    AT(o, 0x4, f32) = AT(o, 0x4, f32) + sx.f;
    if (!(AT(o, 0x4, f32) <= kPi.f)) {
        AT(o, 0x4, f32) = AT(o, 0x4, f32) - k2Pi.f;
    }
    AT(o, 0x8, f32) = AT(o, 0x8, f32) + syz.f;
    if (!(AT(o, 0x8, f32) <= kPi.f)) {
        AT(o, 0x8, f32) = AT(o, 0x8, f32) - k2Pi.f;
    }
    AT(o, 0xC, f32) = AT(o, 0xC, f32) - syz.f;
    if (AT(o, 0xC, f32) < kMinusPi.f) {
        AT(o, 0xC, f32) = AT(o, 0xC, f32) + k2Pi.f;
    }
    return 1;
}

/* 0x00350910 */
s32 Effect78BC0_Update(u8 *o) {
    return spin_step(o, 0x3C0EFA35, 0x3AE4C389);
}

/* a model to draw this frame: position, ..., angles, model, colour. Drawn by D_00478B70's
 * func_0034E9E0 these are a light caustic: n the texture, radius the patch's size, angle[0]
 * the ripple phase, angle[1] / angle[2] its two layers' turns, model the alpha threshold of
 * its glow */
typedef struct ModelDrawParams {
    f32 pos[4];
    s32 n;
    f32 radius;
    f32 angle[3];
    s32 model;
    u32 rgba;
} ModelDrawParams;

/* the temporary draw object func_00350660 fills from the parameters (vtable D_00478B70) */
typedef struct ModelDraw {
    void **vtbl;
    s32 slot;
    u8 pad8[8];
    ModelDrawParams p;
} ModelDraw;

void func_00350660(ModelDraw *d, const ModelDrawParams *p);   /* queue a model draw */

/* +0x14 draw: model +0x10 at its spot (65.64, 6.1, 70.94), turned by its angles */
/* 0x00350860 */
void Effect78BC0_Draw(u8 *o) {
    static const F32Bits kX = {0x42834704}, kY = {0x40C33333}, kZ = {0x428DE227}, kR = {0x421F3333};
    ModelDraw d __attribute__((aligned(16)));
    ModelDrawParams p __attribute__((aligned(16)));

    p.pos[0] = kX.f;
    p.pos[1] = kY.f;
    p.pos[2] = kZ.f;
    p.pos[3] = 1.0f;
    p.n = 0xF;
    p.radius = kR.f;
    p.angle[0] = AT(o, 0x4, f32);
    p.angle[1] = AT(o, 0x8, f32);
    p.angle[2] = AT(o, 0xC, f32);
    p.model = AT(o, 0x10, s32);
    p.rgba = 0x80808080;
    d.slot = -1;
    d.vtbl = D_00478B70;
    func_00350660(&d, &p);
    d.vtbl = D_00469D00;
}

/* +0x18 start: model 0x30 (params[0] 0) or 0x2C, then a first update */
/* 0x00350810 */
void Effect78BC0_SetParams(VObject *o, const u8 *params) {
    if (params == NULL) {
        return;
    }
    AT(o, 0x10, s32) = params[0] == 0 ? 0x30 : 0x2C;
    VCALL(o, 0x10, s32 (*)(VObject *))(o);
}

/* fill draw object `d` from `p` and queue it with the renderer (+0xC, layer 0x19) */
void func_00350660(ModelDraw *d, const ModelDrawParams *p) {
    d->p.pos[0] = p->pos[0];
    d->p.pos[1] = p->pos[1];
    d->p.pos[2] = p->pos[2];
    d->p.pos[3] = p->pos[3];
    d->p.n = p->n;
    d->p.radius = p->radius;
    d->p.angle[0] = p->angle[0];
    d->p.angle[1] = p->angle[1];
    d->p.angle[2] = p->angle[2];
    d->p.model = p->model;
    d->p.rgba = p->rgba;
    VCALL(gRenderer, 0xC, void (*)(VObject *, ModelDraw *, s32, s32))(gRenderer, d, 0x19, 0);
}

/* ---- class D_00477E10 (room 0x55, 0x18 bytes): a light caustic like D_00478BC0's, over the
 * whole room (texture 3, 160 across) at height +0x14; +0x4/+0x8/+0xC its turns (updated by
 * Room55Effect_Update), +0x10 the glow threshold ---- */

extern void *D_00477E10[];

/* +0x8 destructor */
/* 0x003474E0 */
u8 *Room55Effect_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00477E10;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* +0xC reset: three random angles in -pi..pi */
/* 0x003477A0 */
void Room55Effect_Start(u8 *o) {
    static const F32Bits kPi = {0x40490FDB};
    VObject *rng = gRandom;
    s32 k;

    for (k = 0; k < 3; k++) {
        f32 r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);

        AT(o, 0x4 + k * 4, f32) = kPi.f * (360.0f * (r - 0.5f)) / 180.0f;
    }
}

/* 0x003478C0 */
Character *Kind35_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00477E30); }

s32 func_003479D0(void) {
    return 0x23;
}

/* 0x003479E0 */
void *Kind35_MotionFiles(void) {
    return D_0043CDC0;
}

/* +0x14 draw */
/* 0x003475A0 */
void Room55Effect_Draw(u8 *o) {
    ModelDraw d __attribute__((aligned(16)));
    ModelDrawParams p __attribute__((aligned(16)));

    p.pos[0] = 0.0f;
    p.pos[1] = AT(o, 0x14, f32);
    p.pos[2] = 0.0f;
    p.pos[3] = 1.0f;
    p.n = 3;
    p.radius = 160.0f;
    p.angle[0] = AT(o, 0x4, f32);
    p.angle[1] = AT(o, 0x8, f32);
    p.angle[2] = AT(o, 0xC, f32);
    p.model = AT(o, 0x10, s32);
    p.rgba = 0x80808080;
    d.slot = -1;
    d.vtbl = D_00478B70;
    func_00350660(&d, &p);
    d.vtbl = D_00469D00;
}

/* destructor (vtable D_00478B70) */
void *func_00347640(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00478B70;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* Advances three angles (+0x4 by 0.5 deg, +0x8 by 0.1 deg, +0xC by -0.1 deg), wrapped to [-pi, pi]. */
/* 0x003476A0 */
s32 Room55Effect_Update(f32 *a) {
    a[1] += 0x1.1df46ap-7f /* 0.008726646 */;
    if (!(a[1] <= 0x1.921fb6p+1f /* 3.1415927 */)) {
        a[1] -= 0x1.921fb6p+2f /* 6.2831855 */;
    }
    a[2] += 0x1.c98712p-10f /* 0.0017453294 */;
    if (!(a[2] <= 0x1.921fb6p+1f /* 3.1415927 */)) {
        a[2] -= 0x1.921fb6p+2f /* 6.2831855 */;
    }
    a[3] -= 0x1.c98712p-10f /* 0.0017453294 */;
    if (a[3] < -0x1.921fb6p+1f /* -3.1415927 */) {
        a[3] += 0x1.921fb6p+2f /* 6.2831855 */;
    }
    return 1;
}

/* ---- class D_00478BE0 (room 0x4E, 0x10 bytes): a light caustic (texture 8, 120 across) at
 * (-8.77, -7.4, -0.007), glow threshold 0x80; +0x4/+0x8/+0xC its turns (updated by
 * Room4EEffect_Update) ---- */

extern void *D_00478BE0[];

/* +0x8 destructor */
/* 0x00350B30 */
u8 *Room4EEffect_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00478BE0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* +0xC reset: three random angles in -pi..pi */
/* 0x00350D40 */
void Room4EEffect_Start(u8 *o) {
    static const F32Bits kPi = {0x40490FDB};
    VObject *rng = gRandom;
    s32 k;

    for (k = 0; k < 3; k++) {
        f32 r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);

        AT(o, 0x4 + k * 4, f32) = kPi.f * (360.0f * (r - 0.5f)) / 180.0f;
    }
}

/* +0x14 draw */
/* 0x00350B90 */
void Room4EEffect_Draw(u8 *o) {
    static const F32Bits kX = {0xC10C3F14}, kY = {0xC0ECCCCD}, kZ = {0xBBEBEDFA};
    ModelDraw d __attribute__((aligned(16)));
    ModelDrawParams p __attribute__((aligned(16)));

    p.pos[0] = kX.f;
    p.pos[1] = kY.f;
    p.pos[2] = kZ.f;
    p.pos[3] = 1.0f;
    p.n = 8;
    p.radius = 120.0f;
    p.angle[0] = AT(o, 0x4, f32);
    p.angle[1] = AT(o, 0x8, f32);
    p.angle[2] = AT(o, 0xC, f32);
    p.model = 0x80;
    p.rgba = 0x80808080;
    d.slot = -1;
    d.vtbl = D_00478B70;
    func_00350660(&d, &p);
    d.vtbl = D_00469D00;
}

/* 0x00350C40 */
s32 Room4EEffect_Update(f32 *a) {
    a[1] += 0x1.1df46ap-7f /* 0.008726646 */;
    if (!(a[1] <= 0x1.921fb6p+1f /* 3.1415927 */)) {
        a[1] -= 0x1.921fb6p+2f /* 6.2831855 */;
    }
    a[2] += 0x1.c98712p-10f /* 0.0017453294 */;
    if (!(a[2] <= 0x1.921fb6p+1f /* 3.1415927 */)) {
        a[2] -= 0x1.921fb6p+2f /* 6.2831855 */;
    }
    a[3] -= 0x1.c98712p-10f /* 0.0017453294 */;
    if (a[3] < -0x1.921fb6p+1f /* -3.1415927 */) {
        a[3] += 0x1.921fb6p+2f /* 6.2831855 */;
    }
    return 1;
}

/* (as Room4EEffect_Draw)  +0x14 draw: model 0x30 */
/* 0x00377D20 */
void Effect7A3D0_Draw(u8 *o) {
    static const F32Bits kX = {0xC3610000}, kY = {0xC27E7AE2}, kZ = {0xC48CA000};
    ModelDraw d __attribute__((aligned(16)));
    ModelDrawParams p __attribute__((aligned(16)));

    p.pos[0] = kX.f;
    p.pos[1] = kY.f;
    p.pos[2] = kZ.f;
    p.pos[3] = 1.0f;
    p.n = 0xC;
    p.radius = 1400.0f;
    p.angle[0] = AT(o, 0x4, f32);
    p.angle[1] = AT(o, 0x8, f32);
    p.angle[2] = AT(o, 0xC, f32);
    p.model = 0x30;
    p.rgba = 0x80808080;
    d.slot = -1;
    d.vtbl = D_00478B70;
    func_00350660(&d, &p);
    d.vtbl = D_00469D00;
}

/* (class D_0047A3F0)  +0x14 draw: model 0x2A at (0, 30.1, 0), 3750 across, its turns +0x4..,
 * dimmed to 0x40 */
/* 0x00378050 */
void BackdropModel_Draw(u8 *o) {
    static const F32Bits kY = {0x41F0CCCD};
    ModelDraw d __attribute__((aligned(16)));
    ModelDrawParams p __attribute__((aligned(16)));

    p.pos[0] = 0.0f;
    p.pos[1] = kY.f;
    p.pos[2] = 0.0f;
    p.pos[3] = 1.0f;
    p.n = 0;
    p.radius = 3750.0f;
    p.angle[0] = AT(o, 0x4, f32);
    p.angle[1] = AT(o, 0x8, f32);
    p.angle[2] = AT(o, 0xC, f32);
    p.model = 0x2A;
    p.rgba = 0x40404040;
    d.slot = -1;
    d.vtbl = D_00478B70;
    func_00350660(&d, &p);
    d.vtbl = D_00469D00;
}

/* ---- class D_0047A410: model 0x2C turning slowly about y at (400, 27.1, -125) ---- */

/* +0xC reset: a random turn */
/* 0x00378470 */
void TurningModel_Start(u8 *o) {
    f32 r = VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom);

    AT(o, 0x4, f32) = 0x1.921fb6p+1f * (360.0f * (r - 0.5f)) / 180.0f;
}

/* +0x10 update: half a degree on */
/* 0x00378410 */
s32 TurningModel_Update(u8 *o) {
    static const F32Bits kHalfDeg = {0x3C0EFA35}, kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};

    AT(o, 0x4, f32) = AT(o, 0x4, f32) + kHalfDeg.f;
    if (!(AT(o, 0x4, f32) <= kPi.f)) {
        AT(o, 0x4, f32) = AT(o, 0x4, f32) - kTwoPi.f;
    }
    return 1;
}

/* +0x14 draw: 1000 across, dimmed to 0x40 */
/* 0x00378370 */
void TurningModel_Draw(u8 *o) {
    static const F32Bits kY = {0x41D8CCCD};
    ModelDraw d __attribute__((aligned(16)));
    ModelDrawParams p __attribute__((aligned(16)));

    p.pos[0] = 400.0f;
    p.pos[1] = kY.f;
    p.pos[2] = -125.0f;
    p.pos[3] = 1.0f;
    p.n = 0;
    p.radius = 1000.0f;
    p.angle[0] = AT(o, 0x4, f32);
    p.angle[1] = 0.0f;
    p.angle[2] = 0.0f;
    p.model = 0x2C;
    p.rgba = 0x40404040;
    d.slot = -1;
    d.vtbl = D_00478B70;
    func_00350660(&d, &p);
    d.vtbl = D_00469D00;
}

/* (class D_0047A430)  +0x14 draw, dimmed to 0x40 at height 27.1: with +0x10 model 0x2A at (0, 50),
 * 500 across, all three turns; else model 0x2C at (775, -420), 1500 across, turned about y only */
/* 0x00378570 */
void BackdropModel2_Draw(u8 *o) {
    static const F32Bits kY = {0x41D8CCCD};
    ModelDraw d __attribute__((aligned(16)));
    ModelDrawParams p __attribute__((aligned(16)));

    p.pos[3] = 1.0f;
    p.n = 0;
    p.pos[1] = kY.f;
    p.angle[0] = AT(o, 0x4, f32);
    p.rgba = 0x40404040;
    if (AT(o, 0x10, s32) != 0) {
        p.pos[0] = 0.0f;
        p.pos[2] = 50.0f;
        p.radius = 500.0f;
        p.angle[1] = AT(o, 0x8, f32);
        p.angle[2] = AT(o, 0xC, f32);
        p.model = 0x2A;
    } else {
        p.angle[1] = 0.0f;
        p.angle[2] = 0.0f;
        p.pos[0] = 775.0f;
        p.pos[2] = -420.0f;
        p.radius = 1500.0f;
        p.model = 0x2C;
    }
    d.slot = -1;
    d.vtbl = D_00478B70;
    func_00350660(&d, &p);
    d.vtbl = D_00469D00;
}

/* +0x18 start: params[0] 0 at height 45.1 (threshold 0x60), else -5.9 (0x58); then a first
 * update */
/* 0x00347540 */
void Room55Effect_SetParams(VObject *o, const s32 *params) {
    static const F32Bits kHigh = {0x42346666}, kLow = {0xC0BCCCCD};

    if (params == NULL) {
        return;
    }
    if (params[0] == 0) {
        AT(o, 0x14, f32) = kHigh.f;
        AT(o, 0x10, s32) = 0x60;
    } else {
        AT(o, 0x14, f32) = kLow.f;
        AT(o, 0x10, s32) = 0x58;
    }
    VCALL(o, 0x10, s32 (*)(VObject *))(o);
}

/* +0x10 each frame (returns 0 once every particle has left): 16 particles, double-buffered
 * (+0x10, 0x300 per buffer, 0x30 each), drift by their velocity (+0x820) plus a wind
 * (+0x8E0..) whose phase (+0x8F0) changes at random; they spin (+0x660 by +0x760 degrees),
 * count frames up to +0x643, and respawn once past x = 300 */
/* 0x00321FE0 */
s32 Room2AWisps_Update(u8 *o) {
    static const union { u32 u; f32 f; } k0005 = {0x3BA3D70A}, k005 = {0x3D4CCCCD}, kPi = {0x40490FDB},
        kTwoPi = {0x40C90FDB};
    s32 i, k;

    if (AT(o, 0x8F8, u8) == 1) {
        return 0;
    }
    AT(o, 0x8F8, u8) = 1;
    AT(o, 0x8F4, s32) ^= 1;
    if (--AT(o, 0x8EC, s32) == 0) {
        AT(o, 0x8EC, s32) = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 0x3F;
        AT(o, 0x8F0, s32) = (AT(o, 0x8F0, s32) + 1) & 3;
    }
    if (AT(o, 0x8F0, s32) == 1) {
        AT(o, 0x8E0, f32) = AT(o, 0x8E0, f32) - k0005.f;
        if (AT(o, 0x8E0, f32) < -k005.f) {
            AT(o, 0x8E0, f32) = -k005.f;
        }
        AT(o, 0x8E4, f32) = 0.0f;
    } else if (AT(o, 0x8F0, s32) == 3) {
        AT(o, 0x8E0, f32) = AT(o, 0x8E0, f32) + k0005.f;
        if (!(AT(o, 0x8E0, f32) <= k005.f)) {
            AT(o, 0x8E0, f32) = k005.f;
        }
        AT(o, 0x8E4, u32) = 0xBCF5C28F;   /* -0.03f */
    }
    for (i = 0; i < 16; i++) {
        u32 buf = AT(o, 0x8F4, u32);
        u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0x300 + i * 0x30, u32);
        u32 *dst = &AT(o, 0x10 + buf * 0x300 + i * 0x30, u32);
        u8 *p;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        p = o + 0x10 + AT(o, 0x8F4, s32) * 0x300 + i * 0x30;
        AT(p, 0x10, f32) = AT(p, 0x10, f32) + (AT(o, 0x820 + i * 0xC, f32) + AT(o, 0x8E0, f32));
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + (AT(o, 0x824 + i * 0xC, f32) + AT(o, 0x8E4, f32));
        AT(p, 0x18, f32) = AT(p, 0x18, f32) + (AT(o, 0x828 + i * 0xC, f32) + AT(o, 0x8E8, f32));
        if (!(AT(p, 0x10, f32) < 300.0f)) {
            func_003219A0(o, i);
            continue;
        }
        AT(o, 0x8F8, u8) = 0;
        for (k = 0; k < 3; k++) {
            f32 *a = &AT(o, 0x660 + i * 0x10 + k * 4, f32);

            *a = *a + kPi.f * AT(o, 0x760 + i * 0xC + k * 4, f32) / 180.0f;
            if (!(*a <= kPi.f)) {
                *a = *a - kTwoPi.f;
            } else if (*a < -kPi.f) {
                *a = *a + kTwoPi.f;
            }
        }
        if (++AT(p, 0x2C, s32) >= AT(o, 0x643, s8)) {
            AT(p, 0x2C, s32) = 0;
        }
    }
    return 1;
}

/* +0x14 draw: each of the 16 particles is a unit quad in the xz plane turned by its spin
 * (+0x660), drawn by the quad drawer (+0x610) from the current buffer's record */
/* 0x00321E30 */
void Room2AWisps_Draw(u8 *o) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 c[4][4] __attribute__((aligned(16)));
    s32 i;

    AT(o, 0x624, f32 *) = c[0];
    for (i = 0; i < 16; i++) {
        sceVu0UnitMatrix(m);
        sceVu0RotMatrix(m, m, (f32 *)(o + 0x660 + i * 0x10));
        c[0][0] = 0.5f;  c[0][1] = 0.0f; c[0][2] = -0.5f; c[0][3] = 1.0f;
        sceVu0ApplyMatrix(c[0], m, c[0]);
        c[1][0] = 0.5f;  c[1][1] = 0.0f; c[1][2] = 0.5f;  c[1][3] = 1.0f;
        sceVu0ApplyMatrix(c[1], m, c[1]);
        c[2][0] = -0.5f; c[2][1] = 0.0f; c[2][2] = -0.5f; c[2][3] = 1.0f;
        sceVu0ApplyMatrix(c[2], m, c[2]);
        c[3][0] = -0.5f; c[3][1] = 0.0f; c[3][2] = 0.5f;  c[3][3] = 1.0f;
        sceVu0ApplyMatrix(c[3], m, c[3]);
        AT(o, 0x620, u8 *) = o + 0x10 + AT(o, 0x8F4, s32) * 0x300 + i * 0x30;
        func_002E56C0(o + 0x610);
    }
}

#ifdef HG_NATIVE

#define GLR_PRIM_NOZW 0x20000u
#define GLR_PRIM_FIX(f) (0x80000u | (u32)(f) << 24)   /* blend Cs * f / 128 + Cd */

/* func_0034E9E0, the caustic (vtable D_00478B70 +0xC; the draw object of func_00350660): the
 * frame's alpha cleared, then two 8 x 8 grids of the texture (+0x20) - a square of side +0x24
 * at +0x10 lying flat, turned +0x2C / +0x30 about y - added at 1/8 in colour +0x38, depth
 * tested without depth writes. Their texture coordinates wobble by 0.05 with the phase +0x28
 * (cos along one axis, sin along the other, a quarter turn per cell; the second grid -0.4 of a
 * half turn per cell). Where they leave the frame's alpha at least +0x34, the halved screen is
 * blurred (4 diagonal taps at 1/2) and added back at 1/2 - the caustic's glow. On PC glr does
 * the passes; 0 = nothing linked into the layer. */
s32 func_0034E9E0(u8 *d) {
    VObject *cam = gCamera;
    const void *tex = VCALL(gTexCache, 0xC, void *(*)(VObject *, s32, s32))(gTexCache, AT(d, 0x20, s32), 0);
    f32 size = AT(d, 0x24, f32), half = 0.5f * size, cell = 0.125f * size, phase = AT(d, 0x28, f32);
    s32 g;

    if (tex == NULL) {
        return 0;
    }
    glr_caustic_begin();
    for (g = 0; g < 2; g++) {
        f32 clip[4][4] __attribute__((aligned(16)));
        f32 m[4][4] __attribute__((aligned(16)));
        s32 i, j, k;

        VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, clip);
        sceVu0UnitMatrix(m);
        sceVu0RotMatrixY(m, m, AT(d, g == 0 ? 0x2C : 0x30, f32));
        sceVu0TransMatrix(m, m, (f32 *)(d + 0x10));
        sceVu0MulMatrix(clip, clip, m);
        for (i = 0; i < 8; i++) {   /* a strip along each row band */
            f32 xyzw[18][4] __attribute__((aligned(16)));
            f32 st[18][2];
            u8 rgba[18][4];

            for (j = 0; j < 9; j++) {
                for (k = 0; k < 2; k++) {
                    s32 v = j * 2 + k, r = i + k;

                    xyzw[v][0] = (f32)r * cell - half;
                    xyzw[v][1] = 0.0f;
                    xyzw[v][2] = (f32)j * cell - half;
                    AT(&xyzw[v][3], 0, u32) = j == 0 ? 0x8000 : 0;   /* the first column starts the strip */
                    if (g == 0) {
                        st[v][0] = 0.1f + 0.1f * (f32)r
                                   + 0.05f * func_0031C058(func_002E2D00(phase + 0.5f * (3.1415927f * (f32)r)));
                        st[v][1] = 0.1f * (f32)j + 0.05f * func_0031C248(func_002E2D00(phase + 0.5f * (3.1415927f * (f32)j)));
                    } else {
                        st[v][0] = 0.1f * (f32)j + 0.05f * func_0031C058(func_002E2D00(phase + -0.4f * (3.1415927f * (f32)r)));
                        st[v][1] = 0.1f + 0.1f * (f32)r
                                   + 0.05f * func_0031C248(func_002E2D00(phase + -0.4f * (3.1415927f * (f32)j)));
                    }
                    AT(rgba[v], 0, u32) = AT(d, 0x38, u32);
                }
            }
            glr_strip(&clip[0][0], 18, &xyzw[0][0], &st[0][0], &rgba[0][0], tex, 1ULL << 34,
                      0x10 | 0x40 | GLR_PRIM_NOZW | GLR_PRIM_FIX(0x10));
        }
    }
    glr_caustic_glow(AT(d, 0x34, s32));
    return 0;
}
#endif

/* ---- class D_00479400 (room 0x55, 0x6D0 bytes): 16 drops falling from up to 150 over an
 * 80 x 80 square, double-buffered (+0x10 + buffer +0x688 * 0x300, a quad record of 0x30 each)
 * and drawn by the quad drawer at +0x610 (texture group 0x10, cell (0x6C, 0x4C) 8 x 8,
 * additive with glow); +0x648 + i * 4 their fall speed, +0x690 + i * 4 the nav triangle under
 * each (-1 none), +0x68C the next splash sound (0..2). Where a drop lands it splashes (and
 * starts again): on the floor a spray, off the mesh below -6 a splash ring and a spray ---- */

#include "effectmgr.h"

extern void *D_00479400[], *D_00479AE0[], *D_00479AA0[];

/* +0x8 destructor (the quad drawer at +0x610 inlined) */
/* 0x003532A0 */
u8 *FallingDrops_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479400;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

#define DROP(o, i) ((o) + AT(o, 0x688, s32) * 0x300 + (i) * 0x30 + 0x10)

/* drop i anew: faint grey (alpha 0x70), half size, somewhere in the square up to 150 high,
 * turned at random, falling 1.5..2.5 a frame */
void func_00353330(u8 *o, s32 i) {
    static const F32Bits kPi = {0x40490FDB};
    VObject *rng = gRandom;
    u8 *p = DROP(o, i);
    VObject *nav;

    AT(p, 0x0, s32) = 0x20;
    AT(p, 0x4, s32) = 0x20;
    AT(p, 0x8, s32) = 0x20;
    AT(p, 0xC, s32) = 0x70;
#define RND() VCALL(rng, 0x18, f32 (*)(VObject *))(rng)
    AT(p, 0x10, f32) = 80.0f * (RND() - 0.5f);
    AT(p, 0x14, f32) = 150.0f * RND();
    AT(p, 0x18, f32) = 80.0f * (RND() - 0.5f);
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x20, f32) = 0.5f;
    AT(p, 0x24, f32) = 0.5f;
    AT(p, 0x28, f32) = kPi.f * (360.0f * (RND() - 0.5f)) / 180.0f;
    AT(p, 0x2C, s32) = 0;
    AT(o, 0x648 + i * 4, f32) = 1.5f + RND();
#undef RND
    nav = (VObject *)gNavMesh;
    AT(o, 0x690 + i * 4, s32) = VCALL(nav, 0x3C, s32 (*)(VObject *, f32 *, s32))(nav, (f32 *)(p + 0x10), 0);
}

/* +0x14 draw: the current buffer's 16 records */
/* 0x003534F0 */
void FallingDrops_Draw(u8 *o) {
    AT(o, 0x620, u8 *) = DROP(o, 0);
    func_002E56C0(o + 0x610);
}

/* a splash's parameters (D_00479AE0: pos, colour, size) and a spray's (D_00479AA0) */
typedef struct {
    f32 pos[4];
    u8 rgba[4];
    f32 size;
} SplashParams;

typedef struct {
    f32 pos[4];
    u8 rgba[4];
    s32 n;
    f32 v[10];
} SprayParams;

static void splash_init(void **obj) {
    obj[0] = D_00479AE0;
}

static void spray_init(void **obj) {
    obj[0] = D_00479AA0;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* +0x10 update: swap buffers, carry each drop over and drop it; a landed one splashes (the
 * first drop also with sound 2..4 of bank 6) and starts again */
/* 0x00353520 */
s32 FallingDrops_Update(u8 *o) {
    VObject *rng = gRandom;
    u8 *mgr = gEffects;
    VObject *nav;
    VObject *snd;
    s32 i, k;

    AT(o, 0x688, s32) ^= 1;
    nav = (VObject *)gNavMesh;
    snd = gSound;
    for (i = 0; i < 16; i++) {
        u32 buf = AT(o, 0x688, u32);
        u32 *dst = &AT(o, 0x10 + buf * 0x300 + i * 0x30, u32);
        u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0x300 + i * 0x30, u32);
        u8 *p;
        f32 at[4] __attribute__((aligned(16)));
        u8 landed = 0;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        p = DROP(o, i);
        AT(p, 0x14, f32) = AT(p, 0x14, f32) - AT(o, 0x648 + i * 4, f32);
        if (AT(o, 0x690 + i * 4, s32) == -1) {
            if (AT(p, 0x14, f32) < -6.0f) {
                static const F32Bits kWater = {0xC0BCCCCD}, k01 = {0x3DCCCCCD}, k015 = {0x3E19999A},
                                     k02 = {0x3E4CCCCD}, k03 = {0x3E99999A};
                SplashParams s __attribute__((aligned(16)));
                SprayParams r __attribute__((aligned(16)));

                landed = 1;
                sceVu0CopyVector(at, (f32 *)(p + 0x10));
                at[1] = kWater.f;
                sceVu0CopyVector(s.pos, at);
                s.rgba[3] = 0x20;
                s.rgba[0] = 0x40;
                s.rgba[1] = 0x40;
                s.rgba[2] = 0x40;
                s.size = k015.f + k01.f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng);
                func_002D6090(mgr, Effect_New(mgr, 0x40, splash_init), &s);
                sceVu0CopyVector(r.pos, at);
                r.v[2] = 0.0f;
                r.rgba[0] = 0x20;
                r.rgba[3] = 0x30;
                r.n = 1;
                r.rgba[1] = 0x20;
                r.rgba[2] = 0x20;
                r.v[0] = 0.5f;
                r.v[8] = 0.5f;
                r.v[4] = k03.f;
                r.v[1] = k02.f;
                r.v[5] = k02.f;
                r.v[9] = k01.f;
                r.v[3] = 0.0f;
                r.v[6] = 0.0f;
                r.v[7] = 0.0f;
                func_002D6090(mgr, Effect_New(mgr, 0x720, spray_init), &r);
            }
        } else {
            sceVu0CopyVector(at, (f32 *)(p + 0x10));
            VCALL(nav, 0x14, void (*)(VObject *, s32, f32 *))(nav, AT(o, 0x690 + i * 4, s32), at);
            if (AT(p, 0x14, f32) < at[1]) {
                static const F32Bits k01 = {0x3DCCCCCD}, k02 = {0x3E4CCCCD}, k04 = {0x3ECCCCCD};
                SprayParams r __attribute__((aligned(16)));

                landed = 1;
                sceVu0CopyVector(r.pos, at);
                r.rgba[0] = 0x20;
                r.rgba[1] = 0x20;
                r.rgba[2] = 0x20;
                r.rgba[3] = 0x10;
                r.n = 0x10;
                r.v[0] = k04.f;
                r.v[1] = k04.f;
                r.v[8] = 0.5f;
                r.v[2] = k01.f;
                r.v[3] = k02.f;
                r.v[4] = k01.f;
                r.v[5] = k02.f;
                r.v[7] = k02.f;
                r.v[6] = k01.f;
                r.v[9] = k01.f;
                func_002D6090(mgr, Effect_New(mgr, 0x720, spray_init), &r);
            }
        }
        if (landed == 1) {
            if (i == 0) {
                Sound_PlayBankAt(snd, AT(o, 0x68C, s32) + 2, 6, (f32 *)(p + 0x10), 0, 0);
                if (AT(o, 0x68C, s32) >= 2) {
                    AT(o, 0x68C, s32) = 0;
                } else {
                    AT(o, 0x68C, s32)++;
                }
            }
            func_00353330(o, i);
        }
    }
    return 1;
}

/* +0xC reset: the quad drawer's settings and 16 new drops */
/* 0x00353BD0 */
void FallingDrops_Start(u8 *o) {
    s32 i;

    AT(o, 0x688, s32) = 0;
    AT(o, 0x68C, s32) = 0;
    AT(o, 0x618, s64) = -1;
    AT(o, 0x624, s32) = 0;
    AT(o, 0x628, s32) = 0;
    AT(o, 0x62C, s32) = 0;
    AT(o, 0x630, s32) = 0x19;
    AT(o, 0x634, s16) = 0x10;
    AT(o, 0x636, s16) = 0x6C;
    AT(o, 0x638, s16) = 0x4C;
    AT(o, 0x63A, s16) = 8;
    AT(o, 0x63C, s16) = 8;
    AT(o, 0x63E, s16) = 0x200;
    AT(o, 0x640, s16) = 0x100;
    AT(o, 0x642, u8) = 0xC0;
    AT(o, 0x643, u8) = 1;
    AT(o, 0x644, u8) = 1;
    AT(o, 0x645, u8) = 0x10;
    AT(o, 0x646, u8) = 0xFF;
    for (i = 0; i < 16; i++) {
        func_00353330(o, i);
    }
}

/* ---- class D_00477AC0 (room 0x66, 0x1BC0 bytes): smoke rising at spot +0x1BB8 / 2 of the
 * table D_0043B640 (x, z pairs) - 64 puffs, double-buffered (+0x10 + buffer +0x1BB0 * 0xC00, a
 * quad record of 0x30 each) and drawn by the quad drawer at +0x1840, with a glow sprite (record
 * +0x1810, drawer +0x1878) whose alpha follows how many puffs show. Per puff its rise speed
 * (+0x18B0 + i * 4), sway phase (+0x19B0) and strength (+0x1AB0, 1 at first); +0x1BBC set
 * (start parameter < 0) it dies down: strengths fall by 0.2 a respawn, the glow (+0x1BB4) by
 * 0.003 ---- */

extern void *D_00477AC0[];
extern f32 D_0043B640[], D_0043B644[];   /* the spots: x, z (read as pairs) */

#define SMOKE_PUFF(o, i) ((o) + AT(o, 0x1BB0, s32) * 0xC00 + (i) * 0x30 + 0x10)

/* +0x8 destructor (the quad drawers at +0x1840 and +0x1878 inlined) */
/* 0x003453D0 */
u8 *Room66Effect_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00477AC0;
    AT(o, 0x1878, void **) = D_0046FC30;
    AT(o, 0x1878, void **) = D_00469D00;
    AT(o, 0x1840, void **) = D_0046FC30;
    AT(o, 0x1840, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* puff i anew (`again` 0: the first time, part way up already): white, alpha about 0x40
 * (+-0x20 by strength), size 0.5..8, scattered about the spot (wider across x at spot 4,
 * else along z), rising 0.65 +- 0.15 x strength; dying down, its strength falls and once the
 * glow is out it just stays hidden */
void func_00345490(u8 *o, s32 i, s32 again) {
    static const F32Bits k02 = {0x3E4CCCCD}, k0003 = {0x3B449BA6}, k03 = {0x3E99999A}, k065 = {0x3F266666},
                         kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    VObject *rng;
    u8 *p;
    f32 up = 0.0f, f;
    s32 k;

    if (AT(o, 0x1BBC, u8) == 1) {
        f32 *s = &AT(o, 0x1AB0 + i * 4, f32);

        *s = *s - k02.f;
        if (*s < 0.0f) {
            *s = 0.0f;
        }
        AT(o, 0x1BB4, f32) = AT(o, 0x1BB4, f32) - k0003.f;
        if (AT(o, 0x1BB4, f32) < 0.0f) {
            AT(o, 0x1BB4, f32) = 0.0f;
            AT(SMOKE_PUFF(o, i), 0xC, s32) = 0;
            return;
        }
    }
    rng = gRandom;
    p = SMOKE_PUFF(o, i);
    AT(p, 0x0, s32) = 0x80;
    AT(p, 0x4, s32) = 0x80;
    AT(p, 0x8, s32) = 0x80;
    AT(p, 0xC, s32) = (s32)(AT(o, 0x1AB0 + i * 4, f32) *
                            (f32)(s32)((VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x3F) - 0x20)) + 0x40;
    AT(p, 0x20, f32) = 0.5f + 7.5f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
    if (again == 0) {
        up = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
        AT(p, 0xC, s32) = (s32)((f32)AT(p, 0xC, s32) * (1.0f - up));
        AT(p, 0x20, f32) = AT(p, 0x20, f32) + 1.5f * up;
    }
    rng = gRandom;
#define RND(o) VCALL(rng, o, f32 (*)(VObject *))(rng)
    k = AT(o, 0x1BB8, s32);
    f = AT(o, 0x1AB0 + i * 4, f32) * (1.0f + (f32)((k == 4) * 5));
    AT(p, 0x10, f32) = D_0043B640[k] + f * (RND(0x18) - 0.5f);
    AT(p, 0x14, f32) = 30.0f * up - 4.0f;
    k = AT(o, 0x1BB8, s32);
    f = AT(o, 0x1AB0 + i * 4, f32) * (1.0f + (f32)((k != 4) << 4));
    AT(p, 0x18, f32) = D_0043B644[k] + f * (RND(0x18) - 0.5f);
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x24, f32) = AT(p, 0x20, f32);
    AT(p, 0x28, f32) = kPi.f * RND(0x1C) / 180.0f;
    AT(p, 0x2C, s32) = 0;
    AT(o, 0x18B0 + i * 4, f32) = k065.f + (k03.f * AT(o, 0x1AB0 + i * 4, f32)) * (RND(0x18) - 0.5f);
    AT(o, 0x19B0 + i * 4, f32) = k2Pi.f * (RND(0x18) - 0.5f);
#undef RND
}

/* +0x14 draw: the puffs, then the glow */
/* 0x00345A00 */
void Room66Effect_Draw(u8 *o) {
    AT(o, 0x1850, u8 *) = SMOKE_PUFF(o, 0);
    func_002E56C0(o + 0x1840);
    AT(o, 0x1888, u8 *) = o + 0x1810;
    func_002E56C0(o + 0x1878);
}

/* +0x10 update (0 once it has died down and no puff shows): swap buffers; each puff grows,
 * turns, sways and rises, and starts again above 50 or (every other frame, fading faster the
 * weaker it is) once faded out; the glow's alpha is the number showing (+0..7) times +0x1BB4 */
/* 0x00345A60 */
s32 Room66Effect_Update(u8 *o) {
    static const F32Bits k001 = {0x3C23D70A}, k02 = {0x3E4CCCCD}, k01 = {0x3DCCCCCD}, kPi = {0x40490FDB},
                         k2Pi = {0x40C90FDB};
    VObject *rng = gRandom;
    s32 i, k, n;

    AT(o, 0x1BB0, s32) ^= 1;
    for (i = 0; i < 64; i++) {
        u32 buf = AT(o, 0x1BB0, u32);
        u32 *dst = &AT(o, 0x10 + buf * 0xC00 + i * 0x30, u32);
        u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0xC00 + i * 0x30, u32);
        f32 *ph = &AT(o, 0x19B0 + i * 4, f32);
        u8 *p;
        f32 a;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        p = SMOKE_PUFF(o, i);
        AT(p, 0x20, f32) = AT(p, 0x24, f32) =
            AT(p, 0x20, f32) + (k001.f + k02.f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng));
        AT(p, 0x28, f32) = AT(p, 0x28, f32) + 0.5f * (kPi.f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng) / 180.0f);
        a = *ph + kPi.f * (10.0f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng)) / 180.0f;
        *ph = a;
        if (!(a <= kPi.f)) {
            *ph = a - k2Pi.f;
        }
        AT(p, 0x10, f32) = AT(p, 0x10, f32) + k01.f * func_0031C248(*ph);
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + AT(o, 0x18B0 + i * 4, f32);
        AT(p, 0x18, f32) = AT(p, 0x18, f32) + k01.f * func_0031C058(*ph);
        if (!(AT(p, 0x14, f32) <= 50.0f)) {
            func_00345490(o, i, 1);
        }
        if (AT(o, 0x1BB0, s32) == 0) {
            f32 w = 1.0f - AT(o, 0x1AB0 + i * 4, f32);
            u32 r = VCALL(rng, 0x10, u32 (*)(VObject *))(rng);

            AT(p, 0xC, s32) = AT(p, 0xC, s32) - ((s32)(20.0f * w * w) + (s32)(r & 3));
            if (AT(p, 0xC, s32) <= 0) {
                func_00345490(o, i, 1);
            }
        }
    }
    n = 0;
    for (i = 0; i < 64; i++) {
        if (AT(SMOKE_PUFF(o, i), 0xC, s32) != 0) {
            n++;
        }
    }
    if (AT(o, 0x1BBC, u8) == 1 && n == 0) {
        return 0;
    }
    AT(o, 0x181C, s32) = n + (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 7);
    AT(o, 0x181C, s32) = (s32)((f32)AT(o, 0x181C, s32) * AT(o, 0x1BB4, f32));
    return 1;
}

/* 0x00345E20 */
void Room66Effect_Start(u8 *p) {
    s32 i;

    *(s32 *)(p + 0x1BB0) = 0;
    p[0x1BBC] = 0;
    *(f32 *)(p + 0x1BB4) = 1.0f;
    *(s64 *)(p + 0x1848) = -1;
    *(s32 *)(p + 0x1854) = 0;
    *(s32 *)(p + 0x1858) = 0;
    *(s32 *)(p + 0x185C) = 0;
    *(s32 *)(p + 0x1860) = 0x19;
    *(u16 *)(p + 0x1864) = 0x40;
    *(u16 *)(p + 0x1866) = 0x180;
    *(u16 *)(p + 0x1868) = 0x80;
    *(u16 *)(p + 0x186A) = 0x20;
    *(u16 *)(p + 0x186C) = 0x20;
    *(u16 *)(p + 0x186E) = 0x200;
    *(u16 *)(p + 0x1870) = 0x100;
    p[0x1872] = 0x40;
    p[0x1873] = 1;
    p[0x1874] = 1;
    p[0x1875] = 0x10;
    p[0x1876] = 6;
    for (i = 0; i < 64; i++) {
        ((f32 *)(p + 0x1AB0))[i] = 1.0f;
    }
}

/* 0x00345EE0 */
Character *Kind34_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00477AE0); }

s32 func_00345FF0(void) {
    return 0x22;
}

/* 0x00346000 */
void *Kind34_MotionFiles(void) {
    return D_0043B6E0;
}

/* (a pursuer class) Pursuer_FilesLoaded, then its model's +0x34 (1) */
/* 0x00346010 */
void Kind34_FilesLoaded(Pursuer *p) {
    Pursuer_FilesLoaded(p);
    VCALL(p->c.motion, 0x34, void (*)(void *, s32))(p->c.motion, 1);
}

/* Writes a position {0, 0, z} for index 0..3. */
/* 0x00346050 */
void Kind34_DoorOffset(void *self, s32 i, f32 *out) {
    switch (i) {
    case 1: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.be824p+2f /* 6.9767 */; break;
    case 3: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.905f06p+2f /* -6.2558 */; break;
    case 0: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.bdc432p+2f /* -6.9651 */; break;
    case 2: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.ce0418p+2f /* 7.219 */; break;
    }
}

/* Writes a position {x, 0, z} for index 10..15. */
/* 0x003460F0 */
void Kind34_ActionOffsets(void *self, s32 i, f32 *out) {
    switch (i) {
    case 10: case 11: out[0] = 0x1.07c84cp-2f /* 0.2576 */; out[1] = 0.0f; out[2] = 0x1.567fccp+3f /* 10.7031 */; break;
    case 12: case 13: out[0] = 0x1.a4a8c2p+0f /* 1.6432 */; out[1] = 0.0f; out[2] = 0x1.5d182ap+3f /* 10.9092 */; break;
    case 14: out[0] = -0x1.5f06f6p-3f /* -0.1714 */; out[1] = 0.0f; out[2] = -0x1.8f6fd2p+1f /* -3.1206 */; break;
    case 15: out[0] = 0x1.9a0276p-2f /* 0.4004 */; out[1] = 0.0f; out[2] = -0x1.792d78p+1f /* -2.9467 */; break;
    }
}

/* +0x18 start: a byte < 0 makes it die down; else spot (byte & 0xF) * 2: 64 puffs and the glow
 * (pinkish white, 20 across, 12 up at the spot; texture group 0x10, cell (0xA0, 0x40)) */
/* 0x003458A0 */
void Room66Effect_SetParams(u8 *o, const s8 *params) {
    s32 i;

    if (params == NULL) {
        return;
    }
    if (params[0] < 0) {
        AT(o, 0x1BBC, u8) = 1;
        return;
    }
    AT(o, 0x1BB8, s32) = (params[0] & 0xF) * 2;
    for (i = 0; i < 64; i++) {
        func_00345490(o, i, 0);
    }
    AT(o, 0x1880, s64) = -1;
    AT(o, 0x1890, s32) = 0;
    AT(o, 0x1894, s32) = 0;
    AT(o, 0x1898, s32) = 0x19;
    AT(o, 0x189C, s16) = 1;
    AT(o, 0x189E, s16) = 0xA0;
    AT(o, 0x18A0, s16) = 0x40;
    AT(o, 0x18A2, s16) = 0x20;
    AT(o, 0x18A4, s16) = 0x20;
    AT(o, 0x18A6, s16) = 0x200;
    AT(o, 0x18A8, s16) = 0x100;
    AT(o, 0x18AA, u8) = 0x40;
    AT(o, 0x18AB, u8) = 1;
    AT(o, 0x18AC, u8) = 1;
    AT(o, 0x18AD, u8) = 0x10;
    AT(o, 0x18AE, u8) = 0xFF;
    AT(o, 0x1810, s32) = 0x80;
    AT(o, 0x1814, s32) = 0x70;
    AT(o, 0x1818, s32) = 0x70;
    AT(o, 0x181C, s32) = 0x40;
    AT(o, 0x1820, f32) = D_0043B640[AT(o, 0x1BB8, s32)];
    AT(o, 0x1824, f32) = 12.0f;
    AT(o, 0x1828, f32) = D_0043B644[AT(o, 0x1BB8, s32)];
    AT(o, 0x182C, f32) = 1.0f;
    AT(o, 0x1830, f32) = 20.0f;
    AT(o, 0x1834, f32) = AT(o, 0x1830, f32);
    AT(o, 0x1838, s32) = 0;
    AT(o, 0x183C, s32) = 0;
}

/* ---- class D_00479580 (room 0x43, 0x6CF0 bytes): a fire - 128 smoke puffs rising from about
 * (-145.6, 35, 7.3) (+0x10, drawer +0x6040; velocity +0x60E8 + i * 12), 32 flame tongues
 * licking about (-135, 9.5, 11) (+0x3010, drawer +0x6078; per tongue its outward drift
 * +0x66E8, rise +0x68E8 and heading +0x6AE8) and a flickering glow (record +0x6010, drawer
 * +0x60B0); the records double-buffered by +0x6CE8 (0x1800 apart). +0x6CEC set (its start
 * parameter) the fire is out: no smoke or glow, and the flames die away ---- */

extern void *D_00479580[];

#define FIRE_REC(o, base, i) ((o) + AT(o, 0x6CE8, s32) * 0x1800 + (i) * 0x30 + (base))

/* +0x8 destructor (the quad drawers at +0x60B0, +0x6078 and +0x6040 inlined) */
/* 0x003570C0 */
u8 *BigFire_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479580;
    AT(o, 0x60B0, void **) = D_0046FC30;
    AT(o, 0x60B0, void **) = D_00469D00;
    AT(o, 0x6078, void **) = D_0046FC30;
    AT(o, 0x6078, void **) = D_00469D00;
    AT(o, 0x6040, void **) = D_0046FC30;
    AT(o, 0x6040, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* +0xC set up */
/* 0x00358200 */
void BigFire_Start(u8 *o) {
    AT(o, 0x6CE8, s32) = 0;
}

/* smoke puff i anew (`again` 0: the first time, part way up already) */
void func_00357630(u8 *o, s32 i, s32 again) {
    static const F32Bits k004 = {0x3D23D70A}, kX = {0xC311999A}, kZ = {0x40E9999A}, k003 = {0x3CF5C28F},
                         k007 = {0x3D8F5C29}, k01 = {0x3DCCCCCD}, kPi = {0x40490FDB};
    VObject *rng = gRandom;
    u8 *p = FIRE_REC(o, 0x10, i);
    s32 age = 0;

    AT(p, 0x0, s32) = 0x80;
    AT(p, 0x4, s32) = 0x80;
    AT(p, 0x8, s32) = 0x80;
    AT(p, 0xC, s32) = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x1F) + 0x20;
    AT(p, 0x20, f32) = 1.5f;
    AT(p, 0x24, f32) = 1.5f;
    if (again == 0) {
        age = (s32)(65.0f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng));
        AT(p, 0xC, s32) = AT(p, 0xC, s32) - (age >> 1);
        if (AT(p, 0xC, s32) < 0) {
            AT(p, 0xC, s32) = 0;
        }
        AT(p, 0x20, f32) = AT(p, 0x24, f32) = 1.5f + k004.f * (f32)age;
    }
    rng = gRandom;
#define RND() VCALL(rng, 0x18, f32 (*)(VObject *))(rng)
    AT(p, 0x10, f32) = kX.f + 4.0f * (RND() - 0.5f);
    AT(p, 0x14, f32) = 35.0f + (f32)age;
    AT(p, 0x18, f32) = kZ.f + 4.0f * (RND() - 0.5f);
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x28, f32) = kPi.f * (360.0f * (RND() - 0.5f)) / 180.0f;
    AT(p, 0x2C, s32) = 0;
    AT(o, 0x60E8 + i * 12, f32) = k003.f * (RND() - 0.5f);
    AT(o, 0x60EC + i * 12, f32) = k007.f + k01.f * RND();
    AT(o, 0x60F0 + i * 12, f32) = k003.f * (RND() - 0.5f);
#undef RND
}

/* flame tongue i anew: orange-red, its drift 0.7..0.8, rise 0.1..0.3, heading at random,
 * placed about the fire by its heading */
void func_003571B0(u8 *o, s32 i, s32 again) {
    static const F32Bits k07 = {0x3F333333}, k01 = {0x3DCCCCCD}, k02 = {0x3E4CCCCD}, k001 = {0x3C23D70A},
                         kM0025 = {0xBCCCCCCD}, k005 = {0x3D4CCCCD}, k03 = {0x3E99999A}, kPi = {0x40490FDB};
    VObject *rng = gRandom;
    f32 *drift = &AT(o, 0x66E8 + i * 4, f32);
    f32 *rise = &AT(o, 0x68E8 + i * 4, f32);
    f32 *head;
    u8 *p;
    s32 age = 0;
    f32 c, s;

#define RND() VCALL(rng, 0x18, f32 (*)(VObject *))(rng)
    *drift = k07.f + k01.f * RND();
    *rise = k01.f + k02.f * RND();
    AT(o, 0x6AE8 + i * 4, f32) = kPi.f * (360.0f * (RND() - 0.5f)) / 180.0f;
    p = FIRE_REC(o, 0x3010, i);
    AT(p, 0x0, s32) = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x1F) + 0x80;
    AT(p, 0x4, s32) = 0x40;
    AT(p, 0x8, s32) = 0x10;
    AT(p, 0xC, s32) = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x3F) + 0x40;
    if (again == 0) {
        age = (s32)RND();
        AT(p, 0xC, s32) = AT(p, 0xC, s32) - (s32)(128.0f * (f32)age);
        if (AT(p, 0xC, s32) < 0) {
            AT(p, 0xC, s32) = 0;
        }
        *drift = *drift + -1.5f * (f32)age;
        if (*drift < k001.f) {
            *drift = k001.f;
        }
        *rise = *rise + kM0025.f * (f32)age;
        if (*rise < k005.f) {
            *rise = k005.f;
        }
    }
    head = &AT(o, 0x6AE8 + i * 4, f32);
    rng = gRandom;
    c = func_0031C058(*head);
    AT(p, 0x10, f32) = (-135.0f + (f32)age) + 4.0f * (RND() - 0.5f) + k01.f * c;
    AT(p, 0x14, f32) = (9.5f + (f32)age) + 2.0f * (RND() - 0.5f);
    s = func_0031C248(*head);
    AT(p, 0x18, f32) = 11.0f + 4.0f * (RND() - 0.5f) + k01.f * s;
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x20, f32) = AT(p, 0x24, f32) = k03.f + k02.f * RND();
    AT(p, 0x28, f32) = kPi.f * (360.0f * (RND() - 0.5f)) / 180.0f;
    AT(p, 0x2C, s32) = 0;
#undef RND
}

/* +0x10 update (0 once the fire is out and no flame shows): swap buffers; the smoke grows,
 * turns, drifts and fades (every other frame), starting again above 100 or once faded; the
 * glow flickers (alpha 0x30 / 0x38); each flame fades (and reddens less), wanders (its heading
 * by up to 15 degrees a frame, 5 once out), drifts out by its drift less 0.1 plus |cos| / 10,
 * rises ever slower, and starts again once faded */
/* 0x00357BF0 */
s32 BigFire_Update(u8 *o) {
    static const F32Bits k004 = {0x3D23D70A}, kTurn = {0x3D567750}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB},
                         kM0015 = {0xBC75C28F}, kM0005 = {0xBBA3D70A}, kMinusPi = {0xC0490FDB},
                         kM01 = {0xBDCCCCCD}, k001 = {0x3C23D70A}, k01 = {0x3DCCCCCD}, k005 = {0x3D4CCCCD};
    VObject *rng;
    u8 alive;
    s32 i, k;

    AT(o, 0x6CE8, s32) ^= 1;
    alive = AT(o, 0x6CEC, s32) != 0;
    if (AT(o, 0x6CEC, s32) == 0) {
        rng = gRandom;
        for (i = 0; i < 128; i++) {
            u32 buf = AT(o, 0x6CE8, u32);
            u32 *dst = &AT(o, 0x10 + buf * 0x1800 + i * 0x30, u32);
            u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0x1800 + i * 0x30, u32);
            u8 *p;
            f32 t;

            for (k = 0; k < 12; k++) {
                dst[k] = src[k];
            }
            p = FIRE_REC(o, 0x10, i);
            AT(p, 0x20, f32) = AT(p, 0x24, f32) = AT(p, 0x20, f32) + k004.f;
            t = AT(p, 0x28, f32) + kTurn.f;
            AT(p, 0x28, f32) = t;
            if (!(t <= kPi.f)) {
                AT(p, 0x28, f32) = t - k2Pi.f;
            }
            AT(p, 0x10, f32) = AT(p, 0x10, f32) + AT(o, 0x60E8 + i * 12, f32);
            AT(p, 0x14, f32) = AT(p, 0x14, f32) + AT(o, 0x60EC + i * 12, f32);
            AT(p, 0x18, f32) = AT(p, 0x18, f32) + AT(o, 0x60F0 + i * 12, f32);
            if (!(AT(p, 0x14, f32) <= 100.0f)) {
                func_00357630(o, i, 1);
            }
            if (AT(o, 0x6CE8, s32) == 0) {
                AT(p, 0xC, s32) = AT(p, 0xC, s32) - (s32)(VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 3);
            }
            if (AT(p, 0xC, s32) <= 0) {
                func_00357630(o, i, 1);
            }
        }
        AT(o, 0x601C, s32) = AT(o, 0x601C, s32) == 0x30 ? 0x38 : 0x30;
    }
    rng = gRandom;
    for (i = 0; i < 32; i++) {
        u32 buf = AT(o, 0x6CE8, u32);
        u32 *dst = &AT(o, 0x3010 + buf * 0x1800 + i * 0x30, u32);
        u32 *src = &AT(o, 0x3010 + (buf ^ 1) * 0x1800 + i * 0x30, u32);
        f32 *drift = &AT(o, 0x66E8 + i * 4, f32);
        f32 *rise = &AT(o, 0x68E8 + i * 4, f32);
        f32 *head = &AT(o, 0x6AE8 + i * 4, f32);
        u8 *p;
        f32 a, d;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        p = FIRE_REC(o, 0x3010, i);
        if (AT(p, 0xC, s32) > 0) {
            AT(p, 0xC, s32) = AT(p, 0xC, s32) - (s32)((VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 3) + 2);
            AT(p, 0x0, s32) = AT(p, 0x0, s32) - (s32)(VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 1);
            if (AT(p, 0x0, s32) < 0) {
                AT(p, 0x0, s32) = 0;
            }
        }
        if (AT(p, 0xC, s32) <= 0) {
            if (AT(o, 0x6CEC, s32) != 0) {
                AT(p, 0xC, s32) = 0;
            } else {
                func_003571B0(o, i, 1);
                alive = 0;
            }
            continue;
        }
        alive = 0;
        if (AT(o, 0x6CEC, s32) == 0) {
            *rise = *rise + kM0015.f;
            *head = *head + kPi.f * (30.0f * (VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0.5f)) / 180.0f;
        } else {
            *rise = *rise + kM0005.f;
            *head = *head + kPi.f * (10.0f * (VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0.5f)) / 180.0f;
        }
        a = *head;
        if (a < kMinusPi.f) {
            *head = a + k2Pi.f;
        } else if (!(a <= kPi.f)) {
            *head = *head - k2Pi.f;
        }
        *drift = *drift + kM01.f;
        if (*drift < k001.f) {
            *drift = k001.f;
        }
        if (!(k01.f * func_0031C058(*head) <= 0.0f)) {
            d = k01.f * func_0031C058(*head);
        } else {
            d = -(k01.f * func_0031C058(*head));
        }
        AT(p, 0x10, f32) = AT(p, 0x10, f32) + (*drift + d);
        if (*rise < k005.f) {
            *rise = k005.f;
        }
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + *rise;
        AT(p, 0x18, f32) = AT(p, 0x18, f32) + k01.f * func_0031C248(*head);
    }
    return alive != 1;
}

/* +0x14 draw: (while it burns) the smoke and the glow, then the flames */
/* 0x00357B60 */
void BigFire_Draw(u8 *o) {
    if (AT(o, 0x6CEC, s32) == 0) {
        AT(o, 0x6050, u8 *) = FIRE_REC(o, 0x10, 0);
        func_002E56C0(o + 0x6040);
        AT(o, 0x60C0, u8 *) = o + 0x6010;
        func_002E56C0(o + 0x60B0);
    }
    AT(o, 0x6088, u8 *) = FIRE_REC(o, 0x3010, 0);
    func_002E56C0(o + 0x6078);
}

/* +0x18 start: params[0] is the out flag; lit, the three drawers (texture group 0x10: smoke
 * cell (0x80, 0) 64 x 32 x 0x20, flames (0x6C, 0x4C) 8 x 8 additive, glow (0xA0, 0x40) 32 x 32
 * additive), 128 puffs, 32 tongues, and the glow (orange, 5 across at (-128, 5, 14)) */
/* 0x00357940 */
void BigFire_SetParams(u8 *o, const s32 *params) {
    s32 i;

    if (params == NULL) {
        return;
    }
    AT(o, 0x6CEC, s32) = params[0];
    if (AT(o, 0x6CEC, s32) != 0) {
        return;
    }
    AT(o, 0x6048, s64) = -1;
    AT(o, 0x6054, s32) = 0;
    AT(o, 0x6058, s32) = 0;
    AT(o, 0x605C, s32) = 0;
    AT(o, 0x6060, s32) = 0x19;
    AT(o, 0x6064, s16) = 0x80;
    AT(o, 0x6066, s16) = 0;
    AT(o, 0x6068, s16) = 0x40;
    AT(o, 0x606A, s16) = 0x20;
    AT(o, 0x606C, s16) = 0x20;
    AT(o, 0x606E, s16) = 0x200;
    AT(o, 0x6070, s16) = 0x100;
    AT(o, 0x6072, u8) = 0;
    AT(o, 0x6073, u8) = 1;
    AT(o, 0x6074, u8) = 1;
    AT(o, 0x6075, u8) = 0x10;
    AT(o, 0x6076, u8) = 0xFF;
    for (i = 0; i < 128; i++) {
        func_00357630(o, i, 0);
    }
    AT(o, 0x6080, s64) = -1;
    AT(o, 0x608C, s32) = 0;
    AT(o, 0x6090, s32) = 0;
    AT(o, 0x6094, s32) = 0;
    AT(o, 0x6098, s32) = 0x19;
    AT(o, 0x609C, s16) = 0x20;
    AT(o, 0x609E, s16) = 0x6C;
    AT(o, 0x60A0, s16) = 0x4C;
    AT(o, 0x60A2, s16) = 8;
    AT(o, 0x60A4, s16) = 8;
    AT(o, 0x60A6, s16) = 0x200;
    AT(o, 0x60A8, s16) = 0x100;
    AT(o, 0x60AA, u8) = 0x40;
    AT(o, 0x60AB, u8) = 1;
    AT(o, 0x60AC, u8) = 1;
    AT(o, 0x60AD, u8) = 0x10;
    AT(o, 0x60AE, u8) = 0xFF;
    for (i = 0; i < 32; i++) {
        func_003571B0(o, i, 0);
    }
    AT(o, 0x60B8, s64) = -1;
    AT(o, 0x60C8, s32) = 0;
    AT(o, 0x60CC, s32) = 0;
    AT(o, 0x60D0, s32) = 0x19;
    AT(o, 0x60D4, s16) = 1;
    AT(o, 0x60D6, s16) = 0xA0;
    AT(o, 0x60D8, s16) = 0x40;
    AT(o, 0x60DA, s16) = 0x20;
    AT(o, 0x60DC, s16) = 0x20;
    AT(o, 0x60DE, s16) = 0x200;
    AT(o, 0x60E0, s16) = 0x100;
    AT(o, 0x60E2, u8) = 0x40;
    AT(o, 0x60E3, u8) = 1;
    AT(o, 0x60E4, u8) = 1;
    AT(o, 0x60E5, u8) = 0x10;
    AT(o, 0x60E6, u8) = 0xFF;
    AT(o, 0x6010, s32) = 0x70;
    AT(o, 0x6014, s32) = 0x40;
    AT(o, 0x6018, s32) = 0x40;
    AT(o, 0x601C, s32) = 0x30;
    AT(o, 0x6020, f32) = -128.0f;
    AT(o, 0x6024, f32) = 5.0f;
    AT(o, 0x6028, f32) = 14.0f;
    AT(o, 0x602C, f32) = 1.0f;
    AT(o, 0x6030, f32) = 5.0f;
    AT(o, 0x6034, f32) = 5.0f;
    AT(o, 0x6038, s32) = 0;
    AT(o, 0x603C, s32) = 0;
}

/* ---- class D_0046EA90 (event 0x35, a room effect of 0xA0 bytes): a swarm of +0x8C butterflies
 * about a point +0x10 (moving 0.4 a frame toward the target +0x30 set by +0x18), its centre
 * +0x20 wandering (heading +0x44 kept turning back toward the point, its tilt +0x40 swaying);
 * +0x60..+0x68 the flap / bob / turn phases, +0x70..+0x78 each one's phase step, +0x7C..+0x84
 * the circle each flies out on. A downstroke now and then drops a dust puff (D_00470E00) once
 * the swarm has moved (+0x88 the tilt last dusted) ---- */

extern void *D_0046EA90[], *D_0046D730[], *D_00470E00[];
extern f32 D_00412710[8];   /* the butterflies' colours (RGBA words) by number & 7 */

/* +0x8 destructor */
void *func_002B8E70(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046EA90;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046D730;
        }
        if ((s16)flags > 0) {
            func_002672E0(o);
        }
    }
    return o;
}

/* +0x18 start: the target (3 x s16 big-endian at params), the count (params[6], if any); the
 * first time also the point and centre there */
void func_002B8ED0(u8 *o, const u8 *params) {
    AT(o, 0x30, f32) = (f32)(s16)((params[0] << 8) + params[1]);
    AT(o, 0x34, f32) = (f32)(s16)((params[2] << 8) + params[3]);
    AT(o, 0x38, f32) = (f32)(s16)((params[4] << 8) + params[5]);
    if (params[6] != 0) {
        AT(o, 0x8C, f32) = (f32)params[6];
    }
    if (AT(o, 0x3C, f32) == 0.0f) {
        AT(o, 0x3C, f32) = 1.0f;
        sceVu0CopyVector((f32 *)(o + 0x10), (f32 *)(o + 0x30));
        sceVu0CopyVector((f32 *)(o + 0x20), (f32 *)(o + 0x10));
    }
}

/* +0xC set up: all at the origin, the phases random (-pi..pi), the steps pi/2 + random pi
 * (wrapped), the circles 2..4 */
void func_002B9F40(u8 *o) {
    static const F32Bits kPi = {0x40490FDB}, k2Pi = {0x40C90FDB}, kHalfPi = {0x3FC90FDB};
    VObject *rng;
    s32 k;

    AT(o, 0x20, s32) = 0;
    AT(o, 0x24, s32) = 0;
    AT(o, 0x28, s32) = 0;
    AT(o, 0x2C, f32) = 1.0f;
    AT(o, 0x10, s32) = 0;
    AT(o, 0x14, s32) = 0;
    AT(o, 0x18, s32) = 0;
    AT(o, 0x1C, f32) = 1.0f;
    AT(o, 0x40, s32) = 0;
    AT(o, 0x44, s32) = 0;
    AT(o, 0x48, s32) = 0;
    AT(o, 0x4C, s32) = 0;
    AT(o, 0x30, s32) = 0;
    AT(o, 0x34, s32) = 0;
    AT(o, 0x38, s32) = 0;
    rng = gRandom;
    AT(o, 0x3C, s32) = 0;
#define RND() VCALL(rng, 0x20, f32 (*)(VObject *))(rng)
    for (k = 0; k < 3; k++) {
        AT(o, 0x60 + k * 4, f32) = k2Pi.f * RND() - kPi.f;
    }
    for (k = 0; k < 3; k++) {
        f32 a = kHalfPi.f + kPi.f * RND();

        AT(o, 0x70 + k * 4, f32) = a;
        AT(o, 0x70 + k * 4, f32) = func_002E2D00(a);
    }
    for (k = 0; k < 3; k++) {
        AT(o, 0x7C + k * 4, f32) = 2.0f + 2.0f * RND();
    }
#undef RND
}

/* +0x10 update: the point closes on the target 0.4 a frame (or lands on it); the tilt sways
 * (+-5 degrees, kept within 10), the heading turns 15..35 degrees a frame back toward the point
 * and the centre goes on 0.4..0.8 along it; the phases step on */
void func_002B9B00(u8 *o) {
    static const F32Bits kPi = {0x40490FDB}, k10 = {0x3E32B8C3}, k04 = {0x3ECCCCCD}, k60 = {0x3F860A92};
    f32 d[4] __attribute__((aligned(16))) = {0};
    f32 e[4] __attribute__((aligned(16))) = {0};
    f32 v[4] __attribute__((aligned(16)));
    VObject *rng;
    f32 dd, a, t, speed;

    sceVu0SubVector(d, (f32 *)(o + 0x30), (f32 *)(o + 0x10));
    dd = sceVu0InnerProduct(d, d);
    if (!(dd <= 1.0f)) {
        volatile f32 len = __builtin_sqrtf(dd);   /* (sqrt.s then div.s, not rsqrt.s) */

        sceVu0ScaleVector(d, d, k04.f / len);
        sceVu0AddVector((f32 *)(o + 0x10), (f32 *)(o + 0x10), d);
    } else if (!(dd <= 0.0f)) {
        sceVu0CopyVector((f32 *)(o + 0x10), (f32 *)(o + 0x30));
    }
    rng = gRandom;
#define RND() VCALL(rng, 0x20, f32 (*)(VObject *))(rng)
    a = kPi.f * (10.0f * RND() - 5.0f) / 180.0f;
    t = AT(o, 0x40, f32) + a;
    if (!((t <= 0.0f ? -t : t) <= k10.f)) {
        AT(o, 0x40, f32) = AT(o, 0x40, f32) - a;
    } else {
        AT(o, 0x40, f32) = AT(o, 0x40, f32) + a;
    }
    sceVu0SubVector(e, (f32 *)(o + 0x10), (f32 *)(o + 0x20));
    if (!(func_002E2D00(Vec_Heading(e) - AT(o, 0x44, f32)) <= 0.0f)) {
        t = AT(o, 0x44, f32) + kPi.f * (15.0f + 20.0f * RND()) / 180.0f;
        AT(o, 0x44, f32) = t;
        AT(o, 0x44, f32) = func_002E2D00(t);
    } else {
        t = AT(o, 0x44, f32) - kPi.f * (15.0f + 20.0f * RND()) / 180.0f;
        AT(o, 0x44, f32) = t;
        AT(o, 0x44, f32) = func_002E2D00(t);
    }
    rng = gRandom;
    speed = k04.f + k04.f * RND();
    Heading_Vector(v, AT(o, 0x44, f32));
    sceVu0ScaleVector(v, v, speed);
    sceVu0AddVector((f32 *)(o + 0x20), (f32 *)(o + 0x20), v);
    AT(o, 0x60, f32) = func_002E2D00(AT(o, 0x60, f32) + (k60.f + 20.0f * RND()));
    t = AT(o, 0x64, f32) + kPi.f * (2.0f + 9.0f * RND()) / 180.0f;
    AT(o, 0x64, f32) = t;
    AT(o, 0x64, f32) = func_002E2D00(t);
    t = AT(o, 0x68, f32) + kPi.f * (40.0f + 20.0f * RND()) / 180.0f;
    AT(o, 0x68, f32) = t;
    AT(o, 0x68, f32) = func_002E2D00(t);
#undef RND
}

/* one butterfly at matrix `m`: two wings (texture group 0x10, cell 40 x 64 at the origin, own
 * corners) folded up by `flap` (cos 0.7 out, sin up), scaled by `size`, in colour `rgba`,
 * layer 1 */
void func_002B97E0(u8 *o, f32 (*m)[4], u32 rgba, f32 flap, f32 size) {
    f32 c[4][4] __attribute__((aligned(16)));
    QuadRec r __attribute__((aligned(16)));
    QuadDrawer q __attribute__((aligned(16)));
    f32 x = 0x1.666666p-1f /* 0.7 */ * func_0031C058(flap);
    f32 y = func_0031C248(flap);

    r.rgba[0] = rgba & 0xFF;
    r.rgba[1] = (rgba >> 8) & 0xFF;
    r.pos[0] = 0.0f;
    r.pos[1] = 0.0f;
    r.rgba[2] = (rgba >> 16) & 0xFF;
    r.pos[2] = 0.0f;
    r.rgba[3] = (rgba >> 24) & 0xFF;
    r.turn = 0.0f;
    r.pos[3] = 1.0f;
    r.w = 1.0f;
    r.h = 1.0f;
    c[0][2] = 1.0f;
    c[0][3] = 1.0f;
    r.frame = 0;
    c[0][0] = 0.0f;
    c[0][1] = 0.0f;
    sceVu0ApplyMatrix(c[0], m, c[0]);
    c[1][2] = 1.0f;
    c[1][3] = 1.0f;
    c[1][0] = -x;
    c[1][1] = y;
    sceVu0ApplyMatrix(c[1], m, c[1]);
    AT(&c[2][2], 0, u32) = 0xBECCCCCC;   /* -0.4 */
    c[2][3] = 1.0f;
    c[2][1] = 0.0f;
    c[2][0] = 0.0f;
    sceVu0ApplyMatrix(c[2], m, c[2]);
    AT(&c[3][2], 0, u32) = 0xBECCCCCC;
    c[3][3] = 1.0f;
    c[3][0] = -x;
    c[3][1] = y;
    sceVu0ApplyMatrix(c[3], m, c[3]);
    sceVu0ScaleVector(c[0], c[0], size);
    sceVu0ScaleVector(c[1], c[1], size);
    sceVu0ScaleVector(c[2], c[2], size);
    sceVu0ScaleVector(c[3], c[3], size);
    q.a = -1;
    q.layer = 1;
    q.tex = (u64)-1;
    q.vtbl = D_0046FC30;
    q.count = 1;
    q.frames = 1;
    q.cx = 0.0f;
    q.rec = &r;
    q.cy = 0.0f;
    q.corners = (s32)c;
    q.cellX = 0;
    q.cellW = 0x28;
    q.cellY = 0;
    q.cellH = 0x40;
    q.texId = 0;
    q.texW = 0x200;
    q.palette = 0;
    q.texH = 0x100;
    q.flags = 2;
    q.texGroup = 0x10;
    func_002E56C0((u8 *)&q);
    c[1][2] = 1.0f;
    c[1][3] = 1.0f;
    c[1][1] = y;
    c[1][0] = x;
    sceVu0ApplyMatrix(c[1], m, c[1]);
    AT(&c[3][2], 0, u32) = 0xBECCCCCC;
    c[3][3] = 1.0f;
    c[3][1] = y;
    c[3][0] = x;
    sceVu0ApplyMatrix(c[3], m, c[3]);
    sceVu0ScaleVector(c[1], c[1], size);
    sceVu0ScaleVector(c[3], c[3], size);
    func_002E56C0((u8 *)&q);
    q.vtbl = D_00469D00;
}

static void dust_init(void **obj) {
    obj[0] = D_00470E00;
    obj[0x70 / 4] = D_00469D00;
    ((s32 *)obj)[0x74 / 4] = -1;
    obj[0x70 / 4] = D_0046FC30;
}

/* +0x14 draw: butterfly i (sign by its parity, phases scaled by i(i+1)) flies its circle about
 * the wandering centre, bobbing, banking and flapping */
void func_002B9010(u8 *o) {
    static const F32Bits kA = {0x3F6CCE68}, kB = {0x3F51FF7F}, kC = {0x3F95ADF0}, k60 = {0x3F860A92},
                         k08 = {0x3F4CCCCD}, k04 = {0x3ECCCCCD}, k50 = {0x3F5F66F3}, k01 = {0x3DCCCCCD},
                         k0005 = {0x3BA3D70A}, k005 = {0x3D4CCCCD};
    f32 col[8];
    VObject *rng, *cam;
    u8 *mgr;
    s32 i, k;

    for (k = 0; k < 8; k++) {
        AT(&col[k], 0, u32) = AT(&D_00412710[k], 0, u32);
    }
    if ((u8)(u32)AT(o, 0x8C, f32) <= 0) {
        return;
    }
    rng = gRandom;
    cam = gCamDirector;
    mgr = gEffects;
    for (i = 0; i < (u8)(u32)AT(o, 0x8C, f32); i++) {
        f32 m[4][4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));
        f32 d[4] __attribute__((aligned(16)));
        f32 rot[4] __attribute__((aligned(16)));
        f32 at[4] __attribute__((aligned(16)));
        s8 sign = (i & 1) * 2 - 1;
        s32 ii = i * (i + 1);
        f32 a, b, c, flap, bob, x, y, z, turn, size;

        a = (f32)sign * func_002E2D00((f32)ii * (AT(o, 0x70, f32) + kA.f));
        b = (f32)sign * func_002E2D00((f32)ii * (AT(o, 0x74, f32) + kB.f));
        c = (f32)sign * func_002E2D00((f32)ii * (AT(o, 0x78, f32) + kC.f));
        flap = func_0031C248(func_002E2D00(a + (AT(o, 0x60, f32) + AT(o, 0x70, f32)))) * k60.f;
        bob = k08.f * func_0031C248(func_002E2D00(b + (AT(o, 0x64, f32) + AT(o, 0x70, f32))));
        v[0] = AT(o, 0x7C, f32);
        v[1] = AT(o, 0x80, f32);
        v[2] = AT(o, 0x84, f32);
        v[3] = 0.0f;
        bob = bob + k04.f * func_0031C248(func_002E2D00(c + (AT(o, 0x68, f32) + AT(o, 0x70, f32))));
        Vec_TurnY(v, v, func_002E2D00(a));
        y = AT(o, 0x14, f32) + v[1];
        x = AT(o, 0x10, f32) + v[0];
        z = AT(o, 0x18, f32) + v[2];
        sceVu0SubVector(d, (f32 *)(o + 0x20), (f32 *)(o + 0x10));
        turn = (f32)(i + 1) * ((f32)sign * k50.f);
        Vec_TurnY(d, d, func_002E2D00(turn));
        at[1] = y + bob;
        at[0] = x + d[0];
        at[2] = z + d[2];
        rot[0] = AT(o, 0x40, f32);
        rot[1] = func_002E2D00(AT(o, 0x44, f32) + turn);
        rot[2] = AT(o, 0x48, f32);
        rot[3] = 0.0f;
        if (!(func_0031C248(c) <= 0.0f)) {
            size = func_0031C248(c);
        } else {
            size = -func_0031C248(c);
        }
        sceVu0UnitMatrix(m);
        sceVu0RotMatrix(m, m, rot);
        sceVu0TransMatrix(m, m, at);
        func_002B97E0(o, m, AT(&col[i & 7], 0, u32), flap, 0.5f + size);
        if (flap < -1.0f && VCALL(rng, 0x18, f32 (*)(VObject *))(rng) < k01.f &&
            (u8)(u32)AT(o, 0x8C, f32) >= 2 && !(VCALL(cam, 0x38, s32 (*)(VObject *))(cam) & 0xFF) &&
            AT(o, 0x88, f32) != AT(o, 0x40, f32)) {
            s32 slot, p[10];

            AT(o, 0x88, f32) = AT(o, 0x40, f32);
            slot = Effect_New(mgr, 0xC0, dust_init);
            p[0] = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x1F) + 0x80;
            p[1] = VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x1F;
            p[2] = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x1F) + 0x80;
            p[3] = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0xF) + 0x70;
            AT(&p[4], 0, f32) = at[0];
            AT(&p[5], 0, f32) = at[1] + 0.5f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
            AT(&p[6], 0, f32) = at[2];
            AT(&p[7], 0, f32) = k0005.f * (at[0] - AT(o, 0x10, f32));
            AT(&p[8], 0, f32) = k005.f + k005.f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
            AT(&p[9], 0, f32) = k0005.f * (at[2] - AT(o, 0x18, f32));
            func_002D6090(mgr, slot, p);
        }
    }
}

/* ---- class D_00476BD0 (room 0x4F, 0x36C0 bytes): orange smoke spiralling up from about
 * (38.4, 0, -10) - 128 puffs, double-buffered (+0x10 + buffer +0x36B0 * 0x1800), drawn by the
 * quad drawer at +0x3040 (texture group 0x10, cell (0xC0, 0x40), additive), and a flickering
 * glow (record +0x3010, drawer +0x3078); per puff its rise (+0x30B0 + i * 4), spiral angle
 * (+0x32B0) and radius (+0x34B0) ---- */

extern void *D_00476BD0[];

#define SMOKE2_PUFF(o, i) ((o) + AT(o, 0x36B0, s32) * 0x1800 + (i) * 0x30 + 0x10)

/* +0x8 destructor (the quad drawers at +0x3078 and +0x3040 inlined) */
/* 0x0033C480 */
u8 *SpiralSmoke_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00476BD0;
    AT(o, 0x3078, void **) = D_0046FC30;
    AT(o, 0x3078, void **) = D_00469D00;
    AT(o, 0x3040, void **) = D_0046FC30;
    AT(o, 0x3040, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* puff i anew (`again` 0: the first time, part way up and faded already): orange, 2..3 across
 * (more the higher), turned at random, rising 0.5..1.5 a frame */
void func_0033C540(u8 *o, s32 i, s32 again) {
    static const F32Bits kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    VObject *rng = gRandom;
    u8 *p = SMOKE2_PUFF(o, i);
    f32 up = 0.0f;

    AT(p, 0x0, s32) = 0xC0;
    AT(p, 0x4, s32) = 0x60;
    AT(p, 0x8, s32) = 0x40;
    AT(p, 0xC, s32) = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 7) + 0x24;
    AT(p, 0x20, f32) = 2.0f + VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
    if (again == 0) {
        f32 k;

        up = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
        k = 1.0f - up;
        AT(p, 0x0, s32) = (s32)((f32)AT(p, 0x0, s32) * k);
        AT(p, 0x4, s32) = (s32)((f32)AT(p, 0x4, s32) * k);
        AT(p, 0x8, s32) = (s32)((f32)AT(p, 0x8, s32) * k);
        AT(p, 0xC, s32) = (s32)((f32)AT(p, 0xC, s32) * k);
        AT(p, 0x20, f32) = AT(p, 0x20, f32) + 2.5f * up;
    }
    AT(p, 0x10, u32) = 0x421A0000;   /* 38.5 */
    rng = gRandom;
    AT(p, 0x14, f32) = 50.0f * up;
    AT(p, 0x18, f32) = -10.0f;
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x24, f32) = AT(p, 0x20, f32);
    AT(p, 0x28, f32) = kPi.f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng) / 180.0f;
    AT(p, 0x2C, s32) = 0;
    AT(o, 0x30B0 + i * 4, f32) = 0.5f + VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
    AT(o, 0x32B0 + i * 4, f32) = k2Pi.f * (VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0.5f);
    AT(o, 0x34B0 + i * 4, f32) = up + VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
}

/* +0x14 draw: the puffs, then the glow */
/* 0x0033C7D0 */
void SpiralSmoke_Draw(u8 *o) {
    AT(o, 0x3050, u8 *) = SMOKE2_PUFF(o, 0);
    func_002E56C0(o + 0x3040);
    AT(o, 0x3088, u8 *) = o + 0x3010;
    func_002E56C0(o + 0x3078);
}

/* +0x10 update: swap buffers; each puff grows, turns, spirals out (by up to 10 degrees a frame,
 * 0.05 wider) and rises, its colour dimming a step a frame and (every other frame) its alpha
 * too, starting again once faded; the glow flickers (alpha 0x38 / 0x40) */
/* 0x0033C830 */
s32 SpiralSmoke_Update(u8 *o) {
    static const F32Bits k005 = {0x3D4CCCCD}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB}, kX = {0x42193333};
    VObject *rng = gRandom;
    s32 i, k;

    AT(o, 0x36B0, s32) ^= 1;
    for (i = 0; i < 128; i++) {
        u32 buf = AT(o, 0x36B0, u32);
        u32 *dst = &AT(o, 0x10 + buf * 0x1800 + i * 0x30, u32);
        u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0x1800 + i * 0x30, u32);
        f32 *ph = &AT(o, 0x32B0 + i * 4, f32);
        f32 *rad = &AT(o, 0x34B0 + i * 4, f32);
        u8 *p;
        f32 a;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        p = SMOKE2_PUFF(o, i);
        AT(p, 0x20, f32) = AT(p, 0x24, f32) = AT(p, 0x20, f32) + k005.f;
        AT(p, 0x28, f32) = AT(p, 0x28, f32) + 0.5f * (kPi.f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng) / 180.0f);
        a = *ph + kPi.f * (10.0f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng)) / 180.0f;
        *ph = a;
        if (!(a <= kPi.f)) {
            *ph = a - k2Pi.f;
        }
        *rad = *rad + k005.f;
        AT(p, 0x10, f32) = kX.f + *rad * func_0031C248(*ph);
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + AT(o, 0x30B0 + i * 4, f32);
        AT(p, 0x18, f32) = -10.0f + *rad * func_0031C058(*ph);
        if (AT(o, 0x36B0, s32) == 0) {
            AT(p, 0xC, s32)--;
            if (AT(p, 0xC, s32) < 0) {
                func_0033C540(o, i, 1);
            }
        }
        for (k = 0; k < 3; k++) {
            AT(p, k * 4, s32)--;
            if (AT(p, k * 4, s32) < 0) {
                AT(p, k * 4, s32) = 0;
            }
        }
    }
    AT(o, 0x301C, s32) = AT(o, 0x301C, s32) == 0x38 ? 0x40 : 0x38;
    return 1;
}

/* +0xC set up: the drawers (puffs: 128, additive, palette 2; glow: one, additive), 128 puffs
 * and the glow (pinkish, 32 across at (38, 15, -11)) */
/* 0x0033CB40 */
void SpiralSmoke_Start(u8 *o) {
    s32 i;

    AT(o, 0x36B0, s32) = 0;
    AT(o, 0x3048, s64) = -1;
    AT(o, 0x3054, s32) = 0;
    AT(o, 0x3058, s32) = 0;
    AT(o, 0x305C, s32) = 0;
    AT(o, 0x3060, s32) = 0x19;
    AT(o, 0x3064, s16) = 0x80;
    AT(o, 0x3066, s16) = 0xC0;
    AT(o, 0x3068, s16) = 0x40;
    AT(o, 0x306A, s16) = 0x20;
    AT(o, 0x306C, s16) = 0x20;
    AT(o, 0x306E, s16) = 0x200;
    AT(o, 0x3070, s16) = 0x100;
    AT(o, 0x3072, u8) = 0x40;
    AT(o, 0x3073, u8) = 1;
    AT(o, 0x3074, u8) = 1;
    AT(o, 0x3075, u8) = 0x10;
    AT(o, 0x3076, u8) = 2;
    for (i = 0; i < 128; i++) {
        func_0033C540(o, i, 0);
    }
    AT(o, 0x3080, s64) = -1;
    AT(o, 0x3090, s32) = 0;
    AT(o, 0x3094, s32) = 0;
    AT(o, 0x3098, s32) = 0x19;
    AT(o, 0x309C, s16) = 1;
    AT(o, 0x309E, s16) = 0xA0;
    AT(o, 0x30A0, s16) = 0x40;
    AT(o, 0x30A2, s16) = 0x20;
    AT(o, 0x30A4, s16) = 0x20;
    AT(o, 0x30A6, s16) = 0x200;
    AT(o, 0x30A8, s16) = 0x100;
    AT(o, 0x30AA, u8) = 0x40;
    AT(o, 0x30AB, u8) = 1;
    AT(o, 0x30AC, u8) = 1;
    AT(o, 0x30AD, u8) = 0x10;
    AT(o, 0x30AE, u8) = 0xFF;
    AT(o, 0x3010, s32) = 0x80;
    AT(o, 0x3014, s32) = 0x70;
    AT(o, 0x3018, s32) = 0x70;
    AT(o, 0x301C, s32) = 0x38;
    AT(o, 0x3020, f32) = 38.0f;
    AT(o, 0x3024, f32) = 15.0f;
    AT(o, 0x3028, f32) = -11.0f;
    AT(o, 0x302C, f32) = 1.0f;
    AT(o, 0x3030, f32) = 32.0f;
    AT(o, 0x3034, f32) = AT(o, 0x3030, f32);
    AT(o, 0x3038, s32) = 0;
    AT(o, 0x303C, s32) = 0;
}

/* ---- class D_00476BF0 (room 0x52, 0x6E0 bytes): five drips (spots +0x30 + k * 0x10, from
 * D_004309D0) that run by turns - +0x4 + k * 4 on, +0x18 + k * 4 frames to the next switch
 * (90, or 90..345 off), +0x6C0 + k * 4 its sound (-1 none; +0x6D4 the next of four). Each sends
 * up ripples, 16 per spot (+0x80.. height, +0x1C0.. life (its alpha), +0x300.. size, +0x440..
 * turn, +0x580.. rise; 0x40 per spot), flat additive quads (texture group 0x10, cell (0xE0,
 * 0x60) 32 x 32) ---- */

extern void *D_00476BF0[];
extern f32 D_004309D0[];   /* the five spots (x, y, z) */

#define DRIP_P(o, k, j, off) AT((o) + (k) * 0x40 + (j) * 4, (off), f32)
#define DRIP_LIFE(o, k, j) AT((o) + (k) * 0x40 + (j) * 4, 0x1C0, s32)

/* +0x8 destructor */
/* 0x0033CCC0 */
u8 *Drips_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00476BF0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* a ripple of spot k anew: at the spot, size 0.1, turned at random, rising 0.5..0.6 */
static inline __attribute__((always_inline)) void ripple_reset(u8 *o, s32 k, s32 j, VObject *rng) {
    static const F32Bits kPi = {0x40490FDB}, k01 = {0x3DCCCCCD};

    DRIP_P(o, k, j, 0x80) = AT(o, 0x34 + k * 0x10, f32);
    DRIP_LIFE(o, k, j) = 0;
    AT(o + k * 0x40 + j * 4, 0x300, u32) = k01.u;
    DRIP_P(o, k, j, 0x440) = kPi.f * (360.0f * (VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0.5f)) / 180.0f;
    DRIP_P(o, k, j, 0x580) = 0.5f + k01.f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
}

/* spot k's ripples: each live one (life down 2 a frame) spreads (size + 1), turns, and bobs on
 * its rise (falling 0.05 a frame); out of life or 4 below the spot it is reset */
void func_0033CD20(u8 *o, s32 k) {
    static const F32Bits kPi = {0x40490FDB}, k2Pi = {0x40C90FDB}, k005 = {0x3D4CCCCD};
    f32 *spot = &AT(o, 0x34 + k * 0x10, f32);
    VObject *rng = gRandom;
    s32 j;

    for (j = 0; j < 16; j++) {
        f32 a, v;

        if (DRIP_LIFE(o, k, j) == 0) {
            continue;
        }
        DRIP_LIFE(o, k, j) -= 2;
        if (DRIP_LIFE(o, k, j) <= 0) {
            ripple_reset(o, k, j, rng);
            continue;
        }
        DRIP_P(o, k, j, 0x300) = DRIP_P(o, k, j, 0x300) + 1.0f;
        a = DRIP_P(o, k, j, 0x440) + kPi.f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng) / 180.0f;
        DRIP_P(o, k, j, 0x440) = a;
        if (!(a <= kPi.f)) {
            DRIP_P(o, k, j, 0x440) = a - k2Pi.f;
        }
        v = DRIP_P(o, k, j, 0x580) - k005.f;
        DRIP_P(o, k, j, 0x580) = v;
        DRIP_P(o, k, j, 0x80) = DRIP_P(o, k, j, 0x80) + v;
        if (DRIP_P(o, k, j, 0x80) < *spot - 4.0f) {
            ripple_reset(o, k, j, rng);
        }
    }
}

/* +0x14 draw: every live ripple, a flat quad of its size and turn at the spot and its height,
 * blue-grey at alpha its life */
/* 0x0033CFD0 */
void Drips_Draw(u8 *o) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 c[4][4] __attribute__((aligned(16)));
    QuadRec r __attribute__((aligned(16)));
    QuadDrawer q __attribute__((aligned(16)));
    s32 k, j;

    r.rgba[2] = 0x30;
    r.w = 1.0f;
    r.h = 1.0f;
    r.pos[3] = 1.0f;
    q.vtbl = D_0046FC30;
    r.pos[0] = 0.0f;
    q.tex = (u64)-1;
    r.pos[1] = 0.0f;
    q.corners = (s32)c;
    r.pos[2] = 0.0f;
    q.layer = 0x19;
    r.rgba[0] = 0x20;
    q.cellX = 0xE0;
    r.rgba[1] = 0x20;
    q.cellY = 0x60;
    r.turn = 0.0f;
    q.texW = 0x200;
    q.texH = 0x100;
    q.flags = 0x42;
    r.frame = 0;
    q.texGroup = 0x10;
    q.a = -1;
    q.palette = -1;
    q.rec = &r;
    q.cx = 0.0f;
    q.cy = 0.0f;
    q.count = 1;
    q.cellW = 0x20;
    q.cellH = 0x20;
    q.frames = 1;
    q.texId = 1;
    for (k = 0; k < 5; k++) {
        for (j = 0; j < 16; j++) {
            if (DRIP_LIFE(o, k, j) == 0) {
                continue;
            }
            sceVu0UnitMatrix(m);
            m[0][0] = DRIP_P(o, k, j, 0x300);
            m[1][1] = DRIP_P(o, k, j, 0x300);
            m[2][2] = DRIP_P(o, k, j, 0x300);
            sceVu0RotMatrixY(m, m, DRIP_P(o, k, j, 0x440));
            sceVu0CopyVector(at, (f32 *)(o + 0x30 + k * 0x10));
            at[1] = DRIP_P(o, k, j, 0x80);
            sceVu0TransMatrix(m, m, at);
            c[0][1] = 0.0f;
            c[0][2] = -1.0f;
            c[0][0] = 1.0f;
            c[0][3] = 1.0f;
            sceVu0ApplyMatrix(c[0], m, c[0]);
            c[1][1] = 0.0f;
            c[1][0] = 1.0f;
            c[1][2] = 1.0f;
            c[1][3] = 1.0f;
            sceVu0ApplyMatrix(c[1], m, c[1]);
            c[2][1] = 0.0f;
            c[2][0] = -1.0f;
            c[2][2] = -1.0f;
            c[2][3] = 1.0f;
            sceVu0ApplyMatrix(c[2], m, c[2]);
            c[3][1] = 0.0f;
            c[3][0] = -1.0f;
            c[3][2] = 1.0f;
            c[3][3] = 1.0f;
            sceVu0ApplyMatrix(c[3], m, c[3]);
            r.rgba[3] = DRIP_LIFE(o, k, j);
            func_002E56C0((u8 *)&q);
        }
    }
    q.vtbl = D_00469D00;
}

/* +0x10 update: each spot's switch counts down - turning on (90 frames) starts its sound (the
 * next of four, if the bank is loaded), off (90..345); while on, the sound is kept going and a
 * new ripple starts; then its ripples move */
/* 0x0033D2C0 */
s32 Drips_Update(u8 *o) {
    VObject *snd = gSound;
    VObject *rng = gRandom;
    s32 k, j;

    for (k = 0; k < 5; k++) {
        AT(o, 0x18 + k * 4, s32)--;
        if (AT(o, 0x18 + k * 4, s32) == 0) {
            AT(o, 0x4 + k * 4, s32) ^= 1;
            if (AT(o, 0x4 + k * 4, s32) != 0) {
                AT(o, 0x18 + k * 4, s32) = 0x5A;
                if ((u8)VCALL(snd, 0xA4, s32 (*)(VObject *, s32))(snd, 6) == 1) {
                    AT(o, 0x6C0 + k * 4, s32) = AT(o, 0x6D4, s32);
                    Sound_PlayBankAt(snd, (AT(o, 0x6C0 + k * 4, s32) + 7) | 0x40000000, 6, (f32 *)(o + 0x30 + k * 0x10), 0, 0);
                    AT(o, 0x6D4, s32)++;
                    AT(o, 0x6D4, s32) &= 3;
                }
            } else {
                AT(o, 0x18 + k * 4, s32) = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0xFF) + 0x5A;
            }
        }
        if (AT(o, 0x4 + k * 4, s32) != 0) {
            if (AT(o, 0x6C0 + k * 4, s32) != -1) {
                Sound_PlayBankAt(snd, (AT(o, 0x6C0 + k * 4, s32) + 7) | 0xC0000000, 6, (f32 *)(o + 0x30 + k * 0x10), 0, 0);
            }
            for (j = 0; j < 16; j++) {
                if (DRIP_LIFE(o, k, j) == 0) {
                    DRIP_LIFE(o, k, j) = 0x20;
                    break;
                }
            }
        }
        func_0033CD20(o, k);
    }
    return 1;
}

/* +0xC set up: the five spots on or off at random, their ripples reset; a running one is run
 * on 64 frames */
/* 0x0033D4B0 */
void Drips_Start(u8 *o) {
    VObject *rng = gRandom;
    s32 k, j, n;

    AT(o, 0x6D4, s32) = 0;
    for (k = 0; k < 5; k++) {
        AT(o, 0x4 + k * 4, s32) = VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 1;
        AT(o, 0x30 + k * 0x10, f32) = D_004309D0[k * 3 + 0];
        AT(o, 0x34 + k * 0x10, f32) = D_004309D0[k * 3 + 1];
        AT(o, 0x38 + k * 0x10, f32) = D_004309D0[k * 3 + 2];
        AT(o, 0x3C + k * 0x10, f32) = 1.0f;
        AT(o, 0x6C0 + k * 4, s32) = -1;
        for (j = 0; j < 16; j++) {
            ripple_reset(o, k, j, rng);
        }
        if (AT(o, 0x4 + k * 4, s32) != 0) {
            AT(o, 0x18 + k * 4, s32) = 0x5A;
            for (n = 0; n < 64; n++) {
                func_0033CD20(o, k);
            }
        } else {
            AT(o, 0x18 + k * 4, s32) = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0xFF) + 0x5A;
        }
    }
}

/* 0x0033D6C0 */
Character *Kind32_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00476C10); }

/* 0x0033D7D0 */
void *Kind32_ModelFiles(void) {
    return D_00430A10;
}

/* 0x0033D7E0 */
void *Kind32_MotionFiles(void) {
    return D_00430A50;
}

/* 0x0033D800 */
void Kind32_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0033D850 */
void Kind32_EventState(Pursuer *p) { creature_act5(p, &D_00430A90); }

/* 0x0033D920 */
s32 Kind32_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern void *D_00479800[];
extern void *D_0047A050[];
extern void *D_0047A750[];

/* (as Room2AWisps_dtor)  (class D_00479800, room 0x2A) +0x8 destructor (the quad drawer at +0x610 inlined) */
/* 0x0035B3C0 */
u8 *BoneSmoke_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479800;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* (as Room2AWisps_dtor)  (class D_0047A050, room 0x2A) +0x8 destructor (the quad drawer at +0x610 inlined) */
/* 0x0036D360 */
u8 *ThingPuff_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0047A050;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* (as Effect78BC0_Update)  +0x10 update: turn */
/* 0x00377DD0 */
s32 Effect7A3D0_Update(u8 *o) {
    static const F32Bits kPi = {0x40490FDB}, kMinusPi = {0xC0490FDB}, k2Pi = {0x40C90FDB},
                         kStepX = {0x3C0EFA35}, kStepYZ = {0x3AE4C389};

    AT(o, 0x4, f32) = AT(o, 0x4, f32) + kStepX.f;
    if (!(AT(o, 0x4, f32) <= kPi.f)) {
        AT(o, 0x4, f32) = AT(o, 0x4, f32) - k2Pi.f;
    }
    AT(o, 0x8, f32) = AT(o, 0x8, f32) + kStepYZ.f;
    if (!(AT(o, 0x8, f32) <= kPi.f)) {
        AT(o, 0x8, f32) = AT(o, 0x8, f32) - k2Pi.f;
    }
    AT(o, 0xC, f32) = AT(o, 0xC, f32) - kStepYZ.f;
    if (AT(o, 0xC, f32) < kMinusPi.f) {
        AT(o, 0xC, f32) = AT(o, 0xC, f32) + k2Pi.f;
    }
    return 1;
}

/* (as Effect78BC0_Start)  (class D_00478BC0) +0xC reset: three random angles in -pi..pi */
/* 0x00377ED0 */
void Effect7A3D0_Start(u8 *o) {
    static const F32Bits kPi = {0x40490FDB};
    VObject *rng = gRandom;
    s32 k;

    for (k = 0; k < 3; k++) {
        f32 r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);

        AT(o, 0x4 + k * 4, f32) = kPi.f * (360.0f * (r - 0.5f)) / 180.0f;
    }
}

/* (as Effect78BC0_Start)  (class D_00478BC0) +0xC reset: three random angles in -pi..pi */
/* 0x003781F0 */
void BackdropModel_Start(u8 *o) {
    static const F32Bits kPi = {0x40490FDB};
    VObject *rng = gRandom;
    s32 k;

    for (k = 0; k < 3; k++) {
        f32 r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);

        AT(o, 0x4 + k * 4, f32) = kPi.f * (360.0f * (r - 0.5f)) / 180.0f;
    }
}

/* (as Effect78BC0_Start)  (class D_00478BC0) +0xC reset: three random angles in -pi..pi */
/* 0x00378750 */
void BackdropModel2_Start(u8 *o) {
    static const F32Bits kPi = {0x40490FDB};
    VObject *rng = gRandom;
    s32 k;

    for (k = 0; k < 3; k++) {
        f32 r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);

        AT(o, 0x4 + k * 4, f32) = kPi.f * (360.0f * (r - 0.5f)) / 180.0f;
    }
}

/* (as Room2AWisps_dtor)  (class D_0047A750, room 0x2A) +0x8 destructor (the quad drawer at +0x610 inlined) */
/* 0x0037D0A0 */
u8 *LightRing_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0047A750;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* ---- destructors of the same shape in other effect classes (one or two quad drawers inlined; generated from Room2AWisps_dtor / Room66Effect_dtor) ---- */

extern void *D_00474FB0[], *D_00476BB0[], *D_004795E0[], *D_00479A60[], *D_00479E50[], *D_0047A310[], *D_0047A350[];

static inline __attribute__((always_inline)) u8 *fx_dtor1(u8 *o, s32 flags, void **vt, u32 d0) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = vt;
    AT(o, d0, void **) = D_0046FC30;
    AT(o, d0, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

static inline __attribute__((always_inline)) u8 *fx_dtor2(u8 *o, s32 flags, void **vt, u32 d0, u32 d1) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = vt;
    AT(o, d1, void **) = D_0046FC30;
    AT(o, d1, void **) = D_00469D00;
    AT(o, d0, void **) = D_0046FC30;
    AT(o, d0, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* 0x00365F40 */
u8 *Spark_dtor(u8 *o, s32 flags) {
    return fx_dtor2(o, flags, D_00479E50, 0xD0, 0x108);
}

/* 0x003710C0 */
u8 *Embers_dtor(u8 *o, s32 flags) {
    return fx_dtor2(o, flags, D_0047A310, 0xF10, 0xF48);
}

/* 0x00374420 */
u8 *LorenzoSpark_dtor(u8 *o, s32 flags) {
    return fx_dtor1(o, flags, D_0047A350, 0xD0);
}

/* 0x00359220 */
u8 *Cr19Bubbles_dtor(u8 *o, s32 flags) {
    return fx_dtor2(o, flags, D_004795E0, 0x2410, 0x2448);
}

/* 0x0033BE00 */
u8 *DustShaft_dtor(u8 *o, s32 flags) {
    return fx_dtor1(o, flags, D_00476BB0, 0x6010);
}

/* 0x0033C110 */
void DustShaft_Draw(u8 *o) {
    quad_step(o, 0x6010, 0x7450, 0x3000);
}

/* 0x0032E890 */
u8 *ThingBurst_dtor(u8 *o, s32 flags) {
    return fx_dtor2(o, flags, D_00474FB0, 0xC10, 0xC48);
}

/* 0x0035F5B0 */
u8 *CeilingDrips_dtor(u8 *o, s32 flags) {
    return fx_dtor1(o, flags, D_00479A60, 0x550);
}

/* 0x0035F9B0 */
void CeilingDrips_Draw(u8 *o) {
    quad_step(o, 0x550, 0x5C0, 0x2A0);
}

/* (as Effect78BC0_Update, slower) */
/* 0x003780F0 */
s32 BackdropModel_Update(u8 *o) {
    return spin_step(o, 0x3A64C389, 0x393702D4);
}

/* 0x00378650 */
s32 BackdropModel2_Update(u8 *o) {
    return spin_step(o, 0x3AE4C389, 0x3A64C389);
}

#ifdef HG_NATIVE
#include <stdint.h>

/* +0xC draw: GAME_FIX.GFM model +0x30 (texture 2 of group 0x10, palettes the parts' +0x4 plus
 * +0x34) at +0x10 turned by +0x20; its later parts each turned and moved further by their
 * own +0x30 / +0x40. 0 when the texture isn't loaded. */
s32 func_0033B560(u8 *d) {
    VObject *tc = gTexCache;
    f32 m[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 rot[4] __attribute__((aligned(16)));
    f32 trans[4] __attribute__((aligned(16)));
    u8 *texh, *model, *part;
    s32 off, count, first = 1;

    if (VCALL(tc, 0x8, s32 (*)(VObject *, s32, s32))(tc, 2, 0x10) == -1) {
        return 0;
    }
    texh = VCALL(tc, 0xC, u8 *(*)(VObject *, s32, s32))(tc, 2, 0x10);
    model = VCALL(gSystemData, 0x1C, u8 *(*)(VObject *))((VObject *)gSystemData);
    off = AT(model, AT(d, 0x30, s32) * 4, s32);
    if (off <= 0) {
        return 1;
    }
    part = model + off;
    count = AT(part, 0x8, s32);
    while (count != 0) {
        count--;
        sceVu0UnitMatrix(m);
        if (first) {
            first = 0;
            sceVu0CopyVector(rot, (f32 *)(d + 0x20));
            sceVu0RotMatrix(m, m, rot);
            sceVu0CopyVector(trans, (f32 *)(d + 0x10));
        } else {
            sceVu0AddVector(rot, rot, (f32 *)(part + 0x30));
            sceVu0RotMatrix(m, m, rot);
            sceVu0AddVector(trans, trans, (f32 *)(part + 0x40));
        }
        sceVu0TransMatrix(m, m, trans);
        VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
        sceVu0MulMatrix(clip, clip, m);
        part = gl_gfm_part(part, clip, texh, AT(part, 0x4, s32) + AT(d, 0x34, s32));
        part = (u8 *)(((uintptr_t)part + 15) & ~(uintptr_t)15);
    }
    return 1;
}
#endif

/* a model draw `d` filled (+0x10 position, w 1; +0x20 angles; +0x30 / +0x34) and queued in
 * renderer layer `layer` */
void func_0033BCB0(u8 *d, const f32 *pos, const f32 *rot, s32 a, s32 b, s32 layer) {
    sceVu0CopyVector((f32 *)(d + 0x10), pos);
    AT(d, 0x1C, u32) = 0x3F800000;
    sceVu0CopyVector((f32 *)(d + 0x20), rot);
    AT(d, 0x30, s32) = a;
    AT(d, 0x34, s32) = b;
    VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, d, layer, 0);
}

/* ---- D_0047A310 (0x10C8 bytes): rising embers over a fire - 16 small ones (records +0x910 +
 * 0x300 x the current one +0x10C0, drawer +0xF48, rise speeds +0x1040, sway angles +0x1080)
 * and, when started with a non-zero word (+0x10C5), 24 large flames (records +0x10 + 0x480 x
 * the current one, drawer +0xF10, rise speeds +0xF80, sway angles +0xFE0); +0x10C4 off ---- */

#define EMBER_REC(o, buf, i) ((QuadRec *)((o) + 0x910 + (buf) * 0x300) + (i))
#define FLAME_REC(o, buf, i) ((QuadRec *)((o) + 0x10 + (buf) * 0x480) + (i))

static inline f32 rnd18(VObject *rnd) {
    return VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
}

static inline f32 rnd1C(VObject *rnd) {
    return VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
}

/* (re)start ember `i` within 10 of the centre at -4, 0.2..0.4 big, turned at random, rising
 * 0.2..0.6 a frame; not `fresh` (the first round): somewhere up its first 5, dimmer the
 * higher */
void func_00371180(u8 *o, s32 i, s32 fresh) {
    static const union { u32 u; f32 f; } k5 = {0x40A00000}, k02 = {0x3E4CCCCD}, k20 = {0x41A00000},
                                         k360 = {0x43B40000}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};   /* multiplied first */
    QuadRec *r;
    VObject *rnd;
    f32 y = 0.0f;

    if (AT(o, 0x10C4, u8) == 1) {
        return;
    }
    r = EMBER_REC(o, AT(o, 0x10C0, s32), i);
    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = 0x80;
    if (!fresh) {
        y = k5.f * rnd18(gRandom);
        r->rgba[3] = (s32)(k02.f * ((f32)r->rgba[3] * (5.0f - y)));
    }
    rnd = gRandom;
    r->pos[0] = k20.f * (rnd18(rnd) - 0.5f);
    r->pos[1] = y - 4.0f;
    r->pos[2] = k20.f * (rnd18(rnd) - 0.5f);
    r->pos[3] = 1.0f;
    r->w = 0x1.99999ap-3f + 0x1.99999ap-3f * rnd18(rnd);   /* 0.2 + 0.2 x */
    r->h = r->w;
    r->turn = kPi.f * (k360.f * (rnd1C(rnd) - 0.5f)) / 180.0f;
    r->frame = 0;
    AT(o, 0x1040 + i * 4, f32) = 0x1.99999ap-3f + 0x1.99999ap-2f * rnd18(rnd);   /* 0.2 + 0.4 x */
    AT(o, 0x1080 + i * 4, f32) = k2Pi.f * (rnd18(rnd) - 0.5f);
}

/* (re)start flame `i` within 5 of the centre at -16, 10..13 big, turned at random, rising
 * 0.6..0.65 a frame; not `fresh` (the first round): somewhere up its first 25, smaller and
 * dimmer the higher */
void func_00371400(u8 *o, s32 i, s32 fresh) {
    static const union { u32 u; f32 f; } k25 = {0x41C80000}, k004 = {0x3D23D70A}, k10 = {0x41200000},
                                         k360 = {0x43B40000}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};   /* multiplied first */
    QuadRec *r;
    VObject *rnd;
    f32 y = 0.0f;

    if (AT(o, 0x10C4, u8) == 1) {
        return;
    }
    r = FLAME_REC(o, AT(o, 0x10C0, s32), i);
    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = 0x40;
    rnd = gRandom;
    r->w = 10.0f + 3.0f * rnd18(rnd);
    if (!fresh) {
        y = k25.f * rnd18(rnd);
        r->rgba[3] = (s32)(k004.f * ((f32)r->rgba[3] * (25.0f - y)));
        r->w = r->w - 0.5f * y;
    }
    rnd = gRandom;
    r->pos[0] = k10.f * (rnd18(rnd) - 0.5f);
    r->pos[1] = y - 16.0f;
    r->pos[2] = k10.f * (rnd18(rnd) - 0.5f);
    r->pos[3] = 1.0f;
    r->h = r->w;
    r->turn = kPi.f * (k360.f * (rnd1C(rnd) - 0.5f)) / 180.0f;
    r->frame = 0;
    AT(o, 0xF80 + i * 4, f32) = 0x1.333334p-1f + 0x1.99999ap-5f * rnd18(rnd);   /* 0.6 + 0.05 x */
    AT(o, 0xFE0 + i * 4, f32) = k2Pi.f * (rnd18(rnd) - 0.5f);
}

/* +0x18 start: arg { flames too }: the drawers set up (flames 24 of a 32 x 32 cell at (384,
 * 128); embers 16 of 8 x 8 at (108, 76); blended, palette 6) and every one started; none: off */
/* 0x003716C0 */
void Embers_SetParams(u8 *o, s32 *arg) {
    s32 i;

    if (arg == NULL) {
        AT(o, 0x10C4, u8) = 1;
        return;
    }
    AT(o, 0x10C5, u8) = arg[0] != 0;
    if (AT(o, 0x10C5, u8) == 1) {
        AT(o, 0xF18, s64) = -1;
        AT(o, 0xF24, s32) = 0;
        AT(o, 0xF28, s32) = 0;
        AT(o, 0xF2C, s32) = 0;
        AT(o, 0xF30, s32) = 0x19;
        AT(o, 0xF34, s16) = 0x18;
        AT(o, 0xF36, s16) = 0x180;
        AT(o, 0xF38, s16) = 0x80;
        AT(o, 0xF3A, s16) = 0x20;
        AT(o, 0xF3C, s16) = 0x20;
        AT(o, 0xF3E, s16) = 0x200;
        AT(o, 0xF40, s16) = 0x100;
        AT(o, 0xF42, s8) = 0x40;
        AT(o, 0xF43, s8) = 1;
        AT(o, 0xF44, s8) = 1;
        AT(o, 0xF45, s8) = 0x10;
        AT(o, 0xF46, s8) = 6;
        for (i = 0; i < 24; i++) {
            func_00371400(o, i, 0);
        }
    }
    AT(o, 0xF50, s64) = -1;
    AT(o, 0xF5C, s32) = 0;
    AT(o, 0xF60, s32) = 0;
    AT(o, 0xF64, s32) = 0;
    AT(o, 0xF68, s32) = 0x19;
    AT(o, 0xF6C, s16) = 0x10;
    AT(o, 0xF6E, s16) = 0x6C;
    AT(o, 0xF70, s16) = 0x4C;
    AT(o, 0xF72, s16) = 8;
    AT(o, 0xF74, s16) = 8;
    AT(o, 0xF76, s16) = 0x200;
    AT(o, 0xF78, s16) = 0x100;
    AT(o, 0xF7A, s8) = 0x40;
    AT(o, 0xF7B, s8) = 1;
    AT(o, 0xF7C, s8) = 1;
    AT(o, 0xF7D, s8) = 0x10;
    AT(o, 0xF7E, s8) = 6;
    for (i = 0; i < 16; i++) {
        func_00371180(o, i, 0);
    }
}

/* +0x14 draw: the flames (if any), then the embers */
/* 0x00371860 */
void Embers_Draw(u8 *o) {
    if (AT(o, 0x10C4, u8) == 1) {
        return;
    }
    if (AT(o, 0x10C5, u8) == 1) {
        AT(o, 0xF20, u8 *) = o + AT(o, 0x10C0, s32) * 0x480 + 0x10;
        func_002E56C0(o + 0xF10);
    }
    AT(o, 0xF58, u8 *) = o + AT(o, 0x10C0, s32) * 0x300 + 0x910;
    func_002E56C0(o + 0xF48);
}

/* one sway step: the angle on by up to 10 degrees (wrapped), returned */
static inline f32 ember_sway(f32 *a, VObject *rnd) {
    static const union { u32 u; f32 f; } k10 = {0x41200000}, kPi = {0x40490FDB};   /* multiplied first */
    f32 v = *a + kPi.f * (k10.f * rnd18(rnd)) / 180.0f;

    *a = v;
    if (!(v <= 0x1.921fb6p+1f)) {
        *a = v - 0x1.921fb6p+2f;
    }
    return *a;
}

/* +0x10 update: flip the buffers; the flames shrink (out at 0), turn, sway (0.1) and rise,
 * fading 2..3 every other frame and restarted once faded; the embers turn, sway (0.2) and rise,
 * fading 7..10 every other frame and restarted once faded. 0 once off */
/* 0x00371900 */
s32 Embers_Update(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};   /* multiplied first */
    VObject *rnd;
    s32 i, k;

    if (AT(o, 0x10C4, u8) == 1) {
        return 0;
    }
    AT(o, 0x10C0, s32) ^= 1;
    if (AT(o, 0x10C5, u8) == 1) {
        rnd = gRandom;
        for (i = 0; i < 24; i++) {
            u32 *src = (u32 *)FLAME_REC(o, AT(o, 0x10C0, s32) ^ 1, i);
            u32 *dst = (u32 *)FLAME_REC(o, AT(o, 0x10C0, s32), i);
            QuadRec *r;
            f32 a;

            for (k = 0; k < 12; k++) {
                dst[k] = src[k];
            }
            r = FLAME_REC(o, AT(o, 0x10C0, s32), i);
            r->w = r->w - (0x1.99999ap-3f + 0x1.99999ap-4f * rnd18(rnd));   /* 0.2 + 0.1 x */
            if (r->w < 0.0f) {
                r->w = 0.0f;
                r->rgba[3] = 0;
            }
            r->h = r->w;
            r->turn = r->turn + kPi.f * rnd1C(rnd) / 180.0f;
            a = ember_sway(&AT(o, 0xFE0 + i * 4, f32), rnd);
            r->pos[0] = r->pos[0] + 0x1.99999ap-4f * func_0031C248(a);
            r->pos[1] = r->pos[1] + AT(o, 0xF80 + i * 4, f32);
            r->pos[2] = r->pos[2] + 0x1.99999ap-4f * func_0031C058(AT(o, 0xFE0 + i * 4, f32));
            if (AT(o, 0x10C0, s32) == 0) {
                r->rgba[3] = r->rgba[3] - ((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 1) + 2);
                if (r->rgba[3] < 0) {
                    func_00371400(o, i, 1);
                }
            }
        }
    }
    rnd = gRandom;
    for (i = 0; i < 16; i++) {
        u32 *src = (u32 *)EMBER_REC(o, AT(o, 0x10C0, s32) ^ 1, i);
        u32 *dst = (u32 *)EMBER_REC(o, AT(o, 0x10C0, s32), i);
        QuadRec *r;
        f32 a;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        r = EMBER_REC(o, AT(o, 0x10C0, s32), i);
        r->turn = r->turn + 0.5f * (kPi.f * rnd1C(rnd) / 180.0f);
        a = ember_sway(&AT(o, 0x1080 + i * 4, f32), rnd);
        r->pos[0] = r->pos[0] + 0x1.99999ap-3f * func_0031C248(a);
        r->pos[1] = r->pos[1] + AT(o, 0x1040 + i * 4, f32);
        r->pos[2] = r->pos[2] + 0x1.99999ap-3f * func_0031C058(AT(o, 0x1080 + i * 4, f32));
        if (AT(o, 0x10C0, s32) == 0) {
            r->rgba[3] = r->rgba[3] - ((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 3) + 7);
            if (r->rgba[3] < 0) {
                func_00371180(o, i, 1);
            }
        }
    }
    return 1;
}

/* 0x00371E00 */
void Embers_Start(u8 *o) {
    AT(o, 0x10C0, s32) = 0;
    AT(o, 0x10C4, u8) = 0;
}

/* ---- D_00479800 (room 0x2A): 16 wisps of smoke rising from a character's bone (+0x6C8; none:
 * Fiona's bone 0x23, 3.5 to the side and 1 forward), one more each frame (+0x6D0 counting), in
 * two buffers of quad records (+0x10 + 0x300 x the current one +0x6CC), sideways drifts at
 * +0x648 (x, z), the quad drawer at +0x610; each fades in to 0x10, then out on the even
 * frames ---- */

#define WISPS_REC(o, buf, i) ((QuadRec *)((o) + 0x10 + (buf) * 0x300) + (i))

/* wisp `i` (re)started at `p`: hidden, 0.4..0.6 wide and 1.2 high, drifting up to 0.015 a frame */
void func_0035B450(u8 *o, s32 i, f32 *p) {
    static const union { u32 u; f32 f; } k003 = {0x3CF5C28F};   /* 0.03, multiplied first */
    QuadRec *r = WISPS_REC(o, AT(o, 0x6CC, s32), i);
    VObject *rnd = gRandom;

    r->rgba[0] = 0x80;
    r->rgba[1] = 0x80;
    r->rgba[2] = 0x80;
    r->rgba[3] = 0;
    r->pos[0] = p[0];
    r->pos[1] = p[1];
    r->pos[2] = p[2];
    r->pos[3] = 1.0f;
    r->w = 0x1.99999ap-2f + 0x1.99999ap-3f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);   /* 0.4 + 0.2 x */
    r->h = 0x1.333334p+0f;   /* 1.2 */
    r->turn = 0.0f;
    r->frame = 0;
    AT(o, 0x648 + i * 8, f32) = k003.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    AT(o, 0x64C + i * 8, f32) = k003.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
}

/* where the smoke comes from: the character's bone (its +0x98), or Fiona's bone 0x23 */
static inline void wisps_source(u8 *o, f32 *p) {
    u8 *c = AT(o, 0x6C8, u8 *);

    if (c != NULL) {
        s32 bone = VCALL(c, 0x98, s32 (*)(u8 *))(c);

        sceVu0CopyVector(p, Skel_Bone(AT(c, 0x810, void *), bone) + 12);
    } else {
        f32 m[4][4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));

        sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(AT(AT(gCharPlayer, 0xF0, u8 *), 0x810, void *), 0x23));
        v[0] = -3.5f;
        v[2] = 1.0f;
        v[3] = 1.0f;
        v[1] = 0.0f;
        sceVu0ApplyMatrix(p, m, v);
    }
}

/* +0x18 start: from character `c` (none: Fiona), all 16 placed there hidden */
/* 0x0035B5C0 */
void BoneSmoke_SetParams(u8 *o, u8 *c) {
    f32 p[4] __attribute__((aligned(16)));
    s32 i;

    AT(o, 0x6C8, u8 *) = c;
    wisps_source(o, p);
    for (i = 0; i < 16; i++) {
        func_0035B450(o, i, p);
    }
}

/* +0x14 draw (not while the effects are paused) */
/* 0x0035B6A0 */
void BoneSmoke_Draw(u8 *o) {
    if (func_002D6010(gEffects) == 0) {
        AT(o, 0x620, QuadRec *) = WISPS_REC(o, AT(o, 0x6CC, s32), 0);
        func_002E56C0(o + 0x610);
    }
}

/* +0x10 update: flip the buffers; each wisp showing carried over, growing up to 0.04 a frame,
 * drifting and rising 0.02; fading in by 3..6 to 0x10 (then marked, its red 0x7F), then out by
 * 0..1 on the even frames. The first 16 frames one more is restarted at the source, shown. 0
 * once none shows */
/* 0x0035B700 */
s32 BoneSmoke_Update(u8 *o) {
    VObject *rnd = gRandom;
    u8 done = 1;
    s32 i, k;

    AT(o, 0x6CC, s32) ^= 1;
    for (i = 0; i < 16; i++) {
        QuadRec *r;

        for (k = 0; k < 12; k++) {
            ((u32 *)WISPS_REC(o, AT(o, 0x6CC, s32), i))[k] = ((u32 *)WISPS_REC(o, AT(o, 0x6CC, s32) ^ 1, i))[k];
        }
        r = WISPS_REC(o, AT(o, 0x6CC, s32), i);
        if (r->rgba[3] <= 0) {
            continue;
        }
        done = 0;
        r->w = r->w + 0x1.47ae14p-5f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);   /* 0.04 */
        r->h = r->h + 0x1.47ae14p-5f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        r->pos[0] = r->pos[0] + AT(o, 0x648 + i * 8, f32);
        r->pos[1] = r->pos[1] + 0x1.47ae14p-6f;   /* 0.02 */
        r->pos[2] = r->pos[2] + AT(o, 0x64C + i * 8, f32);
        if (r->rgba[0] == 0x80) {
            r->rgba[3] = r->rgba[3] + ((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 3) + 3);
            if (r->rgba[3] >= 0x10) {
                r->rgba[0]--;
                r->rgba[3] = 0x10;
            }
        } else if (AT(o, 0x6CC, s32) == 0) {
            r->rgba[3] = r->rgba[3] - (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 1);
            if (r->rgba[3] < 0) {
                r->rgba[3] = 0;
            }
        }
    }
    if (AT(o, 0x6D0, s32) < 16) {
        f32 p[4] __attribute__((aligned(16)));

        done = 0;
        wisps_source(o, p);
        func_0035B450(o, AT(o, 0x6D0, s32), p);
        WISPS_REC(o, AT(o, 0x6CC, s32), AT(o, 0x6D0, s32))->rgba[3] = 1;
    }
    AT(o, 0x6D0, s32)++;
    return done == 1 ? 0 : 1;
}

/* +0xC set up: frame 0; the drawer (16 quads of a 32 x 32 cell at (0 or 32 at random, 64),
 * blended, the first palette, layer 0x19, half a unit down) */
/* 0x0035BA60 */
void BoneSmoke_Start(u8 *o) {
    AT(o, 0x6CC, s32) = 0;
    AT(o, 0x6D0, s32) = 0;
    AT(o, 0x618, s64) = -1;
    AT(o, 0x628, s32) = 0;
    AT(o, 0x62C, f32) = -0.5f;
    AT(o, 0x630, s32) = 0x19;
    AT(o, 0x634, s16) = 0x10;
    AT(o, 0x636, s16) = (VCALL(gRandom, 0x10, u32 (*)(VObject *))(gRandom) & 1) << 5;
    AT(o, 0x638, s16) = 0x40;
    AT(o, 0x63A, s16) = 0x20;
    AT(o, 0x63C, s16) = 0x20;
    AT(o, 0x63E, s16) = 0x200;
    AT(o, 0x640, s16) = 0x100;
    AT(o, 0x642, s8) = 0x40;
    AT(o, 0x643, s8) = 1;
    AT(o, 0x644, s8) = 1;
    AT(o, 0x645, s8) = 0x10;
    AT(o, 0x646, s8) = -1;
}
