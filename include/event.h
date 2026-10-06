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

/* ---- (was event_cmd.h) ---- */

/* event_cmd.c: what other files call. */

typedef struct VObject VObject;

/* event_cmd.c */
extern void EventCmd_Run(VObject *ev);   /* a command */
extern void Event_RunScript(VObject *ev);   /* a character script command */
extern s32 Event_CharSlot(VObject *ev, s32 id);   /* the step slot for id */

/* ---- (was event_cond.h) ---- */

/* event_cond.c: what other files call. */

typedef struct VObject VObject;

/* event_cond.c */
extern s32 EventCond_AreaCross(VObject *ev, u8 *c, s32 area);   /* a character's relation to an area */

/* ---- (was script.h) ---- */

/* script.c: what other files call. */

/* script.c */
extern void Script_Start(u8 *s, u8 *script);   /* start a script */
extern void Script_RunControl(u8 *s);   /* a control op (0xF0..) */

#endif /* EVENT_H */
