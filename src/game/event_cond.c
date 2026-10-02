/* The event script conditions (0x00..0x65; vt +0x10 evaluates the one at the pc, +0x14 steps
 * over it). Operands are big-endian. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "navmesh.h"
#include "sce/libvu0.h"

extern void *gCharacters[6];
extern u8 *gCharPlayer;
extern u8 *gCharPartner;
extern s32 func_001770D0(Progress *p, s32 id);
extern u8 D_003D72C0[];   /* the conditions' lengths */
extern u8 *D_0044E978;    /* resident data (+0x24: flags kept across games) */

#define PC(ev) AT(ev, 0x4, u8 *)

static inline u32 be16(const u8 *p) {
    return (p[0] << 8 | p[1]) & 0xFFFF;
}

/* +0x14: step over the condition at the pc (0x11 carries a string: 3 + its length) */
void func_001FC700(VObject *ev) {
    u8 *pc = PC(ev);

    if (pc[0] == 0x11) {
        PC(ev) = pc + pc[2] + 3;
    } else {
        PC(ev) = PC(ev) + D_003D72C0[pc[0]];
    }
}

extern f32 func_002E2D00(f32 angle);          /* wrapped into -pi..pi */
extern f32 func_0031C5C0(f32 x, f32 z);       /* heading of (x, z) */
s32 func_001FC390(VObject *ev, u8 *c, s32 area);
extern s32 func_00177620(Progress *p);           /* the game mode */
extern s32 func_001FBF70(VObject *ev, s32 id);   /* the step slot for id */
extern s32 func_00176D80(Progress *p, s32 item);
extern s32 func_00176DD0(Progress *p, s32 item, s32 n);
extern s32 func_001241F0(void *a, void *b, f32 margin, f32 vmargin);   /* a and b close */
extern VObject *D_0044E550;   /* random numbers: +0x18 -> 0..1 */
extern s32 func_002DE1C0(u8 *zone, u8 *c);   /* character in a zone */
extern s32 func_002DE2F0(u8 *zone, f32 *p, f32 r, f32 h);   /* a point against a zone (bits) */
extern s32 func_0019A2B0(u8 *c);             /* the player can be controlled */
extern u8 *D_0044F808;                        /* the stalker in play */

/* the character with script id `id` if it is active (+0x28), else NULL */
static u8 *cond_char(Progress *p, s32 id) {
    u8 i = (u8)func_001770D0(p, id);
    u8 *c = i != 0xFF ? (u8 *)gCharacters[i] : NULL;

    return c != NULL && AT(c, 0x28, u8) == 1 ? c : NULL;
}

/* radians to degrees */
static f32 cond_deg(f32 a) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};

    return 180.0f * a / kPi.f;
}

