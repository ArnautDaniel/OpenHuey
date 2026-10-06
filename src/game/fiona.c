/* Fiona: the player character (vtable 0x46AAB0). */
#include "common.h"
#include "fiona.h"
#include "progress.h"
#include "navmesh.h"
#include "globals.h"
#include "actor.h"
#include "ptmf.h"
#include "memcard.h"
#include "hewie.h"
#include "model.h"
#include "panic.h"
#include "pursuer.h"
#include "scene_game_members.h"
#include "skeleton.h"
#include "stalker_math.h"
#include "stalker_models.h"
#include "stalker_progress.h"
#include "msl.h"

extern void Fiona_IdleAnim(Fiona *f, s32);

extern const PTMF D_003B25A8;      /* idle state */
extern const PTMF D_003B25B8;      /* idle state (while unkE0 is set) */

/* Motion player byte +0x4D8 (1 = paused?) */
#define MOTION_U8(m, off) (*((u8 *)(m) + (off)))

#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

u32 func_00126EC0(void *p);

extern void *D_0046F580[];
extern void *D_00479600[];
void *StrikeMark_dtor(u8 *o, s32 flags);

#define B7_W(p, off)  (*(s32 *)((u8 *)(p) + (off)))

#define B7_B(p, off)  (*(u8 *)((u8 *)(p) + (off)))

void StrikeMark_Start(u8 *p);

u32 func_00126EC0(void *p) {
    if (FLD(p, 0xF8, s32) == 6) {
        s32 k = FLD(p, 0xFC, s32);

        if (k == 0x16) {
            return FLD(p, 0x14C0, u16);
        }
        if (k == 0x17) {
            s32 i = FLD(p, 0x1388, s32);

            if (i < FLD(p, 0x1384, s32)) {
                return FLD(p, 0x138C + i * 2, u16);
            }
        }
    }
    return 0xFFFF;
}
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
            Fiona_IdleAnim(f, -1);
        }
        Actor_SetState(&f->c.a, &D_003B25A8);
    } else {
        Actor_SetState(&f->c.a, &D_003B25B8);
    }
    Progress_ClearFlag(gProgress, 0x2B);
}

/* vtable +0x7C: back to the idle state. */
/* 0x0019A300 */
void Fiona_BackToIdle(Fiona *f) {
    Fiona_ToIdle(f);
}

/* Set the character (by slot) she interacts with. */
/* 0x0019A420 */
void Fiona_SetTarget(Fiona *f, s32 slot, s32 param) {
    f->targetParam = param;
    f->target = gCharacters[slot];
}

/* vtable +0x60? (deactivate) */
/* 0x0019AF10 */
void Fiona_Deactivate(Fiona *f) {
    Character_Deactivate(&f->c);
}

/* Halt (as Character), and take down her message. */
/* 0x0019AA20 */
void Fiona_Halt(Fiona *f) {
    Character_Enable(&f->c);
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
/* 0x0019AAC0 */
void Fiona_Disable(Fiona *f) {
    Character_Disable(&f->c);
    MOTION_U8(f->c.motion, 0x4D8) = 1;
    f->unk1AD630 = 0;
    f->unk1AD62E = 0;
    if (f->msgImage != NULL) {
        VCALL(gBootMessage, 0x10, void (*)(VObject *, u32, void *, s32))(gBootMessage, f->c.msgSlot, f->msgImage, 0);
    }
}

extern void Fiona_StartInDoor(Fiona *f);

/* Put her in room `room` on triangle `tri`, idle. Returns the placement result. */
/* 0x0019A8C0 */
s32 Fiona_PlaceInRoom(Fiona *f, s32 room, u32 tri, s32 arg3) {
    s32 r;

    Character_ToRoom(&f->c, room, arg3, tri);
    r = VCALL(f, 0x28, s32 (*)(Fiona *, u32, const f32 *, f32 *))(f, tri, NULL, NULL);
    Fiona_ToIdle(f);
    return r;
}

/* Forget path/movement state, then idle. */
/* 0x0019AB40 */
void Fiona_ForgetPath(Fiona *f) {
    Character_ResetBehaviour(&f->c);
    Fiona_StartInDoor(f);
    Fiona_ToIdle(f);
}

/* vtable +0x5C: activate (Character part), then reset her own state; two timers get random
 * lengths (300 + 330 * r frames, 300 + 30 * int(20 * r)). */
/* 0x0019AC70 */
void Fiona_Activate(Fiona *f) {
    VObject *rng;
    s32 t;

    Character_Activate(&f->c);
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
    rng = gRandom;
    FI(f, 0x1AD728, s32) = (s32)(30.0f * (11.0f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng))) + 300;
    FI(f, 0x1AD724, s32) = FI(f, 0x1AD728, s32);
    t = (s32)(20.0f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng));
    FI(f, 0x1AD734, s32) = t * 30 + 300;
    FI(f, 0x1AD71C, s32) = 0;
    *(f32 *)&f->c.unk14C4 = -1.0f;
}

#include "sce/libvu0.h"

#define MOTION_SKELETON(m) (*(void **)((u8 *)(m) + 0x810))

/* Point of interest on her for action 8 (sub 0x1A/0x1B), e.g. for the camera: a bone or an
 * offset in front of her. False if there is none. */
/* 0x0019A450 */
s32 Fiona_PointOfInterest(Fiona *f, f32 *out) {
    sceVu0FVECTOR v;

    if (f->c.moveMode != 8) {
        return 0;
    }
    if (f->c.moveSub == 0x1B) {
        if ((Motion_EventFlags(f->c.motion, 0, 0, 1) & 0xFF) & 0x2) {
            return 0;
        }
        *(s32 *)&v[0] = 0;
        v[1] = 10.0f;
        v[2] = f->c.a.radius;
        Mtx_ApplyVector(v, f->c.a.rot, v);
        sceVu0AddVector(out, f->c.a.pos, v);
        return 1;
    }
    if (f->c.moveSub != 0x1A) {
        return 0;
    }
    if (!((Motion_EventFlags(f->c.motion, 0, -1, 1) & 0xFF) & 0x2)) {
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

        sceVu0CopyMatrix(m, (void *)Skel_Bone(MOTION_SKELETON(f->c.motion), bone));
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

        sceVu0CopyVector(out, Skel_Bone(MOTION_SKELETON(f->c.motion), bone) + 12);
        return 1;
    }
    }
}

/* Can she start interaction `kind` now (with character `otherSlot`, 0xFF = none; door `door`
 * for kind 5)? Depends on what she is doing (moveMode/moveSub). */
