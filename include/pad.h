#ifndef PAD_H
#define PAD_H

/* pad.c: what other files call. */
#include "common.h"

/* pad.c */
extern void Pads_Init(u8 *pads);
extern void Pads_Tick(u8 *pads);   /* pad tick */

/* ---- (was rumble.h) ---- */

/* rumble.c: what other files call. */

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
extern void *Rumble_ctor(Rumble *f);   /* rumble constructor (rumble.c) */
extern Rumble *Rumble_dtor(Rumble *f, s32 flags);
extern void Rumble_Tick(Rumble *f);   /* fader tick */

extern void Pads_Shutdown(u8 *pads);

#endif /* PAD_H */
