/* The transition screen (SceneGame +0x73EB40): the game over / ending sequences, run by
 * func_0039E380 each frame until flag 0xC. A state machine (+0x50, a PTMF): the start picks
 * the sequence by its mode (+0x0) - 0 the movie one (its frames drawn by this, +0x4), 1 / 4..
 * and 3 the faded-out ones with their music, 2 none (done at once), and in a special scene
 * (progress +0x1FBEC1) its own one. The faded-out sequences: the music and the movie stop, the
 * game freezes (the panic's shake off, the camera director stopped, every character held, the
 * effects paused), the screen wipes over 64 frames, the music starts, the room's two screen
 * tints (effects 0x1F / 0x1D) go over a second to purple / clear with the panic's fade, then
 * the movie D_0045E2E0 plays; after it the first tint drifts to blue for a minute (any of the
 * face buttons skips), the music stops and the screen goes black. */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "ptmf.h"
#include "input.h"
#include "actor.h"

typedef struct GameOver {
    /* 0x00 */ u8 mode;
    /* 0x01 */ u8 world;        /* the world drawn behind it */
    /* 0x02 */ u8 step;
    /* 0x03 */ u8 hasMovie;     /* the movie gave its frame (+0x8) */
    /* 0x04 */ u8 drawMovie;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ s16 frameW, frameH;
    /* 0x0C */ u8 *frame;       /* two frames, the shown one +0x10 */
    /* 0x10 */ u8 frameBuf;
    /* 0x11 */ u8 framePlain;   /* drawn by the renderer only */
    /* 0x12 */ u8 pad12[2];
    /* 0x14 */ u32 timer;
    /* 0x18 */ u32 from[2];     /* tint 0x1F's colours: from, to */
    /* 0x20 */ u32 to[2];
    /* 0x28 */ u8 *tint;        /* effect 0x1F */
    /* 0x2C */ u8 *tint2;       /* effect 0x1D */
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ u32 from2[2];
    /* 0x3C */ u32 to2[2];
    /* 0x44 */ u8 pad44[0xC];
    /* 0x50 */ PTMF state;
} GameOver;
_Static_assert(__builtin_offsetof(GameOver, state) == 0x50, "GameOver.state");

extern const PTMF D_0041A150;   /* func_002F3710 */
extern const PTMF D_0041A160;   /* func_002F3710 */
extern const PTMF D_0041A170;   /* func_002F2D30 */
extern const PTMF D_0041A180;   /* func_002F2180 */
extern const PTMF D_0041A190;   /* func_002F16F0 */
extern const PTMF D_0041A1A0;   /* func_002F1700 */
extern const PTMF D_0041A1B0;   /* func_002F2180 */
extern const PTMF D_0041A1C0;   /* func_002F0BB0 */
extern const char D_0045E2E0[];  /* the movie */

extern VObject *gFileLoader;
extern void *D_0044E958;        /* the movie playing */
extern u8 *D_0044E980;          /* the music player */
extern VObject *D_0044E970;     /* the music: +0x8 play (track, 0, 0, volume) */
extern VObject *D_00456DF0;
extern VObject *D_0044E4F8;     /* the camera director */
extern VObject *D_0044E560;     /* the sound driver */
extern VObject *D_0044E4F0;     /* the renderer */
extern VObject *D_0044E4E8;     /* the texture cache */
extern u8 *D_0044E4C0;          /* the room's effects */
extern u8 *D_0044E578;          /* the effect manager */
extern void *D_0046D750[];      /* a screen tint */
extern Character *gCharacters[];
extern void *func_00266C40(u8 *fx, s32 n);
extern void *func_002672F0(u32 size, void *place);
extern void func_002EF480(u8 *fade, f32 t);
extern void func_002F02F0(u8 *panic);
extern void func_002D6000(u8 *mgr, u32 v);
extern void func_002D1FD0(u8 *bgm);
extern s32 func_002D20D0(u8 *bgm);
extern void func_001768B0(Progress *p, const char *name, s32 arg);
extern s32 func_002B6640(void *movie);
extern s32 func_002B64F0(void *movie);

