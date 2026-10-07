/* Hewie: the partner dog (vtable 0x46A120).
 *
 * (was hewie_act.c) Hewie's actions (Hewie_SetAction): taking up action `act` sets what he does
 * next - his behaviour (a pointer to member, HEWIE_STATE), his animation group (+0xF3604, changed
 * with a 10-frame blend +0xF3608), timers (+0xF355C, +0xF36B4..), his target (+0xF3544) and
 * command flags (+0xF356C). An action he can't do (the target out of sight, a floor that doesn't
 * allow it) passes on to the one Hewie_AdjustAction picks instead.
 *
 * (was hewie_model.c) Hewie's model (vtable DogModel_vtable, 0xB90 bytes, built by
 * CharLoad_PartnerModel in model.c; two subclasses DogModelA_vtable / DogModelB_vtable differ
 * only in their destructors). On top of the model base it fits his body to the floor as a dog
 * stands - a back frame +0x7D0 from a point behind him to his position, a front frame +0xB40 from
 * his position to a point ahead - and plants his four feet with IK: the front legs on three-bone
 * solvers (+0x960, +0x9F0), the hind legs on two-bone ones (+0xA80, +0xAE0). The feet (front
 * right/left, hind right/left; bones 11, 15, 23, 28): +0x8A0 [4] where each foot is put (vectors)
 * +0x8E0 [4] held where it was planted this frame (+0x8E4 last frame's) +0x8F0 [4] when a held
 * foot lets go: its offset from the animated foot, eased out ... +0x930 [4] ... over this many
 * frames +0x940 [4] frames of easing left +0x950 the feet have been placed once +0xB80 [4] the
 * animation has the foot on the ground (+0xB84 last frame's) +0xB88 how far the spine bends with
 * the slope (0..1, while the motion's flag 0x800) +0x854 / +0x858 how much he is turning / the
 * slope, for the spine and neck
 */
#include "common.h"
#include "hewie.h"
#include "progress.h"
#include "navmesh.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "actor.h"
#include "ptmf.h"
#include "fiona.h"
#include "model.h"
#include "pursuer.h"
#include "scene_game.h"
#include "heap.h"
#include "vecmath.h"
#include "libc.h"
#include "msl.h"
#include "game.h"
#include "memcard.h"
#include "event.h"
#include "lights.h"
#include "sound.h"
#include "effectmgr.h"
#include "debilitas.h"
#include "input.h"
#ifdef HG_NATIVE
#include <stdio.h>
#include <stdlib.h>
#include "glr.h"
#endif

#define MOTION_U8(m, off) (*((u8 *)(m) + (off)))
#define MOTION_ANIM(m) (*(s32 *)((u8 *)(m) + 0x55C))
#define SLOT_U8(h) (*(u8 *)&(h)->c.a.slot)

extern void *Hewie_vtable[];   /* Hewie vtable */
extern void *Character_vtable[];   /* Character vtable */
extern void *Actor_vtable[];   /* Actor base vtable */

extern const s32 D_003B13D0[8];
#define RNG01() VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)

#define STATE(h, fn) ptmf_set_fn(HEWIE_STATE(h), (void *)(fn))

#define TARGET(h) HW(h, 0xF3544, Character *)

#define WAIT(h) HW(h, 0xF355C, s32)

#define CMD(h) HW(h, 0xF356C, s32)

#define MODE(h) HW(h, 0xF8, s32)

extern void *DogModel_vtable[], *DogModelA_vtable[], *DogModelB_vtable[];
extern u8 D_003D5F90[];
extern u8 D_00456EB0[];
extern u8 D_00456F70[];
extern u8 str_O_HEG_HEG_004_PCK[], str_O_HEG_HEG_003_PCK[], str_O_HEG_HEG_002_PCK[], str_O_HEG_HEG_001_PCK[], D_00456F50[];
extern u8 str_O_HED_HED_004_PCK[], str_O_HED_HED_003_PCK[], str_O_HED_HED_002_PCK[], str_O_HED_HED_001_PCK[], D_00456E90[];
#define SKEL(m) AT(m, 0x810, void *)

/* bind pose: a bone's length (+0x20) and position (+0x60) in the skeleton data +0x4C0 */
#define BIND(m, bone, off) AT(AT(m, 0x4C0, u8 *) + 0x10 + (bone) * 0x70, off, f32)

static const s32 sFootBones[4] = { 0xB, 0xF, 0x17, 0x1C };

extern void *IK3_vtable[];
void *IK2_ctor(u8 *p);
extern const char D_00456360[], str_O_HEW_HEW_001_PCK[], str_O_HEW_HEW_002_PCK[], str_O_HEW_HEW_003_PCK[], str_O_HEW_HEW_004_PCK[];   /* HEW_00n.PCK */
extern const char D_004562C0[], str_O_HEW_HEW_001_MRK[], str_O_HEW_HEW_002_MRK[], str_O_HEW_HEW_003_MRK[], str_O_HEW_HEW_004_MRK[];   /* HEW_00n.MRK */
extern const char D_00456400[];   /* O_HEW\HEW_000.TEX */
extern void *IK2_ctor(u8 *p);
/* the dog model's constructor on the full base (ModelBase_ctor), kind `kind`, vtable `vtbl` */
static inline void *dog_model(u8 *m, u8 kind, void **vtbl) {
    ModelBase_ctor(m);
    AT(m, 0x0, void **) = DogModel_vtable;
    AT(m, 0x890, u8) = kind;
    __construct_array(m + 0x960, DogModelArray_ctor, IK3_Destroy, 0x90, 2);
    __construct_array(m + 0xA80, IK2_ctor, IK2_Destroy, 0x60, 2);
    AT(m, 0x0, void **) = vtbl;
    return m;
}

void *DogModelArray_ctor(void *p);
const char *DogModel_ModelFile(void *m, s32 n);
const char *DogModel_MarkerFile(void *m, s32 n);
const char *DogModel_Textures(void);

static void HewieModel_Destroy(u8 *m, s32 flags) {
    AT(m, 0x0, void **) = DogModel_vtable;
    __destroy_arr(m + 0xA80, IK2_Destroy, 0x60, 2);
    __destroy_arr(m + 0x960, IK3_Destroy, 0x90, 2);
    AT(m, 0x0, void **) = Model_vtable;
    AT(m, 0x0, void **) = ModelBase_vtable;
    AT(m, 0x1D0, void **) = Shadow_vtable;
    AT(m, 0x1D0, void **) = Helper469D00_vtable;
    AT(m, 0x10, void **) = ModelDrawer_vtable;
    AT(m, 0x10, void **) = Helper469D00_vtable;
    if ((s16)flags > 0) {
        StalkerModel_delete(m);
    }
}

/* foot `i`'s position in the animation (copied out with its bone's matrix) */
static void foot_at(u8 *m, s32 i, f32 *out) {
    f32 b[4][4] __attribute__((aligned(16)));

    sceVu0CopyMatrix(b, (f32 (*)[4])Skel_Bone(SKEL(m), sFootBones[i]));
    sceVu0CopyVector(out, b[3]);
}

/* fit the body to the floor at all: some reach asked for, and he is on the floor (within
   1e-4 of it, or below it) at `floor` */
static s32 on_floor(u8 *a, const f32 *floor, f32 ahead, f32 behind) {
    f32 d;

    if (ahead == 0.0f && behind == 0.0f) {
        return 0;
    }
    d = AT(a, 0x14, f32) - floor[1];
    if (!(d <= 0.0f)) {
    } else {
        d = -d;
    }
    return d <= 0x1.a36e2e0000000p-14f /* 0.0001 */ || AT(a, 0x14, f32) <= floor[1];
}

/* the floor point `along` ahead on character `a`'s matrix `mtx` */
static void floor_point(u8 *m, u8 *a, f32 (*mtx)[4], f32 *p, f32 along) {
    p[0] = 0.0f;
    p[1] = 0.0f;
    p[2] = along;
    p[3] = 1.0f;
    sceVu0ApplyMatrix(p, mtx, p);
    Motion_OntoFloor(m, p, a);
}

/* a frame along `dir` (unit; its Z), level sideways (X = (dir.z, 0, -dir.x)) */
static void frame_along(f32 (*f)[4], const f32 *dir) {
    f32 x[4] __attribute__((aligned(16)));

    sceVu0UnitMatrix(f);
    x[0] = dir[2];
    x[1] = 0.0f;
    x[2] = -dir[0];
    x[3] = 0.0f;
    sceVu0Normalize(f[0], x);
    vu0_CopyXYZ(f[2], dir);
    sceVu0OuterProduct(f[1], (f32 *)dir, f[0]);
}

/* bone matrix `b` turned by `a1` about `axis1` then `a2` about `axis2` (about its own
   position: the turn carries the bone's translation) */
static void bone_turn(f32 (*b)[4], const f32 *axis1, f32 a1, const f32 *axis2, f32 a2) {
    f32 q[4] __attribute__((aligned(16))) = { 0 };
    f32 r1[4][4] __attribute__((aligned(16)));
    f32 r2[4][4] __attribute__((aligned(16)));
    f32 r[4][4] __attribute__((aligned(16)));

    Quat_FromAxisAngle(q, axis1, a1);
    Quat_ToMatrix(q, r1);
    Quat_FromAxisAngle(q, axis2, a2);
    Quat_ToMatrix(q, r2);
    sceVu0MulMatrix(r, r2, r1);
    sceVu0CopyVector(r[3], b[3]);
    b[3][0] = 0.0f;
    b[3][1] = 0.0f;
    b[3][2] = 0.0f;
    sceVu0MulMatrix(b, r, b);
}

/* the neck / head: turned with his turning (`k` x +0x854) about the axis square to the body's
 * up (+0xB50) and bone `ref`'s level Z, and with the slope (-0.25 x +0x858) about the up */
static void neck_turn(u8 *m, f32 (*b)[4], s32 ref, f32 k) {
    f32 side[4] __attribute__((aligned(16)));
    f32 up[4] __attribute__((aligned(16)));
    f32 fwd[4] __attribute__((aligned(16)));
    f32 *r = Skel_Bone(SKEL(m), ref);

    sceVu0CopyVector(fwd, r + 8);
    fwd[1] = 0.0f;
    if (sceVu0InnerProduct(r + 4, (f32 *)(m + 0xB50)) < 0.0f) {
        sceVu0ScaleVector(fwd, fwd, -1.0f);
    }
    sceVu0CopyVector(up, (f32 *)(m + 0xB50));
    sceVu0OuterProduct(side, up, fwd);
    sceVu0OuterProduct(fwd, side, up);
    sceVu0Normalize(side, side);
    sceVu0Normalize(up, up);
    sceVu0Normalize(fwd, fwd);
    bone_turn(b, side, k * AT(m, 0x854, f32), up, 0.25f * -AT(m, 0x858, f32));
}

void *DogModel_dtor(u8 *m, s32 flags);
void *DogModelA_dtor(u8 *m, s32 flags);
void *DogModelB_dtor(u8 *m, s32 flags);
u8 *DogModelA_KindTable(u8 *m, s32 k);
void *DogModelA_Table(void);
u8 *DogModelB_KindTable(u8 *m, s32 k);
void *DogModelB_Table(void);
void DogModel_Loaded(u8 *m);
void DogModel_Frame(u8 *m);
void DogModel_SecondaryMotion(u8 *m);
s32 DogModel_HeadBone(u8 *m);
void DogModel_HeadPos(u8 *m, f32 *out);
void DogModel_SetMatrix(u8 *m, f32 (*mtx)[4]);
s32 DogModel_Part0(u8 *m);
s32 DogModel_Part1(u8 *m);
s32 DogModel_Part2(u8 *m);
s32 DogModel_Part3(u8 *m);
void DogModel_HalfHip(u8 *m, f32 *out);
u8 DogModel_FootDown(u8 *m, s32 foot, s32 dt);
void DogModel_FeetContact(u8 *m);
void DogModel_FeetUp(u8 *m);
void DogModel_FeetReplace(u8 *m);
void DogModel_LegIK(u8 *m, f32 *fr, f32 *fl, f32 *hr, f32 *hl);
void DogModel_PlantFeet(u8 *m);
void DogModel_BodyFrames(u8 *m, u8 *a, f32 ahead, f32 behind);
f32 DogModel_FloorLevel(u8 *m, u8 *a, f32 ahead, f32 behind);
void DogModel_AdjustBone(u8 *m, s32 bone, f32 (*b)[4], f32 (*parent)[4]);

/* his animation group (blended in over 10 frames when it changes) */
static inline void anim(Hewie *h, s32 a) {
    if (HW(h, 0xF3604, s32) != a) {
        HW(h, 0xF3604, s32) = a;
        HW(h, 0xF3608, s32) = 0xA;
    }
}

/* the action picked instead of `act` (`arg` if it is `act` itself) */
static inline void instead(Hewie *h, s32 act, s32 arg) {
    s32 r = Hewie_AdjustAction(h, act);

    if (r != act) {
        Hewie_SetAction(h, r, 0);
    } else {
        Hewie_SetAction(h, act, arg);
    }
}

/* nothing to do: wander (6) while the game allows, else idle */
static inline void give_up(Hewie *h) {
    if (AT(gProgress, 0x1FBEC1, u8) == 0) {
        instead(h, 6, 0);
    } else {
        instead(h, 0, 0);
    }
}

static inline s32 sees(Hewie *h, void *who) {
    return (Hewie_WithChar2(h, who) & 0xFF) == 1;
}

/* the flags of his walk-mesh triangle (+0x34) */
static inline u32 tri_flags(Hewie *h) {
    u32 t = HW(h, 0x34, u32);
    u8 *tri = NULL;

    if (t < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
        tri = AT(gNavMesh, 0x4, u8 *) + t * 0x50;
    }
    return tri != NULL ? AT(tri, 0x3C, u32) : 0;   /* (the original reads address 0x3C) */
}

static inline void path_clear(Hewie *h) {
    u32 i;

    for (i = 0; i < 0xD; i++) {
        HW(h, 0x148C + i * 4, s32) = 0;
    }
}

/* the door he takes: the path's next door (+0x138C) and its distance */
static inline void door_from_path(Hewie *h) {
    HW(h, 0x14C0, u16) = HW(h, 0x138C, u16);
    HW(h, 0x14C4, f32) = (f32)VCALL(gRooms, 0x38, s32 (*)(VObject *, u32, s32))(gRooms, HW(h, 0x138C, u16),
                                                                                      h->c.a.room);
}

/* an exit of his room he may go through: 8 tries at random */
static inline s32 exit_random(Hewie *h, u8 *seen) {
    u8 e = (u8)(u32)(8.0f * RNG01());

    while (*seen & (1 << e)) {
        e = (e + 1) & 0xFF;
        if (e == 8) {
            e = 0;
        }
    }
    *seen |= 1 << e;
    return e;
}

static inline s32 exit_open(Hewie *h, u32 e) {
    Progress *p = gProgress;

    return (Progress_ExitOpen(p, h->c.a.room, e) & 0xFF) == 1 && !(Progress_ExitUnlocked(p, h->c.a.room, e) & 0xFF)
        && (Progress_ExitPassable(p, h->c.a.room, e, HW(h, 0x20, u8)) & 0xFF) == 1;
}

/* his chasing settings: turn speed +0xF36C8 from his speed choice */
static inline void turn_speed(Hewie *h) {
    switch (HW(h, 0xF36BC, s32)) {
    case 0:
        HW(h, 0xF36C8, f32) = 20.0f;
        break;
    case 1:
        HW(h, 0xF36C8, f32) = 25.0f;
        break;
    case 2:
        HW(h, 0xF36C8, f32) = 30.0f;
        break;
    }
}

/* toward a place in the room (rooms +0x30) */
static inline void to_place(Hewie *h, u32 place) {
    f32 at[4] __attribute__((aligned(16)));

    MODE(h) = 0;
    HW(h, 0x100, s32) = place;
    HW(h, 0xF36B4, s32) = VCALL(gRooms, 0x30, s32 (*)(VObject *, u32, f32 *))(gRooms, place, at);
    HW(h, 0xF36B8, s32) = 0;
    HW(h, 0xF36BC, s32) = Hewie_RandomLevel(h);
    sceVu0CopyVector((f32 *)((u8 *)h + 0xF36E0), at);
    HW(h, 0x124, s32) = HW(h, 0x128, s32);
    CMD(h) = 0x78D;
}

/* turn to face `who`'s heading (+0x100 a character), else the place progress +0x1060 */
static inline void face(Hewie *h) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 heading;

    if (HW(h, 0x100, s32) != 0xFF) {
        anim(h, 0);
        TARGET(h) = gCharacters[HW(h, 0x100, s32)];
        heading = TARGET(h)->a.angle[1];
    } else {
        anim(h, 4);
        sceVu0CopyVector(at, (f32 *)((u8 *)gProgress + (HW(h, 0x20, u8) << 5) + 0x1060));
        sceVu0SubVector(d, h->c.a.pos, at);
        heading = msl_atan2f(d[0], d[2]);
    }
    HW(h, 0xF36C4, f32) = heading;
}

/* the timers by his trust (+0xF35CC) */
static const s32 sWait[8] = {300, 360, 420, 480, 540, 600, 660, 720};

/* vtable +0x8: destructor (nothing to free: he lives inside the scene). */
/* 0x00130A70 */
Hewie *Hewie_dtor(Hewie *h, s32 flags) {
    if (h != NULL) {
        h->c.a.vtbl = Hewie_vtable;
        h->c.a.vtbl = Character_vtable;
        h->c.a.vtbl = Actor_vtable;
        if ((s16)flags > 0) {
            Actor_Destroy(&h->c.a);
        }
    }
    return h;
}

/* vtable +0x10 */
/* 0x00168A00 */
void Hewie_Cleanup(Hewie *h) {
}

/* vtable +0x58: deactivate (Character part only). */
/* 0x00166140 */
void Hewie_Deactivate(Hewie *h) {
    Character_Deactivate(&h->c);
}

/* vtable +0x98 */
/* 0x00130AE0 */
s32 Hewie_IsBusy(Hewie *h) {
    return 1;
}

/* 0x00130AF0 */
void Hewie_SetAction(Hewie *h, s32 act, s32 arg) {
    s32 a;

    HEWIE_ACTION(h) = act;
    HW(h, 0xF3570, s32) = arg;
    HW(h, 0xF3559, s8) = 0;
    if (HEWIE_ACTION(h) != 0x76 && HEWIE_ACTION(h) != 0x77 && HEWIE_ACTION(h) != 0) {
        HW(h, 0xF35B4, s32) = -1;
    }
    WAIT(h) = (s32)(10.0f * RNG01()) * 30 + 90;
    a = HEWIE_ACTION(h);
    switch (a) {
    case 0x0:   /* idle */
        HW(h, 0xF3548, void *) = NULL;
        MODE(h) = 0;
        CMD(h) = 0;
        if (HW(h, 0xE0, u8) == 1) {
            anim(h, 4);
            STATE(h, Hewie_State1D88);
        } else {
            HW(h, 0x2D, u8) = 0;
            HW(h, 0xF3559, s8) = 1;
            anim(h, 4);
            STATE(h, Hewie_State1D98);
        }
        return;
    case 0x2:   /* follow Fiona */
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            WAIT(h) = sWait[HW(h, 0xF35CC, s16)];
            CMD(h) = 0x688;
            STATE(h, Hewie_State1DC8);
        } else {
            instead(h, 4, 0);
        }
        break;
    case 0x7:
        MODE(h) = 0;
        WAIT(h) = sWait[HW(h, 0xF35CC, s16)];
        TARGET(h) = Hewie_PickTarget(h);
        CMD(h) = 0x58C;
        STATE(h, Hewie_StateSteer);
        break;
    case 0x1:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            CMD(h) = 0x6AE;
            STATE(h, Hewie_State1DC8);
        } else {
            instead(h, 4, 0);
        }
        break;
    case 0x3:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            WAIT(h) = HW(h, 0xF3560, s32);
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            CMD(h) = 0x688;
            STATE(h, Hewie_State1DC8);
        } else {
            instead(h, 4, 0);
        }
        break;
    case 0x4:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x7AD;
        STATE(h, Hewie_StatePose4);
        return;
    case 0x5: {
        s32 r;

        MODE(h) = 0;
        r = (s32)(45.0f * RNG01());
        if (r < 0x14) {
            anim(h, 1);
        } else if (r < 0x19) {
            anim(h, 0xD);
        } else if (r < 0x1E) {
            anim(h, 0xB);
        } else {
            anim(h, 0xC);
        }
        CMD(h) = 0x7AD;
        STATE(h, Hewie_StatePose4);
        return;
    }
    case 0x14:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_State1E48);
        return;
    case 0x6:
        MODE(h) = 0;
        WAIT(h) = 0x5A;
        TARGET(h) = Hewie_PickTarget(h);
        CMD(h) = 0x6AF;
        STATE(h, Hewie_StateFaceScent);
        break;
    case 0x8:
        TARGET(h) = Hewie_PickTarget(h);
        if (HW(h, 0xF366D, u8) == 2 || sees(h, TARGET(h))) {
            WAIT(h) = 0x96;
            MODE(h) = 0;
            CMD(h) = 0x7AF;
            STATE(h, Hewie_StateFaceTarget);
        } else {
            instead(h, 6, 0);
        }
        break;
    case 0xA:
        if (HW(h, 0xF3548, void *) == NULL) {
            HW(h, 0xF3548, void *) = Hewie_PickTarget(h);
        }
        if (!sees(h, HW(h, 0xF3548, void *))) {
            instead(h, 6, 0);
        } else {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = (s32)(2.0f * RNG01()) + 1;
            TARGET(h) = HW(h, 0xF3548, Character *);
            CMD(h) = 0x7AF;
            STATE(h, Hewie_StateBarkAtTarget);
        }
        break;
    case 0x9:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            WAIT(h) = 0x12C;
            CMD(h) = 0x384;
            STATE(h, Hewie_StateWaitForFiona);
        } else {
            instead(h, 6, 0);
        }
        break;
    case 0xB:
        if (!sees(h, gCharPlayer)) {
            instead(h, 6, 0);
        } else {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = (s32)(3.0f * RNG01()) + 3;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            CMD(h) = 0x384;
            STATE(h, Hewie_StateBarkAtFiona);
        }
        break;
    case 0xC:
    case 0xD:
    case 0xE:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 2;
            CMD(h) = 0x68F;
            STATE(h, Hewie_StateSlideToFiona);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0xF:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 2;
            CMD(h) = 0x58F;
            STATE(h, Hewie_StateSlideToFiona);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x10:
        if (!sees(h, gCharPlayer)) {
            instead(h, 0, 0);
        } else {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 0;
            HW(h, 0xF36BC, s32) = Hewie_RandomLevel(h);
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            turn_speed(h);
            TARGET(h) = gCharPlayer;
            WAIT(h) = (s32)(3.0f * RNG01()) * 30 + 30;
            CMD(h) = 0x78D;
            STATE(h, Hewie_StateKeepAway);
        }
        break;
    case 0x11:
        TARGET(h) = Hewie_PickTarget(h);
        if (sees(h, TARGET(h))) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 0;
            HW(h, 0xF36BC, s32) = 2;
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            HW(h, 0xF36C8, f32) = 30.0f;
            WAIT(h) = 0x1E;
            CMD(h) = 0x78D;
            STATE(h, Hewie_StateKeepAway);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x12:
        TARGET(h) = Hewie_PickTarget(h);
        if (sees(h, TARGET(h))) {
            MODE(h) = 0;
            anim(h, 0);
            HW(h, 0xF36B4, s32) = -1;
            HW(h, 0xF36B8, s32) = 0;
            CMD(h) = 0x48F;
            STATE(h, Hewie_StateFlank);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x13:
    case 0x64:
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = 0x5A;
        HW(h, 0xF36B8, s32) = 0;
        HW(h, 0xF36C4, f32) = h->c.a.angle[1];
        HW(h, 0xF36C8, f32) = 5.0f;
        CMD(h) = 0x60F;
        STATE(h, Hewie_StateScramble);
        break;
    case 0x63:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            CMD(h) = 0x60F;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 2;
            HW(h, 0x108, s32) = -1;
            STATE(h, Hewie_StateSetOff);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x1D:
        MODE(h) = 0;
        anim(h, 0);
        TARGET(h) = gCharPlayer;
        CMD(h) = 0;
        STATE(h, Hewie_State1F28);
        break;
    case 0x15:
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = 0x1E;
        HW(h, 0xF36B8, s32) = 0;
        HW(h, 0xF36BC, s32) = Hewie_RandomLevel(h);
        HW(h, 0xF36C4, f32) = Angle_Wrap(h->c.a.angle[1] + AT(h->c.motion, 0x858, f32));
        turn_speed(h);
        WAIT(h) = (s32)(3.0f * RNG01()) * 30 + 30;
        CMD(h) = 0x78D;
        STATE(h, Hewie_StateRoam);
        break;
    case 0x16:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            CMD(h) = 0x68E;
            STATE(h, Hewie_StateComeToCommand);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x17:
        Hewie_IdleAction(h);
        break;
    case 0x18:
        if (tri_flags(h) & 3) {
            instead(h, 0, 0);
            return;
        }
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = (s32)(2.0f * RNG01());
        CMD(h) = 0x62D;
        STATE(h, Hewie_StateTricks);
        /* fall through (the floor checked again; his animation group to 4) */
    case 0x19:
    case 0x1A:
    case 0x1B:
        if (!(tri_flags(h) & 3)) {
            MODE(h) = 0;
            anim(h, 4);
            CMD(h) = 0x62D;
            STATE(h, Hewie_StateTricks);
            return;
        }
        instead(h, 0, 0);
        return;
    case 0x1C:
        if (!(tri_flags(h) & 3)) {
            MODE(h) = 0;
            anim(h, 4);
            WAIT(h) = 0x384;
            CMD(h) = 0x62D;
            STATE(h, Hewie_StateTricks);
            return;
        }
        instead(h, 0, 0);
        return;
    case 0x1E:
        MODE(h) = 0;
        anim(h, 0);
        TARGET(h) = gCharPlayer;
        CMD(h) = 0;
        STATE(h, Hewie_State1F88);
        break;
    case 0x1F:
    case 0x20:
    case 0x21:
    case 0x22:   /* at the pursuer */
        if (sees(h, gCharPursuer)) {
            s32 sub = gCharPursuer->moveSub;

            if (gCharPursuer->moveMode != 3 && sub != 9 && sub != 0xA) {
                MODE(h) = 8;
                HW(h, 0xFC, s32) = 0x18;
                HW(h, 0x100, s32) = gCharPursuer->a.slot;
                TARGET(h) = gCharPursuer;
                HW(h, 0x124, s32) = HW(h, 0x128, s32);
                HW(h, 0xF3604, s32) = 4;
                HW(h, 0xF3608, s32) = 0;
                HW(h, 0x104, s32) = -1;
                CMD(h) = 8;
                STATE(h, Hewie_StateRunAtPursuer);
                return;
            }
        }
        if (AT(gProgress, 0x1FBEC1, u8) == 0) {
            instead(h, 6, 0);
        } else {
            instead(h, 0x87, 0);
        }
        break;
    case 0x23:
        if (sees(h, TARGET(h))) {
            s32 m = TARGET(h)->moveMode;

            if (m != 4 && m != 3) {
                if (Actor_Distance(&h->c.a, TARGET(h)->a.pos) <= 10.0f) {
                    if (TARGET(h) != gCharPlayer) {
                        instead(h, 0x59, 0);
                    } else {
                        instead(h, 0x61, 0);
                    }
                    return;
                }
                MODE(h) = 8;
                HW(h, 0xF36B4, s32) = 0x5A;
                CMD(h) = HW(h, 0xF35C0, s32) != 3 ? 8 : 0;
                STATE(h, Hewie_StateCloseOnTarget);
                return;
            }
        }
        give_up(h);
        break;
    case 0x24: {
        u8 place;

        if (sees(h, gCharPlayer)) {
            place = Hewie_FleeExit(h, gCharPlayer, 0);
        } else {
            place = Hewie_FleeExit(h, &h->c, 0);
        }
        if (place == 0xFF) {
            instead(h, 0, 0);
        } else {
            to_place(h, place);
            STATE(h, Hewie_StateGoToExit);
        }
        break;
    }
    case 0x25: {
        u8 place = Hewie_FleeExit(h, &h->c, 0);

        if (place == 0xFF) {
            instead(h, 0x10, 0);
        } else {
            to_place(h, place);
            STATE(h, Hewie_StateGoToExit);
        }
        break;
    }
    case 0x26:
    case 0x27:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x7AF;
        STATE(h, Hewie_StateAfter27);
        return;
    case 0x28:
    case 0x29:
        if (!(tri_flags(h) & 0x80001)) {
            MODE(h) = 0;
            anim(h, 4);
            CMD(h) = 0x7AF;
            STATE(h, Hewie_StateAfter29);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x2A:
    case 0x2B:
        if (!(tri_flags(h) & 0x80001)) {
            MODE(h) = 0;
            anim(h, 4);
            CMD(h) = 0x7AF;
            STATE(h, Hewie_StateAfter2B);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x2C:
    case 0x2D:
    case 0x39:   /* out of the room by the path he has (or to the progress's room) */
        if (HW(h, 0xF3590, u8) == 0) {
            if (Character_Route(&h->c, VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), -1, HEWIE_SIDE(h), -1) <= 0) {
                path_clear(h);
                Hewie_WhenIdle(h);
                return;
            }
            door_from_path(h);
        }
        MODE(h) = 6;
        switch (HEWIE_ACTION(h)) {
        case 0x2C:
            CMD(h) = 0xCF;
            break;
        case 0x2D:
            CMD(h) = 8;
            break;
        case 0x39:
            CMD(h) = 0;
            break;
        }
        CMD(h) = 0xCF;   /* (the original's: the choice above never counts) */
        STATE(h, Hewie_StateToDoor);
        return;
    case 0x2E:
        MODE(h) = 6;
        CMD(h) = 0xDF;
        WAIT(h) = D_003B13D0[HW(h, 0xF35CC, s16)];
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_State2018);
        break;
    case 0x2F:
    case 0x30:
    case 0x31:
        MODE(h) = 6;
        CMD(h) = a == 0x2F ? 0x88 : a == 0x30 ? 0x80 : 0xDF;
        WAIT(h) = HW(h, 0xF3560, s32);
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_State2018);
        break;
    case 0x32: {   /* out by a random open exit */
        u8 seen = 0;
        u32 n;

        for (n = 0; n < 8; n++) {
            u8 e = exit_random(h, &seen);
            u16 door = VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e);

            if (door == 0xFFFF) {
                continue;
            }
            if ((VCALL(gRooms, 0x74, u32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e) & 0xFF) != 1
                || !exit_open(h, e)) {
                continue;
            }
            if (Character_Route(&h->c, VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e), -1,
                              HEWIE_SIDE(h), -1) <= 0) {
                path_clear(h);
                continue;
            }
            if (HW(h, 0xF3590, u8) == 0 || HW(h, 0x14C0, u16) != door) {
                HW(h, 0xF3590, u8) = 0;
                HW(h, 0xFC, s32) = 0x17;
                door_from_path(h);
            }
            MODE(h) = 6;
            CMD(h) = 0xDF;
            STATE(h, Hewie_StateToDoor);
            return;
        }
        Hewie_WhenIdle(h);
        break;
    }
    case 0x33: {   /* to the room +0xF3594 */
        s32 ok = 0;

        if (HW(h, 0xF3590, u8) == 1) {
            u8 e = VCALL(gRooms, 0x3C, s32 (*)(VObject *, u32, s32))(gRooms, HW(h, 0x14C0, u16), h->c.a.room);

            if (e != 0xFF && VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e)
                                 == HW(h, 0xF3594, s32)) {
                ok = 1;
            }
        }
        if (!(ok & 0xFF)) {
            HW(h, 0xF3590, u8) = 0;
            HW(h, 0xFC, s32) = 0x17;
            if (Character_Route(&h->c, HW(h, 0xF3594, s32), -1, HEWIE_SIDE(h), -1) <= 0) {
                path_clear(h);
                Hewie_WhenIdle(h);
                return;
            }
            door_from_path(h);
        }
        MODE(h) = 6;
        CMD(h) = 0xDF;
        STATE(h, Hewie_StateToDoor);
        return;
    }
    case 0x34: {
        f32 at[4] __attribute__((aligned(16)));

        MODE(h) = 6;
        CMD(h) = 0x80;
        WAIT(h) = 0x3C;
        if (Actor_PosInCurrentRoom(&h->c.a, at) != 0) {
            sceVu0SubVector(at, at, h->c.a.pos);
            Actor_PlaySound(&h->c.a, 0x69, 5, 0, 0, at);
        }
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_StateLoudNoise);
        break;
    }
    case 0x35: {   /* to another room by a random open exit (the progress's room: at once) */
        Progress *p = gProgress;
        u8 seen = 0;
        u32 n;

        if (Character_Route(&h->c, VCALL(p, 0xC, s32 (*)(Progress *))(p), -1, HEWIE_SIDE(h), -1) != 2) {
            for (n = 0; n < 8; n++) {
                u8 e = exit_random(h, &seen);
                s32 room;

                if ((VCALL(gRooms, 0x74, u32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e) & 0xFF) != 1) {
                    continue;
                }
                if (!exit_open(h, e)) {
                    continue;
                }
                room = VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e);
                if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p) && Character_Route(&h->c, room, -1, HEWIE_SIDE(h), -1) == 1) {
                    door_from_path(h);
                    MODE(h) = 6;
                    CMD(h) = 0x80;
                    HW(h, 0xF3590, u8) = 0;
                    HW(h, 0xFC, s32) = 0x17;
                    STATE(h, Hewie_StateToDoor);
                    return;
                }
            }
        }
        MODE(h) = 6;
        CMD(h) = 0x80;
        HW(h, 0xF3590, u8) = 0;
        HW(h, 0xFC, s32) = 0x17;
        STATE(h, Hewie_State2098);
        break;
    }
    case 0x36:
        MODE(h) = 6;
        CMD(h) = 0;
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_State20A8);
        break;
    case 0x37:
        MODE(h) = 6;
        CMD(h) = 0;
        path_clear(h);
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_StateToDefault);
        break;
    case 0x38:
        MODE(h) = 6;
        CMD(h) = 0x80;
        switch (HW(h, 0xF36B8, s32)) {
        case 0x1F:
        case 0x20:
            switch (HW(h, 0xF36BC, s32)) {
            case 1:
            case 2:
                WAIT(h) = (HW(h, 0xF35CC, s16) + 1) * 0xF + 0x1F;
                break;
            case 0:
                WAIT(h) = (HW(h, 0xF35CC, s16) + 1) * 0xF + 0x3D;
                break;
            }
            break;
        case 0x21:
        case 0x22:
        case 0x75:
            WAIT(h) = (HW(h, 0xF35CC, s16) + 1) * 0xF + 0x3D;
            break;
        }
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_State20C8);
        break;
    case 0x3B:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        Motion_PlayTable(h->c.motion, HW(h, 0x104, s32), -1);
        HW(h, 0xE1, s8) = 1;
        STATE(h, Hewie_StateRootMotion);
        return;
    case 0x3C:
    case 0x47:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        Motion_PlayBlend(h->c.motion, HW(h, 0x104, s32), HW(h, 0x108, s32), -1);
        HW(h, 0xE1, s8) = 1;
        STATE(h, Hewie_StateRootMotion);
        return;
    case 0x3D:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        Motion_PlayBlend8(h->c.motion, HW(h, 0x104, s32), HW(h, 0x108, s32));
        HW(h, 0xE1, s8) = 1;
        STATE(h, Hewie_StateRootMotion);
        return;
    case 0x3E:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        Hewie_StandAnim(h, HW(h, 0x108, s32));
        HW(h, 0xE1, s8) = 1;
        STATE(h, Hewie_StateRootMotion);
        return;
    case 0x3F:
        MODE(h) = 0;
        CMD(h) = 0;
        HW(h, 0xF36B4, s32) = 0;
        HW(h, 0xF36B8, s32) = 0;
        STATE(h, Hewie_StateSetOff);
        return;
    case 0x40:
        MODE(h) = 0;
        CMD(h) = 0;
        HW(h, 0xF36B4, s32) = 0;
        HW(h, 0xF36B8, s32) = 2;
        STATE(h, Hewie_StateSetOff);
        break;
    case 0x41:
    case 0x42:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_State2138);
        return;
    case 0x43:
        if (gCharacters[HW(h, 0x100, s32)] == NULL) {
            HW(h, 0xE1, s8) = 1;
        } else {
            MODE(h) = 0;
            CMD(h) = 0;
            HW(h, 0x10C, f32) = Actor_HeadingTo(&h->c.a, gCharacters[HW(h, 0x100, s32)]->a.pos);
            STATE(h, Hewie_StateTurnStart);
        }
        break;
    case 0x44:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_StateTurnStart);
        return;
    case 0x45:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_State2168);
        return;
    case 0x46:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_StateHeadForSpot);
        return;
    case 0x48: {   /* turn toward +0x10C, in steps of 6 degrees */
        f32 d;
        s32 steps;

        if (!(Angle_Wrap(HW(h, 0x10C, f32) - h->c.a.angle[1]) <= 0.0f)) {
            d = Angle_Wrap(HW(h, 0x10C, f32) - h->c.a.angle[1]);
        } else {
            d = -Angle_Wrap(HW(h, 0x10C, f32) - h->c.a.angle[1]);
        }
        steps = (s32)(d / 0x1.aceea00000000p-4f /* 0.10471976 */);
        HW(h, 0xF36B4, s32) = steps;
        HW(h, 0xF36B8, s32) = steps < 6 ? 1 : 0;
        MODE(h) = 0xC;
        anim(h, 0);
        TARGET(h) = gCharPlayer;
        CMD(h) = 8;
        STATE(h, Hewie_StateTurnWithFiona);
        break;
    }
    case 0x49:
    case 0x4A:
    case 0x4B:
        MODE(h) = 0xC;
        anim(h, 4);
        CMD(h) = 0x408;
        STATE(h, Hewie_State2198);
        return;
    case 0x4C:
        instead(h, 0x15, 0);
        break;
    case 0x4D:
        Hewie_After4D(h);
        break;
    case 0x4F:
    case 0x50:
        if (HW(h, 0xF3598, s32) == 1) {
            HW(h, 0xF35B0, s16) = 0x384;
        }
        Hewie_AfterCall(h);
        break;
    case 0x4E:
        HW(h, 0xF35B0, s16) = 0;
        Hewie_AfterComingOut(h);
        break;
    case 0x51:
        Hewie_RandomIdle(h);
        break;
    case 0x52:
        Hewie_SetMode(h, 0, -1);
        if (HW(h, 0x29, u8) == 0) {
            MODE(h) = 0;
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 0;
            HW(h, 0x2D, u8) = 1;
        } else {
            MODE(h) = 6;
            HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        }
        HW(h, 0x14C8, s32) = 0;
        WAIT(h) = 0x1518;
        CMD(h) = 0;
        STATE(h, Hewie_StateKnockedDown);
        return;
    case 0x54:
        TARGET(h) = gCharPlayer;
        /* fall through */
    case 0x53:
        if (sees(h, TARGET(h))) {
            s32 m = TARGET(h)->moveMode;

            if (m != 4 && m != 3) {
                HW(h, 0xF36C8, f32) = 30.0f;
                MODE(h) = 8;
                HW(h, 0xF36B4, s32) = 0;
                HW(h, 0xF36B8, s32) = 0;
                HW(h, 0xF36C4, f32) = h->c.a.angle[1];
                WAIT(h) = 0x3C;
                CMD(h) = HW(h, 0xF35C0, s32) != 3 ? 0x88 : 0x80;
                STATE(h, Hewie_StateKeepNear);
                return;
            }
        }
        give_up(h);
        break;
    case 0x55:
        if (sees(h, gCharPlayer)) {
            TARGET(h) = gCharPlayer;
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 0;
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            HW(h, 0xF36C8, f32) = 20.0f;
            CMD(h) = 0x384;
            STATE(h, Hewie_StateKeepNear);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x56:
    case 0x57:
        MODE(h) = 0;
        switch (HEWIE_ACTION(h)) {
        case 0x56:
            HW(h, 0xF36C8, f32) = 30.0f;
            WAIT(h) = 0x3C;
            break;
        case 0x57:
            HW(h, 0xF36C8, f32) = 20.0f;
            break;
        }
        HW(h, 0xF36B4, s32) = 0;
        HW(h, 0xF36B8, s32) = 0;
        HW(h, 0xF36C4, f32) = h->c.a.angle[1];
        CMD(h) = 0x7AF;
        STATE(h, Hewie_StateKeepNear);
        break;
    case 0x58:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x68F;
        STATE(h, Hewie_StatePose2);
        return;
    case 0x5A:
        HW(h, 0xF3560, s32) = 3;
        /* fall through */
    case 0x59:
        if (HW(h, 0xF3548, void *) == NULL) {
            HW(h, 0xF3548, void *) = Hewie_PickTarget(h);
        }
        if (sees(h, HW(h, 0xF3548, void *))) {
            TARGET(h) = HW(h, 0xF3548, Character *);
            if (TARGET(h) != NULL) {
                instead(h, 0x53, 0x23);
                return;
            }
        }
        give_up(h);
        break;
    case 0x5B:
    case 0x5C:
    case 0x5D:
    case 0x5E:
    case 0x5F:
    case 0x60: {   /* at character 3, 4 or 5 (the "+3" ones set +0xF3560 too) */
        s32 k = 0;

        switch (a) {
        case 0x5C:
            HW(h, 0xF3560, s32) = 3;
            /* fall through */
        case 0x5B:
            k = 3;
            break;
        case 0x5E:
            HW(h, 0xF3560, s32) = 3;
            /* fall through */
        case 0x5D:
            k = 4;
            break;
        case 0x60:
            HW(h, 0xF3560, s32) = 3;
            /* fall through */
        case 0x5F:
            k = 5;
            break;
        }
        TARGET(h) = gCharacters[k];
        if (sees(h, TARGET(h))) {
            instead(h, 0x53, 0x23);
            return;
        }
        instead(h, 0, 0);
        break;
    }
    case 0x62:
        HW(h, 0xF3560, s32) = 3;
        /* fall through */
    case 0x61:
        if (!sees(h, gCharPlayer)) {
            give_up(h);
        } else {
            instead(h, 0x54, 0x23);
        }
        break;
    case 0x65:
        MODE(h) = 0;
        if (tri_flags(h) & 0x20000) {
            HW(h, 0x2B, s8) = 1;
        }
        HW(h, 0xF36B8, s32) = 0x5A;
        CMD(h) = 0;
        STATE(h, Hewie_StateSqueeze);
        break;
    case 0x66: {
        f32 at[4] __attribute__((aligned(16)));

        MODE(h) = 0;
        HW(h, 0x2D, u8) = 1;
        HW(h, 0x2B, s8) = 1;
        HW(h, 0xC0, s32) = 8;
        AT(HW(h, 0x1380, void *), 0x40, s32) = HW(h, 0xC0, s32);
        anim(h, 4);
        if (Hewie_PlanAndGo(h, VCALL(gRooms, 0x30, s32 (*)(VObject *, u8, f32 *))(gRooms, (u8)HW(h, 0xF36B4, s32), at),
                          at, 0, 1) != 0) {
            HW(h, 0x124, s32) = HW(h, 0x128, s32);
        }
        CMD(h) = 0;
        STATE(h, Hewie_StateWalkOut);
        break;
    }
    case 0x67:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            HW(h, 0xF36B4, s32) = 0;
            CMD(h) = 0x78D;
            STATE(h, Hewie_StateStepAwayFiona);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x68:
        MODE(h) = 4;
        face(h);
        CMD(h) = 0;
        HW(h, 0xF36C8, f32) = 4.0f;
        HW(h, 0xF36CC, f32) = 0.5f;
        Motion_PlayTable(h->c.motion, 0x1000, -1);
        Hewie_MakeSound(h, 0x66);
        STATE(h, Hewie_StateMoveAlong);
        return;
    case 0x69:
        MODE(h) = 4;
        face(h);
        CMD(h) = 0;
        HW(h, 0xF36C8, f32) = 1.5f;
        HW(h, 0xF36CC, f32) = 0.5f;
        Motion_PlayTable(h->c.motion, 0x1000, -1);
        if (HW(h, 0x104, s32) == 1) {
            Hewie_MakeSound(h, 0x65);
        } else {
            Hewie_MakeSound(h, 0x66);
        }
        STATE(h, Hewie_StateMoveAlong);
        return;
    case 0x6A:
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = 0;
        CMD(h) = 8;
        STATE(h, Hewie_StateOffMesh);
        return;
    case 0x6B:
    case 0x6C:
        MODE(h) = 4;
        anim(h, 4);
        CMD(h) = 0;
        HW(h, 0xF36C8, f32) = 5.0f;
        HW(h, 0xF36CC, f32) = 1.0f;
        Motion_PlayTable(h->c.motion, 0x1000, -1);
        Hewie_MakeSound(h, 0x66);
        if (a == 0x6B) {
            STATE(h, Hewie_StateMoveAlong);
            return;
        }
        HW(h, 0x2D, u8) = 1;
        HW(h, 0x2B, s8) = 1;
        STATE(h, Hewie_StateMoveAlong);
        break;
    case 0x6D:
        if (sees(h, TARGET(h))) {
            s32 m = TARGET(h)->moveMode;

            if (m != 3 && m != 4) {
                HW(h, 0xF36B4, s32) = 0x5A;
                HW(h, 0x124, s32) = HW(h, 0x128, s32);
                MODE(h) = 8;
                CMD(h) = 8;
                HW(h, 0x104, s32) = 0;
                HW(h, 0x108, s32) = 0;
                STATE(h, Hewie_StateCloseIn);
                return;
            }
        }
        give_up(h);
        break;
    case 0x6E:
        HW(h, 0xF36B4, s32) = -1;
        HW(h, 0xF36B8, s32) = 0;
        MODE(h) = 0;
        CMD(h) = 8;
        STATE(h, Hewie_StateSlope);
        return;
    case 0x6F:
        CMD(h) = 0;
        STATE(h, Hewie_StateRunToFiona);
        break;
    case 0x70:
        HW(h, 0xF36B4, s32) = 0x1E;
        HW(h, 0xF36B8, s32) = 0;
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        STATE(h, Hewie_StateRun);
        break;
    case 0x71:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x400;
        STATE(h, Hewie_State22B8);
        return;
    case 0x72:
        if (sees(h, gCharPlayer)) {
            TARGET(h) = gCharPlayer;
            HW(h, 0x124, s32) = HW(h, 0x128, s32);
            MODE(h) = 0;
            CMD(h) = 0;
            STATE(h, Hewie_StateKeepBehind);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x73:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_StateAnimOver);
        return;
    case 0x74:
        MODE(h) = 0;
        CMD(h) = 0;
        Motion_PlayTable(h->c.motion, 0x2213, -1);
        Hewie_MakeSound(h, 0x66);
        STATE(h, Hewie_State22E8);
        return;
    case 0x75:
        if (sees(h, gCharPursuer)) {
            s32 sub = gCharPursuer->moveSub;

            if (gCharPursuer->moveMode != 3 && sub != 9 && sub != 0xA) {
                MODE(h) = 8;
                HW(h, 0x100, s32) = gCharPursuer->a.slot;
                TARGET(h) = gCharPursuer;
                HW(h, 0x124, s32) = HW(h, 0x128, s32);
                HW(h, 0xF3604, s32) = 4;
                HW(h, 0xF3608, s32) = 0;
                CMD(h) = 0;
                HW(h, 0x2D, u8) = 1;
                STATE(h, Hewie_StateTargetPursuer);
                return;
            }
        }
        instead(h, 0, 0);
        break;
    case 0x76:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_StatePlayAnim);
        return;
    case 0x77:
        MODE(h) = 6;
        CMD(h) = 8;
        STATE(h, Hewie_State2318);
        return;
    case 0x78:
        HW(h, 0xF3686, s16) = 0;
        HW(h, 0xF3684, s16) = 0;
        HW(h, 0xF36A4, s32) = 0x78;
        HW(h, 0xF36A2, s8) = 0;
        Hewie_RollReaction(h);
        MODE(h) = 0;
        CMD(h) = 0x68E;
        if (HW(h, 0xF369C, u8) != 0) {
            STATE(h, Hewie_StateFetch);
        } else if (HW(h, 0xF369D, u8) != 0) {
            STATE(h, Hewie_StateFollowMover);
        } else {
            HW(h, 0xF36BC, s32) = 0x5A;
            STATE(h, Hewie_State1A60);
        }
        return;
    case 0x79:
        HW(h, 0x2D, u8) = 1;
        MODE(h) = 0xB;
        Motion_PlayTable(h->c.motion, 0x1301, -1);
        CMD(h) = 0;
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 0;
        STATE(h, Hewie_State2358);
        return;
    case 0x7A:
        MODE(h) = 0;
        WAIT(h) = sWait[HW(h, 0xF35CC, s16)];
        HW(h, 0xF36B4, s32) = 0x3C;
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 0;
        CMD(h) = AT(gProgress, 0x1FBEC1, u8) == 0 ? 0x88 : 8;
        STATE(h, Hewie_State2388);
        return;
    case 0x7B:
        if (sees(h, gCharPursuer)) {
            s32 m = gCharPursuer->moveMode;

            if (m != 3 && m != 4) {
                u8 place = Hewie_FleeExit(h, gCharPursuer, 1);

                if (place != 0xFF) {
                    f32 at[4] __attribute__((aligned(16)));

                    HW(h, 0x100, s32) = place;
                    HW(h, 0xF36B4, s32) = VCALL(gRooms, 0x30, s32 (*)(VObject *, u32, f32 *))(gRooms, place, at);
                    HW(h, 0xF36BC, s32) = 0;
                    sceVu0CopyVector((f32 *)((u8 *)h + 0xF36E0), at);
                    HW(h, 0x124, s32) = HW(h, 0x128, s32);
                    MODE(h) = 0;
                    CMD(h) = 0x88;
                    STATE(h, Hewie_StatePlanPursuer);
                    return;
                }
            }
        }
        instead(h, 6, 0);
        break;
    case 0x7C:
        TARGET(h) = Hewie_PickTarget(h);
        if (sees(h, TARGET(h))) {
            HW(h, 0xF36C8, f32) = 30.0f;
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 0;
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            CMD(h) = 0x88;
            STATE(h, Hewie_StateKeepNear);
            return;
        }
        instead(h, 6, 0);
        break;
    case 0x7D:
    case 0x7E:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            CMD(h) = 0x32D;
            STATE(h, Hewie_StateWhine);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x7F:
        HW(h, 0xF36C4, f32) = HW(h, 0x114, f32);
        HW(h, 0xF36B8, s32) = HW(h, 0x2D, u8) == 1 ? 1 : 0;
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_StateRunForSpot);
        return;
    case 0x80:
        MODE(h) = 0;
        CMD(h) = 8;
        HW(h, 0xF36B4, s32) = HW(h, 0xF3568, s32);
        STATE(h, Hewie_State23D8);
        break;
    case 0x81: {
        f32 dist, ang, to;

        MODE(h) = 0;
        anim(h, 8);
        Motion_LookAt(h->c.motion, (f32 *)((u8 *)h + 0xF3630), &dist, &ang);
        to = Angle_Wrap(h->c.a.angle[1] + ang);
        HW(h, 0xF3614, f32) = dist;
        HW(h, 0xF3618, f32) = Angle_Wrap(to - h->c.a.angle[1]);
        CMD(h) = 0x7AD;
        STATE(h, Hewie_StatePose4);
        break;
    }
    case 0x82: {
        Character *t = TARGET(h);

        if (t != NULL && t->a.active == 1 && h->c.a.room == t->a.room && t->a.navTri != (u32)-1) {
            MODE(h) = 0;
            anim(h, 0);
            HW(h, 0xF36B4, s32) = 0;
            CMD(h) = 8;
            STATE(h, Hewie_StateStepAwayTarget);
            return;
        }
        instead(h, 0, 0);
        break;
    }
    case 0x83:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 8;
        STATE(h, Hewie_StateSteered);
        return;
    case 0x84:
        HW(h, 0x2D, u8) = 1;
        MODE(h) = 0;
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_State2418);
        break;
    case 0x85:   /* look at Fiona, else the pursuer, within 150 */
        MODE(h) = 0;
        TARGET(h) = NULL;
        if (sees(h, gCharPlayer) && Actor_Distance(&h->c.a, gCharPlayer->a.pos) < 150.0f) {
            TARGET(h) = gCharPlayer;
        }
        if (TARGET(h) == NULL && sees(h, gCharPursuer) && Actor_Distance(&h->c.a, gCharPursuer->a.pos) < 150.0f) {
            TARGET(h) = gCharPursuer;
        }
        CMD(h) = 0;
        STATE(h, Hewie_StateBark);
        break;
    case 0x86:
        MODE(h) = 0;
        HW(h, 0x2B, s8) = 1;
        HW(h, 0x2D, u8) = 1;
        VCALL(gRooms, 0x34, void (*)(VObject *, u8, void *))(gRooms, (u8)HW(h, 0xF36B4, s32), (u8 *)h + 0xF36E0);
        CMD(h) = 0;
        STATE(h, Hewie_StateBackOnMesh);
        break;
    case 0x87:
        MODE(h) = 8;
        HW(h, 0xF36D0, f32) = Angle_Wrap(h->c.a.angle[1] + AT(h->c.motion, 0x858, f32));
        CMD(h) = 0;
        STATE(h, Hewie_StateRunUpJump);
        return;
    default:
        return;
    }
}

/* vtable +0x4C: disable (as Character), animation paused. */
/* 0x00165CD0 */
void Hewie_Disable(Hewie *h) {
    Character_Disable(&h->c);
    MOTION_U8(h->c.motion, 0x4D8) = 1;
}

/* Group of his current animation (0 idle .. 0xE, 0xF other). */
/* 0x001669A0 */
s32 Hewie_AnimGroup(Hewie *h) {
    switch (MOTION_ANIM(h->c.motion)) {
    case 0x0: case 0x3: case 0x4: case 0x5: case 0x6: case 0x9:
        return 0;
    case 0x1:
        return 1;
    case 0x2: case 0x7:
        return 2;
    case 0x8:
        return 3;
    case 0x101: case 0x103: case 0x1000: case 0x1003: case 0x1301: case 0x1B00: case 0x1B03:
    case 0x1B04: case 0x1B05: case 0x1C02: case 0x2212:
        return 4;
    case 0x100: case 0x105: case 0x107: case 0x1B01: case 0x1C00: case 0x1C01: case 0x1C04:
    case 0x1C05: case 0x1C06: case 0x1D00: case 0x1D01: case 0x1D02:
        return 5;
    case 0x102: case 0x104: case 0x301: case 0x1B02: case 0x1C07:
        return 6;
    case 0x106:
        return 7;
    case 0x202:
        return 8;
    case 0x201:
        return 9;
    case 0x200: case 0x204: case 0x205: case 0x206:
        return 0xA;
    case 0x203:
        return 0xB;
    case 0x300:
        return 0xC;
    case 0x1002:
        return 0xD;
    case 0x1001: case 0x2213:
        return 0xE;
    default:
        return 0xF;
    }
}

/* Adjust a requested action to his situation: down (no health) -> 0x52 (and progress +0xFB6
 * counts up, max 10000); while blocked (gCamDirector +0x38) or with progress flags 0x13 / 0x2B
 * set his attack-type actions become waiting ones; then substitutions by his condition
 * (+0xC4), mode (+0xF35C0) and flags. In the special mode the action is kept. */
/* 0x0013B2C0 */
s32 Hewie_AdjustAction(Hewie *h, s32 act) {
    Progress *p = gProgress;
    u8 f;

    if (*((u8 *)p + 0x1FBEC1) == 1) {
        return act;
    }
    if (!h->c.unkE0 && act == 0 && h->c.hp == 0 && h->c.a.unkC4 == 2) {
        act = 0x52;
    }
    if (act == 0x52) {
        if (!h->c.a.disabled && !(Actor_TriFreeFor(&h->c.a, &h->c.a) & 0xFF)) {
            act = 0;
            h->c.hp = 1;
            h->c.a.unkC4 = 1;
        } else if (HEWIE_ACTION(h) != 0x52 && HEWIE_ACTION(h) != 0x74 && Hewie_AnimGroup(h) != 0xD) {
            s16 *n = (s16 *)((u8 *)p + 0xFB6);

            *n += 10;
            if (*n < 0) {
                *n = 0;
            } else if (*n > 10000) {
                *n = 10000;
            }
        }
    }
    if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector)) {
        switch (act) {
        case 0x24: case 0x25:
            act = 5;
            break;
        case 0x61: case 0x62:
            act = 0xB;
            break;
        case 0x1F: case 0x20: case 0x21: case 0x22: case 0x23: case 0x4E: case 0x4F: case 0x50:
        case 0x59: case 0x5A: case 0x75:
            act = 0xA;
            break;
        }
    }
    f = Progress_TestFlag(gProgress, 0x13) & 0xFF;
    if ((f | (Progress_TestFlag(gProgress, 0x2B) & 0xFF)) != 0) {
        switch (act) {
        case 0x1F: case 0x20: case 0x21: case 0x22: case 0x23: case 0x4E: case 0x4F: case 0x50:
        case 0x59: case 0x5A: case 0x61: case 0x62: case 0x75:
            act = 0xA;
            break;
        }
    }
    if (HW(h, 0xF35B0, s16) != 0 && (act == 0x50 || act == 0x4F)) {
        act = 0xA;
    }
    if (h->c.a.unkC4 == 1) {
        switch (act) {
        case 0x13: case 0x64:
            if (HW(h, 0xF3598, s32) == 1) {
                act = 0x10;
            }
            break;
        case 0x16: case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x26: case 0x28: case 0x2A:
            act = 0x58;
            break;
        }
    }
    if (HW(h, 0xF35C0, s32) == 2) {
        switch (act) {
        case 0x13: case 0x64:
            if (HW(h, 0xF3598, s32) == 1) {
                act = 0x10;
            }
            break;
        case 0xE:
            if ((Progress_GameMode(p) & 0xFF) == 2) {
                act = 0xF;
            }
            break;
        case 0xC: case 0x16: case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C:
            act = 0x2A;
            break;
        }
    }
    if (HW(h, 0xF35C0, s32) == 1 && act == 0xC) {
        act = 0x16;
    }
    if (HW(h, 0xF3620, u8) == 1 && (act == 5 || act == 4 || act == 1) && !(Progress_GameMode(p) & 0xFF)) {
        act = 0x81;
    }
    if (HW(h, 0xF3588, u8) == 1) {
        act = 4;
    }
    return act;
}

#define RNG01() VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)

/* One chance in `n`: sets +0xF3586. */
/* 0x00138DE0 */
void Hewie_Chance(Hewie *h, s32 n) {
    if ((s32)((f32)n * RNG01()) == 0) {
        HW(h, 0xF3586, u8) = 1;
    }
}

/* Spend `amount` of his pool (+0xF359C) while it is in use (+0xF3598). */
/* 0x00138E60 */
void Hewie_SpendPool(Hewie *h, s32 amount) {
    if (HW(h, 0xF3598, s32) == 1) {
        HW(h, 0xF359C, s32) -= amount;
        HW(h, 0xF3587, u8) = 0;
    }
}

extern VObject *gSceneGameF29740;   /* path planner */

/* Triangles on opposite sides of a divided room (flags 0x100000 / 0x200000). */
static inline s32 Hewie_OtherSide(Hewie *h, u32 tri) {
    return NavMesh_AcrossDivider(gNavMesh, tri, h->c.a.navTri);
}

/* Plan a path to `pos` on `tri` (not across the room's divider); 1 if one was found. */
/* 0x0013C1E0 */
s32 Hewie_PlanTo(Hewie *h, u32 tri, const f32 *pos) {
    s32 r;

    if (Hewie_OtherSide(h, tri)) {
        return 0;
    }
    r = Character_PlanPathKind(&h->c, 0, tri, pos);
    if (r > 0) {
        r = VCALL(gSceneGameF29740, 0x14, s32 (*)(VObject *))(gSceneGameF29740);
    }
    return r > 0;
}

/* Plan a path to `pos` on `tri` (not across the divider) and start it (`direct`: the straight
 * variant); `keep` keeps the planner's previous request. 0 = ok, -1 = failed. */
/* 0x0013EE40 */
s32 Hewie_PlanAndGo(Hewie *h, u32 tri, const f32 *pos, s32 direct, s32 keep) {
    s32 r;

    if (Hewie_OtherSide(h, tri)) {
        return -1;
    }
    r = Character_PlanPathKind(&h->c, 0, tri, pos);
    if (r <= 0) {
        return -(r < 0);
    }
    if (!keep) {
        VCALL(gSceneGameF29740, 0x34, void (*)(VObject *, s32, s32))(gSceneGameF29740, h->c.pathId, 3);
    }
    r = direct ? Character_Waypoints(&h->c) : Character_WaypointsCurve(&h->c);
    return -(r < 0);
}

#define HEWIE_MODE(h) HW(h, 0xF35C0, s32)        /* 0 normal, 1..3 (timed by +0xF35BE) */

/* gProgress +0xFB6 / +0xFB8 counters, kept within 0..10000. */
static inline void Progress_AddCounter(Progress *p, u32 off, s32 n) {
    s16 *v = (s16 *)((u8 *)p + off);

    *v += n;
    if (*v < 0) {
        *v = 0;
    } else if (*v > 10000) {
        *v = 10000;
    }
}

extern const s32 kHewieLevelNormal[];   /* by +0xF35CC (normal) */
extern const s32 kHewieLevelHard[];   /* by +0xF35CC (difficulty 1) */

/* Set his mode (and its timer: `time`, or -1 for the mode's default: 1 and 2 1800 frames, 3 450).
 * Down: only mode 0; in condition 1 only mode 3. Entering mode 1 / 3 counts in the progress
 * counters; mode 1 refills his pool and may restart action 0x34. */
/* 0x00138AD0 */
void Hewie_SetMode(Hewie *h, s32 mode, s32 time) {
    s32 m = mode;

    if (h->c.hp == 0 && h->c.a.unkC4 == 2) {
        m = 0;
    }
    if (h->c.a.unkC4 == 1 && mode != 3) {
        m = 0;
    }
    if ((s16)time != -1) {
        HW(h, 0xF35BE, s16) = time;
    } else {
        switch (m) {
        case 0:
            HW(h, 0xF35BE, s16) = 0;
            break;
        case 1:
            if (HEWIE_MODE(h) != 1) {
                Progress *p = gProgress;
                s16 *n = (s16 *)((u8 *)p + 0xFB8);

                *n += 1;
                if (*n > 10000) {
                    *n = 10000;
                }
                Progress_AddCounter(p, 0xFB6, -1);
            }
            HW(h, 0xF35BE, s16) = 1800;
            break;
        case 2:
            HW(h, 0xF35BE, s16) = 1800;
            break;
        case 3:
            if (HEWIE_MODE(h) != 3) {
                Progress_AddCounter(gProgress, 0xFB6, 20);
            }
            HW(h, 0xF35BE, s16) = 450;
            break;
        }
    }
    HEWIE_MODE(h) = m;
    if (mode != 1) {
        return;
    }
    HW(h, 0xF3598, s32) = 0;
    if ((Progress_GetVar(gProgress, 0x27) & 0xFF) == 1) {
        HW(h, 0xF359C, s32) = kHewieLevelHard[HW(h, 0xF35CC, s16)];
    } else {
        HW(h, 0xF359C, s32) = kHewieLevelNormal[HW(h, 0xF35CC, s16)];
    }
    if ((HW(h, 0xF356C, s32) & 0x80000001) == 1) {
        if (!h->c.a.disabled) {
            HW(h, 0xF3559, u8) = 1;
        } else {
            Hewie_SetAction(h, Hewie_AdjustAction(h, 0x34), 0);
        }
    }
    HW(h, 0xF3586, u8) = 0;
}

/* His feeling score about a character, by its id (NULL: not one he has feelings about). */
static inline s16 *Hewie_Feeling(Hewie *h, u32 id) {
    switch (id) {
    case 0xA: case 0xB: case 0xC: case 0x27:
        return &HW(h, 0xF3678, s16);
    case 0x4: case 0x17: case 0x25:
        return &HW(h, 0xF367A, s16);
    case 0x3: case 0x22: case 0x23: case 0x24:
        return &HW(h, 0xF3676, s16);
    case 0x2: case 0x6: case 0x7: case 0x1B:
        return &HW(h, 0xF3674, s16);
    }
    return NULL;
}

extern const s16 D_003B1264[];   /* by feeling score */

/* In mode 0 with an active pursuer: by his feeling about it, maybe (16-sided roll below the
 * score's threshold) switch to mode 2. */
/* 0x0013C5D0 */
void Hewie_FeelPursuer(Hewie *h) {
    u8 ok;
    s16 *v;
    s32 score;

    if (HEWIE_MODE(h) != 0) {
        return;
    }
    ok = (gCharPursuer != NULL && gCharPursuer->a.active == 1) ? 1 : 0;
    if (!ok) {
        return;
    }
    v = Hewie_Feeling(h, gCharPursuer->unk153C);
    if (v == NULL) {
        return;
    }
    score = *v;
    if ((s32)(16.0f * RNG01()) < D_003B1264[score]) {
        Hewie_SetMode(h, 2, -1);
    }
}

/* His feeling about the kind of character `other` is (-10..10, saved with him), changed by
 * `delta`; then re-evaluated. */
/* 0x00166150 */
void Hewie_ChangeFeeling(Hewie *h, Character *other, s32 delta) {
    s16 *v = Hewie_Feeling(h, other->unk153C);

    if (v != NULL) {
        *v += delta;
        if (*v < -10) {
            *v = -10;
        } else if (*v > 10) {
            *v = 10;
        }
    }
    Hewie_FeelPursuer(h);
}

/* vtable +0x60: forget path/movement state, then his default action. */
/* 0x00165D00 */
void Hewie_ToIdle(Hewie *h) {
    Character_ResetBehaviour(&h->c);
    Hewie_SetAction(h, 0, 0);
}

/* vtable +0x24: save the previous frame's state. */
/* 0x00168360 */
void Hewie_RememberPos(Hewie *h) {
    Character_RememberPos(&h->c);
    HW(h, 0xF354C, f32) = h->c.a.angle[1];
    HW(h, 0xF3568, s32) = HW(h, 0xF3564, s32);
    HW(h, 0xF366C, u8) = HW(h, 0xF366D, u8);
}

/* vtable +0x20: release his message slot and stop his animation player, if set up. */
/* 0x00168680 */
void Hewie_Unload(Hewie *h) {
    if (h->c.a.unkD0) {
        VCALL(gBootMessage, 0xC, void (*)(VObject *, u32))(gBootMessage, h->c.msgSlot);
        h->c.a.unkD0 = 0;
    }
    if (h->c.a.unkD1) {
        VCALL(h->c.motion, 0x10, void (*)(void *))(h->c.motion);
        h->c.a.unkD1 = 0;
    }
}

/* vtable +0x2C: room setup done - (outside the special mode, a pending +0x80 call), then put
 * his animation player on his triangle. */
/* 0x00168600 */
void Hewie_LightChange(Hewie *h) {
    if (*((u8 *)gProgress + 0x1FBEC1) == 0 && h->c.unkE4 == 1) {
        VCALL(h, 0x80, void (*)(Hewie *))(h);
    }
    VCALL(h->c.motion, 0x38, void (*)(void *, s32, u32, s32))(h->c.motion, h->c.unk152C, h->c.a.navTri, 0x1D);
}

/* vtable +0x50: halt (as Character), animation running, then back to his default action. */
/* 0x00165C50 */
void Hewie_Halt(Hewie *h) {
    Character_Enable(&h->c);
    MOTION_U8(h->c.motion, 0x4D8) = 0;
    VCALL(h->c.motion, 0x50, void (*)(void *, Hewie *))(h->c.motion, h);
    Hewie_SetAction(h, Hewie_AdjustAction(h, 0), 0);
}

/* Back to his default action (inlined in several places in the original). */
static inline void Hewie_ToDefault(Hewie *h) {
    Hewie_SetAction(h, Hewie_AdjustAction(h, 0), 0);
}

/* vtable +0x7C: back to his default action. */
/* 0x0013D190 */
void Hewie_BackToIdle(Hewie *h) {
    Hewie_ToDefault(h);
}

/* Add to his trust in Fiona (0..10000) and recompute its level 0..7. From level 2 progress flag
   0x11 is cleared, from level 3 flag 0x1D. */
/* 0x0013D1F0 */
void Hewie_AddTrust(Hewie *h, s32 add) {
    static const s16 bounds[] = { 100, 280, 450, 600, 750, 900, 1000 };
    Progress *p;
    s16 t, level;

    HW(h, 0xF35BC, s16) += add;
    if (HW(h, 0xF35BC, s16) < 0) {
        HW(h, 0xF35BC, s16) = 0;
    } else if (HW(h, 0xF35BC, s16) > 10000) {
        HW(h, 0xF35BC, s16) = 10000;
    }
    t = HW(h, 0xF35BC, s16);
    for (level = 0; level < 7 && t >= bounds[level]; level++) {
    }
    HW(h, 0xF35CC, s16) = level;

    p = gProgress;
    if ((Progress_TestFlag(p, 0x11) & 0xFF) == 1 && HW(h, 0xF35CC, s16) >= 2) {
        Progress_ClearFlag(p, 0x11);
    }
    if ((Progress_TestFlag(p, 0x1D) & 0xFF) == 1 && HW(h, 0xF35CC, s16) >= 3) {
        Progress_ClearFlag(p, 0x1D);
    }
}

#define MOTION_SKELETON(m) (*(void **)((u8 *)(m) + 0x810))

/* vtable +0x74: during action 0x23 with animation 0x1E01, his head bone's position (returns 1). */
/* 0x0013D420 */
s32 Hewie_PointOfInterest(Hewie *h, f32 *out) {
    if (HEWIE_ACTION(h) != 0x23) {
        return 0;
    }
    if (MOTION_ANIM(h->c.motion) != 0x1E01) {
        return 0;
    }
    sceVu0CopyVector(out, Skel_Bone(MOTION_SKELETON(h->c.motion), 0x1F) + 12);
    return 1;
}

/* vtable +0x90: reset (Character part), clear his action state. */
/* 0x0015FBE0 */
void Hewie_FullStop(Hewie *h) {
    Character_EventReset(&h->c);
    HW(h, 0xF358C, s32) = 0;
    HW(h, 0xF35C4, s32) = 0;
    HW(h, 0xF3610, s32) = 0xFF;
    HW(h, 0xF3584, u8) = 0;
    HW(h, 0xF35E0, u8) = 0;
    Progress_ClearFlag(gProgress, 0xB);
}

/* vtable +0x78: left the room being played during action 0x38 -> default action. */
/* 0x0015FB30 */
void Hewie_LeftBehind(Hewie *h) {
    s32 room = h->c.a.room;

    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && HEWIE_ACTION(h) == 0x38) {
        Hewie_ToDefault(h);
    }
    Hewie_SetMode(h, 0, -1);
}

/* vtable +0x94: take `damage` (difficulty 1: x1.5); returns 1 when he is down (difficulty 2:
 * never, he keeps 1). */
/* 0x0015FA20 */
s32 Hewie_TakeDamage(Hewie *h, s32 damage) {
    Progress *p = gProgress;

    if (*((u8 *)p + 0x1FBEC1) == 1 && (Progress_GetVar(p, 0x27) & 0xFF) == 1) {
        damage = (s32)(1.5f * (f32)damage);
    }
    if (damage <= 0) {
        damage = -damage;
    }
    HEWIE_HP(h) -= damage;
    if (HEWIE_HP(h) <= 0) {
        HEWIE_HP(h) = 0;
    }
    if (HEWIE_HP(h) != 0) {
        return 0;
    }
    if ((Progress_GetVar(p, 0x27) & 0xFF) == 2) {
        HEWIE_HP(h) = 1;
        return 0;
    }
    return 1;
}

/* vtable +0x48: apply the animation to the model; while enabled, take his position from the
 * root bone (and find his room and nav-mesh triangle). */
/* 0x00167AF0 */
void Hewie_FollowAnim(Hewie *h) {
    sceVu0FMATRIX m;

    sceVu0UnitMatrix(m);
    VCALL(h->c.motion, 0x28, void (*)(void *, sceVu0FMATRIX))(h->c.motion, m);
    if (h->c.a.disabled) {
        return;
    }
    MOTION_U8(h->c.motion, 0x4D8) = 1;
    Motion_Update(h->c.motion);
    MOTION_U8(h->c.motion, 0x4D8) = 0;
    h->c.a.room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    sceVu0CopyVector(h->c.a.pos, Skel_Bone(MOTION_SKELETON(h->c.motion), 0) + 12);
    h->c.a.navTri = VCALL(gNavMesh, 0x3C, u32 (*)(NavMesh *, f32 *, s32))(gNavMesh, h->c.a.pos, 0);
}

extern s32 Hewie_Arrive(Hewie *h);

/* vtable +0x38: room (re-)entry. In play: active only in the room being played (placed on his
 * side if he has no triangle yet); in the special mode: note his side, hand him to the
 * animation player and the room objects. */
/* 0x00166CE0 */
void Hewie_Vt38(Hewie *h) {
    Progress *p = gProgress;

    if (*((u8 *)p + 0x1FBEC1) != 0) {
        HEWIE_SIDE(h) = VCALL(gRooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(gRooms, h->c.a.room, h->c.door, 0);
        VCALL(h->c.motion, 0x50, void (*)(void *, Hewie *))(h->c.motion, h);
        VCALL(gEvents, 0x2C, void (*)(VObject *, Hewie *))(gEvents, h);
        return;
    }
    Hewie_Arrive(h);
    if (h->c.a.room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        h->c.a.disabled = 1;
        return;
    }
    if (h->c.a.navTri != NAV_NONE) {
        h->c.a.disabled = 0;
        return;
    }
    Actor_TeleportRandom(&h->c.a, HEWIE_SIDE(h));
    h->c.a.disabled = 0;
}

/* Resource table offset (from +0x1540) to pointer, 0 = none. */
#define HEWIE_RES(h, off) (HW(h, off, s32) != 0 ? (void *)((u8 *)(h) + HW(h, off, s32) + 0x1540) : NULL)
#define MOTION_PTR(m, off) (*(void **)((u8 *)(m) + (off)))

/* vtable +0x1C: hook his data up to the animation player and the message display. */
/* 0x00168700 */
void Hewie_FilesLoaded(Hewie *h) {
    void *m = h->c.motion;

    MOTION_PTR(m, 0x4C0) = HEWIE_RES(h, 0x1544);
    MOTION_PTR(m, 0x4D0) = HEWIE_RES(h, 0x1548);
    MOTION_PTR(m, 0x4CC) = HEWIE_RES(h, 0x154C);
    MOTION_PTR(m, 0x4C4) = HEWIE_RES(h, 0x1550);
    h->c.msgSlot = 1;
    if ((VCALL(gBootMessage, 0x8, u32 (*)(VObject *, u32, void *))(gBootMessage, h->c.msgSlot, HEWIE_MSG(h)) & 0xFF) == 1) {
        h->c.a.unkD0 = 1;
    }
    VCALL(h->c.motion, 0xC, void (*)(void *))(h->c.motion);
    h->c.a.unkD1 = 1;
    MOTION_U8(h->c.motion, 0x24) = h->c.msgSlot;
    MOTION_PTR(h->c.motion, 0x4D4) = HEWIE_MRK(h);
}

/* LoadAsync(name, dest) for his files, tagged with his file id. */
#define Hewie_Load(h, loader, name, dest) \
    VCALL(loader, 0xC, void (*)(VObject *, const void *, void *, u32, s32))( \
        loader, name, dest, (h)->c.a.flags24 | (h)->c.a.slot, 0)

/* vtable +0x14: start loading his files: model (by costume, from the unlocked costume bits in
 * the progress flags), textures (into his message buffer + 0x80000), .MRK. */
/* 0x00168830 */
void Hewie_LoadFiles(Hewie *h) {
    Progress *p = gProgress;
    VObject *loader;
    u32 costume = 0;

    if ((((u32 *)p)[0x24 / 4] & 0x2) != 0) {
        costume = 1;
    }
    if ((((u32 *)p)[0x2C / 4] & 0x4) != 0) {
        costume = 2;
    }
    if ((((u32 *)p)[0x2C / 4] & 0x100000) != 0) {
        costume = 3;
    }
    if ((((u32 *)p)[0x2C / 4] & 0x200000) != 0) {
        costume = 4;
    }
    loader = gFileLoader;
    Hewie_Load(h, loader, VCALL(h->c.motion, 0xA0, void *(*)(void *, u32))(h->c.motion, costume), (u8 *)h + 0x1540);
    HEWIE_MSG(h) = Progress_CharLoadBuffer(p, 0);
    HEWIE_MSG(h) = (u8 *)HEWIE_MSG(h) + 0x80000;
    Hewie_Load(h, loader, VCALL(h->c.motion, 0xA8, void *(*)(void *))(h->c.motion), HEWIE_MSG(h));
    Hewie_Load(h, loader, VCALL(h->c.motion, 0xA4, void *(*)(void *, u32))(h->c.motion, costume), HEWIE_MRK(h));
}

/* vtable +0x40: animation update in the room being played: ground fit (off the nav mesh: plain),
 * advance; then (still on the mesh) the animation events. */
/* 0x00167620 */
void Hewie_AnimUpdate(Hewie *h) {
    Progress *p = gProgress;
    s32 room = h->c.a.room;

    if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        if (h->c.a.navTri == NAV_NONE) {
            Model_BodyFrames(h->c.motion, h, 0.0f, 0.0f);
        } else if (HW(h, 0xF3582, u8) == 1) {
            VCALL(h->c.motion, 0x40, void (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
        } else {
            VCALL(h->c.motion, 0x40, void (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 0.0f, 0.0f);
        }
        Motion_Update(h->c.motion);
    }
    room = h->c.a.room;
    if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p) && h->c.a.navTri != NAV_NONE) {
        VCALL(h->c.motion, 0x4C, void (*)(void *, s32, Hewie *))(h->c.motion, 1, h);
    }
}

#define Character_ToIdle(c) VCALL(c, 0x7C, void (*)(Character *))(c)

/* vtable +0x8C: enable (Character part). If he was busy with Fiona (actions 0x48..0x4B, 0x72)
 * or the pursuer (0x1F..0x22, 0x38, 0x75), that character goes back to idle. */
/* 0x0015FC60 */
void Hewie_Interrupted(Hewie *h) {
    u8 ok;

    Character_BackToNormal(&h->c);
    h->c.a.unk2A = 1;
    HW(h, 0xF35E0, u8) = 0;

    ok = (gCharPlayer != NULL && gCharPlayer->a.active == 1) ? 1 : 0;
    if (ok == 1 && !gCharPlayer->unkE0) {
        switch (HEWIE_ACTION(h)) {
        case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x72:
            Character_ToIdle(gCharPlayer);
            break;
        }
    }
    ok = (gCharPursuer != NULL && gCharPursuer->a.active == 1) ? 1 : 0;
    if (ok == 1 && !gCharPursuer->unkE0) {
        switch (HEWIE_ACTION(h)) {
        case 0x1F: case 0x20: case 0x21: case 0x22: case 0x38: case 0x75:
            Character_ToIdle(gCharPursuer);
            break;
        }
    }
}

/* His state in the save data (gProgress +0x800..0x838). */
#define PSAVE(p, off, type) (*(type *)((u8 *)(p) + (off)))
#define HEWIE_SAVE_FIELDS(X)                                                         \
    X(0x81C, 0xF35BC, s16) X(0x81E, 0xF3674, s16) X(0x820, 0xF3676, s16)             \
    X(0x824, 0xF367A, s16) X(0x822, 0xF3678, s16) X(0x826, 0xF367C, s16)             \
    X(0x828, 0xF367E, s16) X(0x82C, 0xF3682, s16) X(0x82A, 0xF3680, s16)             \
    X(0x82E, 0xF3583, u8) X(0x830, 0xF35BE, s16)                                     \
    X(0x832, 0xF3690, s8) X(0x833, 0xF3691, s8) X(0x834, 0xF3692, s8)                \
    X(0x835, 0xF3693, s8) X(0x836, 0xF3694, s8) X(0x837, 0xF3695, s8)

/* vtable +0x70: restore his state from the save data. */
/* 0x001656C0 */
void Hewie_LoadState(Hewie *h) {
    Progress *p = gProgress;
    f32 yaw;

    h->c.a.room = PSAVE(p, 0x800, s32);
    HEWIE_SIDE(h) = PSAVE(p, 0x804, s32);
    h->c.a.navTri = PSAVE(p, 0x808, s32);
    h->c.a.unkC4 = PSAVE(p, 0x80C, s32);
    HEWIE_HP(h) = PSAVE(p, 0x814, s32);
    HW(h, 0xF35C0, s32) = PSAVE(p, 0x818, s32);
#define RESTORE(po, ho, type) HW(h, ho, type) = PSAVE(p, po, type);
    HEWIE_SAVE_FIELDS(RESTORE)
#undef RESTORE
    h->c.door = PSAVE(p, 0x838, u8);
    yaw = PSAVE(p, 0x810, f32);
    h->c.a.angle[1] = yaw;
    sceVu0UnitMatrix(h->c.a.rot);
    sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, yaw);
}

/* vtable +0x6C: store his state in the save data. */
/* 0x00165890 */
void Hewie_SaveState(Hewie *h) {
    Progress *p = gProgress;

    PSAVE(p, 0x800, s32) = h->c.a.room;
    PSAVE(p, 0x804, s32) = HEWIE_SIDE(h);
    PSAVE(p, 0x808, s32) = h->c.a.navTri;
    PSAVE(p, 0x80C, s32) = h->c.a.unkC4;
    PSAVE(p, 0x810, f32) = h->c.a.angle[1];
    PSAVE(p, 0x814, s32) = HEWIE_HP(h);
    PSAVE(p, 0x818, s32) = HW(h, 0xF35C0, s32);
#define SAVE(po, ho, type) PSAVE(p, po, type) = HW(h, ho, type);
    HEWIE_SAVE_FIELDS(SAVE)
#undef SAVE
    PSAVE(p, 0x838, u8) = h->c.door;
}

/* vtable +0xC: initialise (Character part, then his own state). */
/* 0x00168A10 */
void Hewie_Reset(Hewie *h) {
    Character_Reset(&h->c);
    HEWIE_SIDE(h) = 2;
    h->c.a.radius = 2.5f;
    h->c.a.height = 5.0f;
    h->c.a.navMask = 0x29020008;
    h->c.pathReq->unk4 = 6;
    *(s32 *)h->c.pathReq->pad08 = 1;
    h->c.pathReq->mask = h->c.a.navMask;
    h->c.hpMax = 100;
    h->c.hp = h->c.hpMax;
    h->c.hearThreshold = 0;
    HW(h, 0xF35BC, s16) = 0;
    HW(h, 0xF35CC, s16) = 0;
    HW(h, 0xF3674, s16) = 0;
    HW(h, 0xF3676, s16) = 0;
    HW(h, 0xF367A, s16) = 0;
    HW(h, 0xF3678, s16) = 0;
    HW(h, 0xF367C, s16) = 0;
    HW(h, 0xF367E, s16) = 0;
    HW(h, 0xF3682, s16) = 0;
    HW(h, 0xF3680, s16) = 0;
    HW(h, 0xF36F0, s32) = 0;
    HW(h, 0xF3594, s32) = -1;
    HW(h, 0xF35BE, s16) = 0;
    HW(h, 0xF3581, u8) = 0;
    HW(h, 0xF3690, u8) = 0x10;
    HW(h, 0xF3691, u8) = 0x10;
    HW(h, 0xF3692, u8) = 0x10;
    HW(h, 0xF3693, u8) = 0x10;
    HW(h, 0xF3694, u8) = 0x10;
    HW(h, 0xF3695, u8) = 0x10;
    HW(h, 0xF36A2, u8) = 0;
    HW(h, 0xF36A4, s32) = 0;
    HW(h, 0xF36AC, s32) = 0;
}

/* +0x8 */
/* 0x001F7D40 */
void *DogModel_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        HewieModel_Destroy(m, flags);
    }
    return m;
}

/* +0xB4: his secondary-motion table */
/* 0x001F7EA0 */
void DogModel_SecondaryMotion(u8 *m) {
    AT(m, 0x874, u8 *) = D_003D5F90;
}

/* +0x88: his head bone */
/* 0x001F7EB0 */
s32 DogModel_HeadBone(u8 *m) {
    return 0x1F;
}

/* +0x60: his head's position */
/* 0x001F7EC0 */
void DogModel_HeadPos(u8 *m, f32 *out) {
    sceVu0CopyVector(out, Skel_Bone(SKEL(m), 0x1F) + 12);
}

/* +0xA8 the textures (all costumes) */
/* 0x001F7F00 */
const char *DogModel_Textures(void) {
    return D_00456400;
}

/* +0x28: the model matrix (both body frames) = `mtx` */
/* 0x001F7F10 */
void DogModel_SetMatrix(u8 *m, f32 (*mtx)[4]) {
    sceVu0CopyMatrix((f32 (*)[4])(m + 0x7D0), mtx);
    sceVu0CopyMatrix((f32 (*)[4])(m + 0xB40), mtx);
}

/* +0x8C .. +0x98: his mesh parts */
/* 0x001F7F50 */
s32 DogModel_Part0(u8 *m) {
    return 9;
}

/* 0x001F7F60 */
s32 DogModel_Part1(u8 *m) {
    return 0xD;
}

/* 0x001F7F70 */
s32 DogModel_Part2(u8 *m) {
    return 0x15;
}

/* 0x001F7F80 */
s32 DogModel_Part3(u8 *m) {
    return 0x1A;
}

/* +0x18: each frame: the feet's contact from the animation */
/* 0x001F7FA0 */
void DogModel_FeetContact(u8 *m) {
    s32 i;

    for (i = 0; i < 4; i++) {
        AT(m, 0xB84 + i, u8) = AT(m, 0xB80 + i, u8);
        AT(m, 0xB80 + i, u8) = VCALL(m, 0x64, u8 (*)(u8 *, s32, s32))(m, i, 0);
    }
}

/* +0xA4 the marker file of costume `n` */
/* 0x001F8010 */
const char *DogModel_MarkerFile(void *m, s32 n) {
    switch (n) {
    case 0: return D_004562C0;
    case 1: return str_O_HEW_HEW_001_MRK;
    case 2: return str_O_HEW_HEW_002_MRK;
    case 3: return str_O_HEW_HEW_003_MRK;
    case 4: return str_O_HEW_HEW_004_MRK;
    }
    return NULL;
}

/* +0xA0 the model file of costume `n` */
/* 0x001F8090 */
const char *DogModel_ModelFile(void *m, s32 n) {
    switch (n) {
    case 0: return D_00456360;
    case 1: return str_O_HEW_HEW_001_PCK;
    case 2: return str_O_HEW_HEW_002_PCK;
    case 3: return str_O_HEW_HEW_003_PCK;
    case 4: return str_O_HEW_HEW_004_PCK;
    }
    return NULL;
}

/* +0x6C: half his hip height (bone 29 below his position), as an offset */
/* 0x001F8110 */
void DogModel_HalfHip(u8 *m, f32 *out) {
    f32 hip[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));

    sceVu0CopyVector(hip, Skel_Bone(SKEL(m), 0x1D) + 12);
    sceVu0SubVector(d, hip, (f32 *)(m + 0x800));
    out[0] = 0.0f;
    out[1] = 0.5f * -d[1];
    out[2] = 0.0f;
    out[3] = 0.0f;
}

/* +0x54: no foot on the ground */
/* 0x001F8190 */
void DogModel_FeetUp(u8 *m) {
    AT(m, 0xB80, u8) = 0;
    AT(m, 0xB81, u8) = 0;
    AT(m, 0xB82, u8) = 0;
    AT(m, 0xB83, u8) = 0;
}

/* +0x50: place the feet afresh */
/* 0x001F81B0 */
void DogModel_FeetReplace(u8 *m) {
    AT(m, 0x950, u8) = 0;
    VCALL(m, 0x54, void (*)(u8 *))(m);
}

/* +0x4C: each frame, plant the feet: a foot that stays on the ground is held where it was
 * planted; one lifting off eases from there to the animation over 4 frames (from 5); the
 * first time, all go where the animation has them */
/* 0x001F81D0 */
void DogModel_PlantFeet(u8 *m) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    s32 i;

    if (AT(m, 0x950, u8) == 0) {
        for (i = 0; i < 4; i++) {
            AT(m, 0x8E4 + i, u8) = AT(m, 0x8E0 + i, u8);
            AT(m, 0x8E0 + i, u8) = 0;
            foot_at(m, i, (f32 *)(m + 0x8A0 + i * 16));
        }
        AT(m, 0x950, u8) = 1;
        return;
    }
    for (i = 0; i < 4; i++) {
        f32 *foot = (f32 *)(m + 0x8A0 + i * 16);
        f32 *off = (f32 *)(m + 0x8F0 + i * 16);
        s32 *len = &AT(m, 0x930 + i * 4, s32);
        s32 *left = &AT(m, 0x940 + i * 4, s32);

        AT(m, 0x8E4 + i, u8) = AT(m, 0x8E0 + i, u8);
        if (*left == 0 && AT(m, 0xB80 + i, u8) != 0 && AT(m, 0xB84 + i, u8) != 0) {
            AT(m, 0x8E0 + i, u8) = 1;
            continue;
        }
        foot_at(m, i, at);
        if (AT(m, 0x8E4 + i, u8) != 0) {
            *len = 5;
            *left = 4;
            sceVu0SubVector(off, foot, at);
        }
        if (*left == 0) {
            sceVu0CopyVector(foot, at);
        } else {
            f32 k = (f32)*left / (f32)*len;

            k = k * k;
            d[0] = off[0] * k;
            d[1] = off[1] * k;
            d[2] = off[2] * k;
            d[3] = 0.0f;
            sceVu0AddVector(foot, at, d);
            (*left)--;
        }
        AT(m, 0x8E0 + i, u8) = 0;
    }
    DogModel_LegIK(m, (f32 *)(m + 0x8A0), (f32 *)(m + 0x8B0), (f32 *)(m + 0x8C0), (f32 *)(m + 0x8D0));
}

/* the legs' IK set up from the bind pose, the feet put on the targets, solved keeping their
   poses */
/* 0x001F8450 */
void DogModel_LegIK(u8 *m, f32 *fr, f32 *fl, f32 *hr, f32 *hl) {
    f32 d[4] __attribute__((aligned(16)));
    u8 *b = AT(m, 0x4C0, u8 *) + 0x10;

    sceVu0SubVector(d, (f32 *)(b + 0x3E0), (f32 *)(b + 0x530));
    IK3_Setup(m + 0x960, SKEL(m), 8, 9, 10, 11, BIND(m, 9, 0x20), BIND(m, 10, 0x20), BIND(m, 11, 0x20), 1.0f,
                  -1.0f, __builtin_sqrtf(__builtin_fabsf(d[1] * d[1] + d[0] * d[0] + d[2] * d[2])));
    sceVu0SubVector(d, (f32 *)(b + 0x5A0), (f32 *)(b + 0x6F0));
    IK3_Setup(m + 0x9F0, SKEL(m), 12, 13, 14, 15, BIND(m, 13, 0x20), BIND(m, 14, 0x20), BIND(m, 15, 0x20),
                  1.0f, -1.0f, __builtin_sqrtf(__builtin_fabsf(d[1] * d[1] + d[0] * d[0] + d[2] * d[2])));
    IK2_Setup(m + 0xA80, SKEL(m), 0x15, 0x16, 0x17, BIND(m, 0x16, 0x20), BIND(m, 0x17, 0x20), 0.0f);
    IK2_Setup(m + 0xAE0, SKEL(m), 0x1A, 0x1B, 0x1C, BIND(m, 0x1B, 0x20), BIND(m, 0x1C, 0x20), 0.0f);
    sceVu0CopyVector((f32 *)(m + 0x980), fr);
    sceVu0CopyVector((f32 *)(m + 0xA10), fl);
    sceVu0CopyVector((f32 *)(m + 0xAA0), hr);
    sceVu0CopyVector((f32 *)(m + 0xB00), hl);
    VCALL(m + 0x960 + 0x58, 0xC, void (*)(u8 *))(m + 0x960);
    VCALL(m + 0x9F0 + 0x58, 0xC, void (*)(u8 *))(m + 0x9F0);
    VCALL(m + 0xA80 + 0x58, 0xC, void (*)(u8 *))(m + 0xA80);
    VCALL(m + 0xAE0 + 0x58, 0xC, void (*)(u8 *))(m + 0xAE0);
}

/* +0x40: the body frames for character `a`: the front one from his position to the floor
 * point `ahead` along him, the back one from the floor point `behind` (negative: behind him)
 * to his position; both his own matrix when not on the floor */
/* 0x001F8650 */
void DogModel_BodyFrames(u8 *m, u8 *a, f32 ahead, f32 behind) {
    f32 floor[4] __attribute__((aligned(16)));
    f32 mtx[4][4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));

    sceVu0CopyVector(floor, (f32 *)(a + 0x10));
    VCALL(gNavMesh, 0x14, void (*)(NavMesh *, u32, f32 *))(gNavMesh, AT(a, 0x34, u32), floor);
    sceVu0CopyMatrix(mtx, (f32 (*)[4])(a + 0x60));
    sceVu0CopyVector(mtx[3], (f32 *)(a + 0x10));   /* at his position */
    AT(m, 0x80C, f32) = 1.0f;
    if (!on_floor(a, floor, ahead, behind)) {
        sceVu0CopyMatrix((f32 (*)[4])(m + 0x7D0), mtx);
        vu0_CopyXYZ((f32 *)(m + 0x800), (f32 *)(a + 0x10));
        AT(m, 0x80C, f32) = 1.0f;
        sceVu0CopyMatrix((f32 (*)[4])(m + 0xB40), mtx);
        return;
    }
    floor_point(m, a, mtx, p, ahead);
    sceVu0SubVector(p, p, (f32 *)(a + 0x10));
    sceVu0Normalize(p, p);
    frame_along((f32 (*)[4])(m + 0xB40), p);
    floor_point(m, a, mtx, p, behind);
    sceVu0SubVector(p, (f32 *)(a + 0x10), p);
    sceVu0Normalize(p, p);
    frame_along((f32 (*)[4])(m + 0x7D0), p);
    vu0_CopyXYZ((f32 *)(m + 0x800), (f32 *)(a + 0x10));
}

/* +0x48: how level the floor under him runs along his body (the horizontal part of the unit
 * direction between the floor points `ahead` and `behind` along him); 1 when not on
 * the floor */
/* 0x001F8910 */
f32 DogModel_FloorLevel(u8 *m, u8 *a, f32 ahead, f32 behind) {
    f32 floor[4] __attribute__((aligned(16)));
    f32 mtx[4][4] __attribute__((aligned(16)));
    f32 pb[4] __attribute__((aligned(16)));
    f32 pf[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));

    sceVu0CopyVector(floor, (f32 *)(a + 0x10));
    VCALL(gNavMesh, 0x14, void (*)(NavMesh *, u32, f32 *))(gNavMesh, AT(a, 0x34, u32), floor);
    sceVu0CopyMatrix(mtx, (f32 (*)[4])(a + 0x60));
    sceVu0CopyVector(mtx[3], (f32 *)(a + 0x10));   /* at his position */
    AT(m, 0x80C, f32) = 1.0f;
    if (!on_floor(a, floor, ahead, behind)) {
        return 1.0f;
    }
    pb[0] = 0.0f;
    pb[1] = 0.0f;
    pb[2] = ahead;
    pb[3] = 1.0f;
    sceVu0ApplyMatrix(pb, mtx, pb);
    pf[0] = 0.0f;
    pf[1] = 0.0f;
    pf[2] = behind;
    pf[3] = 1.0f;
    sceVu0ApplyMatrix(pf, mtx, pf);
    Motion_OntoFloor(m, pb, a);
    Motion_OntoFloor(m, pf, a);
    sceVu0SubVector(d, pb, pf);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(pb, d, ahead);
    sceVu0AddVector(pb, pb, (f32 *)(a + 0x10));
    sceVu0ScaleVector(pf, d, behind);
    sceVu0AddVector(pf, pf, (f32 *)(a + 0x10));
    Motion_OntoFloor(m, pb, a);
    Motion_OntoFloor(m, pf, a);
    sceVu0SubVector(d, pb, pf);
    sceVu0Normalize(d, d);
    return __builtin_sqrtf(__builtin_fabsf(d[2] * d[2] + d[0] * d[0]));
}

/* ---- the feet ---- */

/* +0x64: foot `foot` (0..3) on the ground at the motion's time + dt: each of the two blended
 * motions (0xA0 apart) samples its contact track for that pair of legs; the foot is planted
 * when both say so, or the current one (+0x540) when only one has the track */
/* 0x001F8B50 */
u8 DogModel_FootDown(u8 *m, s32 foot, s32 dt) {
    f32 c[2][4] __attribute__((aligned(16))) = { { 0 } };
    s32 pair = (foot >> 1) * 8;
    void **trkA = &AT(m, 0x5BC + pair, void *);
    void **trkB = &AT(m, 0x65C + pair, void *);
    s32 i;

    for (i = 0; i < 2; i++) {
        s32 *trk = AT(m, 0x5BC + pair + i * 0xA0, s32 *);
        f32 t, n;

        if (trk == NULL || *trk == 0) {
            continue;
        }
        n = (f32)AT(AT(AT(m, 0x584 + i * 0xA0, u8 *), 0x4, u8 *), 0xC, s32);
        t = AT(m, 0x564 + i * 0xA0, f32) + (f32)dt;
        while (t < 0.0f) {
            t += n;
        }
        while (!(t < n)) {
            t -= n;
        }
        Track_Sample(trk, c[i], t);
    }
    if (*trkA != NULL && **(s32 **)trkA != 0 && *trkB != NULL && **(s32 **)trkB != 0) {
        return !(c[0][foot & 1] <= 0.0f) && !(c[1][foot & 1] <= 0.0f);
    }
    return !(c[AT(m, 0x540, s32)][foot & 1] <= 0.0f);
}

/* +0x14: adjust bone `bone`'s matrix `b` as it is built (`parent`: for the root, the motion's
 * own rotation): the root (0) on the back frame; bone 16 (the shoulders) on the front frame;
 * the spine (18) bent with the slope; neck and head (29, 30, 31) turned */
/* 0x001F8D40 */
void DogModel_AdjustBone(u8 *m, s32 bone, f32 (*b)[4], f32 (*parent)[4]) {
    f32 t[4][4] __attribute__((aligned(16)));
    f32 u[4][4] __attribute__((aligned(16)));
    f32 r[4][4] __attribute__((aligned(16)));
    f32 pos[4] __attribute__((aligned(16)));

    switch (bone) {
    case 0:
        sceVu0CopyMatrix(t, (f32 (*)[4])(m + 0x7D0));
        t[3][0] = t[3][1] = t[3][2] = 0.0f;
        sceVu0MulMatrix(t, t, parent);
        t[3][0] = t[3][1] = t[3][2] = 0.0f;
        sceVu0UnitMatrix(u);
        sceVu0CopyVector(u[3], b[3]);
        u[3][3] = 1.0f;
        sceVu0MulMatrix(b, u, t);
        break;
    case 0x10:
        sceVu0CopyMatrix(t, (f32 (*)[4])(m + 0x7D0));
        t[3][0] = t[3][1] = t[3][2] = 0.0f;
        sceVu0InversMatrix(t, t);
        sceVu0CopyMatrix(u, b);
        sceVu0CopyVector(pos, u[3]);
        u[3][0] = u[3][1] = u[3][2] = 0.0f;
        sceVu0MulMatrix(r, t, u);
        sceVu0MulMatrix(b, (f32 (*)[4])(m + 0xB40), r);
        sceVu0CopyVector(b[3], pos);
        break;
    case 0x12:
        if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 0x800) {
            AT(m, 0xB88, f32) += 0.25f;
            if (!(AT(m, 0xB88, f32) <= 1.0f)) {
                AT(m, 0xB88, f32) = 1.0f;
            }
        } else {
            AT(m, 0xB88, f32) -= 0.25f;
            if (AT(m, 0xB88, f32) < 0.0f) {
                AT(m, 0xB88, f32) = 0.0f;
            }
        }
        bone_turn(b, (f32 *)(m + 0xB40), 0.0f, (f32 *)(m + 0xB50), 0.25f * -AT(m, 0x858, f32) * AT(m, 0xB88, f32));
        break;
    case 0x1D:
        neck_turn(m, b, 0x12, 0x1.99999a0000000p-2f /* 0.4 */);
        break;
    case 0x1E:
        neck_turn(m, b, 0x1D, 0x1.99999a0000000p-2f /* 0.4 */);
        break;
    case 0x1F:
        neck_turn(m, b, 0x1E, 0x1.99999a0000000p-3f /* 0.2 */);
        break;
    }
}

/* +0x10 */
/* 0x001F9560 */
void DogModel_Frame(u8 *m) {
    Model_Frame(m);
}

/* ---- small methods ---- */

/* +0xC: once loaded: the plain model's setup, then his own state cleared */
/* 0x001F9570 */
void DogModel_Loaded(u8 *m) {
    s32 i;

    Model_Loaded(m);
    AT(m, 0x950, u8) = 0;
    AT(m, 0xB88, f32) = 0.0f;
    AT(m, 0x854, f32) = 0.0f;
    AT(m, 0x858, f32) = 0.0f;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 0x1.6666660000000p+2f /* 5.6 */;
    AT(m, 0x868, f32) = 3.0f;
    for (i = 0; i < 4; i++) {
        f32 *v = (f32 *)(m + 0x8F0 + i * 16);

        v[0] = v[1] = v[2] = 0.0f;
        v[3] = 1.0f;
        AT(m, 0x930 + i * 4, s32) = 0;
        AT(m, 0x940 + i * 4, s32) = 0;
        AT(m, 0x8E0 + i, u8) = 0;
        AT(m, 0xB80 + i, u8) = 0;
    }
}

/* 0x002080D0 */
void *DogModelB_ctor(u8 *m, u8 kind) {   /* Hewie, kind 2 (event 0xB7) */
    return dog_model(m, kind, DogModelB_vtable);
}

/* an element of Hewie's model's first array (0x90 bytes) */
/* 0x00208160 */
void *DogModelArray_ctor(void *p) {
    u8 *e = p;

    AT(e, 0x58, void **) = IK3_vtable;
    return e;
}

/* 0x00208180 */
void *DogModelA_ctor(u8 *m, u8 kind) {   /* Hewie, kind 1 (event 0xB7) */
    return dog_model(m, kind, DogModelA_vtable);
}

/* the dog model's constructor (the plain base by HumanModel_BaseCtor), kind `kind` (+0x890) */
/* 0x00208210 */
void *DogModel_ctor(u8 *m, u8 kind) {
    HumanModel_BaseCtor(m);
    AT(m, 0x0, void **) = DogModel_vtable;
    AT(m, 0x890, u8) = kind;
    __construct_array(m + 0x960, DogModelArray_ctor, IK3_Destroy, 0x90, 2);
    __construct_array(m + 0xA80, IK2_ctor, IK2_Destroy, 0x60, 2);
    return m;
}

/* 0x0020BDE0 */
void *DogModelA_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = DogModelA_vtable;
        HewieModel_Destroy(m, flags);
    }
    return m;
}

/* (vtable DogModelA_vtable +0xA0) its table for kind k (0..4), NULL for others */
/* 0x0020BEF0 */
u8 *DogModelA_KindTable(u8 *m, s32 k) {
    switch (k) {
    case 0: return D_00456F50;
    case 1: return str_O_HEG_HEG_001_PCK;
    case 2: return str_O_HEG_HEG_002_PCK;
    case 3: return str_O_HEG_HEG_003_PCK;
    case 4: return str_O_HEG_HEG_004_PCK;
    }
    return NULL;
}

/* 0x0020BF70 */
void *DogModelA_Table(void) {
    return D_00456F70;
}

/* 0x0020BF80 */
void *DogModelB_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = DogModelB_vtable;
        HewieModel_Destroy(m, flags);
    }
    return m;
}

/* (vtable DogModelB_vtable's) the same for the other model */
/* 0x0020C090 */
u8 *DogModelB_KindTable(u8 *m, s32 k) {
    switch (k) {
    case 0: return D_00456E90;
    case 1: return str_O_HED_HED_001_PCK;
    case 2: return str_O_HED_HED_002_PCK;
    case 3: return str_O_HED_HED_003_PCK;
    case 4: return str_O_HED_HED_004_PCK;
    }
    return NULL;
}

/* 0x0020C110 */
void *DogModelB_Table(void) {
    return D_00456EB0;
}

/* vtable +0x28: place him (as Character) on triangle `tri`; (in play) his default animation;
 * reset his per-placement state. Returns the placement result. */
/* 0x001683D0 */
s32 Hewie_PlaceOn(Hewie *h, u32 tri, const f32 *heading, f32 *pos) {
    s32 r = Character_Place(&h->c, tri, heading, pos);

    if (*((u8 *)gProgress + 0x1FBEC1) == 0) {
        Motion_Play(h->c.motion, HW(h, 0xF36F0, s32), -1);
        VCALL(h->c.motion, 0x50, void (*)(void *, Hewie *))(h->c.motion, h);
    }
    HW(h, 0xF3608, s32) = 0;
    HW(h, 0xF3600, s32) = 4;
    HW(h, 0xF361C, s32) = 0;
    HW(h, 0xF3660, u8) = 1;
    HW(h, 0xF3661, u8) = 1;
    HW(h, 0xF3662, u8) = 1;
    HW(h, 0xF3663, u8) = 1;
    HW(h, 0xF3568, s32) = HEWIE_ACTION(h);
    HW(h, 0xF354C, f32) = h->c.a.angle[1];
    h->c.a.prevNavTri = tri;
    sceVu0CopyVector(h->c.a.prevPos, h->c.a.pos);
    HW(h, 0xF35DC, s32) = 0;
    HW(h, 0xF3550, s32) = 0;
    HW(h, 0xF3554, s32) = 1;
    HW(h, 0xF3580, u8) = 0;
    HW(h, 0xF3664, s32) = 0;
    HW(h, 0xF3581, u8) = 0;
    HW(h, 0xF3582, u8) = 1;
    HW(h, 0xF3584, u8) = 0;
    HW(h, 0xF358C, s32) = 0;
    HW(h, 0xF357C, s32) = 0;
    HW(h, 0xF368C, s32) = 0;
    HW(h, 0xF3585, u8) = 0;
    HW(h, 0xF3586, u8) = 0;
    HW(h, 0xF3587, u8) = 1;
    HW(h, 0xF35A0, s16) = 0;
    HW(h, 0xF3548, s32) = 0;
    HW(h, 0xF3620, u8) = 0;
    HW(h, 0xF36A4, s32) = 0;
    HW(h, 0xF36AC, s32) = 0;
    Lists_Clear((u8 *)h + 0xF3748);
    HW(h, 0xF3798, s32) = -1;
    return r;
}

extern u32 Hewie_Dares(Hewie *h, s32 slot);         /* u8 */

/* vtable +0x68: may character `slot` start interaction `kind` with him now (kind 5: through
 * `door`)? For kinds 1..4, if he stands idle facing roughly towards the caller (within 3pi/8),
 * he may react himself instead (+0xF3584, answer no). */
/* 0x00165A40 */
s32 Hewie_CanInteract(Hewie *h, u32 kind, s32 slot, u32 door) {
    u32 k;

    if (h->c.moveMode == 4 && (kind & 0xFF) != 0xB) {
        return 0;
    }
    if (h->c.moveMode == 8 && (u32)(h->c.moveSub - 0x18) < 2) {
        return 0;
    }
    k = kind & 0xFF;
    if (HEWIE_ACTION(h) == 0x76) {
        return 0;
    }
    if (k != 5 && HEWIE_ACTION(h) == 0x65) {
        return 0;
    }
    if (!(Progress_TestFlag(gProgress, 0x1D) & 0xFF) && h->c.moveMode == 0 && k >= 1 && k <= 4
        && h->c.a.unkC4 != 2 && HEWIE_ACTION(h) != 0x79
        && Angle_Wrap(Actor_HeadingTo(&h->c.a, gCharacters[slot]->a.pos) - h->c.a.angle[1]) < 0x1.2d97c8p+0f /* 3pi/8 */
        && (Hewie_Dares(h, slot) & 0xFF) == 1) {
        HW(h, 0xF3584, u8) = 1;
        return 0;
    }
    if (k != 5) {
        return 1;
    }
    return (VCALL(gDoors, 0x40, u32 (*)(VObject *, u32))(gDoors, door & 0xFF) & 0xFF) ? 1 : 0;
}

extern void Hewie_Alert(Hewie *h);

#define Hewie_Place(h, tri) VCALL(h, 0x28, s32 (*)(Hewie *, u32, const f32 *, f32 *))(h, tri, NULL, NULL)

/* Placement with his default action (inlined twice in the original). */
static inline s32 Hewie_PlaceDefault(Hewie *h, u32 tri) {
    s32 r;

    MOTION_PTR(h->c.motion, 0x858) = NULL;
    MOTION_PTR(h->c.motion, 0x854) = NULL;
    HW(h, 0xF36F0, s32) = 0;
    r = Hewie_Place(h, tri);
    Character_ChooseExit(&h->c, 0xFF);
    Hewie_ToDefault(h);
    return r;
}

/* vtable +0x64: put him in room `room` on triangle `tri` (side `side`). In the room being played:
 * placed, default action, handed to the room objects, nearby state updated; elsewhere he only
 * keeps the triangle. Returns the placement result (0 elsewhere). */
/* 0x00166530 */
s32 Hewie_PlaceInRoom(Hewie *h, s32 room, u32 tri, s32 side) {
    Progress *p;
    s32 r;

    Character_ToRoom(&h->c, room, tri, side);
    p = gProgress;
    HEWIE_SIDE(h) = side;
    if (*((u8 *)p + 0x1FBEC1) != 0) {
        return Hewie_PlaceDefault(h, tri);
    }
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        h->c.a.navTri = tri;
        Motion_PlayTable(h->c.motion, 0, -1);
        Hewie_ToDefault(h);
        return 0;
    }
    r = Hewie_PlaceDefault(h, tri);
    VCALL(gEvents, 0x2C, void (*)(VObject *, Hewie *))(gEvents, h);
    if (h->c.a.navTri == NAV_NONE) {
        h->c.a.pos[0] = 0.0f;
        h->c.a.pos[1] = 0.0f;
        h->c.a.pos[2] = 0.0f;
        h->c.a.pos[3] = 0x1.99999ap-4f;   /* 0.1 */
    }
    {
        sceVu0FVECTOR v;

        if (Actor_PosInCurrentRoom(&h->c.a, v)) {
            Character_Sound(&h->c, 3, (s32)v, 0, 0, 0);
        }
    }
    Hewie_Alert(h);
    HW(h, 0xF366C, u8) = HW(h, 0xF366D, u8);
    Character_MarkObjects(&h->c);
    return r;
}

extern u8 gPadState[];                                   /* pad state */
extern void Hewie_TurnHead(Hewie *h);
extern void Hewie_AnimSounds(Hewie *h);
extern void Hewie_Feet(Hewie *h);

/* character c active, not down, in his room (in the room being played only on the mesh) */
static s32 in_his_room(Hewie *h, Character *c) {
    if (c == NULL || c->a.active != 1 || c->a.unkC4 == 2) {
        return 0;
    }
    return h->c.a.room == c->a.room &&
           (h->c.a.room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) || c->a.navTri != (u32)-1);
}

/* tell progress how close Fiona is (+0x7B9: 1 within 20, 2 within 50, 3 further) */
static void report_fiona_near(Hewie *h) {
    if (in_his_room(h, gCharPlayer)) {
        f32 d = Actor_Distance(&h->c.a, gCharPlayer->a.pos);

        if (d <= 20.0f) {
            *((u8 *)gProgress + 0x7B9) = 1;
        } else if (d <= 50.0f) {
            *((u8 *)gProgress + 0x7B9) = 2;
        } else {
            *((u8 *)gProgress + 0x7B9) = 3;
        }
    }
}

/* heading turned by the animation, orientation rebuilt */
static void turn_by_anim(Hewie *h) {
    f32 yaw = Angle_Wrap(h->c.a.angle[1] + Motion_RootRotation(h->c.motion, 0.0f));

    h->c.a.angle[1] = yaw;
    sceVu0UnitMatrix(h->c.a.rot);
    sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, yaw);
}

/* move by the animation's root motion (no rise, forward scaled by the motion's speed factor) */
static void root_motion(Hewie *h) {
    sceVu0FVECTOR root;
    f32 k;

    Motion_RootMovement(h->c.motion, root, 0.0f);
    k = VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    *(s32 *)&root[1] = 0;
    root[2] = root[2] * k;
    sceVu0ApplyMatrix(root, h->c.a.rot, root);
    Actor_Move(&h->c.a, root);
}

/* vtable +0x44: per-frame update - frame counter (up to 3000), surroundings, how close Fiona
 * is (gProgress +0x7B9: 1 within 20, 2 within 50, 3 further), requests, behaviour, turning and
 * root motion from the animation, then his sub-systems. */
/* 0x00167760 */
void Hewie_Think(Hewie *h) {
    Progress *p;

    h->c.a.navMask = h->c.a.unk2B ? 0 : HEWIE_NAV_MASK;
    h->c.pathReq->mask = h->c.a.navMask;
    HW(h, 0xF35A8, s16) += 1;
    if (HW(h, 0xF35A8, s16) > 3000) {
        HW(h, 0xF35A8, s16) = 3000;
    }
    Hewie_Alert(h);
    if (h->c.a.unkC4 != 2) {
        report_fiona_near(h);
    }
    VCALL(h, 0x88, void (*)(Hewie *))(h);
    p = gProgress;
    if (*((u8 *)p + 0x1FBEC1) == 1) {
        HW(h, 0xF3798, s32) = Gesture_Update((u8 *)h + 0xF3748, (f32 *)gPadState);
    }
    HW(h, 0xF3558, u8) = 0;
    HW(h, 0xF3582, u8) = 1;
    ptmf_scall(h, HEWIE_STATE(h));
    turn_by_anim(h);
    if (!h->c.a.disabled && !HW(h, 0xF3558, u8)) {
        root_motion(h);
    }
    if ((Progress_HasRelationCmd(p, SLOT_U8(h)) & 0xFF) == 1) {
        SlotCmd_Cancel(p, SLOT_U8(h));
    }
    Hewie_TurnHead(h);
    Hewie_AnimSounds(h);
    VCALL(h, 0x40, void (*)(Hewie *))(h);
    Hewie_Feet(h);
}

extern const s32 kHewieLevelNormal[];   /* by +0xF35CC (normal) */
extern const s32 kHewieLevelHard[];   /* by +0xF35CC (difficulty 1) */
extern const PTMF Hewie_StateCalm_ptmf;    /* idle state */
extern const PTMF Hewie_StatePending_ptmf;    /* special-mode state */

static inline void Hewie_StartScene(Hewie *h, Progress *p) {
    if (*((u8 *)p + 0x1FBEC1) == 0) {
        HW(h, 0xF35C4, s32) = 0;
        HW(h, 0xF36AC, s32) = 0;
        HW(h, 0xF36A4, s32) = 0;
        HW(h, 0xF366D, u8) = 0;
        Actor_SetState(&h->c.a, &Hewie_StateCalm_ptmf);
        Hewie_SetAction(h, 0, 0);
        return;
    }
    HW(h, 0xF3710, u8) = 0;
    HW(h, 0xF3718, s32) = 6;
    HW(h, 0xF3730, s32) = 0;
    HW(h, 0xF3734, s32) = 0;
    HW(h, 0xF3738, s32) = 0;
    HW(h, 0xF373C, s32) = 0;
    HW(h, 0xF3712, s16) = 0;
    Actor_SetState(&h->c.a, &Hewie_StatePending_ptmf);
    Motion_Play(h->c.motion, 0, -1);
    VCALL(h->c.motion, 0x50, void (*)(void *, Hewie *))(h->c.motion, h);
    Hewie_SetAction(h, 0x83, 0);
}

/* vtable +0x5C: activate (Character part), then reset his own state; in play idle with action 0,
 * in the special mode the special state with action 0x83. */
/* 0x00165D40 */
void Hewie_Activate(Hewie *h) {
    Progress *p;

    Character_Activate(&h->c);
    Hewie_SetMode(h, 0, -1);
    Hewie_AddTrust(h, 0);
    p = gProgress;
    HW(h, 0xF3598, s32) = 0;
    if ((Progress_GetVar(p, 0x27) & 0xFF) == 1) {
        HW(h, 0xF359C, s32) = kHewieLevelHard[HW(h, 0xF35CC, s16)];
    } else {
        HW(h, 0xF359C, s32) = kHewieLevelNormal[HW(h, 0xF35CC, s16)];
    }
    HW(h, 0xF3586, u8) = 0;
    HW(h, 0xF3559, u8) = 1;
    HW(h, 0xF3590, u8) = 0;
    h->c.hp = h->c.hpMax;
    HW(h, 0xF35AC, s32) = 300;
    HW(h, 0xF3640, s32) = 0;
    HW(h, 0xF3648, s32) = 0;
    HW(h, 0xF3650, s32) = 1;
    HW(h, 0xF364C, s32) = 0;
    HW(h, 0xF3654, s32) = 4;
    HW(h, 0xF3658, s32) = 0;
    HW(h, 0xF365C, s32) = 0;
    HW(h, 0xF35C4, s32) = 0;
    HW(h, 0xF35C8, s32) = 0;
    HW(h, 0xF3610, s32) = 0xFF;
    HEWIE_SIDE(h) = 2;
    HW(h, 0xF368A, u8) = 0;
    HW(h, 0xF35B4, s32) = -1;
    HW(h, 0xF3583, u8) = 0;
    HW(h, 0xF3589, u8) = 0;
    HW(h, 0xF3594, s32) = -1;
    HW(h, 0xF35A4, s32) = -1;
    HW(h, 0xF35A8, s16) = 0;
    HW(h, 0xF35BE, s16) = 0;
    HW(h, 0xF3684, s16) = 0;
    HW(h, 0xF3686, s16) = 0;
    HW(h, 0xF355C, s32) = 0;
    HW(h, 0xF36B0, s32) = 0xFF;
    HW(h, 0xF35E0, u8) = 0;
    HW(h, 0xF3588, u8) = 0;
    HW(h, 0xF3688, s16) = 0;
    HW(h, 0xF35B0, s16) = 0;
    HW(h, 0xF358C, s32) = 0;
    HW(h, 0xF3744, u8) = 0;
    HW(h, 0xF35DC, s32) = 0;
    Hewie_StartScene(h, p);
}

/* (Re)start: in play idle with action 0, in the special mode the special state, action 0x83. */
/* (possibly dead code: nothing in the game references it) */
/* 0x00136620 */
void Hewie_Restart(Hewie *h) {
    Hewie_StartScene(h, gProgress);
}

#define HEWIE_LAST_SOUND(h) HW(h, 0xF35A4, s32)
#define HEWIE_SOUND_T(h) HW(h, 0xF35A8, s16)      /* frames since then (up to 3000) */

/* Make sound `snd` (not within 10 frames of the last one; some repeat only after 40..60
 * frames). Barks 0x65/0x66 (loud) and 0x5D/0x5E also make a noise others can hear. */
/* 0x0013A430 */
void Hewie_MakeSound(Hewie *h, s32 snd) {
    s32 prev = HEWIE_LAST_SOUND(h);

    if (prev != 0x59 && prev != 0x58 && prev != 0x70 && prev != 0x6F && HEWIE_SOUND_T(h) < 10) {
        return;
    }
    switch (snd) {
    case 0x65: case 0x66:
        Noise_Make((u8 *)gProgress + 0x788, 0x80, h->c.a.room, h->c.a.navTri, 0xFFFF);
        break;
    case 0x5D: case 0x5E:
        Noise_Make((u8 *)gProgress + 0x788, 0x1B, h->c.a.room, h->c.a.navTri, 0xFFFF);
        break;
    case 0x59:
        if (prev == 0x59 && HEWIE_SOUND_T(h) < 40) {
            return;
        }
        break;
    case 0x58: case 0x6F: case 0x70:
        if (prev == 0x58) {
            if (HEWIE_SOUND_T(h) < 50) {
                return;
            }
        } else if (prev == 0x70) {
            if (HEWIE_SOUND_T(h) < 60) {
                return;
            }
        } else if (prev == 0x6F) {
            if (HEWIE_SOUND_T(h) < 40) {
                return;
            }
        }
        break;
    }
    Actor_PlaySound(&h->c.a, snd, 5, 0, 0, NULL);
    HEWIE_LAST_SOUND(h) = snd;
    HEWIE_SOUND_T(h) = 0;
}

#define F_PI 0x1.921fb6p+1f   /* 0x40490FDB */

/* Best heading near `yaw` for walking `dist`: tries yaw +- `from`..`to` degrees in steps of `step`
 * (left and right, starting on a random side), stops early at a full-length result. */
/* 0x00137720 */
f32 Hewie_BestHeading(Hewie *h, f32 yaw, f32 dist, s32 from, s32 to, s32 step) {
    f32 best = Actor_FreeDistance(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, yaw, dist);
    f32 result = yaw;
    f32 sign;
    s32 deg;

    if (best == dist) {
        return result;
    }
    sign = (RNG01() < 0.5f) ? 1.0f : -1.0f;
    if (from < to) {
        for (deg = from; !(to < deg); deg += step) {
            f32 ang = (F_PI * (f32)deg) / 180.0f;
            s32 i;

            for (i = 0; i < 2; i++) {
                f32 a = Angle_Wrap(yaw + sign * ang);
                f32 r = Actor_FreeDistance(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, a, dist);

                if (best < r) {
                    result = a;
                    best = r;
                    if (r == dist) {
                        deg = to;
                        break;
                    }
                }
                sign *= -1.0f;
            }
        }
    } else {
        for (deg = from; !(deg < to); deg -= step) {
            f32 ang = (F_PI * (f32)deg) / 180.0f;
            s32 i;

            for (i = 0; i < 2; i++) {
                f32 a = Angle_Wrap(yaw + sign * ang);
                f32 r = Actor_FreeDistance(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, a, dist);

                if (best < r) {
                    result = a;
                    best = r;
                    if (r == dist) {
                        deg = to;
                        break;
                    }
                }
                sign *= -1.0f;
            }
        }
    }
    return result;
}

/* Start his standing animation (0 normal, 3 / 4 / 5 by progress state and mode, 6 in condition
 * 1, 4 during actions 8 / 0xA) unless it is already playing; `blend` -1: cut. */
/* 0x00143550 */
void Hewie_StandAnim(Hewie *h, s32 blend) {
    Progress *p;
    s32 cur, a;

    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
    p = gProgress;
    cur = MOTION_ANIM(h->c.motion);
    if ((Progress_GameMode(p) & 0xFF) == 2 && HEWIE_MODE(h) == 3) {
        a = 4;
    } else if (h->c.a.unkC4 == 1) {
        a = 6;
    } else if ((Progress_GameMode(p) & 0xFF) == 2 && HEWIE_MODE(h) == 2) {
        a = 5;
    } else if ((Progress_GameMode(p) & 0xFF) && Progress_CondBit(p, 4)) {
        a = 4;
    } else if (Progress_GameMode(p) & 0xFF) {
        a = 3;
    } else if (HEWIE_ACTION(h) == 8 || HEWIE_ACTION(h) == 0xA) {
        a = 4;
    } else {
        a = 0;
    }
    if (cur == a) {
        return;
    }
    if (blend != -1) {
        Motion_PlayBlend(h->c.motion, a, blend, -1);
    } else {
        Motion_PlayTable(h->c.motion, a, -1);
    }
}

/* ---- small behaviour pieces ---- */

void Hewie_KeepPose(Hewie *h, u32 kind);
extern void Hewie_AfterYelp(Hewie *h);

#define MOTION_EVENTS(m) (*(s32 *)((u8 *)MOTION_PTR(m, 0x6A4) + 0x18))

/* 0x001480B0 */
void Hewie_StatePose2(Hewie *h) {
    Hewie_KeepPose(h, 2);
}

/* 0x0015F750 */
void Hewie_StatePose4(Hewie *h) {
    Hewie_KeepPose(h, 4);
}

/* 0x0014F5A0 */
void Hewie_State2318(Hewie *h) {
}

/* 0x00154D40 */
void Hewie_State20C8(Hewie *h) {
}

/* 0x00154DB0 */
void Hewie_State20A8(Hewie *h) {
}

/* 0x00154E60 */
void Hewie_State2018(Hewie *h) {
}

/* A loud noise where he is (no triangle). */
/* 0x00154E40 */
void Hewie_StateLoudNoise(Hewie *h) {
    Noise_Make((u8 *)gProgress + 0x788, 0x80, h->c.a.room, NAV_NONE, 0xFFFF);
}

/* Set the animation to play (+0xF35B8) and (if >= 0) +0xF35B4. */
/* 0x001654E0 */
void Hewie_SetAnim(Hewie *h, s32 a, s32 anim) {
    if (a >= 0) {
        HW(h, 0xF35B4, s32) = a;
    }
    HW(h, 0xF35B8, s32) = anim;
}

/* Start that animation if it is not playing. */
/* 0x0014F5B0 */
void Hewie_StatePlayAnim(Hewie *h) {
    if (MOTION_ANIM(h->c.motion) != HW(h, 0xF35B8, s32)) {
        Motion_PlayTable(h->c.motion, HW(h, 0xF35B8, s32), -1);
    }
}

/* On the animation's event 0x20: (clear +0x2B and) continue. */
/* 0x00153AB0 */
void Hewie_StateOnEvent20(Hewie *h) {
    if ((MOTION_EVENTS(h->c.motion) & 0x20) != 0) {
        h->c.a.unk2B = 0;
        Hewie_AfterYelp(h);
    }
}

/* Pending (+0xF3559): go to action 0x83, unless already in it. */
/* 0x001602A0 */
void Hewie_StatePending(Hewie *h) {
    if (HW(h, 0xF3559, u8) == 1 && HEWIE_ACTION(h) != 0x83) {
        Hewie_SetAction(h, 0x83, 0);
    }
}

/* Back to his default action. */
/* 0x00154D50 */
void Hewie_StateToDefault(Hewie *h) {
    Hewie_ToDefault(h);
}

/* Switch his behaviour (the pointer to member at +0xF35D0). */
static inline void Hewie_SetBehaviour(Hewie *h, const PTMF *s) {
    *HEWIE_STATE(h) = *s;
}

#define ANIM_DONE(h) ((MOTION_EVENTS((h)->c.motion) & 0x20) != 0)   /* animation event 0x20 */

extern s32 Hewie_StepToPose(Hewie *h, u32 kind);
extern s32 Hewie_PlayerSteering(Hewie *h);   /* u8 */
extern void Hewie_Bark(Hewie *h);
extern const PTMF Hewie_State1B60_ptmf, Hewie_State1A60_ptmf7, Hewie_State1B40_ptmf, Hewie_State19B0_ptmf, Hewie_StateAnimOver3_ptmf, Hewie_State1B00_ptmf3;

/* 0x0014E190 */
void Hewie_State1AA0(Hewie *h) {
    if (Hewie_StepToPose(h, 3) == 0) {
        Hewie_Bark(h);
        Hewie_SetBehaviour(h, &Hewie_State1B60_ptmf);
    }
}

/* 0x0014E200 */
void Hewie_State1B40(Hewie *h) {
    if (ANIM_DONE(h)) {
        HW(h, 0xF36BC, s32) = 90;
        Hewie_SetBehaviour(h, &Hewie_State1A60_ptmf7);
    }
}

/* 0x00154DC0 */
void Hewie_State2098(Hewie *h) {
    if (HW(h, 0xF35C8, s32) == 0) {
        Hewie_ToDefault(h);
    }
}

/* 0x0014E280 */
void Hewie_State1AF0(Hewie *h) {
    if (Hewie_StepToPose(h, 1) == 0) {
        Motion_PlayTable(h->c.motion, 0x1C04, -1);
        Hewie_SetBehaviour(h, &Hewie_State1B40_ptmf);
    }
}

/* 0x00151740 */
void Hewie_State22B8(Hewie *h) {
    if (Hewie_StepToPose(h, 1) == 0) {
        Motion_PlayTable(h->c.motion, 0x1C04, -1);
        Hewie_SetBehaviour(h, &Hewie_State19B0_ptmf);
    }
}

/* 0x0015BD10 */
void Hewie_State1770(Hewie *h) {
    if (Hewie_StepToPose(h, 1) == 0) {
        Motion_PlayTable(h->c.motion, 0x1C04, -1);
        Hewie_SetBehaviour(h, &Hewie_StateAnimOver3_ptmf);
    }
}

/* 0x0014E110 */
void Hewie_State1B60(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_KeepPose(h, 1);
        Hewie_SetBehaviour(h, &Hewie_State1B00_ptmf3);
    }
}

/* Animation over: back to his default action. */
/* 0x001506A0 */
void Hewie_StateAnimOver(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_ToDefault(h);
    }
}

/* 0x0015AD90 */
void Hewie_StateAnimOver2(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_ToDefault(h);
    }
}

/* 0x0015BC90 */
void Hewie_StateAnimOver3(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_ToDefault(h);
    }
}

/* 0x0014DFB0 */
void Hewie_State2358(Hewie *h) {
    if (ANIM_DONE(h)) {
        h->c.a.unk2D = 0;
        Hewie_ToDefault(h);
    }
}

/* Start action `act` as adjusted to his situation. */
static inline void Hewie_Start(Hewie *h, s32 act) {
    Hewie_SetAction(h, Hewie_AdjustAction(h, act), 0);
}

#define HEWIE_NEXT(h) HW(h, 0xF3570, s32)     /* action to continue with */

/* 0x00149D40 */
void Hewie_State1CC8(Hewie *h) {
    if (Hewie_StepToPose(h, 3) == 0) {
        h->c.unkE1 = 1;
        Hewie_KeepPose(h, 3);
        Hewie_ToDefault(h);
    }
}

/* 0x0014B860 */
void Hewie_State1C48(Hewie *h) {
    if (Hewie_StepToPose(h, 0) == 0) {
        h->c.unkE1 = 1;
        Hewie_KeepPose(h, 0);
        Hewie_ToDefault(h);
    }
}

/* Free to take a command: idle, a command queued (+0xF356C > 0), nothing pending. */
static inline s32 Hewie_FreeForCommand(Hewie *h) {
    if (!h->c.unkE0 && h->c.moveMode == 0 && HW(h, 0xF356C, s32) != 0 && !(HW(h, 0xF356C, s32) & 0x80000000)
        && !(Progress_IsLinked(gProgress, 1) & 0xFF) && h->c.state[0] == 0) {
        return 1;
    }
    return 0;
}

/* 0x00138FD0 */
s32 Hewie_FreeForCommand2(Hewie *h) {
    return Hewie_FreeForCommand(h);
}

/* 0x00150610 */
void Hewie_State22E8(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_Start(h, 0x52);
    }
    HW(h, 0xF3558, u8) = 1;
}

/* 0x001532B0 */
void Hewie_StateAnimNext(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_Start(h, HEWIE_NEXT(h));
    }
}

/* 0x001519B0 */
void Hewie_State19A0(Hewie *h) {
    if (--HW(h, 0xF36B4, s32) == 0) {
        Hewie_Start(h, HEWIE_NEXT(h));
    }
    Hewie_KeepPose(h, 3);
}

extern const PTMF Hewie_State19A0_ptmf, Hewie_State1A60_ptmf8;
extern s32 Hewie_PraiseScold(Hewie *h, s32 praise, s8 by);   /* u8 */

/* 0x00151A60 */
void Hewie_State1990(Hewie *h) {
    if (--HW(h, 0xF36B4, s32) == 0) {
        if (HW(h, 0xF3604, s32) != 4) {
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 10;
        }
        HW(h, 0xF36B4, s32) = 15;
        Hewie_SetBehaviour(h, &Hewie_State19A0_ptmf);
    }
    Hewie_KeepPose(h, 3);
}

/* Follow the animation's root motion, keeping the height it gives (not during action 0x47). */
/* 0x0014B4D0 */
void Hewie_StateRootMotion(Hewie *h) {
    if (HEWIE_ACTION(h) == 0x47) {
        HW(h, 0xF3558, u8) = 1;
        return;
    }
    if (h->c.a.unk2B == 1) {
        sceVu0FVECTOR root;
        f32 y = h->c.a.pos[1];

        Motion_RootMovement(h->c.motion, root, 0.0f);
        sceVu0ApplyMatrix(root, h->c.a.rot, root);
        Actor_Move(&h->c.a, root);
        h->c.a.pos[1] = y + root[1];
        HW(h, 0xF3558, u8) = 1;
        HW(h, 0xF3582, u8) = 0;
    }
}

/* 0x001531F0 */
void Hewie_State1F88(Hewie *h) {
    if (ANIM_DONE(h) && MOTION_ANIM(h->c.motion) == 0x1C01) {
        Hewie_ToDefault(h);
    } else if (Hewie_StepToPose(h, 1) == 0) {
        Motion_PlayTable(h->c.motion, 0x1C01, -1);
    }
}

/* `other` is active, not in condition 2, in his room and (in the room being played) on the
 * nav mesh. */
static inline s32 Hewie_WithChar(Hewie *h, Character *other) {
    u8 ok = (other != NULL && other->a.active == 1) ? 1 : 0;
    s32 room;

    if (ok != 1 || other->a.unkC4 == 2) {
        return 0;
    }
    room = h->c.a.room;
    if (room != other->a.room) {
        return 0;
    }
    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return 1;
    }
    return other->a.navTri != NAV_NONE;
}

/* 0x00137650 */
s32 Hewie_WithChar2(Hewie *h, Character *other) {
    return Hewie_WithChar(h, other);
}

/* 0x0014EB40 */
void Hewie_State1A60(Hewie *h) {
    if (--HW(h, 0xF36BC, s32) == 0) {
        HW(h, 0xF368C, s32) = 0;
        HW(h, 0xF36A8, s32) = HEWIE_ACTION(h);
        Hewie_PraiseScold(h, 1, 1);
        Hewie_ToDefault(h);
    }
    Hewie_KeepPose(h, 3);
}

#define HEWIE_TARGET(h) HW(h, 0xF3544, Character *)

/* 0x0014E040 */
void Hewie_State1B00(Hewie *h) {
    if (Hewie_StepToPose(h, 1) == 0) {
        Hewie_KeepPose(h, 1);
        HW(h, 0xF36BC, s32) = 90;
        HEWIE_TARGET(h) = gCharPlayer;
        if (HW(h, 0xF3604, s32) != 0) {
            HW(h, 0xF3604, s32) = 0;
            HW(h, 0xF3608, s32) = 10;
        }
        Hewie_SetBehaviour(h, &Hewie_State1A60_ptmf8);
    }
}

/* His move sub-mode: following a path (mode 6): 0x16 / 0x17 by +0xF3590; standing (mode 0):
 * by his animation group. */
/* 0x00144A60 */
void Hewie_MoveSubMode(Hewie *h) {
    if (h->c.moveMode == 6) {
        h->c.moveSub = (HW(h, 0xF3590, u8) == 1) ? 0x16 : 0x17;
        return;
    }
    if (h->c.moveMode != 0) {
        return;
    }
    switch (Hewie_AnimGroup(h)) {
    case 0: case 4: case 0xF:
        h->c.moveSub = 0;
        break;
    case 1: case 5:
        h->c.moveSub = 3;
        break;
    case 2: case 3: case 6: case 7: case 0xD:
        h->c.moveSub = 4;
        break;
    case 8:
        h->c.moveSub = 2;
        break;
    case 9: case 0xA: case 0xB: case 0xC:
        h->c.moveSub = 1;
        break;
    }
}

/* May his current activity be broken off (`once`: only the first time)? 0 yes, -1 no. Marks the
 * attempt (+0xF358C). */
/* 0x0013D4A0 */
s32 Hewie_MayBreakOff(Hewie *h, s32 once) {
    if (once && HW(h, 0xF358C, s32) == 1) {
        return -1;
    }
    if (h->c.unkE0 != 1 && (HW(h, 0xF356C, s32) & 0x80000008) != 8) {
        return -1;
    }
    if ((HW(h, 0xF3598, s32) != 0 && HEWIE_ACTION(h) != 0x7D) || HEWIE_MODE(h) == 3) {
        HW(h, 0xF358C, s32) = 1;
        return -1;
    }
    HW(h, 0xF358C, s32) = 1;
    return 0;
}

#define MOTION_SPEED(m) (*(f32 *)((u8 *)(m) + 0x550))

extern const PTMF Hewie_State1C58_ptmf, Hewie_State1CC8_ptmf2, Hewie_State1CC8_ptmf;

/* 0x0014B780 */
void Hewie_State23D8(Hewie *h) {
    u8 stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;

    if ((stopped ^ 1) == 0) {
        if (Hewie_AnimGroup(h) != 0) {
            Hewie_StandAnim(h, -1);
        } else {
            Motion_PlayTable(h->c.motion, 0x1C03, -1);
            HW(h, 0xF36B8, s32) = 30;
            Hewie_SetBehaviour(h, &Hewie_State1C58_ptmf);
        }
    }
    HW(h, 0xF3558, u8) = 1;
}

/* Character slot flags in the progress data (+0x1020 + 0x10 * slot). */
#define PROGRESS_SLOT_FLAGS(p, slot) (*((u8 *)(p) + 0x1020 + 0x10 * (slot)))

/* 0x001522F0 */
void Hewie_State1980(Hewie *h) {
    u8 b = PROGRESS_SLOT_FLAGS(gProgress, SLOT_U8(h));

    if ((b & 5) != 0 || h->c.unk104[0] != 0) {
        if (b & 4) {
            HW(h, 0xF3688, s16) = 300;
        }
        HW(h, 0xF36B0, s32) = 0xFF;
        Actor_PlaySound(&h->c.a, 0x6C, 5, 0, 0, NULL);
    }
    Hewie_ToDefault(h);
}

/* 0x0014B590 */
void Hewie_State1D88(Hewie *h) {
    u8 stopped;
    s32 g;

    if (h->c.a.unkC4 == 2) {
        if (Hewie_StepToPose(h, 10) == 0) {
            h->c.unkE1 = 1;
        }
        return;
    }
    stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;
    if ((stopped ^ 1) != 0) {
        return;
    }
    g = Hewie_AnimGroup(h);
    if (g == 0xA || g == 9 || g == 8) {
        Hewie_StandAnim(h, -1);
    } else if (g == 0) {
        h->c.unkE1 = 1;
    } else {
        Hewie_KeepPose(h, 0);
    }
}

/* 0x00149DD0 */
void Hewie_State2168(Hewie *h) {
    if (!(Progress_GameMode(gProgress) & 0xFF) && h->c.a.unkC4 != 1) {
        if (Hewie_StepToPose(h, 3) == 0) {
            Hewie_Bark(h);
            Hewie_SetBehaviour(h, &Hewie_State1CC8_ptmf2);
        }
    } else if (Hewie_StepToPose(h, 0) == 0) {
        Hewie_Bark(h);
        Hewie_SetBehaviour(h, &Hewie_State1CC8_ptmf);
    }
}

extern const PTMF Hewie_StateAnimNext_ptmf2, Hewie_StateAnimNext_ptmf, Hewie_StatePraised_ptmf;

/* 0x00153350 */
void Hewie_State1F28(Hewie *h) {
    if (!(Progress_GameMode(gProgress) & 0xFF) && h->c.a.unkC4 != 1) {
        if (Hewie_StepToPose(h, 3) == 0) {
            Hewie_Bark(h);
            Hewie_SetBehaviour(h, &Hewie_StateAnimNext_ptmf2);
        }
    } else if (Hewie_StepToPose(h, 0) == 0) {
        Hewie_Bark(h);
        Hewie_SetBehaviour(h, &Hewie_StateAnimNext_ptmf);
    }
}

/* 0x00149EC0 */
void Hewie_State1C98(Hewie *h) {
    u8 stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;

    if ((stopped ^ 1) != 0) {
        return;
    }
    if (MOTION_ANIM(h->c.motion) == 0x1300) {
        if (!(Progress_GameMode(gProgress) & 0xFF)) {
            Motion_PlayBlend(h->c.motion, 0, 5, -1);
        } else {
            Motion_PlayBlend(h->c.motion, 3, 5, -1);
        }
        return;
    }
    h->c.unkE1 = 1;
    Hewie_ToDefault(h);
}

/* 0x00149270 */
void Hewie_State2198(Hewie *h) {
    switch (HEWIE_ACTION(h)) {
    case 0x4B:
        Motion_PlayTable(h->c.motion, 0x1C05, -1);
        break;
    case 0x4A:
        Motion_PlayTable(h->c.motion, 0x1D00, -1);
        HW(h, 0xF36BC, s32) = (h->c.hp == h->c.hpMax) ? 1 : 0;
        break;
    case 0x49:
        Motion_PlayTable(h->c.motion, 0x1C04, -1);
        break;
    }
    Hewie_SetBehaviour(h, &Hewie_StatePraised_ptmf);
}

/* 0x0014B680 */
void Hewie_State1C58(Hewie *h) {
    u8 stopped;

    if (Hewie_AnimGroup(h) != 0) {
        if (--HW(h, 0xF36B8, s32) == 0) {
            Hewie_StandAnim(h, -1);
        }
        return;
    }
    stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;
    if ((stopped ^ 1) == 0) {
        Hewie_Start(h, HW(h, 0xF36B4, s32));
    }
}

/* Move by the animation's root motion, through blocking triangles (nav mask 0x80001 off). */
static inline void Hewie_ForcedMove(Hewie *h) {
    sceVu0FVECTOR root;
    f32 k;

    h->c.a.navMask |= 0x80001;
    Motion_RootMovement(h->c.motion, root, 0.0f);
    k = VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    *(s32 *)&root[1] = 0;
    root[2] = root[2] * k;
    sceVu0ApplyMatrix(root, h->c.a.rot, root);
    Actor_Move(&h->c.a, root);
    HW(h, 0xF3558, u8) = 1;
    h->c.a.navMask &= ~0x80001;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* 0x00147480 */
void Hewie_State1D38(Hewie *h) {
    if (Hewie_StepToPose(h, 0) == 0) {
        Hewie_KeepPose(h, 0);
        Hewie_SetAction(h, 0, 0);
    }
    Hewie_ForcedMove(h);
}

/* Can he take a command now: active in the room being played, idle, not angry (mode 3), a
 * command queued (+0xF356C > 0; any in condition 2), nothing pending. */
/* 0x00138EC0 */
s32 Hewie_CanTakeCommand(Hewie *h) {
    Progress *p;
    s32 room;

    if (h->c.a.active != 1) {
        return 0;
    }
    room = h->c.a.room;
    p = gProgress;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return 0;
    }
    if (h->c.unkE0 || h->c.moveMode != 0 || h->c.moveSub == 2 || HEWIE_MODE(h) == 3) {
        return 0;
    }
    if (h->c.a.unkC4 != 2 && (HW(h, 0xF356C, s32) == 0 || (HW(h, 0xF356C, s32) & 0x80000000))) {
        return 0;
    }
    if ((Progress_IsLinked(p, 1) & 0xFF) || h->c.state[0] != 0) {
        return 0;
    }
    return 1;
}

/* 0x00157360 */
void Hewie_State18B0(Hewie *h) {
    if (ANIM_DONE(h)) {
        HW(h, 0xF3688, s16) = 300;
        if (HW(h, 0xF36B4, s32) == 0) {
            Hewie_ToDefault(h);
        } else if (MOTION_ANIM(h->c.motion) == 0x2213) {
            if (h->c.a.unkC4 != 2) {
                Motion_PlayTable(h->c.motion, 0x1003, -1);
            } else {
                Hewie_AfterYelp(h);
            }
        } else if (MOTION_ANIM(h->c.motion) == 0x1003) {
            Hewie_AfterYelp(h);
        }
    }
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* A point 24 units out from door `door` on his side of it (if the door is usable). */
/* 0x00144940 */
void Hewie_DoorPoint(Hewie *h, s32 door, f32 *out) {
    VObject *doors;
    sceVu0FMATRIX m;
    sceVu0FVECTOR at;
    f32 off[4] __attribute__((aligned(16)));
    f32 yaw;
    s32 side;

    if (!(VCALL(gDoors, 0x40, u32 (*)(VObject *, s32))(gDoors, door) & 0xFF)) {
        return;
    }
    doors = gDoors;
    yaw = VCALL(doors, 0x3C, f32 (*)(VObject *, s32))(doors, door);
    side = VCALL(doors, 0x18, s32 (*)(VObject *, s32, f32 *))(doors, door, h->c.a.pos);
    VCALL(doors, 0x44, void (*)(VObject *, s32))(doors, door);
    VCALL(doors, 0x34, void (*)(VObject *, s32, f32 *))(doors, door, at);
    if (side == 0) {
        yaw = Angle_Wrap(F_PI + yaw);
    }
    *(s32 *)&off[0] = 0;
    *(s32 *)&off[1] = 0;
    off[2] = 24.0f;
    Mtx_AtHeading(m, at, yaw);
    Mtx_ApplyPoint(out, m, off);
}

/* 1 unless he is with Fiona's party in the room being played and cannot reach her. */
static inline s32 fiona_reachable(Hewie *h) {
    Progress *p;
    s32 room;

    if (!h->c.a.active || !h->c.unkE0) {
        return 1;
    }
    room = h->c.a.room;
    p = gProgress;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return 1;
    }
    if (h->c.a.unk2D || h->c.a.unk2B || (Character_Held(&h->c) & 0xFF)) {
        return 0;
    }
    if (gCharPlayer == NULL || !gCharPlayer->a.active) {
        return 1;
    }
    room = gCharPlayer->a.room;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return 1;
    }
    return ((Hewie_PlanTo(h, gCharPlayer->a.navTri, gCharPlayer->a.pos) & 0xFF) == 1) ? 1 : 0;
}

/* 0x001364F0 */
s32 Hewie_FionaReachable(Hewie *h) {
    return fiona_reachable(h);
}

/* A random idle action: 1 (50%), 4 (15%), 5 (35%). */
/* 0x00140050 */
void Hewie_RandomIdle(Hewie *h) {
    s32 r = (s32)(100.0f * RNG01());

    if (r < 50) {
        Hewie_Start(h, 1);
    } else if (r < 65) {
        Hewie_Start(h, 4);
    } else {
        Hewie_Start(h, 5);
    }
}

/* 0x0014DE70 */
void Hewie_State2388(Hewie *h) {
    if (HW(h, 0xF3604, s32) != 4) {
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 10;
    }
    if (MOTION_ANIM(h->c.motion) == 9) {
        if (*((u8 *)gProgress + 0x1FBEC1) == 1 && (Hewie_PlayerSteering(h) & 0xFF) == 1) {
            Hewie_SetAction(h, 0, 0);
            return;
        }
        if (HW(h, 0xF36B4, s32) == 0) {
            HW(h, 0xF3585, u8) = 1;
        } else {
            HW(h, 0xF36B4, s32) -= 1;
        }
    } else if (Hewie_StepToPose(h, 0) == 0) {
        Motion_PlayTable(h->c.motion, 9, -1);
    }
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* Helper `kind` done: action `act` -> 2, otherwise 1 (or 5 with his pool in use). */
static inline void Hewie_AfterHelper(Hewie *h, s32 kind, s32 act) {
    if (Hewie_StepToPose(h, kind) == 0) {
        Hewie_KeepPose(h, kind);
        if (HEWIE_ACTION(h) == act) {
            Hewie_Start(h, 2);
        } else if (HW(h, 0xF3598, s32) == 0) {
            Hewie_Start(h, 1);
        } else {
            Hewie_Start(h, 5);
        }
    }
}

/* 0x00155670 */
void Hewie_StateAfter2B(Hewie *h) {
    Hewie_AfterHelper(h, 2, 0x2B);
}

/* 0x001557B0 */
void Hewie_StateAfter29(Hewie *h) {
    Hewie_AfterHelper(h, 1, 0x29);
}

/* 0x001558F0 */
void Hewie_StateAfter27(Hewie *h) {
    Hewie_AfterHelper(h, 0, 0x27);
}

/* 0x0015F760 */
void Hewie_State1DC8(Hewie *h) {
    if (!(Hewie_WithChar(h, gCharPlayer) & 0xFF)) {
        Hewie_ToDefault(h);
    } else {
        Hewie_KeepPose(h, 3);
    }
}

extern const PTMF Hewie_State1D38_ptmf;
extern u32 gPadPressed;     /* pad buttons */
extern u32 gPadHeld;     /* pad buttons (d-pad bits 4..7) */
extern f32 gLeftStick[4];  /* left stick */

/* Is the player steering (buttons, stick / d-pad beyond 0.8) or has a pad command (+0xF3798)? */
/* 0x001367B0 */
s32 Hewie_PlayerSteering(Hewie *h) {
    u32 b = gPadPressed;
    sceVu0FVECTOR v;
    u32 d;
    f32 x, z;

    if ((b >> 14 & 1) || (b >> 13 & 1) || (b >> 12 & 1) || (b >> 15 & 1) || (b >> 10 & 1) || (b >> 11 & 1)) {
        return 1;
    }
    sceVu0CopyVector(v, gLeftStick);
    d = gPadHeld;
    x = v[0] + (f32)(s32)((d >> 5 & 1) - (d >> 7 & 1));
    v[0] = x;
    z = v[2] + (f32)(s32)((d >> 6 & 1) - (d >> 4 & 1));
    v[2] = z;
    if (!(__builtin_sqrtf(z * z + x * x) <= 0x1.99999ap-1f /* 0.8 */)) {
        return 1;
    }
    return HW(h, 0xF3798, s32) != -1;
}

/* 0x00147580 */
void Hewie_State2418(Hewie *h) {
    if (Hewie_StepToPose(h, 10) == 0) {
        u32 r;

        h->c.a.unkC4 = 2;
        Hewie_KeepPose(h, 10);
        r = Hewie_PlayerSteering(h) & 0xFF;
        if (r == 1) {
            h->c.a.unk2D = 0;
            Hewie_SetBehaviour(h, &Hewie_State1D38_ptmf);
        }
    }
    Hewie_ForcedMove(h);
}

/* 0x00148880 */
void Hewie_State1D18(Hewie *h) {
    if (h->c.a.disabled) {
        h->c.a.unk2D = 0;
        Hewie_WhenIdle(h);
        return;
    }
    if (Hewie_StepToPose(h, 0) == 0) {
        h->c.a.unk2D = 0;
        Hewie_KeepPose(h, 0);
        Hewie_ToDefault(h);
    }
    Hewie_ForcedMove(h);
}

/* Take him through exit `exit` into the next room (off screen): room, side, door; disabled. With
 * progress flag 0x8000 he gets up again (1 health) if he was down there. -1: no such exit. */
/* 0x0013AAE0 */
s32 Hewie_ThroughExit(Hewie *h, s32 exit) {
    VObject *rooms;
    u32 d;
    s32 room;

    HW(h, 0xF368A, u8) = 0;
    d = VCALL(gRooms, 0x14, u32 (*)(VObject *, s32, s32))(gRooms, h->c.a.room, exit) & 0xFF;
    if (d == 0xFF) {
        return -1;
    }
    rooms = gRooms;
    h->c.a.room = VCALL(rooms, 0x18, s32 (*)(VObject *, s32, s32))(rooms, h->c.a.room, exit);
    HEWIE_SIDE(h) = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, h->c.a.room, d, 0);
    h->c.door = d;
    h->c.a.navTri = NAV_NONE;
    HW(h, 0xF3590, u8) = 0;
    h->c.moveSub = 0x17;
    h->c.a.disabled = 1;
    if ((((u32 *)gProgress)[0x30 / 4] & 0x8000) != 0) {
        room = h->c.a.room;
        if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && h->c.hp == 0) {
            h->c.hp = 1;
            h->c.a.unkC4 = 1;
        }
    }
    return 0;
}

static inline void Hewie_PlayIfNot(Hewie *h, s32 cur, s32 anim) {
    if (cur != anim) {
        Motion_PlayTable(h->c.motion, anim, -1);
    }
}

/* His walking animation: 0x200; limping (condition 1) 0x206; 0x205 by progress state. */
/* 0x00143400 */
void Hewie_WalkAnim(Hewie *h) {
    s32 cur = MOTION_ANIM(h->c.motion);

    if (HEWIE_MODE(h) == 3) {
        Hewie_PlayIfNot(h, cur, 0x200);
    } else if (h->c.a.unkC4 == 1) {
        Hewie_PlayIfNot(h, cur, 0x206);
    } else if (HEWIE_MODE(h) == 2) {
        Hewie_PlayIfNot(h, cur, 0x200);
    } else if ((Progress_GameMode(gProgress) & 0xFF) == 1) {
        Hewie_PlayIfNot(h, cur, 0x205);
    } else {
        Hewie_PlayIfNot(h, cur, 0x200);
    }
}

/* 0x001515E0 */
void Hewie_State19B0(Hewie *h) {
    if (!ANIM_DONE(h)) {
        return;
    }
    if (HW(h, 0xF3686, s16) <= 0) {
        HW(h, 0xF3684, s16) = 1;
        HW(h, 0xF3686, s16) = 600;
    } else {
        HW(h, 0xF3684, s16) += 1;
        if (HW(h, 0xF3684, s16) < 3 || HEWIE_MODE(h) != 0) {
            HW(h, 0xF3686, s16) = 600;
        } else {
            Progress_AddCounter(gProgress, 0xFB6, 20);
            Hewie_SetMode(h, 2, -1);
            HW(h, 0xF3684, s16) = 0;
        }
    }
    Hewie_ToDefault(h);
}

#define HEWIE_SPOT(h) ((f32 *)((u8 *)(h) + 0xF36E0))   /* a point he moves to / looks at */

/* Off the walkable part of the mesh: push back towards his spot (0.4 a frame) and turn to it;
 * back on it: default action. */
/* 0x00146AE0 */
void Hewie_StateBackOnMesh(Hewie *h) {
    NavTri *t = NavMesh_Tri(gNavMesh, h->c.a.navTri);

    if (!(t->flags & HEWIE_NAV_MASK)) {
        h->c.a.unk2D = 0;
        h->c.a.unk2B = 0;
        Hewie_ToDefault(h);
    } else {
        sceVu0FVECTOR d;

        HW(h, 0xF3558, u8) = 1;
        sceVu0SubVector(d, HEWIE_SPOT(h), h->c.a.pos);
        sceVu0Normalize(d, d);
        sceVu0ScaleVector(d, d, 0x1.99999ap-2f /* 0.4 */);
        Actor_Move(&h->c.a, d);
        {
            f32 yaw = Actor_HeadingTo(&h->c.a, HEWIE_SPOT(h));

            HW(h, 0xF3614, s32) = 0;
            HW(h, 0xF3618, f32) = Angle_Wrap(yaw - h->c.a.angle[1]);
        }
        Hewie_KeepPose(h, 7);
    }
}

/* Random level 0..2 (out of 16), lower thresholds with difficulty 1 / progress state. */
/* 0x001382F0 */
s32 Hewie_RandomLevel(Hewie *h) {
    s32 r = (s32)(16.0f * RNG01());
    Progress *p = gProgress;

    if ((Progress_GetVar(p, 0x27) & 0xFF) == 1) {
        if (!(Progress_GameMode(p) & 0xFF)) {
            return (r < 8) ? 0 : (r < 13) ? 1 : 2;
        }
        return (r < 5) ? 0 : (r < 13) ? 1 : 2;
    }
    if (!(Progress_GameMode(p) & 0xFF)) {
        return (r < 10) ? 0 : (r < 15) ? 1 : 2;
    }
    return (r < 5) ? 0 : (r < 15) ? 1 : 2;
}

/* His secondary (overlay) animation by +0xF3640: 0x1F00 / 0x1F01 / 0x1F02; 3: change at random
 * every 30..450 frames. */
/* 0x0013FC70 */
void Hewie_OverlayAnim(Hewie *h) {
    switch (HW(h, 0xF3640, s32)) {
    case 0:
        Motion_PlayTableNoCheck(h->c.motion, 0x1F00);
        break;
    case 1:
        Motion_PlayTableNoCheck(h->c.motion, 0x1F01);
        break;
    case 2:
        Motion_PlayTableNoCheck(h->c.motion, 0x1F02);
        break;
    case 3:
        if (--HW(h, 0xF3644, s32) < 0) {
            HW(h, 0xF3644, s32) = (s32)(15.0f * RNG01()) * 30 + 30;
            if (RNG01() < 0.5f) {
                Motion_PlayTableNoCheck(h->c.motion, 0x1F00);
            } else {
                Motion_PlayTableNoCheck(h->c.motion, 0x1F01);
            }
        }
        break;
    }
}

static inline void Hewie_Mark4(Hewie *h) {
    if (HW(h, 0xF3604, s32) != 4) {
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 10;
    }
}

/* 0x0015BB20 */
void Hewie_State1E48(Hewie *h) {
    u8 stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;

    if ((u8)(stopped ^ 1) == 1) {
        return;
    }
    if (ANIM_DONE(h) && MOTION_ANIM(h->c.motion) == 0x1C02) {
        Hewie_Start(h, HW(h, 0xF366D, u8) == 0 ? 6 : 8);
    } else if (Hewie_StepToPose(h, 0) == 0) {
        Hewie_Mark4(h);
        Motion_PlayTable(h->c.motion, 0x1C02, -1);
    }
}

/* 0x0015D320 */
void Hewie_State1740(Hewie *h) {
    if (--HW(h, 0xF36B8, s32) == 0) {
        Hewie_ToDefault(h);
        return;
    }
    if (!(Actor_Distance(&h->c.a, gCharPlayer->a.pos) <= 20.0f)) {
        Hewie_Start(h, HEWIE_ACTION(h));
    }
    HEWIE_TARGET(h) = gCharPlayer;
    if (HW(h, 0xF3604, s32) != 0) {
        HW(h, 0xF3604, s32) = 0;
        HW(h, 0xF3608, s32) = 10;
    }
    Hewie_KeepPose(h, 3);
}

/* Fiona is with him and he can take a command (idle, not angry, one queued, nothing pending). */
/* 0x00139060 */
s32 Hewie_FionaCanCommand(Hewie *h) {
    if ((Hewie_WithChar(h, gCharPlayer) & 0xFF) != 1 || h->c.unkE0 || h->c.moveMode != 0 || HEWIE_MODE(h) == 3
        || HW(h, 0xF356C, s32) == 0 || (HW(h, 0xF356C, s32) & 0x80000000)
        || (Progress_IsLinked(gProgress, 1) & 0xFF) || h->c.state[0] != 0) {
        return 0;
    }
    return 1;
}

/* 0x0015F8A0 */
void Hewie_State1D98(Hewie *h) {
    s32 room = h->c.a.room;
    s32 g;

    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        HW(h, 0xF3559, u8) = 1;
        return;
    }
    g = Hewie_AnimGroup(h);
    if (g == 0xA || g == 9 || g == 8) {
        sceVu0FVECTOR root, to;
        f32 k;

        Motion_RootMovement(h->c.motion, root, 0.0f);
        k = VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
        root[2] = root[2] * k;
        sceVu0ApplyMatrix(root, h->c.a.rot, root);
        sceVu0AddVector(to, h->c.a.pos, root);
        if (Actor_TriTo(&h->c.a, to, NAV_NONE) == NAV_NONE) {
            u8 stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;

            if ((u8)(stopped ^ 1) == 0) {
                Hewie_StandAnim(h, -1);
            }
        }
    }
    if (Hewie_StepToPose(h, 4) == 0) {
        Hewie_KeepPose(h, 4);
        HW(h, 0xF3559, u8) = 1;
    }
}

/* One roll (of 32) against threshold `t`, logged as `kind` in his roll history (+0xF3696..). */
static inline s32 Hewie_Roll(Hewie *h, VObject *rng, s32 kind, s8 t, u32 resultOff) {
    u32 r = (u8)(u32)(32.0f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng));
    s32 hit = (s32)r < t;
    u8 n = HW(h, 0xF36A2, u8);

    HW(h, 0xF3696 + n, u8) = kind;
    HW(h, resultOff, u8) = hit;
    HW(h, 0xF36A2, u8) = n + 1;
    return hit;
}

/* 0x001396B0 */
void Hewie_RollReactions(Hewie *h) {
    VObject *rng = gRandom;

    if (!Hewie_Roll(h, rng, 3, HW(h, 0xF3693, s8), 0xF369F)) {
        Hewie_Roll(h, rng, 4, HW(h, 0xF3694, s8), 0xF36A0);
    }
}

extern const PTMF Hewie_StateWalkPath_ptmf;

/* 0x0014A790 */
void Hewie_State2138(Hewie *h) {
    sceVu0FVECTOR root;
    f32 k;
    s32 r;

    Motion_RootMovement(h->c.motion, root, 0.0f);
    k = VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    *(s32 *)&root[1] = 0;
    root[2] = root[2] * k;
    sceVu0ApplyMatrix(root, h->c.a.rot, root);
    Actor_Move(&h->c.a, root);
    HW(h, 0xF3558, u8) = 1;
    VCALL(gNavMesh, 0xC, void (*)(NavMesh *, s32, f32 *))(gNavMesh, h->c.unk104[0], (f32 *)h->c.unk110);
    r = Character_PlanPathKind(&h->c, 0, h->c.unk104[0], (f32 *)h->c.unk110);
    if (r > 0) {
        r = Character_WaypointsCurve(&h->c);
    }
    if (r > 0) {
        Hewie_Mark4(h);
        Hewie_SetBehaviour(h, &Hewie_StateWalkPath_ptmf);
    } else {
        Hewie_ToDefault(h);
    }
}

/* 0x00154E70 */
void Hewie_State1920(Hewie *h) {
    s32 act;

    if (h->c.a.unkC4 == 1) {
        *(f32 *)&h->c.unk14C4 = *(f32 *)&h->c.unk14C4 - 0x1.851eb8p-2f;   /* 0.38 */
    } else {
        *(f32 *)&h->c.unk14C4 = *(f32 *)&h->c.unk14C4 - 0x1.99999ap+0f;   /* 1.6 */
    }
    if (!(*(f32 *)&h->c.unk14C4 < 0.0f)) {
        return;
    }
    act = HEWIE_ACTION(h);
    switch (act) {
    case 0x33:
        if (h->c.a.room == HW(h, 0xF3594, s32)) {
            Hewie_WhenIdle(h);
        } else {
            Hewie_Start(h, 0x33);
        }
        break;
    case 0x2C: case 0x2D: case 0x32: case 0x35: case 0x39:
        Hewie_Start(h, act);
        break;
    }
}

/* vtable-ish setter: his health state (+0xC4: 1 hurt, 2 down) and matching animation set;
 * then restart his action. */
/* 0x00165510 */
void Hewie_SetHealthState(Hewie *h, s32 st) {
    if (!h->c.a.active) {
        return;
    }
    h->c.a.unkC4 = st;
    switch (h->c.a.unkC4) {
    case 1:
        HW(h, 0xF36F0, s32) = 6;
        break;
    case 2:
        HW(h, 0xF36F0, s32) = 0x1002;
        break;
    default:
        HW(h, 0xF36F0, s32) = 0;
        break;
    }
    Motion_Play(h->c.motion, HW(h, 0xF36F0, s32), -1);
    Hewie_AddTrust(h, 0);
    if (h->c.a.unkC4 == 2) {
        Hewie_Start(h, 0x52);
    } else if (HW(h, 0xF3583, u8) == 1) {
        Hewie_Start(h, 0x36);
    } else {
        Hewie_ToDefault(h);
    }
}

/* Character `c` is active, not down, in his room and (in the current room) on the nav mesh. */
static inline u8 Hewie_CharHere(Hewie *h, Character *c) {
    u8 ok = (c != NULL && c->a.active == 1) ? 1 : 0;

    if (ok == 1 && c->a.unkC4 != 2 && h->c.a.room == c->a.room) {
        s32 room = h->c.a.room;

        ok = (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) || c->a.navTri != NAV_NONE) ? 1 : 0;
    } else {
        ok = 0;
    }
    return ok;
}

extern const s8 D_003B12C0[][6];   /* chance (of 100) by kind and pursuer health band */

/* Random roll for reaction `kind`, more likely the more hurt the pursuer is. */
/* 0x001386D0 */
s32 Hewie_ReactionRoll(Hewie *h, s32 kind) {
    u8 ok = (gCharPursuer != NULL && gCharPursuer->a.active == 1) ? 1 : 0;
    s32 band;
    f32 r;

    if (ok != 1) {
        return 0;
    }
    band = 0;
    r = (f32)gCharPursuer->hp / (f32)gCharPursuer->hpMax;
    if (r < 0x1.99999ap-4f) {
        band = 5;
    } else if (r < 0x1.99999ap-3f) {
        band = 4;
    } else if (r < 0x1.99999ap-2f) {
        band = 3;
    } else if (r < 0x1.333334p-1f) {
        band = 2;
    } else if (r < 0x1.99999ap-1f) {
        band = 1;
    }
    if ((s8)(s32)(100.0f * RNG01()) < D_003B12C0[kind][band]) {
        return 1;
    }
    return 0;
}

extern const PTMF Hewie_StateLeapLaunch_ptmf, Hewie_StateGoForPursuer_ptmf;

/* Behaviour: (state 7 ends it) go for the pursuer if he's here. */
/* 0x00150290 */
void Hewie_StateGoForPursuer(Hewie *h) {
    if (h->c.state[0] == 7) {
        h->c.state[0] = 0;
        Hewie_ToDefault(h);
        return;
    }
    if (Hewie_CharHere(h, gCharPursuer)) {
        Hewie_SetBehaviour(h, &Hewie_StateLeapLaunch_ptmf);
    } else {
        Hewie_ToDefault(h);
    }
}

/* Behaviour: target the pursuer if he's here and reachable. */
/* 0x00150450 */
void Hewie_StateTargetPursuer(Hewie *h) {
    if (Hewie_CharHere(h, gCharPursuer) == 1) {
        HEWIE_TARGET(h) = gCharPursuer;
        if ((SlotCmd_Give(gProgress, 2, 7, h->c.a.slot, gCharPursuer->a.slot, 0, 0.0f) & 0xFF) == 1) {
            Hewie_SetBehaviour(h, &Hewie_StateGoForPursuer_ptmf);
            return;
        }
    }
    Hewie_ToDefault(h);
}

extern const PTMF Hewie_State1C98_ptmf, Hewie_StateTurning_ptmf, Hewie_State1C98_ptmf2, Hewie_StateBehindFiona_ptmf;

/* Behaviour: once stopped, turn to heading unk10C: snap if within 1 degree, else start the
 * turning animation (eased by the angle, Hewie_StateTurning). */
/* 0x0014A180 */
void Hewie_StateTurnStart(Hewie *h) {
    u8 stopped;
    f32 target, d, ad;

    HW(h, 0xF3558, u8) = 1;
    stopped = 1;
    if (!(MOTION_SPEED(h->c.motion) <= 0.0f)) {
        stopped = 0;
    }
    if (((stopped ^ 1) & 0xFF) == 1) {
        return;
    }
    d = Angle_Wrap(*(f32 *)&h->c.unk104[2] - h->c.a.angle[1]);
    ad = d;
    if (ad <= 0.0f) {
        ad = -ad;
    }
    if (ad < 0x1.1df46ap-6f /* 1 deg */) {
        target = *(f32 *)&h->c.unk104[2];
        h->c.a.angle[1] = target;
        sceVu0UnitMatrix(h->c.a.rot);
        sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, target);
        Hewie_SetBehaviour(h, &Hewie_State1C98_ptmf);
        return;
    }
    if (!(Progress_GameMode(gProgress) & 0xFF)) {
        Motion_PlayTable(h->c.motion, 0x1300, 0);
    } else {
        Motion_PlayTable(h->c.motion, 0x1300, 3);
    }
    *(s32 *)((u8 *)MOTION_PTR(h->c.motion, 0x6A4) + 0x1C) = 0;
    if (d <= 0.0f) {
        d = -d;
    }
    HW(h, 0xF36C4, f32) = 0x1.028f5cp+0f /* 1.01 */ * d;
    Hewie_SetBehaviour(h, &Hewie_StateTurning_ptmf);
}

/* Behaviour: turning to heading unk10C, playback rate and step eased by a sine of the
 * remaining angle (of the total +0xF36C4). */
/* 0x00149FB0 */
void Hewie_StateTurning(Hewie *h) {
    f32 d, s, rate, step;

    HW(h, 0xF3558, u8) = 1;
    HW(h, 0xF3604, s32) = 8;
    HW(h, 0xF3608, s32) = 0;
    HW(h, 0xF3614, s32) = 0;
    HW(h, 0xF3618, f32) = Angle_Wrap(*(f32 *)&h->c.unk104[2] - h->c.a.angle[1]);
    d = Angle_Wrap(*(f32 *)&h->c.unk104[2] - h->c.a.angle[1]);
    if (d <= 0.0f) {
        d = -d;
    }
    s = msl_sinf(d * F_PI / HW(h, 0xF36C4, f32));
    rate = 2.0f * (s * HW(h, 0xF36C4, f32)) / F_PI;
    if (!(rate <= 0x1.333334p-1f /* 0.6 */)) {
        rate = 0x1.333334p-1f;
    }
    *(f32 *)((u8 *)MOTION_PTR(h->c.motion, 0x6A4) + 0x1C) = rate;
    step = 0x1.99999ap-5f /* 0.05 */ * HW(h, 0xF36C4, f32) * s;
    if (step < 0x1.1df46ap-7f /* 0.5 deg */) {
        step = 0x1.1df46ap-7f;
    }
    if (Actor_TurnToward(&h->c.a, *(f32 *)&h->c.unk104[2], step) == 0.0f) {
        Hewie_SetBehaviour(h, &Hewie_State1C98_ptmf2);
    }
}

/* Behaviour: back to normal unless Fiona is here and not busy (then Hewie_StateBehindFiona_ptmf). */
/* 0x00150FD0 */
void Hewie_StateBackToNormal(Hewie *h) {
    if (Hewie_CharHere(h, gCharPlayer) && gCharPlayer->unkE0 != 1) {
        Hewie_SetBehaviour(h, &Hewie_StateBehindFiona_ptmf);
    } else {
        Hewie_ToDefault(h);
    }
}

extern const PTMF Hewie_StateLeapGrip_ptmf;

/* Behaviour: (state 7: action 0x5A) go for the pursuer if he's here. */
/* 0x00159DC0 */
void Hewie_StateGoForPursuer2(Hewie *h) {
    if (h->c.state[0] == 7) {
        h->c.state[0] = 0;
        Hewie_Start(h, 0x5A);
        return;
    }
    if (Hewie_CharHere(h, gCharPursuer)) {
        HW(h, 0xF3558, u8) = 1;
        Hewie_SetBehaviour(h, &Hewie_StateLeapGrip_ptmf);
    } else {
        Hewie_ToDefault(h);
    }
}

/* Idle: on a plain triangle pick a random idle action 0x18..0x1C, not the last one (+0xF357C). */
/* 0x00140B00 */
void Hewie_IdleAction(Hewie *h) {
    static const s32 idles[5] = { 0x18, 0x19, 0x1C, 0x1A, 0x1B };
    NavTri *t = NavMesh_Tri(gNavMesh, h->c.a.navTri);
    s32 act = 0x18;
    s32 r;

    if (t->flags & 3) {
        Hewie_ToDefault(h);
        return;
    }
    do {
        r = (s32)(5.0f * RNG01());
        if (r >= 0 && r <= 4) {
            act = idles[r];
        }
    } while (act == HW(h, 0xF357C, s32));
    HW(h, 0xF357C, s32) = act;
    Hewie_Start(h, act);
}

/* Count an encounter with the kind of character `other` is (+0xF367C.., next to the feelings;
 * up to 100). */
/* 0x0013B860 */
void Hewie_CountEncounter(Hewie *h, Character *other) {
    s16 *v = Hewie_Feeling(h, other->unk153C);

    if (v != NULL) {
        v[4] += 1;
        if (v[4] > 100) {
            v[4] = 100;
        }
    }
}

extern f32 Hewie_WalkDistance(Hewie *h, Character *c);   /* path distance to c (< 0: none) */

/* Fiona is here, he is free to take a command (not in mode 3) and she is within 150 by path. */
/* 0x001667C0 */
s32 Hewie_FionaNearCommand(Hewie *h) {
    f32 d;

    if (!(Hewie_CharHere(h, gCharPlayer) == 1 && HEWIE_MODE(h) != 3 && Hewie_FreeForCommand(h))) {
        return 0;
    }
    d = Hewie_WalkDistance(h, gCharPlayer);
    if (d < 0.0f) {
        return 0;
    }
    return d <= 150.0f;
}

/* Behaviour: run (move kind 8) until the timer +0xF36B4 runs out or something is within 20
 * ahead; then slow down (kind 7) and once stopped, the default or a stop action (0xC / 0xE). */
/* 0x001517C0 */
void Hewie_StateRun(Hewie *h) {
    u8 stopped;

    if (HW(h, 0xF36B8, s32) == 0) {
        HW(h, 0xF36B4, s32) -= 1;
        if (HW(h, 0xF36B4, s32) == 0
            || Actor_FreeDistance(&h->c.a, h->c.a.navTri, h->c.a.pos, -1, h->c.a.angle[1], 20.0f) < 20.0f) {
            HW(h, 0xF36B8, s32) = 1;
        }
        Hewie_KeepPose(h, 8);
        return;
    }
    Hewie_KeepPose(h, 7);
    stopped = 1;
    if (!(MOTION_SPEED(h->c.motion) <= 0.0f)) {
        stopped = 0;
    }
    if ((stopped ^ 1) & 0xFF) {
        return;
    }
    if (HW(h, 0xF3598, s32) != 0) {
        Hewie_ToDefault(h);
    } else if (!(Progress_GameMode(gProgress) & 0xFF)) {
        Hewie_Start(h, 0xC);
    } else {
        Hewie_Start(h, 0xE);
    }
}

extern const PTMF Hewie_State1990_ptmf;

/* Behaviour: run to Fiona if she's here (bark 0x60, then Hewie_StateRun's run). */
/* 0x00151B10 */
void Hewie_StateRunToFiona(Hewie *h) {
    if (!Hewie_CharHere(h, gCharPlayer)) {
        Hewie_ToDefault(h);
        return;
    }
    if (Hewie_StepToPose(h, 3)) {
        return;
    }
    Actor_PlaySound(&h->c.a, 0x60, 5, 0, 0, 0);
    if (HW(h, 0xF3604, s32) != 0) {
        HW(h, 0xF3604, s32) = 0;
        HW(h, 0xF3608, s32) = 10;
    }
    HEWIE_TARGET(h) = gCharPlayer;
    Hewie_KeepPose(h, 3);
    HW(h, 0xF36B4, s32) = 30;
    Hewie_SetBehaviour(h, &Hewie_State1990_ptmf);
}

extern const PTMF Hewie_State1A60_ptmf4, Hewie_State1A60_ptmf5, Hewie_State1A60_ptmf6;

/* Behaviour: follow the moving character +0xF368C (gone: Hewie_State1A60_ptmf4; elsewhere on the mesh:
 * Hewie_State1A60_ptmf6; slower than 0.5: Hewie_State1A60_ptmf5); else steer towards it (+0xF3614 / +0xF3618). */
/* 0x0014EC10 */
void Hewie_StateFollowMover(Hewie *h) {
    Character *c = HW(h, 0xF368C, Character *);
    f32 v[4] __attribute__((aligned(16)));
    f32 a, turn, yaw;
    u32 tri;

    if (c == NULL || !c->a.active) {
        HW(h, 0xF36BC, s32) = 90;
        Hewie_SetBehaviour(h, &Hewie_State1A60_ptmf4);
        return;
    }
    tri = c->a.navTri;
    if (Actor_TriTo(&h->c.a, c->a.pos, 0) != tri) {
        HW(h, 0xF36BC, s32) = 90;
        Hewie_SetBehaviour(h, &Hewie_State1A60_ptmf6);
        return;
    }
    sceVu0CopyVector(v, HW(h, 0xF368C, Character *)->a.unkB0);
    if (__builtin_sqrtf(sceVu0InnerProduct(v, v)) < 0.5f) {
        HW(h, 0xF36BC, s32) = 90;
        Hewie_SetBehaviour(h, &Hewie_State1A60_ptmf5);
        return;
    }
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    Motion_LookAt(h->c.motion, HW(h, 0xF368C, Character *)->a.pos, &a, &turn);
    yaw = Angle_Wrap(h->c.a.angle[1] + turn);
    HW(h, 0xF3614, f32) = a;
    HW(h, 0xF3618, f32) = Angle_Wrap(yaw - h->c.a.angle[1]);
}

/* Behaviour: (Fiona here and not busy) play the animation to its end, then default. */
/* 0x00150720 */
void Hewie_StatePlayToEnd(Hewie *h) {
    if (!Hewie_CharHere(h, gCharPlayer) || gCharPlayer->unkE0 == 1) {
        Hewie_ToDefault(h);
        return;
    }
    if (ANIM_DONE(h)) {
        Hewie_ToDefault(h);
    }
    HW(h, 0xF3558, u8) = 1;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

extern const PTMF Hewie_State1C48_ptmf;

/* Behaviour: turn to heading unk10C (10 degrees a frame; against the animation's turn
 * direction +0x858: rotate directly) while moving along +0xF36E0 by the root motion; at the
 * animation's end stand (Hewie_State1C48_ptmf). */
/* 0x0014B8F0 */
void Hewie_StateTurnTo(Hewie *h) {
    f32 v[4] __attribute__((aligned(16)));
    f32 root[4] __attribute__((aligned(16)));
    f32 target = *(f32 *)&h->c.unk104[2];
    f32 d = Angle_Wrap(target - h->c.a.angle[1]);
    f32 ad, yaw;

    ad = d;
    if (d <= 0.0f) {
        ad = -d;
    }
    if (ad < 0x1.0c1524p-1f /* 30 deg */) {
        Actor_TurnToward(&h->c.a, target, 0x1.657186p-3f /* 10 deg */);
    } else if (!(*(f32 *)((u8 *)h->c.motion + 0x858) * d < 0.0f)) {
        Actor_TurnToward(&h->c.a, target, 0x1.657186p-3f);
    } else {
        if (d < 0.0f) {
            yaw = Angle_Wrap(h->c.a.angle[1] + 0x1.657186p-3f);
        } else {
            yaw = Angle_Wrap(h->c.a.angle[1] - 0x1.657186p-3f);
        }
        h->c.a.angle[1] = yaw;
        sceVu0UnitMatrix(h->c.a.rot);
        sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, yaw);
        Angle_Wrap(target - h->c.a.angle[1]);
    }
    if (ANIM_DONE(h)) {
        if (HW(h, 0xF36B8, s32) == 0) {
            h->c.a.unk2D = 0;
        }
        Hewie_StandAnim(h, -1);
        Hewie_SetBehaviour(h, &Hewie_State1C48_ptmf);
        VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
    }
    Motion_RootTranslation(h->c.motion, root, 0.0f);
    vu0_ScaleXYZ(v, HEWIE_SPOT(h), root[2]);
    Actor_Move(&h->c.a, v);
    HW(h, 0xF3558, u8) = 1;
}

extern const PTMF Hewie_StateOnEvent20_ptmf;

/* Behaviour: move +0xF36C8 along heading +0xF36C4 (+0xF36CC a frame). Done: unless in action
 * 0x6C, Hewie_StateOnEvent20_ptmf; in 0x6C he leaves through exit +0xF36B4 (clearing unk148C), gets up if
 * down, default action. */
/* 0x00153B00 */
void Hewie_StateMoveAlong(Hewie *h) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    VObject *rooms;

    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = HW(h, 0xF36C8, f32);
    Mtx_TurnY(m, HW(h, 0xF36C4, f32));
    Mtx_ApplyVector(v, m, v);
    Actor_Move(&h->c.a, v);
    HW(h, 0xF3558, u8) = 1;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
    HW(h, 0xF36C8, f32) = HW(h, 0xF36C8, f32) - HW(h, 0xF36CC, f32);
    if (!(HW(h, 0xF36C8, f32) <= 0.0f)) {
        return;
    }
    if (HEWIE_ACTION(h) != 0x6C) {
        Hewie_SetBehaviour(h, &Hewie_StateOnEvent20_ptmf);
        return;
    }
    h->c.a.unk2B = 0;
    h->c.a.unk2D = 0;
    rooms = gRooms;
    if (!(VCALL(rooms, 0x70, u32 (*)(VObject *, s32, s32))(rooms, h->c.a.room, HW(h, 0xF36B4, u8)) & 0xFF)
        && VCALL(rooms, 0x18, s32 (*)(VObject *, s32, s32))(rooms, h->c.a.room, HW(h, 0xF36B4, u8)) != -1
        && Hewie_ThroughExit(h, HW(h, 0xF36B4, u8)) == 0) {
        u32 i;

        for (i = 0; i < 13; i++) {
            h->c.unk148C[i] = 0;
        }
    }
    if (h->c.hp == 0) {
        h->c.hp = 1;
        h->c.a.unkC4 = 1;
    }
    Hewie_ToDefault(h);
}

/* His idle overlay (ear / tail?) animation by +0xF3648: 2 always 0x2001, 1 0x2000 (hurt 0x2002);
 * 0: (hurt 0x2002) alternate 0x2000 / 0x2001 after a random wait (+0xF364C, 300..2100 frames). */
/* 0x0013FA40 */
void Hewie_IdleOverlay(Hewie *h) {
    s32 r;

    switch (HW(h, 0xF3648, s32)) {
    case 0:
        if (h->c.a.unkC4 == 1) {
            Motion_PlayTableNoCheck(h->c.motion, 0x2002);
            break;
        }
        HW(h, 0xF364C, s32) -= 1;
        if (HW(h, 0xF364C, s32) >= 0) {
            break;
        }
        if (HW(h, 0xF3650, s32) == 0) {
            Motion_PlayTableNoCheck(h->c.motion, 0x2000);
            HW(h, 0xF3650, s32) = 1;
        } else {
            Motion_PlayTableNoCheck(h->c.motion, 0x2001);
            HW(h, 0xF3650, s32) = 0;
        }
        r = (s32)(2.0f * RNG01());
        switch (r) {
        case 0:
            HW(h, 0xF364C, s32) = 300;
            break;
        case 1:
            HW(h, 0xF364C, s32) = 900;
            break;
        case 2:
            HW(h, 0xF364C, s32) = 1800;
            break;
        }
        HW(h, 0xF364C, s32) += (s32)(10.0f * RNG01()) * 30;
        break;
    case 1:
        if (h->c.a.unkC4 == 1) {
            Motion_PlayTableNoCheck(h->c.motion, 0x2002);
        } else {
            Motion_PlayTableNoCheck(h->c.motion, 0x2000);
        }
        break;
    case 2:
        Motion_PlayTableNoCheck(h->c.motion, 0x2001);
        break;
    }
}

/* Roll his reaction: record kind 0 (+0xF3696[n]) and whether a 0..31 roll is below +0xF3690
 * (+0xF369C); then kind 2 vs +0xF3692 (+0xF369E) if it was, else kind 1 vs +0xF3691
 * (+0xF369D). +0xF36A2 counts the records. */
/* 0x00139840 */
void Hewie_RollReaction(Hewie *h) {
    u8 r;
    s32 c;

    r = (u32)(32.0f * RNG01());
    c = r < HW(h, 0xF3690, s8);
    HW(h, 0xF3696 + HW(h, 0xF36A2, u8), u8) = 0;
    HW(h, 0xF369C, u8) = c;
    HW(h, 0xF36A2, u8) += 1;
    if (c) {
        r = (u32)(32.0f * RNG01());
        c = r < HW(h, 0xF3692, s8);
        HW(h, 0xF3696 + HW(h, 0xF36A2, u8), u8) = 2;
        HW(h, 0xF369E, u8) = c;
    } else {
        r = (u32)(32.0f * RNG01());
        c = r < HW(h, 0xF3691, s8);
        HW(h, 0xF3696 + HW(h, 0xF36A2, u8), u8) = 1;
        HW(h, 0xF369D, u8) = c;
    }
    HW(h, 0xF36A2, u8) += 1;
}

/* Behaviour: while Fiona is in move mode 0xD, turn (6 degrees a frame) to heading unk10C,
 * walking (kind 1) until +0xF36B4 runs low, else the 0x1300 turning animation. */
/* 0x00149370 */
void Hewie_StateTurnWithFiona(Hewie *h) {
    if (!Hewie_CharHere(h, gCharPlayer)) {
        Hewie_ToDefault(h);
        return;
    }
    if (gCharPlayer->moveMode != 0xD) {
        Hewie_ToDefault(h);
    } else {
        Actor_TurnToward(&h->c.a, *(f32 *)&h->c.unk104[2], 0x1.aceeap-4f /* 6 deg */);
        HW(h, 0xF36B4, s32) -= 1;
        if (HW(h, 0xF36B8, s32) != 0 && HW(h, 0xF36B4, s32) < 16) {
            Hewie_KeepPose(h, 1);
        } else {
            HW(h, 0xF36B8, s32) = 1;
            if (MOTION_ANIM(h->c.motion) != 0x1300) {
                Motion_PlayTable(h->c.motion, 0x1300, -1);
            }
        }
    }
    HW(h, 0xF3558, u8) = 1;
}

extern s32 Hewie_TargetTri(Hewie *h, f32 *out);

/* Behaviour: steer towards a point (from Hewie_TargetTri when he has a target, else the saved
 * point +0xF3630 if +0xF3620), running (kind 8); without one, stand (kind 4). */
/* 0x0015F0A0 */
void Hewie_StateSteer(Hewie *h) {
    f32 v[4] __attribute__((aligned(16)));
    f32 a, turn, yaw;
    u8 have;

    if (HW(h, 0xF366C, u8) == 0 && HW(h, 0xF366D, u8) != 0) {
        Hewie_ToDefault(h);
    }
    have = 1;
    if (HEWIE_TARGET(h) == NULL && HW(h, 0xF366D, u8) == 0) {
        have = 0;
    } else if (Hewie_TargetTri(h, v) == -1) {
        have = 0;
    }
    if (!have && HW(h, 0xF3620, u8) == 1) {
        have = 1;
        sceVu0CopyVector(v, (f32 *)((u8 *)h + 0xF3630));
    }
    if (have == 1) {
        if (HW(h, 0xF3604, s32) != 8) {
            HW(h, 0xF3604, s32) = 8;
            HW(h, 0xF3608, s32) = 10;
        }
        Motion_LookAt(h->c.motion, v, &a, &turn);
        if (HW(h, 0xF3620, u8) == 0) {
            a = 0.0f;
        }
        yaw = Angle_Wrap(h->c.a.angle[1] + turn);
        HW(h, 0xF3614, f32) = a;
        HW(h, 0xF3618, f32) = Angle_Wrap(yaw - h->c.a.angle[1]);
    } else if (HW(h, 0xF3604, s32) != 4) {
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 10;
    }
    if (!Hewie_StepToPose(h, 0) && MOTION_ANIM(h->c.motion) != 3) {
        Motion_PlayTable(h->c.motion, 3, -1);
    }
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

extern const f32 kHewieMeetOffsets[][2];   /* offsets (x, z) from him, by kind */

/* Find a spot at offset `kind` around him, turning 0, +-10, ... +-180 degrees: on the walkable
 * mesh (not flags 0x80001) and reachable from Fiona. Writes the heading and point; -1: none. */
/* 0x00138890 */
s32 Hewie_FindSpot(Hewie *h, s32 kind, f32 *yawOut, f32 *posOut) {
    f32 off[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    NavMesh *nm;
    s32 deg, side;
    f32 ang, yaw;
    u32 tri;

    off[0] = kHewieMeetOffsets[kind][0];
    off[1] = 0.0f;
    off[2] = kHewieMeetOffsets[kind][1];
    off[3] = 1.0f;
    nm = gNavMesh;
    for (deg = 0; deg <= 180; deg += 10) {
        ang = F_PI * (f32)deg;
        for (side = 0;; side++) {
            if (side != 0) {
                yaw = Angle_Wrap(h->c.a.angle[1] + ang / 180.0f);
            } else {
                yaw = Angle_Wrap(h->c.a.angle[1] - ang / 180.0f);
            }
            Mtx_AtHeading(m, h->c.a.pos, yaw);
            Mtx_ApplyPoint(p, m, off);
            tri = Actor_TriOf(&h->c.a, p);
            if (tri != NAV_NONE) {
                NavTri *t = (tri < nm->numTris && nm->tris != NULL) ? &nm->tris[tri] : NULL;

                if (!(t->flags & 0x80001)
                    && Actor_TriFrom(&h->c.a, p, gCharPlayer->a.navTri, gCharPlayer->a.pos, 0x280A0019) == tri) {
                    *yawOut = yaw;
                    sceVu0CopyVector(posOut, p);
                    return tri;
                }
            }
            if (deg == 0 || deg == 180 || side + 1 >= 2) {
                break;
            }
        }
    }
    return -1;
}

extern const PTMF Hewie_State18B0_ptmf, Hewie_StateFalling_ptmf;

/* Behaviour: jump arc. Move by +0xF36E0, rise by +0xF36C8 (less 0.5 a frame); on landing the
 * landing animation (Hewie_State18B0_ptmf); at the top (actions 0x1F / 0x20) the falling animation. */
/* 0x00157770 */
void Hewie_StateJumpArc(Hewie *h) {
    f32 y;

    Actor_Move(&h->c.a, HEWIE_SPOT(h));
    y = HW(h, 0xF36C4, f32) + HW(h, 0xF36C8, f32);
    HW(h, 0xF3558, u8) = 1;
    if (y < h->c.a.pos[1]) {
        if (HW(h, 0xF36B4, s32) == 0) {
            Motion_PlayTable(h->c.motion, 0x2212, -1);
        } else {
            Motion_PlayTable(h->c.motion, 0x2213, -1);
        }
        Hewie_SetBehaviour(h, &Hewie_State18B0_ptmf);
    } else {
        h->c.a.pos[1] = y;
        HW(h, 0xF36C4, f32) = y;
        HW(h, 0xF36C8, f32) = HW(h, 0xF36C8, f32) - 0.5f;
        if (HW(h, 0xF36C8, f32) <= 0.0f) {
            s32 act = HEWIE_ACTION(h);

            if (act == 0x20 || act == 0x1F) {
                switch (h->c.unk104[0]) {
                case 1:
                    Motion_PlayTable(h->c.motion, HW(h, 0xF36B4, s32) == 0 ? 0x2205 : 0x2208, -1);
                    break;
                case 2:
                    Motion_PlayTable(h->c.motion, HW(h, 0xF36B4, s32) == 0 ? 0x220E : 0x2211, -1);
                    break;
                }
            }
            Hewie_SetBehaviour(h, &Hewie_StateFalling_ptmf);
        }
    }
    HW(h, 0xF3582, u8) = 0;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* ---- Hewie under the player's control (gProgress +0x1FBEC1): his movement input, as
 * Fiona's (fiona.c Fiona_MoveInput) ---- */

#define HMOVE_DIR    0xF3700   /* vec: where to move (world, unit or 0) */
#define HMOVE_MODE   0xF3710   /* u8: 0 free, 1 camera-locked, 2 held, 3 reset */
#define HMOVE_LOCK   0xF3712   /* s16: frames the old camera still steers */
#define HMOVE_STILL  0xF3718   /* s32: frames without input (to 6) */
#define HMOVE_STICK  0xF3720   /* vec: last frame's raw input */
#define HMOVE_LAST   0xF3730   /* vec: last frame's normalized input */
#define HMOVE_CAMYAW 0xF3740   /* f32: the camera heading the controls use */
#define HMOVE_GO     0xF3744   /* u8: the action button (0x4000) */

static f32 hwrap_abs(f32 a) {
    if (!(Angle_Wrap(a) <= 0.0f)) {
        return Angle_Wrap(a);
    }
    return -Angle_Wrap(a);
}

/* the left stick (or the d-pad), camera relative: after a camera cut the old camera keeps
   steering while the stick is held (mode 1, then 2 while the direction holds within 15
   degrees) */
/* 0x00136900 */
void Hewie_LeftStick(Hewie *h) {
    static const union { u32 u; f32 f; } k15deg = {0x3E860A92}, k001 = {0x3C23D70A};
    /* the camera rotation the controls use: while moving in mode 1 the original reuses last
     * frame's (left on its stack); the PC build keeps it explicitly */
#ifdef HG_NATIVE
    static
#endif
    sceVu0FMATRIX rot;
    f32 e[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    s32 moving, cut, how = 0;

    HW(h, HMOVE_GO, u8) = 0;
    sceVu0CopyVector(e, gLeftStick);
    e[0] += (f32)(s32)(((gPadHeld >> 5) & 1) - ((gPadHeld >> 7) & 1));
    e[2] += (f32)(s32)(((gPadHeld >> 6) & 1) - ((gPadHeld >> 4) & 1));
    sceVu0Normalize(n, e);
    cut = HW(h, HMOVE_MODE, u8) == 3;
    if (!cut && VCALL(gCamera, 0x94, s32 (*)(VObject *))(gCamera) != -1) {
        s32 prev = VCALL(gCamera, 0x90, s32 (*)(VObject *))(gCamera);

        cut = prev != VCALL(gCamera, 0x94, s32 (*)(VObject *))(gCamera);
    }
    if (cut) {
        HW(h, HMOVE_MODE, u8) = 0;
        HW(h, HMOVE_STILL, s32) = 0;
        if (!((n[0] <= 0.0f ? -n[0] : n[0]) <= 0.5f) || !((n[2] <= 0.0f ? -n[2] : n[2]) <= 0.5f)) {
            HW(h, HMOVE_LOCK, s16) = 3;
            HW(h, HMOVE_MODE, u8) = 1;
        }
    }
    if ((e[0] <= 0.0f ? -e[0] : e[0]) <= 0.5f && (e[2] <= 0.0f ? -e[2] : e[2]) <= 0.5f) {
        moving = 0;
        HW(h, HMOVE_STILL, s32)++;
        if (HW(h, HMOVE_STILL, s32) >= 7) {
            HW(h, HMOVE_STILL, s32) = 6;
        }
    } else {
        moving = 1;
        HW(h, HMOVE_STILL, s32) = 0;
    }
    switch (HW(h, HMOVE_MODE, u8)) {
    case 0:
        if (moving) {
            how = 0;
        } else {
            how = AT(h->c.motion, 0x550, f32) <= 0.0f ? 2 : 1;
        }
        break;
    case 1:
        if (!moving) {
            how = 2;
            if (HW(h, HMOVE_STILL, s32) == 6) {
                HW(h, HMOVE_MODE, u8) = 0;
            }
            break;
        }
        how = 3;
        if (HW(h, HMOVE_LOCK, s16) != 0) {
            HW(h, HMOVE_LOCK, s16)--;
            Mtx_TurnY(rot, HW(h, HMOVE_CAMYAW, f32));
        } else {
            f32 d[4] __attribute__((aligned(16)));

            sceVu0SubVector(d, e, &HW(h, HMOVE_STICK, f32));
            if (__builtin_sqrtf(sceVu0InnerProduct(d, d)) < k001.f) {
                HW(h, HMOVE_MODE, u8) = 2;
                HW(h, 0xF379C, u32) = 0x3C0EFA35;   /* 0.5 degrees */
                Mtx_TurnY(rot, VCALL(gCamera, 0x68, f32 (*)(VObject *))(gCamera));
            }
        }
        Mtx_ApplyVector(v, rot, n);
        vu0_ScaleXYZ(v, v, -1.0f);
        HW(h, 0xF37A0, f32) = msl_atan2f(v[0], v[2]);
        break;
    case 2:
        if (moving) {
            f32 a = msl_atan2f(HW(h, HMOVE_LAST, f32), HW(h, HMOVE_LAST + 8, f32));

            how = 0;
            if (!(hwrap_abs(msl_atan2f(n[0], n[2]) - a) <= k15deg.f)) {
                HW(h, HMOVE_MODE, u8) = 0;
            }
            break;
        }
        how = 2;
        if (HW(h, HMOVE_STILL, s32) == 6) {
            HW(h, HMOVE_MODE, u8) = 0;
        }
        break;
    }
    switch (how) {
    case 3:
        Mtx_TurnY(rot, HW(h, HMOVE_CAMYAW, f32));
        Mtx_ApplyVector(v, rot, n);
        vu0_ScaleXYZ(&HW(h, HMOVE_DIR, f32), v, -1.0f);
        break;
    case 2:
        HW(h, HMOVE_DIR, f32) = 0.0f;
        HW(h, HMOVE_DIR + 4, f32) = 0.0f;
        HW(h, HMOVE_DIR + 8, f32) = 0.0f;
        break;
    case 1:
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 1.0f;
        sceVu0ApplyMatrix(&HW(h, HMOVE_DIR, f32), h->c.a.rot, v);
        break;
    case 0:
        Mtx_TurnY(rot, VCALL(gCamera, 0x68, f32 (*)(VObject *))(gCamera));
        Mtx_ApplyVector(v, rot, n);
        vu0_ScaleXYZ(&HW(h, HMOVE_DIR, f32), v, -1.0f);
        break;
    }
    sceVu0CopyVector(&HW(h, HMOVE_STICK, f32), e);
    if (HW(h, HMOVE_MODE, u8) == 0) {
        HW(h, HMOVE_CAMYAW, f32) = VCALL(gCamera, 0x68, f32 (*)(VObject *))(gCamera);
    }
    if (HW(h, HMOVE_MODE, u8) != 2) {
        sceVu0CopyVector(&HW(h, HMOVE_LAST, f32), n);
    }
    if (gPadHeld & 0x4000) {
        HW(h, HMOVE_GO, u8) = 1;
    }
}

/* ---- Fiona's commands ---- */

extern s32 Hewie_CommandAction(Hewie *h, s32 cmd);   /* the action for a command (-1 none, -2..-5 special) */
extern s32 Hewie_ActOnCommand(Hewie *h, s32 act);
extern u8 *Hewie_NearestThing(Hewie *h);

/* action `act` with argument `arg` unless his situation turns it into another (then that one,
   argument 0) */
static inline void hewie_want(Hewie *h, s32 act, s32 arg) {
    s32 a = Hewie_AdjustAction(h, act);

    Hewie_SetAction(h, a, a != act ? 0 : arg);
}

/* how long he keeps obeying (+0xF359C) by his trust level, the harder table on difficulty 1 */
static inline void obey_time(Hewie *h) {
    if ((Progress_GetVar(gProgress, 0x27) & 0xFF) != 1) {
        HW(h, 0xF359C, s32) = kHewieLevelNormal[HW(h, 0xF35CC, s16)];
    } else {
        HW(h, 0xF359C, s32) = kHewieLevelHard[HW(h, 0xF35CC, s16)];
    }
}

/* a command from Fiona (her +0x14EC, kept at +0xF3578): 1 if he acts on it. Hidden, only the
 * plain ones. Ending a wait (action 0x7D) other than by 0x30 resets his obedience; 0x23 can
 * make him find something to do near her (Hewie_NearestThing: action 0x1D); 0x29 / 0x2F first try
 * Hewie_PraiseScold (actions 0x1D / 0x71); otherwise the command's action (Hewie_CommandAction): -2
 * action 0x1E, -3 0x6F (back to the current one after), -4 / -5 0x1D / 0x71 with his mood
 * set, else Hewie_ActOnCommand */
/* 0x00137020 */
s32 Hewie_FionaCommand(Hewie *h) {
    s32 cmd, act;

    HW(h, 0xF3578, s32) = h->c.state[1];
    if (h->c.a.disabled) {
        act = Hewie_CommandAction(h, HW(h, 0xF3578, s32));
        if (act == -5 || act == -4 || act == -3 || act == -2 || act == -1) {
            return 0;
        }
        HW(h, 0xF3574, s32) = act;
        Hewie_ActOnCommand(h, act);
        return 1;
    }
    if (h->c.unkE0 == 0 && HW(h, 0xF358C, s32) == 1) {
        return 0;
    }
    if (HEWIE_ACTION(h) == 0x7D && HW(h, 0xF3578, s32) != 0x30) {
        HW(h, 0xF3598, s32) = 0;
        obey_time(h);
        HW(h, 0xF3586, u8) = 0;
    }
    cmd = HW(h, 0xF3578, s32);
    if (cmd != 0x2B && cmd != 0x2F) {
        HW(h, 0xF3686, s16) = 0;
        HW(h, 0xF3684, s16) = 0;
    }
    if (HW(h, 0xF3578, s32) == 0x23 && HW(h, 0xF3598, s32) == 0 && HW(h, 0xF358C, s32) != 1 &&
        !(u8)Progress_GameMode(gProgress) && HW(h, 0xF368C, s32) == 0 &&
        (u8)Actor_CanWalkBetween(h, h->c.a.navTri, gCharPlayer->a.navTri, h->c.a.pos, gCharPlayer->a.pos, 0) == 1) {
        HW(h, 0xF368C, u8 *) = Hewie_NearestThing(h);
        if (HW(h, 0xF368C, s32) != 0) {
            hewie_want(h, 0x1D, 0x78);
            h->c.state[0] = 0;
            return 1;
        }
    }
    if (HW(h, 0xF3578, s32) == 0x29) {
        HW(h, 0xF36A8, s32) = HEWIE_ACTION(h);
        if ((u8)Hewie_PraiseScold(h, 1, 3) == 1) {
            HW(h, 0xF35DC, s32) = 0x3C;
            hewie_want(h, 0x1D, 0);
            h->c.state[0] = 0;
            return 1;
        }
    }
    if (HW(h, 0xF3578, s32) == 0x2F) {
        HW(h, 0xF36A8, s32) = HEWIE_ACTION(h);
        if ((u8)Hewie_PraiseScold(h, 0, 3) == 1) {
            HW(h, 0xF35DC, s32) = 0x3C;
            hewie_want(h, 0x71, 0);
            h->c.state[0] = 0;
            return 1;
        }
    }
    act = Hewie_CommandAction(h, HW(h, 0xF3578, s32));
    if (act == -1) {
        return 0;
    }
    HW(h, 0xF35DC, s32) = 0x3C;
    switch (act) {
    case -2:
        hewie_want(h, 0x1E, 0);
        return 1;
    case -3: {
        s32 was = HEWIE_ACTION(h);

        hewie_want(h, 0x6F, was);
        return 1;
    }
    case -4:
        Hewie_SetMode(h, 1, -1);
        hewie_want(h, 0x1D, 0);
        return 1;
    case -5:
        Hewie_SetMode(h, 0, -1);
        HW(h, 0xF35C4, s32) = 0;
        HW(h, 0xF35C8, s32) = 0;
        hewie_want(h, 0x71, 0);
        return 1;
    }
    HW(h, 0xF3574, s32) = act;
    return Hewie_ActOnCommand(h, act);
}

/* ---- whom to go for ---- */

/* the one he goes for: the pursuer when it holds Fiona (her mode 4, sub 9) and he can reach
 * it; a creature (slots 7..9, mode 8) holding her (sub 0x12) he can reach; else the pursuer
 * if he can reach it (not while it moves 3); else the nearest hostile creature (+0x3C) he can
 * reach, not down or holding; NULL */
/* 0x001379C0 */
Character *Hewie_PickTarget(Hewie *h) {
    Character *best = NULL;
    f32 bestd = 0.0f;
    s32 chase = 0, i;

    if (in_his_room(h, gCharPursuer) && gCharPursuer->moveMode != 3 &&
        (u8)Hewie_PlanTo(h, gCharPursuer->a.navTri, gCharPursuer->a.pos) == 1) {
        chase = 1;
    }
    if (chase && in_his_room(h, gCharPlayer) && gCharPlayer->moveMode == 4 && gCharPlayer->moveSub == 9) {
        return gCharPursuer;
    }
    if (in_his_room(h, gCharPlayer) && gCharPlayer->moveMode == 4 && gCharPlayer->moveSub == 0x12) {
        for (i = 7; i < 10; i++) {
            Character *c = ((Character **)gCreatures)[i];

            if (in_his_room(h, c) && c->moveMode == 8 && (u8)Hewie_PlanTo(h, c->a.navTri, c->a.pos) == 1) {
                return c;
            }
        }
    }
    if (chase) {
        return gCharPursuer;
    }
    for (i = 0; i < 10; i++) {
        Character *c = ((Character **)gCreatures)[i];
        f32 d;

        if (!in_his_room(h, c) || (u8)VCALL(&c->a, 0x3C, s32 (*)(void *, u32))(c, i & 0xFF) != 1 || c->a.unkC4 == 2 ||
            c->moveMode == 4) {
            continue;
        }
        d = Actor_Distance(&h->c.a, c->a.pos);
        if ((best == NULL || d < bestd) && (u8)Hewie_PlanTo(h, c->a.navTri, c->a.pos) == 1) {
            best = c;
            bestd = d;
        }
    }
    return best;
}

/* his bite at bone `bone` of his model (within `margin`): each creature in slots 7..9 in his
 * room, hostile (+0x3C), not yet bitten (`done`: a bit per slot +0x20) and not protected
 * (+0x2D), whose body the point is in (its height +0x14 .. +0xCC, its radius +0xC8, plus the
 * margin) is hit for `damage` (state 4, unless already 7); the slots hit */
/* 0x00137FE0 */
u32 Hewie_Bite(Hewie *h, u32 done, s32 damage, s32 bone, f32 margin) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u32 hit = 0;
    s32 i;

    if (bone != -1) {
        sceVu0CopyVector(at, Skel_Bone(AT(h->c.motion, 0x810, void *), bone) + 12);
    }
    for (i = 0; i < 10; i++) {
        Character *c = ((Character **)gCreatures)[i];

        if (!in_his_room(h, c) || (u8)VCALL(&c->a, 0x3C, s32 (*)(void *, u32))(c, i & 0xFF) != 1 ||
            (done & (1 << c->a.slot))) {
            continue;
        }
        if (i < 7 || AT(c, 0x2D, u8) != 0) {
            continue;
        }
        if (at[1] <= c->a.pos[1] - margin || !(at[1] < margin + (c->a.pos[1] + AT(c, 0xCC, f32)))) {
            continue;
        }
        sceVu0SubVector(d, c->a.pos, at);
        if (!(__builtin_sqrtf(__builtin_fabsf(d[2] * d[2] + d[0] * d[0])) < margin + AT(c, 0xC8, f32))) {
            continue;
        }
        if (c->state[0] != 7) {
            /* (the original copies a local whose last fields are never set) */
            c->state[0] = 4;
            c->state[1] = 1;
            c->state[2] = 1;
            c->state[3] = damage;
            c->state[4] = 0;
            *(f32 *)&c->state[5] = 0.0f;
            c->state[6] = 0;
            c->state[7] = 0;
        }
        hit |= 1 << c->a.slot;
    }
    return hit;
}

/* whether he dares go for the character in `slot`: from trust level 2, a 1-in-16 roll under
   his level + 1 + his feeling for its kind (+0xF367C.. / 5, at most 8) */
/* 0x00138460 */
u32 Hewie_Dares(Hewie *h, s32 slot) {
    Character *c = (Character *)gCharacters[slot];
    s32 n = 0;

    if (HW(h, 0xF35CC, s16) < 2 || c == NULL || c->a.active != 1) {
        return 0;
    }
    switch (c->unk153C) {
    case 2: case 6: case 7: case 27:   /* Debilitas */
        n = HW(h, 0xF367C, s16) / 5;
        break;
    case 3: case 34: case 35: case 36:   /* Daniella */
        n = HW(h, 0xF367E, s16) / 5;
        break;
    case 4: case 23: case 37:   /* Riccardo */
        n = HW(h, 0xF3682, s16) / 5;
        break;
    case 10: case 11: case 12: case 39:   /* Lorenzo */
        n = HW(h, 0xF3680, s16) / 5;
        break;
    }
    if (n >= 9) {
        n = 8;
    }
    return (s32)(16.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) < n + HW(h, 0xF35CC, s16) + 1;
}

/* praised (`praise` 1) or scolded for what he just did (+0xF36A4, when it is still +0xF36A8):
   after action 0x78 his skills involved (+0xF36A2 of them, indices +0xF3696) move by `by`
   (0..31) - up for good ones (+0xF369C), down for bad ones; scolding the other way. 1 if it
   counted */
/* 0x001391E0 */
s32 Hewie_PraiseScold(Hewie *h, s32 praise, s8 by) {
    s32 i;

    if (HW(h, 0xF36A4, s32) == 0 || HW(h, 0xF36A4, s32) != HW(h, 0xF36A8, s32)) {
        return 0;
    }
    if (HW(h, 0xF36A8, s32) == 0x78) {
        for (i = 0; i < HW(h, 0xF36A2, u8); i++) {
            u8 k = HW(h, 0xF3696 + i, u8);
            s8 *v = &HW(h, 0xF3690 + k, s8);
            s32 up = HW(h, 0xF369C + k, u8) != 0;

            if (praise != 1) {
                up = !up;
            }
            if (up) {
                *v += by;
                v = &HW(h, 0xF3690 + HW(h, 0xF3696 + i, u8), s8);
                if (*v >= 0x20) {
                    *v = 0x1F;
                }
            } else {
                *v -= by;
                v = &HW(h, 0xF3690 + HW(h, 0xF3696 + i, u8), s8);
                if (*v < 0) {
                    *v = 0;
                }
            }
        }
    }
    HW(h, 0xF368C, s32) = 0;
    HW(h, 0xF36AC, s32) = 0;
    HW(h, 0xF36A4, s32) = 0;
    return 1;
}

#define F_PI_2 0x1.921fb6p+0f   /* 0x3FC90FDB */

/* Fiona's angle to a point, |wrapped| (from her heading) */
static f32 off_her_heading(const f32 *pos) {
    return hwrap_abs(Actor_HeadingTo(&gCharPlayer->a, pos) - gCharPlayer->a.angle[1]);
}

/* the nearest placed thing of kind 0 (+0x20) in front of Fiona (within 90 degrees) that he can
   reach (the triangle at it is its own, +0x34); NULL */
/* 0x00139460 */
u8 *Hewie_NearestThing(Hewie *h) {
    u8 *best = NULL;
    f32 bestd = 0.0f;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        u8 *t = VCALL(gPlacedThings, 0xC, u8 *(*)(VObject *, s32))(gPlacedThings, i);

        if (t == NULL || AT(t, 0x20, s32) != 0) {
            continue;
        }
        if (best != NULL) {
            f32 d = Actor_Distance(&gCharPlayer->a, (f32 *)(t + 0x10));

            if (d < bestd && off_her_heading((f32 *)(t + 0x10)) < F_PI_2 &&
                Actor_TriTo(&h->c.a, (f32 *)(t + 0x10), -1) == AT(t, 0x34, u32)) {
                best = t;
                bestd = d;
            }
        } else if (off_her_heading((f32 *)(t + 0x10)) < F_PI_2 &&
                   Actor_TriTo(&h->c.a, (f32 *)(t + 0x10), -1) == AT(t, 0x34, u32)) {
            best = t;
            bestd = Actor_Distance(&gCharPlayer->a, (f32 *)(t + 0x10));
        }
    }
    return best;
}

/* the side to go at the pursuer from: 0 head on (they face each other within 90 degrees,
   not with progress flag 0x11); else 1 / 2 by its kind and whether he is on its left */
/* 0x00139A70 */
s32 Hewie_AttackSide(Hewie *h) {
    Character *pu = gCharPursuer;
    f32 a = Actor_HeadingTo(&h->c.a, pu->a.pos);   /* (from him) */
    f32 its = hwrap_abs(a - pu->a.angle[1]);
    u8 kind;

    if (!(u8)Progress_TestFlag(gProgress, 0x11) && its < F_PI_2 && hwrap_abs(a - h->c.a.angle[1]) < F_PI_2) {
        return 0;
    }
    kind = gCharPursuer->unk153C;
    if (Angle_Wrap(Angle_Wrap(F_PI + a) - gCharPursuer->a.angle[1]) < 0.0f) {
        return kind == 3 || kind == 34 || kind == 35 || kind == 36 ? 1 : 2;
    }
    return kind == 3 || kind == 34 || kind == 35 || kind == 36 || kind == 11 ? 2 : 1;
}

/* the animation's stride this frame (its root motion's z by the model's speed (+0x48)), at
   least 0 */
static f32 stride(Hewie *h, f32 *v) {
    Motion_RootMovement(h->c.motion, v, 0.0f);
    v[2] *= VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    return v[2];
}

/* one stride along his path: he turns towards a point 12 strides ahead (by the stride x 6 x
 * the slope +0x858, degrees; over 30 degrees off within the turn), the motion's blend +0xF3604
 * 8 frames; facing within ~41 degrees the stride's point he steps there (+0x128 its index;
 * 1), else he takes the stride straight ahead (the path back to +0x128; 0) */
/* 0x00139DE0 */
s32 Hewie_Stride(Hewie *h) {
    static const union { u32 u; f32 f; } k30deg = {0x3F060A92};
    f32 v[4] __attribute__((aligned(16)));    /* (its w, from the root motion, stays) */
    f32 v2[4] __attribute__((aligned(16)));
    f32 v3[4] __attribute__((aligned(16)));
    f32 ahead[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 s, rate, slope, yaw, off;
    u32 tri;
    s32 next;

    s = stride(h, v);
    if (s < 0.0f) {
        s = 0.0f;
    }
    Character_WaypointAhead(&h->c, &tri, ahead, 12.0f * s);
    yaw = Actor_HeadingTo(&h->c.a, ahead);
    next = Character_WaypointAhead(&h->c, &tri, at, s);
    s = stride(h, v2);
    rate = 0.0f;
    if (!(s < 0.0f)) {
        slope = AT(h->c.motion, 0x858, f32);
        if (slope <= 0.0f) {
            slope = -slope;
        }
        rate = s * (12.0f * (0.5f * slope));
    }
    rate = F_PI * rate / 180.0f;
    off = Angle_Wrap(yaw - h->c.a.angle[1]);
    if (!((off <= 0.0f ? -off : off) < k30deg.f) && AT(h->c.motion, 0x858, f32) * off < 0.0f) {
        /* turning against the slope: straight round by the rate */
        h->c.a.angle[1] = Angle_Wrap(off < 0.0f ? h->c.a.angle[1] + rate : h->c.a.angle[1] - rate);
        sceVu0UnitMatrix(h->c.a.rot);
        sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, h->c.a.angle[1]);
        Angle_Wrap(yaw - h->c.a.angle[1]);
    } else {
        Actor_TurnToward(&h->c.a, yaw, rate);
    }
    HW(h, 0xF3604, s32) = 8;
    HW(h, 0xF3608, s32) = 0;
    HW(h, 0xF3614, s32) = 0;
    HW(h, 0xF3618, f32) = Angle_Wrap(yaw - h->c.a.angle[1]);
    sceVu0SubVector(d, at, h->c.a.pos);
    d[1] = 0.0f;
    sceVu0Normalize(d, d);
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 1.0f;
    sceVu0ApplyMatrix(v, h->c.a.rot, v);
    if (!(sceVu0InnerProduct(v, d) <= 0.75f)) {
        h->c.a.navTri = tri;
        sceVu0CopyVector(h->c.a.pos, at);
        h->c.unk128 = next;
        HW(h, 0xF3558, u8) = 1;
        return 1;
    }
    stride(h, v3);
    v3[1] = 0.0f;
    sceVu0ApplyMatrix(v3, h->c.a.rot, v3);
    Actor_Move(&h->c.a, v3);
    HW(h, 0xF3558, u8) = 1;
    h->c.unk124 = h->c.unk128;
    return 0;
}

#include "effectmgr.h"   /* HitEffect_Spawn */

/* a bite's hit effect at his mouth (bone 0x25): a big and a small one when `hard`, else small */
/* 0x0013A1C0 */
void Hewie_BiteEffect(Hewie *h, s32 hard) {
    HitEffectParams hp;

    sceVu0CopyVector(hp.pos, Skel_Bone(AT(h->c.motion, 0x810, void *), 0x25) + 12);
    hp.kind = 1;
    switch (hard) {
    case 1:
        hp.big = 1.0f;
        HitEffect_Spawn(&hp);
        /* fall through */
    case 0:
        hp.big = 0.0f;
        HitEffect_Spawn(&hp);
        break;
    }
}

/* his sounds by his animation (+0x55C): the yelps and whimpers (4 0x6F, 9 0x70 when +0xF3585,
 * 5 0x58, 6 / 7 / 0x206 0x59) at once; the others on the animation's sound event (bit 0):
 * barks and growls (barking during actions 0xA / 0xB / 0x7B also makes a noise others hear),
 * a howl (0x1000) heard far; on bit 4, his splash (0x1001) in the room's water (triangle flags
 * 0x02008000) - and in rooms 7 / 0x106 the ripple */
/* 0x0013A650 */
void Hewie_AnimSounds(Hewie *h) {
    s32 anim = AT(h->c.motion, 0x55C, s32);
    s32 loud = 0;   /* (the original leaves the caller's register for the other actions) */

    switch (anim) {
    case 4:
        Hewie_MakeSound(h, 0x6F);
        return;
    case 9:
        if (HW(h, 0xF3585, u8) == 1) {
            Hewie_MakeSound(h, 0x70);
            return;
        }
        break;
    case 5:
        Hewie_MakeSound(h, 0x58);
        return;
    case 6: case 7: case 0x206:
        Hewie_MakeSound(h, 0x59);
        return;
    }
    if ((u8)Motion_EventFlags(h->c.motion, 0, 0, 1) & 1) {
        switch (anim) {
        case 0x1C01:
            Hewie_MakeSound(h, 0x60);
            break;
        case 0x1B00: case 0x1B01: case 0x1B02:
            Hewie_MakeSound(h, 0x5A);
            break;
        case 0x1B03:
            Hewie_MakeSound(h, HEWIE_ACTION(h) == 0x1D ? 0x5A : 0x5D);
            break;
        case 0x1B04:
            Hewie_MakeSound(h, HEWIE_ACTION(h) == 0x1D ? 0x5A : 0x5E);
            break;
        case 0x1B05:
            Hewie_MakeSound(h, 0x5F);
            break;
        case 0x1C04:
            Hewie_MakeSound(h, 0x5C);
            break;
        case 0x1001: case 0x220C: case 0x2203:
            Hewie_MakeSound(h, 0x66);
            break;
        }
        switch (anim) {
        case 0x1B00: case 0x1B01: case 0x1B02: case 0x1B03: case 0x1B04: case 0x1B05:
            if (HEWIE_ACTION(h) == 0x7B || HEWIE_ACTION(h) == 0xB || HEWIE_ACTION(h) == 0xA) {
                loud = 0x1B;
            }
            Noise_Make((u8 *)gProgress + 0x788, loud, h->c.a.room, h->c.a.navTri, 0xFFFF);
            break;
        case 0x1000:
            Noise_Make((u8 *)gProgress + 0x788, 0x80, h->c.a.room, h->c.a.navTri, 0xFFFF);
            break;
        }
    }
    if (((u8)Motion_EventFlags(h->c.motion, 0, 0, 1) & 0x10) && anim == 0x1001 &&
        (u8)VCALL(gProgress, 0x50, s32 (*)(Progress *))(gProgress) == 1 &&
        h->c.a.room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) &&
        (NavMesh_Tri(gNavMesh, h->c.a.navTri)->flags & 0x02008000) == 0x02008000) {
        Actor_PlaySound(&h->c.a, 0x1E, 6, 0, 0, NULL);
        if (h->c.a.room == 7 || h->c.a.room == 0x106) {
            Character_WaterStep(&h->c, h->c.a.pos, 1);
        }
    }
}

extern const s16 D_003B127E[];   /* by how often Fiona hit him (+0xF35C4): the 1-in-16 he sulks */

/* a blow `hit` (a character's state block: +0x4 how, +0x8 by whom, +0xC damage, +0x10 the
 * door): down (+0xC4 2) he only goes limp (action 0x74) unless it is 6; held (+0xF8 4) only
 * 0xB counts; nor while he is 8 / 0x18..0x19; else the damage (vtable +0x94, double from
 * Fiona), a door's only when it moves (+0x40). Then by `how`: 1..4 he yelps (action 0x69) -
 * from Fiona (by 0) he learns from it and may sulk (action 3 mode), from another sometimes
 * holds a grudge; 3 also knocks him down - 5 a door: pushed aside (0x6B) or back (0x6C) by
 * which side of it he is; 6 / 0xB: -1 (not taken) */
/* 0x0013AC20 */
s32 Hewie_TakeBlow(Hewie *h, s32 *hit) {
    Progress *p;
    f32 yaw;

    if (h->c.a.unkC4 == 2 && hit[1] != 6) {
        if (HEWIE_ACTION(h) != 0x74) {
            hewie_want(h, 0x74, 0);
        }
        return 0;
    }
    if (h->c.moveMode == 4 && hit[1] != 0xB) {
        return -1;
    }
    if (hit[1] != 0xB && h->c.moveMode == 8 && (u32)(h->c.moveSub - 0x18) < 2) {
        return -1;
    }
    if (hit[1] != 5) {
        if (HEWIE_MODE(h) == 3) {
            HW(h, 0xF36B0, s32) = hit[2];
        }
        VCALL(&h->c.a, 0x94, void (*)(Hewie *, s32))(h, hit[2] == 0 ? hit[3] * 2 : hit[3]);
    } else {
        if (!(u8)VCALL(gDoors, 0x40, s32 (*)(VObject *, u8))(gDoors, ((u8 *)hit)[0x10])) {
            return 0;
        }
        VCALL(&h->c.a, 0x94, void (*)(Hewie *, s32))(h, hit[3]);
    }
    if (HW(h, 0xF35AC, s32) <= 0) {
        HW(h, 0xF35AC, s32) = 300;
    }
    AT(h, 0x2B, u8) = 0;
    switch (hit[1]) {
    case 3:
        h->c.a.unkC4 = 2;
        /* fall through */
    case 1: case 2: case 4:
        h->c.unk104[0] = hit[1];
        h->c.unk100 = hit[2];
        if (h->c.unk100 != 0xFF) {
            if (h->c.unk100 != 0) {
                if (VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 0.25f) {
                    Hewie_ChangeFeeling(h, (Character *)gCharacters[h->c.unk100], -1);
                }
                HW(h, 0xF35C4, s32) = 0;
            } else {
                HW(h, 0xF36A8, s32) = HEWIE_ACTION(h);
                Hewie_PraiseScold(h, 0, 3);
                HW(h, 0xF35C4, s32)++;
                if (HW(h, 0xF35C4, s32) >= 7) {
                    HW(h, 0xF35C4, s32) = 6;
                }
                HW(h, 0xF35C8, s32) = 300;
                if ((s16)(s32)(16.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) <
                    D_003B127E[HW(h, 0xF35C4, s32)]) {
                    HW(h, 0xF36B0, s32) = 0xFF;
                    Hewie_SetMode(h, 3, -1);
                }
            }
        }
        hewie_want(h, 0x69, 0);
        return 0;
    case 5:
        h->c.unk104[0] = h->c.state[1];
        h->c.unk100 = h->c.state[2];
        HW(h, 0xF36B4, s32) = h->c.state[4];
        p = gProgress;
        yaw = VCALL(gDoors, 0x3C, f32 (*)(VObject *, u8))(gDoors, HW(h, 0xF36B4, u8));
        if (!((u8)PursuerGroup_Fields(p, HW(h, 0xF36B4, u8), (u8)h->c.unk100) & 0x10)) {
            AT(h, 0x2B, u8) = 1;
            hewie_want(h, 0x6B, 0);
        } else if (((u8)PursuerGroup_Fields(p, HW(h, 0xF36B4, u8), AT(h, 0x20, u8)) & 8) &&
                   (u8)Progress_ExitUnlocked(p, h->c.a.room, HW(h, 0xF36B4, u8)) != 1 &&
                   (u8)Progress_ExitPassable(p, h->c.a.room, HW(h, 0xF36B4, u8), AT(h, 0x20, u8))) {
            yaw += F_PI;
            hewie_want(h, 0x6C, 0);
        } else if (!(NavMesh_Tri(gNavMesh, h->c.a.navTri)->flags & 0x20000)) {
            yaw += F_PI_2;
            hewie_want(h, 0x6B, 0);
        }
        HW(h, 0xF36C4, f32) = Angle_Wrap(yaw);
        return 0;
    case 6: case 0xB:
        return -1;
    }
    return 0;
}

extern const s16 D_003B1290[][2];   /* the scuffle's outcomes: {outcome, percent} */

/* a scuffle with the pursuer while it hunts in his room (its mode 1..3, not caught +0xE0, once
 * per meeting +0xF368A, not within 900 frames +0xF35B0, him not down): an outcome by chance
 * (D_003B1290) - 0 / 1 he is hurt (a tenth of his health, never below 1; maybe a grudge), 2 /
 * 0 yelp or 1 / 3 bark heard next door, 3 nothing - the pursuer told how long it is held
 * (30..180 frames) and hurt (a tenth of its health, more on hard and with +0xA10; none for 1
 * or with progress flags 0x13 / 0x2B, then maybe his grudge), and he goes for it (action
 * 0x30). -1 if no scuffle */
/* 0x0013BA50 */
s32 Hewie_Scuffle(Hewie *h) {
    Progress *p;
    const s16 *o;
    s32 outcome, frames, roll, sum, cur, e, next = 0;
    VObject *rng;

    if (gCharPursuer != NULL && gCharPursuer->a.active == 1 && !in_his_room(h, gCharPursuer)) {
        HW(h, 0xF368A, u8) = 0;
    }
    if ((u32)(h->c.a.unkC4 - 1) < 2 || HW(h, 0xF35B0, s16) != 0) {
        return -1;
    }
    if (!(gCharPursuer != NULL && gCharPursuer->a.active == 1 && !gCharPursuer->unkE0 && in_his_room(h, gCharPursuer) &&
          HW(h, 0xF368A, u8) == 0 && (u32)(AT(gCharPursuer, 0x16C8, u8) - 1) < 3)) {
        return -1;
    }
    HW(h, 0xF368A, u8) = 1;
    HW(h, 0xF35B0, s16) = 900;
    rng = gRandom;
    roll = (s16)(s32)(100.0f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng));
    for (o = D_003B1290[0], sum = 0;; o += 2) {
        sum = (s16)(sum + o[1]);
        if (roll < sum) {
            break;
        }
    }
    outcome = o[0];
    if (outcome == 3) {
        return 0;
    }
    frames = (s16)((s16)(s32)(6.0f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng)) * 30 + 30);
    if ((u32)outcome < 2) {
        VCALL(&h->c.a, 0x94, void (*)(Hewie *, s32))(h, (s32)(0x1.99999a0000000p-4f /* 0.1 */ * (f32)h->c.hpMax));
        if (h->c.hp == 0) {
            h->c.hp = 1;
        }
        if (VCALL(rng, 0x1C, f32 (*)(VObject *))(rng) < 0.25f) {
            Hewie_ChangeFeeling(h, gCharPursuer, -1);
        }
    }
    p = gProgress;
    cur = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    for (e = 0; e < 8; e++) {
        if (VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, cur, e) == h->c.a.room) {
            next = 1;
            break;
        }
    }
    if (next) {
        f32 at[4] __attribute__((aligned(16)));

        if ((u8)Actor_PosInCurrentRoom(&h->c.a, at) == 1) {
            sceVu0SubVector(at, at, h->c.a.pos);
            Actor_PlaySound(&h->c.a, outcome == 2 ? 0x69 : 0x66, 5, 0, 0, at);
        }
    }
    h->c.unk1388 = h->c.unk1384;
    if (outcome == 1 || ((u8)Progress_TestFlag(gProgress, 0x13) | (u8)Progress_TestFlag(gProgress, 0x2B)) != 0) {
        Progress_GetVar(p, 0x27);
        Relation_Request(p, AT(h, 0x20, u8), 4, 8, 0, frames, 0.0f);
    } else {
        u32 dmg = (u16)(u32)(0x1.99999a0000000p-4f /* 0.1 */ * (f32)gCharPursuer->hpMax);

        if ((Progress_GetVar(p, 0x27) & 0xFF) == 1) {
            dmg = (u16)(dmg * 2);
        }
        if (AT(p, 0xA10, s32) != 0) {
            dmg = (u16)(dmg + (dmg >> 1));
        }
        Relation_Request(p, AT(h, 0x20, u8), 4, 8, dmg, frames, 0.0f);
        if (VCALL(rng, 0x1C, f32 (*)(VObject *))(rng) < 0x1.5554760000000p-2f /* 0.33333 */) {
            Hewie_ChangeFeeling(h, gCharPursuer, 1);
        }
    }
    HW(h, 0xF3560, s32) = (s16)frames;
    hewie_want(h, 0x30, 0);
    return 0;
}

/* after a yelp: down - a grudge against who did it (not Fiona), the times he was knocked out
 * counted (progress +0xFBA, to 9999; not after the game's end, +0x30 bit 15), and he lies
 * there (action 0x52); else +0x2D off and, struck by Fiona with her in his room (her control,
 * not his), he cowers (action 9 the first time, 0xB after); else back to normal */
/* 0x0013C300 */
void Hewie_AfterYelp(Hewie *h) {
    if (h->c.a.unkC4 == 2) {
        if (h->c.unk100 != 0xFF && h->c.unk100 != 0) {
            Hewie_ChangeFeeling(h, (Character *)gCharacters[h->c.unk100], -3);
        }
        if (!(AT(gProgress, 0x30, u32) & 0x8000)) {
            s16 *n = &AT(gProgress, 0xFBA, s16);

            if (++*n >= 10000) {
                *n = 9999;
            }
        }
        hewie_want(h, 0x52, 0);
        return;
    }
    AT(h, 0x2D, u8) = 0;
    if (AT(gProgress, 0x1FBEC1, u8) == 0 && h->c.unk100 == 0 && in_his_room(h, gCharPlayer) &&
        HW(h, 0xF35C4, s32) > 0) {
        hewie_want(h, HW(h, 0xF35C4, s32) == 1 ? 9 : 0xB, 0);
        return;
    }
    hewie_want(h, 0, 0);
}

extern s32 Hewie_SituationList(Hewie *h, u32 kind);   /* which of a table's lists fits him */
extern const s8 D_003B1240[];   /* by trust: the chance (percent) he growls at the pursuer */
/* his action lists, per situation, by Hewie_SituationList: {action, weight per trust level 0..7} */
extern u8 *const D_003B02C0[], *const D_003B05F0[], *const D_003B0850[], *const D_003B0480[], *const D_003B06F0[],
    *const D_003B0A90[], *const D_003B0E20[];

/* what to do next: with the pursuer in his room (not caught 3 / 4) and progress flag 9 or
 * 0xA, by chance (by trust) he growls at it (action 0x7B); else an action drawn from the list
 * for the situation - a hostile creature in the room, else the chase state (0 calm, 1 being
 * followed, 2 the chase), lists for obeying (+0xF3598) and for hard (Progress var 0x27) -
 * weighted by his trust level */
/* 0x0013C7D0 */
void Hewie_WhatNext(Hewie *h) {
    Progress *p;
    u8 *const *lists;
    u8 *e;
    s32 hostile = 0, mode, i, sum, roll;

    if (in_his_room(h, gCharPursuer) && gCharPursuer->moveMode != 3 && gCharPursuer->moveMode != 4 &&
        ((u8)Progress_TestFlag(gProgress, 9) == 1 || (u8)Progress_TestFlag(gProgress, 0xA) == 1)) {
        s32 chance = D_003B1240[HW(h, 0xF35CC, s16)];
        s8 r = (s8)(s32)(100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom));

        if (chance < 0) {
            chance = 0;
        } else if (chance >= 101) {
            chance = 100;
        }
        if (r < (s8)chance) {
            hewie_want(h, 0x7B, 0);
            return;
        }
    }
    p = gProgress;
    for (i = 0; i < 10; i++) {
        Character *c = ((Character **)gCreatures)[i];

        if (in_his_room(h, c) && (u8)VCALL(&c->a, 0x3C, s32 (*)(void *, u32))(c, i & 0xFF) == 1) {
            hostile = 1;
            break;
        }
    }
    mode = (u8)Progress_GameMode(p);
    if (HW(h, 0xF3598, s32) == 0) {
        if (hostile) {
            e = D_003B0850[Hewie_SituationList(h, 2)];
        } else {
            i = Hewie_SituationList(h, 0xFF);
            lists = mode == 0 ? D_003B02C0 : mode == 1 ? D_003B05F0 : D_003B0850;
            e = lists[i];
        }
    } else if (hostile) {
        i = Hewie_SituationList(h, 2);
        e = ((Progress_GetVar(p, 0x27) & 0xFF) != 1 ? D_003B0A90 : D_003B0E20)[i];
    } else {
        i = Hewie_SituationList(h, 0xFF);
        if (mode == 0) {
            e = D_003B0480[i];
        } else if (mode == 1) {
            e = D_003B06F0[i];
        } else {
            e = ((Progress_GetVar(p, 0x27) & 0xFF) != 1 ? D_003B0A90 : D_003B0E20)[i];
        }
    }
    roll = (s32)(100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom));
    for (sum = 0;; e += 0xC) {
        sum += e[4 + HW(h, 0xF35CC, s16)];
        if (roll < sum) {
            break;
        }
    }
    hewie_want(h, *(s32 *)e, 0);
}

/* ---- where to run ---- */

/* the exit of his room to flee from `from` by: exits whose door is open and passable from his
 * side (and from `from`'s too if `both`), nearest first, those within 20 of `from` last; the
 * first he can plan a path to. 0xFF if none, or `from` isn't in his room */
/* 0x0013CDC0 */
u8 Hewie_FleeExit(Hewie *h, Character *from, s32 both) {
    VObject *rooms, *doors, *planner;
    Progress *p;
    f32 dist[8];
    u8 exits[8];
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 goal[4] __attribute__((aligned(16)));
    s32 n = 0, i;
    u32 e;

    if (!in_his_room(h, from)) {
        return 0xFF;
    }
    rooms = gRooms;
    p = gProgress;
    doors = gDoors;
    for (e = 0; e < 8; e = (e + 1) & 0xFF) {
        u32 door;
        s32 near = 0;
        f32 len;

        if ((u8)VCALL(rooms, 0x74, s32 (*)(VObject *, s32, u32))(rooms, h->c.a.room, e) != 1) {
            continue;
        }
        door = VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, h->c.a.room, e) & 0xFFFF;
        if ((u8)Progress_DoorOpen(p, door) != 1 || (u8)Progress_DoorUnlocked(p, door) ||
            (u8)Progress_DoorPassable(p, door, (u8)h->c.a.slot) != 1 ||
            ((u8)both && (u8)Progress_DoorPassable(p, door, (u8)from->a.slot) != 1)) {
            continue;
        }
        VCALL(doors, 0x34, void (*)(VObject *, u32, f32 *))(doors, e, at);
        if (from != NULL) {
            sceVu0SubVector(d, at, from->a.pos);
            if (__builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) < 20.0f) {
                near = 1;
                exits[n] = e;
                dist[n++] = 100000.0f;
            }
        }
        if (near) {
            continue;
        }
        sceVu0SubVector(d, at, h->c.a.pos);
        len = __builtin_sqrtf(d[2] * d[2] + d[0] * d[0]);
        for (i = n; i > 0 && len <= dist[i - 1]; i--) {
            dist[i] = dist[i - 1];
            exits[i] = exits[i - 1];
        }
        exits[i] = e;
        n++;
        dist[i] = len;
    }
    planner = gSceneGameF29740;
    for (i = 0; i < n; i++) {
        u32 tri = VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, exits[i], goal);
        s32 r = Character_PlanPathKind(&h->c, 0, tri, goal);

        if (r > 0) {
            r = VCALL(planner, 0x14, s32 (*)(VObject *))(planner);
        }
        if (r > 0) {
            return exits[i];
        }
    }
    return 0xFF;
}

/* ---- which list fits ---- */

/* Fiona's distance band from him: 0 (< 10), 1 (< 20), 2 (< 40), 3 (< 60), 4 (< 80), 5 (< 100), 6
 * (farther or not in his room) */
static s32 player_band(Hewie *h) {
    f32 d;

    if (!in_his_room(h, gCharPlayer)) {
        return 6;
    }
    d = Actor_Distance(&h->c.a, gCharPlayer->a.pos);
    return d < 10.0f ? 0 : d < 20.0f ? 1 : d < 40.0f ? 2 : d < 60.0f ? 3 : d < 80.0f ? 4 : d < 100.0f ? 5 : 6;
}

/* the list of a situation table that fits him (kind 0xFF: the game's current mode). Calm (not
 * mode 2): Fiona's distance band. Tense (mode 2), unless panic is 4 or more (0): 1 if the
 * pursuer in his room moves 1 or 4, 2 if it is within 30 of him, 3 within 30 of Fiona, else 4
 * plus her band */
/* 0x0013E920 */
s32 Hewie_SituationList(Hewie *h, u32 kind) {
    Progress *p;

    if ((u8)kind == 0xFF) {
        kind = Progress_GameMode(gProgress);
    }
    if ((u8)kind != 2) {
        return player_band(h);
    }
    p = gProgress;
    if (AT(p, 0x7B8, u8) >= 4) {
        return 0;
    }
    if (in_his_room(h, gCharPursuer)) {
        if (gCharPursuer->a.unkC4 == 1 || gCharPursuer->moveMode == 4) {
            return 1;
        }
        if (Actor_Distance(&h->c.a, gCharPursuer->a.pos) < 30.0f) {
            return 2;
        }
        if (Actor_Distance(&gCharPlayer->a, gCharPursuer->a.pos) < 30.0f) {
            return 3;
        }
    }
    return 4 + player_band(h);
}

/* ---- idling ---- */

extern const s32 D_003B13D0[8];   /* wait by trust */
extern const u8 D_003B11B0[];     /* idle actions {s32 action, u8 weight[8]} */

/* what he does when idle: action 0x36 while +0xF3583 is set; when moving 1, a quarter of the
 * time action 0x2F with the wait for his trust; else a weighted pick from D_003B11B0, the
 * action that suits the game mode weighted 20 more (0x2C calm-only or mode 2; 0x32 mode 0, 0x2E
 * mode 1) */
/* 0x0013E680 */
void Hewie_WhenIdle(Hewie *h) {
    const u8 *e;
    Progress *p;
    s32 sum, roll;

    if (HW(h, 0xF3583, u8) == 1) {
        hewie_want(h, 0x36, 0);
        return;
    }
    if (h->c.a.unkC4 == 1 && VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 0.25f) {
        HW(h, 0xF3560, s32) = D_003B13D0[HW(h, 0xF35CC, s16)];
        hewie_want(h, 0x2F, 0);
        return;
    }
    roll = (s32)(100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom));
    p = gProgress;
    for (sum = 0, e = D_003B11B0;; e += 0xC) {
        s32 act = *(const s32 *)e;
        s32 favoured = 0x2C;

        sum += e[4 + HW(h, 0xF35CC, s16)];
        if (HW(h, 0xF3598, s32) != 0) {
            switch ((u8)Progress_GameMode(p)) {
            case 0: favoured = 0x32; break;
            case 1: favoured = 0x2E; break;
            case 2: break;
            default: favoured = -1; break;
            }
        }
        if (act == favoured) {
            sum += 0x14;
        }
        if (roll < sum) {
            break;
        }
    }
    hewie_want(h, *(const s32 *)e, 0);
}

/* ---- what a command makes him do ---- */

/* the action Fiona's command `cmd` makes him take, or: -1 none, -2 a refusal (when in an idle
 * group 1 or 5), -4 the come-here stay (0x29) and -5 the wait (0x2F) when he is calm and
 * +0xF3688 is set or a quarter of the time. Out of reach (+0xE0 clear) only when
 * listening (+0xF356C bit 3 without the top bit, +0xF35DC clear, not moving 4); downed only
 * 0x2A. A plain command he takes once when listening or out of reach, the game calm or (+0xF3598)
 * already waiting (0x7D), and not in mood 3 */
/* 0x0013E2D0 */
s32 Hewie_CommandAction(Hewie *h, s32 cmd) {
    u8 reach = h->c.unkE0;
    s32 take;

    if (reach == 0) {
        if (h->c.a.unkC4 == 2) {
            return cmd == 0x2A ? cmd : -1;
        }
        if ((HW(h, 0xF356C, u32) & 0x80000008) != 8 || HW(h, 0xF35DC, s32) != 0 || h->c.moveMode == 4) {
            return -1;
        }
    } else if (h->c.a.unkC4 == 2) {
        return -1;
    }
    if (h->c.a.disabled) {
        return cmd;
    }
    switch (cmd) {
    case 0x28:
    case 0x2B:
    case 0x30:
        return cmd;
    case 0x2F:
        if (h->c.a.unkC4 == 1 || HW(h, 0xF3598, s32) != 0 || (u32)HW(h, 0xF35C0, s32) > 2) {
            return -1;
        }
        if (HW(h, 0xF3688, s16) != 0 || VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 0.25f) {
            return -5;
        }
        return -1;
    case 0x29:
        if (h->c.a.unkC4 == 1 || HW(h, 0xF3598, s32) != 0 || (u32)HW(h, 0xF35C0, s32) > 1 || h->c.hp < 0x50) {
            return -1;
        }
        if (HW(h, 0xF3688, s16) != 0 || VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 0.25f) {
            return -4;
        }
        return -1;
    }
    take = HW(h, 0xF358C, s32) != 1 && (reach == 1 || (HW(h, 0xF356C, u32) & 0x80000008) == 8) &&
           (HW(h, 0xF3598, s32) == 0 || HW(h, 0xF3564, s32) == 0x7D) && HW(h, 0xF35C0, s32) != 3;
    HW(h, 0xF358C, s32) = 0;
    if (take) {
        return cmd;
    }
    if ((u8)Progress_GameMode(gProgress)) {
        return -1;
    }
    take = Hewie_AnimGroup(h);
    return take == 5 || take == 1 ? -2 : -1;
}

/* ---- where his target is ---- */

/* the triangle (and in `out` the point) of his target: the character he goes for when it is in
 * his room, else in target mode 2 the exit +0xF3670 of his room; -1 if none. When it can't be
 * walked to straight, a path is planned there and the point becomes the one 20 along it */
/* 0x0013EFB0 */
s32 Hewie_TargetTri(Hewie *h, f32 *out) {
    Character *t = HEWIE_TARGET(h);
    f32 at[4] __attribute__((aligned(16)));
    s32 tri;

    if (t != NULL) {
        if (!in_his_room(h, t)) {
            return -1;
        }
        tri = HEWIE_TARGET(h)->a.navTri;
        sceVu0CopyVector(at, HEWIE_TARGET(h)->a.pos);
    } else {
        VObject *rooms;
        u8 exit;

        if (HW(h, 0xF366D, u8) != 2 || (exit = HW(h, 0xF3670, u8)) == 0xFF) {
            return -1;
        }
        rooms = gRooms;
        if (!(u8)VCALL(rooms, 0x74, s32 (*)(VObject *, s32, u32))(rooms, h->c.a.room, exit)) {
            return -1;
        }
        tri = VCALL(rooms, 0x34, s32 (*)(VObject *, u32, f32 *))(rooms, exit, at);
    }
    if (!(u8)Actor_CanWalkBetween(h, h->c.a.navTri, tri, h->c.a.pos, at, 0)) {
        s32 r;

        h->c.pathReq->mask = 0x40080;
        r = Hewie_PlanAndGo(h, tri, at, 0, 1);
        h->c.pathReq->mask = h->c.a.navMask;
        if (r == 0) {
            tri = h->c.a.navTri;
            sceVu0CopyVector(at, h->c.a.pos);
            VCALL(gSceneGameF29740, 0x20, s32 (*)(VObject *, u32 *, f32 *, s32, s32, void *, f32))(
                gSceneGameF29740, (u32 *)&tri, at, 0, h->c.unk124, h->c.unk12C, 20.0f);
        }
    }
    sceVu0CopyVector(out, at);
    return tri;
}

/* ---- how far by path ---- */

/* the walking distance from him to c: planned over the mesh (blocked by flags 0x29020008), -1
 * if c isn't in his room, the room isn't the one played on the mesh, or no path */
/* 0x0013F220 */
f32 Hewie_WalkDistance(Hewie *h, Character *c) {
    PathRequest req;
    VObject *planner;
    void *pts;
    s32 n;
    f32 len;

    if (!in_his_room(h, c) || h->c.a.room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return -1.0f;
    }
    req.unk0 = 0;
    req.startTri = h->c.a.navTri;
    sceVu0CopyVector(req.startPos, h->c.a.pos);
    req.goalTri = c->a.navTri;
    sceVu0CopyVector(req.goalPos, c->a.pos);
    req.unk4 = 2;
    req.mask = 0x29020008;
    planner = gSceneGameF29740;
    h->c.pathId = VCALL(planner, 0xC, s32 (*)(VObject *, PathRequest *, s32))(planner, &req, 0);
    if (h->c.pathId == -1) {
        return -1.0f;
    }
    if (VCALL(planner, 0x14, s32 (*)(VObject *))(planner) < 0) {
        VCALL(planner, 0x28, void (*)(VObject *, s32))(planner, h->c.pathId);
        return -1.0f;
    }
    pts = msl_malloc(0x1200);
    planner = gSceneGameF29740;
    n = VCALL(planner, 0x18, s32 (*)(VObject *, s32, void *))(planner, h->c.pathId, pts);
    VCALL(planner, 0x28, void (*)(VObject *, s32))(planner, h->c.pathId);
    len = VCALL(planner, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(planner, h->c.a.pos, 0, n, pts);
    msl_free(pts);
    return len;
}

/* ---- overlay animations by his animation and mood ---- */

/* per main animation, the overlay modes for each mood (+0xF35C0 0..3): head (+0xF3640, for
 * Hewie_OverlayAnim), ears (+0xF3648, Hewie_IdleOverlay) and tail (+0xF3654, Hewie_TailOverlay). -1 ends */
typedef struct HewieOverlays {
    s32 anim;
    s16 head[4];
    s16 ears[4];
    s16 tail[4];
} HewieOverlays;

extern const HewieOverlays D_003B13F0[];   /* calm */
extern const HewieOverlays D_003B1580[];   /* tense */
extern void Hewie_TailOverlay(Hewie *h);

/* set his overlay modes for his animation and mood, restarting a mode's timers when it changes
 * to 3 (head), 0 (ears) or 4 / 6 (tail), and run the overlays */
/* 0x0013FDE0 */
void Hewie_SetOverlays(Hewie *h) {
    const HewieOverlays *e = !(u8)Progress_GameMode(gProgress) ? D_003B13F0 : D_003B1580;
    s32 head = HW(h, 0xF3640, s32), ears = HW(h, 0xF3648, s32), tail = HW(h, 0xF3654, s32);
    s32 m;

    for (; e->anim != -1; e++) {
        if (e->anim == MOTION_ANIM(h->c.motion)) {
            m = HW(h, 0xF35C0, s32);
            if ((u32)m < 4) {
                HW(h, 0xF3640, s32) = e->head[m];
                HW(h, 0xF3648, s32) = e->ears[m];
                HW(h, 0xF3654, s32) = e->tail[m];
            }
            break;
        }
    }
    if (e->anim == -1) {
        return;
    }
    if (HW(h, 0xF3640, s32) != head && HW(h, 0xF3640, s32) == 3) {
        HW(h, 0xF3644, s32) = 0;
    }
    if (HW(h, 0xF3648, s32) != ears && HW(h, 0xF3648, s32) == 0) {
        HW(h, 0xF364C, s32) = 0;
    }
    if (HW(h, 0xF3654, s32) != tail && (HW(h, 0xF3654, s32) == 4 || HW(h, 0xF3654, s32) == 6)) {
        HW(h, 0xF3658, s32) = 0;
        HW(h, 0xF365C, s32) = 0;
    }
    Hewie_OverlayAnim(h);
    Hewie_IdleOverlay(h);
    Hewie_TailOverlay(h);
}

/* ---- tail ---- */

/* one of three tail animations by his animation group: c for groups 2 / 6, b for 1 / 5, else a */
static void tail_play(Hewie *h, s32 g, s32 a, s32 b, s32 c) {
    Motion_PlayTableNoCheck(h->c.motion, g == 6 || g == 2 ? c : g == 5 || g == 1 ? b : a);
}

/* a wag: every so often (+0xF3658 counting down) wag (`wag`) for 10..35 frames, then rest
 * (still, 300 frames) */
static void tail_wag(Hewie *h, s32 g, s32 sa, s32 sb, s32 sc, s32 wa, s32 wb, s32 wc) {
    if (--HW(h, 0xF3658, s32) >= 0) {
        return;
    }
    if (HW(h, 0xF365C, s32) != 0) {
        HW(h, 0xF365C, s32) = 0;
        HW(h, 0xF3658, s32) = 300;
        tail_play(h, g, sa, sb, sc);
    } else {
        HW(h, 0xF365C, s32) = 1;
        HW(h, 0xF3658, s32) = (s32)(6.0f * RNG01()) * 5 + 10;
        tail_play(h, g, wa, wb, wc);
    }
}

/* His tail overlay animation by +0xF3654: 0..3 held poses, 5 / 7 steady wags, 4 / 6 wagging now
 * and then (from pose 2 / 3) */
/* 0x0013F470 */
void Hewie_TailOverlay(Hewie *h) {
    s32 g = Hewie_AnimGroup(h);

    switch (HW(h, 0xF3654, u32)) {
    case 0:
        tail_play(h, g, 0x2107, 0x2108, 0x2109);
        break;
    case 1:
        tail_play(h, g, 0x210A, 0x210B, 0x210C);
        break;
    case 2:
        tail_play(h, g, 0x2100, 0x2103, 0x2106);
        break;
    case 3:
        tail_play(h, g, 0x210D, 0x210E, 0x210F);
        break;
    case 4:
        tail_wag(h, g, 0x2100, 0x2103, 0x2106, 0x2102, 0x2105, 0x2102);
        break;
    case 5:
        tail_play(h, g, 0x2101, 0x2104, 0x2101);
        break;
    case 6:
        tail_wag(h, g, 0x210D, 0x210E, 0x210F, 0x210D, 0x2110, 0x2111);
        break;
    case 7:
        tail_play(h, g, 0x2102, 0x2105, 0x2102);
        break;
    }
}

/* ---- carrying out a command ---- */

extern const s8 D_003B1220[];   /* by trust: chance (percent) he answers 0x30 while she is held */
extern const s8 D_003B1228[];   /* ... at panic 4 */
extern const s8 D_003B1230[];   /* ... at panic 5 */

/* he heard her: stop waiting and obey for a while */
static inline void obeys(Hewie *h) {
    HW(h, 0xF3598, s32) = 0;
    obey_time(h);
    HW(h, 0xF3586, u8) = 0;
}

/* a roll against tbl[trust] percent */
static s32 by_chance(Hewie *h, const s8 *tbl) {
    s32 chance = tbl[HW(h, 0xF35CC, s16)];
    s8 r = (s8)(s32)(100.0f * RNG01());

    if (chance < 0) {
        chance = 0;
    } else if (chance >= 101) {
        chance = 100;
    }
    return r < (s8)chance;
}

/* act on Fiona's command `cmd` (from Hewie_CommandAction); 1 if he took up an action. Hidden (+0x29)
 * he only answers 0x2C (come out: 0x2D, or flag +0xF3559 while +0xF35B4 counts) and 0x30
 * (0x39); 0x2A heals him to 10 */
/* 0x0013D580 */
s32 Hewie_ActOnCommand(Hewie *h, s32 cmd) {
    Progress *p;
    s32 took = 0;

    switch (cmd) {
    case 0x25:
        if (!h->c.a.disabled) {
            took = 1;
            hewie_want(h, 0x1D, 0x29);
            obeys(h);
        }
        break;
    case 0x26:
        if (!h->c.a.disabled) {
            p = gProgress;
            if (!(u8)Progress_GameMode(p)) {
                took = 1;
                hewie_want(h, 0x1D, 0x2B);
            }
            obeys(h);
        }
        break;
    case 0x27:
        if (!h->c.a.disabled) {
            p = gProgress;
            took = 1;
            if (!(u8)Progress_GameMode(p)) {
                s32 a = HW(h, 0xF3564, s32);

                hewie_want(h, 0x1D, a == 3 || a == 2 || a == 1 || a == 5 || a == 4 ? 0x29 : 0x27);
            } else if ((u8)Progress_GameMode(p) == 2) {
                hewie_want(h, 0x1D, 0x7A);
            } else {
                hewie_want(h, 0x1D, 7);
            }
            obeys(h);
        }
        break;
    case 0x2C:
        if (h->c.a.disabled) {
            if (HW(h, 0xF35B4, s32) > 0) {
                HW(h, 0xF35B4, s32) = -1;
                HW(h, 0xF3559, u8) = 1;
                return 0;
            }
            if (HW(h, 0xF3564, s32) != 0x2D) {
                took = 1;
                hewie_want(h, 0x2D, 0);
            }
            break;
        }
        p = gProgress;
        took = 1;
        hewie_want(h, 0x1D, !(u8)Progress_GameMode(p) ? 0xD : 0xE);
        obeys(h);
        break;
    case 0x2D:
        if (!h->c.a.disabled) {
            took = 1;
            hewie_want(h, 0x4E, 0);
            obeys(h);
        }
        break;
    case 0x2A:
        h->c.hp = 10;
        break;
    case 0x23:
        if (!h->c.a.disabled) {
            p = gProgress;
            if (!(u8)Progress_GameMode(p) && HW(h, 0xF368C, s32) == 0) {
                HW(h, 0xF368C, u8 *) = Hewie_NearestThing(h);
                if (HW(h, 0xF368C, s32) != 0) {
                    hewie_want(h, 0x1D, 0x78);
                    return 1;
                }
            }
            /* the spot she points at: state [4] / [5], x / z in 1e-5 units */
            h->c.unk104[0] = h->c.state[4];
            HW(h, 0x10C, s32) = h->c.state[5];
            h->c.unk110[0] = 0x1.4f8b58p-17f /* 1e-5 */ * (f32)h->c.state[2];
            HW(h, 0x114, s32) = 0;
            h->c.unk110[2] = 0x1.4f8b58p-17f /* 1e-5 */ * (f32)h->c.state[3];
            h->c.unk110[3] = 1.0f;
            took = 1;
            if (!(u8)Progress_GameMode(p)) {
                hewie_want(h, 0x1D, 0x63);
            } else {
                hewie_want(h, 0x63, 0);
            }
            obeys(h);
        }
        break;
    case 0x2E:
        if (!h->c.a.disabled) {
            obeys(h);
        }
        break;
    case 0x30:
        if (h->c.a.disabled) {
            HW(h, 0xF35B4, s32) = -1;
            took = 1;
            hewie_want(h, 0x39, 0);
        } else if (AT(gProgress, 0x7B8, u8) == 5) {
            if (by_chance(h, D_003B1230)) {
                took = 1;
                hewie_want(h, 0x4F, 0);
            }
        } else if (AT(gProgress, 0x7B8, u8) == 4) {
            if (by_chance(h, D_003B1228)) {
                took = 1;
                hewie_want(h, 0x4F, 0);
            }
        } else if (gCharPlayer->moveMode == 4 && (gCharPlayer->moveSub == 9 || gCharPlayer->moveSub == 0x12)) {
            if (by_chance(h, D_003B1220)) {
                took = 1;
                hewie_want(h, 0x4F, 0);
            }
        }
        break;
    }
    return took;
}

/* ---- after an action: going for someone ---- */

/* whom to go for after an action (into +0xF3548): 0 if none in reach; 2 the pursuer (while not
 * +0xF3580), for the caller; else 1, a creature (slots 7..9) becoming his target (0x6D) and
 * anyone else 0x5A */
static s32 go_for(Hewie *h) {
    Character *c;

    HW(h, 0xF3548, Character *) = Hewie_PickTarget(h);
    if (!in_his_room(h, HW(h, 0xF3548, Character *))) {
        return 0;
    }
    if (HW(h, 0xF3580, u8) == 0 && HW(h, 0xF3548, Character *) == gCharPursuer) {
        return 2;
    }
    c = HW(h, 0xF3548, Character *);
    if (c != gCharPlayer && c != gCharPursuer && (u32)c->a.slot >= 7 && (u32)c->a.slot < 10) {
        HW(h, 0xF3544, Character *) = c;
        hewie_want(h, 0x6D, 0);
        return 1;
    }
    hewie_want(h, 0x5A, 0);
    return 1;
}

/* after coming out (0x4E): go for someone (the pursuer: 0x21 when +0xF3585 and he can go at
 * it head on, else 0x1F), else 6 (0x87 with progress +0x1FBEC1) */
/* 0x00140190 */
void Hewie_AfterComingOut(Hewie *h) {
    switch (go_for(h)) {
    case 0:
        hewie_want(h, *((u8 *)gProgress + 0x1FBEC1) == 0 ? 6 : 0x87, 0);
        break;
    case 2:
        hewie_want(h, HW(h, 0xF3585, u8) == 1 && Hewie_AttackSide(h) == 0 ? 0x21 : 0x1F, 0);
        break;
    }
}

/* after answering a call (0x4F / 0x50): go for someone (the pursuer: 0x1F after 0x4F, else
 * 0x20), else 6 */
/* 0x001404E0 */
void Hewie_AfterCall(Hewie *h) {
    switch (go_for(h)) {
    case 0:
        hewie_want(h, 6, 0);
        break;
    case 2:
        hewie_want(h, HW(h, 0xF3564, s32) == 0x4F ? 0x1F : 0x20, 0);
        break;
    }
}

/* after 0x4D: by his animation group, a coin toss between 0x28 / 0x26 and 0x2A (moving 1: 0x2A
 * for groups 0 / 4, 0x26 for 2 / 6) */
/* 0x001407C0 */
void Hewie_After4D(Hewie *h) {
    s32 g = Hewie_AnimGroup(h);
    f32 r = RNG01();

    switch (g) {
    case 0:
    case 4:
        hewie_want(h, h->c.a.unkC4 != 1 && r < 0.5f ? 0x28 : 0x2A, 0);
        break;
    case 1:
    case 5:
        hewie_want(h, r < 0.5f ? 0x26 : 0x2A, 0);
        break;
    case 2:
    case 6:
        hewie_want(h, h->c.a.unkC4 == 1 || r < 0.5f ? 0x26 : 0x2A, 0);
        break;
    default:
        hewie_want(h, 0x26, 0);
        break;
    }
}

/* ---- getting into a pose ---- */

extern void Hewie_WalkAnim(Hewie *h);

static inline void pose_play(Hewie *h, s32 anim) {
    Motion_PlayTable(h->c.motion, anim, -1);
}

/* the transition out of animation group g shared by most poses */
static void pose_leave(Hewie *h, s32 g) {
    switch (g) {
    case 3:
        pose_play(h, 0x107);
        break;
    case 7:
        if (ANIM_DONE(h)) {
            pose_play(h, 0x107);
        }
        break;
    case 8:
        if (h->c.a.unkC4 == 1) {
            if (MOTION_ANIM(h->c.motion) != 0x206) {
                pose_play(h, 0x206);
            }
        } else if (MOTION_ANIM(h->c.motion) != 0x201) {
            pose_play(h, 0x201);
        }
        break;
    case 9:
        Hewie_WalkAnim(h);
        break;
    case 10:
    case 15:
        Hewie_StandAnim(h, -1);
        break;
    case 11:
        pose_play(h, 0x301);
        break;
    case 12:
        if (ANIM_DONE(h)) {
            pose_play(h, 0x301);
        }
        break;
    case 13:
        pose_play(h, 0x1003);
        break;
    case 14:
        if (ANIM_DONE(h)) {
            pose_play(h, 0x1003);
        }
        break;
    }
}

/* the transitions of the basic poses for groups 0..2 and 4..6 (the settled group and its
 * entering one): the animation into pose `to` from each, 0 = already there */
static s32 pose_basic(Hewie *h, s32 g, s32 from0, s32 from1, s32 from2) {
    s32 anim = g == 0 || g == 4 ? from0 : g == 1 || g == 5 ? from1 : from2;

    if (g >= 4 && !ANIM_DONE(h)) {
        return -1;
    }
    if (anim == 0) {
        return 0;
    }
    pose_play(h, anim);
    return -1;
}

/* step him toward pose `kind` from his animation group: 0 stand, 1 sit, 2 lie, 3 / 4 the low
 * groups (0..3) ok, 5 most anything still, 6 group 11 (else lie, then 0x300), 7..9 as 5, 10
 * group 13. 0 once there, else -1 (also while blending) */
/* 0x00140CD0 */
s32 Hewie_StepToPose(Hewie *h, u32 kind) {
    s32 g;

    if (!(AT(h->c.motion, 0x550, f32) <= 0.0f)) {
        return -1;
    }
    g = Hewie_AnimGroup(h);
    if (kind >= 11) {
        return -1;
    }
    if (kind >= 7 && kind <= 9) {
        return Hewie_StepToPose(h, 5) == 0 ? 0 : -1;
    }
    if ((u32)g >= 16) {
        return -1;
    }
    switch (kind) {
    case 0:
        if (g <= 2 || (g >= 4 && g <= 6)) {
            return pose_basic(h, g, 0, 0x101, 0x103);
        }
        break;
    case 1:
        if (g <= 2 || (g >= 4 && g <= 6)) {
            return pose_basic(h, g, 0x100, 0, 0x105);
        }
        break;
    case 2:
        if (g <= 2 || (g >= 4 && g <= 6)) {
            return pose_basic(h, g, 0x102, 0x104, 0);
        }
        break;
    case 3:
    case 4:
        if (g <= 2 || (kind == 4 && g == 3)) {
            return 0;
        }
        if (g >= 4 && g <= 6) {
            return ANIM_DONE(h) ? 0 : -1;
        }
        if (kind == 4 && g == 7) {
            if (ANIM_DONE(h)) {
                pose_play(h, 8);
            }
            return -1;
        }
        break;
    case 5:
        if (g <= 2 || (g >= 8 && g <= 11)) {
            return 0;
        }
        if (g >= 4 && g <= 6) {
            return ANIM_DONE(h) ? 0 : -1;
        }
        break;
    case 6:
        if (g == 11) {
            return 0;
        }
        if (g == 12) {
            return ANIM_DONE(h) ? 0 : -1;
        }
        if (Hewie_StepToPose(h, 2) == 0) {
            pose_play(h, 0x300);
        }
        return -1;
    case 10:
        if (g == 13) {
            return 0;
        }
        if (g == 14) {
            return ANIM_DONE(h) ? 0 : -1;
        }
        if (g <= 2 || (g >= 4 && g <= 6)) {
            return pose_basic(h, g, 0x1001, 0x101, 0x103);
        }
        break;
    }
    pose_leave(h, g);
    return -1;
}

/* ---- holding a pose ---- */

/* his walk: 0x206 when moving 1, else `anim` (0x201 / 0x202), unless already playing */
static void pose_walk(Hewie *h, s32 anim) {
    s32 cur = MOTION_ANIM(h->c.motion);

    if (h->c.a.unkC4 == 1) {
        if (cur != 0x206) {
            pose_play(h, 0x206);
        }
    } else if (cur != anim) {
        pose_play(h, anim);
    }
}

/* the settled animation of basic group `base`: standing (Hewie_StandAnim), sitting (1), lying (2,
 * 7 when moving 1), unless already playing */
static void pose_settle(Hewie *h, s32 base) {
    s32 a;

    switch (base) {
    case 0:
        Hewie_StandAnim(h, -1);
        break;
    case 1:
        if (MOTION_ANIM(h->c.motion) != 1) {
            pose_play(h, 1);
        }
        break;
    case 2:
        a = h->c.a.unkC4 == 1 ? 7 : 2;
        if (MOTION_ANIM(h->c.motion) != a) {
            pose_play(h, a);
        }
        break;
    }
}

/* the animations between the basic poses: [to][from] (0 stand, 1 sit, 2 lie; 3 stand from
 * pose 10) */
static const s16 sPoseInto[4][3] = {
    { 0, 0x101, 0x103 },
    { 0x100, 0, 0x105 },
    { 0x102, 0x104, 0 },
    { 0x1001, 0x101, 0x103 },
};

/* keep him in pose `kind` (see Hewie_StepToPose): from a basic group (0..2, or its entering 4..6
 * once done) move toward the pose or play its settled animation (3 / 4 / 5: stay as he is);
 * else leave his group (pose_leave). Not while blending */
/* 0x00141C00 */
void Hewie_KeepPose(Hewie *h, u32 kind) {
    s32 g, base;

    if (!(AT(h->c.motion, 0x550, f32) <= 0.0f)) {
        return;
    }
    g = Hewie_AnimGroup(h);
    switch (kind) {
    case 7:
        if (g == 10 || Hewie_StepToPose(h, 5) == 0) {
            Hewie_WalkAnim(h);
        }
        return;
    case 8:
        if (g == 10 || g == 9 || Hewie_StepToPose(h, 5) == 0) {
            pose_walk(h, 0x201);
        }
        return;
    case 9:
        if (g == 10 || g == 8 || Hewie_StepToPose(h, 5) == 0) {
            pose_walk(h, 0x202);
        }
        return;
    }
    if (kind >= 11 || (u32)g >= 16) {
        return;
    }
    if (kind == 6) {
        if (g == 12) {
            if (ANIM_DONE(h)) {
                pose_play(h, 0x203);
            }
        } else if (g != 11 && Hewie_StepToPose(h, 2) == 0) {
            pose_play(h, 0x300);
        }
        return;
    }
    if (g <= 2 || (g >= 4 && g <= 6)) {
        if (g >= 4 && !ANIM_DONE(h)) {
            return;
        }
        base = g >= 4 ? g - 4 : g;
        if (kind >= 3 && kind <= 5) {
            pose_settle(h, base);
        } else {
            s32 to = kind == 10 ? 3 : kind;

            if (sPoseInto[to][base] == 0) {
                pose_settle(h, base);
            } else {
                pose_play(h, sPoseInto[to][base]);
            }
        }
        return;
    }
    if (kind == 4 && g == 3) {
        return;
    }
    if (kind == 4 && g == 7) {
        if (ANIM_DONE(h)) {
            pose_play(h, 8);
        }
        return;
    }
    if (kind == 5 && g >= 8 && g <= 10) {
        if (g == 10) {
            Hewie_WalkAnim(h);
        } else {
            pose_walk(h, g == 8 ? 0x202 : 0x201);
        }
        return;
    }
    if (kind == 10 && g == 13) {
        return;
    }
    if (kind == 10 && g == 14) {
        if (ANIM_DONE(h)) {
            pose_play(h, 0x1002);
        }
        return;
    }
    pose_leave(h, g);
}

/* ---- barking ---- */

/* play `anim`, or restart it when it is already playing */
static void play_again(Hewie *h, s32 cur, s32 anim) {
    if (cur != anim) {
        Motion_PlayTable(h->c.motion, anim, -1);
    } else {
        Motion_Play(h->c.motion, anim, -1);
    }
}

/* bark (animations 0x1B00..): calm and not in actions 0xA / 0xB, by his pose (standing 0x1B00,
 * sitting 0x1B01, lying 0x1B02; moving 1 0x1B05); else by his mood (2: 0x1B04, else 0x1B03) */
/* 0x001431F0 */
void Hewie_Bark(Hewie *h) {
    s32 cur = MOTION_ANIM(h->c.motion);
    s32 g;

    if (!(u8)Progress_GameMode(gProgress) && HEWIE_ACTION(h) != 0xA && HEWIE_ACTION(h) != 0xB) {
        if (h->c.a.unkC4 == 1) {
            play_again(h, cur, 0x1B05);
            return;
        }
        g = Hewie_AnimGroup(h);
        play_again(h, cur, g == 6 || g == 2 ? 0x1B02 : g == 5 || g == 1 ? 0x1B01 : 0x1B00);
    } else {
        play_again(h, cur, HW(h, 0xF35C0, s32) == 2 ? 0x1B04 : 0x1B03);
    }
}

/* ---- noticing the pursuer ---- */

/* what he is alert to (+0xF366D, with +0xF3670): 1 the pursuer itself when within 200 in front
 * of him (125 degrees either side) with nothing between; 2 an exit of his room leading to the
 * pursuer's room; 3 the noise he heard from slot 2 (the pursuer); else nothing (an exit or the
 * pursuer gone). Each but the last tells the game (Progress_SetCondBit 4) */
/* 0x00143840 */
void Hewie_Alert(Hewie *h) {
    s32 away = 0;

    if (gCharPursuer == NULL || gCharPursuer->a.active != 1 || h->c.a.disabled || h->c.a.unkC4 == 2) {
        HW(h, 0xF366D, u8) = 0;
        return;
    }
    if (in_his_room(h, gCharPursuer) && Actor_Distance(&h->c.a, gCharPursuer->a.pos) <= 200.0f) {
        f32 to = Actor_HeadingTo(&h->c.a, gCharPursuer->a.pos);
        f32 d = Angle_Wrap(to - Angle_Wrap(h->c.a.angle[1] + AT(h->c.motion, 0x858, f32)));

        if (!(d <= 0.0f)) {
        } else {
            d = -d;
        }
        if (d <= 0x1.1740bp+1f /* 125 degrees */ &&
            (u8)Actor_CanWalkBetween(h, h->c.a.navTri, gCharPursuer->a.navTri, h->c.a.pos, gCharPursuer->a.pos, 0) == 1) {
            HW(h, 0xF366D, u8) = 1;
            HW(h, 0xF3670, s32) = gCharPursuer->a.slot;
            Progress_SetCondBit(gProgress, 4);
            return;
        }
    }
    if (gCharPursuer != NULL && gCharPursuer->a.active == 1 && !in_his_room(h, gCharPursuer)) {
        VObject *rooms = gRooms;
        u32 e;

        away = 1;
        for (e = 0; e < 8; e = (e + 1) & 0xFF) {
            if ((u8)VCALL(rooms, 0x74, s32 (*)(VObject *, s32, u32))(rooms, h->c.a.room, e) == 1 &&
                VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, h->c.a.room, e) == gCharPursuer->a.room) {
                HW(h, 0xF366D, u8) = 2;
                HW(h, 0xF3670, s32) = e & 0xFF;
                Progress_SetCondBit(gProgress, 4);
                return;
            }
        }
    }
    if (h->c.heardSlot == 2) {
        HW(h, 0xF366D, u8) = 3;
        HW(h, 0xF3670, s32) = 2;
        Progress_SetCondBit(gProgress, 4);
    } else if (HW(h, 0xF366D, u8) == 2 || away == 1) {
        HW(h, 0xF366D, u8) = 0;
    }
}

/* ---- arriving in the room being played ---- */

#define Hewie_PlaceAt(h, tri, ang, at) VCALL(h, 0x28, s32 (*)(Hewie *, u32, const f32 *, f32 *))(h, tri, ang, at)

/* the animation he arrives in, when none: by how he moves (down 0x1002, 1: 6, else standing) */
static void arrive_anim(Hewie *h) {
    switch (h->c.a.unkC4) {
    case 2:
        HW(h, 0xF36F0, s32) = 0x1002;
        break;
    case 1:
        HW(h, 0xF36F0, s32) = 6;
        break;
    default:
        HW(h, 0xF36F0, s32) = 0;
        break;
    }
}

/* placed in the room: handed to the room objects, at the origin if off the mesh, the sound of
 * his spot, what he is alert to */
static void arrived(Hewie *h) {
    sceVu0FVECTOR v;

    VCALL(gEvents, 0x2C, void (*)(VObject *, Hewie *))(gEvents, h);
    if (h->c.a.navTri == NAV_NONE) {
        h->c.a.pos[0] = 0.0f;
        h->c.a.pos[1] = 0.0f;
        h->c.a.pos[2] = 0.0f;
        h->c.a.pos[3] = 0x1.99999ap-4f;   /* 0.1 */
    }
    if (Actor_PosInCurrentRoom(&h->c.a, v)) {
        Character_Sound(&h->c, 3, (s32)v, 0, 0, 0);
    }
    Hewie_Alert(h);
    HW(h, 0xF366C, u8) = HW(h, 0xF366D, u8);
    Character_MarkObjects(&h->c);
}

static inline void blend_in(Hewie *h) {
    if (HW(h, 0xF3604, s32) != 4) {
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 10;
    }
}

/* he arrives in the room being played: by his action, 0x88 (coming through exit +0xF36B4) put
 * at the exit; the resting / waiting ones (0, 0x2C..0x39, 0x52, 0x77) kept where he is with
 * the animation they had (else anywhere on the room's mesh, or at exit +0x14D4 facing at
 * random), then a fitting action. -1 for any other action, or (but 0x88) another room */
/* 0x00143D20 */
s32 Hewie_Arrive(Hewie *h) {
    VObject *rooms;
    f32 at[4] __attribute__((aligned(16)));
    f32 ang;
    s32 placed, held;
    u32 tri;

    switch (HEWIE_ACTION(h)) {
    case 0x88:
        blend_in(h);
        HW(h, 0xF366D, u8) = 0;
        MOTION_PTR(h->c.motion, 0x858) = NULL;
        MOTION_PTR(h->c.motion, 0x854) = NULL;
        h->c.a.room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
        rooms = gRooms;
        HEWIE_SIDE(h) = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, h->c.a.room,
                                                                              HW(h, 0xF36B4, u8), 0);
        h->c.door = HW(h, 0xF36B4, u8);
        HW(h, 0xF368A, u8) = 0;
        arrive_anim(h);
        tri = VCALL(rooms, 0x34, u32 (*)(VObject *, u32, f32 *))(rooms, HW(h, 0xF36B4, u8), at);
        Hewie_PlaceAt(h, tri, NULL, at);
        if (h->c.a.unkC4 == 2) {
            hewie_want(h, 0x52, 0);
        } else {
            hewie_want(h, HW(h, 0xF35C8, s32) == 0 ? 0 : 0x24, 0);
        }
        arrived(h);
        return 0;
    case 0x00:
    case 0x2C: case 0x2D: case 0x2E: case 0x2F: case 0x30: case 0x31: case 0x32: case 0x33:
    case 0x34: case 0x35: case 0x36: case 0x37: case 0x38: case 0x39:
    case 0x52:
    case 0x77:
        break;
    default:
        return -1;
    }
    if (h->c.a.room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return -1;
    }
    HW(h, 0xF366D, u8) = 0;
    held = 0;
    blend_in(h);
    MOTION_PTR(h->c.motion, 0x858) = NULL;
    MOTION_PTR(h->c.motion, 0x854) = NULL;
    HW(h, 0xF36F0, s32) = -1;
    placed = 0;
    switch (HEWIE_ACTION(h)) {
    case 0x39:
    case 0x2D:
    case 0x2C:
        if (HW(h, 0xF3590, u8) == 1) {
            HW(h, 0xF36F0, s32) = HEWIE_ACTION(h) == 0x39 ? 0x202 : h->c.a.unkC4 == 1 ? 0x206 : 0x201;
            if (h->c.a.navTri != NAV_NONE) {
                /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
                if (NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & h->c.a.navMask) {
                    Actor_TeleportRandom(&h->c.a, HEWIE_SIDE(h));
                } else {
                    VCALL(gNavMesh, 0x14, void (*)(void *, u32, f32 *))(gNavMesh, h->c.a.navTri, h->c.a.pos);
                    Hewie_PlaceAt(h, h->c.a.navTri, &h->c.a.angle[1], h->c.a.pos);
                }
                placed = 1;
            }
        }
        break;
    case 0x2F:
        HW(h, 0xF355C, s32) -= 150;
        if (HW(h, 0xF355C, s32) > 0) {
            if (HW(h, 0xF36B8, s32) == 1) {
                HW(h, 0xF36F0, s32) = h->c.a.unkC4 == 1 ? 7 : 1;
            } else if (HW(h, 0xF36B8, s32) == 2) {
                HW(h, 0xF36F0, s32) = h->c.a.unkC4 == 1 ? 7 : 2;
            }
        } else {
            HW(h, 0xF355C, s32) = 0;
        }
        break;
    case 0x38:
        if (HW(h, 0xF36B8, s32) == 0x20 || HW(h, 0xF36B8, s32) == 0x1F) {
            switch (HW(h, 0xF36BC, s32)) {
            case 1:
                held = 1;
                HW(h, 0xF36F0, s32) = 0x2202;
                break;
            case 2:
                HW(h, 0xF36F0, s32) = 0x220B;
                held = 1;
                break;
            case 0:
                HW(h, 0xF36F0, s32) = 0x2216;
                held = 1;
                break;
            }
        }
        break;
    case 0x77:
        HW(h, 0xF36F0, s32) = HW(h, 0xF35B8, s32);
        break;
    }
    if (!placed) {
        if (HW(h, 0xF36F0, s32) == -1) {
            arrive_anim(h);
        }
        if (h->c.a.navTri != NAV_NONE) {
            if (HEWIE_ACTION(h) == 0x38 && Hewie_PlaceAt(h, h->c.a.navTri, &h->c.a.angle[1], h->c.a.pos) != -1) {
                placed = 1;
            }
            if (!placed && (Hewie_PlaceAt(h, h->c.a.navTri, &h->c.a.angle[1], h->c.a.pos) != -1 ||
                            Hewie_PlaceAt(h, h->c.a.navTri, NULL, NULL) != -1)) {
                placed = 1;
            }
        }
        if (!placed) {
            tri = VCALL(gRooms, 0x34, u32 (*)(VObject *, u32, f32 *))(gRooms, h->c.door, at);
            ang = 2.0f * (0x1.921fb6p+1f /* pi */ * VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom)) -
                  0x1.921fb6p+1f;
            if (tri == NAV_NONE || Hewie_PlaceAt(h, tri, &ang, at) == -1) {
                Actor_TeleportRandom(&h->c.a, HEWIE_SIDE(h));
            }
        }
    }
    if (h->c.a.unkC4 == 2) {
        hewie_want(h, 0x52, 0);
    } else if (held == 1) {
        hewie_want(h, 0x73, 0);
    } else if (HEWIE_ACTION(h) == 0x77) {
        hewie_want(h, 0x76, 0);
    } else if (HEWIE_ACTION(h) == 0x2F && HW(h, 0xF36B4, s32) > 0) {
        HW(h, 0xF3560, s32) = HW(h, 0xF355C, s32);
        hewie_want(h, 3, 0);
    } else {
        hewie_want(h, HW(h, 0xF35C8, s32) == 0 ? 0 : 0x24, 0);
    }
    arrived(h);
    return 0;
}

/* ---- a door to use ---- */

/* the first door of the room he can use (into +0xF36B4): 1 one he can pass that is open (for a
 * plain door) and not locked. Else, stopping at the first door with special access (state bits
 * 4 / 8 / 0x10): 0 when he is at its level (within 5) and within 5 in front of it (kind 0,
 * spot +0xF36E0 from Hewie_DoorPoint), or within 16 of its far point (kind 1, bits 0x10 and 8
 * or doors +0x6C) and that brings him farther from it; -1 none */
/* 0x00144B30 */
s32 Hewie_FirstDoor(Hewie *h) {
    VObject *doors = gDoors;
    Progress *p = gProgress;
    s32 kind = -1;
    u32 e;
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 d2[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));

    for (e = 0; e < 8; e = (e + 1) & 0xFF) {
        u32 k;

        if ((u8)VCALL(doors, 0x40, s32 (*)(VObject *, u32))(doors, e) != 1) {
            continue;
        }
        k = (u8)PursuerGroup_Fields(p, e, (u8)h->c.a.slot);
        if (!(u8)VCALL(doors, 0x30, s32 (*)(VObject *, u32))(doors, e)) {
            if ((k & 1) &&
                (u8)Progress_ExitPassable(p, VCALL(p, 0xC, s32 (*)(Progress *))(p), e, 1) == 1 &&
                !(u8)Progress_ExitUnlocked(p, VCALL(p, 0xC, s32 (*)(Progress *))(p), e)) {
                HW(h, 0xF36B4, s32) = e & 0xFF;
                return 1;
            }
            if (k & 0x10) {
                if ((k & 8) || (u8)VCALL(doors, 0x6C, s32 (*)(VObject *, s32, u32, f32 *))(doors, 3, e, h->c.a.pos) == 1) {
                    kind = 1;
                    break;
                }
            } else if (k & 0xC) {
                kind = 0;
                break;
            }
        } else {
            if ((k & 1) &&
                (u8)Progress_ExitPassable(p, VCALL(p, 0xC, s32 (*)(Progress *))(p), e, 1) == 1 &&
                !(u8)Progress_ExitUnlocked(p, VCALL(p, 0xC, s32 (*)(Progress *))(p), e) &&
                !(u8)Progress_ExitOpen(p, VCALL(p, 0xC, s32 (*)(Progress *))(p), e)) {
                HW(h, 0xF36B4, s32) = e & 0xFF;
                return 1;
            }
            /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
            if ((k & 8) && (NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & 0x20000)) {
                kind = (k & 0x10) ? 1 : 0;
                break;
            }
        }
    }
    switch (kind) {
    case 0:
        VCALL(doors, 0x34, void (*)(VObject *, u32, f32 *))(doors, e, at);
        sceVu0SubVector(d, at, h->c.a.pos);
        if (__builtin_fabsf(d[1]) < 5.0f) {
            f32 m[4][4] __attribute__((aligned(16)));
            f32 dir[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 1.0f, 0.0f };

            Mtx_TurnY(m, VCALL(doors, 0x3C, f32 (*)(VObject *, u32))(doors, e));
            Mtx_ApplyVector(dir, m, dir);
            if (sceVu0InnerProduct(d, dir) < 5.0f) {
                HW(h, 0xF36B4, s32) = e & 0xFF;
                Hewie_DoorPoint(h, e, &HW(h, 0xF36E0, f32));
                return 0;
            }
        }
        break;
    case 1:
        VCALL(doors, 0x38, void (*)(VObject *, u32, f32 *))(doors, e, at);
        sceVu0SubVector(d, at, h->c.a.pos);
        if (__builtin_fabsf(d[1]) < 5.0f && __builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) < 16.0f) {
            Hewie_DoorPoint(h, e, q);
            sceVu0SubVector(d, q, at);
            sceVu0SubVector(d2, h->c.a.pos, at);
            if (!(d[2] * d[2] + d[0] * d[0] <= d2[2] * d2[2] + d2[0] * d2[0])) {
                HW(h, 0xF36B4, s32) = e & 0xFF;
                sceVu0CopyVector(&HW(h, 0xF36E0, f32), q);
                return 0;
            }
        }
        break;
    }
    return -1;
}

/* ---- footsteps ---- */

/* his feet: each foot (motion +0x64, feet 0..3) that touches down this frame leaves a print in
 * rooms 7, 0xD1 and 0x106 (at its bone, deep when walking group 8) and makes a step sound by the
 * floor (+0x14C8 flags: 0x10 plain, 0x14 / 0x18 / 0x1C, 0x78; 0x18 type 6 when the driver says
 * so), louder the harder he steps. Only in the room being played, on the mesh, not while the
 * room objects hold him (+0x50) nor with progress flag 8 */
/* 0x00145080 */
void Hewie_Feet(Hewie *h) {
    static const s32 bones[4] = { 0xB, 0xF, 0x17, 0x1C };
    Progress *p = gProgress;
    u8 down[4];
    s32 steps = 0, i, deep, snd, type, n;
    f32 root[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 f;

    if (h->c.a.room != VCALL(p, 0xC, s32 (*)(Progress *))(p) || h->c.a.navTri == NAV_NONE) {
        return;
    }
    if ((u8)VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 1) {
        return;
    }
    if ((u8)Progress_TestFlag(p, 8) == 1) {
        return;
    }
    for (i = 0; i < 4; i++) {
        down[i] = VCALL(h->c.motion, 0x64, s32 (*)(void *, s32, s32))(h->c.motion, i, -1);
    }
    for (i = 0; i < 4; i++) {
        if (down[i] == 1 && HW(h, 0xF3660 + i, u8) == 0) {
            steps |= 1 << i;
        }
    }
    if (steps != 0 && (h->c.a.room == 7 || h->c.a.room == 0xD1 || h->c.a.room == 0x106)) {
        deep = Hewie_AnimGroup(h) == 8;
        for (i = 0; i < 4; i++) {
            if (steps & (1 << i)) {
                sceVu0CopyVector(at, Skel_Bone(MOTION_SKELETON(h->c.motion), bones[i]) + 12);
                Character_WaterStep(&h->c, at, deep);
            }
        }
    }
    for (i = 0; i < 4; i++) {
        HW(h, 0xF3660 + i, u8) = down[i];
    }
    if (steps == 0) {
        return;
    }
    /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
    snd = 0x10;
    type = 5;
    switch (NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & 0x02018000) {
    case 0x8000:
        snd = 0x14;
        break;
    case 0x10000:
        snd = 0x18;
        break;
    case 0x18000:
        snd = 0x1C;
        break;
    case 0x2000000:
        snd = 0x78;
        break;
    case 0x2008000:
        if ((u8)VCALL(gSound, 0xA4, s32 (*)(VObject *, s32))(gSound, 6) == 1) {
            snd = 0x18;
            type = 6;
        }
        break;
    }
    n = HW(h, 0xF3664, s32)++;
    Motion_RootMovement(h->c.motion, root, 0.0f);
    root[2] *= VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    f = (root[2] - 0x1.1eb852p-2f /* 0.28 */) / 0x1.0f5c2ap+1f /* 2.12 */;
    if (f < 0.0f) {
        f = 0.0f;
    }
    if (!(f <= 1.0f)) {
        f = 1.0f;
    }
    Actor_PlaySound(&h->c.a, snd + (n & 3), type, 0, (s32)((u32)(2.0f * f) & 0x7F), NULL);
}

/* ---- where to be by Fiona ---- */

extern const f32 D_003B1D60[2][4];   /* offsets in front of a door, per side */

/* a free point (not blocked for him) by Fiona: the point p, if its triangle is free */
static s32 free_at(Hewie *h, u32 *tri, f32 *p) {
    *tri = Actor_TriOfOnMesh(&gCharPlayer->a, p);
    /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
    return *tri != NAV_NONE && !(NavMesh_TriFlags(gNavMesh, *tri) & h->c.a.navMask);
}

/* where to go for Fiona's command `cmd` (the point in `out`, its triangle returned): to her
 * when she isn't in his room, his own spot. On another level of the room (mesh flags 0x100000 /
 * 0x200000) the door point on his level nearest her. 0x64: 15 in front of the point she shows
 * (+0x110, heading +0x10C). 0xE / 0xF with the pursuer about: 15 from her toward it (0xF away
 * from it), tried turned 15 degrees either way. Else beside her: +0xF3550 to her side, drifting
 * 0.3 a frame (by +0xF3554) while under 5, else 5 to the other side. Her own spot failing all */
/* 0x00145610 */
u32 Hewie_CommandPlace(Hewie *h, s32 cmd, f32 *out) {
    f32 off[2][4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 best[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    u32 tri, found, mine;
    f32 bestd = 0.0f, side;
    s32 i, j, n;

    if (!in_his_room(h, gCharPlayer)) {
        sceVu0CopyVector(out, h->c.a.pos);
        return h->c.a.navTri;
    }
    if (cmd != 0x64) {
        u32 hers;

        mine = NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & 0x300000;
        hers = NavMesh_TriFlags(gNavMesh, gCharPlayer->a.navTri) & 0x300000;
        if ((mine == 0x100000 && hers == 0x200000) || (mine == 0x200000 && hers == 0x100000)) {
            n = gNavMesh->numDoors;
            found = NAV_NONE;
            for (i = 0; i < 8; i++) {
                off[i >> 2][i & 3] = D_003B1D60[i >> 2][i & 3];
            }
            for (i = 0; i < n; i = (i + 1) & 0xFF) {
                for (j = 0; j < 2; j++) {
                    tri = Actor_DoorFront(h, i & 0xFF, j, off[j], p);
                    if ((NavMesh_TriFlags(gNavMesh, tri) & 0x300000) == mine) {
                        if (found == NAV_NONE) {
                            found = tri;
                            sceVu0CopyVector(best, p);
                            bestd = Actor_Distance(&gCharPlayer->a, p);
                        } else {
                            f32 d = Actor_Distance(&gCharPlayer->a, p);

                            if (!(bestd <= d)) {
                                found = tri;
                                sceVu0CopyVector(best, p);
                                bestd = d;
                            }
                        }
                        break;
                    }
                }
            }
            if (found != NAV_NONE) {
                sceVu0CopyVector(out, best);
                return found;
            }
        }
    }
    switch (cmd) {
    case 0x64:
        Mtx_AtHeading(m, h->c.unk110, HW(h, 0x10C, f32));
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 15.0f;
        Mtx_ApplyPoint(p, m, v);
        if (free_at(h, &tri, p)) {
            sceVu0CopyVector(out, p);
            return tri;
        }
        sceVu0CopyVector(out, gCharPlayer->a.pos);
        return gCharPlayer->a.navTri;
    case 0xE:
    case 0xF:
        if ((HW(h, 0xF366D, u8) == 1 || HW(h, 0xF366D, u8) == 3) && in_his_room(h, gCharPursuer)) {
            if (cmd == 0xE) {
                sceVu0SubVector(v, gCharPursuer->a.pos, gCharPlayer->a.pos);
            } else {
                sceVu0SubVector(v, gCharPlayer->a.pos, gCharPursuer->a.pos);
            }
            sceVu0Normalize(v, v);
            vu0_ScaleXYZ(v, v, 15.0f);
            sceVu0AddVector(p, gCharPlayer->a.pos, v);
            if (free_at(h, &tri, p)) {
                sceVu0CopyVector(out, p);
                return tri;
            }
            /* (the original steps a counter by 15 degrees but always turns by 15) */
            for (i = 15; (f32)i < 90.0f; i = (s32)((f32)i + 15.0f)) {
                Mtx_AtHeading(m, gCharPlayer->a.pos, 0x1.0c1524p-2f /* 15 degrees */);
                Mtx_ApplyPoint(p, m, v);
                if (free_at(h, &tri, p)) {
                    sceVu0CopyVector(out, p);
                    return tri;
                }
                Mtx_AtHeading(m, gCharPlayer->a.pos, -0x1.0c1524p-2f);
                Mtx_ApplyPoint(p, m, v);
                if (free_at(h, &tri, p)) {
                    sceVu0CopyVector(out, p);
                    return tri;
                }
            }
        }
        break;
    }
    side = HW(h, 0xF3550, f32);
    if (__builtin_fabsf(side) < 5.0f) {
        if (HW(h, 0xF3554, s32) & 1) {
            side += 0x1.333334p-2f;   /* 0.3 */
        } else {
            side -= 0x1.333334p-2f;
        }
    }
    v[1] = 0.0f;
    v[0] = side;
    HW(h, 0xF3550, f32) = side;
    v[2] = 0.0f;
    v[3] = 0.0f;
    sceVu0ApplyMatrix(p, h->c.a.rot, v);
    sceVu0AddVector(p, p, gCharPlayer->a.pos);
    if (free_at(h, &tri, p)) {
        sceVu0CopyVector(out, p);
        return tri;
    }
    v[0] = !(side <= 0.0f) ? -5.0f : 5.0f;
    sceVu0ApplyMatrix(p, h->c.a.rot, v);
    sceVu0AddVector(p, p, gCharPlayer->a.pos);
    if (free_at(h, &tri, p)) {
        HW(h, 0xF3554, s32) = side < 0.0f ? 1 : 0;
        HW(h, 0xF3550, f32) = 0.0f;
        sceVu0CopyVector(out, p);
        return tri;
    }
    sceVu0CopyVector(out, gCharPlayer->a.pos);
    return gCharPlayer->a.navTri;
}

/* ---- where he looks ---- */

/* the head (motion +0x60) of character c, or its position without a motion */
static void head_of(Character *c, f32 *out) {
    if (c->motion != NULL) {
        VCALL(c->motion, 0x60, void (*)(void *, f32 *))(c->motion, out);
    } else {
        sceVu0CopyVector(out, c->a.pos);
    }
}

/* turn his head: at character slot +0xF3610 (forgotten when it leaves his room), else unless
 * +0xF35E0 by look mode +0xF3600 (changing to +0xF3604 once the +0xF3608 delay runs out): 0 / 5
 * his target (5 level; mode 4 once it is gone), 1 / 2 / 3 glancing about at random, 8 held, 4
 * ahead, 6 / 7 / 9 / 10 / 11 / 12 / 13 fixed poses; with +0xF35E0 the point +0xF35F0. Down:
 * ahead. Pitch kept to -45..135 degrees and yaw to +-162, turned at 0.15 of the way (yaw
 * faster by how much his heading changed since +0xF354C for modes 0 / 5 / 8) */
/* 0x00146130 */
void Hewie_TurnHead(Hewie *h) {
    f32 at[4] __attribute__((aligned(16)));
    f32 pitch, yaw, dp, dy, ys;
    VObject *rng;
    u32 mode;

    if (h->c.a.unkC4 == 2) {
        pitch = 0.0f;
        yaw = 0.0f;
    } else if (HW(h, 0xF3610, s32) != 0xFF) {
        Character *c = gCharacters[HW(h, 0xF3610, s32)];

        if (in_his_room(h, c)) {
            head_of(c, at);
            Motion_LookAt(h->c.motion, at, &pitch, &yaw);
        } else {
            pitch = 0.0f;
            yaw = 0.0f;
            HW(h, 0xF3610, s32) = 0xFF;
        }
    } else if (HW(h, 0xF35E0, u8) == 0) {
        sceVu0CopyVector(at, Skel_Bone(MOTION_SKELETON(h->c.motion), 0x1F) + 12);
        if (Actor_TriTo(&h->c.a, at, NAV_NONE) != NAV_NONE) {
            HW(h, 0xF361C, f32) = AT(h->c.motion, 0x858, f32);
        }
        if (HW(h, 0xF3608, s32) == 0 && HW(h, 0xF3600, u32) != HW(h, 0xF3604, u32)) {
            HW(h, 0xF3600, u32) = HW(h, 0xF3604, u32);
            HW(h, 0xF360C, s32) = 0;
        } else if (HW(h, 0xF3608, s32) != 0) {
            HW(h, 0xF3608, s32) -= 1;
        }
        mode = HW(h, 0xF3600, u32);
        switch (mode) {
        case 0:
        case 5:
            if (in_his_room(h, HW(h, 0xF3544, Character *))) {
                head_of(HW(h, 0xF3544, Character *), at);
                Motion_LookAt(h->c.motion, at, &pitch, &yaw);
                if (HW(h, 0xF3600, u32) == 5) {
                    pitch = 0.0f;
                }
            } else {
                HW(h, 0xF3600, u32) = 4;
                HW(h, 0xF360C, s32) = 0;
                pitch = 0.0f;
                yaw = 0.0f;
            }
            break;
        case 8:
            pitch = HW(h, 0xF3614, f32);
            yaw = HW(h, 0xF3618, f32);
            break;
        case 1:
        case 2:
            if (HW(h, 0xF360C, s32) == 0) {
                f32 r;

                rng = gRandom;
                HW(h, 0xF360C, s32) = (s32)((mode == 1 ? 90.0f : 60.0f) * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng)) + 20;
                HW(h, 0xF3614, f32) = 0x1.921fb60000000p+1f /* 3.1415927 */ * (0x1.99999a0000000p-4f /* 0.1 */ * VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0x1.99999a0000000p-5f /* 0.05 */);
                if (AT(h->c.motion, 0x858, f32) < 0.0f) {
                    r = 0x1.99999a0000000p-1f /* 0.8 */ * VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0x1.3333340000000p-2f /* 0.3 */;
                    HW(h, 0xF3618, f32) = AT(h->c.motion, 0x858, f32) + 0x1.921fb60000000p+1f /* 3.1415927 */ * r;
                } else {
                    r = 0x1.99999a0000000p-1f /* 0.8 */ * VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0x1.3333340000000p-2f /* 0.3 */;
                    HW(h, 0xF3618, f32) = AT(h->c.motion, 0x858, f32) - 0x1.921fb60000000p+1f /* 3.1415927 */ * r;
                }
            } else {
                HW(h, 0xF360C, s32) -= 1;
            }
            pitch = HW(h, 0xF3614, f32);
            yaw = HW(h, 0xF3618, f32);
            break;
        case 3:
            if (HW(h, 0xF360C, s32) != 0) {
                HW(h, 0xF360C, s32) -= 1;
            } else {
                rng = gRandom;
                HW(h, 0xF360C, s32) = (s32)(128.0f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng)) + 20;
                HW(h, 0xF3614, f32) = 0x1.921fb60000000p+1f /* 3.1415927 */ * (-0x1.99999a0000000p-3f /* 0.2 */ * VCALL(rng, 0x18, f32 (*)(VObject *))(rng));
                HW(h, 0xF3618, f32) = 0x1.921fb60000000p+1f /* 3.1415927 */ * (0x1.99999a0000000p-3f /* 0.2 */ * VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0x1.99999a0000000p-4f /* 0.1 */);
            }
            pitch = HW(h, 0xF3614, f32);
            yaw = HW(h, 0xF3618, f32);
            break;
        case 6:
            pitch = 0.0f;
            yaw = -0x1.921fb60000000p+0f /* 1.5707964 */;
            break;
        case 7:
            pitch = 0.0f;
            yaw = 0x1.921fb60000000p+0f /* 1.5707964 */;
            break;
        case 4:
            pitch = 0.0f;
            yaw = 0.0f;
            break;
        case 9:
            pitch = 0.0f;
            yaw = -0x1.2d97c80000000p+1f /* 2.3561945 */;
            break;
        case 10:
            pitch = 0.0f;
            yaw = 0x1.2d97c80000000p+1f /* 2.3561945 */;
            break;
        case 11:
            yaw = 0.0f;
            pitch = 0x1.e28c760000000p-1f /* 0.9424779 */;
            break;
        case 13:
            pitch = 0x1.3333340000000p-2f /* 0.3 */;
            yaw = 0x1.41b2f80000000p+0f /* 1.2566371 */;
            break;
        case 12:
            pitch = 0x1.3333340000000p-2f /* 0.3 */;
            yaw = -0x1.41b2f80000000p+0f /* 1.2566371 */;
            break;
        }
    } else {
        Motion_LookAt(h->c.motion, &HW(h, 0xF35F0, f32), &pitch, &yaw);
    }
    if (!(pitch <= 0x1.2d97c80000000p+1f /* 2.3561945 */)) {
        pitch = 0x1.2d97c80000000p+1f /* 2.3561945 */;
    }
    if (pitch < -0x1.921fb60000000p-1f /* 0.7853982 */) {
        pitch = -0x1.921fb60000000p-1f /* 0.7853982 */;
    }
    if (!(yaw <= 0x1.69e9560000000p+1f /* 2.8274333 */)) {
        yaw = 0x1.69e9560000000p+1f /* 2.8274333 */;
    }
    if (yaw < -0x1.69e9560000000p+1f /* 2.8274333 */) {
        yaw = -0x1.69e9560000000p+1f /* 2.8274333 */;
    }
    /* (abs as the original has it: 0 becomes -0) */
    dp = pitch - AT(h->c.motion, 0x854, f32);
    dy = yaw - AT(h->c.motion, 0x858, f32);
    if (dp <= 0.0f) {
        dp = -dp;
    }
    if (dy <= 0.0f) {
        dy = -dy;
    }
    ys = 0x1.333334p-3f /* 0.15 */ * dy;
    mode = HW(h, 0xF3600, u32);
    if (mode == 8 || mode == 5 || mode == 0) {
        if (!(Angle_Wrap(h->c.a.angle[1] - HW(h, 0xF354C, f32)) <= 0.0f)) {
            ys += Angle_Wrap(h->c.a.angle[1] - HW(h, 0xF354C, f32));
        } else {
            ys += -Angle_Wrap(h->c.a.angle[1] - HW(h, 0xF354C, f32));
        }
    }
    Motion_EaseTilt(h->c.motion, pitch, yaw, 0x1.3333340000000p-3f /* 0.15 */ * dp, ys);
}

/* ---- telling Fiona ---- */

/* post event (kind, a, b) into c's state block unless it holds 7 (the original copies a local
 * whose last fields are never set) */
static void post_state(Character *c, s32 kind, s32 a, s32 b) {
    if (c->state[0] != 7) {
        c->state[0] = kind;
        c->state[1] = a;
        c->state[2] = b;
        c->state[3] = 0;
        c->state[4] = 0;
        *(f32 *)&c->state[5] = 0.0f;
        c->state[6] = 0;
        c->state[7] = 0;
    }
}

/* look at his target (or ahead without one); once he is down low (pose 3): when Fiona is within
 * 100 in his room, tell her (event 0xD, 0, exit) of each exit whose door he may use (state bit
 * 4) and that isn't open; else tell her where he is (0xD, 1, triangle). Then the default
 * action */
/* 0x00146C50 */
void Hewie_StateLookLow(Hewie *h) {
    s32 told = 0;
    u32 e;

    if (HW(h, 0xF3544, Character *) == NULL) {
        if (HW(h, 0xF3604, s32) != 4) {
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 10;
        }
    } else if (HW(h, 0xF3604, s32) != 0) {
        HW(h, 0xF3604, s32) = 0;
        HW(h, 0xF3608, s32) = 10;
    }
    if (Hewie_StepToPose(h, 3) != 0) {
        return;
    }
    Hewie_KeepPose(h, 3);
    if (in_his_room(h, gCharPlayer) && Actor_Distance(&h->c.a, gCharPlayer->a.pos) < 100.0f) {
        Progress *p = gProgress;

        for (e = 0; e < 8; e = (e + 1) & 0xFF) {
            if ((PursuerGroup_Fields(p, e, (u8)h->c.a.slot) & 0xFF & 4) && !(u8)Progress_ExitOpen(p, h->c.a.room, e)) {
                told = 1;
                post_state(gCharPlayer, 0xD, 0, e & 0xFF);
            }
        }
    }
    if (!told) {
        post_state(gCharPlayer, 0xD, 1, h->c.a.navTri);
    }
    hewie_want(h, 0, 0);
}

/* ---- barking at the pursuer ---- */

extern const PTMF Hewie_StateLookLow_ptmf, Hewie_StateLookLow_ptmf2;

/* the pursuer in his room makes noise for the game unless progress flags 0x13 / 0x2B (the
 * original reads the difficulty and leaves the frame count unset) */
static void bark_noise(Hewie *h, Progress *p) {
    if (in_his_room(h, gCharPursuer)) {
        Progress *q = gProgress;

        if (((u8)Progress_TestFlag(q, 0x13) | (u8)Progress_TestFlag(q, 0x2B)) == 0) {
            Progress_GetVar(p, 0x27);
            Relation_Request(p, AT(h, 0x20, u8), 4, 6, 0, 0, 0.0f);
        }
    }
}

/* bark: look at his target (else ahead); calm and not moving 1, lying down low (pose 3) first,
 * then behaviour Hewie_StateLookLow_ptmf2; else standing (pose 0), then Hewie_StateLookLow_ptmf */
/* 0x001470C0 */
void Hewie_StateBark(Hewie *h) {
    Progress *p;

    if (HW(h, 0xF3544, Character *) == NULL) {
        if (HW(h, 0xF3604, s32) != 4) {
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 10;
        }
    } else if (HW(h, 0xF3604, s32) != 0) {
        HW(h, 0xF3604, s32) = 0;
        HW(h, 0xF3608, s32) = 10;
    }
    p = gProgress;
    if (!(u8)Progress_GameMode(p) && h->c.a.unkC4 != 1) {
        if (Hewie_StepToPose(h, 3) == 0) {
            bark_noise(h, p);
            Hewie_Bark(h);
            Hewie_SetBehaviour(h, &Hewie_StateLookLow_ptmf2);
        }
    } else if (Hewie_StepToPose(h, 0) == 0) {
        bark_noise(h, p);
        Hewie_Bark(h);
        Hewie_SetBehaviour(h, &Hewie_StateLookLow_ptmf);
    }
}

/* ---- turning toward a heading ---- */

/* turn toward heading `a` by `step` (Actor_TurnToward, which returns how far is left); more than 30
 * degrees off with his head turned the other way, his body snaps `step` toward it instead */
static f32 turn_toward(Hewie *h, f32 a, f32 step) {
    f32 d = Angle_Wrap(a - h->c.a.angle[1]), ang;

    if ((d <= 0.0f ? -d : d) < 0x1.0c1524p-1f /* 30 degrees */ || !(AT(h->c.motion, 0x858, f32) * d < 0.0f)) {
        return Actor_TurnToward(&h->c.a, a, step);
    }
    ang = d < 0.0f ? Angle_Wrap(h->c.a.angle[1] + step) : Angle_Wrap(h->c.a.angle[1] - step);
    h->c.a.angle[1] = ang;
    sceVu0UnitMatrix(h->c.a.rot);
    sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, ang);
    d = Angle_Wrap(a - h->c.a.angle[1]);
    return d <= 0.0f ? -d : d;
}

/* the speed he turns at while running: by how fast he runs and how far his head is turned */
static f32 run_turn(Hewie *h) {
    f32 root[4] __attribute__((aligned(16)));
    f32 speed, turn = 0.0f, yaw;

    Motion_RootMovement(h->c.motion, root, 0.0f);
    root[2] *= VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    speed = root[2];
    if (!(speed < 0.0f)) {
        yaw = AT(h->c.motion, 0x858, f32);
        if (yaw <= 0.0f) {
            yaw = -yaw;
        }
        turn = speed * (12.0f * (0.5f * yaw));
    }
    return 0x1.921fb60000000p+1f /* 3.1415927 */ * turn / 180.0f;
}

/* ---- jumping ---- */

extern const PTMF Hewie_StateTackle_ptmf2;

/* run up to a jump toward heading +0xF36D0: in his run (0x202, after pose 5), head held level
 * toward it, turning by how fast he runs and how far his head is turned. Once running and not
 * blending, on ground that allows it (mesh flag 1 stops him: default action), take off: aim 5
 * up and 20 ahead (+0xF36E0), +0xF36B4..+0xF36CC the flight (8 frames, speed 2.8 from his
 * height), facing within 30 degrees of the heading (snapping by 30 when his head turns the
 * other way), animation 0x1E01, sound 0x68, then behaviour Hewie_StateTackle_ptmf2 */
/* 0x001476C0 */
void Hewie_StateRunUpJump(Hewie *h) {
    s32 anim = MOTION_ANIM(h->c.motion);
    f32 v[4] __attribute__((aligned(16)));

    if (Hewie_StepToPose(h, 5) == 0 && anim != 0x202) {
        Motion_PlayTable(h->c.motion, 0x202, -1);
    }
    HW(h, 0xF3604, s32) = 8;
    HW(h, 0xF3608, s32) = 0;
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF36D0, f32) - h->c.a.angle[1]);
    Actor_TurnToward(&h->c.a, HW(h, 0xF36D0, f32), run_turn(h));
    if (!(AT(h->c.motion, 0x550, f32) <= 0.0f) || anim != 0x202) {
        return;
    }
    /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
    if (NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & 1) {
        hewie_want(h, 0, 0);
        return;
    }
    v[0] = 0.0f;
    v[1] = 5.0f;
    v[2] = 20.0f;
    v[3] = 0.0f;
    sceVu0ApplyMatrix(v, h->c.a.rot, v);
    if (HW(h, 0xF3604, s32) != 4) {
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 10;
    }
    sceVu0CopyVector(&HW(h, 0xF36E0, f32), h->c.a.pos);
    sceVu0AddVector(&HW(h, 0xF36E0, f32), &HW(h, 0xF36E0, f32), v);
    HW(h, 0xF36B4, s32) = 1;
    HW(h, 0xF36B8, s32) = (s32)0x1.c924920000000p+2f /* 7.142857 */ + 1;
    HW(h, 0xF36BC, s32) = 0;
    HW(h, 0xF36C4, f32) = 0x1.6666660000000p+1f /* 2.8 */;
    HW(h, 0xF36C8, f32) = h->c.a.pos[1];
    HW(h, 0xF36CC, s32) = 0;
    turn_toward(h, HW(h, 0xF36D0, f32), 0x1.0c1524p-1f /* 30 degrees */);
    Motion_PlayTable(h->c.motion, 0x1E01, -1);
    HW(h, 0xF356C, u32) |= 0x80000000;
    h->c.a.unk2D = 1;
    Hewie_MakeSound(h, 0x68);
    h->c.unk104[0] = 0;
    h->c.unk104[1] = 0;
    Hewie_SetBehaviour(h, &Hewie_StateTackle_ptmf2);
}

/* ---- under her control: running ---- */

/* steered by Fiona (Hewie_LeftStick): stopped (+0xF3718), stand ahead; else run (fast with
 * +0xF3744) with his head toward the way: turning on the spot toward +0xF37A0 (mode 2,
 * speeding up by 0.075 degrees a frame to 3, until done) or along the stick (+0xF3700 /
 * +0xF3708) */
/* 0x00147B90 */
void Hewie_StateSteered(Hewie *h) {
    f32 a, step;

    Hewie_LeftStick(h);
    if (HW(h, 0xF3718, s32) != 0) {
        if (HW(h, 0xF3604, s32) != 4) {
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 10;
        }
        Hewie_KeepPose(h, 4);
        return;
    }
    if (HW(h, 0xF3710, u8) == 2) {
        if (HW(h, 0xF3604, s32) != 8) {
            HW(h, 0xF3604, s32) = 8;
            HW(h, 0xF3608, s32) = 10;
        }
        HW(h, 0xF3614, f32) = 0.0f;
        HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF37A0, f32) - h->c.a.angle[1]);
        HW(h, 0xF379C, f32) += 0x1.57254e0000000p-10f /* 0.001308997 */;
        if (!(HW(h, 0xF379C, f32) <= 0x1.aceea00000000p-5f /* 0.05235988 */)) {
            HW(h, 0xF379C, f32) = 0x1.aceea00000000p-5f /* 0.05235988 */;
        }
        if (turn_toward(h, HW(h, 0xF37A0, f32), HW(h, 0xF379C, f32)) < HW(h, 0xF379C, f32)) {
            HW(h, 0xF3710, u8) = 0;
        }
    } else {
        a = msl_atan2f(HW(h, 0xF3700, f32), HW(h, 0xF3708, f32));
        if (HW(h, 0xF3604, s32) != 8) {
            HW(h, 0xF3604, s32) = 8;
            HW(h, 0xF3608, s32) = 10;
        }
        HW(h, 0xF3614, f32) = 0.0f;
        HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
        step = run_turn(h);
        turn_toward(h, a, step);
    }
    Hewie_KeepPose(h, HW(h, 0xF3744, u8) == 1 ? 9 : 8);
}

/* ---- keeping his distance ---- */

/* actions 0x53..0x57 / 0x7C: keep within +0xF36C8 of a point (his target for 0x53..0x55 / 0x7C
 * while it is in his room and reachable, else the default action; +0xF36E0 for 0x56 / 0x57),
 * moving along heading +0xF36C4: picked anew (away from the point, the freest way within 30..150
 * degrees, Hewie_BestHeading) when +0xF36B4 runs out or the way ahead is shorter than the range
 * (0x55 / 0x57 then hold it 30..90 frames, +0xF36B8 30 frames before rechecking). Running by
 * the way ahead (stop under 10, trot under 20) for 0x53 / 0x54 / 0x56 / 0x7C, pose 7 for 0x55
 * / 0x57. Out of range: action +0xF3570 */
/* 0x001480C0 */
void Hewie_StateKeepNear(Hewie *h) {
    f32 at[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 d[4] __attribute__((aligned(16)));
    f32 room, step, a;
    s32 anim;

    if (HW(h, 0xF3560, s32) != 0) {
        HW(h, 0xF3560, s32) -= 1;
    }
    switch (HEWIE_ACTION(h)) {
    case 0x56:
    case 0x57:
        sceVu0CopyVector(at, &HW(h, 0xF36E0, f32));
        break;
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x7C:
        if (!in_his_room(h, HW(h, 0xF3544, Character *)) ||
            !(u8)Hewie_PlanTo(h, HW(h, 0xF3544, Character *)->a.navTri, HW(h, 0xF3544, Character *)->a.pos)) {
            hewie_want(h, 0, 0);
            return;
        }
        sceVu0CopyVector(at, HW(h, 0xF3544, Character *)->a.pos);
        break;
    }
    if (!(Actor_Distance(&h->c.a, at) <= HW(h, 0xF36C8, f32))) {
        hewie_want(h, HW(h, 0xF3570, s32), 0);
        return;
    }
    room = Actor_FreeDistance(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, HW(h, 0xF36C4, f32), HW(h, 0xF36C8, f32));
    if (HW(h, 0xF36B8, s32) != 0) {
        HW(h, 0xF36B8, s32) -= 1;
    } else {
        if (HW(h, 0xF36B4, s32) != 0) {
            HW(h, 0xF36B4, s32) -= 1;
        }
        if (HW(h, 0xF36B4, s32) == 0 || room < HW(h, 0xF36C8, f32)) {
            switch (HEWIE_ACTION(h)) {
            case 0x7C:
            case 0x56:
            case 0x54:
            case 0x53:
                HW(h, 0xF36B4, s32) = 0;
                break;
            case 0x57:
            case 0x55:
                HW(h, 0xF36B4, s32) = (s32)(3.0f * RNG01()) * 30 + 30;
                HW(h, 0xF36B8, s32) = 30;
                break;
            }
            sceVu0SubVector(d, h->c.a.pos, at);
            HW(h, 0xF36C4, f32) = msl_atan2f(d[0], d[2]);
            HW(h, 0xF36C4, f32) = Hewie_BestHeading(h, HW(h, 0xF36C4, f32), 10.0f + HW(h, 0xF36C8, f32), 30, 150, 30);
        }
    }
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
    step = run_turn(h);
    a = HW(h, 0xF36C4, f32);
    turn_toward(h, a, step);
    switch (HEWIE_ACTION(h)) {
    case 0x7C:
    case 0x56:
    case 0x54:
    case 0x53:
        if (Hewie_StepToPose(h, 5) == 0) {
            anim = MOTION_ANIM(h->c.motion);
            if (room < 10.0f) {
                if (anim != 0x200) {
                    Motion_PlayTable(h->c.motion, 0x200, -1);
                }
            } else if (room < 20.0f) {
                if (anim != 0x201) {
                    Motion_PlayTable(h->c.motion, 0x201, -1);
                }
            } else if (anim != 0x202) {
                Motion_PlayTable(h->c.motion, 0x202, -1);
            }
        }
        break;
    case 0x57:
    case 0x55:
        Hewie_KeepPose(h, 7);
        break;
    }
}

/* ---- knocked down ---- */

extern const PTMF Hewie_State1D18_ptmf;

/* knocked down (pose 10): out of time (+0xF355C) he is left at 1 health, moving 1. Hidden
 * (+0x29): following his path, back to the default action unless down. Else, once down, in a
 * progress state that allows it (+0x1FBEC1, or bit 0x8000 of +0x30) and with no cutscene
 * (gCamDirector +0x38) nor room objects holding him (+0x50) nor progress flag 0x2C, request
 * +0x73EB00 (1, or 3 outside +0x1FBEC1) and set flag 0xC; otherwise left at 1 health. Slides
 * with the root motion (navigation mask 0x80001 lifted); while he has health, behaviour
 * Hewie_State1D18_ptmf */
/* 0x001489D0 */
void Hewie_StateKnockedDown(Hewie *h) {
    f32 root[4] __attribute__((aligned(16)));
    f32 k;

    if (HW(h, 0xF355C, s32) == 0) {
        h->c.hp = 1;
        h->c.a.unkC4 = 1;
    }
    if (h->c.a.disabled) {
        h->c.moveMode = 6;
        h->c.unk1388 = h->c.unk1384;
        if (h->c.hp != 0 && h->c.a.unkC4 != 2) {
            hewie_want(h, 0, 0);
        }
        return;
    }
    h->c.moveMode = 0;
    if (HW(h, 0xF3604, s32) != 4) {
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 10;
    }
    if (Hewie_StepToPose(h, 10) == 0) {
        Progress *p = gProgress;

        if (*((u8 *)p + 0x1FBEC1) == 1 || (AT(p, 0x30, u32) & 0x8000) != 0) {
            if (!(u8)VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) &&
                !(u8)VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) && !(u8)Progress_TestFlag(p, 0x2C)) {
                AT(p, 0x73EB00, u8) = *((u8 *)p + 0x1FBEC1) == 1 ? 1 : 3;
                Progress_SetFlag(p, 0xC);
            } else {
                h->c.hp = 1;
                h->c.a.unkC4 = 1;
            }
        }
        if (MOTION_ANIM(h->c.motion) == 0x1002) {
            h->c.a.unk2D = 0;
        } else {
            Hewie_KeepPose(h, 10);
        }
    }
    h->c.a.navMask |= 0x80001;
    Motion_RootMovement(h->c.motion, root, 0.0f);
    k = VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    root[1] = 0.0f;
    root[2] *= k;
    sceVu0ApplyMatrix(root, h->c.a.rot, root);
    Actor_Move(&h->c.a, root);
    HW(h, 0xF3558, u8) = 1;
    h->c.a.navMask &= 0xFFF7FFFE;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
    if (h->c.hp != 0) {
        h->c.a.unk2D = 1;
        Hewie_SetBehaviour(h, &Hewie_State1D18_ptmf);
    }
}

/* ---- being petted, praised and scolded ---- */

/* at the end of each animation of actions 0x49 / 0x4A / 0x4B. 0x49 (scolded close up, Fiona's
 * 0x2B): scolded again within 600 frames, the third time in mood 0 upsets him (+20 to the
 * mistreatment count +0xFB6, mode 2);
 * else after a scolding (+0xF36B4 -6) his mode and mood reset. 0x4A (petted, animations
 * 0x1D00..0x1D02, held a while longer when Fiona keeps commanding 3 while calm): every 4
 * strokes +5 health; at full health (-1 from +0xFB6 unless +0xF36BC) mode 1 and action 0x1D.
 * 0x4B: +0xF3688 300 frames, then action 2 (0 while waiting) */
/* 0x00148D00 */
void Hewie_StatePraised(Hewie *h) {
    Progress *p = gProgress;
    s32 upset;

    if (!(u8)Progress_GameMode(p) && HEWIE_ACTION(h) == 0x4A && AT(gCharPlayer, 0x1AD6B8, s32) == 3 &&
        MOTION_ANIM(h->c.motion) == 0x1D01) {
        HW(h, 0xF36B8, s32) = 2;
    }
    if (!ANIM_DONE(h)) {
        return;
    }
    switch (HEWIE_ACTION(h)) {
    case 0x49:
        upset = 0;
        if (HW(h, 0xF3686, s16) > 0) {
            HW(h, 0xF3684, s16) += 1;
            if (HW(h, 0xF3684, s16) >= 3 && HW(h, 0xF35C0, s32) == 0) {
                Progress_AddCounter(p, 0xFB6, 20);
                Hewie_SetMode(h, 2, -1);
                upset = 1;
                HW(h, 0xF3684, s16) = 0;
            } else {
                HW(h, 0xF3686, s16) = 600;
            }
        } else {
            HW(h, 0xF3684, s16) = 1;
            HW(h, 0xF3686, s16) = 600;
        }
        if (!upset && HW(h, 0xF36B4, s32) != -7 && HW(h, 0xF36B4, s32) == -6) {
            Hewie_SetMode(h, 0, -1);
            HW(h, 0xF35C4, s32) = 0;
            HW(h, 0xF35C8, s32) = 0;
        }
        hewie_want(h, 0, 0);
        break;
    case 0x4A:
        switch (MOTION_ANIM(h->c.motion)) {
        case 0x1D00:
            HW(h, 0xF36B8, s32) = 3;
            HW(h, 0xF36C0, s32) = 3;
            Motion_Play(h->c.motion, 0x1D01, -1);
            break;
        case 0x1D01:
            HW(h, 0xF36B8, s32) -= 1;
            HW(h, 0xF36C0, s32) -= 1;
            if (HW(h, 0xF36C0, s32) == 0) {
                HW(h, 0xF36C0, s32) = 4;
                h->c.hp += 5;
                if (h->c.hp >= h->c.hpMax) {
                    h->c.hp = h->c.hpMax;
                }
            }
            Motion_Play(h->c.motion, HW(h, 0xF36B8, s32) == 0 ? 0x1D02 : 0x1D01, -1);
            break;
        case 0x1D02:
            if (h->c.hp == h->c.hpMax) {
                HW(h, 0xF36B4, s32) = -8;
                if (HW(h, 0xF36BC, s32) == 0) {
                    Progress_AddCounter(p, 0xFB6, -1);
                }
            }
            if (HW(h, 0xF36B4, s32) == -8) {
                Hewie_SetMode(h, 1, -1);
                hewie_want(h, 0x1D, 0);
            } else {
                hewie_want(h, 0, 0);
            }
            break;
        }
        break;
    case 0x4B:
        HW(h, 0xF3688, s16) = 300;
        hewie_want(h, HW(h, 0xF3598, s32) == 0 ? 2 : 0, 0);
        break;
    }
}

/* ---- milling about a spot ---- */

extern const PTMF Hewie_StateHeadForSpot_ptmf;

/* wander within 15 of the spot +0x110 (pose 7, head toward the way): every 30..90 frames, or
 * when the way ahead is under 20, a new heading (outward from the spot, the freest within
 * 30..150 degrees for 30), rechecked after 30 frames. Farther off: behaviour Hewie_StateHeadForSpot_ptmf */
/* 0x001495B0 */
void Hewie_StateWander(Hewie *h) {
    f32 d[4] __attribute__((aligned(16)));
    f32 step;

    sceVu0SubVector(d, h->c.unk110, h->c.a.pos);
    if (!(__builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) <= 15.0f)) {
        Hewie_SetBehaviour(h, &Hewie_StateHeadForSpot_ptmf);
        return;
    }
    if (HW(h, 0xF36B8, s32) != 0) {
        HW(h, 0xF36B8, s32) -= 1;
    } else {
        f32 room;

        if (HW(h, 0xF36B4, s32) != 0) {
            HW(h, 0xF36B4, s32) -= 1;
        }
        room = Actor_FreeDistance(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, HW(h, 0xF36C4, f32), 20.0f);
        if (HW(h, 0xF36B4, s32) == 0 || room < 20.0f) {
            HW(h, 0xF36B4, s32) = (s32)(3.0f * RNG01()) * 30 + 30;
            HW(h, 0xF36B8, s32) = 30;
            sceVu0SubVector(d, h->c.a.pos, h->c.unk110);
            HW(h, 0xF36C4, f32) = msl_atan2f(d[0], d[2]);
            HW(h, 0xF36C4, f32) = Hewie_BestHeading(h, HW(h, 0xF36C4, f32), 30.0f, 30, 150, 30);
        }
    }
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
    step = run_turn(h);
    turn_toward(h, HW(h, 0xF36C4, f32), step);
    Hewie_KeepPose(h, 7);
}

/* ---- going to a spot ---- */

extern const PTMF Hewie_StateWander_ptmf;

/* head for the spot +0x110 (pose 7, head on it): within 10, keep his heading and behaviour
 * Hewie_StateWander_ptmf; facing within 60 degrees of it, stand and flag +0xE1 */
/* 0x001499F0 */
void Hewie_StateHeadForSpot(Hewie *h) {
    f32 d[4] __attribute__((aligned(16)));
    f32 a, step, left;

    sceVu0SubVector(d, h->c.unk110, h->c.a.pos);
    if (__builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) < 10.0f) {
        HW(h, 0xF36B4, s32) = 0;
        HW(h, 0xF36B8, s32) = 0;
        HW(h, 0xF36C4, f32) = h->c.a.angle[1];
        Hewie_SetBehaviour(h, &Hewie_StateWander_ptmf);
        return;
    }
    a = Actor_HeadingTo(&h->c.a, h->c.unk110);
    step = run_turn(h);
    left = turn_toward(h, a, step);
    Motion_LookAt(h->c.motion, h->c.unk110, &HW(h, 0xF3614, f32), &HW(h, 0xF3618, f32));
    HW(h, 0xF3604, s32) = 8;
    HW(h, 0xF3608, s32) = 0;
    if (left <= 0x1.0c15240000000p+0f /* 1.0471976 */) {
        if (Hewie_StepToPose(h, 0) == 0) {
            h->c.unkE1 = 1;
        }
    } else {
        Hewie_KeepPose(h, 7);
    }
}

/* ---- following a path ---- */

/* his root motion forward this frame (scaled to the ground, motion +0x48), into `root` */
static f32 root_ahead(Hewie *h, f32 *root) {
    Motion_RootMovement(h->c.motion, root, 0.0f);
    root[2] *= VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    return root[2];
}

/* walk his planned path to the spot +0x110 (animation +0x108, else pose 7 for action 0x41 and 9
 * otherwise; not while blending): heading for the spot when he is on its triangle (+0x104),
 * else 10 (action 0x3F) or 20 along the path; stepping along it as far as his root motion
 * goes, less the sharper he turns. At the end, the rest of the step straight on, flag +0xE1
 * and the default action */
/* 0x0014A340 */
void Hewie_StateWalkPath(Hewie *h) {
    f32 at[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 root[4] __attribute__((aligned(16)));
    f32 rest, a, left, mv, s;
    u32 tri;

    if (AT(h->c.motion, 0x550, f32) <= 0.0f) {
        if (h->c.unk104[1] != -1) {
            if (MOTION_ANIM(h->c.motion) != h->c.unk104[1]) {
                Motion_PlayTable(h->c.motion, h->c.unk104[1], -1);
            }
        } else {
            Hewie_KeepPose(h, HEWIE_ACTION(h) == 0x41 ? 7 : 9);
        }
    }
    rest = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
        gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
    if ((h->c.unk104[0] == (s32)Actor_TriTo(&h->c.a, h->c.unk110, NAV_NONE)) != 1) {
        Character_WaypointAhead(&h->c, &tri, at, HEWIE_ACTION(h) == 0x3F ? 10.0f : 20.0f);
        a = Actor_HeadingTo(&h->c.a, at);
    } else {
        a = Actor_HeadingTo(&h->c.a, h->c.unk110);
    }
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
    left = Actor_TurnToward(&h->c.a, a, run_turn(h));
    s = root_ahead(h, root);
    mv = 0.0f;
    if (!(s < 0.0f)) {
        mv = s * ((0x1.921fb60000000p+1f /* 3.1415927 */ - left) / 0x1.921fb60000000p+1f /* 3.1415927 */);
    }
    h->c.unk128 = Character_WaypointAhead(&h->c, &tri, p, mv);
    h->c.a.navTri = tri;
    sceVu0CopyVector(h->c.a.pos, p);
    HW(h, 0xF3558, u8) = 1;
    if (h->c.unk128 >= h->c.unk124) {
        s = root_ahead(h, root);
        if (rest < s) {
            root[0] = 0.0f;
            root[1] = 0.0f;
            root[2] = s - rest;
            sceVu0ApplyMatrix(root, h->c.a.rot, root);
            Actor_Move(&h->c.a, root);
        }
        h->c.unkE1 = 1;
        hewie_want(h, 0, 0);
    }
}

/* ---- walking to a spot and settling ---- */

/* |wrap(+0x10C - his heading)| as the original takes it (twice, 0 becomes -0) */
static f32 off_heading(Hewie *h) {
    if (!(Angle_Wrap(HW(h, 0x10C, f32) - h->c.a.angle[1]) <= 0.0f)) {
        return Angle_Wrap(HW(h, 0x10C, f32) - h->c.a.angle[1]);
    }
    return -Angle_Wrap(HW(h, 0x10C, f32) - h->c.a.angle[1]);
}

/* walk the path to the spot +0x110 (12 frames of root motion ahead along it, or straight once
 * on its triangle +0x104), stepping less the sharper he turns; near the end (under 3) turn on
 * the spot (animation 0x1300) to heading +0x10C over the angle +0xF36C4, slowing as it closes,
 * then stand. Done (at the end, settled, in animation +0x108 or standing): action 0x63 goes on
 * to 0x64 (0x13), else flag +0xE1 and the default action. Walk animation: +0x108, or by the
 * distance left with +0xF36B8 2 (pose 7 / 8 / 9 under 10 / 34 / beyond), else pose 7 */
/* 0x0014A920 */
void Hewie_StateWalkPath2(Hewie *h) {
    f32 root[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 s, a, left, mv, t, r, step, rest;
    s32 on, anim;
    u32 tri;

    on = h->c.unk104[0] == (s32)Actor_TriTo(&h->c.a, h->c.unk110, NAV_NONE);
    s = root_ahead(h, root);
    if ((u8)on != 1) {
        Character_WaypointAhead(&h->c, &tri, at, 12.0f * (s < 0.0f ? 0.0f : s));
        a = Actor_HeadingTo(&h->c.a, at);
    } else {
        a = Actor_HeadingTo(&h->c.a, h->c.unk110);
    }
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    anim = MOTION_ANIM(h->c.motion);
    if (anim == 0x1300) {
        HW(h, 0xF3614, f32) = 0.0f;
        HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0x10C, f32) - h->c.a.angle[1]);
        t = msl_sinf(off_heading(h) * 0x1.becde60000000p+0f /* 1.7453293 */ / HW(h, 0xF36C4, f32));
        r = 2.0f * (t * HW(h, 0xF36C4, f32)) / 0x1.921fb60000000p+1f /* 3.1415927 */;
        if (!(r <= 0x1.3333340000000p-1f /* 0.6 */)) {
            r = 0x1.3333340000000p-1f /* 0.6 */;
        }
        AT(MOTION_PTR(h->c.motion, 0x6A4), 0x1C, f32) = r;
        step = 0x1.99999a0000000p-5f /* 0.05 */ * HW(h, 0xF36C4, f32) * t;
        if (step < 0x1.1df46a0000000p-6f /* 0.017453292 */) {
            step = 0x1.1df46a0000000p-6f /* 0.017453292 */;
        } else if (!(step <= 0x1.aceea00000000p-4f /* 0.10471976 */)) {
            step = 0x1.aceea00000000p-4f /* 0.10471976 */;
        }
        left = Actor_TurnToward(&h->c.a, HW(h, 0x10C, f32), step);
        if (left == 0.0f && AT(h->c.motion, 0x550, f32) <= 0.0f) {
            Motion_PlayBlend(h->c.motion, !(u8)Progress_GameMode(gProgress) ? 0 : 3, 5, -1);
        }
        mv = 0.25f;
    } else {
        HW(h, 0xF3614, f32) = 0.0f;
        HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
        left = Actor_TurnToward(&h->c.a, a, run_turn(h));
        mv = 0.0f;
        if (!(s < 0.0f)) {
            mv = s * ((0x1.921fb60000000p+1f /* 3.1415927 */ - left) / 0x1.921fb60000000p+1f /* 3.1415927 */);
        }
        if (mv < 0x1.99999a0000000p-5f /* 0.05 */) {
            mv = 0x1.99999a0000000p-5f /* 0.05 */;
        }
    }
    h->c.unk128 = Character_WaypointAhead(&h->c, &tri, p, mv);
    h->c.a.navTri = tri;
    sceVu0CopyVector(h->c.a.pos, p);
    HW(h, 0xF3558, u8) = 1;
    if (!(AT(h->c.motion, 0x550, f32) <= 0.0f)) {
        return;
    }
    if (h->c.unk128 >= h->c.unk124 && HW(h, 0xF36B4, s32) != 0 && left == 0.0f &&
        (h->c.unk104[1] != -1 ? h->c.unk104[1] == anim : Hewie_AnimGroup(h) == 0)) {
        if (HEWIE_ACTION(h) == 0x63) {
            hewie_want(h, 0x64, 0x13);
        } else {
            h->c.unkE1 = 1;
            hewie_want(h, 0, 0);
        }
        return;
    }
    if (h->c.unk104[1] != -1) {
        if (anim == h->c.unk104[1]) {
            if (VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
                    gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C) < 3.0f) {
                HW(h, 0xF36B4, s32) = 1;
            }
        } else {
            Motion_PlayTable(h->c.motion, h->c.unk104[1], -1);
        }
    } else if (HW(h, 0xF36B4, s32) == 0) {
        rest = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
            gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
        if (!(rest < 3.0f)) {
            if (HW(h, 0xF36B8, s32) == 2 && !(rest < 10.0f)) {
                Hewie_KeepPose(h, rest < 34.0f ? 8 : 9);
            } else {
                Hewie_KeepPose(h, 7);
            }
        } else {
            r = off_heading(h);
            if (r < 0x1.1df46a0000000p-6f /* 0.017453292 */) {
                r = 0x1.1df46a0000000p-6f /* 0.017453292 */;
            }
            HW(h, 0xF36C4, f32) = r;
            HW(h, 0xF36B4, s32) = 1;
            Motion_PlayTable(h->c.motion, 0x1300, !(u8)Progress_GameMode(gProgress) ? 0 : 3);
            AT(MOTION_PTR(h->c.motion, 0x6A4), 0x1C, f32) = 1.0f;
        }
    }
}

/* ---- setting off for a spot ---- */

extern const PTMF Hewie_StateWalkPath2_ptmf, Hewie_StateWalkPath2_ptmf2;

/* move by his root motion this frame (level, scaled to the ground) */
static void slide_root(Hewie *h) {
    f32 root[4] __attribute__((aligned(16)));
    f32 k;

    Motion_RootMovement(h->c.motion, root, 0.0f);
    k = VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    root[1] = 0.0f;
    root[2] *= k;
    sceVu0ApplyMatrix(root, h->c.a.rot, root);
    Actor_Move(&h->c.a, root);
    HW(h, 0xF3558, u8) = 1;
}

/* plan his path to the spot +0x110 (triangle +0x104): > 0 if there is one */
static s32 plan_to_spot(Hewie *h) {
    s32 r = Character_PlanPathKind(&h->c, 0, h->c.unk104[0], h->c.unk110);

    if (r > 0) {
        r = Character_WaypointsCurve(&h->c);
    }
    return r;
}

/* set off for the spot +0x110: with no animation of his own (+0x108), once standing plan the
 * way and walk it (by the distance with +0xF36B8 2, Hewie_StateWalkPath2) in behaviour Hewie_StateWalkPath2_ptmf;
 * else in that animation, behaviour Hewie_StateWalkPath2_ptmf2. No way there: the default action */
/* 0x0014B190 */
void Hewie_StateSetOff(Hewie *h) {
    slide_root(h);
    if (h->c.unk104[1] == -1) {
        if (Hewie_StepToPose(h, 0) != 0) {
            return;
        }
        if (plan_to_spot(h) > 0) {
            if (HW(h, 0xF36B8, s32) == 2) {
                f32 rest = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
                    gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);

                Hewie_KeepPose(h, !(rest < 10.0f) ? (rest < 34.0f ? 8 : 9) : 7);
            } else {
                Hewie_KeepPose(h, 7);
            }
            Hewie_SetBehaviour(h, &Hewie_StateWalkPath2_ptmf);
            return;
        }
    } else {
        slide_root(h);
        if (plan_to_spot(h) > 0) {
            Hewie_SetBehaviour(h, &Hewie_StateWalkPath2_ptmf2);
            return;
        }
    }
    hewie_want(h, 0, 0);
}

/* ---- leaping to a spot ---- */

extern const PTMF Hewie_StateLeap_ptmf, Hewie_StateTurnTo_ptmf;

/* in a leap (along +0xF36E0, height +0xF36C8, rise +0xF36C4): turning to heading +0x10C by 10
 * degrees; in the rising animation 0x1E04 by its root motion (its rise scaled by +0xF36C4 while
 * event bit 2 is on), then (0x1E05 once it ends) 3 a frame forward falling ever faster (+0xF36CC
 * from 1.2889, +0.5 a frame up to 3). Below the ground: landed (0x1E06), behaviour
 * Hewie_StateTurnTo_ptmf */
/* 0x0014BB10 */
void Hewie_StateLeap(Hewie *h) {
    f32 root[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 ground, y;

    turn_toward(h, HW(h, 0x10C, f32), 0x1.6571860000000p-3f /* 0.17453294 */);
    if (ANIM_DONE(h) && MOTION_ANIM(h->c.motion) != 0x1E05) {
        Motion_Play(h->c.motion, 0x1E05, -1);
        HW(h, 0xF36CC, f32) = 0x1.49f55a0000000p+0f /* 1.2889 */;
    }
    if (MOTION_ANIM(h->c.motion) == 0x1E04) {
        Motion_RootTranslation(h->c.motion, root, 0.0f);
        vu0_ScaleXYZ(v, &HW(h, 0xF36E0, f32), root[2]);
        Actor_Move(&h->c.a, v);
        HW(h, 0xF3558, u8) = 1;
        ground = h->c.a.pos[1];
        if (!((u8)Motion_EventFlags(h->c.motion, 0, 0, 1) & 2)) {
            y = HW(h, 0xF36C8, f32) + root[1];
        } else {
            y = HW(h, 0xF36C8, f32) + root[1] * HW(h, 0xF36C4, f32);
        }
    } else {
        vu0_ScaleXYZ(v, &HW(h, 0xF36E0, f32), 3.0f);
        Actor_Move(&h->c.a, v);
        HW(h, 0xF3558, u8) = 1;
        ground = h->c.a.pos[1];
        HW(h, 0xF36CC, f32) += 0.5f;
        if (!(HW(h, 0xF36CC, f32) <= 3.0f)) {
            HW(h, 0xF36CC, f32) = 3.0f;
        }
        y = HW(h, 0xF36C8, f32) - HW(h, 0xF36CC, f32);
    }
    h->c.a.pos[1] = y;
    HW(h, 0xF36C8, f32) = h->c.a.pos[1];
    if (!(ground <= h->c.a.pos[1])) {
        h->c.a.pos[1] = ground;
        Motion_Play(h->c.motion, 0x1E06, -1);
        Hewie_SetBehaviour(h, &Hewie_StateTurnTo_ptmf);
    }
}

/* run for the spot +0x110 (planned, keeping the request): once within +0x108 of the end of
 * the path and not blending, leap (level toward the spot, turning to it by 20 degrees, rising
 * by animation 0x1E04, behaviour Hewie_StateLeap_ptmf); meanwhile running (0x202) with the stride
 * (Hewie_Stride, else pose 8). No path: the default action */
/* 0x0014BE80 */
void Hewie_StateRunForSpot(Hewie *h) {
    f32 d[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 a, y;

    if (Hewie_PlanAndGo(h, h->c.unk104[0], h->c.unk110, 0, 1) != 0) {
        hewie_want(h, 0, 0);
        return;
    }
    if (AT(h->c.motion, 0x550, f32) <= 0.0f &&
        VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
            gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C) < (f32)h->c.unk104[1]) {
        sceVu0SubVector(d, h->c.unk110, h->c.a.pos);
        d[1] = 0.0f;
        sceVu0Normalize(&HW(h, 0xF36E0, f32), d);
        HW(h, 0xF36C8, f32) = h->c.a.pos[1];
        a = Actor_HeadingTo(&h->c.a, h->c.unk110);
        HW(h, 0x10C, f32) = a;
        turn_toward(h, a, 0x1.6571860000000p-2f /* 0.34906587 */);
        Motion_RootTranslation(h->c.motion, d, 0.0f);
        vu0_ScaleXYZ(v, &HW(h, 0xF36E0, f32), d[2]);
        Actor_Move(&h->c.a, v);
        HW(h, 0xF3558, u8) = 1;
        y = HW(h, 0xF36C8, f32) + d[1] * HW(h, 0xF36C4, f32);
        h->c.a.pos[1] = y;
        HW(h, 0xF36C8, f32) = y;
        Motion_PlayTable(h->c.motion, 0x1E04, -1);
        h->c.a.unk2D = 1;
        Hewie_SetBehaviour(h, &Hewie_StateLeap_ptmf);
        return;
    }
    if (Hewie_StepToPose(h, 5) == 0 && MOTION_ANIM(h->c.motion) != 0x202) {
        Motion_PlayTable(h->c.motion, 0x202, -1);
    }
    if (!(u8)Hewie_Stride(h)) {
        Hewie_KeepPose(h, 8);
    }
}

/* ---- whining at Fiona ---- */

/* with Fiona in his room: once standing, look at her and whine (sound 0x60, action 0x7D, 150
 * frames); when that is over, action +0xF3570. Without her, action +0xF3570 at once */
/* 0x0014C210 */
void Hewie_StateWhine(Hewie *h) {
    if (!in_his_room(h, gCharPlayer) || (HW(h, 0xF36B4, s32) != 0 && HW(h, 0xF355C, s32) == 0)) {
        hewie_want(h, HW(h, 0xF3570, s32), 0);
        return;
    }
    if (Hewie_StepToPose(h, 0) != 0) {
        return;
    }
    if (HW(h, 0xF36B4, s32) == 0) {
        HW(h, 0xF355C, s32) = 150;
        HW(h, 0xF36B4, s32) = 1;
        if (HW(h, 0xF3604, s32) != 0) {
            HW(h, 0xF3604, s32) = 0;
            HW(h, 0xF3608, s32) = 10;
        }
        HW(h, 0xF3544, Character *) = gCharPlayer;
        Hewie_MakeSound(h, 0x60);
        HEWIE_ACTION(h) = 0x7D;
    }
    Hewie_KeepPose(h, 0);
}

/* the last stretch to `at`, straight (no path left): turning to it as he runs, head held level
 * toward it, moving by his root motion unless he already moved this frame */
static void run_straight(Hewie *h, const f32 *at) {
    f32 a = Actor_HeadingTo(&h->c.a, at);
    f32 step = run_turn(h);

    turn_toward(h, a, step);
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
    if (HW(h, 0xF3558, u8) == 0) {
        slide_root(h);
        h->c.unk124 = h->c.unk128;
    }
}

/* ---- running from the pursuer ---- */

extern const s32 D_003B1C00[8];   /* wait after fleeing, by trust */

/* flee the pursuer (in his room, while it hunts: progress flags 9 / 0xA) to the point
 * +0xF36E0 (triangle +0xF36B4): running (pose 9) along the planned path (pose 8 when the stride
 * fails in tense mode), straight at it once on its triangle. When the exit +0x100 he flees by
 * is open to him and the point is nearer the door than he is: through it (Hewie_ThroughExit) and
 * wait (action 0x31) for his trust's time; a dead end, or no pursuer: the default action */
/* 0x0014C480 */
void Hewie_StateFlee(Hewie *h) {
    Progress *p;
    VObject *rooms;
    f32 door[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 c[4] __attribute__((aligned(16)));
    s32 there;

    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    p = gProgress;
    if (!(u8)Progress_TestFlag(p, 9) && !(u8)Progress_TestFlag(p, 0xA)) {
        hewie_want(h, 0, 0);
        return;
    }
    there = HW(h, 0xF36B4, s32) == (s32)Actor_TriTo(&h->c.a, &HW(h, 0xF36E0, f32), NAV_NONE);
    if (!there && h->c.unk128 >= h->c.unk124 && Hewie_PlanAndGo(h, HW(h, 0xF36B4, s32), &HW(h, 0xF36E0, f32), 0, 1) != 0) {
        hewie_want(h, 0, 0);
        return;
    }
    Hewie_KeepPose(h, 9);
    if (!there) {
        if (!(u8)Hewie_Stride(h) && (u8)Progress_GameMode(p)) {
            Hewie_KeepPose(h, 8);
        }
    } else {
        run_straight(h, &HW(h, 0xF36E0, f32));
    }
    if (!(PursuerGroup_Fields(p, (u8)h->c.unk100, (u8)h->c.a.slot) & 0xFF & 1)) {
        return;
    }
    rooms = gRooms;
    VCALL(rooms, 0x2C, void (*)(VObject *, u32, f32 *))(rooms, (u8)h->c.unk100, door);
    sceVu0SubVector(c, &HW(h, 0xF36E0, f32), door);
    sceVu0SubVector(b, h->c.a.pos, door);
    if (sceVu0InnerProduct(c, c) < sceVu0InnerProduct(b, b)) {
        if ((VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, h->c.a.room, (u8)h->c.unk100) & 0xFFFF) == 0xFFFF) {
            hewie_want(h, 0, 0);
        } else {
            Hewie_ThroughExit(h, (u8)h->c.unk100);
            HW(h, 0xF3560, s32) = D_003B1C00[HW(h, 0xF35CC, s16)];
            hewie_want(h, 0x31, 0);
        }
    }
}

/* ---- holding the pursuer off ---- */

extern const PTMF Hewie_StatePlanPursuer_ptmf2;

/* standing guard while the pursuer hunts in his room (flags 9 / 0xA; else the default action):
 * barking at it (with noise, as Hewie_StateBark) while it is 60..100 away by foot, looking at it;
 * nearer or farther, behaviour Hewie_StatePlanPursuer_ptmf2; unreachable, the default action */
/* 0x0014CBD0 */
void Hewie_StateGuard(Hewie *h) {
    Progress *p;
    f32 d;

    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    p = gProgress;
    if (!(u8)Progress_TestFlag(p, 9) && !(u8)Progress_TestFlag(p, 0xA)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (Hewie_StepToPose(h, 0) == 0) {
        d = Character_PathLength(&h->c, gCharPursuer->a.navTri, gCharPursuer->a.pos, NAV_NONE);
        if (d < 0.0f) {
            hewie_want(h, 0, 0);
            return;
        }
        if (d < 60.0f || !(d <= 100.0f)) {
            Hewie_SetBehaviour(h, &Hewie_StatePlanPursuer_ptmf2);
            return;
        }
        Hewie_Bark(h);
        {
            Progress *q = gProgress;

            if (((u8)Progress_TestFlag(q, 0x13) | (u8)Progress_TestFlag(q, 0x2B)) == 0) {
                Progress_GetVar(p, 0x27);
                Relation_Request(p, AT(h, 0x20, u8), 4, 6, 0, 0, 0.0f);
            }
        }
    }
    if (HW(h, 0xF3604, s32) != 0) {
        HW(h, 0xF3604, s32) = 0;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3544, Character *) = gCharPursuer;
}

/* ---- squaring up to the pursuer ---- */

extern const PTMF Hewie_StatePlanPursuer_ptmf, Hewie_StateGuard_ptmf2;

/* while the pursuer hunts in his room (else the default action): within 50, behaviour
 * Hewie_StatePlanPursuer_ptmf; else turn to face it (pose 7, head on it) and, within 60 degrees, take the
 * stance (animation 4, then behaviour Hewie_StateGuard_ptmf2) */
/* 0x0014CF40 */
void Hewie_StatePursuerNear(Hewie *h) {
    Progress *p;
    f32 d[4] __attribute__((aligned(16)));
    f32 a, step, left;

    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    p = gProgress;
    if (!(u8)Progress_TestFlag(p, 9) && !(u8)Progress_TestFlag(p, 0xA)) {
        hewie_want(h, 0, 0);
        return;
    }
    sceVu0SubVector(d, gCharPursuer->a.pos, h->c.a.pos);
    if (__builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) < 50.0f) {
        Hewie_SetBehaviour(h, &Hewie_StatePlanPursuer_ptmf);
        return;
    }
    a = Actor_HeadingTo(&h->c.a, gCharPursuer->a.pos);
    step = run_turn(h);
    left = turn_toward(h, a, step);
    Motion_LookAt(h->c.motion, gCharPursuer->a.pos, &HW(h, 0xF3614, f32), &HW(h, 0xF3618, f32));
    HW(h, 0xF3604, s32) = 8;
    HW(h, 0xF3608, s32) = 0;
    if (!(left <= 0x1.0c15240000000p+0f /* 1.0471976 */)) {
        Hewie_KeepPose(h, 7);
        return;
    }
    if (!(AT(h->c.motion, 0x550, f32) <= 0.0f)) {
        return;
    }
    if (MOTION_ANIM(h->c.motion) == 4) {
        Hewie_SetBehaviour(h, &Hewie_StateGuard_ptmf2);
    } else {
        if (Hewie_StepToPose(h, 0) == 0) {
            Motion_PlayTable(h->c.motion, 4, -1);
        }
        VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
    }
}

/* ---- getting to the spot before the pursuer ---- */

extern const PTMF Hewie_StatePursuerNear_ptmf;

/* while the pursuer hunts in his room (else the default action): run for the spot +0x110 (pose
 * 9; along the planned path, pose 8 when the stride fails; straight once on its triangle).
 * Within 10 of it: behaviour Hewie_StatePursuerNear_ptmf. No path: the default action */
/* 0x0014D490 */
void Hewie_StateRunSpot(Hewie *h) {
    Progress *p;
    s32 there;
    f32 d;

    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    p = gProgress;
    if (!(u8)Progress_TestFlag(p, 9) && !(u8)Progress_TestFlag(p, 0xA)) {
        hewie_want(h, 0, 0);
        return;
    }
    there = h->c.unk104[0] == (s32)Actor_TriTo(&h->c.a, h->c.unk110, NAV_NONE);
    if (!there) {
        if (h->c.unk128 >= h->c.unk124 && Hewie_PlanAndGo(h, h->c.unk104[0], h->c.unk110, 0, 1) != 0) {
            hewie_want(h, 0, 0);
            return;
        }
        d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
            gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
    } else {
        d = Actor_Distance(&h->c.a, h->c.unk110);
    }
    if (d < 10.0f) {
        Hewie_SetBehaviour(h, &Hewie_StatePursuerNear_ptmf);
        return;
    }
    Hewie_KeepPose(h, 9);
    if (!there) {
        if (!(u8)Hewie_Stride(h)) {
            Hewie_KeepPose(h, 8);
        }
    } else {
        run_straight(h, h->c.unk110);
    }
}

/* ---- cutting the pursuer off ---- */

extern const PTMF Hewie_StateGuard_ptmf, Hewie_StateFlee_ptmf, Hewie_StateRunSpot_ptmf;

/* while the pursuer hunts in his room (else the default action): plan the pursuer's way to the
 * point +0xF36E0 (triangle +0xF36B4) and take the spot 80 along it (+0x110, triangle +0x104).
 * Short of its end: behaviour Hewie_StateRunSpot_ptmf; at it, Hewie_StateGuard_ptmf the first time and Hewie_StateFlee_ptmf after
 * (+0xF36BC). No way: the default action */
/* 0x0014DA50 */
void Hewie_StatePlanPursuer(Hewie *h) {
    Progress *p;
    VObject *planner;
    PathRequest *req;
    s32 r;
    u32 tri;

    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    p = gProgress;
    if (!(u8)Progress_TestFlag(p, 9) && !(u8)Progress_TestFlag(p, 0xA)) {
        hewie_want(h, 0, 0);
        return;
    }
    req = h->c.pathReq;
    req->unk0 = 0;
    h->c.pathReq->startTri = gCharPursuer->a.navTri;
    sceVu0CopyVector(h->c.pathReq->startPos, gCharPursuer->a.pos);
    h->c.pathReq->goalTri = HW(h, 0xF36B4, u32);
    sceVu0CopyVector(h->c.pathReq->goalPos, &HW(h, 0xF36E0, f32));
    planner = gSceneGameF29740;
    h->c.pathId = VCALL(planner, 0xC, s32 (*)(VObject *, PathRequest *, s32))(planner, h->c.pathReq, 0);
    if (h->c.pathId == -1) {
        hewie_want(h, 0, 0);
        return;
    }
    r = VCALL(planner, 0x14, s32 (*)(VObject *))(planner);
    if (r > 0) {
        r = Character_WaypointsCurve(&h->c);
    }
    if (r < 0) {
        hewie_want(h, 0, 0);
        return;
    }
    tri = gCharPursuer->a.navTri;
    sceVu0CopyVector(h->c.unk110, gCharPursuer->a.pos);
    r = VCALL(planner, 0x20, s32 (*)(VObject *, u32 *, f32 *, s32, s32, void *, f32))(
        planner, &tri, h->c.unk110, 0, h->c.unk124, h->c.unk12C, 80.0f);
    h->c.unk104[0] = tri;
    if (r == h->c.unk124) {
        Hewie_SetBehaviour(h, HW(h, 0xF36BC, s32) == 0 ? &Hewie_StateGuard_ptmf : &Hewie_StateFlee_ptmf);
    } else {
        Hewie_SetBehaviour(h, &Hewie_StateRunSpot_ptmf);
    }
    HW(h, 0xF36BC, s32) = 1;
}

/* ---- following Fiona ---- */

extern const PTMF Hewie_State1AF0_ptmf2, Hewie_State1B00_ptmf2;

/* one step of going where her command puts him by her (Hewie_CommandPlace): along the planned path
 * (straight once on its triangle), by gait +0xF36B4 with hysteresis on the distance left: walk
 * (0; over 30 trot), trot (1; under 20 walk, over `up` run), run (2; under `down` trot). -1 with
 * no path (the default action taken); 1 when walking within 12 (there; the path ended) */
static s32 follow_step(Hewie *h, f32 up, f32 down) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d, a, step;
    s32 there;
    u32 tri;

    tri = Hewie_CommandPlace(h, HEWIE_ACTION(h), at);
    there = tri == Actor_TriTo(&h->c.a, at, NAV_NONE);
    if (!there && h->c.unk128 >= h->c.unk124 && Hewie_PlanAndGo(h, tri, at, 0, 1) != 0) {
        hewie_want(h, 0, 0);
        return -1;
    }
    if (!there) {
        d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
            gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
    } else {
        d = Actor_Distance(&h->c.a, at);
    }
    switch (HW(h, 0xF36B4, s32)) {
    case 0:
        if (d < 12.0f) {
            h->c.unk124 = h->c.unk128;
            return 1;
        }
        if (d <= 30.0f) {
            Hewie_KeepPose(h, 7);
        } else {
            HW(h, 0xF36B4, s32) = 1;
            Hewie_KeepPose(h, 8);
        }
        break;
    case 1:
        if (d < 20.0f) {
            HW(h, 0xF36B4, s32) = 0;
            Hewie_KeepPose(h, 7);
        } else if (d <= up) {
            Hewie_KeepPose(h, 8);
        } else {
            HW(h, 0xF36B4, s32) = 2;
            Hewie_KeepPose(h, 9);
        }
        break;
    case 2:
        if (d < down) {
            HW(h, 0xF36B4, s32) = 1;
            Hewie_KeepPose(h, 8);
        } else {
            Hewie_KeepPose(h, 9);
        }
        break;
    }
    if (!there) {
        if (!(u8)Hewie_Stride(h) && HW(h, 0xF36B4, s32) == 2) {
            Hewie_KeepPose(h, 8);
        }
        return 0;
    }
    a = Actor_HeadingTo(&h->c.a, at);
    step = run_turn(h);
    turn_toward(h, a, step);
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
    if (HW(h, 0xF3558, u8) == 0) {
        h->c.unk124 = h->c.unk128;
    }
    return 0;
}

/* follow her command's place (follow_step); there: behaviour Hewie_State1AF0_ptmf2 (Hewie_State1B00_ptmf2 with
 * +0xF36B8) */
/* 0x0014E300 */
void Hewie_StateFollowCommand(Hewie *h) {
    if (follow_step(h, 54.0f, 44.0f) == 1) {
        Hewie_SetBehaviour(h, HW(h, 0xF36B8, s32) == 0 ? &Hewie_State1AF0_ptmf2 : &Hewie_State1B00_ptmf2);
    }
}

/* ---- setting off after Fiona ---- */

extern const PTMF Hewie_State1AF0_ptmf, Hewie_State1B00_ptmf, Hewie_StateFollowCommand_ptmf;

/* from lying low (pose 3): slide with the root motion and plan the way to where her command
 * puts him (no way: the default action). Within 12 already: behaviour Hewie_State1AF0_ptmf (Hewie_State1B00_ptmf
 * with +0xF36B8); else pick the gait by the distance (walk under 20 or when moving 1, trot under
 * 44, else run) and follow her (Hewie_StateFollowCommand_ptmf) */
/* 0x0014E8A0 */
void Hewie_StateSlideToCommand(Hewie *h) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d;

    if (Hewie_StepToPose(h, 3) != 0) {
        return;
    }
    Hewie_KeepPose(h, 3);
    slide_root(h);
    if (Hewie_PlanAndGo(h, Hewie_CommandPlace(h, HEWIE_ACTION(h), at), at, 0, 1) != 0) {
        hewie_want(h, 0, 0);
        return;
    }
    d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
        gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
    if (d < 12.0f) {
        h->c.unk124 = h->c.unk128;
        Hewie_SetBehaviour(h, HW(h, 0xF36B8, s32) == 0 ? &Hewie_State1AF0_ptmf : &Hewie_State1B00_ptmf);
        return;
    }
    HW(h, 0xF36B4, s32) = 2;
    if (h->c.a.unkC4 == 1 || d < 20.0f) {
        HW(h, 0xF36B4, s32) = 0;
    } else if (d < 44.0f) {
        HW(h, 0xF36B4, s32) = 1;
    }
    Hewie_SetBehaviour(h, &Hewie_StateFollowCommand_ptmf);
}

/* ---- fetching ---- */

extern const PTMF Hewie_StateSlideToCommand_ptmf, Hewie_State1A60_ptmf, Hewie_StateSlideToCommand_ptmf2, Hewie_State1A60_ptmf2, Hewie_StateSlideToCommand_ptmf3, Hewie_State1AA0_ptmf, Hewie_State1A60_ptmf3;

/* fetch the thrown thing +0xF368C (an actor; its velocity +0xB0). Gone (inactive, or +0x2A 1):
 * a roll (kind 5 against +0xF3695): won, behaviour Hewie_StateSlideToCommand_ptmf (+0xF36B4 2), else Hewie_State1A60_ptmf for
 * 90 frames; likewise Hewie_StateSlideToCommand_ptmf2 / Hewie_State1A60_ptmf2 when it has settled (speed under 0.5) somewhere he
 * can't reach. Else run for it (eager +0xF369E: by the path left, walk / trot / run; else walk),
 * head on it once on its triangle; settled within 6 and him nearly stopped: Hewie_RollReactions's
 * rolls decide: pick it up (it goes inactive; Hewie_StateSlideToCommand_ptmf3), bring it to Fiona (Hewie_State1AA0_ptmf), or
 * leave it (Hewie_State1A60_ptmf3, 90 frames) */
/* 0x0014EE20 */
void Hewie_StateFetch(Hewie *h) {
    Actor *o = HW(h, 0xF368C, Actor *);
    f32 v[4] __attribute__((aligned(16)));
    f32 root[4] __attribute__((aligned(16)));
    f32 speed, s, rest, pitch, yaw, a;

    if (o == NULL || o->active == 0 || o->unk2A == 1) {
        HW(h, 0xF36B8, s32) = 0;
        if (Hewie_Roll(h, gRandom, 5, HW(h, 0xF3695, s8), 0xF36A1)) {
            HW(h, 0xF36B4, s32) = 2;
            Hewie_SetBehaviour(h, &Hewie_StateSlideToCommand_ptmf);
        } else {
            HW(h, 0xF36BC, s32) = 90;
            Hewie_SetBehaviour(h, &Hewie_State1A60_ptmf);
        }
        return;
    }
    sceVu0CopyVector(v, o->unkB0);
    speed = __builtin_sqrtf(sceVu0InnerProduct(v, v));
    if (speed < 0.5f && Hewie_PlanAndGo(h, o->navTri, o->pos, 0, 1) != 0) {
        HW(h, 0xF36B8, s32) = 0;
        if (Hewie_Roll(h, gRandom, 5, HW(h, 0xF3695, s8), 0xF36A1)) {
            HW(h, 0xF36B4, s32) = 2;
            Hewie_SetBehaviour(h, &Hewie_StateSlideToCommand_ptmf2);
        } else {
            HW(h, 0xF36BC, s32) = 90;
            Hewie_SetBehaviour(h, &Hewie_State1A60_ptmf2);
        }
        return;
    }
    s = root_ahead(h, root);
    rest = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
        gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
    if (speed < 1.0f) {
        if (speed < 0.5f && (s <= 0.0f ? -s : s) < 0.8f && rest < 6.0f) {
            HW(h, 0xF36B8, s32) = 1;
            Hewie_RollReactions(h);
            if (HW(h, 0xF369F, u8) != 0) {
                o = HW(h, 0xF368C, Actor *);
                if (o->active != 0) {
                    o->active = 0;
                }
                HW(h, 0xF36B4, s32) = 2;
                Hewie_SetBehaviour(h, &Hewie_StateSlideToCommand_ptmf3);
            } else if (HW(h, 0xF36A0, u8) != 0) {
                if (HW(h, 0xF3604, s32) != 0) {
                    HW(h, 0xF3604, s32) = 0;
                    HW(h, 0xF3608, s32) = 10;
                }
                HW(h, 0xF3544, Character *) = gCharPlayer;
                Hewie_SetBehaviour(h, &Hewie_State1AA0_ptmf);
            } else {
                HW(h, 0xF36BC, s32) = 90;
                Hewie_SetBehaviour(h, &Hewie_State1A60_ptmf3);
            }
            return;
        }
        Hewie_KeepPose(h, HW(h, 0xF369E, u8) == 0 ? 7 : rest < 30.0f ? (rest < 20.0f ? 7 : 8) : 9);
    } else {
        Hewie_KeepPose(h, HW(h, 0xF369E, u8) == 0 ? 7 : rest < 30.0f ? 8 : 9);
    }
    Hewie_Stride(h);
    o = HW(h, 0xF368C, Actor *);
    if (Actor_TriTo(&h->c.a, o->pos, NAV_NONE) == o->navTri) {
        if (HW(h, 0xF3604, s32) != 8) {
            HW(h, 0xF3604, s32) = 8;
            HW(h, 0xF3608, s32) = 10;
        }
        Motion_LookAt(h->c.motion, HW(h, 0xF368C, Actor *)->pos, &pitch, &yaw);
        a = Angle_Wrap(h->c.a.angle[1] + yaw);
        HW(h, 0xF3614, f32) = pitch;
        HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
    }
}

/* airborne in a leap: turn toward +0xF36CC by +0xF36D0, move +0xF36C4 along the way and fall
 * (+0xF36C8 less 0.3 a frame onto height +0xF36EC), never below the ground */
static void leap_fly(Hewie *h) {
    f32 p[4] __attribute__((aligned(16)));
    u32 tri;

    Actor_TurnToward(&h->c.a, HW(h, 0xF36CC, f32), HW(h, 0xF36D0, f32));
    tri = h->c.a.navTri;
    Character_WaypointAhead(&h->c, &tri, p, HW(h, 0xF36C4, f32));
    h->c.a.navTri = tri;
    sceVu0CopyVector(h->c.a.pos, p);
    HW(h, 0xF36C8, f32) -= 0x1.333334p-2f /* 0.3 */;
    HW(h, 0xF36EC, f32) += HW(h, 0xF36C8, f32);
    if (!(HW(h, 0xF36EC, f32) <= h->c.a.pos[1])) {
        h->c.a.pos[1] = HW(h, 0xF36EC, f32);
    }
}

/* ---- the leap at the pursuer ---- */

extern const PTMF Hewie_StateBiteHold_ptmf2;

/* in a leap at the pursuer (in his room; else, or when his state block holds 7, the default
 * action). Airborne (blending): turn toward +0xF36CC by +0xF36D0, move +0xF36C4 along the way,
 * fall (+0xF36C8 -0.3 a frame onto height +0xF36EC, never below the ground); at frame 13 the
 * noise (unless progress flags 0x13 / 0x2B). Landed: on the spot +0x110 (triangle +0x108), the
 * hold time +0xF36BC by his mood (3: 110, 2: none, else 5 x (10 + his feeling about it)), the
 * hit on it (30, +30 on difficulty 1, +half with progress +0xA10; hard when Hewie_ReactionRoll 5) and
 * its effect, then behaviour Hewie_StateBiteHold_ptmf2 */
/* 0x0014F600 */
void Hewie_StateLeapAtPursuer(Hewie *h) {
    Progress *pr;
    u16 dmg;

    if (h->c.state[0] == 7) {
        h->c.state[0] = 0;
        hewie_want(h, 0, 0);
        return;
    }
    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (!(AT(h->c.motion, 0x550, f32) <= 0.0f)) {
        HW(h, 0xF36B4, s32) -= 1;
        if (HW(h, 0xF36B4, s32) == 13) {
            pr = gProgress;
            if (((u8)Progress_TestFlag(pr, 0x13) | (u8)Progress_TestFlag(pr, 0x2B)) == 0) {
                Progress_GetVar(pr, 0x27);
                Relation_Request(pr, AT(h, 0x20, u8), 4, 9, 0, 15, 0.0f);
            }
        }
        leap_fly(h);
    } else {
        h->c.a.navTri = h->c.unk104[1];
        sceVu0CopyVector(h->c.a.pos, h->c.unk110);
        h->c.a.pos[3] = 1.0f;
        if (HW(h, 0xF35C0, s32) == 3) {
            HW(h, 0xF36BC, s32) = 110;
        } else if (HW(h, 0xF35C0, s32) == 2) {
            HW(h, 0xF36BC, s32) = 0;
        } else {
            s16 f = 0;

            if (gCharPursuer != NULL && gCharPursuer->a.active == 1) {
                s16 *v = Hewie_Feeling(h, gCharPursuer->unk153C);

                f = v != NULL ? *v : 0;
            }
            HW(h, 0xF36BC, s32) = (f + 10) * 5;
        }
        HW(h, 0xF36B4, s32) = 1;
        HW(h, 0xF36B8, s32) = 1;
        HW(h, 0xF3585, u8) = 1;
        {
            s32 hard = (u8)Hewie_ReactionRoll(h, 5) == 1;

            pr = gProgress;
            dmg = 30;
            if ((u8)Progress_GetVar(pr, 0x27) == 1) {
                dmg += 30;
            }
            if (AT(pr, 0xA10, s32) != 0) {
                dmg += dmg >> 1;
            }
            Relation_Request(pr, AT(h, 0x20, u8), 4, 0xB, dmg, hard ? -0x8000 : 0, 0.0f);
        }
        Hewie_BiteEffect(h, 1);
        Hewie_SetBehaviour(h, &Hewie_StateBiteHold_ptmf2);
    }
    HW(h, 0xF3581, u8) = 1;
    HW(h, 0xF3558, u8) = 1;
    HW(h, 0xF3582, u8) = 0;
}

/* ---- taking off at the pursuer ---- */

extern const PTMF Hewie_StateLeapAtPursuer_ptmf;

/* the take-off of the leap at the pursuer (in his room; else the default action): once not
 * blending, the leap animation 0x2218 blended over the frames left (+0xF36B4) and behaviour
 * Hewie_StateLeapAtPursuer_ptmf; airborne meanwhile */
/* 0x0014FC70 */
void Hewie_StateLeapTakeOff(Hewie *h) {
    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (AT(h->c.motion, 0x550, f32) <= 0.0f) {
        Motion_PlayWith(h->c.motion, 0x2218, 9, -1, (f32)HW(h, 0xF36B4, s32));
        Hewie_SetBehaviour(h, &Hewie_StateLeapAtPursuer_ptmf);
    }
    leap_fly(h);
    HW(h, 0xF36B4, s32) -= 1;
    HW(h, 0xF3558, u8) = 1;
    HW(h, 0xF3582, u8) = 0;
}

/* ---- launching the leap at the pursuer ---- */

extern const char str_Dog_New_Hide_Attack_No_Route[];                   /* "Dog New Hide Attack -> No Route" */
extern const PTMF Hewie_StateLeapTakeOff_ptmf;

/* launch the leap at the pursuer (in his room; else, or when his state block holds 7, the
 * default action) to the spot +0x110 (triangle +0x108), heading +0x10C: frames (+0xF36B4) a
 * third of the planned distance, at least 19; the speed, the rise (to land at the spot's height
 * under 0.3 a frame of gravity) and the turn a frame from them; animation 0x1E01, sound 0x68,
 * behaviour Hewie_StateLeapTakeOff_ptmf. No way there: a debug message and the default action */
/* 0x0014FF10 */
void Hewie_StateLeapLaunch(Hewie *h) {
    f32 a, d, rest, fn;
    s32 n;

    if (h->c.state[0] == 7) {
        h->c.state[0] = 0;
        hewie_want(h, 0, 0);
        return;
    }
    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    a = HW(h, 0x10C, f32);
    if (Hewie_PlanAndGo(h, h->c.unk104[1], h->c.unk110, 0, 1) != 0) {
        msl_printf(str_Dog_New_Hide_Attack_No_Route);
        hewie_want(h, 0, 0);
        return;
    }
    rest = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
        gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
    n = (s32)(rest / 3.0f);
    if (n < 19) {
        n = 19;
    }
    fn = (f32)n;
    HW(h, 0xF36EC, f32) = h->c.a.pos[1];
    HW(h, 0xF36B4, s32) = n;
    HW(h, 0xF36C4, f32) = rest / fn;
    HW(h, 0xF36C8, f32) = (h->c.unk110[1] - h->c.a.pos[1]) / fn + 0.5f * (0x1.3333340000000p-2f /* 0.3 */ * fn);
    d = Angle_Wrap(a - h->c.a.angle[1]);
    HW(h, 0xF36CC, f32) = a;
    if (d <= 0.0f) {
        d = -d;
    }
    HW(h, 0xF36D0, f32) = d / fn;
    Motion_PlayTable(h->c.motion, 0x1E01, -1);
    HW(h, 0xF3558, u8) = 1;
    Hewie_MakeSound(h, 0x68);
    Hewie_SetBehaviour(h, &Hewie_StateLeapTakeOff_ptmf);
}

/* ---- moving to Fiona's side ---- */

extern const PTMF Hewie_StatePlayToEnd_ptmf;

/* step over to the place by Fiona (her in his room and not out of reach, +0xE0; else the
 * default action): turning toward +0xF36CC by +0xF36D0 and moving +0xF36C4 a frame while
 * blending; then put exactly at +0xF36E0 (triangle +0x104, snapped onto it) facing +0xF36CC,
 * behaviour Hewie_StatePlayToEnd_ptmf */
/* 0x00150930 */
void Hewie_StateByFiona(Hewie *h) {
    f32 p[4] __attribute__((aligned(16)));
    u32 tri;

    if (!in_his_room(h, gCharPlayer) || gCharPlayer->unkE0 == 1) {
        hewie_want(h, 0, 0);
        return;
    }
    if (AT(h->c.motion, 0x550, f32) <= 0.0f) {
        f32 a;

        h->c.a.navTri = h->c.unk104[0];
        sceVu0CopyVector(h->c.a.pos, &HW(h, 0xF36E0, f32));
        VCALL(gNavMesh, 0x14, void (*)(void *, u32, f32 *))(gNavMesh, h->c.a.navTri, h->c.a.pos);
        a = HW(h, 0xF36CC, f32);
        h->c.a.angle[1] = a;
        sceVu0UnitMatrix(h->c.a.rot);
        sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, a);
        Hewie_SetBehaviour(h, &Hewie_StatePlayToEnd_ptmf);
    } else {
        Actor_TurnToward(&h->c.a, HW(h, 0xF36CC, f32), HW(h, 0xF36D0, f32));
        tri = h->c.a.navTri;
        Character_WaypointAhead(&h->c, &tri, p, HW(h, 0xF36C4, f32));
        h->c.a.navTri = tri;
        sceVu0CopyVector(h->c.a.pos, p);
    }
    HW(h, 0xF3558, u8) = 1;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* ---- going behind Fiona ---- */

extern const PTMF Hewie_StateByFiona_ptmf;

/* head for the place behind Fiona (her in his room and not out of reach; else, or with his
 * state block at 7, the default action): animation 0x8000, the point kHewieMeetOffsets in her frame
 * facing heading +0x10C (kHewieMeetOffsets[0]; +0xF36E0, planned; no way: the default action), a tenth of
 * the way and of the turn to face away (+0xF36CC) each frame; behaviour Hewie_StateByFiona_ptmf */
/* 0x00150C10 */
void Hewie_StateBehindFiona(Hewie *h) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 a, back, rest, d;

    if (h->c.state[0] == 7) {
        h->c.state[0] = 0;
        hewie_want(h, 0, 0);
        return;
    }
    if (!in_his_room(h, gCharPlayer) || gCharPlayer->unkE0 == 1) {
        hewie_want(h, 0, 0);
        return;
    }
    a = HW(h, 0x10C, f32);
    back = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + a);
    Motion_PlayWith(h->c.motion, 0x8000, 8, -1, 10.0f);
    v[1] = 0.0f;
    v[3] = 0.0f;
    v[0] = kHewieMeetOffsets[0][0];
    v[2] = kHewieMeetOffsets[0][1];
    Mtx_AtHeading(m, gCharPlayer->a.pos, a);
    Mtx_ApplyPoint(at, m, v);
    if (Hewie_PlanAndGo(h, h->c.unk104[0], at, 0, 1) != 0) {
        hewie_want(h, 0, 0);
        return;
    }
    rest = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
        gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
    sceVu0CopyVector(&HW(h, 0xF36E0, f32), at);
    HW(h, 0xF36C4, f32) = 0x1.99999a0000000p-4f /* 0.1 */ * rest;
    d = Angle_Wrap(back - h->c.a.angle[1]);
    HW(h, 0xF36CC, f32) = back;
    if (d <= 0.0f) {
        d = -d;
    }
    HW(h, 0xF36D0, f32) = 0x1.99999a0000000p-4f /* 0.1 */ * d;
    HW(h, 0xF3558, u8) = 1;
    Hewie_SetBehaviour(h, &Hewie_StateByFiona_ptmf);
}

/* ---- heeling behind Fiona ---- */

extern const PTMF Hewie_StateBackToNormal_ptmf;

/* keep to the place behind Fiona (kHewieMeetOffsets[0] in her frame by heading +0x10C; her own spot
 * when that is off the mesh or blocked by flags 0x29020008), her in his room and not out of
 * reach (else the default action). Within 10 of it and facing away from her (within 90 degrees):
 * hold still, and when the game takes it (SlotCmd_Give 2 6) behaviour Hewie_StateBackToNormal_ptmf. Else walk
 * there (run, 0x202, when the stride says so; else 0x201) */
/* 0x00151190 */
void Hewie_StateKeepBehind(Hewie *h) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    u32 tri;
    s32 anim;

    if (!in_his_room(h, gCharPlayer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (gCharPlayer->unkE0 == 1) {
        hewie_want(h, 0, 0);
        return;
    }
    v[1] = 0.0f;
    v[3] = 0.0f;
    v[0] = kHewieMeetOffsets[0][0];
    v[2] = kHewieMeetOffsets[0][1];
    Mtx_AtHeading(m, gCharPlayer->a.pos, HW(h, 0x10C, f32));
    Mtx_ApplyPoint(at, m, v);
    tri = Actor_TriOf(&gCharPlayer->a, at);
    /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
    if (tri == NAV_NONE || (NavMesh_TriFlags(gNavMesh, tri) & 0x29020008)) {
        tri = gCharPlayer->a.navTri;
        sceVu0CopyVector(at, gCharPlayer->a.pos);
    }
    if (Actor_Distance(&h->c.a, at) < 10.0f) {
        f32 d;

        if (!(Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + HW(h, 0x10C, f32) - h->c.a.angle[1]) <= 0.0f)) {
            d = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + HW(h, 0x10C, f32) - h->c.a.angle[1]);
        } else {
            d = -Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + HW(h, 0x10C, f32) - h->c.a.angle[1]);
        }
        if (d < 0x1.921fb60000000p+0f /* 1.5707964 */) {
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 0;
            if ((SlotCmd_Give(gProgress, 2, 6, AT(h, 0x20, u8), 0, 0, 0.0f) & 0xFF) == 1) {
                Hewie_SetBehaviour(h, &Hewie_StateBackToNormal_ptmf);
            }
            return;
        }
    }
    if (h->c.unk128 < h->c.unk124 || Hewie_PlanAndGo(h, tri, at, 0, 0) == 0) {
        anim = MOTION_ANIM(h->c.motion);
        if ((u8)Hewie_Stride(h) == 1) {
            if (anim != 0x202) {
                Motion_PlayTable(h->c.motion, 0x202, -1);
            }
        } else if (anim != 0x201) {
            Motion_PlayTable(h->c.motion, 0x201, -1);
        }
    }
}

/* ---- on a slope ---- */

/* on sloped ground (mesh flag 1; leaving it: the default action), trotting (pose 8): with a door
 * he may use (state bit 1) through it once +0xF36B4 drops to 0 (it starts at 1 with one, 0
 * without). Heading along the slope: the way he faces down or up it at first; going the other
 * way (+0xF36B8, set when the way ahead runs off the mesh) unless already facing within 60
 * degrees of it, then the default action. Turning at least 1.5 degrees a frame; once facing it,
 * testing the step ahead */
/* 0x00151D10 */
void Hewie_StateSlope(Hewie *h) {
    Progress *p = gProgress;
    f32 n[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    f32 fwd[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 pt[4] __attribute__((aligned(16)));
    f32 a, step;
    u32 e;

    for (e = 0; e < 8; e = (e + 1) & 0xFF) {
        if (PursuerGroup_Fields(p, e, (u8)h->c.a.slot) & 0xFF & 1) {
            break;
        }
    }
    if (HW(h, 0xF36B4, s32) != -1) {
        if ((u8)e != 8) {
            if (HW(h, 0xF36B4, s32) == 0) {
                Hewie_ThroughExit(h, e);
                hewie_want(h, 0, 0);
                return;
            }
        } else {
            HW(h, 0xF36B4, s32) = 0;
        }
    } else {
        HW(h, 0xF36B4, s32) = (u8)e == 8 ? 0 : 1;
    }
    Hewie_KeepPose(h, 8);
    /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
    if (!(NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & 1)) {
        hewie_want(h, 0, 0);
        return;
    }
    VCALL(gNavMesh, 0x2C, void (*)(void *, u32, f32 *))(gNavMesh, h->c.a.navTri, n);
    if (n[1] == 1.0f) {
        hewie_want(h, 0, 0);
        return;
    }
    n[1] = 0.0f;
    sceVu0Normalize(dir, n);
    fwd[2] = 1.0f;
    fwd[0] = 0.0f;
    fwd[1] = 0.0f;
    fwd[3] = 0.0f;
    sceVu0ApplyMatrix(fwd, h->c.a.rot, fwd);
    if (HW(h, 0xF36B8, s32) == 0) {
        a = msl_atan2f(n[0], n[2]);
        if (sceVu0InnerProduct(fwd, dir) < 0.0f) {
            a = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + a);
        }
    } else {
        a = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + msl_atan2f(n[0], n[2]));
        Mtx_TurnY(m, a);
        v[2] = 1.0f;
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[3] = 0.0f;
        Mtx_ApplyVector(fwd, m, v);
        Mtx_ApplyVector(v, h->c.a.rot, v);
        if (!(sceVu0InnerProduct(fwd, v) <= 0.5f)) {
            hewie_want(h, 0, 0);
            return;
        }
    }
    HW(h, 0xF36C4, f32) = a;
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
    step = run_turn(h);
    if (step < 0x1.aceea00000000p-6f /* 0.02617994 */) {
        step = 0x1.aceea00000000p-6f /* 0.02617994 */;
    }
    if (Actor_TurnToward(&h->c.a, HW(h, 0xF36C4, f32), step) == 0.0f) {
        if (root_ahead(h, fwd) < 0.0f) {
            fwd[2] = 0.0f;
        }
        sceVu0ApplyMatrix(fwd, h->c.a.rot, fwd);
        sceVu0AddVector(pt, h->c.a.pos, fwd);
        if (Actor_TriTo(&h->c.a, pt, NAV_NONE) == NAV_NONE) {
            HW(h, 0xF36B8, s32) = 1;
        }
    }
}

/* when his bite landed (his progress hit bits 1 / 4, or a creature, +0x104): on the pursuer a
 * third of the time a grudge and +0xF3688 300 frames; his mood easing (+0xF35C4), sound 0x6C,
 * the bitten collected (+0x108) */
static void bite_landed(Hewie *h, Progress *p) {
    u8 hit = AT(p, 0x1020 + AT(h, 0x20, u8) * 0x10, u8);

    if (!(hit & 5) && h->c.unk104[0] == 0) {
        return;
    }
    if (hit & 4) {
        if (RNG01() < 0x1.555476p-2f /* 0.33333 */) {
            Hewie_ChangeFeeling(h, gCharPursuer, 1);
        }
        HW(h, 0xF3688, s16) = 300;
    }
    if (HW(h, 0xF35C4, s32) != 0) {
        HW(h, 0xF35C4, s32) -= 1;
        HW(h, 0xF35C8, s32) = 300;
    }
    HW(h, 0xF36B0, s32) = 0xFF;
    h->c.unk104[1] |= h->c.unk104[0];
    Actor_PlaySound(&h->c.a, 0x6C, 5, 0, 0, NULL);
}

/* ---- biting ---- */

extern const PTMF Hewie_State1980_ptmf;

/* turn toward `at` as he runs, head held level toward it */
static void aim_run(Hewie *h, const f32 *at) {
    f32 a = Actor_HeadingTo(&h->c.a, at);
    f32 step = run_turn(h);

    turn_toward(h, a, step);
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
}

/* rush his target (+0xF3544, in his room; else the default action) to bite it, for +0xF36B4
 * frames. When it was hit (his progress hit bits 1 / 4, or a bite landed, +0x104): maybe
 * (a third of the time, hitting the pursuer) a grudge, +0xF3688 300 frames, his mood easing
 * (+0xF35C4), sound 0x6C, the bitten collected (+0x108). Running (0x202) at it along the path
 * (pose 8 when the stride fails), aiming straight once on its triangle. Unless progress flags
 * 0x13 / 0x2B: Fiona (1) or the pursuer (4) in bite reach (progress +0x2C, bone 0x1F, 3) are
 * bitten (sound 0x68; 5, +5 on difficulty 1, +half with progress +0xA10; hard by Hewie_ReactionRoll
 * 10; then behaviour Hewie_State1980_ptmf), and the bite tested on the creatures (Hewie_Bite) */
/* 0x001523D0 */
void Hewie_StateRush(Hewie *h) {
    Progress *p;
    Character *t;
    s32 there, mask;
    u16 dmg;

    if (!in_his_room(h, HW(h, 0xF3544, Character *))) {
        hewie_want(h, 0, 0);
        return;
    }
    p = gProgress;
    bite_landed(h, p);
    HW(h, 0xF36B4, s32) -= 1;
    if (HW(h, 0xF36B4, s32) == 0) {
        hewie_want(h, 0, 0);
        return;
    }
    t = HW(h, 0xF3544, Character *);
    there = t->a.navTri == Actor_TriTo(&h->c.a, t->a.pos, NAV_NONE);
    if (!there && Hewie_PlanAndGo(h, t->a.navTri, HW(h, 0xF3544, Character *)->a.pos, 0, 1) != 0) {
        hewie_want(h, 0, 0);
        return;
    }
    if (Hewie_StepToPose(h, 5) == 0 && MOTION_ANIM(h->c.motion) != 0x202) {
        Motion_PlayTable(h->c.motion, 0x202, -1);
    }
    if (!there) {
        if (!(u8)Hewie_Stride(h)) {
            Hewie_KeepPose(h, 8);
        }
    } else {
        aim_run(h, HW(h, 0xF3544, Character *)->a.pos);
    }
    {
        Progress *q = gProgress;

        if (((u8)Progress_TestFlag(q, 0x13) | (u8)Progress_TestFlag(q, 0x2B)) != 0) {
            return;
        }
    }
    mask = 0;
    if (in_his_room(h, gCharPlayer) &&
        (u8)VCALL(p, 0x2C, s32 (*)(Progress *, u32, s32, s32, f32))(p, AT(h, 0x20, u8), 0x1F, 0, 3.0f) == 1) {
        mask = 1;
    }
    if (in_his_room(h, gCharPursuer) &&
        (u8)VCALL(p, 0x2C, s32 (*)(Progress *, u32, s32, s32, f32))(p, AT(h, 0x20, u8), 0x1F, 2, 3.0f) == 1) {
        mask = (mask | 4) & 0xFF;
    }
    if (mask != 0) {
        s32 hard;

        Hewie_MakeSound(h, 0x68);
        hard = (u8)Hewie_ReactionRoll(h, 10) == 1;
        dmg = 5;
        if ((u8)Progress_GetVar(p, 0x27) == 1) {
            dmg += 5;
        }
        if (AT(p, 0xA10, s32) != 0) {
            dmg += dmg >> 1;
        }
        Relation_Request(p, AT(h, 0x20, u8), mask, 1, dmg, hard ? -0x8000 : 0, 20.0f);
        Hewie_SetBehaviour(h, &Hewie_State1980_ptmf);
    }
    h->c.unk104[0] = Hewie_Bite(h, h->c.unk104[1], 5, 0x1F, 10.0f);
}

/* ---- closing in to bite ---- */

extern const PTMF Hewie_StateRush_ptmf;

/* close in on his target (+0xF3544, in his room and reachable; else the default action): once
 * standing and in his run (0x202), behaviour Hewie_StateRush_ptmf; running at it along the path (pose 8
 * when the stride fails), aiming straight once on its triangle */
/* 0x00152D60 */
void Hewie_StateCloseIn(Hewie *h) {
    Character *t;
    s32 there;

    if (!in_his_room(h, HW(h, 0xF3544, Character *))) {
        hewie_want(h, 0, 0);
        return;
    }
    t = HW(h, 0xF3544, Character *);
    there = t->a.navTri == Actor_TriTo(&h->c.a, t->a.pos, NAV_NONE);
    if (!there && Hewie_PlanAndGo(h, t->a.navTri, HW(h, 0xF3544, Character *)->a.pos, 0, 1) != 0) {
        hewie_want(h, 0, 0);
        return;
    }
    if (Hewie_StepToPose(h, 5) == 0) {
        if (MOTION_ANIM(h->c.motion) == 0x202) {
            Hewie_SetBehaviour(h, &Hewie_StateRush_ptmf);
        } else {
            Motion_PlayTable(h->c.motion, 0x202, -1);
        }
    }
    if (!there) {
        if (!(u8)Hewie_Stride(h)) {
            Hewie_KeepPose(h, 8);
        }
    } else {
        aim_run(h, HW(h, 0xF3544, Character *)->a.pos);
    }
}

/* ---- walking off ---- */

/* walk (pose 7) along heading +0xF36C4 for +0xF36B4 frames, head held level toward it (the
 * default action taken when they run out, the walk still finishing this frame) */
/* 0x00153440 */
void Hewie_StateWalkHeading(Hewie *h) {
    f32 step;

    HW(h, 0xF36B4, s32) -= 1;
    if (HW(h, 0xF36B4, s32) == 0) {
        hewie_want(h, 0, 0);
    }
    Hewie_KeepPose(h, 7);
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
    step = run_turn(h);
    turn_toward(h, HW(h, 0xF36C4, f32), step);
}

/* ---- finding his feet ---- */

extern const PTMF Hewie_StateWalkHeading_ptmf;

/* off the mesh: walk (pose 7) along +0xF36C4, picking the freest way near where his head points
 * every 30 frames (Hewie_BestHeading, 20, 30..150 degrees). Once his head (bone 0x1F) and the point 5
 * ahead of him are both on the mesh: 16 frames, behaviour Hewie_StateWalkHeading_ptmf */
/* 0x00153700 */
void Hewie_StateOffMesh(Hewie *h) {
    f32 head[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 step;

    Hewie_KeepPose(h, 7);
    if (HW(h, 0xF36B4, s32) != 0) {
        HW(h, 0xF36B4, s32) -= 1;
    } else {
        HW(h, 0xF36B4, s32) = 30;
        HW(h, 0xF36C4, f32) = Hewie_BestHeading(h, Angle_Wrap(h->c.a.angle[1] + AT(h->c.motion, 0x858, f32)), 20.0f,
                                            30, 150, 30);
    }
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
    step = run_turn(h);
    turn_toward(h, HW(h, 0xF36C4, f32), step);
    sceVu0CopyVector(head, Skel_Bone(MOTION_SKELETON(h->c.motion), 0x1F) + 12);
    if (Actor_TriTo(&h->c.a, head, NAV_NONE) == NAV_NONE) {
        return;
    }
    v[2] = 5.0f;
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[3] = 0.0f;
    sceVu0ApplyMatrix(v, h->c.a.rot, v);
    sceVu0AddVector(head, h->c.a.pos, v);
    if (Actor_TriTo(&h->c.a, head, NAV_NONE) != NAV_NONE) {
        HW(h, 0xF36B4, s32) = 16;
        Hewie_SetBehaviour(h, &Hewie_StateWalkHeading_ptmf);
    }
}

/* step away (pose 7) from `from`: every 30 frames the freest way near straight away from it
 * (Hewie_BestHeading, 20, 30..150 degrees), head held level toward it */
static void back_off(Hewie *h, Actor *from) {
    f32 away, step;

    Hewie_KeepPose(h, 7);
    away = Actor_HeadingTo(from, h->c.a.pos);
    if (HW(h, 0xF36B4, s32) != 0) {
        HW(h, 0xF36B4, s32) -= 1;
    } else {
        HW(h, 0xF36B4, s32) = 30;
        HW(h, 0xF36C4, f32) = Hewie_BestHeading(h, away, 20.0f, 30, 150, 30);
    }
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
    step = run_turn(h);
    turn_toward(h, HW(h, 0xF36C4, f32), step);
}

/* ---- making room ---- */

/* step away (pose 7) from his target (+0xF3544) while it stands within 6 of him in the same
 * room on the mesh (else the default action): every 30 frames the freest way near away from it
 * (Hewie_BestHeading, 20, 30..150 degrees) */
/* 0x00153D20 */
void Hewie_StateStepAwayTarget(Hewie *h) {
    Character *t = HW(h, 0xF3544, Character *);
    f32 d[4] __attribute__((aligned(16)));

    if (t == NULL || t->a.active == 0 || h->c.a.room != t->a.room || t->a.navTri == NAV_NONE) {
        hewie_want(h, 0, 0);
        return;
    }
    sceVu0SubVector(d, t->a.pos, h->c.a.pos);
    if (!(__builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) <= 6.0f)) {
        hewie_want(h, 0, 0);
        return;
    }
    back_off(h, &HW(h, 0xF3544, Character *)->a);
}

/* step away (pose 7) from Fiona while she is within 6 in his room (else the default action) */
/* 0x00154150 */
void Hewie_StateStepAwayFiona(Hewie *h) {
    if (!in_his_room(h, gCharPlayer) || !(Actor_Distance(&h->c.a, gCharPlayer->a.pos) <= 6.0f)) {
        hewie_want(h, 0, 0);
        return;
    }
    back_off(h, &gCharPlayer->a);
}

/* ---- leaving by an exit ---- */

/* walk (pose 7) his path out by the exit +0xF36B4: stepping by the root motion along it and
 * turning toward the point reached by 6 degrees. At its end: through the exit (Hewie_ThroughExit;
 * when that fails his remembered triangles +0x148C clear), +0x2B / +0x2D cleared, picked up if
 * down, and the default action */
/* 0x001545A0 */
void Hewie_StateWalkOut(Hewie *h) {
    f32 root[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 s, a;
    s32 i, n;
    u32 tri;

    if ((h->c.unk128 < h->c.unk124) == 1) {
        s = root_ahead(h, root);
        n = Character_WaypointAhead(&h->c, &tri, p, s < 0.0f ? 0.0f : s);
        Hewie_KeepPose(h, 7);
        a = Actor_HeadingTo(&h->c.a, p);
        turn_toward(h, a, 0x1.aceea00000000p-4f /* 0.10471976 */);
        h->c.a.navTri = tri;
        sceVu0CopyVector(h->c.a.pos, p);
        h->c.unk128 = n;
        HW(h, 0xF3558, u8) = 1;
        return;
    }
    h->c.a.unk2B = 0;
    h->c.a.unk2D = 0;
    if (Hewie_ThroughExit(h, HW(h, 0xF36B4, u8)) == 0) {
        for (i = 0; i < 13; i++) {
            h->c.unk148C[i] = 0;
        }
    }
    if (h->c.hp == 0) {
        h->c.hp = 1;
        h->c.a.unkC4 = 1;
    }
    hewie_want(h, 0, 0);
}

/* ---- squeezing through ---- */

/* squeeze through the gap of exit +0xF36B4 (mesh flag 0x20000) toward +0xF36E0: walking (pose 7)
 * straight at it by his root motion, turning to it, head held level. In the gap he is marked
 * (+0x2B) and blocked only by flag 8. Out of it (+0x2B cleared): done when +0xF36B8 runs out or
 * the rooms say the exit is passed (+0x70); and whenever the door no longer gives the way (state
 * bit 0x20): the default action */
/* 0x00154860 */
void Hewie_StateSqueeze(Hewie *h) {
    f32 dir[4] __attribute__((aligned(16)));
    f32 root[4] __attribute__((aligned(16)));
    f32 a, step;
    NavMesh *nm;

    a = Actor_HeadingTo(&h->c.a, &HW(h, 0xF36E0, f32));
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
    Hewie_KeepPose(h, 7);
    step = run_turn(h);
    turn_toward(h, a, step);
    nm = gNavMesh;
    /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
    if (NavMesh_TriFlags(nm, h->c.a.navTri) & 0x20000) {
        h->c.a.unk2B = 1;
        h->c.a.navMask = 8;
    }
    sceVu0SubVector(dir, &HW(h, 0xF36E0, f32), h->c.a.pos);
    sceVu0Normalize(dir, dir);
    sceVu0ScaleVector(dir, dir, root_ahead(h, root));
    Actor_Move(&h->c.a, dir);
    HW(h, 0xF3558, u8) = 1;
    if (HW(h, 0xF36B8, s32) != 0) {
        HW(h, 0xF36B8, s32) -= 1;
    }
    if (!(NavMesh_TriFlags(nm, h->c.a.navTri) & 0x20000)) {
        h->c.a.unk2B = 0;
        if (HW(h, 0xF36B8, s32) == 0 ||
            (u8)VCALL(gRooms, 0x70, s32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, HW(h, 0xF36B4, u8)) == 1) {
            hewie_want(h, 0, 0);
            return;
        }
    }
    if (!(PursuerGroup_Fields(gProgress, HW(h, 0xF36B4, u8), (u8)h->c.a.slot) & 0xFF & 0x20)) {
        hewie_want(h, 0, 0);
    }
}

/* ---- travelling between rooms ---- */

extern const PTMF Hewie_State1920_ptmf;

/* on his way to door +0x14C0 (actions 0x35 / 0x39 and the like): along his path when it is
 * shown (+0xF3590), else by the distance left (+0x14C4), at 0.38 a frame hurt, 1.6 walking, 10
 * running (0x39). Arrived: the door's exit in his room (+0x14D4; none: action 0x35, or his idle
 * choice in 0x35). A door that isn't open, or locked or closed to him, is remembered (+0x148C)
 * and he stops likewise; an exit flagged in the room's progress is left alone. Else through it:
 * into the room being played, placed at the exit facing in, running in (0x201) and either
 * barking (0x4F, by his trust's chance in 0x39) or carrying on (0x70), settled in as
 * Hewie_Arrive; into another room, off the mesh toward its next door (behaviour Hewie_State1920_ptmf) */
/* 0x00155000 */
void Hewie_StateToDoor(Hewie *h) {
    Progress *p;
    VObject *rooms;
    f32 p0[4] __attribute__((aligned(16)));
    f32 p1[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 ang;
    s32 arrived_ = 0;
    u32 exit, tri;

    if (HW(h, 0xF3590, u8) == 1) {
        Character_FollowWaypoints(&h->c, HEWIE_ACTION(h) == 0x39 ? 10.0f : h->c.a.unkC4 == 1 ? 0x1.851eb80000000p-2f /* 0.38 */ : 0x1.99999a0000000p+0f /* 1.6 */);
        if (h->c.unk128 >= h->c.unk124) {
            arrived_ = 1;
        }
    } else {
        f32 *left = (f32 *)&h->c.unk14C4;

        if (HEWIE_ACTION(h) == 0x39) {
            *left = *left - 10.0f;
        } else if (h->c.a.unkC4 == 1) {
            *left = *left - 0x1.851eb80000000p-2f /* 0.38 */;
        } else {
            *left = *left - 0x1.99999a0000000p+0f /* 1.6 */;
        }
        if (*left < 0.0f) {
            *left = 0.0f;
            arrived_ = 1;
        }
    }
    if ((u8)arrived_ != 1) {
        return;
    }
    rooms = gRooms;
    h->c.door = VCALL(rooms, 0x3C, u8 (*)(VObject *, u32, s32))(rooms, h->c.unk14C0, h->c.a.room);
    exit = h->c.door;
    if (exit == 0xFF) {
        HW(h, 0xF3590, u8) = 0;
        h->c.a.navTri = NAV_NONE;
        if (HEWIE_ACTION(h) == 0x35) {
            hewie_want(h, 0x35, 0);
        } else {
            Hewie_WhenIdle(h);
        }
        return;
    }
    p = gProgress;
    if ((u8)Progress_CurRoomFlag(p, h->c.a.room, exit) == 1) {
        return;
    }
    if ((u8)Progress_ExitOpen(p, h->c.a.room, exit) && (u8)Progress_ExitUnlocked(p, h->c.a.room, exit) != 1 &&
        (u8)Progress_ExitPassable(p, h->c.a.room, exit, AT(h, 0x20, u8))) {
        HW(h, 0xF3590, u8) = 0;
        Hewie_ThroughExit(h, exit);
        if (h->c.a.room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
            rooms = gRooms;
            tri = VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, h->c.door, p0);
            VCALL(rooms, 0x2C, void (*)(VObject *, u32, f32 *))(rooms, h->c.door, p1);
            sceVu0SubVector(d, p1, p0);
            ang = msl_atan2f(d[0], d[2]);
            {
                s32 bark = HEWIE_ACTION(h) == 0x39 && by_chance(h, D_003B1230);

                HW(h, 0xF36F0, s32) = 0x201;
                Hewie_PlaceAt(h, tri, &ang, p0);
                hewie_want(h, bark ? 0x4F : 0x70, 0);
            }
            arrived(h);
        } else {
            h->c.a.navTri = NAV_NONE;
            *(f32 *)&h->c.unk14C4 = (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, h->c.unk14C0, h->c.a.room);
            Hewie_SetBehaviour(h, &Hewie_State1920_ptmf);
        }
        return;
    }
    h->c.unk148C[h->c.unk14C0 >> 5] |= 1 << (h->c.unk14C0 & 0x1F);
    HW(h, 0xF3590, u8) = 0;
    h->c.a.navTri = NAV_NONE;
    if (HEWIE_ACTION(h) == 0x35) {
        hewie_want(h, 0x35, 0);
    } else {
        Hewie_WhenIdle(h);
    }
}

/* ---- landing ---- */

/* landing from a leap: unless already crouched (group 9), the landing animation 0x1E03 (once not
 * blending), its feet touching (+0x64, feet 2 / 3) letting the root motion carry him (+0xF36C0;
 * till then +0xF36C4 a frame); its end, the walk (0x201). In group 9 at the end: the default
 * action. Turning with his head all the while */
/* 0x00155A30 */
void Hewie_StateLanding(Hewie *h) {
    f32 v[4] __attribute__((aligned(16)));
    f32 root[4] __attribute__((aligned(16)));
    f32 a;

    if (Hewie_AnimGroup(h) != 9) {
        if (MOTION_ANIM(h->c.motion) != 0x1E03) {
            if (AT(h->c.motion, 0x550, f32) <= 0.0f) {
                h->c.a.unk2D = 0;
                Motion_PlayTable(h->c.motion, 0x1E03, -1);
            }
        } else {
            if (HW(h, 0xF36C0, s32) == 0) {
                u8 l = VCALL(h->c.motion, 0x64, s32 (*)(void *, s32, s32))(h->c.motion, 2, -1);
                u8 r = VCALL(h->c.motion, 0x64, s32 (*)(void *, s32, s32))(h->c.motion, 3, -1);

                if (l == 1 || r == 1) {
                    HW(h, 0xF36C0, s32) = 1;
                }
            }
            if (ANIM_DONE(h)) {
                h->c.a.unk2D = 0;
                Motion_Play(h->c.motion, 0x201, -1);
            }
        }
    } else if (ANIM_DONE(h)) {
        hewie_want(h, 0, 0);
    }
    a = Angle_Wrap(h->c.a.angle[1] + 0x1.921fb6p+1f /* pi */ * (2.0f * (0.5f * AT(h->c.motion, 0x858, f32))) / 180.0f);
    h->c.a.angle[1] = a;
    sceVu0UnitMatrix(h->c.a.rot);
    sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, a);
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[3] = 0.0f;
    v[2] = HW(h, 0xF36C0, s32) == 0 ? HW(h, 0xF36C4, f32) : root_ahead(h, root);
    sceVu0ApplyMatrix(v, h->c.a.rot, v);
    Actor_Move(&h->c.a, v);
    HW(h, 0xF3558, u8) = 1;
}

/* ---- biting in the air ---- */

extern const PTMF Hewie_StateLanding_ptmf2;

/* the leaping bite in flight: his bite's effects (bite_landed), forward +0xF36C4 a frame, rising
 * +0xF36C8 (less 0.5 a frame) from height +0xF36CC; below the ground (mesh +0x14): land
 * (0x1E03 unless blending, head ahead), +0xF356C top bit cleared, behaviour Hewie_StateLanding_ptmf2 */
/* 0x00155D00 */
void Hewie_StateLeapBite(Hewie *h) {
    f32 v[4] __attribute__((aligned(16)));
    f32 g[4] __attribute__((aligned(16)));

    bite_landed(h, gProgress);
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = HW(h, 0xF36C4, f32);
    v[3] = 0.0f;
    sceVu0ApplyMatrix(v, h->c.a.rot, v);
    Actor_Move(&h->c.a, v);
    HW(h, 0xF3558, u8) = 1;
    HW(h, 0xF36CC, f32) += HW(h, 0xF36C8, f32);
    HW(h, 0xF36C8, f32) -= 0.5f;
    h->c.a.pos[1] = HW(h, 0xF36CC, f32);
    sceVu0CopyVector(g, h->c.a.pos);
    VCALL(gNavMesh, 0x14, void (*)(void *, u32, f32 *))(gNavMesh, h->c.a.navTri, g);
    if (h->c.a.pos[1] < g[1]) {
        if (AT(h->c.motion, 0x550, f32) <= 0.0f) {
            Motion_PlayTable(h->c.motion, 0x1E03, -1);
        }
        if (HW(h, 0xF3604, s32) != 0) {
            HW(h, 0xF3604, s32) = 0;
            HW(h, 0xF3608, s32) = 10;
        }
        h->c.a.pos[1] = g[1];
        HW(h, 0xF356C, u32) &= 0x7FFFFFFF;
        Hewie_SetBehaviour(h, &Hewie_StateLanding_ptmf2);
    }
}

/* ---- the tackle ---- */

extern const PTMF Hewie_StateLeapBite_ptmf, Hewie_StateLanding_ptmf;

/* |wrap(c's heading - his)| as the original takes it (the wrap twice, 0 becomes -0) */
static f32 heading_gap(Hewie *h, Character *c) {
    if (!(Angle_Wrap(c->a.angle[1] - h->c.a.angle[1]) <= 0.0f)) {
        return Angle_Wrap(c->a.angle[1] - h->c.a.angle[1]);
    }
    return -Angle_Wrap(c->a.angle[1] - h->c.a.angle[1]);
}

/* a leaping tackle: its hits so far collected (his progress hit bits into +0xF36BC, and the
 * landed-bite effects); turning toward +0xF36D0 by 10 degrees, forward +0xF36C4 a frame, height
 * eased (sine) from +0xF36C8 to +0xF36E4 over +0xF36B8 frames (then behaviour Hewie_StateLeapBite_ptmf).
 * Unless progress flags 0x13 / 0x2B: the pursuer, else Fiona, not yet hit and in reach (progress
 * +0x2C, bone 0x1F, 3) takes it: 5 / 8 (from behind / not), 10 / 16 when charged (+0xF3585),
 * doubled on difficulty 1, +half with progress +0xA10; on the pursuer hard by Hewie_ReactionRoll
 * (6..9 by charge and side; else the frame count is left unset, taken as 0). The bite also tested
 * on the creatures. Below the ground: landed (0x1E03), behaviour Hewie_StateLanding_ptmf */
/* 0x00155FF0 */
void Hewie_StateTackle(Hewie *h) {
    Progress *p = gProgress;
    f32 v[4] __attribute__((aligned(16)));
    f32 g[4] __attribute__((aligned(16)));
    u8 hit = AT(p, 0x1020 + AT(h, 0x20, u8) * 0x10, u8);
    s32 mask, front, kind, frames, type;
    u16 dmg, base;

    if ((hit & 5) || h->c.unk104[0] != 0) {
        if (hit & 4) {
            if (RNG01() < 0x1.5554760000000p-2f /* 0.33333 */) {
                Hewie_ChangeFeeling(h, gCharPursuer, 1);
            }
            HW(h, 0xF3688, s16) = 300;
        }
        HW(h, 0xF36BC, s32) |= hit;
        if (HW(h, 0xF35C4, s32) != 0) {
            HW(h, 0xF35C4, s32) -= 1;
            HW(h, 0xF35C8, s32) = 300;
        }
        h->c.unk104[1] |= h->c.unk104[0];
        HW(h, 0xF36B0, s32) = 0xFF;
        Actor_PlaySound(&h->c.a, 0x6C, 5, 0, 0, NULL);
    }
    turn_toward(h, HW(h, 0xF36D0, f32), 0x1.6571860000000p-3f /* 0.17453294 */);
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = HW(h, 0xF36C4, f32);
    v[3] = 0.0f;
    sceVu0ApplyMatrix(v, h->c.a.rot, v);
    Actor_Move(&h->c.a, v);
    if (HW(h, 0xF36B4, s32) == HW(h, 0xF36B8, s32)) {
        h->c.a.pos[1] = HW(h, 0xF36E4, f32);
        HW(h, 0xF36C8, f32) = 0.0f;
        HW(h, 0xF36CC, f32) = h->c.a.pos[1];
        HW(h, 0xF36C0, s32) = 0;
        Hewie_SetBehaviour(h, &Hewie_StateLeapBite_ptmf);
    } else {
        f32 y0 = HW(h, 0xF36C8, f32);

        h->c.a.pos[1] = y0 + (HW(h, 0xF36E4, f32) - y0) *
                                 msl_sinf(0.5f * (0x1.921fb60000000p+1f /* 3.1415927 */ * ((f32)HW(h, 0xF36B4, s32) / (f32)HW(h, 0xF36B8, s32))));
        HW(h, 0xF36B4, s32) += 1;
    }
    {
        Progress *q = gProgress;

        HW(h, 0xF3558, u8) = 1;
        if (((u8)Progress_TestFlag(q, 0x13) | (u8)Progress_TestFlag(q, 0x2B)) == 0) {
            mask = 0;
            front = 1;
            frames = 0;
            if (in_his_room(h, gCharPursuer) && !(HW(h, 0xF36BC, s32) & 4) &&
                (u8)VCALL(p, 0x2C, s32 (*)(Progress *, u32, s32, s32, f32))(p, AT(h, 0x20, u8), 0x1F, 2, 3.0f) == 1) {
                mask = 4;
                if (heading_gap(h, gCharPursuer) < 0x1.921fb60000000p+0f /* 1.5707964 */) {
                    front = 0;
                    kind = HW(h, 0xF3585, u8) == 0 ? 7 : 9;
                } else {
                    kind = HW(h, 0xF3585, u8) == 0 ? 6 : 8;
                }
                if ((u8)Hewie_ReactionRoll(h, kind) == 1) {
                    frames = -0x8000;
                }
            }
            if (mask == 0 && in_his_room(h, gCharPlayer) && !(HW(h, 0xF36BC, s32) & 1) &&
                (u8)VCALL(p, 0x2C, s32 (*)(Progress *, u32, s32, s32, f32))(p, AT(h, 0x20, u8), 0x1F, 0, 3.0f) == 1) {
                mask = (mask | 1) & 0xFF;
                if (heading_gap(h, gCharPlayer) < 0x1.921fb60000000p+0f /* 1.5707964 */) {
                    front = 0;
                }
            }
            if (mask != 0) {
                if (HW(h, 0xF3585, u8) == 0) {
                    type = 1;
                    base = (u8)front == 1 ? 5 : 8;
                } else {
                    type = 2;
                    base = (u8)front == 1 ? 10 : 16;
                }
                dmg = base;
                if ((u8)Progress_GetVar(p, 0x27) == 1) {
                    dmg += base;
                }
                if (AT(p, 0xA10, s32) != 0) {
                    dmg += dmg >> 1;
                }
                Relation_Request(p, AT(h, 0x20, u8), mask, type, dmg, frames, 20.0f);
            }
            h->c.unk104[0] = Hewie_Bite(h, h->c.unk104[1], HW(h, 0xF3585, u8) != 0 ? 10 : 5, 0x1F, 10.0f);
        }
    }
    sceVu0CopyVector(g, h->c.a.pos);
    VCALL(gNavMesh, 0x14, void (*)(void *, u32, f32 *))(gNavMesh, h->c.a.navTri, g);
    if (h->c.a.pos[1] < g[1]) {
        h->c.a.pos[1] = g[1];
        if (AT(h->c.motion, 0x550, f32) <= 0.0f) {
            Motion_PlayTable(h->c.motion, 0x1E03, -1);
        }
        HW(h, 0xF356C, u32) &= 0x7FFFFFFF;
        HW(h, 0xF36C0, s32) = 0;
        Hewie_SetBehaviour(h, &Hewie_StateLanding_ptmf);
    }
}

/* ---- closing in to pounce ---- */

extern const PTMF Hewie_StateTackle_ptmf;

/* close on his target (+0xF3544; out of his room, out of time +0xF36B4, or out of reach: the
 * default action) running (0x202). Not blending and within 30 by the path: under 5, by its slot
 * the attack (Fiona 0x61, 2..5: 0x59 / 0x5B / 0x5D / 0x5F) while +0xF3560 lasts, else the
 * default action; else, both on plain ground (mesh flag 1 on either: action 0x6D) and facing it
 * (within 60 degrees) on its triangle: pounce (as Hewie_StateRunUpJump but aimed 5 above it, frames by
 * the distance at 2.8, behaviour Hewie_StateTackle_ptmf). Otherwise along the path (pose 8 when the stride
 * fails) */
/* 0x001569C0 */
void Hewie_StateCloseOnTarget(Hewie *h) {
    Character *t;
    f32 d[4] __attribute__((aligned(16)));
    f32 fwd[4] __attribute__((aligned(16)));
    f32 rest, a;
    u32 tri;

    if (!in_his_room(h, HW(h, 0xF3544, Character *))) {
        hewie_want(h, 0, 0);
        return;
    }
    HW(h, 0xF36B4, s32) -= 1;
    if (HW(h, 0xF36B4, s32) == 0) {
        hewie_want(h, 0, 0);
        return;
    }
    t = HW(h, 0xF3544, Character *);
    tri = t->a.navTri;
    if (Hewie_PlanAndGo(h, tri, t->a.pos, 0, 1) != 0) {
        hewie_want(h, 0, 0);
        return;
    }
    if (Hewie_StepToPose(h, 5) == 0 && MOTION_ANIM(h->c.motion) != 0x202) {
        Motion_PlayTable(h->c.motion, 0x202, -1);
    }
    if (AT(h->c.motion, 0x550, f32) <= 0.0f) {
        rest = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
            gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
        if (rest < 30.0f) {
            if (rest < 5.0f) {
                h->c.unk124 = h->c.unk128;
                if (HW(h, 0xF3560, s32) != 0) {
                    HW(h, 0xF3560, s32) -= 1;
                }
                if (HW(h, 0xF3560, s32) == 0) {
                    hewie_want(h, 0, 0);
                    return;
                }
                switch (HW(h, 0xF3544, Character *)->a.slot) {
                case 0:
                    hewie_want(h, 0x61, 0);
                    break;
                case 2:
                    hewie_want(h, 0x59, 0);
                    break;
                case 3:
                    hewie_want(h, 0x5B, 0);
                    break;
                case 4:
                    hewie_want(h, 0x5D, 0);
                    break;
                case 5:
                    hewie_want(h, 0x5F, 0);
                    break;
                }
                return;
            }
            /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
            if ((NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & 1) || (NavMesh_TriFlags(gNavMesh, tri) & 1)) {
                hewie_want(h, 0x6D, 0);
                return;
            }
            sceVu0SubVector(d, HW(h, 0xF3544, Character *)->a.pos, h->c.a.pos);
            d[1] = 0.0f;
            sceVu0Normalize(d, d);
            fwd[2] = 1.0f;
            fwd[0] = 0.0f;
            fwd[1] = 0.0f;
            fwd[3] = 0.0f;
            sceVu0ApplyMatrix(fwd, h->c.a.rot, fwd);
            if (!(sceVu0InnerProduct(fwd, d) <= 0.5f)) {
                if (HW(h, 0xF3604, s32) != 4) {
                    HW(h, 0xF3604, s32) = 4;
                    HW(h, 0xF3608, s32) = 10;
                }
                t = HW(h, 0xF3544, Character *);
                if (Actor_TriTo(&h->c.a, t->a.pos, NAV_NONE) != t->a.navTri) {
                    h->c.unk124 = h->c.unk128;
                    hewie_want(h, 0, 0);
                    return;
                }
                sceVu0CopyVector(&HW(h, 0xF36E0, f32), HW(h, 0xF3544, Character *)->a.pos);
                HW(h, 0xF36E4, f32) = 5.0f + HW(h, 0xF3544, Character *)->a.pos[1];
                HW(h, 0xF36B4, s32) = 1;
                HW(h, 0xF36B8, s32) = (s32)(rest / 0x1.6666660000000p+1f /* 2.8 */) + 1;
                HW(h, 0xF36BC, s32) = 0;
                HW(h, 0xF36C4, f32) = 0x1.6666660000000p+1f /* 2.8 */;
                HW(h, 0xF36C8, f32) = h->c.a.pos[1];
                HW(h, 0xF36CC, s32) = 0;
                a = Actor_HeadingTo(&h->c.a, HW(h, 0xF3544, Character *)->a.pos);
                HW(h, 0xF36D0, f32) = a;
                turn_toward(h, a, 0x1.0c15240000000p-1f /* 0.5235988 */);
                Motion_PlayTable(h->c.motion, 0x1E01, -1);
                HW(h, 0xF356C, u32) |= 0x80000000;
                h->c.a.unk2D = 1;
                Hewie_MakeSound(h, 0x68);
                h->c.unk104[0] = 0;
                h->c.unk104[1] = 0;
                Hewie_SetBehaviour(h, &Hewie_StateTackle_ptmf);
                return;
            }
        }
    }
    if (!(u8)Hewie_Stride(h)) {
        Hewie_KeepPose(h, 8);
    }
}

/* ---- dropping down ---- */

extern const PTMF Hewie_State18B0_ptmf2;

/* falling (+0xF36E0 a frame across, +0xF36C8 down from height +0xF36C4, faster by 0.5 a
 * frame); in actions 0x1F / 0x20 with the fall animation done, the hanging pose by +0x104 (1:
 * 0x2206, 2: 0x220F). Reaching the ground: landing animation (0x2212, or 0x2213 from a high
 * drop, +0xF36B4: in the room being played, on a splashing floor (mesh flags 0x02008000) the
 * splash sound and, in rooms 7 / 0x106, a print), behaviour Hewie_State18B0_ptmf2 */
/* 0x00157480 */
void Hewie_StateFalling(Hewie *h) {
    f32 y;

    if (HW(h, 0xF36B4, s32) == 0 && ANIM_DONE(h)) {
        s32 anim = MOTION_ANIM(h->c.motion);

        if (HEWIE_ACTION(h) == 0x20 || HEWIE_ACTION(h) == 0x1F) {
            switch (h->c.unk104[0]) {
            case 1:
                if (anim != 0x2206) {
                    Motion_Play(h->c.motion, 0x2206, -1);
                }
                break;
            case 2:
                /* (the original tests for 0x2211 but starts 0x220F) */
                if (anim != 0x2211) {
                    Motion_Play(h->c.motion, 0x220F, -1);
                }
                break;
            }
        }
    }
    Actor_Move(&h->c.a, &HW(h, 0xF36E0, f32));
    y = HW(h, 0xF36C4, f32) + HW(h, 0xF36C8, f32);
    HW(h, 0xF3558, u8) = 1;
    if (y < h->c.a.pos[1]) {
        if (HW(h, 0xF36B4, s32) == 0) {
            Motion_PlayTable(h->c.motion, 0x2212, -1);
        } else {
            Progress *p = gProgress;

            Motion_PlayTable(h->c.motion, 0x2213, -1);
            if ((u8)VCALL(p, 0x50, s32 (*)(Progress *))(p) == 1 &&
                h->c.a.room == VCALL(p, 0xC, s32 (*)(Progress *))(p) &&
                /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
                (NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & 0x02008000) == 0x02008000) {
                Actor_PlaySound(&h->c.a, 0x1E, 6, 0, 0, NULL);
                if (h->c.a.room == 7 || h->c.a.room == 0x106) {
                    Character_WaterStep(&h->c, h->c.a.pos, 1);
                }
            }
        }
        Hewie_SetBehaviour(h, &Hewie_State18B0_ptmf2);
    } else {
        h->c.a.pos[1] = y;
        HW(h, 0xF36C4, f32) = y;
        HW(h, 0xF36C8, f32) -= 0.5f;
    }
    HW(h, 0xF3582, u8) = 0;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* ---- hanging on ---- */

extern const PTMF Hewie_StateJumpArc_ptmf, Hewie_StateJumpArc_ptmf2;

/* hanging on to the pursuer by his jaws (actions 0x1F / 0x20; +0x104 1 or 2 the grip), moved by
 * the root motion (height kept in +0xF36C4). At each animation's end: the pursuer gone, the
 * default action; else a quarter of the time his grudge eases, and he holds on (Hewie_Dares 2:
 * 0x2204 / 0x220D) or is shaken off (+0x94 10, 0x2207 / 0x2210, +0xF36B4 1), flinging him along
 * the last root motion (+0xF36C8 up, +0xF36E0 across); behaviour Hewie_StateJumpArc_ptmf / Hewie_StateJumpArc_ptmf2 */
/* 0x001579C0 */
void Hewie_StateHanging(Hewie *h) {
    f32 root[4] __attribute__((aligned(16)));

    root_ahead(h, root);
    sceVu0ApplyMatrix(root, h->c.a.rot, root);
    Actor_Move(&h->c.a, root);
    HW(h, 0xF36C4, f32) += root[1];
    h->c.a.pos[1] = HW(h, 0xF36C4, f32);
    HW(h, 0xF3558, u8) = 1;
    if (ANIM_DONE(h)) {
        if (!in_his_room(h, gCharPursuer)) {
            hewie_want(h, 0, 0);
            return;
        }
        if (RNG01() < 0.25f) {
            Hewie_ChangeFeeling(h, gCharPursuer, -1);
        }
        if (HEWIE_ACTION(h) == 0x20 || HEWIE_ACTION(h) == 0x1F) {
            s32 grip = h->c.unk104[0];

            if (grip == 1 || grip == 2) {
                if ((u8)Hewie_Dares(h, 2) == 1) {
                    HW(h, 0xF36B4, s32) = 0;
                    Motion_Play(h->c.motion, grip == 1 ? 0x2204 : 0x220D, -1);
                } else {
                    HW(h, 0xF36B4, s32) = 1;
                    VCALL(h, 0x94, void (*)(Hewie *, s32))(h, 10);
                    Motion_Play(h->c.motion, grip == 1 ? 0x2207 : 0x2210, -1);
                }
                HW(h, 0xF36C8, f32) = root[1];
                HW(h, 0xF36E0, f32) = root[0];
                HW(h, 0xF36E4, s32) = 0;
                HW(h, 0xF36E8, f32) = root[2];
                Hewie_SetBehaviour(h, grip == 1 ? &Hewie_StateJumpArc_ptmf : &Hewie_StateJumpArc_ptmf2);
            }
        }
    }
    HW(h, 0xF3582, u8) = 0;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* ---- clamped on the pursuer ---- */

/* clamped on the pursuer: its bite effect (hard when charged, +0xF3585) at each event 0x20; at
 * the animation's end +0xF3688 300 frames and the default action. Moved by the root motion (level),
 * held at the pursuer's grip height (its motion +0x58): fixed (+0xF36B4 0) until event 2, then
 * easing off over 5 frames (+0xF36C4 0..1), then free (2) */
/* 0x00157E30 */
void Hewie_StateClamped(Hewie *h) {
    f32 y0, t, g;

    if (Motion_EventFlags(h->c.motion, 0, 0, 1) & 0xFF & 0x20) {
        Hewie_BiteEffect(h, HW(h, 0xF3585, u8) == 1 ? 1 : 0);
    }
    if (ANIM_DONE(h)) {
        HW(h, 0xF3688, s16) = 300;
        hewie_want(h, 0, 0);
    }
    slide_root(h);
    switch (HW(h, 0xF36B4, s32)) {
    case 0:
        if (Motion_EventFlags(h->c.motion, 0, 0, 1) & 0xFF & 2) {
            HW(h, 0xF36B4, s32) = 1;
        }
        HW(h, 0xF3582, u8) = 0;
        h->c.a.pos[1] = VCALL(gCharPursuer->motion, 0x58, f32 (*)(void *))(gCharPursuer->motion);
        break;
    case 1:
        HW(h, 0xF36C4, f32) += 0x1.99999a0000000p-3f /* 0.2 */;
        if (!(HW(h, 0xF36C4, f32) < 1.0f)) {
            HW(h, 0xF36C4, f32) = 2.0f;
        } else {
            HW(h, 0xF3582, u8) = 0;
            y0 = h->c.a.pos[1];
            t = HW(h, 0xF36C4, f32);
            g = VCALL(gCharPursuer->motion, 0x58, f32 (*)(void *))(gCharPursuer->motion);
            h->c.a.pos[1] = (1.0f - t) * g + y0 * t;
        }
        break;
    }
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* ---- biting and holding on ---- */


extern u32 gPadHeld;   /* pad buttons held */
extern const PTMF Hewie_StateClamped_ptmf, Hewie_StateClamped_ptmf2, Hewie_StateHanging_ptmf, Hewie_StateClamped_ptmf3, Hewie_StateHanging_ptmf2, Hewie_StateClamped_ptmf4;

/* one bite into the pursuer: `base` damage (doubled on difficulty 1, +half with progress
 * +0xA10), hard when Hewie_ReactionRoll `kind` */
static void bite_pursuer(Hewie *h, s32 kind, u16 base) {
    s32 hard = (u8)Hewie_ReactionRoll(h, kind) == 1;
    Progress *p = gProgress;
    u16 dmg = base;

    if ((u8)Progress_GetVar(p, 0x27) == 1) {
        dmg += base;
    }
    if (AT(p, 0xA10, s32) != 0) {
        dmg += dmg >> 1;
    }
    Relation_Request(p, AT(h, 0x20, u8), 4, 0xB, dmg, hard ? -0x8000 : 0, 0.0f);
}

/* the pursuer told to shake him off (its state block 7) */
static void shaken_off(Hewie *h) {
    HW(h, 0xF36B4, u32) = 2;
    post_state(gCharPursuer, 7, 0, 0);
}

/* biting into the pursuer and holding on (+0xF36B4: 0 / 1 biting, 2 let go, 3 thrown; his state
 * block at 7 throws him). The pursuer gone: the default action. Its bite effect at each event
 * 0x20 (hard when charged, +0xF3585). At each animation's end, while biting, a shake roll:
 * +0xF36B8 bites left, the chance to hold on +0xF36BC (+30 on the first, -10 each time held);
 * failing it, let go and the pursuer shakes. Then by his action and grip (+0x104) the bite
 * (0x21 / 0x22 / 0x75: 0x2218, 10; grip 1: 0x2201, 2 / 3; grip 2: 0x220A, 2 / 3; grip 0:
 * 0x2215, 3) or the let-go / thrown animation and behaviour. Between: let go when Fiona calls
 * (commands 0x27 / 0x2C) or the pursuer is down; under her direct control (+0x1FBEC1) let go on
 * cross, square presses (5) to keep biting. Always moved by the root motion at the pursuer's
 * grip height */
/* 0x001580F0 */
void Hewie_StateBiteHold(Hewie *h) {
    if (h->c.state[0] == 7) {
        HW(h, 0xF36B4, u32) = 3;
        h->c.state[0] = 0;
    }
    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (Motion_EventFlags(h->c.motion, 0, 0, 1) & 0xFF & 0x20) {
        Hewie_BiteEffect(h, HW(h, 0xF3585, u8) == 1 ? 1 : 0);
    }
    if (ANIM_DONE(h)) {
        if (HW(h, 0xF36B4, u32) < 2) {
            s32 hold;

            HW(h, 0xF36B8, s32) -= 1;
            if (HW(h, 0xF36B8, s32) != 0) {
                hold = HW(h, 0xF36BC, s32);
                if (HW(h, 0xF36B4, u32) == 0) {
                    HW(h, 0xF36B4, u32) = 1;
                    hold += 30;
                    HW(h, 0xF3714, s16) = 0;
                }
            } else {
                hold = 0;
            }
            if ((s32)(100.0f * RNG01()) < hold) {
                HW(h, 0xF36BC, s32) -= 10;
                if (HW(h, 0xF36BC, s32) < 0) {
                    HW(h, 0xF36BC, s32) = 0;
                }
            } else {
                shaken_off(h);
            }
        }
        switch (HEWIE_ACTION(h)) {
        case 0x75:
        case 0x22:
        case 0x21:
            switch (HW(h, 0xF36B4, u32)) {
            case 0:
            case 1:
                if (MOTION_ANIM(h->c.motion) != 0x2218) {
                    Motion_Play(h->c.motion, 0x2218, -1);
                }
                bite_pursuer(h, 5, 10);
                break;
            case 2:
                HW(h, 0xF36B4, u32) = 0;
                HW(h, 0xF36C4, f32) = 0.0f;
                Motion_Play(h->c.motion, 0x2219, -1);
                Hewie_SetBehaviour(h, &Hewie_StateClamped_ptmf);
                break;
            }
            break;
        case 0x20:
        case 0x1F:
            switch (h->c.unk104[0]) {
            case 1:
            case 2: {
                s32 g2 = h->c.unk104[0] == 2;

                switch (HW(h, 0xF36B4, u32)) {
                case 0:
                case 1:
                    if (MOTION_ANIM(h->c.motion) != (g2 ? 0x220A : 0x2201)) {
                        Motion_Play(h->c.motion, g2 ? 0x220A : 0x2201, -1);
                    }
                    if (g2) {
                        bite_pursuer(h, HW(h, 0xF3585, u8) == 0 ? 0 : 1, HW(h, 0xF3585, u8) == 0 ? 2 : 3);
                    } else {
                        bite_pursuer(h, HW(h, 0xF3585, u8) == 0 ? 2 : 3, HW(h, 0xF3585, u8) == 0 ? 2 : 3);
                    }
                    break;
                case 2:
                    HW(h, 0xF36B4, u32) = 0;
                    HW(h, 0xF36C4, f32) = 0.0f;
                    Motion_Play(h->c.motion, g2 ? 0x220B : 0x2202, -1);
                    Hewie_SetBehaviour(h, g2 ? &Hewie_StateClamped_ptmf3 : &Hewie_StateClamped_ptmf2);
                    break;
                case 3:
                    Motion_Play(h->c.motion, g2 ? 0x220C : 0x2203, -1);
                    HW(h, 0xF36C4, f32) = h->c.a.pos[1];
                    h->c.moveMode = 4;
                    h->c.moveSub = 10;
                    Hewie_SetBehaviour(h, g2 ? &Hewie_StateHanging_ptmf2 : &Hewie_StateHanging_ptmf);
                    break;
                }
                break;
            }
            case 0:
                switch (HW(h, 0xF36B4, u32)) {
                case 0:
                case 1:
                    if (MOTION_ANIM(h->c.motion) != 0x2215) {
                        Motion_Play(h->c.motion, 0x2215, -1);
                    }
                    bite_pursuer(h, 4, 3);
                    break;
                case 2:
                    HW(h, 0xF36B4, u32) = 0;
                    HW(h, 0xF36C4, f32) = 0.0f;
                    Motion_Play(h->c.motion, 0x2216, -1);
                    Hewie_SetBehaviour(h, &Hewie_StateClamped_ptmf4);
                    break;
                }
                break;
            }
            break;
        }
    } else {
        s32 let_go = 0;

        if (*((u8 *)gProgress + 0x1FBEC1) == 0) {
            if (HW(h, 0xF36B4, u32) < 2) {
                s32 cmd = HW(h, 0xF3578, s32);

                if (cmd == 0x27 || cmd == 0x2C || gCharPursuer->hp == 0) {
                    let_go = 1;
                } else if (HW(h, 0xF36B4, u32) == 1 && cmd == 0x2E) {
                    HW(h, 0xF36B4, u32) = 0;
                }
            }
        } else if (HW(h, 0xF36B4, u32) < 2) {
            if ((gPadHeld & PAD_CROSS) || gCharPursuer->hp == 0) {
                let_go = 1;
            } else {
                if (gPadPressed & PAD_SQUARE) {
                    HW(h, 0xF3714, s16) += 1;
                }
                if (HW(h, 0xF36B4, u32) == 1 && HW(h, 0xF3714, s16) >= 5) {
                    HW(h, 0xF36B4, u32) = 0;
                }
            }
        }
        if ((u8)let_go == 1 && gCharPursuer->state[0] != 4) {
            shaken_off(h);
        }
    }
    slide_root(h);
    HW(h, 0xF3581, u8) = 1;
    HW(h, 0xF3582, u8) = 0;
    h->c.a.pos[1] = VCALL(gCharPursuer->motion, 0x58, f32 (*)(void *))(gCharPursuer->motion);
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* ---- sinking his teeth in ---- */

extern const PTMF Hewie_StateBiteHold_ptmf;

/* the leap lands on the pursuer (the pursuer gone: the default action): airborne meanwhile
 * (turning toward +0xF36CC by +0xF36D0, +0xF36C4 along the way). At the animation's end the
 * first bite: by his action and grip the hard-bite test and damage (0x21 / 0x22: 5, 30; grip 1:
 * 2 / 3 charged, 5 / 10; grip 2: 0 / 1, 5 / 10; grip 0: 4, 10), its effect, sound 0x6B; put at the grip spot +0x110 (triangle
 * +0x108) facing +0xF36CC, the bite animation (0x2218 / 0x2201 / 0x220A / 0x2215), biting
 * (+0xF36B4 1) with the hold chance 5 x (10 + his feeling about it), behaviour Hewie_StateBiteHold_ptmf. At
 * the pursuer's grip height throughout */
/* 0x00159040 */
void Hewie_StateLeapLands(Hewie *h) {
    f32 p[4] __attribute__((aligned(16)));
    s32 kind = 0;   /* (kind and base unset in the original for other actions or grips) */
    u16 base = 0;
    u32 tri;

    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (ANIM_DONE(h)) {
        f32 a;
        s16 f = 0;

        switch (HEWIE_ACTION(h)) {
        case 0x22:
        case 0x21:
            base = 30;
            kind = 5;
            break;
        case 0x20:
        case 0x1F:
            switch (h->c.unk104[0]) {
            case 1:
                base = HW(h, 0xF3585, u8) == 1 ? 10 : 5;
                kind = HW(h, 0xF3585, u8) == 1 ? 3 : 2;
                break;
            case 2:
                base = HW(h, 0xF3585, u8) == 1 ? 10 : 5;
                kind = HW(h, 0xF3585, u8) == 1 ? 1 : 0;
                break;
            case 0:
                base = 10;
                kind = 4;
                break;
            }
            break;
        }
        bite_pursuer(h, kind, base);
        Hewie_BiteEffect(h, HW(h, 0xF3585, u8) == 1 ? 1 : 0);
        Actor_PlaySound(&h->c.a, 0x6B, 5, 0, 0, NULL);
        h->c.a.navTri = h->c.unk104[1];
        sceVu0CopyVector(h->c.a.pos, h->c.unk110);
        VCALL(gNavMesh, 0x14, void (*)(void *, u32, f32 *))(gNavMesh, h->c.a.navTri, h->c.a.pos);
        a = HW(h, 0xF36CC, f32);
        h->c.a.angle[1] = a;
        sceVu0UnitMatrix(h->c.a.rot);
        sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, a);
        switch (HEWIE_ACTION(h)) {
        case 0x22:
        case 0x21:
            Motion_Play(h->c.motion, 0x2218, -1);
            break;
        case 0x20:
        case 0x1F:
            switch (h->c.unk104[0]) {
            case 1:
                Motion_Play(h->c.motion, 0x2201, -1);
                break;
            case 2:
                Motion_Play(h->c.motion, 0x220A, -1);
                break;
            case 0:
                Motion_Play(h->c.motion, 0x2215, -1);
                break;
            }
            break;
        }
        HW(h, 0xF36B4, s32) = 1;
        if (gCharPursuer != NULL && gCharPursuer->a.active == 1) {
            s16 *v = Hewie_Feeling(h, gCharPursuer->unk153C);

            f = v != NULL ? *v : 0;
        }
        HW(h, 0xF36BC, s32) = (f + 10) * 5;
        HW(h, 0xF3714, s16) = 0;
        Hewie_SetBehaviour(h, &Hewie_StateBiteHold_ptmf);
    } else {
        Actor_TurnToward(&h->c.a, HW(h, 0xF36CC, f32), HW(h, 0xF36D0, f32));
        tri = h->c.a.navTri;
        Character_WaypointAhead(&h->c, &tri, p, HW(h, 0xF36C4, f32));
        h->c.a.navTri = tri;
        sceVu0CopyVector(h->c.a.pos, p);
    }
    HW(h, 0xF3558, u8) = 1;
    HW(h, 0xF3581, u8) = 1;
    HW(h, 0xF3582, u8) = 0;
    h->c.a.pos[1] = VCALL(gCharPursuer->motion, 0x58, f32 (*)(void *))(gCharPursuer->motion);
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* ---- leaping to bite ---- */

extern const char str_Dog_Bite_Enemy_No_Route[];   /* "Dog Bite Enemy -> No Route" */
extern const s16 D_003B1310[8], D_003B1320[8], D_003B1330[8], D_003B1340[8];   /* bites, by trust */
extern const PTMF Hewie_StateLeapLands_ptmf;

/* leap to bite the pursuer at the grip spot +0x110 (triangle +0x108; no way: a debug message and
 * the default action; the pursuer gone: the default action; his state block at 7: action 0x5A).
 * The bites he gets (+0xF36B8) by his action, grip and trust (0x1F: grip 0 D_003B1330, else
 * D_003B1310 / D_003B1320 charged; 0x21: D_003B1340; 0x20 / 0x22: 1); his mood easing, sub-move
 * 0x19, a third of the time a grudge, the pursuer told (Hewie_CountEncounter). The leap animation (0x2217;
 * grip 1: 0x2200 turned about unless it is kind 0xB, grip 2: 0x2209 turned about, grip 0:
 * 0x2214) toward +0x10C, covering the path and the turn in 12 frames (0x21 / 0x22) or 8; sound
 * 0x68, behaviour Hewie_StateLeapLands_ptmf */
/* 0x00159770 */
void Hewie_StateLeapGrip(Hewie *h) {
    f32 a, rest, d;

    if (h->c.state[0] == 7) {
        h->c.state[0] = 0;
        hewie_want(h, 0x5A, 0);
        return;
    }
    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (Hewie_PlanAndGo(h, h->c.unk104[1], h->c.unk110, 0, 1) != 0) {
        msl_printf(str_Dog_Bite_Enemy_No_Route);
        hewie_want(h, 0, 0);
        return;
    }
    switch (HEWIE_ACTION(h)) {
    case 0x1F:
        if (h->c.unk104[0] != 0) {
            HW(h, 0xF36B8, s32) = (HW(h, 0xF3585, u8) == 1 ? D_003B1320 : D_003B1310)[HW(h, 0xF35CC, s16)];
        } else {
            HW(h, 0xF36B8, s32) = D_003B1330[HW(h, 0xF35CC, s16)];
        }
        break;
    case 0x21:
        HW(h, 0xF36B8, s32) = D_003B1340[HW(h, 0xF35CC, s16)];
        break;
    case 0x22:
    case 0x20:
        HW(h, 0xF36B8, s32) = 1;
        break;
    }
    HW(h, 0xF36B0, s32) = 0xFF;
    if (HW(h, 0xF35C4, s32) != 0) {
        HW(h, 0xF35C4, s32) -= 1;
        HW(h, 0xF35C8, s32) = 300;
    }
    h->c.moveSub = 0x19;
    if (RNG01() < 0x1.5554760000000p-2f /* 0.33333 */) {
        Hewie_ChangeFeeling(h, gCharPursuer, 1);
    }
    Hewie_CountEncounter(h, gCharPursuer);
    a = HW(h, 0x10C, f32);
    switch (HEWIE_ACTION(h)) {
    case 0x22:
    case 0x21:
        Motion_PlayTable(h->c.motion, 0x2217, -1);
        break;
    case 0x20:
    case 0x1F:
        switch (h->c.unk104[0]) {
        case 1:
            if (gCharPursuer->unk153C != 0xB) {
                a = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + a);
            }
            Motion_PlayTable(h->c.motion, 0x2200, -1);
            break;
        case 2:
            a = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + a);
            Motion_PlayTable(h->c.motion, 0x2209, -1);
            break;
        case 0:
            Motion_PlayTable(h->c.motion, 0x2214, -1);
            break;
        }
        break;
    }
    rest = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
        gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
    d = Angle_Wrap(a - h->c.a.angle[1]);
    HW(h, 0xF36CC, f32) = a;
    if ((u32)(HEWIE_ACTION(h) - 0x21) < 2) {
        HW(h, 0xF36C4, f32) = 0x1.5551d60000000p-4f /* 0.08333 */ * rest;
        if (d <= 0.0f) {
            d = -d;
        }
        HW(h, 0xF36D0, f32) = 0x1.5551d60000000p-4f /* 0.08333 */ * d;
    } else {
        HW(h, 0xF36C4, f32) = 0.125f * rest;
        if (d <= 0.0f) {
            d = -d;
        }
        HW(h, 0xF36D0, f32) = 0.125f * d;
    }
    HW(h, 0xF3558, u8) = 1;
    h->c.a.unk2D = 0;
    HW(h, 0xF3581, u8) = 1;
    Hewie_MakeSound(h, 0x68);
    Hewie_SetBehaviour(h, &Hewie_StateLeapLands_ptmf);
}

/* ---- going for the pursuer ---- */

extern const PTMF Hewie_StateGoForPursuer2_ptmf;

/* he goes for the pursuer (in his room, unless progress flags 0x13 / 0x2B: else the default
 * action): with no grip yet (+0x104 -1) the side he comes from (Hewie_AttackSide) picks it (head
 * on: grip 0 for 0x1F / 0x20; the flanks 1 / 2, action 0x1F (0x1F / 0x21) or 0x20 (0x20 / 0x22));
 * then the noise of the attack for the game by action and grip (frames 15 for 0x21 / 0x22; grip 1
 * 10 / 11 charged, grip 2 12 / 13, grip 0 14), behaviour Hewie_StateGoForPursuer2_ptmf */
/* 0x00159F90 */
void Hewie_StateGoesForPursuer(Hewie *h) {
    Progress *p;
    s32 frames = -1;

    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    p = gProgress;
    if ((((u8)Progress_TestFlag(p, 0x13) | (u8)Progress_TestFlag(p, 0x2B)) != 0) == 1) {
        hewie_want(h, 0, 0);
        return;
    }
    if (h->c.unk104[0] == -1) {
        s32 side = Hewie_AttackSide(h);

        switch (side) {
        case 0:
            if (HEWIE_ACTION(h) == 0x20 || HEWIE_ACTION(h) == 0x1F) {
                h->c.unk104[0] = 0;
            }
            break;
        case 1:
        case 2:
            switch (HEWIE_ACTION(h)) {
            case 0x1F:
            case 0x21:
                HEWIE_ACTION(h) = 0x1F;
                break;
            case 0x20:
            case 0x22:
                HEWIE_ACTION(h) = 0x20;
                break;
            }
            h->c.unk104[0] = side;
            break;
        }
    }
    switch (HEWIE_ACTION(h)) {
    case 0x22:
    case 0x21:
        frames = 15;
        break;
    case 0x20:
    case 0x1F:
        switch (h->c.unk104[0]) {
        case 1:
            frames = HW(h, 0xF3585, u8) == 1 ? 11 : 10;
            break;
        case 2:
            frames = HW(h, 0xF3585, u8) == 1 ? 13 : 12;
            break;
        case 0:
            frames = 14;
            break;
        }
        break;
    }
    if (frames >= 0) {
        Progress_GetVar(p, 0x27);
        Relation_Request(p, AT(h, 0x20, u8), 4, 9, 0, frames, 0.0f);
    }
    HW(h, 0xF3558, u8) = 1;
    Hewie_SetBehaviour(h, &Hewie_StateGoForPursuer2_ptmf);
}

/* ---- running at the pursuer ---- */

extern const PTMF Hewie_StateGoesForPursuer_ptmf;

/* run at the pursuer (in his room and reachable; else the default action), trotting or running
 * by the stride; within 23 of it: head level (+0xF3604 4), marked (+0x2D, +0xF356C top bit),
 * behaviour Hewie_StateGoesForPursuer_ptmf */
/* 0x0015A460 */
void Hewie_StateRunAtPursuer(Hewie *h) {
    s32 anim;

    if (!in_his_room(h, gCharPursuer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (Actor_Distance(&h->c.a, gCharPursuer->a.pos) < 23.0f) {
        h->c.a.unk2D = 1;
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 0;
        HW(h, 0xF356C, u32) |= 0x80000000;
        Hewie_SetBehaviour(h, &Hewie_StateGoesForPursuer_ptmf);
        return;
    }
    if (h->c.unk128 >= h->c.unk124 && Hewie_PlanAndGo(h, gCharPursuer->a.navTri, gCharPursuer->a.pos, 0, 0) != 0) {
        hewie_want(h, 0, 0);
        return;
    }
    anim = MOTION_ANIM(h->c.motion);
    if ((u8)Hewie_Stride(h) == 1) {
        if (anim != 0x202) {
            Motion_PlayTable(h->c.motion, 0x202, -1);
        }
    } else if (anim != 0x201) {
        Motion_PlayTable(h->c.motion, 0x201, -1);
    }
}

/* ---- leaving by a door ---- */

/* go to the point +0xF36E0 (triangle +0xF36B4) by the exit +0x100 (pose by +0xF36BC: walk,
 * trot, run; along the path, pose 8 when the stride fails; straight once on its triangle; no
 * way: the default action). When the exit's door is open to him and he is past the point:
 * through it (unless there is no door), remembering the door (+0x148C) when Fiona is in his
 * room but out of his reach; then the default action */
/* 0x0015A720 */
void Hewie_StateGoToExit(Hewie *h) {
    Progress *p;
    VObject *rooms;
    f32 door[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 c[4] __attribute__((aligned(16)));
    s32 there;
    u32 d;

    there = HW(h, 0xF36B4, s32) == (s32)Actor_TriTo(&h->c.a, &HW(h, 0xF36E0, f32), NAV_NONE);
    if (!there && h->c.unk128 >= h->c.unk124 && Hewie_PlanAndGo(h, HW(h, 0xF36B4, s32), &HW(h, 0xF36E0, f32), 0, 1) != 0) {
        hewie_want(h, 0, 0);
        return;
    }
    switch (HW(h, 0xF36BC, s32)) {
    case 0:
        Hewie_KeepPose(h, 7);
        break;
    case 1:
        Hewie_KeepPose(h, 8);
        break;
    case 2:
        Hewie_KeepPose(h, 9);
        break;
    }
    if (!there) {
        if (!(u8)Hewie_Stride(h)) {
            Hewie_KeepPose(h, 8);
        }
    } else {
        run_straight(h, &HW(h, 0xF36E0, f32));
    }
    p = gProgress;
    if (!(PursuerGroup_Fields(p, (u8)h->c.unk100, (u8)h->c.a.slot) & 0xFF & 1)) {
        return;
    }
    rooms = gRooms;
    VCALL(rooms, 0x2C, void (*)(VObject *, u32, f32 *))(rooms, (u8)h->c.unk100, door);
    sceVu0SubVector(c, &HW(h, 0xF36E0, f32), door);
    sceVu0SubVector(b, h->c.a.pos, door);
    if (!(sceVu0InnerProduct(c, c) < sceVu0InnerProduct(b, b))) {
        return;
    }
    d = VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, h->c.a.room, (u8)h->c.unk100) & 0xFFFF;
    if (d != 0xFFFF) {
        if (in_his_room(h, gCharPlayer) && !(u8)Hewie_PlanTo(h, gCharPlayer->a.navTri, gCharPlayer->a.pos)) {
            h->c.unk148C[d >> 5] |= 1 << (d & 0x1F);
        }
        Hewie_ThroughExit(h, (u8)h->c.unk100);
    }
    hewie_want(h, 0, 0);
}

/* ---- tricks ---- */

extern const PTMF Hewie_StateAnimOver2_ptmf4, Hewie_StateAnimOver2_ptmf5, Hewie_StateAnimOver2_ptmf6, Hewie_StateAnimOver2_ptmf7, Hewie_StateAnimOver2_ptmf8;

/* the tricks: once in the pose they need, their animation and behaviour. 0x18 sitting (0x1C06,
 * Hewie_StateAnimOver2_ptmf4; from lying with +0xF36B4: 0x1C07, Hewie_StateAnimOver2_ptmf5); 0x19 sitting (0x1C00, Hewie_StateAnimOver2_ptmf6);
 * 0x1A / 0x1B standing (0x1C08 / 0x1C09, Hewie_StateAnimOver2_ptmf7 / Hewie_StateAnimOver2_ptmf8); 0x1C: sit (0x106), then lie
 * down (8) and stay for +0xF355C, then the default action */
/* 0x0015AE10 */
void Hewie_StateTricks(Hewie *h) {
    switch (HEWIE_ACTION(h)) {
    case 0x18:
        if (HW(h, 0xF36B4, s32) == 0) {
            if (Hewie_StepToPose(h, 1) == 0) {
                Motion_PlayTable(h->c.motion, 0x1C06, -1);
                Hewie_SetBehaviour(h, &Hewie_StateAnimOver2_ptmf4);
            }
        } else if (Hewie_StepToPose(h, 2) == 0) {
            Motion_PlayTable(h->c.motion, 0x1C07, -1);
            Hewie_SetBehaviour(h, &Hewie_StateAnimOver2_ptmf5);
        }
        break;
    case 0x19:
        if (Hewie_StepToPose(h, 1) == 0) {
            Motion_PlayTable(h->c.motion, 0x1C00, -1);
            Hewie_SetBehaviour(h, &Hewie_StateAnimOver2_ptmf6);
        }
        break;
    case 0x1C:
        switch (MOTION_ANIM(h->c.motion)) {
        case 0x106:
            if (ANIM_DONE(h)) {
                Motion_PlayTable(h->c.motion, 8, -1);
            }
            break;
        case 8:
            if (HW(h, 0xF355C, s32) == 0 && ANIM_DONE(h)) {
                hewie_want(h, 0, 0);
            }
            break;
        default:
            if (Hewie_StepToPose(h, 1) == 0) {
                Motion_PlayTable(h->c.motion, 0x106, -1);
            }
            break;
        }
        break;
    case 0x1A:
        if (Hewie_StepToPose(h, 0) == 0) {
            Motion_PlayTable(h->c.motion, 0x1C08, -1);
            Hewie_SetBehaviour(h, &Hewie_StateAnimOver2_ptmf7);
        }
        break;
    case 0x1B:
        if (Hewie_StepToPose(h, 0) == 0) {
            Motion_PlayTable(h->c.motion, 0x1C09, -1);
            Hewie_SetBehaviour(h, &Hewie_StateAnimOver2_ptmf8);
        }
        break;
    }
}

/* ---- coming to Fiona ---- */

/* come where her command puts him by her (her in his room; else the default action): along the
 * path (pose 8 when the stride fails; no way: the default action), straight once on its
 * triangle (his head off the mesh then: the default action); gait by the distance left (walk
 * under 20, trot under 44, else run) */
/* 0x0015B130 */
void Hewie_StateComeToCommand(Hewie *h) {
    f32 at[4] __attribute__((aligned(16)));
    f32 head[4] __attribute__((aligned(16)));
    f32 d;
    u32 tri;

    if (!in_his_room(h, gCharPlayer)) {
        hewie_want(h, 0, 0);
        return;
    }
    tri = Hewie_CommandPlace(h, HEWIE_ACTION(h), at);
    if (tri != Actor_TriTo(&h->c.a, at, NAV_NONE)) {
        if (Hewie_PlanAndGo(h, tri, at, 0, 1) != 0) {
            hewie_want(h, 0, 0);
            return;
        }
        d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
            gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
        if (!(u8)Hewie_Stride(h)) {
            Hewie_KeepPose(h, 8);
        }
    } else {
        aim_run(h, at);
        sceVu0CopyVector(head, Skel_Bone(MOTION_SKELETON(h->c.motion), 0x1F) + 12);
        if (Actor_TriTo(&h->c.a, head, NAV_NONE) == NAV_NONE) {
            hewie_want(h, 0, 0);
            return;
        }
        d = Actor_Distance(&h->c.a, at);
    }
    Hewie_KeepPose(h, d < 20.0f ? 7 : d < 44.0f ? 8 : 9);
}

/* ---- roaming ---- */

/* roam: along heading +0xF36C4 while the way ahead (Actor_FreeDistance) is at least +0xF36C8; every
 * 90..240 frames, or when it is shorter, a new heading (the freest near where his head points,
 * Hewie_BestHeading 30..150 degrees for 10 more), held 30 frames before rechecking. Facing it (within
 * 5 degrees) at a walk he looks about (+0xF3604 2), else ahead. Gait +0xF36BC (walk / trot /
 * run), slower where the way ahead is short (under 10 / 20) */
/* 0x0015B660 */
void Hewie_StateRoam(Hewie *h) {
    f32 room, step, left;

    room = Actor_FreeDistance(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, HW(h, 0xF36C4, f32), HW(h, 0xF36C8, f32));
    if (HW(h, 0xF36B8, s32) != 0) {
        HW(h, 0xF36B8, s32) -= 1;
    } else {
        if (HW(h, 0xF36B4, s32) != 0) {
            HW(h, 0xF36B4, s32) -= 1;
        }
        if (HW(h, 0xF36B4, s32) == 0 || room < HW(h, 0xF36C8, f32)) {
            HW(h, 0xF36B4, s32) = (s32)(6.0f * RNG01()) * 30 + 90;
            HW(h, 0xF36B8, s32) = 30;
            HW(h, 0xF36C4, f32) = Hewie_BestHeading(h, Angle_Wrap(h->c.a.angle[1] + AT(h->c.motion, 0x858, f32)),
                                                10.0f + HW(h, 0xF36C8, f32), 30, 150, 30);
        }
    }
    step = run_turn(h);
    left = turn_toward(h, HW(h, 0xF36C4, f32), step);
    if (left < 0x1.6571860000000p-4f /* 0.08726647 */ && HW(h, 0xF36BC, s32) == 0) {
        if (HW(h, 0xF3604, s32) != 2) {
            HW(h, 0xF3604, s32) = 2;
            HW(h, 0xF3608, s32) = 10;
        }
    } else {
        if (HW(h, 0xF3604, s32) != 8) {
            HW(h, 0xF3604, s32) = 8;
            HW(h, 0xF3608, s32) = 10;
        }
        HW(h, 0xF3614, f32) = 0.0f;
        HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
    }
    if (HW(h, 0xF36BC, s32) == 0 || room < 10.0f) {
        Hewie_KeepPose(h, 7);
    } else if (HW(h, 0xF36BC, s32) == 1 || room < 20.0f) {
        Hewie_KeepPose(h, 8);
    } else {
        Hewie_KeepPose(h, 9);
    }
}

/* ---- scrambling about ---- */

extern const PTMF Hewie_State1770_ptmf;

/* scramble about (animation 0x204) for +0xF355C frames, then head ahead and behaviour
 * Hewie_State1770_ptmf: every 10..30 frames, or when the way ahead is under +0xF36C8, a new heading
 * (Hewie_BestHeading from his heading, 25, 150 down to 30 degrees), held 30 frames; range 15 after */
/* 0x0015BD90 */
void Hewie_StateScramble(Hewie *h) {
    f32 step;

    if (HW(h, 0xF355C, s32) == 0) {
        if (HW(h, 0xF3604, s32) != 4) {
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 10;
        }
        Hewie_SetBehaviour(h, &Hewie_State1770_ptmf);
        return;
    }
    if (HW(h, 0xF36B8, s32) == 0) {
        f32 room;

        if (HW(h, 0xF36B4, s32) != 0) {
            HW(h, 0xF36B4, s32) -= 1;
        }
        room = Actor_FreeDistance(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, HW(h, 0xF36C4, f32), HW(h, 0xF36C8, f32));
        if (HW(h, 0xF36B4, s32) == 0 || room < HW(h, 0xF36C8, f32)) {
            HW(h, 0xF36B4, s32) = (s32)(3.0f * RNG01()) * 10 + 10;
            HW(h, 0xF36B8, s32) = 30;
            HW(h, 0xF36C8, f32) = 15.0f;
            HW(h, 0xF36C4, f32) = Hewie_BestHeading(h, h->c.a.angle[1], 10.0f + 15.0f, 150, 30, 30);
        }
    } else {
        HW(h, 0xF36B8, s32) -= 1;
    }
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
    step = run_turn(h);
    turn_toward(h, HW(h, 0xF36C4, f32), step);
    if (Hewie_StepToPose(h, 5) == 0 && MOTION_ANIM(h->c.motion) != 0x204) {
        Motion_PlayTable(h->c.motion, 0x204, -1);
    }
}

/* ---- circling to its flank ---- */

/* get round to his target's (+0xF3544, in his room; else the default action) side. Behind it
 * (his bearing within 45 degrees of the pursuer's heading) and facing much its way (within 135):
 * line up with the pursuer's heading as he runs; within 40 the side is settled (+0xF36B4 -1,
 * +0xF36B8 1) and, lined up within 30, the default action. Else (once settled: the default
 * action): once on its triangle pick the side by the bearing (+0xF36B4 0 / 1), and run for the
 * point beside it 45 degrees round from straight out, 20 out (up to 40 the more squarely he is
 * behind), the way clear to it; unsettled, along the path to it. Gait by the distance (walk
 * under 20, trot under 44, else run) */
/* 0x0015C1E0 */
void Hewie_StateFlank(Hewie *h) {
    Character *t;
    f32 p, dd, rel, d, step;

    if (!in_his_room(h, HW(h, 0xF3544, Character *))) {
        hewie_want(h, 0, 0);
        return;
    }
    p = gCharPursuer->a.angle[1];
    dd = Actor_Distance(&h->c.a, HW(h, 0xF3544, Character *)->a.pos);
    rel = Angle_Wrap(p - Actor_HeadingTo(&h->c.a, HW(h, 0xF3544, Character *)->a.pos));
    if ((rel <= 0.0f ? -rel : rel) < 0x1.921fb60000000p-1f /* 0.7853982 */ && heading_gap(h, gCharPursuer) < 0x1.2d97c80000000p+1f /* 2.3561945 */) {
        if (HW(h, 0xF3604, s32) != 8) {
            HW(h, 0xF3604, s32) = 8;
            HW(h, 0xF3608, s32) = 10;
        }
        HW(h, 0xF3614, f32) = 0.0f;
        HW(h, 0xF3618, f32) = Angle_Wrap(p - h->c.a.angle[1]);
        step = run_turn(h);
        turn_toward(h, p, step);
        if (dd < 40.0f) {
            HW(h, 0xF36B4, s32) = -1;
            HW(h, 0xF36B8, s32) = 1;
            if (heading_gap(h, gCharPursuer) < 0x1.0c15240000000p-1f /* 0.5235988 */) {
                hewie_want(h, 0, 0);
            }
        }
        d = dd - 40.0f;
    } else {
        if (HW(h, 0xF36B8, s32) != 0) {
            hewie_want(h, 0, 0);
            return;
        }
        if (HW(h, 0xF36B4, s32) == -1) {
            t = HW(h, 0xF3544, Character *);
            if (Actor_TriTo(&h->c.a, t->a.pos, NAV_NONE) == t->a.navTri) {
                HW(h, 0xF36B4, s32) = rel < 0.0f ? 0 : 1;
            }
        }
        if (HW(h, 0xF36B4, s32) != -1) {
            f32 m[4][4] __attribute__((aligned(16)));
            f32 v[4] __attribute__((aligned(16)));
            f32 at[4] __attribute__((aligned(16)));
            f32 out = Actor_HeadingTo(&HW(h, 0xF3544, Character *)->a, h->c.a.pos);
            f32 side, reach, a;

            side = HW(h, 0xF36B4, s32) != 0 ? Angle_Wrap(out + 0x1.921fb60000000p-1f /* 0.7853982 */) : Angle_Wrap(out - 0x1.921fb60000000p-1f /* 0.7853982 */);
            if ((rel <= 0.0f ? -rel : rel) < 0x1.921fb60000000p+0f /* 1.5707964 */) {
                if (rel <= 0.0f) {
                    rel = -rel;
                }
                reach = 20.0f + 20.0f * (1.0f - msl_sinf(rel));
            } else {
                reach = 20.0f;
            }
            t = HW(h, 0xF3544, Character *);
            v[2] = Actor_FreeDistance(&h->c.a, t->a.navTri, t->a.pos, NAV_NONE, side, reach);
            v[0] = 0.0f;
            v[1] = 0.0f;
            v[3] = 0.0f;
            Mtx_AtHeading(m, HW(h, 0xF3544, Character *)->a.pos, side);
            Mtx_ApplyPoint(at, m, v);
            if (Actor_TriTo(&h->c.a, at, NAV_NONE) == NAV_NONE) {
                sceVu0CopyVector(at, HW(h, 0xF3544, Character *)->a.pos);
            }
            a = Actor_HeadingTo(&h->c.a, at);
            if (HW(h, 0xF3604, s32) != 8) {
                HW(h, 0xF3604, s32) = 8;
                HW(h, 0xF3608, s32) = 10;
            }
            HW(h, 0xF3614, f32) = 0.0f;
            HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
            step = run_turn(h);
            turn_toward(h, a, step);
            d = reach + 40.0f;
        } else {
            t = HW(h, 0xF3544, Character *);
            if (Hewie_PlanAndGo(h, t->a.navTri, t->a.pos, 0, 1) != 0) {
                hewie_want(h, 0, 0);
                return;
            }
            if (!(u8)Hewie_Stride(h)) {
                Hewie_KeepPose(h, 8);
            }
            d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
                gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
        }
    }
    Hewie_KeepPose(h, d < 20.0f ? 7 : d < 44.0f ? 8 : 9);
}

/* ---- keeping away from his target ---- */

/* keep away from his target (+0xF3544, in his room and reachable; else the default action):
 * heading +0xF36C4 renewed every 30..150 frames, or when the way ahead is under +0xF36C8 (away
 * from it, the freest within 30..150 degrees for 10 more; needing more than a 170-degree turn:
 * the default action), held 30 frames before rechecking; head level the way he goes; gait
 * +0xF36BC, slower where the way ahead is short */
/* 0x0015CCA0 */
void Hewie_StateKeepAway(Hewie *h) {
    Character *t;
    f32 d[4] __attribute__((aligned(16)));
    f32 room, step;

    if (!in_his_room(h, HW(h, 0xF3544, Character *))) {
        hewie_want(h, 0, 0);
        return;
    }
    t = HW(h, 0xF3544, Character *);
    if (!(u8)Hewie_PlanTo(h, t->a.navTri, t->a.pos)) {
        hewie_want(h, 0, 0);
        return;
    }
    room = Actor_FreeDistance(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, HW(h, 0xF36C4, f32), HW(h, 0xF36C8, f32));
    if (HW(h, 0xF36B8, s32) != 0) {
        HW(h, 0xF36B8, s32) -= 1;
    } else {
        if (HW(h, 0xF36B4, s32) != 0) {
            HW(h, 0xF36B4, s32) -= 1;
        }
        if (HW(h, 0xF36B4, s32) == 0 || room < HW(h, 0xF36C8, f32)) {
            f32 a;

            HW(h, 0xF36B4, s32) = (s32)(5.0f * RNG01()) * 30 + 30;
            HW(h, 0xF36B8, s32) = 30;
            sceVu0SubVector(d, h->c.a.pos, HW(h, 0xF3544, Character *)->a.pos);
            HW(h, 0xF36C4, f32) = msl_atan2f(d[0], d[2]);
            HW(h, 0xF36C4, f32) = Hewie_BestHeading(h, HW(h, 0xF36C4, f32), 10.0f + HW(h, 0xF36C8, f32), 30, 150, 30);
            if (!(Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]) <= 0.0f)) {
                a = Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
            } else {
                a = -Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
            }
            if (!(a <= 0x1.7bc89c0000000p+1f /* 2.9670596 */)) {
                hewie_want(h, 0, 0);
                return;
            }
        }
    }
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(HW(h, 0xF36C4, f32) - h->c.a.angle[1]);
    step = run_turn(h);
    turn_toward(h, HW(h, 0xF36C4, f32), step);
    if (HW(h, 0xF36BC, s32) == 0 || room < 10.0f) {
        Hewie_KeepPose(h, 7);
    } else if (HW(h, 0xF36BC, s32) == 1 || room < 20.0f) {
        Hewie_KeepPose(h, 8);
    } else {
        Hewie_KeepPose(h, 9);
    }
}

/* ---- bringing it to Fiona ---- */

extern const PTMF Hewie_State1740_ptmf2;

/* come to her (in his room; else the default action) as follow_step (running over 40, back to
 * a trot under 30); there: action +0xF3570, or
 * with none behaviour Hewie_State1740_ptmf2 for 60 frames */
/* 0x0015D490 */
void Hewie_StateComeToFiona(Hewie *h) {
    if (!in_his_room(h, gCharPlayer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (follow_step(h, 40.0f, 30.0f) == 1) {
        if (HW(h, 0xF3570, s32) == 0) {
            HW(h, 0xF36B8, s32) = 60;
            Hewie_SetBehaviour(h, &Hewie_State1740_ptmf2);
        } else {
            hewie_want(h, HW(h, 0xF3570, s32), 0);
        }
    }
}

/* ---- getting up to bring it ---- */

extern const PTMF Hewie_State1740_ptmf, Hewie_StateComeToFiona_ptmf;

/* from lying low (pose 3), Fiona in his room (else the default action): slide with the root
 * motion and plan the way to her command's place (no way: the default action). Within 12: there
 * (action +0xF3570, or with none behaviour Hewie_State1740_ptmf for 60 frames); else pick the gait (walk
 * under 20 or when moving 1, trot under 30, else run) and come (Hewie_StateComeToFiona_ptmf) */
/* 0x0015DB60 */
void Hewie_StateSlideToFiona(Hewie *h) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d;

    if (!in_his_room(h, gCharPlayer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (Hewie_StepToPose(h, 3) != 0) {
        return;
    }
    Hewie_KeepPose(h, 3);
    slide_root(h);
    if (Hewie_PlanAndGo(h, Hewie_CommandPlace(h, HEWIE_ACTION(h), at), at, 0, 1) != 0) {
        hewie_want(h, 0, 0);
        return;
    }
    d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
        gSceneGameF29740, h->c.a.pos, h->c.unk128, h->c.unk124, h->c.unk12C);
    if (d < 12.0f) {
        h->c.unk124 = h->c.unk128;
        if (HW(h, 0xF3570, s32) == 0) {
            HW(h, 0xF36B8, s32) = 60;
            Hewie_SetBehaviour(h, &Hewie_State1740_ptmf);
        } else {
            hewie_want(h, HW(h, 0xF3570, s32), 0);
        }
        return;
    }
    HW(h, 0xF36B4, s32) = h->c.a.unkC4 == 1 || d < 20.0f ? 0 : d < 30.0f ? 1 : 2;
    Hewie_SetBehaviour(h, &Hewie_StateComeToFiona_ptmf);
}

/* ---- barking for Fiona ---- */

/* turn to Fiona (in his room; else the default action) and, facing her within 60 degrees and
 * standing, bark (Hewie_Bark) +0xF36B4 times, then the default action; within 10 of her:
 * action 0x55 (0xB) */
/* 0x0015DF40 */
void Hewie_StateBarkAtFiona(Hewie *h) {
    f32 a, step, left;

    if (!in_his_room(h, gCharPlayer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (Actor_Distance(&h->c.a, gCharPlayer->a.pos) < 10.0f) {
        hewie_want(h, 0x55, 0xB);
        return;
    }
    a = Actor_HeadingTo(&h->c.a, gCharPlayer->a.pos);
    step = run_turn(h);
    left = turn_toward(h, a, step);
    if (!(left <= 0x1.0c15240000000p+0f /* 1.0471976 */)) {
        Hewie_KeepPose(h, 7);
        return;
    }
    if (Hewie_StepToPose(h, 0) == 0) {
        Hewie_Bark(h);
        HW(h, 0xF36B4, s32) -= 1;
        if (HW(h, 0xF36B4, s32) == 0) {
            hewie_want(h, 0, 0);
        }
    }
}

/* ---- barking at the spot ---- */

/* bark at where his target is (Hewie_TargetTri; no target in target mode: action 6; none: action
 * 8): turning to it, head level toward it; facing it within 60 degrees, standing: the noise
 * (unless progress flags 0x13 / 0x2B) and a bark, +0xF36B4 times, then the default action;
 * within 10 of it: remember it (+0xF36E0), action 0x57 (0xA). Standing between barks */
/* 0x0015E3A0 */
void Hewie_StateBarkAtTarget(Hewie *h) {
    f32 at[4] __attribute__((aligned(16)));
    f32 a, step, left;

    if (HW(h, 0xF3544, Character *) == NULL && HW(h, 0xF366D, u8) != 0) {
        hewie_want(h, 6, 0);
        Hewie_KeepPose(h, 0);
        return;
    }
    if (Hewie_TargetTri(h, at) == -1) {
        hewie_want(h, 8, 0);
        Hewie_KeepPose(h, 0);
        return;
    }
    if (Actor_Distance(&h->c.a, at) < 10.0f) {
        sceVu0CopyVector(&HW(h, 0xF36E0, f32), at);
        hewie_want(h, 0x57, 0xA);
        return;
    }
    a = Actor_HeadingTo(&h->c.a, at);
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
    step = run_turn(h);
    left = turn_toward(h, a, step);
    if (!(left <= 0x1.0c15240000000p+0f /* 1.0471976 */)) {
        Hewie_KeepPose(h, 7);
        return;
    }
    if (Hewie_StepToPose(h, 0) == 0) {
        Progress *p = gProgress;

        if (((u8)Progress_TestFlag(p, 0x13) | (u8)Progress_TestFlag(p, 0x2B)) == 0) {
            Progress_GetVar(p, 0x27);
            Relation_Request(p, AT(h, 0x20, u8), 4, 6, 0, 0, 0.0f);
        }
        Hewie_Bark(h);
        HW(h, 0xF36B4, s32) -= 1;
        if (HW(h, 0xF36B4, s32) == 0) {
            hewie_want(h, 0, 0);
            return;
        }
    }
    Hewie_KeepPose(h, 0);
}

/* ---- waiting by Fiona ---- */

/* turn to Fiona (in his room; else the default action) and, facing her within 60 degrees and
 * standing, wait in the stance (animation 4, 5 in mood 2); within 10 of her: action 0x55 (9) */
/* 0x0015E880 */
void Hewie_StateWaitForFiona(Hewie *h) {
    f32 a, step, left;

    if (!in_his_room(h, gCharPlayer)) {
        hewie_want(h, 0, 0);
        return;
    }
    if (Actor_Distance(&h->c.a, gCharPlayer->a.pos) < 10.0f) {
        hewie_want(h, 0x55, 9);
        return;
    }
    a = Actor_HeadingTo(&h->c.a, gCharPlayer->a.pos);
    step = run_turn(h);
    left = turn_toward(h, a, step);
    if (!(left <= 0x1.0c15240000000p+0f /* 1.0471976 */)) {
        Hewie_KeepPose(h, 7);
        return;
    }
    if (Hewie_StepToPose(h, 0) == 0) {
        s32 anim = MOTION_ANIM(h->c.motion);

        if (HW(h, 0xF35C0, s32) == 2) {
            if (anim != 5) {
                Motion_PlayTable(h->c.motion, 5, -1);
            }
        } else if (anim != 4) {
            Motion_PlayTable(h->c.motion, 4, -1);
        }
    }
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* ---- facing where his target is ---- */

/* the stance: once standing, animation 5 in mood 2, else 4 */
static void stance(Hewie *h) {
    if (Hewie_StepToPose(h, 0) == 0) {
        s32 anim = MOTION_ANIM(h->c.motion);

        if (HW(h, 0xF35C0, s32) == 2) {
            if (anim != 5) {
                Motion_PlayTable(h->c.motion, 5, -1);
            }
        } else if (anim != 4) {
            Motion_PlayTable(h->c.motion, 4, -1);
        }
    }
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* turn to where his target is (Hewie_TargetTri; none: head ahead) and stand facing it (within 60
 * degrees) in the stance; within 10 of it: remember it (+0xF36E0), action 0x57 (8) */
/* 0x0015ECC0 */
void Hewie_StateFaceTarget(Hewie *h) {
    f32 at[4] __attribute__((aligned(16)));
    f32 a, step, left;

    if (Hewie_TargetTri(h, at) == -1) {
        if (HW(h, 0xF3604, s32) != 4) {
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 10;
        }
        stance(h);
        return;
    }
    if (Actor_Distance(&h->c.a, at) < 10.0f) {
        sceVu0CopyVector(&HW(h, 0xF36E0, f32), at);
        hewie_want(h, 0x57, 8);
        return;
    }
    a = Actor_HeadingTo(&h->c.a, at);
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    HW(h, 0xF3614, f32) = 0.0f;
    HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
    step = run_turn(h);
    left = turn_toward(h, a, step);
    if (!(left <= 0x1.0c15240000000p+0f /* 1.0471976 */)) {
        Hewie_KeepPose(h, 7);
        return;
    }
    stance(h);
}

/* ---- sniffing toward it ---- */

/* face where his target is (Hewie_TargetTri; or the scent +0xF3630 while +0xF3620; nothing: head
 * ahead) and stand alert (animation 3), head on it (level without the scent); turning to it
 * first (beyond 60 degrees walking, pose 7); within 10: remember it (+0xF36E0), action 0x57 (6) */
/* 0x0015F2E0 */
void Hewie_StateFaceScent(Hewie *h) {
    f32 at[4] __attribute__((aligned(16)));
    f32 pitch, yaw, a, step, left;
    s32 have = 1;

    if ((HW(h, 0xF3544, Character *) == NULL && HW(h, 0xF366D, u8) == 0) || Hewie_TargetTri(h, at) == -1) {
        have = 0;
    }
    if (!have && HW(h, 0xF3620, u8) == 1) {
        have = 1;
        sceVu0CopyVector(at, &HW(h, 0xF3630, f32));
    }
    if (have != 1) {
        if (HW(h, 0xF3604, s32) != 4) {
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 10;
        }
    } else if (!(Actor_Distance(&h->c.a, at) < 10.0f)) {
        if (HW(h, 0xF3604, s32) != 8) {
            HW(h, 0xF3604, s32) = 8;
            HW(h, 0xF3608, s32) = 10;
        }
        Motion_LookAt(h->c.motion, at, &pitch, &yaw);
        if (HW(h, 0xF3620, u8) == 0) {
            pitch = 0.0f;
        }
        a = Angle_Wrap(h->c.a.angle[1] + yaw);
        HW(h, 0xF3614, f32) = pitch;
        HW(h, 0xF3618, f32) = Angle_Wrap(a - h->c.a.angle[1]);
        step = run_turn(h);
        left = turn_toward(h, a, step);
        if (!(left <= 0x1.0c15240000000p+0f /* 1.0471976 */)) {
            Hewie_KeepPose(h, 7);
            return;
        }
    } else {
        sceVu0CopyVector(&HW(h, 0xF36E0, f32), at);
        hewie_want(h, 0x57, 6);
        return;
    }
    if (Hewie_StepToPose(h, 0) == 0 && MOTION_ANIM(h->c.motion) != 3) {
        Motion_PlayTable(h->c.motion, 3, -1);
    }
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* ---- obedience over time ---- */

extern const s32 D_003B1390[8], D_003B13B0[8];   /* waiting time by trust (normal / difficulty 1) */

/* his obedience timer (+0xF359C) runs down; waiting (+0xF3598) close to Fiona (within 30, in his
 * room) three times as fast, and unless already in 0x7D / 0x7E every 91 frames (+0xF35A0) he
 * nudges her (Fiona_HewieReact 0x10). Out of time: the first time (+0xF3587) only marked; then
 * obeying turns to waiting (not in mood 1; wait by trust, D_003B1390 / D_003B13B0; bit 1 of
 * +0xF356C flags +0xF3559), waiting back to obeying (obey_time; bit 0 flags +0xF3559 or, hidden,
 * action 0x34). Not waiting: +0xF3586 cleared */
/* 0x0015FE30 */
void Hewie_Obedience(Hewie *h) {
    HW(h, 0xF359C, s32) -= 1;
    if (HW(h, 0xF3598, s32) == 1 && in_his_room(h, gCharPlayer)) {
        if (Actor_Distance(&h->c.a, gCharPlayer->a.pos) < 30.0f) {
            HW(h, 0xF359C, s32) -= 2;
            if (HEWIE_ACTION(h) != 0x7D && HEWIE_ACTION(h) != 0x7E) {
                HW(h, 0xF35A0, s16) += 1;
                if ((u32)HW(h, 0xF35A0, s16) >= 91) {
                    HW(h, 0xF35A0, s16) = 0;
                    Fiona_HewieReact((Fiona *)gCharPlayer, 0x10);
                }
            } else {
                HW(h, 0xF35A0, s16) = 0;
            }
        } else {
            HW(h, 0xF35A0, s16) = 0;
        }
    }
    if (HW(h, 0xF359C, s32) <= 0) {
        if (HW(h, 0xF3587, u8) == 1) {
            HW(h, 0xF359C, s32) = 0;
            if (HW(h, 0xF3598, s32) == 0) {
                if (HW(h, 0xF35C0, s32) != 1) {
                    HW(h, 0xF3598, s32) = 1;
                    if ((Progress_GetVar(gProgress, 0x27) & 0xFF) != 1) {
                        HW(h, 0xF359C, s32) = D_003B1390[HW(h, 0xF35CC, s16)];
                    } else {
                        HW(h, 0xF359C, s32) = D_003B13B0[HW(h, 0xF35CC, s16)];
                    }
                    if ((HW(h, 0xF356C, u32) & 0x80000002) == 2) {
                        HW(h, 0xF3559, u8) = 1;
                    }
                }
            } else {
                HW(h, 0xF3598, s32) = 0;
                obey_time(h);
                if ((HW(h, 0xF356C, u32) & 0x80000001) == 1) {
                    if (!h->c.a.disabled) {
                        HW(h, 0xF3559, u8) = 1;
                    } else {
                        hewie_want(h, 0x34, 0);
                    }
                }
                HW(h, 0xF3586, u8) = 0;
            }
        } else {
            HW(h, 0xF3587, u8) = 1;
        }
    }
    if (HW(h, 0xF3598, s32) == 0) {
        HW(h, 0xF3586, u8) = 0;
    }
}

/* ---- the mode's behaviour ---- */

extern const PTMF Hewie_StateWary_ptmf, Hewie_StateCalm_ptmf;
extern const PTMF Hewie_StateTense_ptmf;   /* tense behaviour */

/* keep his behaviour in step with the game mode (not when down), from the behaviour of mode
 * `own`: in it, his idle choice when flagged (+0xF3559; hidden, Hewie_WhenIdle instead).
 * Otherwise to the mode's behaviour, mood and counters (+0xF35C4 / +0xF36A4 / +0xF36AC)
 * cleared: calm (0, Hewie_StateCalm_ptmf) not alert; wary (1, Hewie_StateWary_ptmf) from calm his feelings tested
 * (Hewie_FeelPursuer, when alert) and bit 9 of +0xF356C starting action 0x14; tense (2, Hewie_StateTense_ptmf)
 * alert to the pursuer in his room and his feelings tested. Bit 2 of +0xF356C flags +0xF3559
 * (not on that calm-to-wary step) */
static void mode_behaviour(Hewie *h, u32 own) {
    Progress *p;
    u32 mode;

    if (h->c.a.unkC4 == 2) {
        return;
    }
    p = gProgress;
    mode = (u8)Progress_GameMode(p);
    if (mode == own) {
        if (HW(h, 0xF3559, u8) == 1) {
            if (!h->c.a.disabled) {
                Hewie_WhatNext(h);
                HW(h, 0xF3559, u8) = 0;
            } else {
                HW(h, 0xF3559, u8) = 0;
                Hewie_WhenIdle(h);
            }
        }
        return;
    }
    switch (mode) {
    case 0:
        HW(h, 0xF35C4, s32) = 0;
        HW(h, 0xF36AC, s32) = 0;
        HW(h, 0xF36A4, s32) = 0;
        HW(h, 0xF366D, u8) = 0;
        Actor_SetState(&h->c.a, &Hewie_StateCalm_ptmf);
        break;
    case 1:
        HW(h, 0xF35C4, s32) = 0;
        HW(h, 0xF36AC, s32) = 0;
        HW(h, 0xF36A4, s32) = 0;
        Actor_SetState(&h->c.a, &Hewie_StateWary_ptmf);
        if (own == 0 && !h->c.a.disabled) {
            if (HW(h, 0xF366D, u8) != 0) {
                Hewie_FeelPursuer(h);
            }
            if ((HW(h, 0xF356C, u32) & 0x80000200) == 0x200) {
                hewie_want(h, 0x14, 0);
            }
            return;
        }
        break;
    case 2:
        HW(h, 0xF35C4, s32) = 0;
        HW(h, 0xF36AC, s32) = 0;
        HW(h, 0xF36A4, s32) = 0;
        Actor_SetState(&h->c.a, &Hewie_StateTense_ptmf);
        if (!h->c.a.disabled) {
            if (in_his_room(h, gCharPursuer)) {
                HW(h, 0xF366D, u8) = 3;
                HW(h, 0xF3670, s32) = gCharPursuer->a.slot;
            }
            if (HW(h, 0xF366D, u8) == 3 || HW(h, 0xF366D, u8) == 1) {
                Hewie_FeelPursuer(h);
            }
        }
        break;
    default:
        return;
    }
    if ((HW(h, 0xF356C, u32) & 0x80000004) == 4) {
        HW(h, 0xF3559, u8) = 1;
    }
}

/* the tense behaviour's mode check (mode_behaviour 2) */
/* 0x001602F0 */
void Hewie_StateTense(Hewie *h) {
    mode_behaviour(h, 2);
}

/* the wary behaviour's mode check */
/* 0x00160690 */
void Hewie_StateWary(Hewie *h) {
    mode_behaviour(h, 1);
}

/* the calm behaviour's mode check */
/* 0x00160B60 */
void Hewie_StateCalm(Hewie *h) {
    mode_behaviour(h, 0);
}

/* ---- hidden: what to do ---- */

extern const s8 D_003B1238[8];   /* by trust: chance (percent) he comes out at panic 4 / 5 */
extern const s8 D_003B11E8[8];   /* by trust: chance he goes to a noise */

/* each frame while hidden (+0x29): down, action 0x52. Fiona's panic rising to 4, then 5 (each
 * once, +0xF3589; reset below 4) may bring him out (action 0x39, by his trust's chance, unless
 * already moving 0x39). Out of time (+0xF35B4 counting): stay hidden (0x2C, +0xF3583 cleared),
 * or with time left come out (0x77). Then by +0xF356C: bit 6 with progress test 1 flags
 * +0xF3559; bit 4 with a new noise heard (in a room other than +0xF3594) may send him to it
 * (action 0x33, by his trust's chance) */
/* 0x00161070 */
void Hewie_HiddenFrame(Hewie *h) {
    Progress *p;

    if (h->c.a.unkC4 == 2) {
        if (HEWIE_ACTION(h) != 0x52) {
            hewie_want(h, 0x52, 0);
        }
        return;
    }
    if (!(HW(h, 0xF3589, u8) & 2)) {
        s32 rise = 0;

        if (AT(gProgress, 0x7B8, u8) == 5) {
            HW(h, 0xF3589, u8) |= 2;
            rise = 1;
        } else if (!(HW(h, 0xF3589, u8) & 1) && AT(gProgress, 0x7B8, u8) == 4) {
            HW(h, 0xF3589, u8) |= 1;
            rise = 1;
        }
        if ((u8)rise == 1 && by_chance(h, D_003B1238) && h->c.moveMode != 0x39) {
            hewie_want(h, 0x39, 0);
            return;
        }
    }
    p = gProgress;
    if ((s32)AT(p, 0x7B8, u8) < 4) {
        HW(h, 0xF3589, u8) = 0;
    }
    if (HW(h, 0xF35B4, s32) != 0) {
        if (HW(h, 0xF35B4, s32) > 0 && HEWIE_ACTION(h) != 0x77) {
            hewie_want(h, 0x77, 0);
            return;
        }
    } else {
        HW(h, 0xF35B4, s32) = -1;
        HW(h, 0xF3583, u8) = 0;
        hewie_want(h, 0x2C, 0);
    }
    if ((HW(h, 0xF356C, u32) & 0x80000040) == 0x40 && Progress_CondBit(p, 1) != 0) {
        HW(h, 0xF3559, u8) = 1;
        return;
    }
    if ((HW(h, 0xF356C, u32) & 0x80000010) == 0x10 && h->c.heardSlot != 0xFF &&
        h->c.heard.room != HW(h, 0xF3594, s32) && by_chance(h, D_003B11E8)) {
        HW(h, 0xF3594, s32) = h->c.heard.room;
        hewie_want(h, 0x33, 0);
    }
}

/* ---- on his feet: what catches his eye ---- */

/* each frame standing about (not hidden, not moving): down, action 0x52 (unless already 0x52 /
 * 0x74). Standing or sitting on a slope (mesh flag 1) facing much along it (within 60 degrees):
 * action 0x6E. Another character (but himself) within 5 in his room on the mesh: his target,
 * action 0x82 */
/* 0x00161500 */
void Hewie_StandingFrame(Hewie *h) {
    f32 n[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    f32 fwd[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    s32 g;
    u32 i;

    if (h->c.a.disabled == 1 || h->c.moveMode != 0) {
        return;
    }
    if (h->c.a.unkC4 == 2) {
        if (HEWIE_ACTION(h) != 0x52 && HEWIE_ACTION(h) != 0x74) {
            hewie_want(h, 0x52, 0);
        }
        return;
    }
    g = Hewie_AnimGroup(h);
    if ((g == 3 || g == 2 || g == 1 || g == 0) && HEWIE_ACTION(h) != 0x6E &&
        /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
        (NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & 1)) {
        VCALL(gNavMesh, 0x2C, void (*)(void *, u32, f32 *))(gNavMesh, h->c.a.navTri, n);
        if (n[1] != 1.0f) {
            n[1] = 0.0f;
            sceVu0Normalize(dir, n);
            fwd[2] = 1.0f;
            fwd[0] = 0.0f;
            fwd[1] = 0.0f;
            fwd[3] = 0.0f;
            sceVu0ApplyMatrix(fwd, h->c.a.rot, fwd);
            if (!(sceVu0InnerProduct(fwd, dir) <= 0.5f)) {
                hewie_want(h, 0x6E, 0);
                return;
            }
        }
    }
    if (HEWIE_ACTION(h) == 0x82) {
        return;
    }
    for (i = 0; i < 7; i++) {
        Character *c = gCharacters[i];

        if (i == 1) {
            continue;
        }
        if (c != NULL && c->a.active == 1 && h->c.a.room == c->a.room && c->a.navTri != NAV_NONE) {
            sceVu0SubVector(d, c->a.pos, h->c.a.pos);
            if (__builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) < 5.0f) {
                HW(h, 0xF3544, Character *) = c;
                hewie_want(h, 0x82, 0);
                return;
            }
        }
    }
}

/* ---- put in a room by a placement ---- */

/* put him where `pl` says (as Hewie_PlaceInRoom): in the room being played (or under direct control,
 * +0x1FBEC1) placed facing as he was (head straight, standing), his door the placement's exit,
 * the default action; in the room being played also settled in (arrived). Elsewhere he only
 * keeps the triangle, standing. Returns the placement result (0 elsewhere) */
/* 0x001662A0 */
s32 Hewie_PlaceAtPlacement(Hewie *h, HewiePlacement *pl) {
    s32 r = 0;

    Character_ToRoom(&h->c, pl->room, pl->tri, pl->side);
    HEWIE_SIDE(h) = pl->side;
    if (*((u8 *)gProgress + 0x1FBEC1) == 0 && pl->room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        h->c.a.navTri = pl->tri;
        Motion_PlayTable(h->c.motion, 0, -1);
        hewie_want(h, 0, 0);
        return 0;
    }
    MOTION_PTR(h->c.motion, 0x858) = NULL;
    MOTION_PTR(h->c.motion, 0x854) = NULL;
    HW(h, 0xF36F0, s32) = 0;
    r = Hewie_Place(h, pl->tri);
    h->c.door = pl->exit;
    hewie_want(h, 0, 0);
    if (*((u8 *)gProgress + 0x1FBEC1) == 0) {
        arrived(h);
    }
    return r;
}

/* ---- joint actions with Fiona ---- */

/* his second state block set to (kind, a) (the original copies a local whose last fields are
 * never set) */
static void post_state2(Character *c, s32 kind, s32 a) {
    c->state2[0] = kind;
    c->state2[1] = a;
    c->state2[2] = 0;
    c->state2[3] = 0;
    c->state2[4] = 0;
    *(f32 *)&c->state2[5] = 0.0f;
    c->state2[6] = 0;
    c->state2[7] = 0;
}

/* take up the joint action the game has queued for him (Progress_HasRelationCmd), with Fiona: 0 if
 * accepted (his second state block (0xC, its type)), -1 if none or cancelled. Out of her reach
 * (+0xE0): cancelled. Only kind 2. Types 0 / 2 / 4 (her starting it): calm, standing or already
 * in it (moves 0 / 0xC), listening (+0xF356C bit 3), on open ground, her within 30 in his room
 * and straight reachable: the meeting point by the type (Hewie_FindSpot 1 / 2 / 3) given to
 * her (+0x104 / +0x10C / +0x110) and his heading. Types 1 / 3 / 5 (the second part): in move
 * 0xC, settled sitting (group 1) facing the agreed way, her in his room */
/* 0x00164830 */
s32 Hewie_JointAction(Hewie *h) {
    Progress *p = gProgress;
    Progress *q;
    u32 kind, type;

    if ((u8)Progress_HasRelationCmd(p, AT(h, 0x20, u8)) != 1) {
        return -1;
    }
    if (h->c.unkE0 == 1) {
        SlotCmd_Cancel(p, AT(h, 0x20, u8));
        return -1;
    }
    q = gProgress;
    SlotCmd_Target(q, AT(h, 0x20, u8));
    kind = SlotCmd_Kind(q, AT(h, 0x20, u8)) & 0xFF;
    type = SlotCmd_Arg(q, AT(h, 0x20, u8)) & 0xFF;
    if (kind == 2) {
        switch (type) {
        case 0:
        case 2:
        case 4:
            if (!(u8)Progress_GameMode(p) && (h->c.moveMode == 0 || h->c.moveMode == 0xC) &&
                (HW(h, 0xF356C, u32) & 0x80000008) == 8 &&
                /* (the original reads the flags at address 0x3C for a triangle off the mesh) */
                !(NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & 0x80001) && in_his_room(h, gCharPlayer) &&
                Actor_Distance(&h->c.a, gCharPlayer->a.pos) < 30.0f &&
                Actor_TriTo(&h->c.a, gCharPlayer->a.pos, 0x60088) == gCharPlayer->a.navTri) {
                f32 yaw;
                f32 at[4] __attribute__((aligned(16)));
                s32 r = Hewie_FindSpot(h, type == 4 ? 3 : type == 0 ? 1 : 2, &yaw, at);

                if (r != -1) {
                    *(f32 *)&gCharPlayer->unk104[2] = yaw;
                    gCharPlayer->unk104[0] = r;
                    sceVu0CopyVector(gCharPlayer->unk110, at);
                    HW(h, 0x10C, f32) = yaw;
                    SlotCmd_Start(p, AT(h, 0x20, u8));
                    post_state2(&h->c, 0xC, type);
                    return 0;
                }
            }
            break;
        case 1:
        case 3:
        case 5:
            if (h->c.moveMode == 0xC && AT(h->c.motion, 0x550, f32) <= 0.0f && Hewie_AnimGroup(h) == 1 &&
                HW(h, 0x10C, f32) == h->c.a.angle[1] && in_his_room(h, gCharPlayer)) {
                SlotCmd_Start(p, AT(h, 0x20, u8));
                post_state2(&h->c, 0xC, type);
                return 0;
            }
            break;
        }
    }
    SlotCmd_Cancel(p, AT(h, 0x20, u8));
    return -1;
}

/* ---- each frame ---- */

/* his per-frame upkeep: hidden (+0x29) when not in the room being played. Unless the partner's
 * state block holds 5: timers run down (+0xF35DC, +0xF35B4, +0xF3688, +0xF35B0, +0xF355C; his
 * mood +0xF35BE back to mode 0 at 0; praise +0xF3686 clearing +0xF3684), +0xF35A8 counts up to
 * 3000; hurt health heals a point every 300 frames (+0xF35AC). At 0 health he goes down (hidden:
 * kept at 1), down he stays in 0x52; back above 0 he is up hurt (obey time renewed; hidden:
 * action 0x36 when +0xF3583, else the default). Under 30 health hurt (moving 1, his mood
 * reset; not in mood 3), else (or in mood 3) well. The mood's grip (+0xF35C4) eases every 300
 * frames; +0xF3585 kept only in move 8 / action 0x7A; bit 7 of +0xF356C flags +0xF3559 when
 * +0xF355C is out */
/* 0x00164DD0 */
void Hewie_Upkeep(Hewie *h) {
    Progress *p = gProgress;

    h->c.a.disabled = h->c.a.room == VCALL(p, 0xC, s32 (*)(Progress *))(p) ? 0 : 1;
    if ((gCharPartner->state[0] == 5) == 1) {
        return;
    }
    if (HW(h, 0xF35DC, s32) != 0) {
        HW(h, 0xF35DC, s32) -= 1;
    }
    if (h->c.a.unkC4 != 2 && h->c.hp != 0 && h->c.hp < h->c.hpMax) {
        HW(h, 0xF35AC, s32) -= 1;
        if (HW(h, 0xF35AC, s32) <= 0) {
            HW(h, 0xF35AC, s32) = 300;
            h->c.hp += 1;
            if (h->c.hp >= h->c.hpMax) {
                h->c.hp = h->c.hpMax;
            }
        }
    }
    if (h->c.a.unkC4 != 2) {
        if (h->c.hp == 0) {
            if (h->c.a.disabled) {
                h->c.hp = 1;
            } else {
                h->c.a.unkC4 = 2;
                Hewie_SetMode(h, 0, -1);
            }
        }
    } else if (h->c.hp == 0) {
        if (!h->c.a.disabled) {
            if (HEWIE_ACTION(h) == 0) {
                hewie_want(h, 0x52, 0);
            }
        } else if (HEWIE_ACTION(h) != 0x52) {
            hewie_want(h, 0x52, 0);
        }
    } else {
        h->c.a.unkC4 = 1;
        HW(h, 0xF3598, s32) = 0;
        if ((Progress_GetVar(p, 0x27) & 0xFF) != 1) {
            HW(h, 0xF359C, s32) = kHewieLevelNormal[HW(h, 0xF35CC, s16)];
        } else {
            HW(h, 0xF359C, s32) = kHewieLevelHard[HW(h, 0xF35CC, s16)];
        }
        HW(h, 0xF3586, u8) = 0;
        if (h->c.a.disabled == 1) {
            if (HW(h, 0xF3583, u8) == 1) {
                if (HEWIE_ACTION(h) != 0x36) {
                    hewie_want(h, 0x36, 0);
                }
            } else {
                hewie_want(h, 0, 0);
            }
        }
    }
    if (h->c.a.unkC4 != 2) {
        if (h->c.a.unkC4 != 1) {
            if (h->c.hp < 30 && HW(h, 0xF35C0, s32) != 3) {
                h->c.a.unkC4 = 1;
                Hewie_SetMode(h, 0, -1);
                HW(h, 0xF35C4, s32) = 0;
                HW(h, 0xF35C8, s32) = 0;
            }
        } else if (!(h->c.hp < 30) || HW(h, 0xF35C0, s32) == 3) {
            h->c.a.unkC4 = 0;
        }
    }
    if (HW(h, 0xF35C0, s32) != 3 && HW(h, 0xF35C8, s32) != 0) {
        HW(h, 0xF35C8, s32) -= 1;
        if (HW(h, 0xF35C8, s32) == 0 && HW(h, 0xF35C4, s32) != 0) {
            HW(h, 0xF35C4, s32) -= 1;
            HW(h, 0xF35C8, s32) = 300;
        }
    }
    if (h->c.moveMode != 8 && HEWIE_ACTION(h) != 0x7A) {
        HW(h, 0xF3585, u8) = 0;
    }
    if (HW(h, 0xF35B4, s32) != 0) {
        HW(h, 0xF35B4, s32) -= 1;
    }
    HW(h, 0xF35A8, s16) += 1;
    if (HW(h, 0xF35A8, s16) >= 3001) {
        HW(h, 0xF35A8, s16) = 3000;
    }
    if (HW(h, 0xF35C0, s32) != 0) {
        HW(h, 0xF35BE, s16) -= 1;
        if (HW(h, 0xF35BE, s16) <= 0) {
            Hewie_SetMode(h, 0, -1);
            HW(h, 0xF35C4, s32) = 0;
            HW(h, 0xF35C8, s32) = 0;
        }
    }
    if (HW(h, 0xF3686, s16) != 0) {
        HW(h, 0xF3686, s16) -= 1;
        if (HW(h, 0xF3686, s16) == 0) {
            HW(h, 0xF3684, s16) = 0;
        }
    }
    if (HW(h, 0xF3688, s16) != 0) {
        HW(h, 0xF3688, s16) -= 1;
    }
    if (HW(h, 0xF355C, s32) > 0) {
        HW(h, 0xF355C, s32) -= 1;
    }
    if (HW(h, 0xF355C, s32) <= 0 && (HW(h, 0xF356C, u32) & 0x80000080) == 0x80) {
        HW(h, 0xF3559, u8) = 1;
    }
    if (HW(h, 0xF35B0, s16) != 0) {
        HW(h, 0xF35B0, s16) -= 1;
    }
}

/* can he get to exit `e` from where he is: the exit is on his side (or he is on both) and a path
   to its spot (left in `at`) exists */
static s32 exit_reachable(Hewie *h, VObject *rooms, s32 e, f32 *at) {
    s32 side = HEWIE_SIDE(h);

    if (side != VCALL(rooms, 0x50, s32 (*)(VObject *, s32, s32, s32))(rooms, h->c.a.room, e, 0) && side != 2) {
        return 0;
    }
    return Hewie_PlanAndGo(h, VCALL(rooms, 0x34, u32 (*)(VObject *, s32, f32 *))(rooms, e, at), at, 1, 1) == 0;
}

/* Fiona left his room through exit `exit` (0xFF: unknown). If he is in the room being played he
   decides how to follow: through that exit, through another, or he can't and waits/whines. When
   the scene is not interactive (+0x1FBEC1) he just takes the same exit. */
/* 0x00166DF0 */
void Hewie_Vt34(Hewie *h, s32 exit) {
    f32 at[4] __attribute__((aligned(16)));
    f32 door[4] __attribute__((aligned(16)));
    f32 d0[4] __attribute__((aligned(16)));
    f32 d1[4] __attribute__((aligned(16)));
    VObject *rooms;
    s32 act, i;
    u32 e, x = exit & 0xFF;

    if (((u8 *)gProgress)[0x1FBEC1] != 0) {
        h->c.a.unk2B = 0;
        if (HEWIE_ACTION(h) != 0x84) {
            h->c.a.unk2D = 0;
        }
        h->c.door = VCALL(gRooms, 0x14, u32 (*)(VObject *, s32, s32))(gRooms, h->c.a.room, exit);
        HW(h, 0xF3710, u8) = 3;
        return;
    }
    if (h->c.unkE0 == 1) {
        VCALL(h, 0x90, void (*)(Hewie *))(h);
    }
    if (h->c.a.room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return;
    }
    if (h->c.hp == 0 &&
        ((((u32 *)gProgress)[0x30 / 4] & 0x8000) != 0 || !(Actor_TriFreeFor(&h->c.a, &h->c.a) & 0xFF))) {
        h->c.hp = 1;
        h->c.a.unkC4 = 1;
    }
    h->c.a.unk2B = 0;
    h->c.a.navMask = 0x29020008;
    if (HEWIE_ACTION(h) == 0x65) {
        h->c.a.navTri = VCALL(gRooms, 0x34, u32 (*)(VObject *, s32, f32 *))(gRooms, HW(h, 0xF36B4, u8), h->c.a.pos);
    }
    if (HEWIE_ACTION(h) == 0x66) {
        h->c.a.navTri = VCALL(gRooms, 0x30, u32 (*)(VObject *, s32, f32 *))(gRooms, HW(h, 0xF36B4, u8), h->c.a.pos);
    }
    for (i = 0; i < 13; i++) {
        h->c.unk148C[i] = 0;
    }
    HW(h, 0xF3590, u8) = 0;
    HW(h, 0xF3583, u8) = 1;
    if (x != 0xFF && exit_reachable(h, gRooms, exit, at)) {
        HW(h, 0xF3583, u8) = 0;
        HW(h, 0xF3590, u8) = 1;
    }
    if (HW(h, 0xF3583, u8) == 1) {
        rooms = gRooms;
        for (e = 0; e < 8; e++) {
            if (e != x &&
                (VCALL(rooms, 0x74, u32 (*)(VObject *, s32, s32))(rooms, h->c.a.room, e) & 0xFF) == 1 &&
                exit_reachable(h, rooms, e, at)) {
                HW(h, 0xF3583, u8) = 0;
                break;
            }
        }
    }

    act = HEWIE_ACTION(h);
    if (act == 0x52 && h->c.hp == 0) {
        return;
    }
    if (act == 0x76) {
        hewie_want(h, 0x77, 0);
        return;
    }
    if (HW(h, 0xF3583, u8) == 1) {
        hewie_want(h, 0x36, 0);
        return;
    }
    if (HW(h, 0xF3590, u8) == 1) {
        if ((Actor_NearerRoom(&h->c.a, exit, gCharPlayer->a.pos) & 0xFF) == 1) {
            HW(h, 0xF36B4, s32) = VCALL(gRooms, 0x14, u32 (*)(VObject *, s32, s32))(gRooms, h->c.a.room, exit) & 0xFF;
            HEWIE_ACTION(h) = 0x88;
            return;
        }
        /* nearer the door than the exit spot: skip the waypoints short of it */
        rooms = gRooms;
        h->c.unk14C0 = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, s32))(rooms, h->c.a.room, exit);
        VCALL(rooms, 0x30, u32 (*)(VObject *, s32, f32 *))(rooms, exit, door);
        sceVu0SubVector(d0, h->c.a.pos, door);
        sceVu0SubVector(d1, at, door);
        if (sceVu0InnerProduct(d0, d0) <= sceVu0InnerProduct(d1, d1) &&
            Actor_TriTo(&h->c.a, door, NAV_NONE) != NAV_NONE) {
            h->c.unk124 = h->c.unk128;
        }
    }

    act = HEWIE_ACTION(h);
    if (HW(h, 0xF3581, u8) == 1 &&
        (act == 0x75 || act == 0x22 || act == 0x21 || act == 0x20 || act == 0x1F)) {
        HW(h, 0xF36B4, s32) = gCharPursuer->unk153C;
        HW(h, 0xF36B8, s32) = HEWIE_ACTION(h);
        HW(h, 0xF36BC, s32) = h->c.unk104[0];
        hewie_want(h, 0x38, 0);
    } else if ((u32)(HEWIE_ACTION(h) - 2) < 2) {
        HW(h, 0xF3560, s32) = HW(h, 0xF355C, s32) + 150;
        HW(h, 0xF36B8, s32) = Hewie_AnimGroup(h);
        hewie_want(h, 0x2F, 0);
    } else {
        for (i = 0; i < 13; i++) {
            h->c.unk148C[i] = 0;
        }
        hewie_want(h, HW(h, 0xF3598, s32) == 0 ? 0x2C : 0, 0);
    }
}

void Hewie_OwnDecisions(Hewie *h);

/* Per-frame update in the normal game (frame counter in Hewie_Upkeep): how close Fiona is, his
 * surroundings, then - unless he is out of play - joint actions, the director (+0x38 busy:
 * Hewie_StandingFrame), his own decisions or, in a scene, the doors he is told to take; behaviour, turn
 * and root motion. +0x2A: Fiona can see him. Out of play only a pending state change runs. */
/* 0x00167BC0 */
void Hewie_Update(Hewie *h) {
    Progress *p;
    u32 i;

    Hewie_Upkeep(h);
    Hewie_Alert(h);
    if (h->c.a.unkC4 != 2) {
        report_fiona_near(h);
    }
    Hewie_Obedience(h);
    HW(h, 0xF3581, u8) = 0;
    if (h->c.a.disabled) {
        if (Hewie_Arrive(h) != 0) {
            HW(h, 0xF3578, s32) = -1;
            HW(h, 0xF3574, s32) = -1;
            if (h->c.state[0] == 0xD) {
                Hewie_FionaCommand(h);
                h->c.state[0] = 0;
            }
            Hewie_HiddenFrame(h);
            Hewie_Scuffle(h);
            ptmf_scall(h, &h->c.a.state);
            ptmf_scall(h, HEWIE_STATE(h));
        }
        VCALL(h, 0x40, void (*)(Hewie *))(h);
    } else {
        VCALL(h, 0x84, void (*)(Hewie *))(h);
        if (h->c.unkE0 == 1) {
            turn_by_anim(h);
            root_motion(h);
            Hewie_TurnHead(h);
            Hewie_SetOverlays(h);
            VCALL(h, 0x40, void (*)(Hewie *))(h);
            Hewie_AnimSounds(h);
            Hewie_Feet(h);
            return;
        }
        h->c.a.navMask = h->c.a.unk2B ? 8 : HEWIE_NAV_MASK;
        h->c.pathReq->mask = h->c.a.navMask;
        HW(h, 0xF3558, u8) = 0;
        HW(h, 0xF3582, u8) = 1;
        p = gProgress;
        if (Progress_HasRelationCmd(p, SLOT_U8(h)) & 0xFF) {
            Hewie_JointAction(h);
        } else {
            if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) != 0) {
                Hewie_StandingFrame(h);
            } else if (*((u8 *)p + 0x1FBEC1) == 0) {
                Hewie_OwnDecisions(h);
            } else {
                VObject *doors;

                if (h->c.a.unkC4 == 2 && h->c.hp == 0 && HEWIE_ACTION(h) == 0x83) {
                    Hewie_SetAction(h, 0x52, 0);
                }
                doors = gDoors;
                for (i = 0; i < 8; i++) {
                    if ((VCALL(doors, 0x40, u32 (*)(VObject *, u32))(doors, i) & 0xFF) == 1 &&
                        !(VCALL(doors, 0x30, u32 (*)(VObject *, u32))(doors, i) & 0xFF) &&
                        (PursuerGroup_Fields(p, i, SLOT_U8(h)) & 0xFF & 8)) {
                        HW(h, 0xF36B4, s32) = i;
                        Hewie_SetAction(h, 0x86, 0);
                    }
                }
            }
            if (*((u8 *)p + 0x1FBEC1) == 1) {
                HW(h, 0xF3798, s32) = Gesture_Update((u8 *)h + 0xF3748, (f32 *)gPadState);
            }
            ptmf_scall(h, &h->c.a.state);
            ptmf_scall(h, HEWIE_STATE(h));
            turn_by_anim(h);
            if (!h->c.a.disabled && !HW(h, 0xF3558, u8)) {
                h->c.a.navMask = h->c.a.unk2B ? 8 : HEWIE_NAV_MASK;
                root_motion(h);
            }
        }
        if (in_his_room(h, gCharPlayer) &&
            (gCharPlayer->unkE0 == 1 || (Actor_Touching(&h->c.a, &gCharPlayer->a, 0.0f, 0.0f) & 0xFF) == 1)) {
            h->c.a.unk2A = 1;
        } else {
            h->c.a.unk2A = 0;
        }
        Hewie_TurnHead(h);
        Hewie_SetOverlays(h);
        Hewie_AnimSounds(h);
        VCALL(h, 0x40, void (*)(Hewie *))(h);
        Hewie_Feet(h);
    }
    HW(h, 0xF3620, u8) = 0;
    Hewie_MoveSubMode(h);
}

extern s32 Hewie_TakeBlow(Hewie *h, s32 *hit);

/* vtable: pending state (+0x14E8) and the command Fiona gave (+0xF4, 0 none). State 7 holds
 * everything; 4 (stuck) and 13 end with +0x90 (back to normal) once he is clear again, 5 calls +0x8C
 * and drops what he does. Then the command becomes the action that carries it out. */
/* 0x001635B0 */
void Hewie_Requests(Hewie *h) {
    s32 act;

    switch (h->c.state[0]) {
    case 7:
        return;
    case 0:
        break;
    case 4:
        if (!(NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & HEWIE_NAV_MASK) && Hewie_TakeBlow(h, h->c.state) == 0) {
            VCALL(h, 0x90, void (*)(Hewie *))(h);
            h->c.state[0] = 0;
            return;
        }
        h->c.state[0] = 0;
        break;
    case 13:
        if ((fiona_reachable(h) & 0xFF) == 1) {
            if ((Hewie_FionaCommand(h) & 0xFF) == 1) {
                VCALL(h, 0x90, void (*)(Hewie *))(h);
            }
            h->c.state[0] = 0;
            return;
        }
        h->c.state[0] = 0;
        break;
    case 5:
        VCALL(h, 0x8C, void (*)(Hewie *))(h);
        hewie_want(h, 0, 0);
        /* fall through */
    default:
        h->c.state[0] = 0;
        break;
    }

    switch ((u32)h->c.unkF4) {
    case 1:
        VCALL(h, 0x90, void (*)(Hewie *))(h);
        /* fall through */
    case 2:
    case 3:
    case 4:
        act = 0;
        break;
    case 5:  act = 0x3F; break;
    case 6:  act = 0x41; break;
    case 7:  act = 0x3B; break;
    case 8:  act = 0x3C; break;
    case 9:  act = 0x3D; break;
    case 10: act = 0x40; break;
    case 11: act = 0x42; break;
    case 14: act = 0x43; break;
    case 15: act = 0x44; break;
    case 16: act = 0x3E; break;
    case 18: act = 0x45; break;
    case 19: act = 0x7F; break;
    case 20: act = 0x46; break;
    case 21: act = 0x47; break;
    case 12:
        HW(h, 0xF35E0, u8) = 0;
        HW(h, 0xF3610, s32) = h->c.unk100;
        h->c.unkE1 = 1;
        /* fall through */
    default:
        act = -1;
        break;
    case 13:
        HW(h, 0xF35E0, u8) = 1;
        sceVu0CopyVector((f32 *)((u8 *)h + 0xF35F0), h->c.unk110);
        act = -1;
        break;
    }
    if (act >= 0) {
        hewie_want(h, act, 0);
    }
    h->c.unkF4 = 0;
}

extern const PTMF Hewie_StateAnimOver2_ptmf;   /* sniffing about behaviours */
extern const PTMF Hewie_StateAnimOver2_ptmf2;
extern const PTMF Hewie_StateAnimOver2_ptmf3;

/* a sniff-about animation with its behaviour (+0xF3604 4 = sniffing, +0xF3608 how long) */
static void sniff(Hewie *h, s32 anim, const PTMF *st) {
    if (HW(h, 0xF3604, s32) != 4) {
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 10;
    }
    Motion_PlayTable(h->c.motion, anim, -1);
    *HEWIE_STATE(h) = *st;
}

/* vtable: pending state (+0x14E8, detail +0x14EC) when he is in the room being played - 12 is a
 * call from Fiona (come, praise/scold, sniff about, ...), 11 a scene request, 8 and 13 other
 * requests; 4 stuck, 5 reset, 7 hold. With nothing pending and a scripted move waiting
 * (+0xF356C) he takes action 0x85. Elsewhere any pending state is dropped. */
/* 0x00163DC0 */
void Hewie_StateBlock(Hewie *h) {
    Progress *p = gProgress;
    s32 room;
    s32 v;

    HW(h, 0xF3578, s32) = -1;
    room = h->c.a.room;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        if (h->c.state[0] != 0) {
            h->c.state[0] = 0;
        }
        return;
    }
    switch (h->c.state[0]) {
    case 7:
        return;
    case 5:
        VCALL(h, 0x8C, void (*)(Hewie *))(h);
        h->c.unk124 = h->c.unk128;
        hewie_want(h, 0, 0);
        h->c.state[0] = 0;
        return;
    case 4:
        if (Hewie_TakeBlow(h, h->c.state) == 0) {
            h->c.state[0] = 0;
            return;
        }
        h->c.state[0] = 0;
        break;
    }

    if (h->c.state[0] == 12 && (u32)h->c.state[1] < 6) {
        switch (h->c.state[1]) {
        case 0:
        case 2:
            HW(h, 0xF36A8, s32) = HEWIE_ACTION(h);
            /* fall through */
        case 4:
            if (HEWIE_ACTION(h) == 0x7D) {
                HW(h, 0xF3598, s32) = 0;
                obey_time(h);
                HW(h, 0xF3586, u8) = 0;
                HW(h, 0xF359C, s32) += 180;
            }
            hewie_want(h, 0x48, 0);
            break;
        case 1:
            v = -7;
            if ((Hewie_PraiseScold(h, 0, 3) & 0xFF) != 1 && h->c.a.unkC4 != 1 && HW(h, 0xF3598, s32) == 0 &&
                (u32)HW(h, 0xF35C0, s32) <= 2 && (HW(h, 0xF3688, s16) != 0 || RNG01() < 0.5f)) {
                v = -6;
            }
            HW(h, 0xF36B4, s32) = v;
            hewie_want(h, 0x49, 0);
            HW(h, 0xF358C, s32) = 0;
            break;
        case 3:
            v = -9;
            if ((Hewie_PraiseScold(h, 1, 3) & 0xFF) != 1 && h->c.a.unkC4 != 1 && HW(h, 0xF3598, s32) == 0 &&
                (u32)HW(h, 0xF35C0, s32) <= 1 && h->c.hp >= 80 && (HW(h, 0xF3688, s16) != 0 || RNG01() < 0.5f)) {
                v = -8;
            }
            HW(h, 0xF36B4, s32) = v;
            hewie_want(h, 0x4A, 0);
            HW(h, 0xF358C, s32) = 0;
            break;
        case 5:
            v = -1;
            if (HW(h, 0xF358C, s32) != 1 && (h->c.unkE0 == 1 || (HW(h, 0xF356C, u32) & 0x80000008) == 8)) {
                if ((HW(h, 0xF3598, s32) == 0 || HEWIE_ACTION(h) == 0x7D) && HW(h, 0xF35C0, s32) != 3) {
                    v = 0;
                }
                HW(h, 0xF358C, s32) = 1;
            }
            if (v == 0) {
                hewie_want(h, 0x4B, 0);
            } else {
                switch ((s32)(4.0f * RNG01())) {
                case 0:
                    sniff(h, 0x1C06, &Hewie_StateAnimOver2_ptmf);
                    break;
                case 1:
                    sniff(h, 0x1C01, &Hewie_StateAnimOver2_ptmf2);
                    break;
                case 2:
                    sniff(h, 0x1C00, &Hewie_StateAnimOver2_ptmf3);
                    break;
                case 3:
                    hewie_want(h, 0x55, 0);
                    break;
                }
            }
            HW(h, 0xF358C, s32) = 0;
            break;
        }
        h->c.state[0] = 0;
    }
    if (h->c.state[0] == 11) {
        switch (h->c.state[1]) {
        case 0:
            hewie_want(h, 0x79, 0);
            break;
        case 1:
            hewie_want(h, 0x7A, 0);
            break;
        case 2:
            hewie_want(h, 0x84, 0);
            break;
        }
        h->c.state[0] = 0;
    }
    if (h->c.state[0] == 8) {
        hewie_want(h, 0x4E, 0);
        h->c.state[0] = 0;
    }
    if (h->c.state[0] == 13) {
        Hewie_FionaCommand(h);
        HW(h, 0xF358C, s32) = 0;
        h->c.state[0] = 0;
    }
    if (h->c.state[0] != 0) {
        h->c.state[0] = 0;
    }
    if (HW(h, 0xF3798, s32) == -1) {
        return;
    }
    if (h->c.unkE0 == 0 && h->c.moveMode == 0 && HW(h, 0xF356C, u32) != 0 && !(HW(h, 0xF356C, u32) & 0x80000000) &&
        !(Progress_IsLinked(p, 1) & 0xFF) && h->c.state[0] == 0) {
        Hewie_SetAction(h, 0x85, 0);
    }
}

extern const s8 D_003B11E0[];   /* by trust: chance (percent) he goes for whoever holds Fiona */

/* go to Fiona (0x62) if he can reach her, else wait for her (Hewie_SetMode) */
static void to_fiona(Hewie *h) {
    if ((Hewie_PlanTo(h, gCharPlayer->a.navTri, gCharPlayer->a.pos) & 0xFF) == 1) {
        hewie_want(h, 0x62, 0);
    } else {
        Hewie_SetMode(h, 0, -1);
    }
}

/* answer the pursuer (0x4F) if he can reach it, else forget the call (+0xF36B0 0) */
static void to_pursuer(Hewie *h) {
    if ((Hewie_PlanTo(h, gCharPursuer->a.navTri, gCharPursuer->a.pos) & 0xFF) == 1) {
        hewie_want(h, 0x4F, 0);
    } else {
        HW(h, 0xF36B0, s32) = 0;
    }
}

/* angry (+0xF35C0 3, after Fiona hit him): a call (+0xF36B0, 0xFF none; 0 from Fiona) or her having hit him
 * lately (+0xF35C4) brings him; else whichever of Fiona and the pursuer is here (the nearer when
 * both). 0: nothing for him to do. */
static s32 staying(Hewie *h) {
    HW(h, 0xF35B0, s16) = 0;
    if (HW(h, 0xF36B0, s32) != 0xFF) {
        if (HW(h, 0xF36B0, s32) == 0 && in_his_room(h, gCharPlayer)) {
            to_fiona(h);
        } else {
            hewie_want(h, 0x4F, 0);
        }
        return 1;
    }
    if (HW(h, 0xF35C4, s32) > 0 && in_his_room(h, gCharPlayer)) {
        to_fiona(h);
        return 1;
    }
    if (in_his_room(h, gCharPlayer) && in_his_room(h, gCharPursuer)) {
        f32 d = Actor_Distance(&h->c.a, gCharPlayer->a.pos);

        if (d < Actor_Distance(&h->c.a, gCharPursuer->a.pos)) {
            to_fiona(h);
        } else {
            to_pursuer(h);
        }
        return 1;
    }
    if (in_his_room(h, gCharPlayer)) {
        to_fiona(h);
        return 1;
    }
    if (in_his_room(h, gCharPursuer)) {
        to_pursuer(h);
        return 1;
    }
    return 0;
}

/* how fast c moves (length of its velocity +0xB0) */
static f32 speed_of(Character *c) {
    sceVu0FVECTOR v;

    sceVu0CopyVector(v, c->a.unkB0);
    return __builtin_sqrtf(sceVu0InnerProduct(v, v));
}

/* c within 10 and facing away from him by more than 90 degrees: he can bite it from behind */
static s32 behind(Hewie *h, Character *c) {
    return Actor_Distance(&h->c.a, c->a.pos) < 10.0f &&
           !(hwrap_abs(h->c.a.angle[1] - c->a.angle[1]) <= 0x1.921fb6p+0f /* pi/2 */);
}

/* His own decisions when nothing else drives him: down (0x52), hurt (+0xF35B4: 0x76), dragged
 * along (Hewie_FirstDoor), Fiona panicking or held (answers by trust), angry with her, scared
 * (0x14), a scene request (+0xF3584: 0x79), a creature he follows (+0xF368C: 0x78), standing on
 * a slope (0x6E), the pursuer or a creature in reach from behind (0x7C, with +0xF356C 0x100),
 * a hole under his nose (0x6A) or Fiona right next to him (0x67, with 0x20), praise (0x7E). */
/* 0x00161860 */
void Hewie_OwnDecisions(Hewie *h) {
    Progress *p;
    Character *t;
    s32 act, g, i;
    u8 held, go;

    if (h->c.moveMode != 0) {
        HW(h, 0xF3584, u8) = 0;
        return;
    }
    if (h->c.a.unkC4 == 2) {
        act = HEWIE_ACTION(h);
        if (act != 0x52 && act != 0x74) {
            hewie_want(h, 0x52, 0);
        }
        return;
    }
    if (HW(h, 0xF35B4, s32) > 0 && HEWIE_ACTION(h) != 0x76) {
        hewie_want(h, 0x76, 0);
        return;
    }
    switch (Hewie_FirstDoor(h)) {
    case 0:
        if (HEWIE_ACTION(h) != 0x65) {
            hewie_want(h, 0x65, 0);
        }
        return;
    case 1:
        act = HEWIE_ACTION(h);
        if (act != 0x66 && act != 0x65) {
            hewie_want(h, 0x66, 0);
        }
        return;
    }
    if (HW(h, 0xF356C, u32) == 0 || (HW(h, 0xF356C, u32) & 0x80000000)) {
        return;
    }

    /* Fiona panicking (progress +0x7B8 5, then 4): once each, by chance he answers */
    if (in_his_room(h, gCharPlayer)) {
        if (h->c.moveMode == 0 && !(HW(h, 0xF3589, u8) & 2)) {
            go = 0;
            if (AT(gProgress, 0x7B8, u8) == 5) {
                HW(h, 0xF3589, u8) |= 2;
                if (HW(h, 0xF35C0, s32) != 3) {
                    go = by_chance(h, D_003B1230);
                }
            } else if (!(HW(h, 0xF3589, u8) & 1) && AT(gProgress, 0x7B8, u8) == 4) {
                HW(h, 0xF3589, u8) |= 1;
                if (HW(h, 0xF35C0, s32) != 3) {
                    go = by_chance(h, D_003B1228);
                }
            }
            if (go == 1) {
                hewie_want(h, 0x4F, 0);
                return;
            }
        }
        if (AT(gProgress, 0x7B8, u8) < 4) {
            HW(h, 0xF3589, u8) = 0;
        }
    }
    if (HW(h, 0xF35C0, s32) == 3 && staying(h)) {
        return;
    }

    /* Fiona held (her mode 4, sub 9 / 0x12): newly so, by chance he goes for whoever holds her */
    held = 0;
    if (in_his_room(h, gCharPlayer)) {
        if (gCharPlayer->moveMode == 4 && (gCharPlayer->moveSub == 9 || gCharPlayer->moveSub == 0x12)) {
            held = 1;
        }
        if (HW(h, 0xF3580, u8) == 0 && held == 1 && HW(h, 0xF35C0, s32) != 3 && by_chance(h, D_003B11E0)) {
            HW(h, 0xF3580, u8) = held;
            if (gCharPlayer->unk100 == 2) {
                hewie_want(h, 0x5A, 0);
            } else {
                HW(h, 0xF3544, Character *) = Hewie_PickTarget(h);
                hewie_want(h, 0x6D, 0);
            }
            return;
        }
    }
    p = gProgress;
    HW(h, 0xF3580, u8) = held;
    if ((Progress_GameMode(p) & 0xFF) != 2 && HW(h, 0xF366C, u8) == 0 && HW(h, 0xF366D, u8) != 0 &&
        HEWIE_ACTION(h) != 0x76) {
        hewie_want(h, 0x14, 0);
        return;
    }
    if (HW(h, 0xF3584, u8) == 1) {
        HW(h, 0xF3584, u8) = 0;
        if (HEWIE_ACTION(h) != 0x79) {
            hewie_want(h, 0x79, 0);
            return;
        }
    }

    /* the creature he follows: lost once it stops, else after it while it is ahead (within 120
       degrees) and he is on its triangle */
    if (Progress_GameMode(p) & 0xFF) {
        HW(h, 0xF368C, Character *) = NULL;
    } else {
        t = HW(h, 0xF368C, Character *);
        act = HEWIE_ACTION(h);
        if (t != NULL && t->a.active == 1 && t->a.unk2A == 0 && act != 0x78 && act != 0x76 &&
            HW(h, 0xF3570, s32) != 0x78) {
            if (speed_of(t) <= 0.5f) {
                HW(h, 0xF368C, Character *) = NULL;
            } else {
                f32 d = Actor_HeadingTo(&h->c.a, t->a.pos);

                d -= Angle_Wrap(h->c.a.angle[1] + *(f32 *)((u8 *)h->c.motion + 0x858));
                if (hwrap_abs(d) < 0x1.0c1524p+1f /* 2pi/3 */) {
                    u32 tri;

                    t = HW(h, 0xF368C, Character *);
                    tri = t->a.navTri;
                    if (Actor_TriTo(&h->c.a, t->a.pos, NAV_NONE) == tri) {
                        if (HEWIE_ACTION(h) == 0x7D) {
                            HW(h, 0xF3598, s32) = 0;
                            obey_time(h);
                            HW(h, 0xF3586, u8) = 0;
                        }
                        hewie_want(h, 0x78, 0);
                        return;
                    }
                }
            }
        }
    }

    /* settled on a slope (triangle flag 1) facing down it: lie down across (0x6E) */
    g = Hewie_AnimGroup(h);
    if ((u32)g <= 3 && HEWIE_ACTION(h) != 0x6E && (NavMesh_TriFlags(gNavMesh, h->c.a.navTri) & 1)) {
        sceVu0FVECTOR n, dir, fwd;

        VCALL(gNavMesh, 0x2C, void (*)(NavMesh *, u32, f32 *))(gNavMesh, h->c.a.navTri, n);
        if (n[1] != 1.0f) {
            *(s32 *)&n[1] = 0;
            sceVu0Normalize(dir, n);
            fwd[2] = 1.0f;
            *(s32 *)&fwd[0] = 0;
            *(s32 *)&fwd[1] = 0;
            sceVu0ApplyMatrix(fwd, h->c.a.rot, fwd);
            if (!(sceVu0InnerProduct(fwd, dir) <= 0.5f)) {
                hewie_want(h, 0x6E, 0);
                return;
            }
        }
    }

    if ((HW(h, 0xF356C, u32) & 0x80000100) == 0x100 && in_his_room(h, gCharPursuer)) {
        if (HEWIE_ACTION(h) != 0x7C && behind(h, gCharPursuer)) {
            hewie_want(h, 0x7C, 0);
            return;
        }
        for (i = 0; i < 10; i++) {
            Character *c = ((Character **)gCreatures)[i];

            if (in_his_room(h, c) && (VCALL(c, 0x3C, u32 (*)(Character *, u32))(c, i & 0xFF) & 0xFF) == 1 &&
                behind(h, c)) {
                hewie_want(h, 0x7C, 0);
                return;
            }
        }
    }

    if ((HW(h, 0xF356C, u32) & 0x80000020) == 0x20 && (g == 10 || (u32)g <= 3)) {
        if (HEWIE_ACTION(h) != 0x6A) {
            sceVu0FVECTOR nose;

            sceVu0CopyVector(nose, Skel_Bone(*(void **)((u8 *)h->c.motion + 0x810), 0x1F) + 12);
            if (Actor_TriTo(&h->c.a, nose, NAV_NONE) == NAV_NONE) {
                hewie_want(h, 0x6A, 0);
                return;
            }
        }
        if (in_his_room(h, gCharPlayer) && HEWIE_ACTION(h) != 0x67 &&
            Actor_Distance(&h->c.a, gCharPlayer->a.pos) < 5.0f) {
            hewie_want(h, 0x67, 0);
            return;
        }
    }

    if (HW(h, 0xF3586, u8) == 1) {
        HW(h, 0xF3586, u8) = 0;
        if (in_his_room(h, gCharPlayer) && HW(h, 0xF3598, s32) == 1 &&
            (HW(h, 0xF356C, u32) & 0x80000400) == 0x400) {
            act = HEWIE_ACTION(h);
            if (act != 0x7D && act != 0x7E) {
                hewie_want(h, 0x7E, 0);
            }
        }
    }
}
