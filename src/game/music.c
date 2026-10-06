/* The stage music director (scene +0x1064600, 0xA3C bytes, global D_00456DF0; base vtable
 * D_0046EB70, one subclass per stage set up by func_0039A8E0). It plays the stage's music on
 * the sound driver's four sequence banks (0..3, an SQ each over the stage's HD/BD bank) and
 * mixes them live: each track's volume and sequence volume, and per MIDI channel a volume,
 * pitch bend and pan, sent as MIDI (driver +0x30: command 0x23). Fades run as "cues" (a
 * member function called each frame). Its state (+0x30) follows the chase (func_00177620),
 * the panic level (progress +0x7B8) and whether the pursuer sees Fiona. The tracks: 0 the
 * panic (PANIC.SQ), 1 / 2 the calm music's two parts (Sn_NORMALA / B), 3 the chase. States:
 *   0 start (the calm parts fade in, the others out); 1 calm; 2 calm fading out (being
 *   followed); 3 / 4 the chase begins (+0x54); 5 / 6 the chase (+0x58 lost / +0x5C seen);
 *   7 panic (+0x60); 8 panic, over at progress +0x7B8 == 5 (+0x64); 9 after.
 *
 *   +0x4    the music volume (the system's), +0x8 the progress's, +0xC the director's
 *   +0x10   the global volume fading (16.16), +0x12 its integer part (0..255), +0x14 its
 *           direction
 *   +0x15   the start step (subclass +0xC sets 0)
 *   +0x18   the chase table (8 bytes per step: distance f32, track 0 volume, bend)
 *   +0x1C / +0x20 / +0x24  per track, 16 channel volumes / bends / pans
 *   +0x28   per track its sequence volume, +0x2C per channel 1: bent with the chase
 *   +0x30   the state
 *   +0x34   4 tracks (0x110 each): 16 channels (volume, bend, pan f32, muted), its volume
 *           (+0x100), sequence volume (+0x104) and scale (+0x108), muted (+0x10C)
 *   +0x474  0x18 cues (0x28 each)
 *   +0x834  the cue running
 *   +0x838  log2(1..128)
 *   +0xA38 / +0xA39  flags */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "ptmf.h"
#include "globals.h"
#include "actor.h"

extern void *D_00456DF0;      /* the director */
extern const PTMF sGameStateNull;
extern const PTMF D_004128A0;   /* func_002C25F0: a track volume fade */
extern const PTMF D_00412890;   /* func_002C2ED0: the global volume fade */
extern const PTMF D_004128B0;   /* func_002C1F80: a sequence volume fade */
extern const PTMF D_004128C0;   /* func_002C1AC0: a channel bend fade */
extern void *D_0046EB70[], *D_0046EBE0[];

#define TRACK(d, k) ((u8 *)(d) + 0x34 + ((k) & 0xFF) * 0x110)
#define CHAN(d, k, c) (TRACK(d, k) + ((c) & 0xFF) * 0x10)
#define CUE(d, i) ((u8 *)(d) + 0x474 + (i) * 0x28)
#define CUR(d) AT(d, 0x834, u8 *)

/* (the EE's float -> unsigned conversion) */
static inline u32 f2u(f32 f) {
    return (u32)f;
}

/* MIDI message `st` `d1` `d2` to track `k`'s channel `ch` (0xFF all) */
static inline void midi(u32 k, u8 st, u8 d1, u8 d2, u32 ch) {
    u8 m[4];

    m[0] = d2;
    m[1] = d1;
    m[2] = st;
    VCALL(gSound, 0x30, void (*)(VObject *, u32, u8 *, u32))(gSound, k, m, ch);
}

/* track volume `v` (0..255) as the driver gets it */
static inline u8 vol_out(u8 *d, u32 v) {
    return f2u((AT(d, 0x8, f32) * (AT(d, 0x4, f32) * (AT(d, 0xC, f32) * (f32)(v * AT(d, 0x12, s16))))) / 255.0f);
}

static inline void send_volume(u8 *d, u32 k) {
    VCALL(gSound, 0x34, void (*)(VObject *, u32, u32))(gSound, k, vol_out(d, f2u(AT(TRACK(d, k), 0x100, f32)) & 0xFF));
}

static inline void send_volumes(u8 *d) {
    u8 k;

    for (k = 0; k < 4; k = (k + 1) & 0xFF) {
        send_volume(d, k);
    }
}

/* track `k` silent (its volume 0) */
static inline void track_zero(u8 *d, u32 k) {
    if (f2u(AT(TRACK(d, k), 0x100, f32)) & 0xFF) {
        AT(TRACK(d, k), 0x100, f32) = 0.0f;
        VCALL(gSound, 0x34, void (*)(VObject *, u32, u32))(gSound, k, vol_out(d, 0) & 0xFF);
    }
}

/* track `k` muted: volume 0 and the synth's voices off (driver +0x28: command 0x38 0) */
static inline void track_mute(u8 *d, u32 k) {
    if (AT(TRACK(d, k), 0x10C, u8) == 0) {
        AT(TRACK(d, k), 0x10C, u8) = 1;
        VCALL(gSound, 0x34, void (*)(VObject *, u32, u32))(gSound, k, vol_out(d, 0) & 0xFF);
        VCALL(gSound, 0x28, void (*)(VObject *, u32, u32))(gSound, k, 0);
    }
}

/* track `k` back on */
static inline void track_unmute(u8 *d, u32 k) {
    if (AT(TRACK(d, k), 0x10C, u8) == 1) {
        AT(TRACK(d, k), 0x10C, u8) = 0;
        VCALL(gSound, 0x28, void (*)(VObject *, u32, u32))(gSound, k, 1);
        AT(d, 0xA39, u8) = 1;
    }
}

/* the cues of type `type` on track `k` stop */
static inline void cues_cancel(u8 *d, u32 k, s8 type) {
    s32 i;

    for (i = 0; i < 0x18; i++) {
        u8 *c = CUE(d, i);

        if (AT(c, 0x0, u8) == (k & 0xFF) && AT(c, 0x24, s8) == type && AT(c, 0x1, u8) == 0xFF) {
            AT(c, 0x25, u8) = 1;
        }
    }
}

/* a free cue (0xFF: none - which the callers don't check: a u8 compared with -1; with all 24
   busy they write cue 0xFF, past the director, into the scene's memory) */
static inline u8 cue_free(u8 *d) {
    s32 i;

    for (i = 0; i < 0x18; i++) {
        if (!ptmf_test((PTMF *)(CUE(d, i) + 4))) {
            return i;
        }
    }
    return 0xFF;
}

void func_002C3760(u8 *d, u32 k);

/* ---- cues ---- */

/* a track volume fade (the cue's track, target +0x14, step +0x1C): done at the target (a
   target 0 mutes the track) */
