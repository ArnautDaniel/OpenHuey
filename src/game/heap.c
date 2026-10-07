/* Block heap (vtable 0x46A1C0): the scene heap (Game.sceneHeap) and the scenes' sub-heaps. A
 * table of blocks in address order covers the heap's memory; allocations are rounded up to 64
 * bytes; freeing merges with free neighbours.
 *
 * (was chainpool.c) The chain pool (gChainPool), the skeleton pool's sibling for motion data: 64
 * chains (12 bytes: ?, first entry, entry count) and 0x14-byte entries (+0x300), each with a use
 * bitmap (+0x2718 chains, +0x2720 entries).
 *
 * (was skeleton.c) The skeleton pool (gSkelPool): 32 skeletons (12 bytes: ?, first node, node
 * count) and 632 0x50-byte bone nodes (+0x180), each with a use bitmap (+0xC700 skeletons,
 * +0xC704 nodes).
 */
#include "common.h"
#include "ptmf.h"
#include "heap.h"
#include "msl.h"
#include "game.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "memcard.h"
#include "pursuer.h"
#include "item.h"
#include "effects.h"
#include "items.h"
#include "placed.h"
#include "scene_game.h"
#include "sound.h"
#include "vecmath.h"
#include "input.h"
#include "daniella.h"
#include "loader.h"
#include "pad.h"
#include "scene.h"
#include "scene_boot.h"
#include "scene_title.h"
#include "system.h"
#include "text.h"
#include "libc.h"
#include "sce/iop.h"
#include "effectmgr.h"
#include "charaction.h"
#include "renderer.h"
#include "gl2d.h"
#include "music.h"
#include "camera.h"
#include "char_load.h"
#include "creature.h"
#include "doors.h"
#include "event.h"
#include "gameover.h"
#include "hewie.h"
#include "model.h"
#include "movie.h"
#include "fiona.h"
#include "pause.h"
#include "room_map.h"
#include "draw_leaves.h"
#include "sce/eekernel.h"
#include "cri/adx.h"
#include "subscreen.h"
#include "sce/intc.h"
#include "sce/libmc.h"
#include "sce/libpad2.h"
#include "sce/sif.h"
#include "ps2hw.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif

extern void *Heap_vtable[], *SceneTableBase_vtable[];

void ChainPool_FreeAll(u8 *pool);
u8 *Chain_Entry(u8 *chain, s32 n);
void ChainPool_Free(u8 *pool, u8 *chain);
s32 Chain_Link(u8 *prev, u8 *e);
u8 *ChainPool_Alloc(u8 *pool, u32 n);

void *BlockPool_ElemAt(B0_Pool *p, u32 i);

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *SceneHeap_ctor(u8 *p);

s32 SkelNode_Link(u8 *prev, u8 *node);

