/* The chain pool (D_004562B0), the skeleton pool's sibling for motion data: 64 chains
 * (12 bytes: ?, first entry, entry count) and 0x14-byte entries (+0x300), each with a use
 * bitmap (+0x2718 chains, +0x2720 entries). */
#include "common.h"

/* free everything */
void func_00179EA0(u8 *pool) {
    s32 i;

    for (i = 0; i < 17; i++) {
        AT(pool, 0x2718 + i * 4, s32) = 0;
    }
}

/* Entries: +0x4..+0xC data, +0x10 next. */

/* the chain's entry `n` (NULL past the end) */
u8 *func_00179F10(u8 *chain, s32 n) {
    u8 *e = AT(chain, 4, u8 *);
    s32 i = 0;

    while (e != NULL) {
        if (i == n) {
            return e;
        }
        e = AT(e, 0x10, u8 *);
        i++;
    }
    return NULL;
}

/* free a chain and its entries (NULL: nothing) */
void func_00179BC0(u8 *pool, u8 *chain) {
    s32 i;
    u32 n;

    if (chain == NULL) {
        return;
    }
    if (AT(chain, 4, u8 *) != NULL) {
        for (i = AT(chain, 8, s32) - 1; i >= 0; i--) {
            u8 *e = func_00179F10(chain, i);

            AT(e, 0x10, s32) = 0;
            AT(e, 0xC, s32) = 0;
            AT(e, 0x4, s32) = 0;
            AT(e, 0x8, s32) = 0;
            n = (u32)(e - (pool + 0x300)) / 0x14;
            AT(pool, 0x2720 + (n >> 5) * 4, u32) &= ~(1 << (n & 0x1F));
        }
    }
    AT(chain, 0, s32) = 0;
    AT(chain, 4, s32) = 0;
    AT(chain, 8, s32) = 0;
    n = (u32)(chain - pool) / 12;
    AT(pool, 0x2718 + (n >> 5) * 4, u32) &= ~(1 << (n & 0x1F));
}