void func_002C25F0(u8 *d) {
    u8 *c = CUR(d);
    u8 k = AT(c, 0x0, u8);
    u32 to = AT(c, 0x14, s32);
    u8 *t = TRACK(d, k);
    f32 diff = AT(t, 0x100, f32) - (f32)(s32)to;
    f32 step = AT(c, 0x1C, f32);

    if (!(diff <= 0.0f)) {
    } else {
        diff = -diff;
    }
    if (diff <= (!(step <= 0.0f) ? step : -step)) {
        u8 v = to & 0xFF;

        if (v != (f2u(AT(t, 0x100, f32)) & 0xFF)) {
            AT(t, 0x100, f32) = (f32)v;
            VCALL(gSound, 0x34, void (*)(VObject *, u32, u32, f32 *))(gSound, k, vol_out(d, v) & 0xFF,
                                                                          (f32 *)(t + 0x100));
        }
        if (AT(CUR(d), 0x14, s32) == 0) {
            track_mute(d, AT(CUR(d), 0x0, u8));
        }
        AT(CUR(d), 0x25, u8) = 1;
        return;
    }
    {
        u8 was = f2u(AT(t, 0x100, f32)) & 0xFF;
        f32 v = AT(t, 0x100, f32) + step;

        AT(t, 0x100, f32) = v;
        v = (v <= 255.0f ? v : 255.0f) < 0.0f ? 0.0f : (v <= 255.0f ? v : 255.0f);
        AT(t, 0x100, f32) = v;
        if (AT(t, 0x10C, u8) != 1) {
            u8 now = f2u(AT(t, 0x100, f32)) & 0xFF;

            if (was != now) {
                VCALL(gSound, 0x34, void (*)(VObject *, u32, u32, f32 *))(gSound, k, vol_out(d, now) & 0xFF,
                                                                              (f32 *)(t + 0x100));
            }
        }
    }
}

/* the global volume fade (+0x10, 16.16, by the cue's +0x18 toward +0x14) */
void func_002C2ED0(u8 *d) {
    s8 dir = AT(d, 0x14, s8);

    if (dir == 1) {
        AT(d, 0x10, s32) += AT(CUR(d), 0x18, s32);
        send_volumes(d);
        if (AT(d, 0x12, s16) >= AT(CUR(d), 0x14, s32)) {
            AT(d, 0x10, s32) = AT(CUR(d), 0x14, s32) << 16;
            send_volumes(d);
            AT(d, 0x14, s8) = 0;
            AT(CUR(d), 0x25, u8) = 1;
        }
    } else if (dir == -1) {
        AT(d, 0x10, s32) -= AT(CUR(d), 0x18, s32);
        send_volumes(d);
        if (AT(CUR(d), 0x14, s32) >= AT(d, 0x12, s16)) {
            AT(d, 0x10, s32) = AT(CUR(d), 0x14, s32) << 16;
            send_volumes(d);
            AT(d, 0x14, s8) = 0;
            AT(CUR(d), 0x25, u8) = 1;
        }
    } else {
        AT(CUR(d), 0x25, u8) = 1;
    }
}

/* the sequence volume of track +0x104 (20..255, x the scale +0x108) */
static inline u8 seqvol_out(f32 v) {
    return f2u(v);
}

/* a sequence volume fade */
void func_002C1F80(u8 *d) {
    u8 *c = CUR(d);
    u8 k = AT(c, 0x0, u8);
    u32 to = AT(c, 0x14, s32);
    u8 *t = TRACK(d, k);
    f32 cur = AT(t, 0x104, f32);
    f32 diff = cur - (f32)(s32)to;
    f32 step = AT(c, 0x1C, f32);

    if (!(diff <= 0.0f)) {
    } else {
        diff = -diff;
    }
    if (diff <= (!(step <= 0.0f) ? step : -step)) {
        u32 v = to & 0xFF;
        u8 was = f2u(cur) & 0xFF;
        f32 f = (f32)v;

        f = (f <= 255.0f ? f : 255.0f) < 20.0f ? 20.0f : (f <= 255.0f ? f : 255.0f);
        AT(t, 0x104, f32) = f;
        if (was != (f2u(AT(t, 0x104, f32)) & 0xFF)) {
            VCALL(gSound, 0x3C, void (*)(VObject *, u32, u32, u32, f32 *))(gSound, k, v, was, (f32 *)(t + 0x104));
        }
        AT(CUR(d), 0x25, u8) = 1;
        return;
    }
    {
        u8 was = f2u(AT(t, 0x104, f32)) & 0xFF;
        f32 v = AT(t, 0x104, f32) + step;
        f32 o;

        AT(t, 0x104, f32) = v;
        v = (v <= 255.0f ? v : 255.0f) < 20.0f ? 20.0f : (v <= 255.0f ? v : 255.0f);
        AT(t, 0x104, f32) = v;
        o = v * AT(t, 0x108, f32);
        o = (o <= 255.0f ? o : 255.0f) < 20.0f ? 20.0f : (o <= 255.0f ? o : 255.0f);
        if (was != (f2u(v) & 0xFF)) {
            VCALL(gSound, 0x3C, void (*)(VObject *, u32, u32, u32, f32 *))(gSound, k, f2u(o) & 0xFF & 0xFF, was,
                                                                               (f32 *)(t + 0x104));
        }
    }
}

/* a channel's bend fade (the cue's track, channel +1) */
void func_002C1AC0(u8 *d) {
    u8 *c = CUR(d);
    u8 k = AT(c, 0x0, u8), ch = AT(c, 0x1, u8);
    s32 to = AT(c, 0x14, s32);
    u8 *e = CHAN(d, k, ch);
    f32 diff = AT(e, 0x4, f32) - (f32)to;
    f32 step = AT(c, 0x1C, f32);

    if (!(diff <= 0.0f)) {
    } else {
        diff = -diff;
    }
    if (!(diff <= (!(step <= 0.0f) ? step : -step))) {
        f32 v = AT(e, 0x4, f32) + step;
        f32 o;

        AT(e, 0x4, f32) = v;
        v = (v <= 127.0f ? v : 127.0f) < 0.0f ? 0.0f : (v <= 127.0f ? v : 127.0f);
        AT(e, 0x4, f32) = v;
        o = v * AT(TRACK(d, k), 0x108, f32);
        o = (o <= 127.0f ? o : 127.0f) < 0.0f ? 0.0f : (o <= 127.0f ? o : 127.0f);
        midi(k, 0xE0, 0, f2u(o) & 0xFF, ch);
        return;
    }
    AT(e, 0x4, f32) = (f32)to;
    midi(AT(CUR(d), 0x0, u8), 0xE0, 0, (s8)AT(CUR(d), 0x14, s32), AT(CUR(d), 0x1, u8));
    AT(CUR(d), 0x25, u8) = 1;
}

/* ---- starting cues ---- */

/* track `k`'s volume to `to` over `frames` frames: 1 if started */
s32 func_002C2CD0(u8 *d, u8 k, s32 frames, s32 to) {
    u8 *t = TRACK(d, k), *c;
    u8 i;

    if (to == (s32)AT(t, 0x100, f32)) {
        return 0;
    }
    i = cue_free(d);
    cues_cancel(d, k, 0);
    c = CUE(d, i);
    AT(c, 0x0, u8) = k;
    to = (to < 0x100 ? to : 0xFF) < 0 ? 0 : (to < 0x100 ? to : 0xFF);
    AT(c, 0x14, s32) = to;
    AT(c, 0x24, s8) = 0;
    *(PTMF *)(c + 4) = D_004128A0;
    AT(c, 0x1C, f32) = ((f32)AT(c, 0x14, s32) - AT(t, 0x100, f32)) / (f32)frames;
    return 1;
}