/* +0x10: evaluate the condition at the pc (and step over it) */
s32 func_001FC760(VObject *ev) {
    Progress *p = gProgress;
    const u8 *pc = PC(ev);
    u8 r = 0;

    switch (pc[0]) {
    case 0x00: {   /* progress flag set */
        u32 f = be16(pc + 1);

        if (AT(p, 0x1C + (f >> 5) * 4, u32) & (1 << (f & 0x1F))) {
            r = 1;
        }
        break;
    }
    case 0x60: {   /* resident flag set */
        u32 f = be16(pc + 1);

        r = (AT(D_0044E978, 0x24 + (f >> 5) * 4, u32) & (1 << (f & 0x1F))) != 0;
        break;
    }
    case 0x04:   /* the exit taken (event +0x702) */
        r = AT(ev, 0x702, u8) == pc[1];
        break;
    case 0x17:   /* the script's character is pc[1] (0xFE: the stalker, if active) */
        if (pc[1] == 0xFE) {
            u8 i = (u8)func_001770D0(p, 0xFE);
            u8 *c = i != 0xFF ? (u8 *)gCharacters[i] : NULL;
            u8 id;

            if (c == NULL || AT(c, 0x28, u8) != 1) {
                break;
            }
            id = AT(*AT(ev, 0x6FC, u8 **), 0x153C, u8);
            i = (u8)func_001770D0(p, 0xFE);
            c = i != 0xFF ? (u8 *)gCharacters[i] : NULL;
            if (c == NULL || AT(c, 0x28, u8) != 1) {
                c = NULL;
            }
            r = AT(c, 0x153C, u8) == id;
        } else {
            r = AT(AT(ev, 0x6FC, u8 *), 0x13, u8) == pc[1];
        }
        break;
    case 0x16: {   /* character pc[1] is active and in this room */
        u8 i = (u8)func_001770D0(p, pc[1]);
        u8 *c = i != 0xFF ? (u8 *)gCharacters[i] : NULL;

        r = c != NULL && AT(c, 0x28, u8) && AT(c, 0x30, s32) == AT(ev, 0x560, s32);
        break;
    }
    case 0x01: {   /* character pc[1] is in this room, inside area pc[2] */
        u8 *c = cond_char(p, pc[1]);

        if (c != NULL && AT(ev, 0x560, s32) == AT(c, 0x30, s32) &&
            (u8)VCALL(ev, 0xD8, s32 (*)(VObject *, f32 *, s32, s32))(ev, (f32 *)(c + 0x10), PC(ev)[2], -1) == 1) {
            r = 1;
        }
        break;
    }
    case 0x02:     /* character pc[1] (in this room) entered area pc[2] */
    case 0x03:     /* ... left it */
        if (cond_char(p, pc[1]) != NULL && AT(cond_char(p, PC(ev)[1]), 0x30, s32) == AT(ev, 0x560, s32)) {
            s32 x = (s8)func_001FC390(ev, cond_char(p, PC(ev)[1]), PC(ev)[2]);

            r = x == (pc[0] == 0x02 ? 1 : -1);
        }
        break;
    case 0x05:     /* character pc[1] faces pc[2] x 2 degrees, within pc[3] */
        if (cond_char(p, pc[1]) != NULL) {
            f32 d = cond_deg(AT(cond_char(p, PC(ev)[1]), 0x54, f32)) - (f32)((s8)PC(ev)[2] * 2);

            if (d <= 0.0f) {
                d = -d;
            }
            if (!(d <= 180.0f)) {
                d = 360.0f - d;
            }
            r = d <= (f32)PC(ev)[3];
        }
        break;
    case 0x06: {   /* character pc[1] is inside area pc[2] and faces its middle, within pc[3] degrees */
        u8 *c = cond_char(p, pc[1]);

        if (c != NULL &&
            (u8)VCALL(ev, 0xD8, s32 (*)(VObject *, f32 *, s32, s32))(ev, (f32 *)(c + 0x10), PC(ev)[2], -1) == 1) {
            u32 *tbl = AT(ev, 0x10, u32 *);
            u8 *e = (u8 *)(tbl + tbl[PC(ev)[2]]);
            f32 ang = func_0031C5C0((AT(e, 0x10, f32) + AT(e, 0x30, f32)) / 2.0f - AT(c, 0x10, f32),
                                    (AT(e, 0x18, f32) + AT(e, 0x38, f32)) / 2.0f - AT(c, 0x18, f32));
            f32 d;

            if (!(cond_deg(func_002E2D00(ang - AT(c, 0x54, f32))) <= 0.0f)) {
                d = cond_deg(func_002E2D00(ang - AT(c, 0x54, f32)));
            } else {
                d = -cond_deg(func_002E2D00(ang - AT(c, 0x54, f32)));
            }
            r = d <= (f32)PC(ev)[3];
        }
        break;
    }
    case 0x0D:     /* the game mode is pc[1] */
        r = pc[1] == (u8)func_00177620(p);
        break;
    case 0x0E:     /* 0xF0..0xFA: that step slot runs; else character pc[1] is held (+0xE0) */
        if (pc[1] >= 0xF0 && pc[1] < 0xFB) {
            r = AT(ev, 0x564 + (u8)func_001FBF70(ev, pc[1]) * 0x18, s32) != 0;
        } else if (cond_char(p, pc[1]) != NULL) {
            r = AT(cond_char(p, PC(ev)[1]), 0xE0, u8) != 0;
        }
        break;
    case 0x15:     /* an item: pc[1] bit 7 its own test, else counted against pc[2] */
        if (pc[1] & 0x80) {
            r = (u8)func_00176D80(p, pc[1]);
        } else {
            r = (u8)func_00176DD0(p, pc[1], pc[2]);
        }
        break;
    case 0x1C: {   /* a pc[1] percent chance */
        f32 pct = (f32)pc[1];

        if (!(pct < 100.0f)) {
            r = 1;
        } else if (!(pct <= 0.0f)) {
            r = 100.0f * VCALL(D_0044E550, 0x18, f32 (*)(VObject *))(D_0044E550) < pct;
        }
        break;
    }
    case 0x1D:     /* the progress state (+0x7B8) is pc[1] (0xFF: 4 or 5) */
        if (pc[1] == 0xFF) {
            r = (u8)(AT(p, 0x7B8, u8) - 4) < 2;
        } else {
            r = AT(p, 0x7B8, u8) == pc[1];
        }
        break;
    case 0x1F: {   /* character pc[1] near pc[2] (margins pc[3], pc[4]) and facing it, within pc[5] degrees */
        u8 *a = cond_char(p, pc[1]);
        u8 *b = cond_char(p, PC(ev)[2]);

        if (a != NULL && b != NULL &&
            (u8)func_001241F0(a, b, (f32)PC(ev)[3], (f32)PC(ev)[4]) == 1) {
            f32 ang = func_0031C5C0(AT(b, 0x10, f32) - AT(a, 0x10, f32), AT(b, 0x18, f32) - AT(a, 0x18, f32));
            f32 d;

            if (!(cond_deg(func_002E2D00(ang - AT(a, 0x54, f32))) <= 0.0f)) {
                d = cond_deg(func_002E2D00(ang - AT(a, 0x54, f32)));
            } else {
                d = -cond_deg(func_002E2D00(ang - AT(a, 0x54, f32)));
            }
            r = d <= (f32)PC(ev)[5];
        }
        break;
    }
    case 0x29:     /* event bit pc[1] (+0x890) */
        r = (AT(ev, 0x890, u32) & (1u << pc[1])) != 0;
        break;
    case 0x2B:     /* +0x11F0 has reached +0x40 */
        r = !(AT(ev, 0x11F0, u8) < AT(ev, 0x40, u8));
        break;
    case 0x35: {   /* character pc[1] (in this room, its radius / height) against zone pc[2]: all of
                    * pc[3]'s bits */
        u8 *c = cond_char(p, pc[1]);

        if (c != NULL && AT(ev, 0x560, s32) == AT(c, 0x30, s32)) {
            u8 bits = (u8)func_002DE2F0((u8 *)ev + 0xBF0 + PC(ev)[2] * 0x30, (f32 *)(c + 0x10), AT(c, 0xC8, f32),
                                        AT(c, 0xCC, f32));

            r = (PC(ev)[3] & bits) == PC(ev)[3];
        }
        break;
    }
    case 0x37:     /* character pc[1] is in zone pc[2] (+0xBF0) */
        r = (u8)func_002DE1C0((u8 *)ev + 0xBF0 + PC(ev)[2] * 0x30, cond_char(p, pc[1]));
        break;
    case 0x3D:     /* the player can be controlled and the progress state is below 4 */
        r = (u8)func_0019A2B0(gCharPlayer) == 1 && AT(p, 0x7B8, u8) < 4;
        break;
    case 0x3F:     /* the player's action (+0x1AD580) is be32 pc[1..4] */
        r = AT(gCharPlayer, 0x1AD580, u32) == (u32)(pc[1] << 24 | pc[2] << 16 | pc[3] << 8 | pc[4]);
        break;
    case 0x41:     /* character pc[1]'s state (+0xC4) is pc[2] */
        if (cond_char(p, pc[1]) != NULL) {
            r = AT(cond_char(p, PC(ev)[1]), 0xC4, s32) == PC(ev)[2];
        }
        break;
    case 0x49:     /* character pc[1]'s action (+0xF8) is pc[2] */
        if (cond_char(p, pc[1]) != NULL) {
            r = AT(cond_char(p, PC(ev)[1]), 0xF8, s32) == PC(ev)[2];
        }
        break;
    case 0x4B:     /* the stalker is active and its +0x10C says so */
        if (D_0044F808 != NULL && AT(D_0044F808, 0x28, u8) != 0 &&
            VCALL((VObject *)D_0044F808, 0x10C, s32 (*)(VObject *))((VObject *)D_0044F808) != 0) {
            r = 1;
        }
        break;
    case 0x64:     /* the progress' +0x1050 is 0xD */
        r = AT(p, 0x1050, u8) == 0xD;
        break;
    case 0x08:     /* progress flag be16 pc[1] */
        r = Progress_TestFlag(p, be16(pc + 1)) != 0;
        break;
    case 0x5D:   /* Hewie is the one controlled */
        r = AT(p, 0x1FBEC1, u8);
        break;
    default:
        if (pc[0] < 0x66) {
#ifdef HG_NATIVE
            extern void hg_debug_todo_cond(s32 op);

            hg_debug_todo_cond(pc[0]);
#endif
        }
        break;
    }
    VCALL(ev, 0x14, void (*)(VObject *))(ev);
    return r;
}


