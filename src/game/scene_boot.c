/* Mode 1 scene: loads the always-resident system files, then runs the boot steps
 * (pad check, memory card check and messages, logos) one after another. */
#include "common.h"
#include "scene_boot.h"
#include "input.h"
#include "globals.h"

extern void *Scene_vtable[];
extern void *SceneBoot_vtable[];
extern void *D_0046D7D0[];   /* vtable of SceneBoot.msg */
extern void *D_0046A0D0[];   /* base vtable of SceneBoot.msg */
extern void *D_0046A058[];   /* vtable of SceneBoot.card */
extern void *D_0046F350[];   /* vtable of SceneBoot.unkC75D0 */
extern void *D_00469D00[];   /* base vtable of SceneBoot.unkC75D0 */

extern const PTMF sSceneEntryState;   /* virtual: vtable +0x10 */
extern const PTMF sTaskIdleState;     /* { 0, -1, Task_StateIdle } */
extern const PTMF sGameStateNull;
extern const PTMF sSceneBootStateLoad;     /* -> SceneBoot_StateLoadSystem */
extern const PTMF sSceneBootStateSequence; /* -> SceneBoot_StateSequence */
extern const PTMF sSceneBootStateDone;     /* -> SceneBoot_StateDone */
extern const PTMF16 sSceneBootSteps[8];    /* boot steps, run in order */

extern VObject *gSystemData;   /* global object, type unknown (+0x1C/+0x20 return resident buffers) */

extern void func_0011F9A0(void *mem);   /* operator delete for scene memory? */
extern void func_0026BCC0(void *msg);
extern void func_0026BC00(VObject *msg);

static const char sErrMesTex[] = "SYSTEM\\ERRMES.TEX";
static const char sGameFixTex[] = "GAME_FIX.TEX";
static const char sGameFixGfm[] = "GAME_FIX.GFM";
static const char sLogoCri[] = "SYSTEM\\LOGO_CRI.BIN";

static inline void Scene_SetState(Scene *scene, const PTMF *state) {
    ptmf_set(&scene->state, state);
}

static inline void Task_Init(Task *task) {
    task->mode = 0;
    task->flags = 0;
    task->id = -1;
    task->child = NULL;
    Scene_SetState((Scene *)task, &sTaskIdleState);
}

SceneBoot *SceneBoot_ctor(SceneBoot *boot) {
    s32 i;

    /* Scene base constructor */
    boot->base.vtbl = Scene_vtable;
    Scene_SetState(&boot->base, &sSceneEntryState);
    boot->base.vtbl = SceneBoot_vtable;

    gBootMessage = &boot->msg;
    boot->msg.vtbl = D_0046D7D0;
    for (i = 0; i < 3; i++) {
        Task_Init(&boot->tasks[i]);
    }

    boot->card.vtbl = D_0046A058;
    Task_Init(&boot->card.task);
    boot->card.state = -1;

    boot->unkC75D0Vtbl = D_00469D00;
    boot->unkC75D4 = -1;
    boot->unkC75D0Vtbl = D_0046F350;
    boot->unkC75F4 = 0;
    boot->unkC75E0 = -1;
    boot->unkC75E4 = 0;
    return boot;
}

SceneBoot *SceneBoot_dtor(SceneBoot *boot, s32 flags) {
    s32 i;

    if (boot == NULL) {
        return boot;
    }
    boot->base.vtbl = SceneBoot_vtable;
    boot->unkC75D0Vtbl = D_0046F350;
    boot->unkC75D0Vtbl = D_00469D00;
    boot->card.vtbl = D_0046A058;
    Task_dtor(&boot->card.task, -1);
    for (i = 2; i >= 0; i--) {
        if (boot->tasks[i].child != NULL) {
            Task_dtor(boot->tasks[i].child, 1);
            boot->tasks[i].child = NULL;
        }
    }
    boot->msg.vtbl = D_0046D7D0;
    boot->msg.vtbl = D_0046A0D0;
    gBootMessage = NULL;
    boot->base.vtbl = Scene_vtable;
    if ((s16)flags > 0) {
        func_0011F9A0(boot);
    }
    return boot;
}

