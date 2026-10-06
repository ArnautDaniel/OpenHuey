#ifndef SKELETON_H
#define SKELETON_H

/* skeleton.c: what other files call. */
#include "common.h"

/* skeleton.c */
extern void SkelPool_FreeAll(u8 *pool);
extern s32 SkelNode_SetParent(u8 *node, f32 *parent);
extern f32 *Skel_Bone(u8 *skel, s32 bone);   /* bone matrix */
extern u8 *SkelPool_Alloc(u8 *pool, u32 nBones);   /* allocate a skeleton */
extern void SkelPool_Free(u8 *pool, u8 *skel);   /* free a skeleton */

#endif /* SKELETON_H */
