/* Debilitas: the Pursuer of the first chapters (kind 2, vtable 0x469D10; code 0x1276F0..0x12C1xx).
 * He overrides about thirty of the Pursuer's virtual functions: his attacks, his own way of
 * searching, and his reactions. The object is a plain Pursuer (0x1800 bytes). See pursuer.h.
 * Most of his code is shared with the second Debilitas class (debilitas2.c) and lives in
 * debilitas_body.inc; this file has the rest: his behaviour picker and chase, wandering, the
 * walk choice and his setup. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "ptmf.h"

/* his functions defined further down */

#define DEBILITAS_GIVE_UP_HITS 20
#include "debilitas_body.inc"
#include "debilitas.h"
#include "hewie.h"
#include "model.h"
#include "pursuer_ai.h"
#include "stalker_models.h"
#include "stalker_progress.h"

/* the destructor of the motion player subclass (vtable 0x46F9E0) the stalkers' models use: two
 * embedded parts at +0x10 and +0x1D0 */
extern void *D_0046F9E0[], *D_0046B210[], *D_0046B1C0[], *D_0046ADA0[], *D_00469D00[];

extern u8 D_003AF250[];
extern u8 D_003AF1D0[];
extern u8 D_003AF210[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *func_001278C0(void);
void func_001278D0(void *p, s32 kind, f32 *out);
void func_00127970(void *p, s32 kind, f32 *out);
void *func_00127A20(void);
void func_00127A30(void *p);
s32 func_0012BE50(void);
f32 func_0012BE80(void);
f32 func_0012BEA0(void);
f32 func_0012BEC0(void);
f32 func_0012BED0(void);
f32 func_0012BEE0(void);
f32 func_0012BEF0(void);
f32 func_0012BF00(void);
f32 func_0012BF10(void);
f32 func_0012BF30(void);
f32 func_0012BF40(void);
f32 func_0012BF60(void);
void func_0012BF80(void *p, s32 t);
void func_0012BFA0(void *p);
void *func_0012BFF0(void);

void *func_00127800(void **m, s32 flags) {
    if (m != NULL) {
        m[0] = D_0046F9E0;
        if (m != NULL) {
            void **a = (void **)((u8 *)m + 0x1D0);
            void **b = (void **)((u8 *)m + 0x10);

            m[0] = D_0046B210;
            if (a != NULL) {
                *a = D_0046B1C0;
                if (a != NULL) {
                    *a = D_00469D00;
                }
            }
            if (b != NULL) {
                *b = D_0046ADA0;
                if (b != NULL) {
                    *b = D_00469D00;
                }
            }
        }
        if ((s16)flags > 0) {
            func_002DC6D0(m);
        }
    }
    return m;
}

void *func_001278C0(void) {
    return D_003AF250;
}

void func_001278D0(void *p, s32 kind, f32 *out) {
    f32 z;

    switch (kind) {
    case 0:
        z = -0x1.8d89380000000p+2f /* 6.2115 */;
        break;
    case 1:
        z = 0x1.4ccccc0000000p+3f /* 10.4 */;
        break;
    case 2:
        z = 0x1.9276c80000000p+2f /* 6.2885 */;
        break;
    case 3:
        z = -0x1.dc2f840000000p+2f /* 7.4404 */;
        break;
    default:
        return;
    }
    out[0] = 0.0f;
    FLD(out, 4, s32) = 0;
    out[2] = z;
}

void func_00127970(void *p, s32 kind, f32 *out) {
    f32 x, z;

    switch (kind) {
    case 10:
    case 11:
        x = 0x1.e2f8380000000p-1f /* 0.9433 */;
        z = 0x1.6807600000000p+3f /* 11.2509 */;
        break;
    case 12:
    case 13:
        x = 0x1.1656040000000p+1f /* 2.1745 */;
        z = 0x1.e1573e0000000p+3f /* 15.0419 */;
        break;
    case 14:
        x = -0x1.9c98600000000p+0f /* 1.6117 */;
        z = -0x1.15e00e0000000p+2f /* 4.3418 */;
        break;
    case 15:
        x = 0x1.2a30560000000p-6f /* 0.0182 */;
        z = -0x1.00346e0000000p+0f /* 1.0008 */;
        break;
    default:
        return;
    }
    out[0] = x;
    FLD(out, 4, s32) = 0;
    out[2] = z;
}

void *func_00127A20(void) {
    return D_003AFAE0;
}

void func_00127A30(void *p) {
    FLD(p, 0x16EE, u8) = 1;
}

/* vtable +0xE8 */
s32 func_00128080(Pursuer *p) {
    return 0;
}

/* count the stun +0x14D0 down; at 0 the motion stops being frozen */
void func_00129AF0(Pursuer *p) {
    if (p->c.unk14D0 > 0) {
        p->c.unk14D0--;
        if (p->c.unk14D0 <= 0) {
            func_001F6E10(p->c.motion);
        }
    }
}

/* state: walking his path while looking out (+0x1624 counts down while the walk goes on) */
void func_00128FC0(Pursuer *p) {
    s32 arrived = 0;

    func_002837C0(p, 0xFF);
    if (PU(p, 0x1590, f32) < 0.0f) {
        PU(p, 0x16EF, u8) = 1;
    }
    if ((p->c.unk128 < p->c.unk124) == 1) {
        arrived = func_00214620(p, p->c.unk128) & 0xFF;
    } else if (PU(p, 0x1590, f32) == 0.0f) {
        arrived = 1;
    } else {
        PU(p, 0x16EF, u8) = 1;
    }
    if (arrived != 1 && PU(p, 0x1624, s32) > 0) {
        PU(p, 0x1624, s32)--;
        return;
    }
    PURSUER_STEP_DONE(p) = 1;
}

extern const PTMF D_003AFEF0;

/* vtable +0x264: the next behaviour; his own (func_0012B490) unless +0x16B4 is set or at threat
   level 5, then the Pursuer's */
void func_0012B990(Pursuer *p) {
    s32 next;

    if (PU(p, 0x16B4, u8) != 0 || AT(gProgress, 0x7B8, u8) == 5) {
        func_002961D0(p);
        return;
    }
    next = PU(p, 0x1758, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFEF0);
    PU(p, 0x1758, s32) = -1;
    if (next == -2) {
        PU(p, 0x16F3, u8) = 0;
        PU(p, 0x1758, s32) = -2;
    } else if (PU(p, 0x16F3, u8) == 1) {
        PU(p, 0x16F3, u8) = 0;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
    } else if (next != -1) {
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, next);
    }
    func_0012B490(p);
}

