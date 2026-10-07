/* The music sequences: see seq.h. */
#include "seq.h"

#include "../core/files.h"
#include "sound.h"

#include <SDL3/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NPORT 4
#define NVOICE 48

typedef struct Chan {
    int prog, vol, expr, pan, bend;
    int rpn;                    /* (CC 101 << 7 | CC 100), 0x3FFF none */
    int bend_range;             /* semitones set by RPN 0, -1: the split's */
    int scale;                  /* the director's channel volume (0..127; driver +0x38) */
} Chan;

typedef struct Port {
    uint8_t *file;
    const uint8_t *data, *end, *pos, *loop_pos;
    int loop_count, loop_left;
    int playing;
    uint8_t status;             /* running status */
    double wait;                /* ticks to the next event */
    int res;                    /* ticks per quarter note */
    unsigned us_per_qn;
    int vol;                    /* the sequence volume (0..255, 128 as written) */
    int port_vol;               /* the synth's 0..255 */
    int nrpn;
    Chan ch[16];
} Port;

enum { ENV_OFF, ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };

typedef struct Voice {
    int port, ch, note;         /* port -1: free (it may still be releasing) */
    int vol14, pan;             /* the note's volume and pan (before the channel's) */
    int base_note, fine, rate, bend_lo, bend_hi;
    unsigned age;
    /* the sound processor's side */
    float gl, gr;
    unsigned pitch;             /* 0x1000: 48 kHz */
    unsigned adsr1, adsr2;
    size_t ssa, lsa, nax;       /* bytes in the .BD */
    int lsa_set;
    int env, level, counter;
    unsigned pos;               /* 12-bit fraction */
    int16_t buf[28];
    int s1, s2, idx, prev, cur;
} Voice;

static struct {
    SDL_Mutex *lock;
    uint8_t *hd, *bd;
    size_t hd_size, bd_size;
    Port port[NPORT];
    Voice voice[NVOICE];
    unsigned age;
    int hooked;
} S;

static unsigned rd32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (unsigned)p[3] << 24; }
static unsigned rd16(const uint8_t *p) { return p[0] | p[1] << 8; }

static void lock(void) {
    if (S.lock == NULL) {
        S.lock = SDL_CreateMutex();
    }
    SDL_LockMutex(S.lock);
}

static void unlock(void) { SDL_UnlockMutex(S.lock); }

/* the header's bytes at off .. off + n, or NULL */
static const uint8_t *hd_at(size_t off, size_t n) {
    return S.hd != NULL && off <= S.hd_size && n <= S.hd_size - off ? S.hd + off : NULL;
}

static unsigned vlq(const uint8_t **pp, const uint8_t *end) {
    unsigned v = 0;

    while (*pp < end) {
        uint8_t b = *(*pp)++;

        v = (v << 7) | (b & 0x7F);
        if (!(b & 0x80)) {
            break;
        }
    }
    return v;
}

/* ---- the sound processor's voices (PS-ADPCM, pitch, ADSR as the hardware: psx-spx) ---- */

static const int kFilter[5][2] = {{0, 0}, {60, 0}, {115, -52}, {98, -55}, {122, -60}};

static void decode_block(Voice *v) {
    const uint8_t *b;
    int shift, f, i;

    if (v->nax + 16 > S.bd_size) {
        v->env = ENV_OFF;
        return;
    }
    b = S.bd + (v->nax & ~(size_t)0xF);
    shift = b[0] & 0xF;
    f = (b[0] >> 4) & 7;
    shift = shift > 12 ? 9 : shift;
    f = f > 4 ? 0 : f;
    if ((b[1] & 4) && !v->lsa_set) {
        v->lsa = v->nax;
    }
    for (i = 0; i < 28; i++) {
        int n = (b[2 + i / 2] >> ((i & 1) * 4)) & 0xF;
        int s = (int16_t)(n << 12) >> shift;

        s += (v->s1 * kFilter[f][0] + v->s2 * kFilter[f][1]) / 64;
        s = s > 32767 ? 32767 : s < -32768 ? -32768 : s;
        v->buf[i] = (int16_t)s;
        v->s2 = v->s1;
        v->s1 = s;
    }
}

