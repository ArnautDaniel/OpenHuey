/* The game's system object (Game +0x69AC0, vtable 0x46ADF0, ticked every frame) and the
 * objects it embeds: file loader, ... Constructors so far.
 *
 * (was game.c) The game object's top level: initialisation and the state machine main loop.
 *
 * (was runtime.c) Metrowerks C++ runtime helpers.
 */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "globals.h"
#include "memcard.h"
#include "actor.h"
#include "navmesh.h"
#include "progress.h"
#include "daniella.h"
#include "loader.h"
#include "pad.h"
#include "renderer.h"
#include "scene_game.h"
#include "sound.h"
#include "system.h"
#include "cri/adx.h"
#include "libc.h"
#include "msl.h"
#include "sce/eekernel.h"
#include "sce/intc.h"
#include "sce/iop.h"
#include "sce/libmc.h"
#include "sce/libpad2.h"
#include "sce/sif.h"
#include "input.h"
#include "vecmath.h"
#include "scene.h"
#include "scene_boot.h"
#include "scene_title.h"
#include "text.h"
#include "ps2hw.h"
#include "heap.h"
#include "movie.h"

extern void *D_0046BEE0[];

extern void *Renderer_vtable[];
extern void *D_0046ACF0[];
extern void *D_0046AD88[];
extern void *D_0046ADD0[];
extern void *D_0046AE10[];
extern void *D_0046AF20[];
extern void *Vram_vtable[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void Fades_Start(void *self, u8 *p);

extern void Game_LoadCommonSounds(Game *game);
extern void Records_Reset(void *obj);           /* init Game.unk20 */
extern const PTMF sGameStateMain;     /* { 0, -1, Game_StateMain } */
extern const PTMF sGameStateShutdown; /* { 0, -1, Game_StateShutdown } */
extern const PTMF sGameStateNull;     /* all zero: ends Game_Run */
extern const PTMF sSceneResetState;   /* virtual: scene vtable +0x14 */
extern void *Helper469D00_vtable[];
extern void *D_0046D770[];
void *TexCache_dtor(u8 *o, s32 flags);
extern void hg_debug_next_scene(s32 *mode, s32 *param);   /* native/platform/debug.c */
extern const char D_0045D7C0[], D_0045D7D0[], D_0045D7E0[];   /* C_0000.HD / .SDT / .BD */
extern void Options_Defaults(u8 *o);
extern void *D_004699E0[];
extern void *Game_vtable[], *D_0046BF08[], *Heap_vtable[];
extern void *Camera_vtable[], *D_00469B40[], *System_vtable[];
extern void *gSkelPool, *gChainPool;
extern void Options_Defaults(u8 *o);
extern void *Pads_dtor(u8 *o, s32 flags);
extern void *TexCache_dtor(u8 *o, s32 flags);
extern u8 str_exception[];
extern u8 str_bad_alloc[];
extern u8 str_bad_exception[];
extern u8 D_0044DAB0[];
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

#define LOADER_SIZE(l, name) VCALL(l, 0x30, u32 (*)(VObject *, const char *))(l, name)

#define LOADER_LOAD(l, name, dst) VCALL(l, 0x34, void (*)(VObject *, const char *, void *))(l, name, dst)

/* load file `name` into a temporary buffer and hand it to sound driver method `method` (bank 5) */
static inline void Game_LoadSoundFile(VObject *loader, const char *name, s32 method) {
    u32 size = LOADER_SIZE(loader, name);
    void *buf;

    if (size != 0 && (buf = msl_memalign(0x40, size)) != NULL) {
        LOADER_LOAD(loader, name, buf);
        VCALL(gSound, method, void (*)(VObject *, s32, void *, u32))(gSound, 5, buf, size);
        msl_free(buf);
    }
}

void Game_LoadCommonSounds(Game *game);
void Records_Reset(void *obj);

/* 0x00100540 */
void *Exception_What(void) {
    return str_exception;
}

/* 0x00100650 */
void *BadAlloc_What(void) {
    return str_bad_alloc;
}

/* __ptmf_cmpr: whether two member function pointers differ */
/* 0x00100B80 */
s32 __ptmf_cmpr(const PTMF *a, const PTMF *b) {
    return (a->this_delta ^ b->this_delta) | (a->vtbl_offset ^ b->vtbl_offset) |
           (a->u.vptr_offset ^ b->u.vptr_offset) ? 1 : 0;
}

/* 0x00102310 */
void *BadException_What(void) {
    return str_bad_exception;
}

/* 0x00114B90 */
void *Libc_Data44DAB0(void) {
    return D_0044DAB0;
}
/* Game's base class constructor. */
/* 0x0020E7F0 */
void *GameBase_ctor(Game *game) {
    gGamePtr = (u8 *)game;
    game->vtbl = D_0046BEE0;
    game->nextMode = 0;
    game->softResetEnabled = 0;
    game->modeParam = 0;
    return game;
}

/* Game.unk20 (records / options?): reset; 12 times at 99:59:59 */
/* 0x002BFB20 */
void Records_Reset(void *obj) {
    u8 *d = obj;
    s32 i;

    *(s32 *)(d + 0x0) = -1;
    *(s32 *)(d + 0x4) = 0;
    *(s32 *)(d + 0x8) = 0;
    *(s32 *)(d + 0xC) = 0;
    Options_Defaults(d + 0x10);
    for (i = 0; i < 12; i++) {
        d[0x1C + i * 4] = 99;
        d[0x1D + i * 4] = 59;
        d[0x1E + i * 4] = 59;
        d[0x1F + i * 4] = 0;
    }
    *(s32 *)(d + 0x4C) = 0;
}

/* 0x002CF9E0 */
u8 *Game_Resident102C0(Game *game) { return (u8 *)game + 0x102C0; }   /* +0x18 */

/* 0x002CF9F0 */
u8 *Game_Resident312C0(Game *game) { return (u8 *)game + 0x312C0; }   /* +0x1C: GAME_FIX.GFM */

/* 0x002CFA00 */
u8 *Game_Resident38AC0(Game *game) { return (u8 *)game + 0x38AC0; }   /* +0x20: GAME_FIX.TEX */

/* Load the common sound bank C_0000 (header, sequence data, wave data) into the sound driver. */
/* 0x002CFA10 */
void Game_LoadCommonSounds(Game *game) {
    VObject *loader = gFileLoader;

    (void)game;
    Game_LoadSoundFile(loader, D_0045D7C0, 0x4C);
    Game_LoadSoundFile(loader, D_0045D7D0, 0x50);
    Game_LoadSoundFile(loader, D_0045D7E0, 0x58);
    VCALL(gSound, 0x60, void (*)(VObject *, s32))(gSound, 5);
}

void Game_StartNextScene(Game *game) {
#ifdef HG_NATIVE
    hg_debug_next_scene(&game->nextMode, &game->modeParam);
#endif
    game->mode = game->nextMode;
    switch (game->nextMode) {
    case 1:
        Game_NewScene(game, 0xC7700, (Scene * (*)(void *))SceneBoot_ctor, 0);
        game->softResetEnabled = 0;
        break;
    case 2:
        Game_NewScene(game, 0x140D00, (Scene * (*)(void *))SceneTitle_ctor, 0);
        *(s32 *)((u8 *)gSceneTitle + 0x14) = game->modeParam;
        game->softResetEnabled = 0;
        break;
    case 3:
        Game_NewScene(game, 0x1065080, (Scene * (*)(void *))SceneGame_ctor, 1);
        Progress_SetStartEntry(gProgress, game->modeParam);
        game->softResetEnabled = 1;
        break;
    case 5:
        Game_NewScene(game, 0x117540, (Scene * (*)(void *))Scene5_ctor, 0);
        game->softResetEnabled = 1;
        break;
    default:
        game->state = sGameStateShutdown;
        break;
    }
}

/* destructor of an entry holding a quad drawer at +0x40 */
/* 0x002D0CE0 */
void *QuadEntry_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x40, void **) = D_0046D770;
        AT(o, 0x40, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* Final state: shut down and clear the state, which ends Game_Run. */
void Game_StateShutdown(Game *game) {
    Slots_Reset2(game->unk14E8C90);
    VCALL(&game->unk69AC0, 0x18, void (*)(VObject *))(&game->unk69AC0);
    game->state = sGameStateNull;
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
    if ((gPadHeld & PAD_SELECT) && (gPadHeld & PAD_START)) {
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

void Game_Init(Game *game) {
    s32 result;
    s32 ret;

    VCALL(&game->unk69AC0, 0xC, void (*)(VObject *))(&game->unk69AC0);
    ret = Iop_LoadEmbeddedModule(&result);
    if (ret >= 0 && result != 0) {
        msl_exit(0);
    }
    Game_LoadCommonSounds(game);
    Slots_Init(game->unk14E8C90);
    Records_Reset(game->unk20);
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

/* Starts a 16.16 fade of channel i from `from` to `to` over `frames` (12 bits) frames. */
/* The original null-checks the address of each member (inlined constructors). */
/* 0x002D4680 */
void Fades_Start(void *self, u8 *p) {
    u32 a = (u32)p;
    s32 i;

    p[0] = 0;
    if (a + 0x4 != 0) F(p, 0x4, u16) = 0;
    if (a + 0x8 != 0) F(p, 0x8, u16) = 0;
    if (a + 0xC != 0) F(p, 0xC, u16) = 0;
    if (a + 0x14 != 0) F(p, 0x14, u16) = 0;
    if (a + 0x18 != 0) F(p, 0x18, u16) = 0;
    if (a + 0x1C != 0) F(p, 0x1C, u16) = 0;
    if (a + 0x20 != 0) F(p, 0x20, u16) = 0;
    if (a + 0x24 != 0) {
        for (i = 0; i < 16; i++) {
            p[0x24 + i] = 0;
        }
    }
    F(p, 0x50, u32) = 0;
    F(p, 0x40, u32) = 0;
    F(p, 0x54, u32) = 0;
    F(p, 0x44, u32) = 0;
    F(p, 0x58, u32) = 0;
    F(p, 0x48, u32) = 0;
    F(p, 0x5C, u32) = 0;
    F(p, 0x4C, u32) = 0;
}

/* 0x0037E2F0 */
u8 *Game_ResidentB2C0(Game *game) { return (u8 *)game + 0xB2C0; }    /* +0x14: MSG_SUB.BIN */

/* 0x0037E300 */
u8 *Game_Resident2AC0(Game *game) { return (u8 *)game + 0x2AC0; }    /* +0x10: MSG_BASE.TEX */

/* Game vtable +0xC..+0x20: resident buffers inside the Game (in Game.unk20) */
/* 0x0037E310 */
u8 *Game_Resident1AC0(Game *game) { return (u8 *)game + 0x1AC0; }    /* +0xC: message base (MSG_BASE.BIN) */

/* Element constructors for two arrays in the +0x395D40 object. */
/* 0x0020E7D0 */
void *IopArrayA_ctor(void *e) {
    AT(e, 0x8, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x0, s32) = 0;
    AT(e, 0xC, u8) = 0;
    AT(e, 0xD, u8) = 0;
    return e;
}

/* 0x0020E7B0 */
void *IopArrayB_ctor(void *e) {
    AT(e, 0x4, s32) = 0;
    AT(e, 0x0, s32) = 0;
    AT(e, 0xC, s32) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0x14, s32) = 0;
    AT(e, 0x10, u8) = 0;
    return e;
}

/* Global pointers to the parts (set by the constructor). */

extern void *System_vtable[], *D_0046ADD0[], *Pads_vtable[], *D_0046ADC4[], *D_0046AD88[], *MemCard_vtable[], *D_0046AEB4[];
extern void *Renderer_vtable[], *D_0046AF00[], *D_0046AF0C[], *MovieLib_vtable[], *Vram_vtable[], *Loader_vtable[];
extern void *SndDriver_vtable[], *D_0046BF2C[];
extern u8 gInput[], kButtonMap[16], kAnalogMap[16];
extern const PTMF sGameStateNull;

extern void RenderState_Defaults(u8 *r);

/* System object constructor. */
/* 0x0020E340 */
void *System_ctor(u8 *s) {
    u8 *p;
    s32 i;

    AT(s, 0x0, void **) = System_vtable;
    gSystem = (VObject *)s;
    AT(s, 0x20, void **) = D_0046AD88;
    gPad = (VObject *)(s + 0x40);
    AT(s, 0x40, void **) = D_0046ADD0;
    Fades_Start(s + 0x40, gInput);
    for (i = 0; i < 16; i++) {
        kButtonMap[i] = i;
    }
    for (i = 0; i < 16; i++) {
        kAnalogMap[i] = i;
    }
    AT(s, 0x40, void **) = Pads_vtable;
    AT(s, 0x58, void **) = D_0046ADC4;
    Rumble_ctor((Rumble *)(s + 0x300));

    gMemCard = (MemCard *)(s + 0x390);
    AT(s, 0x390, void **) = MemCard_vtable;
    AT(s, 0x39C, void **) = D_0046AEB4;
    gRenderer = (VObject *)(s + 0x460);
    AT(s, 0x3A4, PTMF) = sGameStateNull;
    AT(s, 0x460, void **) = Renderer_vtable;
    RenderState_Defaults(s + 0x460);

    p = s + 0x305280;
    gAdx = p;
    AT(p, 0x0, void **) = D_0046AF00;
    AT(p, 0x4, s32) = 0;
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x10, u8) = 0;
    AT(p, 0x124, void **) = D_0046AF0C;
    AT(p, 0x7C48, u8) = 0;
    gMovieLib = p + 0x7C44;
    AT(p, 0x7C44, void **) = MovieLib_vtable;
    msl_memset(p + 0x7C4C, 0, 0x20);
    msl_memset(p + 0x7C6C, 0, 0x30);

    gVram = (VObject *)(s + 0x30CF40);
    AT(s, 0x30CF40, void **) = Vram_vtable;
    gFileLoader = (VObject *)(s + 0x319900);
    AT(s, 0x319900, void **) = Loader_vtable;

    p = s + 0x395D40;
    gSound = (VObject *)(p + 4);
    AT(p, 0x0, void **) = SndDriver_vtable;
    AT(p, 0x4, void **) = D_0046BF2C;
    __construct_array(p + 0x84, IopArrayA_ctor, (void (*)(void *, s32))IopBuffers_dtor, 0x10, 8);
    __construct_array(p + 0x108, IopArrayB_ctor, IopArray_dtor, 0x18, 8);
    AT(p, 0x80, s32) = 0;
    AT(p, 0x104, s8) = -1;
    AT(p, 0x7EC, s32) = 0x100;
    AT(p, 0x7F0, s32) = 0x100;
    AT(p, 0x7F4, s32) = 0x80;
    AT(p, 0x7F8, s32) = 0x80;
    AT(p, 0x7DC, u8 *) = p + 0x1DC;
    AT(p, 0x7E0, u8 *) = p + 0x3DC;
    AT(p, 0x7E4, u8 *) = p + 0x5DC;
    AT(p, 0x7E8, u8 *) = p + 0x6DC;
    return s;
}

extern u32 _fbss;                 /* first word of .bss: libgraph table entry 2 */

/* Renderer state (system +0x460) defaults: 640x512 display, draw buffers, colour 0x80808080. */
/* 0x001B80C0 */
void RenderState_Defaults(u8 *r) {
    u32 i;

    AT(r, 0x304BE8, s32) = 0;
    AT(r, 0x304BEC, s32) = 0x88000;
    AT(r, 0x304BF0, s32) = 0x50000;
    AT(r, 0x304BF4, u32) = 0x80808080;
    AT(r, 0x304BF8, s32) = 0;
    AT(r, 0x304BFC, s16) = 640;
    AT(r, 0x304BFE, s16) = 512;
    AT(r, 0x304C00, s16) = 512;
    AT(r, 0x304C02, s16) = 448;
    AT(r, 0x304C04, u8) = 0;
    AT(r, 0x304C05, u8) = 0;
    AT(r, 0x304C06, u8) = 0x20;
    AT(r, 0x304C07, u8) = 0;
    AT(r, 0x304C08, u8) = 0;
    AT(r, 0x304C09, u8) = 2;
    AT(r, 0x304BB0, u32) = libgraph_TableEntry(1);
    _fbss = libgraph_TableEntry(2);
    AT(r, 0x304BB4, u8) = 0;
    AT(r, 0x304BB5, u8) = 0;
    ((void (*)(u8 *))(*(void ***)r)[0x1C / 4])(r);
    for (i = 0; i < 11; i++) {
        AT(r, 0x304BB8 + i * 4, s32) = -1;
    }
    AT(r, 0x304BE4, s32) = -1;
    AT(r, 0x304D58, s32) = 0;
    AT(r, 0x304DDC, s32) = 0;
}

/* destructor (vtable D_0046ACF0) */
/* 0x001BC090 */
void *Obj46ACF0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046ACF0;
        gRenderer = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AD88) */
/* 0x001BC320 */
void *Obj46AD88_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AD88;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

extern void *D_0046BF08[], *D_004699E0[], *Heap_vtable[];

/* Game +0x400A00: the scene table (4 scene pointers) and the scene heap after it (Game.sceneHeap,
 * 0x10D9000 bytes from +0x40). */
/* 0x0020E280 */
void *SceneTable_ctor(u8 *t) {
    VObject *heap = (VObject *)(t + 0x10D9040);
    s32 i;

    gSceneTable = t;
    AT(t, 0x0, void **) = D_0046BF08;
    heap->vtbl = D_004699E0;
    AT(heap, 0x4, s32) = 0;
    AT(heap, 0x8, s32) = 0;
    heap->vtbl = Heap_vtable;
    AT(heap, 0xC, s32) = 0;
    AT(heap, 0x10, s32) = 0;
    for (i = 0; i < 4; i++) {
        AT(t, 0x4 + i * 4, void *) = NULL;
    }
    Heap_Setup(heap, t + 0x40, 0x10D9000, t + 0x10D9054, 10);
    return t;
}

extern void *Camera_vtable[];

/* Object pool element constructors. */
/* 0x0020E240 */
void *SmallPool_ElemA(void *e) {
    AT(e, 0x0, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x8, s32) = 0;
    return e;
}

/* 0x0020E210 */
void *SmallPool_ElemB(void *e) {
    AT(e, 0xC, s32) = 0;
    AT(e, 0x4, s32) = 0;
    AT(e, 0x8, s32) = 0;
    AT(e, 0x10, s32) = 0;
    return e;
}

/* 0x0020E190 */
void *BigPool_ElemA(void *e) {
    AT(e, 0x0, s32) = 0;
    AT(e, 0x4, s32) = 0;
    return e;
}

/* 0x0020E180 */
void *BigPool_ElemB(void *e) {
    AT(e, 0x44, s32) = 0;
    AT(e, 0x48, s32) = 0;
    return e;
}

extern void *gChainPool, *gSkelPool;   /* the two pools */

/* Game +0x14D9DD0: pool of 64 x 0xC and 462 x 0x14 entries. */
/* 0x0020E1A0 */
void *SmallPool_ctor(u8 *p) {
    gChainPool = p;
    __construct_array(p, SmallPool_ElemA, Triple_dtor, 0xC, 0x40);
    __construct_array(p + 0x300, SmallPool_ElemB, Quad4_dtor, 0x14, 0x1CE);
    return p;
}

/* Game +0x14DC530: pool of 32 x 0xC and 632 x 0x50 entries. */
/* 0x0020E110 */
void *BigPool_ctor(u8 *p) {
    gSkelPool = p;
    __construct_array(p, BigPool_ElemA, Pair_dtor, 0xC, 0x20);
    __construct_array(p + 0x180, BigPool_ElemB, Pair44_dtor, 0x50, 0x278);
    return p;
}

extern void *TexCache_vtable[];

/* reset: 64 empty slots; the ids of the renderer's 10 layers (renderer +0x3C) */
static inline void Slots_Reset(u8 *o) {
    s32 i;

    for (i = 0; i < 64; i++) {
        u8 *e = o + 4 + i * 0xC;

        AT(e, 0x0, s32) = 0;
        AT(e, 0x4, s16) = -1;
        AT(e, 0x6, s16) = -1;
        AT(e, 0x9, s8) = -1;
        AT(e, 0x8, s8) = -1;
    }
    for (i = 0; i < 10; i++) {
        AT(o, 0x304 + i * 4, s32) = -1;
    }
    for (i = 0; i < 10; i++) {
        AT(o, 0x304 + i * 4, s32) = VCALL(gRenderer, 0x3C, s32 (*)(VObject *, s32))(gRenderer, i);
    }
}

/* Game +0x14E8C90 (shut down by Slots_Reset2): constructor */
/* 0x001F4600 */
void *Slots_ctor(u8 *o) {
    AT(o, 0x0, void **) = TexCache_vtable;
    gTexCache = (VObject *)o;
    Slots_Reset(o);
    return o;
}

/* destructor (vtable D_0046BEE0) */
/* 0x0020DB40 */
void *Obj46BEE0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046BEE0;
        gGamePtr = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* the members torn down in reverse: the +0x14E8C90 table, the two pools, the camera, the scene
 * table (+0x400A00, its scenes returned to the heap at +0x14D9A40), the rng, then the +0x69AC0
 * block and its members */
void *Game_dtor(u8 *g, s32 flags) {
    s32 i;

    if (g == NULL) {
        return g;
    }
    AT(g, 0x0, void **) = Game_vtable;
    TexCache_dtor(g + 0x14E8C90, -1);

    __destroy_arr(g + 0x14DC6B0, (void *(*)(void *, s32))Pair44_dtor, 0x50, 0x278);
    __destroy_arr(g + 0x14DC530, (void *(*)(void *, s32))Pair_dtor, 0xC, 0x20);
    gSkelPool = NULL;

    __destroy_arr(g + 0x14DA0D0, (void *(*)(void *, s32))Quad4_dtor, 0x14, 0x1CE);
    __destroy_arr(g + 0x14D9DD0, (void *(*)(void *, s32))Triple_dtor, 0xC, 0x40);
    gChainPool = NULL;

    AT(g, 0x14D9B00, void **) = Camera_vtable;
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
    AT(g, 0x14D9A40, void **) = Heap_vtable;
    AT(g, 0x14D9A40, void **) = D_004699E0;
    gSceneTable = NULL;

    Random_dtor((VObject *)(g + 0x400000), -1);

    AT(g, 0x69AC0, void **) = System_vtable;
    SndDriver_dtor(g + 0x3FF800, -1);
    Loader_dtor((VObject *)(g + 0x3833C0), -1);
    Vram_dtor(g + 0x376A00, -1);
    MovieSys_dtor(g + 0x36ED40, -1);
    Renderer_dtor(g + 0x69F20, -1);
    MemCard_dtor((MemCard *)(g + 0x69E50), -1);
    Rumble_dtor((Rumble *)(g + 0x69DC0), -1);
    Pads_dtor(g + 0x69B00, -1);
    Obj46AD88_dtor(g + 0x69AE0, -1);
    Obj46AE10_dtor(g + 0x69AC0, 0);

    AT(g, 0x0, void **) = D_0046BEE0;
    gGamePtr = NULL;
    if ((s16)flags > 0) {
        __dl__FPv(g);
    }
    return g;
}

void Game_SetState(Game *game, const PTMF *state) {
    game->state = *state;
}

/* ... init (from Game_Init) */
/* 0x001F44D0 */
void Slots_Init(u8 *o) {
    Slots_Reset(o);
}

/* ---- system init (vtable +0xC, from Game_Init) ---- */


extern const char str_SIO2MAN_IRX[], str_SIO2D_IRX[], str_DBCMAN_IRX[], str_LIBSD_IRX[];   /* SIO2MAN, SIO2D, DBCMAN, LIBSD .IRX */
extern void func_001AACD0(void *obj);
extern s32 VBlank_StartHandler(s32 cause), VBlank_EndHandler(s32 cause);         /* vblank start / end handlers */
extern u8 gVblankStartSeen, gVblankEndSeen;   /* vblank start / end seen */
extern u32 gVblankCount;              /* vblank count */

/* 0x001BF080 */
void System_Init(u8 *s) {
    Iop_Reset(s + 0x20);
    libgraph_Reset(1);
    AT(s, 0x4, s32) = Iop_LoadModule(s + 0x20, str_SIO2MAN_IRX, 0, 0, 0);
    AT(s, 0x8, s32) = Iop_LoadModule(s + 0x20, str_SIO2D_IRX, 0, 0, 0);
    AT(s, 0xC, s32) = Iop_LoadModule(s + 0x20, str_DBCMAN_IRX, 0, 0, 0);
    sceDbcInit();
    AT(s, 0x10, s32) = Iop_LoadModule(s + 0x20, str_LIBSD_IRX, 0, 0, 0);
    func_001AACD0(s + 0x305280);
    SndDriver_Start(s + 0x395D40);
    Pads_Init(s + 0x40);
    func_00226570(s + 0x390);
    Renderer_SetupVideo(s + 0x460, 2);
    VCALL(s + 0x30CF40, 0xC, void (*)(void *))(s + 0x30CF40);
    Renderer_AllocVram(s + 0x460);
    /* timer 0 and 1: count on the horizontal blank */
    HW_WRITE32(0x10000010, 0x82);
    HW_WRITE32(0x10000000, 0);
    HW_WRITE32(0x10000810, 0x82);
    gVblankCount = 0;
    gVblankStartSeen = 0;
    AT(s, 0x14, s32) = AddIntcHandler(2, VBlank_StartHandler, 0);
    EnableIntc(2);
    gVblankEndSeen = 0;
    AT(s, 0x18, s32) = AddIntcHandler(3, VBlank_EndHandler, 0);
    EnableIntc(3);
    AT(s, 0x1C, s32) = gVblankCount;
    Loader_Init(s + 0x319900);
    Loader_RegisterAll(s + 0x319900);
}

/* destructor (vtable D_0046AE10) */
/* 0x001BF220 */
void *Obj46AE10_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AE10;
        gSystem = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046AF20) */
