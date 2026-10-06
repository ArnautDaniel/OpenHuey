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
extern const PTMF D_0044C7A0;   /* stored at +0x1053450 */
extern const PTMF D_0044C7B0;   /* next state */

extern void *D_004562B0;
extern void *D_004562A8;

/* Fiona setups, by progress variable 0x26 (0..8), and the partner's, by variable 0x27 (0..2).
 * Probably costumes; each takes the character's player index. */

extern void *D_004699E0[];
extern void *D_00469D00[];
extern void *D_0046A0D0[];
extern void *D_0046B350[];
extern void *D_0046BB20[];
extern void *D_0046C6F0[];
extern void *D_0046C770[];
extern void *D_0046D780[];
extern void *D_0046D7D0[];
extern void *D_0046ED30[];
extern void *D_00476F40[];
void *func_00120EF0(u8 *o, s32 flags);
void *func_001FB0F0(u8 *o, s32 flags);
void *func_00225620(u8 *o, s32 flags);
void *func_0025C850(u8 *o, s32 flags);
void *func_00268110(u8 *o, s32 flags);
void *func_002D00A0(u8 *o, s32 flags);
void *func_002D0C70(u8 *o, s32 flags);
void *func_0033D990(u8 *o, s32 flags);

extern void *D_0046BA68[], *D_0046DB80[];
extern void *D_0046F350[];
extern u8 D_0046BAA0[], D_0046BA80[], D_0046DB40[];
extern u8 D_0046C3E0[];
extern void *D_0046A9D0[];
extern void *D_00456E00;
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *func_002D0FE0(u8 *p);
void *func_002D1000(u8 *p);
void *func_002D1020(u8 *p);
void *func_002D1040(u8 *p);
void *func_002D1080(u8 *p);
void *func_002D10A0(u8 *p);
void *func_002D1130(u8 *p);
void *func_002D12C0(u8 *p);
void *func_002D12E0(u8 *p);
void *func_002D1320(u8 *p);

extern void *D_0046A9B0[];
extern u8 D_0046FC00[];
extern u8 D_0046F5C0[];
extern void *D_004699C0[];
extern void *D_0046A1C0[];
void *func_002D1360(u8 *p);
void *func_002D1470(u8 *p);
void *func_002D1490(u8 *p);
void *func_002D1510(u8 *p);
void *func_002D1560(u8 *p);
void *func_002D1580(u8 *p);
void *func_002D15C0(u8 *p);

#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

void func_0031DDF0(u8 *self, u32 id);
s32 func_0031E0B0(void *self);
void func_0031E0D0(u8 *self);
s32 func_0031E130(u8 *self);

extern void *D_00474460[];

s32 func_00209200(void *o);
s32 func_00209830(void *o);