static void next_block(Voice *v) {
    const uint8_t *b = S.bd + (v->nax & ~(size_t)0xF);

    if (b[1] & 1) {   /* the end: the loop, or silence */
        if (b[1] & 2) {
            v->nax = v->lsa;
        } else {
            v->env = ENV_OFF;
            v->level = 0;
            return;
        }
    } else {
        v->nax += 16;
    }
    decode_block(v);
}

static void env_step(Voice *v, int exp, int dec, int shift, int step) {
    int cycles = 1 << (shift - 11 > 0 ? shift - 11 : 0);
    int delta = (dec ? -8 + step : 7 - step) << (11 - shift > 0 ? 11 - shift : 0);

    if (exp && !dec && v->level > 0x6000) {
        cycles *= 4;
    }
    if (exp && dec) {
        delta = delta * v->level >> 15;
    }
    if (++v->counter < cycles) {
        return;
    }
    v->counter = 0;
    v->level += delta;
    v->level = v->level > 0x7FFF ? 0x7FFF : v->level < 0 ? 0 : v->level;
}

static void env_tick(Voice *v) {
    unsigned a1 = v->adsr1, a2 = v->adsr2;

    switch (v->env) {
    case ENV_ATTACK:
        env_step(v, a1 >> 15, 0, (a1 >> 10) & 0x1F, (a1 >> 8) & 3);
        if (v->level >= 0x7FFF) {
            v->env = ENV_DECAY;
            v->counter = 0;
        }
        break;
    case ENV_DECAY:
        env_step(v, 1, 1, (a1 >> 4) & 0xF, 0);
        if (v->level <= (int)(((a1 & 0xF) + 1) * 0x800)) {
            v->env = ENV_SUSTAIN;
            v->counter = 0;
        }
        break;
    case ENV_SUSTAIN:
        env_step(v, a2 >> 15, (a2 >> 14) & 1, (a2 >> 8) & 0x1F, (a2 >> 6) & 3);
        break;
    case ENV_RELEASE:
        env_step(v, (a2 >> 5) & 1, 1, a2 & 0x1F, 0);
        if (v->level <= 0) {
            v->env = ENV_OFF;
        }
        break;
    }
}

static void key_on(Voice *v) {
    v->nax = v->ssa;
    v->lsa = v->ssa;
    v->lsa_set = 0;
    v->s1 = v->s2 = 0;
    v->pos = 0;
    v->idx = 0;
    v->prev = 0;
    v->env = ENV_ATTACK;
    v->level = 0;
    v->counter = 0;
    decode_block(v);
    v->cur = v->buf[0];
}

static void key_off(Voice *v) {
    if (v->env != ENV_OFF) {
        v->env = ENV_RELEASE;
        v->counter = 0;
    }
}

static void render(float *out, int frames) {
    int n, i;

    for (n = 0; n < NVOICE; n++) {
        Voice *v = &S.voice[n];

        for (i = 0; i < frames && v->env != ENV_OFF; i++) {
            unsigned step = v->pitch > 0x3FFF ? 0x3FFF : v->pitch;
            float s;

            v->pos += step;
            while (v->pos >= 0x1000) {
                v->pos -= 0x1000;
                v->prev = v->cur;
                if (++v->idx >= 28) {
                    v->idx = 0;
                    next_block(v);
                    if (v->env == ENV_OFF) {
                        break;
                    }
                }
                v->cur = v->buf[v->idx];
            }
            if (v->env == ENV_OFF) {
                break;
            }
            s = (v->prev + (v->cur - v->prev) * (v->pos / 4096.0f)) / 32768.0f * (v->level / 32767.0f);
            out[2 * i] += s * v->gl;
            out[2 * i + 1] += s * v->gr;
            env_tick(v);
        }
    }
}

/* ---- the synth (modhsyn) ---- */

