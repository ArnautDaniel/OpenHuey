#ifndef EVENT_H
#define EVENT_H

/* event.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* event.c */
extern void Events_InstallRooms(u8 *ev);
extern void Events_RunPhase(u8 *ev, u8 phase);
extern void Events_RunCharScripts(VObject *ev);
extern void *Obj46BA80_dtor(void **o, s32 flags);
extern void *EventsBase_dtor(void **o, s32 flags);
extern void Events_DealThings(void);

#endif /* EVENT_H */
