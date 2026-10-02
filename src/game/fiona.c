/* Fiona: the player character (vtable 0x46AAB0). */
#include "common.h"
#include "fiona.h"
#include "progress.h"
#include "navmesh.h"

extern Character *gCharacters[6];
extern VObject *gBootMessage;      /* message display, also used in game */
extern Progress *gProgress;

extern void func_001855F0(Fiona *f, s32);
extern void func_00125D40(Character *c);
extern void func_00126810(Character *c);
extern void func_00126910(Character *c);

extern const PTMF D_003B25A8;      /* idle state */
extern const PTMF D_003B25B8;      /* idle state (while unkE0 is set) */

static inline void Actor_SetState(Actor *a, const PTMF *state) {
    PTMF s = *state;

    if (ptmf_test(&s)) {
        a->state = s;
    }
}

/* Motion player byte +0x4D8 (1 = paused?) */
#define MOTION_U8(m, off) (*((u8 *)(m) + (off)))

/* Back to the idle state (inlined in several places in the original). */
static inline void Fiona_ToIdle(Fiona *f) {
    f->unk1AD580 = 0;
    f->c.moveMode = 0;
    f->savedYaw = f->c.a.angle[1];
    f->unk1AD5C0 = 0;
    f->unk1AD588 = 0;
    if (f->c.unkE0 == 0) {
        f->c.a.unk2D = 0;
        if (*(s32 *)((u8 *)f->c.motion + 0x4C4) != 0) {
            func_001855F0(f, -1);
        }
        Actor_SetState(&f->c.a, &D_003B25A8);
    } else {
        Actor_SetState(&f->c.a, &D_003B25B8);
    }
    Progress_ClearFlag(gProgress, 0x2B);
}

/* vtable +0x7C: back to the idle state. */
void func_0019A300(Fiona *f) {
    Fiona_ToIdle(f);
}

/* Set the character (by slot) she interacts with. */
void func_0019A420(Fiona *f, s32 slot, s32 param) {
    f->targetParam = param;
    f->target = gCharacters[slot];
}

/* vtable +0x60? (deactivate) */
void func_0019AF10(Fiona *f) {
    func_00125D40(&f->c);
}

/* Halt (as Character), and take down her message. */
void func_0019AA20(Fiona *f) {
    func_00126810(&f->c);
    MOTION_U8(f->c.motion, 0x4D8) = 0;
    f->unk1AD5D0 = 1;
    f->unk1AD5D1 = 1;
    f->unk1AD630 = 0;
    f->unk1AD62E = 0;
    if (f->msgImage != NULL) {
        VCALL(gBootMessage, 0x14, void (*)(VObject *, u32))(gBootMessage, f->c.msgSlot);
        f->msgImage = NULL;
    }
}

/* Disable (as Character), and show her message if she has one. */
void func_0019AAC0(Fiona *f) {
    func_00126910(&f->c);
    MOTION_U8(f->c.motion, 0x4D8) = 1;
    f->unk1AD630 = 0;
    f->unk1AD62E = 0;
    if (f->msgImage != NULL) {
        VCALL(gBootMessage, 0x10, void (*)(VObject *, u32, void *, s32))(gBootMessage, f->c.msgSlot, f->msgImage, 0);
    }
}

extern s32 func_00125BA0(Character *c, s32 room, s32 a2, s32 a3);
extern void func_00125BE0(Character *c);
extern void func_00184BF0(Fiona *f);

/* Put her in room `room` on triangle `tri`, idle. Returns the placement result. */
s32 func_0019A8C0(Fiona *f, s32 room, u32 tri, s32 arg3) {
    s32 r;

    func_00125BA0(&f->c, room, arg3, tri);
    r = VCALL(f, 0x28, s32 (*)(Fiona *, u32, const f32 *, f32 *))(f, tri, NULL, NULL);
    Fiona_ToIdle(f);
    return r;
}

/* Forget path/movement state, then idle. */
void func_0019AB40(Fiona *f) {
    func_00125BE0(&f->c);
    func_00184BF0(f);
    Fiona_ToIdle(f);
}

extern void func_00125CC0(Character *c);
extern VObject *D_0044E550;   /* random numbers: +0x1C -> 0..1 */

/* vtable +0x5C: activate (Character part), then reset her own state; two timers get random
 * lengths (300 + 330 * r frames, 300 + 30 * int(20 * r)). */
void func_0019AC70(Fiona *f) {
    VObject *rng;
    s32 t;

    func_00125CC0(&f->c);
    f->unk1AD580 = 0;
    FI(f, 0x1AD584, s32) = 0;
    f->unk1AD588 = 0;
    FI(f, 0x1AD58C, s32) = 6;
    FI(f, 0x1AD5D3, u8) = 1;
    f->savedYaw = f->c.a.angle[1];
    FI(f, 0x1AD5E8, s32) = 0;
    FI(f, 0x1AD5EC, s32) = 0;
    FI(f, 0x1AD5C8, s32) = 0;
    FI(f, 0x1AD5FC, u8) = 0;
    f->target = NULL;
    FI(f, 0x1AD610, s32) = 0;
    FI(f, 0x1AD614, s32) = 0;
    FI(f, 0x1AD618, s32) = 0;
    FI(f, 0x1AD61C, f32) = 1.0f;
    FI(f, 0x1AD6BC, s32) = -1;
    FI(f, 0x1AD5F0, s32) = 0;
    FI(f, 0x1AD5CC, f32) = 1.0f;
    FI(f, 0x1AD710, u8) = 1;
    FI(f, 0x1AD714, s32) = 0;
    FI(f, 0x1AD5C4, s32) = 0;
    FI(f, 0x1AD58A, u16) = 0;
    FI(f, 0x1AD5D4, u8) = 0;
    FI(f, 0x1AD5D5, u8) = 0;
    FI(f, 0x1AD5D6, u8) = 0;
    FI(f, 0x1AD5D7, u8) = 0;
    FI(f, 0x1AD5A0, s32) = 0;
    FI(f, 0x1AD5A4, s32) = 0;
    FI(f, 0x1AD5A8, s32) = 0;
    FI(f, 0x1AD5AC, s32) = 0;
    f->unk1AD630 = 0;
    f->unk1AD62E = 0;
    FI(f, 0x1AD5D8, u8) = 0;
    FI(f, 0x1AD719, u8) = 0;
    rng = D_0044E550;
    FI(f, 0x1AD728, s32) = (s32)(30.0f * (11.0f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng))) + 300;
    FI(f, 0x1AD724, s32) = FI(f, 0x1AD728, s32);
    t = (s32)(20.0f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng));
    FI(f, 0x1AD734, s32) = t * 30 + 300;
    FI(f, 0x1AD71C, s32) = 0;
    *(f32 *)&f->c.unk14C4 = -1.0f;
}

#include "sce/libvu0.h"

extern u32 func_001F4770(void *motion, s32, s32, s32);     /* animation state flags (u8) */
extern f32 *func_0017CE80(void *skeleton, s32 bone);        /* bone matrix */
extern void func_002E2DA0(f32 *out, sceVu0FMATRIX m, const f32 *v);

#define MOTION_SKELETON(m) (*(void **)((u8 *)(m) + 0x810))

/* Point of interest on her for action 8 (sub 0x1A/0x1B), e.g. for the camera: a bone or an
 * offset in front of her. False if there is none. */
s32 func_0019A450(Fiona *f, f32 *out) {
    sceVu0FVECTOR v;

    if (f->c.moveMode != 8) {
        return 0;
    }
    if (f->c.moveSub == 0x1B) {
        if ((func_001F4770(f->c.motion, 0, 0, 1) & 0xFF) & 0x2) {
            return 0;
        }
        *(s32 *)&v[0] = 0;
        v[1] = 10.0f;
        v[2] = f->c.a.radius;
        func_002E2DA0(v, f->c.a.rot, v);
        sceVu0AddVector(out, f->c.a.pos, v);
        return 1;
    }
    if (f->c.moveSub != 0x1A) {
        return 0;
    }
    if (!((func_001F4770(f->c.motion, 0, -1, 1) & 0xFF) & 0x2)) {
        return 0;
    }
    switch (Progress_GetVar(gProgress, 0x26) & 0xFF) {
    case 7:
        *(s32 *)&v[0] = 0;
        v[1] = 2.5f;
        v[2] = 10.0f;
        *(s32 *)&v[3] = 0;
        sceVu0ApplyMatrix(v, f->c.a.rot, v);
        sceVu0AddVector(out, f->c.a.pos, v);
        return 1;
    case 6: {
        sceVu0FMATRIX m;
        s32 bone = VCALL(f->c.motion, 0x78, s32 (*)(void *))(f->c.motion);

        sceVu0CopyMatrix(m, (void *)func_0017CE80(MOTION_SKELETON(f->c.motion), bone));
        v[2] = 6.0f;
        v[3] = 1.0f;
        *(s32 *)&v[0] = 0;
        *(s32 *)&v[1] = 0;
        sceVu0ApplyMatrix(out, m, v);
        out[1] = 2.5f + f->c.a.pos[1];
        return 1;
    }
    default: {
        s32 bone = VCALL(f->c.motion, 0x74, s32 (*)(void *))(f->c.motion);

        sceVu0CopyVector(out, func_0017CE80(MOTION_SKELETON(f->c.motion), bone) + 12);
        return 1;
    }
    }
}

extern VObject *D_0044E558;   /* doors: +0x40(door) -> usable */

/* Can she start interaction `kind` now (with character `otherSlot`, 0xFF = none; door `door`
 * for kind 5)? Depends on what she is doing (moveMode/moveSub). */
