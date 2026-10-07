/* Character models (see docs/model_format.md): the model classes' construction. Model base
 * class vtable Model_vtable (constructed through ModelBase_vtable); Hewie's model DogModel_vtable.
 *
 * (was stalker_models.c) The stalkers' models (the character model classes their loaders build,
 * model.c), and the trivial methods of the model base class (vtable Model_vtable) that natively
 * were still stubs. The models' secondary motion (springs for hanging parts) works as Fiona's
 * (model.c).
 */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "navmesh.h"
#include "model.h"
#include "input.h"
#include "globals.h"
#include "actor.h"
#include "ptmf.h"
#include "memcard.h"
#include "pursuer.h"
#include "heap.h"
#include "event.h"
#include "vecmath.h"
#include "scene_game.h"
#include "lights.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif
#include "msl.h"
#include "hewie.h"
#include "sound.h"
#include "effectmgr.h"
#include "debilitas.h"
#include "fiona.h"
#include "libc.h"
#include "effectmgr.h"   /* HitEffect_Spawn */
#include "item.h"
#include "renderer.h"
#include "charaction.h"
#include "gl2d.h"
#include "daniella.h"
#include "debilitas2.h"
#include "lorenzo.h"
#include "system.h"
#include "char_load.h"

extern void *Helper469D00_vtable[], *ModelDrawer_vtable[], *ModelBase_vtable[], *Model_vtable[], *DogModel_vtable[], *IK3_vtable[];

extern void *HangPoint_vtable[];
extern void *Capsule_vtable[];
extern void *BonePoint_vtable[];
extern void *BoneHangPoint_vtable[];
extern void *SprungPoint_vtable[];
extern void *HairPoint_vtable[];
extern void *SwayPointA_vtable[];
extern void *SwayPointB_vtable[];
extern void *HangingPart_vtable[];
extern void *Part50_vtable[];
extern void *Part60_vtable[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *Part_ctor(u8 *p);
void *Capsule_ctor(u8 *p);
void *Part60_ctor(u8 *p);
void *SpringSet_ctor(u8 *p);
void *BonePoint_ctor(u8 *p);
void *HangPoint_ctor(u8 *p);
void *SwayPointA_ctor(u8 *p);
void *SprungPoint_ctor(u8 *p);
void *BoneHangPoint_ctor(u8 *p);
void *IK2_ctor(u8 *p);
void *SwayPointB_ctor(u8 *p);
void *Part50_ctor(u8 *p);
void *HairPoint_ctor(u8 *p);
void *HangingPart_ctor(u8 *p);

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern u8 D_0045E4C0[];
extern u8 str_O_FIN_FIN_200_TEX[];

extern void Mtx_Model(f32 (*mtx)[4], const f32 *pos, f32 heading);
extern void *gSkelPool;   /* the skeleton pool */
extern void *gChainPool;   /* the chain pool (motion buffers) */
extern void Mtx_Model(f32 (*mtx)[4], const f32 *pos, f32 heading);
/* a part's anchor: its bone's position when anchored (+0x20), else the point it hangs from */
static inline void Part_Anchor(u8 *p, u8 *set, f32 *at) {
    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(at, Skel_Bone(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(p, 0x24, s32)) + 12);
    } else {
        sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
    }
}

/* a part kept at its length from its anchor; its velocity is how far it went */
static inline void Part_Hold(u8 *p, const f32 *at, const f32 *prev) {
    f32 d[4] __attribute__((aligned(16)));

    sceVu0SubVector(d, (f32 *)p, (f32 *)at);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)p, (f32 *)at, d);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)p, (f32 *)prev);
}

static inline void SwayPoint_Step(u8 *p, u8 *set) {
    f32 at[4] __attribute__((aligned(16)));
    f32 out[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 *vel = (f32 *)(p + 0x10);
    f32 *own = Skel_Bone(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(p, 0x24, s32));
    f32 *from = Skel_Bone(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(p, 0x44, s32));
    f32 ang, dot;

    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(at, own + 12);
    } else {
        sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
    }
    sceVu0CopyVector(prev, (f32 *)p);
    /* the set's force works along the bone it hangs from */
    sceVu0ScaleVector(d, from, AT(set, 0x4, f32));
    sceVu0AddVector(vel, vel, d);
    sceVu0ScaleVector(vel, vel, AT(set, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, vel);
    sceVu0SubVector(d, (f32 *)p, at);
    sceVu0Normalize(d, d);
    /* swung too far off the bone's X: turn it back onto the cone's edge */
    ang = msl_acosf(sceVu0InnerProduct(d, from));
    if (!(ang <= AT(p, 0x4C, f32))) {
        f32 s = msl_sinf(ang);

        if (!(s <= 0.0f)) {
            f32 near = msl_sinf(AT(p, 0x4C, f32));
            f32 far = msl_sinf(ang - AT(p, 0x4C, f32));
            f32 inv = 1.0f / s;

            d[0] = inv * (far * from[0] + near * d[0]);
            d[1] = inv * (far * from[1] + near * d[1]);
            d[2] = inv * (far * from[2] + near * d[2]);
        }
    }
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)p, at, d);
    sceVu0SubVector(vel, (f32 *)p, prev);
    /* never behind the bone's outward side: pushed back out, at its length */
    sceVu0ApplyMatrix(out, (f32 (*)[4])from, AT(p, 0x48, f32 *));
    dot = sceVu0InnerProduct(d, out);
    if (dot < 0.0f) {
        sceVu0CopyVector(prev, (f32 *)p);
        sceVu0ScaleVector(d, out, -dot);
        sceVu0AddVector((f32 *)p, (f32 *)p, d);
        sceVu0SubVector(d, (f32 *)p, at);
        sceVu0Normalize(d, d);
        sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
        sceVu0AddVector((f32 *)p, at, d);
        sceVu0SubVector(d, (f32 *)p, prev);
        sceVu0AddVector(vel, vel, d);
    }
}

/* its bone: X from the anchor to the point, Y the hanging bone's outward axis, at the anchor */
static inline void SwayPoint_Pose(u8 *p, u8 *set) {
    f32 at[4] __attribute__((aligned(16)));
    f32 (*own)[4] = (f32 (*)[4])Skel_Bone(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(p, 0x24, s32));
    f32 *from = Skel_Bone(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(p, 0x44, s32));

    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(at, own[3]);
    } else {
        sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
    }
    sceVu0SubVector(own[0], (f32 *)p, at);
    sceVu0ApplyMatrix(own[1], (f32 (*)[4])from, AT(p, 0x48, f32 *));
    sceVu0OuterProduct(own[2], own[0], own[1]);
    sceVu0OuterProduct(own[1], own[2], own[0]);
    sceVu0Normalize(own[0], own[0]);
    sceVu0Normalize(own[1], own[1]);
    sceVu0Normalize(own[2], own[2]);
    sceVu0CopyVector(own[3], at);
}

void Model_Vt1C(u8 *m);
void Model_Vt20(u8 *m);
void Model_Vt24(u8 *m);
void Model_Vt2C(u8 *m);
void Model_Vt30(u8 *m);
void Model_AdjustBone(u8 *m);
void Model_Vt3C(u8 *m);
void HumanModel_Vt3C(u8 *m);
void Model_PlantFeet(u8 *m);
void Model_FeetReplace(u8 *m);
void Model_FeetUp(u8 *m);
void Model_HeadPos(u8 *m);
s32 Model_VtAC(u8 *m);
s32 Model_VtB0(u8 *m);
s32 Model_Part3(u8 *m);
s32 Model_Part2(u8 *m);
s32 Model_Part1(u8 *m);
s32 Model_Part0(u8 *m);
s32 Model_Vt78(u8 *m);
s32 Model_Vt74(u8 *m);
s32 Model_Vt70(u8 *m);
s32 Model_Vt94(u8 *m);
s32 Model_Vt98(u8 *m);
s32 Model_Vt9C(u8 *m);
s32 Model_Vt7C(u8 *m);
s32 Model_HeadBone(u8 *m);
s32 Model_MarkerFile(u8 *m);
s32 Model_ModelFile(u8 *m);
s32 HumanModel_Vt74(u8 *m);
s32 HumanModel_Vt78(u8 *m);
s32 HumanModel_Vt7C(u8 *m);
s32 HumanModel_HeadBone(u8 *m);
f32 HumanModel_Vt58(u8 *m);
void Model_Set878(u8 *m, s32 v);
void Model_SetMatrix(u8 *m, f32 (*mtx)[4]);
void Model_SecondaryMotion(u8 *m);
void SpringPart_Reset(u8 *e);
void SpringPart_NoopC(u8 *e);
void SpringPart_Noop10(u8 *e);
void Capsule_Set(u8 *cap, s32 b1, s32 b2, f32 x1, f32 y1, f32 z1, f32 r, f32 x2, f32 y2, f32 z2);
void Capsule_Push(u8 *cap, f32 *out, const f32 *pt, f32 k);
void HairPoint_Pose(u8 *pt, u8 *set);
void HairPoint_Step(u8 *pt, u8 *set);
void HangingPart_Step(u8 *p, u8 *set);
void Capsule_Update(u8 *cap, u8 *m);
void Part60_Step(u8 *p, u8 *set);
void Part50_Step(u8 *p, u8 *set);
void SwayPointB_Step(u8 *p, u8 *set);
void SwayPointA_Step(u8 *p, u8 *set);
void SwayPointB_Pose(u8 *p, u8 *set);
void SwayPointA_Pose(u8 *p, u8 *set);

static void b4_clear_dca70(u8 *p) {
    s32 i;

    F(p, 0x38, u32) = 0;
    F(p, 0x3C, u32) = 0;
    F(p, 0x40, u32) = 0;
    F(p, 0x48, u32) = 0;
    F(p, 0x4C, u32) = 0;
    F(p, 0x50, u32) = 0;
    for (i = 0; i < 16; i++) {
        F(p, 0x58 + i * 4, u32) = 0;
    }
    F(p, 0x870, u32) = 0;
}

/* 0x00127800 */
void *Model_dtor(void **m, s32 flags) {
    if (m != NULL) {
        m[0] = Model_vtable;
        if (m != NULL) {
            void **a = (void **)((u8 *)m + 0x1D0);
            void **b = (void **)((u8 *)m + 0x10);

            m[0] = ModelBase_vtable;
            if (a != NULL) {
                *a = Shadow_vtable;
                if (a != NULL) {
                    *a = Helper469D00_vtable;
                }
            }
            if (b != NULL) {
                *b = ModelDrawer_vtable;
                if (b != NULL) {
                    *b = Helper469D00_vtable;
                }
            }
        }
        if ((s16)flags > 0) {
            StalkerModel_delete(m);
        }
    }
    return m;
}

/* 0x001689F0 */
s32 Model_Textures(Hewie *h) {
    return 0;
}

/* +0x74 .. +0xB0 (and the human models' +0x84 .. +0x90): none */
/* 0x0016D040 */
s32 Model_VtAC(u8 *m) {
    return 0;
}

/* 0x0016D170 */
s32 Model_VtB0(u8 *m) {
    return 0;
}
/* placement new (models) */
/* 0x002DC6E0 */
void *Model_new(u32 size, void *p) {
    return p;
}

/* 0x002DC6F0 */
s32 Model_MarkerFile(u8 *m) {
    return 0;
}

/* 0x002DC700 */
s32 Model_ModelFile(u8 *m) {
    return 0;
}

/* the model's drawing object (vtable ModelDrawer_vtable, on the overlay base Helper469D00_vtable) */
static inline void DrawObj_Init(u8 *o) {
    s32 i;

    AT(o, 0x0, void **) = Helper469D00_vtable;
    AT(o, 0x4, s32) = -1;
    AT(o, 0x0, void **) = ModelDrawer_vtable;
    AT(o, 0xC, s32) = 0;
    AT(o, 0x10, s32) = 0;
    AT(o, 0x8, s32) = 0;
    AT(o, 0x14, u8) = 0xFF;
    AT(o, 0x28, s32) = 0;
    AT(o, 0x2C, s32) = 0;
    AT(o, 0x30, s32) = 0;
    AT(o, 0x38, s32) = 0;
    AT(o, 0x3C, s32) = 0;
    AT(o, 0x40, s32) = 0;
    for (i = 0; i < 16; i++) {
        AT(o, 0x48 + i * 4, s32) = 0;
    }
    for (i = 0; i < 0x70; i++) {
        AT(o, 0x88 + i, u8) = 0;
    }
    AT(o, 0x20, u8) = 0;
    AT(o, 0x24, s32) = 0;
}

/* 0x0016F770 */
void *DrawObj_ctor(u8 *o) {
    DrawObj_Init(o);
    return o;
}

