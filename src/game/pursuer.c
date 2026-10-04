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

extern void func_00125960(Character *c);

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

extern void func_00125D40(Character *c);

/* vtable +0x58: deactivate (Character part) */
void func_0029E600(Pursuer *p) {
    func_00125D40(&p->c);
}

extern void func_00127650(Character *c);

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

extern void func_00125A10(Character *c);   /* base step: walk the animation */
extern void func_00126910(Character *c);   /* disable (base) */
extern void func_00126810(Character *c);   /* enable (base) */
extern s32 func_00122B50(Actor *a, f32 *out);
extern void func_001264C0(Character *c, s32 kind, f32 *pos);
extern s32 func_00125AD0(Character *c);
extern f32 func_002E2D00(f32 angle);       /* wrap an angle into -pi..pi */
extern f32 *func_0017CE80(u8 *skel, s32 bone);
extern void func_00178070(Progress *pr, u32 slot, s32 a2, s32 a3, s32 a4, s32 a5, f32 f);
extern void func_002815E0(Pursuer *p, u32 door);
extern void func_00212CA0(Pursuer *p, u32 door);
extern s32 func_00212A80(Pursuer *p, s32 a1);
extern s32 func_00217510(Pursuer *p);
extern void func_002177D0(Pursuer *p);
extern void func_00297160(Pursuer *p);

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

extern void func_00100490(void *obj);
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

extern const PTMF D_003ED0D0, D_003ED050, D_003ED0E0, D_003ED060, D_003ED530;

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