s32 func_0019A670(Fiona *f, u32 kind, u32 otherSlot, u32 door) {
    s32 mode;

    kind &= 0xFF;
    if (kind == 5) {
        if (!(VCALL(D_0044E558, 0x40, u32 (*)(VObject *, u32))(D_0044E558, door & 0xFF) & 0xFF)) {
            return 0;
        }
    } else if (otherSlot != 0xFF) {
        Character *o = gCharacters[otherSlot];

        if (o == NULL) {
            return 0;
        }
        if (!o->a.active && o->a.disabled == 1) {
            return 0;
        }
    }
    mode = f->c.moveMode;
    switch (kind) {
    case 1:
    case 2:
    case 4:
        return mode != 4;
    case 3: {
        Progress *p = gProgress;

        if (*((u8 *)p + 0x1FBEC1) == 1) {
            ((s32 *)p)[1] = 0;
            VCALL(p, 0x24, void (*)(Progress *, s32, s32, s32))(p, f->c.a.room, 1, 1);
        }
        return 1;
    }
    case 5:
        if (mode == 4 && f->c.moveSub == 0xB && f->c.unk104[0] == f->c.state[4]) {
            return 0;
        }
        return 1;
    case 6:
        if (mode == 4 || mode == 0xA) {
            return 0;
        }
        if (mode != 0) {
            return 1;
        }
        return f->unk1AD580 != 0xF;
    case 9:
    case 10:
        return !(mode == 4 || mode == 3 || mode == 0xA);
    case 12:
        if (mode == 4) {
            return f->c.moveSub == 0x12;
        }
        return !(mode == 3 || mode == 0xA);
    default:
        return 0;
    }
}

extern void func_00127650(Character *c);
extern void func_00127660(Character *c);
extern VObject *gFileLoader;

void func_001A4330(Fiona *f) {
}

/* vtable +0x24: remember the previous position, and her heading. */
void func_001A2CC0(Fiona *f) {
    func_00127650(&f->c);
    FI(f, 0x1AD5B8, f32) = f->c.a.angle[1];
}

/* vtable +0x20: take down her message (unkD0) and stop the animation (unkD1) if requested. */
void func_001A3E60(Fiona *f) {
    if (f->c.a.unkD0) {
        VCALL(gBootMessage, 0xC, void (*)(VObject *, u32))(gBootMessage, f->c.msgSlot);
        f->c.a.unkD0 = 0;
    }
    if (f->c.a.unkD1) {
        VCALL(f->c.motion, 0x10, void (*)(void *))(f->c.motion);
        f->c.a.unkD1 = 0;
    }
}

/* vtable +0x54: start loading her message image (file named by the animation player +0xAC). */
s32 func_001A4080(Fiona *f) {
    if (VCALL(f->c.motion, 0xAC, s32 (*)(void *))(f->c.motion) == 0) {
        return 0;
    }
    VCALL(gFileLoader, 0xC, void (*)(VObject *, s32, void *, u32, s32))(
        gFileLoader, VCALL(f->c.motion, 0xAC, s32 (*)(void *))(f->c.motion), f->msgImage,
        f->c.a.flags24 | f->c.a.slot, 0);
    return 1;
}

/* vtable +0xC: reset; her collision cylinder (radius 2, height 15) and blocking mask. */
void func_001A4340(Fiona *f) {
    func_00127660(&f->c);
    f->c.a.radius = 2.0f;
    f->c.a.height = 15.0f;
    f->c.a.navMask = 0x28020018;
    f->c.pathReq->unk4 = 6;
    f->c.pathReq->mask = f->c.a.navMask;
    f->c.hearThreshold = 0;
    FI(f, 0x1AD5F4, s32) = 0;
    FI(f, 0x1AD5F8, s32) = 0;
    FI(f, 0x1AD548, s32) = 0;
    VCALL(f, 0x5C, void (*)(Fiona *))(f);
}

extern void func_001F6AF0(void *motion);
extern VObject *D_0044FE10;

/* vtable +0x48: follow the animation (cutscene): position from the root bone, room from
 * progress, triangle from the mesh. */
void func_001A3000(Fiona *f) {
    sceVu0FMATRIX m;

    sceVu0UnitMatrix(m);
    VCALL(f->c.motion, 0x28, void (*)(void *, sceVu0FMATRIX))(f->c.motion, m);
    if (f->c.state[0] != 0) {
        f->c.state[0] = 0;
    }
    if (f->c.a.disabled) {
        return;
    }
    func_001F6AF0(f->c.motion);
    if (VCALL(D_0044FE10, 0x54, s32 (*)(VObject *, s32, s32))(D_0044FE10, 0, 0) > 0) {
        MOTION_U8(f->c.motion, 0x850) = 1;
    }
    VCALL(f->c.motion, 0x3C, void (*)(void *))(f->c.motion);
    f->c.a.room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    sceVu0CopyVector(f->c.a.pos, func_0017CE80(MOTION_SKELETON(f->c.motion), 0) + 12);
    f->c.a.navTri = VCALL(D_0044E570, 0x3C, u32 (*)(NavMesh *, f32 *, s32))(D_0044E570, f->c.a.pos, 0);
}

extern void func_00126360(Character *c);
extern void func_00187650(Fiona *f);
extern void func_001792C0(Progress *p, s32);
extern VObject *D_0044E4F8;

/* vtable +0x90: full stop - movement, interaction and the related progress flags. */
void func_0019D190(Fiona *f) {
    Progress *p;

    func_00126360(&f->c);
    FI(f, 0x1AD5FC, u8) = 0;
    func_00187650(f);
    f->unk1AD5D0 = 1;
    f->unk1AD5D1 = 1;
    FI(f, 0x1AD5F0, s32) = 0;
    FI(f, 0x1AD5C8, s32) = 0;
    p = gProgress;
    FI(f, 0x1AD5C4, s32) = 0;
    Progress_ClearFlag(p, 9);
    Progress_ClearFlag(p, 0xA);
    if (*((u8 *)p + 0x1FBEC1) == 0) {
        func_001792C0(p, 0);
    }
    FI(f, 0x1AD5A0, s32) = 0;
    FI(f, 0x1AD5A4, s32) = 0;
    FI(f, 0x1AD5A8, s32) = 0;
    FI(f, 0x1AD5AC, s32) = 0;
    VCALL(D_0044E4F8, 0x30, void (*)(VObject *, s32))(D_0044E4F8, 0);
    Progress_ClearFlag(p, 0x17);
}

extern VObject *D_0044E988;   /* +0x10 current area id (0x89: special) */
extern VObject *D_0044E4F0;   /* GS manager: +0x70 screen fade colour */

#define AREA_SPECIAL 0x89

static inline void Fiona_Fade(Fiona *f) {
    VCALL(D_0044E4F0, 0x70, void (*)(VObject *, u32))(D_0044E4F0, 0x808080 | ((u32)FI(f, 0x1AD62C, u16) << 24));
}

/* vtable +0x2C: choose the screen mode (unk152C) for this frame and pass it to the animation. */
void func_001A38E0(Fiona *f) {
    Progress *p = gProgress;
    s32 s;

    if (*((u8 *)p + 0x1FBEC1) != 0) {
        if (f->c.unkE4 == 1) {
            VCALL(f, 0x80, void (*)(Fiona *))(f);
        }
    } else {
        s = f->c.unk152C;
        if (s != 0x17 && s != 0x1E && D_0044E4F8 != NULL
            && !(VCALL(D_0044E4F8, 0x38, u32 (*)(VObject *))(D_0044E4F8) & 0xFF)) {
            if (f->c.unkE4 == 1) {
                f->c.unk152C = 0xA;
            } else if (VCALL(D_0044E988, 0x10, s32 (*)(VObject *))(D_0044E988) == AREA_SPECIAL) {
                if (FI(f, 0x1AD62C, u16) == 0) {
                    f->c.unk152C = 0x1C;
                } else {
                    f->c.unk152C = 0x1A;
                    Fiona_Fade(f);
                }
            } else {
                f->c.unk152C = 0xF;
                Fiona_Fade(f);
            }
        }
    }
    VCALL(f->c.motion, 0x38, void (*)(void *, s32, u32, s32))(f->c.motion, f->c.unk152C, f->c.a.navTri, 0);
}

extern void func_00182FC0(Fiona *f);

/* Resource table offset (from +0x1540) to pointer, 0 = none. */
#define FIONA_RES(f, off) (FI(f, off, s32) != 0 ? (void *)((u8 *)(f) + FI(f, off, s32) + 0x1540) : NULL)
#define MOTION_PTR(m, off) (*(void **)((u8 *)(m) + (off)))

/* vtable +0x1C: hook her data up to the animation player and the message display. */
void func_001A3EE0(Fiona *f) {
    void *m = f->c.motion;

    MOTION_PTR(m, 0x4C0) = FIONA_RES(f, 0x1544);
    MOTION_PTR(m, 0x4D0) = FIONA_RES(f, 0x1548);
    MOTION_PTR(m, 0x4CC) = FIONA_RES(f, 0x154C);
    MOTION_PTR(m, 0x4C4) = FIONA_RES(f, 0x1550);
    f->c.msgSlot = 0;
    if ((VCALL(gBootMessage, 0x8, u32 (*)(VObject *, u32, void *))(gBootMessage, f->c.msgSlot,
                                                                  FI(f, 0x1AD544, void *)) & 0xFF) == 1) {
        f->c.a.unkD0 = 1;
    }
    VCALL(f->c.motion, 0xC, void (*)(void *))(f->c.motion);
    f->c.a.unkD1 = 1;
    MOTION_U8(f->c.motion, 0x24) = f->c.msgSlot;
    VCALL(D_0044E558, 0x4C, void (*)(VObject *, s32))(D_0044E558, 0);
    MOTION_PTR(f->c.motion, 0x4D4) = (u8 *)f + 0x1AA540;
    func_00182FC0(f);
    if (VCALL(D_0044E988, 0x10, s32 (*)(VObject *, s32))(D_0044E988, 1) == AREA_SPECIAL) {
        f->c.unkE4 = 0;
        FI(f, 0x1AD62C, u16) = 0;
        f->unk1AD630 = 1;
        f->unk1AD62E = 0;
    }
}

extern void *func_001776B0(Progress *p, s32);

static const char sFionaMotion[] = "O_FIN\\FIN_D000.MTN";

/* LoadAsync(name, dest) for her files, tagged with her file id. */
#define Fiona_Load(f, loader, name, dest) \
    VCALL(loader, 0xC, void (*)(VObject *, const void *, void *, u32, s32))( \
        loader, name, dest, (f)->c.a.flags24 | (f)->c.a.slot, 0)

/* vtable +0x14: start loading her files: model (by costume, which comes from the unlocked
 * costume bits in the progress flags), message data, motions, animation set. */
