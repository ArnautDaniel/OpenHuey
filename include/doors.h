#ifndef DOORS_H
#define DOORS_H

/* doors.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* doors.c */
extern void *func_00221920(u8 *e, s32 flags);
extern void *func_00221890(u8 *d, s32 flags);
extern void func_002238F0(VObject *d);   /* the doors released */
extern void func_00220D10(u8 *e, s32 id, s32 how);   /* a door sound */

#endif /* DOORS_H */
