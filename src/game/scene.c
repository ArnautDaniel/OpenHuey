/* Scene: base of the game modes owned by Game (see Game_StartNextScene). */
#include "common.h"
#include "game.h"

extern void *Scene_vtable[];

void Scene_SetSlot(Scene *scene, u8 slot) {
    scene->slot = slot;
}

/* Post a "run" request and reset the status; Scene_Update picks it up next frame. */
void Scene_Activate(Scene *scene) {
    scene->request = SCENE_REQ_RUN;
    scene->status = SCENE_STATUS_IDLE;
    scene->waitFrames = 0;
}

/* Base destructor; derived destructors free the memory. */
Scene *Scene_dtor(Scene *scene, s32 flags) {
    if (scene != NULL) {
        scene->vtbl = Scene_vtable;
    }
    return scene;
}

/* Vtable +0x14: soft reset just asks the scene to finish. */
void Scene_OnSoftReset(Scene *scene) {
    scene->request = SCENE_REQ_FINISH;
}

/* Vtable +0xC, every frame: apply the pending request, then run the current state if running. */
void Scene_Update(Scene *scene, s32 arg) {
    s32 run = 0;

    switch (scene->request) {
    case SCENE_REQ_NONE:
        break;
    case SCENE_REQ_FINISH:
        scene->status = SCENE_STATUS_FINISHED;
        break;
    case SCENE_REQ_RUN:
    case SCENE_REQ_RESUME:
        scene->status = SCENE_STATUS_RUNNING;
        break;
    case SCENE_REQ_WAIT:
        if (scene->status != SCENE_STATUS_FINISHED && scene->waitFrames > 0) {
            scene->status = SCENE_STATUS_WAITING;
        }
        break;
    }
    scene->request = SCENE_REQ_NONE;

    switch (scene->status) {
    case SCENE_STATUS_IDLE:
        break;
    case SCENE_STATUS_FINISHED:
    case SCENE_STATUS_2:
        scene->status = SCENE_STATUS_FINISHED;
        break;
    case SCENE_STATUS_RUNNING:
        run = 1;
        break;
    case SCENE_STATUS_WAITING:
        if (--scene->waitFrames < 0) {
            scene->waitFrames = 0;
            scene->status = SCENE_STATUS_RUNNING;
            run = 1;
        }
        break;
    }

    if (run == 1 && ptmf_test(&scene->state)) {
        ptmf_scall(scene, &scene->state);
    }
}
