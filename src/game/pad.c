/* Controller input (system +0x40, vtable 0x46ADB0, global gPad), on top of libpad2
 * (replaced on PC by native/platform/pad.c).
 *
 * (was rumble.c) Controller rumble (system +0x300, vtable 0x46F4F0, global gRumble): five
 * channels, each with a small-motor value A (on: 1.0 in 16.16) and a large-motor strength B
 * (0..255 in 16.16) held for a number of frames; channel 4 can also follow two command lists.
 * Each frame the strongest values go to the pad manager (system +0x40, +0xC).
 */
#include "common.h"
#include "game.h"
#include "globals.h"
#include "pad.h"
#include "sce/iop.h"
#include "sce/libpad2.h"
#include "sce/libvu0.h"
#include "input.h"
#include "progress.h"
#include "ptmf.h"
#include "memcard.h"
#include "navmesh.h"
#include "daniella.h"
#include "loader.h"
#include "vecmath.h"
#include "scene.h"
#include "scene_boot.h"
#include "scene_game.h"
#include "scene_title.h"
#include "sound.h"
#include "system.h"
#include "text.h"
#include "libc.h"
#include "msl.h"
#include "actor.h"
#include "renderer.h"
#include "cri/adx.h"
#include "sce/eekernel.h"
#include "sce/intc.h"
#include "sce/libmc.h"
#include "sce/sif.h"
#include "ps2hw.h"

extern const char str_DS2O_S1_IRX[];                     /* pad IOP module */

extern void *Pads_vtable[], *D_0046ADC4[], *D_0046ADD0[], *D_0046AD88[];
void *Pads_dtor(u8 *o, s32 flags);

extern void *Rumble_vtable[], *D_0046AE30[];
#define RUMBLE_VCALL(f, off, type) ((type)(f)->vtbl[(off) / 4])

void Pads_Shutdown(u8 *pads);

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

void *RumbleBase_dtor(u8 *o, s32 flags);
void Rumble_Reset(Rumble *f);
void Rumble_Clear(Rumble *f);
void Rumble_SetA(Rumble *f, s32 c, s32 on, s32 time);
void Rumble_SetB(Rumble *f, s32 c, s32 v, s32 time);
void Rumble_MoveB(Rumble *f, s32 c, s32 from, s32 to, s32 time);
void Rumble_StartLists(Rumble *f, const RumbleCmd *a, const RumbleCmd *b);
s32 Rumble_ListRunning(Rumble *f);
void Rumble_Enable(Rumble *f, s32 on);
void Rumble_Set2C(Rumble *f, s32 v);

/* Game +0x69B00's destructor: its vtables (and its +0x18 member's), gPad cleared */
/* 0x001BE150 */
void *Pads_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = Pads_vtable;
    AT(o, 0x18, void **) = D_0046ADC4;
    AT(o, 0x18, void **) = D_0046AD88;
    AT(o, 0x0, void **) = D_0046ADD0;
    gPad = NULL;
    if ((s16)flags > 0) {
        func_00100490(o);
    }
    return o;
}
/* init: libpad2, its IOP module, a socket for port 0 */
/* 0x001BE6A0 */
void Pads_Init(u8 *pads) {
    u8 *p = pads + 0x40;

    func_001EF990(0);
    AT(p, 0x4, s32) = func_001BC0F0(pads + 0x18, str_DS2O_S1_IRX, 0, 0, 0);
    AT(p, 0x0, s32) = func_001EFA38(0, p + 0x140);
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x244, s32) = -1;
    AT(p, 0x24B, u8) = 0;
    AT(p, 0x24A, u8) = 0;
    AT(p, 0x249, u8) = 0;
    AT(p, 0x248, u8) = 0;
}

/* destructor (vtable D_0046AE30) */
/* 0x001BF280 */
void *RumbleBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AE30;
        gRumble = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* +0x8 destructor */
/* 0x0020DF90 */
Rumble *Rumble_dtor(Rumble *f, s32 flags) {
    if (f != NULL) {
        f->vtbl = Rumble_vtable;
        f->vtbl = D_0046AE30;
        gRumble = NULL;
        if ((s16)flags > 0) {
            func_00100490(f);
        }
    }
    return f;
}

/* +0x24 a command list is running */
/* 0x002D3FA0 */
s32 Rumble_ListRunning(Rumble *f) {
    return f->listB != NULL || f->listA != NULL;
}

/* +0x2C */
/* 0x002D3FC0 */
void Rumble_Set2C(Rumble *f, s32 v) {
    f->unk5 = v;
    if ((u8)v == 1) {
        VCALL(gPad, 0xC, void (*)(VObject *, s32, s32, s32))(gPad, 0, 0, 0);
    }
}

