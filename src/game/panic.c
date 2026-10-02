/* Fiona's panic (SceneGame +0x7F8): +0x4 the level (0..100; at 100 she panics), made of a
 * lasting part (+0x1C) and a passing one (+0xC); each frame the fear inputs (+0x10.. +0x2C,
 * mode +0x1) are folded in and cleared. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "sce/libvu0.h"

extern VObject *D_0044E988;   /* the item manager */
extern u8 *gCharPlayer;
extern void func_002EF580(u8 *o);
extern void func_002EFBE0(u8 *o);

typedef union {
    u32 u;
    f32 f;
} F32Bits;

#define PANIC_MAX 100.0f


/* the per-frame update */
void func_002F0500(u8 *o) {
    static const F32Bits k06 = {0x3F19999A}, k08 = {0x3F4CCCCD}, k16 = {0x3FCCCCCD},
                         k09 = {0x3F666666};
    Progress *p = gProgress;
    f32 scale, a, b, c, d;

    if (Progress_TestFlag(p, 8)) {
        AT(o, 0x34, s32) = 0;
    }
    func_002EF580(o);
    if (AT(o, 0x8, s16) != 0) {
        AT(o, 0x8, s16)--;
    }
    if (AT(o, 0x4, f32) < PANIC_MAX) {
        func_002EFBE0(o);
        scale = AT(p, 0x9EC, s32) != 0 ? AT(p, 0x9E8, f32) : 1.0f;
        a = AT(o, 0x10, f32);
        b = AT(o, 0x28, f32);
        c = AT(o, 0x18, f32);
        d = AT(o, 0x20, f32);
        switch (AT(o, 0x1, u8)) {
        case 0:
            d = d + AT(o, 0x24, f32);
            a = a + AT(o, 0x14, f32);
            b = b + AT(o, 0x2C, f32);
            break;
        case 1:
            d = d + k06.f * AT(o, 0x24, f32);
            a = a + k06.f * AT(o, 0x14, f32);
            b = b + 2.0f * AT(o, 0x2C, f32);
            break;
        case 2:
            d = d + k08.f * AT(o, 0x24, f32);
            a = a + k08.f * AT(o, 0x14, f32);
            b = b + k16.f * AT(o, 0x2C, f32);
            break;
        case 3:
            d = d + k09.f * AT(o, 0x24, f32);
            a = a + k09.f * AT(o, 0x14, f32);
            b = b + 1.5f * AT(o, 0x2C, f32);
            break;
        }
        d = d * scale;
        a = a * scale;
        switch (VCALL(D_0044E988, 0x10, s32 (*)(VObject *, s32))(D_0044E988, 1)) {
        case 0x86:
        case 0x87:
            b = b * 1.25f;
            break;
        case 0x88:
            b = b * 2.0f;
            break;
        }
        AT(o, 0xC, f32) = AT(o, 0xC, f32) - c;
        if (AT(o, 0xC, f32) < 0.0f) {
            AT(o, 0xC, f32) = 0.0f;
        }
        AT(o, 0x1C, f32) = AT(o, 0x1C, f32) - b;
        if (AT(o, 0x1C, f32) < 0.0f) {
            AT(o, 0x1C, f32) = 0.0f;
        }
        AT(o, 0x1C, f32) = AT(o, 0x1C, f32) + d;
        if (a <= 0.0f) {
            AT(o, 0x4, f32) = AT(o, 0x1C, f32) + AT(o, 0xC, f32);
            if (!(AT(o, 0x4, f32) < PANIC_MAX)) {
                AT(o, 0x4, f32) = 99.0f;
                AT(o, 0x1C, f32) = 99.0f;
            }
        } else {
            AT(o, 0x1C, f32) = AT(o, 0x1C, f32) + AT(o, 0xC, f32);
            AT(o, 0xC, f32) = a;
            AT(o, 0x4, f32) = AT(o, 0x1C, f32) + AT(o, 0xC, f32);
        }
        /* she can only reach 100 under flag 0x15, or as Hewie's partner away from her room */
        if ((u8)Progress_TestFlag(p, 0x15) == 1 ||
            (AT(p, 0x1FBEC1, u8) == 1 &&
             AT(gCharPlayer, 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p))) {
            if (!(AT(o, 0x4, f32) < PANIC_MAX)) {
                AT(o, 0x4, f32) = 99.0f;
                AT(o, 0x1C, f32) = 99.0f;
                AT(o, 0xC, f32) = 0.0f;
            }
        }
    }
    AT(o, 0x1, u8) = 0;
    AT(o, 0x10, f32) = 0.0f;
    AT(o, 0x14, f32) = 0.0f;
    AT(o, 0x20, f32) = 0.0f;
    AT(o, 0x24, f32) = 0.0f;
    AT(o, 0x28, f32) = 0.0f;
    AT(o, 0x2C, f32) = 0.0f;
    AT(o, 0x20, f32) = 0.0f;
    AT(o, 0x24, f32) = 0.0f;
    AT(o, 0x18, f32) = 0.0f;
}

