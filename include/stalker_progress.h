#ifndef STALKER_PROGRESS_H
#define STALKER_PROGRESS_H

/* stalker_progress.c: what other files call. */
#include "common.h"

typedef struct Progress Progress;

/* stalker_progress.c */
extern void func_001777D0(Progress *p, u32 slot);   /* cancelled */
extern void func_001777F0(Progress *p, u32 slot);   /* accepted */
extern u32 func_00177810(Progress *p, u32 slot);   /* its event type (u8) */
extern u32 func_00177830(Progress *p, u32 slot);   /* its kind (u8) */
extern u32 func_00177850(Progress *p, u32 slot);   /* its partner's slot (u8) */
extern s32 func_00177890(Progress *p, s32 kind, s32 arg, u8 other, u8 slot, s32 n, f32 f);   /* u8 */
extern void func_001779C0(Progress *p, u32 room, u32 slot);   /* the ladder let go */
extern void func_001779F0(Progress *p, u32 room, u32 slot);   /* mark item seen by `slot` */
extern s32 func_00177AB0(void *p, s32 kind, u8 slot);
extern u32 func_00177BF0(Progress *p, u32 i, u32 slot);   /* returns u8 flags */
extern s32 func_00177A20(Progress *p, s32 room, s32 slot);   /* returns u8 flags */
extern void func_00178070(Progress *p, u32 slot, s32 a2, s32 a3, s32 a4, s32 a5, f32 f);
extern s32 func_00178750(Progress *p, s32 room, s32 exit);
extern s32 func_00178840(Progress *p, s32 room, s32 exit);   /* u8 */
extern s32 func_00178A90(Progress *p, s32 room, s32 exit, u32 slot);   /* door is shut */
extern s32 func_00178C10(Progress *p, s32 room, s32 exit, u32 slot);   /* door is open */
extern u32 func_00178DB0(Progress *p, s32 room, s32 exit, u32 side);   /* u8: the door won't let her */
extern s32 func_002EC410(u8 *t);
extern void func_002EF9E0(u8 *o, f32 amount);   /* the threat meter raised */

#endif /* STALKER_PROGRESS_H */
