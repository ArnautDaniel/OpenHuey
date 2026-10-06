/* Hewie's model (vtable D_0046B240, 0xB90 bytes, built by CharLoad_PartnerModel in model.c; two
 * subclasses D_0046B8F0 / D_0046B9B0 differ only in their destructors). On top of the model
 * base it fits his body to the floor as a dog stands - a back frame +0x7D0 from a point behind
 * him to his position, a front frame +0xB40 from his position to a point ahead - and plants
 * his four feet with IK: the front legs on three-bone solvers (+0x960, +0x9F0), the hind legs
 * on two-bone ones (+0xA80, +0xAE0).
 *
 * The feet (front right/left, hind right/left; bones 11, 15, 23, 28):
 *   +0x8A0  [4] where each foot is put (vectors)
 *   +0x8E0  [4] held where it was planted this frame (+0x8E4 last frame's)
 *   +0x8F0  [4] when a held foot lets go: its offset from the animated foot, eased out ...
 *   +0x930  [4] ... over this many frames
 *   +0x940  [4] frames of easing left
 *   +0x950  the feet have been placed once
 *   +0xB80  [4] the animation has the foot on the ground (+0xB84 last frame's)
 *   +0xB88  how far the spine bends with the slope (0..1, while the motion's flag 0x800)
 *   +0x854 / +0x858  how much he is turning / the slope, for the spine and neck */
#include "common.h"
#include "game.h"
#include "navmesh.h"
#include "sce/libvu0.h"
#include "ptmf.h"
#include "model.h"
#include "quat.h"
#include "skeleton.h"
#include "stalker_models.h"
#include "msl.h"

extern void *D_00469D00[], *D_0046ADA0[], *D_0046B1C0[], *D_0046B210[], *D_0046F9E0[];
extern void *D_0046B240[], *D_0046B8F0[], *D_0046B9B0[];
extern u8 D_003D5F90[];

#define SKEL(m) AT(m, 0x810, void *)

/* bind pose: a bone's length (+0x20) and position (+0x60) in the skeleton data +0x4C0 */
#define BIND(m, bone, off) AT(AT(m, 0x4C0, u8 *) + 0x10 + (bone) * 0x70, off, f32)

static const s32 sFootBones[4] = { 0xB, 0xF, 0x17, 0x1C };

/* ---- destructors ---- */

extern u8 D_00456EB0[];
extern u8 D_00456F70[];
void *DogModelA_Table(void);
void *DogModelB_Table(void);

static void HewieModel_Destroy(u8 *m, s32 flags) {
    AT(m, 0x0, void **) = D_0046B240;
    func_001002C0(m + 0xA80, IK2_Destroy, 0x60, 2);
    func_001002C0(m + 0x960, IK3_Destroy, 0x90, 2);
    AT(m, 0x0, void **) = D_0046F9E0;
    AT(m, 0x0, void **) = D_0046B210;
    AT(m, 0x1D0, void **) = D_0046B1C0;
    AT(m, 0x1D0, void **) = D_00469D00;
    AT(m, 0x10, void **) = D_0046ADA0;
    AT(m, 0x10, void **) = D_00469D00;
    if ((s16)flags > 0) {
        StalkerModel_delete(m);
    }
}

/* +0x8 */
/* 0x001F7D40 */
void *DogModel_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        HewieModel_Destroy(m, flags);
    }
    return m;
}

/* 0x0020BDE0 */
void *DogModelA_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = D_0046B8F0;
        HewieModel_Destroy(m, flags);
    }
    return m;
}

/* 0x0020BF80 */
void *DogModelB_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = D_0046B9B0;
        HewieModel_Destroy(m, flags);
    }
    return m;
}

extern u8 D_00456ED0[], D_00456EF0[], D_00456F10[], D_00456F30[], D_00456F50[];
extern u8 D_00456E10[], D_00456E30[], D_00456E50[], D_00456E70[], D_00456E90[];

/* (vtable D_0046B8F0 +0xA0) its table for kind k (0..4), NULL for others */
/* 0x0020BEF0 */
u8 *DogModelA_KindTable(u8 *m, s32 k) {
    switch (k) {
    case 0: return D_00456F50;
    case 1: return D_00456F30;
    case 2: return D_00456F10;
    case 3: return D_00456EF0;
    case 4: return D_00456ED0;
    }
    return NULL;
}

/* 0x0020BF70 */
void *DogModelA_Table(void) {
    return D_00456F70;
}

/* (vtable D_0046B9B0's) the same for the other model */
/* 0x0020C090 */
u8 *DogModelB_KindTable(u8 *m, s32 k) {
    switch (k) {
    case 0: return D_00456E90;
    case 1: return D_00456E70;
    case 2: return D_00456E50;
    case 3: return D_00456E30;
    case 4: return D_00456E10;
    }
    return NULL;
}

/* 0x0020C110 */
void *DogModelB_Table(void) {
    return D_00456EB0;
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

/* +0x10 */
/* 0x001F9560 */
void DogModel_Frame(u8 *m) {
    Model_Frame(m);
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

/* +0x18: each frame: the feet's contact from the animation */
/* 0x001F7FA0 */
void DogModel_FeetContact(u8 *m) {
    s32 i;

    for (i = 0; i < 4; i++) {
        AT(m, 0xB84 + i, u8) = AT(m, 0xB80 + i, u8);
        AT(m, 0xB80 + i, u8) = VCALL(m, 0x64, u8 (*)(u8 *, s32, s32))(m, i, 0);
    }
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

/* foot `i`'s position in the animation (copied out with its bone's matrix) */
static void foot_at(u8 *m, s32 i, f32 *out) {
    f32 b[4][4] __attribute__((aligned(16)));

    sceVu0CopyMatrix(b, (f32 (*)[4])Skel_Bone(SKEL(m), sFootBones[i]));
    sceVu0CopyVector(out, b[3]);
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

/* ---- the body on the floor ---- */

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
    func_0010E5F0(f[2], dir);
    sceVu0OuterProduct(f[1], (f32 *)dir, f[0]);
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
        func_0010E5F0((f32 *)(m + 0x800), (f32 *)(a + 0x10));
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
    func_0010E5F0((f32 *)(m + 0x800), (f32 *)(a + 0x10));
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

/* ---- the bones ---- */

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