extern VObject *D_0044E4B8;   /* the camera */
extern void func_00122C20(u8 *c, s32 a, s32 b, s32 c2, s32 d, s32 e);
extern void func_002EF2B0(u8 *o);

/* the panic stage (+0x0: 0 calm, 1..3 by level 60 / 75 / 90, 4 panicking, 5 calming down),
 * heartbeat timer (+0x30), its loudness (+0x38) and the camera shake (+0x40) */
void func_002EF580(u8 *o) {
    static const F32Bits kShake = {0x3E4CCCCD};   /* 0.2f */
    VObject *cam;

    if (AT(o, 0x38, s32) < 0x80) {
        AT(o, 0x38, s32) += 4;
        if (AT(o, 0x38, s32) > 0x80) {
            AT(o, 0x38, s32) = 0x80;
        }
    }
    if (AT(o, 0x30, s32) != 0) {
        AT(o, 0x30, s32)--;
    }
    cam = D_0044E4B8;
    VCALL(cam, 0x6C, void (*)(VObject *, f32))(cam, 0.0f);
    switch (AT(o, 0x0, u8)) {
    case 0:
        if (AT(o, 0x40, s32) != 0) {
            AT(o, 0x40, s32) -= 8;
            if (AT(o, 0x40, s32) < 0) {
                AT(o, 0x40, s32) = 0;
            }
        }
        /* fallthrough */
    case 1:
    case 2:
    case 3: {
        f32 lvl = AT(o, 0x4, f32);
        s32 t;

        if (!(lvl < PANIC_MAX)) {   /* she panics */
            AT(o, 0x4, f32) = PANIC_MAX;
            AT(o, 0x1C, f32) = PANIC_MAX;
            AT(o, 0xC, f32) = 0.0f;
            if (AT(gCharPlayer, 0xF8, s32) == 0 || AT(gCharPlayer, 0xF8, s32) == 3) {
                Progress *p;

                func_00122C20(gCharPlayer, 0x42, 5, 0x40, 0, 0);
                p = gProgress;
                VCALL(p, 0x44, void (*)(Progress *, s32))(p, 1);
                AT(o, 0x0, u8) = 4;
                AT(o, 0x2, s16) = -30;
                AT(o, 0x30, s32) = 0;
                AT(p, 0xFC2, s16)++;   /* panic count */
                if (AT(p, 0xFC2, s16) >= 10000) {
                    AT(p, 0xFC2, s16) = 9999;
                }
            }
        } else if (!(lvl < 90.0f)) {
            AT(o, 0x0, u8) = 3;
        } else if (!(lvl < 75.0f)) {
            AT(o, 0x0, u8) = 2;
        } else if (!(lvl < 60.0f)) {
            AT(o, 0x0, u8) = 1;
        } else {
            AT(o, 0x0, u8) = 0;
        }
        if (AT(o, 0x0, u8) > 0 && AT(o, 0x30, s32) == 0) {
            AT(o, 0x30, s32) = (4 - AT(o, 0x0, u8)) * 30;
            AT(o, 0x38, s32) = 0;
            func_002EF2B0(o);
        }
        if (AT(o, 0x4, f32) < 60.0f) {
            return;
        }
        t = (s32)(1.5f * (AT(o, 0x4, f32) - 60.0f));
        if (AT(o, 0x40, s32) < t) {
            AT(o, 0x40, s32)++;
        } else if (t < AT(o, 0x40, s32)) {
            AT(o, 0x40, s32)--;
        }
        return;
    }
    case 4:
        if (AT(o, 0x2, s16) < 0) {   /* the panic's start */
            AT(o, 0x2, s16)++;
            if (AT(o, 0x2, s16) == 0) {
                VCALL(gProgress, 0x44, void (*)(Progress *, s32))(gProgress, 0);
                AT(o, 0x2, s16) = 450;
                if (AT(gCharPlayer, 0xC4, s32) == 1) {
                    AT(o, 0x2, s16) += 300;
                } else if (AT(gCharPlayer, 0xC4, s32) == 3) {
                    AT(o, 0x2, s16) += 150;
                }
            }
            AT(o, 0x38, s32) = 0x80;
            return;
        }
        if (!(AT(o, 0x20, f32) < 500.0f)) {   /* more fear: longer, then calm down */
            AT(o, 0x2, s16) += 150;
            if ((u32)(s32)AT(o, 0x2, s16) >= 451) {
                AT(o, 0x2, s16) = 450;
            }
            AT(o, 0x0, u8) = 5;
        }
        /* fallthrough */
    case 5:
        AT(o, 0x2, s16)--;
        if (AT(o, 0x2, s16) < 0) {   /* over */
            AT(o, 0x0, u8) = 0;
            AT(o, 0x4, f32) = 50.0f;
            AT(o, 0x1C, f32) = 50.0f;
            AT(o, 0xC, f32) = 0.0f;
            return;
        }
        if (AT(o, 0x0, u8) != 4) {
            AT(o, 0x40, s32) = 0x90;
            VCALL(cam, 0x6C, void (*)(VObject *, f32))(cam, kShake.f);
        } else {
            AT(o, 0x40, s32) = 0x50;
        }
        if (AT(o, 0x30, s32) != 0) {
            return;
        }
        AT(o, 0x30, s32) = (6 - AT(o, 0x0, u8)) * 7;
        AT(o, 0x38, s32) = 0;
        func_002EF2B0(o);
        return;
    }
}

