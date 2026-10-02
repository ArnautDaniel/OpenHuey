/* Hewie: the partner dog (vtable 0x46A120). */
#include "common.h"
#include "hewie.h"
#include "progress.h"
#include "navmesh.h"
#include "sce/libvu0.h"

extern VObject *gBootMessage;      /* message display, also used in game */
extern Progress *gProgress;
extern Character *gCharacters[6];
extern Character *gCharPlayer;    /* Fiona */
extern Character *gCharPartner;   /* Hewie */
extern Character *gCharPursuer;

#define MOTION_U8(m, off) (*((u8 *)(m) + (off)))
#define MOTION_ANIM(m) (*(s32 *)((u8 *)(m) + 0x55C))
#define SLOT_U8(h) (*(u8 *)&(h)->c.a.slot)

extern void *D_0046A120[];   /* Hewie vtable */
extern void *D_00469C60[];   /* Character vtable */
extern void *D_00469C20[];   /* Actor base vtable */
extern void func_00124E40(Actor *a);

/* vtable +0x8: destructor (nothing to free: he lives inside the scene). */
Hewie *func_00130A70(Hewie *h, s32 flags) {
    if (h != NULL) {
        h->c.a.vtbl = D_0046A120;
        h->c.a.vtbl = D_00469C60;
        h->c.a.vtbl = D_00469C20;
        if ((s16)flags > 0) {
            func_00124E40(&h->c.a);
        }
    }
    return h;
}

/* vtable +0x10 */
void func_00168A00(Hewie *h) {
}

extern void func_00125D40(Character *c);

/* vtable +0x58: deactivate (Character part only). */
void func_00166140(Hewie *h) {
    func_00125D40(&h->c);
}

/* vtable +0x98 */
s32 func_00130AE0(Hewie *h) {
    return 1;
}

extern void func_00126910(Character *c);
extern void func_00126810(Character *c);

/* vtable +0x4C: disable (as Character), animation paused. */
void func_00165CD0(Hewie *h) {
    func_00126910(&h->c);
    MOTION_U8(h->c.motion, 0x4D8) = 1;
}

extern void func_00125BE0(Character *c);
extern void func_00130AF0(Hewie *h, s32 action, s32 arg);

extern s32 func_001235C0(Actor *a, Actor *b);
extern u32 func_00177620(Progress *p);          /* u8 */
extern VObject *D_0044E4F8;

/* Group of his current animation (0 idle .. 0xE, 0xF other). */
s32 func_001669A0(Hewie *h) {
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
 * counts up, max 10000); while blocked (D_0044E4F8 +0x38) or with progress flags 0x13 / 0x2B
 * set his attack-type actions become waiting ones; then substitutions by his condition
 * (+0xC4), mode (+0xF35C0) and flags. In the special mode the action is kept. */
