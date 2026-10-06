#ifndef STALKER_MODELS_H
#define STALKER_MODELS_H

/* stalker_models.c: what other files call. */
#include "common.h"

/* stalker_models.c */
extern void Model_BodyFrames(u8 *m, void *actor, f32 a, f32 b);
extern void Model_Frame(u8 *m);
extern void HumanModel_Frame(u8 *m);
extern void func_001F7AC0(u8 *m);
extern void func_002DC6D0(void *p);   /* operator delete */
extern void Model_ClearDraw(u8 *m);
extern void func_002ED260(u8 *m, s32 pose);

#endif /* STALKER_MODELS_H */
