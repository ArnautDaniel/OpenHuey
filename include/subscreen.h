#ifndef SUBSCREEN_H
#define SUBSCREEN_H

/* The sub screen: the menu screens over the title and in game - options, load / save, the
 * in-game menu pages and the extras galleries (vtable D_0047A790; code 0x384C00..0x3A0000).
 * One object, at SceneTitle +0x74A80 and in SceneGame. See src/game/subscreen.c. */
#include "common.h"
#include "ptmf.h"
#include "scene_boot.h"
#include "task.h"

/* what the screen opens as (SubScreen.mode, set by the owner before it opens) */
enum {
    SUB_MODE_GAME_MENU = 0,   /* the in-game menu (items, map, files, options pages) */
    SUB_MODE_SAVE = 1,
    SUB_MODE_PLATES = 2,
    SUB_MODE_SAVE_3 = 3,      /* the save-slot screens of SYN_SLOT.TEX (3 / 4) */
    SUB_MODE_SAVE_4 = 4,
    SUB_MODE_OPTIONS = 5,     /* from the title */
    SUB_MODE_LOAD = 6,        /* from the title */
    SUB_MODE_EXTRAS_7 = 7,
    SUB_MODE_GALLERY_MOVIE = 8,
    SUB_MODE_SAVE_9 = 9,
    SUB_MODE_EXTRAS = 10,
    SUB_MODE_GALLERY_MODEL = 11,
    SUB_MODE_EXTRAS_12 = 12,
    SUB_MODE_GALLERY_MUSIC = 13,
    SUB_MODE_GALLERY_ART = 14,
    SUB_MODE_GALLERY_TYPE = 15,
};

typedef struct SubScreen {
    /* 0x00000 */ void **vtbl;          /* +0x28 take over (SubScreen_TakeOver), +0x34 controller layout */
    /* 0x00004 */ u8 mode;              /* SUB_MODE_* */
    /* 0x00005 */ u8 pad5[3];
    /* 0x00008 */ u8 pool[0x15F0];      /* the entries' pool (SubPool_Reset) */
    /* 0x015F8 */ u16 unk15F8[0x80];
    /* 0x016F8 */ u8 showBehind;        /* the owner keeps drawing its own screen (false once
                                           the fade to black is done) */
    /* 0x016F9 */ u8 pad16F9[3];
    /* 0x016FC */ PTMF draw;            /* the fades and the running screen (SubScreen_Update) */
    /* 0x01708 */ PTMF state;           /* the page / editor running */
    /* 0x01714 */ PTMF resume;          /* the in-game menu page to reopen on */
    /* 0x01720 */ u8 pad1720[0x20];
    /* 0x01740 */ u8 baseTex[0x10800];  /* SUBSCR\SUBBASE.TEX (VRAM group 0x18) */
    /* 0x11F40 */ u8 pageTex[0x31000];  /* SUBSCR\SUBBACK.TEX or the page's (group 0x19) */
    /* 0x42F40 */ u8 saveBuf0[0x800];   /* the save screens' work (BootCard.buf0 / buf1) */
    /* 0x43740 */ u8 saveBuf1[0x54000];
    /* 0x97740 */ s32 unk97740[9];
    /* 0x97764 */ Task ask;             /* questions (restore defaults?) */
    /* 0x97868 */ Task text;            /* the screen's text */
    /* 0x9796C */ u8 pad9796C[0x14];
    /* 0x97980 */ u8 textObj[0x11140];  /* a text object (TextObj_ctor) */
    /* 0xA8AC0 */ BootCard card;        /* the load / save screens */
    /* 0xA8C44 */ s8 opt[7];            /* the options being edited (system data +0x30): 0 sound
                                           output, 2 / 3 screen x / y, 4 vibration, 6 layout */
    /* 0xA8C4B */ u8 padA8C4B;
    /* 0xA8C4C */ f32 optVolume;
    /* 0xA8C50 */ u8 unkA8C50;
    /* 0xA8C51 */ u8 unkA8C51[3];
    /* 0xA8C54 */ u8 padA8C54;
    /* 0xA8C55 */ u8 unkA8C55;
    /* 0xA8C56 */ u8 optCursor;         /* the options entry (0..4) */
    /* 0xA8C57 */ u8 unkA8C57;
    /* 0xA8C58 */ u8 unkA8C58[8];
    /* 0xA8C60 */ u8 unkA8C60;
    /* 0xA8C61 */ u8 fading;            /* a fade is running: input ignored */
    /* 0xA8C62 */ s16 fade;             /* 0..0x80 */
    /* 0xA8C64 */ s16 fadeStep;
    /* 0xA8C66 */ u8 kind;              /* which panels (D_0044C120); 0x80.. pages over black */
    /* 0xA8C67 */ u8 resumeKind;
    /* 0xA8C68 */ u8 quietClose;        /* no sound when closing */
    /* 0xA8C69 */ u8 padA8C69[0x17];
    /* 0xA8C80 */ u8 page[0x15D];       /* the page's own work (a 0x15C object for modes 3 / 4,
                                           the extras list, the galleries' cursors) */
    /* 0xA8DDD */ u8 tab;               /* the in-game tab (parts 0xC / 0xB): 2 sliding in, 3
                                           shown, 4 sliding out (page[0x15C] the slide) */
    /* 0xA8DDE */ u8 unkA8DDE;
    /* 0xA8DDF */ u8 padA8DDF;
    /* 0xA8DE0 */ s32 frame;            /* frames since it opened (blinking) */
    /* 0xA8DE4 */ u8 close;             /* the page asks to close */
    /* 0xA8DE5 */ u8 open;              /* the screen is up (cleared once faded back) */
    /* 0xA8DE6 */ u8 padA8DE6[2];
} SubScreen;

_Static_assert(__builtin_offsetof(SubScreen, ask) == 0x97764, "ask");
_Static_assert(__builtin_offsetof(SubScreen, card) == 0xA8AC0, "card");
_Static_assert(__builtin_offsetof(SubScreen, opt) == 0xA8C44, "opt");
_Static_assert(__builtin_offsetof(SubScreen, page) == 0xA8C80, "page");
_Static_assert(__builtin_offsetof(SubScreen, open) == 0xA8DE5, "open");

/* the page's work, by offset (its layout depends on the page) */
#define SUB_PAGE(s, off, type) AT((s)->page, off, type)

s32 SubScreen_Update(SubScreen *s);   /* per frame; returns showBehind */

#endif /* SUBSCREEN_H */
