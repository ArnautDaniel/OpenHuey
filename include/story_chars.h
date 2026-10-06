#ifndef STORY_CHARS_H
#define STORY_CHARS_H

#include "common.h"

typedef struct Pursuer Pursuer;

/* story_chars.c */
extern Pursuer *Kind22_dtor(Pursuer *p, s32 flags);
extern void *Kind33_ctor(void *p, s32 arg);
extern void *Kind32_ctor(void *p, s32 arg);
extern void *Kind31_ctor(void *p, s32 arg);
extern void *Kind30_ctor(void *p, s32 arg);
extern void *Kind29_ctor(void *p, s32 arg);
extern void *Kind28_ctor(void *p, s32 arg);
extern void *Kind26_ctor(void *p, s32 arg);
extern void *Kind25_ctor(void *p, s32 arg);
extern void *Kind24_ctor(void *p, s32 arg);
extern void *Kind22_ctor(void *p, u32 id, u32 arg);
extern void *Kind21_ctor(void *p, s32 arg, u32 id);
extern void *Kind20_ctor(void *p, s32 arg);
extern void *Kind19_ctor(void *p, s32 arg);
extern void *Kind18_ctor(void *p, s32 arg);
extern void *Kind17_ctor(void *p, s32 arg);
extern void *Kind16_ctor(void *p, s32 arg);
extern void *Kind14_ctor(void *p, s32 arg, u32 id);
extern void *Kind13_ctor(void *p, s32 arg);
extern void *Kind08_ctor(void *p, s32 arg);
extern void *Kind09_ctor(void *p, s32 arg);
extern u8 *Kind09_ModelFileTable(Pursuer *p);
extern void Kind14_Setup(Pursuer *p);

#endif /* STORY_CHARS_H */
