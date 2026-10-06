/* Mode 3 scene: gameplay. See docs/structure.md for the member layout. */
#include "common.h"
#include "game.h"
#include "navmesh.h"
#include "progress.h"
#include "globals.h"
#include "memcard.h"
#include "ptmf.h"
#include "pursuer.h"
#include "effectmgr.h"
#include "texcache.h"
#include "charaction.h"
#include "gl2d.h"
#include "bgm.h"
#include "bootcard.h"
#include "camctl.h"
#include "chainpool.h"
#include "char_load.h"
#include "creature.h"
#include "doors.h"
#include "event.h"
#include "gameover.h"
#include "heap.h"
#include "hewie.h"
#include "items.h"
#include "loading.h"
#include "model.h"
#include "movie.h"
#include "music.h"
#include "panic.h"
#include "pause.h"
#include "placed.h"
#include "room_map.h"
#include "runtime.h"
#include "scene.h"
#include "scene_game.h"
#include "scene_game_members.h"
#include "scene_title.h"
#include "skeleton.h"
#include "system.h"
#include "unsorted.h"
#include "draw_leaves.h"
#include "libc.h"
#include "msl.h"
#include "sce/eekernel.h"

/* Field at a byte offset, for SceneGame members whose types aren't known yet. */

/* SceneGame member offsets */
#define SG_PROGRESS 0x40        /* Progress (second base class) */
#define SG_FIONA 0xC88840       /* Fiona */
#define SG_PARTNER 0xE35F80     /* second character (Hewie?) */
#define SG_SETTING_A30 0xA30    /* f32 0..1, also given to gSound +0xAC */
#define SG_ENTRY 0x6FC254       /* = Progress +0x6FC214: how the scene was entered (Game modeParam) */
#define ENTRY_FLAG 0x40000000   /* top bits of the entry value */
#define ENTRY_NEW -1            /* -1: start from the resident save buffer */

extern const PTMF sGameStateNull;
extern const PTMF SceneGame_SubStartRoom_ptmf;   /* stored at +0x1053450 */
extern const PTMF SceneGame_StateMain_ptmf;   /* next state */

extern void *gChainPool;
extern void *gSkelPool;

/* Fiona setups, by progress variable 0x26 (0..8), and the partner's, by variable 0x27 (0..2).
 * Probably costumes; each takes the character's player index. */

extern void *D_004699E0[];
extern void *Helper469D00_vtable[];
extern void *D_0046A0D0[];
extern void *D_0046B350[];
extern void *D_0046BB20[];
extern void *D_0046C6F0[];
extern void *D_0046C770[];
extern void *D_0046D780[];
extern void *Message_vtable[];
extern void *Cutscene_vtable[];
extern void *D_00476F40[];
void *SceneTableBase_dtor(u8 *o, s32 flags);
void *Obj46B350_dtor(u8 *o, s32 flags);
void *Obj46C6F0_dtor(u8 *o, s32 flags);
void *RoomMeshes_dtor(u8 *o, s32 flags);
void *Obj46D780_dtor(u8 *o, s32 flags);
void *Message_dtor(u8 *o, s32 flags);
void *Cutscene_dtor(u8 *o, s32 flags);
void *LoadingEmblem_dtor(u8 *o, s32 flags);

extern void *D_0046BA68[], *RoomBase_vtable[];
extern void *Overlay_vtable[];
extern u8 D_0046BAA0[], D_0046BA80[], NavGroups_vtable[];
extern u8 Rooms_vtable[];
extern void *NavMesh_vtable[];
extern void *gRoomEventObj;
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *Obj46BA68_ctor(u8 *p);
void *Cutscene_ctor(u8 *p);
void *RoomBase_ctor(u8 *p);
void *DimOverlay_ctor(u8 *p);
void *Events_ctor(u8 *p);
void *Obj46BA80_ctor(u8 *p);
void *NavGroups_ctor(u8 *p);
void *Rooms_ctor(u8 *p);
void *NavMesh_ctor(u8 *p);
void *Obj46D780_ctor(u8 *p);

extern void *D_0046A9B0[];
extern u8 Creatures_vtable[];
extern u8 PlacedThings_vtable[];
extern void *BlockPool_vtable[];
extern void *Heap_vtable[];
void *RoomMeshes_ctor(u8 *p);
void *Obj46A9B0_ctor(u8 *p);
void *Creatures_ctor(u8 *p);
void *PlacedThings_ctor(u8 *p);
void *Message_ctor(u8 *p);
void *SceneHeap_ctor(u8 *p);
void *Progress_ctor(u8 *p);

#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

void AvoidPrompt_Show(u8 *self, u32 id);
s32 AvoidPrompt_Unload(void *self);
void AvoidPrompt_Tick(u8 *self);
s32 AvoidPrompt_Upload(u8 *self);

extern void *Kind33Model_vtable[];

s32 RoomBase_CharEnterScript(void *o);
s32 RoomBase_EnterScript(void *o);

/* destructor (vtable D_004699E0) */
/* 0x00120EF0 */
void *SceneTableBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004699E0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}
static inline void Scene_SetState(Scene *scene, const PTMF *state) {
    ptmf_set(&scene->state, state);
}

/* Character setup helper: the character's index (+0x20) must be 0 or 1. */
#ifdef HG_NATIVE
extern s32 hg_debug_no_partner(void);
extern s32 hg_debug_freeplay(void);
#endif

#ifdef HG_NATIVE
static u8 sDebugPlace;   /* HG_ROOM start in a room no event places Fiona in */
#endif

static inline void SetupCharacter(Progress *prog, void *chr, void (*setup)(Progress *, u32)) {
    u32 index = AT(chr, 0x20, u32);

    if (index < 2) {
        Characters_Register(prog, index, chr);
        setup(prog, AT(chr, 0x20, u32));
    }
}

/* Vtable +0x10: set up the game world, characters and subsystems. */
void SceneGame_StateEntry(Scene *game) {
    Progress *prog = &AT(game, SG_PROGRESS, Progress);
    void *fiona = (u8 *)game + SG_FIONA;
    void *partner = (u8 *)game + SG_PARTNER;
    VObject *obj550 = gRandom;
    u8 *save = (u8 *)gGamePtr + 0x190;
    void *obj980;
    VObject *obj560;
    f32 *settingDst;

    VCALL(obj550, 0xC, void (*)(VObject *, s32))(obj550, 0x1571);
    ChainPool_FreeAll(gChainPool);
    SkelPool_FreeAll(gSkelPool);
    Progress_Reset((u8 *)prog);
    AT(game, 0xF6CD29, u8) = 0;
    RoomMgr_ctor((u8 *)game + 0x73EE80);
    AT(game, 0xF6C1B0, s32) = 0;

    if (AT(game, SG_ENTRY, s32) != ENTRY_NEW) {
        Progress *p;
        s32 flag;

#ifdef HG_NATIVE
        if (!hg_debug_no_partner())   /* native/platform/debug.c: HG_NOPARTNER */
#endif
        SetupCharacter(prog, partner, CharLoad_PartnerModel);
        p = gProgress;
        Progress_SetVar(p, 0x27, 0);
        flag = (AT(game, SG_ENTRY, s32) & ENTRY_FLAG) ? 1 : 0;
        AT(game, SG_ENTRY, u32) &= 0x3FFFFFFF;
        Progress_SetFlag(prog, 0x28);
        switch (AT(game, SG_ENTRY, s32)) {
        case 0x2A:
            SetupCharacter(prog, fiona, CharLoad_FionaModel);
            Progress_SetVar(p, 0x26, 1);
            AT(game, 0xF6CD28, u8) = 0;
            Progress_SetFlag(prog, 3);
            Progress_SetFlag(prog, 8);
            break;
        case 0x37:
            SetupCharacter(prog, fiona, CharLoad_FionaClothes);
            Progress_SetVar(p, 0x26, 0);
            AT(game, 0xF6CD28, u8) = 0;
            Progress_SetFlag(prog, 8);
            if (AT(game, 0xF6B6B2, u8) == 0xFF) {
                AT(game, 0xF6B6B2, u8) = 0;
            }
            break;
#ifdef HG_NATIVE
        default:   /* HG_ROOM debug start in any other room: Fiona as in a new game, placed
                    * once the room is in (SceneGame_EnterRoom) */
            sDebugPlace = 1;
            SetupCharacter(prog, fiona, CharLoad_FionaModel);
            Progress_SetVar(p, 0x26, 1);
            AT(game, 0xF6CD28, u8) = 0;
            break;
#endif
        }
        if (flag) {
            AT(game, 0x70, s32) |= 0x8000;
        }
    } else {
        Progress *p;

        AT(game, 0xF6CD28, u8) = 1;
        Progress_CopyState(save + 0x50, (u8 *)game + 0x48);
        p = gProgress;
        switch (Progress_GetVar(p, 0x26) & 0xFF) {
        case 0: SetupCharacter(prog, fiona, CharLoad_FionaClothes); break;
        case 1: SetupCharacter(prog, fiona, CharLoad_FionaModel); break;
        case 2: SetupCharacter(prog, fiona, CharLoad_Costume2); break;
        case 3: SetupCharacter(prog, fiona, CharLoad_Costume3); break;
        case 6: SetupCharacter(prog, fiona, CharLoad_Costume6); break;
        case 7: SetupCharacter(prog, fiona, CharLoad_Costume7); break;
        case 8: SetupCharacter(prog, fiona, CharLoad_Costume8); break;
        }
        switch (Progress_GetVar(p, 0x27) & 0xFF) {
        case 0: SetupCharacter(prog, partner, CharLoad_PartnerModel); break;
        case 1: SetupCharacter(prog, partner, CharLoad_DogModelA); break;
        case 2: SetupCharacter(prog, partner, CharLoad_DogModelB); break;
        }
        CharLoad_Partner(prog, save[0x1A]);
    }

    Progress_LoadSoundSet(prog, (Progress_TestFlag(prog, 3) & 0xFF) == 1);
    Progress_StartChars(prog);
    Progress_LoadChars(prog);
    Lights_RoomStart((u8 *)game + 0xF6C1C0);
    obj980 = gAdx;
    Bgm_Init(obj980, (u8 *)game + 0x1030240);
    BgmCtl_ctor((u8 *)game + 0x1053424);
    AT(game, 0x1053440, PTMF) = sGameStateNull;
    Creatures_LoadModel((u8 *)game + 0x706480, &sGameStateNull);
    AT(game, 0x105344D, u8) = 0;
    AT(game, 0x105344E, u8) = 0;
    VCALL(gRumble, 0x10, void (*)(VObject *))(gRumble);
    AT(game, 0x106503C, s32) = 0;
    VCALL(gCamera, 0xC, void (*)(VObject *))(gCamera);
    obj560 = gSound;
    VCALL(obj560, 0x94, void (*)(VObject *, s32))(obj560, 0xFF);
    VCALL(obj560, 0xAC, void (*)(VObject *, f32))(obj560, AT(game, SG_SETTING_A30, f32));
    settingDst = &AT(obj980, 0x120, f32);
    *settingDst = AT(game, SG_SETTING_A30, f32);
    if (*settingDst < 0.0f) {
        *settingDst = 0.0f;
    }
    if (!(*settingDst <= 1.0f)) {
        *settingDst = 1.0f;
    }
    Bgm_ApplyVolume(obj980);
    AT(game, 0x1065040, s32) = VCALL(obj550, 0x10, u32 (*)(VObject *))(obj550) & 0xFF;
    Progress_ClearFlag(prog, 0x2B);
    Progress_ClearFlag(prog, 0x2C);
    AT(game, 0x1053450, PTMF) = SceneGame_SubStartRoom_ptmf;
    Scene_SetState(game, &SceneGame_StateMain_ptmf);
}

/* ---- construction ---- */

#include "actor.h"
#include "subscreen.h"
#include "task.h"

extern void *Scene_vtable[], *SceneGame_vtable[], *Progress_vtable[], *Fiona_vtable[];
extern void *D_0047A7E8[];          /* SceneGame's second base (Progress) */
extern void *Actor_vtable[], *Character_vtable[];   /* Actor, Character */
extern void *Hewie_vtable[];          /* the partner (Hewie) */
extern void *PathPlan_vtable[], *RoutePlanner_vtable[], *Events_vtable[], *D_0046B3B8[], *Lights_vtable[];
extern void *D_0046C6F0[], *D_0046C660[], *CamDirector_vtable[];
extern void *D_004699E0[], *BlockPool_vtable[], *Heap_vtable[];
extern void *SubScreen_vtable[];          /* the sub screen */
extern void *BgmCtl_vtable[];          /* the music controller */
extern void *D_00473440[];
extern const PTMF sSceneEntryState;
extern void *gSceneGameF29740;
extern void *gMovieFlag, *gAvoidPrompt;

/* the characters' common construction (Actor, then Character) */
static inline void Character_Construct(Character *c, s32 slot) {
    c->a.vtbl = Actor_vtable;
    c->a.slot = slot;
    c->a.flags24 = 0x2000000;
    c->a.vtbl = Character_vtable;
    c->pathReq = NULL;
}

