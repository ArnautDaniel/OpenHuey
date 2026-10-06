/* The chain pool (gChainPool), the skeleton pool's sibling for motion data: 64 chains
 * (12 bytes: ?, first entry, entry count) and 0x14-byte entries (+0x300), each with a use
 * bitmap (+0x2718 chains, +0x2720 entries). */
#include "common.h"
#include "chainpool.h"

/* free everything */
/* 0x00179EA0 */
void ChainPool_FreeAll(u8 *pool) {
    s32 i;

    for (i = 0; i < 17; i++) {
        AT(pool, 0x2718 + i * 4, s32) = 0;
    }
}

/* Entries: +0x4..+0xC data, +0x10 next. */

/* the chain's entry `n` (NULL past the end) */
/* 0x00179F10 */
u8 *Chain_Entry(u8 *chain, s32 n) {
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
/* 0x00179BC0 */
void ChainPool_Free(u8 *pool, u8 *chain) {
    s32 i;
    u32 n;

    if (chain == NULL) {
        return;
    }
    if (AT(chain, 4, u8 *) != NULL) {
        for (i = AT(chain, 8, s32) - 1; i >= 0; i--) {
            u8 *e = Chain_Entry(chain, i);

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

/* link entry `e` after `prev` (0: prev already has a next) */
/* 0x00179EE0 */
s32 Chain_Link(u8 *prev, u8 *e) {
    if (AT(prev, 0x10, u8 *) != NULL) {
        return 0;
    }
    AT(prev, 0x10, u8 *) = e;
    return 1;
}

/* allocate a chain of `n` linked entries (NULL: no chain free; a short chain if the entries run
 * out) */
/* 0x00179CD0 */
u8 *ChainPool_Alloc(u8 *pool, u32 n) {
    u8 *chain = NULL;
    u8 *first = NULL;   /* (left unset by the original for 0 entries) */
    u8 *e = NULL;
    u32 i;
    s32 j;

    for (j = 0; j < 0x40; j++) {
        u32 *used = &AT(pool, 0x2718 + (j >> 5) * 4, u32);

        if (!(*used & (1 << (j & 0x1F)))) {
            *used |= 1 << (j & 0x1F);
            chain = pool + j * 12;
            AT(chain, 0, s32) = 0;
            AT(chain, 4, s32) = 0;
            AT(chain, 8, s32) = 0;
            break;
        }
    }
    for (i = 0; i < n; i++) {
        u8 *prev = e;

        e = NULL;
        for (j = 0; j < 0x1CE; j++) {
            u32 *used = &AT(pool, 0x2720 + (j >> 5) * 4, u32);

            if (!(*used & (1 << (j & 0x1F)))) {
                *used |= 1 << (j & 0x1F);
                e = pool + 0x300 + j * 0x14;
                AT(e, 0x10, s32) = 0;
                AT(e, 0xC, s32) = 0;
                AT(e, 0x4, s32) = 0;
                AT(e, 0x8, s32) = 0;
                break;
            }
        }
        if (e == NULL) {
            break;
        }
        if (i == 0) {
            first = e;
        } else {
            Chain_Link(prev, e);
        }
    }
    AT(chain, 4, u8 *) = first;
    AT(chain, 8, u32) = n;
    return chain;
}
