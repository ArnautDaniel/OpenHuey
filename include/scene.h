#ifndef SCENE_H
#define SCENE_H

/* scene.c: what other files call. */
#include "common.h"

typedef struct Scene Scene;

/* scene.c */
extern void Scene_SetSlot(Scene *scene, u32 slot);   /* u8 */
extern void Scene_Activate(Scene *scene);
extern Scene *Scene_dtor(Scene *scene, s32 flags);
extern void Scene_OnSoftReset(Scene *scene);
extern void Scene_Update(Scene *scene, s32 arg);

#endif /* SCENE_H */
