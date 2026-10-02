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
