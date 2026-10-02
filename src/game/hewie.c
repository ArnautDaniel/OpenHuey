/* Hewie: the partner dog (vtable 0x46A120). */
#include "common.h"
#include "hewie.h"
#include "progress.h"
#include "navmesh.h"
#include "sce/libvu0.h"

extern VObject *gBootMessage;      /* message display, also used in game */
extern Progress *gProgress;

#define MOTION_U8(m, off) (*((u8 *)(m) + (off)))
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
extern s32 func_0013B2C0(Hewie *h, s32 arg);

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

#define HEWIE_ACTION(h) HW(h, 0xF3564, s32)
#define MOTION_ANIM(m) (*(s32 *)((u8 *)(m) + 0x55C))
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

extern void func_00138AD0(Hewie *h, s32 a, s32 b);

/* vtable +0x78: left the room being played during action 0x38 -> default action. */
void func_0015FB30(Hewie *h) {
    s32 room = h->c.a.room;

    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && HEWIE_ACTION(h) == 0x38) {
        Hewie_ToDefault(h);
    }
    func_00138AD0(h, 0, -1);
}

#define HEWIE_HP(h) ((h)->c.hp)

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

#define HEWIE_SIDE(h) HW(h, 0xF3668, s32)   /* side of the room (rooms +0x50) */

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
#define HEWIE_MSG(h) HW(h, 0xF3540, void *)     /* his message image */
#define HEWIE_MRK(h) ((u8 *)(h) + 0xF1540)       /* his .MRK data */

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
extern Character *gCharPlayer;
extern Character *gCharPartner;
extern Character *gCharPursuer;

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

extern Character *gCharacters[6];
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

#define HEWIE_NAV_MASK 0x29020008
#define HEWIE_STATE(h) ((PTMF *)((u8 *)(h) + 0xF35D0))   /* current behaviour (pointer to member) */

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