/* the model base's own fields (after its drawing object and +0x1D0 part) */
static inline void ModelBase_Zero(u8 *m) {
    s32 i, k;

    for (i = 0; i < 2; i++) {
        AT(m, 0x584 + i * 0xA0, s32) = 0;
        AT(m, 0x58C + i * 0xA0, s32) = 0;
        AT(m, 0x594 + i * 0xA0, s32) = 0;
        AT(m, 0x588 + i * 0xA0, s32) = 0;
        AT(m, 0x590 + i * 0xA0, s32) = 0;
        AT(m, 0x598 + i * 0xA0, s32) = 0;
        for (k = 0; k < 24; k++) {
            AT(m, 0x59C + i * 0xA0 + k * 4, s32) = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        AT(m, 0x6DC + i * 0x60, s32) = 0;
        AT(m, 0x6E0 + i * 0x60, s32) = 0;
        AT(m, 0x6F8 + i * 0x60, s32) = 0;
        AT(m, 0x6FC + i * 0x60, s32) = 0;
    }
    AT(m, 0x4C0, s32) = 0;
    AT(m, 0x4C4, s32) = 0;
    AT(m, 0x4C8, s32) = 0;
    AT(m, 0x4CC, s32) = 0;
    AT(m, 0x4D0, s32) = 0;
    AT(m, 0x4D4, s32) = 0;
    AT(m, 0x4D9, u8) = 0;
    AT(m, 0x840, u16) = 0;
    AT(m, 0x844, s32) = 0;
}

/* the model base class's constructor */
/* 0x0016F4B0 */
void *ModelBase_ctor(u8 *m) {
    AT(m, 0x0, void **) = ModelBase_vtable;
    DrawObj_ctor(m + 0x10);
    Part_ctor(m + 0x1D0);
    ModelBase_Zero(m);
    AT(m, 0x0, void **) = Model_vtable;
    return m;
}

/* the partner's (Hewie's) model: allocated from the scene heap, put at character `slot` +0xF0 */
/* 0x003A10B0 */
void CharLoad_PartnerModel(Progress *p, u32 slot) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    u8 *m = Model_new(0xB90, VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0xB90));

    if (m != NULL) {
        ModelBase_ctor(m);
        AT(m, 0x0, void **) = DogModel_vtable;
        AT(m, 0x890, u8) = 0;
        __construct_array(m + 0x960, DogModelArray_ctor, IK3_Destroy, 0x90, 2);
        __construct_array(m + 0xA80, IK2_ctor, IK2_Destroy, 0x60, 2);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

extern void *DogModelB_vtable[];

/* ---- constructors (vtables and their quad-drawer parts) of the effects the event commands
 * make (0xC8, 0xD9, 0x9C, 0x9F, 0xA0, 0xA9, 0x35, 0x65, 0x9B, 0x8C) and a few others ---- */

extern void *DustMoteSource_vtable[], *SpriteBurst_vtable[], *Effect71000_vtable[], *Splash_vtable[], *SpeckSwarm_vtable[], *Fog_vtable[],
    *RoomEffectBase_vtable[], *ScreenBlend_vtable[], *CharModel_vtable[], *Costume8Model_vtable[], *WindHangPoint_vtable[], *SpringPartBase_vtable[],
    *CostumeHangPoint_vtable[], *HairPoint2_vtable[], *Effect6FF60_vtable[], *DepthRange_vtable[], *Butterflies_vtable[];
extern void *Helper469D00_vtable[], *QuadDrawer_vtable[];
extern void *HumanModel_PartsCtor(u8 *m);

/* 0x002082A0 */
void **Effect71000_Init(void **o) {   /* event 0xA9 */
    o[0] = Effect71000_vtable;
    return o;
}

/* destructors / constructors of three kinds with a part at +0x30 (based on SpringPartBase_vtable) */
static inline void *part30_dtor(void *o, s32 flags, void **vtbl) {
    if (o != NULL) {
        AT(o, 0x30, void **) = vtbl;
        if (o != NULL) {
            AT(o, 0x30, void **) = SpringPartBase_vtable;
        }
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* 0x002088F0 */
void *WindHangPoint_dtor(void *o, s32 flags) {
    return part30_dtor(o, flags, WindHangPoint_vtable);
}

/* 0x00208950 */
void *WindHangPoint_ctor(void *o) {
    AT(o, 0x30, void **) = WindHangPoint_vtable;
    return o;
}

/* 0x00208970 */
void *CostumeHangPoint_dtor(void *o, s32 flags) {
    return part30_dtor(o, flags, CostumeHangPoint_vtable);
}

/* 0x002089D0 */
void *CostumeHangPoint_ctor(void *o) {
    AT(o, 0x30, void **) = CostumeHangPoint_vtable;
    return o;
}

/* 0x00208E30 */
void *HairPoint2_dtor(void *o, s32 flags) {
    return part30_dtor(o, flags, HairPoint2_vtable);
}

/* 0x00208EB0 */
void **Effect6FF60_Init(void **o) {   /* event 0x8C */
    o[0] = Effect6FF60_vtable;
    return o;
}

/* 0x0020CAD0 */
s32 Model_HeadBone(u8 *m) {
    return 0;
}

/* +0xB4: no secondary-motion table */
/* 0x00210DF0 */
void Model_SecondaryMotion(u8 *m) {
    AT(m, 0x874, void *) = NULL;
}

/* ---- the costume models event 0x97 makes ---- */

extern void *DogModelA_vtable[], *Costume7Model_vtable[], *Costume6Model_vtable[], *Costume3Model_vtable[], *Costume2Model_vtable[], *BonePoint_vtable[],
    *SprungPoint_vtable[], *BoneHangPoint_vtable[], *FionaModel_vtable[];
extern void *SwayPointA_dtor(void *, s32);
void *FionaModelArray_ctor(void *p);

/* the second dog model (DogModelB_vtable, 0xB90 bytes) for character `slot` */
/* 0x003A0F90 */
void CharLoad_DogModelB(Progress *p, u32 slot) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    u8 *m = Model_new(0xB90, VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0xB90));

    if (m != NULL) {
        DogModel_ctor(m, 2);
        AT(m, 0x0, void **) = DogModelB_vtable;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

extern void *HumanModel_vtable[], *CharModel_vtable[], *FionaModel_vtable[], *HairPoint2_vtable[];
extern void *HairPoint2_dtor(void *, s32);

/* the human characters' model base: the model, two of 0x60 at +0x8D0 / +0x930, kind +0x9A0 */
/* 0x00170690 */
void *HumanModel_ctor(u8 *m, s32 kind) {
    ModelBase_ctor(m);
    AT(m, 0x0, void **) = HumanModel_vtable;
    IK2_ctor(m + 0x8D0);
    IK2_ctor(m + 0x930);
    AT(m, 0x0, void **) = CharModel_vtable;
    AT(m, 0x9A0, u8) = kind;
    return m;
}

/* 0x001706F0 */
void *IK2_ctor(u8 *p) {
    FLD(p, 0x58, void **) = IK2_vtable;
    return p;
}

/* the player's (Fiona's) model (0x1820 bytes), put at character `slot` +0xF0 */
/* 0x003A1860 */
void CharLoad_FionaModel(Progress *p, u32 slot) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    u8 *m = Model_new(0x1820, VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0x1820));

    if (m != NULL) {
        u8 *e;

        HumanModel_ctor(m, 1);
        AT(m, 0x0, void **) = FionaModel_vtable;
        __construct_array(m + 0x9B0, FionaModelArray_ctor, HairPoint2_dtor, 0x50, 0x20);
        SpringSet_ctor(m + 0x13B0);
        BoneHangPoint_ctor(m + 0x13F0);
        SpringSet_ctor(m + 0x1440);
        __construct_array(m + 0x1480, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
        for (e = m + 0x15C0; e < m + 0x1740; e += 0x40) {
            BonePoint_ctor(e);
        }
        SpringSet_ctor(m + 0x1740);
        SprungPoint_ctor(m + 0x1780);
        SpringSet_ctor(m + 0x17E0);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

extern void *EventHumanModel_vtable[];

/* Fiona's model in her clothes (0x1270 bytes, vtable EventHumanModel_vtable), put at character `slot` +0xF0:
 * twelve 0x50 nodes, single parts, four 0x50 nodes, six parts of 0x40 and more single parts */
/* 0x003A1720 */
void CharLoad_FionaClothes(Progress *p, u32 slot) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);
    u8 *m = Model_new(0x1270, VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0x1270));

    if (m != NULL) {
        u8 *e;

        HumanModel_ctor(m, 0);
        AT(m, 0x0, void **) = EventHumanModel_vtable;
        __construct_array(m + 0x9B0, SwayPointA_ctor, SwayPointA_dtor, 0x50, 0xC);
        SpringSet_ctor(m + 0xD70);
        BoneHangPoint_ctor(m + 0xDB0);
        SpringSet_ctor(m + 0xE00);
        __construct_array(m + 0xE40, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
        for (e = m + 0xF80; e < m + 0x1100; e += 0x40) {
            BonePoint_ctor(e);
        }
        SpringSet_ctor(m + 0x1100);
        SprungPoint_ctor(m + 0x1140);
        SpringSet_ctor(m + 0x11A0);
        BoneHangPoint_ctor(m + 0x11E0);
        SpringSet_ctor(m + 0x1230);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* ---- the other setups SceneGame_StateEntry picks by progress variables 0x26 (Fiona's
 * costume) and 0x27 (the partner): each model allocated from the scene heap and put at
 * character `slot` +0xF0 ---- */

extern void *Costume7Model_vtable[], *Costume6Model_vtable[], *Costume3Model_vtable[], *Costume2Model_vtable[];
extern void *SwayPointA_dtor(void *, s32);
extern void *CostumeHangPoint_ctor(void *);
extern void *CostumeHangPoint_dtor(void *, s32);
extern void *WindHangPoint_ctor(void *);
extern void *WindHangPoint_dtor(void *, s32);

static inline u8 *setup_alloc(Progress *p, u32 size) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);

    return Model_new(size, VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, size));
}

/* parts of 0x40 from `from` to `to` */
static inline void setup_parts40(u8 *from, u8 *to) {
    u8 *e;

    for (e = from; e < to; e += 0x40) {
        BonePoint_ctor(e);
    }
}

/* partner 1: the first dog model (DogModelA_vtable) */
/* 0x003A1020 */
void CharLoad_DogModelA(Progress *p, u32 slot) {
    u8 *m = setup_alloc(p, 0xB90);

    if (m != NULL) {
        DogModel_ctor(m, 1);
        AT(m, 0x0, void **) = DogModelA_vtable;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* Fiona's costume 8 (0x9B0 bytes, the bare base) */
/* 0x003A1190 */
void CharLoad_Costume8(Progress *p, u32 slot) {
    u8 *m = setup_alloc(p, 0x9B0);

    if (m != NULL) {
        HumanModel_ctor(m, 8);
        AT(m, 0x0, void **) = Costume8Model_vtable;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* Fiona's costume 7 (0xD50 bytes; as Costume7Model_ctor) */
/* 0x003A1220 */
void CharLoad_Costume7(Progress *p, u32 slot) {
    u8 *m = setup_alloc(p, 0xD50);

    if (m != NULL) {
        HumanModel_ctor(m, 7);
        AT(m, 0x0, void **) = Costume7Model_vtable;
        __construct_array(m + 0x9B0, BoneHangPoint_ctor, BoneHangPoint_dtor, 0x50, 5);
        SpringSet_ctor(m + 0xB40);
        SprungPoint_ctor(m + 0xB80);
        SpringSet_ctor(m + 0xBE0);
        SprungPoint_ctor(m + 0xC20);
        SpringSet_ctor(m + 0xC80);
        BoneHangPoint_ctor(m + 0xCC0);
        SpringSet_ctor(m + 0xD10);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* Fiona's costume 6 (0xDE0 bytes; as Costume6Model_ctor) */
/* 0x003A1310 */
void CharLoad_Costume6(Progress *p, u32 slot) {
    u8 *m = setup_alloc(p, 0xDE0);

    if (m != NULL) {
        HumanModel_ctor(m, 6);
        AT(m, 0x0, void **) = Costume6Model_vtable;
        BoneHangPoint_ctor(m + 0x9B0);
        SpringSet_ctor(m + 0xA00);
        __construct_array(m + 0xA40, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
        setup_parts40(m + 0xB80, m + 0xD00);
        SpringSet_ctor(m + 0xD00);   /* (the loop's end, passed through a0) */
        SprungPoint_ctor(m + 0xD40);
        SpringSet_ctor(m + 0xDA0);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* Fiona's costumes 3 and 2 (as costume_model): n 0x50 nodes, single parts, sixteen 0x50
 * nodes, parts of 0x40, three 0x50 nodes and more parts */
static inline void setup_costume(u8 *m, s32 kind, void **vtbl, s32 n, u32 at) {
    HumanModel_ctor(m, kind);
    AT(m, 0x0, void **) = vtbl;
    __construct_array(m + 0x9B0, SwayPointA_ctor, SwayPointA_dtor, 0x50, n);
    SpringSet_ctor(m + at);
    BoneHangPoint_ctor(m + at + 0x40);
    SpringSet_ctor(m + at + 0x90);
    SprungPoint_ctor(m + at + 0xD0);
    SpringSet_ctor(m + at + 0x130);
    BoneHangPoint_ctor(m + at + 0x170);
    SpringSet_ctor(m + at + 0x1C0);
    __construct_array(m + at + 0x200, CostumeHangPoint_ctor, CostumeHangPoint_dtor, 0x50, 0x10);
    SpringSet_ctor(m + at + 0x700);
    setup_parts40(m + at + 0x740, m + at + 0x8C0);
    __construct_array(m + at + 0x8C0, WindHangPoint_ctor, WindHangPoint_dtor, 0x50, 3);   /* (the loop's end, a0) */
    SpringSet_ctor(m + at + 0x9B0);
    setup_parts40(m + at + 0x9F0, m + at + 0xB70);
}

/* Fiona's costume 3 (0x17A0 bytes) */
/* 0x003A1420 */
void CharLoad_Costume3(Progress *p, u32 slot) {
    u8 *m = setup_alloc(p, 0x17A0);

    if (m != NULL) {
        setup_costume(m, 3, Costume3Model_vtable, 8, 0xC30);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* Fiona's costume 2 (0x18E0 bytes) */
/* 0x003A15A0 */
void CharLoad_Costume2(Progress *p, u32 slot) {
    u8 *m = setup_alloc(p, 0x18E0);

    if (m != NULL) {
        setup_costume(m, 2, Costume2Model_vtable, 0xC, 0xD70);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* ---- Fiona's model in her clothes (vtable EventHumanModel_vtable, O_FIN, costumes 0..6): its own
 * methods. Its secondary motion: five spring sets - +0xD70 the 12 hair / clothes nodes at
 * +0x9B0, +0xE00 one node (+0xDB0), +0x1100 four hanging nodes (+0xE40) with six collision
 * spheres (+0xF80), +0x11A0 one node (+0x1140), +0x1230 one node (+0x11E0) ---- */

extern void CharModel_Loaded(u8 *m);
extern const char D_0045E3C0[], str_O_FIN_FIN_001_PCK[], str_O_FIN_FIN_002_PCK[], str_O_FIN_FIN_003_PCK[], str_O_FIN_FIN_004_PCK[],
    str_O_FIN_FIN_005_PCK[], str_O_FIN_FIN_006_PCK[];   /* "O_FIN\FIN_00n.PCK" */
extern u8 D_0041A4B0[], D_0041A4C0[], D_0041A4D0[], D_0041A4E0[], D_0041A4F0[];
extern void CharModel_Frame(u8 *m);
extern void *SprungPoint_vtable[], *SpringPartBase_vtable[], *BoneHangPoint_vtable[];
void *HumanModel_dtor(u8 *m, s32 flags);

/* append a node to a spring set's node list (+0x30 head, +0x34 tail; the node's next +0x28,
 * prev +0x2C) */
static inline void spring_link(u8 *set, u8 *node) {
    if (AT(set, 0x30, u8 *) != NULL && AT(set, 0x34, u8 *) != NULL) {
        AT(AT(set, 0x34, u8 *), 0x28, u8 *) = node;
        AT(node, 0x28, u8 *) = NULL;
        AT(node, 0x2C, u8 *) = AT(set, 0x34, u8 *);
        AT(set, 0x34, u8 *) = node;
    } else {
        AT(set, 0x34, u8 *) = node;
        AT(set, 0x30, u8 *) = node;
        AT(node, 0x2C, u8 *) = NULL;
        AT(node, 0x28, u8 *) = NULL;
    }
}

/* a spring set's gravity (0, g, 0), damping and owner */
static inline void spring_set(u8 *set, u32 g, u32 damping, u8 *owner) {
    AT(set, 0x0, f32) = 0.0f;
    AT(set, 0x4, u32) = g;
    AT(set, 0x8, f32) = 0.0f;
    AT(set, 0x10, u32) = damping;
    AT(set, 0x14, u8 *) = owner;
    AT(set, 0x20, u8) = 0;
    AT(set, 0x1C, s32) = 0;
}

/* +0x84..+0x94: part roles */
/* 0x002F6E10 */
s32 EventHumanModel_Part0(void) { return 3; }
/* 0x002F6E20 */
s32 EventHumanModel_Part1(void) { return 7; }
/* 0x002F6E30 */
s32 EventHumanModel_Part2(void) { return 0x1C; }
/* 0x002F6E40 */
s32 EventHumanModel_Part3(void) { return 0x2C; }
/* 0x002F6E50 */
s32 EventHumanModel_Part4(void) { return 0x17; }

/* +0xB0 the model file's buffer size */
/* 0x002F6DE0 */
u32 EventHumanModel_BufferSize(void) {
    return 0x41000;
}

/* +0xBC */
/* 0x002F6DF0 */
void EventHumanModel_SetFlag(u8 *m, s32 on, f32 x) {
    AT(m, 0x111C, f32) = x;
    AT(m, 0x1120, u8) = on;
}

/* +0xC0 the point at +0x1190 */
/* 0x002F6E00 */
void EventHumanModel_SetPoint(u8 *m, f32 x, f32 y, f32 z) {
    AT(m, 0x1190, f32) = x;
    AT(m, 0x1194, f32) = y;
    AT(m, 0x1198, f32) = z;
}

/* +0xB8: 16 entries of the table D_0041A4B0 (+0x840 count, +0x844 table) */
/* 0x002F7A10 */
void EventHumanModel_MotionTable(u8 *m) {
    AT(m, 0x840, s16) = 16;
    AT(m, 0x844, u8 *) = D_0041A4B0;
}

/* 0x002F8450 */
void SwayPointA_Pose(u8 *p, u8 *set) {
    SwayPoint_Pose(p, set);
}

/* 0x002F8550 */
void SwayPointA_Step(u8 *p, u8 *set) {
    SwayPoint_Step(p, set);
}

/* +0x14: pose its bone */
/* 0x00311C70 */
void SwayPointB_Pose(u8 *p, u8 *set) {
    SwayPoint_Pose(p, set);
}

/* +0x10: step */
/* 0x00311D70 */
void SwayPointB_Step(u8 *p, u8 *set) {
    SwayPoint_Step(p, set);
}

/* +0xC4 which of the swappable parts show (part flag 2 hides): 0..4 one of the four at
 * +0xD8 / +0xE0 / +0xE2 / +0xDA (0 none), 5..9 one set of the eight at +0x9E.. */
/* 0x002F6E60 */
void EventHumanModel_ShowParts(u8 *m, s32 look) {
    u32 k = look & 0xFF;

    if (k < 5) {
        AT(m, 0xD8, u8) |= 2;
        AT(m, 0xE0, u8) |= 2;
        AT(m, 0xE2, u8) |= 2;
        AT(m, 0xDA, u8) |= 2;
        switch (k) {
        case 1: AT(m, 0xD8, u8) &= ~2; break;
        case 2: AT(m, 0xE0, u8) &= ~2; break;
        case 3: AT(m, 0xE2, u8) &= ~2; break;
        case 4: AT(m, 0xDA, u8) &= ~2; break;
        }
        return;
    }
    AT(m, 0x9E, u8) |= 2;
    AT(m, 0xA0, u8) |= 2;
    AT(m, 0xA2, u8) |= 2;
    AT(m, 0xC4, u8) |= 2;
    AT(m, 0xA4, u8) |= 2;
    AT(m, 0xA6, u8) |= 2;
    AT(m, 0xC6, u8) |= 2;
    AT(m, 0xC8, u8) |= 2;
    switch (k) {
    case 5:
        AT(m, 0x9E, u8) &= ~2;
        AT(m, 0xA0, u8) &= ~2;
        break;
    case 6: AT(m, 0xA2, u8) &= ~2; break;
    case 7: AT(m, 0xC4, u8) &= ~2; break;
    case 8:
        AT(m, 0xA4, u8) &= ~2;
        AT(m, 0xA6, u8) &= ~2;
        break;
    case 9:
        AT(m, 0xC6, u8) &= ~2;
        AT(m, 0xC8, u8) &= ~2;
        break;
    }
}

/* +0xA0 the model file of costume `n` */
/* 0x002F7030 */
const char *EventHumanModel_ModelFile(void *m, u32 n) {
    switch (n) {
    case 0: return D_0045E3C0;
    case 1: return str_O_FIN_FIN_001_PCK;
    case 2: return str_O_FIN_FIN_002_PCK;
    case 3: return str_O_FIN_FIN_003_PCK;
    case 4: return str_O_FIN_FIN_004_PCK;
    case 5: return str_O_FIN_FIN_005_PCK;
    case 6: return str_O_FIN_FIN_006_PCK;
    }
    return NULL;
}

/* the one-node set +0x11A0: node +0x1140 on bone 0x19 */
/* 0x002F70C0 */
void EventHumanModel_Set11A0(u8 *m) {
    u8 *node = m + 0x1140;

    SpringSet_Clear(m + 0x11A0);
    spring_link(m + 0x11A0, node);
    AT(m, 0x11A0, f32) = 0.0f;
    AT(m, 0x11A4, f32) = 0.0f;
    AT(m, 0x11A8, f32) = 0.0f;
    AT(m, 0x11B0, f32) = 0.75f;
    AT(m, 0x11B4, u8 *) = m;
    AT(m, 0x11C0, u8) = 0;
    AT(m, 0x11BC, s32) = 0;
    AT(node, 0x40, u32) = 0x3F666666;   /* 0.9f */
    AT(node, 0x24, s32) = 0x19;
    AT(node, 0x20, u8) = 1;
    AT(node, 0x58, f32) = 1.0f;
    AT(node, 0x54, f32) = 1.0f;
    AT(node, 0x50, f32) = 1.0f;
}

/* the set +0x1100: 4 nodes (+0xE40) on bones 0x25..0x28 and 6 collision spheres (+0xF80,
 * chained by +0x2C from +0x1118) */
/* 0x002F7180 */
void EventHumanModel_Set1100(u8 *m) {
    static const s32 sColBones[6] = {0x1A, 0x2A, 0x21, 0x1B, 0x2B, 0x21};
    u8 *set = m + 0x1100;
    s32 i;

    SpringSet_Clear(set);
    for (i = 0; i < 4; i++) {
        spring_link(set, m + 0xE40 + i * 0x50);
    }
    for (i = 0; i < 6; i++) {
        u8 *col = m + 0xF80 + i * 0x40;

        AT(col, 0x2C, u8 *) = NULL;
        if (AT(m, 0x1118, u8 *) == NULL) {
            AT(m, 0x1118, u8 *) = col;
        } else {
            u8 *last = AT(m, 0x1118, u8 *);

            while (AT(last, 0x2C, u8 *) != NULL) {
                last = AT(last, 0x2C, u8 *);
            }
            AT(last, 0x2C, u8 *) = col;
        }
    }
    spring_set(set, 0x3DCCCCCD /* 0.1f */, 0x3F4CCCCD /* 0.8f */, m);
    for (i = 0; i < 4; i++) {
        u8 *node = m + 0xE40 + i * 0x50;

        AT(node, 0x40, u32) = 0x3EE66666;   /* 0.45f */
        AT(node, 0x24, s32) = 0x25 + i;
        AT(node, 0x20, u8) = (i == 0);
    }
    for (i = 0; i < 5; i++) {
        Sphere_Set(m + 0xF80 + i * 0x40, sColBones[i], 0.0f, 0.0f, 0.0f, 1.0f);
    }
    Sphere_Set(m + 0x10C0, sColBones[5], 0.0f, 1.0f, 0.0f, 1.0f);
}

/* the set +0xD70: 12 nodes (+0x9B0) in 6 pairs - bones, kinds and tables per pair, the first
 * of each pair leading, the phases 1 / 2 x 2 pi / 18 */
/* 0x002F73C0 */
void EventHumanModel_SetD70(u8 *m) {
    static u8 *const sTables[6] = {D_0041A4C0, D_0041A4C0, D_0041A4D0, D_0041A4D0, D_0041A4E0, D_0041A4F0};
    static const s32 sBones[6] = {0xB, 0x11, 0xD, 0x13, 0xF, 0x15};
    static const u32 sPhase[2] = {0x3EB2B8C3, 0x3F32B8C3};
    u8 *set = m + 0xD70;
    s32 i, j;

    SpringSet_Clear(set);
    for (i = 0; i < 12; i++) {
        spring_link(set, m + 0x9B0 + i * 0x50);
    }
    spring_set(set, 0x3E4CCCCD /* 0.2f */, 0x3F666666 /* 0.9f */, m);
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 2; j++) {
            u8 *node = m + 0x9B0 + (i * 2 + j) * 0x50;

            AT(node, 0x40, u32) = 0x3ED182AA;
            AT(node, 0x24, s32) = sBones[i] + j;
            AT(node, 0x44, s32) = (i & 1) ? 6 : 2;
            AT(node, 0x48, u8 *) = sTables[i];
            AT(node, 0x20, u8) = (j == 0);
            AT(node, 0x4C, u32) = sPhase[j];
        }
    }
}

/* her own setup: the five spring sets (the +0xE00 node on bone 0x29, the +0x1230 one on
 * 0x30), then a reset (+0x850) */
/* 0x002F7620 */
void EventHumanModel_Setup(u8 *m) {
    u8 *node;

    EventHumanModel_SetD70(m);
    SpringSet_Clear(m + 0xE00);
    node = m + 0xDB0;
    spring_link(m + 0xE00, node);
    spring_set(m + 0xE00, 0x3DCCCCCD /* 0.1f */, 0x3F7D70A4 /* 0.99f */, m);
    AT(node, 0x40, f32) = 1.0f;
    AT(node, 0x24, s32) = 0x29;
    AT(node, 0x20, u8) = 1;
    EventHumanModel_Set1100(m);
    EventHumanModel_Set11A0(m);
    SpringSet_Clear(m + 0x1230);
    node = m + 0x11E0;
    spring_link(m + 0x1230, node);
    spring_set(m + 0x1230, 0x3DCCCCCD /* 0.1f */, 0x3F7D70A4 /* 0.99f */, m);
    AT(node, 0x40, f32) = 1.0f;
    AT(node, 0x24, s32) = 0x30;
    AT(node, 0x20, u8) = 1;
    AT(m, 0x850, u8) = 1;
}

/* the five spring sets, a frame: one step, or after a reset (+0x850) the 4 hanging nodes
 * (+0xE40) put back under their anchors by their length (+0x40) along bone 0x21's Z axis, at
 * rest - 30 steps to settle (as Fiona's sheet model, FionaModel_Springs) */
/* 0x002F7780 */
void EventHumanModel_Springs(u8 *o) {
    static const u16 sSets[5] = {0xD70, 0xE00, 0x1100, 0x11A0, 0x1230};
    f32 down[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    s32 n, i, k;
    u8 *p;

    if (AT(o, 0x850, u8) == 0) {
        n = 1;
    } else {
        sceVu0CopyVector(down, Skel_Bone(AT(AT(o, 0x1114, u8 *), 0x810, void *), 0x21) + 8);
        p = o + 0xE40;
        for (i = 0; i < 4; i++) {
            AT(p, 0x18, f32) = 0.0f;
            AT(p, 0x14, f32) = 0.0f;
            AT(p, 0x10, f32) = 0.0f;
            if (AT(p, 0x20, u8) != 0) {
                sceVu0CopyVector(at, Skel_Bone(AT(AT(o, 0x1114, u8 *), 0x810, void *), AT(p, 0x24, s32)) + 12);
            } else {
                sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
            }
            sceVu0ScaleVector(d, down, -AT(p, 0x40, f32));
            sceVu0AddVector((f32 *)p, at, d);
            p += 0x50;
        }
        n = 0x1E;
    }
    for (k = 0; k < 5; k++) {
        SpringSet_Begin(o + sSets[k]);
    }
    for (i = 0; i < n; i++) {
        for (k = 0; k < 5; k++) {
            SpringSet_Step(o + sSets[k]);
        }
    }
    for (k = 0; k < 5; k++) {
        SpringSet_Finish(o + sSets[k]);
    }
    AT(o, 0x850, u8) = 0;
}

/* +0x10 */
/* 0x002F7910 */
void EventHumanModel_Frame(u8 *m) {
    CharModel_Frame(m);
}

/* +0xC loaded: the base setup, the parts' roles, her own setup, per-part draw settings */
/* 0x002F7920 */
void EventHumanModel_Loaded(u8 *m) {
    static const u8 sParts[] = {0x9C, 0xA8, 0xAA, 0xAC, 0xB2, 0xB4, 0xB6, 0xB8, 0xE4, 0xE6, 0xE8};
    s32 i;

    CharModel_Loaded(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x1E;
    AT(m, 0x8A0, s32) = 6;
    AT(m, 0x8A4, s32) = 7;
    AT(m, 0x8A8, s32) = 8;
    AT(m, 0x8AC, s32) = 9;
    AT(m, 0x8BC, s32) = 0x2E;
    AT(m, 0x8B0, s32) = 0x21;
    AT(m, 0x8B4, s32) = 0x17;
    EventHumanModel_Setup(m);
    AT(m, 0x9C, u8) = 4;
    AT(m, 0x9D, u8) = 0xC0;
    for (i = 1; i < 11; i++) {
        AT(m, sParts[i], u8) = 4;
        AT(m, sParts[i] + 1, u8) = 0x40;
    }
}

/* ---- Fiona's costume 6 (vtable Costume6Model_vtable, built by Costume6Model_ctor): three spring sets -
 * +0xA00 one node (+0x9B0), +0xD00 four hanging nodes (+0xA40) with six collision spheres
 * (+0xB80), +0xDA0 one node (+0xD40) ---- */

extern void *Costume6Model_vtable[], *CharModel_vtable[];

/* ---- Fiona's model (vtable FionaModel_vtable): her files (the game starts with her in a sheet,
 * O_FIS) ---- */

/* ---- Hewie's model (vtable DogModel_vtable): his files by costume 0..4 ---- */

extern void CharModel_Loaded(u8 *m);

/* character model setup common to Fiona and the partner: base setup, defaults, then the
 * class's own reset (vt+0xB8) and two slot inits (vt+0xC4, slots 0 and 5) */
/* 0x001F1FE0 */
void CharModel_Loaded(u8 *m) {
    HumanModel_Loaded(m);
    AT(m, 0x87C, s32) = 1;
    AT(m, 0x880, s32) = 1;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 14.0f;
    AT(m, 0x868, f32) = 0.0f;
    ((void (*)(u8 *))AT(AT(m, 0, u8 *), 0xB8, void *))(m);
    ((void (*)(u8 *, s32))AT(AT(m, 0, u8 *), 0xC4, void *))(m, 0);
    ((void (*)(u8 *, s32))AT(AT(m, 0, u8 *), 0xC4, void *))(m, 5);
}

/* base setup, then the class's post-setup hook (vt+0x54) */
/* 0x002118D0 */
void HumanModel_Loaded(u8 *m) {
    Model_Loaded(m);
    ((void (*)(u8 *))AT(AT(m, 0, u8 *), 0x54, void *))(m);
}

/* the stalker models' delete: nothing (their memory belongs to the character slot) */
/* 0x002DC6D0 */
void StalkerModel_delete(void *p) {
}

extern void ModelBase_Loaded(u8 *m);

/* 0x002DE0A0 */
void Model_Loaded(u8 *m) {
    ModelBase_Loaded(m);
    AT(m, 0x87C, s32) = 0;
    AT(m, 0x880, s32) = 0;
    AT(m, 0x854, s32) = 0;
    AT(m, 0x858, s32) = 0;
    AT(m, 0x870, s32) = 0;
    ((void (*)(u8 *))AT(AT(m, 0, u8 *), 0xB4, void *))(m);
}

extern void *gSkelPool;

/* build the model's skeleton from its bone table (+0x4C0: count, then 0x70-byte bones whose
 * +0x10 is the parent index, -1 for the root) */
/* 0x001F7C40 */
void ModelBase_Loaded(u8 *m) {
    u8 *skel;
    u8 *node;
    u8 *bone;
    s32 i;

    AT(m, 0x4C8, s32) = 0;
    skel = SkelPool_Alloc(gSkelPool, AT(AT(m, 0x4C0, u8 *), 0, s32));
    node = AT(skel, 4, u8 *);
    bone = AT(m, 0x4C0, u8 *) + 0x10;
    for (i = 0; i < AT(skel, 8, s32); i++) {
        AT(node, 0x40, s32) = i;
        if (AT(bone, 0, s32) >= 0)
            SkelNode_SetParent(node, Skel_Bone(skel, AT(bone, 0, s32)));
        node = AT(node, 0x48, u8 *);
        bone += 0x70;
    }
    AT(m, 0x810, u8 *) = skel;
    AT(m, 0x18, u8 *) = AT(m, 0x810, u8 *);
    AT(m, 0x1C, u8 *) = AT(m, 0x4C0, u8 *);
    AT(m, 0x20, u8 *) = AT(m, 0x4D0, u8 *);
    AT(m, 0x1DC, u8 *) = AT(m, 0x810, u8 *);
    AT(m, 0x1D8, u8 *) = AT(m, 0x4CC, u8 *);
    AT(m, 0x4D8, u8) = 0;
    Model_ResetMotion(m);
}

extern void *gChainPool;

/* reset the model's motion state: the two motion slots (+0x564, 0xA0 each; current +0x540,
 * next +0x544) and the three blend channels (+0x6B0, 0x60 each), freeing their skeletons and
 * buffers */
/* 0x001F4910 */
void Model_ResetMotion(u8 *m) {
    void *bufs = gChainPool;
    void *skels = gSkelPool;
    u8 *slot;
    u8 *ch;
    s32 i, j, k;

    AT(m, 0x540, s32) = 0;
    AT(m, 0x544, s32) = AT(m, 0x540, s32) ^ 1;
    for (i = 0; i < 2; i++) {
        slot = m + 0x564 + i * 0xA0;
        for (j = 0; j < 2; j++) {
            u8 *s = slot + j * 4;

            SkelPool_Free(skels, AT(s, 0x28, u8 *));
            AT(s, 0x28, s32) = 0;
            ChainPool_Free(bufs, AT(s, 0x20, void *));
            ChainPool_Free(bufs, AT(s, 0x30, void *));
            AT(s, 0x20, s32) = 0;
            AT(s, 0x30, s32) = 0;
            AT(slot, 0x1C, s32) = 0;
            AT(m, 0x55C + j * 4, s32) = -1;
            AT(m, 0x554 + j * 4, s32) = -1;
        }
        for (k = 0; k < 24; k++) {
            AT(slot, 0x38 + k * 4, s32) = 0;
        }
    }
    AT(m, 0x548, s32) = 0;
    AT(m, 0x54C, s32) = 0;
    AT(m, 0x550, s32) = 0;
    for (i = 0; i < 3; i++) {
        ch = m + 0x6B0 + i * 0x60;
        AT(ch, 0x0, s32) = 0;
        AT(ch, 0x4, s32) = AT(ch, 0x0, s32) ^ 1;
        AT(ch, 0x54, u8 *) = ch + AT(ch, 0x0, s32) * 0x1C + 0x1C;
        AT(ch, 0x58, u8 *) = ch + AT(ch, 0x4, s32) * 0x1C + 0x1C;
        AT(ch, 0x18, s32) = -1;
        AT(ch, 0x14, s32) = -1;
        AT(ch, 0x8, s32) = 0;
        AT(ch, 0xC, s32) = 0;
        AT(ch, 0x10, s32) = 0;
        for (j = 0; j < 2; j++) {
            u8 *s = ch + j * 0x1C;

            SkelPool_Free(skels, AT(s, 0x30, u8 *));
            AT(s, 0x30, s32) = 0;
            ChainPool_Free(bufs, AT(s, 0x2C, void *));
            AT(s, 0x2C, s32) = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        AT(m, 0x4DC + i * 4, s32) = -1;
        AT(m, 0x4F0 + i * 0x14, u8) = 0;
    }
    AT(m, 0x4EC, s32) = -1;
    AT(m, 0x6A4, u8 *) = m + 0x564 + AT(m, 0x540, s32) * 0xA0;
    AT(m, 0x6A8, u8 *) = m + 0x564 + AT(m, 0x544, s32) * 0xA0;
    AT(AT(m, 0x6A4, u8 *), 0x18, s32) = 16;
    AT(AT(m, 0x6A8, u8 *), 0x18, s32) = 16;
}

extern u8 D_003D5CC0[];

/* vt+0xB8: the character's table at +0x874 */
/* 0x001F1D90 */
void CharModel_SecondaryMotion(u8 *m) {
    AT(m, 0x874, u8 *) = D_003D5CC0;
}

/* vt+0xC4 (the slot is ignored here) */
/* 0x00211160 */
void HumanModel_FeetUp(u8 *m, s32 slot) {
    AT(m, 0x990, u8) = 1;
    AT(m, 0x8C4, f32) = 1.0f;
}

/* +0x58: the scale +0x8C0 */
/* 0x00211180 */
f32 HumanModel_Vt58(u8 *m) {
    return AT(m, 0x8C0, f32);
}

/* empty character hook (vt+0xC4 of the base) */
/* 0x001F1F10 */
void CharModel_ShowParts(u8 *m, s32 slot) {
}

/* ---- the human characters' model base (vtable CharModel_vtable): its own methods ---- */

extern const char D_00456150[], str_O_FIN_FIN_001_MRK[], str_O_FIN_FIN_002_MRK[], str_O_FIN_FIN_003_MRK[], str_O_FIN_FIN_004_MRK[],
    str_O_FIN_FIN_005_MRK[], str_O_FIN_FIN_006_MRK[];   /* "O_FIN\FIN_00n.MRK" */

/* +0x10 */
/* 0x001F1FD0 */
void CharModel_Frame(u8 *m) {
    HumanModel_Frame(m);
}

/* +0x68: Model_ClearDraw, then +0x2C */
/* 0x001F1DA0 */
void CharModel_ClearDraw(u8 *m) {
    Model_ClearDraw(m);
    VCALL(m, 0x2C, void (*)(u8 *))(m);
}

/* +0xA4 the marker file of kind `n` (0..6) */
/* 0x001F1F30 */
const char *CharModel_MarkerFile(void *m, s32 n) {
    switch (n) {
    case 0: return D_00456150;
    case 1: return str_O_FIN_FIN_001_MRK;
    case 2: return str_O_FIN_FIN_002_MRK;
    case 3: return str_O_FIN_FIN_003_MRK;
    case 4: return str_O_FIN_FIN_004_MRK;
    case 5: return str_O_FIN_FIN_005_MRK;
    case 6: return str_O_FIN_FIN_006_MRK;
    }
    return NULL;
}

/* empty hooks: +0xBC, +0xC0, +0xC8, +0xCC, +0xD0 */
/* 0x001F1FC0 */
void CharModel_SetFlag(u8 *m) {
}

/* 0x001F1F20 */
void CharModel_SetPoint(u8 *m) {
}

/* 0x001F1F00 */
void CharModel_SetVector(u8 *m) {
}

/* 0x001F1EF0 */
void CharModel_VtCC(u8 *m) {
}

/* 0x001F1EE0 */
void CharModel_VtD0(u8 *m) {
}

/* clear a secondary-motion set (its node list +0x30/+0x34) */
/* 0x002EE960 */
void SpringSet_Clear(u8 *set) {
    AT(set, 0x34, s32) = 0;
    AT(set, 0x30, s32) = 0;
    AT(set, 0x18, s32) = 0;
}

/* set a collision sphere: centre (bone-local, w 1), radius and its inverse, bone */
/* 0x002EE690 */
void Sphere_Set(u8 *col, s32 bone, f32 x, f32 y, f32 z, f32 r) {
    AT(col, 0x10, f32) = x;
    AT(col, 0x14, f32) = y;
    AT(col, 0x18, f32) = z;
    AT(col, 0x1C, f32) = 1.0f;
    AT(col, 0x20, f32) = r;
    AT(col, 0x24, f32) = 1.0f / r;
    AT(col, 0x28, s32) = bone;
}

/* 0x002EE6C0 */
void SpringPart_NoopC(u8 *e) {
}

/* the parts' base (vtable SpringPartBase_vtable; 0x40 / 0x50 / 0x70 parts, the vtable at +0x30): +0x8 reset,
   +0xC / +0x10 nothing */
/* 0x002EE6D0 */
void SpringPart_Reset(u8 *e) {
    AT(e, 0x8, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x0, s32) = 0;
    AT(e, 0x18, s32) = 0;
    AT(e, 0x14, s32) = 0;
    AT(e, 0x10, s32) = 0;
    AT(e, 0x24, s32) = -1;
    AT(e, 0x20, u8) = 1;
    AT(e, 0x2C, s32) = 0;
    AT(e, 0x28, s32) = 0;
}

extern void Motion_Start(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend);

/* the motion back to the default speeds (+0x87C / +0x880) from the start, its flags +0x85C /
   +0x85D cleared */
static inline void motion_defaults(u8 *m) {
    AT(m, 0x85C, u8) = 0;
    AT(m, 0x85D, u8) = 0;
    AT(m, 0x38, s32) = AT(m, 0x87C, s32);
    AT(m, 0x3C, s32) = AT(m, 0x87C, s32);
    AT(m, 0x40, s32) = 0;
    AT(m, 0x48, s32) = AT(m, 0x880, s32);
    AT(m, 0x4C, s32) = AT(m, 0x880, s32);
    AT(m, 0x50, s32) = 0;
}

/* play animation `anim` with these flags, variant and blend at the default speeds */
/* 0x002DE030 */
void Motion_PlayWith(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend) {
    motion_defaults(m);
    Motion_Start(m, anim, flags, variant, blend);
}

/* +0x10 of the base: Model_Release, no secondary-motion table */
/* 0x002DE070 */
void Model_Frame(u8 *m) {
    Model_Release(m);
    AT(m, 0x874, void *) = NULL;
}

/* play animation `anim` (its flags from the table +0x874, 6 bytes per entry) from the start,
 * at the default speeds +0x87C / +0x880 */
/* 0x002DDE20 */
void Motion_Play(u8 *m, s32 anim, s32 variant) {
    s32 i = Motion_AnimIndex(m, anim);
    u32 flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;

    motion_defaults(m);
    Motion_Start(m, anim, flags & 0xFFFF, variant, 0.0f);
}

/* Motion_Play blended in over `blend` frames */
/* 0x002DDC60 */
void Motion_PlayBlend(u8 *m, s32 anim, s32 blend, s32 variant) {
    s32 i = Motion_AnimIndex(m, anim);
    u32 flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;

    motion_defaults(m);
    Motion_Start(m, anim, flags & 0xFFFF, variant, (f32)blend);
}

/* Motion_PlayBlend with flag 8 added to the animation's, the default variant */
/* 0x002DDBA0 */
void Motion_PlayBlend8(u8 *m, s32 anim, s32 blend) {
    s32 i = Motion_AnimIndex(m, anim);
    u32 flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;

    motion_defaults(m);
    Motion_Start(m, anim, (flags | 8) & 0xFFFF, -1, (f32)blend);
}

/* the same blended over the animation's own frames (the table's first s16) */
/* 0x002DDD20 */
void Motion_PlayOwnBlend(u8 *m, s32 anim, s32 variant) {
    s32 i = Motion_AnimIndex(m, anim);
    f32 blend = (f32)(i != -1 ? AT(AT(m, 0x874, u8 *), i * 6, s16) : 0);
    u32 flags;

    i = Motion_AnimIndex(m, anim);
    flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;
    motion_defaults(m);
    Motion_Start(m, anim, (flags | 8) & 0xFFFF, variant, blend);
}

/* the model's +0x6A4 object gets flag 0x40 */
/* 0x001F6E30 */
void Motion_Freeze(u8 *m) {
    AT(AT(m, 0x6A4, u8 *), 0x18, u32) |= 0x40;
}

/* the index of animation `anim` in the motion file (+0x4C4: at its +0xC a table: count, then
 * 8-byte entries from +0x10 starting with the id); -1: not there */
/* 0x001F4710 */
s32 Motion_AnimIndex(u8 *m, s32 anim) {
    u8 *mtn = AT(m, 0x4C4, u8 *);
    u8 *tbl;
    u32 i;

    if (mtn == NULL) {
        return -1;
    }
    tbl = mtn + AT(mtn, 0xC, u32);
    for (i = 0; i < AT(tbl, 0, u32); i++) {
        if (AT(tbl, 0x10 + i * 8, s32) == anim) {
            return i;
        }
    }
    return -1;
}

/* the entry (0x14 bytes from +0x10: +0x4 the body's motion, +0x8.. the 3 parts') of animation
 * `anim` in motion file `mtn` (the last match in its id table); NULL: not there */
/* 0x001F4B80 */
u8 *Motion_AnimEntry(u8 *m, u8 *mtn, s32 anim) {
    u8 *tbl;
    s32 j = -1;
    u32 i;

    if (mtn == NULL) {
        return NULL;
    }
    tbl = mtn + AT(mtn, 0xC, u32);
    for (i = 0; i < AT(tbl, 0, u32); i++) {
        if (AT(tbl, 0x10 + i * 8, s32) == anim) {
            j = AT(tbl, 0x14 + i * 8, s32);
        }
    }
    if (j == -1) {
        return NULL;
    }
    return mtn + j * 0x14 + 0x10;
}

extern void Motion_StartBody(u8 *m, s32 anim, s32 part, u32 flags, s32 variant, f32 blend);
extern void Motion_StartPart(u8 *m, s32 anim, s32 part, u32 flags, u8 *channel, f32 blend);
extern void Motion_PlayWhenFree(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend);

/* animation `anim`'s entry, from the main motion file (+0x4C4) or else the second (+0x4C8) */
static u8 *motion_entry(u8 *m, s32 anim) {
    u8 *e = Motion_AnimEntry(m, AT(m, 0x4C4, u8 *), anim);

    if (e == NULL) {
        e = Motion_AnimEntry(m, AT(m, 0x4C8, u8 *), anim);
    }
    return e;
}

/* start animation `anim`: the body (if the animation has it), then each of the 3 parts that
 * the animation covers (blend channels +0x6B0); a part it doesn't cover keeps / resumes its
 * own animation (+0x4E4) */
/* 0x001F7890 */
void Motion_Start(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend) {
    s32 k;

#ifdef HG_NATIVE
    if (motion_entry(m, anim) == NULL) {   /* not loaded (event motions aren't on PC yet) */
        return;
    }
#endif
    AT(m, 0x4E0, s32) = AT(m, 0x4DC, s32);
    AT(m, 0x4DC, s32) = anim;
    if (AT(motion_entry(m, anim), 0x4, s32) != 0) {
        Motion_StartBody(m, anim, 1, flags, variant, blend);
    } else {
        for (k = 0; k < 3; k++) {
            if (AT(motion_entry(m, anim), 0x8 + k * 4, s32) != 0) {
                AT(m, 0x4E4 + k * 4, s32) = anim;
            }
        }
    }
    for (k = 0; k < 3; k++) {
        if (AT(motion_entry(m, anim), 0x8 + k * 4, s32) != 0) {
            Motion_StartPart(m, anim, k + 2, flags, m + k * 0x60 + 0x6B0, blend);
            if (AT(m, 0x504 + k * 0x14, u8)) {
                AT(m, 0x504 + k * 0x14, u8) = 0;
                AT(m, 0x4E4 + k * 4, s32) = AT(m, 0x508 + k * 0x14, s32);
            }
        } else {
            s32 own = AT(m, 0x4E4 + k * 4, s32);

            if (own != -1 && own != AT(m, 0x6C8 + k * 0x60, s32)) {
                Motion_PlayWhenFree(m, own, flags, -1, blend);
            }
        }
    }
}

/* release the model's skeleton (+0x810) and the skeletons and buffers of its two motion slots
   (+0x564) and three blend channels (+0x6B0), and clear the slots' key lists */
/* 0x001F7AC0 */
void Model_Release(u8 *m) {
    void *skels = gSkelPool;
    void *bufs;
    s32 i, j, k;

    SkelPool_Free(skels, AT(m, 0x810, u8 *));
    AT(m, 0x810, s32) = 0;
    bufs = gChainPool;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            u8 *s = m + i * 0xA0 + j * 4;

            SkelPool_Free(skels, AT(s, 0x58C, u8 *));
            ChainPool_Free(bufs, AT(s, 0x584, void *));
            ChainPool_Free(bufs, AT(s, 0x594, void *));
            AT(s, 0x58C, s32) = 0;
            AT(s, 0x584, s32) = 0;
            AT(s, 0x594, s32) = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            u8 *s = m + i * 0x60 + j * 0x1C;

            ChainPool_Free(bufs, AT(s, 0x6DC, void *));
            AT(s, 0x6DC, s32) = 0;
            SkelPool_Free(skels, AT(s, 0x6E0, u8 *));
            AT(s, 0x6E0, s32) = 0;
        }
    }
    for (i = 0; i < 2; i++) {
        for (k = 0; k < 24; k++) {
            AT(m, 0x59C + i * 0xA0 + k * 4, s32) = 0;
        }
    }
}

extern void Motion_SetupTrack(u8 *m, u8 **motion, u8 **skel, s32 anim, s32 part);

/* start animation `anim` on part channel `ch` (0x60 bytes at +0x6B0: its two slots of 0x1C at
 * +0x1C, current +0x54 / previous +0x58, the animation +0x18), the old one blending out over
 * `blend` frames; the slot's old motion and skeleton are freed first */
/* 0x001F6E50 */
void Motion_StartPart(u8 *m, s32 anim, s32 part, u32 flags, u8 *ch, f32 blend) {
    u8 *cur;

    AT(ch, 0x8, f32) = blend;
    AT(ch, 0xC, f32) = blend;
    AT(ch, 0x10, f32) = 1.0f;
    AT(ch, 0x4, s32) = AT(ch, 0x0, s32);
    AT(ch, 0x0, s32) ^= 1;
    AT(ch, 0x54, u8 *) = ch + 0x1C + AT(ch, 0x0, s32) * 0x1C;
    AT(ch, 0x58, u8 *) = ch + 0x1C + AT(ch, 0x4, s32) * 0x1C;
    AT(ch, 0x14, s32) = AT(ch, 0x18, s32);
    AT(ch, 0x18, s32) = anim;
    AT(AT(ch, 0x54, u8 *), 0xC, u32) = flags & 0xFFFF;
    if (AT(AT(ch, 0x54, u8 *), 0xC, u32) & 8) {
        AT(AT(ch, 0x54, u8 *), 0xC, u32) |= 0x10;
        if (AT(ch, 0x58, u8 *) != NULL) {
            AT(AT(ch, 0x58, u8 *), 0xC, u32) |= 0x10;
        }
    }
    cur = AT(ch, 0x54, u8 *);
    if (AT(cur, 0x10, void *) != NULL) {
        ChainPool_Free(gChainPool, AT(cur, 0x10, void *));
        AT(AT(ch, 0x54, u8 *), 0x10, void *) = NULL;
    }
    cur = AT(ch, 0x54, u8 *);
    if (AT(cur, 0x14, u8 *) != NULL) {
        SkelPool_Free(gSkelPool, AT(cur, 0x14, u8 *));
        AT(AT(ch, 0x54, u8 *), 0x14, u8 *) = NULL;
    }
    cur = AT(ch, 0x54, u8 *);
    Motion_SetupTrack(m, (u8 **)(cur + 0x10), (u8 **)(cur + 0x14), anim, part);
    AT(AT(ch, 0x54, u8 *), 0x0, f32) = 0.0f;
    AT(AT(ch, 0x54, u8 *), 0x8, f32) = 1.0f;
    AT(AT(ch, 0x54, u8 *), 0x18, s32) = 0;
}

/* what an animation's entry covers: bit 0 its +0, bit 1 the body (+4), bits 2..4 the 3 parts */
static inline u32 motion_covers(const u8 *e) {
    u32 c = AT(e, 0x0, s32) != 0;

    c |= AT(e, 0x4, s32) != 0 ? 2 : 0;
    c |= AT(e, 0x8, s32) != 0 ? 4 : 0;
    c |= AT(e, 0xC, s32) != 0 ? 8 : 0;
    c |= AT(e, 0x10, s32) != 0 ? 0x10 : 0;
    return c;
}

/* the parts' animations queued while their channel was blending (+0x504.., 0x14 each) are
 * dropped: each part just keeps the queued one as its own (+0x4E4) */
static inline void motion_drop_queued_parts(u8 *m) {
    s32 k;

    for (k = 0; k < 3; k++) {
        if (AT(m, 0x504 + k * 0x14, u8)) {
            AT(m, 0x504 + k * 0x14, u8) = 0;
            AT(m, 0x4E4 + k * 4, s32) = AT(m, 0x508 + k * 0x14, s32);
        }
    }
}

/* play animation `anim` when the model is free for it: one covering the whole body replaces the
 * body's (unless already playing), or waits (+0x4F0..) while the body still blends; one covering
 * only parts goes to each such part, waiting (+0x504..) while that part's channel blends - a part
 * under a whole-body animation (playing or queued) just remembers it (+0x4E4) */
/* 0x001F7460 */
void Motion_PlayWhenFree(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend) {
    u32 c = motion_covers(motion_entry(m, anim));
    s32 k;

    if (c == 0x1F || (c & 2) == 2) {
        if (AT(m, 0x55C, s32) == anim && AT(m, 0x560, s32) == variant) {
            return;
        }
        if (AT(m, 0x54C, f32) != 0.0f) {
            AT(m, 0x4F0, u8) = 1;
            AT(m, 0x4F4, s32) = anim;
            AT(m, 0x4F8, s16) = flags;
            AT(m, 0x4FC, f32) = blend;
            AT(m, 0x500, s32) = variant;
        } else {
            Motion_Start(m, anim, flags, variant, blend);
        }
        if (c == 0x1F) {
            motion_drop_queued_parts(m);
        }
        return;
    }
    for (k = 0; k < 3; k++) {
        if (AT(motion_entry(m, anim), 0x8 + k * 4, s32) == 0) {
            continue;
        }
        if (motion_covers(motion_entry(m, AT(m, 0x55C, s32))) == 0x1F
            || (AT(m, 0x4F0, u8) && motion_covers(motion_entry(m, AT(m, 0x4F4, s32))) == 0x1F)) {
            AT(m, 0x4E4 + k * 4, s32) = anim;
            continue;
        }
        if (AT(m, 0x6C8 + k * 0x60, s32) == anim) {
            continue;
        }
        if (AT(m, 0x6BC + k * 0x60, f32) == 0.0f) {
            Motion_Start(m, anim, flags, variant, blend);
            continue;
        }
        AT(m, 0x504 + k * 0x14, u8) = 1;
        AT(m, 0x508 + k * 0x14, s32) = anim;
        AT(m, 0x50C + k * 0x14, s16) = flags;
        AT(m, 0x510 + k * 0x14, f32) = blend;
        AT(m, 0x514 + k * 0x14, s32) = variant;
        AT(m, 0x4E4 + k * 4, s32) = anim;
    }
}

extern void Motion_FreeSlot(u8 *m, s32 slot);
extern void Motion_EventKeys(u8 *m, s32 anim, s32 variant);

#define MOTION_SLOT(m, i) ((m) + 0x564 + (i) * 0xA0)

/* the frame count of a slot's motion (its +0x4 header, +0xC) */
static s32 motion_frames(u8 *motion) {
    return AT(AT(motion, 0x4, u8 *), 0xC, s32);
}

/* start body animation `anim` (with `variant`, -1: none) in the other motion slot, cross-fading
 * from the current one over `blend`; synced animations (flag 2 in both) keep the phase */
/* 0x001F71E0 */
void Motion_StartBody(u8 *m, s32 anim, s32 part, u32 flags, s32 variant, f32 blend) {
    u8 *cur;
    u8 *prev;

    AT(m, 0x548, f32) = blend;
    AT(m, 0x54C, f32) = blend;
    AT(m, 0x550, f32) = 1.0f;
    AT(m, 0x544, s32) = AT(m, 0x540, s32);
    AT(m, 0x540, s32) ^= 1;
    AT(m, 0x6A4, u8 *) = MOTION_SLOT(m, AT(m, 0x540, s32));
    AT(m, 0x6A8, u8 *) = MOTION_SLOT(m, AT(m, 0x544, s32));
    AT(m, 0x554, s32) = AT(m, 0x55C, s32);
    AT(m, 0x55C, s32) = anim;
    AT(m, 0x558, s32) = AT(m, 0x560, s32);
    AT(m, 0x560, s32) = variant;
    cur = AT(m, 0x6A4, u8 *);
    AT(cur, 0x18, u32) = flags & 0xFFFF;
    if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 8) {
        AT(AT(m, 0x6A4, u8 *), 0x18, u32) |= 0x10;
        if (AT(m, 0x6A8, u8 *) != NULL) {
            AT(AT(m, 0x6A8, u8 *), 0x18, u32) |= 0x10;
        }
    }
    Motion_FreeSlot(m, AT(m, 0x540, s32));
    cur = AT(m, 0x6A4, u8 *);
    Motion_SetupTrack(m, (u8 **)(cur + 0x20), (u8 **)(cur + 0x28), anim, part);
    cur = AT(m, 0x6A4, u8 *);
    prev = AT(m, 0x6A8, u8 *);
    if (AT(cur, 0x18, u32) & AT(prev, 0x18, u32) & 2) {
        u8 *old = MOTION_SLOT(m, AT(m, 0x544, s32));

        AT(cur, 0x0, f32) = (f32)motion_frames(AT(cur, 0x20, u8 *)) *
                            (AT(old, 0x0, f32) / (f32)motion_frames(AT(old, 0x20, u8 *)));
    } else {
        AT(cur, 0x0, f32) = 0.0f;
    }
    AT(AT(m, 0x6A4, u8 *), 0x8, f32) = -1.0f;
    AT(AT(m, 0x6A4, u8 *), 0x10, f32) = 1.0f;
    AT(AT(m, 0x6A4, u8 *), 0x98, s32) = 0;
    if (variant != -1) {
        cur = AT(m, 0x6A4, u8 *);
        Motion_SetupTrack(m, (u8 **)(cur + 0x24), (u8 **)(cur + 0x2C), variant, part);
        cur = AT(m, 0x6A4, u8 *);
        prev = AT(m, 0x6A8, u8 *);
        if (AT(cur, 0x18, u32) & AT(prev, 0x18, u32) & 2) {
            u8 *old = MOTION_SLOT(m, AT(m, 0x544, s32));

            AT(cur, 0x4, f32) = (f32)motion_frames(AT(cur, 0x24, u8 *)) *
                                (AT(old, 0x0, f32) / (f32)motion_frames(AT(old, 0x20, u8 *)));
        } else {
            AT(cur, 0x4, f32) = 0.0f;
        }
        AT(AT(m, 0x6A4, u8 *), 0x8, f32) = -1.0f;
        AT(AT(m, 0x6A4, u8 *), 0x14, f32) = 1.0f;
        AT(AT(m, 0x6A4, u8 *), 0x9C, s32) = 0;
    }
    Motion_EventKeys(m, anim, variant);
}

/* free motion slot `i`'s two tracks (skeleton +0x28, data +0x20 / +0x30) and clear their
 * 12 keys (+0x38, 8 apart) */
/* 0x001F5020 */
void Motion_FreeSlot(u8 *m, s32 i) {
    void *skels = gSkelPool;
    void *bufs = gChainPool;
    u8 *slot = MOTION_SLOT(m, i);
    s32 j, k;

    for (j = 0; j < 2; j++) {
        u8 *t = slot + j * 4;

        if (AT(t, 0x28, u8 *) != NULL) {
            SkelPool_Free(skels, AT(t, 0x28, u8 *));
            AT(t, 0x28, s32) = 0;
        }
        if (AT(t, 0x20, void *) != NULL) {
            ChainPool_Free(bufs, AT(t, 0x20, void *));
            AT(t, 0x20, s32) = 0;
        }
        if (AT(t, 0x30, void *) != NULL) {
            ChainPool_Free(bufs, AT(t, 0x30, void *));
            AT(t, 0x30, s32) = 0;
        }
        for (k = 0; k < 12; k++) {
            if (AT(t, 0x38 + k * 8, s32) != 0) {
                AT(t, 0x38 + k * 8, s32) = 0;
            }
        }
    }
}

/* set up a motion track of animation `anim`'s part `part` (1 the body, 2.. the 3 parts): a
 * chain of per-bone keys (*keys) and a skeleton (*skel), each bone with its index in the
 * model's bone table */
/* 0x001F4C10 */
void Motion_SetupTrack(u8 *m, u8 **keys, u8 **skel, s32 anim, s32 part) {
    u8 *e = motion_entry(m, anim);
    u8 *mot = e + AT(e, part * 4, u32);
    u8 *tracks;
    u8 *key;
    u8 *node;
    s32 i;

    *keys = ChainPool_Alloc(gChainPool, AT(mot, 0, s32));
    *skel = SkelPool_Alloc(gSkelPool, AT(mot, 0, s32));
    node = AT(*skel, 0x4, u8 *);
    key = AT(*keys, 0x4, u8 *);
    tracks = mot + AT(mot, 0x8, u32);
    for (i = 0; i < AT(*keys, 0x8, s32); i++) {
        u8 *bones;
        s32 bone;

        Triple_Set(key + 4, AT(tracks, 0x4, s32), (s32)(tracks + AT(tracks, 0x8, u32)), AT(mot, 0x4, s32));
        bones = AT(m, 0x4C0, u8 *);
        bone = AT(bones, 0xC, u32) != 0 ? AT(bones + AT(bones, 0xC, u32) + AT(tracks, 0, u32), 1, s8) : 0;
        AT(key, 0x0, s32) = bone;
        AT(node, 0x40, s32) = bone;
        key = AT(key, 0x10, u8 *);
        node = AT(node, 0x48, u8 *);
        tracks += 0xC;
    }
}

/* the event keys of the current motion slot's animation and variant (-1: none): a chain per
 * track (+0x30 / +0x34) of the animation's event part, and the 12 event tracks (+0x38 + k * 8:
 * the key of track -1 - k, if any) */
/* 0x001F6FD0 */
void Motion_EventKeys(u8 *m, s32 anim, s32 variant) {
    s32 ids[2];
    void *bufs = gChainPool;
    s32 t, k, i;

    ids[0] = anim;
    ids[1] = variant;
    for (t = 0; t < 2; t++) {
        u8 **chain;
        u8 *e;
        u8 *evp;
        u8 *key;
        u8 *tracks;

        if (ids[t] == -1) {
            continue;
        }
        chain = (u8 **)(AT(m, 0x6A4, u8 *) + t * 4 + 0x30);
        e = motion_entry(m, ids[t]);
        evp = e + AT(e, 0, u32);
        *chain = ChainPool_Alloc(bufs, AT(evp, 0, s32));
        key = AT(*chain, 0x4, u8 *);
        tracks = evp + AT(evp, 0x8, u32);
        for (i = 0; i < AT(*chain, 0x8, s32); i++) {
            Triple_Set(key + 4, AT(tracks, 0x4, s32), (s32)(tracks + AT(tracks, 0x8, u32)), AT(evp, 0x4, s32));
            AT(key, 0x0, s32) = AT(tracks, 0, s32);
            key = AT(key, 0x10, u8 *);
            tracks += 0xC;
        }
        {
            u8 *c = AT(AT(m, 0x6A4, u8 *) + t * 4, 0x30, u8 *);

            for (k = 0; k < 12; k++) {
                u8 *x;

                AT(AT(m, 0x6A4, u8 *) + t * 4 + k * 8, 0x38, u8 *) = NULL;
                x = AT(c, 0x4, u8 *);
                for (i = 0; i < AT(c, 0x8, s32); i++) {
                    if (AT(x, 0, s32) == -1 - k) {
                        AT(AT(m, 0x6A4, u8 *) + t * 4 + k * 8, 0x38, u8 *) = x + 4;
                    }
                    x = AT(x, 0x10, u8 *);
                }
            }
        }
    }
}

/* the position of the model's reference bone (+0x8B0, in the skeleton +0x810): its matrix's
 * translation row */
/* 0x001F1DE0 */
void HumanModel_HeadPos(u8 *m, f32 *out) {
    u8 *mtx = (u8 *)Skel_Bone(AT(m, 0x810, void *), AT(m, 0x8B0, s32));

    sceVu0CopyVector(out, (f32 *)(mtx + 0x30));
}

/* 0x001F1E20 */
void HumanModel_Vt3C(u8 *m) {
}

/* +0x74 / +0x78 / +0x7C / +0x80: the part roles set up at load (+0x8AC, +0x8BC, +0x8B8, +0x8B0) */
/* 0x001F1E30 */
s32 HumanModel_Vt74(u8 *m) {
    return AT(m, 0x8AC, s32);
}

/* 0x001F1E40 */
s32 HumanModel_Vt78(u8 *m) {
    return AT(m, 0x8BC, s32);
}

/* 0x001F1E50 */
s32 HumanModel_Vt7C(u8 *m) {
    return AT(m, 0x8B8, s32);
}

/* 0x001F1E60 */
s32 HumanModel_HeadBone(u8 *m) {
    return AT(m, 0x8B0, s32);
}

/* +0x34: +0x878 = `v`, then +0x30 */
/* 0x001F1E70 */
void Model_Set878(u8 *m, s32 v) {
    AT(m, 0x878, s32) = v;
    VCALL(m, 0x30, void (*)(u8 *, s32))(m, v);
}

/* 0x001F1E90 */
s32 Model_Vt70(u8 *m) {
    return 0;
}

/* 0x001F1EA0 */
s32 Model_Vt94(u8 *m) {
    return 0;
}

/* 0x001F1EB0 */
s32 Model_Vt98(u8 *m) {
    return 0;
}

/* 0x001F1EC0 */
s32 Model_Vt9C(u8 *m) {
    return 0;
}

/* motion: clear flag 0x40 of the current track (+0x6A4, flags +0x18) */
/* 0x001F6E10 */
void Motion_Unfreeze(u8 *m) {
    AT(AT(m, 0x6A4, u8 *), 0x18, u32) &= ~0x40;
}

/* ---- stick gestures: each recognizer arms once the stick is centred (under 0.4) and fires
   when it is then pushed fully (over 0.99) in its direction (the angle from msl_atan2f:
   0 forward, +-pi back) ---- */

static s32 gesture_step(s32 *state, f32 len, s32 aimed, s32 gesture) {
    if (*state == 0) {
        if (len < 0x1.99999ap-2f /* 0.4 */) {
            *state += 1;
        }
        return -1;
    }
    if (*state != 1 || len <= 0x1.fae148p-1f /* 0.99 */ || !aimed) {
        return -1;
    }
    *state = 0;
    return gesture;
}

/* 0: back (more than pi - 0.6 off forward) */
/* 0x001F1AD0 */
s32 Gesture_Back(u8 *g, f32 len, f32 ang) {
    if (ang <= 0.0f) {
        ang = -ang;
    }
    return gesture_step(&AT(g, 0x28, s32), len, !(ang <= 0x1.4552e8p+1f /* pi - 0.6 */), 0);
}

/* 1: forward (within 0.6) */
/* 0x001F1A10 */
s32 Gesture_Forward(u8 *g, f32 len, f32 ang) {
    if (ang <= 0.0f) {
        ang = -ang;
    }
    return gesture_step(&AT(g, 0x2C, s32), len, ang < 0x1.333334p-1f /* 0.6 */, 1);
}

/* 2: R3 pressed with the stick centred */
/* 0x001F19C0 */
s32 Gesture_R3(u8 *g, f32 len, f32 ang) {
    return len < 0x1.99999ap-2f /* 0.4 */ && (gPadPressed & PAD_R3) ? 2 : -1;
}

/* 3: right (within 0.6 of +pi/2) */
/* 0x001F1900 */
s32 Gesture_Right(u8 *g, f32 len, f32 ang) {
    return gesture_step(&AT(g, 0x34, s32), len,
                        !(ang <= 0x1.f10c38p-1f /* pi/2 - 0.6 */) && ang < 0x1.15dca8p+1f /* pi/2 + 0.6 */, 3);
}

/* 4: left (within 0.6 of -pi/2) */
/* 0x001F1840 */
s32 Gesture_Left(u8 *g, f32 len, f32 ang) {
    return gesture_step(&AT(g, 0x38, s32), len,
                        !(ang <= -0x1.15dca8p+1f /* -pi/2 - 0.6 */) && ang < -0x1.f10c38p-1f /* -pi/2 + 0.6 */, 4);
}

extern const PTMF16 Gesture_Back_ptmf[5];   /* the gesture recognizers (Gesture_Back, Gesture_Forward, ..) */

/* a stick gesture, each frame: the recognizers (5, state +0x14/+0x28 each) are fed the
 * stick's length and direction in turn until one reports a gesture (not -1); the rest are
 * reset (+0x28). No stick: all reset. Returns the gesture, -1 = none. */
/* 0x001F1B90 */
s32 Gesture_Update(u8 *g, f32 *stick) {
    f32 len, ang;
    s32 r = -1;
    u32 i;

    if (stick == NULL) {
        for (i = 0; i < 5; i++) {
            AT(g, 0x28 + i * 4, s32) = 0;
            AT(g, 0x14 + i * 4, s32) = 0;
        }
        return -1;
    }
    len = __builtin_sqrtf(sceVu0InnerProduct(stick, stick));
    ang = msl_atan2f(stick[0], stick[2]);
    for (i = 0; i < 5; i++) {
        if (r == -1) {
            r = ptmf_scall_rff(g, &Gesture_Back_ptmf[i].p, len, ang);
        } else {
            AT(g, 0x28 + i * 4, s32) = 0;
        }
    }
    return r;
}

/* the root translation of slot `slot`'s animation `k` (0, 1: the layer) at its time + dt
 * (wrapped into the animation), scaled by the slot's weight; 0 if it has none */
static void motion_root(u8 *m, s32 slot, s32 k, f32 dt, f32 *out) {
    u8 *s = m + slot * 0xA0;
    f32 tmp[8] __attribute__((aligned(16)));
    void *anim = AT(s, 0x584 + k * 4, void *);
    s32 *track = AT(s, 0x59C + k * 4, s32 *);
    f32 t;

    if ((AT(s, 0x57C, u32) & 0x10) || anim == NULL || track == NULL || *track == 0) {
        return;
    }
    t = AT(s, 0x564 + k * 4, f32) + dt;
    if (t < 0.0f) {
        do {
            t += (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
        } while (t < 0.0f);
    }
    if (!(t < (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32))) {
        do {
            t -= (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
        } while (!(t < (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32)));
    }
    Track_Sample(track, tmp, t);
    sceVu0ScaleVector(out, tmp + 4, AT(s, 0x574 + k * 4, f32));
}

/* the current slot's (+0x540) root translation at its time + dt (0 if it has none) */
/* 0x001F6240 */
void Motion_RootTranslation(u8 *m, f32 *out, f32 dt) {
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 0.0f;
    motion_root(m, AT(m, 0x540, s32), 0, dt, out);
}

/* the motion's root movement over dt: the current slot (+0x540) and the previous one
 * (+0x544), each blended with its layer by the track's weight (+0x6A4/+0x6A8 +0x1C), then
 * cross-faded by +0x550 while a fade runs (+0x54C > 0) */
/* 0x001F6370 */
void Motion_RootMovement(u8 *m, f32 *out, f32 dt) {
    f32 cur[4] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 0.0f};
    f32 prev[4] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 0.0f};
    f32 layer[4] __attribute__((aligned(16)));
    s32 *l;

    motion_root(m, AT(m, 0x540, s32), 0, dt, cur);
    motion_root(m, AT(m, 0x544, s32), 0, dt, prev);
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 0.0f;
    l = AT(AT(m, 0x6A4, u8 *), 0x3C, s32 *);
    if (l != NULL && *l != 0) {
        layer[0] = layer[1] = layer[2] = layer[3] = 0.0f;
        motion_root(m, AT(m, 0x540, s32), 1, dt, layer);
        sceVu0InterVector(cur, cur, layer, AT(AT(m, 0x6A4, u8 *), 0x1C, f32));
    }
    l = AT(AT(m, 0x6A8, u8 *), 0x3C, s32 *);
    if (l != NULL && *l != 0) {
        layer[0] = layer[1] = layer[2] = layer[3] = 0.0f;
        motion_root(m, AT(m, 0x544, s32), 1, dt, layer);
        sceVu0InterVector(prev, prev, layer, AT(AT(m, 0x6A8, u8 *), 0x1C, f32));
    }
    if (AT(m, 0x54C, f32) <= 0.0f) {
        out[0] = cur[0];
        out[1] = cur[1];
        out[2] = cur[2];
    } else {
        sceVu0InterVector(out, prev, cur, AT(m, 0x550, f32));
    }
}

static const union { u32 u; f32 f; } kTrkRot = {0x38C90FDB},   /* 2 pi / 65536 */
    kTrkUnit = {0x38000100},                                  /* ~1 / 32767 */
    kTrkPi = {0x40490FDB}, kTrkTwoPi = {0x40C90FDB};

/* angle n moved by a turn so that it is within pi of cur */
static f32 track_unwrap(f32 n, f32 cur) {
    f32 d = n - cur;

    if (!((d <= 0.0f ? -d : d) <= kTrkPi.f)) {
        if (d <= 0.0f) {
            n += kTrkTwoPi.f;
        } else {
            n -= kTrkTwoPi.f;
        }
    }
    return n;
}

/* sample an animation track { keys, format (+ 0x10000: constant), key count } at time t:
 * out[0..2] rotation or position, out[4..6] position (formats 2, 4, 7), out[0..7] a matrix
 * row pair (6); between keys rotations and positions are interpolated (angles the short way) */
/* 0x001F36B0 */
void Track_Sample(void *track, f32 *out, f32 t) {
    u8 *trk = track;
    u32 flags = AT(trk, 0x4, u32);
    f32 base = (flags & 0x10000) ? 0.0f : (f32)(s32)t;
    s32 k = (s32)base;
    s16 *h;
    f32 *w;
    f32 frac;
    s32 k2;

    switch (flags & 0xFFFF) {
    case 0:
        h = AT(trk, 0x0, s16 *) + k * 3;
        out[0] = kTrkRot.f * (f32)h[0];
        out[1] = kTrkRot.f * (f32)h[1];
        out[2] = kTrkRot.f * (f32)h[2];
        break;
    case 1:
        h = AT(trk, 0x0, s16 *) + k * 3;
        out[0] = 0.00390625f * (f32)h[0];
        out[1] = 0.00390625f * (f32)h[1];
        out[2] = 0.00390625f * (f32)h[2];
        break;
    case 3:
        h = AT(trk, 0x0, s16 *) + k * 4;
        out[0] = kTrkUnit.f * (f32)h[0];
        out[1] = kTrkUnit.f * (f32)h[1];
        out[2] = kTrkUnit.f * (f32)h[2];
        out[3] = kTrkUnit.f * (f32)h[3];
        break;
    case 2:
    case 4:
        h = AT(trk, 0x0, s16 *) + k * 6;
        out[0] = kTrkRot.f * (f32)h[0];
        out[1] = kTrkRot.f * (f32)h[1];
        out[2] = kTrkRot.f * (f32)h[2];
        out[4] = 0.00390625f * (f32)h[3];
        out[5] = 0.00390625f * (f32)h[4];
        out[6] = 0.00390625f * (f32)h[5];
        break;
    case 5: {
        /* the original stores x and y unconverted (the s16 bits as a float) */
        union { s32 i; f32 f; } x, y;

        h = AT(trk, 0x0, s16 *) + k * 3;
        x.i = h[0];
        y.i = h[1];
        out[0] = x.f;
        out[1] = y.f;
        out[2] = kTrkUnit.f * (f32)h[2];
        break;
    }
    case 6:
        w = AT(trk, 0x0, f32 *) + k * 8;
        out[0] = w[0];
        out[1] = w[1];
        out[2] = w[2];
        out[3] = w[3];
        out[4] = w[4];
        out[5] = w[5];
        out[6] = w[6];
        out[7] = w[7];
        break;
    case 7:
        w = AT(trk, 0x0, f32 *) + k * 6;
        out[0] = w[0];
        out[1] = w[1];
        out[2] = w[2];
        out[4] = w[3];
        out[5] = w[4];
        out[6] = w[5];
        break;
    case 8:
    case 9:
        w = AT(trk, 0x0, f32 *) + k * 3;
        out[0] = w[0];
        out[1] = w[1];
        out[2] = w[2];
        break;
    }
    if (t == base || (AT(trk, 0x4, u32) & 0x10000)) {
        return;
    }
    frac = t - base;
    k2 = (s32)base + 1;
    if (!(k2 < AT(trk, 0x8, s32))) {
        k2 = 0;
    }
    switch (AT(trk, 0x4, u32) & 0xFFFF) {
    case 0: {
        f32 n0, n1, n2;

        h = AT(trk, 0x0, s16 *) + k2 * 3;
        n0 = track_unwrap(kTrkRot.f * (f32)h[0], out[0]);
        n1 = track_unwrap(kTrkRot.f * (f32)h[1], out[1]);
        n2 = track_unwrap(kTrkRot.f * (f32)h[2], out[2]);
        out[0] = out[0] + frac * (n0 - out[0]);
        out[1] = out[1] + frac * (n1 - out[1]);
        out[2] = out[2] + frac * (n2 - out[2]);
        break;
    }
    case 1:
        h = AT(trk, 0x0, s16 *) + k2 * 3;
        out[0] = out[0] + frac * (0.00390625f * (f32)h[0] - out[0]);
        out[1] = out[1] + frac * (0.00390625f * (f32)h[1] - out[1]);
        out[2] = out[2] + frac * (0.00390625f * (f32)h[2] - out[2]);
        break;
    case 2:
    case 4: {
        f32 n0, n1, n2;

        h = AT(trk, 0x0, s16 *) + k2 * 6;
        n0 = track_unwrap(kTrkRot.f * (f32)h[0], out[0]);
        n1 = track_unwrap(kTrkRot.f * (f32)h[1], out[1]);
        n2 = track_unwrap(kTrkRot.f * (f32)h[2], out[2]);
        out[0] = out[0] + frac * (n0 - out[0]);
        out[1] = out[1] + frac * (n1 - out[1]);
        out[2] = out[2] + frac * (n2 - out[2]);
        out[4] = out[4] + frac * (0.00390625f * (f32)h[3] - out[4]);
        out[5] = out[5] + frac * (0.00390625f * (f32)h[4] - out[5]);
        out[6] = out[6] + frac * (0.00390625f * (f32)h[5] - out[6]);
        break;
    }
    case 7:
        w = AT(trk, 0x0, f32 *) + k2 * 6;
        out[0] = out[0] + frac * (w[0] - out[0]);
        out[1] = out[1] + frac * (w[1] - out[1]);
        out[2] = out[2] + frac * (w[2] - out[2]);
        out[4] = out[4] + frac * (w[3] - out[4]);
        out[5] = out[5] + frac * (w[4] - out[5]);
        out[6] = out[6] + frac * (w[5] - out[6]);
        break;
    }
}

/* 0x001F46E0 */
void Model_Vt24(u8 *m) {
}

/* 0x001F46F0 */
void Model_Vt20(u8 *m) {
}

/* +0x1C / +0x20 / +0x24 / +0x2C / +0x30 / +0x14 / +0x3C / +0x4C / +0x50 / +0x54 / +0x60:
   nothing */
/* 0x001F4700 */
void Model_Vt1C(u8 *m) {
}

/* the current slot's root rotation (track 0, y) at its time + dt, scaled by its weight;
 * 0 without an animation */
/* 0x001F6140 */
f32 Motion_RootRotation(u8 *m, f32 dt) {
    u8 *s = m + AT(m, 0x540, s32) * 0xA0;
    f32 tmp[8] __attribute__((aligned(16)));
    void *anim = AT(s, 0x584, void *);
    s32 *track = AT(s, 0x59C, s32 *);
    f32 t;

    if (anim == NULL || track == NULL || *track == 0) {
        return 0.0f;
    }
    t = AT(s, 0x564, f32) + dt;
    if (t < 0.0f) {
        do {
            t += (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
        } while (t < 0.0f);
    }
    if (!(t < (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32))) {
        do {
            t -= (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
        } while (!(t < (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32)));
    }
    Track_Sample(track, tmp, t);
    return AT(s, 0x574, f32) * tmp[1];
}

/* the event flags of layer `layer`'s motion at its current frame + `dt` frames (m +0x4D4: per
 * motion of the library +0x4C4, one byte per frame); wrapped into the motion when `loop` and
 * the track loops (+0x18 bit 1), else 0 outside it */
/* 0x001F4770 */
s32 Motion_EventFlags(u8 *m, s32 layer, s32 dt, u32 loop) {
    u8 *ev = AT(m, 0x4D4, u8 *);
    u8 *lib = AT(m, 0x4C4, u8 *);
    s32 id, idx = -1;
    u8 *trk, *anim, *bytes;
    s32 t, len;

    if (ev == NULL) {
        return 0;
    }
    id = AT(m, 0x55C + layer * 4, s32);
    if (lib != NULL) {
        u8 *tbl = lib + AT(lib, 0xC, s32);
        u32 n = AT(tbl, 0x0, u32);
        u32 i;

        for (i = 0; i < n; i++) {
            if (AT(tbl, 0x10 + i * 8, s32) == id) {
                idx = i;
                break;
            }
        }
    }
    if (idx == -1) {
        return 0;
    }
    trk = AT(m, 0x6A4, u8 *);
    t = (s32)((f32)dt + AT(trk, layer * 4, f32));
    anim = AT(trk, 0x20 + layer * 4, u8 *);
    len = AT(AT(anim, 0x4, u8 *), 0xC, s32);
    bytes = ev + AT(ev, 0x4 + AT(ev, 0x0, s32) * 4 + idx * 4, s32);
    if (AT(trk, 0x18, u32) & (loop & 1)) {
        f32 ft = (f32)t;

        if (ft < 0.0f) {
            do {
                ft += (f32)len;
            } while (ft < 0.0f);
        }
        while (!(ft < (f32)len)) {
            ft -= (f32)len;
        }
        return bytes[(s32)ft];
    }
    if (t < 0 || len - 1 < t) {
        return 0;
    }
    return bytes[t];
}

extern void Track_Blend(u8 *m, f32 *out, void *trkA, void *trkB, f32 ta, f32 tb, f32 w);   /* a bone's position */

/* `base` + dt * `speed`, wrapped into an animation of `anim`'s length (0 without one) */
static f32 motion_time(u8 *anim, f32 base, f32 dt, f32 speed) {
    f32 t;

    if (anim == NULL) {
        return 0.0f;
    }
    t = base + dt * speed;
    if (t < 0.0f) {
        do {
            t += (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
        } while (t < 0.0f);
    }
    while (!(t < (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32))) {
        t -= (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
    }
    return t;
}

/* a foot (`left` or right) at the motion's time + dt: its position (out; z scaled by
 * `zscale`, w = 1), cross-faded from the previous track (+0x6A8) by +0x550, and whether it is
 * on the ground (the current track's contact channel +0x50); 0 without contact data */
/* 0x002DD420 */
s32 Motion_FootPos(u8 *m, f32 *out, s32 left, f32 dt, f32 zscale) {
    u8 *cur = AT(m, 0x6A4, u8 *);
    u8 *prev;
    f32 contact[8] __attribute__((aligned(16)));
    f32 pa[4] __attribute__((aligned(16)));
    f32 pb[4] __attribute__((aligned(16)));
    f32 c0, c1, p0, p1, d;
    s32 k;

    if (AT(cur, 0x50, void *) == NULL) {
        return 0;
    }
    d = (AT(cur, 0x18, u32) & 0x10) ? 0.0f : dt;
    c0 = motion_time(AT(cur, 0x20, u8 *), AT(cur, 0x0, f32), d, AT(cur, 0x10, f32));
    c1 = motion_time(AT(cur, 0x24, u8 *), AT(cur, 0x4, f32), d, AT(cur, 0x14, f32));
    prev = AT(m, 0x6A8, u8 *);
    if (AT(prev, 0x18, u32) & 0x10) {
        dt = 0.0f;
    }
    p0 = motion_time(AT(prev, 0x20, u8 *), AT(prev, 0x0, f32), dt, AT(prev, 0x10, f32));
    p1 = motion_time(AT(prev, 0x24, u8 *), AT(prev, 0x4, f32), dt, AT(prev, 0x14, f32));
    Track_Sample(AT(cur, 0x50, void *), contact, c0);
    k = left != 0 ? 2 : 1;
    cur = AT(m, 0x6A4, u8 *);
    Track_Blend(m, pa, AT(cur, 0x38 + k * 8, void *), AT(cur, 0x3C + k * 8, void *), c0, c1,
                  1.0f - AT(cur, 0x1C, f32));
    prev = AT(m, 0x6A8, u8 *);
    if (AT(prev, 0x20, void *) != NULL) {
        Track_Blend(m, pb, AT(prev, 0x38 + k * 8, void *), AT(prev, 0x3C + k * 8, void *), p0, p1,
                      1.0f - AT(prev, 0x1C, f32));
        vu0_LerpXYZ(pa, pb, pa, AT(m, 0x550, f32));
    }
    out[0] = pa[0];
    out[1] = pa[1];
    out[2] = pa[2] * zscale;
    AT(out, 0xC, u32) = 0x3F800000;   /* 1.0 */
    return contact[left] > 0.0f;
}

/* a bone's channel blended between two tracks: track A at ta and B at tb, out = B * w +
 * A * (1 - w) (xyz; format 2 tracks have a second vector at out + 0x10); with only one track,
 * that one (the original takes the second vector from A even when only B exists) */
/* 0x001F5F70 */
void Track_Blend(u8 *m, f32 *out, void *trackA, void *trackB, f32 ta, f32 tb, f32 w) {
    s32 *trkA = trackA, *trkB = trackB;
    f32 a[8] __attribute__((aligned(16)));
    f32 b[8] __attribute__((aligned(16)));

    a[0] = a[1] = a[2] = 0.0f;
    a[4] = a[5] = a[6] = 0.0f;
    b[0] = b[1] = b[2] = 0.0f;
    b[4] = b[5] = b[6] = 0.0f;
    if (trkA != NULL && *trkA != 0) {
        Track_Sample(trkA, a, ta);
    }
    if (trkB != NULL && *trkB != 0) {
        Track_Sample(trkB, b, tb);
    }
    if (trkA != NULL && *trkA != 0 && trkB != NULL && *trkB != 0) {
        vu0_LerpXYZ(out, b, a, w);
        if (AT(trkA, 0x4, u16) == 2) {
            vu0_LerpXYZ(out + 4, b + 4, a + 4, w);
        }
    } else if (trkA != NULL && *trkA != 0) {
        sceVu0CopyVector(out, a);
        if (AT(trkA, 0x4, u16) == 2) {
            sceVu0CopyVector(out + 4, a + 4);
        }
    } else {
        sceVu0CopyVector(out, b);
        if (AT(trkA, 0x4, u16) == 2) {
            sceVu0CopyVector(out + 4, a + 4);
        }
    }
}

/* 0x001F6130 */
void Model_AdjustBone(u8 *m) {
}

/* the model's height above the character's floor (+0x804, smoothed 3:1 with last frame's
 * +0x8C0 unless +0x990 asks for a jump), each frame from the character's matrix and position
 * (kept at +0x7D0 / +0x800): the lower of the two feet put on the floor (feet in the air keep
 * the character's height; flag 0x100: planted feet only), or on slopes (track flag 4) two
 * points `back` / `front` along the step; nothing while both are 0 */
/* 0x00210E00 */
void HumanModel_BodyFrames(u8 *m, u8 *a, f32 back, f32 front) {
    f32 l[4] __attribute__((aligned(16)));
    f32 r[4] __attribute__((aligned(16)));

    sceVu0CopyMatrix((f32 (*)[4])(m + 0x7D0), (f32 (*)[4])(a + 0x60));
    sceVu0CopyVector((f32 *)(m + 0x800), (f32 *)(a + 0x10));
    AT(m, 0x80C, f32) = 1.0f;
    if (back == 0.0f && front == 0.0f) {
        AT(m, 0x8C0, f32) = AT(m, 0x804, f32);
        return;
    }
    if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 4) {
        f32 n[4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));
        f32 ny, k;

        VCALL(gNavMesh, 0x2C, void (*)(NavMesh *, u32, f32 *))(gNavMesh, AT(a, 0x34, u32), n);
        ny = n[1];
        Motion_RootMovement(m, v, 0.0f);
        k = v[2] * (ny * ny);
        l[0] = 0.0f;
        l[1] = 0.0f;
        l[2] = back * k;
        l[3] = 1.0f;
        r[0] = 0.0f;
        r[1] = 0.0f;
        r[2] = front * k;
        r[3] = 1.0f;
        sceVu0ApplyMatrix(l, (f32 (*)[4])(m + 0x7D0), l);
        sceVu0ApplyMatrix(r, (f32 (*)[4])(m + 0x7D0), r);
        Motion_OntoFloor(m, l, a);
        Motion_OntoFloor(m, r, a);
    } else {
        u32 dl = (u8)Motion_FootPos(m, l, 1, 0.0f, 1.0f);
        u32 dr = (u8)Motion_FootPos(m, r, 0, 0.0f, 1.0f);
        s32 useL = 1, useR = 1;

        sceVu0ApplyMatrix(l, (f32 (*)[4])(m + 0x7D0), l);
        sceVu0ApplyMatrix(r, (f32 (*)[4])(m + 0x7D0), r);
        if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 0x100) {
            if (dl == 1 && AT(AT(m, 0x6A8, u8 *), 0x20, void *) != NULL) {
                dl = (u8)Motion_FootDownPrev(m, 1, 0.0f);
            }
            if (dl == 0) {
                useL = 0;
            }
            if (dr == 1 && AT(AT(m, 0x6A8, u8 *), 0x20, void *) != NULL) {
                dr = (u8)Motion_FootDownPrev(m, 0, 0.0f);
            }
            if (dr == 0) {
                useR = 0;
            }
        }
        if (useL == 1) {
            Motion_OntoFloor(m, l, a);
        } else {
            l[1] = AT(a, 0x14, f32);
        }
        if (useR == 1) {
            Motion_OntoFloor(m, r, a);
        } else {
            r[1] = AT(a, 0x14, f32);
        }
    }
    AT(m, 0x804, f32) = l[1] < r[1] ? l[1] : r[1];
    if (AT(m, 0x990, u8) == 0) {
        AT(m, 0x804, f32) = 0.25f * AT(m, 0x804, f32) + 0.75f * AT(m, 0x8C0, f32);
    } else {
        AT(m, 0x990, u8) = 0;
    }
    AT(m, 0x8C0, f32) = AT(m, 0x804, f32);
}

/* put point p on the floor: walk the nav mesh from the character's triangle towards it
 * (vtable +0x24: the edge crossed, 3 inside, 4 lost); in a triangle the mesh gives its height
 * (+0x14); at a wall (no neighbour, or a marked one (+0x18 bit 0x80) blocked by the
 * character's mask +0xC0) its height continues along the line to where it was crossed */
/* 0x002DC710 */
void Motion_OntoFloor(u8 *m, f32 *p, u8 *a) {
    NavMesh *nm;
    NavTri *t;
    u32 tri = AT(a, 0x34, u32);
    f32 hit[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 len;

    if (tri == (u32)-1) {
        return;
    }
    nm = gNavMesh;
    t = NavMesh_Tri(nm, tri);
    for (;;) {
        s32 e = VCALL(nm, 0x24, s32 (*)(NavMesh *, u32, f32 *, f32 *, f32 *))(nm, tri, hit, (f32 *)(a + 0x10), p);
        u8 mark;

        if (e == 3) {
            VCALL(nm, 0x14, void (*)(NavMesh *, u32, f32 *))(nm, tri, p);
            return;
        }
        if (e == 4) {
            VCALL(nm, 0x14, void (*)(NavMesh *, u32, f32 *))(nm, AT(a, 0x34, u32), p);
            return;
        }
        tri = t->adj[e];
        if (tri == (u32)-1) {
            break;
        }
        t = NavMesh_Tri(nm, tri);
        mark = (AT(nm, 0x18, u8 *) != NULL && tri < nm->numTris && nm->tris != NULL) ? AT(nm, 0x18, u8 *)[tri] : 0;
        if ((mark & 0x80) && (t->flags & AT(a, 0xC0, u32))) {
            break;
        }
    }
    sceVu0SubVector(d, p, (f32 *)(a + 0x10));
    len = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    sceVu0SubVector(d, hit, (f32 *)(a + 0x10));
    sceVu0Normalize(d, d);
    p[1] = AT(a, 0x14, f32) + d[1] * len;
}

extern s32 D_004157A0[];   /* hand poses: per pose 5 key frames { s32, s32, f32 } */

/* the hands, when the motion moves on: the motion's hand pose (+0x874 table, byte 2: left pose
 * << 4 | right pose) is blended in over the next 5 frames as their events (bit 4 left, 8
 * right) say, forwards or backwards by the hand's state (+0x85C / +0x85D), into +0x38 / +0x48
 * (poses 0..6 as they are, others fall back to the defaults +0x87C / +0x880); a restarted
 * motion resets them */
/* 0x002DCB40 */
void Motion_Hands(u8 *m) {
    u8 ev[5];
    s32 hand, i, k, idx, bit;
    u32 pose, on;
    u8 *t = AT(m, 0x6A4, u8 *);

    if (AT(t, 0x0, f32) == AT(t, 0x8, f32)) {
        return;
    }
    if (AT(t, 0x0, f32) == 0.0f) {
        AT(m, 0x85C, u8) = 0;
        AT(m, 0x85D, u8) = 0;
        AT(m, 0x38, s32) = AT(m, 0x87C, s32);
        AT(m, 0x3C, s32) = AT(m, 0x87C, s32);
        AT(m, 0x40, f32) = 0.0f;
        AT(m, 0x48, s32) = AT(m, 0x880, s32);
        AT(m, 0x4C, s32) = AT(m, 0x880, s32);
        AT(m, 0x50, f32) = 0.0f;
    }
    if (AT(m, 0x874, u8 *) == NULL) {
        return;
    }
    k = Motion_AnimIndex(m, AT(m, 0x55C, s32));
    if (k == -1) {
        return;
    }
    pose = AT(m, 0x874, u8 *)[k * 6 + 2];
    if (pose == 0) {
        return;
    }
    for (i = 0; i < 5; i++) {
        ev[i] = Motion_EventFlags(m, 0, i, -2);
    }
    for (hand = 0; hand < 2; hand++) {
        s32 *tbl;
        s32 *dst = &AT(m, 0x38 + hand * 16, s32);

        if (hand == 0) {
            on = AT(m, 0x85C, u8);
            bit = 4;
            idx = (pose >> 4) & 0xF;
        } else {
            on = AT(m, 0x85D, u8);
            bit = 8;
            idx = pose & 0xF;
        }
        tbl = D_004157A0 + idx * 15;
        for (i = 0; i < 5; i++) {
            s32 *e = on ? tbl + i * 3 : tbl + (4 - i) * 3;

            if (!(ev[i] & bit)) {
                continue;
            }
            dst[0] = (idx < 7 || e[0] != 0) ? e[0] : AT(m, 0x87C + hand * 4, s32);
            dst[1] = (idx < 7 || e[1] != 0) ? e[1] : AT(m, 0x87C + hand * 4, s32);
            AT(dst, 0x8, f32) = AT(e, 0x8, f32);
            if (i == 0) {
                on = (on ^ 1) != 0;
            }
        }
        AT(m, 0x85C + hand, u8) = on;
    }
}

/* +0x40: the model matrix from the actor's position and heading */
/* 0x002DCDD0 */
void Model_BodyFrames(u8 *m, void *actor, f32 a, f32 b) {
    Mtx_Model((f32 (*)[4])(m + 0x7D0), (f32 *)((u8 *)actor + 0x10), AT(actor, 0x54, f32));
}

extern f32 D_00415B60[14];   /* the eyelids through a blink */

/* the eyes, each frame: a blink plays the eyelid curve over 14 frames (+0x870 counts, both
 * lids +0x74 / +0x78); from frame 30 a new blink starts at random, surely by frame 150 */
/* 0x002DC960 */
void Motion_Eyes(u8 *m) {
    if (AT(m, 0x870, s32) < 14) {
        AT(m, 0x78, f32) = D_00415B60[AT(m, 0x870, s32)];
        AT(m, 0x74, f32) = D_00415B60[AT(m, 0x870, s32)];
    }
    if (AT(m, 0x870, s32) >= 30) {
        f32 r = VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);

        if ((s32)((f32)(150 - AT(m, 0x870, s32)) * r) == 0) {
            AT(m, 0x870, s32) = 0;
        }
    }
    AT(m, 0x870, s32)++;
}

/* 0x002DCA50 */
void Model_HalfHip(void *self, u32 *out) {
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
}

/* +0x68: the drawing state cleared (+0x38.., the 16 words at +0x58), then +0x30 */
/* 0x002DCA70 */
void Model_ClearDraw(u8 *m) {
    s32 i;

    AT(m, 0x38, s32) = 0;
    AT(m, 0x3C, s32) = 0;
    AT(m, 0x40, s32) = 0;
    AT(m, 0x48, s32) = 0;
    AT(m, 0x4C, s32) = 0;
    AT(m, 0x50, s32) = 0;
    for (i = 0; i < 16; i++) {
        AT(m, 0x58 + i * 4, s32) = 0;
    }
    AT(m, 0x870, s32) = 0;
    AT(m, 0x30, u8) = 0;
    VCALL(m, 0x30, void (*)(u8 *))(m);
}

/* 0x002DCAE0 */
void Motion_Disable(u8 *p) {
    b4_clear_dca70(p);
    p[0x30] = 1;
}

extern void Motion_ExtraChannels(u8 *m);
extern void Motion_KeepInStep(u8 *m);
extern void Motion_ApplyChains(u8 *m);
extern void *Motion_BlendLists(u8 *m, void *a, void *b, f32 t);   /* the blend of two poses */
extern void Skel_WorldPose(u8 *m, void *skel);
extern void Motion_Advance(u8 *m);

/* the motion player, each frame: start the queued motion (+0x4F0) and the queued layer
 * motions (+0x504, 3 x 0x14) once their fades are over (a layer waits a frame when the main
 * motion just started), free the faded-out layers' buffers, advance the tracks, then blend
 * the pose: each track's two animations, the current with the previous one (+0x550), the three
 * layers; the skeleton is built from it (+0x810), then vtable +0x18 and Motion_Advance */
/* 0x001F6AF0 */
void Motion_Update(u8 *m) {
    void *chains, *skels, *a, *b;
    s32 started = 0;
    s32 i;

    AT(AT(m, 0x6A4, u8 *), 0x18, u32) &= ~0x80;
    AT(AT(m, 0x704, u8 *), 0xC, u32) &= ~0x80;
    AT(AT(m, 0x764, u8 *), 0xC, u32) &= ~0x80;
    AT(AT(m, 0x7C4, u8 *), 0xC, u32) &= ~0x80;
    if (AT(m, 0x4F0, u8) != 0 && AT(m, 0x54C, f32) <= 0.0f) {
        Motion_Start(m, AT(m, 0x4F4, s32), AT(m, 0x4F8, u16), AT(m, 0x500, s32), AT(m, 0x4FC, f32));
        AT(m, 0x4F0, u8) = 0;
        started = 1;
        AT(AT(m, 0x6A4, u8 *), 0x18, u32) |= 0x80;
    }
    for (i = 0; i < 3; i++) {
        u8 *q = m + i * 0x14;

        if (AT(q, 0x504, u8) == 0 || !(AT(m, 0x6BC + i * 0x60, f32) <= 0.0f)) {
            continue;
        }
        if (!started) {
            Motion_Start(m, AT(q, 0x508, s32), AT(q, 0x50C, u16), AT(q, 0x514, s32), AT(q, 0x510, f32));
            AT(q, 0x504, u8) = 0;
            AT(AT(m, 0x704 + i * 0x60, u8 *), 0xC, u32) |= 0x80;
        } else {
            AT(q, 0x504, u8) = 0;
        }
    }
    chains = gChainPool;
    skels = gSkelPool;
    for (i = 0; i < 3; i++) {
        u8 *l = m + i * 0x60;
        u8 *s;

        if (!(AT(l, 0x6BC, f32) <= 0.0f)) {
            continue;
        }
        s = l + AT(l, 0x6B4, s32) * 0x1C;
        if (AT(s, 0x6DC, void *) != NULL) {
            ChainPool_Free(chains, AT(s, 0x6DC, void *));
            AT(s, 0x6DC, void *) = NULL;
        }
        if (AT(s, 0x6E0, void *) != NULL) {
            SkelPool_Free(skels, AT(s, 0x6E0, u8 *));
            AT(s, 0x6E0, void *) = NULL;
        }
    }
    if (AT(m, 0x54C, f32) <= 0.0f) {
        Motion_FreeSlot(m, AT(m, 0x544, s32));
        if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 8) {
            AT(AT(m, 0x6A4, u8 *), 0x18, u32) &= ~0x10;
        }
    }
    Motion_ExtraChannels(m);
    Motion_KeepInStep(m);
    Motion_ApplyChains(m);
    a = Motion_BlendLists(m, AT(AT(m, 0x6A4, u8 *), 0x28, void *), AT(AT(m, 0x6A4, u8 *), 0x2C, void *),
                      1.0f - AT(AT(m, 0x6A4, u8 *), 0x1C, f32));
    b = Motion_BlendLists(m, AT(AT(m, 0x6A8, u8 *), 0x28, void *), AT(AT(m, 0x6A8, u8 *), 0x2C, void *),
                      1.0f - AT(AT(m, 0x6A8, u8 *), 0x1C, f32));
    AT(m, 0x6AC, void *) = Motion_BlendLists(m, a, b, AT(m, 0x550, f32));
    for (i = 0; i < 3; i++) {
        AT(m, 0x70C + i * 0x60, void *) = Motion_BlendLists(m, AT(AT(m, 0x704 + i * 0x60, u8 *), 0x14, void *),
                                                        AT(AT(m, 0x708 + i * 0x60, u8 *), 0x14, void *),
                                                        AT(m, 0x6C0 + i * 0x60, f32));
    }
    Skel_WorldPose(m, AT(m, 0x810, void *));
    VCALL(m, 0x18, void (*)(u8 *))(m);
    Motion_Advance(m);
}

/* +0x28: the model matrix +0x7D0 = `mtx` */
/* 0x001F6E00 */
void Model_SetMatrix(u8 *m, f32 (*mtx)[4]) {
    sceVu0CopyMatrix((f32 (*)[4])(m + 0x7D0), mtx);
}

/* the current track's extra channels at its time: four (+0x68, 8 bytes apart) into +0x58..,
 * two (+0x88) into the hand poses +0x38 / +0x48 */
/* 0x001F4F40 */
void Motion_ExtraChannels(u8 *m) {
    s32 i;

    for (i = 0; i < 4; i++) {
        s32 *trk = AT(AT(m, 0x6A4, u8 *), 0x68 + i * 8, s32 *);

        if (trk != NULL && *trk != 0) {
            Track_Sample(trk, (f32 *)(m + 0x58 + i * 4), AT(AT(m, 0x6A4, u8 *), 0x0, f32));
        }
    }
    for (i = 0; i < 2; i++) {
        s32 *trk = AT(AT(m, 0x6A4, u8 *), 0x88 + i * 8, s32 *);

        if (trk != NULL && *trk != 0) {
            Track_Sample(trk, (f32 *)(m + 0x38 + i * 16), AT(AT(m, 0x6A4, u8 *), 0x0, f32));
        }
    }
}

/* the length of an animation, in frames */
static f32 anim_frames(u8 *anim) {
    return (f32)AT(AT(anim, 0x4, u8 *), 0xC, s32);
}

/* keep blended animations in step, each frame: in each of the two slots (0xA0 apart) the
 * second animation (+0x588) follows the first's phase and their speeds (+0x574 / +0x578)
 * meet by the blend +0x580; while the motion fades (+0x54C) between two step-synced tracks
 * (flag 2), the tracks' speeds (+0x10) meet the same way by +0x550 instead */
/* 0x001F4D70 */
void Motion_KeepInStep(u8 *m) {
    s32 i;

    if (AT(m, 0x54C, f32) != 0.0f) {
        u8 *prev = AT(m, 0x6A8, u8 *);
        u8 *cur = AT(m, 0x6A4, u8 *);

        if (AT(cur, 0x18, u32) & AT(prev, 0x18, u32) & 2) {
            f32 t = AT(m, 0x550, f32);
            f32 u = 1.0f - t;
            f32 lp = anim_frames(AT(prev, 0x20, u8 *));
            f32 lc = anim_frames(AT(cur, 0x20, u8 *));

            AT(cur, 0x10, f32) = u + (lc / lp) * t;
            AT(AT(m, 0x6A8, u8 *), 0x10, f32) = t + (lp / lc) * u;
            for (i = 0; i < 2; i++, m += 0xA0) {
                if (AT(m, 0x588, u8 *) == NULL) {
                    AT(m, 0x574, f32) = 1.0f;
                    continue;
                }
                AT(m, 0x568, f32) = anim_frames(AT(m, 0x588, u8 *)) * (AT(m, 0x564, f32) / anim_frames(AT(m, 0x584, u8 *)));
                AT(m, 0x578, f32) = AT(m, 0x574, f32) * anim_frames(AT(m, 0x588, u8 *)) / anim_frames(AT(m, 0x584, u8 *));
            }
            return;
        }
    }
    for (i = 0; i < 2; i++, m += 0xA0) {
        f32 b, l0, l1;

        if (AT(m, 0x588, u8 *) == NULL) {
            AT(m, 0x574, f32) = 1.0f;
            continue;
        }
        b = AT(m, 0x580, f32);
        l1 = anim_frames(AT(m, 0x588, u8 *));
        l0 = anim_frames(AT(m, 0x584, u8 *));
        AT(m, 0x574, f32) = b + (l0 / l1) * (1.0f - b);
        AT(m, 0x578, f32) = (1.0f - b) + (l1 / l0) * b;
        AT(m, 0x568, f32) = anim_frames(AT(m, 0x588, u8 *)) * (AT(m, 0x564, f32) / anim_frames(AT(m, 0x584, u8 *)));
    }
}

extern void Motion_PoseChain(u8 *m, void *chain, void *anim, f32 t, f32 w);   /* pose a chain from an animation */

/* apply the animations to their bone chains: the layers' (3 x 2 at +0x6CC, 0x1C apart) and
 * the two slots' two animations (+0x584) */
/* 0x001F5850 */
void Motion_ApplyChains(u8 *m) {
    s32 i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            u8 *s = m + i * 0x60 + j * 0x1C + 0x6CC;

            if (AT(s, 0x10, void *) != NULL) {
                Motion_PoseChain(m, AT(s, 0x14, void *), AT(s, 0x10, void *), AT(s, 0x0, f32), AT(s, 0x18, f32));
            }
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            u8 *s = m + i * 0xA0 + 0x564 + j * 4;

            if (AT(s, 0x20, void *) != NULL) {
                Motion_PoseChain(m, AT(s, 0x28, void *), AT(s, 0x20, void *), AT(s, 0x0, f32), AT(s, 0x98, f32));
            }
        }
    }
}

/* blend two bone lists { +0x4 first bone (next +0x48, id +0x40), +0x8 count }: the longer
 * one (by t, or the other by 1 - t) takes in each of its bones the other's bone with the same
 * id; returns the blended list (one missing: the other) */
/* 0x001F56F0 */
void *Motion_BlendLists(u8 *m, void *listA, void *listB, f32 t) {
    u8 *a = listA, *b = listB;
    f32 tmp[4] __attribute__((aligned(16)));
    u8 *big, *small, *n;
    s32 i;

    tmp[3] = tmp[2] = tmp[1] = tmp[0] = 0.0f;
    if (a == NULL && b == NULL) {
        return NULL;
    }
    if (b == NULL) {
        return a;
    }
    if (a == NULL) {
        return b;
    }
    if (AT(a, 0x8, s32) < AT(b, 0x8, s32)) {
        big = b;
        small = a;
        t = 1.0f - t;
    } else {
        big = a;
        small = b;
    }
    n = AT(big, 0x4, u8 *);
    for (i = 0; i < AT(big, 0x8, s32); i++, n = AT(n, 0x48, u8 *)) {
        u8 *o = AT(small, 0x4, u8 *);
        s32 k;

        for (k = 0; k < AT(small, 0x8, s32); k++, o = AT(o, 0x48, u8 *)) {
            if (AT(n, 0x40, s32) == AT(o, 0x40, s32)) {
                Bone_BlendMatrix(tmp, (f32 (*)[4])n, (f32 (*)[4])n, (f32 (*)[4])o, t);
                break;
            }
        }
    }
    return big;
}

extern void Mtx_FromEuler(void *mat, f32 *rot, const f32 *trans);   /* bone matrix */

/* sample one track at `t` into the motion's rotation (+0x820) and translation (+0x830): kinds
 * 0 (rotation only: the bind translation), 2 / 7 (both), others (translation only: the bind
 * rotation) */
static void pose_sample(u8 *m, u8 *bones, u8 *trk, f32 t) {
    switch (AT(trk, 0x8, u16)) {
    case 7:
    case 2:
        Track_Sample(trk + 4, (f32 *)(m + 0x820), t);
        break;
    case 0:
        Track_Sample(trk + 4, (f32 *)(m + 0x820), t);
        sceVu0CopyVector((f32 *)(m + 0x830), (f32 *)(bones + AT(trk, 0x0, s32) * 0x70 + 0x20));
        break;
    default:
        sceVu0CopyVector((f32 *)(m + 0x820), (f32 *)(bones + AT(trk, 0x0, s32) * 0x70 + 0x10));
        Track_Sample(trk + 4, (f32 *)(m + 0x830), t);
        break;
    }
}

/* pose a bone chain { +0x4 first bone (next +0x48, id +0x40) } from an animation { +0x4 first
 * track (bone +0x0, kind +0x8, next +0x10), +0x8 count } at frame `t`. With `w` > 0 each bone
 * not in the motion's fixed list (+0x844, +0x840 entries) is eased: turned from its pose a
 * frame earlier towards the new one by `w` (shortest way), its position mixed the same */
/* 0x001F5930 */
void Motion_PoseChain(u8 *m, void *chain, void *anim, f32 t, f32 w) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB}, kNegPi = {0xC0490FDB};
    f32 axis[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    f32 p0[4] __attribute__((aligned(16)));
    f32 p1[4] __attribute__((aligned(16)));
    f32 cur[4][4] __attribute__((aligned(16)));
    f32 r[4][4] __attribute__((aligned(16)));
    f32 prev[4][4] __attribute__((aligned(16)));
    u8 *bones = AT(m, 0x4C0, u8 *) + 0x10;
    u8 *b = AT(chain, 0x4, u8 *);
    u8 *trk = AT(anim, 0x4, u8 *);
    s32 i, k, ease;

    for (i = 0; i < AT(anim, 0x8, s32); i++) {
        if (AT(trk, 0x0, s32) >= 0) {
            pose_sample(m, bones, trk, t);
            Mtx_FromEuler(b, (f32 *)(m + 0x820), (f32 *)(m + 0x830));
            ease = 1;
            for (k = 0; k < AT(m, 0x840, s16); k++) {
                if (AT(m, 0x844, s8 *)[k] == AT(b, 0x40, s32)) {
                    ease = 0;
                }
            }
            if (ease && !(w <= 0.0f)) {
                pose_sample(m, bones, trk, t - 1.0f);
                Mtx_FromEuler(prev, (f32 *)(m + 0x820), (f32 *)(m + 0x830));
                q[3] = 0.0f;
                q[0] = 0.0f;
                q[2] = 0.0f;
                q[1] = 0.0f;
                sceVu0CopyMatrix(r, prev);
                sceVu0CopyMatrix(cur, (f32 (*)[4])b);
                sceVu0CopyVector(p0, r[3]);
                sceVu0CopyVector(p1, cur[3]);
                r[3][1] = 0.0f;
                r[3][0] = 0.0f;
                r[3][2] = 0.0f;
                cur[3][0] = 0.0f;
                cur[3][1] = 0.0f;
                cur[3][2] = 0.0f;
                sceVu0TransposeMatrix(r, r);
                sceVu0MulMatrix(r, cur, r);
                Mtx_ToAxisAngle(q, axis, r);
                axis[3] = axis[3] * w;
                if (!(axis[3] <= kPi.f)) {
                    do {
                        axis[3] = axis[3] - kTwoPi.f;
                    } while (!(axis[3] <= kPi.f));
                }
                if (axis[3] < kNegPi.f) {
                    do {
                        axis[3] = axis[3] + kTwoPi.f;
                    } while (axis[3] < kNegPi.f);
                }
                Quat_FromAxisAngle(q, axis, axis[3]);
                Quat_ToMatrix(q, r);
                sceVu0InterVector(p0, p1, p0, w);
                sceVu0MulMatrix((f32 (*)[4])b, r, (f32 (*)[4])b);
                sceVu0CopyVector((f32 *)(b + 0x30), p0);
            }
        }
        trk = AT(trk, 0x10, u8 *);
        b = AT(b, 0x48, u8 *);
    }
}

/* the bone with id `id` in a bone list { +0x4 first (next +0x48, id +0x40), +0x8 count } */
static u8 *bone_find(u8 *list, s32 id) {
    u8 *b;
    s32 i;

    if (list == NULL) {
        return NULL;
    }
    b = AT(list, 0x4, u8 *);
    for (i = 0; i < AT(list, 0x8, s32); i++) {
        if (id == AT(b, 0x40, s32)) {
            return b;
        }
        b = AT(b, 0x48, u8 *);
    }
    return NULL;
}

/* the skeleton's world pose: each bone of `skel` (in parent-first order; parent matrix +0x44) is
 * its parent's matrix (the first: the model's, +0x7D0) times its local pose - from the blended
 * animation (+0x6AC), else the first layer that animates it (+0x70C, 3 x 0x60), else the bind
 * pose (the skeleton's record +0x10 / +0x20) - and, unless +0x4D8, reported to +0x14 */
/* 0x001F5D70 */
void Skel_WorldPose(u8 *m, void *skel) {
    f32 bind[4][4] __attribute__((aligned(16)));
    u8 *rec = AT(m, 0x4C0, u8 *) + 0x10;
    u8 *b = AT(skel, 0x4, u8 *);
    u8 *local;
    s32 i, k;

    for (i = 0; i < AT(skel, 0x8, s32); i++) {
        local = bone_find(AT(m, 0x6AC, u8 *), AT(b, 0x40, s32));
        for (k = 0; local == NULL && k < 3; k++) {
            local = bone_find(AT(m, 0x70C + k * 0x60, u8 *), AT(b, 0x40, s32));
        }
        if (local != NULL) {
            if (i == 0) {
                sceVu0MulMatrix((f32 (*)[4])b, (f32 (*)[4])(m + 0x7D0), (f32 (*)[4])local);
            } else {
                sceVu0MulMatrix((f32 (*)[4])b, (f32 (*)[4])AT(b, 0x44, u8 *), (f32 (*)[4])local);
            }
        } else {
            Mtx_FromEuler(bind, (f32 *)(rec + 0x10), (f32 *)(rec + 0x20));
            if (i == 0) {
                sceVu0MulMatrix((f32 (*)[4])b, (f32 (*)[4])(m + 0x7D0), bind);
            } else {
                sceVu0MulMatrix((f32 (*)[4])b, (f32 (*)[4])AT(b, 0x44, u8 *), bind);
            }
        }
        if (AT(m, 0x4D8, u8) == 0) {
            VCALL(m, 0x14, void (*)(u8 *, s32, u8 *, u8 *))(m, i, b, local);
        }
        b = AT(b, 0x48, u8 *);
        rec += 0x70;
    }
}

/* +0x14 per posed bone (the look-at): bone +0x8B0 is turned by -+0x854 about X and half of
 * +0x858 about Y in its own frame, bone +0x8B4 by half of +0x858 about Y about its own
 * position */
/* 0x002117C0 */
void HumanModel_AdjustBone(u8 *m, s32 i, f32 (*b)[4], void *local) {
    f32 r[4][4] __attribute__((aligned(16)));

    if (i == AT(m, 0x8B0, s32)) {
        sceVu0UnitMatrix(r);
        sceVu0RotMatrixX(r, r, -AT(m, 0x854, f32));
        sceVu0RotMatrixY(r, r, 0.5f * AT(m, 0x858, f32));
        sceVu0MulMatrix(b, b, r);
    }
    if (i == AT(m, 0x8B4, s32)) {
        sceVu0UnitMatrix(r);
        sceVu0RotMatrixY(r, r, 0.5f * AT(m, 0x858, f32));
        sceVu0CopyVector(r[3], b[3]);
        b[3][2] = 0.0f;
        b[3][1] = 0.0f;
        b[3][0] = 0.0f;
        sceVu0MulMatrix(b, r, b);
    }
}

/* +0x10 of the human base */
/* 0x002118C0 */
void HumanModel_Frame(u8 *m) {
    Model_Frame(m);
}

/* (does nothing) */
/* 0x001F1ED0 */
void Model_FeetContact(void) {
}

/* frames in an animation */
static s32 anim_len(void *anim) {
    return AT(AT(anim, 0x4, u8 *), 0xC, s32);
}

/* advance a play time { +0x0 time, prev (`prev`), speed (`speed`) } of animation `anim` by
 * its speed: looping (`flags` bit 0) wraps it into 0..frames (`wrapped` when it went past the
 * end), else it is clamped to 0..frames - 1 (`wrapped` at the end), the overshoot carried to
 * `carry` with `carryFlags` bit 9 while the motion fades (`fading`), else `carry` cleared */
static s32 anim_step(f32 *t, f32 *prev, f32 speed, u32 flags, void *anim, f32 *carry, u32 carryFlags, f32 fading) {
    s32 len = anim_len(anim), wrapped = 0;
    f32 v, v0, last;

    *prev = *t;
    *t = *t + speed;
    if (flags & 1) {
        v = *t;
        while (v < 0.0f) {
            v = v + (f32)len;
        }
        while (!(v < (f32)len)) {
            v = v - (f32)len;
            wrapped = 1;
        }
        *t = v;
        *carry = 0.0f;
        return wrapped;
    }
    v0 = *t;
    v = v0 < 0.0f ? 0.0f : v0;
    last = (f32)len - 1.0f;
    if (!(v <= last)) {
        v = last;
        wrapped = 1;
    }
    *t = v;
    if ((carryFlags & 0x200) && wrapped && fading != 0.0f) {
        *carry = *carry + (v0 - *t);
    } else {
        *carry = 0.0f;
    }
    return wrapped;
}

/* a fade's weight: smoothstep of the frames left / its length */
static f32 fade_weight(f32 left, f32 len) {
    f32 x, xx;

    if (len == 0.0f) {
        return 0.0f;
    }
    x = left / len;
    xx = x * x;
    return 3.0f * xx * (1.0f - x) + xx * x;
}

/* advance the motion a frame (unless the current slot's flags +0x18 have 0x40): both
 * animations of the current slot (+0x6A4) and, while fading (+0x54C), the previous one (+0x6A8)
 * - each pair { time, -, prev, -, speed, ... anim +0x20, carry +0x98 }; the first animation sets
 * the slot's flags 0x20 (wrapped) and 0x400 (at its end) - then the fade (+0x54C counts down,
 * weight +0x550), and the 3 layers' two animations (+0x6CC, 0x1C each) and fades (+0x6B8) */
/* 0x001F5130 */
void Motion_Advance(u8 *m) {
    u8 *slot = NULL, *a;
    s32 k, j, i, w;

    if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 0x40) {
        return;
    }
    for (k = 0; k < 2; k++) {
        if (k != 0 && AT(m, 0x54C, f32) == 0.0f) {
            break;
        }
        slot = k == 0 ? AT(m, 0x6A4, u8 *) : AT(m, 0x6A8, u8 *);
        for (j = 0; j < 2; j++) {
            a = slot + j * 4;
            if (AT(a, 0x20, void *) == NULL || (AT(slot, 0x18, u32) & 0x10)) {
                continue;
            }
            w = anim_step(&AT(a, 0x0, f32), &AT(a, 0x8, f32), AT(a, 0x10, f32), AT(slot, 0x18, u32), AT(a, 0x20, void *),
                          &AT(a, 0x98, f32), AT(slot, 0x18, u32), AT(m, 0x54C, f32));
            if (j == 0) {
                if (w) {
                    AT(slot, 0x18, u32) |= 0x20;
                } else {
                    AT(slot, 0x18, u32) &= ~0x20;
                }
                if (!(AT(a, 0x0, f32) < (f32)(anim_len(AT(a, 0x20, void *)) - 1))) {
                    AT(slot, 0x18, u32) |= 0x400;
                } else {
                    AT(slot, 0x18, u32) &= ~0x400;
                }
            }
        }
    }
    if (!(AT(m, 0x54C, f32) <= 0.0f)) {
        AT(m, 0x54C, f32) = AT(m, 0x54C, f32) - 1.0f;
    }
    AT(m, 0x550, f32) = fade_weight(AT(m, 0x54C, f32), AT(m, 0x548, f32));
    for (i = 0; i < 3; i++) {
        u8 *l = m + i * 0x60;

        for (j = 0; j < 2; j++) {
            a = l + j * 0x1C + 0x6CC;
            if (AT(a, 0x10, void *) == NULL) {
                continue;
            }
            /* (the carry tests the motion slot's flags, as the original) */
            if (anim_step(&AT(a, 0x0, f32), &AT(a, 0x4, f32), AT(a, 0x8, f32), AT(a, 0xC, u32), AT(a, 0x10, void *),
                          &AT(a, 0x18, f32), AT(slot, 0x18, u32), AT(m, 0x54C, f32))) {
                AT(a, 0xC, u32) |= 0x20;
            } else {
                AT(a, 0xC, u32) &= ~0x20;
            }
        }
        if (!(AT(l, 0x6BC, f32) <= 0.0f)) {
            AT(l, 0x6BC, f32) = AT(l, 0x6BC, f32) - 1.0f;
        }
        AT(l, 0x6C0, f32) = fade_weight(AT(l, 0x6BC, f32), AT(l, 0x6B8, f32));
    }
}

extern void Model_LegIK(u8 *m, f32 *right, f32 *left, f32 height);   /* bend the legs to the feet */

/* a foot bone's position (row 3 of its matrix) into `out`; with the slope weight `k` < 1 its
 * height above the floor is eased: out = position (+0x800) + side (+0x7F0) x k x (its offset
 * along it) + forward (+0x7D0) x (its offset along that) */
static void foot_point(u8 *m, s32 bone, f32 *out, f32 k) {
    f32 v[4] __attribute__((aligned(16)));
    f32 a, b, c, d;

    sceVu0CopyVector(out, Skel_Bone(AT(m, 0x810, void *), bone) + 12);
    if (k < 1.0f) {
        a = sceVu0InnerProduct((f32 *)(m + 0x7D0), (f32 *)(m + 0x800));
        b = sceVu0InnerProduct((f32 *)(m + 0x7F0), (f32 *)(m + 0x800));
        c = sceVu0InnerProduct((f32 *)(m + 0x7D0), out);
        d = sceVu0InnerProduct((f32 *)(m + 0x7F0), out);
        sceVu0CopyVector(out, (f32 *)(m + 0x800));
        sceVu0ScaleVector(v, (f32 *)(m + 0x7F0), k * (d - b));
        sceVu0AddVector(out, out, v);
        sceVu0ScaleVector(v, (f32 *)(m + 0x7D0), c - a);
        sceVu0AddVector(out, out, v);
    }
}

/* put the feet on the floor (`on`; off: the weight +0x8C4 back to 1): the weight eases 3:1
 * towards +0x44's for the actor `a`; each foot (left bone +0x8AC, right +0x89C) is dropped onto
 * the floor unless (track flags 0x100 without 4) it isn't planted (+0x64), in which case it keeps
 * the actor's height; then the legs are bent to them */
/* 0x00211190 */
void HumanModel_PlantFeet(u8 *m, s32 on, u8 *a) {
    f32 l[4] __attribute__((aligned(16)));
    f32 r[4] __attribute__((aligned(16)));
    f32 k;
    u32 dl, dr, flags;
    s32 useL, useR;

    if (on == 0) {
        AT(m, 0x8C4, f32) = 1.0f;
        return;
    }
    k = VCALL(m, 0x44, f32 (*)(u8 *, u8 *))(m, a);
    k = 0.75f * AT(m, 0x8C4, f32) + 0.25f * k;
    foot_point(m, AT(m, 0x8AC, s32), l, k);
    dl = (u8)VCALL(m, 0x64, s32 (*)(u8 *, s32, s32))(m, 1, -1);
    foot_point(m, AT(m, 0x89C, s32), r, k);
    dr = (u8)VCALL(m, 0x64, s32 (*)(u8 *, s32, s32))(m, 0, -1);
    useL = 1;
    useR = 1;
    flags = AT(AT(m, 0x6A4, u8 *), 0x18, u32);
    if (!(flags & 4) && (flags & 0x100)) {
        if (dl == 1 && AT(AT(m, 0x6A8, u8 *), 0x20, void *) != NULL) {
            dl = (u8)Motion_FootDownPrev(m, 1, 0.0f);
        }
        if (dl == 0) {
            useL = 0;
        }
        if (dr == 1 && AT(AT(m, 0x6A8, u8 *), 0x20, void *) != NULL) {
            dr = (u8)Motion_FootDownPrev(m, 0, 0.0f);
        }
        if (dr == 0) {
            useR = 0;
        }
    }
    if (useL == 1) {
        Motion_OntoFloor(m, l, a);
    } else {
        l[1] = AT(a, 0x14, f32);
    }
    if (useR == 1) {
        Motion_OntoFloor(m, r, a);
    } else {
        r[1] = AT(a, 0x14, f32);
    }
    Model_LegIK(m, r, l, AT(m, 0x804, f32));
    AT(m, 0x8C4, f32) = k;
}

/* +0x44 how much of the feet's height to keep on a slope (track flag 4; else 1): 1 - (1 -
 * ny^2) x |the actor's facing (+0x60 matrix) along the floor's downhill direction|, where ny
 * is the up part of the floor normal under it (+0x34) */
/* 0x002DD980 */
f32 Model_SlopeKeep(u8 *m, u8 *a) {
    f32 rot[4][4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 k = 1.0f, d;

    if (!(AT(AT(m, 0x6A4, u8 *), 0x18, u32) & 4)) {
        return 1.0f;
    }
    VCALL(gNavMesh, 0x2C, void (*)(NavMesh *, u32, f32 *))(gNavMesh, AT(a, 0x34, u32), n);
    if (n[1] < 1.0f) {
        k = n[1] * n[1];
        n[1] = 0.0f;
        sceVu0Normalize(n, n);
        v[2] = 1.0f;
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[3] = 0.0f;   /* unset in the original */
        sceVu0CopyMatrix(rot, (f32 (*)[4])(a + 0x60));
        sceVu0ApplyMatrix(v, rot, v);
        d = sceVu0InnerProduct(v, n);
        if (!(d <= 0.0f)) {
            d = sceVu0InnerProduct(v, n);
        } else {
            d = -sceVu0InnerProduct(v, n);
        }
        k = 1.0f - (1.0f - k) * d;
    }
    return k;
}

/* 0x002DDAB0 */
f32 Model_Vt58(void) { return 0.0f; }

/* +0x64 is foot `foot` (channel of the contact track +0x50) down `ofs` frames from now in the
 * current slot's animation (the time wrapped into it) */
/* 0x002DCDE0 */
s32 Model_FootDown(u8 *m, s32 foot, s32 ofs) {
    f32 c[4] __attribute__((aligned(16)));
    u8 *slot = AT(m, 0x6A4, u8 *);
    f32 t, len;

    c[3] = 0.0f;
    c[2] = 0.0f;
    c[1] = 0.0f;
    c[0] = 0.0f;
    if (AT(slot, 0x20, void *) == NULL) {
        return 0;
    }
    t = AT(slot, 0x0, f32) + (f32)ofs;
    len = (f32)AT(AT(AT(slot, 0x20, u8 *), 0x4, u8 *), 0xC, s32);
    while (t < 0.0f) {
        t = t + len;
    }
    while (!(t < len)) {
        t = t - len;
    }
    if (AT(slot, 0x50, void *) != NULL && AT(AT(slot, 0x50, u8 *), 0x0, void *) != NULL) {
        Track_Sample(AT(slot, 0x50, void *), c, t);
    }
    return !(c[foot] <= 0.0f);
}

/* 0x002DCF00 */
void Model_HeadPos(u8 *m) {
}

/* Moves the two floats at +0x854/+0x858 toward 0 by |step|. */
/* 0x002DCF10 */
void Model_EaseTilt(u8 *p, f32 step) {
    f32 v, a;

    if (step <= 0.0f) step = -step;
    v = F(p, 0x854, f32);
    a = v <= 0.0f ? -v : v;
    if (a <= step) {
        F(p, 0x854, f32) = 0.0f;
    } else if (v <= 0.0f) {
        F(p, 0x854, f32) += step;
    } else {
        F(p, 0x854, f32) -= step;
    }
    v = F(p, 0x858, f32);
    a = v <= 0.0f ? -v : v;
    if (a <= step) {
        F(p, 0x858, f32) = 0.0f;
    } else if (v <= 0.0f) {
        F(p, 0x858, f32) += step;
    } else {
        F(p, 0x858, f32) -= step;
    }
}

/* 0x002DD010 */
void Model_FeetUp(u8 *m) {
}

/* 0x002DD020 */
void Model_FeetReplace(u8 *m) {
}

/* 0x002DD030 */
void Model_PlantFeet(u8 *m) {
}

/* set up a two-bone IK solver: chain root / middle / end of `skel`, the two bone lengths, -1 */

#define BIND_X(m, bone) AT(AT(m, 0x4C0, u8 *) + 0x10 + (bone) * 0x70, 0x20, f32)

/* one leg's IK (solver `ik`, its end bone's target at ik + 0x20, chain end node ik + 0x54):
 * the target is the floor point `foot` less the foot's offset from the ankle (`toFoot`), at
 * the chain end's height + foot height - `height` */
static void leg_target(u8 *ik, const f32 *foot, const f32 *toFoot, f32 height) {
    AT(ik, 0x20, f32) = foot[0] - toFoot[0];
    AT(ik, 0x24, f32) = AT(AT(ik, 0x54, u8 *), 0x34, f32) + foot[1] - height;
    AT(ik, 0x28, f32) = foot[2] - toFoot[2];
}

/* bend the legs so the feet reach `right` / `left` with the body `height` lower: two-bone IK
 * from hip to ankle (right +0x890 / +0x894 / +0x898, left +0x8A0 / +0x8A4 / +0x8A8), then the
 * ankle and foot (+0x89C / +0x8AC) moved by how far the solved ankle went */
/* 0x00211530 */
void Model_LegIK(u8 *m, f32 *right, f32 *left, f32 height) {
    f32 dr[4] __attribute__((aligned(16)));
    f32 dl[4] __attribute__((aligned(16)));
    f32 er[4] __attribute__((aligned(16)));
    f32 el[4] __attribute__((aligned(16)));
    f32 *a, *b;

    IK2_Setup(m + 0x8D0, AT(m, 0x810, void *), AT(m, 0x890, s32), AT(m, 0x894, s32), AT(m, 0x898, s32),
                  BIND_X(m, AT(m, 0x894, s32)), BIND_X(m, AT(m, 0x898, s32)), -1.0f);
    IK2_Setup(m + 0x930, AT(m, 0x810, void *), AT(m, 0x8A0, s32), AT(m, 0x8A4, s32), AT(m, 0x8A8, s32),
                  BIND_X(m, AT(m, 0x8A4, s32)), BIND_X(m, AT(m, 0x8A8, s32)), -1.0f);
    a = Skel_Bone(AT(m, 0x810, void *), AT(m, 0x898, s32));
    b = Skel_Bone(AT(m, 0x810, void *), AT(m, 0x89C, s32));
    sceVu0SubVector(dr, b + 12, a + 12);
    a = Skel_Bone(AT(m, 0x810, void *), AT(m, 0x8A8, s32));
    b = Skel_Bone(AT(m, 0x810, void *), AT(m, 0x8AC, s32));
    sceVu0SubVector(dl, b + 12, a + 12);
    sceVu0CopyVector(er, (f32 *)(AT(m, 0x924, u8 *) + 0x30));
    sceVu0CopyVector(el, (f32 *)(AT(m, 0x984, u8 *) + 0x30));
    leg_target(m + 0x8D0, right, dr, height);
    leg_target(m + 0x930, left, dl, height);
    VCALL(m + 0x928, 0x8, void (*)(u8 *))(m + 0x8D0);
    VCALL(m + 0x988, 0x8, void (*)(u8 *))(m + 0x930);
    a = Skel_Bone(AT(m, 0x810, void *), AT(m, 0x898, s32));
    b = Skel_Bone(AT(m, 0x810, void *), AT(m, 0x89C, s32));
    sceVu0SubVector(dr, (f32 *)(m + 0x8F0), er);
    sceVu0AddVector(a + 12, a + 12, dr);
    sceVu0AddVector(b + 12, b + 12, dr);
    a = Skel_Bone(AT(m, 0x810, void *), AT(m, 0x8A8, s32));
    b = Skel_Bone(AT(m, 0x810, void *), AT(m, 0x8AC, s32));
    sceVu0SubVector(dl, (f32 *)(m + 0x950), el);
    sceVu0AddVector(a + 12, a + 12, dl);
    sceVu0AddVector(b + 12, b + 12, dl);
}

/* set up a two-bone IK solver { +0x0 root position, +0x10 middle, +0x20 end (the target), +0x40
 * lengths and bend, +0x4C / +0x50 / +0x54 the chain's nodes }: from the bones root / mid / end
 * of `skel` */
/* 0x001F1250 */
void IK2_Setup(u8 *ik, void *skel, s32 root, s32 mid, s32 end, f32 len1, f32 len2, f32 bend) {
    f32 *r = Skel_Bone(skel, root);
    f32 *mi = Skel_Bone(skel, mid);
    f32 *e = Skel_Bone(skel, end);

    AT(ik, 0x4C, f32 *) = r;
    AT(ik, 0x50, f32 *) = mi;
    AT(ik, 0x54, f32 *) = e;
    sceVu0CopyVector((f32 *)ik, AT(ik, 0x4C, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x10), AT(ik, 0x50, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x20), AT(ik, 0x54, f32 *) + 12);
    AT(ik, 0x40, f32) = len1;
    AT(ik, 0x44, f32) = len2;
    AT(ik, 0x48, f32) = bend;
}

extern s32 IK2_PlaceMiddle(u8 *ik, f32 *root, f32 *mid, f32 *end, f32 *pole, f32 len1, f32 len2, f32 bend);   /* 0: solved */
extern void IK2_AimBones(u8 *ik);   /* turn the chain's bones to the solution */

/* +0x8 solve: from the chain root's position (and its Z axis as the bend direction, +0x30),
 * place the middle joint for the end target; a target out of reach is pulled in to the
 * chain's full length along its direction; then the bones follow */
/* 0x001F0E50 */
void IK2_dtor(u8 *ik) {
    f32 d[4] __attribute__((aligned(16)));
    f32 len;

    sceVu0CopyVector((f32 *)ik, AT(ik, 0x4C, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x10), AT(ik, 0x50, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x30), AT(ik, 0x4C, f32 *) + 8);
    if (IK2_PlaceMiddle(ik, (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x20), (f32 *)(ik + 0x30), AT(ik, 0x40, f32),
                      AT(ik, 0x44, f32), AT(ik, 0x48, f32)) != 0) {
        sceVu0SubVector(d, (f32 *)(ik + 0x20), (f32 *)ik);
        sceVu0Normalize(d, d);
        len = AT(ik, 0x40, f32) + AT(ik, 0x44, f32);
        AT(ik, 0x20, f32) = 0.0f + AT(ik, 0x0, f32) + d[0] * len;
        AT(ik, 0x24, f32) = 0.0f + AT(ik, 0x4, f32) + d[1] * len;
        AT(ik, 0x28, f32) = 0.0f + AT(ik, 0x8, f32) + d[2] * len;
    }
    IK2_AimBones(ik);
}

/* place the middle joint `mid` of a two-bone chain (lengths len1, len2) from `root` towards
 * `end`, bent to the side of `pole` x direction (scaled by `bend`); 1 when `end` is out of
 * reach (the chain then points straight at it). (The EE's square root takes |x|.) */
/* 0x001F10C0 */
s32 IK2_PlaceMiddle(u8 *ik, f32 *root, f32 *mid, f32 *end, f32 *pole, f32 len1, f32 len2, f32 bend) {
    f32 d[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 dist, a, h, l1;
    s32 out = 0;

    sceVu0SubVector(d, end, root);
    dist = __builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(d, d)));
    sceVu0Normalize(d, d);
    if (!(dist <= len1 + len2)) {
        dist = len1 + len2;
        out = 1;
    }
    sceVu0OuterProduct(side, pole, d);
    sceVu0Normalize(side, side);
    l1 = len1 * len1;
    side[0] = side[0] * bend;
    side[1] = side[1] * bend;
    side[2] = side[2] * bend;
    a = (l1 - len2 * len2 + dist * dist) / (2.0f * dist);
    h = __builtin_sqrtf(__builtin_fabsf(l1 - a * a));
    mid[0] = root[0] + a * d[0] + h * side[0];
    mid[1] = root[1] + a * d[1] + h * side[1];
    mid[2] = root[2] + a * d[2] + h * side[2];
    return out;
}

/* a bone's matrix aimed from `from` to `to`: X along it, Z from the pole (+0x30) made square
 * to it, Y = Z x X, all unit, at `from` */
static void ik_aim(f32 (*m)[4], const f32 *from, const f32 *to, const f32 *pole) {
    sceVu0UnitMatrix(m);
    m[0][0] = to[0] - from[0];
    m[0][1] = to[1] - from[1];
    m[0][2] = to[2] - from[2];
    sceVu0CopyVector(m[2], (f32 *)pole);
    sceVu0OuterProduct(m[1], m[2], m[0]);
    sceVu0OuterProduct(m[2], m[0], m[1]);
    sceVu0Normalize(m[0], m[0]);
    sceVu0Normalize(m[1], m[1]);
    sceVu0Normalize(m[2], m[2]);
    sceVu0TransMatrix(m, m, (f32 *)from);
}

/* turn the chain's bones to the solved joints: the root (+0x4C) towards the middle, the
 * middle (+0x50) towards the end */
/* 0x001F0F40 */
void IK2_AimBones(u8 *ik) {
    ik_aim((f32 (*)[4])AT(ik, 0x4C, u8 *), (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x30));
    ik_aim((f32 (*)[4])AT(ik, 0x50, u8 *), (f32 *)(ik + 0x10), (f32 *)(ik + 0x20), (f32 *)(ik + 0x30));
}

/* turn bone `m` so its X runs `from` -> `to`, keeping its Z side, at `from` */
static inline void ik_turn(f32 (*m)[4], const f32 *from, const f32 *to) {
    m[0][0] = to[0] - from[0];
    m[0][1] = to[1] - from[1];
    m[0][2] = to[2] - from[2];
    sceVu0OuterProduct(m[1], m[2], m[0]);
    sceVu0OuterProduct(m[2], m[0], m[1]);
    sceVu0Normalize(m[0], m[0]);
    sceVu0Normalize(m[1], m[1]);
    sceVu0Normalize(m[2], m[2]);
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
    sceVu0TransMatrix(m, m, (f32 *)from);
}

/* +0xC solve keeping the bend: the bend direction (+0x30) square to the root -> target line
 * in the plane of the root's Z, the knee kept on the side it is bent to now; a target out of
 * reach is pulled in; then the root and middle bones turn to the joints and the end bone is
 * moved onto the target */
/* 0x001F0AF0 */
void IK2_Loaded(u8 *ik) {
    f32 d[4] __attribute__((aligned(16)));
    f32 z[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 a[4] __attribute__((aligned(16)));
    f32 len;

    sceVu0CopyVector((f32 *)ik, AT(ik, 0x4C, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x10), AT(ik, 0x50, f32 *) + 12);
    sceVu0CopyVector(z, AT(ik, 0x4C, f32 *) + 8);
    sceVu0SubVector(a, (f32 *)(ik + 0x20), AT(ik, 0x4C, f32 *) + 12);
    sceVu0OuterProduct(b, z, a);
    sceVu0OuterProduct((f32 *)(ik + 0x30), a, b);
    sceVu0Normalize((f32 *)(ik + 0x30), (f32 *)(ik + 0x30));
    sceVu0SubVector(a, AT(ik, 0x50, f32 *) + 12, AT(ik, 0x4C, f32 *) + 12);
    sceVu0SubVector(b, AT(ik, 0x54, f32 *) + 12, AT(ik, 0x50, f32 *) + 12);
    sceVu0OuterProduct(z, a, b);
    if (IK2_PlaceMiddle(ik, (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x20), (f32 *)(ik + 0x30), AT(ik, 0x40, f32),
                      AT(ik, 0x44, f32), sceVu0InnerProduct(AT(ik, 0x4C, f32 *) + 8, z) <= 0.0f ? 1.0f : -1.0f) != 0) {
        sceVu0SubVector(d, (f32 *)(ik + 0x20), (f32 *)ik);
        sceVu0Normalize(d, d);
        len = AT(ik, 0x40, f32) + AT(ik, 0x44, f32);
        AT(ik, 0x20, f32) = 0.0f + AT(ik, 0x0, f32) + d[0] * len;
        AT(ik, 0x24, f32) = 0.0f + AT(ik, 0x4, f32) + d[1] * len;
        AT(ik, 0x28, f32) = 0.0f + AT(ik, 0x8, f32) + d[2] * len;
    }
    ik_turn((f32 (*)[4])AT(ik, 0x4C, u8 *), (f32 *)ik, (f32 *)(ik + 0x10));
    ik_turn((f32 (*)[4])AT(ik, 0x50, u8 *), (f32 *)(ik + 0x10), (f32 *)(ik + 0x20));
    sceVu0CopyVector(AT(ik, 0x54, f32 *) + 12, (f32 *)(ik + 0x20));
}

/* ---- the three-bone IK solver (vtable IK3_vtable, derived from the two-bone one; 0x90 bytes):
 * a dog's leg - root +0x0, knee +0x10, hock +0x60, foot +0x20 (the target), the bend
 * direction +0x30; lengths +0x40 / +0x44 / +0x70, the two bends +0x48 / +0x74, how far round
 * from the knee's to the hock's direction the lower leg points +0x7C, the leg's full reach
 * +0x80; bones +0x4C / +0x50 / +0x78 / +0x54 ---- */

/* out = a + b * s */
static void ik_madd(f32 *out, const f32 *a, const f32 *b, f32 s) {
    out[0] = 0.0f + a[0] + b[0] * s;
    out[1] = 0.0f + a[1] + b[1] * s;
    out[2] = 0.0f + a[2] + b[2] * s;
}

/* out = a + b * s (a method of the solver) */
/* 0x001F17F0 */
void IK_MulAdd(u8 *ik, f32 *out, const f32 *a, const f32 *b, f32 s) {
    ik_madd(out, a, b, s);
}

/* v's component along unit `axis` (*along) and its distance from it (*off) */
/* 0x001F1430 */
void IK_AxisSplit(u8 *ik, const f32 *axis, const f32 *v, f32 *off, f32 *along) {
    *along = sceVu0InnerProduct((f32 *)axis, (f32 *)v);
    *off = __builtin_sqrtf(__builtin_fabsf(0.0f + sceVu0InnerProduct((f32 *)v, (f32 *)v) - *along * *along));
}

/* how far round from b to c (by the angles they make with a) a lies: ang(a,b) / (ang(a,b) + ang(a,c)) */
/* 0x001F1330 */
f32 IK_RoundFraction(u8 *ik, f32 *a, f32 *b, f32 *c) {
    f32 la = __builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(a, a)));
    f32 lb = __builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(b, b)));
    f32 lc = __builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(c, c)));
    f32 ab = msl_acosf(sceVu0InnerProduct(a, b) / (la * lb));
    f32 ac = msl_acosf(sceVu0InnerProduct(a, c) / (la * lc));

    return ab / (ab + ac);
}

/* the unit vector `t` of the way along the arc from unit `p` to unit `q`, `w` apart */
static void slerp(f32 *out, const f32 *p, const f32 *q, f32 w, f32 t) {
    f32 s = msl_sinf(w);
    f32 inv, sp, sq;

    if (s == 0.0f) {
        out[0] = p[0];
        out[1] = p[1];
        out[2] = p[2];
        return;
    }
    inv = 1.0f / s;
    sp = msl_sinf(w * (1.0f - t));
    sq = msl_sinf(w * t);
    out[0] = inv * (q[0] * sq + p[0] * sp);
    out[1] = inv * (q[1] * sq + p[1] * sp);
    out[2] = inv * (q[2] * sq + p[2] * sp);
}

/* the unit vector `t` of the way round from `a` to `b`, through `mid` when a and b lie on its
   either side (the arc then goes round by way of it) */
/* 0x001F14A0 */
void IK_RoundVector(u8 *ik, f32 *out, f32 *a, f32 *mid, f32 *b, f32 t) {
    f32 ca[4] __attribute__((aligned(16)));
    f32 cb[4] __attribute__((aligned(16)));
    f32 w1, w2, sum;

    sceVu0OuterProduct(ca, mid, a);
    sceVu0OuterProduct(cb, mid, b);
    if (!(sceVu0InnerProduct(ca, cb) < 0.0f)) {
        slerp(out, a, b, msl_acosf(sceVu0InnerProduct(a, b)), t);
        return;
    }
    w1 = msl_acosf(sceVu0InnerProduct(a, mid));
    w2 = msl_acosf(sceVu0InnerProduct(mid, b));
    sum = w1 + w2;
    if (t < w1 / sum) {
        slerp(out, a, mid, w1, t * sum / w1);
    } else {
        slerp(out, mid, b, w2, (t * sum - w1) / w2);
    }
}

/* place the knee `knee` and the hock `hock` of a leg from `root` to `foot`: the lower leg's
 * direction is `t` of the way round from where the knee (as a two-bone chain root / knee+hock)
 * to where the hock (as root+knee / hock) would put it, by way of the line to the root; a foot
 * out of reach (+0x80) is pulled in first (returns 1) */
/* 0x001F0600 */
s32 IK3_PlaceKneeHock(u8 *ik, f32 *root, f32 *knee, f32 *hock, f32 *foot, f32 *pole, f32 len1, f32 len2,
                  f32 len3, f32 bend1, f32 bend2, f32 t) {
    f32 up[4] __attribute__((aligned(16)));
    f32 dk[4] __attribute__((aligned(16)));
    f32 dh[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    f32 dist;
    s32 r1, r2, out = 0;

    sceVu0SubVector(dk, foot, root);
    up[0] = -dk[0];
    up[1] = -dk[1];
    up[2] = -dk[2];
    up[3] = 0.0f;
    r1 = IK2_PlaceMiddle(ik, root, knee, foot, pole, len1, len2 + len3, bend1);
    sceVu0SubVector(dk, knee, foot);
    sceVu0Normalize(dk, dk);
    r2 = IK2_PlaceMiddle(ik, root, hock, foot, pole, len1 + len2, len3, bend2);
    sceVu0SubVector(dh, hock, foot);
    sceVu0Normalize(dh, dh);
    dist = __builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(up, up)));
    sceVu0Normalize(up, up);
    if (r1 == 0 && r2 == 0) {
        IK_RoundVector(ik, dir, dk, up, dh, t);
    } else {
        sceVu0CopyVector(dir, dk);
    }
    if (!(dist <= AT(ik, 0x80, f32))) {
        out = 1;
        ik_madd(foot, root, up, -AT(ik, 0x80, f32));
    }
    ik_madd(hock, foot, dir, len3);
    IK2_PlaceMiddle(ik, root, knee, hock, pole, len1, len2, bend1);
    return out;
}

/* turn the bones to the solved joints: root -> knee, knee -> hock, hock -> foot */
/* 0x001F08C0 */
void IK3_AimBones(u8 *ik) {
    ik_aim((f32 (*)[4])AT(ik, 0x4C, u8 *), (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x30));
    ik_aim((f32 (*)[4])AT(ik, 0x50, u8 *), (f32 *)(ik + 0x10), (f32 *)(ik + 0x60), (f32 *)(ik + 0x30));
    ik_aim((f32 (*)[4])AT(ik, 0x78, u8 *), (f32 *)(ik + 0x60), (f32 *)(ik + 0x20), (f32 *)(ik + 0x30));
}

/* set up: from the bones root / knee / hock / foot of `skel`, the lengths, the bends, the reach */
/* 0x001F04B0 */
void IK3_Setup(u8 *ik, void *skel, s32 root, s32 knee, s32 hock, s32 foot, f32 len1, f32 len2, f32 len3,
                   f32 bend1, f32 bend2, f32 reach) {
    f32 *r = Skel_Bone(skel, root);
    f32 *k = Skel_Bone(skel, knee);
    f32 *h = Skel_Bone(skel, hock);
    f32 *f = Skel_Bone(skel, foot);

    AT(ik, 0x4C, f32 *) = r;
    AT(ik, 0x50, f32 *) = k;
    AT(ik, 0x78, f32 *) = h;
    AT(ik, 0x54, f32 *) = f;
    sceVu0CopyVector((f32 *)ik, AT(ik, 0x4C, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x10), AT(ik, 0x50, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x60), AT(ik, 0x78, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x20), AT(ik, 0x54, f32 *) + 12);
    AT(ik, 0x40, f32) = len1;
    AT(ik, 0x44, f32) = len2;
    AT(ik, 0x70, f32) = len3;
    AT(ik, 0x48, f32) = bend1;
    AT(ik, 0x74, f32) = bend2;
    AT(ik, 0x7C, f32) = 0.5f;
    AT(ik, 0x80, f32) = reach;
}

/* +0x8 solve with the current lower-leg blend */
/* 0x001F0450 */
void IK3_dtor(u8 *ik) {
    IK3_PlaceKneeHock(ik, (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x60), (f32 *)(ik + 0x20), (f32 *)(ik + 0x30),
                  AT(ik, 0x40, f32), AT(ik, 0x44, f32), AT(ik, 0x70, f32), AT(ik, 0x48, f32), AT(ik, 0x74, f32),
                  AT(ik, 0x7C, f32));
    IK3_AimBones(ik);
}

/* +0xC solve keeping the leg's pose: the bend direction is the root's Z; the blend +0x7C is
 * taken from the current pose (where the hock lies between the knee's and the hock's
 * two-bone solutions, seen from the foot); the target is solved in the plane through the
 * root square to the bend direction, with the hock-to-foot distance from that axis as the
 * lower leg, and moved back out along it; then the bones follow and the foot bone goes onto
 * the target */
/* 0x001F0250 */
void IK3_Loaded(u8 *ik) {
    f32 kneeAt[4] __attribute__((aligned(16)));
    f32 hockAt[4] __attribute__((aligned(16)));
    f32 flat[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 *pole = (f32 *)(ik + 0x30);
    f32 *footAt = AT(ik, 0x54, f32 *) + 12;
    f32 off, along;

    sceVu0CopyVector((f32 *)ik, AT(ik, 0x4C, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x10), AT(ik, 0x50, f32 *) + 12);
    sceVu0CopyVector((f32 *)(ik + 0x60), AT(ik, 0x78, f32 *) + 12);
    sceVu0CopyVector(pole, AT(ik, 0x4C, f32 *) + 8);
    sceVu0SubVector(v, footAt, AT(ik, 0x78, f32 *) + 12);
    IK_AxisSplit(ik, pole, v, &off, &along);
    IK_MulAdd(ik, flat, (f32 *)(ik + 0x20), pole, -along);
    IK2_PlaceMiddle(ik, (f32 *)ik, kneeAt, footAt, pole, AT(ik, 0x40, f32), AT(ik, 0x44, f32) + AT(ik, 0x70, f32),
                  AT(ik, 0x48, f32));
    IK2_PlaceMiddle(ik, (f32 *)ik, hockAt, footAt, pole, AT(ik, 0x40, f32) + AT(ik, 0x44, f32), AT(ik, 0x70, f32),
                  AT(ik, 0x74, f32));
    sceVu0SubVector(v, AT(ik, 0x78, f32 *) + 12, footAt);
    sceVu0SubVector(kneeAt, kneeAt, footAt);
    sceVu0SubVector(hockAt, hockAt, footAt);
    AT(ik, 0x7C, f32) = IK_RoundFraction(ik, v, kneeAt, hockAt);
    IK3_PlaceKneeHock(ik, (f32 *)ik, (f32 *)(ik + 0x10), (f32 *)(ik + 0x60), flat, pole, AT(ik, 0x40, f32),
                  AT(ik, 0x44, f32), off, AT(ik, 0x48, f32), AT(ik, 0x74, f32), AT(ik, 0x7C, f32));
    IK_MulAdd(ik, (f32 *)(ik + 0x20), flat, pole, along);
    IK3_AimBones(ik);
    sceVu0CopyVector(footAt, (f32 *)(ik + 0x20));
}

/* A spring system: point masses (first +0x18, next +0x2C) and the links between them (first
 * +0x30, next +0x28), each with its vtable at +0x30 of itself. */

/* begin a frame: each point (+0xC) with the system's +0x14 */
/* 0x002EE8A0 */
void SpringSet_Begin(u8 *s) {
    u8 *p;

    for (p = AT(s, 0x18, u8 *); p != NULL; p = AT(p, 0x2C, u8 *)) {
        VCALL(p + 0x30, 0xC, void (*)(u8 *, void *))(p, AT(s, 0x14, void *));
    }
}

/* one step: each link (+0x10) */
/* 0x002EE900 */
void SpringSet_Step(u8 *s) {
    u8 *l;

    for (l = AT(s, 0x30, u8 *); l != NULL; l = AT(l, 0x28, u8 *)) {
        VCALL(l + 0x30, 0x10, void (*)(u8 *, u8 *))(l, s);
    }
}

/* finish the frame: each link (+0x14) */
/* 0x002EE840 */
void SpringSet_Finish(u8 *s) {
    u8 *l;

    for (l = AT(s, 0x30, u8 *); l != NULL; l = AT(l, 0x28, u8 *)) {
        VCALL(l + 0x30, 0x14, void (*)(u8 *, u8 *))(l, s);
    }
}

/* +0xC of a point fixed to a bone: its position (+0x0) is the bone (+0x28) of the model's
 * skeleton applied to its offset (+0x10) */
/* 0x002EE570 */
void BonePoint_Loaded(u8 *p, u8 *model) {
    f32 m[4][4] __attribute__((aligned(16)));

    sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(AT(model, 0x810, void *), AT(p, 0x28, s32)));
    sceVu0ApplyMatrix((f32 *)p, m, (f32 *)(p + 0x10));
}

/* +0x10 step of a hanging point (hair / cloth) { +0x0 position, +0x10 velocity, +0x20 anchored to
 * a bone (+0x24) else to the point +0x2C, +0x40 length, +0x44 the bone it hangs along, +0x48
 * its side axis, +0x4C the cone angle } in system `s` (+0x4 gravity, +0x10 damping): pulled
 * along the hanging bone's X axis, damped, moved; then kept at its length from the anchor,
 * within the cone around that axis and off the back of the plane of the axis and the side
 * axis, its velocity following the corrections */
/* 0x00315F00 */
void HairPoint2_Frame(u8 *l, u8 *s) {
    f32 anchor[4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 x[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 *a = Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(l, 0x24, s32));
    f32 *b = Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(l, 0x44, s32));
    f32 ang, sa, sl, sr, inv, k1, k2;

    if (AT(l, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, a + 12);
    } else {
        sceVu0CopyVector(anchor, AT(l, 0x2C, f32 *));
    }
    sceVu0CopyVector(prev, (f32 *)l);
    sceVu0ScaleVector(d, b, AT(s, 0x4, f32));
    sceVu0AddVector((f32 *)(l + 0x10), (f32 *)(l + 0x10), d);
    sceVu0ScaleVector((f32 *)(l + 0x10), (f32 *)(l + 0x10), AT(s, 0x10, f32));
    sceVu0AddVector((f32 *)l, (f32 *)l, (f32 *)(l + 0x10));
    sceVu0SubVector(d, (f32 *)l, anchor);
    sceVu0Normalize(d, d);
    ang = msl_acosf(sceVu0InnerProduct(d, b));
    if (!(ang <= AT(l, 0x4C, f32))) {
        sa = msl_sinf(ang);
        if (!(sa <= 0.0f)) {
            sl = msl_sinf(AT(l, 0x4C, f32));
            sr = msl_sinf(ang - AT(l, 0x4C, f32));
            inv = 1.0f / sa;
            d[0] = inv * (sr * b[0] + sl * d[0]);
            d[1] = inv * (sr * b[1] + sl * d[1]);
            d[2] = inv * (sr * b[2] + sl * d[2]);
        }
    }
    sceVu0ScaleVector(d, d, AT(l, 0x40, f32));
    sceVu0AddVector((f32 *)l, anchor, d);
    sceVu0SubVector((f32 *)(l + 0x10), (f32 *)l, prev);
    sceVu0CopyVector(x, b);
    sceVu0ApplyMatrix(side, (f32 (*)[4])b, AT(l, 0x48, f32 *));
    k1 = sceVu0InnerProduct(d, x);
    k2 = sceVu0InnerProduct(d, side);
    sceVu0CopyVector(prev, (f32 *)l);
    if (k2 < 0.0f) {
        k2 = 0.0f;
    }
    sceVu0ScaleVector(x, x, k1);
    sceVu0ScaleVector(side, side, k2);
    sceVu0AddVector(d, x, side);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(l, 0x40, f32));
    sceVu0AddVector((f32 *)l, anchor, d);
    sceVu0SubVector(d, (f32 *)l, prev);
    sceVu0AddVector((f32 *)(l + 0x10), (f32 *)(l + 0x10), d);
}

/* +0x10 of her six hanging parts (+0xAA0, vtable HangingPart_vtable): pulled along bone 0x1F's Z (by
   +0x44), the set's force, pushed off the colliders, kept above the set's floor (+0x1C, when
   +0x20), damped, held at its length from the anchor */
/* 0x003168F0 */
void HangingPart_Step(u8 *p, u8 *set) {
    f32 g[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 *vel = (f32 *)(p + 0x10);
    u8 *c;

    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0CopyVector(g, Skel_Bone(AT(AT(set, 0x14, u8 *), 0x810, u8 *), 0x1F) + 8);
    sceVu0ScaleVector(g, g, AT(p, 0x44, f32));
    sceVu0AddVector(vel, vel, g);
    sceVu0SubVector(vel, vel, (f32 *)set);
    for (c = AT(set, 0x18, u8 *); c != NULL; c = AT(c, 0x2C, u8 *)) {
        (*(void (**)(u8 *, f32 *, u8 *, f32))(AT(c, 0x30, u8 *) + 8))(c, d, p, 1.0f);
        sceVu0AddVector(vel, vel, d);
    }
    if (AT(set, 0x20, u8) != 0 && AT(p, 0x4, f32) < AT(set, 0x1C, f32)) {
        vel[1] += AT(set, 0x1C, f32) - AT(p, 0x4, f32);
    }
    sceVu0ScaleVector(vel, vel, AT(set, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, vel);
    Part_Anchor(p, set, at);
    Part_Hold(p, at, prev);
}

/* +0x10 of his 0x50 parts (vtable Part50_vtable): drawn toward where its bone points (its length
   out along the bone's X axis, by +0x44), the set's force, damped, at its length */
/* 0x0031EA10 */
void Part50_Step(u8 *p, u8 *set) {
    f32 prev[4] __attribute__((aligned(16)));
    f32 t[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 *vel = (f32 *)(p + 0x10);
    f32 *mtx;

    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0SubVector(vel, vel, (f32 *)set);
    mtx = Skel_Bone(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(p, 0x24, s32));
    t[0] = AT(p, 0x40, f32);
    t[3] = 1.0f;
    t[1] = 0.0f;
    t[2] = 0.0f;
    sceVu0ApplyMatrix(t, (f32 (*)[4])mtx, t);
    sceVu0SubVector(t, t, (f32 *)p);
    sceVu0ScaleVector(t, t, AT(p, 0x44, f32));
    sceVu0AddVector(vel, vel, t);
    sceVu0ScaleVector(vel, vel, AT(set, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, vel);
    Part_Anchor(p, set, at);
    Part_Hold(p, at, prev);
}

/* ---- Riccardo's parts ---- */

/* +0x10 of his 0x60 parts (vtable Part60_vtable): follow the anchor (0.6), the set's force, the
   colliders (strength 1), held about 1.9 from the part it's tied to (+0x48, by +0x44), damped,
   at its length */
/* 0x0031EB70 */
void Part60_Step(u8 *p, u8 *set) {
    f32 at[4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 *vel = (f32 *)(p + 0x10);
    u8 *c;

    Part_Anchor(p, set, at);
    sceVu0SubVector(d, at, (f32 *)(p + 0x50));
    sceVu0ScaleVector(d, d, 0x1.333334p-1f);   /* 0.6 */
    sceVu0AddVector((f32 *)p, (f32 *)p, d);
    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0SubVector(vel, vel, (f32 *)set);
    for (c = AT(set, 0x18, u8 *); c != NULL; c = AT(c, 0x2C, u8 *)) {
        (*(void (**)(u8 *, f32 *, u8 *, f32))(AT(c, 0x30, u8 *) + 8))(c, d, p, 1.0f);
        sceVu0AddVector(vel, vel, d);
    }
    if (AT(p, 0x48, f32 *) != NULL) {
        f32 len;

        sceVu0SubVector(d, AT(p, 0x48, f32 *), (f32 *)p);
        len = __builtin_sqrtf(sceVu0InnerProduct(d, d));
        sceVu0ScaleVector(d, d, AT(p, 0x44, f32) * (len - 0x1.e66666p+0f /* 1.9 */) / len);
        sceVu0AddVector(vel, vel, d);
    }
    sceVu0ScaleVector(vel, vel, AT(set, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, vel);
    Part_Hold(p, at, prev);
    sceVu0CopyVector((f32 *)(p + 0x50), at);
}

/* +0x10 step of a point hanging from a bone (+0x24) { +0x0 position, +0x10 velocity, +0x40
 * length } in system `s` (+0x0 force, subtracted; +0x10 damping): moved, then kept at its
 * length from the bone, on the front side of the bone's Y axis and within 30 degrees of its X
 * axis; its velocity is how far it went */
/* 0x002EE970 */
void BoneHangPoint_Frame(u8 *p, u8 *s) {
    static const union { u32 u; f32 f; } kCone = {0x3F060A92};   /* 30 degrees */
    f32 m[4][4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 anchor[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 k, ang, sl, sr, inv;

    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), (f32 *)s);
    sceVu0ScaleVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), AT(s, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, (f32 *)(p + 0x10));
    sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32)));
    sceVu0CopyVector(anchor, m[3]);
    sceVu0SubVector(d, (f32 *)p, anchor);
    k = sceVu0InnerProduct(d, m[1]);
    if (k < 0.0f) {
        sceVu0ScaleVector(v, m[1], k);
        sceVu0SubVector(d, d, v);
    }
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    ang = msl_acosf(sceVu0InnerProduct(d, m[0]));
    if (!(ang <= kCone.f)) {
        sl = msl_sinf(kCone.f);
        sr = msl_sinf(ang - kCone.f);
        inv = 1.0f / msl_sinf(ang);
        d[0] = inv * (sr * m[0][0] + sl * d[0]);
        d[1] = inv * (sr * m[0][1] + sl * d[1]);
        d[2] = inv * (sr * m[0][2] + sl * d[2]);
    }
    sceVu0AddVector((f32 *)p, anchor, d);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)p, prev);
}

/* +0x10 step of a free hanging point { +0x0 position, +0x10 velocity, +0x20 anchored to a bone
 * (+0x24) else the point +0x2C, +0x40 length } in system `s` (+0x0 force, +0x10 damping, +0x18
 * the colliders, +0x20 / +0x1C a floor height): pushed by each collider (+0x8, strength 1) and
 * up off the floor, damped, moved, then kept at its length from the anchor; its velocity is
 * how far it went */
/* 0x002ECE50 */
void HangPoint_Frame(u8 *p, u8 *s) {
    f32 prev[4] __attribute__((aligned(16)));
    f32 anchor[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u8 *c;

    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), (f32 *)s);
    for (c = AT(s, 0x18, u8 *); c != NULL; c = AT(c, 0x2C, u8 *)) {
        VCALL(c + 0x30, 0x8, void (*)(u8 *, f32 *, u8 *, f32))(c, d, p, 1.0f);
        sceVu0AddVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), d);
    }
    if (AT(s, 0x20, u8) != 0 && AT(p, 0x4, f32) < AT(s, 0x1C, f32)) {
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + (AT(s, 0x1C, f32) - AT(p, 0x4, f32));
    }
    sceVu0ScaleVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), AT(s, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, (f32 *)(p + 0x10));
    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32)) + 12);
    } else {
        sceVu0CopyVector(anchor, AT(p, 0x2C, f32 *));
    }
    sceVu0SubVector(d, (f32 *)p, anchor);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)p, anchor, d);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)p, prev);
}