typedef void (*RectFn)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, u32, s32, s32, s32, s32);

#define PANIC(p) ((u8 *)(p) + 0x7B8)

/* (SceneGame +0x73EB40) its state at +0x50 back to D_0041A150 */
void func_002F39B0(u8 *o) {
    AT(o, 0x50, PTMF) = D_0041A150;
}

/* the movie's frame, full screen: the renderer draws it (layer 0x2B, or 6 when plain) and,
 * unless plain, it is drawn again from VRAM as a sprite (Z writes off) */
void func_002F0940(GameOver *o, s16 *shot) {
    u8 *f = AT(shot, 0x4, u8 *) + shot[1] * ((AT(shot, 0x8, u8) ^ 1) * shot[0]) * 4;

    if (AT(shot, 0x9, u8) != 0) {
        VCALL(D_0044E4F0, 0x40, void (*)(VObject *, u8 *, s32, s32, s32, s32))(D_0044E4F0, f, 0xC0000, shot[0], shot[1], 6);
    } else {
        VObject *r = D_0044E4F0;
        u64 *p;

        VCALL(r, 0x40, void (*)(VObject *, u8 *, s32, s32, s32, s32))(r, f, 0xC0000, shot[0], shot[1], 0x2B);
        p = VCALL(r, 0x10, u64 *(*)(VObject *, s32, s32))(r, 0x10, 0x2C);
        if (p != NULL) {
#ifdef HG_NATIVE
            extern void glr_todo(const char *what);

            glr_todo("game over movie frame (func_002F0940)");
            p[0] = 0x1000000F;
            AT(p, 0x8, u32) = 0;
            AT(p, 0xC, u32) = 0x50000000;
#else
            p[0] = 0x1000000F;
            AT(p, 0x8, u32) = 0;
            AT(p, 0xC, u32) = 0x5000000F;
            p[2] = 0x800E | (0x10000000ULL << 32);
            p[3] = 0xE;
            p[4] = 0x310000A0 | (1ULL << 32);   /* ZBUF_1: no Z writes */
            p[5] = 0x4E;
            p[6] = 0x30000;                     /* TEST_1: Z always */
            p[7] = 0x47;
            p[8] = 0;                           /* TEXFLUSH */
            p[9] = 0x3F;
            p[10] = ((s64)((shot[0] + 0x3F) / 64) << 14) | 0x64003000 | (6ULL << 32);   /* TEX0_1 */
            p[11] = 6;
            p[12] = 0x44;                       /* ALPHA_1 */
            p[13] = 0x42;
            AT(p, 0x70, u32) = 0x80808080;      /* RGBAQ */
            AT(p, 0x74, u32) = 0x3F800000;
            p[15] = 1;
            p[16] = 0x156;                      /* PRIM: sprite, textured, blended, UV */
            p[17] = 0;
            p[18] = 0;                          /* UV */
            p[19] = 3;
            p[20] = 0x72007000;                 /* XYZ2 */
            p[21] = 5;
            p[22] = (s64)((shot[0] << 4) | ((shot[1] << 4) << 16));
            p[23] = 3;
            p[24] = (s64)0x8E009000;
            p[25] = 5;
            p[26] = 0x310000A0;
            p[27] = 0x4E;
            p[28] = 0x5000F;
            p[29] = 0x47;
            p[30] = 0x44;
            p[31] = 0x42;
#endif
        }
    }
    if (D_0044E4E8 != NULL) {
        VCALL(D_0044E4E8, 0x18, void (*)(VObject *))(D_0044E4E8);
    }
}

/* the music player at full volume and started */
static void bgm_full(void) {
    AT(D_0044E980, 0x118, f32) = 1.0f;
    func_002D1FD0(D_0044E980);
}

static void play_music(s32 track) {
    VCALL(D_0044E970, 0x8, void (*)(VObject *, s32, s32, s32, f32))(D_0044E970, track, 0, 0, 1.0f);
}

