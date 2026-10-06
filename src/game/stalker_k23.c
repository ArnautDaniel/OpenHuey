/* The pursuer kind 23 class (vtable D_004738A0): a stalker drawn with a screen tint that fades
 * in while it is on screen (+0x17C4 delay, +0x17C8 shown, +0x17CC alpha, +0x17D0 fade state:
 * 1 in, 2 out, 3 hold). Its other methods are in src/leaf (b5_00315C50.c, small_hand.c,
 * tiny_gen.c, creature_gen.c). */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "texcache.h"

extern VObject *gCamera;   /* the camera */
extern void func_001267F0(Character *c, s32 v);

/* vtable +0xE4: at a door it breaks (func_00178980), outside the ending (gProgress+0x1FBEC1),
   while opening or attacking it: use and damage it and change room through it (vtable +0x28) */
void func_0031F3E0(Pursuer *p, s32 exit) {
    Progress *pr = gProgress;

    if (!(func_00178980(pr, p->c.a.room, exit) & 0xFF) || AT(pr, 0x1FBEC1, u8) != 0) {
        return;
    }
    switch (PU(p, 0x175C, s32)) {
    case 0xC:
    case 0x29:
    case 0xD:
        pr = gProgress;
        func_00178DB0(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        func_00178A90(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        VCALL(p, 0x28, void (*)(Pursuer *, s32, s32, s32))(p, VCALL(gRooms, 0x28, s32 (*)(VObject *, s32))(gRooms, exit), 0, 0);
        break;
    }
}

/* vtable +0xF0: in the ending, use the door and close it behind (func_00178C10) */
void func_0031F4E0(Pursuer *p, s32 exit) {
    Progress *pr = gProgress;

    if (AT(pr, 0x1FBEC1, u8) != 0) {
        func_00178DB0(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        func_00178C10(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    }
}

/* the tint fade: in ramps the alpha down by 4 from 130 to 112, out ramps it to 0 and hides it;
   the renderer's tint (+0x64) is grey with that alpha, and off (+0x6C) while hidden */
void func_0031F560(Pursuer *p) {
    VObject *r;

    switch (PU(p, 0x17D0, s8)) {
    case 1:
        if (PU(p, 0x17CC, f32) < 116.0f) {
            PU(p, 0x17CC, f32) = 112.0f;
            PU(p, 0x17D0, s8) = 0;
        } else {
            PU(p, 0x17CC, f32) -= 4.0f;
        }
        break;
    case 3:
        PU(p, 0x17CC, f32) = 130.0f;
        break;
    case 2:
        PU(p, 0x17CC, f32) -= 4.0f;
        if (PU(p, 0x17CC, f32) <= 0.0f) {
            PU(p, 0x17CC, f32) = 0.0f;
            PU(p, 0x17D0, s8) = 0;
            PU(p, 0x17C8, u8) = 0;
        }
        break;
    }
    r = gRenderer;
    VCALL(r, 0x64, void (*)(VObject *, u32, s32))(r, (u8)(u32)PU(p, 0x17CC, f32) << 24 | 0x808080, 0);
    if (!PU(p, 0x17C8, u8)) {
        VCALL(r, 0x6C, void (*)(VObject *))(r);
    }
}

/* show the tint: held (3) while it is in state 2 (+0xC4), else fading in */
static inline void k23_show(Pursuer *p) {
    if (func_00217510(p) != 0) {
        PU(p, 0x17C8, u8) = 1;
        PU(p, 0x17D0, s8) = p->c.a.unkC4 == 2 ? 3 : 1;
    }
}

/* the tint per frame: once the delay +0x17C4 has run out (it counts only while free: not in
   states 1 / 2, not moving 0x11 / 9) it shows on screen; hidden ones fade out off screen */
void func_0031F6E0(Pursuer *p) {
    if (PU(p, 0x17C4, s32) != 0) {
        if (func_00217510(p) != 0) {
            if (!PU(p, 0x17C8, u8) || (p->c.a.unkC4 == 2 && PU(p, 0x17D0, s8) != 3)) {
                k23_show(p);
            } else if (p->c.a.unkC4 != 2 && PU(p, 0x17D0, s8) == 3) {
                k23_show(p);
            }
        }
        if (p->c.a.unkC4 != 1 && p->c.a.unkC4 != 2 && p->c.moveSub != 0x11 && p->c.moveSub != 9) {
            PU(p, 0x17C4, s32)--;
        }
    }
    if (PU(p, 0x17C4, s32) == 0 && func_00217510(p) != 0 && PU(p, 0x17C8, u8) == 1 &&
        PU(p, 0x17D0, s8) != 2 && func_00217510(p) != 0) {
        PU(p, 0x17D0, s8) = 2;
    }
    func_0031F560(p);
}

extern u8 D_0042A7F0[], D_0042A830[], D_0042A860[], D_0042A8A0[], D_0042A8D0[], D_0042A900[],
    D_0042A930[], D_0042A960[], D_0042A990[], D_0042A9C0[], D_0042A9D0[], D_0042AA00[],
    D_0042AA20[], D_0042AA50[], D_0042AA70[], D_0042AAA0[], D_0042AAB0[], D_0042AAC0[];
extern u8 D_0042ABA0[], D_0042ABE0[], D_0042AC20[], D_0042AC70[], D_0042ACA0[], D_0042ACD0[],
    D_0042ACF0[], D_0042AD40[], D_0042AD80[], D_0042ADC0[], D_0042ADD0[], D_0042AE00[],
    D_0042AE20[], D_0042AE50[], D_0042AE80[], D_0042AEA8[], D_0042AEB8[];

/* its attack tables for situations 0..16; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTables[2][17] = {
    { D_0042A7F0, D_0042A860, D_0042A830, D_0042A8A0, D_0042A8D0, D_0042A900, D_0042A930,
      D_0042A960, D_0042A990, D_0042A9C0, D_0042A9D0, D_0042AA00, D_0042AA20, D_0042AA50,
      D_0042AAA0, D_0042AAB0, D_0042AA70 },
    { D_0042ABA0, D_0042AC20, D_0042ABE0, D_0042AC70, D_0042ACA0, D_0042ACD0, D_0042ACF0,
      D_0042AD40, D_0042AD80, D_0042ADC0, D_0042ADD0, D_0042AE00, D_0042AE20, D_0042AE50,
      D_0042AEA8, D_0042AEB8, D_0042AE80 },
};

/* vtable +0x130: the attack table for a situation; in the ending situation 15 has its own */
void func_0031F880(Pursuer *p, s8 situation) {
    s32 alt;

    if (AT(gProgress, 0x1FBEC1, u8) != 0 && situation == 15) {
        PU(p, 0x1718, u8 *) = D_0042AAC0;
        return;
    }
    alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    PU(p, 0x1718, u8 *) = sAttackTables[alt][(u32)situation < 17 ? situation : 0];
}

/* vtable +0x200: done at the door in the ending or without a side (+0x104), else the
   Pursuer's */
void func_0031FB80(Pursuer *p) {
    if (AT(gProgress, 0x1FBEC1, u8) != 0 || p->c.unk104[0] == 0) {
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    func_0028D6E0(p);
}

/* vtable +0x30: its frame update: the stalkers' (Stalker_ThinkStart / Stalker_ThinkEnd) with
   the tint (func_0031F6E0) before the model and stance */
void func_0031FCE0(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        func_0029B4B0(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
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
    Stalker_ThinkTimers(p);
    func_0031F6E0(p);
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
    VCALL(p, 0x100, void (*)(Pursuer *))(p);
}

/* vtable +0x5C: the Pursuer's reset with the tint off; in the ending the Hewie bite tolerance
   is 20 */
void func_0031FFA0(Pursuer *p) {
    func_0029E520(p);
    PU(p, 0x17C4, s32) = 0;
    PU(p, 0x17C8, u8) = 0;
    PU(p, 0x17CC, f32) = 0.0f;
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        PU(p, 0x16DC, s32) = 20;
    }
}

/* vtable +0x2C: the draw while the tint shows; on layer 0x11 between the texture cache's
   +0x18 and the camera's matrices (+0x4C / +0x50 / +0x58 / +0x54 through one matrix) and its
   +0x14 after */
void func_00320000(Pursuer *p) {
    f32 mtx[4][4] __attribute__((aligned(16)));
    VObject *cam;
    u8 *m;

    if (p->c.a.disabled != 0 || PU(p, 0x17C8, u8) != 1) {
        return;
    }
    if (p->c.unkE4 == 1 && p->c.unk152C != 0x11) {
        VCALL(p, 0x80, void (*)(Pursuer *))(p);
    }
    if (p->c.unk152C == 0x11) {
        VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
        cam = gCamera;
        VCALL(cam, 0x4C, void (*)(VObject *, f32 (*)[4]))(cam, mtx);
        VCALL(cam, 0x50, void (*)(VObject *, f32 (*)[4]))(cam, mtx);
        VCALL(cam, 0x58, void (*)(VObject *, f32 (*)[4]))(cam, mtx);
        VCALL(cam, 0x54, void (*)(VObject *, f32 (*)[4]))(cam, mtx);
    }
    m = p->c.motion;
    VCALL(m, 0x38, void (*)(void *, s32, u32, s32))(m, p->c.unk152C, p->c.a.navTri, 0);
    if (p->c.unk152C == 0x11) {
        VCALL(gCamera, 0x14, void (*)(VObject *))(gCamera);
    }
}

extern u8 D_0042A380[], D_0042A4D0[], D_0042A4E0[], D_0042A770[], D_0042A7A0[], D_0042AAD0[],
    D_0042AB20[], D_0042AB40[], D_0042AB88[], D_0047AD70[];

/* vtable +0xF4: its setup over the Pursuer's (func_0029FB20): tables and stats (more health
   and a closer reach when gProgress+0x30 bit 0x8000 is set), the tint off, layer 0x11 */
void func_003201D0(Pursuer *p) {
    void *m;

    func_0029FB20(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 125;
        PU(p, 0x171C, u8 *) = D_0042A4E0;
        PU(p, 0x1730, u8 *) = D_0042AAD0;
        PU(p, 0x1740, u8 *) = D_0042AB20;
        PU(p, 0x173C, u8 *) = D_0042AB40;
        PU(p, 0x1748, u8 *) = D_0042AB88;
        PU(p, 0x16DC, s32) = 50;              /* Hewie bite tolerance */
        PU(p, 0x16E8, f32) = 10.0f;
        PU(p, 0x16D4, s32) = 300;
        PU(p, 0x16D8, s32) = 1800;
        PU(p, 0x16D0, s32) = 1200;
        PU(p, 0x16E0, s32) = 5400;
        PU(p, 0x16E4, s32) = 45;
    } else {
        p->c.hpMax = 100;
        PU(p, 0x171C, u8 *) = D_0042A4E0;
        PU(p, 0x1730, u8 *) = D_0042AAD0;
        PU(p, 0x1740, u8 *) = D_0042AB20;
        PU(p, 0x173C, u8 *) = D_0042AB40;
        PU(p, 0x1748, u8 *) = D_0042AB88;
        PU(p, 0x16DC, s32) = 50;
        PU(p, 0x16E8, f32) = 15.0f;
        PU(p, 0x16D4, s32) = 300;
        PU(p, 0x16D8, s32) = 1800;
        PU(p, 0x16D0, s32) = 1200;
        PU(p, 0x16E0, s32) = 5400;
        PU(p, 0x16E4, s32) = 45;
    }
    p->c.a.radius = 3.0f;
    p->c.a.height = 17.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x1720, u8 *) = D_0042A770;
    PU(p, 0x1724, u8 *) = D_0042A7A0;
    PU(p, 0x16AC, u8 *) = D_0042A380;
    PU(p, 0x16B0, u8 *) = D_0042A4D0;
    PU(p, 0x1734, u8 *) = D_0047AD70;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
    m = p->c.motion;
    VCALL(m, 0x2C, void (*)(void *))(m);
    PU(p, 0x17C4, s32) = 0;
    PU(p, 0x17C8, u8) = 0;
    PU(p, 0x17CC, f32) = 0.0f;
    func_001267F0(&p->c, 0x11);
}