Scene *SceneGame_ctor(Scene *g) {
    u8 *prog, *m, *o;
    Character *c;
    SubScreen *sub;
    s32 i;

    g->vtbl = Scene_vtable;
    ptmf_set(&g->state, &sSceneEntryState);

    /* the Progress base (+0x40) and its members */
    prog = (u8 *)g + SG_PROGRESS;
    Progress_ctor(prog);
    AT(prog, 0x0, void **) = Progress_vtable;
    Base_ctorNoop(prog + 8);
    AT(prog, 0x1FBEC0, u8) = 0;
    AT(prog, 0x1FBEC1, u8) = 0;
    SceneHeap_ctor(prog + 0x6FBF00);
    Message_ctor(prog + 0x6FC218);
    PlacedThings_ctor(prog + 0x6FC340);
    Creatures_ctor(prog + 0x706440);
    Obj46A9B0_ctor(prog + 0x73EB00);
    DimMessage_ctor(prog + 0x73EB60);
    Progress_Reset((u8 *)((Progress *)prog));
    Heap_Setup((VObject *)(prog + 0x6FBF00), prog + 0x1FBF00, 0x500000, prog + 0x6FBF14, 0x22);
    AT(prog, 0x6FC214, s32) = 0x2A;   /* the entry: room 0x2A (a new game) */
    g->vtbl = SceneGame_vtable;
    AT(g, SG_PROGRESS, void **) = D_0047A7E8;

    m = (u8 *)g + 0x73EE80;
    RoomMeshes_ctor(m);
    Obj46D780_ctor(m + 0x340);
    NavMesh_ctor(m + 0x3E0);
    Rooms_ctor(m + 0x4F0);
    Doors_ctor(m + 0x1640);
    PlacedObjects_ctor(m + 0x6740);
    NavGroups_ctor(m + 0x9360);
    Obstacles_ctor(m + 0x9380);
    RoomMgr_ctor(m);

    /* the player (Fiona) and the partner (Hewie) */
    c = (Character *)((u8 *)g + SG_FIONA);
    Character_Construct(c, 0);
    c->unk153C = 0;
    c->a.vtbl = Fiona_vtable;
    c = (Character *)((u8 *)g + SG_PARTNER);
    Character_Construct(c, 1);
    gSceneGameF29740 = (u8 *)g + 0xF29740;
    c->unk153C = 1;
    c->a.vtbl = Hewie_vtable;
    AT(g, 0xF29740, void **) = PathPlan_vtable;
    gRoutePlanner = (VObject *)((u8 *)g + 0xF6A940);
    AT(g, 0xF6A940, void **) = RoutePlanner_vtable;

    o = (u8 *)g + 0xF6AFB0;
    Obj46BA80_ctor(o);
    Events_ctor(o + 0xC);
    AT(o, 0x0, void **) = Events_vtable;
    AT(o, 0xC, void **) = D_0046B3B8;
    DimOverlay_ctor(o + 0x20);
    func_00100340(o + 0x120, RoomBase_ctor, RoomBase_dtor, 4, 0x110);
    Task_ctor((Task *)(o + 0x708));
    Cutscene_ctor(o + 0x938);
    func_00100340(o + 0xBF0, Obj46BA68_ctor, Obj46BA68_dtor, 0x30, 0x20);
    AT(o, 0x702, u8) = 0xFF;
    for (i = 0; i < 17; i++) {
        AT(o, 0x564 + i * 0x18, s32) = 0;
    }
    gLights = (VObject *)((u8 *)g + 0xF6C1C0);
    AT(o, 0x6FC, s32) = 0;

    o = (u8 *)g + 0xF6C1C0;
    AT(o, 0x0, void **) = Lights_vtable;
    AT(o, 0x320, s32) = -1;
    AT(o, 0x324, s32) = -1;

    o = (u8 *)g + 0xF6CBB0;
    AT(o, 0x8, s32) = 0;
    AT(o, 0xC, s32) = 0;
    AT(o, 0x10, s32) = 0;
    AT(o, 0x14, s32) = 0;
    AT(o, 0x18, s32) = 0;
    AT(o, 0x1C, s32) = 0;
    AT(o, 0x20, s32) = 0;
    AT(o, 0x2C, s32) = 0;
    AT(o, 0x38, s32) = 0;
    AT(o, 0x50, f32) = 6.0f;
    AT(o, 0x60, void **) = D_0046C6F0;
    AT(o, 0x64, void **) = D_0046C660;
    AT(o, 0x60, void **) = CamDirector_vtable;
    gCamDirector = (VObject *)(o + 0x60);

    o = (u8 *)g + 0xF6CD30;
    AT(o, 0x1400, void **) = D_004699E0;
    AT(o, 0x1404, s32) = 0;
    gRoomEffects = o;
    AT(o, 0x1408, s32) = 0;
    AT(o, 0x1400, void **) = BlockPool_vtable;
    AT(o, 0x140C, s32) = 0;
    gEffects = (u8 *)g + 0xF6E200;   /* a sub-heap (its header after its 64 KB) */
    AT(o, 0x1410, s32) = 0;
    AT(o, 0x1414, s32) = 0;
    o = (u8 *)g + 0xF6E200;
    AT(o, 0x10000, void **) = D_004699E0;
    AT(o, 0x10004, s32) = 0;
    AT(o, 0x10008, s32) = 0;
    AT(o, 0x10000, void **) = Heap_vtable;
    AT(o, 0x1000C, s32) = 0;
    AT(o, 0x10010, s32) = 0;

    /* the sub screen (the in-game menu) */
    sub = (SubScreen *)((u8 *)g + 0xF87240);
    SubScreenBase_ctor(sub);
    sub->vtbl = SubScreen_vtable;
    Task_ctor(&sub->ask);
    Task_ctor(&sub->text);
    TextObj_ctor(sub->textObj);
    BootCard_ctor(&sub->card);

    /* the music controller */
    gMusic = (VObject *)((u8 *)g + 0x1053424);
    AT(g, 0x1053424, void **) = BgmCtl_vtable;
    gMovieFlag = (u8 *)g + 0x105344C;

    o = (u8 *)g + 0x1053480;
    gAvoidPrompt = o;
    AT(o, 0x0, void **) = D_00473440;
    Task_Construct((Task *)(o + 0x11048));
    return g;
}

/* destructor (vtable Cutscene_vtable) */
/* (possibly dead code: nothing in the game references it) */
/* 0x002D0C70 */
void *Cutscene_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Cutscene_vtable;
        AT(o, 0x0, void **) = D_0046BB20;
        gCutscene = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* vtable +0xC: count the frame (+0x73EE40), then the scene's request / state machine */
void SceneGame_Update(Scene *g, s32 arg) {
    AT(g, 0x73EE40, s32)++;
    Scene_Update(g, arg);
}

#include "input.h"

extern void *D_00476F40[], *Helper469D00_vtable[];
extern void SceneGame_RoomIn(Scene *g);

/* the frame counted (twice while a button is held): it feeds the random numbers, seeded here */
static inline void rng_tick(Scene *g) {
    u8 rng[0x80] __attribute__((aligned(16)));

    AT(g, 0x1065040, s32)++;
    if ((u16)gPadPressed != 0) {
        AT(g, 0x1065040, s32)++;
    }
    AT(rng, 0x0, void **) = D_00476F40;
    AT(rng, 0x4, s32) = -1;
    Loading_DrawFrame(rng, AT(g, 0x1065040, s32));
    AT(rng, 0x0, void **) = Helper469D00_vtable;
}

/* state, every frame: count the frame (twice while a button is pressed: it feeds the random
 * numbers, seeded here), then the sub-state (+0x1053450: the room load) if any, else the
 * gameplay tick */
/* 0x003A06E0 */
void SceneGame_StateMain(Scene *g) {
    rng_tick(g);
    if (ptmf_test(&AT(g, 0x1053450, PTMF))) {
        ptmf_scall(g, &AT(g, 0x1053450, PTMF));
        return;
    }
    SceneGame_RoomIn(g);
}

extern void SceneGame_LoadedStart(Scene *g);
extern const PTMF SceneGame_SubWaitRoom_ptmf; /* { 0, -1, SceneGame_SubWaitRoom } */

/* sub-state: start the room. Once the progress data is ready: reset the sub screen, the
 * members, the characters; when continuing from a save (+0xF6CD28), its entry room and the
 * sub screen's state from the system data; then request the room's file (the room buffers,
 * +0x73EE80), put the characters in the room, and wait for it (SceneGame_SubWaitRoom). Returns 1 while
 * the progress data isn't ready. */
/* 0x003A04A0 */
s32 SceneGame_SubStartRoom(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    s32 i;

    if (Progress_AnyLoading(prog)) {
        return 1;
    }
    SubScreen_Start((SubScreen *)((u8 *)g + 0xF87240));
    GameOver_Reset((u8 *)g + 0x73EB40);
    AvoidPrompt_Load((u8 *)g + 0x1053480);
    if (AT(g, 0xF6CD28, u8) != 1) {
        Progress_ActivateChar(prog, 1);
        VCALL(gRooms, 0xC, void (*)(VObject *, void *))(gRooms, NULL);
    } else {
        u8 *save = (u8 *)gGamePtr + 0x190;

        AT(g, SG_ENTRY, s32) = AT(gGamePtr, 0x194, s32);
        SubScreen_FromSave((SubScreen *)((u8 *)g + 0xF87240), save);
        VCALL(gRooms, 0xC, void (*)(VObject *, void *))(gRooms, save + 0x1010);
    }
    Progress_ActivateChar(prog, 0);
    Progress_CharsEnterRoom(prog);
    VCALL(gObstacles, 0xC, void (*)(VObject *))(gObstacles);
    Events_InstallRooms((u8 *)g + 0xF6AFB0);
    SceneGame_LoadedStart(g);
    RoomMgr_LoadRoom((u8 *)g + 0x73EE80, AT(g, SG_ENTRY, s32), AT(g, 0xF6C1B0, s32));
    PathPlanHolder_NewRoom((u8 *)g + 0xF29740);
    for (i = 0; i < 6; i++) {
        if (gCharacters[i] != NULL) {
            AT(gCharacters[i], 0x30, s32) = AT(g, SG_ENTRY, s32);   /* their room */
        }
    }
    AT(g, 0xF6CD20, u8) = 0xFF;
    AT(g, 0xF6CD24, s32) = 0;
    RoomEffects_Reset((u8 *)g + 0xF6CD30);
    EffectMgr_Reset((u8 *)g + 0xF6E200);
    AT(g, 0x1053450, PTMF) = SceneGame_SubWaitRoom_ptmf;
    return 0;
}

/* after a room load: on a game loaded from a save (+0xF6CD28 1, ignoring bit 0x80) pick the start variant
 * (vt+0xF8) from the unlocked bonus flags in the progress (+0x1C/+0x24/+0x2C), then start
 * +0x106503C (vt+0xC); nothing on mode 0 */
/* 0x0039AD90 */
void SceneGame_LoadedStart(Scene *g) {
    u8 *prog = (u8 *)g + SG_PROGRESS;
    u8 mode = AT(g, 0xF6CD28, u8) & ~0x80;
    VObject *o;

    if (mode == 1) {
        if (AT(prog, 0x2C, u32) & 0x100000) {
            VCALL(g, 0xF8, void (*)(Scene *, s32))(g, 3);
        } else if (AT(prog, 0x2C, u32) & 0x4) {
            VCALL(g, 0xF8, void (*)(Scene *, s32))(g, 2);
        } else if (AT(prog, 0x24, u32) & 0x2) {
            VCALL(g, 0xF8, void (*)(Scene *, s32))(g, 1);
        } else if (AT(prog, 0x1C, u32) & 0x1000) {
            VCALL(g, 0xF8, void (*)(Scene *, s32))(g, 0);
        }
    } else if (mode == 0) {
        return;
    }
    o = AT(g, 0x106503C, VObject *);
    if (o != NULL) {
        VCALL(o, 0xC, void (*)(VObject *))(o);
    }
}

/* room-load state 2: wait for the room (and the loader) to finish; on a loaded game, also
 * start the save's second room into the other slot; then back to gameplay */
/* 0x003A0390 */
s32 SceneGame_SubWaitRoom(Scene *g) {
    if (RoomMgr_SlotLoading((u8 *)g + 0x73EE80, AT(g, 0xF6C1B0, s32))) {
        return 1;
    }
    if (VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) != 3) {
        return 1;
    }
    if (AT(g, 0xF6CD28, u8) == 1) {
        RoomMgr_LoadRoom((u8 *)g + 0x73EE80, AT(gGamePtr, 0x198, s32),
                      (u8)(AT(g, 0xF6C1B0, s32) == 0));
    }
    AvoidPrompt_Upload((u8 *)g + 0x1053480);
    Creatures_HookMessages((u8 *)g + 0x706480);
    AT(g, 0x1053450, PTMF) = sGameStateNull;
    return 0;
}

extern const PTMF SceneGame_StatePlay_ptmf;   /* the scene's gameplay state */
extern const PTMF SceneGame_SubPlay_ptmf2;   /* its gameplay sub-state (+0x1053440) */
extern void SceneGame_EnterRoom(Scene *g);

/* the room is in: the characters in it enter (+0x38), the room's resources are set up, the
 * player is reset once per scene (+0xF6CD28 bit 0x80), then on to gameplay */
/* 0x003A0160 */
void SceneGame_RoomIn(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    s32 i;

    AT(g, 0x44, s32) = 0;
    AT(g, 0x73EE40, s32) = 0;
    CamDirector_NewRoom((u8 *)g + 0xF6CBB0);
    Progress_CameraOn(prog, 0);
    SceneGame_EnterRoom(g);
    if (AT(g, 0xF6CD28, u8) == 1) {
        VObject *sub = (VObject *)((u8 *)g + 0xF87240);

        VCALL(sub, 0x2C, void (*)(VObject *, s32))(sub, AT(gGamePtr, 0x1A80, s8));
    }
    for (i = 0; i < 3; i++) {
        if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) == 1) {
            VCALL(gCharacters[i], 0x38, void (*)(void *))(gCharacters[i]);
        }
    }
    Creatures_EnterRoom((u8 *)g + 0x706480);
    VCALL(g, 0xDC, void (*)(Scene *))(g);
    Progress_CharRequests(prog);
    CamDirector_RoomStart((u8 *)g + 0xF6CBB0, AT(g, 0x74881C, s32));
    if (!(AT(g, 0xF6CD28, u8) & 0x80)) {
        Character_ChooseExit((Character *)((u8 *)gCharPlayer), 0xFF);
        AT(g, 0xF6CD28, u8) |= 0x80;
    }
    Record20_Clear((u8 *)g + 0x16B4);
    Record20_Clear((u8 *)g + 0x16D4);
    ptmf_set(&g->state, &SceneGame_StatePlay_ptmf);
    ptmf_set(&AT(g, 0x1053440, PTMF), &SceneGame_SubPlay_ptmf2);
}

extern const s32 D_0044C6E0[]; /* rooms flagged at +0x1FBF00 (-1 terminated) */
extern void *CreatureA_ctor(void *o);   /* creature constructors: slots 0..6 */
extern void *CreatureB_ctor(void *o);   /* slots 7..9 */

#define SG_CONTROL 0x1FBF01   /* u8: 0 Fiona is controlled, else Hewie */
#define SG_ROOMFLAG 0x1FBF00

/* enter the room: sounds, the room slot, (loaded game: the save's character and creature
 * state), the controlled character's room, per-room resets, the characters' room entry
 * (Progress +0x34), the event system, and the room flag +0x1FBF00 */
/* 0x0039D310 */
void SceneGame_EnterRoom(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    VObject *snd;
    s32 room;
    s32 i;
    const s32 *t;

    if (!(u8)Progress_TestFlag(prog, 0x27)) {
        snd = gSound;
        VCALL(snd, 0x10, void (*)(VObject *, s32, s32))(snd, 0, 0x1B0C00);
        VCALL(snd, 0x84, void (*)(VObject *, s32))(snd, 6);
        VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 6);
    }
    RoomMgr_MakeCurrent((u8 *)g + 0x73EE80, AT(g, 0xF6C1B0, s32));
    PathPlanHolder_NewRoom((u8 *)g + 0xF29740);
#ifdef HG_NATIVE
    if (sDebugPlace) {   /* debug start: Fiona at the first door's way in (rooms +0x34), else on
                          * the middle of walk-mesh triangle 0 */
        NavMesh *nm = gNavMesh;
        f32 pos[4] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 1.0f};
        f32 dir[4] __attribute__((aligned(16))) = {0.0f, 0.0f, 0.0f, 0.0f};
        u32 tri = NAV_NONE;
        s32 k;

        sDebugPlace = 0;
        for (k = 0; k < 8 && tri == NAV_NONE; k++) {
            tri = VCALL(gRooms, 0x34, u32 (*)(VObject *, u32, f32 *))(gRooms, k, pos);
        }
        if (tri == NAV_NONE && nm != NULL && nm->tris != NULL && nm->numTris > 0) {
            tri = 0;
            for (k = 0; k < 3; k++) {
                pos[0] += nm->tris[0].v[k][0] / 3.0f;
                pos[1] += nm->tris[0].v[k][1] / 3.0f;
                pos[2] += nm->tris[0].v[k][2] / 3.0f;
            }
        }
        if (tri != NAV_NONE) {
            VCALL(gCharPlayer, 0x28, void (*)(void *, s32, void *, void *))(gCharPlayer, tri, dir, pos);
        }
    }
