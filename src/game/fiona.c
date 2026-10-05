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
extern void func_001F1D60(u8 *obj);
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

extern void func_00181010(Fiona *f, f32 d);   /* change the fear by d */

/* Calm down by n/30 (the fear, func_00181010). */
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
extern s32 func_00177620(Progress *p);
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

extern s32 func_00182340(Fiona *f, s32 *state);
extern void func_00124F20(Character *c, u32 door);
extern void func_002DDC60(void *motion, s32 anim, s32 arg, s32);
extern void func_002DDBA0(void *motion, s32 anim, s32 arg);
extern s32 func_001270F0(Character *c);
extern f32 func_001244D0(Actor *a, const f32 *p);
extern const PTMF D_003B2698, D_003B26A8, D_003B26B8, D_003B26C8, D_003B26D8, D_003B26E8, D_003B26F8;
extern const PTMF D_003B2708, D_003B2718, D_003B2728, D_003B2738, D_003B2748, D_003B2758, D_003B2768;
extern const PTMF D_003B2778;
extern VObject *gSceneGameF29740;   /* path planner */

/* Characters in usable shape: present, active and enabled. */
static inline Character *Fiona_Other(u32 slot) {
    Character *o = gCharacters[slot];

    return (o != NULL && o->a.active == 1 && o->a.disabled == 0) ? o : NULL;
}

/* vtable +0x88: handle requests from outside - the Character state block (state[0]: 4 grabbed,
 * 5 released) and the pending command (+0xF4: scripted moves, animations, look-at targets). */
void func_0019F8A0(Fiona *f) {
    switch (f->c.state[0]) {
    case 0:
        break;
    case 4: {
        u32 tri = f->c.a.navTri;

        if (tri != NAV_NONE
            && (f->c.state[1] == 5 || !(NavMesh_Tri(D_0044E570, tri)->flags & FIONA_NAV_MASK))
            && func_00182340(f, f->c.state) == 0) {
            VCALL(f, 0x90, void (*)(Fiona *))(f);
            func_00124F20(&f->c, 0xFF);
            return;
        }
        f->c.state[0] = 0;
        break;
    }
    case 5:
        VCALL(f, 0x8C, void (*)(Fiona *))(f);
        f->unk1AD580 = 0;
        f->c.moveMode = 0;
        Actor_SetState(&f->c.a, f->c.state[1] == 0 ? &D_003B2698 : &D_003B26A8);
        f->c.state[0] = 0;
        break;
    default:
        f->c.state[0] = 0;
        break;
    }

    switch (f->c.unkF4) {
    case 1:
        VCALL(f, 0x90, void (*)(Fiona *))(f);
        func_00124F20(&f->c, 0xFF);
        Fiona_ToIdle(f);
        f->c.unkF4 = 0;
        return;
    case 2:
        Actor_SetState(&f->c.a, &D_003B26B8);
        break;
    case 3:
    case 4:
        f->c.unk104[0] = VCALL(D_0044E558, 0x18, s32 (*)(VObject *, u32, f32 *))(D_0044E558, *(u8 *)&f->c.unk100, f->c.a.pos);
        f->c.unk104[1] = 0;
        f->unk1AD580 = 3;
        f->c.moveMode = 2;
        if (f->c.unkF4 == 3) {
            f->c.moveSub = 0x14;
            Actor_SetState(&f->c.a, &D_003B26C8);
        } else {
            f->c.moveSub = 0x15;
            Actor_SetState(&f->c.a, &D_003B26D8);
        }
        break;
    case 5:
        f->unk1AD580 = 0x12;
        Actor_SetState(&f->c.a, f->c.unk104[1] == -1 ? &D_003B26E8 : &D_003B26F8);
        break;
    case 10:
        f->unk1AD580 = 0x13;
        Actor_SetState(&f->c.a, &D_003B26F8);
        break;
    case 6:
    case 11:
        f->unk1AD580 = (f->c.unkF4 == 6) ? 0x14 : 0x15;
        Actor_SetState(&f->c.a, &D_003B2708);
        break;
    case 7:
        f->unk1AD580 = 0x11;
        func_002DDED0(f->c.motion, f->c.unk104[0], -1);
        f->c.unkE1 = 1;
        Actor_SetState(&f->c.a, &D_003B2718);
        break;
    case 8:
        f->unk1AD580 = 0x11;
        func_002DDC60(f->c.motion, f->c.unk104[0], f->c.unk104[1], -1);
        f->c.unkE1 = 1;
        Actor_SetState(&f->c.a, &D_003B2728);
        break;
    case 9:
        f->unk1AD580 = 0x11;
        func_002DDBA0(f->c.motion, f->c.unk104[0], f->c.unk104[1]);
        f->c.unkE1 = 1;
        Actor_SetState(&f->c.a, &D_003B2738);
        break;
    case 16:
        f->unk1AD580 = 0x11;
        func_001855F0(f, f->c.unk104[1]);
        f->c.unkE1 = 1;
        Actor_SetState(&f->c.a, &D_003B2748);
        break;
    case 12:
        /* look at character unk100 (0xFF: stop) */
        if (f->c.unk100 != 0xFF) {
            Character *o = Fiona_Other(f->c.unk100);

            if (o != NULL) {
                FI(f, 0x1AD5FC, u8) = 1;
                f->target = o;
            }
        } else {
            FI(f, 0x1AD5FC, u8) = 0;
        }
        f->c.unkE1 = 1;
        break;
    case 13:
        /* look at point unk110 */
        FI(f, 0x1AD5FC, u8) = 1;
        f->target = NULL;
        sceVu0CopyVector((f32 *)((u8 *)f + 0x1AD610), f->c.unk110);
        f->c.unkE1 = 1;
        break;
    case 14: {
        /* turn to character unk100 */
        Character *o = Fiona_Other(f->c.unk100);

        if (o != NULL) {
            f->unk1AD580 = 0x16;
            FI(f, 0x1AD5FC, u8) = 0;
            f->savedYaw = func_001244D0(&f->c.a, o->a.pos);
            Actor_SetState(&f->c.a, &D_003B2758);
        } else {
            f->c.unkE1 = 1;
        }
        break;
    }
    case 15:
        /* turn to heading unk10C */
        f->unk1AD580 = 0x17;
        FI(f, 0x1AD5FC, u8) = 0;
        f->savedYaw = *(f32 *)&f->c.unk104[2];
        Actor_SetState(&f->c.a, &D_003B2768);
        break;
    case 17: {
        /* scripted walk to point unk110 with animation unk104/unk108 */
        s32 n = func_00127140(&f->c, 0, f->c.unk100, f->c.unk110);
        s32 frames;
        f32 d, w;

        if (n > 0) {
            n = func_001270F0(&f->c);
        }
        if (n <= 0) {
            f->c.unkE1 = 1;
            break;
        }
        func_002DDC60(f->c.motion, f->c.unk104[0], f->c.unk104[1], -1);
        frames = *(s32 *)((u8 *)(*(void **)((u8 *)(*(void **)((u8 *)MOTION_PTR(f->c.motion, 0x6A4) + 0x20)) + 4)) + 0xC);
        d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
            gSceneGameF29740, f->c.a.pos, f->c.unk128, f->c.unk124, f->c.unk12C);
        FI(f, 0x1AD6D0, f32) = 0x1.99999ap-4f /* 0.1 */ + d / (f32)frames;
        if (!(func_002E2D00(*(f32 *)&f->c.unk104[2] - f->c.a.angle[1]) <= 0.0f)) {
            w = func_002E2D00(*(f32 *)&f->c.unk104[2] - f->c.a.angle[1]);
        } else {
            w = -func_002E2D00(*(f32 *)&f->c.unk104[2] - f->c.a.angle[1]);
        }
        FI(f, 0x1AD6D4, f32) = 0x1.c98712p-10f /* 0.1 deg */ + w / (f32)frames;
        f->unk1AD580 = 0x18;
        Actor_SetState(&f->c.a, &D_003B2778);
        break;
    }
    }
    f->c.unkF4 = 0;
}

extern void func_002A8440(void *events, s32 level, s32 room, u32 tri, s32 exitId);   /* make a noise */
extern void func_001F6E10(void *motion);

#define THREAT_LEVEL(p) (*((u8 *)(p) + 0x7B8))     /* 0..4 */
#define FIONA_FEAR(f) FI(f, 0x1AD5F4, f32)        /* 0..100 */

static inline void Fiona_FearDown(Fiona *f, Progress *p) {
    func_00181010(f, (func_00177620(p) & 0xFF) == 2 ? -0x1.99999ap-4f /* -0.1 */ : -0x1.333334p-3f /* -0.15 */);
}

/* Full panic (threat 4): long panic, a scream (noise event 0x6F at her position). */
static inline void Fiona_FullPanic(Fiona *f, Progress *p) {
    FI(f, 0x1AD584, s32) |= 2;
    FI(f, 0x1AD5E8, s32) = (s32)(3.0f * RNG01()) * 30 + 120;
    FI(f, 0x1AD5EC, s32) = 30;
    func_002A8440((u8 *)p + 0x778, 0x6F, f->c.a.room, f->c.a.navTri, 0xFFFF);
}

/* Panic system: threat level -> panic state and duration, the fear meter, timers, and the
 * area fade / pursuer grab (as in func_001A1CA0). */