/* ---- the capsule collider (vtable Capsule_vtable, +0x30 in a 0x70 part): ends +0 / +0x40 in the
   world, +0x10 / +0x50 in their bones' (+0x28 / +0x60) space, radius +0x20 (1 / it +0x24) ---- */

/* +0x8: the push on point `pt` (into `out`), `k` times its depth: off the segment when it's
   alongside, else off the nearer end (not normalized there, as the original) */
/* 0x002EE220 */
void Capsule_Push(u8 *cap, f32 *out, const f32 *pt, f32 k) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 d1[4] __attribute__((aligned(16)));
    f32 t0, t1, lo, hi, tp, l2;

    sceVu0SubVector(d, (f32 *)(cap + 0x40), (f32 *)cap);
    sceVu0Normalize(axis, d);
    t0 = sceVu0InnerProduct(axis, (f32 *)cap);
    t1 = sceVu0InnerProduct(axis, (f32 *)(cap + 0x40));
    if (t0 <= t1) {
        lo = t0;
        hi = t1;
    } else {
        lo = t1;
        hi = t0;
    }
    tp = sceVu0InnerProduct(axis, (f32 *)pt);
    if (!(tp < lo) && tp <= hi) {
        f32 s;

        sceVu0SubVector(d, (f32 *)pt, (f32 *)cap);
        s = sceVu0InnerProduct(axis, d);
        d[0] = d[0] - s * axis[0];
        d[1] = d[1] - s * axis[1];
        d[2] = d[2] - s * axis[2];
        l2 = sceVu0InnerProduct(d, d);
        if (l2 < AT(cap, 0x20, f32) * AT(cap, 0x20, f32) && !(l2 <= 0.0f)) {
            sceVu0Normalize(d, d);
            sceVu0ScaleVector(out, d, (1.0f - __builtin_sqrtf(l2) * AT(cap, 0x24, f32)) * k);
            return;
        }
    }
    sceVu0SubVector(d, (f32 *)pt, (f32 *)cap);
    l2 = sceVu0InnerProduct(d, d);
    sceVu0SubVector(d1, (f32 *)pt, (f32 *)(cap + 0x40));
    {
        f32 l1 = sceVu0InnerProduct(d1, d1);

        if (!(l2 <= l1)) {
            l2 = l1;
            sceVu0CopyVector(d, d1);
        }
    }
    if (l2 < AT(cap, 0x20, f32) * AT(cap, 0x20, f32) && !(l2 <= 0.0f)) {
        sceVu0ScaleVector(out, d, (1.0f - __builtin_sqrtf(l2) * AT(cap, 0x24, f32)) * k);
        return;
    }
    AT(out, 0x0, s32) = 0;
    AT(out, 0x4, s32) = 0;
    AT(out, 0x8, s32) = 0;
    AT(out, 0xC, s32) = 0;
}

