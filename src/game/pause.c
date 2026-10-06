/* The pause screen (SceneGame +0x73EBA0): opened with Start in play (mode 0) or during a movie
 * (mode 1; bit 0x80 when the system has a single-line message instead, +0x220). It dims the
 * screen (+0x120, a rectangle), turns the sound and music down while open, and offers "back
 * to the game" / "quit", the latter confirmed; a movie can be skipped from it.
 *
 *   +0x4    how far open (0..1; fades by 0.2 a frame)
 *   +0x8    the choice
 *   +0xC    its message task
 *   +0x110  the state (a member function)
 *   +0x220  the mode
 *   +0x224  the choice's highlight (renderer +0x80: x, y, w, h, ..., colour +0x244) */
#include "common.h"
#include "game.h"
#include "input.h"
#include "progress.h"
#include "ptmf.h"
#include "text.h"
#include "globals.h"
#include "music.h"
#include "movie.h"
#include "renderer.h"
#include "pause.h"

extern VObject *gStageMusic;   /* the music director */

/* the states */
extern const PTMF Pause_StateOpening_ptmf, MoviePause_StateOpening_ptmf, Pause_StateOpen_ptmf, Pause_StateClosing_ptmf, Pause_StateMenu_ptmf, Pause_StateClosing_ptmf2, Pause_StateMenuClosing_ptmf,
    Pause_StateQuitAsk_ptmf, Pause_StateQuit_ptmf, Pause_StateOpen_ptmf2, Pause_StateOpen_ptmf3, MoviePause_StateOpen_ptmf, MoviePause_StateSkip_ptmf, MoviePause_StateClosing_ptmf, MoviePause_StateClosing_ptmf2;

#define T(o) AT(o, 0x4, f32)
#define CHOICE(o) AT(o, 0x8, u8)
#define TASK(o) ((Task *)((o) + 0xC))
#define STATE(o) AT(o, 0x110, PTMF)
#define MODE(o) AT(o, 0x220, u8)
#define MESSAGE_ONLY(o) (MODE(o) & 0xF0)

/* sound effect `id` (driver +0x14) */
static void sound(s32 id) {
    VCALL(gSound, 0x14, void (*)(VObject *, s32, s32))(gSound, id, 5);
}

static f32 clamp01(f32 *v) {
    if (*v < 0.0f) {
        *v = 0.0f;
    }
    if (!(*v <= 1.0f)) {
        *v = 1.0f;
    }
    return *v;
}

/* the movie's sound-effects channel paused (1) or going again (0) */
static void movie_pause(s32 on) {
    if (gMovie != NULL) {
        VObject *sfd = AT(gMovie, 0x14, VObject *);

        VCALL(sfd, 0x28, void (*)(VObject *, s32))(sfd, on);
    }
}

/* the sound turned down as far as the screen is open (`down`: 0..1): the driver's master
 * (0..255), the mix, the music (+0x44; +0x48 back to normal when `normal`) and the movie */
static void duck(f32 down, s32 normal) {
    f32 v = (0.0f + 100.0f - 100.0f * down) / 100.0f;

    VCALL(gSound, 0x94, void (*)(VObject *, u8))(gSound, (u8)(u32)(0.0f + 255.0f - 255.0f * down));
    AT(gAdx, 0x114, f32) = v;
    clamp01(&AT(gAdx, 0x114, f32));
    Bgm_ApplyVolume(gAdx);
    if (gStageMusic != NULL) {
        if (!normal) {
            VCALL(gStageMusic, 0x44, void (*)(VObject *, f32))(gStageMusic, v);
        } else {
            VCALL(gStageMusic, 0x48, void (*)(VObject *))(gStageMusic);
        }
    }
    if (gMovie != NULL) {
        AT(gMovie, 0x1CC, f32) = v;
        clamp01(&AT(gMovie, 0x1CC, f32));
        Movie_ApplyVolume(gMovie);
    }
}

/* the dimming rectangle, alpha `a`, drawn */
static void dim(u8 *o, f32 a) {
    Overlay_SetColor(o + 0x120, ((u32)a << 24) & 0xFF000000);
}

static void dim_draw(u8 *o) {
    VCALL(gRenderer, 0xC, void (*)(VObject *, u8 *, s32, s32))(gRenderer, o + 0x120, 0x33, 0);
}

