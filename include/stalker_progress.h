#ifndef STALKER_PROGRESS_H
#define STALKER_PROGRESS_H

/* stalker_progress.c: what other files call. */
#include "common.h"

typedef struct Progress Progress;

/* stalker_progress.c */
extern void SlotCmd_Cancel(Progress *p, u32 slot);   /* cancelled */
extern void SlotCmd_Start(Progress *p, u32 slot);   /* accepted */
extern u32 SlotCmd_Arg(Progress *p, u32 slot);   /* its event type (u8) */
extern u32 SlotCmd_Kind(Progress *p, u32 slot);   /* its kind (u8) */
extern u32 SlotCmd_Target(Progress *p, u32 slot);   /* its partner's slot (u8) */
extern s32 SlotCmd_Give(Progress *p, s32 kind, s32 arg, u8 other, u8 slot, s32 n, f32 f);   /* u8 */
extern void RoomSlots_Leave(Progress *p, u32 room, u32 slot);   /* the ladder let go */
extern void RoomSlots_Enter(Progress *p, u32 room, u32 slot);   /* mark item seen by `slot` */
extern s32 PursuerGroup_Find(void *p, s32 kind, u8 slot);
extern u32 PursuerGroup_Fields(Progress *p, u32 i, u32 slot);   /* returns u8 flags */
extern s32 RoomSlots_Bytes(Progress *p, s32 room, s32 slot);   /* returns u8 flags */
extern void Relation_Request(Progress *p, u32 slot, s32 a2, s32 a3, s32 a4, s32 a5, f32 f);
extern s32 DoorHold_Release(Progress *p, s32 room, s32 exit);
extern s32 DoorHold_Usable(Progress *p, s32 room, s32 exit);   /* u8 */
extern s32 DoorHold_Shut(Progress *p, s32 room, s32 exit, u32 slot);   /* door is shut */
extern s32 DoorHold_Open(Progress *p, s32 room, s32 exit, u32 slot);   /* door is open */
extern u32 DoorHold_Take(Progress *p, s32 room, s32 exit, u32 side);   /* u8: the door won't let her */
extern s32 Countdown_Seconds(u8 *t);
extern void Threat_Raise(u8 *o, f32 amount);   /* the threat meter raised */

#endif /* STALKER_PROGRESS_H */
