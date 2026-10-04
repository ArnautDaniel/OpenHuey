/* Pursuer: the stalkers' shared base class, code 0x278490..0x29FF10. See include/pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"

extern Character *gCharPartner;   /* Hewie */
extern VObject *D_0044E560;       /* sound driver */

/* nothing */
void func_00283EE0(Pursuer *p) {
}


/* a character's step (base) */
void func_002992F0(Pursuer *p) {
    func_00125960(&p->c);
}

/* chasing Hewie? */
s32 func_0029A850(Pursuer *p) {
    return p->target == gCharPartner;
}

/* stop the pursuer's sounds (sound driver +0x10) */
void func_0029D3E0(Pursuer *p) {
    VCALL(D_0044E560, 0x10, void (*)(VObject *, s32, s32))(D_0044E560, 0, 0x400002);
}

/* clear a 3-word vector (third argument) */
void func_0029CE40(Pursuer *p, s32 a1, s32 *out) {
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
}


/* vtable +0x58: deactivate (Character part) */
void func_0029E600(Pursuer *p) {
    func_00125D40(&p->c);
}


/* vtable +0x24 */
void func_0029EAA0(Pursuer *p) {
    func_00127650(&p->c);
}

/* vtable +0x10: nothing */
void func_0029EE00(Pursuer *p) {
}

/* vtable +0x1C4 */
void func_0028ED20(Pursuer *p) {
    PU(p, 0x16EE, u8) = 1;
    PU(p, 0x16F0, u8) = 1;
    p->c.unk100 = -1;
    p->c.unk104[0] = -1;
    p->c.a.unk2B = 0;
    p->c.a.unk2A = 0;
}

/* ---- batch 2 ---- */


extern Progress *gProgress;
extern Character *gCharPlayer;    /* Fiona */
extern VObject *gBootMessage;     /* message display */
extern VObject *gFileLoader;
extern VObject *D_0044E550;       /* random numbers */
extern VObject *D_0044E558;       /* doors */
extern VObject *D_0044E568;       /* rooms */
extern void *D_0044E570;          /* nav mesh */


/* nav triangle +0x179C, if valid */
void func_0027E5A0(Pursuer *p, u32 tri) {
    if (tri < AT(D_0044E570, 0x8, u32)) {
        PU(p, 0x179C, u32) = tri;
    }
}

/* clear the step counters */
void func_0029A6D0(Pursuer *p) {
    PU(p, 0x1624, s32) = 0;
    PU(p, 0x1628, s32) = 0;
    PU(p, 0x162C, s32) = 0;
    PU(p, 0x1630, s32) = 0;
    PU(p, 0x1634, s32) = 0;
    PU(p, 0x1638, s32) = 0;
    PU(p, 0x163C, s32) = 0;
    PU(p, 0x1640, s32) = 0;
    PU(p, 0x1650, s32) = 0;
    PU(p, 0x1654, s32) = 0;
    PU(p, 0x1658, s32) = 0;
    PU(p, 0x165C, s32) = 0;
}

/* vtable +0x2C4: timer +0x1660 to 900 frames (15 s), 1 when the progress byte +0x1FBEC1 is set */
void func_0027E560(Pursuer *p) {
    PU(p, 0x1660, s32) = AT(gProgress, 0x1FBEC1, u8) != 0 ? 1 : 900;
}

/* vtable +0x324 / +0x320: animation ids by stance (+0xC4) and +0x16B8 */
s32 func_00297AC0(Pursuer *p) {
    if (p->c.a.unkC4 != 1) {
        return PU(p, 0x16B8, s32) == 2 ? 0x204 : 0x200;
    }
    return 0x202;
}

s32 func_00297B00(Pursuer *p) {
    if (p->c.a.unkC4 != 1) {
        return PU(p, 0x16B8, s32) == 2 ? 3 : 0;
    }
    return 2;
}

/* vtable +0x328 */
s32 func_00297A70(Pursuer *p) {
    if (p->c.a.unkC4 != 1) {
        if (PU(p, 0x16B8, s32) != 2) {
            return PU(p, 0x16C8, u8) != 0 ? 0x201 : 0x206;
        }
        return 0x205;
    }
    return 0x203;
}

/* stop when the animation has run out (+0x550 time left) */
void func_00292120(Pursuer *p) {
    if (p->c.unkE0 != 0) {
        s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

        if (!((over ^ 1) & 0xFF)) {
            p->c.unkE1 = 1;
        }
    }
}

/* vtable +0x240: step, count +0x104 down */
void func_00289810(Pursuer *p) {
    func_00125A10(&p->c);
    p->c.unk104[0]--;
    if (p->c.unk104[0] <= 0) {
        PURSUER_STEP_DONE(p) = 1;
    }
}

/* step until the animation ends */
void func_00289F50(Pursuer *p) {
    func_00125A10(&p->c);
    if (MOTION_KEYS(p) & MOTION_KEY_END) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

/* vtable +0x10C: in stance 2 playing animation 0x1805 / 0x1806 */
s32 func_0029A870(Pursuer *p) {
    if (p->c.a.unkC4 == 2) {
        s32 anim = MOTION_ANIM(p);

        if (anim != 0x1806 && anim != 0x1805) {
            return 0;
        }
        return 1;
    }
    return 0;
}

void func_00291C30(Pursuer *p) {
    if ((MOTION_KEYS(p) & MOTION_KEY_END) != 0) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_00125A10(&p->c);
    }
}

void func_00285AB0(Pursuer *p) {
    if (MOTION_KEYS(p) & 0x400) {
        PU(p, 0x1624, s32)--;
        if (PU(p, 0x1624, s32) <= 0) {
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            p->c.unk104[0] = -1;
            PU(p, 0x1628, s32) = 0;
            PU(p, 0x1624, s32) = 0;
        }
    }
    func_00125A10(&p->c);
}

/* vtable +0x4C: disable, animation paused, its message taken down */
void func_002990E0(Pursuer *p) {
    func_00126910(&p->c);
    MOTION_AT(p, 0x4D8, u8) = 1;
    if (PU(p, 0x1688, s32) != 0) {
        VCALL(gBootMessage, 0x10, void (*)(VObject *, u32, s32, s32))(gBootMessage, p->c.msgSlot, PU(p, 0x1688, s32), 0);
    }
}

/* vtable +0x150 */
void func_002801B0(Pursuer *p) {
    f32 pos[4] __attribute__((aligned(16)));

    if (func_00122B50(&p->c.a, pos) != 0) {
        func_001264C0(&p->c, 0, pos);
        func_001264C0(&p->c, 5, pos);
    }
}

/* vtable +0x50: enable, animation running, its message released */
void func_00299080(Pursuer *p) {
    func_00126810(&p->c);
    MOTION_AT(p, 0x4D8, u8) = 0;
    if (PU(p, 0x1688, s32) != 0) {
        VCALL(gBootMessage, 0x14, void (*)(VObject *, u32))(gBootMessage, p->c.msgSlot);
        PU(p, 0x1688, s32) = 0;
    }
}

/* vtable +0x54: load the message image (file name at (+0x168C)->+0x1C) */
s32 func_0029EE10(Pursuer *p) {
    char *name = AT(PU(p, 0x168C, u8 *), 0x1C, char *);

    if (*name == 0) {
        return 0;
    }
    VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1688, s32), p->c.a.flags24 | p->c.a.slot, 0);
    return 1;
}

extern void *D_0046D800[];
extern void *D_00469D00[];

/* destructor of a small object (vtables 0x46D800 -> 0x469D00) */
void **func_00278490(void **obj, s32 flags) {
    if (obj != NULL) {
        *obj = D_0046D800;
        if (obj != NULL) {
            *obj = D_00469D00;
        }
        if ((s16)flags > 0) {
            func_00100490(obj);
        }
    }
    return obj;
}

/* the entry of a {threshold, value} table (8 bytes each) whose threshold +0x1588 reaches */
s32 func_00297290(Pursuer *p, f32 *table, u32 n) {
    u32 i;

    for (i = 0; i < n; i++) {
        s32 below = PU(p, 0x1588, f32) <= table[i * 2];

        if ((below ^ 1) == 0) {
            return ((s32 *)table)[i * 2 + 1];
        }
    }
    return 0;
}

/* run the state, stopping on reaching the triangle +0x104 */
void func_00299300(Pursuer *p) {
    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    if ((s32)p->c.a.navTri == p->c.unk104[0]) {
        p->c.unkE1 = 1;
    }
}

/* vtable +0x178: count +0x1624 down while the room's flag +0x17B0 isn't set */
void func_0027CE80(Pursuer *p) {
    if (Progress_CurRoomFlag(gProgress, p->c.a.room, PU(p, 0x17B0, u8)) != 0) {
        PU(p, 0x1624, s32) = 0;
    }
    if (PU(p, 0x1624, s32) > 0) {
        PU(p, 0x1624, s32)--;
    } else {
        PURSUER_STEP_DONE(p) = 1;
    }
}

/* may the pursuer go for its target? */
s32 func_00283870(Pursuer *p) {
    if (p->target->a.unk2D == 1) {
        return 0;
    }
    if ((Progress_TestFlag(gProgress, 0xE) & 0xFF) == 1) {
        return 0;
    }
    return gCharPlayer->moveSub != 0x10;
}

/* vtable +0x1FC: through the door +0x100 once it's open */
void func_0028D540(Pursuer *p) {
    if (VCALL(D_0044E558, 0x30, s32 (*)(VObject *, u32))(D_0044E558, (u8)p->c.unk100) != 0) {
        func_00212CA0(p, (u8)p->c.unk100);
        p->c.moveMode = 0;
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        p->c.a.disabled = 0;
        p->c.unk100 = -1;
    }
}

/* vtable +0x1E0: reset to idle */
void func_0028DBD0(Pursuer *p) {
    PU(p, 0x1624, s32) = 0;
    PURSUER_STEP_DONE(p) = 1;
    PURSUER_STEP_NEXT(p) = 1;
    p->c.unk100 = -1;
    p->c.unk104[0] = -1;
    p->c.a.unk2D = 0;
    p->c.a.unk2B = 0;
    p->c.a.unk2A = 0;
    p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathReq->mask = p->c.a.navMask;
    func_002177D0(p);
}

/* vtable +0x180 */
void func_002919B0(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) == 1) {
        func_00125A10(&p->c);
    } else if (p->c.unkE0 != 0) {
        p->c.unkE1 = 1;
    }
}

/* vtable +0x170 */
void func_0027D130(Pursuer *p) {
    if (func_00217510(p) == 0) {
        if (PU(p, 0x17B4, s32) == 0 && PU(p, 0x1664, s32) == 0 && (func_00212A80(p, 0xFF) & 0xFF) != 0xFF) {
            PURSUER_STEP_DONE(p) = 1;
        }
    } else {
        PU(p, 0x16ED, u8) = 1;
    }
}

/* vtable +0x208 */
void func_0028D040(Pursuer *p) {
    PU(p, 0x1634, s32) = 0;
    p->c.a.unk2B = 0;
    if (VCALL(D_0044E568, 0x70, s32 (*)(VObject *, s32, u32))(D_0044E568, p->c.a.room, (u8)p->c.unk100) & 0xFF) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_002815E0(p, (u8)p->c.unk100);
    }
    p->c.unk100 = -1;
}

/* is `room` (-1: the current one) in the vtable +0x314 list ({room, ...} 8 bytes each, -1 ends)? */
s32 func_0029A8C0(Pursuer *p, s32 room) {
    s32 *e;

    if (room == -1) {
        room = p->c.a.room;
    }
    e = VCALL(p, 0x314, s32 *(*)(Pursuer *))(p);
    for (; *e != -1; e += 2) {
        if (room == *e) {
            return 1;
        }
    }
    return 0;
}

/* vtable +0x28: placement, turned to the animation's heading */
s32 func_0029ED80(Pursuer *p) {
    s32 r = func_00125AD0(&p->c);
    u8 *m;

    MOTION_AT(p, 0x854, s32) = 0;
    MOTION_AT(p, 0x858, f32) = 0.0f;
    PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
    m = p->c.motion;
    VCALL(m, 0x54, void (*)(void *))(m);
    MOTION_AT(p, 0x850, u8) = 1;
    return r;
}

/* run the state; then, a step done, the next behaviour (+0x114) */
void func_002994B0(Pursuer *p) {
    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    if (PURSUER_STEP_DONE(p) == 1) {
        PURSUER_STEP_DONE(p) = 0;
        if (PU(p, 0x175C, s32) != 0) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0);
        }
    }
}

/* vtable +0x2C: room light change */
void func_0029ED00(Pursuer *p) {
    u8 *m;

    if (p->c.a.disabled == 1) {
        return;
    }
    if (p->c.unkE4 == 1 && p->c.unk152C != 0x11 && p->c.unk152C != 0x14) {
        VCALL(p, 0x80, void (*)(Pursuer *))(p);
    }
    m = p->c.motion;
    VCALL(m, 0x38, void (*)(void *, s32, u32, s32))(m, p->c.unk152C, p->c.a.navTri, 0);
}

/* vtable +0x78: left behind in another room while in behaviour 0x23 */
void func_00298FF0(Pursuer *p) {
    s32 room = p->c.a.room;

    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && PU(p, 0x175C, s32) == 0x23) {
        PU(p, 0x1664, s32) = 0;
        VCALL(p, 0x2A8, void (*)(Pursuer *))(p);
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
}

/* release the model files (loader +0x18; the sound bank 7 too for slot 2) */
void func_0029F2C0(Pursuer *p) {
    if (p->c.a.slot != 2) {
        VCALL(gFileLoader, 0x18, void (*)(VObject *, u32))(gFileLoader, p->c.a.flags24 | p->c.a.slot);
        return;
    }
    VCALL(D_0044E560, 0x88, void (*)(VObject *, s32))(D_0044E560, 7);
    VCALL(gFileLoader, 0x18, void (*)(VObject *, u32))(gFileLoader, p->c.a.flags24 | p->c.a.slot);
}

/* vtable +0x18 */
void func_0029F350(Pursuer *p) {
    if (p->c.a.slot != 2) {
        VCALL(gFileLoader, 0x14, void (*)(VObject *, u32))(gFileLoader, p->c.a.flags24 | p->c.a.slot);
        return;
    }
    VCALL(D_0044E560, 0x84, void (*)(VObject *, s32))(D_0044E560, 7);
    VCALL(gFileLoader, 0x14, void (*)(VObject *, u32))(gFileLoader, p->c.a.flags24 | p->c.a.slot);
}

extern const PTMF D_003ED0D0, D_003ED050, D_003ED0E0, D_003ED060, D_003ED530, D_003ED5A0, D_003ECF20,
    D_003ED070, D_003ED0F0, D_003ECF80, D_003ECE80, D_003ED590, D_003ED080, D_003ECF30, D_003ECD30,
    D_003ED210, D_003ECF40, D_003ECCB0, D_003ECC70, D_003ECB90;

void func_0028A100(Pursuer *p) {
    func_00178070(gProgress, *(u8 *)&p->c.a.slot, 1, 9, 0, 8, 0.0f);
    Actor_SetState(&p->c.a, &D_003ED0D0);
}

void func_0028AA80(Pursuer *p) {
    func_00178070(gProgress, *(u8 *)&p->c.a.slot, 1, 9, 0, 6, 0.0f);
    Actor_SetState(&p->c.a, &D_003ED050);
}