/* destructor (vtable D_004699E0) */
void *func_00120EF0(u8 *o, s32 flags) {
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
    u8 *save = (u8 *)gSystemData + 0x190;
    void *obj980;
    VObject *obj560;
    f32 *settingDst;

    VCALL(obj550, 0xC, void (*)(VObject *, s32))(obj550, 0x1571);
    func_00179EA0(D_004562B0);
    func_0017D220(D_004562A8);
    func_00176780((u8 *)prog);
    AT(game, 0xF6CD29, u8) = 0;
    func_00120C80((u8 *)game + 0x73EE80);
    AT(game, 0xF6C1B0, s32) = 0;

    if (AT(game, SG_ENTRY, s32) != ENTRY_NEW) {
        Progress *p;
        s32 flag;

#ifdef HG_NATIVE
        if (!hg_debug_no_partner())   /* native/platform/debug.c: HG_NOPARTNER */
#endif
        SetupCharacter(prog, partner, func_003A10B0);
        p = gProgress;
        Progress_SetVar(p, 0x27, 0);
        flag = (AT(game, SG_ENTRY, s32) & ENTRY_FLAG) ? 1 : 0;
        AT(game, SG_ENTRY, u32) &= 0x3FFFFFFF;
        Progress_SetFlag(prog, 0x28);
        switch (AT(game, SG_ENTRY, s32)) {
        case 0x2A:
            SetupCharacter(prog, fiona, func_003A1860);
            Progress_SetVar(p, 0x26, 1);
            AT(game, 0xF6CD28, u8) = 0;
            Progress_SetFlag(prog, 3);
            Progress_SetFlag(prog, 8);
            break;
        case 0x37:
            SetupCharacter(prog, fiona, func_003A1720);
            Progress_SetVar(p, 0x26, 0);
            AT(game, 0xF6CD28, u8) = 0;
            Progress_SetFlag(prog, 8);
            if (AT(game, 0xF6B6B2, u8) == 0xFF) {
                AT(game, 0xF6B6B2, u8) = 0;
            }
            break;
#ifdef HG_NATIVE
        default:   /* HG_ROOM debug start in any other room: Fiona as in a new game, placed
                    * once the room is in (func_0039D310) */
            sDebugPlace = 1;
            SetupCharacter(prog, fiona, func_003A1860);
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
        func_002A7C70(save + 0x50, (u8 *)game + 0x48);
        p = gProgress;
        switch (Progress_GetVar(p, 0x26) & 0xFF) {
        case 0: SetupCharacter(prog, fiona, func_003A1720); break;
        case 1: SetupCharacter(prog, fiona, func_003A1860); break;
        case 2: SetupCharacter(prog, fiona, func_003A15A0); break;
        case 3: SetupCharacter(prog, fiona, func_003A1420); break;
        case 6: SetupCharacter(prog, fiona, func_003A1310); break;
        case 7: SetupCharacter(prog, fiona, func_003A1220); break;
        case 8: SetupCharacter(prog, fiona, func_003A1190); break;
        }
        switch (Progress_GetVar(p, 0x27) & 0xFF) {
        case 0: SetupCharacter(prog, partner, func_003A10B0); break;
        case 1: SetupCharacter(prog, partner, func_003A1020); break;
        case 2: SetupCharacter(prog, partner, func_003A0F90); break;
        }
        func_00171160(prog, save[0x1A]);
    }

    func_0016D350(prog, (Progress_TestFlag(prog, 3) & 0xFF) == 1);
    func_00176650(prog);
    func_00176550(prog);
    func_001F9D90((u8 *)game + 0xF6C1C0);
    obj980 = gAdx;
    func_002D2370(obj980, (u8 *)game + 0x1030240);
    func_002E34D0((u8 *)game + 0x1053424);
    AT(game, 0x1053440, PTMF) = sGameStateNull;
    func_002E2890((u8 *)game + 0x706480, &sGameStateNull);
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
    func_002D1FD0(obj980);
    AT(game, 0x1065040, s32) = VCALL(obj550, 0x10, u32 (*)(VObject *))(obj550) & 0xFF;
    Progress_ClearFlag(prog, 0x2B);
    Progress_ClearFlag(prog, 0x2C);
    AT(game, 0x1053450, PTMF) = D_0044C7A0;
    Scene_SetState(game, &D_0044C7B0);
}

/* ---- construction ---- */

#include "actor.h"
#include "subscreen.h"
#include "task.h"

extern void *Scene_vtable[], *SceneGame_vtable[], *Progress_vtable[], *Fiona_vtable[];
extern void *D_0047A7E8[];          /* SceneGame's second base (Progress) */
extern void *D_00469C20[], *D_00469C60[];   /* Actor, Character */
extern void *D_0046A120[];          /* the partner (Hewie) */
extern void *D_0046ABB0[], *D_0046C520[], *D_0046B3A0[], *D_0046B3B8[], *D_0046B300[];
extern void *D_0046C6F0[], *D_0046C660[], *D_0046C668[];
extern void *D_004699E0[], *D_004699C0[], *D_0046A1C0[];
extern void *D_0047A790[];          /* the sub screen */
extern void *D_0046A110[];          /* the music controller */
extern void *D_00473440[];
extern const PTMF sSceneEntryState;
extern void *gSceneGameF29740;
extern void *D_0045D1F0, *D_00456DE8;

/* the characters' common construction (Actor, then Character) */
static inline void Character_Construct(Character *c, s32 slot) {
    c->a.vtbl = D_00469C20;
    c->a.slot = slot;
    c->a.flags24 = 0x2000000;
    c->a.vtbl = D_00469C60;
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
    func_002D15C0(prog);
    AT(prog, 0x0, void **) = Progress_vtable;
    func_002D15B0(prog + 8);
    AT(prog, 0x1FBEC0, u8) = 0;
    AT(prog, 0x1FBEC1, u8) = 0;
    func_002D1580(prog + 0x6FBF00);
    func_002D1560(prog + 0x6FC218);
    func_002D1510(prog + 0x6FC340);
    func_002D1490(prog + 0x706440);
    func_002D1470(prog + 0x73EB00);
    func_002D13B0(prog + 0x73EB60);
    func_00176780((u8 *)((Progress *)prog));
    func_00169260((VObject *)(prog + 0x6FBF00), prog + 0x1FBF00, 0x500000, prog + 0x6FBF14, 0x22);
    AT(prog, 0x6FC214, s32) = 0x2A;   /* the entry: room 0x2A (a new game) */
    g->vtbl = SceneGame_vtable;
    AT(g, SG_PROGRESS, void **) = D_0047A7E8;

    m = (u8 *)g + 0x73EE80;
    func_002D1360(m);
    func_002D1320(m + 0x340);
    func_002D12E0(m + 0x3E0);
    func_002D12C0(m + 0x4F0);
    func_002D1200(m + 0x1640);
    func_002D1160(m + 0x6740);
    func_002D1130(m + 0x9360);
    func_002D10C0(m + 0x9380);
    func_00120C80(m);

    /* the player (Fiona) and the partner (Hewie) */
    c = (Character *)((u8 *)g + SG_FIONA);
    Character_Construct(c, 0);
    c->unk153C = 0;
    c->a.vtbl = Fiona_vtable;
    c = (Character *)((u8 *)g + SG_PARTNER);
    Character_Construct(c, 1);
    gSceneGameF29740 = (u8 *)g + 0xF29740;
    c->unk153C = 1;
    c->a.vtbl = D_0046A120;
    AT(g, 0xF29740, void **) = D_0046ABB0;
    gRoutePlanner = (VObject *)((u8 *)g + 0xF6A940);
    AT(g, 0xF6A940, void **) = D_0046C520;

    o = (u8 *)g + 0xF6AFB0;
    func_002D10A0(o);
    func_002D1080(o + 0xC);
    AT(o, 0x0, void **) = D_0046B3A0;
    AT(o, 0xC, void **) = D_0046B3B8;
    func_002D1040(o + 0x20);
    func_00100340(o + 0x120, func_002D1020, func_001FB3B0, 4, 0x110);
    Task_ctor((Task *)(o + 0x708));
    func_002D1000(o + 0x938);
    func_00100340(o + 0xBF0, func_002D0FE0, func_001FB400, 0x30, 0x20);
    AT(o, 0x702, u8) = 0xFF;
    for (i = 0; i < 17; i++) {
        AT(o, 0x564 + i * 0x18, s32) = 0;
    }
    gLights = (VObject *)((u8 *)g + 0xF6C1C0);
    AT(o, 0x6FC, s32) = 0;

    o = (u8 *)g + 0xF6C1C0;
    AT(o, 0x0, void **) = D_0046B300;
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
    AT(o, 0x60, void **) = D_0046C668;
    gCamDirector = (VObject *)(o + 0x60);

    o = (u8 *)g + 0xF6CD30;
    AT(o, 0x1400, void **) = D_004699E0;
    AT(o, 0x1404, s32) = 0;
    gRoomEffects = o;
    AT(o, 0x1408, s32) = 0;
    AT(o, 0x1400, void **) = D_004699C0;
    AT(o, 0x140C, s32) = 0;
    gEffects = (u8 *)g + 0xF6E200;   /* a sub-heap (its header after its 64 KB) */
    AT(o, 0x1410, s32) = 0;
    AT(o, 0x1414, s32) = 0;
    o = (u8 *)g + 0xF6E200;
    AT(o, 0x10000, void **) = D_004699E0;
    AT(o, 0x10004, s32) = 0;
    AT(o, 0x10008, s32) = 0;
    AT(o, 0x10000, void **) = D_0046A1C0;
    AT(o, 0x1000C, s32) = 0;
    AT(o, 0x10010, s32) = 0;

    /* the sub screen (the in-game menu) */
    sub = (SubScreen *)((u8 *)g + 0xF87240);
    SubScreenBase_ctor(sub);
    sub->vtbl = D_0047A790;
    Task_ctor(&sub->ask);
    Task_ctor(&sub->text);
    TextObj_ctor(sub->textObj);
    BootCard_ctor(&sub->card);

    /* the music controller */
    gMusic = (VObject *)((u8 *)g + 0x1053424);
    AT(g, 0x1053424, void **) = D_0046A110;
    D_0045D1F0 = (u8 *)g + 0x105344C;

    o = (u8 *)g + 0x1053480;
    D_00456DE8 = o;
    AT(o, 0x0, void **) = D_00473440;
    Task_Construct((Task *)(o + 0x11048));
    return g;
}

/* destructor (vtable D_0046ED30) */
/* (possibly dead code: nothing in the game references it) */
void *func_002D0C70(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046ED30;
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

extern void *D_00476F40[], *D_00469D00[];
extern void func_003A0160(Scene *g);

/* the frame counted (twice while a button is held): it feeds the random numbers, seeded here */
static inline void rng_tick(Scene *g) {
    u8 rng[0x80] __attribute__((aligned(16)));

    AT(g, 0x1065040, s32)++;
    if ((u16)D_0047E37C != 0) {
        AT(g, 0x1065040, s32)++;
    }
    AT(rng, 0x0, void **) = D_00476F40;
    AT(rng, 0x4, s32) = -1;
    func_0033E2A0(rng, AT(g, 0x1065040, s32));
    AT(rng, 0x0, void **) = D_00469D00;
}

/* state, every frame: count the frame (twice while a button is pressed: it feeds the random
 * numbers, seeded here), then the sub-state (+0x1053450: the room load) if any, else the
 * gameplay tick */
void func_003A06E0(Scene *g) {
    rng_tick(g);
    if (ptmf_test(&AT(g, 0x1053450, PTMF))) {
        ptmf_scall(g, &AT(g, 0x1053450, PTMF));
        return;
    }
    func_003A0160(g);
}

extern void func_0039AD90(Scene *g);
extern const PTMF D_0044C7C0; /* { 0, -1, func_003A0390 } */

/* sub-state: start the room. Once the progress data is ready: reset the sub screen, the
 * members, the characters; when continuing from a save (+0xF6CD28), its entry room and the
 * sub screen's state from the system data; then request the room's file (the room buffers,
 * +0x73EE80), put the characters in the room, and wait for it (func_003A0390). Returns 1 while
 * the progress data isn't ready. */
s32 func_003A04A0(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    s32 i;

    if (func_001764C0(prog)) {
        return 1;
    }
    SubScreen_Start((SubScreen *)((u8 *)g + 0xF87240));
    func_002F39B0((u8 *)g + 0x73EB40);
    func_0031E150((u8 *)g + 0x1053480);
    if (AT(g, 0xF6CD28, u8) != 1) {
        func_001771A0(prog, 1);
        VCALL(gRooms, 0xC, void (*)(VObject *, void *))(gRooms, NULL);
    } else {
        u8 *save = (u8 *)gSystemData + 0x190;

        AT(g, SG_ENTRY, s32) = AT(gSystemData, 0x194, s32);
        func_00385030((SubScreen *)((u8 *)g + 0xF87240), save);
        VCALL(gRooms, 0xC, void (*)(VObject *, void *))(gRooms, save + 0x1010);
    }
    func_001771A0(prog, 0);
    func_001765D0(prog);
    VCALL(gObstacles, 0xC, void (*)(VObject *))(gObstacles);
    func_00209850((u8 *)g + 0xF6AFB0);
    func_0039AD90(g);
    func_00120720((u8 *)g + 0x73EE80, AT(g, SG_ENTRY, s32), AT(g, 0xF6C1B0, s32));
    func_001AABC0((u8 *)g + 0xF29740);
    for (i = 0; i < 6; i++) {
        if (gCharacters[i] != NULL) {
            AT(gCharacters[i], 0x30, s32) = AT(g, SG_ENTRY, s32);   /* their room */
        }
    }
    AT(g, 0xF6CD20, u8) = 0xFF;
    AT(g, 0xF6CD24, s32) = 0;
    func_00267250((u8 *)g + 0xF6CD30);
    func_002D6330((u8 *)g + 0xF6E200);
    AT(g, 0x1053450, PTMF) = D_0044C7C0;
    return 0;
}

/* after a room load: on a game loaded from a save (+0xF6CD28 1, ignoring bit 0x80) pick the start variant
 * (vt+0xF8) from the unlocked bonus flags in the progress (+0x1C/+0x24/+0x2C), then start
 * +0x106503C (vt+0xC); nothing on mode 0 */
void func_0039AD90(Scene *g) {
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
s32 func_003A0390(Scene *g) {
    if (func_00120660((u8 *)g + 0x73EE80, AT(g, 0xF6C1B0, s32))) {
        return 1;
    }
    if (VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) != 3) {
        return 1;
    }
    if (AT(g, 0xF6CD28, u8) == 1) {
        func_00120720((u8 *)g + 0x73EE80, AT(gSystemData, 0x198, s32),
                      (u8)(AT(g, 0xF6C1B0, s32) == 0));
    }
    func_0031E130((u8 *)g + 0x1053480);
    func_002E2820((u8 *)g + 0x706480);
    AT(g, 0x1053450, PTMF) = sGameStateNull;
    return 0;
}

extern const PTMF D_0044C7D0;   /* the scene's gameplay state */
extern const PTMF D_0044C7E0;   /* its gameplay sub-state (+0x1053440) */
extern void func_0039D310(Scene *g);

/* the room is in: the characters in it enter (+0x38), the room's resources are set up, the
 * player is reset once per scene (+0xF6CD28 bit 0x80), then on to gameplay */
void func_003A0160(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    s32 i;

    AT(g, 0x44, s32) = 0;
    AT(g, 0x73EE40, s32) = 0;
    func_00225550((u8 *)g + 0xF6CBB0);
    func_001792C0(prog, 0);
    func_0039D310(g);
    if (AT(g, 0xF6CD28, u8) == 1) {
        VObject *sub = (VObject *)((u8 *)g + 0xF87240);

        VCALL(sub, 0x2C, void (*)(VObject *, s32))(sub, AT(gSystemData, 0x1A80, s8));
    }
    for (i = 0; i < 3; i++) {
        if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) == 1) {
            VCALL(gCharacters[i], 0x38, void (*)(void *))(gCharacters[i]);
        }
    }
    func_002E2650((u8 *)g + 0x706480);
    VCALL(g, 0xDC, void (*)(Scene *))(g);
    func_00175430(prog);
    func_002252B0((u8 *)g + 0xF6CBB0, AT(g, 0x74881C, s32));
    if (!(AT(g, 0xF6CD28, u8) & 0x80)) {
        func_00124F20((Character *)((u8 *)gCharPlayer), 0xFF);
        AT(g, 0xF6CD28, u8) |= 0x80;
    }
    func_002A8410((u8 *)g + 0x16B4);
    func_002A8410((u8 *)g + 0x16D4);
    ptmf_set(&g->state, &D_0044C7D0);
    ptmf_set(&AT(g, 0x1053440, PTMF), &D_0044C7E0);
}