/* track `k`'s sequence volume to `to` (20..255) over `frames` frames */
s32 func_002C23E0(u8 *d, u8 k, s32 frames, s32 to) {
    u8 *t = TRACK(d, k), *c;
    u8 i;

    if (to == (s32)AT(t, 0x104, f32)) {
        return 0;
    }
    i = cue_free(d);
    cues_cancel(d, k, 4);
    c = CUE(d, i);
    AT(c, 0x0, u8) = k;
    to = (to < 0x100 ? to : 0xFF) < 0x14 ? 0x14 : (to < 0x100 ? to : 0xFF);
    AT(c, 0x14, s32) = to;
    AT(c, 0x24, s8) = 4;
    *(PTMF *)(c + 4) = D_004128B0;
    AT(c, 0x1C, f32) = ((f32)AT(c, 0x14, s32) - AT(t, 0x104, f32)) / (f32)frames;
    return 1;
}

/* track `k` channel `ch`'s bend to `to` (0..127) over `frames` frames */
s32 func_002C1D10(u8 *d, u8 k, u8 ch, s32 frames, u32 to) {
    u8 *c;
    u8 i;
    s32 j;

    i = cue_free(d);
    for (j = 0; j < 0x18; j++) {
        u8 *o = CUE(d, j);

        if (AT(o, 0x0, u8) == (k & 0xFF) && AT(o, 0x24, s8) == 2
            && (AT(o, 0x1, u8) == 0xFF || AT(o, 0x1, u8) == (ch & 0xFF))) {
            AT(o, 0x25, u8) = 1;
        }
    }
    c = CUE(d, i);
    AT(c, 0x0, u8) = k;
    AT(c, 0x1, u8) = ch;
    AT(c, 0x24, s8) = 2;
    to &= 0xFF;
    to = (to < 0x80 ? to : 0x7F) < 0 ? 0 : (to < 0x80 ? to : 0x7F);
    AT(c, 0x14, s32) = to;
    AT(c, 0x1C, f32) = 127.0f / (f32)frames;
    if (AT(c, 0x14, s32) < (s32)(f2u(AT(CHAN(d, k, ch), 0x4, f32)) & 0xFF)) {
        AT(c, 0x1C, f32) = AT(c, 0x1C, f32) * -1.0f;
    }
    *(PTMF *)(c + 4) = D_004128C0;
    return 1;
}

/* track `k`'s volume to `to` at `rate` a frame */
s32 func_002C2A70(u8 *d, u8 k, s32 to, f32 rate) {
    u8 *t = TRACK(d, k), *c;
    u8 i;

    if (to == (s32)AT(t, 0x100, f32)) {
        return 0;
    }
    i = cue_free(d);
    cues_cancel(d, k, 0);
    c = CUE(d, i);
    AT(c, 0x0, u8) = k;
    to = (to < 0x100 ? to : 0xFF) < 0 ? 0 : (to < 0x100 ? to : 0xFF);
    AT(c, 0x14, s32) = to;
    AT(c, 0x24, s8) = 0;
    *(PTMF *)(c + 4) = D_004128A0;
    if (rate * (f32)(AT(c, 0x14, s32) - (s32)(f2u(AT(t, 0x100, f32)) & 0xFF)) < 0.0f) {
        AT(c, 0x1C, f32) = -rate;
    } else {
        AT(c, 0x1C, f32) = rate;
    }
    return 1;
}

/* a 16.16 value (its integer half at +2) */
typedef union Fixed {
    s32 w;
    struct {
        u16 frac;
        s16 i;
    } h;
} Fixed;

/* the global volume to `to` (0..255) over `frames` frames (before the start: just set) */
s32 func_002C32D0(u8 *d, s32 frames, u8 to) {
    u8 *c;
    u8 i;
    Fixed x;
    s32 down;

    if (AT(d, 0x15, u8) < 3) {
        AT(d, 0x31, u8) = to;
        return 0;
    }
    if (to == AT(d, 0x12, s16)) {
        return 0;
    }
    i = cue_free(d);
    cues_cancel(d, 0xFF, 6);
    c = CUE(d, i);
    AT(c, 0x0, u8) = 0xFF;
    AT(c, 0x14, s32) = to;
    AT(c, 0x24, s8) = 6;
    *(PTMF *)(c + 4) = D_00412890;
    x.w = 0;
    x.h.i = to;
    x.w -= AT(d, 0x10, s32);
    down = 0;
    if (x.h.i < 0) {
        x.h.i = -x.h.i;
        down = 1;
    }
    x.w = ((x.w << 7) / frames) >> 7;
    AT(c, 0x18, s32) = x.w;
    AT(d, 0x14, s8) = down ? -1 : 1;
    return 1;
}

/* a frame: the tracks' settings sent again if asked (+0xA39), the state (+0x50), the cues */
void func_002C34C0(u8 *d) {
    u8 k;
    s32 i;

    if (AT(d, 0xA39, u8) != 0) {
        for (k = 0; k < 4; k = (k + 1) & 0xFF) {
            func_002C3760(d, k);
        }
        AT(d, 0xA39, u8) = 0;
    }
    VCALL(d, 0x50, void (*)(u8 *))(d);
    for (i = 0; i < 0x18; i++) {
        u8 *c;

        CUR(d) = CUE(d, i);
        c = CUR(d);
        if (AT(c, 0x25, u8) == 1) {
            AT(c, 0x0, u8) = 0xFF;
            AT(c, 0x1, u8) = 0xFF;
            AT(c, 0x10, s32) = -1;
            *(PTMF *)(c + 4) = sGameStateNull;
            AT(c, 0x14, s32) = 0;
            AT(c, 0x18, s32) = 0;
            AT(c, 0x1C, s32) = 0;
            AT(c, 0x20, s32) = 0;
            AT(c, 0x24, s8) = -1;
            AT(c, 0x25, u8) = 0;
        } else if (ptmf_test((PTMF *)(CUR(d) + 4))) {
            ptmf_scall(d, (PTMF *)(CUR(d) + 4));
        }
    }
}

/* ---- the chase table ---- */

/* the chase step (by the pursuer's distance; 0xFF: none in slot 2) */
u8 func_002C1980(u8 *d) {
    u8 i = 0;
    f32 *tbl;

    if (gCharSlot2 == NULL) {
        return 0xFF;
    }
    tbl = AT(d, 0x18, f32 *);
    if (AT(gCharSlot2, 0x30, s32) == AT(gCharPlayer, 0x30, s32)) {
        f32 dist = AT(gCharSlot2, 0x1588, f32);

        extern f32 func_00124490(void *a, f32 *p);

        if (dist < 0.0f) {
            dist = func_00124490(gCharSlot2, (f32 *)((u8 *)gCharPlayer + 0x10));
        }
        while (!(tbl[i * 2] <= 0.0f) && !(dist < tbl[i * 2])) {
            i = (i + 1) & 0xFF;
        }
    } else {
        while (!(tbl[i * 2] <= 0.0f)) {
            i = (i + 1) & 0xFF;
        }
    }
    return i;
}

/* track `k` to the chase step's volume, the marked channels' bends to its bend */
s32 func_002C1760(u8 *d, u32 k) {
    u8 step = func_002C1980(d);
    u8 *e, *t;
    u8 v, ch;

    if (step == 0xFF) {
        return 0;
    }
    t = TRACK(d, k);
    e = AT(d, 0x18, u8 *) + step * 8;
    v = AT(e, 0x4, u8);
    if ((f2u(AT(t, 0x100, f32)) & 0xFF) == v) {
        if (v != (f2u(AT(t, 0x100, f32)) & 0xFF)) {
            AT(t, 0x100, f32) = (f32)v;
            VCALL(gSound, 0x34, void (*)(VObject *, u32, u32))(gSound, k, vol_out(d, v) & 0xFF);
        }
    } else {
        func_002C2A70(d, k, v, 2.5f);
    }
    for (ch = 0; ch < 0x10; ch = (ch + 1) & 0xFF) {
        if (AT(d, 0x2C, u8 *)[ch] != 0) {
            func_002C1D10(d, k, ch, 0x5A, AT(e, 0x5, u8));
        }
    }
    return 1;
}

