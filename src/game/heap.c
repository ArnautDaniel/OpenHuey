/* Block heap (vtable 0x46A1C0): the scene heap (Game.sceneHeap) and the scenes' sub-heaps. A
 * table of blocks in address order covers the heap's memory; allocations are rounded up to 64
 * bytes; freeing merges with free neighbours. */
#include "common.h"
#include "ptmf.h"
#include "heap.h"
#include "msl.h"

extern void *Heap_vtable[], *D_004699E0[];

/* +0x8 destructor */
/* 0x00168C20 */
Heap *Heap_dtor(Heap *h, s32 flags) {
    if (h != NULL) {
        h->vtbl = Heap_vtable;
        h->vtbl = D_004699E0;
        if ((s16)flags > 0) {
            func_00100490(h);
        }
    }
    return h;
}

/* +0xC init: one free block covering everything, the rest empty at the end */
/* 0x001691C0 */
void Heap_Init(Heap *h) {
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
/* 0x00168FC0 */
u8 *Heap_Alloc(Heap *h, u32 size) {
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
/* 0x00168C80 */
void Heap_Free(Heap *h, u8 *addr) {
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
/* 0x0011F9A0 */
void SceneHeap_delete(void *p) {
}

/* ---- fixed-size block pool (vtable BlockPool_vtable; base D_004699E0): n blocks of one size,
 * a used flag each ---- */

extern void *BlockPool_vtable[];
extern void *D_004699E0[];

/* +0x8 */
/* 0x00120D00 */
BlockPool *BlockPool_dtor(BlockPool *p, s32 flags) {
    if (p != NULL) {
        p->vtbl = BlockPool_vtable;
        if (p != NULL) {
            p->vtbl = D_004699E0;
        }
        if ((s16)flags > 0) {
            func_00100490(p);
        }
    }
    return p;
}

/* +0xC free everything */
/* 0x00120E80 */
void BlockPool_FreeAll(BlockPool *p) {
    u8 *used = p->used;
    u32 i;

    for (i = 0; i < p->n; i++) {
        *used++ = 0;
    }
}

/* +0x10 a block of `size` bytes (the pool's size only), NULL if none is free */
/* 0x00120E10 */
void *BlockPool_Alloc(BlockPool *p, u32 size) {
    u8 *used;
    u32 i;

    if (size != p->size) {
        return NULL;
    }
    used = p->used;
    for (i = 0; i < p->n; i++, used++) {
        if (*used == 0) {
            *used = 1;
            return p->base + i * size;
        }
    }
    return NULL;
}

/* +0x14 free block `b` */
/* 0x00120DB0 */
void BlockPool_Free(BlockPool *p, u8 *b) {
    u8 *a = p->base, *used = p->used;
    u32 i;

    for (i = 0; i < p->n; i++) {
        if (a == b) {
            if (*used) {
                *used = 0;
            }
            return;
        }
        used++;
        a += p->size;
    }
}

/* set up over `n` blocks of `size` bytes at `base`, flags at `used`; all free */
/* 0x00120EC0 */
void BlockPool_Init(BlockPool *p, u8 *base, u32 size, u32 n, u8 *used) {
    p->size = size;
    p->n = n;
    p->used = used;
    p->base = base;
    p->total = p->size * p->n;
    VCALL(p, 0xC, void (*)(BlockPool *))(p);
}