/* Vtable +0x10: first state. */
void SceneBoot_StateEntry(SceneBoot *boot) {
    Scene_SetState(&boot->base, &sSceneBootStateLoad);
}

/* Load the files that stay resident for the whole game, then start the boot steps. */
void SceneBoot_StateLoadSystem(SceneBoot *boot) {
    VObject *loader;
    VObject *res;

    func_0026BCC0(&boot->msg);
    loader = gFileLoader;
    VCALL(loader, 0x34, void (*)(VObject *, const char *, void *))(loader, sErrMesTex, boot->errMesTex);
    VCALL(gBootMessage, 0x8, void (*)(VObject *, s32, void *))(gBootMessage, 6, boot->errMesTex);
    res = gSystemData;
    VCALL(loader, 0x34, void (*)(VObject *, const char *, void *))(
        loader, sGameFixTex, VCALL(res, 0x20, void *(*)(VObject *))(res));
    VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(
        gTexCache, VCALL(res, 0x20, void *(*)(VObject *))(res), 0x10);
    VCALL(loader, 0x34, void (*)(VObject *, const char *, void *))(
        loader, sGameFixGfm, VCALL(res, 0x1C, void *(*)(VObject *))(res));
    VCALL(loader, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(loader, sLogoCri, boot->logoCri, 0x10000000, 0);
    boot->step = 0;
    boot->stepTimer = 0;
    boot->stepFlag = 0;
    Scene_SetState(&boot->base, &sSceneBootStateSequence);
}

/* Run the current boot step; a step returns nonzero while it is still busy. */
void SceneBoot_StateSequence(SceneBoot *boot) {
    PTMF steps[9];
    s32 i;

    for (i = 0; i < 8; i++) {
        steps[i] = sSceneBootSteps[i].p;
    }
    steps[8] = sGameStateNull;

#ifdef HG_NATIVE
    {
        extern s32 hg_debug_skip_boot_step(const void *fn);   /* native/platform/debug.c */

        while (steps[boot->step].vtbl_offset == -1 && hg_debug_skip_boot_step(steps[boot->step].u.func)) {
            boot->step++;
        }
    }
#endif
    if (ptmf_test(&steps[boot->step]) && (u8)ptmf_scall_r(boot, &steps[boot->step]) == 0) {
        boot->stepFlag = 0;
        boot->stepTimer = 0;
        boot->step++;
    }
    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    VCALL(&boot->msg, 0x20, void (*)(VObject *))(&boot->msg);
    if (!ptmf_test(&steps[boot->step])) {
        Scene_SetState(&boot->base, &sSceneBootStateDone);
    }
}

/* All steps done: close the message display and finish the scene. */
void SceneBoot_StateDone(SceneBoot *boot) {
    VObject *msg = gBootMessage;

    VCALL(msg, 0x14, void (*)(VObject *, s32))(msg, 6);
    VCALL(msg, 0xC, void (*)(VObject *, s32))(msg, 6);
    func_0026BC00(&boot->msg);
    ((s32 *)gSystemData)[1] = 2;
    ((s32 *)gSystemData)[4] = 2;
    VCALL(boot, 0x14, void (*)(SceneBoot *))(boot);
}

/* ---- boot steps and their helpers ---- */

extern s8 D_0047E360;          /* 0 = (no controller?): boot steps then wait for a button */
extern u8 D_0047B350;
extern void *D_01991EC0;       /* SUBSCR\MSG_BASE.BIN, once loaded */
extern void *D_01991EC8;       /* SUBSCR\MSG_SUB.BIN, once loaded */

static const char sMsgSubBin[] = "SUBSCR\\MSG_SUB.BIN";
static const char sMsgBaseBin[] = "SUBSCR\\MSG_BASE.BIN";
static const char sMsgBaseTex[] = "SUBSCR\\MSG_BASE.TEX";

/* Load the subtitle message file into the resident buffer. */
void func_0037EC00(SceneBoot *boot) {
    VObject *res = gSystemData;

    VCALL(gFileLoader, 0x34, void (*)(VObject *, const char *, void *))(
        gFileLoader, sMsgSubBin, VCALL(res, 0x14, void *(*)(VObject *))(res));
    D_01991EC8 = VCALL(res, 0x14, void *(*)(VObject *))(res);
}

#ifdef HG_NATIVE
#include "gl2d.h"

/* Message sprite `line` of message slot 6 (the boot screens' pictures): its image header, NULL
 * if there's none (or it can't be made resident) */
static u8 *BootSprite_Image(s32 line, s32 prio) {
    VObject *msg = gBootMessage;
    VObject *gs = gRenderer;
    s32 id = VCALL(msg, 0x24, s32 (*)(VObject *, s32, s32))(msg, 6, line);
    u8 *img;

    if (id == -1) {
        return NULL;
    }
    img = VCALL(msg, 0x28, u8 *(*)(VObject *, s32, s32))(msg, 6, line);
    if (id & 0x80000000) {
        if ((VCALL(gs, 0x44, u32 (*)(VObject *, s32, void *, s32))(gs, id & 0x7FFFFFFF, img, prio) & 0xFF) == 0) {
            return NULL;
        }
    }
    return img;
}

/* Draw the single large boot message picture (slot 6, line 0): 512 x 512 from the top left,
 * opaque, layer 1. */
void func_0037F2E0(SceneBoot *boot) {
    u8 *img = BootSprite_Image(0, 1);

    if (img != NULL) {
        gl2d_sprite(1, 0, 0, 512, 512, img, 0, 0, 512, 512, 0x80808080, 0, 0);
    }
}

/* Draw the two boot message lines (slot 6, lines 0 and 1): 512 x 256 each, one under the
 * other, opaque, layer 0x30. */
void func_0037F510(SceneBoot *boot) {
    s32 i;

    for (i = 0; i < 2; i++) {
        u8 *img = BootSprite_Image(i, 0x30);

        if (img != NULL) {
            gl2d_sprite(0x30, 0, i * 256, 512, (i + 1) * 256, img, 0, 0, 512, 256, 0x80808080, 0, 0);
        }
    }
}
#else
void func_0037F2E0(SceneBoot *boot);
void func_0037F510(SceneBoot *boot);
#endif

/* Shared start of the boot steps: without (a controller?), show the message and wait
 * until button bit 14 is pressed. Returns nonzero while still waiting. */
static inline s32 SceneBoot_WaitButton(SceneBoot *boot) {
    if (D_0047E360 == 0) {
        boot->stepFlag = 1;
    }
    if (boot->stepFlag != 0) {
        if (!(D_0047E37C & 0x4000)) {
            func_0037F510(boot);
            return 1;
        }
        boot->stepFlag = 0;
    }
    return 0;
}

/* Boot step: load the boot message texts. */
u32 func_0037FEF0(SceneBoot *boot) {
    VObject *res;
    VObject *loader;

    if (SceneBoot_WaitButton(boot)) {
        return 1;
    }
    boot->stepFlag = 0;
    res = gSystemData;
    loader = gFileLoader;
    VCALL(loader, 0x34, void (*)(VObject *, const char *, void *))(
        loader, sMsgBaseBin, VCALL(res, 0xC, void *(*)(VObject *))(res));
    D_01991EC0 = VCALL(res, 0xC, void *(*)(VObject *))(res);
    VCALL(loader, 0x34, void (*)(VObject *, const char *, void *))(
        loader, sMsgBaseTex, VCALL(res, 0x10, void *(*)(VObject *))(res));
    VCALL(gTexCache, 0x10, void (*)(VObject *, void *, s32))(
        gTexCache, VCALL(res, 0x10, void *(*)(VObject *))(res), 0x14);
    func_0037EC00(boot);
    D_0047B350 = 2;
    return 0;
}

/* Boot step (first): just the button wait. */
u32 func_00380410(SceneBoot *boot) {
    return SceneBoot_WaitButton(boot);
}

/* ---- logo steps: a 64-frame timer (stepTimer); the image is drawn on frames 0..0x3D and
 * the screen fades (colour at +0xC) around the last frames. ---- */

typedef struct BootScreen {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ u32 unk4;     /* 0x88000 for the fade */
    /* 0x08 */ u32 unk8;
    /* 0x0C */ u32 color;    /* 0x80808080 = normal, 0 = black */
    /* 0x10 */ u8 pad10[8];
    /* 0x18 */ s16 width;
    /* 0x1A */ s16 height;
} BootScreen;

static const char sLogoDolby[] = "SYSTEM\\LOGO_DOLBY.BIN";
static const char sCautionTex[] = "SYSTEM\\CAUTION.TEX";

#define BOOT_IMAGE_67C40(boot) ((u8 *)(boot) + 0x67C40)   /* second image buffer, inside logoCri's area */

/* The common frame handling of the logo steps. Returns 0 when the logo is done. */
static inline s32 BootLogo_Frame(SceneBoot *boot, BootScreen *scr) {
    switch (boot->stepTimer) {
    case 0x3F:
        scr->color = 0x80808080;
        return 0;
    case 0x3E:
        scr->unk4 = 0x88000;
        scr->color = 0;
        scr->width = 0x200;
        scr->height = 0x1C0;
        break;
    case 0x3D:
        scr->color = 0;
        break;
    case 1:
        scr->color = 0x80808080;
        break;
    }
    boot->stepTimer++;
    return 1;
}

/* Boot step: CRI logo (loaded by SceneBoot_StateLoadSystem); starts loading the Dolby logo. */
u32 func_0037FC60(SceneBoot *boot) {
    VObject *gs;
    BootScreen *scr;

    if (boot->stepTimer == 0 && VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) != 3) {
        return 1;
    }
    gs = gRenderer;
    scr = VCALL(gs, 0x2C, BootScreen *(*)(VObject *))(gs);
    if (boot->stepTimer == 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, const char *, void *, u32, s32))(
            gFileLoader, sLogoDolby, BOOT_IMAGE_67C40(boot), 0x10000000, 0);
        scr->color = 0;
        scr->width = 0x280;
        scr->height = 0x1C0;
        boot->stepTimer++;
    } else if (!BootLogo_Frame(boot, scr)) {
        return 0;
    }
    if ((u32)boot->stepTimer < 0x3E) {
        VCALL(gs, 0x48, void (*)(VObject *, void *, s32, s32, s32, s32))(
            gs, boot->logoCri, scr->width, scr->height, 0x88000, 0x34);
    }
    return 1;
}

