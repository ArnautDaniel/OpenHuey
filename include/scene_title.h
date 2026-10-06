#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

/* Mode 2 scene: the title screen, its menu (new game, load, options, extras) and the opening
 * and attract movies (0x140CE0 bytes from the scene heap, vtable SceneTitle_vtable). */
#include "common.h"
#include "music.h"
#include "game.h"
#include "ptmf.h"
#include "subscreen.h"
#include "text.h"

/* what the title leads to (SceneTitle.next) */
enum {
    TITLE_NEXT_NEW_GAME = 2,
    TITLE_NEXT_LOADED = 3,      /* a save was loaded */
    TITLE_NEXT_EXTRA_5 = 5,     /* the extras menu entries */
    TITLE_NEXT_EXTRA_6 = 6,
};

typedef struct SceneTitle {
    /* 0x000000 */ Scene base;
    /* 0x000014 */ s32 returnTo;        /* 2: come back to the menu, not the title */
    /* 0x000018 */ s32 timer;           /* frames in the current state */
    /* 0x00001C */ s32 anim;            /* the menu's idle count / the glow's phase */
    /* 0x000020 */ u8 next;             /* TITLE_NEXT_* */
    /* 0x000021 */ s8 cursor;           /* the main menu's entry */
    /* 0x000022 */ u8 movieSkipped;
    /* 0x000023 */ u8 pad23;
    /* 0x000024 */ u8 msg[0x11C];       /* the message object (vtable Message_vtable, gBootMessage) */
    /* 0x000140 */ u8 titleTex[0x51000];    /* SYSTEM\TITLE.TEX */
    /* 0x051140 */ u8 backImage[0x23800];   /* SYSTEM\TITLE_BACK.BIN, the background picture */
    /* 0x074940 */ u8 loaded;           /* the title's files are loaded */
    /* 0x074941 */ u8 pad74941[3];
    /* 0x074944 */ Task task;
    /* 0x074A48 */ u8 pad74A48[0x38];
    /* 0x074A80 */ SubScreen sub;       /* options / load */
    /* 0x11D868 */ u8 pad11D868[0x218];
    /* 0x11DA80 */ s32 unk11DA80;
    /* 0x11DA84 */ s32 texSlot[2];      /* the title textures' VRAM slots */
    /* 0x11DA8C */ u8 *tex[2];          /* and headers */
    /* 0x11DA94 */ u8 extras;           /* the game was finished: two more menu entries */
    /* 0x11DA95 */ u8 pad11DA95[0x2B];
    /* 0x11DAC0 */ u8 bgmWork[0x231E4]; /* the music stream's work buffer */
    /* 0x140CA4 */ BgmCtl bgm;
    /* 0x140CC0 */ PTMF seq;            /* the title's own sequence (SceneTitle_StateTitle runs it) */
    /* 0x140CCC */ f32 movieVolume;
    /* 0x140CD0 */ u8 demo;             /* the next attract movie (pstr_SYSTEM_PLAY_DEMO_0_SFD) */
    /* 0x140CD1 */ u8 pad140CD1[0xF];
} SceneTitle;

_Static_assert(__builtin_offsetof(SceneTitle, task) == 0x74944, "task");
_Static_assert(__builtin_offsetof(SceneTitle, sub) == 0x74A80, "sub");
_Static_assert(__builtin_offsetof(SceneTitle, bgmWork) == 0x11DAC0, "bgmWork");
_Static_assert(__builtin_offsetof(SceneTitle, seq) == 0x140CC0, "seq");
_Static_assert(sizeof(SceneTitle) == 0x140CE0, "SceneTitle size");

typedef struct SubScreen SubScreen;

/* scene_title.c */
extern void *SubScreenBase_ctor(SubScreen *w);
extern void *TextObj_ctor(u8 *o);
extern SceneTitle *SceneTitle_ctor(SceneTitle *t);   /* mode 2: opening movie, title screen, menus */
extern void *TextObj_dtor(u8 *o, s32 flags);
extern void *SubScreenBase_dtor(SubScreen *w, s32 flags);
extern void *Scene5_ctor(u8 *s);   /* mode 5: the ending */

#endif /* SCENE_TITLE_H */
