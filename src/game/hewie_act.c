/* Hewie's actions (func_00130AF0): taking up action `act` sets what he does next - his
 * behaviour (a pointer to member, HEWIE_STATE), his animation group (+0xF3604, changed with a
 * 10-frame blend +0xF3608), timers (+0xF355C, +0xF36B4..), his target (+0xF3544) and command
 * flags (+0xF356C). An action he can't do (the target out of sight, a floor that doesn't
 * allow it) passes on to the one func_0013B2C0 picks instead. */
#include "common.h"
#include "hewie.h"
#include "progress.h"
#include "ptmf.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"

extern Character *gCharPlayer;
extern Character *gCharPursuer;
extern Character *gCharacters[6];
extern Progress *gProgress;

#define RNG01() VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)

extern s32 func_0013B2C0(Hewie *h, s32 act);
extern s32 func_00137650(Hewie *h, void *other);
extern void *func_001379C0(Hewie *h);
extern s32 func_001382F0(Hewie *h);
extern u8 func_0013CDC0(Hewie *h, Character *from, s32 both);
extern s32 func_0013EE40(Hewie *h, u32 tri, const f32 *pos, s32 direct, s32 keep);
extern void func_0013E680(Hewie *h);
extern void func_0013A430(Hewie *h, s32 snd);
extern void func_00139840(Hewie *h);
extern void func_00138AD0(Hewie *h, s32 mode, s32 time);
extern void func_00140B00(Hewie *h);
extern void func_001407C0(Hewie *h);
extern void func_001404E0(Hewie *h);
extern void func_00140190(Hewie *h);
extern void func_00140050(Hewie *h);
extern void func_00143550(Hewie *h, s32 blend);
extern s32 func_00126F80(Character *c, s32 target, s32 unused2, s32 side, s32 unused4);
extern s32 func_00122B50(Actor *a, f32 *out);
extern void func_00122C20(Actor *a, s32 id, s32 b, s32 c, s32 d, f32 *dir);
extern f32 func_00124490(Actor *a, const f32 *p);
extern f32 func_001244D0(Actor *a, const f32 *p);
extern f32 func_002E2D00(f32 a);   /* an angle into -pi..pi */
extern f32 func_0031C5C0(f32 x, f32 z);
extern void func_002DDED0(void *motion, s32 anim, s32 variant);
extern void func_002DDC60(void *motion, s32 anim, s32 blend, s32 variant);
extern void func_002DDBA0(void *motion, s32 anim, s32 blend);
extern void func_002DD110(void *motion, f32 *ref, f32 *dist, f32 *angle);
extern s32 func_00178300(Progress *p, s32 room, u32 exit, u32 side);
extern s32 func_001785B0(Progress *p, s32 room, u32 exit);
extern s32 func_00178980(Progress *p, s32 room, s32 exit);

/* his behaviours */
extern void func_0014B590(Hewie *), func_0015F8A0(Hewie *), func_0015F760(Hewie *), func_0015F0A0(Hewie *);
extern void func_0015F750(Hewie *), func_0015BB20(Hewie *), func_0015F2E0(Hewie *), func_0015ECC0(Hewie *);
extern void func_0015E3A0(Hewie *), func_0015E880(Hewie *), func_0015DF40(Hewie *), func_0015DB60(Hewie *);
extern void func_0015CCA0(Hewie *), func_0015C1E0(Hewie *), func_0015BD90(Hewie *), func_0014B190(Hewie *);
extern void func_00153350(Hewie *), func_0015B660(Hewie *), func_0015B130(Hewie *), func_0015AE10(Hewie *);
extern void func_001531F0(Hewie *), func_0015A460(Hewie *), func_001569C0(Hewie *), func_0015A720(Hewie *);
extern void func_001558F0(Hewie *), func_001557B0(Hewie *), func_00155670(Hewie *), func_00155000(Hewie *);
extern void func_00154E60(Hewie *), func_00154E40(Hewie *), func_00154DC0(Hewie *), func_00154DB0(Hewie *);
extern void func_00154D50(Hewie *), func_00154D40(Hewie *), func_0014B4D0(Hewie *), func_0014A790(Hewie *);
extern void func_0014A180(Hewie *), func_00149DD0(Hewie *), func_001499F0(Hewie *), func_00149370(Hewie *);
extern void func_00149270(Hewie *), func_001489D0(Hewie *), func_001480C0(Hewie *), func_001480B0(Hewie *);
extern void func_00154860(Hewie *), func_001545A0(Hewie *), func_00154150(Hewie *), func_00153B00(Hewie *);
extern void func_00153700(Hewie *), func_00152D60(Hewie *), func_00151D10(Hewie *), func_00151B10(Hewie *);
extern void func_001517C0(Hewie *), func_00151740(Hewie *), func_00151190(Hewie *), func_001506A0(Hewie *);
extern void func_00150610(Hewie *), func_00150450(Hewie *), func_0014F5B0(Hewie *), func_0014F5A0(Hewie *);
extern void func_0014EE20(Hewie *), func_0014EC10(Hewie *), func_0014EB40(Hewie *), func_0014DFB0(Hewie *);
extern void func_0014DE70(Hewie *), func_0014DA50(Hewie *), func_0014C210(Hewie *), func_0014BE80(Hewie *);
extern void func_0014B780(Hewie *), func_00153D20(Hewie *), func_00147B90(Hewie *), func_00147580(Hewie *);
extern void func_001470C0(Hewie *), func_00146AE0(Hewie *), func_001476C0(Hewie *);