extern const s32 D_0044C6E0[]; /* rooms flagged at +0x1FBF00 (-1 terminated) */
extern void *func_0039B280(void *o);   /* creature constructors: slots 0..6 */
extern void *func_0039B230(void *o);   /* slots 7..9 */

#define SG_CONTROL 0x1FBF01   /* u8: 0 Fiona is controlled, else Hewie */
#define SG_ROOMFLAG 0x1FBF00

/* enter the room: sounds, the room slot, (loaded game: the save's character and creature
 * state), the controlled character's room, per-room resets, the characters' room entry
 * (Progress +0x34), the event system, and the room flag +0x1FBF00 */
void func_0039D310(Scene *g) {
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
    func_0011FFB0((u8 *)g + 0x73EE80, AT(g, 0xF6C1B0, s32));
    func_001AABC0((u8 *)g + 0xF29740);
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
        u8 *rd = gSystemData;
        u8 *save = rd + 0x190;
        Progress *gp;
        u8 *cs;
        u8 *stalker;
        u8 *cr;

        AT(gCharPlayer, 0xE8, s32) = AT(rd, 0x1A0, s32);
        AT(gCharPlayer, 0xEC, s32) = AT(rd, 0x1A4, s32);
        VCALL(gCharPlayer, 0x28, void (*)(void *, s32, void *, void *))(
            gCharPlayer, AT(rd, 0x19C, s32), save + 0x44, save + 0x30);
        func_00124F20((Character *)((u8 *)gCharPlayer), 0xFF);
        gp = gProgress;
        cs = (u8 *)gp + 0x800;
        if (AT(save, 0x1F, u8)) {
            func_001662A0((Hewie *)((u8 *)gCharPartner), (HewiePlacement *)cs);
        }
        VCALL(gCharPartner, 0x70, void (*)(void *))(gCharPartner);
        func_00165510((Hewie *)((u8 *)gCharPartner), AT(cs, 0xC, s32));
        if (gCharSlot2 != NULL) {
            if (AT(save, 0x20, u8)) {
                func_001771A0(prog, 2);
            }
            VCALL(gCharSlot2, 0x70, void (*)(void *))(gCharSlot2);
            stalker = (u8 *)gCharSlot2;
            if (AT(stalker, 0x28, u8) &&
                AT(stalker, 0x30, s32) != VCALL(g, 0xA4, s32 (*)(Scene *))(g)) {
                if (AT(gCharSlot2, 0xC4, s32) == 2 ||
                    (u8)VCALL(prog, 0x64, s32 (*)(Progress *))(prog) == 4) {
                    func_002EC470((u8 *)g + 0x7A4, 0);
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
            o = func_002E2330(0x1600, mem);
            if (o != NULL) {
                o = (i < 7) ? func_0039B280(o) : func_0039B230(o);
            }
            AT(cr, i * 4, u8 *) = o;
            AT(AT(cr, i * 4, u8 *), 0x20, s32) = (u8)i;
            if ((u8)i >= 7 && (u8)i < 10) {
                void *part = ((void *(*)(u8 *))AT(AT(cr, 0x28, u8 *), 0xC, void *))(cr);

                part = func_002DC6E0(0x890, part);
                if (part != NULL) {
                    part = func_0038C8D0(part);
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
    func_002D6100((u8 *)g + 0xF6E200);
    func_002A7B40((u8 *)g + 0x1010);
    func_00305520((u8 *)g + 0x101EBC0, AT(gCharPlayer, 0x30, s32));
    Progress_ClearFlag(prog, 0x2D);
    for (i = 0; i < 6; i++) {
        u8 *c = (u8 *)gCharacters[i];

        if (c != NULL && AT(c, 0x28, u8)) {
            func_002A8410(c + 0x14E8);
            func_002A8410(c + 0x1508);
            VCALL(prog, 0x34, void (*)(Progress *, u8))(prog, i);
        }
    }
    func_00209390((u8 *)g + 0xF6AFB0, 0);
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
s32 func_0039D2E0(Scene *g) {
    return AT(g, 0x73F240 + AT(g, 0xF6C1B0, s32) * 4, s32);
}

/* (Progress +0x68) set or clear bit (a + 1 + b) of the bits at +0x73EEEC */
void func_0039BAE0(Scene *g, s32 set, u8 a, s32 b) {
    u32 bit = (u32)(a + 1) + b;
    u32 *w = &AT(g, 0x73EEEC + (bit >> 5) * 4, u32);

    if (set) {
        *w |= 1 << (bit & 0x1F);
    } else {
        *w &= ~(1 << (bit & 0x1F));
    }
}

extern void func_0039B2D0(Scene *g);

/* (+0xDC) the start of play in a room: the room's 8 trigger areas, which of the characters in
 * the room each holds (+0x1010 + k * 6: +1 by position, +2 by the character test) */
void func_0039CDC0(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    u8 *dir = (u8 *)g + 0xF6CBB0;
    VObject *rooms, *ev;
    s32 room;
    u32 k;
    s32 i;

    func_00175DE0(prog);
    if (!((s32 (*)(u8 *))AT(AT(dir, 0x64, u8 *), 0x6C, void *))(dir)) {
        if (!(u8)Progress_TestFlag(prog, 0xF)) {
            func_002F0500((u8 *)g + 0x7F8);
        }
        func_002A7720((u8 *)g + 0xA20);
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
    func_002EC940((u8 *)g + 0x7A4);
    if (Progress_TestFlag(prog, 0x16)) {
        func_0039B2D0(g);
    }
}

extern const PTMF D_0044C7F0;
extern void *D_0045D1F0;

/* the gameplay state (each frame): the sub-state +0x1053440 */
void func_003A0060(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    PTMF *sub = &AT(g, 0x1053440, PTMF);

    if (func_00100B80(sub, &D_0044C7F0) && AT(g, 0x106503C, void *) != NULL) {
        func_002C5980(AT(g, 0x106503C, void *));
    }
    func_002E3200((BgmCtl *)((u8 *)g + 0x1053424));
    if (!(u8)Progress_TestFlag(prog, 8) && !(u8)Progress_TestFlag(prog, 0x2A)) {
        func_0021C840(D_0045D1F0, 0x32, 0);
    }
    VCALL((VObject *)gLights, 0x34, void (*)(VObject *))((VObject *)gLights);
    if (ptmf_test(sub)) {
        ptmf_scall(g, sub);
    }
}

extern const PTMF D_0044C598;   /* the scenes' callback while saving the game */
extern const PTMF D_0044C800, D_0044C810, D_0044C820, D_0044C830;   /* sub-states: menus */
extern s8 D_0047E360;           /* the pad: 0 = not connected */
extern VObject *D_00456DF0;
extern void func_0039C880(Scene *g);
extern void func_0039BB60(Scene *g);

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

extern const PTMF D_0044C588;   /* the gameplay sub-state "in play" (func_0039EAB0) */

/* the gameplay sub-state is "in play" */
s32 func_00399BF0(Scene *g) {
    return func_00100B80(&AT(g, 0x1053440, PTMF), &D_0044C588) == 0;
}

void func_00399C30(Scene *g, u8 a, u8 b) {
    AT(g, 0x105344D, u8) = a;
    AT(g, 0x105344E, u8) = b;
}

/* +0xF6CD24 bit 3 */
void func_00399C50(Scene *g, u32 on) {
    AT(g, 0xF6CD24, u32) = (AT(g, 0xF6CD24, u32) & ~8u) | (on & 1) << 3;
}

/* +0xF6CD24 bits 4..9 */
u32 func_00399C80(Scene *g) {
    return (AT(g, 0xF6CD24, u32) >> 4) & 0x3F;
}

/* the gameplay sub-state, each frame. +0x44: 0 play, 1 leaving the room (event phase 4), 2
 * waiting for the next room: once loaded the characters leave and enter, the camera restarts
 * (event phases 5, 3). In play: event phases 1 .. 3, the camera, the characters' control;
 * then the world update (rooms, effects, characters), the fade (+0x1158 += +0x1160), drawing,
 * and the menus (+0xC sub screen, +6 pause, +4 map) */
void func_0039EAB0(Scene *g) {
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
        func_00120660(rooms, AT(g, 0xF6C1B0, s32) == 0);
        if ((D_0047E37C >> 3) & 1 || D_0047E360 == 0) {
            Progress_SetFlag(prog, 6);
        }
        *flags &= ~1;
        func_001776F0(prog);
        func_00224EE0(cam);
        VCALL(g, 0xDC, void (*)(Scene *))(g);
        func_00209390((u8 *)ev, 1);
        func_00173670(prog);
        func_00173B60(prog);
        func_001739A0(prog);
        func_00175430(prog);
        func_00209390((u8 *)ev, 2);
        if (!(u8)Progress_TestFlag(prog, 0x12)) {
            if (AT(g, SG_CONTROL, u8) == 0) {
                func_00174920(prog);
            } else {
                func_00174270(prog);
            }
        }
        if (!((*flags >> 3) & 1)) {
            func_00209210(ev);
        }
        AT(g, 0x1170, u8) = func_00179170(prog, AT(g, 0x1170, u8));
        if (AT(g, 0x1170, u8) != 0xFF) {
            u8 *c = (u8 *)gCharacters[AT(g, 0x1170, u8)];

            ((void (*)(u8 *, s32, s32))AT(AT(cam, 0x64, u8 *), 0x64, void *))(
                cam, AT(c, 0xE8, s32), AT(c, 0xEC, s32));
        }
        func_00224C60(cam);
        func_00209390((u8 *)ev, 3);
        if (camdir_busy(g) && !(u8)VCALL(ev, 0xC0, s32 (*)(VObject *))(ev)) {
            *flags = (*flags & ~0x3F0) | 0x3F0;
        } else {
            *flags &= ~0x3F0;
        }
        if ((D_0047E37C & 1) && !(u8)Progress_TestFlag(prog, 5) && AT(gCharPlayer, 0xE0, u8) == 0 &&
            AT(gCharPlayer, 0x14E8, s32) != 5 && AT(g, 0x7F8, u8) < 4 &&
            AT(gCharPlayer, 0xF8, s32) == 0 && !Progress_TestFlag(prog, 8) &&
            !VCALL(ev, 0xBC, s32 (*)(VObject *))(ev)) {
            AT(g, 0xF87244, u8) = 0;
            Progress_SetFlag(prog, 4);
        }
    } else {
        if (AT(g, 0x44, s32) == 1) {
            func_00209390((u8 *)ev, 4);
            AT(g, 0x44, s32) = 2;
        }
        if (func_00120660(rooms, AT(g, 0xF6C1B0, s32) == 0) ||
            (u8)VCALL(gFileLoader, 0x38, s32 (*)(VObject *))(gFileLoader) ||
            VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) == 2) {
            func_00209390((u8 *)ev, 3);
            *flags |= 1;
        } else {
            AT(g, 0x44, s32) = 0;
            func_00209390((u8 *)ev, 5);
            for (i = 0; i < 6; i++) {
                if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) != 0) {
                    VCALL(gCharacters[i], 0x34, void (*)(void *, s32))(gCharacters[i],
                                                                      AT(g, 0xF6CD20, u8));
                    AT(gCharacters[i], 0xEC, s32) = -1;
                    AT(gCharacters[i], 0xE8, s32) = -1;
                }
            }
            func_002E26C0((u8 *)g + 0x706480, AT(g, 0xF6CD20, u8));
            AT(g, 0xF6CD20, u8) = 0xFF;
            VCALL(gDoors, 0x5C, void (*)(void *))(gDoors);
            ((void (*)(void *, s32))func_0011FF30)(rooms, AT(g, 0xF6C1B0, s32));
            AT(g, 0xF6C1B0, s32) ^= 1;
            func_0039D310(g);
            for (i = 0; i < 6; i++) {
                if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) != 0 &&
                    AT(gCharacters[i], 0xE2, u8) == 0) {
                    VCALL(gCharacters[i], 0x38, void (*)(void *))(gCharacters[i]);
                }
            }
            func_002E2650((u8 *)g + 0x706480);
            VCALL(g, 0xDC, void (*)(Scene *))(g);
            func_00175430(prog);
            func_002252B0(cam, AT(g, 0x74881C, s32));
            func_001792C0(prog, AT(g, 0x1170, u8));
            for (i = 0; i < 6; i++) {
                if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) != 0) {
                    VCALL(gCharacters[i], 0x40, void (*)(void *))(gCharacters[i]);
                }
            }
            func_002E2740((u8 *)g + 0x706480);
            func_00209390((u8 *)ev, 3);
            func_00224C20(cam);
            *flags |= 1;
        }
    }

    /* saving: the scenes get their save callback, the loader is told, and the game waits */
    if (Progress_TestFlag(prog, 8) && Progress_TestFlag(prog, 0x1C)) {
        loader = gFileLoader;
        if (VCALL(loader, 0x24, s32 (*)(VObject *))(loader) != 2) {
            scenes_to_save();
            VCALL(loader, 0x1C, void (*)(VObject *))(loader);
            AT(gSystemData, 0x4, s32) = 2;
            AT(gSystemData, 0x10, s32) = 1;
        }
        return;
    }

    func_0031E0D0((u8 *)g + 0x1053480);
    if ((*flags & 0xF) == 0) {
        func_00224C20(cam);
    }
    if ((*flags & 0xF) == 0) {
        if (((*flags >> 4) & 0x3F) == 0) {
            func_00176440(prog);
            func_001762B0(prog);
        }
        if (!Progress_TestFlag(prog, 0x17) && !camdir_busy(g)) {
            func_002E2A60((u8 *)g + 0x706480);
        }
        func_0011FEB0(rooms);
        if (!Progress_TestFlag(prog, 0x17)) {
            func_00260EC0((u8 *)g + 0xF87248);
        }
        func_002A7630((u8 *)g + 0x1004);
        func_002D75C0((u8 *)g + 0x6FC380);
        func_002671F0((u8 *)g + 0xF6CD30);
        func_002D6280((u8 *)g + 0xF6E200);
        for (i = 0; i < 6; i++) {
            if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) == 1) {
                func_001269C0(gCharacters[i]);
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
        func_002D1FD0(gAdx);
    }

    if (!(u8)Progress_TestFlag(prog, 8)) {
        func_0011FB20(rooms, AT(g, 0xF6C1B0, s32));
        if (!Progress_TestFlag(prog, 0x17) || Progress_TestFlag(prog, 0x24)) {
            func_00176160(prog);
        }
        if ((!Progress_TestFlag(prog, 0x17) || Progress_TestFlag(prog, 0x24)) && !camdir_busy(g)) {
            func_002E29A0((u8 *)g + 0x706480);
        }
        func_002D74E0((u8 *)g + 0x6FC380);
        func_0039C880(g);
        func_00267160((u8 *)g + 0xF6CD30);
        func_002D61E0((u8 *)g + 0xF6E200);
        if (!(u8)Progress_TestFlag(prog, 0xF) && !camdir_busy(g)) {
            if (!(u8)VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents)) {
                func_002EF480((u8 *)g + 0x7F8, 1.0f);
            }
            func_002F0340((u8 *)g + 0x7F8, 0);
        }
        if (AT(g, 0xF6C1A3, u8) != 0) {
            VCALL(ev, 0xB8, void (*)(VObject *, s32))(ev, AT(g, 0xF6C1A1, u8) == 6 ? 0x30 : 0x31);
        }
        Task_Draw((Task *)((u8 *)g + 0xF6B6B8));
        VCALL((u8 *)g + 0xF87240, 0x24, void (*)(void *))((u8 *)g + 0xF87240);
        func_0031DE10((u8 *)g + 0x1053480);
    } else if (Progress_TestFlag(prog, 0x28)) {
        rng_tick(g);
    }

    /* the menus */
    menu = 0;
    if (Progress_TestFlag(prog, 0xC)) {
        VObject *snd;

        VCALL(gRumble, 0x10, void (*)(VObject *))(gRumble);
        VCALL(gFileLoader, 0x1C, void (*)(VObject *))(gFileLoader);
        ptmf_set(&AT(g, 0x1053440, PTMF), &D_0044C800);
        menu = 1;
        snd = gSound;
        VCALL(snd, 0xAC, void (*)(VObject *, f32))(snd, 1.0f);
        AT(gAdx, 0x120, f32) = 1.0f;
        clamp01(&AT(gAdx, 0x120, f32));
        func_002D1FD0(gAdx);
        if (D_00456DF0 != NULL) {
            VCALL(D_00456DF0, 0x24, void (*)(VObject *, f32))(D_00456DF0, 1.0f);
        }
        if (gMovie != NULL) {
            AT(gMovie, 0x1D4, f32) = 1.0f;
            clamp01(&AT(gMovie, 0x1D4, f32));
            func_002B6340(gMovie);
        }
        VCALL(snd, 0x9C, void (*)(VObject *))(snd);
        VCALL(gMusic, 0x8, void (*)(void *, f32, s32, s32, s32))(gMusic, 1.0f, 0xFF, 0, 0);
    }
    if (!(u8)Progress_TestFlag(prog, 8) && Progress_TestFlag(prog, 6) && !menu) {
        if (camdir_busy(g) && !(u8)Progress_TestFlag(prog, 0x19)) {
            if ((*flags & 0xF) == 0 && AT(g, 0x44, s32) == 0 &&
                !VCALL(ev, 0xBC, s32 (*)(VObject *))(ev) && gMovie != NULL) {
                VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 1);
                func_002F60B0((u8 *)g + 0x73EBA0, (D_0047E360 == 0 ? 0x80 : 0) | 1);
                func_002F02F0((u8 *)g + 0x7F8);
                menu = 1;
                ptmf_set(&AT(g, 0x1053440, PTMF), &D_0044C810);
            }
        } else if (!VCALL(ev, 0xBC, s32 (*)(VObject *))(ev) && gMovie == NULL &&
                   !(u8)Progress_TestFlag(prog, 0x19)) {
            VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 1);
            func_002F60B0((u8 *)g + 0x73EBA0, D_0047E360 == 0 ? 0x80 : 0);
            func_002F02F0((u8 *)g + 0x7F8);
            menu = 1;
            ptmf_set(&AT(g, 0x1053440, PTMF), &D_0044C820);
        }
    }
    if (Progress_TestFlag(prog, 4) && !menu) {
        VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 1);
        ptmf_set(&AT(g, 0x1053440, PTMF), &D_0044C830);
    }
    Progress_ClearFlag(prog, 0xC);
    Progress_ClearFlag(prog, 6);
    Progress_ClearFlag(prog, 4);
    if (!camdir_busy(g)) {
        func_0039BB60(g);
    }
}

