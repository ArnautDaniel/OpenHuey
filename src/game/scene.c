/* Scene: base of the game modes owned by Game (see Game_StartNextScene). */
#include "common.h"
#include "game.h"

void Scene_SetSlot(Scene *scene, u8 slot) {
    scene->slot = slot;
}

void Scene_Activate(Scene *scene) {
    scene->unk11 = 2;
    scene->finished = 0;
    scene->unk13 = 0;
}