void func_001A1FD0(Fiona *f) {
    Progress *p = gProgress;
    s32 flags;
    u8 threat;

    f->c.a.unkC4 = 0;
    flags = FI(f, 0x1AD584, s32);
    threat = THREAT_LEVEL(p);
    if (!(flags & 1)) {
        if (threat >= 2) {
            FI(f, 0x1AD584, s32) |= 1;
            if (threat == 4) {
                Fiona_FullPanic(f, p);
            } else if (threat == 3) {
                FI(f, 0x1AD5E8, s32) = (s32)(4.0f * RNG01()) * 30 + 150;
            } else if (threat == 2) {
                FI(f, 0x1AD5E8, s32) = (s32)(5.0f * RNG01()) * 30 + 240;
            }
        }
    } else if (threat < 4) {
        if (threat < 2) {
            FI(f, 0x1AD5CC, f32) = 1.0f;
            FI(f, 0x1AD584, s32) &= ~3;
        } else {
            FI(f, 0x1AD584, s32) &= ~2;
        }
    } else if (threat == 4 && !(flags & 2)) {
        Fiona_FullPanic(f, p);
    }
    if (f->c.moveMode == 0 && !(FI(f, 0x1AD584, s32) & 2) && FI(f, 0x1AD5F8, s32) > 0) {
        FI(f, 0x1AD5F8, s32) -= 1;
    }

    /* fear meter */
    if (FIONA_FEAR(f) < 100.0f) {
        s32 mode = f->c.moveMode;

        if (mode == 0 && f->unk1AD580 != 0xE) {
            s32 g = Fiona_AnimGroup(MOTION_ANIM(f->c.motion));

            if (g == 3) {
                /* nothing */
            } else if (g == 2) {
                /* running */
                if (D_0044E988 == NULL || VCALL(D_0044E988, 0x10, s32 (*)(VObject *))(D_0044E988) != 0x8C) {
                    func_00181010(f, (func_00177620(p) & 0xFF) == 2 ? 0x1.111112p-4f /* 1/15 */
                                                                     : 0x1.7e4b18p-5f /* 0x3D3F258C */);
                }
            } else if (g == 1) {
                Fiona_FearDown(f, p);
            } else if (!(FI(f, 0x1AD584, s32) & 2)) {
                Fiona_FearDown(f, p);
            }
        } else if (mode == 3) {
            Fiona_AnimGroup(MOTION_ANIM(f->c.motion));
            Fiona_FearDown(f, p);
        }
    }

    /* timers */
    if (f->c.unk14D0 > 0) {
        f->c.unk14D0 -= 1;
        if (f->c.unk14D0 <= 0) {
            func_001F6E10(f->c.motion);
        }
    }
    if (FI(f, 0x1AD5F0, s32) != 0) {
        FI(f, 0x1AD5F0, s32) -= 1;
    }
    if (FI(f, 0x1AD5C8, s32) != 0) {
        FI(f, 0x1AD5C8, s32) -= 1;
    }
    if (f->c.moveMode != 0) {
        FI(f, 0x1AD5C4, s32) = 0;
    }

    /* area fade */
    if (f->unk1AD630 == 0 && f->c.unkE4 == 0) {
        FIONA_FADE(f) += 8;
        if (FIONA_FADE(f) >= 0x80) {
            f->c.unkE4 = 1;
            FIONA_FADE_T(f) = 0;
        }
    } else if (VCALL(D_0044E988, 0x10, s32 (*)(VObject *, s32))(D_0044E988, 1) == AREA_SPECIAL
               && f->c.moveMode == 0 && f->c.moveSub == 0) {
        if (f->unk1AD630 == 0) {
            if (f->c.unkE4 == 1 && ++FIONA_FADE_T(f) == 60) {
                f->c.unkE4 = 0;
                f->unk1AD630 = 1;
                FIONA_FADE(f) = 0x80;
            }
        } else {
            FIONA_FADE(f) -= 8;
            if ((s16)FIONA_FADE(f) < 0) {
                FIONA_FADE(f) = 0;
            }
        }
    } else {
        f->unk1AD630 = 0;
        FIONA_FADE_T(f) = 0;
    }
    if (f->unk1AD630 == 1 && FIONA_FADE(f) == 0 && FI(f, 0x1AD5D7, u8) == 1
        && gCharPursuer->a.unkC4 != 2 && gCharPursuer->unkE0 == 0
        && (func_001241F0(&f->c.a, &gCharPursuer->a, 1.0f, 0.0f) & 0xFF) == 1
        && !(func_00125D80(&f->c) & 0xFF)) {
        if (f->c.state[0] != 7) {
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

extern void func_00185CF0(Fiona *f);   /* walk */
extern void func_00185310(Fiona *f);   /* run */
extern const PTMF D_003B27C8;          /* start pushing */

#define FIONA_STICK_IDLE(f) FI(f, 0x1AD58C, s32)   /* frames since the stick was released, 0 = held */
#define FIONA_STICK_HEADING(f) func_0031C5C0(FI(f, 0x1AD550, f32), FI(f, 0x1AD558, f32))

/* Exhausted from running in panic: catch breath (animation 0x207), new panic run length.
 * (`g` is passed to the random call only because the original leaves it in $a2 there; the
 * callee ignores it - it keeps the differential test's argument check exact.) */
static inline void Fiona_Exhausted(Fiona *f, s32 g) {
    FI(f, 0x1AD5E8, s32) = (s32)(3.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *, s32, s32))(D_0044E550, 0, g)) * 30 + 120;
    f->unk1AD580 = 0xF;
    func_002DDED0(f->c.motion, 0x207, -1);
}

static inline void Fiona_Stand(Fiona *f) {
    FI(f, 0x1AD5C0, u32) = 0;
    func_001855F0(f, -1);
}

/* State: idle and moving by the stick - stand, rest, walk, run, panic running, turning,
 * root motion; starts pushing when she stops against an object. */
void func_0019C600(Fiona *f) {
    sceVu0FMATRIX m;
    sceVu0FVECTOR d, fwd, dir, axis, moved;
    void *mo;
    s32 g, idle;
    f32 dz;

    if (FIONA_STICK_IDLE(f) == 0) {
        FI(f, 0x1AD624, f32) = 1.0f;
    } else {
        FI(f, 0x1AD624, f32) = FI(f, 0x1AD624, f32) - 0x1.99999ap-5f /* 0.05 */;
        if (FI(f, 0x1AD624, f32) < 0.0f) {
            FI(f, 0x1AD624, f32) = 0.0f;
        }
    }
    mo = f->c.motion;
    g = Fiona_AnimGroup(MOTION_ANIM(mo));
    if (MOTION_SPEED(mo) <= 0.0f) {
        if (f->unk1AD580 == 0) {
            if (!(FI(f, 0x1AD584, s32) & 0x2)) {
                idle = FIONA_STICK_IDLE(f);
                if (idle != 0) {
                    /* stick released */
                    switch (g) {
                    case 0:
                        if ((func_00177620(gProgress) & 0xFF) == 1 && FI(f, 0x1AD584, s32) == 0
                            && FIONA_FEAR(f) < 20.0f && FI(f, 0x1AD5F8, s32) < 360) {
                            if (++FI(f, 0x1AD5C0, u32) >= 90) {
                                func_002DDED0(f->c.motion, 1, -1);   /* rest */
                            } else {
                                func_001855F0(f, -1);
                            }
                        } else {
                            Fiona_Stand(f);
                        }
                        FI(f, 0x1AD5C4, s32) = 0;
                        break;
                    case 5:
                        if ((*(s32 *)((u8 *)MOTION_PTR(mo, 0x6A4) + 0x18) & 0x20) != 0) {
                            Fiona_Stand(f);
                        }
                        FI(f, 0x1AD5C4, s32) = 0;
                        break;
                    case 1:
                        if (idle < 6) {
                            func_00185CF0(f);
                        } else {
                            Fiona_Stand(f);
                        }
                        FI(f, 0x1AD5C4, s32) = 0;
                        break;
                    case 2:
                        if (idle < 6) {
                            func_00185310(f);
                        } else {
                            FI(f, 0x1AD5C0, u32) = 0;
                            FI(f, 0x1AD5C4, s32) = 0;
                            func_001855F0(f, -1);
                        }
                        break;
                    default:
                        FI(f, 0x1AD5C0, u32) = 0;
                        FI(f, 0x1AD5C4, s32) = 0;
                        func_001855F0(f, -1);
                        break;
                    }
                } else {
                    /* stick held: walk, or run with the run button */
                    f->savedYaw = FIONA_STICK_HEADING(f);
                    if (!(Progress_TestFlag(gProgress, 0x1E) & 0xFF) && FI(f, 0x1AD5D8, u8) == 1) {
                        if (FI(f, 0x1AD584, s32) & 0x1) {
                            if (--FI(f, 0x1AD5E8, s32) >= 0) {
                                FI(f, 0x1AD5C4, s32) += 1;
                                func_00185310(f);
                            } else {
                                Fiona_Exhausted(f, g);
                                FI(f, 0x1AD5C4, s32) = 0;
                            }
                        } else {
                            FI(f, 0x1AD5C4, s32) += 1;
                            func_00185310(f);
                        }
                    } else {
                        FI(f, 0x1AD5C4, s32) = 0;
                        func_00185CF0(f);
                    }
                }
            } else {
                /* full panic: run until out of breath */
                FI(f, 0x1AD624, f32) = 1.0f;
                if (--FI(f, 0x1AD5E8, s32) >= 0) {
                    func_00185310(f);
                    if (FIONA_STICK_IDLE(f) == 0) {
                        f->savedYaw = FIONA_STICK_HEADING(f);
                    }
                } else {
                    Fiona_Exhausted(f, g);
                }
                FI(f, 0x1AD5C4, s32) = 0;
            }
        } else {
            FI(f, 0x1AD624, f32) = 1.0f;
            if ((*(s32 *)((u8 *)MOTION_PTR(f->c.motion, 0x6A4) + 0x18) & 0x20) != 0) {
                f->unk1AD580 = 0;
                if (!(FI(f, 0x1AD584, s32) & 0x2)) {
                    idle = FIONA_STICK_IDLE(f);
                    if (idle != 0) {
                        if (idle >= 6) {
                            Fiona_Stand(f);
                        }
                    } else {
                        func_00185310(f);
                    }
                } else {
                    func_00185310(f);
                }
            }
        }
    }

    func_00125900(&f->c);
    if (f->unk1AD588 == 2) {
        /* accelerating turn toward +0x1AD5E4 */
        FI(f, 0x1AD5B4, f32) = FI(f, 0x1AD5B4, f32) + 0x1.57254ep-10f /* 0x3AAB92A7 */;
        if (!(FI(f, 0x1AD5B4, f32) <= 0x1.aceea0p-5f /* 3 deg */)) {
            FI(f, 0x1AD5B4, f32) = 0x1.aceea0p-5f;
        }
        if (func_00124530(&f->c.a, FI(f, 0x1AD5E4, f32), FI(f, 0x1AD5B4, f32)) < FI(f, 0x1AD5B4, f32)) {
            f->unk1AD588 = 0;
        }
        f->savedYaw = f->c.a.angle[1];
    } else {
        func_00124530(&f->c.a, f->savedYaw, 0x1.657186p-3f /* 10 deg */);
    }

    /* root motion, scaled down when the stick points away from where she faces */
    func_001F6370(f->c.motion, d, 0.0f);
    dz = d[2] * VCALL(f->c.motion, 0x44, f32 (*)(void *, Fiona *))(f->c.motion, f);
    d[2] = dz;
    func_002E3190(m, f->savedYaw);
    func_002E2DA0(d, m, d);
    axis[2] = 1.0f;
    *(s32 *)&axis[0] = 0;
    *(s32 *)&axis[1] = 0;
    sceVu0ApplyMatrix(fwd, f->c.a.rot, axis);
    sceVu0ApplyMatrix(dir, m, axis);
    func_0010E640(d, d, ((1.0f + sceVu0InnerProduct(dir, fwd)) / 2.0f) * FI(f, 0x1AD624, f32));
    *(s32 *)&d[3] = 0;
    func_001247E0(&f->c.a, d);
    sceVu0SubVector(moved, f->c.a.pos, f->c.a.prevPos);
    if (sceVu0InnerProduct(d, moved) < 0.0f) {
        f->c.a.navTri = f->c.a.prevNavTri;
        sceVu0CopyVector(f->c.a.pos, f->c.a.prevPos);
    }
    if (!(FI(f, 0x1AD584, s32) & 0x2) && f->unk1AD580 != 0xF && MOTION_SPEED(f->c.motion) <= 0.0f
        && func_00188280(f, 0, dz) == 0) {
        f->unk1AD580 = 1;
        f->c.moveSub = 5;
        Actor_SetState(&f->c.a, &D_003B27C8);
    }
}

extern void func_001779F0(Progress *p, u32 item, u32 slot);   /* mark item seen by `slot` */
extern const PTMF D_003B25C8, D_003B25D8, D_003B25E8, D_003B25F8, D_003B2608, D_003B2618;
extern const PTMF D_003B2628, D_003B2638, D_003B2648, D_003B2658, D_003B2668, D_003B2678;
extern const PTMF D_003B2688;
extern VObject *D_0044E568;   /* rooms */

/* +0x1AD71C mood request (9 = asked by Hewie's command), +0x1AD720 its argument;
 * +0x1AD724 the period counter of the +0x1AD719 alternation. */
static inline void Fiona_NudgePeriod(Fiona *f, s32 near, s32 far) {
    if (func_00124490(&f->c.a, gCharPartner->a.pos) < 50.0f) {
        FI(f, 0x1AD724, s32) += near;
    } else if (func_00124490(&f->c.a, gCharPartner->a.pos) < 100.0f) {
        FI(f, 0x1AD724, s32) += far;
    }
}

/* vtable +0x84: handle the Character state block (state[0]: 4 grabbed, 5 released, and the
 * action requests 2/3/9 (go through a door), 8, 0xB, 0xD, 0xE; 0xC with [1] = 6) while she is free to act.
 * state[0] == 7 is left pending; everything else is consumed. */
void func_001A0370(Fiona *f) {
    s32 *st = f->c.state;

    if (st[0] == 5) {
        VCALL(f, 0x8C, void (*)(Fiona *))(f);
        f->unk1AD580 = 0;
        f->c.moveMode = 0;
        Actor_SetState(&f->c.a, st[1] == 0 ? &D_003B25C8 : &D_003B25D8);
        if ((u32)((Progress_GetVar(gProgress, 0x26) & 0xFF) - 6) < 2) {
            VCALL(f->c.motion, 0x2C, void (*)(void *))(f->c.motion);
        }
        st[0] = 0;
        return;
    }
    if (st[0] == 4) {
        if (func_00182340(f, st) == 0) {
            if ((u32)((Progress_GetVar(gProgress, 0x26) & 0xFF) - 6) < 2) {
                VCALL(f->c.motion, 0x2C, void (*)(void *))(f->c.motion);
            }
            st[0] = 0;
            return;
        }
        st[0] = 0;
    }

    if (st[0] == 0xC && st[1] == 6) {
        f->c.moveMode = 0xC;
        f->unk1AD580 = 0x10;
        Actor_SetState(&f->c.a, &D_003B25E8);
        st[0] = 0;
        return;
    }

    if (f->c.moveMode == 0 && FIONA_FEAR(f) < 100.0f && f->unk1AD580 != 0xE && f->unk1AD580 != 1
        && f->unk1AD580 != 0xF) {
        switch (st[0]) {
        case 9:
            if (func_001241F0(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) {
                st[0] = 0;
                break;
            }
            f->c.unk100 = st[2];
            f->c.moveSub = (st[1] == 0) ? 0x15 : 0x14;
            f->c.unk104[0] = VCALL(D_0044E558, 0x18, s32 (*)(VObject *, u32, f32 *))(D_0044E558, *(u8 *)&f->c.unk100, f->c.a.pos);
            f->targetParam = 0;
            Actor_SetState(&f->c.a, &D_003B25F8);
            f->unk1AD580 = 9;
            f->c.moveMode = 9;
            st[0] = 0;
            break;
        case 2: {
            VObject *rooms;
            sceVu0FVECTOR at;

            if (func_001241F0(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) {
                st[0] = 0;
                break;
            }
            f->c.unk100 = st[2];
            f->c.moveSub = (st[1] == 0) ? 0x15 : 0x14;
            rooms = D_0044E568;
            if ((VCALL(rooms, 0x74, u32 (*)(VObject *, s32, u32))(rooms, f->c.a.room, *(u8 *)&f->c.unk100) & 0xFF) == 1) {
                VCALL(rooms, 0x34, u32 (*)(VObject *, u32, f32 *))(rooms, *(u8 *)&f->c.unk100, at);
            } else {
                sceVu0CopyVector(at, f->c.a.pos);
            }
            f->c.unk104[0] = VCALL(D_0044E558, 0x18, s32 (*)(VObject *, u32, f32 *))(D_0044E558, *(u8 *)&f->c.unk100, at);
            f->c.unk104[1] = 0;
            f->targetParam = 0;
            Actor_SetState(&f->c.a, &D_003B2608);
            f->unk1AD580 = 3;
            f->c.moveMode = 2;
            st[0] = 0;
            break;
        }
        case 3:
            if (func_001241F0(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) {
                st[0] = 0;
                break;
            }
            func_001779F0(gProgress, (u8)st[2], SLOT_U8(f));
            f->c.unk100 = st[2];
            f->c.unk104[0] = st[1];
            f->c.moveSub = 6;
            f->targetParam = 0;
            Actor_SetState(&f->c.a, &D_003B2618);
            f->unk1AD580 = 2;
            f->c.moveMode = 3;
            st[0] = 0;
            break;
        case 8:
            if (!Progress_TestFlag(gProgress, 0x14)) {
                f->targetParam = 0;
                if (st[1] == 0x1A) {
                    s32 g;

                    if (f->unk1AD580 == 0xD) {
                        f->c.moveMode = 8;
                        f->unk1AD580 = 6;
                        f->c.moveSub = 0x1B;
                        Actor_SetState(&f->c.a, &D_003B2628);
                    } else if ((g = Fiona_AnimGroup(MOTION_ANIM(f->c.motion))) == 5 || g == 1 || g == 0
                               || (g == 2 && (FI(f, 0x1AD5F8, s32) != 0 || FI(f, 0x1AD5C4, u32) < 0x3D))) {
                        f->c.moveSub = st[1];
                        f->unk1AD580 = 5;
                        f->c.moveMode = 8;
                        f->c.moveSub = 0x1A;
                        Actor_SetState(&f->c.a, &D_003B2648);
                    } else if (g == 2) {
                        f->c.moveMode = 8;
                        f->unk1AD580 = 6;
                        f->c.moveSub = 0x1B;
                        Actor_SetState(&f->c.a, &D_003B2638);
                    }
                }
            }
            st[0] = 0;
            break;
        case 0xB:
            f->targetParam = 0;
            f->c.moveSub = st[1];
            switch (f->c.moveSub) {
            case 0x20:
                f->target = (st[2] == 0xFF) ? NULL : gCharacters[st[2]];
                f->c.moveMode = 0xB;
                f->unk1AD580 = 8;
                f->c.unk104[0] = 3;
                Actor_SetState(&f->c.a, &D_003B2658);
                break;
            case 0x21: {
                NavTri *t = NavMesh_Tri(D_0044E570, f->c.a.navTri);

                f->target = (st[2] == 0xFF) ? NULL : gCharacters[st[2]];
                f->c.moveMode = 0xB;
                f->unk1AD580 = 8;
                f->c.unk104[0] = (t->flags & 0x80001) ? 3 : 1;
                Actor_SetState(&f->c.a, &D_003B2668);
                break;
            }
            case 0x22: {
                Progress *p = gProgress;

                if (!Progress_TestFlag(p, 0x14) && FI(f, 0x1AD5F8, s32) < 360 && THREAT_LEVEL(p) < 4) {
                    f->c.a.unk2D = 1;
                    f->c.moveMode = 0xB;
                    f->unk1AD580 = 0xD;
                    Actor_SetState(&f->c.a, &D_003B2678);
                }
                break;
            }
            }
            st[0] = 0;
            break;
        case 0xE:
            if (!Progress_TestFlag(gProgress, 0x14)) {
                f->targetParam = 0;
                f->c.moveSub = st[1];
                if (st[2] != -1 && (f->c.moveSub == 0x33 || f->c.moveSub == 0x32 || f->c.moveSub == 0x31)) {
                    f->c.moveMode = 0xE;
                    f->unk1AD580 = 7;
                    FI(f, 0x1AD6C0, s32) = 0;
                    f->c.unk100 = st[2];
                    Actor_SetState(&f->c.a, &D_003B2688);
                }
            }
            st[0] = 0;
            break;
        case 0xD:
            if (st[1] == 0) {
                if (!f->c.a.disabled && FI(f, 0x1AD71C, s32) != 3 && FI(f, 0x1AD71C, s32) != 4
                    && FI(f, 0x1AD71C, s32) != 9 && FI(f, 0x1AD719, u8) == 0) {
                    FI(f, 0x1AD71C, s32) = 9;
                    FI(f, 0x1AD720, u8) = st[2];
                }
            } else if (st[1] == 1) {
                if (!f->c.a.disabled) {
                    if (FI(f, 0x1AD719, u8) == 0) {
                        Fiona_NudgePeriod(f, 150, 90);
                    } else {
                        Fiona_NudgePeriod(f, -150, -90);
                    }
                }
            }
            st[0] = 0;
            break;
        }
    }
    if (st[0] != 7) {
        st[0] = 0;
    }
}

extern u32 func_00178610(Progress *p, u32 route);               /* u8 */
extern u32 func_00178840(Progress *p, s32 room, u32 exit);      /* u8 */
extern u32 func_00178200(Progress *p, u32 route, u32 slot);     /* u8 */
extern u32 func_00178980(Progress *p, s32 room, u32 exit);      /* u8 */
extern u32 func_00177BF0(Progress *p, u32 i, u32 slot);         /* u8 flags */

#define FIONA_MOOD(f) FI(f, 0x1AD71C, s32)
#define FIONA_EXIT(f) FI(f, 0x1AD720, u8)    /* exit the mood is about, 0xFF = none yet */
#define FIONA_EXIT2(f) FI(f, 0x1AD721, u8)
#define FIONA_AWAY_T(f) FI(f, 0x1AD738, s32)

#define Room_Side(r, room, door) VCALL(r, 0x50, s32 (*)(VObject *, s32, u32, s32))(r, room, door, 0)
#define Room_ExitTo(r, room, i) VCALL(r, 0x18, s32 (*)(VObject *, s32, u32))(r, room, i)
#define Room_ExitToSide(r, room, i) VCALL(r, 0x58, s32 (*)(VObject *, s32, u32, s32))(r, room, i, 0)
#define Room_ExitDoor(r, room, i) (VCALL(r, 0x14, u32 (*)(VObject *, s32, u32))(r, room, i) & 0xFF)
#define Room_ExitRoute(r, room, i) VCALL(r, 0x10, u32 (*)(VObject *, s32, u32))(r, room, i)
#define Room_ExitOpen(r, room, i) (VCALL(r, 0x74, u32 (*)(VObject *, s32, u32))(r, room, i) & 0xFF)
#define Room_Exit70(r, room, i) (VCALL(r, 0x70, u32 (*)(VObject *, s32, u32))(r, room, i) & 0xFF)
#define Room_Exit78(r, room, i) (VCALL(r, 0x78, u32 (*)(VObject *, s32, u32))(r, room, i) & 0xFF)

/* Exit triangle flags that rule an exit out from side `side` of the room. */
static inline s32 Fiona_WrongSide(s32 side, u32 tri) {
    u32 fl = NavMesh_Tri(D_0044E570, tri)->flags & 0x300000;

    return (side == 0 && fl == 0x100000) || (side == 1 && fl == 0x200000);
}

/* Whether exit `i` may be used: open, route not locked, allowed for her. */
static inline s32 Fiona_ExitUsable(Fiona *f, Progress *p, VObject *rooms, u32 i) {
    u32 route;

    if (Room_ExitOpen(rooms, f->c.a.room, i) != 1) {
        return 0;
    }
    route = Room_ExitRoute(rooms, f->c.a.room, i) & 0xFFFF;
    if (func_00178610(p, route) & 0xFF) {
        return 0;
    }
    if (func_00178840(p, f->c.a.room, i) & 0xFF) {
        return 0;
    }
    return (func_00178200(p, route, SLOT_U8(f)) & 0xFF) == 1;
}

/* Walking distance estimate from `from` to `to`: horizontal distance + 3 * height difference. */
static inline f32 Fiona_ExitScore(f32 *d, const f32 *from, f32 *to) {
    f32 dy;

    sceVu0SubVector(d, (f32 *)from, to);
    dy = d[1];
    dy = (dy <= 0.0f) ? -dy : dy;
    return __builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) + 3.0f * dy;
}

/* Post a "go through exit" request (state[0] = 2) into her own state block, if it is free. */
static inline s32 Fiona_PostExit(Fiona *f, u32 exit) {
    s32 *st = f->c.state;

    if (st[0] != 0) {
        return 0;
    }
    if (st[0] != 7) {
        st[0] = 2;
        st[1] = 1;
        st[2] = exit;
        st[3] = 0;
        st[4] = 0;
        st[5] = 0;
        st[6] = 0;
        st[7] = 0;
    }
    return 1;
}

static inline f32 Fiona_Dist(Fiona *f, const f32 *p) {
    return func_00124490(&f->c.a, p);
}

/* Mood/flight AI (+0x1AD71C): 0 calm near Hewie, 1 waiting, 2 following Hewie, 3/4 fleeing
 * through the best exit (away from the pursuer), 5..8 leaving through some exit, 9 a given exit,
 * 10 caught; 0xB..0xE the same while she is not in the room being played. */
void func_0019D4E0(Fiona *f) {
    Progress *p = gProgress;
    s32 room = f->c.a.room;
    VObject *rooms;
    /* (separate vectors, as in the original: out-parameters keep their own leftovers) */
    sceVu0FVECTOR at, in, out, out2, d;
    u32 tri;

    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        /* not in the room being played */
        switch (FIONA_MOOD(f)) {
        case 0: case 2: case 7: case 8: case 9:
            FIONA_MOOD(f) = 0xB;
            break;
        case 1: case 10:
            FIONA_MOOD(f) = 0xC;
            break;
        case 3: case 4:
            FIONA_MOOD(f) = 0xE;
            break;
        case 5: case 6:
            FIONA_MOOD(f) = 0xD;
            break;
        }
        if (FIONA_PANIC(f) && FIONA_MOOD(f) != 0xE) {
            FIONA_MOOD(f) = 0xE;
        }
        if (FI(f, 0x1AD719, u8) == 0) {
            if (FIONA_MOOD(f) == 0xC) {
                FIONA_MOOD(f) = 0xB;
            }
        } else if (FIONA_MOOD(f) == 0xC) {
            FIONA_AWAY_T(f) -= 1;
            if (FIONA_AWAY_T(f) < 0) {
                FIONA_MOOD(f) = 0xD;
                f->c.unk124 = f->c.unk128;
                *(f32 *)&f->c.unk14C4 = -1.0f;
            }
        } else {
            FIONA_AWAY_T(f) = (s32)(30.0f * (2.0f * RNG01())) + 90;
        }
        return;
    }

    switch (FIONA_MOOD(f)) {
    case 0xB:
        FIONA_MOOD(f) = 0;
        break;
    case 0xC:
        FIONA_MOOD(f) = 1;
        break;
    case 0xD:
        FIONA_MOOD(f) = 5;
        FIONA_EXIT(f) = 0xFF;
        break;
    case 0xE: {
        u8 found = 0;

        if (gCharPursuer != NULL && gCharPursuer->a.active == 1 && gCharPursuer->a.room != room) {
            s32 pr = gCharPursuer->a.room;
            s32 ps, fs;

            rooms = D_0044E568;
            ps = Room_Side(rooms, pr, gCharPursuer->door);
            fs = Room_Side(rooms, f->c.a.room, f->c.door);
            if (func_00126F80(&f->c, pr, ps, fs, 1) != -1) {
                found = 1;
                f->c.unk14C0 = FIONA_ROUTE0(f);
                FIONA_MOOD(f) = 3;
                FIONA_EXIT(f) = 0xFF;
            }
        }
        if (!found) {
            if (FI(f, 0x1AD719, u8) == 0) {
                FIONA_MOOD(f) = 0;
            } else if (RNG01() < 0.5f) {
                FIONA_MOOD(f) = 1;
            } else {
                FIONA_MOOD(f) = 5;
                FIONA_EXIT(f) = 0xFF;
            }
        }
        break;
    }
    }

    if (f->c.moveMode == 2) {
        return;
    }
    if (f->c.moveMode != 0) {
        FIONA_MOOD(f) = 0;
        return;
    }
    if (FIONA_PANIC(f) && FIONA_MOOD(f) != 3 && FIONA_MOOD(f) != 4) {
        FIONA_MOOD(f) = 3;
        FIONA_EXIT(f) = 0xFF;
    }

    /* what Hewie should do (+0x1AD6B8: 0 stay, 1 come, 2 ?) */
    if ((u32)FIONA_MOOD(f) < 11 && FI(f, 0x1AD71A, u8) == 0 && FI(f, 0x1AD734, s32) == 0) {
        if (FI(f, 0x1AD5D7, u8) == 1) {
            if (Fiona_Dist(f, gCharPursuer->a.pos) < 50.0f) {
                FIONA_CMD(f) = 0;
            } else if (!(Fiona_Dist(f, gCharPartner->a.pos) <= 100.0f)) {
                FIONA_CMD(f) = 1;
            }
        } else if (Fiona_Dist(f, gCharPartner->a.pos) < 50.0f) {
            FIONA_CMD(f) = (RNG01() < 0.5f) ? 0 : 2;
        } else if (!(Fiona_Dist(f, gCharPartner->a.pos) <= 100.0f)) {
            FIONA_CMD(f) = 1;
        }
    }

    /* the pursuer is in sight: run, more likely the higher the threat (gProgress +0x7BC) */
    if (FI(f, 0x1AD72C, s32) == 0 && FIONA_MOOD(f) != 3 && FIONA_MOOD(f) != 4 && FI(f, 0x1AD5D7, u8) == 1) {
        s32 t = (s32)*(f32 *)((u8 *)p + 0x7BC);
        u8 flee = 0;

        if (t > 80) {
            flee = 1;
        } else if (t > 60) {
            if (RNG01() < 0x1.99999a0000000p-3f /* 0.2 */) flee = 1;
        } else if (t > 40) {
            if (RNG01() < 0x1.3333340000000p-3f /* 0.15 */) flee = 1;
        } else if (t > 20) {
            if (RNG01() < 0x1.99999a0000000p-4f /* 0.1 */) flee = 1;
        } else if (t > 10) {
            if (RNG01() < 0x1.99999a0000000p-5f /* 0.05 */) flee = 1;
        }
        if (flee == 1) {
            FIONA_MOOD(f) = 3;
            FIONA_EXIT(f) = 0xFF;
        }
    }

    switch (FIONA_MOOD(f)) {
    case 3: case 4: case 7: case 8: case 10:
        break;
    default:
        if (FI(f, 0x1AD719, u8) == 1) {
            if (FIONA_MOOD(f) == 0 || FIONA_MOOD(f) == 2) {
                if (RNG01() < 0.5f) {
                    FIONA_MOOD(f) = 1;
                } else {
                    FIONA_MOOD(f) = 5;
                    FIONA_EXIT(f) = 0xFF;
                }
            }
        } else if (FIONA_MOOD(f) != 0 && FIONA_MOOD(f) != 2 && FIONA_MOOD(f) != 9) {
            FIONA_MOOD(f) = 0;
        }
        break;
    }

    switch (FIONA_MOOD(f)) {
    case 0:
        if (!(Fiona_Dist(f, gCharPartner->a.pos) <= 50.0f)) {
            FIONA_MOOD(f) = 2;
        }
        break;
    case 1:
        break;
    case 2:
        if (func_00180D60(f, gCharPartner->a.navTri, gCharPartner->a.pos, 0) != 0) {
            FIONA_MOOD(f) = 7;
            FIONA_EXIT(f) = 0xFF;
            f->c.unk124 = f->c.unk128;
        } else if (Fiona_Dist(f, gCharPartner->a.pos) < 30.0f) {
            FIONA_MOOD(f) = 0;
        }
        break;

    case 3:
        rooms = D_0044E568;
        if (FIONA_EXIT(f) != 0xFF) {
            tri = Room_ExitPosIn(rooms, FIONA_EXIT(f), at);
            if (func_00180D60(f, tri, at, 0) != 0) {
                FIONA_MOOD(f) = 0;
                break;
            }
            if (Fiona_Dist(f, at) < 1.0f) {
                if (!(func_00178980(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF) && !Fiona_PostExit(f, FIONA_EXIT(f))) {
                    FIONA_MOOD(f) = 0;
                    break;
                }
                FIONA_MOOD(f) = 4;
                FIONA_EXIT2(f) = FIONA_EXIT(f);
            }
            break;
        }
        {
            /* pick the best exit: the nearest, preferring ones she reaches before the pursuer */
            s32 side = Room_Side(rooms, f->c.a.room, f->c.door);
            u8 skip = 0;
            u8 beatsHim = 0;
            f32 best = 0.0f;
            u32 i;

            if (FI(f, 0x1AD5D6, u8) == 1 && FI(f, 0x1AD5D7, u8) == 0) {
                s32 pr = gCharPursuer->a.room;
                s32 ps = Room_Side(rooms, pr, gCharPursuer->door);

                /* not the exits into the pursuer's room on his side */
                for (i = 0; i < 8; i = (i + 1) & 0xFF) {
                    if (Room_ExitTo(rooms, f->c.a.room, i) == pr && Room_ExitToSide(rooms, f->c.a.room, i) == ps) {
                        skip |= 1 << i;
                    }
                }
            }
            tri = 0;
            for (i = 0; i < 8; i = (i + 1) & 0xFF) {
                u32 t;
                f32 score;
                u8 take;

                if (skip & (1 << i)) {
                    continue;
                }
                if (!Fiona_ExitUsable(f, p, rooms, i)) {
                    continue;
                }
                t = Room_ExitPosIn(rooms, i, in);
                if (Fiona_WrongSide(side, t)) {
                    continue;
                }
                take = 0;
                Room_ExitPosOut(rooms, i, out);
                score = Fiona_ExitScore(d, f->c.a.pos, out);
                if (FI(f, 0x1AD5D7, u8) == 1) {
                    u8 first = score < Fiona_ExitScore(d, gCharPursuer->a.pos, out);

                    if (FIONA_EXIT(f) == 0xFF) {
                        beatsHim = first;
                        take = 1;
                    } else if (beatsHim == 0) {
                        if (first == 1) {
                            beatsHim = 1;
                            take = 1;
                        } else if (score < best) {
                            take = 1;
                        }
                    } else if (first == 1 && score < best) {
                        take = 1;
                    }
                } else if (FIONA_EXIT(f) == 0xFF || score < best) {
                    take = 1;
                }
                if (take == 1) {
                    FIONA_EXIT(f) = i;
                    tri = t;
                    sceVu0CopyVector(at, in);
                    best = score;
                }
            }
        }
        if (FIONA_EXIT(f) == 0xFF) {
            FIONA_MOOD(f) = 0;
            break;
        }
        if (Room_Exit78(rooms, f->c.a.room, FIONA_EXIT(f)) == 0) {
            FIONA_MOOD(f) = 4;
            FIONA_EXIT2(f) = FIONA_EXIT(f);
        }
        if (func_00180D60(f, tri, at, 0) != 0) {
            FIONA_EXIT(f) = 0xFF;
            FIONA_MOOD(f) = 0;
            break;
        }
        FIONA_ROUTE0(f) = Room_ExitRoute(rooms, f->c.a.room, FIONA_EXIT(f));
        if ((func_00178980(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF) == 1) {
            Room_ExitPosOut(rooms, FIONA_EXIT(f), out2);
            if (Fiona_Dist(f, out2) < Fiona_Dist(f, at)) {
                FIONA_MOOD(f) = 4;
                FIONA_EXIT2(f) = FIONA_EXIT(f);
            }
        }
        break;

    case 5: case 7:
        rooms = D_0044E568;
        if (FIONA_EXIT(f) != 0xFF) {
            tri = Room_ExitPosIn(rooms, FIONA_EXIT(f), at);
            if (func_00180D60(f, tri, at, 0) != 0) {
                FIONA_MOOD(f) = 0;
                break;
            }
            if (Fiona_Dist(f, at) < 1.0f) {
                if (!(func_00178980(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF) && !Fiona_PostExit(f, FIONA_EXIT(f))) {
                    FIONA_MOOD(f) = 0;
                    break;
                }
                FIONA_MOOD(f) = (FIONA_MOOD(f) == 5) ? 6 : 8;
                FIONA_EXIT2(f) = FIONA_EXIT(f);
            }
            break;
        }
        {
            /* pick a usable exit at random (other than the one she came through) */
            s32 side = Room_Side(rooms, f->c.a.room, f->c.door);
            u8 tried = (u8)(1 << (f->c.door & 0x1F));

            tri = 0;
            while (tried != 0xFF) {
                u32 i = (u8)(u32)(8.0f * RNG01());
                u32 bit = 1 << (i & 0x1F);

                if (tried & bit) {
                    continue;
                }
                tried |= (u8)bit;
                if (!Fiona_ExitUsable(f, p, rooms, i)) {
                    continue;
                }
                tri = Room_ExitPosIn(rooms, i, at);
                if (Fiona_WrongSide(side, tri)) {
                    continue;
                }
                FIONA_EXIT(f) = i;
                break;
            }
        }
        if (FIONA_EXIT(f) == 0xFF) {
            FIONA_EXIT(f) = f->c.door;
            tri = Room_ExitPosIn(rooms, FIONA_EXIT(f), at);
        }
        if (FIONA_EXIT(f) == 0xFF) {
            FIONA_MOOD(f) = 0;
            break;
        }
        if (Room_Exit78(rooms, f->c.a.room, FIONA_EXIT(f)) == 0) {
            FIONA_MOOD(f) = (FIONA_MOOD(f) == 5) ? 6 : 8;
            FIONA_EXIT2(f) = FIONA_EXIT(f);
        }
        if (func_00180D60(f, tri, at, 0) != 0) {
            FIONA_MOOD(f) = 0;
            break;
        }
        FIONA_ROUTE0(f) = Room_ExitRoute(rooms, f->c.a.room, FIONA_EXIT(f));
        if ((func_00178980(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF) == 1) {
            Room_ExitPosOut(rooms, FIONA_EXIT(f), out2);
            if (Fiona_Dist(f, out2) < Fiona_Dist(f, at)) {
                FIONA_MOOD(f) = (FIONA_MOOD(f) == 5) ? 6 : 8;
                FIONA_EXIT2(f) = FIONA_EXIT(f);
            }
        }
        break;

    case 4: case 6: case 8:
        /* at / through the exit */
        rooms = D_0044E568;
        if (FIONA_EXIT2(f) == 0xFF) {
            tri = Room_ExitPosOut(rooms, FIONA_EXIT(f), at);
            if (func_00180D60(f, tri, at, 0) != 0) {
                FIONA_MOOD(f) = 0;
                break;
            }
            FIONA_ROUTE0(f) = Room_ExitRoute(rooms, f->c.a.room, FIONA_EXIT(f));
            f->c.unk14C0 = FIONA_ROUTE0(f);
            break;
        }
        tri = Room_ExitPosOut(rooms, FIONA_EXIT(f), at);
        if (Fiona_Dist(f, at) < 1.0f) {
            u32 door = Room_ExitDoor(rooms, f->c.a.room, FIONA_EXIT(f));

            if (door != 0xFF) {
                u32 i;

                /* through: she is now in the next room, out of sight */
                for (i = 0; i < 13; i++) {
                    f->c.unk148C[i] = 0;
                }
                rooms = D_0044E568;
                f->c.a.room = Room_ExitTo(rooms, f->c.a.room, FIONA_EXIT(f));
                f->c.door = door;
                f->c.a.navTri = NAV_NONE;
                f->c.a.disabled = 1;
                f->c.unk124 = f->c.unk128;
                FIONA_ROUTE0(f) = Room_ExitRoute(rooms, f->c.a.room, f->c.door);
                f->c.unk14C0 = FIONA_ROUTE0(f);
                *(f32 *)&f->c.unk14C4 = (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, FIONA_ROUTE0(f), f->c.a.room);
                FI(f, 0x1AD6C0, s32) = 1;
                FI(f, 0x1AD73C, s32) = 0;
                FIONA_AWAY_T(f) = (s32)(30.0f * (2.0f * RNG01())) + 90;
                break;
            }
        }
        if (func_00180D60(f, tri, at, 0) != 0) {
            FIONA_MOOD(f) = 0;
        }
        break;

    case 9: {
        s32 side;

        if (FIONA_EXIT(f) == 0xFF) {
            FIONA_MOOD(f) = 0;
            break;
        }
        rooms = D_0044E568;
        side = Room_Side(rooms, f->c.a.room, f->c.door);
        if (Room_Exit78(rooms, f->c.a.room, FIONA_EXIT(f)) != 1 || Room_Exit70(rooms, f->c.a.room, FIONA_EXIT(f)) != 0
            || (func_00178980(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF)) {
            FIONA_MOOD(f) = 0;
            break;
        }
        tri = Room_ExitPosIn(rooms, FIONA_EXIT(f), at);
        if (Fiona_WrongSide(side, tri) || func_00180D60(f, tri, at, 0) != 0) {
            FIONA_MOOD(f) = 0;
            break;
        }
        if (!(func_00177BF0(p, FIONA_EXIT(f), 0) & 0x4)) {
            break;
        }
        if (func_00178980(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF) {
            break;
        }
        Fiona_PostExit(f, FIONA_EXIT(f));
        FIONA_MOOD(f) = 0;
        break;
    }

    case 10:
        if (FI(f, 0x1AD5D7, u8) == 0 || gCharPursuer->a.unkC4 == 2) {
            FIONA_MOOD(f) = 0;
        } else {
            f32 r = gCharPursuer->a.radius;

            if (!(Fiona_Dist(f, gCharPursuer->a.pos) <= 1.5f * (f->c.a.radius + r)) || FIONA_PANIC(f)) {
                FIONA_MOOD(f) = 0;
            }
        }
        break;
    }
}

extern VObject *D_0044E558;   /* the doors */
extern VObject *D_0044E568;   /* the rooms */
extern void func_00178C10(Progress *p, s32 room, s32 door, s32 arg);
extern void func_00178A90(Progress *p, s32 room, s32 door, s32 arg);
extern void func_001779C0(Progress *p, s32 door, s32 slot);

#define DOOR_ISOPEN(d, door) VCALL(d, 0x28, s32 (*)(VObject *, s32))(d, door)
#define DOOR_KIND(d, door) VCALL(d, 0x30, s32 (*)(VObject *, s32))(d, door)
#define DOOR_SET(d, slot, door, side, v) VCALL(d, slot, void (*)(VObject *, s32, s32, s32))(d, door, side, v)
#define ROOM_DOOR_LINK(r, room, door) VCALL(r, 0x10, s32 (*)(VObject *, s32, s32))(r, room, door)

extern u8 *D_0044F258;   /* the creatures: 10 slots */

#define FI_HEWIE_NEAR(f) FI(f, 0x1AD5D5, u8)   /* 1: Hewie is with Fiona */

/* |the heading from Fiona to p, relative to hers| (the original wraps it once per use) */
static f32 fiona_turn_to(Fiona *f, f32 a) {
    if (func_002E2D00(a - f->c.a.angle[1]) <= 0.0f) {
        return -func_002E2D00(a - f->c.a.angle[1]);
    }
    return func_002E2D00(a - f->c.a.angle[1]);
}

/* the call command's action: 0x2D to a creature within 10 ahead (Fiona in control state 1, the
 * creature in her room and reachable, within 45 degrees), with Hewie controlled (2) 0x2E when he
 * is with her and in state 8, else 0x2D; otherwise 0x23 */
s32 func_00184700(Fiona *f) {
    u8 who = func_00177620(gProgress);
    s32 i;

    if (who == 2) {
        if (FI_HEWIE_NEAR(f) == 1 && AT(gCharPartner, 0xF8, s32) == 8) {
            return 0x2E;
        }
        return 0x2D;
    }
    if (who == 1) {
        for (i = 0; i < 10; i++) {
            Actor *c = AT(D_0044F258, i * 4, Actor *);
            f32 d[4] __attribute__((aligned(16)));
            u32 tri;

            if (c == NULL || c->active != 1 || f->c.a.room != c->room) {
                continue;
            }
            sceVu0SubVector(d, c->pos, f->c.a.pos);
            if (!(__builtin_sqrtf(__builtin_fabsf(sceVu0InnerProduct(d, d))) < 10.0f)) {
                continue;
            }
            tri = c->navTri;
            if (func_00124480(&f->c.a, c->pos, NAV_NONE) != tri) {
                continue;
            }
            if (fiona_turn_to(f, func_0031C5C0(d[0], d[2])) < 0x1.921fb60000000p-1f /* 0.7853982 */) {
                return 0x2D;
            }
        }
    }
    return 0x23;
}

/* the action code for Hewie command `cmd` from the controls: 0 call (func_00184700), 1 0x2C,
 * 2 (stay / come) 0x24 / 0x25 when he is with her within 12 (by his state), 0x27 from further
 * or when not in control, 3 0x2A / 0x28 within 15 (0x2A: his +0xC4 state 2 and facing within 60
 * degrees), else 0x29, 4 0x2B within 15 else 0x2F */
s32 func_001848F0(Fiona *f, s32 cmd, s32 state) {
    u8 *h;
    u8 who;
    f32 d;
    s32 v;

    switch (cmd) {
    case 0:
        return func_00184700(f);
    case 1:
        return 0x2C;
    case 2:
        if ((u8)func_00177620(gProgress) != 0 || FI_HEWIE_NEAR(f) != 1) {
            return 0x27;
        }
        d = func_00124490(&f->c.a, (f32 *)((u8 *)gCharPartner + 0x10));
        if (!(d < 30.0f)) {
            return 0x27;
        }
        if (!(d < 12.0f)) {
            return 0x25;
        }
        v = AT(gCharPartner, 0xF3564, s32);
        if (v == 0x4B || (AT(gCharPartner, 0xFC, s32) == 3 && v == 2)) {
            return 0x24;
        }
        return 0x25;
    case 3:
        who = func_00177620(gProgress);
        if (FI_HEWIE_NEAR(f) != 1) {
            return 0x29;
        }
        h = (u8 *)gCharPartner;
        if (!(func_00124490(&f->c.a, (f32 *)(h + 0x10)) < 15.0f)) {
            return 0x29;
        }
        if (AT(h, 0xC4, s32) == 2) {
            return fiona_turn_to(f, func_001244D0(&f->c.a, (f32 *)(h + 0x10))) < 0x1.0c15240000000p+0f /* 1.0471976 */ ? 0x2A : 0x29;
        }
        if (who != 0 || AT(h, 0xF35C0, s32) == 3) {
            return 0x29;
        }
        return 0x28;
    case 4:
        if ((u8)func_00177620(gProgress) != 0 || FI_HEWIE_NEAR(f) != 1) {
            return 0x2F;
        }
        h = (u8 *)gCharPartner;
        if (!(func_00124490(&f->c.a, (f32 *)(h + 0x10)) < 15.0f) || AT(h, 0xF35C0, s32) == 3) {
            return 0x2F;
        }
        return 0x2B;
    }
    return -1;
}

/* at the start in a door (+0xF8: 2 passing through, 3 standing in it): set the door's state
 * (open sides) and the progress records for it */
void func_00184BF0(Fiona *f) {
    u8 *c = (u8 *)f;
    s32 st = AT(c, 0xF8, s32);
    VObject *doors;

    if (st == 3) {
        func_001779C0(gProgress, AT(c, 0x100, u8), AT(c, 0x20, u8));
        AT(c, 0x104, s32) = AT(c, 0x100, s32);
        return;
    }
    if (st != 2) {
        return;
    }
    AT(c, 0x2B, u8) = 0;
    AT(c, 0xC0, u32) = 0x28020018;
    if (AT(c, 0x108, s32) == 0) {
        return;
    }
    doors = D_0044E558;
    if (!DOOR_ISOPEN(doors, AT(c, 0x100, u8))) {
        if ((u8)DOOR_KIND(doors, AT(c, 0x100, u8)) == 1
            && (u16)ROOM_DOOR_LINK(D_0044E568, AT(c, 0x30, s32), AT(c, 0x100, u8)) != 0xFFFF) {
            func_00178C10(gProgress, AT(c, 0x30, s32), AT(c, 0x100, u8), 0xFF);
        }
        doors = D_0044E558;
        DOOR_SET(doors, 0x20, AT(c, 0x100, u8), 0, 0x60000);
        DOOR_SET(doors, 0x1C, AT(c, 0x100, u8), 1, 0x60000);
    } else {
        if ((u8)DOOR_KIND(doors, AT(c, 0x100, u8)) == 1
            && (u16)ROOM_DOOR_LINK(D_0044E568, AT(c, 0x30, s32), AT(c, 0x100, u8)) != 0xFFFF) {
            func_00178A90(gProgress, AT(c, 0x30, s32), AT(c, 0x100, u8), 0xFF);
        }
        doors = D_0044E558;
        DOOR_SET(doors, 0x20, AT(c, 0x100, u8), 1, 0x60000);
        DOOR_SET(doors, 0x1C, AT(c, 0x100, u8), 0, 0x60000);
    }
}

/* show the model parts for what she has equipped (D_0044E988 +0x10: equipment slots 1 and 3;
 * items 0x86..0x89 -> part variants 1..4, else 0; items 0x8A..0x8D -> 6..9, else 5) */
void func_00182FC0(Fiona *f) {
    VObject *equip = D_0044E988;
    void *m = f->c.motion;

    switch (VCALL(equip, 0x10, s32 (*)(VObject *, s32))(equip, 1)) {
    case 0x86: VCALL(m, 0xC4, void (*)(void *, s32))(m, 1); break;
    case 0x87: VCALL(m, 0xC4, void (*)(void *, s32))(m, 2); break;
    case 0x88: VCALL(m, 0xC4, void (*)(void *, s32))(m, 3); break;
    case 0x89: VCALL(m, 0xC4, void (*)(void *, s32))(m, 4); break;
    default:   VCALL(m, 0xC4, void (*)(void *, s32))(m, 0); break;
    }
    switch (VCALL(equip, 0x10, s32 (*)(VObject *, s32))(equip, 3)) {
    case 0x8A: VCALL(m, 0xC4, void (*)(void *, s32))(m, 6); break;
    case 0x8B: VCALL(m, 0xC4, void (*)(void *, s32))(m, 7); break;
    case 0x8C: VCALL(m, 0xC4, void (*)(void *, s32))(m, 8); break;
    case 0x8D: VCALL(m, 0xC4, void (*)(void *, s32))(m, 9); break;
    default:   VCALL(m, 0xC4, void (*)(void *, s32))(m, 5); break;
    }
}

/* clear the two 5-word lists at +0x14 and +0x28 */
void func_001F1D60(u8 *o) {
    u32 i;

    for (i = 0; i < 5; i++) {
        AT(o, 0x28 + i * 4, s32) = 0;
        AT(o, 0x14 + i * 4, s32) = 0;
    }
}

extern void func_002DDC60(void *motion, s32 anim, s32 blend, s32 variant);

/* idle animation `anim` with weight variant `variant`: blended over `blend` frames (-1: at
 * once) */
static void fiona_idle(Fiona *f, s32 anim, s32 blend, s32 variant) {
    if (blend == -1) {
        func_002DDED0(f->c.motion, anim, variant);
    } else {
        func_002DDC60(f->c.motion, anim, blend, variant);
    }
}

/* whether the idle weight moved more than 0.1 from the last one (+0x1AD628) */
static s32 idle_weight_changed(Fiona *f, f32 w) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD};   /* 0.1f */
    f32 d = w - FI(f, 0x1AD628, f32);

    if (d <= 0.0f) {
        d = -d;
    }
    return !(d <= k01.f);
}

/* set the idle animation for her condition: exhausted (+0x1AD584 bit 0: variant 4, weight eased
 * toward the progress +0x7BC), out of breath (variant 2 by heart rate +0x1AD5F4) or scared
 * (variant 3 by stamina +0x1AD5F8); anim 5 instead of 0 when the progress mode is 2 */
void func_001855F0(Fiona *f, s32 blend) {
    static const union { u32 u; f32 f; } k005 = {0x3D4CCCCD};   /* 0.05f */
    void *m;
    s32 variant, anim, idle;
    f32 heart, fear;

    if (f->c.moveMode == 0) {
        f->c.moveSub = 0;
    }
    m = f->c.motion;
    variant = AT(m, 0x560, s32);
    anim = AT(m, 0x55C, s32);
    if (FI(f, 0x1AD584, u32) & 1) {
        Progress *p = gProgress;
        f32 t;

        idle = (u8)func_00177620(p) == 2 ? 5 : 0;
        if (!(anim == idle && variant == 4)) {
            fiona_idle(f, idle, blend, 4);
        }
        t = (90.0f - AT(p, 0x7BC, f32)) / 15.0f;
        if (t < 0.0f) {
            t = 0.0f;
        }
        if (FI(f, 0x1AD5CC, f32) < t) {
            FI(f, 0x1AD5CC, f32) = FI(f, 0x1AD5CC, f32) + k005.f;
            if (!(FI(f, 0x1AD5CC, f32) <= t)) {
                FI(f, 0x1AD5CC, f32) = t;
            }
        } else {
            FI(f, 0x1AD5CC, f32) = FI(f, 0x1AD5CC, f32) - k005.f;
            if (FI(f, 0x1AD5CC, f32) < t) {
                FI(f, 0x1AD5CC, f32) = t;
            }
        }
        AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = FI(f, 0x1AD5CC, f32);
        return;
    }
    heart = (100.0f - FI(f, 0x1AD5F4, f32)) / 60.0f;
    fear = (f32)(0x708 - FI(f, 0x1AD5F8, s32)) / 1800.0f;
    if (!(fear < 0.5f) && fear >= heart + 0.25f) {
        if (heart < 1.0f) {
            idle = (u8)func_00177620(gProgress) == 2 ? 5 : 0;
            if (!(anim == idle && variant == 2) || idle_weight_changed(f, heart)) {
                fiona_idle(f, idle, blend, 2);
            }
            AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = heart;
            FI(f, 0x1AD628, f32) = heart;
        } else {
            idle = (u8)func_00177620(gProgress) == 2 ? 5 : 0;
            if (!(anim == idle && variant == -1)) {
                fiona_idle(f, idle, blend, -1);
            }
            AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = 1.0f;
        }
        return;
    }
    idle = (u8)func_00177620(gProgress) == 2 ? 5 : 0;
    if (!(anim == idle && variant == 3) || idle_weight_changed(f, fear)) {
        fiona_idle(f, idle, blend, 3);
    }
    AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = fear;
    FI(f, 0x1AD628, f32) = fear;
}


/* change Fiona's fear (+0x1AD5F4, 0..99) by `d`; the worn accessory (sub screen slot 3)
 * scales it: 0x8A less gain, 0x8B less gain and more loss, 0x8C no gain and double loss,
 * 0x8D none */
void func_00181010(Fiona *f, f32 d) {
    VObject *items = (VObject *)D_0044E988;
    f32 v;

    if (items != NULL) {
        switch (VCALL(items, 0x10, s32 (*)(VObject *, s32))(items, 3)) {
        case 0x8D:
            break;
        case 0x8C:
            if (d <= 0.0f) {
                d = d * 2.0f;
            } else {
                d = 0.0f;
            }
            break;
        case 0x8B:
            if (d <= 0.0f) {
                d = d * 1.5f;
            } else {
                d = d * 0.75f;
            }
            break;
        case 0x8A:
            if (!(d <= 0.0f)) {
                d = d * 0.75f;
            }
            break;
        }
    }
    v = AT(f, 0x1AD5F4, f32) + d;
    AT(f, 0x1AD5F4, f32) = v;
    if (!(v <= 99.0f)) {
        AT(f, 0x1AD5F4, f32) = 99.0f;
    } else if (v < 0.0f) {
        AT(f, 0x1AD5F4, f32) = 0.0f;
    }
}


extern f32 D_0047E3A0[4];   /* the left stick as a vector (x, 0, z) */
extern u32 D_0047E374;      /* pad buttons held */
extern f32 func_002E2D00(f32 angle);   /* angle wrapped to -pi..pi */

#define FMOVE_DIR     0x1AD550   /* vec: where to move (world, unit or 0) */
#define FMOVE_STILL   0x1AD58C   /* s32: frames without input (to 6) */
#define FMOVE_MODE    0x1AD588   /* u8: 0 free, 1 camera-locked, 2 held, 3 reset */
#define FMOVE_LOCK    0x1AD58A   /* s16: frames the old camera still steers */
#define FMOVE_STICK   0x1AD590   /* vec: last frame's raw input */
#define FMOVE_LAST    0x1AD5A0   /* vec: last frame's normalized input */
#define FMOVE_CAMYAW  0x1AD5B0   /* f32: the camera heading the controls use */
#define FMOVE_GO      0x1AD5D8   /* u8: wants to move (or run) this frame */
#define FMOVE_HEADING 0x1AD5E0   /* f32: heading to turn to */
#define FAUTO_STATE   0x1AD71C   /* s32: auto-walk state (0..10) */

/* |wrap(a)| the way the original computes it (the wrap called again for the result) */
static f32 wrap_abs(f32 a) {
    if (!(func_002E2D00(a) <= 0.0f)) {
        return func_002E2D00(a);
    }
    return -func_002E2D00(a);
}

/* Fiona's movement input, each frame. Normally the left stick (or the d-pad), camera
 * relative: after a camera cut the old camera keeps steering while the stick is held
 * (mode 1, then 2 while the direction holds within 15 degrees); 0x4000 runs. While the game
 * walks her (gProgress +0x1FBEC1): towards the target point (states 2..9; 10 once the
 * pursuer is within reach), or (10) at the pursuer, grabbing it (action 8) when facing it. */
void func_00187650(Fiona *f) {
    static const union { u32 u; f32 f; } k15deg = {0x3E860A92}, k30deg = {0x3F060A92}, k001 = {0x3C23D70A};
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

    FI(f, FMOVE_GO, u8) = 0;
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        FI(f, FMOVE_STILL, s32) = 6;
        if (f->c.moveMode == 0 || f->c.moveMode == 10) {
            goto autowalk;
        }
        return;
    }
    sceVu0CopyVector(e, D_0047E3A0);
    e[0] += (f32)(s32)(((D_0047E374 >> 5) & 1) - ((D_0047E374 >> 7) & 1));
    e[2] += (f32)(s32)(((D_0047E374 >> 6) & 1) - ((D_0047E374 >> 4) & 1));
    sceVu0Normalize(n, e);
    cut = FI(f, FMOVE_MODE, u8) == 3;
    if (!cut && VCALL(D_0044E4B8, 0x94, s32 (*)(VObject *))(D_0044E4B8) != -1) {
        s32 prev = VCALL(D_0044E4B8, 0x90, s32 (*)(VObject *))(D_0044E4B8);

        cut = prev != VCALL(D_0044E4B8, 0x94, s32 (*)(VObject *))(D_0044E4B8);
    }
    if (cut) {
        /* a camera cut: face the move direction and lock the controls to the old camera */
        FI(f, FMOVE_MODE, u8) = 0;
        if (f->c.moveMode == 0 || f->c.moveMode == 10) {
            f32 yaw = FI(f, FMOVE_HEADING, f32);

            f->c.a.angle[1] = yaw;
            sceVu0UnitMatrix(f->c.a.rot);
            sceVu0RotMatrixY(f->c.a.rot, f->c.a.rot, yaw);
            FI(f, FMOVE_STILL, s32) = 0;
            if (!((n[0] <= 0.0f ? -n[0] : n[0]) <= 0.5f) || !((n[2] <= 0.0f ? -n[2] : n[2]) <= 0.5f)) {
                FI(f, FMOVE_LOCK, s16) = 3;
                FI(f, FMOVE_MODE, u8) = 1;
                func_002E3190(rot, VCALL(D_0044E4B8, 0x68, f32 (*)(VObject *))(D_0044E4B8));
                func_002E2DA0(v, rot, n);
                func_0010E640(v, v, -1.0f);
                FI(f, FMOVE_HEADING, f32) = func_0031C5C0(v[0], v[2]);
            }
        }
    }
    if ((e[0] <= 0.0f ? -e[0] : e[0]) <= 0.5f && (e[2] <= 0.0f ? -e[2] : e[2]) <= 0.5f) {
        moving = 0;
        FI(f, FMOVE_STILL, s32)++;
        if (FI(f, FMOVE_STILL, s32) >= 7) {
            FI(f, FMOVE_STILL, s32) = 6;
        }
    } else {
        moving = 1;
        FI(f, FMOVE_STILL, s32) = 0;
    }
    switch (FI(f, FMOVE_MODE, u8)) {
    case 0:
        if (moving) {
            how = 0;
        } else {
            how = AT(f->c.motion, 0x550, f32) <= 0.0f ? 2 : 1;
        }
        break;
    case 1:
        if (!moving) {
            how = 2;
            if (FI(f, FMOVE_STILL, s32) == 6) {
                FI(f, FMOVE_MODE, u8) = 0;
            }
            break;
        }
        how = 3;
        if (FI(f, FMOVE_LOCK, s16) != 0) {
            FI(f, FMOVE_LOCK, s16)--;
            func_002E3190(rot, FI(f, FMOVE_CAMYAW, f32));
        } else {
            f32 d[4] __attribute__((aligned(16)));

            sceVu0SubVector(d, e, &FI(f, FMOVE_STICK, f32));
            if (__builtin_sqrtf(sceVu0InnerProduct(d, d)) < k001.f) {
                FI(f, FMOVE_MODE, u8) = 2;
                FI(f, 0x1AD5B4, u32) = 0x3C0EFA35;   /* 0.5 degrees */
                func_002E3190(rot, VCALL(D_0044E4B8, 0x68, f32 (*)(VObject *))(D_0044E4B8));
            }
        }
        func_002E2DA0(v, rot, n);
        func_0010E640(v, v, -1.0f);
        FI(f, 0x1AD5E4, f32) = func_0031C5C0(v[0], v[2]);
        break;
    case 2:
        if (moving) {
            f32 a = func_0031C5C0(FI(f, FMOVE_LAST, f32), FI(f, FMOVE_LAST + 8, f32));

            how = 0;
            if (!(wrap_abs(func_0031C5C0(n[0], n[2]) - a) <= k15deg.f)) {
                FI(f, FMOVE_MODE, u8) = 0;
            }
            break;
        }
        how = 2;
        if (FI(f, FMOVE_STILL, s32) == 6) {
            FI(f, FMOVE_MODE, u8) = 0;
        }
        break;
    }
    switch (how) {
    case 3:
        func_002E3190(rot, FI(f, FMOVE_CAMYAW, f32));
        func_002E2DA0(v, rot, n);
        func_0010E640(&FI(f, FMOVE_DIR, f32), v, -1.0f);
        break;
    case 2:
        FI(f, FMOVE_DIR, f32) = 0.0f;
        FI(f, FMOVE_DIR + 4, f32) = 0.0f;
        FI(f, FMOVE_DIR + 8, f32) = 0.0f;
        break;
    case 1:
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 1.0f;
        sceVu0ApplyMatrix(&FI(f, FMOVE_DIR, f32), f->c.a.rot, v);
        break;
    case 0:
        func_002E3190(rot, VCALL(D_0044E4B8, 0x68, f32 (*)(VObject *))(D_0044E4B8));
        func_002E2DA0(v, rot, n);
        func_0010E640(&FI(f, FMOVE_DIR, f32), v, -1.0f);
        break;
    }
    sceVu0CopyVector(&FI(f, FMOVE_STICK, f32), e);
    if (FI(f, FMOVE_MODE, u8) == 0) {
        FI(f, FMOVE_CAMYAW, f32) = VCALL(D_0044E4B8, 0x68, f32 (*)(VObject *))(D_0044E4B8);
    }
    if (FI(f, FMOVE_MODE, u8) != 2) {
        sceVu0CopyVector(&FI(f, FMOVE_LAST, f32), n);
    }
    if (D_0047E374 & 0x4000) {
        FI(f, FMOVE_GO, u8) = 1;
    }
    return;

autowalk:
    if ((u32)(FI(f, 0x1AD580, s32) - 0xE) < 2) {
        FI(f, FAUTO_STATE, s32) = 0;
        return;
    }
    switch (FI(f, FAUTO_STATE, u32)) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9: {
        f32 pt[4] __attribute__((aligned(16)));
        f32 d[4] __attribute__((aligned(16)));
        u32 tri;

        if (!(f->c.unk128 < f->c.unk124)) {
            return;
        }
        FI(f, FMOVE_STILL, s32) = 0;
        tri = f->c.a.navTri;
        func_001273D0(&f->c, &tri, pt, 1.5f);
        if (!(FI(f, 0x1AD584, u32) & 2) && FI(f, 0x1AD5D7, u8) == 1 && gCharPursuer->a.unkC4 != 2) {
            s32 near = 0;
            f32 py = f->c.a.pos[1];

            if (py < gCharPursuer->a.pos[1]) {
                if (gCharPursuer->a.pos[1] - py <= f->c.a.height) {
                    near = 1;
                }
            } else if (py - gCharPursuer->a.pos[1] <= gCharPursuer->a.height) {
                near = 1;
            }
            if (near == 1) {
                sceVu0SubVector(d, pt, gCharPursuer->a.pos);
                if (__builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) <= f->c.a.radius + gCharPursuer->a.radius) {
                    FI(f, FAUTO_STATE, s32) = 10;
                }
            }
        }
        sceVu0SubVector(d, pt, f->c.a.pos);
        d[1] = 0.0f;
        sceVu0Normalize(&FI(f, FMOVE_DIR, f32), d);
        FI(f, FMOVE_HEADING, f32) = func_0031C5C0(d[0], d[2]);
        FI(f, FMOVE_GO, u8) = (u32)(FI(f, FAUTO_STATE, s32) - 5) < 2 ? 0 : 1;
        return;
    }
    case 10: {
        f32 d[4] __attribute__((aligned(16)));
        struct {
            s32 state, a, b, c, d;
            f32 e;
            s32 f;
            u8 g, h;
            u16 i;
        } act;

        if ((FI(f, 0x1AD584, u32) & 2) || FI(f, 0x1AD5D7, u8) != 1 || gCharPursuer->a.unkC4 == 2) {
            return;
        }
        sceVu0SubVector(d, gCharPursuer->a.pos, f->c.a.pos);
        d[1] = 0.0f;
        sceVu0Normalize(&FI(f, FMOVE_DIR, f32), d);
        FI(f, FMOVE_HEADING, f32) = func_0031C5C0(d[0], d[2]);
        FI(f, FMOVE_GO, u8) = 1;
        if (!(wrap_abs(FI(f, FMOVE_HEADING, f32) - f->c.a.angle[1]) < k30deg.f)) {
            return;
        }
        if (FI(f, 0x14E8, s32) != 0) {
            return;
        }
        act.state = 8;
        act.a = 0x1A;
        FI(f, 0x14E8, s32) = act.state;
        FI(f, 0x14EC, s32) = act.a;
        FI(f, 0x14F0, s32) = act.b;
        FI(f, 0x14F4, s32) = act.c;
        FI(f, 0x14F8, s32) = act.d;
        FI(f, 0x14FC, f32) = act.e;
        FI(f, 0x1500, s32) = act.f;
        FI(f, 0x1504, u8) = act.g;
        FI(f, 0x1505, u8) = act.h;
        FI(f, 0x1506, u16) = act.i;
        return;
    }
    }
}


/* the kind of a motion id: 0 standing (0..5 but 1), 1 walk/run starts, 2 turns, 3, 4, 5,
 * 6 (0x12xx), 7 (0x700..0x707), 8 (0x708/0x709), 9 (0x403), 10 (0xE01), 11 anything else */
static s32 fiona_motion_kind(s32 id) {
    switch (id) {
    case 0xE01:
        return 10;
    case 0x403:
        return 9;
    case 0x708:
    case 0x709:
        return 8;
    case 0x700:
    case 0x701:
    case 0x702:
    case 0x703:
    case 0x704:
    case 0x705:
    case 0x706:
    case 0x707:
        return 7;
    case 0x1200:
    case 0x1201:
    case 0x1202:
    case 0x1203:
        return 6;
    case 0xB01:
        return 4;
    case 0x207:
        return 3;
    case 0x202:
    case 0x203:
    case 0x205:
    case 0x206:
        return 2;
    case 0x200:
    case 0x201:
    case 0x204:
    case 0x208:
    case 0x400:
    case 0x401:
    case 0x402:
        return 1;
    case 1:
        return 5;
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
        return 0;
    }
    return 11;
}

/* the idle step: when the current motion has played out (+0x550 <= 0), a standing one sets
 * +0xE1, any other picks the next idle; then the character's idle update */
void func_0018B600(Fiona *f) {
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        if (fiona_motion_kind(AT(f->c.motion, 0x55C, s32)) == 0) {
            f->c.unkE1 = 1;
        } else {
            func_001855F0(f, -1);
        }
    }
    func_00125A10(&f->c);
}


extern s32 func_00122C90(void *self, u32 triA, u32 triB, const f32 *posA, const f32 *posB, u32 mask);
extern void func_002DD110(void *motion, f32 *target, f32 *pitch, f32 *yaw);   /* head angles to a point */
extern void func_002DD310(void *motion, f32 pitch, f32 yaw, f32 pitchSpeed, f32 yawSpeed);

#define FLOOK_ON     0x1AD5FC   /* u8: looking at a character */
#define FLOOK_WHO    0x1AD600   /* Character *: whom */
#define FLOOK_POINT  0x1AD610   /* vec: its head */
#define FLOOK_HOLD   0x1AD620   /* s32: frames to keep looking */

/* whether Fiona notices character c: near enough, ahead of her (cos > 0.6 with `fwd`) and
 * reachable on the nav mesh */
static s32 fiona_sees(Fiona *f, Character *c, f32 range, f32 *fwd) {
    static const union { u32 u; f32 f; } k06 = {0x3F19999A};
    f32 d[4] __attribute__((aligned(16)));

    if (!(func_00124490(&f->c.a, c->a.pos) < range)) {
        return 0;
    }
    sceVu0SubVector(d, c->a.pos, f->c.a.pos);
    if (sceVu0InnerProduct(fwd, d) <= k06.f) {
        return 0;
    }
    return (u8)func_00122C90(f, f->c.a.navTri, c->a.navTri, f->c.a.pos, c->a.pos, 0) == 1;
}

/* Fiona's head, each frame: she looks at the pursuer (within 200) or Hewie (within 50) when
 * ahead of her and reachable, the head angles clamped (pitch -18..45 degrees, yaw +-90, or
 * +-108 while held or in moves 0xD / 0xB-0x20); otherwise towards where she is turning. The
 * motion eases the head there, faster for bigger changes and while she turns. */
void func_00186180(Fiona *f) {
    static const union { u32 u; f32 f; } kPitchMax = {0x3F490FDB}, kPitchMin = {0xBEA0D97C},
        kYaw = {0x3FC90FDB}, kYawWide = {0x3FF1463B}, k01 = {0x3DCCCCCD};
    f32 fwd[4] __attribute__((aligned(16)));
    f32 pitch, yaw, dp, dy, turn;
    u8 *m;
    s32 a;

    if (f->c.unkE0 == 1) {
        return;
    }
    if (FI(f, FLOOK_ON, u8) == 0) {
        if (FI(f, FLOOK_HOLD, s32) != 0) {
            FI(f, FLOOK_HOLD, s32)--;
            FI(f, FLOOK_ON, u8) = 1;
        }
    }
    if (FI(f, FLOOK_ON, u8) == 0 && f->c.moveMode == 0) {
        a = FI(f, 0x1AD580, s32);
        if (a != 0xE && a != 1 && a != 0xF && !(FI(f, 0x1AD584, u32) & 2) &&
            fiona_motion_kind(AT(f->c.motion, 0x55C, s32)) != 5) {
            fwd[0] = 0.0f;
            fwd[1] = 0.0f;
            fwd[2] = 1.0f;
            sceVu0ApplyMatrix(fwd, f->c.a.rot, fwd);
            if (FI(f, 0x1AD5D7, u8) == 1 && fiona_sees(f, gCharPursuer, 200.0f, fwd)) {
                FI(f, FLOOK_ON, u8) = 1;
                FI(f, FLOOK_WHO, Character *) = gCharPursuer;
            }
            if (FI(f, FLOOK_ON, u8) == 0 && FI(f, 0x1AD5D5, u8) == 1 &&
                fiona_sees(f, gCharPartner, 50.0f, fwd)) {
                FI(f, FLOOK_ON, u8) = 1;
                FI(f, FLOOK_WHO, Character *) = gCharPartner;
            }
        }
    }
    if (FI(f, FLOOK_ON, u8) != 0) {
        Character *c = FI(f, FLOOK_WHO, Character *);

        if (c == NULL || c->a.active != 1 || c->a.disabled != 0) {
            pitch = 0.0f;
            yaw = 0.0f;
        } else {
            VCALL(c->motion, 0x60, void (*)(void *, f32 *))(c->motion, &FI(f, FLOOK_POINT, f32));
            func_002DD110(f->c.motion, &FI(f, FLOOK_POINT, f32), &pitch, &yaw);
            if (!(pitch <= kPitchMax.f)) {
                pitch = kPitchMax.f;
            }
            if (pitch < kPitchMin.f) {
                pitch = kPitchMin.f;
            }
            if (FI(f, FLOOK_HOLD, s32) != 0 || f->c.moveMode == 0xD ||
                (f->c.moveMode == 0xB && f->c.moveSub == 0x20)) {
                if (!(yaw <= kYawWide.f)) {
                    yaw = kYawWide.f;
                }
                if (yaw < -kYawWide.f) {
                    yaw = -kYawWide.f;
                }
            } else {
                if (!(yaw <= kYaw.f)) {
                    yaw = 0.0f;
                }
                if (yaw < -kYaw.f) {
                    yaw = 0.0f;
                }
            }
        }
    } else {
        a = FI(f, 0x1AD580, s32);
        if (f->c.moveMode != 0 || a == 0xE || a == 1 || a == 0xF || (FI(f, 0x1AD584, u32) & 2)) {
            pitch = 0.0f;
            yaw = 0.0f;
        } else {
            pitch = 0.0f;
            yaw = func_002E2D00(FI(f, 0x1AD5E0, f32) - f->c.a.angle[1]);
        }
    }
    m = f->c.motion;
    dp = pitch - AT(m, 0x854, f32);
    dp = dp <= 0.0f ? -dp : dp;
    dy = yaw - AT(m, 0x858, f32);
    dy = dy <= 0.0f ? -dy : dy;
    dp = k01.f * dp;
    dy = k01.f * dy;
    turn = wrap_abs(f->c.a.angle[1] - FI(f, 0x1AD5B8, f32));
    func_002DD310(f->c.motion, pitch, yaw, dp, dy + turn);
}


extern void func_00181180(Fiona *f, s32 id, s32, s32, s32);   /* a voice */
extern void func_00183960(Fiona *f);

/* the sounds of Fiona's motions, on their key frames (motion events 1 and 0x10): steps,
 * crouching, falling (with a noise others hear), voices */
void func_00181F20(Fiona *f) {
    s32 v;

    if (f->c.unk14D0 != 0 && f->c.unk14D0 != 5) {
        return;
    }
    if ((u8)func_001F4770(f->c.motion, 0, 0, 1) & 1) {
        switch (AT(f->c.motion, 0x55C, s32)) {
        case 1:
            if (f->c.moveMode == 0xD) {
                func_00122C20(&f->c.a, 0x39, 5, 0, 0, NULL);
            }
            break;
        case 0xC0E:
        case 0xC0D:
        case 0xC0A:
        case 0xC07:
        case 0xC06:
        case 0xC04:
        case 0xC03:
        case 0xC02:
        case 0xC00:
            if (f->c.moveMode == 0xD) {
                if (!(u8)Progress_TestFlag(gProgress, 0x25)) {
                    func_00183960(f);
                } else {
                    func_00122C20(&f->c.a, 0x33, 5, 0, 0, NULL);
                }
            }
            break;
        case 0xD01:
        case 0xD00:
            func_00122C20(&f->c.a, 0x3C, 5, 0, 0, NULL);
            break;
        case 0xE00: {
            Progress *p = gProgress;

            v = Progress_GetVar(p, 0x26) & 0xFF;
            if (v != 7 && v != 6) {
                func_00122C20(&f->c.a, 0x3C, 5, 0, 0, NULL);
            }
            func_002A8440((u8 *)p + 0x778, 0x1F, f->c.a.room, f->c.a.navTri, 0xFFFF);
            break;
        }
        }
    }
    if ((u8)func_001F4770(f->c.motion, 0, 0, 1) & 0x10) {
        switch (AT(f->c.motion, 0x55C, s32)) {
        case 0x403:
            func_00122C20(&f->c.a, 0xF, 5, 0, 0, NULL);
            break;
        case 0xF02:
        case 0x1404:
        case 0x1403:
        case 0x1500:
            func_00181180(f, 0x7D, 5, 0, 0);
            break;
        case 0xB01:
            func_00122C20(&f->c.a, 0x7E, 5, 0, 0, NULL);
            break;
        case 0x100B:
        case 0x1008:
        case 0xB00:
            func_00181180(f, 0x80, 5, 0, 0);
            break;
        case 0x609:
        case 0x608:
            func_00122C20(&f->c.a, 0x71, 5, 0, 0, NULL);
            break;
        case 0xE00:
            v = Progress_GetVar(gProgress, 0x26) & 0xFF;
            if (v == 6) {
                func_00122C20(&f->c.a, 0x21, 5, 0, 0, NULL);
            } else if (v == 7) {
                func_00122C20(&f->c.a, 0x20, 5, 0, 0, NULL);
            }
            break;
        }
    }
}


extern u32 func_002DD420(void *motion, f32 *footOut, s32 left, f32 a, f32 b);   /* foot on the ground (u8) */
extern u32 func_002DD860(void *motion, s32 left, f32 t);                        /* foot planted while standing (u8) */
extern u32 func_00123E20(Actor *a, f32 *p);
extern u32 func_00123710(void *self, s32 door, s32 side, const f32 *ofs, f32 *out);
extern u32 func_00124320(Actor *a, const f32 *target, u32 tri, const f32 *from, u32 mask);
extern void func_00125E10(Character *c, f32 *pos, s32 big);
extern f32 D_003B2478, D_003B247C;   /* the ladder's foot offset (x, z) */
extern VObject *D_0044E560;         /* the sound system */

#define FSTEP_LEFT  0x1AD5D0   /* u8: left foot down last frame */
#define FSTEP_RIGHT 0x1AD5D1   /* u8: right foot down last frame */
#define FSTEP_COUNT 0x1AD5DC   /* s32: steps taken (picks the sound variant) */

/* the nav triangle `i`, NULL out of range */
static NavTri *step_tri(NavMesh *nm, u32 i) {
    return (i < nm->numTris && nm->tris != NULL) ? &nm->tris[i] : NULL;
}

/* Fiona's footsteps, each frame: when a foot touches down (the motion's foot contacts; while
 * standing, planted feet) the floor under it decides the sound - its nav triangle's material
 * (0x8000.. bits; wet floors ask the sound system), ladders and their rungs (moves 0x700..),
 * splashes in rooms 7 / 0xD1 / 0x106 - with a variant per step, loudness from her speed and
 * a muffled sound when wearing item 0x80; the noise is heard by the pursuer (louder when
 * running) */
void func_001869D0(Fiona *f) {
    static const union { u32 u; f32 f; } k04 = {0x3ECCCCCD}, k07 = {0x3F333334};
    f32 left[4] __attribute__((aligned(16)));
    f32 right[4] __attribute__((aligned(16)));
    f32 foot[4] __attribute__((aligned(16)));
    f32 tmp[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 speed[4] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 0.0f};   /* root motion */
    Progress *p;
    NavMesh *nm;
    NavTri *t;
    u32 l, r, flags, tri;
    s32 step = 0, base, sound, bank, noise, room, id;
    u32 vol;
    f32 x;

    if (f->c.a.disabled == 1 || f->c.a.navTri == (u32)-1) {
        return;
    }
    if ((u8)VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) == 1) {
        return;
    }
    p = gProgress;
    if ((u8)Progress_TestFlag(p, 8) == 1) {
        return;
    }
    l = (u8)func_002DD420(f->c.motion, left, 1, 0.0f, 1.0f);
    r = (u8)func_002DD420(f->c.motion, right, 0, 0.0f, 1.0f);
    if (fiona_motion_kind(AT(f->c.motion, 0x55C, s32)) == 0) {
        if (AT(f->c.motion, 0x554, s32) != -1 && !(AT(f->c.motion, 0x550, f32) <= 0.0f)) {
            l = (u8)func_002DD860(f->c.motion, 1, 0.0f);
            r = (u8)func_002DD860(f->c.motion, 0, 0.0f);
        } else {
            l = 1;
            r = 1;
        }
    }
    if (l == 1 && FI(f, FSTEP_LEFT, u8) == 0) {
        step = 1;
    } else if (r == 1 && FI(f, FSTEP_RIGHT, u8) == 0) {
        step = -1;
    }
    FI(f, FSTEP_LEFT, u8) = l;
    FI(f, FSTEP_RIGHT, u8) = r;
    if (step == 0) {
        if (AT(f->c.motion, 0x550, f32) <= 0.0f && fiona_motion_kind(AT(f->c.motion, 0x55C, s32)) == 0) {
            FI(f, FSTEP_COUNT, s32) = 0;
        }
        return;
    }

    sceVu0CopyMatrix(m, f->c.a.rot);
    sceVu0CopyVector(m[3], f->c.a.pos);
    func_002E2DD0(foot, m, step == 1 ? left : right);
    tri = func_00123E20(&f->c.a, foot);
    nm = D_0044E570;
    t = step_tri(nm, tri != (u32)-1 ? tri : f->c.a.navTri);
    room = f->c.a.room;
    if (room == 7 || room == 0xD1 || room == 0x106) {
        func_00125E10(&f->c, foot, f->c.moveMode == 0 && f->c.moveSub == 2);
    }

    flags = (u32)-1;
    bank = 4;
    sound = -1;
    if (f->c.moveMode == 3) {
        id = AT(f->c.motion, 0x55C, s32);
        switch (id - 0x700) {
        case 0:
            if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
                sound = 0x14;
            }
            break;
        case 1:
        case 2:
        case 5:
        case 6:
        case 8:
        case 9:
            sound = 0x14;
            break;
        case 3:
        case 4:
            v[0] = D_003B2478;
            v[1] = 0.0f;
            v[2] = D_003B247C;
            v[3] = 0.0f;
            tri = func_00123710(f, f->c.unk100, 0, v, tmp);
            if (func_00124320(&f->c.a, foot, tri, tmp, 0) == (u32)-1) {
                sound = 0x14;
            } else if (id == 0x703) {
                tri = VCALL(nm, 0x5C, u32 (*)(NavMesh *, s32, s32, f32 *))(nm, f->c.unk100, 0, tmp);
                tri = func_00124320(&f->c.a, foot, tri, tmp, 0);
                if (tri != (u32)-1) {
                    t = step_tri(nm, tri);
                    flags = t->flags;
                }
            }
            break;
        case 7:
            tri = VCALL(nm, 0x5C, u32 (*)(NavMesh *, s32, s32, f32 *))(nm, f->c.unk100, 1, tmp);
            tri = func_00124320(&f->c.a, foot, tri, tmp, 0);
            if (tri != (u32)-1) {
                t = step_tri(nm, tri);
                flags = t->flags;
            }
            break;
        }
    }
    if (sound != -1) {
        func_00122C20(&f->c.a, sound, 4, 0, 0, NULL);
        func_002A8440((u8 *)p + 0x778, 4, f->c.a.room, f->c.a.navTri, 0xFFFF);
        return;
    }

    if (flags == (u32)-1) {
        flags = t->flags;
    }
    base = AT(gProgress, 0x1FBEC0, u8) != 0 ? 0x15 : 0;
    switch (flags & 0x2018000) {
    case 0x2008000:
        if ((u8)VCALL(D_0044E560, 0xA4, s32 (*)(VObject *, s32))(D_0044E560, 6) == 1) {
            base = 0x10;
            bank = 6;
        } else {
            base = 0x15;
        }
        break;
    case 0x2000000:
        base = 0x10;
        break;
    case 0x18000:
        base = 0xC;
        break;
    case 0x10000:
        base = 8;
        break;
    case 0x8000:
        base = 4;
        break;
    }
    base += FI(f, FSTEP_COUNT, s32) & 3;
    FI(f, FSTEP_COUNT, s32)++;
    func_001F6370(f->c.motion, speed, 0.0f);
    x = (speed[2] - k04.f) / k07.f;
    if (x < 0.0f) {
        x = 0.0f;
    }
    if (!(x <= 1.0f)) {
        x = 1.0f;
    }
    vol = (u32)(2.0f * x) & 0x7F;
    if (D_0044E988 != NULL && VCALL(D_0044E988, 0x10, s32 (*)(VObject *, s32))(D_0044E988, 0) == 0x80) {
        func_00122C20(&f->c.a, base, bank, -0x30, (s8)vol, NULL);
        noise = f->c.moveMode == 0 && f->c.moveSub == 2 ? 5 : 1;
    } else {
        func_00122C20(&f->c.a, base, bank, 0, (s8)vol, NULL);
        noise = f->c.moveMode == 0 && f->c.moveSub == 2 ? 0x14 : 4;
    }
    func_002A8440((u8 *)p + 0x778, noise, f->c.a.room, f->c.a.navTri, 0xFFFF);
}