/* the capsule's +0xC: its ends from their bones */
/* 0x002EE4B0 */
void Capsule_Update(u8 *cap, u8 *m) {
    f32 mtx[4][4] __attribute__((aligned(16)));

    sceVu0CopyMatrix(mtx, (f32 (*)[4])Skel_Bone(AT(m, 0x810, u8 *), AT(cap, 0x28, s32)));
    sceVu0ApplyMatrix((f32 *)cap, mtx, (f32 *)(cap + 0x10));
    sceVu0CopyMatrix(mtx, (f32 (*)[4])Skel_Bone(AT(m, 0x810, u8 *), AT(cap, 0x60, s32)));
    sceVu0ApplyMatrix((f32 *)(cap + 0x40), mtx, (f32 *)(cap + 0x50));
}

/* a capsule collider between bones `b1` and `b2` (ends p1, p2 in their bones' space, radius r) */
/* 0x002EE530 */
void Capsule_Set(u8 *cap, s32 b1, s32 b2, f32 x1, f32 y1, f32 z1, f32 r, f32 x2, f32 y2, f32 z2) {
    AT(cap, 0x10, f32) = x1;
    AT(cap, 0x14, f32) = y1;
    AT(cap, 0x18, f32) = z1;
    AT(cap, 0x1C, f32) = 1.0f;
    AT(cap, 0x20, f32) = r;
    AT(cap, 0x24, f32) = 1.0f / r;
    AT(cap, 0x28, s32) = b1;
    AT(cap, 0x50, f32) = x2;
    AT(cap, 0x54, f32) = y2;
    AT(cap, 0x58, f32) = z2;
    AT(cap, 0x5C, f32) = 1.0f;
    AT(cap, 0x60, s32) = b2;
}

