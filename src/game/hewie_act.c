/* Hewie's actions (Hewie_SetAction): taking up action `act` sets what he does next - his
 * behaviour (a pointer to member, HEWIE_STATE), his animation group (+0xF3604, changed with a
 * 10-frame blend +0xF3608), timers (+0xF355C, +0xF36B4..), his target (+0xF3544) and command
 * flags (+0xF356C). An action he can't do (the target out of sight, a floor that doesn't
 * allow it) passes on to the one Hewie_AdjustAction picks instead. */
#include "common.h"
#include "hewie.h"
#include "progress.h"
#include "ptmf.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "hewie_act.h"
#include "model.h"
#include "msl.h"

#define RNG01() VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)

/* his behaviours */

#define STATE(h, fn) ptmf_set_fn(HEWIE_STATE(h), (void *)(fn))
#define TARGET(h) HW(h, 0xF3544, Character *)
#define WAIT(h) HW(h, 0xF355C, s32)
#define CMD(h) HW(h, 0xF356C, s32)
#define MODE(h) HW(h, 0xF8, s32)

/* his animation group (blended in over 10 frames when it changes) */
static inline void anim(Hewie *h, s32 a) {
    if (HW(h, 0xF3604, s32) != a) {
        HW(h, 0xF3604, s32) = a;
        HW(h, 0xF3608, s32) = 0xA;
    }
}

/* the action picked instead of `act` (`arg` if it is `act` itself) */
static inline void instead(Hewie *h, s32 act, s32 arg) {
    s32 r = Hewie_AdjustAction(h, act);

    if (r != act) {
        Hewie_SetAction(h, r, 0);
    } else {
        Hewie_SetAction(h, act, arg);
    }
}

/* nothing to do: wander (6) while the game allows, else idle */
static inline void give_up(Hewie *h) {
    if (AT(gProgress, 0x1FBEC1, u8) == 0) {
        instead(h, 6, 0);
    } else {
        instead(h, 0, 0);
    }
}

static inline s32 sees(Hewie *h, void *who) {
    return (Hewie_WithChar2(h, who) & 0xFF) == 1;
}

/* the flags of his walk-mesh triangle (+0x34) */
static inline u32 tri_flags(Hewie *h) {
    u32 t = HW(h, 0x34, u32);
    u8 *tri = NULL;

    if (t < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
        tri = AT(gNavMesh, 0x4, u8 *) + t * 0x50;
    }
    return tri != NULL ? AT(tri, 0x3C, u32) : 0;   /* (the original reads address 0x3C) */
}

static inline void path_clear(Hewie *h) {
    u32 i;

    for (i = 0; i < 0xD; i++) {
        HW(h, 0x148C + i * 4, s32) = 0;
    }
}

/* the door he takes: the path's next door (+0x138C) and its distance */
static inline void door_from_path(Hewie *h) {
    HW(h, 0x14C0, u16) = HW(h, 0x138C, u16);
    HW(h, 0x14C4, f32) = (f32)VCALL(gRooms, 0x38, s32 (*)(VObject *, u32, s32))(gRooms, HW(h, 0x138C, u16),
                                                                                      h->c.a.room);
}

/* an exit of his room he may go through: 8 tries at random */
static inline s32 exit_random(Hewie *h, u8 *seen) {
    u8 e = (u8)(u32)(8.0f * RNG01());

    while (*seen & (1 << e)) {
        e = (e + 1) & 0xFF;
        if (e == 8) {
            e = 0;
        }
    }
    *seen |= 1 << e;
    return e;
}

static inline s32 exit_open(Hewie *h, u32 e) {
    Progress *p = gProgress;

    return (Progress_ExitOpen(p, h->c.a.room, e) & 0xFF) == 1 && !(Progress_ExitUnlocked(p, h->c.a.room, e) & 0xFF)
        && (Progress_ExitPassable(p, h->c.a.room, e, HW(h, 0x20, u8)) & 0xFF) == 1;
}

/* his chasing settings: turn speed +0xF36C8 from his speed choice */
static inline void turn_speed(Hewie *h) {
    switch (HW(h, 0xF36BC, s32)) {
    case 0:
        HW(h, 0xF36C8, f32) = 20.0f;
        break;
    case 1:
        HW(h, 0xF36C8, f32) = 25.0f;
        break;
    case 2:
        HW(h, 0xF36C8, f32) = 30.0f;
        break;
    }
}

/* toward a place in the room (rooms +0x30) */
static inline void to_place(Hewie *h, u32 place) {
    f32 at[4] __attribute__((aligned(16)));

    MODE(h) = 0;
    HW(h, 0x100, s32) = place;
    HW(h, 0xF36B4, s32) = VCALL(gRooms, 0x30, s32 (*)(VObject *, u32, f32 *))(gRooms, place, at);
    HW(h, 0xF36B8, s32) = 0;
    HW(h, 0xF36BC, s32) = Hewie_RandomLevel(h);
    sceVu0CopyVector((f32 *)((u8 *)h + 0xF36E0), at);
    HW(h, 0x124, s32) = HW(h, 0x128, s32);
    CMD(h) = 0x78D;
}

