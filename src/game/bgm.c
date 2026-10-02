/* Streamed music (CRI ADX): methods of the ADX sound system (system +0x305280, global
 * D_0044E980) for its music stream. ADXT_* are CRI's library (native: silent). */
#include "common.h"
#include "game.h"
#include "ptmf.h"

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

typedef struct Bgm {
    /* 0x000 */ void **vtbl;
    /* 0x004 */ void *work;      /* the stream's work buffer (0x231E4 bytes), NULL: none */
    /* 0x008 */ void *adxt;      /* the stream (ADXT handle) */
    /* 0x00C */ s32 dir;       /* the track's folder (loader handle), 0: current */
    /* 0x010 */ char name[0x100]; /* the track, empty: none */
    /* 0x110 */ f32 volume[5];   /* multiplied together; [3] the master volume */
} Bgm;

_Static_assert(__builtin_offsetof(Bgm, volume) == 0x110, "Bgm.volume");

extern VObject *D_0044E560;      /* the sound driver: +0x6C the output mode */
extern u8 *D_0044E978;           /* +0x38: the master volume */
extern void ADXT_Destroy(void *adxt);
extern void *ADXT_Create(s32 maxch, void *work, s32 size);
extern void ADXT_SetReloadSct(void *adxt, s32 n);
extern void ADXT_SetOutVol(void *adxt, s32 vol);
extern void ADXT_SetOutPan(void *adxt, s32 ch, s32 pan);
extern void ADXT_SetLpFlg(void *adxt, s32 on);
extern void ADXT_SetWaitPlayStart(void *adxt, s32 on);
extern void func_001D4750(s32 mono);   /* ADX: mono output */

/* Set up the music stream on work buffer `work` (NULL: none): looping, stereo or mono as the
 * sound settings say, full volume. */
void func_002D2370(Bgm *b, void *work) {
    if (b->work != NULL) {
        if (b->adxt != NULL) {
            ADXT_Destroy(b->adxt);
            b->adxt = NULL;
        }
        b->work = NULL;
        b->dir = 0;
    }
    b->work = work;
    if (b->work != NULL) {
        if (b->adxt != NULL) {
            ADXT_Destroy(b->adxt);
            b->adxt = NULL;
        }
        b->adxt = ADXT_Create(2, b->work, 0x231E4);
        if (b->adxt != NULL) {
            ADXT_SetReloadSct(b->adxt, 0x19);
        }
    }
    ADXT_SetOutVol(b->adxt, 0);
    ADXT_SetOutPan(b->adxt, 0, -0x80);
    ADXT_SetOutPan(b->adxt, 1, -0x80);
    if (D_0044E560 == NULL) {
        func_001D4750(0);
    } else {
        s8 mode = VCALL(D_0044E560, 0x6C, s32 (*)(VObject *))(D_0044E560);

        if (mode == 2 || mode == 1) {
            func_001D4750(0);
        } else if (mode == 0) {
            func_001D4750(1);
        } else {
            func_001D4750(0);
        }
    }
    ADXT_SetLpFlg(b->adxt, 1);
    ADXT_SetWaitPlayStart(b->adxt, 0);
    b->volume[3] = *(f32 *)(D_0044E978 + 0x38);
    b->volume[4] = 1.0f;
    b->volume[2] = 1.0f;
    b->volume[1] = 1.0f;
    b->volume[0] = 1.0f;
}

extern f32 func_0031C830(f32 x);   /* log10f */

/* apply the volume (0.1 dB, -99.9 dB for silence) while the stream is on */
void func_002D1FD0(Bgm *b) {
    f32 v = b->volume[4] * (b->volume[2] * (b->volume[1] * (b->volume[0] * b->volume[3])));
    s32 db;

    if (v == 0.0f) {
        db = -999;
    } else {
        db = (s32)(100.0f * func_0031C830(v));
    }
    if (db > 0) {
        db = 0;
    }
    if (db < -999) {
        db = -999;
    }
    if (b->name[0] != 0 && b->adxt != NULL) {
        ADXT_SetOutVol(b->adxt, db);
    }
}

extern VObject *gFileLoader;
extern void ADXT_Stop(void *adxt);
extern void ADXT_Pause(void *adxt, s32 on);
extern s32 ADXT_GetStat(void *adxt);
extern s32 ADXT_IsReadyPlayStart(void *adxt);
extern s32 func_001D3E20(void *adxt);               /* ADX: still playing */
extern void func_001D4A20(void *adxt, char *name);  /* ADXT_StartFname */
extern void func_001E7430(s32 a, s32 dir);          /* CRI file system: the current folder */
extern char *func_001183C0(char *d, const char *s);   /* strcpy */
extern char *func_00118978(char *d, const char *s, s32 n);   /* strncpy */

#define ADXT_STAT_PLAYEND 6

/* stop */
void func_002D1F90(Bgm *b) {
    if (b->adxt != NULL) {
        b->name[0] = 0;
        b->dir = 0;
        ADXT_Stop(b->adxt);
    }
}

/* resume */
void func_002D20A0(Bgm *b) {
    if (b->adxt != NULL) {
        ADXT_Pause(b->adxt, 0);
    }
}

/* nonzero while the track plays (or with nothing to play) */
s32 func_002D20D0(Bgm *b) {
    if (b->adxt == NULL) {
        return 1;
    }
    if (b->name[0] == 0) {
        return 1;
    }
    return func_001D3E20(b->adxt) != 0;
}