/* +0x10 step of a costume hanging point (the class at 0x475CA0) { +0x0 position, +0x10
 * velocity, +0x20 anchored to a bone (+0x24) else the point +0x2C, +0x40 length, +0x44 rigid,
 * +0x48 swing limit }. A rigid one continues the line from its parent's anchor through its
 * parent (+0x2C). Otherwise as HangPoint_Frame (without the floor), then turned back within its
 * swing limit of the bone's X axis. The limit is tested on the length-scaled offset, and the
 * offset is scaled by the length again after (both as in the original; the lengths are short) */
/* 0x00338C20 */
void CostumeHangPoint_Frame(u8 *p, u8 *s) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 anchor[4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 ang, sl, sr, inv;
    u8 *c;

    if (AT(p, 0x44, u8) != 0) {
        u8 *q = AT(p, 0x2C, u8 *);

        if (AT(q, 0x20, u8) != 0) {
            sceVu0CopyVector(anchor, Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(q, 0x24, s32)) + 12);
        } else {
            sceVu0CopyVector(anchor, AT(q, 0x2C, f32 *));
        }
        sceVu0SubVector(d, (f32 *)q, anchor);
        sceVu0Normalize(d, d);
        sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
        sceVu0AddVector((f32 *)p, (f32 *)q, d);
        return;
    }
    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), (f32 *)s);
    for (c = AT(s, 0x18, u8 *); c != NULL; c = AT(c, 0x2C, u8 *)) {
        VCALL(c + 0x30, 0x8, void (*)(u8 *, f32 *, u8 *, f32))(c, d, p, 1.0f);
        sceVu0AddVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), d);
    }
    sceVu0ScaleVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), AT(s, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, (f32 *)(p + 0x10));
    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32)) + 12);
    } else {
        sceVu0CopyVector(anchor, AT(p, 0x2C, f32 *));
    }
    sceVu0SubVector(d, (f32 *)p, anchor);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)p, anchor, d);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)p, prev);
    sceVu0CopyVector(axis, Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32)));
    ang = msl_acosf(sceVu0InnerProduct(d, axis));
    if (!(ang <= AT(p, 0x48, f32))) {
        f32 sa = msl_sinf(ang);

        if (!(sa <= 0.0f)) {
            sl = msl_sinf(AT(p, 0x48, f32));
            sr = msl_sinf(ang - AT(p, 0x48, f32));
            inv = 1.0f / sa;
            d[0] = inv * (sr * axis[0] + sl * d[0]);
            d[1] = inv * (sr * axis[1] + sl * d[1]);
            d[2] = inv * (sr * axis[2] + sl * d[2]);
        }
    }
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)p, anchor, d);
}