void func_001A4110(Fiona *f) {
    Progress *p = gProgress;
    VObject *loader;
    u32 costume = 0;

    if ((((u32 *)p)[0x24 / 4] & 0x2) != 0) {
        costume = 1;
    }
    if ((((u32 *)p)[0x28 / 4] & 0x8000) != 0) {
        costume = 2;
    }
    if ((((u32 *)p)[0x2C / 4] & 0x4) != 0) {
        costume = 3;
    }
    if ((((u32 *)p)[0x2C / 4] & 0x100) != 0) {
        costume = 4;
    }
    if ((((u32 *)p)[0x2C / 4] & 0x100000) != 0) {
        costume = 5;
    }
    if ((((u32 *)p)[0x2C / 4] & 0x200000) != 0) {
        costume = 6;
    }
    loader = gFileLoader;
    FI(f, 0x1AD548, s32) = costume;
    Fiona_Load(f, loader, VCALL(f->c.motion, 0xA0, void *(*)(void *, u32))(f->c.motion, costume), (u8 *)f + 0x1540);
    FI(f, 0x1AD544, void *) = func_001776B0(p, 0);
    Fiona_Load(f, loader, VCALL(f->c.motion, 0xA8, void *(*)(void *))(f->c.motion), FI(f, 0x1AD544, void *));
    Fiona_Load(f, loader, sFionaMotion, VCALL(D_0044E558, 0x48, void *(*)(VObject *, s32))(D_0044E558, 0));
    Fiona_Load(f, loader, VCALL(f->c.motion, 0xA4, void *(*)(void *, u32))(f->c.motion, costume), (u8 *)f + 0x1AA540);
}

extern Character *gCharPartner;
extern Character *gCharPursuer;
extern void func_00126450(Character *c);

#define Character_ToIdle(c) VCALL(c, 0x7C, void (*)(Character *))(c)

/* vtable +0x8C: interrupted (e.g. a cutscene starts): stop, and release Hewie and the pursuer
 * from joint actions with her. */
void func_0019D2B0(Fiona *f) {
    Progress *p;

    func_00126450(&f->c);
    func_00184BF0(f);
    FI(f, 0x1AD58C, s32) = 6;
    FI(f, 0x1AD5D2, u8) = 0;
    p = gProgress;
    FI(f, 0x1AD5FC, u8) = 0;
    if ((Progress_TestFlag(p, 0xA) & 0xFF) == 1) {
        Progress_ClearFlag(p, 0xA);
        f->unk1AD630 = 0;
        f->unk1AD62E = 0;
    }
    if (gCharPartner != NULL && gCharPartner->a.active == 1 && !gCharPartner->unkE0) {
        s32 sub;

        if (f->c.moveMode == 0xC) {
            Character_ToIdle(gCharPartner);
        }
        if (f->c.moveMode == 0xD) {
            sub = f->c.moveSub;
            if (sub == 0x2A || sub == 0x24 || sub == 0x28 || sub == 0x2B) {
                Character_ToIdle(gCharPartner);
            }
        }
    }
    if (gCharPursuer != NULL && gCharPursuer->a.active == 1 && !gCharPursuer->unkE0) {
        if ((f->c.moveMode == 4 && f->c.moveSub == 9)
            || (gCharPursuer->moveMode == 8 && gCharPursuer->moveSub == 0x18)) {
            Character_ToIdle(gCharPursuer);
        }
    }
}

extern s32 func_00125AD0(Character *c, u32 tri, const f32 *heading, f32 *pos);
extern void func_001F1D60(void *obj);
extern void func_002DDE20(void *motion, s32 set, s32 variant);

/* Animation blend weight (motion +0x6A4 -> +0x1C), mirrored at +0x1AD628. */
static inline void Fiona_SetPose(Fiona *f, s32 set, s32 variant, f32 w) {
    func_002DDE20(f->c.motion, set, variant);
    *(f32 *)((u8 *)MOTION_PTR(f->c.motion, 0x6A4) + 0x1C) = w;
    FI(f, 0x1AD628, f32) = w;
}

/* vtable +0x28: place her (Character), reset interaction state, and on the first placement
 * pick her starting pose from her condition (+0x1AD5F4 of 100, +0x1AD5F8 of 1800 frames). */
s32 func_001A3A80(Fiona *f, u32 tri, const f32 *heading, f32 *pos) {
    s32 r = func_00125AD0(&f->c, tri, heading, pos);

    FI(f, 0x1AD5B8, f32) = f->c.a.angle[1];
    f->savedYaw = f->c.a.angle[1];
    f->unk1AD5C0 = 0;
    VCALL(f->c.motion, 0x54, void (*)(void *))(f->c.motion);
    f->unk1AD5D0 = 1;
    f->unk1AD5D1 = 1;
    FI(f, 0x1AD5DC, s32) = 0;
    FI(f, 0x1AD664, s32) = 0;
    f->targetParam = 0;
    FI(f, 0x1AD624, f32) = 1.0f;
    FI(f, 0x1AD720, u8) = 0xFF;
    FI(f, 0x1AD721, u8) = 0xFF;
    FI(f, 0x1AD72C, s32) = 30;
    FI(f, 0x1AD730, s32) = 30;
    func_001F1D60((u8 *)f + 0x1AD668);
    FI(f, 0x1AD6B8, s32) = -1;
    f->c.a.unk2A = 1;
    if (FI(f, 0x1AD5D3, u8) == 1) {
        FI(f, 0x1AD5D3, u8) = 0;
        if (FI(f, 0x1AD584, s32) & 0x2) {
            Fiona_SetPose(f, 4, -1, 1.0f);
        } else {
            f32 a = (100.0f - FI(f, 0x1AD5F4, f32)) / 60.0f;
            f32 b = (f32)(1800 - FI(f, 0x1AD5F8, s32)) / 1800.0f;

            if (b < 0.5f || !(0.25f + a <= b)) {
                Fiona_SetPose(f, 0, 3, b);
            } else if (a < 1.0f) {
                Fiona_SetPose(f, 0, 2, a);
            } else {
                Fiona_SetPose(f, 0, -1, 1.0f);
            }
        }
        Fiona_ToIdle(f);
    }
    if (f->c.unkE2 == 0) {
        MOTION_U8(f->c.motion, 0x850) = 1;
    }
    return r;
}

extern void func_001A1CA0(Fiona *f);
extern s32 func_001F1B90(void *input, void *pad);
extern u8 D_0047E3B0[];   /* pad state */
extern s32 func_001A12B0(Fiona *f);
extern void func_00185FC0(Fiona *f);
extern void func_00181F20(Fiona *f);
extern void func_001869D0(Fiona *f);
extern f32 func_00124490(Actor *a, const f32 *p);
extern void func_00177630(Progress *p, s32 level);

#define FIONA_NAV_MASK 0x28020018

/* vtable +0x44: per-frame update - controls, behaviour state, sub-systems; tells progress when
 * the pursuer is close (within 200 / 150 units). */
void func_001A2D00(Fiona *f) {
    sceVu0FVECTOR head;
    Progress *p;
    f32 h;
    u8 near, veryNear;

    f->c.a.navMask = f->c.a.unk2B ? 0 : FIONA_NAV_MASK;
    f->c.pathReq->mask = f->c.a.navMask;
    VCALL(f->c.motion, 0x60, void (*)(void *, f32 *))(f->c.motion, head);
    h = head[1] - f->c.a.pos[1];
    f->c.a.height = h;
    if (h < 3.0f) {
        f->c.a.height = 3.0f;
    }
    FI(f, 0x1AD5D4, u8) = 0;
    FI(f, 0x1AD5D5, u8) = 0;
    if (gCharPartner != NULL && gCharPartner->a.active == 1) {
        FI(f, 0x1AD5D4, u8) = 1;
        if (gCharPartner->a.disabled == 0) {
            FI(f, 0x1AD5D5, u8) = 1;
        }
    }
    FI(f, 0x1AD5D6, u8) = 0;
    FI(f, 0x1AD5D7, u8) = 0;
    if (gCharPursuer != NULL && gCharPursuer->a.active == 1) {
        FI(f, 0x1AD5D6, u8) = 1;
        if (gCharPursuer->a.disabled == 0) {
            FI(f, 0x1AD5D7, u8) = 1;
        }
    }
    func_001A1CA0(f);
    p = gProgress;
    if ((Progress_TestFlag(p, 0xD) & 0xFF) == 1 && !(Progress_TestFlag(p, 0x2B) & 0xFF)) {
        FI(f, 0x1AD6B8, s32) = func_001F1B90((u8 *)f + 0x1AD668, D_0047E3B0);
    }
    FI(f, 0x1AD6BC, s32) = -1;
    VCALL(f, 0x88, void (*)(Fiona *))(f);
    ptmf_scall(f, &f->c.a.state);
    func_001A12B0(f);
    func_00185FC0(f);
    func_00181F20(f);
    func_001869D0(f);
    VCALL(f, 0x40, void (*)(Fiona *))(f);
    near = 0;
    veryNear = 0;
    if (FI(f, 0x1AD5D7, u8) == 1 && func_00124490(&f->c.a, gCharPursuer->a.pos) <= 200.0f) {
        near = 1;
        if (func_00124490(&f->c.a, gCharPursuer->a.pos) <= 150.0f) {
            veryNear = 1;
        }
    }
    if (near == 1) {
        func_00177630(p, 5);
        if (veryNear == 1) {
            func_00177630(p, 0);
        }
    }
}

extern void func_002DCDD0(void *motion, Fiona *f, f32, f32);
extern void func_002DCB40(void *motion);
extern void func_002DC960(void *motion);

/* Group of an animation id (motion +0x55C). */
static inline s32 Fiona_AnimGroup(s32 anim) {
    switch (anim) {
    case 0x0: case 0x2: case 0x3: case 0x4: case 0x5:
        return 0;
    case 0x1:
        return 5;
    case 0x200: case 0x201: case 0x204: case 0x208: case 0x400: case 0x401: case 0x402:
        return 1;
    case 0x202: case 0x203: case 0x205: case 0x206:
        return 2;
    case 0x207:
        return 3;
    case 0xB01:
        return 4;
    case 0x1200: case 0x1201: case 0x1202: case 0x1203:
        return 6;
    case 0x700: case 0x701: case 0x702: case 0x703: case 0x704: case 0x705: case 0x706: case 0x707:
        return 7;
    case 0x708: case 0x709:
        return 8;
    case 0x403:
        return 9;
    case 0xE01:
        return 10;
    default:
        return 11;
    }
}

