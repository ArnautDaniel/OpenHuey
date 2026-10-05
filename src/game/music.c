/* The stage music director (scene +0x1064600, 0xA3C bytes, global D_00456DF0; base vtable
 * D_0046EB70, one subclass per stage set up by func_0039A8E0). It plays the stage's music on
 * the sound driver's four sequence banks (0..3, an SQ each over the stage's HD/BD bank) and
 * mixes them live: each track's volume and sequence volume, and per MIDI channel a volume,
 * pitch bend and pan, sent as MIDI (driver +0x30: command 0x23). Fades run as "cues" (a
 * member function called each frame). Its state (+0x30) follows the chase (func_00177620),
 * the panic level (progress +0x7B8) and whether the pursuer sees Fiona:
 *   0 start, fading in track 0 (calm) and 1 / 2 (ambience); 1 calm; 2 track 1/2 fading to
 *   the chase; 3 / 4 chase begins (+0x54); 5 / 6 the chase (track 3, +0x58 / +0x5C: seen or
 *   not); 7 panic (+0x60); 8 panic over (+0x64 with progress +0x7B8 == 5); 9 after.
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

extern VObject *D_0044E560;   /* the sound driver */
extern u8 *D_0044E978;        /* +0x38: the music volume */
extern Progress *gProgress;
extern u8 *gCharPursuer;
extern u8 *D_0044F808;        /* the character in slot 2 */
extern u8 *gCharPlayer;
extern VObject *D_0044E4D0;   /* the room objects */
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
    VCALL(D_0044E560, 0x30, void (*)(VObject *, u32, u8 *, u32))(D_0044E560, k, m, ch);
}

/* track volume `v` (0..255) as the driver gets it */
static inline u8 vol_out(u8 *d, u32 v) {
    return f2u((AT(d, 0x8, f32) * (AT(d, 0x4, f32) * (AT(d, 0xC, f32) * (f32)(v * AT(d, 0x12, s16))))) / 255.0f);
}

static inline void send_volume(u8 *d, u32 k) {
    VCALL(D_0044E560, 0x34, void (*)(VObject *, u32, u32))(D_0044E560, k, vol_out(d, f2u(AT(TRACK(d, k), 0x100, f32)) & 0xFF));
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
        VCALL(D_0044E560, 0x34, void (*)(VObject *, u32, u32))(D_0044E560, k, vol_out(d, 0) & 0xFF);
    }
}

/* track `k` muted: volume 0 and the synth's voices off (driver +0x28: command 0x38 0) */
static inline void track_mute(u8 *d, u32 k) {
    if (AT(TRACK(d, k), 0x10C, u8) == 0) {
        AT(TRACK(d, k), 0x10C, u8) = 1;
        VCALL(D_0044E560, 0x34, void (*)(VObject *, u32, u32))(D_0044E560, k, vol_out(d, 0) & 0xFF);
        VCALL(D_0044E560, 0x28, void (*)(VObject *, u32, u32))(D_0044E560, k, 0);
    }
}

/* track `k` back on */
static inline void track_unmute(u8 *d, u32 k) {
    if (AT(TRACK(d, k), 0x10C, u8) == 1) {
        AT(TRACK(d, k), 0x10C, u8) = 0;
        VCALL(D_0044E560, 0x28, void (*)(VObject *, u32, u32))(D_0044E560, k, 1);
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
            VCALL(D_0044E560, 0x34, void (*)(VObject *, u32, u32, f32 *))(D_0044E560, k, vol_out(d, v) & 0xFF,
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
                VCALL(D_0044E560, 0x34, void (*)(VObject *, u32, u32, f32 *))(D_0044E560, k, vol_out(d, now) & 0xFF,
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
            VCALL(D_0044E560, 0x3C, void (*)(VObject *, u32, u32, u32, f32 *))(D_0044E560, k, v, was, (f32 *)(t + 0x104));
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
            VCALL(D_0044E560, 0x3C, void (*)(VObject *, u32, u32, u32, f32 *))(D_0044E560, k, f2u(o) & 0xFF & 0xFF, was,
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

    if (D_0044F808 == NULL) {
        return 0xFF;
    }
    tbl = AT(d, 0x18, f32 *);
    if (AT(D_0044F808, 0x30, s32) == AT(gCharPlayer, 0x30, s32)) {
        f32 dist = AT(D_0044F808, 0x1588, f32);

        extern f32 func_00124490(void *a, f32 *p);

        if (dist < 0.0f) {
            dist = func_00124490(D_0044F808, (f32 *)(gCharPlayer + 0x10));
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
            VCALL(D_0044E560, 0x34, void (*)(VObject *, u32, u32))(D_0044E560, k, vol_out(d, v) & 0xFF);
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
        VCALL(D_0044E560, 0x34, void (*)(VObject *, u32, u32))(D_0044E560, k, vol_out(d, f2u(AT(t, 0x100, f32)) & 0xFF) & 0xFF);
    } else {
        VCALL(D_0044E560, 0x34, void (*)(VObject *, u32, u32))(D_0044E560, k, f2u((AT(d, 0x8, f32) * (AT(d, 0x4, f32) * (AT(d, 0xC, f32) * 0.0f))) / 255.0f) & 0xFF);
    }
    v = AT(t, 0x104, f32) * AT(t, 0x108, f32);
    v = (v <= 255.0f ? v : 255.0f) < 20.0f ? 20.0f : (v <= 255.0f ? v : 255.0f);
    VCALL(D_0044E560, 0x3C, void (*)(VObject *, u32, u32))(D_0044E560, k, f2u(v) & 0xFF);
    for (ch = 0; ch < 0x10; ch = (ch + 1) & 0xFF) {
        u8 *e = t + ch * 0x10;
        f32 b = AT(t, 0x108, f32) * AT(e, 0x4, f32);

        b = (b <= 127.0f ? b : 127.0f) < 0.0f ? 0.0f : (b <= 127.0f ? b : 127.0f);
        midi(k, 0xE0, 0, f2u(b) & 0xFF, ch);
        if (AT(e, 0xC, u8) != 1) {
            VCALL(D_0044E560, 0x38, void (*)(VObject *, u32, u32, u32))(D_0044E560, k, ch, f2u(AT(e, 0x0, f32)) & 0xFF);
        } else {
            VCALL(D_0044E560, 0x38, void (*)(VObject *, u32, u32, u32))(D_0044E560, k, ch, 0);
        }
        midi(k, 0xB0, 0xA, f2u(AT(e, 0x8, f32)) & 0xFF, ch);
    }
}