s32 func_0012BE50(void) {
    return 0x2C020068;
}

f32 func_0012BE80(void) {
    return 0x1.3333340000000p-1f /* 0.6 */;
}

f32 func_0012BEA0(void) {
    return 0x1.6666660000000p+0f /* 1.4 */;
}

f32 func_0012BEC0(void) {
    return 24.0f;
}

f32 func_0012BED0(void) {
    return 16.0f;
}

f32 func_0012BEE0(void) {
    return 20.0f;
}

f32 func_0012BEF0(void) {
    return 10.0f;
}

f32 func_0012BF00(void) {
    return 20.0f;
}

f32 func_0012BF10(void) {
    return 0x1.eb851e0000000p-4f /* 0.12 */;
}

f32 func_0012BF30(void) {
    return 60.0f;
}

f32 func_0012BF40(void) {
    return 0x1.1df46a0000000p-3f /* 0.13962634 */; /* 8 degrees in radians */
}

f32 func_0012BF60(void) {
    return 0x1.aceea00000000p-5f /* 0.052359879 */; /* 3 degrees in radians */
}

void func_0012BF80(void *p, s32 t) {
    FLD(p, 0x1660, s32) = t != 0 ? t : 900;
}

void func_0012BFA0(void *p) {
    FLD(p, 0x1660, s32) = 600;
}

void *func_0012BFF0(void) {
    if (FLD(gProgress, 0x30, u32) & 0x8000) {
        return D_003AF210;
    }
    return D_003AF1D0;
}

extern const PTMF D_003AFFB0;

/* state: start the walk of func_00128FC0 (for 60 frames) once the current animation is over */
void func_00129090(Pursuer *p) {
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p), 0);
    PU(p, 0x1624, s32) = 60;
    Actor_SetState(&p->c.a, &D_003AFFB0);
    func_00128FC0(p);
}