static void movie_stop(void) {
    if (D_0044E958 != NULL) {
        VCALL((VObject *)D_0044E958, 0x14, void (*)(VObject *))((VObject *)D_0044E958);
    }
}

/* the room's effect n, a screen tint made in its slot if missing, into *dst */
static void tint_get(u8 *fx, s32 n, u8 **dst) {
    VObject **slot = &AT(fx, 0x1438 + n * 4, VObject *);
    void *mem;

    *dst = func_00266C40(fx, n);
    if (*dst != NULL) {
        return;
    }
    if (*slot != NULL) {
        u8 *fx2 = D_0044E4C0;

        VCALL((VObject *)(fx2 + 0x1400), 0x14, void (*)(VObject *, void *))((VObject *)(fx2 + 0x1400), *slot);
        AT(fx2, 0x1438 + n * 4, VObject *) = NULL;
    }
    mem = VCALL((VObject *)(fx + 0x1400), 0x10, void *(*)(VObject *, u32))((VObject *)(fx + 0x1400), 0xA0);
    if (mem != NULL) {
        VObject *e = func_002672F0(0xA0, mem);

        if (e != NULL) {
            e->vtbl = D_0046D750;
        }
        *slot = e;
        VCALL(*slot, 0xC, void (*)(VObject *))(*slot);
    }
    *dst = func_00266C40(fx, n);
}

/* the two tints to fade from where they are: 0x1F to purple, 0x1D to clear */
static void tints_start(GameOver *o) {
    u8 *fx = D_0044E4C0;

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
        func_002EF480(PANIC(gProgress), (f32)(s32)o->timer / 60.0f);
    }
    was = o->timer;
    o->timer = was - 1;
    if (was == 0) {
        func_001768B0(gProgress, D_0045E2E0, 6);
        AT(D_0044E958, 0x1C4, u8) = 1;
        o->step++;
    }
}