/* Boot step: Dolby logo, once its load has finished. */
u32 func_0037FAC0(SceneBoot *boot) {
    VObject *gs;
    BootScreen *scr;

    if (VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) != 3) {
        return 1;
    }
    gs = gRenderer;
    scr = VCALL(gs, 0x2C, BootScreen *(*)(VObject *))(gs);
    if (boot->stepTimer == 0) {
        scr->color = 0;
        scr->width = 0x2D0;
        scr->height = 0x21C;
        boot->stepTimer++;
    } else if (!BootLogo_Frame(boot, scr)) {
        return 0;
    }
    if ((u32)boot->stepTimer < 0x3E) {
        VCALL(gs, 0x48, void (*)(VObject *, void *, s32, s32, s32, s32))(
            gs, BOOT_IMAGE_67C40(boot), scr->width, scr->height, 0x88000, 0x34);
    }
    return 1;
}

extern void *gSceneTable;          /* the scene table: scenes[] at +4, the scene heap at +0x10D9040 */
extern void *gMovie;          /* the movie playing (Movie, src/game/movie.c) */
extern void *D_0046ECC0[];        /* SceneMovie */
extern void *__nw__FUiPv(u32 size, void *p);   /* placement new */
extern void *func_002B70D0(void *movie);       /* Movie constructor */
extern void func_002B6D10(void *movie, const char *path, s32 mode, s32 keep);