/* vtable +0x27C: the Pursuer's frame update (func_00294240), with his walk: in the idle group,
   back to the idle while +0x16B4 is set, else the slow walk (vtable +0x324) when chasing within
   +0x17E8 of Fiona, the normal one (vtable +0x328) otherwise */
void func_0012B860(Pursuer *p) {
    func_00294240(p);
    if (PU(p, 0x175C, s32) != 4 || (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) == 1) {
        return;
    }
    if (PU(p, 0x16B4, u8) == 1) {
        if (PU(p, 0x1788, s32) != 0x201) {
            func_00297B40(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p), 0);
        }
    } else if (PU(p, 0x16C8, u8) == 0 && PU(p, 0x1588, f32) < PU(p, 0x17E8, f32)) {
        if (PU(p, 0x1788, s32) != 0x200) {
            func_00297B40(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p), 0);
        }
    } else if (PU(p, 0x1788, s32) != 0x201) {
        func_00297B40(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p), 0);
    }
}

/* vtable +0x128: the walk for his mode: the slow walk (vtable +0x324) when chasing with the path
   short enough (+0x17E8) and Fiona not hiding (move mode 3), or searching in sub-states 0/2;
   the normal walk (vtable +0x328) otherwise */
void func_00128210(Pursuer *p) {
    s32 slow;

    switch (PU(p, 0x16C8, u8)) {
    case 0:
        slow = PU(p, 0x1590, f32) <= PU(p, 0x17E8, f32) && !(PU(p, 0x1588, f32) < 0.0f)
            && gCharPlayer->moveMode != 3;
        break;
    case 1:
    case 2:
    case 4:
        slow = 0;
        break;
    case 3:
        slow = PU(p, 0x16C9, u8) == 0 || PU(p, 0x16C9, u8) == 2;
        break;
    default:
        return;
    }
    if (slow) {
        func_00297B40(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p), 0);
    } else {
        func_00297B40(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p), 0);
    }
}

extern PTMF D_003AF2D0;
extern u8 D_003AF340[], D_003AF4A0[], D_003AF4D0[], D_003AF6F0[], D_003AF730[], D_003AFA90[],
    D_003AFAF0[], D_003AFB10[], D_003AFB58[], D_003AFB70[], D_0047A900[];

/* vtable +0xF4: his setup over the Pursuer's (func_0029FB20): his tables, and his stats, which
   are different when gProgress+0x30 bit 0x8000 is set */
void func_0012C030(Pursuer *p) {
    s32 alt;

    func_0029FB20(p);
    alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    p->c.hpMax = alt ? 110 : 70;
    PU(p, 0x171C, u8 *) = D_003AF4D0;
    PU(p, 0x1730, u8 *) = D_003AFA90;
    PU(p, 0x1740, u8 *) = D_003AFAF0;
    PU(p, 0x173C, u8 *) = D_003AFB10;
    PU(p, 0x1748, u8 *) = D_003AFB58;
    PU(p, 0x17F0, u8 *) = D_003AFB70;
    PU(p, 0x16DC, s32) = 20;              /* Hewie bite tolerance */
    PU(p, 0x16E8, f32) = 10.0f;
    PU(p, 0x16D4, s32) = alt ? 540 : 300; /* frames */
    PU(p, 0x16D8, s32) = 1800;
    PU(p, 0x16D0, s32) = alt ? 1350 : 1800;
    PU(p, 0x16E0, s32) = 4500;
    PU(p, 0x16E4, s32) = 90;
    PU(p, 0x17E4, f32) = alt ? 40.0f : 50.0f;
    PU(p, 0x17E8, f32) = 80.0f;           /* slow walk when the path to Fiona is shorter */
    p->c.a.radius = 5.0f;
    p->c.a.height = 20.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 0;
    PU(p, 0x16B4, u8) = 0;
    PU(p, 0x1714, PTMF *) = &D_003AF2D0;
    PU(p, 0x1720, u8 *) = D_003AF6F0;
    PU(p, 0x1724, u8 *) = D_003AF730;
    PU(p, 0x16AC, u8 *) = D_003AF340;
    PU(p, 0x16B0, u8 *) = D_003AF4A0;
    PU(p, 0x1734, u8 *) = D_0047A900;
    PU(p, 0x17EC, s32) = 0;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
}

