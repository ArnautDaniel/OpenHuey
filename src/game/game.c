/* The game object's top level: initialisation and the state machine main loop. */
#include "common.h"
#include "game.h"

extern void func_001136E8(s32 status);         /* exit() */
extern s32 func_0037E1F0(s32 *result);         /* load the embedded IOP module, *result = its status */
extern void func_002CFA10(Game *game);
extern void func_001F44D0(void *obj);           /* init Game.unk14E8C90 */
extern void func_001F4100(void *obj);           /* shut down Game.unk14E8C90 */
extern void func_002BFB20(void *obj);           /* init Game.unk20 */

extern u32 D_0047E374;     /* pad buttons held (bit 0 Select, bit 3 Start) */
extern VObject *D_0044E4E0; /* global manager object, type unknown */

extern const PTMF sGameStateMain;     /* { 0, -1, Game_StateMain } */
extern const PTMF sGameStateShutdown; /* { 0, -1, Game_StateShutdown } */
extern const PTMF sGameStateNull;     /* all zero: ends Game_Run */
extern const PTMF sSceneResetState;   /* virtual: scene vtable +0x14 */

#define PAD_SELECT 0x1
#define PAD_START 0x8

void Game_Init(Game *game) {
    s32 result;
    s32 ret;

    VCALL(&game->unk69AC0, 0xC, void (*)(VObject *))(&game->unk69AC0);
    ret = func_0037E1F0(&result);
    if (ret >= 0 && result != 0) {
        func_001136E8(0);
    }
    func_002CFA10(game);
    func_001F44D0(game->unk14E8C90);
    func_002BFB20(game->unk20);
    game->nextMode = 1;
    game->modeParam = 0;
    game->resetHoldFrames = 0;
    game->state = sGameStateMain;
}

void Game_Run(Game *game) {
    while (ptmf_test(&game->state)) {
        ptmf_scall(game, &game->state);
    }
}

void Game_SetState(Game *game, const PTMF *state) {
    game->state = *state;
}

static inline Scene *Game_GetScene(Game *game, s32 i) {
    return (u32)i < GAME_NUM_SCENES ? game->scenes[i] : NULL;
}

static inline s32 Game_HasScenes(Game *game) {
    u32 i;

    for (i = 0; i < GAME_NUM_SCENES; i++) {
        if (game->scenes[i] != NULL) {
            return 1;
        }
    }
    return 0;
}

/* Runs every frame while the game is up. */
void Game_StateMain(Game *game) {
    u32 i;

    for (i = 0; i < GAME_NUM_SCENES; i++) {
        Scene *scene = game->scenes[i];

        if (scene == NULL) {
            continue;
        }
        if (scene->finished == 1) {
            /* finished: delete it (virtual destructor) and hand the slot back */
            VCALL(scene, 0x8, void (*)(Scene *, s32))(scene, 1);
            VCALL(&game->sceneHeap, 0x14, void (*)(VObject *, Scene *))(&game->sceneHeap, game->scenes[i]);
            game->scenes[i] = NULL;
        } else {
            VCALL(scene, 0xC, void (*)(Scene *, s32))(scene, 1);
        }
    }

    if (!Game_HasScenes(game)) {
        VCALL(game, 0x24, void (*)(Game *))(game);
    }
    VCALL(&game->unk69AC0, 0x10, void (*)(VObject *))(&game->unk69AC0);

    if (game->softResetEnabled == 0) {
        game->resetHoldFrames = 0;
        return;
    }

    /* Soft reset: hold Select+Start for a second. */
    if ((D_0047E374 & PAD_SELECT) && (D_0047E374 & PAD_START)) {
        u32 n = game->resetHoldFrames + 1;
        if (n != 0) {
            game->resetHoldFrames = n;
        }
    } else {
        game->resetHoldFrames = 0;
    }
    if (game->resetHoldFrames >= 60) {
        game->resetHoldFrames = 0;
        for (i = 0; i < GAME_NUM_SCENES; i++) {
            if (Game_GetScene(game, (s8)i) != NULL) {
                Scene *scene = Game_GetScene(game, (u8)i);

                if (scene != NULL) {
                    PTMF reset = sSceneResetState;

                    if (ptmf_test(&reset)) {
                        scene->state = reset;
                    }
                    scene = game->scenes[(u8)i];
                    VCALL(scene, 0x14, void (*)(Scene *))(scene);
                }
            }
        }
        VCALL(D_0044E4E0, 0x1C, void (*)(VObject *))(D_0044E4E0);
        game->nextMode = 2;
        game->modeParam = 1;
        if (game->mode == 2) {
            game->nextMode = 0;
            game->modeParam = 0;
        }
    }
}

/* Final state: shut down and clear the state, which ends Game_Run. */
void Game_StateShutdown(Game *game) {
    func_001F4100(game->unk14E8C90);
    VCALL(&game->unk69AC0, 0x18, void (*)(VObject *))(&game->unk69AC0);
    game->state = sGameStateNull;
}

/* Scene constructors (placement: they construct in memory from the scene heap and return it). */
extern Scene *SceneBoot_ctor(void *mem);  /* mode 1: memory card check, logos */
extern Scene *SceneTitle_ctor(void *mem); /* mode 2: opening movie, title screen, menus */
extern Scene *SceneGame_ctor(void *mem);  /* mode 3: gameplay (16 MB) */
extern Scene *Scene5_ctor(void *mem);     /* mode 5: unknown */
extern void func_001779B0(void *obj, s32 param);
extern VObject *D_0044E968; /* global object, type unknown (+0x14 gets the mode parameter in mode 2) */
extern VObject *D_0044E4D8; /* global object, type unknown */

static Scene *Game_NewScene(Game *game, u32 size, Scene *(*ctor)(void *), u8 slot) {
    void *mem = VCALL(&game->sceneHeap, 0x10, void *(*)(VObject *, u32))(&game->sceneHeap, size);

    if (mem != NULL) {
        Scene *scene = ctor(mem);

        game->scenes[slot] = scene;
        Scene_SetSlot(game->scenes[slot], slot);
        if (game->scenes[slot] != NULL) {
            Scene_Activate(game->scenes[slot]);
        }
    }
    return game->scenes[slot];
}

/* Game vtable +0x24: called by Game_StateMain once no scene is left; starts game->nextMode. */
void Game_StartNextScene(Game *game) {
    game->mode = game->nextMode;
    switch (game->nextMode) {
    case 1:
        Game_NewScene(game, 0xC7700, SceneBoot_ctor, 0);
        game->softResetEnabled = 0;
        break;
    case 2:
        Game_NewScene(game, 0x140D00, SceneTitle_ctor, 0);
        *(s32 *)((u8 *)D_0044E968 + 0x14) = game->modeParam;
        game->softResetEnabled = 0;
        break;
    case 3:
        Game_NewScene(game, 0x1065080, SceneGame_ctor, 1);
        func_001779B0(D_0044E4D8, game->modeParam);
        game->softResetEnabled = 1;
        break;
    case 5:
        Game_NewScene(game, 0x117540, Scene5_ctor, 0);
        game->softResetEnabled = 1;
        break;
    default:
        game->state = sGameStateShutdown;
        break;
    }
}
