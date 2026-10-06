#ifndef EVENT_H
#define EVENT_H

/* event.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* event.c */
extern void func_00209850(u8 *ev);
extern void func_00209390(u8 *ev, u8 phase);
extern void func_00209210(VObject *ev);
extern void *func_0020C120(void **o, s32 flags);
extern void *func_0020C170(void **o, s32 flags);
extern void func_001FB5F0(void);

#endif /* EVENT_H */