/* +0x10 step of a hanging point of the class at 0x479F90: as CostumeHangPoint_Frame, but also blown
 * by the system's wind (+0x0, a point in the space of bone +0xC, a float): pushed 0.2 along
 * the direction from the point's root bone (its own +0x24 if anchored to a bone, else its
 * parent's, or the grandparent's) to the wind point */
/* 0x00369DF0 */
void WindHangPoint_Frame(u8 *p, u8 *s) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 anchor[4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 root[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 ang, sl, sr, inv;
    s32 bone;
    u8 *c;

    if (AT(p, 0x44, u8) != 0) {
        u8 *q = AT(p, 0x2C, u8 *);

        if (AT(q, 0x20, u8) != 0) {
            sceVu0CopyVector(anchor, Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(q, 0x24, s32)) + 12);
        } else {
            sceVu0CopyVector(anchor, AT(q, 0x2C, f32 *));
        }
        sceVu0SubVector(d, (f32 *)q, anchor);
        sceVu0Normalize(d, d);
        sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
        sceVu0AddVector((f32 *)p, (f32 *)q, d);
        return;
    }
    sceVu0CopyVector(prev, (f32 *)p);
    d[0] = AT(s, 0x0, f32);
    d[1] = AT(s, 0x4, f32);
    d[2] = AT(s, 0x8, f32);
    d[3] = 1.0f;
    if (AT(p, 0x20, u8) != 0) {
        bone = AT(p, 0x24, s32);
    } else if (AT(AT(p, 0x2C, u8 *), 0x20, u8) != 0) {
        bone = AT(AT(p, 0x2C, u8 *), 0x24, s32);
    } else {
        bone = AT(AT(AT(p, 0x2C, u8 *), 0x2C, u8 *), 0x24, s32);
    }
    sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), (s32)AT(s, 0xC, f32)));
    sceVu0CopyVector(root, Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), bone) + 12);
    sceVu0ApplyMatrix(d, m, d);
    sceVu0SubVector(d, d, root);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, -0x1.99999ap-3f);   /* -0.2 */
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), d);
    for (c = AT(s, 0x18, u8 *); c != NULL; c = AT(c, 0x2C, u8 *)) {
        VCALL(c + 0x30, 0x8, void (*)(u8 *, f32 *, u8 *, f32))(c, d, p, 1.0f);
        sceVu0AddVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), d);
    }
    sceVu0ScaleVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), AT(s, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, (f32 *)(p + 0x10));
    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32)) + 12);
    } else {
        sceVu0CopyVector(anchor, AT(p, 0x2C, f32 *));
    }
    sceVu0SubVector(d, (f32 *)p, anchor);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)p, anchor, d);
    sceVu0CopyVector(axis, Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32)));
    ang = msl_acosf(sceVu0InnerProduct(d, axis));
    if (!(ang <= AT(p, 0x48, f32))) {
        f32 sa = msl_sinf(ang);

        if (!(sa <= 0.0f)) {
            sl = msl_sinf(AT(p, 0x48, f32));
            sr = msl_sinf(ang - AT(p, 0x48, f32));
            inv = 1.0f / sa;
            d[0] = inv * (sr * axis[0] + sl * d[0]);
            d[1] = inv * (sr * axis[1] + sl * d[1]);
            d[2] = inv * (sr * axis[2] + sl * d[2]);
        }
    }
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)p, anchor, d);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)p, prev);
}

