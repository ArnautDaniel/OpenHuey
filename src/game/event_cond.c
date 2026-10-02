/* The event script conditions (0x00..0x65; vt +0x10 evaluates the one at the pc, +0x14 steps
 * over it). Operands are big-endian. */
#include "common.h"
#include "game.h"
#include "progress.h"

extern void *gCharacters[6];
extern u8 *gCharPlayer;
extern u8 *gCharPartner;
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