void func_0028A060(Pursuer *p) {
    if (p->c.state[0] != 7) {
        p->c.state[0] = 0;
        Actor_SetState(&p->c.a, &D_003ED0E0);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

void func_0028A9E0(Pursuer *p) {
    if (p->c.state[0] != 7) {
        p->c.state[0] = 0;
        Actor_SetState(&p->c.a, &D_003ED060);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

/* +0x16E0 plus a random 0..3600 frames */
s32 func_0029CE50(Pursuer *p) {
    f32 r = 3600.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);

    return PU(p, 0x16E0, s32) + (u32)r;
}

/* back on its feet: at least 1 hp, chase state reset */
void func_002860D0(Pursuer *p) {
    if (p->c.hp <= 0) {
        p->c.hp = 1;
    }
    if (gCharPartner->a.unkC4 == 2) {
        func_00297160(p);
    }
    PU(p, 0x1760, u8) = 0;
    p->c.unk104[0] = -1;
    PU(p, 0x1770, s32) = 0;
    PU(p, 0x1774, s32) = 0;
    PU(p, 0x1778, s32) = 0;
    PU(p, 0x1624, s32) = 0;
    PU(p, 0x1628, s32) = 0;
    PU(p, 0x1634, s32) = 0;
    PU(p, 0x1638, s32) = 0;
    PURSUER_STEP_DONE(p) = 1;
    PURSUER_STEP_NEXT(p) = 1;
}

/* vtable +0x20: unload: message, model, sounds */
void func_0029EC60(Pursuer *p) {
    if (p->c.a.unkD0 != 0) {
        VCALL(gBootMessage, 0xC, void (*)(VObject *, u32))(gBootMessage, p->c.msgSlot);
        p->c.a.unkD0 = 0;
    }
    if (p->c.a.unkD1 != 0) {
        u8 *m = p->c.motion;

        VCALL(m, 0x10, void (*)(void *))(m);
        p->c.a.unkD1 = 0;
    }
    VCALL(D_0044E560, 0x10, void (*)(VObject *, s32, s32))(D_0044E560, 0, 0x40F002);
}

/* vtable +0x2A4: behaviour state cleared */
void func_0027DFA0(Pursuer *p) {
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED530);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x2A8, void (*)(Pursuer *))(p);
}

/* wait for animation 0x1903 to end, then hold for (+0x1748)[0] frames (90 without) */
void func_0028A660(Pursuer *p) {
    func_00125A10(&p->c);
    if ((MOTION_KEYS(p) & MOTION_KEY_END) != 0 && MOTION_ANIM(p) == 0x1903) {
        s32 *t = PU(p, 0x1748, s32 *);

        PU(p, 0x1760, u8) = 1;
        PU(p, 0x178C, s32) = t != NULL ? *t : 90;
        PURSUER_STEP_NEXT(p) = 1;
        p->c.unk104[0] = -1;
        PURSUER_STEP_DONE(p) = 1;
    }
}

/* vtable +0x108: sound id of the current path node (move 8 / 0x1A..0x1C), -1 none */
u32 func_0029CB40(Pursuer *p) {
    if (p->c.moveMode == 8) {
        s32 sub = p->c.moveSub;

        if (sub == 0x1A || sub == 0x1B || sub == 0x1C) {
            s32 i = PU(p, 0x1728, s32);

            if (i >= 0) {
                s8 k = PU(p, 0x172C, s8);

                if (k < 4) {
                    s8 n = PU(p, 0x1724, s8 *)[i * 4 + k];

                    return AT(PU(p, 0x171C, u8 *) + n * 0x24, 0x12, u16);
                }
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

/* the positions of bones `bones`[1] and (if >= 0) `bones`[2] */
void func_00283A50(Pursuer *p, s32 *bones, f32 *a, f32 *b) {
    sceVu0CopyVector(a, func_0017CE80(MOTION_AT(p, 0x810, u8 *), bones[1]) + 0xC);
    if (bones[2] >= 0) {
        sceVu0CopyVector(b, func_0017CE80(MOTION_AT(p, 0x810, u8 *), bones[2]) + 0xC);
    }
}

/* ---- batch 3 ---- */

extern VObject *D_0044E4D0;       /* room objects */
extern Character *gCharacters[6];
extern void *D_003EC8E0;


/* the defaults of every pursuer (radius, height, hp 100, hearing, timers) */
void func_0029FB20(Pursuer *p) {
    p->c.a.radius = 5.0f;
    p->c.a.height = 20.0f;
    p->c.hpMax = 100;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x16B4, u8) = 0;
    PU(p, 0x1714, s32) = 0;
    PU(p, 0x16AC, s32) = 0;
    PU(p, 0x16B0, s32) = 0;
    PU(p, 0x171C, s32) = 0;
    PU(p, 0x1720, s32) = 0;
    PU(p, 0x1724, s32) = 0;
    PU(p, 0x1734, s32) = 0;
    PU(p, 0x1730, s32) = 0;
    PU(p, 0x1740, s32) = 0;
    PU(p, 0x173C, s32) = 0;
    PU(p, 0x1744, void *) = &D_003EC8E0;
    PU(p, 0x1748, s32) = 0;
    PU(p, 0x1580, f32) = 150.0f;          /* sight range */
    PU(p, 0x1584, f32) = 0x1.0c15240000000p+0f /* 1.0471976 */;      /* 60 degrees */
    PU(p, 0x16DC, s32) = 100;
    PU(p, 0x16E8, f32) = 10.0f;
    PU(p, 0x16D4, s32) = 300;             /* frames */
    PU(p, 0x16D8, s32) = 1800;
    PU(p, 0x16D0, s32) = 1800;
    PU(p, 0x16E0, s32) = 9000;
    PU(p, 0x16E4, s32) = 150;
}

/* vtable +0x16C */
void func_0027D1B0(Pursuer *p) {
    p->c.moveSub = p->c.unk128 < p->c.unk124 ? 0x16 : 0x17;
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED5A0);
    VCALL(p, 0x170, void (*)(Pursuer *))(p);
}

/* step while facing within 45 degrees of the target */
void func_00289500(Pursuer *p) {
    f32 h = func_001244D0(&p->c.a, p->target->a.pos);
    f32 d = func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));

    if (d <= 0.0f) {
        d = -d;
    }
    if (d < 0x1.921fb60000000p-1f /* 0.7853982 */) {
        func_00125A10(&p->c);
    }
}

/* vtable +0x214 */
void func_0028CD70(Pursuer *p) {
    PU(p, 0x1634, s32) = 0;
    p->c.a.unk2B = 0;
    if (!(VCALL(D_0044E568, 0x70, s32 (*)(VObject *, s32, u32))(D_0044E568, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
        if (p->c.a.unkC4 == 2) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
        }
        func_002815E0(p, (u8)p->c.unk100);
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
    }
    p->c.unk100 = -1;
}

/* vtable +0x1EC: open the door the pursuer heads for, unless it already is */
void func_0028D8A0(Pursuer *p) {
    func_00214620(p, 0);
    if (!(func_00177BF0(gProgress, p->c.door, *(u8 *)&p->c.a.slot) & 0xFF & 1)) {
        p->c.unk100 = p->c.door;
        Actor_SetState(&p->c.a, &D_003ECF20);
        VCALL(p, 0x1AC, void (*)(Pursuer *))(p);
    }
}

/* a sound at the pursuer, unless a room object or progress flag 8 holds them */
void func_0029D410(Pursuer *p, s32 sound, s32 a2, s32 a3, s32 a4, void *a5) {
    if (VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
        func_00122C20(&p->c.a, sound, a2, a3, a4, a5);
    }
}

/* raise the threat level when someone it could reach is in sight */
void func_002837C0(Pursuer *p, u32 mask) {
    Progress *pr = gProgress;

    if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
        u32 seen = func_00217920(p) & 0xFF & (mask & 0xFF);

        if (~PU(p, 0x1760, u8) & seen) {
            func_00178070(pr, *(u8 *)&p->c.a.slot, seen & 0xFF, 4, 5, 0, 10.0f);
        }
    }
}

void func_0028A930(Pursuer *p) {
    if (p->c.state[0] != 7) {
        p->c.state[0] = 0;
        func_002DDD20(p->c.motion, 0x1900, -1);
        p->c.moveSub = 0x19;
        Actor_SetState(&p->c.a, &D_003ED070);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

void func_00289FA0(Pursuer *p) {
    if (p->c.state[0] != 7) {
        func_002DDD20(p->c.motion, 0x1A01, -1);
        p->c.a.unk2D = 1;
        p->c.moveSub = 0x19;
        Actor_SetState(&p->c.a, &D_003ED0F0);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

/* vtable +0x210: walk to the door */
void func_0028CF80(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x1634, f32) = func_00212550(p, (u8)p->c.unk100);
    if (VCALL(D_0044E558, 0x70, s32 (*)(VObject *, u32))(D_0044E558, (u8)p->c.unk100) != 0) {
        p->c.a.unk2B = 1;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECF80);
    VCALL(p, 0x218, void (*)(Pursuer *))(p);
}

/* vtable +0x1BC: the door animation over, through */
void func_0028EFC0(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) != 1) {
        VCALL(D_0044E558, 0xC, void (*)(VObject *, u32, s32, s32, s32))(D_0044E558, (u8)p->c.unk100, p->c.unk104[0], p->c.a.slot, 1);
        Actor_SetState(&p->c.a, &D_003ECE80);
    }
}

/* load the motion files (names at (+0x168C)->+0x0 / +0x8) */
void func_0029EF80(Pursuer *p, s32 id) {
    char *name;

    if (id == -1) {
        id = p->c.a.flags24 | p->c.a.slot;
    }
    name = AT(PU(p, 0x168C, u8 *), 0x0, char *);
    if (*name != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, s32, s32))(gFileLoader, name, PU(p, 0x1670, s32), id, 0);
    }
    name = AT(PU(p, 0x168C, u8 *), 0x8, char *);
    if (*name != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, s32, s32))(gFileLoader, name, PU(p, 0x1674, s32), id, 0);
    }
}

/* vtable +0x160: follow the planned path */
void func_0027D810(Pursuer *p) {
    PURSUER_STEP_NEXT(p) = 1;
    p->c.unk124 = p->c.unk128;
    *(f32 *)&p->c.unk14C4 = (f32)VCALL(D_0044E568, 0x38, s32 (*)(VObject *, u32, s32))(D_0044E568, PU(p, 0x138C, u16), p->c.a.room) + 150.0f;
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED590);
    PU(p, 0x1784, s32) = 0;
}

/* animation 0x1901 after the current one, and the threat level up by vtable +0x300 */
void func_0028A860(Pursuer *p) {
    u8 *m = p->c.motion;

    if (MOTION_KEYS(p) & MOTION_KEY_END) {
        func_002DDE20(m, 0x1901, -1);
        Actor_SetState(&p->c.a, &D_003ED080);
        func_002EFA50((u8 *)gProgress + 0x7B8, VCALL(p, 0x300, f32 (*)(Pursuer *))(p));
        return;
    }
    func_00125A10(&p->c);
}

/* vtable +0x1F4: go to the room exit +0x17B0 */
void func_0028D7D0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    PU(p, 0x15A4, s32) = VCALL(D_0044E568, 0x24, s32 (*)(VObject *, u32))(D_0044E568, PU(p, 0x17B0, u8));
    VCALL(D_0044E570, 0xC, void (*)(void *, s32, f32 *))(D_0044E570, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0));
    VCALL(p, 0xD8, void (*)(Pursuer *))(p);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECF30);
    VCALL(p, 0x198, void (*)(Pursuer *))(p);
}

/* vtable +0x6C: store the pursuer's state in the progress (saved with the game) */
void func_0029E9D0(Pursuer *p) {
    u8 *s = (u8 *)gProgress + 0x83C;

    AT(gProgress, 0x83C, s32) = p->c.a.room;
    AT(gProgress, 0x844, s32) = p->c.a.unkC4;
    AT(gProgress, 0x848, s32) = PU(p, 0x16B8, s32);
    AT(gProgress, 0x850, s32) = p->c.hp;
    AT(gProgress, 0x854, s32) = p->c.hpMax;
    AT(gProgress, 0x858, s32) = PU(p, 0x1594, s32);
    AT(gProgress, 0x85C, s32) = PU(p, 0x15A4, s32);
    AT(gProgress, 0x868, u8) = p->c.door;
    AT(gProgress, 0x860, s32) = PU(p, 0x16BC, s32);
    AT(s, 0x28, u32) = (u32)PU(p, 0x16C0, f32);
    AT(s, 0x2C, u8) = p->c.door;
    AT(s, 0x2D, u8) = PU(p, 0x16C4, s32);
    AT(s, 0x2E, u8) = PU(p, 0x16C8, u8);
    AT(s, 0x30, s32) = PU(p, 0x1660, s32);
    AT(s, 0x34, s32) = PU(p, 0x1664, s32);
    AT(s, 0x4, u32) = p->c.a.navTri;
    AT(s, 0x10, f32) = p->c.a.angle[1];
}

/* the behaviour ended (+0x1758 next, -1 none): reset, vtable +0x2A0 */
void func_002927D0(Pursuer *p) {
    s32 next = PU(p, 0x1758, s32);

    if (next != -1 && next != -2) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, next);
    }
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x178C, s32) = 0;
    p->c.unk14D0 = 0;
    func_001F6E10(p->c.motion);
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECD30);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x2A0, void (*)(Pursuer *))(p);
}

/* vtable +0x5C: reset (room entry) */
void func_0029E520(Pursuer *p) {
    p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    if (p->c.a.slot == 2) {
        func_002DDE20(p->c.motion, 0, -1);
    }
    PU(p, 0x17B0, u8) = 0xFF;
    PU(p, 0x1664, s32) = 0;
    PU(p, 0x1544, u8) = 0;
    PU(p, 0x1545, u8) = 0;
    PU(p, 0x16C8, u8) = 3;
    VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
    PU(p, 0x16C9, u8) = 0;
    PU(p, 0x16CB, u8) = 0;
    PU(p, 0x16CA, u8) = 0;
    PU(p, 0x16CC, u8) = 0;
    PU(p, 0x179C, s32) = -1;
    PU(p, 0x16F1, u8) = 0;
    PU(p, 0x16F3, u8) = 0;
    PU(p, 0x16F2, u8) = 0;
    PU(p, 0x16F4, u8) = 0;
    if (p->c.hp <= 0) {
        p->c.hp = p->c.hpMax;
    }
    VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 0);
    func_00125CC0(&p->c);
}

/* the chase towards the target: by path, or straight at it once on its triangle */
void func_00290810(Pursuer *p) {
    if (PU(p, 0x1590, f32) < 0.0f) {
        VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, p->target);
        if (p->c.a.room == p->target->a.room && !(func_00284440(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
    } else {
        Character *t = p->target;
        u32 tri = t->a.navTri;

        if (func_00124480(&p->c.a, t->a.pos, -1) != tri) {
            s32 n = p->c.unk128;

            if ((n < p->c.unk124) == 1) {
                func_00214620(p, n);
            }
        } else {
            func_002143D0(p, p->target->a.pos);
        }
    }
}

/* vtable +0x60: reset all behaviour state */
void func_0029E440(Pursuer *p) {
    if (func_00217510(p) != 0 && p->c.moveMode == 2) {
        func_00213270(p, 0xFF);
    }
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x16F5, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x16EC, u8) = 0;
    PU(p, 0x1710, u8) = 0;
    PU(p, 0x16F9, u8) = 0;
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x15A0, u8) = 1;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    PU(p, 0x17B4, s32) = 0;
    PU(p, 0x1728, s32) = -1;
    PU(p, 0x172C, u8) = 0;
    PU(p, 0x1738, s32) = 0;
    func_0029A6D0(p);
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    PU(p, 0x1770, s32) = 0;
    PU(p, 0x1774, s32) = 0;
    PU(p, 0x1778, s32) = 0;
    VCALL(p, 0x7C, void (*)(Pursuer *))(p);
    func_00125BE0(&p->c);
}

/* give the model the motion banks loaded by character `slot` (its +0x1670 file) */
void func_0029F040(Pursuer *p, u32 slot) {
    u8 *m = p->c.motion;
    u8 *f = PU(gCharacters[slot & 0xFF], 0x1670, u8 *);

    AT(m, 0x4C0, u8 *) = AT(f, 0x4, s32) != 0 ? f + AT(f, 0x4, s32) : NULL;
    AT(m, 0x4D0, u8 *) = AT(f, 0x8, s32) != 0 ? f + AT(f, 0x8, s32) : NULL;
    AT(m, 0x4CC, u8 *) = AT(f, 0xC, s32) != 0 ? f + AT(f, 0xC, s32) : NULL;
    AT(m, 0x4C4, u8 *) = AT(f, 0x10, s32) != 0 ? f + AT(f, 0x10, s32) : NULL;
    m = p->c.motion;
    VCALL(m, 0xC, void (*)(void *))(m);
    p->c.a.unkD1 = 1;
    MOTION_AT(p, 0x24, u8) = slot;
}

/* turn to +0x1634 by +0x1638 while the animation runs; then face it */
void func_00286AA0(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) != 1) {
        f32 h = PU(p, 0x1634, f32);

        p->c.a.angle[1] = h;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
        PU(p, 0x1634, f32) = 1.0f;
        PU(p, 0x1638, s32) = 0;
        Actor_SetState(&p->c.a, &D_003ED210);
    } else {
        func_00124530(&p->c.a, PU(p, 0x1634, f32), PU(p, 0x1638, f32));
    }
}

/* vtable +0x1F8 .. : through the exit +0x17B0 if it's usable */
void func_0028D6E0(Pursuer *p) {
    if (PU(p, 0x16B8, s32) == 2) {
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (!(VCALL(D_0044E568, 0x78, s32 (*)(VObject *, s32, u32))(D_0044E568, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF)) {
        PU(p, 0x16EF, u8) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    p->c.a.disabled = 1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECF40);
    VCALL(p, 0x1F8, void (*)(Pursuer *))(p);
}

/* behaviour starts: the previous one's follow-up (+0x1758, -1: `dflt`), flags reset,
 * the behaviour's step function (+0x174C) set, then vtable `next` */
#define PURSUER_BEGIN(p, dflt)                                                  \
    do {                                                                        \
        s32 prev_ = PU(p, 0x1758, s32);                                         \
        if (prev_ != -1) {                                                      \
            if (prev_ != -2) {                                                  \
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, prev_);            \
            }                                                                   \
        } else {                                                                \
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, dflt);                 \
        }                                                                       \
    } while (0)

/* vtable +0x288 */
void func_00294150(Pursuer *p) {
    PURSUER_BEGIN(p, 0x14);
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x162C, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECCB0);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x28C, void (*)(Pursuer *))(p);
}

/* vtable +0x278: chase Fiona */
void func_002947F0(Pursuer *p) {
    PURSUER_BEGIN(p, 4);
    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECC70);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x27C, void (*)(Pursuer *))(p);
}

/* vtable +0x25C */
void func_00296ED0(Pursuer *p) {
    PURSUER_BEGIN(p, 8);
    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECB90);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x260, void (*)(Pursuer *))(p);
}

/* ---- batch 4 ---- */

extern VObject *D_0044FE10;
extern void *D_0046A620[], *D_00474560[], *D_0046D810[], *D_0046C220[], *D_00469C60[], *D_00469C20[];
extern const PTMF D_003ED190, D_003ED1A0, D_003ED0B0, D_003ECE20, D_003ED510, D_003ED520, D_003ECDA0,
    D_003ECF00;


/* vtable +0x40 (via +0x84 first): blocking flags (8 = held at a door), then +0x110 / +0x40 */
void func_0029E390(Pursuer *p) {
    VCALL(p, 0x84, void (*)(Pursuer *))(p);
    p->c.a.navMask = p->c.a.unk2B == 1 ? 8 : VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathReq->mask = p->c.a.navMask;
    if (func_00217510(p) != 0) {
        func_00297C60(p);
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
    }
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
}

/* the target out of reach on the current triangle: search (0x12) or wait at a door (0xB) */
s32 func_00284440(Pursuer *p) {
    if (PU(p, 0x15A4, s32) == -1) {
        return 0;
    }
    if ((func_002138F0(p, PU(p, 0x15A4, s32)) & 0xFF) == 1) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
        return 1;
    }
    if ((func_00213690(p, PU(p, 0x15A4, s32)) & 0xFF) == 1) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xB);
        return 1;
    }
    if (AT(D_0044E570, 0x14, u32) >= 2 && (func_00212FE0(p, -1) & 0xFF) == 1) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
        return 1;
    }
    return 0;
}

/* a cry heard (gProgress +0x1020 per slot): hold for (+0x1748)[0] frames (90 without), with
 * its sound +0x1764 and reaction animation */
u32 func_0029B4B0(Pursuer *p) {
    Progress *pr = gProgress;
    u32 heard = AT(pr, 0x1020 + *(u8 *)&p->c.a.slot * 0x10, u8) & ~PU(p, 0x1760, u8) & 0xFF;

    if (heard != 0) {
        s32 *t = PU(p, 0x1748, s32 *);
        s32 snd;

        PU(p, 0x178C, s32) = t != NULL ? *t : 90;
        PU(p, 0x1760, u8) |= heard;
        snd = PU(p, 0x1764, s32);
        if (snd >= 0) {
            if (VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) == 0 && Progress_TestFlag(pr, 8) == 0) {
                func_00122C20(&p->c.a, snd, 7, 0, 0, NULL);
            }
            p->c.unk14D0 = 5;
            func_001F6E30(p->c.motion);
        }
    }
    PU(p, 0x1764, s32) = -1;
    return heard;
}