#endif
    if (AT(g, 0xF6CD28, u8) == 1) {
        u8 *rd = gGamePtr;
        u8 *save = rd + 0x190;
        Progress *gp;
        u8 *cs;
        u8 *stalker;
        u8 *cr;

        AT(gCharPlayer, 0xE8, s32) = AT(rd, 0x1A0, s32);
        AT(gCharPlayer, 0xEC, s32) = AT(rd, 0x1A4, s32);
        VCALL(gCharPlayer, 0x28, void (*)(void *, s32, void *, void *))(
            gCharPlayer, AT(rd, 0x19C, s32), save + 0x44, save + 0x30);
        Character_ChooseExit((Character *)((u8 *)gCharPlayer), 0xFF);
        gp = gProgress;
        cs = (u8 *)gp + 0x800;
        if (AT(save, 0x1F, u8)) {
            Hewie_PlaceAtPlacement((Hewie *)((u8 *)gCharPartner), (HewiePlacement *)cs);
        }
        VCALL(gCharPartner, 0x70, void (*)(void *))(gCharPartner);
        Hewie_SetHealthState((Hewie *)((u8 *)gCharPartner), AT(cs, 0xC, s32));
        if (gCharSlot2 != NULL) {
            if (AT(save, 0x20, u8)) {
                Progress_ActivateChar(prog, 2);
            }
            VCALL(gCharSlot2, 0x70, void (*)(void *))(gCharSlot2);
            stalker = (u8 *)gCharSlot2;
            if (AT(stalker, 0x28, u8) &&
                AT(stalker, 0x30, s32) != VCALL(g, 0xA4, s32 (*)(Scene *))(g)) {
                if (AT(gCharSlot2, 0xC4, s32) == 2 ||
                    (u8)VCALL(prog, 0x64, s32 (*)(Progress *))(prog) == 4) {
                    Summoner_Take((u8 *)g + 0x7A4, 0);
                }
            }
        }
        cr = gCreatures;
        for (i = 0; i < 10; i++) {
            u8 *e = (u8 *)gp + 0x878 + i * 0x24;
            u8 *mem;
            u8 *o;
            VObject *hm;

            if (!AT(e, 0x12, u8)) {
                continue;
            }
            mem = ((u8 *(*)(u8 *, s32))AT(AT(cr, 0x28, u8 *), 0x8, void *))(cr, 0x1600);
            o = Creature_new(0x1600, mem);
            if (o != NULL) {
                o = (i < 7) ? CreatureA_ctor(o) : CreatureB_ctor(o);
            }
            AT(cr, i * 4, u8 *) = o;
            AT(AT(cr, i * 4, u8 *), 0x20, s32) = (u8)i;
            if ((u8)i >= 7 && (u8)i < 10) {
                void *part = ((void *(*)(u8 *))AT(AT(cr, 0x28, u8 *), 0xC, void *))(cr);

                part = Model_new(0x890, part);
                if (part != NULL) {
                    part = Kind33Model_ctor(part);
                }
                AT(AT(cr, i * 4, u8 *), 0xF0, void *) = part;
            }
            VCALL(mem, 0xC, void (*)(void *))(mem);
            AT(mem, 0x28, u8) = 1;
            hm = (VObject *)((u8 *)g + 0x706480);
            ((void (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, f32, s64))AT(
                AT(hm, 0x28, u8 *), 0x18, void *))(
                hm, AT(e, 0x0, s32), AT(e, 0x8, s32), AT(e, 0x4, s32), (u8)i, AT(e, 0xE, u8),
                AT(e, 0xF, u8), i, -1.0f, 0);
        }
        if (Progress_TestFlag(prog, 0x16)) {
            VCALL(gp, 0x78, void (*)(Progress *, s32, s32))(gp, 5, 0);
        }
        VCALL(gPlacedThings, 0x1C, void (*)(VObject *))(gPlacedThings);
    }
    VCALL(gPlacedThings, 0x20, void (*)(VObject *))(gPlacedThings);
    if (AT(g, SG_CONTROL, u8) == 0) {
        AT(gCharPlayer, 0x30, s32) = VCALL(g, 0xA4, s32 (*)(Scene *))(g);
    } else {
        AT(gCharPartner, 0x30, s32) = VCALL(g, 0xA4, s32 (*)(Scene *))(g);
    }
    if (AT(g, 0xF6CD28, u8) & 0x80) {
        if (AT(g, SG_CONTROL, u8) == 0) {
            AT(gCharPlayer, 0x34, s32) = -1;
        } else {
            AT(gCharPartner, 0x34, s32) = -1;
        }
    }
    snd = gSound;
    VCALL(snd, 0x7C, void (*)(VObject *, s32, s32))(snd, 0, 0);
    VCALL(snd, 0x7C, void (*)(VObject *, s32, s32))(snd, 1, 0);
    EffectMgr_RoomReset((u8 *)g + 0xF6E200);
    RoomSlotBytes_Clear((u8 *)g + 0x1010);
    Map_FindRoom((u8 *)g + 0x101EBC0, AT(gCharPlayer, 0x30, s32));
    Progress_ClearFlag(prog, 0x2D);
    for (i = 0; i < 6; i++) {
        u8 *c = (u8 *)gCharacters[i];

        if (c != NULL && AT(c, 0x28, u8)) {
            Record20_Clear(c + 0x14E8);
            Record20_Clear(c + 0x1508);
            VCALL(prog, 0x34, void (*)(Progress *, u8))(prog, i);
        }
    }
    Events_RunPhase((u8 *)g + 0xF6AFB0, 0);
    AT(g, SG_ROOMFLAG, u8) = 0;
    room = VCALL(g, 0xA4, s32 (*)(Scene *))(g);
    for (t = D_0044C6E0; *t != -1; t++) {
        if (*t == room) {
            AT(g, SG_ROOMFLAG, u8) = 1;
            return;
        }
    }
}

/* the current room (the tag of the active room slot) */
/* 0x0039D2E0 */
s32 SceneGame_CurrentRoom(Scene *g) {
    return AT(g, 0x73F240 + AT(g, 0xF6C1B0, s32) * 4, s32);
}

/* (Progress +0x68) set or clear bit (a + 1 + b) of the bits at +0x73EEEC */
/* 0x0039BAE0 */
void SceneGame_SetRoomBit(Scene *g, s32 set, u8 a, s32 b) {
    u32 bit = (u32)(a + 1) + b;
    u32 *w = &AT(g, 0x73EEEC + (bit >> 5) * 4, u32);

    if (set) {
        *w |= 1 << (bit & 0x1F);
    } else {
        *w &= ~(1 << (bit & 0x1F));
    }
}

extern void SceneGame_RoomCreatures(Scene *g);

/* (+0xDC) the start of play in a room: the room's 8 trigger areas, which of the characters in
 * the room each holds (+0x1010 + k * 6: +1 by position, +2 by the character test) */
/* 0x0039CDC0 */
void SceneGame_PlayStart(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    u8 *dir = (u8 *)g + 0xF6CBB0;
    VObject *rooms, *ev;
    s32 room;
    u32 k;
    s32 i;

    Progress_WhoIsWhere(prog);
    if (!((s32 (*)(u8 *))AT(AT(dir, 0x64, u8 *), 0x6C, void *))(dir)) {
        if (!(u8)Progress_TestFlag(prog, 0xF)) {
            Panic_Update((u8 *)g + 0x7F8);
        }
        StatusTimers_Frame((u8 *)g + 0xA20);
    }
    room = VCALL(g, 0xA4, s32 (*)(Scene *))(g);
    rooms = gRooms;
    ev = gEvents;
    for (k = 0; k < 8; k++) {
        u8 *z = (u8 *)g + 0x1010 + k * 6;
        u32 area;

        z[2] = 0;
        z[1] = 0;
        area = VCALL(rooms, 0x48, u32 (*)(VObject *, s32, s32))(rooms, room, (u8)k) & 0xFFFF;
        for (i = 0; i < 6; i++) {
            u8 *c = (u8 *)gCharacters[i];

            if (c == NULL || room != AT(c, 0x30, s32) || area == 0xFFFF || AT(c, 0x28, u8) != 1 ||
                AT(c, 0x29, u8)) {
                continue;
            }
            if (VCALL(ev, 0x10, s32 (*)(VObject *, f32 *, u32, s32))(ev, (f32 *)(c + 0x10), area, AT(c, 0x34, s32))) {
                z[1] |= (u8)(1 << i);
            }
            if (VCALL(ev, 0x14, s32 (*)(VObject *, void *, u32))(ev, gCharacters[i], area)) {
                z[2] |= (u8)(1 << i);
            }
        }
    }
    Summoner_RoomStart((u8 *)g + 0x7A4);
    if (Progress_TestFlag(prog, 0x16)) {
        SceneGame_RoomCreatures(g);
    }
}

extern const PTMF SceneGame_SubPaused_ptmf;
extern void *gMovieFlag;

/* the gameplay state (each frame): the sub-state +0x1053440 */
/* 0x003A0060 */
void SceneGame_StatePlay(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    PTMF *sub = &AT(g, 0x1053440, PTMF);

    if (__ptmf_cmpr(sub, &SceneGame_SubPaused_ptmf) && AT(g, 0x106503C, void *) != NULL) {
        MusicDir_Update(AT(g, 0x106503C, void *));
    }
    BgmCtl_Update((BgmCtl *)((u8 *)g + 0x1053424));
    if (!(u8)Progress_TestFlag(prog, 8) && !(u8)Progress_TestFlag(prog, 0x2A)) {
        func_0021C840(gMovieFlag, 0x32, 0);
    }
    VCALL((VObject *)gLights, 0x34, void (*)(VObject *))((VObject *)gLights);
    if (ptmf_test(sub)) {
        ptmf_scall(g, sub);
    }
}

extern const PTMF D_0044C598;   /* the scenes' callback while saving the game */
extern const PTMF SceneGame_SubTransition_ptmf, SceneGame_SubMoviePaused_ptmf, SceneGame_SubPaused_ptmf2, SceneGame_SubSubScreen_ptmf;   /* sub-states: menus */
extern s8 gInput;           /* the pad: 0 = not connected */
extern VObject *gStageMusic;
extern void SceneGame_ActionPrompt(Scene *g);
extern void SceneGame_Danger(Scene *g);

#define SG_EVENT 0xF6AFB0
#define SG_CAMDIR 0xF6CBB0
#define SG_FRAMEFLAGS 0xF6CD24   /* bit 0: room changed, bits 0..3: busy, bits 4..9: event lock */

/* the camera director's +0x6C (its vtable sits at +0x64): an event camera is running */
static s32 camdir_busy(Scene *g) {
    u8 *d = (u8 *)g + SG_CAMDIR;

    return ((s32 (*)(u8 *))AT(AT(d, 0x64, u8 *), 0x6C, void *))(d);
}

static void clamp01(f32 *v) {
    if (*v < 0.0f) {
        *v = 0.0f;
    }
    if (!(*v <= 1.0f)) {
        *v = 1.0f;
    }
}

/* every scene gets the save callback (+0x4) and is told (+0x14) */
static inline void scenes_to_save(void) {
    u8 *scenes = gSceneTable;
    s32 i;

    for (i = 0; i < 4; i++) {
        u8 *sc = AT(scenes, 4 + i * 4, u8 *);

        if (sc != NULL) {
            ptmf_set(&AT(sc, 4, PTMF), &D_0044C598);
            sc = AT(scenes, 4 + i * 4, u8 *);
            VCALL(sc, 0x14, void (*)(void *))(sc);
        }
    }
}

extern const PTMF SceneGame_SubPlay_ptmf;   /* the gameplay sub-state "in play" (SceneGame_SubPlay) */

/* the gameplay sub-state is "in play" */
/* 0x00399BF0 */
s32 SceneGame_InPlay(Scene *g) {
    return __ptmf_cmpr(&AT(g, 0x1053440, PTMF), &SceneGame_SubPlay_ptmf) == 0;
}

/* 0x00399C30 */
void SceneGame_SetCreatureCount(Scene *g, u8 a, u8 b) {
    AT(g, 0x105344D, u8) = a;
    AT(g, 0x105344E, u8) = b;
}

/* +0xF6CD24 bit 3 */
/* 0x00399C50 */
void SceneGame_SetFlag3(Scene *g, u32 on) {
    AT(g, 0xF6CD24, u32) = (AT(g, 0xF6CD24, u32) & ~8u) | (on & 1) << 3;
}

/* +0xF6CD24 bits 4..9 */
/* 0x00399C80 */
u32 SceneGame_Bits4to9(Scene *g) {
    return (AT(g, 0xF6CD24, u32) >> 4) & 0x3F;
}

/* the gameplay sub-state, each frame. +0x44: 0 play, 1 leaving the room (event phase 4), 2
 * waiting for the next room: once loaded the characters leave and enter, the camera restarts
 * (event phases 5, 3). In play: event phases 1 .. 3, the camera, the characters' control;
 * then the world update (rooms, effects, characters), the fade (+0x1158 += +0x1160), drawing,
 * and the menus (+0xC sub screen, +6 pause, +4 map) */