/* +0x28 enable / disable (disabling stops the motors) */
/* 0x002D4020 */
void Rumble_Enable(Rumble *f, s32 on) {
    f->enabled = on;
    if (!(u8)on) {
        VCALL(gPad, 0xC, void (*)(VObject *, s32, s32, s32))(gPad, 0, 0, 0);
    }
}

/* +0x20 start the command lists (A: channel 4 value A, B: channel 4 value B) */
/* 0x002D4070 */
void Rumble_StartLists(Rumble *f, const RumbleCmd *a, const RumbleCmd *b) {
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

/* +0x1C move value B of channel `c` from `from` to `to` over `time` frames */
/* 0x002D41B0 */
void Rumble_MoveB(Rumble *f, s32 c, s32 from, s32 to, s32 time) {
    RumbleChannel *ch = &f->ch[(u8)c];
    s32 n = (u16)time & 0xFFF;

    ch->timeB = n;
    ch->valueB = (u8)from << 16;
    ch->stepB = time != 0 ? (((u8)to << 16) - ch->valueB) / n : 0;
}

/* +0x18 set value B of channel `c` to `v` for `time` frames */
/* 0x002D4220 */
void Rumble_SetB(Rumble *f, s32 c, s32 v, s32 time) {
    RumbleChannel *ch = &f->ch[(u8)c];

    ch->timeB = (u16)time & 0xFFF;
    ch->stepB = 0;
    ch->valueB = (u8)v << 16;
}

/* +0x14 set value A of channel `c` (on: 1.0, off: 0) for `time` frames */
/* 0x002D4260 */
void Rumble_SetA(Rumble *f, s32 c, s32 on, s32 time) {
    RumbleChannel *ch = &f->ch[(u8)c];

    ch->timeA = (u16)time & 0xFFF;
    ch->stepA = 0;
    ch->valueA = on ? 0x10000 : 0;
}

/* per-frame tick: run the channels, send the strongest small / large motor values */
/* 0x002D42A0 */
void Rumble_Tick(Rumble *f) {
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

/* +0x10 clear all channels and lists */
/* 0x002D45A0 */
void Rumble_Clear(Rumble *f) {
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

/* +0xC reset: clear (vtable +0x10), enabled */
/* 0x002D45F0 */
void Rumble_Reset(Rumble *f) {
    RUMBLE_VCALL(f, 0x10, void (*)(Rumble *))(f);
    f->enabled = 1;
    f->unk5 = 0;
}

/* constructor: register, reset (vtable +0xC) */
/* 0x002D4630 */
void *Rumble_ctor(Rumble *f) {
    f->vtbl = Rumble_vtable;
    gRumble = (VObject *)f;
    RUMBLE_VCALL(f, 0xC, void (*)(Rumble *))(f);
    return f;
}

/* port state (system +0x40 +0x40): socket, module, libpad2 state, read phase, data, profile,
 * actuator (rumble) values */
#define PAD_SOCKET(p) AT(p, 0x0, s32)
#define PAD_STATE(p) AT(p, 0x8, s32)
#define PAD_PHASE(p) AT(p, 0xC, s32)
#define PAD_ACTCOUNT(p) AT(p, 0x244, s32)

void Pads_BuildInput(u8 *pads);   /* input state from the pad data (below) */

/* +0xC set the motors of port `port` (only port 0): small on / off, large strength */
/* 0x001BE1F0 */
void Pads_SetMotors(u8 *pads, s32 port, s32 small, s32 large) {
    s32 n = (u8)port;

    if (n <= 0) {
        AT(pads, 0x288 + n * 0x280, u8) = small != 0;
        AT(pads, 0x289 + n * 0x280, s8) = large;
    }
}

/* per-frame tick: read the pad (profile first), send changed motor values, update the input */
/* 0x001BE4B0 */
void Pads_Tick(u8 *pads) {
    u8 *p = pads + 0x40;
    s32 bytes = 0, data = 0, changed = 1;
    u8 m;

    PAD_STATE(p) = func_001EFD40(PAD_SOCKET(p));
    switch (PAD_STATE(p)) {
    case 2:
        break;
    case 1:
        if (PAD_PHASE(p) == 0) {
            func_001EFC70(PAD_SOCKET(p), p + 0x110);
            PAD_PHASE(p) = 1;
        } else {
            func_001EFB98(PAD_SOCKET(p), p + 0x10);
            PAD_PHASE(p) = 2;
        }
        break;
    default:
        PAD_PHASE(p) = 0;
        break;
    }
    if (AT(p, 0x248, u8) == AT(p, 0x24A, u8) && AT(p, 0x249, u8) == AT(p, 0x24B, u8)) {
        changed = 0;
    }
    if (PAD_STATE(p) == 1 && PAD_PHASE(p) == 2) {
        PAD_ACTCOUNT(p) = func_002D25D8(PAD_SOCKET(p), p + 0x242);
        if (PAD_ACTCOUNT(p) > 0) {
            m = AT(p, 0x242, u8);
            if (m & 1) {
                bytes = 1;
                data = AT(p, 0x248, u8) & 1;
            }
            if (m & 2) {
                data = (data | ((AT(p, 0x249, u8) << bytes) & 0xFFFF)) & 0xFFFF;
                bytes += 8;
            }
            bytes = (bytes + 7) / 8;
        }
    } else {
        PAD_ACTCOUNT(p) = -1;
    }
    if (bytes != 0 && changed) {
        AT(p, 0x240, u8) = data;
        AT(p, 0x241, u8) = (u16)data >> 8;
        func_002D2658(PAD_SOCKET(p), PAD_ACTCOUNT(p), p + 0x242, bytes, p + 0x240);
        AT(p, 0x24A, u8) = AT(p, 0x248, u8);
        AT(p, 0x24B, u8) = AT(p, 0x249, u8);
    }
    Pads_BuildInput(pads);
}

/* the pad state the game reads (+0x10): buttons (pressed = 1) and 16 analog bytes (sticks RX, RY,
 * LX, LY, then the button pressures) */
typedef struct PadData {
    u16 buttons;
    u8 pad2[2];
    u8 analog[16];
} PadData;

/* +0x10 port `port`'s pad data (only port 0): 0 when read, 2 no pad, 1 busy, -1 no such port */
/* 0x001BE220 */
s32 Pads_GetData(u8 *pads, s32 port, PadData *out) {
    u8 *p;
    u32 amask;
    s8 i;

    if ((u8)port > 0) {
        return -1;
    }
    p = pads + (u8)port * 0x280 + 0x40;
    if (PAD_STATE(p) != 1) {
        return PAD_STATE(p) != 0 ? 1 : 2;
    }
    if (PAD_PHASE(p) != 2) {
        return 1;
    }
    if (out != NULL) {
        /* the raw data is active-low; the profile says which buttons / analog bytes exist */
        out->buttons = AT(p, 0x11, u8) << 8 | AT(p, 0x10, u8);
        out->buttons &= (AT(p, 0x111, u8) << 8 | AT(p, 0x110, u8)) & 0xFFFF;
        out->buttons ^= 0xFFFF;
        amask = (AT(p, 0x113, u8) << 8 | AT(p, 0x112, u8)) & 0xFFFF;
        for (i = 0; i < 16; i++) {
            u32 bit = 1 << i;

            out->analog[i] = AT(p, 0x12 + i, u8) * (bit == (bit & amask));
        }
    }
    return 0;
}

/* ---- the rest of the system object (2026-10-05) ---- */

/* the pads (+0x40): close the socket, end the library */
/* 0x001BE480 */
void Pads_Shutdown(u8 *pads) {
    func_001EFB40(AT(pads, 0x40, s32));
    func_001EF9D0();
}

/* ---- the game's input state (gInput, one per port) ---- */

typedef struct InputState {
    /* 0x00 */ s8 connected;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ u32 held;          /* bit 0-3 up/right/down/left (pad or left stick), 4 cross,
                                   * 5 triangle, 6 L1, 7 R1, 8 circle, 9 square */
    /* 0x08 */ u16 prevHeld;
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ u32 pressed;       /* new presses (directions: with key repeat) */
    /* 0x10 */ u8 repeat[4];      /* direction key repeat counters */
    /* 0x14 */ u16 raw;           /* all buttons, mapped by kButtonMap */
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ u16 prevRaw;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ u16 rawPressed;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ u16 rawReleased;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ u8 analog[16];     /* mapped by kAnalogMap */
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ f32 stickL[4];
    /* 0x50 */ f32 stickR[4];
} InputState;

_Static_assert(sizeof(InputState) == 0x60, "InputState");

extern InputState gInput;
extern const u8 kButtonMap[16];   /* button map */
extern const u8 kAnalogMap[16];   /* analog map */

#define BIT(v, n) (((v) >> (n)) & 1)
#define SETBIT(w, n, b) ((w) = ((w) & ~(1u << (n))) | ((u32)((b) & 1) << (n)))

/* the face buttons into the input state of port `port` (cross, triangle, circle, square) */
/* 0x0037E320 */
void Pads_FaceButtons(u8 *pads, s32 port) {
    u32 b = AT(pads + port * 0x14, 4, u32);
    InputState *in = &gInput + port;

    SETBIT(in->held, 4, BIT(b, 14));
    SETBIT(in->held, 5, BIT(b, 12));
    SETBIT(in->held, 8, BIT(b, 13));
    SETBIT(in->held, 9, BIT(b, 15));
}

static inline void Input_Clear(InputState *in) {
    s32 i;

    *(u16 *)&in->held = 0;
    in->prevHeld = 0;
    *(u16 *)&in->pressed = 0;
    in->raw = 0;
    in->prevRaw = 0;
    in->rawPressed = 0;
    in->rawReleased = 0;
    for (i = 0; i < 16; i++) {
        in->analog[i] = 0;
    }
    in->stickR[0] = 0;
    in->stickL[0] = 0;
    in->stickR[1] = 0;
    in->stickL[1] = 0;
    in->stickR[2] = 0;
    in->stickL[2] = 0;
    in->stickR[3] = 0;
    in->stickL[3] = 0;
}

/* a stick (x, y bytes): dead zone of 32 around the centre, scaled to about -1..1 */
static inline void Input_Stick(f32 *out, u8 bx, u8 by) {
    f32 v[4] __attribute__((aligned(16)));
    f32 x, y, len = 0.0f;

    v[0] = (f32)bx - 127.0f;
    v[1] = 0.0f;
    v[3] = 1.0f;
    v[2] = (f32)by - 127.0f;
    x = v[0];
    if (x <= 32.0f && !(x < -32.0f)) {
        x = 0.0f;
    }
    v[0] = x / 128.0f;
    y = v[2];
    if (y <= 32.0f && !(y < -32.0f)) {
        y = 0.0f;
    }
    v[2] = y / 128.0f;
    if (!(v[0] == 0.0f && v[2] == 0.0f)) {
        len = __builtin_sqrtf(v[2] * v[2] + v[0] * v[0]);
    }
    sceVu0Normalize(v, v);
    func_0010E640(out, v, len);
}

/* Build the input state from port 0's pad data (pad manager +0x10). */
/* 0x002D4780 */
void Pads_BuildInput(u8 *pads) {
    InputState *in = &gInput;
    s32 wasConnected = in->connected != 0;
    PadData *d = (PadData *)(pads + 4);
    u32 b, newly;
    s32 i;

    switch (VCALL(pads, 0x10, s32 (*)(u8 *, s32, PadData *))(pads, 0, d)) {
    case 0:
        break;
    case 1:   /* busy: keep the last state if there was one */
        in->connected = wasConnected;
        if (wasConnected) {
            return;
        }
        in->connected = 0;
        Input_Clear(in);
        return;
    default:  /* no pad */
        in->connected = 0;
        Input_Clear(in);
        return;
    }
    in->connected = 1;
    in->prevHeld = (u16)in->held;
    *(u16 *)&in->held = 0;
    b = AT(pads, 4, u32);
    SETBIT(in->held, 0, BIT(b, 4) | (d->analog[3] < 0x40));
    SETBIT(in->held, 1, BIT(b, 5) | (d->analog[2] >= 0xC1));
    SETBIT(in->held, 3, BIT(b, 7) | (d->analog[2] < 0x40));
    SETBIT(in->held, 2, BIT(b, 6) | (d->analog[3] >= 0xC1));
    Pads_FaceButtons(pads, 0);
    SETBIT(in->held, 6, BIT(AT(pads, 4, u32), 10));
    *(u16 *)&in->pressed = 0;
    SETBIT(in->held, 7, BIT(AT(pads, 4, u32), 11));
    newly = (u16)in->held & ((u16)in->held ^ in->prevHeld);
    for (i = 0; i < 4; i++) {
        u32 bit = (1 << i) & 0xFF;

        if (newly & bit) {
            in->repeat[i] = 12;
            *(u16 *)&in->pressed |= bit;
        } else if ((u16)in->held & bit) {
            if (--in->repeat[i] == 0) {
                in->repeat[i] = 2;
                *(u16 *)&in->pressed |= bit;
            }
        }
    }
    in->prevRaw = in->raw;
    SETBIT(in->pressed, 4, BIT(newly, 4));
    SETBIT(in->pressed, 5, BIT(newly, 5));
    SETBIT(in->pressed, 6, BIT(newly, 6));
    SETBIT(in->pressed, 7, BIT(newly, 7));
    SETBIT(in->pressed, 8, BIT(newly, 8));
    SETBIT(in->pressed, 9, BIT(newly, 9));
    {
        u16 raw = 0;

        for (i = 0; i < 16; i++) {
            raw |= ((1 << kButtonMap[i] & d->buttons) != 0) << i;
        }
        in->raw = raw;
    }
    in->rawPressed = in->raw & (in->raw ^ in->prevRaw);
    in->rawReleased = ~in->raw & (in->raw ^ in->prevRaw);
    for (i = 0; i < 16; i++) {
        in->analog[i] = d->analog[kAnalogMap[i]];
    }
    Input_Stick(in->stickL, in->analog[2], in->analog[3]);
    Input_Stick(in->stickR, in->analog[0], in->analog[1]);
}

_Static_assert(__builtin_offsetof(Rumble, listB) == 0x80, "Rumble.listB");