/* a point `side` across and `reach` ahead of Fiona (in her frame, +0x60, from +0x40) */
static void push_probe_point(Fiona *f, f32 side, f32 reach, f32 *out) {
    f32 v[4] __attribute__((aligned(16)));

    v[0] = side;
    v[1] = 0.0f;
    v[2] = reach;
#ifdef HG_NATIVE
    v[3] = 0.0f;   /* unset in the original (its frame has no translation, so it doesn't matter) */
#endif
    sceVu0ApplyMatrix(v, (f32 (*)[4])((u8 *)f + 0x60), v);
    sceVu0AddVector(out, (f32 *)((u8 *)f + 0x40), v);
}

/* Looking for something to push `reach` ahead (only while walking, running or pushing): the
 * walk from her triangle towards that point must end at a wall - an edge without a neighbour,
 * or into a triangle blocked for her (+0xC0) - and the floor 2 to each side ahead must be
 * pushable (NAV_PUSHABLE). Then she faces square to the wall (the push direction +0x1AD570,
 * the side of the wall she is on) and, with `pick`, takes the lowest object both sides share
 * (D_0044FE08 +0x24: the objects on a triangle, as bits) as +0x1AD560 (else -1). 0, or -1 when
 * there is nothing to push. */
s32 func_00188280(Fiona *f, s32 pick, f32 reach) {
    static const union { u32 u; f32 f; } kHalfPi = {0x3FC90FDB}, kPi = {0x40490FDB};
    VObject *objs = D_0044FE08;
    NavMesh *nm;
    NavTri *t;
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    u32 tri, from, e, a, b, both, k;
    s32 kind;
    f32 ang, diff;

    if (objs == NULL || AT(f, 0xF8, s32) != 0) {
        return -1;
    }
    kind = fiona_motion_kind(AT(AT(f, 0xF0, u8 *), 0x55C, s32));
    if (kind != 1 && kind != 2 && kind != 6) {
        return -1;
    }
    push_probe_point(f, 0.0f, reach, at);
    nm = D_0044E570;
    tri = AT(f, 0x34, u32);
    for (;;) {
        from = tri;
        e = VCALL(nm, 0x20, u32 (*)(NavMesh *, u32, f32 *, f32 *))(nm, tri, (f32 *)((u8 *)f + 0x10), at);
        if (e == 3) {
            return -1;   /* reached it: no wall */
        }
        if (e == 4) {
            return -1;
        }
        tri = NavMesh_Tri(nm, tri)->adj[e];
        if (tri == NAV_NONE) {
            break;
        }
        if (NavMesh_TriFlags(nm, tri) & AT(f, 0xC0, u32)) {
            break;
        }
    }
    push_probe_point(f, -2.0f, reach, at);
    tri = func_00124480(&f->c.a, at, 0);
    if (tri == NAV_NONE || !(NavMesh_TriFlags(nm, tri) & NAV_PUSHABLE)) {
        return -1;
    }
    a = VCALL(objs, 0x24, u32 (*)(VObject *, u32))(objs, tri);
    push_probe_point(f, 2.0f, reach, at);
    tri = func_00124480(&f->c.a, at, 0);
    if (tri == NAV_NONE || !(NavMesh_TriFlags(nm, tri) & NAV_PUSHABLE)) {
        return -1;
    }
    b = VCALL(objs, 0x24, u32 (*)(VObject *, u32))(objs, tri);

    /* the wall edge's direction, turned a quarter: into the wall, from the side she is on */
    t = NavMesh_Tri(nm, from);
    sceVu0SubVector(d, t->v[e + 1 < 3 ? e + 1 : 0], t->v[e]);
    ang = func_002E2D00(kHalfPi.f + func_0031C5C0(d[0], d[2]));
    if (!(func_002E2D00(ang - AT(f, 0x54, f32)) <= 0.0f)) {
        diff = func_002E2D00(ang - AT(f, 0x54, f32));
    } else {
        diff = -func_002E2D00(ang - AT(f, 0x54, f32));
    }
    if (!(diff <= kHalfPi.f)) {
        ang = func_002E2D00(kPi.f + ang);
    }
    d[0] = 0.0f;
    d[1] = 0.0f;
    d[2] = 1.0f;
    func_002E3190(m, ang);
    func_002E2DA0(FIONA_STICK(f), m, d);
    FI(f, 0x1AD574, f32) = 0.0f;
    FIONA_PUSH_OBJ(f) = -1;
    if (pick != 0) {
        both = a & b;
        if (both != 0) {
            for (k = 0; !(both & (1u << k)); k++) {
            }
            FIONA_PUSH_OBJ(f) = k;
        }
    }
    return 0;
}