extern u8 *gCharPursuer;
extern f32 D_0041A0A0[];   /* fear per pursuer kind and distance band (5 per kind) */
extern f32 D_0041A0F0[];   /* the same by height difference while both are on stairs (6) */
extern s32 func_00177620(Progress *p);
extern s32 func_00177A20(Progress *p, s32 a, s32 b);

/* the pursuer kinds: 0 / 1 / 2 / 3 (-1: none that frightens) */
static s32 pursuer_kind(u8 id) {
    switch (id) {
    case 0x05:
        return 3;
    case 0x04:
        return 2;
    case 0x24: case 0x23: case 0x22: case 0x03:
        return 1;
    case 0x1B: case 0x07: case 0x06: case 0x02:
        return 0;
    }
    return -1;
}

/* a fear amount from a table: negative means a scare (+50 at once, 30 frames of hold) */
static void fear_add(u8 *o, f32 f) {
    static const F32Bits kRate = {0x3D08882F};

    if (f < 0.0f) {
        AT(o, 0x8, s16) = 30;
        AT(o, 0x20, f32) = AT(o, 0x20, f32) + 50.0f;
        AT(o, 0x10, f32) = AT(o, 0x10, f32) + 50.0f;
    } else if (!(f < 0.0f)) {
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + kRate.f * f;
    }
}