/* vtable +0x40: animation update - ground alignment (by animation group), advance, events. */
void func_0019BE70(Fiona *f) {
    s32 room;

    if (!f->c.a.disabled) {
        if (f->c.a.navTri == NAV_NONE) {
            func_002DCDD0(f->c.motion, f, 0.0f, 0.0f);
        } else if (FI(f, 0x1AD5BC, u8) == 1) {
            void *m = f->c.motion;
            s32 g = Fiona_AnimGroup(*(s32 *)((u8 *)m + 0x55C));

            if (g != 2 && g != 9 && g != 10 && g != 3) {
                VCALL(m, 0x40, void (*)(void *, Fiona *, f32, f32))(m, f, 11.0f, -1.0f);
            } else {
                VCALL(m, 0x40, void (*)(void *, Fiona *, f32, f32))(m, f, 5.0f, 0.5f);
            }
        } else {
            VCALL(f->c.motion, 0x40, void (*)(void *, Fiona *, f32, f32))(f->c.motion, f, 0.0f, 0.0f);
        }
    }
    func_002DCB40(f->c.motion);
    func_002DC960(f->c.motion);
    func_001F6AF0(f->c.motion);
    room = f->c.a.room;
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && f->c.a.navTri != NAV_NONE) {
        VCALL(f->c.motion, 0x4C, void (*)(void *, u32, Fiona *))(f->c.motion, FI(f, 0x1AD5BC, u8), f);
    }
    VCALL(f->c.motion, 0x3C, void (*)(void *))(f->c.motion);
}

/* Lower the +0x1AD5F8 counter (frames, of 1800) by 60 / n, not below 0. */
void func_0019A210(Fiona *f, s32 n) {
    FI(f, 0x1AD5F8, s32) -= 60 / (s16)n;
    if (FI(f, 0x1AD5F8, s32) < 0) {
        FI(f, 0x1AD5F8, s32) = 0;
    }
}

extern void func_00181010(Fiona *f, f32 angle);

/* Turn by -n/30 (forwarded to func_00181010). */
void func_0019A280(Fiona *f, s32 n) {
    func_00181010(f, -(0x1.11105ep-5f /* 0x3D08882F, ~1/30 */ * (f32)n));
}

/* Is she idle (action 0, state not 1/0xE/0xF)? */
s32 func_0019A2B0(Fiona *f) {
    s32 s;

    if (f->c.moveMode != 0) {
        return 0;
    }
    s = f->unk1AD580;
    if (s == 0xE || s == 1 || s == 0xF) {
        return 0;
    }
    return 1;
}

extern VObject *D_0044E7A8;   /* sound effects: +0x18(?, id, ?) */
extern void func_00182E80(Fiona *f);
extern const PTMF D_003B27E8;

/* Start action 4 / sub 0xA with parameter `arg` (`flag` 1: also func_00182E80). */
void func_0019A0D0(Fiona *f, u32 arg, u32 flag) {
    func_00184BF0(f);
    if ((u32)((Progress_GetVar(gProgress, 0x26) & 0xFF) - 6) < 2) {
        VCALL(f->c.motion, 0x2C, void (*)(void *))(f->c.motion);
    }
    f->c.moveMode = 4;
    f->c.moveSub = 0xA;
    VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, 0xD0, 0xC);
    f->c.unk100 = arg & 0xFF;
    if ((flag & 0xFF) == 1) {
        func_00182E80(f);
    }
    f->c.a.unk2A = 1;
    Actor_SetState(&f->c.a, &D_003B27E8);
    if (f->c.unkE0 == 1) {
        VCALL(f, 0x90, void (*)(Fiona *))(f);
    }
}

extern void func_00125A10(Character *c);
extern f32 func_00124530(Actor *a, f32 target, f32 step);
extern f32 func_0031C5C0(f32 x, f32 z);
extern void func_002DDD20(void *motion, s32 anim, s32);
extern const PTMF D_003B27F8;

/* State: turn on the spot toward the stick direction (+0x1AD570) by 10 degrees a frame;
 * back to idle if it no longer matches +0x1AD550 (dot <= 0.6). */
void func_00199ED0(Fiona *f) {
    func_00125A10(&f->c);
    if (sceVu0InnerProduct((f32 *)((u8 *)f + 0x1AD570), (f32 *)((u8 *)f + 0x1AD550)) <= 0x1.333334p-1f /* 0.6 */) {
        Fiona_ToIdle(f);
        return;
    }
    if (func_00124530(&f->c.a, func_0031C5C0(FI(f, 0x1AD570, f32), FI(f, 0x1AD578, f32)),
                      0x1.657186p-3f /* 10 deg */) == 0.0f) {
        func_002DDD20(f->c.motion, 0x1200, -1);
        Actor_SetState(&f->c.a, &D_003B27F8);
    }
}

extern s32 func_001241F0(Actor *a, Actor *b, f32 margin, f32 vmargin);
extern s32 func_00188280(Fiona *f, s32, f32 reach);
extern void func_00122C20(Actor *a, s32 id, s32 arg2, s32 arg3, s32 arg4, const f32 *pos);
extern VObject *D_0044FE08;   /* pushable objects: +0x30 blocked(id, dir), +0x3C/+0x1C/+0x20/+0x28 move */
extern const PTMF D_003B2808;  /* push: let go */
extern const PTMF D_003B2818;  /* push: moving */
extern const PTMF D_003B2828;  /* push: stop straining */

#define FIONA_STICK(f) ((f32 *)((u8 *)(f) + 0x1AD570))
#define FIONA_PUSH_OBJ(f) FI(f, 0x1AD560, s32)