/* 0x0019A670 */
s32 Fiona_CanInteract(Fiona *f, u32 kind, u32 otherSlot, u32 door) {
    s32 mode;

    kind &= 0xFF;
    if (kind == 5) {
        if (!(VCALL(gDoors, 0x40, u32 (*)(VObject *, u32))(gDoors, door & 0xFF) & 0xFF)) {
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

/* 0x001A4330 */
void Fiona_Cleanup(Fiona *f) {
}

/* vtable +0x24: remember the previous position, and her heading. */
/* 0x001A2CC0 */
void Fiona_RememberPos(Fiona *f) {
    Character_RememberPos(&f->c);
    FI(f, 0x1AD5B8, f32) = f->c.a.angle[1];
}

/* vtable +0x20: take down her message (unkD0) and stop the animation (unkD1) if requested. */
/* 0x001A3E60 */
void Fiona_Unload(Fiona *f) {
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
/* 0x001A4080 */
s32 Fiona_LoadMessage(Fiona *f) {
    if (VCALL(f->c.motion, 0xAC, s32 (*)(void *))(f->c.motion) == 0) {
        return 0;
    }
    VCALL(gFileLoader, 0xC, void (*)(VObject *, s32, void *, u32, s32))(
        gFileLoader, VCALL(f->c.motion, 0xAC, s32 (*)(void *))(f->c.motion), f->msgImage,
        f->c.a.flags24 | f->c.a.slot, 0);
    return 1;
}

/* vtable +0xC: reset; her collision cylinder (radius 2, height 15) and blocking mask. */
/* 0x001A4340 */
void Fiona_Reset(Fiona *f) {
    Character_Reset(&f->c);
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

/* vtable +0x48: follow the animation (cutscene): position from the root bone, room from
 * progress, triangle from the mesh. */
/* 0x001A3000 */
void Fiona_FollowAnim(Fiona *f) {
    sceVu0FMATRIX m;

    sceVu0UnitMatrix(m);
    VCALL(f->c.motion, 0x28, void (*)(void *, sceVu0FMATRIX))(f->c.motion, m);
    if (f->c.state[0] != 0) {
        f->c.state[0] = 0;
    }
    if (f->c.a.disabled) {
        return;
    }
    Motion_Update(f->c.motion);
    if (VCALL(gCutscene, 0x54, s32 (*)(VObject *, s32, s32))(gCutscene, 0, 0) > 0) {
        MOTION_U8(f->c.motion, 0x850) = 1;
    }
    VCALL(f->c.motion, 0x3C, void (*)(void *))(f->c.motion);
    f->c.a.room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    sceVu0CopyVector(f->c.a.pos, Skel_Bone(MOTION_SKELETON(f->c.motion), 0) + 12);
    f->c.a.navTri = VCALL(gNavMesh, 0x3C, u32 (*)(NavMesh *, f32 *, s32))(gNavMesh, f->c.a.pos, 0);
}

extern void Fiona_MoveInput(Fiona *f);

/* vtable +0x90: full stop - movement, interaction and the related progress flags. */
/* 0x0019D190 */
void Fiona_FullStop(Fiona *f) {
    Progress *p;

    Character_EventReset(&f->c);
    FI(f, 0x1AD5FC, u8) = 0;
    Fiona_MoveInput(f);
    f->unk1AD5D0 = 1;
    f->unk1AD5D1 = 1;
    FI(f, 0x1AD5F0, s32) = 0;
    FI(f, 0x1AD5C8, s32) = 0;
    p = gProgress;
    FI(f, 0x1AD5C4, s32) = 0;
    Progress_ClearFlag(p, 9);
    Progress_ClearFlag(p, 0xA);
    if (*((u8 *)p + 0x1FBEC1) == 0) {
        Progress_CameraOn(p, 0);
    }
    FI(f, 0x1AD5A0, s32) = 0;
    FI(f, 0x1AD5A4, s32) = 0;
    FI(f, 0x1AD5A8, s32) = 0;
    FI(f, 0x1AD5AC, s32) = 0;
    VCALL(gCamDirector, 0x30, void (*)(VObject *, s32))(gCamDirector, 0);
    Progress_ClearFlag(p, 0x17);
}

#define AREA_SPECIAL 0x89

static inline void Fiona_Fade(Fiona *f) {
    VCALL(gRenderer, 0x70, void (*)(VObject *, u32))(gRenderer, 0x808080 | ((u32)FI(f, 0x1AD62C, u16) << 24));
}

/* vtable +0x2C: choose the screen mode (unk152C) for this frame and pass it to the animation. */
/* 0x001A38E0 */
void Fiona_LightChange(Fiona *f) {
    Progress *p = gProgress;
    s32 s;

    if (*((u8 *)p + 0x1FBEC1) != 0) {
        if (f->c.unkE4 == 1) {
            VCALL(f, 0x80, void (*)(Fiona *))(f);
        }
    } else {
        s = f->c.unk152C;
        if (s != 0x17 && s != 0x1E && gCamDirector != NULL
            && !(VCALL(gCamDirector, 0x38, u32 (*)(VObject *))(gCamDirector) & 0xFF)) {
            if (f->c.unkE4 == 1) {
                f->c.unk152C = 0xA;
            } else if (VCALL(gSubScreen, 0x10, s32 (*)(VObject *))(gSubScreen) == AREA_SPECIAL) {
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

/* Resource table offset (from +0x1540) to pointer, 0 = none. */
#define FIONA_RES(f, off) (FI(f, off, s32) != 0 ? (void *)((u8 *)(f) + FI(f, off, s32) + 0x1540) : NULL)
#define MOTION_PTR(m, off) (*(void **)((u8 *)(m) + (off)))

/* vtable +0x1C: hook her data up to the animation player and the message display. */
/* 0x001A3EE0 */
void Fiona_FilesLoaded(Fiona *f) {
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
    VCALL(gDoors, 0x4C, void (*)(VObject *, s32))(gDoors, 0);
    MOTION_PTR(f->c.motion, 0x4D4) = (u8 *)f + 0x1AA540;
    Fiona_ShowEquipment(f);
    if (VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 1) == AREA_SPECIAL) {
        f->c.unkE4 = 0;
        FI(f, 0x1AD62C, u16) = 0;
        f->unk1AD630 = 1;
        f->unk1AD62E = 0;
    }
}

static const char sFionaMotion[] = "O_FIN\\FIN_D000.MTN";

/* LoadAsync(name, dest) for her files, tagged with her file id. */
#define Fiona_Load(f, loader, name, dest) \
    VCALL(loader, 0xC, void (*)(VObject *, const void *, void *, u32, s32))( \
        loader, name, dest, (f)->c.a.flags24 | (f)->c.a.slot, 0)

/* vtable +0x14: start loading her files: model (by costume, which comes from the unlocked
 * costume bits in the progress flags), message data, motions, animation set. */
/* 0x001A4110 */
void Fiona_LoadFiles(Fiona *f) {
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
    FI(f, 0x1AD544, void *) = Progress_CharLoadBuffer(p, 0);
    Fiona_Load(f, loader, VCALL(f->c.motion, 0xA8, void *(*)(void *))(f->c.motion), FI(f, 0x1AD544, void *));
    Fiona_Load(f, loader, sFionaMotion, VCALL(gDoors, 0x48, void *(*)(VObject *, s32))(gDoors, 0));
    Fiona_Load(f, loader, VCALL(f->c.motion, 0xA4, void *(*)(void *, u32))(f->c.motion, costume), (u8 *)f + 0x1AA540);
}

#define Character_ToIdle(c) VCALL(c, 0x7C, void (*)(Character *))(c)

/* vtable +0x8C: interrupted (e.g. a cutscene starts): stop, and release Hewie and the pursuer
 * from joint actions with her. */
/* 0x0019D2B0 */
void Fiona_Interrupted(Fiona *f) {
    Progress *p;

    Character_BackToNormal(&f->c);
    Fiona_StartInDoor(f);
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

/* Animation blend weight (motion +0x6A4 -> +0x1C), mirrored at +0x1AD628. */
static inline void Fiona_SetPose(Fiona *f, s32 set, s32 variant, f32 w) {
    Motion_Play(f->c.motion, set, variant);
    *(f32 *)((u8 *)MOTION_PTR(f->c.motion, 0x6A4) + 0x1C) = w;
    FI(f, 0x1AD628, f32) = w;
}

/* vtable +0x28: place her (Character), reset interaction state, and on the first placement
 * pick her starting pose from her condition (+0x1AD5F4 of 100, +0x1AD5F8 of 1800 frames). */
/* 0x001A3A80 */
s32 Fiona_PlaceOn(Fiona *f, u32 tri, const f32 *heading, f32 *pos) {
    s32 r = Character_Place(&f->c, tri, heading, pos);

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
    Lists_Clear((u8 *)f + 0x1AD668);
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

extern void Fiona_AreaFade(Fiona *f);
extern u8 D_0047E3B0[];   /* pad state */
extern s32 Fiona_JointAction(Fiona *f);
extern void Fiona_HeadLook(Fiona *f);
extern void Fiona_MotionSounds(Fiona *f);
extern void Fiona_Footsteps(Fiona *f);

#define FIONA_NAV_MASK 0x28020018

/* vtable +0x44: per-frame update - controls, behaviour state, sub-systems; tells progress when
 * the pursuer is close (within 200 / 150 units). */
/* 0x001A2D00 */
void Fiona_Think(Fiona *f) {
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
    Fiona_AreaFade(f);
    p = gProgress;
    if ((Progress_TestFlag(p, 0xD) & 0xFF) == 1 && !(Progress_TestFlag(p, 0x2B) & 0xFF)) {
        FI(f, 0x1AD6B8, s32) = Gesture_Update((u8 *)f + 0x1AD668, (f32 *)D_0047E3B0);
    }
    FI(f, 0x1AD6BC, s32) = -1;
    VCALL(f, 0x88, void (*)(Fiona *))(f);
    ptmf_scall(f, &f->c.a.state);
    Fiona_JointAction(f);
    Fiona_HeadLook(f);
    Fiona_MotionSounds(f);
    Fiona_Footsteps(f);
    VCALL(f, 0x40, void (*)(Fiona *))(f);
    near = 0;
    veryNear = 0;
    if (FI(f, 0x1AD5D7, u8) == 1 && Actor_Distance(&f->c.a, gCharPursuer->a.pos) <= 200.0f) {
        near = 1;
        if (Actor_Distance(&f->c.a, gCharPursuer->a.pos) <= 150.0f) {
            veryNear = 1;
        }
    }
    if (near == 1) {
        Progress_SetCondBit(p, 5);
        if (veryNear == 1) {
            Progress_SetCondBit(p, 0);
        }
    }
}

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
/* 0x0019BE70 */
void Fiona_AnimUpdate(Fiona *f) {
    s32 room;

    if (!f->c.a.disabled) {
        if (f->c.a.navTri == NAV_NONE) {
            Model_BodyFrames(f->c.motion, f, 0.0f, 0.0f);
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
    Motion_Hands(f->c.motion);
    Motion_Eyes(f->c.motion);
    Motion_Update(f->c.motion);
    room = f->c.a.room;
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && f->c.a.navTri != NAV_NONE) {
        VCALL(f->c.motion, 0x4C, void (*)(void *, u32, Fiona *))(f->c.motion, FI(f, 0x1AD5BC, u8), f);
    }
    VCALL(f->c.motion, 0x3C, void (*)(void *))(f->c.motion);
}

/* Lower the +0x1AD5F8 counter (frames, of 1800) by 60 / n, not below 0. */
/* 0x0019A210 */
void Fiona_LowerRecovery(Fiona *f, s32 n) {
    FI(f, 0x1AD5F8, s32) -= 60 / (s16)n;
    if (FI(f, 0x1AD5F8, s32) < 0) {
        FI(f, 0x1AD5F8, s32) = 0;
    }
}

extern void Fiona_ChangeFear(Fiona *f, f32 d);   /* change the fear by d */

/* Calm down by n/30 (the fear, Fiona_ChangeFear). */
/* 0x0019A280 */
void Fiona_CalmDown(Fiona *f, s32 n) {
    Fiona_ChangeFear(f, -(0x1.11105ep-5f /* 0x3D08882F, ~1/30 */ * (f32)n));
}

/* Is she idle (action 0, state not 1/0xE/0xF)? */
/* 0x0019A2B0 */
s32 Fiona_IsIdle(Fiona *f) {
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

extern const PTMF D_003B27E8;

/* Start action 4 / sub 0xA with parameter `arg` (`flag` 1: also Fiona_ResetRecovery). */
/* 0x0019A0D0 */
void Fiona_StartAction4(Fiona *f, u32 arg, u32 flag) {
    Fiona_StartInDoor(f);
    if ((u32)((Progress_GetVar(gProgress, 0x26) & 0xFF) - 6) < 2) {
        VCALL(f->c.motion, 0x2C, void (*)(void *))(f->c.motion);
    }
    f->c.moveMode = 4;
    f->c.moveSub = 0xA;
    VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0xD0, 0xC);
    f->c.unk100 = arg & 0xFF;
    if ((flag & 0xFF) == 1) {
        Fiona_ResetRecovery(f);
    }
    f->c.a.unk2A = 1;
    Actor_SetState(&f->c.a, &D_003B27E8);
    if (f->c.unkE0 == 1) {
        VCALL(f, 0x90, void (*)(Fiona *))(f);
    }
}

extern const PTMF D_003B27F8;

/* State: turn on the spot toward the stick direction (+0x1AD570) by 10 degrees a frame;
 * back to idle if it no longer matches +0x1AD550 (dot <= 0.6). */
/* 0x00199ED0 */
void Fiona_StateTurnStick(Fiona *f) {
    Character_RootMoveMasked(&f->c);
    if (sceVu0InnerProduct((f32 *)((u8 *)f + 0x1AD570), (f32 *)((u8 *)f + 0x1AD550)) <= 0x1.333334p-1f /* 0.6 */) {
        Fiona_ToIdle(f);
        return;
    }
    if (Actor_TurnToward(&f->c.a, func_0031C5C0(FI(f, 0x1AD570, f32), FI(f, 0x1AD578, f32)),
                      0x1.657186p-3f /* 10 deg */) == 0.0f) {
        Motion_PlayOwnBlend(f->c.motion, 0x1200, -1);
        Actor_SetState(&f->c.a, &D_003B27F8);
    }
}

extern s32 Fiona_FindPushable(Fiona *f, s32, f32 reach);
extern const PTMF D_003B2808;  /* push: let go */
extern const PTMF D_003B2818;  /* push: moving */
extern const PTMF D_003B2828;  /* push: stop straining */

#define FIONA_STICK(f) ((f32 *)((u8 *)(f) + 0x1AD570))
#define FIONA_PUSH_OBJ(f) FI(f, 0x1AD560, s32)

/* State: pushing an object (animations 0x1200..0x1203). */
/* 0x001998F0 */
void Fiona_StatePush(Fiona *f) {
    s32 blocked;
    void *m;

    blocked = 0;
    if (!(FI(f, 0x1AD584, s32) & 0x2)) {
        if (FI(f, 0x1AD5D7, u8) == 1 && (Actor_Touching(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) == 1) {
            blocked = 1;
        } else {
            Actor_Touching(&f->c.a, &gCharPartner->a, 0.0f, 0.0f);   /* (result unused) */
        }
    } else {
        blocked = 1;
    }
    if (blocked) {
        Fiona_ToIdle(f);
        return;
    }
    Character_RootMoveMasked(&f->c);
    if (sceVu0InnerProduct(FIONA_STICK(f), (f32 *)((u8 *)f + 0x1AD550)) <= 0x1.333334p-1f /* 0.6 */) {
        /* stick released: let go once the animation has stopped */
        m = f->c.motion;
        if (!(*(f32 *)((u8 *)m + 0x550) <= 0.0f)) {
            return;
        }
        if (*(s32 *)((u8 *)m + 0x55C) == 0x1203) {
            Motion_Play(m, 0x1202, -1);
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
    if (FIONA_PUSH_OBJ(f) == -1 && Fiona_FindPushable(f, 1, 0x1.19999ap+1f /* 2.2 */) == -1) {
        Motion_Play(f->c.motion, 0x1202, -1);
        Actor_SetState(&f->c.a, &D_003B2808);
        return;
    }
    if (FIONA_PUSH_OBJ(f) == -1
        || VCALL(gObstacles, 0x30, s32 (*)(VObject *, s32, f32 *))(gObstacles, FIONA_PUSH_OBJ(f), FIONA_STICK(f)) != 0) {
        /* it won't move: strain */
        FI(f, 0x1AD5D2, u8) = 1;
        Motion_Play(f->c.motion, 0x1203, -1);
        return;
    }
    {
        VObject *objs = gObstacles;

        VCALL(objs, 0x3C, void (*)(VObject *, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), FIONA_STICK(f));
        VCALL(objs, 0x1C, void (*)(VObject *, s32, s32))(objs, FIONA_PUSH_OBJ(f), 0);
        VCALL(objs, 0x20, void (*)(VObject *, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), FIONA_STICK(f));
        Motion_Play(f->c.motion, 0x1201, -1);
        VCALL(objs, 0x28, void (*)(VObject *, s32, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), 0, FIONA_STICK(f));
        Actor_PlaySound(&f->c.a, 0x45, 5, 0, 0, NULL);
        Actor_SetState(&f->c.a, &D_003B2818);
    }
}

extern const PTMF D_003B2838;  /* push: let go */

#define NAV_PUSHABLE 0x800000   /* triangle flag: an object may be pushed onto it */

static inline s32 Fiona_PushBlocked(Fiona *f) {
    if (FI(f, 0x1AD584, s32) & 0x2) {
        return 1;
    }
    if (FI(f, 0x1AD5D7, u8) == 1 && (Actor_Touching(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) == 1) {
        return 1;
    }
    Actor_Touching(&f->c.a, &gCharPartner->a, 0.0f, 0.0f);   /* (result unused) */
    return 0;
}

/* State: pushing an object, moving it step by step while the stick holds the direction. */
/* 0x001991E0 */
void Fiona_StatePushStep(Fiona *f) {
    void *m;
    s32 anim;

    if (Fiona_PushBlocked(f)) {
        Fiona_ToIdle(f);
        return;
    }
    Character_RootMoveMasked(&f->c);
    m = f->c.motion;
    anim = *(s32 *)((u8 *)m + 0x55C);
    if ((*(s32 *)((u8 *)MOTION_PTR(m, 0x6A4) + 0x18) & 0x20) != 0) {
        /* step event */
        if (anim == 0x1201) {
            VCALL(gObstacles, 0x2C, void (*)(VObject *, s32))(gObstacles, FIONA_PUSH_OBJ(f));
        }
        if (!(sceVu0InnerProduct(FIONA_STICK(f), (f32 *)((u8 *)f + 0x1AD550)) <= 0x1.333334p-1f /* 0.6 */)) {
            VObject *objs = gObstacles;
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
                tri = Actor_TriTo(&f->c.a, target, 0);
                if (tri != NAV_NONE && (NavMesh_Tri(gNavMesh, tri)->flags & NAV_PUSHABLE)) {
                    ok = 0;
                }
            }
            if (ok == 0
                && VCALL(objs, 0x30, s32 (*)(VObject *, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), FIONA_STICK(f)) == 0) {
                objs = gObstacles;
                VCALL(objs, 0x3C, void (*)(VObject *, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), FIONA_STICK(f));
                VCALL(objs, 0x1C, void (*)(VObject *, s32, s32))(objs, FIONA_PUSH_OBJ(f), 0);
                VCALL(objs, 0x20, void (*)(VObject *, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), FIONA_STICK(f));
                Motion_Play(f->c.motion, 0x1201, -1);
                VCALL(objs, 0x28, void (*)(VObject *, s32, s32, f32 *))(objs, FIONA_PUSH_OBJ(f), 0, FIONA_STICK(f));
                return;
            }
            FI(f, 0x1AD5D2, u8) = 1;
            Motion_Play(f->c.motion, 0x1203, -1);
            return;
        }
        Motion_Play(f->c.motion, 0x1202, -1);
        Actor_SetState(&f->c.a, &D_003B2838);
    } else if (anim == 0x1203) {
        FI(f, 0x1AD5D2, u8) = 1;
    }
    if ((Motion_EventFlags(f->c.motion, 0, 0, 1) & 0xFF) & 0x2) {
        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0x40, 0x10);
    }
}

extern s32 Fiona_HeadFor(Fiona *f, u32 tri, f32 *pos, s32);
extern const PTMF D_003B27D8;

#define Fiona_Place(f, tri, pos) VCALL(f, 0x28, s32 (*)(Fiona *, u32, const f32 *, f32 *))(f, tri, NULL, pos)
#define Room_ExitPosIn(rooms, door, out) VCALL(rooms, 0x34, u32 (*)(VObject *, u32, f32 *))(rooms, door, out)
#define Room_ExitPosOut(rooms, door, out) VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, door, out)
#define Room_DoorTo(rooms, route, room) VCALL(rooms, 0x3C, u32 (*)(VObject *, u32, s32))(rooms, route, room)
#define FIONA_ROUTE0(f) (*(u16 *)(f)->c.unk138C)

/* vtable +0x38: room (re-)entry - place her in the current room, or keep her out of it. */
/* 0x0019AF20 */
void Fiona_Vt38(Fiona *f) {
    Progress *p = gProgress;
    VObject *rooms;
    s32 room;
    u8 ok;

    if (*((u8 *)p + 0x1FBEC1) == 0) {
        FI(f, 0x1AD5FC, u8) = 0;
        VCALL(gEvents, 0x2C, void (*)(VObject *, Fiona *))(gEvents, f);
        return;
    }
    if ((u32)((Progress_GetVar(p, 0x26) & 0xFF) - 6) < 2) {
        VCALL(f->c.motion, 0x2C, void (*)(void *))(f->c.motion);
    }
    if (FI(f, 0x1AD718, u8) == 1) {
        /* coming in through a door */
        f->c.a.room = VCALL(p, 0xC, s32 (*)(Progress *))(p);
        rooms = gRooms;
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
        FI(f, 0x1AD738, s32) = (s32)(30.0f * (2.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom))) + 90;
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
        f->c.a.navTri = Room_ExitPosIn(gRooms, f->c.door, f->c.a.pos);
        if (Fiona_Place(f, f->c.a.navTri, f->c.a.pos) == 0) {
            ok = 1;
        }
    }
    if (!ok) {
        Actor_TeleportRandom(&f->c.a, VCALL(gRooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(gRooms, f->c.a.room, f->c.door, 0));
    }
    VCALL(gEvents, 0x2C, void (*)(VObject *, Fiona *))(gEvents, f);
    if (f->c.a.navTri == NAV_NONE) {
        f->c.a.pos[0] = 0.0f;
        f->c.a.pos[1] = 0.0f;
        f->c.a.pos[2] = 0.0f;
        f->c.a.pos[3] = 0x1.99999ap-4f;   /* 0.1 */
    }
    {
        sceVu0FVECTOR v;

        if (Actor_PosInCurrentRoom(&f->c.a, v)) {
            Character_Sound(&f->c, 3, (s32)v, 0, 0, 0);
        }
    }
    Character_MarkObjects(&f->c);
    if (*((u8 *)p + 0x7B8) == 5) {
        FI(f, 0x1AD71C, s32) = 0;
        f->c.moveMode = 0xA;
        f->unk1AD580 = 0xB;
        Motion_Play(f->c.motion, 0xB01, -1);
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

        rooms = gRooms;
        exit = Room_DoorTo(rooms, FIONA_ROUTE0(f), f->c.a.room) & 0xFF;
        if (exit == 0xFF) {
            return;
        }
        if (f->c.a.navTri == NAV_NONE) {
            f->c.a.navTri = Room_ExitPosIn(rooms, f->c.door, f->c.a.pos);
        }
        if (Fiona_HeadFor(f, Room_ExitPosIn(rooms, exit, exitPos), exitPos, 0) != 0) {
            return;
        }
        tri = f->c.a.navTri;
        step = FI(f, 0x1AD73C, f32) - 5.0f;
        if (step < 0.0f) {
            step = 0.0f;
        }
        Character_WaypointAhead(&f->c, &tri, back, step);
        Character_WaypointAhead(&f->c, &f->c.a.navTri, f->c.a.pos, FI(f, 0x1AD73C, f32));
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

#define FIONA_FADE(f) FI(f, 0x1AD62C, u16)     /* 0..0x80 */
#define FIONA_FADE_T(f) FI(f, 0x1AD62E, s16)

/* Fade handling for area 0x89, and the pursuer grabbing her once the fade has cleared. */
/* 0x001A1CA0 */
void Fiona_AreaFade(Fiona *f) {
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
    } else if (VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 1) != AREA_SPECIAL
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
        && (Actor_Touching(&f->c.a, &gCharPursuer->a, 1.0f, 0.0f) & 0xFF) == 1
        && !(Character_Held(&f->c) & 0xFF)) {
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

#define RNG01() VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)
#define HEWIE_ACTION(c) (*(s32 *)((u8 *)(c) + 0xF3564))

/* Timers: an alternating period (+0x1AD719 flips when +0x1AD724 runs out; random lengths),
 * sped up / slowed down while Hewie stays close, and three 30-frame counters. */
/* 0x001A1860 */
void Fiona_Timers(Fiona *f) {
    Progress *p = gProgress;
    s32 room = f->c.a.room;

    if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        if (Actor_Distance(&f->c.a, gCharPartner->a.pos) < 50.0f) {
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
/* 0x001A12B0 */
s32 Fiona_JointAction(Fiona *f) {
    Progress *p = gProgress;
    Progress *q;
    Character *o;
    u32 kind, type;

    if ((Progress_HasRelationCmd(p, SLOT_U8(f)) & 0xFF) != 1) {
        return -1;
    }
    q = gProgress;
    o = gCharacters[SlotCmd_Target(q, SLOT_U8(f)) & 0xFF];
    kind = SlotCmd_Kind(q, SLOT_U8(f)) & 0xFF;
    type = SlotCmd_Arg(q, SLOT_U8(f)) & 0xFF;
    if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && !(Progress_TestFlag(p, 8) & 0xFF)
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
                tri = Actor_TriFrom(&f->c.a, target, o->a.navTri, o->a.pos, NAV_NONE);
                if (tri != NAV_NONE && f->c.a.navTri == Actor_TriFrom(&f->c.a, f->c.a.pos, tri, target, NAV_NONE)
                    && (Actor_TriFreeFor(f, &f->c.a) & 0xFF) == 1 && Character_PlanPathKind(&f->c, 0, tri, target) > 0) {
                    FI(f, 0x1AD6F0, u32) = tri;
                    FI(f, 0x1AD6F4, f32) = Angle_Wrap(turn + o->a.angle[1]);
                    sceVu0CopyVector((f32 *)((u8 *)f + 0x1AD700), target);
                    SlotCmd_Start(p, SLOT_U8(f));
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
                        ang = Angle_Wrap(base + a / 180.0f);
                    } else {
                        ang = Angle_Wrap(base - a / 180.0f);
                    }
                    Mtx_AtHeading(m, f->c.a.pos, ang);
                    Mtx_ApplyPoint(pt, m, offs);
                    tri = Actor_TriOf(&f->c.a, pt);
                    if (tri != NAV_NONE && tri == Actor_TriFrom(&f->c.a, pt, h->a.navTri, h->a.pos, FIONA_NAV_MASK)) {
                        *(f32 *)&h->unk104[2] = ang;
                        h->unk104[0] = tri;
                        *(f32 *)&f->c.unk104[2] = Angle_Wrap(F_PI + ang);
                        SlotCmd_Start(p, SLOT_U8(f));
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
    SlotCmd_Cancel(p, SLOT_U8(f));
    return -1;
}

#define MOTION_SPEED(m) (*(f32 *)((u8 *)(m) + 0x550))
#define MOTION_ANIM(m) (*(s32 *)((u8 *)(m) + 0x55C))
#define MOTION_EVENTS(m) (*(s32 *)((u8 *)MOTION_PTR(m, 0x6A4) + 0x18))

/* State: the special room entry (animations 0xB01 -> 0xB02), moving by root motion. */
/* 0x0019C210 */
void Fiona_StateSpecialEntry(Fiona *f) {
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
                Motion_PlayTable(f->c.motion, 0xB02, -1);
            }
        } else if (MOTION_ANIM(m) == 0xB02 && (MOTION_EVENTS(m) & 0x20) != 0) {
            Fiona_ToIdle(f);
        }
    }
    Motion_RootMovement(f->c.motion, root, 0.0f);
    if (!(root[2] <= 0.0f)) {
        f32 yaw0 = f->c.a.angle[1];

        Actor_TurnToward(&f->c.a, f->savedYaw, (F_PI * (10.0f * root[2])) / 180.0f);
        *(s32 *)&target[0] = 0;
        *(s32 *)&target[1] = 0;
        target[2] = 4.0f;
        sceVu0ApplyMatrix(probe, f->c.a.rot, target);
        sceVu0AddVector(target, f->c.a.pos, probe);
        if (Actor_TriTo(&f->c.a, target, NAV_NONE) == NAV_NONE) {
            f->c.a.angle[1] = yaw0;
            sceVu0UnitMatrix(f->c.a.rot);
            sceVu0RotMatrixY(f->c.a.rot, f->c.a.rot, yaw0);
        }
    }
    Character_RootTurn(&f->c);
    f->savedYaw = f->c.a.angle[1];
    sceVu0ApplyMatrix(root, f->c.a.rot, root);
    f->c.a.navMask |= 1;
    Actor_Move(&f->c.a, root);
    f->c.a.navMask &= ~1;
    sceVu0AddVector(target, f->c.a.pos, probe);
    if (Actor_TriTo(&f->c.a, target, NAV_NONE) == NAV_NONE) {
        f->c.a.navTri = f->c.a.prevNavTri;
        sceVu0CopyVector(f->c.a.pos, f->c.a.prevPos);
    }
    if (FI(f, 0x1AD6C0, s32) < 0x97 && Fiona_Shakes(f) != 0) {
        *(s16 *)((u8 *)gProgress + 0x7BA) -= 2;
        FI(f, 0x1AD6C0, s32) += 2;
    }
}

extern s32 Fiona_HewieCommandAction(Fiona *f, s32 cmd, s32 state);
extern void Fiona_CommandHewie(Fiona *f);
extern void Fiona_OrderLine(Fiona *f);
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
 * actions (codes from Fiona_HewieCommandAction; 44/45 have their own setup, others are joint actions). */
/* 0x0019F1E0 */
void Fiona_ControlCommand(Fiona *f) {
    s32 cmd, mode, s, code;
    u8 joint;

    if (f->c.moveMode == 0) {
        if (FIONA_PANIC(f)) {
            sceVu0FMATRIX m;
            sceVu0FVECTOR r;

            if (FI(f, 0x1AD5EC, s32) != 0) {
                FI(f, 0x1AD5EC, s32) -= 1;
            }
            Motion_RootMovement(f->c.motion, r, 0.0f);
            Mtx_AtHeading(m, f->c.a.pos,
                          FI(f, 0x1AD58C, s32) != 0 ? f->savedYaw
                                                    : func_0031C5C0(FI(f, 0x1AD550, f32), FI(f, 0x1AD558, f32)));
            Mtx_ApplyPoint(r, m, r);
            if (Actor_TriTo(&f->c.a, r, NAV_NONE) == NAV_NONE) {
                /* ran into something while panicking */
                Actor_PlaySound(&f->c.a, 0x7F, 5, 0, 0, NULL);
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
                Motion_PlayTable(f->c.motion, 0x1001, -1);
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
        Actor_PlaySound(&f->c.a, 0x38, 5, 0, 0, NULL);
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
    code = Fiona_HewieCommandAction(f, cmd, s);
    if (code == -1) {
        return;
    }
    joint = 1;
    if ((code == 44 || code == 45) && f->c.moveSub != 0
        && !(code == 45 && (Progress_GameMode(gProgress) & 0xFF) == 1)) {
        Fiona_MarkActionStart(f, code);
        joint = 0;
        Fiona_CommandHewie(f);
        Fiona_OrderLine(f);
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

extern void Fiona_Timers(Fiona *f);
extern void Fiona_MoodAI(Fiona *f);
extern void Fiona_OffscreenFrame(Fiona *f);
extern void Fiona_Panic(Fiona *f);
extern void Fiona_KeepApart(Fiona *f);
extern void Fiona_Head(Fiona *f);

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
    if (FI(f, 0x1AD5D7, u8) == 1 && (Actor_Touching(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) == 1) {
        return 2;
    }
    return (Actor_Touching(&f->c.a, &gCharPartner->a, 0.0f, 0.0f) & 0xFF) == 1 ? 1 : 0;
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
        return f->unk1AD580 == 5 && ((Motion_EventFlags(f->c.motion, 0, 0, 1) & 0xFF) & 0x20);
    }
    return 0;
}

/* vtable +0x30: gameplay update - room, controls, actions, behaviour state, sub-systems. */
/* 0x001A3110 */
void Fiona_Update(Fiona *f) {
    Progress *p = gProgress;
    u8 *special = (u8 *)p + 0x1FBEC1;
    sceVu0FVECTOR head;
    s32 room;
    u8 hewie, near, veryNear;
    f32 h;

    if (*special == 1) {
        Fiona_Timers(f);
    }
    room = f->c.a.room;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        /* not in the room being played */
        f->c.a.disabled = 1;
        f->c.moveSub = 0;
        Fiona_UpdatePresence(f);
        f->c.state[0] = 0;
        Fiona_MoodAI(f);
        Fiona_OffscreenFrame(f);
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
    Fiona_Panic(f);
    if (*special == 1) {
        Fiona_MoodAI(f);
        if (f->c.a.disabled == 1) {
            f->c.state[0] = 0;
            return;
        }
        if (FI(f, 0x1AD5D7, u8) == 1 && gCharPursuer->a.unkC4 == 2) {
            f->c.a.unk2A = 1;
        }
    }
    Fiona_MoveInput(f);
    FI(f, 0x1AD6BC, s32) = -1;
    if (f->c.state[0] != 0) {
        VCALL(f, 0x84, void (*)(Fiona *))(f);
    } else {
        Fiona_ControlCommand(f);
    }
    FIONA_CMD(f) = Gesture_Update((u8 *)f + 0x1AD668, (f32 *)(Fiona_ReadsPad(f, p) ? D_0047E3B0 : NULL));

    hewie = Fiona_Touching(f) == 1;
    ptmf_scall(f, &f->c.a.state);
    if (hewie != 1 && f->c.a.unk2A != 1) {
        Fiona_KeepApart(f);
    } else if (f->c.moveMode == 0 && Fiona_Touching(f) == 0) {
        f->c.a.unk2A = 0;
    }
    Fiona_JointAction(f);
    Fiona_Head(f);
    Fiona_MotionSounds(f);
    Fiona_Footsteps(f);
    VCALL(f, 0x40, void (*)(Fiona *))(f);
    if (f->c.moveMode == 0 && f->unk1AD580 != 1) {
        s32 sub = f->c.moveSub;

        if (sub != 2 && sub != 1 && sub != 0) {
            f->c.moveSub = 0;
        }
    }
    near = 0;
    veryNear = 0;
    if (FI(f, 0x1AD5D7, u8) == 1 && Actor_Distance(&f->c.a, gCharPursuer->a.pos) <= 200.0f) {
        near = 1;
        if (Actor_Distance(&f->c.a, gCharPursuer->a.pos) <= 150.0f) {
            veryNear = 1;
        }
    }
    if (near == 1) {
        Progress_SetCondBit(p, 5);
        if (veryNear == 1) {
            Progress_SetCondBit(p, 0);
        }
    }
    if (f->c.state[0] == 7) {
        f->c.state[0] = 0;
    }
}

/* vtable +0x34: going through door `door`. In play, turn to face through it (if the stick
 * points that way, relative to the camera); in the special mode, plan the walk into the next
 * room (with Hewie if he is closer to the door). */
/* 0x0019B4F0 */
void Fiona_Vt34(Fiona *f, s32 door) {
    Progress *p = gProgress;
    VObject *rooms;

    if (*((u8 *)p + 0x1FBEC1) == 0) {
        f32 ax, az;

        if (f->c.unkE0 == 1) {
            VCALL(f, 0x90, void (*)(Fiona *))(f);
            Fiona_ToIdle(f);
        }
        f->c.door = VCALL(gRooms, 0x14, u32 (*)(VObject *, s32, s32))(gRooms, f->c.a.room, door);
        ROOMLOG("fiona leaves room %d by exit %d (door %d) -> room %d exit %d, at (%.1f %.1f %.1f)", f->c.a.room,
                door, VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, s32))(gRooms, f->c.a.room, door) & 0xFFFF,
                VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, s32))(gRooms, f->c.a.room, door), f->c.door,
                f->c.a.pos[0], f->c.a.pos[1], f->c.a.pos[2]);
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

            Mtx_TurnY(m, VCALL(gCamera, 0x68, f32 (*)(VObject *))(gCamera));
            Mtx_ApplyVector(v, m, (f32 *)((u8 *)f + 0x1AD5A0));
            func_0010E640(v, v, -1.0f);
            h = func_0031C5C0(v[0], v[2]);
            if (Angle_Wrap(h - f->savedYaw) < 0x1.921fb6p+0f /* pi/2 */) {
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
        Fiona_StartInDoor(f);
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
            rooms = gRooms;
            target = VCALL(rooms, 0x18, s32 (*)(VObject *, s32, s32))(rooms, f->c.a.room, door);
            if (Fiona_HeadFor(f, Room_ExitPosIn(rooms, door, pos), pos, 1) == 0) {
                through = 1;
            }
        }
        if (!through && d != 0xFF) {
            s32 side, tgt2;

            rooms = gRooms;
            tgt2 = VCALL(rooms, 0x58, s32 (*)(VObject *, s32, s32, s32))(rooms, f->c.a.room, door, 0);
            side = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, f->c.a.room, f->c.door, 0);
            if (Character_Route(&f->c, target, tgt2, side, -1) != -1
                && (Room_DoorTo(rooms, FIONA_ROUTE0(f), f->c.a.room) & 0xFF) != 0xFF) {
                Fiona_HeadFor(f, Room_ExitPosIn(rooms, door, pos), pos, 1);
            }
        }
        if (through == 1) {
            FIONA_ROUTE0(f) = VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, s32))(gRooms, f->c.a.room, door);
            if ((Actor_NearerRoom(&f->c.a, door, gCharPartner->a.pos) & 0xFF) == 1) {
                FI(f, 0x1AD718, u8) = 1;
                FI(f, 0x1AD6C0, s32) = 0;
            } else {
                sceVu0FVECTOR a, b, da, db;
                f32 la;

                rooms = gRooms;
                Room_ExitPosOut(rooms, door, a);
                Room_ExitPosIn(rooms, door, b);
                sceVu0SubVector(da, f->c.a.pos, a);
                sceVu0SubVector(db, b, a);
                la = sceVu0InnerProduct(da, da);
                if (la <= sceVu0InnerProduct(db, db) && Actor_TriTo(&f->c.a, a, NAV_NONE) != NAV_NONE) {
                    FI(f, 0x1AD718, u8) = 1;
                    FI(f, 0x1AD6C0, s32) = 1;
                }
            }
        }
    }
}

extern s32 Fiona_React(Fiona *f, s32 *state);
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
/* 0x0019F8A0 */
void Fiona_Requests(Fiona *f) {
    switch (f->c.state[0]) {
    case 0:
        break;
    case 4: {
        u32 tri = f->c.a.navTri;

        if (tri != NAV_NONE
            && (f->c.state[1] == 5 || !(NavMesh_Tri(gNavMesh, tri)->flags & FIONA_NAV_MASK))
            && Fiona_React(f, f->c.state) == 0) {
            VCALL(f, 0x90, void (*)(Fiona *))(f);
            Character_ChooseExit(&f->c, 0xFF);
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
        Character_ChooseExit(&f->c, 0xFF);
        Fiona_ToIdle(f);
        f->c.unkF4 = 0;
        return;
    case 2:
        Actor_SetState(&f->c.a, &D_003B26B8);
        break;
    case 3:
    case 4:
        f->c.unk104[0] = VCALL(gDoors, 0x18, s32 (*)(VObject *, u32, f32 *))(gDoors, *(u8 *)&f->c.unk100, f->c.a.pos);
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
        Motion_PlayTable(f->c.motion, f->c.unk104[0], -1);
        f->c.unkE1 = 1;
        Actor_SetState(&f->c.a, &D_003B2718);
        break;
    case 8:
        f->unk1AD580 = 0x11;
        Motion_PlayBlend(f->c.motion, f->c.unk104[0], f->c.unk104[1], -1);
        f->c.unkE1 = 1;
        Actor_SetState(&f->c.a, &D_003B2728);
        break;
    case 9:
        f->unk1AD580 = 0x11;
        Motion_PlayBlend8(f->c.motion, f->c.unk104[0], f->c.unk104[1]);
        f->c.unkE1 = 1;
        Actor_SetState(&f->c.a, &D_003B2738);
        break;
    case 16:
        f->unk1AD580 = 0x11;
        Fiona_IdleAnim(f, f->c.unk104[1]);
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
            f->savedYaw = Actor_HeadingTo(&f->c.a, o->a.pos);
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
        s32 n = Character_PlanPathKind(&f->c, 0, f->c.unk100, f->c.unk110);
        s32 frames;
        f32 d, w;

        if (n > 0) {
            n = Character_WaypointsCurve(&f->c);
        }
        if (n <= 0) {
            f->c.unkE1 = 1;
            break;
        }
        Motion_PlayBlend(f->c.motion, f->c.unk104[0], f->c.unk104[1], -1);
        frames = *(s32 *)((u8 *)(*(void **)((u8 *)(*(void **)((u8 *)MOTION_PTR(f->c.motion, 0x6A4) + 0x20)) + 4)) + 0xC);
        d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
            gSceneGameF29740, f->c.a.pos, f->c.unk128, f->c.unk124, f->c.unk12C);
        FI(f, 0x1AD6D0, f32) = 0x1.99999ap-4f /* 0.1 */ + d / (f32)frames;
        if (!(Angle_Wrap(*(f32 *)&f->c.unk104[2] - f->c.a.angle[1]) <= 0.0f)) {
            w = Angle_Wrap(*(f32 *)&f->c.unk104[2] - f->c.a.angle[1]);
        } else {
            w = -Angle_Wrap(*(f32 *)&f->c.unk104[2] - f->c.a.angle[1]);
        }
        FI(f, 0x1AD6D4, f32) = 0x1.c98712p-10f /* 0.1 deg */ + w / (f32)frames;
        f->unk1AD580 = 0x18;
        Actor_SetState(&f->c.a, &D_003B2778);
        break;
    }
    }
    f->c.unkF4 = 0;
}

#define THREAT_LEVEL(p) (*((u8 *)(p) + 0x7B8))     /* 0..4 */
#define FIONA_FEAR(f) FI(f, 0x1AD5F4, f32)        /* 0..100 */

static inline void Fiona_FearDown(Fiona *f, Progress *p) {
    Fiona_ChangeFear(f, (Progress_GameMode(p) & 0xFF) == 2 ? -0x1.99999ap-4f /* -0.1 */ : -0x1.333334p-3f /* -0.15 */);
}

/* Full panic (threat 4): long panic, a scream (noise event 0x6F at her position). */
static inline void Fiona_FullPanic(Fiona *f, Progress *p) {
    FI(f, 0x1AD584, s32) |= 2;
    FI(f, 0x1AD5E8, s32) = (s32)(3.0f * RNG01()) * 30 + 120;
    FI(f, 0x1AD5EC, s32) = 30;
    Noise_Make((u8 *)p + 0x778, 0x6F, f->c.a.room, f->c.a.navTri, 0xFFFF);
}

/* Panic system: threat level -> panic state and duration, the fear meter, timers, and the
 * area fade / pursuer grab (as in Fiona_AreaFade). */
/* 0x001A1FD0 */
void Fiona_Panic(Fiona *f) {
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
                if (gSubScreen == NULL || VCALL(gSubScreen, 0x10, s32 (*)(VObject *))(gSubScreen) != 0x8C) {
                    Fiona_ChangeFear(f, (Progress_GameMode(p) & 0xFF) == 2 ? 0x1.111112p-4f /* 1/15 */
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
            Motion_Unfreeze(f->c.motion);
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
    } else if (VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 1) == AREA_SPECIAL
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
        && (Actor_Touching(&f->c.a, &gCharPursuer->a, 1.0f, 0.0f) & 0xFF) == 1
        && !(Character_Held(&f->c) & 0xFF)) {
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

extern void Fiona_WalkLook(Fiona *f);   /* walk */
extern void Fiona_RunLook(Fiona *f);   /* run */
extern const PTMF D_003B27C8;          /* start pushing */

#define FIONA_STICK_IDLE(f) FI(f, 0x1AD58C, s32)   /* frames since the stick was released, 0 = held */
#define FIONA_STICK_HEADING(f) func_0031C5C0(FI(f, 0x1AD550, f32), FI(f, 0x1AD558, f32))

/* Exhausted from running in panic: catch breath (animation 0x207), new panic run length.
 * (`g` is passed to the random call only because the original leaves it in $a2 there; the
 * callee ignores it - it keeps the differential test's argument check exact.) */
static inline void Fiona_Exhausted(Fiona *f, s32 g) {
    FI(f, 0x1AD5E8, s32) = (s32)(3.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *, s32, s32))(gRandom, 0, g)) * 30 + 120;
    f->unk1AD580 = 0xF;
    Motion_PlayTable(f->c.motion, 0x207, -1);
}

static inline void Fiona_Stand(Fiona *f) {
    FI(f, 0x1AD5C0, u32) = 0;
    Fiona_IdleAnim(f, -1);
}

/* State: idle and moving by the stick - stand, rest, walk, run, panic running, turning,
 * root motion; starts pushing when she stops against an object. */
/* 0x0019C600 */
void Fiona_StateIdleMove(Fiona *f) {
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
                        if ((Progress_GameMode(gProgress) & 0xFF) == 1 && FI(f, 0x1AD584, s32) == 0
                            && FIONA_FEAR(f) < 20.0f && FI(f, 0x1AD5F8, s32) < 360) {
                            if (++FI(f, 0x1AD5C0, u32) >= 90) {
                                Motion_PlayTable(f->c.motion, 1, -1);   /* rest */
                            } else {
                                Fiona_IdleAnim(f, -1);
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
                            Fiona_WalkLook(f);
                        } else {
                            Fiona_Stand(f);
                        }
                        FI(f, 0x1AD5C4, s32) = 0;
                        break;
                    case 2:
                        if (idle < 6) {
                            Fiona_RunLook(f);
                        } else {
                            FI(f, 0x1AD5C0, u32) = 0;
                            FI(f, 0x1AD5C4, s32) = 0;
                            Fiona_IdleAnim(f, -1);
                        }
                        break;
                    default:
                        FI(f, 0x1AD5C0, u32) = 0;
                        FI(f, 0x1AD5C4, s32) = 0;
                        Fiona_IdleAnim(f, -1);
                        break;
                    }
                } else {
                    /* stick held: walk, or run with the run button */
                    f->savedYaw = FIONA_STICK_HEADING(f);
                    if (!(Progress_TestFlag(gProgress, 0x1E) & 0xFF) && FI(f, 0x1AD5D8, u8) == 1) {
                        if (FI(f, 0x1AD584, s32) & 0x1) {
                            if (--FI(f, 0x1AD5E8, s32) >= 0) {
                                FI(f, 0x1AD5C4, s32) += 1;
                                Fiona_RunLook(f);
                            } else {
                                Fiona_Exhausted(f, g);
                                FI(f, 0x1AD5C4, s32) = 0;
                            }
                        } else {
                            FI(f, 0x1AD5C4, s32) += 1;
                            Fiona_RunLook(f);
                        }
                    } else {
                        FI(f, 0x1AD5C4, s32) = 0;
                        Fiona_WalkLook(f);
                    }
                }
            } else {
                /* full panic: run until out of breath */
                FI(f, 0x1AD624, f32) = 1.0f;
                if (--FI(f, 0x1AD5E8, s32) >= 0) {
                    Fiona_RunLook(f);
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
                        Fiona_RunLook(f);
                    }
                } else {
                    Fiona_RunLook(f);
                }
            }
        }
    }

    Character_RootTurn(&f->c);
    if (f->unk1AD588 == 2) {
        /* accelerating turn toward +0x1AD5E4 */
        FI(f, 0x1AD5B4, f32) = FI(f, 0x1AD5B4, f32) + 0x1.57254ep-10f /* 0x3AAB92A7 */;
        if (!(FI(f, 0x1AD5B4, f32) <= 0x1.aceea0p-5f /* 3 deg */)) {
            FI(f, 0x1AD5B4, f32) = 0x1.aceea0p-5f;
        }
        if (Actor_TurnToward(&f->c.a, FI(f, 0x1AD5E4, f32), FI(f, 0x1AD5B4, f32)) < FI(f, 0x1AD5B4, f32)) {
            f->unk1AD588 = 0;
        }
        f->savedYaw = f->c.a.angle[1];
    } else {
        Actor_TurnToward(&f->c.a, f->savedYaw, 0x1.657186p-3f /* 10 deg */);
    }

    /* root motion, scaled down when the stick points away from where she faces */
    Motion_RootMovement(f->c.motion, d, 0.0f);
    dz = d[2] * VCALL(f->c.motion, 0x44, f32 (*)(void *, Fiona *))(f->c.motion, f);
    d[2] = dz;
    Mtx_TurnY(m, f->savedYaw);
    Mtx_ApplyVector(d, m, d);
    axis[2] = 1.0f;
    *(s32 *)&axis[0] = 0;
    *(s32 *)&axis[1] = 0;
    sceVu0ApplyMatrix(fwd, f->c.a.rot, axis);
    sceVu0ApplyMatrix(dir, m, axis);
    func_0010E640(d, d, ((1.0f + sceVu0InnerProduct(dir, fwd)) / 2.0f) * FI(f, 0x1AD624, f32));
    *(s32 *)&d[3] = 0;
    Actor_Move(&f->c.a, d);
    sceVu0SubVector(moved, f->c.a.pos, f->c.a.prevPos);
    if (sceVu0InnerProduct(d, moved) < 0.0f) {
        f->c.a.navTri = f->c.a.prevNavTri;
        sceVu0CopyVector(f->c.a.pos, f->c.a.prevPos);
    }
    if (!(FI(f, 0x1AD584, s32) & 0x2) && f->unk1AD580 != 0xF && MOTION_SPEED(f->c.motion) <= 0.0f
        && Fiona_FindPushable(f, 0, dz) == 0) {
        f->unk1AD580 = 1;
        f->c.moveSub = 5;
        Actor_SetState(&f->c.a, &D_003B27C8);
    }
}

extern const PTMF D_003B25C8, D_003B25D8, D_003B25E8, D_003B25F8, D_003B2608, D_003B2618;
extern const PTMF D_003B2628, D_003B2638, D_003B2648, D_003B2658, D_003B2668, D_003B2678;
extern const PTMF D_003B2688;

/* +0x1AD71C mood request (9 = asked by Hewie's command), +0x1AD720 its argument;
 * +0x1AD724 the period counter of the +0x1AD719 alternation. */
static inline void Fiona_NudgePeriod(Fiona *f, s32 near, s32 far) {
    if (Actor_Distance(&f->c.a, gCharPartner->a.pos) < 50.0f) {
        FI(f, 0x1AD724, s32) += near;
    } else if (Actor_Distance(&f->c.a, gCharPartner->a.pos) < 100.0f) {
        FI(f, 0x1AD724, s32) += far;
    }
}

/* vtable +0x84: handle the Character state block (state[0]: 4 grabbed, 5 released, and the
 * action requests 2/3/9 (go through a door), 8, 0xB, 0xD, 0xE; 0xC with [1] = 6) while she is free to act.
 * state[0] == 7 is left pending; everything else is consumed. */
/* 0x001A0370 */
void Fiona_StateBlock(Fiona *f) {
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
        if (Fiona_React(f, st) == 0) {
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
            if (Actor_Touching(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) {
                st[0] = 0;
                break;
            }
            f->c.unk100 = st[2];
            f->c.moveSub = (st[1] == 0) ? 0x15 : 0x14;
            f->c.unk104[0] = VCALL(gDoors, 0x18, s32 (*)(VObject *, u32, f32 *))(gDoors, *(u8 *)&f->c.unk100, f->c.a.pos);
            f->targetParam = 0;
            Actor_SetState(&f->c.a, &D_003B25F8);
            f->unk1AD580 = 9;
            f->c.moveMode = 9;
            st[0] = 0;
            break;
        case 2: {
            VObject *rooms;
            sceVu0FVECTOR at;

            if (Actor_Touching(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) {
                st[0] = 0;
                break;
            }
            f->c.unk100 = st[2];
            f->c.moveSub = (st[1] == 0) ? 0x15 : 0x14;
            rooms = gRooms;
            if ((VCALL(rooms, 0x74, u32 (*)(VObject *, s32, u32))(rooms, f->c.a.room, *(u8 *)&f->c.unk100) & 0xFF) == 1) {
                VCALL(rooms, 0x34, u32 (*)(VObject *, u32, f32 *))(rooms, *(u8 *)&f->c.unk100, at);
            } else {
                sceVu0CopyVector(at, f->c.a.pos);
            }
            f->c.unk104[0] = VCALL(gDoors, 0x18, s32 (*)(VObject *, u32, f32 *))(gDoors, *(u8 *)&f->c.unk100, at);
            f->c.unk104[1] = 0;
            f->targetParam = 0;
            Actor_SetState(&f->c.a, &D_003B2608);
            f->unk1AD580 = 3;
            f->c.moveMode = 2;
            st[0] = 0;
            break;
        }
        case 3:
            if (Actor_Touching(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) {
                st[0] = 0;
                break;
            }
            RoomSlots_Enter(gProgress, (u8)st[2], SLOT_U8(f));
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
                NavTri *t = NavMesh_Tri(gNavMesh, f->c.a.navTri);

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
    u32 fl = NavMesh_Tri(gNavMesh, tri)->flags & 0x300000;

    return (side == 0 && fl == 0x100000) || (side == 1 && fl == 0x200000);
}

/* Whether exit `i` may be used: open, route not locked, allowed for her. */
static inline s32 Fiona_ExitUsable(Fiona *f, Progress *p, VObject *rooms, u32 i) {
    u32 route;

    if (Room_ExitOpen(rooms, f->c.a.room, i) != 1) {
        return 0;
    }
    route = Room_ExitRoute(rooms, f->c.a.room, i) & 0xFFFF;
    if (Progress_DoorUnlocked(p, route) & 0xFF) {
        return 0;
    }
    if (DoorHold_Usable(p, f->c.a.room, i) & 0xFF) {
        return 0;
    }
    return (Progress_DoorPassable(p, route, SLOT_U8(f)) & 0xFF) == 1;
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
    return Actor_Distance(&f->c.a, p);
}

/* Mood/flight AI (+0x1AD71C): 0 calm near Hewie, 1 waiting, 2 following Hewie, 3/4 fleeing
 * through the best exit (away from the pursuer), 5..8 leaving through some exit, 9 a given exit,
 * 10 caught; 0xB..0xE the same while she is not in the room being played. */
/* 0x0019D4E0 */
void Fiona_MoodAI(Fiona *f) {
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

            rooms = gRooms;
            ps = Room_Side(rooms, pr, gCharPursuer->door);
            fs = Room_Side(rooms, f->c.a.room, f->c.door);
            if (Character_Route(&f->c, pr, ps, fs, 1) != -1) {
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
        if (Fiona_HeadFor(f, gCharPartner->a.navTri, gCharPartner->a.pos, 0) != 0) {
            FIONA_MOOD(f) = 7;
            FIONA_EXIT(f) = 0xFF;
            f->c.unk124 = f->c.unk128;
        } else if (Fiona_Dist(f, gCharPartner->a.pos) < 30.0f) {
            FIONA_MOOD(f) = 0;
        }
        break;

    case 3:
        rooms = gRooms;
        if (FIONA_EXIT(f) != 0xFF) {
            tri = Room_ExitPosIn(rooms, FIONA_EXIT(f), at);
            if (Fiona_HeadFor(f, tri, at, 0) != 0) {
                FIONA_MOOD(f) = 0;
                break;
            }
            if (Fiona_Dist(f, at) < 1.0f) {
                if (!(Progress_ExitOpen(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF) && !Fiona_PostExit(f, FIONA_EXIT(f))) {
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
        if (Fiona_HeadFor(f, tri, at, 0) != 0) {
            FIONA_EXIT(f) = 0xFF;
            FIONA_MOOD(f) = 0;
            break;
        }
        FIONA_ROUTE0(f) = Room_ExitRoute(rooms, f->c.a.room, FIONA_EXIT(f));
        if ((Progress_ExitOpen(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF) == 1) {
            Room_ExitPosOut(rooms, FIONA_EXIT(f), out2);
            if (Fiona_Dist(f, out2) < Fiona_Dist(f, at)) {
                FIONA_MOOD(f) = 4;
                FIONA_EXIT2(f) = FIONA_EXIT(f);
            }
        }
        break;

    case 5: case 7:
        rooms = gRooms;
        if (FIONA_EXIT(f) != 0xFF) {
            tri = Room_ExitPosIn(rooms, FIONA_EXIT(f), at);
            if (Fiona_HeadFor(f, tri, at, 0) != 0) {
                FIONA_MOOD(f) = 0;
                break;
            }
            if (Fiona_Dist(f, at) < 1.0f) {
                if (!(Progress_ExitOpen(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF) && !Fiona_PostExit(f, FIONA_EXIT(f))) {
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
        if (Fiona_HeadFor(f, tri, at, 0) != 0) {
            FIONA_MOOD(f) = 0;
            break;
        }
        FIONA_ROUTE0(f) = Room_ExitRoute(rooms, f->c.a.room, FIONA_EXIT(f));
        if ((Progress_ExitOpen(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF) == 1) {
            Room_ExitPosOut(rooms, FIONA_EXIT(f), out2);
            if (Fiona_Dist(f, out2) < Fiona_Dist(f, at)) {
                FIONA_MOOD(f) = (FIONA_MOOD(f) == 5) ? 6 : 8;
                FIONA_EXIT2(f) = FIONA_EXIT(f);
            }
        }
        break;

    case 4: case 6: case 8:
        /* at / through the exit */
        rooms = gRooms;
        if (FIONA_EXIT2(f) == 0xFF) {
            tri = Room_ExitPosOut(rooms, FIONA_EXIT(f), at);
            if (Fiona_HeadFor(f, tri, at, 0) != 0) {
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
                rooms = gRooms;
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
        if (Fiona_HeadFor(f, tri, at, 0) != 0) {
            FIONA_MOOD(f) = 0;
        }
        break;

    case 9: {
        s32 side;

        if (FIONA_EXIT(f) == 0xFF) {
            FIONA_MOOD(f) = 0;
            break;
        }
        rooms = gRooms;
        side = Room_Side(rooms, f->c.a.room, f->c.door);
        if (Room_Exit78(rooms, f->c.a.room, FIONA_EXIT(f)) != 1 || Room_Exit70(rooms, f->c.a.room, FIONA_EXIT(f)) != 0
            || (Progress_ExitOpen(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF)) {
            FIONA_MOOD(f) = 0;
            break;
        }
        tri = Room_ExitPosIn(rooms, FIONA_EXIT(f), at);
        if (Fiona_WrongSide(side, tri) || Fiona_HeadFor(f, tri, at, 0) != 0) {
            FIONA_MOOD(f) = 0;
            break;
        }
        if (!(PursuerGroup_Fields(p, FIONA_EXIT(f), 0) & 0x4)) {
            break;
        }
        if (Progress_ExitOpen(p, f->c.a.room, FIONA_EXIT(f)) & 0xFF) {
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

#define DOOR_ISOPEN(d, door) VCALL(d, 0x28, s32 (*)(VObject *, s32))(d, door)
#define DOOR_KIND(d, door) VCALL(d, 0x30, s32 (*)(VObject *, s32))(d, door)
#define DOOR_SET(d, slot, door, side, v) VCALL(d, slot, void (*)(VObject *, s32, s32, s32))(d, door, side, v)
#define ROOM_DOOR_LINK(r, room, door) VCALL(r, 0x10, s32 (*)(VObject *, s32, s32))(r, room, door)

#define FI_HEWIE_NEAR(f) FI(f, 0x1AD5D5, u8)   /* 1: Hewie is with Fiona */

/* |the heading from Fiona to p, relative to hers| (the original wraps it once per use) */
static f32 fiona_turn_to(Fiona *f, f32 a) {
    if (Angle_Wrap(a - f->c.a.angle[1]) <= 0.0f) {
        return -Angle_Wrap(a - f->c.a.angle[1]);
    }
    return Angle_Wrap(a - f->c.a.angle[1]);
}

/* the call command's action: 0x2D to a creature within 10 ahead (Fiona in control state 1, the
 * creature in her room and reachable, within 45 degrees), with Hewie controlled (2) 0x2E when he
 * is with her and in state 8, else 0x2D; otherwise 0x23 */
/* 0x00184700 */
s32 Fiona_CallAction(Fiona *f) {
    u8 who = Progress_GameMode(gProgress);
    s32 i;

    if (who == 2) {
        if (FI_HEWIE_NEAR(f) == 1 && AT(gCharPartner, 0xF8, s32) == 8) {
            return 0x2E;
        }
        return 0x2D;
    }
    if (who == 1) {
        for (i = 0; i < 10; i++) {
            Actor *c = AT(gCreatures, i * 4, Actor *);
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
            if (Actor_TriTo(&f->c.a, c->pos, NAV_NONE) != tri) {
                continue;
            }
            if (fiona_turn_to(f, func_0031C5C0(d[0], d[2])) < 0x1.921fb60000000p-1f /* 0.7853982 */) {
                return 0x2D;
            }
        }
    }
    return 0x23;
}

/* the action code for Hewie command `cmd` from the controls: 0 call (Fiona_CallAction), 1 0x2C,
 * 2 (stay / come) 0x24 / 0x25 when he is with her within 12 (by his state), 0x27 from further
 * or when not in control, 3 0x2A / 0x28 within 15 (0x2A: his +0xC4 state 2 and facing within 60
 * degrees), else 0x29, 4 0x2B within 15 else 0x2F */
/* 0x001848F0 */
s32 Fiona_HewieCommandAction(Fiona *f, s32 cmd, s32 state) {
    u8 *h;
    u8 who;
    f32 d;
    s32 v;

    switch (cmd) {
    case 0:
        return Fiona_CallAction(f);
    case 1:
        return 0x2C;
    case 2:
        if ((u8)Progress_GameMode(gProgress) != 0 || FI_HEWIE_NEAR(f) != 1) {
            return 0x27;
        }
        d = Actor_Distance(&f->c.a, (f32 *)((u8 *)gCharPartner + 0x10));
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
        who = Progress_GameMode(gProgress);
        if (FI_HEWIE_NEAR(f) != 1) {
            return 0x29;
        }
        h = (u8 *)gCharPartner;
        if (!(Actor_Distance(&f->c.a, (f32 *)(h + 0x10)) < 15.0f)) {
            return 0x29;
        }
        if (AT(h, 0xC4, s32) == 2) {
            return fiona_turn_to(f, Actor_HeadingTo(&f->c.a, (f32 *)(h + 0x10))) < 0x1.0c15240000000p+0f /* 1.0471976 */ ? 0x2A : 0x29;
        }
        if (who != 0 || AT(h, 0xF35C0, s32) == 3) {
            return 0x29;
        }
        return 0x28;
    case 4:
        if ((u8)Progress_GameMode(gProgress) != 0 || FI_HEWIE_NEAR(f) != 1) {
            return 0x2F;
        }
        h = (u8 *)gCharPartner;
        if (!(Actor_Distance(&f->c.a, (f32 *)(h + 0x10)) < 15.0f) || AT(h, 0xF35C0, s32) == 3) {
            return 0x2F;
        }
        return 0x2B;
    }
    return -1;
}

/* at the start in a door (+0xF8: 2 passing through, 3 standing in it): set the door's state
 * (open sides) and the progress records for it */
/* 0x00184BF0 */
void Fiona_StartInDoor(Fiona *f) {
    u8 *c = (u8 *)f;
    s32 st = AT(c, 0xF8, s32);
    VObject *doors;

    if (st == 3) {
        RoomSlots_Leave(gProgress, AT(c, 0x100, u8), AT(c, 0x20, u8));
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
    doors = gDoors;
    if (!DOOR_ISOPEN(doors, AT(c, 0x100, u8))) {
        if ((u8)DOOR_KIND(doors, AT(c, 0x100, u8)) == 1
            && (u16)ROOM_DOOR_LINK(gRooms, AT(c, 0x30, s32), AT(c, 0x100, u8)) != 0xFFFF) {
            DoorHold_Open(gProgress, AT(c, 0x30, s32), AT(c, 0x100, u8), 0xFF);
        }
        doors = gDoors;
        DOOR_SET(doors, 0x20, AT(c, 0x100, u8), 0, 0x60000);
        DOOR_SET(doors, 0x1C, AT(c, 0x100, u8), 1, 0x60000);
    } else {
        if ((u8)DOOR_KIND(doors, AT(c, 0x100, u8)) == 1
            && (u16)ROOM_DOOR_LINK(gRooms, AT(c, 0x30, s32), AT(c, 0x100, u8)) != 0xFFFF) {
            DoorHold_Shut(gProgress, AT(c, 0x30, s32), AT(c, 0x100, u8), 0xFF);
        }
        doors = gDoors;
        DOOR_SET(doors, 0x20, AT(c, 0x100, u8), 1, 0x60000);
        DOOR_SET(doors, 0x1C, AT(c, 0x100, u8), 0, 0x60000);
    }
}

/* show the model parts for what she has equipped (gSubScreen +0x10: equipment slots 1 and 3;
 * items 0x86..0x89 -> part variants 1..4, else 0; items 0x8A..0x8D -> 6..9, else 5) */
/* 0x00182FC0 */
void Fiona_ShowEquipment(Fiona *f) {
    VObject *equip = gSubScreen;
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
/* 0x001F1D60 */
void Lists_Clear(u8 *o) {
    u32 i;

    for (i = 0; i < 5; i++) {
        AT(o, 0x28 + i * 4, s32) = 0;
        AT(o, 0x14 + i * 4, s32) = 0;
    }
}

/* out.xyz = m's 3x3 part x v. (VU0 macro code: its w is whatever the VU register last held;
   0 here - the callers only use x, y, z) */
/* 0x002E2DA0 */
void Mtx_ApplyVector(f32 *out, f32 (*m)[4], const f32 *v) {
    f32 x = v[0], y = v[1], z = v[2];
    s32 i;

    for (i = 0; i < 3; i++) {
        out[i] = m[0][i] * x + m[1][i] * y + m[2][i] * z;
    }
    *(s32 *)&out[3] = 0;
}

/* destructor (vtable D_00479600) */
/* 0x0035A230 */
void *StrikeMark_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479600;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* 0x0035AE90 */
void StrikeMark_Start(u8 *p) {
    B7_B(p, 0x78) = 0;
    B7_W(p, 0x74) = 0;
    B7_W(p, 0x70) = 2;
}

/* idle animation `anim` with weight variant `variant`: blended over `blend` frames (-1: at
 * once) */
static void fiona_idle(Fiona *f, s32 anim, s32 blend, s32 variant) {
    if (blend == -1) {
        Motion_PlayTable(f->c.motion, anim, variant);
    } else {
        Motion_PlayBlend(f->c.motion, anim, blend, variant);
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
/* 0x001855F0 */
void Fiona_IdleAnim(Fiona *f, s32 blend) {
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

        idle = (u8)Progress_GameMode(p) == 2 ? 5 : 0;
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
            idle = (u8)Progress_GameMode(gProgress) == 2 ? 5 : 0;
            if (!(anim == idle && variant == 2) || idle_weight_changed(f, heart)) {
                fiona_idle(f, idle, blend, 2);
            }
            AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = heart;
            FI(f, 0x1AD628, f32) = heart;
        } else {
            idle = (u8)Progress_GameMode(gProgress) == 2 ? 5 : 0;
            if (!(anim == idle && variant == -1)) {
                fiona_idle(f, idle, blend, -1);
            }
            AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = 1.0f;
        }
        return;
    }
    idle = (u8)Progress_GameMode(gProgress) == 2 ? 5 : 0;
    if (!(anim == idle && variant == 3) || idle_weight_changed(f, fear)) {
        fiona_idle(f, idle, blend, 3);
    }
    AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = fear;
    FI(f, 0x1AD628, f32) = fear;
}

/* change Fiona's fear (+0x1AD5F4, 0..99) by `d`; the worn accessory (sub screen slot 3)
 * scales it: 0x8A less gain, 0x8B less gain and more loss, 0x8C no gain and double loss,
 * 0x8D none */
/* 0x00181010 */
void Fiona_ChangeFear(Fiona *f, f32 d) {
    VObject *items = (VObject *)gSubScreen;
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
    if (!(Angle_Wrap(a) <= 0.0f)) {
        return Angle_Wrap(a);
    }
    return -Angle_Wrap(a);
}

/* Fiona's movement input, each frame. Normally the left stick (or the d-pad), camera
 * relative: after a camera cut the old camera keeps steering while the stick is held
 * (mode 1, then 2 while the direction holds within 15 degrees); 0x4000 runs. While the game
 * walks her (gProgress +0x1FBEC1): towards the target point (states 2..9; 10 once the
 * pursuer is within reach), or (10) at the pursuer, grabbing it (action 8) when facing it. */
/* 0x00187650 */
void Fiona_MoveInput(Fiona *f) {
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
    if (!cut && VCALL(gCamera, 0x94, s32 (*)(VObject *))(gCamera) != -1) {
        s32 prev = VCALL(gCamera, 0x90, s32 (*)(VObject *))(gCamera);

        cut = prev != VCALL(gCamera, 0x94, s32 (*)(VObject *))(gCamera);
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
                Mtx_TurnY(rot, VCALL(gCamera, 0x68, f32 (*)(VObject *))(gCamera));
                Mtx_ApplyVector(v, rot, n);
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
            Mtx_TurnY(rot, FI(f, FMOVE_CAMYAW, f32));
        } else {
            f32 d[4] __attribute__((aligned(16)));

            sceVu0SubVector(d, e, &FI(f, FMOVE_STICK, f32));
            if (__builtin_sqrtf(sceVu0InnerProduct(d, d)) < k001.f) {
                FI(f, FMOVE_MODE, u8) = 2;
                FI(f, 0x1AD5B4, u32) = 0x3C0EFA35;   /* 0.5 degrees */
                Mtx_TurnY(rot, VCALL(gCamera, 0x68, f32 (*)(VObject *))(gCamera));
            }
        }
        Mtx_ApplyVector(v, rot, n);
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
        Mtx_TurnY(rot, FI(f, FMOVE_CAMYAW, f32));
        Mtx_ApplyVector(v, rot, n);
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
        Mtx_TurnY(rot, VCALL(gCamera, 0x68, f32 (*)(VObject *))(gCamera));
        Mtx_ApplyVector(v, rot, n);
        func_0010E640(&FI(f, FMOVE_DIR, f32), v, -1.0f);
        break;
    }
    sceVu0CopyVector(&FI(f, FMOVE_STICK, f32), e);
    if (FI(f, FMOVE_MODE, u8) == 0) {
        FI(f, FMOVE_CAMYAW, f32) = VCALL(gCamera, 0x68, f32 (*)(VObject *))(gCamera);
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
        Character_WaypointAhead(&f->c, &tri, pt, 1.5f);
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
/* 0x0018B600 */
void Fiona_StateIdleStep(Fiona *f) {
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        if (fiona_motion_kind(AT(f->c.motion, 0x55C, s32)) == 0) {
            f->c.unkE1 = 1;
        } else {
            Fiona_IdleAnim(f, -1);
        }
    }
    Character_RootMoveMasked(&f->c);
}

#define FLOOK_ON     0x1AD5FC   /* u8: looking at a character */
#define FLOOK_WHO    0x1AD600   /* Character *: whom */
#define FLOOK_POINT  0x1AD610   /* vec: its head */
#define FLOOK_HOLD   0x1AD620   /* s32: frames to keep looking */

/* whether Fiona notices character c: near enough, ahead of her (cos > 0.6 with `fwd`) and
 * reachable on the nav mesh */
static s32 fiona_sees(Fiona *f, Character *c, f32 range, f32 *fwd) {
    static const union { u32 u; f32 f; } k06 = {0x3F19999A};
    f32 d[4] __attribute__((aligned(16)));

    if (!(Actor_Distance(&f->c.a, c->a.pos) < range)) {
        return 0;
    }
    sceVu0SubVector(d, c->a.pos, f->c.a.pos);
    if (sceVu0InnerProduct(fwd, d) <= k06.f) {
        return 0;
    }
    return (u8)Actor_CanWalkBetween(f, f->c.a.navTri, c->a.navTri, f->c.a.pos, c->a.pos, 0) == 1;
}

/* Fiona's head, each frame: she looks at the pursuer (within 200) or Hewie (within 50) when
 * ahead of her and reachable, the head angles clamped (pitch -18..45 degrees, yaw +-90, or
 * +-108 while held or in moves 0xD / 0xB-0x20); otherwise towards where she is turning. The
 * motion eases the head there, faster for bigger changes and while she turns. */
/* 0x00186180 */
void Fiona_Head(Fiona *f) {
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
            Motion_LookAt(f->c.motion, &FI(f, FLOOK_POINT, f32), &pitch, &yaw);
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
            yaw = Angle_Wrap(FI(f, 0x1AD5E0, f32) - f->c.a.angle[1]);
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
    Motion_EaseTilt(f->c.motion, pitch, yaw, dp, dy + turn);
}

extern void Fiona_Voice(Fiona *f, s32 id, s32, s32, s32);   /* a voice */
extern void Fiona_CallHewie(Fiona *f);

/* the sounds of Fiona's motions, on their key frames (motion events 1 and 0x10): steps,
 * crouching, falling (with a noise others hear), voices */
/* 0x00181F20 */
void Fiona_MotionSounds(Fiona *f) {
    s32 v;

    if (f->c.unk14D0 != 0 && f->c.unk14D0 != 5) {
        return;
    }
    if ((u8)Motion_EventFlags(f->c.motion, 0, 0, 1) & 1) {
        switch (AT(f->c.motion, 0x55C, s32)) {
        case 1:
            if (f->c.moveMode == 0xD) {
                Actor_PlaySound(&f->c.a, 0x39, 5, 0, 0, NULL);
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
                    Fiona_CallHewie(f);
                } else {
                    Actor_PlaySound(&f->c.a, 0x33, 5, 0, 0, NULL);
                }
            }
            break;
        case 0xD01:
        case 0xD00:
            Actor_PlaySound(&f->c.a, 0x3C, 5, 0, 0, NULL);
            break;
        case 0xE00: {
            Progress *p = gProgress;

            v = Progress_GetVar(p, 0x26) & 0xFF;
            if (v != 7 && v != 6) {
                Actor_PlaySound(&f->c.a, 0x3C, 5, 0, 0, NULL);
            }
            Noise_Make((u8 *)p + 0x778, 0x1F, f->c.a.room, f->c.a.navTri, 0xFFFF);
            break;
        }
        }
    }
    if ((u8)Motion_EventFlags(f->c.motion, 0, 0, 1) & 0x10) {
        switch (AT(f->c.motion, 0x55C, s32)) {
        case 0x403:
            Actor_PlaySound(&f->c.a, 0xF, 5, 0, 0, NULL);
            break;
        case 0xF02:
        case 0x1404:
        case 0x1403:
        case 0x1500:
            Fiona_Voice(f, 0x7D, 5, 0, 0);
            break;
        case 0xB01:
            Actor_PlaySound(&f->c.a, 0x7E, 5, 0, 0, NULL);
            break;
        case 0x100B:
        case 0x1008:
        case 0xB00:
            Fiona_Voice(f, 0x80, 5, 0, 0);
            break;
        case 0x609:
        case 0x608:
            Actor_PlaySound(&f->c.a, 0x71, 5, 0, 0, NULL);
            break;
        case 0xE00:
            v = Progress_GetVar(gProgress, 0x26) & 0xFF;
            if (v == 6) {
                Actor_PlaySound(&f->c.a, 0x21, 5, 0, 0, NULL);
            } else if (v == 7) {
                Actor_PlaySound(&f->c.a, 0x20, 5, 0, 0, NULL);
            }
            break;
        }
    }
}

extern f32 D_003B2478, D_003B247C;   /* the ladder's foot offset (x, z) */

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
/* 0x001869D0 */
void Fiona_Footsteps(Fiona *f) {
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
    if ((u8)VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 1) {
        return;
    }
    p = gProgress;
    if ((u8)Progress_TestFlag(p, 8) == 1) {
        return;
    }
    l = (u8)Motion_FootPos(f->c.motion, left, 1, 0.0f, 1.0f);
    r = (u8)Motion_FootPos(f->c.motion, right, 0, 0.0f, 1.0f);
    if (fiona_motion_kind(AT(f->c.motion, 0x55C, s32)) == 0) {
        if (AT(f->c.motion, 0x554, s32) != -1 && !(AT(f->c.motion, 0x550, f32) <= 0.0f)) {
            l = (u8)Motion_FootDownPrev(f->c.motion, 1, 0.0f);
            r = (u8)Motion_FootDownPrev(f->c.motion, 0, 0.0f);
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
    Mtx_ApplyPoint(foot, m, step == 1 ? left : right);
    tri = Actor_TriOfOnMesh(&f->c.a, foot);
    nm = gNavMesh;
    t = step_tri(nm, tri != (u32)-1 ? tri : f->c.a.navTri);
    room = f->c.a.room;
    if (room == 7 || room == 0xD1 || room == 0x106) {
        Character_WaterStep(&f->c, foot, f->c.moveMode == 0 && f->c.moveSub == 2);
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
            tri = Actor_DoorFront(f, f->c.unk100, 0, v, tmp);
            if (Actor_TriFrom(&f->c.a, foot, tri, tmp, 0) == (u32)-1) {
                sound = 0x14;
            } else if (id == 0x703) {
                tri = VCALL(nm, 0x5C, u32 (*)(NavMesh *, s32, s32, f32 *))(nm, f->c.unk100, 0, tmp);
                tri = Actor_TriFrom(&f->c.a, foot, tri, tmp, 0);
                if (tri != (u32)-1) {
                    t = step_tri(nm, tri);
                    flags = t->flags;
                }
            }
            break;
        case 7:
            tri = VCALL(nm, 0x5C, u32 (*)(NavMesh *, s32, s32, f32 *))(nm, f->c.unk100, 1, tmp);
            tri = Actor_TriFrom(&f->c.a, foot, tri, tmp, 0);
            if (tri != (u32)-1) {
                t = step_tri(nm, tri);
                flags = t->flags;
            }
            break;
        }
    }
    if (sound != -1) {
        Actor_PlaySound(&f->c.a, sound, 4, 0, 0, NULL);
        Noise_Make((u8 *)p + 0x778, 4, f->c.a.room, f->c.a.navTri, 0xFFFF);
        return;
    }

    if (flags == (u32)-1) {
        flags = t->flags;
    }
    base = AT(gProgress, 0x1FBEC0, u8) != 0 ? 0x15 : 0;
    switch (flags & 0x2018000) {
    case 0x2008000:
        if ((u8)VCALL(gSound, 0xA4, s32 (*)(VObject *, s32))(gSound, 6) == 1) {
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
    Motion_RootMovement(f->c.motion, speed, 0.0f);
    x = (speed[2] - k04.f) / k07.f;
    if (x < 0.0f) {
        x = 0.0f;
    }
    if (!(x <= 1.0f)) {
        x = 1.0f;
    }
    vol = (u32)(2.0f * x) & 0x7F;
    if (gSubScreen != NULL && VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 0) == 0x80) {
        Actor_PlaySound(&f->c.a, base, bank, -0x30, (s8)vol, NULL);
        noise = f->c.moveMode == 0 && f->c.moveSub == 2 ? 5 : 1;
    } else {
        Actor_PlaySound(&f->c.a, base, bank, 0, (s8)vol, NULL);
        noise = f->c.moveMode == 0 && f->c.moveSub == 2 ? 0x14 : 4;
    }
    Noise_Make((u8 *)p + 0x778, noise, f->c.a.room, f->c.a.navTri, 0xFFFF);
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
 * (gObstacles +0x24: the objects on a triangle, as bits) as +0x1AD560 (else -1). 0, or -1 when
 * there is nothing to push. */
/* 0x00188280 */
s32 Fiona_FindPushable(Fiona *f, s32 pick, f32 reach) {
    static const union { u32 u; f32 f; } kHalfPi = {0x3FC90FDB}, kPi = {0x40490FDB};
    VObject *objs = gObstacles;
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
    nm = gNavMesh;
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
    tri = Actor_TriTo(&f->c.a, at, 0);
    if (tri == NAV_NONE || !(NavMesh_TriFlags(nm, tri) & NAV_PUSHABLE)) {
        return -1;
    }
    a = VCALL(objs, 0x24, u32 (*)(VObject *, u32))(objs, tri);
    push_probe_point(f, 2.0f, reach, at);
    tri = Actor_TriTo(&f->c.a, at, 0);
    if (tri == NAV_NONE || !(NavMesh_TriFlags(nm, tri) & NAV_PUSHABLE)) {
        return -1;
    }
    b = VCALL(objs, 0x24, u32 (*)(VObject *, u32))(objs, tri);

    /* the wall edge's direction, turned a quarter: into the wall, from the side she is on */
    t = NavMesh_Tri(nm, from);
    sceVu0SubVector(d, t->v[e + 1 < 3 ? e + 1 : 0], t->v[e]);
    ang = Angle_Wrap(kHalfPi.f + func_0031C5C0(d[0], d[2]));
    if (!(Angle_Wrap(ang - AT(f, 0x54, f32)) <= 0.0f)) {
        diff = Angle_Wrap(ang - AT(f, 0x54, f32));
    } else {
        diff = -Angle_Wrap(ang - AT(f, 0x54, f32));
    }
    if (!(diff <= kHalfPi.f)) {
        ang = Angle_Wrap(kPi.f + ang);
    }
    d[0] = 0.0f;
    d[1] = 0.0f;
    d[2] = 1.0f;
    Mtx_TurnY(m, ang);
    Mtx_ApplyVector(FIONA_STICK(f), m, d);
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
/* 0x00185FC0 */
void Fiona_HeadLook(Fiona *f) {
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
        Motion_LookAt(f->c.motion, (f32 *)((u8 *)f + FLOOK_POINT), &pitch, &yaw);
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

/* keep Fiona out of the others, each frame: overlapping the pursuer (when +0x1AD5D7), she is
 * pushed out to their radii (+0xC8) apart - along the pursuer's walk when it comes at her, else
 * straight away from it - unless that is off the floor (then +0x2A); overlapping the partner
 * (+0x1AD5D5, unless Progress +0x1FBEC1) she steps out of it or back to where she was */
/* 0x00188960 */
void Fiona_KeepApart(Fiona *f) {
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
        (u8)Actor_Touching(&f->c.a, (Actor *)pu, 0.0f, 0.0f) == 1) {
        sceVu0SubVector(d, (f32 *)(a + 0x10), (f32 *)(pu + 0x10));
        dist = __builtin_sqrtf(__builtin_fabsf(d[2] * d[2] + d[0] * d[0]));
        rsum = AT(a, 0xC8, f32) + AT(gCharPursuer, 0xC8, f32);
        Motion_RootMovement(AT(gCharPursuer, 0xF0, u8 *), v, 0.0f);
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
        tri = Actor_TriTo(&f->c.a, at, -1);
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
    if ((u8)Actor_Touching(&f->c.a, (Actor *)gCharPartner, 0.0f, 0.0f) != 1) {
        return;
    }
    if (Actor_PushOut(&f->c.a, (Actor *)gCharPartner) == -1) {
        AT(a, 0x34, u32) = AT(a, 0x38, u32);
        sceVu0CopyVector((f32 *)(a + 0x10), (f32 *)(a + 0x40));
    }
    AT(a, 0x124, s32) = AT(a, 0x128, s32);
}

#define FWALK_BLEND 0x1AD628   /* f32: the walk blend last set */

/* the walk's look, each frame (+0xFC set while idle +0xF8): from Fiona's state +0x1AD5F4
 * ((100 - it) / 60) and +0x1AD5F8 ((1800 - it) / 1800) - the walk (0x200, mode 2 0x208) alone,
 * or blended with 0x201 or 0x204 by those amounts; a blend is reset only when it moves more
 * than 0.1 */
/* 0x00185CF0 */
void Fiona_WalkLook(Fiona *f) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD};
    void *m;
    s32 cur, next, base, change;
    f32 a, b, d;

    if (AT(f, 0xF8, s32) == 0) {
        AT(f, 0xFC, s32) = 1;
    }
    cur = AT(f->c.motion, 0x55C, s32);
    next = AT(f->c.motion, 0x560, s32);
    base = (u8)Progress_GameMode(gProgress) == 2 ? 0x208 : 0x200;
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
            Motion_PlayTable(f->c.motion, base, 0x204);
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
            Motion_PlayTable(f->c.motion, base, 0x201);
        }
        m = f->c.motion;
        AT(AT(m, 0x6A4, u8 *), 0x1C, f32) = a;
        FI(f, FWALK_BLEND, f32) = a;
        return;
    }
    if (!(cur == base && next == -1)) {
        Motion_PlayTable(f->c.motion, base, -1);
    }
    AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = 1.0f;
}

extern const PTMF D_003B2DC8;   /* Fiona's state: turning on the spot */

/* State: turning on the spot to +0x1AD5E0 (10 degrees a frame): once there, back to idle;
 * meanwhile the turn left / right animation (0x400 / 0x401) - each only between animations
 * (no blend running) */
/* 0x0018A5D0 */
void Fiona_StateTurnOnSpot(Fiona *f) {
    static const union { u32 u; f32 f; } k10deg = {0x3E32B8C3};
    f32 left = Actor_TurnToward(&f->c.a, FI(f, 0x1AD5E0, f32), k10deg.f);

    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        if (left == 0.0f) {
            Fiona_IdleAnim(f, -1);
        } else if (Angle_Wrap(FI(f, 0x1AD5E0, f32) - AT(f, 0x54, f32)) < 0.0f) {
            if (AT(f->c.motion, 0x55C, s32) != 0x400) {
                Motion_PlayTable(f->c.motion, 0x400, -1);
            }
        } else if (AT(f->c.motion, 0x55C, s32) != 0x401) {
            Motion_PlayTable(f->c.motion, 0x401, -1);
        }
    }
    Actor_SetState(&f->c.a, &D_003B2DC8);
}

/* State: turning while standing, to +0x1AD5E0 (10 degrees a frame), between animations: in a
 * move she goes idle once within 90 degrees; turned all the way, her action is cleared and she
 * stands (or, held +0xE0, is held), Progress flag 0x2B off */
/* 0x0018A210 */
void Fiona_StateTurnStanding(Fiona *f) {
    static const union { u32 u; f32 f; } k10deg = {0x3E32B8C3}, kHalfPi = {0x3FC90FDB};
    f32 left = Actor_TurnToward(&f->c.a, FI(f, 0x1AD5E0, f32), k10deg.f);

    if (!(AT(f->c.motion, 0x550, f32) <= 0.0f)) {
        return;
    }
    if (fiona_motion_kind(AT(f->c.motion, 0x55C, s32)) != 0) {
        if (left < kHalfPi.f) {
            Fiona_IdleAnim(f, -1);
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
            Fiona_IdleAnim(f, -1);
        }
        Actor_SetState(&f->c.a, &D_003B25A8);
    } else {
        Actor_SetState(&f->c.a, &D_003B25B8);
    }
    Progress_ClearFlag(gProgress, 0x2B);
}

/* State step: +0x2B set clears +0x1AD5BC first */
/* 0x0018B570 */
void Fiona_StateStep(Fiona *f) {
    if (AT(f, 0x2B, u8) == 1) {
        FI(f, 0x1AD5BC, u8) = 0;
    }
    Character_RootMove((Character *)f);
}

/* the run's look, each frame (+0xFC 2 while idle +0xF8), as the walk's (Fiona_WalkLook) with the
 * run 0x202 and its blends 0x203 / 0x205; tired (+0x1AD584 bit 1): the tired run 0x206 alone */
/* 0x00185310 */
void Fiona_RunLook(Fiona *f) {
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
            Motion_PlayTable(f->c.motion, 0x206, -1);
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
            Motion_PlayTable(f->c.motion, 0x202, 0x205);
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
            Motion_PlayTable(f->c.motion, 0x202, 0x203);
        }
        AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = a;
        FI(f, FWALK_BLEND, f32) = a;
        return;
    }
    if (!(cur == 0x202 && next == -1)) {
        Motion_PlayTable(f->c.motion, 0x202, -1);
    }
    AT(AT(f->c.motion, 0x6A4, u8 *), 0x1C, f32) = 1.0f;
}

extern const PTMF D_003B2B58, D_003B2B68, D_003B2B78, D_003B2B88, D_003B2B98, D_003B2BA8, D_003B2BB8,
    D_003B2BC8, D_003B2BD8, D_003B2BE8, D_003B2BF8, D_003B2C08;   /* her states after an action */

/* the character `c` is in sight of her (their nav triangles and positions, no mask) */
static inline s32 fiona_in_sight(Fiona *f, Character *c) {
    return (u8)Actor_CanWalkBetween(f, f->c.a.navTri, c->a.navTri, f->c.a.pos, c->a.pos, 0) == 1;
}

extern const PTMF D_003B2C18;

/* back to an idle animation after action moveSub 0x23..0x2F (progress flag 0x25: at random
 * 1 or 0xC02) */
/* 0x00184570 */
void Fiona_IdleAfterCommand(Fiona *f) {
    if ((u8)Progress_TestFlag(gProgress, 0x25) != 0) {
        if (VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 0.5f) {
            Motion_PlayTable(f->c.motion, 1, -1);
        } else {
            Motion_PlayTable(f->c.motion, 0xC02, -1);
        }
        return;
    }
    switch (f->c.moveSub) {
    case 0x29:
    case 0x2F:
        Motion_PlayTable(f->c.motion, 0xC02, -1);
        break;
    case 0x2E:
        Motion_PlayTable(f->c.motion, 0xC0E, -1);
        break;
    case 0x23:
        Motion_PlayTable(f->c.motion, 0xC04, -1);
        break;
    case 0x24:
        Motion_PlayTable(f->c.motion, 0xC06, -1);
        break;
    case 0x2C:
        Motion_PlayTable(f->c.motion, 0xC02, -1);
        break;
    case 0x2A:
        Motion_PlayTable(f->c.motion, 0xC0A, -1);
        break;
    case 0x25:
    case 0x27:
        Motion_PlayTable(f->c.motion, 0xC03, -1);
        break;
    case 0x2D:
        Motion_PlayTable(f->c.motion, 0xC00, -1);
        break;
    }
}

/* the looking-around idle (animation 0xC0E, then 0xC0F): the one looked at stays the one to
 * face; Fiona_CommandHewie on its motion event 2; at its end back to idle */
/* 0x0018F580 */
void Fiona_StateLookAround(Fiona *f) {
    void *m;

    if (FI(f, 0x1AD664, Character *) != NULL) {
        FI(f, 0x1AD5FC, u8) = 1;
        FI(f, 0x1AD600, Character *) = FI(f, 0x1AD664, Character *);
    }
    if ((u8)Motion_EventFlags(f->c.motion, 0, 0, 1) & 2) {
        Fiona_CommandHewie(f);
    }
    m = f->c.motion;
    if ((MOTION_EVENTS(m) & 0x20) != 0) {
        if (*(s32 *)((u8 *)m + 0x55C) == 0xC0E) {
            Motion_Play(m, 0xC0F, -1);
            return;
        }
        Fiona_ToIdle(f);
    }
}

/* the end of a looking action: (unless progress flag 0x25, which forgets it) the one looked at
 * (+0x1AD664) stays the one to face (+0x1AD5FC set, +0x1AD600); once the animation is over
 * Fiona_IdleAfterCommand and the next state D_003B2C18 */
/* 0x0018F760 */
void Fiona_StateLookEnd(Fiona *f) {
    if ((u8)Progress_TestFlag(gProgress, 0x25) != 0) {
        FI(f, 0x1AD664, Character *) = NULL;
    } else if (FI(f, 0x1AD664, Character *) != NULL) {
        FI(f, 0x1AD5FC, u8) = 1;
        FI(f, 0x1AD600, Character *) = FI(f, 0x1AD664, Character *);
    }
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        Fiona_IdleAfterCommand(f);
        Actor_SetState(&f->c.a, &D_003B2C18);
    }
}

/* an action's state once its animation (motion +0x550) is over, by the action (moveSub +0xFC; 0x23 ..
 * 0x2F): she may glance at Hewie (+0x1AD5D5 he can act) or the pursuer (+0x1AD5D7) when in
 * sight (+0x1AD664 the one looked at, cleared first). 0x24 (progress test 2/4 for her slot)
 * goes back to idle unless it passes; 0x2B and 0x28 change the action to
 * 0x2F / 0x29 when theirs (2/0, 2/2) fail. Then the character's update (Character_RootMoveMasked). */
/* 0x0018F870 */
void Fiona_StateActionOver(Fiona *f) {
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
                        VCALL(gRooms, 0x30, void (*)(VObject *, u8, f32 *))(gRooms, f->c.door, tmp);
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
                        VCALL(gRooms, 0x30, void (*)(VObject *, u8, f32 *))(gRooms, f->c.door, tmp);
                    }
                } else {
                    FI(f, 0x1AD664, Character *) = gCharPartner;
                }
            }
            Actor_SetState(&f->c.a, &D_003B2BA8);
            break;
        case 0x24:
            if ((u8)SlotCmd_Give(gProgress, 2, 4, (u8)f->c.a.slot, 1, 0, 0.0f) != 1) {
                Fiona_ToIdle(f);
            } else {
                Actor_SetState(&f->c.a, &D_003B2BB8);
            }
            break;
        case 0x2B:
            if ((u8)SlotCmd_Give(gProgress, 2, 0, (u8)f->c.a.slot, 1, 0, 0.0f) != 1) {
                f->c.moveSub = 0x2F;
                Actor_SetState(&f->c.a, &D_003B2BD8);
            } else {
                Actor_SetState(&f->c.a, &D_003B2BC8);
            }
            break;
        case 0x28:
            if ((u8)SlotCmd_Give(gProgress, 2, 2, (u8)f->c.a.slot, 1, 0, 0.0f) != 1) {
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
    Character_RootMoveMasked(&f->c);
}

/* ---- Fiona taking a hit or being caught (state block [0] 4) ---- */

extern const PTMF D_003B2DD8, D_003B2DE8, D_003B2DF8, D_003B2E08, D_003B2E18, D_003B2E28, D_003B2E38;
extern const PTMF D_003B2E48, D_003B2E58, D_003B2E68, D_003B2E78, D_003B2E88;

#define CHARM_ITEM 0x88   /* worn (the item manager gSubScreen +0x10, slot 1), she cannot be caught */

/* the reaction for request `req` (the state block's [1]), -1: none. While an event runs or
 * progress flag 8 is set, none; a request about character [2] needs it present. 0xD falls
 * 0x13; 1 / 2 / 4 knocked down 0xE / 0xF / 0xA (on a slope, stairs or at a door: 0xC / 0xD;
 * crawling (3/7): 8); 3 grabbed 0x10 (already: a shake); 5 carried by door [4] 0xB; 6 caught
 * 0x20 (with the charm on, always); 0xA 9; 0xC 0x12 - none while already reacting (4) or down
 * (3 / 0xA) */
/* 0x00184E00 */
s32 Fiona_Reaction(Fiona *f, s32 req) {
    Progress *p;
    s32 m;
    u32 i;
    s32 n;

    if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) != 0) {
        return -1;
    }
    p = gProgress;
    if ((Progress_TestFlag(p, 8) & 0xFF) == 1) {
        return -1;
    }
    if (req == 5) {
        if (!(VCALL(gDoors, 0x40, u32 (*)(VObject *, u32))(gDoors, (u8)f->c.state[4]) & 0xFF)) {
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
        if (NavMesh_TriFlags(gNavMesh, f->c.a.navTri) & 0x80003) {   /* (off the mesh: address 0x3C) */
            return req == 1 ? 0xC : 0xD;
        }
        for (i = 0; i < 8; i = (i + 1) & 0xFF) {
            if (PursuerGroup_Fields(p, i, (u8)f->c.a.slot) & 0xFF & 0x20) {
                return req == 1 ? 0xC : 0xD;
            }
        }
        n = gNavMesh->numDoors;
        for (i = 0; (s32)i < n; i++) {
            if (RoomSlots_Bytes(p, i & 0xFF, (u8)f->c.a.slot) & 0xFF & 8) {
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
            VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0xFF, 0x10);
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
        if (gSubScreen != NULL && VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 1) == CHARM_ITEM) {
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
 * adds): action 4 with the reaction Fiona_Reaction picked (-1: none), the rumble, and its state */
/* 0x00182340 */
s32 Fiona_React(Fiona *f, s32 *st) {
    s32 kind = Fiona_Reaction(f, st[1]);
    Progress *p;

    if (kind == -1) {
        return -1;
    }
    p = gProgress;
    Panic_Fright((u8 *)p + 0x7B8, *(f32 *)&st[5]);
    if (kind == 0x20 && gSubScreen != NULL && VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 1) == CHARM_ITEM) {
        return -1;
    }
    f->targetParam = 0;
    f->c.unk14D0 = 0;
    Motion_Unfreeze(f->c.motion);
    f->c.a.unk2D = 1;
    Fiona_StartInDoor(f);
    f->c.unk100 = st[2];
    f->unk1AD580 = 0xA;
    f->c.moveMode = 4;
    f->c.moveSub = kind;
    VCALL(gSound, 0x10, void (*)(VObject *, s32, s32, s32))(gSound, 0, 0x800000, 4);
    switch (kind) {
    case 0x13: {
        NavMesh *nav;
        u32 i;
        s32 n;

        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0xA0, 8);
        nav = gNavMesh;
        f->c.a.unk2A = 0;
        FI(f, 0x1AD6C0, s32) = 1;
        if (NavMesh_TriFlags(nav, f->c.a.navTri) & 0x80003) {
            FI(f, 0x1AD6C0, s32) = 0;
        }
        if (FI(f, 0x1AD6C0, s32) != 0) {
            for (i = 0; i < 8; i = (i + 1) & 0xFF) {
                if (PursuerGroup_Fields(p, i, (u8)f->c.a.slot) & 0xFF & 0x20) {
                    FI(f, 0x1AD6C0, s32) = 0;
                    break;
                }
            }
        }
        if (FI(f, 0x1AD6C0, s32) != 0) {
            n = nav->numDoors;
            for (i = 0; (s32)i < n; i++) {
                if (RoomSlots_Bytes(p, i & 0xFF, (u8)f->c.a.slot) & 0xFF & 8) {
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
        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, kind != 0xE && kind != 0xC ? 0xA0 : 0x80, 8);
        if (f->c.unk100 != 0xFF && f->c.unk100 != 1) {
            Progress_SetCondBit(p, 1);
        }
        if (st[4] & 0x8000) {
            Fiona_ResetRecovery(f);
        }
        f->c.a.unk2A = 0;
        Actor_SetState(&f->c.a, &D_003B2E68);
        break;
    case 0xB:
        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0xD0, 0xC);
        Progress_SetCondBit(p, 1);
        f->c.a.unk2A = 1;
        f->c.unk104[0] = st[4];
        Actor_SetState(&f->c.a, &D_003B2E58);
        break;
    case 0xA:
        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0xD0, 0xC);
        if (f->c.unk100 != 0xFF && f->c.unk100 != 1) {
            Progress_SetCondBit(p, 1);
        }
        if (st[4] & 0x8000) {
            Fiona_ResetRecovery(f);
        }
        f->c.a.unk2A = 1;
        Actor_SetState(&f->c.a, &D_003B2E48);
        break;
    case 9:
        Progress_SetCondBit(p, 1);
        f->c.a.unk2A = 1;
        FI(f, 0x1AD710, u8) = 1;
        FI(f, 0x1AD714, s32) = 0;
        f->c.unk104[0] = FI(f, 0x1AD6F0, s32);
        FI(f, 0x10C, f32) = FI(f, 0x1AD6F4, f32);
        sceVu0CopyVector(f->c.unk110, (f32 *)((u8 *)f + 0x1AD700));
        f->c.unk104[1] = st[4];
        FI(f, 0x1AD6C8, s32) = 0;
        Actor_PlaySound(&f->c.a, 0x43, 5, 0, 0, NULL);
        Actor_SetState(&f->c.a, f->c.unk104[1] == 6 ? &D_003B2E28 : &D_003B2E38);
        break;
    case 8:
        Progress_SetCondBit(p, 1);
        f->c.a.unk2A = 1;
        Actor_SetState(&f->c.a, &D_003B2E18);
        break;
    case 0x10:
        f->c.a.unk2D = 0;
        Progress_SetFlag(p, 0x2B);
        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0xFF, 0x10);
        f->c.a.unk2A = 0;
        FI(f, 0x1AD6CC, s32) = 0;
        Actor_SetState(&f->c.a, &D_003B2E08);
        break;
    case 0x20:
        if (gSubScreen != NULL && VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 1) == CHARM_ITEM) {
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
                if (NavMesh_TriFlags(gNavMesh, f->c.a.navTri) & 0x80001) {
                    f->c.unk104[0] = 3;
                } else {
                    f32 h = Actor_HeadingTo(&f->c.a, gCharPursuer->a.pos);
                    f32 d;

                    if (!(Angle_Wrap(h - f->c.a.angle[1]) <= 0.0f)) {
                        d = Angle_Wrap(h - f->c.a.angle[1]);
                    } else {
                        d = -Angle_Wrap(h - f->c.a.angle[1]);
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
        } else if ((Actor_TriFreeFor(f, &f->c.a) & 0xFF) == 1) {
            VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0xD0, 0xC);
            f->c.unk100 = 0xFF;
            f->c.a.unk2A = 1;
            Actor_SetState(&f->c.a, &D_003B2DD8);
        } else {
            VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0x80, 8);
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
/* 0x0018BFC0 */
void Fiona_StateHold(Fiona *f) {
    void *m = f->c.motion;

    if (!(AT(m, 0x550, f32) <= 0.0f)) {
        return;
    }
    if (AT(m, 0x55C, s32) != 0x1405 && AT(m, 0x55C, s32) != 0x1406) {
        Motion_PlayTable(m, 0x1405, -1);
    }
    Actor_SetState(&f->c.a, &D_003B2D38);
}

/* 0x13 (D_003B2E88): once the motion has played out, a cry (0x3F) and a fall (noise 0x5F) -
 * where she may (+0x1AD6C0) by the side her character slot's noise point (gProgress +0x1060)
 * is on: ahead 0x1004, behind 0x1005, left / right 0x1007 / 0x1006; else straight down
 * (0x100F) */
/* 0x0018BA70 */
void Fiona_StateFall13(Fiona *f) {
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        Progress *p = gProgress;
        f32 d[4] __attribute__((aligned(16)));
        f32 a, aa;

        sceVu0SubVector(d, f->c.a.pos, (f32 *)((u8 *)p + 0x1060 + f->c.a.slot * 0x20));
        a = Angle_Wrap(func_0031C5C0(d[0], d[2]) - f->c.a.angle[1]);
        aa = a <= 0.0f ? -a : a;
        if (FI(f, 0x1AD6C0, s32) == 0) {
            Actor_PlaySound(&f->c.a, 0x3F, 5, 0, 0, NULL);
            Motion_PlayTable(f->c.motion, 0x100F, -1);
        } else {
            Actor_PlaySound(&f->c.a, 0x3F, 5, 0, 0, NULL);
            if (aa < 0x1.0c1524p+0f /* 60 deg */) {
                Motion_PlayTable(f->c.motion, 0x1004, -1);
            } else if (!(aa <= 0x1.0c1524p+1f /* 120 deg */)) {
                Motion_PlayTable(f->c.motion, 0x1005, -1);
            } else if (a < 0.0f) {
                Motion_PlayTable(f->c.motion, 0x1007, -1);
            } else {
                Motion_PlayTable(f->c.motion, 0x1006, -1);
            }
        }
        Noise_Make((u8 *)p + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
        Actor_SetState(&f->c.a, &D_003B2D48);
    }
    Character_RootMoveMasked(&f->c);
}

extern const PTMF D_003B2998, D_003B29A8;

/* 0x20 caught (D_003B2DF8): once the motion has played out, the rumble, a cry (0x43) and the
 * caught motion by how (+0x104): 1 0xF02, 2 0xF04, 3 0xF03, 4 0xF05 (1, and 3 with someone
 * holding her, set +0x1AD5FC) */
/* 0x00194230 */
void Fiona_StateCaught(Fiona *f) {
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0x80, 8);
        Actor_PlaySound(&f->c.a, 0x43, 5, 0, 0, NULL);
        switch (f->c.unk104[0]) {
        case 4:
            Motion_PlayTable(f->c.motion, 0xF05, -1);
            break;
        case 1:
            FI(f, 0x1AD5FC, u8) = 1;
            Motion_PlayTable(f->c.motion, 0xF02, -1);
            break;
        case 2:
            Motion_PlayTable(f->c.motion, 0xF04, -1);
            break;
        case 3:
            if (f->target != NULL) {
                FI(f, 0x1AD5FC, u8) = 1;
            }
            Motion_PlayTable(f->c.motion, 0xF03, -1);
            break;
        }
        Actor_SetState(&f->c.a, &D_003B2998);
    }
    Character_RootMoveMasked(&f->c);
}

/* 8 (D_003B2E18), caught while crawling: dragged to door +0x104's side 1, 15 out
 * (+0x1AD6E0, +0x1AD6C8 its triangle); a drop under 20 a short pull (0xF02, cry 0x3F, action
 * 0xB), else a fall (0x70A, cry 0x40, +0x1AD6C4); the frames to get there (+0x1AD6C0) falling
 * 0, 0.5, 1, ... a frame, the step across (+0x1AD6D0) */
/* 0x00193E90 */
void Fiona_StateCaughtCrawling(Fiona *f) {
    f32 ofs[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 y, step;
    s32 n;

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    ofs[0] = 0.0f;
    ofs[2] = 10.0f;
    ofs[1] = 0.0f;
    FI(f, 0x1AD6C8, s32) = Actor_DoorFront(f, f->c.unk104[0], 1, ofs, (f32 *)((u8 *)f + 0x1AD6E0));
    sceVu0SubVector(d, f->c.a.pos, (f32 *)((u8 *)f + 0x1AD6E0));
    if (d[1] < 20.0f) {
        FI(f, 0x1AD6C4, s32) = 0;
        f->c.moveMode = 0xB;
        Motion_PlayTable(f->c.motion, 0xF02, -1);
        Actor_PlaySound(&f->c.a, 0x3F, 5, 0, 0, NULL);
    } else {
        FI(f, 0x1AD6C4, s32) = 1;
        Motion_PlayTable(f->c.motion, 0x70A, -1);
        Actor_PlaySound(&f->c.a, 0x40, 5, 0, 0, NULL);
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
/* 0x00183400 */
void Fiona_AlongWall(Fiona *f) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kHalfPi = {0x3FC90FDB}, kMin = {0x3C0EFA35};
    f32 mv[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    NavMesh *nav;
    NavTri *t;
    u32 tri, wall;
    s32 e, e1;
    f32 a, aa, step, yaw;

    VCALL(f->c.motion, 0x60, void (*)(void *, f32 *))(f->c.motion, mv);
    nav = gNavMesh;
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
    if (!(Angle_Wrap(f->savedYaw - f->c.a.angle[1]) <= 0.0f)) {
        aa = Angle_Wrap(f->savedYaw - f->c.a.angle[1]);
    } else {
        aa = -Angle_Wrap(f->savedYaw - f->c.a.angle[1]);
    }
    if (!(aa <= kHalfPi.f)) {
        f->savedYaw = Angle_Wrap(kPi.f + f->savedYaw);
    }
    a = Angle_Wrap(f->savedYaw - f->c.a.angle[1]);
    step = 0.25f * (a <= 0.0f ? -a : a);
    if (step < kMin.f) {
        step = kMin.f;
    }
    if (FI(f, 0x1AD6C0, s32) == -1) {
        FI(f, 0x1AD6C0, s32) = a < 0.0f ? 0 : 1;
    }
    if (FI(f, 0x1AD6C0, s32) != 0) {
        yaw = Angle_Wrap(f->c.a.angle[1] + step);
    } else {
        yaw = Angle_Wrap(f->c.a.angle[1] - step);
    }
    f->c.a.angle[1] = yaw;
    sceVu0UnitMatrix(f->c.a.rot);
    sceVu0RotMatrixY(f->c.a.rot, f->c.a.rot, yaw);
}

/* moved by the motion turned to her heading +0x1AD6D0 (unless +0x1AD6C4 is -1: the plain
 * update) and turned toward it 20 degrees a frame (+0x1AD6C4 set once there) */
static inline __attribute__((always_inline)) void pull_back_move(Fiona *f) {
    if (FI(f, 0x1AD6C4, s32) == -1) {
        Character_RootMoveMasked(&f->c);
    } else {
        f32 d[4] __attribute__((aligned(16)));
        f32 r[4][4] __attribute__((aligned(16)));

        Motion_RootMovement(f->c.motion, d, 0.0f);
        sceVu0UnitMatrix(r);
        sceVu0RotMatrixY(r, r, FI(f, 0x1AD6D0, f32));
        sceVu0ApplyMatrix(d, r, d);
        Actor_Move(&f->c.a, d);
        if (FI(f, 0x1AD6C4, s32) == 0 &&
            Actor_TurnToward(&f->c.a, FI(f, 0x1AD6D0, f32), 0x1.657186p-2f /* 20 deg */) == 0.0f) {
            FI(f, 0x1AD6C4, s32) = 1;
        }
    }
}

/* pulling back from a grab, along any wall */
static inline __attribute__((always_inline)) void pull_back(Fiona *f) {
    pull_back_move(f);
    Fiona_AlongWall(f);
}

/* 0x10 grabbed (D_003B2E08): out of a fall (0x100A / 0xB01 / 0xB02) she gets up (0xB03), out
 * of 0x100D (0x1503) - once that motion lets her (flag 2); else she pulls free: back 0x1101
 * when there is room 17 behind her (+0x1AD6C0 / +0x1AD6C4 -1, her heading kept in +0x1AD6D0),
 * else 0x1100. Pulling back she is moved by the motion turned to that heading (once +0x1AD6C4
 * is set) and turned toward it 20 degrees a frame */
/* 0x00190380 */
void Fiona_StateGrabbed(Fiona *f) {
    void *m;
    s32 cur;
    u8 done = 0, back = 0;

    FI(f, 0x1AD5BC, u8) = 0;
    m = f->c.motion;
    cur = AT(m, 0x55C, s32);
    if (cur == 0x100D) {
        if (!(Motion_EventFlags(m, 0, 0, 1) & 0xFF & 2)) {
            done = 1;
            Motion_PlayTable(f->c.motion, 0x1503, -1);
        }
    } else if (cur == 0x100A || cur == 0xB02 || cur == 0xB01) {
        if (!(Motion_EventFlags(m, 0, 0, 1) & 0xFF & 2)) {
            done = 1;
            Motion_PlayTable(f->c.motion, 0xB03, -1);
        }
    }
    if (done == 0) {
        FI(f, 0x1AD6C0, s32) = -1;
        FI(f, 0x1AD6C4, s32) = -1;
        FI(f, 0x1AD6D0, f32) = f->c.a.angle[1];
        if (!(Actor_TriFreeFor(f, &f->c.a) & 0xFF)) {
            Motion_PlayTable(f->c.motion, 0x1100, -1);
        } else {
            f32 v[4] __attribute__((aligned(16)));

            v[2] = 17.0f;
            v[0] = 0.0f;
            v[1] = 0.0f;
            Mtx_ApplyVector(v, f->c.a.rot, v);
            sceVu0AddVector(v, v, f->c.a.pos);
            if (Actor_TriTo(&f->c.a, v, 0x80001) == (u32)-1) {
                Motion_PlayTable(f->c.motion, 0x1100, -1);
            } else {
                back = 1;
                Motion_PlayTable(f->c.motion, 0x1101, -1);
            }
        }
    }
    if (back == 1) {
        pull_back(f);
    } else {
        Character_RootMoveMasked(&f->c);
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
/* 0x00191800 */
void Fiona_StateThrown(Fiona *f) {
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
        if (!(Angle_Wrap(FI(f, 0x1AD6D0, f32) - f->c.a.angle[1]) <= 0.0f)) {
            aa = Angle_Wrap(FI(f, 0x1AD6D0, f32) - f->c.a.angle[1]);
        } else {
            aa = -Angle_Wrap(FI(f, 0x1AD6D0, f32) - f->c.a.angle[1]);
        }
        if (aa < kHalfPi.f) {
            FI(f, 0x1AD6C8, s32) = 0;
            Motion_PlayTable(f->c.motion, 0x1008, -1);
        } else {
            FI(f, 0x1AD6C8, s32) = 1;
            FI(f, 0x1AD6D0, f32) = Angle_Wrap(kPi.f + FI(f, 0x1AD6D0, f32));
            if (FI(f, 0x1AD584, s32) & 2) {
                FI(f, 0x1AD6C4, s32) = -1;
                f->c.a.angle[1] = FI(f, 0x1AD6D0, f32);
                Motion_PlayTable(f->c.motion, 0xB04, -1);
            } else {
                Motion_PlayTable(f->c.motion, 0x100B, -1);
            }
        }
        if (FI(f, 0x1AD584, s32) & 2) {
            Progress *p = gProgress;

            AT(p, 0x7D8, f32) = 1000.0f;
            Actor_PlaySound(&f->c.a, 0x42, 5, 0, 0, NULL);
            Noise_Make((u8 *)p + 0x778, 0x6F, f->c.a.room, f->c.a.navTri, 0xFFFF);
        } else {
            Actor_PlaySound(&f->c.a, 0x40, 5, 0, 0, NULL);
            Noise_Make((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
        }
        FI(f, 0x1AD6C0, s32) = -1;
        Actor_SetState(&f->c.a, &D_003B2AB8);
    }
    f->c.a.navMask |= 1;
    Character_RootMoveMasked(&f->c);
    f->c.a.navMask &= ~1;
}

extern const PTMF D_003B2B08;

/* 0xB hit by door [4] (+0x104; D_003B2E58): thrown along the door's swing (its angle, turned
 * round from her side of it) - forward (0x1008) or backward (0x100B; 0xB04 facing back for a
 * hard fall, +0x1AD584 bit 2, with a scream 0x42, noise 0x6F and the panic at 1000), else a cry
 * 0x40 and noise 0x5F; +0x1AD6C8 which way, +0x1AD6D0 the direction */
/* 0x00190FA0 */
void Fiona_StateHitByDoor(Fiona *f) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kHalfPi = {0x3FC90FDB};
    Progress *p;
    f32 a, aa;

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    a = VCALL(gDoors, 0x3C, f32 (*)(VObject *, u32))(gDoors, (u8)f->c.unk104[0]);
    p = gProgress;
    if (!(PursuerGroup_Fields(p, (u8)f->c.unk104[0], (u8)f->c.a.slot) & 0xFF & 0x10)) {
        a = Angle_Wrap(kPi.f + a);
    }
    FI(f, 0x1AD6C4, s32) = 0;
    if (!(Angle_Wrap(a - f->c.a.angle[1]) <= 0.0f)) {
        aa = Angle_Wrap(a - f->c.a.angle[1]);
    } else {
        aa = -Angle_Wrap(a - f->c.a.angle[1]);
    }
    if (aa < kHalfPi.f) {
        FI(f, 0x1AD6C8, s32) = 0;
        FI(f, 0x1AD6D0, f32) = a;
        Motion_Play(f->c.motion, 0x1008, -1);
    } else {
        FI(f, 0x1AD6C8, s32) = 1;
        FI(f, 0x1AD6D0, f32) = Angle_Wrap(kPi.f + a);
        if (FI(f, 0x1AD584, s32) & 2) {
            FI(f, 0x1AD6C4, s32) = -1;
            f->c.a.angle[1] = FI(f, 0x1AD6D0, f32);
            Motion_Play(f->c.motion, 0xB04, -1);
        } else {
            Motion_Play(f->c.motion, 0x100B, -1);
        }
    }
    FI(f, 0x1AD6C0, s32) = -1;
    f->c.a.navMask |= 1;
    Character_RootMoveMasked(&f->c);
    f->c.a.navMask &= ~1;
    if (FI(f, 0x1AD584, s32) & 2) {
        Progress *q = gProgress;

        AT(q, 0x7D8, f32) = 1000.0f;
        Actor_PlaySound(&f->c.a, 0x42, 5, 0, 0, NULL);
        Noise_Make((u8 *)q + 0x778, 0x6F, f->c.a.room, f->c.a.navTri, 0xFFFF);
    } else {
        Actor_PlaySound(&f->c.a, 0x40, 5, 0, 0, NULL);
        Noise_Make((u8 *)p + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
    }
    Actor_SetState(&f->c.a, &D_003B2B08);
}

extern const PTMF D_003B2B18;

/* the fall by the side the hit came from (`aa` its size, `a` signed): ahead `front`, behind
 * `front` + 1, left / right `front` + 3 / + 2 */
static inline __attribute__((always_inline)) void fall_by_side(Fiona *f, f32 aa, f32 a, s32 front) {
    if (aa < 0x1.0c1524p+0f /* 60 deg */) {
        Motion_PlayTable(f->c.motion, front, -1);
    } else if (!(aa <= 0x1.0c1524p+1f /* 120 deg */)) {
        Motion_PlayTable(f->c.motion, front + 1, -1);
    } else if (a < 0.0f) {
        Motion_PlayTable(f->c.motion, front + 3, -1);
    } else {
        Motion_PlayTable(f->c.motion, front + 2, -1);
    }
}

/* 0xC..0xF knocked down (D_003B2E68, D_003B2DE8): once the motion has played out, from the side
 * of whoever did it ([2] +0x100, 0xFF: her slot's noise point gProgress +0x1060) - 0xC /
 * 0xE a stumble (cry 0x3E; 0xC on the spot 0x100E, 0xE by side 0x1000..), 0xD / 0xF a fall
 * (cry 0x3F; 0xD 0x100F, 0xF by side 0x1004..), each with noise 0x5F */
/* 0x00190B50 */
void Fiona_StateKnockedDown(Fiona *f) {
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
            a = Angle_Wrap(func_0031C5C0(d[0], d[2]) - f->c.a.angle[1]);
            aa = a <= 0.0f ? -a : a;
        } else {
            a = f->c.a.angle[1];
            aa = 0.0f;
        }
        switch (f->c.moveSub) {
        case 0xC:
            Actor_PlaySound(&f->c.a, 0x3E, 5, 0, 0, NULL);
            Motion_PlayTable(f->c.motion, 0x100E, -1);
            Noise_Make((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
            break;
        case 0xE:
            Actor_PlaySound(&f->c.a, 0x3E, 5, 0, 0, NULL);
            fall_by_side(f, aa, a, 0x1000);
            Noise_Make((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
            break;
        case 0xD:
            Actor_PlaySound(&f->c.a, 0x3F, 5, 0, 0, NULL);
            Motion_PlayTable(f->c.motion, 0x100F, -1);
            Noise_Make((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
            break;
        case 0xF:
            Actor_PlaySound(&f->c.a, 0x3F, 5, 0, 0, NULL);
            fall_by_side(f, aa, a, 0x1004);
            Noise_Make((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
            break;
        }
        Actor_SetState(&f->c.a, &D_003B2B18);
    }
    Character_RootMoveMasked(&f->c);
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
    r = Character_PlanPathKind(&f->c, 0, f->c.unk104[0], f->c.unk110);
    if (r > 0) {
        r = Character_WaypointsCurve(&f->c);
    }
    if (r > 0) {
        f32 a;

        FI(f, 0x1AD6D0, f32) = 0x1.99999ap-3f /* 0.2 */ *
            VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
                gSceneGameF29740, f->c.a.pos, f->c.unk128, f->c.unk124, f->c.unk12C);
        Motion_PlayOwnBlend(f->c.motion, anim, -1);
        FI(f, 0x1AD6D8, f32) = FI(f, 0x10C, f32);
        a = Angle_Wrap(FI(f, 0x10C, f32) - f->c.a.angle[1]);
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
/* 0x00191FF0 */
void Fiona_StateLedWalking(Fiona *f) {
    led_away(f, 0x1500, &D_003B2A98);
}

/* 9 led away by the hand ([4] 6; D_003B2E28) */
/* 0x00193400 */
void Fiona_StateLedByHand(Fiona *f) {
    led_away(f, 0x1400, &D_003B29F8);
}

/* ---- what follows the reactions ---- */

extern const PTMF D_003B2D58;

/* after the 0x13 fall (D_003B2D48): at the motion's event 0x400 the threat meter rises by 75, on to
 * D_003B2D58 */
/* 0x0018B9D0 */
void Fiona_StateAfterFall(Fiona *f) {
    if ((MOTION_EVENTS(f->c.motion) & 0x400) != 0) {
        Threat_Raise((u8 *)gProgress + 0x7B8, 75.0f);
        Actor_SetState(&f->c.a, &D_003B2D58);
    }
    Character_RootMoveMasked(&f->c);
}

/* at the motion's event 0x20 she stands (+0x1AD5EC 30) */
static inline __attribute__((always_inline)) void stand_up(Fiona *f) {
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        FI(f, 0x1AD5EC, s32) = 0x1E;
        f->c.a.unk2D = 0;
        Fiona_ToIdle(f);
    }
    Character_RootMoveMasked(&f->c);
}

/* after being knocked down (D_003B2B18) */
/* 0x00190A00 */
void Fiona_StateAfterKnockDown(Fiona *f) {
    stand_up(f);
}

/* after the 0x13 fall, on the ground (D_003B2D58) */
/* 0x0018B880 */
void Fiona_StateOnGround(Fiona *f) {
    stand_up(f);
}

/* after being caught (D_003B2998): at the motion's event 0x20 she stands; until then, caught
 * from in front or behind (+0x104 1 / 3) by someone, +0x1AD5FC stays set */
/* 0x001940A0 */
void Fiona_StateAfterCaught(Fiona *f) {
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        Fiona_ToIdle(f);
    } else if ((f->c.unk104[0] == 3 || f->c.unk104[0] == 1) && f->target != NULL) {
        FI(f, 0x1AD5FC, u8) = 1;
    }
    Character_RootMoveMasked(&f->c);
}

extern const PTMF D_003B29B8, D_003B29C8;

/* held after a grab (D_003B2B48): a cry (0x41) the first time (+0x1AD6CC); at the motion's
 * event 0x20 (once, +0x1AD6C8) - unless progress flag 0x2C - the game-over flag 0xC (and
 * gProgress +0x73EB00 set); pulling back (0x1101) as she was, else the plain update */
/* 0x00190190 */
void Fiona_StateHeld(Fiona *f) {
    if (FI(f, 0x1AD6CC, s32) == 0) {
        FI(f, 0x1AD6CC, s32) = 1;
        Actor_PlaySound(&f->c.a, 0x41, 5, 0, 0, NULL);
    }
    FI(f, 0x1AD5BC, u8) = 0;
    if (FI(f, 0x1AD6C8, s32) == 0 && (MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        FI(f, 0x1AD6C8, s32) = 1;
        if (!(Progress_TestFlag(gProgress, 0x2C) & 0xFF)) {
            Progress *p = gProgress;

            AT(p, 0x73EB00, u8) = 1;
            Progress_SetFlag(p, 0xC);
        }
    }
    if (AT(f->c.motion, 0x55C, s32) == 0x1101) {
        pull_back(f);
    } else {
        Character_RootMoveMasked(&f->c);
    }
}

/* the drop after a crawl-catch (D_003B29A8): +0x1AD6C0 frames down by the step (+0x1AD6D0,
 * falling 0.5 a frame faster; a long fall turns to 0x70B at the motion's event 0x20); then
 * on the spot (+0x1AD6E0, triangle +0x1AD6C8) with the rumble and a cry - 0x7D (a short pull)
 * or 0x81 (a fall) */
/* 0x00193C60 */
void Fiona_StateCrawlDrop(Fiona *f) {
    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    FI(f, 0x1AD6C0, s32) = FI(f, 0x1AD6C0, s32) - 1;
    if (FI(f, 0x1AD6C0, s32) != 0) {
        if (FI(f, 0x1AD6C4, s32) != 0 && AT(f->c.motion, 0x55C, s32) != 0x70B &&
            (MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
            Motion_PlayTable(f->c.motion, 0x70B, -1);
        }
        f->c.a.pos[0] = f->c.a.pos[0] - FI(f, 0x1AD6D0, f32);
        f->c.a.pos[1] = f->c.a.pos[1] - FI(f, 0x1AD6D4, f32);
        f->c.a.pos[2] = f->c.a.pos[2] - FI(f, 0x1AD6D8, f32);
        FI(f, 0x1AD6D4, f32) = FI(f, 0x1AD6D4, f32) + 0.5f;
        return;
    }
    f->c.a.unk2A = 0;
    f->c.a.navTri = FI(f, 0x1AD6C8, u32);
    sceVu0CopyVector(f->c.a.pos, (f32 *)((u8 *)f + 0x1AD6E0));
    VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0xD0, 0xC);
    if (FI(f, 0x1AD6C4, s32) == 0) {
        Fiona_Voice(f, 0x7D, 5, 0, 0);
        Actor_SetState(&f->c.a, &D_003B29B8);
    } else {
        Fiona_Voice(f, 0x81, 5, 0, 0);
        Actor_SetState(&f->c.a, &D_003B29C8);
    }
}

extern u32 D_0047E37C;   /* pad buttons pressed this frame */

/* struggling: how many shakes this frame - the stick (and d-pad) swung through more than 120
 * degrees from the last direction (+0x1AD714), or out from rest (+0x1AD710: back under 0.2),
 * and each of the buttons bits 10..15 pressed; none while the game drives her */
/* 0x00183190 */
s32 Fiona_Shakes(Fiona *f) {
    static const union { u32 u; f32 f; } k08 = {0x3F4CCCCD}, k02 = {0x3E4CCCCD}, k120 = {0x40060A92};
    f32 e[4] __attribute__((aligned(16)));
    s32 n = 0;
    f32 len;

    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        return 0;
    }
    sceVu0CopyVector(e, D_0047E3A0);
    e[0] = e[0] + (f32)(s32)(((D_0047E374 >> 5) & 1) - ((D_0047E374 >> 7) & 1));
    e[2] = e[2] + (f32)(s32)(((D_0047E374 >> 6) & 1) - ((D_0047E374 >> 4) & 1));
    len = __builtin_sqrtf(e[2] * e[2] + e[0] * e[0]);
    if (len <= k08.f) {
        if (len < k02.f) {
            FI(f, 0x1AD710, u8) = 1;
            FI(f, 0x1AD714, f32) = 0.0f;
        }
    } else {
        f32 a = func_0031C5C0(e[0], e[2]);

        if (FI(f, 0x1AD710, u8) != 0) {
            n++;
            FI(f, 0x1AD710, u8) = 0;
            FI(f, 0x1AD714, f32) = a;
        } else {
            f32 d;

            if (!(Angle_Wrap(FI(f, 0x1AD714, f32) - a) <= 0.0f)) {
                d = Angle_Wrap(FI(f, 0x1AD714, f32) - a);
            } else {
                d = -Angle_Wrap(FI(f, 0x1AD714, f32) - a);
            }
            if (!(d <= k120.f)) {
                n++;
                FI(f, 0x1AD714, f32) = a;
            }
        }
    }
    if ((D_0047E37C >> 14) & 1 || (D_0047E37C >> 13) & 1 || (D_0047E37C >> 12) & 1 || (D_0047E37C >> 15) & 1 ||
        (D_0047E37C >> 10) & 1 || (D_0047E37C >> 11) & 1) {
        n++;
    }
    return n;
}

/* held (D_003B2D38): she struggles - each shake takes a frame off both +0x1AD6C0 (the hold)
 * and +0x1AD6C4 (beyond one a frame); the hold (0x1405, then 0x1406 at the motion's event 0x20)
 * lasts until +0x1AD6C0 runs out or no carrier (creatures 7..9 in her room, action 8) is
 * left, then she gets up (0x1407; standing at its event 0x20) */
/* 0x0018BCC0 */
void Fiona_StateStruggle(Fiona *f) {
    void *m;

    FI(f, 0x1AD6C0, s32) = FI(f, 0x1AD6C0, s32) - 1;
    if (FI(f, 0x1AD6C0, s32) > 0 && FI(f, 0x1AD6C4, s32) > 0) {
        s32 k = Fiona_Shakes(f);

        FI(f, 0x1AD6C0, s32) = FI(f, 0x1AD6C0, s32) - k;
        FI(f, 0x1AD6C4, s32) = FI(f, 0x1AD6C4, s32) - k;
    }
    m = f->c.motion;
    switch (AT(m, 0x55C, s32)) {
    case 0x1407:
        if ((MOTION_EVENTS(m) & 0x20) != 0) {
            f->c.a.unk2D = 0;
            Fiona_ToIdle(f);
        }
        break;
    case 0x1406:
        if (AT(m, 0x550, f32) <= 0.0f) {
            u8 held = 0;

            if (FI(f, 0x1AD6C0, s32) > 0) {
                s32 i;

                for (i = 7; i < 10; i++) {
                    Character *c = AT(gCreatures, i * 4, Character *);

                    if (c != NULL && c->a.active == 1 && f->c.a.room == c->a.room && c->moveMode == 8) {
                        held = 1;
                        break;
                    }
                }
            }
            if (held == 0) {
                f->c.a.unk2D = 1;
                f->c.moveMode = 0;
                Motion_Play(f->c.motion, 0x1407, -1);
            }
        }
        break;
    case 0x1405:
        if ((MOTION_EVENTS(m) & 0x20) != 0) {
            Motion_Play(m, 0x1406, -1);
        }
        break;
    }
}

extern const PTMF D_003B2AC8, D_003B2AD8, D_003B2AE8, D_003B2AF8;

/* down after a hard fall: on the ground (0xB01), action 0xA / 0xB, the struggle reset */
static inline __attribute__((always_inline)) void floored(Fiona *f, const PTMF *next) {
    f->c.a.unk2D = 0;
    FI(f, 0x1AD6C0, s32) = 0;
    Motion_PlayTable(f->c.motion, 0xB01, -1);
    f->c.moveMode = 0xA;
    f->unk1AD580 = 0xB;
    FI(f, 0x1AD710, u8) = 1;
    FI(f, 0x1AD714, s32) = 0;
    Actor_SetState(&f->c.a, next);
}

/* after being thrown or hit by a door (D_003B2AB8, D_003B2B08): at the motion's event 0x20 -
 * fallen forward (0x1008) she gets up (0x100A), backward (0x100B) 0x100D; a hard fall (bit 2,
 * or 0xB04) leaves her on the ground. Meanwhile she slides with the fall (+0x1AD6D0; nav flag 1
 * not blocking, the doorway flags too while down by a door, action 0xB) and along walls - not
 * while down (0xA) at a door she can pass (exit bit 1) */
/* 0x001913F0 */
void Fiona_StateAfterThrow(Fiona *f) {
    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        s32 cur = AT(f->c.motion, 0x55C, s32);

        if (FI(f, 0x1AD6C8, s32) == 0) {
            if (cur == 0x1008) {
                if (!(FI(f, 0x1AD584, s32) & 2)) {
                    f->c.a.unk2D = 1;
                    Motion_Play(f->c.motion, 0x100A, -1);
                    Actor_SetState(&f->c.a, &D_003B2AC8);
                } else {
                    floored(f, &D_003B2AD8);
                }
            }
        } else if (cur == 0xB04) {
            floored(f, &D_003B2AF8);
        } else if (cur == 0x100B) {
            f->c.a.unk2D = 1;
            Motion_Play(f->c.motion, 0x100D, -1);
            Actor_SetState(&f->c.a, &D_003B2AE8);
        }
    }
    if (f->c.moveSub == 0xB) {
        f->c.a.navMask = 0x8000018;
    }
    f->c.a.navMask |= 1;
    pull_back_move(f);
    f->c.a.navMask &= ~1;
    if (f->c.moveSub == 0xA) {
        Progress *p = gProgress;
        u32 i;

        for (i = 0; i < 8; i = (i + 1) & 0xFF) {
            if (PursuerGroup_Fields(p, i, (u8)f->c.a.slot) & 0xFF & 1) {
                return;
            }
        }
    }
    Fiona_AlongWall(f);
}

extern const PTMF D_003B29D8;

/* after a crawl-catch fall (D_003B29C8): once the motion has played out, 0x70C on to
 * D_003B29D8 */
/* 0x00193BB0 */
void Fiona_StateCrawlFall(Fiona *f) {
    FI(f, 0x1AD5BC, u8) = 0;
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        Motion_PlayTable(f->c.motion, 0x70C, -1);
        Actor_SetState(&f->c.a, &D_003B29D8);
    }
}

/* getting up: standing at the motion's event 0x20; her update with nav flag 1 not blocking */
static inline __attribute__((always_inline)) void getting_up(Fiona *f, u8 busy) {
    f->c.a.unk2A = busy;
    FI(f, 0x1AD5BC, u8) = 0;
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        f->c.a.unk2D = 0;
        Fiona_ToIdle(f);
    }
    f->c.a.navMask |= 1;
    Character_RootMoveMasked(&f->c);
    f->c.a.navMask &= ~1;
}

/* getting up after a throw (D_003B2AC8, D_003B2AE8) */
/* 0x00191280 */
void Fiona_StateGetUp(Fiona *f) {
    getting_up(f, 0);
}

extern const PTMF D_003B29E8;

/* after the crawl-catch short pull (D_003B29B8): at the motion's event 0x20 she stands, out
 * of the door region she is in (RoomSlots_Leave), first Fiona_ResetRecovery after a fall */
/* 0x00193900 */
void Fiona_StateCrawlPull(Fiona *f) {
    FI(f, 0x1AD5BC, u8) = 0;
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        u32 n = gNavMesh->numDoors;
        u32 i = 0;

        if (n != 0) {
            Progress *p = gProgress;

            for (i = 0; i < n; i++) {
                if (RoomSlots_Bytes(p, i & 0xFF, (u8)f->c.a.slot) & 0xFF & 1) {
                    break;
                }
            }
        }
        if (i != n) {
            RoomSlots_Leave(gProgress, i & 0xFF, (u8)f->c.a.slot);
        }
        f->c.a.unk2D = 0;
        if (FI(f, 0x1AD6C4, s32) != 0) {
            Fiona_ResetRecovery(f);
        }
        Fiona_ToIdle(f);
    }
    Character_RootMoveMasked(&f->c);
}

/* after the crawl-catch fall, down (D_003B29D8): at the motion's event 0x20 she gets up
 * (0x100D) */
/* 0x00193B10 */
void Fiona_StateCrawlDown(Fiona *f) {
    FI(f, 0x1AD5BC, u8) = 0;
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        Motion_Play(f->c.motion, 0x100D, -1);
        Actor_SetState(&f->c.a, &D_003B29E8);
    }
    Character_RootMoveMasked(&f->c);
}

extern const PTMF D_003B2AA8;

/* a step led along: turned to +0x1AD6D8 (by +0x1AD6DC) and on along the path by +0x1AD6D0 */
static inline __attribute__((always_inline)) void led_step(Fiona *f) {
    f32 at[4] __attribute__((aligned(16)));
    u32 tri;

    Actor_TurnToward(&f->c.a, FI(f, 0x1AD6D8, f32), FI(f, 0x1AD6DC, f32));
    tri = f->c.a.navTri;
    f->c.unk128 = Character_WaypointAhead(&f->c, &tri, at, FI(f, 0x1AD6D0, f32));
    f->c.a.navTri = tri;
    sceVu0CopyVector(f->c.a.pos, at);
}

/* at the spot (+0x104 / +0x110, on the floor) facing +0x1AD6D8 */
static inline __attribute__((always_inline)) void led_arrive(Fiona *f) {
    f32 yaw;

    f->c.a.navTri = f->c.unk104[0];
    sceVu0CopyVector(f->c.a.pos, f->c.unk110);
    VCALL((VObject *)gNavMesh, 0x14, void (*)(VObject *, u32, f32 *))((VObject *)gNavMesh, f->c.a.navTri, f->c.a.pos);
    yaw = FI(f, 0x1AD6D8, f32);
    f->c.a.angle[1] = yaw;
    sceVu0UnitMatrix(f->c.a.rot);
    sceVu0RotMatrixY(f->c.a.rot, f->c.a.rot, yaw);
}

/* led away walking (D_003B2A98): while the motion plays she turns to
 * +0x1AD6D8 (by +0x1AD6DC) and goes along the path by +0x1AD6D0 a frame; then she is at the
 * spot (+0x104 / +0x110, on the floor) facing +0x1AD6D8, progress flag 0x2B set unless the game
 * drives her; her leader gone, she stands */
/* 0x00191D10 */
void Fiona_StateLedAway(Fiona *f) {
    Character *c;

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    c = gCharacters[f->c.unk100];
    if (c == NULL || c->a.active == 0 || c->a.disabled == 1) {
        Fiona_ToIdle(f);
        return;
    }
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        led_arrive(f);
        FI(f, 0x1AD6C0, s32) = 0;
        FI(f, 0x1AD6C8, s32) = 0;
        if (AT(gProgress, 0x1FBEC1, u8) == 0) {
            Progress_SetFlag(gProgress, 0x2B);
        }
        Actor_SetState(&f->c.a, &D_003B2AA8);
    } else {
        led_step(f);
    }
}

extern const PTMF D_003B2A08;

/* led away by the hand (D_003B29F8): as Fiona_StateLedAway, but only while her leader leads
 * (action 8); the pull rumbles (motion flag 2), and when the game drives her she may cry out
 * (0x38, 1 in 4) at the spot */
/* 0x00192F70 */
void Fiona_StateLedHand(Fiona *f) {
    Character *c;

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
    if (Motion_EventFlags(f->c.motion, 0, 0, 1) & 0xFF & 2) {
        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0x60, 8);
    }
    if (AT(f->c.motion, 0x550, f32) <= 0.0f) {
        if (AT(gProgress, 0x1FBEC1, u8) == 1 &&
            VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 0.25f) {
            Actor_PlaySound(&f->c.a, 0x38, 5, 0, 0, NULL);
        }
        led_arrive(f);
        FI(f, 0x1AD6C0, s32) = 0;
        FI(f, 0x1AD6C8, s32) = 0;
        FI(f, 0x1AD6CC, s32) = 0;
        Actor_SetState(&f->c.a, &D_003B2A08);
    } else {
        led_step(f);
    }
}

extern const PTMF D_003B2A18, D_003B2A28;

/* her leader [2] still there: in usable shape, else she stands */
static inline __attribute__((always_inline)) Character *led_by(Fiona *f) {
    Character *c = gCharacters[f->c.unk100];

    if (c == NULL || c->a.active == 0 || c->a.disabled == 1) {
        Fiona_ToIdle(f);
        return NULL;
    }
    return c;
}

/* led away, at the spot (D_003B2AA8): at the motion's event 0x20 - unless progress flag 0x2C -
 * the game-over flag 0xC (and gProgress +0x73EB00 set) */
/* 0x00191B40 */
void Fiona_StateLedSpot(Fiona *f) {
    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    if (led_by(f) == NULL) {
        return;
    }
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0 && !(Progress_TestFlag(gProgress, 0x2C) & 0xFF)) {
        Progress *p = gProgress;

        AT(p, 0x73EB00, u8) = 1;
        Progress_SetFlag(p, 0xC);
    }
    Character_RootMoveMasked(&f->c);
}

/* led by the hand, at the spot (D_003B2A08): her leader no longer leading, she pulls free
 * (0xF02, D_003B2A18); still led, at the motion's event 0x20 0x1401 (D_003B2A28); the pull
 * rumbles (motion flag 2) */
/* 0x00192CE0 */
void Fiona_StateHandSpot(Fiona *f) {
    Character *c;

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    c = led_by(f);
    if (c == NULL) {
        return;
    }
    if (c->moveMode != 8) {
        Motion_PlayTable(f->c.motion, 0xF02, -1);
        Actor_SetState(&f->c.a, &D_003B2A18);
    } else if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        Motion_Play(f->c.motion, 0x1401, -1);
        Actor_SetState(&f->c.a, &D_003B2A28);
    }
    if (Motion_EventFlags(f->c.motion, 0, 0, 1) & 0xFF & 2) {
        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0x60, 8);
    }
    Character_RootMoveMasked(&f->c);
}

/* pulled free of the hand that led her (D_003B2A18) */
/* 0x00192690 */
void Fiona_StatePulledFree(Fiona *f) {
    getting_up(f, 1);
}

extern const PTMF D_003B2A38, D_003B2A48, D_003B2A58, D_003B2A68;
extern s8 D_0047A910[];   /* shakes needed to break free, by the threat meter's level */

/* dragged by the hand (D_003B2A28): she struggles - shakes (Fiona_Shakes) add to +0x1AD6C8,
 * capped at 10 per pull so far (+0x1AD6CC); enough for the threat level (D_0047A910, or when
 * the game drives her, as many pulls as the level) and she breaks free (+0x1AD6C8 -1,
 * +0x1AD6C0 set, the leader told to stop: state 7). Each pull (0x1401, at the motion's event
 * 0x20) may draw a cry when the game drives her; free, she slips out (0x1403); the sixth pull
 * drags her off (0x1404). Her leader gone or not leading, she pulls free (0xF02) */
/* 0x00192800 */
void Fiona_StateDragged(Fiona *f) {
    Character *c;

    f->c.a.unk2A = 1;
    if (Motion_EventFlags(f->c.motion, 0, 0, 1) & 0xFF & 2) {
        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0x60, 8);
    }
    FI(f, 0x1AD5BC, u8) = 0;
    c = gCharacters[f->c.unk100];
    if (c == NULL || c->a.active == 0 || c->a.disabled == 1) {
        Motion_PlayTable(f->c.motion, 0xF02, -1);
        Actor_SetState(&f->c.a, &D_003B2A38);
        return;
    }
    if (FI(f, 0x1AD6C8, s32) != -1) {
        s32 lim;

        FI(f, 0x1AD6C8, s32) = FI(f, 0x1AD6C8, s32) + Fiona_Shakes(f);
        lim = (FI(f, 0x1AD6CC, s32) + 1) * 10;
        if (lim < FI(f, 0x1AD6C8, s32)) {
            FI(f, 0x1AD6C8, s32) = lim;
        }
    }
    if (c->moveMode != 8) {
        Motion_PlayTable(f->c.motion, 0xF02, -1);
        Actor_SetState(&f->c.a, &D_003B2A48);
    } else if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        if (FI(f, 0x1AD6C0, s32) != 0) {
            Motion_Play(f->c.motion, 0x1403, -1);
            Actor_SetState(&f->c.a, &D_003B2A58);
        } else {
            FI(f, 0x1AD6CC, s32) = FI(f, 0x1AD6CC, s32) + 1;
            if (FI(f, 0x1AD6CC, s32) == 6) {
                if (AT(gProgress, 0x1FBEC1, u8) == 0) {
                    Progress_SetFlag(gProgress, 0x2B);
                }
                Motion_Play(f->c.motion, 0x1404, -1);
                Actor_SetState(&f->c.a, &D_003B2A68);
            } else {
                Motion_Play(f->c.motion, 0x1401, -1);
                if (AT(gProgress, 0x1FBEC1, u8) == 1 &&
                    VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 0.25f) {
                    Actor_PlaySound(&f->c.a, 0x38, 5, 0, 0, NULL);
                }
            }
        }
    } else if (FI(f, 0x1AD6C8, s32) != -1) {
        Progress *p = gProgress;

        if (AT(p, 0x1FBEC1, u8) == 0) {
            if (!(FI(f, 0x1AD6C8, s32) < D_0047A910[AT(p, 0x7B8, u8)])) {
                FI(f, 0x1AD6C8, s32) = -1;
            }
        } else if (FI(f, 0x1AD6CC, s32) == AT(p, 0x7B8, u8)) {
            FI(f, 0x1AD6C8, s32) = -1;
        }
        if (FI(f, 0x1AD6C8, s32) == -1 && FI(f, 0x1AD6C0, s32) == 0) {
            FI(f, 0x1AD6C0, s32) = 1;
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
        }
    }
    Character_RootMoveMasked(&f->c);
}

extern const PTMF D_003B2A78, D_003B2A88;

/* dragged off (D_003B2A68): at the end of 0x1404 (its event 0x20) - unless progress flag 0x2C -
 * the game-over flag 0xC (and gProgress +0x73EB00 set); her leader gone or not leading, she
 * pulls free (0xF02) */
/* 0x001924F0 */
void Fiona_StateDraggedOff(Fiona *f) {
    Character *c;

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    c = gCharacters[f->c.unk100];
    if (c == NULL || c->a.active == 0 || c->a.disabled == 1) {
        Motion_PlayTable(f->c.motion, 0xF02, -1);
        Actor_SetState(&f->c.a, &D_003B2A78);
        return;
    }
    if (c->moveMode != 8) {
        Motion_PlayTable(f->c.motion, 0xF02, -1);
        Actor_SetState(&f->c.a, &D_003B2A88);
    } else if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0 && AT(f->c.motion, 0x55C, s32) == 0x1404 &&
               !(Progress_TestFlag(gProgress, 0x2C) & 0xFF)) {
        Progress *p = gProgress;

        AT(p, 0x73EB00, u8) = 1;
        Progress_SetFlag(p, 0xC);
    }
    Character_RootMoveMasked(&f->c);
}

/* ---- going through a door (moveSub 0x14 in, 0x15 out; the door in unk100, unk104[0] 0 a
 * single door / else double): the door animation (0x600 + kind, FI 0x1AD6CC, kind at FI
 * 0x1AD6C8: in 0 / 2 / 4 / 6, out 1 / 3 / 5 / 7, kinds 4..7 the slow ones when she can't
 * hurry - FI 0x1AD584 bit 2 or the game mode 2), a spot by the door (FI 0x1AD640, its nav tri
 * FI 0x1AD634) to walk to first ---- */

extern const PTMF D_003B28B8, D_003B28C8, D_003B28D8, D_003B28E8;

/* the door's spot for `kind`: position into pos, facing (y) into dir[1]; its nav tri, or -1 */
static inline s32 door_spot(Fiona *f, f32 *pos, f32 *dir) {
    return VCALL(gDoors, 0x14, s32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(
        gDoors, *(u8 *)&f->c.unk100, FI(f, 0x1AD6C8, s32), pos, dir, 0);
}

/* walk to the spot (tri, at; facing yaw) and then go on in `next` */
static inline __attribute__((always_inline)) void door_walk(Fiona *f, s32 tri, f32 *at, f32 yaw,
                                                            const PTMF *next) {
    f->c.unk124 = f->c.unk128;
    FI(f, 0x1AD650, s32) = 0;
    FI(f, 0x1AD634, s32) = tri;
    sceVu0CopyVector((f32 *)((u8 *)f + 0x1AD640), at);
    VCALL(gNavMesh, 0x14, void (*)(NavMesh *, s32, f32 *))(gNavMesh, tri, (f32 *)((u8 *)f + 0x1AD640));
    f->savedYaw = yaw;
    Actor_SetState(&f->c.a, next);
}

static inline void door_kind(Fiona *f, s32 kind, s32 anim) {
    FI(f, 0x1AD6C8, s32) = kind;
    FI(f, 0x1AD6CC, s32) = anim;
}

/* state D_003B26C8 / D_003B26D8 / D_003B2608 (and 0x2E98): start. Out (0x15): to the spot
 * (D_003B28E8); no spot: idle, the door shut behind (doors +0x20 / +0x1C, DoorHold_Shut). In
 * (0x14) through a door that isn't locked (DoorHold_Usable != 1): the same, opening it the other
 * way (DoorHold_Open). A locked one: a try at it - slow: from where she is when hurrying is
 * blocked (D_003B28B8), else from the spot (D_003B28C8); quick: from the spot (D_003B28D8) */
/* 0x00197640 */
void Fiona_StateDoorStart(Fiona *f) {
    f32 at[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    u8 quick = 1;
    s32 tri;

    if ((FI(f, 0x1AD584, s32) & 2) || (Progress_GameMode(gProgress) & 0xFF) == 2) {
        quick = 0;
    }
    if (f->c.moveSub != 0x14) {
        if (f->c.unk104[0] == 0) {
            if (quick == 1) {
                door_kind(f, 1, 0x601);
            } else {
                door_kind(f, 5, 0x605);
            }
        } else if (quick == 1) {
            door_kind(f, 3, 0x603);
        } else {
            door_kind(f, 7, 0x607);
        }
        FI(f, 0x1AD6C0, s32) = door_spot(f, at, dir);
        if (FI(f, 0x1AD6C0, s32) == -1) {
            VObject *doors;
            Progress *p;

            f->unk1AD580 = 0;
            f->c.moveMode = 0;
            f->savedYaw = f->c.a.angle[1];
            f->unk1AD5C0 = 0;
            f->unk1AD588 = 0;
            if (f->c.unkE0 == 0) {
                f->c.a.unk2D = 0;
                if (*(s32 *)((u8 *)f->c.motion + 0x4C4) != 0) {
                    Fiona_IdleAnim(f, -1);
                }
                Actor_SetState(&f->c.a, &D_003B25A8);
            } else {
                Actor_SetState(&f->c.a, &D_003B25B8);
            }
            p = gProgress;
            Progress_ClearFlag(p, 0x2B);
            doors = gDoors;
            VCALL(doors, 0x20, void (*)(VObject *, u32, s32, s32))(doors, *(u8 *)&f->c.unk100, 1, 0x60000);
            VCALL(doors, 0x1C, void (*)(VObject *, u32, s32, s32))(doors, *(u8 *)&f->c.unk100, 0, 0x60000);
            DoorHold_Shut(p, f->c.a.room, *(u8 *)&f->c.unk100, *(u8 *)&f->c.a.slot);
            return;
        }
        tri = FI(f, 0x1AD6C0, s32);
        door_walk(f, tri, at, dir[1], &D_003B28E8);
        return;
    }
    if ((DoorHold_Usable(gProgress, f->c.a.room, *(u8 *)&f->c.unk100) & 0xFF) != 1) {
        if (f->c.unk104[0] == 0) {
            if (quick == 1) {
                door_kind(f, 0, 0x600);
            } else {
                door_kind(f, 4, 0x604);
            }
        } else if (quick == 1) {
            door_kind(f, 2, 0x602);
        } else {
            door_kind(f, 6, 0x606);
        }
        FI(f, 0x1AD6C0, s32) = door_spot(f, at, dir);
        if (FI(f, 0x1AD6C0, s32) == -1) {
            VObject *doors;
            Progress *p;

            f->unk1AD580 = 0;
            f->c.moveMode = 0;
            f->savedYaw = f->c.a.angle[1];
            f->unk1AD5C0 = 0;
            f->unk1AD588 = 0;
            if (f->c.unkE0 == 0) {
                f->c.a.unk2D = 0;
                if (*(s32 *)((u8 *)f->c.motion + 0x4C4) != 0) {
                    Fiona_IdleAnim(f, -1);
                }
                Actor_SetState(&f->c.a, &D_003B25A8);
            } else {
                Actor_SetState(&f->c.a, &D_003B25B8);
            }
            p = gProgress;
            Progress_ClearFlag(p, 0x2B);
            doors = gDoors;
            VCALL(doors, 0x20, void (*)(VObject *, u32, s32, s32))(doors, *(u8 *)&f->c.unk100, 0, 0x60000);
            VCALL(doors, 0x1C, void (*)(VObject *, u32, s32, s32))(doors, *(u8 *)&f->c.unk100, 1, 0x60000);
            DoorHold_Open(p, f->c.a.room, *(u8 *)&f->c.unk100, *(u8 *)&f->c.a.slot);
            return;
        }
        tri = FI(f, 0x1AD6C0, s32);
        door_walk(f, tri, at, dir[1], &D_003B28E8);
        return;
    }
    if (!quick) {
        if (f->c.unk104[0] == 0) {
            door_kind(f, 4, 0x60B);
        } else {
            door_kind(f, 6, 0x60A);
        }
        if (FI(f, 0x1AD584, s32) & 2) {
            FI(f, 0x1AD634, s32) = door_spot(f, (f32 *)((u8 *)f + 0x1AD640), dir);
            FI(f, 0x1AD65C, f32) = dir[1];
            Actor_SetState(&f->c.a, &D_003B28B8);
        } else {
            FI(f, 0x1AD6C0, s32) = door_spot(f, at, dir);
            tri = FI(f, 0x1AD6C0, s32);
            door_walk(f, tri, at, dir[1], &D_003B28C8);
        }
    } else {
        if (f->c.unk104[0] == 0) {
            door_kind(f, 0, 0x609);
        } else {
            door_kind(f, 2, 0x608);
        }
        FI(f, 0x1AD6C0, s32) = door_spot(f, at, dir);
        tri = FI(f, 0x1AD6C0, s32);
        door_walk(f, tri, at, dir[1], &D_003B28D8);
    }
}

extern s32 Fiona_DoorFrame(Fiona *f);   /* walk to the door spot: < 0 can't, 0 there, > 0 on the way */
extern const PTMF D_003B28F8, D_003B2908, D_003B2918, D_003B2928, D_003B2938;
extern u8 D_003B2450[];   /* the event script of a locked door's rattle */

/* give up on the door: idle (the door flag 0x2B cleared) */
static inline __attribute__((always_inline)) void door_give_up(Fiona *f, Progress *p) {
    f->unk1AD580 = 0;
    f->c.moveMode = 0;
    f->savedYaw = f->c.a.angle[1];
    f->unk1AD5C0 = 0;
    f->unk1AD588 = 0;
    if (f->c.unkE0 == 0) {
        f->c.a.unk2D = 0;
        if (*(s32 *)((u8 *)f->c.motion + 0x4C4) != 0) {
            Fiona_IdleAnim(f, -1);
        }
        Actor_SetState(&f->c.a, &D_003B25A8);
    } else {
        Actor_SetState(&f->c.a, &D_003B25B8);
    }
    Progress_ClearFlag(p, 0x2B);
}

/* the motion's current animation has run out */
static inline s32 door_anim_done(Fiona *f) {
    u8 done = 1;

    if (!(AT(f->c.motion, 0x550, f32) <= 0.0f)) {
        done = 0;
    }
    return done;
}

/* ---- walking to a spot (FI 0x1AD640, its nav tri FI 0x1AD634) and facing savedYaw there, for
 * the doors. FI 0x1AD650 its flags: 1 the turn is to the right, 2 a quarter turn first, 4 a
 * half turn first, 8 close (under 7) - the last part, 0x10 there, 0x20 finishing (stop
 * animation), 0x40 the turn started, 0x100 the turn animation slowed (FI 0x1AD660 its speed);
 * FI 0x1AD654 frames of the last straight part, FI 0x1AD658 the step, FI 0x1AD65C the turn a
 * frame ---- */

#define WALK_FLAGS(f) FI(f, 0x1AD650, u32)
#define WALK_STEP(f) FI(f, 0x1AD658, f32)
#define WALK_TURN(f) FI(f, 0x1AD65C, f32)
#define WALK_ANIM_SPEED(f) FI(f, 0x1AD660, f32)

/* |the turn left to savedYaw| (the original wraps it twice) */
static inline __attribute__((always_inline)) f32 walk_turn_left(Fiona *f) {
    f32 t;

    if (!(Angle_Wrap(f->savedYaw - f->c.a.angle[1]) <= 0.0f)) {
        t = Angle_Wrap(f->savedYaw - f->c.a.angle[1]);
    } else {
        t = -Angle_Wrap(f->savedYaw - f->c.a.angle[1]);
    }
    return t;
}

/* a turn animation, its speed k (by the distance): under 0.5 played at half speed with the
 * step doubled, under 1 at k, else at full speed (and the step as it is) */
static inline __attribute__((always_inline)) void walk_turn_anim(Fiona *f, s32 anim, f32 k) {
    if (k < 1.0f) {
        if (k < 0.5f) {
            if (f->c.moveMode == 0) {
                f->c.moveSub = 1;
            }
            if ((Progress_GameMode(gProgress) & 0xFF) == 2) {
                Motion_PlayOwnBlend(f->c.motion, anim, 5);
            } else {
                Motion_PlayOwnBlend(f->c.motion, anim, 0);
            }
            AT(MOTION_PTR(f->c.motion, 0x6A4), 0x1C, f32) = 0.5f;
            WALK_ANIM_SPEED(f) = WALK_ANIM_SPEED(f) * 2.0f;
            WALK_FLAGS(f) |= 0x100;
        } else {
            if (f->c.moveMode == 0) {
                f->c.moveSub = 1;
            }
            if ((Progress_GameMode(gProgress) & 0xFF) == 2) {
                Motion_PlayOwnBlend(f->c.motion, anim, 5);
            } else {
                Motion_PlayOwnBlend(f->c.motion, anim, 0);
            }
            AT(MOTION_PTR(f->c.motion, 0x6A4), 0x1C, f32) = k;
        }
    } else {
        if (f->c.moveMode == 0) {
            f->c.moveSub = 1;
        }
        if ((Progress_GameMode(gProgress) & 0xFF) == 2) {
            Motion_PlayOwnBlend(f->c.motion, anim, 5);
        } else {
            Motion_PlayOwnBlend(f->c.motion, anim, 0);
        }
        AT(MOTION_PTR(f->c.motion, 0x6A4), 0x1C, f32) = 1.0f;
        WALK_FLAGS(f) |= 0x100;
    }
}

/* there: on the spot, facing savedYaw */
static inline __attribute__((always_inline)) s32 walk_arrive(Fiona *f) {
    f32 yaw;

    f->c.a.navTri = FI(f, 0x1AD634, s32);
    sceVu0CopyVector(f->c.a.pos, (f32 *)((u8 *)f + 0x1AD640));
    yaw = f->savedYaw;
    f->c.a.angle[1] = yaw;
    sceVu0UnitMatrix((f32 (*)[4])((u8 *)f + 0x60));
    sceVu0RotMatrixY((f32 (*)[4])((u8 *)f + 0x60), (f32 (*)[4])((u8 *)f + 0x60), yaw);
    WALK_FLAGS(f) |= 0x10;
    return 0;
}

/* a frame of it: 0 there, 1 on the way, -1 she can't (no path, blocked by a wall or the
 * pursuer). Far off, she follows the planned path (turning toward its next point, the step by
 * how well she faces it); within 7 of the spot she settles how to face savedYaw there - a
 * quarter turn (anims 0x400 / 0x401) or a half turn (0x402) played on the way, else a straight
 * last part - and steps by the turn's root motion; at the spot she ends with a stop animation */
/* 0x00188C10 */
s32 Fiona_DoorFrame(Fiona *f) {
    static const union { u32 u; f32 f; } kQuarterPi = {0x3F490FDB}, kThreeQuarterPi = {0x4016CBE4},
        kHalfDeg = {0x3C0EFA35}, kEightDeg = {0x3E0EFA35};
    f32 out[4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 r[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    u32 tri;
    u8 turned = 0;
    s32 n, near;
    f32 d, t;

    if (WALK_FLAGS(f) & 0x10) {
        return 0;
    }
    if (!(WALK_FLAGS(f) & 8)) {
        if (!(f->c.unk128 < f->c.unk124)) {
            n = Character_PlanPathKind(&f->c, 0, FI(f, 0x1AD634, u32), (f32 *)((u8 *)f + 0x1AD640));
            if (n > 0) {
                n = Character_WaypointsCurve(&f->c);
            }
            if (n < 0) {
                Character_RootMoveMasked(&f->c);
                return -1;
            }
        }
        d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
            gSceneGameF29740, f->c.a.pos, f->c.unk128, f->c.unk124, (u8 *)f + 0x12C);
        if (d < 7.0f) {
            if (!door_anim_done(f)) {
                Character_RootMoveMasked(&f->c);
                f->c.unk124 = f->c.unk128;
                return 1;
            }
            WALK_FLAGS(f) |= 8;
            sceVu0SubVector(v, (f32 *)((u8 *)f + 0x1AD640), f->c.a.pos);
            d = Angle_Wrap(f->savedYaw - func_0031C5C0(v[0], v[2]));
            if (!(d <= 0.0f)) {
                t = d;
            } else {
                t = -d;
            }
            if (!(t <= kQuarterPi.f)) {
                if (!(d <= 0.0f)) {
                    t = d;
                } else {
                    t = -d;
                }
                if (!(t <= kThreeQuarterPi.f)) {
                    WALK_FLAGS(f) |= 4;
                } else {
                    WALK_FLAGS(f) |= 2;
                    if (d < 0.0f) {
                        WALK_FLAGS(f) |= 1;
                    }
                }
            }
        }
    }

    if (!(WALK_FLAGS(f) & 8)) {
        /* on the path: the step by how well she faces its next point (max 3 down the root
         * motion), turning toward it 8 degrees a frame */
        f32 ahead[4] __attribute__((aligned(16)));
        u32 aheadTri;
        f32 yaw, fwd;

        if (door_anim_done(f)) {
            Fiona_WalkLook(f);
        }
        Character_WaypointAhead(&f->c, &aheadTri, ahead, 3.0f);
        yaw = Actor_HeadingTo(&f->c.a, ahead);
        Motion_RootMovement(f->c.motion, v, 0.0f);
        fwd = v[2];
        if (fwd < 0.0f) {
            fwd = 0.0f;
        }
        sceVu0UnitMatrix(m);
        sceVu0RotMatrixY(m, m, yaw);
        r[0] = 0.0f;
        r[1] = 0.0f;
        r[2] = 1.0f;
        r[3] = 0.0f;   /* (unset in the original) */
        sceVu0ApplyMatrix(v, m, r);
        sceVu0ApplyMatrix(r, (f32 (*)[4])((u8 *)f + 0x60), r);
        WALK_STEP(f) = fwd * ((1.0f + sceVu0InnerProduct(v, r)) / 2.0f);
        Actor_TurnToward(&f->c.a, yaw, kEightDeg.f);
        goto step;
    }

    if (!(WALK_FLAGS(f) & 0x60)) {
        d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
            gSceneGameF29740, f->c.a.pos, f->c.unk128, f->c.unk124, (u8 *)f + 0x12C);
        if (d < 0.5f) {
            WALK_STEP(f) = 0x1.99999ap-3f /* 0.2 */ * d;
            WALK_TURN(f) = 0x1.99999ap-3f /* 0.2 */ * walk_turn_left(f);
            WALK_FLAGS(f) |= 0x20;
            Fiona_IdleAnim(f, -1);
        } else {
            WALK_FLAGS(f) |= 0x40;
            if (WALK_FLAGS(f) & 4) {
                WALK_STEP(f) = 0.0f;
                WALK_ANIM_SPEED(f) = d / 0x1.933334p+1f /* 3.15 */;
                WALK_TURN(f) = 2.0f * (0x1.99999ap-5f /* 0.05 */ * walk_turn_left(f));
                walk_turn_anim(f, 0x402, WALK_ANIM_SPEED(f));
            } else if (!(WALK_FLAGS(f) & 2)) {
                FI(f, 0x1AD654, s32) = 10;
                WALK_STEP(f) = 0x1.26e978p-4f /* 0.072 */ * d;
                WALK_TURN(f) = 0x1.26e978p-4f /* 0.072 */ * walk_turn_left(f);
                Fiona_WalkLook(f);
            } else {
                if (!(WALK_FLAGS(f) & 1)) {
                    t = d / 0x1.23d70ap+2f /* 4.56 */;
                    WALK_ANIM_SPEED(f) = t;
                    walk_turn_anim(f, 0x400, t);
                } else {
                    t = d / 0x1.f851ecp+1f /* 3.94 */;
                    WALK_ANIM_SPEED(f) = t;
                    walk_turn_anim(f, 0x401, t);
                }
                WALK_STEP(f) = 0.0f;
                WALK_TURN(f) = 2.0f * (0x1.70a3d8p-5f /* 0.045 */ * walk_turn_left(f));
            }
        }
    }

    if (WALK_FLAGS(f) & 6) {
        /* turning on the way: the step is the turn's root motion */
        WALK_STEP(f) = 0.0f;
        if (door_anim_done(f)) {
            Motion_RootMovement(f->c.motion, out, 0.0f);
            if (WALK_FLAGS(f) & 2) {
                t = out[0];
            } else {
                t = out[2];
            }
            if (!(t <= 0.0f)) {
                WALK_STEP(f) = t;
            } else {
                WALK_STEP(f) = -t;
            }
            if (WALK_FLAGS(f) & 0x100) {
                WALK_STEP(f) = WALK_STEP(f) * WALK_ANIM_SPEED(f);
            }
            if (WALK_STEP(f) < 0x1.99999ap-5f /* 0.05 */) {
                WALK_STEP(f) = 0x1.99999ap-5f;
            }
        }
    } else if (WALK_STEP(f) < 0x1.99999ap-5f /* 0.05 */) {
        WALK_STEP(f) = 0x1.99999ap-5f;
    }
    if (WALK_TURN(f) < kHalfDeg.f) {
        WALK_TURN(f) = kHalfDeg.f;
    }
    if (Actor_TurnToward(&f->c.a, f->savedYaw, WALK_TURN(f)) == 0.0f) {
        turned = 1;
    }

step:
    n = Character_WaypointAhead(&f->c, &tri, out, WALK_STEP(f));
    if (f->c.a.unk2B == 0) {
        u8 *nm;
        u32 fl;

        if (tri == (u32)-1) {
            return -1;
        }
        nm = (u8 *)gNavMesh;
        fl = 0;   /* (the original reads a NULL record for a triangle out of range) */
        if (tri < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) {
            fl = AT(AT(nm, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
        }
        if (AT(f, 0xC0, u32) & fl) {
            return -1;
        }
    }
    f->c.a.navTri = tri;
    sceVu0CopyVector(f->c.a.pos, out);
    f->c.unk128 = n;

    near = 0;
    if (FI(f, 0x1AD5D7, u8) == 1 && (Actor_Touching(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) == 1) {
        near = 2;
    } else if ((Actor_Touching(&f->c.a, &((Character *)gCharPartner)->a, 0.0f, 0.0f) & 0xFF) == 1) {
        near = 1;
    }
    if (near == 2) {
        return -1;
    }
    if (near == 1) {
        f->c.a.unk2A = 1;
    }

    if (!(WALK_FLAGS(f) & 8)) {
        return 1;
    }
    FI(f, 0x1AD654, s32) -= 1;
    if (!door_anim_done(f)) {
        return 1;
    }
    if (WALK_FLAGS(f) & 6) {
        if (f->c.unk128 < f->c.unk124) {
            return 1;
        }
        if (turned == 1) {
            if (WALK_FLAGS(f) & 0x20) {
                return walk_arrive(f);
            }
            WALK_FLAGS(f) |= 0x20;
        }
        if (Fiona_AnimGroup(AT(f->c.motion, 0x55C, s32)) != 0) {
            if ((Progress_GameMode(gProgress) & 0xFF) == 2) {
                Motion_PlayBlend(f->c.motion, 5, 5, -1);
            } else {
                Motion_PlayBlend(f->c.motion, 0, 5, -1);
            }
        }
        return 1;
    }
    if (WALK_FLAGS(f) & 0x20) {
        if (turned != 1) {
            return 1;
        }
        return walk_arrive(f);
    }
    if (FI(f, 0x1AD654, s32) > 0) {
        return 1;
    }
    if ((WALK_FLAGS(f) & 0x60) != 0x40) {
        return 1;
    }
    WALK_FLAGS(f) |= 0x20;
    Fiona_IdleAnim(f, -1);
    return 1;
}

/* D_003B28E8: walking to the spot; there, a door that is barred for her (exit bit 8) or whose
 * state (Progress_ExitOpen) says it can't be used this way makes her give up; else unless it holds
 * her back (DoorHold_Take) it is opened (doors +0x1C) with the door animation (D_003B28F8) */
/* 0x00196FC0 */
void Fiona_StateDoorWalk(Fiona *f) {
    Progress *p;
    s32 r;
    u8 ok;

    r = Fiona_DoorFrame(f);
    if (r < 0) {
        door_give_up(f, gProgress);
        return;
    }
    if (r != 0) {
        return;
    }
    p = gProgress;
    if ((PursuerGroup_Fields(p, *(u8 *)&f->c.unk100, 2) & 0xFF) & 8) {
        door_give_up(f, p);
        return;
    }
    ok = Progress_ExitOpen(p, f->c.a.room, *(u8 *)&f->c.unk100);
    if (f->c.moveSub == 0x14) {
        if (ok == 1) {
            door_give_up(f, p);
            return;
        }
    } else if (ok == 0) {
        door_give_up(f, p);
        return;
    }
    if ((DoorHold_Take(p, f->c.a.room, *(u8 *)&f->c.unk100, *(u8 *)&f->c.a.slot) & 0xFF) != 0) {
        door_give_up(f, p);
        return;
    }
    f->c.unk104[1] = 1;
    if (f->c.moveSub == 0x14) {
        VCALL(gDoors, 0x1C, void (*)(VObject *, u32, s32, s32))(gDoors, *(u8 *)&f->c.unk100, 1, 0x20000);
    } else {
        VCALL(gDoors, 0x1C, void (*)(VObject *, u32, s32, s32))(gDoors, *(u8 *)&f->c.unk100, 0, 0x20000);
    }
    Motion_PlayOwnBlend(f->c.motion, FI(f, 0x1AD6CC, s32), -1);
    f->c.a.unk2B = 1;
    f->c.a.unk2A = 1;
    Actor_SetState(&f->c.a, &D_003B28F8);
}

/* D_003B28F8: the door animation; when it has run out, the doors are told she is through
 * (+0xC) and she steps out (D_003B2908) */
/* 0x00196EF0 */
void Fiona_StateDoorAnim(Fiona *f) {
    f->c.a.unk2A = 1;
    if (!door_anim_done(f)) {
        return;
    }
    VCALL(gDoors, 0xC, void (*)(VObject *, u32, s32, s32, s32))(
        gDoors, *(u8 *)&f->c.unk100, FI(f, 0x1AD6C8, s32), f->c.a.slot, 0);
    Actor_SetState(&f->c.a, &D_003B2908);
}

/* D_003B2908: stepping out; at the animation's event 0x20 she is free again (nav flags
 * 0x28020018): the door shut behind her (doors +0x20 / +0x1C; the progress told,
 * DoorHold_Open in / DoorHold_Shut out) and idle. Every frame: shown (unk2D) unless still in
 * the door's region (doors +0x18) on a triangle without flag 0x20000 */
/* 0x00196C20 */
void Fiona_StateDoorStepOut(Fiona *f) {
    s32 r;

    f->c.a.unk2A = 1;
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        VObject *doors;
        Progress *p;

        f->c.a.unk2A = 0;
        f->c.a.unk2B = 0;
        AT(f, 0xC0, u32) = 0x28020018;
        doors = gDoors;
        if (f->c.moveSub == 0x14) {
            VCALL(doors, 0x20, void (*)(VObject *, u32, s32, s32))(doors, *(u8 *)&f->c.unk100, 0, 0x60000);
            VCALL(doors, 0x1C, void (*)(VObject *, u32, s32, s32))(doors, *(u8 *)&f->c.unk100, 1, 0x60000);
            p = gProgress;
            DoorHold_Open(p, f->c.a.room, *(u8 *)&f->c.unk100, 0xFF);
        } else {
            VCALL(doors, 0x20, void (*)(VObject *, u32, s32, s32))(doors, *(u8 *)&f->c.unk100, 1, 0x60000);
            VCALL(doors, 0x1C, void (*)(VObject *, u32, s32, s32))(doors, *(u8 *)&f->c.unk100, 0, 0x60000);
            p = gProgress;
            DoorHold_Shut(p, f->c.a.room, *(u8 *)&f->c.unk100, 0xFF);
        }
        door_give_up(f, p);
    }
    Character_RootMoveMasked(&f->c);
    r = VCALL(gDoors, 0x18, s32 (*)(VObject *, u32, f32 *))(gDoors, *(u8 *)&f->c.unk100, f->c.a.pos);
    if (f->c.unk104[0] != r) {
        f->c.a.unk2D = 1;
        return;
    }
    {
        u8 *nm = (u8 *)gNavMesh;
        u32 i = f->c.a.navTri;
        u32 fl = 0;   /* (the original reads it through a NULL record - address 0x3C - for a
                       * triangle out of range; 0 here) */

        if (i < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) {
            fl = AT(AT(nm, 0x4, u8 *) + i * 0x50, 0x3C, u32);
        }
        f->c.a.unk2D = (fl & 0x20000) ? 1 : 0;
    }
}

/* D_003B2918: the try at a locked door from where she stood; turning (FI 0x1AD65C at
 * FI 0x1AD6D4 a frame) and walking (FI 0x1AD6D0 a frame) to the spot while it plays, then put
 * on the spot facing the door (D_003B2928) */
/* 0x00196660 */
void Fiona_StateLockedTry(Fiona *f) {
    f32 at[4] __attribute__((aligned(16)));
    u32 tri;
    f32 yaw;

    if (door_anim_done(f)) {
        f->c.a.navTri = FI(f, 0x1AD634, s32);
        sceVu0CopyVector(f->c.a.pos, (f32 *)((u8 *)f + 0x1AD640));
        VCALL(gNavMesh, 0x14, void (*)(NavMesh *, s32, f32 *))(gNavMesh, f->c.a.navTri, f->c.a.pos);
        yaw = FI(f, 0x1AD65C, f32);
        f->c.a.angle[1] = yaw;
        sceVu0UnitMatrix((f32 (*)[4])((u8 *)f + 0x60));
        sceVu0RotMatrixY((f32 (*)[4])((u8 *)f + 0x60), (f32 (*)[4])((u8 *)f + 0x60), yaw);
        Actor_SetState(&f->c.a, &D_003B2928);
        return;
    }
    Actor_TurnToward(&f->c.a, FI(f, 0x1AD65C, f32), FI(f, 0x1AD6D4, f32));
    tri = f->c.a.navTri;
    f->c.unk128 = Character_WaypointAhead(&f->c, &tri, at, FI(f, 0x1AD6D0, f32));
    f->c.a.navTri = tri;
    sceVu0CopyVector(f->c.a.pos, at);
}

/* D_003B28B8: a locked door, slow: once the current animation is done, a path to the spot
 * (Character_PlanPathKind / Character_WaypointsCurve; none: give up); its length / 7 the step and the turn to the
 * door's facing / 7 the turn a frame, the try played (FI 0x1AD6CC, D_003B2918) */
/* 0x001967D0 */
void Fiona_StateLockedSlow(Fiona *f) {
    static const union { u32 u; f32 f; } kSeventh = {0x3E126E98};
    s32 r;
    f32 d;

    if (!door_anim_done(f)) {
        return;
    }
    r = Character_PlanPathKind(&f->c, 0, FI(f, 0x1AD634, u32), (f32 *)((u8 *)f + 0x1AD640));
    if (r > 0) {
        r = Character_WaypointsCurve(&f->c);
    }
    if (r <= 0) {
        door_give_up(f, gProgress);
        return;
    }
    d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
        gSceneGameF29740, f->c.a.pos, f->c.unk128, f->c.unk124, (u8 *)f + 0x12C);
    FI(f, 0x1AD6D0, f32) = kSeventh.f * d;
    if (!(Angle_Wrap(FI(f, 0x1AD65C, f32) - f->c.a.angle[1]) <= 0.0f)) {
        d = Angle_Wrap(FI(f, 0x1AD65C, f32) - f->c.a.angle[1]);
    } else {
        d = -Angle_Wrap(FI(f, 0x1AD65C, f32) - f->c.a.angle[1]);
    }
    FI(f, 0x1AD6D4, f32) = kSeventh.f * d;
    Motion_PlayOwnBlend(f->c.motion, FI(f, 0x1AD6CC, s32), -1);
    Actor_SetState(&f->c.a, &D_003B2918);
}

/* D_003B28C8: a locked door, from the spot: walking there, then the try played (D_003B2938) */
/* 0x001964C0 */
void Fiona_StateLockedSpot(Fiona *f) {
    s32 r = Fiona_DoorFrame(f);

    if (r < 0) {
        door_give_up(f, gProgress);
        return;
    }
    if (r != 0) {
        return;
    }
    Motion_PlayOwnBlend(f->c.motion, FI(f, 0x1AD6CC, s32), -1);
    Actor_SetState(&f->c.a, &D_003B2938);
}

/* D_003B28D8: a locked door, quick: walking to the spot, then +0x8C, the rattle's event script
 * run for her (events +0x1C) and the scripted action 7 with the try's animation (unk104[0]) */
/* 0x00196A90 */
void Fiona_StateLockedQuick(Fiona *f) {
    s32 r = Fiona_DoorFrame(f);

    if (r < 0) {
        door_give_up(f, gProgress);
        return;
    }
    if (r != 0) {
        return;
    }
    f->c.a.unk2A = 1;
    VCALL(f, 0x8C, void (*)(Fiona *))(f);
    VCALL(gEvents, 0x1C, void (*)(VObject *, u32, u8 *))(gEvents, *(u8 *)&f->c.a.slot, D_003B2450);
    f->c.unkF4 = 7;
    f->c.unk104[0] = FI(f, 0x1AD6CC, s32);
}

/* ---- scripted moves (the commands of Fiona_Requests) ---- */

extern const PTMF D_003B2DB8;   /* walking to the scripted spot */

/* D_003B2778 (command 17): the walk's animation playing - turning (to unk104[2] at FI 0x1AD6D4
 * a frame) and stepping (FI 0x1AD6D0) on the path; at its event 0x20 put on the spot (unk110,
 * tri unk100) facing unk104[2], the command done */
/* 0x0018A120 */
void Fiona_StateCmdWalkAnim(Fiona *f) {
    f32 at[4] __attribute__((aligned(16)));
    u32 tri;

    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        f32 yaw;

        f->c.a.navTri = f->c.unk100;
        sceVu0CopyVector(f->c.a.pos, f->c.unk110);
        VCALL(gNavMesh, 0x14, void (*)(NavMesh *, s32, f32 *))(gNavMesh, f->c.a.navTri, f->c.a.pos);
        yaw = *(f32 *)&f->c.unk104[2];
        f->c.a.angle[1] = yaw;
        sceVu0UnitMatrix((f32 (*)[4])((u8 *)f + 0x60));
        sceVu0RotMatrixY((f32 (*)[4])((u8 *)f + 0x60), (f32 (*)[4])((u8 *)f + 0x60), yaw);
        f->c.unkE1 = 1;
        return;
    }
    Actor_TurnToward(&f->c.a, *(f32 *)&f->c.unk104[2], FI(f, 0x1AD6D4, f32));
    tri = f->c.a.navTri;
    f->c.unk128 = Character_WaypointAhead(&f->c, &tri, at, FI(f, 0x1AD6D0, f32));
    f->c.a.navTri = tri;
    sceVu0CopyVector(f->c.a.pos, at);
}

/* D_003B2DB8: walking to the scripted spot; there the command is done, unable: idle */
/* 0x0018A720 */
void Fiona_StateCmdToSpot(Fiona *f) {
    s32 r = Fiona_DoorFrame(f);

    if (r == 0) {
        f->c.unkE1 = 1;
    } else if (r < 0) {
        door_give_up(f, gProgress);
    }
}

/* D_003B26E8 (command 5 with no animation): walk to the spot unk110 (tri unk104[0]) and face
 * unk104[2] there (D_003B2DB8) */
/* 0x0018A860 */
void Fiona_StateCmdWalkSpot(Fiona *f) {
    f32 yaw = *(f32 *)&f->c.unk104[2];
    s32 tri = f->c.unk104[0];

    f->c.unk124 = f->c.unk128;
    FI(f, 0x1AD650, s32) = 0;
    FI(f, 0x1AD634, s32) = tri;
    sceVu0CopyVector((f32 *)((u8 *)f + 0x1AD640), f->c.unk110);
    VCALL(gNavMesh, 0x14, void (*)(NavMesh *, s32, f32 *))(gNavMesh, tri, (f32 *)((u8 *)f + 0x1AD640));
    f->savedYaw = yaw;
    Actor_SetState(&f->c.a, &D_003B2DB8);
}

extern const PTMF D_003B2DA8;   /* walking the scripted path */
extern const PTMF D_003B2D98;   /* the animation run out, then done */

/* the scripted animation (unk104[1]; -1: walk when FI 0x1AD580 is `walk`, else run - none if
 * `walk` is 0) restarted whenever the current one has run out */
static inline __attribute__((always_inline)) void script_anim_keep(Fiona *f, s32 walk) {
    if (door_anim_done(f)) {
        s32 a = f->c.unk104[1];

        if (a == -1) {
            if (walk) {
                if (f->unk1AD580 == walk) {
                    Fiona_WalkLook(f);
                } else {
                    Fiona_RunLook(f);
                }
            }
        } else if (AT(f->c.motion, 0x55C, s32) != a) {
            Motion_PlayTable(f->c.motion, a, -1);
        }
    }
}

/* D_003B2DA8: walking the planned path, turning toward it (10 degrees a frame) and stepping by
 * the root motion (less the sharper the turn); at its end the rest of the step taken, the
 * command done and idle */
/* 0x0018A950 */
void Fiona_StateCmdPath(Fiona *f) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTenDeg = {0x3E32B8C3};
    f32 root[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 d, step, turn;
    u32 tri;
    s32 n;

    script_anim_keep(f, 0x14);
    d = VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
        gSceneGameF29740, f->c.a.pos, f->c.unk128, f->c.unk124, f->c.unk12C);
    Motion_RootMovement(f->c.motion, root, 0.0f);
    Character_WaypointAhead(&f->c, &tri, at, 3.0f);
    turn = Actor_TurnToward(&f->c.a, Actor_HeadingTo(&f->c.a, at), kTenDeg.f);
    if (root[2] < 0.0f) {
        step = 0.0f;
    } else {
        step = root[2] * ((kPi.f - turn) / kPi.f);
    }
    n = Character_WaypointAhead(&f->c, &tri, at, step);
    f->c.a.navTri = tri;
    sceVu0CopyVector(f->c.a.pos, at);
    f->c.unk128 = n;
    if (f->c.unk128 < f->c.unk124) {
        return;
    }
    Motion_RootMovement(f->c.motion, root, 0.0f);
    if (d < root[2]) {
        root[0] = 0.0f;
        root[1] = 0.0f;
        root[2] = root[2] - d;
        sceVu0ApplyMatrix(root, (f32 (*)[4])((u8 *)f + 0x60), root);
        Actor_Move(&f->c.a, root);
    }
    f->c.unkE1 = 1;
    door_give_up(f, gProgress);
}

/* D_003B2708 (commands 6 / 11): a path to the scripted spot (unk110, tri unk104[0]) walked
 * (D_003B2DA8); none: idle */
/* 0x0018AC60 */
void Fiona_StateCmdPathTo(Fiona *f) {
    s32 n;

    VCALL(gNavMesh, 0xC, void (*)(NavMesh *, s32, f32 *))(gNavMesh, f->c.unk104[0], f->c.unk110);
    n = Character_PlanPathKind(&f->c, 0, f->c.unk104[0], f->c.unk110);
    if (n > 0) {
        n = Character_WaypointsCurve(&f->c);
    }
    if (n > 0) {
        Actor_SetState(&f->c.a, &D_003B2DA8);
        return;
    }
    door_give_up(f, gProgress);
}

/* D_003B2D78 / D_003B2D98: once the animation has run out the command is done - unless it was
 * the default one and she is still moving, then she is stopped first */
/* 0x0018AE20 */
void Fiona_StateCmdAnimDone(Fiona *f) {
    if (!door_anim_done(f)) {
        return;
    }
    if (f->c.unk104[1] == -1 && Fiona_AnimGroup(AT(f->c.motion, 0x55C, s32)) != 0) {
        Fiona_IdleAnim(f, -1);
        return;
    }
    f->c.unkE1 = 1;
}

/* D_003B2D88: turning to unk104[2] (20 degrees a frame) with the scripted animation; then the
 * wait for it to run out (D_003B2D98) */
/* 0x0018B0A0 */
void Fiona_StateCmdTurn(Fiona *f) {
    static const union { u32 u; f32 f; } kTwentyDeg = {0x3EB2B8C3};

    script_anim_keep(f, 0);
    if (Actor_TurnToward(&f->c.a, *(f32 *)&f->c.unk104[2], kTwentyDeg.f) == 0.0f) {
        Actor_SetState(&f->c.a, &D_003B2D98);
    }
}

extern const PTMF D_003B2D68, D_003B2D78, D_003B2D88;

/* D_003B2D68 (command 5 / 10): walking the planned path (as D_003B2DA8, no rest step); at
 * its end turning to unk104[2] (D_003B2D88), or already facing it, the wait (D_003B2D78) */
/* 0x0018B190 */
void Fiona_StateCmdPathTurn(Fiona *f) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTenDeg = {0x3E32B8C3};
    f32 root[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 step, turn;
    u32 tri;
    s32 n;

    Motion_RootMovement(f->c.motion, root, 0.0f);
    Character_WaypointAhead(&f->c, &tri, at, 3.0f);
    turn = Actor_TurnToward(&f->c.a, Actor_HeadingTo(&f->c.a, at), kTenDeg.f);
    if (root[2] < 0.0f) {
        step = 0.0f;
    } else {
        step = root[2] * ((kPi.f - turn) / kPi.f);
    }
    n = Character_WaypointAhead(&f->c, &tri, at, step);
    f->c.a.navTri = tri;
    sceVu0CopyVector(f->c.a.pos, at);
    f->c.unk128 = n;
    if (f->c.unk128 < f->c.unk124) {
        script_anim_keep(f, 0x12);
        return;
    }
    if (Actor_TurnToward(&f->c.a, *(f32 *)&f->c.unk104[2], kTenDeg.f) == 0.0f) {
        Actor_SetState(&f->c.a, &D_003B2D78);
    } else {
        Actor_SetState(&f->c.a, &D_003B2D88);
    }
}

/* D_003B26F8 (command 5 / 10): a path to the spot unk110 (tri unk104[0]) walked (D_003B2D68);
 * none: idle */
/* 0x0018B3D0 */
void Fiona_StateCmdPathSpot(Fiona *f) {
    s32 n = Character_PlanPathKind(&f->c, 0, f->c.unk104[0], f->c.unk110);

    if (n > 0) {
        n = Character_WaypointsCurve(&f->c);
    }
    if (n > 0) {
        Actor_SetState(&f->c.a, &D_003B2D68);
        return;
    }
    door_give_up(f, gProgress);
}

/* D_003B25D8 / D_003B26A8: the command done once the animation has run out */
/* 0x0018B5B0 */
void Fiona_StateCmdDone(Fiona *f) {
    if (door_anim_done(f)) {
        f->c.unkE1 = 1;
    }
}

/* D_003B27A8 (panic attack): for FI 0x1AD6C0 frames (then idle, though it plays on) the
 * panic animation 2 kept going with her cry (0x44) */
/* 0x0018C080 */
void Fiona_StatePanicAttack(Fiona *f) {
    FI(f, 0x1AD6C0, s32) -= 1;
    if (FI(f, 0x1AD6C0, s32) == 0) {
        door_give_up(f, gProgress);
    }
    if (door_anim_done(f) && AT(f->c.motion, 0x55C, s32) != 2) {
        Actor_PlaySound(&f->c.a, 0x44, 5, 0, 0, NULL);
        Motion_PlayTable(f->c.motion, 2, -1);
    }
    Character_RootMoveMasked(&f->c);
}

#include "effectmgr.h"

/* the effect on a character her shove met: 0 a burst at it, 1 a hit spark at one of four of its
 * bones (motion +0x84 / +0x88 / +0x8C / +0x90, at random) */
/* 0x0017FD50 */
void Fiona_ShoveEffect(Fiona *f, s32 kind, Character *c) {
    u8 *mgr;

    if (kind == 1) {
        HitEffectParams hp;
        f32 at[4] __attribute__((aligned(16)));
        s32 bone;

        switch ((s32)(4.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom))) {
        case 0:
            bone = VCALL(c->motion, 0x84, s32 (*)(void *))(c->motion);
            break;
        case 1:
            bone = VCALL(c->motion, 0x88, s32 (*)(void *))(c->motion);
            break;
        case 2:
            bone = VCALL(c->motion, 0x8C, s32 (*)(void *))(c->motion);
            break;
        default:
            bone = VCALL(c->motion, 0x90, s32 (*)(void *))(c->motion);
            break;
        }
        sceVu0CopyVector(at, Skel_Bone(AT(c->motion, 0x810, void *), bone) + 12);
        hp.pos[0] = at[0];
        hp.pos[1] = at[1];
        hp.pos[2] = at[2];
        hp.pos[3] = at[3];
        hp.kind = c->a.navTri << 8;
        hp.big = 0.0f;
        HitEffect_Spawn(&hp);
    } else if (kind == 0) {
        mgr = gEffects;
        EffectMgr_Start(mgr, Effect_New(mgr, 0xFD0, ShoveBurst_Init), c->a.pos);
    }
}

/* ---- moving between rooms out of sight (FI 0x1AD71C: 0xB following Hewie, 0xC staying,
 * 0xD wandering, 0xE fleeing the pursuer); the link (door pair) she is on in +0x138C, the time
 * left to its end in +0x14C4 (< 0: choose the next one), the time spent FI 0x1AD73C ---- */

extern const PTMF D_003B2E98;   /* entering through a door */

#define LINK(f) AT(f, 0x138C, u16)
#define LINK_LEFT(f) AT(f, 0x14C4, f32)

/* back in view: placed, the events told, on the mesh */
static inline __attribute__((always_inline)) void fiona_show(Fiona *f) {
    f32 at[4] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 0.0f};   /* (an out-parameter) */

    VCALL(gEvents, 0x2C, void (*)(VObject *, Fiona *))(gEvents, f);
    if (f->c.a.navTri == (u32)-1) {
        AT(f, 0x10, s32) = 0;
        AT(f, 0x14, s32) = 0;
        AT(f, 0x18, s32) = 0;
        AT(f, 0x1C, f32) = 0x1.99999ap-4f /* 0.1 */;
    }
    if (Actor_PosInCurrentRoom(&f->c.a, at)) {
        Character_Sound(&f->c, 3, (s32)at, 0, 0, 0);
    }
    Character_MarkObjects(&f->c);
}

/* a frame of it. In the current room, just placed and shown. Elsewhere her pace by her panic
 * (calm 1; following / fleeing 1 .. 0.75, waiting / wandering 0.45 .. 0.3 - the panic easing
 * then, growing while she follows unless she wears 0x8C); at a link's end the next room, where
 * a door that is shut has to be opened (or she waits, -1) - or the current room, entered
 * through it (D_003B2E98). Then the next link: fleeing, one away from the pursuer's room; for
 * Hewie, his way (Character_Route); else a random open one */
/* 0x001800E0 */
void Fiona_OffscreenFrame(Fiona *f) {
    static const union { u32 u; f32 f; } kSlowPace = {0x3E99999A}, kWaitPace = {0x3EE66666},
        kEaseFast = {0xBDCCCCCD}, kEase = {0xBE19999A}, kFearFast = {0x3D888889}, kFear = {0x3D3F258C};
    Progress *p = gProgress;
    VObject *rooms;
    f32 speed = 0.0f;   /* (unset in the original for any other mode) */
    f32 k;

    if (f->c.a.room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        f->c.a.disabled = 0;
        VCALL(f, 0x28, void (*)(Fiona *, u32, f32 *, f32 *))(f, f->c.a.navTri, NULL, f->c.a.pos);
        fiona_show(f);
        return;
    }
    if (AT(p, 0x7B8, u8) == 5) {
        speed = 0x1.99999ap-4f /* 0.1 */;
    } else {
        k = (100.0f - FI(f, 0x1AD5F4, f32)) / 60.0f;
        switch (FI(f, 0x1AD71C, s32)) {
        case 0xC:
        case 0xD:
            if (k < 1.0f) {
                speed = kSlowPace.f * (1.0f - k) + kWaitPace.f * k;
            } else {
                speed = kWaitPace.f;
            }
            if ((Progress_GameMode(p) & 0xFF) == 2) {
                Fiona_ChangeFear(f, kEaseFast.f);
            } else {
                Fiona_ChangeFear(f, kEase.f);
            }
            break;
        case 0xE:
        case 0xB:
            speed = 1.0f;
            if (k < speed) {
                speed = 0.0f + k + 0.75f * (1.0f - k);
            }
            if (VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 3) != 0x8C) {
                if ((Progress_GameMode(p) & 0xFF) == 2) {
                    Fiona_ChangeFear(f, kFearFast.f);
                } else {
                    Fiona_ChangeFear(f, kFear.f);
                }
            }
            break;
        }
    }
    if (FI(f, 0x1AD71C, s32) == 0xC) {
        return;
    }
    if (f->c.unk128 < f->c.unk124) {
        Character_FollowWaypoints(&f->c, speed);
        if (f->c.unk128 < f->c.unk124) {
            return;
        }
        LINK_LEFT(f) = 0.0f;
        FI(f, 0x1AD6C0, s32) = 0;
        return;
    }
    if (!(LINK_LEFT(f) < 0.0f)) {
        /* on a link */
        LINK_LEFT(f) = LINK_LEFT(f) - speed;
        FI(f, 0x1AD73C, f32) = FI(f, 0x1AD73C, f32) + speed;
        if (!(LINK_LEFT(f) < 0.0f)) {
            return;
        }
        f->c.unk124 = f->c.unk128;
        if (FI(f, 0x1AD6C0, s32) != 0) {
            return;
        }
        FI(f, 0x1AD6C0, s32) = 1;
        rooms = gRooms;
        f->c.a.room = VCALL(rooms, 0x1C, s32 (*)(VObject *, u32, s32))(rooms, LINK(f), f->c.a.room);
        f->c.door = VCALL(rooms, 0x3C, s32 (*)(VObject *, u32, s32))(rooms, LINK(f), f->c.a.room);
        FI(f, 0x1AD73C, s32) = 0;
        FI(f, 0x1AD738, s32) =
            (s32)(30.0f * (2.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom))) + 90;
        if (f->c.a.room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
            /* another room out of sight: a shut door is opened, unless she can't (wait) */
            f->c.a.navTri = (u32)-1;
            if ((Progress_ExitOpen(p, f->c.a.room, f->c.door) & 0xFF) == 0) {
                if ((Progress_ExitUnlocked(p, f->c.a.room, f->c.door) & 0xFF) == 1 ||
                    (DoorHold_Usable(p, f->c.a.room, f->c.door) & 0xFF) == 1 ||
                    (Progress_ExitPassable(p, f->c.a.room, f->c.door, *(u8 *)&f->c.a.slot) & 0xFF) == 0) {
                    LINK_LEFT(f) = -1.0f;
                    return;
                }
                if ((DoorHold_Take(p, f->c.a.room, f->c.door, *(u8 *)&f->c.a.slot) & 0xFF) == 1) {
                    LINK_LEFT(f) = -1.0f;
                    return;
                }
                DoorHold_Open(p, f->c.a.room, f->c.door, *(u8 *)&f->c.a.slot);
            }
            LINK_LEFT(f) = (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, LINK(f), f->c.a.room);
            return;
        }
        /* into the current room */
        f->c.a.disabled = 0;
        f->c.a.unk2A = 1;
        if ((Progress_ExitOpen(p, f->c.a.room, f->c.door) & 0xFF) == 1) {
            f32 a[4] __attribute__((aligned(16)));
            f32 b[4] __attribute__((aligned(16)));
            f32 d[4] __attribute__((aligned(16)));
            f32 yaw;
            u32 tri;

            tri = VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, f->c.door, a);
            VCALL(rooms, 0x2C, void (*)(VObject *, u32, f32 *))(rooms, f->c.door, b);
            sceVu0SubVector(d, b, a);
            yaw = func_0031C5C0(d[0], d[2]);
            VCALL(f, 0x28, void (*)(Fiona *, u32, f32 *, f32 *))(f, tri, &yaw, a);
        } else {
            f32 at[4] __attribute__((aligned(16)));
            f32 spot[4] __attribute__((aligned(16)));
            f32 dir[4] __attribute__((aligned(16)));
            VObject *doors;
            s32 dbl, kind;
            u32 tri;

            VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, f->c.door, at);
            dbl = VCALL(gDoors, 0x18, s32 (*)(VObject *, u32, f32 *))(gDoors, f->c.door, at);
            if ((FI(f, 0x1AD584, s32) & 2) || (Progress_GameMode(p) & 0xFF) == 2) {
                kind = dbl ? 6 : 4;
            } else {
                kind = dbl ? 2 : 0;
            }
            doors = gDoors;
            tri = VCALL(doors, 0x14, u32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(
                doors, f->c.door, kind, spot, dir, 0);
            VCALL(f, 0x28, void (*)(Fiona *, u32, f32 *, f32 *))(f, tri, &dir[1], spot);
            f->c.unk100 = f->c.door;
            f->c.unk104[0] = VCALL(doors, 0x18, s32 (*)(VObject *, u32, f32 *))(doors, *(u8 *)&f->c.unk100,
                                                                                f->c.a.pos);
            f->c.unk104[1] = 0;
            FI(f, 0x1AD620, s32) = 0;
            f->unk1AD580 = 3;
            f->c.moveMode = 2;
            f->c.moveSub = 0x14;
            Actor_SetState(&f->c.a, &D_003B2E98);
        }
        fiona_show(f);
        return;
    }

    /* the next link */
    {
        s32 cur, mode;
        u8 found = 0, tried = 0;

        f->c.unk124 = f->c.unk128;
        rooms = gRooms;
        cur = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, f->c.a.room, f->c.door, 0);
        if (FI(f, 0x1AD5D6, u8) == 1) {
            s32 pr = gCharPursuer->a.room;

            if (f->c.a.room == pr) {
                found = 1;
            } else {
                s32 target = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, pr, gCharPursuer->door, 0);
                u8 i;

                for (i = 0; i < 8; i++) {
                    if (VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, f->c.a.room, i) == pr &&
                        VCALL(rooms, 0x58, s32 (*)(VObject *, s32, u32, s32))(rooms, f->c.a.room, i, 0) == target) {
                        found = 1;
                        tried |= 1 << i;
                    }
                }
            }
        }
        if (!(FI(f, 0x1AD71C, s32) == 0xE && found != 0)) {
            if (FI(f, 0x1AD719, u8) != 0) {
                if (VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 0.5f) {
                    FI(f, 0x1AD71C, s32) = 0xC;
                } else {
                    FI(f, 0x1AD71C, s32) = 0xD;
                }
            } else {
                FI(f, 0x1AD71C, s32) = 0xB;
            }
        }
        mode = FI(f, 0x1AD71C, s32);
        if (mode == 0xB) {
            Character *h = (Character *)gCharPartner;
            s32 way = AT(h, 0xF3668, s32);

            if (way == 2) {
                way = cur;
            }
            if (Character_Route(&f->c, h->a.room, way, cur, -1) == -1) {
                return;
            }
            f->c.unk14C0 = LINK(f);
            if (FI(f, 0x1AD5D6, u8) == 1 &&
                VCALL(rooms, 0x1C, s32 (*)(VObject *, u32, s32))(rooms, LINK(f), f->c.a.room) == gCharPursuer->a.room) {
                FI(f, 0x1AD71C, s32) = 0xC;
                return;
            }
            LINK_LEFT(f) = (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, LINK(f), f->c.a.room);
            FI(f, 0x1AD6C0, s32) = 0;
            return;
        }
        if (mode == 0xD) {
            tried = 0;
        } else if (mode != 0xE) {
            return;
        }
        while (tried != 0xFF) {
            u8 j = (u8)(u32)(8.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom));

            if (tried & (1 << j)) {
                continue;
            }
            tried |= (u8)(1 << j);
            if ((VCALL(rooms, 0x74, s32 (*)(VObject *, s32, u32))(rooms, f->c.a.room, j) & 0xFF) != 1) {
                continue;
            }
            if (Progress_ExitUnlocked(p, f->c.a.room, j) & 0xFF) {
                continue;
            }
            if (VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, f->c.a.room, j, 0) != cur) {
                continue;
            }
            rooms = gRooms;
            LINK(f) = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, f->c.a.room, j);
            f->c.unk14C0 = LINK(f);
            LINK_LEFT(f) = (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, LINK(f), f->c.a.room);
            FI(f, 0x1AD6C0, s32) = 0;
            return;
        }
    }
}

/* ---- panic and voice helpers ---- */

/* add `amount` to her panic (FI 0x1AD5F4, kept to 0..100), scaled by the accessory she wears
 * (items +0x10(3)): 0x8A gains x0.75, 0x8B gains x0.75 / recovery x1.5, 0x8C gains x0.5 /
 * recovery x2 */
/* 0x00180E90 */
void Fiona_AddPanic(Fiona *f, f32 amount) {
    f32 v;

    if (gSubScreen != NULL) {
        switch (VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 3)) {
        case 0x8D:
            break;
        case 0x8C:
            if (!(amount <= 0.0f)) {
                amount = amount * 0.5f;
            } else {
                amount = amount * 2.0f;
            }
            break;
        case 0x8B:
            if (!(amount <= 0.0f)) {
                amount = amount * 0.75f;
            } else {
                amount = amount * 1.5f;
            }
            break;
        case 0x8A:
            if (!(amount <= 0.0f)) {
                amount = amount * 0.75f;
            }
            break;
        }
    }
    v = FI(f, 0x1AD5F4, f32) + amount;
    FI(f, 0x1AD5F4, f32) = v;
    if (!(v <= 100.0f)) {
        FI(f, 0x1AD5F4, f32) = 100.0f;
    } else if (v < 0.0f) {
        FI(f, 0x1AD5F4, f32) = 0.0f;
    }
}

/* the flags of nav triangle i (the original reads a NULL record - address 0x3C - out of
 * range; 0 here) */
static inline u32 fiona_tri_flags(u8 *nm, u32 i) {
    if (i < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) {
        return AT(AT(nm, 0x4, u8 *) + i * 0x50, 0x3C, u32);
    }
    return 0;
}

/* her voice `id` - unless, while the game runs in her room, the line from her feet to her
 * mouth (motion +0x60) crosses a muffling triangle (flags 0x2008000): then a muffled sound
 * (0x1C), which in rooms 7 and 0x106 is heard (a big noise) */
/* 0x00181180 */
void Fiona_Voice(Fiona *f, s32 id, s32 a2, s32 a3, s32 a4) {
    f32 mouth[4] __attribute__((aligned(16)));
    Progress *p = gProgress;

    if ((VCALL(p, 0x50, s32 (*)(Progress *))(p) & 0xFF) == 1 &&
        f->c.a.room == VCALL(p, 0xC, s32 (*)(Progress *))(p) && f->c.a.navTri != (u32)-1) {
        u8 *nm;
        u32 tri;

        VCALL(f->c.motion, 0x60, void (*)(void *, f32 *))(f->c.motion, mouth);
        tri = f->c.a.navTri;
        nm = (u8 *)gNavMesh;
        do {
            s32 side;
            u8 *rec;

            if ((fiona_tri_flags(nm, tri) & 0x2008000) == 0x2008000) {
                Actor_PlaySound(&f->c.a, 0x1C, 6, 0, 0, NULL);
                if (f->c.a.room == 7 || f->c.a.room == 0x106) {
                    Character_WaterStep(&f->c, f->c.a.pos, 1);
                }
                return;
            }
            side = VCALL(nm, 0x20, s32 (*)(void *, u32, f32 *, f32 *))(nm, tri, f->c.a.pos, mouth);
            if (side == 3 || side == 4) {
                break;
            }
            rec = NULL;
            if (tri < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) {
                rec = AT(nm, 0x4, u8 *) + tri * 0x50;
            }
            if (rec == NULL) {
                break;   /* (the original reads low memory here) */
            }
            tri = AT(rec + side * 4, 0x30, u32);   /* the neighbour across that side */
        } while (tri != (u32)-1);
    }
    Actor_PlaySound(&f->c.a, id, a2, a3, a4, NULL);
}

/* a blow (request 4, `arg`) to the creatures in slots 7..9 within reach of `pos` in her room
 * and not in `skip` (by slot): with reach < 0, those touching her (Actor_Touching); else those
 * whose height span (+-reach) holds pos.y and whose radius + reach holds it. Each one not
 * already in request 7 takes it; the slots hit */
/* 0x001813D0 */
u32 Fiona_BlowCreatures(Fiona *f, u32 skip, s32 arg, f32 *pos, f32 reach) {
    Character **list = (Character **)gCreatures;
    u32 mask = 0;
    s32 i;

    for (i = 0; i < 10; i++, list++) {
        Character *c = *list;
        f32 d[4] __attribute__((aligned(16)));
        u8 hit;

        if (c == NULL || c->a.active != 1 || c->a.unk2D != 0 || f->c.a.room != c->a.room ||
            (skip & (1 << c->a.slot))) {
            continue;
        }
        switch (i) {
        case 7:
        case 8:
        case 9:
            hit = 0;
            if (reach < 0.0f) {
                if ((Actor_Touching(&f->c.a, &c->a, 0.0f, 0.0f) & 0xFF) == 1) {
                    hit = 1;
                }
            } else {
                f32 y = c->a.pos[1];
                f32 top = reach + (y + AT(c, 0xCC, f32));

                if (!(pos[1] <= y - reach) && pos[1] < top) {
                    sceVu0SubVector(d, c->a.pos, pos);
                    if (ee_sqrtf(d[2] * d[2] + d[0] * d[0]) < reach + AT(c, 0xC8, f32)) {
                        hit = 1;
                    }
                }
            }
            if (hit == 1) {
                /* (the original copies a request local whose last fields are never set) */
                if (c->state[0] != 7) {
                    c->state[0] = 4;
                    c->state[1] = 1;
                    c->state[2] = 0;
                    c->state[3] = arg;
                    c->state[4] = 0;
                    AT(c, 0x14FC, f32) = 0.0f;
                    c->state[6] = 0;
                    AT(c, 0x1504, u8) = 0;
                    AT(c, 0x1505, u8) = 0;
                    AT(c, 0x1506, u16) = 0;
                }
                mask |= 1 << c->a.slot;
            }
            break;
        }
    }
    return mask;
}

extern s8 D_0047A908[6];   /* chances (%) by the pursuer's health ratio */

/* when she is being chased (FI 0x1AD5D6), a roll against the chance for how worn the pursuer
 * is (+0x14C8 / +0x14CC: over 0.8, 0.6, 0.4, 0.2, 0.1, under) */
/* 0x00181650 */
s32 Fiona_ChaseRoll(Fiona *f) {
    s32 k;
    f32 r;

    if (FI(f, 0x1AD5D6, u8) != 1) {
        return 0;
    }
    r = (f32)AT(gCharPursuer, 0x14C8, s32) / (f32)AT(gCharPursuer, 0x14CC, s32);
    if (!(r <= 0x1.99999ap-1f /* 0.8 */)) {
        k = 0;
    } else if (!(r <= 0x1.333334p-1f /* 0.6 */)) {
        k = 1;
    } else if (!(r <= 0x1.99999ap-2f /* 0.4 */)) {
        k = 2;
    } else if (!(r <= 0x1.99999ap-3f /* 0.2 */)) {
        k = 3;
    } else if (!(r <= 0x1.99999ap-4f /* 0.1 */)) {
        k = 4;
    } else {
        k = 5;
    }
    if ((s8)(s32)(100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) < D_0047A908[k]) {
        return 1;
    }
    return 0;
}

extern s32 D_003B2520[][2];   /* Hewie's reactions: { cost, 1 in n chance (when within 30) } */

/* |the wrapped angle t| (the original wraps it twice) */
static inline __attribute__((always_inline)) f32 fiona_abs_wrap(f32 t) {
    if (!(Angle_Wrap(t) <= 0.0f)) {
        return Angle_Wrap(t);
    }
    return -Angle_Wrap(t);
}

/* slamming a door (doors +0x68, closing = 0 pushed / 1 pulled) shut as she runs through
 * toward `to`: the rumble and the door state; then who it shuts out - Hewie (2) when he's
 * with her and the door is between them, the pursuer (4) when she's being chased and he is
 * right behind it */
static inline __attribute__((always_inline)) void door_slam(Fiona *f, VObject *doors, u32 i, s32 pulled) {
    f->c.unk14D0 = 5;
    Motion_Freeze(f->c.motion);
    VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0x90, 8);
    VCALL(doors, 0x68, void (*)(VObject *, u32, s32))(doors, i, pulled);
}

/* run through (and slam) a door she faces (within 45 degrees; `ahead`: facing her way, else
 * looking back) on her way toward `to`, standing at its side 4 (push: an open door she may go
 * through) or 1 (pull: a closed one, sound 0x91); the progress is told who was shut out
 * (Relation_Request). 1 if anyone was */
/* 0x00181880 */
s32 Fiona_SlamDoor(Fiona *f, f32 *to, u8 ahead) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kHalfPi = {0x3FC90FDB},
                                         kQuarterPi = {0x3F490FDB};
    VObject *doors = gDoors;
    Progress *p = gProgress;
    u8 *nm = (u8 *)gNavMesh;
    u8 who = 0;
    u8 i;

    for (i = 0; i < 8; i++) {
        f32 facing, a;
        u32 tri;

        if ((VCALL(doors, 0x40, s32 (*)(VObject *, u32))(doors, i) & 0xFF) != 1) {
            continue;
        }
        if ((VCALL(doors, 0x30, s32 (*)(VObject *, u32))(doors, i) & 0xFF) != 1) {
            continue;
        }
        if ((VCALL(doors, 0x6C, s32 (*)(VObject *, s32, u32, f32 *))(doors, 4, i, f->c.a.pos) & 0xFF) == 1 &&
            (Progress_ExitOpen(p, f->c.a.room, i) & 0xFF) == 0) {
            facing = VCALL(doors, 0x3C, f32 (*)(VObject *, u32))(doors, i);
            if (ahead == 1) {
                a = f->c.a.angle[1];
            } else {
                a = Angle_Wrap(kPi.f + f->c.a.angle[1]);
            }
            facing = facing - a;
            if (fiona_abs_wrap(facing) < kQuarterPi.f && (Progress_ExitUnlocked(p, f->c.a.room, i) & 0xFF) == 0 &&
                (DoorHold_Usable(p, f->c.a.room, i) & 0xFF) == 0 &&
                (tri = Actor_TriTo(&f->c.a, to, 0)) != (u32)-1 && (fiona_tri_flags(nm, tri) & 0x20000) &&
                (DoorHold_Take(p, f->c.a.room, i, *(u8 *)&f->c.a.slot) & 0xFF) == 0) {
                u16 region;

                door_slam(f, doors, i, 0);
                region = VCALL(gRooms, 0x10, s32 (*)(VObject *, s32, u32))(gRooms, f->c.a.room, i);
                who = 0;
                if (FI(f, 0x1AD5D5, u8) == 1 && (PursuerGroup_Fields(p, i, 1) & 0x14) == 0x14) {
                    who |= 2;
                }
                if (FI(f, 0x1AD5D6, u8) == 1) {
                    if (FI(f, 0x1AD5D7, u8) == 1) {
                        if ((PursuerGroup_Fields(p, i, 2) & 0x14) == 0x14) {
                            who |= 4;
                        }
                    } else if ((func_00126EC0(gCharPursuer) & 0xFFFF) == region &&
                               Character_PathRemaining2(gCharPursuer) < 15.0f) {
                        who |= 4;
                    }
                }
                break;
            }
        }
        if ((VCALL(doors, 0x6C, s32 (*)(VObject *, s32, u32, f32 *))(doors, 1, i, f->c.a.pos) & 0xFF) == 1 &&
            (Progress_ExitOpen(p, f->c.a.room, i) & 0xFF) == 1) {
            facing = Angle_Wrap(VCALL(doors, 0x3C, f32 (*)(VObject *, u32))(doors, i) + kHalfPi.f);
            if (ahead == 1) {
                a = f->c.a.angle[1];
            } else {
                a = Angle_Wrap(kPi.f + f->c.a.angle[1]);
            }
            facing = facing - a;
            if (fiona_abs_wrap(facing) < kQuarterPi.f && (tri = Actor_TriTo(&f->c.a, to, 0)) != (u32)-1 &&
                (fiona_tri_flags(nm, tri) & 0x20000) &&
                (DoorHold_Take(p, f->c.a.room, i, *(u8 *)&f->c.a.slot) & 0xFF) == 0) {
                u32 s;

                Actor_PlaySound(&f->c.a, 0x91, 5, 0, 0, NULL);
                door_slam(f, doors, i, 1);
                who = 0;
                if (FI(f, 0x1AD5D5, u8) == 1) {
                    s = PursuerGroup_Fields(p, i, 1) & 0xFF;
                    if ((s & 4) && (s & 0x18)) {
                        who |= 2;
                    }
                }
                if (FI(f, 0x1AD5D7, u8) == 1) {
                    s = PursuerGroup_Fields(p, i, 2) & 0xFF;
                    if ((s & 4) && (s & 0x18)) {
                        who |= 4;
                    }
                }
                break;
            }
        }
    }
    if (who == 0) {
        return 0;
    }
    Relation_Request(p, *(u8 *)&f->c.a.slot, who, 5, 3, i, 0.0f);
    return 1;
}

/* her panic's recovery delay (FI 0x1AD5F8) back to 1800 frames - with accessory 0x8C never,
 * 0x8B half the time, 0x8A three times in four */
/* 0x00182E80 */
void Fiona_ResetRecovery(Fiona *f) {
    if (gSubScreen != NULL) {
        switch (VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 3)) {
        case 0x8D:
            break;
        case 0x8C:
            return;
        case 0x8B:
            if (!(VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 0.5f)) {
                return;
            }
            break;
        case 0x8A:
            if (!(VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 0.75f)) {
                return;
            }
            break;
        }
    }
    FI(f, 0x1AD5F8, s32) = 0x708;
}

static inline void fiona_voice(Fiona *f, s32 id) {
    Actor_PlaySound(&f->c.a, id, 5, 0, 0, NULL);
}

static inline f32 fiona_rnd(void) {
    return VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
}

/* her line for an order (moveSub 0x2C: 0x31..0x33 by the game mode, heard as a noise unless
 * Hewie is with her; 0x2D: 0x30) - once flag 0x25 is set, just 0x33 or 0x39 */
/* 0x00183780 */
void Fiona_OrderLine(Fiona *f) {
    Progress *p = gProgress;

    if (Progress_TestFlag(p, 0x25) & 0xFF) {
        if (fiona_rnd() < 0.5f) {
            fiona_voice(f, 0x33);
        } else {
            fiona_voice(f, 0x39);
        }
        return;
    }
    switch (f->c.moveSub) {
    case 0x2C:
        switch (Progress_GameMode(p) & 0xFF) {
        case 2:
            fiona_voice(f, 0x33);
            break;
        case 1:
            fiona_voice(f, 0x31);
            break;
        case 0:
            fiona_voice(f, 0x32);
            break;
        }
        if (FI(f, 0x1AD5D5, u8) == 0) {
            Noise_Make((u8 *)p + 0x778, 0x20, f->c.a.room, f->c.a.navTri, 0xFFFF);
        }
        break;
    case 0x2D:
        fiona_voice(f, 0x30);
        break;
    }
}

/* her call to Hewie for the command (moveSub 0x23..0x2F); for 0x27 (praise / scold) by what he
 * is doing (his action, Hewie_AnimGroup) */
/* 0x00183960 */
void Fiona_CallHewie(Fiona *f) {
    switch (f->c.moveSub) {
    case 0x23:
        if ((Progress_GameMode(gProgress) & 0xFF) == 0) {
            fiona_voice(f, 0x2E);
        } else {
            fiona_voice(f, 0x2F);
        }
        break;
    case 0x24:
        fiona_voice(f, 0x37);
        break;
    case 0x25:
        fiona_voice(f, 0x34);
        break;
    case 0x26:
        break;
    case 0x27: {
        Character *h = (Character *)gCharPartner;
        s32 act = AT(h, 0xF3564, s32);
        u8 done = 0;

        switch (act) {
        case 0x22: case 0x20: case 0x21: case 0x1F: case 0x59: case 0x53:
            if ((u32)(Hewie_AnimGroup((Hewie *)h) - 8) < 2) {
                done = 1;
                fiona_voice(f, 0x36);
            }
            break;
        }
        if (done == 0) {
            switch (act) {
            case 0x25: case 0x24: case 0x63: case 0x13: case 0xA: case 0x22: case 0x20: case 0x21: case 0x1F:
                done = 1;
                fiona_voice(f, 0x36);
                break;
            }
        }
        if (done == 0 && act == 8) {
            done = 1;
            fiona_voice(f, 0x35);
        }
        if (done == 0 && h->moveMode == 0 && h->moveSub == 3) {
            done = 1;
            fiona_voice(f, 0x35);
        }
        if (done == 0) {
            if (fiona_rnd() < 0.5f) {
                fiona_voice(f, 0x35);
            } else {
                fiona_voice(f, 0x36);
            }
        }
        break;
    }
    case 0x28:
        if (fiona_rnd() < 0.75f) {
            fiona_voice(f, 0x3A);
        } else {
            fiona_voice(f, 0x32);
        }
        break;
    case 0x29:
        if (AT(gCharPartner, 0xC4, s32) == 2) {
            fiona_voice(f, 0x33);
        } else {
            fiona_voice(f, 0x3A);
        }
        break;
    case 0x2A:
        fiona_voice(f, 0x39);
        break;
    case 0x2B:
        if (fiona_rnd() < 0.75f) {
            fiona_voice(f, 0x3B);
        } else {
            fiona_voice(f, 0x30);
        }
        break;
    case 0x2C:
        fiona_voice(f, 0x31);
        if (FI(f, 0x1AD5D5, u8) == 0) {
            Noise_Make((u8 *)gProgress + 0x778, 0x20, f->c.a.room, f->c.a.navTri, 0xFFFF);
        }
        break;
    case 0x2D:
        if ((Progress_GameMode(gProgress) & 0xFF) == 2) {
            fiona_voice(f, 0x2F);
        } else {
            fiona_voice(f, 0x30);
        }
        break;
    case 0x2E:
        fiona_voice(f, 0x2F);
        break;
    case 0x2F:
        fiona_voice(f, 0x3B);
        break;
    }
}

/* Hewie, when he's with her (FI 0x1AD5D5), reacts to what she did (n) */
static inline __attribute__((always_inline)) void hewie_react(Fiona *f, s32 n) {
    Character *h;

    if (FI(f, 0x1AD5D5, u8) != 1) {
        return;
    }
    h = (Character *)gCharPartner;
    Hewie_SpendPool((Hewie *)h, D_003B2520[n][0]);
    if (D_003B2520[n][1] > 0 && Actor_Distance(&f->c.a, h->a.pos) < 30.0f) {
        Hewie_Chance((Hewie *)h, D_003B2520[n][1]);
    }
}

/* 0x001817C0 */
void Fiona_HewieReact(Fiona *f, s32 n) {
    hewie_react(f, n);
}

/* her command to Hewie (moveSub 0x23..0x30): when he is free (no request) and flag 0x25 isn't
 * set, the request 0xD with it, her point (FI 0x1AD6E0, x and z in 1/100000; for "come", 0x23,
 * moved to 5 short of where he'd have to go when that is further) and facing (FI 0x1AD6D0) -
 * unless the game holds him (gProgress +0x1FBEC1); then his reaction to it */
/* 0x00183F10 */
void Fiona_CommandHewie(Fiona *f) {
    Character *h = (Character *)gCharPartner;
    Progress *p;

    if (h->state[0] == 0 && !((p = gProgress, Progress_TestFlag(p, 0x25)) & 0xFF)) {
        if (f->c.moveSub == 0x23) {
            f32 d = Actor_FreeDistance(&h->a, FI(f, 0x1AD6C0, u32), (f32 *)((u8 *)f + 0x1AD6E0), (u32)-1,
                                  FI(f, 0x1AD6D0, f32), 15.0f);

            if (!(d <= 5.0f)) {
                f32 m[4][4] __attribute__((aligned(16)));
                f32 v[4] __attribute__((aligned(16)));
                f32 at[4] __attribute__((aligned(16)));
                u32 tri;

                v[0] = 0.0f;
                v[1] = 0.0f;
                v[2] = d - 5.0f;
                Mtx_AtHeading(m, (f32 *)((u8 *)f + 0x1AD6E0), FI(f, 0x1AD6D0, f32));
                Mtx_ApplyPoint(at, m, v);
                tri = Actor_TriTo(&f->c.a, at, 0x29020008);
                if (tri != (u32)-1) {
                    FI(f, 0x1AD6C0, u32) = tri;
                    sceVu0CopyVector((f32 *)((u8 *)f + 0x1AD6E0), at);
                }
            }
        }
        if (AT(p, 0x1FBEC1, u8) == 0) {
            s32 x = (s32)(100000.0f * FI(f, 0x1AD6E0, f32));
            s32 z = (s32)(100000.0f * FI(f, 0x1AD6E8, f32));

            /* (the original copies a request local whose last fields are never set) */
            h = (Character *)gCharPartner;
            if (h->state[0] != 7) {
                h->state[0] = 0xD;
                h->state[1] = f->c.moveSub;
                h->state[2] = x;
                h->state[3] = z;
                h->state[4] = FI(f, 0x1AD6C0, s32);
                AT(h, 0x14FC, f32) = FI(f, 0x1AD6D0, f32);
                h->state[6] = 0;
                AT(h, 0x1504, u8) = 0;
                AT(h, 0x1505, u8) = 0;
                AT(h, 0x1506, u16) = 0;
            }
        }
    }
    switch (f->c.moveSub) {
    case 0x2D:
    case 0x23:
        hewie_react(f, 0);
        break;
    case 0x2C:
        hewie_react(f, 1);
        break;
    case 0x27:
    case 0x25:
        hewie_react(f, 2);
        break;
    case 0x2F:
        hewie_react(f, 3);
        break;
    case 0x29:
        hewie_react(f, 4);
        break;
    case 0x30:
        hewie_react(f, 14);
        break;
    case 0x2E:
        hewie_react(f, 15);
        break;
    }
}

extern const PTMF D_003B2D28;   /* the flee turn */

/* D_003B2D28: the flee turn (anim 0x403) - turning to savedYaw (12 degrees a frame) and moving
 * by the root motion; at its mark 2 (unless the progress var 0x26 is 8) her panic +10, at mark
 * 0x20 a door slammed behind her (her radius back); at its event 0x20 Hewie's reaction 9 and
 * idle */
/* 0x0018C240 */
void Fiona_StateFleeTurn(Fiona *f) {
    static const union { u32 u; f32 f; } kTwelveDeg = {0x3E567750};
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 to[4] __attribute__((aligned(16)));

    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        hewie_react(f, 9);
        door_give_up(f, gProgress);
        return;
    }
    if ((Motion_EventFlags(f->c.motion, 0, 0, 1) & 0xFF) & 2) {
        f->c.moveMode = 0;
        f->c.a.unk2D = 0;
        if ((Progress_GetVar(gProgress, 0x26) & 0xFF) != 8) {
            Fiona_AddPanic(f, 10.0f);
        }
    }
    if ((Motion_EventFlags(f->c.motion, 0, 0, 1) & 0xFF) & 0x20) {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = -AT(f, 0xC8, f32);
        v[3] = 0.0f;   /* (unset in the original) */
        Mtx_ApplyVector(v, (f32 (*)[4])((u8 *)f + 0x60), v);
        sceVu0AddVector(to, f->c.a.pos, v);
        Fiona_SlamDoor(f, to, 0);
    }
    Actor_TurnToward(&f->c.a, f->savedYaw, kTwelveDeg.f);
    Motion_RootMovement(f->c.motion, v, 0.0f);
    Mtx_TurnY(m, f->savedYaw);
    Mtx_ApplyVector(v, m, v);
    Actor_Move(&f->c.a, v);
}

/* D_003B2678: once the animation has run out the flee turn (D_003B2D28) - away from the
 * pursuer when she's being chased and he is within 30, else the way she faces */
/* 0x0018C540 */
void Fiona_StateFleeStart(Fiona *f) {
    Character_RootMoveMasked(&f->c);
    if (!door_anim_done(f)) {
        return;
    }
    f->savedYaw = f->c.a.angle[1];
    if (FI(f, 0x1AD5D7, u8) == 1 && Actor_Distance(&f->c.a, gCharPursuer->a.pos) < 30.0f) {
        f->savedYaw = Actor_HeadingTo(&f->c.a, gCharPursuer->a.pos);
    }
    Motion_PlayTable(f->c.motion, 0x403, -1);
    Actor_SetState(&f->c.a, &D_003B2D28);
}

/* her hand: where the thing leaves it (motion +0x78 the bone) */
static inline __attribute__((always_inline)) void fiona_hand(Fiona *f, f32 *out) {
    s32 bone = VCALL(f->c.motion, 0x78, s32 (*)(void *))(f->c.motion);

    sceVu0CopyVector(out, Skel_Bone(AT(f->c.motion, 0x810, void *), bone) + 12);
}

/* D_003B2D18: letting go of an item (moveSub 0x31 throw, 0x32 drop, 0x33 set down) while
 * turning to FI 0x1AD6D0: idle at the animation's event 0x20; at its mark 2 the thing (kind
 * unk100) made at her hand and sent off (+0x44: thrown 4, dropped 6 - toward FI 0x1AD6E0 when
 * aimed (FI 0x1AD6C0), else her facing; set down: straight down). A thrown ball (kind 0) Hewie
 * may fetch (+0xF368C), after his reaction 12 */
/* 0x0018C660 */
void Fiona_StateLetGo(Fiona *f) {
    static const union { u32 u; f32 f; } kFiveDeg = {0x3DB2B8C3};
    VObject *things;
    u8 *t;
    f32 hand[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));

    Actor_TurnToward(&f->c.a, FI(f, 0x1AD6D0, f32), kFiveDeg.f);
    switch (f->c.moveSub) {
    case 0x31:
    case 0x32:
        if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
            door_give_up(f, gProgress);
        }
        if (!((Motion_EventFlags(f->c.motion, 0, -1, 1) & 0xFF) & 2)) {
            break;
        }
        things = gPlacedThings;
        if (things == NULL) {
            break;
        }
        if (f->c.moveSub == 0x31) {
            VCALL(things, 0x14, void (*)(VObject *, s32))(things, f->c.unk100);
        }
        t = VCALL(things, 0x8, u8 *(*)(VObject *, s32))(things, f->c.unk100);
        if (t == NULL) {
            break;
        }
        VCALL((VObject *)t, 0xC, void (*)(u8 *))(t);
        fiona_hand(f, hand);
        if (FI(f, 0x1AD6C0, s32) == 0) {
            sceVu0CopyVector(dir, (f32 *)((u8 *)f + 0x50));
        } else {
            f32 d[4] __attribute__((aligned(16)));

            sceVu0SubVector(d, (f32 *)((u8 *)f + 0x1AD6E0), hand);
            d[1] = 0.0f;
            dir[0] = 0.0f;
            dir[1] = func_0031C5C0(d[0], d[2]);
            dir[2] = 0.0f;
            dir[3] = 1.0f;
        }
        AT(t, 0x28, u8) = VCALL((VObject *)t, 0x44, s32 (*)(u8 *, Fiona *, f32 *, f32 *, f32, f32))(
            t, f, hand, dir, f->c.moveSub == 0x31 ? 4.0f : 6.0f, 0.0f);
        if (f->c.moveSub == 0x31) {
            Character *h;

            hewie_react(f, 12);
            if (AT(t, 0x28, u8) == 1 && AT(t, 0x20, s32) == 0 && (Progress_GameMode(gProgress) & 0xFF) == 0 &&
                (h = (Character *)gCharPartner) != NULL && h->a.active == 1 && h->a.disabled == 0 &&
                AT(h, 0xF3564, s32) != 0x78) {
                AT(h, 0xF368C, u8 *) = t;
            }
        }
        break;
    case 0x33:
        if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
            door_give_up(f, gProgress);
        }
        if (!((Motion_EventFlags(f->c.motion, 0, -1, 1) & 0xFF) & 2)) {
            break;
        }
        things = gPlacedThings;
        if (things == NULL) {
            break;
        }
        VCALL(things, 0x14, void (*)(VObject *, s32))(things, f->c.unk100);
        t = VCALL(things, 0x8, u8 *(*)(VObject *, s32))(things, f->c.unk100);
        if (t == NULL) {
            break;
        }
        VCALL((VObject *)t, 0xC, void (*)(u8 *))(t);
        dir[0] = 0.0f;
        dir[1] = -1.0f;
        dir[2] = 0.0f;
        dir[3] = 1.0f;
        fiona_hand(f, hand);
        AT(t, 0x28, u8) = VCALL((VObject *)t, 0x44, s32 (*)(u8 *, Fiona *, f32 *, f32 *, f32, f32))(
            t, f, hand, dir, 0.0f, 0.0f);
        break;
    }
    Character_RootMoveMasked(&f->c);
}

extern const PTMF D_003B2D18, D_003B2CF8, D_003B2D08;

/* D_003B2688 (moveSub 0x31..0x33, an item to let go): once the animation is done, her
 * facing kept (FI 0x1AD6D0) and the animation for it (0xD00 / 0xD01 / 0xD02) - for 0x32 aimed
 * at the pursuer instead (FI 0x1AD6E0, FI 0x1AD6C0 set) when she's being chased and he is within
 * 80 and 60 degrees of her facing; then D_003B2D18 */
/* 0x0018CCA0 */
void Fiona_StateLetGoStart(Fiona *f) {
    static const union { u32 u; f32 f; } kSixtyDeg = {0x3F860A92};

    if (door_anim_done(f)) {
        switch (f->c.moveSub) {
        case 0x31:
            FI(f, 0x1AD6D0, f32) = f->c.a.angle[1];
            Motion_PlayTable(f->c.motion, 0xD00, -1);
            break;
        case 0x32:
            FI(f, 0x1AD6D0, f32) = f->c.a.angle[1];
            if (FI(f, 0x1AD5D7, u8) == 1 && Actor_Distance(&f->c.a, gCharPursuer->a.pos) < 80.0f) {
                f32 yaw = Actor_HeadingTo(&f->c.a, gCharPursuer->a.pos);

                if (fiona_abs_wrap(yaw - f->c.a.angle[1]) < kSixtyDeg.f) {
                    sceVu0CopyVector((f32 *)((u8 *)f + 0x1AD6E0), gCharPursuer->a.pos);
                    FI(f, 0x1AD6C0, s32) = 1;
                    FI(f, 0x1AD6D0, f32) = yaw;
                }
            }
            Motion_PlayTable(f->c.motion, 0xD01, -1);
            break;
        case 0x33:
            FI(f, 0x1AD6D0, f32) = f->c.a.angle[1];
            Motion_PlayTable(f->c.motion, 0xD02, -1);
            break;
        }
        Actor_SetState(&f->c.a, &D_003B2D18);
    }
    Character_RootMoveMasked(&f->c);
}

/* D_003B2D08: idle at the animation's event 0x20 */
/* 0x0018CED0 */
void Fiona_StateIdleAtEvent(Fiona *f) {
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        door_give_up(f, gProgress);
    }
}

/* D_003B2CF8: turning to FI 0x1AD6D0 (FI 0x1AD6D4 a frame) until the animation is done, then
 * set to it (D_003B2D08) */
/* 0x0018D010 */
void Fiona_StateTurnUntilDone(Fiona *f) {
    f32 yaw;

    if (!door_anim_done(f)) {
        Actor_TurnToward(&f->c.a, FI(f, 0x1AD6D0, f32), FI(f, 0x1AD6D4, f32));
        return;
    }
    yaw = FI(f, 0x1AD6D0, f32);
    f->c.a.angle[1] = yaw;
    sceVu0UnitMatrix((f32 (*)[4])((u8 *)f + 0x60));
    sceVu0RotMatrixY((f32 (*)[4])((u8 *)f + 0x60), (f32 (*)[4])((u8 *)f + 0x60), yaw);
    Actor_SetState(&f->c.a, &D_003B2D08);
}

/* D_003B25E8 (request 0xC / 6): animation 0x8000 (8, speed 10) while she turns to unk104[2]
 * (a tenth of the way a frame; D_003B2CF8) */
/* 0x0018D100 */
void Fiona_StateRequestTurn(Fiona *f) {
    Motion_PlayWith(f->c.motion, 0x8000, 8, -1, 10.0f);
    Actor_SetState(&f->c.a, &D_003B2CF8);
    FI(f, 0x1AD6D0, f32) = *(f32 *)&f->c.unk104[2];
    FI(f, 0x1AD6D4, f32) = 0x1.99999ap-4f /* 0.1 */ * fiona_abs_wrap(*(f32 *)&f->c.unk104[2] - f->c.a.angle[1]);
}

extern const PTMF D_003B2CE8;

/* she is touching the pursuer while being chased */
static inline __attribute__((always_inline)) s32 fiona_caught(Fiona *f) {
    return FI(f, 0x1AD5D7, u8) == 1 && (Actor_Touching(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) == 1;
}

/* looking at the command's target (FI 0x1AD664), if any */
static inline __attribute__((always_inline)) void gesture_look(Fiona *f) {
    if (FI(f, 0x1AD664, s32) != 0) {
        FI(f, 0x1AD5FC, u8) = 1;
        FI(f, 0x1AD600, s32) = FI(f, 0x1AD664, s32);
    }
}

/* D_003B2798 / D_003B2928 / D_003B2938: idle at the animation's event 0x20 */
/* 0x0018D200 */
void Fiona_StateIdleAtEvent2(Fiona *f) {
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        door_give_up(f, gProgress);
    }
    Character_RootMoveMasked(&f->c);
}

/* D_003B2CE8: the end of a command gesture - idle at its event 0x20, or at once if the pursuer
 * has her */
/* 0x0018D340 */
void Fiona_StateGestureEnd(Fiona *f) {
    if (fiona_caught(f)) {
        door_give_up(f, gProgress);
        return;
    }
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        door_give_up(f, gProgress);
    }
    Character_RootMoveMasked(&f->c);
}

/* D_003B2CD8: a command gesture - looking at its target (FI 0x1AD664) if any; at its event 0x20
 * the command given (Fiona_CommandHewie) and the gesture's end (0xC0C, D_003B2CE8); idle if the
 * pursuer has her */
/* 0x0018D5C0 */
void Fiona_StateGesture(Fiona *f) {
    if (fiona_caught(f)) {
        door_give_up(f, gProgress);
        return;
    }
    gesture_look(f);
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        Fiona_CommandHewie(f);
        Motion_Play(f->c.motion, 0xC0C, -1);
        Actor_SetState(&f->c.a, &D_003B2CE8);
    }
    Character_RootMoveMasked(&f->c);
}

