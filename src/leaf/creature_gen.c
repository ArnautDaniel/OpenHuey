/* Methods of the room creature classes (pursuer-like characters: the destructor, the
 * "in play" step, taking action 5, the slot's progress entry) and two room callbacks still
 * left in the PS2 disassembly, matched to the shape of their decompiled counterparts
 * (func_00308EC0, func_00311AE0, func_00311B30, func_00311C00, func_002AFA00, func_002CCA80)
 * and generated from them (2026-10-05). */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "actor.h"
#include "progress.h"
#include "pursuer.h"

extern void *D_0046D810[], *D_0046C220[], *D_00469C60[], *D_00469C20[];
extern Character *gCharPursuer;
extern void func_00124E40(Actor *a);
extern void func_0016CEC0(Progress *p, const char *name);
extern s32 func_0016CD60(Progress *p, s32 who, s32 arg);
extern void func_0016CD30(Progress *p);

/* destructor: own vtable -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character; the model freed for
 * slots 3..5 */
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt) {
    if (c != NULL) {
        c->a.vtbl = vt;
        c->a.vtbl = D_0046D810;
        VCALL(c, 0x10, void (*)(Character *))(c);
        if ((u32)c->a.slot >= 3 && (u32)c->a.slot < 6) {
            void **m = c->motion;

            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                c->motion = NULL;
            }
        }
        c->a.vtbl = D_0046C220;
        VCALL(c, 0x10, void (*)(Character *))(c);
        c->a.vtbl = D_00469C60;
        c->a.vtbl = D_00469C20;
        if ((s16)flags > 0) {
            func_00124E40(&c->a);
        }
    }
    return c;
}

/* in play: func_00124890(-1) */
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p) {
    if (func_00217510(p) != 0) {
        func_00124890(&p->c.a, -1);
    }
}

