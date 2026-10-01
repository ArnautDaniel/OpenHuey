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
