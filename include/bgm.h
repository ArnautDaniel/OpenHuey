#ifndef BGM_H
#define BGM_H

/* Music (src/game/bgm.c). */
#include "common.h"

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
extern void func_002D2370(Bgm *b, void *work);
extern void func_002D1FD0(Bgm *b);   /* apply the music volume */
extern void func_002D20A0(Bgm *b);
extern s32 func_002D20D0(Bgm *b);   /* the stream is free */
extern s32 func_002D2120(Bgm *b);
extern void func_002D2330(Bgm *b);
extern void func_002E3200(BgmCtl *c);
extern void func_002E31D0(BgmCtl *c);

#endif /* BGM_H */