/* message `id` centred on x 0x100 at y, alpha a */
static void text(u8 *o, s32 id, s32 y, s32 a) {
    u32 w = Task_MessageWidth(TASK(o), id, 0x10);

    Task_ShowText(TASK(o), 0x100 - (w >> 1), y, 0, Task_MessageText(TASK(o), id), a, 0x33, 0x10, 0x15);
}

/* the highlight round a text `w` wide (y `y`), alpha a */
static void highlight_w(u8 *o, u32 w, s32 y, f32 a) {
    AT(o, 0x224, s32) = 0x100 - w;
    AT(o, 0x228, s32) = y;
    AT(o, 0x22C, s32) = w * 2;
    AT(o, 0x230, s32) = 0x20;
    AT(o, 0x244, u32) = (AT(o, 0x244, u32) & 0xFFFFFF) | (((u32)a << 24) & 0xFF000000);
    VCALL(gRenderer, 0x80, void (*)(VObject *, u8 *))(gRenderer, o + 0x224);
}

/* ... round message `id` */
static void highlight(u8 *o, s32 id, s32 y, f32 a) {
    highlight_w(o, Task_MessageWidth(TASK(o), id, 0x10), y, a);
}

/* the movie pause's texts (the system's message instead in message mode) */
static void movie_texts(u8 *o) {
    if (!MESSAGE_ONLY(o)) {
        Task_ShowText(TASK(o), 0x140, 0x190, 0, Task_MessageText(TASK(o), 0x8089), (s32)(128.0f * T(o)), 0x33, 0x10,
                      0x15);
        Task_ShowText(TASK(o), 0x140, 0x1A4, 0, Task_MessageText(TASK(o), 0x808A), (s32)(128.0f * T(o)), 0x33, 0x10,
                      0x15);
    } else {
        Task_ShowMessage(TASK(o), 0x4008, 0, (s32)(128.0f * T(o)), 0x33);
    }
}

/* the pause's "PAUSE" (message 0x8088) with its highlight (or the system's message) */
static void pause_text(u8 *o) {
    u32 w;

    if (MESSAGE_ONLY(o)) {
        Task_ShowMessage(TASK(o), 0x4008, 0, (s32)(128.0f * T(o)), 0x33);
        return;
    }
    w = Task_MessageWidth(TASK(o), 0x8088, 0x10);
    highlight_w(o, w, 0xD2, 128.0f * T(o));
    Task_ShowText(TASK(o), 0x100 - (w >> 1), 0xD6, 0, Task_MessageText(TASK(o), 0x8088), (s32)(128.0f * T(o)), 0x33,
                  0x10, 0x15);
}

/* the menu: "back" / "quit" (0x8081 / 0x8082) */
static void menu_texts(u8 *o) {
    highlight(o, CHOICE(o) != 0 ? 0x8082 : 0x8081, 0xC4 + (CHOICE(o) << 5), 128.0f * T(o));
    text(o, 0x8081, 0xC8, (s32)(128.0f * T(o)));
    text(o, 0x8082, 0xE8, (s32)(128.0f * T(o)));
}

/* the confirmation: "quit?" (0x8083) "yes" / "no" (0x8084 / 0x8085) at alpha `a` */
static void confirm_texts(u8 *o, f32 a) {
    text(o, 0x8083, 0xC8, (s32)a);
    highlight(o, CHOICE(o) != 0 ? 0x8085 : 0x8084, 0xE4 + CHOICE(o) * 0x14, a);
    text(o, 0x8084, 0xE8, (s32)a);
    text(o, 0x8085, 0xFC, (s32)a);
}

/* up / down picks the first / second choice (a sound when it changes) */
static void choose(u8 *o) {
    u8 was = CHOICE(o);

    if (gMenuPressed & MENU_UP) {
        CHOICE(o) = 0;
    }
    if (gMenuPressed & MENU_DOWN) {
        CHOICE(o) = 1;
    }
    if (was != CHOICE(o)) {
        sound(0x2A);
    }
}

/* ---- the movie pause ---- */