/* the model takes the motion banks of its file +0x1670; the message image +0x1674 */
void func_0029EE70(Pursuer *p) {
    u8 *f = PU(p, 0x1670, u8 *);
    u8 *m = p->c.motion;

    AT(m, 0x4C0, u8 *) = AT(f, 0x4, s32) != 0 ? f + AT(f, 0x4, s32) : NULL;
    AT(m, 0x4D0, u8 *) = AT(f, 0x8, s32) != 0 ? f + AT(f, 0x8, s32) : NULL;
    AT(m, 0x4CC, u8 *) = AT(f, 0xC, s32) != 0 ? f + AT(f, 0xC, s32) : NULL;
    AT(m, 0x4C4, u8 *) = AT(f, 0x10, s32) != 0 ? f + AT(f, 0x10, s32) : NULL;
    if (*AT(PU(p, 0x168C, u8 *), 0x8, char *) != 0) {
        p->c.msgSlot = p->c.a.slot;
        if ((VCALL(gBootMessage, 0x8, s32 (*)(VObject *, u32, s32))(gBootMessage, p->c.msgSlot, PU(p, 0x1674, s32)) & 0xFF) == 1) {
            p->c.a.unkD0 = 1;
        }
        m = p->c.motion;
        VCALL(m, 0xC, void (*)(void *))(m);
        p->c.a.unkD1 = 1;
        MOTION_AT(p, 0x24, u8) = p->c.msgSlot;
    }
}

/* threat: `amount` within 10 units of the target, * 0.75 for each further 10, none past 40 */
void func_002982A0(Pursuer *p, f32 amount) {
    f32 d = PU(p, 0x1588, f32);

    if (d < 0.0f) {
        return;
    }
    if (d < 10.0f) {
        func_002EF9E0((u8 *)gProgress + 0x7B8, amount);
    } else if (d < 20.0f) {
        func_002EF9E0((u8 *)gProgress + 0x7B8, 0.75f * amount);
    } else if (d < 30.0f) {
        func_002EF9E0((u8 *)gProgress + 0x7B8, 0.75f * (0.75f * amount));
    } else if (d < 40.0f) {
        func_002EF9E0((u8 *)gProgress + 0x7B8, 0.75f * (0.75f * (0.75f * amount)));
    }
}

/* stance step */
void func_00280090(Pursuer *p) {
    u8 k;

    func_00214ED0(p);
    func_0027FE90(p);
    k = PU(p, 0x16C8, u8);
    switch (k) {
    case 0:
        if (PU(p, 0x1544, u8) == 0) {
            PU(p, 0x16C8, u8) = 2;
            VCALL(p, 0x2C0, void (*)(Pursuer *, u32))(p, k);
            PU(p, 0x16C9, u8) = 3;
            PU(p, 0x16CA, u8) = 4;
        }
        break;
    case 1:
        if (func_00217260(p) != 0) {
            if (PU(p, 0x159C, s32) == -2) {
                PU(p, 0x159C, s32) = -1;
            }
            if (PU(p, 0x16C9, u8) == 2) {
                PU(p, 0x16C9, u8) = 0;
                PU(p, 0x16CB, u8) = 0;
                PU(p, 0x16CA, u8) = 0;
                PU(p, 0x16CC, u8) = 0;
                PU(p, 0x179C, s32) = -1;
                PU(p, 0x16F1, u8) = 0;
                PU(p, 0x16F3, u8) = 0;
                PU(p, 0x16F2, u8) = 0;
                PU(p, 0x16F4, u8) = 0;
            }
            PU(p, 0x16C8, u8) = 2;
        }
        break;
    }
}

/* play animation `anim` (restart only if it ended or doesn't loop); 1 if started */
s32 func_00297B40(Pursuer *p, s32 anim, s32 blend) {
    u8 *m = p->c.motion;

    if (anim == AT(m, 0x55C, s32)) {
        s32 over = AT(m, 0x550, f32) <= 0.0f;

        if ((over ^ 1) & 0xFF) {
            return 0;
        }
        if (((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) != 1) {
            s32 i = func_001F4710(m, anim);
            u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

            if (fl & 4) {
                return 0;
            }
        }
        func_002DDED0(p->c.motion, anim, -1);
        return 1;
    }
    if (blend == 0) {
        func_002DDED0(m, anim, -1);
    } else {
        func_002DDE20(m, anim, -1);
    }
    return 1;
}

/* hit reaction by the animation being played (inlined in func_00287950) */
static void Pursuer_HitReact(Pursuer *p) {
    if (p->c.a.unkC4 != 2 && MOTION_ANIM(p) == 0x1709) {
        PU(p, 0x1624, s32) = 0x1806;
    } else {
        PU(p, 0x1624, s32) = MOTION_ANIM(p);
    }
    PU(p, 0x1628, s32) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (PU(p, 0x1624, s32) != 0x1807 && PU(p, 0x1624, s32) != 0x1803) {
        Actor_SetState(&p->c.a, &D_003ED1A0);
        func_00287CD0(p);
    } else {
        Actor_SetState(&p->c.a, &D_003ED190);
        func_00287B50(p);
    }
}

void func_00288030(Pursuer *p) {
    Pursuer_HitReact(p);
}

/* keep walking until the animation (+0x550) is over; returns 1 while walking */
static s32 Pursuer_WalkOn(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) == 1) {
        if (PU(p, 0x15C0, u8) != 0xFF) {
            if (p->c.unk128 < p->c.unk124) {
                func_00214620(p, p->c.unk128);
            }
        } else {
            func_00125A10(&p->c);
        }
        return 1;
    }
    return 0;
}

void func_0028A540(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    p->target = gCharPlayer;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED0B0);
    func_0028A190(p);
}

/* vtable +0x1C8 */
void func_0028FC10(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    PU(p, 0x1568, s32) = 0;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECE20);
    VCALL(p, 0x1AC, void (*)(Pursuer *))(p);
}

/* vtable +0x184 */
void func_00291880(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    func_00297300(p, (u8)p->c.unk104[0]);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECDA0);
    VCALL(p, 0x188, void (*)(Pursuer *))(p);
}

/* vtable +0x1F0 */
void func_0028DAA0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    VCALL(p, 0x128, void (*)(Pursuer *))(p);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECF00);
    VCALL(p, 0x1E8, void (*)(Pursuer *))(p);
}

/* vtable +0x74: the point the attack lands (+0x1770), while attacking */
s32 func_0029CBE0(Pursuer *p, f32 *out) {
    if (PU(p, 0x175C, s32) != 0x23) {
        if (p->c.moveMode == 8) {
            s32 sub = p->c.moveSub;

            if (sub != 0x1A && sub != 0x1B && sub != 0x1C) {
                return 0;
            }
            if (PU(p, 0x1728, s32) >= 0 && !(func_001F4770(p->c.motion, 0, -2, 1) & 0xFF & 2)) {
                return 0;
            }
            sceVu0CopyVector(out, (f32 *)((u8 *)p + 0x1770));
            return 1;
        }
        return 0;
    }
    if (!(func_001F4770(p->c.motion, 0, -2, 1) & 0xFF & 2)) {
        return 0;
    }
    sceVu0CopyVector(out, (f32 *)((u8 *)p + 0x1770));
    return 1;
}

/* vtable +0x290: the idle behaviour (another one while the progress byte +0x1FBEC1 is set) */
void func_0027E440(Pursuer *p) {
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED510);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x298, void (*)(Pursuer *))(p);
        return;
    }
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED520);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x294, void (*)(Pursuer *))(p);
}

/* vtable +0x188: once the animation ends, back to the stand animation */
void func_00291760(Pursuer *p) {
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        u8 *m;

        PURSUER_STEP_DONE(p) = 1;
        p->c.unk104[0] = -1;
        m = p->c.motion;
        if (AT(m, 0x55C, s32) == 0) {
            s32 over = AT(m, 0x550, f32) <= 0.0f;

            if (!((over ^ 1) & 0xFF)) {
                if (((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) != 1) {
                    s32 i = func_001F4710(m, 0);
                    u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

                    if (fl & 4) {
                        return;
                    }
                }
                func_002DDED0(p->c.motion, 0, -1);
            }
        } else {
            func_002DDED0(m, 0, -1);
        }
    } else {
        func_00125A10(&p->c);
    }
}

/* the chance roll, rising each time it fails: true when 100 * random <= the chance
 * (table +0x1740 by situation: {chance, rise}) plus what built up (+0x16C0) */
s32 func_00297160(Pursuer *p) {
    f32 *t = PU(p, 0x1740, f32 *);

    if (t != NULL) {
        f32 chance, rise;

        if (gCharPartner->a.unkC4 == 2) {
            chance = t[0];
            rise = t[1];
        } else if (PU(p, 0x1760, u8) & 2) {
            chance = t[2];
            rise = t[3];
        } else if (PU(p, 0x1761, u8) != 0) {
            chance = t[6];
            rise = t[7];
        } else {
            chance = t[4];
            rise = t[5];
        }
        if (100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) <= chance + PU(p, 0x16C0, f32)) {
            PU(p, 0x16BC, s32) = 0;
            PU(p, 0x16C0, f32) = 0.0f;
        } else {
            PU(p, 0x16C0, f32) += rise;
            return 0;
        }
    } else {
        PU(p, 0x16BC, s32) = 0;
        PU(p, 0x16C0, f32) = 0.0f;
    }
    PU(p, 0x16F8, u8) = 0;
    return 1;
}

/* vtable +0x48: place the model on the character (or let the model place it) */
void func_0029A2A0(Pursuer *p) {
    u8 *m;

    if (p->c.unkE3 == 0) {
        f32 mat[4][4] __attribute__((aligned(16)));

        sceVu0UnitMatrix(mat);
        m = p->c.motion;
        VCALL(m, 0x28, void (*)(void *, f32 (*)[4]))(m, mat);
    } else {
        m = p->c.motion;
        VCALL(m, 0x40, void (*)(void *, Pursuer *, f32, f32))(m, p, 0.0f, 0.0f);
    }
    if (p->c.a.disabled == 0) {
        func_001F6AF0(p->c.motion);
        if (VCALL(D_0044FE10, 0x54, s32 (*)(VObject *, s32, s32))(D_0044FE10, 0, 0) > 0) {
            MOTION_AT(p, 0x850, u8) = 1;
        }
        m = p->c.motion;
        VCALL(m, 0x3C, void (*)(void *))(m);
        p->c.a.room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
        if (p->c.unkE3 == 0) {
            sceVu0CopyVector(p->c.a.pos, func_0017CE80(MOTION_AT(p, 0x810, u8 *), 0) + 0xC);
        }
        p->c.a.navTri = VCALL(D_0044E570, 0x3C, u32 (*)(void *, f32 *, s32))(D_0044E570, p->c.a.pos, 0);
    }
}

/* destructor of the kind 0x15 class (vtables 0x474560 / 0x46A620 -> 0x46D810 -> 0x46C220 ->
 * Character) */