/* 0x001BF7A0 */
void *Obj46AF20_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AF20;
        gVram = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

extern void Adx_SoundTick(void *snd);     /* ADX sound system tick */

/* +0x10 end of frame: finish the renderer's frame, wait for the vblank (at least two since the
 * last frame: the game runs at 30 fps), restart the timers, send the frame, then tick the parts. */
/* 0x001BEEF0 */
void System_EndFrame(u8 *s) {
    s32 n;

    Renderer_WaitChain(s + 0x460);
    VSYNC_WAIT(gVblankStartSeen);
    n = gVblankCount - AT(s, 0x1C, s32);
    if (n < 0) {
        n = -n;
    }
    if (n < 2) {
        VSYNC_WAIT(gVblankStartSeen);
    }
    AT(s, 0x1C, s32) = gVblankCount;
    HW_WRITE32(0x10000800, 0);   /* timer 1 count */
    Renderer_SendFinal(s + 0x460);
    Renderer_NextClear(s + 0x460);
    VSYNC_WAIT(gVblankEndSeen);
    HW_WRITE32(0x10000000, 0);   /* timer 0 count */
    Renderer_EndFrame(s + 0x460);
    SndDriver_Frame(s + 0x395D40);
    Adx_SoundTick(s + 0x305280);
    Loader_Tick(s + 0x319900);
    Rumble_Tick((Rumble *)(s + 0x300));
    Pads_Tick(s + 0x40);
    MemCard_Tick((MemCard *)(s + 0x390));
}

