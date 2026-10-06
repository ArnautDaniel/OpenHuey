/* The game object's top level: initialisation and the state machine main loop. */
#include "common.h"
#include "game.h"
#include "input.h"
#include "globals.h"

extern void func_001136E8(s32 status);         /* exit() */
extern s32 func_0037E1F0(s32 *result);         /* load the embedded IOP module, *result = its status */
extern void func_002CFA10(Game *game);
extern void func_001F44D0(void *obj);           /* init Game.unk14E8C90 */
extern void func_001F4100(void *obj);           /* shut down Game.unk14E8C90 */
extern void func_002BFB20(void *obj);           /* init Game.unk20 */


extern const PTMF sGameStateMain;     /* { 0, -1, Game_StateMain } */
extern const PTMF sGameStateShutdown; /* { 0, -1, Game_StateShutdown } */
extern const PTMF sGameStateNull;     /* all zero: ends Game_Run */
extern const PTMF sSceneResetState;   /* virtual: scene vtable +0x14 */


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
        if (scene->status == SCENE_STATUS_FINISHED) {
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
        VCALL(gFileLoader, 0x1C, void (*)(VObject *))(gFileLoader);
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
extern Scene *Scene5_ctor(void *mem);     /* mode 5: the ending */
extern void func_001779B0(void *obj, s32 param);
extern VObject *gSceneTitle; /* global object, type unknown (+0x14 gets the mode parameter in mode 2) */
extern void *gProgress;     /* include/progress.h */

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
#ifdef HG_NATIVE
extern void hg_debug_next_scene(s32 *mode, s32 *param);   /* native/platform/debug.c */
#endif

void Game_StartNextScene(Game *game) {
#ifdef HG_NATIVE
    hg_debug_next_scene(&game->nextMode, &game->modeParam);
#endif
    game->mode = game->nextMode;
    switch (game->nextMode) {
    case 1:
        Game_NewScene(game, 0xC7700, SceneBoot_ctor, 0);
        game->softResetEnabled = 0;
        break;
    case 2:
        Game_NewScene(game, 0x140D00, SceneTitle_ctor, 0);
        *(s32 *)((u8 *)gSceneTitle + 0x14) = game->modeParam;
        game->softResetEnabled = 0;
        break;
    case 3:
        Game_NewScene(game, 0x1065080, SceneGame_ctor, 1);
        func_001779B0(gProgress, game->modeParam);
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

extern const char D_0045D7C0[], D_0045D7D0[], D_0045D7E0[];   /* C_0000.HD / .SDT / .BD */
extern void *func_00114DA8(u32 align, u32 size);   /* memalign */
extern void func_00114FD0(void *p);                /* free */

#define LOADER_SIZE(l, name) VCALL(l, 0x30, u32 (*)(VObject *, const char *))(l, name)
#define LOADER_LOAD(l, name, dst) VCALL(l, 0x34, void (*)(VObject *, const char *, void *))(l, name, dst)

/* load file `name` into a temporary buffer and hand it to sound driver method `method` (bank 5) */
static inline void Game_LoadSoundFile(VObject *loader, const char *name, s32 method) {
    u32 size = LOADER_SIZE(loader, name);
    void *buf;

    if (size != 0 && (buf = func_00114DA8(0x40, size)) != NULL) {
        LOADER_LOAD(loader, name, buf);
        VCALL(gSound, method, void (*)(VObject *, s32, void *, u32))(gSound, 5, buf, size);
        func_00114FD0(buf);
    }
}

/* Load the common sound bank C_0000 (header, sequence data, wave data) into the sound driver. */
void func_002CFA10(Game *game) {
    VObject *loader = gFileLoader;

    (void)game;
    Game_LoadSoundFile(loader, D_0045D7C0, 0x4C);
    Game_LoadSoundFile(loader, D_0045D7D0, 0x50);
    Game_LoadSoundFile(loader, D_0045D7E0, 0x58);
    VCALL(gSound, 0x60, void (*)(VObject *, s32))(gSound, 5);
}

extern void func_002A7AA0(u8 *o);

/* Game.unk20 (records / options?): reset; 12 times at 99:59:59 */
void func_002BFB20(void *obj) {
    u8 *d = obj;
    s32 i;

    *(s32 *)(d + 0x0) = -1;
    *(s32 *)(d + 0x4) = 0;
    *(s32 *)(d + 0x8) = 0;
    *(s32 *)(d + 0xC) = 0;
    func_002A7AA0(d + 0x10);
    for (i = 0; i < 12; i++) {
        d[0x1C + i * 4] = 99;
        d[0x1D + i * 4] = 59;
        d[0x1E + i * 4] = 59;
        d[0x1F + i * 4] = 0;
    }
    *(s32 *)(d + 0x4C) = 0;
}


/* Options defaults: sound mode from the sound driver (+0x6C), the video mode and screen offset
 * from the renderer, +4 on, +8 = 1.0. */
void func_002A7AA0(u8 *o) {
    VObject *r;
    u8 *disp;

    o[0] = VCALL(gSound, 0x6C, s32 (*)(VObject *))(gSound);
    r = gRenderer;
    o[1] = VCALL(r, 0x28, u8 (*)(VObject *))(r);
    disp = VCALL(r, 0x2C, u8 *(*)(VObject *))(r);
    o[2] = (s8)disp[0x1F];
    o[3] = (s8)disp[0x20];
    o[4] = 1;
    o[5] = 0;
    o[6] = 0;
    *(u32 *)(o + 8) = 0x3F800000;   /* 1.0f */
}

/* Game vtable +0xC..+0x20: resident buffers inside the Game (in Game.unk20) */
u8 *func_0037E310(Game *game) { return (u8 *)game + 0x1AC0; }    /* +0xC: message base (MSG_BASE.BIN) */
u8 *func_0037E300(Game *game) { return (u8 *)game + 0x2AC0; }    /* +0x10: MSG_BASE.TEX */
u8 *func_0037E2F0(Game *game) { return (u8 *)game + 0xB2C0; }    /* +0x14: MSG_SUB.BIN */
u8 *func_002CF9E0(Game *game) { return (u8 *)game + 0x102C0; }   /* +0x18 */
u8 *func_002CF9F0(Game *game) { return (u8 *)game + 0x312C0; }   /* +0x1C: GAME_FIX.GFM */
u8 *func_002CFA00(Game *game) { return (u8 *)game + 0x38AC0; }   /* +0x20: GAME_FIX.TEX */

/* ---- destructors left (2026-10-05) ---- */

extern void *Task_dtor(void *t, s32 flags);
extern void func_00100490(void *p);   /* operator delete */
extern void func_001002C0(void *array, void *(*dtor)(void *, s32), u32 size, u32 n);   /* __destroy_arr */
extern void *D_00473440[], *D_0046F3D0[], *D_0046D770[], *D_00469D00[], *D_0046ECF0[], *D_0046F390[];
extern void *D_0046FC00[], *D_004699C0[], *D_004699E0[], *D_0046A980[];
extern void *D_00456DE8, *D_00456DF8, *gCreatures;

/* destructor (D_00473440): its task (+0x110C4) ended, then the base (D_0046F3D0, clearing
 * D_00456DE8) */
void *func_002D0B60(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00473440;
        if (AT(o, 0x110C4, void *) != NULL) {
            Task_dtor(AT(o, 0x110C4, void *), 1);
            AT(o, 0x110C4, void *) = NULL;
        }
        AT(o, 0x0, void **) = D_0046F3D0;
        D_00456DE8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor of an entry holding a quad drawer at +0x40 */
void *func_002D0CE0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x40, void **) = D_0046D770;
        AT(o, 0x40, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* (possibly dead code: nothing in the game references it) */
/* destructor (D_0046ECF0): its 64 entries (+0x20, 0xB0 each), then the base (D_0046F390,
 * clearing D_00456DF8) */
/* (possibly dead code: nothing in the game references it) */
void *func_002D0D60(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046ECF0;
        func_001002C0(o + 0x20, (void *(*)(void *, s32))func_002D0CE0, 0xB0, 0x40);
        AT(o, 0x0, void **) = D_0046F390;
        D_00456DF8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* (possibly dead code: nothing in the game references it) */
/* destructor (vtable at +0x28, D_0046FC00): members at +0xF630 / +0xDC40, then the base
 * (D_0046A980, clearing gCreatures) */
/* (possibly dead code: nothing in the game references it) */
void *func_002D0DF0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x28, void **) = D_0046FC00;
        AT(o, 0xF630, void **) = D_004699C0;
        AT(o, 0xF630, void **) = D_004699E0;
        AT(o, 0xDC40, void **) = D_004699C0;
        AT(o, 0xDC40, void **) = D_004699E0;
        AT(o, 0x28, void **) = D_0046A980;
        gCreatures = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* ---- the game's destructor (2026-10-05) ---- */

extern void *D_0046ADB0[], *D_0046ADC4[], *D_0046ADD0[], *D_0046AD88[];

/* Game +0x69B00's destructor: its vtables (and its +0x18 member's), gPad cleared */
void *func_001BE150(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_0046ADB0;
    AT(o, 0x18, void **) = D_0046ADC4;
    AT(o, 0x18, void **) = D_0046AD88;
    AT(o, 0x0, void **) = D_0046ADD0;
    gPad = NULL;
    if ((s16)flags > 0) {
        func_00100490(o);
    }
    return o;
}

extern void *Game_vtable[], *D_0046BEE0[], *D_0046BF08[], *D_0046A1C0[], *D_004699E0[];
extern void *D_00469A60[], *D_00469B40[], *D_0046ADF0[];
extern void *gSystemData, *gSceneTable, *D_004562A8, *D_004562B0;
extern void *func_001F4590(u8 *, s32), *func_0020D920(u8 *, s32), *func_0020D8D0(u8 *, s32);
extern void *func_0020D9C0(u8 *, s32), *func_0020D970(u8 *, s32);
extern void *func_001A4850(void *, s32), *func_0020E000(u8 *, s32), *func_00169280(void *, s32);
extern void *func_001BF880(u8 *, s32), *func_001BF6C0(void *, s32), *func_001AAE10(u8 *, s32);
extern void *func_001BF550(void *, s32), *func_0020DF90(void *, s32), *func_001BC320(u8 *, s32);
extern void *func_001BF220(u8 *, s32);

/* the members torn down in reverse: the +0x14E8C90 table, the two pools, the camera, the scene
 * table (+0x400A00, its scenes returned to the heap at +0x14D9A40), the rng, then the +0x69AC0
 * block and its members */
void *Game_dtor(u8 *g, s32 flags) {
    s32 i;

    if (g == NULL) {
        return g;
    }
    AT(g, 0x0, void **) = Game_vtable;
    func_001F4590(g + 0x14E8C90, -1);

    func_001002C0(g + 0x14DC6B0, (void *(*)(void *, s32))func_0020D920, 0x50, 0x278);
    func_001002C0(g + 0x14DC530, (void *(*)(void *, s32))func_0020D8D0, 0xC, 0x20);
    D_004562A8 = NULL;

    func_001002C0(g + 0x14DA0D0, (void *(*)(void *, s32))func_0020D9C0, 0x14, 0x1CE);
    func_001002C0(g + 0x14D9DD0, (void *(*)(void *, s32))func_0020D970, 0xC, 0x40);
    D_004562B0 = NULL;

    AT(g, 0x14D9B00, void **) = D_00469A60;
    AT(g, 0x14D9B00, void **) = D_00469B40;
    gCamera = NULL;

    AT(g, 0x400A00, void **) = D_0046BF08;
    for (i = 0; i < 4; i++) {
        void **slot = (void **)(g + 0x400A04) + i;

        if (*slot != NULL) {
            VObject *heap = (VObject *)(g + 0x14D9A40);

            VCALL(heap, 0x14, void (*)(VObject *, void *))(heap, *slot);
            if (*slot != NULL) {
                VCALL(*slot, 0x8, void (*)(void *, s32))(*slot, 1);
            }
            *slot = NULL;
        }
    }
    AT(g, 0x14D9A40, void **) = D_0046A1C0;
    AT(g, 0x14D9A40, void **) = D_004699E0;
    gSceneTable = NULL;

    func_001A4850(g + 0x400000, -1);

    AT(g, 0x69AC0, void **) = D_0046ADF0;
    func_0020E000(g + 0x3FF800, -1);
    func_00169280(g + 0x3833C0, -1);
    func_001BF880(g + 0x376A00, -1);
    func_001BF6C0(g + 0x36ED40, -1);
    func_001AAE10(g + 0x69F20, -1);
    func_001BF550(g + 0x69E50, -1);
    func_0020DF90(g + 0x69DC0, -1);
    func_001BE150(g + 0x69B00, -1);
    func_001BC320(g + 0x69AE0, -1);
    func_001BF220(g + 0x69AC0, 0);

    AT(g, 0x0, void **) = D_0046BEE0;
    gSystemData = NULL;
    if ((s16)flags > 0) {
        func_00100490(g);
    }
    return g;
}