static const char sCapcomSfd[] = "CAPCOM.SFD";
static const PTMF sSceneFinish = {0, 0x14, {(void *)0}};   /* virtual +0x14 */

#define SCENE_TABLE_SCENE(i) (*(Scene **)((u8 *)gSceneTable + 4 + (i) * 4))

/* Boot step: the Capcom logo movie, as scene 1, until it's over (Start skips it). */
u32 func_0037F7D0(SceneBoot *boot) {
    Scene *movie;
    void *mem;
    s32 ok;

    if (boot->stepTimer != 0) {
        if (gMovie != NULL && !(D_0047E37C & 8)) {
            return 1;
        }
        movie = SCENE_TABLE_SCENE(1);
        if (movie != NULL) {
            Scene_SetState(movie, &sSceneFinish);
            VCALL(SCENE_TABLE_SCENE(1), 0x14, void (*)(Scene *))(SCENE_TABLE_SCENE(1));
        }
        return 0;
    }
    {
        VObject *heap = (VObject *)((u8 *)gSceneTable + 0x10D9040);

        mem = VCALL(heap, 0x10, void *(*)(VObject *, u32))(heap, 0x600200);
    }
    if (mem != NULL) {
        movie = __nw__FUiPv(0x600200, mem);
        if (movie != NULL) {
            func_002B70D0(movie);
            movie->vtbl = D_0046ECC0;
        }
        SCENE_TABLE_SCENE(1) = movie;
        SCENE_TABLE_SCENE(1)->slot = 1;
        movie = SCENE_TABLE_SCENE(1);
        if (movie != NULL) {
            movie->request = SCENE_REQ_RUN;
            movie->status = 0;
            movie->waitFrames = 0;
            ok = 1;
        } else {
            ok = 0;
        }
    } else {
        ok = 0;
    }
    if (ok) {
        func_002B6D10(gMovie, sCapcomSfd, 1, 0);
    }
    boot->stepTimer++;
    return 1;
}