extern const PTMF D_003B2CC8, D_003B2CD8;

/* D_003B2CC8: the gesture's lead-in - at its event 0x20 the gesture (0xC0B, D_003B2CD8) */
/* 0x0018D7E0 */
void Fiona_StateGestureLeadIn(Fiona *f) {
    if (fiona_caught(f)) {
        door_give_up(f, gProgress);
        return;
    }
    gesture_look(f);
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        Motion_Play(f->c.motion, 0xC0B, -1);
        Actor_SetState(&f->c.a, &D_003B2CD8);
    }
    Character_RootMoveMasked(&f->c);
}

/* D_003B2B78: walking on until the animation is done, then the gesture's lead-in (0xC0A,
 * D_003B2CC8) */
/* 0x0018DA00 */
void Fiona_StateWalkThenGesture(Fiona *f) {
    if (fiona_caught(f)) {
        door_give_up(f, gProgress);
        return;
    }
    gesture_look(f);
    if (!door_anim_done(f)) {
        Fiona_WalkLook(f);
        return;
    }
    Motion_PlayTable(f->c.motion, 0xC0A, -1);
    Actor_SetState(&f->c.a, &D_003B2CC8);
}

/* D_003B2CB8: a held command (moveSub 0x28: 0xC07 then 0xC08 repeated FI 0x1AD6C0 times -
 * 3 while Hewie answers it (his moveMode 0xC, normal mode), cut to 2 if he does once it plays -
 * then 0xC09; 0x2B / 0x24 one gesture): at each animation's event 0x20 the next; at the end
 * Hewie's reaction (6 / 5 / 7) and idle */