s32 func_0013B2C0(Hewie *h, s32 act) {
    Progress *p = gProgress;
    u8 f;

    if (*((u8 *)p + 0x1FBEC1) == 1) {
        return act;
    }
    if (!h->c.unkE0 && act == 0 && h->c.hp == 0 && h->c.a.unkC4 == 2) {
        act = 0x52;
    }
    if (act == 0x52) {
        if (!h->c.a.disabled && !(func_001235C0(&h->c.a, &h->c.a) & 0xFF)) {
            act = 0;
            h->c.hp = 1;
            h->c.a.unkC4 = 1;
        } else if (HEWIE_ACTION(h) != 0x52 && HEWIE_ACTION(h) != 0x74 && func_001669A0(h) != 0xD) {
            s16 *n = (s16 *)((u8 *)p + 0xFB6);

            *n += 10;
            if (*n < 0) {
                *n = 0;
            } else if (*n > 10000) {
                *n = 10000;
            }
        }
    }
    if (VCALL(D_0044E4F8, 0x38, s32 (*)(VObject *))(D_0044E4F8)) {
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
            if ((func_00177620(p) & 0xFF) == 2) {
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
    if (HW(h, 0xF3620, u8) == 1 && (act == 5 || act == 4 || act == 1) && !(func_00177620(p) & 0xFF)) {
        act = 0x81;
    }
    if (HW(h, 0xF3588, u8) == 1) {
        act = 4;
    }
    return act;
}

extern VObject *D_0044E550;   /* random numbers: +0x1C -> 0..1 */
#define RNG01() VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550)

/* One chance in `n`: sets +0xF3586. */
void func_00138DE0(Hewie *h, s32 n) {
    if ((s32)((f32)n * RNG01()) == 0) {
        HW(h, 0xF3586, u8) = 1;
    }
}

/* Spend `amount` of his pool (+0xF359C) while it is in use (+0xF3598). */
void func_00138E60(Hewie *h, s32 amount) {
    if (HW(h, 0xF3598, s32) == 1) {
        HW(h, 0xF359C, s32) -= amount;
        HW(h, 0xF3587, u8) = 0;
    }
}

extern s32 func_00127140(Character *c, s32 kind, u32 goalTri, const f32 *goal);
extern s32 func_001270A0(Character *c);
extern s32 func_001270F0(Character *c);
extern VObject *gSceneGameF29740;   /* path planner */

/* Triangles on opposite sides of a divided room (flags 0x100000 / 0x200000). */
static inline s32 Hewie_OtherSide(Hewie *h, u32 tri) {
    NavTri *t = NavMesh_Tri(D_0044E570, tri);
    NavTri *cur = NavMesh_Tri(D_0044E570, h->c.a.navTri);
    u32 ft = t->flags & 0x300000, fc = cur->flags & 0x300000;

    return (ft == 0x100000 && fc == 0x200000) || (ft == 0x200000 && fc == 0x100000);
}

/* Plan a path to `pos` on `tri` (not across the room's divider); 1 if one was found. */
s32 func_0013C1E0(Hewie *h, u32 tri, const f32 *pos) {
    s32 r;

    if (Hewie_OtherSide(h, tri)) {
        return 0;
    }
    r = func_00127140(&h->c, 0, tri, pos);
    if (r > 0) {
        r = VCALL(gSceneGameF29740, 0x14, s32 (*)(VObject *))(gSceneGameF29740);
    }
    return r > 0;
}

/* Plan a path to `pos` on `tri` (not across the divider) and start it (`direct`: the straight
 * variant); `keep` keeps the planner's previous request. 0 = ok, -1 = failed. */
s32 func_0013EE40(Hewie *h, u32 tri, const f32 *pos, s32 direct, s32 keep) {
    s32 r;

    if (Hewie_OtherSide(h, tri)) {
        return -1;
    }
    r = func_00127140(&h->c, 0, tri, pos);
    if (r <= 0) {
        return -(r < 0);
    }
    if (!keep) {
        VCALL(gSceneGameF29740, 0x34, void (*)(VObject *, s32, s32))(gSceneGameF29740, h->c.pathId, 3);
    }
    r = direct ? func_001270A0(&h->c) : func_001270F0(&h->c);
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

extern const s32 D_003B1350[];   /* by +0xF35CC (normal) */
extern const s32 D_003B1370[];   /* by +0xF35CC (difficulty 1) */

/* Set his mode (and its timer: `time`, or -1 for the mode's default: 1 and 2 1800 frames, 3 450).
 * Down: only mode 0; in condition 1 only mode 3. Entering mode 1 / 3 counts in the progress
 * counters; mode 1 refills his pool and may restart action 0x34. */
void func_00138AD0(Hewie *h, s32 mode, s32 time) {
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
        HW(h, 0xF359C, s32) = D_003B1370[HW(h, 0xF35CC, s16)];
    } else {
        HW(h, 0xF359C, s32) = D_003B1350[HW(h, 0xF35CC, s16)];
    }
    if ((HW(h, 0xF356C, s32) & 0x80000001) == 1) {
        if (!h->c.a.disabled) {
            HW(h, 0xF3559, u8) = 1;
        } else {
            func_00130AF0(h, func_0013B2C0(h, 0x34), 0);
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
void func_0013C5D0(Hewie *h) {
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
        func_00138AD0(h, 2, -1);
    }
}

/* His feeling about the kind of character `other` is (-10..10, saved with him), changed by
 * `delta`; then re-evaluated. */
void func_00166150(Hewie *h, Character *other, s32 delta) {
    s16 *v = Hewie_Feeling(h, other->unk153C);

    if (v != NULL) {
        *v += delta;
        if (*v < -10) {
            *v = -10;
        } else if (*v > 10) {
            *v = 10;
        }
    }
    func_0013C5D0(h);
}

/* vtable +0x60: forget path/movement state, then his default action. */
void func_00165D00(Hewie *h) {
    func_00125BE0(&h->c);
    func_00130AF0(h, 0, 0);
}

extern void func_00127650(Character *c);

/* vtable +0x24: save the previous frame's state. */
void func_00168360(Hewie *h) {
    func_00127650(&h->c);
    HW(h, 0xF354C, f32) = h->c.a.angle[1];
    HW(h, 0xF3568, s32) = HW(h, 0xF3564, s32);
    HW(h, 0xF366C, u8) = HW(h, 0xF366D, u8);
}

/* vtable +0x20: release his message slot and stop his animation player, if set up. */
void func_00168680(Hewie *h) {
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
void func_00168600(Hewie *h) {
    if (*((u8 *)gProgress + 0x1FBEC1) == 0 && h->c.unkE4 == 1) {
        VCALL(h, 0x80, void (*)(Hewie *))(h);
    }
    VCALL(h->c.motion, 0x38, void (*)(void *, s32, u32, s32))(h->c.motion, h->c.unk152C, h->c.a.navTri, 0x1D);
}

/* vtable +0x50: halt (as Character), animation running, then back to his default action. */
void func_00165C50(Hewie *h) {
    func_00126810(&h->c);
    MOTION_U8(h->c.motion, 0x4D8) = 0;
    VCALL(h->c.motion, 0x50, void (*)(void *, Hewie *))(h->c.motion, h);
    func_00130AF0(h, func_0013B2C0(h, 0), 0);
}

/* Back to his default action (inlined in several places in the original). */
static inline void Hewie_ToDefault(Hewie *h) {
    func_00130AF0(h, func_0013B2C0(h, 0), 0);
}

/* vtable +0x7C: back to his default action. */
void func_0013D190(Hewie *h) {
    Hewie_ToDefault(h);
}

#define MOTION_SKELETON(m) (*(void **)((u8 *)(m) + 0x810))

extern f32 *func_0017CE80(void *skeleton, s32 bone);        /* bone matrix */

/* vtable +0x74: during action 0x23 with animation 0x1E01, his head bone's position (returns 1). */
s32 func_0013D420(Hewie *h, f32 *out) {
    if (HEWIE_ACTION(h) != 0x23) {
        return 0;
    }
    if (MOTION_ANIM(h->c.motion) != 0x1E01) {
        return 0;
    }
    sceVu0CopyVector(out, func_0017CE80(MOTION_SKELETON(h->c.motion), 0x1F) + 12);
    return 1;
}

extern void func_00126360(Character *c);

/* vtable +0x90: reset (Character part), clear his action state. */
void func_0015FBE0(Hewie *h) {
    func_00126360(&h->c);
    HW(h, 0xF358C, s32) = 0;
    HW(h, 0xF35C4, s32) = 0;
    HW(h, 0xF3610, s32) = 0xFF;
    HW(h, 0xF3584, u8) = 0;
    HW(h, 0xF35E0, u8) = 0;
    Progress_ClearFlag(gProgress, 0xB);
}


/* vtable +0x78: left the room being played during action 0x38 -> default action. */
void func_0015FB30(Hewie *h) {
    s32 room = h->c.a.room;

    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && HEWIE_ACTION(h) == 0x38) {
        Hewie_ToDefault(h);
    }
    func_00138AD0(h, 0, -1);
}


/* vtable +0x94: take `damage` (difficulty 1: x1.5); returns 1 when he is down (difficulty 2:
 * never, he keeps 1). */
s32 func_0015FA20(Hewie *h, s32 damage) {
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

extern void func_001F6AF0(void *motion);
extern NavMesh *D_0044E570;

/* vtable +0x48: apply the animation to the model; while enabled, take his position from the
 * root bone (and find his room and nav-mesh triangle). */
void func_00167AF0(Hewie *h) {
    sceVu0FMATRIX m;

    sceVu0UnitMatrix(m);
    VCALL(h->c.motion, 0x28, void (*)(void *, sceVu0FMATRIX))(h->c.motion, m);
    if (h->c.a.disabled) {
        return;
    }
    MOTION_U8(h->c.motion, 0x4D8) = 1;
    func_001F6AF0(h->c.motion);
    MOTION_U8(h->c.motion, 0x4D8) = 0;
    h->c.a.room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    sceVu0CopyVector(h->c.a.pos, func_0017CE80(MOTION_SKELETON(h->c.motion), 0) + 12);
    h->c.a.navTri = VCALL(D_0044E570, 0x3C, u32 (*)(NavMesh *, f32 *, s32))(D_0044E570, h->c.a.pos, 0);
}

extern void func_00143D20(Hewie *h);
extern void func_00124890(Actor *a, s32 side);
extern VObject *D_0044E568;   /* rooms */
extern VObject *D_0044E4D0;   /* room objects */


/* vtable +0x38: room (re-)entry. In play: active only in the room being played (placed on his
 * side if he has no triangle yet); in the special mode: note his side, hand him to the
 * animation player and the room objects. */
void func_00166CE0(Hewie *h) {
    Progress *p = gProgress;

    if (*((u8 *)p + 0x1FBEC1) != 0) {
        HEWIE_SIDE(h) = VCALL(D_0044E568, 0x50, s32 (*)(VObject *, s32, u32, s32))(D_0044E568, h->c.a.room, h->c.door, 0);
        VCALL(h->c.motion, 0x50, void (*)(void *, Hewie *))(h->c.motion, h);
        VCALL(D_0044E4D0, 0x2C, void (*)(VObject *, Hewie *))(D_0044E4D0, h);
        return;
    }
    func_00143D20(h);
    if (h->c.a.room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        h->c.a.disabled = 1;
        return;
    }
    if (h->c.a.navTri != NAV_NONE) {
        h->c.a.disabled = 0;
        return;
    }
    func_00124890(&h->c.a, HEWIE_SIDE(h));
    h->c.a.disabled = 0;
}

/* Resource table offset (from +0x1540) to pointer, 0 = none. */
#define HEWIE_RES(h, off) (HW(h, off, s32) != 0 ? (void *)((u8 *)(h) + HW(h, off, s32) + 0x1540) : NULL)
#define MOTION_PTR(m, off) (*(void **)((u8 *)(m) + (off)))

/* vtable +0x1C: hook his data up to the animation player and the message display. */
void func_00168700(Hewie *h) {
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

extern VObject *gFileLoader;
extern void *func_001776B0(Progress *p, s32);

/* LoadAsync(name, dest) for his files, tagged with his file id. */
#define Hewie_Load(h, loader, name, dest) \
    VCALL(loader, 0xC, void (*)(VObject *, const void *, void *, u32, s32))( \
        loader, name, dest, (h)->c.a.flags24 | (h)->c.a.slot, 0)

/* vtable +0x14: start loading his files: model (by costume, from the unlocked costume bits in
 * the progress flags), textures (into his message buffer + 0x80000), .MRK. */
void func_00168830(Hewie *h) {
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
    HEWIE_MSG(h) = func_001776B0(p, 0);
    HEWIE_MSG(h) = (u8 *)HEWIE_MSG(h) + 0x80000;
    Hewie_Load(h, loader, VCALL(h->c.motion, 0xA8, void *(*)(void *))(h->c.motion), HEWIE_MSG(h));
    Hewie_Load(h, loader, VCALL(h->c.motion, 0xA4, void *(*)(void *, u32))(h->c.motion, costume), HEWIE_MRK(h));
}

extern void func_002DCDD0(void *motion, Hewie *h, f32, f32);

/* vtable +0x40: animation update in the room being played: ground fit (off the nav mesh: plain),
 * advance; then (still on the mesh) the animation events. */
void func_00167620(Hewie *h) {
    Progress *p = gProgress;
    s32 room = h->c.a.room;

    if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        if (h->c.a.navTri == NAV_NONE) {
            func_002DCDD0(h->c.motion, h, 0.0f, 0.0f);
        } else if (HW(h, 0xF3582, u8) == 1) {
            VCALL(h->c.motion, 0x40, void (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
        } else {
            VCALL(h->c.motion, 0x40, void (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 0.0f, 0.0f);
        }
        func_001F6AF0(h->c.motion);
    }
    room = h->c.a.room;
    if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p) && h->c.a.navTri != NAV_NONE) {
        VCALL(h->c.motion, 0x4C, void (*)(void *, s32, Hewie *))(h->c.motion, 1, h);
    }
}

extern void func_00126450(Character *c);

#define Character_ToIdle(c) VCALL(c, 0x7C, void (*)(Character *))(c)

/* vtable +0x8C: enable (Character part). If he was busy with Fiona (actions 0x48..0x4B, 0x72)
 * or the pursuer (0x1F..0x22, 0x38, 0x75), that character goes back to idle. */
void func_0015FC60(Hewie *h) {
    u8 ok;

    func_00126450(&h->c);
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
void func_001656C0(Hewie *h) {
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
void func_00165890(Hewie *h) {
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

extern void func_00127660(Character *c);

/* vtable +0xC: initialise (Character part, then his own state). */
void func_00168A10(Hewie *h) {
    func_00127660(&h->c);
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

extern s32 func_00125AD0(Character *c, u32 tri, const f32 *heading, f32 *pos);
extern void func_002DDE20(void *motion, s32 set, s32 variant);
extern void func_001F1D60(void *obj);

/* vtable +0x28: place him (as Character) on triangle `tri`; (in play) his default animation;
 * reset his per-placement state. Returns the placement result. */
s32 func_001683D0(Hewie *h, u32 tri, const f32 *heading, f32 *pos) {
    s32 r = func_00125AD0(&h->c, tri, heading, pos);

    if (*((u8 *)gProgress + 0x1FBEC1) == 0) {
        func_002DDE20(h->c.motion, HW(h, 0xF36F0, s32), -1);
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
    func_001F1D60((u8 *)h + 0xF3748);
    HW(h, 0xF3798, s32) = -1;
    return r;
}

extern f32 func_001244D0(Actor *a, const f32 *p);     /* heading towards a point */
extern f32 func_002E2D00(f32 angle);                  /* angle wrapped to -pi..pi */
extern u32 func_00138460(Hewie *h, s32 slot);         /* u8 */
extern VObject *D_0044E558;                           /* doors: +0x40(door) -> usable */

/* vtable +0x68: may character `slot` start interaction `kind` with him now (kind 5: through
 * `door`)? For kinds 1..4, if he stands idle facing roughly towards the caller (within 3pi/8),
 * he may react himself instead (+0xF3584, answer no). */
s32 func_00165A40(Hewie *h, u32 kind, s32 slot, u32 door) {
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
        && func_002E2D00(func_001244D0(&h->c.a, gCharacters[slot]->a.pos) - h->c.a.angle[1]) < 0x1.2d97c8p+0f /* 3pi/8 */
        && (func_00138460(h, slot) & 0xFF) == 1) {
        HW(h, 0xF3584, u8) = 1;
        return 0;
    }
    if (k != 5) {
        return 1;
    }
    return (VCALL(D_0044E558, 0x40, u32 (*)(VObject *, u32))(D_0044E558, door & 0xFF) & 0xFF) ? 1 : 0;
}

extern s32 func_00125BA0(Character *c, s32 room, s32 a2, s32 a3);
extern void func_00124F20(Character *c, u32 door);
extern void func_002DDED0(void *motion, s32 anim, s32 arg);
extern s32 func_00122B50(Actor *a, f32 *out);
extern void func_001264C0(Character *c, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void func_00143840(Hewie *h);
extern void func_00126270(Character *c);

#define Hewie_Place(h, tri) VCALL(h, 0x28, s32 (*)(Hewie *, u32, const f32 *, f32 *))(h, tri, NULL, NULL)

/* Placement with his default action (inlined twice in the original). */
static inline s32 Hewie_PlaceDefault(Hewie *h, u32 tri) {
    s32 r;

    MOTION_PTR(h->c.motion, 0x858) = NULL;
    MOTION_PTR(h->c.motion, 0x854) = NULL;
    HW(h, 0xF36F0, s32) = 0;
    r = Hewie_Place(h, tri);
    func_00124F20(&h->c, 0xFF);
    Hewie_ToDefault(h);
    return r;
}

/* vtable +0x64: put him in room `room` on triangle `tri` (side `side`). In the room being played:
 * placed, default action, handed to the room objects, nearby state updated; elsewhere he only
 * keeps the triangle. Returns the placement result (0 elsewhere). */
s32 func_00166530(Hewie *h, s32 room, u32 tri, s32 side) {
    Progress *p;
    s32 r;

    func_00125BA0(&h->c, room, tri, side);
    p = gProgress;
    HEWIE_SIDE(h) = side;
    if (*((u8 *)p + 0x1FBEC1) != 0) {
        return Hewie_PlaceDefault(h, tri);
    }
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        h->c.a.navTri = tri;
        func_002DDED0(h->c.motion, 0, -1);
        Hewie_ToDefault(h);
        return 0;
    }
    r = Hewie_PlaceDefault(h, tri);
    VCALL(D_0044E4D0, 0x2C, void (*)(VObject *, Hewie *))(D_0044E4D0, h);
    if (h->c.a.navTri == NAV_NONE) {
        h->c.a.pos[0] = 0.0f;
        h->c.a.pos[1] = 0.0f;
        h->c.a.pos[2] = 0.0f;
        h->c.a.pos[3] = 0x1.99999ap-4f;   /* 0.1 */
    }
    {
        sceVu0FVECTOR v;

        if (func_00122B50(&h->c.a, v)) {
            func_001264C0(&h->c, 3, (s32)v, 0, 0, 0);
        }
    }
    func_00143840(h);
    HW(h, 0xF366C, u8) = HW(h, 0xF366D, u8);
    func_00126270(&h->c);
    return r;
}


extern f32 func_00124490(Actor *a, const f32 *p);          /* distance to a point */
extern s32 func_001F1B90(void *input, void *pad);
extern u8 D_0047E3B0[];                                   /* pad state */
extern f32 func_001F6140(void *motion, f32 t);            /* animation turn this frame */
extern void func_001F6370(void *motion, f32 *out, f32 t); /* animation root motion this frame */
extern void func_001247E0(Actor *a, const f32 *delta);
extern u32 func_00177870(Progress *p, u32 slot);          /* joint action pending (u8) */
extern void func_001777D0(Progress *p, u32 slot);         /* cancelled */
extern void func_00146130(Hewie *h);
extern void func_0013A650(Hewie *h);
extern void func_00145080(Hewie *h);

/* vtable +0x44: per-frame update - frame counter (up to 3000), surroundings, how close Fiona
 * is (gProgress +0x7B9: 1 within 20, 2 within 50, 3 further), requests, behaviour, turning and
 * root motion from the animation, then his sub-systems. */
void func_00167760(Hewie *h) {
    Progress *p;
    f32 yaw;
    u8 ok;

    h->c.a.navMask = h->c.a.unk2B ? 0 : HEWIE_NAV_MASK;
    h->c.pathReq->mask = h->c.a.navMask;
    HW(h, 0xF35A8, s16) += 1;
    if (HW(h, 0xF35A8, s16) > 3000) {
        HW(h, 0xF35A8, s16) = 3000;
    }
    func_00143840(h);
    if (h->c.a.unkC4 != 2) {
        ok = (gCharPlayer != NULL && gCharPlayer->a.active == 1) ? 1 : 0;
        if (ok == 1 && gCharPlayer->a.unkC4 != 2 && h->c.a.room == gCharPlayer->a.room) {
            s32 room = h->c.a.room;

            ok = (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) || gCharPlayer->a.navTri != NAV_NONE) ? 1 : 0;
        } else {
            ok = 0;
        }
        if (ok == 1) {
            f32 d = func_00124490(&h->c.a, gCharPlayer->a.pos);

            if (d <= 20.0f) {
                *((u8 *)gProgress + 0x7B9) = 1;
            } else if (d <= 50.0f) {
                *((u8 *)gProgress + 0x7B9) = 2;
            } else {
                *((u8 *)gProgress + 0x7B9) = 3;
            }
        }
    }
    VCALL(h, 0x88, void (*)(Hewie *))(h);
    p = gProgress;
    if (*((u8 *)p + 0x1FBEC1) == 1) {
        HW(h, 0xF3798, s32) = func_001F1B90((u8 *)h + 0xF3748, D_0047E3B0);
    }
    HW(h, 0xF3558, u8) = 0;
    HW(h, 0xF3582, u8) = 1;
    ptmf_scall(h, HEWIE_STATE(h));
    yaw = func_002E2D00(h->c.a.angle[1] + func_001F6140(h->c.motion, 0.0f));
    h->c.a.angle[1] = yaw;
    sceVu0UnitMatrix(h->c.a.rot);
    sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, yaw);
    if (!h->c.a.disabled && !HW(h, 0xF3558, u8)) {
        sceVu0FVECTOR root;
        f32 k;

        func_001F6370(h->c.motion, root, 0.0f);
        k = VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
        *(s32 *)&root[1] = 0;
        root[2] = root[2] * k;
        sceVu0ApplyMatrix(root, h->c.a.rot, root);
        func_001247E0(&h->c.a, root);
    }
    if ((func_00177870(p, SLOT_U8(h)) & 0xFF) == 1) {
        func_001777D0(p, SLOT_U8(h));
    }
    func_00146130(h);
    func_0013A650(h);
    VCALL(h, 0x40, void (*)(Hewie *))(h);
    func_00145080(h);
}

extern void func_00125CC0(Character *c);
extern void func_0013D1F0(Hewie *h, s32 arg);
extern const s32 D_003B1350[];   /* by +0xF35CC (normal) */
extern const s32 D_003B1370[];   /* by +0xF35CC (difficulty 1) */
extern const PTMF D_003B02B0;    /* idle state */
extern const PTMF D_003B0280;    /* special-mode state */

/* vtable +0x5C: activate (Character part), then reset his own state; in play idle with action 0,
 * in the special mode the special state with action 0x83. */
void func_00165D40(Hewie *h) {
    Progress *p;

    func_00125CC0(&h->c);
    func_00138AD0(h, 0, -1);
    func_0013D1F0(h, 0);
    p = gProgress;
    HW(h, 0xF3598, s32) = 0;
    if ((Progress_GetVar(p, 0x27) & 0xFF) == 1) {
        HW(h, 0xF359C, s32) = D_003B1370[HW(h, 0xF35CC, s16)];
    } else {
        HW(h, 0xF359C, s32) = D_003B1350[HW(h, 0xF35CC, s16)];
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
    if (*((u8 *)p + 0x1FBEC1) == 0) {
        HW(h, 0xF35C4, s32) = 0;
        HW(h, 0xF36AC, s32) = 0;
        HW(h, 0xF36A4, s32) = 0;
        HW(h, 0xF366D, u8) = 0;
        Actor_SetState(&h->c.a, &D_003B02B0);
        func_00130AF0(h, 0, 0);
        return;
    }
    HW(h, 0xF3710, u8) = 0;
    HW(h, 0xF3718, s32) = 6;
    HW(h, 0xF3730, s32) = 0;
    HW(h, 0xF3734, s32) = 0;
    HW(h, 0xF3738, s32) = 0;
    HW(h, 0xF373C, s32) = 0;
    HW(h, 0xF3712, s16) = 0;
    Actor_SetState(&h->c.a, &D_003B0280);
    func_002DDE20(h->c.motion, 0, -1);
    VCALL(h->c.motion, 0x50, void (*)(void *, Hewie *))(h->c.motion, h);
    func_00130AF0(h, 0x83, 0);
}

extern void func_002A8440(void *noise, s32 level, s32 room, u32 tri, s32 exitId);
extern void func_00122C20(Actor *a, s32 sound, s32, s32, s32, void *);

#define HEWIE_LAST_SOUND(h) HW(h, 0xF35A4, s32)
#define HEWIE_SOUND_T(h) HW(h, 0xF35A8, s16)      /* frames since then (up to 3000) */

/* Make sound `snd` (not within 10 frames of the last one; some repeat only after 40..60
 * frames). Barks 0x65/0x66 (loud) and 0x5D/0x5E also make a noise others can hear. */
void func_0013A430(Hewie *h, s32 snd) {
    s32 prev = HEWIE_LAST_SOUND(h);

    if (prev != 0x59 && prev != 0x58 && prev != 0x70 && prev != 0x6F && HEWIE_SOUND_T(h) < 10) {
        return;
    }
    switch (snd) {
    case 0x65: case 0x66:
        func_002A8440((u8 *)gProgress + 0x788, 0x80, h->c.a.room, h->c.a.navTri, 0xFFFF);
        break;
    case 0x5D: case 0x5E:
        func_002A8440((u8 *)gProgress + 0x788, 0x1B, h->c.a.room, h->c.a.navTri, 0xFFFF);
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
    func_00122C20(&h->c.a, snd, 5, 0, 0, NULL);
    HEWIE_LAST_SOUND(h) = snd;
    HEWIE_SOUND_T(h) = 0;
}

extern f32 func_00123A70(Actor *a, u32 tri, const f32 *pos, u32 mask, f32 angle, f32 dist);

#define F_PI 0x1.921fb6p+1f   /* 0x40490FDB */

/* Best heading near `yaw` for walking `dist`: tries yaw +- `from`..`to` degrees in steps of `step`
 * (left and right, starting on a random side), stops early at a full-length result. */
f32 func_00137720(Hewie *h, f32 yaw, f32 dist, s32 from, s32 to, s32 step) {
    f32 best = func_00123A70(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, yaw, dist);
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
                f32 a = func_002E2D00(yaw + sign * ang);
                f32 r = func_00123A70(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, a, dist);

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
                f32 a = func_002E2D00(yaw + sign * ang);
                f32 r = func_00123A70(&h->c.a, h->c.a.navTri, h->c.a.pos, NAV_NONE, a, dist);

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

extern s32 func_00177670(Progress *p, s32 n);
extern void func_002DDC60(void *motion, s32 anim, s32 blend, s32);

/* Start his standing animation (0 normal, 3 / 4 / 5 by progress state and mode, 6 in condition
 * 1, 4 during actions 8 / 0xA) unless it is already playing; `blend` -1: cut. */
void func_00143550(Hewie *h, s32 blend) {
    Progress *p;
    s32 cur, a;

    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
    p = gProgress;
    cur = MOTION_ANIM(h->c.motion);
    if ((func_00177620(p) & 0xFF) == 2 && HEWIE_MODE(h) == 3) {
        a = 4;
    } else if (h->c.a.unkC4 == 1) {
        a = 6;
    } else if ((func_00177620(p) & 0xFF) == 2 && HEWIE_MODE(h) == 2) {
        a = 5;
    } else if ((func_00177620(p) & 0xFF) && func_00177670(p, 4)) {
        a = 4;
    } else if (func_00177620(p) & 0xFF) {
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
        func_002DDC60(h->c.motion, a, blend, -1);
    } else {
        func_002DDED0(h->c.motion, a, -1);
    }
}

/* ---- small behaviour pieces ---- */

extern s32 func_00141C00(Hewie *h, s32 kind);
extern void func_0013C300(Hewie *h);

#define MOTION_EVENTS(m) (*(s32 *)((u8 *)MOTION_PTR(m, 0x6A4) + 0x18))

s32 func_001480B0(Hewie *h) {
    return func_00141C00(h, 2);
}

s32 func_0015F750(Hewie *h) {
    return func_00141C00(h, 4);
}

void func_0014F5A0(Hewie *h) {
}

void func_00154D40(Hewie *h) {
}

void func_00154DB0(Hewie *h) {
}

void func_00154E60(Hewie *h) {
}

s32 func_001689F0(Hewie *h) {
    return 0;
}

/* A loud noise where he is (no triangle). */
void func_00154E40(Hewie *h) {
    func_002A8440((u8 *)gProgress + 0x788, 0x80, h->c.a.room, NAV_NONE, 0xFFFF);
}

/* Set the animation to play (+0xF35B8) and (if >= 0) +0xF35B4. */
void func_001654E0(Hewie *h, s32 a, s32 anim) {
    if (a >= 0) {
        HW(h, 0xF35B4, s32) = a;
    }
    HW(h, 0xF35B8, s32) = anim;
}

/* Start that animation if it is not playing. */
void func_0014F5B0(Hewie *h) {
    if (MOTION_ANIM(h->c.motion) != HW(h, 0xF35B8, s32)) {
        func_002DDED0(h->c.motion, HW(h, 0xF35B8, s32), -1);
    }
}

/* On the animation's event 0x20: (clear +0x2B and) continue. */
void func_00153AB0(Hewie *h) {
    if ((MOTION_EVENTS(h->c.motion) & 0x20) != 0) {
        h->c.a.unk2B = 0;
        func_0013C300(h);
    }
}

/* Pending (+0xF3559): go to action 0x83, unless already in it. */
void func_001602A0(Hewie *h) {
    if (HW(h, 0xF3559, u8) == 1 && HEWIE_ACTION(h) != 0x83) {
        func_00130AF0(h, 0x83, 0);
    }
}

/* Back to his default action. */
void func_00154D50(Hewie *h) {
    Hewie_ToDefault(h);
}

/* Switch his behaviour (the pointer to member at +0xF35D0). */
static inline void Hewie_SetBehaviour(Hewie *h, const PTMF *s) {
    *HEWIE_STATE(h) = *s;
}

#define ANIM_DONE(h) ((MOTION_EVENTS((h)->c.motion) & 0x20) != 0)   /* animation event 0x20 */

extern s32 func_00140CD0(Hewie *h, s32 kind);
extern void func_001431F0(Hewie *h);
extern const PTMF D_003B1B58, D_003B1B48, D_003B1B38, D_003B19A8, D_003B1778, D_003B1B68;

void func_0014E190(Hewie *h) {
    if (func_00140CD0(h, 3) == 0) {
        func_001431F0(h);
        Hewie_SetBehaviour(h, &D_003B1B58);
    }
}

void func_0014E200(Hewie *h) {
    if (ANIM_DONE(h)) {
        HW(h, 0xF36BC, s32) = 90;
        Hewie_SetBehaviour(h, &D_003B1B48);
    }
}

void func_00154DC0(Hewie *h) {
    if (HW(h, 0xF35C8, s32) == 0) {
        Hewie_ToDefault(h);
    }
}

void func_0014E280(Hewie *h) {
    if (func_00140CD0(h, 1) == 0) {
        func_002DDED0(h->c.motion, 0x1C04, -1);
        Hewie_SetBehaviour(h, &D_003B1B38);
    }
}

void func_00151740(Hewie *h) {
    if (func_00140CD0(h, 1) == 0) {
        func_002DDED0(h->c.motion, 0x1C04, -1);
        Hewie_SetBehaviour(h, &D_003B19A8);
    }
}

void func_0015BD10(Hewie *h) {
    if (func_00140CD0(h, 1) == 0) {
        func_002DDED0(h->c.motion, 0x1C04, -1);
        Hewie_SetBehaviour(h, &D_003B1778);
    }
}

void func_0014E110(Hewie *h) {
    if (ANIM_DONE(h)) {
        func_00141C00(h, 1);
        Hewie_SetBehaviour(h, &D_003B1B68);
    }
}

/* Animation over: back to his default action. */
void func_001506A0(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_ToDefault(h);
    }
}

void func_0015AD90(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_ToDefault(h);
    }
}

void func_0015BC90(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_ToDefault(h);
    }
}

void func_0014DFB0(Hewie *h) {
    if (ANIM_DONE(h)) {
        h->c.a.unk2D = 0;
        Hewie_ToDefault(h);
    }
}

/* Start action `act` as adjusted to his situation. */
static inline void Hewie_Start(Hewie *h, s32 act) {
    func_00130AF0(h, func_0013B2C0(h, act), 0);
}

#define HEWIE_NEXT(h) HW(h, 0xF3570, s32)     /* action to continue with */

void func_00149D40(Hewie *h) {
    if (func_00140CD0(h, 3) == 0) {
        h->c.unkE1 = 1;
        func_00141C00(h, 3);
        Hewie_ToDefault(h);
    }
}

void func_0014B860(Hewie *h) {
    if (func_00140CD0(h, 0) == 0) {
        h->c.unkE1 = 1;
        func_00141C00(h, 0);
        Hewie_ToDefault(h);
    }
}

extern u32 func_00177770(Progress *p, s32 n);   /* u8 */

/* Free to take a command: idle, a command queued (+0xF356C > 0), nothing pending. */
s32 func_00138FD0(Hewie *h) {
    if (!h->c.unkE0 && h->c.moveMode == 0 && HW(h, 0xF356C, s32) != 0 && !(HW(h, 0xF356C, s32) & 0x80000000)
        && !(func_00177770(gProgress, 1) & 0xFF) && h->c.state[0] == 0) {
        return 1;
    }
    return 0;
}

void func_00150610(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_Start(h, 0x52);
    }
    HW(h, 0xF3558, u8) = 1;
}

void func_001532B0(Hewie *h) {
    if (ANIM_DONE(h)) {
        Hewie_Start(h, HEWIE_NEXT(h));
    }
}

void func_001519B0(Hewie *h) {
    if (--HW(h, 0xF36B4, s32) == 0) {
        Hewie_Start(h, HEWIE_NEXT(h));
    }
    func_00141C00(h, 3);
}