/* +0x8 destructor */
/* 0x00168C20 */
Heap *Heap_dtor(Heap *h, s32 flags) {
    if (h != NULL) {
        h->vtbl = Heap_vtable;
        h->vtbl = SceneTableBase_vtable;
        if ((s16)flags > 0) {
            __dl__FPv(h);
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

/* Heap (vtable 0x46A1C0) setup: memory, size, block table, block count; then its init (+0xC). */
/* 0x00169260 */
void Heap_Setup(VObject *h, void *base, u32 size, void *blocks, s32 count) {
    AT(h, 0x4, void *) = base;
    AT(h, 0x8, u32) = size;
    AT(h, 0xC, void *) = blocks;
    AT(h, 0x10, s32) = count;
    VCALL(h, 0xC, void (*)(VObject *, void *, u32, void *, s32))(h, base, size, blocks, count);
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

/* free everything */
/* 0x00179EA0 */
void ChainPool_FreeAll(u8 *pool) {
    s32 i;

    for (i = 0; i < 17; i++) {
        AT(pool, 0x2718 + i * 4, s32) = 0;
    }
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

/* Bone nodes: a 4x4 matrix, then +0x40 index, +0x44 parent, +0x48 next. */

/* link `node` after `prev` (0: prev already has a next) */
/* 0x0017CE30 */
s32 SkelNode_Link(u8 *prev, u8 *node) {
    if (AT(prev, 0x48, u8 *) != NULL) {
        return 0;
    }
    AT(prev, 0x48, u8 *) = node;
    return 1;
}

/* set the node's parent (0: none given) */
/* 0x0017CE60 */
s32 SkelNode_SetParent(u8 *node, f32 *parent) {
    if (parent == NULL) {
        return 0;
    }
    AT(node, 0x44, f32 *) = parent;
    return 1;
}

/* free a skeleton and its nodes (NULL: nothing) */
/* 0x0017CED0 */
void SkelPool_Free(u8 *pool, u8 *skel) {
    s32 i;
    u32 n;

    if (skel == NULL) {
        return;
    }
    if (AT(skel, 4, u8 *) != NULL) {
        for (i = AT(skel, 8, s32) - 1; i >= 0; i--) {
            u8 *node = (u8 *)Skel_Bone(skel, i);

            AT(node, 0x44, s32) = 0;
            AT(node, 0x48, s32) = 0;
            n = (u32)(node - (pool + 0x180)) / 0x50;
            AT(pool, 0xC704 + (n >> 5) * 4, u32) &= ~(1 << (n & 0x1F));
        }
    }
    AT(skel, 0, s32) = 0;
    AT(skel, 4, s32) = 0;
    n = (u32)(skel - pool) / 12;
    AT(pool, 0xC700 + (n >> 5) * 4, u32) &= ~(1 << (n & 0x1F));
}

/* allocate a skeleton of `nBones` linked nodes (NULL: none free; a short chain if the nodes
 * run out) */
/* 0x0017D000 */
u8 *SkelPool_Alloc(u8 *pool, u32 nBones) {
    u8 *skel = NULL;
    u8 *first = NULL;   /* (left unset by the original for 0 bones) */
    u8 *node = NULL;
    u32 i;
    s32 j;

    for (j = 0; j < 32; j++) {
        u32 *used = &AT(pool, 0xC700 + (j >> 5) * 4, u32);
        if (!(*used & (1 << (j & 0x1F)))) {
            *used |= 1 << (j & 0x1F);
            skel = pool + j * 12;
            AT(skel, 0, s32) = 0;
            AT(skel, 4, s32) = 0;
            break;
        }
    }
    for (i = 0; i < nBones; i++) {
        u8 *prev = node;

        node = NULL;
        for (j = 0; j < 0x278; j++) {
            u32 *used = &AT(pool, 0xC704 + (j >> 5) * 4, u32);
            if (!(*used & (1 << (j & 0x1F)))) {
                *used |= 1 << (j & 0x1F);
                node = pool + 0x180 + j * 0x50;
                AT(node, 0x44, s32) = 0;
                AT(node, 0x48, s32) = 0;
                break;
            }
        }
        if (node == NULL) {
            break;
        }
        if (i == 0) {
            first = node;
        } else {
            SkelNode_Link(prev, node);
        }
    }
    AT(skel, 4, u8 *) = first;
    AT(skel, 8, s32) = nBones;
    return skel;
}

/* free everything */
/* 0x0017D220 */
void SkelPool_FreeAll(u8 *pool) {
    s32 i;

    for (i = 0; i < 21; i++) {
        AT(pool, 0xC700 + i * 4, s32) = 0;
    }
}

/* 0x002D1580 */
void *SceneHeap_ctor(u8 *p) {
    F(p, 0x0, void *) = SceneTableBase_vtable;
    F(p, 0x4, u32) = 0;
    F(p, 0x8, u32) = 0;
    F(p, 0x0, void *) = Heap_vtable;
    F(p, 0xC, u32) = 0;
    F(p, 0x10, u32) = 0;
    return p;
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

/* ---- fixed-size block pool (vtable BlockPool_vtable; base SceneTableBase_vtable): n blocks of one size,
 * a used flag each ---- */

extern void *BlockPool_vtable[];
extern void *SceneTableBase_vtable[];

/* +0x8 */
/* 0x00120D00 */
BlockPool *BlockPool_dtor(BlockPool *p, s32 flags) {
    if (p != NULL) {
        p->vtbl = BlockPool_vtable;
        if (p != NULL) {
            p->vtbl = SceneTableBase_vtable;
        }
        if ((s16)flags > 0) {
            __dl__FPv(p);
        }
    }
    return p;
}

/* 0x00120D60 */
void *BlockPool_ElemAt(B0_Pool *p, u32 i) {
    if (i < p->count && p->used[i] != 0) {
        return p->base + i * p->elemSize;
    }
    return NULL;
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
