#ifndef ROOM_MAP_H
#define ROOM_MAP_H

/* room_map.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* room_map.c */
extern void func_0021B040(u8 *rooms);
extern void func_0021ABC0(u8 *list);   /* the obstacles released */
extern VObject *func_0021B0F0(VObject *r, s32 flags);
extern void func_0021AFD0(void *list);
extern void func_0021AC10(void *list);
extern void *func_0021A370(void *o, s32 flags);
extern void *func_0021A2E0(u8 *l, s32 flags);

#endif /* ROOM_MAP_H */