/* State: pushing an object (animations 0x1200..0x1203). */
void func_001998F0(Fiona *f) {
    s32 blocked;
    void *m;

    blocked = 0;
    if (!(FI(f, 0x1AD584, s32) & 0x2)) {
        if (FI(f, 0x1AD5D7, u8) == 1 && (func_001241F0(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) == 1) {
            blocked = 1;
        } else {
            func_001241F0(&f->c.a, &gCharPartner->a, 0.0f, 0.0f);   /* (result unused) */
        }
    } else {
        blocked = 1;
    }
    if (blocked) {
        Fiona_ToIdle(f);
        return;
    }
    func_00125A10(&f->c);
    if (sceVu0InnerProduct(FIONA_STICK(f), (f32 *)((u8 *)f + 0x1AD550)) <= 0x1.333334p-1f /* 0.6 */) {
        /* stick released: let go once the animation has stopped */
        m = f->c.motion;
        if (!(*(f32 *)((u8 *)m + 0x550) <= 0.0f)) {
            return;
        }
        if (*(s32 *)((u8 *)m + 0x55C) == 0x1203) {
            func_002DDE20(m, 0x1202, -1);
            Actor_SetState(&f->c.a, &D_003B2828);
        } else {
            Fiona_ToIdle(f);
        }
        return;
    }
    m = f->c.motion;
    if ((*(s32 *)((u8 *)MOTION_PTR(m, 0x6A4) + 0x18) & 0x20) == 0) {
        /* between steps */
        if (*(s32 *)((u8 *)m + 0x55C) == 0x1203) {
            FI(f, 0x1AD5D2, u8) = 1;
        }
        return;
    }
    if (FIONA_PUSH_OBJ(f) == -1 && func_00188280(f, 1, 0x1.19999ap+1f /* 2.2 */) == -1) {
        func_002DDE20(f->c.motion, 0x1202, -1);
        Actor_SetState(&f->c.a, &D_003B2808);
        return;
    }
    if (FIONA_PUSH_OBJ(f) == -1
        || VCALL(D_0044FE08, 0x30, s32 (*)(VObject *, s32, f32 *))(D_0044FE08, FIONA_PUSH_OBJ(f), FIONA_STICK(f)) != 0) {
        /* it won't move: strain */
        FI(f, 0x1AD5D2, u8) = 1;
        func_002DDE20(f->c.motion, 0x1203, -1);
        return;
    }
    {
        VObject *objs = D_0044FE08;

        VCALL(objs, 0x3C, void (*)(VObject *, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), FIONA_STICK(f));
        VCALL(objs, 0x1C, void (*)(VObject *, s32, s32))(objs, FIONA_PUSH_OBJ(f), 0);
        VCALL(objs, 0x20, void (*)(VObject *, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), FIONA_STICK(f));
        func_002DDE20(f->c.motion, 0x1201, -1);
        VCALL(objs, 0x28, void (*)(VObject *, s32, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), 0, FIONA_STICK(f));
        func_00122C20(&f->c.a, 0x45, 5, 0, 0, NULL);
        Actor_SetState(&f->c.a, &D_003B2818);
    }
}

extern u32 func_00124480(Actor *a, const f32 *target, u32 mask);
extern const PTMF D_003B2838;  /* push: let go */

#define NAV_PUSHABLE 0x800000   /* triangle flag: an object may be pushed onto it */

static inline s32 Fiona_PushBlocked(Fiona *f) {
    if (FI(f, 0x1AD584, s32) & 0x2) {
        return 1;
    }
    if (FI(f, 0x1AD5D7, u8) == 1 && (func_001241F0(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) == 1) {
        return 1;
    }
    func_001241F0(&f->c.a, &gCharPartner->a, 0.0f, 0.0f);   /* (result unused) */
    return 0;
}

/* State: pushing an object, moving it step by step while the stick holds the direction. */
void func_001991E0(Fiona *f) {
    void *m;
    s32 anim;

    if (Fiona_PushBlocked(f)) {
        Fiona_ToIdle(f);
        return;
    }
    func_00125A10(&f->c);
    m = f->c.motion;
    anim = *(s32 *)((u8 *)m + 0x55C);
    if ((*(s32 *)((u8 *)MOTION_PTR(m, 0x6A4) + 0x18) & 0x20) != 0) {
        /* step event */
        if (anim == 0x1201) {
            VCALL(D_0044FE08, 0x2C, void (*)(VObject *, s32))(D_0044FE08, FIONA_PUSH_OBJ(f));
        }
        if (!(sceVu0InnerProduct(FIONA_STICK(f), (f32 *)((u8 *)f + 0x1AD550)) <= 0x1.333334p-1f /* 0.6 */)) {
            VObject *objs = D_0044FE08;
            s32 ok = -1;

            if (objs != NULL && f->c.moveMode == 0
                && Fiona_AnimGroup(*(s32 *)((u8 *)f->c.motion + 0x55C)) == 6) {
                sceVu0FVECTOR v, target;
                u32 tri;

                *(s32 *)&v[0] = 0;
                *(s32 *)&v[1] = 0;
                v[2] = 0x1.19999ap+1f;   /* 2.2 */
                sceVu0ApplyMatrix(v, f->c.a.rot, v);
                sceVu0AddVector(target, f->c.a.prevPos, v);
                tri = func_00124480(&f->c.a, target, 0);
                if (tri != NAV_NONE && (NavMesh_Tri(D_0044E570, tri)->flags & NAV_PUSHABLE)) {
                    ok = 0;
                }
            }
            if (ok == 0
                && VCALL(objs, 0x30, s32 (*)(VObject *, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), FIONA_STICK(f)) == 0) {
                objs = D_0044FE08;
                VCALL(objs, 0x3C, void (*)(VObject *, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), FIONA_STICK(f));
                VCALL(objs, 0x1C, void (*)(VObject *, s32, s32))(objs, FIONA_PUSH_OBJ(f), 0);
                VCALL(objs, 0x20, void (*)(VObject *, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), FIONA_STICK(f));
                func_002DDE20(f->c.motion, 0x1201, -1);
                VCALL(objs, 0x28, void (*)(VObject *, s32, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), 0, FIONA_STICK(f));
                return;
            }
            FI(f, 0x1AD5D2, u8) = 1;
            func_002DDE20(f->c.motion, 0x1203, -1);
            return;
        }
        func_002DDE20(f->c.motion, 0x1202, -1);
        Actor_SetState(&f->c.a, &D_003B2838);
    } else if (anim == 0x1203) {
        FI(f, 0x1AD5D2, u8) = 1;
    }
    if ((func_001F4770(f->c.motion, 0, 0, 1) & 0xFF) & 0x2) {
        VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, 0x40, 0x10);
    }
}

extern VObject *D_0044E568;      /* rooms */
extern VObject *D_0044E4D0;      /* room objects: +0x2C(chr) */
extern s32 func_00122B50(Actor *a, f32 *out);
extern void func_001264C0(Character *c, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void func_00126270(Character *c);
extern void func_00124890(Actor *a, s32 kind);
extern s32 func_00180D60(Fiona *f, u32 tri, f32 *pos, s32);
extern s32 func_001273D0(Character *c, u32 *triOut, f32 *posOut, f32 step);
extern const PTMF D_003B27D8;

#define Fiona_Place(f, tri, pos) VCALL(f, 0x28, s32 (*)(Fiona *, u32, const f32 *, f32 *))(f, tri, NULL, pos)
#define Room_ExitPosIn(rooms, door, out) VCALL(rooms, 0x34, u32 (*)(VObject *, u32, f32 *))(rooms, door, out)
#define Room_ExitPosOut(rooms, door, out) VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, door, out)
#define Room_DoorTo(rooms, route, room) VCALL(rooms, 0x3C, u32 (*)(VObject *, u32, s32))(rooms, route, room)
#define FIONA_ROUTE0(f) (*(u16 *)(f)->c.unk138C)

/* vtable +0x38: room (re-)entry - place her in the current room, or keep her out of it. */
void func_0019AF20(Fiona *f) {
    Progress *p = gProgress;
    VObject *rooms;
    s32 room;
    u8 ok;

    if (*((u8 *)p + 0x1FBEC1) == 0) {
        FI(f, 0x1AD5FC, u8) = 0;
        VCALL(D_0044E4D0, 0x2C, void (*)(VObject *, Fiona *))(D_0044E4D0, f);
        return;
    }
    if ((u32)((Progress_GetVar(p, 0x26) & 0xFF) - 6) < 2) {
        VCALL(f->c.motion, 0x2C, void (*)(void *))(f->c.motion);
    }
    if (FI(f, 0x1AD718, u8) == 1) {
        /* coming in through a door */
        f->c.a.room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
        rooms = D_0044E568;
        f->c.door = Room_DoorTo(rooms, FIONA_ROUTE0(f), f->c.a.room);
        if (FI(f, 0x1AD6C0, s32) == 0) {
            f->c.a.navTri = Room_ExitPosIn(rooms, f->c.door, f->c.a.pos);
        } else {
            f->c.a.navTri = Room_ExitPosOut(rooms, f->c.door, f->c.a.pos);
        }
    }
    room = f->c.a.room;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        /* not in the room being played: out of the game until she comes in */
        f->c.a.disabled = 1;
        FI(f, 0x1AD73C, f32) = 0.0f;
        FI(f, 0x1AD738, s32) = (s32)(30.0f * (2.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550))) + 90;
        return;
    }
    f->c.a.disabled = 0;
    ok = 0;
    if (f->c.a.navTri != NAV_NONE) {
        if (Fiona_Place(f, f->c.a.navTri, f->c.a.pos) == 0 || Fiona_Place(f, f->c.a.navTri, NULL) == 0) {
            ok = 1;
        }
    }
    if (!ok) {
        f->c.a.navTri = Room_ExitPosIn(D_0044E568, f->c.door, f->c.a.pos);
        if (Fiona_Place(f, f->c.a.navTri, f->c.a.pos) == 0) {
            ok = 1;
        }
    }
    if (!ok) {
        func_00124890(&f->c.a, VCALL(D_0044E568, 0x50, s32 (*)(VObject *, s32, u32, s32))(D_0044E568, f->c.a.room, f->c.door, 0));
    }
    VCALL(D_0044E4D0, 0x2C, void (*)(VObject *, Fiona *))(D_0044E4D0, f);
    if (f->c.a.navTri == NAV_NONE) {
        f->c.a.pos[0] = 0.0f;
        f->c.a.pos[1] = 0.0f;
        f->c.a.pos[2] = 0.0f;
        f->c.a.pos[3] = 0x1.99999ap-4f;   /* 0.1 */
    }
    {
        sceVu0FVECTOR v;

        if (func_00122B50(&f->c.a, v)) {
            func_001264C0(&f->c, 3, (s32)v, 0, 0, 0);
        }
    }
    func_00126270(&f->c);
    if (*((u8 *)p + 0x7B8) == 5) {
        FI(f, 0x1AD71C, s32) = 0;
        f->c.moveMode = 0xA;
        f->unk1AD580 = 0xB;
        func_002DDE20(f->c.motion, 0xB01, -1);
        Actor_SetState(&f->c.a, &D_003B27D8);
        return;
    }
    switch (FI(f, 0x1AD71C, u32)) {
    case 9:
        FI(f, 0x1AD71C, s32) = FI(f, 0x1AD719, u8) ? 1 : 0;
        break;
    case 10:
        FI(f, 0x1AD71C, s32) = 0;
        break;
    case 4:
    case 8:
        FI(f, 0x1AD71C, s32) = 3;
        FI(f, 0x1AD720, u8) = 0xFF;
        break;
    case 6:
        FI(f, 0x1AD71C, s32) = 5;
        FI(f, 0x1AD720, u8) = 0xFF;
        break;
    case 11:
    case 13:
    case 14: {
        /* walking in through the door: continue along the route */
        sceVu0FVECTOR exitPos, back, d;
        u32 exit, tri;
        f32 step;

        rooms = D_0044E568;
        exit = Room_DoorTo(rooms, FIONA_ROUTE0(f), f->c.a.room) & 0xFF;
        if (exit == 0xFF) {
            return;
        }
        if (f->c.a.navTri == NAV_NONE) {
            f->c.a.navTri = Room_ExitPosIn(rooms, f->c.door, f->c.a.pos);
        }
        if (func_00180D60(f, Room_ExitPosIn(rooms, exit, exitPos), exitPos, 0) != 0) {
            return;
        }
        tri = f->c.a.navTri;
        step = FI(f, 0x1AD73C, f32) - 5.0f;
        if (step < 0.0f) {
            step = 0.0f;
        }
        func_001273D0(&f->c, &tri, back, step);
        func_001273D0(&f->c, &f->c.a.navTri, f->c.a.pos, FI(f, 0x1AD73C, f32));
        sceVu0SubVector(d, f->c.a.pos, back);
        {
            f32 yaw = func_0031C5C0(d[0], d[2]);

            f->c.a.angle[1] = yaw;
            sceVu0UnitMatrix(f->c.a.rot);
            sceVu0RotMatrixY(f->c.a.rot, f->c.a.rot, yaw);
        }
        break;
    }
    }
}

extern s32 func_00125D80(Character *c);

#define FIONA_FADE(f) FI(f, 0x1AD62C, u16)     /* 0..0x80 */
#define FIONA_FADE_T(f) FI(f, 0x1AD62E, s16)

/* Fade handling for area 0x89, and the pursuer grabbing her once the fade has cleared. */
void func_001A1CA0(Fiona *f) {
    Progress *p = gProgress;
    s32 flags = FI(f, 0x1AD584, s32);

    if ((flags & 0x1) && (s32)*((u8 *)p + 0x7B8) < 2) {
        FI(f, 0x1AD584, s32) = flags & ~3;
    }
    if (f->unk1AD630 == 0 && f->c.unkE4 == 0) {
        FIONA_FADE(f) += 8;
        if (FIONA_FADE(f) >= 0x80) {
            f->c.unkE4 = 1;
            FIONA_FADE_T(f) = 0;
        }
    } else if (VCALL(D_0044E988, 0x10, s32 (*)(VObject *, s32))(D_0044E988, 1) != AREA_SPECIAL
               || (Progress_TestFlag(p, 0xA) & 0xFF) != 1) {
        f->unk1AD630 = 0;
        FIONA_FADE_T(f) = 0;
    } else if (f->unk1AD630 != 0) {
        FIONA_FADE(f) -= 8;
        if ((s16)FIONA_FADE(f) < 0) {
            FIONA_FADE(f) = 0;
        }
    } else if (f->c.unkE4 == 1) {
        if (++FIONA_FADE_T(f) == 60) {
            f->c.unkE4 = 0;
            f->unk1AD630 = 1;
            FIONA_FADE(f) = 0x80;
        }
    }

    if (f->unk1AD630 == 1 && FIONA_FADE(f) == 0 && FI(f, 0x1AD5D7, u8) == 1
        && gCharPursuer->a.unkC4 != 2 && !gCharPursuer->unkE0
        && (func_001241F0(&f->c.a, &gCharPursuer->a, 1.0f, 0.0f) & 0xFF) == 1
        && !(func_00125D80(&f->c) & 0xFF)) {
        if (f->c.state[0] != 7) {
            /* grabbed (the original copies a local whose last fields are never set) */
            f->c.state[0] = 4;
            f->c.state[1] = 6;
            f->c.state[2] = 2;
            f->c.state[3] = 0;
            f->c.state[4] = 3;
            *(f32 *)&f->c.state[5] = 0.0f;
            f->c.state[6] = 0;
            f->c.state[7] = 0;
        }
        f->unk1AD630 = 0;
        FIONA_FADE_T(f) = 0;
    }
}

