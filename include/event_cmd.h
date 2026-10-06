#ifndef EVENT_CMD_H
#define EVENT_CMD_H

/* event_cmd.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* event_cmd.c */
extern void func_002029B0(VObject *ev);   /* a command */
extern void func_00201B90(VObject *ev);   /* a character script command */
extern s32 func_001FBF70(VObject *ev, s32 id);   /* the step slot for id */

#endif /* EVENT_CMD_H */
