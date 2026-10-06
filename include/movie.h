#ifndef MOVIE_H
#define MOVIE_H

/* movie.c: what other files call. */
#include "common.h"
#include "common.h"
#include "game.h"

typedef struct Movie {
    /* 0x000 */ Scene base;
    /* 0x014 */ VObject *ply;        /* the player (mwPly handle, NULL: none) */
    /* 0x018 */ u8 *frame;           /* the current frame: image, w at +0x20, h +0x24, no. +0x38 */
    /* 0x01C */ u8 frm1C[0x20 - 0x1C];
    /* 0x020 */ s32 frameW;
    /* 0x024 */ s32 frameH;
    /* 0x028 */ u8 frm28[0x38 - 0x28];
    /* 0x038 */ s32 frameNo;
    /* 0x03C */ u8 frm3C[0xA0 - 0x3C];
    /* 0x0A0 */ u8 stat;             /* the player's status: 2 playing, 3 / 4 ended */
    /* 0x0A1 */ u8 padA1[3];
    /* 0x0A4 */ void *work;          /* the work buffer, when allocated */
    /* 0x0A8 */ char name[0x100];    /* the file, empty: none */
    /* 0x1A8 */ s32 dir;             /* its folder (loader handle), 0: current */
    /* 0x1AC */ u8 keepRenderer;
    /* 0x1AD */ u8 pad1AD[3];
    /* 0x1B0 */ s32 time;
    /* 0x1B4 */ u8 hasFrame;
    /* 0x1B5 */ u8 frameBuf;        /* class 0: the frame written next (0 / 1) */
    /* 0x1B6 */ u8 pad1B6[2];
    /* 0x1B8 */ void *frames;        /* class 0: two 256 x 224 frames the movie is decoded into */
    /* 0x1BC */ u8 mode;
    /* 0x1BD */ u8 pad1BD[3];
    /* 0x1C0 */ s32 shownFrame;      /* -1 none */
    /* 0x1C4 */ u8 loop;
    /* 0x1C5 */ u8 pad1C5[3];
    /* 0x1C8 */ f32 volume[5];       /* multiplied together */
} Movie;

/* movie.c */
extern void Movie_ApplyVolume(Movie *m);   /* apply the movie volume */
extern s32 Movie_Status(Movie *m);   /* 2 playing, 0 done, -1 none */
extern s32 Movie_FrameShown(Movie *m);
extern s32 Movie_Restart(Movie *m);   /* restarted: 2 / 0 / -1 as Movie_Status */
extern void Movie_SetFile(Movie *m, const char *path, s32 mode, s32 keep);
extern Movie *Movie_ctor(Movie *m);   /* Movie constructor */

#endif /* MOVIE_H */