/* a nav triangle's flags (+0x3C), 0 for none */
static inline u32 Debilitas_TriFlags(u32 tri) {
    u8 *tris = AT(gNavMesh, 0x4, u8 *);

    return tri < AT(gNavMesh, 0x8, u32) && tris != NULL ? AT(tris + tri * 0x50, 0x3C, u32) : 0;
}

extern const PTMF D_003AFFA0;

/* start of a wander: try as many random nav triangles as there are for one he may walk on, on
   the same floor level as his (flags 0x300000; not both), at least 20 units away; then walk
   there (state func_00128FC0, at most 60 frames). None: give up (+0x16EF) */
void func_001291C0(Pursuer *p) {
    void *nav = gNavMesh;
    u32 n;
    u32 i;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    n = AT(nav, 0x8, u32);
    for (i = 0; i < n; i++) {
        f32 pos[4] __attribute__((aligned(16)));
        u32 tri = 0;
        u32 flags;

        if ((s32)(n - 1) > 0) {
            tri = (s32)((f32)(s32)n * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom));
        }
        flags = Debilitas_TriFlags(tri);
        if ((flags & p->c.a.navMask) != 0) {
            continue;
        }
        if ((Debilitas_TriFlags(p->c.a.navTri) & 0x300000) != (flags & 0x300000)) {
            continue;
        }
        if ((flags & 0x100000) && (flags & 0x200000)) {
            continue;
        }
        VCALL(nav, 0xC, void (*)(void *, u32, f32 *))(nav, tri, pos);
        if (func_00214B90(p, tri, pos) < 20.0f) {
            continue;
        }
        PU(p, 0x15A4, u32) = tri;
        VCALL(nav, 0xC, void (*)(void *, u32, f32 *))(nav, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0));
        VCALL(p, 0xD8, s32 (*)(Pursuer *))(p);
        break;
    }
    if (i >= n) {
        PU(p, 0x16EF, u8) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003AFFA0);
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p), 0);
    PU(p, 0x1624, s32) = 60;
    Actor_SetState(&p->c.a, &D_003AFFB0);
    func_00128FC0(p);
}

extern const PTMF D_003AFF00, D_003AFF10, D_003AFF20;

/* his behaviour: Fiona as the target. Out of sight of her, head for her (vtable +0xB0). Otherwise
   the pending action (0x1C rumbles the pad), or by a 0..100 roll against his table +0x17F0:
   whether she faces him (within 90 degrees of her heading, his sight range) and whether he's
   within +0x17E4 of her picks action 1 (close and seen: no wait), or 5 / 6 with the wait +0x162C
   from the table. Then his chase step (func_0012A4C0). */
void func_0012B490(Pursuer *p) {
    s32 next;

    if (PU(p, 0x16B4, u8) == 1) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF00);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x264, void (*)(Pursuer *))(p);
        return;
    }
    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    if (PU(p, 0x1544, u8) == 0) {
        VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF10);
        PU(p, 0x1758, s32) = -1;
        return;
    }
    next = PU(p, 0x1758, s32);
    if (next == -1) {
        f32 roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        Character *t = p->target;
        u8 *tbl;

        if (!(func_00218300(p, &t->a, &p->c.a, t->a.angle[1], PU(p, 0x1580, f32), 0x1.921fb6p+0f /* 90 degrees */) & 0xFF)) {
            if (!(PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32))) {
                if (roll <= AT(PU(p, 0x17F0, u8 *), 0x24, f32)) {
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
                    PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x0, s32);
                } else {
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
                    PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x4, s32);
                }
            } else if (roll <= (f32)AT(PU(p, 0x17F0, u8 *), 0x8, u32)) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x8, s32);
            } else {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0xC, s32);
            }
        } else if (!(PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32))) {
            tbl = PU(p, 0x17F0, u8 *);
            if (roll <= AT(tbl, 0x2C, f32)) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x10, s32);
            } else {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x14, s32);
            }
        } else {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PU(p, 0x162C, s32) = 0;
        }
    } else if (next != -2) {
        if (next == 0x1C) {
            VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 3, 0x80, 0x1E);
        }
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, PU(p, 0x1758, s32));
    }
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x1630, s32) = AT(PU(p, 0x17F0, u8 *), 0x20, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF20);
    PU(p, 0x1758, s32) = -1;
    func_0012A4C0(p);
}

