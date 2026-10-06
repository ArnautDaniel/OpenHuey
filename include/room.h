#ifndef ROOM_H
#define ROOM_H

/* meshdraw.c: what other files call. */
#include "common.h"

/* meshdraw.c */
extern s32 PlacedMesh_LastShown(u8 *o);   /* the drawn object was on screen */
extern void RoomMesh_Reset(u8 *o);

/* room.c */
extern void *PlacedObjects_ctor(u8 *p);
extern void RoomMgr_Clear(u8 *m);
extern void RoomMgr_ctor(u8 *m);
extern void RoomMgr_LoadRoom(u8 *rm, u32 room, s32 slot);
extern s32 RoomMgr_SlotLoading(u8 *rm, s32 slot);   /* room slot still loading */
extern void RoomMgr_MakeCurrent(u8 *rm, s32 slot);
extern void PlacedObject_ToDef(u8 *o);
extern void PlacedObject_StartAnim(u8 *o, s32 id);
extern void RoomMgr_Draw(u8 *rm, s32 slot);
extern void RoomMgr_Leave(u8 *rm);
extern void RoomMgr_Update(u8 *rm);
extern s32 RoomMgr_SlotLoaded(u8 *o, s32 k);
extern void RoomMgr_LoadSlot(u8 *o, s32 k);

#endif /* ROOM_H */
