/* Fader (system +0x300, vtable 0x46F4F0, global D_0044E7A8): five channels, each with two
 * values (16.16 fixed point) that move linearly to a target over a number of frames, plus two
 * command lists that drive channel 4. Probably screen fades / tints. */
#include "common.h"
#include "game.h"

typedef struct FadeChannel {
    /* 0x00 */ s16 timeA;
    /* 0x04 */ s32 stepA;
    /* 0x08 */ s32 valueA;   /* 0 or 1.0 (0x10000) */
    /* 0x0C */ s16 timeB;
    /* 0x10 */ s32 stepB;
    /* 0x14 */ s32 valueB;   /* 0..255 (<< 16) */
} FadeChannel;

/* A command list entry: cmd & 0xF000 == 0: set channel 4 (b0, b1, frames = cmd & 0xFFF);
 * 0x4000: jump to entry cmd & 0xFFF; any other high bits: end. */
typedef struct FadeCmd {
    u8 b0;
    u8 b1;
    u16 cmd;
} FadeCmd;

typedef struct Fader {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ u8 enabled;
    /* 0x05 */ u8 unk5;
    /* 0x08 */ FadeChannel ch[5];
    /* 0x80 */ const FadeCmd *listB;
    /* 0x84 */ const FadeCmd *listA;
    /* 0x88 */ s32 posB;
    /* 0x8C */ s32 posA;
} Fader;

_Static_assert(__builtin_offsetof(Fader, listB) == 0x80, "Fader.listB");

#define FADER_VCALL(f, off, type) ((type)(f)->vtbl[(off) / 4])

extern void *D_0046F4F0[], *D_0046AE30[];
extern Fader *D_0044E7A8;      /* the fader */
extern VObject *D_0044FEB0;    /* system +0x40 */
extern void func_00100490(void *p);   /* operator delete */

/* constructor: register, reset (vtable +0xC) */
void *func_002D4630(Fader *f) {
    f->vtbl = D_0046F4F0;
    D_0044E7A8 = f;
    FADER_VCALL(f, 0xC, void (*)(Fader *))(f);
    return f;
}

/* +0x8 destructor */
Fader *func_0020DF90(Fader *f, s32 flags) {
    if (f != NULL) {
        f->vtbl = D_0046F4F0;
        f->vtbl = D_0046AE30;
        D_0044E7A8 = NULL;
        if ((s16)flags > 0) {
            func_00100490(f);
        }
    }
    return f;
}

/* +0xC reset: clear (vtable +0x10), enabled */
void func_002D45F0(Fader *f) {
    FADER_VCALL(f, 0x10, void (*)(Fader *))(f);
    f->enabled = 1;
    f->unk5 = 0;
}

/* +0x10 clear all channels and lists */
void func_002D45A0(Fader *f) {
    s32 i;

    for (i = 0; i < 5; i++) {
        f->ch[i].stepA = 0;
        f->ch[i].valueA = 0;
        f->ch[i].timeA = 0;
        f->ch[i].stepB = 0;
        f->ch[i].valueB = 0;
        f->ch[i].timeB = 0;
    }
    f->listB = NULL;
    f->listA = NULL;
    f->posA = 0;
    f->posB = 0;
}

/* +0x14 set value A of channel `c` (on: 1.0, off: 0) for `time` frames */
void func_002D4260(Fader *f, s32 c, s32 on, s32 time) {
    FadeChannel *ch = &f->ch[(u8)c];

    ch->timeA = (u16)time & 0xFFF;
    ch->stepA = 0;
    ch->valueA = on ? 0x10000 : 0;
}

/* +0x18 set value B of channel `c` to `v` for `time` frames */
void func_002D4220(Fader *f, s32 c, s32 v, s32 time) {
    FadeChannel *ch = &f->ch[(u8)c];

    ch->timeB = (u16)time & 0xFFF;
    ch->stepB = 0;
    ch->valueB = (u8)v << 16;
}

/* +0x1C move value B of channel `c` from `from` to `to` over `time` frames */
void func_002D41B0(Fader *f, s32 c, s32 from, s32 to, s32 time) {
    FadeChannel *ch = &f->ch[(u8)c];
    s32 n = (u16)time & 0xFFF;

    ch->timeB = n;
    ch->valueB = (u8)from << 16;
    ch->stepB = time != 0 ? (((u8)to << 16) - ch->valueB) / n : 0;
}

/* +0x20 start the command lists (A: channel 4 value A, B: channel 4 value B) */
void func_002D4070(Fader *f, const FadeCmd *a, const FadeCmd *b) {
    const FadeCmd *e;

    f->posA = 0;
    f->posB = 0;
    f->listA = a;
    f->listB = b;
    for (;;) {
        if (f->listA == NULL) {
            f->listA = NULL;
            f->posA = 0;
            break;
        }
        e = &f->listA[f->posA];
        if (!(e->cmd & 0xF000)) {
            FADER_VCALL(f, 0x14, void (*)(Fader *, s32, s32, s32))(f, 4, e->b0 != 0, e->cmd);
            break;
        }
        if (!(e->cmd & 0x4000)) {
            f->listA = NULL;
            f->posA = 0;
            break;
        }
        f->posA = e->cmd & 0xFFF;
    }
    for (;;) {
        if (f->listB == NULL) {
            f->listB = NULL;
            f->posB = 0;
            break;
        }
        e = &f->listB[f->posB];
        if (!(e->cmd & 0xF000)) {
            FADER_VCALL(f, 0x1C, void (*)(Fader *, s32, s32, s32, s32))(f, 4, e->b0, e->b1, e->cmd);
            break;
        }
        if (!(e->cmd & 0x4000)) {
            f->listB = NULL;
            f->posB = 0;
            break;
        }
        f->posB = e->cmd & 0xFFF;
    }
}

/* +0x24 a command list is running */
s32 func_002D3FA0(Fader *f) {
    return f->listB != NULL || f->listA != NULL;
}

/* +0x28 enable / disable (disabling notifies system +0x40) */
void func_002D4020(Fader *f, s32 on) {
    f->enabled = on;
    if (!(u8)on) {
        VCALL(D_0044FEB0, 0xC, void (*)(VObject *, s32, s32, s32))(D_0044FEB0, 0, 0, 0);
    }
}

/* +0x2C */
void func_002D3FC0(Fader *f, s32 v) {
    f->unk5 = v;
    if ((u8)v == 1) {
        VCALL(D_0044FEB0, 0xC, void (*)(VObject *, s32, s32, s32))(D_0044FEB0, 0, 0, 0);
    }
}