/* this frame's fear inputs: calming with time, and the pursuer: by its kind and how near it is
 * (distance + 3 * height difference; on stairs by height alone) */
void func_002EFBE0(u8 *o) {
    static const F32Bits kCalm0 = {0x3DCCCC46}, kCalm = {0x3D08882F}, kRecover = {0x3E2AAA3B},
                         kAway = {0x3D88882F};
    Progress *p = gProgress;
    u8 *pl, *pu;
    f32 v[4] __attribute__((aligned(16)));
    f32 dy, d;
    s32 kind, band;

    if ((u8)func_00177620(p) == 0) {
        AT(o, 0x2C, f32) = AT(o, 0x2C, f32) + kCalm0.f;
    } else if (AT(gCharPlayer, 0xF8, s32) == 0 && AT(gCharPlayer, 0xFC, s32) == 0) {
        AT(o, 0x2C, f32) = AT(o, 0x2C, f32) + kCalm.f;
    }
    if (AT(o, 0x8, s16) == 0) {
        AT(o, 0x18, f32) = AT(o, 0x18, f32) + kRecover.f;
    }
    pu = gCharPursuer;
    if (pu == NULL || !AT(pu, 0x28, u8) || AT(pu, 0xC4, s32) == 2 || gCharPlayer == NULL ||
        !AT(gCharPlayer, 0x28, u8)) {
        return;
    }
    if (AT(gCharPlayer, 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p) &&
        AT(gCharPlayer, 0x30, s32) == AT(gCharPursuer, 0x30, s32)) {
        /* (Hewie controlled) she is with the pursuer elsewhere */
        if (AT(p, 0x1FBEC1, u8) == 1) {
            AT(o, 0x24, f32) = AT(o, 0x24, f32) + kAway.f;
        }
        return;
    }
    if (AT(gCharPursuer, 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return;
    }
    if ((u8)Progress_TestFlag(p, 0x21) == 1) {
        return;
    }
    pl = gCharPlayer;
    pu = gCharPursuer;
    sceVu0SubVector(v, (f32 *)(pu + 0x10), (f32 *)(pl + 0x10));
    dy = v[1];
    d = __builtin_sqrtf(v[2] * v[2] + v[0] * v[0]);
    if (dy <= 0.0f) {
        dy = -dy;
    }
    d = d + 3.0f * dy;
    kind = pursuer_kind(AT(gCharPursuer, 0x153C, u8));
    if (kind < 0) {
        return;
    }
    if (!(d < 100.0f)) {
        band = -1;
    } else if (d < 10.0f) {
        band = 0;
    } else if (d < 20.0f) {
        band = 1;
    } else if (d < 40.0f) {
        band = 2;
    } else if (d < 60.0f) {
        band = 3;
    } else {
        band = 4;
    }
    if (band >= 0) {
        fear_add(o, D_0041A0A0[kind * 5 + band]);
    }
    if (AT(gCharPlayer, 0xF8, s32) != 3 || AT(gCharPlayer, 0xFC, s32) != 7 ||
        AT(gCharPursuer, 0xF8, s32) != 3 || AT(gCharPursuer, 0xFC, s32) != 7) {
        return;
    }
    if (!((u8)func_00177A20(p, AT(gCharPlayer, 0x100, u8), AT(gCharPursuer, 0x20, u8)) & 1)) {
        return;
    }
    if (!(dy < 100.0f)) {
        band = -1;
    } else if (dy < 20.0f) {
        band = 0;
    } else if (dy < 30.0f) {
        band = 1;
    } else if (dy < 40.0f) {
        band = 2;
    } else if (dy < 50.0f) {
        band = 3;
    } else if (dy < 60.0f) {
        band = 4;
    } else {
        band = 5;
    }
    if (band >= 0) {
        fear_add(o, D_0041A0F0[kind * 6 + band]);
    }
}