/* the head's look, each frame: while looking at a character (FLOOK_ON / FLOOK_WHO, that one
 * active and not hidden) at its head (its motion +0x60 into FLOOK_POINT), the head angles
 * aimed there (pitch -18..45 degrees, yaw -126..126) - else straight ahead - eased 10% of the
 * way (the motion's +0x854 / +0x858) */
void func_00185FC0(Fiona *f) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD}, kUp = {0x3F490FDB}, kDown = {0xBEA0D97C},
        kSide = {0x400CBE4C}, kNegSide = {0xC00CBE4C};
    u8 *who;
    f32 pitch, yaw;

    if (FI(f, FLOOK_ON, u8) == 1 && (who = FI(f, FLOOK_WHO, u8 *)) != NULL) {
        if (AT(who, 0x28, u8) == 1 && AT(who, 0x29, u8) == 0) {
            VCALL(AT(who, 0xF0, u8 *), 0x60, void (*)(u8 *, f32 *))(AT(who, 0xF0, u8 *), (f32 *)((u8 *)f + FLOOK_POINT));
        } else {
            FI(f, FLOOK_ON, u8) = 0;
        }
    }
    yaw = 0.0f;
    pitch = 0.0f;
    if (FI(f, FLOOK_ON, u8) == 1) {
        func_002DD110(f->c.motion, (f32 *)((u8 *)f + FLOOK_POINT), &pitch, &yaw);
        if (!(pitch <= kUp.f)) {
            pitch = kUp.f;
        }
        if (pitch < kDown.f) {
            pitch = kDown.f;
        }
        if (!(yaw <= kSide.f)) {
            yaw = kSide.f;
        }
        if (yaw < kNegSide.f) {
            yaw = kNegSide.f;
        }
    }
    AT(f->c.motion, 0x854, f32) = AT(f->c.motion, 0x854, f32) + k01.f * (pitch - AT(f->c.motion, 0x854, f32));
    AT(f->c.motion, 0x858, f32) = AT(f->c.motion, 0x858, f32) + k01.f * (yaw - AT(f->c.motion, 0x858, f32));
}


