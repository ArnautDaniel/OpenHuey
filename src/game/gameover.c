/* The transition screen (SceneGame +0x73EB40): the game over / ending sequences, run by
 * SceneGame_SubTransition each frame until flag 0xC. A state machine (+0x50, a PTMF): the start picks
 * the sequence by its mode (+0x0) - 0 the movie one (its frames drawn by this, +0x4), 1 / 4..
 * and 3 the faded-out ones with their music, 2 none (done at once), and in a special scene
 * (progress +0x1FBEC1) its own one. The faded-out sequences: the music and the movie stop, the
 * game freezes (the panic's shake off, the camera director stopped, every character held, the
 * effects paused), the screen wipes over 64 frames, the music starts, the room's two screen
 * tints (effects 0x1F / 0x1D) go over a second to purple / clear with the panic's fade, then
 * the movie str_SYSTEM_GAMEOVER_SFD plays; after it the first tint drifts to blue for a minute (any of the
 * face buttons skips), the music stops and the screen goes black. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "ptmf.h"
#include "input.h"
#include "actor.h"
#include "globals.h"
#include "memcard.h"
#include "navmesh.h"
#include "bgm.h"
#include "gameover.h"
#include "movie.h"
#include "panic.h"
#include "scene_game_members.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif

_Static_assert(__builtin_offsetof(GameOver, state) == 0x50, "GameOver.state");

extern const PTMF GameOver_StateStart_ptmf;   /* GameOver_StateStart */
extern const PTMF GameOver_StateStart_ptmf2;   /* GameOver_StateStart */
extern const PTMF GameOver_StateMovie_ptmf;   /* GameOver_StateMovie */
extern const PTMF GameOver_StateOthers_ptmf;   /* GameOver_StateOthers */
extern const PTMF GameOver_StateDone_ptmf;   /* GameOver_StateDone */
extern const PTMF GameOver_StateMode3_ptmf;   /* GameOver_StateMode3 */
extern const PTMF GameOver_StateOthers_ptmf2;   /* GameOver_StateOthers */
extern const PTMF GameOver_StateSpecial_ptmf;   /* GameOver_StateSpecial */
extern const char str_SYSTEM_GAMEOVER_SFD[];  /* the movie */

extern VObject *gStageMusic;
extern void *Tint_vtable[];      /* a screen tint */

typedef void (*RectFn)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32, s32);

#define PANIC(p) ((u8 *)(p) + 0x7B8)

extern void *D_0046D730[];
void *GameOverBase_dtor(u8 *o, s32 flags);

void SceneGame_SetByte19034(u8 *p, u32 v);

/* destructor (vtable Tint_vtable) */
/* 0x00267310 */
void *GameOverBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Tint_vtable;
        AT(o, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            RoomEffects_delete(o);
        }
    }
    return o;
}

/* 0x002D6000 */
void SceneGame_SetByte19034(u8 *p, u32 v) { p[0x19034] = (u8)v; }
/* (SceneGame +0x73EB40) its state at +0x50 back to GameOver_StateStart_ptmf */
/* 0x002F39B0 */
void GameOver_Reset(u8 *o) {
    AT(o, 0x50, PTMF) = GameOver_StateStart_ptmf;
}

#ifdef HG_NATIVE

/* the movie's frame, full screen: the renderer takes it (layer 0x2B, or 6 when plain) and,
 * unless plain, it is drawn again over the screen by its alpha (layer 0x2C) */
/* 0x002F0940 */
void GameOver_DrawMovieFrame(GameOver *o, s16 *shot) {
    u8 *f = AT(shot, 0x4, u8 *) + shot[1] * ((AT(shot, 0x8, u8) ^ 1) * shot[0]) * 4;

    if (AT(shot, 0x9, u8) != 0) {
        VCALL(gRenderer, 0x40, void (*)(VObject *, u8 *, s32, s32, s32, s32))(gRenderer, f, 0xC0000, shot[0], shot[1], 6);
    } else {
        VCALL(gRenderer, 0x40, void (*)(VObject *, u8 *, s32, s32, s32, s32))(gRenderer, f, 0xC0000, shot[0], shot[1],
                                                                                0x2B);
        glr_vram_draw(0xC0000, 0x2C);
    }
    if (gTexCache != NULL) {
        VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    }
}
#else
void GameOver_DrawMovieFrame(GameOver *o, s16 *shot);
#endif

/* the music player at full volume and started */
static void bgm_full(void) {
    AT(gAdx, 0x118, f32) = 1.0f;
    Bgm_ApplyVolume(gAdx);
}

