#ifndef MUSIC_H
#define MUSIC_H

/* music.c: what other files call. */
#include "common.h"

/* music.c */
extern void MusicDir_Update(u8 *d);

/* ---- (was bgm.h) ---- */

/* Music (src/game/bgm.c). */

/* The music controller (global gMusic, in the title and the game scenes): plays a track of
 * the table, fading it in and out. Vtable +0x8 want(track, pause, restart, level). */
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
_Static_assert(sizeof(BgmCtl) == 0x1C, "BgmCtl size");

typedef struct Bgm {
    /* 0x000 */ void **vtbl;
    /* 0x004 */ void *work;      /* the stream's work buffer (0x231E4 bytes), NULL: none */
    /* 0x008 */ void *adxt;      /* the stream (ADXT handle) */
    /* 0x00C */ s32 dir;       /* the track's folder (loader handle), 0: current */
    /* 0x010 */ char name[0x100]; /* the track, empty: none */
    /* 0x110 */ f32 volume[5];   /* multiplied together; [3] the master volume */
} Bgm;

/* bgm.c */
extern void Bgm_Init(Bgm *b, void *work);
extern void Bgm_ApplyVolume(Bgm *b);   /* apply the music volume */
extern void Bgm_Resume(Bgm *b);
extern s32 Bgm_IsPlaying(Bgm *b);   /* the stream is free */
extern s32 Bgm_CanStart(Bgm *b);
extern void Bgm_Release(Bgm *b);
extern void BgmCtl_Update(BgmCtl *c);
extern void BgmCtl_StopNow(BgmCtl *c);

/* music.c */
extern void BgmCtl_ctor(u8 *p);

#endif /* MUSIC_H */