#define RNG01() VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550)
#define HEWIE_ACTION(c) (*(s32 *)((u8 *)(c) + 0xF3564))

/* Timers: an alternating period (+0x1AD719 flips when +0x1AD724 runs out; random lengths),
 * sped up / slowed down while Hewie stays close, and three 30-frame counters. */
void func_001A1860(Fiona *f) {
    Progress *p = gProgress;
    s32 room = f->c.a.room;

    if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        if (func_00124490(&f->c.a, gCharPartner->a.pos) < 50.0f) {
            FI(f, 0x1AD730, s32) = FI(f, 0x1AD730, s32) - 1;
        } else {
            FI(f, 0x1AD730, s32) = 30;
        }
        if (FI(f, 0x1AD730, s32) == 0) {
            FI(f, 0x1AD730, s32) = 30;
            if (FI(f, 0x1AD719, u8) == 0) {
                FI(f, 0x1AD724, s32) += 15;
            } else {
                FI(f, 0x1AD724, s32) -= 15;
            }
        }
        if (FI(f, 0x1AD728, s32) < FI(f, 0x1AD724, s32)) {
            FI(f, 0x1AD724, s32) = FI(f, 0x1AD728, s32);
        }
        if (FI(f, 0x1AD71A, u8) == 0) {
            if (HEWIE_ACTION(gCharPartner) == 0x84) {
                FI(f, 0x1AD71A, u8) = 1;
                if (FI(f, 0x1AD719, u8) == 1 && RNG01() < 0.25f) {
                    FI(f, 0x1AD724, s32) = 0;
                }
            }
        } else if (HEWIE_ACTION(gCharPartner) != 0x84) {
            FI(f, 0x1AD71A, u8) = 0;
        }
    }
    if (FI(f, 0x1AD719, u8) == 0 || f->c.a.room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        FI(f, 0x1AD724, s32) -= 1;
    }
    if (FI(f, 0x1AD724, s32) <= 0) {
        if (FI(f, 0x1AD719, u8) == 0) {
            FI(f, 0x1AD719, u8) = 1;
            FI(f, 0x1AD728, s32) = (s32)(30.0f * (16.0f * RNG01())) + 150;
        } else {
            FI(f, 0x1AD719, u8) = 0;
            FI(f, 0x1AD728, s32) = (s32)(30.0f * (11.0f * RNG01())) + 300;
        }
        FI(f, 0x1AD724, s32) = FI(f, 0x1AD728, s32);
    }
    if (FI(f, 0x1AD734, s32) != 0) {
        FI(f, 0x1AD734, s32) -= 1;
    } else {
        FI(f, 0x1AD734, s32) = (s32)(20.0f * RNG01()) * 30 + 300;
    }
    if (FI(f, 0x1AD72C, s32) == 0) {
        FI(f, 0x1AD72C, s32) = 30;
    } else {
        FI(f, 0x1AD72C, s32) -= 1;
    }
}

extern u32 func_00177870(Progress *p, u32 slot);   /* joint action pending (u8) */
extern u32 func_00177850(Progress *p, u32 slot);   /* its partner's slot (u8) */
extern u32 func_00177830(Progress *p, u32 slot);   /* its kind (u8) */
extern u32 func_00177810(Progress *p, u32 slot);   /* its event type (u8) */
extern void func_001777F0(Progress *p, u32 slot);  /* accepted */
extern void func_001777D0(Progress *p, u32 slot);  /* cancelled */
extern u32 func_00124320(Actor *a, const f32 *target, u32 tri, const f32 *from, u32 mask);
extern s32 func_001235C0(void *self, Actor *a);
extern s32 func_00127140(Character *c, s32 kind, u32 goalTri, const f32 *goal);
extern u32 func_00123D20(Actor *a, const f32 *p);
extern f32 func_002E2D00(f32 angle);
extern void func_002E3130(sceVu0FMATRIX out, const f32 *pos, f32 angle);
extern void func_002E2DD0(f32 *out, sceVu0FMATRIX m, const f32 *v);

typedef struct MeetOffset {
    f32 x, z;
    f32 deg;
} MeetOffset;
extern MeetOffset D_003B2460[];
extern f32 D_003B24A8, D_003B24AC;

#define SLOT_U8(f) (*(u8 *)&(f)->c.a.slot)
#define F_PI 0x1.921fb6p+1f   /* 0x40490FDB */

/* Meeting-point table index by costume and event type (6 or other). */
static inline s32 Fiona_MeetIndex(u32 costume, u32 type) {
    switch (costume) {
    case 0: return type == 6 ? 4 : 5;
    case 1:
    case 2: return type == 6 ? 7 : 8;
    case 3: return type == 6 ? 9 : 0xB;
    case 4: return type == 6 ? 0xA : 0xC;
    case 5: return 0xD;
    case 6: return type == 6 ? 0xE : 0xF;
    default: return 0;   /* (uninitialised in the original; costumes are 0..6) */
    }
}

/* Start the joint action progress has queued for her: 0 if she is on her way, -1 if not. */
s32 func_001A12B0(Fiona *f) {
    Progress *p = gProgress;
    Progress *q;
    Character *o;
    u32 kind, type;

    if ((func_00177870(p, SLOT_U8(f)) & 0xFF) != 1) {
        return -1;
    }
    q = gProgress;
    o = gCharacters[func_00177850(q, SLOT_U8(f)) & 0xFF];
    kind = func_00177830(q, SLOT_U8(f)) & 0xFF;
    type = func_00177810(q, SLOT_U8(f)) & 0xFF;
    if (VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) == 0 && !(Progress_TestFlag(p, 8) & 0xFF)
        && o != NULL && o->a.active == 1 && o->a.disabled == 0) {
        if (kind == 1) {
            if (f->c.moveMode == 0) {
                const MeetOffset *mo = &D_003B2460[Fiona_MeetIndex(FI(f, 0x1AD548, u32), type)];
                sceVu0FMATRIX m;
                sceVu0FVECTOR v, target;
                f32 turn;
                u32 tri;

                *(s32 *)&v[1] = 0;
                *(s32 *)&v[3] = 0;
                v[0] = mo->x;
                v[2] = mo->z;
                turn = (F_PI * mo->deg) / 180.0f;
                sceVu0CopyMatrix(m, o->a.rot);
                sceVu0ApplyMatrix(v, m, v);
                sceVu0AddVector(target, o->a.pos, v);
                tri = func_00124320(&f->c.a, target, o->a.navTri, o->a.pos, NAV_NONE);
                if (tri != NAV_NONE && f->c.a.navTri == func_00124320(&f->c.a, f->c.a.pos, tri, target, NAV_NONE)
                    && (func_001235C0(f, &f->c.a) & 0xFF) == 1 && func_00127140(&f->c, 0, tri, target) > 0) {
                    FI(f, 0x1AD6F0, u32) = tri;
                    FI(f, 0x1AD6F4, f32) = func_002E2D00(turn + o->a.angle[1]);
                    sceVu0CopyVector((f32 *)((u8 *)f + 0x1AD700), target);
                    func_001777F0(p, SLOT_U8(f));
                    return 0;
                }
            }
        } else if (kind == 2 && type == 6 && f->c.moveMode == 0) {
            Character *h = gCharPartner;
            f32 base = *(f32 *)&h->unk104[2];
            sceVu0FVECTOR offs;
            s32 deg, side;

            offs[0] = D_003B24A8;
            *(s32 *)&offs[1] = 0;
            offs[2] = D_003B24AC;
            *(s32 *)&offs[3] = 0;
            for (deg = 0; deg < 181; deg += 10) {
                f32 a = F_PI * (f32)deg;

                for (side = 0;;) {
                    sceVu0FMATRIX m;
                    sceVu0FVECTOR pt;
                    f32 ang;
                    u32 tri;

                    if (side != 0) {
                        ang = func_002E2D00(base + a / 180.0f);
                    } else {
                        ang = func_002E2D00(base - a / 180.0f);
                    }
                    func_002E3130(m, f->c.a.pos, ang);
                    func_002E2DD0(pt, m, offs);
                    tri = func_00123D20(&f->c.a, pt);
                    if (tri != NAV_NONE && tri == func_00124320(&f->c.a, pt, h->a.navTri, h->a.pos, FIONA_NAV_MASK)) {
                        *(f32 *)&h->unk104[2] = ang;
                        h->unk104[0] = tri;
                        *(f32 *)&f->c.unk104[2] = func_002E2D00(F_PI + ang);
                        func_001777F0(p, SLOT_U8(f));
                        /* queue it (the original copies a local whose other fields are never set) */
                        f->c.state2[0] = 0xC;
                        f->c.state2[1] = type;
                        f->c.state2[2] = 0;
                        f->c.state2[3] = 0;
                        f->c.state2[4] = 0;
                        f->c.state2[5] = 0;
                        f->c.state2[6] = 0;
                        f->c.state2[7] = 0;
                        return 0;
                    }
                    if (deg == 0 || deg == 180 || ++side >= 2) {
                        break;
                    }
                }
            }
        }
    }
    func_001777D0(p, SLOT_U8(f));
    return -1;
}

extern void func_002DDED0(void *motion, s32 anim, s32);
extern void func_001F6370(void *motion, f32 *out, f32 t);
extern void func_00125900(Character *c);
extern void func_001247E0(Actor *a, const f32 *delta);
extern s32 func_00183190(Fiona *f);