extern void SaveScreen_Init(BootCard *card, s32, s32);
extern void BootCard_Check(BootCard *card);

/* Boot step: run the object at +0xC7440 until it reports done (+0xC7444 < 0). */
u32 func_0037FE50(SceneBoot *boot) {
    if (boot->stepTimer == 0) {
        SaveScreen_Init(&boot->card, 0, 0);
        boot->card.state = 0;
        boot->card.hidden = 0;
        boot->stepTimer = 1;
    }
    if (boot->card.state < 0) {
        return 0;
    }
    BootCard_Check(&boot->card);
    return 1;
}


/* Boot step (last): the caution screen, shown with tasks[0] until frame 0x3F. */
u32 func_0037F980(SceneBoot *boot) {
    if (boot->stepTimer == 0x3F) {
        Task_Close(&boot->tasks[0]);
        return 0;
    }
    if (boot->stepTimer == 0) {
        VCALL(gFileLoader, 0x34, void (*)(VObject *, const char *, void *))(
            gFileLoader, sCautionTex, BOOT_IMAGE_67C40(boot));
        VCALL(gBootMessage, 0x10, void (*)(VObject *, s32, void *, s32))(
            gBootMessage, 6, BOOT_IMAGE_67C40(boot), 0);
    }
    boot->stepTimer++;
    if (boot->stepTimer != 0) {
        func_0037F2E0(boot);
        Task_Open(&boot->tasks[0], 1);
    }
    Task_Run(&boot->tasks[0]);
    return 1;
}