/* 0x0039EAB0 */
void SceneGame_SubPlay(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    VObject *ev = (VObject *)((u8 *)g + SG_EVENT);
    u8 *cam = (u8 *)g + SG_CAMDIR;
    u32 *flags = &AT(g, SG_FRAMEFLAGS, u32);
    void *rooms = (u8 *)g + 0x73EE80;
    VObject *loader;
    s32 menu;
    s32 i;

#ifdef HG_NATIVE
    if (hg_debug_freeplay()) {   /* native/platform/debug.c: HG_FREEPLAY */
        Progress_ClearFlag(prog, 8);
    }
    {
        extern void hg_debug_flaglog(s32 flag, s32 on);

        hg_debug_flaglog(8, (u8)Progress_TestFlag(prog, 8));
        hg_debug_flaglog(63, VCALL(prog, 0xC, s32 (*)(Progress *))(prog));   /* (63: the current room) */
    }
#endif
    if (AT(g, 0x44, s32) == 0) {
        RoomMgr_SlotLoading(rooms, AT(g, 0xF6C1B0, s32) == 0);
        if ((gPadPressed >> 3) & 1 || gInput == 0) {
            Progress_SetFlag(prog, 6);
        }
        *flags &= ~1;
        Progress_PursuerRequest(prog);
        CamDirector_Ease(cam);
        VCALL(g, 0xDC, void (*)(Scene *))(g);
        Events_RunPhase((u8 *)ev, 1);
        Progress_RelationChanges(prog);
        Progress_ResolveRelations(prog);
        Progress_OwnRequests(prog);
        Progress_CharRequests(prog);
        Events_RunPhase((u8 *)ev, 2);
        if (!(u8)Progress_TestFlag(prog, 0x12)) {
            if (AT(g, SG_CONTROL, u8) == 0) {
                Progress_PlayerButtons(prog);
            } else {
                Progress_CommandButtons(prog);
            }
        }
        if (!((*flags >> 3) & 1)) {
            Events_RunCharScripts(ev);
        }
        AT(g, 0x1170, u8) = Progress_CameraFollow(prog, AT(g, 0x1170, u8));
        if (AT(g, 0x1170, u8) != 0xFF) {
            u8 *c = (u8 *)gCharacters[AT(g, 0x1170, u8)];

            ((void (*)(u8 *, s32, s32))AT(AT(cam, 0x64, u8 *), 0x64, void *))(
                cam, AT(c, 0xE8, s32), AT(c, 0xEC, s32));
        }
        CamDirector_Track(cam);
        Events_RunPhase((u8 *)ev, 3);
        if (camdir_busy(g) && !(u8)VCALL(ev, 0xC0, s32 (*)(VObject *))(ev)) {
            *flags = (*flags & ~0x3F0) | 0x3F0;
        } else {
            *flags &= ~0x3F0;
        }
        if ((gPadPressed & 1) && !(u8)Progress_TestFlag(prog, 5) && AT(gCharPlayer, 0xE0, u8) == 0 &&
            AT(gCharPlayer, 0x14E8, s32) != 5 && AT(g, 0x7F8, u8) < 4 &&
            AT(gCharPlayer, 0xF8, s32) == 0 && !Progress_TestFlag(prog, 8) &&
            !VCALL(ev, 0xBC, s32 (*)(VObject *))(ev)) {
            AT(g, 0xF87244, u8) = 0;
            Progress_SetFlag(prog, 4);
        }
    } else {
        if (AT(g, 0x44, s32) == 1) {
            Events_RunPhase((u8 *)ev, 4);
            AT(g, 0x44, s32) = 2;
        }
        if (RoomMgr_SlotLoading(rooms, AT(g, 0xF6C1B0, s32) == 0) ||
            (u8)VCALL(gFileLoader, 0x38, s32 (*)(VObject *))(gFileLoader) ||
            VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) == 2) {
            Events_RunPhase((u8 *)ev, 3);
            *flags |= 1;
        } else {
            AT(g, 0x44, s32) = 0;
            Events_RunPhase((u8 *)ev, 5);
            for (i = 0; i < 6; i++) {
                if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) != 0) {
                    VCALL(gCharacters[i], 0x34, void (*)(void *, s32))(gCharacters[i],
                                                                      AT(g, 0xF6CD20, u8));
                    AT(gCharacters[i], 0xEC, s32) = -1;
                    AT(gCharacters[i], 0xE8, s32) = -1;
                }
            }
            Creatures_FionaLeft((u8 *)g + 0x706480, AT(g, 0xF6CD20, u8));
            AT(g, 0xF6CD20, u8) = 0xFF;
            VCALL(gDoors, 0x5C, void (*)(void *))(gDoors);
            ((void (*)(void *, s32))RoomMgr_Leave)(rooms, AT(g, 0xF6C1B0, s32));
            AT(g, 0xF6C1B0, s32) ^= 1;
            SceneGame_EnterRoom(g);
            for (i = 0; i < 6; i++) {
                if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) != 0 &&
                    AT(gCharacters[i], 0xE2, u8) == 0) {
                    VCALL(gCharacters[i], 0x38, void (*)(void *))(gCharacters[i]);
                }
            }
            Creatures_EnterRoom((u8 *)g + 0x706480);
            VCALL(g, 0xDC, void (*)(Scene *))(g);
            Progress_CharRequests(prog);
            CamDirector_RoomStart(cam, AT(g, 0x74881C, s32));
            Progress_CameraOn(prog, AT(g, 0x1170, u8));
            for (i = 0; i < 6; i++) {
                if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) != 0) {
                    VCALL(gCharacters[i], 0x40, void (*)(void *))(gCharacters[i]);
                }
            }
            Creatures_Call40((u8 *)g + 0x706480);
            Events_RunPhase((u8 *)ev, 3);
            CamDirector_Update(cam);
            *flags |= 1;
        }
    }

    /* saving: the scenes get their save callback, the loader is told, and the game waits */
    if (Progress_TestFlag(prog, 8) && Progress_TestFlag(prog, 0x1C)) {
        loader = gFileLoader;
        if (VCALL(loader, 0x24, s32 (*)(VObject *))(loader) != 2) {
            scenes_to_save();
            VCALL(loader, 0x1C, void (*)(VObject *))(loader);
            AT(gGamePtr, 0x4, s32) = 2;
            AT(gGamePtr, 0x10, s32) = 1;
        }
        return;
    }

    AvoidPrompt_Tick((u8 *)g + 0x1053480);
    if ((*flags & 0xF) == 0) {
        CamDirector_Update(cam);
    }
    if ((*flags & 0xF) == 0) {
        if (((*flags >> 4) & 0x3F) == 0) {
            Progress_CharsThink(prog);
            Progress_CharsFrame(prog);
        }
        if (!Progress_TestFlag(prog, 0x17) && !camdir_busy(g)) {
            Creatures_Update((u8 *)g + 0x706480);
        }
        RoomMgr_Update(rooms);
        if (!Progress_TestFlag(prog, 0x17)) {
            Items_Update((u8 *)g + 0xF87248);
        }
        PlayTime_Tick((u8 *)g + 0x1004);
        PlacedThings_Update((u8 *)g + 0x6FC380);
        RoomEffects_Update((u8 *)g + 0xF6CD30);
        EffectMgr_Update((u8 *)g + 0xF6E200);
        for (i = 0; i < 6; i++) {
            if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) == 1) {
                Character_Hearing(gCharacters[i]);
            }
        }
    }

    /* the screen fade */
    AT(g, 0x1158, f32) += AT(g, 0x1160, f32);
    if (!(AT(g, 0x1158, f32) < 1.0f)) {
        AT(g, 0x1158, f32) = 1.0f;
        AT(g, 0x1160, f32) = 0.0f;
    }
    if (AT(g, 0x1158, f32) <= 0.0f) {
        AT(g, 0x1158, f32) = 0.0f;
        AT(g, 0x1160, f32) = 0.0f;
    }
    if (AT(g, 0x1158, f32) != AT(g, 0x115C, f32)) {
        AT(g, 0x115C, f32) = AT(g, 0x1158, f32);
        AT(gAdx, 0x118, f32) = AT(g, 0x1158, f32);
        clamp01(&AT(gAdx, 0x118, f32));
        Bgm_ApplyVolume(gAdx);
    }

    if (!(u8)Progress_TestFlag(prog, 8)) {
        RoomMgr_Draw(rooms, AT(g, 0xF6C1B0, s32));
        if (!Progress_TestFlag(prog, 0x17) || Progress_TestFlag(prog, 0x24)) {
            Progress_DrawChars(prog);
        }
        if ((!Progress_TestFlag(prog, 0x17) || Progress_TestFlag(prog, 0x24)) && !camdir_busy(g)) {
            Creatures_Draw((u8 *)g + 0x706480);
        }
        PlacedThings_Draw((u8 *)g + 0x6FC380);
        SceneGame_ActionPrompt(g);
        RoomEffects_Draw((u8 *)g + 0xF6CD30);
        EffectMgr_Draw((u8 *)g + 0xF6E200);
        if (!(u8)Progress_TestFlag(prog, 0xF) && !camdir_busy(g)) {
            if (!(u8)VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents)) {
                ScreenFade_Level((u8 *)g + 0x7F8, 1.0f);
            }
            ScreenFade_Frame((u8 *)g + 0x7F8, 0);
        }
        if (AT(g, 0xF6C1A3, u8) != 0) {
            VCALL(ev, 0xB8, void (*)(VObject *, s32))(ev, AT(g, 0xF6C1A1, u8) == 6 ? 0x30 : 0x31);
        }
        Task_Draw((Task *)((u8 *)g + 0xF6B6B8));
        VCALL((u8 *)g + 0xF87240, 0x24, void (*)(void *))((u8 *)g + 0xF87240);
        AvoidPrompt_Update((u8 *)g + 0x1053480);
    } else if (Progress_TestFlag(prog, 0x28)) {
        rng_tick(g);
    }

    /* the menus */
    menu = 0;
    if (Progress_TestFlag(prog, 0xC)) {
        VObject *snd;

        VCALL(gRumble, 0x10, void (*)(VObject *))(gRumble);
        VCALL(gFileLoader, 0x1C, void (*)(VObject *))(gFileLoader);
        ptmf_set(&AT(g, 0x1053440, PTMF), &SceneGame_SubTransition_ptmf);
        menu = 1;
        snd = gSound;
        VCALL(snd, 0xAC, void (*)(VObject *, f32))(snd, 1.0f);
        AT(gAdx, 0x120, f32) = 1.0f;
        clamp01(&AT(gAdx, 0x120, f32));
        Bgm_ApplyVolume(gAdx);
        if (gStageMusic != NULL) {
            VCALL(gStageMusic, 0x24, void (*)(VObject *, f32))(gStageMusic, 1.0f);
        }
        if (gMovie != NULL) {
            AT(gMovie, 0x1D4, f32) = 1.0f;
            clamp01(&AT(gMovie, 0x1D4, f32));
            Movie_ApplyVolume(gMovie);
        }
        VCALL(snd, 0x9C, void (*)(VObject *))(snd);
        VCALL(gMusic, 0x8, void (*)(void *, f32, s32, s32, s32))(gMusic, 1.0f, 0xFF, 0, 0);
    }
    if (!(u8)Progress_TestFlag(prog, 8) && Progress_TestFlag(prog, 6) && !menu) {
        if (camdir_busy(g) && !(u8)Progress_TestFlag(prog, 0x19)) {
            if ((*flags & 0xF) == 0 && AT(g, 0x44, s32) == 0 &&
                !VCALL(ev, 0xBC, s32 (*)(VObject *))(ev) && gMovie != NULL) {
                VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 1);
                Pause_Open((u8 *)g + 0x73EBA0, (gInput == 0 ? 0x80 : 0) | 1);
                Panic_Pause((u8 *)g + 0x7F8);
                menu = 1;
                ptmf_set(&AT(g, 0x1053440, PTMF), &SceneGame_SubMoviePaused_ptmf);
            }
        } else if (!VCALL(ev, 0xBC, s32 (*)(VObject *))(ev) && gMovie == NULL &&
                   !(u8)Progress_TestFlag(prog, 0x19)) {
            VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 1);
            Pause_Open((u8 *)g + 0x73EBA0, gInput == 0 ? 0x80 : 0);
            Panic_Pause((u8 *)g + 0x7F8);
            menu = 1;
            ptmf_set(&AT(g, 0x1053440, PTMF), &SceneGame_SubPaused_ptmf2);
        }
    }
    if (Progress_TestFlag(prog, 4) && !menu) {
        VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 1);
        ptmf_set(&AT(g, 0x1053440, PTMF), &SceneGame_SubSubScreen_ptmf);
    }
    Progress_ClearFlag(prog, 0xC);
    Progress_ClearFlag(prog, 6);
    Progress_ClearFlag(prog, 4);
    if (!camdir_busy(g)) {
        SceneGame_Danger(g);
    }
}

/* Progress +0x54: an event camera is running and the event does not hide the room's alpha
 * parts (event +0xC0) */
/* 0x0039A7C0 */
s32 SceneGame_EventCamRunning(Scene *g) {
    VObject *ev = (VObject *)((u8 *)g + SG_EVENT);

    if (!camdir_busy(g)) {
        return 0;
    }
    return (u8)VCALL(ev, 0xC0, s32 (*)(VObject *))(ev) == 0;
}

/* show (event +0x3C) or hide (+0x40) the action markers 0x800A..0x800D */
static void prompt_show(s32 id) {
    VCALL(gEvents, 0x3C, void (*)(VObject *, s32))(gEvents, id);
}

static void prompt_hide_all(void) {
    VObject *ev = gEvents;

    VCALL(ev, 0x40, void (*)(VObject *, s32))(ev, 0x800A);
    VCALL(ev, 0x40, void (*)(VObject *, s32))(ev, 0x800B);
    VCALL(ev, 0x40, void (*)(VObject *, s32))(ev, 0x800C);
    VCALL(ev, 0x40, void (*)(VObject *, s32))(ev, 0x800D);
}

/* the action prompt, each frame: for the action Fiona could take now (+0x16B4; last shown
 * +0x16D4) the marker of its kind (+0x16D1: 1..3 0x800B, 4 0x800C, 5 / 6 0x800A or, with Hewie
 * controlled, 0x800D, others 0x800A; 7 / 0xFF none) is shown when the action changed (5 / 6
 * also while the chase music state +0x50 / +0x52 changes to or from 2); no action, or Fiona
 * busy: all hidden */
/* 0x0039C880 */
void SceneGame_ActionPrompt(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    u8 *cur = (u8 *)g + 0x16B4;
    u8 *last = (u8 *)g + 0x16D4;
    s32 show = 0, changed, c1, c2;

    if (gCharPlayer == NULL || AT(gCharPlayer, 0x28, u8) == 0) {
        return;
    }
    if (Progress_TestFlag(prog, 0x12)) {
        Record20_Clear(cur);
        Record20_Clear(last);
    }
    if (AT(gCharPlayer, 0xE0, u8) != 0) {
        prompt_hide_all();
        return;
    }
    if (AT(gCharPlayer, 0xF8, s32) == 0) {
        switch (AT(cur, 0x0, s32)) {
        case (s32)0x80000002:
        case (s32)0x80000001:
        case (s32)0x80000000:
            show = 1;
            break;
        case (s32)0x80000003:
        case 5:
            if (AT(gCharPlayer, 0xE0, u8) == 0) {
                show = 1;
            }
            break;
        case 0xD:
        case 3:
        case 2:
            show = 1;
            break;
        case 0:
            Record20_Clear(last);
            break;
        }
    }
    if (!((show & ~(Progress_TestFlag(prog, 0x12) & 0xFF)) != 0 &&
          AT(gCharPlayer, 0x1AD580, s32) != 0xD)) {
        prompt_hide_all();
        return;
    }
    c1 = !(AT(last, 0x0, s32) == AT(cur, 0x0, s32) && AT(last, 0x8, s32) == AT(cur, 0x8, s32));
    c2 = c1 || AT(last, 0x1D, u8) != AT(cur, 0x1D, u8);
    changed = c2 || AT(last, 0x4, s32) != AT(cur, 0x4, s32);
    if (AT(cur, 0x0, s32) == AT(last, 0x0, s32)) {
        if (AT(cur, 0x0, s32) == 3) {
            if (AT(cur, 0x18, s32) == AT(last, 0x18, s32) && AT(cur, 0x8, s32) == AT(last, 0x8, s32)) {
                changed = 0;
            }
        } else if (AT(cur, 0x0, s32) == 2) {
            VObject *rooms = gRooms;
            u16 a = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, AT(cur, 0x18, s32), AT(cur, 0x8, u8));
            u16 b = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, AT(last, 0x18, s32), AT(last, 0x8, u8));

            if (a == b) {
                changed = 0;
            }
        }
    }
    switch (AT(cur, 0x1D, u8)) {
    case 7:
    case 0xFF:
        break;
    case 4:
        if (changed) {
            prompt_show(0x800C);
        }
        break;
    case 3:
    case 2:
    case 1:
        if (changed) {
            prompt_show(0x800B);
        }
        break;
    case 6:
    case 5: {
        s32 blink = 0;

        if (AT(g, 0x52, u8) != AT(g, 0x50, u8) && (AT(g, 0x50, u8) == 2 || AT(g, 0x52, u8) == 2)) {
            blink = 1;
        }
        if (changed || blink) {
            prompt_show((u8)Progress_GameMode(prog) == 2 ? 0x800D : 0x800A);
        }
        break;
    }
    default:
        if (changed) {
            prompt_show(0x800A);
        }
        break;
    }
    AT(last, 0x0, s32) = AT(cur, 0x0, s32);
    AT(last, 0x4, s32) = AT(cur, 0x4, s32);
    AT(last, 0x8, s32) = AT(cur, 0x8, s32);
    AT(last, 0xC, s32) = AT(cur, 0xC, s32);
    AT(last, 0x10, s32) = AT(cur, 0x10, s32);
    AT(last, 0x14, f32) = AT(cur, 0x14, f32);
    AT(last, 0x18, s32) = AT(cur, 0x18, s32);
    AT(last, 0x1C, u8) = AT(cur, 0x1C, u8);
    AT(last, 0x1D, u8) = AT(cur, 0x1D, u8);
    AT(last, 0x1E, u16) = AT(cur, 0x1E, u16);
}

