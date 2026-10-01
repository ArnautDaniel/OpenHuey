#ifndef SCENE_BOOT_H
#define SCENE_BOOT_H

#include "common.h"
#include "game.h"

/*
 * Task: small object with its own state machine, embedded in scenes (0x104 bytes).
 * Only the fields the boot scene touches are known.
 */
typedef struct Task {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ PTMF state;      /* initially sTaskIdleState (an empty function) */
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ s16 id;          /* -1 = none */
    /* 0x14 */ u8 pad14[0x7C - 0x14];
    /* 0x7C */ struct Task *child; /* owned, deleted with the task */
    /* 0x80 */ u8 pad80[0x104 - 0x80];
} Task;
_Static_assert(sizeof(Task) == 0x104, "Task size");

/* Mode 1 scene: memory card check, pad check, logos (0xC7700 bytes from the scene heap). */
typedef struct SceneBoot {
    /* 0x00000 */ Scene base;
    /* 0x00014 */ u32 pad14;
    /* 0x00018 */ s32 step;       /* index into the boot step table (0..8) */
    /* 0x0001C */ s32 stepTimer;  /* reset when a step completes */
    /* 0x00020 */ s32 stepFlag;   /* reset when a step completes */
    /* 0x00024 */ u32 pad24;
    /* 0x00028 */ VObject msg;    /* message/error display object (gBootMessage points here) */
    /* 0x0002C */ u8 msgBody[0x130 - 0x2C];
    /* 0x00130 */ Task tasks[3];
    /* 0x0043C */ u32 pad43C;
    /* 0x00440 */ u8 errMesTex[0x21440 - 0x440];   /* SYSTEM\ERRMES.TEX */
    /* 0x21440 */ u8 logoCri[0xC7440 - 0x21440];   /* SYSTEM\LOGO_CRI.BIN */
    /* 0xC7440 */ void **unkC7440Vtbl;
    /* 0xC7444 */ s32 unkC7444;                    /* -1 */
    /* 0xC7448 */ u8 padC7448[0x10];
    /* 0xC7458 */ Task unkC7458;
    /* 0xC755C */ u8 padC755C[0xC75D0 - 0xC755C];
    /* 0xC75D0 */ void **unkC75D0Vtbl;
    /* 0xC75D4 */ s32 unkC75D4;                    /* -1 */
    /* 0xC75D8 */ u8 padC75D8[8];
    /* 0xC75E0 */ s32 unkC75E0;                    /* -1 */
    /* 0xC75E4 */ u8 unkC75E4;
    /* 0xC75E5 */ u8 padC75E5[0xC75F4 - 0xC75E5];
    /* 0xC75F4 */ s32 unkC75F4;
    /* 0xC75F8 */ u8 padC75F8[0xC7700 - 0xC75F8];
} SceneBoot;
_Static_assert(__builtin_offsetof(SceneBoot, msg) == 0x28, "msg");
_Static_assert(__builtin_offsetof(SceneBoot, tasks) == 0x130, "tasks");
_Static_assert(__builtin_offsetof(SceneBoot, errMesTex) == 0x440, "errMesTex");
_Static_assert(__builtin_offsetof(SceneBoot, logoCri) == 0x21440, "logoCri");
_Static_assert(__builtin_offsetof(SceneBoot, unkC7458) == 0xC7458, "unkC7458");
_Static_assert(__builtin_offsetof(SceneBoot, unkC75F4) == 0xC75F4, "unkC75F4");
_Static_assert(sizeof(SceneBoot) == 0xC7700, "SceneBoot size");

#endif /* SCENE_BOOT_H */