/* ---- channels ---- */

/* channel `ch` of track `k` on at its volume */
void func_002C35F0(u8 *d, u32 k, u32 ch) {
    u8 *e = CHAN(d, k, ch);

    AT(e, 0xC, u8) = 0;
    midi(k, 0xB0, 7, f2u(AT(e, 0x0, f32)) & 0xFF, ch);
}

/* channel `ch` of track `k` silent */
void func_002C36A0(u8 *d, u32 k, u32 ch) {
    AT(CHAN(d, k, ch), 0xC, u8) = 1;
    midi(k, 0xB0, 7, 0, ch);
}

/* track `k`'s notes off (CC 120) */
void func_002C3710(u8 *d, u32 k) {
    midi(k, 0xB0, 0x78, 0, 0xFF);
}

/* track `k`'s settings to the driver: volume, sequence volume, and per channel bend, volume,
   pan */
void func_002C3760(u8 *d, u32 k) {
    u8 *t = TRACK(d, k);
    f32 v;
    u8 ch;

    if (AT(t, 0x10C, u8) != 1) {
        VCALL(gSound, 0x34, void (*)(VObject *, u32, u32))(gSound, k, vol_out(d, f2u(AT(t, 0x100, f32)) & 0xFF) & 0xFF);
    } else {
        VCALL(gSound, 0x34, void (*)(VObject *, u32, u32))(gSound, k, f2u((AT(d, 0x8, f32) * (AT(d, 0x4, f32) * (AT(d, 0xC, f32) * 0.0f))) / 255.0f) & 0xFF);
    }
    v = AT(t, 0x104, f32) * AT(t, 0x108, f32);
    v = (v <= 255.0f ? v : 255.0f) < 20.0f ? 20.0f : (v <= 255.0f ? v : 255.0f);
    VCALL(gSound, 0x3C, void (*)(VObject *, u32, u32))(gSound, k, f2u(v) & 0xFF);
    for (ch = 0; ch < 0x10; ch = (ch + 1) & 0xFF) {
        u8 *e = t + ch * 0x10;
        f32 b = AT(t, 0x108, f32) * AT(e, 0x4, f32);

        b = (b <= 127.0f ? b : 127.0f) < 0.0f ? 0.0f : (b <= 127.0f ? b : 127.0f);
        midi(k, 0xE0, 0, f2u(b) & 0xFF, ch);
        if (AT(e, 0xC, u8) != 1) {
            VCALL(gSound, 0x38, void (*)(VObject *, u32, u32, u32))(gSound, k, ch, f2u(AT(e, 0x0, f32)) & 0xFF);
        } else {
            VCALL(gSound, 0x38, void (*)(VObject *, u32, u32, u32))(gSound, k, ch, 0);
        }
        midi(k, 0xB0, 0xA, f2u(AT(e, 0x8, f32)) & 0xFF, ch);
    }
}

/* ---- life ---- */

extern void func_00100340(void *array, void *(*ctor)(void *), void *(*dtor)(void *, s32), u32 size, u32 n);
extern void func_001002C0(void *block, void *(*dtor)(void *, s32), u32 size, u32 n);
extern void func_00100490(void *p);   /* operator delete */
extern f32 func_0031C6E8(f32 x);      /* logf */

/* placement new */
void *func_002C01A0(u32 size, void *p) {
    return p;
}

/* operator delete of the scene's objects: nothing */
void func_002C0190(void *p) {
}

/* a channel (0x10) */
u8 *func_0039AD70(u8 *e) {
    AT(e, 0x0, f32) = 100.0f;
    AT(e, 0x4, f32) = 64.0f;
    AT(e, 0x8, f32) = 64.0f;
    AT(e, 0xC, u8) = 0;
    return e;
}

void *func_002BFD70(void *e, s32 flags) {
    if (e != NULL && (s16)flags > 0) {
        func_00100490(e);
    }
    return e;
}

/* a track (0x110) */
u8 *func_0039AD10(u8 *t) {
    func_00100340(t, (void *(*)(void *))func_0039AD70, func_002BFD70, 0x10, 0x10);
    AT(t, 0x100, f32) = 0.0f;
    AT(t, 0x104, f32) = 100.0f;
    AT(t, 0x108, f32) = 1.0f;
    AT(t, 0x10C, u8) = 0;
    AT(t, 0x10D, u8) = 0;
    return t;
}

void *func_002BFD10(u8 *t, s32 flags) {
    if (t != NULL) {
        func_001002C0(t, func_002BFD70, 0x10, 0x10);
        if ((s16)flags > 0) {
            func_00100490(t);
        }
    }
    return t;
}

/* a cue (0x28) */
u8 *func_0039ACB0(u8 *c) {
    AT(c, 0x0, u8) = 0xFF;
    AT(c, 0x1, u8) = 0xFF;
    AT(c, 0x10, s32) = -1;
    *(PTMF *)(c + 4) = sGameStateNull;
    AT(c, 0x14, s32) = 0;
    AT(c, 0x18, s32) = 0;
    AT(c, 0x1C, s32) = 0;
    AT(c, 0x20, s32) = 0;
    AT(c, 0x24, s8) = -1;
    AT(c, 0x25, u8) = 0;
    return c;
}

void *func_002BFDC0(void *c, s32 flags) {
    if (c != NULL && (s16)flags > 0) {
        func_00100490(c);
    }
    return c;
}

/* the director's setup: the synths' bend range (RPN 0: 12 semitones), log2(1..128), volumes */
void func_002C5FF0(u8 *d) {
    u8 i;

    midi(0, 0xB0, 0x65, 0, 0xFF);
    midi(0, 0xB0, 0x64, 0, 0xFF);
    midi(0, 0xB0, 0x06, 0xC, 0xFF);
    midi(0, 0xB0, 0x26, 0xC, 0xFF);
    for (i = 0; i < 0x80; i = (i + 1) & 0xFF) {
        AT(d, 0x838 + i * 4, f32) = func_0031C6E8((f32)(i + 1)) / 0.6931472f;
    }
    AT(d, 0x10, s32) = 0;
    AT(d, 0xC, f32) = 1.0f;
    AT(d, 0xA38, u8) = 0;
    AT(d, 0xA39, u8) = 0;
    AT(d, 0x15, u8) = 0xFF;
    AT(d, 0x31, u8) = 0xFF;
    AT(d, 0x4, f32) = AT(gSystemData, 0x38, f32);
    if (gProgress != NULL) {
        AT(d, 0x8, f32) = AT(gProgress, 0x9F0, f32);
    } else {
        AT(d, 0x8, f32) = 1.0f;
    }
}

/* the base's destructor (D_0046EBE0) */
void *func_002C6180(u8 *d, s32 flags) {
    if (d != NULL) {
        AT(d, 0x0, void **) = D_0046EBE0;
        if (d != NULL) {
            D_00456DF0 = NULL;
        }
        if ((s16)flags > 0) {
            func_00100490(d);
        }
    }
    return d;
}