static void play_music(s32 track) {
    VCALL(gMusic, 0x8, void (*)(VObject *, s32, s32, s32, f32))(gMusic, track, 0, 0, 1.0f);
}

static void movie_stop(void) {
    if (gMovie != NULL) {
        VCALL((VObject *)gMovie, 0x14, void (*)(VObject *))((VObject *)gMovie);
    }
}

/* the room's effect n, a screen tint made in its slot if missing, into *dst */
static void tint_get(u8 *fx, s32 n, u8 **dst) {
    VObject **slot = &AT(fx, 0x1438 + n * 4, VObject *);
    void *mem;

    *dst = RoomEffects_Get(fx, n);
    if (*dst != NULL) {
        return;
    }
    if (*slot != NULL) {
        u8 *fx2 = gRoomEffects;

        VCALL((VObject *)(fx2 + 0x1400), 0x14, void (*)(VObject *, void *))((VObject *)(fx2 + 0x1400), *slot);
        AT(fx2, 0x1438 + n * 4, VObject *) = NULL;
    }
    mem = VCALL((VObject *)(fx + 0x1400), 0x10, void *(*)(VObject *, u32))((VObject *)(fx + 0x1400), 0xA0);
    if (mem != NULL) {
        VObject *e = RoomEffects_new(0xA0, mem);

        if (e != NULL) {
            e->vtbl = Tint_vtable;
        }
        *slot = e;
        VCALL(*slot, 0xC, void (*)(VObject *))(*slot);
    }
    *dst = RoomEffects_Get(fx, n);
}

/* the two tints to fade from where they are: 0x1F to purple, 0x1D to clear */
static void tints_start(GameOver *o) {
    u8 *fx = gRoomEffects;

    tint_get(fx, 0x1F, &o->tint);
    o->from[0] = AT(o->tint, 0x10, u32);
    o->from[1] = AT(o->tint, 0x14, u32);
    o->to[0] = 0xAA0000C8;
    o->to[1] = 0xFFFFFFFF;
    tint_get(fx, 0x1D, &o->tint2);
    o->from2[0] = AT(o->tint2, 0x10, u32);
    o->from2[1] = AT(o->tint2, 0x14, u32);
    o->to2[0] = 0;
    o->to2[1] = 0;
}

/* a colour `t` / `n` of the way back from `to` to `from`, by byte */
static u32 rgba_mix(const u32 *from, const u32 *to, s32 t, s32 n) {
    union { u32 w; u8 b[4]; } r;
    s32 k;

    for (k = 0; k < 4; k++) {
        const u8 *f = (const u8 *)from + k, *g = (const u8 *)to + k;

        r.b[k] = *g + t * (s16)(*f - *g) / n;
    }
    return r.w;
}

/* the tints a step on (timer 60 .. 0) with the panic's fade; at the end the movie starts.
 * `fade`: the panic's fade follows */
static void tints_step(GameOver *o, s32 fade) {
    s32 t = o->timer;
    u32 a = rgba_mix(&o->from[1], &o->to[1], t, 60);
    u32 b = rgba_mix(&o->from[0], &o->to[0], t, 60);
    u32 c = rgba_mix(&o->from2[0], &o->to2[0], t, 60);
    u32 d = rgba_mix(&o->from2[1], &o->to2[1], t, 60);
    u32 was;

    AT(o->tint, 0x10, u32) = b;
    AT(o->tint, 0x14, u32) = a;
    AT(o->tint2, 0x10, u32) = c;
    AT(o->tint2, 0x14, u32) = d;
    if (fade) {
        ScreenFade_Level(PANIC(gProgress), (f32)(s32)o->timer / 60.0f);
    }
    was = o->timer;
    o->timer = was - 1;
    if (was == 0) {
        Progress_PlayMovie(gProgress, str_SYSTEM_GAMEOVER_SFD, 6);
        AT(gMovie, 0x1C4, u8) = 1;
        o->step++;
    }
}

/* the movie's end: tint 0x1F to drift to blue over a minute */
static void tint_drift_start(GameOver *o) {
    if (Movie_FrameShown(gMovie) < 0) {
        u8 *fx = gRoomEffects;

        o->timer = 1800;
        tint_get(fx, 0x1F, &o->tint);
        o->from[0] = AT(o->tint, 0x10, u32);
        o->from[1] = AT(o->tint, 0x14, u32);
        o->to[0] = 0x82000082;
        o->to[1] = 0xFFFFFFFF;
        o->step++;
    }
}