extern s32 func_00123F70(Actor *a, Actor *b);   /* step `a` out of `b` (its triangle, -1: can't) */
extern void func_0010E640(f32 *d, const f32 *a, f32 s);   /* scale x, y, z */

/* keep Fiona out of the others, each frame: overlapping the pursuer (when +0x1AD5D7), she is
 * pushed out to their radii (+0xC8) apart - along the pursuer's walk when it comes at her, else
 * straight away from it - unless that is off the floor (then +0x2A); overlapping the partner
 * (+0x1AD5D5, unless Progress +0x1FBEC1) she steps out of it or back to where she was */
void func_00188960(Fiona *f) {
    static const union { u32 u; f32 f; } k20 = {0x41A00000};
    u8 *a = (u8 *)f;
    u8 *pu = (u8 *)gCharPursuer;
    f32 d[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 dist, rsum;
    s32 along;
    u32 tri;

    if (FI(f, 0x1AD5D7, u8) == 1 && AT(pu, 0x2A, u8) == 0 &&
        (u8)func_001241F0(&f->c.a, (Actor *)pu, 0.0f, 0.0f) == 1) {
        sceVu0SubVector(d, (f32 *)(a + 0x10), (f32 *)(pu + 0x10));
        dist = __builtin_sqrtf(__builtin_fabsf(d[2] * d[2] + d[0] * d[0]));
        rsum = AT(a, 0xC8, f32) + AT(gCharPursuer, 0xC8, f32);
        func_001F6370(AT(gCharPursuer, 0xF0, u8 *), v, 0.0f);
        along = 0;
        if (!(v[2] <= 0.0f)) {
            sceVu0CopyMatrix(m, (f32 (*)[4])((u8 *)gCharPursuer + 0x60));
            sceVu0ApplyMatrix(v, m, v);
            if (!(sceVu0InnerProduct(v, d) <= 0.0f)) {
                along = 1;
                func_0010E640(v, v, k20.f);
                sceVu0AddVector(v, v, d);
            }
        }
        if (!along) {
            sceVu0CopyVector(v, d);
        }
        v[1] = 0.0f;
        sceVu0Normalize(v, v);
        func_0010E640(v, v, rsum - dist);
        sceVu0AddVector(at, (f32 *)(a + 0x10), v);
        tri = func_00124480(&f->c.a, at, -1);
        if (tri == NAV_NONE) {
            AT(a, 0x2A, u8) = 1;
        } else {
            AT(a, 0x34, u32) = tri;
            sceVu0CopyVector((f32 *)(a + 0x10), at);
        }
        AT(a, 0x124, s32) = AT(a, 0x128, s32);
        return;
    }
    if (AT(gProgress, 0x1FBEC1, u8) != 0 || FI(f, 0x1AD5D5, u8) != 1 || AT(gCharPartner, 0x2A, u8) != 0) {
        return;
    }
    if ((u8)func_001241F0(&f->c.a, (Actor *)gCharPartner, 0.0f, 0.0f) != 1) {
        return;
    }
    if (func_00123F70(&f->c.a, (Actor *)gCharPartner) == -1) {
        AT(a, 0x34, u32) = AT(a, 0x38, u32);
        sceVu0CopyVector((f32 *)(a + 0x10), (f32 *)(a + 0x40));
    }
    AT(a, 0x124, s32) = AT(a, 0x128, s32);
}


extern void func_002DDED0(void *motion, s32 anim, s32 blend);   /* play anim blended with another (-1 none) */

#define FWALK_BLEND 0x1AD628   /* f32: the walk blend last set */

/* the walk's look, each frame (+0xFC set while idle +0xF8): from Fiona's state +0x1AD5F4
 * ((100 - it) / 60) and +0x1AD5F8 ((1800 - it) / 1800) - the walk (0x200, mode 2 0x208) alone,
 * or blended with 0x201 or 0x204 by those amounts; a blend is reset only when it moves more
 * than 0.1 */
void func_00185CF0(Fiona *f) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD};
    void *m;
    s32 cur, next, base, change;
    f32 a, b, d;

    if (AT(f, 0xF8, s32) == 0) {
        AT(f, 0xFC, s32) = 1;
    }
    cur = AT(f->c.motion, 0x55C, s32);
    next = AT(f->c.motion, 0x560, s32);
    base = (u8)func_00177620(gProgress) == 2 ? 0x208 : 0x200;
    a = (100.0f - FI(f, 0x1AD5F4, f32)) / 60.0f;
    b = (f32)(0x708 - FI(f, 0x1AD5F8, s32)) / 1800.0f;
    change = 0;
    if (b < 0.5f || !(a + 0.25f <= b)) {
        if (cur == base && next == 0x204) {
            d = b - FI(f, FWALK_BLEND, f32);
            if (d <= 0.0f) {
                d = -d;
            }
            if (!(d <= k01.f)) {
                change = 1;
            }
        } else {
            change = 1;
        }
        if (change == 1) {
            func_002DDED0(f->c.motion, base, 0x204);
        }
        m = f->c.motion;
        AT(AT(m, 0x6A4, u8 *), 0x1C, f32) = b;
        FI(f, FWALK_BLEND, f32) = b;
        return;
    }
    if (a < 1.0f) {
        if (cur == base && next == 0x201) {
            d = a - FI(f, FWALK_BLEND, f32);
            if (d <= 0.0f) {
                d = -d;
            }
            if (!(d <= k01.f)) {
                change = 1;
            }
        } else {
            change = 1;
        }
        if (change == 1) {
            func_002DDED0(f->c.motion, base, 0x201);
        }
        m = f->c.motion;
        AT(AT(m, 0x6A4, u8 *), 0x1C, f32) = a;
        FI(f, FWALK_BLEND, f32) = a;
        return;
    }
    if (!(cur == base && next == -1)) {
        func_002DDED0(f->c.motion, base, -1);
    }
    AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = 1.0f;
}