/* the director's members and base destructor (notes off on all four tracks) */
static inline void director_dtor(u8 *d) {
    u8 k;

    AT(d, 0x0, void **) = D_0046EB70;
    for (k = 0; k < 4; k++) {
        midi(k, 0xB0, 0x78, 0, 0xFF);
    }
    func_001002C0(d + 0x474, func_002BFDC0, 0x28, 0x18);
    func_001002C0(d + 0x34, (void *(*)(void *, s32))func_002BFD10, 0x110, 4);
    if (d != NULL) {
        AT(d, 0x0, void **) = D_0046EBE0;
        if (d != NULL) {
            D_00456DF0 = NULL;
        }
    }
}

/* +0x8 */
void *func_002BFBB0(u8 *d, s32 flags) {
    if (d != NULL) {
        director_dtor(d);
    }
    return d;
}

/* +0xC, +0x40: nothing */
void func_002BFE10(u8 *d) {
}

void func_002BFE20(u8 *d) {
}

/* +0x18 */
void func_002BFE30(u8 *d) {
    AT(d, 0xA38, u8) = 0;
}

/* +0x28..+0x34 */
f32 func_002C0150(u8 *d) {
    return AT(d, 0x4, f32);
}

u16 func_002C0160(u8 *d) {
    return AT(d, 0x12, u16);
}

f32 func_002C0170(u8 *d) {
    return AT(d, 0xC, f32);
}

s8 func_002C0180(u8 *d) {
    return AT(d, 0x14, s8);
}

/* +0x20 the music volume (0..1) */
void func_002BFE50(u8 *d, f32 v) {
    if (!(v < 0.0f)) {
        AT(d, 0x4, f32) = v <= 1.0f ? v : 1.0f;
    } else {
        AT(d, 0x4, f32) = 0.0f;
    }
    send_volumes(d);
}

/* +0x24 the progress's music volume (0..1) */
void func_002BFFD0(u8 *d, f32 v) {
    if (!(v < 0.0f)) {
        AT(d, 0x8, f32) = v <= 1.0f ? v : 1.0f;
    } else {
        AT(d, 0x8, f32) = 0.0f;
    }
    send_volumes(d);
}

/* +0x48 the director's volume back to full */
void func_002C4330(u8 *d) {
    AT(d, 0xC, f32) = 1.0f;
    send_volumes(d);
}

/* +0x44 the director's volume (0..1; else half) */
void func_002C4480(u8 *d, f32 v) {
    if (v <= 1.0f && !(v < 0.0f)) {
        AT(d, 0xC, f32) = v;
    } else {
        AT(d, 0xC, f32) = 0.5f;
    }
    send_volumes(d);
}

/* track 0's channels 3..8 to their volumes */
static inline void calm_channels_on(u8 *d) {
    u8 ch;

    for (ch = 3; ch < 9; ch++) {
        AT(CHAN(d, 0, ch), 0xC, u8) = 0;
        midi(0, 0xB0, 7, f2u(AT(CHAN(d, 0, ch), 0x0, f32)) & 0xFF, ch);
    }
}

/* +0x64 the panic over: track 0's channels 3..8 back */
void func_002C01B0(u8 *d) {
    AT(d, 0x30, u8) = 9;
    calm_channels_on(d);
}

/* +0x60 panic: track 0 alone, channels 3..8 silent */
void func_002C04D0(u8 *d) {
    u8 ch;

    AT(d, 0x30, u8) = 8;
    midi(0, 0xB0, 0x78, 0, 0xFF);
    VCALL(gSound, 0x24, void (*)(VObject *, u32, u32))(gSound, 0, 1);
    func_002C3760(d, 0);
    func_002C2CD0(d, 1, 1, 0);
    func_002C2CD0(d, 2, 1, 0);
    func_002C2CD0(d, 3, 1, 0);
    func_002C2CD0(d, 0, 1, 0xFF);
    for (ch = 3; ch < 9; ch++) {
        AT(CHAN(d, 0, ch), 0xC, u8) = 1;
        midi(0, 0xB0, 7, 0, ch);
    }
}

/* +0x54 the chase begins (`now`: at once): track 3 (the chase) from its start at the chase
   step's bend, tracks 1 / 2 out; seen or not (+0x5C / +0x58) */
void func_002C0720(u8 *d, s32 now) {
    u8 *p = (u8 *)gCharPursuer;
    u8 step = func_002C1980(d);
    u32 vol, bend;
    s32 lost = 0;
    s32 frames;
    f32 b;

    if (step != 0xFF) {
        vol = AT(AT(d, 0x18, u8 *) + step * 8, 0x4, u8);
        bend = AT(AT(d, 0x18, u8 *) + step * 8, 0x5, u8);
    } else {
        vol = 0;
        bend = 0;
    }
    if (p != NULL) {
        lost = AT(p, 0x16C8, u8) == 0;
    }
    if (now == 0) {
        frames = 0x5A;
        func_002C2CD0(d, 0, 0x5A, 0);
    } else {
        frames = 1;
        func_002C2CD0(d, 1, 1, 0);
        func_002C2CD0(d, 2, 1, 0);
    }
    if (lost) {
        VCALL(d, 0x5C, void (*)(u8 *, s32))(d, 1);
        vol = 0xFF;
    } else {
        VCALL(d, 0x58, void (*)(u8 *, s32))(d, 1);
    }
    b = (f32)bend;
    b = (b <= 127.0f ? b : 127.0f) < 0.0f ? 0.0f : (b <= 127.0f ? b : 127.0f);
    AT(CHAN(d, 3, 0), 0x4, f32) = b;
    midi(3, 0xE0, 0, f2u(b) & 0xFF, 0);
    midi(3, 0xB0, 0x78, 0, 0xFF);
    VCALL(gSound, 0x24, void (*)(VObject *, u32, u32))(gSound, 3, 1);
    func_002C2CD0(d, 3, frames, vol & 0xFF);
}

/* +0x4C everything silent and muted */
void func_002C3C50(u8 *d) {
    AT(d, 0x30, u8) = 2;
    track_zero(d, 1);
    track_zero(d, 2);
    track_zero(d, 3);
    track_zero(d, 0);
    track_mute(d, 1);
    track_mute(d, 2);
    track_mute(d, 3);
    track_mute(d, 0);
}

extern s32 func_00177620(Progress *p);   /* the chase: 0 none, 1 / 2 being chased */

/* back to calm: tracks 1 / 2 on (their fades dropped) */
static inline void to_calm(u8 *d) {
    AT(d, 0x30, u8) = 0;
    track_unmute(d, 1);
    track_unmute(d, 2);
    cues_cancel(d, 1, 0);
    cues_cancel(d, 2, 0);
}

/* panic: track 0 on */
static inline void to_panic(u8 *d) {
    AT(d, 0x30, u8) = 7;
    track_unmute(d, 0);
    cues_cancel(d, 0, 0);
}

/* chased: track 3 on, state `s` */
static inline void to_chase(u8 *d, u8 s) {
    AT(d, 0x30, u8) = s;
    track_unmute(d, 3);
    cues_cancel(d, 3, 0);
}

