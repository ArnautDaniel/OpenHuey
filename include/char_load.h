#ifndef CHAR_LOAD_H
#define CHAR_LOAD_H

/* char_load.c: what other files call. */
#include "common.h"

typedef struct Progress Progress;

/* char_load.c */
extern s32 CharLoad_Partner(Progress *p, u32 id);   /* load character id as the partner */
extern void CharLoad_Buffers(Progress *p, s32 slot);
extern s32 Characters_Register(void *self, u32 kind, void *obj);
extern s32 CharLoad_EventChar(Progress *p, u32 id, u32 slot);   /* load event character id into slot */

#endif /* CHAR_LOAD_H */
