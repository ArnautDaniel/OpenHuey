/* Block heap (vtable 0x46A1C0): the scene heap (Game.sceneHeap) and the scenes' sub-heaps. A
 * table of blocks in address order covers the heap's memory; allocations are rounded up to 64
 * bytes; freeing merges with free neighbours. */
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

extern void *D_0046A1C0[], *D_004699E0[];
extern void func_00100490(void *p);   /* operator delete */

/* +0x8 destructor */
Heap *func_00168C20(Heap *h, s32 flags) {
    if (h != NULL) {
        h->vtbl = D_0046A1C0;
        h->vtbl = D_004699E0;
        if ((s16)flags > 0) {
            func_00100490(h);
        }
    }
    return h;
}

/* +0xC init: one free block covering everything, the rest empty at the end */
void func_001691C0(Heap *h) {
    u32 i;

    h->blocks[0].used = 0;
    h->blocks[0].id = 0;
    h->blocks[0].addr = h->base;
    h->blocks[0].size = h->size;
    for (i = 1; i < h->count; i++) {
        h->blocks[i].used = 0;
        h->blocks[i].id = i;
        h->blocks[i].addr = h->base + h->size;
        h->blocks[i].size = 0;
    }
}

/* +0x10 allocate `size` bytes (64-byte multiples): an exact fit, else split the first larger
 * free block (the rest goes to a free next block or a new one). NULL if nothing fits. */
u8 *func_00168FC0(Heap *h, u32 size) {
    u32 n = (size & ~0x3F) + ((size & 0x3F) ? 0x40 : 0);
    u32 i, j, rest;
    s32 carryUsed;
    u8 *carryAddr;
    u32 carrySize;

    for (i = 0; i < h->count; i++) {
        if (h->blocks[i].used != 1 && n == h->blocks[i].size) {
            h->blocks[i].used = 1;
            return h->blocks[i].addr;
        }
    }
    for (i = 0; i < h->count; i++) {
        if (h->blocks[i].used == 1 || n >= h->blocks[i].size) {
            continue;
        }
        h->blocks[i].used = 1;
        rest = h->blocks[i].size - n;
        h->blocks[i].size = n;
        if (i + 1 < h->count && h->blocks[i + 1].used == 0) {
            h->blocks[i + 1].size += rest;
            h->blocks[i + 1].addr -= rest;
            return h->blocks[i].addr;
        }
        /* insert a free block for the rest, shifting the later ones down */
        carryUsed = 0;
        carryAddr = n + h->blocks[i].addr;
        carrySize = rest;
        for (j = i + 1; j < h->count; j++) {
            s32 u = h->blocks[j].used;
            u8 *a = h->blocks[j].addr;
            u32 s = h->blocks[j].size;

            h->blocks[j].used = carryUsed;
            carryUsed = u;
            h->blocks[j].addr = carryAddr;
            carryAddr = a;
            h->blocks[j].size = carrySize;
            carrySize = s;
        }
        return h->blocks[i].addr;
    }
    return NULL;
}

/* move blocks [from + by, count) up by `by` (no ids) */
static inline void Heap_Close(Heap *h, u32 from, u32 by) {
    u32 j;

    for (j = from; j < h->count - by; j++) {
        h->blocks[j].used = h->blocks[j + by].used;
        h->blocks[j].addr = h->blocks[j + by].addr;
        h->blocks[j].size = h->blocks[j + by].size;
    }
}

/* +0x14 free the block at `addr`, merging it with free neighbours */
void func_00168C80(Heap *h, u8 *addr) {
    HeapBlock *b;
    s32 prev, next, mode;
    u32 i;

    for (i = 0; i < h->count; i++) {
        b = &h->blocks[i];
        if (b->used == 0) {
            continue;
        }
        if (addr < b->addr) {
            return;
        }
        if (b->addr == addr) {
            break;
        }
    }
    if (i >= h->count) {
        return;
    }
    b->used = 0;
    if (i == 0) {
        if (b[1].used == 1) {
            return;
        }
        mode = 0;
    } else if (i == h->count - 1) {
        if (b[-1].used == 1) {
            return;
        }
        mode = 1;
    } else {
        prev = b[-1].used;
        next = b[1].used;
        if (prev == 1 && next == 1) {
            return;
        }
        if (prev == 1 && next == 0) {
            mode = 0;
        } else if (prev == 0 && next == 1) {
            mode = 1;
        } else {
            mode = 2;
        }
    }
    switch (mode) {
    case 0:   /* take in the next block */
        b->size += b[1].size;
        Heap_Close(h, i + 1, 1);
        break;
    case 1:   /* go into the previous block */
        b[-1].size += b->size;
        Heap_Close(h, i, 1);
        break;
    case 2:   /* previous + this + next */
        b[-1].size += b->size + b[1].size;
        Heap_Close(h, i, 2);
        h->blocks[h->count - 2].used = h->blocks[h->count - 1].used;
        h->blocks[h->count - 2].addr = h->blocks[h->count - 1].addr;
        h->blocks[h->count - 2].size = h->blocks[h->count - 1].size;
        break;
    }
}

/* operator delete for objects placed in the scene heap: nothing (the heap is freed as a whole) */
void func_0011F9A0(void *p) {
}
