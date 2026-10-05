/* The music sequences on PC: what Sony's modmidi (MIDI sequencer) and modhsyn (synthesizer on
 * an HD/BD bank) do for the sound driver's sequence ports (SNDDRV commands 0x0C..0x21, 0x38).
 * The sequences are SQ files ("SEQp" chunks: MIDI with running status, a set top bit on an
 * event's last data byte meaning no delta time follows, one-byte note-offs, NRPN 20 / 30 loop
 * marks); notes are played on SPU2 core 0's voices (spu2.c) from the port's HD bank. Timing
 * advances with the output (seq_tick from the mixer). */
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "iop_mem.h"
#include "snd.h"

#define NPORT 4
#define NVOICE 24   /* core 0 */

typedef struct Chan {
    int prog, vol, expr, pan, bend;
} Chan;

typedef struct Port {
    int loaded, playing;
    unsigned hd, spu;           /* its bank */
    const unsigned char *data, *end, *pos;
    const unsigned char *loopPos;
    int loopCount, loopLeft;
    unsigned char status;       /* running status */
    double wait;                /* ticks to the next event */
    int res;                    /* ticks per quarter note */
    unsigned usPerQn;
    int relTempo;               /* 0x100 = as written (commands 0x20 / 0x21) */
    int vol;                    /* the sequence volume 0..127 (0x1E) */
    int synthVol;               /* the synth's 0..255 (0x1B) */
    int nrpn;
    Chan ch[16];
} Port;

typedef struct SVoice {
    int port, ch, note;         /* -1: free */
    int vol14, pan;             /* the note's volume and pan (before the channel's) */
    int baseNote, fine, rate, bendLo, bendHi;
    unsigned age;
} SVoice;

static Port sPort[NPORT];
static SVoice sVoice[NVOICE];
static unsigned sAge;

static unsigned rd32(const unsigned char *p) { return p[0] | p[1] << 8 | p[2] << 16 | (unsigned)p[3] << 24; }
static unsigned rd16(const unsigned char *p) { return p[0] | p[1] << 8; }

static unsigned vlq(const unsigned char **pp, const unsigned char *end) {
    unsigned v = 0;

    while (*pp < end) {
        unsigned char b = *(*pp)++;

        v = (v << 7) | (b & 0x7F);
        if (!(b & 0x80)) {
            break;
        }
    }
    return v;
}

/* ---- the synth ---- */

static unsigned short sd_voice(int v) { return (unsigned short)(v * 2); }

/* a free voice (or the oldest) */
static int voice_alloc(void) {
    int v, best = 0;

    for (v = 0; v < NVOICE; v++) {
        if (sVoice[v].port < 0 && sceSdGetParam(sd_voice(v) | 0x0500) == 0) {
            return v;
        }
    }
    for (v = 0; v < NVOICE; v++) {
        if (sVoice[v].age < sVoice[best].age) {
            best = v;
        }
    }
    return best;
}

static void voice_volume(int v) {
    SVoice *sv = &sVoice[v];
    Port *p = &sPort[sv->port];
    Chan *c = &p->ch[sv->ch];
    int x = sv->vol14 * c->vol / 127 * c->expr / 127 * p->vol / 127 * p->synthVol / 255;
    int pan = sv->pan + (c->pan - 64), l, r;

    if (pan < 0) {
        pan = 0;
    } else if (pan > 127) {
        pan = 127;
    }
    if (pan < 0x40) {
        l = x;
        r = x * pan * 2 >> 7;
    } else if (pan > 0x40) {
        r = x;
        l = x * (0x100 - (pan == 0x7F ? 0x80 : pan) * 2) >> 7;
    } else {
        l = r = x;
    }
    sceSdSetParam(sd_voice(v) | 0x0000, (unsigned short)(l & 0x3FFF));
    sceSdSetParam(sd_voice(v) | 0x0100, (unsigned short)(r & 0x3FFF));
}