Pursuer *func_00179970(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_0046A620;
        if (p != NULL) {
            p->c.a.vtbl = D_00474560;
            if (p != NULL) {
                p->c.a.vtbl = D_0046D810;
                VCALL(p, 0x10, void (*)(Pursuer *))(p);
                if ((u32)p->c.a.slot >= 3 && (u32)p->c.a.slot < 6) {
                    void **m = p->c.motion;

                    if (m != NULL) {
                        if (m != NULL) {
                            VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                        }
                        p->c.motion = NULL;
                    }
                }
                if (p != NULL) {
                    p->c.a.vtbl = D_0046C220;
                    VCALL(p, 0x10, void (*)(Pursuer *))(p);
                    if (p != NULL) {
                        p->c.a.vtbl = D_00469C60;
                        if (p != NULL) {
                            p->c.a.vtbl = D_00469C20;
                        }
                    }
                }
            }
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* ---- batch 5 ---- */

extern const PTMF D_003ECF50, D_003ECFC0, D_003ECF10, D_003ECF90, D_003ECCD0, D_003ED090, D_003ED0A0,
    D_003ECDB0;
extern void *D_0046D810[];


/* vtable +0x8: destructor (Pursuer 0x46D810 -> NPC 0x46C220 -> Character); the model is
 * freed for slots 3..5 */
Pursuer *func_00172810(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_0046D810;
        VCALL(p, 0x10, void (*)(Pursuer *))(p);
        if ((u32)p->c.a.slot >= 3 && (u32)p->c.a.slot < 6) {
            void **m = p->c.motion;

            if (m != NULL) {
                if (m != NULL) {
                    VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                }
                p->c.motion = NULL;
            }
        }
        if (p != NULL) {
            p->c.a.vtbl = D_0046C220;
            VCALL(p, 0x10, void (*)(Pursuer *))(p);
            if (p != NULL) {
                p->c.a.vtbl = D_00469C60;
                if (p != NULL) {
                    p->c.a.vtbl = D_00469C20;
                }
            }
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* vtable +0x1F8: open the door at exit +0x17B0 and step through */
void func_0028D5B0(Pursuer *p) {
    VObject *d = D_0044E558;
    u32 side;

    p->c.unk100 = PU(p, 0x17B0, u8);
    side = ((s8)VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, (u8)p->c.unk100, p->c.a.pos) != 0 ? 3 : 1) & 0xFFFF;
    if ((func_00178DB0(gProgress, p->c.a.room, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        p->c.unk100 = -1;
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    p->c.moveMode = 2;
    p->c.moveSub = 0x15;
    func_00212DC0(p, (u8)p->c.unk100);
    VCALL(d, 0xC, void (*)(VObject *, u32, u32, s32, s32))(d, (u8)p->c.unk100, side, p->c.a.slot, 1);
    Actor_SetState(&p->c.a, &D_003ECF50);
}

/* arrive at the goal (+0x15A4 / +0x15B0): snap onto it, or turn to the heading +0x10C */
void func_00299370(Pursuer *p) {
    if (PURSUER_STEP_DONE(p) == 1) {
        s32 tri = PU(p, 0x15A4, s32);

        if ((s32)p->c.a.navTri != tri) {
            f32 d = func_001257B0(&p->c, tri, (f32 *)((u8 *)p + 0x15B0), -1);

            if (d < 0x1.99999a0000000p-4f /* 0.1 */ && !(d < 0.0f)) {
                p->c.a.navTri = PU(p, 0x15A4, s32);
                sceVu0CopyVector(p->c.a.pos, (f32 *)((u8 *)p + 0x15B0));
                return;
            }
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
        } else if (func_002140A0(p, AT(p, 0x10C, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) == 0.0f) {
            PURSUER_STEP_DONE(p) = 0;
            if (PU(p, 0x175C, s32) != 0) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0);
            }
        }
    } else if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
}

/* vtable +0x68: does event `kind` concern the pursuer (character `slot`, door `door`)? */
s32 func_0029CD00(Pursuer *p, u32 kind, s32 slot, u32 door) {
    kind &= 0xFF;
    if (kind != 5) {
        Character *c = gCharacters[slot];

        if (c == NULL || (c->a.active == 0 && c->a.disabled == 1)) {
            return 0;
        }
    } else if (!(VCALL(D_0044E558, 0x40, s32 (*)(VObject *, u32))(D_0044E558, door & 0xFF) & 0xFF)) {
        return 0;
    }
    switch (kind) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 11:
        return 1;
    case 6:
        if (p->c.moveMode != 4) {
            return 1;
        }
        return 0;
    case 9:
    case 10:
        if (p->c.moveSub != 9 && p->c.moveSub != 0xA && p->c.moveMode != 3) {
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}

/* walk on to the exit the pursuer heads for */
void func_00285360(Pursuer *p) {
    s32 done = 0;

    if (PU(p, 0x1590, f32) < 0.0f) {
        VObject *rm;
        u32 node;

        if (func_00284440(p) & 0xFF) {
            return;
        }
        rm = D_0044E568;
        PU(p, 0x17B0, u8) = p->c.door;
        PU(p, 0x15A4, s32) = VCALL(rm, 0x34, s32 (*)(VObject *, u32, f32 *))(rm, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
        VCALL(p, 0xD8, void (*)(Pursuer *))(p);
        node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.door) & 0xFFFF;
        p->c.unk148C[node >> 5] |= 1 << (node & 0x1F);
    }
    if ((p->c.unk128 < p->c.unk124) != 1) {
        if (PU(p, 0x1590, f32) != 0.0f) {
            PU(p, 0x16EF, u8) = 1;
        } else {
            done = 1;
        }
    } else {
        done = func_00214620(p, p->c.unk128) & 0xFF;
    }
    if (done == 1) {
        PURSUER_STEP_DONE(p) = 1;
    }
}

/* turned to the door (+0x1568): open it with the arm (animation 0x700 / 0x704 by +0x104) */
void func_0028C420(Pursuer *p) {
    u8 *m;

    if (func_002140A0(p, PU(p, 0x1568, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) != 0.0f) {
        return;
    }
    m = p->c.motion;
    if (AT(m, 0x858, f32) == 0.0f) {
        s32 over = AT(m, 0x550, f32) <= 0.0f;

        if (((over ^ 1) & 0xFF) != 1) {
            func_001779F0(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot);
            p->c.a.unk2A = 1;
            PU(p, 0x15C0, u8) = 0xFF;
            PU(p, 0x1624, s32) = PU(p, 0x15A4, s32);
            if (p->c.unk104[0] == 1) {
                func_002DDD20(p->c.motion, 0x700, -1);
            } else {
                func_002DDD20(p->c.motion, 0x704, -1);
            }
            Actor_SetState(&p->c.a, &D_003ECFC0);
        }
    }
}

/* how is Fiona doing (for the chase): 0 calm, 1 in move mode 0xB, 2 panicking, 3 out of play
 * or in move 0xA / 0xB, 4 / 5 by the threat stage (gProgress +0x7B8) */
s32 func_00283EF0(Pursuer *p) {
    s32 can;

    if (p->target->a.unk2D == 1 || (Progress_TestFlag(gProgress, 0xE) & 0xFF) == 1 || gCharPlayer->moveSub == 0x10) {
        can = 0;
    } else {
        can = 1 & 0xFF;
    }
    if (can & 0xFF) {
        s32 hiding = 1;

        if (gCharPlayer->moveSub != 0xA && gCharPlayer->moveSub != 0xB) {
            hiding = 0;
        }
        switch (AT(gProgress, 0x7B8, u8)) {
        case 4:
            if (hiding == 0) {
                return 4;
            }
            /* fallthrough */
        case 5:
            return 5;
        default:
            if (hiding == 0) {
                if (gCharPlayer->moveMode == 0xB) {
                    return 1;
                }
                return func_00217560() == 0 ? 0 : 2;
            }
            return 3;
        }
    }
    return 3;
}

/* vtable +0x1E8: through the door the pursuer heads for */
void func_0028D950(Pursuer *p) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    VObject *d;
    u32 r, side;

    if (VCALL(D_0044E568, 0x34, s32 (*)(VObject *, u32, f32 *))(D_0044E568, p->c.door, a) == -1) {
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    d = D_0044E558;
    r = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, p->c.door, a) & 0xFF;
    if (r == 0) {
        side = 1;
    } else if (r == 1) {
        side = 3;
    } else {
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    PU(p, 0x15C4, s32) = VCALL(d, 0x14, s32 (*)(VObject *, u32, u32, f32 *, f32 *, s32))(d, p->c.door, side, (f32 *)((u8 *)p + 0x15D0), b, 1);
    if (!(VCALL(p, 0xDC, s32 (*)(Pursuer *))(p) & 0xFF)) {
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    Actor_SetState(&p->c.a, &D_003ECF10);
    VCALL(p, 0x1EC, void (*)(Pursuer *))(p);
}

/* vtable +0x90: reset after an event (stance 4 resolved, Hewie back on his feet: Fiona's
 * relief) */
void func_0029A3D0(Pursuer *p) {
    if (p->c.moveMode == 2 && (Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
        func_00213270(p, 0xFF);
    }
    if (PU(p, 0x16C8, u8) == 4) {
        if (VCALL(p, 0xC0, s32 (*)(Pursuer *))(p) != 0) {
            PU(p, 0x16C8, u8) = 0;
            VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
            PU(p, 0x16C9, u8) = 6;
            PU(p, 0x16CA, u8) = 7;
        } else {
            PU(p, 0x16C8, u8) = 2;
            func_0027E790(p);
            PU(p, 0x16C9, u8) = 2;
            PU(p, 0x16CA, u8) = 3;
        }
    }
    PURSUER_STEP_NEXT(p) = 1;
    PU(p, 0x16F6, u8) = 1;
    p->c.unk100 = -1;
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    func_00126360(&p->c);
    if (p->c.a.unkC4 == 2) {
        p->c.a.unkC4 = 0;
        p->c.hp = p->c.hpMax;
        func_00178070(gProgress, *(u8 *)&gCharPlayer->a.slot, 4, 3, 0, 0, 0.0f);
    }
}

/* vtable +0x214: push through the door +0x100 */
void func_0028CE20(Pursuer *p) {
    if (VCALL(D_0044E568, 0x70, s32 (*)(VObject *, s32, u32))(D_0044E568, p->c.a.room, (u8)p->c.unk100) != 0 &&
        p->c.a.unk2B != 0 && !(func_00177BF0(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF & 8)) {
        p->c.a.unk2B = 0;
    }
    if (VCALL(D_0044E558, 0x70, s32 (*)(VObject *, u32))(D_0044E558, (u8)p->c.unk100) != 0 && p->c.a.unk2B != 0) {
        f32 v[4] __attribute__((aligned(16)));

        func_002E2C10(v, PU(p, 0x1634, f32));
        func_0010E640(v, v, 3.0f);
        func_001247E0(&p->c.a, v);
    }
    if ((Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) != 1) {
        Actor_SetState(&p->c.a, &D_003ECF90);
        VCALL(p, 0x214, void (*)(Pursuer *))(p);
    } else {
        func_00125A10(&p->c);
    }
}

/* vtable +0x280: go after Fiona (behaviour 0x1C / 0x27 by +0x16CA) */
void func_002934C0(Pursuer *p) {
    s32 prev = PU(p, 0x1758, s32);

    if (prev != -1) {
        if (prev != -2) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, prev);
        }
    } else if (PU(p, 0x16CA, u8) == 4) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1C);
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x27);
    }
    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    if (func_00217260(p) != 0 && !(func_0027CA00(p) & 0xFF)) {
        PURSUER_STEP_NEXT(p) = 1;
        func_0029AF20(p);
        return;
    }
    PU(p, 0x162C, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECCD0);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x284, void (*)(Pursuer *))(p);
}

/* repeated taunt: threat up each time the animation ends, 6 times, then animation 0x1904 */
void func_0028A700(Pursuer *p) {
    func_00125A10(&p->c);
    if (MOTION_KEYS(p) & MOTION_KEY_END) {
        func_002EFA50((u8 *)gProgress + 0x7B8, VCALL(p, 0x304, f32 (*)(Pursuer *))(p));
        p->c.unk104[0]++;
        if (p->c.state[0] != 7) {
            if (p->c.unk104[0] >= 6) {
                func_002DDE20(p->c.motion, 0x1904, -1);
                Actor_SetState(&p->c.a, &D_003ED0A0);
                return;
            }
            func_002DDE20(p->c.motion, 0x1901, -1);
        } else {
            p->c.state[0] = 0;
            func_002DDE20(p->c.motion, 0x1903, -1);
            Actor_SetState(&p->c.a, &D_003ED090);
        }
    }
}

/* vtable +0x194: walk to the goal triangle +0x15A4 */
void func_00291600(Pursuer *p) {
    u32 tri;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    tri = PU(p, 0x15A4, u32);
    if (tri == (u32)-1 || tri >= AT(D_0044E570, 0x8, u32)) {
        VCALL(p, 0xB4, void (*)(Pursuer *, s32))(p, 0);
    }
    if (VCALL(D_0044E570, 0x10, s32 (*)(void *, u32, f32 *))(D_0044E570, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0)) == 4) {
        VCALL(p, 0xB4, void (*)(Pursuer *, s32))(p, 0);
    }
    if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
        if (!(func_00284440(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        func_00125A10(&p->c);
        return;
    }
    PU(p, 0x1624, s32) = -1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECDB0);
    VCALL(p, 0x198, void (*)(Pursuer *))(p);
}

/* ---- batch 6 ---- */

extern const PTMF D_003ED360, D_003ED100, D_003ED2A0, D_003ED1F0, D_003ECF70, D_003ECD60, D_003ECB80,
    D_003ECE40;


/* play `anim` unless it is already playing (or ended and loops) */
static void Pursuer_PlayAnim(Pursuer *p, s32 anim) {
    u8 *m = p->c.motion;

    if (anim == AT(m, 0x55C, s32)) {
        s32 over = AT(m, 0x550, f32) <= 0.0f;

        if ((over ^ 1) & 0xFF) {
            return;
        }
        if (((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) != 1) {
            s32 i = func_001F4710(m, anim);
            u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

            if (fl & 4) {
                return;
            }
        }
        func_002DDED0(p->c.motion, anim, -1);
    } else {
        func_002DDED0(m, anim, -1);
    }
}

/* the attack: pick one from the table +0x1718 ({kind, value, chance} by a 0..100 roll) */
void func_00283AE0(Pursuer *p) {
    u8 *e;
    s32 kind;
    f32 roll;
    u32 i;

    VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, AT(gProgress, 0x7B8, u8) >= 4 ? 7 : 6);
    roll = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);
    for (i = 0;; i = (i + 1) & 0xFF) {
        e = PU(p, 0x1718, u8 *) + (i & 0xFF) * 0xC;
        if (roll <= AT(e, 0x8, f32)) {
            break;
        }
    }
    kind = AT(e, 0x0, s32);
    if (kind == 0x17) {
        p->c.unk104[0] = AT(e, 0x4, s32);
    } else if (kind == 0x13) {
        PU(p, 0x1728, s32) = AT(e, 0x4, s32);
        PU(p, 0x172C, u8) = 0;
    }
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED360);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, kind);
}

/* how much ground the pursuer gains on its target in 5 frames of the animations (the target's
 * stride counted only while it can't see the pursuer: 150 units, 90 degrees either side) */
f32 func_002838E0(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    f32 gain = 0.0f;
    u32 i;

    for (i = 0; i < 5; i = (i + 1) & 0xFF) {
        func_001F6370(p->c.motion, v, (f32)i);
        gain += v[2];
        if (!(func_00218300(p, &p->target->a, &p->c.a, p->target->a.angle[1], 150.0f, 0x1.921fb60000000p+0f /* 1.5707964 */) & 0xFF)) {
            func_001F6370(p->target->motion, v, (f32)i);
            gain -= v[2];
        }
    }
    if (AT(gProgress, 0x7B8, u8) == 5 && p->target == gCharPlayer) {
        return 10.0f + gain;
    }
    return gain + VCALL(p, 0x2F0, f32 (*)(Pursuer *))(p);
}

/* keep standing on walkable ground (step off triangles that block the pursuer) */
void func_0029E210(Pursuer *p) {
    if (func_00214A90(p, p->c.a.navTri) != 0 && p->c.a.unk2B == 0 && func_00217510(p) != 0) {
        p->c.a.navTri = func_00211B00(p, p->c.a.navTri);
        if (p->c.a.navTri != (u32)-1 && !(func_00214A90(p, p->c.a.navTri) & 0xFF)) {
            VCALL(D_0044E570, 0xC, void (*)(void *, u32, f32 *))(D_0044E570, p->c.a.navTri, p->c.a.pos);
        } else {
            func_00124890(&p->c.a, -1);
        }
    }
    if (p->c.moveMode == 0) {
        void *nm = D_0044E570;

        if (VCALL(nm, 0x10, s32 (*)(void *, u32, f32 *))(nm, p->c.a.navTri, p->c.a.pos) == 4) {
            if (func_00214A90(p, p->c.a.navTri) & 0xFF) {
                p->c.a.navTri = func_00211B00(p, p->c.a.navTri);
                if (p->c.a.navTri == (u32)-1 || func_00214A90(p, p->c.a.navTri) != 0) {
                    func_00124890(&p->c.a, -1);
                }
            } else {
                VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, p->c.a.navTri, p->c.a.pos);
            }
        }
    }
}

/* hit reaction over: back to the stand animation */
void func_00287B50(Pursuer *p) {
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        if (p->c.a.unkC4 != 0) {
            p->c.a.unkC4 = 0;
        }
        PURSUER_STEP_NEXT(p) = 1;
        Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        PU(p, 0x1761, u8) = 0;
        PU(p, 0x1624, s32) = 0;
        PU(p, 0x1628, s32) = 0;
        PU(p, 0x1784, s32) = 0;
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    PURSUER_STEP_NEXT(p) = 0;
    func_00213B60(p, -1);
    func_00125A10(&p->c);
}

/* vtable +0x...: look around (animation 0x1A00) */
void func_00289DD0(Pursuer *p) {
    u8 *m;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    m = p->c.motion;
    if (AT(m, 0x55C, s32) != 0x1A00) {
        func_002DDD20(m, 0x1A00, -1);
    }
    VCALL(p, 0x12C, void (*)(Pursuer *, f32))(p, VCALL(p, 0x308, f32 (*)(Pursuer *))(p));
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED100);
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_00125A10(&p->c);
    }
}

/* vtable +0x258: go to the door the pursuer heads for */
void func_00284FD0(Pursuer *p) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    VObject *d;
    u32 r, side;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (VCALL(D_0044E568, 0x34, s32 (*)(VObject *, u32, f32 *))(D_0044E568, p->c.door, a) == -1) {
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    d = D_0044E558;
    r = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, p->c.door, a) & 0xFF;
    if (r == 0) {
        side = 1;
    } else if (r == 1) {
        side = 3;
    } else {
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    PU(p, 0x15A4, s32) = VCALL(d, 0x14, s32 (*)(VObject *, u32, u32, f32 *, f32 *, s32))(d, p->c.door, side, (f32 *)((u8 *)p + 0x15B0), b, 1);
    if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
        if (!(func_00284440(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED2A0);
    VCALL(p, 0x198, void (*)(Pursuer *))(p);
}

/* plan where to go by the stance +0x16C8: to Fiona's room, through a random other exit, ... */
s32 func_0027CA00(Pursuer *p) {
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        PU(p, 0x1594, s32) = gCharPlayer->a.room;
        PU(p, 0x1598, s32) = func_002172F0(p, gCharPlayer);
        /* fallthrough */
    case 1:
    case 2:
        return func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0;
    case 3: {
        s32 again = 0;
        VObject *rm;
        u32 exit;

        for (;;) {
            exit = func_00212A80(p, 0xFF) & 0xFF;
            if (exit == 0xFF) {
                return 0;
            }
            if (again != 0) {
                break;
            }
            again = 1;
            if (exit != p->c.door) {
                break;
            }
        }
        rm = D_0044E568;
        PU(p, 0x1594, s32) = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit);
        PU(p, 0x1598, s32) = -1;
        if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0) {
            return 1;
        }
        PU(p, 0x1594, s32) = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.door);
        return func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0;
    }
    case 4:
        return VCALL(p, 0xE8, s32 (*)(Pursuer *))(p);
    default:
        return 0;
    }
}

/* vtable +0x1C: the model files loaded: give the model its motion banks, message image, door
 * sounds and +0x4D4 (slot 2; others func_0029EE70) */
void func_0029F120(Pursuer *p) {
    if (p->c.a.slot == 2) {
        u8 *f = PU(p, 0x1670, u8 *);
        u8 *m = p->c.motion;

        AT(m, 0x4C0, u8 *) = AT(f, 0x4, s32) != 0 ? f + AT(f, 0x4, s32) : NULL;
        AT(m, 0x4D0, u8 *) = AT(f, 0x8, s32) != 0 ? f + AT(f, 0x8, s32) : NULL;
        AT(m, 0x4CC, u8 *) = AT(f, 0xC, s32) != 0 ? f + AT(f, 0xC, s32) : NULL;
        AT(m, 0x4C4, u8 *) = AT(f, 0x10, s32) != 0 ? f + AT(f, 0x10, s32) : NULL;
        if (*AT(PU(p, 0x168C, u8 *), 0x8, char *) != 0) {
            p->c.msgSlot = p->c.a.slot;
            if ((VCALL(gBootMessage, 0x8, s32 (*)(VObject *, u32, s32))(gBootMessage, p->c.msgSlot, PU(p, 0x1674, s32)) & 0xFF) == 1) {
                p->c.a.unkD0 = 1;
            }
            m = p->c.motion;
            VCALL(m, 0xC, void (*)(void *))(m);
            p->c.a.unkD1 = 1;
            MOTION_AT(p, 0x24, u8) = p->c.msgSlot;
        }
        if (*AT(PU(p, 0x168C, u8 *), 0xC, char *) != 0) {
            VCALL(D_0044E558, 0x4C, void (*)(VObject *, s32))(D_0044E558, 1);
        }
        if (*AT(PU(p, 0x168C, u8 *), 0x4, char *) != 0) {
            MOTION_AT(p, 0x4D4, s32) = PU(p, 0x1678, s32);
        } else {
            MOTION_AT(p, 0x4D4, s32) = 0;
        }
        return;
    }
    func_0029EE70(p);
}

/* vtable +0x...: the door ahead: stand back if it's in the way, else turn away from it */
void func_002871E0(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    Progress *pr;
    u32 st;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    pr = gProgress;
    st = func_00177BF0(pr, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF;
    VCALL(D_0044E558, 0x38, void (*)(VObject *, u32, f32 *))(D_0044E558, (u8)p->c.unk100, v);
    if ((func_00124490(&p->c.a, v) < 9.0f || (st & 0xFF & 9)) &&
        func_00178300(pr, p->c.a.room, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) != 0) {
        PU(p, 0x1634, f32) = func_00212550(p, (u8)p->c.unk100);
        p->c.a.unk2B = 1;
    } else {
        if (p->c.hp <= 0) {
            if (p->c.a.unkC4 == 2) {
                p->c.hp = p->c.hpMax;
                PU(p, 0x1664, s32) = 1;
            } else {
                p->c.hp = 1;
            }
        }
        PU(p, 0x1634, f32) = func_002E2D00(0x1.921fb60000000p+1f /* 3.1415927 */ + func_00212550(p, (u8)p->c.unk100));
        p->c.unk100 = 0xFF;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED1F0);
    func_00286F10(p);
}

/* vtable +0x204: back off from the door +0x100 */
void func_0028D0C0(Pursuer *p) {
    f32 h;

    if (VCALL(D_0044E568, 0x70, s32 (*)(VObject *, s32, u32))(D_0044E568, p->c.a.room, (u8)p->c.unk100) != 0 &&
        p->c.a.unk2B != 0 && !(func_00177BF0(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF & 8)) {
        p->c.a.unk2B = 0;
    }
    if (VCALL(D_0044E558, 0x70, s32 (*)(VObject *, u32))(D_0044E558, (u8)p->c.unk100) != 0 && p->c.a.unk2B != 0) {
        f32 v[4] __attribute__((aligned(16)));

        func_002E2C10(v, PU(p, 0x1634, f32));
        func_0010E640(v, v, 3.0f);
        func_001247E0(&p->c.a, v);
    }
    h = func_002E2D00(0x1.921fb60000000p+1f /* 3.1415927 */ + PU(p, 0x1634, f32));
    func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    if ((Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) != 1) {
        Actor_SetState(&p->c.a, &D_003ECF70);
        VCALL(p, 0x208, void (*)(Pursuer *))(p);
    } else {
        func_00125A10(&p->c);
    }
}

/* vtable +0x...: stand (vtable +0x320 animation) */
void func_00292170(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECD60);
    if (p->c.unkE0 != 0) {
        s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

        if (!((over ^ 1) & 0xFF)) {
            p->c.unkE1 = 1;
        }
    }
}

/* vtable +0x260: a door near the pursuer it should go through (not during behaviours 0x10,
 * 0x11, 0x21, 0x22) */
void func_00296FC0(Pursuer *p) {
    s32 b = PU(p, 0x175C, s32);

    if (b != 0x22 && b != 0x11 && b != 0x21 && b != 0x10 && p->c.moveMode != 2) {
        u32 door = func_00212850(p) & 0xFF;

        if (door != 0xFF) {
            p->c.unk100 = door;
            if (p->c.moveSub == 0xA) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x11);
            } else {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECB80);
                PU(p, 0x1758, s32) = -1;
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x10);
            }
            if (p->c.a.unkC4 != 2) {
                PU(p, 0x16C8, u8) = 0;
                VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
                PU(p, 0x16C9, u8) = 6;
                PU(p, 0x16CA, u8) = 7;
            }
            if ((VCALL(D_0044E558, 0x70, s32 (*)(VObject *, u32))(D_0044E558, (u8)p->c.unk100) & 0xFF) == 1 &&
                (func_00177BF0(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF & 8)) {
                p->c.a.unk2B = 1;
            }
        }
    }
}