static void tint_drift(GameOver *o) {
    s32 t = o->timer;
    u32 a = rgba_mix(&o->from[1], &o->to[1], t, 1800);
    u32 b = rgba_mix(&o->from[0], &o->to[0], t, 1800);

    AT(o->tint, 0x10, u32) = b;
    AT(o->tint, 0x14, u32) = a;
    if (--o->timer == 0 || (gPadPressed & 0xF000)) {
        play_music(0xFF);
        o->timer = 0;
        o->step++;
    }
}

static void movie_wait(GameOver *o) {
    void *m = gMovie;

    if (Movie_Restart(m) > 0) {
        AT(m, 0x1BC, u8) = 1;
        o->step++;
    }
}

/* to black over 30 frames; then, the music faded, flag 8 */
static void to_black(GameOver *o) {
    u32 a;

    if (o->timer < 30) {
        o->timer++;
    }
    a = (u32)(127.0f * ((f32)(s32)o->timer / 30.0f));
    if (a >= 0x80) {
        a = 0x7F;
    }
    VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0, 0, 0x200, 0x200, 0, 0, 0, 0, (a << 24) & 0xFF000000, -1, 0, 0x30, -1);
    if (o->timer == 30 && Bgm_IsPlaying(gAdx) != 0) {
        Progress_SetFlag(gProgress, 8);
        movie_stop();
        o->step++;
    }
}

/* black; once the movie is gone, done (flag 0xC) */
static void black(GameOver *o) {
    VCALL(gRenderer, 0x7C, RectFn)(gRenderer, 0, 0, 0x200, 0x200, 0, 0, 0, 0, 0x7F000000, -1, 0, 0x30, -1);
    if (gMovie == NULL) {
        Progress_SetFlag(gProgress, 0xC);
        VCALL(gSound, 0xC, void (*)(VObject *))(gSound);
    }
}

/* the faded-out sequences; `kind` 0 the special scene's (music by progress var 0x2E), 1 mode 3
 * (track 0x47, a sound), 2 the others (music by the progress state) */
static void game_over(GameOver *o, s32 kind) {
    Progress *p;
    s32 i;

    switch (o->step) {
    case 0:
        if (gStageMusic != NULL) {
            VCALL(gStageMusic, 0x38, void (*)(VObject *, s32, s32))(gStageMusic, 0x1E, 0);
        }
        movie_stop();
        o->step++;
        return;
    case 1:
        Panic_Pause(PANIC(gProgress));
        VCALL(gCamDirector, 0x3C, void (*)(VObject *))(gCamDirector);
        for (i = 0; i < 6; i++) {
            if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) != 0) {
                AT(gCharacters[i], 0x29, u8) = 1;
            }
        }
        SceneGame_SetByte19034(gEffects, 1);
        o->step++;
        /* fallthrough */
    case 2:
        VCALL(gRenderer, 0x60, void (*)(VObject *, s32))(gRenderer, 0);
        if (gMovie == NULL) {
            o->timer = 0x40;
            o->step++;
        }
        break;
    case 3:
        VCALL(gRenderer, 0x60, void (*)(VObject *, s32))(gRenderer, (0x40 - o->timer) & 0xFF);
        if (--o->timer == 0) {
            bgm_full();
            p = gProgress;
            if (kind == 0) {
                switch (Progress_GetVar(p, 0x2E) & 0xFF) {
                case 0: play_music(0xA); break;
                case 1: play_music(0xB); break;
                case 2: play_music(0xF); break;
                case 3: play_music(0xC); break;
                }
            } else if (kind == 1) {
                play_music(0x47);
            } else if (AT(p, 0x30, u32) & 0x10) {
                play_music(0xF);
            } else if (AT(p, 0x2C, u32) & 0x200000) {
                play_music(0xE);
            } else if (AT(p, 0x2C, u32) & 0x100000) {
                play_music(0xD);
            } else if (AT(p, 0x2C, u32) & 4) {
                play_music(0xC);
            } else if (AT(p, 0x24, u32) & 2) {
                play_music(0xB);
            } else {
                play_music(0xA);
            }
            o->timer = 60;
            o->step++;
        }
        break;
    case 4:
        if (--o->timer == 0) {
            if (kind == 1) {
                VCALL(gSound, 0x14, void (*)(VObject *, s32, s32))(gSound, 0x39, 5);
            }
            tints_start(o);
            ScreenFade_Level(PANIC(gProgress), 1.0f);
            o->timer = 60;
            o->step++;
        }
        break;
    case 5:
        tints_step(o, 1);
        break;
    case 6:
        o->step++;
        break;
    case 7:
        movie_wait(o);
        break;
    case 8:
        tint_drift_start(o);
        break;
    case 9:
        tint_drift(o);
        break;
    case 10:
        to_black(o);
        break;
    case 11:
        black(o);
        break;
    }
}

