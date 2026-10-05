/* Debilitas: the Pursuer of the first chapters (kind 2, vtable 0x469D10; code 0x1276F0..0x12C1xx).
 * He overrides about thirty of the Pursuer's virtual functions: his attacks, his own way of
 * searching, and his reactions. The object is a plain Pursuer (0x1800 bytes). See pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"

extern void *D_00469D10[];

/* his functions defined further down */
void func_00128A20(Pursuer *p);
void func_0012B490(Pursuer *p);

/* vtable +0x8: destructor */
Pursuer *func_001276F0(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_00469D10;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* the destructor of the motion player subclass (vtable 0x46F9E0) the stalkers' models use: two
 * embedded parts at +0x10 and +0x1D0 */
extern void *D_0046F9E0[], *D_0046B210[], *D_0046B1C0[], *D_0046ADA0[], *D_00469D00[];
extern void func_002DC6D0(void *p);   /* operator delete */

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

/* vtable +0x328: his walk animation (0x203 while resting) */
s32 func_00127CC0(Pursuer *p) {
    if (PU(p, 0x16C8, u8) == 4) {
        return 0x203;
    }
    return func_00297A70(p);
}

/* vtable +0x11C: does he give up and go elsewhere? Never with less than 20 hits taken (+0x16C4),
 * nor while resting or held by the story flag */
s32 func_00127FC0(Pursuer *p) {
    if (PU(p, 0x16C4, s32) < 20) {
        return 0;
    }
    if (PU(p, 0x16C8, u8) == 4) {
        return 0;
    }
    if ((func_00177AB0(gProgress, 1, p->c.a.slot) & 0xFF) != 0xFF) {
        return 0;
    }
    if (VCALL(p, 0xE8, s32 (*)(Pursuer *))(p) & 0xFF) {
        return 1;
    }
    func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    return 0;
}

/* vtable +0xE8 */
s32 func_00128080(Pursuer *p) {
    return 0;
}

/* vtable +0x228 / +0x224: the Pursuer's */
void func_00129550(Pursuer *p) {
    func_0028BD40(p);
}

void func_00129560(Pursuer *p) {
    func_0028BEF0(p);
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

/* vtable +0x310 / +0x30C */
s32 func_0012BE60(Pursuer *p) {
    return 0xE;
}

s32 func_0012BE70(Pursuer *p) {
    return 0xD;
}

/* a stun: the flinch (0x1004) unless already reeling, and frozen while it lasts */
void func_00128970(Pursuer *p) {
    if (PU(p, 0x1788, s32) != 0x1000 && p->c.unk14D0 <= 0) {
        func_00297B40(p, 0x1004, 1);
    }
    if (((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0)) {
        PU(p, 0x1770, s32) = 0;
        PU(p, 0x1774, s32) = 0;
        PU(p, 0x1778, s32) = 0;
        PU(p, 0x16F7, u8) = 0;
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else if (p->c.unk14D0 <= 0) {
        func_00125A10(&p->c);
    }
}

extern const PTMF D_003AFF80, D_003B0010;

/* vtable +0x2B4: a fresh start of his behaviour (vtable +0x2B8 picks it) */
void func_00129D10(Pursuer *p) {
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF80);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x2B8, void (*)(Pursuer *))(p);
}

/* state: an animation that, at threat level 5 (gProgress+0x7B8), leads straight into attack 8;
   otherwise its end ends the step */
void func_001286F0(Pursuer *p) {
    func_00125A10(&p->c);
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        if (AT(gProgress, 0x7B8, u8) != 5) {
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            return;
        }
        PU(p, 0x1728, s32) = 8;
        Actor_SetState(&p->c.a, &D_003B0010);
        p->c.moveMode = 8;
        func_0028B970(p);
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

extern const PTMF D_003AFFD0;

/* state: turning to Fiona; when the animation ends, the next part (func_00128A20) */
void func_00128CA0(Pursuer *p) {
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        func_00297B40(p, 0x205, 0);
        p->c.moveSub = 0x1C;
        Actor_SetState(&p->c.a, &D_003AFFD0);
        func_00128A20(p);
        return;
    }
    func_00125A10(&p->c);
    {
        Character *t = gCharPlayer != NULL ? gCharPlayer : p->target;
        f32 h = func_001244D0(&p->c.a, t->a.pos);

        func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
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

/* vtable +0xB0: head for Fiona (the Pursuer's func_00219100 without the door test); resting,
   func_0029B190 instead */
void func_0012BBF0(Pursuer *p) {
    Character *f;
    s32 side;

    if (p->c.unkE0 == 0 && PU(p, 0x16C8, u8) == 4) {
        func_0029B190(p);
        return;
    }
    side = PU(p, 0x1598, s32);
    f = gCharPlayer;
    if (func_00217370(p, f) != 0) {
        side = func_002172F0(p, f);
    } else {
        if (p->c.a.room == f->a.room || PU(p, 0x1594, s32) != f->a.room) {
            side = -1;
        }
        PU(p, 0x15A4, s32) = func_00216E00(p, f->a.navTri, f->a.pos, (f32 *)((u8 *)p + 0x15B0));
    }
    if (func_00126F80(&p->c, f->a.room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = f->a.room;
        PU(p, 0x1598, s32) = side;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}