/* opening */
/* 0x002F4330 */
void MoviePause_StateOpening(u8 *o) {
    T(o) += 0x1.99999a0000000p-3f /* 0.2 */;
    if (!(T(o) <= 1.0f)) {
        T(o) = 1.0f;
        if (gMovie == NULL || VCALL(gEvents, 0x54, s32 (*)(VObject *))(gEvents) == 0) {
            STATE(o) = MoviePause_StateOpen_ptmf;
        }
    }
    duck(T(o), 0);
    dim(o, 63.0f * T(o));
    dim_draw(o);
    movie_texts(o);
}

/* open: Cancel skips the movie; Start (or Confirm in message mode) goes back to it */
/* 0x002F4090 */
void MoviePause_StateOpen(u8 *o) {
    dim_draw(o);
    movie_texts(o);
    if (!MESSAGE_ONLY(o)) {
        if (gMenuPressed & MENU_CANCEL) {
            T(o) = 0.0f;
            STATE(o) = MoviePause_StateSkip_ptmf;
            sound(0x97);
        } else if (gPadPressed & PAD_START) {
            movie_pause(0);
            STATE(o) = MoviePause_StateClosing_ptmf;
            sound(0x97);
        }
    } else if (gMenuPressed & MENU_CONFIRM) {
        movie_pause(0);
        STATE(o) = MoviePause_StateClosing_ptmf2;
        sound(0x97);
    }
}

/* skipping: darkening to near black; then progress flag 0x1A */
/* 0x002F39E0 */
void MoviePause_StateSkip(u8 *o) {
    T(o) += 0x1.99999a0000000p-3f /* 0.2 */;
    if (!(T(o) <= 1.0f)) {
        T(o) = 1.0f;
        Progress_SetFlag(gProgress, 0x1A);
    }
    duck(1.0f - T(o), 1.0f - T(o) == 0.0f);   /* (the sound comes back as the skip goes on) */
    dim(o, 0.0f + 63.0f + 192.0f * T(o));
    dim_draw(o);
    Task_ShowText(TASK(o), 0x140, 0x190, 0, Task_MessageText(TASK(o), 0x8089), (s32)(128.0f * (1.0f - T(o))), 0x33,
                  0x10, 0x15);
    Task_ShowText(TASK(o), 0x140, 0x1A4, 0, Task_MessageText(TASK(o), 0x808A), (s32)(128.0f * (1.0f - T(o))), 0x33,
                  0x10, 0x15);
}

/* closing (back to the movie, or the game): then progress flag 6 */
/* 0x002F3D30 */
void MoviePause_StateClosing(u8 *o) {
    T(o) -= 0x1.99999a0000000p-3f /* 0.2 */;
    if (T(o) <= 0.0f) {
        T(o) = 0.0f;
        Progress_SetFlag(gProgress, 6);
    }
    duck(T(o), T(o) == 0.0f);
    dim(o, 63.0f * T(o));
    dim_draw(o);
    movie_texts(o);
}

/* ---- the pause ---- */

/* opening */
/* 0x002F5C80 */
void Pause_StateOpening(u8 *o) {
    T(o) += 0x1.99999a0000000p-3f /* 0.2 */;
    if (!(T(o) <= 1.0f)) {
        T(o) = 1.0f;
        STATE(o) = Pause_StateOpen_ptmf;
    }
    duck(T(o), 0);
    dim(o, 63.0f * T(o));
    dim_draw(o);
    pause_text(o);
}

/* open: Start closes it, Select opens the menu (message mode: Confirm closes it) */
/* 0x002F59A0 */
void Pause_StateOpen(u8 *o) {
    if (!MESSAGE_ONLY(o)) {
        if (gPadPressed & PAD_START) {
            STATE(o) = Pause_StateClosing_ptmf;
            sound(0x97);
        } else if (gPadPressed & PAD_SELECT) {
            CHOICE(o) = 0;
            STATE(o) = Pause_StateMenu_ptmf;
            sound(0x96);
        }
    } else if (gMenuPressed & MENU_CONFIRM) {
        STATE(o) = Pause_StateClosing_ptmf2;
        sound(0x97);
    }
    dim_draw(o);
    pause_text(o);
}

