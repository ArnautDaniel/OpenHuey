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
extern const PTMF sActorResetState;   /* virtual: actor vtable +0x14 */

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
    game->unk4 = 1;
    game->unk10 = 0;
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

static inline Actor *Game_GetActor(Game *game, s32 i) {
    return (u32)i < GAME_NUM_ACTORS ? game->actors[i] : NULL;
}

static inline s32 Game_HasActors(Game *game) {
    u32 i;

    for (i = 0; i < GAME_NUM_ACTORS; i++) {
        if (game->actors[i] != NULL) {
            return 1;
        }
    }
    return 0;
}

/* Runs every frame while the game is up. */
void Game_StateMain(Game *game) {
    u32 i;

    for (i = 0; i < GAME_NUM_ACTORS; i++) {
        Actor *actor = game->actors[i];

        if (actor == NULL) {
            continue;
        }
        if (actor->unk12 == 1) {
            /* finished: delete it (virtual destructor) and hand the slot back */
            VCALL(actor, 0x8, void (*)(Actor *, s32))(actor, 1);
            VCALL(&game->unk14D9A40, 0x14, void (*)(VObject *, Actor *))(&game->unk14D9A40, game->actors[i]);
            game->actors[i] = NULL;
        } else {
            VCALL(actor, 0xC, void (*)(Actor *, s32))(actor, 1);
        }
    }

    if (!Game_HasActors(game)) {
        VCALL(game, 0x24, void (*)(Game *))(game);
    }
    VCALL(&game->unk69AC0, 0x10, void (*)(VObject *))(&game->unk69AC0);

    if (game->unkC == 0) {
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
        for (i = 0; i < GAME_NUM_ACTORS; i++) {
            if (Game_GetActor(game, (s8)i) != NULL) {
                Actor *actor = Game_GetActor(game, (u8)i);

                if (actor != NULL) {
                    PTMF reset = sActorResetState;

                    if (ptmf_test(&reset)) {
                        actor->state = reset;
                    }
                    actor = game->actors[(u8)i];
                    VCALL(actor, 0x14, void (*)(Actor *))(actor);
                }
            }
        }
        VCALL(D_0044E4E0, 0x1C, void (*)(VObject *))(D_0044E4E0);
        game->unk4 = 2;
        game->unk10 = 1;
        if (game->unk8 == 2) {
            game->unk4 = 0;
            game->unk10 = 0;
        }
    }
}

/* Final state: shut down and clear the state, which ends Game_Run. */
void Game_StateShutdown(Game *game) {
    func_001F4100(game->unk14E8C90);
    VCALL(&game->unk69AC0, 0x18, void (*)(VObject *))(&game->unk69AC0);
    game->state = sGameStateNull;
}