/* 0x0018DC30 */
void Fiona_StateHeldCommand(Fiona *f) {
    Progress *p = gProgress;

    if ((Progress_GameMode(p) & 0xFF) == 0 && f->c.moveSub == 0x28 && FI(f, 0x1AD6B8, s32) == 3 &&
        FI(f, 0x1AD5D5, u8) == 1 && ((Character *)gCharPartner)->moveMode == 0xC &&
        AT(f->c.motion, 0x55C, s32) == 0xC08) {
        FI(f, 0x1AD6C0, s32) = 2;
    }
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        switch (f->c.moveSub) {
        case 0x28:
            switch (AT(f->c.motion, 0x55C, s32)) {
            case 0xC07:
                FI(f, 0x1AD6C0, s32) = 1;
                if ((Progress_GameMode(p) & 0xFF) == 0 && FI(f, 0x1AD5D5, u8) == 1 &&
                    ((Character *)gCharPartner)->moveMode == 0xC) {
                    FI(f, 0x1AD6C0, s32) = 3;
                }
                Motion_Play(f->c.motion, 0xC08, -1);
                break;
            case 0xC08:
                FI(f, 0x1AD6C0, s32) -= 1;
                if (FI(f, 0x1AD6C0, s32) == 0) {
                    Motion_Play(f->c.motion, 0xC09, -1);
                } else {
                    Motion_Play(f->c.motion, 0xC08, -1);
                }
                break;
            case 0xC09:
                hewie_react(f, 6);
                door_give_up(f, p);
                break;
            }
            break;
        case 0x2B:
            hewie_react(f, 5);
            door_give_up(f, p);
            break;
        case 0x24:
            hewie_react(f, 7);
            door_give_up(f, p);
            break;
        }
    }
    Character_RootMoveMasked(&f->c);
}