/* destructor (vtable D_0046ADD0) */
/* 0x001BE730 */
void *Obj46ADD0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046ADD0;
        gPad = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* +0x395D40 +0x84 element (8 x 0x10): two IOP buffers */
/* 0x001BEC10 */
void *IopBuffers_dtor(u8 *e, s32 flags) {
    if (e == NULL) {
        return e;
    }
    if (AT(e, 0x0, u32) != 0) {
        sceSifFreeIopHeap(AT(e, 0x0, u32));
        AT(e, 0x0, u32) = 0;
    }
    if (AT(e, 0x4, u32) != 0) {
        sceSifFreeIopHeap(AT(e, 0x4, u32));
        AT(e, 0x4, u32) = 0;
    }
    AT(e, 0x8, s32) = 0;
    AT(e, 0xC, u8) = 0;
    AT(e, 0xD, u8) = 0;
    if ((s16)flags > 0) {
        __dl__FPv(e);
    }
    return e;
}

/* destructor (vtable ?) */
/* 0x001BECA0 */
void *IopArray_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x4, s32) = 0;
        AT(o, 0x0, s32) = 0;
        AT(o, 0xC, s32) = 0;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x14, s32) = 0;
        AT(o, 0x10, u8) = 0;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

extern void *D_0046AE10[], *D_0046AF90[], *D_0046AF20[], *D_0046A220[], *D_0046ACF0[];
extern void *D_0046AEC0[], *D_0046AED0[], *D_0046AE60[], *Rumble_vtable[], *D_0046AE30[];

