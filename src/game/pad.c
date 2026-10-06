/* Controller input (system +0x40, vtable 0x46ADB0, global gPad), on top of libpad2
 * (replaced on PC by native/platform/pad.c). */
#include "common.h"
#include "game.h"


extern s32 func_001EF990(s32 mode);                 /* libpad2: init */
extern s32 func_001EFA38(s32 port, void *buffer);   /* libpad2: create a socket for port, DMA buffer */
extern s32 func_001BC0F0(void *iop, const char *module, s32, s32, s32);   /* IOP: load a module */
extern const char D_0044FEA0[];                     /* pad IOP module */

/* init: libpad2, its IOP module, a socket for port 0 */
void func_001BE6A0(u8 *pads) {
    u8 *p = pads + 0x40;

    func_001EF990(0);
    AT(p, 0x4, s32) = func_001BC0F0(pads + 0x18, D_0044FEA0, 0, 0, 0);
    AT(p, 0x0, s32) = func_001EFA38(0, p + 0x140);
    AT(p, 0x8, s32) = 0;
    AT(p, 0xC, s32) = 0;
    AT(p, 0x244, s32) = -1;
    AT(p, 0x24B, u8) = 0;
    AT(p, 0x24A, u8) = 0;
    AT(p, 0x249, u8) = 0;
    AT(p, 0x248, u8) = 0;
}

/* port state (system +0x40 +0x40): socket, module, libpad2 state, read phase, data, profile,
 * actuator (rumble) values */
#define PAD_SOCKET(p) AT(p, 0x0, s32)
#define PAD_STATE(p) AT(p, 0x8, s32)
#define PAD_PHASE(p) AT(p, 0xC, s32)
#define PAD_ACTCOUNT(p) AT(p, 0x244, s32)

extern s32 func_001EFD40(s32 socket);                 /* libpad2: state (1 = ready) */
extern s32 func_001EFC70(s32 socket, void *profile);  /* libpad2: button profile */
extern s32 func_001EFB98(s32 socket, void *data);     /* libpad2: read */
extern s32 func_002D25D8(s32 socket, void *mask);     /* actuators (libdbc): count, mask */
extern void func_002D2658(s32 socket, s32 count, void *mask, s32 bytes, void *data);   /* set actuators (libdbc) */
void func_002D4780(u8 *pads);   /* input state from the pad data (below) */

/* +0xC set the motors of port `port` (only port 0): small on / off, large strength */
void func_001BE1F0(u8 *pads, s32 port, s32 small, s32 large) {
    s32 n = (u8)port;

    if (n <= 0) {
        AT(pads, 0x288 + n * 0x280, u8) = small != 0;
        AT(pads, 0x289 + n * 0x280, s8) = large;
    }
}

/* per-frame tick: read the pad (profile first), send changed motor values, update the input */
void func_001BE4B0(u8 *pads) {
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
    func_002D4780(pads);
}

/* the pad state the game reads (+0x10): buttons (pressed = 1) and 16 analog bytes (sticks RX, RY,
 * LX, LY, then the button pressures) */
typedef struct PadData {
    u16 buttons;
    u8 pad2[2];
    u8 analog[16];
} PadData;

/* +0x10 port `port`'s pad data (only port 0): 0 when read, 2 no pad, 1 busy, -1 no such port */
s32 func_001BE220(u8 *pads, s32 port, PadData *out) {
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

/* ---- the game's input state (D_0047E360, one per port) ---- */

typedef struct InputState {
    /* 0x00 */ s8 connected;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ u32 held;          /* bit 0-3 up/right/down/left (pad or left stick), 4 cross,
                                   * 5 triangle, 6 L1, 7 R1, 8 circle, 9 square */
    /* 0x08 */ u16 prevHeld;
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ u32 pressed;       /* new presses (directions: with key repeat) */
    /* 0x10 */ u8 repeat[4];      /* direction key repeat counters */
    /* 0x14 */ u16 raw;           /* all buttons, mapped by D_0047E3C0 */
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ u16 prevRaw;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ u16 rawPressed;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ u16 rawReleased;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ u8 analog[16];     /* mapped by D_0047E3D0 */
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ f32 stickL[4];
    /* 0x50 */ f32 stickR[4];
} InputState;

_Static_assert(sizeof(InputState) == 0x60, "InputState");

extern InputState D_0047E360;
extern const u8 D_0047E3C0[16];   /* button map */
extern const u8 D_0047E3D0[16];   /* analog map */
extern void sceVu0Normalize(f32 *out, f32 *v);
extern void func_0010E640(f32 *out, const f32 *v, f32 s);   /* libvu0: scale x, y, z */

#define BIT(v, n) (((v) >> (n)) & 1)
#define SETBIT(w, n, b) ((w) = ((w) & ~(1u << (n))) | ((u32)((b) & 1) << (n)))

/* the face buttons into the input state of port `port` (cross, triangle, circle, square) */
void func_0037E320(u8 *pads, s32 port) {
    u32 b = AT(pads + port * 0x14, 4, u32);
    InputState *in = &D_0047E360 + port;

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
void func_002D4780(u8 *pads) {
    InputState *in = &D_0047E360;
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
    func_0037E320(pads, 0);
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
            raw |= ((1 << D_0047E3C0[i] & d->buttons) != 0) << i;
        }
        in->raw = raw;
    }
    in->rawPressed = in->raw & (in->raw ^ in->prevRaw);
    in->rawReleased = ~in->raw & (in->raw ^ in->prevRaw);
    for (i = 0; i < 16; i++) {
        in->analog[i] = d->analog[D_0047E3D0[i]];
    }
    Input_Stick(in->stickL, in->analog[2], in->analog[3]);
    Input_Stick(in->stickR, in->analog[0], in->analog[1]);
}
