/* The code the two Debilitas classes share (see debilitas.c and debilitas2.c): written with the
 * first class's symbols; debilitas2.c renames them to its own before including this. Each class
 * sets DEBILITAS_GIVE_UP_HITS. */

extern void *D_00469D10[];
void func_00128A20(Pursuer *p);

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

/* vtable +0x328: his walk animation (0x203 while resting) */
s32 func_00127CC0(Pursuer *p) {
    if (PU(p, 0x16C8, u8) == 4) {
        return 0x203;
    }
    return func_00297A70(p);
}

/* vtable +0x11C: does he give up and go elsewhere? Never with fewer than DEBILITAS_GIVE_UP_HITS hits taken (+0x16C4),
 * nor while resting or held by the story flag */
s32 func_00127FC0(Pursuer *p) {
    if (PU(p, 0x16C4, s32) < DEBILITAS_GIVE_UP_HITS) {
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

/* vtable +0x228 / +0x224: the Pursuer's */
void func_00129550(Pursuer *p) {
    func_0028BD40(p);
}

void func_00129560(Pursuer *p) {
    func_0028BEF0(p);
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

/* the end of his lunge animation: at threat level 5 (gProgress+0x7B8) it leads straight into
   attack 8; otherwise it ends the step */
static inline void Debilitas_LungeEnd(Pursuer *p) {
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

/* state: the lunge animation (see Debilitas_LungeEnd) */
void func_001286F0(Pursuer *p) {
    Debilitas_LungeEnd(p);
}

extern const PTMF D_003B0000;

/* start of the lunge: finish the current walk, then animation 0x1306 in state func_001286F0 */
void func_001287B0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0x1306, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003B0000);
    Debilitas_LungeEnd(p);
}

extern const PTMF D_003AFFD0;

/* turning to Fiona; when the animation ends, the next part (func_00128A20) */
static inline void Debilitas_TurnToFiona(Pursuer *p) {
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

/* state: turning to Fiona (see Debilitas_TurnToFiona) */
void func_00128CA0(Pursuer *p) {
    Debilitas_TurnToFiona(p);
}

extern const PTMF D_003AFFC0;

/* start of the turn to Fiona: finish the current walk, then animation 0x1304 in state
   func_00128CA0 */
void func_00128DB0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0x1304, 0);
    PU(p, 0x16F7, u8) = 1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003AFFC0);
    Debilitas_TurnToFiona(p);
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
    nm = gNavMesh;
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

/* a grab at Fiona: at the animation's hit key, once, if she's within reach (gProgress vtable
   +0x2C), it lands (func_00178070 kind 1); over when the animation ends, she's out of sight, or
   60 units away */
static inline void Debilitas_Grab(Pursuer *p) {
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

/* state: the grab (see Debilitas_Grab) */
void func_00128390(Pursuer *p) {
    Debilitas_Grab(p);
}

extern const PTMF D_003B0020;

/* start of the grab: finish the current walk, then animation 0xE06 in state func_00128390 */
void func_001284C0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0xE06, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003B0020);
    Debilitas_Grab(p);
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

/* the rooms he heads for when he has lost Fiona: {room, nav triangle}, ended by room -1 */
typedef struct { s32 room; s32 tri; } DebilitasRoomSpot;
extern DebilitasRoomSpot D_003AFAE0[];

/* vtable +0xE8: pick where to go. Already in one of his rooms: go to its spot (when on screen).
   Otherwise route to the nearest of them. Returns 0 when none can be reached. */
s32 func_00128090(Pursuer *p) {
    u8 i;
    u32 best = -1;
    s32 bestRoom = -1;

    for (i = 0; D_003AFAE0[i].room != -1; i++) {
        DebilitasRoomSpot *e = &D_003AFAE0[i];
        u32 d;

        if (p->c.a.room == e->room) {
            if (func_00217510(p) != 0) {
                f32 pos[4];

                VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, e->tri, pos);
                VCALL(p, 0xAC, void (*)(Pursuer *, s32, f32 *, s32))(p, e->tri, pos, D_003AFAE0[i].room);
            }
            return 1;
        }
        d = func_00126F80(&p->c, e->room, PU(p, 0x1598, s32), -1, -1);
        if (d != 0 && d < best) {
            bestRoom = e->room;
            best = d;
        }
    }
    if (bestRoom == -1) {
        return 0;
    }
    PU(p, 0x1594, s32) = bestRoom;
    PU(p, 0x1598, s32) = -1;
    func_00126F80(&p->c, bestRoom, PU(p, 0x1598, s32), -1, -1);
    return 1;
}