/* closing: then progress flag 6 */
/* 0x002F55C0 */
void Pause_StateClosing(u8 *o) {
    T(o) -= 0x1.99999a0000000p-3f /* 0.2 */;
    if (T(o) <= 0.0f) {
        T(o) = 0.0f;
        Progress_SetFlag(gProgress, 6);
    }
    duck(T(o), T(o) == 0.0f);
    dim(o, 63.0f * T(o));
    dim_draw(o);
    pause_text(o);
}

/* the menu: "back" closes, "quit" asks */
/* 0x002F5280 */
void Pause_StateMenu(u8 *o) {
    choose(o);
    dim_draw(o);
    menu_texts(o);
    if (gMenuPressed & MENU_CONFIRM) {
        if (CHOICE(o) == 0) {
            STATE(o) = Pause_StateMenuClosing_ptmf;
            sound(0x97);
        } else {
            CHOICE(o) = 1;
            STATE(o) = Pause_StateQuitAsk_ptmf;
            sound(0x96);
        }
    }
}

/* closing from the menu: then progress flag 6 */
/* 0x002F4E40 */
void Pause_StateMenuClosing(u8 *o) {
    T(o) -= 0x1.99999a0000000p-3f /* 0.2 */;
    if (T(o) <= 0.0f) {
        T(o) = 0.0f;
        Progress_SetFlag(gProgress, 6);
    }
    duck(T(o), T(o) == 0.0f);
    dim(o, 63.0f * T(o));
    dim_draw(o);
    menu_texts(o);
}

/* "quit?": yes quits, no (or Cancel) goes back to the pause */
/* 0x002F4A30 */
void Pause_StateQuitAsk(u8 *o) {
    choose(o);
    dim_draw(o);
    confirm_texts(o, 128.0f * T(o));
    if (gMenuPressed & MENU_CONFIRM) {
        if (CHOICE(o) == 0) {
            T(o) = 0.0f;
            STATE(o) = Pause_StateQuit_ptmf;
            sound(0x97);
        } else {
            STATE(o) = Pause_StateOpen_ptmf2;
            sound(0x96);
        }
    } else if (gMenuPressed & MENU_CANCEL) {
        STATE(o) = Pause_StateOpen_ptmf3;
        sound(0x97);
    }
}

/* quitting: darkening to near black; then progress flags 6 and 0x1C (to the title) */
/* 0x002F46B0 */
void Pause_StateQuit(u8 *o) {
    T(o) += 0x1.99999a0000000p-3f /* 0.2 */;
    if (!(T(o) <= 1.0f)) {
        T(o) = 1.0f;
        Progress_SetFlag(gProgress, 6);
        Progress_SetFlag(gProgress, 0x1C);
    }
    dim(o, 0.0f + 63.0f + 192.0f * T(o));
    dim_draw(o);
    confirm_texts(o, 128.0f * (1.0f - T(o)));
}

/* open in mode `mode`: the movie's sound effects paused in mode 1 */
/* 0x002F60B0 */
void Pause_Open(u8 *o, u8 mode) {
    T(o) = 0.0f;
    MODE(o) = mode;
    switch (MODE(o) & 0x0F) {
    case 0:
        STATE(o) = Pause_StateOpening_ptmf;
        break;
    case 1:
        movie_pause(1);
        STATE(o) = MoviePause_StateOpening_ptmf;
        break;
    }
    sound(0x96);
    AT(o, 0x224, s32) = 0;
    AT(o, 0x228, s32) = 0;
    AT(o, 0x22C, s32) = 0;
    AT(o, 0x230, s32) = 0;
    AT(o, 0x234, s32) = 0x30;
    AT(o, 0x238, s32) = 0x20;
    AT(o, 0x23C, s32) = 0x20;
    AT(o, 0x240, s32) = 0x20;
    AT(o, 0x244, u32) = 0x80808080;
    AT(o, 0x248, s32) = 2;
    AT(o, 0x24C, s32) = 0x10;
    AT(o, 0x250, s32) = 0x30;
    AT(o, 0x254, s32) = 5;
}

/* each frame open: the texture cache's layers and the boot message's reset, the state run */
/* 0x002F6050 */
void Pause_Update(u8 *o) {
    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    VCALL(gBootMessage, 0x20, void (*)(VObject *))(gBootMessage);
    ptmf_scall(o, &STATE(o));
}
