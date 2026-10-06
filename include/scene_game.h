#ifndef SCENE_GAME_H
#define SCENE_GAME_H

/* scene_game.c: what other files call. */
#include "common.h"

typedef struct Scene Scene;

/* scene_game.c */
extern Scene *SceneGame_ctor(Scene *g);   /* mode 3: gameplay (16 MB) */
extern void *func_001FB3B0(void *o, s32 flags);
extern void *func_001FB400(u8 *o, s32 flags);
extern void *func_0038C8D0(u8 *m);

#endif /* SCENE_GAME_H */