extern const PTMF D_003B2C78, D_003B2C88, D_003B2C98, D_003B2CA8, D_003B2CB8;

/* D_003B2C78 / D_003B2C88 / D_003B2C98: the held command's gesture (0x2B 0xC0D, 0x28 0xC07,
 * 0x24 0xC06; D_003B2CB8) - Hewie gone: idle; a request 7 for her: cleared, the wait
 * (D_003B2CA8) */
/* 0x0018E2B0 */
void Fiona_StateHeldGesture(Fiona *f) {
    if (f->c.state[0] == 7) {
        f->c.state[0] = 0;
        Actor_SetState(&f->c.a, &D_003B2CA8);
        return;
    }
    if (FI(f, 0x1AD5D5, u8) == 0) {
        Character_RootMoveMasked(&f->c);
        door_give_up(f, gProgress);
        return;
    }
    switch (f->c.moveSub) {
    case 0x2B:
        Motion_PlayTable(f->c.motion, 0xC0D, -1);
        break;
    case 0x28:
        Motion_PlayTable(f->c.motion, 0xC07, -1);
        break;
    case 0x24:
        Motion_PlayTable(f->c.motion, 0xC06, -1);
        break;
    }
    Actor_SetState(&f->c.a, &D_003B2CB8);
}

/* D_003B2C68 / D_003B2CA8: waiting for Hewie to be ready for a held command (his action 0x48,
 * Hewie_AnimGroup 1, the animation done); then the command registered with the progress
 * (SlotCmd_Give 2, 0x2B: 1 / 0x28: 3 / 0x24: 5) and its gesture; Hewie gone, busy otherwise,
 * or the command refused: idle */