#define VIDEO_MODE_480P 0x50

extern void func_002CF390(void *obj, u32 alpha);

static const char sProgTex[] = "SYSTEM\\PROG.TEX";
static const char sCountFmt[] = "%d";

/* Boot step: progressive scan. Holding triangle + cross at boot asks whether to switch to 480p
 * (tasks[0] runs the question); after switching, the choice must be confirmed within 10 s
 * (countdown via tasks[2]) or the previous video mode is restored. */
u32 func_00380050(SceneBoot *boot) {
    VObject *gs;
    s32 result = 1;
    s32 counting = 0;
    u8 busy = boot->stepFlag;   /* low byte */
    Task *ask = &boot->tasks[0];

    switch (boot->stepTimer) {
    case 0:
        VCALL(gFileLoader, 0x34, void (*)(VObject *, const char *, void *))(
            gFileLoader, sProgTex, BOOT_IMAGE_67C40(boot));
        VCALL(gBootMessage, 0x10, void (*)(VObject *, s32, void *, s32))(
            gBootMessage, 6, BOOT_IMAGE_67C40(boot), 0);
        gs = gRenderer;
        boot->savedVideoMode = VCALL(gs, 0x28, u32 (*)(VObject *))(gs);
        if ((D_0047E374 & PAD_TRIANGLE) && (D_0047E374 & PAD_CROSS)) {
            boot->stepTimer = 1;
        } else {
            result = 0;
        }
        break;
    case 1:
        /* ask: switch to progressive? */
        Task_Open(ask, 9);
        Task_Update(ask);
        boot->stepTimer = 2;
        /* fall through */
    case 2:
        if (busy || ask->mode) {
            break;
        }
        if (ask->answer != 0) {
            result = 0;   /* answered no */
            break;
        }
        boot->countdown = 300;
        boot->stepTimer = 3;
        VCALL(gRenderer, 0x24, void (*)(VObject *, u32))(gRenderer, VIDEO_MODE_480P);
        break;
    case 3:
        /* ask: keep this mode? */
        Task_Open(ask, 10);
        Task_Update(ask);
        boot->stepTimer = 4;
        /* fall through */
    case 4:
        if (boot->countdown != 0 && --boot->countdown == 0) {
            /* timed out: back to the old mode, ask again */
            VCALL(gRenderer, 0x24, void (*)(VObject *, u32))(gRenderer, boot->savedVideoMode);
            boot->stepTimer = 1;
        }
        if (busy || ask->mode) {
            counting = 1;
            break;
        }
        if (ask->answer != 0) {
            /* answered no: back to the old mode, ask again */
            VCALL(gRenderer, 0x24, void (*)(VObject *, u32))(gRenderer, boot->savedVideoMode);
            boot->stepTimer = 1;
            break;
        }
        result = 0;
        break;
    }

    if (boot->stepFlag == 0) {
        Task_Update(ask);
    }
    Task_Draw(ask);
    if (counting) {
        u32 secs = (u8)(boot->countdown / 30);

        if ((s32)secs >= 10) {
            secs = 9;
        }
        Task_PrintfEx(&boot->tasks[2], 0xF8, 0xB3, 0, 0x80, 0x30, sCountFmt, secs & 0xFF);
    }

    /* same button wait as the other steps, but drawn through tasks[1] */
    if (D_0047E360 == 0) {
        boot->stepFlag = 1;
    }
    busy = 0;
    if (boot->stepFlag != 0) {
        if (!(D_0047E37C & PAD_CROSS)) {
            Task_ShowMessage(&boot->tasks[1], 8, 0, 0x80, 0x33);
            busy = 1;
        } else {
            boot->stepFlag = 0;
        }
    }
    func_002CF390(&boot->unkC75D0Vtbl, (u32)(busy ? 0x5F : 0) << 24);
    VCALL(gRenderer, 0xC, void (*)(VObject *, void *, s32, s32))(gRenderer, &boot->unkC75D0Vtbl, 0x31, 0);
    if (boot->stepTimer != 0 && ask->mode) {
        func_0037F2E0(boot);
    }
    return busy ? busy : result;
}