/* which side of the line a -> b point p is on (2-D, x / z): 1 left or on, -1 right */
static s32 line_side(const f32 *p, const f32 *a, const f32 *b) {
    return (p[0] - a[0]) * (b[2] - a[2]) - (p[2] - a[2]) * (b[0] - a[0]) < 0.0f ? -1 : 1;
}

/* how character `c` (position +0x10 on triangle +0x34, last frame's +0x40 / +0x38) crossed area
 * `area` of the room's event areas (+0x10: offsets, then entries) this frame: a region (entry
 * word 0 set): 1 entered, -1 left (the event's +0xD8 inside test); a gate (an entry's line
 * +0x10 -> +0x20, heights +0x14 .. +0x8): the side it came to when its step crossed the line
 * at a height within the gate; else 0 */
s32 func_001FC390(VObject *ev, u8 *c, s32 area) {
    u32 *tbl = AT(ev, 0x10, u32 *);
    u8 *e;
    NavMesh *nm;
    f32 prev[4] __attribute__((aligned(16)));
    f32 cur[4] __attribute__((aligned(16)));
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    s32 n[4];

    if (tbl == NULL) {
        return 0;
    }
    e = (u8 *)(tbl + tbl[area]);
    if (AT(e, 0x0, u32) != 0) {
        sceVu0CopyVector(prev, (f32 *)(c + 0x40));
        if ((u8)VCALL(ev, 0xD8, s32 (*)(VObject *, f32 *, s32, s32))(ev, (f32 *)(c + 0x10), area, -1) == 1) {
            return (u8)VCALL(ev, 0xD8, s32 (*)(VObject *, f32 *, s32, s32))(ev, prev, area, -1) ? 0 : 1;
        }
        return (u8)VCALL(ev, 0xD8, s32 (*)(VObject *, f32 *, s32, s32))(ev, prev, area, -1) == 1 ? -1 : 0;
    }
    n[0] = n[1] = n[2] = n[3] = 0;
    sceVu0CopyVector(cur, (f32 *)(c + 0x10));
    nm = D_0044E570;
    VCALL(nm, 0x14, void (*)(NavMesh *, u32, f32 *))(nm, AT(c, 0x34, u32), cur);
    sceVu0CopyVector(prev, (f32 *)(c + 0x40));
    VCALL(nm, 0x14, void (*)(NavMesh *, u32, f32 *))(nm, AT(c, 0x38, u32), prev);
    a[0] = AT(e, 0x10, f32);
    a[2] = AT(e, 0x18, f32);
    a[1] = AT(e, 0x14, f32);
    a[3] = AT(e, 0x1C, f32);
    b[2] = AT(e, 0x28, f32);
    b[3] = AT(e, 0x2C, f32);
    b[0] = AT(e, 0x20, f32);
    b[1] = AT(e, 0x24, f32);
    n[0] += line_side(cur, a, b);
    n[1] += line_side(prev, a, b);
    if (n[0] + n[1] != 0) {
        return 0;   /* both on one side */
    }
    n[2] += line_side(a, cur, prev);
    n[3] += line_side(b, cur, prev);
    if (n[2] + n[3] != 0) {
        return 0;   /* the step passes the gate's end */
    }
    cur[1] = cur[1] + 1.0f;
    prev[1] = prev[1] + 1.0f;
    if ((!(cur[1] < a[1]) && cur[1] <= AT(e, 0x8, f32)) || (!(prev[1] < a[1]) && prev[1] <= AT(e, 0x8, f32))) {
        return (s8)n[0];
    }
    return 0;
}


extern s32 func_002DE0F0(u8 *zone, u8 *c);             /* character c in the zone */

/* is character `c` in zone `zone` - or, for NULL, any active character in this room (its
 * point +0x74 fully inside: bits 1 and 2) */
s32 func_002DE1C0(u8 *zone, u8 *c) {
    f32 pt[4] __attribute__((aligned(16)));
    Progress *p;
    u8 i;

    if (c != NULL) {
        return func_002DE0F0(zone, c);
    }
    p = gProgress;
    for (i = 0; i < 6; i++) {
        u8 *k = gCharacters[i];

        if (k != NULL && AT(k, 0x28, u8) != 0 &&
            AT(k, 0x30, s32) == VCALL(p, 0xC, s32 (*)(Progress *))(p) &&
            (u8)VCALL(k, 0x74, s32 (*)(u8 *, f32 *))(k, pt) &&
            ((u8)func_002DE2F0(zone, pt, 0.0f, 0.0f) & 3) == 3) {
            return 1;
        }
    }
    return 0;
}