/* turn to the heading +0x10C, then wait for the animation (or play the stand animation) */
void func_00299140(Pursuer *p) {
    if (func_002140A0(p, AT(p, 0x10C, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) == 0.0f) {
        if (PU(p, 0x1788, s32) == 0) {
            s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

            if (!((over ^ 1) & 0xFF)) {
                AT(p, 0x10C, s32) = 0;
                p->c.unkE1 = 1;
            }
        } else {
            Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        }
    }
}

/* vtable +0x40: model update: placed by its stance (+0x1694.. by +0x1788) or on the floor */
void func_0029EAB0(Pursuer *p) {
    s32 on = 0;
    u8 *m;

    if (func_00217510(p) != 0) {
        if (p->c.a.navTri != (u32)-1) {
            on = 1;
            if (p->c.moveMode == 3 || (p->c.unkE0 == 1 && p->c.a.unk2B == 1)) {
                on = 0;
            }
            m = p->c.motion;
            if ((on & 0xFF) == 1) {
                if (PU(p, 0x1788, s32) == 0x201) {
                    VCALL(m, 0x40, void (*)(void *, Pursuer *, f32, f32))(m, p, PU(p, 0x1694, f32), PU(p, 0x169C, f32));
                } else {
                    VCALL(m, 0x40, void (*)(void *, Pursuer *, f32, f32))(m, p, PU(p, 0x1698, f32), PU(p, 0x16A0, f32));
                }
            } else {
                VCALL(m, 0x40, void (*)(void *, Pursuer *, f32, f32))(m, p, 0.0f, 0.0f);
            }
        } else {
            func_002DCDD0(p->c.motion, p, 0.0f, 0.0f);
        }
    }
    func_00284040(p);
    func_002DCB40(p->c.motion);
    func_002DC960(p->c.motion);
    func_001F6AF0(p->c.motion);
    if (p->c.a.room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && p->c.a.navTri != (u32)-1) {
        m = p->c.motion;
        VCALL(m, 0x4C, void (*)(void *, s32, Pursuer *))(m, on, p);
    }
    m = p->c.motion;
    VCALL(m, 0x3C, void (*)(void *))(m);
}

/* vtable +0x8C: back to normal (deactivation of an event) */
void func_0029A520(Pursuer *p) {
    if (p->c.moveMode == 2 && (Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
        func_00213270(p, 0xFF);
    }
    if (gCharPartner != NULL && gCharPartner->a.active == 1 && gCharPartner->unkE0 == 0 && p->c.moveSub == 9) {
        VCALL(gCharPartner, 0x7C, void (*)(Character *))(gCharPartner);
    }
    if (gCharPlayer->unkE0 == 0 && (u32)(p->c.moveSub - 0x18) < 2) {
        VCALL(gCharPlayer, 0x7C, void (*)(Character *))(gCharPlayer);
    }
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x16F5, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x16EC, u8) = 0;
    PU(p, 0x1710, u8) = 0;
    PU(p, 0x16F9, u8) = 0;
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x15A0, u8) = 1;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    PU(p, 0x17B4, s32) = 0;
    PU(p, 0x1728, s32) = -1;
    PU(p, 0x172C, u8) = 0;
    PU(p, 0x1738, s32) = 0;
    func_0029A6D0(p);
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    PU(p, 0x1770, s32) = 0;
    PU(p, 0x1774, s32) = 0;
    PU(p, 0x1778, s32) = 0;
    p->c.unk100 = -1;
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    func_00126450(&p->c);
}

/* the door-opening animation: done (0x703 / 0x707: placed on the other side) or moving */
void func_0028BD40(Pursuer *p) {
    Progress *pr = gProgress;
    u8 *m;

    if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
        u32 seen = func_00217920(p) & 0xFF;

        if (~PU(p, 0x1760, u8) & seen) {
            func_00178070(pr, *(u8 *)&p->c.a.slot, seen & 0xFF, 4, 5, 0, 10.0f);
        }
    }
    m = p->c.motion;
    if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        f32 ofs[4] __attribute__((aligned(16)));

        switch (AT(m, 0x55C, s32)) {
        case 0x703:
            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, 3, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 0, ofs, p->c.a.pos);
            break;
        case 0x707:
            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, 2, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 1, ofs, p->c.a.pos);
            break;
        }
        func_001779C0(pr, (u8)p->c.unk100, *(u8 *)&p->c.a.slot);
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        p->c.unk100 = -1;
        p->c.unk104[0] = -1;
        p->c.a.unk2A = 0;
        return;
    }
    {
        f32 v[4] __attribute__((aligned(16)));

        func_001F6370(m, v, 0.0f);
        func_00125900(&p->c);
        sceVu0ApplyMatrix(v, p->c.a.rot, v);
        sceVu0AddVector(p->c.a.pos, p->c.a.pos, v);
        p->c.a.pos[3] = 1.0f;
    }
}

/* vtable +0x1B0: go through door +0x100 from side +0x104, if it isn't locked to us */
void func_0028F840(Pursuer *p) {
    Progress *pr = gProgress;

    if ((Progress_CurRoomFlag(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1 &&
        (func_00177BF0(pr, (u8)p->c.unk100, 0) & 0xFF & 4)) {
        goto fail;
    }
    switch (p->c.unk104[0]) {
    case 1:
    case 3:
        if (!(func_00178980(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
            goto fail;
        }
        break;
    case 0:
    case 2:
        if (func_00178980(pr, p->c.a.room, (u8)p->c.unk100) != 0) {
            goto fail;
        }
        break;
    default:
        goto fail;
    }
    if (!(func_00214620(p, p->c.unk128) & 0xFF)) {
        return;
    }
    Actor_SetState(&p->c.a, &D_003ECE40);
    VCALL(p, 0x1B4, void (*)(Pursuer *))(p);
    return;
fail:
    p->c.unk100 = -1;
    p->c.unk104[0] = -1;
    PU(p, 0x1568, s32) = 0;
    PU(p, 0x16EF, u8) = 1;
}

/* ---- batch 7 ---- */

extern const PTMF D_003ECEF0, D_003ED540, D_003ED550, D_003ECE50, D_003ECDF0, D_003ED270, D_003ED250,
    D_003ED1C0, D_003ED1D0, D_003ECEC0, D_003ECE10, D_003ECD90, D_003ED030, D_003ED280, D_003ED290,
    D_003ECED0;

/* add `n` stops to the search route: 60% the room's next point of interest (+0x1738 counts them
 * through), otherwise a random walkable triangle (+0x15DC of the entry: 1) */
void func_0027E5D0(Pursuer *p, s32 n) {
    u32 count = n & 0xFF;

    if ((s32)count > 0) {
        VObject *o = VCALL(D_0044E4D0, 0x64, VObject *(*)(VObject *))(D_0044E4D0);
        s32 *pts = VCALL(o, 0x38, s32 *(*)(VObject *))(o);
        u32 npts = 0, used = 0, i;

        if (pts != NULL) {
            while (pts[npts & 0xFF] != -1) {
                npts = (npts + 1) & 0xFF;
                if (npts >= 8) {
                    break;
                }
            }
        }
        for (i = 0; i < count; i = (i + 1) & 0xFF) {
            VObject *rnd = D_0044E550;
            u32 np = npts & 0xFF;

            if (PU(p, 0x1738, u32) >= np) {
                PU(p, 0x1738, u32) = 0;
            }
            if ((s32)(used & 0xFF) < (s32)np && 100.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd) <= 60.0f) {
                u32 k = PU(p, 0x1738, u32);

                PU(p, 0x1738, u32) = k + 1;
                func_00218C20(p, pts[k]);
                used = (used + 1) & 0xFF;
                PU(p, 0x15DC + PU(p, 0x1621, u8) * 8, u8) = 0;
            } else {
                func_00218C20(p, func_00214940(p));
                PU(p, 0x15DC + PU(p, 0x1621, u8) * 8, u8) = 1;
            }
        }
    }
}

/* vtable +0x1DC: wait at the door until it opens; make noise if Fiona holds it shut */
void func_0028DC40(Pursuer *p) {
    VObject *d = D_0044E558;

    if (!(VCALL(d, 0x30, s32 (*)(VObject *, u32))(d, (u8)p->c.unk100) & 0xFF)) {
        if (p->c.unk104[0] == 0) {
            f32 a = VCALL(d, 0x64, f32 (*)(VObject *, u32))(d, (u8)p->c.unk100);

            if (a > -80.0f && a < -10.0f &&
                ((VCALL(d, 0x6C, s32 (*)(VObject *, s32, u32, f32 *))(d, 2, (u8)p->c.unk100, gCharPlayer->a.pos) & 0xFF) == 1 ||
                 (func_00177BF0(gProgress, (u8)p->c.unk100, 0) & 0xFF & 8))) {
                func_00178070(gProgress, *(u8 *)&p->c.a.slot, 1, 5, 0, *(s16 *)&p->c.unk100, 20.0f);
            }
        }
        func_00125A10(&p->c);
        return;
    }
    if (p->c.moveSub == 0x15) {
        func_00212CA0(p, (u8)p->c.unk100);
    } else {
        func_00212D30(p, (u8)p->c.unk100);
    }
    p->c.moveMode = 0;
    Actor_SetState(&p->c.a, &D_003ECEF0);
    VCALL(p, 0x1E0, void (*)(Pursuer *))(p);
}

/* vtable +0x2A8: give up (unless +0x1664): stand, heal, behaviour by the stance */
void func_0027DDB0(Pursuer *p) {
    if (PU(p, 0x1664, s32) == 0) {
        u8 *m = p->c.motion;

        if (AT(m, 0x55C, s32) == 0) {
            Pursuer_PlayAnim(p, 0);
        } else {
            func_002DDE20(m, 0, -1);
        }
        PU(p, 0x1624, s32) = 0;
        if (p->c.a.unkC4 == 2) {
            p->c.a.unkC4 = 0;
        }
        if (p->c.hp <= 0) {
            p->c.hp = p->c.hpMax;
        }
        PU(p, 0x1761, u8) = 0;
        PU(p, 0x1760, u8) = 0;
        if (PU(p, 0x16C8, u8) != 4) {
            PU(p, 0x16F6, u8) = 1;
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED550);
        } else {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED540);
        }
        PU(p, 0x1758, s32) = -1;
    }
}

/* vtable +0x1B4: at the door, turn to it (within ~60 degrees) or step to its side */
void func_0028F650(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) != 1) {
        u32 t = func_00213EC0(p, PU(p, 0x1568, f32), 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;

        if (t == 0xFF) {
            Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        } else {
            func_00297300(p, t);
            PU(p, 0x1624, s32) = t & 0xFF;
        }
        func_00297C60(p);
        Actor_SetState(&p->c.a, &D_003ECE50);
        VCALL(p, 0x1B8, void (*)(Pursuer *))(p);
    }
}

/* vtable +0x194..: walk the search route (8 stops at most, then give up the search) */
void func_00290620(Pursuer *p) {
    void *nm = D_0044E570;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    for (;;) {
        if (PU(p, 0x1620, u8) < PU(p, 0x1621, u8)) {
            func_00218E70(p);
        } else {
            PU(p, 0x15A4, s32) = func_00214940(p);
            VCALL(nm, 0xC, void (*)(void *, s32, f32 *))(nm, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0));
            func_00218C20(p, PU(p, 0x15A4, s32));
            PU(p, 0x15DC + PU(p, 0x1621, u8) * 8, u8) = 1;
        }
        if (VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) != 0) {
            break;
        }
        if (func_00284440(p) != 0) {
            return;
        }
        PU(p, 0x1620, u8)++;
        if (PU(p, 0x1620, u8) >= 8) {
            s32 k;

            for (k = 0; k < 8; k++) {
                PU(p, 0x15E0 + k * 8, s32) = -1;
                PU(p, 0x15E4 + k * 8, u8) = 0;
            }
            PU(p, 0x1620, u8) = 0xFF;
            PU(p, 0x1621, u8) = 0xFF;
            PU(p, 0x1794, s32) = 0;
            PU(p, 0x16EF, u8) = 1;
            return;
        }
    }
    if (PU(p, 0x15E4 + PU(p, 0x1620, u8) * 8, u8) != 0) {
        if (PU(p, 0x1798, s32) == 0) {
            PU(p, 0x1798, s32) = 150;
        }
    } else {
        PU(p, 0x1798, s32) = 0;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECDF0);
    VCALL(p, 0x198, void (*)(Pursuer *))(p);
}

/* vtable +0x248: walked there; the stance animation */
void func_002854A0(Pursuer *p) {
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p));
    Actor_SetState(&p->c.a, &D_003ED270);
    VCALL(p, 0x24C, void (*)(Pursuer *))(p);
}

/* vtable +0x244: follow the path (+0xE8) to the next exit on it */
void func_002858B0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (VCALL(p, 0xE8, s32 (*)(Pursuer *))(p) & 0xFF) {
        if (func_00217260(p) & 0xFF) {
            if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
                if (!(func_00284440(p) & 0xFF)) {
                    PU(p, 0x16EF, u8) = 1;
                }
                return;
            }
        } else {
            VObject *rm = D_0044E568;

            PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room);
            PU(p, 0x15A4, s32) = VCALL(rm, 0x34, s32 (*)(VObject *, u32, f32 *))(rm, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
            if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
                if (!(func_00284440(p) & 0xFF)) {
                    PU(p, 0x16EF, u8) = 1;
                }
                return;
            }
            if (PU(p, 0x1590, f32) < 20.0f && !(PU(p, 0x1590, f32) < 0.0f)) {
                PURSUER_STEP_DONE(p) = 1;
                return;
            }
        }
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, &D_003ED250);
        VCALL(p, 0x248, void (*)(Pursuer *))(p);
        return;
    }
    func_0029B190(p);
    VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1D);
}

/* the search while out of sight: in the played room, catch up the route by the time spent away
 * (one stop per 150 frames); away, count the time the route still takes */
void func_0027EEA0(Pursuer *p) {
    if (func_00217510(p) != 0) {
        u32 t = PU(p, 0x17B4, u32);

        if (t != 0) {
            s32 left = PU(p, 0x1621, u8) - PU(p, 0x1620, u8);
            u32 steps = (u32)((f32)t / 150.0f) & 0xFF & 0xFF;

            if (left >= (s32)steps) {
                if ((s32)steps < left && steps != (u32)left) {
                    do {
                        PU(p, 0x1620, u8)++;
                    } while (steps != (u32)(PU(p, 0x1621, u8) - PU(p, 0x1620, u8)));
                }
            } else {
                func_0027E5D0(p, (steps - left) & 0xFF);
            }
            if ((s32)steps > 0 && steps != 0xFF) {
                PU(p, 0x1794, s32) = 1800;
            } else {
                PU(p, 0x1794, s32) = 0;
            }
            PU(p, 0x17B4, s32) = 0;
        } else {
            PU(p, 0x1620, u8) = PU(p, 0x1621, u8);
        }
        if (PU(p, 0x1620, u8) == PU(p, 0x1621, u8) && PU(p, 0x1621, u8) != 0xFF) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
        }
    } else if (PU(p, 0x17B4, s32) == 0) {
        u8 k = PU(p, 0x16C8, u8);

        if (k == 3 || k == 2) {
            PU(p, 0x17B4, s32) = (PU(p, 0x1621, u8) - PU(p, 0x1620, u8)) * 150;
        }
    }
}

/* vtable +0x...: hurt: by the animation being played (0x1800 / 0x1804 / 0x1709: knocked down) */
void func_00287950(Pursuer *p) {
    s32 a;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    AT(gProgress, 0x874, u8) = 0;
    PU(p, 0x1784, s32) = 0;
    a = MOTION_ANIM(p);
    if (a != 0x1804 && a != 0x1800 && a != 0x1709) {
        Actor_SetState(&p->c.a, &D_003ED1D0);
        Pursuer_HitReact(p);
    } else {
        Actor_SetState(&p->c.a, &D_003ED1C0);
        func_00288150(p);
    }
}

/* vtable +0x1B8: facing the door; open it (side +0x104) if it is ours to open */
void func_0028E2D0(Pursuer *p) {
    if (func_002140A0(p, PU(p, 0x1568, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) == 0.0f) {
        Progress *pr = gProgress;

        if ((Progress_CurRoomFlag(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1 &&
            (func_00177BF0(pr, (u8)p->c.unk100, 0) & 0xFF & 4)) {
            Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        } else {
            if (func_00178980(pr, p->c.a.room, (u8)p->c.unk100) != 0) {
                PURSUER_STEP_DONE(p) = 1;
                return;
            }
            PURSUER_STEP_NEXT(p) = 0;
            Actor_SetState(&p->c.a, &D_003ECEC0);
        }
    }
}

/* stance by where Fiona is (vtable +0xC0 sees her -> 0 chase; a different side -> 1) */
void func_0027FE90(Pursuer *p) {
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        if (PU(p, 0x16C8, u8) == 4 || p->c.a.room != gCharPlayer->a.room) {
            return;
        }
    } else if (PU(p, 0x16C8, u8) == 4 || func_00217510(p) == 0) {
        return;
    }
    PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
    PU(p, 0x1544, u8) = VCALL(p, 0xC0, s32 (*)(Pursuer *))(p);
    if (PU(p, 0x1544, u8) == 1) {
        VCALL(p, 0xCC, void (*)(Pursuer *))(p);
        PU(p, 0x16C8, u8) = 0;
        VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
    }
    if (PU(p, 0x16C8, u8) == 0) {
        s32 side = func_00217340(p);

        if (side != func_002172F0(p, gCharPlayer)) {
            PU(p, 0x16C8, u8) = 1;
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        }
    }
}

/* vtable +0x18C */
void func_0028FD30(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECE10);
    VCALL(p, 0x190, void (*)(Pursuer *))(p);
}

/* vtable +0x17C */
void func_00291A20(Pursuer *p) {
    PU(p, 0x16EC, u8) = 1;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    if (PU(p, 0x1788, s32) != 0) {
        Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECD90);
    VCALL(p, 0x180, void (*)(Pursuer *))(p);
}

/* walked up: face Fiona (stand animation) */
void func_0028AEC0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    p->target = gCharPlayer;
    p->c.unk104[0] = 0;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED030);
    func_0028AB10(p);
}

/* vtable +0x250: to the next exit of the path; look through it first if it's where Fiona was */
void func_00285150(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (p->c.unk1388 < p->c.unk1384 && func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1) {
        VObject *rm = D_0044E568;

        PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room);
        PU(p, 0x15A4, s32) = VCALL(rm, 0x34, s32 (*)(VObject *, u32, f32 *))(rm, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
        if (func_00212190(p, PU(p, 0x17B0, u8)) != 0) {
            u32 t = func_00213EC0(p, func_00212730(p, PU(p, 0x17B0, u8)), 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;

            if (t == 0xFF) {
                PURSUER_STEP_DONE(p) = 1;
            } else {
                p->c.unk104[0] = t;
                Actor_SetState(&p->c.a, &D_003ED280);
            }
        } else {
            if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
                if (!(func_00284440(p) & 0xFF)) {
                    PU(p, 0x16EF, u8) = 1;
                }
                return;
            }
            PU(p, 0x1784, s32) = 0;
            Actor_SetState(&p->c.a, &D_003ED290);
            VCALL(p, 0x198, void (*)(Pursuer *))(p);
        }
    } else {
        PU(p, 0x16EF, u8) = 1;
    }
}

