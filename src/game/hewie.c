/* Hewie: the partner dog (vtable 0x46A120). */
#include "common.h"
#include "hewie.h"
#include "progress.h"
#include "navmesh.h"
#include "sce/libvu0.h"

extern VObject *gBootMessage;      /* message display, also used in game */
extern Progress *gProgress;

#define MOTION_U8(m, off) (*((u8 *)(m) + (off)))

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

#define HEWIE_HP(h) HW(h, 0x14C8, s32)

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