/* 0x0018E510 */
void Fiona_StateWaitHewie(Fiona *f) {
    s32 b;
    const PTMF *next;

    if (FI(f, 0x1AD5D5, u8) == 0 || AT(gCharPartner, 0xF3564, s32) != 0x48) {
        Character_RootMoveMasked(&f->c);
        door_give_up(f, gProgress);
        return;
    }
    if (!door_anim_done(f)) {
        Character_RootMoveMasked(&f->c);
        return;
    }
    if (Hewie_AnimGroup((Hewie *)((Character *)gCharPartner)) != 1 || !door_anim_done(f)) {
        return;
    }
    switch (f->c.moveSub) {
    case 0x2B:
        b = 1;
        next = &D_003B2C78;
        break;
    case 0x28:
        b = 3;
        next = &D_003B2C88;
        break;
    case 0x24:
        b = 5;
        next = &D_003B2C98;
        break;
    default:
        Character_RootMoveMasked(&f->c);
        door_give_up(f, gProgress);
        return;
    }
    if ((SlotCmd_Give(gProgress, 2, b, *(u8 *)&f->c.a.slot, 1, 0, 0.0f) & 0xFF) == 1) {
        Actor_SetState(&f->c.a, next);
        return;
    }
    Character_RootMoveMasked(&f->c);
    door_give_up(f, gProgress);
}