static int voice_alloc(void) {
    int v, best = 0;

    for (v = 0; v < NVOICE; v++) {
        if (S.voice[v].port < 0 && S.voice[v].env == ENV_OFF) {
            return v;
        }
    }
    for (v = 0; v < NVOICE; v++) {
        if (S.voice[v].port < 0 && S.voice[v].age < S.voice[best].age) {
            best = v;
        }
    }
    if (S.voice[best].port < 0) {
        return best;
    }
    for (v = 0; v < NVOICE; v++) {
        if (S.voice[v].age < S.voice[best].age) {
            best = v;
        }
    }
    return best;
}

static void voice_volume(int v) {
    Voice *sv = &S.voice[v];
    Port *p = &S.port[sv->port];
    Chan *c = &p->ch[sv->ch];
    int x = sv->vol14 * c->vol / 127 * c->expr / 127 * p->vol / 128 * p->port_vol / 255 * c->scale / 127;
    int pan = sv->pan + (c->pan - 64), l, r;

    pan = pan < 0 ? 0 : pan > 127 ? 127 : pan;
    x = x > 0x3FFF ? 0x3FFF : x;
    if (pan < 0x40) {
        l = x;
        r = x * pan * 2 >> 7;
    } else if (pan > 0x40) {
        r = x;
        l = x * (0x100 - (pan == 0x7F ? 0x80 : pan) * 2) >> 7;
    } else {
        l = r = x;
    }
    sv->gl = (l & 0x3FFF) / 16384.0f;
    sv->gr = (r & 0x3FFF) / 16384.0f;
}

/* sceSdNote2Pitch: 0x1000 at the sample's own note */
static unsigned note_pitch(int base, int note, int fine) {
    double p = 4096.0 * pow(2.0, ((note - base) + fine / 128.0) / 12.0);

    return p > 0x3FFF ? 0x3FFF : (unsigned)p;
}

static void voice_pitch(int v) {
    Voice *sv = &S.voice[v];
    Chan *c = &S.port[sv->port].ch[sv->ch];
    int b = c->bend - 8192;
    int hi = c->bend_range >= 0 ? c->bend_range : sv->bend_hi, lo = c->bend_range >= 0 ? c->bend_range : sv->bend_lo;
    double semis = b >= 0 ? b / 8192.0 * hi : b / 8192.0 * lo;

    sv->pitch = note_pitch(sv->base_note, sv->note, sv->fine + (int)lround(semis * 128.0)) * (unsigned)sv->rate / 48000u;
}

static void note_off(Port *p, int ch, int note) {
    int v;

    for (v = 0; v < NVOICE; v++) {
        if (S.voice[v].port == (int)(p - S.port) && S.voice[v].ch == ch && S.voice[v].note == note) {
            key_off(&S.voice[v]);
            S.voice[v].port = -1;
        }
    }
}