/* the movie's end: tint 0x1F to drift to blue over a minute */
static void tint_drift_start(GameOver *o) {
    if (func_002B64F0(D_0044E958) < 0) {
        u8 *fx = D_0044E4C0;

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
    if (--o->timer == 0 || (D_0047E37C & 0xF000)) {
        play_music(0xFF);
        o->timer = 0;
        o->step++;
    }
}

static void movie_wait(GameOver *o) {
    void *m = D_0044E958;

    if (func_002B6640(m) > 0) {
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
    VCALL(D_0044E4F0, 0x7C, RectFn)(D_0044E4F0, 0, 0, 0x200, 0x200, 0, 0, 0, 0, (a << 24) & 0xFF000000, -1, 0, 0x30, -1);
    if (o->timer == 30 && func_002D20D0(D_0044E980) != 0) {
        Progress_SetFlag(gProgress, 8);
        movie_stop();
        o->step++;
    }
}

/* black; once the movie is gone, done (flag 0xC) */
static void black(GameOver *o) {
    VCALL(D_0044E4F0, 0x7C, RectFn)(D_0044E4F0, 0, 0, 0x200, 0x200, 0, 0, 0, 0, 0x7F000000, -1, 0, 0x30, -1);
    if (D_0044E958 == NULL) {
        Progress_SetFlag(gProgress, 0xC);
        VCALL(D_0044E560, 0xC, void (*)(VObject *))(D_0044E560);
    }
}

/* the faded-out sequences; `kind` 0 the special scene's (music by progress var 0x2E), 1 mode 3
 * (track 0x47, a sound), 2 the others (music by the progress state) */
static void game_over(GameOver *o, s32 kind) {
    Progress *p;
    s32 i;

    switch (o->step) {
    case 0:
        if (D_00456DF0 != NULL) {
            VCALL(D_00456DF0, 0x38, void (*)(VObject *, s32, s32))(D_00456DF0, 0x1E, 0);
        }
        movie_stop();
        o->step++;
        return;
    case 1:
        func_002F02F0(PANIC(gProgress));
        VCALL(D_0044E4F8, 0x3C, void (*)(VObject *))(D_0044E4F8);
        for (i = 0; i < 6; i++) {
            if (gCharacters[i] != NULL && AT(gCharacters[i], 0x28, u8) != 0) {
                AT(gCharacters[i], 0x29, u8) = 1;
            }
        }
        func_002D6000(D_0044E578, 1);
        o->step++;
        /* fallthrough */
    case 2:
        VCALL(D_0044E4F0, 0x60, void (*)(VObject *, s32))(D_0044E4F0, 0);
        if (D_0044E958 == NULL) {
            o->timer = 0x40;
            o->step++;
        }
        break;
    case 3:
        VCALL(D_0044E4F0, 0x60, void (*)(VObject *, s32))(D_0044E4F0, (0x40 - o->timer) & 0xFF);
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
                VCALL(D_0044E560, 0x14, void (*)(VObject *, s32, s32))(D_0044E560, 0x39, 5);
            }
            tints_start(o);
            func_002EF480(PANIC(gProgress), 1.0f);
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
void func_002F0BB0(GameOver *o) {
    game_over(o, 0);
}

/* state: none - done */
void func_002F16F0(GameOver *o) {
    Progress_SetFlag(gProgress, 0xC);
}

/* state: mode 3's sequence */
void func_002F1700(GameOver *o) {
    game_over(o, 1);
}

/* state: the others' sequence */
void func_002F2180(GameOver *o) {
    game_over(o, 2);
}

/* state: the movie sequence - as the others without the freeze and the wipe, the music (track
 * 6) at once; when the movie gave its frame (+0x3 at the start) it draws that over everything
 * and the panic's fade stays */
void func_002F2D30(GameOver *o) {
    switch (o->step) {
    case 0:
        bgm_full();
        play_music(6);
        if (D_00456DF0 != NULL) {
            VCALL(D_00456DF0, 0x38, void (*)(VObject *, s32, s32))(D_00456DF0, 0x1E, 0);
        }
        movie_stop();
        o->step++;
        break;
    case 1:
        o->drawMovie = o->hasMovie;
        o->step++;
        break;
    case 2:
        if (D_0044E958 == NULL) {
            o->timer = 60;
            o->step++;
        }
        break;
    case 3:
        if (--o->timer == 0) {
            tints_start(o);
            if (o->drawMovie == 0) {
                func_002EF480(PANIC(gProgress), 1.0f);
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
        func_002F0940(o, &o->frameW);
    }
}

/* state: start - unless the file loader is busy (2), pick the sequence (the world drawn behind
 * all but mode 2's), take the movie's last frame (+0x8) and, with one, the panic's fade off */
void func_002F3710(GameOver *o) {
    Progress *p;

    if (VCALL(gFileLoader, 0x24, s32 (*)(VObject *))(gFileLoader) == 2) {
        return;
    }
    p = gProgress;
    if (AT(p, 0x1FBEC1, u8) == 0) {
        switch (o->mode) {
        case 0:
            o->state = D_0041A170;
            o->world = 1;
            break;
        case 1:
            o->state = D_0041A180;
            o->world = 1;
            break;
        case 2:
            o->state = D_0041A190;
            o->world = 0;
            break;
        case 3:
            o->state = D_0041A1A0;
            o->world = 1;
            break;
        default:
            o->state = D_0041A1B0;
            o->world = 1;
            break;
        }
    } else {
        o->state = D_0041A1C0;
        o->world = 1;
    }
    o->step = 0;
    o->hasMovie = 0;
    if (D_0044E958 != NULL) {
        o->hasMovie = VCALL((VObject *)D_0044E958, 0x18, u8 (*)(VObject *, s16 *))((VObject *)D_0044E958, &o->frameW);
    }
    o->drawMovie = 0;
    if (o->hasMovie != 0) {
        func_002EF480(PANIC(p), 0.0f);
    }
}

/* each frame: the state, and when done (flag 0xC) the music stopped and back to the start */
void func_002F3910(GameOver *o) {
    ptmf_scall(o, &o->state);
    if (Progress_TestFlag(gProgress, 0xC) != 0) {
        play_music(0xFF);
        o->state = D_0041A160;
    }
}
