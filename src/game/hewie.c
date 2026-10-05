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

/* Add to his trust in Fiona (0..10000) and recompute its level 0..7. From level 2 progress flag
   0x11 is cleared, from level 3 flag 0x1D. */
void func_0013D1F0(Hewie *h, s32 add) {
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
extern const s32 D_003B1350[];   /* by +0xF35CC (normal) */
extern const s32 D_003B1370[];   /* by +0xF35CC (difficulty 1) */
extern const PTMF D_003B02B0;    /* idle state */
extern const PTMF D_003B0280;    /* special-mode state */

static inline void Hewie_StartScene(Hewie *h, Progress *p) {
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
    Hewie_StartScene(h, p);
}

/* (Re)start: in play idle with action 0, in the special mode the special state, action 0x83. */
void func_00136620(Hewie *h) {
    Hewie_StartScene(h, gProgress);
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
extern s32 func_001367B0(Hewie *h);   /* u8 */
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
static inline s32 Hewie_FreeForCommand(Hewie *h) {
    if (!h->c.unkE0 && h->c.moveMode == 0 && HW(h, 0xF356C, s32) != 0 && !(HW(h, 0xF356C, s32) & 0x80000000)
        && !(func_00177770(gProgress, 1) & 0xFF) && h->c.state[0] == 0) {
        return 1;
    }
    return 0;
}

s32 func_00138FD0(Hewie *h) {
    return Hewie_FreeForCommand(h);
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

extern const PTMF D_003B1998, D_003B1B78;
extern s32 func_001391E0(Hewie *h, s32 praise, s8 by);   /* u8 */

void func_00151A60(Hewie *h) {
    if (--HW(h, 0xF36B4, s32) == 0) {
        if (HW(h, 0xF3604, s32) != 4) {
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 10;
        }
        HW(h, 0xF36B4, s32) = 15;
        Hewie_SetBehaviour(h, &D_003B1998);
    }
    func_00141C00(h, 3);
}

/* Follow the animation's root motion, keeping the height it gives (not during action 0x47). */
void func_0014B4D0(Hewie *h) {
    if (HEWIE_ACTION(h) == 0x47) {
        HW(h, 0xF3558, u8) = 1;
        return;
    }
    if (h->c.a.unk2B == 1) {
        sceVu0FVECTOR root;
        f32 y = h->c.a.pos[1];

        func_001F6370(h->c.motion, root, 0.0f);
        sceVu0ApplyMatrix(root, h->c.a.rot, root);
        func_001247E0(&h->c.a, root);
        h->c.a.pos[1] = y + root[1];
        HW(h, 0xF3558, u8) = 1;
        HW(h, 0xF3582, u8) = 0;
    }
}

void func_001531F0(Hewie *h) {
    if (ANIM_DONE(h) && MOTION_ANIM(h->c.motion) == 0x1C01) {
        Hewie_ToDefault(h);
    } else if (func_00140CD0(h, 1) == 0) {
        func_002DDED0(h->c.motion, 0x1C01, -1);
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

s32 func_00137650(Hewie *h, Character *other) {
    return Hewie_WithChar(h, other);
}

void func_0014EB40(Hewie *h) {
    if (--HW(h, 0xF36BC, s32) == 0) {
        HW(h, 0xF368C, s32) = 0;
        HW(h, 0xF36A8, s32) = HEWIE_ACTION(h);
        func_001391E0(h, 1, 1);
        Hewie_ToDefault(h);
    }
    func_00141C00(h, 3);
}

#define HEWIE_TARGET(h) HW(h, 0xF3544, Character *)

void func_0014E040(Hewie *h) {
    if (func_00140CD0(h, 1) == 0) {
        func_00141C00(h, 1);
        HW(h, 0xF36BC, s32) = 90;
        HEWIE_TARGET(h) = gCharPlayer;
        if (HW(h, 0xF3604, s32) != 0) {
            HW(h, 0xF3604, s32) = 0;
            HW(h, 0xF3608, s32) = 10;
        }
        Hewie_SetBehaviour(h, &D_003B1B78);
    }
}

/* His move sub-mode: following a path (mode 6): 0x16 / 0x17 by +0xF3590; standing (mode 0):
 * by his animation group. */
void func_00144A60(Hewie *h) {
    if (h->c.moveMode == 6) {
        h->c.moveSub = (HW(h, 0xF3590, u8) == 1) ? 0x16 : 0x17;
        return;
    }
    if (h->c.moveMode != 0) {
        return;
    }
    switch (func_001669A0(h)) {
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
s32 func_0013D4A0(Hewie *h, s32 once) {
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

extern const PTMF D_003B1C50, D_003B1CD0, D_003B1CC0;

void func_0014B780(Hewie *h) {
    u8 stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;

    if ((stopped ^ 1) == 0) {
        if (func_001669A0(h) != 0) {
            func_00143550(h, -1);
        } else {
            func_002DDED0(h->c.motion, 0x1C03, -1);
            HW(h, 0xF36B8, s32) = 30;
            Hewie_SetBehaviour(h, &D_003B1C50);
        }
    }
    HW(h, 0xF3558, u8) = 1;
}

/* Character slot flags in the progress data (+0x1020 + 0x10 * slot). */
#define PROGRESS_SLOT_FLAGS(p, slot) (*((u8 *)(p) + 0x1020 + 0x10 * (slot)))

void func_001522F0(Hewie *h) {
    u8 b = PROGRESS_SLOT_FLAGS(gProgress, SLOT_U8(h));

    if ((b & 5) != 0 || h->c.unk104[0] != 0) {
        if (b & 4) {
            HW(h, 0xF3688, s16) = 300;
        }
        HW(h, 0xF36B0, s32) = 0xFF;
        func_00122C20(&h->c.a, 0x6C, 5, 0, 0, NULL);
    }
    Hewie_ToDefault(h);
}

void func_0014B590(Hewie *h) {
    u8 stopped;
    s32 g;

    if (h->c.a.unkC4 == 2) {
        if (func_00140CD0(h, 10) == 0) {
            h->c.unkE1 = 1;
        }
        return;
    }
    stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;
    if ((stopped ^ 1) != 0) {
        return;
    }
    g = func_001669A0(h);
    if (g == 0xA || g == 9 || g == 8) {
        func_00143550(h, -1);
    } else if (g == 0) {
        h->c.unkE1 = 1;
    } else {
        func_00141C00(h, 0);
    }
}

void func_00149DD0(Hewie *h) {
    if (!(func_00177620(gProgress) & 0xFF) && h->c.a.unkC4 != 1) {
        if (func_00140CD0(h, 3) == 0) {
            func_001431F0(h);
            Hewie_SetBehaviour(h, &D_003B1CD0);
        }
    } else if (func_00140CD0(h, 0) == 0) {
        func_001431F0(h);
        Hewie_SetBehaviour(h, &D_003B1CC0);
    }
}

extern const PTMF D_003B1958, D_003B1948, D_003B1D00;

void func_00153350(Hewie *h) {
    if (!(func_00177620(gProgress) & 0xFF) && h->c.a.unkC4 != 1) {
        if (func_00140CD0(h, 3) == 0) {
            func_001431F0(h);
            Hewie_SetBehaviour(h, &D_003B1958);
        }
    } else if (func_00140CD0(h, 0) == 0) {
        func_001431F0(h);
        Hewie_SetBehaviour(h, &D_003B1948);
    }
}

void func_00149EC0(Hewie *h) {
    u8 stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;

    if ((stopped ^ 1) != 0) {
        return;
    }
    if (MOTION_ANIM(h->c.motion) == 0x1300) {
        if (!(func_00177620(gProgress) & 0xFF)) {
            func_002DDC60(h->c.motion, 0, 5, -1);
        } else {
            func_002DDC60(h->c.motion, 3, 5, -1);
        }
        return;
    }
    h->c.unkE1 = 1;
    Hewie_ToDefault(h);
}

void func_00149270(Hewie *h) {
    switch (HEWIE_ACTION(h)) {
    case 0x4B:
        func_002DDED0(h->c.motion, 0x1C05, -1);
        break;
    case 0x4A:
        func_002DDED0(h->c.motion, 0x1D00, -1);
        HW(h, 0xF36BC, s32) = (h->c.hp == h->c.hpMax) ? 1 : 0;
        break;
    case 0x49:
        func_002DDED0(h->c.motion, 0x1C04, -1);
        break;
    }
    Hewie_SetBehaviour(h, &D_003B1D00);
}

void func_0014B680(Hewie *h) {
    u8 stopped;

    if (func_001669A0(h) != 0) {
        if (--HW(h, 0xF36B8, s32) == 0) {
            func_00143550(h, -1);
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
    func_001F6370(h->c.motion, root, 0.0f);
    k = VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    *(s32 *)&root[1] = 0;
    root[2] = root[2] * k;
    sceVu0ApplyMatrix(root, h->c.a.rot, root);
    func_001247E0(&h->c.a, root);
    HW(h, 0xF3558, u8) = 1;
    h->c.a.navMask &= ~0x80001;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

void func_00147480(Hewie *h) {
    if (func_00140CD0(h, 0) == 0) {
        func_00141C00(h, 0);
        func_00130AF0(h, 0, 0);
    }
    Hewie_ForcedMove(h);
}

/* Can he take a command now: active in the room being played, idle, not angry (mode 3), a
 * command queued (+0xF356C > 0; any in condition 2), nothing pending. */
s32 func_00138EC0(Hewie *h) {
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
    if ((func_00177770(p, 1) & 0xFF) || h->c.state[0] != 0) {
        return 0;
    }
    return 1;
}

void func_00157360(Hewie *h) {
    if (ANIM_DONE(h)) {
        HW(h, 0xF3688, s16) = 300;
        if (HW(h, 0xF36B4, s32) == 0) {
            Hewie_ToDefault(h);
        } else if (MOTION_ANIM(h->c.motion) == 0x2213) {
            if (h->c.a.unkC4 != 2) {
                func_002DDED0(h->c.motion, 0x1003, -1);
            } else {
                func_0013C300(h);
            }
        } else if (MOTION_ANIM(h->c.motion) == 0x1003) {
            func_0013C300(h);
        }
    }
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

extern void func_002E3130(sceVu0FMATRIX out, const f32 *pos, f32 angle);
extern void func_002E2DD0(f32 *out, sceVu0FMATRIX m, const f32 *v);

/* A point 24 units out from door `door` on his side of it (if the door is usable). */
void func_00144940(Hewie *h, s32 door, f32 *out) {
    VObject *doors;
    sceVu0FMATRIX m;
    sceVu0FVECTOR at;
    f32 off[4] __attribute__((aligned(16)));
    f32 yaw;
    s32 side;

    if (!(VCALL(D_0044E558, 0x40, u32 (*)(VObject *, s32))(D_0044E558, door) & 0xFF)) {
        return;
    }
    doors = D_0044E558;
    yaw = VCALL(doors, 0x3C, f32 (*)(VObject *, s32))(doors, door);
    side = VCALL(doors, 0x18, s32 (*)(VObject *, s32, f32 *))(doors, door, h->c.a.pos);
    VCALL(doors, 0x44, void (*)(VObject *, s32))(doors, door);
    VCALL(doors, 0x34, void (*)(VObject *, s32, f32 *))(doors, door, at);
    if (side == 0) {
        yaw = func_002E2D00(F_PI + yaw);
    }
    *(s32 *)&off[0] = 0;
    *(s32 *)&off[1] = 0;
    off[2] = 24.0f;
    func_002E3130(m, at, yaw);
    func_002E2DD0(out, m, off);
}

extern s32 func_00125D80(Character *c);

/* 1 unless he is with Fiona's party in the room being played and cannot reach her. */
s32 func_001364F0(Hewie *h) {
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
    if (h->c.a.unk2D || h->c.a.unk2B || (func_00125D80(&h->c) & 0xFF)) {
        return 0;
    }
    if (gCharPlayer == NULL || !gCharPlayer->a.active) {
        return 1;
    }
    room = gCharPlayer->a.room;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return 1;
    }
    return ((func_0013C1E0(h, gCharPlayer->a.navTri, gCharPlayer->a.pos) & 0xFF) == 1) ? 1 : 0;
}

/* A random idle action: 1 (50%), 4 (15%), 5 (35%). */
void func_00140050(Hewie *h) {
    s32 r = (s32)(100.0f * RNG01());

    if (r < 50) {
        Hewie_Start(h, 1);
    } else if (r < 65) {
        Hewie_Start(h, 4);
    } else {
        Hewie_Start(h, 5);
    }
}


void func_0014DE70(Hewie *h) {
    if (HW(h, 0xF3604, s32) != 4) {
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 10;
    }
    if (MOTION_ANIM(h->c.motion) == 9) {
        if (*((u8 *)gProgress + 0x1FBEC1) == 1 && (func_001367B0(h) & 0xFF) == 1) {
            func_00130AF0(h, 0, 0);
            return;
        }
        if (HW(h, 0xF36B4, s32) == 0) {
            HW(h, 0xF3585, u8) = 1;
        } else {
            HW(h, 0xF36B4, s32) -= 1;
        }
    } else if (func_00140CD0(h, 0) == 0) {
        func_002DDED0(h->c.motion, 9, -1);
    }
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* Helper `kind` done: action `act` -> 2, otherwise 1 (or 5 with his pool in use). */
static inline void Hewie_AfterHelper(Hewie *h, s32 kind, s32 act) {
    if (func_00140CD0(h, kind) == 0) {
        func_00141C00(h, kind);
        if (HEWIE_ACTION(h) == act) {
            Hewie_Start(h, 2);
        } else if (HW(h, 0xF3598, s32) == 0) {
            Hewie_Start(h, 1);
        } else {
            Hewie_Start(h, 5);
        }
    }
}

void func_00155670(Hewie *h) {
    Hewie_AfterHelper(h, 2, 0x2B);
}

void func_001557B0(Hewie *h) {
    Hewie_AfterHelper(h, 1, 0x29);
}

void func_001558F0(Hewie *h) {
    Hewie_AfterHelper(h, 0, 0x27);
}

void func_0015F760(Hewie *h) {
    if (!(Hewie_WithChar(h, gCharPlayer) & 0xFF)) {
        Hewie_ToDefault(h);
    } else {
        func_00141C00(h, 3);
    }
}

extern const PTMF D_003B1D30;
extern void func_0013E680(Hewie *h);
extern u32 D_0047E37C;     /* pad buttons */
extern u32 D_0047E374;     /* pad buttons (d-pad bits 4..7) */
extern f32 D_0047E3A0[4];  /* left stick */

/* Is the player steering (buttons, stick / d-pad beyond 0.8) or has a pad command (+0xF3798)? */
s32 func_001367B0(Hewie *h) {
    u32 b = D_0047E37C;
    sceVu0FVECTOR v;
    u32 d;
    f32 x, z;

    if ((b >> 14 & 1) || (b >> 13 & 1) || (b >> 12 & 1) || (b >> 15 & 1) || (b >> 10 & 1) || (b >> 11 & 1)) {
        return 1;
    }
    sceVu0CopyVector(v, D_0047E3A0);
    d = D_0047E374;
    x = v[0] + (f32)(s32)((d >> 5 & 1) - (d >> 7 & 1));
    v[0] = x;
    z = v[2] + (f32)(s32)((d >> 6 & 1) - (d >> 4 & 1));
    v[2] = z;
    if (!(__builtin_sqrtf(z * z + x * x) <= 0x1.99999ap-1f /* 0.8 */)) {
        return 1;
    }
    return HW(h, 0xF3798, s32) != -1;
}

void func_00147580(Hewie *h) {
    if (func_00140CD0(h, 10) == 0) {
        u32 r;

        h->c.a.unkC4 = 2;
        func_00141C00(h, 10);
        r = func_001367B0(h) & 0xFF;
        if (r == 1) {
            h->c.a.unk2D = 0;
            Hewie_SetBehaviour(h, &D_003B1D30);
        }
    }
    Hewie_ForcedMove(h);
}

void func_00148880(Hewie *h) {
    if (h->c.a.disabled) {
        h->c.a.unk2D = 0;
        func_0013E680(h);
        return;
    }
    if (func_00140CD0(h, 0) == 0) {
        h->c.a.unk2D = 0;
        func_00141C00(h, 0);
        Hewie_ToDefault(h);
    }
    Hewie_ForcedMove(h);
}

/* Take him through exit `exit` into the next room (off screen): room, side, door; disabled. With
 * progress flag 0x8000 he gets up again (1 health) if he was down there. -1: no such exit. */
s32 func_0013AAE0(Hewie *h, s32 exit) {
    VObject *rooms;
    u32 d;
    s32 room;

    HW(h, 0xF368A, u8) = 0;
    d = VCALL(D_0044E568, 0x14, u32 (*)(VObject *, s32, s32))(D_0044E568, h->c.a.room, exit) & 0xFF;
    if (d == 0xFF) {
        return -1;
    }
    rooms = D_0044E568;
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
        func_002DDED0(h->c.motion, anim, -1);
    }
}

/* His walking animation: 0x200; limping (condition 1) 0x206; 0x205 by progress state. */
void func_00143400(Hewie *h) {
    s32 cur = MOTION_ANIM(h->c.motion);

    if (HEWIE_MODE(h) == 3) {
        Hewie_PlayIfNot(h, cur, 0x200);
    } else if (h->c.a.unkC4 == 1) {
        Hewie_PlayIfNot(h, cur, 0x206);
    } else if (HEWIE_MODE(h) == 2) {
        Hewie_PlayIfNot(h, cur, 0x200);
    } else if ((func_00177620(gProgress) & 0xFF) == 1) {
        Hewie_PlayIfNot(h, cur, 0x205);
    } else {
        Hewie_PlayIfNot(h, cur, 0x200);
    }
}

void func_001515E0(Hewie *h) {
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
            func_00138AD0(h, 2, -1);
            HW(h, 0xF3684, s16) = 0;
        }
    }
    Hewie_ToDefault(h);
}

#define HEWIE_SPOT(h) ((f32 *)((u8 *)(h) + 0xF36E0))   /* a point he moves to / looks at */

/* Off the walkable part of the mesh: push back towards his spot (0.4 a frame) and turn to it;
 * back on it: default action. */
void func_00146AE0(Hewie *h) {
    NavTri *t = NavMesh_Tri(D_0044E570, h->c.a.navTri);

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
        func_001247E0(&h->c.a, d);
        {
            f32 yaw = func_001244D0(&h->c.a, HEWIE_SPOT(h));

            HW(h, 0xF3614, s32) = 0;
            HW(h, 0xF3618, f32) = func_002E2D00(yaw - h->c.a.angle[1]);
        }
        func_00141C00(h, 7);
    }
}

/* Random level 0..2 (out of 16), lower thresholds with difficulty 1 / progress state. */
s32 func_001382F0(Hewie *h) {
    s32 r = (s32)(16.0f * RNG01());
    Progress *p = gProgress;

    if ((Progress_GetVar(p, 0x27) & 0xFF) == 1) {
        if (!(func_00177620(p) & 0xFF)) {
            return (r < 8) ? 0 : (r < 13) ? 1 : 2;
        }
        return (r < 5) ? 0 : (r < 13) ? 1 : 2;
    }
    if (!(func_00177620(p) & 0xFF)) {
        return (r < 10) ? 0 : (r < 15) ? 1 : 2;
    }
    return (r < 5) ? 0 : (r < 15) ? 1 : 2;
}

extern void func_002DDB30(void *motion, s32 anim);

/* His secondary (overlay) animation by +0xF3640: 0x1F00 / 0x1F01 / 0x1F02; 3: change at random
 * every 30..450 frames. */
void func_0013FC70(Hewie *h) {
    switch (HW(h, 0xF3640, s32)) {
    case 0:
        func_002DDB30(h->c.motion, 0x1F00);
        break;
    case 1:
        func_002DDB30(h->c.motion, 0x1F01);
        break;
    case 2:
        func_002DDB30(h->c.motion, 0x1F02);
        break;
    case 3:
        if (--HW(h, 0xF3644, s32) < 0) {
            HW(h, 0xF3644, s32) = (s32)(15.0f * RNG01()) * 30 + 30;
            if (RNG01() < 0.5f) {
                func_002DDB30(h->c.motion, 0x1F00);
            } else {
                func_002DDB30(h->c.motion, 0x1F01);
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

void func_0015BB20(Hewie *h) {
    u8 stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;

    if ((u8)(stopped ^ 1) == 1) {
        return;
    }
    if (ANIM_DONE(h) && MOTION_ANIM(h->c.motion) == 0x1C02) {
        Hewie_Start(h, HW(h, 0xF366D, u8) == 0 ? 6 : 8);
    } else if (func_00140CD0(h, 0) == 0) {
        Hewie_Mark4(h);
        func_002DDED0(h->c.motion, 0x1C02, -1);
    }
}

void func_0015D320(Hewie *h) {
    if (--HW(h, 0xF36B8, s32) == 0) {
        Hewie_ToDefault(h);
        return;
    }
    if (!(func_00124490(&h->c.a, gCharPlayer->a.pos) <= 20.0f)) {
        Hewie_Start(h, HEWIE_ACTION(h));
    }
    HEWIE_TARGET(h) = gCharPlayer;
    if (HW(h, 0xF3604, s32) != 0) {
        HW(h, 0xF3604, s32) = 0;
        HW(h, 0xF3608, s32) = 10;
    }
    func_00141C00(h, 3);
}

/* Fiona is with him and he can take a command (idle, not angry, one queued, nothing pending). */
s32 func_00139060(Hewie *h) {
    if ((Hewie_WithChar(h, gCharPlayer) & 0xFF) != 1 || h->c.unkE0 || h->c.moveMode != 0 || HEWIE_MODE(h) == 3
        || HW(h, 0xF356C, s32) == 0 || (HW(h, 0xF356C, s32) & 0x80000000)
        || (func_00177770(gProgress, 1) & 0xFF) || h->c.state[0] != 0) {
        return 0;
    }
    return 1;
}

extern u32 func_00124480(Actor *a, const f32 *target, u32 mask);

void func_0015F8A0(Hewie *h) {
    s32 room = h->c.a.room;
    s32 g;

    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        HW(h, 0xF3559, u8) = 1;
        return;
    }
    g = func_001669A0(h);
    if (g == 0xA || g == 9 || g == 8) {
        sceVu0FVECTOR root, to;
        f32 k;

        func_001F6370(h->c.motion, root, 0.0f);
        k = VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
        root[2] = root[2] * k;
        sceVu0ApplyMatrix(root, h->c.a.rot, root);
        sceVu0AddVector(to, h->c.a.pos, root);
        if (func_00124480(&h->c.a, to, NAV_NONE) == NAV_NONE) {
            u8 stopped = (MOTION_SPEED(h->c.motion) <= 0.0f) ? 1 : 0;

            if ((u8)(stopped ^ 1) == 0) {
                func_00143550(h, -1);
            }
        }
    }
    if (func_00140CD0(h, 4) == 0) {
        func_00141C00(h, 4);
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

void func_001396B0(Hewie *h) {
    VObject *rng = D_0044E550;

    if (!Hewie_Roll(h, rng, 3, HW(h, 0xF3693, s8), 0xF369F)) {
        Hewie_Roll(h, rng, 4, HW(h, 0xF3694, s8), 0xF36A0);
    }
}

extern const PTMF D_003B1C80;

void func_0014A790(Hewie *h) {
    sceVu0FVECTOR root;
    f32 k;
    s32 r;

    func_001F6370(h->c.motion, root, 0.0f);
    k = VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    *(s32 *)&root[1] = 0;
    root[2] = root[2] * k;
    sceVu0ApplyMatrix(root, h->c.a.rot, root);
    func_001247E0(&h->c.a, root);
    HW(h, 0xF3558, u8) = 1;
    VCALL(D_0044E570, 0xC, void (*)(NavMesh *, s32, f32 *))(D_0044E570, h->c.unk104[0], (f32 *)h->c.unk110);
    r = func_00127140(&h->c, 0, h->c.unk104[0], (f32 *)h->c.unk110);
    if (r > 0) {
        r = func_001270F0(&h->c);
    }
    if (r > 0) {
        Hewie_Mark4(h);
        Hewie_SetBehaviour(h, &D_003B1C80);
    } else {
        Hewie_ToDefault(h);
    }
}

void func_00154E70(Hewie *h) {
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
            func_0013E680(h);
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
void func_00165510(Hewie *h, s32 st) {
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
    func_002DDE20(h->c.motion, HW(h, 0xF36F0, s32), -1);
    func_0013D1F0(h, 0);
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
s32 func_001386D0(Hewie *h, s32 kind) {
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

extern const PTMF D_003B1A08, D_003B19F8;

/* Behaviour: (state 7 ends it) go for the pursuer if he's here. */
void func_00150290(Hewie *h) {
    if (h->c.state[0] == 7) {
        h->c.state[0] = 0;
        Hewie_ToDefault(h);
        return;
    }
    if (Hewie_CharHere(h, gCharPursuer)) {
        Hewie_SetBehaviour(h, &D_003B1A08);
    } else {
        Hewie_ToDefault(h);
    }
}

extern s32 func_00177890(Progress *p, s32 a, s32 b, u8 from, u8 to, s32 c, f32 d);   /* u8 */

/* Behaviour: target the pursuer if he's here and reachable. */
void func_00150450(Hewie *h) {
    if (Hewie_CharHere(h, gCharPursuer) == 1) {
        HEWIE_TARGET(h) = gCharPursuer;
        if ((func_00177890(gProgress, 2, 7, h->c.a.slot, gCharPursuer->a.slot, 0, 0.0f) & 0xFF) == 1) {
            Hewie_SetBehaviour(h, &D_003B19F8);
            return;
        }
    }
    Hewie_ToDefault(h);
}

extern f32 func_0031C248(f32 x);   /* sinf */
extern f32 func_00124530(Actor *a, f32 target, f32 step);
extern const PTMF D_003B1C90, D_003B1CA0, D_003B1CB0, D_003B19C8;

/* Behaviour: once stopped, turn to heading unk10C: snap if within 1 degree, else start the
 * turning animation (eased by the angle, func_00149FB0). */
void func_0014A180(Hewie *h) {
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
    d = func_002E2D00(*(f32 *)&h->c.unk104[2] - h->c.a.angle[1]);
    ad = d;
    if (ad <= 0.0f) {
        ad = -ad;
    }
    if (ad < 0x1.1df46ap-6f /* 1 deg */) {
        target = *(f32 *)&h->c.unk104[2];
        h->c.a.angle[1] = target;
        sceVu0UnitMatrix(h->c.a.rot);
        sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, target);
        Hewie_SetBehaviour(h, &D_003B1C90);
        return;
    }
    if (!(func_00177620(gProgress) & 0xFF)) {
        func_002DDED0(h->c.motion, 0x1300, 0);
    } else {
        func_002DDED0(h->c.motion, 0x1300, 3);
    }
    *(s32 *)((u8 *)MOTION_PTR(h->c.motion, 0x6A4) + 0x1C) = 0;
    if (d <= 0.0f) {
        d = -d;
    }
    HW(h, 0xF36C4, f32) = 0x1.028f5cp+0f /* 1.01 */ * d;
    Hewie_SetBehaviour(h, &D_003B1CA0);
}

/* Behaviour: turning to heading unk10C, playback rate and step eased by a sine of the
 * remaining angle (of the total +0xF36C4). */
void func_00149FB0(Hewie *h) {
    f32 d, s, rate, step;

    HW(h, 0xF3558, u8) = 1;
    HW(h, 0xF3604, s32) = 8;
    HW(h, 0xF3608, s32) = 0;
    HW(h, 0xF3614, s32) = 0;
    HW(h, 0xF3618, f32) = func_002E2D00(*(f32 *)&h->c.unk104[2] - h->c.a.angle[1]);
    d = func_002E2D00(*(f32 *)&h->c.unk104[2] - h->c.a.angle[1]);
    if (d <= 0.0f) {
        d = -d;
    }
    s = func_0031C248(d * F_PI / HW(h, 0xF36C4, f32));
    rate = 2.0f * (s * HW(h, 0xF36C4, f32)) / F_PI;
    if (!(rate <= 0x1.333334p-1f /* 0.6 */)) {
        rate = 0x1.333334p-1f;
    }
    *(f32 *)((u8 *)MOTION_PTR(h->c.motion, 0x6A4) + 0x1C) = rate;
    step = 0x1.99999ap-5f /* 0.05 */ * HW(h, 0xF36C4, f32) * s;
    if (step < 0x1.1df46ap-7f /* 0.5 deg */) {
        step = 0x1.1df46ap-7f;
    }
    if (func_00124530(&h->c.a, *(f32 *)&h->c.unk104[2], step) == 0.0f) {
        Hewie_SetBehaviour(h, &D_003B1CB0);
    }
}

/* Behaviour: back to normal unless Fiona is here and not busy (then D_003B19C8). */
void func_00150FD0(Hewie *h) {
    if (Hewie_CharHere(h, gCharPlayer) && gCharPlayer->unkE0 != 1) {
        Hewie_SetBehaviour(h, &D_003B19C8);
    } else {
        Hewie_ToDefault(h);
    }
}

extern const PTMF D_003B17F8;

/* Behaviour: (state 7: action 0x5A) go for the pursuer if he's here. */
void func_00159DC0(Hewie *h) {
    if (h->c.state[0] == 7) {
        h->c.state[0] = 0;
        Hewie_Start(h, 0x5A);
        return;
    }
    if (Hewie_CharHere(h, gCharPursuer)) {
        HW(h, 0xF3558, u8) = 1;
        Hewie_SetBehaviour(h, &D_003B17F8);
    } else {
        Hewie_ToDefault(h);
    }
}

/* Idle: on a plain triangle pick a random idle action 0x18..0x1C, not the last one (+0xF357C). */
void func_00140B00(Hewie *h) {
    static const s32 idles[5] = { 0x18, 0x19, 0x1C, 0x1A, 0x1B };
    NavTri *t = NavMesh_Tri(D_0044E570, h->c.a.navTri);
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
void func_0013B860(Hewie *h, Character *other) {
    s16 *v = Hewie_Feeling(h, other->unk153C);

    if (v != NULL) {
        v[4] += 1;
        if (v[4] > 100) {
            v[4] = 100;
        }
    }
}

extern f32 func_0013F220(Hewie *h, Character *c);   /* path distance to c (< 0: none) */

/* Fiona is here, he is free to take a command (not in mode 3) and she is within 150 by path. */
s32 func_001667C0(Hewie *h) {
    f32 d;

    if (!(Hewie_CharHere(h, gCharPlayer) == 1 && HEWIE_MODE(h) != 3 && Hewie_FreeForCommand(h))) {
        return 0;
    }
    d = func_0013F220(h, gCharPlayer);
    if (d < 0.0f) {
        return 0;
    }
    return d <= 150.0f;
}

/* Behaviour: run (move kind 8) until the timer +0xF36B4 runs out or something is within 20
 * ahead; then slow down (kind 7) and once stopped, the default or a stop action (0xC / 0xE). */
void func_001517C0(Hewie *h) {
    u8 stopped;

    if (HW(h, 0xF36B8, s32) == 0) {
        HW(h, 0xF36B4, s32) -= 1;
        if (HW(h, 0xF36B4, s32) == 0
            || func_00123A70(&h->c.a, h->c.a.navTri, h->c.a.pos, -1, h->c.a.angle[1], 20.0f) < 20.0f) {
            HW(h, 0xF36B8, s32) = 1;
        }
        func_00141C00(h, 8);
        return;
    }
    func_00141C00(h, 7);
    stopped = 1;
    if (!(MOTION_SPEED(h->c.motion) <= 0.0f)) {
        stopped = 0;
    }
    if ((stopped ^ 1) & 0xFF) {
        return;
    }
    if (HW(h, 0xF3598, s32) != 0) {
        Hewie_ToDefault(h);
    } else if (!(func_00177620(gProgress) & 0xFF)) {
        Hewie_Start(h, 0xC);
    } else {
        Hewie_Start(h, 0xE);
    }
}

extern const PTMF D_003B1988;

/* Behaviour: run to Fiona if she's here (bark 0x60, then func_001517C0's run). */
void func_00151B10(Hewie *h) {
    if (!Hewie_CharHere(h, gCharPlayer)) {
        Hewie_ToDefault(h);
        return;
    }
    if (func_00140CD0(h, 3)) {
        return;
    }
    func_00122C20(&h->c.a, 0x60, 5, 0, 0, 0);
    if (HW(h, 0xF3604, s32) != 0) {
        HW(h, 0xF3604, s32) = 0;
        HW(h, 0xF3608, s32) = 10;
    }
    HEWIE_TARGET(h) = gCharPlayer;
    func_00141C00(h, 3);
    HW(h, 0xF36B4, s32) = 30;
    Hewie_SetBehaviour(h, &D_003B1988);
}

extern void func_002DD110(void *motion, const f32 *target, f32 *a, f32 *turn);
extern const PTMF D_003B1AB8, D_003B1AC8, D_003B1AD8;

/* Behaviour: follow the moving character +0xF368C (gone: D_003B1AB8; elsewhere on the mesh:
 * D_003B1AD8; slower than 0.5: D_003B1AC8); else steer towards it (+0xF3614 / +0xF3618). */
void func_0014EC10(Hewie *h) {
    Character *c = HW(h, 0xF368C, Character *);
    f32 v[4] __attribute__((aligned(16)));
    f32 a, turn, yaw;
    u32 tri;

    if (c == NULL || !c->a.active) {
        HW(h, 0xF36BC, s32) = 90;
        Hewie_SetBehaviour(h, &D_003B1AB8);
        return;
    }
    tri = c->a.navTri;
    if (func_00124480(&h->c.a, c->a.pos, 0) != tri) {
        HW(h, 0xF36BC, s32) = 90;
        Hewie_SetBehaviour(h, &D_003B1AD8);
        return;
    }
    sceVu0CopyVector(v, HW(h, 0xF368C, Character *)->a.unkB0);
    if (__builtin_sqrtf(sceVu0InnerProduct(v, v)) < 0.5f) {
        HW(h, 0xF36BC, s32) = 90;
        Hewie_SetBehaviour(h, &D_003B1AC8);
        return;
    }
    if (HW(h, 0xF3604, s32) != 8) {
        HW(h, 0xF3604, s32) = 8;
        HW(h, 0xF3608, s32) = 10;
    }
    func_002DD110(h->c.motion, HW(h, 0xF368C, Character *)->a.pos, &a, &turn);
    yaw = func_002E2D00(h->c.a.angle[1] + turn);
    HW(h, 0xF3614, f32) = a;
    HW(h, 0xF3618, f32) = func_002E2D00(yaw - h->c.a.angle[1]);
}

/* Behaviour: (Fiona here and not busy) play the animation to its end, then default. */
void func_00150720(Hewie *h) {
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

extern void func_001F6240(void *motion, f32 *out, f32 t);   /* root motion (variant) */
extern void func_0010E640(f32 *out, const f32 *v, f32 s);   /* libvu0: scale x, y, z */
extern const PTMF D_003B1C40;

/* Behaviour: turn to heading unk10C (10 degrees a frame; against the animation's turn
 * direction +0x858: rotate directly) while moving along +0xF36E0 by the root motion; at the
 * animation's end stand (D_003B1C40). */
void func_0014B8F0(Hewie *h) {
    f32 v[4] __attribute__((aligned(16)));
    f32 root[4] __attribute__((aligned(16)));
    f32 target = *(f32 *)&h->c.unk104[2];
    f32 d = func_002E2D00(target - h->c.a.angle[1]);
    f32 ad, yaw;

    ad = d;
    if (d <= 0.0f) {
        ad = -d;
    }
    if (ad < 0x1.0c1524p-1f /* 30 deg */) {
        func_00124530(&h->c.a, target, 0x1.657186p-3f /* 10 deg */);
    } else if (!(*(f32 *)((u8 *)h->c.motion + 0x858) * d < 0.0f)) {
        func_00124530(&h->c.a, target, 0x1.657186p-3f);
    } else {
        if (d < 0.0f) {
            yaw = func_002E2D00(h->c.a.angle[1] + 0x1.657186p-3f);
        } else {
            yaw = func_002E2D00(h->c.a.angle[1] - 0x1.657186p-3f);
        }
        h->c.a.angle[1] = yaw;
        sceVu0UnitMatrix(h->c.a.rot);
        sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, yaw);
        func_002E2D00(target - h->c.a.angle[1]);
    }
    if (ANIM_DONE(h)) {
        if (HW(h, 0xF36B8, s32) == 0) {
            h->c.a.unk2D = 0;
        }
        func_00143550(h, -1);
        Hewie_SetBehaviour(h, &D_003B1C40);
        VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
    }
    func_001F6240(h->c.motion, root, 0.0f);
    func_0010E640(v, HEWIE_SPOT(h), root[2]);
    func_001247E0(&h->c.a, v);
    HW(h, 0xF3558, u8) = 1;
}

extern void func_002E3190(f32 (*out)[4], f32 angle);
extern void func_002E2DA0(f32 *out, f32 (*m)[4], const f32 *v);
extern const PTMF D_003B1928;

/* Behaviour: move +0xF36C8 along heading +0xF36C4 (+0xF36CC a frame). Done: unless in action
 * 0x6C, D_003B1928; in 0x6C he leaves through exit +0xF36B4 (clearing unk148C), gets up if
 * down, default action. */
void func_00153B00(Hewie *h) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    VObject *rooms;

    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = HW(h, 0xF36C8, f32);
    func_002E3190(m, HW(h, 0xF36C4, f32));
    func_002E2DA0(v, m, v);
    func_001247E0(&h->c.a, v);
    HW(h, 0xF3558, u8) = 1;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
    HW(h, 0xF36C8, f32) = HW(h, 0xF36C8, f32) - HW(h, 0xF36CC, f32);
    if (!(HW(h, 0xF36C8, f32) <= 0.0f)) {
        return;
    }
    if (HEWIE_ACTION(h) != 0x6C) {
        Hewie_SetBehaviour(h, &D_003B1928);
        return;
    }
    h->c.a.unk2B = 0;
    h->c.a.unk2D = 0;
    rooms = D_0044E568;
    if (!(VCALL(rooms, 0x70, u32 (*)(VObject *, s32, s32))(rooms, h->c.a.room, HW(h, 0xF36B4, u8)) & 0xFF)
        && VCALL(rooms, 0x18, s32 (*)(VObject *, s32, s32))(rooms, h->c.a.room, HW(h, 0xF36B4, u8)) != -1
        && func_0013AAE0(h, HW(h, 0xF36B4, u8)) == 0) {
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
void func_0013FA40(Hewie *h) {
    s32 r;

    switch (HW(h, 0xF3648, s32)) {
    case 0:
        if (h->c.a.unkC4 == 1) {
            func_002DDB30(h->c.motion, 0x2002);
            break;
        }
        HW(h, 0xF364C, s32) -= 1;
        if (HW(h, 0xF364C, s32) >= 0) {
            break;
        }
        if (HW(h, 0xF3650, s32) == 0) {
            func_002DDB30(h->c.motion, 0x2000);
            HW(h, 0xF3650, s32) = 1;
        } else {
            func_002DDB30(h->c.motion, 0x2001);
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
            func_002DDB30(h->c.motion, 0x2002);
        } else {
            func_002DDB30(h->c.motion, 0x2000);
        }
        break;
    case 2:
        func_002DDB30(h->c.motion, 0x2001);
        break;
    }
}

/* Roll his reaction: record kind 0 (+0xF3696[n]) and whether a 0..31 roll is below +0xF3690
 * (+0xF369C); then kind 2 vs +0xF3692 (+0xF369E) if it was, else kind 1 vs +0xF3691
 * (+0xF369D). +0xF36A2 counts the records. */
void func_00139840(Hewie *h) {
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
void func_00149370(Hewie *h) {
    if (!Hewie_CharHere(h, gCharPlayer)) {
        Hewie_ToDefault(h);
        return;
    }
    if (gCharPlayer->moveMode != 0xD) {
        Hewie_ToDefault(h);
    } else {
        func_00124530(&h->c.a, *(f32 *)&h->c.unk104[2], 0x1.aceeap-4f /* 6 deg */);
        HW(h, 0xF36B4, s32) -= 1;
        if (HW(h, 0xF36B8, s32) != 0 && HW(h, 0xF36B4, s32) < 16) {
            func_00141C00(h, 1);
        } else {
            HW(h, 0xF36B8, s32) = 1;
            if (MOTION_ANIM(h->c.motion) != 0x1300) {
                func_002DDED0(h->c.motion, 0x1300, -1);
            }
        }
    }
    HW(h, 0xF3558, u8) = 1;
}

extern s32 func_0013EFB0(Hewie *h, f32 *out);

/* Behaviour: steer towards a point (from func_0013EFB0 when he has a target, else the saved
 * point +0xF3630 if +0xF3620), running (kind 8); without one, stand (kind 4). */
void func_0015F0A0(Hewie *h) {
    f32 v[4] __attribute__((aligned(16)));
    f32 a, turn, yaw;
    u8 have;

    if (HW(h, 0xF366C, u8) == 0 && HW(h, 0xF366D, u8) != 0) {
        Hewie_ToDefault(h);
    }
    have = 1;
    if (HEWIE_TARGET(h) == NULL && HW(h, 0xF366D, u8) == 0) {
        have = 0;
    } else if (func_0013EFB0(h, v) == -1) {
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
        func_002DD110(h->c.motion, v, &a, &turn);
        if (HW(h, 0xF3620, u8) == 0) {
            a = 0.0f;
        }
        yaw = func_002E2D00(h->c.a.angle[1] + turn);
        HW(h, 0xF3614, f32) = a;
        HW(h, 0xF3618, f32) = func_002E2D00(yaw - h->c.a.angle[1]);
    } else if (HW(h, 0xF3604, s32) != 4) {
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 10;
    }
    if (!func_00140CD0(h, 0) && MOTION_ANIM(h->c.motion) != 3) {
        func_002DDED0(h->c.motion, 3, -1);
    }
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

extern u32 func_00123D20(Actor *a, const f32 *p);
extern u32 func_00124320(Actor *a, const f32 *target, u32 tri, const f32 *from, u32 mask);
extern const f32 D_003B12A0[][2];   /* offsets (x, z) from him, by kind */

/* Find a spot at offset `kind` around him, turning 0, +-10, ... +-180 degrees: on the walkable
 * mesh (not flags 0x80001) and reachable from Fiona. Writes the heading and point; -1: none. */
s32 func_00138890(Hewie *h, s32 kind, f32 *yawOut, f32 *posOut) {
    f32 off[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    NavMesh *nm;
    s32 deg, side;
    f32 ang, yaw;
    u32 tri;

    off[0] = D_003B12A0[kind][0];
    off[1] = 0.0f;
    off[2] = D_003B12A0[kind][1];
    off[3] = 1.0f;
    nm = D_0044E570;
    for (deg = 0; deg <= 180; deg += 10) {
        ang = F_PI * (f32)deg;
        for (side = 0;; side++) {
            if (side != 0) {
                yaw = func_002E2D00(h->c.a.angle[1] + ang / 180.0f);
            } else {
                yaw = func_002E2D00(h->c.a.angle[1] - ang / 180.0f);
            }
            func_002E3130(m, h->c.a.pos, yaw);
            func_002E2DD0(p, m, off);
            tri = func_00123D20(&h->c.a, p);
            if (tri != NAV_NONE) {
                NavTri *t = (tri < nm->numTris && nm->tris != NULL) ? &nm->tris[tri] : NULL;

                if (!(t->flags & 0x80001)
                    && func_00124320(&h->c.a, p, gCharPlayer->a.navTri, gCharPlayer->a.pos, 0x280A0019) == tri) {
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

extern const PTMF D_003B18A8, D_003B18B8;

/* Behaviour: jump arc. Move by +0xF36E0, rise by +0xF36C8 (less 0.5 a frame); on landing the
 * landing animation (D_003B18A8); at the top (actions 0x1F / 0x20) the falling animation. */
void func_00157770(Hewie *h) {
    f32 y;

    func_001247E0(&h->c.a, HEWIE_SPOT(h));
    y = HW(h, 0xF36C4, f32) + HW(h, 0xF36C8, f32);
    HW(h, 0xF3558, u8) = 1;
    if (y < h->c.a.pos[1]) {
        if (HW(h, 0xF36B4, s32) == 0) {
            func_002DDED0(h->c.motion, 0x2212, -1);
        } else {
            func_002DDED0(h->c.motion, 0x2213, -1);
        }
        Hewie_SetBehaviour(h, &D_003B18A8);
    } else {
        h->c.a.pos[1] = y;
        HW(h, 0xF36C4, f32) = y;
        HW(h, 0xF36C8, f32) = HW(h, 0xF36C8, f32) - 0.5f;
        if (HW(h, 0xF36C8, f32) <= 0.0f) {
            s32 act = HEWIE_ACTION(h);

            if (act == 0x20 || act == 0x1F) {
                switch (h->c.unk104[0]) {
                case 1:
                    func_002DDED0(h->c.motion, HW(h, 0xF36B4, s32) == 0 ? 0x2205 : 0x2208, -1);
                    break;
                case 2:
                    func_002DDED0(h->c.motion, HW(h, 0xF36B4, s32) == 0 ? 0x220E : 0x2211, -1);
                    break;
                }
            }
            Hewie_SetBehaviour(h, &D_003B18B8);
        }
    }
    HW(h, 0xF3582, u8) = 0;
    VCALL(h->c.motion, 0x54, void (*)(void *))(h->c.motion);
}

/* ---- Hewie under the player's control (gProgress +0x1FBEC1): his movement input, as
 * Fiona's (fiona.c func_00187650) ---- */

extern VObject *D_0044E4B8;   /* the camera */
extern void func_002E3190(sceVu0FMATRIX m, f32 yaw);
extern void func_002E2DA0(f32 *out, sceVu0FMATRIX m, const f32 *v);
extern void func_0010E640(f32 *out, const f32 *v, f32 s);   /* libvu0: scale x, y, z */
extern f32 func_0031C5C0(f32 x, f32 z);   /* heading of (x, z) */

#define HMOVE_DIR    0xF3700   /* vec: where to move (world, unit or 0) */
#define HMOVE_MODE   0xF3710   /* u8: 0 free, 1 camera-locked, 2 held, 3 reset */
#define HMOVE_LOCK   0xF3712   /* s16: frames the old camera still steers */
#define HMOVE_STILL  0xF3718   /* s32: frames without input (to 6) */
#define HMOVE_STICK  0xF3720   /* vec: last frame's raw input */
#define HMOVE_LAST   0xF3730   /* vec: last frame's normalized input */
#define HMOVE_CAMYAW 0xF3740   /* f32: the camera heading the controls use */
#define HMOVE_GO     0xF3744   /* u8: the action button (0x4000) */

static f32 hwrap_abs(f32 a) {
    if (!(func_002E2D00(a) <= 0.0f)) {
        return func_002E2D00(a);
    }
    return -func_002E2D00(a);
}

/* the left stick (or the d-pad), camera relative: after a camera cut the old camera keeps
   steering while the stick is held (mode 1, then 2 while the direction holds within 15
   degrees) */
void func_00136900(Hewie *h) {
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
    sceVu0CopyVector(e, D_0047E3A0);
    e[0] += (f32)(s32)(((D_0047E374 >> 5) & 1) - ((D_0047E374 >> 7) & 1));
    e[2] += (f32)(s32)(((D_0047E374 >> 6) & 1) - ((D_0047E374 >> 4) & 1));
    sceVu0Normalize(n, e);
    cut = HW(h, HMOVE_MODE, u8) == 3;
    if (!cut && VCALL(D_0044E4B8, 0x94, s32 (*)(VObject *))(D_0044E4B8) != -1) {
        s32 prev = VCALL(D_0044E4B8, 0x90, s32 (*)(VObject *))(D_0044E4B8);

        cut = prev != VCALL(D_0044E4B8, 0x94, s32 (*)(VObject *))(D_0044E4B8);
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
            func_002E3190(rot, HW(h, HMOVE_CAMYAW, f32));
        } else {
            f32 d[4] __attribute__((aligned(16)));

            sceVu0SubVector(d, e, &HW(h, HMOVE_STICK, f32));
            if (__builtin_sqrtf(sceVu0InnerProduct(d, d)) < k001.f) {
                HW(h, HMOVE_MODE, u8) = 2;
                HW(h, 0xF379C, u32) = 0x3C0EFA35;   /* 0.5 degrees */
                func_002E3190(rot, VCALL(D_0044E4B8, 0x68, f32 (*)(VObject *))(D_0044E4B8));
            }
        }
        func_002E2DA0(v, rot, n);
        func_0010E640(v, v, -1.0f);
        HW(h, 0xF37A0, f32) = func_0031C5C0(v[0], v[2]);
        break;
    case 2:
        if (moving) {
            f32 a = func_0031C5C0(HW(h, HMOVE_LAST, f32), HW(h, HMOVE_LAST + 8, f32));

            how = 0;
            if (!(hwrap_abs(func_0031C5C0(n[0], n[2]) - a) <= k15deg.f)) {
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
        func_002E3190(rot, HW(h, HMOVE_CAMYAW, f32));
        func_002E2DA0(v, rot, n);
        func_0010E640(&HW(h, HMOVE_DIR, f32), v, -1.0f);
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
        func_002E3190(rot, VCALL(D_0044E4B8, 0x68, f32 (*)(VObject *))(D_0044E4B8));
        func_002E2DA0(v, rot, n);
        func_0010E640(&HW(h, HMOVE_DIR, f32), v, -1.0f);
        break;
    }
    sceVu0CopyVector(&HW(h, HMOVE_STICK, f32), e);
    if (HW(h, HMOVE_MODE, u8) == 0) {
        HW(h, HMOVE_CAMYAW, f32) = VCALL(D_0044E4B8, 0x68, f32 (*)(VObject *))(D_0044E4B8);
    }
    if (HW(h, HMOVE_MODE, u8) != 2) {
        sceVu0CopyVector(&HW(h, HMOVE_LAST, f32), n);
    }
    if (D_0047E374 & 0x4000) {
        HW(h, HMOVE_GO, u8) = 1;
    }
}

/* ---- Fiona's commands ---- */

extern s32 func_0013E2D0(Hewie *h, s32 cmd);   /* the action for a command (-1 none, -2..-5 special) */
extern s32 func_0013D580(Hewie *h, s32 act);
extern s32 func_00122C90(void *self, u32 triA, u32 triB, const f32 *posA, const f32 *posB, u32 mask);
extern u8 *func_00139460(Hewie *h);

/* action `act` with argument `arg` unless his situation turns it into another (then that one,
   argument 0) */
static inline void hewie_want(Hewie *h, s32 act, s32 arg) {
    s32 a = func_0013B2C0(h, act);

    func_00130AF0(h, a, a != act ? 0 : arg);
}

/* how long he keeps obeying (+0xF359C) by his trust level, the harder table on difficulty 1 */
static inline void obey_time(Hewie *h) {
    if ((Progress_GetVar(gProgress, 0x27) & 0xFF) != 1) {
        HW(h, 0xF359C, s32) = D_003B1350[HW(h, 0xF35CC, s16)];
    } else {
        HW(h, 0xF359C, s32) = D_003B1370[HW(h, 0xF35CC, s16)];
    }
}

/* a command from Fiona (her +0x14EC, kept at +0xF3578): 1 if he acts on it. Hidden, only the
 * plain ones. Ending a wait (action 0x7D) other than by 0x30 resets his obedience; 0x23 can
 * make him find something to do near her (func_00139460: action 0x1D); 0x29 / 0x2F first try
 * func_001391E0 (actions 0x1D / 0x71); otherwise the command's action (func_0013E2D0): -2
 * action 0x1E, -3 0x6F (back to the current one after), -4 / -5 0x1D / 0x71 with his mood
 * set, else func_0013D580 */
s32 func_00137020(Hewie *h) {
    s32 cmd, act;

    HW(h, 0xF3578, s32) = h->c.state[1];
    if (h->c.a.disabled) {
        act = func_0013E2D0(h, HW(h, 0xF3578, s32));
        if (act == -5 || act == -4 || act == -3 || act == -2 || act == -1) {
            return 0;
        }
        HW(h, 0xF3574, s32) = act;
        func_0013D580(h, act);
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
        !(u8)func_00177620(gProgress) && HW(h, 0xF368C, s32) == 0 &&
        (u8)func_00122C90(h, h->c.a.navTri, gCharPlayer->a.navTri, h->c.a.pos, gCharPlayer->a.pos, 0) == 1) {
        HW(h, 0xF368C, u8 *) = func_00139460(h);
        if (HW(h, 0xF368C, s32) != 0) {
            hewie_want(h, 0x1D, 0x78);
            h->c.state[0] = 0;
            return 1;
        }
    }
    if (HW(h, 0xF3578, s32) == 0x29) {
        HW(h, 0xF36A8, s32) = HEWIE_ACTION(h);
        if ((u8)func_001391E0(h, 1, 3) == 1) {
            HW(h, 0xF35DC, s32) = 0x3C;
            hewie_want(h, 0x1D, 0);
            h->c.state[0] = 0;
            return 1;
        }
    }
    if (HW(h, 0xF3578, s32) == 0x2F) {
        HW(h, 0xF36A8, s32) = HEWIE_ACTION(h);
        if ((u8)func_001391E0(h, 0, 3) == 1) {
            HW(h, 0xF35DC, s32) = 0x3C;
            hewie_want(h, 0x71, 0);
            h->c.state[0] = 0;
            return 1;
        }
    }
    act = func_0013E2D0(h, HW(h, 0xF3578, s32));
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
        func_00138AD0(h, 1, -1);
        hewie_want(h, 0x1D, 0);
        return 1;
    case -5:
        func_00138AD0(h, 0, -1);
        HW(h, 0xF35C4, s32) = 0;
        HW(h, 0xF35C8, s32) = 0;
        hewie_want(h, 0x71, 0);
        return 1;
    }
    HW(h, 0xF3574, s32) = act;
    return func_0013D580(h, act);
}

/* ---- whom to go for ---- */

extern Character **D_0044F258;   /* the creatures (10 slots) */


/* character c active, not down, in his room (in the room being played only on the mesh) */
static s32 in_his_room(Hewie *h, Character *c) {
    if (c == NULL || c->a.active != 1 || c->a.unkC4 == 2) {
        return 0;
    }
    return h->c.a.room == c->a.room &&
           (h->c.a.room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) || c->a.navTri != (u32)-1);
}

/* the one he goes for: the pursuer when it holds Fiona (her mode 4, sub 9) and he can reach
 * it; a creature (slots 7..9, mode 8) holding her (sub 0x12) he can reach; else the pursuer
 * if he can reach it (not while it moves 3); else the nearest hostile creature (+0x3C) he can
 * reach, not down or holding; NULL */
Character *func_001379C0(Hewie *h) {
    Character *best = NULL;
    f32 bestd = 0.0f;
    s32 chase = 0, i;

    if (in_his_room(h, gCharPursuer) && gCharPursuer->moveMode != 3 &&
        (u8)func_0013C1E0(h, gCharPursuer->a.navTri, gCharPursuer->a.pos) == 1) {
        chase = 1;
    }
    if (chase && in_his_room(h, gCharPlayer) && gCharPlayer->moveMode == 4 && gCharPlayer->moveSub == 9) {
        return gCharPursuer;
    }
    if (in_his_room(h, gCharPlayer) && gCharPlayer->moveMode == 4 && gCharPlayer->moveSub == 0x12) {
        for (i = 7; i < 10; i++) {
            Character *c = D_0044F258[i];

            if (in_his_room(h, c) && c->moveMode == 8 && (u8)func_0013C1E0(h, c->a.navTri, c->a.pos) == 1) {
                return c;
            }
        }
    }
    if (chase) {
        return gCharPursuer;
    }
    for (i = 0; i < 10; i++) {
        Character *c = D_0044F258[i];
        f32 d;

        if (!in_his_room(h, c) || (u8)VCALL(&c->a, 0x3C, s32 (*)(void *, u32))(c, i & 0xFF) != 1 || c->a.unkC4 == 2 ||
            c->moveMode == 4) {
            continue;
        }
        d = func_00124490(&h->c.a, c->a.pos);
        if ((best == NULL || d < bestd) && (u8)func_0013C1E0(h, c->a.navTri, c->a.pos) == 1) {
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
u32 func_00137FE0(Hewie *h, u32 done, s32 damage, s32 bone, f32 margin) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u32 hit = 0;
    s32 i;

    if (bone != -1) {
        sceVu0CopyVector(at, func_0017CE80(AT(h->c.motion, 0x810, void *), bone) + 12);
    }
    for (i = 0; i < 10; i++) {
        Character *c = D_0044F258[i];

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

extern VObject *D_0044E550;   /* random numbers: +0x1C 0..1 */

/* whether he dares go for the character in `slot`: from trust level 2, a 1-in-16 roll under
   his level + 1 + his feeling for its kind (+0xF367C.. / 5, at most 8) */
u32 func_00138460(Hewie *h, s32 slot) {
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
    return (s32)(16.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550)) < n + HW(h, 0xF35CC, s16) + 1;
}

/* praised (`praise` 1) or scolded for what he just did (+0xF36A4, when it is still +0xF36A8):
   after action 0x78 his skills involved (+0xF36A2 of them, indices +0xF3696) move by `by`
   (0..31) - up for good ones (+0xF369C), down for bad ones; scolding the other way. 1 if it
   counted */
s32 func_001391E0(Hewie *h, s32 praise, s8 by) {
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

extern VObject *D_0044F260;   /* the placed things (+0xC: entry i of 128) */
#define F_PI_2 0x1.921fb6p+0f   /* 0x3FC90FDB */

/* Fiona's angle to a point, |wrapped| (from her heading) */
static f32 off_her_heading(const f32 *pos) {
    return hwrap_abs(func_001244D0(&gCharPlayer->a, pos) - gCharPlayer->a.angle[1]);
}

/* the nearest placed thing of kind 0 (+0x20) in front of Fiona (within 90 degrees) that he can
   reach (the triangle at it is its own, +0x34); NULL */
u8 *func_00139460(Hewie *h) {
    u8 *best = NULL;
    f32 bestd = 0.0f;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        u8 *t = VCALL(D_0044F260, 0xC, u8 *(*)(VObject *, s32))(D_0044F260, i);

        if (t == NULL || AT(t, 0x20, s32) != 0) {
            continue;
        }
        if (best != NULL) {
            f32 d = func_00124490(&gCharPlayer->a, (f32 *)(t + 0x10));

            if (d < bestd && off_her_heading((f32 *)(t + 0x10)) < F_PI_2 &&
                func_00124480(&h->c.a, (f32 *)(t + 0x10), -1) == AT(t, 0x34, u32)) {
                best = t;
                bestd = d;
            }
        } else if (off_her_heading((f32 *)(t + 0x10)) < F_PI_2 &&
                   func_00124480(&h->c.a, (f32 *)(t + 0x10), -1) == AT(t, 0x34, u32)) {
            best = t;
            bestd = func_00124490(&gCharPlayer->a, (f32 *)(t + 0x10));
        }
    }
    return best;
}

/* the side to go at the pursuer from: 0 head on (they face each other within 90 degrees,
   not with progress flag 0x11); else 1 / 2 by its kind and whether he is on its left */
s32 func_00139A70(Hewie *h) {
    Character *pu = gCharPursuer;
    f32 a = func_001244D0(&h->c.a, pu->a.pos);   /* (from him) */
    f32 its = hwrap_abs(a - pu->a.angle[1]);
    u8 kind;

    if (!(u8)Progress_TestFlag(gProgress, 0x11) && its < F_PI_2 && hwrap_abs(a - h->c.a.angle[1]) < F_PI_2) {
        return 0;
    }
    kind = gCharPursuer->unk153C;
    if (func_002E2D00(func_002E2D00(F_PI + a) - gCharPursuer->a.angle[1]) < 0.0f) {
        return kind == 3 || kind == 34 || kind == 35 || kind == 36 ? 1 : 2;
    }
    return kind == 3 || kind == 34 || kind == 35 || kind == 36 || kind == 11 ? 2 : 1;
}

extern s32 func_001273D0(Character *c, u32 *triOut, f32 *posOut, f32 step);   /* along the path (its next point) */

extern void func_001247E0(Actor *a, const f32 *move);

/* the animation's stride this frame (its root motion's z by the model's speed (+0x48)), at
   least 0 */
static f32 stride(Hewie *h, f32 *v) {
    func_001F6370(h->c.motion, v, 0.0f);
    v[2] *= VCALL(h->c.motion, 0x48, f32 (*)(void *, Hewie *, f32, f32))(h->c.motion, h, 5.0f, -5.0f);
    return v[2];
}

/* one stride along his path: he turns towards a point 12 strides ahead (by the stride x 6 x
 * the slope +0x858, degrees; over 30 degrees off within the turn), the motion's blend +0xF3604
 * 8 frames; facing within ~41 degrees the stride's point he steps there (+0x128 its index;
 * 1), else he takes the stride straight ahead (the path back to +0x128; 0) */
s32 func_00139DE0(Hewie *h) {
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
    func_001273D0(&h->c, &tri, ahead, 12.0f * s);
    yaw = func_001244D0(&h->c.a, ahead);
    next = func_001273D0(&h->c, &tri, at, s);
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
    off = func_002E2D00(yaw - h->c.a.angle[1]);
    if (!((off <= 0.0f ? -off : off) < k30deg.f) && AT(h->c.motion, 0x858, f32) * off < 0.0f) {
        /* turning against the slope: straight round by the rate */
        h->c.a.angle[1] = func_002E2D00(off < 0.0f ? h->c.a.angle[1] + rate : h->c.a.angle[1] - rate);
        sceVu0UnitMatrix(h->c.a.rot);
        sceVu0RotMatrixY(h->c.a.rot, h->c.a.rot, h->c.a.angle[1]);
        func_002E2D00(yaw - h->c.a.angle[1]);
    } else {
        func_00124530(&h->c.a, yaw, rate);
    }
    HW(h, 0xF3604, s32) = 8;
    HW(h, 0xF3608, s32) = 0;
    HW(h, 0xF3614, s32) = 0;
    HW(h, 0xF3618, f32) = func_002E2D00(yaw - h->c.a.angle[1]);
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
    func_001247E0(&h->c.a, v3);
    HW(h, 0xF3558, u8) = 1;
    h->c.unk124 = h->c.unk128;
    return 0;
}

#include "effectmgr.h"   /* HitEffect_Spawn */

/* a bite's hit effect at his mouth (bone 0x25): a big and a small one when `hard`, else small */
void func_0013A1C0(Hewie *h, s32 hard) {
    HitEffectParams hp;

    sceVu0CopyVector(hp.pos, func_0017CE80(AT(h->c.motion, 0x810, void *), 0x25) + 12);
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

extern s32 func_001F4770(void *model, s32 a, s32 b, s32 c);   /* the animation's event bits this frame (u8) */
extern void func_00125E10(Character *c, const f32 *pos, s32 n);

/* his sounds by his animation (+0x55C): the yelps and whimpers (4 0x6F, 9 0x70 when +0xF3585,
 * 5 0x58, 6 / 7 / 0x206 0x59) at once; the others on the animation's sound event (bit 0):
 * barks and growls (barking during actions 0xA / 0xB / 0x7B also makes a noise others hear),
 * a howl (0x1000) heard far; on bit 4, his splash (0x1001) in the room's water (triangle flags
 * 0x02008000) - and in rooms 7 / 0x106 the ripple */
void func_0013A650(Hewie *h) {
    s32 anim = AT(h->c.motion, 0x55C, s32);
    s32 loud = 0;   /* (the original leaves the caller's register for the other actions) */

    switch (anim) {
    case 4:
        func_0013A430(h, 0x6F);
        return;
    case 9:
        if (HW(h, 0xF3585, u8) == 1) {
            func_0013A430(h, 0x70);
            return;
        }
        break;
    case 5:
        func_0013A430(h, 0x58);
        return;
    case 6: case 7: case 0x206:
        func_0013A430(h, 0x59);
        return;
    }
    if ((u8)func_001F4770(h->c.motion, 0, 0, 1) & 1) {
        switch (anim) {
        case 0x1C01:
            func_0013A430(h, 0x60);
            break;
        case 0x1B00: case 0x1B01: case 0x1B02:
            func_0013A430(h, 0x5A);
            break;
        case 0x1B03:
            func_0013A430(h, HEWIE_ACTION(h) == 0x1D ? 0x5A : 0x5D);
            break;
        case 0x1B04:
            func_0013A430(h, HEWIE_ACTION(h) == 0x1D ? 0x5A : 0x5E);
            break;
        case 0x1B05:
            func_0013A430(h, 0x5F);
            break;
        case 0x1C04:
            func_0013A430(h, 0x5C);
            break;
        case 0x1001: case 0x220C: case 0x2203:
            func_0013A430(h, 0x66);
            break;
        }
        switch (anim) {
        case 0x1B00: case 0x1B01: case 0x1B02: case 0x1B03: case 0x1B04: case 0x1B05:
            if (HEWIE_ACTION(h) == 0x7B || HEWIE_ACTION(h) == 0xB || HEWIE_ACTION(h) == 0xA) {
                loud = 0x1B;
            }
            func_002A8440((u8 *)gProgress + 0x788, loud, h->c.a.room, h->c.a.navTri, 0xFFFF);
            break;
        case 0x1000:
            func_002A8440((u8 *)gProgress + 0x788, 0x80, h->c.a.room, h->c.a.navTri, 0xFFFF);
            break;
        }
    }
    if (((u8)func_001F4770(h->c.motion, 0, 0, 1) & 0x10) && anim == 0x1001 &&
        (u8)VCALL(gProgress, 0x50, s32 (*)(Progress *))(gProgress) == 1 &&
        h->c.a.room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) &&
        (NavMesh_Tri(D_0044E570, h->c.a.navTri)->flags & 0x02008000) == 0x02008000) {
        func_00122C20(&h->c.a, 0x1E, 6, 0, 0, NULL);
        if (h->c.a.room == 7 || h->c.a.room == 0x106) {
            func_00125E10(&h->c, h->c.a.pos, 1);
        }
    }
}

extern VObject *D_0044E558;   /* the doors */
extern const s16 D_003B127E[];   /* by how often Fiona hit him (+0xF35C4): the 1-in-16 he sulks */

extern u32 func_00177BF0(Progress *p, u32 door, u32 slot);   /* u8 flags */
extern s32 func_001785B0(Progress *p, s32 room, u32 exit);
extern s32 func_00178300(Progress *p, s32 room, u32 exit, u32 side);

/* a blow `hit` (a character's state block: +0x4 how, +0x8 by whom, +0xC damage, +0x10 the
 * door): down (+0xC4 2) he only goes limp (action 0x74) unless it is 6; held (+0xF8 4) only
 * 0xB counts; nor while he is 8 / 0x18..0x19; else the damage (vtable +0x94, double from
 * Fiona), a door's only when it moves (+0x40). Then by `how`: 1..4 he yelps (action 0x69) -
 * from Fiona (by 0) he learns from it and may sulk (action 3 mode), from another sometimes
 * holds a grudge; 3 also knocks him down - 5 a door: pushed aside (0x6B) or back (0x6C) by
 * which side of it he is; 6 / 0xB: -1 (not taken) */
s32 func_0013AC20(Hewie *h, s32 *hit) {
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
        if (!(u8)VCALL(D_0044E558, 0x40, s32 (*)(VObject *, u8))(D_0044E558, ((u8 *)hit)[0x10])) {
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
                if (VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) < 0.25f) {
                    func_00166150(h, (Character *)gCharacters[h->c.unk100], -1);
                }
                HW(h, 0xF35C4, s32) = 0;
            } else {
                HW(h, 0xF36A8, s32) = HEWIE_ACTION(h);
                func_001391E0(h, 0, 3);
                HW(h, 0xF35C4, s32)++;
                if (HW(h, 0xF35C4, s32) >= 7) {
                    HW(h, 0xF35C4, s32) = 6;
                }
                HW(h, 0xF35C8, s32) = 300;
                if ((s16)(s32)(16.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550)) <
                    D_003B127E[HW(h, 0xF35C4, s32)]) {
                    HW(h, 0xF36B0, s32) = 0xFF;
                    func_00138AD0(h, 3, -1);
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
        yaw = VCALL(D_0044E558, 0x3C, f32 (*)(VObject *, u8))(D_0044E558, HW(h, 0xF36B4, u8));
        if (!((u8)func_00177BF0(p, HW(h, 0xF36B4, u8), (u8)h->c.unk100) & 0x10)) {
            AT(h, 0x2B, u8) = 1;
            hewie_want(h, 0x6B, 0);
        } else if (((u8)func_00177BF0(p, HW(h, 0xF36B4, u8), AT(h, 0x20, u8)) & 8) &&
                   (u8)func_001785B0(p, h->c.a.room, HW(h, 0xF36B4, u8)) != 1 &&
                   (u8)func_00178300(p, h->c.a.room, HW(h, 0xF36B4, u8), AT(h, 0x20, u8))) {
            yaw += F_PI;
            hewie_want(h, 0x6C, 0);
        } else if (!(NavMesh_Tri(D_0044E570, h->c.a.navTri)->flags & 0x20000)) {
            yaw += F_PI_2;
            hewie_want(h, 0x6B, 0);
        }
        HW(h, 0xF36C4, f32) = func_002E2D00(yaw);
        return 0;
    case 6: case 0xB:
        return -1;
    }
    return 0;
}
