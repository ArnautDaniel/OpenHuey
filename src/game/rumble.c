/* Controller rumble (system +0x300, vtable 0x46F4F0, global gRumble): five channels, each
 * with a small-motor value A (on: 1.0 in 16.16) and a large-motor strength B (0..255 in 16.16)
 * held for a number of frames; channel 4 can also follow two command lists. Each frame the
 * strongest values go to the pad manager (system +0x40, +0xC). */
#include "common.h"
#include "game.h"
#include "globals.h"
#include "memcard.h"
#include "navmesh.h"

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

_Static_assert(__builtin_offsetof(Rumble, listB) == 0x80, "Rumble.listB");

#define RUMBLE_VCALL(f, off, type) ((type)(f)->vtbl[(off) / 4])

extern void *D_0046F4F0[], *D_0046AE30[];
extern void func_00100490(void *p);   /* operator delete */

void *func_001BF280(u8 *o, s32 flags);

/* destructor (vtable D_0046AE30) */
void *func_001BF280(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AE30;
        gRumble = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}
/* constructor: register, reset (vtable +0xC) */
void *func_002D4630(Rumble *f) {
    f->vtbl = D_0046F4F0;
    gRumble = (VObject *)f;
    RUMBLE_VCALL(f, 0xC, void (*)(Rumble *))(f);
    return f;
}

/* +0x8 destructor */
Rumble *func_0020DF90(Rumble *f, s32 flags) {
    if (f != NULL) {
        f->vtbl = D_0046F4F0;
        f->vtbl = D_0046AE30;
        gRumble = NULL;
        if ((s16)flags > 0) {
            func_00100490(f);
        }
    }
    return f;
}

/* +0xC reset: clear (vtable +0x10), enabled */
void func_002D45F0(Rumble *f) {
    RUMBLE_VCALL(f, 0x10, void (*)(Rumble *))(f);
    f->enabled = 1;
    f->unk5 = 0;
}

/* +0x10 clear all channels and lists */
void func_002D45A0(Rumble *f) {
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
void func_002D4260(Rumble *f, s32 c, s32 on, s32 time) {
    RumbleChannel *ch = &f->ch[(u8)c];

    ch->timeA = (u16)time & 0xFFF;
    ch->stepA = 0;
    ch->valueA = on ? 0x10000 : 0;
}

/* +0x18 set value B of channel `c` to `v` for `time` frames */
void func_002D4220(Rumble *f, s32 c, s32 v, s32 time) {
    RumbleChannel *ch = &f->ch[(u8)c];

    ch->timeB = (u16)time & 0xFFF;
    ch->stepB = 0;
    ch->valueB = (u8)v << 16;
}

/* +0x1C move value B of channel `c` from `from` to `to` over `time` frames */
void func_002D41B0(Rumble *f, s32 c, s32 from, s32 to, s32 time) {
    RumbleChannel *ch = &f->ch[(u8)c];
    s32 n = (u16)time & 0xFFF;

    ch->timeB = n;
    ch->valueB = (u8)from << 16;
    ch->stepB = time != 0 ? (((u8)to << 16) - ch->valueB) / n : 0;
}

/* +0x20 start the command lists (A: channel 4 value A, B: channel 4 value B) */
void func_002D4070(Rumble *f, const RumbleCmd *a, const RumbleCmd *b) {
    const RumbleCmd *e;

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
            RUMBLE_VCALL(f, 0x14, void (*)(Rumble *, s32, s32, s32))(f, 4, e->b0 != 0, e->cmd);
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
            RUMBLE_VCALL(f, 0x1C, void (*)(Rumble *, s32, s32, s32, s32))(f, 4, e->b0, e->b1, e->cmd);
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
s32 func_002D3FA0(Rumble *f) {
    return f->listB != NULL || f->listA != NULL;
}

/* +0x28 enable / disable (disabling stops the motors) */
void func_002D4020(Rumble *f, s32 on) {
    f->enabled = on;
    if (!(u8)on) {
        VCALL(gPad, 0xC, void (*)(VObject *, s32, s32, s32))(gPad, 0, 0, 0);
    }
}

/* +0x2C */
void func_002D3FC0(Rumble *f, s32 v) {
    f->unk5 = v;
    if ((u8)v == 1) {
        VCALL(gPad, 0xC, void (*)(VObject *, s32, s32, s32))(gPad, 0, 0, 0);
    }
}

/* advance a channel's value: held while its frames last, then off */
static inline void RumbleChannel_Step(s16 *time, s32 *step, s32 *value) {
    if ((u16)(*time)-- > 0) {
        *value += *step;
        if (*value > 0) {
            return;
        }
        *step = 0;
        *value = 0;
    } else {
        *step = 0;
        *value = 0;
    }
    *time = 0;
}

/* channel 4: same, but it only bottoms out at 0; when its frames are up, the next command of
 * the list (if any) */
static inline s32 Rumble_StepList(Rumble *f, s16 *time, s32 *step, s32 *value, const RumbleCmd **list, s32 *pos, s32 isB) {
    const RumbleCmd *e;

    if ((u16)(*time)-- > 0) {
        *value += *step;
        if (*value <= 0) {
            *value = 0;
        }
        return *value;
    }
    *step = 0;
    *value = 0;
    *time = 0;
    if (*list != NULL) {
        (*pos)++;
        for (;;) {
            if (*list == NULL) {
                *list = NULL;
                *pos = 0;
                break;
            }
            e = &(*list)[*pos];
            if (!(e->cmd & 0xF000)) {
                if (isB) {
                    RUMBLE_VCALL(f, 0x1C, void (*)(Rumble *, s32, s32, s32, s32))(f, 4, e->b0, e->b1, e->cmd);
                } else {
                    RUMBLE_VCALL(f, 0x14, void (*)(Rumble *, s32, s32, s32))(f, 4, e->b0 != 0, e->cmd);
                }
                break;
            }
            if (!(e->cmd & 0x4000)) {
                *list = NULL;
                *pos = 0;
                break;
            }
            *pos = e->cmd & 0xFFF;
        }
    }
    return *value;
}

/* per-frame tick: run the channels, send the strongest small / large motor values */
void func_002D42A0(Rumble *f) {
    s32 maxA = 0, maxB = 0, i;

    if (!f->enabled) {
        return;
    }
    for (i = 0; i < 4; i++) {
        RumbleChannel *c = &f->ch[i];

        RumbleChannel_Step(&c->timeA, &c->stepA, &c->valueA);
        maxA = maxA < c->valueA ? c->valueA : maxA;
        RumbleChannel_Step(&c->timeB, &c->stepB, &c->valueB);
        maxB = maxB < c->valueB ? c->valueB : maxB;
    }
    if (f->unk5 == 0) {
        RumbleChannel *c = &f->ch[4];
        s32 v;

        v = Rumble_StepList(f, &c->timeA, &c->stepA, &c->valueA, &f->listA, &f->posA, 0);
        maxA = maxA < v ? v : maxA;
        v = Rumble_StepList(f, &c->timeB, &c->stepB, &c->valueB, &f->listB, &f->posB, 1);
        maxB = maxB < v ? v : maxB;
    }
    VCALL(gPad, 0xC, void (*)(VObject *, s32, s32, s32))(gPad, 0, (maxA >> 16) & 0xFF, (maxB >> 16) & 0xFF);
}
