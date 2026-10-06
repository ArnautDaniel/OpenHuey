#ifndef CHAINPOOL_H
#define CHAINPOOL_H

/* chainpool.c: what other files call. */
#include "common.h"

/* chainpool.c */
extern void ChainPool_FreeAll(u8 *pool);
extern void ChainPool_Free(u8 *pool, u8 *chain);   /* free into D_004562B0 */
extern u8 *ChainPool_Alloc(u8 *pool, u32 n);   /* allocate a chain of n entries */

#endif /* CHAINPOOL_H */
