#ifndef MESSAGE_H
#define MESSAGE_H

/* message.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* message.c */
extern void Message_Init(u8 *m);
extern void Message_ClearAll(VObject *m);

#endif /* MESSAGE_H */