extern const PTMF D_003B2DC8;   /* Fiona's state: turning on the spot */

/* State: turning on the spot to +0x1AD5E0 (10 degrees a frame): once there, back to idle;
 * meanwhile the turn left / right animation (0x400 / 0x401) - each only between animations
 * (no blend running) */
void func_0018A5D0(Fiona *f) {
    static const union { u32 u; f32 f; } k10deg = {0x3E32B8C3};
    f32 left = func_00124530(&f->c.a, FI(f, 0x1AD5E0, f32), k10deg.f);

    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        if (left == 0.0f) {
            func_001855F0(f, -1);
        } else if (func_002E2D00(FI(f, 0x1AD5E0, f32) - AT(f, 0x54, f32)) < 0.0f) {
            if (AT(f->c.motion, 0x55C, s32) != 0x400) {
                func_002DDED0(f->c.motion, 0x400, -1);
            }
        } else if (AT(f->c.motion, 0x55C, s32) != 0x401) {
            func_002DDED0(f->c.motion, 0x401, -1);
        }
    }
    Actor_SetState(&f->c.a, &D_003B2DC8);
}



/* State: turning while standing, to +0x1AD5E0 (10 degrees a frame), between animations: in a
 * move she goes idle once within 90 degrees; turned all the way, her action is cleared and she
 * stands (or, held +0xE0, is held), Progress flag 0x2B off */
void func_0018A210(Fiona *f) {
    static const union { u32 u; f32 f; } k10deg = {0x3E32B8C3}, kHalfPi = {0x3FC90FDB};
    f32 left = func_00124530(&f->c.a, FI(f, 0x1AD5E0, f32), k10deg.f);

    if (!(AT(f->c.motion, 0x550, f32) <= 0.0f)) {
        return;
    }
    if (fiona_motion_kind(AT(f->c.motion, 0x55C, s32)) != 0) {
        if (left < kHalfPi.f) {
            func_001855F0(f, -1);
        }
        return;
    }
    if (left != 0.0f) {
        return;
    }
    AT(f, 0xE1, u8) = 0;
    FI(f, 0x1AD580, s32) = 0;
    AT(f, 0xF8, s32) = 0;
    FI(f, 0x1AD5E0, f32) = AT(f, 0x54, f32);
    FI(f, 0x1AD5C0, s32) = 0;
    FI(f, 0x1AD588, u8) = 0;
    if (AT(f, 0xE0, u8) == 0) {
        AT(f, 0x2D, u8) = 0;
        if (AT(f->c.motion, 0x4C4, void *) != NULL) {
            func_001855F0(f, -1);
        }
        Actor_SetState(&f->c.a, &D_003B25A8);
    } else {
        Actor_SetState(&f->c.a, &D_003B25B8);
    }
    Progress_ClearFlag(gProgress, 0x2B);
}


extern void func_00125960(Fiona *f);   /* a character's step (base) */

/* State step: +0x2B set clears +0x1AD5BC first */
void func_0018B570(Fiona *f) {
    if (AT(f, 0x2B, u8) == 1) {
        FI(f, 0x1AD5BC, u8) = 0;
    }
    func_00125960(f);
}


/* the run's look, each frame (+0xFC 2 while idle +0xF8), as the walk's (func_00185CF0) with the
 * run 0x202 and its blends 0x203 / 0x205; tired (+0x1AD584 bit 1): the tired run 0x206 alone */
void func_00185310(Fiona *f) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD};
    s32 cur, next, change;
    f32 a, b, d;

    if (AT(f, 0xF8, s32) == 0) {
        AT(f, 0xFC, s32) = 2;
    }
    next = AT(f->c.motion, 0x560, s32);
    cur = AT(f->c.motion, 0x55C, s32);
    if (FI(f, 0x1AD584, s32) & 2) {
        if (cur != 0x206) {
            func_002DDED0(f->c.motion, 0x206, -1);
            AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = 1.0f;
        }
        return;
    }
    a = (100.0f - FI(f, 0x1AD5F4, f32)) / 60.0f;
    b = (f32)(0x708 - FI(f, 0x1AD5F8, s32)) / 1800.0f;
    change = 0;
    if (b < 0.5f || !(a + 0.25f <= b)) {
        if (cur == 0x202 && next == 0x205) {
            d = b - FI(f, FWALK_BLEND, f32);
            if (d <= 0.0f) {
                d = -d;
            }
            if (!(d <= k01.f)) {
                change = 1;
            }
        } else {
            change = 1;
        }
        if (change == 1) {
            func_002DDED0(f->c.motion, 0x202, 0x205);
        }
        AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = b;
        FI(f, FWALK_BLEND, f32) = b;
        return;
    }
    if (a < 1.0f) {
        if (cur == 0x202 && next == 0x203) {
            d = a - FI(f, FWALK_BLEND, f32);
            if (d <= 0.0f) {
                d = -d;
            }
            if (!(d <= k01.f)) {
                change = 1;
            }
        } else {
            change = 1;
        }
        if (change == 1) {
            func_002DDED0(f->c.motion, 0x202, 0x203);
        }
        AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = a;
        FI(f, FWALK_BLEND, f32) = a;
        return;
    }
    if (!(cur == 0x202 && next == -1)) {
        func_002DDED0(f->c.motion, 0x202, -1);
    }
    AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = 1.0f;
}


extern s32 func_00177890(Progress *p, s32 a, s32 b, u8 from, u8 to, s32 c, f32 d);   /* u8 */
extern const PTMF D_003B2B58, D_003B2B68, D_003B2B78, D_003B2B88, D_003B2B98, D_003B2BA8, D_003B2BB8,
    D_003B2BC8, D_003B2BD8, D_003B2BE8, D_003B2BF8, D_003B2C08;   /* her states after an action */

/* the character `c` is in sight of her (their nav triangles and positions, no mask) */
static inline s32 fiona_in_sight(Fiona *f, Character *c) {
    return (u8)func_00122C90(f, f->c.a.navTri, c->a.navTri, f->c.a.pos, c->a.pos, 0) == 1;
}

extern const PTMF D_003B2C18;

/* back to an idle animation after action moveSub 0x23..0x2F (progress flag 0x25: at random
 * 1 or 0xC02) */
void func_00184570(Fiona *f) {
    if ((u8)Progress_TestFlag(gProgress, 0x25) != 0) {
        if (VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) < 0.5f) {
            func_002DDED0(f->c.motion, 1, -1);
        } else {
            func_002DDED0(f->c.motion, 0xC02, -1);
        }
        return;
    }
    switch (f->c.moveSub) {
    case 0x29:
    case 0x2F:
        func_002DDED0(f->c.motion, 0xC02, -1);
        break;
    case 0x2E:
        func_002DDED0(f->c.motion, 0xC0E, -1);
        break;
    case 0x23:
        func_002DDED0(f->c.motion, 0xC04, -1);
        break;
    case 0x24:
        func_002DDED0(f->c.motion, 0xC06, -1);
        break;
    case 0x2C:
        func_002DDED0(f->c.motion, 0xC02, -1);
        break;
    case 0x2A:
        func_002DDED0(f->c.motion, 0xC0A, -1);
        break;
    case 0x25:
    case 0x27:
        func_002DDED0(f->c.motion, 0xC03, -1);
        break;
    case 0x2D:
        func_002DDED0(f->c.motion, 0xC00, -1);
        break;
    }
}

/* the looking-around idle (animation 0xC0E, then 0xC0F): the one looked at stays the one to
 * face; func_00183F10 on its motion event 2; at its end back to idle */
void func_0018F580(Fiona *f) {
    void *m;

    if (FI(f, 0x1AD664, Character *) != NULL) {
        FI(f, 0x1AD5FC, u8) = 1;
        FI(f, 0x1AD600, Character *) = FI(f, 0x1AD664, Character *);
    }
    if ((u8)func_001F4770(f->c.motion, 0, 0, 1) & 2) {
        func_00183F10(f);
    }
    m = f->c.motion;
    if ((MOTION_EVENTS(m) & 0x20) != 0) {
        if (*(s32 *)((u8 *)m + 0x55C) == 0xC0E) {
            func_002DDE20(m, 0xC0F, -1);
            return;
        }
        Fiona_ToIdle(f);
    }
}

/* the end of a looking action: (unless progress flag 0x25, which forgets it) the one looked at
 * (+0x1AD664) stays the one to face (+0x1AD5FC set, +0x1AD600); once the animation is over
 * func_00184570 and the next state D_003B2C18 */
void func_0018F760(Fiona *f) {
    if ((u8)Progress_TestFlag(gProgress, 0x25) != 0) {
        FI(f, 0x1AD664, Character *) = NULL;
    } else if (FI(f, 0x1AD664, Character *) != NULL) {
        FI(f, 0x1AD5FC, u8) = 1;
        FI(f, 0x1AD600, Character *) = FI(f, 0x1AD664, Character *);
    }
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        func_00184570(f);
        Actor_SetState(&f->c.a, &D_003B2C18);
    }
}