extern const PTMF D_003AFF90;
extern PTMF D_0045B340;

/* vtable +0x2B8: carry on with his behaviour. Out of route (and not just changing rooms,
   func_0029A8C0): head for one of his rooms (vtable +0xE8), forgetting the searched rooms once if
   none is left; then the move step, and when chasing, back to his first behaviour with the idle
   move */
void func_00129B30(Pursuer *p) {
    PTMF *st;

    if (!(func_0029A8C0(p, -1) & 0xFF) && !(p->c.unk1388 < p->c.unk1384)) {
        s32 retried = 0;

        for (;;) {
            if ((VCALL(p, 0xE8, s32 (*)(Pursuer *))(p) & 0xFF) == 1) {
                PU(p, 0x17B0, u8) = VCALL(gRooms, 0x3C, u32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C, u16), p->c.a.room);
                break;
            }
            if (retried) {
                PU(p, 0x1660, s32) = 0;
                break;
            }
            {
                u32 k;

                for (k = 0; k < 13; k++) {
                    p->c.unk148C[k] = 0;
                }
            }
            retried = 1;
        }
    }
    st = (PTMF *)((u8 *)p + 0x17A0);
    if (ptmf_test(st)) {
        ptmf_scall(p, st);
    }
    if (PU(p, 0x16C8, u8) == 0) {
        PU(p, 0x16F6, u8) = 1;
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF90);
        PU(p, 0x1758, s32) = -1;
        Pursuer_SetMove(p, &D_0045B340);
    }
}

extern u8 D_003AF780[], D_003AF7C0[], D_003AF800[], D_003AF840[], D_003AF880[], D_003AF8A0[],
    D_003AF8C0[], D_003AF900[], D_003AF930[], D_003AF978[], D_003AF990[], D_003AF9D0[],
    D_003AF9E0[], D_003AFA10[], D_003AFA40[], D_003AFA70[], D_003AFA80[];
extern u8 D_003AFBA0[], D_003AFBF0[], D_003AFC40[], D_003AFC80[], D_003AFCB0[], D_003AFCF0[],
    D_003AFD10[], D_003AFD50[], D_003AFD80[], D_003AFDC0[], D_003AFDD0[], D_003AFE00[],
    D_003AFE10[], D_003AFE50[], D_003AFEA0[], D_003AFED0[], D_003AFEE0[];

/* his attack tables ({kind, value, chance}, see func_00283C50) for each situation 0..16; the
   second set when gProgress+0x30 bit 0x8000 is set */
static u8 *const sAttackTables[2][17] = {
    { D_003AF780, D_003AF800, D_003AF7C0, D_003AF840, D_003AF880, D_003AF8A0, D_003AF8C0,
      D_003AF900, D_003AF930, D_003AF978, D_003AF990, D_003AF9D0, D_003AF9E0, D_003AFA10,
      D_003AFA70, D_003AFA80, D_003AFA40 },
    { D_003AFBA0, D_003AFC40, D_003AFBF0, D_003AFC80, D_003AFCB0, D_003AFCF0, D_003AFD10,
      D_003AFD50, D_003AFD80, D_003AFDC0, D_003AFDD0, D_003AFE00, D_003AFE10, D_003AFE50,
      D_003AFED0, D_003AFEE0, D_003AFEA0 },
};