/* +0x50 a frame of the state (not while the room objects hold it, or with progress flag 8) */
void func_002C0A30(u8 *d) {
    Progress *p;
    s32 seen = 0;

    if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) != 0) {
        return;
    }
    p = gProgress;
    if (Progress_TestFlag(p, 8) != 0) {
        return;
    }
    if (gCharPursuer != NULL) {
        seen = AT(gCharPursuer, 0x16C8, u8) == 0;
    }
    switch (AT(d, 0x30, u8)) {
    case 0:
        AT(d, 0x30, u8) = 1;
        func_002C2CD0(d, 3, 0x5A, 0);
        func_002C2CD0(d, 0, 0x5A, 0);
        func_002C2CD0(d, 1, 0x5A, 0xFF);
        func_002C2CD0(d, 2, 0x5A, 0xFF);
        break;
    case 1:
        if ((func_00177620(p) & 0xFF) == 1) {
            AT(d, 0x30, u8) = 2;
            func_002C2CD0(d, 1, 0x5A, 0);
            func_002C2CD0(d, 2, 0x5A, 0);
        } else if ((func_00177620(p) & 0xFF) == 2) {
            to_chase(d, 3);
        } else if (AT(p, 0x7B8, u8) == 4) {
            to_panic(d);
        }
        break;
    case 2:
        if (!(func_00177620(p) & 0xFF)) {
            to_calm(d);
        } else if ((func_00177620(p) & 0xFF) == 2) {
            to_chase(d, 3);
        } else if (AT(p, 0x7B8, u8) == 4) {
            to_panic(d);
        }
        break;
    case 3:
        VCALL(d, 0x54, void (*)(u8 *, s32))(d, 1);
        break;
    case 4:
        VCALL(d, 0x54, void (*)(u8 *, s32))(d, 0);
        break;
    case 5:
        if (!(func_00177620(p) & 0xFF)) {
            to_calm(d);
        } else if ((func_00177620(p) & 0xFF) == 1) {
            AT(d, 0x30, u8) = 2;
            func_002C2CD0(d, 3, 0x5A, 0);
        } else if (seen) {
            VCALL(d, 0x5C, void (*)(u8 *, s32, s32))(d, 0, 1);
            func_002C2CD0(d, 3, 0x1E, 0xFF);
        } else if (AT(p, 0x7B8, u8) == 4) {
            to_panic(d);
        } else {
            func_002C1760(d, 3);
        }
        break;
    case 6:
        if (!(func_00177620(p) & 0xFF)) {
            to_calm(d);
        } else if ((func_00177620(p) & 0xFF) == 1) {
            AT(d, 0x30, u8) = 2;
            func_002C2CD0(d, 3, 0x5A, 0);
        } else if (!seen) {
            VCALL(d, 0x58, void (*)(u8 *, s32, s32))(d, 0, 1);
        } else if (AT(p, 0x7B8, u8) == 4) {
            to_panic(d);
        }
        break;
    case 7:
        VCALL(d, 0x60, void (*)(u8 *))(d);
        break;
    case 8:
        if (AT(p, 0x7B8, u8) == 5) {
            VCALL(d, 0x64, void (*)(u8 *))(d);
        }
        /* fall through */
    case 9:
        if (AT(p, 0x7B8, u8) < 4) {
            if (!(func_00177620(p) & 0xFF)) {
                to_calm(d);
            } else if ((func_00177620(p) & 0xFF) != 1) {
                if ((func_00177620(p) & 0xFF) == 2) {
                    to_chase(d, 4);
                }
            } else {
                AT(d, 0x30, u8) = 2;
                func_002C2CD0(d, 0, 0x5A, 0);
            }
        }
        break;
    }
}

/* channel `ch` (0xFF all) of track `k`: a value from `tbl` into field `off` (0..127) */
static inline void chan_set(u8 *d, u32 k, u8 ch, u32 v, s32 off) {
    f32 f = (f32)v;

    f = (f <= 127.0f ? f : 127.0f) < 0.0f ? 0.0f : (f <= 127.0f ? f : 127.0f);
    if (ch == 0xFF) {
        u8 i;

        for (i = 0; i < 0x10; i = (i + 1) & 0xFF) {
            AT(CHAN(d, k, i), off, f32) = f;
        }
    } else {
        AT(CHAN(d, k, ch), off, f32) = f;
    }
}

/* +0x3C start: the volumes at the global one, tracks 1 / 2 full, 0 / 3 silent, each track's
   sequence volume and channels from the stage's tables, tracks 3 and 0 muted; then +0x40 */
void func_002C4600(u8 *d) {
    u8 k;

    AT(d, 0x30, u8) = 1;
    AT(d, 0x14, s8) = 0;
    AT(d, 0x10, s32) = 0;
    send_volumes(d);
    if ((f2u(AT(TRACK(d, 1), 0x100, f32)) & 0xFF) != 0xFF) {
        AT(TRACK(d, 1), 0x100, f32) = 255.0f;
        VCALL(gSound, 0x34, void (*)(VObject *, u32, u32))(gSound, 1, vol_out(d, 0xFF) & 0xFF);
    }
    if ((f2u(AT(TRACK(d, 2), 0x100, f32)) & 0xFF) != 0xFF) {
        AT(TRACK(d, 2), 0x100, f32) = 255.0f;
        VCALL(gSound, 0x34, void (*)(VObject *, u32, u32))(gSound, 2, vol_out(d, 0xFF) & 0xFF);
    }
    track_zero(d, 3);
    track_zero(d, 0);
    track_unmute(d, 1);
    track_unmute(d, 2);
    for (k = 0; k < 4; k++) {
        u8 *t = TRACK(d, k);
        u32 sv = AT(d, 0x28, u8 *)[k];
        u8 was = f2u(AT(t, 0x104, f32)) & 0xFF;
        f32 f = (f32)sv;
        u8 ch;

        f = (f <= 255.0f ? f : 255.0f) < 20.0f ? 20.0f : (f <= 255.0f ? f : 255.0f);
        AT(t, 0x104, f32) = f;
        if (was != (f2u(f) & 0xFF)) {
            VCALL(gSound, 0x3C, void (*)(VObject *, u32, u32))(gSound, k, sv);
        }
        for (ch = 0; ch < 0x10; ch++) {
            u8 *e = CHAN(d, k, ch);
            u32 v;

            AT(e, 0xC, u8) = 0;
            midi(k, 0xB0, 7, f2u(AT(e, 0x0, f32)) & 0xFF, ch);
            v = AT(d, 0x24, u8 *)[k * 0x10 + ch];
            chan_set(d, k, ch, v, 0x8);
            midi(k, 0xB0, 0xA, f2u(AT(e, 0x8, f32)) & 0xFF, ch);
            v = AT(d, 0x20, u8 *)[k * 0x10 + ch];
            chan_set(d, k, ch, v, 0x4);
            midi(k, 0xE0, 0, f2u(AT(e, 0x4, f32)) & 0xFF, ch);
            v = AT(d, 0x1C, u8 *)[k * 0x10 + ch];
            chan_set(d, k, ch, v, 0x0);
            VCALL(gSound, 0x38, void (*)(VObject *, u32, u32, u32))(gSound, k, ch, f2u(AT(e, 0x0, f32)) & 0xFF);
        }
    }
    track_mute(d, 3);
    track_mute(d, 0);
    VCALL(d, 0x40, void (*)(u8 *))(d);
}

/* each frame of play (scene +0x106503C): the start steps (+0x15) - 0 wait for the banks (+0x10),
 * 1 wait for flag +0xA38 to clear, 2 start each track's sequence once (driver +0x1C; track
 * +0x10D), 3 the four tracks on (driver +0x24), the director started (+0x3C), the global
 * volume faded in over 90 frames to the one asked for meanwhile (+0x31) - then 4: the frame
 * update */