/* vtable +0x1D4 */
void func_0028E0C0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    if (PU(p, 0x1788, s32) != 0x200) {
        Pursuer_PlayAnim(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p));
    }
    PU(p, 0x1624, s32) = 0;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECED0);
    VCALL(p, 0x1D8, void (*)(Pursuer *))(p);
}

/* ---- batch 8 ---- */

extern const PTMF D_003ED260, D_003ECE30, D_003ED5B0, D_003ECD70, D_003ED130, D_003ED580, D_003ECD80;
extern PTMF D_0045B3A0;   /* followed by the idle move: +0xC kind, +0x10 move mode, +0x14 sub */

/* vtable +0x254: on to the next exit (of the path, or a random one) */
void func_002856A0(Pursuer *p) {
    VObject *rm;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (p->c.unk1388 < p->c.unk1384 && func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1) {
        PU(p, 0x17B0, u8) = VCALL(D_0044E568, 0x3C, u32 (*)(VObject *, u32, s32))(D_0044E568, PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room);
    } else {
        PU(p, 0x17B0, u8) = func_00212A80(p, p->c.door);
    }
    PU(p, 0x15A4, s32) = VCALL(D_0044E568, 0x34, s32 (*)(VObject *, u32, f32 *))(D_0044E568, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
    if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
        u32 node;

        if (func_00284440(p) & 0xFF) {
            return;
        }
        rm = D_0044E568;
        PU(p, 0x17B0, u8) = p->c.door;
        PU(p, 0x15A4, s32) = VCALL(rm, 0x34, s32 (*)(VObject *, u32, f32 *))(rm, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
        VCALL(p, 0xD8, s32 (*)(Pursuer *))(p);
        node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.door) & 0xFFFF;
        p->c.unk148C[node >> 5] |= 1 << (node & 0x1F);
    }
    if (PU(p, 0x1590, f32) < 20.0f && !(PU(p, 0x1590, f32) < 0.0f)) {
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED260);
    VCALL(p, 0x248, void (*)(Pursuer *))(p);
}

/* vtable +0x1AC: line up at the door +0x100 (side by which way it opens) */
void func_0028FA00(Pursuer *p) {
    f32 dir[4] __attribute__((aligned(16)));
    VObject *d = D_0044E558;
    s8 r = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, (u8)p->c.unk100, p->c.a.pos);

    if (r == -1) {
        p->c.unk100 = -1;
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    if ((func_00178980(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
        p->c.unk104[0] = (r != 0 ? 3 : 1) & 0xFFFF;
    } else {
        p->c.unk104[0] = (r != 0 ? 2 : 0) & 0xFFFF;
    }
    PU(p, 0x15C4, s32) = VCALL(d, 0x14, s32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(d, (u8)p->c.unk100, p->c.unk104[0], (f32 *)((u8 *)p + 0x15D0), dir, 1);
    if ((VCALL(p, 0xDC, s32 (*)(Pursuer *))(p) & 0xFF) != 1) {
        p->c.unk100 = -1;
        p->c.unk104[0] = -1;
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    PU(p, 0x1568, f32) = dir[1];
    if (func_00124490(&p->c.a, (f32 *)((u8 *)p + 0x15D0)) < 0x1.99999a0000000p-4f /* 0.1 */) {
        f32 d2 = p->c.a.angle[1] - PU(p, 0x1568, f32);

        if (d2 <= 0.0f) {
            d2 = -d2;
        }
        if (d2 < VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) {
            sceVu0CopyVector(p->c.a.pos, (f32 *)((u8 *)p + 0x15D0));
            p->c.a.angle[1] = PU(p, 0x1568, f32);
        } else {
            VCALL(p, 0x128, void (*)(Pursuer *))(p);
        }
    } else {
        VCALL(p, 0x128, void (*)(Pursuer *))(p);
    }
    Actor_SetState(&p->c.a, &D_003ECE30);
}

/* vtable +0x...: back onto the walk mesh (to the exit or the way in) */
void func_0027FC70(Pursuer *p) {
    f32 h;

    if (p->c.a.navTri == (u32)-1) {
        if (PU(p, 0x17AC, s32) != 4 && p->c.door != 0xFF) {
            if (*(f32 *)&p->c.unk14C4 <= 80.0f && PU(p, 0x17B0, u8) != 0xFF) {
                p->c.a.navTri = VCALL(D_0044E568, 0x28, u32 (*)(VObject *, u32))(D_0044E568, PU(p, 0x17B0, u8));
                h = func_00212730(p, PU(p, 0x17B0, u8));
            } else {
                p->c.a.navTri = VCALL(D_0044E568, 0x28, u32 (*)(VObject *, u32))(D_0044E568, p->c.door);
                h = func_002E2D00(0x1.921fb60000000p+1f /* 3.1415927 */ + func_00212730(p, p->c.door));
            }
            VCALL(p, 0x28, s32 (*)(Pursuer *, u32, f32 *, s32))(p, p->c.a.navTri, &h, 0);
        } else {
            func_00124890(&p->c.a, func_00217340(p));
        }
    } else {
        s32 k = PU(p, 0x17AC, s32);

        if ((k == 5 || k == 0 || k == 1 || k == 3) && *(f32 *)&p->c.unk14C4 <= 80.0f && PU(p, 0x17B0, u8) != 0xFF) {
            p->c.a.navTri = VCALL(D_0044E568, 0x28, u32 (*)(VObject *, u32))(D_0044E568, PU(p, 0x17B0, u8));
            h = func_00212730(p, PU(p, 0x17B0, u8));
            VCALL(p, 0x28, s32 (*)(Pursuer *, u32, f32 *, s32))(p, p->c.a.navTri, &h, 0);
            return;
        }
        if (VCALL(p, 0x28, s32 (*)(Pursuer *, u32, f32 *, s32))(p, p->c.a.navTri, NULL, 0) == -1) {
            func_00124890(&p->c.a, -1);
        }
    }
}

extern u8 D_003AF270[], D_0041B610[], D_00413570[], D_0042E4E0[], D_003D73D0[], D_00414860[],
    D_00422400[], D_004224E0[], D_00444B30[], D_0041A610[], D_00423B90[], D_00419E30[], D_00429790[],
    D_00429820[], D_00429CB0[], D_00429D40[], D_00429DD0[], D_0042A150[], D_0042C960[], D_0042C8D0[],
    D_0042A360[], D_00441870[], D_0042C3E0[], D_0042C700[], D_0042C9F0[], D_0042F4D0[], D_00430880[],
    D_00430910[], D_004309A0[], D_00430A70[], D_0043B610[], D_00443530[];

/* vtable +0xFC: the motion / message files of character id +0x153C (slot 2: vtable +0xFC) */
u8 *func_0029F690(Pursuer *p) {
    if (p->c.a.slot != 2) {
        switch (p->c.unk153C) {
        case 2: return D_003AF270;
        case 3: case 34: case 35: case 36: return D_003D73D0;
        case 4: return D_00414860;
        case 6: return D_0041B610;
        case 7: return D_00413570;
        case 8: return D_00419E30;
        case 9: return D_00422400;
        case 10: return D_004224E0;
        case 11: return D_0041A610;
        case 12: return D_00423B90;
        case 13: return D_00429790;
        case 14: case 15: return D_00429820;
        case 16: return D_00429CB0;
        case 17: return D_00429D40;
        case 18: return D_00429DD0;
        case 19: return D_0042A150;
        case 20: return D_0042C960;
        case 21: case 22: return D_0042C8D0;
        case 23: return D_0042A360;
        case 24: return D_0042C3E0;
        case 25: return D_0042C700;
        case 26: return D_0042C9F0;
        case 27: return D_0042E4E0;
        case 28: return D_0042F4D0;
        case 29: return D_00430880;
        case 30: return D_00430910;
        case 31: return D_004309A0;
        case 32: return D_00430A70;
        case 33: return D_0043B610;
        case 37: return D_00441870;
        case 38: return D_00443530;
        case 39: return D_00444B30;
        default: return NULL;
        }
    }
    return VCALL(p, 0xFC, u8 *(*)(Pursuer *))(p);
}

/* sidestep around the target at the angle +0x104 (5 degree units), back and forth up to 30
 * units, both ways; done when blocked twice or after 120 frames */
void func_00289050(Pursuer *p) {
    s32 flipped = 0;

    for (;;) {
        f32 v[4] __attribute__((aligned(16)));
        f32 a = (f32)p->c.unk104[0] * 0x1.6571860000000p-4f /* 0.08726647 */;
        f32 r = func_00124490(&p->c.a, p->target->a.pos);
        u32 tri;
        s32 ok, clear;

        sceVu0SubVector(v, p->c.a.pos, p->target->a.pos);
        func_002E2CA0(v, v, a);
        sceVu0Normalize(v, v);
        sceVu0ScaleVector(v, v, r);
        sceVu0AddVector(v, v, p->target->a.pos);
        tri = func_00124480(&p->c.a, v, 0);
        ok = tri != (u32)-1 && func_00214A90(p, tri) == 0;
        clear = func_00122C90(&p->c.a, tri, p->target->a.navTri, v, p->target->a.pos, 0) & 0xFF;
        if (!(ok & 0xFF) || !(clear & 0xFF)) {
            if (!(flipped & 0xFF)) {
                PU(p, 0x1634, f32) = 0.0f;
                flipped = 1;
                p->c.unk104[0] = -p->c.unk104[0];
                continue;
            }
            PURSUER_STEP_DONE(p) = 1;
            return;
        }
        func_002143D0(p, v);
        PU(p, 0x1634, f32) += func_00124490(&p->c.a, p->c.a.prevPos);
        if (PU(p, 0x1634, f32) <= 30.0f) {
            if (PU(p, 0x1784, u32) >= 121) {
                PU(p, 0x1634, f32) = 0.0f;
                p->c.unk104[0] = -1;
                PURSUER_STEP_DONE(p) = 1;
            }
            return;
        }
        PU(p, 0x1634, f32) = 0.0f;
        p->c.unk104[0] = -p->c.unk104[0];
        return;
    }
}

/* vtable +0x174: knock at / try the door: wait 1..3 s, a door sound (table +0x16B0 by chance,
 * else 0x21), noise if it's locked */
void func_0027CEF0(Pursuer *p) {
    f32 pos[4] __attribute__((aligned(16)));
    VObject *rnd = D_0044E550;
    VObject *rm = D_0044E568;
    s16 sound;
    u32 exit;
    Progress *pr;

    PU(p, 0x1624, s32) = (s32)(30.0f * (2.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd))) + 60;
    exit = VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF;
    VCALL(rm, 0x30, u32 (*)(VObject *, u32, f32 *))(rm, exit, pos);
    sound = 0x21;
    if (PU(p, 0x16B0, u8 *) != NULL) {
        u8 *t = PU(p, 0x16B0, u8 *);
        f32 roll = 100.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        u32 i;

        for (i = 0;; i = (i + 1) & 0xFF) {
            s32 hit = roll <= AT(t + (i & 0xFF) * 8, 0x4, f32);

            if ((hit ^ 1) == 0) {
                break;
            }
        }
        sound = AT(t + (i & 0xFF) * 8, 0x0, s16);
    }
    if (VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
        func_00122C20(&p->c.a, sound, 7, 0, 0, pos);
    }
    pr = gProgress;
    if (func_00177BF0(pr, exit, 0) & 0xFF & 4) {
        func_00178070(pr, *(u8 *)&p->c.a.slot, 1, 6, 0, 2, 5.0f);
    }
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED5B0);
    VCALL(p, 0x178, void (*)(Pursuer *))(p);
}

/* vtable +0x...: walk, then play the animation +0x104 (-1: done) */
void func_00291EE0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (p->c.unk104[0] < 0) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, p->c.unk104[0]);
    p->c.unk104[0] = -1;
    PURSUER_STEP_NEXT(p) = 1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECD70);
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_00125A10(&p->c);
    }
}

/* vtable +0x...: pace (stance 0 chasing: 1.0..1.2, searching 0.6, else 1.0; 2.0 with
 * progress flag 9), then the exits along the way */
void func_0027D5B0(Pursuer *p) {
    Progress *pr;

    if (PU(p, 0x1664, s32) != 0 || PU(p, 0x17B4, s32) != 0) {
        return;
    }
    if (PU(p, 0x16C8, u8) == 4) {
        s32 room = p->c.a.room;
        s32 *e = VCALL(p, 0x314, s32 *(*)(Pursuer *))(p);

        for (; *e != -1; e += 2) {
            if (room == *e) {
                return;
            }
        }
    }
    pr = gProgress;
    if (Progress_TestFlag(pr, 9) == 0) {
        switch (PU(p, 0x16C8, u8)) {
        case 0:
            func_001272B0(&p->c, 1.0f + 0x1.99999a0000000p-3f /* 0.2 */ * VCALL(D_0044E550, 0x18, f32 (*)(VObject *))(D_0044E550));
            break;
        case 2:
        case 3:
            func_001272B0(&p->c, 0x1.3333340000000p-1f /* 0.6 */);
            break;
        case 1:
        case 4:
            func_001272B0(&p->c, 1.0f);
            break;
        }
    } else {
        func_001272B0(&p->c, 2.0f);
    }
    if (!(p->c.unk128 < p->c.unk124)) {
        s32 room = VCALL(D_0044E568, 0x18, s32 (*)(VObject *, s32, u32))(D_0044E568, p->c.a.room, PU(p, 0x17B0, u8));

        if (room == VCALL(pr, 0xC, s32 (*)(Progress *))(pr) && (func_00211E00(p, PU(p, 0x17B0, u8)) & 0xFF) == 5 &&
            PU(p, 0x16C8, u8) != 4) {
            PURSUER_STEP_DONE(p) = 1;
            return;
        }
        if (func_0027F0A0(p, PU(p, 0x17B0, u8)) & 0xFF) {
            func_002815E0(p, PU(p, 0x17B0, u8));
        }
    }
}

/* vtable +0x...: walk, then the 0x404 animation, turning to the target */
void func_002895B0(Pursuer *p) {
    f32 h, d;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, 0x404);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED130);
    h = func_001244D0(&p->c.a, p->target->a.pos);
    d = func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    if (d <= 0.0f) {
        d = -d;
    }
    if (d < 0x1.921fb60000000p-1f /* 0.7853982 */) {
        func_00125A10(&p->c);
    }
}

/* vtable +0x16C: plan the way to the goal room (+0x1594), first exit to +0x17B0, its path
 * length to +0x14C4; none: the idle move */
void func_0027D8D0(Pursuer *p) {
    PURSUER_STEP_NEXT(p) = 1;
    p->c.unk124 = p->c.unk128;
    if (p->c.unk1388 >= p->c.unk1384) {
        u32 i;

        PU(p, 0x1598, s32) = -1;
        for (i = 0; i < 13; i++) {
            p->c.unk148C[i] = 0;
        }
        switch (PU(p, 0x16C9, u8)) {
        case 1:
        case 3:
        case 4:
        case 5:
            PU(p, 0x16C9, u8) = 0;
            PU(p, 0x16CB, u8) = 0;
            PU(p, 0x16CA, u8) = 0;
            PU(p, 0x16CC, u8) = 0;
            PU(p, 0x179C, s32) = -1;
            PU(p, 0x16F1, u8) = 0;
            PU(p, 0x16F3, u8) = 0;
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
            break;
        }
        if (!(func_0027CA00(p) & 0xFF)) {
            ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_0045B3A0);
            PU(p, 0x17AC, s32) = (&AT(&D_0045B3A0, 0xC, s32))[0];
            p->c.moveMode = AT(&D_0045B3A0, 0x10, s32);
            p->c.moveSub = AT(&D_0045B3A0, 0x14, s32);
            p->c.unk1530 = 0;
            p->c.unk1538 = 0;
            p->c.unk1534 = 0;
            if (PU(p, 0x16C8, u8) != 2) {
                PU(p, 0x16F4, u8) = 1;
            }
            return;
        }
    } else if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) <= 0) {
        p->c.unk1384 = p->c.unk1388;
        PU(p, 0x16F4, u8) = 1;
        return;
    }
    PU(p, 0x17B0, u8) = VCALL(D_0044E568, 0x3C, u32 (*)(VObject *, u32, s32))(D_0044E568, PU(p, 0x138C, u16), p->c.a.room);
    p->c.unk14C0 = PU(p, 0x138C, u16);
    *(f32 *)&p->c.unk14C4 = func_00212060(p, p->c.a.room, PU(p, 0x17B0, u8), p->c.door);
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED580);
    PU(p, 0x1784, s32) = 0;
}

/* vtable +0x...: walk, then play the gesture +0x104 of the table +0x1720 ({anim, wait} 8 bytes) */
void func_00291C80(Pursuer *p) {
    u8 *e;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (p->c.unk104[0] < 0) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    e = PU(p, 0x1720, u8 *) + p->c.unk104[0] * 8;
    Pursuer_PlayAnim(p, AT(e, 0x0, s32));
    if (AT(e, 0x4, u8) != 0) {
        PURSUER_STEP_NEXT(p) = 1;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECD80);
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_00125A10(&p->c);
    }
}

/* ---- batch 9 ---- */

extern const PTMF D_003ED020, D_003ECE90, D_003ECA00, D_003ECA10, D_003ECA20, D_003ED560, D_003ED570,
    D_003ECC30, D_003ECDE0, D_003ECDD0, D_003ED140;
extern PTMF D_0045B358;   /* followed by the wait move: +0xC kind, +0x10 move mode, +0x14 sub */

/* vtable +0x70: placed into room `room` (triangle `tri`, -1: by its exit / way in), on side
 * `side` (0 / 1; else any usable exit) */
