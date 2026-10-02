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