/* ---- the item found ---- */

#include "sound.h"

extern void Task_DrawBox(Task *t, s32 x, s32 y, s32 w, s32 h, s32 alpha, s32 layer);

extern u8 func_00322CD0(u8 *o, s32 shown);              /* the screen's step: bit 0 up, bit 7 done */
extern void func_00323510(u8 *o, u8 row, u8 v, s32 a);
extern void func_003230B0(u8 *o, u8 k, u8 v);
extern s32 func_00322970(u8 *o);
extern void func_00261090(u8 *items, s32 id, s32 n);    /* an item added (n of it) */
extern const u8 D_00460A00[22];
extern const char D_00463AB0[];                         /* its count's format */
extern const PTMF D_0044AE10, D_0044AE20;

/* state: an item found (+0x14A its place, -1 none; +0x14B how many; +0x14C its id, big endian):
 * the screen's rows and cells refreshed; once up, "obtained" with the item's name (and count)
 * in a box, its sound played once (+0x152) - or message 5 for none; when done and confirmed,
 * the item added, the places cleared (but +0x150), the task closed and the next state
 * (func_00322970: D_0044AE10, else D_0044AE20) */
void func_0037ED50(u8 *o) {
    Task *t = (Task *)(o + 0x14);
    u8 flags = func_00322CD0(o, (s8)o[0x14A] >= 0);
    s32 id = -1, i;

    for (i = 0; i < 10; i++) {
        func_00323510(o, i, o[0x120 + i], 0);
    }
    for (i = 0; i < 22; i++) {
        u8 k = D_00460A00[i];
        s8 c = o[0x12F + k];

        if (c != -1 && c != 4) {
            func_003230B0(o, k, c);
        }
    }
    if (flags & 1) {
        if ((s8)o[0x14A] < 0) {
            Task_Open(t, 5);
            Task_Run(t);
        } else {
            u16 w1, w2, cw;
            s32 x;

            if ((s8)o[0x152] == 0) {
                Sound_Play(gSound, 3, 6);
                o[0x152] = 1;
            }
            id = o[0x14C] << 24 | o[0x14D] << 16 | o[0x14E] << 8 | o[0x14F];
            w1 = Task_MessageWidth(t, (id + 0x8100) & 0xFFFF, 0x10);
            cw = (s8)o[0x14B] == 1 ? 0 : 0x1B;
            w2 = Task_MessageWidth(t, 4, 0x10);
            x = (u16)(0x100 - (s32)(w1 + cw + w2) / 2);
            Task_DrawBox(t, 0x100, 0x172, 0x100, 5, 0x70, 0x30);
            Task_ShowText(t, x, 0x164, 0, Task_MessageText(t, 4), 0x80, 0x30, 0x10, 0x15);
            Task_ShowText(t, x + w2, 0x164, 7, Task_MessageText(t, (id + 0x8100) & 0xFFFF), 0x80, 0x30, 0x10, 0x15);
            if (cw != 0) {
                Task_Printf(t, w1 + (x + w2), 0x164, 0, D_00463AB0, (s8)o[0x14B]);
            }
        }
    }
    if ((flags & 0x80) && (D_0047E36C & MENU_CONFIRM)) {
        if (id != -1) {
            func_00261090((u8 *)gItems + 8, id, o[0x14B]);
        }
        for (i = 0; i < 0x40; i++) {
            if (i != 0x38) {
                o[0x118 + i] = 0xFF;
            }
        }
        Task_Close(t);
        AT(o, 0x10, s32) = 0;
        if (func_00322970(o)) {
            ptmf_set((PTMF *)(o + 0x4), &D_0044AE10);
        } else {
            ptmf_set((PTMF *)(o + 0x4), &D_0044AE20);
        }
    }
}