static void voice_pitch(int v) {
    SVoice *sv = &sVoice[v];
    int b = sPort[sv->port].ch[sv->ch].bend - 8192;
    double semis = b >= 0 ? b / 8192.0 * sv->bendHi : b / 8192.0 * sv->bendLo;
    int fine = sv->fine + (int)lround(semis * 128.0);
    unsigned p = sceSdNote2Pitch((unsigned short)sv->baseNote, 0, (unsigned short)sv->note, (short)fine);

    sceSdSetParam(sd_voice(v) | 0x0200, (unsigned short)(p * (unsigned)sv->rate / 48000u));
}

static void note_off(Port *p, int ch, int note) {
    int v;

    for (v = 0; v < NVOICE; v++) {
        if (sVoice[v].port == (int)(p - sPort) && sVoice[v].ch == ch && sVoice[v].note == note) {
            sceSdSetSwitch(0x1600, 1u << v);
            sVoice[v].port = -1;
        }
    }
}

static void note_on(Port *p, int ch, int note, int vel) {
    const unsigned char *h = iop_ptr(p->hd, 0x40), *pc, *pr, *sc, *ss, *mc, *vc, *va;
    unsigned progOff, ssetOff, smplOff, vagiOff;
    int prog = p->ch[ch].prog, s, i;

    if (vel == 0) {
        note_off(p, ch, note);
        return;
    }
    if (h == NULL) {
        return;
    }
    progOff = rd32(h + 0x24);
    ssetOff = rd32(h + 0x28);
    smplOff = rd32(h + 0x2C);
    vagiOff = rd32(h + 0x30);
    pc = iop_ptr(p->hd + progOff, 0x10);
    sc = iop_ptr(p->hd + ssetOff, 0x10);
    mc = iop_ptr(p->hd + smplOff, 0x10);
    vc = iop_ptr(p->hd + vagiOff, 0x10);
    if (pc == NULL || sc == NULL || mc == NULL || vc == NULL || prog > (int)rd32(pc + 0xC)
        || rd32(pc + 0x10 + prog * 4) == 0xFFFFFFFF) {
        return;
    }
    pr = iop_ptr(p->hd + progOff + rd32(pc + 0x10 + prog * 4), 0x24);
    va = vc + 0x10 + (rd32(vc + 0xC) + 1) * 4;
    for (s = 0; s < pr[4]; s++) {
        const unsigned char *sp = pr + rd32(pr) + s * pr[5];

        if (note < sp[2] || note > sp[4]) {
            continue;
        }
        ss = iop_ptr(p->hd + ssetOff + rd32(sc + 0x10 + rd16(sp) * 4), 4);
        if (ss == NULL || vel < ss[1] || vel > ss[2]) {
            continue;
        }
        for (i = 0; i < ss[3]; i++) {
            const unsigned char *sm = iop_ptr(p->hd + smplOff + rd32(mc + 0x10 + rd16(ss + 4 + i * 2) * 4), 0x2A);
            const unsigned char *vg;
            int v, pan;

            if (sm == NULL || vel < sm[2] || vel > sm[4]) {
                continue;
            }
            vg = va + rd16(sm) * 8;
            v = voice_alloc();
            sceSdSetSwitch(0x1600, 1u << v);
            sVoice[v].port = (int)(p - sPort);
            sVoice[v].ch = ch;
            sVoice[v].note = note + (signed char)pr[8] + (signed char)sp[0x12];
            sVoice[v].baseNote = sm[0xB];
            sVoice[v].fine = (signed char)sm[0xC] + (signed char)pr[9] + (signed char)sp[0x13];
            sVoice[v].rate = rd16(vg + 4);
            sVoice[v].bendLo = sp[7];
            sVoice[v].bendHi = sp[9];
            sVoice[v].vol14 = pr[6] * sp[0x10] * sm[0x10] / 170 * vel / 127;
            pan = (signed char)(sm[0xD] - 0x40) + (signed char)(pr[7] - 0x40) + (signed char)(sp[0x11] - 0x40) + 0x40;
            sVoice[v].pan = pan < 0 ? 0 : pan > 127 ? 127 : pan;
            sVoice[v].age = ++sAge;
            sceSdSetAddr(sd_voice(v) | 0x2040, (rd32(vg) + p->spu) & 0x7FFFF0);
            sceSdSetParam(sd_voice(v) | 0x0300, (unsigned short)rd16(sm + 0x12));
            sceSdSetParam(sd_voice(v) | 0x0400, (unsigned short)rd16(sm + 0x14));
            voice_pitch(v);
            voice_volume(v);
            sceSdSetSwitch(0x1500, 1u << v);
        }
    }
}