/* destructor (vtable +0x8): the members in reverse, each with its vtable chain and global cleared */
/* 0x001BE7A0 */
void *System_dtor(u8 *s, s32 flags) {
    u8 *p;

    if (s == NULL) {
        return s;
    }
    AT(s, 0x0, void **) = System_vtable;

    p = s + 0x395D40;   /* sound driver */
    AT(p, 0x0, void **) = SndDriver_vtable;
    AT(p, 0x4, void **) = D_0046BF2C;
    __destroy_arr(p + 0x108, (void *(*)(void *, s32))IopArray_dtor, 0x18, 8);
    __destroy_arr(p + 0x84, (void *(*)(void *, s32))IopBuffers_dtor, 0x10, 8);
    AT(p, 0x4, void **) = D_0046AF90;
    gSound = NULL;
    AT(p, 0x0, void **) = D_0046AD88;

    AT(s, 0x319900, void **) = Loader_vtable;   /* file loader */
    AT(s, 0x319900, void **) = D_0046A220;
    gFileLoader = NULL;

    AT(s, 0x30CF40, void **) = Vram_vtable;
    AT(s, 0x30CF40, void **) = D_0046AF20;
    gVram = NULL;

    p = s + 0x305280;   /* ADX sound system */
    AT(p, 0x0, void **) = D_0046AF00;
    AT(p, 0x124, void **) = D_0046AF0C;
    AT(p, 0x7C44, void **) = MovieLib_vtable;
    AT(p, 0x7C44, void **) = D_0046AED0;
    AT(p, 0x7C48, u8) = 0;
    gMovieLib = NULL;
    AT(p, 0x124, void **) = D_0046AD88;
    AT(p, 0x0, void **) = D_0046AEC0;
    AT(p, 0x4, s32) = 0;
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    gAdx = NULL;

    AT(s, 0x460, void **) = Renderer_vtable;   /* renderer */
    AT(s, 0x460, void **) = D_0046ACF0;
    gRenderer = NULL;

    AT(s, 0x390, void **) = MemCard_vtable;   /* memory card */
    AT(s, 0x39C, void **) = D_0046AEB4;
    AT(s, 0x39C, void **) = D_0046AD88;
    AT(s, 0x390, void **) = D_0046AE60;
    gMemCard = NULL;

    AT(s, 0x300, void **) = Rumble_vtable;   /* rumble */
    AT(s, 0x300, void **) = D_0046AE30;
    gRumble = NULL;

    AT(s, 0x40, void **) = Pads_vtable;    /* pads */
    AT(s, 0x58, void **) = D_0046ADC4;
    AT(s, 0x58, void **) = D_0046AD88;
    AT(s, 0x40, void **) = D_0046ADD0;
    gPad = NULL;

    AT(s, 0x20, void **) = D_0046AD88;
    AT(s, 0x0, void **) = D_0046AE10;
    gSystem = NULL;
    if ((s16)flags > 0) {
        __dl__FPv(s);
    }
    return s;
}

/* +0x18 shutdown: the vblank handlers off, then each part's shutdown, then reset the GS */
/* 0x001BEDD0 */
void System_Shutdown(u8 *s) {
    DisableIntc(2);
    DisableIntc(3);
    RemoveIntcHandler(3, AT(s, 0x18, s32));
    RemoveIntcHandler(2, AT(s, 0x14, s32));
    Pads_Shutdown(s + 0x40);
    MemCard_Shutdown((MemCard *)(s + 0x390));
    func_001EEA38();
    Loader_CloseAll(s + 0x319900);
    SndDriver_FreeIop(s + 0x395D40);
    MovieLib_Shutdown(s + 0x305280);
    libgraph_Reset(0);
}

/* +0x14 frame without the vblank wait: finish and send the renderer's frame, tick the parts */
/* 0x001BEE70 */
void System_FrameNoWait(u8 *s) {
    Renderer_WaitChain(s + 0x460);
    mwPly_ExecServer();
    Renderer_EndFrame(s + 0x460);
    SndDriver_Frame(s + 0x395D40);
    Adx_SoundTick(s + 0x305280);
    Loader_Tick(s + 0x319900);
    Rumble_Tick((Rumble *)(s + 0x300));
    Pads_Tick(s + 0x40);
}
