#ifndef SCENE_BOOT_H
#define SCENE_BOOT_H

#include "common.h"
#include "game.h"
#include "task.h"


/* The system data kept on the memory card (0x50 bytes, the options). */
typedef struct SysData {
    /* 0x00 */ u32 sum;        /* of bytes 0x04..0x4F */
    /* 0x04 */ f32 f[6];
    /* 0x1C */ s8 b[48];
    /* 0x4C */ u32 flags;      /* 0x80: not valid */
} SysData;
_Static_assert(sizeof(SysData) == 0x50, "SysData size");

/* The boot memory card check (SceneBoot +0xC7440, vtable D_0046A058): reads the system data
 * from the card in slot 1 or 2, telling the player when there's none. */
typedef struct BootCard {
    /* 0x000 */ void **vtbl;
    /* 0x004 */ s32 state;      /* -1 done */
    /* 0x008 */ s32 unk8;
    /* 0x00C */ s32 port;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ s32 timer;
    /* 0x018 */ Task task;
    /* 0x11C */ SysData *sys;
    /* 0x120 */ s32 unk120;
    /* 0x124 */ s32 unk124;
    /* 0x128 */ u8 pad128[0xC];
    /* 0x134 */ SysData saved;  /* restored when reading fails */
    /* 0x184 */ u8 pad184[0xC];
} BootCard;
_Static_assert(__builtin_offsetof(BootCard, saved) == 0x134, "BootCard.saved");
_Static_assert(sizeof(BootCard) == 0x190, "BootCard size");

/* Mode 1 scene: memory card check, pad check, logos (0xC7700 bytes from the scene heap). */
typedef struct SceneBoot {
    /* 0x00000 */ Scene base;
    /* 0x00014 */ u32 countdown;  /* progressive-scan confirmation, frames (300 = 10 s) */
    /* 0x00018 */ s32 step;       /* index into the boot step table (0..8) */
    /* 0x0001C */ s32 stepTimer;  /* reset when a step completes */
    /* 0x00020 */ s32 stepFlag;   /* reset when a step completes */
    /* 0x00024 */ u8 savedVideoMode; /* restored if 480p isn't confirmed */
    /* 0x00025 */ u8 pad25[3];
    /* 0x00028 */ VObject msg;    /* message/error display object (gBootMessage points here) */
    /* 0x0002C */ u8 msgBody[0x130 - 0x2C];
    /* 0x00130 */ Task tasks[3];
    /* 0x0043C */ u32 pad43C;
    /* 0x00440 */ u8 errMesTex[0x21440 - 0x440];   /* SYSTEM\ERRMES.TEX */
    /* 0x21440 */ u8 logoCri[0xC7440 - 0x21440];   /* SYSTEM\LOGO_CRI.BIN */
    /* 0xC7440 */ BootCard card;
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
_Static_assert(__builtin_offsetof(SceneBoot, card) == 0xC7440, "card");
_Static_assert(__builtin_offsetof(SceneBoot, unkC75F4) == 0xC75F4, "unkC75F4");
_Static_assert(sizeof(SceneBoot) == 0xC7700, "SceneBoot size");

#endif /* SCENE_BOOT_H */
