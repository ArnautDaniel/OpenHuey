#ifndef ROOM_MAP_H
#define ROOM_MAP_H

/* room_map.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* room_map.c */
extern void Obstacles_Reset(u8 *rooms);
extern void Obstacles_Release(u8 *list);   /* the obstacles released */
extern VObject *Rooms_dtor(VObject *r, s32 flags);
extern void Obstacles_Update(void *list);
extern void Obstacles_ModelsFollow(void *list);
extern void *Obstacle_dtor(void *o, s32 flags);
extern void *Obstacles_dtor(u8 *l, s32 flags);

#endif /* ROOM_MAP_H */