/* nonzero once the track can start; a track that has ended starts again */
s32 func_002D2120(Bgm *b) {
    if (b->adxt == NULL) {
        return 1;
    }
    if (b->name[0] == 0) {
        return 1;
    }
    if (ADXT_GetStat(b->adxt) == ADXT_STAT_PLAYEND) {
        if (b->dir != 0) {
            func_001E7430(0, b->dir);
        }
        func_001D4A20(b->adxt, b->name);
        return 0;
    }
    return ADXT_IsReadyPlayStart(b->adxt) != 0;
}

/* play track `path` (folder\name or a name in the current folder), looping or not, paused or
 * not */
void func_002D21B0(Bgm *b, const char *path, s32 loop, s32 pause) {
    char dir[256];
    s32 i;

    if (b->adxt == NULL || path[0] == 0) {
        return;
    }
    b->dir = 0;
    dir[0] = 0;
    for (i = 0; i < 256; i++) {
        if (path[i] == 0) {
            break;
        }
        if (path[i] == '\\') {
            func_001183C0(b->name, path + i + 1);
            func_00118978(dir, path, i);
            dir[i] = 0;
            break;
        }
    }
    if (dir[0] != 0) {
        b->dir = VCALL(gFileLoader, 0x3C, s32 (*)(VObject *, char *))(gFileLoader, dir);
    } else {
        b->dir = VCALL(gFileLoader, 0x3C, s32 (*)(VObject *, char *))(gFileLoader, NULL);
        func_001183C0(b->name, path);
    }
    if (b->dir != 0) {
        func_001E7430(0, b->dir);
    }
    ADXT_SetLpFlg(b->adxt, loop & 0xFF);
    ADXT_SetWaitPlayStart(b->adxt, 0);
    ADXT_Pause(b->adxt, pause & 0xFF);
    func_001D4A20(b->adxt, b->name);
}

/* release the stream */
void func_002D2330(Bgm *b) {
    if (b->adxt != NULL) {
        ADXT_Destroy(b->adxt);
        b->adxt = NULL;
    }
    b->work = NULL;
    b->dir = 0;
}

/* ---- the music controller (SceneTitle +0x140CA4, global D_0044E970): plays a track from the
 * table, fading it in and out ---- */

typedef struct BgmCtl {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ u8 cur;          /* the track playing, 0xFF none */
    /* 0x05 */ u8 req;          /* the track wanted */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ f32 fade;        /* 0..1 */
    /* 0x0C */ f32 fadeSpeed;   /* per frame */
    /* 0x10 */ f32 level;       /* 0..1 */
    /* 0x14 */ f32 levelSpeed;
    /* 0x18 */ u8 pause;        /* start the next track paused */
} BgmCtl;

typedef struct BgmTrack {
    /* 0x0 */ const char *name;
    /* 0x4 */ u8 loop;
    /* 0x5 */ u8 pad5[3];
} BgmTrack;

extern BgmTrack D_00416800[];   /* the tracks */
extern Bgm *D_0044E980;

/* every frame: follow the wanted track (fade out to stop, start a new one at full volume),
 * step the fade and the level, set the stream's volume */
void func_002E3200(BgmCtl *c) {
    Bgm *b;
    s32 start = 0;
    f32 v;

    if (c->cur != c->req) {
        if (c->req == 0xFF) {
            c->fadeSpeed = -0x1.111112p-5f;   /* -1/30 */
        } else {
            start = 1;
            c->fade = 1.0f;
        }
    } else if (c->req != 0xFF && c->fadeSpeed < 0.0f) {
        c->fadeSpeed = 0x1.111112p-5f;
    }
    if (c->cur != 0xFF) {
        b = D_0044E980;
        if (func_002D2120(b) && func_002D20D0(b)) {
            c->fade = 0.0f;
            c->fadeSpeed = 0.0f;
            c->cur = 0xFF;
            c->req = 0xFF;
            if (b != NULL) {
                func_002D1F90(b);
            }
        }
        c->fade += c->fadeSpeed;
        if (!(c->fade <= 1.0f)) {
            c->fade = 1.0f;
            c->fadeSpeed = 0.0f;
        }
        if (c->fade <= 0.0f) {
            c->fade = 0.0f;
            c->fadeSpeed = 0.0f;
            if (c->cur != 0xFF) {
                if (b != NULL) {
                    func_002D1F90(b);
                }
                c->cur = 0xFF;
            }
        }
    }
    if (c->levelSpeed != 0.0f) {
        c->level += c->levelSpeed;
        if (!(c->level <= 1.0f)) {
            c->level = 1.0f;
            c->levelSpeed = 0.0f;
        }
        if (c->level < 0.0f) {
            c->level = 0.0f;
            c->levelSpeed = 0.0f;
        }
    }
    v = c->fade * c->level;
    if (!(v <= 1.0f)) {
        v = 1.0f;
    }
    if (v < 0.0f) {
        v = 0.0f;
    }
    b = D_0044E980;
    if (b != NULL) {
        b->volume[0] = v;
        if (v < 0.0f) {
            b->volume[0] = 0.0f;
        }
        if (!(b->volume[0] <= 1.0f)) {
            b->volume[0] = 1.0f;
        }
        func_002D1FD0(b);
    }
    if (start) {
        c->cur = c->req;
        func_002D21B0(b, D_00416800[c->cur].name, D_00416800[c->cur].loop, c->pause);
    }
}

/* stop the music at once */
void func_002E31D0(void) {
    if (D_0044E980 != NULL) {
        func_002D1F90(D_0044E980);
    }
}