#define CHASE_STATE(g) AT(g, 0x50, u8)   /* 0 calm, 1 tense, 2 chased */
#define CHASE_PREV(g)  AT(g, 0x51, u8)   /* the state before the last change */
#define CHASE_LAST(g)  AT(g, 0x52, u8)   /* the state last frame */
#define CHASE_TIMER(g) AT(g, 0x58, u32)  /* frames before the state may change again */

/* the danger state (the chase music), each frame: Progress flags force it (0x1B calm, 0x1F
 * chased, 7 tense); the stalker in Fiona's room makes it chased; otherwise it moves between
 * calm, tense and chased by the conditions (bit 2 stalker present, 6 hunted, flag 0x22), the
 * stalker's alert (+0x64: 3 / 4) and whether a creature (+0x3C) is in her room, each change
 * holding for a while (+0x58). Also keeps conditions 2 / 3 for an active / chasing stalker
 * and sets 6 when calm while condition 1 */
/* 0x0039BB60 */
void SceneGame_Danger(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    u8 alert = (u8)VCALL(prog, 0x64, s32 (*)(Progress *))(prog);
    s32 stalker = 0, chasing = 0, creature = 0, near = 0;
    s32 room, sroom = -1, i;

    if (gCharSlot2 != NULL && AT(gCharSlot2, 0x28, u8) != 0) {
        stalker = 1;
        chasing = AT(gCharSlot2, 0xC4, s32) == 2;
    }
    if (alert == 0xFF && !stalker) {
        Progress_SetCondBit(prog, 2);
    }
    if (chasing) {
        Progress_SetCondBit(prog, 3);
    }
    CHASE_LAST(g) = CHASE_STATE(g);
    if (Progress_TestFlag(prog, 0x1B)) {
        CHASE_STATE(g) = 0;
        CHASE_TIMER(g) = 0;
    } else if (Progress_TestFlag(prog, 0x1F)) {
        CHASE_STATE(g) = 2;
        CHASE_TIMER(g) = 0;
    } else if (Progress_TestFlag(prog, 7)) {
        CHASE_STATE(g) = 1;
        CHASE_TIMER(g) = 0;
    } else {
        room = AT(gCharPlayer, 0x30, s32);
        if (stalker) {
            sroom = AT(gCharPursuer, 0x30, s32);
            if (sroom != room) {
                VObject *rooms = gRooms;

                /* (as the original: any exit not leading to the stalker's room counts) */
                for (i = 0; i < 8; i++) {
                    if (VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, room, i) != sroom) {
                        near = 1;
                        break;
                    }
                }
            }
        }
        for (i = 0; i < 10; i++) {
            VObject *c = AT(gCreatures, i * 4, VObject *);

            if (c != NULL && (u8)VCALL(c, 0x3C, s32 (*)(VObject *, u32))(c, (u8)i) == 1 &&
                AT(c, 0x30, s32) == room) {
                creature = 1;
                break;
            }
        }
        if (stalker && room == sroom) {
            CHASE_STATE(g) = 2;
            CHASE_TIMER(g) = 0x1E;
        } else {
            switch (CHASE_STATE(g)) {
            case 0:
                if (Progress_TestFlag(prog, 0x22)) {
                    CHASE_STATE(g) = 1;
                    CHASE_TIMER(g) = 0x1C2;
                } else if (CHASE_TIMER(g) == 0) {
                    if (!Progress_CondBit(prog, 2) || creature == 1 || Progress_CondBit(prog, 6)) {
                        CHASE_STATE(g) = 1;
                        CHASE_TIMER(g) = 0x1C2;
                    }
                }
                break;
            case 1:
                if (Progress_CondBit(prog, 6) || Progress_TestFlag(prog, 0x22)) {
                    CHASE_TIMER(g) += 0x96;
                    if (CHASE_TIMER(g) > 0x1C2) {
                        CHASE_TIMER(g) = 0x1C2;
                    }
                }
                if (Progress_CondBit(prog, 2) && !creature) {
                    if (CHASE_TIMER(g) == 0) {
                        CHASE_STATE(g) = 0;
                        CHASE_TIMER(g) = 0x1E;
                    }
                } else if (CHASE_PREV(g) == 0) {
                    CHASE_TIMER(g) = 0x1C2;
                } else if (CHASE_PREV(g) == 2) {
                    CHASE_TIMER(g) = 0x96;
                }
                break;
            case 2:
                if (Progress_CondBit(prog, 2)) {
                    CHASE_STATE(g) = Progress_TestFlag(prog, 0x22) || creature ? 1 : 0;
                    CHASE_TIMER(g) = 0x1E;
                } else if ((u32)(alert - 3) < 2 && near) {
                    if (CHASE_TIMER(g) == 0) {
                        CHASE_STATE(g) = 1;
                        CHASE_TIMER(g) = 0x1E;
                    }
                } else {
                    CHASE_TIMER(g) = 0x96;
                }
                break;
            }
            if (CHASE_TIMER(g) != 0) {
                CHASE_TIMER(g)--;
            }
        }
    }
    if (CHASE_LAST(g) != CHASE_STATE(g)) {
        CHASE_PREV(g) = CHASE_LAST(g);
    }
    i = (u8)Progress_CondBit(prog, 1);
    AT(g, 0x54, s32) = 0;
    if (i && CHASE_STATE(g) == 0) {
        Progress_SetCondBit(prog, 6);
    }
}

/* set the state (a, b) of every door with a side in room `room` (the rooms' doors, +0x6C side
 * 0 / 1, up to 400, ending at -1) */
/* 0x0039C5C0 */
void SceneGame_SetRoomDoors(Scene *g, s32 room, s32 a, s32 b) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    VObject *rooms = gRooms;
    u32 d;
    s32 r;

    for (d = 0; d < 0x190; d++) {
        r = VCALL(rooms, 0x6C, s32 (*)(VObject *, u32, s32))(rooms, d & 0xFFFF, 0);
        if (r == -1) {
            return;
        }
        if (r == room || VCALL(rooms, 0x6C, s32 (*)(VObject *, u32, s32))(rooms, d & 0xFFFF, 1) == room) {
            Progress_LockDoorFor(prog, d & 0xFFFF, a, b);
        }
    }
}

/* get the room behind exit `exit` of the current one ready: loaded into the other room slot
 * (+0x73F240, current slot +0xF6C1B0) unless it is there already */
/* 0x0039D070 */
void SceneGame_PrepareExit(Scene *g, s32 exit) {
    s32 room = VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, s32))(
        gRooms, AT(g, 0x73F240 + AT(g, 0xF6C1B0, s32) * 4, s32), exit);
    u8 other;

    if (room & 0x80000000) {
        return;
    }
    other = AT(g, 0xF6C1B0, s32) == 0;
    if (room != AT(g, 0x73F240 + other * 4, s32)) {
        RoomMgr_LoadRoom((u8 *)g + 0x73EE80, room, other);
    }
}

/* leave the current room through exit `exit`: unless a room change keeps the sound going
 * (progress flag 0x27) the sounds stop; when the other room slot doesn't already hold the room
 * behind the exit (SceneGame_PrepareExit) it is dropped and the room is loaded (+0xAC); then the change
 * starts (+0x44 = 1, the exit kept in +0xF6CD20 for the arrival). Returns the exit on the
 * other side (rooms +0x14): the event script's "exit taken" (event +0x702), which the next
 * room's entry script uses to place her */
/* 0x0039D120 */
u8 SceneGame_LeaveRoom(Scene *g, u8 exit) {
    VObject *rooms = gRooms;
    u8 other = AT(g, 0xF6C1B0, s32) == 0;
    s32 next = AT(g, 0x73F240 + other * 4, s32);
    s32 room = VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(
        rooms, AT(g, 0x73F240 + AT(g, 0xF6C1B0, s32) * 4, s32), exit);

    if (!(u8)Progress_TestFlag((Progress *)((u8 *)g + SG_PROGRESS), 0x27)) {
        VObject *snd = gSound;

        VCALL(snd, 0x10, void (*)(VObject *, s32, s32))(snd, 0, 0x1B0C00);
        VCALL(snd, 0x84, void (*)(VObject *, s32))(snd, 6);
        VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 6);
    }
    if (next != room) {
        if (next != -1) {
            RoomMgr_LoadSlot((u8 *)g + 0x73EE80, AT(g, 0xF6C1B0, s32) == 0);
        }
        VCALL(g, 0xAC, void (*)(Scene *, u32))(g, exit);
    }
    u8 in;

    AT(g, 0x44, s32) = 1;
    in = VCALL(rooms, 0x14, s32 (*)(VObject *, s32, u32))(rooms, AT(g, 0x73F240 + AT(g, 0xF6C1B0, s32) * 4, s32), exit);
    AT(g, 0xF6CD20, u8) = exit;
    return in;
}

/* ---- doors, nearby rooms, the creatures, the save snapshot ---- */

#include "sce/libvu0.h"

extern void *Actor_vtable[], *Character_vtable[], *CreatureB_vtable[], *CreatureA_vtable[];

#define SG_ROOM(g) VCALL((VObject *)(g), 0xA4, s32 (*)(Scene *))(g)

/* every door with a side in room `room` unlocked / locked */
static void doors_of_room(Scene *g, s32 room, s32 (*set)(Progress *, u32)) {
    VObject *rooms = gRooms;
    u32 d;

    for (d = 0; d < 0x190; d++) {
        s32 a = VCALL(rooms, 0x6C, s32 (*)(VObject *, u32, s32))(rooms, d & 0xFFFF, 0);

        if (a == -1) {
            return;
        }
        if (a == room || VCALL(rooms, 0x6C, s32 (*)(VObject *, u32, s32))(rooms, d & 0xFFFF, 1) == room) {
            set(&AT(g, SG_PROGRESS, Progress), d & 0xFFFF);
        }
    }
}

/* 0x0039C6C0 */
void SceneGame_UnlockRoomDoors(Scene *g, s32 room) {
    doors_of_room(g, room, Progress_UnlockDoor);
}

/* 0x0039C7A0 */
void SceneGame_LockRoomDoors(Scene *g, s32 room) {
    doors_of_room(g, room, Progress_LockDoor);
}

/* room `room`'s exits all closed to `kind`'s side (lock bits 4..7 of the door states: kind 0
   bit 0, 1 bit 1, 2..5 bit 2, others none) */
/* 0x0039C4C0 */
s32 SceneGame_RoomClosedTo(Scene *g, s32 room, u32 kind) {
    VObject *rooms = gRooms;
    u32 side = 0, exit;

    switch (kind & 0xFF) {
    case 0:
        side = 1;
        break;
    case 1:
        side = 2;
        break;
    case 2: case 3: case 4: case 5:
        side = 4;
        break;
    }
    for (exit = 0; exit < 8; exit++) {
        u32 d = VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, room, exit) & 0xFFFF;

        if (d < 0x190 && !((AT(g, SG_PROGRESS + 0x124 + d * 4, u32) >> 4) & 0xF & side)) {
            return 0;
        }
    }
    return 1;
}

/* the way through `room`'s exit `exit` is open: no flag, passable from all three sides */
static s32 exit_open(Scene *g, s32 room, u32 exit) {
    Progress *p = &AT(g, SG_PROGRESS, Progress);

    return (u8)Progress_ExitUnlocked(p, room, exit) != 1 && (u8)Progress_ExitPassable(p, room, exit, 0) &&
           (u8)Progress_ExitPassable(p, room, exit, 1) && (u8)Progress_ExitPassable(p, room, exit, 2);
}

static s32 in_list(const s32 *list, u32 n, s32 room) {
    u32 i;

    for (i = 0; i < n; i++) {
        if (list[i] == room) {
            return 1;
        }
    }
    return 0;
}

/* +0xF4: the rooms within two open doors of `room` (-1: the current one): `room` first, then
   each new one; the rest of the 65 entries -1; how many */
/* 0x0039C040 */
u32 SceneGame_RoomsNear(Scene *g, s32 *list, s32 room) {
    VObject *rooms = gRooms;
    u32 n = 1, i, j;

    if (list == NULL) {
        return 0;
    }
    if (room == -1) {
        room = SG_ROOM(g);
    }
    list[0] = room;
    for (i = 0; i < 8; i++) {
        s32 r = VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, room, i & 0xFF);

        if (r == -1 || !exit_open(g, room, i & 0xFF) || in_list(list, n, r)) {
            continue;
        }
        list[n++] = r;
        for (j = 0; j < 8; j++) {
            s32 r2 = VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, r, j & 0xFF);

            if (r2 != -1 && exit_open(g, r, j & 0xFF) && !in_list(list, n, r2)) {
                list[n++] = r2;
            }
        }
    }
    for (i = n; i < 0x41; i++) {
        list[i] = -1;
    }
    return n;
}

/* a random one of n things: (u8)(n x the random 0..1, +0x1C) */
static u32 pick(u32 n) {
    f32 r = VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);

    return (u8)(u32)((f32)n * r);
}

/* a random room near `room` (SceneGame_RoomsNear, 2 or more), else -1 */
/* 0x0039C380 */
s32 SceneGame_RandomRoomNear(Scene *g, s32 room) {
    s32 *list = func_00114FA8(0x104);
    u32 n;
    s32 r;

    if (list == NULL) {
        return -1;
    }
    n = VCALL((VObject *)g, 0xF4, u32 (*)(Scene *, s32 *, s32))(g, list, room);
    if (n < 2) {
        func_00114FD0(list);
        return -1;
    }
    r = list[pick(n)];
    func_00114FD0(list);
    return r;
}

