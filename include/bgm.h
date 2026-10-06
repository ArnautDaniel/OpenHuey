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

#endif /* BGM_H */
