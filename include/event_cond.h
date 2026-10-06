#ifndef EVENT_COND_H
#define EVENT_COND_H

/* event_cond.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* event_cond.c */
extern s32 EventCond_AreaCross(VObject *ev, u8 *c, s32 area);   /* a character's relation to an area */

#endif /* EVENT_COND_H */