/* Progress +0x54: an event camera is running and the event does not hide the room's alpha
 * parts (event +0xC0) */
s32 func_0039A7C0(Scene *g) {
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
void func_0039C880(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    u8 *cur = (u8 *)g + 0x16B4;
    u8 *last = (u8 *)g + 0x16D4;
    s32 show = 0, changed, c1, c2;

    if (gCharPlayer == NULL || AT(gCharPlayer, 0x28, u8) == 0) {
        return;
    }
    if (Progress_TestFlag(prog, 0x12)) {
        func_002A8410(cur);
        func_002A8410(last);
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
            func_002A8410(last);
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
            prompt_show((u8)func_00177620(prog) == 2 ? 0x800D : 0x800A);
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
void func_0039BB60(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    u8 alert = (u8)VCALL(prog, 0x64, s32 (*)(Progress *))(prog);
    s32 stalker = 0, chasing = 0, creature = 0, near = 0;
    s32 room, sroom = -1, i;

    if (gCharSlot2 != NULL && AT(gCharSlot2, 0x28, u8) != 0) {
        stalker = 1;
        chasing = AT(gCharSlot2, 0xC4, s32) == 2;
    }
    if (alert == 0xFF && !stalker) {
        func_00177630(prog, 2);
    }
    if (chasing) {
        func_00177630(prog, 3);
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
                    if (!func_00177670(prog, 2) || creature == 1 || func_00177670(prog, 6)) {
                        CHASE_STATE(g) = 1;
                        CHASE_TIMER(g) = 0x1C2;
                    }
                }
                break;
            case 1:
                if (func_00177670(prog, 6) || Progress_TestFlag(prog, 0x22)) {
                    CHASE_TIMER(g) += 0x96;
                    if (CHASE_TIMER(g) > 0x1C2) {
                        CHASE_TIMER(g) = 0x1C2;
                    }
                }
                if (func_00177670(prog, 2) && !creature) {
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
                if (func_00177670(prog, 2)) {
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
    i = (u8)func_00177670(prog, 1);
    AT(g, 0x54, s32) = 0;
    if (i && CHASE_STATE(g) == 0) {
        func_00177630(prog, 6);
    }
}

/* set the state (a, b) of every door with a side in room `room` (the rooms' doors, +0x6C side
 * 0 / 1, up to 400, ending at -1) */
void func_0039C5C0(Scene *g, s32 room, s32 a, s32 b) {
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
            func_001780C0(prog, d & 0xFFFF, a, b);
        }
    }
}

/* get the room behind exit `exit` of the current one ready: loaded into the other room slot
 * (+0x73F240, current slot +0xF6C1B0) unless it is there already */
void func_0039D070(Scene *g, s32 exit) {
    s32 room = VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, s32))(
        gRooms, AT(g, 0x73F240 + AT(g, 0xF6C1B0, s32) * 4, s32), exit);
    u8 other;

    if (room & 0x80000000) {
        return;
    }
    other = AT(g, 0xF6C1B0, s32) == 0;
    if (room != AT(g, 0x73F240 + other * 4, s32)) {
        func_00120720((u8 *)g + 0x73EE80, room, other);
    }
}

/* leave the current room through exit `exit`: unless a room change keeps the sound going
 * (progress flag 0x27) the sounds stop; when the other room slot doesn't already hold the room
 * behind the exit (func_0039D070) it is dropped and the room is loaded (+0xAC); then the change
 * starts (+0x44 = 1, the exit kept in +0xF6CD20 for the arrival). Returns the exit on the
 * other side (rooms +0x14): the event script's "exit taken" (event +0x702), which the next
 * room's entry script uses to place her */
u8 func_0039D120(Scene *g, u8 exit) {
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
            func_001205A0((u8 *)g + 0x73EE80, AT(g, 0xF6C1B0, s32) == 0);
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

extern void *D_00469C20[], *D_00469C60[], *D_00474080[], *D_0046FAA0[];

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

void func_0039C6C0(Scene *g, s32 room) {
    doors_of_room(g, room, func_00178450);
}

void func_0039C7A0(Scene *g, s32 room) {
    doors_of_room(g, room, func_00178500);
}

/* room `room`'s exits all closed to `kind`'s side (lock bits 4..7 of the door states: kind 0
   bit 0, 1 bit 1, 2..5 bit 2, others none) */
s32 func_0039C4C0(Scene *g, s32 room, u32 kind) {
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

    return (u8)func_001785B0(p, room, exit) != 1 && (u8)func_00178300(p, room, exit, 0) &&
           (u8)func_00178300(p, room, exit, 1) && (u8)func_00178300(p, room, exit, 2);
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
u32 func_0039C040(Scene *g, s32 *list, s32 room) {
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

/* a random room near `room` (func_0039C040, 2 or more), else -1 */
s32 func_0039C380(Scene *g, s32 room) {
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
    AT(o, 0x0, void **) = D_00469C20;
    AT(o, 0x20, s32) = 0x0FFFFFFF;
    AT(o, 0x24, s32) = 0x02000000;
    AT(o, 0x0, void **) = D_00469C60;
    AT(o, 0x1380, s32) = 0;
    AT(o, 0x153C, u8) = 0;
    AT(o, 0x0, void **) = vtbl;
    return o;
}

void *func_0039B230(void *o) {
    return creature_init(o, D_00474080);
}

void *func_0039B280(void *o) {
    return creature_init(o, D_0046FAA0);
}

/* a placed character (gCreatures) of the creature class in slot `k`, its model too for
 * slots 7..9; its memory block (NULL: none) */
static inline __attribute__((always_inline)) u8 *creature_new(u8 *cr, u32 k, s32 other) {
    u8 *mem = ((u8 *(*)(u8 *, s32))AT(AT(cr, 0x28, u8 *), 0x8, void *))(cr, 0x1600);
    u8 *o = func_002E2330(0x1600, mem);

    if (o != NULL) {
        o = other ? func_0039B230(o) : func_0039B280(o);
    }
    AT(cr, k * 4, u8 *) = o;
    AT(AT(cr, k * 4, u8 *), 0x20, s32) = k;
    if (k >= 7 && k < 10) {
        void *part = ((void *(*)(u8 *))AT(AT(cr, 0x28, u8 *), 0xC, void *))(cr);

        part = func_002DC6E0(0x890, part);
        if (part != NULL) {
            part = func_0038C8D0(part);
        }
        AT(AT(cr, k * 4, u8 *), 0xF0, void *) = part;
    }
    return mem;
}

/* place a creature (flags bit 7: of the other class, in a free slot 7..9; else at triangle
 * `tri`... -1: a free slot 0..5, otherwise slot 6) in `room` with kind `kind` and the rest;
 * out of this room: no triangle */
void func_0039AEA0(Scene *g, u32 room, s32 tri, s32 a3, u32 flags, s32 kind, s32 which, s32 a7, f32 f) {
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
        o = func_002E2330(0x1600, mem);
        if (o != NULL) {
            o = func_0039B280(o);
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
 * the current one (within reach, func_0039C040, and through one of its exits); slots 7..9 also
 * get a model */
void func_0039B2D0(Scene *g) {
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
        o = func_002E2330(0x1600, mem);
        if (o != NULL) {
            o = alt ? func_0039B230(o) : func_0039B280(o);
        }
        AT(pool, slot * 4, u8 *) = o;
        AT(AT(pool, slot * 4, u8 *), 0x20, s32) = slot;
        if (slot >= 7 && slot < 10) {
            void *mm = func_002DC6E0(0x890, VCALL_AT(pool, 0x28, 0xC, void *(*)(void *))(pool));

            if (mm != NULL) {
                mm = func_0038C8D0(mm);
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
void func_0039BAC0(Scene *g) {
    AT(g, 0x73EEE0, u8) = 1;
}

/* save the game into the resident data (gSystemData): save slot `slot`'s header (+0x70, 0x18
 * each: room, the sub screen's +0x30, a flag of +0x70, the date, +0x1004..+0x1007) and the
 * snapshot +0x190 (room, entry, Fiona's triangle / +0xE8 / +0xEC / position / heading, the six
 * characters' kinds and activity, the progress flags +0x50, the rooms' +0x1010 (13 words), the
 * sub screen's +0x18F0 and its own part (func_003851B0)), after the partner / stalker / placed
 * things / gPlacedThings save their state */
void func_0039B800(Scene *g, u32 slot) {
    u8 *rd = gSystemData;
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
    func_002A7C70((u8 *)g + 0x48, s + 0x50);
    w = VCALL(gRooms, 0x7C, const s32 *(*)(VObject *))(gRooms);
    for (i = 0; i < 13; i++) {
        AT(s, 0x1010 + i * 4, s32) = w[i];
    }
    AT(s, 0x18F0, u8) = VCALL(sub, 0x30, u8 (*)(VObject *))(sub);
    func_003851B0((SubScreen *)sub, s);
}

/* ---- the menu sub-states (+0x1053440): each draws the world as it stands (frozen) under its
 * screen and goes back to play (func_0039EAB0) when done ---- */

extern const PTMF D_0044C840, D_0044C850, D_0044C860, D_0044C870;   /* back to play */

/* the frozen world drawn (rooms, progress - `chars`: 0 unless flag 0x17, 1 with the placed
 * things unless 0x17 without 0x24, 2 both always - effects, panic, the event's message, the
 * task, the sub screen's +0x24 when `sub`) */
static void frozen_draw(Scene *g, s32 chars, s32 sub) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    VObject *ev = (VObject *)((u8 *)g + SG_EVENT);

    func_0011FB20((u8 *)g + 0x73EE80, AT(g, 0xF6C1B0, s32));
    if (chars == 2 || (chars == 1 && (!Progress_TestFlag(prog, 0x17) || Progress_TestFlag(prog, 0x24)))) {
        func_00176160(prog);
        func_002E29A0((u8 *)g + 0x706480);
    } else if (chars == 0 && !Progress_TestFlag(prog, 0x17)) {
        func_00176160(prog);
    }
    func_002D74E0((u8 *)g + 0x6FC380);
    func_0039C880(g);
    func_00267160((u8 *)g + 0xF6CD30);
    func_002D61E0((u8 *)g + 0xF6E200);
    if (!(u8)Progress_TestFlag(prog, 0xF) && !camdir_busy(g)) {
        func_002F0340((u8 *)g + 0x7F8, 1);
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
void func_0039DF10(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);

    func_00209390((u8 *)g + SG_EVENT, 3);
    if (!(u8)Progress_TestFlag(prog, 8)) {
        frozen_draw(g, 1, 1);
        func_0031DE10((u8 *)g + 0x1053480);
    } else if (Progress_TestFlag(prog, 0x28)) {
        rng_tick(g);
    }
    func_002F6050((u8 *)g + 0x73EBA0);
    if (Progress_TestFlag(prog, 6)) {
        if (Progress_TestFlag(prog, 0x1C)) {
            VObject *loader = gFileLoader;

            if (VCALL(loader, 0x24, s32 (*)(VObject *))(loader) != 2) {
                scenes_to_save();
                VCALL(loader, 0x1C, void (*)(VObject *))(loader);
                AT(gSystemData, 0x4, s32) = 2;
                AT(gSystemData, 0x10, s32) = 1;
            }
            return;
        }
        VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 0);
        to_play(g, &D_0044C860);
    }
    Progress_ClearFlag(prog, 6);
}

/* a movie paused over play: the world goes on (as in play, without the control), the pause
 * screen; closed: back to play */
void func_0039D990(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    VObject *ev = (VObject *)((u8 *)g + SG_EVENT);
    u8 *cam = (u8 *)g + SG_CAMDIR;
    u32 *flags = &AT(g, SG_FRAMEFLAGS, u32);
    s32 i;

    func_001776F0(prog);
    func_00224EE0(cam);
    VCALL(g, 0xDC, void (*)(Scene *))(g);
    func_00209390((u8 *)ev, 1);
    func_00173670(prog);
    func_00173B60(prog);
    func_001739A0(prog);
    func_00175430(prog);
    func_00209390((u8 *)ev, 2);
    func_00209210(ev);
    AT(g, 0x1170, u8) = func_00179170(prog, AT(g, 0x1170, u8));
    if (AT(g, 0x1170, u8) != 0xFF) {
        u8 *c = (u8 *)gCharacters[AT(g, 0x1170, u8)];

        ((void (*)(u8 *, s32, s32))AT(AT(cam, 0x64, u8 *), 0x64, void *))(cam, AT(c, 0xE8, s32), AT(c, 0xEC, s32));
    }
    func_00224C60(cam);
    func_00209390((u8 *)ev, 3);
    if (camdir_busy(g) && !(u8)VCALL(ev, 0xC0, s32 (*)(VObject *))(ev)) {
        *flags = (*flags & ~0x3F0) | 0x3F0;
    } else {
        *flags &= ~0x3F0;
    }
    func_00224C20(cam);
    if (((*flags >> 4) & 0x3F) == 0) {
        func_00176440(prog);
        func_001762B0(prog);
    }
    func_0011FEB0((u8 *)g + 0x73EE80);
    if (((*flags >> 4) & 0x3F) == 0) {
        func_00260EC0((u8 *)g + 0xF87248);
        func_002A7630((u8 *)g + 0x1004);
        func_002D75C0((u8 *)g + 0x6FC380);
        func_002671F0((u8 *)g + 0xF6CD30);
        func_002D6280((u8 *)g + 0xF6E200);
        for (i = 0; i < 6; i++) {
            if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) == 1) {
                func_001269C0(gCharacters[i]);
            }
        }
    }
    if (!(u8)Progress_TestFlag(prog, 8)) {
        frozen_draw(g, 0, 0);
        func_0031DE10((u8 *)g + 0x1053480);
    } else if (Progress_TestFlag(prog, 0x28)) {
        rng_tick(g);
    }
    func_002F6050((u8 *)g + 0x73EBA0);
    if (Progress_TestFlag(prog, 6)) {
        VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 0);
        func_0039BB60(g);
        to_play(g, &D_0044C870);
    }
    Progress_ClearFlag(prog, 6);
}

/* the sub screen (items, files...): Select closes it (flag 4); drawn over the frozen world
   (when +0xF88938 asks for it) or alone; closed: back to play */
void func_0039E7D0(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    u8 *cam = (u8 *)g + SG_CAMDIR;

    if (D_0047E37C & 1) {
        Progress_SetFlag(prog, 4);
    }
    func_00224EE0(cam);
    func_00224C60(cam);
    func_00224C20(cam);
    func_00209390((u8 *)g + SG_EVENT, 3);
    if (AT(g, 0xF88938, u8) != 0 && !(u8)Progress_TestFlag(prog, 8)) {
        frozen_draw(g, 2, 1);
    } else {
        VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
        VCALL((u8 *)g + 0x6FC258, 0x20, void (*)(void *))((u8 *)g + 0x6FC258);
    }
    SubScreen_Update((SubScreen *)((u8 *)g + 0xF87240));
    func_002A7630((u8 *)g + 0x1004);
    if (Progress_TestFlag(prog, 4)) {
        VCALL(gRumble, 0x2C, void (*)(VObject *, s32))(gRumble, 0);
        to_play(g, &D_0044C840);
    }
    Progress_ClearFlag(prog, 4);
}

/* the transition screen (+0x73EB40: game over / continue) running: the world drawn behind it
 * while +0x73EB41; done (flag 0xC): with +0x1FBF01 clear - its mode 2 (game over) keeps
 * Hewie's trust and two values for the continue (+0xFF4 / +0xFFE / +0x1000) - every scene
 * saves and the game goes on (resident +0x4: 5 continue, else 2 title); else the game restarts
 * in place: flag 8, the camera director released, the panic reset, door 0x10C locked, the room
 * re-entered, Hewie's action 0x37 and the stalker removed, then back to play */
void func_0039E380(Scene *g) {
    Progress *prog = (Progress *)((u8 *)g + SG_PROGRESS);
    u8 *cam = (u8 *)g + SG_CAMDIR;

    func_002F3910((GameOver *)((u8 *)g + 0x73EB40));
    if (AT(g, 0x73EB41, u8) != 0 && !Progress_TestFlag(prog, 8)) {
        func_00224EE0(cam);
        func_00224C60(cam);
        func_00224C20(cam);
        func_00209390((u8 *)g + SG_EVENT, 3);
        func_0011FB20((u8 *)g + 0x73EE80, AT(g, 0xF6C1B0, s32));
        func_00267160((u8 *)g + 0xF6CD30);
        func_002D61E0((u8 *)g + 0xF6E200);
        func_00176160(prog);
        func_002E29A0((u8 *)g + 0x706480);
        func_002F0340((u8 *)g + 0x7F8, 0);
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
        AT(gSystemData, 0x4, s32) = AT(g, 0x73EB40, u8) == 2 ? 5 : 2;
        AT(gSystemData, 0x10, s32) = 0;
        return;
    }
    VCALL(gFileLoader, 0x1C, void (*)(VObject *))(gFileLoader);
    Progress_SetFlag(prog, 8);
    AT(g, 0x1FBF01, u8) = 0;
    VCALL((VObject *)gCamDirector, 0x40, void (*)(VObject *, f32))(gCamDirector, -1.0f);
    func_002F0260((u8 *)g + 0x7F8, 0);
    func_00178A30(prog, 0x10C);
    VCALL(g, 0xE8, void (*)(Scene *, s32, s32, s32))(g, VCALL(g, 0xA4, s32 (*)(Scene *))(g), 1, 0);
    VCALL((VObject *)gCharPartner, 0x64, void (*)(void *, s32, s32, s32))(gCharPartner, 0x37, -1, 0);
    func_001773A0(prog, 2, 0);
    VCALL(g, 0xB0, void (*)(Scene *, s32))(g, 0x37);
    AT(g, 0x44, s32) = 1;
    AT(g, 0xF6B6B2, u8) = 0x81;
    to_play(g, &D_0044C850);
}

/* room `room` (>= 0) loaded into the spare room slot unless it is already there */
void func_0039D000(Scene *g, s32 room) {
    u32 spare;

    if (room & 0x80000000) {
        return;
    }
    spare = AT(g, 0xF6C1B0, s32) == 0;
    if (room != AT(g, 0x73F240 + spare * 4, s32)) {
        func_00120720((u8 *)g + 0x73EE80, room, spare);
    }
}

/* ---- small SceneGame methods left (2026-10-05) ---- */

extern const u16 D_0044C5B0[];   /* flag numbers, 0xFFFF-terminated */

/* the room data of the file slot not in use (+0xF6C1B0 says which), once loaded; NULL before */
u8 *func_0039A510(u8 *g) {
    u8 k = AT(g, 0xF6C1B0, s32) == 0;

    if (func_00120540(g + 0x73EE80, k)) {
        return NULL;
    }
    k = AT(g, 0xF6C1B0, s32) == 0;
    return g + 0x748840 + k * 0x2A0000;
}

/* start loading that slot */
void func_0039A5B0(u8 *g) {
    func_001205A0(g + 0x73EE80, AT(g, 0xF6C1B0, s32) == 0);
}

/* how many flags D_0044C5B0 lists */
s32 func_0039A5E0(void) {
    const u16 *f = D_0044C5B0;
    s32 n = 0;

    while (*f != 0xFFFF) {
        f++;
        n++;
    }
    return n;
}

/* how many of them are set in o's bits (+0x5C) */
s32 func_0039A620(u8 *o) {
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
u8 func_0039A690(u8 *g) {
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
s32 func_0039A830(u8 *g) {
    return VCALL(g, 0xA4, s32 (*)(void *))(g) == 7;
}

/* end and delete its object at +0x106503C (+0x14, then destructor +0x8) */
void func_0039A860(u8 *g) {
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
void func_00179E60(u8 *o) {
    s32 i;

    AT(o, 0x2718, s32) = 0;
    AT(o, 0x271C, s32) = 0;
    for (i = 0; i < 15; i++) {
        AT(o, 0x2720 + i * 4, s32) = 0;
    }
}

/* destructor (vtable D_0046B350) */
void *func_001FB0F0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046B350;
        gLights = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the base (D_0046DB80): destructor */
void *func_001FB3B0(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046DB80;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046BA68) */
void *func_001FB400(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046BA68;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the base's defaults: nothing (0) */
s32 func_00209200(void *o) {   /* +0x30 */
    return 0;
}

s32 func_00209830(void *o) {   /* +0xC */
    return 0;
}

/* destructor (vtable D_0046C6F0) */
void *func_00225620(u8 *o, s32 flags) {
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
void *func_0025C850(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046C770;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046D780) */
void *func_00268110(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D780;
        AT(o, 0x8, s32) = 0;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable D_0046D7D0) */
/* (possibly dead code: nothing in the game references it) */
void *func_002D00A0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046D7D0;
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

    if (func_00120540(g + 0x73EE80, AT(g, 0xF6C1B0, s32) ^ 1) != 0) {
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
    func_002E27B0(g + 0x706480);
    func_00176720((Progress *)(g + 0x40));
    func_00120980(g + 0x73EE80);
    func_001F9D20(g + 0xF6C1C0);
    func_001766D0((Progress *)(g + 0x40));
    sc = AT(gSceneTable, 4, u8 *);
    if (sc != NULL) {
        ptmf_set(&AT(sc, 4, PTMF), &D_0044C598);
        sc = AT(gSceneTable, 4, u8 *);
        VCALL(sc, 0x14, void (*)(void *))(sc);
    }
    AT(VCALL(gRenderer, 0x2C, u8 *(*)(VObject *))(gRenderer), 0x1C, u8) = 0;
    func_002E31D0((BgmCtl *)(g + 0x1053424));
    bgm = (u8 *)gAdx;
    func_002D2330((Bgm *)bgm);
    VCALL((VObject *)(g + 0x6FC380), 0x24, void (*)(VObject *))((VObject *)(g + 0x6FC380));
    ((void (*)(void *))func_00267140)(g + 0xF6CD30);
    func_0031E0B0(g + 0x1053480);
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
    func_002D1FD0((Bgm *)bgm);
    func_00179E60(D_004562B0);
    func_0017D1B0(D_004562A8);
    g[0x11] = 1;
}

/* ---- SceneGame's destructor and the member destructors it needs (2026-10-05) ---- */

extern void *D_0046A9D0[], *D_0046AA40[], *D_0046A9C0[], *D_0046F350[], *D_00469D00[], *D_0046A9B0[];
extern void *D_00473440[], *D_0046A110[], *D_0046A100[], *D_0047A790[], *D_0046A1C0[], *D_004699E0[];
extern void *D_004699C0[], *D_0046C660[], *D_0046C668[], *D_0046C6F0[], *D_0046B300[], *D_0046B350[];
extern void *D_0046B3A0[], *D_0046B3B8[], *D_0046C520[], *D_0046C530[], *D_0046ABB0[], *D_0046AC00[];
extern void *D_0046A120[], *D_00469C60[], *D_00469C20[];

/* (D_0046A9C0): its quad drawer (+0x120) and its task's child (+0x88) */
void *func_00179B10(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046A9C0;
        AT(o, 0x120, void **) = D_0046F350;
        AT(o, 0x120, void **) = D_00469D00;
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
void *func_00179AC0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x4C, void **) = D_0046A9B0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the progress object: gProgress cleared */
void *func_002D0F90(void *o, s32 flags) {
    if (o != NULL) {
        gProgress = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

void *func_002D0FE0(u8 *p) {
    F(p, 0x0, void *) = D_0046BA68;
    p[0x4] = 0;
    return p;
}

void *func_002D1000(u8 *p) {
    gCutscene = (VObject *)p;
    F(p, 0x0, void *) = D_0046ED30;
    return p;
}

void *func_002D1020(u8 *p) {
    F(p, 0x0, void *) = D_0046DB80;
    return p;
}

void *func_002D1040(u8 *p) {
    F(p, 0x0, void *) = D_00469D00;
    F(p, 0x4, s32) = -1;
    F(p, 0x0, void *) = D_0046F350;
    F(p, 0x24, u32) = 0;
    F(p, 0x10, s32) = -1;
    p[0x14] = 0;
    return p;
}

void *func_002D1080(u8 *p) {
    gEvents = (VObject *)p;
    F(p, 0x0, void *) = D_0046BAA0;
    return p;
}

void *func_002D10A0(u8 *p) {
    F(p, 0x0, void *) = D_0046BA80;
    F(p, 0x4, u32) = 0;
    p[0x8] = 0;
    return p;
}

void *func_002D1130(u8 *p) {
    D_00456E00 = p;
    F(p, 0x0, void *) = D_0046DB40;
    F(p, 0x4, u32) = 0;
    F(p, 0x8, u32) = 0;
    return p;
}

void *func_002D12C0(u8 *p) {
    gRooms = (VObject *)p;
    F(p, 0x0, void *) = D_0046C3E0;
    return p;
}

void *func_002D12E0(u8 *p) {
    s32 i;

    gNavMesh = (NavMesh *)p;
    F(p, 0x0, void *) = D_0046A9D0;
    for (i = 0x4; i <= 0x18; i += 4) {
        F(p, i, u32) = 0;
    }
    return p;
}

void *func_002D1320(u8 *p) {
    F(p, 0x0, void *) = D_00469D00;
    F(p, 0x4, s32) = -1;
    F(p, 0x0, void *) = D_0046D780;
    F(p, 0x20, u32) = 0;
    F(p, 0x24, u32) = 0;
    F(p, 0x18, s32) = -1;
    return p;
}

void *func_002D1360(u8 *p) {
    F(p, 0x0, void *) = D_00469D00;
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

void *func_002D1470(u8 *p) {
    F(p, 0x4C, void *) = D_0046A9B0;
    return p;
}

void *func_002D1490(u8 *p) {
    u8 *a = p + 0xDC40;
    u8 *b = p + 0xF630;

    gCreatures = p;
    F(p, 0x28, void *) = D_0046FC00;
    F(a, 0x0, void *) = D_004699E0;
    F(a, 0x4, u32) = 0;
    F(a, 0x8, u32) = 0;
    F(a, 0x0, void *) = D_004699C0;
    F(a, 0xC, u32) = 0;
    F(a, 0x10, u32) = 0;
    F(a, 0x14, u32) = 0;
    F(b, 0x0, void *) = D_004699E0;
    F(b, 0x4, u32) = 0;
    F(b, 0x8, u32) = 0;
    F(b, 0x0, void *) = D_004699C0;
    F(b, 0xC, u32) = 0;
    F(b, 0x10, u32) = 0;
    F(b, 0x14, u32) = 0;
    return p;
}

void *func_002D1510(u8 *p) {
    u8 *a = p + 0xA040;

    gPlacedThings = (VObject *)p;
    F(p, 0x0, void *) = D_0046F5C0;
    F(a, 0x0, void *) = D_004699E0;
    F(a, 0x4, u32) = 0;
    F(a, 0x8, u32) = 0;
    F(a, 0x0, void *) = D_004699C0;
    F(a, 0xC, u32) = 0;
    F(a, 0x10, u32) = 0;
    F(a, 0x14, u32) = 0;
    return p;
}

void *func_002D1560(u8 *p) {
    gBootMessage = (VObject *)p;
    F(p, 0x0, void *) = D_0046D7D0;
    return p;
}

void *func_002D1580(u8 *p) {
    F(p, 0x0, void *) = D_004699E0;
    F(p, 0x4, u32) = 0;
    F(p, 0x8, u32) = 0;
    F(p, 0x0, void *) = D_0046A1C0;
    F(p, 0xC, u32) = 0;
    F(p, 0x10, u32) = 0;
    return p;
}

/* Progress constructor: registers the global instance. */
void *func_002D15C0(u8 *p) {
    gProgress = (Progress *)p;
    return p;
}

void func_0031DDF0(u8 *self, u32 id) {
    self[0x11040] = (u8)id;
    S32(self, 0x11044) = 30;
}

/* (possibly dead code: nothing in the game references it) */
s32 func_0031E0B0(void *self) {
    return VCALL(gTexCache, 0x14, s32 (*)(void *, s32))(gTexCache, 0x2D);
}

/* Count down the timer set by func_0031DDF0; id 0xFF = none. */
void func_0031E0D0(u8 *self) {
    if (self[0x11040] != 0xFF) {
        S32(self, 0x11044) -= 1;
        if (S32(self, 0x11044) == 0) {
            self[0x11040] = 0xFF;
        }
    }
}

s32 func_0031E130(u8 *self) {
    return VCALL(gTexCache, 0x10, s32 (*)(void *, void *, s32))(gTexCache, self + 0x40, 0x2D);
}

/* destructor (vtable D_00476F40) */
void *func_0033D990(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00476F40;
        AT(o, 0x0, void **) = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

void *func_0038C8D0(u8 *m) {
    func_0016FCD0(m);
    AT(m, 0x0, void **) = D_00474460;
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
    func_002D0C10(g + 0x1053480, 0);
    D_0045D1F0 = NULL;
    AT(g, 0x1053424, void **) = D_0046A110;
    AT(g, 0x1053424, void **) = D_0046A100;
    gMusic = NULL;
    AT(g, 0xF87240, void **) = D_0047A790;
    BootCard_dtor((BootCard *)(g + 0x102FD00), -1);
    TextObj_dtor(g + 0x101EBC0, -1);
    Task_dtor((Task *)(g + 0x101EAA8), -1);
    Task_dtor((Task *)(g + 0x101E9A4), -1);
    SubScreenBase_dtor((SubScreen *)(g + 0xF87240), 0);
    AT(g, 0xF7E200, void **) = D_0046A1C0;
    AT(g, 0xF7E200, void **) = D_004699E0;
    gEffects = NULL;
    AT(g, 0xF6E130, void **) = D_004699C0;
    AT(g, 0xF6E130, void **) = D_004699E0;
    gRoomEffects = NULL;
    AT(g, 0xF6CC14, void **) = D_0046C660;
    AT(g, 0xF6CC10, void **) = D_0046C668;
    AT(g, 0xF6CC10, void **) = D_0046C6F0;
    gCamDirector = NULL;
    AT(g, 0xF6CBDC, s32) = 0;
    AT(g, 0xF6CBE8, s32) = 0;
    AT(g, 0xF6CC00, f32) = 6.0f;
    for (i = 0; i < 7; i++) {
        AT(g, 0xF6CBB8 + i * 4, s32) = 0;
    }
    AT(g, 0xF6C1C0, void **) = D_0046B300;
    AT(g, 0xF6C1C0, void **) = D_0046B350;
    gLights = NULL;
    AT(g, 0xF6AFB0, void **) = D_0046B3A0;
    AT(g, 0xF6AFBC, void **) = D_0046B3B8;
    func_001002C0(g + 0xF6BBA0, (void * (*)(void *, s32))func_001FB400, 0x30, 0x20);
    func_002D0C70(g + 0xF6B8E8, -1);
    Task_dtor((Task *)(g + 0xF6B6B8), -1);
    func_001002C0(g + 0xF6B0D0, func_001FB3B0, 4, 0x110);
    func_0016CC40(g + 0xF6AFD0, -1);
    func_0020C170((void **)(g + 0xF6AFBC), 0);
    func_0020C120((void **)(g + 0xF6AFB0), 0);
    AT(g, 0xF6A940, void **) = D_0046C520;
    AT(g, 0xF6A940, void **) = D_0046C530;
    gRoutePlanner = NULL;
    AT(g, 0xF29740, void **) = D_0046ABB0;
    AT(g, 0xF29740, void **) = D_0046AC00;
    gSceneGameF29740 = NULL;
    AT(g, 0xE35F80, void **) = D_0046A120;
    AT(g, 0xE35F80, void **) = D_00469C60;
    AT(g, 0xE35F80, void **) = D_00469C20;
    AT(g, 0xC88840, void **) = Fiona_vtable;
    AT(g, 0xC88840, void **) = D_00469C60;
    AT(g, 0xC88840, void **) = D_00469C20;
    func_00120980(g + 0x73EE80);
    func_0021A2E0(g + 0x748200, -1);
    func_002A8520(g + 0x7481E0, -1);
    func_002D0D60(g + 0x7455C0, -1);
    func_00221890(g + 0x7404C0, -1);
    func_0021B0F0((VObject *)(g + 0x73F370), -1);
    func_00179F60(g + 0x73F260, -1);
    func_00268110(g + 0x73F1C0, -1);
    func_0025C850(g + 0x73EE80, -1);
    AT(g, 0x40, void **) = Progress_vtable;
    func_00176780((u8 *)((Progress *)(g + 0x40)));
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
    func_00179B10(g + 0x73EBA0, -1);
    func_00179AC0(g + 0x73EB40, -1);
    func_002D0DF0(g + 0x706480, -1);
    func_002D0EE0(g + 0x6FC380, -1);
    func_002D00A0(g + 0x6FC258, -1);
    func_00168C20((Heap *)(g + 0x6FBF40), -1);
    func_0020E820(g + 0x48, -1);
    func_002D0F90(g + 0x40, 0);
    AT(g, 0x0, void **) = Scene_vtable;
    if ((s16)flags > 0) {
        func_0011F9A0(g);
    }
    return g;
}