s32 func_0029D180(Pursuer *p, s32 room, s32 tri, u32 side) {
    VObject *rm;
    u32 i;

    func_00125BA0(&p->c);
    p->c.a.room = room;
    rm = D_0044E568;
    if (side < 2) {
        for (i = 0; i < 8; i = (i + 1) & 0xFF) {
            if (VCALL(rm, 0x74, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i) != 0 && (func_00211E00(p, i) & 0xFF) != 3 &&
                side == (u32)VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, p->c.a.room, i, 1)) {
                p->c.door = i;
                break;
            }
        }
        if ((i & 0xFF) >= 8) {
            p->c.door = 0;
        }
    } else {
        s32 more;

        i = 0;
        do {
            if (VCALL(rm, 0x74, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i) != 0 && (func_00211E00(p, i) & 0xFF) != 3) {
                p->c.door = i;
            }
            i = (i + 1) & 0xFF;
            more = i < 8;
        } while (more != 0);
        if (more == 0) {
            p->c.door = 0;
        }
    }
    if (PU(p, 0x16C8, u8) == 4) {
        PU(p, 0x16C8, u8) = 3;
        VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
    }
    VCALL(p, 0x7C, void (*)(Pursuer *))(p);
    if (func_00217510(p) != 0) {
        if (tri != -1) {
            VCALL(p, 0x28, s32 (*)(Pursuer *, s32, f32 *, s32))(p, tri, NULL, 0);
        } else {
            func_00124890(&p->c.a, func_00217340(p));
        }
        func_002DDE20(p->c.motion, 0, -1);
        p->c.a.unk2A = 0;
    } else {
        p->c.a.disabled = 1;
        p->c.a.unk2A = 1;
        p->c.a.unk2B = 0;
        p->c.a.unk2D = 0;
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
        p->c.a.navTri = tri;
    }
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    func_0027EAF0(p);
    return 0;
}

extern u8 D_00419DF0[], D_00423B50[], D_00429750[], D_004297E0[], D_00429C70[], D_00429D00[],
    D_00429D90[], D_0042A110[], D_0042C920[], D_0042C890[], D_0042C3A0[], D_0042C6C0[], D_0042C9B0[],
    D_0042F490[], D_00430840[], D_004308D0[], D_00430960[], D_00430A30[], D_0043B5D0[], D_004434F0[];

/* vtable +0xF8: the model files of character id +0x153C (slot 2: vtable +0xF8) */
u8 *func_0029F8C0(Pursuer *p) {
    if (p->c.a.slot != 2) {
        switch (p->c.unk153C) {
        case 2: return func_0012BFB0(p);
        case 3: return func_0020D620(p);
        case 4: return func_002DC460(p);
        case 6: return func_002FC8F0(p);
        case 7: return func_002CF140(p);
        case 8: return D_00419DF0;
        case 9: return func_00309410(p);
        case 10: return func_0030C1B0(p);
        case 11: return func_002F9000(p);
        case 12: return D_00423B50;
        case 13: return D_00429750;
        case 14: case 15: return D_004297E0;
        case 16: return D_00429C70;
        case 17: return D_00429D00;
        case 18: return D_00429D90;
        case 19: return D_0042A110;
        case 20: return D_0042C920;
        case 21: case 22: return D_0042C890;
        case 23: return func_00320150(p);
        case 24: return D_0042C3A0;
        case 25: return D_0042C6C0;
        case 26: return D_0042C9B0;
        case 27: return func_00331200(p);
        case 28: return D_0042F490;
        case 29: return D_00430840;
        case 30: return D_004308D0;
        case 31: return D_00430960;
        case 32: return D_00430A30;
        case 33: return D_0043B5D0;
        case 34: return func_00347290(p);
        case 35: return func_00348620(p);
        case 36: return func_003495B0(p);
        case 37: return func_0034D980(p);
        case 38: return D_004434F0;
        case 39: return func_00365D10(p);
        default: return NULL;
        }
    }
    return VCALL(p, 0xF8, u8 *(*)(Pursuer *))(p);
}

/* the attack's active frames: everyone in its reach (entry +0xC) is hit (+0x1760 bits, the
 * hit reported with the entry's kind +0x10, sound +0x12, +0x4 and strength +0x14); after it
 * the next attack step */
void func_0028B0D0(Pursuer *p) {
    u8 *e = PU(p, 0x171C, u8 *) + PU(p, 0x1724, s8 *)[PU(p, 0x1728, s32) * 4 + PU(p, 0x172C, s8)] * 0x24;

    if (func_001F4770(p->c.motion, 0, -1, 1) & 0xFF & 2) {
        Progress *pr = gProgress;

        if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
            u32 i;

            for (i = 0; i < 3; i = (i + 1) & 0xFF) {
                u32 s = i & 0xFF;
                Character *c = gCharacters[s];

                if (c != NULL && s != (u32)p->c.a.slot && c->a.active != 0 && func_00124490(&p->c.a, c->a.pos) < AT(e, 0xC, f32)) {
                    PU(p, 0x1760, u8) |= (1 << s) & 0xFF;
                }
            }
            if (PU(p, 0x1760, u8) != 0) {
                func_00178070(pr, (u8)p->c.a.slot, PU(p, 0x1760, u8), AT(e, 0x10, u8), AT(e, 0x12, u16), AT(e, 0x4, s16), AT(e, 0x14, f32));
            }
        }
    }
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        if (PU(p, 0x1760, u8) != 0) {
            s32 *t = PU(p, 0x1748, s32 *);

            PU(p, 0x178C, s32) = t != NULL ? t[2] : 30;
        }
        PU(p, 0x172C, s8)++;
        Actor_SetState(&p->c.a, &D_003ED020);
        return;
    }
    if (AT(e, 0x1C, u8) != 0) {
        f32 h = func_001244D0(&p->c.a, p->target->a.pos);

        func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
    func_00125A10(&p->c);
}

/* vtable +0x1C0: pushing at the door +0x100 (held shut from Fiona's side: noise) */
void func_0028ED50(Pursuer *p) {
    VObject *d;
    u32 tri = p->c.a.navTri;
    u8 *e;
    s32 side;

    e = tri < AT(D_0044E570, 0x8, u32) && AT(D_0044E570, 0x4, u8 *) != NULL ? AT(D_0044E570, 0x4, u8 *) + tri * 0x50 : NULL;
    p->c.a.unk2A = (AT(e, 0x3C, u32) & 0x20000) ? 1 : 0;
    d = D_0044E558;
    side = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, (u8)p->c.unk100, gCharPlayer->a.pos);
    if (VCALL(d, 0x30, s32 (*)(VObject *, u32))(d, (u8)p->c.unk100) & 0xFF) {
        if (p->c.moveSub == 0x15) {
            func_00212CA0(p, (u8)p->c.unk100);
        } else {
            func_00212D30(p, (u8)p->c.unk100);
        }
        p->c.moveMode = 0;
        Actor_SetState(&p->c.a, &D_003ECE90);
        VCALL(p, 0x1C4, void (*)(Pursuer *))(p);
        return;
    }
    if (side == 1 && (p->c.unk104[0] == 2 || p->c.unk104[0] == 0)) {
        f32 a = VCALL(d, 0x64, f32 (*)(VObject *, u32))(d, (u8)p->c.unk100);

        if (a > -80.0f && a < -10.0f &&
            ((VCALL(d, 0x6C, s32 (*)(VObject *, s32, u32, f32 *))(d, 2, (u8)p->c.unk100, gCharPlayer->a.pos) & 0xFF) == 1 ||
             (func_00177BF0(gProgress, (u8)p->c.unk100, 0) & 0xFF & 8))) {
            func_00178070(gProgress, *(u8 *)&p->c.a.slot, 1, 5, 0, *(s16 *)&p->c.unk100, 20.0f);
        }
    }
    func_00125A10(&p->c);
}

/* vtable +0x2BC: chase Fiona in the played room (behaviour 0xC), or the wait move elsewhere */
void func_0029AF20(Pursuer *p) {
    if (p->c.a.unkC4 == 2) {
        p->c.a.unkC4 = 0;
        PU(p, 0x1664, s32) = 0;
    }
    if (func_00217510(p) != 0) {
        if (PURSUER_STEP_NEXT(p) == 1 && p->c.unkE0 == 0) {
            if (PU(p, 0x175C, s32) == 0xC) {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA00);
                PU(p, 0x1758, s32) = -1;
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0xC);
            } else {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA10);
                PU(p, 0x1758, s32) = -1;
            }
        }
        PU(p, 0x16C8, u8) = 2;
        VCALL(p, 0x2C0, void (*)(Pursuer *))(p);
        PU(p, 0x1594, s32) = p->c.a.room;
        func_0027E790(p);
    } else {
        p->c.unk1388 = p->c.unk1384;
        ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_0045B358);
        PU(p, 0x17AC, s32) = AT(&D_0045B358, 0xC, s32);
        p->c.moveMode = AT(&D_0045B358, 0x10, s32);
        p->c.moveSub = AT(&D_0045B358, 0x14, s32);
        p->c.unk1530 = 0;
        p->c.unk1538 = 0;
        p->c.unk1534 = 0;
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA20);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x16C8, u8) = 1;
        VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        PURSUER_STEP_NEXT(p) = 1;
    }
    PU(p, 0x16C9, u8) = 0;
    PU(p, 0x16CB, u8) = 0;
    PU(p, 0x16CA, u8) = 0;
    PU(p, 0x16CC, u8) = 0;
    PU(p, 0x179C, s32) = -1;
    PU(p, 0x16F1, u8) = 0;
    PU(p, 0x16F3, u8) = 0;
    PU(p, 0x16F2, u8) = 0;
    PU(p, 0x16F4, u8) = 0;
}

/* vtable +0x168: plan the way on (Fiona's room / the search), path length to the next exit */
void func_0027DB30(Pursuer *p) {
    VObject *rm;
    u32 node;

    PURSUER_STEP_NEXT(p) = 1;
    p->c.unk124 = p->c.unk128;
    if (p->c.unk1388 >= p->c.unk1384 && PU(p, 0x16C8, u8) == 2) {
        u8 k;

        PU(p, 0x1598, s32) = -1;
        k = PU(p, 0x16C9, u8);
        if (k == 2 || k == 4 || k == 1 || k == 5) {
            PU(p, 0x16C9, u8) = 0;
            PU(p, 0x16CB, u8) = 0;
            PU(p, 0x16CA, u8) = 0;
            PU(p, 0x16CC, u8) = 0;
            PU(p, 0x179C, s32) = -1;
            PU(p, 0x16F1, u8) = 0;
            PU(p, 0x16F3, u8) = 0;
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        }
    }
    rm = D_0044E568;
    node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.door) & 0xFFFF;
    if ((func_0027CA00(p) & 0xFF) != 1) {
        u32 i;

        for (i = 0; i < 13; i++) {
            p->c.unk148C[i] = 0;
        }
        if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) == -1) {
            PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C, u16), p->c.a.room);
            func_0029AC50(p);
            return;
        }
        *(f32 *)&p->c.unk14C4 = func_00211F70(p, p->c.a.room, node, PU(p, 0x138C, u16));
        PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C, u16), p->c.a.room);
        ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED570);
    } else {
        *(f32 *)&p->c.unk14C4 = func_00211F70(p, p->c.a.room, node, PU(p, 0x138C, u16));
        PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C, u16), p->c.a.room);
        ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED560);
    }
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    p->c.unk14C0 = PU(p, 0x138C, u16);
    PU(p, 0x1784, s32) = 0;
}

/* vtable +0x270: go for Hewie (behaviour 1 attack / 3 turn first / 7 approach; 30 s limit) */
void func_002953F0(Pursuer *p) {
    s32 prev;

    p->target = gCharPartner;
    PU(p, 0x158C, f32) = func_00213D40(p, gCharPartner);
    PU(p, 0x16C9, u8) = PU(p, 0x16C9, u8) > 0 ? PU(p, 0x16C9, u8) : 1;
    PU(p, 0x16CA, u8) = PU(p, 0x16CA, u8) >= 2 ? PU(p, 0x16CA, u8) : 2;
    prev = PU(p, 0x1758, s32);
    if (prev != -1) {
        if (prev != -2) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, prev);
        }
    } else if (p->c.a.room == p->target->a.room) {
        if (PU(p, 0x158C, f32) < 0.0f) {
            if (!(func_00284440(p) & 0xFF)) {
                VCALL(p, 0x13C, void (*)(Pursuer *))(p);
                return;
            }
        } else if (PU(p, 0x158C, f32) <= VCALL(p, 0x2F0, f32 (*)(Pursuer *))(p) && (func_002175B0(&p->c.a, &gCharPartner->a) & 0xFF)) {
            u32 t = func_00213FA0(p, p->target->a.pos, 0x1.0c15240000000p+0f /* 1.0471976 */, 0x1.4f1a6ep+1f) & 0xFF;

            if (t == 0xFF) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            } else {
                p->c.unk104[0] = t;
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 3);
            }
        } else {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 7);
        }
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 7);
    }
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x162C, s32) = 0;
    PU(p, 0x1630, s32) = PU(p, 0x16D8, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECC30);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x274, void (*)(Pursuer *))(p);
}

/* vtable +0x1A4 / +0x1A0: closing in on the target (stance animation +0x328 / +0x324) */
static void Pursuer_CloseIn(Pursuer *p, s32 skip, u32 slotAnim, const PTMF *next) {
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    VCALL(p, 0xE0, void (*)(Pursuer *, Character *))(p, NULL);
    if (!(PU(p, 0x1590, f32) < 0.0f)) {
        if (PU(p, 0x1788, s32) != skip) {
            Pursuer_PlayAnim(p, VCALL(p, slotAnim, s32 (*)(Pursuer *))(p));
        }
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, next);
        VCALL(p, 0x1A8, void (*)(Pursuer *))(p);
        return;
    }
    VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, p->target);
    if (!(func_00284440(p) & 0xFF)) {
        PU(p, 0x16EF, u8) = 1;
    }
}

void func_002908F0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    Pursuer_CloseIn(p, 0x201, 0x328, &D_003ECDE0);
}

void func_00290B70(Pursuer *p) {
    PU(p, 0x16EC, u8) = 1;
    PURSUER_STEP_NEXT(p) = 1;
    Pursuer_CloseIn(p, 0x200, 0x324, &D_003ECDD0);
}

/* vtable +0x...: sidestep around the target if there's room (8 units) */
void func_00289280(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (func_00217FC0(p, 8.0f) & 0xFF) {
        Character *t;

        if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
            return;
        }
        t = p->target;
        p->c.unk104[0] = func_002E2D00(func_002E2D00(func_001244D0(&t->a, p->c.a.pos) - t->a.angle[1])) <= 0.0f ? -1 : 1;
        PU(p, 0x1634, s32) = 0;
        if (PU(p, 0x1788, s32) != 0x200) {
            Pursuer_PlayAnim(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p));
        }
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, &D_003ED140);
        func_00289050(p);
        return;
    }
    PURSUER_STEP_DONE(p) = 1;
}

/* ---- batch 10 ---- */

#include "effectmgr.h"

extern const PTMF D_003EC930, D_003EC940, D_003ED350, D_003ED4B0, D_003ED4C0, D_003ED4D0, D_003ED4E0,
    D_003ED4F0, D_003ED500, D_003ECEE0, D_003ECB60, D_003ECB70, D_003ED240;
extern PTMF D_0045B340;   /* followed by a move: +0xC kind, +0x10 move mode, +0x14 sub */
extern void *D_00479FF0[];

/* a move from one of the tables at 0x45B340 / 0x45B358 / 0x45B3A0: step function to +0x17A0, kind
 * +0x17AC, move mode and sub */
static void Pursuer_SetMove(Pursuer *p, PTMF *m) {
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), m);
    PU(p, 0x17AC, s32) = AT(m, 0xC, s32);
    p->c.moveMode = AT(m, 0x10, s32);
    p->c.moveSub = AT(m, 0x14, s32);
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
}

/* vtable +0x84: the state block (+0x14E8) of an event: 4 hit, 5 back to normal, 0xC hurt (30),
 * 7 freed */
void func_0029C8C0(Pursuer *p) {
    switch (p->c.state[0]) {
    case 4:
        if (func_00217510(p) != 0) {
            func_0029B8B0(p);
        } else {
            func_0029B5C0(p);
        }
        return;
    case 5:
        if (func_00217510(p) & 0xFF) {
            if (p->c.moveSub != 7) {
                VCALL(p, 0x8C, void (*)(Pursuer *))(p);
                VCALL(p, 0x7C, void (*)(Pursuer *))(p);
            } else {
                s32 a = PU(p, 0x1624, s32);
                s32 b = PU(p, 0x1628, s32);

                PU(p, 0x16ED, u8) = 0;
                PURSUER_STEP_DONE(p) = 0;
                PU(p, 0x16EF, u8) = 0;
                PU(p, 0x16F5, u8) = 0;
                PURSUER_STEP_NEXT(p) = 0;
                PU(p, 0x16EC, u8) = 0;
                PU(p, 0x1710, u8) = 0;
                PU(p, 0x16F9, u8) = 0;
                PU(p, 0x16F6, u8) = 0;
                PU(p, 0x16F8, u8) = 0;
                PU(p, 0x16F7, u8) = 0;
                PU(p, 0x15A0, u8) = 1;
                PU(p, 0x1758, s32) = -1;
                PU(p, 0x1761, u8) = 0;
                PU(p, 0x1760, u8) = 0;
                PU(p, 0x178C, s32) = 0;
                PU(p, 0x1764, s32) = -1;
                p->c.unk14D0 = 0;
                PU(p, 0x17B4, s32) = 0;
                PU(p, 0x1728, s32) = -1;
                PU(p, 0x172C, u8) = 0;
                PU(p, 0x1738, s32) = 0;
                func_0029A6D0(p);
                PU(p, 0x1700, s32) = 0;
                PU(p, 0x1704, s32) = 0;
                PU(p, 0x1708, s32) = 0;
                PU(p, 0x1770, s32) = 0;
                PU(p, 0x1774, s32) = 0;
                PU(p, 0x1778, s32) = 0;
                PU(p, 0x1624, s32) = a;
                PU(p, 0x1628, s32) = b;
                func_00126450(&p->c);
            }
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC930);
            PU(p, 0x1758, s32) = -1;
            p->c.state[0] = 0;
        } else {
            p->c.state[0] = 0;
        }
        p->c.state[1] = 0;
        break;
    case 12:
        if (p->c.state[1] == 30) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC940);
            PU(p, 0x1758, s32) = -1;
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0);
        }
        p->c.state[0] = 0;
        p->c.state[1] = 0;
        break;
    case 7:
        if (p->c.moveMode == 0) {
            p->c.state[0] = 0;
            p->c.state[1] = 0;
        }
        return;
    }
}

/* footsteps heard through the walls: while in another room within 80 units (by path) of Fiona,
 * each time the pursuer has closed in by more than 10, a footstep at its position (left / right
 * by +0x16A4). The loudness was meant to grow from 10 to 15 units of approach, but the step is
 * always taken as 15, so it is always full */
