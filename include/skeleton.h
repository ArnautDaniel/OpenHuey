#ifndef SKELETON_H
#define SKELETON_H

/* skeleton.c: what other files call. */
#include "common.h"

/* skeleton.c */
extern void func_0017D220(u8 *pool);
extern s32 func_0017CE60(u8 *node, f32 *parent);
extern f32 *func_0017CE80(u8 *skel, s32 bone);   /* bone matrix */
extern u8 *func_0017D000(u8 *pool, u32 nBones);   /* allocate a skeleton */
extern void func_0017CED0(u8 *pool, u8 *skel);   /* free a skeleton */

#endif /* SKELETON_H */
