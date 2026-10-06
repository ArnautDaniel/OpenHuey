#ifndef DOORS_H
#define DOORS_H

/* doors.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* doors.c */
extern void *Door_dtor(u8 *e, s32 flags);
extern void *Doors_dtor(u8 *d, s32 flags);
extern void Doors_Release(VObject *d);   /* the doors released */
extern void Door_PlaySound(u8 *e, s32 id, s32 how);   /* a door sound */

/* doors.c */
extern void Door_ReleaseRequest(u8 *p);
extern void *Doors_ctor(u8 *p);

#endif /* DOORS_H */