static void port_voices(Port *p, int ch, void (*f)(int)) {
    int v;

    for (v = 0; v < NVOICE; v++) {
        if (sVoice[v].port == (int)(p - sPort) && (ch < 0 || sVoice[v].ch == ch)) {
            f(v);
        }
    }
}

static void all_off(Port *p) {
    int v;

    for (v = 0; v < NVOICE; v++) {
        if (sVoice[v].port == (int)(p - sPort)) {
            sceSdSetSwitch(0x1600, 1u << v);
            sVoice[v].port = -1;
        }
    }
}

/* ---- the sequencer ---- */

static void port_rewind(Port *p) {
    int i;

    p->pos = p->data;
    p->status = 0;
    p->loopPos = NULL;
    p->usPerQn = 500000;
    for (i = 0; i < 16; i++) {
        p->ch[i].vol = 100;
        p->ch[i].expr = 127;
        p->ch[i].pan = 64;
        p->ch[i].bend = 8192;
    }
    p->wait = p->pos < p->end ? vlq(&p->pos, p->end) : 0;
}

/* one event; 0 at the end */
static int port_event(Port *p) {
    unsigned char d[2];
    int n, i, hi, ch;

    if (p->pos >= p->end) {
        return 0;
    }
    if (*p->pos & 0x80) {
        p->status = *p->pos++;
    }
    if (p->status == 0xFF) {
        int type = p->pos[0], len = p->pos[1];

        if (type == 0x2F) {
            return 0;
        }
        if (type == 0x51 && len == 3) {
            p->usPerQn = (unsigned)p->pos[2] << 16 | p->pos[3] << 8 | p->pos[4];
        }
        p->pos += 2 + len;
        p->wait += vlq(&p->pos, p->end);
        return 1;
    }
    hi = p->status & 0xF0;
    ch = p->status & 0xF;
    n = (hi == 0xC0 || hi == 0xD0 || hi == 0x80) ? 1 : 2;
    for (i = 0; i < n; i++) {
        d[i] = p->pos < p->end ? *p->pos++ : 0;
    }
    if (d[n - 1] & 0x80) {
        d[n - 1] &= 0x7F;
    } else {
        p->wait += vlq(&p->pos, p->end);
    }
    switch (hi) {
    case 0x80:
        note_off(p, ch, d[0]);
        break;
    case 0x90:
        note_on(p, ch, d[0], d[1]);
        break;
    case 0xB0:
        switch (d[0]) {
        case 7:
            p->ch[ch].vol = d[1];
            port_voices(p, ch, voice_volume);
            break;
        case 10:
            p->ch[ch].pan = d[1];
            port_voices(p, ch, voice_volume);
            break;
        case 11:
            p->ch[ch].expr = d[1];
            port_voices(p, ch, voice_volume);
            break;
        case 99:
            p->nrpn = d[1];
            break;
        case 6:   /* NRPN data: 20 loop start (count), 30 loop end */
            if (p->nrpn == 20) {
                p->loopPos = p->pos;
                p->loopCount = d[1];
                p->loopLeft = d[1];
            } else if (p->nrpn == 30 && p->loopPos != NULL) {
                if (p->loopCount == 0 || --p->loopLeft > 0) {
                    p->pos = p->loopPos;
                }
            }
            break;
        case 120:
        case 123:
            all_off(p);
            break;
        }
        break;
    case 0xC0:
        p->ch[ch].prog = d[0];
        break;
    case 0xE0:
        p->ch[ch].bend = d[0] | d[1] << 7;
        port_voices(p, ch, voice_pitch);
        break;
    }
    return 1;
}