void func_00130AF0(Hewie *h, s32 act, s32 arg);

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
    s32 r = func_0013B2C0(h, act);

    if (r != act) {
        func_00130AF0(h, r, 0);
    } else {
        func_00130AF0(h, act, arg);
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
    return (func_00137650(h, who) & 0xFF) == 1;
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

    return (func_00178980(p, h->c.a.room, e) & 0xFF) == 1 && !(func_001785B0(p, h->c.a.room, e) & 0xFF)
        && (func_00178300(p, h->c.a.room, e, HW(h, 0x20, u8)) & 0xFF) == 1;
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
    HW(h, 0xF36BC, s32) = func_001382F0(h);
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

void func_00130AF0(Hewie *h, s32 act, s32 arg) {
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
            STATE(h, func_0014B590);
        } else {
            HW(h, 0x2D, u8) = 0;
            HW(h, 0xF3559, s8) = 1;
            anim(h, 4);
            STATE(h, func_0015F8A0);
        }
        return;
    case 0x2:   /* follow Fiona */
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            WAIT(h) = sWait[HW(h, 0xF35CC, s16)];
            CMD(h) = 0x688;
            STATE(h, func_0015F760);
        } else {
            instead(h, 4, 0);
        }
        break;
    case 0x7:
        MODE(h) = 0;
        WAIT(h) = sWait[HW(h, 0xF35CC, s16)];
        TARGET(h) = func_001379C0(h);
        CMD(h) = 0x58C;
        STATE(h, func_0015F0A0);
        break;
    case 0x1:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            CMD(h) = 0x6AE;
            STATE(h, func_0015F760);
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
            STATE(h, func_0015F760);
        } else {
            instead(h, 4, 0);
        }
        break;
    case 0x4:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x7AD;
        STATE(h, func_0015F750);
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
        STATE(h, func_0015F750);
        return;
    }
    case 0x14:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, func_0015BB20);
        return;
    case 0x6:
        MODE(h) = 0;
        WAIT(h) = 0x5A;
        TARGET(h) = func_001379C0(h);
        CMD(h) = 0x6AF;
        STATE(h, func_0015F2E0);
        break;
    case 0x8:
        TARGET(h) = func_001379C0(h);
        if (HW(h, 0xF366D, u8) == 2 || sees(h, TARGET(h))) {
            WAIT(h) = 0x96;
            MODE(h) = 0;
            CMD(h) = 0x7AF;
            STATE(h, func_0015ECC0);
        } else {
            instead(h, 6, 0);
        }
        break;
    case 0xA:
        if (HW(h, 0xF3548, void *) == NULL) {
            HW(h, 0xF3548, void *) = func_001379C0(h);
        }
        if (!sees(h, HW(h, 0xF3548, void *))) {
            instead(h, 6, 0);
        } else {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = (s32)(2.0f * RNG01()) + 1;
            TARGET(h) = HW(h, 0xF3548, Character *);
            CMD(h) = 0x7AF;
            STATE(h, func_0015E3A0);
        }
        break;
    case 0x9:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            WAIT(h) = 0x12C;
            CMD(h) = 0x384;
            STATE(h, func_0015E880);
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
            STATE(h, func_0015DF40);
        }
        break;
    case 0xC:
    case 0xD:
    case 0xE:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 2;
            CMD(h) = 0x68F;
            STATE(h, func_0015DB60);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0xF:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 2;
            CMD(h) = 0x58F;
            STATE(h, func_0015DB60);
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
            HW(h, 0xF36BC, s32) = func_001382F0(h);
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            turn_speed(h);
            TARGET(h) = gCharPlayer;
            WAIT(h) = (s32)(3.0f * RNG01()) * 30 + 30;
            CMD(h) = 0x78D;
            STATE(h, func_0015CCA0);
        }
        break;
    case 0x11:
        TARGET(h) = func_001379C0(h);
        if (sees(h, TARGET(h))) {
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 0;
            HW(h, 0xF36BC, s32) = 2;
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            HW(h, 0xF36C8, f32) = 30.0f;
            WAIT(h) = 0x1E;
            CMD(h) = 0x78D;
            STATE(h, func_0015CCA0);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x12:
        TARGET(h) = func_001379C0(h);
        if (sees(h, TARGET(h))) {
            MODE(h) = 0;
            anim(h, 0);
            HW(h, 0xF36B4, s32) = -1;
            HW(h, 0xF36B8, s32) = 0;
            CMD(h) = 0x48F;
            STATE(h, func_0015C1E0);
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
        STATE(h, func_0015BD90);
        break;
    case 0x63:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            CMD(h) = 0x60F;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 2;
            HW(h, 0x108, s32) = -1;
            STATE(h, func_0014B190);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x1D:
        MODE(h) = 0;
        anim(h, 0);
        TARGET(h) = gCharPlayer;
        CMD(h) = 0;
        STATE(h, func_00153350);
        break;
    case 0x15:
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = 0x1E;
        HW(h, 0xF36B8, s32) = 0;
        HW(h, 0xF36BC, s32) = func_001382F0(h);
        HW(h, 0xF36C4, f32) = func_002E2D00(h->c.a.angle[1] + AT(h->c.motion, 0x858, f32));
        turn_speed(h);
        WAIT(h) = (s32)(3.0f * RNG01()) * 30 + 30;
        CMD(h) = 0x78D;
        STATE(h, func_0015B660);
        break;
    case 0x16:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            CMD(h) = 0x68E;
            STATE(h, func_0015B130);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x17:
        func_00140B00(h);
        break;
    case 0x18:
        if (tri_flags(h) & 3) {
            instead(h, 0, 0);
            return;
        }
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = (s32)(2.0f * RNG01());
        CMD(h) = 0x62D;
        STATE(h, func_0015AE10);
        /* fall through (the floor checked again; his animation group to 4) */
    case 0x19:
    case 0x1A:
    case 0x1B:
        if (!(tri_flags(h) & 3)) {
            MODE(h) = 0;
            anim(h, 4);
            CMD(h) = 0x62D;
            STATE(h, func_0015AE10);
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
            STATE(h, func_0015AE10);
            return;
        }
        instead(h, 0, 0);
        return;
    case 0x1E:
        MODE(h) = 0;
        anim(h, 0);
        TARGET(h) = gCharPlayer;
        CMD(h) = 0;
        STATE(h, func_001531F0);
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
                STATE(h, func_0015A460);
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
                if (func_00124490(&h->c.a, TARGET(h)->a.pos) <= 10.0f) {
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
                STATE(h, func_001569C0);
                return;
            }
        }
        give_up(h);
        break;
    case 0x24: {
        u8 place;

        if (sees(h, gCharPlayer)) {
            place = func_0013CDC0(h, gCharPlayer, 0);
        } else {
            place = func_0013CDC0(h, &h->c, 0);
        }
        if (place == 0xFF) {
            instead(h, 0, 0);
        } else {
            to_place(h, place);
            STATE(h, func_0015A720);
        }
        break;
    }
    case 0x25: {
        u8 place = func_0013CDC0(h, &h->c, 0);

        if (place == 0xFF) {
            instead(h, 0x10, 0);
        } else {
            to_place(h, place);
            STATE(h, func_0015A720);
        }
        break;
    }
    case 0x26:
    case 0x27:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x7AF;
        STATE(h, func_001558F0);
        return;
    case 0x28:
    case 0x29:
        if (!(tri_flags(h) & 0x80001)) {
            MODE(h) = 0;
            anim(h, 4);
            CMD(h) = 0x7AF;
            STATE(h, func_001557B0);
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
            STATE(h, func_00155670);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x2C:
    case 0x2D:
    case 0x39:   /* out of the room by the path he has (or to the progress's room) */
        if (HW(h, 0xF3590, u8) == 0) {
            if (func_00126F80(&h->c, VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), -1, HEWIE_SIDE(h), -1) <= 0) {
                path_clear(h);
                func_0013E680(h);
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
        STATE(h, func_00155000);
        return;
    case 0x2E:
        MODE(h) = 6;
        CMD(h) = 0xDF;
        WAIT(h) = D_003B13D0[HW(h, 0xF35CC, s16)];
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, func_00154E60);
        break;
    case 0x2F:
    case 0x30:
    case 0x31:
        MODE(h) = 6;
        CMD(h) = a == 0x2F ? 0x88 : a == 0x30 ? 0x80 : 0xDF;
        WAIT(h) = HW(h, 0xF3560, s32);
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, func_00154E60);
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
            if (func_00126F80(&h->c, VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, h->c.a.room, e), -1,
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
            STATE(h, func_00155000);
            return;
        }
        func_0013E680(h);
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
            if (func_00126F80(&h->c, HW(h, 0xF3594, s32), -1, HEWIE_SIDE(h), -1) <= 0) {
                path_clear(h);
                func_0013E680(h);
                return;
            }
            door_from_path(h);
        }
        MODE(h) = 6;
        CMD(h) = 0xDF;
        STATE(h, func_00155000);
        return;
    }
    case 0x34: {
        f32 at[4] __attribute__((aligned(16)));

        MODE(h) = 6;
        CMD(h) = 0x80;
        WAIT(h) = 0x3C;
        if (func_00122B50(&h->c.a, at) != 0) {
            sceVu0SubVector(at, at, h->c.a.pos);
            func_00122C20(&h->c.a, 0x69, 5, 0, 0, at);
        }
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, func_00154E40);
        break;
    }
    case 0x35: {   /* to another room by a random open exit (the progress's room: at once) */
        Progress *p = gProgress;
        u8 seen = 0;
        u32 n;

        if (func_00126F80(&h->c, VCALL(p, 0xC, s32 (*)(Progress *))(p), -1, HEWIE_SIDE(h), -1) != 2) {
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
                if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p) && func_00126F80(&h->c, room, -1, HEWIE_SIDE(h), -1) == 1) {
                    door_from_path(h);
                    MODE(h) = 6;
                    CMD(h) = 0x80;
                    HW(h, 0xF3590, u8) = 0;
                    HW(h, 0xFC, s32) = 0x17;
                    STATE(h, func_00155000);
                    return;
                }
            }
        }
        MODE(h) = 6;
        CMD(h) = 0x80;
        HW(h, 0xF3590, u8) = 0;
        HW(h, 0xFC, s32) = 0x17;
        STATE(h, func_00154DC0);
        break;
    }
    case 0x36:
        MODE(h) = 6;
        CMD(h) = 0;
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, func_00154DB0);
        break;
    case 0x37:
        MODE(h) = 6;
        CMD(h) = 0;
        path_clear(h);
        HW(h, 0x1388, s32) = HW(h, 0x1384, s32);
        STATE(h, func_00154D50);
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
        STATE(h, func_00154D40);
        break;
    case 0x3B:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        func_002DDED0(h->c.motion, HW(h, 0x104, s32), -1);
        HW(h, 0xE1, s8) = 1;
        STATE(h, func_0014B4D0);
        return;
    case 0x3C:
    case 0x47:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        func_002DDC60(h->c.motion, HW(h, 0x104, s32), HW(h, 0x108, s32), -1);
        HW(h, 0xE1, s8) = 1;
        STATE(h, func_0014B4D0);
        return;
    case 0x3D:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        func_002DDBA0(h->c.motion, HW(h, 0x104, s32), HW(h, 0x108, s32));
        HW(h, 0xE1, s8) = 1;
        STATE(h, func_0014B4D0);
        return;
    case 0x3E:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        func_00143550(h, HW(h, 0x108, s32));
        HW(h, 0xE1, s8) = 1;
        STATE(h, func_0014B4D0);
        return;
    case 0x3F:
        MODE(h) = 0;
        CMD(h) = 0;
        HW(h, 0xF36B4, s32) = 0;
        HW(h, 0xF36B8, s32) = 0;
        STATE(h, func_0014B190);
        return;
    case 0x40:
        MODE(h) = 0;
        CMD(h) = 0;
        HW(h, 0xF36B4, s32) = 0;
        HW(h, 0xF36B8, s32) = 2;
        STATE(h, func_0014B190);
        break;
    case 0x41:
    case 0x42:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, func_0014A790);
        return;
    case 0x43:
        if (gCharacters[HW(h, 0x100, s32)] == NULL) {
            HW(h, 0xE1, s8) = 1;
        } else {
            MODE(h) = 0;
            CMD(h) = 0;
            HW(h, 0x10C, f32) = func_001244D0(&h->c.a, gCharacters[HW(h, 0x100, s32)]->a.pos);
            STATE(h, func_0014A180);
        }
        break;
    case 0x44:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, func_0014A180);
        return;
    case 0x45:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, func_00149DD0);
        return;
    case 0x46:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, func_001499F0);
        return;
    case 0x48: {   /* turn toward +0x10C, in steps of 6 degrees */
        f32 d;
        s32 steps;

        if (!(func_002E2D00(HW(h, 0x10C, f32) - h->c.a.angle[1]) <= 0.0f)) {
            d = func_002E2D00(HW(h, 0x10C, f32) - h->c.a.angle[1]);
        } else {
            d = -func_002E2D00(HW(h, 0x10C, f32) - h->c.a.angle[1]);
        }
        steps = (s32)(d / 0x1.aceea00000000p-4f /* 0.10471976 */);
        HW(h, 0xF36B4, s32) = steps;
        HW(h, 0xF36B8, s32) = steps < 6 ? 1 : 0;
        MODE(h) = 0xC;
        anim(h, 0);
        TARGET(h) = gCharPlayer;
        CMD(h) = 8;
        STATE(h, func_00149370);
        break;
    }
    case 0x49:
    case 0x4A:
    case 0x4B:
        MODE(h) = 0xC;
        anim(h, 4);
        CMD(h) = 0x408;
        STATE(h, func_00149270);
        return;
    case 0x4C:
        instead(h, 0x15, 0);
        break;
    case 0x4D:
        func_001407C0(h);
        break;
    case 0x4F:
    case 0x50:
        if (HW(h, 0xF3598, s32) == 1) {
            HW(h, 0xF35B0, s16) = 0x384;
        }
        func_001404E0(h);
        break;
    case 0x4E:
        HW(h, 0xF35B0, s16) = 0;
        func_00140190(h);
        break;
    case 0x51:
        func_00140050(h);
        break;
    case 0x52:
        func_00138AD0(h, 0, -1);
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
        STATE(h, func_001489D0);
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
                STATE(h, func_001480C0);
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
            STATE(h, func_001480C0);
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
        STATE(h, func_001480C0);
        break;
    case 0x58:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x68F;
        STATE(h, func_001480B0);
        return;
    case 0x5A:
        HW(h, 0xF3560, s32) = 3;
        /* fall through */
    case 0x59:
        if (HW(h, 0xF3548, void *) == NULL) {
            HW(h, 0xF3548, void *) = func_001379C0(h);
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
        STATE(h, func_00154860);
        break;
    case 0x66: {
        f32 at[4] __attribute__((aligned(16)));

        MODE(h) = 0;
        HW(h, 0x2D, u8) = 1;
        HW(h, 0x2B, s8) = 1;
        HW(h, 0xC0, s32) = 8;
        AT(HW(h, 0x1380, void *), 0x40, s32) = HW(h, 0xC0, s32);
        anim(h, 4);
        if (func_0013EE40(h, VCALL(gRooms, 0x30, s32 (*)(VObject *, u8, f32 *))(gRooms, (u8)HW(h, 0xF36B4, s32), at),
                          at, 0, 1) != 0) {
            HW(h, 0x124, s32) = HW(h, 0x128, s32);
        }
        CMD(h) = 0;
        STATE(h, func_001545A0);
        break;
    }
    case 0x67:
        if (sees(h, gCharPlayer)) {
            MODE(h) = 0;
            anim(h, 0);
            TARGET(h) = gCharPlayer;
            HW(h, 0xF36B4, s32) = 0;
            CMD(h) = 0x78D;
            STATE(h, func_00154150);
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
        func_002DDED0(h->c.motion, 0x1000, -1);
        func_0013A430(h, 0x66);
        STATE(h, func_00153B00);
        return;
    case 0x69:
        MODE(h) = 4;
        face(h);
        CMD(h) = 0;
        HW(h, 0xF36C8, f32) = 1.5f;
        HW(h, 0xF36CC, f32) = 0.5f;
        func_002DDED0(h->c.motion, 0x1000, -1);
        if (HW(h, 0x104, s32) == 1) {
            func_0013A430(h, 0x65);
        } else {
            func_0013A430(h, 0x66);
        }
        STATE(h, func_00153B00);
        return;
    case 0x6A:
        MODE(h) = 0;
        HW(h, 0xF36B4, s32) = 0;
        CMD(h) = 8;
        STATE(h, func_00153700);
        return;
    case 0x6B:
    case 0x6C:
        MODE(h) = 4;
        anim(h, 4);
        CMD(h) = 0;
        HW(h, 0xF36C8, f32) = 5.0f;
        HW(h, 0xF36CC, f32) = 1.0f;
        func_002DDED0(h->c.motion, 0x1000, -1);
        func_0013A430(h, 0x66);
        if (a == 0x6B) {
            STATE(h, func_00153B00);
            return;
        }
        HW(h, 0x2D, u8) = 1;
        HW(h, 0x2B, s8) = 1;
        STATE(h, func_00153B00);
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
                STATE(h, func_00152D60);
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
        STATE(h, func_00151D10);
        return;
    case 0x6F:
        CMD(h) = 0;
        STATE(h, func_00151B10);
        break;
    case 0x70:
        HW(h, 0xF36B4, s32) = 0x1E;
        HW(h, 0xF36B8, s32) = 0;
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0;
        STATE(h, func_001517C0);
        break;
    case 0x71:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 0x400;
        STATE(h, func_00151740);
        return;
    case 0x72:
        if (sees(h, gCharPlayer)) {
            TARGET(h) = gCharPlayer;
            HW(h, 0x124, s32) = HW(h, 0x128, s32);
            MODE(h) = 0;
            CMD(h) = 0;
            STATE(h, func_00151190);
        } else {
            instead(h, 0, 0);
        }
        break;
    case 0x73:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, func_001506A0);
        return;
    case 0x74:
        MODE(h) = 0;
        CMD(h) = 0;
        func_002DDED0(h->c.motion, 0x2213, -1);
        func_0013A430(h, 0x66);
        STATE(h, func_00150610);
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
                STATE(h, func_00150450);
                return;
            }
        }
        instead(h, 0, 0);
        break;
    case 0x76:
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, func_0014F5B0);
        return;
    case 0x77:
        MODE(h) = 6;
        CMD(h) = 8;
        STATE(h, func_0014F5A0);
        return;
    case 0x78:
        HW(h, 0xF3686, s16) = 0;
        HW(h, 0xF3684, s16) = 0;
        HW(h, 0xF36A4, s32) = 0x78;
        HW(h, 0xF36A2, s8) = 0;
        func_00139840(h);
        MODE(h) = 0;
        CMD(h) = 0x68E;
        if (HW(h, 0xF369C, u8) != 0) {
            STATE(h, func_0014EE20);
        } else if (HW(h, 0xF369D, u8) != 0) {
            STATE(h, func_0014EC10);
        } else {
            HW(h, 0xF36BC, s32) = 0x5A;
            STATE(h, func_0014EB40);
        }
        return;
    case 0x79:
        HW(h, 0x2D, u8) = 1;
        MODE(h) = 0xB;
        func_002DDED0(h->c.motion, 0x1301, -1);
        CMD(h) = 0;
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 0;
        STATE(h, func_0014DFB0);
        return;
    case 0x7A:
        MODE(h) = 0;
        WAIT(h) = sWait[HW(h, 0xF35CC, s16)];
        HW(h, 0xF36B4, s32) = 0x3C;
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 0;
        CMD(h) = AT(gProgress, 0x1FBEC1, u8) == 0 ? 0x88 : 8;
        STATE(h, func_0014DE70);
        return;
    case 0x7B:
        if (sees(h, gCharPursuer)) {
            s32 m = gCharPursuer->moveMode;

            if (m != 3 && m != 4) {
                u8 place = func_0013CDC0(h, gCharPursuer, 1);

                if (place != 0xFF) {
                    f32 at[4] __attribute__((aligned(16)));

                    HW(h, 0x100, s32) = place;
                    HW(h, 0xF36B4, s32) = VCALL(gRooms, 0x30, s32 (*)(VObject *, u32, f32 *))(gRooms, place, at);
                    HW(h, 0xF36BC, s32) = 0;
                    sceVu0CopyVector((f32 *)((u8 *)h + 0xF36E0), at);
                    HW(h, 0x124, s32) = HW(h, 0x128, s32);
                    MODE(h) = 0;
                    CMD(h) = 0x88;
                    STATE(h, func_0014DA50);
                    return;
                }
            }
        }
        instead(h, 6, 0);
        break;
    case 0x7C:
        TARGET(h) = func_001379C0(h);
        if (sees(h, TARGET(h))) {
            HW(h, 0xF36C8, f32) = 30.0f;
            MODE(h) = 0;
            HW(h, 0xF36B4, s32) = 0;
            HW(h, 0xF36B8, s32) = 0;
            HW(h, 0xF36C4, f32) = h->c.a.angle[1];
            CMD(h) = 0x88;
            STATE(h, func_001480C0);
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
            STATE(h, func_0014C210);
            return;
        }
        instead(h, 0, 0);
        break;
    case 0x7F:
        HW(h, 0xF36C4, f32) = HW(h, 0x114, f32);
        HW(h, 0xF36B8, s32) = HW(h, 0x2D, u8) == 1 ? 1 : 0;
        MODE(h) = 0;
        CMD(h) = 0;
        STATE(h, func_0014BE80);
        return;
    case 0x80:
        MODE(h) = 0;
        CMD(h) = 8;
        HW(h, 0xF36B4, s32) = HW(h, 0xF3568, s32);
        STATE(h, func_0014B780);
        break;
    case 0x81: {
        f32 dist, ang, to;

        MODE(h) = 0;
        anim(h, 8);
        func_002DD110(h->c.motion, (f32 *)((u8 *)h + 0xF3630), &dist, &ang);
        to = func_002E2D00(h->c.a.angle[1] + ang);
        HW(h, 0xF3614, f32) = dist;
        HW(h, 0xF3618, f32) = func_002E2D00(to - h->c.a.angle[1]);
        CMD(h) = 0x7AD;
        STATE(h, func_0015F750);
        break;
    }
    case 0x82: {
        Character *t = TARGET(h);

        if (t != NULL && t->a.active == 1 && h->c.a.room == t->a.room && t->a.navTri != (u32)-1) {
            MODE(h) = 0;
            anim(h, 0);
            HW(h, 0xF36B4, s32) = 0;
            CMD(h) = 8;
            STATE(h, func_00153D20);
            return;
        }
        instead(h, 0, 0);
        break;
    }
    case 0x83:
        MODE(h) = 0;
        anim(h, 4);
        CMD(h) = 8;
        STATE(h, func_00147B90);
        return;
    case 0x84:
        HW(h, 0x2D, u8) = 1;
        MODE(h) = 0;
        HW(h, 0xF3604, s32) = 4;
        HW(h, 0xF3608, s32) = 0;
        CMD(h) = 0;
        STATE(h, func_00147580);
        break;
    case 0x85:   /* look at Fiona, else the pursuer, within 150 */
        MODE(h) = 0;
        TARGET(h) = NULL;
        if (sees(h, gCharPlayer) && func_00124490(&h->c.a, gCharPlayer->a.pos) < 150.0f) {
            TARGET(h) = gCharPlayer;
        }
        if (TARGET(h) == NULL && sees(h, gCharPursuer) && func_00124490(&h->c.a, gCharPursuer->a.pos) < 150.0f) {
            TARGET(h) = gCharPursuer;
        }
        CMD(h) = 0;
        STATE(h, func_001470C0);
        break;
    case 0x86:
        MODE(h) = 0;
        HW(h, 0x2B, s8) = 1;
        HW(h, 0x2D, u8) = 1;
        VCALL(gRooms, 0x34, void (*)(VObject *, u8, void *))(gRooms, (u8)HW(h, 0xF36B4, s32), (u8 *)h + 0xF36E0);
        CMD(h) = 0;
        STATE(h, func_00146AE0);
        break;
    case 0x87:
        MODE(h) = 8;
        HW(h, 0xF36D0, f32) = func_002E2D00(h->c.a.angle[1] + AT(h->c.motion, 0x858, f32));
        CMD(h) = 0;
        STATE(h, func_001476C0);
        return;
    default:
        return;
    }
}