/* program `prog`'s splits that take the note and velocity: a voice for each sample of each */
static void note_on(Port *p, int ch, int note, int vel) {
    const uint8_t *h = hd_at(0, 0x40), *pc, *pr, *sc, *mc, *vc, *va;
    unsigned prog_off, sset_off, smpl_off, vagi_off;
    int prog = p->ch[ch].prog, s, i;

    if (vel == 0) {
        note_off(p, ch, note);
        return;
    }
    if (h == NULL || S.bd == NULL) {
        return;
    }
    prog_off = rd32(h + 0x24);
    sset_off = rd32(h + 0x28);
    smpl_off = rd32(h + 0x2C);
    vagi_off = rd32(h + 0x30);
    pc = hd_at(prog_off, 0x10);
    sc = hd_at(sset_off, 0x10);
    mc = hd_at(smpl_off, 0x10);
    vc = hd_at(vagi_off, 0x10);
    if (pc == NULL || sc == NULL || mc == NULL || vc == NULL || prog > (int)rd32(pc + 0xC) ||
        hd_at(prog_off + 0x10 + prog * 4, 4) == NULL || rd32(pc + 0x10 + prog * 4) == 0xFFFFFFFFu) {
        return;
    }
    pr = hd_at(prog_off + rd32(pc + 0x10 + prog * 4), 0x24);
    if (pr == NULL) {
        return;
    }
    va = vc + 0x10 + (rd32(vc + 0xC) + 1) * 4;
    for (s = 0; s < pr[4]; s++) {
        const uint8_t *sp = hd_at((size_t)(pr - S.hd) + rd32(pr) + s * pr[5], 0x14), *ss;

        if (sp == NULL || note < sp[2] || note > sp[4] || hd_at(sset_off + 0x10 + rd16(sp) * 4, 4) == NULL) {
            continue;
        }
        ss = hd_at(sset_off + rd32(sc + 0x10 + rd16(sp) * 4), 4);
        if (ss == NULL || vel < ss[1] || vel > ss[2]) {
            continue;
        }
        for (i = 0; i < ss[3]; i++) {
            const uint8_t *sm, *vg;
            int v, pan;

            if (hd_at(smpl_off + 0x10 + rd16(ss + 4 + i * 2) * 4, 4) == NULL) {
                continue;
            }
            sm = hd_at(smpl_off + rd32(mc + 0x10 + rd16(ss + 4 + i * 2) * 4), 0x2A);
            if (sm == NULL || vel < sm[2] || vel > sm[4]) {
                continue;
            }
            vg = hd_at((size_t)(va - S.hd) + rd16(sm) * 8, 8);
            if (vg == NULL) {
                continue;
            }
            v = voice_alloc();
            S.voice[v].port = (int)(p - S.port);
            S.voice[v].ch = ch;
            S.voice[v].note = note + (int8_t)pr[8] + (int8_t)sp[0x12];
            S.voice[v].base_note = sm[0xB];
            S.voice[v].fine = (int8_t)sm[0xC] + (int8_t)pr[9] + (int8_t)sp[0x13];
            S.voice[v].rate = (int)rd16(vg + 4);
            S.voice[v].bend_lo = sp[7];
            S.voice[v].bend_hi = sp[9];
            S.voice[v].vol14 = pr[6] * sp[0x10] * sm[0x10] / 170 * vel / 127;
            pan = (int8_t)(sm[0xD] - 0x40) + (int8_t)(pr[7] - 0x40) + (int8_t)(sp[0x11] - 0x40) + 0x40;
            S.voice[v].pan = pan < 0 ? 0 : pan > 127 ? 127 : pan;
            S.voice[v].age = ++S.age;
            S.voice[v].ssa = rd32(vg) & ~(size_t)0xF;
            S.voice[v].adsr1 = rd16(sm + 0x12);
            S.voice[v].adsr2 = rd16(sm + 0x14);
            voice_pitch(v);
            voice_volume(v);
            key_on(&S.voice[v]);
        }
    }
}

static void port_voices(Port *p, int ch, void (*f)(int)) {
    int v;

    for (v = 0; v < NVOICE; v++) {
        if (S.voice[v].port == (int)(p - S.port) && (ch < 0 || S.voice[v].ch == ch)) {
            f(v);
        }
    }
}

static void all_off(Port *p) {
    int v;

    for (v = 0; v < NVOICE; v++) {
        if (S.voice[v].port == (int)(p - S.port)) {
            key_off(&S.voice[v]);
            S.voice[v].port = -1;
        }
    }
}

/* ---- the sequencer (modmidi) ---- */

static void chan_defaults(Chan *c) {
    c->vol = 100;
    c->expr = 127;
    c->pan = 64;
    c->bend = 8192;
    c->rpn = 0x3FFF;
    c->bend_range = -1;
}

static void port_rewind(Port *p) {
    int i;

    p->pos = p->data;
    p->status = 0;
    p->loop_pos = NULL;
    p->us_per_qn = 500000;
    for (i = 0; i < 16; i++) {
        chan_defaults(&p->ch[i]);
    }
    p->wait = p->pos < p->end ? vlq(&p->pos, p->end) : 0;
}