void SpringPartBase_AdjustBone(u8 *p, u8 *s);

/* +0x14 of a hanging point of the class at 0x479F90: unless +0x4C, first as SpringPartBase_AdjustBone;
 * then its bone (+0x24) aimed at it - X from the anchor to the point, keeping the Z axis of
 * the anchor's bone, made square, at the anchor */
/* 0x0036A1C0 */
void WindHangPoint_AdjustBone(u8 *p, u8 *s) {
    f32 *a;
    f32 anchor[4] __attribute__((aligned(16)));
    f32 x[4] __attribute__((aligned(16)));
    f32 z[4] __attribute__((aligned(16)));

    if (AT(p, 0x4C, u8) == 0) {
        SpringPartBase_AdjustBone(p, s);
    }
    a = Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32));
    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, a + 12);
        sceVu0SubVector(x, (f32 *)p, anchor);
        sceVu0CopyVector(z, a + 8);
    } else {
        sceVu0CopyVector(anchor, AT(p, 0x2C, f32 *));
        sceVu0SubVector(x, (f32 *)p, anchor);
        sceVu0CopyVector(z, Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(AT(p, 0x2C, u8 *), 0x24, s32)) + 8);
    }
    sceVu0CopyVector(a, x);
    sceVu0OuterProduct(a + 4, z, a);
    sceVu0OuterProduct(a + 8, a, a + 4);
    sceVu0Normalize(a, a);
    sceVu0Normalize(a + 4, a + 4);
    sceVu0Normalize(a + 8, a + 8);
    sceVu0CopyVector(a + 12, anchor);
}

/* +0x8 a sphere collider { +0x0 centre, +0x20 radius, +0x24 falloff }: the push `out` on the
 * point `at` (strength `k`): inside the sphere its offset from the centre x (1 - distance x
 * falloff) x k, else none */
/* 0x002EE5C0 */
void BonePoint_dtor(u8 *c, f32 *out, f32 *at, f32 k) {
    f32 d[4] __attribute__((aligned(16)));
    f32 dd = 0.0f;

    sceVu0SubVector(d, at, (f32 *)c);
    dd = sceVu0InnerProduct(d, d);
    if (dd < AT(c, 0x20, f32) * AT(c, 0x20, f32) && !(dd <= 0.0f)) {
        sceVu0ScaleVector(out, d, (1.0f - __builtin_sqrtf(__builtin_fabsf(dd)) * AT(c, 0x24, f32)) * k);
        return;
    }
    out[3] = 0.0f;
    out[2] = 0.0f;
    out[1] = 0.0f;
    out[0] = 0.0f;
}

/* +0x10 step of a point sprung to a bone (+0x24) { +0x0 position, +0x10 velocity, +0x40
 * stiffness } in system `s` (+0x10 damping): pulled to the bone, damped, moved, then flattened
 * into the bone's X / Y plane and kept within -0.2..0.2 along X and -0.2..0.3 along Y of it */
/* 0x002EEDA0 */
void SprungPoint_Frame(u8 *p, u8 *s) {
    static const union { u32 u; f32 f; } k03 = {0x3E99999A}, k02 = {0x3E4CCCCD}, kn02 = {0xBE4CCCCD};
    f32 *b = Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32));
    f32 prev[4] __attribute__((aligned(16)));
    f32 o[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 x, y;

    sceVu0CopyVector(prev, (f32 *)p);
    sceVu0CopyVector(o, b + 12);
    sceVu0SubVector(d, o, (f32 *)p);
    sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
    sceVu0AddVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), d);
    sceVu0ScaleVector((f32 *)(p + 0x10), (f32 *)(p + 0x10), AT(s, 0x10, f32));
    sceVu0AddVector((f32 *)p, (f32 *)p, (f32 *)(p + 0x10));
    sceVu0SubVector(d, (f32 *)p, b + 12);
    sceVu0ScaleVector(v, b + 8, sceVu0InnerProduct(d, b + 8));
    sceVu0SubVector(d, d, v);
    x = sceVu0InnerProduct(d, b);
    y = sceVu0InnerProduct(d, b + 4);
    if (!(y <= k03.f)) {
        y = k03.f;
    }
    if (y < kn02.f) {
        y = kn02.f;
    }
    if (!(x <= k02.f)) {
        x = k02.f;
    }
    if (x < kn02.f) {
        x = kn02.f;
    }
    sceVu0ScaleVector(d, b, x);
    sceVu0ScaleVector(v, b + 4, y);
    sceVu0AddVector(d, d, v);
    sceVu0AddVector((f32 *)p, b + 12, d);
    sceVu0SubVector((f32 *)(p + 0x10), (f32 *)p, prev);
}

/* ---- Daniella's hair point (vtable HairPoint_vtable, +0x30 in a 0x70 part): the position +0, its
   velocity +0x10, anchored (+0x20) to bone +0x24 or the point +0x2C, its length +0x40, the
   partner point in the other strand +0x44 (side +0x48), the anchor last frame +0x50, its
   stiffness +0x60 ---- */

/* +0x14: its bone's matrix from the point: X toward the point, Y toward the partner, at the
   anchor */
/* 0x002EEF90 */
void HairPoint_Pose(u8 *pt, u8 *set) {
    f32 *mtx = Skel_Bone(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(pt, 0x24, s32));
    f32 at[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));

    if (AT(pt, 0x20, u8) != 0) {
        sceVu0CopyVector(at, mtx + 12);
    } else {
        sceVu0CopyVector(at, AT(pt, 0x2C, f32 *));
    }
    sceVu0SubVector(side, AT(pt, 0x44, f32 *), (f32 *)pt);
    sceVu0ScaleVector(side, side, AT(pt, 0x48, f32));
    sceVu0Normalize(side, side);
    sceVu0SubVector(mtx, (f32 *)pt, at);
    sceVu0CopyVector(mtx + 4, side);
    sceVu0OuterProduct(mtx + 8, mtx, mtx + 4);
    sceVu0OuterProduct(mtx + 4, mtx + 8, mtx);
    sceVu0Normalize(mtx, mtx);
    sceVu0Normalize(mtx + 4, mtx + 4);
    sceVu0Normalize(mtx + 8, mtx + 8);
    sceVu0CopyVector(mtx + 12, at);
}

/* +0x10: a step: follow the anchor by the stiffness, the set's force, pushed off the colliders
   (3 times their depth), kept about 0.438 from its partner, damped, held at its length from
   the anchor */
/* 0x002EF0A0 */
void HairPoint_Step(u8 *pt, u8 *set) {
    f32 *mtx = Skel_Bone(AT(AT(set, 0x14, u8 *), 0x810, u8 *), AT(pt, 0x24, s32));
    f32 at[4] __attribute__((aligned(16)));
    f32 prev[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 *vel = (f32 *)(pt + 0x10);
    u8 *c;
    f32 len;

    if (AT(pt, 0x20, u8) != 0) {
        sceVu0CopyVector(at, mtx + 12);
    } else {
        sceVu0CopyVector(at, AT(pt, 0x2C, f32 *));
    }
    sceVu0CopyVector(prev, (f32 *)pt);
    sceVu0SubVector(d, at, (f32 *)(pt + 0x50));
    sceVu0ScaleVector(d, d, AT(pt, 0x60, f32));
    sceVu0AddVector((f32 *)pt, (f32 *)pt, d);
    sceVu0SubVector(vel, vel, (f32 *)set);
    for (c = AT(set, 0x18, u8 *); c != NULL; c = AT(c, 0x2C, u8 *)) {
        (*(void (**)(u8 *, f32 *, u8 *, f32))(AT(c, 0x30, u8 *) + 8))(c, d, pt, 3.0f);
        sceVu0AddVector(vel, vel, d);
    }
    sceVu0SubVector(d, (f32 *)pt, AT(pt, 0x44, f32 *));
    len = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, 0x1.99999ap-1f * -(len - 0x1.c068dcp-2f));   /* 0.8, 0.4379 */
    sceVu0AddVector(vel, vel, d);
    sceVu0ScaleVector(vel, vel, AT(set, 0x10, f32));
    sceVu0AddVector((f32 *)pt, (f32 *)pt, vel);
    sceVu0SubVector(d, (f32 *)pt, at);
    sceVu0Normalize(d, d);
    sceVu0ScaleVector(d, d, AT(pt, 0x40, f32));
    sceVu0AddVector((f32 *)pt, at, d);
    sceVu0SubVector(vel, (f32 *)pt, prev);
    sceVu0CopyVector((f32 *)(pt + 0x50), at);
}

/* 0x002F6DC0 */
void *EventHumanModel_Textures(void) {
    return D_0045E4C0;
}

/* 0x002F6DD0 */
void *EventHumanModel_Textures2(void) {
    return str_O_FIN_FIN_200_TEX;
}

/* +0x14 of a hanging point: its anchor bone (+0x24) is aimed at it - X from the anchor to the
 * point, Y the hanging bone's (+0x44) side axis (+0x48), made square, at the anchor */
/* 0x00315E00 */
void HairPoint2_AdjustBone(u8 *p, u8 *s) {
    f32 *a = Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32));
    f32 *b = Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x44, s32));
    f32 anchor[4] __attribute__((aligned(16)));

    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, a + 12);
    } else {
        sceVu0CopyVector(anchor, AT(p, 0x2C, f32 *));
    }
    sceVu0SubVector(a, (f32 *)p, anchor);
    sceVu0ApplyMatrix(a + 4, (f32 (*)[4])b, AT(p, 0x48, f32 *));
    sceVu0OuterProduct(a + 8, a, a + 4);
    sceVu0OuterProduct(a + 4, a + 8, a);
    sceVu0Normalize(a, a);
    sceVu0Normalize(a + 4, a + 4);
    sceVu0Normalize(a + 8, a + 8);
    sceVu0CopyVector(a + 12, anchor);
}

/* +0x14 of a chained point: its bone (+0x24) is aimed at it - X from the anchor (the bone's own
 * position, or the point it hangs from, +0x2C) to the point, keeping the Y axis of the anchor's
 * bone, made square, at the anchor */
/* 0x002EE710 */
void SpringPartBase_AdjustBone(u8 *p, u8 *s) {
    f32 *a = Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32));
    f32 anchor[4] __attribute__((aligned(16)));
    f32 x[4] __attribute__((aligned(16)));
    f32 up[4] __attribute__((aligned(16)));

    if (AT(p, 0x20, u8) != 0) {
        sceVu0CopyVector(anchor, a + 12);
        sceVu0SubVector(x, (f32 *)p, anchor);
        sceVu0CopyVector(up, a + 4);
    } else {
        sceVu0CopyVector(anchor, AT(p, 0x2C, f32 *));
        sceVu0SubVector(x, (f32 *)p, anchor);
        sceVu0CopyVector(up, Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(AT(p, 0x2C, u8 *), 0x24, s32)) + 4);
    }
    sceVu0CopyVector(a, x);
    sceVu0OuterProduct(a + 8, a, up);
    sceVu0OuterProduct(a + 4, a + 8, a);
    sceVu0Normalize(a, a);
    sceVu0Normalize(a + 4, a + 4);
    sceVu0Normalize(a + 8, a + 8);
    sceVu0CopyVector(a + 12, anchor);
}

/* 0x002EE830 */
void SpringPart_Noop10(u8 *e) {
}

/* +0x14 of a sprung point: its bone (+0x24) moves to it, its axes scaled by +0x50 / +0x54 /
 * +0x58 */
/* 0x002EED20 */
void SprungPoint_AdjustBone(u8 *p, u8 *s) {
    f32 *b = Skel_Bone(AT(AT(s, 0x14, u8 *), 0x810, void *), AT(p, 0x24, s32));

    sceVu0ScaleVector(b, b, AT(p, 0x50, f32));
    sceVu0ScaleVector(b + 4, b + 4, AT(p, 0x54, f32));
    sceVu0ScaleVector(b + 8, b + 8, AT(p, 0x58, f32));
    sceVu0CopyVector(b + 12, (f32 *)p);
}

extern void Model_DrawWithShadow(u8 *m, s32 layer, s32 a, s32 b, f32 *light);   /* queue the model's drawing */

/* draw the model in `layer` (a, b: the character's draw parameters) with its light (+0x6C) */
/* 0x002DDAC0 */
void Model_Draw(u8 *m, s32 layer, s32 a, s32 b) {
    f32 light[4] __attribute__((aligned(16)));

    VCALL(m, 0x6C, void (*)(u8 *, f32 *))(m, light);
    Model_DrawWithShadow(m, layer, a, b, light);
}

#ifdef HG_NATIVE
#include <stdio.h>
#include <stdlib.h>

/* ---- PC: a character model drawn with OpenGL (what the model drawer's VU1 packets do) ----
 *
 * Resource 0 (+0x4C0, docs/model_format.md): bone records (0x70, the inverse bind matrix at
 * +0x30), the skinned parts (mesh table) and the rigid ones; resource 1 (+0x4D0) the morphing
 * parts. A vertex is in model space; bone b moves it by (inverse bind b) x (b's world matrix,
 * the skeleton at +0x810). Skinning is done here on the CPU. */

typedef struct {
    f32 *xyzw, *st;
    u8 *rgba;
    s32 cap;
} ModelBuf;

static ModelBuf sMb;

/* Lighting, as the character microprograms do it (tools/vudis.py, e.g. the chain D_003A38C0):
 * the model's light set comes from the scene's lights (+0x10, Lights_ForModel, as the drawer
 * func_001BDF80 asks for it: at its root bone +0x2C, on nav triangle +0x28) - per light a
 * direction (columns of the transposed matrix, the 4th row -dir.L), a colour, a falloff; the
 * 4th colour row is the ambient. A vertex at world P with normal N gets
 *   min(ambient + sum_i colour_i * max(dir_i.N^, 0) * max(1 + falloff_i * (dir_i.P - dir_i.L_i), 0), 128)
 * (0x80 = 1.0 against the texture), alpha 127.
 * Not ported: parts whose mode ((model +0x88 + 2 x part) | record +0x18) & 0xFC is 4, 8 or 0xC
 * use the lit programs (e.g. D_003A2EF0 for rigid parts), which also put a specular term into
 * the vertex alpha: sum over the lights of max(R.V, 0)^4 x the light's strength (colour row .y)
 * x its diffuse term, scaled by the eye vector's w (R the light reflected about the normal, V
 * towards the eye). The other programs (D_003A58A0 ...) skip normalizing the normal and give
 * alpha 127, as here. */
static f32 sLDir[4][4] __attribute__((aligned(16)));
static f32 sLCol[4][4] __attribute__((aligned(16)));
static f32 sLFall[4];

static void light_setup(u8 *m) {
    f32 *root = Skel_Bone(AT(m, 0x18, void *), AT(m, 0x2C, s32));

    VCALL(gLights, 0x10, void (*)(VObject *, f32 *, s32, f32 (*)[4], f32 (*)[4], f32 *, f32 (*)[4]))(
        gLights, root != NULL ? root + 12 : NULL, AT(m, 0x28, s32), sLDir, sLCol, sLFall, NULL);
    {
        static s32 sDbg = -1, sN;

        if (sDbg < 0) {
            sDbg = getenv("HG_LIGHTDEBUG") != NULL;
        }
        if (sDbg && (sN++ % 120) == 0) {
            fprintf(stderr, "light: model %p tri %d bone %d lights %d amb %.1f %.1f %.1f\n", (void *)m, AT(m, 0x28, s32),
                    AT(m, 0x2C, s32), AT(gLights, 0x10, s32), sLCol[3][0], sLCol[3][1], sLCol[3][2]);
            fprintf(stderr, "  col0 %.1f %.1f %.1f col1 %.1f %.1f %.1f col2 %.1f %.1f %.1f fall %.4f %.4f %.4f\n",
                    sLCol[0][0], sLCol[0][1], sLCol[0][2], sLCol[1][0], sLCol[1][1], sLCol[1][2], sLCol[2][0],
                    sLCol[2][1], sLCol[2][2], sLFall[0], sLFall[1], sLFall[2]);
        }
    }
}

/* a vertex colour for world position `p` and normal `n` (not unit) */
static u32 light_rgba(const f32 *n, const f32 *p) {
    f32 len2 = n[0] * n[0] + n[1] * n[1] + n[2] * n[2];
    f32 r = len2 > 0.0f ? 1.0f / __builtin_sqrtf(len2) : 0.0f;
    f32 c[3];
    s32 i, k;
    u32 out = 0x7F000000u;

    for (k = 0; k < 3; k++) {
        c[k] = sLCol[3][k];
    }
    for (i = 0; i < 3; i++) {
        f32 d = (sLDir[0][i] * n[0] + sLDir[1][i] * n[1] + sLDir[2][i] * n[2]) * r;
        f32 f = 1.0f + sLFall[i] * (sLDir[0][i] * p[0] + sLDir[1][i] * p[1] + sLDir[2][i] * p[2] + sLDir[3][i]);
        f32 l = (d > 0.0f ? d : 0.0f) * (f > 0.0f ? f : 0.0f);

        for (k = 0; k < 3; k++) {
            c[k] += sLCol[i][k] * l;
        }
    }
    for (k = 0; k < 3; k++) {
        s32 v = (s32)(c[k] < 128.0f ? c[k] : 128.0f);

        if (v < 0) {
            v = 0;
        }
        out |= (u32)v << (k * 8);
    }
    return out;
}

/* a normal (3 x s16 / 32768) turned by a matrix's 3 x 3, scaled by w, added to `acc` */
static void normal_add(f32 *acc, f32 (*m)[4], const s16 *n, f32 w) {
    f32 x = n[0] / 32768.0f, y = n[1] / 32768.0f, z = n[2] / 32768.0f;
    s32 k;

    for (k = 0; k < 3; k++) {
        acc[k] += w * (m[0][k] * x + m[1][k] * y + m[2][k] * z);
    }
}

static void mb_reserve(s32 n) {
    if (n > sMb.cap) {
        sMb.cap = n;
        sMb.xyzw = realloc(sMb.xyzw, n * 16);
        sMb.st = realloc(sMb.st, n * 8);
        sMb.rgba = realloc(sMb.rgba, n * 4);
    }
}

/* bone `b`'s skinning matrix */
static void bone_skin(u8 *m, s32 b, f32 (*out)[4]) {
    sceVu0MulMatrix(out, (f32 (*)[4])Skel_Bone(AT(m, 0x810, void *), b),
                    (f32 (*)[4])(AT(m, 0x4C0, u8 *) + 0x10 + b * 0x70 + 0x30));
}

/* the part's texture (.TEX entry): from the character's texture set (gBootMessage slot +0x24;
 * 0x24-byte slots from +0x4: the .TEX file at +0x8, its count at +0xC - the original only
 * needs the VRAM entry the upload went to, the PC decodes the entry itself) */
static void *model_tex(u8 *m, s32 tex) {
    u8 *slot = (u8 *)gBootMessage + 4 + AT(m, 0x24, u8) * 0x24;

    if (tex < 0 || AT(slot, 0x0, u8) == 0 || AT(slot, 0x8, u8 *) == NULL || (u32)tex >= AT(slot, 0xC, u32)) {
        return NULL;
    }
    return AT(slot, 0x8, u8 *) + 0x10 + tex * 0x10;
}

static void model_emit(u8 *m, const f32 *mvp, s32 n, s32 tex, s32 flags) {
    /* second-pass parts (flags bit 0) are cut out by their texture's alpha */
    glr_strip(mvp, n, sMb.xyzw, sMb.st, sMb.rgba, model_tex(m, tex), flags & 1 ? 1ULL << 34 : 0, 0x1C);
}

static void vtx_set(s32 i, const f32 *p, u16 u, u16 v, u32 noTri) {
    sMb.xyzw[i * 4] = p[0];
    sMb.xyzw[i * 4 + 1] = p[1];
    sMb.xyzw[i * 4 + 2] = p[2];
    AT(&sMb.xyzw[i * 4 + 3], 0, u32) = noTri ? 0x8000 : 0;
    sMb.st[i * 2] = u / 32768.0f;
    sMb.st[i * 2 + 1] = v / 32768.0f;
    AT(sMb.rgba, i * 4, u32) = 0x80808080;
}

static void gl_skinned_parts(u8 *m, const f32 *mvp) {
    u8 *r0 = AT(m, 0x4C0, u8 *);
    u8 *mt = r0 + AT(r0, 0x4, s32);
    f32 pal[32][4][4] __attribute__((aligned(16)));
    s32 i, k, j;

    for (i = 0; i < AT(mt, 0x0, s32); i++) {
        u8 *rec = mt + 0x10 + i * 0x30;
        s32 n = AT(rec, 0x0, s32), npal = AT(rec, 0x24, s32), infl = AT(rec, 0x2C, s32);
        const s32 *start = (const s32 *)(rec + AT(rec, 0x4, s32));
        const s16 *d = (const s16 *)(rec + AT(rec, 0x4, s32) + 0x10);
        const u16 *uv = (const u16 *)(rec + AT(rec, 0x8, s32));
        const s16 *nrm = (const s16 *)(rec + AT(rec, 0xC, s32));
        const u16 *w = (const u16 *)(rec + AT(rec, 0x10, s32));
        const u8 *bi = rec + AT(rec, 0x14, s32);
        const u8 *fl = rec + AT(rec, 0x18, s32);
        const u8 *pb = rec + AT(rec, 0x28, s32);
        s32 x = start[0], y = start[1], z = start[2];

        if (n <= 0 || npal > 32 || infl < 1 || infl > 4) {
            continue;
        }
        for (j = 0; j < npal; j++) {
            bone_skin(m, pb[j], pal[j]);
        }
        mb_reserve(n);
        for (k = 0; k < n; k++) {
            f32 v[4] = {0, 0, 0, 1}, o[3] = {0, 0, 0}, nn[3] = {0, 0, 0}, t[4], ws = 0.0f;

            x += d[k * 3];
            y += d[k * 3 + 1];
            z += d[k * 3 + 2];
            v[0] = x / 4096.0f;
            v[1] = y / 4096.0f;
            v[2] = z / 4096.0f;
            for (j = 0; j < infl; j++) {
                ws += w[k * infl + j];
            }
            for (j = 0; j < infl; j++) {
                f32 wt = ws == 0.0f ? (j == 0 ? 1.0f : 0.0f) : w[k * infl + j] / 32768.0f;
                s32 s = bi[k * infl + j] / 4;

                if (wt == 0.0f || s >= npal) {
                    continue;
                }
                sceVu0ApplyMatrix(t, pal[s], v);
                o[0] += t[0] * wt;
                o[1] += t[1] * wt;
                o[2] += t[2] * wt;
                normal_add(nn, pal[s], nrm + k * 3, wt);
            }
            vtx_set(k, o, uv[k * 2], uv[k * 2 + 1], fl[k] & 1);
            AT(sMb.rgba, k * 4, u32) = light_rgba(nn, o);
        }
        model_emit(m, mvp, n, AT(rec, 0x1C, s32), AT(rec, 0x20, s32));
    }
}

static void gl_rigid_parts(u8 *m, const f32 *mvp) {
    u8 *r0 = AT(m, 0x4C0, u8 *);
    u8 *rt = r0 + AT(r0, 0x8, s32);
    f32 b[4][4] __attribute__((aligned(16)));
    s32 i, k;

    if (AT(r0, 0x8, s32) == 0) {
        return;
    }
    for (i = 0; i < AT(rt, 0x0, s32); i++) {
        u8 *rec = rt + 0x10 + i * 0x20;
        s32 n = AT(rec, 0x0, s32);
        const s32 *start = (const s32 *)(rec + AT(rec, 0x4, s32));
        const s16 *d = (const s16 *)(rec + AT(rec, 0x4, s32) + 0x10);
        const u16 *uv = (const u16 *)(rec + AT(rec, 0x8, s32));
        const s16 *nrm = (const s16 *)(rec + AT(rec, 0xC, s32));
        const u8 *fl = rec + AT(rec, 0x10, s32);
        s32 x = start[0], y = start[1], z = start[2];

        if (n <= 0) {
            continue;
        }
        bone_skin(m, AT(rec, 0x1C, s32), b);
        mb_reserve(n);
        for (k = 0; k < n; k++) {
            f32 v[4], t[4];

            x += d[k * 3];
            y += d[k * 3 + 1];
            z += d[k * 3 + 2];
            v[0] = x / 4096.0f;
            v[1] = y / 4096.0f;
            v[2] = z / 4096.0f;
            v[3] = 1.0f;
            sceVu0ApplyMatrix(t, b, v);
            vtx_set(k, t, uv[k * 2], uv[k * 2 + 1], fl[k] & 1);
            {
                f32 nn[3] = {0, 0, 0};

                normal_add(nn, b, nrm + k * 3, 1.0f);
                AT(sMb.rgba, k * 4, u32) = light_rgba(nn, t);
            }
        }
        model_emit(m, mvp, n, AT(rec, 0x14, s32), 0);
    }
}

/* a morph shape's position and normal (shape table entry: offsets from it), into acc by w */
static void morph_add(u8 *rec, s32 shape, s32 k, f32 w, f32 *pos, f32 *nrm) {
    u8 *e = rec + AT(rec, 0x10, s32) + shape * 8;
    const s16 *p = (const s16 *)(e + AT(e, 0x0, s32)) + k * 3;
    const s16 *n = (const s16 *)(e + AT(e, 0x4, s32)) + k * 3;
    s32 j;

    for (j = 0; j < 3; j++) {
        pos[j] += w * (f32)p[j];
        nrm[j] += w * (f32)n[j];
    }
}

/* morphing parts (resource 1, as func_001BD650 picks them): the face (kind 1) blends the rest
 * shape with shapes 1 (mouth), 7 and 8 (eyes) by the model's weights (+0x58 + 4 x shape; the
 * rest gets what is left); the hands (kind 0) blend two shapes - the second part by +0x38 /
 * +0x3C / +0x40 (shapes, weight), the others by +0x48 / +0x4C / +0x50 */
static void gl_morph_parts(u8 *m, const f32 *mvp) {
    u8 *r1 = AT(m, 0x4D0, u8 *);
    f32 b[4][4] __attribute__((aligned(16)));
    s32 i, k;

    if (r1 == NULL) {
        return;
    }
    for (i = 0; i < AT(r1, 0x0, s32); i++) {
        u8 *rec = r1 + 0x10 + i * 0x40;
        s32 n = AT(rec, 0x4, s32), nshape = AT(rec, 0x0, s32);
        const u16 *uv = (const u16 *)(rec + AT(rec, 0x8, s32));
        const u8 *fl = rec + AT(rec, 0xC, s32);   /* strip flags: the face's u32 (bit 15), the hands' u8 (1) */
        const s32 *base = (const s32 *)(rec + 0x30);
        s32 sh[4], ns = 0;
        f32 w[4];

        if (n <= 0) {
            continue;
        }
        if (AT(rec, 0x1C, s32) != 0) {
            const f32 *fw = (const f32 *)(m + 0x58);

            sh[0] = 0; sh[1] = 1; sh[2] = 7; sh[3] = 8;
            w[1] = fw[1];
            w[2] = fw[7];
            w[3] = fw[8];
            w[0] = 1.0f - w[1] - w[2] - w[3];
            ns = nshape > 8 ? 4 : 1;
        } else {
            u8 *h = m + (i == 1 ? 0x38 : 0x48);

            sh[0] = AT(h, 0x0, s32);
            sh[1] = AT(h, 0x4, s32);
            w[1] = AT(h, 0x8, f32);
            w[0] = 1.0f - w[1];
            ns = sh[0] >= 0 && sh[0] < nshape && sh[1] >= 0 && sh[1] < nshape ? 2 : 0;
            if (ns == 0) {
                sh[0] = 0;
                w[0] = 1.0f;
                ns = 1;
            }
        }
        if (ns == 1) {
            w[0] = 1.0f;
        }
        bone_skin(m, AT(rec, 0x14, s32), b);
        mb_reserve(n);
        for (k = 0; k < n; k++) {
            f32 v[4], t[4], p[3] = {0, 0, 0}, nn[3] = {0, 0, 0}, wn[3] = {0, 0, 0};
            s32 j;

            for (j = 0; j < ns; j++) {
                morph_add(rec, sh[j], k, w[j], p, nn);
            }
            v[0] = (base[0] + p[0]) / 4096.0f;
            v[1] = (base[1] + p[1]) / 4096.0f;
            v[2] = (base[2] + p[2]) / 4096.0f;
            v[3] = 1.0f;
            sceVu0ApplyMatrix(t, b, v);
            vtx_set(k, t, uv[k * 2], uv[k * 2 + 1],
                    AT(rec, 0x1C, s32) != 0 ? AT(fl, k * 4, u32) & 0x8000 : fl[k] & 1);
            for (j = 0; j < 3; j++) {
                wn[j] = b[0][j] * nn[0] + b[1][j] * nn[1] + b[2][j] * nn[2];
            }
            AT(sMb.rgba, k * 4, u32) = light_rgba(wn, t);
        }
        model_emit(m, mvp, n, AT(rec, 0x18, s32), AT(rec, 0x20, s32));
    }
}