/* the sequences on by `sec` seconds (the mixer, before each chunk) */
void seq_tick(double sec) {
    int k;

    for (k = 0; k < NPORT; k++) {
        Port *p = &sPort[k];
        double ticks;

        if (!p->playing || p->data == NULL) {
            continue;
        }
        ticks = sec * 1e6 / p->usPerQn * p->res * p->relTempo / 256.0;
        p->wait -= ticks;
        while (p->wait <= 0 && p->playing) {
            if (!port_event(p)) {
                p->playing = 0;
                all_off(p);
            }
        }
    }
}

/* ---- the driver's commands (snddrv.c) ---- */

void seq_reset(void) {
    int v, k;

    for (v = 0; v < NVOICE; v++) {
        sVoice[v].port = -1;
    }
    for (k = 0; k < NPORT; k++) {
        sPort[k].relTempo = 0x100;
        sPort[k].vol = 127;
        sPort[k].synthVol = 255;
    }
}

/* port `k` gets bank `hd` / `spu` (0x0C) */
void seq_load_bank(int k, unsigned hd, unsigned spu) {
    if (k >= 0 && k < NPORT) {
        sPort[k].hd = hd;
        sPort[k].spu = spu;
    }
}

/* port `k` gets the SQ at IOP `addr` (0x0D) */
int seq_load(int k, unsigned addr) {
    const unsigned char *q = iop_ptr(addr, 0x40), *mid, *blk;
    Port *p;

    if (k < 0 || k >= NPORT || q == NULL || memcmp(q + 0x10, "IECSuqeS", 8) != 0) {
        return -1;
    }
    p = &sPort[k];
    mid = q + 0x30;
    if (memcmp(mid, "IECSidiM", 8) != 0) {
        return -1;
    }
    all_off(p);
    blk = mid + rd32(mid + 0x10);
    p->res = rd16(blk + 4);
    if (p->res == 0) {
        p->res = 480;
    }
    p->data = blk + rd32(blk);
    p->end = mid + rd32(mid + 8);
    p->loaded = 1;
    p->playing = 0;
    port_rewind(p);
    return 0;
}

/* play / stop (0x19 / 0x1A) */
void seq_play(int k, int on) {
    Port *p;

    if (k < 0 || k >= NPORT) {
        return;
    }
    p = &sPort[k];
    if (on) {
        if (!p->playing && p->pos >= p->end) {
            port_rewind(p);
        }
        p->playing = p->data != NULL;
    } else {
        p->playing = 0;
        all_off(p);
    }
}

/* back to the start (0x18 with 0) */
void seq_locate(int k, unsigned pos) {
    if (k >= 0 && k < NPORT && sPort[k].data != NULL) {
        all_off(&sPort[k]);
        port_rewind(&sPort[k]);
        (void)pos;
    }
}

int seq_playing(int k) { return k >= 0 && k < NPORT && sPort[k].playing; }

void seq_set_volume(int k, int v) {
    if (k >= 0 && k < NPORT) {
        sPort[k].vol = v & 0x7F;
        port_voices(&sPort[k], -1, voice_volume);
    }
}

int seq_volume(int k) { return k >= 0 && k < NPORT ? sPort[k].vol : 0; }

void seq_set_synth_volume(int k, int v) {
    if (k >= 0 && k < NPORT) {
        sPort[k].synthVol = v & 0xFF;
        port_voices(&sPort[k], -1, voice_volume);
    }
}

void seq_set_tempo(int k, int t) {
    if (k >= 0 && k < NPORT) {
        sPort[k].relTempo = t & 0xFFFF;
    }
}

int seq_tempo(int k) { return k >= 0 && k < NPORT ? sPort[k].relTempo : 0; }
