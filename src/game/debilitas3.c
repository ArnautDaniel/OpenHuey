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