static void gl_draw_model(u8 *m) {
    f32 clip[4][4] __attribute__((aligned(16)));

    if (AT(m, 0x4C0, u8 *) == NULL || AT(m, 0x810, u8 *) == NULL) {
        return;
    }
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
    light_setup(m);
    gl_skinned_parts(m, &clip[0][0]);
    gl_rigid_parts(m, &clip[0][0]);
    gl_morph_parts(m, &clip[0][0]);
}
#endif

#ifdef HG_NATIVE

/* draw the model in `layer` (a, b: the character's draw parameters, kept at +0x28 / +0x2C;
 * layer 0x14 none), then its shadow volumes (+0x1D0, layer 6) unless +0x4D9 or in layers
 * 0x14 / 0x17 / 0x1C. (The original queues the model drawer, layer 0xB twice around masks.) */
/* 0x001F6870 */
void Model_DrawWithShadow(u8 *m, s32 layer, s32 a, s32 b, f32 *light) {
    if (layer == 0x14) {
        AT(m, 0x28, s32) = -1;
        AT(m, 0x2C, s32) = -1;
    } else {
        AT(m, 0x28, s32) = a;
        AT(m, 0x2C, s32) = b;
    }
    glr_layer((u16)layer);
    gl_draw_model(m);
    glr_layer(-1);
    if (AT(m, 0x4D9, u8) == 0 && layer != 0x17 && layer != 0x14 && layer != 0x1C) {
        Shadow_Queue(m + 0x1D0, a, b, light, layer);
    }
}
#endif

/* reset the play state before a new animation (as Motion_Play): +0x85C / +0x85D off, the
 * speeds +0x38 / +0x3C and +0x48 / +0x4C from the defaults +0x87C / +0x880, +0x40 / +0x50 zero */
static void motion_reset_play(u8 *m) {
    motion_defaults(m);
}

/* play animation `anim` (variant `variant`) blending in over the frames the table (+0x874)
 * gives at +0, with its flags (+4); an animation not in the table cuts in at once */
/* 0x002DDED0 */
void Motion_PlayTable(u8 *m, s32 anim, s32 variant) {
    s32 i;
    f32 blend;
    u32 flags;

    if (Motion_AnimIndex(m, anim) == -1) {
        motion_reset_play(m);
        Motion_Start(m, anim, 0, -1, 0.0f);
        return;
    }
    i = Motion_AnimIndex(m, anim);
    blend = (f32)(i != -1 ? AT(AT(m, 0x874, u8 *), i * 6, s16) : 0);
    i = Motion_AnimIndex(m, anim);
    flags = i != -1 ? AT(AT(m, 0x874, u8 *), i * 6 + 4, u16) : 0;
    motion_reset_play(m);
    Motion_Start(m, anim, flags & 0xFFFF, variant, blend);
}

/* play animation `anim` driven from outside: both motion layers' flag 0x10 (no own time) */
/* 0x002DD040 */
void Motion_PlayDriven(u8 *m, s32 anim) {
    Motion_PlayTable(m, anim, 0);
    AT(AT(m, 0x6A4, u8 *), 0x18, u32) |= 0x10;
    AT(AT(m, 0x6A8, u8 *), 0x18, u32) |= 0x10;
}

/* the motion's time (+0x6A4 +0) = how far cutscene frame `frame` is into its shot (director
   +0x20), shared by the three blend channels' time pointers (+0x704, 0x60 apart) */
/* 0x002DD090 */
void Motion_CutsceneTime(u8 *m, s32 frame) {
    s32 i;

    *AT(m, 0x6A4, f32 *) = (f32)VCALL(gCutscene, 0x20, s32 (*)(VObject *, s32))(gCutscene, frame);
    for (i = 0; i < 3; i++) {
        f32 *t = AT(m, 0x704 + i * 0x60, f32 *);

        if (t != NULL) {
            *t = *AT(m, 0x6A4, f32 *);
        }
    }
}

/* how the model (its frame +0x7D0: up +0x7E0, forward +0x7F0) has to look at `target` from its
 * eye (+0x860, in model space): the pitch (atan2 of the height over the level distance) and
 * the turn (the angle from forward on the level, negative to the left) */
/* 0x002DD110 */
void Motion_LookAt(u8 *m, const f32 *target, f32 *pitch, f32 *turn) {
    f32 v[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 *up = (f32 *)(m + 0x7E0), *fwd = (f32 *)(m + 0x7F0);
    f32 h, along, pp, ff, c;

    v[0] = AT(m, 0x860, f32);
    v[1] = AT(m, 0x864, f32);
    v[2] = AT(m, 0x868, f32);
    v[3] = 1.0f;
    sceVu0ApplyMatrix(v, (f32 (*)[4])(m + 0x7D0), v);
    sceVu0SubVector(v, (f32 *)target, v);
    h = sceVu0InnerProduct(v, up);
    p[0] = 0.0f + v[0] - h * up[0];
    p[1] = 0.0f + v[1] - h * up[1];
    p[2] = 0.0f + v[2] - h * up[2];
    p[3] = __builtin_sqrtf(p[2] * p[2] + p[0] * p[0]);
    *pitch = msl_atan2f(h, p[3]);
    along = sceVu0InnerProduct(p, fwd);
    pp = sceVu0InnerProduct(p, p);
    ff = sceVu0InnerProduct(fwd, fwd);
    c = along / (__builtin_sqrtf(pp) * __builtin_sqrtf(ff));
    if (c < -1.0f) {
        c = -1.0f;
    }
    if (!(c <= 1.0f)) {
        c = 1.0f;
    }
    *turn = msl_acosf(c);
    sceVu0OuterProduct(p, fwd, v);
    if (p[1] < 0.0f) {
        *turn = *turn * -1.0f;
    }
}

/* Moves the floats at +0x854/+0x858 toward (tx, ty) by at most |sx|/|sy|. */
/* 0x002DD310 */
void Motion_EaseTilt(u8 *p, f32 tx, f32 ty, f32 sx, f32 sy) {
    f32 v, d;

    if (sx <= 0.0f) sx = -sx;
    v = F(p, 0x854, f32);
    d = tx - v;
    if (d <= 0.0f) d = -d;
    if (d <= sx) {
        F(p, 0x854, f32) = tx;
    } else if (tx <= v) {
        F(p, 0x854, f32) -= sx;
    } else {
        F(p, 0x854, f32) += sx;
    }
    if (sy <= 0.0f) sy = -sy;
    v = F(p, 0x858, f32);
    d = ty - v;
    if (d <= 0.0f) d = -d;
    if (d <= sy) {
        F(p, 0x858, f32) = ty;
    } else if (ty <= v) {
        F(p, 0x858, f32) -= sy;
    } else {
        F(p, 0x858, f32) += sy;
    }
}

/* is foot `foot` down (the contact track +0x50, channel `foot`) `ofs` frames from now in the
 * previous slot's animation (+0x6A8; the time wrapped into it) */
/* 0x002DD860 */
u32 Motion_FootDownPrev(void *motion, s32 foot, f32 ofs) {
    f32 c[4] __attribute__((aligned(16)));
    u8 *slot = AT(motion, 0x6A8, u8 *);
    f32 t, len;

    c[3] = 0.0f;
    c[2] = 0.0f;
    c[1] = 0.0f;
    c[0] = 0.0f;
    if (AT(slot, 0x20, void *) == NULL) {
        return 0;
    }
    t = AT(slot, 0x0, f32) + ofs;
    len = (f32)AT(AT(AT(slot, 0x20, u8 *), 0x4, u8 *), 0xC, s32);
    while (t < 0.0f) {
        t = t + len;
    }
    while (!(t < len)) {
        t = t - len;
    }
    if (AT(slot, 0x50, void *) != NULL && AT(AT(slot, 0x50, u8 *), 0x0, void *) != NULL) {
        Track_Sample(AT(slot, 0x50, void *), c, t);
    }
    return !(c[foot] <= 0.0f);
}

/* 0x002DD970 */
f32 Model_FloorLevel(void) { return 1.0f; }

/* ---- the stalkers' and event characters' models (built by the loaders CharLoad_Kind33Model ..
 * CharLoad_DebilitasModel for CharLoad_Partner) ---- */

extern void *Shadow_vtable[], *IK2_vtable[], *SpringPartBase_vtable[];

/* the human characters' model base before its kind (vtable ModelBase_vtable): drawing object, the
   +0x1D0 part, the model fields */
/* 0x0016FCD0 */
void *HumanModel_BaseCtor(u8 *m) {
    AT(m, 0x0, void **) = ModelBase_vtable;
    DrawObj_Init(m + 0x10);
    AT(m, 0x1D0, void **) = Helper469D00_vtable;
    AT(m, 0x1D4, s32) = -1;
    AT(m, 0x1D0, void **) = Shadow_vtable;
    AT(m, 0x4B0, s32) = 0;
    ModelBase_Zero(m);
    return m;
}

/* the human model base with its two 0x60 parts' vtables (+0x928 / +0x988) */
/* 0x0016FC30 */
void *HumanModel_PartsCtor(u8 *m) {
    HumanModel_BaseCtor(m);
    AT(m, 0x0, void **) = HumanModel_vtable;
    AT(m, 0x928, void **) = IK2_vtable;
    AT(m, 0x988, void **) = IK2_vtable;
    return m;
}

/* its destructor */
/* 0x0016F9E0 */
void *HumanModel_dtor(u8 *m, s32 flags) {
    return HumanModel_Destroy(m, flags);
}

/* 0x0016FAE0 */
void *Capsule_ctor(u8 *p) {
    FLD(p, 0x30, void **) = Capsule_vtable;
    return p;
}

/* destructors of the model's parts and array elements: the element's vtable (at +0 or +0x30)
   back to its base, then (flags > 0) delete */
static inline void *Part_Destroy(u8 *e, s32 at, void **vt, void **base, s32 flags) {
    if (e != NULL) {
        AT(e, at, void **) = vt;
        AT(e, at, void **) = base;
        if ((s16)flags > 0) {
            __dl__FPv(e);
        }
    }
    return e;
}

/* 0x0016F680 */
void *Shadow_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x0, Shadow_vtable, Helper469D00_vtable, flags);
}

/* 0x0016F6E0 */
void *ModelDrawer_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x0, ModelDrawer_vtable, Helper469D00_vtable, flags);
}

/* 0x0016F740 */
void *Part_ctor(u8 *p) {
    FLD(p, 0x0, void **) = Helper469D00_vtable;
    FLD(p, 0x4, s32) = -1;
    FLD(p, 0x0, void **) = Shadow_vtable;
    FLD(p, 0x2E0, s32) = 0;
    return p;
}

/* 0x0016F990 */
void *Part_delete(void *e, s32 flags) {
    if (e != NULL && (s16)flags > 0) {
        __dl__FPv(e);
    }
    return e;
}

/* 0x0016FC80 */
void *IK2_Destroy(void *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x58, void **) = IK2_vtable;
        if ((s16)flags > 0) {
            __dl__FPv(e);
        }
    }
    return e;
}

/* the three-bone IK solver's destructor */
/* 0x001F7E40 */
void *IK3_Destroy(void *e, s32 flags) {
    if (e != NULL) {
        AT(e, 0x58, void **) = IK3_vtable;
        AT(e, 0x58, void **) = IK2_vtable;
        if ((s16)flags > 0) {
            __dl__FPv(e);
        }
    }
    return e;
}

/* 0x001F7F90 */
s32 Model_Vt7C(u8 *m) {
    return 0;
}

extern void *HangPoint_vtable[], *Part60_vtable[], *SwayPointA_vtable[], *SprungPoint_vtable[], *BoneHangPoint_vtable[],
    *SwayPointB_vtable[], *Part50_vtable[], *HairPoint_vtable[], *HangingPart_vtable[];

/* 0x0016FBB0 */
void *HangPoint_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, HangPoint_vtable, SpringPartBase_vtable, flags);
}

/* 0x0016FC10 */
void *HangPoint_ctor(u8 *p) {
    FLD(p, 0x30, void **) = HangPoint_vtable;
    return p;
}

/* 0x0016FB00 */
void *Part60_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, Part60_vtable, SpringPartBase_vtable, flags);
}

/* 0x0016FB60 */
void *Part60_ctor(u8 *p) {
    FLD(p, 0x30, void **) = Part60_vtable;
    return p;
}

/* 0x0016FB80 */
void *SpringSet_ctor(u8 *p) {
    FLD(p, 0x34, s32) = 0;
    FLD(p, 0x30, s32) = 0;
    return p;
}

/* 0x0016FB90 */
void *BonePoint_ctor(u8 *p) {
    FLD(p, 0x30, void **) = BonePoint_vtable;
    return p;
}

/* 0x00170080 */
void *SwayPointA_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, SwayPointA_vtable, SpringPartBase_vtable, flags);
}

/* 0x00170290 */
void *SprungPoint_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, SprungPoint_vtable, SpringPartBase_vtable, flags);
}

/* 0x001702F0 */
void *BoneHangPoint_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, BoneHangPoint_vtable, SpringPartBase_vtable, flags);
}

/* 0x001709D0 */
void *SwayPointB_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, SwayPointB_vtable, SpringPartBase_vtable, flags);
}

/* 0x00170A30 */
void *SwayPointB_ctor(u8 *p) {
    FLD(p, 0x30, void **) = SwayPointB_vtable;
    return p;
}

/* 0x00170CB0 */
void *Part50_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, Part50_vtable, SpringPartBase_vtable, flags);
}

/* 0x00170D10 */
void *Part50_ctor(u8 *p) {
    FLD(p, 0x30, void **) = Part50_vtable;
    return p;
}

/* 0x00170EB0 */
void *HairPoint_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, HairPoint_vtable, SpringPartBase_vtable, flags);
}

/* 0x00170F10 */
void *HairPoint_ctor(u8 *p) {
    FLD(p, 0x30, void **) = HairPoint_vtable;
    return p;
}

/* 0x00170F30 */
void *HangingPart_dtor(void *e, s32 flags) {
    return Part_Destroy(e, 0x30, HangingPart_vtable, SpringPartBase_vtable, flags);
}

/* 0x00170F90 */
void *HangingPart_ctor(u8 *p) {
    FLD(p, 0x30, void **) = HangingPart_vtable;
    return p;
}

/* the base model's destructor (vtable ModelBase_vtable down to its parts) */
/* 0x0016F5D0 */
void *ModelBase_dtor(void *p, s32 flags) {
    u8 *m = p;

    if (m != NULL) {
        AT(m, 0x0, void **) = ModelBase_vtable;
        AT(m, 0x1D0, void **) = Shadow_vtable;
        AT(m, 0x1D0, void **) = Helper469D00_vtable;
        AT(m, 0x10, void **) = ModelDrawer_vtable;
        AT(m, 0x10, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            __dl__FPv(m);
        }
    }
    return m;
}

/* the human-with-kind model's destructor (vtable CharModel_vtable, then the human base) */
/* 0x00170350 */
void *CharModel_dtor(void *p, s32 flags) {
    u8 *m = p;

    if (m != NULL) {
        AT(m, 0x0, void **) = CharModel_vtable;
        HumanModel_Destroy(m, flags);
    }
    return m;
}

/* 0x00170460 */
void *SwayPointA_ctor(u8 *p) {
    FLD(p, 0x30, void **) = SwayPointA_vtable;
    return p;
}

/* ---- Fiona's model in the clothes left on the bed (vtable Kind18Model_vtable, the EventHumanModel_vtable layout
 * with its parts destroyed one by one) ---- */

extern void *Kind18Model_vtable[];

extern void CharModel_Loaded(u8 *m);
extern void EventHumanModel_Setup(u8 *m);

extern void *EventHumanModel_vtable[], *BonePoint_vtable[];
extern void *SwayPointA_dtor(void *, s32);

/* a human event character's model (vtable EventHumanModel_vtable, 0x1270 bytes) of `kind`: twelve 0x50
   parts at +0x9B0, four at +0xE40, six 0x40 parts at +0xF80 and the single parts between */
/* 0x001700E0 */
void *EventHumanModel_ctor(u8 *m, s32 kind) {
    u8 *e;

    HumanModel_PartsCtor(m);
    AT(m, 0x0, void **) = CharModel_vtable;
    AT(m, 0x9A0, u8) = kind;
    AT(m, 0x0, void **) = EventHumanModel_vtable;
    __construct_array(m + 0x9B0, SwayPointA_ctor, SwayPointA_dtor, 0x50, 0xC);
    AT(m, 0xDA4, s32) = 0;
    AT(m, 0xDA0, s32) = 0;
    AT(m, 0xDE0, void **) = BoneHangPoint_vtable;
    AT(m, 0xE34, s32) = 0;
    AT(m, 0xE30, s32) = 0;
    __construct_array(m + 0xE40, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
    for (e = m + 0xF80; e < m + 0x1100; e += 0x40) {
        AT(e, 0x30, void **) = BonePoint_vtable;
    }
    AT(m, 0x1134, s32) = 0;
    AT(m, 0x1130, s32) = 0;
    AT(m, 0x1170, void **) = SprungPoint_vtable;
    AT(m, 0x11D4, s32) = 0;
    AT(m, 0x11D0, s32) = 0;
    AT(m, 0x1210, void **) = BoneHangPoint_vtable;
    AT(m, 0x1264, s32) = 0;
    AT(m, 0x1260, s32) = 0;
    return m;
}

/* its destructor */
/* 0x0016FF50 */
void *EventHumanModel_dtor(void *p, s32 flags) {
    u8 *m = p;

    if (m != NULL) {
        AT(m, 0x0, void **) = EventHumanModel_vtable;
        AT(m, 0x1210, void **) = BoneHangPoint_vtable;
        AT(m, 0x1210, void **) = SpringPartBase_vtable;
        AT(m, 0x1170, void **) = SprungPoint_vtable;
        AT(m, 0x1170, void **) = SpringPartBase_vtable;
        __destroy_arr(m + 0xE40, HangPoint_dtor, 0x50, 4);
        AT(m, 0xDE0, void **) = BoneHangPoint_vtable;
        AT(m, 0xDE0, void **) = SpringPartBase_vtable;
        __destroy_arr(m + 0x9B0, SwayPointA_dtor, 0x50, 0xC);
        AT(m, 0x0, void **) = CharModel_vtable;
        HumanModel_dtor(m, 0);
        if ((s16)flags > 0) {
            StalkerModel_delete(m);
        }
    }
    return m;
}

/* ---- the model loaders: a model of the kind's size from the scene heap (+0x6FBF00), built,
   and put at character `slot` +0xF0 (NULL when the heap is full) ---- */

static inline u8 *Model_New(Progress *p, u32 size) {
    VObject *heap = (VObject *)((u8 *)p + 0x6FBF00);

    return Model_new(size, VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, size));
}

extern void *Kind33Model_vtable[], *Kind23Model_vtable[], *Kind18Model_vtable[], *Model_vtable[], *Kind12Model_vtable[],
    *Kind14Model_vtable[], *Lorenzo2Model_vtable[], *LorenzoModel_vtable[], *Kind09Model_vtable[], *RiccardoModel_vtable[], *DaniellaModel_vtable[],
    *DebilitasModel_vtable[];

extern void *HumanModel_vtable[];   /* the base with two parts at +0x8D0 / +0x930 */

extern void *BonePoint_vtable[];

extern void *Capsule_vtable[];

/* kind 33 */
/* 0x0016F420 */
void CharLoad_Kind33Model(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x890);

    if (m != NULL) {
        ModelBase_ctor(m);
        AT(m, 0x0, void **) = Kind33Model_vtable;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kinds 23 / 37 */
/* 0x0016F860 */
void CharLoad_Kind23Model(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1310);

    if (m != NULL) {
        u8 *e;

        HumanModel_PartsCtor(m);
        AT(m, 0x0, void **) = Kind23Model_vtable;
        __construct_array(m + 0x9A0, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
        for (e = m + 0xAE0; e < m + 0xBE0; e += 0x40) {
            BonePoint_ctor(e);
        }
        SpringSet_ctor(m + 0xBE0);
        __construct_array(m + 0xC20, Part60_ctor, Part60_dtor, 0x60, 0xC);
        SpringSet_ctor(m + 0x10A0);
        for (e = m + 0x10E0; e < m + 0x1310; e += 0x70) {
            Capsule_ctor(e);
        }
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 18 */
/* 0x0016FEC0 */
void CharLoad_Kind18Model(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1270);

    if (m != NULL) {
        EventHumanModel_ctor(m, 5);
        AT(m, 0x0, void **) = Kind18Model_vtable;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kinds 14 / 15 */
/* 0x00170480 */
void CharLoad_Kind14Model(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x890);

    if (m != NULL) {
        ModelBase_ctor(m);
        AT(m, 0x0, void **) = Kind14Model_vtable;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 13 */
/* 0x00170510 */
void CharLoad_Kind13Model(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1270);

    if (m != NULL) {
        u8 *e;

        HumanModel_ctor(m, 0);
        AT(m, 0x0, void **) = EventHumanModel_vtable;
        __construct_array(m + 0x9B0, SwayPointA_ctor, SwayPointA_dtor, 0x50, 0xC);
        SpringSet_ctor(m + 0xD70);
        BoneHangPoint_ctor(m + 0xDB0);
        SpringSet_ctor(m + 0xE00);
        __construct_array(m + 0xE40, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
        for (e = m + 0xF80; e < m + 0x1100; e += 0x40) {
            BonePoint_ctor(e);
        }
        SpringSet_ctor(m + 0x1100);
        SprungPoint_ctor(m + 0x1140);
        SpringSet_ctor(m + 0x11A0);
        BoneHangPoint_ctor(m + 0x11E0);
        SpringSet_ctor(m + 0x1230);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* 0x00170650 */
void *SprungPoint_ctor(u8 *p) {
    FLD(p, 0x30, void **) = SprungPoint_vtable;
    return p;
}

/* 0x00170670 */
void *BoneHangPoint_ctor(u8 *p) {
    FLD(p, 0x30, void **) = BoneHangPoint_vtable;
    return p;
}

/* the plain model (vtable Model_vtable): most event characters */
/* 0x00170710 */
void CharLoad_PlainModel(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x890);

    if (m != NULL) {
        HumanModel_BaseCtor(m);
        AT(m, 0x0, void **) = Model_vtable;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 12 */
/* 0x001707A0 */
void CharLoad_Kind12Model(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x9A0);

    if (m != NULL) {
        HumanModel_PartsCtor(m);
        AT(m, 0x0, void **) = Kind12Model_vtable;
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 11 (Lorenzo) */
/* 0x00170830 */
void CharLoad_LorenzoModel(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0xD00);

    if (m != NULL) {
        u8 *e;

        ModelBase_ctor(m);
        AT(m, 0x0, void **) = LorenzoModel_vtable;
        __construct_array(m + 0x890, HangPoint_ctor, HangPoint_dtor, 0x50, 6);
        for (e = m + 0xA70; e < m + 0xC70; e += 0x40) {
            BonePoint_ctor(e);
        }
        SpringSet_ctor(m + 0xCC0);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kinds 10 / 39 (the second Lorenzo) */
/* 0x00170910 */
void CharLoad_Lorenzo2Model(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1160);

    if (m != NULL) {
        HumanModel_PartsCtor(m);
        AT(m, 0x0, void **) = Lorenzo2Model_vtable;
        __construct_array(m + 0x9A0, SwayPointB_ctor, SwayPointB_dtor, 0x50, 0x18);
        SpringSet_ctor(m + 0x1120);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 9 */
/* 0x00170A50 */
void CharLoad_Kind09Model(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1500);

    if (m != NULL) {
        u8 *e;

        HumanModel_PartsCtor(m);
        AT(m, 0x0, void **) = Kind09Model_vtable;
        __construct_array(m + 0x9A0, HangPoint_ctor, HangPoint_dtor, 0x50, 6);
        for (e = m + 0xB80; e < m + 0xD00; e += 0x40) {
            BonePoint_ctor(e);
        }
        SpringSet_ctor(m + 0xD00);
        __construct_array(m + 0xD40, SwayPointB_ctor, SwayPointB_dtor, 0x50, 0x18);
        SpringSet_ctor(m + 0x14C0);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kind 4 (Riccardo) */
/* 0x00170B60 */
void CharLoad_RiccardoModel(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1490);

    if (m != NULL) {
        u8 *e;

        HumanModel_PartsCtor(m);
        AT(m, 0x0, void **) = RiccardoModel_vtable;
        __construct_array(m + 0x9A0, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
        for (e = m + 0xAE0; e < m + 0xBE0; e += 0x40) {
            BonePoint_ctor(e);
        }
        SpringSet_ctor(m + 0xBE0);
        __construct_array(m + 0xC20, Part60_ctor, Part60_dtor, 0x60, 0xC);
        SpringSet_ctor(m + 0x10A0);
        for (e = m + 0x10E0; e < m + 0x1310; e += 0x70) {
            Capsule_ctor(e);
        }
        __construct_array(m + 0x1310, Part50_ctor, Part50_dtor, 0x50, 4);
        SpringSet_ctor(m + 0x1450);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kinds 3 / 34..36 (Daniella) */
/* 0x00170D30 */
void CharLoad_DaniellaModel(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0x1580);

    if (m != NULL) {
        u8 *e;

        HumanModel_PartsCtor(m);
        AT(m, 0x0, void **) = DaniellaModel_vtable;
        SpringSet_ctor(m + 0x9A0);
        SpringSet_ctor(m + 0x9E0);
        SpringSet_ctor(m + 0xA20);
        SpringSet_ctor(m + 0xA60);
        __construct_array(m + 0xAA0, HangingPart_ctor, HangingPart_dtor, 0x50, 6);
        for (e = m + 0xC80; e < m + 0xD60; e += 0x70) {
            Capsule_ctor(e);
        }
        for (e = m + 0xD60; e < m + 0xDE0; e += 0x40) {
            BonePoint_ctor(e);
        }
        __construct_array(m + 0xDE0, HairPoint_ctor, HairPoint_dtor, 0x70, 0xA);
        for (e = m + 0x1240; e < m + 0x1470; e += 0x70) {
            Capsule_ctor(e);
        }
        SprungPoint_ctor(m + 0x1470);
        __construct_array(m + 0x14D0, BoneHangPoint_ctor, BoneHangPoint_dtor, 0x50, 2);
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* kinds 2 / 6 / 7 / 27 (the Debilitas kind) */
/* 0x00170FB0 */
void CharLoad_DebilitasModel(Progress *p, u32 slot) {
    u8 *m = Model_New(p, 0xBA0);

    if (m != NULL) {
        u8 *e;

        HumanModel_PartsCtor(m);
        AT(m, 0x0, void **) = DebilitasModel_vtable;
        __construct_array(m + 0x9A0, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
        SpringSet_ctor(m + 0xAE0);
        for (e = m + 0xB20; e < m + 0xBA0; e += 0x40) {
            BonePoint_ctor(e);
        }
    }
    AT(gCharacters[slot], 0xF0, void *) = m;
}

/* the skeleton's bone `bone` (its matrix; NULL past the end) */
/* 0x0017CE80 */
f32 *Skel_Bone(u8 *skel, s32 bone) {
    u8 *node = AT(skel, 4, u8 *);
    s32 i = 0;

    while (node != NULL) {
        if (i == bone) {
            return (f32 *)node;
        }
        node = AT(node, 0x48, u8 *);
        i++;
    }
    return NULL;
}

/* 0x001800A0 */
s32 Model_Part3(u8 *m) {
    return 0;
}

/* 0x001800B0 */
s32 Model_Part2(u8 *m) {
    return 0;
}

/* 0x001800C0 */
s32 Model_Part1(u8 *m) {
    return 0;
}

/* 0x001800D0 */
s32 Model_Part0(u8 *m) {
    return 0;
}

/* 0x0018CC90 */
s32 Model_Vt78(u8 *m) {
    return 0;
}

/* 0x00194FB0 */
void Model_Vt2C(u8 *m) {
}

/* 0x00195C60 */
s32 Model_Vt74(u8 *m) {
    return 0;
}

/* 0x00195ED0 */
void Model_Vt30(u8 *m) {
}

/* 0x0019C200 */
void Model_Vt3C(u8 *m) {
}

/* play animation `anim` with its table entry's (+0x874, 6 bytes each) flags, blended in over
 * the entry's frames (no check for a missing entry) */
/* 0x002DDB30 */
void Motion_PlayTableNoCheck(u8 *m, s32 anim) {
    s32 i = Motion_AnimIndex(m, anim);
    u8 *e = AT(m, 0x874, u8 *) + i * 6;

    Motion_PlayWhenFree(m, anim, AT(e, 0x4, u16), -1, (f32)AT(e, 0x0, s16));
}

/* ---- more of the models at 0x35B000: load setups, part roles, a table ---- */

/* ---- Fiona's costumes 2 (Costume2Model_vtable, `at` 0xDA0) and 3 (Costume3Model_vtable, `at` 0xC60): the same
   layout from `at` (see costume_model), costume 3's bones 4 lower. Six spring sets: at - 0x30
   (the 12 / 8 hair nodes at +0x9B0), at + 0x60 (node at + 0x10), at + 0x100 (node at + 0xA0),
   at + 0x190 (node at + 0x140), at + 0x6D0 (16 nodes at + 0x1D0, colliders at + 0x710) and
   at + 0x980 (3 nodes at + 0x890, colliders at + 0x9C0) ---- */

extern void *WindHangPoint_dtor(void *e, s32 flags);
extern void *CostumeHangPoint_dtor(void *e, s32 flags);
extern void *SwayPointA_dtor(void *e, s32 flags);
extern void *Costume2Model_vtable[], *Costume3Model_vtable[];

/* ---- Fiona's costume 7 (Costume7Model_vtable, built by Costume7Model_ctor): four spring sets - +0xB40 five
   anchored nodes (+0x9B0, bones 0x1A..0x1E), +0xBE0 / +0xC80 one node each (+0xB80 bone 0xC,
   +0xC20 bone 0xD), +0xD10 one node (+0xCC0, bone 0xE) ---- */
