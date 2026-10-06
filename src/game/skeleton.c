/* The skeleton pool (gSkelPool): 32 skeletons (12 bytes: ?, first node, node count) and 632
 * 0x50-byte bone nodes (+0x180), each with a use bitmap (+0xC700 skeletons, +0xC704 nodes). */
#include "common.h"
#include "skeleton.h"

/* free everything */
/* 0x0017D220 */
void SkelPool_FreeAll(u8 *pool) {
    s32 i;

    for (i = 0; i < 21; i++) {
        AT(pool, 0xC700 + i * 4, s32) = 0;
    }
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

/* the skeleton's bone `bone` (its matrix; NULL past the end) */
/* 0x0017CE80 */
f32 *Skel_Bone(u8 *skel, s32 bone) {
    u8 *node = AT(skel, 4, u8 *);
    s32 i = 0;

    while (node != NULL) {
        if (i == bone) {
            return (f32 *)node;
        }
        node = AT(node, 0x48, u8 *);
        i++;
    }
    return NULL;
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