extern const PTMF D_003B2C58, D_003B2C68;

/* Hewie gone or no longer waiting for a held command (his action 0x48) */
static inline __attribute__((always_inline)) s32 held_off(Fiona *f) {
    return FI(f, 0x1AD5D5, u8) == 0 || AT(gCharPartner, 0xF3564, s32) != 0x48;
}

/* D_003B2C58: the animation run out, the wait for Hewie (D_003B2C68); idle if he's off */
/* 0x0018EAC0 */
void Fiona_StateHeldStopped(Fiona *f) {
    if (held_off(f)) {
        Character_RootMoveMasked(&f->c);
        door_give_up(f, gProgress);
        return;
    }
    if (door_anim_done(f)) {
        Actor_SetState(&f->c.a, &D_003B2C68);
    }
}

/* D_003B2C48: walking to the spot for a held command; there, stopped (D_003B2C58); idle if
 * Hewie is off or she can't get there */
/* 0x0018EDA0 */
void Fiona_StateHeldWalk(Fiona *f) {
    s32 r;

    if (held_off(f)) {
        Character_RootMoveMasked(&f->c);
        door_give_up(f, gProgress);
        return;
    }
    r = Fiona_DoorFrame(f);
    if (r < 0) {
        Character_RootMoveMasked(&f->c);
        door_give_up(f, gProgress);
        return;
    }
    if (r != 0) {
        return;
    }
    Fiona_IdleAnim(f, -1);
    Actor_SetState(&f->c.a, &D_003B2C58);
}

extern const PTMF D_003B2C28, D_003B2C38, D_003B2C48;

/* D_003B2BB8 / D_003B2BC8 / D_003B2BE8 (a held command, Hewie at unk110 / tri unk104[0]
 * facing unk104[2]): a request 7 for her switches it (0x2B: 0x2F, D_003B2C28; 0x28: 0x29,
 * D_003B2C38; 0x24: idle); Hewie gone: idle; else walk to his spot facing him (D_003B2C48) */
/* 0x0018F180 */
void Fiona_StateHeldStart(Fiona *f) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 yaw;
    s32 tri;

    if (f->c.state[0] == 7) {
        f->c.state[0] = 0;
        Character_RootMoveMasked(&f->c);
        switch (f->c.moveSub) {
        case 0x2B:
            f->c.moveSub = 0x2F;
            Actor_SetState(&f->c.a, &D_003B2C28);
            break;
        case 0x28:
            f->c.moveSub = 0x29;
            Actor_SetState(&f->c.a, &D_003B2C38);
            break;
        case 0x24:
            door_give_up(f, gProgress);
            break;
        }
        return;
    }
    if (FI(f, 0x1AD5D5, u8) == 0) {
        Character_RootMoveMasked(&f->c);
        door_give_up(f, gProgress);
        return;
    }
    yaw = Angle_Wrap(kPi.f + *(f32 *)&f->c.unk104[2]);
    tri = f->c.unk104[0];
    f->c.unk124 = f->c.unk128;
    FI(f, 0x1AD650, s32) = 0;
    FI(f, 0x1AD634, s32) = tri;
    sceVu0CopyVector((f32 *)((u8 *)f + 0x1AD640), f->c.unk110);
    VCALL(gNavMesh, 0x14, void (*)(NavMesh *, s32, f32 *))(gNavMesh, tri, (f32 *)((u8 *)f + 0x1AD640));
    f->savedYaw = yaw;
    Actor_SetState(&f->c.a, &D_003B2C48);
}

extern const PTMF D_003B2B28, D_003B2B38;
extern void Fiona_AlongWall(Fiona *f);

/* D_003B2B28 (fallen): sliding on by the root motion turned to FI 0x1AD6D0 (and turning to
 * it, 20 degrees a frame, until there: FI 0x1AD6C4 1) - not while FI 0x1AD6C4 is -1; at the
 * animation's event 0x20 getting up (0xB01, D_003B2B38). Blocked by nothing for the check
 * (+0xC0 bit 0); when no exit is closed to her (bit 0 of every door's state) Fiona_AlongWall */
/* 0x001906A0 */
void Fiona_StateFallen(Fiona *f) {
    static const union { u32 u; f32 f; } kTwentyDeg = {0x3EB2B8C3};
    Progress *p;
    u8 i;

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        f->c.a.unk2D = 0;
        FI(f, 0x1AD6C0, s32) = 0;
        Motion_PlayTable(f->c.motion, 0xB01, -1);
        f->c.moveMode = 0xA;
        f->unk1AD580 = 0xB;
        FI(f, 0x1AD710, u8) = 1;
        FI(f, 0x1AD714, s32) = 0;
        Actor_SetState(&f->c.a, &D_003B2B38);
    }
    AT(f, 0xC0, u32) |= 1;
    if (FI(f, 0x1AD6C4, s32) == -1) {
        Character_RootMoveMasked(&f->c);
    } else {
        f32 m[4][4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));

        Motion_RootMovement(f->c.motion, v, 0.0f);
        sceVu0UnitMatrix(m);
        sceVu0RotMatrixY(m, m, FI(f, 0x1AD6D0, f32));
        sceVu0ApplyMatrix(v, m, v);
        Actor_Move(&f->c.a, v);
        if (FI(f, 0x1AD6C4, s32) == 0 &&
            Actor_TurnToward(&f->c.a, FI(f, 0x1AD6D0, f32), kTwentyDeg.f) == 0.0f) {
            FI(f, 0x1AD6C4, s32) = 1;
        }
    }
    p = gProgress;
    AT(f, 0xC0, u32) &= ~1;
    for (i = 0; i < 8; i++) {
        if ((PursuerGroup_Fields(p, i, *(u8 *)&f->c.a.slot) & 0xFF) & 1) {
            return;
        }
    }
    Fiona_AlongWall(f);
}

