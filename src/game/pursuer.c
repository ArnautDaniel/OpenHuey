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

extern f32 func_001244D0(Actor *a, const f32 *pos);       /* heading towards a point */
extern void func_002DDD20(void *motion, s32 anim, s32 variant);
extern void func_002DDE20(void *motion, s32 anim, s32 variant);
extern void func_001F6E10(void *motion);
extern void func_002EFA50(void *threat, f32 amount);
extern u32 func_00177BF0(Progress *pr, u32 i, u32 slot);
extern void func_00122C20(Actor *a, s32 sound, s32 a2, s32 a3, s32 a4, void *a5);
extern void func_00124530(Actor *a, f32 target, f32 step);
extern void func_00125BE0(Character *c);
extern void func_00125CC0(Character *c);
extern f32 func_002140A0(Pursuer *p, f32 heading, f32 step);
extern void func_00213270(Pursuer *p, u32 a1);
extern void func_00214620(Pursuer *p, s32 node);
extern void func_002143D0(Pursuer *p, f32 *pos);
extern s32 func_00284440(Pursuer *p);
extern f32 func_00212550(Pursuer *p, u32 door);
extern void func_00297B40(Pursuer *p, s32 anim, s32 a2);
extern void func_00297C60(Pursuer *p);
extern s32 func_00217920(Pursuer *p);
extern u32 func_00124480(Actor *a, const f32 *target, u32 mask);

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