static void channel_msg(Port *p, int hi, int ch, int d0, int d1) {
    switch (hi) {
    case 0x80:
        note_off(p, ch, d0);
        break;
    case 0x90:
        note_on(p, ch, d0, d1);
        break;
    case 0xB0:
        switch (d0) {
        case 7:
            p->ch[ch].vol = d1;
            port_voices(p, ch, voice_volume);
            break;
        case 10:
            p->ch[ch].pan = d1;
            port_voices(p, ch, voice_volume);
            break;
        case 11:
            p->ch[ch].expr = d1;
            port_voices(p, ch, voice_volume);
            break;
        case 101:
            p->ch[ch].rpn = (p->ch[ch].rpn & 0x7F) | d1 << 7;
            break;
        case 100:
            p->ch[ch].rpn = (p->ch[ch].rpn & ~0x7F) | d1;
            break;
        case 99:
            p->nrpn = d1;
            p->ch[ch].rpn = 0x3FFF;
            break;
        case 6:   /* data entry: RPN 0 the bend range; NRPN 20 a loop's start (its count), 30 its end */
            if (p->ch[ch].rpn == 0) {
                p->ch[ch].bend_range = d1;
            } else if (p->nrpn == 20) {
                p->loop_pos = p->pos;
                p->loop_count = d1;
                p->loop_left = d1;
            } else if (p->nrpn == 30 && p->loop_pos != NULL) {
                if (p->loop_count == 0 || --p->loop_left > 0) {
                    p->pos = p->loop_pos;
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
        p->ch[ch].prog = d0;
        break;
    case 0xE0:
        p->ch[ch].bend = d0 | d1 << 7;
        port_voices(p, ch, voice_pitch);
        break;
    }
}

static int port_event(Port *p) {
    uint8_t d[2] = {0, 0};
    int n, i, hi, ch;

    if (p->pos >= p->end) {
        return 0;
    }
    if (*p->pos & 0x80) {
        p->status = *p->pos++;
    }
    if (p->status == 0xFF) {   /* meta: the end, the tempo */
        int type, len;

        if (p->pos + 2 > p->end) {
            return 0;
        }
        type = p->pos[0];
        len = p->pos[1];
        if (type == 0x2F) {
            return 0;
        }
        if (type == 0x51 && len == 3 && p->pos + 5 <= p->end) {
            p->us_per_qn = (unsigned)p->pos[2] << 16 | p->pos[3] << 8 | p->pos[4];
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
    channel_msg(p, hi, ch, d[0], d[1]);
    return 1;
}

static void tick(double sec) {
    int k;

    for (k = 0; k < NPORT; k++) {
        Port *p = &S.port[k];

        if (!p->playing || p->data == NULL) {
            continue;
        }
        p->wait -= sec * 1e6 / p->us_per_qn * p->res;
        while (p->wait <= 0 && p->playing) {
            if (!port_event(p)) {   /* (the stage music loops by its marks; past the end: over) */
                p->playing = 0;
                all_off(p);
            }
        }
    }
}

static void mix(int16_t *out, int frames) {
    float buf[2048];
    int done = 0, i;

    lock();
    while (done < frames) {
        int n = frames - done > 1024 ? 1024 : frames - done;

        tick((double)n / SOUND_RATE);
        memset(buf, 0, sizeof(float) * (size_t)n * 2);
        render(buf, n);
        for (i = 0; i < n * 2; i++) {
            int v = out[done * 2 + i] + (int)(buf[i] * 32767.0f);

            out[done * 2 + i] = (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
        }
        done += n;
    }
    unlock();
}

/* ---- the driver's side ---- */

static void hook(void) {
    if (!S.hooked) {
        int v;

        S.hooked = 1;
        for (v = 0; v < NVOICE; v++) {
            S.voice[v].port = -1;
        }
        for (v = 0; v < NPORT; v++) {
            int c;

            S.port[v].vol = 128;
            S.port[v].port_vol = 255;
            for (c = 0; c < 16; c++) {
                S.port[v].ch[c].scale = 127;
            }
        }
        sound_stream_slot(2, mix);
    }
}

int seq_bank(const char *name) {
    char path[128];
    uint8_t *hd, *bd;
    size_t hs, bs;

    snprintf(path, sizeof(path), "%s.HD", name);
    hd = files_read(path, &hs);
    snprintf(path, sizeof(path), "%s.BD", name);
    bd = files_read(path, &bs);
    if (hd == NULL || bd == NULL) {
        free(hd);
        free(bd);
        return 0;
    }
    lock();
    hook();
    free(S.hd);
    free(S.bd);
    S.hd = hd;
    S.hd_size = hs;
    S.bd = bd;
    S.bd_size = bs;
    unlock();
    return 1;
}

int seq_load(int k, const char *path) {
    size_t size;
    uint8_t *q;
    const uint8_t *mid, *blk;
    Port *p;

    if (k < 0 || k >= NPORT) {
        return 0;
    }
    q = files_read(path, &size);
    if (q == NULL || size < 0x50 || memcmp(q + 0x10, "IECSuqeS", 8) != 0 || memcmp(q + 0x30, "IECSidiM", 8) != 0) {
        free(q);
        return 0;
    }
    mid = q + 0x30;
    blk = mid + rd32(mid + 0x10);
    if (blk + 8 > q + size) {
        free(q);
        return 0;
    }
    lock();
    hook();
    p = &S.port[k];
    all_off(p);
    free(p->file);
    p->file = q;
    p->res = (int)rd16(blk + 4);
    p->res = p->res == 0 ? 480 : p->res;
    p->data = blk + rd32(blk);
    p->end = mid + rd32(mid + 8);
    if (p->end > q + size) {
        p->end = q + size;
    }
    p->playing = 0;
    port_rewind(p);
    unlock();
    return 1;
}

void seq_play(int k, int on) {
    Port *p;

    if (k < 0 || k >= NPORT) {
        return;
    }
    lock();
    p = &S.port[k];
    all_off(p);
    if (on && p->data != NULL) {
        int vol = p->vol;

        port_rewind(p);
        p->vol = vol;
        p->playing = 1;
    } else {
        p->playing = 0;
    }
    unlock();
}

int seq_playing(int k) {
    int on;

    lock();
    on = k >= 0 && k < NPORT && S.port[k].playing;
    unlock();
    return on;
}

void seq_volume(int k, int v) {
    if (k >= 0 && k < NPORT) {
        lock();
        S.port[k].vol = v < 0 ? 0 : v > 255 ? 255 : v;
        port_voices(&S.port[k], -1, voice_volume);
        unlock();
    }
}

void seq_port_volume(int k, int v) {
    if (k >= 0 && k < NPORT) {
        lock();
        S.port[k].port_vol = v < 0 ? 0 : v > 255 ? 255 : v;
        port_voices(&S.port[k], -1, voice_volume);
        unlock();
    }
}

void seq_chan_volume(int k, int ch, int v) {
    if (k >= 0 && k < NPORT && ch >= 0 && ch < 16) {
        lock();
        S.port[k].ch[ch].scale = v < 0 ? 0 : v > 127 ? 127 : v;
        port_voices(&S.port[k], ch, voice_volume);
        unlock();
    }
}

void seq_midi(int k, int status, int d1, int d2) {
    if (k < 0 || k >= NPORT || !(status & 0x80) || status >= 0xF0) {
        return;
    }
    lock();
    channel_msg(&S.port[k], status & 0xF0, status & 0xF, d1 & 0x7F, d2 & 0x7F);
    unlock();
}

void seq_reset(void) {
    int k;

    lock();
    for (k = 0; k < NPORT; k++) {
        all_off(&S.port[k]);
        S.port[k].playing = 0;
        free(S.port[k].file);
        S.port[k].file = NULL;
        S.port[k].data = S.port[k].end = S.port[k].pos = NULL;
    }
    for (k = 0; k < NVOICE; k++) {
        S.voice[k].env = ENV_OFF;
    }
    unlock();
}
