/* Mode 1 scene: loads the always-resident system files, then runs the boot steps
 * (pad check, memory card check and messages, logos) one after another. */
#include "common.h"
#include "scene_boot.h"

extern void *Scene_vtable[];
extern void *SceneBoot_vtable[];
extern void *D_0046D7D0[];   /* vtable of SceneBoot.msg */
extern void *D_0046A0D0[];   /* base vtable of SceneBoot.msg */
extern void *D_0046A058[];   /* vtable of SceneBoot.unkC7440 */
extern void *D_0046F350[];   /* vtable of SceneBoot.unkC75D0 */
extern void *D_00469D00[];   /* base vtable of SceneBoot.unkC75D0 */

extern const PTMF sSceneEntryState;   /* virtual: vtable +0x10 */
extern const PTMF sTaskIdleState;     /* { 0, -1, Task_StateIdle } */
extern const PTMF sGameStateNull;
extern const PTMF sSceneBootStateLoad;     /* -> SceneBoot_StateLoadSystem */
extern const PTMF sSceneBootStateSequence; /* -> SceneBoot_StateSequence */
extern const PTMF sSceneBootStateDone;     /* -> SceneBoot_StateDone */
extern const PTMF16 sSceneBootSteps[8];    /* boot steps, run in order */

extern VObject *gFileLoader;  /* +0x34 Load(name, dest), +0xC LoadAsync?(name, dest, flags) */
extern VObject *gBootMessage; /* = &SceneBoot.msg while the boot scene exists */
extern VObject *D_0044E978;   /* global object, type unknown (+0x1C/+0x20 return resident buffers) */
extern VObject *D_0044E4E8;   /* global object, type unknown (+0x10 upload(buf, n), +0x18 per frame) */

extern Task *Task_dtor(Task *task, s32 flags);
extern void func_0011F9A0(void *mem);   /* operator delete for scene memory? */
extern void func_0026BCC0(VObject *msg);
extern void func_0026BC00(VObject *msg);

static const char sErrMesTex[] = "SYSTEM\\ERRMES.TEX";
static const char sGameFixTex[] = "GAME_FIX.TEX";
static const char sGameFixGfm[] = "GAME_FIX.GFM";
static const char sLogoCri[] = "SYSTEM\\LOGO_CRI.BIN";

static inline void Scene_SetState(Scene *scene, const PTMF *state) {
    PTMF s = *state;

    if (ptmf_test(&s)) {
        scene->state = s;
    }
}

static inline void Task_Init(Task *task) {
    task->unk10 = 0;
    task->unk11 = 0;
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

    boot->unkC7440Vtbl = D_0046A058;
    Task_Init(&boot->unkC7458);
    boot->unkC7444 = -1;

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
    boot->unkC7440Vtbl = D_0046A058;
    Task_dtor(&boot->unkC7458, -1);
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
    res = D_0044E978;
    VCALL(loader, 0x34, void (*)(VObject *, const char *, void *))(
        loader, sGameFixTex, VCALL(res, 0x20, void *(*)(VObject *))(res));
    VCALL(D_0044E4E8, 0x10, void (*)(VObject *, void *, s32))(
        D_0044E4E8, VCALL(res, 0x20, void *(*)(VObject *))(res), 0x10);
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

    if (ptmf_test(&steps[boot->step]) && (u8)ptmf_scall_r(boot, &steps[boot->step]) == 0) {
        boot->stepFlag = 0;
        boot->stepTimer = 0;
        boot->step++;
    }
    VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
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
    ((s32 *)D_0044E978)[1] = 2;
    ((s32 *)D_0044E978)[4] = 2;
    VCALL(boot, 0x14, void (*)(SceneBoot *))(boot);
}
