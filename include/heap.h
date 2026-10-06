#ifndef HEAP_H
#define HEAP_H

/* heap.c: what other files call. */
#include "common.h"
#include "common.h"

typedef struct HeapBlock {
    s32 used;
    s32 id;
    u8 *addr;
    u32 size;
} HeapBlock;

typedef struct Heap {
    void **vtbl;
    u8 *base;
    u32 size;
    HeapBlock *blocks;
    u32 count;
} Heap;

typedef struct BlockPool {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ u8 *base;
    /* 0x08 */ u32 total;    /* size * n */
    /* 0x0C */ u32 size;
    /* 0x10 */ u32 n;
    /* 0x14 */ u8 *used;
} BlockPool;

/* heap.c */
extern Heap *Heap_dtor(Heap *h, s32 flags);
extern void SceneHeap_delete(void *p);   /* delete (scene heap) */
extern void BlockPool_Init(BlockPool *p, u8 *base, u32 size, u32 n, u8 *used);   /* BlockPool init */

/* ---- (was chainpool.h) ---- */

/* chainpool.c: what other files call. */

/* chainpool.c */
extern void ChainPool_FreeAll(u8 *pool);
extern void ChainPool_Free(u8 *pool, u8 *chain);   /* free into gChainPool */
extern u8 *ChainPool_Alloc(u8 *pool, u32 n);   /* allocate a chain of n entries */

/* ---- (was skeleton.h) ---- */

/* skeleton.c: what other files call. */

/* skeleton.c */
extern void SkelPool_FreeAll(u8 *pool);
extern s32 SkelNode_SetParent(u8 *node, f32 *parent);
extern u8 *SkelPool_Alloc(u8 *pool, u32 nBones);   /* allocate a skeleton */
extern void SkelPool_Free(u8 *pool, u8 *skel);   /* free a skeleton */

typedef struct B0_Pool B0_Pool;
typedef struct VObject VObject;

/* heap.c */
extern void *BlockPool_ElemAt(B0_Pool *p, u32 i);   /* the pool's object i (NULL if free) */
extern void Heap_Setup(VObject *h, void *base, u32 size, void *blocks, s32 count);   /* heap init */

#endif /* HEAP_H */
