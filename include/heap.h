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
extern Heap *func_00168C20(Heap *h, s32 flags);
extern void func_0011F9A0(void *p);   /* delete (scene heap) */
extern void func_00120EC0(BlockPool *p, u8 *base, u32 size, u32 n, u8 *used);   /* BlockPool init */

#endif /* HEAP_H */
