/* Mode 3 scene: gameplay. See docs/structure.md for the member layout. */
#include "common.h"
#include "game.h"
#include "progress.h"

/* Field at a byte offset, for SceneGame members whose types aren't known yet. */

/* SceneGame member offsets */
#define SG_PROGRESS 0x40        /* Progress (second base class) */
#define SG_FIONA 0xC88840       /* Fiona */
#define SG_PARTNER 0xE35F80     /* second character (Hewie?) */
#define SG_SETTING_A30 0xA30    /* f32 0..1, also given to D_0044E560 +0xAC */
#define SG_ENTRY 0x6FC254       /* = Progress +0x6FC214: how the scene was entered (Game modeParam) */
#define ENTRY_FLAG 0x40000000   /* top bits of the entry value */
#define ENTRY_NEW -1            /* -1: start from the resident save buffer */

extern const PTMF sGameStateNull;
extern const PTMF D_0044C7A0;   /* stored at +0x1053450 */
extern const PTMF D_0044C7B0;   /* next state */

extern VObject *D_0044E550;
extern VObject *D_0044E560;
extern VObject *D_0044E7A8;
extern VObject *D_0044E4B8;
extern void *D_0044E978;        /* resident data; +0x190 holds the save buffer used here */
extern void *D_0044E980;
extern void *D_004562B0;
extern void *D_004562A8;

extern void func_00179EA0(void *);
extern void func_0017D220(void *);
extern void func_00176780(Progress *);
extern void func_00120C80(void *);
extern void Characters_Register(Progress *, u32, void *character);
extern void func_002A7C70(void *src, void *dst);  /* copies saved flags into Progress */
extern void func_00171160(Progress *, u32);
extern void func_0016D350(Progress *, s32);
extern void func_00176650(Progress *);
extern void func_00176550(Progress *);
extern void func_001F9D90(void *);
extern void func_002D2370(void *, void *);
extern void func_002E34D0(void *);
extern void func_002E2890(void *, const PTMF *);
extern void func_002D1FD0(void *);

/* Fiona setups, by progress variable 0x26 (0..8), and the partner's, by variable 0x27 (0..2).
 * Probably costumes; each takes the character's player index. */
extern void func_003A1720(Progress *, u32);
extern void func_003A1860(Progress *, u32);
extern void func_003A15A0(Progress *, u32);
extern void func_003A1420(Progress *, u32);
extern void func_003A1310(Progress *, u32);
extern void func_003A1220(Progress *, u32);
extern void func_003A1190(Progress *, u32);
extern void func_003A10B0(Progress *, u32);
extern void func_003A1020(Progress *, u32);
extern void func_003A0F90(Progress *, u32);

static inline void Scene_SetState(Scene *scene, const PTMF *state) {
    ptmf_set(&scene->state, state);
}

/* Character setup helper: the character's index (+0x20) must be 0 or 1. */
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
    VObject *obj550 = D_0044E550;
    u8 *save = (u8 *)D_0044E978 + 0x190;
    void *obj980;
    VObject *obj560;
    f32 *settingDst;

    VCALL(obj550, 0xC, void (*)(VObject *, s32))(obj550, 0x1571);
    func_00179EA0(D_004562B0);
    func_0017D220(D_004562A8);
    func_00176780(prog);
    AT(game, 0xF6CD29, u8) = 0;
    func_00120C80((u8 *)game + 0x73EE80);
    AT(game, 0xF6C1B0, s32) = 0;

    if (AT(game, SG_ENTRY, s32) != ENTRY_NEW) {
        Progress *p;
        s32 flag;

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
    obj980 = D_0044E980;
    func_002D2370(obj980, (u8 *)game + 0x1030240);
    func_002E34D0((u8 *)game + 0x1053424);
    AT(game, 0x1053440, PTMF) = sGameStateNull;
    func_002E2890((u8 *)game + 0x706480, &sGameStateNull);
    AT(game, 0x105344D, u8) = 0;
    AT(game, 0x105344E, u8) = 0;
    VCALL(D_0044E7A8, 0x10, void (*)(VObject *))(D_0044E7A8);
    AT(game, 0x106503C, s32) = 0;
    VCALL(D_0044E4B8, 0xC, void (*)(VObject *))(D_0044E4B8);
    obj560 = D_0044E560;
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
extern void *gSceneGameF29740, *D_0044E580, *D_0044E4C8, *D_0044E4F8, *D_0044E4C0, *D_0044E578;
extern void *D_0044E970, *D_0045D1F0, *D_00456DE8;

extern void func_002D15C0(void *);
extern void func_002D15B0(void *);
extern void func_002D1580(void *);
extern void func_002D1560(void *);
extern void func_002D1510(void *);
extern void func_002D1490(void *);
extern void func_002D1470(void *);
extern void func_002D13B0(void *);
extern void func_00169260(void *, void *, u32, void *, s32);
extern void func_002D1360(void *);
extern void func_002D1320(void *);
extern void func_002D12E0(void *);
extern void func_002D12C0(void *);
extern void func_002D1200(void *);
extern void func_002D1160(void *);
extern void func_002D1130(void *);
extern void func_002D10C0(void *);
extern void func_002D10A0(void *);
extern void func_002D1080(void *);
extern void func_002D1040(void *);
extern void *func_002D1020(void *);
extern void *func_001FB3B0(void *, s32);
extern void func_002D1000(void *);
extern void *func_002D0FE0(void *);
extern void *func_001FB400(void *, s32);
extern void func_00100340(void *array, void *(*ctor)(void *), void *(*dtor)(void *, s32), u32 size, u32 n);
extern void *SubScreenBase_ctor(SubScreen *w);
extern void *TextObj_ctor(void *o);
extern BootCard *BootCard_ctor(BootCard *b);

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
    func_00176780((Progress *)prog);
    func_00169260(prog + 0x6FBF00, prog + 0x1FBF00, 0x500000, prog + 0x6FBF14, 0x22);
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
    D_0044E580 = (u8 *)g + 0xF6A940;
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
    D_0044E4C8 = (u8 *)g + 0xF6C1C0;
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
    D_0044E4F8 = o + 0x60;

    o = (u8 *)g + 0xF6CD30;
    AT(o, 0x1400, void **) = D_004699E0;
    AT(o, 0x1404, s32) = 0;
    D_0044E4C0 = o;
    AT(o, 0x1408, s32) = 0;
    AT(o, 0x1400, void **) = D_004699C0;
    AT(o, 0x140C, s32) = 0;
    D_0044E578 = (u8 *)g + 0xF6E200;   /* a sub-heap (its header after its 64 KB) */
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
    D_0044E970 = (u8 *)g + 0x1053424;
    AT(g, 0x1053424, void **) = D_0046A110;
    D_0045D1F0 = (u8 *)g + 0x105344C;

    o = (u8 *)g + 0x1053480;
    D_00456DE8 = o;
    AT(o, 0x0, void **) = D_00473440;
    Task_Construct((Task *)(o + 0x11048));
    return g;
}

/* vtable +0xC: count the frame (+0x73EE40), then the scene's request / state machine */
void SceneGame_Update(Scene *g, s32 arg) {
    AT(g, 0x73EE40, s32)++;
    Scene_Update(g, arg);
}
