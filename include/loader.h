#ifndef LOADER_H
#define LOADER_H

/* loader.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* loader.c */
extern void func_0016C530(u8 *l);
extern void func_00169680(u8 *l);
extern void func_0016BFB0(u8 *l);   /* file loader tick */
extern void *func_00169280(VObject *l, s32 flags);
extern void func_0016BF50(u8 *l);

#endif /* LOADER_H */
