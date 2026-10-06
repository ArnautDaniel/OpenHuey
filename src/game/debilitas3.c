/* The third Debilitas class (kind 7, vtable D_0046F020; code 0x2CD7A0..0x2CF3xx): a Pursuer
 * with Debilitas's model and its own behaviour, used where the countdown runs (its reset gives
 * +0x16DC 0x10 while progress +0x1FBEC1 is set, pursuer.c func_002CF0F0). See debilitas.c. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"

extern u8 D_004137B0[], D_00414220[], D_00414270[], D_00413E10[], D_00413E60[], D_00413E80[], D_00413EC8[];
extern u8 D_004139D0[], D_00413A10[], D_00413640[], D_004137A0[], D_0047AC08[];
extern const PTMF D_004135D0;

/* vtable +0xF4: his setup over the Pursuer's (func_0029FB20): his tables and stats (165 hp and
 * +0x16E8 18 when gProgress+0x30 bit 0x8000 is set, else 110 hp and 12) */
void func_002CF1C0(Pursuer *p) {
    func_0029FB20(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 165;
        PU(p, 0x171C, u8 *) = D_004137B0;
        PU(p, 0x1730, u8 *) = D_00414220;
        PU(p, 0x1740, u8 *) = D_00414270;
        PU(p, 0x173C, u8 *) = D_00413E80;
        PU(p, 0x1748, u8 *) = D_00413EC8;
        PU(p, 0x16DC, s32) = 40;              /* Hewie bite tolerance */
        PU(p, 0x16E8, f32) = 18.0f;
    } else {
        p->c.hpMax = 110;
        PU(p, 0x171C, u8 *) = D_004137B0;
        PU(p, 0x1730, u8 *) = D_00413E10;
        PU(p, 0x1740, u8 *) = D_00413E60;
        PU(p, 0x173C, u8 *) = D_00413E80;
        PU(p, 0x1748, u8 *) = D_00413EC8;
        PU(p, 0x16DC, s32) = 40;
        PU(p, 0x16E8, f32) = 12.0f;
    }
    PU(p, 0x16D4, s32) = 300;   /* frames */
    PU(p, 0x16D8, s32) = 1800;
    PU(p, 0x16D0, s32) = 1350;
    PU(p, 0x16E0, s32) = 9000;
    PU(p, 0x16E4, s32) = 150;
    p->c.a.radius = 5.0f;
    p->c.a.height = 20.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 0;
    PU(p, 0x16B4, u8) = 2;
    PU(p, 0x1714, const PTMF *) = &D_004135D0;
    PU(p, 0x1720, u8 *) = D_004139D0;
    PU(p, 0x1724, u8 *) = D_00413A10;
    PU(p, 0x16AC, u8 *) = D_00413640;
    PU(p, 0x16B0, u8 *) = D_004137A0;
    PU(p, 0x1734, u8 *) = D_0047AC08;
    PU(p, 0x17EC, s32) = 0;
    PU(p, 0x1694, f32) = 6.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 13.0f;
    PU(p, 0x16A0, s32) = 0;
}

/* vtable +0xB4: head for character `c` (NULL: the target) - Debilitas's func_0012BAA0 without
 * the resting check */