/* the creatures' two classes (0x1600 bytes on the event character base) */
static void *creature_init(u8 *o, void **vtbl) {
    AT(o, 0x0, void **) = Actor_vtable;
    AT(o, 0x20, s32) = 0x0FFFFFFF;
    AT(o, 0x24, s32) = 0x02000000;
    AT(o, 0x0, void **) = Character_vtable;
    AT(o, 0x1380, s32) = 0;
    AT(o, 0x153C, u8) = 0;
    AT(o, 0x0, void **) = vtbl;
    return o;
}

/* 0x0039B230 */
void *CreatureB_ctor(void *o) {
    return creature_init(o, CreatureB_vtable);
}

/* 0x0039B280 */
void *CreatureA_ctor(void *o) {
    return creature_init(o, CreatureA_vtable);
}

/* a placed character (gCreatures) of the creature class in slot `k`, its model too for
 * slots 7..9; its memory block (NULL: none) */
static inline __attribute__((always_inline)) u8 *creature_new(u8 *cr, u32 k, s32 other) {
    u8 *mem = ((u8 *(*)(u8 *, s32))AT(AT(cr, 0x28, u8 *), 0x8, void *))(cr, 0x1600);
    u8 *o = Creature_new(0x1600, mem);

    if (o != NULL) {
        o = other ? CreatureB_ctor(o) : CreatureA_ctor(o);
    }
    AT(cr, k * 4, u8 *) = o;
    AT(AT(cr, k * 4, u8 *), 0x20, s32) = k;
    if (k >= 7 && k < 10) {
        void *part = ((void *(*)(u8 *))AT(AT(cr, 0x28, u8 *), 0xC, void *))(cr);

        part = Model_new(0x890, part);
        if (part != NULL) {
            part = Kind33Model_ctor(part);
        }
        AT(AT(cr, k * 4, u8 *), 0xF0, void *) = part;
    }
    return mem;
}

/* place a creature (flags bit 7: of the other class, in a free slot 7..9; else at triangle
 * `tri`... -1: a free slot 0..5, otherwise slot 6) in `room` with kind `kind` and the rest;
 * out of this room: no triangle */
/* 0x0039AEA0 */
void SceneGame_PlaceCreature(Scene *g, u32 room, s32 tri, s32 a3, u32 flags, s32 kind, s32 which, s32 a7, f32 f) {
    VObject *hm = (VObject *)((u8 *)g + 0x706480);
    u8 *mem = NULL;
    s32 k, n;

    if (flags & 0x80) {
        s32 busy;

        for (n = 0, k = 7;;) {   /* the first free slot */
            busy = ((s32 (*)(VObject *, s32))AT(AT(hm, 0x28, u8 *), 0x10, void *))(hm, k & 0xFF);
            if (busy == 0) {
                break;
            }
            k++;
            n++;
            if (k >= 0xA) {
                break;
            }
        }
        if (n >= 3) {
            return;
        }
        if (busy == 0) {
            mem = creature_new(gCreatures, k & 0xFF, 1);
        }
    } else if (which == -1) {
        s32 busy;

        for (n = 0, k = 0;;) {   /* the first free slot */
            busy = ((s32 (*)(VObject *, s32))AT(AT(hm, 0x28, u8 *), 0x10, void *))(hm, k & 0xFF);
            if (busy == 0) {
                break;
            }
            k++;
            n++;
            if (k >= 6) {
                break;
            }
        }
        if (n >= 6) {
            return;
        }
        if (busy == 0) {
            mem = creature_new(gCreatures, k & 0xFF, 0);
        }
    } else {
        u8 *cr = gCreatures;
        u8 *o;

        mem = ((u8 *(*)(u8 *, s32))AT(AT(cr, 0x28, u8 *), 0x8, void *))(cr, 0x1600);
        o = Creature_new(0x1600, mem);
        if (o != NULL) {
            o = CreatureA_ctor(o);
        }
        AT(cr, 0x18, u8 *) = o;
        k = 6;
        AT(AT(cr, 0x18, u8 *), 0x20, s32) = k;
    }
    if (mem == NULL) {
        return;
    }
    VCALL(mem, 0xC, void (*)(void *))(mem);
    AT(mem, 0x28, u8) = 1;
    if ((room & 0xFFFF) != (u32)VCALL(g, 0xA4, s32 (*)(Scene *))(g)) {
        tri = -1;
    }
    ((void (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, f32, s64))AT(AT(hm, 0x28, u8 *), 0x18, void *))(
        hm, room & 0xFFFF, (s16)tri, a3, k & 0xFF, flags, (s16)kind, -1, f, (u16)a7);
}

/* the room's creatures (count +0x105344D, at most 6 - 3 of the other class, flag
 * +0x105344E bit 7, in slots 7..): each slot not yet placed gets one in a random room next to
 * the current one (within reach, SceneGame_RoomsNear, and through one of its exits); slots 7..9 also
 * get a model */
/* 0x0039B2D0 */
void SceneGame_RoomCreatures(Scene *g) {
    u8 alt = AT(g, 0x105344E, u8) & 0x80;
    u8 *count = &AT(g, 0x105344D, u8);
    VObject *placed = (VObject *)((u8 *)g + 0x706480);
    VObject *rooms = gRooms;
    u8 *pool = gCreatures;   /* (its pool's vtable at +0x28) */
    s32 i;

    if (*count >= (alt ? 4 : 7)) {
        *count = alt ? 3 : 6;
    }
    for (i = 0; i < *count; i++) {
        u8 slot = alt ? i + 7 : i;
        s32 room, *list;
        u32 n, m, k, e;
        u8 *mem;
        u8 *o;

        if (VCALL_AT(placed, 0x28, 0x10, s32 (*)(VObject *, u8))(placed, slot) != 0) {
            continue;
        }
        room = SG_ROOM(g);
        list = func_00114FA8(0x104);
        if (list == NULL) {
            continue;
        }
        n = VCALL((VObject *)g, 0xF4, u32 (*)(Scene *, s32 *, s32))(g, list, room);
        m = 0;
        for (k = 0; k < n; k++) {
            if (list[k] == -1 || list[k] == room) {
                continue;
            }
            for (e = 0; e < 8; e++) {
                if (list[k] == VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, room, e & 0xFF)) {
                    list[m++] = list[k];
                }
            }
        }
        if (m == 0) {
            func_00114FD0(list);
            continue;
        }
        k = pick(m);
        mem = VCALL_AT(pool, 0x28, 0x8, void *(*)(void *, u32))(pool, 0x1600);
        o = Creature_new(0x1600, mem);
        if (o != NULL) {
            o = alt ? CreatureB_ctor(o) : CreatureA_ctor(o);
        }
        AT(pool, slot * 4, u8 *) = o;
        AT(AT(pool, slot * 4, u8 *), 0x20, s32) = slot;
        if (slot >= 7 && slot < 10) {
            void *mm = Model_new(0x890, VCALL_AT(pool, 0x28, 0xC, void *(*)(void *))(pool));

            if (mm != NULL) {
                mm = Kind33Model_ctor(mm);
            }
            AT(AT(pool, slot * 4, u8 *), 0xF0, void *) = mm;
        }
        if (mem != NULL) {
            VCALL((VObject *)mem, 0xC, void (*)(void *))(mem);
            AT(mem, 0x28, u8) = 1;
            VCALL_AT(placed, 0x28, 0x14, void (*)(VObject *, s32, s32, u8, s32))(placed, list[k], -1, slot, 0);
        }
        func_00114FD0(list);
    }
}

/* SubScreen +0x40 at the start: flag +0x73EEE0 */
/* 0x0039BAC0 */
void SceneGame_SubScreenStart(Scene *g) {
    AT(g, 0x73EEE0, u8) = 1;
}

/* save the game into the resident data (gGamePtr): save slot `slot`'s header (+0x70, 0x18
 * each: room, the sub screen's +0x30, a flag of +0x70, the date, +0x1004..+0x1007) and the
 * snapshot +0x190 (room, entry, Fiona's triangle / +0xE8 / +0xEC / position / heading, the six
 * characters' kinds and activity, the progress flags +0x50, the rooms' +0x1010 (13 words), the
 * sub screen's +0x18F0 and its own part (SubScreen_ToSave)), after the partner / stalker / placed
 * things / gPlacedThings save their state */
/* 0x0039B800 */
void SceneGame_Save(Scene *g, u32 slot) {
    u8 *rd = gGamePtr;
    u8 *h = rd + 0x70 + (slot & 0xFF) * 0x18;
    u8 *s = rd + 0x190;
    VObject *sub = (VObject *)((u8 *)g + 0xF87240);
    VObject *placed = (VObject *)((u8 *)g + 0x706480);
    const s32 *w;
    s32 i;

    AT(h, 0x4, s32) = SG_ROOM(g);
    AT(h, 0x9, u8) = VCALL(sub, 0x30, u8 (*)(VObject *))(sub);
    AT(h, 0xA, u8) = (AT(g, 0x70, u32) & 0x8000) != 0;
    func_00110878(h + 0xB);
    AT(h, 0x13, u8) = AT(g, 0x1004, u8);
    AT(h, 0x14, u8) = AT(g, 0x1005, u8);
    AT(h, 0x15, u8) = AT(g, 0x1006, u8);
    AT(h, 0x16, u8) = AT(g, 0x1007, u8);
    AT(s, 0x4, s32) = SG_ROOM(g);
    AT(s, 0x8, s32) = AT(g, 0x73F240 + (AT(g, 0xF6C1B0, s32) == 0) * 4, s32);
    AT(s, 0xC, s32) = AT(gCharPlayer, 0x34, s32);
    AT(s, 0x10, s32) = AT(gCharPlayer, 0xE8, s32);
    AT(s, 0x14, s32) = AT(gCharPlayer, 0xEC, s32);
    for (i = 0; i < 6; i++) {
        if (gCharacters[i] != NULL) {
            AT(s, 0x18 + i, u8) = AT(gCharacters[i], 0x153C, u8);
            AT(s, 0x1E + i, u8) = AT(gCharacters[i], 0x28, u8);
        } else {
            AT(s, 0x18 + i, u8) = 0xFF;
            AT(s, 0x1E + i, u8) = 0;
        }
    }
    sceVu0CopyVector((f32 *)(s + 0x30), (f32 *)((u8 *)gCharPlayer + 0x10));
    sceVu0CopyVector((f32 *)(s + 0x40), (f32 *)((u8 *)gCharPlayer + 0x50));
    VCALL((VObject *)gCharPartner, 0x6C, void (*)(void *))(gCharPartner);
    if (gCharSlot2 != NULL) {
        VCALL((VObject *)gCharSlot2, 0x6C, void (*)(void *))(gCharSlot2);
    }
    VCALL_AT(placed, 0x28, 0x1C, void (*)(VObject *))(placed);
    VCALL(gPlacedThings, 0x18, void (*)(VObject *))(gPlacedThings);
    Progress_ClearFlag(&AT(g, SG_PROGRESS, Progress), 0x18);
    Progress_CopyState((u8 *)g + 0x48, s + 0x50);
    w = VCALL(gRooms, 0x7C, const s32 *(*)(VObject *))(gRooms);
    for (i = 0; i < 13; i++) {
        AT(s, 0x1010 + i * 4, s32) = w[i];
    }
    AT(s, 0x18F0, u8) = VCALL(sub, 0x30, u8 (*)(VObject *))(sub);
    SubScreen_ToSave((SubScreen *)sub, s);
}

/* ---- the menu sub-states (+0x1053440): each draws the world as it stands (frozen) under its
 * screen and goes back to play (SceneGame_SubPlay) when done ---- */

extern const PTMF SceneGame_SubPlay_ptmf3, SceneGame_SubPlay_ptmf4, SceneGame_SubPlay_ptmf5, SceneGame_SubPlay_ptmf6;   /* back to play */

/* the frozen world drawn (rooms, progress - `chars`: 0 unless flag 0x17, 1 with the placed
 * things unless 0x17 without 0x24, 2 both always - effects, panic, the event's message, the
 * task, the sub screen's +0x24 when `sub`) */
static void frozen_draw(Scene *g, s32 chars, s32 sub) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    VObject *ev = (VObject *)((u8 *)g + SG_EVENT);

    RoomMgr_Draw((u8 *)g + 0x73EE80, AT(g, 0xF6C1B0, s32));
    if (chars == 2 || (chars == 1 && (!Progress_TestFlag(prog, 0x17) || Progress_TestFlag(prog, 0x24)))) {
        Progress_DrawChars(prog);
        Creatures_Draw((u8 *)g + 0x706480);
    } else if (chars == 0 && !Progress_TestFlag(prog, 0x17)) {
        Progress_DrawChars(prog);
    }
    PlacedThings_Draw((u8 *)g + 0x6FC380);
    SceneGame_ActionPrompt(g);
    RoomEffects_Draw((u8 *)g + 0xF6CD30);
    EffectMgr_Draw((u8 *)g + 0xF6E200);
    if (!(u8)Progress_TestFlag(prog, 0xF) && !camdir_busy(g)) {
        ScreenFade_Frame((u8 *)g + 0x7F8, 1);
    }
    if (AT(g, 0xF6C1A3, u8) != 0) {
        VCALL(ev, 0xB8, void (*)(VObject *, s32))(ev, AT(g, 0xF6C1A1, u8) == 6 ? 0x30 : 0x31);
    }
    Task_Draw((Task *)((u8 *)g + 0xF6B6B8));
    if (sub) {
        VCALL((u8 *)g + 0xF87240, 0x24, void (*)(void *))((u8 *)g + 0xF87240);
    }
}

/* +0x1053440 back to play */
static inline void to_play(Scene *g, const PTMF *back) {
    ptmf_set(&AT(g, 0x1053440, PTMF), back);
}

/* paused: the pause screen; closed: quitting (flag 0x1C) - once the loader is idle, every
 * scene saves and the game goes to the title (resident +0x4 = 2) - or back to play */
/* 0x0039DF10 */
void SceneGame_SubPaused(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);

    Events_RunPhase((u8 *)g + SG_EVENT, 3);
    if (!(u8)Progress_TestFlag(prog, 8)) {
        frozen_draw(g, 1, 1);
        AvoidPrompt_Update((u8 *)g + 0x1053480);
    } else if (Progress_TestFlag(prog, 0x28)) {
        rng_tick(g);
    }
    Pause_Update((u8 *)g + 0x73EBA0);
    if (Progress_TestFlag(prog, 6)) {
        if (Progress_TestFlag(prog, 0x1C)) {
            VObject *loader = gFileLoader;

            if (VCALL(loader, 0x24, s32 (*)(VObject *))(loader) != 2) {
                scenes_to_save();
                VCALL(loader, 0x1C, void (*)(VObject *))(loader);
                AT(gGamePtr, 0x4, s32) = 2;
                AT(gGamePtr, 0x10, s32) = 1;
            }
            return;
        }
        VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 0);
        to_play(g, &SceneGame_SubPlay_ptmf5);
    }
    Progress_ClearFlag(prog, 6);
}

/* a movie paused over play: the world goes on (as in play, without the control), the pause
 * screen; closed: back to play */