/* the action 5 taken (+0x14E8): in play +0x8C, the state st, +0x114 1; the action cleared */
static inline __attribute__((always_inline)) void creature_act5(Pursuer *p, const PTMF *st) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((u8)func_00217510(p) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), st);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* its slot's progress entry (func_00177870) 1: func_001777D0; -1 */
static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p) {
    Progress *g = gProgress;

    if ((u8)func_00177870(g, *(u8 *)&p->c.a.slot) == 1) {
        func_001777D0(g, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

extern void *D_00472020[];
Character *func_003119A0(Character *c, s32 flags) { return creature_dtor(c, flags, D_00472020); }

extern void *D_004734A0[];
Character *func_0031E5C0(Character *c, s32 flags) { return creature_dtor(c, flags, D_004734A0); }

void func_0031E700(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_0042A170;
void func_0031E750(Pursuer *p) { creature_act5(p, &D_0042A170); }

s32 func_0031E820(Pursuer *p) { return creature_slot_done(p); }

extern const char *const D_0042C358;
/* a room callback: byte 3 0 a progress name, 1 wait for character 3 (2 while not), else done */
s32 func_003212A0(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        func_0016CEC0(gProgress, D_0042C358);
        return 1;
    case 1:
        return func_0016CD60(gProgress, 3, 0) == 0 ? 2 : 1;
    }
    func_0016CD30(gProgress);
    return 1;
}

extern void *D_00473CD0[];
Character *func_00321640(Character *c, s32 flags) { return creature_dtor(c, flags, D_00473CD0); }

void func_00321780(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_0042C400;
void func_003217D0(Pursuer *p) { creature_act5(p, &D_0042C400); }

s32 func_003218A0(Pursuer *p) { return creature_slot_done(p); }

void func_0032C830(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_0042C8F0;
void func_0032C880(Pursuer *p) { creature_act5(p, &D_0042C8F0); }

s32 func_0032C950(Pursuer *p) { return creature_slot_done(p); }

extern void *D_00474890[];
Character *func_0032C9C0(Character *c, s32 flags) { return creature_dtor(c, flags, D_00474890); }

void func_0032CB00(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_0042C980;
void func_0032CB50(Pursuer *p) { creature_act5(p, &D_0042C980); }

s32 func_0032CC20(Pursuer *p) { return creature_slot_done(p); }

extern s32 func_0029A710(Pursuer *p);
/* a room callback: the pursuer's func_0029A710 */
s32 func_00339E20(void) { return func_0029A710((Pursuer *)gCharPursuer); }

extern void *D_00476120[];
Character *func_0033ABA0(Character *c, s32 flags) { return creature_dtor(c, flags, D_00476120); }

void func_0033ACE0(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_004308A0;
void func_0033AD30(Pursuer *p) { creature_act5(p, &D_004308A0); }

s32 func_0033AE00(Pursuer *p) { return creature_slot_done(p); }

extern void *D_00477790[];
Character *func_00345100(Character *c, s32 flags) { return creature_dtor(c, flags, D_00477790); }

void func_00345240(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_0043B630;
void func_00345290(Pursuer *p) { creature_act5(p, &D_0043B630); }

s32 func_00345360(Pursuer *p) { return creature_slot_done(p); }

extern void *D_00478770[];
Character *func_0034B7B0(Character *c, s32 flags) { return creature_dtor(c, flags, D_00478770); }

extern void *D_00474130[];
Character *func_0032C240(Character *c, s32 flags) { return creature_dtor(c, flags, D_00474130); }

void func_0032C380(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_0042C720;
void func_0032C3D0(Pursuer *p) { creature_act5(p, &D_0042C720); }

s32 func_0032C4A0(Pursuer *p) { return creature_slot_done(p); }

extern void *D_00476C10[];
Character *func_0033D6C0(Character *c, s32 flags) { return creature_dtor(c, flags, D_00476C10); }

void func_0033D800(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_00430A90;
void func_0033D850(Pursuer *p) { creature_act5(p, &D_00430A90); }

s32 func_0033D920(Pursuer *p) { return creature_slot_done(p); }

extern void *D_00477AE0[];
Character *func_00345EE0(Character *c, s32 flags) { return creature_dtor(c, flags, D_00477AE0); }

extern void *D_00477E30[];
Character *func_003478C0(Character *c, s32 flags) { return creature_dtor(c, flags, D_00477E30); }

extern void *D_00478160[];
Character *func_00348850(Character *c, s32 flags) { return creature_dtor(c, flags, D_00478160); }

extern void *D_00479B20[];
Character *func_003634F0(Character *c, s32 flags) { return creature_dtor(c, flags, D_00479B20); }

extern void *D_0046F020[];
Character *func_002CD4E0(Character *c, s32 flags) { return creature_dtor(c, flags, D_0046F020); }

extern void *D_00473110[];
Character *func_0031D6F0(Character *c, s32 flags) { return creature_dtor(c, flags, D_00473110); }

void func_0031D830(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_00429DF0;
void func_0031D880(Pursuer *p) { creature_act5(p, &D_00429DF0); }

s32 func_0031D950(Pursuer *p) { return creature_slot_done(p); }

extern void *D_00474C10[];
Character *func_0032CE00(Character *c, s32 flags) { return creature_dtor(c, flags, D_00474C10); }

void func_0032DA60(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_0042CA10;
void func_0032DAB0(Pursuer *p) { creature_act5(p, &D_0042CA10); }

s32 func_0032DB80(Pursuer *p) { return creature_slot_done(p); }

extern void *D_00475DB0[];
Character *func_00339A10(Character *c, s32 flags) { return creature_dtor(c, flags, D_00475DB0); }

void func_00339B50(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_0042F4F0;
void func_00339BA0(Pursuer *p) { creature_act5(p, &D_0042F4F0); }

s32 func_00339C70(Pursuer *p) { return creature_slot_done(p); }

extern void *D_004764A0[];
Character *func_0033AF20(Character *c, s32 flags) { return creature_dtor(c, flags, D_004764A0); }

void func_0033B060(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_00430930;
void func_0033B0B0(Pursuer *p) { creature_act5(p, &D_00430930); }

s32 func_0033B180(Pursuer *p) { return creature_slot_done(p); }

extern void *D_004767D0[];
Character *func_0033B1F0(Character *c, s32 flags) { return creature_dtor(c, flags, D_004767D0); }

void func_0033B330(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_004309C0;
void func_0033B380(Pursuer *p) { creature_act5(p, &D_004309C0); }

s32 func_0033B450(Pursuer *p) { return creature_slot_done(p); }

extern void *D_00478FF0[];
Character *func_00351F20(Character *c, s32 flags) { return creature_dtor(c, flags, D_00478FF0); }

void func_00352060(Pursuer *p) { creature_inplay(p); }

extern const PTMF D_00443550;
void func_003520B0(Pursuer *p) { creature_act5(p, &D_00443550); }

s32 func_00352180(Pursuer *p) { return creature_slot_done(p); }

extern void *D_004738A0[];
Character *func_0031F110(Character *c, s32 flags) { return creature_dtor(c, flags, D_004738A0); }

extern void *D_00474FD0[];
Character *func_0032F5B0(Character *c, s32 flags) { return creature_dtor(c, flags, D_00474FD0); }
