#ifndef GAMEOVER_H
#define GAMEOVER_H

/* gameover.c: what other files call. */
#include "common.h"
#include "common.h"
#include "ptmf.h"

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

/* gameover.c */
extern void func_002F39B0(u8 *o);
extern void func_002F3910(GameOver *o);

#endif /* GAMEOVER_H */
