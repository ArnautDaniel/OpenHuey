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

/* vtable +0xAC: head for triangle `tri` at `pos` in `room` (-1 the current one) */
void func_0012BD10(Pursuer *p, u32 tri, const f32 *pos, s32 room) {
    void *nm;

    if (room == -1) {
        room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    }
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        PU(p, 0x15A4, u32) = func_00216E00(p, tri, pos, (f32 *)((u8 *)p + 0x15B0));
    }
    nm = D_0044E570;
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, const f32 *))(nm, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0)) != 3) {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0));
    }
    if (func_00126F80(&p->c, room, -1, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = -1;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* vtable +0xB4: head for character `c` (NULL: the target); resting, func_0029AF20 instead */
void func_0012BAA0(Pursuer *p, Character *c) {
    s32 side;

    if (p->c.unkE0 == 0 && PU(p, 0x16C8, u8) == 4) {
        func_0029AF20(p);
        return;
    }
    if (c == NULL) {
        c = p->target;
    }
    side = PU(p, 0x1598, s32);
    if (func_00217370(p, c) != 0) {
        side = func_002172F0(p, c);
    } else {
        s32 room = c->a.room;

        if (p->c.a.room == room || PU(p, 0x1594, s32) != room) {
            side = -1;
        }
        if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
            PU(p, 0x15A4, s32) = func_00216E00(p, c->a.navTri, c->a.pos, (f32 *)((u8 *)p + 0x15B0));
        }
    }
    if (func_00126F80(&p->c, c->a.room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = c->a.room;
        PU(p, 0x1598, s32) = side;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
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

/* state: a grab at Fiona: at the animation's hit key, once, if she's within reach (gProgress
   vtable +0x2C), it lands (func_00178070 kind 1); over when the animation ends, she's out of
   sight, or 60 units away */
void func_00128390(Pursuer *p) {
    func_00125A10(&p->c);
    if ((func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 2) && !(PU(p, 0x1760, u8) & 1)) {
        Progress *pr = gProgress;

        if (VCALL(pr, 0x2C, s32 (*)(Progress *, u32, s32, s32, f32))(pr, *(u8 *)&p->c.a.slot, 0x1E, 0, 5.0f) != 0) {
            func_00178070(pr, *(u8 *)&p->c.a.slot, 1, 6, 0, 3, 10.0f);
            PU(p, 0x1760, u8) |= 1;
        }
    }
    if ((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) || PU(p, 0x1544, u8) == 0 ||
        !(PU(p, 0x1590, f32) < 60.0f) || PU(p, 0x1590, f32) < 0.0f) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

extern const PTMF D_003AFF60;

/* vtable +0x2AC: go for Fiona: the pending action (+0x1758; -1 action 4), then rest mode with her
   as the target, and pick the behaviour (vtable +0x2B0) */
void func_0012A390(Pursuer *p) {
    s32 next = PU(p, 0x1758, s32);

    if (next == -1) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
    } else if (next != -2) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, next);
    }
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x16F7, u8) = 1;
    p->target = gCharPlayer;
    PU(p, 0x16C9, u8) = 6;
    PU(p, 0x16CA, u8) = 7;
    if (PU(p, 0x16C8, u8) != 4) {
        PU(p, 0x16C8, u8) = 4;
        VCALL(p, 0x2C8, void (*)(Pursuer *, s32))(p, 0);
    }
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF60);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x2B0, void (*)(Pursuer *))(p);
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