/* turn to face `who`'s heading (+0x100 a character), else the place progress +0x1060 */
static inline void face(Hewie *h) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 heading;

    if (HW(h, 0x100, s32) != 0xFF) {
        anim(h, 0);
        TARGET(h) = gCharacters[HW(h, 0x100, s32)];
        heading = TARGET(h)->a.angle[1];
    } else {
        anim(h, 4);
        sceVu0CopyVector(at, (f32 *)((u8 *)gProgress + (HW(h, 0x20, u8) << 5) + 0x1060));
        sceVu0SubVector(d, h->c.a.pos, at);
        heading = func_0031C5C0(d[0], d[2]);
    }
    HW(h, 0xF36C4, f32) = heading;
}

/* the timers by his trust (+0xF35CC) */
static const s32 sWait[8] = {300, 360, 420, 480, 540, 600, 660, 720};
extern const s32 D_003B13D0[8];

/* 0x00130AF0 */
void Hewie_SetAction(Hewie *h, s32 act, s32 arg) {
    s32 a;

    HEWIE_ACTION(h) = act;
    HW(h, 0xF3570, s32) = arg;
    HW(h, 0xF3559, s8) = 0;
    if (HEWIE_ACTION(h) != 0x76 && HEWIE_ACTION(h) != 0x77 && HEWIE_ACTION(h) != 0) {
        HW(h, 0xF35B4, s32) = -1;
    }
    WAIT(h) = (s32)(10.0f * RNG01()) * 30 + 90;
    a = HEWIE_ACTION(h);
    switch (a) {
    case 0x0:   /* idle */
        HW(h, 0xF3548, void *) = NULL;
        MODE(h) = 0;
        CMD(h) = 0;
        if (HW(h, 0xE0, u8) == 1) {
            anim(h, 4);
            STATE(h, Hewie_State1D88);
        } else {
            HW(h, 0x2D, u8) = 0;
            HW(h, 0xF3559, s8) = 1;
            anim(h, 4);
            STATE(h, Hewie_State1D98);
        }
        return;
    case 0x2:   /* follow Fiona */
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            WAIT(h) = sWait[HW(h, 0xF35CC, s16)];
            CMD(h) = 0x688;
            STATE(h, Hewie_State1DC8);
        } else {
            instead(h, 4, 0);
        }
        break;
    case 0x7:
        MODE(h) = 0;
        WAIT(h) = sWait[HW(h, 0xF35CC, s16)];
        TARGET(h) = Hewie_PickTarget(h);
        CMD(h) = 0x58C;
        STATE(h, Hewie_StateSteer);
        break;
    case 0x1:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            CMD(h) = 0x6AE;
            STATE(h, Hewie_State1DC8);
        } else {
            instead(h, 4, 0);
        }
        break;
    case 0x3:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            WAIT(h) = HW(h, 0xF3560, s32);
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            CMD(h) = 0x688;
            STATE(h, Hewie_State1DC8);
        } else {
            instead(h, 4, 0);
        }
        break;
    case 0x4:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x7AD;
        STATE(h, Hewie_StatePose4);
        return;
    case 0x5: {
        s32 r;

        MODE(h) = 0;
        r = (s32)(45.0f * RNG01());
        if (r < 0x14) {
            anim(h, 1);
        } else if (r < 0x19) {
            anim(h, 0xD);
        } else if (r < 0x1E) {
            anim(h, 0xB);
        } else {
            anim(h, 0xC);
        }
        CMD(h) = 0x7AD;
        STATE(h, Hewie_StatePose4);
        return;
    }
    case 0x14:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_State1E48);
        return;
    case 0x6:
        MODE(h) = 0;
        WAIT(h) = 0x5A;
        TARGET(h) = Hewie_PickTarget(h);
        CMD(h) = 0x6AF;
        STATE(h, Hewie_StateFaceScent);
        break;
    case 0x8:
        TARGET(h) = Hewie_PickTarget(h);
        if (HW(h, 0xF366D, u8) == 2 || sees(h, TARGET(h))) {
            WAIT(h) = 0x96;
            MODE(h) = 0;
            CMD(h) = 0x7AF;
            STATE(h, Hewie_StateFaceTarget);
        } else {
            instead(h, 6, 0);
        }
        break;
    case 0xA:
        if (HW(h, 0xF3548, void *) == NULL) {
            HW(h, 0xF3548, void *) = Hewie_PickTarget(h);
        }
        if (!sees(h, HW(h, 0xF3548, void *))) {
            instead(h, 6, 0);
        } else {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = (s32)(2.0f * RNG01()) + 1;
            TARGET(h) = HW(h, 0xF3548, Character *);
            CMD(h) = 0x7AF;
            STATE(h, Hewie_StateBarkAtTarget);
        }
        break;
    case 0x9:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            WAIT(h) = 0x12C;
            CMD(h) = 0x384;
            STATE(h, Hewie_StateWaitForFiona);
        } else {
            instead(h, 6, 0);
        }
        break;
    case 0xB:
        if (!sees(h, gCharPlayer)) {
            instead(h, 6, 0);
        } else {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = (s32)(3.0f * RNG01()) + 3;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            CMD(h) = 0x384;
            STATE(h, Hewie_StateBarkAtFiona);
        }
        break;
    case 0xC:
    case 0xD:
    case 0xE:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 2;
            CMD(h) = 0x68F;
            STATE(h, Hewie_StateSlideToFiona);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0xF:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 2;
            CMD(h) = 0x58F;
            STATE(h, Hewie_StateSlideToFiona);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x10:
        if (!sees(h, gCharPlayer)) {
            instead(h, 0, 0);
        } else {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 0;
            HW(h, 0xF36BC, s32) = Hewie_RandomLevel(h);
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            turn_speed(h);
            TARGET(h) = gCharPlayer;
            WAIT(h) = (s32)(3.0f * RNG01()) * 30 + 30;
            CMD(h) = 0x78D;
            STATE(h, Hewie_StateKeepAway);
        }
        break;
    case 0x11:
        TARGET(h) = Hewie_PickTarget(h);
        if (sees(h, TARGET(h))) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 0;
            HW(h, 0xF36BC, s32) = 2;
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            HW(h, 0xF36C8, f32) = 30.0f;
            WAIT(h) = 0x1E;
            CMD(h) = 0x78D;
            STATE(h, Hewie_StateKeepAway);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x12:
        TARGET(h) = Hewie_PickTarget(h);
        if (sees(h, TARGET(h))) {
            MODE(h) = 0;
            anim(h, 0);
            HW(h, 0xF36B4, s32) = -1;
            HW(h, 0xF36B8, s32) = 0;
            CMD(h) = 0x48F;
            STATE(h, Hewie_StateFlank);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x13:
    case 0x64:
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = 0x5A;
        HW(h, 0xF36B8, s32) = 0;
        HW(h, 0xF36C4, f32) = h->c.a.angle[1];
        HW(h, 0xF36C8, f32) = 5.0f;
        CMD(h) = 0x60F;
        STATE(h, Hewie_StateScramble);
        break;
    case 0x63:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            CMD(h) = 0x60F;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 2;
            HW(h, 0x108, s32) = -1;
            STATE(h, Hewie_StateSetOff);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x1D:
        MODE(h) = 0;
        anim(h, 0);
        TARGET(h) = gCharPlayer;
        CMD(h) = 0;
        STATE(h, Hewie_State1F28);
        break;
    case 0x15:
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = 0x1E;
        HW(h, 0xF36B8, s32) = 0;
        HW(h, 0xF36BC, s32) = Hewie_RandomLevel(h);
        HW(h, 0xF36C4, f32) = Angle_Wrap(h->c.a.angle[1] + AT(h->c.motion, 0x858, f32));
        turn_speed(h);
        WAIT(h) = (s32)(3.0f * RNG01()) * 30 + 30;
        CMD(h) = 0x78D;
        STATE(h, Hewie_StateRoam);
        break;
    case 0x16:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            CMD(h) = 0x68E;
            STATE(h, Hewie_StateComeToCommand);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x17:
        Hewie_IdleAction(h);
        break;
    case 0x18:
        if (tri_flags(h) & 3) {
            instead(h, 0, 0);
            return;
        }
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = (s32)(2.0f * RNG01());
        CMD(h) = 0x62D;
        STATE(h, Hewie_StateTricks);
        /* fall through (the floor checked again; his animation group to 4) */
    case 0x19:
    case 0x1A:
    case 0x1B:
        if (!(tri_flags(h) & 3)) {
            MODE(h) = 0;
            anim(h, 4);
            CMD(h) = 0x62D;
            STATE(h, Hewie_StateTricks);
            return;
        }
        instead(h, 0, 0);
        return;
    case 0x1C:
        if (!(tri_flags(h) & 3)) {
            MODE(h) = 0;
            anim(h, 4);
            WAIT(h) = 0x384;
            CMD(h) = 0x62D;
            STATE(h, Hewie_StateTricks);
            return;
        }
        instead(h, 0, 0);
        return;
    case 0x1E:
        MODE(h) = 0;
        anim(h, 0);
        TARGET(h) = gCharPlayer;
        CMD(h) = 0;
        STATE(h, Hewie_State1F88);
        break;
    case 0x1F:
    case 0x20:
    case 0x21:
    case 0x22:   /* at the pursuer */
        if (sees(h, gCharPursuer)) {
            s32 sub = gCharPursuer->moveSub;

            if (gCharPursuer->moveMode != 3 && sub != 9 && sub != 0xA) {
                MODE(h) = 8;
                HW(h, 0xFC, s32) = 0x18;
                HW(h, 0x100, s32) = gCharPursuer->a.slot;
                TARGET(h) = gCharPursuer;
                HW(h, 0x124, s32) = HW(h, 0x128, s32);
                HW(h, 0xF3604, s32) = 4;
                HW(h, 0xF3608, s32) = 0;
                HW(h, 0x104, s32) = -1;
                CMD(h) = 8;
                STATE(h, Hewie_StateRunAtPursuer);
                return;
            }
        }
        if (AT(gProgress, 0x1FBEC1, u8) == 0) {
            instead(h, 6, 0);
        } else {
            instead(h, 0x87, 0);
        }
        break;
    case 0x23:
        if (sees(h, TARGET(h))) {
            s32 m = TARGET(h)->moveMode;

            if (m != 4 && m != 3) {
                if (Actor_Distance(&h->c.a, TARGET(h)->a.pos) <= 10.0f) {
                    if (TARGET(h) != gCharPlayer) {
                        instead(h, 0x59, 0);
                    } else {
                        instead(h, 0x61, 0);
                    }
                    return;
                }
                MODE(h) = 8;
                HW(h, 0xF36B4, s32) = 0x5A;
                CMD(h) = HW(h, 0xF35C0, s32) != 3 ? 8 : 0;
                STATE(h, Hewie_StateCloseOnTarget);
                return;
            }
        }
        give_up(h);
        break;
    case 0x24: {
        u8 place;

        if (sees(h, gCharPlayer)) {
            place = Hewie_FleeExit(h, gCharPlayer, 0);
        } else {
            place = Hewie_FleeExit(h, &h->c, 0);
        }
        if (place == 0xFF) {
            instead(h, 0, 0);
        } else {
            to_place(h, place);
            STATE(h, Hewie_StateGoToExit);
        }
        break;
    }
    case 0x25: {
        u8 place = Hewie_FleeExit(h, &h->c, 0);

        if (place == 0xFF) {
            instead(h, 0x10, 0);
        } else {
            to_place(h, place);
            STATE(h, Hewie_StateGoToExit);
        }
        break;
    }
    case 0x26:
    case 0x27:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x7AF;
        STATE(h, Hewie_StateAfter27);
        return;
    case 0x28:
    case 0x29:
        if (!(tri_flags(h) & 0x80001)) {
            MODE(h) = 0;
            anim(h, 4);
            CMD(h) = 0x7AF;
            STATE(h, Hewie_StateAfter29);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x2A:
    case 0x2B:
        if (!(tri_flags(h) & 0x80001)) {
            MODE(h) = 0;
            anim(h, 4);
            CMD(h) = 0x7AF;
            STATE(h, Hewie_StateAfter2B);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x2C:
    case 0x2D:
    case 0x39:   /* out of the room by the path he has (or to the progress's room) */
        if (HW(h, 0xF3590, u8) == 0) {
            if (Character_Route(&h->c, VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), -1, HEWIE_SIDE(h), -1) <= 0) {
                path_clear(h);
                Hewie_WhenIdle(h);
                return;
            }
            door_from_path(h);
        }
        MODE(h) = 6;
        switch (HEWIE_ACTION(h)) {
        case 0x2C:
            CMD(h) = 0xCF;
            break;
        case 0x2D:
            CMD(h) = 8;
            break;
        case 0x39:
            CMD(h) = 0;
            break;
        }
        CMD(h) = 0xCF;   /* (the original's: the choice above never counts) */
        STATE(h, Hewie_StateToDoor);
        return;
    case 0x2E:
        MODE(h) = 6;
        CMD(h) = 0xDF;
        WAIT(h) = D_003B13D0[HW(h, 0xF35CC, s16)];
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_State2018);
        break;
    case 0x2F:
    case 0x30:
    case 0x31:
        MODE(h) = 6;
        CMD(h) = a == 0x2F ? 0x88 : a == 0x30 ? 0x80 : 0xDF;
        WAIT(h) = HW(h, 0xF3560, s32);
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_State2018);
        break;
    case 0x32: {   /* out by a random open exit */
        u8 seen = 0;
        u32 n;

        for (n = 0; n < 8; n++) {
            u8 e = exit_random(h, &seen);
            u16 door = VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e);

            if (door == 0xFFFF) {
                continue;
            }
            if ((VCALL(gRooms, 0x74, u32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e) & 0xFF) != 1
                || !exit_open(h, e)) {
                continue;
            }
            if (Character_Route(&h->c, VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e), -1,
                              HEWIE_SIDE(h), -1) <= 0) {
                path_clear(h);
                continue;
            }
            if (HW(h, 0xF3590, u8) == 0 || HW(h, 0x14C0, u16) != door) {
                HW(h, 0xF3590, u8) = 0;
                HW(h, 0xFC, s32) = 0x17;
                door_from_path(h);
            }
            MODE(h) = 6;
            CMD(h) = 0xDF;
            STATE(h, Hewie_StateToDoor);
            return;
        }
        Hewie_WhenIdle(h);
        break;
    }
    case 0x33: {   /* to the room +0xF3594 */
        s32 ok = 0;

        if (HW(h, 0xF3590, u8) == 1) {
            u8 e = VCALL(gRooms, 0x3C, s32 (*)(VObject *, u32, s32))(gRooms, HW(h, 0x14C0, u16), h->c.a.room);

            if (e != 0xFF && VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e)
                                 == HW(h, 0xF3594, s32)) {
                ok = 1;
            }
        }
        if (!(ok & 0xFF)) {
            HW(h, 0xF3590, u8) = 0;
            HW(h, 0xFC, s32) = 0x17;
            if (Character_Route(&h->c, HW(h, 0xF3594, s32), -1, HEWIE_SIDE(h), -1) <= 0) {
                path_clear(h);
                Hewie_WhenIdle(h);
                return;
            }
            door_from_path(h);
        }
        MODE(h) = 6;
        CMD(h) = 0xDF;
        STATE(h, Hewie_StateToDoor);
        return;
    }
    case 0x34: {
        f32 at[4] __attribute__((aligned(16)));

        MODE(h) = 6;
        CMD(h) = 0x80;
        WAIT(h) = 0x3C;
        if (Actor_PosInCurrentRoom(&h->c.a, at) != 0) {
            sceVu0SubVector(at, at, h->c.a.pos);
            Actor_PlaySound(&h->c.a, 0x69, 5, 0, 0, at);
        }
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_StateLoudNoise);
        break;
    }
    case 0x35: {   /* to another room by a random open exit (the progress's room: at once) */
        Progress *p = gProgress;
        u8 seen = 0;
        u32 n;

        if (Character_Route(&h->c, VCALL(p, 0xC, s32 (*)(Progress *))(p), -1, HEWIE_SIDE(h), -1) != 2) {
            for (n = 0; n < 8; n++) {
                u8 e = exit_random(h, &seen);
                s32 room;

                if ((VCALL(gRooms, 0x74, u32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e) & 0xFF) != 1) {
                    continue;
                }
                if (!exit_open(h, e)) {
                    continue;
                }
                room = VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e);
                if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p) && Character_Route(&h->c, room, -1, HEWIE_SIDE(h), -1) == 1) {
                    door_from_path(h);
                    MODE(h) = 6;
                    CMD(h) = 0x80;
                    HW(h, 0xF3590, u8) = 0;
                    HW(h, 0xFC, s32) = 0x17;
                    STATE(h, Hewie_StateToDoor);
                    return;
                }
            }
        }
        MODE(h) = 6;
        CMD(h) = 0x80;
        HW(h, 0xF3590, u8) = 0;
        HW(h, 0xFC, s32) = 0x17;
        STATE(h, Hewie_State2098);
        break;
    }
    case 0x36:
        MODE(h) = 6;
        CMD(h) = 0;
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_State20A8);
        break;
    case 0x37:
        MODE(h) = 6;
        CMD(h) = 0;
        path_clear(h);
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_StateToDefault);
        break;
    case 0x38:
        MODE(h) = 6;
        CMD(h) = 0x80;
        switch (HW(h, 0xF36B8, s32)) {
        case 0x1F:
        case 0x20:
            switch (HW(h, 0xF36BC, s32)) {
            case 1:
            case 2:
                WAIT(h) = (HW(h, 0xF35CC, s16) + 1) * 0xF + 0x1F;
                break;
            case 0:
                WAIT(h) = (HW(h, 0xF35CC, s16) + 1) * 0xF + 0x3D;
                break;
            }
            break;
        case 0x21:
        case 0x22:
        case 0x75:
            WAIT(h) = (HW(h, 0xF35CC, s16) + 1) * 0xF + 0x3D;
            break;
        }
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, Hewie_State20C8);
        break;
    case 0x3B:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        Motion_PlayTable(h->c.motion, HW(h, 0x104, s32), -1);
        HW(h, 0xE1, s8) = 1;
        STATE(h, Hewie_StateRootMotion);
        return;
    case 0x3C:
    case 0x47:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        Motion_PlayBlend(h->c.motion, HW(h, 0x104, s32), HW(h, 0x108, s32), -1);
        HW(h, 0xE1, s8) = 1;
        STATE(h, Hewie_StateRootMotion);
        return;
    case 0x3D:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        Motion_PlayBlend8(h->c.motion, HW(h, 0x104, s32), HW(h, 0x108, s32));
        HW(h, 0xE1, s8) = 1;
        STATE(h, Hewie_StateRootMotion);
        return;
    case 0x3E:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        Hewie_StandAnim(h, HW(h, 0x108, s32));
        HW(h, 0xE1, s8) = 1;
        STATE(h, Hewie_StateRootMotion);
        return;
    case 0x3F:
        MODE(h) = 0;
        CMD(h) = 0;
        HW(h, 0xF36B4, s32) = 0;
        HW(h, 0xF36B8, s32) = 0;
        STATE(h, Hewie_StateSetOff);
        return;
    case 0x40:
        MODE(h) = 0;
        CMD(h) = 0;
        HW(h, 0xF36B4, s32) = 0;
        HW(h, 0xF36B8, s32) = 2;
        STATE(h, Hewie_StateSetOff);
        break;
    case 0x41:
    case 0x42:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_State2138);
        return;
    case 0x43:
        if (gCharacters[HW(h, 0x100, s32)] == NULL) {
            HW(h, 0xE1, s8) = 1;
        } else {
            MODE(h) = 0;
            CMD(h) = 0;
            HW(h, 0x10C, f32) = Actor_HeadingTo(&h->c.a, gCharacters[HW(h, 0x100, s32)]->a.pos);
            STATE(h, Hewie_StateTurnStart);
        }
        break;
    case 0x44:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_StateTurnStart);
        return;
    case 0x45:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_State2168);
        return;
    case 0x46:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_StateHeadForSpot);
        return;
    case 0x48: {   /* turn toward +0x10C, in steps of 6 degrees */
        f32 d;
        s32 steps;

        if (!(Angle_Wrap(HW(h, 0x10C, f32) - h->c.a.angle[1]) <= 0.0f)) {
            d = Angle_Wrap(HW(h, 0x10C, f32) - h->c.a.angle[1]);
        } else {
            d = -Angle_Wrap(HW(h, 0x10C, f32) - h->c.a.angle[1]);
        }
        steps = (s32)(d / 0x1.aceea00000000p-4f /* 0.10471976 */);
        HW(h, 0xF36B4, s32) = steps;
        HW(h, 0xF36B8, s32) = steps < 6 ? 1 : 0;
        MODE(h) = 0xC;
        anim(h, 0);
        TARGET(h) = gCharPlayer;
        CMD(h) = 8;
        STATE(h, Hewie_StateTurnWithFiona);
        break;
    }
    case 0x49:
    case 0x4A:
    case 0x4B:
        MODE(h) = 0xC;
        anim(h, 4);
        CMD(h) = 0x408;
        STATE(h, Hewie_State2198);
        return;
    case 0x4C:
        instead(h, 0x15, 0);
        break;
    case 0x4D:
        Hewie_After4D(h);
        break;
    case 0x4F:
    case 0x50:
        if (HW(h, 0xF3598, s32) == 1) {
            HW(h, 0xF35B0, s16) = 0x384;
        }
        Hewie_AfterCall(h);
        break;
    case 0x4E:
        HW(h, 0xF35B0, s16) = 0;
        Hewie_AfterComingOut(h);
        break;
    case 0x51:
        Hewie_RandomIdle(h);
        break;
    case 0x52:
        Hewie_SetMode(h, 0, -1);
        if (HW(h, 0x29, u8) == 0) {
            MODE(h) = 0;
            HW(h, 0xF3604, s32) = 4;
            HW(h, 0xF3608, s32) = 0;
            HW(h, 0x2D, u8) = 1;
        } else {
            MODE(h) = 6;
            HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        }
        HW(h, 0x14C8, s32) = 0;
        WAIT(h) = 0x1518;
        CMD(h) = 0;
        STATE(h, Hewie_StateKnockedDown);
        return;
    case 0x54:
        TARGET(h) = gCharPlayer;
        /* fall through */
    case 0x53:
        if (sees(h, TARGET(h))) {
            s32 m = TARGET(h)->moveMode;

            if (m != 4 && m != 3) {
                HW(h, 0xF36C8, f32) = 30.0f;
                MODE(h) = 8;
                HW(h, 0xF36B4, s32) = 0;
                HW(h, 0xF36B8, s32) = 0;
                HW(h, 0xF36C4, f32) = h->c.a.angle[1];
                WAIT(h) = 0x3C;
                CMD(h) = HW(h, 0xF35C0, s32) != 3 ? 0x88 : 0x80;
                STATE(h, Hewie_StateKeepNear);
                return;
            }
        }
        give_up(h);
        break;
    case 0x55:
        if (sees(h, gCharPlayer)) {
            TARGET(h) = gCharPlayer;
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 0;
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            HW(h, 0xF36C8, f32) = 20.0f;
            CMD(h) = 0x384;
            STATE(h, Hewie_StateKeepNear);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x56:
    case 0x57:
        MODE(h) = 0;
        switch (HEWIE_ACTION(h)) {
        case 0x56:
            HW(h, 0xF36C8, f32) = 30.0f;
            WAIT(h) = 0x3C;
            break;
        case 0x57:
            HW(h, 0xF36C8, f32) = 20.0f;
            break;
        }
        HW(h, 0xF36B4, s32) = 0;
        HW(h, 0xF36B8, s32) = 0;
        HW(h, 0xF36C4, f32) = h->c.a.angle[1];
        CMD(h) = 0x7AF;
        STATE(h, Hewie_StateKeepNear);
        break;
    case 0x58:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x68F;
        STATE(h, Hewie_StatePose2);
        return;
    case 0x5A:
        HW(h, 0xF3560, s32) = 3;
        /* fall through */
    case 0x59:
        if (HW(h, 0xF3548, void *) == NULL) {
            HW(h, 0xF3548, void *) = Hewie_PickTarget(h);
        }
        if (sees(h, HW(h, 0xF3548, void *))) {
            TARGET(h) = HW(h, 0xF3548, Character *);
            if (TARGET(h) != NULL) {
                instead(h, 0x53, 0x23);
                return;
            }
        }
        give_up(h);
        break;
    case 0x5B:
    case 0x5C:
    case 0x5D:
    case 0x5E:
    case 0x5F:
    case 0x60: {   /* at character 3, 4 or 5 (the "+3" ones set +0xF3560 too) */
        s32 k = 0;

        switch (a) {
        case 0x5C:
            HW(h, 0xF3560, s32) = 3;
            /* fall through */
        case 0x5B:
            k = 3;
            break;
        case 0x5E:
            HW(h, 0xF3560, s32) = 3;
            /* fall through */
        case 0x5D:
            k = 4;
            break;
        case 0x60:
            HW(h, 0xF3560, s32) = 3;
            /* fall through */
        case 0x5F:
            k = 5;
            break;
        }
        TARGET(h) = gCharacters[k];
        if (sees(h, TARGET(h))) {
            instead(h, 0x53, 0x23);
            return;
        }
        instead(h, 0, 0);
        break;
    }
    case 0x62:
        HW(h, 0xF3560, s32) = 3;
        /* fall through */
    case 0x61:
        if (!sees(h, gCharPlayer)) {
            give_up(h);
        } else {
            instead(h, 0x54, 0x23);
        }
        break;
    case 0x65:
        MODE(h) = 0;
        if (tri_flags(h) & 0x20000) {
            HW(h, 0x2B, s8) = 1;
        }
        HW(h, 0xF36B8, s32) = 0x5A;
        CMD(h) = 0;
        STATE(h, Hewie_StateSqueeze);
        break;
    case 0x66: {
        f32 at[4] __attribute__((aligned(16)));

        MODE(h) = 0;
        HW(h, 0x2D, u8) = 1;
        HW(h, 0x2B, s8) = 1;
        HW(h, 0xC0, s32) = 8;
        AT(HW(h, 0x1380, void *), 0x40, s32) = HW(h, 0xC0, s32);
        anim(h, 4);
        if (Hewie_PlanAndGo(h, VCALL(gRooms, 0x30, s32 (*)(VObject *, u8, f32 *))(gRooms, (u8)HW(h, 0xF36B4, s32), at),
                          at, 0, 1) != 0) {
            HW(h, 0x124, s32) = HW(h, 0x128, s32);
        }
        CMD(h) = 0;
        STATE(h, Hewie_StateWalkOut);
        break;
    }
    case 0x67:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            HW(h, 0xF36B4, s32) = 0;
            CMD(h) = 0x78D;
            STATE(h, Hewie_StateStepAwayFiona);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x68:
        MODE(h) = 4;
        face(h);
        CMD(h) = 0;
        HW(h, 0xF36C8, f32) = 4.0f;
        HW(h, 0xF36CC, f32) = 0.5f;
        Motion_PlayTable(h->c.motion, 0x1000, -1);
        Hewie_MakeSound(h, 0x66);
        STATE(h, Hewie_StateMoveAlong);
        return;
    case 0x69:
        MODE(h) = 4;
        face(h);
        CMD(h) = 0;
        HW(h, 0xF36C8, f32) = 1.5f;
        HW(h, 0xF36CC, f32) = 0.5f;
        Motion_PlayTable(h->c.motion, 0x1000, -1);
        if (HW(h, 0x104, s32) == 1) {
            Hewie_MakeSound(h, 0x65);
        } else {
            Hewie_MakeSound(h, 0x66);
        }
        STATE(h, Hewie_StateMoveAlong);
        return;
    case 0x6A:
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = 0;
        CMD(h) = 8;
        STATE(h, Hewie_StateOffMesh);
        return;
    case 0x6B:
    case 0x6C:
        MODE(h) = 4;
        anim(h, 4);
        CMD(h) = 0;
        HW(h, 0xF36C8, f32) = 5.0f;
        HW(h, 0xF36CC, f32) = 1.0f;
        Motion_PlayTable(h->c.motion, 0x1000, -1);
        Hewie_MakeSound(h, 0x66);
        if (a == 0x6B) {
            STATE(h, Hewie_StateMoveAlong);
            return;
        }
        HW(h, 0x2D, u8) = 1;
        HW(h, 0x2B, s8) = 1;
        STATE(h, Hewie_StateMoveAlong);
        break;
    case 0x6D:
        if (sees(h, TARGET(h))) {
            s32 m = TARGET(h)->moveMode;

            if (m != 3 && m != 4) {
                HW(h, 0xF36B4, s32) = 0x5A;
                HW(h, 0x124, s32) = HW(h, 0x128, s32);
                MODE(h) = 8;
                CMD(h) = 8;
                HW(h, 0x104, s32) = 0;
                HW(h, 0x108, s32) = 0;
                STATE(h, Hewie_StateCloseIn);
                return;
            }
        }
        give_up(h);
        break;
    case 0x6E:
        HW(h, 0xF36B4, s32) = -1;
        HW(h, 0xF36B8, s32) = 0;
        MODE(h) = 0;
        CMD(h) = 8;
        STATE(h, Hewie_StateSlope);
        return;
    case 0x6F:
        CMD(h) = 0;
        STATE(h, Hewie_StateRunToFiona);
        break;
    case 0x70:
        HW(h, 0xF36B4, s32) = 0x1E;
        HW(h, 0xF36B8, s32) = 0;
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        STATE(h, Hewie_StateRun);
        break;
    case 0x71:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x400;
        STATE(h, Hewie_State22B8);
        return;
    case 0x72:
        if (sees(h, gCharPlayer)) {
            TARGET(h) = gCharPlayer;
            HW(h, 0x124, s32) = HW(h, 0x128, s32);
            MODE(h) = 0;
            CMD(h) = 0;
            STATE(h, Hewie_StateKeepBehind);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x73:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_StateAnimOver);
        return;
    case 0x74:
        MODE(h) = 0;
        CMD(h) = 0;
        Motion_PlayTable(h->c.motion, 0x2213, -1);
        Hewie_MakeSound(h, 0x66);
        STATE(h, Hewie_State22E8);
        return;
    case 0x75:
        if (sees(h, gCharPursuer)) {
            s32 sub = gCharPursuer->moveSub;

            if (gCharPursuer->moveMode != 3 && sub != 9 && sub != 0xA) {
                MODE(h) = 8;
                HW(h, 0x100, s32) = gCharPursuer->a.slot;
                TARGET(h) = gCharPursuer;
                HW(h, 0x124, s32) = HW(h, 0x128, s32);
                HW(h, 0xF3604, s32) = 4;
                HW(h, 0xF3608, s32) = 0;
                CMD(h) = 0;
                HW(h, 0x2D, u8) = 1;
                STATE(h, Hewie_StateTargetPursuer);
                return;
            }
        }
        instead(h, 0, 0);
        break;
    case 0x76:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_StatePlayAnim);
        return;
    case 0x77:
        MODE(h) = 6;
        CMD(h) = 8;
        STATE(h, Hewie_State2318);
        return;
    case 0x78:
        HW(h, 0xF3686, s16) = 0;
        HW(h, 0xF3684, s16) = 0;
        HW(h, 0xF36A4, s32) = 0x78;
        HW(h, 0xF36A2, s8) = 0;
        Hewie_RollReaction(h);
        MODE(h) = 0;
        CMD(h) = 0x68E;
        if (HW(h, 0xF369C, u8) != 0) {
            STATE(h, Hewie_StateFetch);
        } else if (HW(h, 0xF369D, u8) != 0) {
            STATE(h, Hewie_StateFollowMover);
        } else {
            HW(h, 0xF36BC, s32) = 0x5A;
            STATE(h, Hewie_State1A60);
        }
        return;
    case 0x79:
        HW(h, 0x2D, u8) = 1;
        MODE(h) = 0xB;
        Motion_PlayTable(h->c.motion, 0x1301, -1);
        CMD(h) = 0;
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 0;
        STATE(h, Hewie_State2358);
        return;
    case 0x7A:
        MODE(h) = 0;
        WAIT(h) = sWait[HW(h, 0xF35CC, s16)];
        HW(h, 0xF36B4, s32) = 0x3C;
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 0;
        CMD(h) = AT(gProgress, 0x1FBEC1, u8) == 0 ? 0x88 : 8;
        STATE(h, Hewie_State2388);
        return;
    case 0x7B:
        if (sees(h, gCharPursuer)) {
            s32 m = gCharPursuer->moveMode;

            if (m != 3 && m != 4) {
                u8 place = Hewie_FleeExit(h, gCharPursuer, 1);

                if (place != 0xFF) {
                    f32 at[4] __attribute__((aligned(16)));

                    HW(h, 0x100, s32) = place;
                    HW(h, 0xF36B4, s32) = VCALL(gRooms, 0x30, s32 (*)(VObject *, u32, f32 *))(gRooms, place, at);
                    HW(h, 0xF36BC, s32) = 0;
                    sceVu0CopyVector((f32 *)((u8 *)h + 0xF36E0), at);
                    HW(h, 0x124, s32) = HW(h, 0x128, s32);
                    MODE(h) = 0;
                    CMD(h) = 0x88;
                    STATE(h, Hewie_StatePlanPursuer);
                    return;
                }
            }
        }
        instead(h, 6, 0);
        break;
    case 0x7C:
        TARGET(h) = Hewie_PickTarget(h);
        if (sees(h, TARGET(h))) {
            HW(h, 0xF36C8, f32) = 30.0f;
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 0;
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            CMD(h) = 0x88;
            STATE(h, Hewie_StateKeepNear);
            return;
        }
        instead(h, 6, 0);
        break;
    case 0x7D:
    case 0x7E:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            CMD(h) = 0x32D;
            STATE(h, Hewie_StateWhine);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x7F:
        HW(h, 0xF36C4, f32) = HW(h, 0x114, f32);
        HW(h, 0xF36B8, s32) = HW(h, 0x2D, u8) == 1 ? 1 : 0;
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_StateRunForSpot);
        return;
    case 0x80:
        MODE(h) = 0;
        CMD(h) = 8;
        HW(h, 0xF36B4, s32) = HW(h, 0xF3568, s32);
        STATE(h, Hewie_State23D8);
        break;
    case 0x81: {
        f32 dist, ang, to;

        MODE(h) = 0;
        anim(h, 8);
        Motion_LookAt(h->c.motion, (f32 *)((u8 *)h + 0xF3630), &dist, &ang);
        to = Angle_Wrap(h->c.a.angle[1] + ang);
        HW(h, 0xF3614, f32) = dist;
        HW(h, 0xF3618, f32) = Angle_Wrap(to - h->c.a.angle[1]);
        CMD(h) = 0x7AD;
        STATE(h, Hewie_StatePose4);
        break;
    }
    case 0x82: {
        Character *t = TARGET(h);

        if (t != NULL && t->a.active == 1 && h->c.a.room == t->a.room && t->a.navTri != (u32)-1) {
            MODE(h) = 0;
            anim(h, 0);
            HW(h, 0xF36B4, s32) = 0;
            CMD(h) = 8;
            STATE(h, Hewie_StateStepAwayTarget);
            return;
        }
        instead(h, 0, 0);
        break;
    }
    case 0x83:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 8;
        STATE(h, Hewie_StateSteered);
        return;
    case 0x84:
        HW(h, 0x2D, u8) = 1;
        MODE(h) = 0;
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 0;
        CMD(h) = 0;
        STATE(h, Hewie_State2418);
        break;
    case 0x85:   /* look at Fiona, else the pursuer, within 150 */
        MODE(h) = 0;
        TARGET(h) = NULL;
        if (sees(h, gCharPlayer) && Actor_Distance(&h->c.a, gCharPlayer->a.pos) < 150.0f) {
            TARGET(h) = gCharPlayer;
        }
        if (TARGET(h) == NULL && sees(h, gCharPursuer) && Actor_Distance(&h->c.a, gCharPursuer->a.pos) < 150.0f) {
            TARGET(h) = gCharPursuer;
        }
        CMD(h) = 0;
        STATE(h, Hewie_StateBark);
        break;
    case 0x86:
        MODE(h) = 0;
        HW(h, 0x2B, s8) = 1;
        HW(h, 0x2D, u8) = 1;
        VCALL(gRooms, 0x34, void (*)(VObject *, u8, void *))(gRooms, (u8)HW(h, 0xF36B4, s32), (u8 *)h + 0xF36E0);
        CMD(h) = 0;
        STATE(h, Hewie_StateBackOnMesh);
        break;
    case 0x87:
        MODE(h) = 8;
        HW(h, 0xF36D0, f32) = Angle_Wrap(h->c.a.angle[1] + AT(h->c.motion, 0x858, f32));
        CMD(h) = 0;
        STATE(h, Hewie_StateRunUpJump);
        return;
    default:
        return;
    }
}