/* does Fiona (the target) face him: within 90 degrees of her heading and his sight range */
static inline s32 Debilitas_Seen(Pursuer *p) {
    Character *t = p->target;

    return func_00218300(p, &t->a, &p->c.a, t->a.angle[1], PU(p, 0x1580, f32), 0x1.921fb6p+0f /* 90 degrees */) & 0xFF;
}

/* back off (action 5) or hold off (6) by his table +0x17F0 entry `i` (0 unseen and far, 1 unseen
   and close, 2 seen): chance in percent at +0x24 + 4i, waits at +8i / +8i+4 */
static void Debilitas_BackOrHold(Pursuer *p, f32 roll, s32 i) {
    if (roll <= AT(PU(p, 0x17F0, u8 *), 0x24 + i * 4, f32)) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
        PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), i * 8, s32);
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
        PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), i * 8 + 4, s32);
    }
}

extern f32 D_003AF4B0[];

/* a waited-out back-off or hold-off: strike by the chance for the threat level (func_00297290 of
   D_003AF4B0), else back or hold off again. 1 when he strikes */
static s32 Debilitas_StrikeOrWait(Pursuer *p) {
    u32 chance = func_00297290(p, D_003AF4B0, 3);
    f32 roll;

    if (100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < (f32)chance) {
        VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0xA);
        func_00283C50(p);
        return 1;
    }
    roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
    if (!Debilitas_Seen(p)) {
        Debilitas_BackOrHold(p, roll, PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32) ? 1 : 0);
    } else {
        Debilitas_BackOrHold(p, roll, 2);
    }
    return 0;
}

extern const PTMF D_003AFF30, D_003AFF40, D_003AFF50;

/* his chase (from func_0012B490): as the Pursuer's func_00295670 he stalks Fiona, closing in
   (1), backing off (5) or holding off (6) for the waits from his table +0x17F0, and now and then
   lunging (attack table 0xA) by the threat-level chance. Seen by her while close, he comes
   straight on (1) a limited number of times (+0x1630). Right up against her with no room
   around, he steps in (0x1D); close and with her standing still, grabs (0x1000) */