void func_002C5980(u8 *d) {
    VObject *drv;
    u32 k;

    switch (AT(d, 0x15, u8)) {
    case 0:
        if (!(u8)VCALL(d, 0x10, s32 (*)(u8 *))(d)) {
            AT(d, 0x15, u8) = 1;
        }
        break;
    case 1:
        if (AT(d, 0xA38, u8) != 0) {
            break;
        }
        AT(d, 0x15, u8) = 2;
        /* fall through */
    case 2:
        for (k = 0; k < 4; k++) {
            if (AT(TRACK(d, k), 0x10D, u8) != 1) {
                VCALL(gSound, 0x1C, void (*)(VObject *, u32, u32, u32))(gSound, k, 0, 0);
                AT(TRACK(d, k), 0x10D, u8) = 1;
            }
        }
        AT(d, 0x15, u8) = 3;
        break;
    case 3:
        drv = gSound;
        for (k = 0; k < 4; k++) {
            VCALL(drv, 0x24, void (*)(VObject *, u32, u32))(drv, k, 1);
        }
        VCALL(d, 0x3C, void (*)(u8 *))(d);
        VCALL(d, 0x38, s32 (*)(u8 *, s32, u8))(d, 90, AT(d, 0x31, u8));
        AT(d, 0x31, u8) = 0xFF;
        AT(d, 0x15, u8) = 4;
        break;
    case 4:
        func_002C34C0(d);
        break;
    }
}

/* +0x14 reset: the banks' files dropped (still loading) or the banks unloaded; the cues and
   the tracks back to their defaults */
void func_002C5BD0(u8 *d) {
    s32 loading = 0;
    s32 i, k, ch;

    for (k = 0; k < 4; k++) {
        loading |= VCALL(gSound, 0x74, s32 (*)(VObject *, s32))(gSound, k) & 0xFF;
    }
    for (k = 0; k < 4; k++) {
        if (loading) {
            VCALL(gSound, 0x84, void (*)(VObject *, s32))(gSound, k);
        } else {
            VCALL(gSound, 0x64, void (*)(VObject *, s32))(gSound, k);
        }
    }
    for (i = 0; i < 0x18; i++) {
        u8 *c = CUE(d, i);

        AT(c, 0x0, u8) = 0xFF;
        AT(c, 0x1, u8) = 0xFF;
        AT(c, 0x10, s32) = -1;
        *(PTMF *)(c + 4) = sGameStateNull;
        AT(c, 0x14, s32) = 0;
        AT(c, 0x18, s32) = 0;
        AT(c, 0x1C, s32) = 0;
        AT(c, 0x20, s32) = 0;
        AT(c, 0x24, s8) = -1;
        AT(c, 0x25, u8) = 0;
    }
    for (k = 0; k < 4; k++) {
        u8 *t = TRACK(d, k);

        for (ch = 0; ch < 0x10; ch++) {
            AT(t + ch * 0x10, 0x0, f32) = 100.0f;
            AT(t + ch * 0x10, 0x4, f32) = 64.0f;
            AT(t + ch * 0x10, 0x8, f32) = 64.0f;
            AT(t + ch * 0x10, 0xC, u8) = 0;
        }
        AT(t, 0x100, f32) = 0.0f;
        AT(t, 0x104, f32) = 100.0f;
        AT(t, 0x108, f32) = 1.0f;
        AT(t, 0x10C, u8) = 0;
        AT(t, 0x10D, u8) = 0;
    }
}

/* +0x10 the banks in: 1 while still loading; else registered, silent */
s32 func_002C5E80(u8 *d) {
    s32 loading = 0;
    s32 k;

    for (k = 0; k < 4; k++) {
        loading |= VCALL(gSound, 0x74, s32 (*)(VObject *, s32))(gSound, k) & 0xFF;
    }
    if (loading) {
        return 1;
    }
    for (k = 0; k < 4; k++) {
        VCALL(gSound, 0x60, void (*)(VObject *, s32))(gSound, k);
    }
    for (k = 0; k < 4; k++) {
        VCALL(gSound, 0x34, void (*)(VObject *, s32, s32))(gSound, k, 0);
    }
    return 0;
}

/* ---- the stages ---- */

extern void *D_0046F480[], *D_00473830[], *D_00478AE0[], *D_004799F0[];
extern u8 D_00414650[], D_004146C0[], D_00414700[], D_00414740[], D_0047AC28[], D_00414780[];
extern u8 D_0042A180[], D_0042A1F0[], D_0042A230[], D_0042A270[], D_0047AD68[], D_0042A2B0[];
extern u8 D_00442C20[], D_00442C90[], D_00442CD0[], D_00442D10[], D_0047AF68[], D_00442D50[];
extern u8 D_00444850[], D_004448C0[], D_00444900[], D_00444940[], D_0047AFF8[], D_01991AA0[];

/* the music director for stage set `stage` (0..3) at scene +0x1064600 (scene +0x106503C) */
void func_0039A8E0(u8 *scene, u32 stage) {
    static void **const sVtbl[4] = {D_0046F480, D_00473830, D_00478AE0, D_004799F0};
    static u8 *const sTables[4][6] = {
        {D_00414650, D_004146C0, D_00414700, D_00414740, D_0047AC28, D_00414780},
        {D_0042A180, D_0042A1F0, D_0042A230, D_0042A270, D_0047AD68, D_0042A2B0},
        {D_00442C20, D_00442C90, D_00442CD0, D_00442D10, D_0047AF68, D_00442D50},
        {D_00444850, D_004448C0, D_00444900, D_00444940, D_0047AFF8, D_01991AA0},
    };
    u8 *d;
    s32 i;

    stage &= 0xFF;
    if (stage >= 4) {
        return;
    }
    d = func_002C01A0(0xA3C, scene + 0x1064600);
    if (d != NULL) {
        D_00456DF0 = d;
        AT(d, 0x0, void **) = D_0046EB70;
        func_00100340(d + 0x34, (void *(*)(void *))func_0039AD10, (void *(*)(void *, s32))func_002BFD10, 0x110, 4);
        func_00100340(d + 0x474, (void *(*)(void *))func_0039ACB0, func_002BFDC0, 0x28, 0x18);
        func_002C5FF0(d);
        AT(d, 0x0, void **) = sVtbl[stage];
        for (i = 0; i < 6; i++) {
            AT(d, 0x18 + i * 4, u8 *) = sTables[stage][i];
        }
    }
    AT(scene, 0x106503C, u8 *) = d;
}

/* the stages' music (subclass +0xC): the bank's header, the four sequences (0 panic, 1 / 2 the
   calm music's two parts, 3 the chase) and the bank's samples into the progress's buffers */
static inline void stage_load(u8 *d, const char *const *files) {
    u8 *p = (u8 *)gProgress;

    AT(d, 0x15, u8) = 0;
    VCALL(gSound, 0x80, void (*)(VObject *, const char *, u32, u32, void *))(gSound, files[0], 0, 0, p + 0x16C0);
    VCALL(gSound, 0x80, void (*)(VObject *, const char *, u32, u32, void *))(gSound, files[1], 0, 1, p + 0x36C0);
    VCALL(gSound, 0x80, void (*)(VObject *, const char *, u32, u32, void *))(gSound, files[2], 1, 1, p + 0x56C0);
    VCALL(gSound, 0x80, void (*)(VObject *, const char *, u32, u32, void *))(gSound, files[3], 2, 1, p + 0x76C0);
    VCALL(gSound, 0x80, void (*)(VObject *, const char *, u32, u32, void *))(gSound, files[4], 3, 1, p + 0x96C0);
    VCALL(gSound, 0x80, void (*)(VObject *, const char *, u32, u32, void *))(gSound, files[5], 0, 3, p + 0xB6C0);
}

