#ifndef EVENT_CMD_H
#define EVENT_CMD_H

/* event_cmd.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* event_cmd.c */
extern void EventCmd_Run(VObject *ev);   /* a command */
extern void Event_RunScript(VObject *ev);   /* a character script command */
extern s32 Event_CharSlot(VObject *ev, s32 id);   /* the step slot for id */

#endif /* EVENT_CMD_H */
