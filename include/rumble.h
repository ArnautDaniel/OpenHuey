#ifndef RUMBLE_H
#define RUMBLE_H

/* rumble.c: what other files call. */
#include "common.h"
#include "common.h"

typedef struct RumbleChannel {
    /* 0x00 */ s16 timeA;
    /* 0x04 */ s32 stepA;
    /* 0x08 */ s32 valueA;   /* 0 or 1.0 (0x10000) */
    /* 0x0C */ s16 timeB;
    /* 0x10 */ s32 stepB;
    /* 0x14 */ s32 valueB;   /* 0..255 (<< 16) */
} RumbleChannel;

/* A command list entry: cmd & 0xF000 == 0: set channel 4 (b0, b1, frames = cmd & 0xFFF);
 * 0x4000: jump to entry cmd & 0xFFF; any other high bits: end. */
typedef struct RumbleCmd {
    u8 b0;
    u8 b1;
    u16 cmd;
} RumbleCmd;

typedef struct Rumble {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ u8 enabled;
    /* 0x05 */ u8 unk5;
    /* 0x08 */ RumbleChannel ch[5];
    /* 0x80 */ const RumbleCmd *listB;
    /* 0x84 */ const RumbleCmd *listA;
    /* 0x88 */ s32 posB;
    /* 0x8C */ s32 posA;
} Rumble;

/* rumble.c */
extern void *func_002D4630(Rumble *f);   /* rumble constructor (rumble.c) */
extern Rumble *func_0020DF90(Rumble *f, s32 flags);
extern void func_002D42A0(Rumble *f);   /* fader tick */

#endif /* RUMBLE_H */
