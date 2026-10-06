#ifndef CHAINPOOL_H
#define CHAINPOOL_H

/* chainpool.c: what other files call. */
#include "common.h"

/* chainpool.c */
extern void func_00179EA0(u8 *pool);
extern void func_00179BC0(u8 *pool, u8 *chain);   /* free into D_004562B0 */
extern u8 *func_00179CD0(u8 *pool, u32 n);   /* allocate a chain of n entries */

#endif /* CHAINPOOL_H */
