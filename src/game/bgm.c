/* Streamed music (CRI ADX): methods of the ADX sound system (system +0x305280, global
 * gAdx) for its music stream. ADXT_* are CRI's library (native: silent). */
#include "common.h"
#include "bgm.h"
#include "game.h"
#include "ptmf.h"
#include "globals.h"
#include "cri/adx.h"
#include "libc.h"
#include "msl.h"

_Static_assert(__builtin_offsetof(Bgm, volume) == 0x110, "Bgm.volume");

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
    if (gSound == NULL) {
        func_001D4750(0);
    } else {
        s8 mode = VCALL(gSound, 0x6C, s32 (*)(VObject *))(gSound);

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
    b->volume[3] = *(f32 *)(gSystemData + 0x38);
    b->volume[4] = 1.0f;
    b->volume[2] = 1.0f;
    b->volume[1] = 1.0f;
    b->volume[0] = 1.0f;
}

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

/* ---- the music controller (SceneTitle +0x140CA4, global gMusic): plays a track from the
 * table, fading it in and out ---- */

typedef struct BgmTrack {
    /* 0x0 */ const char *name;
    /* 0x4 */ u8 loop;
    /* 0x5 */ u8 pad5[3];
} BgmTrack;

extern BgmTrack D_00416800[];   /* the tracks */

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
        b = gAdx;
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
    b = gAdx;
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
void func_002E31D0(BgmCtl *c) {
    if (gAdx != NULL) {
        func_002D1F90(gAdx);
    }
}

extern void *D_0046A110[];   /* BgmCtl */
extern void *D_0046A100[];   /* its base */

/* +0x8 want track `track` (0xFF: none, fade out) at level `level`; `restart`: from the start
 * even if it's the one playing; `pause`: start it paused */
void func_00130A40(BgmCtl *c, s32 track, s32 pause, s32 restart, f32 level) {
    c->req = track;
    if ((u8)track != 0xFF) {
        c->level = level;
        if (restart) {
            c->cur = 0xFF;
        }
    }
    c->pause = pause;
}

/* +0xC */
BgmCtl *func_001309D0(BgmCtl *c, s32 flags) {
    if (c != NULL) {
        c->vtbl = D_0046A110;
        if (c != NULL) {
            c->vtbl = D_0046A100;
            if (c != NULL) {
                gMusic = NULL;
            }
        }
        if ((s16)flags > 0) {
            func_00100490(c);
        }
    }
    return c;
}