/* vtable +0x130: the attack table for a situation (the Pursuer has none) */
void func_00127D00(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = (u32)situation < 17 ? sAttackTables[alt][situation] : sAttackTables[alt][0];
}

extern const f32 D_003B0030[4];

/* vtable +0x1A8: the chase towards the target (func_00290810), but on the path he cuts straight
   for the target once he's no more than 10 units further from it than the path's end is */
void func_00129570(Pursuer *p) {
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

        end[0] = D_003B0030[0];
        end[1] = D_003B0030[1];
        end[2] = D_003B0030[2];
        end[3] = D_003B0030[3];
        end[0] = AT(p, 0x124 + n * 0xC, f32);
        end[2] = AT(p, 0x128 + p->c.unk124 * 0xC, f32);
        tri = func_00124480(&p->c.a, end, p->c.a.navMask);
        if (tri == AT(p, 0x120 + p->c.unk124 * 0xC, u32)) {
            VCALL(gNavMesh, 0x14, void (*)(void *, u32, f32 *))(gNavMesh, tri, end);
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

extern const PTMF D_003AFFE0, D_003AFFF0;

/* state: his blow, after the turn to Fiona. The hit point is the bone of the attack entry
   (+0x171C, +0x94); off the nav mesh it misses (flinch, state D_003AFFE0). Otherwise when it
   reaches Fiona or Hewie it lands: func_00178070 with the entry's kind and damage, stunning
   (0x8000) when a 0..100 roll is under its stun chance; then the flinch and state D_003AFFF0 */
void func_00128A20(Pursuer *p) {
    u8 *e = PU(p, 0x171C, u8 *) + 0x90;
    f32 pos[4] __attribute__((aligned(16)));
    u32 hit;

    sceVu0CopyVector(pos, func_0017CE80(MOTION_AT(p, 0x810, u8 *), AT(e, 0x4, s32)) + 0xC);
    if (func_00124480(&p->c.a, pos, p->c.a.navMask & ~0x40) == (u32)-1) {
        func_00297B40(p, 0x1004, (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) != 0);
        p->c.moveSub = 0;
        Actor_SetState(&p->c.a, &D_003AFFE0);
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
        f32 roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        s16 stun = roll < AT(e, 0x18, f32) ? 0x8000 : 0;

        func_00178070(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
    }
    PU(p, 0x1764, s32) = 12;
    if ((((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) == 0) {
        func_00297B40(p, 0x1004, 0);
    }
    p->c.moveSub = 0;
    Actor_SetState(&p->c.a, &D_003AFFF0);
}

/* vtable +0x30: his frame update (Stalker_ThinkStart / Stalker_ThinkEnd around it): on screen
   the full think: hits, reactions to cries, the behaviour step, a growl (sound 0x2A) every 90
   frames while idle, the stance logic (+0x110) and his voice; off screen the behaviour step and
   the off-screen move */
void func_001297C0(Pursuer *p) {
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

extern const PTMF D_003AFF70;

/* vtable +0x2B0: his step while heading somewhere through doors (as func_002928B0): walk to the
   door +0x17B0, open it (0xA / 0xE), go through, or plan round it; the attacks 0x28 / 0x29 pick
   their table (vtable +0x130) and strike; 0xB / 0x10 / 0x12 end in a walk (4). When the step is
   over: through the door (+0x16ED), or back to his first behaviour when chasing */
void func_00129DB0(Pursuer *p) {
    u32 ds = 0xFF;

    PU(p, 0x1544, u8) = 1;
    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    if (p->c.moveMode != 2) {
        func_002837C0(p, 0xFF);
    }
    if (PU(p, 0x17B0, u8) != 0xFF) {
        ds = func_00177BF0(gProgress, PU(p, 0x17B0, u8), *(u8 *)&p->c.a.slot) & 0xFF;
    }
    switch (PU(p, 0x175C, s32)) {
    case 0xD:
    case 0xC:
        if (PURSUER_STEP_DONE(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x26);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0x28:
    case 0x29:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 8);
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, PU(p, 0x175C, s32) == 0x28 ? 0xE : 0xF);
            func_00283C50(p);
            return;
        }
        break;
    case 0x12:
    case 0xB:
    case 0x10:
        if (PU(p, 0x16EF, u8) == 1) {
            PURSUER_STEP_NEXT(p) = 1;
            func_0029B190(p);
            PU(p, 0x16EF, u8) = 0;
            return;
        }
        if (PURSUER_STEP_DONE(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0xE:
        if (PURSUER_STEP_DONE(p) == 1 || (ds & 2)) {
            PU(p, 0x16ED, u8) = 1;
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0xA:
        if (PU(p, 0x16EF, u8) == 1) {
            PU(p, 0x16EF, u8) = 0;
            func_0029B190(p);
            return;
        }
        if (PURSUER_STEP_DONE(p) == 1) {
            /* at the door: open it (0xE left in the register by the case chain, as func_002928B0) */
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xE);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 4:
    case 0x26:
    case 0x25: {
        VObject *rooms;
        f32 door[4] __attribute__((aligned(16)));
        u32 onMesh, a;

        if (PU(p, 0x16EF, u8) == 1) {
            PU(p, 0x16EF, u8) = 0;
            func_0029B190(p);
            return;
        }
        if (func_0029A8C0(p, -1) != 0) {
            if (PURSUER_STEP_DONE(p) == 1) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x25);
            }
            break;
        }
        rooms = gRooms;
        VCALL(rooms, 0x30, void (*)(VObject *, u32, f32 *))(rooms, PU(p, 0x17B0, u8), door);
        onMesh = func_00124480(&p->c.a, door, -1) != (u32)-1;
        a = func_00123C60(&p->c.a, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0)) & 0xFF;
        if (!(ds & 4) && PURSUER_STEP_DONE(p) != 1 && !((onMesh & 0xFF) & a)) {
            break;
        }
        switch (func_00211E00(p, PU(p, 0x17B0, u8)) & 0xFF) {
        case 5:
        case 6:
            if (!(Progress_CurRoomFlag(gProgress, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF)) {
                p->c.unk100 = PU(p, 0x17B0, u8);
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xA);
                PURSUER_STEP_DONE(p) = 0;
            }
            break;
        case 4:
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xE);
            PURSUER_STEP_DONE(p) = 0;
            break;
        case 3: {
            /* locked: mark the link as visited (+0x148C) and plan again to the goal room */
            u32 bit = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFFFF;

            p->c.unk148C[bit >> 5] |= 1 << (bit & 0x1F);
            if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) == -1 &&
                (func_0027CA00(p) & 0xFF) != 1) {
                func_0029AC50(p);
                return;
            }
            PU(p, 0x17B0, u8) = VCALL(rooms, 0x3C, s32 (*)(VObject *, u32, s32))(rooms, PU(p, 0x138C, u16), p->c.a.room);
            PU(p, 0x15A4, s32) = VCALL(rooms, 0x34, s32 (*)(VObject *, u32, f32 *))(rooms, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
            PURSUER_STEP_DONE(p) = 0;
            break;
        }
        }
        break;
    }
    }
    if (PURSUER_STEP_NEXT(p) != 1) {
        return;
    }
    if (PU(p, 0x16ED, u8) == 1) {
        func_002815E0(p, PU(p, 0x17B0, u8));
        PU(p, 0x16ED, u8) = 0;
        return;
    }
    if (PU(p, 0x16C8, u8) == 0) {
        PU(p, 0x16F6, u8) = 1;
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF70);
        PU(p, 0x1758, s32) = -1;
    }
}