/* D_003B2788 (panic: fall): once the animation is done the fall itself (0xB00, her cry 0x40, a
 * loud noise 0x5F; D_003B2B28) facing on */
/* 0x001908E0 */
void Fiona_StatePanicFall(Fiona *f) {
    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    if (door_anim_done(f)) {
        FI(f, 0x1AD6C0, s32) = -1;
        FI(f, 0x1AD6C4, s32) = -1;
        FI(f, 0x1AD6D0, f32) = f->c.a.angle[1];
        Motion_PlayTable(f->c.motion, 0xB00, -1);
        Actor_PlaySound(&f->c.a, 0x40, 5, 0, 0, NULL);
        Noise_Make((u8 *)gProgress + 0x778, 0x5F, f->c.a.room, f->c.a.navTri, 0xFFFF);
        Actor_SetState(&f->c.a, &D_003B2B28);
    }
    Character_RootMoveMasked(&f->c);
}

extern const PTMF D_003B2988;   /* the kick */

/* D_003B2988: the kick (0xE01). A hit taken back (gProgress +0x1020 + 16 x her slot: 4 / the
 * creatures hit last frame FI 0x1AD6C8 rumble; 2 the kick count +0xFB6 +3, to 10000) - her cry
 * 0x90, recoil; the hits so far gathered (FI 0x1AD6C0 who, FI 0x1AD6CC creatures). While no
 * recoil she steps forward until the animation is done. In its hit window (motion flags bit 2
 * clear): a door ahead slammed, else a blow (2, damage 5 x gProgress +0xA04, the pursuer worn
 * down Fiona_ChaseRoll -0x8000) to Hewie (2, once) and the pursuer (4, once) when touching, and to
 * the creatures (FI 0x1AD6C8). At its event 0x20 Hewie's reaction 11 (if he wasn't kicked),
 * panic +10, idle */
/* 0x001943D0 */
void Fiona_StateKick(Fiona *f) {
    static const union { u32 u; f32 f; } kStep = {0x3F89B08A};
    Progress *p = gProgress;
    u8 hit = AT(p, 0x1020 + *(u8 *)&f->c.a.slot * 16, u8);
    f32 v[4] __attribute__((aligned(16)));

    if (hit != 0 || FI(f, 0x1AD6C8, s32) != 0) {
        if ((hit & 4) || FI(f, 0x1AD6C8, s32) != 0) {
            VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 0, 0xC0, 0xC);
        }
        if (hit & 2) {
            AT(p, 0xFB6, s16) = AT(p, 0xFB6, s16) + 3;
            if (AT(p, 0xFB6, s16) < 0) {
                AT(p, 0xFB6, s16) = 0;
            } else if (!(AT(p, 0xFB6, s16) < 0x2711)) {
                AT(p, 0xFB6, s16) = 0x2710;
            }
        }
        Actor_PlaySound(&f->c.a, 0x90, 5, 0, 0, NULL);
        f->c.unk14D0 = 5;
        Motion_Freeze(f->c.motion);
        FI(f, 0x1AD6C0, s32) |= hit;
        FI(f, 0x1AD6CC, s32) |= FI(f, 0x1AD6C8, s32);
        FI(f, 0x1AD6C8, s32) = 0;
    }
    if (f->c.unk14D0 == 0) {
        if (!door_anim_done(f)) {
            v[0] = 0.0f;
            v[1] = 0.0f;
            v[2] = kStep.f;
            v[3] = 0.0f;   /* (unset in the original) */
            sceVu0ApplyMatrix(v, (f32 (*)[4])((u8 *)f + 0x60), v);
            Actor_Move(&f->c.a, v);
        } else {
            Character_RootMoveMasked(&f->c);
        }
    }
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        if (FI(f, 0x1AD6C4, s32) == 0) {
            hewie_react(f, 11);
        }
        Fiona_AddPanic(f, 10.0f);
        FI(f, 0x1AD5C4, s32) = 0;
        door_give_up(f, p);
        return;
    }
    if ((Motion_EventFlags(f->c.motion, 0, 0, 1) & 0xFF) & 2) {
        return;
    }
    {
        f32 to[4] __attribute__((aligned(16)));
        u8 who = 0;

        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = AT(f, 0xC8, f32);
        v[3] = 0.0f;   /* (unset in the original) */
        Mtx_ApplyVector(v, (f32 (*)[4])((u8 *)f + 0x60), v);
        sceVu0AddVector(to, f->c.a.pos, v);
        if (Fiona_SlamDoor(f, to, 1) & 0xFF) {
            return;
        }
        if (FI(f, 0x1AD5D5, u8) == 1 && !(FI(f, 0x1AD6C0, s32) & 2) &&
            (Actor_Touching(&f->c.a, &((Character *)gCharPartner)->a, 0.0f, 0.0f) & 0xFF) == 1) {
            who |= 2;
            FI(f, 0x1AD6C4, s32) = 1;
        }
        if (FI(f, 0x1AD5D7, u8) == 1 && !(FI(f, 0x1AD6C0, s32) & 4) &&
            (Actor_Touching(&f->c.a, &gCharPursuer->a, 0.0f, 0.0f) & 0xFF) == 1) {
            who |= 4;
        }
        if (who != 0) {
            u16 dmg = (u16)(u32)(5.0f * AT(p, 0xA04, f32));

            if ((Fiona_ChaseRoll(f) & 0xFF) == 1) {
                Relation_Request(p, *(u8 *)&f->c.a.slot, who, 2, dmg, -0x8000, 0.0f);
            } else {
                Relation_Request(p, *(u8 *)&f->c.a.slot, who, 2, dmg, 0, 0.0f);
            }
        }
        FI(f, 0x1AD6C8, s32) = Fiona_BlowCreatures(f, FI(f, 0x1AD6CC, u32), 5, NULL, -1.0f);
    }
}

/* D_003B2628 / D_003B2638: once the animation is done the kick (her cry 0x3D, 0xE01,
 * D_003B2988) */
/* 0x001949D0 */
void Fiona_StateKickStart(Fiona *f) {
    Character_RootMoveMasked(&f->c);
    if (!door_anim_done(f)) {
        return;
    }
    Actor_PlaySound(&f->c.a, 0x3D, 5, 0, 0, NULL);
    FI(f, 0x1AD6C0, s32) = 0;
    FI(f, 0x1AD6C4, s32) = 0;
    FI(f, 0x1AD6C8, s32) = 0;
    FI(f, 0x1AD6CC, s32) = 0;
    Motion_PlayOwnBlend(f->c.motion, 0xE01, -1);
    Actor_SetState(&f->c.a, &D_003B2988);
}

extern void Fiona_ShoveEffect(Fiona *f, s32 kind, Character *c);

/* the effect `kind` on everyone her shove met: Hewie (2), the pursuer (4), the creatures in
 * FI 0x1AD6C8 */
static inline __attribute__((always_inline)) void shove_effects(Fiona *f, u8 hit, s32 kind) {
    Character **list;
    s32 i;

    if (hit & 2) {
        Fiona_ShoveEffect(f, kind, (Character *)gCharPartner);
    }
    if (hit & 4) {
        Fiona_ShoveEffect(f, kind, gCharPursuer);
    }
    list = (Character **)gCreatures;
    for (i = 0; i < 10; i++, list++) {
        if (FI(f, 0x1AD6C8, s32) & (1 << i)) {
            Fiona_ShoveEffect(f, kind, *list);
        }
    }
}

/* D_003B2978: the shove (0xE00). When it meets someone (gProgress +0x1020 + 16 x her slot, or
 * the creatures FI 0x1AD6C8): Hewie (2) counts in the kick count +0xFB6 (+3, to 10000) and
 * spares him the reaction later (FI 0x1AD6C4); her voice by the progress var 0x26 (6: 0x22,
 * 7: none, else 0x8F) - with var 7 the sparks on them, while she faces back (FI 0x1AD6D0 < 0)
 * the bursts; recoil. Once the animation is done: from 0xE00 its end (0x101); after it Hewie's
 * reaction 10, idle (vars 6 / 7: motion +0x2C) */
/* 0x00194AD0 */
void Fiona_StateShove(Fiona *f) {
    Progress *p = gProgress;
    u8 hit = AT(p, 0x1020 + *(u8 *)&f->c.a.slot * 16, u8);

    if (hit != 0 || FI(f, 0x1AD6C8, s32) != 0) {
        u8 v;

        if (hit & 2) {
            FI(f, 0x1AD6C4, s32) = 1;
            AT(p, 0xFB6, s16) = AT(p, 0xFB6, s16) + 3;
            if (AT(p, 0xFB6, s16) < 0) {
                AT(p, 0xFB6, s16) = 0;
            } else if (!(AT(p, 0xFB6, s16) < 0x2711)) {
                AT(p, 0xFB6, s16) = 0x2710;
            }
        }
        v = Progress_GetVar(p, 0x26);
        if (v == 6) {
            Actor_PlaySound(&f->c.a, 0x22, 5, 0, 0, NULL);
        } else if (v != 7) {
            Actor_PlaySound(&f->c.a, 0x8F, 5, 0, 0, NULL);
        }
        if (v == 7) {
            shove_effects(f, hit, 1);
        }
        if (FI(f, 0x1AD6D0, f32) < 0.0f) {
            shove_effects(f, hit, 0);
        }
        f->c.unk14D0 = 5;
        Motion_Freeze(f->c.motion);
        FI(f, 0x1AD6C8, s32) = 0;
    }
    if (door_anim_done(f)) {
        if (AT(f->c.motion, 0x55C, s32) == 0xE00) {
            Motion_PlayWith(f->c.motion, 0, 0x101, -1, 10.0f);
        } else {
            if (FI(f, 0x1AD6C4, s32) == 0) {
                hewie_react(f, 10);
            }
            p = gProgress;
            door_give_up(f, p);
            if ((u32)((Progress_GetVar(p, 0x26) & 0xFF) - 6) < 2) {
                VCALL(f->c.motion, 0x2C, void (*)(void *))(f->c.motion);
            }
        }
    }
    Character_RootMoveMasked(&f->c);
}

extern const PTMF D_003B2948, D_003B2958, D_003B2968;

/* D_003B2648: once the animation is done the shove (0xE00, D_003B2968) - with the progress var
 * 0x26 6 / 7 at its normal pace (after motion +0x30); else slower the longer she has been
 * panicking (the recovery delay FI 0x1AD5F8 past 450: 1.5 x (3150 - it) / 1800) and the more
 * panicked she is (over 40: (160 - panic) / 120), whichever is slower */
/* 0x00195C70 */
void Fiona_StateShoveStart(Fiona *f) {
    u8 v = Progress_GetVar(gProgress, 0x26);

    if (door_anim_done(f)) {
        if (v == 7 || v == 6) {
            VCALL(f->c.motion, 0x30, void (*)(void *))(f->c.motion);
            Motion_PlayTable(f->c.motion, 0xE00, -1);
        } else {
            f32 k = 1.0f, j;

            if (!(FI(f, 0x1AD5F8, s32) < 0x1C3)) {
                k = 1.5f * ((3150.0f - (f32)FI(f, 0x1AD5F8, s32)) / 1800.0f);
            }
            j = 1.0f;
            if (!(FI(f, 0x1AD5F4, f32) <= 40.0f)) {
                j = (160.0f - FI(f, 0x1AD5F4, f32)) / 120.0f;
            }
            if (k < j) {
                FI(f, 0x1AD6D0, f32) = k;
            } else {
                FI(f, 0x1AD6D0, f32) = j;
            }
            Motion_PlayTable(f->c.motion, 0xE00, 2);
            AT(MOTION_PTR(f->c.motion, 0x6A4), 0x1C, f32) = FI(f, 0x1AD6D0, f32);
        }
        FI(f, 0x1AD6C0, s32) = 0;
        FI(f, 0x1AD6C4, s32) = 0;
        FI(f, 0x1AD6C8, s32) = 0;
        FI(f, 0x1AD6CC, s32) = 0;
        FI(f, 0x1AD6D0, f32) = 1.0f;
        FI(f, 0x1AD6D4, f32) = 1.0f;
        Actor_SetState(&f->c.a, &D_003B2968);
    }
    Character_RootMoveMasked(&f->c);
}

/* D_003B2958: the scripted door opened; at the animation's event 0x20 the progress told (in:
 * DoorHold_Release, out: Progress_UseDoor), idle */
/* 0x00195EE0 */
void Fiona_StateScriptDoorOpened(Fiona *f) {
    Progress *p;

    if ((MOTION_EVENTS(f->c.motion) & 0x20) == 0) {
        return;
    }
    p = gProgress;
    if (f->c.moveSub == 0x14) {
        DoorHold_Release(p, f->c.a.room, *(u8 *)&f->c.unk100);
    } else {
        ((void (*)(Progress *, s32, s32))Progress_UseDoor)(p, f->c.a.room, *(u8 *)&f->c.unk100);
    }
    door_give_up(f, p);
}

/* D_003B2948: walking to the scripted door's spot; there, unless it holds her back
 * (DoorHold_Take), its animation (single 0x600 / double 0x602, D_003B2958); else idle */
/* 0x00196070 */
void Fiona_StateScriptDoorWalk(Fiona *f) {
    s32 r = Fiona_DoorFrame(f);
    Progress *p;

    if (r < 0) {
        door_give_up(f, gProgress);
        return;
    }
    if (r != 0) {
        return;
    }
    p = gProgress;
    if ((DoorHold_Take(p, f->c.a.room, *(u8 *)&f->c.unk100, *(u8 *)&f->c.a.slot) & 0xFF) != 0) {
        door_give_up(f, p);
        return;
    }
    if (f->c.unk104[0] == 0) {
        Motion_PlayOwnBlend(f->c.motion, 0x600, -1);
    } else {
        Motion_PlayOwnBlend(f->c.motion, 0x602, -1);
    }
    Actor_SetState(&f->c.a, &D_003B2958);
}

/* D_003B25F8: a scripted door (unk100, unk104[0] double): its spot (kind 0 / 2) walked to
 * (D_003B2948) */
/* 0x00196350 */
void Fiona_StateScriptDoor(Fiona *f) {
    f32 at[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    s32 tri;

    FI(f, 0x1AD6C0, s32) = VCALL(gDoors, 0x14, s32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(
        gDoors, *(u8 *)&f->c.unk100, f->c.unk104[0] == 0 ? 0 : 2, at, dir, 0);
    tri = FI(f, 0x1AD6C0, s32);
    door_walk(f, tri, at, dir[1], &D_003B2948);
}

/* ---- the ladder (unk100 its door, unk104[0] 1 from the bottom / 0 from the top) ---- */

extern const PTMF D_003B2848, D_003B2858;

/* D_003B2878.. (climbing): at the animation's event 0x20 off the ladder - at the top (0x707)
 * or the bottom (0x703) placed by it (Actor_DoorFront) - the ladder let go, idle; until then moved
 * by the root motion */
/* 0x00197FF0 */
void Fiona_StateLadderOff(Fiona *f) {
    FI(f, 0x1AD5BC, u8) = 0;
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        f32 v[4] __attribute__((aligned(16)));
        Progress *p;

        switch (AT(f->c.motion, 0x55C, s32)) {
        case 0x707:
            v[0] = D_003B2460[3].x;
            v[1] = 0.0f;
            v[2] = D_003B2460[3].z;
            v[3] = 0.0f;
            f->c.a.navTri = Actor_DoorFront(f, f->c.unk100, 1, v, f->c.a.pos);
            break;
        case 0x703:
            v[0] = D_003B2478;
            v[1] = 0.0f;
            v[2] = D_003B247C;
            v[3] = 0.0f;
            f->c.a.navTri = Actor_DoorFront(f, f->c.unk100, 0, v, f->c.a.pos);
            break;
        }
        p = gProgress;
        f->unk1AD588 = 0;
        f->c.a.unk2A = 0;
        RoomSlots_Leave(p, *(u8 *)&f->c.unk100, *(u8 *)&f->c.a.slot);
        door_give_up(f, p);
    } else {
        f32 d[4] __attribute__((aligned(16)));

        Motion_RootMovement(f->c.motion, d, 0.0f);
        Character_RootTurn(&f->c);
        sceVu0ApplyMatrix(d, (f32 (*)[4])((u8 *)f + 0x60), d);
        sceVu0AddVector(f->c.a.pos, f->c.a.pos, d);
        f->c.a.pos[3] = 1.0f;
    }
}

/* D_003B2848: walking to the ladder; there, onto it (moveSub 7; from the bottom 0x700, from the
 * top 0x704; D_003B2858); can't: the ladder let go, idle */
/* 0x00198A70 */
void Fiona_StateLadderWalk(Fiona *f) {
    s32 r = Fiona_DoorFrame(f);

    if (r < 0) {
        Progress *p = gProgress;

        RoomSlots_Leave(p, *(u8 *)&f->c.unk100, *(u8 *)&f->c.a.slot);
        door_give_up(f, p);
        return;
    }
    if (r != 0) {
        return;
    }
    f->c.a.unk2A = 1;
    f->c.moveSub = 7;
    if (f->c.unk104[0] != 0) {
        Motion_PlayOwnBlend(f->c.motion, 0x700, -1);
    } else {
        Motion_PlayOwnBlend(f->c.motion, 0x704, -1);
    }
    Actor_SetState(&f->c.a, &D_003B2858);
}

/* D_003B2618: the ladder's foot (or top) point and facing (nav +0x58) walked to (D_003B2848);
 * none: the ladder let go, idle */
/* 0x00198C50 */
void Fiona_StateLadder(Fiona *f) {
    f32 v[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    VObject *nm;
    f32 yaw;
    s32 tri;

    if (f->c.unk104[0] != 0) {
        v[0] = D_003B2460[0].x;
        v[1] = 0.0f;
        v[2] = D_003B2460[0].z;
        v[3] = 0.0f;
    } else {
        v[0] = D_003B2460[1].x;
        v[1] = 0.0f;
        v[2] = D_003B2460[1].z;
        v[3] = 0.0f;
    }
    FI(f, 0x1AD6C0, s32) = Actor_DoorFront(f, f->c.unk100, f->c.unk104[0], v, at);
    if (FI(f, 0x1AD6C0, s32) == -1) {
        Progress *p = gProgress;

        RoomSlots_Leave(p, *(u8 *)&f->c.unk100, *(u8 *)&f->c.a.slot);
        door_give_up(f, p);
        return;
    }
    nm = (VObject *)gNavMesh;
    yaw = VCALL(nm, 0x58, f32 (*)(VObject *, s32, s32))(nm, f->c.unk100, f->c.unk104[0]);
    tri = FI(f, 0x1AD6C0, s32);
    f->c.unk124 = f->c.unk128;
    FI(f, 0x1AD650, s32) = 0;
    FI(f, 0x1AD634, s32) = tri;
    sceVu0CopyVector((f32 *)((u8 *)f + 0x1AD640), at);
    VCALL(nm, 0x14, void (*)(VObject *, s32, f32 *))(nm, tri, (f32 *)((u8 *)f + 0x1AD640));
    f->savedYaw = yaw;
    Actor_SetState(&f->c.a, &D_003B2848);
}

extern f32 D_0047E3A8;   /* the stick's vertical */
extern const PTMF D_003B2868, D_003B2878, D_003B2888, D_003B2898, D_003B28A8;

/* the ladder's foot (0) or top (1) end point (nav +0x5C) */
static inline __attribute__((always_inline)) void ladder_end(Fiona *f, s32 top, f32 *out) {
    VCALL((VObject *)gNavMesh, 0x5C, void (*)(VObject *, s32, s32, f32 *))((VObject *)gNavMesh, f->c.unk100, top, out);
}

/* D_003B2858: on the ladder (moves 0x700..0x709: 0x701 / 0x705 rungs down / up, 0x702 / 0x706
 * their ends, 0x703 / 0x707 off at the bottom / top, 0x708 / 0x709 holding). Knocked (FI
 * 0x1AD584 bit 2): she falls off (D_003B2868). When a move ends (its event 0x20, or holding) the
 * stick (pad up / down plus the analog) picks the next: down - near the foot (18) off (0x703,
 * D_003B2878) unless the pursuer is below (door bit 4: hold); up - near the top (3) off (0x707,
 * D_003B2898 / from 0x700 D_003B28A8) - not while the pursuer is above her (door bit 2, within
 * his height): then as if released; released - hold. After a new move her nav triangle is set
 * at the end she's nearest. Moved by the root motion */
/* 0x00198240 */
void Fiona_StateOnLadder(Fiona *f) {
    s32 anim;

    f->c.a.unk2A = 1;
    FI(f, 0x1AD5BC, u8) = 0;
    if (!door_anim_done(f)) {
        return;
    }
    if (FI(f, 0x1AD584, s32) & 2) {
        Fiona_StartInDoor(f);
        f->unk1AD580 = 0xA;
        f->c.moveMode = 4;
        f->c.a.unk2A = 1;
        f->c.a.unk2D = 1;
        f->c.moveSub = 8;
        Actor_SetState(&f->c.a, &D_003B2868);
        return;
    }
    anim = AT(f->c.motion, 0x55C, s32);
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0 || (u32)(anim - 0x708) < 2) {
        f32 end[4] __attribute__((aligned(16)));
        f32 stick, a;
        u8 idle = 1, moved = 0;

        stick = D_0047E3A8 + (f32)(s32)(((D_0047E374 >> 6) & 1) - ((D_0047E374 >> 4) & 1));
        if (!(stick <= 0.0f)) {
            a = stick;
        } else {
            a = -stick;
        }
        if (!(a <= 0.5f)) {
            if (stick < 0.0f) {
                /* down */
                idle = 0;
                switch (anim - 0x700) {
                case 0: case 2: case 6: case 8:
                    ladder_end(f, 0, end);
                    if (end[1] - f->c.a.pos[1] < 18.0f) {
                        u8 blocked = FI(f, 0x1AD5D7, u8) == 1 &&
                                     ((RoomSlots_Bytes(gProgress, *(u8 *)&f->c.unk100, 2) & 0xFF) & 4);

                        if (blocked) {
                            if (anim != 0x708) {
                                moved = 1;
                                Motion_PlayOwnBlend(f->c.motion, 0x708, -1);
                            }
                        } else {
                            if (anim == 0x700 || anim == 0x702) {
                                Motion_Play(f->c.motion, 0x703, -1);
                            } else {
                                Motion_PlayOwnBlend(f->c.motion, 0x703, -1);
                            }
                            moved = 1;
                            Actor_SetState(&f->c.a, &D_003B2878);
                        }
                    } else {
                        if (anim == 0x700 || anim == 0x702) {
                            Motion_Play(f->c.motion, 0x701, -1);
                        } else {
                            Motion_PlayOwnBlend(f->c.motion, 0x701, -1);
                        }
                        moved = 1;
                    }
                    break;
                case 1:
                    moved = 1;
                    Motion_Play(f->c.motion, 0x702, -1);
                    break;
                case 4:
                    if (FI(f, 0x1AD5D7, u8) == 1 &&
                        ((RoomSlots_Bytes(gProgress, *(u8 *)&f->c.unk100, 2) & 0xFF) & 4)) {
                        Motion_PlayOwnBlend(f->c.motion, 0x708, -1);
                    } else {
                        Motion_PlayOwnBlend(f->c.motion, 0x703, -1);
                        Actor_SetState(&f->c.a, &D_003B2888);
                    }
                    moved = 1;
                    break;
                case 5:
                case 9:
                    moved = 1;
                    Motion_PlayOwnBlend(f->c.motion, 0x702, -1);
                    break;
                }
            } else {
                /* up */
                idle = 0;
                if (FI(f, 0x1AD5D7, u8) == 1 &&
                    ((RoomSlots_Bytes(gProgress, *(u8 *)&f->c.unk100, 2) & 0xFF) & 2) &&
                    f->c.a.pos[1] - gCharPursuer->a.pos[1] < AT(gCharPursuer, 0xCC, f32)) {
                    idle = 1;
                }
                if (idle == 0) {
                    switch (anim - 0x700) {
                    case 0:
                        moved = 1;
                        Motion_PlayOwnBlend(f->c.motion, 0x707, -1);
                        Actor_SetState(&f->c.a, &D_003B28A8);
                        break;
                    case 1:
                    case 9:
                        moved = 1;
                        Motion_PlayOwnBlend(f->c.motion, 0x706, -1);
                        break;
                    case 2: case 4: case 6: case 8:
                        ladder_end(f, 1, end);
                        if (f->c.a.pos[1] - end[1] < 3.0f) {
                            if (anim == 0x704 || anim == 0x706) {
                                Motion_Play(f->c.motion, 0x707, -1);
                            } else {
                                Motion_PlayOwnBlend(f->c.motion, 0x707, -1);
                            }
                            Actor_SetState(&f->c.a, &D_003B2898);
                        } else if (anim == 0x704 || anim == 0x706) {
                            Motion_Play(f->c.motion, 0x705, -1);
                        } else {
                            Motion_PlayOwnBlend(f->c.motion, 0x705, -1);
                        }
                        moved = 1;
                        break;
                    case 5:
                        moved = 1;
                        Motion_Play(f->c.motion, 0x706, -1);
                        break;
                    }
                }
            }
        }
        if (idle == 1) {
            switch (anim - 0x700) {
            case 0: case 2: case 4: case 6:
                Motion_PlayOwnBlend(f->c.motion, 0x708, -1);
                break;
            case 1: case 5:
                Motion_PlayOwnBlend(f->c.motion, 0x709, -1);
                break;
            }
            moved = 1;
        }
        if (moved == 1) {
            f32 v[4] __attribute__((aligned(16)));
            f32 at[4] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 0.0f};   /* (an out-parameter) */
            s32 now = AT(f->c.motion, 0x55C, s32);

            if (now == 0x703 || now == 0x704) {
                v[0] = D_003B2478;
                v[1] = 0.0f;
                v[2] = D_003B247C;
                v[3] = 0.0f;
                f->c.a.navTri = Actor_DoorFront(f, f->c.unk100, 0, v, at);
            } else {
                v[0] = D_003B2460[3].x;
                v[1] = 0.0f;
                v[2] = D_003B2460[3].z;
                v[3] = 0.0f;
                f->c.a.navTri = Actor_DoorFront(f, f->c.unk100, 1, v, at);
            }
        }
    }
    {
        f32 d[4] __attribute__((aligned(16)));

        Motion_RootMovement(f->c.motion, d, 0.0f);
        Character_RootTurn(&f->c);
        sceVu0ApplyMatrix(d, (f32 (*)[4])((u8 *)f + 0x60), d);
        sceVu0AddVector(f->c.a.pos, f->c.a.pos, d);
        f->c.a.pos[3] = 1.0f;
    }
}

/* D_003B2808 / D_003B2828 / D_003B2838 (letting go of a pushed object): idle when blocked
 * (knocked, or the pursuer has her) or at the animation's event 0x20 */
/* 0x00198F00 */
void Fiona_StatePushLetGo(Fiona *f) {
    if (Fiona_PushBlocked(f)) {
        door_give_up(f, gProgress);
        return;
    }
    Character_RootMoveMasked(&f->c);
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        door_give_up(f, gProgress);
    }
}

extern void *D_00479600[];
extern const PTMF D_003B2978;

static void strike_mark_init(void **obj) {
    obj[0] = D_00479600;
}

/* a chance out of 1 */
static inline __attribute__((always_inline)) s32 fiona_chance(f32 c) {
    return VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < c;
}

/* D_003B2968: her strike (the progress var 0x26: 6 / 7 the two special forms). Hits taken back
 * as for the shove, gathered (FI 0x1AD6C0 / 0x1AD6CC). At its event 0x20 idle (6 / 7: motion
 * +0x2C). At its strike mark (motion flags bit 2): with form 7 once a mark (D_00479600); the
 * hand's point (form 6 / 7 bone +0x78, else +0x74) - a door there slammed; normally a point off
 * the mesh ends the swing (0x101) - then whom it reaches (the progress +0x30: Hewie 2 / the
 * pursuer 4, once each) from the striking point (form 7: 10 to the side, reach 10; form 6: 6
 * ahead, reach 3; else the hand, reach 2): the blow (form 6 kind 1 damage 5, form 7 kind 2 10,
 * else kind 1 1; the weapon 0x83 one in ten 50 / 100 (FI 0x1AD6D0 -1), 0x82 kind 2 5, 0x81 2;
 * a chance of -0x8000) x gProgress +0xA04, and to the creatures there */
/* 0x00194FC0 */
void Fiona_StateStrike(Fiona *f) {
    Progress *p = gProgress;
    u8 v = Progress_GetVar(p, 0x26);
    u8 hit = AT(p, 0x1020 + *(u8 *)&f->c.a.slot * 16, u8);
    f32 at[4] __attribute__((aligned(16)));
    f32 reach;
    u8 who;
    s32 kind, extra;
    u32 dmg;
    u16 d;

    if (hit != 0 || FI(f, 0x1AD6C8, s32) != 0) {
        if (hit & 2) {
            FI(f, 0x1AD6C4, s32) = 1;
            AT(p, 0xFB6, s16) = AT(p, 0xFB6, s16) + 3;
            if (AT(p, 0xFB6, s16) < 0) {
                AT(p, 0xFB6, s16) = 0;
            } else if (!(AT(p, 0xFB6, s16) < 0x2711)) {
                AT(p, 0xFB6, s16) = 0x2710;
            }
        }
        if (v == 6) {
            Actor_PlaySound(&f->c.a, 0x22, 5, 0, 0, NULL);
        } else if (v != 7) {
            Actor_PlaySound(&f->c.a, 0x8F, 5, 0, 0, NULL);
        }
        if (v == 7) {
            shove_effects(f, hit, 1);
        }
        if (FI(f, 0x1AD6D0, f32) < 0.0f) {
            shove_effects(f, hit, 0);
        }
        f->c.unk14D0 = 5;
        Motion_Freeze(f->c.motion);
        FI(f, 0x1AD6C0, s32) |= hit;
        FI(f, 0x1AD6CC, s32) |= FI(f, 0x1AD6C8, s32);
        FI(f, 0x1AD6C8, s32) = 0;
    }
    Character_RootMoveMasked(&f->c);
    if ((MOTION_EVENTS(f->c.motion) & 0x20) != 0) {
        door_give_up(f, p);
        if ((u32)(v - 6) < 2) {
            VCALL(f->c.motion, 0x2C, void (*)(void *))(f->c.motion);
        }
        return;
    }
    if (!((Motion_EventFlags(f->c.motion, 0, -1, 1) & 0xFF) & 2)) {
        return;
    }
    if (v == 7 && !(FI(f, 0x1AD6D4, f32) <= 0.0f)) {
        u8 *mgr = gEffects;
        s32 none = 0;

        FI(f, 0x1AD6D4, f32) = -1.0f;
        EffectMgr_Start(mgr, Effect_New(mgr, 0x80, strike_mark_init), &none);
    }
    {
        s32 bone;

        if ((u32)(v - 6) < 2) {
            bone = VCALL(f->c.motion, 0x78, s32 (*)(void *))(f->c.motion);
        } else {
            bone = VCALL(f->c.motion, 0x74, s32 (*)(void *))(f->c.motion);
        }
        sceVu0CopyVector(at, Skel_Bone(AT(f->c.motion, 0x810, void *), bone) + 12);
    }
    who = Fiona_SlamDoor(f, at, 1);
    if (v != 7 && v != 6 && Actor_TriTo(&f->c.a, at, 0x20018) == (u32)-1) {
        if (door_anim_done(f)) {
            Motion_PlayWith(f->c.motion, 0, 0x101, -1, 10.0f);
        }
        Actor_SetState(&f->c.a, &D_003B2978);
        return;
    }
    if (who != 0) {
        return;
    }
    {
        f32 m[4][4] __attribute__((aligned(16)));
        f32 o[4] __attribute__((aligned(16)));
        f32 pt[4] __attribute__((aligned(16)));

        if (v == 7) {
            sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(AT(f->c.motion, 0x810, void *),
                                                          VCALL(f->c.motion, 0x78, s32 (*)(void *))(f->c.motion)));
            o[0] = -10.0f;
            o[1] = 0.0f;
            o[2] = 0.0f;
            o[3] = 1.0f;
            sceVu0ApplyMatrix(pt, m, o);
            reach = 10.0f;
        } else if (v == 6) {
            sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(AT(f->c.motion, 0x810, void *),
                                                          VCALL(f->c.motion, 0x78, s32 (*)(void *))(f->c.motion)));
            o[0] = 0.0f;
            o[1] = 0.0f;
            o[2] = 6.0f;
            o[3] = 1.0f;
            sceVu0ApplyMatrix(pt, m, o);
            reach = 3.0f;
        } else {
            sceVu0CopyVector(pt, Skel_Bone(AT(f->c.motion, 0x810, void *),
                                               VCALL(f->c.motion, 0x74, s32 (*)(void *))(f->c.motion)) + 12);
            reach = 2.0f;
        }
        who = 0;
        if (FI(f, 0x1AD5D5, u8) == 1 && !(FI(f, 0x1AD6C0, s32) & 2) &&
            (VCALL(p, 0x30, s32 (*)(Progress *, u32, f32 *, s32, f32))(p, *(u8 *)&f->c.a.slot, pt, 1, reach) & 0xFF) == 1) {
            who |= 2;
        }
        if (FI(f, 0x1AD5D7, u8) == 1 && !(FI(f, 0x1AD6C0, s32) & 4) &&
            (VCALL(p, 0x30, s32 (*)(Progress *, u32, f32 *, s32, f32))(p, *(u8 *)&f->c.a.slot, pt, 2, reach) & 0xFF) == 1) {
            who |= 4;
        }
        FI(f, 0x1AD6D0, f32) = 1.0f;
        extra = 0;
        if (v == 7) {
            dmg = 0xA;
            kind = 2;
            if (gSubScreen != NULL) {
                if (VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 0) == 0x83 &&
                    fiona_chance(0x1.99999ap-4f /* 0.1 */)) {
                    dmg = 0x64;
                    FI(f, 0x1AD6D0, f32) = -1.0f;
                }
                if (fiona_chance(0x1.99999ap-3f /* 0.2 */)) {
                    extra = -0x8000;
                }
            }
        } else if (v == 6) {
            dmg = 5;
            kind = 1;
            if (gSubScreen != NULL) {
                if (VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 0) == 0x83 &&
                    fiona_chance(0x1.99999ap-4f /* 0.1 */)) {
                    dmg = 0x32;
                    FI(f, 0x1AD6D0, f32) = -1.0f;
                }
                if (fiona_chance(0x1.99999ap-3f /* 0.2 */)) {
                    extra = -0x8000;
                }
            }
        } else {
            dmg = 1;
            kind = 1;
            if (gSubScreen != NULL) {
                switch (VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 0)) {
                case 0x83:
                    if (fiona_chance(0x1.99999ap-4f /* 0.1 */)) {
                        kind = 2;
                        dmg = 0x32;
                        FI(f, 0x1AD6D0, f32) = -1.0f;
                    }
                    break;
                case 0x82:
                    if (fiona_chance(0x1.99999ap-3f /* 0.2 */)) {
                        extra = -0x8000;
                    }
                    kind = 2;
                    dmg = 5;
                    break;
                case 0x81:
                    if (fiona_chance(0x1.99999ap-4f /* 0.1 */)) {
                        extra = -0x8000;
                    }
                    dmg = 2;
                    break;
                }
            }
        }
        d = (u16)(u32)((f32)dmg * AT(p, 0xA04, f32));
        if (who != 0) {
            Relation_Request(p, *(u8 *)&f->c.a.slot, who, kind, d, extra, 0.0f);
        }
        FI(f, 0x1AD6C8, s32) = Fiona_BlowCreatures(f, FI(f, 0x1AD6CC, u32), d, pt, reach);
    }
}

/* head for tri / pos (planning the path, Character_PlanPathKind): 0 on the way, -1 when it's across the
 * room's divider from her or there is no path. `run` 0 starts walking it (Character_WaypointsCurve), else
 * Character_Waypoints */
/* 0x00180D60 */
s32 Fiona_HeadFor(Fiona *f, u32 tri, f32 *pos, s32 run) {
    s32 r;

    if (NavMesh_AcrossDivider(gNavMesh, tri, f->c.a.navTri)) {
        return -1;
    }
    r = Character_PlanPathKind(&f->c, 0, tri, pos);
    if (r > 0) {
        r = run == 0 ? Character_WaypointsCurve(&f->c) : Character_Waypoints(&f->c);
    }
    return -(r < 0);
}