/* state: the special scene's sequence */
/* 0x002F0BB0 */
void GameOver_StateSpecial(GameOver *o) {
    game_over(o, 0);
}

/* state: none - done */
/* 0x002F16F0 */
void GameOver_StateDone(GameOver *o) {
    Progress_SetFlag(gProgress, 0xC);
}

/* state: mode 3's sequence */
/* 0x002F1700 */
void GameOver_StateMode3(GameOver *o) {
    game_over(o, 1);
}

/* state: the others' sequence */
/* 0x002F2180 */
void GameOver_StateOthers(GameOver *o) {
    game_over(o, 2);
}

/* state: the movie sequence - as the others without the freeze and the wipe, the music (track
 * 6) at once; when the movie gave its frame (+0x3 at the start) it draws that over everything
 * and the panic's fade stays */
/* 0x002F2D30 */
void GameOver_StateMovie(GameOver *o) {
    switch (o->step) {
    case 0:
        bgm_full();
        play_music(6);
        if (gStageMusic != NULL) {
            VCALL(gStageMusic, 0x38, void (*)(VObject *, s32, s32))(gStageMusic, 0x1E, 0);
        }
        movie_stop();
        o->step++;
        break;
    case 1:
        o->drawMovie = o->hasMovie;
        o->step++;
        break;
    case 2:
        if (gMovie == NULL) {
            o->timer = 60;
            o->step++;
        }
        break;
    case 3:
        if (--o->timer == 0) {
            tints_start(o);
            if (o->drawMovie == 0) {
                ScreenFade_Level(PANIC(gProgress), 1.0f);
            }
            o->timer = 60;
            o->step++;
        }
        break;
    case 4:
        tints_step(o, o->drawMovie == 0);
        break;
    case 5:
        o->step++;
        break;
    case 6:
        movie_wait(o);
        break;
    case 7:
        tint_drift_start(o);
        break;
    case 8:
        tint_drift(o);
        break;
    case 9:
        to_black(o);
        break;
    case 10:
        black(o);
        break;
    }
    if (o->drawMovie != 0) {
        GameOver_DrawMovieFrame(o, &o->frameW);
    }
}

/* state: start - unless the file loader is busy (2), pick the sequence (the world drawn behind
 * all but mode 2's), take the movie's last frame (+0x8) and, with one, the panic's fade off */
/* 0x002F3710 */
void GameOver_StateStart(GameOver *o) {
    Progress *p;

    if (VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) == 2) {
        return;
    }
    p = gProgress;
    if (AT(p, 0x1FBEC1, u8) == 0) {
        switch (o->mode) {
        case 0:
            o->state = GameOver_StateMovie_ptmf;
            o->world = 1;
            break;
        case 1:
            o->state = GameOver_StateOthers_ptmf;
            o->world = 1;
            break;
        case 2:
            o->state = GameOver_StateDone_ptmf;
            o->world = 0;
            break;
        case 3:
            o->state = GameOver_StateMode3_ptmf;
            o->world = 1;
            break;
        default:
            o->state = GameOver_StateOthers_ptmf2;
            o->world = 1;
            break;
        }
    } else {
        o->state = GameOver_StateSpecial_ptmf;
        o->world = 1;
    }
    o->step = 0;
    o->hasMovie = 0;
    if (gMovie != NULL) {
        o->hasMovie = VCALL((VObject *)gMovie, 0x18, u8 (*)(VObject *, s16 *))((VObject *)gMovie, &o->frameW);
    }
    o->drawMovie = 0;
    if (o->hasMovie != 0) {
        ScreenFade_Level(PANIC(p), 0.0f);
    }
}

/* each frame: the state, and when done (flag 0xC) the music stopped and back to the start */
/* 0x002F3910 */
void GameOver_Update(GameOver *o) {
    ptmf_scall(o, &o->state);
    if (Progress_TestFlag(gProgress, 0xC) != 0) {
        play_music(0xFF);
        o->state = GameOver_StateStart_ptmf2;
    }
}