void func_0012A4C0(Pursuer *p) {
    f32 near;

    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    switch (PU(p, 0x175C, s32)) {
    case 0x1A:
    case 0x19:
        if ((PU(p, 0x175C, s32) == 0x1A || AT(PU(p, 0x1720, u8 *) + p->c.unk104[0] * 8, 0x4, u8) != 0) &&
            !(PU(p, 0x1588, f32) <= 100.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
            PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x1C, s32);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16EF, u8) = 0;
        }
        if (PURSUER_STEP_DONE(p) == 1 || PU(p, 0x16EF, u8) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16EF, u8) = 0;
        }
        break;
    case 0x10:
    case 0x12:
    case 0xB:
    case 3:
    case 0x1C:
        if (PU(p, 0x16EF, u8) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0x1000:
        PU(p, 0x1544, u8) = 0;
        if (PU(p, 0x16EF, u8) == 1) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF30);
            PU(p, 0x1758, s32) = -1;
            PU(p, 0x16EF, u8) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0x1D:
        if (!(PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32)) ||
            (func_00217ED0(p, 0x1.921fb6p+1f /* 180 degrees */, 2.0f) == 0 &&
             func_00217ED0(p, 0x1.eb7c16p+0f /* 110 degrees */, 2.0f) == 0 &&
             func_00217ED0(p, -0x1.eb7c16p+0f, 2.0f) == 0)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
        }
        if (PU(p, 0x1588, f32) < 10.0f && !(PU(p, 0x1588, f32) <= 0.0f) && gCharPlayer->moveMode == 0 &&
            gCharPlayer->moveSub != 0 && (PursuerGroup_Find(gProgress, 4, 0) & 0xFF) == 0xFF) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1000);
            PU(p, 0x162C, s32) = 0;
        }
        break;
    case 1: {
        f32 roll;

        /* every 90 frames, a lunge by the threat-level chance */
        if ((PU(p, 0x1780, u32) + 1) % 90 == 0) {
            u32 chance = func_00297290(p, D_003AF4B0, 3);

            if (100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= (f32)chance) {
                VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0xA);
                func_00283C50(p);
                return;
            }
        }
        if (!((((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF))) {
            u32 dir = func_00213FA0(p, gCharPlayer->a.pos, 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;

            if (dir != 0xFF) {
                p->c.unk104[0] = dir;
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 3);
            }
        }
        roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        if (PU(p, 0x1588, f32) < 0.0f) {
            if (func_00284440(p) & 0xFF) {
                break;
            }
            func_0029AF20(p);
            return;
        }
        if (!Debilitas_Seen(p)) {
            Debilitas_BackOrHold(p, roll, 1);
        } else if (!(PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32) + 20.0f)) {
            Debilitas_BackOrHold(p, roll, 2);
        } else if (PU(p, 0x1630, s32) > 0) {
            PU(p, 0x1630, s32)--;
        } else {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
            PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x18, s32);
        }
        break;
    }
    case 6:
        if (PU(p, 0x16EF, u8) != 0) {
            PU(p, 0x16EF, u8) = 0;
        }
        if (PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32) && Debilitas_Seen(p) == 1) {
            if (PU(p, 0x1630, s32) > 0) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
                PU(p, 0x1630, s32) = AT(PU(p, 0x17F0, u8 *), 0x20, s32);
                PU(p, 0x162C, s32) = 0;
            } else {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x18, s32);
            }
            break;
        }
        if (PU(p, 0x162C, s32) > 0 || !(PU(p, 0x1588, f32) < 100.0f)) {
            PU(p, 0x162C, s32)--;
            break;
        }
        if (Debilitas_StrikeOrWait(p)) {
            return;
        }
        break;
    case 5:
        if (PU(p, 0x16EF, u8) != 0) {
            PU(p, 0x16EF, u8) = 0;
        }
        if (PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32) && Debilitas_Seen(p) == 1 && PU(p, 0x1630, s32) > 0) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PU(p, 0x1630, s32) = AT(PU(p, 0x17F0, u8 *), 0x20, s32);
            PU(p, 0x162C, s32) = 0;
            break;
        }
        if (PU(p, 0x162C, s32) > 0) {
            if (PU(p, 0x1588, f32) <= 100.0f) {
                PU(p, 0x162C, s32)--;
            } else {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x1C, s32);
            }
            break;
        }
        if (Debilitas_StrikeOrWait(p)) {
            return;
        }
        break;
    }
    if (PURSUER_STEP_NEXT(p) == 1 && PU(p, 0x175C, s32) != 0x12) {
        s32 d = func_002131A0();

        if (d != -1) {
            /* Fiona is hiding: go to the door */
            p->c.unk100 = d;
            p->c.unk104[0] = -1;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
        }
    }
    if (p->c.moveSub == 6 && gCharPlayer->moveMode != 3 && !(PU(p, 0x1588, f32) <= 0.0f)) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    if (PURSUER_STEP_NEXT(p) != 1) {
        return;
    }
    if (PU(p, 0x1544, u8) == 0) {
        VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF40);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x162C, s32) = 0;
        return;
    }
    if (AT(gProgress, 0x7B8, u8) == 5) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF50);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x162C, s32) = 0;
        return;
    }
    near = p->target->moveMode == 3 ? func_00124490(&p->c.a, p->target->a.pos) : PU(p, 0x1588, f32);
    if (!(near < VCALL(p, 0x2F4, f32 (*)(Pursuer *))(p)) || near < 0.0f) {
        return;
    }
    if (near < func_002838E0(p) || near < 10.0f) {
        f32 a;

        if (!(func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
            a = func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        } else {
            a = -func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        }
        if (a < 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f &&
            func_002175B0(&p->c.a, &p->target->a) != 0) {
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, (s8)func_00283EF0(p));
            func_00283C50(p);
            PU(p, 0x162C, s32) = 0;
        }
    }
}