/* an action's state once its animation (motion +0x550) is over, by the action (moveSub +0xFC; 0x23 ..
 * 0x2F): she may glance at Hewie (+0x1AD5D5 he can act) or the pursuer (+0x1AD5D7) when in
 * sight (+0x1AD664 the one looked at, cleared first). 0x24 (progress test 2/4 for her slot)
 * goes back to idle unless it passes; 0x2B and 0x28 change the action to
 * 0x2F / 0x29 when theirs (2/0, 2/2) fail. Then the character's update (func_00125A10). */
void func_0018F870(Fiona *f) {
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        f32 tmp[4] __attribute__((aligned(16)));

        FI(f, 0x1AD664, Character *) = NULL;
        switch (f->c.moveSub) {
        case 0x23:
            if (FI(f, 0x1AD5D5, u8) == 1 && fiona_in_sight(f, gCharPartner)) {
                FI(f, 0x1AD664, Character *) = gCharPartner;
            }
            Actor_SetState(&f->c.a, &D_003B2B58);
            break;
        case 0x2D:
            if (FI(f, 0x1AD5D7, u8) == 1 && fiona_in_sight(f, gCharPursuer)) {
                FI(f, 0x1AD664, Character *) = gCharPursuer;
            } else if (FI(f, 0x1AD5D5, u8) == 1 && fiona_in_sight(f, gCharPartner)) {
                FI(f, 0x1AD664, Character *) = gCharPartner;
            }
            Actor_SetState(&f->c.a, &D_003B2B68);
            break;
        case 0x2A:
        case 0x2E:
            if (FI(f, 0x1AD5D5, u8) == 1 && fiona_in_sight(f, gCharPartner)) {
                FI(f, 0x1AD664, Character *) = gCharPartner;
            }
            Actor_SetState(&f->c.a, f->c.moveSub == 0x2A ? &D_003B2B78 : &D_003B2B88);
            break;
        case 0x29:
        case 0x2C:
        case 0x2F:
            if (FI(f, 0x1AD5D4, u8) == 1) {
                if (FI(f, 0x1AD5D5, u8) == 0) {
                    if (f->c.door != 0xFF && FI(f, 0x1AD5F0, s32) != 0) {
                        VCALL(D_0044E568, 0x30, void (*)(VObject *, u8, f32 *))(D_0044E568, f->c.door, tmp);
                    }
                } else if (fiona_in_sight(f, gCharPartner)) {
                    FI(f, 0x1AD664, Character *) = gCharPartner;
                }
            }
            Actor_SetState(&f->c.a, &D_003B2B98);
            break;
        case 0x27:
            if (FI(f, 0x1AD5D4, u8) == 1) {
                if (FI(f, 0x1AD5D5, u8) != 1) {
                    if (f->c.door != 0xFF && FI(f, 0x1AD5F0, s32) != 0) {
                        VCALL(D_0044E568, 0x30, void (*)(VObject *, u8, f32 *))(D_0044E568, f->c.door, tmp);
                    }
                } else {
                    FI(f, 0x1AD664, Character *) = gCharPartner;
                }
            }
            Actor_SetState(&f->c.a, &D_003B2BA8);
            break;
        case 0x24:
            if ((u8)func_00177890(gProgress, 2, 4, (u8)f->c.a.slot, 1, 0, 0.0f) != 1) {
                Fiona_ToIdle(f);
            } else {
                Actor_SetState(&f->c.a, &D_003B2BB8);
            }
            break;
        case 0x2B:
            if ((u8)func_00177890(gProgress, 2, 0, (u8)f->c.a.slot, 1, 0, 0.0f) != 1) {
                f->c.moveSub = 0x2F;
                Actor_SetState(&f->c.a, &D_003B2BD8);
            } else {
                Actor_SetState(&f->c.a, &D_003B2BC8);
            }
            break;
        case 0x28:
            if ((u8)func_00177890(gProgress, 2, 2, (u8)f->c.a.slot, 1, 0, 0.0f) != 1) {
                f->c.moveSub = 0x29;
                Actor_SetState(&f->c.a, &D_003B2BF8);
            } else {
                Actor_SetState(&f->c.a, &D_003B2BE8);
            }
            break;
        case 0x25:
            if (FI(f, 0x1AD5D4, u8) == 1) {
                FI(f, 0x1AD664, Character *) = gCharPartner;
            }
            Actor_SetState(&f->c.a, &D_003B2C08);
            break;
        }
    }
    func_00125A10(&f->c);
}

/* ---- Fiona taking a hit or being caught (state block [0] 4) ---- */

extern u32 func_00177BF0(Progress *p, u32 exit, u32 slot);   /* exit bits for a character */
extern u32 func_00177A20(Progress *p, u32 door, u32 slot);   /* door-region bits for a character */
extern void func_002EFA50(u8 *panic, f32 amount);           /* panic up by amount */
extern const PTMF D_003B2DD8, D_003B2DE8, D_003B2DF8, D_003B2E08, D_003B2E18, D_003B2E28, D_003B2E38;
extern const PTMF D_003B2E48, D_003B2E58, D_003B2E68, D_003B2E78, D_003B2E88;

#define AREA_HOLY 0x88   /* the current area (D_0044E988 +0x10 (1)) where she cannot be caught */

/* the reaction for request `req` (the state block's [1]), -1: none. While an event runs or
 * progress flag 8 is set, none; a request about character [2] needs it present. 0xD falls
 * 0x13; 1 / 2 / 4 knocked down 0xE / 0xF / 0xA (on a slope, stairs or at a door: 0xC / 0xD;
 * crawling (3/7): 8); 3 grabbed 0x10 (already: a shake); 5 carried by door [4] 0xB; 6 caught
 * 0x20; 0xA 9; 0xC 0x12 - none while already reacting (4) or down (3 / 0xA) */
s32 func_00184E00(Fiona *f, s32 req) {
    Progress *p;
    s32 m;
    u32 i;
    s32 n;

    if (VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) != 0) {
        return -1;
    }
    p = gProgress;
    if ((Progress_TestFlag(p, 8) & 0xFF) == 1) {
        return -1;
    }
    if (req == 5) {
        if (!(VCALL(D_0044E558, 0x40, u32 (*)(VObject *, u32))(D_0044E558, (u8)f->c.state[4]) & 0xFF)) {
            return -1;
        }
    } else if (f->c.state[2] != 0xFF) {
        Character *c = gCharacters[f->c.state[2]];

        if (c == NULL || (c->a.active == 0 && c->a.disabled == 1)) {
            return -1;
        }
    }
    m = f->c.moveMode;
    switch (req) {
    case 0xD:
        if (m == 4 || m == 0xA || m == 3) {
            return -1;
        }
        return 0x13;
    case 1:
    case 2:
    case 4:
        if (m == 4 || m == 0xA) {
            return -1;
        }
        if (m == 3 && f->c.moveSub == 7) {
            return 8;
        }
        if (NavMesh_TriFlags(D_0044E570, f->c.a.navTri) & 0x80003) {   /* (off the mesh: address 0x3C) */
            return req == 1 ? 0xC : 0xD;
        }
        for (i = 0; i < 8; i = (i + 1) & 0xFF) {
            if (func_00177BF0(p, i, (u8)f->c.a.slot) & 0xFF & 0x20) {
                return req == 1 ? 0xC : 0xD;
            }
        }
        n = D_0044E570->numDoors;
        for (i = 0; (s32)i < n; i++) {
            if (func_00177A20(p, i & 0xFF, (u8)f->c.a.slot) & 0xFF & 8) {
                return req == 1 ? 0xC : 0xD;
            }
        }
        if (req == 1) {
            return 0xE;
        }
        if (req == 2) {
            return 0xF;
        }
        if (req == 4) {
            return 0xA;
        }
        return -1;
    case 3:
        if (m == 4 && f->c.moveSub == 0x10) {
            VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, 0xFF, 0x10);
            return -1;
        }
        return 0x10;
    case 5:
        if (m == 4 && f->c.moveSub == 0xB && f->c.unk104[0] == f->c.state[4]) {
            return -1;
        }
        return 0xB;
    case 6:
        if (m == 4 || m == 0xA || (m == 0 && f->unk1AD580 == 0xF)) {
            return -1;
        }
        if (D_0044E988 != NULL && VCALL(D_0044E988, 0x10, s32 (*)(VObject *, s32))(D_0044E988, 1) == AREA_HOLY) {
            return 0x20;
        }
        if (m == 3 && f->c.moveSub == 7) {
            return 8;
        }
        return 0x20;
    case 0xA:
        if (m == 4 || m == 3 || m == 0xA) {
            return -1;
        }
        return 9;
    case 0xC:
        if (m == 4) {
            if (f->c.moveSub != 0x12) {
                return -1;
            }
        } else if (m == 3 || m == 0xA) {
            return -1;
        }
        return 0x12;
    }
    return -1;
}

/* react to the state block `st` ([1] the request, [2] by whom, [4] its detail, [5] the panic it
 * adds): action 4 with the reaction func_00184E00 picked (-1: none), the rumble, and its state */
s32 func_00182340(Fiona *f, s32 *st) {
    s32 kind = func_00184E00(f, st[1]);
    Progress *p;

    if (kind == -1) {
        return -1;
    }
    p = gProgress;
    func_002EFA50((u8 *)p + 0x7B8, *(f32 *)&st[5]);
    if (kind == 0x20 && D_0044E988 != NULL && VCALL(D_0044E988, 0x10, s32 (*)(VObject *, s32))(D_0044E988, 1) == AREA_HOLY) {
        return -1;
    }
    f->targetParam = 0;
    f->c.unk14D0 = 0;
    func_001F6E10(f->c.motion);
    f->c.a.unk2D = 1;
    func_00184BF0(f);
    f->c.unk100 = st[2];
    f->unk1AD580 = 0xA;
    f->c.moveMode = 4;
    f->c.moveSub = kind;
    VCALL(D_0044E560, 0x10, void (*)(VObject *, s32, s32, s32))(D_0044E560, 0, 0x800000, 4);
    switch (kind) {
    case 0x13: {
        NavMesh *nav;
        u32 i;
        s32 n;

        VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, 0xA0, 8);
        nav = D_0044E570;
        f->c.a.unk2A = 0;
        FI(f, 0x1AD6C0, s32) = 1;
        if (NavMesh_TriFlags(nav, f->c.a.navTri) & 0x80003) {
            FI(f, 0x1AD6C0, s32) = 0;
        }
        if (FI(f, 0x1AD6C0, s32) != 0) {
            for (i = 0; i < 8; i = (i + 1) & 0xFF) {
                if (func_00177BF0(p, i, (u8)f->c.a.slot) & 0xFF & 0x20) {
                    FI(f, 0x1AD6C0, s32) = 0;
                    break;
                }
            }
        }
        if (FI(f, 0x1AD6C0, s32) != 0) {
            n = nav->numDoors;
            for (i = 0; (s32)i < n; i++) {
                if (func_00177A20(p, i & 0xFF, (u8)f->c.a.slot) & 0xFF & 8) {
                    FI(f, 0x1AD6C0, s32) = 0;
                    break;
                }
            }
        }
        Actor_SetState(&f->c.a, &D_003B2E88);
        break;
    }
    case 0x12:
        f->c.a.unk2D = 0;
        f->c.a.unk2A = 1;
        FI(f, 0x1AD710, u8) = 1;
        FI(f, 0x1AD714, s32) = 0;
        FI(f, 0x1AD6C0, s32) = 0xB4;
        FI(f, 0x1AD6C4, s32) = 0x3C;
        Actor_SetState(&f->c.a, &D_003B2E78);
        break;
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
        VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, kind != 0xE && kind != 0xC ? 0xA0 : 0x80, 8);
        if (f->c.unk100 != 0xFF && f->c.unk100 != 1) {
            func_00177630(p, 1);
        }
        if (st[4] & 0x8000) {
            func_00182E80(f);
        }
        f->c.a.unk2A = 0;
        Actor_SetState(&f->c.a, &D_003B2E68);
        break;
    case 0xB:
        VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, 0xD0, 0xC);
        func_00177630(p, 1);
        f->c.a.unk2A = 1;
        f->c.unk104[0] = st[4];
        Actor_SetState(&f->c.a, &D_003B2E58);
        break;
    case 0xA:
        VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, 0xD0, 0xC);
        if (f->c.unk100 != 0xFF && f->c.unk100 != 1) {
            func_00177630(p, 1);
        }
        if (st[4] & 0x8000) {
            func_00182E80(f);
        }
        f->c.a.unk2A = 1;
        Actor_SetState(&f->c.a, &D_003B2E48);
        break;
    case 9:
        func_00177630(p, 1);
        f->c.a.unk2A = 1;
        FI(f, 0x1AD710, u8) = 1;
        FI(f, 0x1AD714, s32) = 0;
        f->c.unk104[0] = FI(f, 0x1AD6F0, s32);
        FI(f, 0x10C, f32) = FI(f, 0x1AD6F4, f32);
        sceVu0CopyVector(f->c.unk110, (f32 *)((u8 *)f + 0x1AD700));
        f->c.unk104[1] = st[4];
        FI(f, 0x1AD6C8, s32) = 0;
        func_00122C20(&f->c.a, 0x43, 5, 0, 0, NULL);
        Actor_SetState(&f->c.a, f->c.unk104[1] == 6 ? &D_003B2E28 : &D_003B2E38);
        break;
    case 8:
        func_00177630(p, 1);
        f->c.a.unk2A = 1;
        Actor_SetState(&f->c.a, &D_003B2E18);
        break;
    case 0x10:
        f->c.a.unk2D = 0;
        Progress_SetFlag(p, 0x2B);
        VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, 0xFF, 0x10);
        f->c.a.unk2A = 0;
        FI(f, 0x1AD6CC, s32) = 0;
        Actor_SetState(&f->c.a, &D_003B2E08);
        break;
    case 0x20:
        if (D_0044E988 != NULL && VCALL(D_0044E988, 0x10, s32 (*)(VObject *, s32))(D_0044E988, 1) == AREA_HOLY) {
            return -1;
        }
        if (st[4] != 5) {
            f->c.a.unk2D = 0;
            f->unk1AD580 = 8;
            f->c.moveMode = 0xB;
            switch (st[4]) {
            case 3:
                f->c.unk104[0] = 3;
                f->target = gCharPursuer;
                break;
            case 1:
                if (NavMesh_TriFlags(D_0044E570, f->c.a.navTri) & 0x80001) {
                    f->c.unk104[0] = 3;
                } else {
                    f32 h = func_001244D0(&f->c.a, gCharPursuer->a.pos);
                    f32 d;

                    if (!(func_002E2D00(h - f->c.a.angle[1]) <= 0.0f)) {
                        d = func_002E2D00(h - f->c.a.angle[1]);
                    } else {
                        d = -func_002E2D00(h - f->c.a.angle[1]);
                    }
                    f->c.unk104[0] = d < 0x1.921fb6p+0f /* pi/2 */ ? 1 : 3;
                }
                f->target = gCharPursuer;
                break;
            case 2:
                f->c.unk104[0] = 2;
                break;
            case 4:
                f->c.unk104[0] = 4;
                break;
            }
            Actor_SetState(&f->c.a, &D_003B2DF8);
        } else if ((func_001235C0(f, &f->c.a) & 0xFF) == 1) {
            VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, 0xD0, 0xC);
            f->c.unk100 = 0xFF;
            f->c.a.unk2A = 1;
            Actor_SetState(&f->c.a, &D_003B2DD8);
        } else {
            VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, 0x80, 8);
            f->c.unk100 = 2;
            f->c.a.unk2A = 0;
            f->c.moveSub = 0xC;
            Actor_SetState(&f->c.a, &D_003B2DE8);
        }
        break;
    }
    return 0;
}

/* ---- the reaction states (action 4) ---- */

extern const PTMF D_003B2D38, D_003B2D48;

/* 0x12 (D_003B2E78): once the motion has played out, the hold (0x1405, or 0x1406 already) */
void func_0018BFC0(Fiona *f) {
    void *m = f->c.motion;

    if (!(AT(m, 0x550, f32) <= 0.0f)) {
        return;
    }
    if (AT(m, 0x55C, s32) != 0x1405 && AT(m, 0x55C, s32) != 0x1406) {
        func_002DDED0(m, 0x1405, -1);
    }
    Actor_SetState(&f->c.a, &D_003B2D38);
}

/* 0x13 (D_003B2E88): once the motion has played out, a cry (0x3F) and a fall (noise 0x5F) -
 * where she may (+0x1AD6C0) by the side her character slot's noise point (gProgress +0x1060)
 * is on: ahead 0x1004, behind 0x1005, left / right 0x1007 / 0x1006; else straight down
 * (0x100F) */
void func_0018BA70(Fiona *f) {
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        Progress *p = gProgress;
        f32 d[4] __attribute__((aligned(16)));
        f32 a, aa;

        sceVu0SubVector(d, f->c.a.pos, (f32 *)((u8 *)p + 0x1060 + f->c.a.slot * 0x20));
        a = func_002E2D00(func_0031C5C0(d[0], d[2]) - f->c.a.angle[1]);
        aa = a <= 0.0f ? -a : a;
        if (FI(f, 0x1AD6C0, s32) == 0) {
            func_00122C20(&f->c.a, 0x3F, 5, 0, 0, NULL);
            func_002DDED0(f->c.motion, 0x100F, -1);
        } else {
            func_00122C20(&f->c.a, 0x3F, 5, 0, 0, NULL);
            if (aa < 0x1.0c1524p+0f /* 60 deg */) {
                func_002DDED0(f->c.motion, 0x1004, -1);
            } else if (!(aa <= 0x1.0c1524p+1f /* 120 deg */)) {
                func_002DDED0(f->c.motion, 0x1005, -1);
            } else if (a < 0.0f) {
                func_002DDED0(f->c.motion, 0x1007, -1);
            } else {
                func_002DDED0(f->c.motion, 0x1006, -1);
            }
        }
        func_002A8440((u8 *)p + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
        Actor_SetState(&f->c.a, &D_003B2D48);
    }
    func_00125A10(&f->c);
}

extern const PTMF D_003B2998, D_003B29A8;
extern u32 func_00123710(void *self, s32 door, s32 side, const f32 *ofs, f32 *out);   /* a point by a door */

/* 0x20 caught (D_003B2DF8): once the motion has played out, the rumble, a cry (0x43) and the
 * caught motion by how (+0x104): 1 0xF02, 2 0xF04, 3 0xF03, 4 0xF05 (1, and 3 with someone
 * holding her, set +0x1AD5FC) */
void func_00194230(Fiona *f) {
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 0, 0x80, 8);
        func_00122C20(&f->c.a, 0x43, 5, 0, 0, NULL);
        switch (f->c.unk104[0]) {
        case 4:
            func_002DDED0(f->c.motion, 0xF05, -1);
            break;
        case 1:
            FI(f, 0x1AD5FC, u8) = 1;
            func_002DDED0(f->c.motion, 0xF02, -1);
            break;
        case 2:
            func_002DDED0(f->c.motion, 0xF04, -1);
            break;
        case 3:
            if (f->target != NULL) {
                FI(f, 0x1AD5FC, u8) = 1;
            }
            func_002DDED0(f->c.motion, 0xF03, -1);
            break;
        }
        Actor_SetState(&f->c.a, &D_003B2998);
    }
    func_00125A10(&f->c);
}