/* the chase seen (subclass +0x5C): state 6, the chase's sequence volume to `vol`, the listed
   channels bent up fully */
static inline void stage_seen(u8 *d, s32 now, u32 vol, const u8 *chans, s32 n) {
    s32 frames, i;

    AT(d, 0x30, u8) = 6;
    frames = now == 0 ? 0x1E : 1;
    func_002C23E0(d, 3, frames, vol);
    for (i = 0; i < n; i++) {
        func_002C1D10(d, 3, chans[i], frames, 0x7F);
    }
}

/* the chase lost (subclass +0x58): state 5, the chase's sequence volume back, the listed
   channels' bend centred */
static inline void stage_lost(u8 *d, s32 now, const u8 *chans, s32 n) {
    s32 frames, i;

    AT(d, 0x30, u8) = 5;
    frames = now == 0 ? 0x5A : 1;
    func_002C23E0(d, 3, frames, AT(d, 0x28, u8 *)[3]);
    for (i = 0; i < n; i++) {
        func_002C1D10(d, 3, chans[i], frames, 0x40);
    }
}

/* the chase's channels in `c` on or silent (subclass +0x40) */
static inline void stage_chans(u8 *d, const u8 *c, s32 n, s32 on) {
    s32 i;

    for (i = 0; i < n; i++) {
        if (on) {
            func_002C35F0(d, 3, c[i]);
        } else {
            func_002C36A0(d, 3, c[i]);
        }
    }
}

/* a stage's destructor */
static inline void *stage_dtor(u8 *d, s32 flags, void **vt) {
    if (d != NULL) {
        u8 k;

        AT(d, 0x0, void **) = vt;
        if (d != NULL) {
            AT(d, 0x0, void **) = D_0046EB70;
            for (k = 0; k < 4; k++) {
                func_002C3710(d, k);
            }
            func_001002C0(d + 0x474, func_002BFDC0, 0x28, 0x18);
            func_001002C0(d + 0x34, (void *(*)(void *, s32))func_002BFD10, 0x110, 4);
            if (d != NULL) {
                AT(d, 0x0, void **) = D_0046EBE0;
                if (d != NULL) {
                    D_00456DF0 = NULL;
                }
            }
        }
        if ((s16)flags > 0) {
            func_002C0190(d);
        }
    }
    return d;
}

/* stage 1 (D_0046F480) */

extern const char D_0045D8B0[], D_0045D8C8[], D_0045D8E0[], D_0045D900[], D_0045D920[], D_0045D930[];

void *func_002D3A90(u8 *d, s32 flags) {
    return stage_dtor(d, flags, D_0046F480);
}

void func_002D3E80(u8 *d) {
    static const char *const sFiles[6] = {D_0045D8B0, D_0045D8C8, D_0045D8E0, D_0045D900, D_0045D920, D_0045D930};

    stage_load(d, sFiles);
}

void func_002D3B80(u8 *d, s32 now) {
    static const u8 sChans[7] = {0, 1, 3, 4, 5, 6, 7};

    stage_seen(d, now, 0x88, sChans, 7);
}

void func_002D3C80(u8 *d, s32 now) {
    static const u8 sChans[6] = {1, 3, 4, 5, 6, 7};

    stage_lost(d, now, sChans, 6);
}

/* (progress word +0x1C bit 27: which of the chase's two sets) */
void func_002D3D60(u8 *d) {
    static const u8 sA[4] = {2, 3, 4, 5}, sB[3] = {6, 7, 8};

    if (AT(gProgress, 0x1C, u32) & 0x08000000) {
        stage_chans(d, sA, 4, 1);
        stage_chans(d, sB, 3, 0);
    } else {
        stage_chans(d, sA, 4, 0);
        stage_chans(d, sB, 3, 1);
    }
}

/* stage 2 (D_00473830) */

extern const char D_0045FFC0[], D_0045FFD8[], D_0045FFF0[], D_00460010[], D_00460030[], D_00460040[];

void *func_0031ED70(u8 *d, s32 flags) {
    return stage_dtor(d, flags, D_00473830);
}

void func_0031EFF0(u8 *d) {
    static const char *const sFiles[6] = {D_0045FFC0, D_0045FFD8, D_0045FFF0, D_00460010, D_00460030, D_00460040};

    stage_load(d, sFiles);
}

void func_0031EE70(u8 *d, s32 now) {
    static const u8 sChans[5] = {0, 2, 3, 4, 5};

    stage_seen(d, now, 0x8C, sChans, 5);
}

void func_0031EF40(u8 *d, s32 now) {
    static const u8 sChans[4] = {2, 3, 4, 5};

    stage_lost(d, now, sChans, 4);
}

void func_0031EE60(u8 *d) {
}

/* stage 3 (D_00478AE0) */

extern const char D_00462CD0[], D_00462CE8[], D_00462D00[], D_00462D20[], D_00462D40[], D_00462D50[];

void *func_0034DCC0(u8 *d, s32 flags) {
    return stage_dtor(d, flags, D_00478AE0);
}

void func_0034DFC0(u8 *d) {
    static const char *const sFiles[6] = {D_00462CD0, D_00462CE8, D_00462D00, D_00462D20, D_00462D40, D_00462D50};

    stage_load(d, sFiles);
}

void func_0034DDB0(u8 *d, s32 now) {
    static const u8 sChans[6] = {0, 1, 2, 4, 5, 6};

    stage_seen(d, now, 0x8C, sChans, 6);
}

void func_0034DE90(u8 *d, s32 now) {
    static const u8 sChans[4] = {1, 2, 4, 6};

    stage_lost(d, now, sChans, 4);
}

/* (progress word +0x2C bit 9) */
void func_0034DF40(u8 *d) {
    static const u8 sA[2] = {5, 6};

    stage_chans(d, sA, 2, (AT(gProgress, 0x2C, u32) & 0x200) != 0);
}

/* stage 4 (D_004799F0) */

extern const char D_004633C0[], D_004633D8[], D_004633F0[], D_00463410[], D_00463430[], D_00463440[];

void *func_0035F100(u8 *d, s32 flags) {
    return stage_dtor(d, flags, D_004799F0);
}

void func_0035F490(u8 *d) {
    static const char *const sFiles[6] = {D_004633C0, D_004633D8, D_004633F0, D_00463410, D_00463430, D_00463440};

    stage_load(d, sFiles);
}

void func_0035F1F0(u8 *d, s32 now) {
    static const u8 sChans[6] = {0, 1, 2, 3, 5, 6};

    stage_seen(d, now, 0x6E, sChans, 6);
}

void func_0035F2D0(u8 *d, s32 now) {
    static const u8 sChans[6] = {0, 1, 2, 3, 5, 6};

    stage_lost(d, now, sChans, 6);
}

/* (progress word +0x2C bit 21) */
void func_0035F3B0(u8 *d) {
    static const u8 sA[2] = {3, 4}, sB[3] = {5, 6, 7};

    if (AT(gProgress, 0x2C, u32) & 0x200000) {
        stage_chans(d, sA, 2, 0);
        stage_chans(d, sB, 3, 1);
    } else {
        stage_chans(d, sA, 2, 1);
        stage_chans(d, sB, 3, 0);
    }
}