void func_002CEC40(Pursuer *p, Character *c) {
    s32 side;

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

/* vtable +0xB0: head for Fiona - Debilitas's func_0012BBF0 without the resting check */
void func_002CED60(Pursuer *p) {
    Character *f = gCharPlayer;
    s32 side = PU(p, 0x1598, s32);

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

/* vtable +0xAC: head for triangle `tri` at `pos` in `room` (-1 the current one) - as Debilitas's
 * func_0012BD10 */
void func_002CEE50(Pursuer *p, u32 tri, const f32 *pos, s32 room) {
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

/* a stun: the flinch (0x1004) unless already reeling, and frozen while it lasts - as Debilitas's
 * func_00128970 */
void func_002CE050(Pursuer *p) {
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

extern const PTMF D_004142E0;

/* the end of his lunge animation: at threat level 5 (gProgress+0x7B8) it leads straight into
 * attack 8 (state `attack`); otherwise it ends the step */
static inline void Debilitas3_LungeEnd(Pursuer *p, const PTMF *attack) {
    func_00125A10(&p->c);
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        if (AT(gProgress, 0x7B8, u8) != 5) {
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            return;
        }
        PU(p, 0x1728, s32) = 8;
        Actor_SetState(&p->c.a, attack);
        p->c.moveMode = 8;
        func_0028B970(p);
    }
}

/* state: the lunge animation (see Debilitas3_LungeEnd) */
void func_002CDDD0(Pursuer *p) {
    Debilitas3_LungeEnd(p, &D_004142E0);
}

extern const PTMF D_004142D0;

/* start of the lunge: finish the current walk, then animation 0x1306 in state func_002CDDD0 */
void func_002CDE90(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0x1306, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_004142D0);
    Debilitas3_LungeEnd(p, &D_004142E0);
}

extern u8 D_00413A60[], D_00413AB0[], D_00413AF0[], D_00413B50[], D_00413B90[], D_00413BD0[],
    D_00413BF0[], D_00413C30[], D_00413C70[], D_00413CB0[], D_00413CC0[], D_00413D10[],
    D_00413D30[], D_00413D70[], D_00413DC0[], D_00413DE8[], D_00413DF8[];
extern u8 D_00413EE0[], D_00413F40[], D_00413F90[], D_00413FE0[], D_00414010[], D_00414040[],
    D_00414050[], D_00414090[], D_004140B0[], D_004140F0[], D_00414100[], D_00414140[],
    D_00414160[], D_004141A0[], D_004141E0[], D_004141F8[], D_00414208[];

/* his attack tables for each situation 0..16; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTables3[2][17] = {
    { D_00413A60, D_00413AF0, D_00413AB0, D_00413B50, D_00413B90, D_00413BD0, D_00413BF0,
      D_00413C30, D_00413C70, D_00413CB0, D_00413CC0, D_00413D10, D_00413D30, D_00413D70,
      D_00413DE8, D_00413DF8, D_00413DC0 },
    { D_00413EE0, D_00413F90, D_00413F40, D_00413FE0, D_00414010, D_00414040, D_00414050,
      D_00414090, D_004140B0, D_004140F0, D_00414100, D_00414140, D_00414160, D_004141A0,
      D_004141F8, D_00414208, D_004141E0 },
};

/* vtable +0x130: the attack table for a situation */
void func_002CD7B0(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = (u32)situation < 17 ? sAttackTables3[alt][situation] : sAttackTables3[alt][0];
}

/* vtable +0x228 / +0x224: the Pursuer's */
void func_002CE6A0(Pursuer *p) {
    func_0028BD40(p);
}

void func_002CE6B0(Pursuer *p) {
    func_0028BEF0(p);
}

/* a grab at Fiona: at the animation's hit key, once, if she's within reach (gProgress vtable
 * +0x2C), it lands (func_00178070 kind 1); over when the animation ends, she's out of sight, or
 * 60 units away - as Debilitas's */
static inline void Debilitas3_Grab(Pursuer *p) {
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

/* state: the grab (see Debilitas3_Grab) */
void func_002CDA70(Pursuer *p) {
    Debilitas3_Grab(p);
}

extern const PTMF D_004142F0;

/* start of the grab: finish the current walk, then animation 0xE06 in state func_002CDA70 */
void func_002CDBA0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0xE06, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_004142F0);
    Debilitas3_Grab(p);
}

void func_002CE100(Pursuer *p);
extern const PTMF D_004142A0;

/* turning to Fiona; when the animation ends, the blow (state `st`, run at once: `blow`) */
static inline void Debilitas3_TurnToFiona(Pursuer *p, const PTMF *st, void (*blow)(Pursuer *)) {
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        func_00297B40(p, 0x205, 0);
        p->c.moveSub = 0x1C;
        Actor_SetState(&p->c.a, st);
        blow(p);
        return;
    }
    func_00125A10(&p->c);
    {
        Character *t = gCharPlayer != NULL ? gCharPlayer : p->target;
        f32 h = func_001244D0(&p->c.a, t->a.pos);

        func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
}

/* state: turning to Fiona (see Debilitas3_TurnToFiona) */
void func_002CE380(Pursuer *p) {
    Debilitas3_TurnToFiona(p, &D_004142A0, func_002CE100);
}

extern const PTMF D_00414290;

/* start of the turn to Fiona: finish the current walk, then animation 0x1304 in state
 * func_002CE380 */
void func_002CE490(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0x1304, 0);
    PU(p, 0x16F7, u8) = 1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_00414290);
    Debilitas3_TurnToFiona(p, &D_004142A0, func_002CE100);
}