/* 0x0039D990 */
void SceneGame_SubMoviePaused(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    VObject *ev = (VObject *)((u8 *)g + SG_EVENT);
    u8 *cam = (u8 *)g + SG_CAMDIR;
    u32 *flags = &AT(g, SG_FRAMEFLAGS, u32);
    s32 i;

    Progress_PursuerRequest(prog);
    CamDirector_Ease(cam);
    VCALL(g, 0xDC, void (*)(Scene *))(g);
    Events_RunPhase((u8 *)ev, 1);
    Progress_RelationChanges(prog);
    Progress_ResolveRelations(prog);
    Progress_OwnRequests(prog);
    Progress_CharRequests(prog);
    Events_RunPhase((u8 *)ev, 2);
    Events_RunCharScripts(ev);
    AT(g, 0x1170, u8) = Progress_CameraFollow(prog, AT(g, 0x1170, u8));
    if (AT(g, 0x1170, u8) != 0xFF) {
        u8 *c = (u8 *)gCharacters[AT(g, 0x1170, u8)];

        ((void (*)(u8 *, s32, s32))AT(AT(cam, 0x64, u8 *), 0x64, void *))(cam, AT(c, 0xE8, s32), AT(c, 0xEC, s32));
    }
    CamDirector_Track(cam);
    Events_RunPhase((u8 *)ev, 3);
    if (camdir_busy(g) && !(u8)VCALL(ev, 0xC0, s32 (*)(VObject *))(ev)) {
        *flags = (*flags & ~0x3F0) | 0x3F0;
    } else {
        *flags &= ~0x3F0;
    }
    CamDirector_Update(cam);
    if (((*flags >> 4) & 0x3F) == 0) {
        Progress_CharsThink(prog);
        Progress_CharsFrame(prog);
    }
    RoomMgr_Update((u8 *)g + 0x73EE80);
    if (((*flags >> 4) & 0x3F) == 0) {
        Items_Update((u8 *)g + 0xF87248);
        PlayTime_Tick((u8 *)g + 0x1004);
        PlacedThings_Update((u8 *)g + 0x6FC380);
        RoomEffects_Update((u8 *)g + 0xF6CD30);
        EffectMgr_Update((u8 *)g + 0xF6E200);
        for (i = 0; i < 6; i++) {
            if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) == 1) {
                Character_Hearing(gCharacters[i]);
            }
        }
    }
    if (!(u8)Progress_TestFlag(prog, 8)) {
        frozen_draw(g, 0, 0);
        AvoidPrompt_Update((u8 *)g + 0x1053480);
    } else if (Progress_TestFlag(prog, 0x28)) {
        rng_tick(g);
    }
    Pause_Update((u8 *)g + 0x73EBA0);
    if (Progress_TestFlag(prog, 6)) {
        VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 0);
        SceneGame_Danger(g);
        to_play(g, &SceneGame_SubPlay_ptmf6);
    }
    Progress_ClearFlag(prog, 6);
}

/* the sub screen (items, files...): Select closes it (flag 4); drawn over the frozen world
   (when +0xF88938 asks for it) or alone; closed: back to play */
/* 0x0039E7D0 */
void SceneGame_SubSubScreen(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    u8 *cam = (u8 *)g + SG_CAMDIR;

    if (gPadPressed & 1) {
        Progress_SetFlag(prog, 4);
    }
    CamDirector_Ease(cam);
    CamDirector_Track(cam);
    CamDirector_Update(cam);
    Events_RunPhase((u8 *)g + SG_EVENT, 3);
    if (AT(g, 0xF88938, u8) != 0 && !(u8)Progress_TestFlag(prog, 8)) {
        frozen_draw(g, 2, 1);
    } else {
        VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
        VCALL((u8 *)g + 0x6FC258, 0x20, void (*)(void *))((u8 *)g + 0x6FC258);
    }
    SubScreen_Update((SubScreen *)((u8 *)g + 0xF87240));
    PlayTime_Tick((u8 *)g + 0x1004);
    if (Progress_TestFlag(prog, 4)) {
        VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 0);
        to_play(g, &SceneGame_SubPlay_ptmf3);
    }
    Progress_ClearFlag(prog, 4);
}

/* the transition screen (+0x73EB40: game over / continue) running: the world drawn behind it
 * while +0x73EB41; done (flag 0xC): with +0x1FBF01 clear - its mode 2 (game over) keeps
 * Hewie's trust and two values for the continue (+0xFF4 / +0xFFE / +0x1000) - every scene
 * saves and the game goes on (resident +0x4: 5 continue, else 2 title); else the game restarts
 * in place: flag 8, the camera director released, the panic reset, door 0x10C locked, the room
 * re-entered, Hewie's action 0x37 and the stalker removed, then back to play */
/* 0x0039E380 */
void SceneGame_SubTransition(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    u8 *cam = (u8 *)g + SG_CAMDIR;

    GameOver_Update((GameOver *)((u8 *)g + 0x73EB40));
    if (AT(g, 0x73EB41, u8) != 0 && !Progress_TestFlag(prog, 8)) {
        CamDirector_Ease(cam);
        CamDirector_Track(cam);
        CamDirector_Update(cam);
        Events_RunPhase((u8 *)g + SG_EVENT, 3);
        RoomMgr_Draw((u8 *)g + 0x73EE80, AT(g, 0xF6C1B0, s32));
        RoomEffects_Draw((u8 *)g + 0xF6CD30);
        EffectMgr_Draw((u8 *)g + 0xF6E200);
        Progress_DrawChars(prog);
        Creatures_Draw((u8 *)g + 0x706480);
        ScreenFade_Frame((u8 *)g + 0x7F8, 0);
    }
    if (!Progress_TestFlag(prog, 0xC)) {
        return;
    }
    Progress_ClearFlag(prog, 0xC);
    if (AT(g, 0x1FBF01, u8) == 0) {
        if (AT(g, 0x73EB40, u8) == 2) {
            AT(g, 0xFF4, s16) = AT(gCharPartner, 0xF35CC, s16);
            AT(g, 0xFFE, s16) = VCALL(g, 0x104, s16 (*)(Scene *))(g);
            AT(g, 0x1000, s16) = VCALL(g, 0x108, s16 (*)(Scene *))(g);
            VCALL(g, 0xBC, void (*)(Scene *, s32))(g, 0);
        }
        scenes_to_save();
        VCALL(gFileLoader, 0x1C, void (*)(VObject *))(gFileLoader);
        AT(gGamePtr, 0x4, s32) = AT(g, 0x73EB40, u8) == 2 ? 5 : 2;
        AT(gGamePtr, 0x10, s32) = 0;
        return;
    }
    VCALL(gFileLoader, 0x1C, void (*)(VObject *))(gFileLoader);
    Progress_SetFlag(prog, 8);
    AT(g, 0x1FBF01, u8) = 0;
    VCALL((VObject *)gCamDirector, 0x40, void (*)(VObject *, f32))(gCamDirector, -1.0f);
    Panic_SetStage((u8 *)g + 0x7F8, 0);
    Progress_DoorClearBit1(prog, 0x10C);
    VCALL(g, 0xE8, void (*)(Scene *, s32, s32, s32))(g, VCALL(g, 0xA4, s32 (*)(Scene *))(g), 1, 0);
    VCALL((VObject *)gCharPartner, 0x64, void (*)(void *, s32, s32, s32))(gCharPartner, 0x37, -1, 0);
    Progress_RemoveChar(prog, 2, 0);
    VCALL(g, 0xB0, void (*)(Scene *, s32))(g, 0x37);
    AT(g, 0x44, s32) = 1;
    AT(g, 0xF6B6B2, u8) = 0x81;
    to_play(g, &SceneGame_SubPlay_ptmf4);
}

/* room `room` (>= 0) loaded into the spare room slot unless it is already there */
/* 0x0039D000 */
void SceneGame_LoadSpareRoom(Scene *g, s32 room) {
    u32 spare;

    if (room & 0x80000000) {
        return;
    }
    spare = AT(g, 0xF6C1B0, s32) == 0;
    if (room != AT(g, 0x73F240 + spare * 4, s32)) {
        RoomMgr_LoadRoom((u8 *)g + 0x73EE80, room, spare);
    }
}

/* ---- small SceneGame methods left (2026-10-05) ---- */

extern const u16 D_0044C5B0[];   /* flag numbers, 0xFFFF-terminated */

/* the room data of the file slot not in use (+0xF6C1B0 says which), once loaded; NULL before */
/* 0x0039A510 */
u8 *SceneGame_SpareRoomData(u8 *g) {
    u8 k = AT(g, 0xF6C1B0, s32) == 0;

    if (RoomMgr_SlotLoaded(g + 0x73EE80, k)) {
        return NULL;
    }
    k = AT(g, 0xF6C1B0, s32) == 0;
    return g + 0x748840 + k * 0x2A0000;
}

/* start loading that slot */
/* 0x0039A5B0 */
void SceneGame_LoadSpareSlot(u8 *g) {
    RoomMgr_LoadSlot(g + 0x73EE80, AT(g, 0xF6C1B0, s32) == 0);
}

/* how many flags D_0044C5B0 lists */
/* 0x0039A5E0 */
s32 SceneGame_BonusFlagCount(void) {
    const u16 *f = D_0044C5B0;
    s32 n = 0;

    while (*f != 0xFFFF) {
        f++;
        n++;
    }
    return n;
}

/* how many of them are set in o's bits (+0x5C) */
/* 0x0039A620 */
s32 SceneGame_BonusFlagsSet(u8 *o) {
    const u16 *f = D_0044C5B0;
    s32 n = 0;

    for (; *f != 0xFFFF; f++) {
        if ((AT(o, 0x5C + (*f >> 5) * 4, u32) & (1u << (*f & 0x1F))) != 0) {
            n++;
        }
    }
    return n;
}

/* what is going on, as bits: 1 mode 2 or a stalker (slot 2+) active in the room, 2 the
 * partner busy (+0xE0), 4 someone else busy or the low bits of +0xF6CD24 set */
/* 0x0039A690 */
u8 SceneGame_Busy(u8 *g) {
    u8 bits = 0;
    s32 i;

    if (g[0x50] == 2) {
        bits |= 1;
    }
    for (i = 0; i < 6; i++) {
        u8 *c = (u8 *)gCharacters[i];

        if (c == NULL || c[0x28] == 0) {
            continue;
        }
        if (c[0xE0] != 0) {
            bits |= i == 1 ? 2 : 4;
        }
        if (i >= 2 && AT(c, 0x30, s32) == VCALL(g, 0xA4, s32 (*)(void *))(g)) {
            bits |= 1;
        }
    }
    if (AT(g, 0xF6CD24, u32) & 0xF) {
        bits |= 4;
    }
    return bits;
}

/* is the scene (+0xA4) in state 7? */
/* 0x0039A830 */
s32 SceneGame_State7(u8 *g) {
    return VCALL(g, 0xA4, s32 (*)(void *))(g) == 7;
}

/* end and delete its object at +0x106503C (+0x14, then destructor +0x8) */
/* 0x0039A860 */
void SceneGame_EndMusic(u8 *g) {
    void *o = AT(g, 0x106503C, void *);

    if (o != NULL) {
        VCALL(o, 0x14, void (*)(void *))(o);
        o = AT(g, 0x106503C, void *);
        if (o != NULL) {
            VCALL(o, 0x8, void (*)(void *, s32))(o, 1);
        }
        AT(g, 0x106503C, void *) = NULL;
    }
}

/* clear the 17 words at +0x2718 */
/* 0x00179E60 */
void SceneGame_Clear2718(u8 *o) {
    s32 i;

    AT(o, 0x2718, s32) = 0;
    AT(o, 0x271C, s32) = 0;
    for (i = 0; i < 15; i++) {
        AT(o, 0x2720 + i * 4, s32) = 0;
    }
}

/* destructor (vtable D_0046B350) */
/* 0x001FB0F0 */
void *Obj46B350_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046B350;
        gLights = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the base (RoomBase_vtable): destructor */
/* 0x001FB3B0 */
void *RoomBase_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = RoomBase_vtable;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046BA68) */
/* 0x001FB400 */
void *Obj46BA68_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046BA68;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the base's defaults: nothing (0) */
/* 0x00209200 */
s32 RoomBase_CharEnterScript(void *o) {   /* +0x30 */
    return 0;
}

/* 0x00209830 */
s32 RoomBase_EnterScript(void *o) {   /* +0xC */
    return 0;
}

/* destructor (vtable D_0046C6F0) */
/* 0x00225620 */
void *Obj46C6F0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046C6F0;
        gCamDirector = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046C770) */
/* 0x0025C850 */
void *RoomMeshes_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046C770;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x0, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046D780) */
/* 0x00268110 */
void *Obj46D780_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D780;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x0, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable Message_vtable) */
/* (possibly dead code: nothing in the game references it) */
/* 0x002D00A0 */
void *Message_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Message_vtable;
        AT(o, 0x0, void **) = D_0046A0D0;
        gBootMessage = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* (vtable) a soft reset: once the other room file is in and the loader is idle (3), the sounds
 * stopped (every channel but 5), the room state, path data, VRAM and progress reset, the
 * current scene called back (D_0044C598), the renderer, BGM and effects reset, the volume back
 * to full, and the scene flagged (+0x11) to start over */
void SceneGame_OnSoftReset(u8 *g) {
    VObject *snd;
    u8 *bgm;
    u8 *sc;

    if (RoomMgr_SlotLoaded(g + 0x73EE80, AT(g, 0xF6C1B0, s32) ^ 1) != 0) {
        return;
    }
    if (VCALL(gFileLoader, 0x24, s32 (*)(void *))(gFileLoader) != 3) {
        return;
    }
    VCALL(gRumble, 0x10, void (*)(VObject *))(gRumble);
    snd = gSound;
    VCALL(snd, 0x8C, void (*)(VObject *))(snd);
    VCALL(snd, 0xC, void (*)(VObject *))(snd);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 7);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 6);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 4);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 3);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 2);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 1);
    VCALL(snd, 0x64, void (*)(VObject *, s32))(snd, 0);
    Creatures_ShowMessage(g + 0x706480);
    Progress_ResetParts((Progress *)(g + 0x40));
    RoomMgr_Clear(g + 0x73EE80);
    Lights_ReleaseVram(g + 0xF6C1C0);
    Progress_RemoveAll((Progress *)(g + 0x40));
    sc = AT(gSceneTable, 4, u8 *);
    if (sc != NULL) {
        ptmf_set(&AT(sc, 4, PTMF), &D_0044C598);
        sc = AT(gSceneTable, 4, u8 *);
        VCALL(sc, 0x14, void (*)(void *))(sc);
    }
    AT(VCALL(gRenderer, 0x2C, u8 *(*)(VObject *))(gRenderer), 0x1C, u8) = 0;
    BgmCtl_StopNow((BgmCtl *)(g + 0x1053424));
    bgm = (u8 *)gAdx;
    Bgm_Release((Bgm *)bgm);
    VCALL((VObject *)(g + 0x6FC380), 0x24, void (*)(VObject *))((VObject *)(g + 0x6FC380));
    ((void (*)(void *))Renderer_Call5C)(g + 0xF6CD30);
    AvoidPrompt_Unload(g + 0x1053480);
    VCALL(g, 0xFC, void (*)(void *))(g);
    snd = gSound;
    VCALL(snd, 0x94, void (*)(VObject *, s32))(snd, 0xFF);
    VCALL(snd, 0xAC, void (*)(VObject *, f32))(snd, 1.0f);
    {
        volatile f32 *vol = &AT(bgm, 0x120, f32);

        *vol = 1.0f;
        if (*vol < 0.0f) {
            *vol = 0.0f;
        }
        if (!(*vol <= 1.0f)) {
            *vol = 1.0f;
        }
    }
    Bgm_ApplyVolume((Bgm *)bgm);
    SceneGame_Clear2718(gChainPool);
    Clear_C700(gSkelPool);
    g[0x11] = 1;
}