void func_0029D7F0(Pursuer *p) {
    if (func_00217510(p) == 0 && PU(p, 0x17AC, s32) != 4) {
        f32 d;

        PU(p, 0x1538, f32) = PU(p, 0x1534, f32);
        d = func_00126E40(&p->c);
        PU(p, 0x1534, f32) = d;
        if (d <= 80.0f) {
            f32 acc, f;

            if (!(PU(p, 0x1538, f32) <= 0.0f)) {
                PU(p, 0x1530, f32) = PU(p, 0x1530, f32) + (PU(p, 0x1538, f32) - d);
            } else {
                PU(p, 0x1530, f32) = 0.0f;
            }
            acc = PU(p, 0x1530, f32);
            if (!(acc <= 10.0f)) {
                f32 pos[4] __attribute__((aligned(16)));

                if (!(acc <= 10.0f)) {
                    acc = 15.0f;
                }
                f = (acc - 10.0f) / 5.0f;
                PU(p, 0x1530, f32) = PU(p, 0x1530, f32) - acc;
                if (f < 0.0f) {
                    f = 0.0f;
                }
                if (!(f <= 1.0f)) {
                    f = 1.0f;
                }
                if (func_001264D0(&p->c, pos) != 0) {
                    s8 vol = (u8)(u32)(10.0f * f) & 0x7F;
                    s8 vol2 = (u8)(u32)(2.0f * f) & 0x7F;
                    s32 foot = PU(p, 0x16A4, s32) & 1;

                    if (VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
                        func_00122C20(&p->c.a, foot, 7, vol, vol2, pos);
                    }
                    PU(p, 0x16A4, s32)++;
                }
            }
        }
    }
}

/* vtable +0x130: pick from the attack table +0x1718 ({kind, value, chance}) */
void func_00283C50(Pursuer *p) {
    f32 roll = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);
    u8 *e;
    s32 kind;
    u32 i;

    for (i = 0;; i = (i + 1) & 0xFF) {
        e = PU(p, 0x1718, u8 *) + (i & 0xFF) * 0xC;
        if (roll <= AT(e, 0x8, f32)) {
            break;
        }
    }
    kind = AT(e, 0x0, s32);
    switch (kind) {
    case 0x1:
        return;
    case 0x13:
        PU(p, 0x1728, s32) = AT(e, 0x4, s32);
        PU(p, 0x172C, u8) = 0;
        break;
    case 0x18:
    case 0x19:
        p->c.unk104[0] = AT(e, 0x4, s32);
        /* fallthrough */
    case 0x1A:
        VCALL(p, 0x114, void (*)(Pursuer *, s32, u32))(p, kind, i);
        return;
    case 0x2:
    case 0x17:
        p->c.unk104[0] = AT(e, 0x4, s32);
        if ((PU(p, 0x16C8, u8) != 0 || PU(p, 0x1544, u8) == 0) && p->target == gCharPlayer) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32, u32))(p, kind, i);
            return;
        }
        break;
    case 0x14:
    case 0x15:
        if (!(func_001235C0(p, gCharPlayer, i) & 0xFF)) {
            VCALL(p, 0x134, void (*)(Pursuer *))(p);
            return;
        }
        break;
    case 0x9:
    case 0xD:
        VCALL(p, 0x114, void (*)(Pursuer *, s32, u32))(p, kind, i);
        return;
    }
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED350);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, kind);
}

/* vtable +0x154: what next after a move, by the move mode / behaviour / stance */
void func_0027F350(Pursuer *p) {
    if (p->c.moveMode == 4) {
        if (p->c.a.unkC4 == 2 || PU(p, 0x175C, s32) == 0x20 || p->c.moveSub == 0xA) {
            if (PU(p, 0x1664, u32) < 60) {
                PU(p, 0x1664, u32) = 120;
            }
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED4B0);
        } else if (PU(p, 0x175C, s32) == 0x23 && PU(p, 0x1664, u32) != 0) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED4C0);
        } else if (PU(p, 0x16C8, u8) != 4) {
            if (p->c.moveSub != 0x11) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            }
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED4E0);
        } else {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED4D0);
        }
    } else if (PU(p, 0x16C8, u8) == 4) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED4F0);
    } else {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED500);
    }
    PU(p, 0x1758, s32) = -1;
}

/* vtable +0x8C... : into room `room` by event (`found`: chasing Fiona / searching) */
void func_0029CEE0(Pursuer *p, s32 room, u32 found, s32 plan, s32 side) {
    VCALL(p, 0x64, s32 (*)(Pursuer *, s32, s32, s32))(p, room, -1, side);
    func_002DDE20(p->c.motion, 0, -1);
    if ((found & 0xFF) == 1) {
        if (room == gCharPlayer->a.room) {
            PU(p, 0x16C8, u8) = 2;
            PU(p, 0x1594, s32) = room;
        } else {
            PU(p, 0x16C8, u8) = 1;
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            PU(p, 0x16C9, u8) = 2;
            PU(p, 0x16CA, u8) = 3;
        }
    } else {
        PU(p, 0x16C8, u8) = 3;
        VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
        if (plan != 0) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        } else {
            func_0027CA00(p);
        }
    }
    if (p->c.unkE0 == 1) {
        p->c.unkE0 = 0;
    }
    func_0027EAF0(p);
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x16F5, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x16EC, u8) = 0;
    PU(p, 0x1710, u8) = 0;
    PU(p, 0x16F9, u8) = 0;
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x15A0, u8) = 1;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    PU(p, 0x17B4, s32) = 0;
    PU(p, 0x1728, s32) = -1;
    PU(p, 0x172C, u8) = 0;
    PU(p, 0x1738, s32) = 0;
    func_0029A6D0(p);
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    PU(p, 0x1770, s32) = 0;
    PU(p, 0x1774, s32) = 0;
    PU(p, 0x1778, s32) = 0;
    PU(p, 0x1784, s32) = 0;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x1664, s32) = 0;
    PU(p, 0x1790, s32) = 0;
    PU(p, 0x17B4, s32) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
        break;
    case 1:
    case 2:
        VCALL(p, 0x2C0, void (*)(Pursuer *))(p);
        break;
    case 3:
        VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
        break;
    case 4:
        VCALL(p, 0x2C8, void (*)(Pursuer *, s32))(p, 0);
        break;
    }
    if (plan == 0) {
        p->c.door = VCALL(D_0044E568, 0x3C, u32 (*)(VObject *, u32, s32))(D_0044E568, PU(p, 0x138C, u16), p->c.a.room);
    }
}

/* the door it barged through: shove on to a free triangle, then go on (not able: hurt,
 * behaviour 0x20) */
void func_00287380(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));

    func_00125A10(&p->c);
    if (p->c.unk100 == 0xFF) {
        if (MOTION_KEYS(p) & MOTION_KEY_END) {
            func_002E2C10(v, PU(p, 0x1634, f32));
            for (;;) {
                u32 tri = p->c.a.navTri, flags;

                if (tri < AT(D_0044E570, 0x8, u32) && AT(D_0044E570, 0x4, u8 *) != NULL) {
                    flags = AT(AT(D_0044E570, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
                } else {
                    flags = 0;
                }
                if (!(p->c.a.navMask & flags)) {
                    break;
                }
                func_001247E0(&p->c.a, v);
            }
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            p->c.a.unk2B = 0;
            p->c.unk100 = -1;
            PU(p, 0x1634, s32) = 0;
        }
    } else {
        VObject *rm = D_0044E568;

        if (VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.unk100 & 0xFF) != 0 && p->c.a.unk2B != 0) {
            Progress *pr = gProgress;
            u32 st = func_00177BF0(pr, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF;

            if (!(st & 8) && ((st ^ (func_00177BF0(pr, (u8)p->c.unk100, 0) & 0xFF)) & 0x10)) {
                p->c.a.unk2B = 0;
            }
        }
        func_002E2C10(v, PU(p, 0x1634, f32));
        func_0010E640(v, v, 3.0f);
        func_001247E0(&p->c.a, v);
        if (!(Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
            PU(p, 0x1634, s32) = 0;
            p->c.a.unk2B = 0;
            if (!(VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
                if (p->c.hp > 0) {
                    PU(p, 0x1664, s32) = 150;
                } else {
                    VCALL(p, 0x2CC, void (*)(Pursuer *))(p);
                    PU(p, 0x1790, s32) = 0;
                    p->c.a.unkC4 = 2;
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
                }
                func_002815E0(p, (u8)p->c.unk100);
            } else {
                if (p->c.hp <= 0) {
                    p->c.hp = 1;
                }
                PURSUER_STEP_DONE(p) = 1;
                PURSUER_STEP_NEXT(p) = 1;
            }
            p->c.unk100 = -1;
        }
    }
}

/* vtable +0x14C: an event over: the door left, placed by the side it's on, state cleared */
void func_00280210(Pursuer *p, s32 exit) {
    if (p->c.moveMode == 2) {
        Progress *pr = gProgress;

        if ((Progress_CurRoomFlag(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
            if (PU(p, 0x175C, s32) == 0xF) {
                func_00212D30(p, (u8)p->c.unk100);
            } else if ((func_00178980(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
                func_00212CA0(p, (u8)p->c.unk100);
            } else {
                func_00212D30(p, (u8)p->c.unk100);
            }
        }
        p->c.moveMode = 0;
    }
    if (p->c.moveMode == 3) {
        f32 ofs[4] __attribute__((aligned(16)));
        f32 y = p->c.a.pos[1];

        func_001779C0(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot);
        if (!(y <= PU(p, 0x15D4, f32)) || (y == PU(p, 0x15D4, f32) && y < PU(p, 0x15B4, f32))) {
            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, 3, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 0, ofs, p->c.a.pos);
        } else {
            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, 2, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 1, ofs, p->c.a.pos);
        }
    }
    if (p->c.unkE0 == 1) {
        VCALL(p, 0x90, void (*)(Pursuer *))(p);
    }
    PU(p, 0x1624, s32) = 0;
    PU(p, 0x1628, s32) = 0;
    PU(p, 0x162C, s32) = 0;
    PU(p, 0x1630, s32) = 0;
    PU(p, 0x1634, s32) = 0;
    PU(p, 0x1638, s32) = 0;
    PU(p, 0x163C, s32) = 0;
    PU(p, 0x1640, s32) = 0;
    PU(p, 0x1650, s32) = 0;
    PU(p, 0x1654, s32) = 0;
    PU(p, 0x1658, s32) = 0;
    PU(p, 0x165C, s32) = 0;
    p->c.unk104[0] = -1;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    func_001F6E10(p->c.motion);
    PU(p, 0x16F5, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    PU(p, 0x16F3, u8) = 0;
    if (func_00217510(p) != 0) {
        p->c.a.disabled = 1;
        p->c.a.unk2A = 1;
        p->c.a.unk2B = 0;
        p->c.a.unk2D = 0;
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
        p->c.unk124 = p->c.unk128;
    } else if (VCALL(D_0044E568, 0x18, s32 (*)(VObject *, s32, s32))(D_0044E568, gCharPlayer->a.room, exit) == p->c.a.room) {
        p->c.unk14C4 = 0;
        p->c.a.unk2A = 0;
    }
}

/* vtable +0x14: load the files: motions / message (+0x0 / +0x8), slot 2 also its sound banks
 * (+0x10 / +0x14 / +0x18), door sounds (+0xC) and +0x4 */
void func_0029F3E0(Pursuer *p) {
    u8 *f = PU(p, 0x168C, u8 *);
    char *name;

    if (p->c.a.slot != 2) {
        u32 id = p->c.a.flags24 | p->c.a.slot;

        name = AT(f, 0x0, char *);
        if (*name != 0) {
            VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1670, s32), id, 0);
        }
        name = AT(PU(p, 0x168C, u8 *), 0x8, char *);
        if (*name != 0) {
            VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1674, s32), id, 0);
        }
        return;
    }
    name = AT(f, 0x10, char *);
    if (*name != 0) {
        VCALL(D_0044E560, 0x80, void (*)(VObject *, char *, s32, s32, s32))(D_0044E560, name, 7, 2, PU(p, 0x167C, s32));
    }
    name = AT(PU(p, 0x168C, u8 *), 0x14, char *);
    if (*name != 0) {
        VCALL(D_0044E560, 0x80, void (*)(VObject *, char *, s32, s32, s32))(D_0044E560, name, 7, 0, PU(p, 0x1680, s32));
    }
    name = AT(PU(p, 0x168C, u8 *), 0x18, char *);
    if (*name != 0) {
        VCALL(D_0044E560, 0x80, void (*)(VObject *, char *, s32, s32, s32))(D_0044E560, name, 7, 3, PU(p, 0x1684, s32));
    }
    name = AT(PU(p, 0x168C, u8 *), 0x0, char *);
    if (*name != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1670, s32), p->c.a.flags24 | p->c.a.slot, 0);
    }
    name = AT(PU(p, 0x168C, u8 *), 0x8, char *);
    if (*name != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1674, s32), p->c.a.flags24 | p->c.a.slot, 0);
    }
    if (*AT(PU(p, 0x168C, u8 *), 0xC, char *) != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, AT(PU(p, 0x168C, u8 *), 0xC, char *),
                                                                           VCALL(D_0044E558, 0x48, s32 (*)(VObject *, s32))(D_0044E558, 1),
                                                                           p->c.a.flags24 | p->c.a.slot, 0);
    }
    name = AT(PU(p, 0x168C, u8 *), 0x4, char *);
    if (*name != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1678, s32), p->c.a.flags24 | p->c.a.slot, 0);
    }
}

/* may the pursuer use exit `exit`? (clears its "tried" bit +0x148C when it can) */
s32 func_0027F0A0(Pursuer *p, u32 exit) {
    Progress *pr = gProgress;
    VObject *rm;
    u32 node;

    if ((Progress_CurRoomFlag(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 0;
    }
    rm = D_0044E568;
    if (VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) != VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
        switch (func_00211CF0(p, exit) & 0xFF) {
        case 1:
            node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) & 0xFFFF;
            p->c.unk148C[node >> 5] &= ~(1 << (node & 0x1F));
            return 1;
        case 2:
            return 0;
        case 0:
            if (PU(p, 0x17AC, s32) != 4) {
                func_0027CB90(p, exit);
            }
            return 0;
        default:
            return 0;
        }
    }
    switch (func_00211E00(p, exit) & 0xFF) {
    case 4:
        node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) & 0xFFFF;
        p->c.unk148C[node >> 5] &= ~(1 << (node & 0x1F));
        return 1;
    case 3:
        if (PU(p, 0x17AC, s32) != 4) {
            func_0027CB90(p, exit);
        }
        /* fallthrough */
    case 2:
        return 0;
    default:
        rm = D_0044E568;
        node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) & 0xFFFF;
        p->c.unk148C[node >> 5] &= ~(1 << (node & 0x1F));
        p->c.unk100 = VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) & 0xFF;
        pr = gProgress;
        func_00178DB0(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        func_00178750(pr, p->c.a.room, exit);
        return 1;
    }
}

/* vtable +0x1D8: barge through the door +0x100 (animation 0x600..0x603 by side) */
void func_0028DE10(Pursuer *p) {
    f32 dir[4] __attribute__((aligned(16)));
    VObject *d = D_0044E558;
    s32 r = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, (u8)p->c.unk100, p->c.a.pos);
    Progress *pr;

    if (r == -1) {
        p->c.unk100 = -1;
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    pr = gProgress;
    if ((func_00178980(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
        p->c.unk104[0] = (r != 0 ? 3 : 1) & 0xFFFF;
        p->c.moveSub = 0x15;
    } else {
        p->c.unk104[0] = (r != 0 ? 2 : 0) & 0xFFFF;
        p->c.moveSub = 0x14;
    }
    if (gCharPlayer->moveMode != 0 && (func_00177BF0(pr, (u8)p->c.unk100, 0) & 0xFF & 0x20)) {
        return;
    }
    p->c.a.unk2B = 1;
    p->c.a.unk2A = 1;
    if (p->c.moveSub == 0x14) {
        func_00212DE0(p, (u8)p->c.unk100);
    } else if (p->c.moveSub == 0x15) {
        func_00212DC0(p, (u8)p->c.unk100);
    }
    p->c.a.navTri = VCALL(d, 0x14, u32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(d, (u8)p->c.unk100, p->c.unk104[0], p->c.a.pos, dir, 1);
    p->c.a.angle[1] = dir[1];
    sceVu0UnitMatrix(p->c.a.rot);
    sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, dir[1]);
    switch (p->c.unk104[0]) {
    case 0: func_002DDE20(p->c.motion, 0x600, -1); break;
    case 1: func_002DDE20(p->c.motion, 0x601, -1); break;
    case 2: func_002DDE20(p->c.motion, 0x602, -1); break;
    case 3: func_002DDE20(p->c.motion, 0x603, -1); break;
    }
    VCALL(d, 0xC, void (*)(VObject *, u32, s32, s32, s32))(d, (u8)p->c.unk100, p->c.unk104[0], p->c.a.slot, 1);
    Actor_SetState(&p->c.a, &D_003ECEE0);
}

/* vtable +0x7C: back to the move of the stance (behaviour 1), in the room or off-screen */
void func_00298D20(Pursuer *p) {
    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        Pursuer_SetMove(p, &D_0045B340);
        break;
    case 1:
    case 3:
    case 4:
        Pursuer_SetMove(p, &D_0045B358);
        break;
    case 2:
        Pursuer_SetMove(p, &D_0045B3A0);
        break;
    }
    if (p->c.unkE0 == 0) {
        if (func_00217510(p) != 0) {
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
        } else {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECB70);
            PU(p, 0x1758, s32) = -1;
        }
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECB60);
        PU(p, 0x1758, s32) = -1;
    }
}

static void Pursuer_EffectInit(void **obj) {
    *obj = D_00479FF0;
}

/* a special animation (0x1006) with an effect (0x479FF0) for +0x104 */
void func_00285B10(Pursuer *p) {
    u8 *m;
    s32 args[2];
    u8 *mgr;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    Pursuer_PlayAnim(p, 0x1006);
    m = p->c.motion;
    if (!(AT(AT(p->c.motion, 0x874, u8 *) + func_001F4710(m, AT(m, 0x55C, s32)) * 6, 0x4, u16) & 1)) {
        PU(p, 0x1624, s32) = 1;
    } else {
        PU(p, 0x1624, s32) = p->c.unk104[0];
    }
    mgr = D_0044E578;
    PU(p, 0x1628, s32) = Effect_New(mgr, 0x38, Pursuer_EffectInit);
    args[0] = (p->c.unk104[0] - 4) >> 1;
    args[1] = PU(p, 0x1624, s32);
    func_002D6090(mgr, PU(p, 0x1628, s32), args);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED240);
    if (MOTION_KEYS(p) & 0x400) {
        PU(p, 0x1624, s32)--;
        if (PU(p, 0x1624, s32) <= 0) {
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            p->c.unk104[0] = -1;
            PU(p, 0x1628, s32) = 0;
            PU(p, 0x1624, s32) = 0;
        }
    }
    func_00125A10(&p->c);
}