extern const PTMF D_004142B0, D_004142C0;

/* state: his blow, after the turn to Fiona - as Debilitas's func_00128A20: the hit point is the
 * bone of the attack entry (+0x171C, +0x94); off the nav mesh it misses (flinch, state
 * D_004142B0), otherwise on reaching Fiona or Hewie it lands, then the flinch and state D_004142C0 */
void func_002CE100(Pursuer *p) {
    u8 *e = PU(p, 0x171C, u8 *) + 0x90;
    f32 pos[4] __attribute__((aligned(16)));
    u32 hit;

    sceVu0CopyVector(pos, func_0017CE80(MOTION_AT(p, 0x810, u8 *), AT(e, 0x4, s32)) + 0xC);
    if (func_00124480(&p->c.a, pos, p->c.a.navMask & ~0x40) == (u32)-1) {
        func_00297B40(p, 0x1004, (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) != 0);
        p->c.moveSub = 0;
        Actor_SetState(&p->c.a, &D_004142B0);
        return;
    }
    sceVu0CopyVector((f32 *)((u8 *)p + 0x1770), pos);
    hit = func_00217B90(p, AT(e, 0x4, s32), AT(e, 0xC, f32)) & 0xFF & ~PU(p, 0x1760, u8);
    hit = (hit | (func_00217920(p) & 0xFF & ~PU(p, 0x1760, u8) & 0xFF)) & 0xFF;
    if (hit == 0) {
        func_00125A10(&p->c);
        return;
    }
    {
        f32 roll = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);
        s16 stun = roll < AT(e, 0x18, f32) ? 0x8000 : 0;

        func_00178070(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
    }
    PU(p, 0x1764, s32) = 12;
    if ((((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) == 0) {
        func_00297B40(p, 0x1004, 0);
    }
    p->c.moveSub = 0;
    Actor_SetState(&p->c.a, &D_004142C0);
}

extern const f32 D_00414300[4];

/* vtable +0x1A8: the chase towards the target - as Debilitas's func_00129570: on the path he cuts
 * straight for the target once he's no more than 10 units further from it than the path's end */
void func_002CE6C0(Pursuer *p) {
    Character *t;

    if (PU(p, 0x1590, f32) < 0.0f && p->c.a.room == p->target->a.room) {
        VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, p->target);
        if (p->c.a.room == p->target->a.room || !(func_00284440(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        return;
    }
    if ((p->c.unk128 < p->c.unk124) != 1) {
        return;
    }
    t = p->target;
    if (func_00214A90(p, t->a.navTri) == 0) {
        u32 tri = p->target->a.navTri;

        if (func_00124480(&p->c.a, p->target->a.pos, -1) == tri) {
            func_002143D0(p, p->target->a.pos);
            return;
        }
    } else {
        s32 n = p->c.unk124;
        f32 end[4] __attribute__((aligned(16)));
        f32 d[4] __attribute__((aligned(16)));
        u32 tri;
        f32 left;

        end[0] = D_00414300[0];
        end[1] = D_00414300[1];
        end[2] = D_00414300[2];
        end[3] = D_00414300[3];
        end[0] = AT(p, 0x124 + n * 0xC, f32);
        end[2] = AT(p, 0x128 + p->c.unk124 * 0xC, f32);
        tri = func_00124480(&p->c.a, end, p->c.a.navMask);
        if (tri == AT(p, 0x120 + p->c.unk124 * 0xC, u32)) {
            VCALL(D_0044E570, 0x14, void (*)(void *, u32, f32 *))(D_0044E570, tri, end);
            sceVu0SubVector(d, p->target->a.pos, end);
            d[3] = 0.0f;
            left = __builtin_sqrtf(sceVu0InnerProduct(d, d));
            if (func_00124490(&p->c.a, p->target->a.pos) - left <= 10.0f) {
                func_002143D0(p, p->target->a.pos);
                return;
            }
        }
    }
    func_00214620(p, p->c.unk128);
}

/* vtable +0x30: his frame update - as Debilitas's func_001297C0 (full think on screen, with a
 * growl every 90 idle frames; off screen the behaviour step and the off-screen move) */
void func_002CE910(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        func_00218110(p);
        func_0029B4B0(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        PU(p, 0x17EC, u32)++;
        if (PU(p, 0x17EC, u32) >= 90) {
            PU(p, 0x17EC, u32) = 0;
            if (p->c.moveMode == 0 && PU(p, 0x1788, s32) != 0x1300 && PU(p, 0x1788, s32) != 0x1600) {
                func_0029D410(p, 0x2A, 7, 0, 0, NULL);
            }
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
        if (p->c.unk14D0 <= 0 || p->c.unk14D0 == 5) {
            func_0029D4C0(p, -1);
        }
        func_00213E30(p);
        func_0029E210(p);
    } else {
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        func_0029D7F0(p);
    }
    Stalker_ThinkEnd(p);
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern const PTMF D_0042F1E0;
extern const PTMF D_0042F1C0;
extern const PTMF D_0042F1A0;
extern const PTMF D_0042F1B0;
extern const f32 D_0042F1F0[4];

extern u8 D_0042E9D0[], D_0042EA00[], D_0042EA30[], D_0042EA70[], D_0042EAA0[], D_0042EAE0[], D_0042EB10[],
    D_0042EB40[], D_0042EB70[], D_0042EBB0[], D_0042EBC0[], D_0042EC00[], D_0042EC20[], D_0042EC50[],
    D_0042ECA0[], D_0042ECC8[], D_0042ECD8[], D_0042EDC0[], D_0042EE10[], D_0042EE60[], D_0042EEB0[],
    D_0042EEE0[], D_0042EF10[], D_0042EF20[], D_0042EF60[], D_0042EF80[], D_0042EFC8[], D_0042EFE0[],
    D_0042F020[], D_0042F040[], D_0042F080[], D_0042F0C0[], D_0042F0E8[], D_0042F0F8[], D_00474FD0[];

/* the D_00474FD0 Debilitas's attack tables for each situation 0..16; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTablesD[2][17] = {
    { D_0042E9D0, D_0042EA30, D_0042EA00, D_0042EA70, D_0042EAA0, D_0042EAE0, D_0042EB10,
      D_0042EB40, D_0042EB70, D_0042EBB0, D_0042EBC0, D_0042EC00, D_0042EC20, D_0042EC50,
      D_0042ECC8, D_0042ECD8, D_0042ECA0 },
    { D_0042EDC0, D_0042EE60, D_0042EE10, D_0042EEB0, D_0042EEE0, D_0042EF10, D_0042EF20,
      D_0042EF60, D_0042EF80, D_0042EFC8, D_0042EFE0, D_0042F020, D_0042F040, D_0042F080,
      D_0042F0E8, D_0042F0F8, D_0042F0C0 },
};

/* vtable +0x130: the attack table for a situation (as func_002CD7B0) */
void func_0032F880(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = (u32)situation < 17 ? sAttackTablesD[alt][situation] : sAttackTablesD[alt][0];
}

/* (as func_002CDA70)  state: the grab (see Debilitas3_Grab) */
void func_0032FB40(Pursuer *p) {
    Debilitas3_Grab(p);
}

/* (as func_002CDBA0)  start of the grab: finish the current walk, then animation 0xE06 in state func_002CDA70 */
void func_0032FC70(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0xE06, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_0042F1E0);
    Debilitas3_Grab(p);
}

extern const PTMF D_0042F1D0;

/* state: the lunge animation (as func_002CDDD0) */
void func_0032FEA0(Pursuer *p) {
    Debilitas3_LungeEnd(p, &D_0042F1D0);
}

void func_003301D0(Pursuer *p);
extern const PTMF D_0042F190, D_0042F180;

/* state: turning to Fiona (as func_002CE380) */
void func_00330450(Pursuer *p) {
    Debilitas3_TurnToFiona(p, &D_0042F190, func_003301D0);
}

/* start of the turn to Fiona (as func_002CE490) */
void func_00330560(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0x1304, 0);
    PU(p, 0x16F7, u8) = 1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_0042F180);
    Debilitas3_TurnToFiona(p, &D_0042F190, func_003301D0);
}

/* (as func_002CDE90)  start of the lunge: finish the current walk, then animation 0x1306 in state func_002CDDD0 */
void func_0032FF60(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0x1306, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_0042F1C0);
    Debilitas3_LungeEnd(p, &D_0042F1D0);
}

/* (as func_002CE050)  a stun: the flinch (0x1004) unless already reeling, and frozen while it lasts - as Debilitas's
 * func_00128970 */
void func_00330120(Pursuer *p) {
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

/* (as func_002CE100)  state: his blow, after the turn to Fiona - as Debilitas's func_00128A20: the hit point is the
 * bone of the attack entry (+0x171C, +0x94); off the nav mesh it misses (flinch, state
 * D_0042F1A0), otherwise on reaching Fiona or Hewie it lands, then the flinch and state D_0042F1B0 */
void func_003301D0(Pursuer *p) {
    u8 *e = PU(p, 0x171C, u8 *) + 0x90;
    f32 pos[4] __attribute__((aligned(16)));
    u32 hit;

    sceVu0CopyVector(pos, func_0017CE80(MOTION_AT(p, 0x810, u8 *), AT(e, 0x4, s32)) + 0xC);
    if (func_00124480(&p->c.a, pos, p->c.a.navMask & ~0x40) == (u32)-1) {
        func_00297B40(p, 0x1004, (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) != 0);
        p->c.moveSub = 0;
        Actor_SetState(&p->c.a, &D_0042F1A0);
        return;
    }
    sceVu0CopyVector((f32 *)((u8 *)p + 0x1770), pos);
    hit = func_00217B90(p, AT(e, 0x4, s32), AT(e, 0xC, f32)) & 0xFF & ~PU(p, 0x1760, u8);
    hit = (hit | (func_00217920(p) & 0xFF & ~PU(p, 0x1760, u8) & 0xFF)) & 0xFF;
    if (hit == 0) {
        func_00125A10(&p->c);
        return;
    }
    {
        f32 roll = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);
        s16 stun = roll < AT(e, 0x18, f32) ? 0x8000 : 0;

        func_00178070(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
    }
    PU(p, 0x1764, s32) = 12;
    if ((((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) == 0) {
        func_00297B40(p, 0x1004, 0);
    }
    p->c.moveSub = 0;
    Actor_SetState(&p->c.a, &D_0042F1B0);
}

/* (as func_002CE6C0)  vtable +0x1A8: the chase towards the target - as Debilitas's func_00129570: on the path he cuts
 * straight for the target once he's no more than 10 units further from it than the path's end */
void func_00330790(Pursuer *p) {
    Character *t;

    if (PU(p, 0x1590, f32) < 0.0f && p->c.a.room == p->target->a.room) {
        VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, p->target);
        if (p->c.a.room == p->target->a.room || !(func_00284440(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        return;
    }
    if ((p->c.unk128 < p->c.unk124) != 1) {
        return;
    }
    t = p->target;
    if (func_00214A90(p, t->a.navTri) == 0) {
        u32 tri = p->target->a.navTri;

        if (func_00124480(&p->c.a, p->target->a.pos, -1) == tri) {
            func_002143D0(p, p->target->a.pos);
            return;
        }
    } else {
        s32 n = p->c.unk124;
        f32 end[4] __attribute__((aligned(16)));
        f32 d[4] __attribute__((aligned(16)));
        u32 tri;
        f32 left;

        end[0] = D_0042F1F0[0];
        end[1] = D_0042F1F0[1];
        end[2] = D_0042F1F0[2];
        end[3] = D_0042F1F0[3];
        end[0] = AT(p, 0x124 + n * 0xC, f32);
        end[2] = AT(p, 0x128 + p->c.unk124 * 0xC, f32);
        tri = func_00124480(&p->c.a, end, p->c.a.navMask);
        if (tri == AT(p, 0x120 + p->c.unk124 * 0xC, u32)) {
            VCALL(D_0044E570, 0x14, void (*)(void *, u32, f32 *))(D_0044E570, tri, end);
            sceVu0SubVector(d, p->target->a.pos, end);
            d[3] = 0.0f;
            left = __builtin_sqrtf(sceVu0InnerProduct(d, d));
            if (func_00124490(&p->c.a, p->target->a.pos) - left <= 10.0f) {
                func_002143D0(p, p->target->a.pos);
                return;
            }
        }
    }
    func_00214620(p, p->c.unk128);
}

/* (as func_002CE910)  vtable +0x30: his frame update - as Debilitas's func_001297C0 (full think on screen, with a
 * growl every 90 idle frames; off screen the behaviour step and the off-screen move) */
void func_003309E0(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        func_00218110(p);
        func_0029B4B0(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        PU(p, 0x17EC, u32)++;
        if (PU(p, 0x17EC, u32) >= 90) {
            PU(p, 0x17EC, u32) = 0;
            if (p->c.moveMode == 0 && PU(p, 0x1788, s32) != 0x1300 && PU(p, 0x1788, s32) != 0x1600) {
                func_0029D410(p, 0x2A, 7, 0, 0, NULL);
            }
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
        if (p->c.unk14D0 <= 0 || p->c.unk14D0 == 5) {
            func_0029D4C0(p, -1);
        }
        func_00213E30(p);
        func_0029E210(p);
    } else {
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        func_0029D7F0(p);
    }
    Stalker_ThinkEnd(p);
}

/* (as func_002CEC40)  vtable +0xB4: head for character `c` (NULL: the target) - Debilitas's func_0012BAA0 without
 * the resting check */
void func_00330D50(Pursuer *p, Character *c) {
    s32 side;

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

/* (as func_002CED60)  vtable +0xB0: head for Fiona - Debilitas's func_0012BBF0 without the resting check */
void func_00330E70(Pursuer *p) {
    Character *f = gCharPlayer;
    s32 side = PU(p, 0x1598, s32);

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

/* (as func_002CEE50)  vtable +0xAC: head for triangle `tri` at `pos` in `room` (-1 the current one) - as Debilitas's
 * func_0012BD10 */
void func_00330F60(Pursuer *p, u32 tri, const f32 *pos, s32 room) {
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

extern u8 D_0042E720[], D_0042F110[], D_0042F160[], D_0042ED60[], D_0042EDA8[], D_0042ECF0[], D_0042ED40[];
extern u8 D_0042E940[], D_0042E980[], D_0042E5B0[], D_0042E710[], D_0047ADC0[];
extern const PTMF D_0042E540;

/* (as func_002CF1C0) the D_00474FD0 Debilitas's vtable +0xF4: its setup over the Pursuer's -
 * its tables and stats (165 hp and +0x16E8 18 when gProgress+0x30 bit 0x8000 is set, else 110
 * hp and 12; Hewie bite tolerance 30) */
void func_00331280(Pursuer *p) {
    func_0029FB20(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 165;
        PU(p, 0x171C, u8 *) = D_0042E720;
        PU(p, 0x1730, u8 *) = D_0042F110;
        PU(p, 0x1740, u8 *) = D_0042F160;
        PU(p, 0x173C, u8 *) = D_0042ED60;
        PU(p, 0x1748, u8 *) = D_0042EDA8;
        PU(p, 0x16DC, s32) = 30;
        PU(p, 0x16E8, f32) = 18.0f;
    } else {
        p->c.hpMax = 110;
        PU(p, 0x171C, u8 *) = D_0042E720;
        PU(p, 0x1730, u8 *) = D_0042ECF0;
        PU(p, 0x1740, u8 *) = D_0042ED40;
        PU(p, 0x173C, u8 *) = D_0042ED60;
        PU(p, 0x1748, u8 *) = D_0042EDA8;
        PU(p, 0x16DC, s32) = 30;
        PU(p, 0x16E8, f32) = 12.0f;
    }
    PU(p, 0x16D4, s32) = 300;
    PU(p, 0x16D8, s32) = 1800;
    PU(p, 0x16D0, s32) = 1350;
    PU(p, 0x16E0, s32) = 9000;
    PU(p, 0x16E4, s32) = 150;
    p->c.a.radius = 5.0f;
    p->c.a.height = 20.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 0;
    PU(p, 0x16B4, u8) = 3;
    PU(p, 0x1714, const PTMF *) = &D_0042E540;
    PU(p, 0x1720, u8 *) = D_0042E940;
    PU(p, 0x1724, u8 *) = D_0042E980;
    PU(p, 0x16AC, u8 *) = D_0042E5B0;
    PU(p, 0x16B0, u8 *) = D_0042E710;
    PU(p, 0x1734, u8 *) = D_0047ADC0;
    PU(p, 0x17EC, s32) = 0;
    PU(p, 0x1694, f32) = 6.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 13.0f;
    PU(p, 0x16A0, s32) = 0;
}