/* ---- SceneGame's destructor and the member destructors it needs (2026-10-05) ---- */

extern void *NavMesh_vtable[], *D_0046AA40[], *D_0046A9C0[], *Overlay_vtable[], *Helper469D00_vtable[], *D_0046A9B0[];
extern void *D_00473440[], *BgmCtl_vtable[], *D_0046A100[], *SubScreen_vtable[], *Heap_vtable[], *D_004699E0[];
extern void *BlockPool_vtable[], *D_0046C660[], *CamDirector_vtable[], *D_0046C6F0[], *Lights_vtable[], *D_0046B350[];
extern void *Events_vtable[], *D_0046B3B8[], *RoutePlanner_vtable[], *D_0046C530[], *PathPlan_vtable[], *D_0046AC00[];
extern void *Hewie_vtable[], *Character_vtable[], *Actor_vtable[];

/* (D_0046A9C0): its quad drawer (+0x120) and its task's child (+0x88) */
/* 0x00179B10 */
void *Obj46A9C0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046A9C0;
        AT(o, 0x120, void **) = Overlay_vtable;
        AT(o, 0x120, void **) = Helper469D00_vtable;
        if (AT(o, 0x88, void *) != NULL) {
            Task_dtor(AT(o, 0x88, Task *), 1);
            AT(o, 0x88, void *) = NULL;
        }
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* (vtable at +0x4C, D_0046A9B0) */
/* 0x00179AC0 */
void *Obj46A9B0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x4C, void **) = D_0046A9B0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the progress object: gProgress cleared */
/* 0x002D0F90 */
void *Progress_dtorGlobal(void *o, s32 flags) {
    if (o != NULL) {
        gProgress = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* 0x002D0FE0 */
void *Obj46BA68_ctor(u8 *p) {
    F(p, 0x0, void *) = D_0046BA68;
    p[0x4] = 0;
    return p;
}

/* 0x002D1000 */
void *Cutscene_ctor(u8 *p) {
    gCutscene = (VObject *)p;
    F(p, 0x0, void *) = Cutscene_vtable;
    return p;
}

/* 0x002D1020 */
void *RoomBase_ctor(u8 *p) {
    F(p, 0x0, void *) = RoomBase_vtable;
    return p;
}

/* 0x002D1040 */
void *DimOverlay_ctor(u8 *p) {
    F(p, 0x0, void *) = Helper469D00_vtable;
    F(p, 0x4, s32) = -1;
    F(p, 0x0, void *) = Overlay_vtable;
    F(p, 0x24, u32) = 0;
    F(p, 0x10, s32) = -1;
    p[0x14] = 0;
    return p;
}

/* 0x002D1080 */
void *Events_ctor(u8 *p) {
    gEvents = (VObject *)p;
    F(p, 0x0, void *) = D_0046BAA0;
    return p;
}

/* 0x002D10A0 */
void *Obj46BA80_ctor(u8 *p) {
    F(p, 0x0, void *) = D_0046BA80;
    F(p, 0x4, u32) = 0;
    p[0x8] = 0;
    return p;
}

/* 0x002D1130 */
void *NavGroups_ctor(u8 *p) {
    gRoomEventObj = p;
    F(p, 0x0, void *) = NavGroups_vtable;
    F(p, 0x4, u32) = 0;
    F(p, 0x8, u32) = 0;
    return p;
}

/* 0x002D12C0 */
void *Rooms_ctor(u8 *p) {
    gRooms = (VObject *)p;
    F(p, 0x0, void *) = Rooms_vtable;
    return p;
}

/* 0x002D12E0 */
void *NavMesh_ctor(u8 *p) {
    s32 i;

    gNavMesh = (NavMesh *)p;
    F(p, 0x0, void *) = NavMesh_vtable;
    for (i = 0x4; i <= 0x18; i += 4) {
        F(p, i, u32) = 0;
    }
    return p;
}

/* 0x002D1320 */
void *Obj46D780_ctor(u8 *p) {
    F(p, 0x0, void *) = Helper469D00_vtable;
    F(p, 0x4, s32) = -1;
    F(p, 0x0, void *) = D_0046D780;
    F(p, 0x20, u32) = 0;
    F(p, 0x24, u32) = 0;
    F(p, 0x18, s32) = -1;
    return p;
}

/* 0x002D1360 */
void *RoomMeshes_ctor(u8 *p) {
    F(p, 0x0, void *) = Helper469D00_vtable;
    F(p, 0x4, s32) = -1;
    F(p, 0x0, void *) = D_0046C770;
    F(p, 0x7C, u32) = 0;
    F(p, 0x80, u32) = 0;
    F(p, 0x64, s32) = -1;
    p[0x84] = 0;
    p[0x60] = 0;
    F(p, 0x68, u32) = 0;
    p[0x85] = 0;
    return p;
}

/* 0x002D1470 */
void *Obj46A9B0_ctor(u8 *p) {
    F(p, 0x4C, void *) = D_0046A9B0;
    return p;
}

/* 0x002D1490 */
void *Creatures_ctor(u8 *p) {
    u8 *a = p + 0xDC40;
    u8 *b = p + 0xF630;

    gCreatures = p;
    F(p, 0x28, void *) = Creatures_vtable;
    F(a, 0x0, void *) = D_004699E0;
    F(a, 0x4, u32) = 0;
    F(a, 0x8, u32) = 0;
    F(a, 0x0, void *) = BlockPool_vtable;
    F(a, 0xC, u32) = 0;
    F(a, 0x10, u32) = 0;
    F(a, 0x14, u32) = 0;
    F(b, 0x0, void *) = D_004699E0;
    F(b, 0x4, u32) = 0;
    F(b, 0x8, u32) = 0;
    F(b, 0x0, void *) = BlockPool_vtable;
    F(b, 0xC, u32) = 0;
    F(b, 0x10, u32) = 0;
    F(b, 0x14, u32) = 0;
    return p;
}

/* 0x002D1510 */
void *PlacedThings_ctor(u8 *p) {
    u8 *a = p + 0xA040;

    gPlacedThings = (VObject *)p;
    F(p, 0x0, void *) = PlacedThings_vtable;
    F(a, 0x0, void *) = D_004699E0;
    F(a, 0x4, u32) = 0;
    F(a, 0x8, u32) = 0;
    F(a, 0x0, void *) = BlockPool_vtable;
    F(a, 0xC, u32) = 0;
    F(a, 0x10, u32) = 0;
    F(a, 0x14, u32) = 0;
    return p;
}

/* 0x002D1560 */
void *Message_ctor(u8 *p) {
    gBootMessage = (VObject *)p;
    F(p, 0x0, void *) = Message_vtable;
    return p;
}

/* 0x002D1580 */
void *SceneHeap_ctor(u8 *p) {
    F(p, 0x0, void *) = D_004699E0;
    F(p, 0x4, u32) = 0;
    F(p, 0x8, u32) = 0;
    F(p, 0x0, void *) = Heap_vtable;
    F(p, 0xC, u32) = 0;
    F(p, 0x10, u32) = 0;
    return p;
}

/* Progress constructor: registers the global instance. */
/* 0x002D15C0 */
void *Progress_ctor(u8 *p) {
    gProgress = (Progress *)p;
    return p;
}

/* 0x0031DDF0 */
void AvoidPrompt_Show(u8 *self, u32 id) {
    self[0x11040] = (u8)id;
    S32(self, 0x11044) = 30;
}

/* (possibly dead code: nothing in the game references it) */
/* 0x0031E0B0 */
s32 AvoidPrompt_Unload(void *self) {
    return VCALL(gTexCache, 0x14, s32 (*)(void *, s32))(gTexCache, 0x2D);
}

/* Count down the timer set by AvoidPrompt_Show; id 0xFF = none. */
/* 0x0031E0D0 */
void AvoidPrompt_Tick(u8 *self) {
    if (self[0x11040] != 0xFF) {
        S32(self, 0x11044) -= 1;
        if (S32(self, 0x11044) == 0) {
            self[0x11040] = 0xFF;
        }
    }
}

/* 0x0031E130 */
s32 AvoidPrompt_Upload(u8 *self) {
    return VCALL(gTexCache, 0x10, s32 (*)(void *, void *, s32))(gTexCache, self + 0x40, 0x2D);
}

/* destructor (vtable D_00476F40) */
/* 0x0033D990 */
void *LoadingEmblem_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00476F40;
        AT(o, 0x0, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* 0x0038C8D0 */
void *Kind33Model_ctor(u8 *m) {
    HumanModel_BaseCtor(m);
    AT(m, 0x0, void **) = Kind33Model_vtable;
    return m;
}

/* destructor: the members torn down in reverse (task manager +0x1053480, BGM +0x1053424, the sub
 * screen +0xF87240, effects +0xF6E200, room effects +0xF6CD30, +0xF6CBB0, +0xF6C1C0, the events
 * +0xF6AFB0, +0xF6A940, the path planner +0xF29740, +0xE35F80, Fiona +0xC88840, the rooms
 * +0x73EE80, the progress +0x40 with the characters in slots 2..5), then the scene base */
void *SceneGame_dtor(u8 *g, s32 flags) {
    Character **slot;
    s32 i;

    if (g == NULL) {
        return g;
    }
    AT(g, 0x0, void **) = SceneGame_vtable;
    AT(g, 0x40, void **) = D_0047A7E8;
    AT(g, 0x1053480, void **) = D_00473440;
    Task_dtor((Task *)(g + 0x10644C8), -1);
    AvoidPromptBase_dtor(g + 0x1053480, 0);
    gMovieFlag = NULL;
    AT(g, 0x1053424, void **) = BgmCtl_vtable;
    AT(g, 0x1053424, void **) = D_0046A100;
    gMusic = NULL;
    AT(g, 0xF87240, void **) = SubScreen_vtable;
    BootCard_dtor((BootCard *)(g + 0x102FD00), -1);
    TextObj_dtor(g + 0x101EBC0, -1);
    Task_dtor((Task *)(g + 0x101EAA8), -1);
    Task_dtor((Task *)(g + 0x101E9A4), -1);
    SubScreenBase_dtor((SubScreen *)(g + 0xF87240), 0);
    AT(g, 0xF7E200, void **) = Heap_vtable;
    AT(g, 0xF7E200, void **) = D_004699E0;
    gEffects = NULL;
    AT(g, 0xF6E130, void **) = BlockPool_vtable;
    AT(g, 0xF6E130, void **) = D_004699E0;
    gRoomEffects = NULL;
    AT(g, 0xF6CC14, void **) = D_0046C660;
    AT(g, 0xF6CC10, void **) = CamDirector_vtable;
    AT(g, 0xF6CC10, void **) = D_0046C6F0;
    gCamDirector = NULL;
    AT(g, 0xF6CBDC, s32) = 0;
    AT(g, 0xF6CBE8, s32) = 0;
    AT(g, 0xF6CC00, f32) = 6.0f;
    for (i = 0; i < 7; i++) {
        AT(g, 0xF6CBB8 + i * 4, s32) = 0;
    }
    AT(g, 0xF6C1C0, void **) = Lights_vtable;
    AT(g, 0xF6C1C0, void **) = D_0046B350;
    gLights = NULL;
    AT(g, 0xF6AFB0, void **) = Events_vtable;
    AT(g, 0xF6AFBC, void **) = D_0046B3B8;
    func_001002C0(g + 0xF6BBA0, (void * (*)(void *, s32))Obj46BA68_dtor, 0x30, 0x20);
    Cutscene_dtor(g + 0xF6B8E8, -1);
    Task_dtor((Task *)(g + 0xF6B6B8), -1);
    func_001002C0(g + 0xF6B0D0, RoomBase_dtor, 4, 0x110);
    Progress73EC80_dtor(g + 0xF6AFD0, -1);
    EventsBase_dtor((void **)(g + 0xF6AFBC), 0);
    Obj46BA80_dtor((void **)(g + 0xF6AFB0), 0);
    AT(g, 0xF6A940, void **) = RoutePlanner_vtable;
    AT(g, 0xF6A940, void **) = D_0046C530;
    gRoutePlanner = NULL;
    AT(g, 0xF29740, void **) = PathPlan_vtable;
    AT(g, 0xF29740, void **) = D_0046AC00;
    gSceneGameF29740 = NULL;
    AT(g, 0xE35F80, void **) = Hewie_vtable;
    AT(g, 0xE35F80, void **) = Character_vtable;
    AT(g, 0xE35F80, void **) = Actor_vtable;
    AT(g, 0xC88840, void **) = Fiona_vtable;
    AT(g, 0xC88840, void **) = Character_vtable;
    AT(g, 0xC88840, void **) = Actor_vtable;
    RoomMgr_Clear(g + 0x73EE80);
    Obstacles_dtor(g + 0x748200, -1);
    NavGroups_dtor(g + 0x7481E0, -1);
    PlacedThings_dtor(g + 0x7455C0, -1);
    Doors_dtor(g + 0x7404C0, -1);
    Rooms_dtor((VObject *)(g + 0x73F370), -1);
    NavMesh_dtor(g + 0x73F260, -1);
    Obj46D780_dtor(g + 0x73F1C0, -1);
    RoomMeshes_dtor(g + 0x73EE80, -1);
    AT(g, 0x40, void **) = Progress_vtable;
    Progress_Reset((u8 *)((Progress *)(g + 0x40)));
    for (i = 2, slot = &gCharacters[2]; i < 6; i++, slot++) {
        if (*slot != NULL) {
            VObject *objs = (VObject *)(g + 0x6FBF40);
            void *c;

            VCALL(objs, 0x14, void (*)(VObject *, void *))(objs, *slot);
            c = *slot;
            if (c != NULL) {
                VCALL(c, 0x8, void (*)(void *, s32))(c, 1);
            }
            *slot = NULL;
        }
    }
    Obj46A9C0_dtor(g + 0x73EBA0, -1);
    Obj46A9B0_dtor(g + 0x73EB40, -1);
    Creatures_dtor(g + 0x706480, -1);
    PlacedThings_Destroy(g + 0x6FC380, -1);
    Message_dtor(g + 0x6FC258, -1);
    Heap_dtor((Heap *)(g + 0x6FBF40), -1);
    NoVtable_dtor(g + 0x48, -1);
    Progress_dtorGlobal(g + 0x40, 0);
    AT(g, 0x0, void **) = Scene_vtable;
    if ((s16)flags > 0) {
        SceneHeap_delete(g);
    }
    return g;
}