#define MOTION_SPEED(m) (*(f32 *)((u8 *)(m) + 0x550))
#define MOTION_ANIM(m) (*(s32 *)((u8 *)(m) + 0x55C))
#define MOTION_EVENTS(m) (*(s32 *)((u8 *)MOTION_PTR(m, 0x6A4) + 0x18))

/* State: the special room entry (animations 0xB01 -> 0xB02), moving by root motion. */
void func_0019C210(Fiona *f) {
    sceVu0FVECTOR root, target;
    sceVu0FVECTOR probe = {0.0f, 0.0f, 0.0f, 0.0f};   /* (never set in the original without forward motion) */
    void *m;

    FI(f, 0x1AD5BC, u8) = 0;
    m = f->c.motion;
    if (MOTION_SPEED(m) <= 0.0f) {
        if (MOTION_ANIM(m) == 0xB01) {
            if (FI(f, 0x1AD58C, s32) == 0) {
                f->savedYaw = func_0031C5C0(FI(f, 0x1AD550, f32), FI(f, 0x1AD558, f32));
            }
            if (!(FI(f, 0x1AD584, s32) & 0x1)) {
                func_002DDED0(f->c.motion, 0xB02, -1);
            }
        } else if (MOTION_ANIM(m) == 0xB02 && (MOTION_EVENTS(m) & 0x20) != 0) {
            Fiona_ToIdle(f);
        }
    }
    func_001F6370(f->c.motion, root, 0.0f);
    if (!(root[2] <= 0.0f)) {
        f32 yaw0 = f->c.a.angle[1];

        func_00124530(&f->c.a, f->savedYaw, (F_PI * (10.0f * root[2])) / 180.0f);
        *(s32 *)&target[0] = 0;
        *(s32 *)&target[1] = 0;
        target[2] = 4.0f;
        sceVu0ApplyMatrix(probe, f->c.a.rot, target);
        sceVu0AddVector(target, f->c.a.pos, probe);
        if (func_00124480(&f->c.a, target, NAV_NONE) == NAV_NONE) {
            f->c.a.angle[1] = yaw0;
            sceVu0UnitMatrix(f->c.a.rot);
            sceVu0RotMatrixY(f->c.a.rot, f->c.a.rot, yaw0);
        }
    }
    func_00125900(&f->c);
    f->savedYaw = f->c.a.angle[1];
    sceVu0ApplyMatrix(root, f->c.a.rot, root);
    f->c.a.navMask |= 1;
    func_001247E0(&f->c.a, root);
    f->c.a.navMask &= ~1;
    sceVu0AddVector(target, f->c.a.pos, probe);
    if (func_00124480(&f->c.a, target, NAV_NONE) == NAV_NONE) {
        f->c.a.navTri = f->c.a.prevNavTri;
        sceVu0CopyVector(f->c.a.pos, f->c.a.prevPos);
    }
    if (FI(f, 0x1AD6C0, s32) < 0x97 && func_00183190(f) != 0) {
        *(s16 *)((u8 *)gProgress + 0x7BA) -= 2;
        FI(f, 0x1AD6C0, s32) += 2;
    }
}

extern s32 func_001848F0(Fiona *f, s32 cmd, s32 state);
extern u32 func_00177620(Progress *p);
extern void func_00183F10(Fiona *f);
extern void func_00183780(Fiona *f);
extern const PTMF D_003B2788;  /* panic: fall */
extern const PTMF D_003B2798;  /* panic: stumble */
extern const PTMF D_003B27A8;  /* panic attack */
extern const PTMF D_003B27B8;  /* joint action */

#define FIONA_CMD(f) FI(f, 0x1AD6B8, s32)   /* command from the controls, -1 = none */
#define FIONA_PANIC(f) (FI(f, 0x1AD584, s32) & 0x2)

/* Remember where an action started. */
static inline void Fiona_MarkActionStart(Fiona *f, s32 code) {
    f->c.moveSub = code;
    FI(f, 0x1AD6BC, s32) = code;
    FI(f, 0x1AD6D0, f32) = f->savedYaw;
    FI(f, 0x1AD6C0, u32) = f->c.a.navTri;
    sceVu0CopyVector((f32 *)((u8 *)f + 0x1AD6E0), f->c.a.pos);
}

/* Act on the controls' command: panic stumbles and panic attacks, calling Hewie, and starting
 * actions (codes from func_001848F0; 44/45 have their own setup, others are joint actions). */
void func_0019F1E0(Fiona *f) {
    s32 cmd, mode, s, code;
    u8 joint;

    if (f->c.moveMode == 0) {
        if (FIONA_PANIC(f)) {
            sceVu0FMATRIX m;
            sceVu0FVECTOR r;

            if (FI(f, 0x1AD5EC, s32) != 0) {
                FI(f, 0x1AD5EC, s32) -= 1;
            }
            func_001F6370(f->c.motion, r, 0.0f);
            func_002E3130(m, f->c.a.pos,
                          FI(f, 0x1AD58C, s32) != 0 ? f->savedYaw
                                                    : func_0031C5C0(FI(f, 0x1AD550, f32), FI(f, 0x1AD558, f32)));
            func_002E2DD0(r, m, r);
            if (func_00124480(&f->c.a, r, NAV_NONE) == NAV_NONE) {
                /* ran into something while panicking */
                func_00122C20(&f->c.a, 0x7F, 5, 0, 0, NULL);
                if (FI(f, 0x1AD5EC, s32) == 0) {
                    FI(f, 0x1AD5EC, s32) = 30;
                    if (RNG01() < 0x1.99999ap-3f /* 0.2 */) {
                        *(f32 *)((u8 *)gProgress + 0x7D8) = 1000.0f;
                        f->c.moveMode = 4;
                        f->unk1AD580 = 0xA;
                        f->c.a.unk2D = 1;
                        Actor_SetState(&f->c.a, &D_003B2788);
                        return;
                    }
                }
                f->c.moveMode = 4;
                f->unk1AD580 = 0xA;
                func_002DDED0(f->c.motion, 0x1001, -1);
                Actor_SetState(&f->c.a, &D_003B2798);
                return;
            }
        } else if (!(FI(f, 0x1AD5F4, f32) < 100.0f)) {
            /* panic attack */
            FI(f, 0x1AD6C0, s32) = 150;
            FI(f, 0x1AD5F4, f32) = 75.0f;
            f->unk1AD580 = 0xE;
            f->c.a.unk2D = 0;
            Actor_SetState(&f->c.a, &D_003B27A8);
            return;
        }
    }

    cmd = FIONA_CMD(f);
    if (cmd == -1) {
        return;
    }
    mode = f->c.moveMode;
    if (FIONA_PANIC(f) || (mode == 4 && (f->c.moveSub == 9 || f->c.moveSub == 0x12))) {
        /* call Hewie */
        if (f->c.state[0] == 0 && *((u8 *)gProgress + 0x1FBEC1) == 0 && gCharPartner->state[0] != 7) {
            /* (the original copies a local whose other fields are never set) */
            gCharPartner->state[0] = 0xD;
            gCharPartner->state[1] = 0x30;
            gCharPartner->state[2] = 0;
            gCharPartner->state[3] = 0;
            gCharPartner->state[4] = 0;
            gCharPartner->state[5] = 0;
            gCharPartner->state[6] = 0;
            gCharPartner->state[7] = 0;
        }
        func_00122C20(&f->c.a, 0x38, 5, 0, 0, NULL);
        FI(f, 0x1AD5C8, s32) = 60;
        return;
    }
    if (mode != 0) {
        return;
    }
    s = f->unk1AD580;
    if (s == 0xE || s == 1 || s == 0xF) {
        return;
    }
    code = func_001848F0(f, cmd, s);
    if (code == -1) {
        return;
    }
    joint = 1;
    if ((code == 44 || code == 45) && f->c.moveSub != 0
        && !(code == 45 && (func_00177620(gProgress) & 0xFF) == 1)) {
        Fiona_MarkActionStart(f, code);
        joint = 0;
        func_00183F10(f);
        func_00183780(f);
        if (!(Progress_TestFlag(gProgress, 0x25) & 0xFF)) {
            f->targetParam = 30;
            f->target = gCharacters[1];
        }
        FI(f, 0x1AD5C8, s32) = 60;
    }
    if (joint == 1) {
        f->c.moveMode = 0xD;
        f->unk1AD580 = 0xC;
        Fiona_MarkActionStart(f, code);
        Actor_SetState(&f->c.a, &D_003B27B8);
    }
}

extern void func_001A1860(Fiona *f);
extern void func_0019D4E0(Fiona *f);
extern void func_001800E0(Fiona *f);
extern void func_001A1FD0(Fiona *f);
extern void func_00188960(Fiona *f);
extern void func_00186180(Fiona *f);

/* Note whether Hewie / the pursuer are present (+0x1AD5D4/D6) and enabled (+0x1AD5D5/D7). */
static inline void Fiona_UpdatePresence(Fiona *f) {
    FI(f, 0x1AD5D4, u8) = 0;
    FI(f, 0x1AD5D5, u8) = 0;
    if (gCharPartner != NULL && gCharPartner->a.active == 1) {
        FI(f, 0x1AD5D4, u8) = 1;
        if (gCharPartner->a.disabled == 0) {
            FI(f, 0x1AD5D5, u8) = 1;
        }
    }
    FI(f, 0x1AD5D6, u8) = 0;
    FI(f, 0x1AD5D7, u8) = 0;
    if (gCharPursuer != NULL && gCharPursuer->a.active == 1) {
        FI(f, 0x1AD5D6, u8) = 1;
        if (gCharPursuer->a.disabled == 0) {
            FI(f, 0x1AD5D7, u8) = 1;
        }
    }
}