/* 8 (D_003B2E18), caught while crawling: dragged to door +0x104's side 1, 15 out
 * (+0x1AD6E0, +0x1AD6C8 its triangle); a drop under 20 a short pull (0xF02, cry 0x3F, action
 * 0xB), else a fall (0x70A, cry 0x40, +0x1AD6C4); the frames to get there (+0x1AD6C0) falling
 * 0, 0.5, 1, ... a frame, the step across (+0x1AD6D0) */
void func_00193E90(Fiona *f) {
    f32 ofs[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 y, step;
    s32 n;

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    ofs[0] = 0.0f;
    ofs[2] = 10.0f;
    ofs[1] = 0.0f;
    FI(f, 0x1AD6C8, s32) = func_00123710(f, f->c.unk104[0], 1, ofs, (f32 *)((u8 *)f + 0x1AD6E0));
    sceVu0SubVector(d, f->c.a.pos, (f32 *)((u8 *)f + 0x1AD6E0));
    if (d[1] < 20.0f) {
        FI(f, 0x1AD6C4, s32) = 0;
        f->c.moveMode = 0xB;
        func_002DDED0(f->c.motion, 0xF02, -1);
        func_00122C20(&f->c.a, 0x3F, 5, 0, 0, NULL);
    } else {
        FI(f, 0x1AD6C4, s32) = 1;
        func_002DDED0(f->c.motion, 0x70A, -1);
        func_00122C20(&f->c.a, 0x40, 5, 0, 0, NULL);
    }
    y = d[1];
    step = 0.0f;
    n = 1;
    for (;;) {
        y = y - step;
        if (y < 0.0f) {
            break;
        }
        step = step + 0.5f;
        n++;
    }
    d[1] = y;
    FI(f, 0x1AD6C0, s32) = n;
    FI(f, 0x1AD6D0, f32) = d[0] / (f32)n;
    FI(f, 0x1AD6D4, s32) = 0;
    FI(f, 0x1AD6D8, f32) = d[2] / (f32)n;
    Actor_SetState(&f->c.a, &D_003B29A8);
}

extern const PTMF D_003B2B48;

/* turn her along the wall her motion this frame (motion +0x60) runs into: follow the mesh
 * from her triangle (nav +0x20 the edge crossed; 3 / 4 none) to the first blocking edge, take
 * its direction (+0x1AD5E0, flipped to within 90 degrees of her heading) and turn toward it by
 * a quarter of the difference (at least half a degree), the way +0x1AD6C0 chose (-1: by the
 * side it is on) */
void func_00183400(Fiona *f) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kHalfPi = {0x3FC90FDB}, kMin = {0x3C0EFA35};
    f32 mv[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    NavMesh *nav;
    NavTri *t;
    u32 tri, wall;
    s32 e, e1;
    f32 a, aa, step, yaw;

    VCALL(f->c.motion, 0x60, void (*)(void *, f32 *))(f->c.motion, mv);
    nav = D_0044E570;
    tri = f->c.a.navTri;
    for (;;) {
        wall = tri;
        e = VCALL((VObject *)nav, 0x20, s32 (*)(VObject *, u32, f32 *, f32 *))((VObject *)nav, tri, f->c.a.pos, mv);
        if (e == 3 || e == 4) {
            return;
        }
        t = NavMesh_Tri(nav, tri);
#ifdef HG_NATIVE
        if (t == NULL) {
            return;
        }
#endif
        tri = t->adj[e];
        if (tri == NAV_NONE) {
            break;
        }
        t = NavMesh_Tri(nav, tri);
#ifdef HG_NATIVE
        if (t == NULL) {
            break;
        }
#endif
        if (t->flags & f->c.a.navMask) {
            break;
        }
    }
    e1 = e + 1;
    if ((u32)e1 >= 3) {
        e1 = 0;
    }
    t = NavMesh_Tri(nav, wall);
#ifdef HG_NATIVE
    if (t == NULL) {
        return;
    }
#endif
    sceVu0SubVector(d, t->v[e1], t->v[e]);
    f->savedYaw = func_0031C5C0(d[0], d[2]);
    if (!(func_002E2D00(f->savedYaw - f->c.a.angle[1]) <= 0.0f)) {
        aa = func_002E2D00(f->savedYaw - f->c.a.angle[1]);
    } else {
        aa = -func_002E2D00(f->savedYaw - f->c.a.angle[1]);
    }
    if (!(aa <= kHalfPi.f)) {
        f->savedYaw = func_002E2D00(kPi.f + f->savedYaw);
    }
    a = func_002E2D00(f->savedYaw - f->c.a.angle[1]);
    step = 0.25f * (a <= 0.0f ? -a : a);
    if (step < kMin.f) {
        step = kMin.f;
    }
    if (FI(f, 0x1AD6C0, s32) == -1) {
        FI(f, 0x1AD6C0, s32) = a < 0.0f ? 0 : 1;
    }
    if (FI(f, 0x1AD6C0, s32) != 0) {
        yaw = func_002E2D00(f->c.a.angle[1] + step);
    } else {
        yaw = func_002E2D00(f->c.a.angle[1] - step);
    }
    f->c.a.angle[1] = yaw;
    sceVu0UnitMatrix(f->c.a.rot);
    sceVu0RotMatrixY(f->c.a.rot, f->c.a.rot, yaw);
}

/* 0x10 grabbed (D_003B2E08): out of a fall (0x100A / 0xB01 / 0xB02) she gets up (0xB03), out
 * of 0x100D (0x1503) - once that motion lets her (flag 2); else she pulls free: back 0x1101
 * when there is room 17 behind her (+0x1AD6C0 / +0x1AD6C4 -1, her heading kept in +0x1AD6D0),
 * else 0x1100. Pulling back she is moved by the motion turned to that heading (once +0x1AD6C4
 * is set) and turned toward it 20 degrees a frame */
void func_00190380(Fiona *f) {
    void *m;
    s32 cur;
    u8 done = 0, back = 0;

    FI(f, 0x1AD5BC, u8) = 0;
    m = f->c.motion;
    cur = AT(m, 0x55C, s32);
    if (cur == 0x100D) {
        if (!(func_001F4770(m, 0, 0, 1) & 0xFF & 2)) {
            done = 1;
            func_002DDED0(f->c.motion, 0x1503, -1);
        }
    } else if (cur == 0x100A || cur == 0xB02 || cur == 0xB01) {
        if (!(func_001F4770(m, 0, 0, 1) & 0xFF & 2)) {
            done = 1;
            func_002DDED0(f->c.motion, 0xB03, -1);
        }
    }
    if (done == 0) {
        FI(f, 0x1AD6C0, s32) = -1;
        FI(f, 0x1AD6C4, s32) = -1;
        FI(f, 0x1AD6D0, f32) = f->c.a.angle[1];
        if (!(func_001235C0(f, &f->c.a) & 0xFF)) {
            func_002DDED0(f->c.motion, 0x1100, -1);
        } else {
            f32 v[4] __attribute__((aligned(16)));

            v[2] = 17.0f;
            v[0] = 0.0f;
            v[1] = 0.0f;
            func_002E2DA0(v, f->c.a.rot, v);
            sceVu0AddVector(v, v, f->c.a.pos);
            if (func_00124480(&f->c.a, v, 0x80001) == (u32)-1) {
                func_002DDED0(f->c.motion, 0x1100, -1);
            } else {
                back = 1;
                func_002DDED0(f->c.motion, 0x1101, -1);
            }
        }
    }
    if (back == 1) {
        if (FI(f, 0x1AD6C4, s32) == -1) {
            func_00125A10(&f->c);
        } else {
            f32 d[4] __attribute__((aligned(16)));
            f32 r[4][4] __attribute__((aligned(16)));

            func_001F6370(f->c.motion, d, 0.0f);
            sceVu0UnitMatrix(r);
            sceVu0RotMatrixY(r, r, FI(f, 0x1AD6D0, f32));
            sceVu0ApplyMatrix(d, r, d);
            func_001247E0(&f->c.a, d);
            if (FI(f, 0x1AD6C4, s32) == 0 &&
                func_00124530(&f->c.a, FI(f, 0x1AD6D0, f32), 0x1.657186p-2f /* 20 deg */) == 0.0f) {
                FI(f, 0x1AD6C4, s32) = 1;
            }
        }
        func_00183400(f);
    } else {
        func_00125A10(&f->c);
    }
    FI(f, 0x1AD6C8, s32) = 0;
    Actor_SetState(&f->c.a, &D_003B2B48);
}

extern const PTMF D_003B2AB8;

/* 0xA / 0x20 thrown (D_003B2E48, D_003B2DD8): once the motion has played out she falls away
 * from whoever did it ([2] +0x100; 0xFF: straight on, +0x1AD6C4 -1): forward (0x1008) or
 * backward (0x100B, or 0xB04 facing back for a hard fall, +0x1AD584 bit 2, with a scream 0x42,
 * noise 0x6F and the panic at 1000), else a cry (0x40) and noise 0x5F; +0x1AD6C8 which way.
 * Her update runs with nav flag 1 not blocking */
void func_00191800(Fiona *f) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kHalfPi = {0x3FC90FDB};

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        f32 aa;

        if (f->c.unk100 == 0xFF) {
            FI(f, 0x1AD6C4, s32) = -1;
            FI(f, 0x1AD6D0, f32) = f->c.a.angle[1];
        } else {
            f32 d[4] __attribute__((aligned(16)));

            FI(f, 0x1AD6C4, s32) = 0;
            sceVu0SubVector(d, f->c.a.pos, ((Character *)gCharacters[f->c.unk100])->a.pos);
            FI(f, 0x1AD6D0, f32) = func_0031C5C0(d[0], d[2]);
        }
        if (!(func_002E2D00(FI(f, 0x1AD6D0, f32) - f->c.a.angle[1]) <= 0.0f)) {
            aa = func_002E2D00(FI(f, 0x1AD6D0, f32) - f->c.a.angle[1]);
        } else {
            aa = -func_002E2D00(FI(f, 0x1AD6D0, f32) - f->c.a.angle[1]);
        }
        if (aa < kHalfPi.f) {
            FI(f, 0x1AD6C8, s32) = 0;
            func_002DDED0(f->c.motion, 0x1008, -1);
        } else {
            FI(f, 0x1AD6C8, s32) = 1;
            FI(f, 0x1AD6D0, f32) = func_002E2D00(kPi.f + FI(f, 0x1AD6D0, f32));
            if (FI(f, 0x1AD584, s32) & 2) {
                FI(f, 0x1AD6C4, s32) = -1;
                f->c.a.angle[1] = FI(f, 0x1AD6D0, f32);
                func_002DDED0(f->c.motion, 0xB04, -1);
            } else {
                func_002DDED0(f->c.motion, 0x100B, -1);
            }
        }
        if (FI(f, 0x1AD584, s32) & 2) {
            Progress *p = gProgress;

            AT(p, 0x7D8, f32) = 1000.0f;
            func_00122C20(&f->c.a, 0x42, 5, 0, 0, NULL);
            func_002A8440((u8 *)p + 0x778, 0x6F, f->c.a.room, f->c.a.navTri, 0xFFFF);
        } else {
            func_00122C20(&f->c.a, 0x40, 5, 0, 0, NULL);
            func_002A8440((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
        }
        FI(f, 0x1AD6C0, s32) = -1;
        Actor_SetState(&f->c.a, &D_003B2AB8);
    }
    f->c.a.navMask |= 1;
    func_00125A10(&f->c);
    f->c.a.navMask &= ~1;
}

extern const PTMF D_003B2B08;
extern void func_002DDE20(void *motion, s32 anim, s32 arg);

/* 0xB hit by door [4] (+0x104; D_003B2E58): thrown along the door's swing (its angle, turned
 * round from her side of it) - forward (0x1008) or backward (0x100B; 0xB04 facing back for a
 * hard fall, +0x1AD584 bit 2, with a scream 0x42, noise 0x6F and the panic at 1000), else a cry
 * 0x40 and noise 0x5F; +0x1AD6C8 which way, +0x1AD6D0 the direction */
void func_00190FA0(Fiona *f) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kHalfPi = {0x3FC90FDB};
    Progress *p;
    f32 a, aa;

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    a = VCALL(D_0044E558, 0x3C, f32 (*)(VObject *, u32))(D_0044E558, (u8)f->c.unk104[0]);
    p = gProgress;
    if (!(func_00177BF0(p, (u8)f->c.unk104[0], (u8)f->c.a.slot) & 0xFF & 0x10)) {
        a = func_002E2D00(kPi.f + a);
    }
    FI(f, 0x1AD6C4, s32) = 0;
    if (!(func_002E2D00(a - f->c.a.angle[1]) <= 0.0f)) {
        aa = func_002E2D00(a - f->c.a.angle[1]);
    } else {
        aa = -func_002E2D00(a - f->c.a.angle[1]);
    }
    if (aa < kHalfPi.f) {
        FI(f, 0x1AD6C8, s32) = 0;
        FI(f, 0x1AD6D0, f32) = a;
        func_002DDE20(f->c.motion, 0x1008, -1);
    } else {
        FI(f, 0x1AD6C8, s32) = 1;
        FI(f, 0x1AD6D0, f32) = func_002E2D00(kPi.f + a);
        if (FI(f, 0x1AD584, s32) & 2) {
            FI(f, 0x1AD6C4, s32) = -1;
            f->c.a.angle[1] = FI(f, 0x1AD6D0, f32);
            func_002DDE20(f->c.motion, 0xB04, -1);
        } else {
            func_002DDE20(f->c.motion, 0x100B, -1);
        }
    }
    FI(f, 0x1AD6C0, s32) = -1;
    f->c.a.navMask |= 1;
    func_00125A10(&f->c);
    f->c.a.navMask &= ~1;
    if (FI(f, 0x1AD584, s32) & 2) {
        Progress *q = gProgress;

        AT(q, 0x7D8, f32) = 1000.0f;
        func_00122C20(&f->c.a, 0x42, 5, 0, 0, NULL);
        func_002A8440((u8 *)q + 0x778, 0x6F, f->c.a.room, f->c.a.navTri, 0xFFFF);
    } else {
        func_00122C20(&f->c.a, 0x40, 5, 0, 0, NULL);
        func_002A8440((u8 *)p + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
    }
    Actor_SetState(&f->c.a, &D_003B2B08);
}

extern const PTMF D_003B2B18;

/* the fall by the side the hit came from (`aa` its size, `a` signed): ahead `front`, behind
 * `front` + 1, left / right `front` + 3 / + 2 */
static inline __attribute__((always_inline)) void fall_by_side(Fiona *f, f32 aa, f32 a, s32 front) {
    if (aa < 0x1.0c1524p+0f /* 60 deg */) {
        func_002DDED0(f->c.motion, front, -1);
    } else if (!(aa <= 0x1.0c1524p+1f /* 120 deg */)) {
        func_002DDED0(f->c.motion, front + 1, -1);
    } else if (a < 0.0f) {
        func_002DDED0(f->c.motion, front + 3, -1);
    } else {
        func_002DDED0(f->c.motion, front + 2, -1);
    }
}

/* 0xC..0xF knocked down (D_003B2E68, D_003B2DE8): once the motion has played out, from the side
 * of whoever did it ([2] +0x100, 0xFF: her slot's noise point gProgress +0x1060) - 0xC /
 * 0xE a stumble (cry 0x3E; 0xC on the spot 0x100E, 0xE by side 0x1000..), 0xD / 0xF a fall
 * (cry 0x3F; 0xD 0x100F, 0xF by side 0x1004..), each with noise 0x5F */
void func_00190B50(Fiona *f) {
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        f32 d[4] __attribute__((aligned(16)));
        u8 have = 0;
        f32 a, aa;

        if (f->c.unk100 == 0xFF) {
            have = 1;
            sceVu0SubVector(d, f->c.a.pos, (f32 *)((u8 *)gProgress + 0x1060 + f->c.a.slot * 0x20));
        } else {
            Character *c = gCharacters[f->c.unk100];

            if (c != NULL && c->a.active == 1 && c->a.disabled == 0) {
                have = 1;
                sceVu0SubVector(d, f->c.a.pos, c->a.pos);
            }
        }
        if (have == 1) {
            a = func_002E2D00(func_0031C5C0(d[0], d[2]) - f->c.a.angle[1]);
            aa = a <= 0.0f ? -a : a;
        } else {
            a = f->c.a.angle[1];
            aa = 0.0f;
        }
        switch (f->c.moveSub) {
        case 0xC:
            func_00122C20(&f->c.a, 0x3E, 5, 0, 0, NULL);
            func_002DDED0(f->c.motion, 0x100E, -1);
            func_002A8440((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
            break;
        case 0xE:
            func_00122C20(&f->c.a, 0x3E, 5, 0, 0, NULL);
            fall_by_side(f, aa, a, 0x1000);
            func_002A8440((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
            break;
        case 0xD:
            func_00122C20(&f->c.a, 0x3F, 5, 0, 0, NULL);
            func_002DDED0(f->c.motion, 0x100F, -1);
            func_002A8440((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
            break;
        case 0xF:
            func_00122C20(&f->c.a, 0x3F, 5, 0, 0, NULL);
            fall_by_side(f, aa, a, 0x1004);
            func_002A8440((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
            break;
        }
        Actor_SetState(&f->c.a, &D_003B2B18);
    }
    func_00125A10(&f->c);
}

extern const PTMF D_003B2A98, D_003B29F8;

/* 9 led away: by whoever leads her ([2] +0x100); gone, or no longer leading (action 8), she
 * stands (and a leader not held tells it so: state 7); else a path to the spot +0x104 / +0x110,
 * walked (`anim`) at a fifth of its length a frame, turning to +0x10C at a fifth of the
 * difference, in state `next` */
static inline __attribute__((always_inline)) void led_away(Fiona *f, s32 anim, const PTMF *next) {
    Character *c;
    s32 r;

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    c = gCharacters[f->c.unk100];
    if (c == NULL || c->a.active == 0 || c->a.disabled == 1) {
        Fiona_ToIdle(f);
        return;
    }
    if (c->moveMode != 8) {
        Fiona_ToIdle(f);
        return;
    }
    r = func_00127140(&f->c, 0, f->c.unk104[0], f->c.unk110);
    if (r > 0) {
        r = func_001270F0(&f->c);
    }
    if (r > 0) {
        f32 a;

        FI(f, 0x1AD6D0, f32) = 0x1.99999ap-3f /* 0.2 */ *
            VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
                gSceneGameF29740, f->c.a.pos, f->c.unk128, f->c.unk124, f->c.unk12C);
        func_002DDD20(f->c.motion, anim, -1);
        FI(f, 0x1AD6D8, f32) = FI(f, 0x10C, f32);
        a = func_002E2D00(FI(f, 0x10C, f32) - f->c.a.angle[1]);
        FI(f, 0x1AD6DC, f32) = 0x1.99999ap-3f /* 0.2 */ * (a <= 0.0f ? -a : a);
        Actor_SetState(&f->c.a, next);
        return;
    }
    if (c->state[0] != 7) {
        /* (the original copies a local whose other fields are never set) */
        c->state[0] = 7;
        c->state[1] = 0;
        c->state[2] = 0;
        c->state[3] = 0;
        c->state[4] = 0;
        c->state[5] = 0;
        c->state[6] = 0;
        c->state[7] = 0;
    }
    Fiona_ToIdle(f);
}

/* 9 led away walking (D_003B2E38) */
void func_00191FF0(Fiona *f) {
    led_away(f, 0x1500, &D_003B2A98);
}

/* 9 led away by the hand ([4] 6; D_003B2E28) */
void func_00193400(Fiona *f) {
    led_away(f, 0x1400, &D_003B29F8);
}