/* Touching: 2 = the pursuer, 1 = Hewie, 0 = neither (Hewie is always tested). */
static inline s32 Fiona_Touching(Fiona *f) {
    if (FI(f, 0x1AD5D7, u8) == 1 && (func_001241F0(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) == 1) {
        return 2;
    }
    return (func_001241F0(&f->c.a, &gCharPartner->a, 0.0f, 0.0f) & 0xFF) == 1 ? 1 : 0;
}

/* Should the controller be read this frame? */
static inline s32 Fiona_ReadsPad(Fiona *f, Progress *p) {
    s32 mode;

    if ((Progress_TestFlag(p, 0xD) & 0xFF) != 1 || (Progress_TestFlag(p, 0x2B) & 0xFF)
        || *((u8 *)p + 0x1FBEC1) != 0 || FI(f, 0x1AD5C8, s32) != 0) {
        return 0;
    }
    mode = f->c.moveMode;
    if (mode == 0xD || mode == 0xA) {
        return 1;
    }
    if (mode == 0) {
        return f->unk1AD580 != 1 && f->unk1AD580 != 0xE;
    }
    if (mode == 4) {
        return f->c.moveSub == 9 || f->c.moveSub == 0x12;
    }
    if (mode == 8) {
        return f->unk1AD580 == 5 && ((func_001F4770(f->c.motion, 0, 0, 1) & 0xFF) & 0x20);
    }
    return 0;
}

/* vtable +0x30: gameplay update - room, controls, actions, behaviour state, sub-systems. */
void func_001A3110(Fiona *f) {
    Progress *p = gProgress;
    u8 *special = (u8 *)p + 0x1FBEC1;
    sceVu0FVECTOR head;
    s32 room;
    u8 hewie, near, veryNear;
    f32 h;

    if (*special == 1) {
        func_001A1860(f);
    }
    room = f->c.a.room;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        /* not in the room being played */
        f->c.a.disabled = 1;
        f->c.moveSub = 0;
        Fiona_UpdatePresence(f);
        f->c.state[0] = 0;
        func_0019D4E0(f);
        func_001800E0(f);
        if (f->c.a.disabled == 1) {
            VCALL(f, 0x40, void (*)(Fiona *))(f);
            return;
        }
    } else {
        f->c.a.disabled = 0;
    }
    f->c.a.navMask = f->c.a.unk2B ? 0 : FIONA_NAV_MASK;
    f->c.pathReq->mask = f->c.a.navMask;
    FI(f, 0x1AD5D2, u8) = 0;
    FI(f, 0x1AD5FC, u8) = 0;
    FI(f, 0x1AD5BC, u8) = 1;
    VCALL(f->c.motion, 0x60, void (*)(void *, f32 *))(f->c.motion, head);
    h = head[1] - f->c.a.pos[1];
    f->c.a.height = h;
    if (h < 3.0f) {
        f->c.a.height = 3.0f;
    }
    Fiona_UpdatePresence(f);
    func_001A1FD0(f);
    if (*special == 1) {
        func_0019D4E0(f);
        if (f->c.a.disabled == 1) {
            f->c.state[0] = 0;
            return;
        }
        if (FI(f, 0x1AD5D7, u8) == 1 && gCharPursuer->a.unkC4 == 2) {
            f->c.a.unk2A = 1;
        }
    }
    func_00187650(f);
    FI(f, 0x1AD6BC, s32) = -1;
    if (f->c.state[0] != 0) {
        VCALL(f, 0x84, void (*)(Fiona *))(f);
    } else {
        func_0019F1E0(f);
    }
    FIONA_CMD(f) = func_001F1B90((u8 *)f + 0x1AD668, Fiona_ReadsPad(f, p) ? D_0047E3B0 : NULL);

    hewie = Fiona_Touching(f) == 1;
    ptmf_scall(f, &f->c.a.state);
    if (hewie != 1 && f->c.a.unk2A != 1) {
        func_00188960(f);
    } else if (f->c.moveMode == 0 && Fiona_Touching(f) == 0) {
        f->c.a.unk2A = 0;
    }
    func_001A12B0(f);
    func_00186180(f);
    func_00181F20(f);
    func_001869D0(f);
    VCALL(f, 0x40, void (*)(Fiona *))(f);
    if (f->c.moveMode == 0 && f->unk1AD580 != 1) {
        s32 sub = f->c.moveSub;

        if (sub != 2 && sub != 1 && sub != 0) {
            f->c.moveSub = 0;
        }
    }
    near = 0;
    veryNear = 0;
    if (FI(f, 0x1AD5D7, u8) == 1 && func_00124490(&f->c.a, gCharPursuer->a.pos) <= 200.0f) {
        near = 1;
        if (func_00124490(&f->c.a, gCharPursuer->a.pos) <= 150.0f) {
            veryNear = 1;
        }
    }
    if (near == 1) {
        func_00177630(p, 5);
        if (veryNear == 1) {
            func_00177630(p, 0);
        }
    }
    if (f->c.state[0] == 7) {
        f->c.state[0] = 0;
    }
}

extern VObject *D_0044E4B8;   /* camera: +0x68 heading */
extern void func_002E3190(sceVu0FMATRIX out, f32 angle);
extern void func_0010E640(f32 *out, const f32 *v, f32 s);   /* libvu0: scale x, y, z */
extern s32 func_00126F80(Character *c, s32 target, s32 unused2, s32 side, s32 unused4);
extern s32 func_00123C60(Actor *a, s32 room, const f32 *pos);

/* vtable +0x34: going through door `door`. In play, turn to face through it (if the stick
 * points that way, relative to the camera); in the special mode, plan the walk into the next
 * room (with Hewie if he is closer to the door). */
void func_0019B4F0(Fiona *f, s32 door) {
    Progress *p = gProgress;
    VObject *rooms;

    if (*((u8 *)p + 0x1FBEC1) == 0) {
        f32 ax, az;

        if (f->c.unkE0 == 1) {
            VCALL(f, 0x90, void (*)(Fiona *))(f);
            Fiona_ToIdle(f);
        }
        f->c.door = VCALL(D_0044E568, 0x14, u32 (*)(VObject *, s32, s32))(D_0044E568, f->c.a.room, door);
        FI(f, 0x1AD5F0, s32) = 150;
        f->unk1AD588 = 0;
        f->savedYaw = f->c.a.angle[1];
        ax = FI(f, 0x1AD5A0, f32);
        ax = (ax <= 0.0f) ? -ax : ax;
        az = FI(f, 0x1AD5A8, f32);
        az = (az <= 0.0f) ? -az : az;
        if (!(ax <= 0.5f) || !(az <= 0.5f)) {
            sceVu0FMATRIX m;
            sceVu0FVECTOR v;
            f32 h;

            func_002E3190(m, VCALL(D_0044E4B8, 0x68, f32 (*)(VObject *))(D_0044E4B8));
            func_002E2DA0(v, m, (f32 *)((u8 *)f + 0x1AD5A0));
            func_0010E640(v, v, -1.0f);
            h = func_0031C5C0(v[0], v[2]);
            if (func_002E2D00(h - f->savedYaw) < 0x1.921fb6p+0f /* pi/2 */) {
                f->unk1AD588 = 3;
                f->c.a.angle[1] = h;
                sceVu0UnitMatrix(f->c.a.rot);
                sceVu0RotMatrixY(f->c.a.rot, f->c.a.rot, h);
                f->savedYaw = f->c.a.angle[1];
            }
        }
        return;
    }

    /* special mode */
    f->c.a.unk2B = 0;
    f->c.a.navMask = FIONA_NAV_MASK;
    f->c.pathReq->mask = f->c.a.navMask;
    if (f->c.unkE0 == 1) {
        VCALL(f, 0x90, void (*)(Fiona *))(f);
        Fiona_ToIdle(f);
    }
    FI(f, 0x1AD718, u8) = 0;
    {
        s32 room = f->c.a.room;
        u32 i;

        if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
            return;
        }
        for (i = 0; i < 13; i++) {
            f->c.unk148C[i] = 0;
        }
    }
    if (f->c.moveMode == 0) {
        if (f->unk1AD580 == 0xE) {
            Fiona_ToIdle(f);
        }
    } else {
        func_00184BF0(f);
        Fiona_ToIdle(f);
    }
    if (FI(f, 0x1AD71C, s32) == 0 || FI(f, 0x1AD71C, s32) == 2) {
        sceVu0FVECTOR pos;
        u32 d = door & 0xFF;
        s32 target = 0;
        u8 through = 0;

        FI(f, 0x1AD71C, s32) = 2;
        f->c.unk124 = f->c.unk128;
        *(f32 *)&f->c.unk14C4 = -1.0f;
        if (d != 0xFF) {
            rooms = D_0044E568;
            target = VCALL(rooms, 0x18, s32 (*)(VObject *, s32, s32))(rooms, f->c.a.room, door);
            if (func_00180D60(f, Room_ExitPosIn(rooms, door, pos), pos, 1) == 0) {
                through = 1;
            }
        }
        if (!through && d != 0xFF) {
            s32 side, tgt2;

            rooms = D_0044E568;
            tgt2 = VCALL(rooms, 0x58, s32 (*)(VObject *, s32, s32, s32))(rooms, f->c.a.room, door, 0);
            side = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, f->c.a.room, f->c.door, 0);
            if (func_00126F80(&f->c, target, tgt2, side, -1) != -1
                && (Room_DoorTo(rooms, FIONA_ROUTE0(f), f->c.a.room) & 0xFF) != 0xFF) {
                func_00180D60(f, Room_ExitPosIn(rooms, door, pos), pos, 1);
            }
        }
        if (through == 1) {
            FIONA_ROUTE0(f) = VCALL(D_0044E568, 0x10, u32 (*)(VObject *, s32, s32))(D_0044E568, f->c.a.room, door);
            if ((func_00123C60(&f->c.a, door, gCharPartner->a.pos) & 0xFF) == 1) {
                FI(f, 0x1AD718, u8) = 1;
                FI(f, 0x1AD6C0, s32) = 0;
            } else {
                sceVu0FVECTOR a, b, da, db;
                f32 la;

                rooms = D_0044E568;
                Room_ExitPosOut(rooms, door, a);
                Room_ExitPosIn(rooms, door, b);
                sceVu0SubVector(da, f->c.a.pos, a);
                sceVu0SubVector(db, b, a);
                la = sceVu0InnerProduct(da, da);
                if (la <= sceVu0InnerProduct(db, db) && func_00124480(&f->c.a, a, NAV_NONE) != NAV_NONE) {
                    FI(f, 0x1AD718, u8) = 1;
                    FI(f, 0x1AD6C0, s32) = 1;
                }
            }
        }
    }
}
